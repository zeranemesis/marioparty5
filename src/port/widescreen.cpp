// Adaptive widescreen arithmetic. Adapted from the Mario Party 4 port "Party Board"
// (github.com/zeranemesis/Marioparty4: include/port/widescreen_fit.hpp and
// src/port/settings.cpp), without its safe-area and netplay handling.

#include "port/widescreen.h"
#include "port/settings.h"
#include "game/disp.h"

#include <algorithm>
#include <cassert>
#include <cmath>

#include <dolphin/gx/GXAurora.h>
#include "aurora/lib/window.hpp"

namespace {

constexpr float kMaxScale = 4.0f; // 48:9; a wider window is stretched past it

float aspect_scale(float width, float height)
{
    if (!(width > 0.0f) || !(height > 0.0f)) {
        return 1.0f;
    }
    const float scale = width / height * 0.75f;
    return std::isfinite(scale) ? std::clamp(scale, 1.0f, kMaxScale) : 1.0f;
}

// Where HUD x (0..2*c) is drawn in the widened HUD projection (c-c*s .. c+c*s).
// The middle half stays over the 3D scene it belongs to; the outer quarter on each
// side moves out with the window edge, keeping its margin; the HUD between
// follows part way, so nothing jumps when it slides across.
float placed_x(float hudX, float c, float s)
{
    const float edge = hudX < c ? hudX - c * (s - 1.0f) : hudX + c * (s - 1.0f);
    const float weight = std::clamp((std::fabs(hudX - c) / c - 0.5f) * 4.0f, 0.0f, 1.0f);
    return hudX + (edge - hudX) * weight;
}

float picture_x(float hudX, float c, float s)
{
    return c + (placed_x(hudX, c, s) - c) / s;
}

float hud_x(float pictureX, float c, float s)
{
    // placed_x only grows with hudX: bisect
    const float target = c + (pictureX - c) * s;
    float lo = c - c * 8.0f - std::fabs(target);
    float hi = c + c * 8.0f + std::fabs(target);
    for (int i = 0; i < 40; ++i) {
        const float mid = (lo + hi) * 0.5f;
        (placed_x(mid, c, s) < target ? lo : hi) = mid;
    }
    return (lo + hi) * 0.5f;
}

[[maybe_unused]] const bool selfCheck = [] {
    constexpr float c = 288.0f;
    assert(placed_x(c, c, 4.0f / 3.0f) == c);                          // centre stays
    assert(std::fabs(placed_x(0.0f, c, 4.0f / 3.0f) + 96.0f) < 1e-3f); // left edge to window edge
    assert(placed_x(c * 0.5f, c, 2.0f) == c * 0.5f);                   // middle half stays
    assert(std::fabs(hud_x(picture_x(100.0f, c, 2.0f), c, 2.0f) - 100.0f) < 1e-2f);
    return true;
}();

} // namespace

extern "C" float HuWideFactor(void)
{
    const auto &video = mp5native::getSettings().video;
    if (video.lockAspectRatio.getValue()) {
        AuroraSetViewportPolicy(AURORA_VIEWPORT_FIT);
        return 1.0f;
    }
    if (!video.widescreen.getValue()) {
        AuroraSetViewportPolicy(AURORA_VIEWPORT_STRETCH);
        return 1.0f;
    }
    // the window itself: FIT makes the content framebuffer 4:3
    const AuroraWindowSize size = aurora::window::get_window_size();
    const u32 w = size.native_fb_width, h = size.native_fb_height;
    // narrower than 4:3 is letterboxed rather than cut off
    AuroraSetViewportPolicy((u64)w * 3 < (u64)h * 4 ? AURORA_VIEWPORT_FIT : AURORA_VIEWPORT_STRETCH);
    return aspect_scale((float)w, (float)h);
}

extern "C" float HuWideHudShift(float hudX)
{
    const float s = HuWideFactor();
    if (s == 1.0f || mp5native::getSettings().video.hudCentered.getValue()) {
        return 0.0f;
    }
    return placed_x(hudX, HU_DISP_CENTERX, s) - hudX;
}

extern "C" float HuWideHudX(float pictureX)
{
    const float s = HuWideFactor();
    if (s == 1.0f) {
        return pictureX;
    }
    if (mp5native::getSettings().video.hudCentered.getValue()) {
        return HU_DISP_CENTERX + (pictureX - HU_DISP_CENTERX) * s;
    }
    return hud_x(pictureX, HU_DISP_CENTERX, s);
}

extern "C" float HuSprWidePointShift(float x, float y); // sprman.c

extern "C" float HuWidePictureX(float hudX, float hudY)
{
    const float s = HuWideFactor();
    if (s == 1.0f) {
        return hudX;
    }
    if (mp5native::getSettings().video.hudCentered.getValue()) {
        return HU_DISP_CENTERX + (hudX - HU_DISP_CENTERX) / s;
    }
    // where this frame's sprite layout put that HUD point
    return HU_DISP_CENTERX + (hudX + HuSprWidePointShift(hudX, hudY) - HU_DISP_CENTERX) / s;
}
