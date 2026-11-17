// Settings > Loading Speed = GameCube: disc reads take the time the console's drive took.
//
// The port reads the disc image at once, so every load ended as soon as it started. This models the
// GameCube drive instead: a constant angular velocity (CAV) disc, so the transfer rate grows with the
// radius the data sits at, plus a head seek whenever a read does not follow the previous one. One
// drive serves one read at a time, so reads queue behind each other.
//
// Blocking reads (the console's CPU spins until the drive is done) sleep the calling thread until the
// modelled end. Background reads (the console's drive works while the game runs) report completion
// through Mp5Native_DvdTimingPump, called once per game tick on the main thread, so no other thread
// touches game state. The constants were tuned against Dolphin with its disc speed emulation on
// (see the commit that introduced this file).

#include <SDL3/SDL_timer.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <vector>

#include "port/settings.h"

namespace {

// Nintendo GameCube optical disc (8 cm): the data band between these radii, in millimetres.
constexpr double kDiscBytes = 1459978240.0;
constexpr double kInnerRadius = 24.0;
constexpr double kOuterRadius = 40.0;
// Transfer rate at the outer edge (bytes/s); CAV: proportional to the radius.
constexpr double kOuterRate = 3.125e6;
// Head seek: a fixed settle time plus a part proportional to the radial distance.
constexpr double kSeekBase = 0.060;
constexpr double kSeekFull = 0.105;
// Command overhead of each read.
constexpr double kCommand = 0.002;
// Reads closer than this to the previous end continue without a seek (read-ahead).
constexpr double kSequentialGap = 64.0 * 1024.0;

double s_driveFreeAt = 0.0;
double s_lastEnd = -1.0;

struct Deferred {
    double at;
    void (*fn)(void *);
    void *arg;
};
std::vector<Deferred> s_deferred;

double radius_at(double offset)
{
    const double t = std::clamp(offset / kDiscBytes, 0.0, 1.0);
    // equal areas per byte: r^2 grows linearly with the offset
    return std::sqrt(kInnerRadius * kInnerRadius + (kOuterRadius * kOuterRadius - kInnerRadius * kInnerRadius) * t);
}

} // namespace

extern "C" {

double Mp5Native_DvdTimingNow(void)
{
    return static_cast<double>(SDL_GetTicksNS()) * 1e-9;
}

int Mp5Native_DvdTimingOn(void)
{
    // MP5_TEST_LOADSPEED=0|1: test bench override of the setting
    static const char *env = std::getenv("MP5_TEST_LOADSPEED");
    if (env != nullptr && *env != 0) {
        return std::atoi(env) == 1;
    }
    return mp5native::getSettings().game.loadingSpeed.getValue() == 1;
}

// The read [offset, offset + length) of the disc was issued now: when the drive is done with it.
double Mp5Native_DvdTimingRead(uint32_t offset, uint32_t length)
{
    const double now = Mp5Native_DvdTimingNow();
    double t = std::max(now, s_driveFreeAt) + kCommand;
    const double start = static_cast<double>(offset);
    if (s_lastEnd < 0.0 || std::fabs(start - s_lastEnd) > kSequentialGap) {
        const double dr = std::fabs(radius_at(start) - radius_at(std::max(s_lastEnd, 0.0)));
        t += kSeekBase + kSeekFull * dr / (kOuterRadius - kInnerRadius);
    }
    const double rate = kOuterRate * radius_at(start + length * 0.5) / kOuterRadius;
    t += static_cast<double>(length) / rate;
    s_driveFreeAt = t;
    s_lastEnd = start + length;
    return t;
}

void Mp5Native_DvdTimingSleepUntil(double deadline)
{
    const double left = deadline - Mp5Native_DvdTimingNow();
    if (left > 0.0) {
        SDL_DelayPrecise(static_cast<Uint64>(left * 1e9));
    }
}

// Run fn(arg) on the main thread once the deadline has passed (the end of a background read).
void Mp5Native_DvdTimingDefer(double deadline, void (*fn)(void *), void *arg)
{
    s_deferred.push_back({deadline, fn, arg});
}

// Once per game tick on the main thread, and from the game's own wait loops: completes the background
// reads whose time has come, in the order they were issued.
void Mp5Native_DvdTimingPump(void)
{
    if (s_deferred.empty()) {
        return;
    }
    const double now = Mp5Native_DvdTimingNow();
    for (size_t i = 0; i < s_deferred.size();) {
        if (s_deferred[i].at <= now) {
            const Deferred d = s_deferred[i];
            s_deferred.erase(s_deferred.begin() + static_cast<std::ptrdiff_t>(i));
            d.fn(d.arg);
        } else {
            ++i;
        }
    }
}

} // extern "C"
