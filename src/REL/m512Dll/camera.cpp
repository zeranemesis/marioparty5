// m512Dll/REL/m512Dll/camera
// CCamera: the minigame camera singleton.  A PTMF state hook picks the
// behaviour (follow the players, debug fly, intro motion, ending orbit).
#define _MATH_H
#include "types.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/audio.h"
#include "game/pad.h"
#include "game/hsfex.h"

#ifdef __MWERKS__
#define CAM_RODATA __declspec(section ".rodata")
#else
#define CAM_RODATA
#endif

struct CamNode {
    CamNode *next; // 0x00
    CamNode *prev; // 0x04
};

// GC pointer-to-member-function layout
struct CamPtmf {
    s32 adj;
    s32 idx;
    void (*fn)(void *);
};

struct Camera512 {
    CamNode node;   // 0x00
    void **vtbl;    // 0x08
    s32 unkC;       // 0x0C
    HuVecF pos;     // 0x10
    HuVecF up;      // 0x1C
    HuVecF target;  // 0x28
    s32 frame;      // 0x34
    CamPtmf hook;   // 0x38
    s16 motion;     // 0x44
};

extern "C" {
double sin(double x);
double cos(double x);
void *__dl__FPv(void *);
s32 __ptmf_test(void *ptmf);
void __ptmf_scall(void *obj);
void *fn_1_1264(void *self);
void fn_1_1270(void *self, int flag);
void fn_1_1330(void *self, void *arg);
void fn_1_1D4(const char *msg, const char *file, int line);
s32 fn_1_1F30(void);
void fn_1_A350(HuVecF *center, HuVecF *extent);
f32 fn_1_A4BC(HuVecF *out);
void fn_1_C08(void *self, s32 arg);

extern CamNode lbl_1_bss_C;
extern void *lbl_1_data_1FC[4];
extern Camera512 *lbl_1_bss_A14;
}

extern "C" void fn_1_6C98(Camera512 *self);
extern "C" void fn_1_6F0C(Camera512 *self);
extern "C" void fn_1_7130(Camera512 *self);
extern "C" void fn_1_7214(Camera512 *self);
extern "C" void fn_1_74CC(Camera512 *self, s32 bit);
extern "C" void fn_1_764C(Camera512 *self);
extern "C" s32 fn_1_7708(HuVecF *pos, s32 se);
extern "C" void *fn_1_77D8(Camera512 *self, s16 flag);

// -------------------------------------------------------------------- data

extern "C" {
CamPtmf lbl_1_data_268 = { 0, -1, (void (*)(void *))fn_1_6C98 };
char lbl_1_data_274[] = "ms_pointer == 0";
char lbl_1_data_284[] = "singleton.h";
char lbl_1_data_290[] = "ms_pointer != 0";
CamPtmf lbl_1_data_2A0 = { 0, -1, (void (*)(void *))fn_1_6C98 };
CamPtmf lbl_1_data_2AC = { 0, -1, (void (*)(void *))fn_1_6C98 };
CamPtmf lbl_1_data_2B8 = { 0, -1, (void (*)(void *))fn_1_7214 };
CamPtmf lbl_1_data_2C4 = { 0, -1, (void (*)(void *))fn_1_7130 };
CamPtmf lbl_1_data_2D0 = { 0, -1, (void (*)(void *))fn_1_6F0C };
void *lbl_1_data_2DC[5] = { 0, 0, (void *)fn_1_77D8, (void *)fn_1_74CC, (void *)fn_1_764C };

CAM_RODATA const f32 lbl_1_rodata_80[3] = { 0.0f, 2800.0f, 3000.0f };
CAM_RODATA const f32 lbl_1_rodata_8C[3] = { 0.0f, 1.0f, 0.0f };
CAM_RODATA const f32 lbl_1_rodata_98[3] = { 0.0f, 0.0f, 200.0f };
}

// ------------------------------------------------------------------ functions

extern "C" Camera512 *fn_1_67FC(Camera512 *self)
{
    CamPtmf tmp;

    fn_1_1264(self);
    self->vtbl = lbl_1_data_1FC;
    self->unkC = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    if (lbl_1_bss_A14 == 0) {
    } else {
        fn_1_1D4(lbl_1_data_274, lbl_1_data_284, 0x35);
    }
    {
        Camera512 *s = (Camera512 *)((char *)self + 0x10);
        if (s != 0) {
            s = (Camera512 *)((char *)s - 0x10);
        }
        lbl_1_bss_A14 = s;
    }
    self->vtbl = lbl_1_data_2DC;
    tmp = lbl_1_data_268;
    self->hook = tmp;
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 45.0f, 100.0f, 10000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCamera[0].fov = 45.0f;
    Hu3DFogSet(1500.0f, 30000.0f, 200, 200, 200);
    self->pos.x = lbl_1_rodata_80[0];
    self->pos.y = lbl_1_rodata_80[1];
    self->pos.z = lbl_1_rodata_80[2];
    self->up.x = lbl_1_rodata_8C[0];
    self->up.y = lbl_1_rodata_8C[1];
    self->up.z = lbl_1_rodata_8C[2];
    self->target.x = lbl_1_rodata_98[0];
    self->target.y = lbl_1_rodata_98[1];
    self->target.z = lbl_1_rodata_98[2];
    self->frame = 0;
    fn_1_6C98(self);
    return self;
}

extern "C" Camera512 *fn_1_6A5C(Camera512 *self, s16 flag)
{
    if (self != 0) {
        if (lbl_1_bss_A14 != 0) {
        } else {
            fn_1_1D4(lbl_1_data_290, lbl_1_data_284, 0x3D);
        }
        lbl_1_bss_A14 = 0;
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

extern "C" void *fn_1_6AE8(void *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// Camera position for a player centre: out = centre + distance * (0, cos 45, sin 45)
extern "C" void fn_1_6B40(void *self, HuVecF *center, HuVecF *out)
{
    HuVecF ext;
    f32 dist;
    f32 off;

    fn_1_A350(center, &ext);
    dist = ext.x;
    ext.z = ext.z * 0.9;
    if (ext.z > dist) {
        dist = ext.z;
    }
    dist += 750.0f;
    center->y += 100.0f;
#ifdef TARGET_PC
    off = 0.0f;
#endif
    if (ext.x > ext.z) {
        off = 0.1f * (ext.x - ext.z);
    }
    center->z -= off;
    center->z += 200.0f;
    out->x = center->x;
    out->y = center->y + dist * cos(0.7853981633974483);
    out->z = center->z + dist * sin(0.7853981633974483);
}

extern "C" void fn_1_6C98(Camera512 *self)
{
    HuVecF ext;
    HuVecF res;
    HuVecF center;
    f32 dist;
    f32 off;
    HuVecF *pos = &self->pos;
    HuVecF *target = &self->target;

    fn_1_A350(&center, &ext);
    dist = ext.x;
    ext.z = ext.z * 0.9;
    if (ext.z > dist) {
        dist = ext.z;
    }
    dist += 750.0f;
    center.y += 100.0f;
#ifdef TARGET_PC
    off = 0.0f;
#endif
    if (ext.x > ext.z) {
        off = 0.1f * (ext.x - ext.z);
    }
    center.z -= off;
    center.z = center.z + 200.0f;
    res.x = center.x;
    res.y = center.y + dist * cos(0.7853981633974483);
    res.z = center.z + dist * sin(0.7853981633974483);
    pos->x += 0.05f * (res.x - pos->x);
    pos->y += 0.05f * (res.y - pos->y);
    pos->z += 0.05f * (res.z - pos->z);
    target->x += 0.1f * (center.x - target->x);
    target->y += 0.1f * (center.y - target->y);
    target->z += 0.1f * (center.z - target->z);
    Hu3DCameraPosSetV(1, (Vec *)pos, (Vec *)&self->up, (Vec *)target);
}

extern "C" void fn_1_6F0C(Camera512 *self)
{
    HuVecF center;
    HuVecF res;
    HuVecF ext;
    f32 dist;
    f32 off;

    fn_1_A350(&center, &ext);
    dist = ext.x;
    ext.z = ext.z * 0.9;
    if (ext.z > dist) {
        dist = ext.z;
    }
    dist += 750.0f;
    center.y += 100.0f;
#ifdef TARGET_PC
    off = 0.0f;
#endif
    if (ext.x > ext.z) {
        off = 0.1f * (ext.x - ext.z);
    }
    center.z -= off;
    center.z = center.z + 200.0f;
    res.x = center.x;
    res.y = center.y + dist * cos(0.7853981633974483);
    res.z = center.z + dist * sin(0.7853981633974483);
    Hu3DCameraPosSetV(1, (Vec *)&res, (Vec *)&self->up, (Vec *)&center);
}

extern "C" void fn_1_7074(Camera512 *self)
{
    CamPtmf tmp;

    fn_1_C08(&self->pos, 0);
    if ((HuPadBtnDown[0] & 0x800) != 0) {
        tmp = lbl_1_data_2A0;
        self->hook = tmp;
    }
    Hu3DCameraPosSetV(1, (Vec *)&self->pos, (Vec *)&self->up, (Vec *)&self->target);
}

extern "C" void fn_1_7130(Camera512 *self)
{
    HuVecF pos;
    HuVecF up;
    HuVecF target;
    CamPtmf tmp;

    if (Hu3DMotionEndCheck(self->motion) != 0) {
        tmp = lbl_1_data_2AC;
        self->hook = tmp;
        self->unkC &= ~1;
        Hu3DCameraPosGet(1, (Vec *)&pos, (Vec *)&up, (Vec *)&target);
        self->pos = pos;
        self->up = up;
        self->target = target;
    }
    self->frame = self->frame + 1;
}

extern "C" void fn_1_7214(Camera512 *self)
{
    HuVecF center;
    HuVecF res;
    f32 ang;
    f32 dx;
    f32 dz;
    HuVecF *pos = &self->pos;
    HuVecF *target = &self->target;

    ang = 90.0f + -fn_1_A4BC(&center);
    dx = 100.0 * (10.0 * cos((3.141592653589793 * ang) / 180.0));
    dz = 100.0 * (10.0 * sin((3.141592653589793 * ang) / 180.0));
    center.y += 100.0f;
    res.x = center.x + dx;
    res.z = center.z + dz;
    res.y = 300.0f + center.y;
    pos->x += 0.05f * (res.x - pos->x);
    pos->y += 0.05f * (res.y - pos->y);
    pos->z += 0.05f * (res.z - pos->z);
    target->x += 0.1f * (center.x - target->x);
    target->y += 0.1f * (center.y - target->y);
    target->z += 0.1f * (center.z - target->z);
    Hu3DCameraPosSetV(1, (Vec *)pos, (Vec *)&self->up, (Vec *)target);
}

extern "C" void fn_1_7488(Camera512 *self)
{
    CamPtmf tmp;

    tmp = lbl_1_data_2B8;
    self->hook = tmp;
}

extern "C" void fn_1_74CC(Camera512 *self, s32 bit)
{
    CamPtmf tmp;

    switch (bit) {
    case 0:
        self->frame = 0;
        tmp = lbl_1_data_2C4;
        self->hook = tmp;
        self->motion = Hu3DModelCameraCreate(Hu3DMotionCreate(HuDataSelHeapReadNum(0x460012, 0x10000000, HEAP_MODEL)), 1);
        Hu3DCameraMotionStart(self->motion, 1);
        return;
    case 1:
        Hu3DModelKill(self->motion);
        self->unkC &= ~(1 << bit);
        return;
    case 2:
        tmp = lbl_1_data_2D0;
        self->hook = tmp;
        self->unkC &= ~(1 << bit);
        return;
    case 3:
        self->unkC &= ~(1 << bit);
        return;
    case 4:
        self->unkC &= ~(1 << bit);
        return;
    }
}

extern "C" void fn_1_764C(Camera512 *self)
{
#ifdef TARGET_PC
    if (self->hook.fn != 0) {
        self->hook.fn(self);
        Hu3DFogSet(1000.0f, 30000.0f, 200, 200, 200);
    }
#else
    if (__ptmf_test(&self->hook) != 0) {
        __ptmf_scall(self);
        Hu3DFogSet(1000.0f, 30000.0f, 200, 200, 200);
    }
#endif
}

extern "C" s32 fn_1_76BC(HuVecF *pos, s32 se)
{
    HuVecF *p = pos;
    s32 s = se;

    if (fn_1_1F30() == 2) {
        fn_1_7708(p, s);
    }
    return -1;
}

extern "C" s32 fn_1_7708(HuVecF *pos, s32 se)
{
    HuVecF scr;
    s16 pan;
    s16 p;
    s32 handle;

    Hu3D3Dto2D(pos, 1, &scr);
    scr.x = scr.x * 0.055555556f;
    p = (s16)(48.0f + scr.x);
    if (p < 0) {
        pan = 0;
    } else if (p > 0x7F) {
        pan = 0x7F;
    } else {
        pan = p;
    }
    handle = HuAudFXPlay(se);
    HuAudFXPanning(handle, pan);
    return handle;
}

extern "C" void *fn_1_77D8(Camera512 *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_2DC;
        if ((void *)((char *)self + 0x10) != 0) {
            if (lbl_1_bss_A14 != 0) {
            } else {
                fn_1_1D4(lbl_1_data_290, lbl_1_data_284, 0x3D);
            }
            lbl_1_bss_A14 = 0;
        }
        if (self != 0) {
            self->vtbl = lbl_1_data_1FC;
            if (self != 0) {
                fn_1_1270(self, 0);
            }
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}
