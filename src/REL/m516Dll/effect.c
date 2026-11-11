#include "types.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/memory.h"
#include "game/sprite.h"
#include "math.h"
#include "string.h"
#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "REL/m516Dll/m516types.h"

extern void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);

typedef struct {
    s32 active;
    u32 color;
    f32 size;
    Vec pos;
} EFFSPRITE;

typedef struct {
    ANIMDATA *anim;
    EFFSPRITE sprites[8];
} EFFHOOK1;

typedef struct {
    EFFRING *rings[4];
    f32 sinTbl[32];
    f32 cosTbl[32];
} EFFHOOK2;

OMOBJ *lbl_1_bss_40;

void fn_1_6F84(OMOBJ *obj);
void fn_1_6FC8(OMOBJ *obj);
void fn_1_6FF0(OMOBJ *obj);
void fn_1_70AC(OMOBJ *obj);
void fn_1_7164(HU3DMODEL *modelP, Mtx *mtx);
void fn_1_74B0(OMOBJ *obj);
void fn_1_75E8(HU3DMODEL *modelP, Mtx *mtx);

void fn_1_6F24(OMOBJMAN *objman)
{
    lbl_1_bss_40 = omAddObjEx(objman, 0x32, 2, 0, -1, fn_1_6F84);
    fn_1_6F84(lbl_1_bss_40);
}

void fn_1_6F80(void)
{
}

void fn_1_6F84(OMOBJ *obj)
{
    fn_1_6FF0(obj);
    fn_1_74B0(obj);
    obj->objFunc = fn_1_6FC8;
}

void fn_1_6FC8(OMOBJ *obj)
{
    fn_1_70AC(obj);
}

void fn_1_6FF0(OMOBJ *obj)
{
    EFFHOOK1 *hook;
    s16 mdl;
    HU3DMODEL *model;

    mdl = Hu3DHookFuncCreate(fn_1_7164);
    obj->mdlId[0] = mdl;
    Hu3DModelLayerSet(mdl, 0);
    model = &Hu3DData[mdl];
    hook = HuMemDirectMallocNum(HEAP_HEAP, sizeof(EFFHOOK1), 0x10000000);
    model->hookData = hook;
    memset(hook, 0, sizeof(EFFHOOK1));
    hook->anim = HuSprAnimRead(HuDataSelHeapReadNum(0x4A0011, 0x10000000, HEAP_MODEL));
}

void fn_1_70AC(OMOBJ *obj)
{
    EFFSPRITE *spr;
    GXColor *col;
    s32 i;
    HU3DMODEL *model;
    s32 alpha;
    s16 mdl = obj->mdlId[0];
    EFFHOOK1 *hook;

    model = &Hu3DData[mdl];
    hook = model->hookData;
    for (i = 0; i < 8; i++) {
        spr = &hook->sprites[i];
        if (spr->active != 0) {
            spr->size += 5.0;
            col = (GXColor *)&spr->color;
            alpha = col->a - 4;
            col->a = alpha;
            if (alpha <= 0) {
                spr->active = 0;
            }
        }
    }
}

void fn_1_7164(HU3DMODEL *modelP, Mtx *mtx)
{
    EFFHOOK1 *hook = modelP->hookData;
    EFFSPRITE *spr;
    Mtx m;
    f32 size;
    s32 i;
    u32 color;

    HuSprTexLoad(hook->anim, 0, 0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_TRUE, GX_LESS, GX_FALSE);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_LEQUAL, 0xFF);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetNumChans(1);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    for (i = 0; i < 8; i++) {
        spr = &hook->sprites[i];
        if (spr->active != 0) {
            color = spr->color;
            size = spr->size;
            PSMTXIdentity(m);
            PSMTXTrans(m, spr->pos.x, spr->pos.y, spr->pos.z);
            PSMTXConcat(*mtx, m, m);
            GXLoadPosMtxImm(m, 0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(-size, 0.0f, -size);
            GXColor1u32(color);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(size, 0.0f, -size);
            GXColor1u32(color);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(size, 0.0f, size);
            GXColor1u32(color);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(-size, 0.0f, size);
            GXColor1u32(color);
            GXTexCoord2f32(0.0f, 1.0f);
            GXEnd();
        }
    }
}

// Remaining difference: placement of the loop index initialisation.
void fn_1_74B0(OMOBJ *obj)
{
    EFFHOOK2 *hook;
    s16 mdl;
    HU3DMODEL *model;
    f32 angle;
    s32 i;

    mdl = Hu3DHookFuncCreate(fn_1_75E8);
    obj->mdlId[1] = mdl;
    Hu3DModelLayerSet(mdl, 5);
    model = &Hu3DData[mdl];
    hook = HuMemDirectMallocNum(HEAP_HEAP, sizeof(EFFHOOK2), 0x10000000);
    model->hookData = hook;
    memset(hook, 0, sizeof(EFFHOOK2));
    angle = 0.0f;
    for (i = 0; i < 0x20; angle += 11.0f) {
        hook->sinTbl[i] = sin((3.141592653589793 * angle) / 180.0);
        hook->cosTbl[i] = cos((3.141592653589793 * angle) / 180.0);
        i++;
    }
}

void fn_1_75E8(HU3DMODEL *modelP, Mtx *mtx)
{
    EFFHOOK2 *hook = modelP->hookData;
    f32 *sinTbl = hook->sinTbl;
    f32 *cosTbl = hook->cosTbl;
    EFFRING *ring;
    Mtx m;
    f32 size;
    s32 i;
    s32 j;

    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_TRUE, GX_LESS, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    for (i = 0; i < 4; i++) {
        ring = hook->rings[i];
        if (ring != NULL && ring->active != 0) {
            size = ring->size;
            PSMTXIdentity(m);
            PSMTXTrans(m, ring->pos->x, ring->pos->y, ring->pos->z);
            PSMTXConcat(*mtx, m, m);
            GXLoadPosMtxImm(m, 0);
            GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 0x22);
            GXPosition3f32(0.0f, 0.0f, 0.0f);
            GXColor1u32(0x80);
            for (j = 0; j < 0x20; j++) {
                GXPosition3f32(size * sinTbl[j], 0.0f, size * cosTbl[j]);
                GXColor1u32(8);
            }
            GXPosition3f32(*sinTbl * size, 0.0f, *cosTbl * size);
            GXColor1u32(8);
            GXEnd();
        }
    }
}

void fn_1_78B8(Vec *pos, f32 size)
{
    EFFSPRITE *spr;
    s32 i;
    OMOBJ *eff = lbl_1_bss_40;
    HU3DMODEL *model;
    s16 mdl = eff->mdlId[0];
    EFFHOOK1 *hook;

    model = &Hu3DData[mdl];
    hook = model->hookData;
    for (i = 0; i < 8; i++) {
        spr = &hook->sprites[i];
        if (spr->active == 0) {
            spr->active = 1;
            spr->color = -1;
            spr->size = size;
            spr->pos = *pos;
            return;
        }
    }
}

void fn_1_796C(EFFRING *ring)
{
    EFFRING **slot;
    s32 i;
    OMOBJ *eff = lbl_1_bss_40;
    HU3DMODEL *model;
    s16 mdl = eff->mdlId[1];
    EFFHOOK2 *hook;

    model = &Hu3DData[mdl];
    hook = model->hookData;
    slot = hook->rings;
    for (i = 0; i < 4; i++, slot++) {
        if (*slot == NULL) {
            *slot = ring;
            return;
        }
    }
}
