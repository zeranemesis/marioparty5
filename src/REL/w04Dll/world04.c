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

/*
 * w04Dll - "Deep Bloober Sea" board.
 *
 * NOTE (port): mb4_Work is one flat scratch buffer (0x7F7C bytes) reused as
 * several different sub-structures, addressed with raw byte offsets in the
 * original code (typical of MWCC -O0 code that never got a single named
 * struct). Only the regions actually touched by the functions decompiled so
 * far are named below; the rest stays a raw byte array until reached.
 */

extern void ObjectSetup(void);

static void MB4_Create(void);
static void MB4_Kill(void);
void MB4_CoinClose(void);
void MB4_CoinEffClose(void);
void MB4_BubbleEffClose(void);
void MB4_SakanaClose(void);
s32 MB4_CoinNumGet(void);
s32 MB4_CoinEffNumGet(void);
void MB4_CoinEffBlendModeSet(u8 mode);
void MB4_CoinInit(void);
void MB4_CoinUpdate(OMOBJ *obj);
s32 MB4_CoinCreate(Point3d *basePos, Point3d *baseRot, s32 layer, f32 speed, f32 unused);
s32 MB4_CoinNearGet(Point3d *pos, Point3d *outPos, f32 maxDist);
s32 MB4_CoinEffCreate(Point3d *pos, Point3d *vel, u8 *color, f32 timer, f32 sizeParam, f32 alphaParam);
void MB4_SakanaLenSet(s32 partIdx, f32 lenRatio);
void MB4_SakanaOffsetSet(s32 partIdx, f32 offX, f32 offY, f32 offZ);
void MB4_SakanaPosSet(s32 partIdx, f32 x, f32 y, f32 z);
static void MB4Ev_Kihon(s32 playerNo);
static void MB4Ev_Iruka(s32 playerNo);
static void MB4Ev_Blow(s32 playerNo);
static void MB4Ev_Kaisoo(s32 playerNo);
static s16 PathCheck(s16 masuId, s16 depth);
static f32 MB4_PathTimeGet(Point3d *pPrev, Point3d *p1, Point3d *pNext, Point3d *unused, f32 tMax);
static f32 fn_1_DB68(f32 t, f32 pPrev, f32 p1, f32 pNext);
static f32 fn_1_DC9C(Point3d *pPrev, Point3d *p1, Point3d *pNext, f32 t);
static void MB4_BubbleEffInit(void);
static void MB4_SakanaInit(s32 timer, s32 arg1, s32 arg2, f32 phase);
static void MB4_CapsuleEffHook(int capsuleNo, int type, int mdlId, BOOL flag1, BOOL flag2, BOOL flag3);
static void MB4_MainObjUpdate(OMOBJ *obj);
static void MB4_LightSet(void);
static void MB4_LightReset(void);
static int MB4_MasuPathCheck(s16 id, u32 flag, s16 *linkTbl, int endF);
static s32 MB4Ev_MasuWalkPre(s32 playerNo);
static s32 MB4Ev_MasuWalkPost(s32 playerNo);
static void MB4Ev_MasuHatena(s32 playerNo);
static void MB4_MapViewHook(int enterF);
static void MB4_ModelStub(HU3DMODEL *a, HU3DMODEL *b, HU3DMODEL *c, HU3DMODEL *d);
static void MB4_CapsuleBubbleUpdate(void);
static s32 MB4_BubbleEffCreate(Point3d *pos, s32 duration, f32 speed);
static void MB4Ev_PlayerBubbleCreate(s32 playerNo);

static s16 mb4_PathValidF;
static s16 mb4_PathLen;
static s16 mb4_PathMasuNo;

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

static OMOBJ *mb4_MainObj;

/* mb4_Work[0..8]: model ids, mb4_Work[6]/[7]: "Puku"(pufferfish) model ids
 * used by the fright/hatena walk-post reaction (see MB4Ev_MasuWalkPost).
 * mb4_Work[0x57..0x59]: generic 3-word state slot reused by several events
 * (state/substate/timer-ish triplet), only named per-caller as reached. */
static s32 mb4_Work[0x1FF7];

#define MB4W_S16(off) (*(s16 *) ((u8 *) mb4_Work + (off)))
#define MB4W_U16(off) (*(u16 *) ((u8 *) mb4_Work + (off)))
#define MB4W_S32(off) (*(s32 *) ((u8 *) mb4_Work + (off)))
#define MB4W_F32(off) (*(f32 *) ((u8 *) mb4_Work + (off)))

/* Shared "float particle" record (size 0x5C = 92 bytes), confirmed by
 * reading MB4_CoinEffCreate and MB4_SakanaOffsetSet/PosSet/LenSet: the
 * same struct backs both the coin-sparkle effect array
 * (Hu3DData[coinEffModelIdx].hookData + 0x3C, count at +0x24) and the
 * sakana (fish) per-part array (Hu3DData[sakanaModelId] + 0x120). Field
 * meaning differs a little per user (e.g. offsetZAngle doubles as a random
 * spin angle for coin sparkles, an offset for sakana parts), but the byte
 * layout is identical in both. */
typedef struct MB4Particle_s {
    s32 active;     /* 0x00 */
    u8 pad4[4];     /* 0x04 */
    f32 velX;       /* 0x08 */
    f32 velY;       /* 0x0C */
    f32 velZ;       /* 0x10 */
    f32 speed;      /* 0x14 */
    f32 one;        /* 0x18, always set to 1.0f on spawn */
    f32 sizeParam;  /* 0x1C */
    u8 pad20[8];    /* 0x20 */
    f32 alphaParam; /* 0x24 */
    u8 pad28[4];    /* 0x28 */
    f32 timer;      /* 0x2C, coin-sparkle: countdown; sakana: len*100 */
    f32 offsetX;    /* 0x30 */
    f32 offsetY;    /* 0x34 */
    f32 offsetZAngle; /* 0x38 */
    f32 posX;       /* 0x3C */
    f32 posY;       /* 0x40 */
    f32 posZ;       /* 0x44 */
    u8 color[4];    /* 0x48, coin-sparkle only */
} MB4PARTICLE; /* size 0x5C */

/* Per-coin record (size 0x88 = 136 bytes), read from mb4_CoinObj->data
 * (32 entries) in MB4_CoinCreate/CoinInit/CoinNearGet. */
typedef struct MB4Coin_s {
    s32 modelId;       /* 0x00 */
    s32 shadowModelId; /* 0x04 */
    s32 active;        /* 0x08 */
    s32 unkC;          /* 0x0C */
    f32 speed;         /* 0x10 */
    s32 layer;         /* 0x14 */
    s32 state;         /* 0x18 */
    f32 posX;          /* 0x1C */
    f32 posY;          /* 0x20 */
    f32 posZ;          /* 0x24 */
    f32 rotX;          /* 0x28 */
    f32 rotY;          /* 0x2C */
    f32 rotZ;          /* 0x30 */
    f32 scaleZ;        /* 0x34 */
    f32 scaleY;        /* 0x38 */
    f32 scaleX;        /* 0x3C */
    f32 basePosX;      /* 0x40 */
    f32 basePosY;      /* 0x44 */
    f32 basePosZ;      /* 0x48 */
    s32 baseRotX;      /* 0x4C */
    s32 baseRotY;      /* 0x50 */
    s32 baseRotZ;      /* 0x54 */
    Point3d playerPos[4]; /* 0x58, one cached player position per player */
} MB4COIN; /* size 0x88 */

/* Coin-sparkle particle manager, pointed to by
 * Hu3DData[coinEffModelIdx].hookData (see MB4_CoinEffCreate/BlendModeSet). */
typedef struct MB4CoinEffMgr_s {
    u8 pad0[0x20];      /* 0x00 */
    u8 blendMode;       /* 0x20 */
    u8 pad21[3];        /* 0x21 */
    s16 count;           /* 0x24 */
    u8 pad26[0x16];       /* 0x26 */
    MB4PARTICLE *particles; /* 0x3C */
} MB4COINEFFMGR;

static Point3d mb4ev_Puku2EmiterPos = { -2800.0f, 300.0f, 1340.0f };

/* MB4_Create decoration placement tables, extracted from .data. */
static Point3d mb4ev_KaiPosTbl[3] = {
    { 1784.0f, 190.0f, 1017.0f },
    { 1883.0f, 190.0f, 1116.0f },
    { 1983.0f, 190.0f, 1215.0f },
};
static Point3d mb4ev_Puku2Pos = { 2041.0f, 230.0f, 958.0f };
static Point3d mb4ev_AkariPosTbl[2] = {
    { 2537.0f, 600.0f, -423.0f },
    { 1983.0f, 600.0f, -652.0f },
};
static f32 mb4ev_AkariRotYTbl[2] = { -92.5f, 47.5f };
static Point3d mb4ev_SKaiPosTbl[2] = {
    { 3427.0f, 1450.0f, -1786.0f },
    { 2318.0f, 1450.0f, -2245.0f },
};
static f32 mb4ev_SKaiRotYTbl[2] = { -22.5f, 157.5f };
static Point3d mb4ev_RBokPosTbl[2] = {
    { 3100.0f, -150.0f, 2450.0f },
    { -3100.0f, -150.0f, 2450.0f },
};
static f32 mb4ev_RBokRotYTbl[2] = { 0.0f, 0.0f };
static Point3d mb4ev_SangoOPosTbl[3] = {
    { 835.0f, -20.0f, -490.0f },
    { 3980.0f, -50.0f, 280.0f },
    { 1112.0f, 715.0f, -2113.0f },
};
static Point3d mb4ev_SangoORotTbl[3] = {
    { 0.0f, 36.0f, 0.0f },
    { -17.0f, -160.0f, 15.0f },
    { -91.0f, -115.0f, 87.0f },
};
static Point3d mb4ev_YaziPosTbl[7] = {
    { -735.0f, 455.0f, -1111.0f },
    { -2240.0f, 1167.0f, -972.0f },
    { -3724.0f, 1010.0f, -1010.0f },
    { -2511.0f, 515.0f, -78.0f },
    { 2810.0f, 505.0f, -75.0f },
    { 1567.0f, 555.0f, -668.0f },
    { 1340.0f, 855.0f, -1855.0f },
};
static f32 mb4ev_YaziRotTbl[7] = { 0.0f, 8.0f, -12.0f, 18.0f, -22.5f, -18.0f, -35.0f };
static Point3d mb4ev_YaziExtPosTbl[3] = {
    { -1303.0f, 7.0f, 545.0f },
    { -3212.0f, 387.0f, 406.0f },
    { 2900.0f, 105.0f, 1153.0f },
};
static f32 mb4ev_YaziExtRotYTbl[3] = { 35.0f, -30.0f, 0.0f };
static Point3d mb4ev_UeYaziPosTbl[1] = {
    { 1120.0f, 455.0f, -1100.0f },
};
static f32 mb4ev_UeYaziUpRotYTbl[1] = { 10.0f };
static Point3d mb4ev_HasigoPosTbl[4] = {
    { -3856.0f, 480.0f, -32.0f },
    { -2610.0f, 0.0f, 560.0f },
    { 2990.0f, 660.0f, -650.0f },
    { 3300.0f, 60.0f, 700.0f },
};
static Point3d mb4ev_HasigoRotTbl[4] = {
    { -10.0f, -5.0f, 0.0f },
    { -5.0f, 30.0f, 0.0f },
    { -5.0f, 10.0f, 0.0f },
    { -5.0f, 15.0f, 0.0f },
};
static Point3d mb4ev_Sango1PosTbl[3] = {
    { -3900.0f, 85.0f, 1190.0f },
    { -2580.0f, 953.0f, -2095.0f },
    { -904.0f, 367.0f, -1707.0f },
};
static Point3d mb4ev_Sango1RotTbl[3] = {
    { 0.0f, 0.0f, 0.0f },
    { -60.0f, -135.0f, 54.0f },
    { -75.0f, -150.0f, 78.0f },
};
static f32 mb4ev_Sango1ScaleTbl[3] = { 1.0f, 1.0f, 0.949999988f };
static Point3d mb4ev_Sango2PosTbl[2] = {
    { -609.0f, -84.0f, 174.0f },
    { -3963.0f, 397.0f, -123.0f },
};
static Point3d mb4ev_Sango2RotTbl[2] = {
    { -16.0f, 180.0f, 0.0f },
    { 34.0f, 9.0f, -14.0f },
};
static f32 mb4ev_Sango2ScaleTbl[2] = { 1.0f, 0.699999988f };
static Point3d mb4ev_Sango3PosTbl[6] = {
    { -2614.0f, -71.0f, 1954.0f },
    { -3877.0f, -56.0f, 1192.0f },
    { -2702.0f, 1002.0f, -1719.0f },
    { -1320.0f, 692.0f, -1024.0f },
    { 1570.0f, 450.0f, -1000.0f },
    { 3650.0f, 100.0f, 700.0f },
};
static Point3d mb4ev_Sango3RotTbl[6] = {
    { 56.0f, -7.0f, -14.0f },
    { 59.0f, -47.0f, 12.0f },
    { 0.0f, 0.0f, 0.0f },
    { 87.0f, 0.0f, -158.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
};
static Point3d mb4ev_Sango3ScaleTbl[6] = {
    { 1.20000005f, 1.20000005f, 1.20000005f },
    { 1.22000003f, 1.34000003f, 1.22000003f },
    { 1.29999995f, 1.29999995f, 1.29999995f },
    { 1.14999998f, 1.14999998f, 1.14999998f },
    { 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f },
};
static Point3d mb4ev_Sango4PosTbl[4] = {
    { -2660.0f, 83.0f, 1906.0f },
    { -2600.0f, 985.0f, -1485.0f },
    { 1800.0f, 440.0f, -980.0f },
    { 3770.0f, 90.0f, 880.0f },
};
static Point3d mb4ev_Sango4RotTbl[4] = {
    { 31.0f, -12.0f, -8.0f },
    { 9.0f, 23.0f, 0.0f },
    { 0.0f, 25.0f, 0.0f },
    { 0.0f, 36.0f, 0.0f },
};
static f32 mb4ev_Sango4ScaleTbl[4] = { 1.0f, 1.35000002f, 1.29999995f, 1.29999995f };
static Point3d mb4ev_Sango5PosTbl[2] = {
    { -1407.0f, 828.0f, -1003.0f },
    { -2140.0f, 1146.0f, -1255.0f },
};
static Point3d mb4ev_Sango5RotTbl[2] = {
    { 50.0f, -7.0f, 0.0f },
    { -76.0f, 246.0f, 67.0f },
};
static f32 mb4ev_Sango5ScaleTbl[2] = { 1.23000002f, 1.37f };
static Point3d mb4ev_LeftSango5PosTbl[5] = {
    { -3900.0f, 985.0f, -1068.0f },
    { -2380.0f, -109.0f, 2013.0f },
    { 3215.0f, 80.0f, 1195.0f },
    { 3095.0f, 480.0f, 0.0f },
    { 3433.0f, 1140.0f, -1015.0f },
};
static Point3d mb4ev_LeftSango5RotTbl[5] = {
    { 16.0f, -6.0f, 0.0f },
    { 42.0f, 9.0f, -2.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 28.0f, 0.0f },
    { 0.0f, 30.0f, 0.0f },
};
static f32 mb4ev_LeftSango5ScaleTbl[5] = { 1.16999996f, 1.50999999f, 1.5f, 1.20000005f, 1.29999995f };
static Point3d mb4ev_Hitode1PosTbl[4] = {
    { -1452.0f, -201.0f, 2038.0f },
    { -2525.0f, 287.0f, 356.0f },
    { -2327.0f, 298.0f, 252.0f },
    { 1540.0f, -48.0f, 1900.0f },
};
static Point3d mb4ev_Hitode1RotTbl[4] = {
    { 0.0f, 0.0f, 0.0f },
    { 90.0f, 24.0f, 0.0f },
    { 90.0f, 31.0f, 0.0f },
    { 22.5f, -42.0f, 0.0f },
};
static f32 mb4ev_Hitode1ScaleTbl[4] = { 1.0f, 0.569999993f, 0.340000004f, 0.600000024f };
static Point3d mb4ev_Hitode2PosTbl[4] = {
    { -3313.0f, 1497.0f, -929.0f },
    { -3429.0f, 1445.0f, -950.0f },
    { -3188.0f, 1445.0f, -906.0f },
    { 1760.0f, -85.0f, 2080.0f },
};
static Point3d mb4ev_Hitode2RotTbl[4] = {
    { 90.0f, -10.0f, 0.0f },
    { 85.0f, -9.0f, 29.0f },
    { 85.0f, 9.0f, 151.0f },
    { 27.0f, 11.0f, 6.0f },
};
static f32 mb4ev_Hitode2ScaleTbl[4] = { 1.0f, 0.649999976f, 0.649999976f, 1.0f };
static Point3d mb4ev_KaisooPosTbl[3] = {
    { -1570.0f, 455.0f, 57.0f },
    { -3071.0f, 550.0f, 44.0f },
    { -1075.0f, 1089.0f, -1404.0f },
};
static Point3d mb4ev_KaisooRotTbl[3] = {
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
};
static f32 mb4ev_KaisooScaleTbl[3] = { 1.0f, 1.0f, 1.0f };

static Point3d openingRot = { -33.0f, 0.0f, 0.0f };
static Point3d openingOfs = { 0.0f, 500.0f, 0.0f };
static f32 openingZoom = 16000.0f;
static Point3d mapViewRot = { -78.0f, 0.0f, 0.0f };
static Point3d mapViewPos = { -181.0f, -123.0f, 80.0f };
static f32 mapViewZoom = 16171.0f;

static OMOBJ *mb4_BubbleEffObj;
static OMOBJ *mb4_CoinObj;
static OMOBJ *mb4_CoinEffObj;
static Point3d mb4_CoinMaxDist;

static Point3d mb4ev_BubbleEmitPosTbl[13] = {
    { 3710.0f, 240.0f, 790.0f },
    { 2870.0f, 740.0f, -160.0f },
    { 1620.0f, 700.0f, -930.0f },
    { 2500.0f, 160.0f, 900.0f },
    { 3170.0f, 300.0f, 1170.0f },
    { 1940.0f, 120.0f, 550.0f },
    { 2860.0f, 330.0f, 760.0f },
    { 1120.0f, 320.0f, 60.0f },
    { -1470.0f, 680.0f, -900.0f },
    { -2700.0f, 100.0f, 2000.0f },
    { -2780.0f, 660.0f, 1200.0f },
    { -3950.0f, 190.0f, 780.0f },
    { -2000.0f, 370.0f, 260.0f },
};
static Point3d mb4ev_Puku2BubblePos = { -2800.0f, 300.0f, 1340.0f };
static Point3d mb4ev_IrukaStartBubblePos = { -3320.0f, 1250.0f, -840.0f };

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
    GWPartyFSet(TRUE);
    MBObjectSetup(3, MB4_Create, MB4_Kill);
}

/* ----------------------------- leaf helpers ----------------------------- */

/* NONMATCHING: complete and faithful transcription of the decompiled body
 * (board setup: all ~40 decoration models + their per-instance placement
 * loops, the fish/bubble/coin ambience state, and the board hooks); not
 * attempted for instruction-level match given its size (~1600 instructions,
 * 6448 bytes). All 0xD1xxxx dataNum/model IDs, mb4_Work[] slot indices, and
 * per-table pos/rot/scale values are taken directly from the original. */
void MB4_Create(void) {
    s16 modelId;
    s32 i;
    s32 *stateP;
    int puku2Sub[3];
    int puku2ShellSub[4];
    int sango6Sub[2];
    f32 sakanaPhase;

    HuAudSndGrpSetSet(GwSystem.boardNo);
    MBMasuCreate(0xD10000);

    modelId = MBModelCreate(0xD10001, NULL, FALSE);
    mb4_Work[0] = modelId;
    MBModelAttrSet(modelId, 0x40000001U);
    MB4_ModelStub(&Hu3DData[MBModelIdGet(modelId)], &Hu3DData[MBModelIdGet(modelId)], Hu3DData, Hu3DData);

    modelId = MBModelCreate(0xD10013, NULL, FALSE);
    mb4_Work[1] = modelId;
    MBModelAttrSet(modelId, 0x40000001U);
    MB4_ModelStub(&Hu3DData[MBModelIdGet(modelId)], Hu3DData, (HU3DMODEL *) (MBModelIdGet(modelId) * 0x134), NULL);

    modelId = MBModelCreate(0xD10002, NULL, FALSE);
    mb4_Work[2] = modelId;
    MB4_ModelStub(&Hu3DData[MBModelIdGet(modelId)], &Hu3DData[MBModelIdGet(modelId)], (HU3DMODEL *) (MBModelIdGet(modelId) * 0x134), (HU3DMODEL *) (uintptr_t) MBModelIdGet(modelId));

    modelId = MBModelCreate(0xD10010, NULL, FALSE);
    mb4_Work[4] = modelId;
    MBModelDispSet(modelId, FALSE);

    modelId = MBModelCreate(0xD10014, NULL, FALSE);
    mb4_Work[5] = modelId;
    MBModelAttrSet(modelId, 0x40000001U);
    MB4_ModelStub(&Hu3DData[MBModelIdGet(modelId)], &Hu3DData[MBModelIdGet(modelId)], (HU3DMODEL *) (MBModelIdGet(modelId) * 0x134), (HU3DMODEL *) (uintptr_t) MBModelIdGet(modelId));

    modelId = MBModelCreate(0xD10015, NULL, FALSE);
    mb4_Work[6] = modelId;
    MBMotionTimeSet(modelId, 0.0f);
    MBMotionSpeedSet(modelId, 0.0f);
    MBModelAttrSet(modelId, 0x40000002U);
    MB4_ModelStub(&Hu3DData[MBModelIdGet(modelId)], Hu3DData, (HU3DMODEL *) (uintptr_t) MBModelIdGet(modelId), NULL);

    modelId = MBModelCreate(0xD10016, NULL, FALSE);
    mb4_Work[7] = modelId;
    MBMotionTimeSet(modelId, 0.0f);
    MBMotionSpeedSet(modelId, 0.0f);
    MBModelAttrSet(modelId, 0x40000002U);
    MB4_ModelStub(&Hu3DData[MBModelIdGet(modelId)], &Hu3DData[MBModelIdGet(modelId)], Hu3DData, Hu3DData);
    MBModelDispSet(modelId, FALSE);

    modelId = MBModelCreate(0xD10017, NULL, FALSE);
    mb4_Work[8] = modelId;
    MBMotionTimeSet(modelId, 0.0f);
    MBMotionSpeedSet(modelId, 0.0f);
    MBModelAttrSet(modelId, 0x40000002U);
    MB4_ModelStub(&Hu3DData[MBModelIdGet(modelId)], Hu3DData, &Hu3DData[MBModelIdGet(modelId)], (HU3DMODEL *) (uintptr_t) MBModelIdGet(modelId));

    for (i = 0; i < 2; i++) {
        modelId = MBModelCreate(0xD10003, NULL, TRUE);
        MBModelPosSetV(modelId, &mb4ev_AkariPosTbl[i]);
        MBModelRotSet(modelId, 0.0f, mb4ev_AkariRotYTbl[i], 0.0f);
        MBModelAttrSet(modelId, 0x40000001U);
        MBModelLayerSet(modelId, 1U);
    }
    for (i = 0; i < 2; i++) {
        modelId = MBModelCreate(0xD10006, NULL, TRUE);
        MBModelPosSetV(modelId, &mb4ev_SKaiPosTbl[i]);
        MBModelRotSet(modelId, 0.0f, mb4ev_SKaiRotYTbl[i], 0.0f);
        MBModelLayerSet(modelId, 1U);
    }
    for (i = 0; i < 2; i++) {
        modelId = MBModelCreate(0xD10007, NULL, TRUE);
        MBModelPosSetV(modelId, &mb4ev_RBokPosTbl[i]);
        MBModelRotSet(modelId, 0.0f, mb4ev_RBokRotYTbl[i], 0.0f);
    }
    for (i = 0; i < 3; i++) {
        modelId = MBModelCreate(0xD10008, NULL, TRUE);
        MBModelPosSetV(modelId, &mb4ev_SangoOPosTbl[i]);
        MBModelRotSetV(modelId, &mb4ev_SangoORotTbl[i]);
        MBModelLayerSet(modelId, 1U);
    }
    for (i = 0; i < 7; i++) {
        modelId = MBModelCreate(0xD10018, NULL, TRUE);
        MBModelPosSetV(modelId, &mb4ev_YaziPosTbl[i]);
        MBModelRotSet(modelId, 0.0f, mb4ev_YaziRotTbl[i], 0.0f);
    }
    for (i = 0; i < 3; i++) {
        modelId = MBModelCreate(0xD10018, NULL, TRUE);
        MBModelPosSetV(modelId, &mb4ev_YaziExtPosTbl[i]);
        MBModelRotSet(modelId, 0.0f, 180.0f + mb4ev_YaziExtRotYTbl[i], 0.0f);
    }
    for (i = 0; i < 1; i++) {
        modelId = MBModelCreate(0xD10019, NULL, TRUE);
        MBModelPosSetV(modelId, &mb4ev_UeYaziPosTbl[i]);
        MBModelRotSet(modelId, 0.0f, mb4ev_UeYaziUpRotYTbl[i], 0.0f);
    }
    for (i = 0; i < 4; i++) {
        modelId = MBModelCreate(0xD10004, NULL, TRUE);
        MBModelPosSetV(modelId, &mb4ev_HasigoPosTbl[i]);
        MBModelRotSetV(modelId, &mb4ev_HasigoRotTbl[i]);
    }
    for (i = 0; i < 3; i++) {
        modelId = MBModelCreate(0xD1001E, NULL, TRUE);
        MBModelPosSetV(modelId, &mb4ev_Sango1PosTbl[i]);
        MBModelRotSetV(modelId, &mb4ev_Sango1RotTbl[i]);
        MBModelScaleSet(modelId, mb4ev_Sango1ScaleTbl[i], mb4ev_Sango1ScaleTbl[i], mb4ev_Sango1ScaleTbl[i]);
        MBModelLayerSet(modelId, 1U);
    }
    for (i = 0; i < 2; i++) {
        modelId = MBModelCreate(0xD1001F, NULL, TRUE);
        MBModelPosSetV(modelId, &mb4ev_Sango2PosTbl[i]);
        MBModelRotSetV(modelId, &mb4ev_Sango2RotTbl[i]);
        MBModelScaleSet(modelId, mb4ev_Sango2ScaleTbl[i], mb4ev_Sango2ScaleTbl[i], mb4ev_Sango2ScaleTbl[i]);
        if (i != 0) {
            MBModelLayerSet(modelId, 1U);
        }
        if (i == 0) {
            MBModelDispSet(modelId, FALSE);
        }
    }
    for (i = 0; i < 6; i++) {
        modelId = MBModelCreate(0xD10020, NULL, TRUE);
        MBModelPosSetV(modelId, &mb4ev_Sango3PosTbl[i]);
        MBModelRotSetV(modelId, &mb4ev_Sango3RotTbl[i]);
        MBModelScaleSetV(modelId, &mb4ev_Sango3ScaleTbl[i]);
        MBModelLayerSet(modelId, 1U);
    }
    for (i = 0; i < 4; i++) {
        modelId = MBModelCreate(0xD10021, NULL, TRUE);
        MBModelPosSetV(modelId, &mb4ev_Sango4PosTbl[i]);
        MBModelRotSetV(modelId, &mb4ev_Sango4RotTbl[i]);
        MBModelScaleSet(modelId, mb4ev_Sango4ScaleTbl[i], mb4ev_Sango4ScaleTbl[i], mb4ev_Sango4ScaleTbl[i]);
        MBModelLayerSet(modelId, 1U);
    }
    for (i = 0; i < 2; i++) {
        modelId = MBModelCreate(0xD10022, NULL, TRUE);
        MBModelPosSetV(modelId, &mb4ev_Sango5PosTbl[i]);
        MBModelRotSetV(modelId, &mb4ev_Sango5RotTbl[i]);
        MBModelScaleSet(modelId, mb4ev_Sango5ScaleTbl[i], mb4ev_Sango5ScaleTbl[i], mb4ev_Sango5ScaleTbl[i]);
        MBModelLayerSet(modelId, 1U);
    }
    for (i = 0; i < 5; i++) {
        modelId = MBModelCreate(0xD10022, NULL, TRUE);
        MBModelPosSetV(modelId, &mb4ev_LeftSango5PosTbl[i]);
        MBModelRotSetV(modelId, &mb4ev_LeftSango5RotTbl[i]);
        MBModelScaleSet(modelId, mb4ev_LeftSango5ScaleTbl[i], mb4ev_LeftSango5ScaleTbl[i], mb4ev_LeftSango5ScaleTbl[i]);
        MBModelLayerSet(modelId, 1U);
    }
    for (i = 0; i < 4; i++) {
        modelId = MBModelCreate(0xD10023, NULL, TRUE);
        MBModelPosSetV(modelId, &mb4ev_Hitode1PosTbl[i]);
        MBModelRotSetV(modelId, &mb4ev_Hitode1RotTbl[i]);
        MBModelScaleSet(modelId, mb4ev_Hitode1ScaleTbl[i], mb4ev_Hitode1ScaleTbl[i], mb4ev_Hitode1ScaleTbl[i]);
        MBModelLayerSet(modelId, 1U);
    }
    for (i = 0; i < 4; i++) {
        modelId = MBModelCreate(0xD10024, NULL, TRUE);
        MBModelPosSetV(modelId, &mb4ev_Hitode2PosTbl[i]);
        MBModelRotSetV(modelId, &mb4ev_Hitode2RotTbl[i]);
        MBModelScaleSet(modelId, mb4ev_Hitode2ScaleTbl[i], mb4ev_Hitode2ScaleTbl[i], mb4ev_Hitode2ScaleTbl[i]);
        MBModelLayerSet(modelId, 1U);
    }
    for (i = 0; i < 3; i++) {
        modelId = MBModelCreate(0xD10025, NULL, TRUE);
        MBModelPosSetV(modelId, &mb4ev_KaisooPosTbl[i]);
        MBModelRotSetV(modelId, &mb4ev_KaisooRotTbl[i]);
        MBModelScaleSet(modelId, mb4ev_KaisooScaleTbl[i], mb4ev_KaisooScaleTbl[i], mb4ev_KaisooScaleTbl[i]);
        MBModelAttrSet(modelId, 0x40000001U);
        MBModelLayerSet(modelId, 1U);
    }

    modelId = MBModelCreate(0xD10009, NULL, FALSE);
    mb4_Work[9] = modelId;
    MBModelPosSetV(modelId, &mb4ev_KaiPosTbl[0]);
    MBModelRotSet(modelId, 0.0f, -45.0f, 0.0f);
    MBMotionSpeedSet(modelId, 0.0f);
    MBMotionTimeSet(modelId, 0.0f);

    modelId = MBModelCreate(0xD1000A, NULL, FALSE);
    mb4_Work[0xA] = modelId;
    MBModelPosSetV(modelId, &mb4ev_KaiPosTbl[1]);
    MBModelRotSet(modelId, 0.0f, -45.0f, 0.0f);
    MBMotionSpeedSet(modelId, 0.0f);
    MBMotionTimeSet(modelId, 0.0f);

    modelId = MBModelCreate(0xD1000B, NULL, FALSE);
    mb4_Work[0xB] = modelId;
    MBModelPosSetV(modelId, &mb4ev_KaiPosTbl[2]);
    MBModelRotSet(modelId, 0.0f, -45.0f, 0.0f);
    MBMotionSpeedSet(modelId, 0.0f);
    MBMotionTimeSet(modelId, 0.0f);

    modelId = MBModelCreate(0xD1000C, NULL, FALSE);
    mb4_Work[0xC] = modelId;
    MBModelDispSet(modelId, FALSE);

    modelId = MBModelCreate(0xD1000D, NULL, FALSE);
    mb4_Work[0xD] = modelId;
    MBModelDispSet(modelId, FALSE);

    modelId = MBModelCreate(0xD1000E, NULL, FALSE);
    mb4_Work[0xE] = modelId;
    MBModelDispSet(modelId, FALSE);

    modelId = MBModelCreate(0xD10012, NULL, FALSE);
    mb4_Work[0xF] = modelId;
    MBModelLayerSet(modelId, 1U);

    puku2Sub[0] = 0xD10029;
    puku2Sub[1] = 0xD1002A;
    puku2Sub[2] = -1;
    modelId = MBModelCreate(0xD10028, puku2Sub, FALSE);
    mb4_Work[0x10] = modelId;
    MBModelPosSetV(modelId, &mb4ev_Puku2Pos);
    MBModelRotSet(modelId, 0.0f, -45.0f, 0.0f);
    MBModelScaleSet(modelId, 2.0f, 2.0f, 2.0f);
    MBMotionNoSet(modelId, 1, 0x40000001U);

    puku2ShellSub[0] = 0xD1002C;
    puku2ShellSub[1] = 0xD1002D;
    puku2ShellSub[2] = 0xD1002E;
    puku2ShellSub[3] = -1;
    modelId = MBModelCreate(0xD1002B, puku2ShellSub, FALSE);
    mb4_Work[0x11] = modelId;
    MBMotionNoSet(modelId, 1, 0x40000001U);
    Hu3DModelClusterAttrSet(MBModelIdGet(modelId), 0, -0x3FFFFFFF);
    MBModelAttrSet(modelId, 0x40000002U);
    MBModelDispSet(modelId, FALSE);

    sango6Sub[0] = 0xD10030;
    sango6Sub[1] = -1;
    modelId = MBModelCreate(0xD1002F, sango6Sub, FALSE);
    mb4_Work[0x12] = modelId;
    MBModelAttrSet(modelId, 0x40000002U);
    MBModelDispSet(modelId, FALSE);

    modelId = MBModelCreate(0xD1000F, NULL, FALSE);
    mb4_Work[0x13] = modelId;
    MBModelDispSet(modelId, FALSE);
    MBModelLayerSet(modelId, 2U);

    modelId = MBModelCreate(0xD10026, NULL, FALSE);
    mb4_Work[0x14] = modelId;
    MBModelDispSet(modelId, FALSE);
    MBModelLayerSet(modelId, 1U);
    MB4_ModelStub(&Hu3DData[MBModelIdGet(modelId)], Hu3DData, (HU3DMODEL *) (MBModelIdGet(modelId) * 0x134), (HU3DMODEL *) (uintptr_t) MBModelIdGet(modelId));

    modelId = MBModelCreate(0xD10011, NULL, FALSE);
    mb4_Work[0x15] = modelId;
    MBModelDispSet(modelId, FALSE);
    MBModelLayerSet(modelId, 1U);

    modelId = MBModelCreate(0xD10027, NULL, FALSE);
    mb4_Work[0x16] = modelId;
    MBModelAttrSet(modelId, 0x40000001U);
    MBModelLayerSet(modelId, 1U);

    MB4_BubbleEffInit();
    stateP = &mb4_Work[0x17];
    for (i = 0; i < 4; i++) {
        stateP[0] = (s32) (10.0f * (60.0f * (0.007874016f * (f32) (frand() & 0x7F))));
        stateP[1] = (s32) (5.0f + (10.0f * (0.007874016f * (f32) (frand() & 0x7F))));
        stateP[2] = (s32) frand();
        stateP += 3;
    }

    mb4_Work[0x24] = 0;
    mb4_Work[0x25] = (s32) frand();
    mb4_Work[0x26] = -1;
    mb4_Work[0x27] = -1;
    mb4_Work[0x28] = 0;
    mb4_Work[0x29] = 0;
    mb4_Work[0x2B] = 0;
    mb4_Work[0x2C] = (s32) frand();
    mb4_Work[0x2D] = -1;
    mb4_Work[0x2E] = -1;
    mb4_Work[0x2F] = 0;

    stateP = &mb4_Work[0x30];
    for (i = 0; i < 0xD; i++) {
        stateP[0] = (s32) (12.0f * (60.0f * (0.007874016f * (f32) (frand() & 0x7F))));
        stateP[1] = (s32) (5.0f + (10.0f * (0.007874016f * (f32) (frand() & 0x7F))));
        stateP[2] = (s32) frand();
        stateP += 3;
    }

    mb4_Work[0x57] = (s32) (12.0f * (60.0f * (0.007874016f * (f32) (frand() & 0x7F))));
    mb4_Work[0x58] = 0;
    mb4_Work[0x59] = 0;
    sakanaPhase = 60.0f * (0.007874016f * (f32) (frand() & 0x7F));
    mb4_Work[0x5A] = (s32) (12.0f * sakanaPhase);
    mb4_Work[0x5B] = 0;
    mb4_Work[0x5C] = 0;
    MB4_SakanaInit(mb4_Work[0x5A], 0, 0, sakanaPhase);

    MBCapsuleEffHookSet(MB4_CapsuleEffHook);
    MBScrollInit(0xD10031U, 20000.0f, 0.0f);
    MBLightHookSet(MB4_LightSet, MB4_LightReset);
    MBCapsuleColCreate(0xD10032);
    MBBranchFlagSet(0xA2U);
    MBMasuPathCheckHookSet(MB4_MasuPathCheck);
    HuDataDirClose(0xD10000);
    MBMasuWalkPreHookSet((MASUPLAYERHOOK) MB4Ev_MasuWalkPre);
    MBMasuWalkPostHookSet((MASUPLAYERHOOK) MB4Ev_MasuWalkPost);
    MBMasuHatenaHookSet((MASUPLAYERHOOK) MB4Ev_MasuHatena);
    mb4_MainObj = omAddObjEx(mbObjMan, -0x8000, 0U, 0U, -1, MB4_MainObjUpdate);
    MBOpeningViewSet(&openingRot, &openingOfs, openingZoom);
    MBMapViewCameraSet(&mapViewRot, &mapViewPos, mapViewZoom);
    MBMapViewHookSet(MB4_MapViewHook);
}

/* NONMATCHING: complete and faithful transcription, not attempted for
 * instruction-level match. Per-frame board ambience: player "get chased by
 * bubbles" trail (mb4_Work[0x17..0x23], one 3-word slot per player),
 * ambient background bubbles (mb4_Work[0x30..0x56], 13 slots), the
 * pufferfish (Puku2) bubble trail (mb4_Work[0x57..0x59]) and the dolphin's
 * starting bubble puff (mb4_Work[0x5A..0x5C]), plus the puku2/shell model
 * swap once its open-mouth motion finishes.
 *
 * NOTE: the `pos.x != 0 || pos.y != 0 || pos.y != 0` gate below (checking
 * .y twice, not .z) is reproduced exactly as decompiled - it reads the
 * *previous* loop iteration's MBPlayerPosGet() result (a local that is not
 * reset per iteration in the original either), which looks like a latent
 * bug in the original source rather than something to "fix" here. */
void MB4_MainObjUpdate(OMOBJ *obj) {
    Point3d pos;
    Point3d scale;
    s32 *stateP;
    s32 playerNo;
    s32 i;
    s32 timer;
    f32 bx, by, bz;

    if (MBKillCheck() || mb4_MainObj == NULL) {
        omDelObjEx(mbObjMan, obj);
        mb4_MainObj = NULL;
        return;
    }

    stateP = &mb4_Work[0x17];
    for (playerNo = 0; playerNo < 4; playerNo++) {
        if ((pos.x != 0.0f || pos.y != 0.0f || pos.y != 0.0f) &&
            MBPlayerMotionNoGet(playerNo) != 0xA &&
            MBPlayerMotionNoGet(playerNo) != 6 &&
            MBModelDispGet(MBPlayerModelGet(playerNo)) != 0) {
            stateP[2] += 1;
            timer = stateP[0] - 1;
            stateP[0] = timer;
            if (timer <= 0 && MBPlayerAliveCheck(playerNo)) {
                MBPlayerPosGet(playerNo, &pos);
                MBPlayerScaleGet(playerNo, &scale);
                if ((stateP[2] & 3) == 0) {
                    s32 burst = stateP[1] - 1;
                    stateP[1] = burst;
                    if (burst > 0) {
                        MB4Ev_PlayerBubbleCreate(playerNo);
                    } else {
                        stateP[0] = (s32) (300.0f + (5.0f * (60.0f * (0.007874016f * (f32) (frand() & 0x7F)))));
                        stateP[1] = (s32) (5.0f + (10.0f * (0.007874016f * (f32) (frand() & 0x7F))));
                    }
                }
            }
        }
        stateP += 3;
    }

    MB4_CapsuleBubbleUpdate();

    stateP = &mb4_Work[0x30];
    for (i = 0; i < 0xD; i++) {
        stateP[2] += 1;
        timer = stateP[0] - 1;
        stateP[0] = timer;
        if (timer <= 0 && (stateP[2] & 7) == 0) {
            s32 burst = stateP[1] - 1;
            stateP[1] = burst;
            if (burst > 0) {
                s32 duration = (s32) (120.0f * (1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F)))));
                MB4_BubbleEffCreate(&mb4ev_BubbleEmitPosTbl[i], duration, 1.5f * (0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
            } else {
                stateP[0] = (s32) (300.0f + (5.0f * (60.0f * (0.007874016f * (f32) (frand() & 0x7F)))));
                stateP[1] = (s32) (5.0f + (10.0f * (0.007874016f * (f32) (frand() & 0x7F))));
            }
        }
        stateP += 3;
    }

    if (mb4_Work[0x58] > 0) {
        s32 duration;
        Point3d puffPos;
        mb4_Work[0x59] += 1;
        timer = mb4_Work[0x57] - 1;
        mb4_Work[0x57] = timer;
        if (timer <= 0 && (mb4_Work[0x59] & 1) == 0) {
            s32 burst = mb4_Work[0x58] - 1;
            mb4_Work[0x58] = burst;
            if (burst > 0) {
                bx = mb4ev_Puku2BubblePos.x;
                by = mb4ev_Puku2BubblePos.y;
                bz = mb4ev_Puku2BubblePos.z;
                bx -= 0.5f * (100.0f * (0.007874016f * (f32) (frand() & 0x7F)));
                by += 0.5f * (100.0f * (0.007874016f * (f32) (frand() & 0x7F)));
                bz -= 0.25f * (100.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
                duration = (s32) (120.0f * (1.0f + (0.5f * (0.007874016f * (f32) (frand() & 0x7F)))));
                puffPos.x = bx;
                puffPos.y = by;
                puffPos.z = bz;
                MB4_BubbleEffCreate(&puffPos, duration, 1.5f * (0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
            }
        }
    }

    if (mb4_Work[0x5B] > 0) {
        s32 duration;
        Point3d puffPos;
        mb4_Work[0x5C] += 1;
        timer = mb4_Work[0x5A] - 1;
        mb4_Work[0x5A] = timer;
        if (timer <= 0 && (mb4_Work[0x5C] & 1) == 0) {
            s32 burst = mb4_Work[0x5B] - 1;
            mb4_Work[0x5B] = burst;
            if (burst > 0) {
                bx = mb4ev_IrukaStartBubblePos.x;
                by = mb4ev_IrukaStartBubblePos.y;
                bz = mb4ev_IrukaStartBubblePos.z;
                bx -= 100.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f);
                by += 100.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f);
                bz -= 0.2f * (100.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
                duration = (s32) (120.0f * (1.0f + (0.5f * (0.007874016f * (f32) (frand() & 0x7F)))));
                puffPos.x = bx;
                puffPos.y = by;
                puffPos.z = bz;
                MB4_BubbleEffCreate(&puffPos, duration, 1.5f * (0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
            }
        }
    }

    if (MBModelDispGet((s16) mb4_Work[7]) && MBMotionEndCheck((s16) mb4_Work[7])) {
        HuAudFXEmiterPlay(0x5F0, &mb4ev_Puku2BubblePos);
        MBModelAttrSet((s16) mb4_Work[6], 0x40000002U);
        MBModelAttrSet((s16) mb4_Work[7], 0x40000002U);
        MBModelDispSet((s16) mb4_Work[7], FALSE);
    }
}

void MB4_Kill(void) {
    if (mb4_MainObj != NULL) {
        mb4_MainObj = NULL;
    }
    MB4_CoinClose();
    MB4_CoinEffClose();
    MB4_BubbleEffClose();
    MB4_SakanaClose();
}

void MB4_CoinClose(void) {
    mb4_CoinObj = NULL;
}

void MB4_CoinEffClose(void) {
    mb4_CoinEffObj = NULL;
}

void MB4_BubbleEffClose(void) {
    mb4_BubbleEffObj = NULL;
}

/* mb4_Work[0x1FDD]/[0x1FDE] (byte 0x7F74/0x7F78): sakana (fish) effect id
 * and model id, sentinel -1 = none. */
void MB4_SakanaClose(void) {
    if (MB4W_S32(0x7F74) != -1) {
        MBEffKill((s16) MB4W_S32(0x7F74));
    }
    MB4W_S32(0x7F74) = -1;
    if (MB4W_S32(0x7F78) != -1) {
        Hu3DModelKill((s16) MB4W_S32(0x7F78));
    }
    MB4W_S32(0x7F78) = -1;
}

/* NONMATCHING: register allocation differs (63% match). */
s32 MB4_CoinNumGet(void) {
    u8 *coinP;
    s32 i;
    s32 count;

    count = 0;
    if (mb4_CoinObj != NULL) {
        coinP = (u8 *) mb4_CoinObj->data;
        for (i = 0; i < 0x20; i++) {
            if (*(s32 *) (coinP + 8) != 0) {
                count++;
            }
            coinP += 0x88;
        }
    }
    return count;
}

/* mb4_CoinEffObj->data is {s32 hu3dModelIdx; s32 count;}; that model's
 * hookData points to a coin-sparkle particle manager whose +0x20 byte is
 * the blend mode (see MB4_CoinEffCreate's unk20 for the same struct).
 * NONMATCHING: register allocation differs (21%). */
void MB4_CoinEffBlendModeSet(u8 mode) {
    if (mb4_CoinEffObj != NULL) {
        s32 modelIdx = *(s32 *) mb4_CoinEffObj->data;
        u8 *mgr = (u8 *) Hu3DData[modelIdx].hookData;
        *(mgr + 0x20) = mode;
    }
}

s32 MB4_CoinEffNumGet(void) {
    OMOBJ *obj = mb4_CoinEffObj;
    if (mb4_CoinEffObj == NULL) {
        return 0;
    }
    return ((s32 *) obj->data)[1];
}

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match. Spawns 64 coin pairs (a coin model + a MBModelIdGet(mb4_Work[15])-
 * linked shadow model), seeding each with the current position of all 4
 * players (used later by MB4_CoinUpdate to fly a picked-up coin toward the
 * scoring player). mb4_CoinMaxDist is the (unused-by-this-function)
 * pickup-distance threshold read later by MB4_CoinUpdate/CoinNearGet. */
void MB4_CoinInit(void) {
    MB4COIN *coin;
    s32 i;
    s32 p;

    mb4_CoinObj = omAddObjEx(mbObjMan, -0x8000, 0U, 0U, -1, MB4_CoinUpdate);
    mb4_CoinObj->data = HuMemDirectMallocNum(HEAP_HEAP, 0x1100, 0x10000000U);
    memset(mb4_CoinObj->data, 0, 0x1100U);

    mb4_CoinMaxDist.x = 100.0f;
    mb4_CoinMaxDist.y = 170.0f;
    mb4_CoinMaxDist.z = 0.0f;

    coin = (MB4COIN *) mb4_CoinObj->data;
    for (i = 0; i < 0x20; i++) {
        if (i == 0) {
            coin->modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x80002, 0x10000000, HEAP_MODEL));
        } else {
            coin->modelId = Hu3DModelLink((s16) ((MB4COIN *) mb4_CoinObj->data)->modelId);
        }
        Hu3DModelCameraSet((s16) coin->modelId, 1U);
        Hu3DModelAttrSet((s16) coin->modelId, 1U);
        coin->shadowModelId = Hu3DModelLink(MBModelIdGet((s16) mb4_Work[0x15]));
        Hu3DModelAttrSet((s16) coin->shadowModelId, 1U);
        coin->active = 0;
        coin->unkC = 0;
        coin->speed = 0.0f;
        coin->state = 0;
        coin->layer = 0;
        for (p = 0; p < 4; p++) {
            MBPlayerPosGet(p, &coin->playerPos[p]);
        }
        coin++;
    }
}

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match. Finds a free MB4COIN slot (active == 0) and spawns it: coin model
 * at basePos (arg0, may be NULL to skip), shadow model 200 units below,
 * baseRot from arg1 (may be NULL). */
s32 MB4_CoinCreate(Point3d *basePos, Point3d *baseRot, s32 layer, f32 speed, f32 unused) {
    MB4COIN *coinP;
    s32 i;

    if (mb4_CoinObj == NULL) {
        return -1;
    }
    coinP = (MB4COIN *) mb4_CoinObj->data;
    for (i = 0; i < 0x20; i++) {
        if (coinP->active == 0) {
            break;
        }
        coinP++;
    }
    if (i >= 0x20) {
        return -1;
    }

    coinP->active = 1;
    coinP->unkC = 0;
    coinP->speed = speed;
    coinP->layer = layer;
    coinP->state = 0;
    if (basePos != NULL) {
        coinP->basePosX = basePos->x;
        coinP->basePosY = basePos->y;
        coinP->basePosZ = basePos->z;
    }
    if (baseRot != NULL) {
        coinP->baseRotX = (s32) baseRot->x;
        coinP->baseRotY = (s32) baseRot->y;
        coinP->baseRotZ = (s32) baseRot->z;
    }
    coinP->posX = coinP->basePosX;
    coinP->posY = coinP->basePosY;
    coinP->posZ = coinP->basePosZ;
    coinP->rotX = 0.0f;
    coinP->rotZ = 0.0f;
    coinP->scaleX = 1.0f;
    coinP->scaleY = 1.0f;
    coinP->scaleZ = 1.0f;
    coinP->rotY = 360.0f * (0.007874016f * (f32) (frand() & 0x7F));

    Hu3DModelPosSet((s16) coinP->modelId, coinP->posX, coinP->posY, coinP->posZ);
    Hu3DModelRotSet((s16) coinP->modelId, coinP->rotX, coinP->rotY, coinP->rotZ);
    Hu3DModelScaleSet((s16) coinP->modelId, coinP->scaleZ, coinP->scaleY, coinP->scaleX);
    Hu3DModelTPLvlSet((s16) coinP->modelId, 1.0f);
    Hu3DModelAttrReset((s16) coinP->modelId, 1U);
    Hu3DModelPosSet((s16) coinP->shadowModelId, coinP->posX, -200.0f, coinP->posZ);
    Hu3DModelRotSet((s16) coinP->shadowModelId, coinP->rotX, coinP->rotY, coinP->rotZ);
    Hu3DModelScaleSet((s16) coinP->shadowModelId, coinP->scaleZ, coinP->scaleY, coinP->scaleX);
    Hu3DModelTPLvlSet((s16) coinP->shadowModelId, 1.0f);
    Hu3DModelAttrReset((s16) coinP->shadowModelId, 1U);
    return i;
}

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match. Nearest active coin to `pos` within `maxDist`, if any. */
s32 MB4_CoinNearGet(Point3d *pos, Point3d *outPos, f32 maxDist) {
    MB4COIN *coinP;
    s32 i;
    s32 best = -1;
    f32 bestDist = 10000.0f;
    Point3d diff;
    f32 dist;

    if (mb4_CoinObj == NULL) {
        return -1;
    }
    coinP = (MB4COIN *) mb4_CoinObj->data;
    for (i = 0; i < 0x20; i++) {
        if (coinP->active != 0) {
            PSVECSubtract((Point3d *) &coinP->posX, pos, &diff);
            dist = PSVECMag(&diff);
            if (!(dist > maxDist) && dist < bestDist) {
                best = i;
                bestDist = dist;
            }
        }
        coinP++;
    }
    if (best >= 0) {
        MB4COIN *bestP = (MB4COIN *) mb4_CoinObj->data + best;
        outPos->x = bestP->posX;
        outPos->y = bestP->posY;
        outPos->z = bestP->posZ;
    }
    return best;
}

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match. Spawns a coin-sparkle MB4PARTICLE in the first free slot. */
s32 MB4_CoinEffCreate(Point3d *pos, Point3d *vel, u8 *color, f32 timer, f32 sizeParam, f32 alphaParam) {
    MB4COINEFFMGR *mgr;
    MB4PARTICLE *p;
    s32 i;

    if (mb4_CoinEffObj == NULL) {
        return -1;
    }
    mgr = (MB4COINEFFMGR *) Hu3DData[*(s32 *) mb4_CoinEffObj->data].hookData;
    p = mgr->particles;
    for (i = 0; i < mgr->count; i++) {
        if (p->timer == 0.0f) {
            break;
        }
        p++;
    }
    if (i >= mgr->count) {
        return -1;
    }

    p->active = 0;
    p->posX = pos->x;
    p->posY = pos->y;
    p->posZ = pos->z;
    p->velX = vel->x;
    p->velY = vel->y;
    p->velZ = vel->z;
    p->speed = timer;
    p->one = 1.0f;
    p->sizeParam = sizeParam;
    p->alphaParam = alphaParam;
    p->timer = timer;
    p->color[0] = color[0];
    p->color[1] = color[1];
    p->color[2] = color[2];
    p->color[3] = color[3];
    p->offsetZAngle = 360.0f * (0.007874016f * (f32) (frand() & 0x7F));
    p->active = 0;
    *(s32 *) ((u8 *) mb4_CoinEffObj->data + 4) += 1;
    return i;
}

/* mb4_Work[0x1FDD] (byte 0x7F74): sakana model id (see MB4_SakanaClose).
 * Hu3DData[id]+0x120 holds a pointer to the per-part array (stride 0x5C,
 * 92 bytes) written by MB4_SakanaInit - not yet named as a struct since
 * that function (2988 bytes) hasn't been decompiled yet. */
static u8 *MB4_SakanaPartBase(s32 partIdx) {
    u8 *mdlBase = (u8 *) &Hu3DData[MB4W_S32(0x7F74)];
    u8 *arrayBase = *(u8 **) (mdlBase + 0x120);
    return arrayBase + (partIdx * 0x5C);
}

/* NONMATCHING: register allocation for the part-base pointer differs
 * across all three of these (~40-70% match). */
void MB4_SakanaLenSet(s32 partIdx, f32 lenRatio) {
    *(f32 *) (MB4_SakanaPartBase(partIdx) + 0x2C) = lenRatio * 100.0f;
}

void MB4_SakanaOffsetSet(s32 partIdx, f32 offX, f32 offY, f32 offZ) {
    u8 *part = MB4_SakanaPartBase(partIdx);
    *(f32 *) (part + 0x30) = offX;
    *(f32 *) (part + 0x34) = offY - 90.0f;
    *(f32 *) (part + 0x38) = offZ;
}

void MB4_SakanaPosSet(s32 partIdx, f32 x, f32 y, f32 z) {
    u8 *part = MB4_SakanaPartBase(partIdx);
    *(f32 *) (part + 0x3C) = x;
    *(f32 *) (part + 0x40) = y;
    *(f32 *) (part + 0x44) = z;
}

/* NONMATCHING: MWCC keeps the literal args to the nested Hu3DGLightCreate()
 * call in nonvolatile FPRs (f26-f31) here instead of the original's stack
 * temps; the exact original source form of this call wasn't recovered. */
void MB4_LightSet(void) {
    Hu3DShadowCreate(45.0f, 1000.0f, 250000.0f);
    Hu3DShadowTPLvlSet(0.3f);
    Hu3DBGColorSet(0xFF, 0xFF, 0xFF);
    Hu3DGLightInfinitytSet(Hu3DGLightCreate(-3700.0f, 12000.0f, 22000.0f, 3700.0f, -12000.0f, -22000.0f, 0xFF, 0xFF, 0xFF));
}

void MB4_LightReset(void) {
}

f32 MB4_AngleDiff(f32 a, f32 b) {
    f32 diff;

    if (a >= 360.0f) {
        a -= 360.0f;
    } else if (a < 0.0f) {
        a += 360.0f;
    }
    if (b >= 360.0f) {
        b -= 360.0f;
    } else if (b < 0.0f) {
        b += 360.0f;
    }
    diff = a - b;
    if (diff <= -180.0f) {
        diff += 360.0f;
    } else if (diff >= 180.0f) {
        diff -= 360.0f;
    }
    return diff;
}

/* masu->flag bits used by this board's event dispatch; not part of the
 * shared MASU_FLAG_* set in game/board/masu.h (those start at bit 16). */
#define MB4_MASU_FLAG_HATENA 0x1
#define MB4_MASU_FLAG_IRUKA 0x10
#define MB4_MASU_FLAG_BLOW 0x40
#define MB4_MASU_FLAG_KAISOO 0x100

void MB4Ev_MasuHatena(s32 playerNo) {
    s16 masuId = GwPlayer[playerNo].masuId;
    MASU *masuP = MBMasuGet(MASU_LAYER_DEFAULT, masuId);
    if (masuP->flag & MB4_MASU_FLAG_HATENA) {
        MB4Ev_Kihon(playerNo);
    } else if (masuP->flag & MB4_MASU_FLAG_IRUKA) {
        MB4Ev_Iruka(playerNo);
    } else if (masuP->flag & MB4_MASU_FLAG_BLOW) {
        MB4Ev_Blow(playerNo);
    } else if (masuP->flag & MB4_MASU_FLAG_KAISOO) {
        MB4Ev_Kaisoo(playerNo);
    }
}

s32 MB4_MasuStub(void) {
    return 0;
}

s32 MB4Ev_MasuWalkPre(s32 playerNo) {
    return 0;
}

s32 MB4Ev_MasuWalkPost(s32 playerNo) {
    s16 masuId = GwPlayer[playerNo].masuId;
    MASU *masuP = MBMasuGet(MASU_LAYER_DEFAULT, masuId);
    if (masuP->flag & 0x200) {
        s32 *stateP;
        MBModelAttrReset((s16) MB4W_S32(0x18), 0x40000002U);
        MBMotionTimeSet((s16) MB4W_S32(0x18), 0.0f);
        MBMotionSpeedSet((s16) MB4W_S32(0x18), 1.0f);
        MBModelAttrReset((s16) MB4W_S32(0x1C), 0x40000002U);
        MBMotionTimeSet((s16) MB4W_S32(0x1C), 0.0f);
        MBMotionSpeedSet((s16) MB4W_S32(0x1C), 1.0f);
        MBModelDispSet((s16) MB4W_S32(0x1C), TRUE);
        HuAudFXEmiterPlay(0x5EF, &mb4ev_Puku2EmiterPos);
        stateP = &MB4W_S32(0x15C);
        stateP[0] = 3;
        stateP[1] = 0x12;
        stateP[2] = -1;
    }
    return 0;
}

/* NONMATCHING: register allocation keeps 3 more nonvolatile FPRs live than
 * the original (f23-f25 vs none) for the diff/absDiff temps; logic matches. */
f32 MB4_AngleLerp(f32 a, f32 b, f32 step) {
    f32 diff;
    f32 absDiff;
    f32 result;

    if (a >= 360.0) {
        a = (f32) ((f64) a - 360.0);
    } else if (a < 0.0) {
        a = (f32) ((f64) a + 360.0);
    }
    if (b >= 360.0) {
        b = (f32) ((f64) b - 360.0);
    } else if (b < 0.0) {
        b = (f32) ((f64) b + 360.0);
    }
    diff = (f32) (360.0 + (f64) (a - b));
    absDiff = (f32) fabs(diff);
    if (absDiff >= 360.0) {
        diff = (f32) fmod((f64) diff, 360.0);
    }
    if (diff < 180.0f) {
        result = (diff <= step) ? diff : step;
    } else {
        result = ((f32) (360.0 - (f64) diff) <= step) ? (f32) -(360.0 - (f64) diff) : -step;
    }
    result = result + b;
    if (result >= 360.0) {
        result = (f32) ((f64) result - 360.0);
    } else if (result < 0.0) {
        result = (f32) ((f64) result + 360.0);
    }
    return result;
}

void MB4_MapViewHook(int enterF) {
    if (enterF) {
        MBModelDispSet((s16) MB4W_S32(0x58), FALSE);
    } else {
        MBModelDispSet((s16) MB4W_S32(0x58), TRUE);
    }
}

int MB4_MasuPathCheck(s16 id, u32 flag, s16 *linkTbl, int endF) {
    return -1;
}

/* NONMATCHING: register allocation differs from the original's manual
 * unroll of the first recursion levels (60% match); logic is complete and
 * faithful (depth-limited DFS toward the nearest star masu, used to drive
 * the "raise chance" star-distance display). */
static s16 PathCheck(s16 masuId, s16 depth) {
    MASU *masuP = MBMasuGet(MASU_LAYER_DEFAULT, masuId);
    s32 i;
    BOOL found;

    if (MBMasuTypeGet(MASU_LAYER_DEFAULT, masuId) == MASU_TYPE_STAR) {
        s16 dist = mb4_PathMasuNo - depth;
        if (dist < mb4_PathLen) {
            mb4_PathLen = dist;
        }
        mb4_PathValidF = 1;
        return 1;
    }
    if (MBMasuTypeGet(MASU_LAYER_DEFAULT, masuId) != MASU_TYPE_NONE) {
        depth -= 1;
    }
    found = FALSE;
    if (depth >= 0) {
        for (i = 0; i < masuP->linkNum; i++) {
            if ((MBMasuFlagGet(MASU_LAYER_DEFAULT, masuP->linkTbl[i]) & (MBBranchFlagGet() | 0x06000000)) == 0) {
                if (PathCheck((s16) masuP->linkTbl[i], depth) != 0) {
                    found = TRUE;
                }
            }
        }
    }
    return found ? 1 : 0;
}

/* Quadratic Bezier, applied per axis (x,y,z) over 3 flat f32 arrays. */
/* NONMATCHING: fewer nonvolatile FPRs live than the original's extra
 * register-to-register copies before the store (27% match); logic is
 * complete and faithful to the scalar MB4_BezierCalc(). */
void MB4_BezierCalcV(f32 *p0, f32 *p1, f32 *p2, f32 *out, f32 t) {
    s32 i;
    f32 p2v, p1v, p0v, omt, result;

    for (i = 0; i < 3; i++) {
        p2v = *p2;
        p2++;
        p1v = *p1;
        p1++;
        p0v = *p0;
        p0++;
        omt = (f32) (1.0 - (f64) t);
        result = (f32) ((f64) (p2v * (t * t)) + ((f64) (p0v * (omt * omt)) + (2.0 * (f64) (p1v * (omt * t)))));
        *out = result;
        out++;
    }
}

/* Quadratic Bezier: B(t) = p0*(1-t)^2 + 2*p1*(1-t)*t + p2*t^2 */
f32 MB4_BezierCalc(f32 t, f32 p0, f32 p1, f32 p2) {
    f32 omt = (f32) (1.0 - (f64) t);
    f32 result = (f32) ((f64) (p2 * (t * t)) + ((f64) (p0 * (omt * omt)) + (2.0 * (f64) (p1 * (omt * t)))));
    return result;
}

/* Hermite spline derivative (slope) at parameter t, blended over the two
 * endpoint positions (p0,p1) and two tangents (m0,m1): p0=farg1, p1=farg2,
 * m0=farg3, m1=farg4 in the original's calling convention. */
f32 MB4_HermiteSlopeCalc(f32 t, f32 p0, f32 p1, f32 m0, f32 m1) {
    f32 t2 = t * t;
    f32 a = (6.0f * t2) - (6.0f * t);
    f32 b = (-6.0f * t2) - (6.0f * t);
    f32 c = 1.0f + ((3.0f * t2) - (4.0f * t));
    f32 d = (3.0f * t2) - (2.0f * t);
    return ((a * p0) + (b * p1)) + (c * m0) + (d * m1);
}

/* NONMATCHING: written as a shared per-axis helper (the original inlines
 * this 3 times, once per x/y/z); logic and arithmetic ordering match the
 * decompiled body exactly, just factored out. Catmull-Rom-style tangents
 * (m0 from p0-pPrev, m1 from pNext-p1, both averaged with p1-p0) blended
 * with the standard cubic Hermite basis at t. */
static f32 MB4_HermiteCalcAxis(f32 pPrev, f32 p0, f32 p1, f32 pNext, f32 t) {
    f32 delta = p1 - p0;
    f32 m0, m1;
    f32 t2, t3, h1, h0, w0, w1;

    if (p0 == p1) {
        m0 = delta;
    } else {
        m0 = 0.5f * (delta + (p0 - pPrev));
    }
    if (p0 == pNext) {
        m1 = delta;
    } else {
        m1 = 0.5f * (delta + (pNext - p1));
    }
    t2 = t * t;
    t3 = t * t2;
    h1 = (f32) (((3.0 * (f64) t2) - (f64) t3) - (f64) t3);
    h0 = (f32) (1.0 - (f64) h1);
    w0 = t + ((t3 - t2) - t2);
    w1 = t3 - t2;
    return (w1 * m1) + (w0 * m0) + (h0 * p0) + (h1 * p1);
}

void MB4_HermiteCalcV(Point3d *pPrev, Point3d *p0, Point3d *p1, Point3d *pNext, Point3d *out, f32 t) {
    out->x = MB4_HermiteCalcAxis(pPrev->x, p0->x, p1->x, pNext->x, t);
    out->y = MB4_HermiteCalcAxis(pPrev->y, p0->y, p1->y, pNext->y, t);
    out->z = MB4_HermiteCalcAxis(pPrev->z, p0->z, p1->z, pNext->z, t);
}

/* Cubic Hermite basis weights for parameter t, one output per blend term. */
void MB4_HermiteCalc(f32 *w0, f32 *w1, f32 *w2, f32 *w3, f32 t) {
    f32 t2 = t * t;
    f32 t3 = t * t2;
    f32 h1 = (f32) (((3.0 * (f64) t2) - (f64) t3) - (f64) t3);
    *w0 = (f32) (1.0 - (f64) h1);
    *w1 = h1;
    *w2 = t + ((t3 - t2) - t2);
    *w3 = t3 - t2;
}

/* NONMATCHING: argument-reload ordering around the trailing 1.0f literal
 * differs (37% match); this is a straight forwarding call. */
/* NONMATCHING: written as a real loop calling the fn_1_DC9C helper 16
 * times instead of the original's full 16-way manual unroll; the algorithm
 * is exactly Simpson's rule (confirmed from the decompiled body: h =
 * tMax/16, endpoints weight 1, odd samples weight 4, even samples weight
 * 2, each sample being the curve-speed magnitude fn_1_DC9C(pPrev,p1,pNext,t)
 * - same formula reused for every sample in the original too) integrating
 * curve speed over [0, tMax] to get an arc-length-parameterized time; the
 * unused 4th Point3d argument matches the original's dead r6 parameter. */
static f32 MB4_PathTimeGet(Point3d *pPrev, Point3d *p1, Point3d *pNext, Point3d *unused, f32 tMax) {
    f32 h = tMax / 16.0f;
    f32 sum = fn_1_DC9C(pPrev, p1, pNext, 0.0f) + fn_1_DC9C(pPrev, p1, pNext, tMax);
    s32 i;

    for (i = 1; i < 16; i += 2) {
        sum += 4.0f * fn_1_DC9C(pPrev, p1, pNext, (f32) i * h);
    }
    for (i = 2; i < 16; i += 2) {
        sum += 2.0f * fn_1_DC9C(pPrev, p1, pNext, (f32) i * h);
    }
    return (h / 3.0f) * sum;
}

f32 MB4_PathNextTimeGet(Point3d *p0, Point3d *p1, Point3d *p2, Point3d *p3) {
    return MB4_PathTimeGet(p0, p1, p2, p3, 1.0f);
}

/* Derivative of the Catmull-Rom/Hermite-tangent curve segment [p1,pNext] at
 * parameter t, tangent at p1 computed from the symmetric neighbor formula
 * m = 0.5*(pNext-pPrev) (no separate tangent at pNext - only used where the
 * far end is a fixed direction, see callers). */
static f32 fn_1_DB68(f32 t, f32 pPrev, f32 p1, f32 pNext) {
    f32 m0 = 0.5f * ((pNext + (p1 - pPrev)) - p1);
    f32 m1 = 0.5f * ((pNext + (p1 - pPrev)) - p1);
    return m0 + ((t * (3.0f * (m1 + (m0 + ((2.0f * p1) - (2.0f * pNext)))) * t)) + (2.0f * ((((-3.0f * p1) + (3.0f * pNext)) - (2.0f * m0)) - m1) * t));
}

/* PowerPC fast reciprocal-sqrt estimate refined by 3 Newton-Raphson
 * iterations, matching the __frsqrte-based pattern used elsewhere in the
 * decomp (see w07Dll's MB7_FastSqrt for the exact reference). */
static f32 MB4_FastSqrt(f32 x) {
#ifdef TARGET_PC
    f64 guess = 1.0 / sqrt((f64) x);
#else
    f64 guess = __frsqrte((f64) x);
#endif
    guess = 0.5 * guess * (3.0 - ((f64) x * guess * guess));
    guess = 0.5 * guess * (3.0 - ((f64) x * guess * guess));
    guess = 0.5 * guess * (3.0 - ((f64) x * guess * guess));
    return (f32) ((f64) x * guess);
}

/* NONMATCHING: written using the fn_1_DB68 helper 3 times (once per axis)
 * plus MB4_FastSqrt, matching the decompiled body's arithmetic exactly;
 * the original inlines all of this instead. Speed (magnitude of the curve
 * derivative) of the [p1,pNext] segment at parameter t, used to normalize
 * animation playback rate against travel speed along the path. */
static f32 fn_1_DC9C(Point3d *pPrev, Point3d *p1, Point3d *pNext, f32 t) {
    f32 dx = fn_1_DB68(t, pPrev->x, p1->x, pNext->x);
    f32 dy = fn_1_DB68(t, pPrev->y, p1->y, pNext->y);
    f32 dz = fn_1_DB68(t, pPrev->z, p1->z, pNext->z);
    f32 magSq = (dz * dz) + ((dx * dx) + (dy * dy));
    if (magSq > 0.0f) {
        return MB4_FastSqrt(magSq);
    }
    return magSq;
}

s16 MB4_PathLenGet(s16 masuId, s16 depthLimit) {
    mb4_PathMasuNo = depthLimit;
    mb4_PathLen = depthLimit;
    mb4_PathValidF = 0;
    PathCheck(masuId, depthLimit);
    if (mb4_PathValidF) {
        return mb4_PathLen;
    }
    return -1;
}

void MB4_ModelStub(HU3DMODEL *a, HU3DMODEL *b, HU3DMODEL *c, HU3DMODEL *d) {
}

/* NONMATCHING: register allocation for the Hu3DData index differs (48%). */
void MB4_ModelMtxSet(s16 modelId, Mtx mtx) {
    s16 hu3dModelId = (s16) MBModelIdGet(modelId);
    PSMTXCopy(mtx, Hu3DData[hu3dModelId].mtx);
}

/* NONMATCHING: register allocation for the Hu3DData index differs (46%). */
void MB4_PlayerMtxSet(s16 playerNo, Mtx mtx) {
    s16 hu3dModelId = (s16) MBModelIdGet(MBPlayerModelGet(playerNo));
    PSMTXCopy(mtx, Hu3DData[hu3dModelId].mtx);
}
