// Netplay rollback groundwork: see include/port/rollback.h.
#include "port/rollback.h"
#include "rollback_pages.hpp"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <vector>

#include <SDL3/SDL_timer.h>
#include <dolphin/ar.h>
#include <dolphin/dvd.h>
#include <dolphin/gx.h>
#include <fmt/format.h>

// this file is the wrapper the game's DVDOpen/DVDFastOpen/DVDClose are renamed to (CMakeLists.txt): it calls Aurora's
#undef DVDOpen
#undef DVDFastOpen
#undef DVDClose
#undef DVDReadAsyncPrio
extern "C" {
BOOL DVDFastOpen(s32 entrynum, DVDFileInfo* fileInfo);
BOOL DVDOpen(const char* fileName, DVDFileInfo* fileInfo);
BOOL DVDClose(DVDFileInfo* fileInfo);
BOOL DVDReadAsyncPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset, DVDCallback callback, s32 prio);
void* HuMemHeapGet(int heap, u32* size); // malloc.c
void* HuPrcStackHeapGet(u32* size);       // process.c
void* msmPCSndHeapGet(u32* size);         // msmsys.c
int HuPrcStackList(void** handles, int max); // process.c
int PortThreadStackList(void** handles, int max); // osthread_pc.c
void Mp5Net_TraceLog(const char* fmt, ...);
void HuInterpInvalidate(void);              // frame_interp.cpp
void* salAiGetDest(void);                  // MusyX: its output buffer
void AuroraGXSetDiscard(GXBool discard);   // Aurora: drop draws while ticks run again
void Mp5Net_AddrName(const void* fn, char* buf, int size);
int omOvlLoadPending(void);                // objmain.c
}

namespace {

struct Region {
    u8* p;
    size_t n;
    std::string what;
};

// a function's static: modules register from their static constructors, possibly before this file's run
std::map<const void*, std::vector<Region>>& modules() {
    static std::map<const void*, std::vector<Region>> m;
    return m;
}
u32 sModuleGen; // a module came or went: snapshots taken before it no longer match the memory map

std::vector<Region> regions() {
    std::vector<Region> out;
    for (const auto& [key, rs] : modules()) {
        out.insert(out.end(), rs.begin(), rs.end());
    }
    for (int heap = 0; heap < 5; heap++) {
        u32 size = 0;
        if (u8* p = (u8*)HuMemHeapGet(heap, &size); p != nullptr) {
            out.push_back({p, size, fmt::format("heap{}", heap)});
        }
    }
    u32 size = 0;
    if (u8* p = (u8*)HuPrcStackHeapGet(&size); p != nullptr) {
        out.push_back({p, size, "stacks"});
    }
    if (u8* p = (u8*)msmPCSndHeapGet(&size); p != nullptr) {
        out.push_back({p, size, "sound"});
    }
    // Aurora's ARAM: MusyX's samples and the music streams' buffers
    if (u8* p = (u8*)ARGetStorageAddress(); p != nullptr) {
        out.push_back({p, ARGetSize(), "aram"});
    }
    return out;
}

// HuMem's block header (game/memory.c): a heap is a ring of blocks, each a header then its data
struct HuMemBlock {
    s32 size;
    u8 magic;
    u8 flag; // 0: free
    HuMemBlock* prev;
    HuMemBlock* next;
    u32 num;
    u32 retAddr;
};
static_assert(sizeof(HuMemBlock) == 32, "HuMem headers are 32 bytes");

bool isHuMemHeap(const Region& r) { return r.what.rfind("heap", 0) == 0 || r.what == "stacks" || r.what == "sound"; }

size_t heapUsed(const Region& r) {
    if (!isHuMemHeap(r)) {
        return r.n;
    }
    size_t used = 0;
    const HuMemBlock* b = (const HuMemBlock*)r.p;
    do {
        used += b->flag ? (size_t)b->size : sizeof(HuMemBlock);
        b = b->next;
    } while (b != (const HuMemBlock*)r.p && b != nullptr);
    return used;
}

struct Snapshot {
    std::vector<u8> mem;
    Mp5NetTickState net{};
    u32 gen = 0;
    u32 tick = 0;
};

u32 sTick; // logic ticks since the start (rewinds with a rollback)

// --- disc files ---------------------------------------------------------------------------------------------------
// Aurora keeps a host object per open file (DVDFileInfo.cb.userData). The game's memory goes back in time, the host
// objects do not: a close is only carried out once no snapshot from before it can come back, and the files a rollback
// erases from history are closed then (the game opens them again as it runs the ticks again).
// A tick label is sTick when it happened: during tick t that is t + 1, the first snapshot that sees it (a snapshot
// taken at the top of tick s carries s).
constexpr u32 kOpen = 0xFFFFFFFF;
struct DvdFile {
    void* obj;
    u32 openTick, closeTick;
};
std::vector<DvdFile> sDvd;
bool sTracking; // rollback in use: disc files are tracked

std::map<void*, std::string> sDvdDead; // diagnostics: host objects closed for good, and why

void dvdReallyClose(void* obj, const char* why) {
    if (getenv("MP5_RB_DVDCHECK") != nullptr) {
        sDvdDead[obj] = fmt::format("{} at tick {}", why, sTick);
    }
    DVDFileInfo fi{};
    fi.cb.userData = obj;
    DVDClose(&fi);
}

void dvdRewind(u32 tick) { // the game's memory is back at the start of this tick
    for (size_t i = 0; i < sDvd.size();) {
        if (sDvd[i].openTick > tick) {
            dvdReallyClose(sDvd[i].obj, "opened after the restored tick");
            sDvd.erase(sDvd.begin() + i);
            continue;
        }
        if (sDvd[i].closeTick != kOpen && sDvd[i].closeTick > tick) {
            sDvd[i].closeTick = kOpen;
        }
        i++;
    }
}

void dvdSettle(u32 tick) { // no snapshot from before this tick can come back
    for (size_t i = 0; i < sDvd.size();) {
        if (sDvd[i].closeTick != kOpen && sDvd[i].closeTick <= tick) {
            dvdReallyClose(sDvd[i].obj, "settled");
            sDvd.erase(sDvd.begin() + i);
        } else if (sDvd[i].closeTick == kOpen && sDvd[i].openTick <= tick) {
            sDvd.erase(sDvd.begin() + i); // older than any snapshot: an ordinary open file again
        } else {
            i++;
        }
    }
}

void save(Snapshot& s) {
    size_t total = 0;
    const auto rs = regions();
    for (const auto& r : rs) {
        total += r.n;
    }
    s.mem.resize(total);
    u8* out = s.mem.data();
    for (const auto& r : rs) {
        memcpy(out, r.p, r.n);
        out += r.n;
    }
    Mp5Net_TickSave(&s.net);
    s.gen = sModuleGen;
    s.tick = sTick;
}

void load(const Snapshot& s) {
    const u8* in = s.mem.data();
    for (const auto& r : regions()) {
        memcpy(r.p, in, r.n);
        in += r.n;
    }
    Mp5Net_TickLoad(&s.net);
    dvdRewind(s.tick);
    sTick = s.tick;
    HuInterpInvalidate();
}

// --- light snapshots ----------------------------------------------------------------------------------------------
// What a tick's snapshot holds besides the page logs (rollback_pages.cpp): the partial pages at the ends of each
// region, the coroutine stacks' live parts (the stack heap is not page-tracked: a fault on a protected stack page could
// not even be delivered, the handler runs on that stack), and the session state.
struct Light {
    u32 gen = 0;
    Mp5NetTickState net{};
    std::vector<u8> bytes; // the pieces, in layout order
    std::vector<std::pair<u8*, size_t>> layout;
};
std::map<u32, Light> sLights;
u32 sTrackedGen = 0xFFFFFFFF;
constexpr u32 kKeepTicks = 12; // how far back a restore may go (netplay runs at most 8 frames ahead)

// the stack heap's pieces worth keeping: every block header, and for a coroutine its saved registers and the part of
// its stack above the saved stack pointer
void stackPieces(const Region& r, std::vector<std::pair<u8*, size_t>>& out) {
    void* handles[512];
    int n = HuPrcStackList(handles, 448);
    n += PortThreadStackList(handles + n, 512 - n);
    HuMemBlock* b = (HuMemBlock*)r.p;
    do {
        u8* data = (u8*)b + sizeof(HuMemBlock);
        out.push_back({(u8*)b, sizeof(HuMemBlock)});
        if (b->flag) {
            u8* end = (u8*)b + b->size;
            if (std::find(handles, handles + n, (void*)data) != handles + n) {
                u8* sp = *(u8**)data;
                out.push_back({data, 0x200});
                if (sp > data + 0x200 && sp < end) {
                    out.push_back({sp, (size_t)(end - sp)});
                } else {
                    out.push_back({data + 0x200, (size_t)(end - data - 0x200)});
                }
            } else {
                out.push_back({data, (size_t)(end - data)});
            }
        }
        b = b->next;
    } while (b != (HuMemBlock*)r.p && b != nullptr);
}

std::vector<std::pair<u8*, size_t>> pieces() {
    std::vector<std::pair<u8*, size_t>> out;
    const size_t pg = mp5rb::pageSize();
    for (const auto& r : regions()) {
        if (r.what == "stacks") {
            stackPieces(r, out);
            continue;
        }
        u8* lo = (u8*)(((uintptr_t)r.p + pg - 1) & ~(uintptr_t)(pg - 1));
        u8* hi = (u8*)(((uintptr_t)r.p + r.n) & ~(uintptr_t)(pg - 1));
        if (hi <= lo) {
            out.push_back({r.p, r.n});
            continue;
        }
        if (lo > r.p) {
            out.push_back({r.p, (size_t)(lo - r.p)});
        }
        if (r.p + r.n > hi) {
            out.push_back({hi, (size_t)(r.p + r.n - hi)});
        }
    }
    return out;
}

void ensureTracked() {
    if (sTrackedGen == sModuleGen) {
        return;
    }
    std::vector<std::pair<u8*, size_t>> rs;
    for (const auto& r : regions()) {
        if (r.what != "stacks") {
            rs.push_back({r.p, r.n});
        }
    }
    mp5rb::track(rs);
    sTrackedGen = sModuleGen;
    sLights.clear();
}

// the state at the top of this tick can be restored from now on
void lightSave(u32 tick) {
    ensureTracked();
    mp5rb::mark(tick);
    Light& l = sLights[tick];
    l.gen = sModuleGen;
    Mp5Net_TickSave(&l.net);
    l.layout = pieces();
    size_t total = 0;
    for (const auto& [p, n] : l.layout) {
        total += n;
    }
    l.bytes.resize(total);
    u8* out = l.bytes.data();
    for (const auto& [p, n] : l.layout) {
        memcpy(out, p, n);
        out += n;
    }
    if (tick > kKeepTicks) {
        sLights.erase(sLights.begin(), sLights.lower_bound(tick - kKeepTicks));
        mp5rb::forget(tick - kKeepTicks);
    }
}

bool lightRestore(u32 tick) {
    auto it = sLights.find(tick);
    if (it == sLights.end() || it->second.gen != sModuleGen || !mp5rb::restore(tick)) {
        return false;
    }
    const Light& l = it->second;
    const u8* in = l.bytes.data();
    for (const auto& [p, n] : l.layout) {
        memcpy(p, in, n);
        in += n;
    }
    Mp5Net_TickLoad(&l.net);
    dvdRewind(tick);
    sTick = tick;
    HuInterpInvalidate();
    sLights.erase(std::next(it), sLights.end());
    return true;
}

// Bytes that differ without the game's state differing, blanked before two snapshots are compared: a coroutine's
// stack below its saved stack pointer (whatever the last calls left there), and MusyX's output buffer (sound already
// handed out; the synth's state itself is compared).
void maskDeadStacks(std::vector<u8>& a, std::vector<u8>& b) {
    size_t ofs = 0;
    u8* out = (u8*)salAiGetDest();
    constexpr size_t kOutBytes = 96000 / 200 * 8 * sizeof(s16); // SAL_PC_MAX_FRAMES * SAL_PC_MAX_CHANNELS
    for (const auto& r : regions()) {
        if (out >= r.p && out + kOutBytes <= r.p + r.n) {
            memset(a.data() + ofs + (out - r.p), 0, kOutBytes);
            memset(b.data() + ofs + (out - r.p), 0, kOutBytes);
        }
        if (r.what == "stacks") {
            void* handles[512];
            int n = HuPrcStackList(handles, 448);
            n += PortThreadStackList(handles + n, 512 - n);
            for (int i = 0; i < n; i++) {
                u8* lo = (u8*)handles[i] + 0x200; // libco's saved registers
                u8* sp = *(u8**)handles[i];
                if (lo >= r.p && sp <= r.p + r.n && sp > lo) {
                    memset(a.data() + ofs + (lo - r.p), 0, sp - lo);
                    memset(b.data() + ofs + (lo - r.p), 0, sp - lo);
                }
            }
        }
        ofs += r.n;
    }
}

// the first differences between two snapshots of the same memory map, by region (module offsets symbolise with
// llvm-symbolizer --obj=<module> DATA 0x<preferred base + offset>)
int report(const Snapshot& was, const Snapshot& now) {
    size_t ofs = 0;
    int shown = 0, regionsBad = 0;
    std::string lastVar;
    for (const auto& r : regions()) {
        const u8* a = was.mem.data() + ofs;
        const u8* b = now.mem.data() + ofs;
        size_t diffs = 0, first = 0;
        for (size_t i = 0; i < r.n; i++) {
            if (a[i] != b[i]) {
                if (diffs++ == 0) {
                    first = i;
                }
                if (shown < 40) { // one line per variable (the first byte that differs in it)
                    char name[256];
                    Mp5Net_AddrName(r.p + i, name, sizeof(name));
                    const char* sym = strchr(name, ' ');
                    std::string var = sym != nullptr ? std::string(sym, strcspn(sym, "+")) : std::string();
                    if (var.empty() || var != lastVar || diffs == 1) {
                        Mp5Net_TraceLog("  %s+0x%zx (%s): %02x -> %02x", r.what.c_str(), i, name, a[i], b[i]);
                        shown++;
                        lastVar = var;
                    }
                }
            }
        }
        if (diffs != 0) {
            regionsBad++;
            Mp5Net_TraceLog("  %s: %zu byte(s) differ, first at +0x%zx", r.what.c_str(), diffs, first);
        }
        ofs += r.n;
    }
    return regionsBad;
}

// --- synctest ---------------------------------------------------------------------------------------------------
enum class Phase { Idle, Run, Replay };
Phase sPhase = Phase::Idle;
int sEvery = -1;
u32 sTarget;
Snapshot sStart, sEnd;
std::map<u32, std::array<PADStatus, 4>> sPads;
std::map<u32, Snapshot> sEach; // MP5_RB_EACH: the state at every tick of the window, to find the first that differs
bool sReplaying;
u32 sTests, sFails;
u64 sLightNs, sLightTicks, sLightFaults0;

} // namespace

extern "C" void Mp5Rb_Sections(const void* key, void* d0, void* d1, void* b0, void* b1) {
    char name[256];
    Mp5Net_AddrName(key, name, sizeof(name));
    std::string mod = name;
    mod = mod.substr(0, mod.find('+'));
    mp5rb::stop(); // the memory map changes: tracking starts again from the next tick
    auto& rs = modules()[key];
    if (d1 > d0) {
        rs.push_back({(u8*)d0, (size_t)((u8*)d1 - (u8*)d0), mod + ":data"});
    }
    if (b1 > b0) {
        rs.push_back({(u8*)b0, (size_t)((u8*)b1 - (u8*)b0), mod + ":bss"});
    }
    sModuleGen++;
    Mp5Net_TraceLog("rollback: module %s loaded at tick %u", mod.c_str(), sTick);
}

extern "C" void Mp5Rb_Forget(const void* key) {
    mp5rb::stop(); // its pages go away now (and another module may be mapped there): no page of it may stay protected
    modules().erase(key);
    sModuleGen++;
    Mp5Net_TraceLog("rollback: a module unloaded at tick %u", sTick);
}

bool synctestOn() {
    if (sEvery < 0) {
        const char* e = getenv("MP5_RB_SYNCTEST");
        sEvery = e != nullptr ? atoi(e) : 0;
    }
    sTracking = sTracking || sEvery > 0;
    return sEvery > 0;
}

// Aurora runs the frame's GX commands at its end, and they point into the game's memory: a rollback therefore only
// happens before the frame's first tick, never between two ticks of one frame.
extern "C" int Mp5Rb_FrameBegin(void) {
    if (!synctestOn() || sPhase != Phase::Run || sTick < sTarget) {
        return 0;
    }
    if (sStart.gen != sModuleGen) { // a module was loaded or unloaded inside the window: not comparable
        sPhase = Phase::Idle;
        return 0;
    }
    static const bool fast = getenv("MP5_RB_FAST") != nullptr; // test bench: no full compare, timings only
    const u64 t0 = SDL_GetTicksNS();
    if (!fast) {
        save(sEnd);
    } else {
        sEnd.tick = sTick;
        sEnd.gen = sModuleGen;
    }
    const u64 t1 = SDL_GetTicksNS();
    if (getenv("MP5_RB_NOOP") != nullptr) { // test bench: restore the state just saved (isolates the copy itself)
        load(sEnd);
        sPhase = Phase::Idle;
        return 0;
    }
    if (!lightRestore(sStart.tick)) {
        Mp5Net_TraceLog("synctest: tick %u cannot be restored", sStart.tick);
        sPhase = Phase::Idle;
        return 0;
    }
    const u64 t2 = SDL_GetTicksNS();
    if (sTests % 60 == 0) {
        for (const auto& r : regions()) {
            const size_t used = heapUsed(r);
            Mp5Net_TraceLog("  region %s: %zu KB, %zu KB used", r.what.c_str(), r.n / 1024, used / 1024);
        }
    }
    if (sTests % 60 == 0) {
        Mp5Net_TraceLog("synctest: restore %.2f ms (%u ticks back); per tick: snapshot %.3f ms, %.1f page faults",
                        (t2 - t1) / 1e6, sEnd.tick - sStart.tick, sLightNs / 1e6 / std::max<u64>(sLightTicks, 1),
                        (double)(mp5rb::faults() - sLightFaults0) / std::max<u64>(sLightTicks, 1));
        sLightNs = sLightTicks = 0;
        sLightFaults0 = mp5rb::faults();
    }
    sTarget = sEnd.tick;
    if (getenv("MP5_RB_VERBOSE") != nullptr) {
        Mp5Net_TraceLog("synctest: back to tick %u, running %u tick(s) again", sStart.tick, sEnd.tick - sStart.tick);
    }
    sPhase = Phase::Replay;
    sReplaying = true;
    AuroraGXSetDiscard(GX_TRUE); // ticks run again are not shown (and would overflow the frame's buffers)
    return (int)(sEnd.tick - sStart.tick); // the window's ticks run again
}

namespace {
u32 sResimTarget = kOpen; // rollback session: the tick the game was at before running ticks again

// rollback session: snapshot every tick; after a misprediction, back to the tick that made the first wrong frame
int netTick() {
    sTracking = true;
    u32 tick = sTick;
    int extra = 0;
    if (sResimTarget != kOpen && tick >= sResimTarget) {
        sResimTarget = kOpen;
        AuroraGXSetDiscard(GX_FALSE);
    }
    if (sResimTarget == kOpen) {
        if (omOvlLoadPending()) {
            Mp5Net_WaitConfirmed(); // a module load cannot be rolled back: no prediction may reach over it
        }
        u32 frame;
        if (Mp5Net_TakeMisprediction(&frame)) {
            u32 to = kOpen;
            for (auto it = sLights.rbegin(); it != sLights.rend(); ++it) {
                if (it->second.net.frame <= frame && it->second.gen == sModuleGen) {
                    to = it->first;
                    break;
                }
            }
            AuroraGXSetDiscard(GX_TRUE); // drains what this frame queued so far, then drops the ticks run again
            if (to != kOpen && to < tick && lightRestore(to)) {
                Mp5Net_RollbackDone(tick - to);
                sResimTarget = tick;
                extra = (int)(tick - to);
                tick = to;
            } else {
                AuroraGXSetDiscard(GX_FALSE);
                Mp5Net_TraceLog("rollback: frame %u cannot be run again (snapshot tick %d of %zu kept, oldest frame %u; page log %s, "
                                "%zu pages), a desync may follow",
                                frame, to == kOpen ? -1 : (int)to, sLights.size(),
                                sLights.empty() ? 0u : sLights.begin()->second.net.frame,
                                mp5rb::overflowed() ? "full" : "ok", mp5rb::logged());
            }
        }
    }
    lightSave(tick);
    sTick = tick + 1;
    return extra;
}
} // namespace

extern "C" int Mp5Rb_Resimulating(void) { return sResimTarget != kOpen || sReplaying; }

// the tick running now can be restored (no module came or went since its snapshot): its pads may be predicted
extern "C" int Mp5Rb_TickRestorable(void) {
    auto it = sLights.find(sTick - 1);
    return sTrackedGen == sModuleGen && it != sLights.end() && it->second.gen == sModuleGen;
}

extern "C" int Mp5Rb_TickBegin(void) {
    if (Mp5Net_RollbackActive() && getenv("MP5_RB_SYNCTEST") == nullptr) {
        return netTick();
    }
    if (!synctestOn()) {
        return 0;
    }
    const u32 tick = sTick; // the tick about to run
    {
        const u64 t0 = SDL_GetTicksNS();
        lightSave(tick);
        sLightNs += SDL_GetTicksNS() - t0;
        sLightTicks++;
    }
    // MP5_RB_DIRTY=1: how many 4 KB pages of each region change from one tick to the next (sizing the snapshots)
    if (static const bool dirty = getenv("MP5_RB_DIRTY") != nullptr; dirty && sPhase != Phase::Replay) {
        static Snapshot prev;
        static std::map<std::string, std::pair<u64, u64>> acc; // region: changed pages, ticks
        Snapshot now;
        save(now);
        if (prev.mem.size() == now.mem.size()) {
            size_t ofs = 0;
            for (const auto& r : regions()) {
                u64 pages = 0;
                for (size_t p = 0; p < r.n; p += 4096) {
                    const size_t len = std::min<size_t>(4096, r.n - p);
                    pages += memcmp(prev.mem.data() + ofs + p, now.mem.data() + ofs + p, len) != 0;
                }
                acc[r.what].first += pages;
                acc[r.what].second++;
                ofs += r.n;
            }
        }
        if (tick % 300 == 0) {
            for (const auto& [what, a] : acc) {
                Mp5Net_TraceLog("dirty: %s %.1f pages per tick", what.c_str(), a.second ? (double)a.first / a.second : 0.0);
            }
            acc.clear();
        }
        prev = std::move(now);
    }
    if (static const bool trace = getenv("MP5_RB_TRACE") != nullptr; trace) {
        Mp5Net_TraceLog("tick %u phase %d", tick, (int)sPhase);
    }
    // MP5_RB_EACH=<tick>: from that tick on, compare every tick of the replay (slow)
    static const long eachFrom = getenv("MP5_RB_EACH") != nullptr ? atol(getenv("MP5_RB_EACH")) : -1;
    const bool each = eachFrom >= 0 && tick >= (u32)eachFrom;
    if (each && sPhase == Phase::Run) {
        save(sEach[tick]);
    }
    if (each && sPhase == Phase::Replay && tick != sTarget && sEach.count(tick)) {
        Snapshot now;
        save(now);
        Snapshot& was = sEach[tick];
        if (now.mem.size() == was.mem.size()) {
            maskDeadStacks(now.mem, was.mem);
            if (now.mem != was.mem) {
                Mp5Net_TraceLog("synctest: the replay parts at tick %u (window from %u):", tick, sStart.tick);
                report(was, now);
            }
        }
    }
    if (sPhase == Phase::Replay && tick == sTarget) {
        sReplaying = false;
        AuroraGXSetDiscard(GX_FALSE);
        sTests++;
        if (sEnd.gen != sModuleGen) {
            Mp5Net_TraceLog("synctest: the replay loaded or unloaded a module (tick %u)", tick);
        }
        static const bool fast = getenv("MP5_RB_FAST") != nullptr;
        Snapshot now;
        if (!fast) {
            save(now);
            maskDeadStacks(now.mem, sEnd.mem);
        }
        if (fast) {
            // timings only
        } else if (now.mem != sEnd.mem || memcmp(&now.net, &sEnd.net, sizeof(now.net)) != 0) {
            sFails++;
            Mp5Net_TraceLog("synctest FAILED at tick %u (window of %u tick(s)), %u of %u failed so far:", tick,
                            sEnd.tick - sStart.tick, sFails, sTests);
            report(sEnd, now);
            if (memcmp(&now.net, &sEnd.net, sizeof(now.net)) != 0) {
                Mp5Net_TraceLog("  session: frame %u/%u time %lld/%lld audio %llu/%llu", sEnd.net.frame, now.net.frame,
                                (long long)sEnd.net.lastTime, (long long)now.net.lastTime,
                                (unsigned long long)sEnd.net.audioFrames, (unsigned long long)now.net.audioFrames);
            }
        } else if (sTests % 60 == 0) {
            Mp5Net_TraceLog("synctest: %u window(s) checked, %u failed", sTests, sFails);
        }
        sPhase = Phase::Idle;
    }
    if (sPhase == Phase::Idle) {
        sEach.clear();
        dvdSettle(sLights.empty() ? tick : sLights.begin()->first);
        sStart.mem.clear();
        sStart.tick = tick;
        sStart.gen = sModuleGen;
        Mp5Net_TickSave(&sStart.net);
        sTarget = tick + sEvery;
        sPhase = Phase::Run;
        sPads.erase(sPads.begin(), sPads.lower_bound(sStart.net.frame));
    }
    sTick = tick + 1;
    return 0;
}

extern "C" int Mp5Rb_PadReplay(u32 frame, PADStatus* status) {
    if (!sReplaying) {
        return 0;
    }
    auto it = sPads.find(frame);
    if (it == sPads.end()) {
        Mp5Net_TraceLog("synctest: no recorded pads for frame %u", frame);
        return 0;
    }
    memcpy(status, it->second.data(), sizeof(PADStatus) * 4);
    return 1;
}

extern "C" void Mp5Rb_PadRecord(u32 frame, const PADStatus* status) {
    if (sEvery > 0 && !sReplaying) {
        auto& slot = sPads[frame];
        memcpy(slot.data(), status, sizeof(PADStatus) * 4);
    }
}

// --- disc files (DVDOpen=Mp5Rb_DVDOpen... for the game's code, CMakeLists.txt) ---------------------------------------

static void dvdOpened(DVDFileInfo* fileInfo) {
    sDvdDead.erase(fileInfo->cb.userData); // an address the allocator gave again
}

extern "C" BOOL Mp5Rb_DVDFastOpen(s32 entrynum, DVDFileInfo* fileInfo) {
    const BOOL ok = DVDFastOpen(entrynum, fileInfo);
    if (ok) {
        dvdOpened(fileInfo);
    }
    if (ok && sTracking && fileInfo->cb.userData != nullptr) {
        sDvd.push_back({fileInfo->cb.userData, sTick, kOpen});
    }
    return ok;
}

extern "C" BOOL Mp5Rb_DVDOpen(const char* fileName, DVDFileInfo* fileInfo) {
    const BOOL ok = DVDOpen(fileName, fileInfo);
    if (ok) {
        dvdOpened(fileInfo);
    }
    if (ok && sTracking && fileInfo->cb.userData != nullptr) {
        sDvd.push_back({fileInfo->cb.userData, sTick, kOpen});
    }
    return ok;
}

extern "C" BOOL Mp5Rb_DVDClose(DVDFileInfo* fileInfo) {
    if (!sTracking || fileInfo == nullptr || fileInfo->cb.userData == nullptr) {
        return DVDClose(fileInfo);
    }
    auto it = std::find_if(sDvd.begin(), sDvd.end(),
                           [&](const DvdFile& f) { return f.obj == fileInfo->cb.userData && f.closeTick == kOpen; });
    if (it != sDvd.end()) {
        it->closeTick = sTick;
    } else {
        sDvd.push_back({fileInfo->cb.userData, 0, sTick}); // opened before tracking: its close waits too
    }
    fileInfo->cb.userData = nullptr;
    fileInfo->cb.state = DVD_STATE_END;
    return TRUE;
}

// MP5_RB_DVDCHECK=1: a read through a host object already closed for good is reported (it would crash)
extern "C" BOOL Mp5Rb_DVDReadAsyncPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset, DVDCallback callback,
                                       s32 prio) {
    if (auto it = sDvdDead.find(fileInfo->cb.userData); it != sDvdDead.end()) {
        char name[256];
        Mp5Net_AddrName(fileInfo, name, sizeof(name));
        Mp5Net_TraceLog("rollback: read through a closed file object %p (%s) in %s, at tick %u", fileInfo->cb.userData,
                        it->second.c_str(), name, sTick);
        return FALSE;
    }
    return DVDReadAsyncPrio(fileInfo, addr, length, offset, callback, prio);
}

// MP5_RB_EACH=<tick> also hashes every region at points inside each tick (main.c): the first point where the replay
// parts from the original names the step of the tick that went another way
namespace {
std::map<std::pair<u32, int>, std::vector<u64>> sProbe;
std::vector<u64> hashRegions() {
    std::vector<u64> out;
    for (const auto& r : regions()) {
        u64 h = 1469598103934665603ull;
        const u64* w = (const u64*)r.p;
        for (size_t i = 0; i < r.n / 8; i++) {
            h = (h ^ w[i]) * 1099511628211ull;
        }
        out.push_back(h);
    }
    return out;
}
} // namespace

extern "C" void Mp5Rb_Probe(int point) {
    // test bench: MP5_RB_ZERODEAD=1 blanks the coroutine stacks' dead part before the processes run (a local read before
    // it is written then sees zeros in every run, whatever host code ran there before)
    if (static const bool zero = getenv("MP5_RB_ZERODEAD") != nullptr; zero && point == 1) {
        for (const auto& r : regions()) {
            if (r.what == "stacks") {
                void* handles[512];
                int n = HuPrcStackList(handles, 448);
                n += PortThreadStackList(handles + n, 512 - n);
                for (int i = 0; i < n; i++) {
                    u8* lo = (u8*)handles[i] + 0x200;
                    u8* sp = *(u8**)handles[i];
                    if (lo >= r.p && sp <= r.p + r.n && sp > lo) {
                        memset(lo, 0, sp - lo);
                    }
                }
            }
        }
    }
    static const long eachFrom = getenv("MP5_RB_EACH") != nullptr ? atol(getenv("MP5_RB_EACH")) : -1;
    const u32 tick = sTick - 1; // inside the tick TickBegin started
    if (eachFrom < 0 || tick < (u32)eachFrom || sEvery <= 0) {
        return;
    }
    if (sPhase == Phase::Run) {
        sProbe[{tick, point}] = hashRegions();
    } else if (sPhase == Phase::Replay) {
        auto it = sProbe.find({tick, point});
        if (it == sProbe.end()) {
            return;
        }
        const auto now = hashRegions();
        const auto rs = regions();
        for (size_t i = 0; i < now.size() && i < it->second.size(); i++) {
            if (now[i] != it->second[i]) {
                Mp5Net_TraceLog("probe: tick %u point %d: %s differs", tick, point, rs[i].what.c_str());
            }
        }
    }
    if (sProbe.size() > 400) {
        sProbe.erase(sProbe.begin());
    }
}

// after each process the scheduler ran (process.c): the first process after which the replay's memory differs
namespace {
u32 sProcTick = 0xFFFFFFFF;
int sProcIndex;
bool sProcReported;
std::map<std::pair<u32, int>, const void*> sProcFunc;
} // namespace

extern "C" void Mp5Rb_ProcProbe(void* func) {
    static const long eachFrom = getenv("MP5_RB_EACH") != nullptr ? atol(getenv("MP5_RB_EACH")) : -1;
    const u32 tick = sTick - 1;
    if (eachFrom < 0 || tick < (u32)eachFrom || sEvery <= 0 || sPhase == Phase::Idle) {
        return;
    }
    if (tick != sProcTick) {
        sProcTick = tick;
        sProcIndex = 0;
        sProcReported = false;
    }
    const int point = 1000 + sProcIndex++;
    // MP5_RB_DIFFAT=<tick>:<point>: the whole state there in the original, and the variables the replay changed
    static Snapshot diffSnap;
    static u32 diffTick = 0xFFFFFFFF;
    static int diffPoint = -1;
    if (static bool read = false; !read) {
        read = true;
        if (const char* e = getenv("MP5_RB_DIFFAT"); e != nullptr) {
            sscanf(e, "%u:%d", &diffTick, &diffPoint);
        }
    }
    if (tick == diffTick && point == diffPoint) {
        if (sPhase == Phase::Run) {
            save(diffSnap);
        } else {
            Snapshot now;
            save(now);
            maskDeadStacks(now.mem, diffSnap.mem);
            Mp5Net_TraceLog("diffat: tick %u point %d, original -> replay:", tick, point);
            report(diffSnap, now);
        }
    }
    if (sPhase == Phase::Run) {
        sProcFunc[{tick, point}] = func;
    } else if (!sProcReported) {
        auto it = sProbe.find({tick, point});
        if (it != sProbe.end() && hashRegions() != it->second) {
            char a[256], b[256];
            Mp5Net_AddrName(func, a, sizeof(a));
            Mp5Net_AddrName(sProcFunc[{tick, point}], b, sizeof(b));
            Mp5Net_TraceLog("probe: tick %u, process #%d: memory differs after %s (the original ran %s there)", tick,
                            sProcIndex - 1, a, b);
            sProcReported = true;
        }
        return;
    }
    sProbe[{tick, point}] = hashRegions();
}
