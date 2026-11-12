#include "types.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/memory.h"
#include "game/sprite.h"
#include "humath.h"
#include "datadir_enum.h"
#include <string.h>

#include "REL/m580Dll.h"

#ifdef __MWERKS__
#define M580_RODATA __declspec(section ".rodata")
#else
#define M580_RODATA
#endif

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

double fmod(double, double);
f32 fn_1_1970(void);
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);

#define M580_SHADOW_MAX 7

/* effect object work block (0x1C bytes) */
typedef struct M580EffectData_s {
    /* 0x00 */ ANIMDATA *shadowAnim;
    /* 0x04 */ f32 shadowAlpha;
    /* 0x08 */ s32 show;
    /* 0x0C */ s32 hudState;
    /* 0x10 */ f32 hudAlpha;
    /* 0x14 */ s16 hudSpr;
    /* 0x18 */ f32 hudScale;
} M580EffectData;

void fn_1_D86C(OMOBJ *obj);
void fn_1_D8C0(OMOBJ *obj);
void fn_1_D8F8(OMOBJ *obj);
void fn_1_DA00(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
void fn_1_DC10(OMOBJ *obj);
void fn_1_DD18(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
void fn_1_DF28(OMOBJ *obj);
void fn_1_DFD8(OMOBJ *obj);
void fn_1_E114(HU3DMODEL *modelP, Mtx *mtx);
void fn_1_E36C(M580_SHADOW *shadow, Mtx *mtx);
void fn_1_E8D0(OMOBJ *obj);
void fn_1_E9B8(OMOBJ *obj);

s32 lbl_1_data_7A0[16] = {
    0x860025,
    0x860026,
    0x860027,
    0x860028,
    0x860029,
    0x86002A,
    0x86002B,
    0x86002C,
    0x86002D,
    0x86002E,
    0x86002F,
    0x860030,
    0x860031,
    0x860032,
    0x860025,
    0x860025,
};

OMOBJ *lbl_1_bss_248;

M580_RODATA f32 lbl_1_rodata_618[2][3] = { { 0.02f, 0.0f, 0.0f }, { 0.0f, 0.02f, 0.0f } };
M580_RODATA GXVtxDescList lbl_1_rodata_630[5] = {
    { GX_VA_POS, GX_DIRECT },
    { GX_VA_CLR0, GX_DIRECT },
    { GX_VA_TEX0, GX_DIRECT },
    { GX_VA_TEX1, GX_DIRECT },
    { GX_VA_NULL, GX_NONE },
};
M580_RODATA GXVtxAttrFmtList lbl_1_rodata_658[5] = {
    { GX_VA_POS, GX_POS_XYZ, GX_F32, 0 },
    { GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0 },
    { GX_VA_TEX0, GX_TEX_ST, GX_F32, 0 },
    { GX_VA_TEX1, GX_TEX_ST, GX_F32, 0 },
    { GX_VA_NULL, GX_POS_XYZ, GX_F32, 0 },
};

void fn_1_D7E0(OMOBJMAN *objMan)
{
    OMOBJ *obj;

    obj = omAddObjEx(objMan, 0x14, 3, 0, OM_GRP_NONE, fn_1_D86C);
    lbl_1_bss_248 = obj;
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M580EffectData), HU_MEMNUM_OVL);
    memset(obj->data, 0, sizeof(M580EffectData));
}

void fn_1_D868(void)
{
}

void fn_1_D86C(OMOBJ *obj)
{
    fn_1_DF28(obj);
    fn_1_E8D0(obj);
    fn_1_D8F8(obj);
    fn_1_DC10(obj);
    obj->objFunc = fn_1_D8C0;
}

void fn_1_D8C0(OMOBJ *obj)
{
    fn_1_DFD8(obj);
    fn_1_E9B8(obj);
}

void fn_1_D8F8(OMOBJ *obj)
{
    HU3DMODELID model;
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *pd;
    s32 i;

    model = Hu3DParticleCreate(HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m580, 0x22), HU_MEMNUM_OVL, HEAP_MODEL)), 0x100);
    obj->mdlId[0] = model;
    particle = Hu3DData[model].hookData;
    Hu3DParticleBlendModeSet(model, 0);
    Hu3DParticleHookSet(model, fn_1_DA00);
    Hu3DModelLayerSet(model, 5);
    Hu3DModelPosSet(model, 0.0f, 0.0f, 0.0f);
    pd = particle->data;
    for (i = 0; i < particle->maxCnt; i++, pd++) {
        pd->time = -1;
        pd->scale = 0.0f;
    }
}

void fn_1_DA00(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *pd = particleP->data;
    s32 i;
    s32 alpha;

    for (i = 0; i < particleP->maxCnt; i++, pd++) {
        switch (pd->time) {
            case 0:
                pd->time++;
                pd->vel.x = 2.0 * (3.0517578125e-05 * (f32)(frand() - 0x8000));
                pd->vel.y = 5.0 + 2.0 * (1.52587890625e-05 * (f32)frand());
                pd->vel.z = 2.0 * (3.0517578125e-05 * (f32)(frand() - 0x8000));
                pd->color.r = 0xFF;
                pd->color.g = 0xFF;
                pd->color.b = 0xFF;
                pd->color.a = 0x80;
                pd->scale = 30.0f;
            case 1:
                VECAdd(&pd->vel, &pd->pos, &pd->pos);
                alpha = pd->color.a - 8;
                if (alpha < 0) {
                    alpha = 0;
                    pd->scale = 0.0f;
                    pd->time = -1;
                }
                pd->color.a = alpha;
                break;
        }
    }
}

void fn_1_DC10(OMOBJ *obj)
{
    HU3DMODELID model;
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *pd;
    s32 i;

    model = Hu3DParticleCreate(HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m580, 0x21), HU_MEMNUM_OVL, HEAP_MODEL)), 0x100);
    obj->mdlId[1] = model;
    particle = Hu3DData[model].hookData;
    Hu3DParticleBlendModeSet(model, 0);
    Hu3DParticleHookSet(model, fn_1_DD18);
    Hu3DModelLayerSet(model, 5);
    Hu3DModelPosSet(model, 0.0f, 0.0f, 0.0f);
    pd = particle->data;
    for (i = 0; i < particle->maxCnt; i++, pd++) {
        pd->time = -1;
        pd->scale = 0.0f;
    }
}

void fn_1_DD18(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *pd = particleP->data;
    s32 i;
    s32 alpha;

    for (i = 0; i < particleP->maxCnt; i++, pd++) {
        switch (pd->time) {
            case 0:
                pd->time++;
                pd->vel.x = 2.0 * (3.0517578125e-05 * (f32)(frand() - 0x8000));
                pd->vel.y = 5.0 + 2.0 * (1.52587890625e-05 * (f32)frand());
                pd->vel.z = 2.0 * (3.0517578125e-05 * (f32)(frand() - 0x8000));
                pd->color.r = 0xE0;
                pd->color.g = 0xE0;
                pd->color.b = 0x80;
                pd->color.a = 0x80;
                pd->scale = 150.0f;
            case 1:
                VECAdd(&pd->vel, &pd->pos, &pd->pos);
                alpha = pd->color.a - 4;
                if (alpha < 0) {
                    alpha = 0;
                    pd->scale = 0.0f;
                    pd->time = -1;
                }
                pd->color.a = alpha;
                break;
        }
    }
}

void fn_1_DF28(OMOBJ *obj)
{
    M580EffectData *data = obj->data;
    HU3DMODELID model;
    void *list;

    data->shadowAnim = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m580, 0x23), HU_MEMNUM_OVL, HEAP_MODEL));
    model = Hu3DHookFuncCreate(fn_1_E114);
    obj->mdlId[2] = model;
    Hu3DModelLayerSet(model, 1);
    list = HuMemDirectMallocNum(HEAP_HEAP, M580_SHADOW_MAX * sizeof(M580_SHADOW *), HU_MEMNUM_OVL);
    Hu3DData[model].hookData = list;
    memset(list, 0, M580_SHADOW_MAX * sizeof(M580_SHADOW *));
}

void fn_1_DFD8(OMOBJ *obj)
{
    M580EffectData *data = obj->data;
    HU3DMODELID model = obj->mdlId[2];
    M580_SHADOW **list = Hu3DData[model].hookData;
    M580_SHADOW *shadow;
    f32 angle;
    s32 frame;
    s32 i;

    data->shadowAlpha = fn_1_1970();
    for (i = 0; i < M580_SHADOW_MAX; i++) {
        shadow = list[i];
        if (shadow != NULL) {
            angle = fmod(11.25 + *shadow->rot, 360.0);
            if (angle < 0.0f) {
                angle = angle + 360.0;
            }
            frame = angle / 22.5;
            if (frame < 0) {
                frame = 0;
            }
            if (frame >= 0xF) {
                frame = 0xF;
            }
            shadow->frame = frame;
        }
    }
}

void fn_1_E114(HU3DMODEL *modelP, Mtx *mtx)
{
    M580_SHADOW **list = modelP->hookData;
    M580EffectData *data = lbl_1_bss_248->data;
    s32 i;

    HuSprTexLoad(data->shadowAnim, 0, GX_TEXMAP1, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    GXSetNumIndStages(1);
    GXSetIndTexMtx(GX_ITM_0, lbl_1_rodata_618, 0);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP1);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_ALPHA0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetCullMode(GX_CULL_NONE);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZCompLoc(GX_TRUE);
    GXSetZMode(GX_TRUE, GX_LESS, GX_FALSE);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_LEQUAL, 0xFF);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXClearVtxDesc();
    GXSetVtxDescv(lbl_1_rodata_630);
    GXSetVtxAttrFmtv(GX_VTXFMT0, lbl_1_rodata_658);
    for (i = 0; i < M580_SHADOW_MAX; i++) {
        fn_1_E36C(list[i], mtx);
    }
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetNumIndStages(0);
}

void fn_1_E36C(M580_SHADOW *shadow, Mtx *mtx)
{
    M580EffectData *data;
    Mtx m;
    f32 x;
    f32 y;
    f32 z;
    f32 s0;
    f32 s1;
    f32 alpha;
    s32 a0;
    s32 a1;

    if (shadow != NULL && shadow->frame != -1 && shadow->active != 0) {
        x = shadow->pos->x + shadow->ofs->x;
        y = -shadow->pos->y + shadow->ofs->y;
        z = shadow->pos->z + shadow->ofs->z;
        PSMTXScale(m, 205.0f, 205.0f, 80.0f);
        PSMTXTransApply(m, m, x, y, z);
        PSMTXConcat(*mtx, m, m);
        GXLoadPosMtxImm(m, GX_PNMTX0);
        HuSprTexLoad(shadow->anim, 0, GX_TEXMAP0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
        s0 = shadow->frame * 0.0625;
        s1 = (shadow->frame + 1) * 0.0625;
        PSMTXScale(m, 0.5f, 0.5f, 1.0f);
        PSMTXTransApply(m, m, 0.0025f * x, 0.0025f * z, 0.0f);
        GXLoadTexMtxImm(m, GX_TEXMTX0, GX_MTX2x4);
        data = lbl_1_bss_248->data;
        alpha = data->shadowAlpha;
        a0 = 96.0f * alpha;
        a1 = 24.0f * alpha;
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-0.5f, 0.0f, 0.0f);
        GXColor4u8(0, 0, 0, a0);
        GXTexCoord2f32(s0, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(0.5f, 0.0f, 0.0f);
        GXColor4u8(0, 0, 0, a0);
        GXTexCoord2f32(s1, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(0.5f, -1.0f, 1.0f);
        GXColor4u8(0, 0, 0, a1);
        GXTexCoord2f32(s1, 1.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-0.5f, -1.0f, 1.0f);
        GXColor4u8(0, 0, 0, a1);
        GXTexCoord2f32(s0, 1.0f);
        GXTexCoord2f32(0.0f, 1.0f);
    }
}

void fn_1_E8D0(OMOBJ *obj)
{
    M580EffectData *data = obj->data;
    ANIMDATA *anim;
    HUSPRGRPID grp;

    data->show = 0;
    data->hudState = 0;
    data->hudAlpha = 0.0f;
    data->hudScale = 0.0f;
    anim = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m580, 0x24), HU_MEMNUM_OVL, HEAP_MODEL));
    grp = HuSprGrpCreate(1);
    data->hudSpr = grp;
    HuSprGrpPosSet(grp, 288.0f, 350.0f);
    HuSprGrpMemberSet(grp, 0, HuSprCreate(anim, 0xA, 0));
    HuSprAttrSet(grp, 0, 4);
}

void fn_1_E9B8(OMOBJ *obj)
{
    M580EffectData *data = obj->data;
    HUSPRGRPID grp = data->hudSpr;

    if (data->show != 0) {
        data->hudAlpha = data->hudAlpha + 0.2;
        if (data->hudAlpha > 1.0f) {
            data->hudAlpha = 1.0f;
        }
        HuSprTPLvlSet(grp, 0, data->hudAlpha);
        HuSprAttrReset(grp, 0, 4);
    } else {
        data->hudAlpha = data->hudAlpha - 0.2;
        if (data->hudAlpha <= 0.0f) {
            data->hudAlpha = 0.0f;
            HuSprAttrSet(grp, 0, 4);
        } else {
            HuSprTPLvlSet(grp, 0, data->hudAlpha);
        }
    }
    switch (data->hudState) {
        case 0:
            if (data->show == 0) {
                break;
            }
            data->hudState++;
        case 1:
            data->hudScale = data->hudScale + 0.3;
            if (data->hudScale >= 1.5f) {
                data->hudState++;
            }
            break;
        case 2:
            data->hudScale = data->hudScale - 0.1;
            if (!(data->hudScale > 1.0f)) {
                data->hudState++;
                data->hudScale = 1.0f;
            }
            break;
        case 3:
            if (data->show == 0) {
                data->hudState = 0;
            }
            break;
    }
    HuSprScaleSet(grp, 0, data->hudScale, data->hudScale);
}

void fn_1_EBCC(HuVecF *pos)
{
    HU3DPARTICLE *particle = Hu3DData[lbl_1_bss_248->mdlId[0]].hookData;
    HU3DPARTICLEDATA *pd = particle->data;
    s32 i;

    for (i = 0; i < particle->maxCnt; i++, pd++) {
        if (pd->time < 0) {
            pd->time = 0;
            pd->pos = *pos;
            return;
        }
    }
}

void fn_1_EC74(HuVecF *pos, HuVecF *vel)
{
    HU3DPARTICLE *particle = Hu3DData[lbl_1_bss_248->mdlId[1]].hookData;
    HU3DPARTICLEDATA *pd = particle->data;
    s32 i;

    for (i = 0; i < particle->maxCnt; i++, pd++) {
        if (pd->time < 0) {
            pd->time = 0;
            pd->pos = *pos;
            pd->accel = *vel;
            return;
        }
    }
}

void fn_1_ED34(M580_SHADOW *shadow)
{
    M580_SHADOW **list = Hu3DData[lbl_1_bss_248->mdlId[2]].hookData;
    s32 i;

    for (i = 0; i < M580_SHADOW_MAX; i++, list++) {
        if (*list == NULL) {
            *list = shadow;
            shadow->anim = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_7A0[shadow->charNo], HU_MEMNUM_OVL, HEAP_MODEL));
            shadow->frame = -1;
            return;
        }
    }
}

void fn_1_EDF8(s32 show)
{
    OMOBJ *obj = lbl_1_bss_248;
    M580EffectData *data = obj->data;

    data->show = show;
}
