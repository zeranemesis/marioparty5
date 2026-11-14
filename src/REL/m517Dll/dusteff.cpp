// m517Dll dusteff: two flat ground blobs drawn by a hook model: a translucent
// square (M517DustQuad) and a round fan (M517DustFan).
#include "REL/m517Dll.h"
extern "C" {
#include <math.h>

#ifdef __MWERKS__
#define M517_RODATA __declspec(section ".rodata")
#else
#define M517_RODATA
#endif

void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);

void fn_1_BDB0(HU3DMODEL *model, Mtx mtx);
void fn_1_BF38(M517DustQuad *self, Mtx mtx);
void fn_1_C3E8(HU3DMODEL *model, Mtx mtx);
void fn_1_C550(M517DustFan *self, Mtx mtx);

M517_RODATA const f32 lbl_1_rodata_1B8[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

// Hook model draw callback: forward to the quad.
void fn_1_BDB0(HU3DMODEL *model, Mtx mtx)
{
    MtxPtr sp8 = mtx;

    fn_1_BF38((M517DustQuad *)model->hookData, sp8);
}

M517DustQuad *fn_1_BDF4(M517DustQuad *self)
{
    HU3DMODEL *model;

    self->model = Hu3DHookFuncCreate((HU3DMODELHOOK)fn_1_BDB0);
    model = &Hu3DData[self->model];
    model->hookData = self;
    Hu3DModelLayerSet(self->model, 4);
    Hu3DModelCameraSet(self->model, 1);
    self->anim = HuSprAnimRead(HuDataSelHeapReadNum(0x4B0013, HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(self->anim);
    self->pos.x = 0.0f;
    self->pos.y = 0.0f;
    self->pos.z = 0.0f;
    self->col[0] = 0;
    self->col[1] = 0;
    self->col[2] = 0x40;
    self->col[3] = 0x40;
    return self;
}

M517DustQuad *fn_1_BEDC(M517DustQuad *self, s16 flag)
{
    if (self != 0) {
        Hu3DModelKill(self->model);
        HuSprAnimKill(self->anim);
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

// Draw the 100x100 translucent square, 5 units above pos.
void fn_1_BF38(M517DustQuad *self, Mtx mtx)
{
    Mtx m;
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    PSMTXTrans(m, self->pos.x, 5.0f + self->pos.y, self->pos.z);
    PSMTXConcat(mtx, m, m);
    GXLoadPosMtxImm(m, 0);
    HuSprTexLoad(self->anim, 0, 0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, 0x7D);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT0, GX_DF_CLAMP, GX_AF_SPOT);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    r = self->col[0];
    g = self->col[1];
    b = self->col[2];
    a = self->col[3];
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-50.0f, 0.0f, -50.0f);
    GXColor4u8(r, g, b, a);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(50.0f, 0.0f, -50.0f);
    GXColor4u8(r, g, b, a);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(50.0f, 0.0f, 50.0f);
    GXColor4u8(r, g, b, a);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(-50.0f, 0.0f, 50.0f);
    GXColor4u8(r, g, b, a);
    GXTexCoord2f32(1.0f, 0.0f);
    GXEnd();
}

// Hook model draw callback: forward to the fan.
void fn_1_C3E8(HU3DMODEL *model, Mtx mtx)
{
    MtxPtr sp8 = mtx;

    fn_1_C550((M517DustFan *)model->hookData, sp8);
}

M517DustFan *fn_1_C42C(M517DustFan *self)
{
    HU3DMODEL *model;

    self->model = Hu3DHookFuncCreate((HU3DMODELHOOK)fn_1_C3E8);
    model = &Hu3DData[self->model];
    model->hookData = self;
    Hu3DModelLayerSet(self->model, 2);
    Hu3DModelCameraSet(self->model, 1);
    self->anim = HuSprAnimRead(HuDataSelHeapReadNum(0x4B0013, HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(self->anim);
    self->pos.x = 0.0f;
    self->pos.y = 0.0f;
    self->pos.z = 0.0f;
    return self;
}

M517DustFan *fn_1_C4F4(M517DustFan *self, s16 flag)
{
    if (self != 0) {
        Hu3DModelKill(self->model);
        HuSprAnimKill(self->anim);
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

// Draw the round blob (a 10 vertex fan of radius 50), 2 units above pos.
void fn_1_C550(M517DustFan *self, Mtx mtx)
{
    Mtx m;
    s32 i;
    f32 ang;
    f32 c;
    f32 s;

    PSMTXTrans(m, self->pos.x, 2.0f + self->pos.y, self->pos.z);
    PSMTXConcat(mtx, m, m);
    GXLoadPosMtxImm(m, 0);
    HuSprTexLoad(self->anim, 0, 0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, 0x7D);
    GXSetCullMode(GX_CULL_BACK);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT0, GX_DF_CLAMP, GX_AF_SPOT);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 10);
    GXPosition3f32(0.0f, 0.0f, 0.0f);
    GXColor4u8(0, 0, 0x40, 0x40);
    GXTexCoord2f32(0.5f, 0.5f);
    for (i = 0; i < 9; i++) {
        ang = (3.141592653589793 * (45.0f * (f32)i)) / 180.0;
        c = cos(ang);
        s = sin(ang);
        GXPosition3f32(100.0f * (0.5f * c), -16.0f, 100.0f * (0.5f * s));
        GXColor4u8(0, 0, 0x40, 0x40);
        GXTexCoord2f32(0.5f + (0.5f * c), 0.5f + (0.5f * s));
    }
    GXEnd();
}

} // extern "C"
