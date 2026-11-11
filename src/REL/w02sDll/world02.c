#include "game/board/main.h"
#include "game/board/effect.h"
#include "game/board/audio.h"
#include "game/board/scroll.h"
#include "game/board/model.h"
#include "game/board/masu.h"
#include "game/board/opening.h"
#include "game/board/capsule.h"
#include "game/board/branch.h"
#include "game/board/camera.h"
#include "game/board/player.h"
#include "game/board/pad.h"
#include "game/board/window.h"
#include "game/board/coin.h"
#include "game/board/star.h"
#include "game/board/com.h"

#include "game/sprite.h"
#include "game/wipe.h"
#include "game/gamemes.h"
#include "game/pad.h"
#include "game/main.h"
#include "game/object.h"
#include "game/charman.h"
#include "game/hsfex.h"

#include <math.h>
#include "game/gxarray.h"

#ifdef TARGET_PC
/* __frsqrte is a PowerPC compiler intrinsic with no host equivalent; every
 * call site below only uses it to seed the same 3-iteration Newton-Raphson
 * refinement (see w07Dll's MB7_FastSqrt for the identical pattern), so an
 * exact 1/sqrt(x) seed converges to the same result. */
static double mb2_frsqrte(double x) {
    return 1.0 / sqrt(x);
}
#define __frsqrte mb2_frsqrte
#endif

/* w02sDll - Rainbow Dream board (secondary module). */

s32 MB2_MasuLinkGet(s16 arg0, s16 *arg1, s32 arg2, s32 arg3);
static HSFOBJECT *objCall(HU3DMODEL *arg0, char *arg1, HSFOBJECT *arg2);
void ParticleDraw(HU3DMODEL *modelP, f32 (*mtx)[3][4]);
void RainbowObjDraw(HU3DMODEL *modelP, f32 (*mtx)[3][4]);
void ObjectSetup(void);
void MB2_Create(void);
void MB2_Kill(void);
void MB2_MainObjUpdate(OMOBJ *obj);
void MB2_LightSet(void);
void MB2_LightReset(void);
int MB2Ev_MasuHatena(int playerNo);
void MB2_Stub(void);
int MB2Ev_MasuWalkPre(int playerNo);
int MB2Ev_MasuWalkPost(int playerNo);
int MB2_MasuPathCheck(s16 id, u32 flag, s16 *linkTbl, BOOL endF);
s32 MB2Ev_Rainbow(s32 arg0, s32 arg1);
void MB2Ev_HatenaTaiyo(s32 playerNo);
s16 MB2_RainbowMasuNoEndGet(s32 arg0);
void MB2_CoinDispUpdate(s32 arg0, s32 arg1, s32 arg2);
f32 MB2_AngleDiff(f32 farg0, f32 farg1);
f32 MB2_AngleLerp(f32 farg0, f32 farg1, f32 farg2);
f32 fn_1_42FC(f32 farg0, f32 farg1, f32 farg2);
HSFOBJECT *MB2_ModelObjPtrGet(HU3DMODEL *arg0, char *arg1);
f32 MB2_BezierCalc(f32 t, f32 p0, f32 p1, f32 p2);
void MB2_BezierCalcV(f32 *arg0, f32 *arg1, f32 *arg2, f32 *arg3, f32 t);
void MB2_HermiteCalc(f32 *arg0, f32 *arg1, f32 *arg2, f32 *arg3, f32 farg0);
f32 fn_1_5260(f32 t, f32 p0, f32 p1, f32 p2, f32 p3, f32 tension);
void MB2_HermiteCalcV(Point3d *arg0, Point3d *arg1, Point3d *arg2, Point3d *arg3, Point3d *arg4, f32 farg0);
void MB2_CoinInit(void);
void MB2_CoinExec(OMOBJ *obj);
void MB2_CoinClose(void);
s32 MB2_CoinNumGet(void);
s32 MB2_CoinCreate(Point3d *arg0, Point3d *arg1, s32 arg2, f32 farg0);
void MB2_CoinEffInit(void);
void MB2_CoinEffUpdate(OMOBJ *obj);
void MB2_CoinEffClose(void);
s32 MB2_CoinEffNumGet(void);
void MB2_CoinEffBlendModeSet(s32 arg0);
s32 MB2_CoinEffCreate(f32 *arg0, f32 *arg1, u8 *arg2, f32 farg0, f32 farg1, f32 farg2);
s32 MB2_CoinEffCreateRand(void *arg0, s32 arg1, f32 farg0, f32 farg1, f32 farg2, f32 farg3, f32 farg4);
s32 MB2_CoinEffCreateColRand(f32 *arg0, s32 arg1, f32 farg0, f32 farg1, f32 farg2, f32 farg3, f32 farg4);
void MB2_SplashEffInit(void);
void MB2_SplashEffUpdate(OMOBJ *obj);
void MB2_SplashEffClose(void);
s32 MB2_SplashEffNumGet(void);
void MB2_SplashEffCreate(HuVecF *arg0, HuVecF *arg1, u8 *arg2, s32 arg3, s32 arg4, f32 farg0, f32 farg1, f32 farg2);
void MB2_SplashEffCreateRandX(HuVecF *arg0, u8 *arg1, s32 arg2, f32 farg0);
s16 MB2_ParticleCreate(ANIMDATA *arg0, s16 arg1);
s16 MB2_ParticleCopy(s16 arg0);
void MB2_YajiInit(Point3d *pos);
void MB2_YajiUpdate(OMOBJ *obj);
void MB2_YajiClose(void);
void MB2_YajiHiliteSet(u32 arg0);
void MB2_YajiDispSet(s32 arg0);
void MB2_RainbowObjInit(void);
void MB2_RainbowObjUpdate(OMOBJ *obj);
void MB2_RainbowObjClose(void);
s32 MB2_RainbowObjCreate(Point3d *arg0, s32 arg1, s32 arg2);
void MB2_RainbowObjAlphaSet(s32 arg0, s8 arg1);
s32 MB2_RainbowObjVtxTopGet(s32 arg0, void **arg1);
typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];
static Point3d mb2ev_Rainbow1VtxTbl[0xA] = {
    { 1200.0f, 1318.0f, 4600.0f },
    { 1139.5439453125f, 1360.8270263671875f, 4500.419921875f },
    { 1077.9530029296875f, 1400.10302734375f, 4400.10009765625f },
    { 1014.083984375f, 1431.4510498046875f, 4298.4599609375f },
    { 946.9959716796875f, 1448.8499755859375f, 4195.669921875f },
    { 876.7360229492188f, 1444.4830322265625f, 4093.81005859375f },
    { 805.4639892578125f, 1414.5699462890625f, 3996.989990234375f },
    { 735.5139770507812f, 1365.81103515625f, 3906.93994140625f },
    { 667.239013671875f, 1306.3809814453125f, 3822.080078125f },
    { 600.0f, 1242.0f, 3740.0f },
};
static Point3d mb2ev_TaiyoRainbow1VtxTbl[0x14] = {
    { -200.0f, 2490.23804f, 2385.0f },
    { -279.208008f, 2511.43896f, 2455.77002f },
    { -358.641998f, 2531.49805f, 2526.62012f },
    { -438.518005f, 2549.18799f, 2597.6001f },
    { -519.031006f, 2562.94604f, 2668.72998f },
    { -600.27002f, 2570.94409f, 2739.87988f },
    { -682.120972f, 2570.85498f, 2810.72998f },
    { -764.052002f, 2559.8501f, 2880.51001f },
    { -844.828979f, 2534.80493f, 2947.83008f },
    { -922.260986f, 2492.97998f, 3010.52002f },
    { -993.385986f, 2433.29102f, 3065.87988f },
    { -1056.13599f, 2358.41406f, 3112.41992f },
    { -1112.06494f, 2274.70703f, 3152.13989f },
    { -1162.33606f, 2185.14697f, 3186.3501f },
    { -1208.15002f, 2091.7251f, 3216.30005f },
    { -1250.56897f, 1995.74097f, 3243.04004f },
    { -1290.479f, 1898.05798f, 3267.43994f },
    { -1328.61597f, 1799.26904f, 3290.17993f },
    { -1365.60498f, 1699.80298f, 3311.86011f },
    { -1402.0f, 1600.0f, 3333.0f },
};
static Point3d mb2ev_TaiyoRainbow2VtxTbl[0x14] = {
    { -0.0f, 2490.23804f, 2385.0f },
    { -0.0f, 2519.45996f, 2516.8501f },
    { -0.0f, 2547.39111f, 2648.98999f },
    { -0.0f, 2572.72998f, 2781.6499f },
    { -0.0f, 2593.91797f, 2915.02002f },
    { -0.0f, 2608.95508f, 3049.21997f },
    { -0.0f, 2615.1731f, 3184.09009f },
    { -0.0f, 2608.24707f, 3318.88989f },
    { -0.0f, 2581.50293f, 3451.02002f },
    { -0.0f, 2530.42798f, 3575.48999f },
    { -0.0f, 2452.4751f, 3685.08008f },
    { -0.0f, 2351.5979f, 3774.37988f },
    { -0.0f, 2236.58691f, 3844.92993f },
    { -0.0f, 2114.448f, 3902.34009f },
    { -0.0f, 1988.12305f, 3949.94995f },
    { -0.0f, 1859.30701f, 3990.42993f },
    { -0.0f, 1728.98596f, 4025.84009f },
    { -0.0f, 1597.75806f, 4057.75f },
    { -0.0f, 1466.005f, 4087.43994f },
    { 0.0f, 1334.0f, 4116.0f },
};
static Point3d mb2ev_TaiyoRainbow3VtxTbl[0x14] = {
    { 200.0f, 2490.23804f, 2385.0f },
    { 277.376007f, 2510.95288f, 2454.1499f },
    { 354.923004f, 2530.59302f, 2523.41992f },
    { 432.791992f, 2548.03491f, 2592.90991f },
    { 511.100006f, 2561.9751f, 2662.69995f },
    { 589.892029f, 2570.77197f, 2732.78003f },
    { 669.044006f, 2571.66309f, 2802.95996f },
    { 747.955017f, 2561.97412f, 2872.63989f },
    { 825.297974f, 2538.33594f, 2940.55005f },
    { 898.677979f, 2497.60889f, 3004.47998f },
    { 964.953003f, 2438.59399f, 3061.61011f },
    { 1021.66101f, 2363.58691f, 3109.82007f },
    { 1070.11902f, 2278.85205f, 3150.45996f },
    { 1112.203f, 2188.29492f, 3185.28003f },
    { 1149.34998f, 2094.01489f, 3215.62988f },
    { 1182.79199f, 1997.33997f, 3242.62988f },
    { 1213.53003f, 1899.11597f, 3267.18994f },
    { 1242.37402f, 1799.90405f, 3290.04004f },
    { 1270.0f, 1700.09802f, 3311.80005f },
    { 1297.0f, 1600.0f, 3333.0f },
};













static Point3d openingRot = { -33.0f, 0.0f, 0.0f };
static Point3d openingOfs = { 266.0f, 2095.0f, 4317.0f };
static f32 openingZoom = 7500.0f;
static Point3d mapViewRot = { -78.0f, 0.0f, 0.0f };
static Point3d mapViewPos = { 0.0f, -664.0f, 2625.0f };
static char lbl_1_data_37C[0xA] = "s_cloud00";
static s32 mb2ev_RainbowNoTbl[0xB] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static s32 mb2ev_RainbowDirTbl[0xB] = { 0, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1 };
static void *mb2ev_RainbowVtxTbl[5] = {
    &mb2ev_Rainbow1VtxTbl,
    &mb2ev_Rainbow1VtxTbl,
    &mb2ev_Rainbow1VtxTbl,
    &mb2ev_Rainbow1VtxTbl,
    &mb2ev_Rainbow1VtxTbl,
};
static s32 mb2ev_RainbowVtxNumTbl[5] = { 0xA, 0xA, 0xA, 0xA, 0xA };
static s32 mb2ev_RainbowRevF[5] = { 0, 0, 0, 0, 1 };
static void *mb2ev_TaiyoRainbowVtxTbl[3][3] = {
    { &mb2ev_TaiyoRainbow1VtxTbl, &mb2ev_TaiyoRainbow2VtxTbl, &mb2ev_TaiyoRainbow3VtxTbl },
    { &mb2ev_TaiyoRainbow1VtxTbl, &mb2ev_TaiyoRainbow2VtxTbl, &mb2ev_TaiyoRainbow3VtxTbl },
    { &mb2ev_TaiyoRainbow1VtxTbl, &mb2ev_TaiyoRainbow2VtxTbl, &mb2ev_TaiyoRainbow3VtxTbl },
};
static s32 mb2ev_TaiyoRainbowVtxNumTbl[3][3] = { { 0x14, 0x14, 0x14 }, { 0x14, 0x14, 0x14 }, { 0x14, 0x14, 0x14 } };
static s32 mb2ev_TaiyoDestTbl[3][3] = { { 0, 1, 2 }, { 0, 1, 2 }, { 0, 1, 2 } };
static Point3d mb2ev_TaiyoCoinBasePos = { 0.0f, 2940.0f, -860.0f };
static u8 mb2ev_CoinEffColTbl[7][4] = {
    { 0xFF, 0x7F, 0x7F, 0xFF },
    { 0xFF, 0x7F, 0x40, 0xFF },
    { 0xFF, 0xFF, 0x7F, 0xFF },
    { 0x7F, 0xFF, 0x7F, 0xFF },
    { 0x7F, 0x7F, 0xFF, 0xFF },
    { 0x40, 0x40, 0xFF, 0xFF },
    { 0xFF, 0x7F, 0xFF, 0xFF },
};
typedef struct MB2Work_s {
    s32 unk00;          /* 0x00 */
    s32 unk04;          /* 0x04 */
    s32 mdlMain;        /* 0x08 */
    s32 unk0C;          /* 0x0C */
    s32 unk10;          /* 0x10 */
    s32 mdl5;           /* 0x14 */
    s32 mdl6;           /* 0x18 */
    s32 mdlArr[4];      /* 0x1C */
    s32 mdlB;           /* 0x2C */
    s32 unk30[10];      /* 0x30 */
    s32 mdlRainbow;     /* 0x58 */
    s32 mdl17;          /* 0x5C */
    s32 unk60;          /* 0x60 */
    s32 unk64;          /* 0x64 */
    s32 mdlCloud;       /* 0x68 */
    ANIMDATA *anim;     /* 0x6C */
    struct MB2TaiyoSeg_s *seg; /* 0x70 */
    s32 lightId;        /* 0x74 */
    f32 lightPos0;      /* 0x78 */
    f32 lightPos1;      /* 0x7C */
    f32 lightPos2;      /* 0x80 */
    f32 lightPos3;      /* 0x84 */
    f32 lightPos4;      /* 0x88 */
    f32 lightPos5;      /* 0x8C */
    u8 color[4];        /* 0x90 */
    f32 lightRate;      /* 0x94 */
    s32 lightOnF;       /* 0x98 */
    s32 unk9C;          /* 0x9C */
    s32 unkA0;          /* 0xA0 */
    s32 unkA4;          /* 0xA4 */
    s32 angle[5];       /* 0xA8 */
} MB2_WORK;

static MB2_WORK mb2_Work;
static u32 *mb2_SaveWorkP;
static s32 mb2_TimerObj;
static s32 mb2_TimerValue;
static Point3d mb2_CoinMaxDist;
static OMOBJ *mb2_RainbowObj;
static OMOBJ *mb2_YajiObj;
static OMOBJ *mb2_SplashEffObj;
static OMOBJ *mb2_CoinEffObj;
static OMOBJ *mb2_CoinObj;
static OMOBJ *mb2_MainObj;

#define MB2_PARTICLE_ATTR_NONE 0
#define MB2_PARTICLE_ATTR_COUNTER_RESET (1 << 0)
#define MB2_PARTICLE_ATTR_COUNTER_UPDATE (1 << 1)

#define MB2_PARTICLE_DISPATTR_NONE 0
#define MB2_PARTICLE_DISPATTR_ZBUF_OFF (1 << 0)
#define MB2_PARTICLE_DISPATTR_NOANIM (1 << 1)
#define MB2_PARTICLE_DISPATTR_CAMERA_ROT (1 << 2)
#define MB2_PARTICLE_DISPATTR_ROT3D (1 << 3)
#define MB2_PARTICLE_DISPATTR_ALL 15

typedef struct MB1Particle_s MB2_PARTICLE;
typedef void (*MB2_PARTICLE_HOOK)(HU3DMODEL *modelP, MB2_PARTICLE *particleP, Mtx *matrix);

typedef struct MB1ParticleData_s {
    s16 time; //0x00
    s16 work; //0x02
    s16 mode; //0x04
    s16 cameraBit; //0x06
    HuVecF vel; //0x08
    float baseScale; //0x14
    float scaleFactor; //0x18
    float speed; //0x1C
    float unk20; //0x20
    float gravity; //0x24
    float unk28; //0x28
    float animTime; //0x2C
    float animSpeed; //0x30
    float scale; //0x34
    HuVecF rot; //0x38
    HuVecF pos; //0x44
    GXColor color; //0x50
    int animNo; //0x54
} MB2_PARTICLE_DATA;

struct MB1Particle_s {
    s16 mode; //0x00
    s16 time; //0x02
    HuVecF vel; //0x04
    s16 work[8]; //0x10
    u8 blendMode; //0x20
    u8 attr; //0x21
    u8 dispAttr; //0x22
    u8 unk23;
    HU3DMODELID modelId; //0x24
    s16 num; //0x26
    u32 count; //0x28
    u32 prevCounter; //0x2C
    u32 prevCount; //0x30
    u32 dlSize; //0x34
    ANIMDATA *anim; //0x38
    MB2_PARTICLE_DATA *data; //0x3C
    HuVecF *vertex; //0x40
    HuVec2f *st; //0x44
    void *dl; //0x48
    MB2_PARTICLE_HOOK hook; //0x4C
    HU3DMODEL *hookMdlP; //0x50
};


/* mb2_CoinObj->data: 0x20-entry array, stride 0x84 (coin drop slots). */
typedef struct MB2CoinSlot_s {
    s32 modelId;   /* 0x00 */
    s32 activeF;   /* 0x04, 0 = free slot */
    s32 unk8;
    f32 unkC;
    s32 unk10;
    s32 unk14;
    Point3d pos;    /* 0x18 */
    Point3d rot;    /* 0x24 */
    Point3d scale;  /* 0x30 */
    Point3d srcPos; /* 0x3C, saved starting position */
    Point3d dstPos; /* 0x48 */
    Point3d playerPos[4]; /* 0x54, one per player */
} MB2_COIN_SLOT; /* size 0x84 */

/* mb2_SplashEffObj->data: 2 splash-type model ids + their live-particle counts. */
typedef struct MB2SplashEffData_s {
    s32 modelId[2]; /* 0x00 */
    s32 count[2];   /* 0x08 */
    ANIMDATA *anim; /* 0x10 */
} MB2_SPLASHEFF_DATA; /* size 0x14 */



int _prolog(void) {
    const VoidFunc* ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void) {
    const VoidFunc* dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void) {
    GWPartyFSet(FALSE);
    MBObjectSetup(MBNO_WORLD_2, MB2_Create, MB2_Kill);
}

void MB2_Create(void) {
    s32 sp8;
    s16 temp_r3;
    s16 temp_r3_2;
    s16 temp_r3_3;

    sp8 = MBBoardNoGet();
    HuAudSndGrpSetSet(0xF);
    MBMasuCreate(0xCE0000);
    MBScrollInit(0xCE0015U, 18000.0f, -500.0f);
    temp_r3 = MBModelCreate(0xCE0007, NULL, 0);
    mb2_Work.mdlMain = temp_r3;
    MBModelAttrSet(temp_r3, 0x40000001U);
    mb2_Work.mdl5 = MBModelCreate(0xCE000E, NULL, 0);
    mb2_Work.mdl6 = MBModelCreate(0xCE0006, NULL, 0);
    temp_r3_2 = MBModelCreate(0xCE0014, NULL, 0);
    mb2_Work.mdlB = temp_r3_2;
    MBModelAttrSet(temp_r3_2, 0x40000001U);
    mb2_Work.mdlArr[0] = MBModelCreate(0xCE000F, NULL, 0);
    mb2_Work.mdlArr[1] = MBModelCreate(0xCE0010, NULL, 0);
    mb2_Work.mdlArr[2] = MBModelCreate(0xCE0011, NULL, 0);
    mb2_Work.mdlArr[3] = MBModelCreate(0xCE0012, NULL, 0);
    MBModelDispSet((s16) mb2_Work.mdlArr[0], 0);
    MBModelDispSet((s16) mb2_Work.mdlArr[1], 0);
    MBModelDispSet((s16) mb2_Work.mdlArr[3], 0);
    temp_r3_3 = MBModelCreate(0xCE0008, NULL, 0);
    mb2_Work.mdlRainbow = temp_r3_3;
    MBMotionSpeedSet(temp_r3_3, 0.0f);
    MBMotionTimeSet(temp_r3_3, 0.0f);
    mb2_Work.mdl17 = MBModelCreate(0xCE0009, NULL, 0);
    mb2_Work.mdlCloud = MBModelCreate(0xCE0013, NULL, 0);
    MBCapsuleColCreate(0xCE0016);
    mb2_Work.anim = HuSprAnimRead(HuDataReadNum(0xCE0017, 0x10000000));
    mb2_Work.seg = NULL;
    mb2_Work.lightId = -1;
    mb2_Work.lightRate = 1.0f;
    mb2_Work.lightOnF = 1;
    mb2_Work.unk9C = 1;
    mb2_Work.unkA0 = 1;
    mb2_Work.unkA4 = 0;
    mb2_Work.angle[0] = (s32) (360.0f * (0.007874016f * (f32) (frand() & 0x7F)));
    mb2_Work.angle[1] = (s32) (360.0f * (0.007874016f * (f32) (frand() & 0x7F)));
    mb2_Work.angle[2] = (s32) (360.0f * (0.007874016f * (f32) (frand() & 0x7F)));
    mb2_Work.angle[3] = (s32) (360.0f * (0.007874016f * (f32) (frand() & 0x7F)));
    mb2_Work.angle[4] = (s32) (360.0f * (0.007874016f * (f32) (frand() & 0x7F)));
    mb2_SaveWorkP = GwSystem.mbSaveWork;
    if (_CheckFlag(0x10001U) == 0) {
        *mb2_SaveWorkP = (frand() & 0x7FFF) % 3;
    }
    if ((s32) *mb2_SaveWorkP == 0) {
        *mb2_SaveWorkP = (frand() & 0x7FFF) % 3;
    }
    if ((s32) *mb2_SaveWorkP < 1) {
        *mb2_SaveWorkP = 1;
    } else if ((s32) *mb2_SaveWorkP > 3) {
        *mb2_SaveWorkP = 3;
    }
    MB2_SplashEffInit();
    HuDataDirClose(0xCE0000);
    MBBranchFlagSet(0x40U);
    MBLightHookSet(MB2_LightSet, MB2_LightReset);
    MBMasuWalkPreHookSet(MB2Ev_MasuWalkPre);
    MBMasuWalkPostHookSet(MB2Ev_MasuWalkPost);
    MBMasuHatenaHookSet(MB2Ev_MasuHatena);
    MBMasuPathCheckHookSet(MB2_MasuPathCheck);
    HuDataDirRead(0xB0000);
    mb2_MainObj = omAddObjEx(mbObjMan, -0x8000, 0U, 0U, -1, MB2_MainObjUpdate);
    MBOpeningViewSet(&openingRot, &openingOfs, openingZoom);
    MBMapViewCameraSet(&mapViewRot, &mapViewPos, 12202.0f);
}

void MB2_Kill(void) {
    mb2_MainObj = NULL;
    if (mb2_Work.anim != 0) {
        HuSprAnimKill(mb2_Work.anim);
    }
    mb2_Work.anim = NULL;
    if (mb2_Work.seg != 0) {
        HuMemDirectFree(mb2_Work.seg);
    }
    mb2_Work.seg = NULL;
    MB2_CoinClose();
    MB2_CoinEffClose();
    MB2_YajiClose();
    MB2_RainbowObjClose();
    MB2_SplashEffClose();
}

void MB2_MainObjUpdate(OMOBJ *obj) {
    MBCAMERA *camP;
    s32 i;
    HSFOBJECT *hsfObj;
    HU3DMODEL *modelP;
    s16 mdlId;
    f32 scaleX;
    f32 scaleY;
    f32 scaleZ;

    if ((MBKillCheck() != 0) || (mb2_MainObj == NULL)) {
        omDelObjEx(mbObjMan, mb2_MainObj);
        mb2_MainObj = NULL;
        return;
    }
    camP = MBCameraGet();
    for (i = 0; i < 4; i++) {
        mb2_Work.angle[i] += ((frand() & 0x7FFF) % 3) + 1;
        scaleX = (f32) (1.0 + (0.009999999776482582 * sin((3.141592653589793 * (f64) mb2_Work.angle[i]) / 180.0)));
        scaleY = (f32) (1.0 + (0.0024999999441206455 * sin((3.141592653589793 * (f64) mb2_Work.angle[i]) / 180.0)));
        scaleZ = (f32) (1.0 + (0.009999999776482582 * sin((3.141592653589793 * (f64) mb2_Work.angle[i]) / 180.0)));
        MBModelScaleSet((s16) mb2_Work.mdlArr[i], scaleX, scaleY, scaleZ);
    }
    mb2_Work.angle[4] += ((frand() & 0x7FFF) % 3) + 1;
    scaleX = (f32) (1.0 + (0.009999999776482582 * sin((3.141592653589793 * (f64) mb2_Work.angle[4]) / 180.0)));
    scaleY = (f32) (1.0 + (0.0024999999441206455 * sin((3.141592653589793 * (f64) mb2_Work.angle[4]) / 180.0)));
    scaleZ = (f32) (1.0 + (0.009999999776482582 * sin((3.141592653589793 * (f64) mb2_Work.angle[4]) / 180.0)));
    mdlId = (s16) MBModelIdGet((s16) mb2_Work.mdlCloud);
    modelP = &Hu3DData[mdlId];
    hsfObj = MB2_ModelObjPtrGet(modelP, lbl_1_data_37C);
    if (hsfObj) {
        hsfObj->mesh.base.scale.x = scaleX;
        hsfObj->mesh.base.scale.y = scaleY;
        hsfObj->mesh.base.scale.z = scaleZ;
    }
    if (mb2_Work.lightId != -1) {
        if (mb2_Work.lightOnF != 0) {
            if (mb2_Work.lightRate < 1.0f) {
                mb2_Work.lightRate += 0.03f;
                if (mb2_Work.lightRate > 1.0f) {
                    mb2_Work.lightRate = 1.0f;
                }
            }
        } else if (mb2_Work.lightRate > 0.0f) {
            mb2_Work.lightRate -= 0.03f;
            if (mb2_Work.lightRate < 0.0f) {
                mb2_Work.lightRate = 0.0f;
            }
        }
        mb2_Work.color[0] = mb2_Work.color[1] = mb2_Work.color[2] = mb2_Work.color[3] = (255.0f * mb2_Work.lightRate);
        Hu3DGLightColorSet((s16) mb2_Work.lightId, mb2_Work.color[0], mb2_Work.color[1], mb2_Work.color[2], mb2_Work.color[3]);
    }
}

void MB2_LightSet(void) {
    Hu3DShadowCreate(45.0f, 1000.0f, 250000.0f);
    Hu3DShadowTPLvlSet(0.3f);
    Hu3DBGColorSet(0xFFU, 0xFFU, 0xFFU);
    mb2_Work.lightPos0 = -3700.0f;
    mb2_Work.lightPos1 = 18100.0f;
    mb2_Work.lightPos2 = 21300.0f;
    mb2_Work.lightPos3 = 3700.0f;
    mb2_Work.lightPos4 = -18100.0f;
    mb2_Work.lightPos5 = -21300.0f;
    mb2_Work.color[0] = 0xFFU;
    mb2_Work.color[1] = 0xFFU;
    mb2_Work.color[2] = 0xFFU;
    mb2_Work.lightId = Hu3DGLightCreate(mb2_Work.lightPos0, mb2_Work.lightPos1, mb2_Work.lightPos2, mb2_Work.lightPos3, mb2_Work.lightPos4, mb2_Work.lightPos5, mb2_Work.color[0], mb2_Work.color[1], mb2_Work.color[2]);
    Hu3DGLightInfinitytSet((s16) mb2_Work.lightId);
}

void MB2_LightReset(void) {

}

int MB2Ev_MasuHatena(int playerNo) {
    MASU *masu;
    s16 masuId;
    u16 flag;

    masuId = GwPlayer[playerNo].masuId;
    masu = MBMasuGet(0, masuId);
    flag = masu->flag;
    if ((u32) (flag & 0x1) != 0) {
        MB2Ev_HatenaTaiyo(playerNo);
    }
#ifdef TARGET_PC
    return 0; /* the original returns whatever is left in r3 */
#endif
}

void MB2_Stub(void) {

}

int MB2Ev_MasuWalkPre(int playerNo) {
#ifdef TARGET_PC
    return 0; /* the original returns playerNo (untouched r3) */
#endif
}

int MB2Ev_MasuWalkPost(int playerNo) {
    MASU *masu;
    s16 masuId;
    u16 flag;
    s32 evNo;

    masuId = GwPlayer[playerNo].masuId;
    masu = MBMasuGet(0, masuId);
    flag = masu->flag;
    if ((u32) (flag & 0x2) != 0) {
        evNo = 9;
        MB2Ev_Rainbow(evNo, playerNo);
    }
    return 0;
}

int MB2_MasuPathCheck(s16 id, u32 flag, s16 *linkTbl, BOOL endF) {
    return -1;
}

s32 MB2Ev_Rainbow(s32 arg0, s32 arg1) {
    Point3d sp50;
    Point3d sp44;
    Point3d sp38;
    Point3d sp2C;
    Point3d sp20;
    Point3d *vtx;
    MASU *sp10;
    s32 spC;
    s32 sp8;
    s32 ret;
    s32 masuNo;
    s32 rbNo;
    f32 t;
    s32 rbId;
    s32 choice;
    s32 i;
    s32 j;
    s32 steps;
    s32 steps2;
    f32 rate;
    f32 yaw;
    f32 totalLen;
    s16 masuId;

    ret = 0;
    if ((arg0 <= 0) || (arg0 > 0xA)) {
        return 0;
    }
    if (arg0 == 0xA) {
        return 0;
    }
    masuId = GwPlayer[arg1].masuId;
    sp10 = MBMasuGet(0, masuId);
    rbNo = mb2ev_RainbowNoTbl[arg0];
    masuNo = MB2_RainbowMasuNoEndGet(masuId);
    if (masuNo == -1) {
        return 0;
    }
    MBPlayerMotionNoShiftSet(arg1, 1, 0.0f, 8.0f, 0U);
    MB2_RainbowObjInit();
    rbId = MB2_RainbowObjCreate(mb2ev_RainbowVtxTbl[rbNo], mb2ev_RainbowVtxNumTbl[rbNo], mb2ev_RainbowRevF[rbNo]);
    MB2_RainbowObjAlphaSet(rbId, 0);
    MB2_CoinEffInit();
    MBWalkNumDispSet(arg1, 0);
    MBCameraFocusPlayerSet(arg1);
    MBPlayerMotionNoShiftSet(arg1, 1, 0.0f, 8.0f, 0U);
    do {
        if (arg0 == 9) {
            choice = 0;
        } else {
            MBWinCreateChoice(1, 0x2E0000U, 0, 0);
            MBTopWinAttrSet(0x10U);
            if (MBPlayerCoinGet(arg1) < 0xA) {
                MBTopWinChoiceDisable(0);
            }
            if (GwPlayer[arg1].comF != 0) {
                MBComChoiceSetUp();
            }
            MBTopWinWait();
            choice = MBWinLastChoiceGet();
            MBTopWinKill();
            switch (choice) {
            case 2:
                MBScrollExec(arg1);
                MBStatusDispSetAll(1);
                break;
            }
        }
    } while (choice == 2);
    if (choice == 0) {
        if (arg0 != 9) {
            MBCoinAddExec(arg1, -0xA);
        }
        HuAudFXPlay(0x5C8);
        for (i = 0; (f32) i < 30.0f; i++) {
            MB2_RainbowObjAlphaSet(rbId, (s8) (255.0f * ((f32) i / 30.0f)));
            HuPrcVSleep();
        }
        MB2_RainbowObjAlphaSet(rbId, 0xFF);
        MBPlayerPosFixEvent(arg1, masuNo, 0);
        vtx = (Point3d *) mb2ev_RainbowVtxTbl[rbNo];
        totalLen = 0.0f;
        for (i = 1; i < mb2ev_RainbowVtxNumTbl[rbNo]; i++) {
            PSVECSubtract(&vtx[i - 1], &vtx[i], &sp38);
            totalLen += PSVECMag(&sp38);
        }
        spC = (s32) (totalLen / 20.0f);
        MBPlayerMotionNoShiftSet(arg1, 3, 0.0f, 8.0f, 0x40000001U);
        if (mb2ev_RainbowDirTbl[arg0] > 0) {
            MBPlayerPosGet(arg1, &sp50);
            sp44 = vtx[0];
            PSVECSubtract(&sp44, &sp50, &sp38);
            steps = (s32) (PSVECMag(&sp38) / 20.0f);
            for (i = 0; i < steps; i++) {
                t = (f32) i / (f32) steps;
                PSVECSubtract(&sp44, &sp50, &sp38);
                PSVECScale(&sp38, &sp38, t);
                PSVECAdd(&sp50, &sp38, &sp38);
                MBPlayerPosSetV(arg1, &sp38);
                PSVECSubtract(&sp44, &sp50, &sp38);
                MBPlayerRotGet(arg1, &sp2C);
                sp2C.y = (f32) (180.0 * (atan2((f64) sp38.x, (f64) sp38.z) / 3.141592653589793));
                MBPlayerRotSetV(arg1, &sp2C);
                HuPrcVSleep();
            }
            MBPlayerPosSetV(arg1, &sp44);
            for (i = 0; i < mb2ev_RainbowVtxNumTbl[rbNo] - 1; i++) {
                MBPlayerPosGet(arg1, &sp50);
                sp44 = vtx[i + 1];
                PSVECSubtract(&sp44, &sp50, &sp38);
                steps2 = (s32) (PSVECMag(&sp38) / 20.0f);
                MBPlayerRotGet(arg1, &sp2C);
                PSVECSubtract(&sp44, &sp50, &sp38);
                yaw = (f32) (180.0 * (atan2((f64) sp38.x, (f64) sp38.z) / 3.141592653589793));
                for (j = 0; j < steps2; j++) {
                    rate = (f32) j / (f32) steps2;
                    PSVECSubtract(&sp44, &sp50, &sp38);
                    PSVECScale(&sp38, &sp38, rate);
                    PSVECAdd(&sp50, &sp38, &sp38);
                    MBPlayerPosSetV(arg1, &sp38);
                    sp2C.y = fn_1_42FC(rate, sp2C.y, yaw);
                    MBPlayerRotSet(arg1, 0.0f, sp2C.y, 0.0f);
                    MBPlayerPosGet(arg1, &sp38);
                    sp20 = sp38;
                    MB2_CoinEffCreateColRand(&sp20.x, 2, 100.0f * (0.2f + (0.1f * (0.007874016f * (f32) (frand() & 0x7F)))), 0.016666668f * (0.5f + (0.3f * (0.007874016f * (f32) (frand() & 0x7F)))), 150.0f, 100.0f, 0.0f);
                    HuPrcVSleep();
                }
                MBPlayerPosSetV(arg1, &sp44);
                MBPlayerRotSet(arg1, 0.0f, yaw, 0.0f);
            }
        } else {
            MBPlayerPosGet(arg1, &sp50);
            sp44 = vtx[mb2ev_RainbowVtxNumTbl[rbNo] - 1];
            PSVECSubtract(&sp44, &sp50, &sp38);
            steps = (s32) (PSVECMag(&sp38) / 20.0f);
            for (i = 0; i < steps; i++) {
                t = (f32) i / (f32) steps;
                PSVECSubtract(&sp44, &sp50, &sp38);
                PSVECScale(&sp38, &sp38, t);
                PSVECAdd(&sp50, &sp38, &sp38);
                MBPlayerPosSetV(arg1, &sp38);
                PSVECSubtract(&sp44, &sp50, &sp38);
                MBPlayerRotGet(arg1, &sp2C);
                sp2C.y = (f32) (180.0 * (atan2((f64) sp38.x, (f64) sp38.z) / 3.141592653589793));
                MBPlayerRotSetV(arg1, &sp2C);
                HuPrcVSleep();
            }
            MBPlayerPosSetV(arg1, &sp44);
            for (i = mb2ev_RainbowVtxNumTbl[rbNo] - 1; i > 0; i--) {
                MBPlayerPosGet(arg1, &sp50);
                sp44 = vtx[i - 1];
                PSVECSubtract(&sp44, &sp50, &sp38);
                steps2 = (s32) (PSVECMag(&sp38) / 20.0f);
                MBPlayerRotGet(arg1, &sp2C);
                PSVECSubtract(&sp44, &sp50, &sp38);
                yaw = (f32) (180.0 * (atan2((f64) sp38.x, (f64) sp38.z) / 3.141592653589793));
                for (j = 0; j < steps2; j++) {
                    rate = (f32) j / (f32) steps2;
                    PSVECSubtract(&sp44, &sp50, &sp38);
                    PSVECScale(&sp38, &sp38, rate);
                    PSVECAdd(&sp50, &sp38, &sp38);
                    MBPlayerPosSetV(arg1, &sp38);
                    sp2C.y = fn_1_42FC(rate, sp2C.y, yaw);
                    MBPlayerRotSet(arg1, 0.0f, sp2C.y, 0.0f);
                    MBPlayerPosGet(arg1, &sp38);
                    sp20 = sp38;
                    MB2_CoinEffCreateColRand(&sp20.x, 2, 100.0f * (0.2f + (0.1f * (0.007874016f * (f32) (frand() & 0x7F)))), 0.016666668f * (0.5f + (0.3f * (0.007874016f * (f32) (frand() & 0x7F)))), 150.0f, 100.0f, 0.0f);
                    HuPrcVSleep();
                }
                MBPlayerPosSetV(arg1, &sp44);
                MBPlayerRotSet(arg1, 0.0f, yaw, 0.0f);
            }
        }
        MBPlayerPosGet(arg1, &sp50);
        MBMasuPosGet(0, masuNo, &sp44);
        PSVECSubtract(&sp44, &sp50, &sp38);
        steps = (s32) (PSVECMag(&sp38) / 20.0f);
        for (i = 0; i < steps; i++) {
            t = (f32) i / (f32) steps;
            PSVECSubtract(&sp44, &sp50, &sp38);
            PSVECScale(&sp38, &sp38, t);
            PSVECAdd(&sp50, &sp38, &sp38);
            MBPlayerPosSetV(arg1, &sp38);
            PSVECSubtract(&sp44, &sp50, &sp38);
            MBPlayerRotGet(arg1, &sp2C);
            sp2C.y = (f32) (180.0 * (atan2((f64) sp38.x, (f64) sp38.z) / 3.141592653589793));
            MBPlayerRotSetV(arg1, &sp2C);
            HuPrcVSleep();
        }
        MBPlayerPosSetV(arg1, &sp44);
        MBPlayerMotionNoShiftSet(arg1, 1, 0.0f, 8.0f, 0x40000001U);
        GwPlayer[arg1].masuId = (s16) masuNo;
        for (i = 0; (f32) i < 60.0f; i++) {
            MB2_RainbowObjAlphaSet(rbId, (s8) (255.0f * (1.0f - ((f32) i / 60.0f))));
            HuPrcVSleep();
        }
        MB2_RainbowObjAlphaSet(rbId, 0);
        ret = 1;
    }
    MB2_RainbowObjClose();
    MB2_CoinEffClose();
    MBWalkNumDispSet(arg1, 1);
    return ret;
}



/* Warning: Gap in callee-saved word stack region.
 * Saved: [-0xe8, -0xe4, -0xd8, -0xd4, -0xc8, -0xc4, -0xb8, -0xb4, -0xa8, -0xa4, -0x98, -0x94, -0x88, -0x84, -0x78, -0x74, -0x68, -0x64, -0x58, -0x54, -0x48, -0x44, 0xed4, 0xed8, 0xedc, 0xee0, 0xee4, 0xee8, 0xeec, 0xef0, 0xef4, 0xef8, 0xefc, 0xf00, 0xf04, 0xf08, 0xf0c, 0xf10, 0xf14, 0xf20, 0xf24, 0xf30, 0xf34, 0xf40, 0xf44, 0xf50, 0xf54, 0xf60, 0xf64, 0xf70, 0xf74, 0xf80, 0xf84, 0xf90, 0xf94, 0xfa0, 0xfa4, 0xfb0, 0xfb4], gap at: -0xe0. */
/* mb2_Work[0x1C]: array of path segments (one per vertex pair) of the chosen rainbow. */
typedef struct MB2TaiyoSeg_s {
    f32 len;         /* 0x00 */
    f32 start;       /* 0x04, distance from the path start */
    Point3d from;    /* 0x08 */
    Point3d to;      /* 0x14 */
    Point3d rot;     /* 0x20 */
    Point3d prevRot; /* 0x2C */
} MB2_TAIYO_SEG;     /* size 0x38 */

#define MB2_RND() (0.007874016f * (f32) (frand() & 0x7F))

/* complete; stack frame is 0x20 smaller than the original (temp layout), shifting offsets. */
void MB2Ev_HatenaTaiyo(s32 playerNo) {
    Point3d curPos;
    Point3d dstPos;
    Point3d tmp;
    Point3d rot;
    Point3d effPos;
    Point3d *vtx;
    MB2_TAIYO_SEG *seg;
    s32 rbId[3];
    s32 masuTgt[3];
    f32 pathLen;
    f32 dist;
    f32 speed;
    f32 rate;
    Point3d prevRot;
    f32 ang;
    s32 vtxNum;
    s32 sndId;
    s32 sndId2;
    s32 motId;
    s32 rbIdx;
    s32 choice;
    s32 prevChoice;
    s32 tgtMasu;
    s32 i;
    s32 j;
    s32 coin;
    s32 btn;
    s16 masuId;
    s16 helpWin;
    s32 flag;
    s8 stkX;
    s8 stkY;

    masuId = GwPlayer[playerNo].masuId;
    flag = (u16) MBMasuGet(0, masuId)->flag;
    MBPlayerMotionNoShiftSet(playerNo, 1, 0.0f, 8.0f, 0U);
    MB2_CoinInit();
    mb2_CoinMaxDist.x = 100.0f;
    mb2_CoinMaxDist.y = 200.0f;
    mb2_CoinMaxDist.z = -1.0f;
    MB2_CoinEffInit();
    rbIdx = (frand() & 0x7FFF) % 3;
    coin = MBPlayerCoinGet(playerNo);
    motId = MBPlayerMotionCreate(playerNo, 0x8C0008);
    for (i = 1; i < MBMasuNumGet(0); i++) {
        if ((u32) (MBMasuGet(0, i)->flag & 0x10) != 0) {
            break;
        }
    }
    masuTgt[0] = (i >= MBMasuNumGet(0)) ? masuId : i;
    for (i = 1; i < MBMasuNumGet(0); i++) {
        if ((u32) (MBMasuGet(0, i)->flag & 0x8) != 0) {
            break;
        }
    }
    masuTgt[1] = (i >= MBMasuNumGet(0)) ? masuId : i;
    for (i = 1; i < MBMasuNumGet(0); i++) {
        if ((u32) (MBMasuGet(0, i)->flag & 0x20) != 0) {
            break;
        }
    }
    masuTgt[2] = (i >= MBMasuNumGet(0)) ? masuId : i;
    MBPlayerPosGet(playerNo, &curPos);
    dstPos.x = curPos.x;
    dstPos.y = 150.0f + curPos.y;
    dstPos.z = curPos.z - 300.0f;
    MBCameraModelViewSet(MBPlayerModelGet(playerNo), NULL, NULL, 3000.0f, -1.0f, 0xA);
    MBCameraMotionWait();
    MBPlayerRotateStart(playerNo, 0xB4, 0xF);
    while (MBPlayerRotateCheck(playerNo) == 0) {
        HuPrcVSleep();
    }
    MBPlayerMotionNoShiftSet(playerNo, 4, 0.0f, 8.0f, 0U);
    for (i = 0; (f32) i < 27.0f; i++) {
        rate = (f32) i / 24.0f;
        PSVECSubtract(&dstPos, &curPos, &tmp);
        PSVECScale(&tmp, &tmp, rate);
        PSVECAdd(&curPos, &tmp, &tmp);
        tmp.y = (f32) ((f64) tmp.y + (200.0 * sin((3.141592653589793 * (f64) (180.0f * rate)) / 180.0)));
        MBPlayerPosSetV(playerNo, &tmp);
        HuPrcVSleep();
    }
    MBPlayerMotionNoShiftSet(playerNo, 1, 0.0f, 8.0f, 0x40000001U);
    MB2_YajiInit(&dstPos);
    sndId = HuAudFXPlay(0x5D8);
    MBMotionSpeedSet((s16) mb2_Work.mdlRainbow, 0.25f);
    MBMotionTimeSet((s16) mb2_Work.mdlRainbow, 0.0f);
    while (MBMotionTimeGet((s16) mb2_Work.mdlRainbow) <= 15.0f) {
        HuPrcVSleep();
    }
    if (sndId != -1) {
        HuAudFXStop(sndId);
    }
    HuAudFXPlay(0x5D7);
    while (MBMotionTimeGet((s16) mb2_Work.mdlRainbow) <= 25.0f) {
        HuPrcVSleep();
    }
    WipeColorSet(0xFFU, 0xFFU, 0xFFU);
    WipeCreate(2, 0, 0x1E);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    while (MBMotionEndCheck((s16) mb2_Work.mdlRainbow) == 0) {
        HuPrcVSleep();
    }
    MB2_RainbowObjInit();
    rbId[0] = MB2_RainbowObjCreate(mb2ev_TaiyoRainbowVtxTbl[rbIdx][0], mb2ev_TaiyoRainbowVtxNumTbl[rbIdx][0], 0);
    rbId[1] = MB2_RainbowObjCreate(mb2ev_TaiyoRainbowVtxTbl[rbIdx][1], mb2ev_TaiyoRainbowVtxNumTbl[rbIdx][1], 0);
    rbId[2] = MB2_RainbowObjCreate(mb2ev_TaiyoRainbowVtxTbl[rbIdx][2], mb2ev_TaiyoRainbowVtxNumTbl[rbIdx][2], 0);
    for (i = 0; i < 0x10; i++) {
        tmp = mb2ev_TaiyoCoinBasePos;
        tmp.y += 100.0f;
        effPos = tmp;
        effPos.z = mb2ev_TaiyoCoinBasePos.z;
        MB2_CoinEffCreateColRand(&effPos.x, 2, 100.0f * (0.2f + (0.1f * MB2_RND())), 0.016666668f * (0.5f + (0.3f * MB2_RND())), 150.0f, 100.0f, 0.0f);
    }
    MBMasuPlayerLightSet(playerNo, 0);
    HuAudFXPlay(0x5C8);
    WipeColorSet(0xFFU, 0xFFU, 0xFFU);
    WipeCreate(1, 0, 0x5A);
    do {
        if ((u32) (frand() & 1) == 0) {
            effPos = mb2ev_TaiyoCoinBasePos;
            MB2_CoinEffCreateColRand(&effPos.x, 2, 100.0f * (0.2f + (0.1f * MB2_RND())), 0.016666668f * (0.5f + (0.3f * MB2_RND())), 150.0f, 50.0f, 0.0f);
        }
        for (i = 0; i < 6; i++) {
            j = (frand() & 0x7FFF) % 3;
            if (MB2_RainbowObjVtxTopGet(rbId[j], (void **) &vtx) != 0) {
                effPos = vtx[(frand() & 0x7FFF) % 18];
                MB2_CoinEffCreateColRand(&effPos.x, 2, 100.0f * (0.1f + (0.1f * MB2_RND())), 0.016666668f * (0.5f + (0.3f * MB2_RND())), 150.0f, 50.0f, 0.0f);
            }
        }
        HuPrcVSleep();
    } while (WipeCheckEnd() != 0);
    MBPlayerRotateStart(playerNo, 0, 0xF);
    while (MBPlayerRotateCheck(playerNo) == 0) {
        HuPrcVSleep();
    }
    MBPlayerMotionNoShiftSet(playerNo, 9, 0.0f, 8.0f, 0U);
    while (MBMotionShiftIDGet(MBPlayerModelGet(playerNo)) != -1) {
        HuPrcVSleep();
    }
    while (MBMotionEndCheck(MBPlayerModelGet(playerNo)) == 0) {
        HuPrcVSleep();
    }
    MBPlayerMotionNoShiftSet(playerNo, 1, 0.0f, 8.0f, 0x40000001U);
    MBWinCreate(2, 0x2E000CU, -1);
    MBTopWinWait();
    MBTopWinKill();
    helpWin = MBWinCreateHelp(0x2E000DU);
    MBWinPosSet(helpWin, 0xC9, 0x120);
    MB2_YajiDispSet(1);
    choice = 1;
    prevChoice = 1;
    if (GwPlayer[playerNo].comF == 0) {
        do {
            HuPrcVSleep();
            stkX = MBPadStkXGet(GwPlayer[playerNo].padNo);
            stkY = MBPadStkYGet(GwPlayer[playerNo].padNo);
            btn = (s32) HuPadBtnDown[GwPlayer[playerNo].padNo];
            if ((fabs((f64) stkX) > 8.0) || (fabs((f64) stkY) > 8.0)) {
                ang = (f32) (180.0 * (atan2((f64) stkX, (f64) -stkY) / 3.141592653589793));
                if (fabs(ang) < 45.0) {
                    choice = 1;
                } else if (ang >= 45.0f) {
                    choice = 2;
                } else if (ang <= -45.0f) {
                    choice = 0;
                }
                if (choice != prevChoice) {
                    MB2_YajiHiliteSet(choice);
                }
                prevChoice = choice;
            }
        } while ((btn & 0x100) == 0);
    } else {
        HuPrcSleep((s32) (60.0f * MB2_RND()));
        choice = (frand() & 0x7FFF) % 3;
        MB2_YajiHiliteSet(choice);
        HuPrcSleep((s32) (60.0f * MB2_RND()));
    }
    MB2_YajiDispSet(0);
    MBTopWinKill();
    vtxNum = MB2_RainbowObjVtxTopGet(rbId[choice], (void **) &vtx);
    tgtMasu = masuTgt[mb2ev_TaiyoDestTbl[rbIdx][choice]];
    MBPlayerPosFixEvent(playerNo, tgtMasu, 1);
    seg = HuMemDirectMallocNum(HEAP_HEAP, vtxNum * sizeof(MB2_TAIYO_SEG), 0x10000000U);
    mb2_Work.seg = seg;
    memset(seg, 0, vtxNum * sizeof(MB2_TAIYO_SEG));
    pathLen = 0.0f;
    for (i = 0; i < vtxNum - 1; i++, seg++) {
        f32 sq;
        f32 horiz;

        PSVECSubtract(&vtx[i + 1], &vtx[i], &tmp);
        seg->len = PSVECMag(&tmp);
        seg->start = pathLen;
        seg->from = vtx[i];
        seg->to = vtx[i + 1];
        sq = (tmp.x * tmp.x) + (tmp.z * tmp.z);
        if (sq > 0.0f) {
            f64 r0 = __frsqrte(sq);
            f64 r1 = 0.5 * r0 * (3.0 - ((f64) sq * (r0 * r0)));
            f64 r2 = 0.5 * r1 * (3.0 - ((f64) sq * (r1 * r1)));
            horiz = (f32) ((f64) sq * (0.5 * r2 * (3.0 - ((f64) sq * (r2 * r2)))));
        } else {
            horiz = sq;
        }
        seg->rot.x = (f32) -(180.0 * (atan2((f64) tmp.y, (f64) horiz) / 3.141592653589793));
        seg->rot.y = (f32) (180.0 * (atan2((f64) tmp.x, (f64) tmp.z) / 3.141592653589793));
        seg->rot.z = 0.0f;
        if (i == 0) {
            seg->prevRot = seg->rot;
        } else {
            seg->prevRot = prevRot;
        }
        prevRot = seg->rot;
        pathLen += seg->len;
    }
    MBPlayerMotionNoShiftSet(playerNo, motId, 0.0f, 8.0f, 0U);
    MBPlayerPosGet(playerNo, &curPos);
    dstPos = vtx[0];
    PSVECSubtract(&dstPos, &curPos, &tmp);
    PSVECMag(&tmp);
    for (i = 0; (f32) i < 27.0f; i++) {
        rate = (f32) i / 27.0f;
        PSVECSubtract(&dstPos, &curPos, &tmp);
        PSVECScale(&tmp, &tmp, rate);
        PSVECAdd(&curPos, &tmp, &tmp);
        tmp.y = (f32) ((f64) tmp.y + (2.0 * (100.0 * sin((3.141592653589793 * (f64) (180.0f * rate)) / 180.0))));
        MBPlayerPosSetV(playerNo, &tmp);
        PSVECSubtract(&dstPos, &curPos, &tmp);
        MBPlayerRotGet(playerNo, &rot);
        rot.y = (f32) (180.0 * (atan2((f64) tmp.x, (f64) tmp.z) / 3.141592653589793));
        MBPlayerRotSetV(playerNo, &rot);
        HuPrcVSleep();
    }
    MBPlayerPosSetV(playerNo, &dstPos);
    sndId2 = HuAudFXPlay(0x5D9);
    dist = 0.0f;
    speed = 10.0f;
    do {
        if (speed < 30.000002f) {
            speed += 0.1f;
        }
        dist += speed;
        seg = mb2_Work.seg;
        for (i = 0; i < vtxNum - 1; i++, seg++) {
            if (dist < (seg->len + seg->start)) {
                break;
            }
        }
        if (i >= vtxNum - 1) {
            break;
        }
        rate = (dist - seg->start) / seg->len;
        PSVECSubtract(&seg->to, &seg->from, &tmp);
        PSVECScale(&tmp, &tmp, rate);
        PSVECAdd(&seg->from, &tmp, &tmp);
        MBPlayerRotGet(playerNo, &rot);
        rot.x = fn_1_42FC(rate, seg->prevRot.x, seg->rot.x);
        rot.y = fn_1_42FC(rate, seg->prevRot.y, seg->rot.y);
        rot.z = 0.0f;
        MBPlayerPosSetV(playerNo, &tmp);
        MBPlayerRotSetV(playerNo, &rot);
        effPos = tmp;
        MB2_CoinEffCreateColRand(&effPos.x, 2, 100.0f * (0.2f + (0.1f * MB2_RND())), 0.016666668f * (0.5f + (0.3f * MB2_RND())), 150.0f, 100.0f, 0.0f);
        HuPrcVSleep();
    } while (dist < pathLen);
    MBPlayerRotSet(playerNo, 0.0f, 0.0f, 0.0f);
    if (sndId2 != -1) {
        HuAudFXStop(sndId2);
    }
    MBPlayerMotionNoShiftSet(playerNo, 4, 0.0f, 8.0f, 0U);
    MBPlayerPosGet(playerNo, &curPos);
    MBMasuPosGet(0, tgtMasu, &dstPos);
    PSVECSubtract(&dstPos, &curPos, &tmp);
    PSVECMag(&tmp);
    for (i = 0; (f32) i < 27.0f; i++) {
        rate = (f32) i / 27.0f;
        PSVECSubtract(&dstPos, &curPos, &tmp);
        PSVECScale(&tmp, &tmp, rate);
        PSVECAdd(&curPos, &tmp, &tmp);
        tmp.y = (f32) ((f64) tmp.y + (2.0 * (100.0 * sin((3.141592653589793 * (f64) (180.0f * rate)) / 180.0))));
        MBPlayerPosSetV(playerNo, &tmp);
        PSVECSubtract(&dstPos, &curPos, &tmp);
        MBPlayerRotGet(playerNo, &rot);
        rot.y = (f32) (180.0 * (atan2((f64) tmp.x, (f64) tmp.z) / 3.141592653589793));
        MBPlayerRotSetV(playerNo, &rot);
        HuPrcVSleep();
    }
    MBPlayerPosSetV(playerNo, &dstPos);
    GwPlayer[playerNo].masuId = (s16) tgtMasu;
    MBPlayerMotionNoShiftSet(playerNo, 1, 0.0f, 8.0f, 0x40000001U);
    MBMasuPlayerLightSet(playerNo, 1);
    for (i = 0; (f32) i < 60.0f; i++) {
        rate = 1.0f - ((f32) i / 60.0f);
        MB2_RainbowObjAlphaSet(rbId[0], (s8) (255.0f * rate));
        MB2_RainbowObjAlphaSet(rbId[1], (s8) (255.0f * rate));
        MB2_RainbowObjAlphaSet(rbId[2], (s8) (255.0f * rate));
        HuPrcVSleep();
    }
    HuPrcSleep(0x1E);
    MBMotionSpeedSet((s16) mb2_Work.mdlRainbow, 0.0f);
    MBMotionTimeSet((s16) mb2_Work.mdlRainbow, 0.0f);
    if (motId != -1) {
        MBPlayerMotionKill(playerNo, motId);
    }
    if (mb2_Work.seg != NULL) {
        HuMemDirectFree(mb2_Work.seg);
    }
    mb2_Work.seg = NULL;
    MB2_RainbowObjClose();
    MB2_YajiClose();
    MB2_CoinClose();
    MB2_CoinEffClose();
}

s16 MB2_RainbowMasuNoEndGet(s32 masuId) {
    s16 linkTbl[16];
    s32 i;
    MASU *masu;
    s32 mode;
    s32 linkNum;

    masu = MBMasuGet(0, masuId);
    mode = 9;
    if (mode == 2 || mode == 4 || mode == 6 || mode == 8) {
        return -1;
    }
    if ((masu->flag & 0x3E) != 0) {
        u16 linkId;

        masu = MBMasuGet(0, masuId);
        for (i = 0; i < masu->linkNum; i++) {
            if ((MBMasuFlagGet(0, masu->linkTbl[i]) & 0x40) != 0) {
                break;
            }
        }
        if (i < masu->linkNum) {
            linkId = masu->linkTbl[i];
            linkNum = MB2_MasuLinkGet(linkId, linkTbl, 1, 1);
            for (i = 0; i < linkNum; i++) {
                if (masuId != linkTbl[i]) {
                    break;
                }
            }
            if (i < linkNum) {
                return linkTbl[i];
            }
        }
    }
    return -1;
}

void MB2_CoinDispUpdate(s32 arg0, s32 arg1, s32 arg2) {
    Point3d spC;
    s32 var_r29;
    s32 var_r30;

    var_r30 = -1;
    MBPlayerRotateStart(arg0, 0, 0xF);
    while (MBPlayerRotateCheck(arg0) == 0) {
        HuPrcVSleep();
    }
    if (arg1 > 0) {
        MBPlayerPosGet(arg0, &spC);
        spC.y += 250.0f;
        var_r29 = (s16) MBCoinDispCreateSe(&spC, arg1);
    } else {
        var_r29 = -1;
    }
    if (arg1 <= 0) {
        MBPlayerWinLoseVoicePlay(arg0, 0xD, 0x200);
        MBPlayerMotionNoShiftSet(arg0, 0xD, 0.0f, 8.0f, 0U);
    } else if (arg1 < arg2) {
        MBPlayerWinLoseVoicePlay(arg0, 0xC, 0x1FB);
        MBPlayerMotionNoShiftSet(arg0, 0xC, 0.0f, 8.0f, 0U);
    } else {
        MBPlayerWinLoseVoicePlay(arg0, 0xC, 0x1FB);
        var_r30 = MBPlayerMotionCreate(arg0, 0x8C002D);
        MBPlayerMotionNoShiftSet(arg0, var_r30, 0.0f, 8.0f, 0U);
    }
    do {
loop_11:
        HuPrcVSleep();
        if (MBPlayerMotionEndCheck(arg0) == 0) {
            goto loop_11;
        }
    } while (MBMotionShiftIDGet(MBPlayerModelGet(arg0)) != -1);
    MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 8.0f, 0U);
    do {
        HuPrcVSleep();
    } while (MBMotionShiftIDGet(MBPlayerModelGet(arg0)) != -1);
    if (var_r29 != -1) {
        while (MBCoinDispKillCheck(var_r29) == 0) {
            HuPrcVSleep();
        }
    }
    if (var_r30 != -1) {
        MBPlayerMotionKill(arg0, var_r30);
    }
}

f32 MB2_AngleDiff(f32 farg0, f32 farg1) {
    f32 diff;

    if (farg0 >= 360.0f) {
        farg0 -= 360.0f;
    } else if (farg0 < 0.0f) {
        farg0 += 360.0f;
    }
    if (farg1 >= 360.0f) {
        farg1 -= 360.0f;
    } else if (farg1 < 0.0f) {
        farg1 += 360.0f;
    }
    diff = farg0 - farg1;
    if (diff <= -180.0f) {
        diff += 360.0f;
    } else if (diff >= 180.0f) {
        diff -= 360.0f;
    }
    return diff;
}

f32 MB2_AngleLerp(f32 farg0, f32 farg1, f32 farg2) {
    f32 diff;
    f32 res;

    if (farg0 >= 360.0) {
        farg0 -= 360.0;
    } else if (farg0 < 0.0) {
        farg0 += 360.0;
    }
    if (farg1 >= 360.0) {
        farg1 -= 360.0;
    } else if (farg1 < 0.0) {
        farg1 += 360.0;
    }
    diff = 360.0 + (farg0 - farg1);
    if (fabs(diff) >= 360.0) {
        diff = fmod(diff, 360.0);
    }
    if (diff < 180.0) {
        if (diff <= farg2) {
            res = diff;
        } else {
            res = farg2;
        }
    } else {
        if ((360.0 - diff) <= farg2) {
            res = -(360.0 - diff);
        } else {
            res = -farg2;
        }
    }
    res += farg1;
    if (res >= 360.0) {
        res -= 360.0;
    } else if (res < 0.0) {
        res += 360.0;
    }
    return res;
}

f32 fn_1_42FC(f32 farg0, f32 farg1, f32 farg2) {
    f32 diff;
    f32 step;

    diff = MB2_AngleDiff(farg2, farg1);
    step = fabs(diff * farg0);
    return MB2_AngleLerp(farg2, farg1, step);
}

s32 MB2_MasuLinkGet(s16 arg0, s16 *arg1, s32 arg2, s32 arg3) {
    MASU *masu;
    s32 i;
    s32 num;

    num = 0;
    if (arg2 != 0) {
        masu = MBMasuGet(0, arg0);
        num = masu->linkNum;
        for (i = 0; i < num; i++) {
            arg1[i] = masu->linkTbl[i];
        }
    }
    if (arg3 != 0) {
        num += MBMasuLinkTblGet(0, arg0, arg1 + num);
    }
    return num;
}

HSFOBJECT *MB2_ModelObjPtrGet(HU3DMODEL *arg0, char *arg1) {
    HSFDATA *hsf;

    hsf = arg0->hsf;
    return objCall(arg0, arg1, hsf->root);
}

static HSFOBJECT *objCall(HU3DMODEL *arg0, char *arg1, HSFOBJECT *arg2) {
    s32 i;
    HSFOBJECT *meshObj;
    HSFOBJECT *replicaObj;
    HSFOBJECT *ret;

    switch (arg2->type) {
        case HSF_OBJ_NULL1:
        case HSF_OBJ_MESH:
        case HSF_OBJ_ROOT:
        case HSF_OBJ_JOINT:
        case HSF_OBJ_NULL2:
        case HSF_OBJ_NULL3:
        case HSF_OBJ_MAP: {
            meshObj = arg2;
            if (!strcmp(arg1, meshObj->name)) {
                return meshObj;
            }
            for (i = 0; i < meshObj->mesh.childNum; i++) {
                ret = objCall(arg0, arg1, meshObj->mesh.child[i]);
                if (ret) {
                    return ret;
                }
            }
        } break;
        case HSF_OBJ_REPLICA: {
            replicaObj = arg2;
            if (!strcmp(arg1, replicaObj->name)) {
                return replicaObj;
            }
            for (i = 0; i < replicaObj->mesh.childNum; i++) {
                ret = objCall(arg0, arg1, replicaObj->mesh.child[i]);
                if (ret) {
                    return ret;
                }
            }
        } break;
    }
    return NULL;
}

f32 MB2_BezierCalc(f32 t, f32 p0, f32 p1, f32 p2) {
    f32 u;
    f32 res;

    u = 1.0 - t;
    res = (p2 * (t * t)) + ((p0 * (u * u)) + (2.0 * (p1 * (u * t))));
    return res;
}

void MB2_BezierCalcV(f32 *arg0, f32 *arg1, f32 *arg2, f32 *arg3, f32 t) {
    f32 a;
    f32 b;
    f32 c;
    f32 r;
    s32 i;

    for (i = 0; i < 3; i++) {
        c = *arg2++;
        b = *arg1++;
        a = *arg0++;
        r = MB2_BezierCalc(t, a, b, c);
        *arg3++ = r;
    }
}

void MB2_HermiteCalc(f32 *arg0, f32 *arg1, f32 *arg2, f32 *arg3, f32 farg0) {
    f32 temp_f29;
    f32 temp_f30;
    f32 temp_f31;

    temp_f31 = farg0 * farg0;
    temp_f30 = farg0 * temp_f31;
    temp_f29 = (f32) (((3.0 * (f64) temp_f31) - (f64) temp_f30) - (f64) temp_f30);
    *arg0 = (f32) (1.0 - (f64) temp_f29);
    *arg1 = temp_f29;
    *arg2 = farg0 + ((temp_f30 - temp_f31) - temp_f31);
    *arg3 = temp_f30 - temp_f31;
}

f32 fn_1_5260(f32 t, f32 p0, f32 p1, f32 p2, f32 p3, f32 tension) {
    f32 res;
    f32 m1;
    f32 m2;
    f32 d;
    f32 h0;
    f32 h1;
    f32 h2;
    f32 h3;
    f32 k1;
    f32 k2;
    s32 dummy;

    d = p2 - p1;
    dummy = (s32) (p2 - p1);
    if (p1 == p2) {
        m1 = d;
    } else {
        k1 = tension;
        m1 = k1 * (d + (p1 - p0));
    }
    if (p1 == p3) {
        m2 = d;
    } else {
        k2 = tension;
        m2 = k2 * (d + (p3 - p2));
    }
    MB2_HermiteCalc(&h0, &h1, &h2, &h3, t);
    res = (h3 * m2) + ((h2 * m1) + ((h0 * p1) + (h1 * p2)));
    return res;
}

void MB2_HermiteCalcV(Point3d *arg0, Point3d *arg1, Point3d *arg2, Point3d *arg3, Point3d *arg4, f32 farg0) {
    f32 tension = 0.5f;

    arg4->x = fn_1_5260(farg0, arg0->x, arg1->x, arg2->x, arg3->x, tension);
    arg4->y = fn_1_5260(farg0, arg0->y, arg1->y, arg2->y, arg3->y, tension);
    arg4->z = fn_1_5260(farg0, arg0->z, arg1->z, arg2->z, arg3->z, tension);
}

void MB2_CoinInit(void) {
    OMOBJ *obj;
    MB2_COIN_SLOT *slot;
    s32 i;
    s32 j;

    obj = mb2_CoinObj = omAddObjEx(mbObjMan, -0x8000, 0, 0, -1, MB2_CoinExec);
    slot = obj->data = HuMemDirectMallocNum(HEAP_HEAP, 0x20 * sizeof(MB2_COIN_SLOT), 0x10000000U);
    memset(slot, 0, 0x20 * sizeof(MB2_COIN_SLOT));
    mb2_CoinMaxDist.x = 100.0f;
    mb2_CoinMaxDist.y = 170.0f;
    mb2_CoinMaxDist.z = 0.0f;
    for (i = 0; i < 0x20; i++, slot++) {
        slot->modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x80002, 0x10000000, HEAP_MODEL));
        Hu3DModelCameraSet((s16) slot->modelId, TRUE);
        Hu3DModelAttrSet((s16) slot->modelId, 1U);
        slot->activeF = 0;
        slot->unk8 = 0;
        slot->unkC = 0.0f;
        slot->unk10 = slot->unk14 = 0;
        for (j = 0; j < 4; j++) {
            MBPlayerPosGet(j, &slot->playerPos[j]);
        }
    }
}

typedef struct MB2Col4_s {
    u8 c[4];
} MB2_COL4;

void MB2_CoinExec(OMOBJ *obj) {
    MB2_COIN_SLOT *slot;
    s32 i;
    s32 j;
    s32 k;
    s32 collect;
    s32 collectedF;
    s32 killF;

    slot = (MB2_COIN_SLOT *) obj->data;
    if (MBKillCheck() || (mb2_CoinObj == NULL)) {
        for (i = 0; i < 0x20; i++, slot++) {
            Hu3DModelKill((s16) slot->modelId);
            slot->modelId = -1;
        }
        omDelObjEx(mbObjMan, obj);
        mb2_CoinObj = NULL;
        return;
    }
    for (i = 0; i < 0x20; i++, slot++) {
        if (slot->activeF == 0) {
            continue;
        }
        killF = 0;
        collectedF = 0;
        switch (slot->unk8) {
        case 0: {
            f32 t;
            Point3d src;
            Point3d dst;
            Point3d delta;
            slot->unk14++;
            t = (f32) slot->unk14 / (f32) slot->unk10;
            src = slot->srcPos;
            dst = slot->dstPos;
            PSVECSubtract(&dst, &src, &delta);
            PSVECScale(&delta, &delta, t);
            PSVECAdd(&delta, &src, &delta);
            delta.y += slot->unkC * sin((3.141592653589793 * (180.0f * t)) / 180.0);
            slot->pos = delta;
            slot->rot.y += 10.0f;
            if (t >= 1.0f) {
                slot->unk8++;
                slot->unk14 = 0;
            }
            break;
        }
        case 1:
            slot->pos = slot->dstPos;
            slot->rot.y += 10.0f;
            break;
        }
        if (slot->unk8 <= 2) {
            for (j = 0; j < 4; j++) {
                Point3d delta;
                Point3d pos;
                Point3d prevPos;
                MBPlayerPosGet(j, &pos);
                prevPos = slot->playerPos[j];
                PSVECSubtract(&slot->pos, &pos, &delta);
                collect = 0;
                if (pos.y >= prevPos.y) {
                    if (slot->pos.y >= prevPos.y && slot->pos.y <= pos.y + mb2_CoinMaxDist.y) {
                        collect = 1;
                    } else if (slot->pos.y <= prevPos.y + mb2_CoinMaxDist.y && slot->pos.y >= pos.y) {
                        collect = 1;
                    }
                }
                if (mb2_CoinMaxDist.z < 0.0f) {
                    collect = 1;
                }
                if (collect != 0) {
                    f32 distSq;
                    distSq = (delta.x * delta.x) + (delta.z * delta.z);
                    if (sqrtf(distSq) < mb2_CoinMaxDist.x) {
                        MBPlayerCoinAdd(j, 1);
                        MBAudFXPlay(7);
                        collectedF = 1;
                    }
                }
                slot->playerPos[j] = pos;
            }
        }
        Hu3DModelPosSet((s16) slot->modelId, slot->pos.x, slot->pos.y, slot->pos.z);
        Hu3DModelRotSet((s16) slot->modelId, slot->rot.x, slot->rot.y, slot->rot.z);
        Hu3DModelScaleSet((s16) slot->modelId, slot->scale.x, slot->scale.y, slot->scale.z);
        if (collectedF != 0 || killF != 0) {
            slot->activeF = 0;
            Hu3DModelAttrSet((s16) slot->modelId, 1U);
            if (collectedF != 0) {
                f32 ang = 0.0f;
                for (k = 0; k < 0x20; k++) {
                    f32 speed;
                    f32 tilt;
                    f32 r;
                    Point3d pos;
                    Point3d vel;
                    Point3d pos2;
                    Point3d vel2;
                    MB2_COL4 col;
                    MB2_COL4 col2;
                    ang += 22.5f * (1.0f + (0.007874016f * (f32) (frand() & 0x7F)));
                    pos = slot->pos;
                    speed = 2.5f * (1.0f + (0.007874016f * (f32) (frand() & 0x7F)));
                    tilt = 45.0f - (90.0f * (0.007874016f * (f32) (frand() & 0x7F)));
                    vel.x = speed * (sin((3.141592653589793 * ang) / 180.0) * cos((3.141592653589793 * tilt) / 180.0));
                    vel.y = speed * (cos((3.141592653589793 * ang) / 180.0) * cos((3.141592653589793 * tilt) / 180.0));
                    vel.z = speed * sin((3.141592653589793 * tilt) / 180.0);
                    r = 0.007874016f * (f32) (frand() & 0x7F);
                    col.c[0] = (u8) (192.0f + (63.0f * r));
                    col.c[1] = (u8) (192.0f + (63.0f * r));
                    col.c[2] = (u8) (64.0f + (63.0f * r));
                    col.c[3] = (u8) (192.0f + (63.0f * (0.007874016f * (f32) (frand() & 0x7F))));
                    col2 = col;
                    vel2 = vel;
                    pos2 = pos;
                    MB2_CoinEffCreate(&pos2.x, &vel2.x, col2.c,
                        100.0f * (0.3f + (0.1f * (0.007874016f * (f32) (frand() & 0x7F)))),
                        0.016666668f * (1.0f + (0.5f * (0.007874016f * (f32) (frand() & 0x7F)))), 0.0f);
                }
                return;
            }
        }
    }
}

void MB2_CoinClose(void) {
    mb2_CoinObj = NULL;
}

s32 MB2_CoinNumGet(void) {
    OMOBJ *obj;
    MB2_COIN_SLOT *slot;
    s32 i;
    s32 num;

    obj = mb2_CoinObj;
    num = 0;
    if (mb2_CoinObj == NULL) {
        return 0;
    }
    slot = (MB2_COIN_SLOT *) obj->data;
    for (i = 0; i < 0x20; i++, slot++) {
        if (slot->activeF != 0) {
            num++;
        }
    }
    return num;
}

s32 MB2_CoinCreate(Point3d *arg0, Point3d *arg1, s32 arg2, f32 farg0) {
    OMOBJ *obj;
    MB2_COIN_SLOT *slot;
    s32 i;
    s32 unused;

    obj = mb2_CoinObj;
    unused = 0;
    if (mb2_CoinObj == NULL) {
        return -1;
    }
    slot = (MB2_COIN_SLOT *) obj->data;
    for (i = 0; i < 0x20; i++, slot++) {
        if (slot->activeF == 0) {
            break;
        }
    }
    if (i >= 0x20) {
        return -1;
    }
    slot->activeF = 1;
    slot->unk8 = 0;
    slot->unkC = farg0;
    slot->unk10 = arg2;
    slot->unk14 = 0;
    if (arg0 != NULL) {
        slot->srcPos = *arg0;
    }
    if (arg1 != NULL) {
        slot->dstPos = *arg1;
    }
    slot->pos = slot->srcPos;
    slot->rot.x = slot->rot.y = slot->rot.z = 0.0f;
    slot->scale.x = slot->scale.y = slot->scale.z = 1.0f;
    slot->rot.y = 360.0f * (0.007874016f * (f32) (frand() & 0x7F));
    Hu3DModelPosSet((s16) slot->modelId, slot->pos.x, slot->pos.y, slot->pos.z);
    Hu3DModelRotSet((s16) slot->modelId, slot->rot.x, slot->rot.y, slot->rot.z);
    Hu3DModelScaleSet((s16) slot->modelId, slot->scale.x, slot->scale.y, slot->scale.z);
    Hu3DModelTPLvlSet((s16) slot->modelId, 1.0f);
    Hu3DModelAttrReset((s16) slot->modelId, 1U);
    return i;
}

typedef struct MB2CoinEffData_s {
    s32 modelId; /* 0x00 */
    s32 count;   /* 0x04 */
    ANIMDATA *anim; /* 0x08 */
} MB2_COINEFF_DATA;

void MB2_CoinEffInit(void) {
    OMOBJ *obj;
    MB2_COINEFF_DATA *dat;
    MB2_PARTICLE *particleP;
    HU3DMODEL *modelP;
    ANIMDATA *anim;
    s32 modelId;

    obj = mb2_CoinEffObj = omAddObjEx(mbObjMan, -0x8000, 0, 0, -1, MB2_CoinEffUpdate);
    dat = obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MB2_COINEFF_DATA), 0x10000000U);
    memset(dat, 0, sizeof(MB2_COINEFF_DATA));
    dat->anim = anim = HuSprAnimRead(HuDataReadNum(0xB0042, 0x10000000));
    dat->modelId = modelId = MB2_ParticleCreate(anim, 0x100);
    Hu3DModelLayerSet(dat->modelId, 3);
    dat->count = 0;
    modelP = &Hu3DData[dat->modelId];
    particleP = modelP->hookData;
    particleP->blendMode = 1;
    particleP->dispAttr = MB2_PARTICLE_DISPATTR_NOANIM;
}

void MB2_CoinEffUpdate(OMOBJ *obj) {
    MB2_COINEFF_DATA *dat;
    MB2_PARTICLE *particleP;
    MB2_PARTICLE_DATA *particleDataP;
    HU3DMODEL *modelP;
    s32 i;

    dat = obj->data;
    if (MBKillCheck() || (mb2_CoinEffObj == NULL)) {
        Hu3DModelKill(dat->modelId);
        dat->modelId = -1;
        HuSprAnimKill(dat->anim);
        dat->anim = NULL;
        omDelObjEx(mbObjMan, obj);
        mb2_CoinEffObj = NULL;
        return;
    }
    if (dat->count <= 0) {
        Hu3DModelAttrSet(dat->modelId, 1U);
        return;
    }
    Hu3DModelAttrReset(dat->modelId, 1U);
    modelP = &Hu3DData[dat->modelId];
    particleP = modelP->hookData;
    particleDataP = particleP->data;
    particleP->unk23 = 0;
    for (i = 0; i < particleP->num; i++, particleDataP++) {
        if (particleDataP->scale <= 0.0f) {
            continue;
        }
        particleDataP->pos.x += particleDataP->vel.x;
        particleDataP->pos.y += particleDataP->vel.y;
        particleDataP->pos.z += particleDataP->vel.z;
        if (particleDataP->gravity) {
            particleDataP->vel.y -= particleDataP->gravity;
        }
        particleDataP->scale = particleDataP->baseScale * particleDataP->scaleFactor;
        particleDataP->scaleFactor -= particleDataP->speed;
        if (particleDataP->scaleFactor <= 0.0f) {
            particleDataP->animNo = 0;
            particleDataP->time = 0;
            particleDataP->scale = 0.0f;
            dat->count--;
        }
    }
}

void MB2_CoinEffClose(void) {
    mb2_CoinEffObj = NULL;
}

s32 MB2_CoinEffNumGet(void) {
    OMOBJ *obj;
    MB2_COINEFF_DATA *dat;

    obj = mb2_CoinEffObj;
    if (mb2_CoinEffObj == NULL) {
        return 0;
    }
    dat = (MB2_COINEFF_DATA *) obj->data;
    return dat->count;
}

void MB2_CoinEffBlendModeSet(s32 arg0) {
    OMOBJ *obj;
    MB2_COINEFF_DATA *dat;
    MB2_PARTICLE *particleP;
    HU3DMODEL *modelP;

    obj = mb2_CoinEffObj;
    if (mb2_CoinEffObj != NULL) {
        dat = (MB2_COINEFF_DATA *) obj->data;
        modelP = &Hu3DData[dat->modelId];
        particleP = modelP->hookData;
        particleP->blendMode = arg0;
    }
}

s32 MB2_CoinEffCreate(f32 *arg0, f32 *arg1, u8 *arg2, f32 farg0, f32 farg1, f32 farg2) {
    OMOBJ *obj;
    MB2_COINEFF_DATA *dat;
    MB2_PARTICLE *particleP;
    MB2_PARTICLE_DATA *particleDataP;
    HU3DMODEL *modelP;
    s32 i;

    obj = mb2_CoinEffObj;
    if (mb2_CoinEffObj == NULL) {
        return -1;
    }
    dat = obj->data;
    modelP = &Hu3DData[dat->modelId];
    particleP = modelP->hookData;
    for (particleDataP = particleP->data, i = 0; i < particleP->num; i++, particleDataP++) {
        if (particleDataP->scale <= 0.0f) {
            break;
        }
    }
    if (i >= particleP->num) {
        return -1;
    }
    particleDataP->time = particleDataP->work = 0;
    particleDataP->pos.x = arg0[0];
    particleDataP->pos.y = arg0[1];
    particleDataP->pos.z = arg0[2];
    particleDataP->vel.x = arg1[0];
    particleDataP->vel.y = arg1[1];
    particleDataP->vel.z = arg1[2];
    particleDataP->baseScale = farg0;
    particleDataP->scaleFactor = 1.0f;
    particleDataP->speed = farg1;
    particleDataP->gravity = farg2;
    particleDataP->scale = farg0;
    particleDataP->color.r = arg2[0];
    particleDataP->color.g = arg2[1];
    particleDataP->color.b = arg2[2];
    particleDataP->color.a = arg2[3];
    particleDataP->rot.z = 360.0f * (0.007874016f * (f32) (frand() & 0x7F));
    particleDataP->animNo = 0;
    particleDataP->time = 0;
    dat->count++;
    return i;
}

s32 MB2_CoinEffCreateRand(void *arg0, s32 arg1, f32 farg0, f32 farg1, f32 farg2, f32 farg3, f32 farg4) {
    Point3d *pos;
    Point3d srcPos;
    Point3d vel;
    Point3d srcPos2;
    Point3d vel2;
    MB2_COL4 col;
    MB2_COL4 col2;
    f32 spare;
    f32 t;

    pos = (Point3d *) arg0;
    if (mb2_CoinEffObj == NULL) {
        return -1;
    }
    srcPos.x = pos->x + (farg2 * (-0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
    srcPos.y = pos->y + (farg3 * (-0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
    srcPos.z = pos->z + farg4;
    vel.x = 0.0f;
    vel.z = 0.0f;
    switch (arg1) {
    case 1:
        vel.y = -1.0f * (1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F))));
        spare = 0.0f;
        break;
    case 2:
        vel.y = 1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F)));
        spare = 0.1f;
        break;
    default:
        vel.y = 0.0f;
        spare = 0.0f;
        break;
    }
    t = 0.007874016f * (f32) (frand() & 0x7F);
    col.c[0] = (u8) (192.0f + (63.0f * t));
    col.c[1] = (u8) (192.0f + (63.0f * t));
    col.c[2] = (u8) (192.0f + (63.0f * t));
    col.c[3] = (u8) (192.0f + (63.0f * (0.007874016f * (f32) (frand() & 0x7F))));
    col2 = col;
    vel2 = vel;
    srcPos2 = srcPos;
    return MB2_CoinEffCreate(&srcPos2.x, &vel2.x, col2.c, farg0, farg1, 0.0f);
}

s32 MB2_CoinEffCreateColRand(f32 *arg0, s32 arg1, f32 farg0, f32 farg1, f32 farg2, f32 farg3, f32 farg4) {
    Point3d *pos;
    Point3d srcPos;
    Point3d vel;
    Point3d srcPos2;
    Point3d vel2;
    MB2_COL4 col;
    MB2_COL4 col2;
    f32 spare;
    u32 idx;

    pos = (Point3d *) arg0;
    if (mb2_CoinEffObj == NULL) {
        return -1;
    }
    srcPos.x = pos->x + (farg2 * (-0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
    srcPos.y = pos->y + (farg3 * (-0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
    srcPos.z = pos->z + farg4;
    vel.x = 0.0f;
    vel.z = 0.0f;
    switch (arg1) {
    case 1:
        vel.y = -1.0f * (1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F))));
        spare = 0.0f;
        break;
    case 2:
        vel.y = 1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F)));
        spare = 0.1f;
        break;
    default:
        vel.y = 0.0f;
        spare = 0.0f;
        break;
    }
    idx = (frand() & 0x7FFF) % 7;
    col = *(MB2_COL4 *) mb2ev_CoinEffColTbl[idx];
    col2 = col;
    vel2 = vel;
    srcPos2 = srcPos;
    return MB2_CoinEffCreate(&srcPos2.x, &vel2.x, col2.c, farg0, farg1, 0.0f);
}

void MB2_SplashEffInit(void) {
    OMOBJ *obj;
    MB2_SPLASHEFF_DATA *dat;
    ANIMDATA *anim;
    s32 modelId;
    s32 i;
    MBEFFECT *eff;
    MBEFFECTDATA *p;
    s32 j;
    HU3DMODEL *modelP;

    obj = mb2_SplashEffObj = omAddObjEx(mbObjMan, -0x8000, 0, 0, -1, MB2_SplashEffUpdate);
    dat = obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MB2_SPLASHEFF_DATA), 0x10000000U);
    memset(dat, 0, sizeof(MB2_SPLASHEFF_DATA));
    dat->anim = anim = HuSprAnimRead(HuDataReadNum(0xCE0018, 0x10000000));
    for (i = 0; i < 2; i++) {
        dat->modelId[i] = modelId = MBEffCreate(anim, 0x100);
        modelP = &Hu3DData[dat->modelId[i]];
        eff = (MBEFFECT *) modelP->hookData;
        p = eff->data;
        for (j = 0; j < eff->num; j++, p++) {
            p->dispF = 0;
        }
        if (j == 0) {
            Hu3DModelLayerSet(dat->modelId[i], 1);
            eff->blendMode = 0;
        } else {
            Hu3DModelLayerSet(dat->modelId[i], 2);
            eff->blendMode = 1;
        }
        MBEffAttrSet(dat->modelId[i], 2);
        dat->count[i] = 0;
    }
}

void MB2_SplashEffUpdate(OMOBJ *obj) {
    MB2_SPLASHEFF_DATA *dat;
    s32 i;
    s32 j;
    MBEFFECT *eff;
    MBEFFECTDATA *p;
    HU3DMODEL *modelP;
    f32 t;

    dat = (MB2_SPLASHEFF_DATA *) obj->data;
    if (MBKillCheck() || (mb2_SplashEffObj == NULL)) {
        for (i = 0; i < 2; i++) {
            if (dat->modelId[i] != -1) {
                MBEffKill(dat->modelId[i]);
            }
            dat->modelId[i] = -1;
        }
        omDelObjEx(mbObjMan, obj);
        mb2_SplashEffObj = NULL;
        return;
    }
    for (i = 0; i < 2; i++) {
        modelP = &Hu3DData[dat->modelId[i]];
        eff = (MBEFFECT *) modelP->hookData;
        p = eff->data;
        for (j = 0; j < eff->num; j++, p++) {
            if (p->dispF) {
                PSVECAdd((Point3d *) &p->pos, (Point3d *) &p->vel, (Point3d *) &p->pos);
                p->vel.y -= p->accel.y;
                t = (f32) p->time / (f32) p->parManId;
                p->time--;
                p->color.a = (u8) (p->accel.z * t);
                if (t <= 0.0f) {
                    p->dispF = 0;
                    dat->count[i]--;
                }
            }
        }
    }
}

void MB2_SplashEffClose(void) {
    mb2_SplashEffObj = NULL;
}

s32 MB2_SplashEffNumGet(void) {
    OMOBJ *obj;
    MB2_SPLASHEFF_DATA *dat;

    obj = mb2_SplashEffObj;
    if (mb2_SplashEffObj == NULL) {
        return 0;
    }
    dat = (MB2_SPLASHEFF_DATA *) obj->data;
    return dat->count[0] + dat->count[1];
}

void MB2_SplashEffCreate(HuVecF *arg0, HuVecF *arg1, u8 *arg2, s32 arg3, s32 arg4, f32 farg0, f32 farg1, f32 farg2) {
    OMOBJ *obj;
    MB2_SPLASHEFF_DATA *dat;
    HU3DMODEL *modelP;
    MBEFFECT *eff;
    MBEFFECTDATA *p;
    s32 j;
    s32 idx;

    obj = mb2_SplashEffObj;
#ifdef __MWERKS__
    dat = obj->data;
#endif
    if (mb2_SplashEffObj != NULL) {
#ifndef __MWERKS__
        dat = obj->data;
#endif
        if (arg4 == 0) {
            modelP = &Hu3DData[dat->modelId[0]];
        } else {
            modelP = &Hu3DData[dat->modelId[1]];
        }
        eff = (MBEFFECT *) modelP->hookData;
        p = eff->data;
        for (j = 0; j < eff->num; j++, p++) {
            if (!p->dispF) {
                break;
            }
        }
        if (j < eff->num) {
            p->time = p->parManId = arg3;
            p->pos = *arg0;
            p->scale = farg0;
            p->color.r = arg2[0];
            p->color.g = arg2[1];
            p->color.b = arg2[2];
            p->color.a = arg2[3];
            p->rot.z = farg1;
            p->animBank = 0;
            p->animNo = 0;
            p->animSpeed = 0.0f;
            p->animTime = 0.0f;
            p->dispF = 1;
            if (arg1 == NULL) {
                p->vel.x = p->vel.y = p->vel.z = 0.0f;
            } else {
                p->vel = *arg1;
            }
            p->accel.y = farg2;
            p->accel.z = (f32) arg2[3];
            if (arg4 == 0) {
                idx = 0;
            } else {
                idx = 1;
            }
            dat->count[idx]++;
        }
    }
}

void MB2_SplashEffCreateRandX(HuVecF *arg0, u8 *arg1, s32 arg2, f32 farg0) {
    HuVecF vel;
    u8 col[4];
    s32 life;
    f32 rotY;

    vel.x = 0.0f;
    vel.z = 0.0f;
    vel.y = 5.0f * (0.8f + (0.1f * (0.007874016f * (f32) (frand() & 0x7F))));
    col[0] = arg1[0];
    col[1] = arg1[1];
    col[2] = arg1[2];
    col[3] = arg1[3];
    life = 60.0f * (0.25f + (0.05f * (0.007874016f * (f32) (frand() & 0x7F))));
    rotY = 360.0f * (0.007874016f * (f32) (frand() & 0x7F));
    MB2_SplashEffCreate(arg0, &vel, col, life, arg2,
        farg0 * (100.0f * (0.75f + (0.5f * (0.007874016f * (f32) (frand() & 0x7F))))),
        rotY, 0.8166667f);
}

s16 MB2_ParticleCreate(ANIMDATA *animP, s16 num)
{
    MB2_PARTICLE *effP;
    MB2_PARTICLE_DATA *effDataP;
    s16 i;
    HuVec2f *st;
    HU3DMODEL *modelP;
    HuVecF *vtx;
    HU3DMODELID modelId;
    void *dlBuf;
    void *dlBegin;
    modelId = Hu3DHookFuncCreate(ParticleDraw);
    Hu3DModelCameraSet(modelId, HU3D_CAM0);
    modelP = &Hu3DData[modelId];
    modelP->hookData = effP = HuMemDirectMallocNum(HEAP_MODEL, sizeof(MB2_PARTICLE), modelP->mallocNo);
    effP->anim = animP;
    effP->num = num;
    effP->blendMode = HU3D_PARTICLE_BLEND_NORMAL;
    effP->dispAttr = MB2_PARTICLE_DISPATTR_NONE;
    effP->hook = NULL;
    effP->hookMdlP = NULL;
    effP->count = 0;
    effP->attr = MB2_PARTICLE_ATTR_NONE;
    effP->unk23 = 0;
    effP->prevCount = 0;
    effP->mode = effP->time = 0;
    effP->data = effDataP = HuMemDirectMallocNum(HEAP_MODEL, num*sizeof(MB2_PARTICLE_DATA), modelP->mallocNo);
    memset(effDataP, 0, num*sizeof(MB2_PARTICLE_DATA));
    for(i=0; i<num; i++, effDataP++) {
        effDataP->scale = 0;
        effDataP->rot.x = effDataP->rot.y = effDataP->rot.z = 0;
        effDataP->animTime = 0;
        effDataP->animSpeed = 1;
        effDataP->pos.x = ((frand() & 0x7F)-64)*20;
        effDataP->pos.y = ((frand() & 0x7F)-64)*30;
        effDataP->pos.z = ((frand() & 0x7F)-64)*20;
        effDataP->color.r = effDataP->color.g = effDataP->color.b = effDataP->color.a = 255;
        effDataP->animNo = 0;
    }
    effP->vertex = vtx = HuMemDirectMallocNum(HEAP_MODEL, num*sizeof(HuVecF)*4, modelP->mallocNo);
    for(i=0; i<num*4; i++, vtx++) {
        vtx->x = vtx->y = vtx->z = 0;
    }
    effP->st = st = HuMemDirectMallocNum(HEAP_MODEL, num*sizeof(HuVec2f)*4, modelP->mallocNo);
    for(i=0; i<num; i++) {
        st->x = 0;
        st->y = 0;
        st++;

        st->x = 1;
        st->y = 0;
        st++;

        st->x = 1;
        st->y = 1;
        st++;

        st->x = 0;
        st->y = 1;
        st++;
    }
    dlBegin = dlBuf = HuMemDirectMallocNum(HEAP_MODEL, 0x10000, modelP->mallocNo);
    DCFlushRange(dlBuf, 0x10000);
    GXBeginDisplayList(dlBegin, 0x10000);
    GXBegin(GX_QUADS, GX_VTXFMT0, num*4);
    for(i=0; i<num; i++) {
        GXPosition1x16(i*4);
        GXColor1x16(i);
        GXTexCoord1x16(i*4);

        GXPosition1x16((i*4)+1);
        GXColor1x16(i);
        GXTexCoord1x16((i*4)+1);

        GXPosition1x16((i*4)+2);
        GXColor1x16(i);
        GXTexCoord1x16((i*4)+2);

        GXPosition1x16((i*4)+3);
        GXColor1x16(i);
        GXTexCoord1x16((i*4)+3);
    }
    GXEnd();
    effP->dlSize = GXEndDisplayList();
    effP->dl = HuMemDirectMallocNum(HEAP_MODEL, effP->dlSize, modelP->mallocNo);
    memcpy(effP->dl, dlBuf, effP->dlSize);
    DCFlushRange(effP->dl, effP->dlSize);
    HuMemDirectFree(dlBuf);
    return modelId;
}

s16 MB2_ParticleCopy(s16 oldMdlId)
{
    MB2_PARTICLE *newParticleP; //r31
    MB2_PARTICLE_DATA *particleDataP; //r30
    HuVec2f *st; //r29
    s16 i; //r28
    HU3DMODEL *newModelP; //r27
    MB2_PARTICLE *oldParticleP; //r26
    HuVecF *pos; //r25
    int num; //r24
    HU3DMODEL *oldModelP; //r23
    HU3DMODELID newMdlId; //r22

    oldModelP = &Hu3DData[oldMdlId];
    oldParticleP = oldModelP->hookData;
    newMdlId = Hu3DHookFuncCreate(oldModelP->hookFunc);
    Hu3DModelCameraSet(newMdlId, HU3D_CAM0);
    newModelP = &Hu3DData[newMdlId];
    newModelP->hookData = newParticleP = HuMemDirectMallocNum(HEAP_MODEL, sizeof(MB2_PARTICLE), newModelP->mallocNo);
    newParticleP->anim = oldParticleP->anim;
    newParticleP->num = oldParticleP->num;
    num = newParticleP->num;
    newParticleP->blendMode = HU3D_PARTICLE_BLEND_NORMAL;
    newParticleP->dispAttr = oldParticleP->dispAttr;
    newParticleP->hook = NULL;
    newParticleP->hookMdlP = NULL;
    newParticleP->count = 0;
    newParticleP->attr = MB2_PARTICLE_ATTR_NONE;
    newParticleP->prevCount = 0;
    newParticleP->mode = newParticleP->time = 0;
    newParticleP->data = particleDataP = HuMemDirectMallocNum(HEAP_MODEL, sizeof(MB2_PARTICLE_DATA)*num, newModelP->mallocNo);
    for(i=0; i<num; i++, particleDataP++) {
        particleDataP->scale = 0;
        particleDataP->rot.x = particleDataP->rot.y = particleDataP->rot.z = 0;
        particleDataP->animTime = 0;
        particleDataP->animSpeed = 1;
        particleDataP->pos.x = ((frand()&0x7F)-64)*20;
        particleDataP->pos.y = ((frand()&0x7F)-64)*30;
        particleDataP->pos.z = ((frand()&0x7F)-64)*20;
        particleDataP->color.r = particleDataP->color.g = particleDataP->color.b = particleDataP->color.a = 255;
        particleDataP->animNo = 0;
    }
    newParticleP->vertex = pos = HuMemDirectMallocNum(HEAP_MODEL, sizeof(HuVecF)*num*4, newModelP->mallocNo);
    for(i=0; i<num*4; i++, pos++) {
        pos->x = pos->y = pos->z = 0;
    }
    newParticleP->st = st = HuMemDirectMallocNum(HEAP_MODEL, sizeof(HuVec2f)*num*4, newModelP->mallocNo);
    for(i=0; i<num; i++) {
        st->x = 0;
        st->y = 0;
        st++;
        st->x = 1;
        st->y = 0;
        st++;
        st->x = 1;
        st->y = 1;
        st++;
        st->x = 0;
        st->y = 1;
        st++;
    }
    newParticleP->dlSize = oldParticleP->dlSize;
    newParticleP->dl = oldParticleP->dl;
    return newMdlId;
}

void ParticleDraw(HU3DMODEL *modelP, Mtx *mtx)
{
    MB2_PARTICLE *effP;
    MB2_PARTICLE_DATA *effDataP;
    HuVecF *vtx;
    HuVec2f *st;
    HuVecF *scaleVtxP;
    s16 i;
    s16 j;
    HuVecF *initVtxP;
    s16 bmpFmt;
    s16 row;
    s16 col;
    MB2_PARTICLE_HOOK hook;

    Mtx mtxInv;
    Mtx mtxPos;
    Mtx mtxRotZ;
    HuVecF scaleVtx[4];
    HuVecF finalVtx[4];
    HuVecF initVtx[4];
    ROMtx basePosMtx;

    static HuVecF vtxTbl[] = {
        { -0.5f,  0.5f, 0.0f },
        {  0.5f,  0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        { -0.5f, -0.5f, 0.0f }
    };

    static HuVec2f uvTbl[] = {
        { 0.0f, 0.0f },
        { 0.25f, 0.0f },
        { 0.25f, 0.25f },
        { 0.0f, 0.25f },
    };

    effP = modelP->hookData;
    if(!shadowModelDrawF) {
        if(effP->hookMdlP && effP->hookMdlP != modelP){
            ParticleDraw(effP->hookMdlP, mtx);
        }
        GXLoadPosMtxImm(*mtx, GX_PNMTX0);
        GXSetNumTevStages(1);
        GXSetNumTexGens(1);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        if(shadowModelDrawF) {
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ONE, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
            GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        } else {
            bmpFmt = (effP->anim->bmp->dataFmt & 0xF);
            if(bmpFmt == ANIM_BMP_I8 || bmpFmt == ANIM_BMP_I4) {
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_RASC, GX_CC_ZERO);
            } else {
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
            }
            if(effP->dispAttr & MB2_PARTICLE_DISPATTR_ZBUF_OFF) {
                GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
            } else if(modelP->attr & HU3D_ATTR_ZWRITE_OFF) {
                GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
            } else {
                GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
            }
        }
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        HuSprTexLoad(effP->anim, 0, GX_TEXMAP0, GX_REPEAT, GX_REPEAT, GX_LINEAR);
        GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
        GXSetZCompLoc(GX_FALSE);
        switch (effP->blendMode) {
            case HU3D_PARTICLE_BLEND_NORMAL:
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
                break;
            case HU3D_PARTICLE_BLEND_ADDCOL:
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
                break;
            case HU3D_PARTICLE_BLEND_INVCOL:
                GXSetBlendMode(GX_BM_BLEND, GX_BL_ZERO, GX_BL_INVSRCALPHA, GX_LO_NOOP);
                break;
        }
        if(HmfInverseMtxF3X3(*mtx, mtxInv) == FALSE) {
            PSMTXIdentity(mtxInv);
        }
        PSMTXReorder(mtxInv, basePosMtx);
        if(effP->hook) {
            hook = effP->hook;
            hook(modelP, effP, mtx);
        }
        effDataP = effP->data;
        vtx = effP->vertex;
        st = effP->st;
        if(effP->dispAttr & MB2_PARTICLE_DISPATTR_CAMERA_ROT) {
            MTXIdentity(mtxInv);
            MTXIdentity(*(Mtx *)(&basePosMtx));
            initVtx[0] = vtxTbl[0];
            initVtx[1] = vtxTbl[1];
            initVtx[2] = vtxTbl[2];
            initVtx[3] = vtxTbl[3];
        } else {
            PSMTXROMultVecArray(basePosMtx, &vtxTbl[0], initVtx, 4);
        }
        for(i=0; i<effP->num; i++, effDataP++) {
            if(!effDataP->scale) {
                vtx->x = vtx->y = vtx->z = 0;
                vtx++;
                vtx->x = vtx->y = vtx->z = 0;
                vtx++;
                vtx->x = vtx->y = vtx->z = 0;
                vtx++;
                vtx->x = vtx->y = vtx->z = 0;
                vtx++;
            } else if(effP->dispAttr & MB2_PARTICLE_DISPATTR_ROT3D) {
                VECScale(&vtxTbl[0], &scaleVtx[0], effDataP->scale);
                VECScale(&vtxTbl[1], &scaleVtx[1], effDataP->scale);
                VECScale(&vtxTbl[2], &scaleVtx[2], effDataP->scale);
                VECScale(&vtxTbl[3], &scaleVtx[3], effDataP->scale);
                mtxRot(mtxPos, effDataP->rot.x, effDataP->rot.y, effDataP->rot.z);
                PSMTXMultVecArray(mtxPos, scaleVtx, finalVtx, 4);
                VECAdd(&finalVtx[0], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[1], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[2], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[3], &effDataP->pos, vtx++);
            } else if(!effDataP->rot.z) {
                scaleVtxP = scaleVtx;
                initVtxP = initVtx;
                VECScale(initVtxP++, scaleVtxP, effDataP->scale);
                VECAdd(scaleVtxP++, &effDataP->pos, vtx++);
                VECScale(initVtxP++, scaleVtxP, effDataP->scale);
                VECAdd(scaleVtxP++, &effDataP->pos, vtx++);
                VECScale(initVtxP++, scaleVtxP, effDataP->scale);
                VECAdd(scaleVtxP++, &effDataP->pos, vtx++);
                VECScale(initVtxP++, scaleVtxP, effDataP->scale);
                VECAdd(scaleVtxP++, &effDataP->pos, vtx++);
            } else {
                VECScale(&vtxTbl[0], &scaleVtx[0], effDataP->scale);
                VECScale(&vtxTbl[1], &scaleVtx[1], effDataP->scale);
                VECScale(&vtxTbl[2], &scaleVtx[2], effDataP->scale);
                VECScale(&vtxTbl[3], &scaleVtx[3], effDataP->scale);
                MTXRotRad(mtxRotZ, 'Z', effDataP->rot.z);
                PSMTXConcat(mtxInv, mtxRotZ, mtxPos);
                PSMTXMultVecArray(mtxPos, scaleVtx, finalVtx, 4);
                VECAdd(&finalVtx[0], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[1], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[2], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[3], &effDataP->pos, vtx++);

            }
        }
        effDataP = effP->data;
        st = effP->st;
        if(!(effP->dispAttr & MB2_PARTICLE_DISPATTR_NOANIM)) {
            for(i=0; i<effP->num; i++, effDataP++) {
                row = effDataP->animNo & 0x3;
                col = (effDataP->animNo >> 2) & 0x3;
                for(j=0; j<4; j++, st++) {
                    st->x = (0.25f*row)+uvTbl[j].x;
                    st->y = (0.25f*col)+uvTbl[j].y;
                }
            }
        } else {
            for(i=0; i<effP->num; i++, effDataP++) {
                for(j=0; j<4; j++, st++) {
                    st->x = 4*uvTbl[j].x;
                    st->y = 4*uvTbl[j].y;
                }
            }
        }
        DCFlushRangeNoSync(effP->vertex, effP->num*sizeof(HuVecF)*4);
        DCFlushRangeNoSync(effP->st, effP->num*sizeof(HuVec2f)*4);
        DCFlushRangeNoSync(effP->data, effP->num*sizeof(MB2_PARTICLE_DATA));
        PPCSync();
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSETARRAY(GX_VA_POS, effP->vertex, effP->num*sizeof(HuVecF)*4, sizeof(HuVecF), TRUE);
        GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        GXSETARRAY(GX_VA_CLR0, &effP->data->color, effP->num*sizeof(MB2_PARTICLE_DATA), sizeof(MB2_PARTICLE_DATA), TRUE);
        GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSETARRAY(GX_VA_TEX0, effP->st, effP->num*sizeof(HuVec2f)*4, sizeof(HuVec2f), TRUE);
        GXCallDisplayList(effP->dl, effP->dlSize);
        if(shadowModelDrawF == FALSE) {
            if(!(effP->attr & MB2_PARTICLE_ATTR_COUNTER_UPDATE)) {
                effP->count++;
            }
            if(effP->prevCount != 0 && effP->prevCount <= effP->count) {
                if(effP->attr & MB2_PARTICLE_ATTR_COUNTER_RESET) {
                    effP->count = 0;
                }
                effP->count = effP->prevCount;
            }
        }
    }
}

static f32 yajiAngleTbl[3] = { -45.0f, 0.0f, 45.0f };

void MB2_YajiInit(Point3d *pos) {
    OMOBJ *obj;
    f32 ang;
    s32 *dat;
    s32 i;

    obj = mb2_YajiObj = omAddObjEx(mbObjMan, -0x8000, 3U, 0U, -1, MB2_YajiUpdate);
    dat = obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(s32), 0x10000000U);
    *dat = 0;
    for (i = 0; i < 3; i++) {
        ang = yajiAngleTbl[i];
        obj->mdlId[i] = Hu3DModelCreate(HuDataSelHeapReadNum(0x80000, 0x10000000, HEAP_MODEL));
        Hu3DModelCameraSet(obj->mdlId[i], 1U);
        Hu3DModelAttrSet(obj->mdlId[i], 1U);
        Hu3DModelLayerSet(obj->mdlId[i], 3);
        Hu3DModelPosSet(obj->mdlId[i],
            (f32) ((f64) pos->x + (3.0 * (100.0 * sin((3.141592653589793 * (f64) ang) / 180.0)))),
            100.0f + pos->y,
            (f32) ((f64) pos->z + (3.0 * (100.0 * cos((3.141592653589793 * (f64) ang) / 180.0)))));
        Hu3DModelRotSet(obj->mdlId[i], 0.0f, ang, 0.0f);
    }
    obj->work[0] = 1;
    obj->stat |= 0x100;
}

void MB2_YajiUpdate(OMOBJ *obj) {
    s32 i;
    s32 *dat;
    HU3DMODEL *modelP;
    f32 scale;

    dat = (s32 *) obj->data;
    if (MBKillCheck() || (mb2_YajiObj == NULL)) {
        omDelObjEx(mbObjMan, obj);
        mb2_YajiObj = NULL;
        return;
    }
    for (i = 0; i < 3; i++) {
        modelP = &Hu3DData[obj->mdlId[i]];
        scale = modelP->scale.z;
        if ((modelP->attr & 1) == 0) {
            if ((u32) obj->work[0] == (u32) i) {
                *dat = *dat + 3;
                scale = (f32) (2.0 + fabs(sin((3.141592653589793 * (f64) (*dat % 360)) / 180.0)));
            } else {
                if (scale > 2.0f) {
                    scale -= 0.1f;
                }
                if (scale < 2.0f) {
                    scale += 0.1f;
                }
            }
            Hu3DModelScaleSet(obj->mdlId[i], scale, scale, scale);
        }
    }
}

void MB2_YajiClose(void) {
    mb2_YajiObj = NULL;
}

void MB2_YajiHiliteSet(u32 arg0) {
    OMOBJ *obj;
    s32 *dat;

    obj = mb2_YajiObj;
    dat = (s32 *) obj->data;
    if (mb2_YajiObj != NULL) {
        obj->work[0] = arg0;
        *dat = 0;
    }
}

void MB2_YajiDispSet(s32 arg0) {
    OMOBJ *obj;
    s32 i;
    s32 *dat;

    obj = mb2_YajiObj;
    dat = (s32 *) obj->data;
    if (mb2_YajiObj != NULL) {
        for (i = 0; i < 3; i++) {
            if (arg0 != 0) {
                Hu3DModelAttrReset(obj->mdlId[i], 1U);
            } else {
                Hu3DModelAttrSet(obj->mdlId[i], 1U);
            }
        }
        *dat = 0;
    }
}

/* mb2_RainbowObj->data: 0xC-byte header {s32 modelId; ANIMDATA *srcAnim;
 * void *slots (8 entries, stride 0x28)}. Each rainbow-arc slot: +0x00
 * activeF, +0x04 vtx count *3, +0x08 vtx array, +0x0C rot array, +0x10/0x14/
 * 0x18 more per-vtx arrays (draw-side), +0x1C..0x1F RGBA (alpha at +0x1F),
 * +0x20 display list, +0x24 display list size. Confirmed by
 * MB2_RainbowObjAlphaSet/VtxTopGet's simple, unambiguous field usage. */
typedef struct MB2RainbowSlot_s {
    s32 activeF;   /* 0x00 */
    s32 vtxNum;    /* 0x04 */
    Point3d *vtx;  /* 0x08 */
    Point3d *ang;  /* 0x0C */
    Point3d *posArr; /* 0x10, 2 vertices per arc point */
    f32 *uvArr;    /* 0x14, 2 floats per vertex */
    u8 *colArr;    /* 0x18, RGBA per vertex */
    u8 color[3];   /* 0x1C */
    union {
        u8 u;
        s8 s;
    } alpha;       /* 0x1F */
    void *dl;      /* 0x20 */
    u32 dlSize;    /* 0x24 */
} MB2_RAINBOW_SLOT; /* 0x28 on GC */

typedef struct MB2RainbowObjData_s {
    s32 modelId;   /* 0x00 */
    ANIMDATA *srcAnim; /* 0x04 */
    MB2_RAINBOW_SLOT *slots; /* 0x08, 8 entries */
} MB2_RAINBOWOBJ_DATA;

void MB2_RainbowObjInit(void) {
    OMOBJ *obj;
    HU3DMODEL *modelP;
    MB2_RAINBOWOBJ_DATA *dat;
    MB2_RAINBOW_SLOT *slots;
    s32 i;

    obj = mb2_RainbowObj = omAddObjEx(mbObjMan, -0x8000, 0, 0, -1, MB2_RainbowObjUpdate);
    dat = obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MB2_RAINBOWOBJ_DATA), 0x10000000U);
    memset(dat, 0, sizeof(MB2_RAINBOWOBJ_DATA));
    dat->modelId = Hu3DHookFuncCreate(RainbowObjDraw);
    Hu3DModelCameraSet((s16) dat->modelId, 1U);
    Hu3DModelLayerSet((s16) dat->modelId, 3);
    modelP = &Hu3DData[dat->modelId];
    modelP->pos.y = -100000.0f;
    modelP->pos.z = -100000.0f;
    modelP->hookData = dat;
    dat->srcAnim = mb2_Work.anim;
    slots = (MB2_RAINBOW_SLOT *) HuMemDirectMallocNum(HEAP_MODEL, 8 * sizeof(MB2_RAINBOW_SLOT), modelP->mallocNo);
    dat->slots = slots;
    memset(slots, 0, 8 * sizeof(MB2_RAINBOW_SLOT));
    for (i = 0; i < 8; i++, slots++) {
        slots->activeF = 0;
    }
}

void MB2_RainbowObjUpdate(OMOBJ *obj) {
    MB2_RAINBOWOBJ_DATA *dat;
    MB2_RAINBOW_SLOT *slot;
    s32 i;

    dat = (MB2_RAINBOWOBJ_DATA *) obj->data;
    if (MBKillCheck() || (mb2_RainbowObj == NULL)) {
        Hu3DModelKill((s16) dat->modelId);
        omDelObjEx(mbObjMan, obj);
        mb2_RainbowObj = NULL;
        return;
    }
    slot = dat->slots;
    for (i = 0; i < 8; i++, slot++) {
    }
}

void MB2_RainbowObjClose(void) {
    mb2_RainbowObj = NULL;
}

s32 MB2_RainbowObjCreate(Point3d *arg0, s32 arg1, s32 arg2) {
    OMOBJ *obj;
    MB2_RAINBOWOBJ_DATA *dat;
    HU3DMODEL *modelP;
    MB2_RAINBOW_SLOT *slot;
    s32 slotNo;
    Point3d *out;
    f32 *uvOut;
    u8 *colOut;
    void *dispListTmp;
    Point3d p0;
    Point3d p1;
    Point3d p2;
    Point3d p3;
    Point3d delta; /* not reset per iteration: the last vertex keeps the previous delta */
    f32 distSq;
    f32 dist;
    s32 i;
    s32 k;

    obj = mb2_RainbowObj;
    if (mb2_RainbowObj == NULL) {
        return -1;
    }
    dat = (MB2_RAINBOWOBJ_DATA *) obj->data;
    slot = dat->slots;
    modelP = &Hu3DData[dat->modelId];
    for (slotNo = 0; slotNo < 8; slotNo++, slot++) {
        if (slot->activeF == 0) {
            break;
        }
    }
    if (slotNo >= 8) {
        return -1;
    }
    slot->activeF = 1;

    slot->vtx = (Point3d *) HuMemDirectMallocNum(HEAP_MODEL, arg1 * 3 * sizeof(Point3d), modelP->mallocNo);
    out = slot->vtx;
    memset(slot->vtx, 0, arg1 * 3 * sizeof(Point3d));
    for (i = 0; i < arg1 - 1; i++) {
        if (i > 0) {
            p0 = arg0[i - 1];
        } else {
            p0 = arg0[i];
        }
        p1 = arg0[i];
        p2 = arg0[i + 1];
        if (i < arg1 - 2) {
            p3 = arg0[i + 2];
        } else {
            p3 = arg0[i + 1];
        }
        for (k = 0; k < 3; k++) {
            MB2_HermiteCalcV(&p0, &p1, &p2, &p3, out, (f32) k / 3.0f);
            out++;
        }
    }

    arg1 = (arg1 - 1) * 3;
    slot->vtxNum = arg1;
    slot->ang = (Point3d *) HuMemDirectMallocNum(HEAP_MODEL, arg1 * sizeof(Point3d), modelP->mallocNo);
    memset(slot->ang, 0, arg1 * sizeof(Point3d));
    for (i = 0; i < arg1; i++) {
        if (i < arg1 - 1) {
            PSVECSubtract(&slot->vtx[i + 1], &slot->vtx[i], &delta);
        }
        distSq = (delta.x * delta.x) + (delta.z * delta.z);
        dist = sqrtf(distSq);
        slot->ang[i].x = 180.0 * (atan2(dist, delta.y) / 3.141592653589793);
        slot->ang[i].y = 180.0 * (atan2(delta.x, delta.z) / 3.141592653589793);
        slot->ang[i].z = 0.0f;
    }

    slot->alpha.u = 0xFF;
    slot->color[2] = 0xFF;
    slot->color[1] = 0xFF;
    slot->color[0] = 0xFF;

    slot->posArr = (Point3d *) HuMemDirectMallocNum(HEAP_MODEL, arg1 * sizeof(Point3d) * 2, modelP->mallocNo);
    out = slot->posArr;
    memset(slot->posArr, 0, arg1 * sizeof(Point3d));
    slot->uvArr = (f32 *) HuMemDirectMallocNum(HEAP_MODEL, arg1 * 8 * 2, modelP->mallocNo);
    uvOut = slot->uvArr;
    memset(slot->uvArr, 0, arg1 * 8);
    slot->colArr = (u8 *) HuMemDirectMallocNum(HEAP_MODEL, arg1 * 4 * 2, modelP->mallocNo);
    colOut = slot->colArr;
    memset(slot->colArr, 0, arg1 * 4);

    for (i = 0; i < arg1; i++) {
        out->x = slot->vtx[i].x + 75.0 * sin((3.141592653589793 * (90.0f + slot->ang[i].y)) / 180.0);
        out->z = slot->vtx[i].z + 75.0 * cos((3.141592653589793 * (90.0f + slot->ang[i].y)) / 180.0);
        out->y = slot->vtx[i].y;
        uvOut[0] = 0.0f;
        uvOut[1] = (1.0f / (f32) arg1) * (f32) i;
        colOut[2] = 0xC0;
        colOut[1] = 0xC0;
        colOut[0] = 0xC0;
        if (i < 5) {
            colOut[3] = (u8) (192.0f * ((f32) i / 5.0f));
        } else if (i > arg1 - 5) {
            colOut[3] = (u8) (192.0f * ((f32) (5 - (i - (arg1 - 5))) / 5.0f));
        } else {
            colOut[3] = 0xC0;
        }
        out++;
        uvOut += 2;
        colOut += 4;
        out->x = slot->vtx[i].x + 75.0 * sin((3.141592653589793 * (slot->ang[i].y - 90.0f)) / 180.0);
        out->z = slot->vtx[i].z + 75.0 * cos((3.141592653589793 * (slot->ang[i].y - 90.0f)) / 180.0);
        out->y = slot->vtx[i].y;
        uvOut[0] = 1.0f;
        uvOut[1] = (1.0f / (f32) arg1) * (f32) i;
        colOut[2] = 0xFF;
        colOut[1] = 0xFF;
        colOut[0] = 0xFF;
        if (i < 5) {
            colOut[3] = (u8) (192.0f * ((f32) i / 5.0f));
        } else if (i > arg1 - 5) {
            colOut[3] = (u8) (192.0f * ((f32) (5 - (i - (arg1 - 5))) / 5.0f));
        } else {
            colOut[3] = 0xC0;
        }
        out++;
        uvOut += 2;
        colOut += 4;
    }

    DCFlushRange(slot->posArr, arg1 * sizeof(Point3d) * 2);
    DCFlushRange(slot->uvArr, arg1 * 8 * 2);
    DCFlushRange(slot->colArr, arg1 * 4 * 2);
    dispListTmp = HuMemDirectMallocNum(HEAP_MODEL, 0x10000, modelP->mallocNo);
    DCFlushRange(dispListTmp, 0x10000U);
    GXBeginDisplayList(dispListTmp, 0x10000U);
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (u16) (arg1 * 2));
    if (arg2 == 0) {
        for (i = 0; i < arg1; i++) {
            GXPosition1x16((u16) (i * 2));
            GXColor1x16((u16) (i * 2));
            GXTexCoord1x16((u16) (i * 2));
            GXPosition1x16((u16) ((i * 2) + 1));
            GXColor1x16((u16) ((i * 2) + 1));
            GXTexCoord1x16((u16) ((i * 2) + 1));
        }
    } else {
        for (i = arg1 - 1; i >= 0; i--) {
            GXPosition1x16((u16) ((i * 2) + 1));
            GXColor1x16((u16) (i * 2));
            GXTexCoord1x16((u16) ((i * 2) + 1));
            GXPosition1x16((u16) (i * 2));
            GXColor1x16((u16) ((i * 2) + 1));
            GXTexCoord1x16((u16) (i * 2));
        }
    }
    slot->dlSize = GXEndDisplayList();
    slot->dl = HuMemDirectMallocNum(HEAP_MODEL, slot->dlSize, modelP->mallocNo);
    memcpy(slot->dl, dispListTmp, slot->dlSize);
    DCFlushRange(slot->dl, slot->dlSize);
    HuMemDirectFree(dispListTmp);
    return slotNo;
}

void MB2_RainbowObjAlphaSet(s32 arg0, s8 arg1) {
    MB2_RAINBOWOBJ_DATA *dat;
    MB2_RAINBOW_SLOT *slot;
    OMOBJ *obj;

    obj = mb2_RainbowObj;
    if (mb2_RainbowObj != NULL) {
        dat = (MB2_RAINBOWOBJ_DATA *) obj->data;
        slot = dat->slots + arg0;
        slot->alpha.s = arg1;
    }
}

s32 MB2_RainbowObjVtxTopGet(s32 arg0, void **arg1) {
    MB2_RAINBOWOBJ_DATA *dat;
    MB2_RAINBOW_SLOT *slot;
    OMOBJ *obj;

    obj = mb2_RainbowObj;
    if (mb2_RainbowObj != NULL) {
        dat = (MB2_RAINBOWOBJ_DATA *) obj->data;
        slot = dat->slots + arg0;
        *arg1 = slot->vtx;
        arg0 = slot->vtxNum;
    }
    return arg0;
}

void RainbowObjDraw(HU3DMODEL *modelP, f32 (*mtx)[3][4]) {
    MB2_RAINBOWOBJ_DATA *dat;
    MB2_RAINBOW_SLOT *slot;
    GXColor tint;
    s32 i;

    if (mb2_RainbowObj == NULL) {
        return;
    }
    dat = (MB2_RAINBOWOBJ_DATA *) modelP->hookData;
    slot = dat->slots;
    if (shadowModelDrawF != 0) {
        return;
    }
    PSMTXCopy(Hu3DCameraMtx, (f32 (*)[4]) mtx);
    GXLoadPosMtxImm((f32 (*)[4]) mtx, 0U);
    GXSetNumTevStages(1U);
    GXSetNumTexGens(1U);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3CU, 0U, 0x7DU);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetZMode(1U, GX_LEQUAL, 0U);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1U, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A2, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1U, GX_TEVPREV);
    GXSetNumChans(1U);
    GXSetChanCtrl(GX_COLOR0A0, 0U, GX_SRC_REG, GX_SRC_VTX, 0U, GX_DF_CLAMP, GX_AF_NONE);
    HuSprTexLoad(dat->srcAnim, 0, 0, 1, 1, 1);
    GXSetAlphaCompare(GX_GEQUAL, 1U, GX_AOP_AND, GX_GEQUAL, 1U);
    GXSetZCompLoc(0U);
    GXSetCullMode(GX_CULL_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    slot = dat->slots;
    for (i = 0; i < 8; i++, slot++) {
        if (slot->activeF == 0) {
            continue;
        }
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0U);
        GXSETARRAY(GX_VA_POS, slot->posArr, slot->vtxNum * 0xC * 2, 0xCU, TRUE);
        GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_TEX_ST, GX_RGBA8, 0U);
        GXSETARRAY(GX_VA_CLR0, slot->colArr, slot->vtxNum * 4U * 2, 4U, TRUE);
        GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_RGBA6, 0U);
        GXSETARRAY(GX_VA_TEX0, slot->uvArr, slot->vtxNum * 8U * 2, 8U, TRUE);
        tint.r = slot->color[0];
        tint.g = slot->color[1];
        tint.b = slot->color[2];
        tint.a = slot->alpha.u;
        GXSetTevColor(GX_TEVREG2, tint);
        GXCallDisplayList(slot->dl, slot->dlSize);
    }
}
