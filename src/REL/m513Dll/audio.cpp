// m513Dll audio: positional sound helpers (project a world position through a
// camera to screen space, then pan / attenuate).
#include "REL/m513Dll.h"
#include "game/audio.h"
#include "game/charman.h"

extern "C" {
#include <math.h>
void C_MTXLookAt(Mtx m, const Point3d *camPos, const Vec *camUp, const Point3d *target);
void PSMTXMultVec(const Mtx m, const Vec *src, Vec *dst);
s32 HuAudFXPanning(s32 seNo, s16 pan);
s32 HuAudFXVolSet(s32 seNo, s16 vol);

void fn_1_14348(Point3d *pos, s16 cameraBit, Point3d *out);
s32 fn_1_145A8(s32 seId, Point3d *pos);
void fn_1_14918(s16 charNo, Point3d *pos);
}

// Projects pos through the camera into 640x480 screen space.
#define PROJECT(camIdx, outX, outY, outZ)                                                          \
    HU3DCAMERA *cam = &Hu3DCamera[camIdx];                                                         \
    C_MTXLookAt(mtx, &cam->pos, &cam->up, &cam->target);                                           \
    PSMTXMultVec(mtx, pos, &view);                                                                 \
    w = 1.2 * (view.z * (sin(M_PI * (0.5f * cam->fov) / 180.0) / cos(M_PI * (0.5f * cam->fov) / 180.0))); \
    h = view.z * (sin(M_PI * (0.5f * cam->fov) / 180.0) / cos(M_PI * (0.5f * cam->fov) / 180.0));  \
    outX = 288.0f + (view.x * (288.0f / -w));                                                      \
    outY = 240.0f + (view.y * (240.0f / h));                                                       \
    outZ = 0.0f

void fn_1_14348(Point3d *pos, s16 cameraBit, Point3d *out)
{
    Vec view;
    Mtx mtx;
    f32 w, h;
    s16 i = 0;
    while (i < 16) {
        if ((cameraBit & (1 << i)) != 0) {
            break;
        }
        i++;
    }
    PROJECT(i, out->x, out->y, out->z);
}

s32 fn_1_145A8(s32 seId, Point3d *pos)
{
    Vec view;
    Mtx mtx;
    f32 w, h;
    Vec scr;
    f32 rate;
    s16 vol;
    s32 se;
    s16 i = 0;
    while (i < 16) {
        if (((1 << i) & 1) != 0) {
            break;
        }
        i++;
    }
    PROJECT(i, scr.x, scr.y, scr.z);
    PSVECSubtract(pos, &Hu3DCamera[0].pos, pos);
    rate = (3000.0f - PSVECMag(pos)) / 3000.0f;
    vol = 0;
    if (rate > 0.0f) {
        vol = 104.0f + (23.0f * rate);
    }
    se = HuAudFXPlay(seId);
    HuAudFXPanning(se, (s16)(32.0f + (64.0f * (scr.x / 640.0f))));
    HuAudFXVolSet(se, vol);
    return se;
}

void fn_1_14918(s16 charNo, Point3d *pos)
{
    Vec view;
    Mtx mtx;
    f32 w, h;
    Vec scr;
    s16 i = 0;
    while (i < 16) {
        if (((1 << i) & 1) != 0) {
            break;
        }
        i++;
    }
    PROJECT(i, scr.x, scr.y, scr.z);
    CharModelVoicePanSet(charNo, (s16)(32.0f + (64.0f * (scr.x / 640.0f))));
}
