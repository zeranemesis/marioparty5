// Settings > Cheats: the built-in cheats and the cheat code file (cheats.txt in the config folder).
//
// Cheat codes are GameCube Gecko / unencrypted Action Replay write codes. They name console addresses; the port's
// memory is laid out differently (64-bit, little-endian, its own addresses), so a code only works when it writes a
// whole field the table below knows: each such field is found at its console address and written natively. Codes
// that patch the game's machine code (C2, 04 into .text), use pointers or write part of a bit field are reported as
// unsupported and skipped.
//
// cheats.txt:   $Name             a code, on
//               -$Name            a code, off
//               04xxxxxx yyyyyyyy its lines (00/02/04 writes, 20/22/28/2A conditions, E0/E2 end of conditions)
//               # ...             a comment

#include "port/cheats.h"

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#include "port/main.h"
#include "port/settings.h"

extern "C" {
#include "game/gamework.h"
}

namespace {

// the console's addresses (config/GP5E01_00/symbols.txt, GP5E01). The field offsets are the console's, from the
// GameCube compiler (build/probeoff.py on a probe returning &p->field): MWCC packs the bit fields differently from
// MSVC, so offsetof on PC is not the console's (GwPlayer.coin: 0x20 there, 0x22 here). The array strides match.
constexpr uint32_t kGwPlayer = 0x8022A070;
constexpr uint32_t kGwSystem = 0x8022A490;
static_assert(sizeof(GWPLAYER) == 0x108, "GwPlayer[] stride differs from the console's");

struct Field {
    const char *name;
    uint32_t gc;  // console address of element 0
    uint32_t size; // bytes of one element
    uint32_t count;
    void *pc;
};

std::vector<Field> s_fields;

void add(const char *name, uint32_t gc, void *pc, uint32_t size, uint32_t count = 1)
{
    s_fields.push_back({name, gc, size, count, pc});
}

void build_fields()
{
    if (!s_fields.empty()) {
        return;
    }
#define PLAYER(f, gcOfs, n) add(#f, kGwPlayer + i * 0x108 + (gcOfs), &GwPlayer[i].f, sizeof(GwPlayer[i].f) / (n), (n))
    for (uint32_t i = 0; i < GW_PLAYER_MAX; i++) {
        PLAYER(handicap, 0x03, 1);
        PLAYER(capsule, 0x05, 3);
        PLAYER(walkNum, 0x0A, 1);
        PLAYER(masuId, 0x0C, 1);
        PLAYER(capsuleUse, 0x12, 1);
        PLAYER(blueMasuNum, 0x14, 1);
        PLAYER(redMasuNum, 0x15, 1);
        PLAYER(capsuleMasuNum, 0x16, 1);
        PLAYER(hatenaMasuNum, 0x17, 1);
        PLAYER(kupaMasuNum, 0x18, 1);
        PLAYER(battleMasuNum, 0x1A, 1);
        PLAYER(starMasuNum, 0x1D, 1);
        PLAYER(coin, 0x20, 1);
        PLAYER(coinTotalMg, 0x22, 1);
        PLAYER(coinTotal, 0x24, 1);
        PLAYER(coinMax, 0x26, 1);
        PLAYER(coinBattle, 0x28, 1);
        PLAYER(mgCoin, 0x2A, 1);
        PLAYER(mgCoinBonus, 0x2C, 1);
        PLAYER(mgScore, 0x30, 1);
        PLAYER(star, 0x34, 1);
        PLAYER(starMax, 0x36, 1);
    }
#undef PLAYER
#define SYSTEM(f, gcOfs) add(#f, kGwSystem + (gcOfs), &GwSystem.f, sizeof(GwSystem.f))
    SYSTEM(storyDif, 0x01);
    SYSTEM(turnNo, 0x04);
    SYSTEM(turnMax, 0x05);
    SYSTEM(starTotal, 0x07);
    SYSTEM(last5Effect, 0x09);
    SYSTEM(bankCoin, 0x38);
#undef SYSTEM
}

// the native element a console write of `size` bytes at `addr` is exactly
void *resolve(uint32_t addr, uint32_t size)
{
    for (const Field &f : s_fields) {
        if (addr >= f.gc && addr < f.gc + f.size * f.count && (addr - f.gc) % f.size == 0) {
            return f.size == size ? static_cast<char *>(f.pc) + (addr - f.gc) : nullptr;
        }
    }
    return nullptr;
}

uint32_t read(void *p, uint32_t size)
{
    switch (size) {
    case 1: return *static_cast<uint8_t *>(p);
    case 2: return *static_cast<uint16_t *>(p);
    default: return *static_cast<uint32_t *>(p);
    }
}

void write(void *p, uint32_t size, uint32_t v)
{
    switch (size) {
    case 1: *static_cast<uint8_t *>(p) = static_cast<uint8_t>(v); break;
    case 2: *static_cast<uint16_t *>(p) = static_cast<uint16_t>(v); break;
    default: *static_cast<uint32_t *>(p) = v; break;
    }
}

struct Line {
    uint32_t a, b;
};

struct Code {
    std::string name;
    bool on;
    bool supported = true;
    std::vector<Line> lines;
};

std::vector<Code> s_codes;
std::string s_status;

bool supported(const Line &l)
{
    const uint32_t type = l.a >> 24;
    const uint32_t addr = (l.a & 0x01FFFFFF) | 0x80000000;
    switch (type) {
    case 0x00: return resolve(addr, 1) != nullptr;
    case 0x02: return resolve(addr, 2) != nullptr;
    case 0x04: return resolve(addr, 4) != nullptr;
    case 0x20: case 0x22: return resolve(addr, 4) != nullptr;
    case 0x28: case 0x2A: return resolve(addr, 2) != nullptr;
    case 0xE0: case 0xE2: return true;
    default: return false;
    }
}

void run(const Code &c)
{
    bool active = true;
    for (const Line &l : c.lines) {
        const uint32_t type = l.a >> 24;
        const uint32_t addr = (l.a & 0x01FFFFFF) | 0x80000000;
        if (type == 0xE0 || type == 0xE2) {
            active = true;
            continue;
        }
        if (!active) {
            continue;
        }
        switch (type) {
        case 0x00: { // 00aaaaaa NNNN00VV: VV into NNNN+1 bytes
            for (uint32_t n = 0; n <= (l.b >> 16); n++) {
                if (void *p = resolve(addr + n, 1)) write(p, 1, l.b & 0xFF);
            }
            break;
        }
        case 0x02: { // 02aaaaaa NNNNVVVV: VVVV into NNNN+1 halfwords
            for (uint32_t n = 0; n <= (l.b >> 16); n++) {
                if (void *p = resolve(addr + n * 2, 2)) write(p, 2, l.b & 0xFFFF);
            }
            break;
        }
        case 0x04:
            write(resolve(addr, 4), 4, l.b);
            break;
        case 0x20: case 0x22: { // 32-bit equal / not equal
            const bool eq = read(resolve(addr, 4), 4) == l.b;
            active = (type == 0x20) ? eq : !eq;
            break;
        }
        case 0x28: case 0x2A: { // 28aaaaaa MMMMVVVV: (value & ~MMMM) equal / not equal VVVV
            const bool eq = (read(resolve(addr, 2), 2) & ~(l.b >> 16) & 0xFFFF) == (l.b & 0xFFFF);
            active = (type == 0x28) ? eq : !eq;
            break;
        }
        }
    }
}

uint32_t hex(const std::string &s, bool &ok)
{
    char *end = nullptr;
    const unsigned long v = std::strtoul(s.c_str(), &end, 16);
    ok = s.size() == 8 && end == s.c_str() + 8;
    return static_cast<uint32_t>(v);
}

} // namespace

namespace mp5native::cheats {

std::filesystem::path file_path()
{
    return Mp5Native_ConfigPath / "cheats.txt";
}

void reload()
{
    build_fields();
    s_codes.clear();
    std::ifstream in(file_path());
    if (!in) {
        std::ofstream out(file_path());
        out << "# Mario Party 5 (USA) cheat codes: Gecko / unencrypted Action Replay write codes.\n"
               "# $Name turns a code on, -$Name off. Settings > Cheats > Reload Codes applies changes.\n"
               "# Only codes writing whole fields of GwPlayer / GwSystem (coins, stars, capsules, turns...) can work.\n\n"
               "-$Example: player 1 keeps 999 coins\n";
        char buf[160];
        for (const Field &f : s_fields) {
            if (std::strcmp(f.name, "coin") == 0 && f.gc < kGwPlayer + 0x108) {
                std::snprintf(buf, sizeof(buf), "02%06X 000003E7\n", f.gc & 0xFFFFFF);
                out << buf;
            }
        }
        out << "\n# Addresses the port can write (players 2-4: add 0x108 per player):\n";
        for (const Field &f : s_fields) {
            if (f.gc < kGwPlayer + 0x108 || f.gc >= kGwSystem) {
                std::snprintf(buf, sizeof(buf), "#   %08X  %-14s %u-bit%s\n", f.gc, f.name, f.size * 8, f.count > 1 ? " x3" : "");
                out << buf;
            }
        }
        s_status = "Created " + file_path().filename().string();
        return;
    }
    std::string line;
    int bad = 0;
    while (std::getline(in, line)) {
        while (!line.empty() && (line.back() == '\r' || line.back() == ' ' || line.back() == '\t')) line.pop_back();
        if (line.empty() || line[0] == '#' || line[0] == '*') {
            continue;
        }
        if (line[0] == '$' || (line[0] == '-' && line.size() > 1 && line[1] == '$')) {
            s_codes.push_back({line.substr(line[0] == '-' ? 2 : 1), line[0] == '$'});
            continue;
        }
        if (s_codes.empty()) {
            continue;
        }
        bool okA = false, okB = false;
        const size_t sp = line.find_first_of(" \t");
        const uint32_t a = hex(line.substr(0, sp), okA);
        const uint32_t b = sp == std::string::npos ? 0 : hex(line.substr(line.find_first_not_of(" \t", sp)), okB);
        if (!okA || !okB) {
            s_codes.back().supported = false; // encrypted AR codes (xxxx-xxxx-xxxxx) and malformed lines
            bad++;
            continue;
        }
        s_codes.back().lines.push_back({a, b});
        if (!supported(s_codes.back().lines.back())) {
            s_codes.back().supported = false;
        }
    }
    int on = 0, unsupported = 0;
    for (const Code &c : s_codes) {
        on += c.on && c.supported;
        unsupported += c.on && !c.supported;
    }
    s_status = std::to_string(on) + " code(s) active";
    if (unsupported) {
        s_status += ", " + std::to_string(unsupported) + " unsupported (skipped)";
    }
}

const std::string &status()
{
    return s_status;
}

} // namespace mp5native::cheats

extern "C" int mp5native_settings_unlock_bowsers_gnarly_party_c(void)
{
    return mp5native_settings_unlock_bowsers_gnarly_party() ? 1 : 0;
}

// once per game tick, after the game's own logic
extern "C" void Mp5Native_CheatsTick(void)
{
    const auto &game = mp5native::getSettings().game;
    if (game.speedrunMode.getValue()) {
        return;
    }
    if (game.infiniteCoins.getValue() && GWPartyFGet()) {
        for (int i = 0; i < GW_PLAYER_MAX; i++) {
            if (!GwPlayer[i].comF && GwPlayer[i].coin < 999) {
                GwPlayer[i].coin = 999;
            }
        }
    }
    if (!game.enableCheatCodes.getValue()) {
        return;
    }
    static bool loaded = false;
    if (!loaded) {
        mp5native::cheats::reload();
        loaded = true;
    }
    for (const Code &c : s_codes) {
        if (c.on && c.supported) {
            run(c);
        }
    }
}
