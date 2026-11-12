#include "dolphin.h"
#include "game/data.h"
#include "game/hu3d.h"
#include "game/pad.h"
#include <math.h>
#include "REL/m579Dll/m579objs.h"

extern "C" {
s32 __ptmf_test(void *ptmf);
void __ptmf_scall(void *self, void *ptmf);
int HuAudFXPlayVolPan(int seId, s16 vol, s16 pan);
void Hu3D3Dto2D(Vec *src, s16 cameraBit, Vec *dst);
void fn_1_1264(void *self);
void fn_1_1270(void *self, s32 arg);
void fn_1_1330(void *self, void *arg);
void fn_1_1D4(const char *expr, const char *file, s32 line);
void fn_1_C8(void *self);
void fn_1_654(M579Cam *cam, const M579Pose *pose);
void fn_1_6F4(M579Cam *cam, M579Pose *pose);
void fn_1_76C(M579Cam *cam, M579Pose *a, M579Pose *b, f32 t);
void fn_1_9A0(M579Cam *cam, s32 arg);
void fn_1_9E0(M579Cam *cam, s32 arg);
void fn_1_C08(M579Cam *cam, s32 arg);

extern M579Camera *lbl_1_bss_A08;
extern u8 lbl_1_bss_C[];
extern u8 lbl_1_data_1FC[];

void *fn_1_8B0C(M579Camera *self);
M579Camera *fn_1_8C44(M579Camera *self, s16 flag);
M579Camera *fn_1_8CD0(M579Camera *self, s16 flag);
M579Camera *fn_1_8D28(M579Camera *self, s16 flag);
void fn_1_8DF4(M579Camera *self);
void fn_1_8E88(M579Camera *self);
void fn_1_8EB8(M579Camera *self);
void fn_1_8FCC(M579Camera *self, M579Actor *target);
void fn_1_901C(M579Camera *self, s32 bit);
void fn_1_9124(M579Camera *self);
void fn_1_916C(M579Camera *self);
void fn_1_91D4(M579Camera *self);
void fn_1_920C(M579Camera *self);
s16 fn_1_9344(Vec *pos);
s16 fn_1_93E4(Vec *pos);
void fn_1_9668(Vec *pos, s32 seId);
}

M579Ptmf lbl_1_data_2B0 = { 0, -1, (void *) fn_1_8E88 };
char lbl_1_data_2BC[16] = "ms_pointer == 0";
char lbl_1_data_2CC[12] = "singleton.h";
char lbl_1_data_2D8[16] = "ms_pointer != 0";
M579Ptmf lbl_1_data_2E8 = { 0, -1, (void *) fn_1_8E88 };
M579Ptmf lbl_1_data_2F4 = { 0, -1, (void *) fn_1_8EB8 };
M579Ptmf lbl_1_data_300 = { 0, -1, (void *) fn_1_8DF4 };
void *lbl_1_data_30C[5] = { 0, 0, (void *) fn_1_8D28, (void *) fn_1_901C, (void *) fn_1_9124 };

M579_RODATA const M579Pose lbl_1_rodata_1C8 = { { -4.401607036590576f, 1200.50634765625f, 1453.7222900390625f }, { -0.38001033663749695f, 3.3136682286993846e-09f, 8.065794254719094e-09f, 0.9249823093414307f } };
M579_RODATA const Vec lbl_1_rodata_1E4 = { 0.0f, 700.0f, 500.0f };
M579_RODATA const Vec lbl_1_rodata_1F0 = { 0.0f, 200.0f, 0.0f };

void *fn_1_8B0C(M579Camera *self)
{
    fn_1_1264(self);
    self->base.vtbl = (void **) lbl_1_data_1FC;
    self->base.flags = 0;
    fn_1_1330(self, lbl_1_bss_C);
    lbl_1_bss_A08 == 0 ? (void) 0 : fn_1_1D4(lbl_1_data_2BC, lbl_1_data_2CC, 0x35);
    lbl_1_bss_A08 = self;
    self->base.vtbl = lbl_1_data_30C;
    self->update = lbl_1_data_2B0;
    self->model = -1;
    self->target = 0;
    self->debugOn = 0;
    self->debugHold = 0;
    Hu3DCameraCreate(1);
    Hu3DBGColorSet(0, 0, 0);
    fn_1_654(&self->debugCam, &lbl_1_rodata_1C8);
    return self;
}

M579Camera *fn_1_8C44(M579Camera *self, s16 flag)
{
    if (self != 0) {
        lbl_1_bss_A08 != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_2D8, lbl_1_data_2CC, 0x3D);
        lbl_1_bss_A08 = 0;
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M579Camera *fn_1_8CD0(M579Camera *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M579Camera *fn_1_8D28(M579Camera *self, s16 flag)
{
    if (self != 0) {
        self->base.vtbl = lbl_1_data_30C;
        if (self != 0) {
            lbl_1_bss_A08 != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_2D8, lbl_1_data_2CC, 0x3D);
            lbl_1_bss_A08 = 0;
        }
        if (self != 0) {
            self->base.vtbl = (void **) lbl_1_data_1FC;
            if (self != 0) {
                fn_1_1270(self, 0);
            }
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_8DF4(M579Camera *self)
{
    M579Ptmf p;

    if (self->model != -1) {
        fn_1_9E0(&self->cam, 1);
        if (Hu3DMotionEndCheck(self->model) != 0) {
            p = lbl_1_data_2E8;
            self->update = p;
            self->base.flags = self->base.flags & 0xFFFFFFFE;
        }
    }
}

void fn_1_8E88(M579Camera *self)
{
    fn_1_9A0(&self->cam, 1);
}

void fn_1_8EB8(M579Camera *self)
{
    M579Cam c;
    M579Pose poseA;
    M579Pose poseB;
    Vec *tpos;

    self->frame = self->frame + 1;
    if (!((f32) self->frame < 90.0f)) {
        tpos = &self->target->pos;
        PSVECAdd(tpos, &lbl_1_rodata_1E4, &c.pos);
        PSVECAdd(tpos, &lbl_1_rodata_1F0, &c.target);
        c.up.x = 0.0f;
        c.up.y = 1.0f;
        c.up.z = 0.0f;
        fn_1_6F4(&self->cam, &poseA);
        fn_1_6F4(&c, &poseB);
        fn_1_76C(&self->cam, &poseA, &poseB, 0.05f);
        fn_1_9A0(&self->cam, 1);
    }
}

void fn_1_8FCC(M579Camera *self, M579Actor *target)
{
    M579Ptmf p;

    self->frame = 0;
    self->target = target;
    p = lbl_1_data_2F4;
    self->update = p;
}

void fn_1_901C(M579Camera *self, s32 bit)
{
    M579Ptmf p;

    switch (bit) {
    case 0:
        fn_1_916C(self);
        p = lbl_1_data_300;
        self->update = p;
        return;
    case 1:
        fn_1_91D4(self);
        self->base.flags = self->base.flags & ~(1 << bit);
        return;
    case 2:
        self->base.flags = self->base.flags & ~(1 << bit);
        return;
    case 3:
        self->base.flags = self->base.flags & ~(1 << bit);
        return;
    case 4:
        self->base.flags = self->base.flags & ~(1 << bit);
        return;
    }
}

void fn_1_9124(M579Camera *self)
{
#ifdef TARGET_PC
    if (self->update.func != 0) {
        ((void (*)(M579Camera *)) self->update.func)(self);
    }
#else
    if (__ptmf_test(&self->update) != 0) {
        __ptmf_scall(self, &self->update);
    }
#endif
}

void fn_1_916C(M579Camera *self)
{
    s32 mot;

    mot = Hu3DMotionCreate(HuDataSelHeapReadNum(0x850002, 0x10000000, HEAP_MODEL));
    self->model = Hu3DModelCameraCreate(mot, 1);
    Hu3DCameraMotionStart(self->model, 1);
}

void fn_1_91D4(M579Camera *self)
{
    Hu3DModelKill(self->model);
    self->model = -1;
}

void fn_1_920C(M579Camera *self)
{
    Vec *a;
    Vec *b;
    Vec *c;
    u8 d;
    u16 btn;

    btn = HuPadBtnDown[0];
    if ((btn & 0x800) != 0) {
        self->debugOn = (self->debugOn == 0);
    }
    if (self->debugOn != 0) {
        if ((btn & 0x40) != 0) {
            self->debugHold = (self->debugHold == 0);
        }
        if (self->debugHold == 0) {
            fn_1_C08(&self->debugCam, 0);
        }
        if ((btn & 0x20) != 0) {
            self->debugCam = self->cam;
        }
        fn_1_9A0(&self->debugCam, 1);
        a = &self->debugCam.pos;
        b = &self->debugCam.target;
        c = &self->debugCam.up;
        if (self->debugHold != 0) {
            d = 0xFF;
        } else {
            d = 0;
        }
    }
}

s16 fn_1_9344(Vec *pos)
{
    Vec scr;
    s16 v;
    s16 r;

    Hu3D3Dto2D(pos, 1, &scr);
    scr.x = scr.x * 0.055555556f;
    v = (s16) (48.0f + scr.x);
    if (v < 0) {
        r = 0;
    } else if (v > 0x7F) {
        r = 0x7F;
    } else {
        r = v;
    }
    return r;
}

s16 fn_1_93E4(Vec *pos)
{
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
    v = (s16) (127.0f - 32.0f * (f32) ((dist / 100.0 - 14.0) / 11.0));
    if (v < 0x60) {
        r = 0x60;
    } else if (v > 0x7F) {
        r = 0x7F;
    } else {
        r = v;
    }
    return r;
}

void fn_1_9668(Vec *pos, s32 seId)
{
    Vec scr;
    f32 vol;
    s16 v;
    s16 r;

    vol = 9.0f + pos->z / 100.0f;
    if (vol < 0.0f) {
        vol = 0.0f;
    }
    if (vol > 18.0f) {
        vol = 18.0f;
    }
    Hu3D3Dto2D(pos, 1, &scr);
    scr.x = scr.x * 0.055555556f;
    v = (s16) (48.0f + scr.x);
    if (v < 0) {
        r = 0;
    } else if (v > 0x7F) {
        r = 0x7F;
    } else {
        r = v;
    }
    HuAudFXPlayVolPan(seId, (s16) (96.0 + (31.0 * (f64) vol) / 18.0), r);
}
