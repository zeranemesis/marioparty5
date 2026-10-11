#include "REL/m570Dll.h"

#include "game/mg/seqman.h"
#include "game/mg/timer.h"
#include "game/hu3d.h"
#include "game/hsfex.h"
#include "game/sprite.h"
#include "game/audio.h"
#include "game/esprite.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/flag.h"

#include "dolphin/mtx.h"

#include "math.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* The particle manager parameter blob is 0x54 bytes; the shared header type
 * stops at 0x50 because it has no explicit field for the two padding bytes
 * after colorNum and the trailing word. */
typedef struct Hu3DParmanParam2_s {
    /* 0x00 */ s16 maxTime;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ float accelRange;
    /* 0x08 */ float scaleRange;
    /* 0x0C */ float angleRange;
    /* 0x10 */ Vec gravity;
    /* 0x1C */ float speedBase;
    /* 0x20 */ float speedDecay;
    /* 0x24 */ float scaleBase;
    /* 0x28 */ float scaleDecay;
    /* 0x2C */ s16 colorNum;
    /* 0x2E */ s16 pad;
    /* 0x30 */ GXColor colorStart[4];
    /* 0x40 */ GXColor colorEnd[4];
    /* 0x50 */ s32 unk3;
} HU3DPARMANPARAM2;

/* Camera descriptor handled by fn_1_1A50 / fn_1_2050 (0x48 bytes). */
typedef struct m570Cam_s {
    /* 0x00 */ s32 camNo;
    /* 0x04 */ s32 state;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ HuVecF pos;
    /* 0x20 */ HuVecF up;
    /* 0x2C */ HuVecF target;
    /* 0x38 */ HuVecF rot;
    /* 0x44 */ s32 unk44;
} M570CAM;

/* Light descriptor passed to Hu3DGLightCreateV by fn_1_1AA0. */
typedef struct m570Light_s {
    /* 0x00 */ HuVecF pos;
    /* 0x0C */ HuVecF dir;
    /* 0x18 */ GXColor color;
} M570LIGHT;

/* ---------------------------------------------------------------- rodata */

static const f64 gHalf3[2] = {0.5, 3.0};
static const f32 kZero = 0.0f;
static const f32 kScreenW = 640.0f;
static const f32 kScreenH = 480.0f;
static const f32 kOne = 1.0f;
static const f32 kOnePoint2 = 1.2f;
static const f32 kOnePoint1 = 0.1f;
static const f64 gTwoPowM1022 = 2.2250738585072014e-308;
static const f64 gRoundToEven = 4503601774854144.0;
static const f64 gPi = 3.141592653589793;
static const f64 gD180 = 180.0;
static const f64 gD1 = 1.0;
static const f64 gDHalf = 0.5;
static const f64 gD3 = 3.0;
static const f32 k90 = 90.0f;
static const f64 gD90 = 90.0;
static const f32 k360 = 360.0f;
static const f64 gD360 = 360.0;
static const f32 k180 = 180.0f;
static const f32 kSmall = 0.001f;
static const f32 k30 = 30.0f;
static const s32 gSeTableA[13] = {
    0x007D0000, 0x007D0001, 0x007D0002, 0x007D0003, 0x007D0004, 0x007D0005,
    0x007D0006, 0x007D0007, 0x007D0008, 0x007D0009, 0x007D000A, 0x007D0018,
    0x007D0018
};
static const s32 gSeTableB[5] = {
    0x007D001A, 0x007D001C, 0x007D001E, 0x007D0020, 0x007D0022
};
static const s32 gSeTableC[5] = {
    0x007D0019, 0x007D001B, 0x007D001D, 0x007D001F, 0x007D0021
};
static const f32 kCamXMin = -270.0f;
static const f32 kCamYMin = -350.0f;
static const f32 kCamXMax = 540.0f;
static const f32 kCamYMax = 350.0f;
static const f32 kShadowTp = 0.5f;
static const f32 kShadowX = 120.0f;
static const f32 kShadowY = 80.0f;
static const f32 kShadowZ = 288.0f;
static const f32 kShadowRange = 420.0f;
static const f32 kShadowAlpha = 0.05f;
static const f32 kShadowNear = 60.0f;
static const f32 kShadowAngle = 2.0f;
static const f32 kPanBase = 32.0f;
static const f32 kPanScale = 64.0f;
static const f32 gScreenHalf[2] = {576.0f, 0.0f};

/* ------------------------------------------------------------------ data */

static const char gStrCamDiff[] = "MgCameraMoveEntry : Camera Number Different\n";
static const char gStrFlowMax[] = "MgFlowSet : Value is Max Over\n";
static const char gStrFlowNext[] = "MgFlowNext : Value is Max Over\n";

void fn_1_120(s16 mode, s16 frame);
void fn_1_520(s16 mode, s16 frame);
void fn_1_A78(s16 mode, s16 frame);
void fn_1_ABC(s16 mode, s16 frame);
void fn_1_D88(s16 mode, s16 frame);
void fn_1_DEC(s16 mode, s16 frame);
void fn_1_F54(s16 mode, s16 frame);
void fn_1_1034(s16 mode, s16 frame);
void fn_1_A7C(s16 mode, s16 frame);

static MGSEQ_PARAM gSeqParam = {
    0,
    0,
    fn_1_120,
    fn_1_520,
    fn_1_A78,
    fn_1_ABC,
    fn_1_D88,
    fn_1_DEC,
    fn_1_F54,
    NULL,
    fn_1_1034
};

static const s32 gLightParam[7] = {
    0, (s32)300.0f, (s32)0.001f, 0, (s32)-300.0f, 0, -1
};
static const s32 gShadowParamA[3] = {0, (s32)2000.0f, 0};
static const s32 gShadowParamB[3] = {(s32)1.0f, 0, 0};
static const s32 gShadowParamC[3] = {0, 0, 0};
static const f32 gCamTarget[5] = {250.0f, 270.0f, 288.0f, 310.0f, 330.0f};

static const s32 gCamTbl[7][18] = {
    {1, 1, (s32)40.0f, (s32)20.0f, (s32)8000.0f, 0, 0, 0, 0, (s32)1.0f, 0,
     (s32)-300.0f, (s32)100.0f, (s32)50.0f, (s32)-15.0f, (s32)90.0f, 0,
     (s32)500.0f},
    {1, 1, (s32)40.0f, (s32)20.0f, (s32)8000.0f, 0, 0, 0, 0, (s32)1.0f, 0,
     (s32)-300.0f, (s32)100.0f, (s32)50.0f, (s32)-15.0f, 0, 0, (s32)500.0f},
    {1, 1, (s32)40.0f, (s32)20.0f, (s32)8000.0f, 0, 0, 0, 0, (s32)1.0f, 0,
     (s32)300.0f, (s32)100.0f, (s32)50.0f, (s32)-15.0f, (s32)-90.0f, 0,
     (s32)600.0f},
    {1, 1, (s32)40.0f, (s32)20.0f, (s32)8000.0f, 0, 0, 0, 0, (s32)1.0f, 0,
     (s32)300.0f, (s32)100.0f, (s32)50.0f, (s32)-15.0f, 0, 0, (s32)600.0f},
    {1, 1, (s32)40.0f, (s32)20.0f, (s32)8000.0f, 0, 0, 0, 0, (s32)1.0f, 0, 0,
     (s32)100.0f, (s32)-300.0f, (s32)-36.0f, 0, 0, (s32)400.0f},
    {1, 1, (s32)40.0f, (s32)20.0f, (s32)8000.0f, 0, 0, 0, 0, (s32)1.0f, 0, 0,
     (s32)150.0f, 0, (s32)-15.0f, 0, 0, (s32)1000.0f},
    {1, 1, (s32)40.0f, (s32)20.0f, (s32)8000.0f, 0, 0, 0, 0, (s32)1.0f, 0, 0,
     (s32)150.0f, 0, (s32)-15.0f, 0, 0, (s32)1250.0f}
};

static const HU3DPARMANPARAM2 gParManParam = {
    30,
    0,
    100.0f,
    15.0f,
    180.0f,
    {0.0f, 0.0f, 0.0f},
    1.2f,
    0.98f,
    30.0f,
    1.0f,
    1,
    -1,
    {{0x00, 0x00, 0x70, 0xFF}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0xFF, 0xFF, 0, 0}},
    {{0x00, 0x00, 0x00, 0xFF}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    0
};

/* ------------------------------------------------------------------- bss */

static s32 lbl_1_bss_0;
static s32 lbl_1_bss_4;
static s32 lbl_1_bss_8;
static s32 lbl_1_bss_C[8];
static s16 lbl_1_bss_2C[5];
static s16 lbl_1_bss_36[5];
static HUSPRGRPID lbl_1_bss_40;
static s16 lbl_1_bss_42;
static s32 lbl_1_bss_44;
static s32 lbl_1_bss_48;
static s16 lbl_1_bss_4C;
static s16 lbl_1_bss_4E;
static s16 lbl_1_bss_50;
s16 lbl_1_bss_52;
static s32 lbl_1_bss_54[0x48 / 4];
static MGTIMER *lbl_1_bss_9C;
static s32 lbl_1_bss_A0[0x1C / 4];
static s32 lbl_1_bss_BC[0x48 / 4];
static s16 lbl_1_bss_104;
static s16 lbl_1_bss_106;
OMOBJMAN *lbl_1_bss_108;
static s32 lbl_1_bss_10C;

/* -------------------------------------------------------------- functions */

static void fn_1_1354(OMOBJ *obj);
static void fn_1_1674(OMOBJ *obj);
static void fn_1_14A8(OMOBJ *obj);
static void fn_1_1AFC(OMOBJ *obj);
void fn_1_12F0(void);
void fn_1_2020(M570CAM *src);
void fn_1_2050(M570CAM *cam);

void ObjectSetup(void);

int _prolog(void)
{
    const VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    lbl_1_bss_108 = omInitObjMan(200, 0x1000);
    lbl_1_bss_104 = -1;
    omGameSysInit(lbl_1_bss_108);
    MgSeqCreatePrio(&gSeqParam, 2000);
    MgSeqModeHookAdd(MGSEQ_MODE_PREMAIN, fn_1_A7C);
    MgSeqModeDelaySet(MGSEQ_MODE_FADEIN, 1500);
}

void fn_1_A78(s16 mode, s16 frame)
{
}

void fn_1_A7C(s16 mode, s16 frame)
{
    lbl_1_bss_50 = 9;
    lbl_1_bss_4C = 0;
    fn_1_12F0();
}

void fn_1_D88(s16 mode, s16 frame)
{
    if (frame == 0 && lbl_1_bss_48 != -1) {
        HuAudSStreamFadeOut(lbl_1_bss_48, 100);
    }
    lbl_1_bss_50 = 13;
}

void fn_1_1034(s16 mode, s16 frame)
{
}

void fn_1_1038(void)
{
    lbl_1_bss_9C = MgTimerCreate(0);
    MgTimerParamSet(lbl_1_bss_9C, 180, 0, 0);
    MgTimerModeOnSet(lbl_1_bss_9C, 1);
}

void fn_1_1098(void)
{
    s16 winner[M570_PLAYER_MAX];
    s32 i;

    for (i = 0; i < M570_PLAYER_MAX; i++) {
        winner[i] = -1;
        if (lbl_1_bss_52 == lbl_1_bss_1C4[i]->grp) {
            winner[i] = lbl_1_bss_1C4[i]->charNo;
            if (!_CheckFlag(0x1000F)) {
                /* The unit stores the mini-game score word at offset 0x2C of GWPLAYER. */
*(s16 *)((s8 *)&GwPlayer[i] + 0x2C) = 10;
            }
        }
    }
    MgSeqWinnerSet(winner[0], winner[1], winner[2], winner[3]);
}

// NONMATCHING: instruction scheduling and register allocation differ from the original build
void fn_1_1174(void)
{
    HUSPRGRPID esp;
    s32 i;

    lbl_1_bss_40 = MgScoreWinCreate(0x84, 0x28);
    MgScoreWinPosSet(lbl_1_bss_40, kShadowZ, kShadowRange);
    MgScoreWinDispSet(lbl_1_bss_40, FALSE);
    for (i = 0; i < 5; i++) {
        esp = espEntry(gSeTableC[i], 0x20, 0);
        espDrawNoSet(esp, 0);
        espPosSet(esp, gCamTarget[i], kShadowRange);
        espDispOff(esp);
        lbl_1_bss_2C[i] = esp;
        esp = espEntry(gSeTableB[i], 0x10, 0);
        espDrawNoSet(esp, 0);
        espPosSet(esp, gCamTarget[i], kShadowRange);
        espDispOff(esp);
        lbl_1_bss_36[i] = esp;
    }
}

void fn_1_12F0(void)
{
    s32 i;

    MgScoreWinDispSet(lbl_1_bss_40, TRUE);
    for (i = 0; i < 5; i++) {
        espDispOn(lbl_1_bss_2C[i]);
    }
}

// NONMATCHING: instruction scheduling and register allocation differ from the original build
static void fn_1_1354(OMOBJ *obj)
{
    f32 tpLvl;
    s32 i;

    obj->work[0]++;
    tpLvl = kOne - (f32)((f64)obj->work[0] + gTwoPowM1022 - gTwoPowM1022) * kShadowAlpha;
    MgScoreWinTPLvlSet(lbl_1_bss_40, tpLvl);
    for (i = 0; i < 5; i++) {
        espTPLvlSet(lbl_1_bss_2C[i], tpLvl);
        espTPLvlSet(lbl_1_bss_36[i], tpLvl);
    }
    if (obj->work[0] == 20) {
        omDelObjEx(lbl_1_bss_108, obj);
    }
}

void fn_1_1464(void)
{
    omAddObjEx(lbl_1_bss_108, 0x1300, 0, 0, OM_OBJ_NONE, fn_1_1354);
}

// NONMATCHING: instruction scheduling and register allocation differ from the original build
static void fn_1_14A8(OMOBJ *obj)
{
    HUSPRGRPID espId;
    f32 t;
    f32 scale;

    espId = (HUSPRGRPID)obj->work[0];
    if (obj->work[1] == 0) {
        espDispOn(espId);
    }
    t = (f32)sin((f64)((f32)((f64)obj->work[1] + gTwoPowM1022 - gTwoPowM1022) * k90 /
                          kShadowNear * gPi / gD180));
    scale = kOne + (kShadowAngle - kShadowAngle * t);
    espScaleSet(espId, scale, scale);
    espTPLvlSet(espId, t);
    obj->work[1]++;
    if (obj->work[1] > 60) {
        omDelObjEx(lbl_1_bss_108, obj);
    }
}

void fn_1_15FC(s32 idx)
{
    OMOBJ *obj;

    obj = omAddObjEx(lbl_1_bss_108, 0x1300, 0, 0, OM_OBJ_NONE, fn_1_14A8);
    obj->work[0] = lbl_1_bss_36[idx];
    obj->work[1] = 0;
}

static void fn_1_1674(OMOBJ *obj)
{
    if (--obj->work[0] == 0) {
        Hu3DParManKill((s16)obj->work[1]);
        omDelObjEx(lbl_1_bss_108, obj);
    }
}

void fn_1_16D4(HuVecF *pos)
{
    ANIMDATA *anim;
    HU3DPARMANID parManId;
    OMOBJ *obj;

    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x210002, HU_MEMNUM_OVL, HEAP_MODEL));
    parManId = Hu3DParManCreate(anim, 100, (HU3DPARMANPARAM *)&gParManParam);
    Hu3DParManPosSet(parManId, pos->x, pos->y, pos->z);
    Hu3DParManVecSet(parManId, kZero, kZero, kOne);
    Hu3DParManTimeLimitSet(parManId, 30);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(parManId), 3);
    obj = omAddObjEx(lbl_1_bss_108, 0x1300, 0, 0, OM_OBJ_NONE, fn_1_1674);
    obj->work[0] = 30;
    obj->work[1] = parManId;
}

s32 fn_1_17DC(HuVecF *pos)
{
    HuVecF screen;

    Hu3D3Dto2D(pos, (s16)lbl_1_bss_54[0], &screen);
    return (s32)(kPanBase + kPanScale * (screen.x / gScreenHalf[0]));
}

void fn_1_1864(HuVecF *pos, s16 seId)
{
    HuVecF screen;
    s32 pan;

    Hu3D3Dto2D(pos, (s16)lbl_1_bss_54[0], &screen);
    pan = (s32)(kPanBase + kPanScale * (screen.x / gScreenHalf[0]));
    HuAudFXPlayPan(seId, pan);
}

void fn_1_1910(void)
{
    lbl_1_bss_108 = omInitObjMan(200, 0x1000);
    lbl_1_bss_104 = -1;
    omGameSysInit(lbl_1_bss_108);
}

void fn_1_1A50(M570CAM *cam)
{
    if (cam->state == 1) {
        fn_1_2050(cam);
    }
    Hu3DCameraPosSetV(cam->camNo, &cam->pos, &cam->up, &cam->target);
}

// NONMATCHING: instruction scheduling and register allocation differ from the original build
void fn_1_1AA0(M570LIGHT *light)
{
    HU3DLIGHTID lightId;

    lightId = Hu3DGLightCreateV(&light->pos, &light->dir, &light->color);
    Hu3DGLightStaticSet(lightId, TRUE);
    Hu3DGLightInfinitytSet(lightId);
}

// NONMATCHING: instruction scheduling and register allocation differ from the original build
void fn_1_1F74(M570CAM *cam)
{
    OMOBJ *obj;

    obj = omAddObjEx(lbl_1_bss_108, 0x4000, 0, 0, OM_OBJ_NONE, fn_1_1AFC);
    obj->work[0] = (s32)cam;
    obj->work[1] = (cam->state != 0) ? 1 : 0;
    fn_1_2020(cam);
}

void fn_1_2020(M570CAM *src)
{
    M570CAM *dst = (M570CAM *)lbl_1_bss_BC;

    *dst = *src;
}

// NONMATCHING: instruction scheduling and register allocation differ from the original build
s32 fn_1_3B40(HuVecF *pos, HuVecF *goal, HuVecF *angle, f32 step, const f32 *tbl)
{
    HuVecF dir;
    f32 mag;

    PSVECSubtract(goal, pos, &dir);
    mag = PSVECMag(&dir);
    if (mag == kZero) {
        return 1;
    }
    PSVECNormalize(&dir, &dir);
    PSVECScale(&dir, &dir, step);
    if (PSVECMag(&dir) >= mag) {
        pos->x = goal->x;
        pos->y = goal->y;
        pos->z = goal->z;
        return 1;
    }
    PSVECAdd(pos, &dir, pos);
    angle->y = (f32)(gD180 * (atan2(dir.x, dir.z) / gPi));
    return 0;
}

// NONMATCHING: instruction scheduling and register allocation differ from the original build
f32 fn_1_3C64(const f32 *tbl, f32 val, f32 min, f32 max)
{
    f32 diff;
    f32 ret;

    diff = (f32)fmod(min - val, gD360);
    if (diff < kZero) {
        diff += k360;
    }
    if (diff > k180) {
        diff -= k360;
    }
    if (fabs(diff) > max) {
        if (diff < kZero) {
            diff = -max;
        } else {
            diff = max;
        }
    }
    ret = (f32)fmod(val + diff, gD360);
    if (ret < kZero) {
        ret += k360;
    }
    return ret;
}

/* 0x00 src, 0x04 from, 0x4C to, 0x94 progress, 0x98 accum, 0x9C flags. */
typedef struct m570Move_s {
    /* 0x00 */ M570CAM *src;
    /* 0x04 */ M570CAM from_cam;
    /* 0x4C */ M570CAM to;
    /* 0x94 */ f32 progress;
    /* 0x98 */ f32 accum;
    /* 0x9C */ u32 flags;
} M570MOVE;

void fn_1_36D0(void)
{
}

// Delete a queued camera-move object by its work pointer.
void fn_1_2E64(OMOBJ *obj)
{
    omDelObjEx(lbl_1_bss_108, obj);
}

// True when a camera move has run to completion (bit 1 of its flags word).
BOOL fn_1_2E30(OMOBJ *obj)
{
    M570MOVE *mv = (M570MOVE *)obj->data;

    return (mv->flags & 2) ? 1 : 0;
}

/* 0x00 src, 0x04 from, 0x4C to, 0x94 progress, 0x98 accum, 0x9C flags. */
void fn_1_2B18(OMOBJ *obj);

// QUEUE a camera move between two descriptors. Refuses when the two sides name
// different cameras, otherwise allocates the move object, seeds both endpoints
// and pre-transforms them when the source camera is already live.
OMOBJ *MgCameraMoveEntry(M570CAM *from, M570CAM *to, f32 arg2)
{
    OMOBJ *obj;
    M570MOVE *mv;

    if (from->camNo != to->camNo) {
        OSReport(gStrCamDiff);
        goto done;
    }
    obj = omAddObjEx(lbl_1_bss_108, 0x2000, 0, 0, -1, fn_1_2B18);
    obj->data = HuMemDirectMallocNum(0, 0xA0, HU_MEMNUM_OVL);
    mv = (M570MOVE *)obj->data;
    mv->progress = arg2;
    mv->accum = kZero;
    mv->src = from;
    mv->from_cam = *from;
    mv->to = *to;
    mv->flags = 3;
    if (from->state == 1) {
        fn_1_2050(&mv->from_cam);
        fn_1_2050(&mv->to);
    }
done:
    return obj;
}
