// m517Dll papereff: the paper-sheet effect. A texture holder singleton (four
// player-specific animation pairs), a particle emitter object and the sheet
// itself, a 9x9 vertex grid that is waved and drawn as a double-sided mesh.
#define _MATH_H
#include "REL/m517Dll.h"
extern "C" {
#include "game/frand.h"
#include "game/audio.h"
#include "game/gamework.h"
}

#ifdef __MWERKS__
#define M517_RODATA __declspec(section ".rodata")
#else
#define M517_RODATA
#endif

extern "C" {

double sin(double);
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT,
                  GXTexFilter filter);
/* Texture holder singleton: per player the paper texture and its second layer. */
typedef struct M517PaperTex_s {
    ANIMDATA *a[4];
    ANIMDATA *b[4];
} M517PaperTex;

/* Particle emitter task. */
typedef struct M517PaperPart_s {
    M517Task task;
    HU3DMODELID id;
    ANIMDATA *anim;
    HU3DPARTICLE *particle;
    Vec pos;
    Vec vec;
} M517PaperPart;

#define M517_PAPER_GRID 9

/* The sheet task. */
typedef struct M517PaperEff_s {
    M517Task task;
    HU3DMODELID hook;
    ANIMDATA *a;
    ANIMDATA *b;
    f32 timer;
    Vec pos;
    Vec target;
    u8 pad38[0xC];
    Vec vec44;
    Vec rot;
    u8 landed;
    Vec grid[M517_PAPER_GRID][M517_PAPER_GRID];
    u8 pad42C[0x30];
    s32 state;
    f32 w;
    f32 pad464;
    f32 h;
    s32 snd;
    M517PaperPart *part;
} M517PaperEff;

#define M517_PAPER_TEX() ((M517PaperTex *)lbl_1_bss_8)

void fn_1_13590(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx);
M517PaperPart *fn_1_135D4(M517PaperPart *self, Vec *pos);
M517PaperPart *fn_1_1379C(M517PaperPart *self, s16 flag);
void fn_1_13838(M517PaperPart *self, s32 bit);
void fn_1_13850(M517PaperPart *self, HU3DPARTICLEDATA *p);
void fn_1_13A70(M517PaperPart *self, Mtx mtx);
void fn_1_13F44(M517PaperPart *self, Vec *src);
void fn_1_13F60(M517PaperPart *self, Vec *src);
M517PaperTex *fn_1_13F7C(M517PaperTex *self);
void fn_1_140D0(M517PaperTex *self, s16 flag);
M517PaperTex *fn_1_1415C(M517PaperTex *self, s16 flag);
ANIMDATA *fn_1_1424C(M517PaperTex *self, s32 idx);
ANIMDATA *fn_1_14258(M517PaperTex *self, s32 idx);
void fn_1_14268(HU3DMODEL *model, Mtx mtx);
M517PaperEff *fn_1_142AC(M517PaperEff *self, Vec *pos, Vec *target, Vec *rot, s32 idx);
void fn_1_14774(M517Task **self, s16 flag);
M517PaperEff *fn_1_147F0(M517PaperEff *self, s16 flag);
void fn_1_14904(M517PaperEff *self, s32 bit);
void fn_1_1491C(M517PaperEff *self);
void fn_1_14B1C(M517PaperEff *self, Mtx mtx);
void fn_1_14E6C(M517PaperEff *self);
void fn_1_14F90(M517PaperEff *self, s32 state);
void fn_1_15104(M517PaperEff *self, u8 back);

char lbl_1_data_678[] = "ms_pointer == 0";
char lbl_1_data_688[] = "singleton.h";
char lbl_1_data_694[] = "ms_pointer != 0";
void *lbl_1_data_6A4[5] = { 0, 0, (void *)fn_1_147F0, (void *)fn_1_14904, (void *)fn_1_1491C };
void *lbl_1_data_6B8[6] = { 0, 0, (void *)fn_1_1379C, (void *)fn_1_13838, (void *)fn_1_1E80, 0 };

M517_RODATA const s32 lbl_1_rodata_4C8[13] = { 0x4B0014, 0x4B0016, 0x4B0018, 0x4B001A, 0x4B001C, 0x4B001E, 0x4B0020,
                                               0x4B0022, 0x4B0024, 0x4B0026, 0x4B0028, 0x4B002A, 0x4B002C };
M517_RODATA const s32 lbl_1_rodata_4FC[13] = { 0x4B0015, 0x4B0017, 0x4B0019, 0x4B001B, 0x4B001D, 0x4B001F, 0x4B0021,
                                               0x4B0023, 0x4B0025, 0x4B0027, 0x4B0029, 0x4B002B, 0x4B002D };
M517_RODATA const GXColor lbl_1_rodata_560 = { 0xFF, 0xFF, 0xFF, 0x99 };

// Restart one smoke particle around the emitter.
#define M517_PAPER_PART_INIT(self, p)                                                              \
    {                                                                                              \
        u32 col_;                                                                                  \
        (p)->time = 0;                                                                             \
        (p)->parManId = (frand() & 0xF) + 4;                                                       \
        (p)->pos.x = (self)->pos.x + (50.0f * (0.000030517578f * ((f32)(u16)frand() - 32768.0f))); \
        (p)->pos.y = (self)->pos.y;                                                                \
        (p)->pos.z = (self)->pos.z + (50.0f * (0.000030517578f * ((f32)(u16)frand() - 32768.0f))); \
        (p)->scale = 100.0f * (0.2f + (0.2f * (0.000015258789f * (f32)(u16)frand())));             \
        (p)->zRot = 0.017453292f * (360.0f * (0.000015258789f * (f32)(u16)frand()));               \
        col_ = frand();                                                                            \
        (p)->color.r = (u8)col_;                                                                   \
        (p)->color.g = (u8)(col_ >> 8);                                                            \
        (p)->color.b = (u8)(col_ >> 16);                                                           \
        (p)->color.a = 0;                                                                          \
    }

// Particle hook: forward to the emitter that owns the particle.
void fn_1_13590(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx)
{
    fn_1_13A70((M517PaperPart *)particle->work, mtx);
}

M517PaperPart *fn_1_135D4(M517PaperPart *self, Vec *pos)
{
    HU3DMODELID id;

    fn_1_2DA8(&self->task.link);
    self->task.vtbl = &lbl_1_data_68.vtbl;
    self->task.flags = 0;
    fn_1_2E74(&self->task.link, &lbl_1_bss_24);
    self->task.vtbl = (M517TaskVtbl *)lbl_1_data_6B8;
    self->anim = HuSprAnimRead(HuDataSelHeapReadNum(0x4B0012, 0x10000000, HEAP_MODEL));
    HuSprAnimLock(self->anim);
    id = Hu3DParticleCreate(self->anim, 10);
    self->particle = (HU3DPARTICLE *)Hu3DData[id].hookData;
    self->particle->work = self;
    Hu3DParticleHookSet(id, fn_1_13590);
    Hu3DParticleColSet(id, 0xFF, 0xFF, 0xFF);
    Hu3DParticleTPLvlSet(id, 0.0f);
    Hu3DParticleBlendModeSet(id, 1);
    Hu3DModelLayerSet(id, 5);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    self->pos.x = pos->x;
    self->pos.y = pos->y;
    self->pos.z = pos->z;
    self->vec.x = 0.0f;
    self->vec.y = 0.0f;
    self->vec.z = 0.0f;
    self->id = id;
    return self;
}

M517PaperPart *fn_1_1379C(M517PaperPart *self, s16 flag)
{
    if (self != 0) {
        self->task.vtbl = (M517TaskVtbl *)lbl_1_data_6B8;
        self->particle->work = 0;
        Hu3DModelKill(self->id);
        HuSprAnimKill(self->anim);
        self->task.vtbl = &lbl_1_data_68.vtbl;
        fn_1_2DB4(&self->task.link, 0);
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

void fn_1_13838(M517PaperPart *self, s32 bit)
{
    self->task.flags &= ~(1u << bit);
}

void fn_1_13850(M517PaperPart *self, HU3DPARTICLEDATA *p)
{
    M517_PAPER_PART_INIT(self, p);
}

// Particle hook body: fade the particles in and out and spin them.
void fn_1_13A70(M517PaperPart *self, Mtx mtx)
{
    HU3DPARTICLE *particle = self->particle;
    HU3DPARTICLEDATA *p;
    s32 i;

    if (particle->count == 0) {
        p = particle->data;
        for (i = 0; i < (s16)particle->maxCnt; i++, p++) {
            M517_PAPER_PART_INIT(self, p);
        }
    }
    p = particle->data;
    for (i = 0; i < (s16)particle->maxCnt; i++, p++) {
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
                M517_PAPER_PART_INIT(self, p);
            }
        }
        p->zRot = p->zRot + 0.017453292f;
    }
}

void fn_1_13F44(M517PaperPart *self, Vec *src)
{
    self->pos.x = src->x;
    self->pos.y = src->y;
    self->pos.z = src->z;
}

void fn_1_13F60(M517PaperPart *self, Vec *src)
{
    self->vec.x = src->x;
    self->vec.y = src->y;
    self->vec.z = src->z;
}

// Texture holder constructor: load the animation pair of every human player.
M517PaperTex *fn_1_13F7C(M517PaperTex *self)
{
    s32 i;

    M517_ASSERT(lbl_1_bss_8 == 0, lbl_1_data_678, lbl_1_data_688, 0x35);
    lbl_1_bss_8 = self;
    for (i = 0; i < 4; i++) {
        s16 charNo = GwPlayerConf[i].charNo;

        if (GwPlayerConf[i].grp == 0) {
            self->a[i] = 0;
            self->b[i] = 0;
        } else {
            self->a[i] = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_rodata_4C8[charNo], 0x10000000, HEAP_MODEL));
            HuSprAnimLock(self->a[i]);
            self->b[i] = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_rodata_4FC[charNo], 0x10000000, HEAP_MODEL));
            HuSprAnimLock(self->b[i]);
        }
    }
    return self;
}

// Singleton base destructor.
void fn_1_140D0(M517PaperTex *self, s16 flag)
{
    if (self != 0) {
        M517_ASSERT(lbl_1_bss_8 != 0, lbl_1_data_694, lbl_1_data_688, 0x3D);
        lbl_1_bss_8 = 0;
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
}

M517PaperTex *fn_1_1415C(M517PaperTex *self, s16 flag)
{
    if (self != 0) {
        s32 i;

        for (i = 0; i < 4; i++) {
            if (self->a[i] != 0 && self->b[i] != 0) {
                HuSprAnimKill(self->a[i]);
                HuSprAnimKill(self->b[i]);
            }
        }
        if (self != 0) {
            M517_ASSERT(lbl_1_bss_8 != 0, lbl_1_data_694, lbl_1_data_688, 0x3D);
            lbl_1_bss_8 = 0;
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

ANIMDATA *fn_1_1424C(M517PaperTex *self, s32 idx)
{
    return self->a[idx];
}

ANIMDATA *fn_1_14258(M517PaperTex *self, s32 idx)
{
    return self->b[idx];
}

// Draw hook of the sheet model.
void fn_1_14268(HU3DMODEL *model, Mtx mtx)
{
    fn_1_14B1C((M517PaperEff *)model->hookData, mtx);
}

M517PaperEff *fn_1_142AC(M517PaperEff *self, Vec *pos, Vec *target, Vec *rot, s32 idx)
{
    HU3DMODELID hook;
    M517PaperTex *tex;
    ANIMBMP *bmp;
    M517PaperPart *part;

    fn_1_2DA8(&self->task.link);
    self->task.vtbl = &lbl_1_data_68.vtbl;
    self->task.flags = 0;
    fn_1_2E74(&self->task.link, &lbl_1_bss_24);
    self->task.vtbl = (M517TaskVtbl *)lbl_1_data_6A4;
    self->timer = 0.0f;
    self->landed = 0;
    self->state = 0;
    self->part = 0;
    hook = Hu3DHookFuncCreate((HU3DMODELHOOK)fn_1_14268);
    Hu3DModelLayerSet(hook, 4);
    Hu3DModelCameraSet(hook, 1);
    Hu3DData[hook].hookData = self;
    self->hook = hook;
    M517_ASSERT(lbl_1_bss_8 != 0, lbl_1_data_694, lbl_1_data_688, 0x27);
    tex = M517_PAPER_TEX();
    self->a = tex->a[idx];
    self->b = tex->b[idx];
    self->pos.x = pos->x;
    self->pos.y = pos->y;
    self->pos.z = pos->z;
    self->target.x = target->x;
    self->target.y = target->y;
    self->target.z = target->z;
    self->rot.x = rot->x;
    self->rot.y = rot->y;
    self->rot.z = rot->z;
    bmp = self->a->bmp;
    self->w = 2.0f * ((f32)bmp->sizeX * 0.00390625f);
    self->h = 2.0f * ((f32)bmp->sizeY * 0.00390625f);
    self->vec44.x = 0.0f;
    self->vec44.y = 0.0f;
    self->vec44.z = 0.0f;
    fn_1_14E6C(self);
    part = (M517PaperPart *)fn_1_1E84(sizeof(M517PaperPart));
    if (part != 0) {
        part = fn_1_135D4(part, &self->pos);
    }
    if (self->part != part) {
        M517PaperPart *old = self->part;

        if (old != 0) {
            old->task.vtbl->dtor(&old->task, 1);
        }
        self->part = part;
    }
    self->pos.y = self->pos.y + 30.000002f;
    {
        Vec v;

        v.x = self->pos.x;
        v.y = self->pos.y;
        v.z = self->pos.z;
        self->snd = fn_1_1B6EC(0x6F3, (Point3d *)&v);
    }
    return self;
}

// Smart-pointer holder destructor.
void fn_1_14774(M517Task **self, s16 flag)
{
    if (self != 0) {
        M517Task *obj = *self;

        if (obj != 0) {
            obj->vtbl->dtor(obj, 1);
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
}

M517PaperEff *fn_1_147F0(M517PaperEff *self, s16 flag)
{
    if (self != 0) {
        self->task.vtbl = (M517TaskVtbl *)lbl_1_data_6A4;
        if (self->part != 0) {
            M517PaperPart *old = self->part;

            if (old != 0) {
                old->task.vtbl->dtor(&old->task, 1);
            }
            self->part = 0;
        }
        HuAudFXStop(self->snd);
        Hu3DModelKill(self->hook);
        if ((void *)&self->part != 0) {
            M517PaperPart *old = self->part;

            if (old != 0) {
                old->task.vtbl->dtor(&old->task, 1);
            }
        }
        self->task.vtbl = &lbl_1_data_68.vtbl;
        fn_1_2DB4(&self->task.link, 0);
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

void fn_1_14904(M517PaperEff *self, s32 bit)
{
    self->task.flags &= ~(1u << bit);
}

// Update: the sheet falls, then glides to its target; the state number drives the grid wave.
void fn_1_1491C(M517PaperEff *self)
{
    Vec d;
    f32 dist;

    switch (self->state) {
        case 0:
            self->timer = (f32)((f64)self->timer + 0.016666666666666666);
            self->pos.y = self->pos.y + (-self->pos.y / 60.0f);
            if (self->timer < 1.0f) {
                break;
            }
            self->timer = 0.0f;
            self->state++;
            // fall through
        case 1:
            self->pos.x = self->pos.x + ((self->target.x - self->pos.x) / 120.0f);
            self->pos.y = self->pos.y + ((self->target.y - self->pos.y) / 120.0f);
            self->pos.z = self->pos.z + ((self->target.z - self->pos.z) / 120.0f);
            PSVECSubtract(&self->pos, &self->target, &d);
            dist = PSVECMag(&d);
            self->timer = (f32)((f64)self->timer + 0.016666666666666666);
            if (dist > 200.0f) {
                break;
            }
            self->state++;
            // fall through
        case 2:
            self->landed = 1;
            self->state++;
            break;
    }
    fn_1_14F90(self, self->state);
    self->part->pos.x = self->pos.x;
    self->part->pos.y = self->pos.y;
    self->part->pos.z = self->pos.z;
}

// Draw hook body: both faces of the sheet.
void fn_1_14B1C(M517PaperEff *self, Mtx mtx)
{
    Mtx rot;
    Mtx trans;
    Vec pos;

    pos.x = self->pos.x;
    pos.y = self->pos.y;
    pos.z = self->pos.z;
    PSMTXRotRad(rot, 'y', 0.017453292f * self->rot.y);
    PSMTXTrans(trans, pos.x, pos.y, pos.z);
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(mtx, trans, trans);
    GXLoadPosMtxImm(trans, 0);
    HuSprTexLoad(self->a, 0, 0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    HuSprTexLoad(self->b, 0, 1, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetTevColor(GX_TEVREG0, lbl_1_rodata_560);
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetCullMode(GX_CULL_BACK);
    fn_1_15104(self, 0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetCullMode(GX_CULL_FRONT);
    fn_1_15104(self, 1);
}

// Lay out the flat 9x9 grid of the sheet.
void fn_1_14E6C(M517PaperEff *self)
{
    Vec *v = &self->grid[0][0];
    s32 i;
    s32 j;

    for (i = 0; i < M517_PAPER_GRID; i++) {
        for (j = 0; j < M517_PAPER_GRID; j++, v++) {
            v->x = self->w * (100.0f * ((0.125f * (f32)j) - 0.5f));
            v->y = 0.0f;
            v->z = self->h * (100.0f * ((0.125f * (f32)i) - 0.5f));
        }
    }
}

// Wave the grid while the sheet is gliding (state 1).
void fn_1_14F90(M517PaperEff *self, s32 state)
{
    if (state == 1) {
        Vec *v = &self->grid[0][0];
        f32 phase = 2.0f * self->timer;
        f32 amp = 1.0f - (self->pos.y / self->target.y);
        s32 i;
        s32 j;

        phase = phase * (5.0f * amp);
        for (i = 0; i < M517_PAPER_GRID; i++) {
            for (j = 0; j < M517_PAPER_GRID; j++, v++) {
                f32 ang = (f32)((f64)phase + ((3.141592653589793 * (f64)(i * 20)) / 180.0));

                v->y = (f32)(amp * (50.0 * sin(ang)));
            }
        }
    }
}

// Emit the grid as 8x8 quads; back selects the flipped texture coordinates.
void fn_1_15104(M517PaperEff *self, u8 back)
{
    Vec *grid = &self->grid[0][0];
    s32 i;
    s32 j;

    GXBegin(GX_QUADS, GX_VTXFMT0, 0x100);
    for (i = 0; i < 8; i++) {
        s32 r0 = i * M517_PAPER_GRID;
        s32 r1 = (i + 1) * M517_PAPER_GRID;
        f32 v0;
        f32 v1;

        if (back == 0) {
            v0 = 0.125f * (f32)i;
            v1 = 0.125f * (f32)(i + 1);
        } else {
            v0 = 1.0f - (0.125f * (f32)i);
            v1 = 1.0f - (0.125f * (f32)(i + 1));
        }
        for (j = 0; j < 8; j++) {
            f32 u0 = 0.125f * (f32)j;
            f32 u1 = 0.125f * (f32)(j + 1);
            Vec *a = &grid[r0 + j];
            Vec *b = &grid[r0 + j + 1];
            Vec *c = &grid[r1 + j + 1];
            Vec *d = &grid[r1 + j];

            GXPosition3f32(a->x, a->y, a->z);
            GXTexCoord2f32(u0, v0);
            GXPosition3f32(b->x, b->y, b->z);
            GXTexCoord2f32(u1, v0);
            GXPosition3f32(c->x, c->y, c->z);
            GXTexCoord2f32(u1, v1);
            GXPosition3f32(d->x, d->y, d->z);
            GXTexCoord2f32(u0, v1);
        }
    }
    GXEnd();
}

}
