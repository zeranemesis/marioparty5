#include "REL/m531Dll.h"

extern "C" {
#include <math.h>
void C_MTXLookAt(Mtx m, const Point3d *camPos, const Vec *camUp, const Point3d *target);
void PSMTXMultVec(const Mtx m, const Vec *src, Vec *dst);

void fn_1_CB98(Point3d *pos, s16 cameraBit, Point3d *out);
int fn_1_CDF8(int seId, Point3d *pos);
void fn_1_D0C4(s16 charNo, Point3d *pos);
void fn_1_D378(int seNo, Point3d *pos);
}

// Projects pos through camera 0 (or the first camera in a bit mask) into 640x480 screen space.
#define PROJECT(camIdx, outX, outY, outZ)                                                          \
    HU3DCAMERA *cam = &Hu3DCamera[camIdx];                                                         \
    C_MTXLookAt(mtx, &cam->pos, &cam->up, &cam->target);                                           \
    PSMTXMultVec(mtx, pos, &view);                                                                 \
    w = 1.2 * (view.z * (sin(M_PI * (cam->fov * 0.5f) / 180.0) / cos(M_PI * (cam->fov * 0.5f) / 180.0))); \
    h = view.z * (sin(M_PI * (cam->fov * 0.5f) / 180.0) / cos(M_PI * (cam->fov * 0.5f) / 180.0));  \
    outX = 288.0f + (view.x * (288.0f / -w));                                                      \
    outY = 240.0f + (view.y * (240.0f / h));                                                       \
    outZ = 0.0f

void fn_1_CB98(Point3d *pos, s16 cameraBit, Point3d *out)
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

int fn_1_CDF8(int seId, Point3d *pos)
{
    Vec view;
    Mtx mtx;
    f32 w, h;
    Vec scr;
    int se;
    s16 i = 0;
    while (i < 16) {
        if (((1 << i) & 1) != 0) {
            break;
        }
        i++;
    }
    PROJECT(i, scr.x, scr.y, scr.z);
    se = HuAudFXPlay(seId);
    HuAudFXPanning(se, (s16)(48.0f + (32.0f * (scr.x / 640.0f))));
    return se;
}

void fn_1_D0C4(s16 charNo, Point3d *pos)
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
    CharModelVoicePanSet(charNo, (s16)(48.0f + (32.0f * (scr.x / 640.0f))));
}

void fn_1_D378(int seNo, Point3d *pos)
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
    HuAudFXPanning(seNo, (s16)(48.0f + (32.0f * (scr.x / 640.0f))));
}
