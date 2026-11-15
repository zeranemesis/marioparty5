// m532Dll "man_map": the map manager. It owns the stage/sun/stairs models, the
// sixteen balls (as a vector of handles), a spinning sun and the fog setup; also
// holds three methods of the camera demo object.
extern "C" {
#include "dolphin.h"
#include "game/hu3d.h"
#include "game/hsfformat.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/pad.h"
}
#include "REL/m532Dll/man_map.h"

#undef NULL
#define NULL 0

#ifdef TARGET_PC
#include <stdlib.h>
#include <math.h>
#else
extern "C" {
void abort(void);
double fabs(double x);
}
#endif

// Allocation size for a struct whose type is not visible here.
// shortcut: twice the GameCube size on PC (every field is at most pointer wide).
#ifndef M532_ALLOC_SIZE
#ifdef TARGET_PC
#define M532_ALLOC_SIZE(n) ((n) * 2)
#else
#define M532_ALLOC_SIZE(n) (n)
#endif
#endif

class M532MapFrame {
public:
    M532MapFrame(s32 frame) { m_frame = frame; }
    s32 m_frame;
};

extern "C" {

extern u8 __files[];
#ifdef TARGET_PC /* the stream is ignored by fn_1_1D7C */
#define __files ((u8 *)0)
#endif

extern void *lbl_1_data_B4[];
extern M532ManMap *lbl_1_bss_8;
extern u8 lbl_1_bss_24[];

void *fn_1_1B48(u32 size);
void fn_1_1C10(void *ptr);
void fn_1_1D1C(const char *expr, const char *file, s32 line);
void fn_1_1D7C(void *file, const char *fmt, ...);
void fn_1_24E8(void *cam, u16 bit);
void fn_1_2750(void *cam, s32 arg);
void fn_1_32B0(void *node);
void fn_1_32BC(void *node, s32 arg);
void fn_1_337C(void *node, void *where);
void fn_1_4C54(s16 *mdl, s32 dataNum);
void fn_1_4DC8(s16 *mdl, u32 attr);
void fn_1_500C(s16 *mdl, s32 layer);
void fn_1_50B4(s16 *mdl, s32 arg);
HSFOBJECT *fn_1_5110(s16 *mdl, const char *name);
void fn_1_51D4(s16 *mdl);
void fn_1_7084(s32 camBit, Vec *dir, Vec *target, f32 width, f32 depth);
void *fn_1_604(void *ball, f32 angle, s32 idx);
void *fn_1_EB7C(void *ball);
void *fn_1_EB84(void *handle);
Vec *fn_1_984(void *ball);
u8 fn_1_98C(void *ball);
void *fn_1_8DC(void *ball, Vec *pt);

char lbl_1_data_400[] = "ms_pointer == 0";
char lbl_1_data_410[] = "singleton.h";
char lbl_1_data_41C[] = "tenmen_sun";
char lbl_1_data_427[] = "tenmen_sun_b";
char lbl_1_data_434[] = "kaidan";
char lbl_1_data_43B[] = "lights > 0";
char lbl_1_data_446[] = "man_map.cpp";
char lbl_1_data_452[] = "inf_light1";
char lbl_1_data_45D[] = "light_id >= 0";
char lbl_1_data_46B[] = "ms_pointer != 0";
char lbl_1_data_490[] = "vector::insert length error\n";
char lbl_1_data_4AD[] = "Memory allocation failure";

void *lbl_1_data_47C[5] = { 0, 0, (void *)fn_1_A3C0, (void *)fn_1_A7FC, (void *)fn_1_A814 };

// Constants of the original .rodata.
static const Vec lbl_1_rodata_238 = { 0.0f, 170.0f, 300.0f };
static const M532MapFog lbl_1_rodata_290 = { 0, 0.0f, 0.0f, { 0, 0, 0, 0 } };
static const GXColor lbl_1_rodata_2A0 = { 0, 0, 0, 0 };

// Static initialisers (fn_1_AF9C on GameCube).
M532MapFrame lbl_1_bss_A48(300);
M532MapFrame lbl_1_bss_A4C(60);

static void ManMapSingletonClear(void)
{
    if (lbl_1_bss_8 != NULL) {
        (void)0;
    } else {
        fn_1_1D1C(lbl_1_data_46B, lbl_1_data_410, 0x3D);
    }
    lbl_1_bss_8 = NULL;
}

// NONMATCHING: register allocation and inlined temporaries differ
void fn_1_9B9C(M532CamCtl *self)
{
    s16 mot = Hu3DMotionCreateData(0x5A0000);

    self->camMdl = Hu3DModelCameraCreate(mot, 1);
    Hu3DCameraMotionStart(self->camMdl, 1);
}

void fn_1_9C00(M532CamCtl *self)
{
    Hu3DModelKill(self->camMdl);
    self->camMdl = -1;
}

// NONMATCHING: register allocation and inlined temporaries differ
void fn_1_9C38(M532CamCtl *self)
{
    u16 btn = HuPadBtnDown[0];

    if (btn & 0x800) {
        self->freeCam = (self->freeCam == 0);
    }
    if (self->freeCam) {
        if (btn & 0x40) {
            self->hold = (self->hold == 0);
        }
        if (self->hold == 0) {
            fn_1_2750(&self->pos2, 0);
        }
        if (HuPadBtnDown[0] & 0x20) {
            self->pos2 = self->pos;
            self->target2 = self->target;
            self->up2 = self->up;
        }
        fn_1_24E8(&self->pos2, 1);
    }
}

// NONMATCHING: register allocation and inlined temporaries differ
M532ManMap *fn_1_9D7C(M532ManMap *self)
{
    s16 mdl;
    s32 lights;
    s16 lightId;
    Vec pos;
    Vec dir;
    GXColor color;
    Vec shadowDir;
    s32 i;

    fn_1_32B0(self);
    self->vtable = lbl_1_data_B4;
    self->flags = 0;
    fn_1_337C(self, lbl_1_bss_24);
    if (lbl_1_bss_8 != NULL) {
        fn_1_1D1C(lbl_1_data_400, lbl_1_data_410, 0x35);
    }
    lbl_1_bss_8 = self;
    fn_1_4C54(&self->mdl[0], 0x5A0001);
    self->vtable = lbl_1_data_47C;
    fn_1_4C54(&self->mdl[1], 0x5A0002);
    fn_1_4C54(&self->mdl[2], 0x5A0003);
    fn_1_4C54(&self->mdl[3], 0x5A0004);
    self->balls.cap = 0;
    self->balls.size = 0;
    self->balls.items = NULL;
    self->sunRotA = &fn_1_5110(&self->mdl[0], lbl_1_data_41C)->mesh.base.rot.y;
    self->sunRotB = &fn_1_5110(&self->mdl[0], lbl_1_data_427)->mesh.base.rot.y;
    self->angle = 11.25f;
    self->speed = 0.0f;
    self->se = 0;
    fn_1_500C(&self->mdl[0], 2);
    fn_1_4DC8(&self->mdl[0], 0x40000001);
    fn_1_500C(&self->mdl[1], 0);
    fn_1_500C(&self->mdl[2], 1);
    fn_1_500C(&self->mdl[3], 2);
    fn_1_51D4(&self->mdl[3]);
    fn_1_5110(&self->mdl[3], lbl_1_data_434)->flags |= 0x4000;
    mdl = self->mdl[0];
    lights = Hu3DModelLightInfoSet(mdl, 1);
    if (lights > 0) {
        (void)0;
    } else {
        fn_1_1D1C(lbl_1_data_43B, lbl_1_data_446, 0x35);
    }
    mdl = self->mdl[0];
    lightId = Hu3DModelLightIdGet(mdl, lbl_1_data_452);
    if (lightId >= 0) {
        (void)0;
    } else {
        fn_1_1D1C(lbl_1_data_45D, lbl_1_data_446, 0x3B);
    }
    Hu3DGLightParamGet(lightId, &pos, &dir, &color);
    shadowDir = lbl_1_rodata_238;
    fn_1_7084(1, &dir, &shadowDir, 680.0f, 445.0f);
    fn_1_51D4(&self->mdl[0]);
    fn_1_A874(self);
    Hu3DLayerHookSet(0, fn_1_AB38);
    Hu3DLayerHookSet(1, fn_1_AA64);
    Hu3DLayerHookSet(7, fn_1_AB38);
    for (i = 0; i < 16; i++) {
        f32 angle = (f32)(360.0 * ((0.5 + (f64)i) * 0.0625));
        void *ball = fn_1_1B48(M532_ALLOC_SIZE(0x5C));
        void *handle;

        if (ball != NULL) {
            ball = fn_1_604(ball, angle, i);
        }
        handle = fn_1_EB7C(ball);
        fn_1_ABE4(&self->balls, self->balls.items + self->balls.size, 1, &handle);
    }
    return self;
}

// The three vector destructors differ only in how many redundant null checks the
// compiler left behind.
// NONMATCHING: register allocation and inlined temporaries differ
void *fn_1_A190(void *self, s16 flag)
{
    M532MapVec *vec = (M532MapVec *)self;

    if (self != NULL) {
        if (vec->items != NULL) {
            fn_1_1C10(vec->items);
        }
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

// NONMATCHING: register allocation and inlined temporaries differ
void *fn_1_A224(void *self, s16 flag)
{
    return fn_1_A190(self, flag);
}

// NONMATCHING: register allocation and inlined temporaries differ
void *fn_1_A2B0(void *self, s16 flag)
{
    return fn_1_A190(self, flag);
}

void *fn_1_A334(void *self, s16 flag)
{
    if (self != NULL) {
        ManMapSingletonClear();
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

// NONMATCHING: register allocation and inlined temporaries differ
M532ManMap *fn_1_A3C0(M532ManMap *self, s16 flag)
{
    if (self != NULL) {
        self->vtable = lbl_1_data_47C;
        if (self->balls.items != NULL) {
            fn_1_1C10(self->balls.items);
        }
        fn_1_50B4(&self->mdl[3], -1);
        fn_1_50B4(&self->mdl[2], -1);
        fn_1_50B4(&self->mdl[1], -1);
        fn_1_50B4(&self->mdl[0], 0);
        ManMapSingletonClear();
        self->vtable = lbl_1_data_B4;
        fn_1_32BC(self, 0);
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

void fn_1_A518(M532ManMap *self)
{
    self->speed = 1.125f;
    self->se = HuAudFXPlay(0x7AB);
}

void fn_1_A55C(M532ManMap *self)
{
    self->speed = 0.0f;
    HuAudFXStop(self->se);
}

f32 fn_1_A59C(M532ManMap *self)
{
    return self->angle;
}

// True when a live ball in front (z > 0) is within 20 units of x.
// NONMATCHING: register allocation and inlined temporaries differ
s32 fn_1_A5A4(M532ManMap *self, f32 x)
{
    void **it = self->balls.items;
    void **end = self->balls.items + self->balls.size;

    for (; it != end; it++) {
        void *ball = fn_1_EB84(*it);
        Vec *pos;
        f32 dx;

        if (fn_1_98C(ball) != 0) {
            continue;
        }
        pos = fn_1_984(ball);
        if (pos->z <= 0.0f) {
            continue;
        }
        dx = x - pos->x;
        if (fabs(dx) < 20.0) {
            return 1;
        }
    }
    return 0;
}

// Nearest live ball to pt (within 10000 units); returns fn_1_8DC's handle or 0.
// NONMATCHING: register allocation and inlined temporaries differ
void *fn_1_A6C0(M532ManMap *self, Vec *pt)
{
    void **it = self->balls.items;
    void **end = self->balls.items + self->balls.size;
    void *best = NULL;
    f32 bestDist = 10000.0f;

    for (; it != end; it++) {
        void *ball = fn_1_EB84(*it);
        Vec diff;
        f32 dist;

        if (fn_1_98C(ball) != 0) {
            continue;
        }
        PSVECSubtract(pt, fn_1_984(ball), &diff);
        dist = PSVECMag(&diff);
        if (bestDist > dist) {
            bestDist = dist;
            best = ball;
        }
    }
    if (best == NULL) {
        return NULL;
    }
    return fn_1_8DC(best, pt);
}

// NONMATCHING: register allocation and inlined temporaries differ
void fn_1_A7FC(M532ManMap *self, s32 bit)
{
    self->flags = self->flags & ~(1 << bit);
}

// NONMATCHING: register allocation and inlined temporaries differ
void fn_1_A814(M532ManMap *self)
{
    self->angle = self->angle + self->speed;
    if (self->angle >= 360.0f) {
        self->angle = self->angle - 360.0f;
    }
    *self->sunRotA = self->angle;
    *self->sunRotB = -self->angle;
}

// Reads the stage's own fog from the model's scene data.
// NONMATCHING: register allocation and inlined temporaries differ
void fn_1_A874(M532ManMap *self)
{
    s16 id = self->mdl[0];
    HU3DMODEL *model = &Hu3DData[id];
    HSFDATA *hsf = model->hsf;

    if (hsf->sceneNum > 0) {
        HSFSCENE scene = *hsf->scene;

        if (scene.fogStart > 0.0f && scene.fogStart < scene.fogEnd) {
            self->fog.type = 4;
            self->fog.start = scene.fogStart;
            self->fog.end = scene.fogEnd;
            self->fog.color.r = scene.fogColor.r;
            self->fog.color.g = scene.fogColor.g;
            self->fog.color.b = scene.fogColor.b;
            self->fog.color.a = 0xFF;
            return;
        }
    }
    self->fog = lbl_1_rodata_290;
}

// NONMATCHING: register allocation and inlined temporaries differ
void fn_1_A9E0(M532ManMap *self)
{
    f32 fov, nearZ, farZ;
    GXColor color;

    Hu3DCameraPerspectiveGet(1, &fov, &nearZ, &farZ);
    color = self->fog.color;
    GXSetFog((GXFogType)self->fog.type, self->fog.start, self->fog.end, nearZ, farZ, color);
}

// Layer hook: applies the map's fog.
// NONMATCHING: register allocation and inlined temporaries differ
void fn_1_AA64(s16 layer)
{
    M532ManMap *self;
    f32 fov, nearZ, farZ;
    GXColor color;

    if (lbl_1_bss_8 != NULL) {
        (void)0;
    } else {
        fn_1_1D1C(lbl_1_data_46B, lbl_1_data_410, 0x27);
    }
    self = lbl_1_bss_8;
    Hu3DCameraPerspectiveGet(1, &fov, &nearZ, &farZ);
    color = self->fog.color;
    GXSetFog((GXFogType)self->fog.type, self->fog.start, self->fog.end, nearZ, farZ, color);
}

// Layer hook: no fog.
void fn_1_AB38(s16 layer)
{
    GXColor color = lbl_1_rodata_2A0;

    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, color);
}

// NONMATCHING: plain vector<void *>::insert(pos, n, value); the original is the inlined library body.
void fn_1_ABE4(M532MapVec *self, void **pos, u32 n, void **val)
{
    void *value;
    const u32 max = 0x7FFFFFFF;

    if (n == 0) {
        return;
    }
    if (n > max || self->size > max - n) {
        fn_1_1D7C(__files + 0xA0, lbl_1_data_490);
        abort();
    }
    value = *val;
    if (self->size + n <= self->cap) {
        void **end = self->items + self->size;
        void **p;
        u32 k;

        if (end != pos) {
            void **src = end;
            void **dst = end + n;
            while (src > pos) {
                *--dst = *--src;
            }
        }
        p = pos;
        for (k = n; k != 0; k--) {
            *p++ = value;
        }
        self->size += n;
    } else {
        s32 idx = (s32)(pos - self->items);
        u32 newSize = self->size + n;
        u32 cap = (self->cap != 0) ? self->cap : 1;
        void **old = self->items;
        void **mem;
        void **p;
        u32 k;
        s32 j;

        while (newSize > cap) {
            if (cap < (max >> 1)) {
                cap *= 2;
            } else {
                cap = max;
            }
        }
        mem = (void **)fn_1_1B48(cap * sizeof(void *));
        if (mem == NULL) {
            fn_1_1D7C(__files + 0xA0, lbl_1_data_4AD);
            abort();
        }
        self->items = mem;
        p = mem + idx;
        for (k = n; k != 0; k--) {
            *p++ = value;
        }
        if (old != NULL) {
            for (j = 0; j < idx; j++) {
                mem[j] = old[j];
            }
            for (j = idx; j < (s32)self->size; j++) {
                mem[j + n] = old[j];
            }
            fn_1_1C10(old);
        }
        self->size = newSize;
        self->cap = cap;
    }
}

}
