#include "REL/m537Dll.h"
#include <math.h>

#ifdef __MWERKS__
extern "C" void __construct_array(void *ptr, void *ctor, void *dtor, u32 size, u32 n);
extern "C" void __destroy_arr(void *block, void *dtor, u32 size, u32 n);
#else
// Portable stand-ins for the MWCC runtime helpers.
static void __construct_array(void *ptr, void *ctor, void *dtor, u32 size, u32 n)
{
    u8 *p = (u8 *)ptr;

    (void)dtor;
    while (n-- != 0) {
        ((void (*)(void *))ctor)(p);
        p += size;
    }
}

static void __destroy_arr(void *block, void *dtor, u32 size, u32 n)
{
    u8 *p = (u8 *)block + size * n;

    while (n-- != 0) {
        p -= size;
        ((void (*)(void *, s16))dtor)(p, -1);
    }
}
#endif

#ifdef TARGET_PC
#include <stdio.h>
#define M537_STDERR ((void *)stderr)
#else
extern "C" u8 __files[];
#define M537_STDERR ((void *)(__files + 0xA0))
#endif

extern "C" {

void abort(void);
void fn_1_9F88(Vec *pos);
M537Splash *fn_1_13294(M537Splash *self, Vec *pos);
M537Ripple *fn_1_12C80(M537Ripple *self, Vec *pos);
s16 *fn_1_2CDC(s16 *out, void *hookFunc);
void fn_1_30BC(s16 *modelId, void *hookData);

// Inlined helpers of the original (std::vector begin/end, Vec normalise, dot).
static inline M537Pt *M537PtBegin(M537VecPt *v)
{
    M537Pt **p = &v->m_data;
    M537Pt *r = *p;
    return r;
}

static inline M537Pt *M537PtEnd(M537VecPt *v)
{
    M537Pt **p = &v->m_data;
    M537Pt *r = *p + v->m_size;
    return r;
}

static inline void M537Lerp(Vec *out, Vec *a, Vec *b, f32 t)
{
    f32 bx = b->x;
    f32 ax = a->x;
    f32 r0 = (f32)((1.0 - (f64)t) * (f64)ax + (f64)t * (f64)bx);
    f32 r1 = r0;
    f32 by;
    f32 ay;
    f32 r2;
    f32 r3;
    f32 bz;
    f32 az;
    f32 r4;
    f32 r5;

    out->x = r1;
    by = b->y;
    ay = a->y;
    r2 = (f32)((1.0 - (f64)t) * (f64)ay + (f64)t * (f64)by);
    r3 = r2;
    out->y = r3;
    bz = b->z;
    az = a->z;
    r4 = (f32)((1.0 - (f64)t) * (f64)az + (f64)t * (f64)bz);
    r5 = r4;
    out->z = r5;
}

static inline void M537Normalize(Vec *v)
{
    PSVECNormalize(v, v);
}

M537Arm *fn_1_108CC(M537Arm *self, s32 index, Vec *target);
M537Arm *fn_1_10B20(M537Arm *self, s16 freeIt);
M537ArmPtr *fn_1_10120(M537ArmPtr *self, s16 freeIt);
void fn_1_10178(M537ArmPtr *self);
void fn_1_10860(M537Arm *arm);
void fn_1_10888(HU3DMODEL *model, Mtx *mtx);
void fn_1_10BF0(M537Arm *self, s32 index);
void fn_1_10DA8(M537Arm *arm);
void fn_1_110A4(M537Pt *pt, Vec *pos);
void fn_1_11108(M537Pt *pt);
void fn_1_11158(M537Pt *pt);
void fn_1_111C0(M537Arm *arm, Vec *target);
void fn_1_114E4(M537Arm *arm, Mtx mtx);
void fn_1_11524(M537Arm *arm, Mtx mtx);
void fn_1_116D0(Vec *cam, Vec *pts, Vec *out, f32 width);
void fn_1_11864(M537Arm *arm);
void fn_1_11FB4(M537ArmPtr *self, M537Arm *arm);
void fn_1_11FBC(M537_NODE *head, void (*fn)(M537Arm *));
void fn_1_1201C(M537VecPt *vec, u32 count, M537Pt *value);
void fn_1_1218C(M537VecV3 *vec, u32 count, Vec *value);
void *fn_1_122CC(M537VecPt *self, s16 freeIt);
void *fn_1_12358(M537VecV3 *self, s16 freeIt);
void fn_1_123E4(M537_NODE *first, M537_NODE *stop, void (*fn)(M537Arm *));
void fn_1_12444(M537VecPt *vec);
void fn_1_124A0(M537VecV3 *vec);

#ifdef __MWERKS__
#define M537_RODATA __declspec(section ".rodata")
#else
#define M537_RODATA
#endif

M537_RODATA const Vec lbl_1_rodata_528[4] = {
    { -250.0f, 1050.0f, -250.0f },
    { 250.0f, 1050.0f, -250.0f },
    { -250.0f, 1050.0f, 250.0f },
    { 250.0f, 1050.0f, 250.0f },
};
M537_RODATA const s32 lbl_1_rodata_57C[5] = { 0x5F000B, 0x5F000C, 0x5F000D, 0x5F000E, 0 };

M537Pt lbl_1_bss_A64;
Vec lbl_1_bss_A88[1];

#ifdef TARGET_PC
// Global list of ropes: the static init/exit code runs the constructor and destructor.
class M537ArmList : public M537_NODE {
public:
    M537ArmList() { fn_1_1264(this); }
    ~M537ArmList() { fn_1_1270(this, 0); }
};

M537ArmList lbl_1_bss_A5C;
#else
// The GC build runs the initialiser by hand: fn_1_124FC below is the .ctors entry.
u8 lbl_1_bss_A50[12];
M537_NODE lbl_1_bss_A5C;
void *fn_1_12554(void *self, s16 freeIt);
#endif

M537Center *fn_1_FF4C(M537Center *self)
{
    M537Arm *arm;
    s32 i;

    if (lbl_1_bss_A0C == 0) {
    } else {
        fn_1_1D4("ms_pointer == 0", "singleton.h", 0x35);
    }
    lbl_1_bss_A0C = self;
    fn_1_2C94(&self->m_mdl, 0x5F000A);
    self->m_flag = 0;
    __construct_array(self->m_arm, (void *)fn_1_10178, (void *)fn_1_10120, sizeof(M537ArmPtr), 4);
    fn_1_304C(&self->m_mdl, 5);
    fn_1_2E08(&self->m_mdl, 0x40000001);
    self->m_target.x = 0.0f;
    self->m_target.y = 1050.0f;
    self->m_target.z = 0.0f;
    self->m_pos.x = self->m_target.x;
    self->m_pos.y = self->m_target.y;
    self->m_pos.z = self->m_target.z;
    fn_1_2EF4(&self->m_mdl, &self->m_pos);
    self->m_vel.x = 0.0f;
    self->m_vel.y = 0.0f;
    self->m_vel.z = 0.0f;
    for (i = 0; i < 4; i++) {
#ifdef TARGET_PC
        arm = (M537Arm *)fn_1_0(sizeof(M537Arm));
#else
        arm = (M537Arm *)fn_1_0(0x34);
#endif
        if (arm != 0) {
            arm = fn_1_108CC(arm, i, (Vec *)&lbl_1_rodata_528[i]);
        }
        if (self->m_arm[i].m_p != arm) {
            fn_1_10B20(self->m_arm[i].m_p, 1);
            self->m_arm[i].m_p = arm;
        }
    }
    return self;
}

M537ArmPtr *fn_1_10120(M537ArmPtr *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_10B20(self->m_p, 1);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_10178(M537ArmPtr *self)
{
    fn_1_11FB4(self, 0);
}

M537Center *fn_1_101A4(M537Center *self, s16 freeIt)
{
    if (self != 0) {
        if (lbl_1_bss_A0C != 0) {
        } else {
            fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A0C = 0;
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M537Center *fn_1_10230(M537Center *self, s16 freeIt)
{
    if (self != 0) {
        __destroy_arr(self->m_arm, (void *)fn_1_10120, sizeof(M537ArmPtr), 4);
        fn_1_30F4(&self->m_mdl, 0);
        if (self != 0) {
            if (lbl_1_bss_A0C != 0) {
            } else {
                fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
            }
            lbl_1_bss_A0C = 0;
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

Vec *fn_1_102E8(M537Center *self)
{
    return &self->m_pos;
}

M537Arm *fn_1_102F0(M537Center *self, s32 index)
{
    return self->m_arm[index].m_p;
}

void fn_1_10300(M537Center *self)
{
    self->m_flag = 1;
}

// Moves the centre object towards its target and spawns the splash on landing.
void fn_1_1030C(M537Center *self)
{
    f32 oldY;
    f32 t0, t1, t2;
    f32 p0, p1, p2;
    f32 r0, r1, r2, r3, r4, r5;
    M537Splash *splash;
    M537Ripple *ripple;

    if (self->m_flag == 0) {
        fn_1_9F88(&self->m_target);
    } else {
        PSVECAdd(&self->m_vel, &self->m_target, &self->m_target);
        self->m_vel.y = self->m_vel.y - 0.27240694f;
    }
    oldY = self->m_pos.y;
    t0 = self->m_target.x;
    p0 = self->m_pos.x;
    r0 = (f32)(0.9 * (f64)p0 + 0.1 * (f64)t0);
    r1 = r0;
    self->m_pos.x = r1;
    t1 = self->m_target.y;
    p1 = self->m_pos.y;
    r2 = (f32)(0.9 * (f64)p1 + 0.1 * (f64)t1);
    r3 = r2;
    self->m_pos.y = r3;
    t2 = self->m_target.z;
    p2 = self->m_pos.z;
    r4 = (f32)(0.9 * (f64)p2 + 0.1 * (f64)t2);
    r5 = r4;
    self->m_pos.z = r5;
    fn_1_2EF4(&self->m_mdl, &self->m_pos);
    if (oldY > 0.0f) {
        if (self->m_pos.y <= 0.0f) {
            fn_1_304C(&self->m_mdl, 2);
#ifdef TARGET_PC
            splash = (M537Splash *)fn_1_0(sizeof(M537Splash));
#else
            splash = (M537Splash *)fn_1_0(0x14);
#endif
            if (splash != 0) {
                splash = fn_1_13294(splash, &self->m_pos);
            }
#ifdef TARGET_PC
            ripple = (M537Ripple *)fn_1_0(sizeof(M537Ripple));
#else
            ripple = (M537Ripple *)fn_1_0(0x1C);
#endif
            if (ripple != 0) {
                ripple = fn_1_12C80(ripple, &self->m_pos);
            }
        }
    }
}

// Same step as fn_1_1030C, expanded in place (the original compiler inlined it).
void fn_1_10590(void)
{
    M537Center *ms;
    f32 oldY;
    f32 t0, t1, t2;
    f32 p0, p1, p2;
    f32 r0, r1, r2, r3, r4, r5;
    M537Splash *splash;
    M537Ripple *ripple;

    if (lbl_1_bss_A0C != 0) {
    } else {
        fn_1_1D4("ms_pointer != 0", "singleton.h", 0x27);
    }
    ms = lbl_1_bss_A0C;
    if (ms->m_flag == 0) {
        fn_1_9F88(&ms->m_target);
    } else {
        PSVECAdd(&ms->m_vel, &ms->m_target, &ms->m_target);
        ms->m_vel.y = ms->m_vel.y - 0.27240694f;
    }
    oldY = ms->m_pos.y;
    t0 = ms->m_target.x;
    p0 = ms->m_pos.x;
    r0 = (f32)(0.9 * (f64)p0 + 0.1 * (f64)t0);
    r1 = r0;
    ms->m_pos.x = r1;
    t1 = ms->m_target.y;
    p1 = ms->m_pos.y;
    r2 = (f32)(0.9 * (f64)p1 + 0.1 * (f64)t1);
    r3 = r2;
    ms->m_pos.y = r3;
    t2 = ms->m_target.z;
    p2 = ms->m_pos.z;
    r4 = (f32)(0.9 * (f64)p2 + 0.1 * (f64)t2);
    r5 = r4;
    ms->m_pos.z = r5;
    fn_1_2EF4(&ms->m_mdl, &ms->m_pos);
    if (oldY > 0.0f) {
        if (ms->m_pos.y <= 0.0f) {
            fn_1_304C(&ms->m_mdl, 2);
#ifdef TARGET_PC
            splash = (M537Splash *)fn_1_0(sizeof(M537Splash));
#else
            splash = (M537Splash *)fn_1_0(0x14);
#endif
            if (splash != 0) {
                splash = fn_1_13294(splash, &ms->m_pos);
            }
#ifdef TARGET_PC
            ripple = (M537Ripple *)fn_1_0(sizeof(M537Ripple));
#else
            ripple = (M537Ripple *)fn_1_0(0x1C);
#endif
            if (ripple != 0) {
                ripple = fn_1_12C80(ripple, &ms->m_pos);
            }
        }
    }
    fn_1_11FBC(&lbl_1_bss_A5C, fn_1_10860);
}

void fn_1_10860(M537Arm *arm)
{
    fn_1_10DA8(arm);
}

void fn_1_10888(HU3DMODEL *model, Mtx *mtx)
{
    Mtx *m;

    m = mtx;
    fn_1_114E4((M537Arm *)model->hookData, *mtx);
}

M537Arm *fn_1_108CC(M537Arm *self, s32 index, Vec *target)
{
    M537Pt tmp;
    M537VecPt *vec;
    s32 idx;
    Vec *tg;

    idx = index;
    tg = target;
    fn_1_1264(self);
    fn_1_2CDC(&self->m_hook, (void *)fn_1_10888);
    self->m_count = 0x10;
    self->m_len = 24.0f;
    vec = &self->m_pts;
    vec->m_cap = 0;
    vec->m_size = 0;
    vec->m_data = 0;
    tmp = lbl_1_bss_A64;
    fn_1_1201C(vec, self->m_count, &tmp);
    self->m_flag = 0;
    fn_1_1330(self, &lbl_1_bss_A5C);
    fn_1_304C(&self->m_hook, 5);
    fn_1_30BC(&self->m_hook, self);
    fn_1_10BF0(self, idx);
    fn_1_111C0(self, tg);
    return self;
}

void *fn_1_10A10(M537VecPt *self, s16 freeIt)
{
    if (self != 0) {
        if (self != 0) {
            fn_1_122CC(self, 0);
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void *fn_1_10A70(M537VecPt *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_122CC(self, 0);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void *fn_1_10AC8(M537_NODE *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M537Arm *fn_1_10B20(M537Arm *self, s16 freeIt)
{
    if (self != 0) {
        if ((u8 *)self + 0x14 != 0 && (u8 *)self + 0x14 != 0) {
            fn_1_122CC(&self->m_pts, 0);
        }
        fn_1_30F4(&self->m_hook, 0);
        if (self != 0) {
            fn_1_1270(self, 0);
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_10BB0(M537Arm *self, Vec *target)
{
    Vec *tg;

    tg = target;
    fn_1_111C0(self, target);
    self->m_flag = 0;
}

void fn_1_10BF0(M537Arm *self, s32 index)
{
    s32 idx;

    idx = index;
    self->m_anim = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_rodata_57C[idx], 0x10000000, HEAP_MODEL));
}

// Bends the segment b->c so that it turns at most maxAng away from a->b.
void fn_1_10C54(Vec *a, Vec *b, Vec *c, f32 k, f32 maxAng)
{
    Mtx mtx;
    Vec d1;
    Vec d2;
    Vec axis;
    Vec *a0;
    f32 k0;
    f32 ang;

    a0 = a;
    k0 = k;
    PSVECSubtract(b, a0, &d1);
    PSVECNormalize(&d1, &d1);
    PSVECSubtract(c, b, &d2);
    PSVECNormalize(&d2, &d2);
    ang = (f32)acos(PSVECDotProduct(&d1, &d2));
    if (fabs(ang) > maxAng) {
        PSVECCrossProduct(&d1, &d2, &axis);
        PSMTXRotAxisRad(mtx, &axis, maxAng - ang);
        PSMTXMultVec(mtx, &d2, &d2);
    }
    PSVECScale(&d2, &d2, k0);
    PSVECAdd(b, &d2, c);
}

// Simulates one rope step: the head follows the centre object, the rest trails it.
void fn_1_10DA8(M537Arm *arm)
{
    Mtx mtx;
    Vec tgt;
    Vec d1;
    Vec d2;
    Vec axis;
    M537Center *ms0;
    M537Center *ms1;
    Vec *src;
    M537Pt *pt;
    f32 len;
    f32 ang;
    f32 dot;
    f32 dot2;

    if (arm->m_flag == 0) {
        arm->m_flag = 1;
        return;
    }
    if (lbl_1_bss_A0C != 0) {
    } else {
        fn_1_1D4("ms_pointer != 0", "singleton.h", 0x27);
    }
    ms0 = lbl_1_bss_A0C;
    ms1 = ms0;
    src = &ms1->m_pos;
    tgt = *src;
    fn_1_110A4(M537PtBegin(&arm->m_pts), &tgt);
    PSVECAdd(&arm->m_dir, &tgt, &tgt);
    fn_1_110A4(M537PtBegin(&arm->m_pts) + 1, &tgt);
    for (pt = M537PtBegin(&arm->m_pts) + 2; pt != M537PtEnd(&arm->m_pts); pt++) {
        fn_1_11108(pt);
    }
    for (pt = M537PtBegin(&arm->m_pts); pt != M537PtEnd(&arm->m_pts) - 2; pt++) {
        len = arm->m_len;
        PSVECSubtract(&pt[1].m_pos, &pt[0].m_pos, &d1);
        M537Normalize(&d1);
        PSVECSubtract(&pt[2].m_pos, &pt[1].m_pos, &d2);
        M537Normalize(&d2);
        dot = PSVECDotProduct(&d1, &d2);
        dot2 = dot;
        ang = (f32)acos(dot2);
        if (fabs(ang) > 0.5235987901687622) {
            PSVECCrossProduct(&d1, &d2, &axis);
            PSMTXRotAxisRad(mtx, &axis, 0.5235988f - ang);
            PSMTXMultVec(mtx, &d2, &d2);
        }
        PSVECScale(&d2, &d2, len);
        PSVECAdd(&pt[1].m_pos, &d2, &pt[2].m_pos);
    }
    for (pt = M537PtBegin(&arm->m_pts) + 2; pt != M537PtEnd(&arm->m_pts); pt++) {
        fn_1_11158(pt);
    }
}

void fn_1_110A4(M537Pt *pt, Vec *pos)
{
    pt->m_pos.x = pos->x;
    pt->m_pos.y = pos->y;
    pt->m_pos.z = pos->z;
    pt->m_prev.x = pos->x;
    pt->m_prev.y = pos->y;
    pt->m_prev.z = pos->z;
    pt->m_vel.x = 0.0f;
    pt->m_vel.y = 0.0f;
    pt->m_vel.z = 0.0f;
}

void fn_1_11108(M537Pt *pt)
{
    PSVECAdd(&pt->m_vel, &pt->m_pos, &pt->m_pos);
    pt->m_pos.y -= 0.8172208f;
}

void fn_1_11158(M537Pt *pt)
{
    PSVECSubtract(&pt->m_pos, &pt->m_prev, &pt->m_vel);
    PSVECScale(&pt->m_vel, &pt->m_vel, 0.8f);
    pt->m_prev.x = pt->m_pos.x;
    pt->m_prev.y = pt->m_pos.y;
    pt->m_prev.z = pt->m_pos.z;
}

// Lays the rope out in a straight line from the centre object to the target.
void fn_1_111C0(M537Arm *arm, Vec *target)
{
    Vec start;
    Vec p;
    M537Center *ms0;
    M537Center *ms1;
    M537Center *ms2;
    Vec *src;
    Vec *dir0;
    Vec *dir1;
    M537Pt *pt;
    f32 t;
    s32 i;

    if (lbl_1_bss_A0C != 0) {
    } else {
        fn_1_1D4("ms_pointer != 0", "singleton.h", 0x27);
    }
    ms0 = lbl_1_bss_A0C;
    ms1 = ms0;
    ms2 = ms1;
    src = &ms2->m_pos;
    start = *src;
    PSVECSubtract(target, &start, &arm->m_dir);
    PSVECNormalize(&arm->m_dir, &arm->m_dir);
    dir0 = &arm->m_dir;
    dir1 = dir0;
    PSVECScale(dir1, dir1, 30.0f);
    for (i = 0; i < (s32)arm->m_count; i++) {
        t = (f32)i / (f32)(arm->m_count - 1);
        M537Lerp(&p, &start, target, t);
        pt = M537PtBegin(&arm->m_pts) + i;
        fn_1_110A4(pt, &p);
    }
}

void fn_1_114E4(M537Arm *arm, Mtx mtx)
{
    Mtx *m;

    m = (Mtx *)mtx;
    fn_1_11524(arm, mtx);
    fn_1_11864(arm);
}

// Sets up the texture and TEV state used to draw a rope ribbon.
void fn_1_11524(M537Arm *arm, Mtx mtx)
{
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetNumChans(0);
    HuSprTexLoad(arm->m_anim, 0, GX_TEXMAP0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetCullMode(GX_CULL_NONE);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZCompLoc(GX_FALSE);
    GXSetZMode(GX_TRUE, GX_LESS, GX_TRUE);
    GXSetAlphaCompare(GX_GEQUAL, 0xC0, GX_AOP_AND, GX_LEQUAL, 0xFF);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
}

// Computes the two ribbon edge points around pts[1] facing the camera.
void fn_1_116D0(Vec *cam, Vec *pts, Vec *out, f32 width)
{
    Vec a;
    Vec b;
    Vec *cam0;
    f32 w;

    cam0 = cam;
    w = width;
    PSVECSubtract(&pts[1], &pts[0], &a);
    PSVECSubtract(&pts[2], &pts[1], &b);
    if (PSVECSquareMag(&a) <= 0.0f) {
        a = b;
    } else {
        if (PSVECSquareMag(&b) <= 0.0f) {
            b = a;
        }
    }
    PSVECNormalize(&a, &a);
    PSVECNormalize(&b, &b);
    PSVECAdd(&b, &a, &a);
    PSVECSubtract(&pts[1], cam0, &b);
    PSVECCrossProduct(&b, &a, &a);
    PSVECNormalize(&a, &a);
    PSVECScale(&a, &a, w);
    PSVECAdd(&pts[1], &a, &out[0]);
    PSVECSubtract(&pts[1], &a, &out[1]);
}

// Builds the ribbon for a rope and draws it as a strip of quads.
void fn_1_11864(M537Arm *arm)
{
    M537VecV3 verts;
    Vec zero;
    Vec cam;
    Vec up;
    Vec target;
    Vec p3[3];
    s32 i;
    s32 i0;
    s32 i1;
    s32 i2;
    s32 last;
    f32 v0;
    f32 v1;
    Vec *q0;
    Vec *q1;
    Vec *q2;
    Vec *q3;

    verts.m_cap = 0;
    verts.m_size = 0;
    verts.m_data = 0;
    zero = lbl_1_bss_A88[0];
    fn_1_1218C(&verts, arm->m_count * 2, &zero);
    Hu3DCameraPosGet(1, &cam, &up, &target);
    for (i = 0; i < (s32)arm->m_count; i++) {
        last = arm->m_count - 1;
        if (i - 1 < 0) {
            i0 = 0;
        } else if (i - 1 > last) {
            i0 = last;
        } else {
            i0 = i - 1;
        }
        last = arm->m_count - 1;
        if (i < 0) {
            i1 = 0;
        } else if (i > last) {
            i1 = last;
        } else {
            i1 = i;
        }
        last = arm->m_count - 1;
        if (i + 1 < 0) {
            i2 = 0;
        } else if (i + 1 > last) {
            i2 = last;
        } else {
            i2 = i + 1;
        }
        p3[0].x = 0.0f;
        p3[0].y = 0.0f;
        p3[0].z = 0.0f;
        p3[1].x = 0.0f;
        p3[1].y = 0.0f;
        p3[1].z = 0.0f;
        p3[2].x = 0.0f;
        p3[2].y = 0.0f;
        p3[2].z = 0.0f;
        p3[0] = arm->m_pts.m_data[i0].m_pos;
        p3[1] = arm->m_pts.m_data[i1].m_pos;
        p3[2] = arm->m_pts.m_data[i2].m_pos;
        fn_1_116D0(&cam, p3, verts.m_data + i * 2, 9.0f);
    }
    for (i = 0; i < (s32)(arm->m_count - 1); i++) {
        q0 = verts.m_data + i * 2;
        q1 = verts.m_data + (i * 2 + 1);
        q2 = verts.m_data + (i * 2 + 2);
        q3 = verts.m_data + (i * 2 + 3);
        if (i == (s32)(arm->m_count - 2)) {
            v0 = (f32)0.6;
        } else {
            v0 = (f32)0.2;
        }
        v1 = (f32)(0.4 + (f64)v0);
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
        GXPosition3f32(q0->x, q0->y, q0->z);
        GXTexCoord2f32(v0, 0.0f);
        GXPosition3f32(q1->x, q1->y, q1->z);
        GXTexCoord2f32(v0, 1.0f);
        GXPosition3f32(q2->x, q2->y, q2->z);
        GXTexCoord2f32(v1, 0.0f);
        GXPosition3f32(q3->x, q3->y, q3->z);
        GXTexCoord2f32(v1, 1.0f);
#ifdef TARGET_PC
        GXEnd();
#endif
    }
    fn_1_12358(&verts, 0);
}

void *fn_1_11EFC(M537VecV3 *self, s16 freeIt)
{
    if (self != 0) {
        if (self != 0) {
            fn_1_12358(self, 0);
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void *fn_1_11F5C(M537VecV3 *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_12358(self, 0);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_11FB4(M537ArmPtr *self, M537Arm *arm)
{
    self->m_p = arm;
}

void fn_1_11FBC(M537_NODE *head, void (*fn)(M537Arm *))
{
    M537_NODE *h;
    M537_NODE *f0;
    M537_NODE *f1;

    h = head;
    f0 = head->next;
    f1 = f0;
    fn_1_123E4(f1, h, fn);
}

// std::vector<M537Pt>(count, value)
void fn_1_1201C(M537VecPt *vec, u32 count, M537Pt *value)
{
    M537VecPt *v0;
    M537VecPt *v1;
    M537VecPt *v2;
    M537Pt **pData0;
    M537Pt **pData1;
    M537Pt **pData2;
    M537Pt *mem;
    M537Pt *mem2;
    M537Pt *end;
    M537Pt *sp10;
    M537Pt *cur;
    M537Pt *spC;
    M537Pt *sp14;
    M537Pt *val;
    u32 lim0;
    u32 lim1;
    u32 lim2;

    val = value;
    if (count != 0) {
        v0 = vec;
        lim0 = 0x071C71C7;
        lim1 = lim0;
        lim2 = lim1;
        if (count > lim2) {
            fn_1_234(M537_STDERR, "vector length error\n");
            abort();
        }
        pData0 = &vec->m_data;
        v1 = vec;
        mem = (M537Pt *)fn_1_0(count * sizeof(M537Pt));
        if (mem == 0) {
            fn_1_234(M537_STDERR, "Memory allocation failure");
            abort();
        }
        mem2 = mem;
        *pData0 = mem2;
        v2 = vec;
        v2->m_cap = count;
        pData1 = &vec->m_data;
        end = *pData1 + count;
        pData2 = &vec->m_data;
        sp10 = *pData2;
        cur = sp10;
        while (cur < end) {
            spC = cur;
            sp14 = spC;
            if (spC != 0) {
                *sp14 = *val;
            }
            cur++;
            vec->m_size = vec->m_size + 1;
        }
    }
}

// std::vector<Vec>(count, value)
void fn_1_1218C(M537VecV3 *vec, u32 count, Vec *value)
{
    M537VecV3 *v0;
    M537VecV3 *v1;
    M537VecV3 *v2;
    Vec **pData0;
    Vec **pData1;
    Vec **pData2;
    Vec *mem;
    Vec *mem2;
    Vec *end;
    Vec *sp10;
    Vec *cur;
    Vec *spC;
    Vec *sp14;
    Vec *val;
    u32 lim0;
    u32 lim1;
    u32 lim2;

    val = value;
    if (count != 0) {
        v0 = vec;
        lim0 = 0x15555555;
        lim1 = lim0;
        lim2 = lim1;
        if (count > lim2) {
            fn_1_234(M537_STDERR, "vector length error\n");
            abort();
        }
        pData0 = &vec->m_data;
        v1 = vec;
        mem = (Vec *)fn_1_0(count * sizeof(Vec));
        if (mem == 0) {
            fn_1_234(M537_STDERR, "Memory allocation failure");
            abort();
        }
        mem2 = mem;
        *pData0 = mem2;
        v2 = vec;
        v2->m_cap = count;
        pData1 = &vec->m_data;
        end = *pData1 + count;
        pData2 = &vec->m_data;
        sp10 = *pData2;
        cur = sp10;
        while (cur < end) {
            spC = cur;
            sp14 = spC;
            if (spC != 0) {
                *sp14 = *val;
            }
            cur++;
            vec->m_size = vec->m_size + 1;
        }
    }
}

void *fn_1_122CC(M537VecPt *self, s16 freeIt)
{
    M537Pt **p0;
    M537Pt **p1;
    M537Pt *d;

    if (self != 0) {
        fn_1_12444(self);
        p0 = &self->m_data;
        if (*p0 != 0) {
            p1 = &self->m_data;
            d = *p1;
            fn_1_C8(d);
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void *fn_1_12358(M537VecV3 *self, s16 freeIt)
{
    Vec **p0;
    Vec **p1;
    Vec *d;

    if (self != 0) {
        fn_1_124A0(self);
        p0 = &self->m_data;
        if (*p0 != 0) {
            p1 = &self->m_data;
            d = *p1;
            fn_1_C8(d);
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_123E4(M537_NODE *first, M537_NODE *stop, void (*fn)(M537Arm *))
{
    M537_NODE *node;
    M537_NODE *next;

    node = first;
    while (node != stop) {
        fn((M537Arm *)node);
        next = node->next;
        node = next;
    }
}

void fn_1_12444(M537VecPt *vec)
{
    M537Pt *p;
    M537Pt *first;
    M537Pt *q;
    M537Pt **pd;

    pd = &vec->m_data;
    first = *pd;
    p = first + vec->m_size;
    while (p > first) {
        p--;
        q = p;
    }
    vec->m_size = 0;
}

void fn_1_124A0(M537VecV3 *vec)
{
    Vec *p;
    Vec *first;
    Vec *q;
    Vec **pd;

    pd = &vec->m_data;
    first = *pd;
    p = first + vec->m_size;
    while (p > first) {
        p--;
        q = p;
    }
    vec->m_size = 0;
}


#ifndef TARGET_PC
void fn_1_124FC(void)
{
    M537_NODE *obj0;
    M537_NODE *obj1;

    obj0 = &lbl_1_bss_A5C;
    fn_1_1264(obj0);
    obj1 = obj0;
    __register_global_object(obj1, (void *)fn_1_12554, lbl_1_bss_A50);
}

void *fn_1_12554(void *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}
#endif

}
