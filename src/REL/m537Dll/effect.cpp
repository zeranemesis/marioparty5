#include "REL/m537Dll.h"
#include <math.h>

extern "C" {
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT,
                  GXTexFilter filter);

extern void *lbl_1_data_1FC[];
extern const f32 lbl_1_rodata_1F8;

// Lens flare object: list node, vtable, flags, then the four flare sprites.
struct M537Effect {
    M537_NODE m_node;   /* 0x00 */
    void **m_vptr;      /* 0x08 */
    s32 m_flag;         /* 0x0C */
    ANIMDATA *m_anim[4];/* 0x10 */
};

// One flare ghost: size, position along the sun axis, colour and sprite.
struct M537Flare {
    f32 m_size;
    f32 m_pos;
    u32 m_color;
    s32 m_tex;
};

void fn_1_764C(M537Effect *self, Mtx mtx);
void fn_1_7DBC(M537Effect *self, Vec *pos, f32 alpha, f32 size, u32 color, s32 tex);
void fn_1_81AC(Vec *out, M537Effect *self, Vec *pos, Mtx mtx);
M537Effect *fn_1_74C0(M537Effect *self, s16 freeIt);
void fn_1_75B8(M537Effect *self, s32 bit);
void fn_1_83C8(void);

M537Effect *lbl_1_bss_9F8;

Vec lbl_1_data_2FC = { 0.0f, 3537.0f, 0.0f };
void *lbl_1_data_308[6] = { 0, 0, (void *)fn_1_74C0, (void *)fn_1_75B8, (void *)fn_1_83C8, 0 };

#ifdef __MWERKS__
#define M537_RODATA __declspec(section ".rodata")
#else
#define M537_RODATA
#endif

M537_RODATA const s32 lbl_1_rodata_F8[4] = { 0x5F0019, 0x5F001A, 0x5F001B, 0x5F001C };
M537_RODATA const s32 lbl_1_rodata_108[4] = { 0, 1, 2, 3 };
M537_RODATA const M537Flare lbl_1_rodata_118[8] = {
    { 10.0f, -1.0f, 0xFFFFFFFF, 0 },
    { 8.0f, -0.8f, 0x30FFFF80, 2 },
    { 7.0f, -0.7f, 0x60C080FF, 1 },
    { 5.0f, 0.1f, 0x50FFFFFF, 1 },
    { 10.0f, 0.2f, 0x30FFFFC0, 2 },
    { 15.0f, 0.7f, 0x60FFFFFF, 1 },
    { 20.0f, 0.8f, 0x40FFC0FF, 2 },
    { 12.0f, 1.0f, 0x30C0FFFF, 1 },
};

void fn_1_7254(HU3DMODEL *model, Mtx *mtx)
{
    M537Effect *e1;
    M537Effect *e0;

    e0 = lbl_1_bss_9F8;
    e1 = e0;
    fn_1_764C(e1, *mtx);
}

M537Effect *fn_1_72A0(M537Effect *self)
{
    M537Effect *ms;
    s16 mdl;
    s32 i;

    fn_1_1264(self);
    self->m_vptr = lbl_1_data_1FC;
    self->m_flag = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    if (lbl_1_bss_9F8 == 0) {
    } else {
        fn_1_1D4("ms_pointer == 0", "singleton.h", 0x35);
    }
    ms = (M537Effect *)((u8 *)self + 0x10);
    if (ms != 0) {
        ms = (M537Effect *)((u8 *)ms - 0x10);
    }
    lbl_1_bss_9F8 = ms;
    self->m_vptr = lbl_1_data_308;
    mdl = Hu3DHookFuncCreate(fn_1_7254);
    Hu3DModelLayerSet(mdl, 7);
    Hu3DModelCameraSet(mdl, 1);
    for (i = 0; i < 4; i++) {
        self->m_anim[i] = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_rodata_F8[i], 0x10000000, HEAP_MODEL));
        HuSprAnimLock(self->m_anim[i]);
    }
    return self;
}

M537Effect *fn_1_73DC(M537Effect *self, s16 freeIt)
{
    if (self != 0) {
        if (lbl_1_bss_9F8 != 0) {
        } else {
            fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_9F8 = 0;
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M537Effect *fn_1_7468(M537Effect *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M537Effect *fn_1_74C0(M537Effect *self, s16 freeIt)
{
    s32 i;

    if (self != 0) {
        self->m_vptr = lbl_1_data_308;
        for (i = 0; i < 4; i++) {
            HuSprAnimKill(self->m_anim[i]);
        }
        if ((M537Effect *)((u8 *)self + 0x10) != 0) {
            if (lbl_1_bss_9F8 != 0) {
            } else {
                fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
            }
            lbl_1_bss_9F8 = 0;
        }
        if (self != 0) {
            self->m_vptr = lbl_1_data_1FC;
            if (self != 0) {
                fn_1_1270(self, 0);
            }
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_75B8(M537Effect *self, s32 bit)
{
    self->m_flag &= ~(1 << bit);
}

void fn_1_75D0(M537Effect *self)
{
    s32 i;

    for (i = 1; i < 4; i++) {
        HuSprTexLoad(self->m_anim[i], 0, (s16)lbl_1_rodata_108[i], GX_REPEAT, GX_REPEAT, GX_LINEAR);
    }
}

// Draws the sun glare and the chain of flare ghosts for the current camera.
void fn_1_764C(M537Effect *self, Mtx mtx)
{
    Vec toCam;
    Vec dir;
    Vec sunPos;
    Vec sunScr;
    Vec ghostPos;
    Vec pos0;
    Mtx ident;
    f32 s0;
    f32 s1;
    f32 s2;
    f32 s3;
    f32 s4;
    f32 s5;
    f32 cosFov;
    f32 amount;
    s32 i;
    s32 j;

    PSVECSubtract(&Hu3DCamera->pos, &lbl_1_data_2FC, &toCam);
    PSVECNormalize(&toCam, &toCam);
    PSMTXIdentity(ident);
    GXLoadPosMtxImm(ident, GX_PNMTX0);
    PSMTXMultVecSR(mtx, &toCam, &dir);
    if (!(dir.z < 0.0f)) {
        HuSprTexLoad(self->m_anim[0], 0, GX_TEXMAP0, GX_REPEAT, GX_REPEAT, GX_LINEAR);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetNumTevStages(1);
        GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT0, GX_DF_CLAMP, GX_AF_SPOT);
        GXSetZCompLoc(GX_FALSE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
        GXSetAlphaUpdate(GX_TRUE);
        s0 = 240.0f * lbl_1_rodata_118[0].m_pos;
        pos0.x = dir.x * s0;
        pos0.y = dir.y * s0;
        pos0.z = -100.0f;
        ghostPos = pos0;
        fn_1_7DBC(self, &ghostPos, 1.0f, lbl_1_rodata_118[0].m_size, lbl_1_rodata_118[0].m_color,
                  lbl_1_rodata_118[0].m_tex);
        cosFov = (f32)cos((f64)(0.017453292f * Hu3DCamera->fov));
        s1 = (dir.z - cosFov) / (1.0f - cosFov);
        PSVECScale(&dir, &dir, 1.0f - cosFov);
        if (!(s1 < 0.0f)) {
            sunPos = lbl_1_data_2FC;
            sunScr = lbl_1_data_2FC;
            fn_1_81AC(&ghostPos, self, &sunScr, mtx);
            pos0 = ghostPos;
            for (i = 1; i < 4; i++) {
                HuSprTexLoad(self->m_anim[i], 0, (s16)lbl_1_rodata_108[i], GX_REPEAT, GX_REPEAT, GX_LINEAR);
            }
            for (j = 1; j < 8; j++) {
                s2 = 240.0f * lbl_1_rodata_118[j].m_pos;
                pos0.x = dir.x * s2;
                pos0.y = dir.y * s2;
                pos0.z = -100.0f;
                ghostPos = pos0;
                fn_1_7DBC(self, &ghostPos, s1, lbl_1_rodata_118[j].m_size, lbl_1_rodata_118[j].m_color,
                          lbl_1_rodata_118[j].m_tex);
            }
            if (!(s1 < 0.8f)) {
                GXClearVtxDesc();
                GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
                GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
                GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
                s3 = 220.0f * ((s1 - 0.8f) / 0.2f);
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                GXPosition3f32(-320.0f, -240.0f, -100.0f);
                GXColor4u8(0xFF, 0xFF, 0xFF, (u8)(s32)s3);
                GXPosition3f32(-320.0f, 240.0f, -100.0f);
                GXColor4u8(0xFF, 0xFF, 0xFF, (u8)(s32)s3);
                GXPosition3f32(320.0f, 240.0f, -100.0f);
                GXColor4u8(0xFF, 0xFF, 0xFF, (u8)(s32)s3);
                GXPosition3f32(320.0f, -240.0f, -100.0f);
                GXColor4u8(0xFF, 0xFF, 0xFF, (u8)(s32)s3);
#ifdef TARGET_PC
                GXEnd();
#endif
            }
        }
    }
}

// Draws one flare quad centred on pos.
void fn_1_7DBC(M537Effect *self, Vec *pos, f32 alpha, f32 size, u32 color, s32 tex)
{
    u8 c0;
    u8 c1;
    u8 c2;
    s32 a;
    f32 half;

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, (GXTexMapID)tex, GX_COLOR0A0);
    c0 = (u8)color;
    c1 = (u8)(color >> 8);
    c2 = (u8)(color >> 16);
    a = (s32)(alpha * (f32)(u8)(color >> 24));
    half = size * alpha + alpha * (4.0f * (f32)tex);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(pos->x - half, pos->y - half, pos->z);
    GXColor4u8(c0, c1, c2, a);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(pos->x - half, pos->y + half, pos->z);
    GXColor4u8(c0, c1, c2, a);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(pos->x + half, pos->y + half, pos->z);
    GXColor4u8(c0, c1, c2, a);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(pos->x + half, pos->y - half, pos->z);
    GXColor4u8(c0, c1, c2, a);
    GXTexCoord2f32(1.0f, 0.0f);
#ifdef TARGET_PC
    GXEnd();
#endif
}

// Projects a world position into screen space (x, y; z is left 0).
void fn_1_81AC(Vec *out, M537Effect *self, Vec *pos, Mtx mtx)
{
    Vec v;
    f32 sx;
    f32 sy;
    f32 scr;

    PSMTXMultVec(mtx, pos, &v);
    sx = (f32)(1.2000000476837158 * ((f64)v.z * (sin(3.141592653589793 * (f64)(Hu3DCamera->fov * 0.5f) / 180.0) /
                                                  cos(3.141592653589793 * (f64)(Hu3DCamera->fov * 0.5f) / 180.0))));
    sy = (f32)((f64)v.z * (sin(3.141592653589793 * (f64)(Hu3DCamera->fov * 0.5f) / 180.0) /
                           cos(3.141592653589793 * (f64)(Hu3DCamera->fov * 0.5f) / 180.0)));
    scr = lbl_1_rodata_1F8 + (v.x * (lbl_1_rodata_1F8 / -sx));
    out->x = scr;
    out->y = 240.0f + (v.y * (240.0f / sy));
    out->z = 0.0f;
}

void fn_1_83C8(void)
{
}

}
