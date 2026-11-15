// High frame rate with interpolation: see include/port/frame_interp.h.
//
// What is interpolated between the last two logic ticks: cameras (pos/target/up/fov),
// shadow cameras, model root pos/rot/scale and extra matrix, model skeleton poses
// (the per-object transforms the motion evaluation wrote), sprite and sprite group
// pos/rot/scale/alpha, Hu3DParticleCreate particles (pos/scale/zRot/colour per slot, whatever
// hook moves them). Everything else (lights, texture animation,
// shape/cluster deformation, anything a hook draws from its own data) shows the
// latest tick. Rotations are Euler degrees: lerped the short way round.

#include "port/frame_interp.h"
#include "port/imgui.h"
#include "port/settings.h"

extern "C" {
#include "game/hu3d.h"
#include "game/init.h"
#include "game/main.h"
#include "game/object.h"
#include "game/printfunc.h"
#include "game/sprite.h"
#include "game/wipe.h"
void Hu3DInterpCopyClearSet(void); // hsfman.c
}

#include <SDL3/SDL.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "aurora/lib/window.hpp"

SHARED_SYM BOOL HuInterpDrawF = FALSE;
extern "C" void Mp5Test_ProfStart(void);                 // prof_sampler.cpp
extern "C" void Mp5Test_ProfFrame(double t0, double t1); // prof_sampler.cpp

namespace {

constexpr double kTick = 1.0 / 60.0;
constexpr int kMaxTicksPerFrame = 4;  // a longer stall is dropped, not caught up
constexpr float kModelJump = 300.0f;  // world units per tick: further is a teleport
constexpr float kAngleJump = 90.0f;   // degrees per tick: further is a snap
constexpr float kSpriteJump = 80.0f;  // HUD pixels per tick
constexpr float kParJump = 150.0f;    // particle world units per tick: further is a respawn
constexpr float kPi = 3.14159265f;

// the per-particle fields particleFunc draws from (HU3DPARTICLEDATA is 68 bytes, this 28)
struct ParSnap {
    HuVecF pos;
    float scale, zRot;
    GXColor color;
    s16 time;
};

struct ModelSnap {
    const void *id; // hsf, or the hook function of a hook model
    u32 mallocNo;
    u32 attr;
    HuVecF pos, rot, scale;
    Mtx mtx;
    HSFOBJECT *objects;
    s32 objectNum;
    bool posed;     // has a pose below
    bool poseMoved; // pose differs from the previous tick's
    std::vector<HSFTRANSFORM> pose;
    HU3DPARTICLEDATA *parData; // Hu3DParticleCreate model: its particle array, else null
    s32 parCnt;
    std::vector<ParSnap> par; // grows only: no allocation once warm
};

struct SprSnap {
    ANIMDATA *data;
    s16 attr;
    HuVec2f pos, scale;
    float zRot, a;
};

struct GrpSnap {
    s16 sprNum;
    HuVec2f pos, scale;
    float zRot;
};

struct Frame {
    bool valid;
    u32 counter;
    OMOVL ovl;
    s16 pause;
    ModelSnap model[HU3D_MODEL_MAX];
    HU3DCAMERA cam[HU3D_CAM_MAX];
    HU3DSHADOW shadow[HU3D_CAM_MAX];
    SprSnap spr[HUSPR_MAX];
    GrpSnap grp[HUSPR_GRP_MAX];
};

Frame sFrame[2];
int sCurIdx = 0;
Frame &cur() { return sFrame[sCurIdx]; }
Frame &prev() { return sFrame[sCurIdx ^ 1]; }

bool sLerpOk = false; // prev -> cur may be lerped
float sAlpha = 1.0f;
bool sPoseWritten[HU3D_MODEL_MAX];

// ---- pacing state

bool sHfr = false;
double sNextTick = 0.0;
double sNextFrame = 0.0;
int sTicksThisFrame = 0;
int sTickNo = 0;
float sRefresh = 0.0f;
double sRefreshTime = -10.0;

// test bench: MP5_TEST_FPS=<cap> (0 = display), MP5_TEST_FPS_UNCAPPED=1 ignores the refresh rate
int sTestFps = -2;
bool sTestUncapped = false;
double sStatTime = 0.0;
u32 sStatFrames = 0, sStatTicks = 0, sStatInterp = 0;
u32 sStatPar = 0; // particles drawn lerped
double sTLogic = 0, sTDraw = 0, sTPresent = 0, sTWait = 0, sMark = 0;
double sTPar = 0; // particle lerp + restore (part of draw)
double sLastFrame = 0.0;
std::vector<float> sFrameMs; // frame-to-frame times of the stats window

double now_s()
{
    static const double freq = (double)SDL_GetPerformanceFrequency();
    return (double)SDL_GetPerformanceCounter() / freq;
}

// test bench: where a frame's time goes
void mark(double &acc)
{
    const double t = now_s();
    acc += t - sMark;
    sMark = t;
}

float lerp(float a, float b, float t) { return t >= 1.0f ? b : a + (b - a) * t; }

float lerp_angle(float a, float b, float t)
{
    if (t >= 1.0f) {
        return b;
    }
    float d = std::fmod(b - a, 360.0f);
    if (d > 180.0f) {
        d -= 360.0f;
    } else if (d < -180.0f) {
        d += 360.0f;
    }
    return std::fabs(d) > kAngleJump ? b : a + d * t;
}

float dist3(const HuVecF &a, const HuVecF &b)
{
    const float x = a.x - b.x, y = a.y - b.y, z = a.z - b.z;
    return std::sqrt(x * x + y * y + z * z);
}

void lerp3(HuVecF &out, const HuVecF &a, const HuVecF &b, float t)
{
    out.x = lerp(a.x, b.x, t);
    out.y = lerp(a.y, b.y, t);
    out.z = lerp(a.z, b.z, t);
}

void lerp_rot3(HuVecF &out, const HuVecF &a, const HuVecF &b, float t)
{
    out.x = lerp_angle(a.x, b.x, t);
    out.y = lerp_angle(a.y, b.y, t);
    out.z = lerp_angle(a.z, b.z, t);
}

bool has_zero(const HuVecF &v) { return v.x == 0.0f || v.y == 0.0f || v.z == 0.0f; }

bool interp_allowed()
{
    return wipeData.mode == WIPE_MODE_DUMMY && cur().valid;
}

bool same_model(const ModelSnap &a, const ModelSnap &b)
{
    return a.id != nullptr && a.id == b.id && a.mallocNo == b.mallocNo && a.objects == b.objects && a.objectNum == b.objectNum
        && ((a.attr ^ b.attr) & (HU3D_ATTR_DISPOFF | HU3D_ATTR_HOOKFUNC)) == 0;
}

void snapshot()
{
    sCurIdx ^= 1;
    Frame &f = cur();
    const Frame &p = prev();
    f.valid = true;
    f.counter = GlobalCounter;
    f.ovl = omcurovl;
    f.pause = Hu3DPauseF;
    for (int i = 0; i < HU3D_MODEL_MAX; i++) {
        const HU3DMODEL &m = Hu3DData[i];
        ModelSnap &s = f.model[i];
        s.id = (const void *)m.hsf;
        s.posed = false;
        s.poseMoved = false;
        if (!m.hsf) {
            continue;
        }
        s.mallocNo = m.mallocNo;
        s.attr = m.attr;
        s.pos = m.pos;
        s.rot = m.rot;
        s.scale = m.scale;
        std::memcpy(s.mtx, m.mtx, sizeof(Mtx));
        s.objects = nullptr;
        s.objectNum = 0;
        s.parData = nullptr;
        s.parCnt = 0;
        // Hu3DParticleCreate model: what its particle hook left in the array is what the tick drew
        if ((m.attr & (HU3D_ATTR_HOOKFUNC | HU3D_ATTR_PARTICLE)) == (HU3D_ATTR_HOOKFUNC | HU3D_ATTR_PARTICLE) && m.hookData) {
            const HU3DPARTICLE *pp = (const HU3DPARTICLE *)m.hookData;
            if (pp->data && pp->maxCnt > 0) {
                s.parData = pp->data;
                s.parCnt = pp->maxCnt;
                s.par.resize(s.parCnt);
                for (int j = 0; j < s.parCnt; j++) {
                    const HU3DPARTICLEDATA &d = pp->data[j];
                    ParSnap &ps = s.par[j];
                    ps.pos = d.pos;
                    ps.scale = d.scale;
                    ps.zRot = d.zRot;
                    ps.color = d.color;
                    ps.time = d.time;
                }
            }
        }
        // a hook model's hsf is its hook function (Hu3DHookFuncCreate: mallocNo = id + 10000), whatever its attr says now
        if ((m.attr & HU3D_ATTR_HOOKFUNC) || m.mallocNo == (u32)i + 10000) {
            continue;
        }
        s.objects = m.hsf->object;
        s.objectNum = m.hsf->objectNum;
        if (!s.objects || s.objectNum <= 0) {
            continue;
        }
        s.pose.resize(s.objectNum);
        for (int j = 0; j < s.objectNum; j++) {
            s.pose[j] = s.objects[j].mesh.curr;
        }
        s.posed = true;
        const ModelSnap &ps = p.model[i];
        s.poseMoved = ps.posed && same_model(ps, s) && std::memcmp(ps.pose.data(), s.pose.data(), sizeof(HSFTRANSFORM) * s.objectNum) != 0;
    }
    std::memcpy(f.cam, Hu3DCamera, sizeof(f.cam));
    std::memcpy(f.shadow, Hu3DShadowBuf, sizeof(f.shadow));
    for (int i = 0; i < HUSPR_MAX; i++) {
        const HUSPRITE &sp = HuSprData[i];
        SprSnap &s = f.spr[i];
        s.data = (sp.attr & HUSPR_ATTR_FUNC) ? nullptr : sp.data;
        s.attr = sp.attr;
        s.pos = sp.pos;
        s.scale = sp.scale;
        s.zRot = sp.zRot;
        s.a = sp.a;
    }
    for (int i = 0; i < HUSPR_GRP_MAX; i++) {
        const HUSPRGRP &gp = HuSprGrpData[i];
        GrpSnap &s = f.grp[i];
        s.sprNum = gp.sprNum;
        s.pos = gp.pos;
        s.scale = gp.scale;
        s.zRot = gp.zRot;
    }
    // overlay change, pause toggle, a skipped tick or a wipe: snap everything to the new state
    sLerpOk = p.valid && p.counter + 1 == f.counter && p.ovl == f.ovl && p.pause == f.pause && wipeData.mode == WIPE_MODE_DUMMY;
}

void camera_lerp(HU3DCAMERA &out, const HU3DCAMERA &a, const HU3DCAMERA &b, float t)
{
    if (a.fov == -1.0f || b.fov == -1.0f || a.near != b.near || a.far != b.far || a.aspect != b.aspect || a.viewportX != b.viewportX
        || a.viewportY != b.viewportY || a.viewportW != b.viewportW || a.viewportH != b.viewportH || a.scissorX != b.scissorX
        || a.scissorW != b.scissorW) {
        return;
    }
    const float dist = std::max(dist3(b.pos, b.target), 1.0f);
    const float jump = std::max(100.0f, dist * 0.25f);
    if (dist3(a.pos, b.pos) > jump || dist3(a.target, b.target) > jump || std::fabs(a.fov - b.fov) > 10.0f) {
        return; // a cut
    }
    lerp3(out.pos, a.pos, b.pos, t);
    lerp3(out.target, a.target, b.target, t);
    lerp3(out.up, a.up, b.up, t);
    out.fov = lerp(a.fov, b.fov, t);
}

// live values the interpolated frame overwrites, put back afterwards
struct ModelSave {
    HuVecF pos, rot, scale;
    Mtx mtx;
    u32 attr;
};
ModelSave sModelSave[HU3D_MODEL_MAX];
HU3DCAMERA sCamSave[HU3D_CAM_MAX];
HU3DSHADOW sShadowSave[HU3D_CAM_MAX];
HUSPRITE sSprSave[HUSPR_MAX];
HUSPRGRP sGrpSave[HUSPR_GRP_MAX];
bool sParWritten[HU3D_MODEL_MAX];

// alive in both ticks, slot not reused (age did not go back), no jump
bool par_lerpable(const ParSnap &a, const ParSnap &b)
{
    return b.scale != 0.0f && a.scale != 0.0f && b.time >= a.time && dist3(a.pos, b.pos) <= kParJump;
}

float lerp_rad(float a, float b, float t)
{
    float d = std::fmod(b - a, 2.0f * kPi);
    if (d > kPi) {
        d -= 2.0f * kPi;
    } else if (d < -kPi) {
        d += 2.0f * kPi;
    }
    return std::fabs(d) > kPi * 0.5f ? b : a + d * t;
}

// writes the lerped particles into the live array (particleFunc draws from it); restore() puts the tick's back.
// A particle snaps (keeps the tick's values) when it was not alive last tick, its slot was reused
// (age went back) or it moved further than a particle plausibly flies in a tick.
void particles_lerp(int i, const ModelSnap &pa, const ModelSnap &pb, float t)
{
    if (!pb.parData || pa.parData != pb.parData || pa.parCnt != pb.parCnt) {
        return;
    }
    const double t0 = now_s();
    HU3DPARTICLEDATA *d = pb.parData;
    for (int j = 0; j < pb.parCnt; j++) {
        const ParSnap &a = pa.par[j], &b = pb.par[j];
        if (!par_lerpable(a, b)) {
            continue;
        }
        const float dx = b.pos.x - a.pos.x, dy = b.pos.y - a.pos.y, dz = b.pos.z - a.pos.z;
        HU3DPARTICLEDATA &p = d[j];
        p.pos.x = a.pos.x + dx * t;
        p.pos.y = a.pos.y + dy * t;
        p.pos.z = a.pos.z + dz * t;
        p.scale = a.scale + (b.scale - a.scale) * t;
        // 0 means "no rotation" to particleFunc: keep it exact
        p.zRot = (a.zRot == 0.0f || b.zRot == 0.0f) ? b.zRot : lerp_rad(a.zRot, b.zRot, t);
        p.color.r = (u8)(a.color.r + (b.color.r - a.color.r) * t);
        p.color.g = (u8)(a.color.g + (b.color.g - a.color.g) * t);
        p.color.b = (u8)(a.color.b + (b.color.b - a.color.b) * t);
        p.color.a = (u8)(a.color.a + (b.color.a - a.color.a) * t);
        sStatPar++;
    }
    sParWritten[i] = true;
    sTPar += now_s() - t0;
}

void particles_restore(int i)
{
    const double t0 = now_s();
    sParWritten[i] = false;
    const ModelSnap &s = cur().model[i];
    for (int j = 0; j < s.parCnt; j++) {
        const ParSnap &b = s.par[j];
        HU3DPARTICLEDATA &p = s.parData[j];
        p.pos = b.pos;
        p.scale = b.scale;
        p.zRot = b.zRot;
        p.color = b.color;
    }
    sTPar += now_s() - t0;
}

void apply_lerp(float t)
{
    const Frame &a = prev();
    const Frame &b = cur();
    for (int i = 0; i < HU3D_MODEL_MAX; i++) {
        HU3DMODEL &m = Hu3DData[i];
        if (!m.hsf) {
            continue;
        }
        ModelSave &sv = sModelSave[i];
        sv.pos = m.pos;
        sv.rot = m.rot;
        sv.scale = m.scale;
        std::memcpy(sv.mtx, m.mtx, sizeof(Mtx));
        sv.attr = m.attr;
        // draw only: no motion pass flags from the tick, no NOPAUSE updates in hooks
        m.attr &= ~(HU3D_ATTR_MOT_EXEC | HU3D_ATTR_NOPAUSE);
        const ModelSnap &pa = a.model[i];
        const ModelSnap &pb = b.model[i];
        if (sLerpOk && !(m.attr & HU3D_ATTR_DISPOFF) && same_model(pa, pb)) {
            particles_lerp(i, pa, pb, t);
        }
        if (!sLerpOk || !same_model(pa, pb) || dist3(pa.pos, pb.pos) > kModelJump) {
            continue;
        }
        lerp3(m.pos, pa.pos, pb.pos, t);
        lerp_rot3(m.rot, pa.rot, pb.rot, t);
        if (!has_zero(pa.scale) && !has_zero(pb.scale)) {
            lerp3(m.scale, pa.scale, pb.scale, t);
        }
        // the extra matrix carries things attached to other models' joints
        if (std::fabs(pa.mtx[0][3] - pb.mtx[0][3]) + std::fabs(pa.mtx[1][3] - pb.mtx[1][3]) + std::fabs(pa.mtx[2][3] - pb.mtx[2][3]) < kModelJump) {
            for (int r = 0; r < 3; r++) {
                for (int c = 0; c < 4; c++) {
                    m.mtx[r][c] = lerp(pa.mtx[r][c], pb.mtx[r][c], t);
                }
            }
        }
    }
    std::memcpy(sCamSave, Hu3DCamera, sizeof(sCamSave));
    std::memcpy(sShadowSave, Hu3DShadowBuf, sizeof(sShadowSave));
    if (sLerpOk) {
        for (int i = 0; i < HU3D_CAM_MAX; i++) {
            camera_lerp(Hu3DCamera[i], a.cam[i], b.cam[i], t);
            const HU3DSHADOW &sa = a.shadow[i], &sb = b.shadow[i];
            if (sb.buf && dist3(sa.camPos, sb.camPos) <= kModelJump && dist3(sa.camTarget, sb.camTarget) <= kModelJump) {
                lerp3(Hu3DShadowBuf[i].camPos, sa.camPos, sb.camPos, t);
                lerp3(Hu3DShadowBuf[i].camTarget, sa.camTarget, sb.camTarget, t);
                lerp3(Hu3DShadowBuf[i].camUp, sa.camUp, sb.camUp, t);
            }
        }
    }
    std::memcpy(sSprSave, HuSprData, sizeof(sSprSave));
    std::memcpy(sGrpSave, HuSprGrpData, sizeof(sGrpSave));
    if (sLerpOk) {
        for (int i = 0; i < HUSPR_MAX; i++) {
            const SprSnap &sa = a.spr[i], &sb = b.spr[i];
            HUSPRITE &sp = HuSprData[i];
            if (sa.data != sb.data || ((sa.attr ^ sb.attr) & (HUSPR_ATTR_DISPOFF | HUSPR_ATTR_FUNC))
                || std::fabs(sa.pos.x - sb.pos.x) + std::fabs(sa.pos.y - sb.pos.y) > kSpriteJump) {
                continue;
            }
            sp.pos.x = lerp(sa.pos.x, sb.pos.x, t);
            sp.pos.y = lerp(sa.pos.y, sb.pos.y, t);
            sp.scale.x = lerp(sa.scale.x, sb.scale.x, t);
            sp.scale.y = lerp(sa.scale.y, sb.scale.y, t);
            sp.zRot = lerp_angle(sa.zRot, sb.zRot, t);
            sp.a = lerp(sa.a, sb.a, t);
        }
        for (int i = 0; i < HUSPR_GRP_MAX; i++) {
            const GrpSnap &ga = a.grp[i], &gb = b.grp[i];
            HUSPRGRP &gp = HuSprGrpData[i];
            if (ga.sprNum != gb.sprNum || gb.sprNum == 0
                || std::fabs(ga.pos.x - gb.pos.x) + std::fabs(ga.pos.y - gb.pos.y) > kSpriteJump) {
                continue;
            }
            gp.pos.x = lerp(ga.pos.x, gb.pos.x, t);
            gp.pos.y = lerp(ga.pos.y, gb.pos.y, t);
            gp.scale.x = lerp(ga.scale.x, gb.scale.x, t);
            gp.scale.y = lerp(ga.scale.y, gb.scale.y, t);
            gp.zRot = lerp_angle(ga.zRot, gb.zRot, t);
        }
    }
}

void restore()
{
    const Frame &b = cur();
    for (int i = HU3D_MODEL_MAX - 1; i >= 0; i--) {
        HU3DMODEL &m = Hu3DData[i];
        if (sPoseWritten[i]) {
            sPoseWritten[i] = false;
            const ModelSnap &s = b.model[i];
            for (int j = 0; j < s.objectNum; j++) {
                s.objects[j].mesh.curr = s.pose[j];
            }
        }
        if (sParWritten[i]) {
            particles_restore(i);
        }
        if (!m.hsf) {
            continue;
        }
        const ModelSave &sv = sModelSave[i];
        m.pos = sv.pos;
        m.rot = sv.rot;
        m.scale = sv.scale;
        std::memcpy(m.mtx, sv.mtx, sizeof(Mtx));
        m.attr = sv.attr;
    }
    std::memcpy(Hu3DCamera, sCamSave, sizeof(sCamSave));
    std::memcpy(Hu3DShadowBuf, sShadowSave, sizeof(sShadowSave));
    std::memcpy(HuSprData, sSprSave, sizeof(sSprSave));
    std::memcpy(HuSprGrpData, sGrpSave, sizeof(sGrpSave));
}

// the EFB may hold this pass's logic-tick frame: clear it with the copy clear colour (a full-EFB copy with clear)
void clear_efb()
{
    alignas(32) static u8 dummy[32];
    Hu3DInterpCopyClearSet();
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetTexCopySrc(0, 0, RenderMode->fbWidth, RenderMode->efbHeight);
    GXSetTexCopyDst(RenderMode->fbWidth, RenderMode->efbHeight, GX_TF_RGBA8, GX_FALSE);
    GXCopyTex(dummy, GX_TRUE);
}

int target_fps()
{
    auto &video = mp5native::getSettings().video;
    int cap = sTestFps >= 0 ? sTestFps : video.frameRate.getValue();
    if (sTestFps < 0 && !video.frameInterpolation.getValue()) {
        return 60;
    }
    const double t = now_s();
    if (t - sRefreshTime > 1.0) {
        sRefreshTime = t;
        sRefresh = 0.0f;
        if (SDL_Window *win = aurora::window::get_sdl_window()) {
            if (const SDL_DisplayMode *mode = SDL_GetCurrentDisplayMode(SDL_GetDisplayForWindow(win))) {
                sRefresh = mode->refresh_rate;
            }
        }
    }
    const int refresh = (int)std::lround(sRefresh);
    if (cap <= 0) {
        return refresh > 0 ? refresh : 60;
    }
    return refresh > 0 && !sTestUncapped ? std::min(cap, refresh) : cap;
}

void wait_until(double t)
{
    double left = t - now_s();
    if (left > 0.0015) {
        SDL_DelayPrecise((Uint64)((left - 0.001) * 1e9));
    }
    while (now_s() < t) {
    }
}

// test bench: one frame started at t
void frame_stat(double t)
{
    sStatFrames++;
    if (sTestFps >= 0 && sLastFrame != 0.0) {
        sFrameMs.push_back((float)((t - sLastFrame) * 1e3));
        Mp5Test_ProfFrame(sLastFrame, t);
    }
    sLastFrame = t;
}

void stats(double t)
{
    if (sTestFps < 0) {
        return;
    }
    if (sStatTime == 0.0) {
        sStatTime = t;
        return;
    }
    const double dt = t - sStatTime;
    if (dt >= 2.0) {
        const double n = std::max<u32>(sStatFrames, 1);
        std::sort(sFrameMs.begin(), sFrameMs.end());
        const float p99 = sFrameMs.empty() ? 0.0f : sFrameMs[sFrameMs.size() * 99 / 100];
        const float worst = sFrameMs.empty() ? 0.0f : sFrameMs.back();
        std::printf("HFR: render %.2f fps, logic %.2f Hz, interpolated %.2f fps, target %d, refresh %.1f, hfr %d"
                    " | ms/frame logic %.2f draw %.2f present %.2f wait %.2f | frame p99 %.2f max %.2f | particles lerped %.0f/frame in %.3f ms\n",
            sStatFrames / dt, sStatTicks / dt, sStatInterp / dt, target_fps(), sRefresh, (int)sHfr, sTLogic * 1e3 / n,
            sTDraw * 1e3 / n, sTPresent * 1e3 / n, sTWait * 1e3 / n, p99, worst,
            sStatPar / (double)std::max<u32>(sStatInterp, 1), sTPar * 1e3 / std::max<u32>(sStatInterp, 1));
        sFrameMs.clear();
        sTLogic = sTDraw = sTPresent = sTWait = sTPar = 0;
        sStatPar = 0;
        std::fflush(stdout);
        sStatTime = t;
        sStatFrames = sStatTicks = sStatInterp = 0;
    }
}

// test bench: MP5_TEST_FPS_TRACE=<first tick> logs 600 interpolated frames from that tick on:
// time, tick, alpha, camera 0 and the model that moved most in the first traced tick, as drawn
void trace()
{
    static int start = -2, model = -1, left = 600;
    if (start == -2) {
        const char *env = std::getenv("MP5_TEST_FPS_TRACE");
        start = env && *env ? std::atoi(env) : -1;
    }
    if (start < 0 || left <= 0 || (int)cur().counter < start) {
        return;
    }
    if (model < 0) {
        float best = 0.0f;
        for (int i = 0; i < HU3D_MODEL_MAX; i++) {
            const float d = dist3(prev().model[i].pos, cur().model[i].pos);
            if (cur().model[i].id && same_model(prev().model[i], cur().model[i]) && d > best && d < kModelJump) {
                best = d;
                model = i;
            }
        }
        if (model < 0) {
            return;
        }
    }
    left--;
    const HuVecF &c = Hu3DCamera[0].pos, &p = Hu3DData[model].pos;
    std::printf("HFRT: %.6f %u %.4f cam %.4f %.4f %.4f mdl%d %.4f %.4f %.4f rot %.3f\n", now_s(), cur().counter, sAlpha, c.x, c.y, c.z,
        model, p.x, p.y, p.z, Hu3DData[model].rot.y);
}

// test bench: MP5_TEST_FPS_TRACE also logs 1200 frames of one lerped particle (re-picked when it dies or snaps):
// time, tick, alpha, model, slot, position as drawn, then at the tick
void trace_par()
{
    static int start = -2, model = -1, slot = -1, left = 1200;
    if (start == -2) {
        const char *env = std::getenv("MP5_TEST_FPS_TRACE");
        start = env && *env ? std::atoi(env) : -1;
    }
    if (start < 0 || left <= 0 || (int)cur().counter < start) {
        return;
    }
    auto ok = [](int i, int j) {
        const ModelSnap &a = prev().model[i], &b = cur().model[i];
        return sParWritten[i] && j < b.parCnt && a.parData == b.parData && par_lerpable(a.par[j], b.par[j]);
    };
    if (model < 0 || !ok(model, slot)) {
        model = -1;
        for (int i = 0; i < HU3D_MODEL_MAX && model < 0; i++) {
            for (int j = 0; j < cur().model[i].parCnt; j++) {
                if (ok(i, j) && dist3(prev().model[i].par[j].pos, cur().model[i].par[j].pos) > 0.5f) {
                    model = i;
                    slot = j;
                    break;
                }
            }
        }
        if (model < 0) {
            return;
        }
    }
    left--;
    const HuVecF &p = cur().model[model].parData[slot].pos, &b = cur().model[model].par[slot].pos;
    std::printf("HFRP: %.6f %u %.4f %d %d %.4f %.4f %.4f tick %.4f %.4f %.4f\n", now_s(), cur().counter, sAlpha, model, slot, p.x, p.y,
        p.z, b.x, b.y, b.z);
}

} // namespace

extern "C" BOOL HuInterpPoseSet(s16 modelId)
{
    const ModelSnap &pa = prev().model[modelId];
    const ModelSnap &pb = cur().model[modelId];
    if (!sLerpOk || !pb.posed || !pb.poseMoved || !pa.posed || !same_model(pa, pb) || sPoseWritten[modelId]
        || dist3(pa.pos, pb.pos) > kModelJump) {
        return FALSE;
    }
    sPoseWritten[modelId] = true;
    const float t = sAlpha;
    for (int j = 0; j < pb.objectNum; j++) {
        HSFOBJECT &obj = pb.objects[j];
        if (obj.type == HSF_OBJ_CAMERA || obj.type == HSF_OBJ_LIGHT) {
            continue;
        }
        const HSFTRANSFORM &a = pa.pose[j], &b = pb.pose[j];
        HSFTRANSFORM &out = obj.mesh.curr;
        if (dist3(a.pos, b.pos) <= kModelJump) {
            lerp3(out.pos, a.pos, b.pos, t);
        }
        lerp_rot3(out.rot, a.rot, b.rot, t);
        if (!has_zero(a.scale) && !has_zero(b.scale)) {
            lerp3(out.scale, a.scale, b.scale, t);
        }
    }
    return TRUE;
}

extern "C" int HuInterpFrameBegin(void)
{
    if (sTestFps == -2) {
        const char *env = std::getenv("MP5_TEST_FPS");
        sTestFps = env && *env ? std::atoi(env) : -1;
        const char *uncap = std::getenv("MP5_TEST_FPS_UNCAPPED");
        sTestUncapped = uncap && *uncap == '1';
        Mp5Test_ProfStart();
    }
    const bool hfr = target_fps() > 60;
    const double t = now_s();
    stats(t);
    if (hfr != sHfr) {
        sHfr = hfr;
        sNextTick = sNextFrame = t;
        sFrame[0].valid = sFrame[1].valid = false;
        sLerpOk = false;
    }
    if (!sHfr) {
        sTicksThisFrame = 1;
        frame_stat(t);
        sMark = t;
        return 1;
    }
    int ticks = 0;
    while (t >= sNextTick && ticks < kMaxTicksPerFrame) {
        ticks++;
        sNextTick += kTick;
    }
    if (t >= sNextTick) {
        sNextTick = t + kTick; // a stall: drop the rest
    }
    if (ticks == 0 && !interp_allowed()) {
        // nothing new to draw (a wipe, or no state yet): wait for the tick
        wait_until(sNextTick);
        return -1;
    }
    sTicksThisFrame = ticks;
    sTickNo = 0;
    frame_stat(t);
    sMark = now_s();
    return ticks;
}

extern "C" void HuInterpTickEnd(void)
{
    sStatTicks++;
    if (sHfr) {
        snapshot();
        if (++sTickNo < sTicksThisFrame) {
            clear_efb(); // catching up: the next tick draws the frame again
        }
    }
    mark(sTLogic);
}

extern "C" void HuInterpDraw(void)
{
    if (!sHfr || !interp_allowed() || (sTicksThisFrame > 0 && !sLerpOk)) {
        // at 60 fps, in a wipe, or after a snap: the tick's own frame is the latest state already
        return;
    }
    sAlpha = (float)std::clamp(1.0 - (sNextTick - now_s()) / kTick, 0.0, 1.0);
    sStatInterp++;

    const u32 counter = GlobalCounter;
    const s16 pauseF = Hu3DPauseF;
    const s16 cameraNo = Hu3DCameraNo, cameraBit = Hu3DCameraBit;
    HU3DSHADOW *shadow = Hu3DShadow;
    Mtx cameraMtx, cameraMtxXPose;
    std::memcpy(cameraMtx, Hu3DCameraMtx, sizeof(Mtx));
    std::memcpy(cameraMtxXPose, Hu3DCameraMtxXPose, sizeof(Mtx));

    apply_lerp(sAlpha);
    trace();
    trace_par();
    // hooks see the counter the tick drew with (particle updates are once per counter) and a pause
    GlobalCounter = cur().counter - 1;
    Hu3DPauseF = TRUE;
    HuInterpDrawF = TRUE;

    clear_efb();
    HuSysBeforeRender();
    GXSetAlphaUpdate(GX_TRUE);
    Hu3DExec();
    pfDrawFonts();
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetColorUpdate(GX_TRUE);
    GXDrawDone();

    HuInterpDrawF = FALSE;
    restore();
    GlobalCounter = counter;
    Hu3DPauseF = pauseF;
    Hu3DCameraNo = cameraNo;
    Hu3DCameraBit = cameraBit;
    Hu3DShadow = shadow;
    std::memcpy(Hu3DCameraMtx, cameraMtx, sizeof(Mtx));
    std::memcpy(Hu3DCameraMtxXPose, cameraMtxXPose, sizeof(Mtx));
    mark(sTDraw);
}

extern "C" void HuInterpFrameEnd(void)
{
    mark(sTPresent);
    if (!sHfr) {
        frame_limiter();
        mark(sTWait);
        return;
    }
    const double interval = 1.0 / std::max(target_fps(), 1);
    const double t = now_s();
    sNextFrame += interval;
    if (sNextFrame < t - interval) {
        sNextFrame = t; // behind: start again from now, no burst
    }
    wait_until(sNextFrame);
    mark(sTWait);
}
