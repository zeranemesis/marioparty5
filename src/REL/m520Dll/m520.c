#include "game/mg/seqman.h"
#include "game/charman.h"
#include "game/memory.h"
#include "game/data.h"

extern void (*_dtors[])(void);
extern void (*_ctors[])(void);
extern void ObjectSetup(void);
extern void fn_1_5F90(void);
extern void *lbl_1_bss_1B8[];
extern s16 lbl_1_bss_198;
extern u32 lbl_1_data_A4;
extern void *lbl_1_bss_158;
s32 lbl_1_bss_5C8[0x200 / 4];
s32 lbl_1_bss_1D0[0xA0 / 4];
extern void *lbl_1_bss_19C;
extern s32 lbl_1_bss_1A0;
extern s32 lbl_1_bss_1A4[];
extern s32 lbl_1_data_A0;
/* the m2c draft reads these two as saved-register scratch and the matrices it
 * builds for the ortho overlay pass */
s32 saved_reg_r29;
s32 saved_reg_r30;
extern Mtx lbl_1_bss_18;
extern Mtx lbl_1_bss_48;
extern u8 RenderMode[];
extern u8 lbl_1_rodata_460[];
extern s32 lbl_1_bss_280[];
extern s32 lbl_1_bss_164;
extern void *lbl_1_bss_160;
extern u8 lbl_1_data_EC[];
extern Mtx lbl_1_bss_B8;
extern Mtx lbl_1_bss_E8;
extern s32 lbl_1_bss_4;
extern GXTexObj lbl_1_bss_118[];
extern u8 lbl_1_rodata_49C[];
extern Mtx lbl_1_bss_A8;
extern Mtx lbl_1_bss_98;
extern s32 lbl_1_bss_17C;
SHARED_SYM extern u8 esprite[];
SHARED_SYM extern u8 espanim[];
extern u8 lbl_1_rodata_4D0[];
extern s8 lbl_1_bss_270[];
extern void fn_1_43BC(OMOBJ *);
extern s32 lbl_1_bss_0;
extern s32 lbl_1_bss_14;
extern u8 lbl_1_rodata_344[];
extern u8 lbl_1_rodata_36C[];
extern u8 lbl_1_rodata_4F0[];
extern s32 lbl_1_bss_1A4[];
extern s32 lbl_1_bss_15C;
extern void *lbl_1_data_138;
extern void fn_1_B70(OMOBJ *);
extern s32 lbl_1_data_D0;
extern s32 lbl_1_bss_10;
extern f32 lbl_1_bss_C;

/*
 * Bakes the 32-step ring texture used by the overlay: sets up an ortho pass,
 * emits a GX_TRIANGLESTRIP straight into the write-gather pipe, then copies
 * the result out with GXCopyTex and restores the copy filter. lbl_1_bss_164
 * doubles as the "texture dirty" flag and as the flush size.
 */
void fn_1_7830(HU3DMODEL *modelP, f32 (*mtx)[3][4])
{
    s32 r29;
    s32 r30;
    s32 r31;

    r29 = saved_reg_r29;
    r30 = saved_reg_r30;
    if (lbl_1_bss_164 != 0) {
        C_MTXOrtho(lbl_1_bss_18, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f);
        GXSetProjection(lbl_1_bss_18, GX_ORTHOGRAPHIC);
        PSMTXIdentity(lbl_1_bss_48);
        GXLoadPosMtxImm(lbl_1_bss_48, 0);
        GXSetNumTevStages(1);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
        GXSetZMode(0, GX_ALWAYS, 0);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_TEX_ST, GX_RGBA8, 0);
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 0x42);
        for (r31 = 0; r31 < 0x20; r31++) {
            f32 t = (f32) r31 * 0.03125f;

            r30 = (s32) (128.0 + (127.0 * cos((M_PI * (360.0f * t)) / 180.0)));
            r29 = (s32) (128.0 + (127.0 * cos((M_PI * (2.0f * (360.0f * t))) / 180.0)));
            *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
            *(f32 *) (uintptr_t) 0xCC008000 = (f32) r31 / 480.0f;
            *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
            *(u32 *) (uintptr_t) 0xCC008000 = 0;
            *(u8 *) (uintptr_t) 0xCC008000 = (u8) r29;
            *(u8 *) (uintptr_t) 0xCC008000 = (u8) r30;
            *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
            *(f32 *) (uintptr_t) 0xCC008000 = 0.05f;
            *(f32 *) (uintptr_t) 0xCC008000 = (f32) r31 / 480.0f;
            *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
            *(u32 *) (uintptr_t) 0xCC008000 = 0;
            *(u8 *) (uintptr_t) 0xCC008000 = (u8) r29;
            *(u8 *) (uintptr_t) 0xCC008000 = (u8) r30;
            *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
        }
        *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
        *(f32 *) (uintptr_t) 0xCC008000 = (f32) r31 / 480.0f;
        *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
        *(u32 *) (uintptr_t) 0xCC008000 = 0;
        *(u8 *) (uintptr_t) 0xCC008000 = (u8) r29;
        *(u8 *) (uintptr_t) 0xCC008000 = (u8) r30;
        *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
        *(f32 *) (uintptr_t) 0xCC008000 = 0.05f;
        *(f32 *) (uintptr_t) 0xCC008000 = (f32) r31 / 480.0f;
        *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
        *(u32 *) (uintptr_t) 0xCC008000 = 0;
        *(u8 *) (uintptr_t) 0xCC008000 = (u8) r29;
        *(u8 *) (uintptr_t) 0xCC008000 = (u8) r30;
        *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
        GXSetCopyFilter(0, (void *) (RenderMode + 0x1A), 0, (void *) (RenderMode + 0x32));
        GXSetTexCopySrc(0, 0, 0x20, 0x20);
        GXSetTexCopyDst(0x20, 0x20, GX_CTF_GB8, 0);
        GXCopyTex(lbl_1_bss_160, 0);
        DCFlushRange(lbl_1_bss_160, (u32) lbl_1_bss_164);
        GXPixModeSync();
        GXSetCopyFilter(RenderMode[0x19], (void *) (RenderMode + 0x1A), 1, (void *) (RenderMode + 0x32));
        lbl_1_bss_164 = 0;
    }
}
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
extern s32 fontcolor;
/* fn_1_A0 reads the pad directly; game/pad.h declares the arrays it needs. */
#include "game/pad.h"

/*
 * Free-camera debug update: reads the pad and nudges the camera pivot
 * (Center), the zoom (CZoom) and the orbit angles (CRot), prints them, then
 * converts the spherical orbit back to a cartesian eye position. The pad bit
 * extractions and the double-precision trigonometry are what the target does;
 * the constants are lbl_1_rodata_10 (25, the zoom step), _18 (0.03125, the
 * horizontal nudge), _20 (-32, the vertical nudge divisor), _30/_38 (PI/180).
 */
void fn_1_A0(void)
{
    f32 cx;
    f32 cy;
    f32 cz;
    f32 rx;
    f32 ry;
    f32 rz;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 sp18;
    f32 sp1C;
    f32 sp20;
    f32 sp24;
    f32 sp28;
    f32 sp2C;

    cx = Center.x + (25.0 * (f64) ((s32) ((HuPadBtn[0] & 2) != 0) - (s32) ((HuPadBtn[0] & 1) != 0)));
    Center.x = cx;
    cy = Center.y + (25.0 * (f64) ((s32) ((HuPadBtn[0] & 8) != 0) - (s32) ((HuPadBtn[0] & 4) != 0)));
    Center.y = cy;
    cz = CZoom + (25.0 * (f64) ((s32) ((HuPadBtn[0] & 0x400) != 0) - (s32) ((HuPadBtn[0] & 0x800) != 0)));
    CZoom = cz;
    ry = CRot.y + ((f32) HuPadSubStkX[0] * 0.03125f);
    CRot.y = ry;
    rx = CRot.x + ((f32) HuPadSubStkY[0] / -32.0f);
    CRot.x = rx;
    rz = CRot.z;
    fontcolor = 0xF;
    print8(0x40, 0x188, 2.0f, "%f,%f,%f", cx, cy, cz);
    print8(0x40, 0x198, 2.0f, "%f,%f,%f", rx, ry, rz);
    print8(0x40, 0x1A8, 2.0f, "%f", cz);
    /* spherical orbit back to a cartesian eye position; the target keeps these
     * three in stack slots rather than a named struct */
    dx = Center.x + (cz * (f32) ((f64) sin(((3.141592653589793 * rx) / 180.0)) * (f64) cos(((3.141592653589793 * ry) / 180.0))));
    dz = Center.y + (cz * (f32) -((f64) sin(((3.141592653589793 * rx) / 180.0))));
    dy = Center.z + (cz * (f32) ((f64) cos(((3.141592653589793 * rx) / 180.0)) * (f64) cos(((3.141592653589793 * ry) / 180.0))));
    sp24 = dx;
    sp28 = dz;
    sp2C = dy;
    sp18 = Center.x;
    sp1C = Center.y;
    sp20 = Center.z;
}
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
    if ((u32) (uintptr_t) *dtor != 0) {
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
    if ((u32) (uintptr_t) *ctor != 0) {
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
    *(void (**)(void))((unsigned char *)arg0 + 0x14) = fn_1_5F90;
}

/*
 * Main update: mirrors each Mg actor's position onto its shadow model, runs the
 * per-slot sound effects, then dispatches on the sequence mode. The trailing
 * jump table is lbl_1_data_104 (12 entries, mode 0..0xB); the ordering of the
 * cases below is the table's order, not a switch statement's.
 */
void fn_1_5F90(void)
{
    s32 sp44[8];
    s32 prevObj[4];
    s32 frame;
    s32 mode;
    s32 i;
    s32 j;

    frame = MgSeqFrameNoGet();
    lbl_1_bss_1A0 = MgSeqModeGet();
    for (i = 0; i < 4; i++) {
        prevObj[i] = ((s32 *) lbl_1_bss_1B8[i])[0x5C / 4];
    }
    MgActorExec();
    for (i = 0; i < 4; i++) {
        s32 actor = ((s32 *) (uintptr_t) lbl_1_bss_1A4)[i];
        HuVecF pos;

        MgActorPosGet(((s32 *) (uintptr_t) actor)[0x3C / 4], &pos);
        if (i == 0 && ((s32 *) (uintptr_t) prevObj[i])[0x1C / 4] == 3) {
            continue;
        }
        ((f32 *) lbl_1_bss_1B8[i])[0x18 / 4] = pos.x;
        ((f32 *) lbl_1_bss_1B8[i])[0x1C / 4] = pos.y;
        ((f32 *) lbl_1_bss_1B8[i])[0x20 / 4] = pos.z;
        ((f32 *) lbl_1_bss_1B8[i])[0x28 / 4] = ((f32 *) (uintptr_t) actor)[0x68 / 4];
    }
    for (i = 0; i < 8; i++) {
        f32 slot[8];

        slot[0] = 1826.0f;
        slot[1] = 1827.0f;
        slot[2] = 1828.0f;
        slot[3] = 1829.0f;
        slot[4] = 1830.0f;
        slot[5] = 1831.0f;
        slot[6] = 1832.0f;
        slot[7] = 1833.0f;
        if (((s32 *) ((u8 *) lbl_1_bss_1D0 + (i * 0x14)))[0] > 3) {
            ((s32 *) ((u8 *) lbl_1_bss_1D0 + (i * 0x14)))[0] = 3;
        }
        if (((s32 *) ((u8 *) lbl_1_bss_1D0 + (i * 0x14)))[0] <= 0) {
            continue;
        }
        ((s32 *) ((u8 *) lbl_1_bss_1D0 + (i * 0x14)))[0x10 / 4] = HuAudFXPlay((u32) slot[i]);
        ((s32 *) ((u8 *) lbl_1_bss_1D0 + (i * 0x14)))[0] -= 1;
    }
    mode = lbl_1_bss_1A0;
    switch (mode) {
    case 0:
        if (WipeCheckEnd() == 0) {
            MgSeqModeNext();
        }
        break;
    case 1:
        if (frame == 0xA) {
            lbl_1_data_A0 = HuAudSStreamPlay(0x50);
        }
        break;
    case 2:
        if (((s32 *) (uintptr_t) prevObj[0])[0x1C / 4] != 3 || ((s32 *) (uintptr_t) prevObj[1])[0x1C / 4] != 3
            || ((s32 *) (uintptr_t) prevObj[2])[0x1C / 4] != 3) {
            break;
        }
        lbl_1_data_9C = 0;
        MgSeqModeNext();
        break;
    case 3:
        if (frame != 1) {
            break;
        }
        if (((s32 *) ((u8 *) lbl_1_bss_1D0 + 0x24))[0] != -1) {
            HuAudFXStop(((s32 *) ((u8 *) lbl_1_bss_1D0 + 0x24))[0]);
        }
        if (lbl_1_data_A0 != -1) {
            HuAudSStreamFadeOut(lbl_1_data_A0, 0x64);
        }
        if (((s32 *) (uintptr_t) prevObj[0])[0x1C / 4] == 3 && ((s32 *) (uintptr_t) prevObj[1])[0x1C / 4] == 3
            && ((s32 *) (uintptr_t) prevObj[2])[0x1C / 4] == 3) {
            lbl_1_data_9C = 0;
            MgSeqWinnerSet((s16) ((s32 *) (uintptr_t) prevObj[0])[0xC / 4], -1, -1, -1);
        } else {
            lbl_1_data_9C = 1;
            MgSeqWinnerSet((s16) ((s32 *) (uintptr_t) prevObj[0])[0xC / 4], (s16) ((s32 *) (uintptr_t) prevObj[1])[0xC / 4],
                           (s16) ((s32 *) (uintptr_t) prevObj[2])[0xC / 4], -1);
        }
        break;
    case 4:
        if (frame != 0xA) {
            break;
        }
        j = lbl_1_data_9C;
        if (_CheckFlag(0x10F) == 0) {
            *(s16 *) ((u8 *) &GwPlayer[((s32 *) (uintptr_t) prevObj[j])[4 / 4]] + 0x2C) = 0xA;
        }
        if (lbl_1_data_9C == 1) {
            if (_CheckFlag(0x10F) == 0) {
                *(s16 *) ((u8 *) &GwPlayer[((s32 *) (uintptr_t) prevObj[1])[4 / 4]] + 0x2C) = 0xA;
            }
            if (_CheckFlag(0x10F) == 0) {
                *(s16 *) ((u8 *) &GwPlayer[((s32 *) (uintptr_t) prevObj[2])[4 / 4]] + 0x2C) = 0xA;
            }
        }
        if (frame >= 0x46) {
            MgSeqModeNext();
        }
        break;
    default:
        break;
    }
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
    *(s32 *)lbl_1_bss_1B4 = (s32) (uintptr_t) obj;
    for (i = 0; i < 4; i++) {
        obj = omAddObjEx(lbl_1_bss_19C, 0x66, 0x2, 0x3, 0x0, fn_1_8A98);
        *(s32 *)((s8 *)lbl_1_bss_1B8 + i * 4) = (s32) (uintptr_t) obj;
        *(s32 *)((s8 *)obj + 0x5C) = (s32) (uintptr_t) HuMemDirectMallocNum(0, 0x54, 0x1000);
        *(s16 *)((s8 *)obj + 0x4C) = (s16)i;
    }
    obj = omAddObjEx(lbl_1_bss_19C, 0x7FDA, 0x0, 0x0, 0x0, omOutView);
    *(s32 *)lbl_1_bss_1C8 = (s32) (uintptr_t) obj;
    obj = omAddObjEx(lbl_1_bss_19C, 0x67, 0x0, 0x0, 0x0, fn_1_9860);
    *(s32 *)lbl_1_bss_1CC = (s32) (uintptr_t) obj;
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
    mgr = (OMOBJMAN *) (uintptr_t) MgActorObjectSetup();
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

/*
 * Screen-blit pass: grabs the EFB, wraps it as a texture, then draws one
 * indirect-textured quad over the screen with the ring texture warped through
 * an indirect matrix built from lbl_1_rodata_49C scaled by the frame axes.
 */
void fn_1_7014(HU3DMODEL *modelP, f32 (*mtx)[3][4])
{
    f32 sp8[8];
    f32 ax;
    f32 ay;
    f32 az;
    f32 aw;
    f32 off;
    s32 i;

    ax = *(f32 *) (lbl_1_data_EC + 0) * 0.03125f;
    ay = *(f32 *) (lbl_1_data_EC + 4) * 0.03125f;
    az = *(f32 *) (lbl_1_data_EC + 8) * 0.03125f;
    aw = *(f32 *) (lbl_1_data_EC + 0xC) * 0.03125f;
    off = (f32) (lbl_1_bss_4 % *(s32 *) (lbl_1_data_EC + 0x10)) / *(f32 *) (lbl_1_data_EC + 0x10);
    C_MTXOrtho(lbl_1_bss_B8, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f);
    GXSetProjection(lbl_1_bss_B8, GX_ORTHOGRAPHIC);
    PSMTXIdentity(lbl_1_bss_E8);
    GXLoadPosMtxImm(lbl_1_bss_E8, 0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
    GXSetZMode(0, GX_ALWAYS, 0);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_RGBA6, 0);
    GXSetCopyFilter(0, (void *) (RenderMode + 0x1A), 0, (void *) (RenderMode + 0x32));
    GXSetTexCopySrc(0, 0, 0x280, 0x1E0);
    GXSetTexCopyDst(0x280, 0x1E0, GX_TF_RGBA8, 0);
    GXCopyTex(lbl_1_bss_168, 0);
    DCFlushRange(lbl_1_bss_168, (u32) lbl_1_bss_16C);
    GXPixModeSync();
    GXSetCopyFilter(RenderMode[0x19], (void *) (RenderMode + 0x1A), 1, (void *) (RenderMode + 0x32));
    GXInitTexObj(lbl_1_bss_118, lbl_1_bss_168, 0x280, 0x1E0, 6, GX_CLAMP, GX_CLAMP, 0);
    GXLoadTexObj(lbl_1_bss_118, GX_TEXMAP0);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_RGBA6, 0);
    GXInitTexObj(&lbl_1_bss_118[1], lbl_1_bss_160, 0x20, 0x20, 3, GX_REPEAT, GX_REPEAT, 0);
    GXLoadTexObj(&lbl_1_bss_118[1], GX_TEXMAP1);
    for (i = 0; i < 6; i++) {
        sp8[i] = *(f32 *) (lbl_1_rodata_49C + (i * 4));
    }
    sp8[2] *= ay;
    sp8[5] *= ax;
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x3C, 0, 0x7D);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetIndTexMtx(GX_ITM_0, (f32 (*)[3]) &sp8[0], 0);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, 1, 0, GX_ITM_0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
    *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
    *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
    *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
    *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
    *(f32 *) (uintptr_t) 0xCC008000 = 0.0f / az;
    *(f32 *) (uintptr_t) 0xCC008000 = (0.0f / aw) + off;
    *(f32 *) (uintptr_t) 0xCC008000 = 1.0f;
    *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
    *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
    *(f32 *) (uintptr_t) 0xCC008000 = 1.0f;
    *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
    *(f32 *) (uintptr_t) 0xCC008000 = 1.0f / az;
    *(f32 *) (uintptr_t) 0xCC008000 = (0.0f / aw) + off;
    *(f32 *) (uintptr_t) 0xCC008000 = 1.0f;
    *(f32 *) (uintptr_t) 0xCC008000 = 1.0f;
    *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
    *(f32 *) (uintptr_t) 0xCC008000 = 1.0f;
    *(f32 *) (uintptr_t) 0xCC008000 = 1.0f;
    *(f32 *) (uintptr_t) 0xCC008000 = 1.0f / az;
    *(f32 *) (uintptr_t) 0xCC008000 = (12.0f / aw) + off;
    *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
    *(f32 *) (uintptr_t) 0xCC008000 = 1.0f;
    *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
    *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
    *(f32 *) (uintptr_t) 0xCC008000 = 1.0f;
    *(f32 *) (uintptr_t) 0xCC008000 = 0.0f / az;
    *(f32 *) (uintptr_t) 0xCC008000 = (12.0f / aw) + off;
    GXSetNumIndStages(0);
    GXSetTevDirect(GX_TEVSTAGE0);
}
/*
 * Draws the stage-select markers: transforms the four corner anchors through
 * the view matrix, then emits one textured quad per active slot. The slot's
 * unk18 drives both the texture cell (unk18/3, clamped to 8) and the vertex
 * alpha (a ramp up, a hold, then a ramp down).
 */
void fn_1_6640(f32 (*arg1)[4])
{
    Point3d pts[4];
    f32 m[8];
    s32 i;
    s32 cell;
    s32 alpha;
    f32 u0;
    f32 v0;
    f32 u1;
    f32 v1;

    for (i = 0; i < 0xB; i++) {
        ((f32 *) m)[i] = *(f32 *) (lbl_1_rodata_460 + (i * 4));
    }
    m[2] *= lbl_1_data_EC[1] * 0.03125f;
    m[5] *= lbl_1_data_EC[0] * 0.03125f;
    C_MTXLookAt(lbl_1_bss_A8, &Hu3DCamera->pos, &Hu3DCamera->up, &Hu3DCamera->target);
    PSMTXTranspose(lbl_1_bss_A8, lbl_1_bss_98);
    PSMTXMultVecArray(lbl_1_bss_98, (Point3d *) &m[0], pts, 4);
    GXLoadPosMtxImm(arg1, 0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
    GXSetZMode(1, GX_LEQUAL, 0);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZCompLoc(0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_TEX_ST, GX_RGBA8, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_RGBA6, 0);
    /* esprite[i] = {s16 memberNo; s16 animNo;} (4 bytes) and
     * espanim[j] = {u32 dataNum; u16 useCnt; ANIMDATA *anim;} (0xC bytes), per
     * src/game/esprite.c. So the texture is espanim[esprite[i].animNo].anim. */
    HuSprTexLoad(*(u32 *) (espanim + (*(u32 *) (esprite + (*(s32 *) (uintptr_t) lbl_1_bss_17C * 4) + 2) * 0xC) + 8), 0, 0, 0, 0, 1);
    for (i = 0; i < 0x1E; i++) {
        s32 v = ((s32 *) ((u8 *) lbl_1_bss_280 + (i * 0x14)))[6];

        if (v == 0) {
            continue;
        }
        cell = v / 3;
        if (cell > 8) {
            cell = 8;
        }
        if (v < 4) {
            alpha = (0xF0 * v) / 4;
        } else if (v < 0x1B) {
            alpha = 0xF0;
        } else if (v < 0x20) {
            alpha = 0xF0 - ((0xF0 * (v - 0x1B)) / 5);
        } else {
            alpha = 0;
        }
        u0 = ((f32) (cell % 5) / 5.0f) + 0.003125f;
        v0 = ((f32) (cell / 5) * 0.5f) + 0.0078125f;
        u1 = (0.2f + ((f32) (cell % 5) / 5.0f)) - 0.003125f;
        v1 = (0.5f + ((f32) (cell / 5) * 0.5f)) - 0.0078125f;
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        *(f32 *) (uintptr_t) 0xCC008000 = pts[0].x + ((f32 *) ((u8 *) lbl_1_bss_280 + (i * 0x14)))[0];
        *(f32 *) (uintptr_t) 0xCC008000 = pts[0].y + ((f32 *) ((u8 *) lbl_1_bss_280 + (i * 0x14)))[1];
        *(f32 *) (uintptr_t) 0xCC008000 = pts[0].z + ((f32 *) ((u8 *) lbl_1_bss_280 + (i * 0x14)))[2];
        *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
        *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
        *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
        *(u8 *) (uintptr_t) 0xCC008000 = (u8) alpha;
        *(f32 *) (uintptr_t) 0xCC008000 = u0;
        *(f32 *) (uintptr_t) 0xCC008000 = v0;
        *(f32 *) (uintptr_t) 0xCC008000 = pts[1].x + ((f32 *) ((u8 *) lbl_1_bss_280 + (i * 0x14)))[0];
        *(f32 *) (uintptr_t) 0xCC008000 = pts[1].y + ((f32 *) ((u8 *) lbl_1_bss_280 + (i * 0x14)))[1];
        *(f32 *) (uintptr_t) 0xCC008000 = pts[1].z + ((f32 *) ((u8 *) lbl_1_bss_280 + (i * 0x14)))[2];
        *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
        *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
        *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
        *(u8 *) (uintptr_t) 0xCC008000 = (u8) alpha;
        *(f32 *) (uintptr_t) 0xCC008000 = u1;
        *(f32 *) (uintptr_t) 0xCC008000 = v0;
        *(f32 *) (uintptr_t) 0xCC008000 = pts[2].x + ((f32 *) ((u8 *) lbl_1_bss_280 + (i * 0x14)))[0];
        *(f32 *) (uintptr_t) 0xCC008000 = pts[2].y + ((f32 *) ((u8 *) lbl_1_bss_280 + (i * 0x14)))[1];
        *(f32 *) (uintptr_t) 0xCC008000 = pts[2].z + ((f32 *) ((u8 *) lbl_1_bss_280 + (i * 0x14)))[2];
        *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
        *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
        *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
        *(u8 *) (uintptr_t) 0xCC008000 = (u8) alpha;
        *(f32 *) (uintptr_t) 0xCC008000 = u1;
        *(f32 *) (uintptr_t) 0xCC008000 = v1;
        *(f32 *) (uintptr_t) 0xCC008000 = pts[3].x + ((f32 *) ((u8 *) lbl_1_bss_280 + (i * 0x14)))[0];
        *(f32 *) (uintptr_t) 0xCC008000 = pts[3].y + ((f32 *) ((u8 *) lbl_1_bss_280 + (i * 0x14)))[1];
        *(f32 *) (uintptr_t) 0xCC008000 = pts[3].z + ((f32 *) ((u8 *) lbl_1_bss_280 + (i * 0x14)))[2];
        *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
        *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
        *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
        *(u8 *) (uintptr_t) 0xCC008000 = (u8) alpha;
        *(f32 *) (uintptr_t) 0xCC008000 = u0;
        *(f32 *) (uintptr_t) 0xCC008000 = v1;
    }
    GXPixModeSync();
}
/*
 * Create: builds the whole stage model set (board, four player markers, the
 * five hub props, the two hook layers and the sprite), seeds the motion times
 * from rand8, then hands over to fn_1_43BC. arg0->mdlId is an s16 array, so
 * m2c's ->unkN is just index N.
 */
void fn_1_7ED8(OMOBJ *arg0)
{
    s32 sp10[4];
    s32 sp8;
    u8 sp20[0x18];
    s32 i;
    f32 maxTime;

    for (i = 0; i < 4; i++) {
        sp10[i] = ((s32 *) (uintptr_t) ((u8 *) lbl_1_bss_1B8 + (i * 4)))[0];
        sp10[i] = ((s32 *) (uintptr_t) ((s32 *) (uintptr_t) ((u8 *) lbl_1_bss_1B8 + (i * 4)))[0x5C / 4])[0x5C / 4];
        sp10[i] = ((s32 *) (uintptr_t) ((u8 *) lbl_1_bss_1B8 + (i * 4)))[0x5C / 4];
    }
    arg0->mdlId[0] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E0000, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[0], 1);
    Hu3DModelShadowMapSet(arg0->mdlId[0]);
    sp8 = arg0->mdlId[0];
    arg0->mdlId[0xA] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E0005, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(arg0->mdlId[0xA], 1);
    arg0->mdlId[0x10] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E000C, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelPosSet(arg0->mdlId[0x10], 0.0f, 450.0f, 0.0f);
    Hu3DModelAttrSet(arg0->mdlId[0x10], 1);
    MgActorColMapInit(&sp8, 3, 0x80);
    arg0->mdlId[2] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E0001, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[2], 1);
    Hu3DModelAttrSet(arg0->mdlId[2], 0x40000001);
    Hu3DMotionSpeedSet(arg0->mdlId[2], 0.25f);
    arg0->mdlId[4] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E0002, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[4], 1);
    Hu3DModelAttrSet(arg0->mdlId[4], 0x40000001);
    arg0->mdlId[6] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E0003, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[6], 1);
    Hu3DModelAttrSet(arg0->mdlId[6], 0x40000001);
    arg0->mdlId[8] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E0004, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[8], 1);
    Hu3DModelAttrSet(arg0->mdlId[8], 0x40000001);
    arg0->mdlId[0x1C] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E0006, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[0x1C], 2);
    Hu3DModelAttrSet(arg0->mdlId[0x1C], 0x40000001);
    arg0->mdlId[0x1E] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E0007, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[0x1E], 2);
    Hu3DModelAttrSet(arg0->mdlId[0x1E], 0x40000001);
    arg0->mdlId[0x20] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E0008, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[0x20], 2);
    Hu3DModelAttrSet(arg0->mdlId[0x20], 0x40000001);
    arg0->mdlId[0x22] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E0009, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[0x22], 2);
    Hu3DModelAttrSet(arg0->mdlId[0x22], 0x40000001);
    arg0->mdlId[0xE] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E000A, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[0xE], 1);
    Hu3DModelAttrSet(arg0->mdlId[0xE], 0x40000002);
    Hu3DModelPosSet(arg0->mdlId[0xE], 0.0f, 450.0f, 0.0f);
    Hu3DModelShadowMapSet(arg0->mdlId[0xE]);
    arg0->mdlId[0xC] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E000B, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[0xC], 1);
    Hu3DModelAttrSet(arg0->mdlId[0xC], 0x40000001);
    Hu3DModelPosSet(arg0->mdlId[0xC], 0.0f, 450.0f, 0.0f);
    for (i = 0; i < 5; i++) {
        u32 dataNum = *(u32 *) (lbl_1_rodata_4D0 + (i * 4));
        s16 id = Hu3DModelCreate(HuDataSelHeapReadNum(dataNum, HU_MEMNUM_OVL, HEAP_MODEL));

        arg0->mdlId[i + 9] = id;
        Hu3DModelLayerSet(arg0->mdlId[i + 9], 1);
        if (i < 2) {
            Hu3DModelAttrSet(arg0->mdlId[i + 9], 1);
            Hu3DMotionTimeSet(arg0->mdlId[i + 9], 540.0f);
        }
        if (i == 2) {
            Hu3DModelAttrSet(arg0->mdlId[i + 9], 0x40000001);
        }
    }
    arg0->mdlId[0x26] = Hu3DHookFuncCreate((void (*)(HU3DMODEL *, f32 (*)[3][4])) fn_1_6640);
    Hu3DModelLayerSet(arg0->mdlId[0x26], 4);
    arg0->mdlId[0x28] = Hu3DHookFuncCreate(fn_1_7014);
    Hu3DModelLayerSet(arg0->mdlId[0x28], 5);
    arg0->mdlId[0x2A] = Hu3DHookFuncCreate(fn_1_7830);
    Hu3DModelLayerSet(arg0->mdlId[0x2A], 0);
    lbl_1_bss_17C = espEntry(0x4E0012, 0, 0);
    espDispOff((s16) lbl_1_bss_17C);
    memset(lbl_1_bss_5C8, 0, 0x200);
    lbl_1_bss_5C8[0] = -50.0f;
    lbl_1_bss_5C8[4] = 0.0f;
    lbl_1_bss_5C8[2] = 0.0f;
    Hu3DModelRotSet(arg0->mdlId[0xC], 0.0f, lbl_1_bss_5C8[2], 0.0f);
    Hu3DModelRotSet(arg0->mdlId[0x10], 0.0f, lbl_1_bss_5C8[2], 0.0f);
    memset(lbl_1_bss_280, 0, 0x348);
    for (i = 0; i < 2; i++) {
        ((s32 *) lbl_1_bss_270)[(i * 8) / 4] = 1;
        maxTime = Hu3DMotionMaxTimeGet(arg0->mdlId[i + 0xE], 0);
        ((s32 *) lbl_1_bss_270)[(i * 8) / 4 + 1] = (s32) (((f32) rand8() / 255.0f) * maxTime);
        Hu3DMotionTimeSet(arg0->mdlId[i + 0xE], (f32) ((s32 *) lbl_1_bss_270)[(i * 8) / 4 + 1]);
        Hu3DMotionTimeSet(arg0->mdlId[i + 0x10], (f32) ((s32 *) lbl_1_bss_270)[(i * 8) / 4 + 1]);
        Hu3DModelAttrSet(arg0->mdlId[i + 0xE], 1);
        Hu3DModelAttrSet(arg0->mdlId[i + 0x10], 1);
    }
    MgActorColAttrParamGet(sp20, 1);
    /* the two collisions parameters that follow are zeroed then written back */
    MgActorColAttrParamSet(sp20, 1);
    MgActorColAttrParamGet(sp20, 2);
    MgActorColAttrParamSet(sp20, 2);
    memset((f32 *) lbl_1_bss_1D0, 0, 0xA0);
    for (i = 0; i < 8; i++) {
        ((s32 *) ((u8 *) lbl_1_bss_1D0 + (i * 0x14)))[0x10 / 4] = -1;
    }
    arg0->objFunc = fn_1_43BC;
}
/*
 * Update: drives the camera interpolation across the sequence modes, the
 * two-phase animation cycle, and the 30-slot particle field. lbl_1_bss_5C8 is
 * an f32 state block: [0] is the lift height, [2] the azimuth, [4]/[5]/[6] the
 * frame counters the animation and the particle bursts key off.
 */
void fn_1_43BC(OMOBJ *arg0)
{
    s32 sp34[4];
    u8 sp44[0x18];
    u8 sp80[0x28];
    u8 sp58[0x28];
    f32 lift;
    f32 azim;
    f32 t;
    f32 k;
    f32 f;
    s32 i;
    s32 j;
    s32 slot;

    for (i = 0; i < 4; i++) {
        sp34[i] = ((s32 *) (uintptr_t) ((u8 *) lbl_1_bss_1B8 + (i * 4)))[0x5C / 4];
        sp34[i] = ((s32 *) (uintptr_t) ((s32 *) (uintptr_t) ((u8 *) lbl_1_bss_1B8 + (i * 4)))[0x5C / 4])[0x5C / 4];
        sp34[i] = *(s32 *) ((u8 *) lbl_1_bss_1B8 + (i * 4));
    }
    lbl_1_bss_1A0 = MgSeqModeGet();
    lbl_1_bss_0 = MgSeqFrameNoGet();
    lbl_1_bss_4 += 1;
    if (lbl_1_bss_164 == 0) {
        Hu3DModelAttrSet(arg0->mdlId[0x2A], 1);
    }
    if ((lbl_1_bss_1A0 == 2 && lbl_1_bss_0 >= 0x82) || lbl_1_bss_1A0 == 3) {
        lbl_1_bss_14 += 1;
        t = lbl_1_bss_14;
        if (t < 16.0f) {
            k = (((1.0f + t) * 0.5f * t) * 0.0625f) / 48.0f;
        } else if (t < 48.0f) {
            k = (8.5f / 48.0f) + ((t - 16.0f) / 48.0f);
        } else if (t < 63.0f) {
            k = 1.0f - (((((1.0f + (63.0f - t)) * 0.5f * (63.0f - t)) / 15.0f) / 48.0f));
        } else {
            k = 1.0f;
        }
        CZoom = (1425.0f * (1.0f - k)) + (4575.0f * k);
        Center.y = (505.0f * (1.0f - k)) + (-120.0f * k);
        CRot.x = (-35.0f * (1.0f - k)) + (-70.0f * k);
    }
    if (lbl_1_bss_1A0 == 3 && lbl_1_bss_0 > 0x2F) {
        f32 n = (f32) (0x3E - lbl_1_bss_0);

        if (n > 0.0f) {
            f32 sign = ((s32) n & 1) ? 1.0f : -1.0f;
            f32 m = n * (sign / 15.0f);

            Center.y = (f32) (-120.0 + ((6.0f * m) * cos(1.2217304763960306)));
            Center.z = (f32) ((6.0f * m) * -sin(1.2217304763960306));
        }
    }
    if (lbl_1_bss_1A0 == 8 && lbl_1_data_9C == 0) {
        f32 e = 1.0f - 0.9f;

        for (i = 0; i < 10; i++) {
            ((f32 *) sp80)[i] = *(f32 *) (lbl_1_rodata_344 + (i * 4));
            ((f32 *) sp58)[i] = *(f32 *) (lbl_1_rodata_36C + (i * 4));
        }
        CZoom = (CZoom * 0.9f) + (900.0f * e);
        Center.y = (Center.y * 0.9f) + (e * (((f32 *) sp80)[((s32 *) (uintptr_t) sp34[0])[2]] - 140.0f));
        CRot.x = (CRot.x * 0.9f) + (e * ((f32 *) sp58)[((s32 *) (uintptr_t) sp34[0])[2]]);
    }
    if (lbl_1_bss_1A0 == 8 && lbl_1_data_9C == 1) {
        f32 e = 1.0f - 0.9f;

        CZoom = (CZoom * 0.9f) + (3825.0f * e);
        Center.y = (Center.y * 0.9f) + (-95.0f * e);
        CRot.x = (CRot.x * 0.9f) + (-51.0f * e);
    }
    MgActorColAttrParamGet(sp44, 2);
    MgActorColAttrParamSet(sp44, lbl_1_bss_1A0 == 5 ? 0.6666667f : 0.0f);
    lbl_1_bss_5C8[4] += 1;
    if (lbl_1_bss_5C8[6] >= 480.0f && lbl_1_bss_1A0 == 5 && ((s32 *) ((u8 *) lbl_1_bss_1D0 + 0x14))[0] < 3) {
        ((f32 *) ((u8 *) lbl_1_bss_1D0 + (((s32 *) ((u8 *) lbl_1_bss_1D0 + 0x14))[0] * 0x14)))[0x54 / 4] = 0.0f;
        ((s32 *) ((u8 *) lbl_1_bss_1D0 + 0x14))[0] += 1;
    }
    if (lbl_1_bss_5C8[6] == 2.0f) {
        HuAudFXStop(((s32 *) ((u8 *) lbl_1_bss_1D0 + 0x18))[0]);
    }
    lbl_1_bss_5C8[6] = lbl_1_bss_5C8[6] >= 480.0f ? 0.0f : lbl_1_bss_5C8[6] + 1.0f;
    lbl_1_bss_5C8[5] = lbl_1_bss_5C8[5] >= 120.0f ? 0.0f : lbl_1_bss_5C8[5] + 1.0f;
    if (lbl_1_bss_1A0 > 5) {
        lbl_1_bss_5C8[0] -= 1.5f;
        if (lbl_1_bss_5C8[0] < -190.0f) {
            lbl_1_bss_5C8[0] = -190.0f;
        }
    } else {
        f32 ramp = lbl_1_bss_5C8[5] / 120.0f;

        lbl_1_bss_5C8[0] = ramp;
        azim = (lbl_1_bss_5C8[5] == lbl_1_bss_5C8[6]) ? -75.0f : -60.0f;
        lbl_1_bss_5C8[0] = (f32) (-50.0 + ((f64) azim * sin((M_PI * (180.0 * ((0.5 * lbl_1_bss_5C8[0]) + (0.5 * (1.0f - ((1.0f - lbl_1_bss_5C8[0]) * (1.0f - lbl_1_bss_5C8[0]))))))) / 180.0)));
        f = (lbl_1_bss_5C8[5] == lbl_1_bss_5C8[6]) ? 1.0f : 0.6666667f;
        if (lbl_1_bss_5C8[6] <= 80.0f) {
            t = 0.0f;
        } else if (lbl_1_bss_5C8[6] <= 120.0f) {
            t = (lbl_1_bss_5C8[6] - 80.0f) / 40.0f;
        } else if (lbl_1_bss_5C8[6] <= 240.0f) {
            t = 1.0f;
        } else if (lbl_1_bss_5C8[6] <= 480.0f) {
            t = 1.0f - ((lbl_1_bss_5C8[6] - 240.0f) / 240.0f);
        } else {
            t = 0.0f;
        }
        lift = 135.0f - ((lbl_1_bss_5C8[0] - -50.0f) * f);
        lift = (lift + (-90.0f * t)) + 45.0f;
        if (lift > 180.0f) {
            lift = 180.0f;
        }
        /* the three overlay models ride the lift */
        for (i = 0; i < 3; i++) {
            Hu3DModelAttrSet(((s32 *) ((u8 *) lbl_1_bss_1B4 + 0x14))[i + 0xB], 1);
            Hu3DModelPosSet(((s32 *) ((u8 *) lbl_1_bss_1B4 + 0x14))[i + 0xB], 0.0f, lift, 0.0f);
        }
    }
    {
        s32 mdl = *(s32 *) ((u8 *) lbl_1_bss_1B4 + 0x14);

        Hu3DModelPosSet(mdl + 0xC, 0.0f, 500.0f + lbl_1_bss_5C8[0], 0.0f);
        Hu3DModelPosSet(mdl + 0x10, 0.0f, 500.0f + lbl_1_bss_5C8[0], 0.0f);
        Hu3DModelPosSet(mdl + 0xE, 0.0f, 500.0f + lbl_1_bss_5C8[0], 0.0f);
    }
    if (lbl_1_bss_5C8[6] == 0.0f) {
        Hu3DModelAttrReset(*(s32 *) ((u8 *) lbl_1_bss_1B4 + 0x14) + 0x16, 1);
    } else if (lbl_1_bss_5C8[6] <= 180.0f) {
        Hu3DMotionTimeSet(*(s32 *) ((u8 *) lbl_1_bss_1B4 + 0x14) + 0x18, lbl_1_bss_5C8[6]);
        Hu3DModelAttrReset(*(s32 *) ((u8 *) lbl_1_bss_1B4 + 0x14) + 0x18, 1);
    } else if (lbl_1_bss_5C8[6] <= 480.0f) {
        Hu3DMotionTimeSet(*(s32 *) ((u8 *) lbl_1_bss_1B4 + 0x14) + 0x16, 0.0f);
        Hu3DMotionTimeSet(*(s32 *) ((u8 *) lbl_1_bss_1B4 + 0x14) + 0x1A, lbl_1_bss_5C8[6] - 180.0f);
        Hu3DModelAttrReset(*(s32 *) ((u8 *) lbl_1_bss_1B4 + 0x14) + 0x1A, 1);
    }
    if (lbl_1_bss_5C8[6] == 30.0f) {
        if (((s32 *) ((u8 *) lbl_1_bss_1D0 + 0x14))[0] < 3) {
            ((f32 *) ((u8 *) lbl_1_bss_1D0 + (((s32 *) ((u8 *) lbl_1_bss_1D0 + 0x14))[0] * 0x14)))[0x4 / 4] = 0.0f;
            ((s32 *) ((u8 *) lbl_1_bss_1D0 + 0x14))[0] += 1;
        }
    }
    if (lbl_1_bss_5C8[6] == 5.0f) {
        Hu3DModelAttrReset(*(s32 *) ((u8 *) lbl_1_bss_1B4 + 0x14) + 0x14, 1);
        Hu3DMotionTimeSet(*(s32 *) ((u8 *) lbl_1_bss_1B4 + 0x14) + 0x14, 0.0f);
    }
    if (lbl_1_bss_5C8[6] == 0x55) {
        Hu3DModelAttrSet(*(s32 *) ((u8 *) lbl_1_bss_1B4 + 0x14) + 0x14, 1);
    }
    if (lbl_1_bss_5C8[6] == 30.0f) {
        Hu3DModelAttrReset(*(s32 *) ((u8 *) lbl_1_bss_1B4 + 0x14) + 0x12, 1);
        Hu3DMotionTimeSet(*(s32 *) ((u8 *) lbl_1_bss_1B4 + 0x14) + 0x12, 0.0f);
    }
    if (lbl_1_bss_5C8[6] == 210.0f) {
        Hu3DModelAttrSet(*(s32 *) ((u8 *) lbl_1_bss_1B4 + 0x14) + 0x12, 1);
    }
    /* particle bursts: one every 6 frames of the 60-frame cycle, three at a
     * time, at the azimuth the ring is currently showing */
    if ((lbl_1_bss_1A0 == 5 || (lbl_1_bss_1A0 == 3 && lbl_1_bss_0 > 0x2D))
        && ((((s32) lbl_1_bss_5C8[4]) % 60) == 1 || (((s32) lbl_1_bss_5C8[4]) % 60) == 7
            || (((s32) lbl_1_bss_5C8[4]) % 60) == 11 || (((s32) lbl_1_bss_5C8[4]) % 60) == 17
            || (((s32) lbl_1_bss_5C8[4]) % 60) == 21 || (((s32) lbl_1_bss_5C8[4]) % 60) == 27)) {
        for (i = 0; i < 3; i++) {
            f32 ang = lbl_1_bss_5C8[2] + ((f32) ((i * 0x168) / 3));
            for (slot = 0; slot < 0x1E; slot++) {
                if (((s32 *) ((u8 *) lbl_1_bss_280 + (slot * 0x14)))[6] != 0) {
                    continue;
                }
                break;
            }
            if (slot == 0x1E) {
                continue;
            }
            ((s32 *) ((u8 *) lbl_1_bss_280 + (slot * 0x14)))[6] = 1;
            ((f32 *) ((u8 *) lbl_1_bss_280 + (slot * 0x14)))[0] = (f32) (175.0 * sin((M_PI * ang) / 180.0));
            ((f32 *) ((u8 *) lbl_1_bss_280 + (slot * 0x14)))[1] = 400.0f + lbl_1_bss_5C8[0];
            ((f32 *) ((u8 *) lbl_1_bss_280 + (slot * 0x14)))[2] = (f32) (200.0 * cos((M_PI * ang) / 180.0));
            ((f32 *) ((u8 *) lbl_1_bss_280 + (slot * 0x14)))[3] = (f32) ((575.0 * sin((M_PI * ang) / 180.0)) * 0.03125);
            ((f32 *) ((u8 *) lbl_1_bss_280 + (slot * 0x14)))[4] = -4.285714f;
            ((f32 *) ((u8 *) lbl_1_bss_280 + (slot * 0x14)))[5] = (f32) ((575.0 * cos((M_PI * ang) / 180.0)) * 0.03125);
        }
    }
    for (slot = 0; slot < 0x1E; slot++) {
        f32 *rec = (f32 *) ((u8 *) lbl_1_bss_280 + (slot * 0x14));

        if (((s32 *) rec)[6] == 0) {
            continue;
        }
        ((s32 *) rec)[6] += 1;
        rec[0] += rec[3];
        rec[1] += rec[4];
        rec[2] += rec[5];
        if (((s32 *) rec)[6] == 0x21) {
            ((s32 *) rec)[6] = 0;
        }
    }
    for (i = 0; i < 2; i++) {
        s32 *rec = (s32 *) ((u8 *) lbl_1_bss_270 + (i * 8));

        if (rec[0] == 0) {
            rec[1] -= 1;
            if (rec[1] <= 0) {
                rec[0] = 1;
                rec[1] = 0;
                Hu3DMotionTimeSet(arg0->mdlId[i + 0xE], 0.0f);
                Hu3DModelAttrReset(arg0->mdlId[i + 0xE], 1);
                Hu3DMotionTimeSet(arg0->mdlId[i + 0x10], 0.0f);
                Hu3DModelAttrReset(arg0->mdlId[i + 0x10], 1);
                j = (i == 0) ? 3 : 2;
                if (((s32 *) ((u8 *) lbl_1_bss_1D0 + (j * 0x14)))[0] < 3) {
                    ((s32 *) ((u8 *) lbl_1_bss_1D0 + (j * 0x14)))[0] += 1;
                    ((f32 *) ((u8 *) lbl_1_bss_1D0 + (j * 0x14) + (((s32 *) ((u8 *) lbl_1_bss_1D0 + (j * 0x14)))[0] * 4)))[1] = 0.0f;
                }
            }
        } else {
            rec[1] += 1;
            if (rec[1] == (s32) Hu3DMotionMaxTimeGet(arg0->mdlId[i + 0xE], 0)) {
                rec[0] = 0;
                rec[1] = (s32) (60.0f * ((f32) rand8() / 255.0f));
                Hu3DModelAttrSet(arg0->mdlId[i + 0xE], 1);
                Hu3DModelAttrSet(arg0->mdlId[i + 0x10], 1);
            }
        }
    }
}
/* per-player creation state, 0x54 bytes, zeroed at entry */
typedef struct M520Player_s {
    s32 idx;      /* 0x00 slot index */
    s32 conf;     /* 0x04 GwPlayerConf index */
    s32 charNo;   /* 0x08 clamped char number */
    s32 charFull; /* 0x0C raw char number */
    s32 padNo;    /* 0x10 */
    s32 dif;      /* 0x14 */
    s32 unk18;
    s32 unk1C;    /* 0x1C life state */
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;    /* 0x34 fall scale */
    s32 unk38;    /* 0x38 ai state */
    s32 unk3C;    /* 0x3C ai timer */
    s32 unk40;    /* 0x40 ai delay */
    s32 unk44;    /* 0x44 streak count */
    f32 unk48;    /* 0x48 chosen direction */
    f32 unk4C;    /* 0x4C target angle */
    f32 unk50;    /* 0x50 ring radius */
} M520PLAYER;

/*
 * Player Create: resolves the slot to a GwPlayerConf entry (grouping players of
 * the same colour together), creates the Mg actor, loads the character model
 * and its three joint motions, then wires the per-slot callbacks. The ARAM
 * lookups fall back to a heap read when the MRAM path is not resident.
 */
void fn_1_8A98(OMOBJ *arg0)
{
    f32 sp5C[4];
    HuVecF sp88;
    u8 sp24[0x48];
    M520PLAYER *pl;
    s32 n;
    s32 i;
    s32 j;
    s32 k;
    u32 motion;

    for (i = 0; i < 4; i++) {
        sp5C[i] = *(f32 *) (lbl_1_rodata_4F0 + (i * 4));
    }
    pl = (M520PLAYER *) arg0->data;
    memset(pl, 0, 0x54);
    n = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (i == GwPlayerConf[j].grp) {
                sp5C[n] = j;
                n += 1;
            }
        }
    }
    pl->idx = arg0->work[0];
    pl->conf = (s32) sp5C[pl->idx];
    if (pl->conf == 4) {
        pl->conf = pl->idx;
    }
    pl->charFull = GwPlayerConf[pl->conf].charNo;
    pl->charNo = pl->charFull >= 9 ? 9 : pl->charFull;
    pl->padNo = GwPlayerConf[pl->conf].type != 0 ? -1 : GwPlayerConf[pl->conf].padNo;
    pl->dif = pl->padNo != -1 ? 0 : GwPlayerConf[pl->conf].dif;
    pl->unk1C = 2;
    pl->unk34 = 1.0f;
    sp88.x = 150.0f;
    sp88.y = 40.0f;
    sp88.z = (f32) pl->idx;
    MgPlayerCreate((s16) pl->conf, &sp88, 4, 1, 0x13, pl->idx == -1 ? lbl_1_data_138 : NULL);
    arg0->mdlId[0] = (s16) (*(u32 *) ((u8 *) (uintptr_t) lbl_1_bss_1A4[pl->idx] + 0x3C) + 0x40);
    CharEffectLayerSet(3);
    {
        u8 *act = (u8 *) (uintptr_t) (*(u32 *) ((u8 *) (uintptr_t) lbl_1_bss_1A4[pl->idx] + 0x3C));

        if (pl->idx == 0) {
            *(f32 *) (act + 0x2C) = 0.0f;
            *(f32 *) (act + 0x30) = 600.0f;
            *(f32 *) (act + 0x34) = 0.0f;
        } else {
            *(f32 *) (act + 0x2C) = (f32) (0x21C * sin((M_PI * ((pl->idx * 0x78) + 0x3C)) / 180.0));
            *(f32 *) (act + 0x30) = 400.0f;
            *(f32 *) (act + 0x34) = (f32) ((0x21C * cos((M_PI * ((pl->idx * 0x78) + 0x3C)) / 180.0)) - 25.0);
        }
    }
    MgPlayerVibrateCreate(lbl_1_bss_1A4[pl->idx], pl->idx * 4);
    {
        u8 *act = (u8 *) (uintptr_t) (*(u32 *) ((u8 *) (uintptr_t) lbl_1_bss_1A4[pl->idx] + 0x3C));

        sp5C[0] = *(f32 *) (act + 0x2C);
        sp5C[1] = *(f32 *) (act + 0x30);
        sp5C[2] = *(f32 *) (act + 0x34);
    }
    if (MgActorColMapPolyGet((u8 *) (uintptr_t) lbl_1_bss_1A4[pl->idx] + 0x2C, sp24, 0xFFFF, (u8 *) sp24 + 0x30) != 0) {
        f32 h = *(f32 *) ((u8 *) sp24 + 0x34);

        *(f32 *) ((u8 *) sp24 + 0x34) = h + 75.0f;
        MgActorPosUpdate(lbl_1_bss_1A4[pl->idx], (HuVecF *) ((u8 *) sp24 + 0x30), *(f32 *) ((u8 *) sp24 + 0x34));
        *(f32 *) ((u8 *) (uintptr_t) (*(u32 *) ((u8 *) (uintptr_t) lbl_1_bss_1A4[pl->idx] + 0x3C)) + 0x30) = *(f32 *) ((u8 *) sp24 + 0x34);
    }
    Hu3DModelLayerSet(arg0->mdlId[0], 1);
    Hu3DModelShadowSet(arg0->mdlId[0]);
    Hu3DModelAmbSet(arg0->mdlId[0], 1.0f, 0.5f, 0.5f);
    if (pl->idx == 0) {
        arg0->mdlId[2] = Hu3DModelCreate(HuDataSelHeapReadNum(charDataDirTbl[pl->charNo][0], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelLayerSet(arg0->mdlId[2], 1);
        Hu3DModelShadowSet(arg0->mdlId[2]);
        Hu3DModelAmbSet(arg0->mdlId[2], 1.0f, 0.5f, 0.5f);
    }
    (*(s32 *) ((u8 *) (uintptr_t) lbl_1_bss_1A4[pl->idx] + 0x68)) = (s32) (uintptr_t) fn_1_A08;
    if (pl->idx == 0) {
        /* the leader gets three explicit joint motions; the rest get one */
        arg0->mtnId[0] = Hu3DJointMotion(arg0->mdlId[2], HuDataSelHeapReadNum(charDataDirTbl[pl->charNo][4] | 0x2F, HU_MEMNUM_OVL, HEAP_MODEL));
        arg0->mtnId[2] = Hu3DJointMotion(arg0->mdlId[2], HuDataSelHeapReadNum(charDataDirTbl[pl->charNo][4] | 0x75, HU_MEMNUM_OVL, HEAP_MODEL));
        arg0->mtnId[4] = Hu3DJointMotion(arg0->mdlId[2], HuDataSelHeapReadNum(charDataDirTbl[pl->charNo][4] | 0x75, HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DMotionSet(arg0->mdlId[2], arg0->mtnId[0]);
    } else {
        motion = CharMotionCreate((s16) pl->charNo, pl->idx == 0 ? 0x8C002F : 0x8C0006);
        *(u32 *) ((u8 *) (uintptr_t) (*(u32 *) ((u8 *) (uintptr_t) lbl_1_bss_1A4[pl->idx] + 8)) + 0x48 + 0x14) = motion;
    }
    motion = CharMotionCreate((s16) pl->charNo, pl->idx == 0 ? 0x870075 : 0x8C0015);
    *(u32 *) ((u8 *) (uintptr_t) (*(u32 *) ((u8 *) (uintptr_t) lbl_1_bss_1A4[pl->idx] + 8)) + 0x48 + 0x16) = motion;
    if (pl->idx == 0) {
        *(u32 *) ((u8 *) (uintptr_t) (*(u32 *) ((u8 *) (uintptr_t) lbl_1_bss_1A4[pl->idx] + 8)) + 0x48 + 0x18) = (u32) (uintptr_t) CharMotionCreate((s16) pl->charNo, 0x8C0031);
        lbl_1_bss_15C = (*(s32 *) ((u8 *) (uintptr_t) lbl_1_bss_1A4[pl->idx] + 0x40));
        (*(s32 *) ((u8 *) (uintptr_t) lbl_1_bss_1A4[pl->idx] + 0x40)) = (s32) (uintptr_t) fn_1_698;
        lbl_1_bss_158 = (void *) (uintptr_t) (*(u32 *) ((u8 *) (uintptr_t) lbl_1_bss_1A4[pl->idx] + 0x54));
        (*(s32 *) ((u8 *) (uintptr_t) lbl_1_bss_1A4[pl->idx] + 0x54)) = (s32) (uintptr_t) fn_1_90C;
        ((s32 *) (uintptr_t) lbl_1_bss_1A4[pl->idx])[0x70 / 4] = (s32) (uintptr_t) fn_1_B04;
    }
    arg0->objFunc = fn_1_B70;
}
/*
 * fn_1_B70 rodata, extracted from the target via fnasm (build/m2c/m520Dll).
 * These are the ORIGINAL binary's .rodata, referenced as extern above; the
 * values are recorded here so the function body can be written against them
 * without re-running the extraction. Indices are the table's own offsets.
 *
 * lbl_1_rodata_6C  (8) : 30,10,10,0, 30,20,10,5      hold times, per dif
 * lbl_1_rodata_8C  (4) : 20,50,70,90                 hold times, per dif
 * lbl_1_rodata_9C  (4) : 0,10,30,50                  attack thresholds
 * lbl_1_rodata_AC  (8) : 120,60,20,20, 60,30,90,60   reaction delays
 * lbl_1_rodata_CC  (8) : 15,15,10,7, 20,20,15,10     direction-hold times
 * lbl_1_rodata_EC  (8) : 23,18,12,10, 15,5,4,4       move durations
 * lbl_1_rodata_10C (5) : 0,2,6,8,10                  ai turn delays
 * lbl_1_rodata_120 (5) : 0,0,0,5,10                  ai turn delays
 * lbl_1_rodata_134 (11): 0,0,1,2,7, 0,0,1,2,10       ai turn delays
 * lbl_1_rodata_15C (20): 40,40,74,75,108,161,186,229,278,333,374,431,470,521,565,613,643,700,700,700
 * lbl_1_rodata_1AC (20): 104,120,126,155,188,227,266,309,358,413,462,511,550,601,645,697,750,770,800,800
 * lbl_1_rodata_344/36C/49C/4D0/4F0: camera and quad matrices (already used)
 */
/* fn_1_B70's target rodata, recorded as literals (values extracted with fnasm).
 * See the note above for what each table drives. */
/* the Mg player record and its actor, as far as this file touches them */
typedef struct M520Actor_s {
    u8 pad[0x2C];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    u8 pad38[0x2C];
    f32 unk68;
    u8 pad6C[0x20];
    u32 unk84;
} M520ACTOR;

typedef struct M520MgPlayer_s {
    u8 pad0[0x02];
    s16 unk2;      /* 0x02 */
    s8 unk4;       /* 0x04 */
    u8 pad6[0x02];
    u8 *unk8;      /* 0x08 */
    u8 padC[0x1C];
    s32 unk28;     /* 0x28 */
    u8 pad2C[0x10];
    M520ACTOR *actor; /* 0x3C */
    u8 *unk40;
    u8 *unk44;
    u8 *unk48;
    u8 *unk4C;
    u8 *unk50;
    u8 *unk54;
    u8 *unk58;
    u8 *unk5C;
    u8 *unk60;
    u8 *unk64;
    u8 *unk68;
    u8 *unk6C;
    u8 *unk70;
} M520MGPLAYER;

/* 3 rows x 10 columns of animation weights, indexed [state][char]. The target
 * copies 0x78 bytes out of lbl_1_rodata_1FC with a loop, so fnasm never dumps
 * the values; they still have to be read out of the binary. */
static const f32 m520_r1FC[30] = { 0 };
static const s32 m520_r6C[8] = { 30, 10, 10, 0, 30, 20, 10, 5 };
static const s32 m520_r8C[4] = { 20, 50, 70, 90 };
static const s32 m520_r9C[4] = { 0, 10, 30, 50 };
static const s32 m520_rAC[8] = { 120, 60, 20, 20, 60, 30, 90, 60 };
static const s32 m520_rCC[8] = { 15, 15, 10, 7, 20, 20, 15, 10 };
static const s32 m520_rEC[8] = { 23, 18, 12, 10, 15, 5, 4, 4 };
static const s32 m520_r10C[5] = { 0, 2, 6, 8, 10 };
static const s32 m520_r120[5] = { 0, 0, 0, 5, 10 };
static const s32 m520_r134[11] = { 0, 0, 1, 2, 7, 0, 0, 1, 2, 10 };
static const f32 m520_r15C[20] = { 40.f, 40.f, 74.f, 75.f, 108.f, 161.f, 186.f, 229.f,
                                   278.f, 333.f, 374.f, 431.f, 470.f, 521.f, 565.f, 613.f,
                                   643.f, 700.f, 700.f, 700.f };
static const f32 m520_r1AC[20] = { 104.f, 120.f, 126.f, 155.f, 188.f, 227.f, 266.f, 309.f,
                                   358.f, 413.f, 462.f, 511.f, 550.f, 601.f, 645.f, 697.f,
                                   750.f, 770.f, 800.f, 800.f };

/* lbl_1_bss_5C8 is read both as floats and as raw ints by the target, so the
 * body uses this view for the counters at [4], [5], [6] and [0x7F]. */
typedef union M520State_u {
    f32 f;
    s32 i;
} M520STATE;

/* MGPLAYER is an opaque typedef in game/mg/actman.h, so its fields are read by
 * raw offset, as the rest of this file does for other opaque records. */
/* MGPLAYER/MGACTOR are opaque in game/mg/actman.h; forward-declared here so the
 * accessor calls below can take the pointer type without pulling the header. */
typedef struct MgPlayer_s MGPLAYER;
typedef struct MgActor_s MGACTOR;
#define MP8(p, o) (*(s8 *) ((u8 *) (p) + (o)))
#define MP16(p, o) (*(s16 *) ((u8 *) (p) + (o)))
#define MP32(p, o) (*(s32 *) ((u8 *) (p) + (o)))
#define MPP(p, o) (*(u8 **) ((u8 *) (p) + (o)))
#define F32(p, o) (*(f32 *) ((u8 *) (p) + (o)))
#define U32(p, o) (*(u32 *) ((u8 *) (p) + (o)))
#define M520I(arr, n) (((M520STATE *) (arr))[n].i)
#define M520F(arr, n) ((arr)[n])

/*
 * Per-player update. Slot 0 is the human leader and drives the ring: it reads
 * the pad, runs the 5-state AI that picks a direction, and integrates the ring
 * azimuth and speed. The other slots are AI-driven: they pick a target angle
 * on the ring and steer the stick toward it.
 */
void fn_1_B70(OMOBJ *arg0)
{
    f32 sp260[4];
    f32 px;
    f32 py;
    f32 pz;
    s32 sp138;
    f32 sp13C;
    s32 sp11C;
    s32 var_r22;
    s32 var_r21;
    s32 var_r26;
    s32 var_r27;
    s32 var_r30;
    s32 spF0;
    s32 sp48;
    M520PLAYER *pl;
    u8 *mp;
    s32 i;
    s32 j;

    pl = (M520PLAYER *) arg0->data;
    mp = (u8 *) (uintptr_t) lbl_1_bss_1A4[pl->idx];
    sp13C = 0.0f;
    var_r22 = 0;
    var_r21 = 0;
    var_r26 = 0;
    var_r27 = 0;
    for (var_r30 = 0; var_r30 < 4; var_r30++) {
        sp260[var_r30] = *(f32 *) ((u8 *) lbl_1_bss_1B8 + (var_r30 * 4));
    }
    MgActorPosGet(MPP(mp, 0x3C), &px, &py, &pz);
    if (pl->idx <= 0 || pl->unk1C != 3) {
        arg0->trans.x = px;
        arg0->trans.y = py;
        arg0->trans.z = pz;
        arg0->rot.y = F32(MPP(mp, 0x3C), 0x68);
    }
    lbl_1_bss_1A0 = MgSeqModeGet();
    lbl_1_bss_0 = MgSeqFrameNoGet();
    sp138 = HuPadBtn[(s8) MP8(mp, 4)] & 0xFFFFFEFF;
    var_r27 = HuPadBtnDown[(s8) MP8(mp, 4)] & 0xFFFFFEFF;
    if (lbl_1_bss_1A0 != 5) {
        MgPlayerPadSet((MGPLAYER *) mp, 0, 0, 0, 0);
        HuPadTrigR[(s8) MP8(mp, 4)] = 0;
        HuPadTrigL[(s8) MP8(mp, 4)] = 0;
    } else if (pl->padNo != -1) {
        var_r22 = (s8) (u8) HuPadStkX[pl->padNo];
        var_r21 = (s8) (u8) HuPadStkY[pl->padNo];
        var_r27 = HuPadBtn[pl->padNo];
        var_r26 = HuPadBtnDown[pl->padNo];
    }
    if (pl->idx == 0) {
        HU3DMODEL *mdl = &Hu3DData[arg0->mdlId[0]];
        f32 target;
        f32 step;

        target = 0.0f;
        Hu3DModelAttrReset(arg0->mdlId[0], 1);
        Hu3DModelAttrSet(arg0->mdlId[2], 1);
        if (lbl_1_bss_1A0 == 2) {
            if (lbl_1_bss_0 == 0x44) {
                var_r26 = 0x100;
                var_r27 = 0x100;
            } else if (lbl_1_bss_0 > 0x44 && lbl_1_bss_0 <= 0x47) {
                var_r27 = 0x100;
            } else if (lbl_1_bss_0 == 0x4E) {
                var_r26 = 0x100;
                var_r27 = 0x100;
            }
        } else if (lbl_1_bss_1A0 == 5 && pl->padNo != -1) {
            target = (f32) (((HuPadBtn[pl->padNo] & 0x20) > 0 ? 0x60 : 0) - ((HuPadBtn[pl->padNo] & 0x40) > 0 ? 0x60 : 0)) / 96.0f;
        } else if (lbl_1_bss_1A0 == 5) {
            /* pick the target: the closest of the three AI players, measured in
             * the ring's own angle, preferring the nearer one */
            s32 best = -1;
            f32 bestScore = -1.0f;
            f32 cur = M520F(lbl_1_bss_5C8, 2);

            for (i = 0; i < 3; i++) {
                f32 ox;
                f32 oz;
                f32 ang;
                f32 dist;
                f32 len2;
                f32 len;
                f32 near = 360.0f;
                f32 score;

                MgActorPosGet(MPP((u8 *) (uintptr_t) lbl_1_bss_1A4[i + 1], 0x3C), &ox, &py, &oz);
                ang = 180.0f * ((f32) atan2(ox, oz) / (f32) M_PI);
                len2 = (ox * ox) + (oz * oz);
                if (len2 > 0.0f) {
                    f64 r = __frsqrte(len2);

                    r = 0.5 * r * (3.0 - (len2 * (r * r)));
                    r = 0.5 * r * (3.0 - (len2 * (r * r)));
                    len = (f32) (len2 * (0.5 * r * (3.0 - (len2 * (r * r)))));
                } else {
                    len = len2;
                }
                for (j = 0; j < 3; j++) {
                    f32 d = ang - (cur + (f32) (j * 0x78));

                    while (d < -180.0f) {
                        d += 360.0f;
                    }
                    while (d > 180.0f) {
                        d -= 360.0f;
                    }
                    if (fabs(d) < fabs(near)) {
                        near = d;
                    }
                }
                score = (60.0f - fabs(near)) + (10.0f * ((len - 300.0f) / 400.0f));
                if (best == -1 || bestScore < score) {
                    best = i;
                    bestScore = score;
                }
            }
            /* 5-state AI: idle -> charge -> lean -> fling -> recover */
            switch (pl->unk38) {
            case 0:
                target = M520F(lbl_1_bss_5C8, 3);
                if (pl->unk3C == 0) {
                    pl->unk40 = m520_r6C[pl->dif] + (rand8() % 10);
                }
                pl->unk3C += 1;
                if (pl->unk3C >= pl->unk40) {
                    if (M520I(lbl_1_bss_5C8, 6) == 0) {
                        target = 3;
                        pl->unk3C = 0;
                    } else {
                        pl->unk40 = m520_rAC[pl->dif] + (rand8() % 10);
                        if (pl->dif == 3) {
                            pl->unk38 = (M520F(lbl_1_bss_5C8, 3) * ((f32 *) sp260)[best]) < 0.0f ? 1 : 2;
                        } else {
                            pl->unk38 = ((f32 *) sp260)[best] > (100.0f * ((f32) rand8() / 255.0f)) ? 2 : 1;
                        }
                        pl->unk3C = 0;
                    }
                }
                break;
            case 3:
                if (pl->unk3C == 1) {
                    var_r26 = 0x100;
                    var_r27 = 0x100;
                } else if (pl->unk3C < m520_rCC[pl->dif]) {
                    var_r27 = 0x100;
                } else if (pl->unk3C == m520_rCC[pl->dif]) {
                    var_r26 = 0x100;
                    var_r27 = 0x100;
                }
                pl->unk3C += 1;
                if (pl->unk3C > m520_rCC[pl->dif] && MP32(mp, 0x28) == 0) {
                    pl->unk3C = 0;
                    switch (pl->dif) {
                    case 0:
                        pl->unk38 = 4;
                        pl->unk48 = 0.01f * ((rand8() & 1) ? 1.0f : -1.0f);
                        pl->unk40 = 0x78;
                        break;
                    case 1:
                        pl->unk38 = 5;
                        pl->unk48 = (rand8() & 1) ? 1.0f : -1.0f;
                        pl->unk40 = 0x78;
                        break;
                    case 2:
                        pl->unk38 = (rand8() & 1) + 4;
                        pl->unk48 = (rand8() & 1) ? 1.0f : -1.0f;
                        pl->unk40 = pl->unk38 == 4 ? rand8() % 30 : (rand8() % 40) + 0x28;
                        break;
                    case 3:
                        pl->unk38 = (rand8() % 2) + 4;
                        pl->unk48 = (rand8() % 3 == 0 ? 1.0f : 0.01f) * ((rand8() & 1) ? 1.0f : -1.0f);
                        pl->unk40 = pl->unk38 == 4 ? (rand8() % 25) + 0xF : (rand8() % 65) + 0x23;
                        break;
                    }
                }
                break;
            default:
                target = pl->unk48;
                if (M520F(lbl_1_bss_5C8, 3) == pl->unk48) {
                    pl->unk3C += 1;
                    if (pl->unk38 != 4 && pl->unk3C > pl->unk40) {
                        pl->unk38 = 0;
                        pl->unk40 = 1;
                        pl->unk3C = 1;
                    } else if (pl->unk3C > pl->unk40 && pl->unk38 == 4) {
                        pl->unk38 = 5;
                        pl->unk3C = 0;
                        pl->unk40 = (rand8() % 40) + 0x64;
                        pl->unk48 = pl->unk48 < 0.0f ? 1.0f : -1.0f;
                    }
                }
                break;
            }
        }
        if (MP32(mp, 0x28) != 0 || mdl->motIdShift != -1) {
            target = 0.0f;
        }
        /* integrate the ring speed toward the target, then the azimuth */
        step = M520F(lbl_1_bss_5C8, 3);
        if (fabs(target) < fabs(M520F(lbl_1_bss_5C8, 3))) {
            step = 1.0f / 15.0f;
        } else {
            step = 1.5f / 15.0f;
        }
        if (M520F(lbl_1_bss_5C8, 3) < target) {
            j = 1;
        } else {
            j = -1;
        }
        M520F(lbl_1_bss_5C8, 3) += (f32) j * step;
        if ((step == target && M520F(lbl_1_bss_5C8, 3) > target) || (step == target && M520F(lbl_1_bss_5C8, 3) < target)) {
            M520F(lbl_1_bss_5C8, 3) = target;
        }
        /* the 60-entry ring history, wrapped with a modulo */
        ((f32 *) ((u8 *) lbl_1_bss_5C8 + (M520I(lbl_1_bss_5C8, 0x7F) * 0x1C)))[0x1C / 4] = M520F(lbl_1_bss_5C8, 2);
        ((f32 *) ((u8 *) lbl_1_bss_5C8 + (M520I(lbl_1_bss_5C8, 0x7F) * 0x1C)))[0x10C / 4] = M520F(lbl_1_bss_5C8, 3);
        M520I(lbl_1_bss_5C8, 0x7F) += 1;
        M520I(lbl_1_bss_5C8, 0x7F) %= 60;
        M520F(lbl_1_bss_5C8, 2) += (360.0f * M520F(lbl_1_bss_5C8, 3)) / 360.0f;
        if (M520F(lbl_1_bss_5C8, 2) < -180.0f) {
            M520F(lbl_1_bss_5C8, 2) += 360.0f;
        } else if (M520F(lbl_1_bss_5C8, 2) == 180.0f) {
            M520F(lbl_1_bss_5C8, 2) -= 360.0f;
        }
        {
            s32 mdl2 = *(s32 *) ((u8 *) lbl_1_bss_1B4 + 0x14);

            Hu3DModelRotSet(mdl2 + 0xC, 0.0f, M520F(lbl_1_bss_5C8, 2), 0.0f);
            Hu3DModelRotSet(mdl2 + 0x10, 0.0f, M520F(lbl_1_bss_5C8, 2), 0.0f);
            Hu3DMotionTimeSet(mdl2 + 0xE, 20.0f + (20.0f * M520F(lbl_1_bss_5C8, 3)));
        }
        if (lbl_1_bss_10 == 0 && ((target < 0.0f && lbl_1_bss_C != 0.0f) || (target > 0.0f && lbl_1_bss_C != 0.0f))) {
            if (((s32 *) ((u8 *) lbl_1_bss_1D0 + 0x23))[0] < 3) {
                ((f32 *) ((u8 *) lbl_1_bss_1D0 + (((s32 *) ((u8 *) lbl_1_bss_1D0 + 0x23))[0] * 0x14)))[0x90 / 4] = 0.0f;
                ((s32 *) ((u8 *) lbl_1_bss_1D0 + 0x23))[0] += 1;
            }
            lbl_1_bss_10 = 6;
        }
        lbl_1_bss_10 -= lbl_1_bss_10 > 0;
        lbl_1_bss_C = target;
        var_r22 = 0;
        var_r21 = 0;
    } else {
        f32 spB8;
        f32 sp114;
        s32 sp118;

        sp11C = pl->unk1C;
        sp118 = (U32(MPP(mp, 0x3C), 0x84) & 0x407F) == 0;
        if (lbl_1_bss_1A0 == 2 || lbl_1_bss_1A0 == 3) {
            if (lbl_1_bss_1A0 == 2) {
                if (lbl_1_bss_0 < 0xD7) {
                    F32(MPP(mp, 0x3C), 0x2C) = (f32) (0x21C * sin((M_PI * ((0.5 * (pl->idx == 1)) + ((pl->idx * 0x78) + 0x3C))) / 180.0));
                    F32(MPP(mp, 0x3C), 0x34) = (f32) ((0x21C * cos((M_PI * ((0.5 * (pl->idx == 1)) + ((pl->idx * 0x78) + 0x3C))) / 180.0)) - 25.0);
                }
                if (lbl_1_bss_0 == 0xE1) {
                    var_r26 = 0x100;
                    var_r27 = 0x100;
                } else if (lbl_1_bss_0 > 0xE1 && lbl_1_bss_0 <= 0xF0) {
                    var_r27 = 0x100;
                }
            }
            if (lbl_1_bss_1A0 == 2 && ((lbl_1_bss_0 >= 0xDC && lbl_1_bss_0 <= 0xE1) || sp118 != 0)) {
                f32 r = 55.0f * (-px / sp114);

                var_r22 = (s8) r;
                var_r21 = (s8) (55.0f * (-pz / sp114));
            }
        } else if (lbl_1_bss_1A0 == 5 && pl->unk1C == 2 && pl->padNo == -1) {
            /* AI player: steer the stick toward the chosen angle on the ring */
            f32 ang;
            f32 dist;
            f32 best = 10000.0f;
            f32 radius;
            f32 ringAng;
            f32 ringSpd;
            f32 tx;
            f32 tz;
            f32 tl;
            f32 speed;
            s32 idx;

            sp48 = 0;
            if (M520I(lbl_1_bss_5C8, 6) > 0 && M520I(lbl_1_bss_5C8, 6) < 0xBE) {
                sp48 = 1;
            }
            idx = ((M520I(lbl_1_bss_5C8, 0x7F) - m520_rEC[pl->dif]) + 0x3C) % 60;
            ringAng = ((f32 *) ((u8 *) lbl_1_bss_5C8 + (idx * 0x1C)))[0x1C / 4];
            ringSpd = ((f32 *) ((u8 *) lbl_1_bss_5C8 + (idx * 0x1C)))[0x10C / 4];
            ang = 180.0f * ((f32) atan2(px, pz) / (f32) M_PI);
            for (i = -6; i <= 6; i++) {
                f32 d = fabs(ang - (ringAng + (f32) (i * 0x78)));

                if (fabs(best) > d) {
                    best = ang - (ringAng + (f32) (i * 0x78));
                }
            }
            if (M520I(lbl_1_bss_5C8, 6) == 0) {
                speed = 360.0f;
            } else if (M520I(lbl_1_bss_5C8, 6) < 0x46) {
                speed = 360.0f + (80.0f * ((f32) M520I(lbl_1_bss_5C8, 6) / 70.0f));
            } else if (M520I(lbl_1_bss_5C8, 6) < 0x64) {
                speed = 440.0f;
            } else if (M520I(lbl_1_bss_5C8, 6) < 0xA0) {
                speed = 210.0f;
            } else if (M520I(lbl_1_bss_5C8, 6) < 0xBE) {
                speed = 210.0f + (150.0f * ((f32) (M520I(lbl_1_bss_5C8, 6) - 0xA0) / 30.0f));
            } else {
                speed = 360.0f;
            }
            var_r26 = M520I(lbl_1_bss_5C8, 6) == 0x66 ? 0x100 : 0;
            var_r27 = (M520I(lbl_1_bss_5C8, 6) >= 0x66 && M520I(lbl_1_bss_5C8, 6) <= 0x6E) ? 0x100 : 0;
            pl->unk3C += 1;
            switch (pl->unk38) {
            case 0:
                pl->unk4C = ang;
                if ((fabs(ringSpd) > 0.3f && (best * ringSpd) > 0.0f && fabs(best) < 45.0f)
                    || (fabs(best) < 35.0f && ((best * ringSpd) > 0.0f || fabs(ringSpd) < 0.5f))) {
                    pl->unk38 = best < 0.0f ? -1 : 1;
                    pl->unk3C = 0;
                }
                break;
            case 1:
            case -1:
                if (pl->unk3C >= pl->unk40
                    && ((fabs(ringSpd) < 0.3f && fabs(best) > 45.0f) || ((best * (f32) pl->unk38) < 0.0f && fabs(best) < 35.0f))) {
                    pl->unk38 = 0;
                    pl->unk3C = 0;
                } else {
                    pl->unk4C = ang + (f32) (pl->unk38 * 5);
                }
                if (pl->unk3C >= pl->unk40) {
                    switch (pl->dif) {
                    case 0:
                        pl->unk40 = ((rand8() % 3) * 5) + 5;
                        break;
                    case 1:
                        pl->unk40 = m520_r10C[rand8() % 5];
                        break;
                    case 2:
                        pl->unk40 = m520_r120[rand8() % 5];
                        break;
                    case 3:
                        pl->unk40 = m520_r134[(sp48 * 5) + (rand8() % 5)];
                        break;
                    }
                    pl->unk3C = 0;
                }
                break;
            }
            pl->unk50 = speed;
            tx = (f32) ((f64) pl->unk50 * sin((M_PI * pl->unk4C) / 180.0)) - px;
            tz = (f32) ((f64) pl->unk50 * cos((M_PI * pl->unk4C) / 180.0)) - pz;
            tl = sqrtf((tx * tx) + (tz * tz));
            speed = 5.6f * tl;
            dist = speed < 120.0f ? 1.0f : 64.0f * (1.0f / speed);
            var_r22 = (s8) ((5.6f * tx) * dist);
            var_r21 = (s8) ((-5.6f * tz) * dist);
        }
        if ((lbl_1_bss_1A0 == 5 || lbl_1_bss_1A0 == 6) && pl->unk1C == 2) {
            f32 spB4;

            if (px * px + pz * pz > 0.0f) {
                f64 r = __frsqrte(px * px + pz * pz);

                r = 0.5 * r * (3.0 - ((px * px + pz * pz) * (r * r)));
                r = 0.5 * r * (3.0 - ((px * px + pz * pz) * (r * r)));
                spB4 = (f32) ((px * px + pz * pz) * (0.5 * r * (3.0 - ((px * px + pz * pz) * (r * r)))));
            } else {
                spB4 = px * px + pz * pz;
            }
            sp114 = spB4;
            if ((sp114 > 700.0f && (sp118 == 0 || py < 0.0f))
                || (lbl_1_bss_1A0 == 5 && M520I(lbl_1_bss_5C8, 6) >= 0x28 && M520I(lbl_1_bss_5C8, 6) < 0xC8
                    && (U32(MPP(mp, 0x3C), 0x84) & 0x407F) != 0)) {
                pl->unk1C = 3;
            }
            if (lbl_1_bss_1A0 == 5) {
                for (i = 0; i < 0x1E; i++) {
                    f32 *rec = (f32 *) ((u8 *) lbl_1_bss_280 + (i * 0x14));

                    if (((s32 *) rec)[6] == 0) {
                        continue;
                    }
                    if (((px - rec[0]) * (px - rec[0])) + ((pz - rec[2]) * (pz - rec[2])) < 9025.0f) {
                        pl->unk1C = 3;
                    }
                }
            }
        }
        if (pl->unk1C == 3) {
            f32 spA4;

            var_r26 = 0;
            var_r27 = 0;
            var_r21 = 0;
            var_r22 = 0;
            if (sp11C == 2) {
                MgPlayerModeSet((MGPLAYER *) mp, 0xB);
                MgPlayerPauseOn((MGPLAYER *) mp);
                omVibrate((s16) pl->conf, 0x30, 1, 0);
                CharFXPlay((s16) pl->charFull, 0x1F9);
                pl->unk34 = 1.0f;
            } else {
                px = arg0->trans.x;
                py = arg0->trans.y;
                pz = arg0->trans.z;
            }
            if (px * px + pz * pz > 0.0f) {
                f64 r = __frsqrte(px * px + pz * pz);

                r = 0.5 * r * (3.0 - ((px * px + pz * pz) * (r * r)));
                r = 0.5 * r * (3.0 - ((px * px + pz * pz) * (r * r)));
                spA4 = (f32) ((px * px + pz * pz) * (0.5 * r * (3.0 - ((px * px + pz * pz) * (r * r)))));
            } else {
                spA4 = px * px + pz * pz;
            }
            if (spA4 > 1800.0f) {
                Hu3DModelAttrSet(arg0->mdlId[0], 1);
            }
            CharMotionShiftSet(MP16(mp, 2), F32(MPP(MPP(mp, 8), 0x48), 0x16), 1.0f, 0.0f, 0x40000001);
            {
                f32 dx = spA4 == 0.0f ? 0.0f : px / spA4;
                f32 dz = spA4 == 0.0f ? 0.0f : pz / spA4;

                px += 16.0f * dx;
                pz += 16.0f * dz;
                py += 50.0f * pl->unk34;
                pl->unk34 -= 0.04f;
                arg0->rot.y = 180.0f * ((f32) atan2(dx, dz) / (f32) M_PI);
                omSetTra(arg0, px, py, pz);
            }
        }
    }
    if ((lbl_1_bss_1A0 == 8 || lbl_1_bss_1A0 == 9) && lbl_1_data_9C == 0 && pl->idx == 0) {
        HU3DMODEL *mdl = &Hu3DData[arg0->mdlId[2]];

        Hu3DModelAttrSet(arg0->mdlId[0], 1);
        Hu3DModelAttrSet(*(u32 *) ((u8 *) lbl_1_bss_1B8 + 4), 1);
        Hu3DModelAttrSet(*(u32 *) ((u8 *) lbl_1_bss_1B8 + 8), 1);
        Hu3DModelAttrSet(*(u32 *) ((u8 *) lbl_1_bss_1B8 + 0xC), 1);
        Hu3DModelAttrReset(arg0->mdlId[2], 1);
        if (pl->charNo == 3) {
            i = -0x1E;
        } else {
            i = pl->charNo == 4 ? -0x1E : 0;
        }
        Hu3DModelPosSet(arg0->mdlId[2], arg0->trans.x, arg0->trans.y, arg0->trans.z + (f32) i);
        if (mdl->motId != (s16) arg0->mtnId[4]) {
            Hu3DMotionSet(arg0->mdlId[2], arg0->mtnId[4]);
            Hu3DModelAttrReset(arg0->mdlId[2], 0x40000002);
        } else if ((s32) (mdl->motWork.time - 1) == *(s32 *) ((u8 *) (uintptr_t) lbl_1_data_D0 + pl->charFull)) {
            CharFXPlay((s16) pl->charFull, 0x1FA);
        }
    } else if (lbl_1_bss_1A0 == 8 && lbl_1_data_9C > 0 && pl->idx > 0) {
        MgPlayerModeSet((MGPLAYER *) mp, 0xA);
    } else if (lbl_1_bss_1A0 == 8 && lbl_1_data_9C == 1 && pl->idx == 0) {
        MgPlayerModeSet((MGPLAYER *) mp, 0xC);
    }
    MgPlayerPadSet((MGPLAYER *) mp, var_r22, var_r21, var_r26 | var_r27, var_r27 | sp138);
    HuPadBtnRep[(s8) MP8(mp, 4)] &= 0x1010;
    HuPadBtnRep[(s8) MP8(mp, 4)] |= var_r26 | var_r27;
    if (pl->idx == 0) {
        f32 spCC = 0.0f;
        HU3DMODEL *mdl = &Hu3DData[arg0->mdlId[0]];

        if (lbl_1_bss_1A0 == 2 && lbl_1_bss_0 <= 0x44) {
            spCC = *(f32 *) ((u8 *) &m520_r1FC[0] + (pl->charNo * 4));
        } else {
            s32 sp208[2];

            for (i = 0; i < 2; i++) {
                s16 id = i == 0 ? mdl->motId : mdl->motIdShift;

                if (id == -1) {
                    sp208[i] = -1;
                } else if (id == (s16) F32(MPP(MPP(mp, 8), 0x48), 0x16)) {
                    sp208[i] = 0;
                } else if (id == (s16) F32(MPP(MPP(mp, 8), 0x48), 0x14) || id == (s16) F32(MPP(MPP(mp, 8), 0x48), 0x18)) {
                    sp208[i] = 2;
                } else {
                    sp208[i] = 1;
                }
            }
            spCC = *(f32 *) ((u8 *) &m520_r1FC[0] + ((sp208[0] * 0x28) + (pl->charNo * 4)));
            if (sp208[1] != -1) {
                f32 t = mdl->motOvlWork.start / mdl->motOvlWork.end;

                spCC = (spCC * (1.0f - t)) + (t * *(f32 *) ((u8 *) &m520_r1FC[0] + ((sp208[1] * 0x28) + (pl->charNo * 4))));
            }
        }
        F32(MPP(mp, 0x3C), 0x34) = spCC;
    }
}
/*
 * Module state. These live in the REL's own .bss: the original m520.c never
 * declared them because the unit was still incomplete, and the port's linker
 * needs them resolved in this object rather than imported from dol.
 */
void *lbl_1_bss_1B8[4];
s32 lbl_1_bss_1A0;
s32 lbl_1_bss_0;
s32 lbl_1_bss_4;
s32 lbl_1_bss_10;
s32 lbl_1_bss_14;
f32 lbl_1_bss_C;
s32 lbl_1_bss_164;
s32 lbl_1_bss_16C;
s32 lbl_1_bss_17C;
void *lbl_1_bss_158;
s32 lbl_1_bss_15C;
u32 lbl_1_data_A4;
void *lbl_1_bss_19C;
void *lbl_1_bss_1B4;
void *lbl_1_bss_1C8;
void *lbl_1_bss_1CC;


s32 lbl_1_bss_1A4[4];
s32 lbl_1_data_9C;
s32 lbl_1_data_A0;
void *lbl_1_data_138;
Mtx lbl_1_bss_18;
Mtx lbl_1_bss_48;
Mtx lbl_1_bss_B8;
Mtx lbl_1_bss_E8;
Mtx lbl_1_bss_A8;
Mtx lbl_1_bss_98;
GXTexObj lbl_1_bss_118[2];
u8 lbl_1_bss_EC[0x20];
u8 lbl_1_rodata_460[0x28];
u8 lbl_1_rodata_49C[0x18];
u8 lbl_1_rodata_4D0[0x14];
u8 lbl_1_rodata_344[0x28];
u8 lbl_1_rodata_36C[0x28];
u8 lbl_1_rodata_4F0[0x10];
s32 fontcolor;
u16 HuPadBtnDown[4];


/* Module state, defined here rather than imported: the original m520.c never
 * declared them (the unit was incomplete) and the port's linker needs them in
 * this object. Types match the extern declarations above. */
s16 lbl_1_bss_198;
u8 RenderMode[0x40];
s32 lbl_1_bss_280[0x348 / 4];
void *lbl_1_bss_160;
u8 lbl_1_data_EC[0x40];
s8 lbl_1_bss_270[0x20];
s32 lbl_1_data_D0;
s32 lbl_1_bss_190;
void *lbl_1_bss_168;
u32 lbl_1_data_A8[0x10];
f32 CZoom;
HuVecF Center;
HuVecF CRot;
HU3DMODEL Hu3DData[0x200];
u32 lbl_1_data_100;
