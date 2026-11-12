// m513Dll powereff: the power-up smoke effect. A smoke particle object and a
// controller that drifts a glowing, distorted quad towards a target.
#define _MATH_H
#include "REL/m513Dll.h"
extern "C" {
#include "game/frand.h"
}

#ifdef __MWERKS__
#define M513_RODATA __declspec(section ".rodata")
#else
#define M513_RODATA
#endif

// Smoke particle object (0x34 bytes).
typedef struct M513Smoke_s {
    M513Node link;
    void **vtbl;
    u32 flags;
    s16 modelId;
    HU3DPARTICLE *particle;
    Vec pos;
    Vec vec;
    u8 active;
} M513Smoke;

// Controller (0x48 bytes): owns the smoke object and a draw hook.
typedef struct M513PowerEff_s {
    M513Node link;
    void **vtbl;
    u32 flags;
    Vec pos;
    Vec target;
    s16 hookModel;
    u8 col[4];
    f32 timer;
    s32 state;
    f32 limit;
    M513Smoke *smoke;
    s32 frame;
    f32 scale;
} M513PowerEff;

extern "C" {

void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);

void fn_1_8E78(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx);
M513Smoke *fn_1_9114(M513Smoke *self, s16 flag);
void fn_1_929C(M513Smoke *self, s32 bit);
void fn_1_92B4(M513Smoke *self, HU3DPARTICLEDATA *p);
void fn_1_94E4(M513Smoke *self, Mtx mtx);
void fn_1_985C(HU3DMODEL *model, Mtx mtx);
M513PowerEff *fn_1_9F20(M513PowerEff *self, s16 flag);
void fn_1_A064(M513PowerEff *self, s32 bit);
void fn_1_A07C(M513PowerEff *self);
void fn_1_A528(M513PowerEff *self, Mtx mtx);

char lbl_1_data_330[] = "ms_pointer != 0";
char lbl_1_data_340[] = "singleton.h";
void *lbl_1_data_34C[5] = { 0, 0, (void *)fn_1_9F20, (void *)fn_1_A064, (void *)fn_1_A07C };
void *lbl_1_data_360[6] = { 0, 0, (void *)fn_1_9114, (void *)fn_1_929C, (void *)fn_1_22E0, 0 };
M513_RODATA const f32 lbl_1_rodata_220[6] = { 0.1f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f };

// Restart one smoke particle around the smoke object.
#define M513_SMOKE_INIT(self, p)                                                                   \
    {                                                                                              \
        u16 col_;                                                                                  \
        (p)->time = 0;                                                                             \
        (p)->parManId = (frand() & 0xF) + 4;                                                       \
        (p)->pos.x = (self)->pos.x + (50.0f * (0.000030517578f * ((f32)(u16)frand() - 32768.0f))); \
        (p)->pos.y = (self)->pos.y;                                                                \
        (p)->pos.z = (self)->pos.z + (50.0f * (0.000030517578f * ((f32)(u16)frand() - 32768.0f))); \
        (p)->scale = 100.0f * (0.2f + (0.2f * (0.000015258789f * (f32)(u16)frand())));             \
        (p)->zRot = 0.017453292f * (360.0f * (0.000015258789f * (f32)(u16)frand()));               \
        col_ = frand();                                                                            \
        (p)->vel.y = 0.0f;                                                                         \
        (p)->color.r = (u8)col_;                                                                   \
        (p)->color.g = (u8)(col_ >> 8);                                                            \
        (p)->color.b = (u8)(col_ >> 16);                                                           \
        (p)->color.a = 0;                                                                          \
    }

void fn_1_8E78(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx)
{
    MtxPtr sp8 = mtx;

    fn_1_94E4((M513Smoke *)particle->work, sp8);
}

M513Smoke *fn_1_8EBC(M513Smoke *self)
{
    HU3DMODELID id;
    HU3DPARTICLEDATA *p;
    s32 i;

    fn_1_1768(&self->link);
    self->vtbl = (void **)&lbl_1_data_218;
    self->flags = 0;
    fn_1_1834(&self->link, &lbl_1_bss_C);
    self->vtbl = lbl_1_data_360;
    self->active = 0;
    M513_ASSERT(lbl_1_bss_9F0 != 0, lbl_1_data_330, lbl_1_data_340, 0x27);
    id = Hu3DParticleCreate(lbl_1_bss_9F0->subAnim[1], 2);
    self->particle = (HU3DPARTICLE *)Hu3DData[id].hookData;
    self->particle->work = self;
    Hu3DParticleHookSet(id, fn_1_8E78);
    Hu3DParticleColSet(id, 0xFF, 0xFF, 0xFF);
    Hu3DParticleTPLvlSet(id, 0.0f);
    Hu3DParticleBlendModeSet(id, 1);
    Hu3DModelLayerSet(id, 5);
    Hu3DModelCameraSet(id, 1);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    self->pos.x = 0.0f;
    self->pos.y = 0.0f;
    self->pos.z = 0.0f;
    self->vec.x = 0.0f;
    self->vec.y = 0.0f;
    self->vec.z = 0.0f;
    self->modelId = id;
    p = self->particle->data;
    for (i = 0; i < (s16)self->particle->maxCnt; i++, p++) {
        fn_1_92B4(self, p);
        p->time = 0xFF;
    }
    return self;
}

M513Smoke *fn_1_9114(M513Smoke *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_360;
        self->particle->work = 0;
        Hu3DModelKill(self->modelId);
        if (self != 0) {
            self->vtbl = (void **)&lbl_1_data_218;
            if (self != 0) {
                fn_1_1774(&self->link, 0);
            }
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// Start the smoke at `pos`.
void fn_1_91A8(M513Smoke *self, Vec *pos)
{
    HU3DPARTICLEDATA *p;
    s32 i;

    p = self->particle->data;
    for (i = 0; i < (s16)self->particle->maxCnt; i++, p++) {
        fn_1_92B4(self, p);
    }
    self->pos.x = pos->x;
    self->pos.y = pos->y;
    self->pos.z = pos->z;
    self->active = 1;
}

void fn_1_9240(M513Smoke *self)
{
    HU3DPARTICLEDATA *p;
    s32 i;

    p = self->particle->data;
    for (i = 0; i < (s16)self->particle->maxCnt; i++, p++) {
        p->color.a = 0;
        p->time = 0xFF;
    }
    self->active = 0;
}

void fn_1_929C(M513Smoke *self, s32 bit)
{
    self->flags &= ~(1 << bit);
}

void fn_1_92B4(M513Smoke *self, HU3DPARTICLEDATA *p)
{
    M513_SMOKE_INIT(self, p);
}

// Particle hook: fade the smoke in and out and let it rise.
void fn_1_94E4(M513Smoke *self, Mtx mtx)
{
    HU3DMODEL *model;
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *p;
    s32 i;
    f32 prev;

    if (self->active != 0) {
        model = &Hu3DData[self->modelId];
        particle = self->particle;
        p = particle->data;
        for (i = 0; i < (s16)particle->maxCnt; i++, p++) {
            if ((s16)p->time != 0xFF) {
                if ((s16)p->time == 0) {
                    p->color.a = p->color.a + 0x10;
                    if (p->color.a > 0xEE) {
                        p->color.a = 0xFF;
                        p->time++;
                    }
                } else {
                    p->color.a = p->color.a - p->parManId;
                    if (p->color.a < 0x14) {
                        p->time = 0;
                        M513_SMOKE_INIT(self, p);
                    }
                }
                p->zRot = p->zRot + 0.017453292f;
                prev = p->vel.y;
                p->vel.y = p->vel.y + -0.05448138970072485;
                p->pos.y = p->pos.y + (0.5f * (prev + p->vel.y));
            }
        }
    }
}

void fn_1_9824(M513Smoke *self, Vec *src)
{
    self->pos.x = src->x;
    self->pos.y = src->y;
    self->pos.z = src->z;
}

void fn_1_9840(M513Smoke *self, Vec *src)
{
    self->vec.x = src->x;
    self->vec.y = src->y;
    self->vec.z = src->z;
}

void fn_1_985C(HU3DMODEL *model, Mtx mtx)
{
    MtxPtr sp8 = mtx;

    fn_1_A528((M513PowerEff *)model->hookData, sp8);
}

M513PowerEff *fn_1_98A0(M513PowerEff *self)
{
    HU3DMODELID hook;
    M513Smoke *smoke;

    fn_1_1768(&self->link);
    self->vtbl = (void **)&lbl_1_data_218;
    self->flags = 0;
    fn_1_1834(&self->link, &lbl_1_bss_C);
    self->vtbl = lbl_1_data_34C;
    self->timer = 0.0f;
    self->state = 0;
    self->limit = 0.0f;
    self->smoke = 0;
    self->frame = 0;
    self->scale = 0.0f;
    self->pos.x = 0.0f;
    self->pos.y = 0.0f;
    self->pos.z = 0.0f;
    self->target.x = 0.0f;
    self->target.y = 0.0f;
    self->target.z = 0.0f;
    hook = Hu3DHookFuncCreate((HU3DMODELHOOK)fn_1_985C);
    Hu3DData[hook].hookData = self;
    Hu3DModelLayerSet(hook, 4);
    Hu3DModelCameraSet(hook, 1);
    self->hookModel = hook;
    self->col[2] = 0xFF;
    self->col[1] = 0xFF;
    self->col[0] = 0xFF;
    self->col[3] = 0;
    smoke = (M513Smoke *)fn_1_0(sizeof(M513Smoke));
    if (smoke != 0) {
        HU3DMODELID id;
        HU3DPARTICLEDATA *p;
        s32 i;

        fn_1_1768(&smoke->link);
        smoke->vtbl = (void **)&lbl_1_data_218;
        smoke->flags = 0;
        fn_1_1834(&smoke->link, &lbl_1_bss_C);
        smoke->vtbl = lbl_1_data_360;
        smoke->active = 0;
        M513_ASSERT(lbl_1_bss_9F0 != 0, lbl_1_data_330, lbl_1_data_340, 0x27);
        id = Hu3DParticleCreate(lbl_1_bss_9F0->subAnim[1], 2);
        smoke->particle = (HU3DPARTICLE *)Hu3DData[id].hookData;
        smoke->particle->work = smoke;
        Hu3DParticleHookSet(id, fn_1_8E78);
        Hu3DParticleColSet(id, 0xFF, 0xFF, 0xFF);
        Hu3DParticleTPLvlSet(id, 0.0f);
        Hu3DParticleBlendModeSet(id, 1);
        Hu3DModelLayerSet(id, 5);
        Hu3DModelCameraSet(id, 1);
        Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
        smoke->pos.x = 0.0f;
        smoke->pos.y = 0.0f;
        smoke->pos.z = 0.0f;
        smoke->vec.x = 0.0f;
        smoke->vec.y = 0.0f;
        smoke->vec.z = 0.0f;
        smoke->modelId = id;
        p = smoke->particle->data;
        for (i = 0; i < (s16)smoke->particle->maxCnt; i++, p++) {
            M513_SMOKE_INIT(smoke, p);
            p->time = 0xFF;
        }
    }
    if (self->smoke != smoke) {
        M513Smoke *old = self->smoke;

        if (old != 0) {
            ((M513TaskVtbl *)old->vtbl)->dtor((M513Task *)old, 1);
        }
        self->smoke = smoke;
    }
    return self;
}

M513PowerEff **fn_1_9EA4(M513PowerEff **ptr, s16 flag)
{
    if (ptr != 0) {
        M513PowerEff *obj = *ptr;

        if (obj != 0) {
            ((M513TaskVtbl *)obj->vtbl)->dtor((M513Task *)obj, 1);
        }
        if (flag > 0) {
            fn_1_C8(ptr);
        }
    }
    return ptr;
}

M513PowerEff *fn_1_9F20(M513PowerEff *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_34C;
        Hu3DModelKill(self->hookModel);
        if ((void *)&self->smoke != 0) {
            M513Smoke *old = self->smoke;

            if (old != 0) {
                ((M513TaskVtbl *)old->vtbl)->dtor((M513Task *)old, 1);
            }
        }
        if (self != 0) {
            self->vtbl = (void **)&lbl_1_data_218;
            if (self != 0) {
                fn_1_1774(&self->link, 0);
            }
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// Start the effect: from `pos` towards `target`, holding for `limit` seconds.
void fn_1_9FE4(M513PowerEff *self, Vec *pos, Vec *target, f32 limit)
{
    self->timer = 0.0f;
    self->state = 0;
    self->pos.x = pos->x;
    self->pos.y = pos->y;
    self->pos.z = pos->z;
    self->target.x = target->x;
    self->target.y = target->y;
    self->target.z = target->z;
    self->limit = limit;
    self->frame = 0;
    self->scale = 0.0f;
    self->col[2] = 0xFF;
    self->col[1] = 0xFF;
    self->col[0] = 0xFF;
    self->col[3] = 0;
}

void fn_1_A064(M513PowerEff *self, s32 bit)
{
    self->flags &= ~(1 << bit);
}

// Virtual update(): fade in, follow the target, fade out.
void fn_1_A07C(M513PowerEff *self)
{
    Vec d;
    M513Smoke *smoke;

    switch (self->state) {
        case 0: {
            M513Smoke *sm;
            HU3DPARTICLEDATA *p;
            s32 i;

            self->timer = self->timer + 0.016666666666666666;
            if (self->limit > self->timer) {
                goto move;
            }
            sm = self->smoke;
            p = sm->particle->data;
            for (i = 0; i < (s16)sm->particle->maxCnt; i++, p++) {
                M513_SMOKE_INIT(sm, p);
            }
            sm->pos.x = self->pos.x;
            sm->pos.y = self->pos.y;
            sm->pos.z = self->pos.z;
            sm->active = 1;
            self->state++;
        }
        // fall through
        case 1:
            self->col[3] = self->col[3] + 0x10;
            if (self->col[3] < 0xDC) {
                goto move;
            }
            self->state++;
            // fall through
        case 2:
            PSVECSubtract(&self->pos, &self->target, &d);
            if (PSVECMag(&d) > 50.0f) {
                goto move;
            }
            self->state++;
            // fall through
        case 3:
            self->col[3] = self->col[3] - 8;
            if (self->col[3] > 8) {
                goto move;
            }
            self->col[3] = 0;
            self->state++;
            // fall through
        case 4: {
            M513Smoke *sm = self->smoke;
            HU3DPARTICLEDATA *p;
            s32 i;

            p = sm->particle->data;
            for (i = 0; i < (s16)sm->particle->maxCnt; i++, p++) {
                p->color.a = 0;
                p->time = 0xFF;
            }
            sm->active = 0;
            self->state++;
            return;
        }
        default:
        move:
            if (self->state > 0) {
                Vec v;

                v.x = self->pos.x;
                v.y = self->pos.y;
                v.z = self->pos.z;
                PSVECSubtract(&self->target, &v, &v);
                PSVECScale(&v, &v, 0.025000002f);
                PSVECAdd(&v, &self->pos, &self->pos);
                self->scale = self->scale + ((1.5f - self->scale) / 48.0);
                self->frame++;
                if (self->frame > 0x1F) {
                    self->frame = 0;
                }
            }
            smoke = self->smoke;
            smoke->pos.x = self->pos.x;
            smoke->pos.y = self->pos.y;
            smoke->pos.z = self->pos.z;
            return;
    }
}

// Draw hook: a glowing quad (distorted through an indirect texture).
void fn_1_A528(M513PowerEff *self, Mtx mtx)
{
    Mtx inv;
    Mtx trans;
    Mtx lightMtx;
    Mtx texMtx;
    Mtx scaleMtx;
    f32 size;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    HU3DCAMERA *cam;

    if ((self->state == 0) || (self->state > 4)) {
        return;
    }
    if (self->col[3] != 0) {
        PSMTXInverse(mtx, inv);
        inv[0][3] = 0.0f;
        inv[1][3] = 0.0f;
        inv[2][3] = 0.0f;
        PSMTXTrans(trans, self->pos.x, self->pos.y, self->pos.z);
        PSMTXConcat(trans, inv, trans);
        PSMTXConcat(mtx, trans, mtx);
        GXLoadPosMtxImm(mtx, 0);
        M513_ASSERT(lbl_1_bss_9F0 != 0, lbl_1_data_330, lbl_1_data_340, 0x27);
        GXLoadTexObj(&lbl_1_bss_9F0->texObj, GX_TEXMAP0);
        HuSprTexLoad(lbl_1_bss_9F0->subAnim[0], 0, 1, GX_CLAMP, GX_CLAMP, GX_LINEAR);
        HuSprTexLoad(lbl_1_bss_9F0->sprAnim[self->frame], 0, 2, GX_REPEAT, GX_REPEAT, GX_LINEAR);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
        GXSetZCompLoc(GX_TRUE);
        GXSetAlphaUpdate(GX_TRUE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT0, GX_DF_CLAMP, GX_AF_SPOT);
        GXSetNumTevStages(3);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_ZERO, GX_CC_TEXA, GX_CC_CPREV);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_KONST, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetNumTexGens(3);
        cam = &Hu3DCamera[Hu3DCameraNo];
        C_MTXLightPerspective(lightMtx, cam->fov, cam->aspect, 0.5f, -0.5f, 0.5f, 0.5f);
        PSMTXConcat(lightMtx, mtx, texMtx);
        GXLoadTexMtxImm(texMtx, 0x1E, GX_MTX3x4);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, 0x1E, GX_FALSE, 0x7D);
        GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, 0x7D);
        GXSetNumIndStages(1);
        PSMTXTrans(trans, 0.0f, 0.0f, 0.0f);
        PSMTXScale(scaleMtx, 1.0f, 1.0f, 1.0f);
        PSMTXConcat(scaleMtx, trans, texMtx);
        GXLoadTexMtxImm(texMtx, 0x21, GX_MTX2x4);
        GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, 0x21, GX_FALSE, 0x7D);
        GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD2, GX_TEXMAP2);
        GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
        GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_0);
        GXSetIndTexMtx(GX_ITM_0, (f32 (*)[3])lbl_1_rodata_220, 0);
        GXSetCullMode(GX_CULL_BACK);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_TEX_ST, GX_RGBA8, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_RGBA6, 0);
        size = 40.0f;
        if (self->state > 0) {
            size = 40.0f * self->scale;
        }
        r = self->col[0];
        g = self->col[1];
        b = self->col[2];
        a = self->col[3];
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-size, -size, 0.0f);
        GXColor4u8(r, g, b, a);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(-size, size, 0.0f);
        GXColor4u8(r, g, b, a);
        GXTexCoord2f32(0.0f, 1.0f);
        GXPosition3f32(size, size, 0.0f);
        GXColor4u8(r, g, b, a);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(size, -size, 0.0f);
        GXColor4u8(r, g, b, a);
        GXTexCoord2f32(1.0f, 0.0f);
        GXEnd();
        GXSetNumIndStages(0);
        GXSetTevDirect(GX_TEVSTAGE0);
        GXSetTevDirect(GX_TEVSTAGE1);
        GXSetTevDirect(GX_TEVSTAGE2);
        GXSetTexCoordScaleManually(GX_TEXCOORD0, GX_FALSE, 0, 0);
        GXSetTexCoordScaleManually(GX_TEXCOORD1, GX_FALSE, 0, 0);
        GXSetTexCoordScaleManually(GX_TEXCOORD2, GX_FALSE, 0, 0);
    }
}

}
