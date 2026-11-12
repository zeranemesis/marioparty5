#include <dolphin.h>

#include "datadir_enum.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/pad.h"
#include "humath.h"

typedef struct Mgr511Ptmf {
    s32 type;
    s32 param;
    void (*func)(void);
} Mgr511Ptmf;

extern s32 __ptmf_test(void *ptmf);
extern void __ptmf_scall(void *self, register const Mgr511Ptmf *ptmf);
extern void *__dl__FPv(void *self);
extern void fn_1_CF8(char *cond, char *file, s32 line);
extern void fn_1_1D88(void *self);
extern void fn_1_1D94(void *self, s32 arg1);
extern void fn_1_1E54(void *self, void **arg1);
extern s32 fn_1_29C8(void);
extern void fn_1_112E0(s32 idx, Vec *pos, Vec *target);
extern void fn_1_1167C(s32 idx, Vec *pos, Vec *target);

/* Position, up vector and target of one camera. */
typedef struct Mgr511Pose {
    Vec pos;
    Vec up;
    Vec target;
} Mgr511Pose;

extern void fn_1_172C(Mgr511Pose *pose, s32 mode);
extern void fn_1_14C4(Mgr511Pose *pose, u16 cameraBit);

/* One of the four split-screen cameras. */
typedef struct Mgr511CamElem {
    Mgr511Pose pose;
    f32 x;
    f32 y;
    f32 pad_2C;
    f32 w;
    f32 h;
    f32 pad_38;
    f32 pad_3C;
    u8 active;
} Mgr511CamElem;

typedef struct Mgr511Camera {
    void *next;
    void *prev;
    void **vtable;
    u32 flags;
    f32 splitX;
    f32 splitY;
    s32 pad_18;
    Mgr511CamElem elem[4];
    Mgr511Pose freeCam;
    s32 counter;
    Mgr511Ptmf ptmf;
    s16 motionModel;
    s32 split;
} Mgr511Camera;

typedef struct Mgr511Arr12 {
    f32 v[12];
} Mgr511Arr12;

extern void *lbl_1_data_40[];
extern void *lbl_1_bss_2C[];

void fn_1_A278(Mgr511Camera *cam);
void fn_1_A3B4(Mgr511Camera *cam);
void fn_1_A580(Mgr511Camera *cam);
void fn_1_ABA8(Mgr511Camera *cam);
void fn_1_AD04(Mgr511Camera *cam);
void fn_1_B324(Mgr511Camera *cam, s32 mode);
void fn_1_B4C0(Mgr511Camera *cam);
void *fn_1_BC4C(Mgr511Camera *cam, s16 arg1);

Mgr511Camera *lbl_1_bss_A3C;
f32 lbl_1_bss_A50[4];
f32 lbl_1_bss_A60;
u8 lbl_1_bss_A64;
f32 lbl_1_bss_A68;
u8 lbl_1_bss_A6C;
s32 lbl_1_bss_A70;

static Mgr511Ptmf lbl_1_data_368 = { 0, -1, (void (*)(void))fn_1_A278 };
static char lbl_1_data_374[] = "ms_pointer == 0";
static char lbl_1_data_384[] = "singleton.h";
static char lbl_1_data_390[] = "ms_pointer != 0";
static Mgr511Ptmf lbl_1_data_3A0 = { 0, -1, (void (*)(void))fn_1_A3B4 };
static Mgr511Ptmf lbl_1_data_3AC = { 0, -1, (void (*)(void))fn_1_A580 };
static Mgr511Ptmf lbl_1_data_3B8 = { 0, -1, (void (*)(void))fn_1_AD04 };
static Mgr511Ptmf lbl_1_data_3C4 = { 0, -1, (void (*)(void))fn_1_ABA8 };
static Mgr511Ptmf lbl_1_data_3D0 = { 0, -1, (void (*)(void))fn_1_A3B4 };

void *lbl_1_data_3DC[5] = {
    0,
    0,
    (void *)fn_1_BC4C,
    (void *)fn_1_B324,
    (void *)fn_1_B4C0,
};

const s32 lbl_1_rodata_248[4] = { 1, 2, 4, 8 };
const Mgr511Arr12 lbl_1_rodata_258 = { {
    0.0f, 0.0f, 0.0f, 320.0f, 0.0f, 0.0f, 0.0f, 240.0f, 0.0f, 320.0f, 240.0f, 0.0f,
} };
const Vec lbl_1_rodata_288 = { 0.0f, 2800.0f, 3000.0f };
const Vec lbl_1_rodata_294 = { 0.0f, 1.0f, 0.0f };
const Vec lbl_1_rodata_2A0 = { 0.0f, 0.0f, 200.0f };
const f32 lbl_1_rodata_2D4[1] = { 0.1f };
const f32 lbl_1_rodata_2D8[1] = { 0.05f };

Mgr511Camera *fn_1_97C8(Mgr511Camera *cam)
{
    s32 i;
    s32 j;
    Mgr511CamElem *e;
    Mgr511Arr12 init;

    fn_1_1D88(cam);
    cam->vtable = lbl_1_data_40;
    cam->flags = 0;
    fn_1_1E54(cam, lbl_1_bss_2C);
    if (lbl_1_bss_A3C != 0) {
        fn_1_CF8(lbl_1_data_374, lbl_1_data_384, 0x35);
    }
    lbl_1_bss_A3C = cam;
    cam->vtable = lbl_1_data_3DC;
    cam->ptmf = lbl_1_data_368;
    cam->split = -1;
    init = lbl_1_rodata_258;
    for (i = 0; i < 4; i++) {
        e = &cam->elem[i];
        e->pose.pos.x = 0.0f;
        e->pose.pos.y = lbl_1_rodata_288.y;
        e->pose.pos.z = lbl_1_rodata_288.z;
        e->pose.up.x = 0.0f;
        e->pose.up.y = lbl_1_rodata_294.y;
        e->pose.up.z = lbl_1_rodata_294.z;
        e->pose.target.x = 0.0f;
        e->pose.target.y = lbl_1_rodata_2A0.y;
        e->pose.target.z = lbl_1_rodata_2A0.z;
        e->active = 0;
        cam->counter = 0;
    }
    for (j = 0; j < 4; j++) {
        lbl_1_bss_A50[j] = 1.0f;
    }
    return cam;
}

// NONMATCHING: the singleton.h assert block is out-of-line in the original
// (beq into it, then a b to skip); mwcceppc 1.3.2 always branches over it.
Mgr511Camera *fn_1_9A0C(Mgr511Camera *cam, s16 arg1)
{
    if (cam != 0) {
        if (lbl_1_bss_A3C == 0) {
            fn_1_CF8(lbl_1_data_390, lbl_1_data_384, 0x3D);
        }
        lbl_1_bss_A3C = 0;
        if (arg1 > 0) {
            __dl__FPv(cam);
        }
    }
    return cam;
}

void fn_1_9A98(Mgr511Camera *cam, s32 idx, f32 x, f32 y, f32 w, f32 h)
{
    Mgr511CamElem *e;

    e = &cam->elem[idx];
    if (x < 0.0f) {
        x = 0.0f;
    }
    if (y < 0.0f) {
        y = 0.0f;
    }
    if (x > 640.0f) {
        x = 640.0f;
    }
    if (y > 480.0f) {
        y = 480.0f;
    }
    if (w < 0.0f) {
        w = 0.0f;
    }
    if (h < 0.0f) {
        h = 0.0f;
    }
    if (w > 640.0f) {
        w = 640.0f;
    }
    if (h > 480.0f) {
        h = 480.0f;
    }
    e->x = x;
    e->y = y;
    e->w = w;
    e->h = h;
    if (w != 0.0f && h != 0.0f) {
        if (e->active == 0) {
            Hu3DCameraCreate(lbl_1_rodata_248[idx]);
            e->active = 1;
        }
    } else if (e->active == 1) {
        Hu3DCameraKill(lbl_1_rodata_248[idx]);
        e->active = 0;
    }
    if (e->active != 0) {
        Hu3DCameraPerspectiveSet(lbl_1_rodata_248[idx], 45.0f, 100.0f, 35000.0f, 1.2f);
        Hu3DCameraViewportSet(lbl_1_rodata_248[idx], x, y, w, h, 0.0f, 1.0f);
        Hu3DCameraScissorSet(lbl_1_rodata_248[idx], (u32)x, (u32)y, (u32)w, (u32)h);
    }
}

static inline void CamSplit(Mgr511Camera *cam, f32 w, f32 h)
{
    f32 x0 = 0.0f;
    f32 y0 = x0;
    f32 wm = w - 1.0f;
    f32 hm = h - 1.0f;

    cam->splitX = w;
    cam->splitY = h;
    fn_1_9A98(cam, 0, y0, x0, wm, hm);
    fn_1_9A98(cam, 1, 1.0f + w, 0.0f, 639.0f - w, hm);
    fn_1_9A98(cam, 2, 0.0f, 1.0f + h, wm, 479.0f - h);
    fn_1_9A98(cam, 3, 1.0f + w, 1.0f + h, 639.0f - w, 479.0f - h);
}

static inline void CamSplitRaw(Mgr511Camera *cam, f32 w, f32 h)
{
    cam->splitX = w;
    cam->splitY = h;
    fn_1_9A98(cam, 0, 0.0f, 0.0f, w - 1.0f, h - 1.0f);
    fn_1_9A98(cam, 1, 1.0f + w, 0.0f, 639.0f - w, h - 1.0f);
    fn_1_9A98(cam, 2, 0.0f, 1.0f + h, w - 1.0f, 479.0f - h);
    fn_1_9A98(cam, 3, 1.0f + w, 1.0f + h, 639.0f - w, 479.0f - h);
}

void fn_1_9DA8(Mgr511Camera *cam, f32 w, f32 h)
{
    CamSplit(cam, w, h);
}

/* The original inlines fn_1_9DA8 with its arguments substituted textually. */
#define CAM_SPLIT(c, w, h)     do {         (c)->splitX = (w);         (c)->splitY = (h);         fn_1_9A98((c), 0, 0.0f, 0.0f, (w) - 1.0f, (h) - 1.0f);         fn_1_9A98((c), 1, 1.0f + (w), 0.0f, 639.0f - (w), (h) - 1.0f);         fn_1_9A98((c), 2, 0.0f, 1.0f + (h), (w) - 1.0f, 479.0f - (h));         fn_1_9A98((c), 3, 1.0f + (w), 1.0f + (h), 639.0f - (w), 479.0f - (h));     } while (0)

void fn_1_9F98(Mgr511Camera *cam, s32 idx, Vec *arg2, Vec *arg3)
{
    Mgr511CamElem *e = &cam->elem[idx];

    PSVECSubtract(arg2, &e->pose.target, arg2);
    PSVECScale(arg2, arg2, lbl_1_rodata_2D4[0]);
    PSVECAdd(arg2, &e->pose.target, arg2);
    PSVECSubtract(arg3, &e->pose.pos, arg3);
    PSVECScale(arg3, arg3, lbl_1_rodata_2D8[0]);
    PSVECAdd(arg3, &e->pose.pos, arg3);
}

void fn_1_A060(Mgr511Camera *cam, s32 idx, Vec *arg2, Vec *arg3)
{
    Vec a;
    Vec b;

    fn_1_112E0(idx, &a, &b);
    fn_1_9F98(cam, idx, &a, &b);
    arg2->x = a.x;
    arg2->y = a.y;
    arg2->z = a.z;
    arg3->x = b.x;
    arg3->y = b.y;
    arg3->z = b.z;
}

void fn_1_A16C(Mgr511Camera *cam, s32 idx, Vec *arg2, Vec *arg3)
{
    Vec a;
    Vec b;

    fn_1_1167C(idx, &a, &b);
    fn_1_9F98(cam, idx, &a, &b);
    arg2->x = a.x;
    arg2->y = a.y;
    arg2->z = a.z;
    arg3->x = b.x;
    arg3->y = b.y;
    arg3->z = b.z;
}

static inline void CamFollow112E0(Mgr511Camera *cam)
{
    Vec b;
    Vec a;
    s32 i;
    Mgr511CamElem *e;

    for (i = 0; i < 4; i++) {
        e = &cam->elem[i];
        fn_1_112E0(i, &a, &b);
        fn_1_9F98(cam, i, &a, &b);
        e->pose.target.x = a.x;
        e->pose.target.y = a.y;
        e->pose.target.z = a.z;
        e->pose.pos.x = b.x;
        e->pose.pos.y = b.y;
        e->pose.pos.z = b.z;
        fn_1_14C4(&e->pose, lbl_1_rodata_248[i]);
    }
}

void fn_1_A278(Mgr511Camera *cam)
{
    CamFollow112E0(cam);
}

void fn_1_A3B4(Mgr511Camera *cam)
{
    CamFollow112E0(cam);
}

void fn_1_A4F0(Mgr511Camera *cam)
{
    Mgr511Ptmf tmp;

    fn_1_172C(&cam->freeCam, 0);
    if (HuPadBtnDown[0] & 0x800) {
        tmp = lbl_1_data_3A0;
        cam->ptmf = tmp;
    }
    fn_1_14C4(&cam->freeCam, 1);
}

void fn_1_A580(Mgr511Camera *cam)
{
    f32 t;

    if ((s8)lbl_1_bss_A64 == 0) {
        lbl_1_bss_A60 = 60.0f;
        lbl_1_bss_A64 = 1;
    }
    t = (f32)cam->counter;
    cam->counter = cam->counter + 1;
    if (t > lbl_1_bss_A60) {
        t = lbl_1_bss_A60;
    }
    CAM_SPLIT(cam, 640.0f - ((320.0f * t) / lbl_1_bss_A60), 480.0f - ((240.0f * t) / lbl_1_bss_A60));
    CamFollow112E0(cam);
}

void fn_1_ABA8(Mgr511Camera *cam)
{
    Vec target;
    Vec up;
    Vec pos;
    s32 i;
    Mgr511CamElem *e;

    if (Hu3DMotionEndCheck(cam->motionModel) != 0) {
        cam->counter = 0;
        cam->ptmf = lbl_1_data_3AC;
        cam->flags &= 0xFFFFFFFE;
        for (i = 0; i < 4; i++) {
            e = &cam->elem[i];
            Hu3DCameraPosGet(lbl_1_rodata_248[0], &pos, &up, &target);
            e->pose.pos.x = pos.x;
            e->pose.pos.y = pos.y;
            e->pose.pos.z = pos.z;
            e->pose.up.x = up.x;
            e->pose.up.y = up.y;
            e->pose.up.z = up.z;
            e->pose.target.x = target.x;
            e->pose.target.y = target.y;
            e->pose.target.z = target.z;
        }
    }
    cam->counter = cam->counter + 1;
    Hu3DCameraPerspectiveSet(1, 45.0f, 100.0f, 35000.0f, 1.2f);
}

void fn_1_AD04(Mgr511Camera *cam)
{
    Vec b;
    Vec a;
    s32 i;
    s32 j;
    f32 t;
    f32 frac;
    f32 cx;
    f32 cy;
    Mgr511CamElem *e;

    for (i = 0; i < 4; i++) {
        e = &cam->elem[i];
        fn_1_1167C(i, &a, &b);
        fn_1_9F98(cam, i, &a, &b);
        e->pose.target.x = a.x;
        e->pose.target.y = a.y;
        e->pose.target.z = a.z;
        e->pose.pos.x = b.x;
        e->pose.pos.y = b.y;
        e->pose.pos.z = b.z;
        fn_1_14C4(&e->pose, lbl_1_rodata_248[i]);
    }
    if (cam->split >= 0) {
        if ((s8)lbl_1_bss_A6C == 0) {
            lbl_1_bss_A68 = 120.0f;
            lbl_1_bss_A6C = 1;
        }
        t = (f32)cam->counter;
        cam->counter = cam->counter + 1;
        if (t > lbl_1_bss_A68) {
            t = lbl_1_bss_A68;
        }
        frac = t / lbl_1_bss_A68;
        cx = 320.0f;
        cy = 240.0f;
        switch (cam->split) {
        case 0:
            cx = (f32)(320.0 + (320.0 * (f64)frac));
            cy = (f32)(240.0 + (240.0 * (f64)frac));
            break;
        case 1:
            cx = (f32)(320.0 - (320.0 * (f64)frac));
            cy = (f32)(240.0 + (240.0 * (f64)frac));
            break;
        case 2:
            cx = (f32)(320.0 + (320.0 * (f64)frac));
            cy = (f32)(240.0 - (240.0 * (f64)frac));
            break;
        case 3:
            cx = (f32)(320.0 - (320.0 * (f64)frac));
            cy = (f32)(240.0 - (240.0 * (f64)frac));
            break;
        }
        CamSplitRaw(cam, cx, cy);
        for (j = 0; j < 4; j++) {
            lbl_1_bss_A50[j] = (f32)(1.0 - (f64)frac);
            if (lbl_1_bss_A50[j] < 0.0f) {
                lbl_1_bss_A50[j] = 0.0f;
            }
        }
        lbl_1_bss_A50[cam->split] = 1.0f;
    }
}

void fn_1_B2D4(Mgr511Camera *cam, s32 arg1)
{
    Mgr511Ptmf tmp;

    cam->split = arg1;
    tmp = lbl_1_data_3B8;
    cam->ptmf = tmp;
    cam->counter = 0;
}

void fn_1_B324(Mgr511Camera *cam, s32 mode)
{
    Mgr511Ptmf tmp;

    lbl_1_bss_A70 = mode;
    switch (mode) {
    case 0:
        cam->counter = 0;
        tmp = lbl_1_data_3C4;
        cam->ptmf = tmp;
        Hu3DCameraCreate(1);
        cam->motionModel = Hu3DModelCameraCreate(Hu3DMotionCreateData(DATANUM(DATA_m511, 0x1F)), 1);
        Hu3DCameraMotionStart(cam->motionModel, 1);
        return;
    case 1:
        Hu3DModelKill(cam->motionModel);
        cam->flags &= ~(1 << mode);
        return;
    case 2:
        tmp = lbl_1_data_3D0;
        cam->ptmf = tmp;
        cam->flags &= ~(1 << mode);
        return;
    case 3:
        cam->flags &= ~(1 << mode);
        return;
    case 4:
        cam->flags &= ~(1 << mode);
        return;
    }
}

// NONMATCHING: the original passes the PTMF in r12 (CodeWarrior builtin
// __ptmf_scall ABI) and pads a nop after the call; mwcceppc 1.3.2 uses r4.
void fn_1_B4C0(Mgr511Camera *cam)
{
    if (__ptmf_test(&cam->ptmf)) {
        __ptmf_scall(cam, &cam->ptmf);
    }
}

s16 fn_1_B508(HuVecF *pos, s32 camIdx)
{
    HuVecF out;
    f32 pan;
    s16 result;
    s32 idx;
    Mgr511Camera *cam;

    if (lbl_1_bss_A3C == 0) {
        fn_1_CF8(lbl_1_data_390, lbl_1_data_384, 0x27);
    }
    cam = lbl_1_bss_A3C;
    idx = camIdx;
    if (lbl_1_bss_A70 == 1) {
        idx = 0;
    }
    if (lbl_1_bss_A70 == 3 && cam->split >= 0) {
        idx = cam->split;
    }
    Hu3D3Dto2D(pos, (s16)lbl_1_rodata_248[idx], &out);
    pan = (cam->elem[idx].x + ((out.x / 576.0f) * cam->elem[idx].w)) * 0.11111111f;
    if ((s32)(32.0f + pan) < 0) {
        result = 0;
    } else if ((s32)(32.0f + pan) > 0x7F) {
        result = 0x7F;
    } else {
        result = (s16)(32.0f + pan);
    }
    return result;
}

s32 fn_1_B6C0(HuVecF *pos, s32 seId, s32 camIdx)
{
    HuVecF out;
    s16 pan;
    s16 result;
    s32 idx;
    s32 id;
    Mgr511Camera *cam;

    id = HuAudFXPlay(seId);
    if (lbl_1_bss_A3C == 0) {
        fn_1_CF8(lbl_1_data_390, lbl_1_data_384, 0x27);
    }
    cam = lbl_1_bss_A3C;
    idx = camIdx;
    if (lbl_1_bss_A70 == 1) {
        idx = 0;
    }
    if (lbl_1_bss_A70 == 3 && cam->split >= 0) {
        idx = cam->split;
    }
    Hu3D3Dto2D(pos, (s16)lbl_1_rodata_248[idx], &out);
    pan = (s16)(32.0f + ((cam->elem[idx].x + ((out.x / 576.0f) * cam->elem[idx].w)) * 0.11111111f));
    if (pan < 0) {
        result = 0;
    } else if (pan > 0x7F) {
        result = 0x7F;
    } else {
        result = pan;
    }
    HuAudFXPanning(id, result);
    return id;
}

s32 fn_1_B864(HuVecF *pos, s32 seId, s32 camIdx)
{
    HuVecF out;
    s16 pan;
    s16 result;
    s32 idx;
    s32 id;
    Mgr511Camera *cam;

    id = -1;
    if (fn_1_29C8() == 2) {
        id = HuAudFXPlay(seId);
        if (lbl_1_bss_A3C == 0) {
            fn_1_CF8(lbl_1_data_390, lbl_1_data_384, 0x27);
        }
        cam = lbl_1_bss_A3C;
        idx = camIdx;
        if (lbl_1_bss_A70 == 1) {
            idx = 0;
        }
        if (lbl_1_bss_A70 == 3 && cam->split >= 0) {
            idx = cam->split;
        }
        Hu3D3Dto2D(pos, (s16)lbl_1_rodata_248[idx], &out);
        pan = (s16)(32.0f + ((cam->elem[idx].x + ((out.x / 576.0f) * cam->elem[idx].w)) * 0.11111111f));
        if (pan < 0) {
            result = 0;
        } else if (pan > 0x7F) {
            result = 0x7F;
        } else {
            result = pan;
        }
        HuAudFXPanning(id, result);
    }
    return id;
}

void fn_1_BA18(HuVecF *pos, s32 seId, s32 camIdx, f32 pitchT)
{
    HuVecF out;
    s16 pitch;
    s16 pan;
    s16 result;
    s32 idx;
    Mgr511Camera *cam;

    pitch = (s16)(8191.0 * (2.0 * ((f64)pitchT - 0.5)));
    if (pitch > 0x1FFF) {
        pitch = 0x1FFF;
    }
    if (pitch < -0x2000) {
        pitch = -0x2000;
    }
    if (lbl_1_bss_A3C == 0) {
        fn_1_CF8(lbl_1_data_390, lbl_1_data_384, 0x27);
    }
    cam = lbl_1_bss_A3C;
    idx = camIdx;
    if (lbl_1_bss_A70 == 1) {
        idx = 0;
    }
    if (lbl_1_bss_A70 == 3 && cam->split >= 0) {
        idx = cam->split;
    }
    Hu3D3Dto2D(pos, (s16)lbl_1_rodata_248[idx], &out);
    pan = (s16)(32.0f + ((cam->elem[idx].x + ((out.x / 576.0f) * cam->elem[idx].w)) * 0.11111111f));
    if (pan < 0) {
        result = 0;
    } else if (pan > 0x7F) {
        result = 0x7F;
    } else {
        result = pan;
    }
    HuAudFXPanning(seId, result);
    HuAudFXPitchSet(seId, pitch);
    HuAudFXVolSet(seId, (s32)(112.0f * lbl_1_bss_A50[camIdx]));
}

void *fn_1_BC4C(Mgr511Camera *cam, s16 arg1)
{
    if (cam != 0) {
        cam->vtable = lbl_1_data_3DC;
        if ((void *)((u8 *)cam + 0x10) != 0) {
            if (lbl_1_bss_A3C == 0) {
                fn_1_CF8(lbl_1_data_390, lbl_1_data_384, 0x3D);
            }
            lbl_1_bss_A3C = 0;
        }
        if (cam != 0) {
            cam->vtable = lbl_1_data_40;
            if (cam != 0) {
                fn_1_1D94(cam, 0);
            }
        }
        if (arg1 > 0) {
            __dl__FPv(cam);
        }
    }
    return cam;
}
