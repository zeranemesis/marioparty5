// m517Dll audio: positional sound helpers (project a world position through a
// camera to 640x480 screen space, then pan the sound or voice).
#include "REL/m517Dll.h"
extern "C" {
#include "game/audio.h"
#include "game/charman.h"
#include <math.h>
void C_MTXLookAt(Mtx m, const Point3d *camPos, const Vec *camUp, const Point3d *target);
void PSMTXMultVec(const Mtx m, const Vec *src, Vec *dst);

void fn_1_1B48C(Point3d *pos, s16 cameraBit, Point3d *out);
s32 fn_1_1B6EC(s32 seId, Point3d *pos);
void fn_1_1B9B8(s16 charNo, Point3d *pos);
void fn_1_1BC6C(s32 seNo, Point3d *pos);
}

// Projects pos through camera camIdx into screen space.
#define M517_PROJECT(camIdx, outX, outY, outZ)                                                     \
    HU3DCAMERA *cam = &Hu3DCamera[camIdx];                                                         \
    C_MTXLookAt(mtx, &cam->pos, &cam->up, &cam->target);                                           \
    PSMTXMultVec(mtx, pos, &view);                                                                 \
    w = 1.2f * (view.z * (sin(M_PI * (0.5f * cam->fov) / 180.0) / cos(M_PI * (0.5f * cam->fov) / 180.0))); \
    h = view.z * (sin(M_PI * (0.5f * cam->fov) / 180.0) / cos(M_PI * (0.5f * cam->fov) / 180.0));  \
    outX = 288.0f + (view.x * (288.0f / -w));                                                      \
    outY = 240.0f + (view.y * (240.0f / h));                                                       \
    outZ = 0.0f

// Screen position of pos as seen by the first camera of cameraBit.
void fn_1_1B48C(Point3d *pos, s16 cameraBit, Point3d *out)
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
    M517_PROJECT(i, out->x, out->y, out->z);
}

// Play a sound effect panned according to the screen position of pos.
s32 fn_1_1B6EC(s32 seId, Point3d *pos)
{
    Vec view;
    Mtx mtx;
    f32 w, h;
    Vec scr;
    s32 pan;
    s32 se;
    s16 i = 0;

    while (i < 16) {
        if (((1 << i) & 1) != 0) {
            break;
        }
        i++;
    }
    M517_PROJECT(i, scr.x, scr.y, scr.z);
    pan = (s32)(40.0f + (48.0f * (scr.x / 640.0f)));
    se = HuAudFXPlay(seId);
    HuAudFXPanning(se, pan);
    return se;
}

// Pan a character voice according to the screen position of pos.
void fn_1_1B9B8(s16 charNo, Point3d *pos)
{
    Vec view;
    Mtx mtx;
    f32 w, h;
    Vec scr;
    s32 pan;
    s16 i = 0;

    while (i < 16) {
        if (((1 << i) & 1) != 0) {
            break;
        }
        i++;
    }
    M517_PROJECT(i, scr.x, scr.y, scr.z);
    pan = (s32)(40.0f + (48.0f * (scr.x / 640.0f)));
    CharModelVoicePanSet(charNo, pan);
}

// Pan a playing sound effect according to the screen position of pos.
void fn_1_1BC6C(s32 seNo, Point3d *pos)
{
    Vec view;
    Mtx mtx;
    f32 w, h;
    Vec scr;
    s32 pan;
    s16 i = 0;

    while (i < 16) {
        if (((1 << i) & 1) != 0) {
            break;
        }
        i++;
    }
    M517_PROJECT(i, scr.x, scr.y, scr.z);
    pan = (s32)(40.0f + (48.0f * (scr.x / 640.0f)));
    HuAudFXPanning(seNo, pan);
}
