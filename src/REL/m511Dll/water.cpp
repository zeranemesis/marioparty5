#include <math.h>

#include <dolphin.h>
#include <dolphin/mtx.h>

#include "datadir_enum.h"
#include "game/data.h"
#include "game/hu3d.h"
#include "game/sprite.h"

#ifdef TARGET_PC
#define WT_VTX3(x, y, z) GXPosition3f32(x, y, z)
#define WT_TEX2(s, t) GXTexCoord2f32(s, t)
#define WT_CLR4(r, g, b, a) GXColor4u8(r, g, b, a)
#define WT_END() GXEnd()
#else
#define WT_VTX3(x, y, z) \
    do { \
        GXWGFifo.f32 = (x); \
        GXWGFifo.f32 = (y); \
        GXWGFifo.f32 = (z); \
    } while (0)
#define WT_TEX2(s, t) \
    do { \
        GXWGFifo.f32 = (s); \
        GXWGFifo.f32 = (t); \
    } while (0)
#define WT_CLR4(r, g, b, a) \
    do { \
        GXWGFifo.u8 = (r); \
        GXWGFifo.u8 = (g); \
        GXWGFifo.u8 = (b); \
        GXWGFifo.u8 = (a); \
    } while (0)
#define WT_END() ((void)0)
#endif

extern "C" {

void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);
void *__dl__FPv(void *self);
void fn_1_1D88(void *self);
void fn_1_1D94(void *self, s32 arg1);
void fn_1_1E54(void *self, void **arg1);
void fn_1_CF8(char *cond, char *file, s32 line);
u16 fn_1_5578(void);

extern void *lbl_1_data_40[];
extern "C" void *lbl_1_bss_2C[2];

/* Lattice size of the water surface. */
#define WATER_N 16

/* Layout common to the three water objects: list node, vtable, flags. */
struct Mgr511Water;

extern Mgr511Water *lbl_1_bss_1C;

/* Main water surface: 16x16 height field. */
struct Mgr511Water {
    void *next;
    void *prev;
    void **vtable;
    u32 flags;
    ANIMDATA *anim1;
    ANIMDATA *anim2;
    s16 modelId;
    f32 time;
    s32 counter;
    s32 unk24;
    Vec vtx[WATER_N * WATER_N];
    Vec tex[WATER_N * WATER_N];
    f32 vel[WATER_N * WATER_N];
    f32 height[WATER_N * WATER_N];
    GXColor col[WATER_N * WATER_N];
    Vec dir;
};

/* Ring of foam patches around the boat. */
struct Mgr511WaterRing {
    void *next;
    void *prev;
    void **vtable;
    u32 flags;
    ANIMDATA *anim1;
    ANIMDATA *anim2;
    s16 modelId;
    f32 time;
    Vec dir;
    Vec pts[80];
};

/* Ripple that follows an object on the water surface. */
struct Mgr511WaterRipple {
    void *next;
    void *prev;
    void **vtable;
    u32 flags;
    ANIMDATA *anim1;
    ANIMDATA *anim2;
    s16 modelId;
    f32 time;
    Vec dir;
    Vec pts[32];
    Vec pos;
};

extern void *lbl_1_data_6F4[5];
extern void *lbl_1_data_708[5];
extern void *lbl_1_data_71C[5];

/* Indirect texture matrices (2x3 each) used by the ring and the ripple. */
static const f32 lbl_1_rodata_AB8[2][3] = { { 0.1f, 0.0f, 0.0f }, { 0.0f, 0.1f, 0.0f } };
static const f32 lbl_1_rodata_AE8[2][3] = { { 0.1f, 0.0f, 0.0f }, { 0.0f, 0.1f, 0.0f } };

void fn_1_19B58(Mgr511Water *arg0);
void fn_1_19D18(Mgr511Water *arg0);
void fn_1_19FC8(Mgr511Water *arg0, Vec *arg1, f32 arg2);
f32 fn_1_1A150(Mgr511Water *arg0, Vec *arg1);
void fn_1_1A8C0(Mgr511Water *arg0, Mtx mtx);
void fn_1_1A9EC(Mgr511Water *arg0);
void fn_1_1B0CC(Mgr511Water *arg0, Mtx mtx);
void fn_1_1BC84(Mgr511WaterRing *arg0, Mtx mtx);
void fn_1_1BD9C(Mgr511WaterRing *arg0);
void fn_1_1C4F4(Mgr511WaterRipple *arg0);
void fn_1_1C9EC(Mgr511WaterRipple *arg0, Mtx mtx);

static inline void WaterVert(Vec v, GXColor c, Vec t)
{
    WT_VTX3(v.x, v.y, v.z);
    WT_CLR4(c.r, c.g, c.b, c.a);
    WT_TEX2(t.x, t.z);
}

static inline void RingVert(f32 x, f32 y, f32 z, f32 s, f32 t)
{
    WT_VTX3(x, y, z);
    WT_TEX2(s, t);
}

void fn_1_19804(HU3DMODEL *p, Mtx *mtx)
{
    Mgr511Water *o = (Mgr511Water *)p->hookData;

    fn_1_1A8C0(o, *mtx);
}

Mgr511Water *fn_1_19848(Mgr511Water *arg0)
{
    HU3DMODELID id;

    fn_1_1D88(arg0);
    arg0->vtable = lbl_1_data_40;
    arg0->flags = 0;
    fn_1_1E54(arg0, lbl_1_bss_2C);
    if (lbl_1_bss_1C != NULL) {
        fn_1_CF8("ms_pointer == 0", "singleton.h", 0x35);
    }
    lbl_1_bss_1C = arg0;
    arg0->vtable = &lbl_1_data_71C[0];
    arg0->time = 0.0f;
    arg0->counter = 0;
    id = Hu3DHookFuncCreate(fn_1_19804);
    Hu3DData[id].hookData = arg0;
    arg0->modelId = id;
    Hu3DModelLayerSet(id, 3);
    Hu3DModelCameraSet(id, 0xF);
    arg0->anim1 = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m511, 0x21), HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(arg0->anim1);
    arg0->anim2 = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m511, 0x2B), HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(arg0->anim2);
    fn_1_19B58(arg0);
    arg0->dir.x = 0.0f;
    arg0->dir.y = 0.0f;
    arg0->dir.z = 0.0f;
    return arg0;
}

Mgr511Water *fn_1_199E8(Mgr511Water *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        if (lbl_1_bss_1C == NULL) {
            fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_1C = (Mgr511Water *)NULL;
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

Mgr511Water *fn_1_19A74(Mgr511Water *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        arg0->vtable = &lbl_1_data_71C[0];
        Hu3DModelKill(arg0->modelId);
        HuSprAnimKill(arg0->anim1);
        HuSprAnimKill(arg0->anim2);
        if (lbl_1_bss_1C == NULL) {
            fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_1C = (Mgr511Water *)NULL;
        arg0->vtable = lbl_1_data_40;
        fn_1_1D94(arg0, 0);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_19B58(Mgr511Water *arg0)
{
    s32 i;
    s32 j;

    for (i = 0; i < WATER_N; i++) {
        for (j = 0; j < WATER_N; j++) {
            s32 idx = j + (i * WATER_N);
            Vec *v = &arg0->vtx[idx];
            Vec *t = &arg0->tex[idx];
            GXColor *c = &arg0->col[idx];

            v->x = (1600.0f * (f32)j) - 12800.0f;
            v->y = 0.0f;
            v->z = (1600.0f * (f32)i) - 12800.0f;
            t->x = 0.0625f * (f32)j;
            t->z = 0.0625f * (f32)i;
            c->b = 0;
            c->g = 0;
            c->r = 0;
            c->a = 0xB4;
            arg0->vel[idx] = 0.0f;
        }
    }
}

void fn_1_19D18(Mgr511Water *arg0)
{
    s8 shade;
    s32 i;
    s32 j;
    f32 *src = &arg0->height[0];
    f32 *dst;

    dst = src;
    for (i = 0; i < WATER_N; i++) {
        for (j = 0; j < WATER_N; j++) {
            *dst = arg0->vtx[i * WATER_N + j].y;
            dst++;
        }
    }
    for (i = 3; i < 13; i++) {
        for (j = 3; j < 13; j++) {
            f32 *h = &arg0->height[i * WATER_N + j];
            s32 idx = j + (i * WATER_N);
            f32 *vel = &arg0->vel[idx];
            Vec *v = &arg0->vtx[idx];
            GXColor *c = &arg0->col[idx];

            *vel = (f32)((f64)*vel + (0.008333333333333333 * (f64)((0.0f + (0.04f * h[1]) + (0.04f * h[-1]) + (0.04f * h[WATER_N]) + (0.04f * h[-WATER_N])) - (4.0f * (0.04f * h[0])))));
            v->y = v->y + *vel;
            if (v->y > 100.0f) {
                v->y = 100.0f;
                *vel = 0.0f;
            }
            if (v->y < -100.0f) {
                v->y = -100.0f;
                *vel = 0.0f;
            }
            shade = (s8)(180.0f * (0.005f * (100.0f + v->y)));
            c->b = (u8)shade;
            c->g = (u8)shade;
            c->r = (u8)shade;
        }
    }
}

void fn_1_19FC8(Mgr511Water *arg0, Vec *arg1, f32 arg2)
{
    s32 cx;
    s32 cz;
    s32 x;
    s32 z;
    s32 dx;
    s32 dz;

    x = (s32)(8.0f + (f32)(s32)(0.000625f * arg1->x));
    z = (s32)(8.0f + (f32)(s32)(0.000625f * arg1->z));
    if (x < 1) {
        cx = 1;
    } else if (x > 0xE) {
        cx = 0xE;
    } else {
        cx = x;
    }
    if (z < 1) {
        cz = 1;
    } else if (z > 0xE) {
        cz = 0xE;
    } else {
        cz = z;
    }
    for (dz = -1; dz <= 1; dz++) {
        for (dx = -1; dx <= 1; dx++) {
            arg0->vel[cx + (dx + ((cz + dz) * WATER_N))] += arg2;
        }
    }
}

f32 fn_1_1A150(Mgr511Water *arg0, Vec *arg1)
{
    Vec p;
    s32 cx;
    s32 cz;
    s32 x;
    s32 z;
    s32 i0;
    s32 i1;
    Vec *v = &arg0->vtx[0];

    x = (s32)(0.000625f * (12800.0f + arg1->x));
    z = (s32)(0.000625f * (12800.0f + arg1->z));
    if (x < 1) {
        cx = 1;
    } else if (x > 0xE) {
        cx = 0xE;
    } else {
        cx = x;
    }
    if (z < 1) {
        cz = 1;
    } else if (z > 0xE) {
        cz = 0xE;
    } else {
        cz = z;
    }
    i0 = cx + (cz * WATER_N);
    i1 = cx + ((cz + 1) * WATER_N);
    p.x = arg1->x;
    p.y = arg1->y;
    p.z = arg1->z;
    p.x -= v[i0].x;
    p.z -= v[i0].z;
    p.x = p.x * 0.000625f;
    p.z = p.z * 0.000625f;
    return ((1.0f - p.z) * ((v[i0].y * (1.0f - p.x)) + (v[i0 + 1].y * p.x))) + (p.z * ((v[i1].y * (1.0f - p.x)) + (v[i1 + 1].y * p.x)));
}

void fn_1_1A394(Vec *arg0, Mgr511Water *arg1, Vec *arg2)
{
    Vec n;
    Vec a;
    Vec b;
    s32 cx;
    s32 cz;
    s32 x;
    s32 z;
    s32 i0;
    s32 i1;
    Vec *v = &arg1->vtx[0];

    x = (s32)(0.000625f * (12800.0f + arg2->x));
    z = (s32)(0.000625f * (12800.0f + arg2->z));
    if (x < 1) {
        cx = 1;
    } else if (x > 0xE) {
        cx = 0xE;
    } else {
        cx = x;
    }
    if (z < 1) {
        cz = 1;
    } else if (z > 0xE) {
        cz = 0xE;
    } else {
        cz = z;
    }
    i0 = cx + (cz * WATER_N);
    i1 = cx + ((cz + 1) * WATER_N);
    PSVECSubtract(&v[i0], &v[i0 + 1], &a);
    PSVECSubtract(&v[i0], &v[i1], &b);
    PSVECCrossProduct(&b, &a, &n);
    PSVECNormalize(&n, &n);
    arg0->x = n.x;
    arg0->y = n.y;
    arg0->z = n.z;
}

void fn_1_1A518(Mgr511Water *p, s32 bit)
{
    p->flags &= ~((u32)1 << bit);
}

void fn_1_1A530(Mgr511Water *arg0)
{
    Vec p;
    f32 ang;
    f32 ang2;
    f64 sinA;
    f64 sinB;
    f32 r;

    arg0->time = (f32)((f64)arg0->time + 0.016666666666666666);
    fn_1_19D18(arg0);
    ang = 0.017453292f * (10.0f * arg0->time);
    ang2 = 0.017453292f * (20.0f * arg0->time);
    sinB = sin(ang2);
    sinA = sin(ang);
    arg0->dir.x = (f32)sinA;
    arg0->dir.y = 0.0f;
    arg0->dir.z = (f32)sinB;
    if ((arg0->counter & 0x3F) == 0) {
        r = 95.0f * (0.000030517578f * ((f32)fn_1_5578() - 32768.0f));
        p.x = 100.0f * r;
        p.y = 0.0f;
        p.z = 100.0f * (95.0f * (0.000030517578f * ((f32)fn_1_5578() - 32768.0f)));
        fn_1_19FC8(arg0, &p, -0.5f);
    }
    arg0->counter = arg0->counter + 1;
}

void fn_1_1A8C0(Mgr511Water *arg0, Mtx mtx)
{
    GXLoadPosMtxImm(mtx, 0);
    HuSprTexLoad(arg0->anim1, 0, 0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    HuSprTexLoad(arg0->anim2, 0, 1, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    GXSetZMode(1, GX_LEQUAL, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZCompLoc(1);
    GXSetAlphaUpdate(1);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetCullMode(GX_CULL_BACK);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
    fn_1_1A9EC(arg0);
    fn_1_1B0CC(arg0, mtx);
}

void fn_1_1A9EC(Mgr511Water *arg0)
{
    Mtx trans;
    Mtx scale;
    Mtx tex;
    s32 y;
    s32 x;

    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_RASC, GX_CC_TEXC, GX_CC_RASA, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_RASA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    PSMTXTrans(trans, 0.2f * arg0->dir.x, 0.0f, 0.2f * arg0->dir.z);
    PSMTXScale(scale, 10.0f, 10.0f, 1.0f);
    PSMTXConcat(scale, trans, tex);
    GXLoadTexMtxImm(tex, 0x1E, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1E, 0, 0x7D);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 0x384);
    for (y = 0; y < 15; y++) {
        s32 row0 = y * WATER_N;
        s32 row1 = (y + 1) * WATER_N;

        for (x = 0; x < 15; x++) {
            s32 i0 = row0 + x;
            s32 i1 = row0 + (x + 1);
            s32 i2 = row1 + (x + 1);
            s32 i3 = row1 + x;
            WaterVert(arg0->vtx[i0], arg0->col[i0], arg0->tex[i0]);
            WaterVert(arg0->vtx[i1], arg0->col[i1], arg0->tex[i1]);
            WaterVert(arg0->vtx[i2], arg0->col[i2], arg0->tex[i2]);
            WaterVert(arg0->vtx[i3], arg0->col[i3], arg0->tex[i3]);
        }
    }
    WT_END();
}

void fn_1_1B0CC(Mgr511Water *arg0, Mtx mtx)
{
    Vec d;
    Mtx trans;
    Mtx rot;
    f32 ang;
    f32 absAng;
    HU3DCAMERA *cam;
    GXColor color = { 0xFF, 0xFF, 0xFF, 0x00 };

    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
    cam = &Hu3DCamera[Hu3DCameraNo];
    PSVECSubtract(&cam->pos, &cam->target, &d);
    ang = (f32)(180.0 * (atan2(d.x, d.z) / 3.141592653589793));
    PSMTXTrans(trans, cam->pos.x, 0.0f, cam->pos.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * ang);
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(mtx, trans, trans);
    GXLoadPosMtxImm(trans, 0);
    if (ang < 0.0f) {
        absAng = -ang;
    } else {
        absAng = ang;
    }
    color.a = (u8)(160.0f * (absAng / 180.0f));
    GXSetNumTevStages(1);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    WT_VTX3(-4000.0f, 0.0f, -8000.0f);
    WT_TEX2(0.0f, 0.0f);
    WT_VTX3(4000.0f, 0.0f, -8000.0f);
    WT_TEX2(1.0f, 0.0f);
    WT_VTX3(4000.0f, 0.0f, -1000.0f);
    WT_TEX2(1.0f, 1.0f);
    WT_VTX3(-4000.0f, 0.0f, -1000.0f);
    WT_TEX2(0.0f, 1.0f);
    WT_END();
}

void fn_1_1B534(HU3DMODEL *p, Mtx *mtx)
{
    Mgr511WaterRing *o = (Mgr511WaterRing *)p->hookData;

    fn_1_1BC84(o, *mtx);
}

Mgr511WaterRing *fn_1_1B578(Mgr511WaterRing *arg0)
{
    HU3DMODELID id;
    Vec *p;
    s32 i;
    f32 ang;
    f32 r;
    f64 z;

    fn_1_1D88(arg0);
    arg0->vtable = lbl_1_data_40;
    arg0->flags = 0;
    fn_1_1E54(arg0, lbl_1_bss_2C);
    arg0->vtable = &lbl_1_data_708[0];
    arg0->time = 0.0f;
    id = Hu3DHookFuncCreate(fn_1_1B534);
    Hu3DData[id].hookData = arg0;
    arg0->modelId = id;
    Hu3DModelLayerSet(id, 1);
    Hu3DModelCameraSet(id, 0xF);
    arg0->anim1 = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m511, 0x22), HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(arg0->anim1);
    arg0->anim2 = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m511, 0x2F), HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(arg0->anim2);
    p = &arg0->pts[0];
    for (i = 0; i < 30; i++) {
        ang = (f32)((3.141592653589793 * (f64)(30.0f * (f32)i)) / 180.0);
        r = 100.0f * (15.0f + (10.0f * (f32)(i % 5)));
        z = (f64)r * cos(ang);
        p->x = (f32)((f64)r * sin(ang));
        p->y = -350.0f;
        p->z = (f32)z;
        p++;
    }
    for (i = 30; i < 60; i++) {
        ang = (f32)((3.141592653589793 * (f64)(30.0f * (f32)i)) / 180.0);
        r = 100.0f * (15.0f + (10.0f * (f32)(i % 5)));
        z = ((f64)r * cos(ang)) - 5000.0;
        p->x = (f32)((f64)r * sin(ang));
        p->y = -350.0f;
        p->z = (f32)z;
        p++;
    }
    for (i = 60; i < 80; i++) {
        ang = (f32)((3.141592653589793 * (f64)(30.0f * (f32)i)) / 180.0);
        r = 100.0f * (15.0f + (10.0f * (f32)(i % 5)));
        z = 5000.0 + ((f64)r * cos(ang));
        p->x = (f32)((f64)r * sin(ang));
        p->y = -350.0f;
        p->z = (f32)z;
        p++;
    }
    arg0->dir.x = 0.0f;
    arg0->dir.y = 0.0f;
    arg0->dir.z = 0.0f;
    return arg0;
}

Mgr511WaterRing *fn_1_1BAD0(Mgr511WaterRing *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        arg0->vtable = &lbl_1_data_708[0];
        HuSprAnimKill(arg0->anim1);
        HuSprAnimKill(arg0->anim2);
        Hu3DModelKill(arg0->modelId);
        arg0->vtable = lbl_1_data_40;
        fn_1_1D94(arg0, 0);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_1BB68(Mgr511WaterRing *p, s32 bit)
{
    p->flags &= ~((u32)1 << bit);
}

void fn_1_1BB80(Mgr511WaterRing *arg0)
{
    f32 ang;
    f32 ang2;
    f64 sinA;
    f64 sinB;

    arg0->time = (f32)((f64)arg0->time + 0.016666666666666666);
    ang = 0.017453292f * (10.0f * arg0->time);
    ang2 = 0.017453292f * (20.0f * arg0->time);
    sinB = sin(ang2);
    sinA = sin(ang);
    arg0->dir.x = (f32)sinA;
    arg0->dir.y = 0.0f;
    arg0->dir.z = (f32)sinB;
}

void fn_1_1BC84(Mgr511WaterRing *arg0, Mtx mtx)
{
    GXLoadPosMtxImm(mtx, 0);
    HuSprTexLoad(arg0->anim1, 0, 0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    HuSprTexLoad(arg0->anim2, 0, 1, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    GXSetZMode(1, GX_LEQUAL, 1);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZCompLoc(1);
    GXSetAlphaUpdate(1);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetCullMode(GX_CULL_BACK);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
    fn_1_1BD9C(arg0);
}

void fn_1_1BD9C(Mgr511WaterRing *arg0)
{
    Vec d;
    Vec p;
    Mtx tex;
    HU3DCAMERA *cam;
    Vec *src;
    f32 dist;
    s32 i;

    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
    PSMTXTrans(tex, arg0->dir.x, arg0->dir.z, 0.0f);
    GXLoadTexMtxImm(tex, 0x21, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, 0x21, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, 1, 0, GX_ITM_1);
    GXSetIndTexMtx(GX_ITM_1, (f32 (*)[3])lbl_1_rodata_AB8, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetZMode(1, GX_LEQUAL, 0);
    cam = &Hu3DCamera[Hu3DCameraNo];
    src = &arg0->pts[0];
    for (i = 0; i < 80; i++) {
        p = *src;
        PSVECSubtract(&p, &cam->pos, &d);
        dist = PSVECMag(&d);
        if (!(dist > 8000.0f)) {
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            RingVert(p.x - 1000.0f, p.y, p.z - 1000.0f, 0.0f, 0.0f);
            RingVert(1000.0f + p.x, p.y, p.z - 1000.0f, 0.0f, 1.0f);
            RingVert(1000.0f + p.x, p.y, 1000.0f + p.z, 1.0f, 1.0f);
            RingVert(p.x - 1000.0f, p.y, 1000.0f + p.z, 1.0f, 0.0f);
            WT_END();
        }
        src++;
    }
    GXSetNumIndStages(0);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetTexCoordScaleManually(GX_TEXCOORD0, 0, 0, 0);
    GXSetTexCoordScaleManually(GX_TEXCOORD1, 0, 0, 0);
}

void fn_1_1C2E0(HU3DMODEL *p, Mtx *mtx)
{
    Mgr511WaterRipple *o = (Mgr511WaterRipple *)p->hookData;

    fn_1_1C9EC(o, *mtx);
}

Mgr511WaterRipple *fn_1_1C324(Mgr511WaterRipple *arg0, Vec *arg1)
{
    HU3DMODELID id;

    fn_1_1D88(arg0);
    arg0->vtable = lbl_1_data_40;
    arg0->flags = 0;
    fn_1_1E54(arg0, lbl_1_bss_2C);
    arg0->vtable = &lbl_1_data_6F4[0];
    arg0->time = 0.0f;
    arg0->pos.x = arg1->x;
    arg0->pos.y = arg1->y;
    arg0->pos.z = arg1->z;
    id = Hu3DHookFuncCreate(fn_1_1C2E0);
    Hu3DData[id].hookData = arg0;
    arg0->modelId = id;
    Hu3DModelLayerSet(id, 3);
    Hu3DModelCameraSet(id, 0xF);
    arg0->anim1 = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m511, 0x2C), HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(arg0->anim1);
    arg0->anim2 = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m511, 0x2F), HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(arg0->anim2);
    fn_1_1C4F4(arg0);
    return arg0;
}

Mgr511WaterRipple *fn_1_1C45C(Mgr511WaterRipple *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        arg0->vtable = &lbl_1_data_6F4[0];
        Hu3DModelKill(arg0->modelId);
        HuSprAnimKill(arg0->anim1);
        HuSprAnimKill(arg0->anim2);
        arg0->vtable = lbl_1_data_40;
        fn_1_1D94(arg0, 0);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_1C4F4(Mgr511WaterRipple *arg0)
{
    s32 i;
    f32 ang;
    f32 radius;

    for (i = 0; i < 32; i++) {
        ang = (f32)((3.141592653589793 * (f64)(22.5f * (f32)(i >> 1))) / 180.0);
        if ((i & 1) == 0) {
            radius = 550.0f;
        } else {
            radius = 800.0f;
        }
        arg0->pts[i].x = (f32)((f64)radius * sin(ang));
        arg0->pts[i].y = 0.0f;
        arg0->pts[i].z = (f32)((f64)radius * cos(ang));
    }
}

void fn_1_1C640(Mgr511WaterRipple *p, s32 bit)
{
    p->flags &= ~((u32)1 << bit);
}

void fn_1_1C658(Mgr511WaterRipple *arg0)
{
    f32 ang;
    f32 ang2;
    f64 sinA;
    f64 sinB;
    Vec p;

    arg0->time = (f32)((f64)arg0->time + 0.016666666666666666);
    ang = 0.017453292f * (10.0f * arg0->time);
    ang2 = 0.017453292f * (20.0f * arg0->time);
    sinB = sin(ang2);
    sinA = sin(ang);
    arg0->dir.x = (f32)sinA;
    arg0->dir.y = 0.0f;
    arg0->dir.z = (f32)sinB;
    if (lbl_1_bss_1C == NULL) {
        fn_1_CF8("ms_pointer != 0", "singleton.h", 0x27);
    }
    p.x = arg0->pos.x;
    p.y = arg0->pos.y;
    p.z = arg0->pos.z;
    arg0->pos.y = 30.000002f + fn_1_1A150(lbl_1_bss_1C, &p);
}

void fn_1_1C9EC(Mgr511WaterRipple *arg0, Mtx mtx)
{
    Mtx trans;
    Mtx tex;
    Vec p0;
    Vec p1;
    Vec p2;
    Vec p3;
    s32 i;

    PSMTXTrans(trans, arg0->pos.x, arg0->pos.y, arg0->pos.z);
    PSMTXConcat(mtx, trans, trans);
    GXLoadPosMtxImm(trans, 0);
    HuSprTexLoad(arg0->anim1, 0, 0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    HuSprTexLoad(arg0->anim2, 0, 1, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
    PSMTXTrans(tex, arg0->dir.x, arg0->dir.z, 0.0f);
    GXLoadTexMtxImm(tex, 0x21, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, 0x21, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_RASA, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, 1, 0, GX_ITM_1);
    GXSetIndTexMtx(GX_ITM_1, (f32 (*)[3])lbl_1_rodata_AE8, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetZMode(1, GX_LEQUAL, 0);
    GXSetCullMode(GX_CULL_NONE);
    GXBegin(GX_QUADS, GX_VTXFMT0, 0x40);
    for (i = 0; i < 32; i += 2) {
        p0 = arg0->pts[i & 0x1F];
        p1 = arg0->pts[(i + 1) & 0x1F];
        p3 = arg0->pts[(i + 3) & 0x1F];
        p2 = arg0->pts[(i + 2) & 0x1F];
        WT_VTX3(p0.x, p0.y, p0.z);
        WT_CLR4(0xFF, 0xFF, 0xFF, 0xFF);
        WT_TEX2(0.0f, 0.0f);
        WT_VTX3(p1.x, p1.y, p1.z);
        WT_CLR4(0xFF, 0xFF, 0xFF, 0);
        WT_TEX2(0.0f, 1.0f);
        WT_VTX3(p3.x, p3.y, p3.z);
        WT_CLR4(0xFF, 0xFF, 0xFF, 0);
        WT_TEX2(1.0f, 1.0f);
        WT_VTX3(p2.x, p2.y, p2.z);
        WT_CLR4(0xFF, 0xFF, 0xFF, 0xFF);
        WT_TEX2(1.0f, 0.0f);
    }
    WT_END();
    GXSetNumIndStages(0);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetTexCoordScaleManually(GX_TEXCOORD0, 0, 0, 0);
    GXSetTexCoordScaleManually(GX_TEXCOORD1, 0, 0, 0);
}

void *lbl_1_data_6F4[5] = {
    NULL,
    NULL,
    (void *)fn_1_1C45C,
    (void *)fn_1_1C640,
    (void *)fn_1_1C658,
};

void *lbl_1_data_708[5] = {
    NULL,
    NULL,
    (void *)fn_1_1BAD0,
    (void *)fn_1_1BB68,
    (void *)fn_1_1BB80,
};

void *lbl_1_data_71C[5] = {
    NULL,
    NULL,
    (void *)fn_1_19A74,
    (void *)fn_1_1A518,
    (void *)fn_1_1A530,
};

}
