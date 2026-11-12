// m513Dll powerind: the "indicator" singleton. It copies the frame buffer into
// a texture (owned by the object) every frame and loads the number / marker
// sprites the other effects use.
#include "REL/m513Dll.h"
extern "C" {
#include "game/sprite.h"
#include "dolphin/gx.h"
}

extern "C" {

M513IndHook *fn_1_B110(M513IndHook *self, s16 flag);
void fn_1_B23C(M513IndHook *self, s32 bit);
void fn_1_B254(M513IndHook *self);
void fn_1_B2AC(M513IndHook *self);
void fn_1_B458(M513IndHook *self, Mtx mtx);

void fn_1_1220C(void);
void fn_1_12208(void);

char lbl_1_data_378[] = "ms_pointer == 0";
char lbl_1_data_388[] = "singleton.h";
char lbl_1_data_394[] = "ms_pointer != 0";
void *lbl_1_data_3A4[5] = { 0, 0, (void *)fn_1_B110, (void *)fn_1_B23C, (void *)fn_1_B254 };
M513Ptmf lbl_1_data_3B8 = { 0, -1, (void (*)(void *))fn_1_1220C };
M513Ptmf lbl_1_data_3C4 = { 0, -1, (void (*)(void *))fn_1_12208 };

// Hook: draw the frame buffer copy.
void fn_1_AE0C(HU3DMODEL *model, Mtx *mtx)
{
    Mtx *sp8 = mtx;
    M513IndHook *a = lbl_1_bss_9F0;
    M513IndHook *b = a;

    fn_1_B458(b, (MtxPtr)sp8);
}

M513IndHook *fn_1_AE58(M513IndHook *self)
{
    M513IndHook *ms;
    HU3DMODELID hookId;
    s32 i;

    fn_1_1768(&self->link);
    self->vtbl = (void **)&lbl_1_data_218;
    self->flags = 0;
    fn_1_1834(&self->link, &lbl_1_bss_C);
    M513_ASSERT(lbl_1_bss_9F0 == 0, lbl_1_data_378, lbl_1_data_388, 0x35);
    ms = (M513IndHook *)((char *)self + 0x10);
    if (ms != 0) {
        ms = (M513IndHook *)((char *)ms - 0x10);
    }
    lbl_1_bss_9F0 = ms;
    self->vtbl = lbl_1_data_3A4;
    hookId = Hu3DHookFuncCreate(fn_1_AE0C);
    Hu3DModelLayerSet(hookId, 4);
    Hu3DModelCameraSet(hookId, 1);
    fn_1_B2AC(self);
    self->fade[0] = 0.0f;
    self->fade[1] = 0.0f;
    self->fade[2] = 0.0f;
    self->subAnim[0] = HuSprAnimDataRead(M513_ANM_ind_a);
    HuSprAnimLock(self->subAnim[0]);
    self->subAnim[1] = HuSprAnimDataRead(M513_ANM_ind_b);
    HuSprAnimLock(self->subAnim[1]);
    self->subAnim[2] = HuSprAnimDataRead(M513_ANM_ind_c);
    HuSprAnimLock(self->subAnim[2]);
    self->subAnim[3] = HuSprAnimDataRead(M513_ANM_ind_d);
    HuSprAnimLock(self->subAnim[3]);
    for (i = 0; i < 32; i++) {
        self->sprAnim[i] = HuSprAnimDataRead(M513_ANM_ind_num + i);
        HuSprAnimLock(self->sprAnim[i]);
    }
    return self;
}

M513IndHook *fn_1_B084(M513IndHook *self, s16 flag)
{
    if (self != 0) {
        M513_ASSERT(lbl_1_bss_9F0 != 0, lbl_1_data_394, lbl_1_data_388, 0x3D);
        lbl_1_bss_9F0 = 0;
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M513IndHook *fn_1_B110(M513IndHook *self, s16 flag)
{
    s32 i;

    if (self != 0) {
        self->vtbl = lbl_1_data_3A4;
        HuSprAnimKill(self->subAnim[0]);
        HuSprAnimKill(self->subAnim[1]);
        HuSprAnimKill(self->subAnim[2]);
        HuSprAnimKill(self->subAnim[3]);
        for (i = 0; i < 32; i++) {
            HuSprAnimKill(self->sprAnim[i]);
        }
        if ((void *)((char *)self + 0x10) != 0) {
            M513_ASSERT(lbl_1_bss_9F0 != 0, lbl_1_data_394, lbl_1_data_388, 0x3D);
            lbl_1_bss_9F0 = 0;
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

void fn_1_B23C(M513IndHook *self, s32 bit)
{
    self->flags &= ~(1 << bit);
}

void fn_1_B254(M513IndHook *self)
{
    self->fade[0] += 0.003f;
    self->fade[1] += 0.003f;
    self->fade[2] += 0.003f;
}

// Set up the frame buffer texture.
void fn_1_B2AC(M513IndHook *self)
{
    GXInitTexObj(&self->texObj, self->frameBuf, 0x280, 0x1E0, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&self->texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
}

void fn_1_B338(M513IndHook *self)
{
    GXSetTexCopySrc(0, 0, 0x280, 0x1E0);
    GXSetTexCopyDst(0x280, 0x1E0, GX_TF_RGB565, GX_FALSE);
    GXCopyTex(self->frameBuf, GX_FALSE);
}

// Texture projection of the frame buffer copy.
void fn_1_B390(Mtx mtx)
{
    Mtx lightMtx;
    Mtx texMtx;

    GXSetNumTexGens(1);
    C_MTXLightPerspective(lightMtx, Hu3DCamera[Hu3DCameraNo].fov, Hu3DCamera[Hu3DCameraNo].aspect, 0.5f, -0.5f, 0.5f, 0.5f);
    PSMTXConcat(lightMtx, mtx, texMtx);
    GXLoadTexMtxImm(texMtx, 0x1E, GX_MTX3x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, 0x1E, GX_FALSE, 0x7D);
}

void fn_1_B458(M513IndHook *self, Mtx mtx)
{
    Mtx lightMtx;
    Mtx texMtx;

    GXLoadPosMtxImm(mtx, 0);
    GXSetNumTexGens(1);
    C_MTXLightPerspective(lightMtx, Hu3DCamera[Hu3DCameraNo].fov, Hu3DCamera[Hu3DCameraNo].aspect, 0.5f, -0.5f, 0.5f, 0.5f);
    PSMTXConcat(lightMtx, mtx, texMtx);
    GXLoadTexMtxImm(texMtx, 0x1E, GX_MTX3x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, 0x1E, GX_FALSE, 0x7D);
    GXSetTexCopySrc(0, 0, 0x280, 0x1E0);
    GXSetTexCopyDst(0x280, 0x1E0, GX_TF_RGB565, GX_FALSE);
    GXCopyTex(self->frameBuf, GX_FALSE);
}

}
