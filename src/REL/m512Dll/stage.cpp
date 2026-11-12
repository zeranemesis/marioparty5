// m512Dll/REL/m512Dll/stage
// CStage: the eight stage models (floor, walls, props, shadow caster) and a
// draw hook that renders the black oval fade-out ring.
#define _MATH_H
#include "types.h"
#include "game/hu3d.h"
#include "game/data.h"

#ifdef __MWERKS__
#define STAGE_RODATA __declspec(section ".rodata")
#else
#define STAGE_RODATA
#endif

struct StageNode {
    StageNode *next; // 0x00
    StageNode *prev; // 0x04
};

struct StageModelId {
    s16 id;
};

struct Stage {
    StageNode node;         // 0x00
    void **vtbl;            // 0x08
    s32 unkC;               // 0x0C
    StageModelId model[8];  // 0x10
};

extern "C" {
double sin(double x);
double cos(double x);
void *__dl__FPv(void *);
void *fn_1_1264(void *self);
void fn_1_1270(void *self, int flag);
void fn_1_1330(void *self, void *arg);
void fn_1_1D4(const char *msg, const char *file, int line);
void fn_1_2C94(void *self, s32 dataNum);
void fn_1_30F4(void *self, s32 id);

extern StageNode lbl_1_bss_C;
extern void *lbl_1_data_1FC[4];
extern Stage *lbl_1_bss_A10;
}

extern "C" void *fn_1_8D1C(Stage *self, s16 flag);
extern "C" void fn_1_8754(void);
extern "C" void fn_1_8758(Stage *self, s32 bit);
extern "C" void fn_1_87D0(Stage *self, Mtx mtx);
extern "C" void fn_1_8800(void);

// -------------------------------------------------------------------- data

extern "C" {
char lbl_1_data_330[] = "ms_pointer == 0";
char lbl_1_data_340[] = "singleton.h";
char lbl_1_data_34C[] = "ms_pointer != 0";
STAGE_RODATA const f32 lbl_1_rodata_140[3] = { 0.0f, 3800.0f, 0.0f };
STAGE_RODATA const f32 lbl_1_rodata_14C[3] = { 0.0f, 0.0f, 1.0f };
STAGE_RODATA const f32 lbl_1_rodata_158[3] = { 0.0f, 0.0f, 0.0f };
void *lbl_1_data_35C[5] = { 0, 0, (void *)fn_1_8D1C, (void *)fn_1_8758, (void *)fn_1_8754 };
}

// ------------------------------------------------------------------ functions

extern "C" Stage *fn_1_8144(Stage *self)
{
    s16 m;
    s16 shape;

    fn_1_1264(self);
    self->vtbl = lbl_1_data_1FC;
    self->unkC = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    if (lbl_1_bss_A10 == 0) {
    } else {
        fn_1_1D4(lbl_1_data_330, lbl_1_data_340, 0x35);
    }
    {
        Stage *s = (Stage *)((char *)self + 0x10);
        if (s != 0) {
            s = (Stage *)((char *)s - 0x10);
        }
        lbl_1_bss_A10 = s;
    }
    self->vtbl = lbl_1_data_35C;
    fn_1_2C94(&self->model[0], 0x460000);
    fn_1_2C94(&self->model[1], 0x460001);
    fn_1_2C94(&self->model[2], 0x460002);
    fn_1_2C94(&self->model[3], 0x460003);
    fn_1_2C94(&self->model[4], 0x460004);
    fn_1_2C94(&self->model[5], 0x460005);
    fn_1_2C94(&self->model[6], 0x460007);
    fn_1_2C94(&self->model[7], 0x460008);

    m = self->model[0].id;
    Hu3DModelLayerSet(m, 2);
    Hu3DModelLightInfoSet(m, 1);
    Hu3DModelPosSet(m, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(m, 0x40000002);
    Hu3DModelAttrSet(m, 0x40000001);
    Hu3DModelAttrReset(m, 1);

    m = self->model[1].id;
    Hu3DModelLayerSet(m, 2);
    Hu3DModelPosSet(m, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(m, 0x40000002);
    Hu3DModelAttrSet(m, 0x40000001);
    Hu3DModelAttrReset(m, 1);
    Hu3DModelShadowMapSet(m);

    m = self->model[2].id;
    Hu3DModelLayerSet(m, 2);
    Hu3DModelPosSet(m, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(m, 0x40000002);
    Hu3DModelAttrSet(m, 0x40000001);
    Hu3DModelAttrReset(m, 1);

    m = self->model[3].id;
    Hu3DModelLayerSet(m, 1);
    Hu3DModelPosSet(m, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(m, 0x40000002);
    Hu3DModelAttrSet(m, 0x40000001);
    Hu3DModelAttrReset(m, 1);
    Hu3DModelShadowMapSet(m);

    m = self->model[4].id;
    Hu3DModelLayerSet(m, 0);
    Hu3DModelPosSet(m, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(m, 0x40000002);
    Hu3DModelAttrSet(m, 0x40000001);
    Hu3DModelAttrReset(m, 1);

    m = self->model[5].id;
    Hu3DModelLayerSet(m, 0);
    Hu3DModelPosSet(m, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(m, 0x40000002);
    Hu3DModelAttrSet(m, 0x40000001);
    Hu3DModelAttrReset(m, 1);

    m = self->model[6].id;
    Hu3DModelLayerSet(m, 2);
    Hu3DModelPosSet(m, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(m, 0x40000002);
    Hu3DModelAttrSet(m, 0x40000001);
    Hu3DModelAttrReset(m, 1);
    shape = Hu3DMotionShapeIDGet(m);
    Hu3DModelAttrSet(m, 0x40000040);
    Hu3DMotionShapeSet(m, shape);

    m = self->model[7].id;
    Hu3DModelLayerSet(m, 0);
    Hu3DModelPosSet(m, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(m, 0x40000002);
    Hu3DModelAttrSet(m, 0x40000001);
    Hu3DModelAttrReset(m, 1);
    shape = Hu3DMotionShapeIDGet(m);
    Hu3DModelAttrSet(m, 0x40000040);
    Hu3DMotionShapeSet(m, shape);

    Hu3DShadowSizeSet(0x1E0);
    Hu3DShadowCreate(45.0f, 100.0f, 6000.0f);
    Hu3DShadowTPLvlSet(0.2f);
    Hu3DShadowPosSet((HuVecF *)&lbl_1_rodata_140, (HuVecF *)&lbl_1_rodata_14C, (HuVecF *)&lbl_1_rodata_158);
    return self;
}

extern "C" Stage *fn_1_86C8(Stage *self, s16 flag)
{
    if (self != 0) {
        if (lbl_1_bss_A10 != 0) {
        } else {
            fn_1_1D4(lbl_1_data_34C, lbl_1_data_340, 0x3D);
        }
        lbl_1_bss_A10 = 0;
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

extern "C" void fn_1_8754(void)
{
}

extern "C" void fn_1_8758(Stage *self, s32 bit)
{
    self->unkC &= ~(1 << bit);
    if (bit < 5) {
    }
}

extern "C" void fn_1_8780(HU3DMODEL *model, Mtx mtx)
{
    Stage *s = lbl_1_bss_A10;
    Stage *s2;

    Hu3DModelObjDrawInit();
    s2 = s;
    fn_1_87D0(s2, mtx);
}

extern "C" void fn_1_87D0(Stage *self, Mtx mtx)
{
    GXLoadPosMtxImm(mtx, 0);
    fn_1_8800();
}

// Draws a black rounded-rectangle ring as one triangle strip.
extern "C" void fn_1_8800(void)
{
    f32 sinOuter[33];
    f32 cosOuter[33];
    f32 sinInner[33];
    f32 cosInner[33];
    f32 vy[80];
    f32 vx[80];
    f32 ang;
    f32 rad;
    s32 n;
    s32 i;
    u16 count;

    n = 0;
    for (i = 0; i < 0x21; i++) {
        ang = ((360.0f * ((f32)(i & 0x1F) * 0.03125f)) - 180.0f) + 90.0f;
        if (ang > 180.0f) {
            ang -= 360.0f;
        }
        rad = (f32)((3.141592653589793 * ang) / 180.0);
        cosOuter[i] = 100.0 * (15.0 * cos(rad));
        sinOuter[i] = 100.0 * (15.0 * sin(rad));
        cosInner[i] = 100.0 * (7.0 * cos(rad));
        sinInner[i] = 100.0 * (7.0 * sin(rad));
    }
    for (i = 0; i < 0x11; i++) {
        vx[n] = 300.0 + cosOuter[i];
        vy[n] = sinOuter[i];
        n++;
        vx[n] = 300.0 + cosInner[i];
        vy[n] = sinInner[i];
        n++;
    }
    for (i = 0x10; i < 0x21; i++) {
        vx[n] = -300.0 + cosOuter[i];
        vy[n] = sinOuter[i];
        n++;
        vx[n] = -300.0 + cosInner[i];
        vy[n] = sinInner[i];
        n++;
    }
    vx[n] = 300.0 + cosOuter[0];
    vy[n] = sinOuter[0];
    n++;
    vx[n] = 300.0 + cosInner[0];
    vy[n] = sinInner[0];
    count = n + 1;

    GXSetNumTevStages(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetCullMode(GX_CULL_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_TEX_ST, GX_RGBA8, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, count);
    for (i = 0; i < (s32)count; i++) {
#ifdef TARGET_PC
        GXPosition3f32(vx[i], 0.0f, vy[i]);
        GXColor4u8(0x40, 0x40, 0x40, 0xFF);
#else
        GXWGFifo.f32 = vx[i];
        GXWGFifo.f32 = 0.0f;
        GXWGFifo.f32 = vy[i];
        GXWGFifo.u8 = 0x40;
        GXWGFifo.u8 = 0x40;
        GXWGFifo.u8 = 0x40;
        GXWGFifo.u8 = 0xFF;
#endif
    }
#ifdef TARGET_PC
    GXEnd();
#endif
}

extern "C" void *fn_1_8D1C(Stage *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_35C;
        fn_1_30F4(&self->model[7], -1);
        fn_1_30F4(&self->model[6], -1);
        fn_1_30F4(&self->model[5], -1);
        fn_1_30F4(&self->model[4], -1);
        fn_1_30F4(&self->model[3], -1);
        fn_1_30F4(&self->model[2], -1);
        fn_1_30F4(&self->model[1], -1);
        fn_1_30F4(&self->model[0], -1);
        if ((void *)((char *)self + 0x10) != 0) {
            if (lbl_1_bss_A10 != 0) {
            } else {
                fn_1_1D4(lbl_1_data_34C, lbl_1_data_340, 0x3D);
            }
            lbl_1_bss_A10 = 0;
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
