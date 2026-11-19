// Netplay, lockstep on the pads (like Dolphin's): every machine runs the whole game from the same boot state
// (same disc region, build, RNG seed, memory card and logic-relevant settings, all sent by the host), and the
// game only ever sees the pads every machine agreed on for that frame. A machine's pads read at frame f are the
// game's pads of frame f + delay, which hides the round trip to the host. Star topology: clients send their
// pads to the host, the host sends every complete frame to everyone.
//
// A "frame" is one pad VSync callback (PadReadVSync, once per VIWaitForRetrace), the game's own tick.
//
// Every kHashEvery frames each machine hashes the game's work state (GwCommon/GwSystem/GwPlayer, RNG seed);
// the host compares, and a mismatch is logged with a dump of that state on every machine (desync).
//
// shortcut: TCP for everything (reliable and ordered, so no resend logic); the rollback milestone moves the pads
// to UDP with redundancy, where a lost packet costs nothing.
// MusyX normally renders on its own thread, paced by the sound card, and the game reads the synth's state (has
// this sound ended?): that would differ between machines. In a session the synth renders exactly 1/60 s of
// audio per frame on the game's thread instead (sndPCRender has no wall-clock dependence).
//
// shortcut: no mid-game join, no reconnection: a dropped peer ends the session, the game goes on locally.
#include "port/netplay.h"
#include <dolphin/os.h>
// CMake renames the game's clock reads to the functions below; this file defines them and needs the real ones
#undef OSGetTick
#undef OSGetTime
extern "C" {
OSTick OSGetTick(void);
OSTime OSGetTime(void);
OSTime Mp5Net_OSGetTime(void);
OSTick Mp5Net_OSGetTick(void);
}

#include "port/settings.h"
#include "port/ui.h"
#include "iso_validate.hpp"
#include "mp5native_version.h"

#include <aurora/lib/logging.hpp>
#include <musyx/pc.h>
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_timer.h>

#include <algorithm>
#include <array>
#include <bit>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <ctime>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using sock_t = SOCKET;
static int sockErr() { return WSAGetLastError(); }
static bool wouldBlock(int e) { return e == WSAEWOULDBLOCK; }
constexpr int kSendFlags = 0;
#else
#include <arpa/inet.h>
#include <cerrno>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
using sock_t = int;
#define INVALID_SOCKET (-1)
#define closesocket close
static int sockErr() { return errno; }
static bool wouldBlock(int e) { return e == EAGAIN || e == EWOULDBLOCK || e == EINPROGRESS; }
constexpr int kSendFlags = MSG_NOSIGNAL; // a peer gone must not kill the game with SIGPIPE
#endif


extern "C" {
#include <game/gamework.h>
u32* Mp5Test_FrandSeed(void);
extern s32 rnd_seed;
extern int omcurovl; // object.c (OMOVL); game/object.h is not C++-clean
void CARDSetBasePath(const char* path, s32 chan);
}

namespace {

std::filesystem::path Mp5Native_ConfigPath; // set by portmain.cpp (Mp5Net_SetConfigPath)

aurora::Module Log("mp5native::netplay");

constexpr u32 kProto = 2;
constexpr u16 kDefaultPort = 51525;
constexpr u32 kHashEvery = 60;
constexpr u64 kTimeoutMs = 10000;
const char kBuild[] = MP5_NATIVE_WC_DESCRIBE " " MP5_NATIVE_PLATFORM_NAME " " MP5_NATIVE_ARCH;

enum : u8 { MsgHello = 1, MsgStart, MsgReject, MsgInput, MsgFrame, MsgHash, MsgDesync };

struct Peer {
    sock_t s = INVALID_SOCKET;
    std::vector<u8> rx;
    int firstPort = 0, nPorts = 0;
};

struct Slot {
    std::array<PADStatus, 4> pad{};
    u8 have = 0; // bit per port received
};

bool sActive, sHost;
int sDelay;
std::vector<Peer> sPeers; // host: its clients; client: the host
int sMyFirstPort, sMyPorts;
u8 sPortMask;
u32 sFrame, sNextBroadcast;
std::map<u32, Slot> sFrames;
std::map<u32, u32> sHostHash;            // host: its own hashes
std::multimap<u32, u32> sPendingHash;    // host: client hashes not yet compared
std::map<u32, std::vector<u8>> sStates;  // the last few hashed states, dumped on a desync
bool sAudioOn;
SDL_AudioStream* sAudio; // null without a device: the synth still runs, the game waits on it
u64 sAudioFrames;        // game frames rendered

OSTime sBaseTime, sLastTime; // session clock (Mp5Net_OSGetTime)
FILE* sLogFile;      // <config>/netplay_<role>.log, one per machine: compare them side by side
u64 sWaitMs, sWaitMax; // time spent waiting for the network since the last report

// Logged, and written at once to this machine's netplay log with the time and game frame.
template <typename... Args> void note(bool error, fmt::format_string<Args...> f, Args&&... args) {
    const std::string line = fmt::format(f, std::forward<Args>(args)...);
    if (error) {
        Log.error("{}", line);
    } else {
        Log.info("{}", line);
    }
    if (sLogFile != nullptr) {
        const time_t now = time(nullptr);
        char stamp[16];
        strftime(stamp, sizeof(stamp), "%H:%M:%S", localtime(&now));
        fmt::println(sLogFile, "{} frame {:>7} {}{}", stamp, sFrame, error ? "ERROR " : "", line);
        fflush(sLogFile);
    }
}

// --- wire -------------------------------------------------------------------------------------------------

struct Writer {
    std::vector<u8> b;
    template <typename T> void put(const T& v) { b.insert(b.end(), (const u8*)&v, (const u8*)&v + sizeof(T)); }
    void bytes(const void* p, size_t n) { b.insert(b.end(), (const u8*)p, (const u8*)p + n); }
};
struct Reader {
    const u8* p; size_t n; bool ok = true;
    template <typename T> T get() { T v{}; read(&v, sizeof(T)); return v; }
    void read(void* out, size_t len) {
        if (len > n) { ok = false; n = 0; return; }
        memcpy(out, p, len); p += len; n -= len;
    }
};

void setNonBlocking(sock_t s) {
#ifdef _WIN32
    u_long on = 1;
    ioctlsocket(s, FIONBIO, &on);
#else
    fcntl(s, F_SETFL, fcntl(s, F_GETFL, 0) | O_NONBLOCK);
#endif
    int one = 1;
    setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (const char*)&one, sizeof(one));
}

bool waitSocket(sock_t s, bool write, int ms) {
    fd_set set;
    FD_ZERO(&set);
    FD_SET(s, &set);
    timeval tv{ms / 1000, (ms % 1000) * 1000};
    return select((int)s + 1, write ? nullptr : &set, write ? &set : nullptr, nullptr, &tv) > 0;
}

bool sendMsg(Peer& peer, u8 type, const std::vector<u8>& payload) {
    Writer w;
    w.put(type);
    w.put((u32)payload.size());
    w.bytes(payload.data(), payload.size());
    size_t off = 0;
    const u64 start = SDL_GetTicks();
    while (off < w.b.size()) {
        const int r = send(peer.s, (const char*)w.b.data() + off, (int)(w.b.size() - off), kSendFlags);
        if (r > 0) {
            off += r;
        } else if (r < 0 && wouldBlock(sockErr()) && SDL_GetTicks() - start < kTimeoutMs) {
            waitSocket(peer.s, true, 10);
        } else {
            return false;
        }
    }
    return true;
}

// Reads what arrived; false when the connection is gone.
bool recvSome(Peer& peer) {
    static u8 buf[16384]; // not on the stack: this can run on a game process's small coroutine stack
    for (;;) {
        const int r = recv(peer.s, (char*)buf, sizeof(buf), 0);
        if (r > 0) {
            peer.rx.insert(peer.rx.end(), buf, buf + r);
        } else if (r < 0 && wouldBlock(sockErr())) {
            return true;
        } else {
            return false;
        }
    }
}

// Pops one complete message off peer.rx.
bool popMsg(Peer& peer, u8& type, std::vector<u8>& payload) {
    if (peer.rx.size() < 5) {
        return false;
    }
    u32 len;
    memcpy(&len, peer.rx.data() + 1, 4);
    if (peer.rx.size() < 5 + (size_t)len) {
        return false;
    }
    type = peer.rx[0];
    payload.assign(peer.rx.begin() + 5, peer.rx.begin() + 5 + len);
    peer.rx.erase(peer.rx.begin(), peer.rx.begin() + 5 + len);
    return true;
}

// Waits up to ms for one message from peer, keeping the window alive meanwhile. ms < 0: no time limit.
bool waitMsg(Peer& peer, u8& type, std::vector<u8>& payload, int ms) {
    const u64 start = SDL_GetTicks();
    while (!popMsg(peer, type, payload)) {
        if (ms >= 0 && SDL_GetTicks() - start > (u64)ms) {
            return false;
        }
        SDL_PumpEvents();
        if (waitSocket(peer.s, false, 16) && !recvSome(peer)) {
            return false;
        }
    }
    return true;
}

void closeAll() {
    for (Peer& p : sPeers) {
        closesocket(p.s);
    }
    sPeers.clear();
    sActive = false;
    if (sAudioOn) { // back to MusyX's own audio thread
        sAudioOn = false;
        SDL_DestroyAudioStream(sAudio);
        sAudio = nullptr;
        sndPCStartAudio();
    }
}

// --- game state -------------------------------------------------------------------------------------------

std::vector<u8> captureState() {
    Writer w;
    w.put(GwCommon);
    w.put(GwSystem);
    w.put(GwPlayer);
    w.put(GwPlayerConf);
    w.put(*Mp5Test_FrandSeed());
    w.put(rnd_seed); // rand8 (main.c), the game's second generator
    return w.b;
}

u32 crc32(const std::vector<u8>& data) {
    u32 c = 0xFFFFFFFF;
    for (u8 byte : data) {
        c ^= byte;
        for (int k = 0; k < 8; k++) {
            c = (c >> 1) ^ (0xEDB88320 & (0u - (c & 1)));
        }
    }
    return ~c;
}

void reportDesync(u32 frame) {
    static bool reported = false; // the first one is what matters: everything after it differs anyway
    if (reported) {
        return;
    }
    reported = true;
    note(true, "DESYNC at frame {}: the machines no longer run the same game", frame);
    ui_push_toast("controller", "Online play: desync", "The machines no longer run the same game. See netplay_*.log.", 8000);
    fflush(stdout);
    fflush(stderr);
    auto it = sStates.find(frame);
    if (it != sStates.end()) {
        const auto path = Mp5Native_ConfigPath / fmt::format("netplay_desync_{}_{}.bin", frame, sHost ? "host" : "client");
        std::ofstream(path, std::ios::binary).write((const char*)it->second.data(), it->second.size());
        note(true, "state of frame {} dumped to {}", frame, path.string());
    }
}

// Host: a client's hash or its own, compared as soon as both exist.
void checkHashes() {
    for (auto it = sPendingHash.begin(); it != sPendingHash.end();) {
        auto own = sHostHash.find(it->first);
        if (own == sHostHash.end()) {
            ++it;
            continue;
        }
        if (own->second != it->second) {
            Writer w;
            w.put(it->first);
            for (Peer& p : sPeers) {
                sendMsg(p, MsgDesync, w.b);
            }
            reportDesync(it->first);
        } else if (it->first % 600 == 0) {
            note(false, "frame {}: in sync with a player", it->first);
        }
        it = sPendingHash.erase(it);
    }
}

// --- frames -----------------------------------------------------------------------------------------------

void storePart(u32 frame, int firstPort, int n, const PADStatus* pads) {
    Slot& slot = sFrames[frame];
    for (int i = 0; i < n; i++) {
        slot.pad[firstPort + i] = pads[i];
        slot.have |= 1 << (firstPort + i);
    }
}

// Host: sends every frame that just became complete, in order.
void broadcastComplete() {
    for (;;) {
        auto it = sFrames.find(sNextBroadcast);
        if (it == sFrames.end() || it->second.have != sPortMask) {
            return;
        }
        Writer w;
        w.put(sNextBroadcast);
        w.put(it->second.pad);
        for (Peer& p : sPeers) {
            sendMsg(p, MsgFrame, w.b);
        }
        sNextBroadcast++;
    }
}

void handle(Peer& peer, u8 type, const std::vector<u8>& payload) {
    Reader r{payload.data(), payload.size()};
    if (type == MsgInput && sHost) {
        const u32 frame = r.get<u32>();
        PADStatus pads[4];
        const u8 n = r.get<u8>();
        // a client only sets its own ports, and only frames not sent to everyone yet
        if (n != peer.nPorts || frame < sNextBroadcast || frame > sNextBroadcast + 600) {
            return;
        }
        r.read(pads, sizeof(PADStatus) * n);
        if (r.ok) {
            storePart(frame, peer.firstPort, n, pads);
            broadcastComplete();
        }
    } else if (type == MsgFrame && !sHost) {
        const u32 frame = r.get<u32>();
        Slot slot;
        r.read(slot.pad.data(), sizeof(slot.pad));
        slot.have = sPortMask;
        if (r.ok) {
            sFrames[frame] = slot;
        }
    } else if (type == MsgHash && sHost) {
        const u32 frame = r.get<u32>();
        const u32 crc = r.get<u32>();
        if (r.ok) {
            sPendingHash.emplace(frame, crc);
            checkHashes();
        }
    } else if (type == MsgDesync && !sHost) {
        reportDesync(r.get<u32>());
    }
}

void drain(Peer& peer) {
    u8 type;
    std::vector<u8> payload;
    while (popMsg(peer, type, payload)) {
        handle(peer, type, payload);
    }
}

// Reads from every peer for up to ms; false when one of them is gone.
bool pump(int ms) {
    for (Peer& p : sPeers) { // what the lobby already received (a frame right behind the start message)
        drain(p);
    }
    fd_set set;
    FD_ZERO(&set);
    sock_t maxS = 0;
    for (Peer& p : sPeers) {
        FD_SET(p.s, &set);
        maxS = std::max(maxS, p.s);
    }
    timeval tv{0, ms * 1000};
    if (select((int)maxS + 1, &set, nullptr, nullptr, &tv) <= 0) {
        return true;
    }
    for (Peer& p : sPeers) {
        if (FD_ISSET(p.s, &set)) {
            if (!recvSome(p)) {
                return false;
            }
            drain(p);
        }
    }
    return true;
}

// --- session setup ----------------------------------------------------------------------------------------

struct StartInfo {
    u32 seed;
    u8 delay, firstPort, nPorts, portMask;
    u8 language, quickTransform, infiniteCoins, cheatCodes, unlockMg, unlockBowser, speedrun, recording;
    s64 baseTime; // the host's clock at the start: the game's clock in the session starts there
};

// Settings that change what the game does (not how it looks or sounds) follow the host.
void applySettings(const StartInfo& si) {
    auto& g = mp5native::getSettings().game;
    g.language.setOverrideValue((mp5native::GameLanguage)si.language);
    g.enableQuickTransform.setOverrideValue(si.quickTransform);
    g.infiniteCoins.setOverrideValue(si.infiniteCoins);
    g.enableCheatCodes.setOverrideValue(si.cheatCodes);
    g.unlockAllMinigames.setOverrideValue(si.unlockMg);
    g.unlockBowsersGnarlyParty.setOverrideValue(si.unlockBowser);
    g.speedrunMode.setOverrideValue(si.speedrun);
    g.recordingMode.setOverrideValue(si.recording);
    // real-time disc timing, pausing on focus loss and turbo would let one machine run differently
    g.loadingSpeed.setOverrideValue(0);
    g.pauseOnFocusLost.setOverrideValue(false);
    g.enableTurboKeybind.setOverrideValue(false);
    *Mp5Test_FrandSeed() = si.seed;
    sBaseTime = si.baseTime;
    sLastTime = 0;
}

// The host's memory cards (GCI folders <config>/<region>/Card A|B), so every machine boots on the same save.
void packCards(Writer& w) {
    std::vector<std::pair<std::string, std::vector<u8>>> files;
    std::error_code ec;
    for (const auto& region : std::filesystem::directory_iterator(Mp5Native_ConfigPath, ec)) {
        for (const char* slot : {"Card A", "Card B"}) {
            for (const auto& f : std::filesystem::directory_iterator(region.path() / slot, ec)) {
                std::ifstream in(f.path(), std::ios::binary);
                std::vector<u8> data((std::istreambuf_iterator<char>(in)), {});
                files.emplace_back(std::filesystem::relative(f.path(), Mp5Native_ConfigPath).generic_string(), std::move(data));
            }
        }
    }
    w.put((u16)files.size());
    for (const auto& [name, data] : files) {
        w.put((u16)name.size());
        w.bytes(name.data(), name.size());
        w.put((u32)data.size());
        w.bytes(data.data(), data.size());
    }
}

// Client: the host's cards go to <config>/netplay, which replaces the player's own cards for this run.
bool unpackCards(Reader& r) {
    const auto dir = Mp5Native_ConfigPath / "netplay";
    std::error_code ec;
    std::filesystem::remove_all(dir, ec);
    const u16 count = r.get<u16>();
    for (u16 i = 0; i < count && r.ok; i++) {
        std::string name(r.get<u16>(), '\0');
        r.read(name.data(), name.size());
        std::vector<u8> data(r.get<u32>());
        r.read(data.data(), data.size());
        const auto path = (dir / std::filesystem::path(name)).lexically_normal();
        // the name comes from the network: it must stay inside the netplay folder
        if (!r.ok || name.find("..") != std::string::npos || std::filesystem::path(name).is_absolute()) {
            return false;
        }
        std::filesystem::create_directories(path.parent_path(), ec);
        std::ofstream(path, std::ios::binary).write((const char*)data.data(), data.size());
    }
    std::filesystem::create_directories(dir, ec);
    CARDSetBasePath(dir.string().c_str(), -1);
    return r.ok;
}

int envInt(const char* name, int def) {
    const char* v = getenv(name);
    return v != nullptr && *v != 0 ? atoi(v) : def;
}

// --- lobby: before the game boots, polled by the UI (ui/netplay.cpp) or in a loop by Mp5Net_Boot ----------

constexpr u16 kDiscoveryPort = 51526;
const char kDiscoverAsk[] = "MP5NET?";

NetLobby sLobby = NetLobby::Off;
std::string sStatus;
sock_t sListen = INVALID_SOCKET, sDiscovery = INVALID_SOCKET;
u16 sHostPort;
bool sPal;
std::vector<Peer> sPending; // host: connected, Hello not read yet

int nextFreePort() {
    int port = sMyPorts;
    for (const Peer& p : sPeers) {
        port += p.nPorts;
    }
    return port;
}

bool discPal() { return mp5native::iso::isPal(mp5native::getSettings().backend.isoPath.getValue().c_str()); }

void netInit() {
#ifdef _WIN32
    static bool done = false;
    if (!done) {
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);
        done = true;
    }
#endif
}

void fail(std::string why) {
    Log.error("{}", why);
    Mp5Net_Leave();
    sStatus = std::move(why);
    sLobby = NetLobby::Failed;
}

void hostStatus() {
    sStatus = fmt::format("Hosting on port {}: {} player(s) joined, pad ports 1-{} in use", sHostPort, sPeers.size(),
                          nextFreePort());
}

// Host: a newcomer's Hello, accepted or refused.
void admit(Peer peer, const std::vector<u8>& payload) {
    Reader r{payload.data(), payload.size()};
    const u32 proto = r.get<u32>();
    char build[96] = {};
    r.read(build, sizeof(build) - 1);
    const u8 peerPal = r.get<u8>();
    const u8 nLocal = r.get<u8>();
    const int port = nextFreePort();
    std::string reason;
    if (!r.ok || proto != kProto || strcmp(build, kBuild) != 0) {
        reason = fmt::format("different build (host: {})", kBuild);
    } else if ((peerPal != 0) != sPal) {
        reason = "different disc region";
    } else if (nLocal < 1 || port + nLocal > 4) {
        reason = "no pad port left";
    }
    if (!reason.empty()) {
        Log.warn("player refused: {}", reason);
        sendMsg(peer, MsgReject, std::vector<u8>(reason.begin(), reason.end()));
        closesocket(peer.s);
        return;
    }
    peer.firstPort = port;
    peer.nPorts = nLocal;
    Log.info("player {} joined on pad port(s) {}..{}", sPeers.size() + 1, port + 1, port + nLocal);
    sPeers.push_back(std::move(peer));
    hostStatus();
}

void pollHost() {
    while (waitSocket(sListen, false, 0)) {
        Peer peer;
        peer.s = accept(sListen, nullptr, nullptr);
        if (peer.s == INVALID_SOCKET) {
            break;
        }
        setNonBlocking(peer.s);
        sPending.push_back(std::move(peer));
    }
    for (size_t i = 0; i < sPending.size();) {
        u8 type = 0;
        std::vector<u8> payload;
        const bool alive = recvSome(sPending[i]);
        if (alive && popMsg(sPending[i], type, payload) && type == MsgHello) {
            Peer peer = std::move(sPending[i]);
            sPending.erase(sPending.begin() + i);
            admit(std::move(peer), payload);
        } else if (!alive || type != 0 || (!sPending[i].rx.empty() && sPending[i].rx[0] != MsgHello)) {
            closesocket(sPending[i].s);
            sPending.erase(sPending.begin() + i);
        } else {
            i++;
        }
    }
    for (size_t i = 0; i < sPeers.size();) { // a player who leaves the lobby frees its ports
        if (waitSocket(sPeers[i].s, false, 0) && !recvSome(sPeers[i])) {
            Log.info("player {} left", i + 1);
            closesocket(sPeers[i].s);
            sPeers.erase(sPeers.begin() + i);
            int port = sMyPorts;
            for (Peer& p : sPeers) {
                p.firstPort = port;
                port += p.nPorts;
            }
            hostStatus();
        } else {
            i++;
        }
    }
    // LAN discovery: "MP5NET?" gets our TCP port back
    while (sDiscovery != INVALID_SOCKET && waitSocket(sDiscovery, false, 0)) {
        char buf[64];
        sockaddr_in from{};
        socklen_t fromLen = sizeof(from);
        const int n = recvfrom(sDiscovery, buf, sizeof(buf), 0, (sockaddr*)&from, &fromLen);
        if (n == (int)sizeof(kDiscoverAsk) && memcmp(buf, kDiscoverAsk, n) == 0) {
            const std::string reply = fmt::format("MP5NET {}", sHostPort);
            sendto(sDiscovery, reply.c_str(), (int)reply.size() + 1, 0, (sockaddr*)&from, fromLen);
        }
    }
}

void pollClient() {
    Peer& host = sPeers[0];
    if (waitSocket(host.s, false, 0) && !recvSome(host)) {
        fail("The host left");
        return;
    }
    u8 type;
    std::vector<u8> payload;
    if (!popMsg(host, type, payload)) {
        return;
    }
    if (type == MsgReject) {
        fail("The host refused: " + std::string(payload.begin(), payload.end()));
        return;
    }
    Reader r{payload.data(), payload.size()};
    StartInfo si = r.get<StartInfo>();
    if (type != MsgStart || !r.ok || si.firstPort + si.nPorts > 4 || !unpackCards(r)) {
        fail("Bad start message from the host");
        return;
    }
    sDelay = si.delay;
    sMyFirstPort = si.firstPort;
    sMyPorts = si.nPorts;
    sPortMask = si.portMask;
    applySettings(si);
    sLobby = NetLobby::Started;
}

} // namespace

bool Mp5Net_Host(u16 port, int delay) {
    Mp5Net_Leave();
    netInit();
    sHost = true;
    sPal = discPal();
    sHostPort = port;
    sDelay = std::clamp(delay, 0, 30);
    sMyFirstPort = 0;
    sMyPorts = std::clamp(envInt("MP5_NET_LOCAL", 1), 1, 4);
    sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int one = 1;
    setsockopt(sListen, SOL_SOCKET, SO_REUSEADDR, (const char*)&one, sizeof(one));
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (bind(sListen, (sockaddr*)&addr, sizeof(addr)) != 0 || listen(sListen, 4) != 0) {
        fail(fmt::format("Cannot listen on port {} (error {})", port, sockErr()));
        return false;
    }
    sDiscovery = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    addr.sin_port = htons(kDiscoveryPort);
    if (bind(sDiscovery, (sockaddr*)&addr, sizeof(addr)) != 0) { // another host on this machine has it: not findable
        closesocket(sDiscovery);
        sDiscovery = INVALID_SOCKET;
    }
    sLobby = NetLobby::Hosting;
    Log.info("hosting on port {}", port);
    hostStatus();
    return true;
}

bool Mp5Net_Join(const std::string& address) {
    Mp5Net_Leave();
    netInit();
    sHost = false;
    sMyPorts = std::clamp(envInt("MP5_NET_LOCAL", 1), 1, 4);
    std::string host = address;
    std::string port = std::to_string(kDefaultPort);
    if (const auto colon = host.rfind(':'); colon != std::string::npos) {
        port = host.substr(colon + 1);
        host.resize(colon);
    }
    addrinfo hints{}, *res = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host.c_str(), port.c_str(), &hints, &res) != 0 || res == nullptr) {
        fail("Cannot find " + address);
        return false;
    }
    Peer peer;
    peer.s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    // non-blocking with a 3 s limit: an unreachable address must not freeze the lobby for the OS's ~20 s
    setNonBlocking(peer.s);
    bool connected = connect(peer.s, res->ai_addr, (int)res->ai_addrlen) == 0;
    freeaddrinfo(res);
    if (!connected && wouldBlock(sockErr()) && waitSocket(peer.s, true, 3000)) {
        int err = 0;
        socklen_t len = sizeof(err);
        getsockopt(peer.s, SOL_SOCKET, SO_ERROR, (char*)&err, &len);
        connected = err == 0;
    }
    if (!connected) {
        closesocket(peer.s);
        fail(fmt::format("Cannot connect to {}", address));
        return false;
    }
    Writer hello;
    hello.put(kProto);
    char build[95] = {};
    strncpy(build, kBuild, sizeof(build) - 1);
    hello.bytes(build, sizeof(build));
    hello.put((u8)discPal());
    hello.put((u8)sMyPorts);
    sendMsg(peer, MsgHello, hello.b);
    sPeers.push_back(std::move(peer));
    sLobby = NetLobby::Joined;
    sStatus = "Connected to " + address + ", waiting for the host to start";
    Log.info("{}", sStatus);
    return true;
}

NetLobby Mp5Net_Poll() {
    if (sLobby == NetLobby::Hosting) {
        pollHost();
    } else if (sLobby == NetLobby::Joined) {
        pollClient();
    }
    return sLobby;
}

bool Mp5Net_Start() {
    if (sLobby != NetLobby::Hosting || sPeers.empty()) {
        return false;
    }
    const auto& g = mp5native::getSettings().game;
    StartInfo si{};
    si.seed = getenv("MP5_TEST_SEED") ? (u32)strtoul(getenv("MP5_TEST_SEED"), nullptr, 0) : (u32)time(nullptr) | 1;
    si.delay = (u8)sDelay;
    si.portMask = (u8)((1 << nextFreePort()) - 1);
    si.language = (u8)g.language.getValue();
    si.quickTransform = g.enableQuickTransform.getValue();
    si.infiniteCoins = g.infiniteCoins.getValue();
    si.cheatCodes = g.enableCheatCodes.getValue();
    si.unlockMg = g.unlockAllMinigames.getValue();
    si.unlockBowser = g.unlockBowsersGnarlyParty.getValue();
    si.speedrun = g.speedrunMode.getValue();
    si.recording = g.recordingMode.getValue();
    // test bench: with MP5_TEST_SEED the whole session replays identically, clock included
    si.baseTime = getenv("MP5_TEST_SEED") ? (OSTime)si.seed * OS_TIMER_CLOCK : OSGetTime();
    Writer cards;
    packCards(cards);
    for (Peer& p : sPeers) {
        si.firstPort = (u8)p.firstPort;
        si.nPorts = (u8)p.nPorts;
        Writer w;
        w.put(si);
        w.bytes(cards.b.data(), cards.b.size());
        if (!sendMsg(p, MsgStart, w.b)) {
            fail("Lost a player while starting");
            return false;
        }
    }
    for (Peer& p : sPending) {
        closesocket(p.s);
    }
    sPending.clear();
    for (sock_t* s : {&sListen, &sDiscovery}) {
        if (*s != INVALID_SOCKET) {
            closesocket(*s);
            *s = INVALID_SOCKET;
        }
    }
    sPortMask = si.portMask;
    applySettings(si);
    sLobby = NetLobby::Started;
    return true;
}

void Mp5Net_Leave() {
    for (Peer& p : sPending) {
        closesocket(p.s);
    }
    sPending.clear();
    for (sock_t* s : {&sListen, &sDiscovery}) {
        if (*s != INVALID_SOCKET) {
            closesocket(*s);
            *s = INVALID_SOCKET;
        }
    }
    closeAll();
    sLobby = NetLobby::Off;
    sStatus.clear();
}

const std::string& Mp5Net_Status() { return sStatus; }

int Mp5Net_Players() { return (int)sPeers.size(); }

std::string Mp5Net_FindLan() {
    netInit();
    sock_t s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int one = 1;
    setsockopt(s, SOL_SOCKET, SO_BROADCAST, (const char*)&one, sizeof(one));
    sockaddr_in to{};
    to.sin_family = AF_INET;
    to.sin_port = htons(kDiscoveryPort);
    for (u32 ip : {(u32)INADDR_BROADCAST, (u32)INADDR_LOOPBACK}) { // loopback: a host on this same machine
        to.sin_addr.s_addr = htonl(ip);
        sendto(s, kDiscoverAsk, sizeof(kDiscoverAsk), 0, (sockaddr*)&to, sizeof(to));
    }
    std::string found;
    if (waitSocket(s, false, 500)) {
        char buf[64] = {};
        sockaddr_in from{};
        socklen_t fromLen = sizeof(from);
        const int n = recvfrom(s, buf, sizeof(buf) - 1, 0, (sockaddr*)&from, &fromLen);
        int port = 0;
        if (n > 0 && sscanf(buf, "MP5NET %d", &port) == 1 && port > 0 && port < 65536) {
            char ip[INET_ADDRSTRLEN] = {};
            inet_ntop(AF_INET, &from.sin_addr, ip, sizeof(ip));
            found = fmt::format("{}:{}", ip, port);
        }
    }
    closesocket(s);
    return found;
}

namespace {

// --- audio ------------------------------------------------------------------------------------------------


void audioFrame() {
    if (!sAudioOn) {
        return;
    }
    const SND_PC_RENDER_INFO info = sndPCGetRenderInfo();
    const size_t frames = (size_t)((sAudioFrames + 1) * info.mixRate / 60 - sAudioFrames * info.mixRate / 60);
    sAudioFrames++;
    static std::vector<s16> buf;
    buf.resize(frames * info.channels);
    if (!sndPCRender(buf.data(), frames)) {
        return;
    }
    // shortcut: the game's 60 Hz and the sound card drift apart; past 100 ms queued, drop audio (state is unaffected)
    const int bytes = (int)(buf.size() * sizeof(s16));
    if (sAudio != nullptr && SDL_GetAudioStreamQueued(sAudio) < (int)(info.mixRate / 10 * info.channels * sizeof(s16))) {
        SDL_PutAudioStreamData(sAudio, buf.data(), bytes);
    }
}

} // namespace

extern "C" void Mp5Net_AudioStart(void) {
    const SND_PC_RENDER_INFO info = sndPCGetRenderInfo();
    const SDL_AudioSpec spec{SDL_AUDIO_S16, (int)info.channels, (int)info.mixRate};
    sAudioOn = true;
    if (sAudio != nullptr) {
        SDL_SetAudioStreamFormat(sAudio, &spec, nullptr);
        return;
    }
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO) ||
        (sAudio = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr)) == nullptr) {
        Log.warn("no audio device: the synth still runs, silent");
    } else {
        SDL_ResumeAudioStreamDevice(sAudio);
    }
}

void Mp5Net_SetConfigPath(const char* path) { Mp5Native_ConfigPath = std::filesystem::u8path(path); }

bool Mp5Net_Boot() {
    // test bench: MP5_NET_HOST=<port> [MP5_NET_CLIENTS=n] / MP5_NET_JOIN=<ip[:port]>, MP5_NET_DELAY, MP5_NET_LOCAL
    const char* hostPort = getenv("MP5_NET_HOST");
    const char* join = getenv("MP5_NET_JOIN");
    if (sLobby != NetLobby::Started && (hostPort != nullptr || join != nullptr)) {
        const int clients = envInt("MP5_NET_CLIENTS", 1);
        if (hostPort != nullptr ? !Mp5Net_Host(*hostPort ? (u16)atoi(hostPort) : kDefaultPort, envInt("MP5_NET_DELAY", 2))
                                : !Mp5Net_Join(join)) {
            return false;
        }
        while (Mp5Net_Poll() != NetLobby::Started) {
            if (sLobby == NetLobby::Failed) {
                return false;
            }
            if (sHost && (int)sPeers.size() >= clients && !Mp5Net_Start()) {
                return false;
            }
            SDL_PumpEvents();
            SDL_Delay(10);
        }
    }
    if (sLobby != NetLobby::Started) {
        return true;
    }
    sActive = true;
    sFrame = 0;
    // a session is debugged by comparing the machines' logs side by side: nothing may stay in a buffer
    setvbuf(stdout, nullptr, _IONBF, 0);
    setvbuf(stderr, nullptr, _IONBF, 0);
    sNextBroadcast = sDelay;
    const auto logPath = Mp5Native_ConfigPath / (sHost ? "netplay_host.log" : "netplay_client.log");
    sLogFile = fopen(logPath.string().c_str(), "w");
    note(false, "session started ({}): pad port(s) {}..{} are this machine's, {} player(s), delay {} frame(s), build {}",
         sHost ? "host" : "client", sMyFirstPort + 1, sMyFirstPort + sMyPorts, std::popcount(sPortMask), sDelay, kBuild);
    Log.info("netplay log: {}", logPath.string());
    note(false, "seed {:08x}, session clock starts at {} (replay this session: MP5_TEST_SEED=0x{:08x})", *Mp5Test_FrandSeed(), sBaseTime, *Mp5Test_FrandSeed());
    return true;
}

// The game reads the clock to seed mini-game generators (m537: OSGetTick), time menus and wait for sounds
// (HuAudSndGrpSetSet: up to 500 ms). In a session that clock is the host's time at the start plus 1/60 s per
// frame, plus 10 us per read so that a loop waiting on it within one frame still ends: the same on every machine.
extern "C" OSTime Mp5Net_OSGetTime(void) {
    if (!sActive) {
        return OSGetTime();
    }
    sLastTime = std::max(sBaseTime + (OSTime)sFrame * (OS_TIMER_CLOCK / 60), sLastTime + OS_TIMER_CLOCK / 100000);
    return sLastTime;
}

extern "C" OSTick Mp5Net_OSGetTick(void) { return (OSTick)Mp5Net_OSGetTime(); }

extern "C" int Mp5Net_Active(void) { return sActive; }

extern "C" void Mp5Net_PadHook(PADStatus* status) {
    if (!sActive) {
        return;
    }
    const u32 frame = sFrame++;
    audioFrame();
    // this machine's pads now are the game's pads delay frames later
    if (sHost) {
        storePart(frame + sDelay, sMyFirstPort, sMyPorts, status);
        broadcastComplete();
    } else {
        Writer w;
        w.put(frame + sDelay);
        w.put((u8)sMyPorts);
        w.bytes(status, sizeof(PADStatus) * sMyPorts);
        if (!sendMsg(sPeers[0], MsgInput, w.b)) {
            note(true, "lost the host at frame {}, playing on locally", frame);
            ui_push_toast("controller", "Online play ended", "The host left: the game goes on here alone.", 8000);
            closeAll();
            return;
        }
    }

    if (frame % kHashEvery == 0) {
        auto state = captureState();
        const u32 crc = crc32(state);
        sStates[frame] = std::move(state);
        if (frame % 600 == 0) { // both sides, so two logs can be compared even without the host's check
            note(false, "crc {:08x} frand {:08x} overlay {:3} | waited for the network {} ms (longest {} ms)", crc,
                 *Mp5Test_FrandSeed(), (int)omcurovl, sWaitMs, sWaitMax);
            sWaitMs = sWaitMax = 0;
        }
        while (sStates.size() > 16) {
            sStates.erase(sStates.begin());
        }
        if (sHost) {
            sHostHash[frame] = crc;
            while (sHostHash.size() > 64) {
                sHostHash.erase(sHostHash.begin());
            }
            checkHashes();
        } else {
            Writer w;
            w.put(frame);
            w.put(crc);
            sendMsg(sPeers[0], MsgHash, w.b);
        }
    }

    Slot slot;
    if (frame < (u32)sDelay) {
        slot.have = sPortMask; // nobody has pads for the first frames: neutral for all
    } else {
        const u64 start = SDL_GetTicks();
        u64 lastPump = start;
        for (;;) {
            auto it = sFrames.find(frame);
            if (it != sFrames.end() && it->second.have == sPortMask) {
                slot = it->second;
                break;
            }
            if (!pump(2) || SDL_GetTicks() - start > kTimeoutMs) {
                note(true, "no pads from a peer for {} s at frame {}: session over, playing on locally", kTimeoutMs / 1000, frame);
                ui_push_toast("controller", "Online play ended", "A player stopped answering: the game goes on here alone.", 8000);
                closeAll();
                return;
            }
            if (SDL_GetTicks() - lastPump > 16) { // a long wait must not look like a hang to the OS
                SDL_PumpEvents();
                lastPump = SDL_GetTicks();
            }
        }
        const u64 waited = SDL_GetTicks() - start;
        sWaitMs += waited;
        sWaitMax = std::max(sWaitMax, waited);
        sFrames.erase(sFrames.begin(), sFrames.find(frame));
    }
    for (int i = 0; i < 4; i++) {
        status[i] = slot.pad[i];
        if (!(sPortMask & (1 << i))) {
            status[i].err = PAD_ERR_NO_CONTROLLER;
        }
    }
}
