#include <math.h>

#include <dolphin.h>
#include <dolphin/mtx.h>

#include "datadir_enum.h"
#include "game/data.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/sprite.h"

extern "C" void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);

/* These are plain C in the original: m511.o defines them unmangled
   (m511.cpp is NonMatching, so the module links the original object).
   Left as C++ they mangle and the REL link fails with
   "Failed to find symbol fn_1_1D88__FPv in any module". */
extern "C" void *__dl__FPv(void *self);
extern "C" void fn_1_1D88(void *self);
extern "C" void fn_1_1D94(void *self, s32 arg1);
extern "C" void fn_1_1E54(void *self, void **arg1);
extern "C" void fn_1_CF8(char *cond, char *file, s32 line);

struct Mgr511Effring;

#ifdef TARGET_PC /* the definitions below sit in extern "C" blocks */
#define M511_PC_C extern "C"
#else
#define M511_PC_C
#endif
M511_PC_C void fn_1_6EDC(Mgr511Effring *arg0, Mtx mtx);
M511_PC_C void fn_1_7654(Mgr511Effring *arg1, Point3d *arg2, s32 color, s32 texMap, f32 scale, f32 size);
M511_PC_C void fn_1_7A44(f32 *arg0, Mgr511Effring *arg1, Mtx mtx);

struct Mgr511Effring {
    void *unk0;
    void *unk4;
    void **vtable;
    s32 unkC;
    ANIMDATA *anim[4];
    f32 unk20;
    f32 unk24;
    f32 unk28;
};

struct EffringQuad {
    f32 size;
    f32 scale;
    s32 color;
    s32 rate; /* texture map index (0..2); the decomp had it as a float of denormal bit patterns */
};

extern "C" {

extern "C" void *lbl_1_bss_2C[2];

extern void *lbl_1_data_40[6];
extern void *lbl_1_data_284[6];

#ifdef TARGET_PC /* the original object defines it; this unit is linked from source on PC */
Mgr511Effring *lbl_1_bss_A10;
#else
extern Mgr511Effring *lbl_1_bss_A10;
#endif

static const s32 lbl_1_rodata_D0[4] = {0x450023, 0x450024, 0x450025, 0x450026};
static const s32 lbl_1_rodata_E8[4] = {0, 1, 2, 3};
static const struct EffringQuad lbl_1_rodata_F8[8] = {
    {10.0f, -1.0f, -1, 0},
    {8.0f, -0.8f, 0x30FFFF80, 2},
    {7.0f, -0.7f, 0x60C080FF, 1},
    {5.0f, 0.1f, 0x50FFFFFF, 1},
    {10.0f, 0.2f, 0x30FFFFC0, 2},
    {15.0f, 0.7f, 0x60FFFFFF, 1},
    {20.0f, 0.8f, 0x40FFC0FF, 2},
    {12.0f, 1.0f, 0x30C0FFFF, 1},
};

void fn_1_6B0C(HU3DMODEL *arg1, Mtx *mtx)
{
    fn_1_6EDC(lbl_1_bss_A10, *mtx);
}

// NONMATCHING: the vtable pointer differs.
Mgr511Effring *fn_1_6B58(Mgr511Effring *arg0, s32 arg_sp0)
{
    s32 i;
    HU3DMODELID hookId;

    fn_1_1D88(arg0);
    arg0->vtable = &lbl_1_data_40[2];
    arg0->unkC = 0;
    fn_1_1E54(arg0, (void **)lbl_1_bss_2C);
    if (lbl_1_bss_A10 != NULL) {
        fn_1_CF8("ms_pointer == 0", "singleton.h", 0x35);
    }
    lbl_1_bss_A10 = arg0;
    arg0->vtable = lbl_1_data_284;
    arg0->unk20 = 0.0f;
    arg0->unk24 = 500.0f;
    arg0->unk28 = 500.0f;
    hookId = Hu3DHookFuncCreate(fn_1_6B0C);
    Hu3DModelLayerSet(hookId, 4);
    Hu3DModelCameraSet(hookId, 0xF);
    for (i = 0; i < 4; i++) {
        arg0->anim[i] = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_rodata_D0[i], HU_MEMNUM_OVL, HEAP_MODEL));
        HuSprAnimLock(arg0->anim[i]);
    }
    return arg0;
}

void *fn_1_6CC4(void *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        if (lbl_1_bss_A10 == NULL) {
            fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A10 = (Mgr511Effring *)NULL;
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

// NONMATCHING: the vtable pointer differs.
void *fn_1_6D50(void *arg0, s16 arg1)
{
    Mgr511Effring *self = (Mgr511Effring *)arg0;
    s32 i;

    if (arg0 != NULL) {
        self->vtable = lbl_1_data_284;
        for (i = 0; i < 4; i++) {
            HuSprAnimKill(self->anim[i]);
        }
        if (lbl_1_bss_A10 == NULL) {
            fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A10 = (Mgr511Effring *)NULL;
        self->vtable = &lbl_1_data_40[2];
        fn_1_1D94(arg0, 0);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_6E48(void *arg0, s32 arg1)
{
    s32 *flag = (s32 *)((u8 *)arg0 + 3 * sizeof(void *)); /* after the list node and vtable: GC 0xC */

    *flag &= ~(1 << arg1);
}

void fn_1_6E60(Mgr511Effring *arg0)
{
    s32 i;

    for (i = 1; i < 4; i++) {
        HuSprTexLoad(arg0->anim[i], (s16)0, (s16)lbl_1_rodata_E8[i], (GXTexWrapMode)1, (GXTexWrapMode)1, (GXTexFilter)1);
    }
}

// NONMATCHING: the direct GX FIFO writes and the vertex attributes are folded
// differently, and the projection helper arguments do not line up.
void fn_1_6EDC(Mgr511Effring *arg0, Mtx mtx)
{
    HU3DCAMERA *cam;
    Point3d sp70;
    Point3d sp64;
    Point3d spVec;
    f32 identMat[4][4];
    f32 scale;
    Point3d sp10;
    s32 i;
    s32 j;
    f32 temp_f30;
    f32 temp_f31;
    f32 cosFov;

    cam = &Hu3DCamera[Hu3DCameraNo];
    PSVECSubtract(&cam->pos, (Vec *)&arg0->unk20, &sp70);
    PSVECNormalize(&sp70, &sp70);
    PSMTXIdentity((f32 (*)[4])identMat);
    GXLoadPosMtxImm((f32 (*)[4])identMat, 0);
    PSMTXMultVecSR(mtx, &sp70, &sp64);
    if (!(sp64.z < 0.0f)) {
        HuSprTexLoad(arg0->anim[0], (s16)0, (s16)0, (GXTexWrapMode)1, (GXTexWrapMode)1, (GXTexFilter)1);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_TEX_ST, GX_RGBA8, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_RGBA6, 0);
        GXSetNumTevStages(1);
        GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
        GXSetZMode(1, GX_ALWAYS, 0);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_VTX, GX_SRC_VTX, 1, GX_DF_CLAMP, GX_AF_SPOT);
        GXSetZCompLoc(0);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
        GXSetAlphaUpdate(1);
        scale = 240.0f * lbl_1_rodata_F8[0].scale;
        spVec.x = sp64.x * scale;
        spVec.y = sp64.y * scale;
        spVec.z = -100.0f;
        fn_1_7654(arg0, &spVec, lbl_1_rodata_F8[0].color, lbl_1_rodata_F8[0].rate, 1.0f, lbl_1_rodata_F8[0].size);
        cosFov = (f32)cos((f64)(0.017453292f * cam->fov));
        temp_f31 = (sp64.z - cosFov) / (1.0f - cosFov);
        PSVECScale(&sp64, &sp64, 1.0f - cosFov);
        if (!(temp_f31 < 0.0f)) {
            spVec.x = arg0->unk20;
            spVec.y = arg0->unk24;
            spVec.z = arg0->unk28;
            sp10.x = 0.0f;
            sp10.y = 0.0f;
            sp10.z = 0.0f;
            fn_1_7A44(&spVec.y, arg0, mtx);
            for (i = 1; i < 4; i++) {
                HuSprTexLoad(arg0->anim[i], (s16)0, (s16)lbl_1_rodata_E8[i], (GXTexWrapMode)1, (GXTexWrapMode)1, (GXTexFilter)1);
            }
            for (j = 1; j < 8; j++) {
                scale = 240.0f * lbl_1_rodata_F8[j].scale;
                sp10.x = sp64.x * scale;
                sp10.y = sp64.y * scale;
                sp10.z = -100.0f;
                fn_1_7654(arg0, &sp10, lbl_1_rodata_F8[j].color, lbl_1_rodata_F8[j].rate, temp_f31, lbl_1_rodata_F8[j].size);
                scale = lbl_1_rodata_F8[j].size;
            }
            if (!(temp_f31 < 0.8f)) {
                GXClearVtxDesc();
                GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
                GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_TEX_ST, GX_RGBA8, 0);
                GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
                temp_f30 = 220.0f * ((temp_f31 - 0.8f) / 0.2f);
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                GXWGFifo.f32 = -320.0f;
                GXWGFifo.f32 = -240.0f;
                GXWGFifo.f32 = -100.0f;
                GXWGFifo.u8 = 0xFF;
                GXWGFifo.u8 = 0xFF;
                GXWGFifo.u8 = 0xFF;
                GXWGFifo.u8 = (u8)temp_f30;
                GXWGFifo.f32 = -320.0f;
                GXWGFifo.f32 = 240.0f;
                GXWGFifo.f32 = -100.0f;
                GXWGFifo.u8 = 0xFF;
                GXWGFifo.u8 = 0xFF;
                GXWGFifo.u8 = 0xFF;
                GXWGFifo.u8 = (u8)temp_f30;
                GXWGFifo.f32 = 320.0f;
                GXWGFifo.f32 = 240.0f;
                GXWGFifo.f32 = -100.0f;
                GXWGFifo.u8 = 0xFF;
                GXWGFifo.u8 = 0xFF;
                GXWGFifo.u8 = 0xFF;
                GXWGFifo.u8 = (u8)temp_f30;
                GXWGFifo.f32 = 320.0f;
                GXWGFifo.f32 = -240.0f;
                GXWGFifo.f32 = -100.0f;
                GXWGFifo.u8 = 0xFF;
                GXWGFifo.u8 = 0xFF;
                GXWGFifo.u8 = 0xFF;
                GXWGFifo.u8 = (u8)temp_f30;
            }
        }
    }
}

// NONMATCHING: register allocation. Semantics from the asm: r6 is the texture map index, the colour is
// bytes [r, g, b, a] = [c, c >> 8, c >> 16, scale * (c >> 24)], and the quad UVs are (0,0) (0,1) (1,1) (1,0).
void fn_1_7654(Mgr511Effring *arg1, Point3d *arg2, s32 color, s32 texMap, f32 scale, f32 size)
{
    f32 half;
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, (GXTexMapID)texMap, GX_COLOR0A0);
    r = (u8)color;
    g = (u8)(color >> 8);
    b = (u8)(color >> 16);
    a = (u8)(s32)(scale * (f32)(u8)((u32)color >> 24));
    half = (size * scale) + (scale * (4.0f * (f32)texMap));
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXWGFifo.f32 = arg2->x - half;
    GXWGFifo.f32 = arg2->y - half;
    GXWGFifo.f32 = arg2->z;
    GXWGFifo.u8 = r;
    GXWGFifo.u8 = g;
    GXWGFifo.u8 = b;
    GXWGFifo.u8 = a;
    GXWGFifo.f32 = 0.0f;
    GXWGFifo.f32 = 0.0f;
    GXWGFifo.f32 = arg2->x - half;
    GXWGFifo.f32 = arg2->y + half;
    GXWGFifo.f32 = arg2->z;
    GXWGFifo.u8 = r;
    GXWGFifo.u8 = g;
    GXWGFifo.u8 = b;
    GXWGFifo.u8 = a;
    GXWGFifo.f32 = 0.0f;
    GXWGFifo.f32 = 1.0f;
    GXWGFifo.f32 = arg2->x + half;
    GXWGFifo.f32 = arg2->y + half;
    GXWGFifo.f32 = arg2->z;
    GXWGFifo.u8 = r;
    GXWGFifo.u8 = g;
    GXWGFifo.u8 = b;
    GXWGFifo.u8 = a;
    GXWGFifo.f32 = 1.0f;
    GXWGFifo.f32 = 1.0f;
    GXWGFifo.f32 = arg2->x + half;
    GXWGFifo.f32 = arg2->y - half;
    GXWGFifo.f32 = arg2->z;
    GXWGFifo.u8 = r;
    GXWGFifo.u8 = g;
    GXWGFifo.u8 = b;
    GXWGFifo.u8 = a;
    GXWGFifo.f32 = 1.0f;
    GXWGFifo.f32 = 0.0f;
}

// NONMATCHING: the perspective divide is folded differently.
void fn_1_7A44(f32 *arg0, Mgr511Effring *arg1, Mtx mtx)
{
    Point3d sp1C;
    HU3DCAMERA *cam;
    f32 sinFov;
    f32 cosFov;
    f32 depth;

    cam = &Hu3DCamera[Hu3DCameraNo];
    PSMTXMultVec(mtx, (Vec *)&arg1->unk20, &sp1C);
    cosFov = (f32)cos((3.141592653589793 * (f64)(cam->fov * 0.5f)) / 180.0);
    sinFov = (f32)sin((3.141592653589793 * (f64)(cam->fov * 0.5f)) / 180.0);
    depth = (f32)((f64)sp1C.z * (sinFov / cosFov));
    /* asm: x uses 1.2 * depth (the aspect ratio), y the plain depth */
    arg0[0] = 288.0f + (sp1C.x * (288.0f / -(f32)(1.2000000476837158 * (f64)((f64)sp1C.z * (sinFov / cosFov)))));
    arg0[1] = 240.0f + (sp1C.y * (240.0f / depth));
    arg0[2] = 0.0f;
}

void fn_1_7C74(void)
{
}

}

/* The vtables this object defines: their entries are
   functions defined above, so the definitions sit here. */

/* lbl_1_data_284 is a vtable effring.o itself defines (6 words at .data+0x2c), and this unit is linked from source, so it has to be a definition rather than an extern declaration: with only the declaration the REL link reported "Failed to find symbol lbl_1_data_284 in any module". 2 leading words are zero and the rest are ADDR32 relocations to functions in this object. The definition is at the end of the unit because its entries are functions defined further down. */
extern "C" void *lbl_1_data_284[6] = {
    0,
    0,
    (void *) fn_1_6D50,
    (void *) fn_1_6E48,
    (void *) fn_1_7C74,
};

