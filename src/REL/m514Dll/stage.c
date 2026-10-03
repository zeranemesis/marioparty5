#define _MATH_H
#define GXSetFog GXSetFogByValue
#include "game/flag.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/process.h"

#include "datadir_enum.h"

#include <string.h>
#undef GXSetFog

void GXSetFog(GXFogType type, f32 start, f32 end, f32 near, f32 far, GXColor *color);


typedef struct M514Fog_s {
    f32 start;
    f32 end;
    GXColor color;
} M514Fog;

typedef enum M514GxSrc_e {
    M514_GX_SRC_VTX = 1
} M514GxSrc;

typedef enum M514GxAlpha_e {
    M514_GX_AF_NONE = 2
} M514GxAlpha;

f32 fn_1_9794(void);

void fn_1_8180(f32 *arg0);

void fn_1_510C(HUPROCESS *process);
void fn_1_51BC(void);
void fn_1_51C0(OMOBJ *obj);
void fn_1_536C(OMOBJ *obj);
void fn_1_5430(s32 fogType, s32 idx);
void fn_1_54DC(s16 layerNo);
void fn_1_5574(s16 layerNo);
void fn_1_5610(s16 layerNo);
void fn_1_56AC(s16 layerNo);
void fn_1_5744(HUPROCESS *process);
void fn_1_57F0(OMOBJ *obj);
void fn_1_5858(OMOBJ *obj);
void fn_1_585C(HU3DMODEL *modelP, Mtx *mtx);

M514Fog lbl_1_data_4A0[3] = {
    { 20.0f, 7700.0f, { 0x00, 0xEE, 0xFF, 0xFF } },
    { 20.0f, 15000.0f, { 0x00, 0xEE, 0xFF, 0xFF } },
    { 20.0f, 25000.0f, { 0x00, 0xEE, 0xFF, 0xFF } },
};

f32 lbl_1_data_4C4[3] = { -4644.0f, -1228.0f, -4732.0f };

s32 lbl_1_data_4D0 = 0x42FF7A80;

static OMOBJ *lbl_1_bss_210;
static f32 lbl_1_bss_214;

void fn_1_510C(HUPROCESS *process)
{
    OMOBJ *obj;
    void *data;

    obj = omAddObjEx(process, 0x14, 6, 0, -1, fn_1_51C0);
    lbl_1_bss_210 = obj;
    obj->stat |= 0x100;
    data = HuMemDirectMallocNum(HEAP_HEAP, 4, 0x10000000);
    obj->data = data;
    memset(data, 0, 4);
    fn_1_5744(process);
    obj->work[0] = 0;
}

void fn_1_51BC(void)
{
}

void fn_1_51C0(OMOBJ *obj)
{
    HU3DMODELID mdlId;
    f32 *work = obj->data;


    mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m514, 0), HU_MEMNUM_OVL, HEAP_MODEL));
    obj->mdlId[0] = mdlId;
    Hu3DModelAttrSet(mdlId, 0x40000001);
    Hu3DModelLayerSet(mdlId, 4);
    Hu3DModelShadowMapSet(mdlId);

    mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m514, 1), HU_MEMNUM_OVL, HEAP_MODEL));
    obj->mdlId[1] = mdlId;
    Hu3DModelAttrSet(mdlId, 0x40000001);
    Hu3DModelLayerSet(mdlId, 4);

    mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m514, 2), HU_MEMNUM_OVL, HEAP_MODEL));
    obj->mdlId[2] = mdlId;
    Hu3DModelAttrSet(mdlId, 0x40000040);
    Hu3DModelLayerSet(mdlId, 3);

    mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m514, 3), HU_MEMNUM_OVL, HEAP_MODEL));
    obj->mdlId[3] = mdlId;
    Hu3DModelAttrSet(mdlId, 0x40000001);
    Hu3DModelLayerSet(mdlId, 2);

    mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m514, 4), HU_MEMNUM_OVL, HEAP_MODEL));
    obj->mdlId[4] = mdlId;
    Hu3DModelAttrSet(mdlId, 0x40000001);
    Hu3DModelLayerSet(mdlId, 1);
    Hu3DModelShadowMapSet(mdlId);

    if (_CheckFlag(0x30002) == 0) {
        fn_1_57F0(obj);
    }
    obj->objFunc = fn_1_536C;
}

void fn_1_536C(OMOBJ *obj)
{
    if ((obj->work[0] & 3) == 0) {
        fn_1_8180(lbl_1_data_4C4);
    }
if ((lbl_1_bss_214 = lbl_1_bss_214 - 0.013) < 1.0) {
        lbl_1_bss_214 = 1.0f;
    }

    if (_CheckFlag(0x30002) == 0) {
        fn_1_5858(obj);
    }
    obj->work[0] += 1;
}

void fn_1_5430(s32 fogType, s32 idx)
{
    f32 fov, near, far;
    GXColor color;
    GXColor *cp;
    M514Fog *fog = &lbl_1_data_4A0[idx];

    Hu3DCameraPerspectiveGet(1, &fov, &near, &far);
    color.r = fog->color.r;
    color.g = fog->color.g;
    color.b = fog->color.b;
    color.a = fog->color.a;
    cp = &color;
    GXSetFog(fogType, fog->start, fog->end * lbl_1_bss_214, near, far, cp);
}

void fn_1_54DC(s16 layerNo)
{
    GXColor color;
    f32 far, near, fov;
    GXColor *cp;
    M514Fog *fog = &lbl_1_data_4A0[0];

    Hu3DCameraPerspectiveGet(1, &fov, &near, &far);
    color.r = fog->color.r;
    color.g = fog->color.g;
    color.b = fog->color.b;
    color.a = fog->color.a;
    cp = &color;
    GXSetFog(GX_FOG_LIN, fog->start, fog->end * lbl_1_bss_214, near, far, cp);
}

void fn_1_5574(s16 layerNo)
{
    GXColor color;
    f32 far, near, fov;
    GXColor *cp;
    M514Fog *fog = &lbl_1_data_4A0[1];

    Hu3DCameraPerspectiveGet(1, &fov, &near, &far);
    color.r = fog->color.r;
    color.g = fog->color.g;
    color.b = fog->color.b;
    color.a = fog->color.a;
    cp = &color;
    GXSetFog(GX_FOG_LIN, fog->start, fog->end * lbl_1_bss_214, near, far, cp);
}

void fn_1_5610(s16 layerNo)
{
    GXColor color;
    f32 far, near, fov;
    GXColor *cp;
    M514Fog *fog = &lbl_1_data_4A0[2];

    Hu3DCameraPerspectiveGet(1, &fov, &near, &far);
    color.r = fog->color.r;
    color.g = fog->color.g;
    color.b = fog->color.b;
    color.a = fog->color.a;
    cp = &color;
    GXSetFog(GX_FOG_LIN, fog->start, fog->end * lbl_1_bss_214, near, far, cp);
}

void fn_1_56AC(s16 layerNo)
{
    GXColor color;
    f32 far, near, fov;
    GXColor *cp;
    M514Fog *fog = &lbl_1_data_4A0[0];

    Hu3DCameraPerspectiveGet(1, &fov, &near, &far);
    color.r = fog->color.r;
    color.g = fog->color.g;
    color.b = fog->color.b;
    color.a = fog->color.a;
    cp = &color;
    GXSetFog(GX_FOG_NONE, fog->start, fog->end * lbl_1_bss_214, near, far, cp);
}

void fn_1_5744(HUPROCESS *process)
{
    M514Fog *fog = &lbl_1_data_4A0[0];

    lbl_1_bss_214 = 2.0f;
    Hu3DBGColorSet(fog->color.r, fog->color.g, fog->color.b);
    Hu3DFogSet(fog->start, fog->end, fog->color.r, fog->color.g, fog->color.b);

    Hu3DLayerHookSet(0, fn_1_56AC);
    Hu3DLayerHookSet(2, fn_1_5610);
    Hu3DLayerHookSet(3, fn_1_5574);
    Hu3DLayerHookSet(4, fn_1_54DC);
}

void fn_1_57F0(OMOBJ *obj)
{
    f32 *data = obj->data;
    HU3DMODELID hookId;

    *data = fn_1_9794();
    hookId = Hu3DHookFuncCreate(fn_1_585C);
    obj->mdlId[5] = hookId;
    Hu3DModelLayerSet(hookId, 5);
}

void fn_1_5858(OMOBJ *obj)
{
}

void fn_1_585C(HU3DMODEL *modelP, Mtx *mtx)
{
    Mtx mtx2;
    OMOBJ *obj = lbl_1_bss_210;
    f32 *data = obj->data;
    u32 c0, c1, c2, c3;

    PSMTXIdentity(mtx2);
    PSMTXTrans(mtx2, *data, 0.0f, 0.0f);
    PSMTXConcat(*mtx, mtx2, mtx2);
    GXLoadPosMtxImm(mtx2, GX_PNMTX0);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, M514_GX_SRC_VTX, M514_GX_SRC_VTX, 0, GX_DF_NONE, M514_GX_AF_NONE);

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_RGBA6, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_TEX_ST, GX_RGBA8, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

GXWGFifo.f32 = -15.0f;
    GXWGFifo.f32 = 1.0f;
    GXWGFifo.f32 = -240.0f;
    c0 = lbl_1_data_4D0;
    GXWGFifo.u32 = c0;

    GXWGFifo.f32 = 15.0f;
    GXWGFifo.f32 = 1.0f;
    GXWGFifo.f32 = -240.0f;
    c1 = lbl_1_data_4D0;
    GXWGFifo.u32 = c1;

    GXWGFifo.f32 = 15.0f;
    GXWGFifo.f32 = 1.0f;
    GXWGFifo.f32 = 230.0f;
    c2 = lbl_1_data_4D0;
    GXWGFifo.u32 = c2;

    GXWGFifo.f32 = -15.0f;
    GXWGFifo.f32 = 1.0f;
    GXWGFifo.f32 = 230.0f;
    c3 = lbl_1_data_4D0;
    GXWGFifo.u32 = c3;
}

