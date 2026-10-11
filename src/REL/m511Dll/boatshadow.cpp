#include "dolphin.h"

#include "datadir_enum.h"
#include "game/data.h"
#include "game/hu3d.h"
#include "game/sprite.h"

typedef u8 bool8;

#ifndef GX_N3
#define GX_N3 1
#endif

/* These are plain C in the original: m511.o defines them unmangled
   (m511.cpp is NonMatching, so the module links the original object).
   Left as C++ they mangle and the REL link fails with
   "Failed to find symbol fn_1_1D88__FPv in any module". */
extern "C" void *fn_1_1D88(void *self);
extern "C" void fn_1_1D94(void *self, s32 arg1);
extern "C" void fn_1_1E54(void *self, void *arg1);
extern "C" void fn_1_CF8(char *cond, char *file, s32 line);
extern "C" void *__dl__FPv(void *self);
/* The original calls the C-linkage symbol: the target's call target is plain
   HuSprTexLoad, not a CodeWarrior-mangled HuSprTexLoad__FP10AnimData_ssslll.
   Without extern "C" the C++ declaration below mangles and the branch differs. */
extern "C" void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, s32 wrapS, s32 wrapT, s32 filter);

struct Mgr510Shadow;


struct Mgr510Singleton {
    u32 unk0;
    u32 unk4;
    void **vtable;
    s32 unkC;
    ANIMDATA *unk10;
    ANIMDATA *unk14;
    ANIMDATA *unk18;
    ANIMDATA *unk1C;
    ANIMDATA *unk20;
};

struct Mgr510Shadow {
    ANIMDATA *unk0;
    s16 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
};

extern "C" {

/* lbl_1_bss_2C is defined by m511.o, which references it as UND here. Defining it too makes
   the linker report it multiply-defined. */
extern "C" void *lbl_1_bss_2C[2];

extern void *lbl_1_data_41C[5];
/* lbl_1_data_40 is boat.o's, and the original boatshadow.o references it UND:
   defining it here too makes the linker report it multiply-defined. */
extern "C" u8 lbl_1_data_40[8];

void fn_1_C1CC(Mgr510Shadow *arg0, f32 (*arg1)[4]);
void fn_1_C038(HU3DMODEL *arg0, Mtx *arg1);

Mgr510Singleton *lbl_1_bss_A38;

Mgr510Singleton *fn_1_BD18(Mgr510Singleton *arg0)
{
    Mgr510Singleton *var_r29;

    fn_1_1D88(arg0);
    arg0->vtable = (void **)lbl_1_data_40;
    arg0->unkC = 0;
    fn_1_1E54(arg0, &lbl_1_bss_2C);
    if (lbl_1_bss_A38 == NULL) {
        fn_1_CF8("ms_pointer == 0", "singleton.h", 0x35);
    }
    var_r29 = (Mgr510Singleton *)((u8 *)arg0 + 0x10);
    if (var_r29 != NULL) {
        var_r29 -= 0x10;
    }
    lbl_1_bss_A38 = var_r29;
    arg0->vtable = lbl_1_data_41C;
    arg0->unk10 = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m511, 0x29), HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(arg0->unk10);
    arg0->unk14 = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m511, 0x28), HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(arg0->unk14);
    arg0->unk18 = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m511, 0x2A), HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(arg0->unk18);
    arg0->unk1C = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m511, 0x30), HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(arg0->unk1C);
    arg0->unk20 = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m511, 0x2E), HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(arg0->unk20);
    return arg0;
}

Mgr510Singleton *fn_1_BE9C(Mgr510Singleton *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        if (lbl_1_bss_A38 == NULL) {
            fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A38 = (Mgr510Singleton *)NULL;
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

Mgr510Singleton *fn_1_BF28(Mgr510Singleton *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        arg0->vtable = lbl_1_data_41C;
        HuSprAnimKill(arg0->unk10);
        HuSprAnimKill(arg0->unk14);
        HuSprAnimKill(arg0->unk18);
        HuSprAnimKill(arg0->unk1C);
        HuSprAnimKill(arg0->unk20);
        if ((Mgr510Singleton *)((u8 *)arg0 + 0x10) != NULL) {
            if (lbl_1_bss_A38 == NULL) {
                fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
            }
            lbl_1_bss_A38 = (Mgr510Singleton *)NULL;
        }
        if (arg0 != NULL) {
            arg0->vtable = (void **)lbl_1_data_40;
            if (arg0 != NULL) {
                fn_1_1D94(arg0, 0);
            }
        }
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_C01C(Mgr510Singleton *arg0, s32 arg1)
{
    arg0->unkC &= ~(1 << arg1);
}

void fn_1_C034(void)
{
}

void fn_1_C038(HU3DMODEL *arg0, Mtx *arg1)
{
    fn_1_C1CC((Mgr510Shadow *)(*(void **)((u8 *)arg0 + 0x120)), (f32 (*)[4])arg1);
}

Mgr510Shadow *fn_1_C07C(Mgr510Shadow *arg0)
{
    s16 temp_r3;

    temp_r3 = Hu3DHookFuncCreate(fn_1_C038);
    Hu3DData[temp_r3].hookData = arg0;
    arg0->unk4 = temp_r3;
    Hu3DModelLayerSet(temp_r3, 3);
    Hu3DModelCameraSet(temp_r3, 0xF);
    arg0->unk0 = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m511, 0x2D), HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(arg0->unk0);
    return arg0;
}

Mgr510Shadow *fn_1_C124(Mgr510Shadow *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        Hu3DModelKill(arg0->unk4);
        HuSprAnimKill(arg0->unk0);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_C180(Mgr510Shadow *arg0, Vec *arg1, Vec *arg2)
{
    arg0->unk8 = arg1->x;
    arg0->unkC = arg1->y;
    arg0->unk10 = arg1->z;
    arg0->unk14 = arg2->x;
    arg0->unk18 = arg2->y;
    arg0->unk1C = arg2->z;
    arg0->unkC = arg0->unkC + 10.0f;
}

void fn_1_C1CC(Mgr510Shadow *arg0, f32 (*arg1)[4])
{
    /* Both locals are 0x30 bytes in the target (0xc and 0x3c), which is f32[3][4]
       and not f32[4][4]; the wider spelling pushes the frame from 0x80 to 0xa0. */
    Mtx sp3C;
    Mtx spC;

    PSMTXTrans(sp3C, arg0->unk8, arg0->unkC, arg0->unk10);
    PSMTXRotRad(spC, 'Y', 0.017453292f * arg0->unk18);
    PSMTXConcat(sp3C, spC, sp3C);
    PSMTXConcat(arg1, sp3C, sp3C);
    GXLoadPosMtxImm(sp3C, 0);
    HuSprTexLoad(arg0->unk0, 0, 0, 1, 1, 1);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZCompLoc(1);
    GXSetAlphaUpdate(1);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_RGBA6, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_RGBA6, 0);
    GXSetZMode(1, GX_LEQUAL, 0);
    GXSetCullMode(GX_CULL_NONE);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    *(volatile f32 *)0xCC008000 = -80.0f;
    *(volatile f32 *)0xCC008000 = 0.0f;
    *(volatile f32 *)0xCC008000 = -160.0f;
    *(volatile f32 *)0xCC008000 = 0.0f;
    *(volatile f32 *)0xCC008000 = 1.0f;
    *(volatile f32 *)0xCC008000 = -80.0f;
    *(volatile f32 *)0xCC008000 = 0.0f;
    *(volatile f32 *)0xCC008000 = 200.0f;
    *(volatile f32 *)0xCC008000 = 0.0f;
    *(volatile f32 *)0xCC008000 = 0.0f;
    *(volatile f32 *)0xCC008000 = 80.0f;
    *(volatile f32 *)0xCC008000 = 0.0f;
    *(volatile f32 *)0xCC008000 = 200.0f;
    *(volatile f32 *)0xCC008000 = 1.0f;
    *(volatile f32 *)0xCC008000 = 0.0f;
    *(volatile f32 *)0xCC008000 = 80.0f;
    *(volatile f32 *)0xCC008000 = 0.0f;
    *(volatile f32 *)0xCC008000 = -160.0f;
    *(volatile f32 *)0xCC008000 = 1.0f;
    *(volatile f32 *)0xCC008000 = 1.0f;
}

void *lbl_1_data_41C[5] = {
    NULL,
    NULL,
    (void *)fn_1_BF28,
    (void *)fn_1_C01C,
    (void *)fn_1_C034,
};

}
