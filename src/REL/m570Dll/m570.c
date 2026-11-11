#define fn_1_3B40 hdr_fn_1_3B40
#define fn_1_3C64 hdr_fn_1_3C64
#define fn_1_17DC hdr_fn_1_17DC
#define fn_1_16D4 hdr_fn_1_16D4
#include "REL/m570Dll.h"
#undef fn_1_3B40
#undef fn_1_3C64
#undef fn_1_17DC
#undef fn_1_16D4

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
#include "game/wipe.h"
#include "game/pad.h"

#include "dolphin/mtx.h"

#include "math.h"

#define M570_RAD(x) (3.141592653589793 * (f64)(x) / 180.0)

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
    /* 0x08 */ f32 fov;
    /* 0x0C */ f32 nearZ;
    /* 0x10 */ f32 farZ;
    /* 0x14 */ HuVecF pos;
    /* 0x20 */ HuVecF up;
    /* 0x2C */ HuVecF target;
    /* 0x38 */ HuVecF rot;
    /* 0x44 */ f32 dist;
} M570CAM;

/* Light descriptor passed to Hu3DGLightCreateV by fn_1_1AA0. */
typedef struct m570Light_s {
    /* 0x00 */ HuVecF pos;
    /* 0x0C */ HuVecF dir;
    /* 0x18 */ GXColor color;
} M570LIGHT;

/* 0x00 src, 0x04 from, 0x4C to, 0x94 progress, 0x98 accum, 0x9C flags. */
typedef struct m570Move_s {
    /* 0x00 */ M570CAM *src;
    /* 0x04 */ M570CAM from_cam;
    /* 0x4C */ M570CAM to;
    /* 0x94 */ f32 progress;
    /* 0x98 */ f32 accum;
    /* 0x9C */ s32 flags;
} M570MOVE;

static inline BOOL M570MoveDone(OMOBJ *obj)
{
    M570MOVE *mv = (M570MOVE *)obj->data;

    if ((mv->flags & 2) != 0) {
        return 1;
    }
    return 0;
}

static inline void M570MoveDel(OMOBJ *obj);

typedef void (*M570FLOWFUNC)(OMOBJ *obj);

typedef struct m570Flow_s {
    /* 0x00 */ s32 idx;
    /* 0x04 */ M570FLOWFUNC *tbl;
    /* 0x08 */ s32 count;
    /* 0x0C */ s16 mesId;
    /* 0x10 */ MGTIMER *timer;
    /* 0x14 */ s32 timerType;
    /* 0x18 */ s32 timerMax;
    /* 0x1C */ s32 timerEnd;
    /* 0x20 */ s32 timerSpeed;
    /* 0x24 */ s32 pad24;
    /* 0x28 */ s32 work[16];
} M570FLOW;

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

static const s32 gCam0[18] = {1, 1, (s32)40.0f, (s32)20.0f, (s32)8000.0f, 0, 0, 0, 0, (s32)1.0f, 0,
     (s32)-300.0f, (s32)100.0f, (s32)50.0f, (s32)-15.0f, (s32)90.0f, 0,
     (s32)500.0f};
static const s32 gCam1[18] = {1, 1, (s32)40.0f, (s32)20.0f, (s32)8000.0f, 0, 0, 0, 0, (s32)1.0f, 0,
     (s32)-300.0f, (s32)100.0f, (s32)50.0f, (s32)-15.0f, 0, 0, (s32)500.0f};
static const s32 gCam2[18] = {1, 1, (s32)40.0f, (s32)20.0f, (s32)8000.0f, 0, 0, 0, 0, (s32)1.0f, 0,
     (s32)300.0f, (s32)100.0f, (s32)50.0f, (s32)-15.0f, (s32)-90.0f, 0,
     (s32)600.0f};
static const s32 gCam3[18] = {1, 1, (s32)40.0f, (s32)20.0f, (s32)8000.0f, 0, 0, 0, 0, (s32)1.0f, 0,
     (s32)300.0f, (s32)100.0f, (s32)50.0f, (s32)-15.0f, 0, 0, (s32)600.0f};
static const s32 gCam4[18] = {1, 1, (s32)40.0f, (s32)20.0f, (s32)8000.0f, 0, 0, 0, 0, (s32)1.0f, 0, 0,
     (s32)100.0f, (s32)-300.0f, (s32)-36.0f, 0, 0, (s32)400.0f};
static const s32 gCam5[18] = {1, 1, (s32)40.0f, (s32)20.0f, (s32)8000.0f, 0, 0, 0, 0, (s32)1.0f, 0, 0,
     (s32)150.0f, 0, (s32)-15.0f, 0, 0, (s32)1000.0f};
static const s32 gCam6[18] = {1, 1, (s32)40.0f, (s32)20.0f, (s32)8000.0f, 0, 0, 0, 0, (s32)1.0f, 0, 0,
     (s32)150.0f, 0, (s32)-15.0f, 0, 0, (s32)1250.0f};

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
static OMOBJ *lbl_1_bss_4;
static s32 lbl_1_bss_8;
static OMOBJ *lbl_1_bss_C[8];
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
#define BSS54 ((M570CAM *)lbl_1_bss_54)
static MGTIMER *lbl_1_bss_9C;
static s16 lbl_1_bss_A0[14];
static s32 lbl_1_bss_BC[0x48 / 4];
static s16 lbl_1_bss_104;
static s16 lbl_1_bss_106;
OMOBJMAN *lbl_1_bss_108;
static s32 lbl_1_bss_10C;

/* -------------------------------------------------------------- functions */

static inline void M570MoveDel(OMOBJ *obj)
{
    omDelObjEx(lbl_1_bss_108, obj);
}

static void fn_1_1354(OMOBJ *obj);
static void fn_1_1674(OMOBJ *obj);
static void fn_1_14A8(OMOBJ *obj);
static void fn_1_1AFC(OMOBJ *obj);
void fn_1_12F0(void);
void fn_1_2020(M570CAM *src);
void fn_1_2050(M570CAM *cam);

void ObjectSetup(void);

extern M570PLAYER *lbl_1_bss_1B4[M570_CPU_MAX];
extern M570PLAYER *lbl_1_bss_1C0;
void fn_1_4210(void);
void fn_1_4440(M570PLAYER *player, s16 motNo);
void fn_1_4D2C(void);
void fn_1_4DC4(void);
void fn_1_4EF8(void);
void fn_1_5048(void);
void fn_1_51A8(void);
void fn_1_5354(void);
s32 fn_1_5538(void);
void fn_1_56A0(void);
void fn_1_5820(s16 motNo);
void fn_1_5A78(void);
void fn_1_5E38(void);
void fn_1_5F24(s32 loop);
M570CAM *fn_1_1964(M570CAM *src);
void fn_1_2714(M570CAM *cam);
void fn_1_1174(void);
void fn_1_1038(void);
void fn_1_1098(void);
void fn_1_1464(void);
void fn_1_15FC(s32 idx);
void fn_1_1A50(M570CAM *cam);
OMOBJ *MgCameraMoveEntry(M570CAM *from, M570CAM *to, f32 arg2);


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

void fn_1_120(s16 mode, s16 frame)
{
    s32 i;
    s16 lightId;

    for (i = 0; i < 13; i++) {
        lbl_1_bss_A0[i] = Hu3DModelCreate(HuDataSelHeapReadNum(gSeTableA[i], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(lbl_1_bss_A0[i], 0x40000001);
        Hu3DModelPosSet(lbl_1_bss_A0[i], kZero, kZero, kZero);
        Hu3DModelRotSet(lbl_1_bss_A0[i], kZero, kZero, kZero);
        Hu3DModelLayerSet(lbl_1_bss_A0[i], 0);
        Hu3DModelAmbSet(lbl_1_bss_A0[i], kOne, kOne, kOne);
    }
    Hu3DModelPosSet(lbl_1_bss_A0[4], kZero, kZero, kCamXMin);
    Hu3DModelAttrReset(lbl_1_bss_A0[10], 0x40000001);
    Hu3DModelAttrSet(lbl_1_bss_A0[10], 0x40000002);
    Hu3DModelAttrSet(lbl_1_bss_A0[11], 1);
    Hu3DModelAttrSet(lbl_1_bss_A0[12], 1);
    Hu3DModelAttrReset(lbl_1_bss_A0[11], 0x40000001);
    Hu3DModelAttrReset(lbl_1_bss_A0[12], 0x40000001);
    Hu3DModelAttrSet(lbl_1_bss_A0[11], 0x40000002);
    Hu3DModelAttrSet(lbl_1_bss_A0[12], 0x40000002);
    Hu3DModelPosSet(lbl_1_bss_A0[11], kCamYMin, kZero, kCamXMax);
    Hu3DModelPosSet(lbl_1_bss_A0[12], kCamYMax, kZero, kCamXMax);
    fn_1_1964((M570CAM *)gCam0);
    *BSS54 = *(M570CAM *)gCam0;
    lightId = Hu3DGLightCreateV((HuVecF *)&gLightParam[0], (HuVecF *)&gLightParam[3], (GXColor *)&gLightParam[6]);
    Hu3DGLightStaticSet(lightId, 1);
    Hu3DGLightInfinitytSet(lightId);
    Hu3DShadowCreate(BSS54->fov, BSS54->nearZ, BSS54->farZ);
    Hu3DShadowTPLvlSet(kShadowTp);
    Hu3DShadowPosSet((HuVecF *)gShadowParamA, (HuVecF *)gShadowParamB, (HuVecF *)gShadowParamC);
    Hu3DModelShadowMapSet(lbl_1_bss_A0[3]);
    fn_1_4210();
    lbl_1_bss_50 = 0;
    lbl_1_bss_4E = 0;
    lbl_1_bss_4C = 0;
    lbl_1_bss_52 = -1;
    lbl_1_bss_48 = -1;
    fn_1_56A0();
    fn_1_5A78();
    fn_1_1174();
    MgSeqModeNext();
}

void fn_1_520(s16 mode, s16 frame)
{
    s32 i;

    if (WipeCheckEnd() == 0 && lbl_1_bss_48 == -1) {
        lbl_1_bss_48 = HuAudSStreamPlay(0x25);
    }
    switch (lbl_1_bss_50) {
    case 0:
        lbl_1_bss_C[0] = MgCameraMoveEntry((M570CAM *)gCam0, (M570CAM *)gCam1, 120.0f);
        fn_1_4440(lbl_1_bss_1C0, 3);
        lbl_1_bss_50 = 1;
        lbl_1_bss_44 = HuAudFXPlay(0x39C);
        break;
    case 1:
        if (M570MoveDone(lbl_1_bss_C[0]) == 0) {
            M570MoveDel(lbl_1_bss_C[0]);
            lbl_1_bss_50 = 2;
            lbl_1_bss_8 = 0;
        }
        break;
    case 2:
        lbl_1_bss_8++;
        if (lbl_1_bss_8 == 29) {
            WipeCreate(2, 0x81, 60);
        }
        if (lbl_1_bss_8 == 30) {
            lbl_1_bss_C[0] = MgCameraMoveEntry((M570CAM *)gCam2, (M570CAM *)gCam3, 120.0f);
            lbl_1_bss_50 = 3;
            WipeCreate(1, 0x81, 60);
            lbl_1_bss_8 = 0;
        }
        break;
    case 3:
        if (lbl_1_bss_8 == 0) {
            fn_1_4440(lbl_1_bss_1B4[0], 3);
        } else if (lbl_1_bss_8 == 20) {
            fn_1_4440(lbl_1_bss_1B4[1], 3);
        } else if (lbl_1_bss_8 == 40) {
            fn_1_4440(lbl_1_bss_1B4[2], 3);
        }
        lbl_1_bss_8++;
        if (M570MoveDone(lbl_1_bss_C[0]) == 0) {
            M570MoveDel(lbl_1_bss_C[0]);
            lbl_1_bss_50 = 4;
            lbl_1_bss_8 = 0;
        }
        break;
    case 4:
        lbl_1_bss_8++;
        if (lbl_1_bss_8 == 29) {
            WipeCreate(2, 0x81, 60);
        }
        if (lbl_1_bss_8 == 30) {
            fn_1_1A50((M570CAM *)gCam4);
            lbl_1_bss_50 = 5;
            WipeCreate(1, 0x81, 60);
            fn_1_5E38();
            fn_1_4DC4();
            HuAudFXStop(lbl_1_bss_44);
            lbl_1_bss_8 = 0;
        }
        break;
    case 5:
        if (WipeCheckEnd() == 0) {
            if (lbl_1_bss_8 == 0) {
                Hu3DModelAttrReset(lbl_1_bss_A0[10], 0x40000002);
                Hu3DModelAttrSet(lbl_1_bss_A0[8], 1);
                Hu3DModelAttrSet(lbl_1_bss_A0[9], 1);
            }
            if (Hu3DMotionEndCheck(lbl_1_bss_A0[10]) != 0) {
                lbl_1_bss_C[0] = MgCameraMoveEntry((M570CAM *)gCam4, (M570CAM *)gCam5, 30.0f);
                lbl_1_bss_50 = 6;
            }
            lbl_1_bss_8++;
        }
        break;
    case 6:
        if (M570MoveDone(lbl_1_bss_C[0]) == 0) {
            M570MoveDel(lbl_1_bss_C[0]);
            lbl_1_bss_50 = 8;
            for (i = 0; i < 4; i++) {
                fn_1_4440(lbl_1_bss_1C4[i], 4);
            }
        }
        break;
    default:
        MgSeqModeNext();
        break;
    }
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

void fn_1_ABC(s16 mode, s16 frame)
{
    switch (lbl_1_bss_50) {
    case 9:
        if (lbl_1_bss_4C == 0) {
            fn_1_4D2C();
            fn_1_5820(1);
        }
        if (lbl_1_bss_4C++ == 20) {
            fn_1_1038();
            lbl_1_bss_44 = HuAudFXPlayVol(0x39C, 0x60);
            fn_1_5F24(1);
            fn_1_15FC(lbl_1_bss_4E);
            lbl_1_bss_50 = 10;
            lbl_1_bss_4C = 0;
        }
        break;
    case 10:
        if (lbl_1_bss_4C++ == 60) {
            HuAudFXStop(lbl_1_bss_44);
        }
        if (MgTimerDoneCheck(lbl_1_bss_9C) != 0) {
            lbl_1_bss_50 = 11;
            fn_1_5048();
            lbl_1_bss_4C = 0;
        }
        break;
    case 11:
        lbl_1_bss_4C++;
        if (lbl_1_bss_4C == 90) {
            lbl_1_bss_4E++;
            MgTimerKill(lbl_1_bss_9C);
            fn_1_51A8();
            if (fn_1_5538() != 0 || lbl_1_bss_4E == 5) {
                if (fn_1_5538() != 0) {
                    lbl_1_bss_52 = 0;
                } else {
                    lbl_1_bss_52 = 1;
                    lbl_1_bss_1C0->gotItem = 1;
                    lbl_1_bss_1C0->state = 5;
                    fn_1_4440(lbl_1_bss_1C0, 6);
                }
                fn_1_1098();
                lbl_1_bss_50 = 13;
                fn_1_5820(2);
                lbl_1_bss_44 = HuAudFXPlayVol(0x39C, 0x60);
                fn_1_5F24(2);
                fn_1_1464();
                MgSeqModeNext();
            }
        } else if (lbl_1_bss_4C == 180) {
            lbl_1_bss_4C = 0;
            lbl_1_bss_50 = 9;
        }
        break;
    }
}

void fn_1_D88(s16 mode, s16 frame)
{
    if (frame == 0 && lbl_1_bss_48 != -1) {
        HuAudSStreamFadeOut(lbl_1_bss_48, 100);
    }
    lbl_1_bss_50 = 13;
}

void fn_1_DEC(s16 mode, s16 frame)
{
    if (frame == 0) {
        HuAudFXStop(lbl_1_bss_44);
    }
    switch (lbl_1_bss_50) {
    case 13:
        lbl_1_bss_4 = MgCameraMoveEntry((M570CAM *)gCam5, (M570CAM *)gCam6, 80.0f);
        fn_1_4EF8();
        lbl_1_bss_50 = 14;
        break;
    case 14:
        if (M570MoveDone(lbl_1_bss_4) == 0) {
            lbl_1_bss_0 = 0;
            M570MoveDel(lbl_1_bss_4);
            lbl_1_bss_50 = 15;
            fn_1_5354();
            fn_1_5820(3);
            lbl_1_bss_44 = HuAudFXPlayVol(0x39C, 0x60);
            fn_1_5F24(20);
            MgSeqModeNext();
        }
        break;
    }
}

void fn_1_F54(s16 mode, s16 frame)
{
    s32 i;

    if (frame == 0) {
        lbl_1_bss_50 = 17;
    }
    if (frame == 100) {
        for (i = 0; i < 4; i++) {
            omVibrate(i, 60, 12, 0);
        }
        HuAudFXPlay(0x914);
        Hu3DModelAttrReset(lbl_1_bss_A0[11], 1);
        Hu3DModelAttrReset(lbl_1_bss_A0[12], 1);
        Hu3DModelAttrReset(lbl_1_bss_A0[11], 0x40000002);
        Hu3DModelAttrReset(lbl_1_bss_A0[12], 0x40000002);
    }
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

// NONMATCHING: remaining diff is literal-pool/register naming only
void fn_1_1174(void)
{
    s32 i;
    HUSPRGRPID esp;

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

// NONMATCHING: remaining diff is literal-pool/register naming only
static void fn_1_1354(OMOBJ *obj)
{
    f32 tpLvl;
    s32 i;

    obj->work[0]++;
    tpLvl = kOne - kShadowAlpha * (f32)obj->work[0];
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

// NONMATCHING: remaining diff is literal-pool/register naming only
static void fn_1_14A8(OMOBJ *obj)
{
    s16 espId;
    f32 scale;
    f32 t;

    espId = (s16)obj->work[0];
    if (obj->work[1] == 0) {
        espDispOn((s16)obj->work[0]);
    }
    t = (f32)sin(M570_RAD(k90 * (f32)obj->work[1] / kShadowNear));
    scale = kOne + (kShadowAngle - kShadowAngle * t);
    espScaleSet(espId, scale, scale);
    espTPLvlSet(espId, t);
    obj->work[1]++;
    if ((u32)obj->work[1] > 60) {
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
    obj->work[0]--;
    if (obj->work[0] == 0) {
        Hu3DParManKill((s16)obj->work[1]);
        omDelObjEx(lbl_1_bss_108, obj);
    }
}

s16 fn_1_16D4(HuVecF *pos)
{
    HU3DPARMANID parManId;
    OMOBJ *obj;
    void *data;
    ANIMDATA *anim;

    data = HuDataSelHeapReadNum(0x210002, HU_MEMNUM_OVL, HEAP_MODEL);
    anim = HuSprAnimRead(data);
    parManId = Hu3DParManCreate(anim, 100, (HU3DPARMANPARAM *)&gParManParam);
    Hu3DParManPosSet(parManId, pos->x, pos->y, pos->z);
    Hu3DParManVecSet(parManId, kZero, kZero, kOne);
    Hu3DParManTimeLimitSet(parManId, 30);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(parManId), 3);
    obj = omAddObjEx(lbl_1_bss_108, 0x1300, 0, 0, OM_OBJ_NONE, fn_1_1674);
    obj->work[0] = 30;
    obj->work[1] = parManId;
    return parManId;
}

s16 fn_1_17DC(HuVecF *pos)
{
    HuVecF screen;
    s16 pan;

    Hu3D3Dto2D(pos, (s16)lbl_1_bss_54[0], &screen);
    pan = (s16)(kPanBase + kPanScale * (screen.x / gScreenHalf[0]));
    return pan;
}

void fn_1_1864(HuVecF *pos, s16 seId)
{
    HuAudFXPlayPan(seId, fn_1_17DC(pos));
}

void fn_1_1910(void)
{
    lbl_1_bss_108 = omInitObjMan(200, 0x1000);
    lbl_1_bss_104 = -1;
    omGameSysInit(lbl_1_bss_108);
}

M570CAM *fn_1_1964(M570CAM *src)
{
    M570CAM *cam;

    cam = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M570CAM), HU_MEMNUM_OVL);
    *cam = *src;
    Hu3DCameraCreate(src->camNo);
    Hu3DCameraViewportSet(src->camNo, kZero, kZero, kScreenW, kScreenH, kZero, kOne);
    Hu3DCameraPerspectiveSet(src->camNo, src->fov, src->nearZ, src->farZ, kOnePoint2);
    fn_1_1A50(src);
    return cam;
}

void fn_1_1A50(M570CAM *cam)
{
    if (cam->state == 1) {
        fn_1_2050(cam);
    }
    Hu3DCameraPosSetV(cam->camNo, &cam->pos, &cam->up, &cam->target);
}

// NONMATCHING: remaining diff is literal-pool/register naming only
s16 fn_1_1AA0(M570LIGHT *light)
{
    HU3DLIGHTID lightId;

    lightId = Hu3DGLightCreateV(&light->pos, &light->dir, &light->color);
    Hu3DGLightStaticSet(lightId, TRUE);
    Hu3DGLightInfinitytSet(lightId);
    return lightId;
}

static void fn_1_1AFC(OMOBJ *obj)
{
    M570CAM *cam = (M570CAM *)obj->work[0];

    if (HuPadBtn[0] & 0x800) {
        if (HuPadBtnDown[0] & 0x1000) {
            *cam = *(M570CAM *)lbl_1_bss_BC;
        }
        if (HuPadBtnDown[0] & 0x200) {
            obj->work[1]++;
            if (obj->work[1] == 2) {
                obj->work[1] = 0;
            }
        }
        switch (obj->work[1]) {
        case 0:
            cam->pos.z = cam->pos.z - kOnePoint1 * (f32)HuPadTrigL[0];
            cam->pos.z = cam->pos.z + kOnePoint1 * (f32)HuPadTrigR[0];
            cam->pos.x = cam->pos.x + kOnePoint1 * (f32)HuPadStkX[0];
            cam->pos.y = cam->pos.y + kOnePoint1 * (f32)HuPadStkY[0];
            fn_1_2714(cam);
            break;
        case 1:
            cam->rot.x = cam->rot.x - kOnePoint1 * (f32)HuPadStkY[0];
            cam->rot.y = cam->rot.y + kOnePoint1 * (f32)HuPadStkX[0];
            cam->dist = cam->dist - kOnePoint1 * (f32)HuPadTrigL[0];
            cam->dist = cam->dist + kOnePoint1 * (f32)HuPadTrigR[0];
            fn_1_2050(cam);
            break;
        }
        if (HuPadBtn[0] & 0x400) {
            cam->target.z = cam->target.z - kOnePoint1 * (f32)HuPadSubStkY[0];
        } else {
            cam->target.x = cam->target.x + kOnePoint1 * (f32)HuPadSubStkX[0];
            cam->target.y = cam->target.y + kOnePoint1 * (f32)HuPadSubStkY[0];
        }
        Hu3DCameraPosSetV(cam->camNo, &cam->pos, &cam->up, &cam->target);
    }
}

// NONMATCHING: remaining diff is literal-pool/register naming only
void fn_1_1F74(M570CAM *cam)
{
    OMOBJ *obj;

    obj = omAddObjEx(lbl_1_bss_108, 0x4000, 0, 0, OM_OBJ_NONE, fn_1_1AFC);
    obj->work[0] = (uintptr_t)cam;
    if (cam->state == 0) {
        obj->work[1] = 0;
    } else {
        obj->work[1] = 1;
    }
    *(M570CAM *)lbl_1_bss_BC = *cam;
}

void fn_1_2020(M570CAM *src)
{
    *(M570CAM *)lbl_1_bss_BC = *src;
}

void fn_1_2050(M570CAM *cam)
{
    HuVecF axis;
    f32 rx;
    f32 ry;
    f32 rz;
    f32 ux;
    f32 uy;
    f32 uz;

    rx = cam->rot.x;
    ry = cam->rot.y;
    rz = cam->rot.z;
    cam->pos.x = cam->target.x + cam->dist * (sin(M570_RAD(ry)) * cos(M570_RAD(rx)));
    cam->pos.y = cam->target.y + cam->dist * -sin(M570_RAD(rx));
    cam->pos.z = cam->target.z + cam->dist * (cos(M570_RAD(ry)) * cos(M570_RAD(rx)));
    PSVECSubtract(&cam->pos, &cam->target, &axis);
    PSVECNormalize(&axis, &axis);
    ux = sin(M570_RAD(ry)) * sin(M570_RAD(rx));
    uy = cos(M570_RAD(rx));
    uz = cos(M570_RAD(ry)) * sin(M570_RAD(rx));
    cam->up.x = ux * ((axis.x * axis.x) + (1.0f - axis.x * axis.x) * cos(M570_RAD(rz)))
                + uy * ((axis.x * axis.y) * (1.0 - cos(M570_RAD(rz))) - axis.z * sin(M570_RAD(rz)))
                + uz * ((axis.x * axis.z) * (1.0 - cos(M570_RAD(rz))) + axis.y * sin(M570_RAD(rz)));
    cam->up.y = uy * ((axis.y * axis.y) + (1.0f - axis.y * axis.y) * cos(M570_RAD(rz)))
                + ux * ((axis.x * axis.y) * (1.0 - cos(M570_RAD(rz))) + axis.z * sin(M570_RAD(rz)))
                + uz * ((axis.y * axis.z) * (1.0 - cos(M570_RAD(rz))) - axis.x * sin(M570_RAD(rz)));
    cam->up.z = uz * ((axis.z * axis.z) + (1.0f - axis.z * axis.z) * cos(M570_RAD(rz)))
                + (ux * ((axis.x * axis.z) * (1.0 - cos(M570_RAD(rz))) - axis.y * sin(M570_RAD(rz)))
                   + uy * ((axis.y * axis.z) * (1.0 - cos(M570_RAD(rz))) + axis.x * sin(M570_RAD(rz))));
    PSVECNormalize(&cam->up, &cam->up);
}

void fn_1_2714(M570CAM *cam)
{
    f32 m[4][4];
    f32 dx;
    f32 dy;
    f32 dz;
    f32 ang;
    f32 w;
    f32 vx;
    f32 vy;
    s32 i;
    s32 j;

    dx = cam->pos.x - cam->target.x;
    dy = cam->pos.y - cam->target.y;
    dz = cam->pos.z - cam->target.z;
    cam->dist = sqrtf(dz * dz + (dx * dx + dy * dy));
    ang = (f32)(gD180 * (atan2(dz, dx) / gPi));
    cam->rot.y = -(ang - k90);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            m[i][j] = kZero;
        }
    }
    m[0][0] = m[2][2] = (f32)cos(M570_RAD(-ang));
    m[2][0] = (f32)-sin(M570_RAD(-ang));
    m[0][2] = (f32)sin(M570_RAD(-ang));
    m[1][1] = m[3][3] = kOne;
    vx = dz * m[2][0] + (dx * m[0][0] + dy * m[1][0]);
    vy = dz * m[2][1] + (dx * m[0][1] + dy * m[1][1]);
    w = dz * m[2][3] + (dx * m[0][3] + dy * m[1][3]);
    cam->rot.x = -(gD180 * (atan2(vy, vx) / gPi));
    cam->rot.z = kZero;
}

void fn_1_2B18(OMOBJ *obj)
{
    M570CAM *from;
    M570CAM *cam;
    M570CAM *to;
    M570MOVE *mv;
    f32 t;

    mv = (M570MOVE *)obj->data;
    if ((mv->flags & 2) != 0) {
        cam = mv->src;
        from = &mv->from_cam;
        to = &mv->to;
        mv->accum += kOne;
        t = mv->accum / mv->progress;
        if (cam->state == 0) {
            cam->pos.x = from->pos.x + t * (to->pos.x - from->pos.x);
            cam->pos.y = from->pos.y + t * (to->pos.y - from->pos.y);
            cam->pos.z = from->pos.z + t * (to->pos.z - from->pos.z);
        } else {
            cam->dist = from->dist + t * (to->dist - from->dist);
            cam->rot.x = from->rot.x + t * (to->rot.x - from->rot.x);
            cam->rot.y = from->rot.y + t * (to->rot.y - from->rot.y);
        }
        cam->target.x = from->target.x + t * (to->target.x - from->target.x);
        cam->target.y = from->target.y + t * (to->target.y - from->target.y);
        cam->target.z = from->target.z + t * (to->target.z - from->target.z);
        if (mv->accum >= mv->progress) {
            *cam = *to;
            mv->flags &= ~2;
        }
        fn_1_1A50(cam);
    }
}

// QUEUE a camera move between two descriptors. Refuses when the two sides name
// different cameras, otherwise allocates the move object, seeds both endpoints
// and pre-transforms them when the source camera is already live.
OMOBJ *MgCameraMoveEntry(M570CAM *from, M570CAM *to, f32 arg2)
{
    OMOBJ *obj;
    M570MOVE *mv;

    if (from->camNo != to->camNo) {
        OSReport(gStrCamDiff);
#ifdef TARGET_PC
        // the GC code returns whatever OSReport left in r3
        return NULL;
#else
        return;
#endif
    }
    obj = omAddObjEx(lbl_1_bss_108, 0x2000, 0, 0, -1, fn_1_2B18);
    obj->data = HuMemDirectMallocNum(0, sizeof(M570MOVE), HU_MEMNUM_OVL);
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
    return obj;
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

    if ((mv->flags & 2) != 0) {
        return 1;
    }
    return 0;
}

/* 0x00 src, 0x04 from, 0x4C to, 0x94 progress, 0x98 accum, 0x9C flags. */

void fn_1_2E98(OMOBJ *obj)
{
    M570CAM *cam;
    s32 range;

    cam = (M570CAM *)obj->data;
    range = obj->work[1];
    if (obj->work[0] == 0) {
        fn_1_1A50((M570CAM *)obj->work[2]);
        omDelObjEx(lbl_1_bss_108, obj);
        return;
    }
    cam->target.x = ((M570CAM *)obj->work[2])->target.x + (f32)frandmod(range * 2) - (f32)range;
    cam->target.y = ((M570CAM *)obj->work[2])->target.y + (f32)frandmod(range * 2) - (f32)range;
    fn_1_1A50(cam);
    obj->work[0]--;
}

void fn_1_2FE0(M570CAM *cam, s32 frames, s32 range)
{
    OMOBJ *obj;
    M570CAM *dst;

    obj = omAddObjEx(lbl_1_bss_108, 0x2000, 0, 0, OM_OBJ_NONE, fn_1_2E98);
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M570CAM), HU_MEMNUM_OVL);
    dst = (M570CAM *)obj->data;
    obj->work[0] = frames;
    obj->work[1] = range;
    *dst = *cam;
    obj->work[2] = (uintptr_t)cam;
}

f32 fn_1_309C(s16 pad)
{
    f32 mag;

    mag = (f32)(HuPadStkX[pad] * HuPadStkX[pad] + HuPadStkY[pad] * HuPadStkY[pad]);
    return sqrtf(mag);
}

f32 fn_1_3220(s16 pad)
{
    f32 ang;

    ang = (f32)(gD180 * (atan2((f64)(f32)HuPadStkY[pad], (f64)(f32)HuPadStkX[pad]) / gPi) - gD90);
    if (ang < kZero) {
        ang += k360;
    }
    return ang;
}

static void fn_1_33E0(OMOBJ *obj);

static void fn_1_3330(OMOBJ *obj)
{
    M570FLOW *flow;

    flow = (M570FLOW *)obj->data;
    if (omSysExitReq != 0) {
        if (lbl_1_bss_104 != -1) {
            HuAudFadeOut(1000);
            lbl_1_bss_104 = -1;
        }
        obj->objFunc = fn_1_33E0;
        WipeCreate(2, 6, 60);
        return;
    }
    flow->tbl[flow->idx](obj);
}

static void fn_1_33E0(OMOBJ *obj)
{
    if (WipeCheckEnd() == 0) {
        omOvlReturnEx(1, 1);
    }
}

OMOBJ *fn_1_3418(M570FLOWFUNC *tbl)
{
    M570FLOW *flow;
    OMOBJ *obj;
    s32 i;
    s32 count;

    count = 0;
    while (tbl[count] != NULL) {
        count++;
    }
    obj = omAddObjEx(lbl_1_bss_108, 0x2000, 0, 0, OM_OBJ_NONE, fn_1_3330);
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M570FLOW), HU_MEMNUM_OVL);
    flow = (M570FLOW *)obj->data;
    flow->count = count;
    flow->tbl = tbl;
    flow->idx = 0;
    flow->mesId = -1;
    for (i = 0; i < 16; i++) {
        flow->work[i] = 0;
    }
    return obj;
}

void MgFlowSet(OMOBJ *obj, s32 idx)
{
    M570FLOW *flow;

    flow = (M570FLOW *)obj->data;
    if (idx >= flow->count) {
        OSReport(gStrFlowMax);
        return;
    }
    flow->idx = idx;
}

void MgFlowNext(OMOBJ *obj)
{
    M570FLOW *flow;

    flow = (M570FLOW *)obj->data;
    if (flow->idx == flow->count - 1) {
        OSReport(gStrFlowNext);
        return;
    }
    flow->idx++;
}

s32 fn_1_35B8(OMOBJ *obj)
{
    M570FLOW *flow;

    flow = (M570FLOW *)obj->data;
    return flow->idx;
}

s32 fn_1_35D4(OMOBJ *obj, s32 idx)
{
    M570FLOW *flow;

    flow = (M570FLOW *)obj->data;
    if (idx == fn_1_35B8(obj)) {
        return 1;
    }
    return 0;
}

void fn_1_3620(OMOBJ *obj, s32 type, s32 maxTime, s32 endTime, s32 speed)
{
    M570FLOW *flow;

    flow = (M570FLOW *)obj->data;
    flow->timerType = type;
    flow->timerMax = maxTime * 60;
    flow->timerEnd = endTime * 60;
    flow->timerSpeed = speed * 60;
}

BOOL fn_1_3654(OMOBJ *obj)
{
    M570FLOW *flow;

    flow = (M570FLOW *)obj->data;
    return MgTimerDoneCheck(flow->timer);
}

void fn_1_3690(OMOBJ *obj)
{
    M570FLOW *flow;

    flow = (M570FLOW *)obj->data;
    if (flow->timer->stopF == 0) {
        flow->timer->stopF = 1;
        flow->timer->mode = 1;
    }
}

void fn_1_36D0(void)
{
}

void fn_1_36D4(OMOBJ *obj)
{
    if (WipeCheckEnd() == 0) {
        MgFlowNext(obj);
    }
}

void fn_1_374C(void)
{
    if (lbl_1_bss_104 != -1) {
        HuAudFadeOut(1000);
        lbl_1_bss_104 = -1;
    }
    if (WipeCheckEnd() == 0) {
        omOvlReturnEx(1, 1);
    }
}

void fn_1_37B0(OMOBJ *obj, s16 mesNo, s16 type)
{
    M570FLOW *flow;

    flow = (M570FLOW *)obj->data;
    if (flow->mesId == -1) {
        flow->mesId = GameMesCreate(mesNo, type);
        return;
    }
    if (GameMesStatGet(flow->mesId) == 0) {
        flow->mesId = -1;
        MgFlowNext(obj);
    }
}

void fn_1_386C(OMOBJ *obj)
{
    M570FLOW *flow;

    flow = (M570FLOW *)obj->data;
    if (flow->mesId == -1) {
        flow->mesId = GameMesCreate(2, 0);
        return;
    }
    if (GameMesStatGet(flow->mesId) == 0) {
        flow->mesId = -1;
        MgFlowNext(obj);
    }
}

void fn_1_3920(OMOBJ *obj)
{
    M570FLOW *flow;

    flow = (M570FLOW *)obj->data;
    if (flow->mesId == -1) {
        flow->mesId = GameMesCreate(2, 1);
        return;
    }
    if (GameMesStatGet(flow->mesId) == 0) {
        flow->mesId = -1;
        MgFlowNext(obj);
    }
}

void fn_1_39D4(OMOBJ *obj)
{
    M570FLOW *flow;

    flow = (M570FLOW *)obj->data;
    flow->timer = MgTimerCreate(flow->timerType);
    MgTimerParamSet(flow->timer, flow->timerMax, flow->timerEnd, flow->timerSpeed);
    MgTimerModeOnSet(flow->timer, 1);
    MgFlowNext(obj);
}

void fn_1_3A74(M570PLAYER *player, s16 idx)
{
    player->objId = idx;
    player->charNo = GwPlayerConf[idx].charNo;
    player->padNo = GwPlayerConf[idx].padNo;
    player->grp = GwPlayerConf[idx].grp;
    player->unkC = GwPlayerConf[idx].type;
    player->unk8 = GwPlayerConf[idx].dif;
    player->state = 0;
    player->tra.x = player->tra.y = player->tra.z = kZero;
    player->rot.x = player->rot.y = player->rot.z = kZero;
}

// NONMATCHING: remaining diff is literal-pool/register naming only
s32 fn_1_3B40(HuVecF *pos, HuVecF *goal, HuVecF *angle, f32 step)
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
        *pos = *goal;
        return 1;
    }
    PSVECAdd(pos, &dir, pos);
    angle->y = (f32)(gD180 * (atan2(dir.x, dir.z) / gPi));
    return 0;
}

// NONMATCHING: the original keeps the fmod/fabs results in extra FPRs (frame 0x80 vs 0x60); register allocation only
f32 fn_1_3C64(f32 val, f32 min, f32 max)
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

s32 fn_1_3DD4(HuVecF *pos, HuVecF *goal, HuVecF *from, f32 dist)
{
    HuVecF dir;
    HuVecF step;
    HuVecF saved;
    f32 len;
    f32 ang;

    saved = *goal;
    for (;;) {
        PSVECSubtract(goal, pos, &dir);
        if (PSVECMag(&dir) == kZero) {
            return 0;
        }
        PSVECSubtract(goal, from, &step);
        len = PSVECMag(&step);
        if (len < dist) {
            if (len != kZero) {
                PSVECNormalize(&step, &step);
                PSVECScale(&step, &step, dist);
                PSVECAdd(from, &step, goal);
            } else {
                *goal = *pos;
            }
            PSVECSubtract(goal, pos, &step);
            if (PSVECMag(&step) < kSmall) {
                len = PSVECMag(&dir);
                ang = (f32)(gD180 * (atan2(step.z, step.x) / gPi));
                ang += k30;
                dir.x = len * sin(M570_RAD(ang));
                dir.z = len * cos(M570_RAD(ang));
                PSVECAdd(pos, &dir, goal);
                continue;
            }
            return 1;
        }
        return 0;
    }
}

void fn_1_400C(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *out)
{
    HuVecF v1;
    HuVecF v2;

    PSVECSubtract(c, b, &v1);
    PSVECSubtract(b, a, &v2);
    PSVECCrossProduct(&v1, &v2, out);
    PSVECNormalize(out, out);
}

f32 fn_1_4084(HuVecF *n, HuVecF *p0, HuVecF *p)
{
    HuVecF d;

    PSVECSubtract(p, p0, &d);
    return PSVECDotProduct(n, &d);
}

s32 fn_1_40C8(HuVecF *n, HuVecF *p0, HuVecF *a, HuVecF *b, HuVecF *out)
{
    HuVecF d;
    f32 num;
    f32 den;

    PSVECSubtract(b, a, &d);
    num = fn_1_4084(n, p0, a);
    den = fn_1_4084(n, b, a);
    if (den == kZero) {
        return 0;
    }
    PSVECScale(&d, &d, num / den);
    PSVECAdd(a, &d, out);
    return 1;
}
