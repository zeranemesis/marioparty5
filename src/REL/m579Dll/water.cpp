#include "dolphin.h"
#include "game/data.h"
#include "game/hu3d.h"
#include "game/sprite.h"
#include "REL/m579Dll/m579types.h"

/* Water singleton: spray/ripple sprites and the particle managers. */
typedef struct M579WaterMgr {
    M579Base base;
    ANIMDATA *anim[3];
} M579WaterMgr;

extern "C" {
void *__register_global_object(void *object, void *destructor, void *regmem);
void GXSetVtxAttrFmtv(GXVtxFmt vtxfmt, GXVtxAttrFmtList *list);
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);
void fn_1_2CDC(s16 *model, HU3DMODELHOOK hook);
void fn_1_30BC(s16 *model, void *hookData);
f32 fn_1_4FC8(f32 lo, f32 hi);
extern M579WaterMgr *lbl_1_bss_A0C;
extern u8 lbl_1_bss_C[];
extern u8 lbl_1_data_1FC[];

void *fn_1_97CC(M579WaterMgr *self);
M579WaterMgr *fn_1_98E0(M579WaterMgr *self, s16 flag);
M579WaterMgr *fn_1_996C(M579WaterMgr *self, s16 flag);
ANIMDATA *fn_1_9A70(s32 idx);
void fn_1_9AF0(M579WaterMgr *self, s32 bit);
void fn_1_9B08(void);
M579Spray *fn_1_9B30(M579Spray *self, Vec *pos);
M579Spray *fn_1_9C40(M579Spray *self, s16 flag);
M579Spray *fn_1_9C98(M579Spray *self, s16 flag);
void fn_1_9D00(void);
void fn_1_9DD0(void);
void fn_1_9E00(M579Spray *self);
void fn_1_9E80(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
M579Spray *fn_1_A084(M579Spray *self, Vec *pos);
M579Spray *fn_1_A1AC(M579Spray *self, s16 flag);
M579Spray *fn_1_A204(M579Spray *self, s16 flag);
void fn_1_A26C(void);
void fn_1_A33C(void);
void fn_1_A36C(M579Spray *self);
void fn_1_A3EC(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
M579Ripple *fn_1_A680(M579Ripple *self, Vec *pos, f32 maxScale);
M579Ripple *fn_1_A754(M579Ripple *self, s16 flag);
M579Ripple *fn_1_A7AC(M579Ripple *self, s16 flag);
void fn_1_A818(HU3DMODEL *modelP, Mtx *mtx);
void fn_1_A850(void);
void fn_1_A924(void);
void fn_1_A954(M579Ripple *self);
void fn_1_A97C(M579Ripple *self);
void fn_1_AA44(M579Ripple *self, Mtx mtx);
void fn_1_AEA0(M579Node *list, void (*fn)(M579Spray *));
void fn_1_AF00(M579Node *list, void (*fn)(M579Spray *));
void fn_1_AF60(M579Node *list, void (*fn)(M579Ripple *));
M579Ripple *fn_1_B050(M579Ripple *self, s16 flag);
void fn_1_B0A8(M579Node *first, M579Node *list, void (*fn)(M579Spray *));
void fn_1_B108(M579Node *first, M579Node *list, void (*fn)(M579Spray *));
void fn_1_B168(void);
M579Spray *fn_1_B218(M579Spray *self, s16 flag);
M579Spray *fn_1_B270(M579Spray *self, s16 flag);
}

char lbl_1_data_320[16] = "ms_pointer == 0";
char lbl_1_data_330[12] = "singleton.h";
char lbl_1_data_33C[16] = "ms_pointer != 0";
void *lbl_1_data_34C[5] = { 0, 0, (void *) fn_1_996C, (void *) fn_1_9AF0, (void *) fn_1_9B08 };

u8 lbl_1_bss_A48[12];
M579Node lbl_1_bss_A54;
u8 lbl_1_bss_A5C[12];
M579Node lbl_1_bss_A68;
u8 lbl_1_bss_A70[12];
M579Node lbl_1_bss_A7C;

M579_RODATA const s32 lbl_1_rodata_280[3] = { 0x850003, 0x850004, 0x850005 };
M579_RODATA const Vec lbl_1_rodata_2B4 = { 0.0f, 0.0f, 75.0f };
M579_RODATA const GXColor lbl_1_rodata_2C0[8] = {
    { 0xFF, 0xFF, 0xFF, 0xFF }, { 0xFF, 0xFF, 0xC0, 0xFF }, { 0xFF, 0xC0, 0xFF, 0xFF }, { 0xFF, 0xC0, 0xC0, 0xFF },
    { 0xC0, 0xFF, 0xFF, 0xFF }, { 0xC0, 0xFF, 0xC0, 0xFF }, { 0xC0, 0xC0, 0xFF, 0xFF }, { 0xFF, 0xFF, 0xFF, 0xFF }
};
M579_RODATA const GXColor lbl_1_rodata_330 = { 0, 0, 0, 0 };
M579_RODATA const GXVtxDescList lbl_1_rodata_334[3] = {
    { GX_VA_POS, GX_DIRECT }, { GX_VA_TEX0, GX_DIRECT }, { GX_VA_NULL, GX_NONE }
};
M579_RODATA const GXVtxAttrFmtList lbl_1_rodata_34C[3] = {
    { GX_VA_POS, GX_POS_XYZ, GX_F32, 0 }, { GX_VA_TEX0, GX_TEX_ST, GX_F32, 0 }, { GX_VA_NULL, GX_POS_XY, GX_U8, 0 }
};

void *fn_1_97CC(M579WaterMgr *self)
{
    s32 i;

    fn_1_1264(self);
    self->base.vtbl = (void **) lbl_1_data_1FC;
    self->base.flags = 0;
    fn_1_1330(self, lbl_1_bss_C);
    lbl_1_bss_A0C == 0 ? (void) 0 : fn_1_1D4(lbl_1_data_320, lbl_1_data_330, 0x35);
#ifdef TARGET_PC
    lbl_1_bss_A0C = self;
#else
    {
        u8 *q = (u8 *) self + 0x10;
        if (q != 0) {
            q -= 0x10;
        }
        lbl_1_bss_A0C = (M579WaterMgr *) q;
    }
#endif
    self->base.vtbl = lbl_1_data_34C;
    for (i = 0; i < 3; i++) {
        HuDataSelHeapReadNum(lbl_1_rodata_280[i], 0x10000000, HEAP_MODEL);
        self->anim[i] = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_rodata_280[i], 0x10000000, HEAP_MODEL));
        HuSprAnimLock(self->anim[i]);
    }
    return self;
}

M579WaterMgr *fn_1_98E0(M579WaterMgr *self, s16 flag)
{
    if (self != 0) {
        lbl_1_bss_A0C != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_33C, lbl_1_data_330, 0x3D);
        lbl_1_bss_A0C = 0;
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M579WaterMgr *fn_1_996C(M579WaterMgr *self, s16 flag)
{
    s32 i;

    if (self != 0) {
        self->base.vtbl = lbl_1_data_34C;
        fn_1_9D00();
        fn_1_A26C();
        fn_1_A850();
        for (i = 0; i < 3; i++) {
            HuSprAnimKill(self->anim[i]);
        }
        if (self != 0) {
            lbl_1_bss_A0C != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_33C, lbl_1_data_330, 0x3D);
            lbl_1_bss_A0C = 0;
        }
        if (self != 0) {
            self->base.vtbl = (void **) lbl_1_data_1FC;
            if (self != 0) {
                fn_1_1270(self, 0);
            }
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

ANIMDATA *fn_1_9A70(s32 idx)
{
    s32 i;

    i = idx;
    lbl_1_bss_A0C != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_33C, lbl_1_data_330, 0x27);
    return lbl_1_bss_A0C->anim[i];
}

void fn_1_9AF0(M579WaterMgr *self, s32 bit)
{
    self->base.flags = self->base.flags & ~(1 << bit);
}

void fn_1_9B08(void)
{
    fn_1_9DD0();
    fn_1_A33C();
    fn_1_A924();
}

M579Spray *fn_1_9B30(M579Spray *self, Vec *pos)
{
    fn_1_1264(self);
    fn_1_1330(self, &lbl_1_bss_A54);
    lbl_1_bss_A0C != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_33C, lbl_1_data_330, 0x27);
    self->model = Hu3DParticleCreate(lbl_1_bss_A0C->anim[0], 1);
    Hu3DModelPosSetV(self->model, pos);
    Hu3DModelLayerSet(self->model, 5);
    Hu3DParticleScaleSet(self->model, 100.0f);
    Hu3DParticleHookSet(self->model, fn_1_9E80);
    Hu3DParticleTPLvlSet(self->model, 0.0f);
    Hu3DParticleBlendModeSet(self->model, 0);
    self->count = 0;
    return self;
}

M579Spray *fn_1_9C40(M579Spray *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M579Spray *fn_1_9C98(M579Spray *self, s16 flag)
{
    if (self != 0) {
        Hu3DModelKill(self->model);
        if (self != 0) {
            fn_1_1270(self, 0);
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_9D00(void)
{
    M579Spray *p;
    u8 empty;

    for (;;) {
        empty = 0;
        if (lbl_1_bss_A54.next == &lbl_1_bss_A54 && lbl_1_bss_A54.next == lbl_1_bss_A54.prev) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        p = (M579Spray *) lbl_1_bss_A54.prev;
        if (p != 0) {
            Hu3DModelKill(p->model);
            if (p != 0) {
                fn_1_1270(p, 0);
            }
            fn_1_C8(p);
        }
    }
}

void fn_1_9DD0(void)
{
    fn_1_AEA0(&lbl_1_bss_A54, fn_1_9E00);
}

void fn_1_9E00(M579Spray *self)
{
    if (((HU3DPARTICLE *) Hu3DData[self->model].hookData)->dataCnt < 0 && self != 0) {
        Hu3DModelKill(self->model);
        if (self != 0) {
            fn_1_1270(self, 0);
        }
        fn_1_C8(self);
    }
}

void fn_1_9E80(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *end;
    HU3DPARTICLEDATA *p;
    HU3DPARTICLEDATA *q;
    s16 n;
    s32 g;
    s32 b;
    s32 a;

    end = &particleP->data[particleP->maxCnt];
    if (particleP->count == 0) {
        for (p = particleP->data; p != end; p++) {
            p->vel.x = 2.5f * fn_1_4FC8(-1.0f, 1.0f);
            p->vel.y = 2.5f * fn_1_4FC8(-1.0f, 1.0f);
            p->vel.z = 2.5f * fn_1_4FC8(-1.0f, 1.0f);
            p->pos.z = 0.0f;
            p->pos.y = 0.0f;
            p->pos.x = 0.0f;
            p->color.r = 0x80;
            p->color.g = 0xFF;
            p->color.b = 0xFF;
            p->color.a = 0x80;
        }
    }
    n = 0;
    for (q = particleP->data; q != end; q++) {
        q->vel.y = (f32) ((f64) q->vel.y - 0.27240694444444447);
        PSVECScale(&q->vel, &q->vel, 0.8f);
        PSVECAdd(&q->vel, &q->pos, &q->pos);
        q->color.r = 0xFF;
        g = q->color.g + 0x20;
        if (g > 0xFF) {
            g = 0xFF;
        }
        q->color.g = (u8) g;
        b = q->color.b + 0x20;
        if (b > 0xFF) {
            b = 0xFF;
        }
        q->color.b = (u8) b;
        a = q->color.a - 8;
        if (a < 0) {
            a = 0;
            q->scale = 0.0f;
            particleP->dataCnt = -1;
        }
        q->color.a = (u8) a;
        n += 1;
    }
    particleP->emitCnt = n;
}

M579Spray *fn_1_A084(M579Spray *self, Vec *pos)
{
    fn_1_1264(self);
    fn_1_1330(self, &lbl_1_bss_A68);
    lbl_1_bss_A0C != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_33C, lbl_1_data_330, 0x27);
    self->model = Hu3DParticleCreate(lbl_1_bss_A0C->anim[2], 1);
    Hu3DModelPosSetV(self->model, pos);
    Hu3DModelLayerSet(self->model, 5);
    Hu3DParticleScaleSet(self->model, 30.0f + 50.0f * fn_1_4D1C());
    Hu3DParticleHookSet(self->model, fn_1_A3EC);
    Hu3DParticleTPLvlSet(self->model, 0.0f);
    Hu3DParticleBlendModeSet(self->model, 0);
    self->count = 0;
    return self;
}

M579Spray *fn_1_A1AC(M579Spray *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M579Spray *fn_1_A204(M579Spray *self, s16 flag)
{
    if (self != 0) {
        Hu3DModelKill(self->model);
        if (self != 0) {
            fn_1_1270(self, 0);
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_A26C(void)
{
    M579Spray *p;
    u8 empty;

    for (;;) {
        empty = 0;
        if (lbl_1_bss_A68.next == &lbl_1_bss_A68 && lbl_1_bss_A68.next == lbl_1_bss_A68.prev) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        p = (M579Spray *) lbl_1_bss_A68.prev;
        if (p != 0) {
            Hu3DModelKill(p->model);
            if (p != 0) {
                fn_1_1270(p, 0);
            }
            fn_1_C8(p);
        }
    }
}

void fn_1_A33C(void)
{
    fn_1_AF00(&lbl_1_bss_A68, fn_1_A36C);
}

void fn_1_A36C(M579Spray *self)
{
    if (((HU3DPARTICLE *) Hu3DData[self->model].hookData)->dataCnt < 0 && self != 0) {
        Hu3DModelKill(self->model);
        if (self != 0) {
            fn_1_1270(self, 0);
        }
        fn_1_C8(self);
    }
}

void fn_1_A3EC(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    Mtx rx;
    Mtx ry;
    Vec v;
    HU3DPARTICLEDATA *end;
    HU3DPARTICLEDATA *p;
    HU3DPARTICLEDATA *q;
    s32 idx;
    s32 a;

    end = &particleP->data[particleP->maxCnt];
    if (particleP->count == 0) {
        for (p = particleP->data; p != end; p++) {
            PSMTXRotRad(rx, 'x', 0.017453292f * ((180.0f * fn_1_4D1C()) - 90.0f));
            PSMTXRotRad(ry, 'y', 0.017453292f * ((360.0f * fn_1_4D1C()) - 180.0f));
            PSMTXConcat(ry, rx, rx);
            PSMTXMultVec(rx, &lbl_1_rodata_2B4, &v);
            p->vel.x = v.x * 0.016666668f;
            p->vel.y = v.y * 0.016666668f;
            p->vel.z = v.z * 0.016666668f;
            p->pos.x = v.x;
            p->pos.y = v.y;
            p->pos.z = v.z;
            idx = fn_1_4AE0() & 7;
            p->color.r = lbl_1_rodata_2C0[idx].r;
            p->color.g = lbl_1_rodata_2C0[idx].g;
            p->color.b = lbl_1_rodata_2C0[idx].b;
            p->color.a = lbl_1_rodata_2C0[idx].a;
            p->zRot = (f32) ((3.141592653589793 * (f64) (360.0f * fn_1_4D1C())) / 180.0);
        }
    }
    for (q = particleP->data; q != end; q++) {
        PSVECAdd(&q->vel, &q->pos, &q->pos);
        a = q->color.a - particleP->emitCnt;
        if (a < 0) {
            a = 0;
            q->scale = 0.0f;
            particleP->dataCnt = -1;
        }
        q->color.a = (u8) a;
        q->zRot = (f32) ((3.141592653589793 * (5.0 + (180.0 * ((f64) q->zRot / 3.141592653589793)))) / 180.0);
    }
    particleP->emitCnt = (s16) (particleP->emitCnt + 1);
}

M579Ripple *fn_1_A680(M579Ripple *self, Vec *pos, f32 maxScale)
{
    fn_1_1264(self);
    fn_1_2CDC(&self->model, fn_1_A818);
    self->pos.x = pos->x;
    self->pos.y = pos->y;
    self->pos.z = pos->z;
    self->scale = (f32) (0.1 * (f64) maxScale);
    self->maxScale = maxScale;
    self->alpha = 0xFF;
    fn_1_1330(self, &lbl_1_bss_A7C);
    fn_1_30BC(&self->model, self);
    fn_1_304C(&self->model, 1);
    return self;
}

M579Ripple *fn_1_A754(M579Ripple *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M579Ripple *fn_1_A7AC(M579Ripple *self, s16 flag)
{
    if (self != 0) {
        fn_1_30F4(&self->model, 0);
        if (self != 0) {
            fn_1_1270(self, 0);
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_A818(HU3DMODEL *modelP, Mtx *mtx)
{
    fn_1_AA44((M579Ripple *) modelP->hookData, *mtx);
}

void fn_1_A850(void)
{
    M579Ripple *p;
    u8 empty;

    for (;;) {
        empty = 0;
        if (lbl_1_bss_A7C.next == &lbl_1_bss_A7C && lbl_1_bss_A7C.next == lbl_1_bss_A7C.prev) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        p = (M579Ripple *) lbl_1_bss_A7C.prev;
        if (p != 0) {
            fn_1_30F4(&p->model, 0);
            if (p != 0) {
                fn_1_1270(p, 0);
            }
            fn_1_C8(p);
        }
    }
}

void fn_1_A924(void)
{
    fn_1_AF60(&lbl_1_bss_A7C, fn_1_A954);
}

void fn_1_A954(M579Ripple *self)
{
    fn_1_A97C(self);
}

void fn_1_A97C(M579Ripple *self)
{
    f32 s;

    self->alpha = (u8) (255.0 * (1.0 - (f64) (self->scale / self->maxScale)));
    s = (f32) ((f64) self->scale + ((f64) self->maxScale / 48.0));
    self->scale = s;
    if (s >= self->maxScale && self != 0) {
        fn_1_30F4(&self->model, 0);
        if (self != 0) {
            fn_1_1270(self, 0);
        }
        fn_1_C8(self);
    }
}

void fn_1_AA44(M579Ripple *self, Mtx mtx)
{
    Mtx m;
    GXColor c;

    PSMTXScale(m, self->scale, 1.0f, self->scale);
    PSMTXTransApply(m, m, self->pos.x, -150.0f, self->pos.z);
    PSMTXConcat(mtx, m, m);
    GXLoadPosMtxImm(m, 0);
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR_NULL, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    lbl_1_bss_A0C != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_33C, lbl_1_data_330, 0x27);
    HuSprTexLoad(lbl_1_bss_A0C->anim[1], 0, 0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    PSMTXIdentity(m);
    GXLoadTexMtxImm(m, 0x1E, GX_MTX2x4);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1E, GX_FALSE, 0x7D);
    c = lbl_1_rodata_330;
    c.a = self->alpha;
    GXSetTevColor(GX_TEVREG0, c);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ONE);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetCullMode(GX_CULL_BACK);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZCompLoc(GX_TRUE);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_LEQUAL, 0xFF);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXClearVtxDesc();
    GXSetVtxDescv((GXVtxDescList *) lbl_1_rodata_334);
    GXSetVtxAttrFmtv(GX_VTXFMT0, (GXVtxAttrFmtList *) lbl_1_rodata_34C);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-0.5f, 1.0f, -0.5f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(0.5f, 1.0f, -0.5f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(0.5f, 1.0f, 0.5f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(-0.5f, 1.0f, 0.5f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXEnd();
}

void fn_1_AEA0(M579Node *list, void (*fn)(M579Spray *))
{
    M579Node *l;
    M579Node *first;
    M579Node *n;

    l = list;
    first = l->next;
    n = first;
    while (n != l) {
        fn((M579Spray *) n);
        n = n->next;
    }
}

void fn_1_AF00(M579Node *list, void (*fn)(M579Spray *))
{
    M579Node *l;
    M579Node *first;
    M579Node *n;

    l = list;
    first = l->next;
    n = first;
    while (n != l) {
        fn((M579Spray *) n);
        n = n->next;
    }
}

void fn_1_AF60(M579Node *list, void (*fn)(M579Ripple *))
{
    M579Node tmp;
    u8 empty;
    u8 empty2;

    empty = 0;
    if (list->next == list && list->next == list->prev) {
        empty = 1;
    }
    if (empty == 0) {
        fn_1_1264(&tmp);
        fn_1_1360(list, &tmp);
        for (;;) {
            empty2 = 0;
            if (tmp.next == &tmp && tmp.next == tmp.prev) {
                empty2 = 1;
            }
            if (empty2 != 0) {
                break;
            }
            fn_1_12DC(tmp.next);
            fn_1_1330(tmp.next, list);
            fn((M579Ripple *) tmp.next);
        }
        fn_1_1270(&tmp, 0);
    }
}

M579Ripple *fn_1_B050(M579Ripple *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_B0A8(M579Node *first, M579Node *list, void (*fn)(M579Spray *))
{
    M579Node *n;

    n = first;
    while (n != list) {
        fn((M579Spray *) n);
        n = n->next;
    }
}

void fn_1_B108(M579Node *first, M579Node *list, void (*fn)(M579Spray *))
{
    M579Node *n;

    n = first;
    while (n != list) {
        fn((M579Spray *) n);
        n = n->next;
    }
}

void fn_1_B168(void)
{
    M579Node *a;

    a = &lbl_1_bss_A54;
    fn_1_1264(a);
#ifndef TARGET_PC
    __register_global_object(a, (void *) fn_1_B270, lbl_1_bss_A48);
#endif
    a = &lbl_1_bss_A68;
    fn_1_1264(a);
#ifndef TARGET_PC
    __register_global_object(a, (void *) fn_1_B218, lbl_1_bss_A5C);
#endif
    a = &lbl_1_bss_A7C;
    fn_1_1264(a);
#ifndef TARGET_PC
    __register_global_object(a, (void *) fn_1_B050, lbl_1_bss_A70);
#endif
}

M579Spray *fn_1_B218(M579Spray *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M579Spray *fn_1_B270(M579Spray *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}
