#define _MATH_H /* the original object has no math.h sqrt literals */
#include "types.h"
#include "game/hu3d.h"
#include "REL/m568Dll.h"

/*
 * camera.cpp: the minigame camera singleton. A pointer-to-member state hook
 * picks the behaviour each frame (two scripted pans between a pair of
 * position/orientation keys, an intro blend and a fixed shot).
 */

#ifdef __MWERKS__
#define CAM_RODATA __declspec(section ".rodata")
#else
#define CAM_RODATA
#endif

// GC pointer-to-member-function layout
struct CamPtmf {
    s32 adj;
    s32 idx;
    void (*fn)(void *);
};

// Look-at camera state.
struct CamState {
    Vec pos;     // 0x00
    Vec up;      // 0x0C
    Vec target;  // 0x18
};

// Position + orientation key (the look-at state converted by fn_1_D0C).
struct CamKey {
    Vec pos;     // 0x00
    f32 quat[4]; // 0x0C
};

struct Camera568 {
    M568Link node;   // 0x00
    void **vtbl;     // 0x08
    s32 flags;       // 0x0C
    CamState state;  // 0x10
    s32 frame;       // 0x34
    CamPtmf hook;    // 0x38
    s32 f44;         // 0x44
    s32 f48;         // 0x48
    u8 pad4C[8];     // 0x4C
    CamKey key0;     // 0x54
    CamKey key1;     // 0x70
};

extern "C" {

double sin(double x);
double cos(double x);
s32 __ptmf_test(void *ptmf);
void __ptmf_scall(void *obj);
extern CamPtmf __ptmf_null;

void *fn_1_1948(void *obj, void *list);
void fn_1_1220(CamState *state, s32 arg);
void fn_1_B1C(CamState *state, Vec *v);
void fn_1_D0C(CamState *state, CamKey *key);
void fn_1_D84(CamState *state, CamKey *key0, CamKey *key1, f32 t);
f32 fn_1_DEC0(M568PosOwner *owner);
void fn_1_E94(CamState *state, Vec *v, f32 f);
void fn_1_FB8(CamState *state, s32 bit);

extern void *lbl_1_data_23C[4];
extern M568Link lbl_1_bss_C;
extern Camera568 *lbl_1_bss_A14;

void *fn_1_B59C(Camera568 *self, s16 flag);
void fn_1_B104(Camera568 *self, s32 mode);
void fn_1_B2CC(Camera568 *self);
void fn_1_9FE8(f32 t);
void fn_1_A664(Camera568 *self);
void fn_1_AE00(Camera568 *self);
void fn_1_B4A0(Camera568 *self);

char lbl_1_data_388[] = "ms_pointer == 0";
char lbl_1_data_398[] = "singleton.h";
char lbl_1_data_3A4[] = "ms_pointer != 0";
f32 lbl_1_data_3B4 = 18.966015f;
f32 lbl_1_data_3B8 = 23.7f;
f32 lbl_1_data_3BC = 38.16f;
CamPtmf lbl_1_data_3C0 = { 0, -1, (void (*)(void *))fn_1_AE00 };
CamPtmf lbl_1_data_3CC = { 0, -1, (void (*)(void *))fn_1_A664 };
CamPtmf lbl_1_data_3D8 = { 0, -1, (void (*)(void *))fn_1_B4A0 };
void *lbl_1_data_3E4[5] = { 0, 0, (void *)fn_1_B59C, (void *)fn_1_B104, (void *)fn_1_B2CC };

Camera568 *fn_1_9D60(Camera568 *self)
{
    Camera568 *s;
    CamPtmf tmp;

    fn_1_187C(self);
    self->vtbl = lbl_1_data_23C;
    self->flags = 0;
    fn_1_1948(self, &lbl_1_bss_C);
    if (lbl_1_bss_A14 == 0) {
    } else {
        fn_1_7EC(lbl_1_data_388, lbl_1_data_398, 0x35);
    }
    s = self;
    lbl_1_bss_A14 = s;
    self->vtbl = lbl_1_data_3E4;
    tmp = __ptmf_null;
    self->hook = tmp;
    self->f44 = 0;
    self->f48 = 0;
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 35.7766f, 100.0f, 50000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    fn_1_9FE8(0.0f);
    return self;
}

Camera568 *fn_1_9F04(Camera568 *self, s16 flag)
{
    if (self != 0) {
        if (lbl_1_bss_A14 != 0) {
        } else {
            fn_1_7EC(lbl_1_data_3A4, lbl_1_data_398, 0x3D);
        }
        lbl_1_bss_A14 = 0;
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

void *fn_1_9F90(void *self, s16 flag)
{
    if (self != 0) {
        fn_1_1888(self, 0);
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

// Fog and background colour for blend t (0 = clear sky, 1 = full white-out).
void fn_1_9FE8(f32 t)
{
    f32 start;
    s32 r, g, b;

    start = (1.0 - t) * 10000.0 + t * 0.001f;
    r = (1.0 - t) * 208.0 + t * 255.0;
    g = (1.0 - t) * 224.0 + t * 255.0;
    b = (1.0 - t) * 224.0 + t * 255.0;
    Hu3DFogSet(start, 1000.0f + start, (u8)r, (u8)g, (u8)b);
    Hu3DBGColorSet((u8)r, (u8)g, (u8)b);
}

// Pan start shot: pos in arg1, target in arg2.
void fn_1_A198(Camera568 *self, Vec *pos, Vec *target)
{
    M568PosOwner *owner;
    f32 base, lo, hi;
    Vec a, b;

    if (lbl_1_bss_9F0 != 0) {
    } else {
        fn_1_7EC(lbl_1_data_3A4, lbl_1_data_398, 0x27);
    }
    owner = lbl_1_bss_9F0;
    base = fn_1_DEC0(owner) + 922.7f;
    lo = base - 100.0f * lbl_1_data_3B8 * 0.5f;
    a.x = 1600.0f;
    a.y = -2000.0f;
    a.z = lo;
    hi = base - 100.0f * lbl_1_data_3B8 * 0.5f;
    b.x = 1600.0f;
    b.y = 2000.0f;
    b.z = hi;
    owner->src.Project(&b, &b, &a);
    pos->x = 400.0f;
    pos->y = b.y + 100.0f * (lbl_1_data_3B4 * 0.5f);
    pos->z = base - 100.0f * lbl_1_data_3B8;
    target->x = 0.0f;
    target->y = 0.0f;
    target->z = base;
}

// Pan end shot: pos in arg1, target in arg2.
void fn_1_A3F8(Camera568 *self, Vec *pos, Vec *target)
{
    M568PosOwner *owner;
    f32 base, lo, hi, h;
    Vec a, b;

    if (lbl_1_bss_9F0 != 0) {
    } else {
        fn_1_7EC(lbl_1_data_3A4, lbl_1_data_398, 0x27);
    }
    owner = lbl_1_bss_9F0;
    base = fn_1_DEC0(owner) + 100.0f * lbl_1_data_3BC;
    lo = base - 100.0f * lbl_1_data_3B8 * 0.5f;
    a.x = 1600.0f;
    a.y = -2000.0f;
    a.z = lo;
    h = 100.0f * lbl_1_data_3B8;
    hi = base - h * 0.5f;
    b.x = 1600.0f;
    b.y = 2000.0f;
    b.z = hi;
    owner->src.Project(&b, &b, &a);
    pos->x = 1.0f;
    pos->y = b.y + 100.0f * lbl_1_data_3B4;
    pos->z = base;
    target->x = 1.0f;
    target->y = b.y;
    target->z = base - 100.0f * lbl_1_data_3B8;
}

static inline f32 Lerp(f32 a, f32 b, f32 t)
{
    f32 r = (1.0 - t) * a + t * b;
    return r;
}

// Blends the start and end shots over 150 frames, then hands over to AE00.
void fn_1_A664(Camera568 *self)
{
    Vec posA, targA, posB, targB;
    f32 t;

    fn_1_A198(self, &posA, &targA);
    fn_1_A3F8(self, &posB, &targB);
    if (self->frame >= 0x96) {
        self->hook = lbl_1_data_3C0;
        self->flags &= ~1;
    }
    t = (f32)self->frame / (f32)0x96;
    self->state.pos.x = Lerp(posA.x, posB.x, t);
    self->state.pos.y = Lerp(posA.y, posB.y, t);
    self->state.pos.z = Lerp(posA.z, posB.z, t);
    self->state.target.x = Lerp(targA.x, targB.x, t);
    self->state.target.y = Lerp(targA.y, targB.y, t);
    self->state.target.z = Lerp(targA.z, targB.z, t);
    self->state.up.x = 0.0f;
    self->state.up.y = 1.0f;
    self->state.up.z = 0.0f;
    fn_1_FB8(&self->state, 1);
}

// Settled end shot.
void fn_1_AE00(Camera568 *self)
{
    Vec pos, target;

    fn_1_A3F8(self, &pos, &target);
    self->state.pos.x = pos.x;
    self->state.pos.y = pos.y;
    self->state.pos.z = pos.z;
    self->state.target.x = target.x;
    self->state.target.y = target.y;
    self->state.target.z = target.z;
    self->state.up.x = 0.0f;
    self->state.up.y = 1.0f;
    self->state.up.z = 0.0f;
    fn_1_FB8(&self->state, 1);
}

void fn_1_B0D0(void)
{
}

void fn_1_B0D4(Camera568 *self)
{
    fn_1_1220(&self->state, 0);
}

// Mode switch (vtable slot): selects the per-frame hook.
void fn_1_B104(Camera568 *self, s32 mode)
{
    CamState *st;

    self->frame = 0;
    switch (mode) {
    case 0:
        self->hook = lbl_1_data_3CC;
        break;
    case 1:
        self->flags &= ~(1 << mode);
        break;
    case 2:
        self->flags &= ~(1 << mode);
        break;
    case 4:
        self->hook = __ptmf_null;
        st = &self->state;
        st->up.x = 0.0f;
        st->up.y = 1.0f;
        st->up.z = 0.0f;
        self->state.pos.x = 0.0f;
        st->pos.y = 712.93f;
        st->pos.z = 3394.86f;
        st->target.x = 0.0f;
        st->target.y = 367.26f;
        st->target.z = -54.77f;
        fn_1_FB8(st, 1);
        Hu3DCamera[0].fov = 18.43f;
        break;
    }
}

void fn_1_B2CC(Camera568 *self)
{
    if (__ptmf_test(&self->hook) != 0) {
        __ptmf_scall(self);
    }
    self->frame++;
}

// Starts the pan towards pos: key0 = current shot, key1 = framed on pos / z.
void fn_1_B320(Camera568 *self, Vec *pos, volatile f32 z)
{
    CamState tmp;
    Vec v;
    f64 x0, y0, z0;

    fn_1_D0C(&self->state, &self->key0);
    tmp = self->state;
    z0 = pos->z;
    y0 = 120.00001f + pos->y;
    x0 = pos->x;
    v.x = x0;
    v.y = y0;
    v.z = z0;
    fn_1_E94(&tmp, &v, 120.00001f);
    v.x = 0.0f;
    v.y = 0.0f;
    v.z = z;
    fn_1_B1C(&tmp, &v);
    fn_1_D0C(&tmp, &self->key1);
    self->frame = 0;
    self->hook = lbl_1_data_3D8;
}

static inline f32 Smooth(f32 t)
{
    return (3.0f - 2.0f * t) * (t * t);
}

// Eased blend from key0 to key1 over 90 frames.
void fn_1_B4A0(Camera568 *self)
{
    f32 t;

    t = (f32)self->frame / (f32)0x5A;
    if (t > 1.0f) {
        t = 1.0f;
    }
    fn_1_D84(&self->state, &self->key0, &self->key1, Smooth(t));
    fn_1_FB8(&self->state, 1);
}

void *fn_1_B59C(Camera568 *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_3E4;
        if (self != 0) {
            if (lbl_1_bss_A14 != 0) {
            } else {
                fn_1_7EC(lbl_1_data_3A4, lbl_1_data_398, 0x3D);
            }
            lbl_1_bss_A14 = 0;
        }
        if (self != 0) {
            self->vtbl = lbl_1_data_23C;
            if (self != 0) {
                fn_1_1888(self, 0);
            }
        }
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

}
