#include "dolphin.h"
#include "game/data.h"
#include "game/gxarray.h"
#include "game/hu3d.h"
#include "game/sprite.h"
#include "REL/m579Dll/m579objs.h"

extern "C" {
extern M579WaterEff *lbl_1_bss_A24;
extern u8 lbl_1_bss_C[];
extern u8 lbl_1_data_1FC[];
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);
void GXSetVtxAttrFmtv(GXVtxFmt vtxfmt, GXVtxAttrFmtList *list);
void fn_1_2CDC(s16 *model, HU3DMODELHOOK hook);
void fn_1_30BC(s16 *model, void *hookData);

void *fn_1_148AC(M579WaterEff *self);
M579U8Vec *fn_1_14AEC(M579U8Vec *self, s16 flag);
M579U8Vec *fn_1_14B80(M579U8Vec *self, s16 flag);
M579U8Vec *fn_1_14C0C(M579U8Vec *self, s16 flag);
M579WaterEff *fn_1_14C90(M579WaterEff *self, s16 flag);
M579WaterEff *fn_1_14D1C(M579WaterEff *self, s16 flag);
void fn_1_14E50(M579WaterEff *self, s32 bit);
void fn_1_14E68(Vec *pos, f32 *out);
void fn_1_14ED8(M579WaterEff *self);
void fn_1_15130(HU3DMODEL *modelP, Mtx *mtx);
void fn_1_15168(M579WaterEff *self, GXTexMapID map, u32 texMtxId, Mtx mtx);
void fn_1_15280(M579WaterEff *self, GXTexMapID map, u32 texMtxId);
void fn_1_152CC(M579WaterEff *self, s32 map, u32 texMtxId);
void fn_1_1532C(GXIndTexStageID ind, GXTexMapID map, GXTexCoordID coord);
void fn_1_1537C(M579WaterEff *self, Mtx mtx);
void fn_1_15958(void);
void fn_1_159E0(M579U8Vec *vec, u32 n, u8 *fill);
}

u8 lbl_1_data_640 = 0;
char lbl_1_data_641[16] = "ms_pointer == 0";
char lbl_1_data_651[12] = "singleton.h";
char lbl_1_data_65D[19] = "ms_pointer != 0";
void *lbl_1_data_670[5] = { 0, 0, (void *) fn_1_14D1C, (void *) fn_1_14E50, (void *) fn_1_14ED8 };
char lbl_1_data_684[21] = "vector length error\n";
char lbl_1_data_699[31] = "Memory Allocation failure";

M579_RODATA const f32 lbl_1_rodata_78C[18] = {
    0.01f, 0.0f, 0.0f, 0.0f, 0.01f, 0.0f,
    0.02f, 0.0f, 0.0f, 0.0f, 0.02f, 0.0f,
    0.03f, 0.0f, 0.0f, 0.0f, 0.03f, 0.0f
};
M579_RODATA const GXColor lbl_1_rodata_7D4 = { 0x3D, 0x3B, 0x1B, 0x29 };
M579_RODATA const GXVtxDescList lbl_1_rodata_7D8[4] = {
    { GX_VA_POS, GX_INDEX8 },
    { GX_VA_CLR0, GX_INDEX8 },
    { GX_VA_TEX0, GX_INDEX8 },
    { GX_VA_NULL, GX_NONE }
};
M579_RODATA const GXVtxAttrFmtList lbl_1_rodata_7F8[4] = {
    { GX_VA_POS, GX_POS_XYZ, GX_S16, 14 },
    { GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA4, 0 },
    { GX_VA_TEX0, GX_TEX_ST, GX_S16, 14 },
    { GX_VA_NULL, GX_POS_XY, GX_U8, 0 }
};
M579_RODATA const s16 lbl_1_rodata_840[96] = {
    0, 0, 0, 0, 0, 8192, -1761, 0, 8000, -3439, 0, 7434,
    -4957, 0, 6521, -6243, 0, 5303, -7237, 0, 3837, -7893, 0, 2191,
    -8179, 0, 443, -8084, 0, -1325, -7610, 0, -3032, -6780, 0, -4597,
    -5633, 0, -5947, -4223, 0, -7019, -2615, 0, -7763, -885, 0, -8143,
    885, 0, -8143, 2615, 0, -7763, 4223, 0, -7019, 5633, 0, -5947,
    6780, 0, -4597, 7610, 0, -3032, 8084, 0, -1325, 8179, 0, 443,
    7893, 0, 2191, 7237, 0, 3837, 6243, 0, 5303, 4957, 0, 6521,
    3439, 0, 7434, 1761, 0, 8000, 0, 0, 0, 0, 0, 0
};
M579_RODATA const u16 lbl_1_rodata_900[16] = {
    0x5742, 0x17FB, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};
M579_RODATA const s16 lbl_1_rodata_920[64] = {
    8192, 8192, 8192, 16384, 6430, 16192, 4752, 15626, 3234, 14713, 1948, 13495,
    954, 12029, 298, 10383, 12, 8635, 107, 6866, 581, 5159, 1411, 3594,
    2558, 2244, 3968, 1172, 5576, 428, 7306, 48, 9077, 48, 10807, 428,
    12415, 1172, 13825, 2244, 14972, 3594, 15802, 5159, 16276, 6866, 16371, 8635,
    16085, 10383, 15429, 12029, 14435, 13495, 13149, 14713, 11631, 15626, 9953, 16192,
    0, 0, 0, 0
};
M579_RODATA const u8 ATTRIBUTE_ALIGN(32) lbl_1_rodata_9A0[96] = {
    0xA0, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x02, 0x01, 0x02, 0x03, 0x01, 0x03, 0x04,
    0x01, 0x04, 0x05, 0x01, 0x05, 0x06, 0x01, 0x06, 0x07, 0x01, 0x07, 0x08, 0x01, 0x08, 0x09, 0x01,
    0x09, 0x0A, 0x01, 0x0A, 0x0B, 0x01, 0x0B, 0x0C, 0x01, 0x0C, 0x0D, 0x01, 0x0D, 0x0E, 0x01, 0x0E,
    0x0F, 0x01, 0x0F, 0x10, 0x01, 0x10, 0x11, 0x01, 0x11, 0x12, 0x01, 0x12, 0x13, 0x01, 0x13, 0x14,
    0x01, 0x14, 0x15, 0x01, 0x15, 0x16, 0x01, 0x16, 0x17, 0x01, 0x17, 0x18, 0x01, 0x18, 0x19, 0x01,
    0x19, 0x1A, 0x01, 0x1A, 0x1B, 0x01, 0x1B, 0x1C, 0x01, 0x1C, 0x1D, 0x01, 0x1D, 0x01, 0x01, 0x01
};

void *fn_1_148AC(M579WaterEff *self)
{
    u32 texSize;
    M579U8Vec *vec;

    fn_1_1264(self);
    self->base.vtbl = (void **) lbl_1_data_1FC;
    self->base.flags = 0;
    fn_1_1330(self, lbl_1_bss_C);
    lbl_1_bss_A24 == 0 ? (void) 0 : fn_1_1D4(lbl_1_data_641, lbl_1_data_651, 0x35);
#ifdef TARGET_PC
    lbl_1_bss_A24 = self;
#else
    {
        u8 *q = (u8 *) self + 0x10;
        if (q != 0) {
            q -= 0x10;
        }
        lbl_1_bss_A24 = (M579WaterEff *) q;
    }
#endif
    fn_1_2CDC(&self->model, fn_1_15130);
    self->base.vtbl = lbl_1_data_670;
    texSize = GXGetTexBufferSize(0x140, 0xF0, 4, 0, 0);
    vec = &self->buf;
    self->buf.cap = 0;
    vec->size = 0;
    vec->data = 0;
    fn_1_159E0(vec, texSize, &lbl_1_data_640);
    self->waveData = HuDataSelHeapReadNum(0x850012, 0x10000000, HEAP_MODEL);
    self->anim = HuSprAnimRead(HuDataSelHeapReadNum(0x850013, 0x10000000, HEAP_MODEL));
    self->scrollX = 0.0f;
    self->scrollY = 0.0f;
    fn_1_30BC(&self->model, self);
    fn_1_304C(&self->model, 1);
    PSMTXIdentity(self->mtx[0]);
    PSMTXIdentity(self->mtx[1]);
    PSMTXIdentity(self->mtx[2]);
    PSMTXIdentity(self->mtx[3]);
    GXInitTexObj(&self->reflTex, self->buf.data, 0x140, 0xF0, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObj(&self->waveTex, self->waveData, 0x80, 0x80, GX_TF_IA8, GX_REPEAT, GX_REPEAT, GX_TRUE);
    GXInitTexObjLOD(&self->waveTex, GX_LIN_MIP_LIN, GX_LINEAR, 0.0f, 7.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    return self;
}

M579U8Vec *fn_1_14AEC(M579U8Vec *self, s16 flag)
{
    M579U8Vec *a;
    M579U8Vec *b;
    u8 **p;
    u8 **q;
    u8 *d;

    if (self != 0) {
        a = self;
        b = a;
        if (self != 0 && a != 0 && b != 0) {
            p = &self->data;
            if (*p != 0) {
                q = &self->data;
                d = *q;
                fn_1_C8(d);
            }
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M579U8Vec *fn_1_14B80(M579U8Vec *self, s16 flag)
{
    M579U8Vec *a;
    u8 **p;
    u8 **q;
    u8 *d;

    if (self != 0) {
        a = self;
        if (self != 0 && a != 0) {
            p = &self->data;
            if (*p != 0) {
                q = &self->data;
                d = *q;
                fn_1_C8(d);
            }
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M579U8Vec *fn_1_14C0C(M579U8Vec *self, s16 flag)
{
    u8 **p;
    u8 **q;
    u8 *d;

    if (self != 0) {
        p = &self->data;
        if (*p != 0) {
            q = &self->data;
            d = *q;
            fn_1_C8(d);
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M579WaterEff *fn_1_14C90(M579WaterEff *self, s16 flag)
{
    if (self != 0) {
        lbl_1_bss_A24 != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_65D, lbl_1_data_651, 0x3D);
        lbl_1_bss_A24 = 0;
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M579WaterEff *fn_1_14D1C(M579WaterEff *self, s16 flag)
{
    if (self != 0) {
        self->base.vtbl = lbl_1_data_670;
        if (&self->buf != 0 && &self->buf != 0 && &self->buf != 0 && self->buf.data != 0) {
            fn_1_C8(self->buf.data);
        }
        fn_1_30F4(&self->model, 0);
        if (self != 0) {
            lbl_1_bss_A24 != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_65D, lbl_1_data_651, 0x3D);
            lbl_1_bss_A24 = 0;
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

void fn_1_14E50(M579WaterEff *self, s32 bit)
{
    self->base.flags = self->base.flags & ~(1 << bit);
}

void fn_1_14E68(Vec *pos, f32 *out)
{
    Vec up;
    f32 fov;

    Hu3DCameraPosGet(1, pos, &up, &up);
    Hu3DCameraPerspectiveGet(1, &fov, &fov, out);
    *out = (f32) ((f64) *out * 1.8);
}

void fn_1_14ED8(M579WaterEff *self)
{
    Vec pos;
    Vec up;
    f32 far;
    f32 fov;
    f32 t;

    Hu3DCameraPosGet(1, &pos, &up, &up);
    Hu3DCameraPerspectiveGet(1, &fov, &fov, &far);
    far = (f32) ((f64) far * 1.8);
    PSMTXScale(self->mtx[0], far, 1.0f, far);
    PSMTXTransApply(self->mtx[0], self->mtx[0], pos.x, -150.0f, pos.z);
    self->scrollX = (f32) (self->scrollX - 1.4285715f);
    self->scrollY = (f32) (self->scrollY - 0.5882353f);
    t = far / 600.0f;
    PSMTXScale(self->mtx[1], t, t, 1.0f);
    PSMTXTransApply(self->mtx[1], self->mtx[1], (f32) (((f64) pos.x - (0.5 * (f64) self->scrollX)) / 600.0),
                    (f32) (((f64) pos.z + (0.8 * (f64) self->scrollY)) / 600.0), 0.0f);
    PSMTXScale(self->mtx[2], t, t, 1.0f);
    PSMTXTransApply(self->mtx[2], self->mtx[2], (pos.x + self->scrollX) / 600.0f, (pos.z + self->scrollY) / 600.0f, 0.0f);
    PSMTXScale(self->mtx[3], t, t, 1.0f);
    PSMTXTransApply(self->mtx[3], self->mtx[3], pos.x / 600.0f, pos.z / 600.0f, 0.0f);
}

void fn_1_15130(HU3DMODEL *modelP, Mtx *mtx)
{
    fn_1_1537C((M579WaterEff *) modelP->hookData, *mtx);
}

void fn_1_15168(M579WaterEff *self, GXTexMapID map, u32 texMtxId, Mtx mtx)
{
    Mtx proj;
    f32 fov;
    f32 near;

    GXSetTexCopySrc(0, 0, 0x280, 0x1E0);
    GXSetTexCopyDst(0x140, 0xF0, GX_TF_RGB565, GX_TRUE);
    GXCopyTex(self->buf.data, GX_FALSE);
    GXPixModeSync();
    GXLoadTexObj(&self->reflTex, map);
    Hu3DCameraPerspectiveGet(1, &fov, &near, &near);
    C_MTXLightPerspective(proj, fov, 1.2f, 0.5f, -0.5f, 0.5f, 0.5f);
    PSMTXConcat(proj, mtx, proj);
    GXLoadTexMtxImm(proj, texMtxId, GX_MTX3x4);
}

void fn_1_15280(M579WaterEff *self, GXTexMapID map, u32 texMtxId)
{
    GXLoadTexObj(&self->waveTex, map);
    GXLoadTexMtxImm(self->mtx[2], texMtxId, GX_MTX2x4);
}

void fn_1_152CC(M579WaterEff *self, s32 map, u32 texMtxId)
{
    HuSprTexLoad(self->anim, 0, (s16) map, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    GXLoadTexMtxImm(self->mtx[3], texMtxId, GX_MTX2x4);
}

void fn_1_1532C(GXIndTexStageID ind, GXTexMapID map, GXTexCoordID coord)
{
    GXSetIndTexOrder(ind, coord, map);
    GXSetIndTexCoordScale(ind, GX_ITS_1, GX_ITS_1);
}

void fn_1_1537C(M579WaterEff *self, Mtx mtx)
{
    Mtx wmtx;
    GXColor kc;

    PSMTXConcat(mtx, self->mtx[0], wmtx);
    GXLoadPosMtxImm(wmtx, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    fn_1_15168(self, GX_TEXMAP0, 0x1E, wmtx);
    fn_1_15280(self, GX_TEXMAP1, 0x21);
    fn_1_152CC(self, 2, 0x24);
    GXLoadTexMtxImm(self->mtx[1], 0x27, GX_MTX2x4);
    GXSetNumTexGens(4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, 0x1E, GX_FALSE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, 0x27, GX_FALSE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, 0x21, GX_FALSE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD3, GX_TG_MTX2x4, GX_TG_TEX0, 0x24, GX_FALSE, 0x7D);
    GXSetIndTexMtx(GX_ITM_0, (f32 (*)[3]) &lbl_1_rodata_78C[0], 0);
    GXSetIndTexMtx(GX_ITM_1, (f32 (*)[3]) &lbl_1_rodata_78C[6], 0);
    GXSetIndTexMtx(GX_ITM_2, (f32 (*)[3]) &lbl_1_rodata_78C[0xC], 0);
    GXSetNumIndStages(2);
    fn_1_1532C(GX_INDTEXSTAGE0, GX_TEXMAP1, GX_TEXCOORD2);
    fn_1_1532C(GX_INDTEXSTAGE1, GX_TEXMAP2, GX_TEXCOORD3);
    kc = lbl_1_rodata_7D4;
    GXSetTevKColor(GX_KCOLOR0, kc);
    GXSetNumTevStages(4);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevIndWarp(GX_TEVSTAGE1, GX_INDTEXSTAGE1, GX_TRUE, GX_FALSE, GX_ITM_1);
    GXSetTevKColorSel(GX_TEVSTAGE1, GX_TEV_KCSEL_K0);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_KONST, GX_CC_TEXA, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_CPREV, GX_CC_RASC, GX_CC_RASA, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevIndWarp(GX_TEVSTAGE3, GX_INDTEXSTAGE1, GX_TRUE, GX_FALSE, GX_ITM_2);
    GXSetTevKColorSel(GX_TEVSTAGE3, GX_TEV_KCSEL_K0_A);
    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD2, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_KONST, GX_CC_TEXC, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetCullMode(GX_CULL_BACK);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZCompLoc(GX_TRUE);
    GXSetZMode(GX_TRUE, GX_LESS, GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0xFF, GX_AOP_OR, GX_ALWAYS, 0xFF);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
    fn_1_15958();
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetTevDirect(GX_TEVSTAGE2);
    GXSetTevDirect(GX_TEVSTAGE3);
    GXSetNumIndStages(0);
}

void fn_1_15958(void)
{
    GXClearVtxDesc();
    GXSetVtxDescv((GXVtxDescList *) lbl_1_rodata_7D8);
    GXSetVtxAttrFmtv(GX_VTXFMT0, (GXVtxAttrFmtList *) lbl_1_rodata_7F8);
    GXSETARRAY(GX_VA_POS, lbl_1_rodata_840, sizeof(lbl_1_rodata_840), 6, TRUE);
    GXSETARRAY(GX_VA_CLR0, lbl_1_rodata_900, sizeof(lbl_1_rodata_900), 2, TRUE);
    GXSETARRAY(GX_VA_TEX0, lbl_1_rodata_920, sizeof(lbl_1_rodata_920), 4, TRUE);
    GXCallDisplayList((void *) lbl_1_rodata_9A0, 0x60);
}

void fn_1_159E0(M579U8Vec *vec, u32 n, u8 *fill)
{
    M579U8Vec *v;
    M579U8Vec *w;
    M579U8Vec *x;
    u32 max;
    u32 max2;
    u32 max3;
    u8 *buf;
    u8 *p;
    u32 i;

    if (n != 0) {
        v = vec;
        max = 0xFFFFFFFF;
        max2 = max;
        max3 = max2;
        if (n > max3) {
            fn_1_234(M579_STDERR, lbl_1_data_684);
            abort();
        }
        w = vec;
        buf = (u8 *) fn_1_0(n);
        if (buf == 0) {
            fn_1_234(M579_STDERR, lbl_1_data_699);
            abort();
        }
        vec->data = buf;
        x = vec;
        vec->size = n;
        x->cap = n;
        i = n;
        p = vec->data;
        while (i != 0) {
            *p = *fill;
            p++;
            i--;
        }
    }
}
