#include "game/mg/seqman.h"
#include "game/charman.h"
#include "game/memory.h"
#include "game/data.h"

extern void (*_dtors[])(void);
extern void (*_ctors[])(void);
extern void ObjectSetup(void);
extern void fn_1_5F90(OMOBJ *);
extern void *lbl_1_bss_1B8[];
extern s16 lbl_1_bss_198;
extern u32 lbl_1_data_A4;
extern void *lbl_1_bss_158;
extern s32 lbl_1_bss_5C8[];
extern float lbl_1_bss_1D0[];
extern void *lbl_1_bss_19C;
extern void *lbl_1_bss_1B4;
extern void *lbl_1_bss_1C8;
extern void *lbl_1_bss_1CC;
extern void fn_1_7ED8(OMOBJ *);
extern void fn_1_8A98(OMOBJ *);
extern void fn_1_9860(OMOBJ *);
extern void *HuAR_ARAMtoMRAMFileRead(unsigned int dataNum, u32 num, HEAPID heap);
extern s32 lbl_1_bss_190;
extern s32 lbl_1_bss_16C;
extern s32 lbl_1_bss_164;
extern void *lbl_1_bss_168;
extern void *lbl_1_bss_160;
extern s32 lbl_1_data_9C;
extern u32 lbl_1_data_A8[];
extern f32 CZoom;
extern HuVecF Center;
extern HuVecF CRot;
extern u16 HuPadBtnDown[];
extern s32 MgPlayerAttrCheck(void *, s32);
extern void MgPlayerModeSet(void *, s32);
extern HU3DMODEL Hu3DData[];
extern void Hu3DMotionTimeSet(HU3DMOTID motId, f32 time);
extern u32 lbl_1_data_100;
static const Vec kLightPos = {0.0f, 5000.0f, 0.0f};
static const Vec kLightDir = {0.0f, -1.0f, -0.5f};
static const GXColor kLightCol = {0xFF, 0xFF, 0xFF, 0xFF};
static const Vec kShadowPos = {0.0f, 10000.0f, 0.0f};
static const Vec kShadowUp = {0.0f, 1.0f, 0.0f};
static const Vec kShadowTarget = {0.0f, 0.0f, -0.1f};

void fn_1_5F60(void *unk, s16 arg0)
{
    if (arg0 >= 0x3C) {
        MgSeqModeNext();
    }
}

void _epilog(void)
{
    void (**dtor)(void) = _dtors;
    goto check;
    
call:
    (*dtor)();
    dtor++;
    
check:
    if ((u32)*dtor != 0) {
        goto call;
    }
}

int _prolog(void)
{
    void (**ctor)(void) = _ctors;
    goto check;
    
call:
    (*ctor)();
    ctor++;
    
check:
    if ((u32)*ctor != 0) {
        goto call;
    }
    ObjectSetup();
    return 0;
}

void fn_1_B04(void *arg0)
{
    if (*(unsigned short *)((unsigned char *)arg0 + 0x10) == 0xC) {
        return;
    }
    
    CharMotionShiftSet(
        *(short *)((unsigned char *)arg0 + 0x2),
        *(short *)((unsigned char *)*(void **)((unsigned char *)*(void **)((unsigned char *)arg0 + 0x8) + 0x48) + 0x18),
        0.0f, 1.0f, 0
    );
    *(unsigned short *)((unsigned char *)arg0 + 0x10) = 0xC;
}

void fn_1_A94(void *arg0)
{
    if (*(unsigned short *)((unsigned char *)arg0 + 0x10) == 0xB) {
        return;
    }
    
    CharMotionShiftSet(
        *(short *)((unsigned char *)arg0 + 0x2),
        *(short *)((unsigned char *)*(void **)((unsigned char *)*(void **)((unsigned char *)arg0 + 0x8) + 0x48) + 0x16),
        0.0f, 1.0f, 0x40000001
    );
    *(unsigned short *)((unsigned char *)arg0 + 0x10) = 0xB;
}

void fn_1_A08(void *arg0)
{
    if (*(unsigned short *)((unsigned char *)arg0 + 0x10) == 0xA) {
        goto skip_motion;
    }
    
    CharMotionShiftSet(
        *(short *)((unsigned char *)arg0 + 0x2),
        *(short *)((unsigned char *)*(void **)((unsigned char *)*(void **)((unsigned char *)arg0 + 0x8) + 0x48) + 0x14),
        0.0f, 5.0f, 0
    );
    *(unsigned short *)((unsigned char *)arg0 + 0x10) = 0xA;
    
skip_motion:
    *(float *)((unsigned char *)*(void **)((unsigned char *)arg0 + 0x3C) + 0x68) *= 0.7;
}

void fn_1_9860(OMOBJ *arg0)
{
    *(void (**)(OMOBJ *))((unsigned char *)arg0 + 0x14) = fn_1_5F90;
}

void fn_1_5F48(void)
{
}

void fn_1_5F4C(void)
{
}

void fn_1_5F50(void)
{
}

void fn_1_5F54(void)
{
}

void fn_1_5F58(void)
{
}

void fn_1_5F5C(void)
{
}

// NONMATCHING: the original allocates four live FPR pairs (f28..f31) plus a
// different stack layout for r30/r31; the fabs+frsp chain and the awkward
// stack home for the mode variables cannot be reproduced from this C
void fn_1_698(void *arg0)
{
    if (*(u32 *)((u8 *)*(void **)((u8 *)arg0 + 0x3C) + 0x84) & 0x407F) {
        if (!(u16)MgPlayerAttrCheck(arg0, 1)) {
            if (HuPadBtnDown[*(s8 *)((u8 *)arg0 + 4)] & 0x100) {
                if (*(u32 *)((u8 *)arg0 + 0xC) & 0x2) {
                    MgPlayerModeSet(arg0, 1);
                }
            }
        }
    } else {
        if ((f32)fabs(*(f32 *)((u8 *)*(void **)((u8 *)arg0 + 0x3C) + 0x70)) > 2000.0f) {
            MgPlayerModeSet(arg0, 2);
        }
    }
    if (*(u16 *)((u8 *)arg0 + 0x10) == 0xB) {
        return;
    }
    if (lbl_1_data_100 == 0) {
        CharMotionShiftSet(
            *(s16 *)((u8 *)arg0 + 2),
            10.0f + 10.0f * *(f32 *)((u8 *)lbl_1_bss_5C8 + 0xC),
            10.0f + 10.0f * *(f32 *)((u8 *)lbl_1_bss_5C8 + 0xC),
            12.0f, 0x40000002
        );
        *(u16 *)((u8 *)arg0 + 0x10) = 0xB;
    } else {
        lbl_1_data_100 -= (lbl_1_data_100 > 0) ? 1 : 0;
    }
}

// NONMATCHING: interpolation curve. The original keeps four FPRs live across the
// branches (frame -0x50) and puts the variable operand first in every fmuls;
// MWCC orders the loaded constant first (frame -0x30).
f32 fn_1_41F8(f32 a, f32 b, f32 c, f32 d)
{
    f32 t;
    f32 ret;

    t = d * 0.5f + (b * 0.5f + c);
    if (a < b) {
        ret = (a + 1.0f) * 0.5f * a / b / t;
    } else if (a < b + c) {
        ret = (b + 1.0f) * 0.5f * b / b / t + (a - b) / t;
    } else if (a < d + (b + c) - 1.0f) {
        ret = 1.0f - ((d + (b + c) - a) * 0.5f * (d + (b + c) - a - 2.0f) / (d - 1.0f)) / t;
    } else {
        ret = 1.0f;
    }
    return ret;
}

// NONMATCHING: original spills each array element to 0x8(r1) via a stack local;
// no ordering of four locals reproduces the target's register allocation (r31=i,
// r29=bss_1B8 base, r30=bss_198, r28=temp). The spill to 0x8(r1) is dead code.
void fn_1_9E18(void *arg0)
{
    s32 i;
    s16 bss_198 = lbl_1_bss_198;
    s32 *bss_1B8 = (s32 *)lbl_1_bss_1B8;

    MgSeqModeNext();
    for (i = 0; i < 4; i++) {
        *(s32 *)((unsigned char *)arg0 + 0x8) = bss_1B8[i * 0x17];
    }
    if ((s32)bss_198 != 0) {
        GameMesKill(bss_198);
    }
    GameMesClose();
    HuAudAllStop();
    omOvlReturnEx((s16)lbl_1_data_A4, 1);
}

// NONMATCHING: frame size and register allocation differ; original keeps arg0 in r31
// and half_26 in r30, avoiding stack spills that C cannot avoid
void fn_1_90C(void *arg0)
{
    void (*func)(void) = *(void (**)(void))lbl_1_bss_158;
    func();
    if (*(s16 *)((unsigned char *)arg0 + 0x26) != 2) {
        return;
    }
    if (*(s16 *)((unsigned char *)arg0 + 0x26) != 3) {
        return;
    }
    if (lbl_1_bss_5C8[0x14/4] != 0) {
        return;
    }
    lbl_1_bss_5C8[0x14/4] = 1;
    if (lbl_1_bss_5C8[0x18/4] != 0) {
        return;
    }
    lbl_1_bss_5C8[0x18/4] = 1;
    if (lbl_1_bss_1D0[0x64/4] < 3) {
        s32 idx = (s32)lbl_1_bss_1D0[0x64/4];
        lbl_1_bss_1D0[0x68/4 + idx] = 0.0f;
        lbl_1_bss_1D0[0x64/4] = idx + 1;
    }
}

void fn_1_9CE0(void)
{
    OMOBJ *obj;
    s32 i;

    obj = omAddObjEx(lbl_1_bss_19C, 0x64, 0x16, 0x2, 0x1, fn_1_7ED8);
    *(s32 *)lbl_1_bss_1B4 = (s32)obj;
    for (i = 0; i < 4; i++) {
        obj = omAddObjEx(lbl_1_bss_19C, 0x66, 0x2, 0x3, 0x0, fn_1_8A98);
        *(s32 *)((s8 *)lbl_1_bss_1B8 + i * 4) = (s32)obj;
        *(s32 *)((s8 *)obj + 0x5C) = (s32)HuMemDirectMallocNum(0, 0x54, 0x1000);
        *(s16 *)((s8 *)obj + 0x4C) = (s16)i;
    }
    obj = omAddObjEx(lbl_1_bss_19C, 0x7FDA, 0x0, 0x0, 0x0, omOutView);
    *(s32 *)lbl_1_bss_1C8 = (s32)obj;
    obj = omAddObjEx(lbl_1_bss_19C, 0x67, 0x0, 0x0, 0x0, fn_1_9860);
    *(s32 *)lbl_1_bss_1CC = (s32)obj;
    MgSeqModeNext();
}

// NONMATCHING: constant load order in the camera/light/shadow setup differs
void ObjectSetup(void)
{
    OMOBJMAN *mgr;
    Vec pos;
    Vec dir;
    Vec spos;
    Vec unk18;
    Vec unk0C;
    GXColor color;

    OSReport("******* M520 ObjectSetup *********\n");
    mgr = (OMOBJMAN *)MgActorObjectSetup();
    lbl_1_bss_19C = mgr;

    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 20.0f, 20.0f, 25000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(1, 0, 0, 640, 480);

    CZoom = 5000.0f;
    Center.x = 0.0f;
    Center.y = 30.0f;
    Center.z = 0.0f;

    CRot.x = -70.0f;
    CRot.y = 0.0f;
    CRot.z = 0.0f;

    CZoom = 1425.0f;
    Center.x = 0.0f;
    Center.y = 505.0f;
    Center.z = 0.0f;

    CRot.x = -35.0f;
    CRot.y = 0.0f;
    CRot.z = 0.0f;

    Hu3DLighInit();

    pos = kLightPos;
    dir = kLightDir;
    color = kLightCol;
    lbl_1_bss_190 = (s32)(s16)(s32)Hu3DGLightCreateV(&pos, &dir, &color);
    Hu3DGLightStaticSet((s16)lbl_1_bss_190, 1);
    Hu3DGLightInfinitytSet((s16)lbl_1_bss_190);

    spos.x = 0.0f;
    spos.y = 10000.0f;
    spos.z = 0.0f;
    unk18.x = 0.0f;
    unk18.y = 1.0f;
    unk18.z = 0.0f;
    unk0C.x = 0.0f;
    unk0C.y = 0.0f;
    unk0C.z = -0.1f;
    Hu3DShadowCreate(8.5f, 5000.0f, 13000.0f);
    Hu3DShadowPosSet(&spos, &unk18, &unk0C);

    lbl_1_bss_198 = -1;
    lbl_1_data_9C = -1;

    lbl_1_bss_16C = GXGetTexBufferSize(640, 480, 6, 0, 0);
    lbl_1_bss_168 = HuMemDirectMallocNum(2, lbl_1_bss_16C, HU_MEMNUM_OVL);
    lbl_1_bss_164 = GXGetTexBufferSize(32, 32, 3, 0, 0);
    lbl_1_bss_160 = HuMemDirectMallocNum(2, lbl_1_bss_164, HU_MEMNUM_OVL);

    MgSeqCreate((MGSEQ_PARAM *)lbl_1_data_A8);
}

// NONMATCHING: frame uses _savegpr_27/_restgpr_27 for 5 GPRs; register allocation
// of loop counters (r27-r31) differs from CodeWarrior; complex control flow
// with three search loops cannot be expressed in plain C without register shifts
void fn_1_8918(s16 arg0, u16 arg1)
{
    u16 r28;
    u16 r30;
    u16 r31;
    void *r29;
    s16 r0;
    u32 val;

    r28 = (u16)(arg1 & 0xFFFF);
    r30 = (u16)(arg1 >> 16);
    r31 = 0;

    while (r31 < 0xD) {
        if (r28 == charDataDirTbl[r31][4]) {
            break;
        }
        r31++;
    }

    if (r31 == 0xD) {
        if (r28 == 0) {
            return;
        }
    }

    r30 = r30 & 0xFFFF;
    r0 = (s16)arg0 * 0x18;
    val = charDataDirTbl[r0 / 4][4];
    r30 |= val;
    r29 = HuAR_ARAMtoMRAMFileRead(r30, 0x1000, 2);

    if (r29 == 0) {
        HuDataSelHeapReadNum(r30, 0x1000, 2);
    } else {
        HuDataReadNumHeapShortForce(r30, 0x1000, 2);
    }
    return;
}
