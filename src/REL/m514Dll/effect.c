#include "types.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/sprite.h"
#include "REL/m514Dll/m514types.h"

#include <math.h>
#include <string.h>

#define M514_RAD(x) ((3.141592653589793 * (f64)(x)) / 180.0)

void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);

/* Arc (jump trail) hook data: three of these live in the first hook model. */
typedef struct M514Arc_s {
    s32 state;
    s32 pad4;
    s32 counter;
    f32 thick;
    f32 base;
    f32 alpha;
    f32 angOff;
    f32 span;
    f32 tgtSpan;
    f32 maxY;
    f32 yScale;
    HuVecF start;
    HuVecF cur;
} M514ARC;

typedef struct M514Spark_s {
    s32 state;
    s32 t1;
    s32 t2;
    s32 t3;
    GXColor col;
    f32 size;
    f32 x;
    f32 y;
    f32 z;
    s32 pad24;
} M514SPARK;

typedef struct M514Sparks_s {
    ANIMDATA *anim;
    M514SPARK spark[0x100];
} M514SPARKS;

typedef struct M514Ring_s {
    s32 state;
    s32 timer;
    f32 radius;
    GXColor col;
    HuVecF pos;
} M514RING;

typedef struct M514Rings_s {
    M514RING ring[0x200];
    f32 sinTbl[16];
    f32 cosTbl[16];
} M514RINGS;

typedef struct M514Eff_s {
    s32 state;
    s32 pad4;
    f32 alpha;
    s16 grp;
    s16 pad0E;
    f32 scale;
    s16 bank;
    s16 prevBank;
    s32 pad18;
} M514EFF;

void fn_1_5B90(OMOBJ *obj);
void fn_1_5BEC(OMOBJ *obj);
void fn_1_5C34(OMOBJ *obj);
void fn_1_5CD4(OMOBJ *obj);
void fn_1_5DE4(HU3DMODEL *modelP, Mtx *mtx);
void fn_1_6588(OMOBJ *obj);
void fn_1_6644(OMOBJ *obj);
void fn_1_68EC(HU3DMODEL *modelP, Mtx *mtx);
void fn_1_6C44(OMOBJ *obj);
void fn_1_6D7C(OMOBJ *obj);
void fn_1_6ED4(HU3DMODEL *modelP, Mtx *mtx);
void fn_1_7184(OMOBJ *obj);
void fn_1_728C(HU3DMODEL *modelP, HU3DPARTICLE *par, Mtx mtx);
void fn_1_76D0(OMOBJ *obj);
void fn_1_77BC(OMOBJ *obj);
void fn_1_7EBC(HuVecF *pos);

GXColor lbl_1_data_4D8[7] = {
    { 0xFF, 0x00, 0x00, 0x00 },
    { 0xFF, 0x80, 0x00, 0x00 },
    { 0xFF, 0xFF, 0x00, 0x00 },
    { 0x00, 0xFF, 0x00, 0x00 },
    { 0x00, 0x80, 0xFF, 0x00 },
    { 0x00, 0x00, 0xFF, 0x00 },
    { 0x80, 0x00, 0xFF, 0x00 },
};

OMOBJ *lbl_1_bss_218;

#define M514_HOOKDATA(idx) (Hu3DData[(s16)lbl_1_bss_218->mdlId[idx]].hookData)

void fn_1_5B04(OMOBJMAN *objman)
{
    OMOBJ *obj;

    obj = omAddObjEx(objman, 0x3C, 4, 0, -1, fn_1_5B90);
    lbl_1_bss_218 = obj;
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M514EFF), 0x10000000);
    memset(obj->data, 0, sizeof(M514EFF));
}

void fn_1_5B8C(void)
{
}

void fn_1_5B90(OMOBJ *obj)
{
    fn_1_5C34(obj);
    fn_1_6588(obj);
    fn_1_6C44(obj);
    fn_1_76D0(obj);
    fn_1_7184(obj);
    obj->objFunc = fn_1_5BEC;
}

void fn_1_5BEC(OMOBJ *obj)
{
    fn_1_5CD4(obj);
    fn_1_6644(obj);
    fn_1_6D7C(obj);
    fn_1_77BC(obj);
}

void fn_1_5C34(OMOBJ *obj)
{
    HU3DMODELID model;
    void *data;

    model = Hu3DHookFuncCreate(fn_1_5DE4);
    obj->mdlId[0] = model;
    Hu3DModelLayerSet(model, 6);
    data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M514ARC) * 3, 0x10000000);
    Hu3DData[model].hookData = data;
    memset(data, 0, sizeof(M514ARC) * 3);
}

void fn_1_5CD4(OMOBJ *obj)
{
    M514ARC *arc = Hu3DData[(s16)obj->mdlId[0]].hookData;
    s32 i;

    for (i = 0; i < 3; i++, arc++) {
        switch (arc->state) {
            case 2:
            case 0:
                break;
            case 1:
                arc->state++;
                arc->angOff = 0.0f;
                arc->tgtSpan = 5.0f;
                arc->span = 5.0f;
                arc->yScale = 1.0f;
                break;
            case 3:
                arc->alpha = arc->alpha - 0.01;
                if (!(arc->alpha > 0.0f)) {
                    arc->state = 0;
                }
                break;
        }
    }
}

void fn_1_5DE4(HU3DMODEL *modelP, Mtx *mtx)
{
    M514ARC *arc = modelP->hookData;
    GXColor colA[7];
    GXColor colB[7];
    Mtx m;
    s32 i;
    s32 seg;
    s32 k;

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
    memcpy(colA, lbl_1_data_4D8, sizeof(colA));
    memcpy(colB, lbl_1_data_4D8, sizeof(colB));
    for (i = 0; i < 3; i++, arc++) {
        if (arc->state >= 2) {
            f32 midX = arc->start.x + 0.5 * (arc->cur.x - arc->start.x);
            f32 midZ = arc->start.z + 0.5 * (arc->cur.z - arc->start.z);
            f32 yScale = arc->yScale;
            HuVecF d;
            f32 radius;
            f32 width;
            f32 step;
            f32 ang;

            d.x = arc->cur.x - midX;
            d.y = arc->cur.y - 0.0f;
            d.z = arc->cur.z - midZ;
            radius = PSVECMag(&d);
            width = arc->thick;
            step = arc->span / 24.0f;
            PSMTXIdentity(m);
            PSMTXTrans(m, midX, 0.0f, midZ);
            PSMTXConcat(*mtx, m, m);
            GXLoadPosMtxImm(m, GX_PNMTX0);
            ang = 0.0f;
            for (seg = 0; seg < 0x18; seg++) {
                f32 a0 = (f32)(270.0 + (f64)(ang + arc->angOff));
                f32 a1 = a0 + step;
                f32 t;
                s32 alpha0;
                s32 alpha1;

                if (0.0 != arc->span) {
                    t = (f32)(180.0 * (f64)(ang / arc->span));
                } else {
                    t = 0.0f;
                }
                alpha0 = (s32)(128.0f * (f32)sin(M514_RAD(t)) * arc->alpha);
                alpha1 = (s32)(128.0f * (f32)sin(M514_RAD(t + step)) * arc->alpha);
                for (k = 0; k < 7; k++) {
                    colA[k].a = (u8)alpha0;
                    colB[k].a = (u8)alpha1;
                }
                GXBegin(GX_QUADS, GX_VTXFMT0, 0x18);
                for (k = 0; k < 6; k++) {
                    f32 r0 = radius - width * (f32)k;
                    f32 r1;

                    if (r0 < 0.0f) {
                        r0 = 0.0f;
                    }
                    r1 = r0 - width;
                    if (r1 < 0.0f) {
                        r1 = 0.0f;
                    }
                    GXPosition3f32((f32)((f64)r0 * sin(M514_RAD(a0))), (f32)((f64)yScale * ((f64)r0 * cos(M514_RAD(a0)))), 0.0f);
                    GXColor1u32(*(u32 *)&colA[k]);
                    GXPosition3f32((f32)((f64)r0 * sin(M514_RAD(a1))), (f32)((f64)yScale * ((f64)r0 * cos(M514_RAD(a1)))), 0.0f);
                    GXColor1u32(*(u32 *)&colB[k]);
                    GXPosition3f32((f32)((f64)r1 * sin(M514_RAD(a1))), (f32)((f64)yScale * ((f64)r1 * cos(M514_RAD(a1)))), 0.0f);
                    GXColor1u32(*(u32 *)&colB[k + 1]);
                    GXPosition3f32((f32)((f64)r1 * sin(M514_RAD(a0))), (f32)((f64)yScale * ((f64)r1 * cos(M514_RAD(a0)))), 0.0f);
                    GXColor1u32(*(u32 *)&colA[k + 1]);
                }
                ang += step;
            }
        }
    }
}

void fn_1_6588(OMOBJ *obj)
{
    HU3DMODELID model;
    M514SPARKS *sparks;

    model = Hu3DHookFuncCreate(fn_1_68EC);
    obj->mdlId[1] = model;
    Hu3DModelLayerSet(model, 6);
    sparks = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M514SPARKS), 0x10000000);
    Hu3DData[model].hookData = sparks;
    memset(sparks, 0, sizeof(M514SPARKS));
    sparks->anim = HuSprAnimRead(HuDataSelHeapReadNum(0x480020, 0x10000000, HEAP_MODEL));
}

void fn_1_6644(OMOBJ *obj)
{
    M514SPARKS *sparks = Hu3DData[(s16)obj->mdlId[1]].hookData;
    s32 i;

    for (i = 0; i < 0x100; i++) {
        M514SPARK *sp = &sparks->spark[i];

        if (sp->state != 0) {
            u8 a;
            s32 n;
            f32 ang;

            sp->size = sp->size - 0.5;
            sp->y = sp->y + 2.0;
            a = sp->col.a - 6;
            sp->col.a = a;
            n = sp->t1;
            sp->t1 = n + 1;
            ang = (f32)(5.625 * (f64)(f32)(n & 0x1F));
            sp->col.r = (u8)(s32)(255.0 * sin(M514_RAD(ang)));
            n = sp->t2;
            sp->t2 = n + 1;
            ang = (f32)(5.625 * (f64)(f32)(n & 0x1F));
            sp->col.g = (u8)(s32)(255.0 * sin(M514_RAD(ang)));
            n = sp->t3;
            sp->t3 = n + 1;
            ang = (f32)(5.625 * (f64)(f32)(n & 0x1F));
            sp->col.b = (u8)(s32)(255.0 * sin(M514_RAD(ang)));
            if (sp->size <= 0.0f || (s32)a <= 0) {
                sp->state = 0;
            }
        }
    }
}

void fn_1_68EC(HU3DMODEL *modelP, Mtx *mtx)
{
    M514SPARKS *sparks = modelP->hookData;
    s32 i;

    HuSprTexLoad(sparks->anim, 0, 0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_TRUE, GX_LESS, GX_FALSE);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_LEQUAL, 0xFF);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
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
    GXLoadPosMtxImm(*mtx, GX_PNMTX0);
    for (i = 0; i < 0x100; i++) {
        M514SPARK *sp = &sparks->spark[i];

        if (sp->state != 0) {
            u32 col = *(u32 *)&sp->col;
            f32 size = sp->size;
            f32 x = sp->x;
            f32 y = sp->y;

            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(x - size, y - size, 0.0f);
            GXColor1u32(col);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(x + size, y - size, 0.0f);
            GXColor1u32(col);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(x + size, y + size, 0.0f);
            GXColor1u32(col);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(x - size, y + size, 0.0f);
            GXColor1u32(col);
            GXTexCoord2f32(0.0f, 1.0f);
        }
    }
}

void fn_1_6C44(OMOBJ *obj)
{
    HU3DMODELID model;
    M514RINGS *rings;
    f64 ang;
    s32 i;

    model = Hu3DHookFuncCreate(fn_1_6ED4);
    obj->mdlId[2] = model;
    Hu3DModelLayerSet(model, 4);
    rings = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M514RINGS), 0x10000000);
    Hu3DData[model].hookData = rings;
    memset(rings, 0, sizeof(M514RINGS));
    ang = 0.0;
    for (i = 0; i < 0x10; i++) {
        rings->sinTbl[i] = (f32)sin(M514_RAD(ang));
        rings->cosTbl[i] = (f32)cos(M514_RAD(ang));
        ang = (f32)ang + 22.0f;
    }
}

void fn_1_6D7C(OMOBJ *obj)
{
    M514RINGS *rings = Hu3DData[(s16)obj->mdlId[2]].hookData;
    s32 i;

    for (i = 0; i < 0x200; i++) {
        M514RING *ring = &rings->ring[i];

        switch (ring->state) {
            case 0:
                break;
            case 1:
                ring->state++;
                *(u32 *)&ring->col = 0;
                /* fallthrough */
            case 2:
                ring->col.a += 4;
                if (ring->col.a < 0x30) {
                    break;
                }
                ring->state++;
                ring->timer = 0;
                ring->col.a = 0x30;
                /* fallthrough */
            case 3:
                ring->timer++;
                if ((f32)ring->timer < 300.0) {
                    break;
                }
                ring->state++;
                /* fallthrough */
            case 4:
                ring->col.a -= 1;
                if (ring->col.a <= 4) {
                    ring->state = 0;
                }
                break;
        }
    }
}

void fn_1_6ED4(HU3DMODEL *modelP, Mtx *mtx)
{
    M514RINGS *rings = modelP->hookData;
    Mtx m;
    s32 i;
    s32 k;

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
    for (i = 0; i < 0x200; i++) {
        M514RING *ring = &rings->ring[i];

        if (ring->state >= 2) {
            f32 radius = ring->radius;
            u32 col = *(u32 *)&ring->col;

            PSMTXIdentity(m);
            PSMTXTrans(m, ring->pos.x, ring->pos.y, ring->pos.z);
            PSMTXConcat(*mtx, m, m);
            GXLoadPosMtxImm(m, GX_PNMTX0);
            GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 0x12);
            GXPosition3f32(0.0f, 0.0f, 0.0f);
            GXColor1u32(col);
            for (k = 0; k < 0x10; k++) {
                GXPosition3f32(radius * rings->sinTbl[k], 0.0f, radius * rings->cosTbl[k]);
                GXColor1u32(col);
            }
            GXPosition3f32(rings->sinTbl[0] * radius, 0.0f, rings->cosTbl[0] * radius);
            GXColor1u32(col);
        }
    }
}

void fn_1_7184(OMOBJ *obj)
{
    HU3DMODELID model;
    HU3DPARTICLE *par;
    HU3DPARTICLEDATA *data;
    ANIMDATA *anim;
    s32 i;

    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x480021, 0x10000000, HEAP_MODEL));
    model = Hu3DParticleCreate(anim, 0x80);
    obj->mdlId[3] = model;
    par = Hu3DData[model].hookData;
    Hu3DParticleBlendModeSet(model, 0);
    Hu3DParticleHookSet(model, fn_1_728C);
    Hu3DModelLayerSet(model, 5);
    Hu3DModelPosSet(model, 0.0f, 0.0f, 0.0f);
    data = par->data;
    for (i = 0; i < par->maxCnt; i++, data++) {
        data->time = -1;
        data->scale = 0.0f;
    }
}

void fn_1_728C(HU3DMODEL *modelP, HU3DPARTICLE *par, Mtx mtx)
{
    HU3DPARTICLEDATA *data = par->data;
    s32 i;

    for (i = 0; i < par->maxCnt; i++, data++) {
        switch (data->time) {
            case 0:
                data->time++;
                data->pos.x = data->pos.x + 100.0 * (3.0517578125e-05 * (f64)(f32)((s32)frand() - 0x8000));
                data->pos.y = data->pos.y + 30.0 * (3.0517578125e-05 * (f64)(f32)((s32)frand() - 0x8000));
                data->pos.z = data->pos.z + 100.0 * (3.0517578125e-05 * (f64)(f32)((s32)frand() - 0x8000));
                data->accel.x = 2.0 * (3.0517578125e-05 * (f64)(f32)((s32)frand() - 0x8000));
                data->accel.y = 1.0 + 0.2 * (1.52587890625e-05 * (f64)(f32)frand());
                data->accel.z = 0.8 * (3.0517578125e-05 * (f64)(f32)((s32)frand() - 0x8000));
                data->scale = 80.0 + 30.0 * (1.52587890625e-05 * (f64)(f32)frand());
                data->zRot = 360.0 * (1.52587890625e-05 * (f64)(f32)frand());
                data->color.a = 0xC8;
                /* fallthrough */
            case 1:
                data->scale = data->scale + 5.0f;
                data->color.a = data->color.a - 2;
                if (data->color.a > 0xF0) {
                    data->time = -1;
                    data->scale = 0.0f;
                } else {
                    data->pos.x = data->pos.x + data->accel.x;
                    data->pos.y = data->pos.y + data->accel.y;
                    data->pos.z = data->pos.z + data->accel.z;
                    data->accel.x = data->accel.x * 0.95;
                    data->accel.y = data->accel.y - 0.03;
                    data->accel.z = data->accel.z * 0.95;
                }
                break;
        }
    }
}

void fn_1_76D0(OMOBJ *obj)
{
    M514EFF *w = obj->data;
    ANIMDATA *anim;
    HUSPRGRPID grp;

    w->state = 0;
    w->alpha = 0.0f;
    w->scale = 0.0f;
    w->prevBank = -1;
    w->bank = -1;
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x48001F, 0x10000000, HEAP_MODEL));
    grp = HuSprGrpCreate(1);
    w->grp = grp;
    HuSprGrpPosSet(grp, 288.0f, 410.0f);
    HuSprGrpMemberSet(grp, 0, HuSprCreate(anim, 10, 0));
    HuSprAttrSet(grp, 0, 4);
}

void fn_1_77BC(OMOBJ *obj)
{
    M514EFF *w = obj->data;
    HUSPRGRPID grp = w->grp;

    if (w->bank >= 0) {
        w->alpha = w->alpha + 0.2;
        if (w->alpha > 1.0) {
            w->alpha = 1.0f;
        }
        HuSprTPLvlSet(grp, 0, w->alpha);
        HuSprAttrReset(grp, 0, 4);
        if (w->prevBank != w->bank) {
            w->state = 1;
            w->scale = 0.0f;
        }
    } else {
        w->alpha = w->alpha - 0.2;
        if (w->alpha <= 0.0) {
            w->alpha = 0.0f;
            HuSprAttrSet(grp, 0, 4);
        } else {
            HuSprTPLvlSet(grp, 0, w->alpha);
        }
    }
    switch (w->state) {
        case 1:
            w->scale = w->scale + 0.3;
            if (!(w->scale < 1.5)) {
                w->state++;
            }
            break;
        case 2:
            w->scale = w->scale - 0.1;
            if (!(w->scale > 1.0)) {
                w->state = 0;
                w->scale = 1.0f;
            }
            break;
    }
    HuSprScaleSet(grp, 0, w->scale, w->scale);
    if (w->bank != w->prevBank) {
        if (w->bank >= 0) {
            HuSprBankSet(grp, 0, w->bank);
        }
        w->prevBank = w->bank;
    }
}

s32 fn_1_79F8(HuVecF *pos, f32 size)
{
    M514ARC *arc = M514_HOOKDATA(0);
    s32 i;

    for (i = 0; i < 3; i++, arc++) {
        if (arc->state == 0) {
            arc->state++;
            arc->counter = 1;
            arc->start.x = pos->x;
            arc->start.y = pos->y;
            arc->start.z = pos->z;
            arc->cur.x = pos->x;
            arc->cur.y = pos->y;
            arc->cur.z = pos->z;
            arc->base = size;
            arc->thick = size;
            arc->alpha = 1.0f;
            arc->tgtSpan = 5.0f;
            arc->span = 5.0f;
            arc->maxY = 0.0f;
            arc->yScale = 1.0f;
            return i;
        }
    }
    return -1;
}

void fn_1_7B14(s32 id)
{
    OMOBJ *mgr = lbl_1_bss_218;
    s16 cur = mgr->mdlId[0];
    HU3DMODEL *ent = &Hu3DData[cur];
    M514ARC *tab = ent->hookData;

    if (id >= 0 && id < 3) {
        M514ARC *slot = &tab[id];
        slot->state = 0;
    }
}

void fn_1_7B8C(s32 id)
{
    M514ARC *arcs = M514_HOOKDATA(0);

    if (id >= 0 && id < 3) {
        M514ARC *arc = &arcs[id];
        if (arc->state != 0) {
            arc->state = 3;
        }
    }
}

void fn_1_7C10(s32 id, HuVecF *pos)
{
    M514ARC *arcs = M514_HOOKDATA(0);
    s32 rising = 0;

    if (id >= 0 && id < 3) {
        M514ARC *arc = &arcs[id];

        if (pos->y < arc->cur.y) {
            rising = 1;
        }
        if (arc->maxY < pos->y) {
            arc->maxY = pos->y;
        }
        arc->cur.x = arc->cur.x + 0.1 * (pos->x - arc->cur.x);
        arc->cur.y = arc->cur.y + 0.1 * (pos->y - arc->cur.y);
        arc->cur.z = pos->z;
        arc->yScale = 1.0 - (f64)(arc->cur.x - arc->start.x) / 4000.0;
        arc->thick = arc->base * (0.9 * (1.0 + (1.0 - arc->yScale)));
        if (arc->tgtSpan < 120.0 && rising == 0) {
            arc->tgtSpan = arc->tgtSpan + 4.0f;
            if (arc->tgtSpan > 120.0) {
                arc->tgtSpan = 120.0f;
            }
        } else if (0.0 != arc->maxY) {
            arc->tgtSpan = 120.0 + 60.0 * (f64)((arc->maxY - pos->y) / arc->maxY);
        }
        arc->span = arc->span + 0.2 * (arc->tgtSpan - arc->span);
        if (arc->span > 180.0) {
            arc->span = 180.0f;
        }
        arc->counter--;
        if (arc->counter <= 0) {
            arc->counter = 2;
            fn_1_7EBC(pos);
        }
    }
}

void fn_1_7EBC(HuVecF *pos)
{
    M514SPARKS *sparks = M514_HOOKDATA(1);
    s32 i;

    for (i = 0; i < 0x100; i++) {
        M514SPARK *sp = &sparks->spark[i];

        if (sp->state == 0) {
            sp->state = 1;
            sp->t1 = frand();
            sp->t2 = frand();
            sp->t3 = frand();
            *(u32 *)&sp->col = 0xFFFFFFFF;
            sp->size = 40.0 + 20.0 * (1.52587890625e-05 * (f64)(f32)frand());
            sp->x = (pos->x + 30.0 * (3.0517578125e-05 * (f64)(f32)((s32)frand() - 0x8000))) - 50.0;
            sp->y = pos->y + 50.0 * (3.0517578125e-05 * (f64)(f32)((s32)frand() - 0x8000));
            sp->z = pos->z - 100.0;
            return;
        }
    }
}

void fn_1_80C0(HuVecF *pos, f32 size)
{
    M514RINGS *rings = M514_HOOKDATA(2);
    s32 i;

    for (i = 0; i < 0x200; i++) {
        M514RING *ring = &rings->ring[i];

        if (ring->state == 0) {
            ring->state++;
            ring->pos.x = pos->x;
            ring->pos.y = 3.0 + pos->y;
            ring->pos.z = pos->z;
            ring->radius = size;
            return;
        }
    }
}

void fn_1_8180(HuVecF *pos)
{
    HU3DPARTICLE *par = M514_HOOKDATA(3);
    HU3DPARTICLEDATA *data = par->data;
    s32 i;

    for (i = 0; i < par->maxCnt; i++, data++) {
        if (data->time < 0) {
            data->time = 0;
            data->pos.x = pos->x;
            data->pos.y = pos->y;
            data->pos.z = pos->z;
            return;
        }
    }
}

void fn_1_8228(s16 bank)
{
    OMOBJ *mgr = lbl_1_bss_218;
    M514EFF *src = mgr->data;
    M514EFF *dst = src;

    dst->bank = (s16)bank;
}
