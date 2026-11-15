// m532Dll "rumble_warn": warning sprite shown when the rumble minigame event
// starts. It is an unk object (see unk.cpp) with a small pointer-to-member state
// machine: grow in, hold for a while, fade out, then delete itself.
#define _MATH_H
extern "C" {
#include "dolphin.h"
#include "game/animdata.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/memory.h"
#include "game/sprite.h"
#include "datadir_enum.h"
}
#include "REL/m532Dll/rumble_warn.h"

#ifdef __MWERKS__
#define M532_CTORS __declspec(section ".ctors")
#define M532_RODATA __declspec(section ".rodata")
#else
#define M532_RODATA
#endif

extern "C" {

void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, s32 wrapS, s32 wrapT, s32 filter);
void GXSetVtxAttrFmtv(GXVtxFmt fmt, const GXVtxAttrFmtList *list);
void fn_1_1AFC(void);

M532RumbleWarn *fn_1_8298(M532RumbleWarn *self, s16 flag);
void fn_1_830C(M532RumbleWarn *self);
void fn_1_8528(M532RumbleWarn *self, Mtx *mtx);
s32 fn_1_8380(M532RumbleWarn *self);
s32 fn_1_8418(M532RumbleWarn *self);
s32 fn_1_8480(M532RumbleWarn *self);
s32 fn_1_84D8(M532RumbleWarn *self);

M532Ptmf lbl_1_data_2E0 = M532_PTMF_INIT(fn_1_84D8);
M532Ptmf lbl_1_data_2EC = M532_PTMF_INIT(fn_1_8418);
M532Ptmf lbl_1_data_2F8 = M532_PTMF_INIT(fn_1_8480);
M532UnkObjVtbl lbl_1_data_304 = { { 0, 0 },
                                  (M532UnkObj * (*)(M532UnkObj *, s16)) fn_1_8298,
                                  (void (*)(M532UnkObj *))fn_1_830C,
                                  (void (*)(M532UnkObj *, Mtx *))fn_1_8528,
                                  (void (*)(M532UnkObj *))fn_1_1AFC };

s32 lbl_1_bss_A08;
s32 lbl_1_bss_A0C;

M532_RODATA const GXColor lbl_1_rodata_1A4 = { 0, 0, 0, 0 };
M532_RODATA const GXVtxDescList lbl_1_rodata_1A8[3] = {
    { GX_VA_POS, GX_DIRECT },
    { GX_VA_TEX0, GX_DIRECT },
    { GX_VA_NULL, GX_NONE },
};
M532_RODATA const GXVtxAttrFmtList lbl_1_rodata_1C0[3] = {
    { GX_VA_POS, GX_POS_XYZ, GX_F32, 0 },
    { GX_VA_TEX0, GX_TEX_ST, GX_F32, 0 },
    { GX_VA_NULL, GX_POS_XY, GX_U8, 0 },
};

M532RumbleWarn *fn_1_8130(M532RumbleWarn *self, Vec *pos)
{
    fn_1_EA08(&self->base);
    self->base.vtbl = &lbl_1_data_304;
    self->state = lbl_1_data_2E0;
    self->anim0 = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m532, 0x18), HU_MEMNUM_OVL, HEAP_MODEL));
    self->anim1 = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m532, 0x19), HU_MEMNUM_OVL, HEAP_MODEL));
    self->sizeX = (f32)self->anim0->bmp->sizeX;
    self->sizeY = (f32)self->anim0->bmp->sizeY;
    self->pos = *pos;
    self->scale = 0.0f;
    self->alpha = 1.0f;
    self->timer = 0;
    fn_1_500C(&self->base.model, 6);
    HuSprAnimLock(self->anim0);
    HuSprAnimLock(self->anim1);
    return self;
}

M532RumbleWarn *fn_1_8298(M532RumbleWarn *self, s16 flag)
{
    if (self != 0) {
        self->base.vtbl = &lbl_1_data_304;
        HuSprAnimKill(self->anim1);
        HuSprAnimKill(self->anim0);
        fn_1_EAF0(&self->base, 0);
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

// Update: run the current state; when it reports it is finished, delete the object.
void fn_1_830C(M532RumbleWarn *self)
{
    if (M532PtmfCall(self, &self->state) == 0 && self != 0) {
        self->base.vtbl->dtor(&self->base, 1);
    }
}

// State: grow in, then switch to the hold state.
s32 fn_1_8380(M532RumbleWarn *self)
{
    self->scale = self->scale + 0.16666667f;
    if (self->scale >= 1.0) {
        self->scale = 1.0f;
        self->state = lbl_1_data_2EC;
    }
    return 1;
}

// State: hold, then switch to the fade-out state.
s32 fn_1_8418(M532RumbleWarn *self)
{
    self->timer = self->timer + 1;
    if (self->timer >= lbl_1_bss_A0C) {
        self->state = lbl_1_data_2F8;
    }
    return 1;
}

// State: fade out; returns 0 when fully transparent.
s32 fn_1_8480(M532RumbleWarn *self)
{
    self->alpha = self->alpha - 0.083333336f;
    if (self->alpha <= 0.0f) {
        return 0;
    }
    return 1;
}

// State: show at full size for lbl_1_bss_A0C frames, then finish.
s32 fn_1_84D8(M532RumbleWarn *self)
{
    self->scale = 1.0f;
    self->alpha = 1.0f;
    self->timer = self->timer + 1;
    return self->timer < lbl_1_bss_A0C;
}

// Draw hook: a camera facing quad textured with anim0 (colour) and anim1 (alpha mask).
void fn_1_8528(M532RumbleWarn *self, Mtx *mtx)
{
    Mtx work;
    Mtx inv;
    GXColor color;
    GXColor tev;
    f32 x0;
    f32 x1;
    f32 y0;
    f32 y1;

    PSMTXInverse(*mtx, inv);
    inv[0][3] = 0.0f;
    inv[1][3] = 0.0f;
    inv[2][3] = 0.0f;
    PSMTXScale(work, self->scale, self->scale, self->scale);
    PSMTXTransApply(work, work, self->pos.x, self->pos.y, self->pos.z);
    PSMTXConcat(*mtx, work, work);
    PSMTXConcat(work, inv, work);
    GXLoadPosMtxImm(work, GX_PNMTX0);
    GXSetNumChans(0);
    HuSprTexLoad(self->anim0, 0, 0, 0, 0, 1);
    HuSprTexLoad(self->anim1, 0, 1, 0, 0, 1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    color = lbl_1_rodata_1A4;
    color.a = (u8)(255.0f * self->alpha);
    tev = lbl_1_rodata_1A4;
    tev.a = color.a;
    GXSetTevColor(GX_TEVREG0, tev);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_TEXA, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetCullMode(GX_CULL_NONE);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, (u8)(s32)(0.75 * (f64)color.a), GX_AOP_AND, GX_LEQUAL, 0xFF);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXClearVtxDesc();
    GXSetVtxDescv((GXVtxDescList *)lbl_1_rodata_1A8);
    GXSetVtxAttrFmtv(GX_VTXFMT0, lbl_1_rodata_1C0);
    x0 = -self->sizeX;
    x1 = self->sizeX;
    y1 = self->sizeY;
    y0 = -self->sizeY;
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x0, y1, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(x1, y1, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(x1, y0, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(x0, y0, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
}

void fn_1_8A08(void)
{
    lbl_1_bss_A08 = 0x12C;
    lbl_1_bss_A0C = 0x3C;
}

// The unit's one static initialiser, hooked into the constructor table.
#ifdef __MWERKS__
M532_CTORS void (*const lbl_ctor_8A08)(void) = fn_1_8A08;
#else
struct M532RumbleStaticInit {
    M532RumbleStaticInit() { fn_1_8A08(); }
};
static M532RumbleStaticInit lbl_static_init;
#endif
}
