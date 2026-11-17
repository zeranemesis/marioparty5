// Translation of GameCube RAM addresses to the port's variables.
//
// A RetroAchievements set reads the console's memory ("the byte at 0x80xxxxxx")
// rather than the game's variables. The port keeps those variables wherever the
// host placed them, in the host's byte order. For each variable the set uses,
// this file rebuilds the bytes the console held at its address
// (config/GP5E01_00/symbols.txt), big-endian.
//
// Any address outside the translated regions is reported as unreadable, and
// rcheevos disables the achievements that use it rather than evaluating them
// against made-up data. Those addresses are logged, so the log lists what is
// left to translate.

#include "port/retroachievements_memory.hpp"

#include <aurora/lib/logging.hpp>

#include <cstdint>
#include <cstdio>
#include <mutex>
#include <set>
#include <string>
#include <vector>

namespace mp5native::ra {

namespace {

aurora::Module Log("mp5native::ra");

constexpr uint32_t kRamBase = 0x80000000u;

// One variable, served as the console laid it out. The image is rebuilt at most
// once per game tick.
struct Region {
    uint32_t start; // console address
    uint32_t size;
    void (*build)(std::vector<uint8_t> &out);
    std::vector<uint8_t> image;
    uint64_t builtAt = ~0ull;
};

// Filled once the set's addresses are known (MP5_RA_DUMP_SET).
std::vector<Region> s_regions;

std::mutex s_mutex;
uint64_t s_tick = 0;
std::set<uint32_t> s_unmapped;

bool read_byte(uint32_t address, uint8_t &out)
{
    for (Region &region : s_regions) {
        if (address >= region.start && address < region.start + region.size) {
            if (region.builtAt != s_tick) {
                region.image.assign(region.size, 0);
                region.build(region.image);
                region.builtAt = s_tick;
            }
            out = region.image[address - region.start];
            return true;
        }
    }
    return false;
}

} // namespace

void beginTick()
{
    std::lock_guard lock(s_mutex);
    ++s_tick;
}

uint32_t readMemory(uint32_t address, uint8_t *buffer, uint32_t numBytes)
{
    std::lock_guard lock(s_mutex);
    for (uint32_t i = 0; i < numBytes; ++i) {
        if (!read_byte(kRamBase + address + i, buffer[i])) {
            s_unmapped.insert(address + i);
            return i;
        }
    }
    return numBytes;
}

void logUnmappedAddresses()
{
    std::lock_guard lock(s_mutex);
    if (s_unmapped.empty()) {
        Log.info("Every GameCube address the set reads is translated");
        return;
    }
    Log.warn("{} GameCube address(es) read by the set are not translated; achievements and "
             "leaderboards using them are disabled:",
        s_unmapped.size());
    std::string line;
    for (const uint32_t address : s_unmapped) {
        char text[16];
        std::snprintf(text, sizeof(text), " %08X", kRamBase + address);
        line += text;
        if (line.size() > 100) {
            Log.warn("{}", line);
            line.clear();
        }
    }
    if (!line.empty()) {
        Log.warn("{}", line);
    }
}

} // namespace mp5native::ra
