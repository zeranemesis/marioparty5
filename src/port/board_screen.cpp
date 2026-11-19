// Second screen (AYN Thor bottom display, BoardScreen.java): a live view of the board from above, rendered by the
// game's own 3D engine (Hu3DExec draws camera 0 a second time into an offscreen target, nothing advances), read
// back to memory 15 times a second. The players are left out of that render and shown as tokens at their live
// position, with their stars and coins.
#include <algorithm>
#include <atomic>
#include <cmath>
#include <mutex>

extern "C" {
#include "game/board/masu.h"
#include "game/board/model.h"
#include "game/board/player.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/object.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
HU3DMODELID MBMasuDrawModelGet(void);
}

namespace {
constexpr int kW = 620, kH = 540; // half the bottom screen (1240x1080)
constexpr int kHeader = 6;        // active, frame, turnNo, turnMax, turnPlayerNo, teamF
constexpr int kPlayer = 7;        // u, v (0..1 on the view, < 0: off it), r, g, b, coins, stars
constexpr int kInfo = kHeader + GW_PLAYER_MAX * kPlayer;

std::atomic_bool g_screenOn{false}; // the Java side shows the second screen
alignas(32) u8 g_copy[kW * kH * 4];
bool g_copyPending;
HU3DCAMERA g_cam;

std::mutex g_mutex; // guards the two below, read by the Java thread
u32 g_pixels[kW * kH];
float g_info[kInfo];
u32 g_frame;

bool OnBoard() {
    return omcurovl >= DLL_w01dll && omcurovl <= DLL_w20dll && MBMasuNumGet(MASU_LAYER_DEFAULT) > 0;
}

// GX_TF_RGBA8 in the GameCube layout (4x4 tiles: 16 AR pairs, then 16 GB pairs) to ARGB for an Android Bitmap
void Decode() {
    const u8* src = g_copy;
    for (int ty = 0; ty < kH; ty += 4) {
        for (int tx = 0; tx < kW; tx += 4, src += 64) {
            for (int i = 0; i < 16; ++i) {
                const int x = tx + (i & 3), y = ty + (i >> 2);
                if (x < kW && y < kH) {
                    const u8 a = src[i * 2], r = src[i * 2 + 1], g = src[32 + i * 2], b = src[32 + i * 2 + 1];
                    (void)a; // the board is opaque; the copy's alpha is the EFB's, not coverage
                    g_pixels[y * kW + x] = 0xFF000000u | (u32{r} << 16) | (u32{g} << 8) | b;
                }
            }
        }
    }
}
} // namespace

// Hu3DExec, after the frame's normal camera loop: sets camera 0 above the board and starts the offscreen target
extern "C" BOOL Mp5BoardScreen_OverheadBegin(HU3DCAMERA* cam) {
    static u32 tick;
    if (!g_screenOn || !OnBoard() || ++tick % 4 != 0) { // 15 views a second at 60 fps
        return FALSE;
    }
    // the previous view, copied 4 frames ago: its frame was submitted since, so this does not wait (it would
    // forever from inside the frame that issued the copy)
    if (g_copyPending) {
        g_copyPending = false;
        if (AuroraGXCopyReadbackSync(g_copy)) {
            std::lock_guard lock{g_mutex};
            Decode();
            ++g_frame;
        }
    }
    const int num = MBMasuNumGet(MASU_LAYER_DEFAULT);
    float minX = 1e9f, maxX = -1e9f, minZ = 1e9f, maxZ = -1e9f, sumY = 0;
    for (int i = 1; i <= num; ++i) {
        const MASU* m = MBMasuGet(MASU_LAYER_DEFAULT, i);
        minX = std::min(minX, m->pos.x), maxX = std::max(maxX, m->pos.x);
        minZ = std::min(minZ, m->pos.z), maxZ = std::max(maxZ, m->pos.z);
        sumY += m->pos.y;
    }
    const float cx = (minX + maxX) / 2, cz = (minZ + maxZ) / 2, cy = sumY / num;
    const float aspect = float(kW) / kH, fov = 30.f;
    const float half = std::tan(fov * 0.5f * float(M_PI) / 180.f);
    // fit the spaces with a margin, seen slightly tilted (as the game's own map view) so models keep some depth
    const float dist = std::max((maxX - minX) / (2 * half * aspect), (maxZ - minZ) / (2 * half)) * 1.15f;
    cam->fov = fov;
    cam->aspect = aspect;
    cam->near = dist * 0.1f;
    cam->far = dist * 4.f;
    cam->target = {cx, cy, cz};
    cam->pos = {cx, cy + dist * 0.97f, cz + dist * 0.25f};
    cam->up = {0, 0, -1};
    cam->viewportX = cam->viewportY = 0;
    cam->viewportW = kW, cam->viewportH = kH;
    cam->viewportNear = 0, cam->viewportFar = 1;
    cam->scissorX = cam->scissorY = 0;
    cam->scissorW = kW, cam->scissorH = kH;
    g_cam = *cam;
    GXCreateFrameBuffer(kW, kH);
    return TRUE;
}

// Hu3DExec, after the overhead pass: copy the view to memory (collected by the next OverheadBegin), back to the EFB
extern "C" void Mp5BoardScreen_OverheadEnd(void) {
    GXSetTexCopySrc(0, 0, kW, kH);
    GXSetTexCopyDst(kW, kH, GX_TF_RGBA8, GX_FALSE);
    AuroraGXSetCopyReadback(GX_TRUE);
    GXCopyTex(g_copy, GX_FALSE);
    AuroraGXSetCopyReadback(GX_FALSE);
    GXRestoreFrameBuffer();
    g_copyPending = true;
}

// Left out of the overhead pass: the players (tokens instead), and effects whose draw hooks advance them
extern "C" BOOL Mp5BoardScreen_ModelHidden(s16 modelId) {
    if ((Hu3DData[modelId].attr & HU3D_ATTR_HOOKFUNC) && modelId != MBMasuDrawModelGet()) {
        return TRUE;
    }
    for (int p = 0; p < GW_PLAYER_MAX; ++p) {
        if (MBModelIdGet(MBPlayerModelGet(p)) == modelId) {
            return TRUE;
        }
    }
    return FALSE;
}

// Game thread, once per vsync (pad.c): the players and the turn
extern "C" void Mp5BoardScreen_Hook(void) {
    if (!g_screenOn) {
        return;
    }
    const bool board = OnBoard();
    float info[kInfo] = {};
    if (board) {
        info[0] = 1;
        info[2] = GwSystem.turnNo;
        info[3] = GwSystem.turnMax;
        info[4] = GwSystem.turnPlayerNo;
        info[5] = GwSystem.teamF;
        Mtx view;
        MTXLookAt(view, &g_cam.pos, &g_cam.up, &g_cam.target);
        const float half = std::tan(g_cam.fov * 0.5f * float(M_PI) / 180.f);
        for (int p = 0; p < GW_PLAYER_MAX; ++p) {
            HuVecF pos = {};
            MBPlayerPosGet(p, &pos);
            Vec v;
            MTXMultVec(view, &pos, &v);
            float* o = info + kHeader + p * kPlayer;
            o[0] = o[1] = -1;
            if (v.z < 0) {
                o[0] = 0.5f + 0.5f * (v.x / -v.z) / (half * g_cam.aspect);
                o[1] = 0.5f - 0.5f * (v.y / -v.z) / half;
            }
            const GXColor c = MBPlayerColorGet(p);
            o[2] = c.r, o[3] = c.g, o[4] = c.b;
            o[5] = MBPlayerCoinGet(p);
            o[6] = MBPlayerStarGet(p);
        }
    }
    std::lock_guard lock{g_mutex};
    info[1] = static_cast<float>(g_frame);
    std::copy(std::begin(info), std::end(info), std::begin(g_info));
}

#ifdef __ANDROID__
#include <SDL3/SDL_events.h>
#include <jni.h>

#define BOARD_SCREEN_JNI(name) Java_io_github_zeranemesis_mp5native_BoardScreen_##name

extern "C" JNIEXPORT void JNICALL BOARD_SCREEN_JNI(nativeSetActive)(JNIEnv*, jclass, jboolean on) {
    g_screenOn = on;
}

// info: see kHeader / kPlayer; pixels (kW x kH ARGB) only filled when the view changed since `lastFrame`
extern "C" JNIEXPORT jboolean JNICALL BOARD_SCREEN_JNI(nativeFrame)(JNIEnv* env, jclass, jfloatArray info,
                                                                    jintArray pixels, jint lastFrame) {
    if (env->GetArrayLength(info) < kInfo || env->GetArrayLength(pixels) < kW * kH) {
        return JNI_FALSE;
    }
    std::lock_guard lock{g_mutex};
    env->SetFloatArrayRegion(info, 0, kInfo, g_info);
    if (static_cast<u32>(lastFrame) == g_frame) {
        return JNI_FALSE;
    }
    env->SetIntArrayRegion(pixels, 0, kW * kH, reinterpret_cast<const jint*>(g_pixels));
    return JNI_TRUE;
}

// The Settings button: the same key as the menu shortcut (F1, the pad's Back button).
extern "C" JNIEXPORT void JNICALL BOARD_SCREEN_JNI(nativeOpenMenu)(JNIEnv*, jclass) {
    for (const bool down : {true, false}) {
        SDL_Event e{};
        e.type = down ? SDL_EVENT_KEY_DOWN : SDL_EVENT_KEY_UP;
        e.key.key = SDLK_F1;
        e.key.scancode = SDL_SCANCODE_F1;
        e.key.down = down;
        SDL_PushEvent(&e);
    }
}
#endif
