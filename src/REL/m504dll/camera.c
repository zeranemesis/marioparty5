#define _MATH_H

#include "REL/m504dll.h"
#include "string.h"

#ifdef __MWERKS__
#define CAM_RODATA __declspec(section ".rodata")
#else
#define CAM_RODATA
#endif

extern s32 rand8(void);
extern double sin();

void PSVECAdd(const Vec *a, const Vec *b, Vec *ab);
void PSVECSubtract(const Vec *a, const Vec *b, Vec *ab);
void PSVECScale(const Vec *src, Vec *dst, f32 scale);

void fn_1_C64C(M504CAMERA *cam, s32 camBit);
void fn_1_C810(M504CAMERA *cam);

s32 lbl_1_data_45E0[4] = { 1, 2, 4, 8 };

CAM_RODATA const Vec lbl_1_rodata_318 = { 0.0f, 0.0f, -1.0f };
CAM_RODATA const Vec lbl_1_rodata_324 = { 0.0f, 2000.0f, 0.0f };
CAM_RODATA const Vec lbl_1_rodata_330 = { 0.0f, 500.0f, 0.0f };
CAM_RODATA const Vec lbl_1_rodata_35C = { 0.0f, 0.0f, 0.0f };

u16 fn_1_C3E0(INPUTWORK *work)
{
    u16 ret = 0;

    switch (work->dif) {
        case 0:
            ret = rand8() % 3;
            break;
        case 1:
            ret = rand8() % 5;
            break;
        case 2:
            ret = (rand8() % 20) + 30;
            break;
        case 3:
            ret = (rand8() % 20) + 30;
            break;
    }
    return ret;
}

void fn_1_C4F4(M504CAMERA *cam, s32 slotNo, struct M504Slot_s *slot)
{
    Vec up;
    Vec posTgt;
    Vec pos;

    memset(cam, 0, sizeof(M504CAMERA));
    cam->slot = slot;
    cam->camBit = lbl_1_data_45E0[slotNo];
    fn_1_C64C(cam, cam->camBit);
    up = lbl_1_rodata_318;
    cam->up = up;
    posTgt = lbl_1_rodata_324;
    cam->posTgt = posTgt;
    pos = lbl_1_rodata_330;
    cam->pos = pos;
    cam->smooth = 0.4f;
    if (slotNo != 0) {
        Hu3DCameraScissorSet(cam->camBit, 0, 0, 0, 0);
    }
}

void fn_1_C64C(M504CAMERA *cam, s32 camBit)
{
    Hu3DCameraCreate(camBit);
    Hu3DCameraViewportSet(camBit, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(camBit, 20.0f, 20.0f, 30000.0f, 1.2f);
    Hu3DCameraScissorSet(camBit, 0, 0, 640, 480);
}

void fn_1_C71C(M504CAMERA *cam)
{
    memset(cam, 0, sizeof(M504CAMERA));
}

void fn_1_C74C(M504CAMERA *cam)
{
    Vec posDelta;
    Vec lookDelta;
    Vec pos;
    Vec look;

    PSVECSubtract(&cam->posTgt, &cam->pos, &posDelta);
    PSVECSubtract(&cam->lookTgt, &cam->look, &lookDelta);
    PSVECScale(&posDelta, &posDelta, cam->smooth);
    PSVECScale(&lookDelta, &lookDelta, cam->smooth);
    PSVECAdd(&cam->pos, &posDelta, &cam->pos);
    PSVECAdd(&cam->look, &lookDelta, &cam->look);
    fn_1_C810(cam);
    PSVECAdd(&cam->pos, &cam->shakePos, &pos);
    PSVECAdd(&cam->look, &cam->shakeLook, &look);
    Hu3DCameraPosSetV(cam->camBit, &pos, &cam->up, &look);
}

void fn_1_C810(M504CAMERA *cam)
{
    f32 v;
    Vec zero;

    if (cam->shakeCnt != 0) {
        v = cam->shakeAmp * sin(3.141592653589793 * cam->shakePhase / 180.0);
        cam->shakePhase += cam->shakeSpd;
        cam->shakeAmp *= cam->shakeDecay;
        if (cam->shakeType == 0 || cam->shakeType == 3) {
            cam->shakePos.x = v;
            cam->shakeLook.x = v;
        } else if (cam->shakeType == 1 || cam->shakeType == 3) {
            cam->shakePos.y = v;
            cam->shakeLook.y = v;
        } else if (cam->shakeType == 2 || cam->shakeType == 3) {
            cam->shakePos.z = v;
            cam->shakeLook.z = v;
        }
        if (--cam->shakeCnt == 0) {
            zero = lbl_1_rodata_35C;
            cam->shakeLook = zero;
            cam->shakePos = cam->shakeLook;
            cam->shakePhase = 0.0f;
            cam->shakeSpd = 0.0f;
            cam->shakeDecay = 0.0f;
            cam->shakeAmp = 0.0f;
        }
    }
}

void fn_1_C99C(M504CAMERA *cam, s16 type, s32 cnt, f32 amp, f32 decay, f32 speed)
{
    cam->shakeType = type;
    cam->shakeCnt = cnt;
    cam->shakeAmp = amp;
    cam->shakeDecay = decay;
    cam->shakeSpd = speed;
    cam->shakePhase = 90.0f;
}

void fn_1_C9C4(void)
{
}

void fn_1_C9C8(Vec *dst, Vec *a, Vec *b, f32 t)
{
    f32 tt;
    f32 s;
    f32 e;
    Vec out;

    tt = t;
    if (tt > 1.0f) {
        tt = 1.0f;
    }
    s = sin(3.141592653589793 * (90.0f * tt) / 180.0);
    e = sin(3.141592653589793 * (90.0f * tt) / 180.0) * s;
    out.x = a->x + e * (b->x - a->x);
    out.y = a->y + e * (b->y - a->y);
    out.z = a->z + e * (b->z - a->z);
    *dst = out;
}

void fn_1_CB3C(M504CAMERA *cam, Vec *pos, Vec *up, Vec *look)
{
    cam->up = *up;
    cam->fromPos = *pos;
    cam->pos = cam->fromPos;
    cam->posTgt = cam->pos;
    cam->fromLook = *look;
    cam->look = cam->fromLook;
    cam->lookTgt = cam->look;
}

void fn_1_CBE8(M504CAMERA *cam, Vec *pos, Vec *up, Vec *look, f32 *smooth)
{
    cam->up = *up;
    cam->posTgt = *pos;
    cam->lookTgt = *look;
    cam->smooth = *smooth;
}

void fn_1_CC3C(M504CAMERA *cam, Vec *pos, Vec *up, Vec *look)
{
    cam->up = *up;
    cam->fromPos = *pos;
    cam->fromLook = *look;
}

void fn_1_CC88(M504CAMERA *cam, Vec *pos, Vec *up, Vec *look)
{
    cam->up = *up;
    cam->toPos = *pos;
    cam->toLook = *look;
    cam->blend = 0.0f;
}

void fn_1_CCE4(M504CAMERA *cam, f32 blend)
{
    cam->blend = blend;
}

s32 fn_1_CCEC(M504CAMERA *cam, Vec *up, f32 duration)
{
    f32 t;
    f32 s;
    f32 e;
    Vec pos;
    Vec look;

    cam->blend += 1.0f / duration;
    t = cam->blend;
    if (t > 1.0f) {
        t = 1.0f;
    }
    s = sin(3.141592653589793 * (90.0f * t) / 180.0);
    e = sin(3.141592653589793 * (90.0f * t) / 180.0) * s;
    pos.x = cam->fromPos.x + e * (cam->toPos.x - cam->fromPos.x);
    pos.y = cam->fromPos.y + e * (cam->toPos.y - cam->fromPos.y);
    pos.z = cam->fromPos.z + e * (cam->toPos.z - cam->fromPos.z);
    t = cam->blend;
    if (t > 1.0f) {
        t = 1.0f;
    }
    s = sin(3.141592653589793 * (90.0f * t) / 180.0);
    e = sin(3.141592653589793 * (90.0f * t) / 180.0) * s;
    look.x = cam->fromLook.x + e * (cam->toLook.x - cam->fromLook.x);
    look.y = cam->fromLook.y + e * (cam->toLook.y - cam->fromLook.y);
    look.z = cam->fromLook.z + e * (cam->toLook.z - cam->fromLook.z);
    cam->up = *up;
    cam->pos = pos;
    cam->posTgt = cam->pos;
    cam->look = look;
    cam->lookTgt = cam->look;
    if (1.0f <= cam->blend) {
        return 1;
    }
    return 0;
}
