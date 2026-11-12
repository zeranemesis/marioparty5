#define _MATH_H
#include "REL/m522Dll.h"

#include "game/data.h"
#include "game/hu3d.h"
#include "game/pad.h"

#ifdef TARGET_PC
#include <math.h>
#else
extern "C" double cos(double x);
#endif

#ifdef __MWERKS__
#define CAMERA_RODATA __declspec(section ".rodata")
#else
#define CAMERA_RODATA
#endif

/* Camera key: position and orientation quaternion. */
typedef struct M522CamPath_s {
    f32 pos[3];
    f32 quat[4];
} M522_CAMPATH;

/* Look-at description handed to the camera converters. */
typedef struct M522CamView_s {
    M522_VEC3 pos;
    M522_VEC3 up;
    M522_VEC3 target;
} M522_CAMVIEW;

typedef struct M522Camera_s {
    /* 0x00 */ M522_NODE node;
    /* 0x08 */ M522_VTFN *vtbl;
    /* 0x0C */ u32 flags;
    /* 0x10 */ M522_CAMVIEW view;
    /* 0x34 */ M522_PTMF hook;
    /* 0x40 */ s16 camModel;
    /* 0x44 */ s32 duration;
    /* 0x48 */ s32 timer;
    /* 0x4C */ M522_CAMPATH pathA;
    /* 0x68 */ M522_CAMPATH pathB;
    /* 0x84 */ u8 debug;
    /* 0x85 */ u8 debugFreeze;
    /* 0x88 */ M522_CAMVIEW debugView;
} M522_CAMERA;

extern "C" {
extern M522_CAMERA *lbl_1_bss_A1C;
extern void *lbl_1_bss_18;
extern M522_NODE lbl_1_bss_2C;
extern M522_VTFN lbl_1_data_3A8[];

void fn_1_6964(void *ptr);
void fn_1_6A70(const char *cond, const char *file, s32 line);
void fn_1_6EF0(M522_CAMVIEW *view, const M522_CAMPATH *path);
void fn_1_6F90(const M522_CAMVIEW *view, M522_CAMPATH *path);
void fn_1_7008(M522_CAMVIEW *view, M522_CAMPATH *from, M522_CAMPATH *to, f32 t);
void fn_1_723C(M522_CAMVIEW *view, s32 flag);
void fn_1_727C(M522_CAMVIEW *view, s32 flag);
void fn_1_74A4(M522_CAMVIEW *view, s32 flag);
void fn_1_8004(M522_NODE *node);
void fn_1_8010(M522_NODE *node, s32 flag);
void fn_1_80D0(M522_NODE *node, M522_NODE *other);
f32 fn_1_F1B8(void *self);
s32 fn_1_FA28(void);
s32 __ptmf_test(M522_PTMF *ptmf);
void __ptmf_scall(void *self);

M522_CAMERA *fn_1_D1B4(M522_CAMERA *self);
void *fn_1_D2F4(M522_CAMERA *self, s16 flag);
void *fn_1_D380(M522_NODE *node, s16 flag);
void *fn_1_D3D8(M522_CAMERA *self, s16 flag);
void fn_1_D4A4(M522_CAMERA *self, s32 mode);
void fn_1_D60C(M522_CAMERA *self);
void fn_1_D654(M522_CAMERA *self);
void fn_1_D6E8(M522_CAMERA *self);
void fn_1_D718(M522_CAMERA *self, f32 offset);
void fn_1_D798(M522_CAMERA *self);
void fn_1_D7F4(M522_CAMERA *self);
void fn_1_D980(M522_CAMERA *self);
void fn_1_DB5C(M522_CAMERA *self);
void fn_1_DCC0(M522_CAMERA *self);
void fn_1_DD24(M522_CAMERA *self);
}

/* Pointer to member function constants. */
M522_PTMF lbl_1_data_410 = { 0, -1, { (void *)fn_1_D6E8 } };

char lbl_1_data_41C[] = "ms_pointer == 0";
char lbl_1_data_42C[] = "singleton.h";
char lbl_1_data_438[] = "ms_pointer != 0";

M522_PTMF lbl_1_data_448 = { 0, -1, { (void *)fn_1_D654 } };
M522_PTMF lbl_1_data_454 = { 0, -1, { (void *)fn_1_D798 } };
M522_PTMF lbl_1_data_460 = { 0, -1, { (void *)fn_1_D6E8 } };
M522_PTMF lbl_1_data_46C = { 0, -1, { (void *)fn_1_DB5C } };
M522_PTMF lbl_1_data_478 = { 0, -1, { (void *)fn_1_DB5C } };
M522_PTMF lbl_1_data_484 = { 0, -1, { (void *)fn_1_D6E8 } };

M522_VTFN lbl_1_data_490[] = {
    (M522_VTFN)0,
    (M522_VTFN)0,
    (M522_VTFN)fn_1_D3D8,
    (M522_VTFN)fn_1_D4A4,
    (M522_VTFN)fn_1_D60C,
    (M522_VTFN)0,
};

extern "C" {
CAMERA_RODATA extern const M522_CAMPATH lbl_1_rodata_390;
CAMERA_RODATA extern const f32 lbl_1_rodata_3AC;
CAMERA_RODATA extern const M522_VEC3 lbl_1_rodata_3B0;
CAMERA_RODATA extern const M522_VEC3 lbl_1_rodata_3D0[2];
CAMERA_RODATA extern const M522_VEC3 lbl_1_rodata_3E8[2];

CAMERA_RODATA const M522_CAMPATH lbl_1_rodata_390 = {
    { -194.448059f, 3260.52979f, 1098.86255f },
    { -0.47258842f, 0.000948564033f, 0.000509720878f, 0.881282687f },
};
CAMERA_RODATA const f32 lbl_1_rodata_3AC = 0.1f;
CAMERA_RODATA const M522_VEC3 lbl_1_rodata_3B0 = { -580.0f, 0.0f, 0.0f };
CAMERA_RODATA const M522_VEC3 lbl_1_rodata_3D0[2] = {
    { -580.0f, 450.0f, 0.0f },
    { 0.0f, 50.0f, 0.0f },
};
CAMERA_RODATA const M522_VEC3 lbl_1_rodata_3E8[2] = {
    { 0.0f, -460.0f, -800.0f },
    { 0.0f, -920.0f, -1100.0f },
};
}

extern "C" {

M522_CAMERA *fn_1_D1B4(M522_CAMERA *self)
{
    M522_CAMERA *singleton;

    fn_1_8004(&self->node);
    self->vtbl = lbl_1_data_3A8;
    self->flags = 0;
    fn_1_80D0(&self->node, &lbl_1_bss_2C);
    if (lbl_1_bss_A1C == 0) {
    } else {
        fn_1_6A70(lbl_1_data_41C, lbl_1_data_42C, 0x35);
    }
    singleton = self;
    lbl_1_bss_A1C = singleton;
    self->vtbl = lbl_1_data_490;
    self->hook = lbl_1_data_410;
    self->camModel = -1;
    self->duration = 1;
    self->timer = 0;
    self->debug = 0;
    self->debugFreeze = 0;
    Hu3DCameraCreate(1);
    Hu3DBGColorSet(0, 0, 0);
    fn_1_6EF0(&self->debugView, &lbl_1_rodata_390);
    return self;
}

void *fn_1_D2F4(M522_CAMERA *self, s16 flag)
{
    if (self != 0) {
        if (lbl_1_bss_A1C != 0) {
        } else {
            fn_1_6A70(lbl_1_data_438, lbl_1_data_42C, 0x3D);
        }
        lbl_1_bss_A1C = 0;
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

void *fn_1_D380(M522_NODE *node, s16 flag)
{
    if (node != 0) {
        fn_1_8010(node, 0);
        if (flag > 0) {
            fn_1_6964(node);
        }
    }
    return node;
}

void *fn_1_D3D8(M522_CAMERA *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_490;
        if (self != 0) {
            if (lbl_1_bss_A1C != 0) {
            } else {
                fn_1_6A70(lbl_1_data_438, lbl_1_data_42C, 0x3D);
            }
            lbl_1_bss_A1C = 0;
        }
        if (self != 0) {
            self->vtbl = lbl_1_data_3A8;
            if (self != 0) {
                fn_1_8010(&self->node, 0);
            }
        }
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

void fn_1_D4A4(M522_CAMERA *self, s32 mode)
{
    M522_PTMF next;

    switch (mode) {
    case 0:
        fn_1_DCC0(self);
        next = lbl_1_data_448;
        self->hook = next;
        break;
    case 1:
        fn_1_DD24(self);
        self->flags = self->flags & ~(1 << mode);
        break;
    case 2:
        fn_1_6F90(&self->view, &self->pathA);
        fn_1_6F90(&self->view, &self->pathB);
        next = lbl_1_data_454;
        self->hook = next;
        self->flags = self->flags & ~(1 << mode);
        break;
    case 3:
        fn_1_D7F4(self);
        self->flags = self->flags & ~(1 << mode);
        break;
    case 4:
        fn_1_D980(self);
        self->flags = self->flags & ~(1 << mode);
        break;
    }
}

void fn_1_D60C(M522_CAMERA *self)
{
#ifdef TARGET_PC
    if (self->hook.f_data.f_addr != 0) {
        ((void (*)(M522_CAMERA *))self->hook.f_data.f_addr)(self);
    }
#else
    if (__ptmf_test(&self->hook) != 0) {
        __ptmf_scall(self);
    }
#endif
}

void fn_1_D654(M522_CAMERA *self)
{
    M522_PTMF next;

    if (self->camModel != -1) {
        fn_1_727C(&self->view, 1);
        if (Hu3DMotionEndCheck(self->camModel) != 0) {
            self->flags = self->flags & ~1;
            next = lbl_1_data_460;
            self->hook = next;
        }
    }
}

void fn_1_D6E8(M522_CAMERA *self)
{
    fn_1_723C(&self->view, 1);
}

void fn_1_D718(M522_CAMERA *self, f32 offset)
{
    fn_1_6F90(&self->view, &self->pathA);
    self->pathB = self->pathA;
    self->pathB.pos[1] = self->pathB.pos[1] - offset;
}

void fn_1_D798(M522_CAMERA *self)
{
    fn_1_7008(&self->view, &self->pathA, &self->pathB, lbl_1_rodata_3AC);
    fn_1_6F90(&self->view, &self->pathA);
    fn_1_723C(&self->view, 1);
}

void fn_1_D7F4(M522_CAMERA *self)
{
    M522_CAMVIEW view;
    M522_VEC3 target;
    M522_PTMF next;
    f32 offset;

    fn_1_6F90(&self->view, &self->pathA);
    if (lbl_1_bss_18 != 0) {
    } else {
        fn_1_6A70(lbl_1_data_438, lbl_1_data_42C, 0x27);
    }
    offset = fn_1_F1B8(lbl_1_bss_18);
    target = lbl_1_rodata_3B0;
    target.y = (f32)(450.0 + offset);
    view.pos = self->view.pos;
    view.up.x = 0.0f;
    view.up.y = 1.0f;
    view.up.z = 0.0f;
    view.target = target;
    fn_1_6F90(&view, &self->pathB);
    self->timer = 0;
    self->duration = 0xB4;
    next = lbl_1_data_46C;
    self->hook = next;
}

void fn_1_D980(M522_CAMERA *self)
{
    M522_CAMVIEW view;
    M522_PTMF next;
    f32 offset;
    s32 index;

    fn_1_6F90(&self->view, &self->pathA);
    if (lbl_1_bss_18 != 0) {
    } else {
        fn_1_6A70(lbl_1_data_438, lbl_1_data_42C, 0x27);
    }
    offset = fn_1_F1B8(lbl_1_bss_18);
    if (fn_1_FA28() == 0) {
        index = 0;
    } else {
        index = 1;
    }
    view.target.x = lbl_1_rodata_3D0[index].x;
    view.target.y = (f32)offset + lbl_1_rodata_3D0[index].y;
    view.target.z = lbl_1_rodata_3D0[index].z;
    PSVECSubtract((Vec *)&view.target, (Vec *)&lbl_1_rodata_3E8[index], (Vec *)&view.pos);
    view.up.x = 0.0f;
    view.up.y = 1.0f;
    view.up.z = 0.0f;
    fn_1_6F90(&view, &self->pathB);
    self->timer = 0;
    self->duration = 0x78;
    next = lbl_1_data_478;
    self->hook = next;
}

void fn_1_DB5C(M522_CAMERA *self)
{
    M522_PTMF next;
    f64 ease;
    f64 square;

    ease = 1.0 - cos((3.141592653589793 * (f64)self->timer) / (f64)self->duration);
    square = ease * ease;
    fn_1_7008(&self->view, &self->pathA, &self->pathB, (f32)(square * 0.25));
    fn_1_723C(&self->view, 1);
    self->timer = self->timer + 1;
    if (self->timer > self->duration) {
        next = lbl_1_data_484;
        self->hook = next;
    }
}

void fn_1_DCC0(M522_CAMERA *self)
{
    s16 motion;

    motion = Hu3DMotionCreate(HuDataSelHeapReadNum(0x500000, 0x10000000, HEAP_MODEL));
    self->camModel = Hu3DModelCameraCreate(motion, 1);
    Hu3DCameraMotionStart(self->camModel, 1);
}

void fn_1_DD24(M522_CAMERA *self)
{
    Hu3DModelKill(self->camModel);
    self->camModel = -1;
}

void fn_1_DD5C(M522_CAMERA *self)
{
    M522_CAMVIEW *debugView;
    M522_VEC3 *debugUp;
    M522_VEC3 *debugTarget;
    u8 frozen;

    if ((HuPadBtnDown[0] & 0x800) != 0) {
        self->debug = (self->debug == 0);
    }
    if (self->debug != 0) {
        if ((HuPadBtnDown[0] & 0x40) != 0) {
            self->debugFreeze = (self->debugFreeze == 0);
        }
        if (self->debugFreeze == 0) {
            fn_1_74A4(&self->debugView, 0);
        }
        if ((HuPadBtnDown[0] & 0x20) != 0) {
            self->debugView = self->view;
        }
        fn_1_723C(&self->debugView, 1);
        debugView = &self->debugView;
        debugTarget = &self->debugView.target;
        debugUp = &self->debugView.up;
        if (self->debugFreeze != 0) {
            frozen = 0xFF;
        } else {
            frozen = 0;
        }
    }
}

}
