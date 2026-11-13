#include "game/mg/seqman.h"
#include "game/charman.h"
#include "game/memory.h"
#include "game/data.h"
#include "game/init.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/printfunc.h"
#include "game/gamework.h"
#include "game/flag.h"
#include "game/audio.h"
#include "game/wipe.h"
#include "game/gamemes.h"
#include "game/mg/actman.h"
#include "game/mg/colman.h"
#include <math.h>

/* lbl_1_bss_5C8 is read both as floats and as raw ints by the target */
typedef union M520State_u {
    f32 f;
    s32 i;
} M520STATE;

void fn_1_5F48(void);
void fn_1_5F4C(void);
void fn_1_5F50(void);
void fn_1_5F54(void);
void fn_1_5F58(void);
void fn_1_5F5C(void);
void fn_1_5F60(s16 mode, s16 frame);
void fn_1_9CE0(void);
void fn_1_9E18(void);

/* module state (the REL's own .bss/.data); pointers are real pointers, not 32-bit slots */
OMOBJMAN *lbl_1_bss_19C;
OMOBJ *lbl_1_bss_1B4;
OMOBJ *lbl_1_bss_1B8[4];
OMOBJ *lbl_1_bss_1C8;
OMOBJ *lbl_1_bss_1CC;
MGPLAYER *lbl_1_bss_1A4[4];
MGPLAYER_MODE_FUNC lbl_1_bss_158;
MGPLAYER_MODE_FUNC lbl_1_bss_15C;
s32 lbl_1_bss_0;
s32 lbl_1_bss_4;
f32 lbl_1_bss_C;
s32 lbl_1_bss_10;
s32 lbl_1_bss_14;
Mtx lbl_1_bss_18;
Mtx lbl_1_bss_48;
Mtx lbl_1_bss_98;
Mtx lbl_1_bss_A8;
Mtx lbl_1_bss_B8;
Mtx lbl_1_bss_E8;
GXTexObj lbl_1_bss_118[2];
void *lbl_1_bss_160;
s32 lbl_1_bss_164;
void *lbl_1_bss_168;
s32 lbl_1_bss_16C;
s32 lbl_1_bss_17C;
s32 lbl_1_bss_190;
s16 lbl_1_bss_198;
s32 lbl_1_bss_1A0;
s32 lbl_1_bss_1D0[0xA0 / 4];
s32 lbl_1_bss_270[4];
s32 lbl_1_bss_280[0x348 / 4];
M520STATE lbl_1_bss_5C8[0x200 / 4];

s32 lbl_1_data_9C = -1;
s32 lbl_1_data_A0 = -1;
u32 lbl_1_data_A4 = 1;
MGSEQ_PARAM lbl_1_data_A8 = {
    0x1E, 2, (MGSEQ_FUNC) fn_1_9CE0, (MGSEQ_FUNC) fn_1_5F48, (MGSEQ_FUNC) fn_1_5F4C, (MGSEQ_FUNC) fn_1_5F50,
    (MGSEQ_FUNC) fn_1_5F54, (MGSEQ_FUNC) fn_1_5F58, (MGSEQ_FUNC) fn_1_5F5C, fn_1_5F60, (MGSEQ_FUNC) fn_1_9E18
};
u8 lbl_1_data_D0[13] = { 0x0D, 0x00, 0x17, 0x69, 0x0D, 0x1D, 0x14, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D };
f32 lbl_1_data_EC[5] = { 32.0f, 32.0f, 32.0f, 32.0f, 60.0f };
u32 lbl_1_data_100 = 6;
unsigned int lbl_1_data_138[17] = {
    0x00870075, 0x008C0001, 0x008C0002, 0x008C0003, 0x008C0004, 0x008C0005, 0x008C0050, 0x008C0008, 0x008C000A,
    0x008C0009, 0x008C000F, 0x008C0010, 0x008C0012, 0x008C0013, 0x008C0017, 0x008C0018, 0
};

static const f32 lbl_1_rodata_344[10] = { 480.0f, 530.0f, 530.0f, 480.0f, 480.0f, 530.0f, 550.0f, 440.0f, 480.0f, 480.0f };
static const f32 lbl_1_rodata_36C[10] = { -40.0f, -34.0f, -27.0f, -40.0f, -38.0f, -27.0f, -40.0f, -40.0f, -30.0f, -40.0f };
static const f32 lbl_1_rodata_460[12] = { -150.0f, 150.0f, 0.0f, 150.0f, 150.0f, 0.0f, 150.0f, -150.0f, 0.0f, -150.0f, -150.0f, 0.0f };
static const f32 lbl_1_rodata_49C[6] = { 0.0f, 0.0026666666381061077f, 0.0f, 0.004000000189989805f, 0.0f, 0.0f };
static const u32 lbl_1_rodata_4D0[5] = { 0x004E000D, 0x004E000E, 0x004E000F, 0x004E0010, 0x004E0011 };
static const u32 lbl_1_rodata_4F0[4] = { 0, 1, 2, 3 };

extern void (*_dtors[])(void);
extern void (*_ctors[])(void);
extern void ObjectSetup(void);
extern void fn_1_5F90(void);
/* the sprite pool lives in the DOL (src/game/esprite.c); same views as there */
typedef struct esprite_s {
    s16 memberNo;
    s16 animNo;
} ESPRITE;
typedef struct espanim_s {
    unsigned int dataNum;
    u16 useCnt;
    ANIMDATA *anim;
} ESPANIM;
SHARED_SYM extern ESPRITE esprite[];
SHARED_SYM extern ESPANIM espanim[];
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);
extern void fn_1_43BC(OMOBJ *);
extern void fn_1_B70(OMOBJ *);

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

    r29 = 0;
    r30 = 0;
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
            GXPosition3f32(0.0f, (f32) r31 / 480.0f, 0.0f);
            GXColor4u8(0, (u8) r29, (u8) r30, 0xFF);
            GXPosition3f32(0.05f, (f32) r31 / 480.0f, 0.0f);
            GXColor4u8(0, (u8) r29, (u8) r30, 0xFF);
        }
        GXPosition3f32(0.0f, (f32) r31 / 480.0f, 0.0f);
        GXColor4u8(0, (u8) r29, (u8) r30, 0xFF);
        GXPosition3f32(0.05f, (f32) r31 / 480.0f, 0.0f);
        GXColor4u8(0, (u8) r29, (u8) r30, 0xFF);
        GXSetCopyFilter(0, RenderMode->sample_pattern, 0, RenderMode->vfilter);
        GXSetTexCopySrc(0, 0, 0x20, 0x20);
        GXSetTexCopyDst(0x20, 0x20, GX_CTF_GB8, 0);
        GXCopyTex(lbl_1_bss_160, 0);
        DCFlushRange(lbl_1_bss_160, (u32) lbl_1_bss_164);
        GXPixModeSync();
        GXSetCopyFilter(RenderMode->aa, RenderMode->sample_pattern, 1, RenderMode->vfilter);
        lbl_1_bss_164 = 0;
    }
}
extern void fn_1_7ED8(OMOBJ *);
extern void fn_1_8A98(OMOBJ *);
extern void fn_1_9860(OMOBJ *);
extern void *HuAR_ARAMtoMRAMFileRead(unsigned int dataNum, u32 num, HEAPID heap);
/* fn_1_A0 reads the pad directly; game/pad.h declares the arrays it needs. */
#include "game/pad.h"
/* not declared in game/hu3d.h on purpose (see there): this unit uses the float return */
float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);
#ifdef TARGET_PC
double __frsqrte(double);
s16 print8(s16 x, s16 y, float scale, char *str, ...);
#endif

/*
 * Free-camera debug update: reads the pad and nudges the camera pivot
 * (Center), the zoom (CZoom) and the orbit angles (CRot), prints them, then
 * converts the spherical orbit back to a cartesian eye position. The pad bit
 * extractions and the double-precision trigonometry are what the target does;
 * the constants are lbl_1_rodata_10 (25, the zoom step), _18 (0.03125, the
 * horizontal nudge), _20 (-32, the vertical nudge divisor), _30/_38 (PI/180).
 */
// NONMATCHING: debug-only camera nudge; the unused eye/centre vectors are kept in different stack slots
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
static const Vec kLightPos = {0.0f, 5000.0f, 0.0f};
static const Vec kLightDir = {0.0f, -1.0f, -0.5f};
static const GXColor kLightCol = {0xFF, 0xFF, 0xFF, 0xFF};
static const Vec kShadowPos = {0.0f, 10000.0f, 0.0f};
static const Vec kShadowUp = {0.0f, 1.0f, 0.0f};
static const Vec kShadowTarget = {0.0f, 0.0f, -0.1f};

void fn_1_5F60(s16 mode, s16 frame)
{
    if (frame >= 0x3C) {
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
    if (*dtor != NULL) {
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
    if (*ctor != NULL) {
        goto call;
    }
    ObjectSetup();
    return 0;
}

void fn_1_B04(MGPLAYER *player)
{
    if (player->motNo == 0xC) {
        return;
    }
    CharMotionShiftSet(player->charNo, player->omObj->mtnId[12], 0.0f, 1.0f, 0);
    player->motNo = 0xC;
}

void fn_1_A94(MGPLAYER *player)
{
    if (player->motNo == 0xB) {
        return;
    }
    CharMotionShiftSet(player->charNo, player->omObj->mtnId[11], 0.0f, 1.0f, 0x40000001);
    player->motNo = 0xB;
}

void fn_1_A08(MGPLAYER *player)
{
    if (player->motNo != 0xA) {
        CharMotionShiftSet(player->charNo, player->omObj->mtnId[10], 0.0f, 5.0f, 0);
        player->motNo = 0xA;
    }
    player->actor->rotY = (f32) (player->actor->rotY * 0.7);
}

void fn_1_9860(OMOBJ *arg0)
{
    arg0->objFunc = (OMOBJFUNC) fn_1_5F90;
}

/*
 * Main update: mirrors each Mg actor's position onto its shadow model, runs the
 * per-slot sound effects, then dispatches on the sequence mode (jump table
 * jumptable_1_data_104: modes 1, 2, 5, 6 and 7 have code).
 */
// NONMATCHING: switch lowered differently and register allocation differs
void fn_1_5F90(void)
{
    s32 *pd[4];
    s32 frame;
    s32 i;
    s32 snd[8];
    HuVecF pos;
    s32 plNo;

    frame = MgSeqFrameNoGet();
    lbl_1_bss_1A0 = MgSeqModeGet();
    for (i = 0; i < 4; i++) {
        pd[i] = (s32 *) lbl_1_bss_1B8[i]->data;
    }
    MgActorExec();
    for (i = 0; i < 4; i++) {
        MgActorPosGet(lbl_1_bss_1A4[i]->actor, &pos);
        if (i > 0 && pd[i][0x1C / 4] == 3) {
            /* fallen player: keep the last mirrored position */
        } else {
            lbl_1_bss_1B8[i]->trans.x = pos.x;
            lbl_1_bss_1B8[i]->trans.y = pos.y;
            lbl_1_bss_1B8[i]->trans.z = pos.z;
            lbl_1_bss_1B8[i]->rot.y = lbl_1_bss_1A4[i]->actor->rotY;
        }
        if (i == 0) {
            if (lbl_1_bss_1A0 == 2 && frame == 6) {
                Hu3DMotionShiftSet(lbl_1_bss_1B8[0]->mdlId[1], lbl_1_bss_1B8[0]->mtnId[1],
                                   10.0f + (10.0f * lbl_1_bss_5C8[3].f), 12.0f, 0x40000002);
            }
            if (lbl_1_bss_1A0 == 2 && frame <= 0x44) {
                Hu3DModelPosSet(lbl_1_bss_1B8[0]->mdlId[1], lbl_1_bss_1B8[0]->trans.x, lbl_1_bss_1B8[0]->trans.y,
                                lbl_1_bss_1B8[0]->trans.z);
                Hu3DModelAttrReset(lbl_1_bss_1B8[0]->mdlId[1], 1);
                Hu3DModelAttrSet(lbl_1_bss_1B8[0]->mdlId[0], 1);
                Hu3DModelAttrSet(lbl_1_bss_1B8[2]->mdlId[0], 1);
                Hu3DModelAttrSet(lbl_1_bss_1B8[3]->mdlId[0], 1);
            } else if (lbl_1_bss_1A0 == 2) {
                Hu3DModelAttrSet(lbl_1_bss_1B8[0]->mdlId[1], 1);
                Hu3DModelAttrReset(lbl_1_bss_1B8[0]->mdlId[0], 1);
                Hu3DModelAttrReset(lbl_1_bss_1B8[2]->mdlId[0], 1);
                Hu3DModelAttrReset(lbl_1_bss_1B8[3]->mdlId[0], 1);
            }
        }
    }
    for (i = 0; i < 8; i++) {
        snd[0] = 0x722;
        snd[1] = 0x723;
        snd[2] = 0x724;
        snd[3] = 0x725;
        snd[4] = 0x726;
        snd[5] = 0x727;
        snd[6] = 0x728;
        snd[7] = 0x729;
        if (lbl_1_bss_1D0[i * 5] > 3) {
            lbl_1_bss_1D0[i * 5] = 3;
        }
        if (lbl_1_bss_1D0[i * 5] > 0) {
            lbl_1_bss_1D0[i * 5 + 4] = HuAudFXPlay(snd[i]);
            lbl_1_bss_1D0[i * 5] -= 1;
        }
    }
    switch (lbl_1_bss_1A0) {
    case 1:
        if (WipeCheckEnd() == 0) {
            MgSeqModeNext();
        }
        break;
    case 2:
        if (frame == 0xA) {
            lbl_1_data_A0 = HuAudSStreamPlay(0x50);
        }
        break;
    case 5:
        if (pd[0][0x1C / 4] == 3 && pd[1][0x1C / 4] == 3 && pd[2][0x1C / 4] == 3) {
            lbl_1_data_9C = 0;
            MgSeqModeNext();
        }
        break;
    case 6:
        if (frame == 1) {
            if (lbl_1_bss_1D0[0x24 / 4] != -1) {
                HuAudFXStop(lbl_1_bss_1D0[0x24 / 4]);
            }
            if (lbl_1_data_A0 != -1) {
                HuAudSStreamFadeOut(lbl_1_data_A0, 0x64);
            }
        }
        if (pd[0][0x1C / 4] == 3 && pd[1][0x1C / 4] == 3 && pd[2][0x1C / 4] == 3) {
            lbl_1_data_9C = 0;
            MgSeqWinnerSet((s16) pd[0][0xC / 4], -1, -1, -1);
        } else {
            lbl_1_data_9C = 1;
            MgSeqWinnerSet((s16) pd[0][0xC / 4], (s16) pd[1][0xC / 4], (s16) pd[2][0xC / 4], -1);
        }
        break;
    case 7:
        if (frame == 0xA) {
            plNo = pd[lbl_1_data_9C][4 / 4];
            if (_CheckFlag(FLAG_MG_PRACTICE) == 0) {
                GwPlayer[plNo].mgCoin = 10;
            }
            if (lbl_1_data_9C == 1) {
                plNo = pd[1][4 / 4];
                if (_CheckFlag(FLAG_MG_PRACTICE) == 0) {
                    GwPlayer[plNo].mgCoin = 10;
                }
                plNo = pd[2][4 / 4];
                if (_CheckFlag(FLAG_MG_PRACTICE) == 0) {
                    GwPlayer[plNo].mgCoin = 10;
                }
            }
        }
        if (frame >= 0x46) {
            MgSeqModeNext();
        }
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

// NONMATCHING: the original allocates four live FPR pairs (f28..f31) for the fabs+frsp chain
void fn_1_698(MGPLAYER *player)
{
    HU3DMODEL *mdl;

    if (player->actor->colGroundAttr & 0x407F) {
        if ((u16) MgPlayerAttrCheck(player, 1) == 0) {
            if (HuPadBtnDown[player->padNo] & 0x100) {
                if (player->actionFlag & 0x2) {
                    MgPlayerModeSet(player, 1);
                }
            }
        }
    } else {
        if ((f32) fabs(player->actor->velY) > 2000.0f) {
            MgPlayerModeSet(player, 2);
        }
    }
    if (player->motNo != 0xB) {
        if (lbl_1_data_100 == 0) {
            CharMotionShiftSet(player->charNo, player->omObj->mtnId[11], 10.0f + (10.0f * lbl_1_bss_5C8[3].f), 12.0f, 0x40000002);
            player->motNo = 0xB;
        }
        lbl_1_data_100 = lbl_1_data_100 - (lbl_1_data_100 > 0);
    }
    if (player->motNo == 0xB) {
        mdl = &Hu3DData[lbl_1_bss_1B8[0]->mdlId[0]];
        if (mdl->motIdShift != -1) {
            mdl->motShiftWork.time = 10.0f + (10.0f * lbl_1_bss_5C8[3].f);
        } else {
            Hu3DMotionTimeSet(player->actor->mdlId, 10.0f + (10.0f * lbl_1_bss_5C8[3].f));
        }
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

// NONMATCHING: the original spills each player's data pointer to a dead stack slot in the loop
void fn_1_9E18(void)
{
    s32 i;
    void *dead;

    MgSeqModeNext();
    for (i = 0; i < 4; i++) {
        dead = lbl_1_bss_1B8[i]->data;
    }
    if (lbl_1_bss_198 >= 0) {
        GameMesKill(lbl_1_bss_198);
    }
    GameMesClose();
    HuAudAllStop();
    omOvlReturnEx((s16) lbl_1_data_A4, 1);
}

// NONMATCHING: frame size and register allocation differ (arg0 in r31, the old subMode in r30)
void fn_1_90C(MGPLAYER *player)
{
    s16 oldMode;

    oldMode = player->subMode;
    lbl_1_bss_158(player);
    if (oldMode != 2) {
        return;
    }
    if (player->subMode != 3) {
        return;
    }
    if (lbl_1_bss_5C8[5].i != 0) {
        return;
    }
    lbl_1_bss_5C8[5].i = 1;
    if (lbl_1_bss_5C8[6].i != 0) {
        return;
    }
    lbl_1_bss_5C8[6].i = 1;
    if (lbl_1_bss_1D0[0x64 / 4] < 3) {
        lbl_1_bss_1D0[0x68 / 4 + lbl_1_bss_1D0[0x64 / 4]] = 0;
        lbl_1_bss_1D0[0x64 / 4] += 1;
    }
}

void fn_1_9CE0(void)
{
    OMOBJ *obj;
    s32 i;

    obj = omAddObjEx(lbl_1_bss_19C, 0x64, 0x16, 0x2, 0x1, fn_1_7ED8);
    lbl_1_bss_1B4 = obj;
    for (i = 0; i < 4; i++) {
        obj = omAddObjEx(lbl_1_bss_19C, 0x66, 0x2, 0x3, 0x0, fn_1_8A98);
        lbl_1_bss_1B8[i] = obj;
        obj->data = HuMemDirectMallocNum(0, 0x54, HU_MEMNUM_OVL);
        obj->work[0] = i;
    }
    obj = omAddObjEx(lbl_1_bss_19C, 0x7FDA, 0x0, 0x0, -1, omOutView);
    lbl_1_bss_1C8 = obj;
    obj = omAddObjEx(lbl_1_bss_19C, 0x67, 0x0, 0x0, -1, fn_1_9860);
    lbl_1_bss_1CC = obj;
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
    mgr = MgActorObjectSetup();
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

    MgSeqCreate(&lbl_1_data_A8);
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

    ax = lbl_1_data_EC[0] * 0.03125f;
    ay = lbl_1_data_EC[1] * 0.03125f;
    az = lbl_1_data_EC[2] * 0.03125f;
    aw = lbl_1_data_EC[3] * 0.03125f;
    off = (f32) (lbl_1_bss_4 % (s32) lbl_1_data_EC[4]) / lbl_1_data_EC[4];
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
    GXSetCopyFilter(0, RenderMode->sample_pattern, 0, RenderMode->vfilter);
    GXSetTexCopySrc(0, 0, 0x280, 0x1E0);
    GXSetTexCopyDst(0x280, 0x1E0, GX_TF_RGBA8, 0);
    GXCopyTex(lbl_1_bss_168, 0);
    DCFlushRange(lbl_1_bss_168, (u32) lbl_1_bss_16C);
    GXPixModeSync();
    GXSetCopyFilter(RenderMode->aa, RenderMode->sample_pattern, 1, RenderMode->vfilter);
    GXInitTexObj(lbl_1_bss_118, lbl_1_bss_168, 0x280, 0x1E0, 6, GX_CLAMP, GX_CLAMP, 0);
    GXLoadTexObj(lbl_1_bss_118, GX_TEXMAP0);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_RGBA6, 0);
    GXInitTexObj(&lbl_1_bss_118[1], lbl_1_bss_160, 0x20, 0x20, 3, GX_REPEAT, GX_REPEAT, 0);
    GXLoadTexObj(&lbl_1_bss_118[1], GX_TEXMAP1);
    for (i = 0; i < 6; i++) {
        sp8[i] = lbl_1_rodata_49C[i];
    }
    sp8[1] *= ay;
    sp8[3] *= ax;
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x3C, 0, 0x7D);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetIndTexMtx(GX_ITM_0, (f32 (*)[3]) &sp8[0], 0);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, 1, 0, GX_ITM_0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(0.0f, 0.0f, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXTexCoord2f32(0.0f / aw, (0.0f / az) + off);
    GXPosition3f32(1.0f, 0.0f, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXTexCoord2f32(1.0f / aw, (0.0f / az) + off);
    GXPosition3f32(1.0f, 1.0f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXTexCoord2f32(1.0f / aw, (12.0f / az) + off);
    GXPosition3f32(0.0f, 1.0f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXTexCoord2f32(0.0f / aw, (12.0f / az) + off);
    GXSetNumIndStages(0);
    GXSetTevDirect(GX_TEVSTAGE0);
}
/*
 * Draws the particle sprites: transforms the four camera-facing corner anchors
 * through the view matrix, then emits one textured quad per active slot. The
 * slot's age (word 6) drives both the texture cell and the vertex alpha (a
 * ramp up, a hold, then a ramp down).
 */
// NONMATCHING: register allocation of the per-slot temporaries differs
void fn_1_6640(HU3DMODEL *modelP, f32 (*mtx)[3][4])
{
    Point3d pts[4];
    f32 m[12];
    s32 i;
    s32 cell;
    s32 alpha;
    f32 u0;
    f32 v0;
    f32 u1;
    f32 v1;
    s32 age;
    f32 *rec;

    for (i = 0; i < 12; i++) {
        m[i] = lbl_1_rodata_460[i];
    }
    C_MTXLookAt(lbl_1_bss_A8, &Hu3DCamera->pos, &Hu3DCamera->up, &Hu3DCamera->target);
    PSMTXTranspose(lbl_1_bss_A8, lbl_1_bss_98);
    PSMTXMultVecArray(lbl_1_bss_98, (Point3d *) &m[0], pts, 4);
    GXLoadPosMtxImm(*mtx, 0);
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
    HuSprTexLoad(espanim[esprite[lbl_1_bss_17C].animNo].anim, 0, 0, GX_CLAMP, GX_CLAMP, GX_TRUE);
    for (i = 0; i < 0x1E; i++) {
        age = lbl_1_bss_280[i * 7 + 6];
        if (age == 0) {
            continue;
        }
        if (age < 3) {
            cell = 0;
        } else if (age < 6) {
            cell = 1;
        } else if (age < 9) {
            cell = 2;
        } else if (age < 12) {
            cell = 3;
        } else if (age < 15) {
            cell = 4;
        } else if (age < 19) {
            cell = 5;
        } else if (age < 23) {
            cell = 6;
        } else if (age < 27) {
            cell = 7;
        } else {
            cell = 8;
        }
        if (cell > 8) {
            cell = 8;
        }
        u0 = (f32) (cell % 5) / 5.0f;
        v0 = (f32) (cell / 5) * 0.5f;
        u1 = 0.2f + u0;
        v1 = 0.5f + v0;
        u0 = u0 + 0.003125f;
        v0 = v0 + 0.0078125f;
        u1 = u1 - 0.003125f;
        v1 = v1 - 0.0078125f;
        alpha = 0xF0;
        if (age < 4) {
            alpha = (alpha * age) / 4;
        } else if (age < 0x1B) {
            alpha = alpha;
        } else if (age < 0x20) {
            alpha = alpha - ((alpha * (age - 0x1B)) / 5);
        } else {
            alpha = 0;
        }
        rec = (f32 *) &lbl_1_bss_280[i * 7];
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(pts[0].x + rec[0], pts[0].y + rec[1], pts[0].z + rec[2]);
        GXColor4u8(0xFF, 0xFF, 0xFF, (u8) alpha);
        GXTexCoord2f32(u0, v0);
        GXPosition3f32(pts[1].x + rec[0], pts[1].y + rec[1], pts[1].z + rec[2]);
        GXColor4u8(0xFF, 0xFF, 0xFF, (u8) alpha);
        GXTexCoord2f32(u1, v0);
        GXPosition3f32(pts[2].x + rec[0], pts[2].y + rec[1], pts[2].z + rec[2]);
        GXColor4u8(0xFF, 0xFF, 0xFF, (u8) alpha);
        GXTexCoord2f32(u1, v1);
        GXPosition3f32(pts[3].x + rec[0], pts[3].y + rec[1], pts[3].z + rec[2]);
        GXColor4u8(0xFF, 0xFF, 0xFF, (u8) alpha);
        GXTexCoord2f32(u0, v1);
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
    s16 sp8[3];
    COL_ATTRPARAM sp20;
    u32 sp34[5];
    s32 i;
    s32 idx;
    f32 maxTime;

    for (i = 0; i < 5; i++) {
        sp34[i] = lbl_1_rodata_4D0[i];
    }
    idx = 0;
    arg0->mdlId[idx] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E0000, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[idx], 1);
    Hu3DModelShadowMapSet(arg0->mdlId[idx]);
    sp8[0] = arg0->mdlId[idx];
    idx = 5;
    arg0->mdlId[idx] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E0005, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(arg0->mdlId[idx], 1);
    sp8[1] = arg0->mdlId[idx];
    idx = 8;
    arg0->mdlId[idx] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E000C, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelPosSet(arg0->mdlId[idx], 0.0f, 450.0f, 0.0f);
    Hu3DModelAttrSet(arg0->mdlId[idx], 1);
    sp8[2] = arg0->mdlId[idx];
    MgActorColMapInit(sp8, 3, 0x80);
    idx = 1;
    arg0->mdlId[idx] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E0001, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[idx], 1);
    Hu3DModelAttrSet(arg0->mdlId[idx], 0x40000001);
    Hu3DMotionSpeedSet(arg0->mdlId[idx], 0.25f);
    idx = 2;
    arg0->mdlId[idx] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E0002, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[idx], 1);
    Hu3DModelAttrSet(arg0->mdlId[idx], 0x40000001);
    idx = 3;
    arg0->mdlId[idx] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E0003, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[idx], 1);
    Hu3DModelAttrSet(arg0->mdlId[idx], 0x40000001);
    idx = 4;
    arg0->mdlId[idx] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E0004, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[idx], 1);
    Hu3DModelAttrSet(arg0->mdlId[idx], 0x40000001);
    idx = 0xE;
    arg0->mdlId[idx] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E0006, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[idx], 2);
    Hu3DModelAttrSet(arg0->mdlId[idx], 0x40000001);
    idx = 0xF;
    arg0->mdlId[idx] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E0007, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[idx], 2);
    Hu3DModelAttrSet(arg0->mdlId[idx], 0x40000001);
    idx = 0x10;
    arg0->mdlId[idx] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E0008, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[idx], 2);
    Hu3DModelAttrSet(arg0->mdlId[idx], 0x40000001);
    idx = 0x11;
    arg0->mdlId[idx] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E0009, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[idx], 2);
    Hu3DModelAttrSet(arg0->mdlId[idx], 0x40000001);
    idx = 7;
    arg0->mdlId[idx] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E000A, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[idx], 1);
    Hu3DModelAttrSet(arg0->mdlId[idx], 0x40000002);
    Hu3DModelPosSet(arg0->mdlId[idx], 0.0f, 450.0f, 0.0f);
    Hu3DModelShadowMapSet(arg0->mdlId[idx]);
    idx = 6;
    arg0->mdlId[idx] = Hu3DModelCreate(HuDataSelHeapReadNum(0x4E000B, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(arg0->mdlId[idx], 1);
    Hu3DModelAttrSet(arg0->mdlId[idx], 0x40000001);
    Hu3DModelPosSet(arg0->mdlId[idx], 0.0f, 450.0f, 0.0f);
    for (i = 0; i < 5; i++) {
        idx = i + 9;
        arg0->mdlId[idx] = Hu3DModelCreate(HuDataSelHeapReadNum(sp34[i], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelLayerSet(arg0->mdlId[idx], 1);
        if (i < 2) {
            Hu3DModelAttrSet(arg0->mdlId[idx], 1);
            Hu3DMotionTimeSet(arg0->mdlId[idx], 540.0f);
        }
        if (i == 2) {
            Hu3DModelAttrSet(arg0->mdlId[idx], 0x40000001);
        }
    }
    idx = 0x13;
    arg0->mdlId[idx] = Hu3DHookFuncCreate(fn_1_6640);
    Hu3DModelLayerSet(arg0->mdlId[idx], 4);
    idx = 0x14;
    arg0->mdlId[idx] = Hu3DHookFuncCreate(fn_1_7014);
    Hu3DModelLayerSet(arg0->mdlId[idx], 5);
    idx = 0x15;
    arg0->mdlId[idx] = Hu3DHookFuncCreate(fn_1_7830);
    Hu3DModelLayerSet(arg0->mdlId[idx], 0);
    lbl_1_bss_17C = (s16) espEntry(0x4E0012, 0, 0);
    espDispOff((s16) lbl_1_bss_17C);
    memset(lbl_1_bss_5C8, 0, 0x200);
    ((f32 *) lbl_1_bss_5C8)[0] = -50.0f;
    lbl_1_bss_5C8[4].i = 0;
    ((f32 *) lbl_1_bss_5C8)[2] = 0.0f;
    Hu3DModelRotSet(((OMOBJ *) lbl_1_bss_1B4)->mdlId[6], 0.0f, ((f32 *) lbl_1_bss_5C8)[2], 0.0f);
    Hu3DModelRotSet(((OMOBJ *) lbl_1_bss_1B4)->mdlId[8], 0.0f, ((f32 *) lbl_1_bss_5C8)[2], 0.0f);
    memset(lbl_1_bss_280, 0, 0x348);
    for (i = 0; i < 2; i++) {
        lbl_1_bss_270[i * 2] = 1;
        maxTime = Hu3DMotionMaxTimeGet(arg0->mdlId[i + 0xE]);
        lbl_1_bss_270[i * 2 + 1] = (s32) (((f32) rand8() / 255.0f) * maxTime);
        Hu3DMotionTimeSet(arg0->mdlId[i + 0xE], (f32) lbl_1_bss_270[i * 2 + 1]);
        Hu3DMotionTimeSet(arg0->mdlId[i + 0x10], (f32) lbl_1_bss_270[i * 2 + 1]);
        Hu3DModelAttrSet(arg0->mdlId[i + 0xE], 1);
        Hu3DModelAttrSet(arg0->mdlId[i + 0x10], 1);
    }
    MgActorColAttrParamGet(&sp20, 1);
    sp20.yDeviate = 0.0f;
    MgActorColAttrParamSet(&sp20, 1);
    MgActorColAttrParamGet(&sp20, 2);
    sp20.yDeviate = 0.0f;
    MgActorColAttrParamSet(&sp20, 2);
    memset(lbl_1_bss_1D0, 0, 0xA0);
    for (i = 0; i < 8; i++) {
        lbl_1_bss_1D0[i * 5 + 4] = -1;
    }
    arg0->objFunc = fn_1_43BC;
}
/*
 * Update: drives the camera interpolation across the sequence modes, the
 * two-phase animation cycle, and the 30-slot particle field. lbl_1_bss_5C8 is
 * a state block: f[0] is the lift height, f[2] the azimuth, i[4] a frame
 * counter, i[5]/i[6] the cycle counters the animation and particle bursts key off.
 */
// NONMATCHING: float temporaries and register allocation differ from the original
void fn_1_43BC(OMOBJ *arg0)
{
    static f32 s_lift;
    s32 *pd[4];
    COL_ATTRPARAM sp44;
    f32 sp80[10];
    f32 sp58[10];
    f32 lift;
    f32 t;
    f32 k;
    f32 kk;
    f32 mm;
    f32 ang;
    f32 tt;
    s32 i;
    s32 j;
    s32 slot;
    s32 az;
    s32 cnt;
    s32 *rec;
    M520STATE *w = lbl_1_bss_5C8;

    for (i = 0; i < 4; i++) {
        pd[i] = (s32 *) lbl_1_bss_1B8[i]->data;
    }
    lbl_1_bss_1A0 = MgSeqModeGet();
    lbl_1_bss_0 = MgSeqFrameNoGet();
    lbl_1_bss_4 += 1;
    if (lbl_1_bss_164 == 0) {
        Hu3DModelAttrSet(lbl_1_bss_1B4->mdlId[0x15], 1);
    }
    if ((lbl_1_bss_1A0 == 2 && lbl_1_bss_0 >= 0x82) || lbl_1_bss_1A0 == 3) {
        lbl_1_bss_14 += 1;
        t = (f32) lbl_1_bss_14;
        if (t < 16.0f) {
            k = ((((1.0f + t) * 0.5f) * t) * 0.0625f) / 48.0f;
        } else if (t < 48.0f) {
            k = (8.5f / 48.0f) + ((t - 16.0f) / 48.0f);
        } else if (t < 63.0f) {
            k = 1.0f - (((((1.0f + ((64.0f - t) - 1.0f)) * 0.5f) * ((63.0f - t) - 1.0f)) / 15.0f) / 48.0f);
        } else {
            k = 1.0f;
        }
        kk = 1.0f - k;
        CZoom = (1425.0f * kk) + (4575.0f * k);
        Center.y = (505.0f * kk) + (-120.0f * k);
        CRot.x = (-35.0f * kk) + (-70.0f * k);
    }
    if (lbl_1_bss_1A0 == 3 && lbl_1_bss_0 > 0x2F) {
        mm = (f32) (0x3E - lbl_1_bss_0);
        if (mm >= 0.0f) {
            s32 sgn = ((s32) mm & 1) ? -1 : 1;

            mm = mm * ((f32) sgn / 15.0f);
            Center.y = (f32) (-120.0 + ((6.0f * mm) * cos(1.2217304763960306)));
            Center.z = (f32) ((6.0f * mm) * -sin(1.2217304763960306));
        }
    }
    if (lbl_1_bss_1A0 == 8 && lbl_1_data_9C == 0) {
        f32 a = 0.9f;
        f32 e = 1.0f - a;

        for (i = 0; i < 10; i++) {
            sp80[i] = lbl_1_rodata_344[i];
            sp58[i] = lbl_1_rodata_36C[i];
        }
        CZoom = CZoom * a;
        CZoom = CZoom + (900.0f * e);
        Center.y = Center.y * a;
        Center.y = Center.y + (e * (sp80[pd[0][2]] - 140.0f));
        CRot.x = CRot.x * a;
        CRot.x = CRot.x + (e * sp58[pd[0][2]]);
    }
    if (lbl_1_bss_1A0 == 8 && lbl_1_data_9C == 1) {
        f32 a = 0.9f;
        f32 e = 1.0f - a;

        CZoom = CZoom * a;
        CZoom = CZoom + (3825.0f * e);
        Center.y = Center.y * a;
        Center.y = Center.y + (-95.0f * e);
        CRot.x = CRot.x * a;
        CRot.x = CRot.x + (-51.0f * e);
    }
    MgActorColAttrParamGet(&sp44, 2);
    if (lbl_1_bss_1A0 == 5) {
        sp44.yDeviate = 0.6666667f;
    } else {
        sp44.yDeviate = 0.0f;
    }
    MgActorColAttrParamSet(&sp44, 2);
    w[4].i += 1;
    w[6].i = w[6].i + (w[6].i > 0);
    if (w[6].i >= 0x1E0 && lbl_1_bss_1A0 == 5 && lbl_1_bss_1D0[0x50 / 4] < 3) {
        lbl_1_bss_1D0[0x54 / 4 + lbl_1_bss_1D0[0x50 / 4]] = 0;
        lbl_1_bss_1D0[0x50 / 4] += 1;
    }
    if (w[6].i == 2 && lbl_1_bss_1D0[0x60 / 4] != -1) {
        HuAudFXStop(lbl_1_bss_1D0[0x60 / 4]);
    }
    w[6].i = (w[6].i >= 0x1E0) ? 0 : w[6].i;
    w[5].i = w[5].i + (w[5].i > 0);
    w[5].i = (w[5].i >= 0x78) ? 0 : w[5].i;
    if (lbl_1_bss_1A0 > 5) {
        w[0].f = w[0].f - 1.5f;
        w[0].f = (w[0].f < -190.0f) ? -190.0f : w[0].f;
        lift = s_lift; /* the original leaves f31 unset on this path: keep the last value */
    } else {
        w[0].f = (w[5].i > 0x78) ? 0.0f : ((f32) w[5].i / 120.0f);
        az = (w[5].i == w[6].i) ? -0x4B : -0x3C;
        w[0].f = (f32) (-50.0 + ((f64) az * sin((3.141592653589793 * (180.0 * ((0.5 * w[0].f) + (0.5 * (1.0f - ((1.0f - w[0].f) * (1.0f - w[0].f))))))) / 180.0)));
        kk = (w[5].i == w[6].i) ? 1.0f : 0.6666667f;
        lift = 135.0f - ((w[0].f - -50.0f) * kk);
        if (w[6].i <= 0x50) {
            tt = 0.0f;
        } else if (w[6].i <= 0x78) {
            tt = (f32) (w[6].i - 0x50) / 40.0f;
        } else if (w[6].i <= 0xF0) {
            tt = 1.0f;
        } else if (w[6].i <= 0x1E0) {
            tt = 1.0f - ((f32) (w[6].i - 0xF0) / 240.0f);
        } else {
            tt = 0.0f;
        }
        lift = lift + (-90.0f * tt);
        lift = lift + 45.0f;
        if (lift > 180.0f) {
            lift = 180.0f;
        }
        s_lift = lift;
    }
    Hu3DModelPosSet(lbl_1_bss_1B4->mdlId[6], 0.0f, 500.0f + w[0].f, 0.0f);
    Hu3DModelPosSet(lbl_1_bss_1B4->mdlId[8], 0.0f, 500.0f + w[0].f, 0.0f);
    Hu3DModelPosSet(lbl_1_bss_1B4->mdlId[7], 0.0f, 500.0f + w[0].f, 0.0f);
    for (i = 0; i < 3; i++) {
        Hu3DModelAttrSet(lbl_1_bss_1B4->mdlId[i + 0xB], 1);
        Hu3DModelPosSet(lbl_1_bss_1B4->mdlId[i + 0xB], 0.0f, lift, 0.0f);
    }
    if (w[6].i == 0) {
        Hu3DModelAttrReset(lbl_1_bss_1B4->mdlId[0xB], 1);
    } else if (w[6].i <= 0xB4) {
        Hu3DMotionTimeSet(lbl_1_bss_1B4->mdlId[0xC], (f32) w[6].i);
        Hu3DModelAttrReset(lbl_1_bss_1B4->mdlId[0xC], 1);
    } else if (w[6].i <= 0x1E0) {
        Hu3DMotionTimeSet(lbl_1_bss_1B4->mdlId[0xB], 0.0f);
        Hu3DMotionTimeSet(lbl_1_bss_1B4->mdlId[0xD], (f32) (w[6].i - 0xB4));
        Hu3DModelAttrReset(lbl_1_bss_1B4->mdlId[0xD], 1);
    }
    if (w[6].i == 0x1E && lbl_1_bss_1D0[0] < 3) {
        lbl_1_bss_1D0[1 + lbl_1_bss_1D0[0]] = 0;
        lbl_1_bss_1D0[0] += 1;
    }
    if (w[6].i == 5) {
        Hu3DModelAttrReset(lbl_1_bss_1B4->mdlId[0xA], 1);
        Hu3DMotionTimeSet(lbl_1_bss_1B4->mdlId[0xA], 0.0f);
    }
    if (w[6].i == 0x55) {
        Hu3DModelAttrSet(lbl_1_bss_1B4->mdlId[0xA], 1);
    }
    if (w[6].i == 0x1E) {
        Hu3DModelAttrReset(lbl_1_bss_1B4->mdlId[9], 1);
        Hu3DMotionTimeSet(lbl_1_bss_1B4->mdlId[9], 0.0f);
    }
    if (w[6].i == 0xD2) {
        Hu3DModelAttrSet(lbl_1_bss_1B4->mdlId[9], 1);
    }
    if (lbl_1_bss_1A0 > 5) {
        HU3DMODEL *mdl = &Hu3DData[lbl_1_bss_1B4->mdlId[9]];

        mdl->pos.y = mdl->pos.y - 20.0f;
        Hu3DModelAttrSet(arg0->mdlId[lbl_1_bss_1B4->mdlId[9]], 0x40000002);
    }
    if (lbl_1_bss_1A0 == 3 && lbl_1_bss_0 == 0x2E) {
        if (lbl_1_bss_1D0[0x14 / 4] < 3) {
            lbl_1_bss_1D0[0x18 / 4 + lbl_1_bss_1D0[0x14 / 4]] = 0;
            lbl_1_bss_1D0[0x14 / 4] += 1;
        }
        if (lbl_1_bss_1D0[0x78 / 4] < 3) {
            lbl_1_bss_1D0[0x7C / 4 + lbl_1_bss_1D0[0x78 / 4]] = 0;
            lbl_1_bss_1D0[0x78 / 4] += 1;
        }
    }
    if (lbl_1_bss_1A0 == 5 || (lbl_1_bss_1A0 == 3 && lbl_1_bss_0 > 0x2D)) {
        s32 ph = w[4].i % 32;

        if (ph == 1 || ph == 7 || ph == 0xB || ph == 0x11 || ph == 0x15 || ph == 0x1B) {
            for (i = 0; i < 3; i++) {
                ang = w[2].f + (f32) ((i * 0x168) / 3);
                for (slot = 0; slot < 0x1E; slot++) {
                    if (lbl_1_bss_280[slot * 7 + 6] == 0) {
                        break;
                    }
                }
                if (slot == 0x1E) {
                    break;
                }
                rec = &lbl_1_bss_280[slot * 7];
                rec[6] = 1;
                ((f32 *) rec)[0] = (f32) (175.0 * sin((3.141592653589793 * ang) / 180.0));
                ((f32 *) rec)[1] = 400.0f + w[0].f;
                ((f32 *) rec)[2] = (f32) (200.0 * cos((3.141592653589793 * ang) / 180.0));
                ((f32 *) rec)[3] = (f32) ((575.0 * sin((3.141592653589793 * ang) / 180.0)) * 0.03125);
                ((f32 *) rec)[4] = -4.285714f;
                ((f32 *) rec)[5] = (f32) ((575.0 * cos((3.141592653589793 * ang) / 180.0)) * 0.03125);
            }
        }
    }
    for (i = 0; i < 0x1E; i++) {
        rec = &lbl_1_bss_280[i * 7];
        if (rec[6] != 0) {
            rec[6] += 1;
            ((f32 *) rec)[0] = ((f32 *) rec)[0] + ((f32 *) rec)[3];
            ((f32 *) rec)[1] = ((f32 *) rec)[1] + ((f32 *) rec)[4];
            ((f32 *) rec)[2] = ((f32 *) rec)[2] + ((f32 *) rec)[5];
            if (rec[6] == 0x21) {
                rec[6] = 0;
            }
        }
    }
    for (i = 0; i < 2; i++) {
        s32 *r = &lbl_1_bss_270[i * 2];

        if (r[0] == 0) {
            r[1] -= 1;
            if (r[1] > 0) {
                continue;
            }
            r[0] = 1;
            r[1] = 0;
            Hu3DMotionTimeSet(arg0->mdlId[i + 0xE], 0.0f);
            Hu3DModelAttrReset(arg0->mdlId[i + 0xE], 1);
            Hu3DMotionTimeSet(arg0->mdlId[i + 0x10], 0.0f);
            Hu3DModelAttrReset(arg0->mdlId[i + 0x10], 1);
            j = (i == 0) ? 3 : 2;
            cnt = lbl_1_bss_1D0[j * 5];
            if (cnt < 3) {
                lbl_1_bss_1D0[j * 5 + 1 + cnt] = 0;
                lbl_1_bss_1D0[j * 5] += 1;
            }
        } else {
            r[1] += 1;
            if ((f32) r[1] >= Hu3DMotionMaxTimeGet(arg0->mdlId[i + 0xE])) {
                r[0] = 0;
                r[1] = (s32) (60.0f * ((f32) rand8() / 255.0f));
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
 * Loads a character data file: looks the directory half of dataNum up in the
 * charDataDirTbl columns [4] and [5] and reads it through the ARAM cache, the
 * short-heap path or the plain heap read. fn_1_8A98 carries this inlined.
 */
void *fn_1_8918(s16 charNo, u32 dataNum)
{
    s16 i;
    u32 hi;
    void *res;

    hi = dataNum & 0xFFFF0000;
    for (i = 0; (s16) i < 0xD; i++) {
        if (hi == charDataDirTbl[i][4]) {
            break;
        }
    }
    if ((s16) i == 0xD && hi != 0) {
        for (i = 0; (s16) i < 0xD; i++) {
            if (hi == charDataDirTbl[i][5]) {
                break;
            }
        }
        if ((s16) i != 0xD) {
            dataNum = (dataNum & 0xFFFF) | charDataDirTbl[charNo][5];
            res = HuDataReadNumHeapShortForce(dataNum, HU_MEMNUM_OVL, HEAP_MODEL);
        } else {
            res = HuDataSelHeapReadNum(dataNum, HU_MEMNUM_OVL, HEAP_MODEL);
        }
    } else {
        dataNum = (dataNum & 0xFFFF) | charDataDirTbl[charNo][4];
        res = HuAR_ARAMtoMRAMFileRead(dataNum, HU_MEMNUM_OVL, HEAP_MODEL);
        if (res == NULL) {
            res = HuDataSelHeapReadNum(dataNum, HU_MEMNUM_OVL, HEAP_MODEL);
        }
    }
    return res;
}

/*
 * Player Create: resolves the slot to a GwPlayerConf entry (grouping players of
 * the same colour together), creates the Mg actor, loads the character model
 * and its joint motions, then wires the per-slot callbacks. The ARAM lookups
 * fall back to a heap read when the MRAM path is not resident.
 */
// NONMATCHING: the character-data loads are calls to fn_1_8918 here (inlined in the original); MgActorPosUpdate gets the actor, the original passed the player
void fn_1_8A98(OMOBJ *arg0)
{
    s32 sp5C[4];
    MGACTOR_PARAM sp88;
    HuVecF sp50;
    MGACTOR_COLMAP_POLY sp6C;
    M520PLAYER *pl;
    MGPLAYER *player;
    MGACTOR *actor;
    s32 n;
    s32 i;
    s32 j;
    s32 ang;

    for (i = 0; i < 4; i++) {
        sp5C[i] = lbl_1_rodata_4F0[i];
    }
    n = 0;
    pl = (M520PLAYER *) arg0->data;
    memset(pl, 0, 0x54);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (i == GwPlayerConf[j].grp) {
                sp5C[n] = j;
                n += 1;
            }
        }
    }
    pl->idx = arg0->work[0];
    pl->conf = sp5C[pl->idx];
    if (pl->conf == 4) {
        pl->conf = pl->idx;
    }
    pl->charFull = GwPlayerConf[pl->conf].charNo;
    pl->charNo = pl->charFull >= 9 ? 9 : pl->charFull;
    pl->padNo = GwPlayerConf[pl->conf].type != 0 ? -1 : GwPlayerConf[pl->conf].padNo;
    pl->dif = pl->padNo != -1 ? 0 : GwPlayerConf[pl->conf].dif;
    pl->unk1C = 2;
    pl->unk34 = 1.0f;
    sp88.height = 150.0f;
    sp88.radius = 40.0f;
    sp88.param = pl->idx;
    sp88.type = 0;
    sp88.attr = 0;
    sp88.correctHookParam = 0;
    sp88.narrowHook = NULL;
    sp88.correctHook = NULL;
    lbl_1_bss_1A4[pl->idx] = MgPlayerCreate((s16) pl->conf, &sp88, 4, 1, 0x13, pl->idx == -1 ? lbl_1_data_138 : NULL);
    player = lbl_1_bss_1A4[pl->idx];
    actor = player->actor;
    arg0->mdlId[0] = (s16) actor->mdlId;
    CharEffectLayerSet(3);
    if (pl->idx == 0) {
        actor->pos.x = 0.0f;
        actor->pos.y = 600.0f;
        actor->pos.z = 0.0f;
    } else {
        ang = (pl->idx * 0x78) + 0x3C;
        actor->pos.x = (f32) ((0 + 0x21C) * sin((3.141592653589793 * ang) / 180.0));
        actor->pos.y = 400.0f;
        actor->pos.z = (f32) (((0 + 0x21C) * cos((3.141592653589793 * ang) / 180.0)) - 25.0);
    }
    MgPlayerVibrateCreate(player);
    sp50.x = actor->pos.x;
    sp50.y = actor->pos.y;
    sp50.z = actor->pos.z;
    sp50.y = -500.0f;
    if (MgActorColMapPolyGet(&actor->pos, &sp50, 0xFFFF, &sp6C) != 0) {
        sp6C.pos.y = sp6C.pos.y + 75.0f;
        MgActorPosUpdate(actor, &sp6C.pos);
        actor->pos.y = sp6C.pos.y;
    }
    Hu3DModelLayerSet(arg0->mdlId[0], 1);
    Hu3DModelShadowSet(arg0->mdlId[0]);
    Hu3DModelAmbSet(arg0->mdlId[0], 1.0f, 0.5f, 0.5f);
    if (pl->idx == 0) {
        arg0->mdlId[1] = Hu3DModelCreate(HuDataSelHeapReadNum(charDataDirTbl[pl->charFull][0], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelLayerSet(arg0->mdlId[1], 1);
        Hu3DModelShadowSet(arg0->mdlId[1]);
        Hu3DModelAmbSet(arg0->mdlId[1], 1.0f, 0.5f, 0.5f);
    }
    player->modeFunc[10] = (MGPLAYER_MODE_FUNC) fn_1_A08;
    if (pl->idx == 0) {
        arg0->mtnId[0] = Hu3DJointMotion(arg0->mdlId[1], fn_1_8918((s16) pl->charFull, 0x8C002F));
        arg0->mtnId[1] = Hu3DJointMotion(arg0->mdlId[1], fn_1_8918((s16) pl->charFull, 0x870075));
        arg0->mtnId[2] = Hu3DJointMotion(arg0->mdlId[1], fn_1_8918((s16) pl->charFull, 0x8C002F));
        Hu3DMotionSet(arg0->mdlId[1], arg0->mtnId[0]);
    } else {
        player->omObj->mtnId[10] = CharMotionCreate((s16) pl->charFull, 0x8C0006);
    }
    player->omObj->mtnId[11] = CharMotionCreate((s16) pl->charFull, pl->idx == 0 ? 0x870075 : 0x8C0015);
    if (pl->idx == 0) {
        player->omObj->mtnId[12] = CharMotionCreate((s16) pl->charFull, 0x8C0031);
        lbl_1_bss_15C = player->modeFunc[0];
        player->modeFunc[0] = (MGPLAYER_MODE_FUNC) fn_1_698;
        lbl_1_bss_158 = player->modeFunc[5];
        player->modeFunc[5] = (MGPLAYER_MODE_FUNC) fn_1_90C;
        player->modeFunc[12] = (MGPLAYER_MODE_FUNC) fn_1_B04;
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
/* 3 rows x 10 columns of z offsets, indexed [state][char] */
static const s32 m520_r1FC[30] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -15, -30, 0, -15, -30, -10, 0, -20, 0, 0, 0, -30, 0, 0, 0, 0, 0, 0 };
/* MWCC inlines sqrtf as three frsqrte Newton steps (and returns the input for <= 0) */
static f32 M520Sqrt(f32 v)
{
    f64 r;

    if (v > 0.0f) {
        r = __frsqrte(v);
        r = 0.5 * r * (3.0 - (v * (r * r)));
        r = 0.5 * r * (3.0 - (v * (r * r)));
        r = 0.5 * r * (3.0 - (v * (r * r)));
        return (f32) (v * r);
    }
    return v;
}
void MgPlayerPauseOn(MGPLAYER *playerP);
#ifdef TARGET_PC
/* a COM player's padNo can be -1: the original indexes the pad arrays with it anyway (reads the byte before the array); the port must not */
#define M520_PADOK(n) ((n) >= 0 && (n) < 4)
#else
#define M520_PADOK(n) 1
#endif
static const s32 m520_r6C[8] = { 30, 10, 10, 0, 30, 20, 10, 5 };
static const s32 m520_r8C[4] = { 20, 50, 70, 90 };
static const s32 m520_r9C[4] = { 0, 10, 30, 50 };
static const s32 m520_rAC[8] = { 120, 60, 20, 20, 60, 30, 90, 60 };
static const s32 m520_rCC[8] = { 15, 15, 10, 7, 20, 20, 15, 10 };
static const s32 m520_rEC[8] = { 23, 18, 12, 10, 15, 5, 4, 4 };
static const s32 m520_r10C[5] = { 0, 2, 6, 8, 10 };
static const s32 m520_r120[5] = { 0, 0, 0, 5, 10 };
static const s32 m520_r134[10] = { 0, 0, 1, 2, 7, 0, 0, 1, 2, 10 };
static const f32 m520_r15C[20] = { 40.f, 40.f, 74.f, 75.f, 108.f, 161.f, 186.f, 229.f,
                                   278.f, 333.f, 374.f, 431.f, 470.f, 521.f, 565.f, 613.f,
                                   643.f, 700.f, 700.f, 700.f };
static const f32 m520_r1AC[20] = { 104.f, 120.f, 126.f, 155.f, 188.f, 227.f, 266.f, 309.f,
                                   358.f, 413.f, 462.f, 511.f, 550.f, 601.f, 645.f, 697.f,
                                   750.f, 770.f, 800.f, 800.f };


/*
 * Per-player update. Slot 0 is the human leader and drives the ring: it reads
 * the pad, runs the 5-state AI that picks a direction, and integrates the ring
 * azimuth and speed. The other slots are AI-driven: they pick a target angle
 * on the ring and steer the stick toward it.
 */
// NONMATCHING: float temporaries, the inlined sqrt and register allocation differ from the original
void fn_1_B70(OMOBJ *arg0)
{
    M520PLAYER *pl;
    MGPLAYER *mp;
    HuVecF pos;
    HuVecF opos[3];
    f32 angTbl[3];
    f32 nearTbl[3];
    f32 lenTbl[3];
    s32 padOk;
    s32 btnMask;
    s32 btnDownMask;
    s32 stkX;
    s32 stkY;
    s32 btn;
    s32 btnDown;
    s32 i;
    s32 j;
    s32 best;
    f32 bestScore;
    f32 cur;
    f32 target;
    f32 d;
    f32 score;
    f32 len;
    f32 w3old;
    f32 step;
    s32 sgn;
    s32 oldState;
    s32 airborne;
    f64 val;
    s32 sp48;
    HU3DMODEL *mdl;

    pl = (M520PLAYER *) arg0->data;
    mp = lbl_1_bss_1A4[pl->idx];
    stkX = 0;
    stkY = 0;
    btn = 0;
    btnDown = 0;
    MgActorPosGet(mp->actor, &pos);
    if (pl->idx <= 0 || pl->unk1C != 3) {
        arg0->trans.x = pos.x;
        arg0->trans.y = pos.y;
        arg0->trans.z = pos.z;
        arg0->rot.y = mp->actor->rotY;
    }
    lbl_1_bss_1A0 = MgSeqModeGet();
    lbl_1_bss_0 = MgSeqFrameNoGet();
    padOk = M520_PADOK(mp->padNo);
    btnMask = padOk ? (HuPadBtn[mp->padNo] & 0xFEFF) : 0;
    btnDownMask = padOk ? (HuPadBtnDown[mp->padNo] & 0xFEFF) : 0;
    if (lbl_1_bss_1A0 != 5) {
        MgPlayerPadSet(mp, 0, 0, 0, 0);
        if (padOk) {
            HuPadTrigR[mp->padNo] = 0;
            HuPadTrigL[mp->padNo] = 0;
        }
    } else if (pl->padNo != -1) {
        stkX = HuPadStkX[pl->padNo];
        stkY = HuPadStkY[pl->padNo];
        btn = HuPadBtn[pl->padNo];
        btnDown = HuPadBtnDown[pl->padNo];
    }
    if (pl->idx == 0) {
        mdl = &Hu3DData[arg0->mdlId[0]];
        target = 0.0f;
        Hu3DModelAttrReset(arg0->mdlId[0], 1);
        Hu3DModelAttrSet(arg0->mdlId[1], 1);
        if (lbl_1_bss_1A0 == 2) {
            if (lbl_1_bss_0 == 0x44) {
                btnDown = 0x100;
                btn = 0x100;
            } else if (lbl_1_bss_0 > 0x44 && lbl_1_bss_0 <= 0x47) {
                btn = 0x100;
            } else if (lbl_1_bss_0 == 0x4E) {
                btnDown = 0x100;
                btn = 0x100;
            }
        } else if (lbl_1_bss_1A0 == 5 && pl->padNo != -1) {
            target = (f32) (((HuPadBtn[pl->padNo] & 0x20) != 0 ? 0x60 : 0) - ((HuPadBtn[pl->padNo] & 0x40) != 0 ? 0x60 : 0)) / 96.0f;
        } else if (lbl_1_bss_1A0 == 5) {
            best = -1;
            bestScore = -1.0f;
            cur = lbl_1_bss_5C8[2].f;
            for (i = 0; i < 3; i++) {
                MgActorPosGet(lbl_1_bss_1A4[i + 1]->actor, &opos[i]);
                angTbl[i] = (f32) (180.0 * (atan2(opos[i].x, opos[i].z) / 3.141592653589793));
                nearTbl[i] = 360.0f;
                lenTbl[i] = M520Sqrt((opos[i].x * opos[i].x) + (opos[i].z * opos[i].z));
                for (j = 0; j < 3; j++) {
                    d = angTbl[i] - (cur + (f32) (j * 0x78));
                    d = d + (f32) ((d < -180.0f) ? 180 : ((d > 180.0f) ? -180 : 0));
                    d = d + (f32) ((d < -180.0f) ? 180 : ((d > 180.0f) ? -180 : 0));
                    if (fabs(d) < fabs(nearTbl[i])) {
                        nearTbl[i] = d;
                    }
                }
                len = (lenTbl[i] < 300.0f) ? 0.0f : ((lenTbl[i] - 300.0f) / 400.0f);
                score = (f32) ((60.0 - fabs(nearTbl[i])) + (10.0f * len));
                if (best == -1 || bestScore < score) {
                    best = i;
                    bestScore = score;
                }
            }
            switch (pl->unk38) {
            case 0:
                target = lbl_1_bss_5C8[3].f;
                if (pl->unk3C == 0) {
                    pl->unk40 = m520_r6C[pl->dif] + (rand8() % m520_r6C[pl->dif + 4]);
                }
                pl->unk3C += 1;
                if (pl->unk3C < pl->unk40) {
                    break;
                }
                if (lbl_1_bss_5C8[6].i == 0) {
                    if ((f32) m520_r8C[pl->dif] > (100.0f * ((f32) rand8() / 255.0f))) {
                        pl->unk38 = 3;
                        pl->unk3C = 0;
                        break;
                    }
                }
                pl->unk40 = m520_rAC[pl->dif] + (rand8() % m520_rAC[pl->dif + 4]);
                if (pl->dif == 3) {
                    pl->unk38 = ((lbl_1_bss_5C8[3].f * nearTbl[best]) < 0.0f) ? 1 : 2;
                } else {
                    pl->unk38 = ((f32) m520_r9C[pl->dif] > (100.0f * ((f32) rand8() / 255.0f))) ? 2 : 1;
                }
                if (pl->unk38 == 1) {
                    sgn = 1;
                } else if (pl->unk44 == 1) {
                    sgn = (rand8() % 2) + 1;
                } else {
                    sgn = (pl->unk44 >= 2) ? 1 : pl->unk38;
                }
                pl->unk38 = sgn;
                pl->unk3C = 0;
                if (pl->unk38 == 2) {
                    pl->unk40 = 0;
                    pl->unk44 += 1;
                } else {
                    pl->unk44 = 0;
                }
                if (pl->unk38 == 1 || pl->unk44 > 0) {
                    val = 1.0;
                } else if (rand8() % 3 != 0) {
                    val = 0.01;
                } else if (rand8() % 3 != 0) {
                    val = 0.5;
                } else {
                    val = 1.0;
                }
                pl->unk48 = (f32) (((pl->unk48 < 0.0f) ? 1 : -1) * val);
                break;
            case 3:
                pl->unk3C += 1;
                if (pl->unk3C == 1) {
                    btnDown = 0x100;
                    btn = 0x100;
                } else if (pl->unk3C < m520_rCC[pl->dif]) {
                    btn = 0x100;
                } else if (pl->unk3C == m520_rCC[pl->dif + 4]) {
                    btnDown = 0x100;
                    btn = 0x100;
                }
                if (pl->unk3C > m520_rCC[pl->dif + 4] && mp->mode == 0) {
                    pl->unk3C = 0;
                    switch (pl->dif) {
                    case 0:
                        pl->unk38 = 4;
                        pl->unk48 = (f32) (0.01 * (f64) (((rand8() % 2) * 2) - 1));
                        pl->unk40 = 0x78;
                        break;
                    case 1:
                        pl->unk38 = 5;
                        pl->unk48 = (f32) (((rand8() % 2) * 2) - 1);
                        pl->unk40 = 0x78;
                        break;
                    case 2:
                        pl->unk38 = (rand8() % 2) + 4;
                        pl->unk48 = (f32) (((rand8() % 2) * 2) - 1);
                        if (pl->unk38 == 4) {
                            pl->unk40 = rand8() % 30;
                        } else {
                            pl->unk40 = (rand8() % 40) + 0x28;
                        }
                        break;
                    case 3:
                        pl->unk38 = ((rand8() % 4) == 0) + 4;
                        val = ((rand8() % 2) != 0) ? 1.0 : 0.01;
                        pl->unk48 = (f32) (val * (f64) (((rand8() % 2) * 2) - 1));
                        if (pl->unk38 == 4) {
                            pl->unk40 = (rand8() % 25) + 0xF;
                        } else {
                            pl->unk40 = (rand8() % 65) + 0x23;
                        }
                        break;
                    }
                }
                break;
            case 1:
            case 2:
            case 4:
            case 5:
                target = pl->unk48;
                if (lbl_1_bss_5C8[3].f == pl->unk48) {
                    pl->unk3C += 1;
                    if (pl->unk38 != 4 && pl->unk3C > pl->unk40) {
                        if (pl->dif == 3 && (pl->unk48 * nearTbl[best]) >= 0.0f && fabs(nearTbl[best]) < 30.0
                            && pl->unk3C < pl->unk40 + 0x5A) {
                            /* keep going */
                        } else {
                            pl->unk38 = 0;
                            pl->unk40 = 1;
                            pl->unk3C = 1;
                            break;
                        }
                    }
                    if (pl->unk3C > pl->unk40 && pl->unk38 == 4) {
                        pl->unk38 = 5;
                        pl->unk3C = 0;
                        pl->unk40 = (rand8() % 40) + 0x64;
                        pl->unk48 = (f32) ((pl->unk48 < 0.0f) ? 1 : -1);
                    }
                }
                break;
            }
        }
        if (mp->mode != 0 || mdl->motIdShift != -1) {
            target = 0.0f;
        }
        w3old = lbl_1_bss_5C8[3].f;
        if (fabs(target) < fabs(w3old)) {
            step = 1.0f / 15.0f;
        } else {
            step = 1.5f / 15.0f;
        }
        sgn = (w3old < target) ? 1 : -1;
        lbl_1_bss_5C8[3].f = lbl_1_bss_5C8[3].f + ((f32) sgn * step);
        if ((w3old <= target && lbl_1_bss_5C8[3].f > target) || (w3old >= target && lbl_1_bss_5C8[3].f < target)) {
            lbl_1_bss_5C8[3].f = target;
        }
        lbl_1_bss_5C8[0x7 + lbl_1_bss_5C8[0x7F].i].f = lbl_1_bss_5C8[2].f;
        lbl_1_bss_5C8[0x43 + lbl_1_bss_5C8[0x7F].i].f = lbl_1_bss_5C8[3].f;
        lbl_1_bss_5C8[0x7F].i += 1;
        lbl_1_bss_5C8[0x7F].i = lbl_1_bss_5C8[0x7F].i % 60;
        lbl_1_bss_5C8[2].f = lbl_1_bss_5C8[2].f + ((360.0f * lbl_1_bss_5C8[3].f) / 360.0f);
        if (lbl_1_bss_5C8[2].f < -180.0f) {
            sgn = 360;
        } else if (lbl_1_bss_5C8[2].f >= 180.0f) {
            sgn = -360;
        } else {
            sgn = 0;
        }
        lbl_1_bss_5C8[2].f = lbl_1_bss_5C8[2].f + (f32) sgn;
        Hu3DModelRotSet(lbl_1_bss_1B4->mdlId[6], 0.0f, lbl_1_bss_5C8[2].f, 0.0f);
        Hu3DModelRotSet(lbl_1_bss_1B4->mdlId[8], 0.0f, lbl_1_bss_5C8[2].f, 0.0f);
        Hu3DMotionTimeSet(lbl_1_bss_1B4->mdlId[7], 20.0f + (20.0f * lbl_1_bss_5C8[3].f));
        if (lbl_1_bss_10 == 0) {
            if ((target < 0.0f && lbl_1_bss_C >= 0.0f) || (target > 0.0f && lbl_1_bss_C <= 0.0f)) {
                if (lbl_1_bss_1D0[0x8C / 4] < 3) {
                    lbl_1_bss_1D0[0x90 / 4 + lbl_1_bss_1D0[0x8C / 4]] = 0;
                    lbl_1_bss_1D0[0x8C / 4] += 1;
                }
                lbl_1_bss_10 = 6;
            }
        }
        lbl_1_bss_10 = lbl_1_bss_10 - (lbl_1_bss_10 > 0);
        lbl_1_bss_C = target;
        stkX = 0;
        stkY = 0;
    } else {
        oldState = pl->unk1C;
        airborne = (mp->actor->colGroundAttr & 0x407F) == 0;
        len = M520Sqrt((pos.x * pos.x) + (pos.z * pos.z));
        if (lbl_1_bss_1A0 == 2 || lbl_1_bss_1A0 == 3) {
            if (lbl_1_bss_1A0 == 2) {
                if (lbl_1_bss_0 < 0xD7) {
                    mp->actor->pos.x = (f32) ((0 + 0x21C) * sin((3.141592653589793 * ((0.5 * (f64) (pl->idx == 1)) + (f64) ((pl->idx * 0x78) + 0x3C))) / 180.0));
                    mp->actor->pos.z = (f32) (((0 + 0x21C) * cos((3.141592653589793 * ((0.5 * (f64) (pl->idx == 1)) + (f64) ((pl->idx * 0x78) + 0x3C))) / 180.0)) - 25.0);
                }
                if (lbl_1_bss_0 == 0xE1) {
                    btnDown = 0x100;
                    btn = 0x100;
                } else if (lbl_1_bss_0 > 0xE1 && lbl_1_bss_0 <= 0xF0) {
                    btn = 0x100;
                }
            }
            if (lbl_1_bss_1A0 == 2 && ((lbl_1_bss_0 >= 0xDC && lbl_1_bss_0 <= 0xE1) || airborne != 0)) {
                stkX = (s32) (55.0f * (-pos.x / len));
                stkY = (s32) (-55.0f * (-pos.z / len));
            }
        } else if (lbl_1_bss_1A0 == 5 && pl->unk1C == 2 && pl->padNo == -1) {
            f32 ang;
            f32 ringAng;
            f32 ringSpd;
            f32 bestD;
            f32 speed;
            f32 tx;
            f32 tz;
            f32 tl;
            f32 sp;
            f32 dist;

            sp48 = 0;
            if (lbl_1_bss_5C8[6].i > 0 && lbl_1_bss_5C8[6].i < 0xBE) {
                sp48 = 1;
            }
            j = ((lbl_1_bss_5C8[0x7F].i - m520_rEC[(sp48 * 4) + pl->dif]) + 0x3C) % 60;
            ringAng = lbl_1_bss_5C8[0x7 + j].f;
            ringSpd = lbl_1_bss_5C8[0x43 + j].f;
            ang = (f32) (180.0 * (atan2(pos.x, pos.z) / 3.141592653589793));
            bestD = 10000.0f;
            for (i = -6; i <= 6; i++) {
                d = ang - (ringAng + (f32) (i * 0x78));
                if (fabs(bestD) > fabs(d)) {
                    bestD = d;
                }
            }
            if (lbl_1_bss_5C8[6].i == 0) {
                speed = 360.0f;
            } else if (lbl_1_bss_5C8[6].i < 0x46) {
                speed = 360.0f + (80.0f * ((f32) lbl_1_bss_5C8[6].i / 70.0f));
            } else if (lbl_1_bss_5C8[6].i < 0x64) {
                speed = 440.0f;
            } else if (lbl_1_bss_5C8[6].i < 0xA0) {
                speed = 210.0f;
            } else if (lbl_1_bss_5C8[6].i < 0xBE) {
                speed = 210.0f + (150.0f * ((f32) (lbl_1_bss_5C8[6].i - 0xA0) / 30.0f));
            } else {
                speed = 360.0f;
            }
            btnDown = (lbl_1_bss_5C8[6].i == 0x66) ? 0x100 : 0;
            btn = (lbl_1_bss_5C8[6].i >= 0x66 && lbl_1_bss_5C8[6].i <= 0x6E) ? 0x100 : 0;
            pl->unk3C += 1;
            switch (pl->unk38) {
            case 0:
                pl->unk4C = ang;
                if ((fabs(ringSpd) > 0.3 && (bestD * ringSpd) > 0.0f && fabs(bestD) < 45.0)
                    || (fabs(bestD) < 35.0 && ((bestD * ringSpd) > 0.0f || fabs(ringSpd) < 0.5))) {
                    pl->unk38 = (bestD < 0.0f) ? -1 : 1;
                    pl->unk3C = 0;
                }
                break;
            case 1:
            case -1:
                if (pl->unk3C >= pl->unk40
                    && ((fabs(ringSpd) < 0.3 && fabs(bestD) > 45.0) || ((bestD * (f32) pl->unk38) < 0.0f && fabs(bestD) < 35.0))) {
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
                        j = 0;
                        if (lbl_1_bss_5C8[6].i > 0 && lbl_1_bss_5C8[6].i < 0xBE) {
                            j = 1;
                        }
                        pl->unk40 = m520_r134[(j * 5) + (rand8() % 5)];
                        break;
                    }
                    pl->unk3C = 0;
                }
                break;
            }
            pl->unk50 = speed;
            tx = (f32) (((f64) pl->unk50 * sin((3.141592653589793 * pl->unk4C) / 180.0)) - pos.x);
            tz = (f32) (((f64) pl->unk50 * cos((3.141592653589793 * pl->unk4C) / 180.0)) - pos.z);
            tl = M520Sqrt((tx * tx) + (tz * tz));
            sp = 5.6f * tl;
            if (sp < 120.0f) {
                dist = 1.0f;
            } else {
                dist = 64.0f * (1.0f / sp);
            }
            stkX = (s32) ((5.6f * tx) * dist);
            if (sp < 120.0f) {
                dist = 1.0f;
            } else {
                dist = 64.0f * (1.0f / sp);
            }
            stkY = (s32) ((-5.6f * tz) * dist);
        }
        if ((lbl_1_bss_1A0 == 5 || lbl_1_bss_1A0 == 6) && pl->unk1C == 2) {
            if ((len > 700.0f && (airborne == 0 || pos.y < 0.0f))) {
                pl->unk1C = 3;
            } else if (lbl_1_bss_1A0 == 5 && lbl_1_bss_5C8[6].i >= 0x28 && lbl_1_bss_5C8[6].i < 0xC8
                       && (mp->actor->colGroundAttr & 0x407F) != 0) {
                s32 t = lbl_1_bss_5C8[6].i - 0x1E;
                f32 fr = (f32) (t % 10) / 10.0f;
                f32 lo = ((1.0f - fr) * (f32) m520_r15C[t / 10]) + (fr * (f32) m520_r15C[(t / 10) + 1]);
                f32 hi = ((1.0f - fr) * (f32) m520_r1AC[t / 10]) + (fr * (f32) m520_r1AC[(t / 10) + 1]);

                if (lo - 30.0f <= len && len <= 30.0f + hi) {
                    pl->unk1C = 3;
                }
            }
            if (lbl_1_bss_1A0 == 5) {
                for (i = 0; i < 0x1E; i++) {
                    s32 *rec = &lbl_1_bss_280[i * 7];

                    if (rec[6] != 0) {
                        f32 dx = pos.x - ((f32 *) rec)[0];
                        f32 dy = pos.y - ((f32 *) rec)[1];
                        f32 dz = pos.z - ((f32 *) rec)[2];

                        if ((dx * dx) + (dz * dz) < 9025.0f) {
                            pl->unk1C = 3;
                        }
                    }
                }
            }
        }
        if (pl->unk1C == 3) {
            f32 dirx;
            f32 dirz;

            btnDown = 0;
            btn = 0;
            stkY = 0;
            stkX = 0;
            if (oldState == 2) {
                MgPlayerModeSet(mp, 0xB);
                MgPlayerPauseOn(mp);
                omVibrate((s16) pl->conf, 0x30, 1, 0);
                CharFXPlay((s16) pl->charFull, 0x1F9);
                pl->unk34 = 1.0f;
            } else {
                pos.x = arg0->trans.x;
                pos.y = arg0->trans.y;
                pos.z = arg0->trans.z;
            }
            len = M520Sqrt((pos.x * pos.x) + (pos.z * pos.z));
            if (len > 1800.0f) {
                Hu3DModelAttrSet(arg0->mdlId[0], 1);
            }
            CharMotionShiftSet(mp->charNo, mp->omObj->mtnId[11], 1.0f, 0.0f, 0x40000001);
            if (len == 0.0f) {
                dirz = 0.0f;
                dirx = 0.0f;
            } else {
                dirx = pos.x / len;
                dirz = pos.z / len;
            }
            pos.x = pos.x + (16.0f * dirx);
            pos.z = pos.z + (16.0f * dirz);
            pos.y = pos.y + (50.0f * pl->unk34);
            pl->unk34 = pl->unk34 - 0.04f;
            arg0->rot.y = (f32) (180.0 * (atan2(dirx, dirz) / 3.141592653589793));
            omSetTra(arg0, pos.x, pos.y, pos.z);
        }
    }
    if ((lbl_1_bss_1A0 == 8 || lbl_1_bss_1A0 == 9) && lbl_1_data_9C == 0 && pl->idx == 0) {
        mdl = &Hu3DData[arg0->mdlId[1]];
        Hu3DModelAttrSet(arg0->mdlId[0], 1);
        Hu3DModelAttrSet(lbl_1_bss_1B8[1]->mdlId[0], 1);
        Hu3DModelAttrSet(lbl_1_bss_1B8[2]->mdlId[0], 1);
        Hu3DModelAttrSet(lbl_1_bss_1B8[3]->mdlId[0], 1);
        Hu3DModelAttrReset(arg0->mdlId[1], 1);
        if (pl->charNo == 3) {
            i = -0x1E;
        } else if (pl->charNo == 4) {
            i = -0x1E;
        } else {
            i = 0;
        }
        Hu3DModelPosSet(arg0->mdlId[1], arg0->trans.x, arg0->trans.y, arg0->trans.z + (f32) i);
        if (mdl->motId != arg0->mtnId[2]) {
            Hu3DMotionSet(arg0->mdlId[1], arg0->mtnId[2]);
            Hu3DModelAttrReset(arg0->mdlId[1], 0x40000002);
        } else if (((s32) mdl->motWork.time) - 1 == lbl_1_data_D0[pl->charFull]) {
            CharFXPlay((s16) pl->charFull, 0x1FA);
        }
    } else if (lbl_1_bss_1A0 == 8 && lbl_1_data_9C > 0 && pl->idx > 0) {
        MgPlayerModeSet(mp, 0xA);
    } else if (lbl_1_bss_1A0 == 8 && lbl_1_data_9C == 1 && pl->idx == 0) {
        MgPlayerModeSet(mp, 0xC);
    }
    MgPlayerPadSet(mp, stkX, stkY, btnDown | btnDownMask, btn | btnMask);
    if (padOk) {
        HuPadBtnRep[mp->padNo] = HuPadBtnRep[mp->padNo] & 0x1010;
        HuPadBtnRep[mp->padNo] = HuPadBtnRep[mp->padNo] | (btnDown | btnDownMask);
    }
    if (pl->idx == 0) {
        f32 fcc;
        s32 mot[2];

        fcc = 0.0f;
        mdl = &Hu3DData[arg0->mdlId[0]];
        if (lbl_1_bss_1A0 == 2 && lbl_1_bss_0 <= 0x44) {
            fcc = (f32) m520_r1FC[pl->charNo];
        } else {
            for (i = 0; i < 2; i++) {
                s16 id = (i == 0) ? mdl->motId : mdl->motIdShift;

                if (id == -1) {
                    mot[i] = -1;
                } else if (id == mp->omObj->mtnId[11]) {
                    mot[i] = 0;
                } else if (id == mp->omObj->mtnId[10] || id == mp->omObj->mtnId[12]) {
                    mot[i] = 2;
                } else {
                    mot[i] = 1;
                }
            }
            fcc = (f32) m520_r1FC[(mot[0] * 10) + pl->charNo];
            if (mot[1] != -1) {
                f32 fr = mdl->motOvlWork.start / mdl->motOvlWork.end;

                fcc = (fcc * (1.0f - fr)) + (fr * (f32) m520_r1FC[(mot[1] * 10) + pl->charNo]);
            }
        }
        mp->actor->pos.z = fcc;
    }
}
