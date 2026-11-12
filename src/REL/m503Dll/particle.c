#include "types.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/memory.h"
#include "game/sprite.h"
#include "humath.h"
#include "REL/m503Dll.h"
#include <string.h>

#ifndef M_PI
#define M_PI 3.141592653589793
#endif


typedef struct {
    M503Ring *rings[0x28];
    f32 sinTbl[0x20];
    f32 cosTbl[0x20];
} M503RingWork;

void fn_1_C574(OMOBJMAN *objMan);
void fn_1_C598(void);
void fn_1_C59C(void);
void fn_1_C6A4(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
void fn_1_C9FC(void);
void fn_1_CB48(HU3DMODEL *modelP, Mtx *mtx);

s16 lbl_1_bss_C0;
s16 lbl_1_bss_C2;

void fn_1_C574(OMOBJMAN *objMan)
{
    fn_1_C59C();
    fn_1_C9FC();
}

void fn_1_C598(void)
{
}

void fn_1_C59C(void)
{
    HU3DMODELID id;
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *data;
    s32 i;

    id = Hu3DParticleCreate(HuSprAnimRead(HuDataSelHeapReadNum(0x3D0027, HU_MEMNUM_OVL, HEAP_MODEL)), 0x80);
    lbl_1_bss_C2 = id;
    particle = Hu3DData[id].hookData;
    Hu3DParticleBlendModeSet(id, 0);
    Hu3DParticleHookSet(id, fn_1_C6A4);
    Hu3DModelLayerSet(id, 1);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    data = particle->data;
    for (i = 0; i < particle->maxCnt; i++, data++) {
        data->time = -1;
        data->scale = 0.0f;
    }
}

void fn_1_C6A4(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *data = particleP->data;
    s32 i;

    for (i = 0; i < particleP->maxCnt; i++, data++) {
        switch (data->time) {
            case 0:
                data->time++;
                data->accel.x = 0.8 * (3.0517578125e-05 * (f64)(f32)((s32)frand() - 0x8000));
                data->accel.y = 5.0 + 0.5 * (1.52587890625e-05 * (f64)(f32)(s32)frand());
                data->accel.z = 0.8 * (3.0517578125e-05 * (f64)(f32)((s32)frand() - 0x8000));
                data->scale = 200.0 + 40.0 * (1.52587890625e-05 * (f64)(f32)(s32)frand());
                data->zRot = 360.0 * (1.52587890625e-05 * (f64)(f32)(s32)frand());
                data->color.a = 0xB4;
            case 1:
                data->scale += 10.0f;
                data->color.a -= 2;
                if (data->pos.y < -14000.0 || data->scale <= 0.0f || data->color.a > 0xF0) {
                    data->time = -1;
                    data->scale = 0.0f;
                } else {
                    data->pos.x += data->accel.x;
                    data->pos.y += data->accel.y;
                    data->pos.z += data->accel.z;
                    data->accel.x = data->accel.x * 0.9;
                    data->accel.y = data->accel.y - 0.05;
                    data->accel.z = data->accel.z * 0.9;
                }
                break;
        }
    }
}

void fn_1_C9FC(void)
{
    HU3DMODEL *model;
    M503RingWork *work;
    f32 angle;
    s32 i;

    lbl_1_bss_C0 = Hu3DHookFuncCreate(fn_1_CB48);
    Hu3DModelLayerSet(lbl_1_bss_C0, 6);
    model = &Hu3DData[lbl_1_bss_C0];
    work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M503RingWork), HU_MEMNUM_OVL);
    model->hookData = work;
    memset(work, 0, sizeof(M503RingWork));
    angle = 0.0f;
    for (i = 0; i < 0x20; i++) {
        work->sinTbl[i] = HuSin(angle);
        work->cosTbl[i] = HuCos(angle);
        angle += 11.0f;
    }
}

void fn_1_CB48(HU3DMODEL *modelP, Mtx *mtx)
{
    M503RingWork *work = modelP->hookData;
    f32 *sinTbl = work->sinTbl;
    f32 *cosTbl = work->cosTbl;
    M503Ring *ring;
    HuVecF pos;
    Mtx mat;
    f32 scale;
    s32 i;
    s32 j;

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_TRUE, GX_LESS, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    for (i = 0; i < 0x28; i++) {
        ring = work->rings[i];
        if (ring != NULL && ring->active != 0) {
            scale = ring->scale;
            PSVECAdd(ring->pos, ring->offset, &pos);
            PSMTXIdentity(mat);
            PSMTXTrans(mat, pos.x, pos.y, pos.z);
            PSMTXConcat(*mtx, mat, mat);
            GXLoadPosMtxImm(mat, 0);
            GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 0x22);
#ifdef TARGET_PC
            GXPosition3f32(0.0f, 0.0f, 0.0f);
            GXColor1u32(0xA0);
            for (j = 0; j < 0x20; j++) {
                GXPosition3f32(scale * sinTbl[j], 0.0f, scale * cosTbl[j]);
                GXColor1u32(0x16);
            }
            GXPosition3f32(sinTbl[0] * scale, 0.0f, cosTbl[0] * scale);
            GXColor1u32(0x16);
            GXEnd();
#else
            GXWGFifo.f32 = 0.0f;
            GXWGFifo.f32 = 0.0f;
            GXWGFifo.f32 = 0.0f;
            GXWGFifo.u32 = 0xA0;
            for (j = 0; j < 0x20; j++) {
                GXWGFifo.f32 = scale * sinTbl[j];
                GXWGFifo.f32 = 0.0f;
                GXWGFifo.f32 = scale * cosTbl[j];
                GXWGFifo.u32 = 0x16;
            }
            GXWGFifo.f32 = sinTbl[0] * scale;
            GXWGFifo.f32 = 0.0f;
            GXWGFifo.f32 = cosTbl[0] * scale;
            GXWGFifo.u32 = 0x16;
#endif
        }
    }
}

void fn_1_CE1C(HuVecF *pos)
{
    HU3DPARTICLE *particle = Hu3DData[lbl_1_bss_C2].hookData;
    HU3DPARTICLEDATA *data = particle->data;
    s32 i;

    for (i = 0; i < particle->maxCnt; i++, data++) {
        if (data->time < 0) {
            data->time = 0;
            data->pos = *pos;
            return;
        }
    }
}

void fn_1_CEB0(HuVecF *pos)
{
    HU3DPARTICLE *particle = Hu3DData[lbl_1_bss_C2].hookData;
    HU3DPARTICLEDATA *data = particle->data;
    s32 i;

    for (i = 0; i < particle->maxCnt; i++, data++) {
        if (data->time < 0) {
            data->time = 1;
            data->pos = *pos;
            data->accel.x = 0.6 * (3.0517578125e-05 * (f64)(f32)((s32)frand() - 0x8000));
            data->accel.y = 2.0 + 0.5 * (1.52587890625e-05 * (f64)(f32)(s32)frand());
            data->accel.z = 0.6 * (3.0517578125e-05 * (f64)(f32)((s32)frand() - 0x8000));
            data->scale = 300.0 + 40.0 * (1.52587890625e-05 * (f64)(f32)(s32)frand());
            data->zRot = 360.0 * (1.52587890625e-05 * (f64)(f32)(s32)frand());
            data->color.a = 0xA0;
            return;
        }
    }
}

void fn_1_D134(M503Ring *ring)
{
    M503RingWork *work = Hu3DData[lbl_1_bss_C0].hookData;
    s32 i;

    for (i = 0; i < 0x28; i++) {
        if (work->rings[i] == NULL) {
            work->rings[i] = ring;
            return;
        }
    }
}
