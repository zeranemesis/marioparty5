#include "REL/m509Dll/m509.h"
#include "game/pad.h"
#include <math.h>

extern "C" {
s32 __ptmf_test(void *ptmf);
void __ptmf_scall(void *self, void *ptmf);
void fn_1_5324(Vec *pos, s32 arg);
void fn_1_F924(void);
void Hu3D3Dto2D(Vec *src, s16 cameraBit, Vec *dst);
void fn_1_FFFC(Vec *pos);
extern void *lbl_1_bss_48;
}


extern "C" {
void *fn_1_DEEC(Camera *self);
void *fn_1_E114(Camera *self, s16 flag);
s16 fn_1_E1A0(Vec *pos);
s16 fn_1_E4B4(Vec *pos);
void fn_1_E560(Camera *self);
void fn_1_E6E0(Camera *self);
void fn_1_E724(Camera *self);
void fn_1_E86C(Camera *self);
void fn_1_E928(Camera *self, s32 bit);
void fn_1_EA00(Camera *self);
void *fn_1_EA48(Camera *self, s16 flag);
}

M509Ptmf lbl_1_data_33D0 = { 0, -1, (void *)fn_1_E724 };
char lbl_1_data_33DC[] = "ms_pointer == 0";
char lbl_1_data_33EC[] = "singleton.h";
char lbl_1_data_33F8[] = "ms_pointer != 0";
M509Ptmf lbl_1_data_3408 = { 0, -1, (void *)fn_1_E560 };
M509Ptmf lbl_1_data_3414 = { 0, -1, 0 };
void *lbl_1_data_3420[6] = { 0, 0, (void *)fn_1_EA48, (void *)fn_1_E928, (void *)fn_1_EA00, 0 };

XX_RODATA const Vec lbl_1_rodata_318 = { 0.0f, 4500.0f, 0.0f };
XX_RODATA const Vec lbl_1_rodata_324 = { 0.0f, 0.0f, -1.0f };
XX_RODATA const Vec lbl_1_rodata_330 = { 0.0f, 0.0f, 0.0f };
XX_RODATA const Vec lbl_1_rodata_39C = { 0.0f, 900.0f, 500.0f };
XX_RODATA const Vec lbl_1_rodata_3A8 = { 0.0f, 0.0f, -200.0f };
XX_RODATA const Vec lbl_1_rodata_3B8 = { 0.0f, 0.0f, -1.0f };

static inline void SetPosV(Vec *pos, Vec *up, Vec *target)
{
    Hu3DCameraPosSetV(1, pos, up, target);
}

void *fn_1_DEEC(Camera *self)
{
    Vec up;
    Vec target;
    u8 *q;

    fn_1_E48(self);
    self->base.vtbl = (void **)lbl_1_data_1C4;
    self->base.flags = 0;
    fn_1_F14(self, lbl_1_bss_1C);
    lbl_1_bss_48 == 0 ? (void)0 : fn_1_0(lbl_1_data_33DC, lbl_1_data_33EC, 0x35);
    q = (u8 *)self + 0x10;
    if (q != 0) {
        q -= 0x10;
    }
    lbl_1_bss_48 = q;
    self->base.vtbl = lbl_1_data_3420;
    self->update = lbl_1_data_33D0;
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 45.0f, 100.0f, 10000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    up = lbl_1_rodata_324;
    target = lbl_1_rodata_330;
    self->pos.x = 0.0f;
    self->pos.y = lbl_1_rodata_318.y;
    self->pos.z = lbl_1_rodata_318.z;
    self->up = up;
    self->target = target;
    self->frame = 0;
    return self;
}

void *fn_1_E114(Camera *self, s16 flag)
{
    if (self != 0) {
        lbl_1_bss_48 != 0 ? (void)0 : fn_1_0(lbl_1_data_33F8, lbl_1_data_33EC, 0x3D);
        lbl_1_bss_48 = 0;
        if (flag > 0) {
            fn_1_21C(self);
        }
    }
    return self;
}

s16 fn_1_E1A0(Vec *pos)
{
    static f64 farDist = 45.0;
    static f64 nearDist = 10.0;
    Vec camPos;
    Vec camUp;
    Vec camTarget;
    Vec diff;
    f32 sq;
    f64 dist;
    s16 v;
    s16 r;

    Hu3DCameraPosGet(1, &camPos, &camUp, &camTarget);
    PSVECSubtract(pos, &camPos, &diff);
    sq = diff.z * diff.z + (diff.x * diff.x + diff.y * diff.y);
    dist = sqrt(sq);
    v = (s16)(127.0f - 32.0f * (f32)((dist / 100.0 - nearDist) / (farDist - nearDist)));
    if (v < 0x68) {
        r = 0x68;
    } else if (v > 0x7F) {
        r = 0x7F;
    } else {
        r = v;
    }
    return r;
}

s16 fn_1_E4B4(Vec *pos)
{
    Vec scr;
    s16 v;
    s16 r;

    Hu3D3Dto2D(pos, 1, &scr);
    scr.x = scr.x * 0.11111111f;
    v = (s16)(32.0f + scr.x);
    if (v < 0) {
        r = 0;
    } else if (v > 0x7F) {
        r = 0x7F;
    } else {
        r = v;
    }
    return r;
}

void fn_1_E560(Camera *self)
{
    Vec *pos = &self->pos;
    Vec *target = &self->target;

    pos->x = pos->x + 0.05f * (lbl_1_rodata_39C.x - pos->x);
    pos->y = pos->y + 0.05f * (lbl_1_rodata_39C.y - pos->y);
    pos->z = pos->z + 0.05f * (lbl_1_rodata_39C.z - pos->z);
    target->x = target->x + 0.05f * (lbl_1_rodata_3A8.x - target->x);
    target->y = target->y + 0.05f * (lbl_1_rodata_3A8.y - target->y);
    target->z = target->z + 0.05f * (lbl_1_rodata_3A8.z - target->z);
    SetPosV(pos, &self->up, target);
}

void fn_1_E6E0(Camera *self)
{
    M509Ptmf p = lbl_1_data_3408;

    self->update = p;
}

void fn_1_E724(Camera *self)
{
    f32 t;

    self->up.x = 0.0f;
    self->up.y = lbl_1_rodata_3B8.y;
    self->up.z = lbl_1_rodata_3B8.z;
    fn_1_FFFC(&self->pos);
    if (self->frame < 60) {
        t = self->pos.y * 0.125f;
        self->pos.y = t + 7.0f * t * ((f32)self->frame / 60.0f);
    }
    self->target.x = self->pos.x;
    self->target.y = 0.0f;
    self->target.z = self->pos.z;
    SetPosV(&self->pos, &self->up, &self->target);
    self->frame = self->frame + 1;
}

void fn_1_E86C(Camera *self)
{
    M509Ptmf p;

    fn_1_5324(&self->pos, 0);
    if ((HuPadBtnDown[0] & 0x800) != 0) {
        p = lbl_1_data_3414;
        self->update = p;
    }
    SetPosV(&self->pos, &self->up, &self->target);
}

void fn_1_E928(Camera *self, s32 bit)
{
    switch (bit) {
    case 0:
        self->base.flags &= ~(1 << bit);
        break;
    case 1:
        self->base.flags &= ~(1 << bit);
        break;
    case 2:
        self->base.flags &= ~(1 << bit);
        break;
    case 3:
        fn_1_F924();
        self->base.flags &= ~(1 << bit);
        break;
    case 4:
        self->base.flags &= ~(1 << bit);
        break;
    }
}

void fn_1_EA00(Camera *self)
{
#ifdef TARGET_PC
    if (self->update.func != 0) {
        ((void (*)(Camera *))self->update.func)(self);
    }
#else
    if (__ptmf_test(&self->update)) {
        __ptmf_scall(self, &self->update);
    }
#endif
}

void *fn_1_EA48(Camera *self, s16 flag)
{
    if (self != 0) {
        self->base.vtbl = lbl_1_data_3420;
        if ((u8 *)self + 0x10 != 0) {
            lbl_1_bss_48 != 0 ? (void)0 : fn_1_0(lbl_1_data_33F8, lbl_1_data_33EC, 0x3D);
            lbl_1_bss_48 = 0;
        }
        if (self != 0) {
            self->base.vtbl = (void **)lbl_1_data_1C4;
            if (self != 0) {
                fn_1_E54(self, 0);
            }
        }
        if (flag > 0) {
            fn_1_21C(self);
        }
    }
    return self;
}
