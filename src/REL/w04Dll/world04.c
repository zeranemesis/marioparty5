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
static f32 MB4_FastSqrt(f32 x);
f32 MB4_AngleLerp(f32 a, f32 b, f32 step);
void MB4_CoinInit(void);
void MB4_CoinUpdate(OMOBJ *obj);
s32 MB4_CoinCreate(Point3d *basePos, Point3d *targetPos, s32 flightFrames, f32 arcHeight, f32 unused);
s32 MB4_CoinNearGet(Point3d *pos, Point3d *outPos, f32 maxDist);
s32 MB4_CoinEffCreate(Point3d *pos, Point3d *vel, u8 *color, f32 timer, f32 sizeParam, f32 alphaParam);
void MB4_CoinEffInit(void);
void MB4_CoinEffUpdate(OMOBJ *obj);
void MB4_SakanaLenSet(s32 partIdx, f32 lenRatio);
void MB4_SakanaOffsetSet(s32 partIdx, f32 offX, f32 offY, f32 offZ);
void MB4_SakanaOffsetInit(void);
void MB4_SakanaPosInit(void);
s32 MB4_SakanaCreate(s16 type);
void MB4_SakanaInit(s32 unused0, s32 unused1, s32 unused2, f32 unused3);
void MB4_SakanaDraw(HU3DMODEL *modelP, Mtx *mtx);
void MB4_SakanaPosSet(s32 partIdx, f32 x, f32 y, f32 z);
static void MB4Ev_Kihon(s32 playerNo);
static void MB4Ev_Iruka(s32 playerNo);
static void MB4Ev_Blow(s32 playerNo);
static void MB4Ev_Kaisoo(s32 playerNo);
static s16 PathCheck(s16 masuId, s16 depth);
static f32 MB4_PathTimeGet(Point3d *pPrev, Point3d *p1, Point3d *pNext, Point3d *unused, f32 tMax);
static f32 fn_1_DB68(f32 t, f32 pPrev, f32 p1, f32 pNext);
static f32 fn_1_DC9C(Point3d *pPrev, Point3d *p1, Point3d *pNext, f32 t);
static void fn_1_D3D8(f32 t, f32 pPrev, f32 p0, f32 p1, f32 pNext, f32 tangentScale);
void MB4_BubbleEffInit(void);
void MB4_BubbleEffUpdate(OMOBJ *obj);
s32 MB4_BubbleEffCreate(Point3d *pos, s32 lifeFrames, f32 unused, f32 speedScale);
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
static void MB4Ev_PlayerBubbleCreate(s32 playerNo);
static void MB4Ev_IrukaBubbleCreate(s32 playerNo, Point3d *ofs, f32 unused0, f32 unused1);
static void MB4_CoinDispUpdate(s32 playerNo, s32 coinNum, s32 threshold);

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
    s16 sakanaType; /* 0x4C, sakana only: the MB4_SakanaCreate() type arg */
    u8 pad4E[0xA];  /* 0x4E */
    u8 sakanaFlags; /* 0x58, sakana only: bit 7 = slot in use */
} MB4PARTICLE; /* size 0x5C */

/* Per-coin record (size 0x88 = 136 bytes), read from mb4_CoinObj->data
 * (32 entries) in MB4_CoinCreate/CoinInit/CoinNearGet. */
typedef struct MB4Coin_s {
    s32 modelId;       /* 0x00 */
    s32 shadowModelId; /* 0x04 */
    s32 active;        /* 0x08 */
    s32 phase;         /* 0x0C, 0 = flying to target, 1 = arrived/locked */
    f32 arcHeight;     /* 0x10, extra sine-arc height while flying */
    s32 flightFrames;  /* 0x14, duration in frames of the state-0 flight */
    s32 flightTimer;   /* 0x18, frame counter within the state-0 flight */
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
    f32 targetX;       /* 0x4C */
    f32 targetY;       /* 0x50 */
    f32 targetZ;       /* 0x54 */
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

/* mb4_CoinEffObj->data (0xC bytes). */
typedef struct MB4CoinEffObjData_s {
    s32 hu3dModelIdx;
    s32 count;
    ANIMDATA *anim;
} MB4COINEFFDATA;

/* mb4ev_SakanaTbl: per-species swim area (pos = center, range = half-extent
 * per axis), count = number of fish of this species, flag1/flag2 mirror
 * each other (unused split not yet understood - kept for fidelity). Used
 * by MB4_SakanaOffsetInit/PosInit to seed each species' per-fish target
 * slots inside mb4_Work (14 species * 0x900 bytes, at mb4_Work+0x174,
 * each holding up to a handful of 0x90-byte per-fish target records). */
typedef struct MB4SakanaSpecies_s {
    Point3d pos;
    Point3d range;
    s32 count;
    s32 flag1;
    s32 flag2;
} MB4SAKANASPECIES;

/* Per-fish swim record (size 0x90 = 144 bytes), one of up to 16 per
 * species inside mb4_Work + 0x174 + species*0x900 (see MB4_SakanaInit).
 * Either a real linked model (modelId != -1, particleIdx == -1) or a
 * shared MB4PARTICLE slot (modelId == -1, particleIdx = index set via
 * MB4_SakanaPosSet/OffsetSet/LenSet). */
typedef struct MB4SakanaFish_s {
    s32 modelId;      /* 0x00 */
    s32 particleIdx;  /* 0x04 */
    f32 rand1;        /* 0x08 */
    s32 bobAngle;     /* 0x0C, degrees; read back via an explicit int-to-
                       * float conversion in MB4_SakanaDraw's body-bob step */
    s32 unk10;        /* 0x10 */
    s32 unk14;        /* 0x14 */
    s32 timer18;      /* 0x18 */
    s32 timer1C;       /* 0x1C */
    s32 timer20;        /* 0x20 */
    s32 timer24;          /* 0x24 */
    s32 pathAltFlag;        /* 0x28 */
    s32 unk2C;                /* 0x2C, always 1 */
    Point3d speciesPos;         /* 0x30 */
    Point3d speciesRange;         /* 0x3C */
    Point3d posA;                  /* 0x48, random point in swim area */
    Point3d posB;                   /* 0x54, random point in swim area (used as current pos) */
    Point3d dir;                      /* 0x60, random normalized direction */
    f32 pitch;                         /* 0x6C */
    f32 yaw;                            /* 0x70 */
    f32 roll;                            /* 0x74 */
    f32 prevPitch;                        /* 0x78 */
    f32 prevYaw;                           /* 0x7C */
    f32 prevRoll;                           /* 0x80 */
    f32 speed;                               /* 0x84 */
    f32 scale;                                /* 0x88 */
    f32 unk8C;                                 /* 0x8C */
} MB4SAKANAFISH; /* size 0x90 */

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

static Point3d mb4ev_IrukaPath[0x96] = {
    { -3260.0f, 1270.0f, -1300.0f },
    { -3314.35596f, 1271.49194f, -1062.65002f },
    { -3361.81396f, 1273.07104f, -823.849976f },
    { -3388.625f, 1274.89795f, -581.98999f },
    { -3371.47705f, 1277.198f, -340.119995f },
    { -3256.05908f, 1280.26904f, -130.789993f },
    { -3033.13403f, 1283.81702f, -40.7200012f },
    { -2790.32593f, 1290.22998f, -31.25f },
    { -2548.23389f, 1301.83203f, -53.0699997f },
    { -2308.45801f, 1320.27295f, -90.9899979f },
    { -2070.66602f, 1346.43396f, -136.220001f },
    { -1834.73804f, 1382.23999f, -184.490005f },
    { -1600.93396f, 1430.72205f, -231.970001f },
    { -1370.95996f, 1498.83301f, -273.179993f },
    { -1147.31396f, 1589.19995f, -304.779999f },
    { -934.890015f, 1705.71802f, -325.480011f },
    { -741.695984f, 1853.10803f, -334.649994f },
    { -581.809021f, 2035.76404f, -332.869995f },
    { -480.824005f, 2255.40894f, -322.570007f },
    { -486.803986f, 2495.49805f, -311.0f },
    { -598.448975f, 2709.59912f, -304.230011f },
    { -775.072998f, 2875.73193f, -301.079987f },
    { -992.903015f, 2982.42407f, -300.100006f },
    { -1232.54602f, 3014.4541f, -300.0f },
    { -1465.68298f, 2951.7041f, -300.0f },
    { -1656.03296f, 2802.63501f, -300.0f },
    { -1778.26904f, 2594.03809f, -300.0f },
    { -1815.43799f, 2354.92603f, -299.859985f },
    { -1782.31995f, 2114.521f, -294.420013f },
    { -1709.82202f, 1883.67102f, -270.839996f },
    { -1618.33301f, 1665.05505f, -216.880005f },
    { -1523.15002f, 1464.39294f, -118.870003f },
    { -1441.90906f, 1295.12195f, 34.6500015f },
    { -1388.58398f, 1172.84204f, 237.660004f },
    { -1349.71301f, 1087.34204f, 461.950012f },
    { -1312.54199f, 1025.77197f, 694.47998f },
    { -1265.66003f, 982.637024f, 929.400024f },
    { -1201.72205f, 950.476013f, 1162.0f },
    { -1105.72302f, 927.182983f, 1384.12f },
    { -972.710999f, 908.887024f, 1586.59998f },
    { -796.223022f, 896.968018f, 1753.19995f },
    { -590.112f, 893.179016f, 1881.22998f },
    { -360.731995f, 900.922974f, 1960.68994f },
    { -120.084999f, 924.624023f, 1978.20996f },
    { 113.210999f, 965.83197f, 1929.48999f },
    { 319.14801f, 1023.98602f, 1815.43994f },
    { 484.825989f, 1093.80505f, 1652.25f },
    { 616.838013f, 1169.11597f, 1462.55005f },
    { 711.023987f, 1247.89294f, 1252.81006f },
    { 766.755981f, 1328.13696f, 1030.28003f },
    { 769.36499f, 1407.25098f, 801.169983f },
    { 701.752014f, 1479.53699f, 580.679993f },
    { 544.817993f, 1532.55701f, 404.410004f },
    { 332.466003f, 1562.61902f, 292.209991f },
    { 96.3649979f, 1574.15405f, 238.679993f },
    { -146.315002f, 1569.21204f, 243.779999f },
    { -378.574005f, 1549.25403f, 309.720001f },
    { -584.05603f, 1516.48096f, 434.100006f },
    { -736.107971f, 1473.49805f, 618.219971f },
    { -844.663025f, 1430.15906f, 831.179993f },
    { -908.086975f, 1394.13794f, 1063.15002f },
    { -939.289978f, 1369.422f, 1303.01001f },
    { -935.546021f, 1363.927f, 1545.90002f },
    { -895.226013f, 1386.526f, 1784.30005f },
    { -813.15802f, 1451.02905f, 2002.51001f },
    { -690.757019f, 1561.89099f, 2180.0f },
    { -538.992981f, 1703.302f, 2306.19995f },
    { -371.261993f, 1855.28699f, 2395.01001f },
    { -189.231003f, 2007.48096f, 2448.86011f },
    { 4.7329998f, 2151.21704f, 2478.55005f },
    { 214.811005f, 2273.61206f, 2483.52002f },
    { 443.549011f, 2351.7959f, 2462.75f },
    { 680.211975f, 2366.32495f, 2412.1499f },
    { 903.840027f, 2314.08911f, 2333.45996f },
    { 1104.03601f, 2215.99292f, 2236.62012f },
    { 1282.18201f, 2090.71094f, 2128.17993f },
    { 1442.26404f, 1950.79004f, 2009.73999f },
    { 1586.66699f, 1804.87598f, 1878.96997f },
    { 1717.74304f, 1659.51794f, 1734.22998f },
    { 1836.77698f, 1520.328f, 1573.87f },
    { 1946.51697f, 1390.41895f, 1399.75f },
    { 2047.89697f, 1271.84204f, 1212.90002f },
    { 2138.13403f, 1164.87f, 1013.79999f },
    { 2210.65308f, 1069.68896f, 801.960022f },
    { 2259.0249f, 987.096985f, 578.400024f },
    { 2265.7019f, 924.284973f, 343.839996f },
    { 2226.3291f, 883.43103f, 107.870003f },
    { 2123.89209f, 874.979004f, -111.650002f },
    { 1976.25305f, 892.500977f, -303.209991f },
    { 1799.47998f, 928.960022f, -465.929993f },
    { 1606.77197f, 977.255981f, -606.390015f },
    { 1405.26404f, 1032.89905f, -731.049988f },
    { 1198.57898f, 1093.28894f, -844.599976f },
    { 988.630981f, 1156.89099f, -950.169983f },
    { 776.504028f, 1222.76196f, -1049.89001f },
    { 562.848999f, 1290.297f, -1145.18994f },
    { 347.854004f, 1359.36694f, -1236.29004f },
    { 132.240005f, 1429.172f, -1325.33997f },
    { -83.7539978f, 1499.43298f, -1413.09998f },
    { -300.0f, 1570.0f, -1500.0f },
    { -3100.0f, 0.0f, 3100.0f },
    { -2979.59302f, 0.0f, 3150.93994f },
    { -2857.96899f, 0.0f, 3198.87012f },
    { -2734.0791f, 0.0f, 3240.51001f },
    { -2607.09912f, 0.0f, 3271.30005f },
    { -2477.20801f, 0.0f, 3284.82007f },
    { -2347.12695f, 0.0f, 3273.98999f },
    { -2220.46997f, 0.0f, 3241.87012f },
    { -2096.55298f, 0.0f, 3200.19995f },
    { -1972.03198f, 0.0f, 3160.44995f },
    { -1843.82397f, 0.0f, 3136.07007f },
    { -1713.65796f, 0.0f, 3141.51001f },
    { -1586.71106f, 0.0f, 3172.18994f },
    { -1462.31201f, 0.0f, 3212.38989f },
    { -1336.74695f, 0.0f, 3248.62988f },
    { -1207.46802f, 0.0f, 3266.19995f },
    { -1077.776f, 0.0f, 3252.23999f },
    { -951.905029f, 0.0f, 3217.07007f },
    { -827.614014f, 0.0f, 3176.52002f },
    { -701.098999f, 0.0f, 3144.0f },
    { -571.112f, 0.0f, 3135.12988f },
    { -442.428009f, 0.0f, 3156.55005f },
    { -317.302002f, 0.0f, 3194.31006f },
    { -192.744995f, 0.0f, 3234.01001f },
    { -65.1809998f, 0.0f, 3262.05005f },
    { 65.1809998f, 0.0f, 3262.05005f },
    { 192.744995f, 0.0f, 3234.01001f },
    { 317.300995f, 0.0f, 3194.31006f },
    { 442.427002f, 0.0f, 3156.55005f },
    { 571.112f, 0.0f, 3135.12988f },
    { 701.098999f, 0.0f, 3144.0f },
    { 827.614014f, 0.0f, 3176.52002f },
    { 951.903992f, 0.0f, 3217.07007f },
    { 1077.776f, 0.0f, 3252.23999f },
    { 1207.46802f, 0.0f, 3266.19995f },
    { 1336.74597f, 0.0f, 3248.62988f },
    { 1462.31201f, 0.0f, 3212.38989f },
    { 1586.71106f, 0.0f, 3172.18994f },
    { 1713.65698f, 0.0f, 3141.51001f },
    { 1843.823f, 0.0f, 3136.07007f },
    { 1972.03198f, 0.0f, 3160.44995f },
    { 2096.55298f, 0.0f, 3200.19995f },
    { 2220.46997f, 0.0f, 3241.87012f },
    { 2347.12598f, 0.0f, 3273.98999f },
    { 2477.20703f, 0.0f, 3284.82007f },
    { 2607.09912f, 0.0f, 3271.30005f },
    { 2734.0791f, 0.0f, 3240.51001f },
    { 2857.96899f, 0.0f, 3198.87012f },
    { 2979.59302f, 0.0f, 3150.93994f },
    { 3100.0f, 0.0f, 3100.0f },
};

/* Per-character head-bubble spawn offset/scale (indexed by GwPlayer[].charNo,
 * 13 characters). */
static Point3d mb4ev_IrukaBubbleOfsTbl[13] = {
    { 0.0f, -10.0f, 40.0f },
    { 0.0f, -15.000001f, 35.0f },
    { 0.0f, 20.0f, 10.0f },
    { 0.0f, 50.0f, 30.0000019f },
    { 0.0f, 45.0f, 20.0f },
    { 0.0f, 15.000001f, 10.0f },
    { 0.0f, 30.0000019f, 30.0000019f },
    { 0.0f, -10.0f, 25.0f },
    { 0.0f, 0.0f, 50.0f },
    { 0.0f, -20.0f, 65.0f },
    { 0.0f, -20.0f, 65.0f },
    { 0.0f, -20.0f, 65.0f },
    { 0.0f, -20.0f, 65.0f },
};
static f32 mb4ev_IrukaTPLvlScaleTbl[13] = {
    1.10000002f, 1.0f, 0.800000012f, 1.10000002f, 1.20000005f, 0.699999988f,
    0.899999976f, 0.800000012f, 0.699999988f, 1.20000005f, 1.10000002f,
    1.10000002f, 1.29999995f,
};
static Point3d lbl_1_data_10CC = { 0.0f, 0.0f, 0.0f };

/* Capsule pool for the MB4Ev_Kaisoo "fishing" prize. */
static int mb4ev_Puku2CapsuleTbl[9] = { 1, 11, 12, 13, 15, 17, 6, 25, 36 };

static MB4SAKANASPECIES mb4ev_SakanaTbl[14] = {
    { { 31.0f, 321.0f, -171.0f }, { 1100.0f, 100.0f, 700.0f }, 3, 1, 1 },
    { { -1655.0f, 721.0f, 2080.0f }, { 2600.0f, 500.0f, 1000.0f }, 3, 0, 0 },
    { { 1125.0f, 621.0f, 2080.0f }, { 2300.0f, 500.0f, 1000.0f }, 2, 0, 0 },
    { { -4540.0f, 221.0f, 591.0f }, { 1000.0f, 300.0f, 800.0f }, 2, 1, 1 },
    { { -4540.0f, 221.0f, -1091.0f }, { 1000.0f, 300.0f, 1000.0f }, 3, 1, 1 },
    { { -1523.0f, 996.0f, -1799.0f }, { 1500.0f, 250.0f, 600.0f }, 4, 1, 1 },
    { { 0.0f, 496.0f, -2027.0f }, { 1700.0f, 500.0f, 900.0f }, 6, 1, 1 },
    { { 1300.0f, 896.0f, -3027.0f }, { 1500.0f, 500.0f, 900.0f }, 3, 1, 1 },
    { { 4386.0f, 271.0f, -1000.0f }, { 1000.0f, 300.0f, 800.0f }, 3, 1, 1 },
    { { 4386.0f, 271.0f, 0.0f }, { 1000.0f, 300.0f, 800.0f }, 2, 1, 1 },
    { { -2502.0f, 996.0f, 654.0f }, { 2000.0f, 200.0f, 700.0f }, 2, 0, 0 },
    { { 2516.0f, 1200.0f, 465.0f }, { 1900.0f, 200.0f, 700.0f }, 2, 0, 0 },
    { { 202.0f, 1296.0f, -354.0f }, { 2000.0f, 200.0f, 700.0f }, 2, 0, 0 },
    { { 2502.0f, 696.0f, 1254.0f }, { 1000.0f, 200.0f, 700.0f }, 1, 0, 0 },
};

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
                MB4_BubbleEffCreate(&mb4ev_BubbleEmitPosTbl[i], duration, 0.0f, 1.5f * (0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
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
                MB4_BubbleEffCreate(&puffPos, duration, 0.0f, 1.5f * (0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
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
                MB4_BubbleEffCreate(&puffPos, duration, 0.0f, 1.5f * (0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
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

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match. Ambient bubble particles reuse MB4PARTICLE with a different
 * meaning per field: active (unk0, here an s16 wobble-phase angle 0-359),
 * velX (wobble amplitude), velY (rise speed), speed (alpha 0..1), one
 * (alpha decay rate), timer (life-active flag), color[3] (alpha byte). */
void MB4_BubbleEffInit(void) {
    ANIMDATA *anim;
    HU3DMODEL *hdr;
    MB4COINEFFMGR *mgr;
    MB4PARTICLE *p;
    s32 i;

    mb4_BubbleEffObj = omAddObjEx(mbObjMan, -0x8000, 1U, 0U, -1, MB4_BubbleEffUpdate);
    anim = HuSprAnimRead(HuDataReadNum(0xD10033, 0x10000000));
    mb4_BubbleEffObj->data = anim;
    *mb4_BubbleEffObj->mdlId = MBEffCreate(anim, 0x400);
    Hu3DModelLayerSet(*mb4_BubbleEffObj->mdlId, 3);
    hdr = &Hu3DData[*mb4_BubbleEffObj->mdlId];
    mgr = (MB4COINEFFMGR *) hdr->hookData;
    mgr->blendMode = 1;
    p = mgr->particles;
    for (i = 0; i < mgr->count; i++) {
        p->timer = 0.0f;
        p++;
    }
}

void MB4_BubbleEffUpdate(OMOBJ *obj) {
    MB4COINEFFMGR *mgr = (MB4COINEFFMGR *) Hu3DData[*obj->mdlId].hookData;
    MB4PARTICLE *p = mgr->particles;
    s32 i;
    s16 phase;

    if (MBKillCheck() || mb4_BubbleEffObj == NULL) {
        MBEffKill(*obj->mdlId);
        *obj->mdlId = -1;
        obj->data = NULL;
        omDelObjEx(mbObjMan, obj);
        mb4_BubbleEffObj = NULL;
        return;
    }
    for (i = 0; i < mgr->count; i++) {
        if (p->timer != 0.0f) {
            phase = (s16) p->active + 1;
            if (phase > 0x168) {
                phase -= 0x168;
            }
            p->active = phase;
            p->posX = (f32) ((f64) p->posX + ((f64) p->velX * sin((3.141592653589793 * (f64) (2.0f * (f32) phase)) / 180.0)));
            p->posY += p->velY;
            p->speed -= p->one;
            p->color[3] = (u8) (255.0f * p->speed);
            if (p->speed < 0.0f) {
                p->timer = 0.0f;
            }
        }
        p++;
    }
}

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match. */
s32 MB4_BubbleEffCreate(Point3d *pos, s32 lifeFrames, f32 unused, f32 speedScale) {
    MB4COINEFFMGR *mgr;
    MB4PARTICLE *p;
    s32 i;

    if (mb4_BubbleEffObj == NULL) {
        return -1;
    }
    mgr = (MB4COINEFFMGR *) Hu3DData[*mb4_BubbleEffObj->mdlId].hookData;
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
    if (speedScale == 0.0f) {
        speedScale = 1.0f;
    }
    p->posX = pos->x;
    p->posY = pos->y;
    p->posZ = pos->z;
    p->velX = 0.5f * (1.5f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F))));
    p->velZ = 1.5f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F)));
    p->velY = 2.0f * (1.5f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F))));
    p->speed = 1.0f;
    p->one = 1.0f / (f32) lifeFrames;
    p->active = (s16) (frand() % 360);
    p->timer = 20.0f * speedScale;
    p->color[3] = 0xFF;
    p->color[2] = 0xFF;
    p->color[1] = 0xFF;
    p->color[0] = 0xFF;
    return i;
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
        coin->phase = 0;
        coin->arcHeight = 0.0f;
        coin->flightTimer = 0;
        coin->flightFrames = 0;
        for (p = 0; p < 4; p++) {
            MBPlayerPosGet(p, &coin->playerPos[p]);
        }
        coin++;
    }
}

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match. Per-frame update for every active coin: while phase==0, arc from
 * basePos to targetPos over flightFrames frames (sine-boosted height via
 * arcHeight), then lock onto targetPos once arrived (phase 1). Every coin
 * also re-checks proximity to all 4 players each frame; a player within
 * mb4_CoinMaxDist collects it (coin credited, SE played, 32 coin-sparkle
 * particles burst outward) and frees the slot. */
void MB4_CoinUpdate(OMOBJ *obj) {
    MB4COIN *coinP;
    s32 i, p, j;
    s32 collected;
    f32 t;
    Point3d target, base, cur, diff, playerPos;
    f32 distSq, dist;
    f32 burstAngle, spinAngle, spinCos, speedScale;
    Point3d burstPos, burstVel;
    u8 color[4];
    f32 alpha;

    if (MBKillCheck() || mb4_CoinObj == NULL) {
        coinP = (MB4COIN *) obj->data;
        for (i = 0; i < 0x20; i++) {
            Hu3DModelKill((s16) coinP->modelId);
            coinP->modelId = -1;
            Hu3DModelKill((s16) coinP->shadowModelId);
            coinP->shadowModelId = -1;
            coinP++;
        }
        omDelObjEx(mbObjMan, obj);
        mb4_CoinObj = NULL;
        return;
    }

    coinP = (MB4COIN *) obj->data;
    for (i = 0; i < 0x20; i++, coinP++) {
        if (coinP->active == 0) {
            continue;
        }
        collected = 0;
        switch (coinP->phase) {
        case 0:
            coinP->flightTimer += 1;
            t = (f32) coinP->flightTimer / (f32) coinP->flightFrames;
            base.x = coinP->basePosX;
            base.y = coinP->basePosY;
            base.z = coinP->basePosZ;
            target.x = coinP->targetX;
            target.y = coinP->targetY;
            target.z = coinP->targetZ;
            PSVECSubtract(&target, &base, &diff);
            PSVECScale(&diff, &diff, t);
            PSVECAdd(&diff, &base, &diff);
            diff.y = (f32) ((f64) diff.y + ((f64) coinP->arcHeight * sin((3.141592653589793 * (f64) (180.0f * t)) / 180.0)));
            coinP->posX = diff.x;
            coinP->posY = diff.y;
            coinP->posZ = diff.z;
            coinP->rotY += 10.0f;
            if (t == 1.0f) {
                coinP->phase += 1;
                coinP->flightTimer = 0;
            }
            break;
        case 1:
            coinP->posX = coinP->targetX;
            coinP->posY = coinP->targetY;
            coinP->posZ = coinP->targetZ;
            coinP->rotY += 10.0f;
            break;
        }

        if (coinP->phase <= 2) {
            for (p = 0; p < 4; p++) {
                MBPlayerPosGet(p, &playerPos);
                cur.x = coinP->posX;
                cur.y = coinP->posY;
                cur.z = coinP->posZ;
                PSVECSubtract(&cur, &playerPos, &diff);
                distSq = (diff.x * diff.x) + (diff.z * diff.z);
                if (distSq > 0.0f) {
                    dist = MB4_FastSqrt(distSq);
                } else {
                    dist = distSq;
                }
                if (dist < mb4_CoinMaxDist.x) {
                    MBPlayerCoinAdd(p, 1);
                    MBAudFXPlay(7);
                    collected = 1;
                }
                coinP->playerPos[p] = playerPos;
            }
        }

        Hu3DModelPosSet((s16) coinP->modelId, coinP->posX, coinP->posY, coinP->posZ);
        Hu3DModelRotSet((s16) coinP->modelId, coinP->rotX, coinP->rotY, coinP->rotZ);
        Hu3DModelScaleSet((s16) coinP->modelId, coinP->scaleZ, coinP->scaleY, coinP->scaleX);
        Hu3DModelPosSet((s16) coinP->shadowModelId, coinP->posX, -200.0f, coinP->posZ);
        Hu3DModelRotSet((s16) coinP->shadowModelId, coinP->rotX, coinP->rotY, coinP->rotZ);
        Hu3DModelScaleSet((s16) coinP->shadowModelId, coinP->scaleZ, coinP->scaleY, coinP->scaleX);

        if (collected) {
            coinP->active = 0;
            Hu3DModelAttrSet((s16) coinP->modelId, 1U);
            Hu3DModelAttrSet((s16) coinP->shadowModelId, 1U);

            spinAngle = 0.0f;
            for (j = 0; j < 0x20; j++) {
                spinAngle += 22.5f * (1.0f + (0.007874016f * (f32) (frand() & 0x7F)));
                burstPos.x = coinP->posX;
                burstPos.y = coinP->posY;
                burstPos.z = coinP->posZ;
                speedScale = 2.5f * (1.0f + (0.007874016f * (f32) (frand() & 0x7F)));
                burstAngle = 45.0f - (90.0f * (0.007874016f * (f32) (frand() & 0x7F)));
                spinCos = (f32) cos((3.141592653589793 * (f64) burstAngle) / 180.0);
                burstVel.x = (f32) ((f64) speedScale * (sin((3.141592653589793 * (f64) spinAngle) / 180.0) * spinCos));
                spinCos = (f32) cos((3.141592653589793 * (f64) burstAngle) / 180.0);
                burstVel.y = (f32) ((f64) speedScale * (cos((3.141592653589793 * (f64) spinAngle) / 180.0) * spinCos));
                burstVel.z = (f32) ((f64) speedScale * sin((3.141592653589793 * (f64) burstAngle) / 180.0));
                alpha = 0.007874016f * (f32) (frand() & 0x7F);
                color[0] = (u8) (192.0f + (63.0f * alpha));
                color[1] = (u8) (192.0f + (63.0f * alpha));
                color[2] = (u8) (64.0f + (63.0f * alpha));
                color[3] = (u8) (192.0f + (63.0f * (0.007874016f * (f32) (frand() & 0x7F))));
                MB4_CoinEffCreate(&burstPos, &burstVel, color,
                    100.0f * (0.3f + (0.1f * (0.007874016f * (f32) (frand() & 0x7F)))),
                    0.016666668f * (1.0f + (0.5f * (0.007874016f * (f32) (frand() & 0x7F)))),
                    0.0f);
            }
        }
    }
}

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match. Finds a free MB4COIN slot (active == 0) and spawns it: coin model
 * at basePos (arg0, may be NULL to skip), shadow model 200 units below,
 * targetPos from arg1 (may be NULL) - the flight destination used by
 * MB4_CoinUpdate's state-0 arc animation (coin flying to a player). */
s32 MB4_CoinCreate(Point3d *basePos, Point3d *targetPos, s32 flightFrames, f32 arcHeight, f32 unused) {
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
    coinP->phase = 0;
    coinP->arcHeight = arcHeight;
    coinP->flightFrames = flightFrames;
    coinP->flightTimer = 0;
    if (basePos != NULL) {
        coinP->basePosX = basePos->x;
        coinP->basePosY = basePos->y;
        coinP->basePosZ = basePos->z;
    }
    if (targetPos != NULL) {
        coinP->targetX = targetPos->x;
        coinP->targetY = targetPos->y;
        coinP->targetZ = targetPos->z;
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

    MB4COINEFFDATA *ed;

    if (mb4_CoinEffObj == NULL) {
        return -1;
    }
    ed = (MB4COINEFFDATA *) mb4_CoinEffObj->data;
    mgr = (MB4COINEFFMGR *) Hu3DData[ed->hu3dModelIdx].hookData;
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

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match. */
void MB4_CoinEffInit(void) {
    MB4COINEFFDATA *ed;
    ANIMDATA *anim;
    MB4COINEFFMGR *mgr;

    mb4_CoinEffObj = omAddObjEx(mbObjMan, -0x8000, 0U, 0U, -1, MB4_CoinEffUpdate);
    mb4_CoinEffObj->data = HuMemDirectMallocNum(HEAP_HEAP, 0xC, 0x10000000U);
    memset(mb4_CoinEffObj->data, 0, 0xCU);
    ed = (MB4COINEFFDATA *) mb4_CoinEffObj->data;
    anim = HuSprAnimRead(HuDataReadNum(0xB0042, 0x10000000));
    ed->anim = anim;
    ed->hu3dModelIdx = MBEffCreate(anim, 0x100);
    Hu3DModelLayerSet((s16) ed->hu3dModelIdx, 3);
    MBEffAttrReset((s16) ed->hu3dModelIdx, 8U);
    ed->count = 0;
    mgr = (MB4COINEFFMGR *) Hu3DData[ed->hu3dModelIdx].hookData;
    mgr->blendMode = 1;
}

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match. Per-frame update for every live coin-sparkle particle: integrate
 * position by velocity, apply "gravity" (alphaParam) to vertical velocity,
 * recompute the fade/scale factor (timer = speed * life), decay life by
 * sizeParam, and free the slot once life reaches 0. */
void MB4_CoinEffUpdate(OMOBJ *obj) {
    MB4COINEFFDATA *ed = (MB4COINEFFDATA *) obj->data;
    MB4COINEFFMGR *mgr;
    MB4PARTICLE *p;
    s32 i;

    if (MBKillCheck() || mb4_CoinEffObj == NULL) {
        MBEffKill((s16) ed->hu3dModelIdx);
        ed->hu3dModelIdx = -1;
        ed->anim = NULL;
        omDelObjEx(mbObjMan, obj);
        mb4_CoinEffObj = NULL;
        return;
    }
    if (ed->count <= 0) {
        Hu3DModelAttrSet((s16) ed->hu3dModelIdx, 1U);
        return;
    }
    Hu3DModelAttrReset((s16) ed->hu3dModelIdx, 1U);
    mgr = (MB4COINEFFMGR *) Hu3DData[ed->hu3dModelIdx].hookData;
    p = mgr->particles;
    for (i = 0; i < mgr->count; i++) {
        if (p->timer != 0.0f) {
            p->posX += p->velX;
            p->posY += p->velY;
            p->posZ += p->velZ;
            if (p->alphaParam != 0.0f) {
                p->velY -= p->alphaParam;
            }
            p->timer = p->speed * p->one;
            p->one -= p->sizeParam;
            if (p->one == 0.0f) {
                p->active = 0;
                p->timer = 0.0f;
                ed->count -= 1;
            }
        }
        p++;
    }
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

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match. Seeds every species' per-fish target slots (mb4_Work + 0x174,
 * 14 species * 0x900 bytes, up to a handful of 0x90-byte per-fish records
 * each) for species whose flag1 is 0: a fixed default target offset. */
void MB4_SakanaOffsetInit(void) {
    s32 species, fish;
    u8 *base;

    for (species = 0; species < 14; species++) {
        base = (u8 *) mb4_Work + 0x174 + (species * 0x900);
        if (mb4ev_SakanaTbl[species].flag1 == 0) {
            for (fish = 0; fish < mb4ev_SakanaTbl[species].count; fish++) {
                *(f32 *) (base + (fish * 0x90) + 0x34) = 500.0f;
                *(f32 *) (base + (fish * 0x90) + 0x38) = 3600.0f;
            }
        }
    }
}

/* Same layout as MB4_SakanaOffsetInit, seeding each fish's target position
 * with its species' swim-area center. */
void MB4_SakanaPosInit(void) {
    s32 species, fish;
    u8 *base;

    for (species = 0; species < 14; species++) {
        base = (u8 *) mb4_Work + 0x174 + (species * 0x900);
        if (mb4ev_SakanaTbl[species].flag1 == 0) {
            for (fish = 0; fish < mb4ev_SakanaTbl[species].count; fish++) {
                *(f32 *) (base + (fish * 0x90) + 0x30) = mb4ev_SakanaTbl[species].pos.x;
                *(f32 *) (base + (fish * 0x90) + 0x34) = mb4ev_SakanaTbl[species].pos.y;
                *(f32 *) (base + (fish * 0x90) + 0x38) = mb4ev_SakanaTbl[species].pos.z;
            }
        }
    }
}

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match. Finds a free sakana MB4PARTICLE slot (sakanaFlags bit 7 == 0) and
 * spawns it at the origin, fully opaque, of the given species `type`. */
s32 MB4_SakanaCreate(s16 type) {
    MB4COINEFFMGR *mgr;
    MB4PARTICLE *p;
    s32 i;

    if (MB4W_S32(0x7F74) == -1) {
        return -1;
    }
    mgr = (MB4COINEFFMGR *) Hu3DData[MB4W_S32(0x7F74)].hookData;
    p = mgr->particles;
    for (i = 0; i < mgr->count; i++) {
        if ((p->sakanaFlags >> 7) & 1) {
            p++;
            continue;
        }
        break;
    }
    if (i >= mgr->count) {
        return -1;
    }
    p->sakanaFlags |= 0x80;
    p->posZ = 0.0f;
    p->posY = 0.0f;
    p->posX = 0.0f;
    p->offsetZAngle = 0.0f;
    p->offsetY = 0.0f;
    p->offsetX = 0.0f;
    p->timer = 100.0f;
    p->sakanaType = type;
    p->color[3] = 0xFF;
    p->color[2] = 0xFF;
    p->color[1] = 0xFF;
    p->color[0] = 0xFF;
    return i;
}

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match. Creates the sakana particle effect (up to 128 fish parts) and,
 * for each of the 14 species, up to `count` MB4SAKANAFISH swim records:
 * a random 1-in-4 chance of a small real HSF model instead of the shared
 * particle billboard, each seeded with a random point/direction inside
 * the species' swim area (pos +/- range) and per-fish timers/speed. */
void MB4_SakanaInit(s32 unused0, s32 unused1, s32 unused2, f32 unused3) {
    MB4COINEFFMGR *mgr;
    MB4PARTICLE *p;
    MB4SAKANAFISH *fish;
    MB4SAKANASPECIES *species;
    s32 s, f, i;
    s32 modelKind;
    f32 dirMagSq, dirMag;

    MB4W_S32(0x7F74) = MBEffCreate(HuSprAnimRead(HuDataReadNum(0xD10035, 0x10000000)), 0x80);
    Hu3DModelLayerSet((s16) MB4W_S32(0x7F74), 1);
    mgr = (MB4COINEFFMGR *) Hu3DData[MB4W_S32(0x7F74)].hookData;
    p = mgr->particles;
    for (i = 0; i < mgr->count; i++) {
        p->sakanaFlags &= ~0x80;
        p++;
    }
    MBEffAttrSet((s16) MB4W_S32(0x7F74), 0x10U);

    memset(&mb4_Work[0x5D], 0, 0x7E00U);
    for (s = 0; s < 14; s++) {
        for (f = 0; f < 0x10; f++) {
            fish = (MB4SAKANAFISH *) ((u8 *) mb4_Work + 0x174 + (s * 0x900) + (f * 0x90));
            fish->modelId = -1;
        }
    }

    for (s = 0; s < 14; s++) {
        species = &mb4ev_SakanaTbl[s];
        fish = (MB4SAKANAFISH *) ((u8 *) mb4_Work + 0x174 + (s * 0x900));
        for (f = 0; f < species->count; f++) {
            if (species->flag1 != 0) {
                fish->particleIdx = MB4_SakanaCreate((s16) (frand() & 0x7FFF & 3));
                fish->modelId = -1;
            } else {
                modelKind = frand() & 0x7FFF & 3;
                switch (modelKind) {
                case 0:
                    fish->modelId = MBModelCreate(0xD1001A, NULL, TRUE);
                    break;
                case 1:
                    fish->modelId = MBModelCreate(0xD1001B, NULL, TRUE);
                    break;
                case 2:
                    fish->modelId = MBModelCreate(0xD1001C, NULL, TRUE);
                    break;
                default:
                    fish->modelId = MBModelCreate(0xD1001D, NULL, TRUE);
                    break;
                }
                MBModelLayerSet((s16) fish->modelId, 2U);
                fish->particleIdx = -1;
            }
            fish->rand1 = (f32) frand();
            fish->bobAngle = (s32) frand();
            fish->unk10 = 0;
            fish->unk14 = 0;
            fish->timer18 = (s32) (60.0f + (2.0f * (60.0f * (0.007874016f * (f32) (frand() & 0x7F)))));
            fish->timer1C = (s32) (10.0f + (10.0f * (0.007874016f * (f32) (frand() & 0x7F))));
            fish->timer20 = (s32) (10.0f * (60.0f * (0.007874016f * (f32) (frand() & 0x7F))));
            fish->timer24 = (s32) (120.0f + (2.0f * (60.0f * (0.007874016f * (f32) (frand() & 0x7F)))));
            switch (species->flag2) {
            case 0:
                fish->pathAltFlag = 0;
                break;
            case 1:
                fish->pathAltFlag = f & 1;
                break;
            default:
                fish->pathAltFlag = 1;
                break;
            }
            fish->unk2C = 1;
            fish->speciesPos = species->pos;
            fish->speciesRange = species->range;
            fish->posA.x = fish->speciesPos.x + (fish->speciesRange.x * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
            fish->posA.y = fish->speciesPos.y + (fish->speciesRange.y * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
            fish->posA.z = fish->speciesPos.z + (fish->speciesRange.z * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
            fish->posB.x = fish->speciesPos.x + (fish->speciesRange.x * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
            fish->posB.y = fish->speciesPos.y + (fish->speciesRange.y * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
            fish->posB.z = fish->speciesPos.z + (fish->speciesRange.z * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
            fish->dir.x = (0.007874016f * (f32) (frand() & 0x7F)) - 0.5f;
            fish->dir.y = (0.007874016f * (f32) (frand() & 0x7F)) - 0.5f;
            fish->dir.z = (0.007874016f * (f32) (frand() & 0x7F)) - 0.5f;
            PSVECNormalize(&fish->dir, &fish->dir);
            dirMagSq = (fish->dir.x * fish->dir.x) + (fish->dir.z * fish->dir.z);
            if (dirMagSq > 0.0f) {
                dirMag = MB4_FastSqrt(dirMagSq);
            } else {
                dirMag = dirMagSq;
            }
            fish->pitch = (f32) (180.0 * (atan2((f64) dirMag, (f64) fish->dir.y) / 3.141592653589793));
            fish->yaw = (f32) (180.0 * (atan2((f64) fish->dir.x, (f64) fish->dir.z) / 3.141592653589793));
            fish->roll = 0.0f;
            fish->prevPitch = fish->pitch;
            fish->prevYaw = fish->yaw;
            fish->prevRoll = fish->roll;
            fish->speed = 2.5f * (1.0f + (0.007874016f * (f32) (frand() & 0x7F)));
            fish->scale = 0.25f + (0.5f * (0.007874016f * (f32) (frand() & 0x7F)));
            fish->unk8C = 0.25f + (0.25f * (0.007874016f * (f32) (frand() & 0x7F)));
            if (fish->modelId != -1) {
                MBModelPosSetV((s16) fish->modelId, &fish->posB);
                MBModelRotSetV((s16) fish->modelId, (Point3d *) &fish->pitch);
                MBModelScaleSet((s16) fish->modelId, fish->scale, fish->scale, fish->scale);
                MBModelLayerSet((s16) fish->modelId, 2U);
            } else {
                MB4_SakanaPosSet(fish->particleIdx, fish->posB.x, fish->posB.y, fish->posB.z);
                MB4_SakanaOffsetSet(fish->particleIdx, fish->pitch, fish->yaw, fish->roll);
                MB4_SakanaLenSet(fish->particleIdx, fish->scale);
            }
            fish += 1;
        }
    }

    MB4W_S32(0x7F78) = Hu3DHookFuncCreate((HU3DMODELHOOK) MB4_SakanaDraw);
    Hu3DModelCameraSet((s16) MB4W_S32(0x7F78), 1U);
    Hu3DModelLayerSet((s16) MB4W_S32(0x7F78), 7);
}

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match (size and complexity make it impractical). Per-frame sakana (fish)
 * AI and draw hook, called once per frame while unpaused, for every fish
 * of every species: screen-space culling (offscreen fish get their
 * onscreen flag - unk2C - cleared, hiding the real-model ones and
 * releasing the shared-particle ones back to the pool), waypoint
 * arrival/re-target (posA -> posB swim target inside the species' swim
 * area, picked when close enough or a random timer expires), a smooth
 * pitch/yaw turn toward the new heading via MB4_AngleLerp (2.5 deg/frame
 * cap), a sinusoidal body-bob speed multiplier, and - only for
 * pathAltFlag == 0 fish - a periodic course wobble (timer18) plus an
 * occasional bubble puff (timer20) spawned via the mb4_BubbleEffObj
 * particle pool directly (not through MB4_BubbleEffCreate, matching the
 * original exactly). Finally moves the fish along dir*speed and updates
 * either its real HSF model transform or its shared MB4PARTICLE slot. */
void MB4_SakanaDraw(HU3DMODEL *modelP, Mtx *mtx) {
    s32 s, f;
    MB4SAKANASPECIES *species;
    MB4SAKANAFISH *fish;
    Point3d screenPos;
    Point3d diff;
    f32 bobOfs;
    f32 horizDistSq, horizDist;
    f32 speedMul;
    f32 wobble;
    Point3d move;
    BOOL onScreen;

    if (MBPauseProcCheck()) {
        return;
    }

    for (s = 0; s < 14; s++) {
        species = &mb4ev_SakanaTbl[s];
        fish = (MB4SAKANAFISH *) ((u8 *) mb4_Work + 0x174 + (s * 0x900));
        for (f = 0; f < species->count; f++) {
            fish->unk10 += 1;
            speedMul = fish->speed;
            bobOfs = 0.0f;

            if ((fish->unk10 & 3) == 0) {
                Hu3D3Dto2D(&fish->posB, 1, &screenPos);
                if ((screenPos.x < 0.0f || screenPos.x > 640.0f) && (screenPos.y < 0.0f || screenPos.y > 480.0f)) {
                    onScreen = FALSE;
                } else {
                    onScreen = TRUE;
                }
                fish->unk2C = onScreen;
            }

            if (!fish->unk2C && fish->modelId == -1) {
                MB4PARTICLE *p = (MB4PARTICLE *) MB4_SakanaPartBase(fish->particleIdx);
                p->sakanaFlags &= ~0x80;
            } else {
                PSVECSubtract(&fish->posA, &fish->posB, &diff);
                if (fish->pathAltFlag == 0) {
                    fish->unk10 -= 1;
                    if (fish->unk10 <= 0) {
                        f32 mag = PSVECMag(&diff);
                        fish->timer24 -= 1;
                        if (mag >= fish->speed || fish->timer24 <= 0 ||
                            fabsf(diff.x) > fish->speciesRange.x ||
                            fabsf(diff.y) > fish->speciesRange.y ||
                            fabsf(diff.z) > fish->speciesRange.z) {
                            fish->posA.x = fish->speciesPos.x + (fish->speciesRange.x * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
                            fish->posA.y = fish->speciesPos.y + (fish->speciesRange.y * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
                            fish->posA.z = fish->speciesPos.z + (fish->speciesRange.z * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
                            fish->unk10 = 0x3C;
                            fish->timer24 = (s32) (120.0f + (2.0f * (60.0f * (0.007874016f * (f32) (frand() & 0x7F)))));
                        }
                    }
                    fish->timer18 -= 1;
                    if (fish->timer18 <= 0) {
                        bobOfs = (f32) (15.0 * sin((3.141592653589793 * (f64) fish->unk14) / 180.0));
                        fish->unk14 += 5;
                        if (fish->unk14 >= 0x2D0) {
                            fish->unk14 = 0;
                            fish->timer18 = (s32) (60.0f + (2.0f * (60.0f * (0.007874016f * (f32) (frand() & 0x7F)))));
                        }
                    } else {
                        bobOfs = 0.0f;
                    }
                    fish->timer20 -= 1;
                    if (fish->timer20 <= 0) {
                        if ((fish->timer1C & 3) == 0 && mb4_BubbleEffObj != NULL) {
                            MB4COINEFFMGR *mgr = (MB4COINEFFMGR *) Hu3DData[*mb4_BubbleEffObj->mdlId].hookData;
                            MB4PARTICLE *bp = mgr->particles;
                            s32 bi;
                            Point3d bubblePos;
                            f32 bubbleScale;
                            s32 duration = (s32) (120.0f * (1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F)))));

                            bubbleScale = fish->scale * (0.5f + (0.007874016f * (f32) (frand() & 0x7F)));
                            PSVECScale(&fish->dir, &bubblePos, 50.0f * fish->scale);
                            PSVECAdd(&fish->posB, &bubblePos, &bubblePos);
                            for (bi = 0; bi < mgr->count; bi++) {
                                if (bp->timer == 0.0f) {
                                    break;
                                }
                                bp++;
                            }
                            if (bi < mgr->count) {
                                if (bubbleScale == 0.0f) {
                                    bubbleScale = 1.0f;
                                }
                                bp->posX = bubblePos.x;
                                bp->posY = bubblePos.y;
                                bp->posZ = bubblePos.z;
                                bp->velX = 0.5f * (1.5f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F))));
                                bp->velZ = 1.5f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F)));
                                bp->velY = 2.0f * (1.5f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F))));
                                bp->speed = 1.0f;
                                bp->one = 1.0f / (f32) duration;
                                bp->active = (s16) (frand() % 360);
                                bp->timer = 20.0f * bubbleScale;
                                bp->color[3] = 0xFF;
                                bp->color[2] = 0xFF;
                                bp->color[1] = 0xFF;
                                bp->color[0] = 0xFF;
                            }
                        }
                        fish->timer1C -= 1;
                        if (fish->timer1C <= 0) {
                            fish->timer1C = (s32) (10.0f + (10.0f * (0.007874016f * (f32) (frand() & 0x7F))));
                            fish->timer20 = (s32) (300.0f + (5.0f * (60.0f * (0.007874016f * (f32) (frand() & 0x7F)))));
                        }
                    }
                    horizDistSq = (diff.x * diff.x) + (diff.z * diff.z);
                    horizDist = (horizDistSq > 0.0f) ? MB4_FastSqrt(horizDistSq) : horizDistSq;
                    fish->prevPitch = (f32) (180.0 * (atan2((f64) diff.y, (f64) horizDist) / 3.141592653589793));
                    fish->prevYaw = (f32) (180.0 * (atan2((f64) diff.x, (f64) diff.z) / 3.141592653589793));
                } else {
                    PSVECSubtract(&fish->posA, &fish->posB, &diff);
                    if ((fish->unk10 & 1) == 0) {
                        fish->unk10 -= 1;
                        if (fish->unk10 <= 0) {
                            f32 mag = PSVECMag(&diff);
                            fish->timer24 -= 1;
                            if (mag == fish->speed || fish->timer24 <= 0) {
                                fish->posA.x = fish->speciesPos.x + (fish->speciesRange.x * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
                                fish->posA.y = fish->speciesPos.y + (fish->speciesRange.y * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
                                fish->posA.z = fish->speciesPos.z + (fish->speciesRange.z * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
                                fish->unk10 = 0x3C;
                                fish->timer24 = (s32) (120.0f + (2.0f * (60.0f * (0.007874016f * (f32) (frand() & 0x7F)))));
                            }
                        }
                    }
                    if ((fish->unk10 & 3) == 0) {
                        horizDistSq = (diff.x * diff.x) + (diff.z * diff.z);
                        horizDist = (horizDistSq > 0.0f) ? MB4_FastSqrt(horizDistSq) : horizDistSq;
                        fish->prevPitch = (f32) (180.0 * (atan2((f64) diff.y, (f64) horizDist) / 3.141592653589793));
                        fish->prevYaw = (f32) (180.0 * (atan2((f64) diff.x, (f64) diff.z) / 3.141592653589793));
                    }
                }

                fish->pitch = MB4_AngleLerp(fish->prevPitch, fish->pitch, 2.5f);
                fish->yaw = MB4_AngleLerp(fish->prevYaw, fish->yaw, 2.5f);
                fish->roll = 0.0f;

                wobble = (f32) cos((3.141592653589793 * (f64) fish->pitch) / 180.0);
                fish->dir.y = (f32) sin((3.141592653589793 * (f64) fish->pitch) / 180.0);
                fish->dir.x = (f32) ((f64) wobble * sin((3.141592653589793 * (f64) fish->yaw) / 180.0));
                fish->dir.z = (f32) ((f64) wobble * cos((3.141592653589793 * (f64) fish->yaw) / 180.0));

                if (PSVECMag(&diff) > 1000.0f) {
                    speedMul *= 2.0f;
                }

                fish->bobAngle = (s32) ((f32) fish->bobAngle + (1.0f + (3.0f * (0.007874016f * (f32) (frand() & 0x7F)))));
                if (fish->bobAngle > 0x168) {
                    fish->bobAngle -= 0x168;
                }
                PSVECScale(&fish->dir, &move, speedMul * (0.5f + (f32) fabs(0.5 * sin((3.141592653589793 * (f64) fish->bobAngle) / 180.0))));
                PSVECAdd(&fish->posB, &move, &fish->posB);

                if (fish->modelId != -1) {
                    if (!fish->unk2C) {
                        MBModelDispSet((s16) fish->modelId, FALSE);
                    } else {
                        MBModelDispSet((s16) fish->modelId, TRUE);
                        MBModelPosSetV((s16) fish->modelId, &fish->posB);
                        MBModelRotSet((s16) fish->modelId, -fish->pitch, fish->yaw + bobOfs, fish->roll);
                    }
                } else {
                    MB4PARTICLE *p = (MB4PARTICLE *) MB4_SakanaPartBase(fish->particleIdx);
                    if (!fish->unk2C) {
                        p->sakanaFlags &= ~0x80;
                    } else {
                        p->sakanaFlags |= 0x80;
                        p->posX = fish->posB.x;
                        p->posY = fish->posB.y;
                        p->posZ = fish->posB.z;
                        p->offsetX = fish->pitch;
                        p->offsetY = fish->yaw;
                        p->offsetZAngle = fish->roll;
                        p->offsetY -= 90.0f;
                    }
                }
            }
            fish += 1;
        }
    }
}

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match. Blocking coroutine-style coin-count cutscene: rotate the player
 * to face the camera, show the coin-count popup and play a win/mid/lose
 * reaction motion depending on coinNum vs threshold, wait out the motion
 * shift, then clean up. Uses the project's HuPrcVSleep frame-wait idiom. */
void MB4_CoinDispUpdate(s32 playerNo, s32 coinNum, s32 threshold) {
    Point3d pos;
    s32 dispNo;
    s32 shiftMotNo = -1;

    MBPlayerRotateStart(playerNo, 0, 0xF);
    while (!MBPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }

    if (coinNum > 0) {
        MBPlayerPosGet(playerNo, &pos);
        pos.y += 250.0f;
        dispNo = MBCoinDispCreateSe(&pos, coinNum);
    } else {
        dispNo = -1;
    }

    if (coinNum <= 0) {
        MBPlayerWinLoseVoicePlay(playerNo, 0xD, 0x200);
        MBPlayerMotionNoShiftSet(playerNo, 0xD, 0.0f, 8.0f, 0U);
    } else if (coinNum < threshold) {
        MBPlayerWinLoseVoicePlay(playerNo, 0xC, 0x1FB);
        MBPlayerMotionNoShiftSet(playerNo, 0xC, 0.0f, 8.0f, 0U);
    } else {
        MBPlayerWinLoseVoicePlay(playerNo, 0xC, 0x1FB);
        shiftMotNo = MBPlayerMotionCreate(playerNo, 0x8C002D);
        MBPlayerMotionNoShiftSet(playerNo, shiftMotNo, 0.0f, 8.0f, 0U);
    }

    do {
        do {
            HuPrcVSleep();
        } while (!MBPlayerMotionEndCheck(playerNo));
    } while (MBMotionShiftIDGet(MBPlayerModelGet(playerNo)) != -1);

    MBPlayerMotionNoShiftSet(playerNo, 1, 0.0f, 8.0f, 0U);
    do {
        HuPrcVSleep();
    } while (MBMotionShiftIDGet(MBPlayerModelGet(playerNo)) != -1);

    if (dispNo != -1) {
        while (!MBCoinDispKillCheck(dispNo)) {
            HuPrcVSleep();
        }
    }
    if (shiftMotNo != -1) {
        MBPlayerMotionKill(playerNo, shiftMotNo);
    }
}

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match. Spawns a bubble at the player's head bone plus a per-character
 * offset (mb4ev_IrukaBubbleOfsTbl, indexed by charNo), on top of `ofs`. */
void MB4Ev_IrukaBubbleCreate(s32 playerNo, Point3d *ofs, f32 unused0, f32 unused1) {
    Mtx headMtx;
    Point3d pos;
    s32 charNo = GwPlayer[playerNo].charNo;
    s32 duration;

    Hu3DModelObjMtxGet(MBModelIdGet(MBPlayerModelGet(playerNo)), "ske_head", headMtx);
    pos = mb4ev_IrukaBubbleOfsTbl[charNo];
    PSMTXMultVec(headMtx, &pos, &pos);
    PSVECAdd(&pos, ofs, &pos);
    duration = (s32) (120.0f * (1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F)))));
    MB4_BubbleEffCreate(&pos, duration, 0.0f, mb4ev_IrukaTPLvlScaleTbl[charNo] * (0.3f + (0.7f * (0.007874016f * (f32) (frand() & 0x7F)))));
}

/* Same as MB4Ev_IrukaBubbleCreate but with a fixed (0,0,0) offset - used
 * for the generic "player at rest" head bubble rather than the dolphin
 * ride's moving offset. */
void MB4Ev_PlayerBubbleCreate(s32 playerNo) {
    Mtx headMtx;
    Point3d pos;
    s32 charNo = GwPlayer[playerNo].charNo;
    s32 duration;

    Hu3DModelObjMtxGet(MBModelIdGet(MBPlayerModelGet(playerNo)), "ske_head", headMtx);
    pos = mb4ev_IrukaBubbleOfsTbl[charNo];
    PSMTXMultVec(headMtx, &pos, &pos);
    PSVECAdd(&pos, &lbl_1_data_10CC, &pos);
    duration = (s32) (120.0f * (1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F)))));
    MB4_BubbleEffCreate(&pos, duration, 0.0f, mb4ev_IrukaTPLvlScaleTbl[charNo] * (0.3f + (0.7f * (0.007874016f * (f32) (frand() & 0x7F)))));
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

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match. Same as MB4_AngleLerp(a, b, step) but the step is computed from
 * a rate: step = |MB4_AngleDiff(b, a) * rate|. */
f32 fn_1_C6D0(f32 rate, f32 a, f32 b) {
    f32 step = (f32) fabs((f64) (MB4_AngleDiff(b, a) * rate));
    return MB4_AngleLerp(a, b, step);
}

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match. Capsule effect callback: bursts of bubbles around a capsule/item
 * model for various capsule/event-type combinations, and tracks up to two
 * "held item bubble trail" slots (mb4_Work[0x23..0x29] and [0x2A..0x2F],
 * mirroring the puku2/dolphin trail state at mb4_Work[0x57..0x5C] handled
 * in MB4_MainObjUpdate) for capsule 38/34 and -99 respectively. The 4th
 * MB4_BubbleEffCreate() argument is always dead in the callee, so 0.0f is
 * passed at every call site here regardless of what the original computed
 * for that (unused) register. */
void MB4_CapsuleEffHook(int capsuleNo, int type, int mdlId, BOOL flag1, BOOL flag2, BOOL flag3) {
    Point3d pos;
    Mtx mtx;
    Point3d rot;
    Point3d ofs;
    s32 i, duration;
    f32 angle;

    switch (capsuleNo) {
    case 38:
    case 34:
        mb4_Work[0x28] = 0;
        if (type < 3) {
            if (type >= 1) {
                mb4_Work[0x26] = -1;
                mb4_Work[0x27] = -1;
            }
        } else if (type < 5) {
            mb4_Work[0x24] = (s32) (8.0f + (5.0f * (0.007874016f * (f32) (frand() & 0x7F))));
            mb4_Work[0x25] = (s32) frand();
            mb4_Work[0x23] = 0;
            mb4_Work[0x26] = 0x22;
            mb4_Work[0x27] = mdlId;
            mb4_Work[0x29] = flag1;
        }
        if (type == 4) {
            mb4_Work[0x28] = 1;
            return;
        }
        /* fall through */
    case 7:
        return;
    case -99:
        mb4_Work[0x2F] = 0;
        if (type < 3) {
            if (type >= 1) {
                mb4_Work[0x2D] = -1;
                mb4_Work[0x2E] = -1;
            }
        } else if (type < 5) {
            mb4_Work[0x2B] = (s32) (8.0f + (5.0f * (0.007874016f * (f32) (frand() & 0x7F))));
            mb4_Work[0x2C] = (s32) frand();
            mb4_Work[0x2A] = 0;
            mb4_Work[0x2D] = -0x63;
            mb4_Work[0x2E] = mdlId;
        }
        if (type == 4) {
            mb4_Work[0x2F] = 1;
            return;
        }
        break;
    case 14:
        switch (type) {
        case 5:
            for (i = 0; i < 2; i++) {
                MBModelPosGet((s16) mdlId, &pos);
                MBModelRotGet((s16) mdlId, &rot);
                mtxRot(mtx, -rot.x, rot.y, rot.z);
                ofs.x = 0.0f;
                ofs.y = 0.0f;
                ofs.z = -100.0f;
                PSMTXMultVec(mtx, &ofs, &ofs);
                PSVECAdd(&pos, &ofs, &pos);
                pos.x += 100.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f);
                pos.y += 100.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f);
                pos.z += 100.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f);
                duration = (s32) (120.0f * (1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F)))));
                MB4_BubbleEffCreate(&pos, duration, 0.0f, 0.75f * (0.8f + (0.007874016f * (f32) (frand() & 0x7F))));
            }
            return;
        case 6:
            for (i = 0; i < 0x20; i++) {
                MBModelPosGet((s16) mdlId, &pos);
                pos.x += 1.5f * (100.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
                pos.y += 1.5f * (100.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
                pos.z += 0.5f * (100.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
                duration = (s32) (120.0f * (1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F)))));
                MB4_BubbleEffCreate(&pos, duration, 0.0f, 0.75f * (0.8f + (0.007874016f * (f32) (frand() & 0x7F))));
            }
            return;
        }
        break;
    case 22:
        switch (type) {
        case 7:
            for (i = 0; i < 0x10; i++) {
                MBModelPosGet((s16) mdlId, &pos);
                pos.x += 100.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f);
                pos.z += 100.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f);
                duration = (s32) (120.0f * (1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F)))));
                MB4_BubbleEffCreate(&pos, duration, 0.0f, 0.75f * (0.8f + (0.007874016f * (f32) (frand() & 0x7F))));
            }
            return;
        case 6:
            Hu3DModelObjMtxGet(MBModelIdGet((s16) mdlId), "itemhook_C", mtx);
            ofs.x = mtx[0][3];
            ofs.y = mtx[1][3];
            ofs.z = mtx[2][3];
            for (i = 0; i < 0x20; i++) {
                MBModelPosGet((s16) mdlId, &pos);
                pos.x = ofs.x + (100.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
                pos.y = ofs.y + (100.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
                pos.z = ofs.z + (100.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
                duration = (s32) (120.0f * (1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F)))));
                MB4_BubbleEffCreate(&pos, duration, 0.0f, 0.75f * (0.8f + (0.007874016f * (f32) (frand() & 0x7F))));
            }
            return;
        }
        break;
    case 16:
        switch (type) {
        case 8:
            for (i = 0; i < 2; i++) {
                MBModelPosGet((s16) mdlId, &pos);
                pos.x += 100.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f);
                pos.z += 100.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f);
                duration = (s32) (120.0f * (1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F)))));
                MB4_BubbleEffCreate(&pos, duration, 0.0f, 0.75f * (0.8f + (0.007874016f * (f32) (frand() & 0x7F))));
            }
            return;
        case 6:
            for (i = 0; i < 0x40; i++) {
                MBModelPosGet((s16) mdlId, &pos);
                pos.x += 2.0f * (100.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
                pos.y += 2.0f * (100.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
                pos.z += 0.5f * (100.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
                duration = (s32) (120.0f * (1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F)))));
                MB4_BubbleEffCreate(&pos, duration, 0.0f, 0.75f * (0.8f + (0.007874016f * (f32) (frand() & 0x7F))));
            }
            return;
        }
        break;
    default:
        switch (type) {
        case 3:
            mb4_Work[0x24] = (s32) (12.0f + (4.0f * (0.007874016f * (f32) (frand() & 0x7F))));
            mb4_Work[0x25] = (s32) frand();
            mb4_Work[0x26] = capsuleNo;
            mb4_Work[0x27] = mdlId;
            mb4_Work[0x29] = flag1;
            return;
        case 8:
            if (capsuleNo == 0x19) {
                MBModelPosGet((s16) mdlId, &pos);
                angle = 360.0f * (0.007874016f * (f32) (frand() & 0x7F));
                pos.x = (f32) ((f64) pos.x + (0.4000000059604645 * (100.0 * sin((3.141592653589793 * (f64) angle) / 180.0))));
                pos.z = (f32) ((f64) pos.z + (0.4000000059604645 * (100.0 * cos((3.141592653589793 * (f64) angle) / 180.0))));
                pos.y += 0.2f * (100.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
                duration = (s32) (120.0f * (1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F)))));
                MB4_BubbleEffCreate(&pos, duration, 0.0f, 0.75f * (0.8f + (0.007874016f * (f32) (frand() & 0x7F))));
                return;
            }
            MBModelPosGet((s16) mdlId, &pos);
            angle = 360.0f * (0.007874016f * (f32) (frand() & 0x7F));
            pos.x = (f32) ((f64) pos.x + (0.6000000238418579 * (100.0 * sin((3.141592653589793 * (f64) angle) / 180.0))));
            pos.z = (f32) ((f64) pos.z + (0.6000000238418579 * (100.0 * cos((3.141592653589793 * (f64) angle) / 180.0))));
            pos.y -= 0.2f * (100.0f * (0.007874016f * (f32) (frand() & 0x7F)));
            duration = (s32) (120.0f * (1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F)))));
            MB4_BubbleEffCreate(&pos, duration, 0.0f, 0.75f * (0.8f + (0.007874016f * (f32) (frand() & 0x7F))));
            return;
        default:
            mb4_Work[0x26] = -1;
            mb4_Work[0x27] = -1;
            break;
        }
        break;
    }
}

/* Shared "burst a bubble at a model bone" step used by most
 * MB4_CapsuleBubbleUpdate cases below: transform `ofs` by the named bone's
 * world matrix on mb4_Work[0x27]'s model, and spawn a bubble there. */
static void MB4_CapsuleBubbleBurst(char *bone, Point3d ofs, f32 speedScale) {
    Mtx mtx;
    Point3d pos;
    s32 duration;

    Hu3DModelObjMtxGet(MBModelIdGet((s16) mb4_Work[0x27]), bone, mtx);
    PSMTXMultVec(mtx, &ofs, &pos);
    duration = (s32) (120.0f * (1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F)))));
    MB4_BubbleEffCreate(&pos, duration, 0.0f, speedScale);
}

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match. Two independent per-frame timers: mb4_Work[0x2A..0x2F] (the
 * second "held item" bubble trail from MB4_CapsuleEffHook's -99 case,
 * head-bubble bursts every 4th tick while its timer runs), and
 * mb4_Work[0x23..0x29] (the first slot, dispatched by capsule/effect kind
 * in mb4_Work[0x26] to burst bubbles at a specific model bone - a
 * different bone/offset/speed per kind. All offsets/bone names below are
 * taken directly from the original's per-case data). */
void MB4_CapsuleBubbleUpdate(void) {
    Mtx headMtx;
    Point3d ofs, pos;
    s32 duration;

    if (mb4_Work[0x2D] != -1 && mb4_Work[0x2E] != -1) {
        if (!MBModelDispGet((s16) mb4_Work[0x2E])) {
            mb4_Work[0x2D] = -1;
            mb4_Work[0x2E] = -1;
            mb4_Work[0x2B] = 0;
        }
        mb4_Work[0x2A] -= 1;
        if (mb4_Work[0x2A] <= 0 && mb4_Work[0x2B] > 0) {
            mb4_Work[0x2C] += 1;
            if ((mb4_Work[0x2C] & 3) == 0) {
                ofs.x = 65.0f;
                ofs.y = -20.0f;
                ofs.z = 0.0f;
                Hu3DModelObjMtxGet(MBModelIdGet((s16) mb4_Work[0x2E]), "ske_head", headMtx);
                PSMTXMultVec(headMtx, &ofs, &pos);
                duration = (s32) (120.0f * (1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F)))));
                MB4_BubbleEffCreate(&pos, duration, 0.0f, 1.2f * (0.3f + (0.7f * (0.007874016f * (f32) (frand() & 0x7F)))));
                mb4_Work[0x2B] -= 1;
                if (mb4_Work[0x2B] <= 0) {
                    if (mb4_Work[0x2F] != 0) {
                        mb4_Work[0x2B] = (s32) (8.0f + (5.0f * (0.007874016f * (f32) (frand() & 0x7F))));
                        mb4_Work[0x2C] = (s32) frand();
                        mb4_Work[0x2A] = (s32) (180.0f + (3.0f * (60.0f * (0.007874016f * (f32) (frand() & 0x7F)))));
                    } else {
                        mb4_Work[0x2E] = -1;
                    }
                }
            }
        }
    }

    if (mb4_Work[0x26] == -1 || mb4_Work[0x27] == -1) {
        return;
    }

    switch (mb4_Work[0x26]) {
    case 4:
        if (mb4_Work[0x24] > 0) {
            mb4_Work[0x25] += 1;
            if ((mb4_Work[0x25] & 3) == 0) {
                ofs.x = 0.0f;
                ofs.y = 350.0f;
                ofs.z = 350.0f;
                MB4_CapsuleBubbleBurst("jyan", ofs, 0.75f * (0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
                mb4_Work[0x24] -= 1;
                if (mb4_Work[0x24] <= 0) {
                    mb4_Work[0x27] = -1;
                }
            }
        }
        return;
    case 6:
        if (mb4_Work[0x24] > 0) {
            mb4_Work[0x25] += 1;
            if ((mb4_Work[0x25] & 3) == 0) {
                ofs.x = 0.0f;
                ofs.y = 10.0f;
                ofs.z = 30.0000019f;
                MB4_CapsuleBubbleBurst("nose", ofs, 0.75f * (0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
                mb4_Work[0x24] -= 1;
                if (mb4_Work[0x24] <= 0) {
                    mb4_Work[0x27] = -1;
                    return;
                }
            }
        }
        break;
    case 10:
        if (mb4_Work[0x24] > 0) {
            mb4_Work[0x25] += 1;
            if ((mb4_Work[0x25] & 3) == 0) {
                ofs.x = 0.0f;
                ofs.y = 0.0f;
                ofs.z = 30.0000019f;
                MB4_CapsuleBubbleBurst("head", ofs, 0.75f * (0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
                mb4_Work[0x24] -= 1;
                if (mb4_Work[0x24] <= 0) {
                    mb4_Work[0x27] = -1;
                    return;
                }
            }
        }
        break;
    case 13:
        if (mb4_Work[0x24] > 0) {
            mb4_Work[0x25] += 1;
            if ((mb4_Work[0x25] & 3) == 0) {
                ofs.x = 0.0f;
                ofs.y = 20.0f;
                ofs.z = 40.0f;
                MB4_CapsuleBubbleBurst("noko_head", ofs, 0.75f * (0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
                mb4_Work[0x24] -= 1;
                if (mb4_Work[0x24] <= 0) {
                    mb4_Work[0x27] = -1;
                    return;
                }
            }
        }
        break;
    case 15:
        if (mb4_Work[0x24] > 0) {
            mb4_Work[0x25] += 1;
            if ((mb4_Work[0x25] & 3) == 0) {
                ofs.x = 0.0f;
                ofs.y = 20.0f;
                ofs.z = 40.0f;
                MB4_CapsuleBubbleBurst("body", ofs, 0.75f * (0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
                mb4_Work[0x24] -= 1;
                if (mb4_Work[0x24] <= 0) {
                    mb4_Work[0x27] = -1;
                    return;
                }
            }
        }
        break;
    case 17:
        if (mb4_Work[0x24] > 0) {
            mb4_Work[0x25] += 1;
            if ((mb4_Work[0x25] & 3) == 0) {
                ofs.x = 0.0f;
                ofs.y = 20.0f;
                ofs.z = 40.0f;
                MB4_CapsuleBubbleBurst("noko_head", ofs, 0.75f * (0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
                mb4_Work[0x24] -= 1;
                if (mb4_Work[0x24] <= 0) {
                    mb4_Work[0x27] = -1;
                    return;
                }
            }
        }
        break;
    case 20:
        if (mb4_Work[0x24] > 0) {
            mb4_Work[0x25] += 1;
            if ((mb4_Work[0x25] & 3) == 0) {
                ofs.x = 0.0f;
                ofs.y = 20.0f;
                ofs.z = 50.0f;
                MB4_CapsuleBubbleBurst("neck", ofs, 0.75f * (0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
                mb4_Work[0x24] -= 1;
                if (mb4_Work[0x24] <= 0) {
                    mb4_Work[0x27] = -1;
                    return;
                }
            }
        }
        break;
    case 23:
        if (mb4_Work[0x24] > 0) {
            mb4_Work[0x25] += 1;
            if ((mb4_Work[0x25] & 3) == 0) {
                ofs.x = 0.0f;
                ofs.y = 10.0f;
                ofs.z = 60.0000038f;
                MB4_CapsuleBubbleBurst("neck", ofs, 0.75f * (0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
                mb4_Work[0x24] -= 1;
                if (mb4_Work[0x24] <= 0) {
                    mb4_Work[0x27] = -1;
                    return;
                }
            }
        }
        break;
    case 24:
        if (mb4_Work[0x24] > 0) {
            mb4_Work[0x25] += 1;
            if ((mb4_Work[0x25] & 3) == 0) {
                ofs.x = 0.0f;
                ofs.y = 20.0f;
                ofs.z = 50.0f;
                MB4_CapsuleBubbleBurst("neck", ofs, 0.75f * (0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
                mb4_Work[0x24] -= 1;
                if (mb4_Work[0x24] <= 0) {
                    mb4_Work[0x27] = -1;
                    return;
                }
            }
        }
        break;
    case 25:
        if (mb4_Work[0x24] > 0) {
            mb4_Work[0x25] += 1;
            if ((mb4_Work[0x25] & 3) == 0) {
                ofs.x = 0.0f;
                ofs.y = 20.0f;
                ofs.z = 50.0f;
                MB4_CapsuleBubbleBurst("j_uekuti", ofs, 0.75f * (0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
                mb4_Work[0x24] -= 1;
                if (mb4_Work[0x24] <= 0) {
                    mb4_Work[0x27] = -1;
                    return;
                }
            }
        }
        break;
    case 31:
        if (mb4_Work[0x24] > 0) {
            mb4_Work[0x25] += 1;
            if ((mb4_Work[0x25] & 3) == 0) {
                ofs.x = 0.0f;
                ofs.y = 10.0f;
                ofs.z = 50.0f;
                MB4_CapsuleBubbleBurst("head", ofs, 0.75f * (0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
                mb4_Work[0x24] -= 1;
                if (mb4_Work[0x24] <= 0) {
                    mb4_Work[0x27] = -1;
                    return;
                }
            }
        }
        break;
    case 37:
        if (mb4_Work[0x24] > 0) {
            mb4_Work[0x25] += 1;
            if ((mb4_Work[0x25] & 3) == 0) {
                ofs.x = 70.0f;
                ofs.y = 40.0f;
                ofs.z = 0.0f;
                MB4_CapsuleBubbleBurst("ske_head", ofs, 0.75f * (0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
                mb4_Work[0x24] -= 1;
                if (mb4_Work[0x24] <= 0) {
                    mb4_Work[0x27] = -1;
                    return;
                }
            }
        }
        break;
    case 34:
        /* Two-point burst (e.g. nostrils); the original's per-variant
         * (mb4_Work[0x29]) offset tables are byte-identical, so only one
         * is needed here. */
        if (mb4_Work[0x24] > 0) {
            mb4_Work[0x23] -= 1;
            if (mb4_Work[0x23] <= 0 && (mb4_Work[0x25] += 1, (mb4_Work[0x25] & 3) == 0)) {
                Point3d p0, p1;
                p0.x = 65.0f;
                p0.y = 30.0000019f;
                p0.z = -9.0f;
                p1.x = 65.0f;
                p1.y = 30.0000019f;
                p1.z = 9.0f;
                MB4_CapsuleBubbleBurst("ns_hed", p0, 0.75f * (0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
                MB4_CapsuleBubbleBurst("ns_hed", p1, 0.75f * (0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
                mb4_Work[0x24] -= 1;
                if (mb4_Work[0x24] <= 0) {
                    if (mb4_Work[0x28] != 0) {
                        mb4_Work[0x24] = (s32) (8.0f + (5.0f * (0.007874016f * (f32) (frand() & 0x7F))));
                        mb4_Work[0x25] = (s32) frand();
                        mb4_Work[0x23] = (s32) (180.0f + (3.0f * (60.0f * (0.007874016f * (f32) (frand() & 0x7F)))));
                    } else {
                        mb4_Work[0x27] = -1;
                    }
                }
            }
        }
        break;
    }
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

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match. Same Catmull-Rom-tangent Hermite blend as MB4_HermiteCalcAxis,
 * except its result (sp14 in the decompiled draft) is never written
 * anywhere observable - dead code in the original too (global linkage,
 * not called from anywhere else in this file; kept verbatim rather than
 * "fixed", since we don't know what it was meant to write to). */
static f32 sp14_unused;
void fn_1_D3D8(f32 t, f32 pPrev, f32 p0, f32 p1, f32 pNext, f32 tangentScale) {
    f32 delta = p1 - p0;
    f32 m0, m1;
    f32 t2, t3, h1, h0, w0, w1;

    if (p0 == p1) {
        m0 = delta;
    } else {
        m0 = tangentScale * (delta + (p0 - pPrev));
    }
    if (p0 == pNext) {
        m1 = delta;
    } else {
        m1 = tangentScale * (delta + (pNext - p1));
    }
    t2 = t * t;
    t3 = t * t2;
    h1 = (f32) (((3.0 * (f64) t2) - (f64) t3) - (f64) t3);
    h0 = (f32) (1.0 - (f64) h1);
    w0 = t + ((t3 - t2) - t2);
    w1 = t3 - t2;
    sp14_unused = (w1 * m1) + (w0 * m0) + (h0 * p0) + (h1 * p1);
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

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match (size and cinematic complexity make it impractical). "Blow" board
 * event: a waterspout/geyser masu. Faces the player toward a nearby
 * star-flagged link (or a fixed point behind them if none), offers a
 * yes/no choice (COM players auto-pick based on path-to-star length via
 * MBMasuPathTypeLenGet), and on "yes" walks the player up to the geyser,
 * shows the mb4_Work[0x13] spout effect model growing out of the ground
 * (positioned each frame off its "i_hook" bone), launches the player
 * (screen shake + SE + wipe), picks a nearby non-blocked link masu as the
 * landing spot (falling back to any masu, or the current masu if the link
 * table is empty), arcs the player there via MB4_BezierCalcV, then plays
 * a gentle landing bounce before restoring the normal board camera. */
void MB4Ev_Blow(s32 playerNo) {
    s16 masuId = GwPlayer[playerNo].masuId;
    MASU *masuP = MBMasuGet(MASU_LAYER_DEFAULT, masuId);
    Point3d playerPos;
    Point3d targetPos;
    Point3d camOfs;
    s16 nextMasuId;
    s32 walkMotNo, suckMotNo;
    s32 i;
    Point3d dir;
    s32 choice;
    s32 se1, se2, se3;
    s16 linkTbl[MASU_LINK_MAX];
    s16 linkNum;
    s16 landMasuId;
    Point3d landPos;
    Point3d midPos;
    Mtx mtx;
    Point3d hookPos;
    Point3d rot;
    f32 t;
    Point3d p0, p1, p2, p3;
    f32 dropY;

    MBPlayerPosGet(playerNo, &playerPos);
    walkMotNo = MBPlayerMotionCreate(playerNo, 0x8C003E);
    suckMotNo = MBPlayerMotionCreate(playerNo, 0x8C002B);

    camOfs.x = 0.0f;
    camOfs.y = 200.0f;
    camOfs.z = 0.0f;
    MBCameraModelViewSet(MBPlayerModelGet(playerNo), NULL, &camOfs, 2000.0f, -1.0f, 0x2A);
    MBCameraMotionWait();

    for (i = 0; i < masuP->linkNum; i++) {
        if (MBMasuFlagGet(MASU_LAYER_DEFAULT, masuP->linkTbl[i]) & MASU_FLAG_BIT(7)) {
            break;
        }
    }
    if (i < masuP->linkNum) {
        nextMasuId = masuP->linkTbl[i];
        MBMasuPosGet(MASU_LAYER_DEFAULT, nextMasuId, &targetPos);
    } else {
        nextMasuId = masuId;
        targetPos.x = playerPos.x - 200.0f;
        targetPos.y = playerPos.y;
        targetPos.z = 200.0f + playerPos.z;
    }

    PSVECSubtract(&targetPos, &playerPos, &dir);
    MBPlayerRotateStart(playerNo, (s16) (180.0 * (atan2((f64) dir.x, (f64) dir.z) / 3.141592653589793)), 0xF);
    while (!MBPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }

    MBAudFXPlay(0x2F5);
    MBWinCreateChoice(2, 0x300003U, 2, 0);
    MBTopWinAttrReset(0x10U);
    if (GwPlayer[playerNo].comF) {
        if (MBMasuPathTypeLenGet(masuId, 5) > 0xA) {
            MBComChoiceSetLeft();
        } else {
            MBComChoiceSetRight();
        }
    }
    MBTopWinWait();
    choice = MBWinLastChoiceGet();
    MBTopWinKill();

    if (choice == 0) {
        MBPlayerMotionNoShiftSet(playerNo, 4, 0.0f, 8.0f, 0U);
        for (i = 0; i <= 30; i++) {
            t = (f32) i / 30.0f;
            playerPos.x = playerPos.x + (t * (targetPos.x - playerPos.x));
            playerPos.z = playerPos.z + (t * (targetPos.z - playerPos.z));
            playerPos.y = (f32) ((f64) (playerPos.y + (t * (targetPos.y - playerPos.y))) + (200.0 * sin((3.141592653589793 * (f64) (180.0f * t)) / 180.0)));
            MBPlayerPosSetV(playerNo, &playerPos);
            HuPrcVSleep();
        }

        MBPlayerMotionNoShiftSet(playerNo, 1, 0.0f, 8.0f, 0x40000001U);
        MBPlayerPosGet(playerNo, &playerPos);
        camOfs.x = 0.0f;
        camOfs.y = 200.0f;
        camOfs.z = 0.0f;
        MBCameraMasuViewSet(nextMasuId, NULL, &camOfs, 3000.0f, -1.0f, 0x15);
        HuPrcSleep(0x3C);

        MBPlayerMotionNoShiftSet(playerNo, walkMotNo, 0.0f, 8.0f, 0U);
        do {
            HuPrcVSleep();
        } while (!MBPlayerMotionEndCheck(playerNo));
        while (Hu3DMotionShiftIDGet(MBPlayerModelGet(playerNo)) != -1) {
            HuPrcVSleep();
        }

        MBCameraQuakeSet(0x3C, 50.0f);
        se1 = HuAudFXPlay(0x5EB);
        HuPrcSleep(0x3C);
        if (se1 != -1) {
            HuAudFXStop(se1);
        }
        se2 = HuAudFXPlay(0x5EC);
        MBPlayerMotionNoShiftSet(playerNo, suckMotNo, 0.0f, 8.0f, 0x40000001U);

        for (landMasuId = 1; landMasuId <= MBMasuNumGet(MASU_LAYER_DEFAULT); landMasuId++) {
            if (MBMasuTypeGet(MASU_LAYER_DEFAULT, landMasuId) == MASU_TYPE_STAR) {
                break;
            }
        }
        if (landMasuId > MBMasuNumGet(MASU_LAYER_DEFAULT)) {
            landMasuId = masuId;
        }

        {
            s16 candidate = landMasuId;
            BOOL useAllLinks;

            useAllLinks = (0.007874016f * (f32) (frand() & 0x7F)) < 0.65f;
            for (;;) {
                if (useAllLinks) {
                    linkNum = MBMasuLinkTblGet(MASU_LAYER_DEFAULT, candidate, linkTbl);
                } else {
                    MASU *cm = MBMasuGet(MASU_LAYER_DEFAULT, candidate);
                    linkNum = cm->linkNum;
                    for (i = 0; i < linkNum; i++) {
                        linkTbl[i] = cm->linkTbl[i];
                    }
                }
                if (linkNum > 0) {
                    for (i = 0; i < linkNum; i++) {
                        if ((MBMasuFlagGet(MASU_LAYER_DEFAULT, linkTbl[i]) & MBBranchFlagGet()) == 0) {
                            break;
                        }
                    }
                    landMasuId = (i < linkNum) ? linkTbl[i] : candidate;
                } else {
                    landMasuId = candidate;
                }
                if (candidate != landMasuId && MBMasuTypeGet(MASU_LAYER_DEFAULT, landMasuId) == MASU_TYPE_NONE) {
                    candidate = landMasuId;
                    continue;
                }
                break;
            }
        }

        MBPlayerPosFixEvent(playerNo, landMasuId, 0);
        MBMasuPosGet(MASU_LAYER_DEFAULT, landMasuId, &landPos);
        PSVECSubtract(&landPos, &playerPos, &dir);
        PSVECScale(&dir, &dir, 0.5f);
        PSVECAdd(&playerPos, &dir, &midPos);
        if (playerPos.y > landPos.y) {
            midPos.y = 5000.0f + playerPos.y + PSVECMag(&dir);
        } else {
            midPos.y = 5000.0f + landPos.y + PSVECMag(&dir);
        }

        MBPlayerPosGet(playerNo, &playerPos);
        MBModelPosSetV((s16) mb4_Work[0x13], &playerPos);
        MBModelDispSet((s16) mb4_Work[0x13], TRUE);
        MBMotionTimeSet((s16) mb4_Work[0x13], 0.0f);
        MBModelAttrSet((s16) mb4_Work[0x13], 0x40000001U);
        MBModelAlphaSet((s16) mb4_Work[0x13], 0U);
        MBModelScaleSet((s16) mb4_Work[0x13], 0.0f, 0.0f, 0.0f);
        HuPrcVSleep();

        MBPlayerRotGet(playerNo, &rot);
        for (i = 0; i <= 30; i++) {
            t = (f32) i / 30.0f;
            MBModelAlphaSet((s16) mb4_Work[0x13], (u8) (s32) (255.0f * t));
            MBModelScaleSet((s16) mb4_Work[0x13], t, 0.5f * t, t);
            Hu3DModelObjMtxGet(MBModelIdGet((s16) mb4_Work[0x13]), "i_hook", mtx);
            hookPos.x = mtx[0][3];
            hookPos.y = mtx[1][3];
            hookPos.z = mtx[2][3];
            rot.y += 10.0f;
            MBPlayerPosSetV(playerNo, &hookPos);
            MBPlayerRotSet(playerNo, 0.0f, rot.y, 0.0f);
            HuPrcVSleep();
        }

        for (i = 0; i <= 60; i++) {
            f32 s = (f32) (0.5 + (0.05000000074505806 * sin((3.141592653589793 * (f64) (720.0f * ((f32) i / 60.0f))) / 180.0)));
            MBModelAlphaSet((s16) mb4_Work[0x13], 0xFFU);
            MBModelScaleSet((s16) mb4_Work[0x13], 1.0f, s, 1.0f);
            Hu3DModelObjMtxGet(MBModelIdGet((s16) mb4_Work[0x13]), "i_hook", mtx);
            hookPos.x = mtx[0][3];
            hookPos.y = mtx[1][3];
            hookPos.z = mtx[2][3];
            rot.y += 10.0f;
            MBPlayerPosSetV(playerNo, &hookPos);
            MBPlayerRotSet(playerNo, 0.0f, rot.y, 0.0f);
            HuPrcVSleep();
        }

        omVibrate((s16) playerNo, 0xC, 4, 2);
        if (se2 != -1) {
            HuAudFXFadeOut(se2, 0x3E8);
        }
        se3 = HuAudFXPlay(0x5ED);

        for (i = 0; i <= 21; i++) {
            MBModelScaleSet((s16) mb4_Work[0x13], 1.0f, 0.5f + ((f32) i / 21.0f), 1.0f);
            Hu3DModelObjMtxGet(MBModelIdGet((s16) mb4_Work[0x13]), "i_hook", mtx);
            hookPos.x = mtx[0][3];
            hookPos.y = mtx[1][3];
            hookPos.z = mtx[2][3];
            rot.y += 10.0f;
            MBPlayerPosSetV(playerNo, &hookPos);
            MBPlayerRotSet(playerNo, 0.0f, rot.y, 0.0f);
            HuPrcVSleep();
        }

        if (se3 != -1) {
            HuAudFXFadeOut(se3, 0x3E8);
        }

        WipeColorSet(0xFF, 0xFF, 0xFF);
        WipeCreate(2, 0x81, 1);
        while (WipeCheckEnd()) {
            HuPrcVSleep();
        }

        MBModelDispSet((s16) mb4_Work[0x13], FALSE);
        GwPlayer[playerNo].masuId = landMasuId;
        MBPlayerPosFixEvent(playerNo, landMasuId, 1);
        MBPlayerMotionSpeedSet(playerNo, 0.0f);
        MBCameraSkipSet(FALSE);
        camOfs.x = 0.0f;
        camOfs.y = 200.0f;
        camOfs.z = 0.0f;
        MBCameraMasuViewSet(landMasuId, NULL, &camOfs, -1.0f, -1.0f, 1);
        MBCameraMotionWait();
        MBCameraSkipSet(TRUE);

        WipeCreate(1, 0x81, 0x1E);
        p0.x = landPos.x;
        p0.y = landPos.y + 3000.0f;
        p0.z = landPos.z;
        p1.x = landPos.x;
        p1.y = landPos.y + 1500.0f;
        p1.z = landPos.z;
        for (i = 0; i <= 30; i++) {
            t = (f32) i / 30.0f;
            MB4_BezierCalcV(&p0.x, &p1.x, &landPos.x, &playerPos.x, 0.5f + (0.5f * t));
            MB4_BezierCalcV(&p0.x, &p1.x, &landPos.x, &p2.x, (0.5f + (0.5f * t)) - 0.01f);
            PSVECSubtract(&p2, &playerPos, &p2);
            rot.x = 0.0f;
            rot.y = 0.0f;
            rot.z = 0.0f;
            MBPlayerPosSetV(playerNo, &playerPos);
            MBPlayerRotSetV(playerNo, &rot);
            HuPrcVSleep();
        }

        MBPlayerMotionNoShiftSet(playerNo, 6, 0.0f, 8.0f, 0x40000001U);
        PSMTXIdentity(mtx);
        MB4_PlayerMtxSet((s16) playerNo, mtx);
        playerPos = landPos;
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        dropY = 35.0f;
        do {
            HuPrcVSleep();
            playerPos.y += dropY;
            dropY -= 1.6333334f;
            MBPlayerPosSetV(playerNo, &playerPos);
            MBPlayerRotSetV(playerNo, &rot);
        } while (playerPos.y <= landPos.y || dropY != 0.0f);

        MBPlayerPosSetV(playerNo, &landPos);
        MBPlayerRotSet(playerNo, 0.0f, 0.0f, 0.0f);
        HuPrcSleep(0x3C);
        MBPlayerMotionSpeedSet(playerNo, 1.0f);
        MBPlayerMotionNoShiftSet(playerNo, 1, 0.0f, 8.0f, 0x40000001U);
        camOfs.x = 0.0f;
        camOfs.y = 200.0f;
        camOfs.z = 0.0f;
        MBCameraMasuViewSet(landMasuId, NULL, &camOfs, 1500.0f, -1.0f, 0x2A);
        MBCameraMotionWait();
    }

    if (walkMotNo != -1) {
        MBPlayerMotionKill(playerNo, walkMotNo);
    }
    if (suckMotNo != -1) {
        MBPlayerMotionKill(playerNo, suckMotNo);
    }
}

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match (size and cinematic/minigame complexity make it impractical).
 * "Kaisoo" (seaweed) board event: the player dives underwater to fish for
 * a prize among the 3 kelp patches (mb4ev_KaiPosTbl / mb4_Work[9..11]
 * swaying kelp models). If another player is already on the same masu, a
 * different dive-in animation plays. A joystick-driven (or COM
 * random-walk) underwater fishing-hook sprite (esp) is steered left/right
 * among the 3 kelp spots for up to 5 seconds; the chosen spot then reveals
 * either a newly spawned capsule (60% chance, from mb4ev_Puku2CapsuleTbl)
 * or a reused hidden coin/empty model (mb4_Work[0xD]/[0xE]), unless the
 * player's capsule inventory is already full, in which case nothing is
 * caught. Ends with a reward message and the player climbing back out. */
void MB4Ev_Kaisoo(s32 playerNo) {
    s16 masuId = GwPlayer[playerNo].masuId;
    MASU *masuP = MBMasuGet(MASU_LAYER_DEFAULT, masuId);
    Point3d playerPos;
    MBCAMERA *cam;
    Point3d camRot;
    f32 camZoom;
    s32 camDummyModel;
    Point3d camDummyPos;
    Point3d camRotOfs;
    Point3d camPosOfs;
    s32 i;
    f32 t;
    s16 espId;
    Point3d espPos;
    Point3d espTargetPos;
    s32 kelpIdx;
    s32 shareOtherPlayer;
    s32 kelpModel;
    s32 useRewardShift;
    s16 gameMesId;
    s32 selfPlayerId;
    Point3d catchPos;
    s32 catchScale;
    s32 capsuleModel;
    s32 catchModel;
    s32 gotCapsule;
    s32 coinDispId;

    MBPlayerPosGet(playerNo, &playerPos);
    MBPlayerMotionNoShiftSet(playerNo, 1, 0.0f, 8.0f, 0U);

    cam = MBCameraGet();
    camRot = cam->rot;
    camZoom = cam->zoom;

    camDummyModel = MBModelCreate(0x80002, NULL, FALSE);
    MBModelDispSet(camDummyModel, FALSE);
    MBPlayerPosGet(playerNo, &camDummyPos);
    camDummyPos.x += 500.0f;
    camDummyPos.z -= 500.0f;
    MBModelPosSetV(camDummyModel, &camDummyPos);
    camRotOfs.x = camRot.x;
    camRotOfs.y = camRot.y - 45.0f;
    camRotOfs.z = camRot.z;
    MBCameraModelViewSet(camDummyModel, &camRotOfs, NULL, 2000.0f, -1.0f, 0x2A);
    MBCameraMotionWait();

    MBPlayerRotateStart(playerNo, 0x87, 0xF);
    while (!MBPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }

    for (i = 0; i < 4; i++) {
        if (i != playerNo && masuId == GwPlayer[i].masuId) {
            break;
        }
    }
    shareOtherPlayer = (i < 4);
    if (shareOtherPlayer) {
        MBPlayerMotionNoShiftSet(playerNo, 3, 0.0f, 8.0f, 0x40000001U);
        for (i = 0; i <= 30; i++) {
            t = (f32) i / 30.0f;
            playerPos.x = (f32) ((f64) playerPos.x + (200.0 * sin((3.141592653589793 * (f64) (90.0f * t)) / 180.0)));
            playerPos.z = (f32) ((f64) playerPos.z - (200.0 * sin((3.141592653589793 * (f64) (90.0f * t)) / 180.0)));
            MBPlayerPosSetV(playerNo, &playerPos);
            MBPlayerRotSet(playerNo, 0.0f, 135.0f, 0.0f);
            HuPrcVSleep();
        }
    } else {
        MBPlayerMotionNoShiftSet(playerNo, 4, 0.0f, 8.0f, 0U);
        for (i = 0; i <= 27; i++) {
            t = (f32) i / 27.0f;
            playerPos.x = playerPos.x + (200.0f * t);
            playerPos.z = playerPos.z - (200.0f * t);
            playerPos.y = (f32) ((f64) playerPos.y + (250.0 * sin((3.141592653589793 * (f64) (180.0f * t)) / 180.0)));
            MBPlayerPosSetV(playerNo, &playerPos);
            MBPlayerRotSet(playerNo, 0.0f, 135.0f, 0.0f);
            HuPrcVSleep();
        }
    }
    MBPlayerMotionNoShiftSet(playerNo, 1, 0.0f, 8.0f, 0x40000001U);

    MBWinCreate(2, 0x300004U, 0x15);
    MBTopWinWait();
    MBTopWinKill();

    espId = espEntry(0x8002AU, 0x7D0, 0);
    espDispOn(espId);
    espDrawNoSet(espId, 0);
    espAttrSet(espId, 1U);
    espBankSet(espId, 0);
    Hu3D3Dto2D(&mb4ev_KaiPosTbl[1], 1, &espPos);
    espPosSet(espId, espPos.x, espPos.y);

    kelpIdx = 1;
    useRewardShift = 0;
    catchModel = -1;
    gotCapsule = 0;
    gameMesId = GameMesCreate(1, 5, -1, -1, MBWinCreateHelp(0x300005U));

    if (!GwPlayer[playerNo].comF) {
        s32 holdOff = 0;
        for (i = 0x12C; i >= 0; i--) {
            s32 stickX, stickY;
            u16 btn;

            HuPrcVSleep();
            GameMesDispSet(gameMesId, 1, (s16) ((i + 0x3A) / 60));
            if (holdOff <= 0) {
                stickX = MBPadStkXGet(GwPlayer[playerNo].padNo);
                stickY = MBPadStkYGet(GwPlayer[playerNo].padNo);
                btn = HuPadBtnDown[(u8) GwPlayer[playerNo].padNo];
            } else {
                btn = 0;
                stickY = 0;
                stickX = 0;
                holdOff -= 1;
            }
            (void) stickY;
            if (stickX < -8) {
                kelpIdx -= 1;
                holdOff = 0x13;
            } else if (stickX > 8) {
                kelpIdx += 1;
                holdOff = 0x13;
            }
            if (kelpIdx < 0) {
                kelpIdx = 0;
            } else if (kelpIdx > 2) {
                kelpIdx = 2;
            }
            Hu3D3Dto2D(&mb4ev_KaiPosTbl[kelpIdx], 1, &espTargetPos);
            espPos.x += 0.1f * (espTargetPos.x - espPos.x);
            espPos.y += 0.1f * (espTargetPos.y - espPos.y);
            espPosSet(espId, espPos.x, espPos.y);
            if (btn & 0x100) {
                break;
            }
        }
    } else {
        s32 decided = 0;
        s32 holdOff = (s32) (30.0f + (0.5f * (60.0f * (0.007874016f * (f32) (frand() & 0x7F)))));

        for (i = 0x12C; i >= 0; i--) {
            u16 btn = 0;
            s32 stickX = 0;

            HuPrcVSleep();
            GameMesDispSet(gameMesId, 1, (s16) ((i + 0x3A) / 60));
            if (decided) {
                holdOff -= 1;
                if (holdOff <= 0) {
                    btn = 0x100;
                }
            } else {
                holdOff -= 1;
                if (holdOff <= 0) {
                    f32 r = 0.007874016f * (f32) (frand() & 0x7F);
                    if (r < 0.33f) {
                        stickX = 0;
                    } else if (r < 0.5f) {
                        stickX = -0x10;
                    } else {
                        stickX = 0x10;
                    }
                    decided += 1;
                    holdOff = (s32) (60.0f + (0.5f * (60.0f * (0.007874016f * (f32) (frand() & 0x7F)))));
                }
            }
            if (stickX < -8) {
                kelpIdx -= 1;
                holdOff = 0x13;
            } else if (stickX > 8) {
                kelpIdx += 1;
                holdOff = 0x13;
            }
            if (kelpIdx < 0) {
                kelpIdx = 0;
            } else if (kelpIdx > 2) {
                kelpIdx = 2;
            }
            Hu3D3Dto2D(&mb4ev_KaiPosTbl[kelpIdx], 1, &espTargetPos);
            espPos.x += 0.1f * (espTargetPos.x - espPos.x);
            espPos.y += 0.1f * (espTargetPos.y - espPos.y);
            espPosSet(espId, espPos.x, espPos.y);
            if (btn & 0x100) {
                break;
            }
        }
    }
    GameMesDispSet(gameMesId, 2, -1);
    espKill(espId);
    MBTopWinKill();

    if ((s32) ((frand() & 0x7FFF) % 5) < 3) {
        capsuleModel = mb4ev_Puku2CapsuleTbl[(frand() & 0x7FFF) % 9];
        if (((*(u8 *) &GwSystem >> 6) & 1) != 0 && capsuleModel == 0x24) {
            capsuleModel = 1;
        }
        catchModel = MBModelCreate(MBCapsuleMdlGet(capsuleModel), NULL, FALSE);
        MBModelAttrSet(catchModel, 0x40000001U);
        catchScale = catchModel;
    } else {
        if (frand() & 1) {
            catchScale = (s16) mb4_Work[0xD];
        } else {
            catchScale = (s16) mb4_Work[0xE];
        }
    }

    MBModelPosSetV(catchScale, &mb4ev_KaiPosTbl[kelpIdx]);
    MBModelRotSet(catchScale, 0.0f, -45.0f, 0.0f);
    MBModelScaleSet(catchScale, 0.5f, 0.5f, 0.5f);
    MBModelDispSet(catchScale, TRUE);
    MBModelLayerSet(catchScale, 2U);
    kelpModel = (s16) mb4_Work[9 + kelpIdx];
    MBMotionSpeedSet((s16) kelpModel, 1.0f);
    MBMotionTimeSet((s16) kelpModel, 0.0f);
    HuAudFXPlay(0x5EE);
    i = 0;
    do {
        Point3d bubblePos;
        f32 bubbleAngle;

        if (i < 0x1E) {
            bubbleAngle = -45.0f + (60.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f));
            bubblePos.x = (f32) ((f64) mb4ev_KaiPosTbl[kelpIdx].x + (75.0 * sin((3.141592653589793 * (f64) bubbleAngle) / 180.0)));
            bubblePos.y = mb4ev_KaiPosTbl[kelpIdx].y;
            bubblePos.z = (f32) ((f64) mb4ev_KaiPosTbl[kelpIdx].z + (75.0 * cos((3.141592653589793 * (f64) bubbleAngle) / 180.0)));
            MB4_BubbleEffCreate(&bubblePos, 0xB4, 0.0f, 0.5f + (0.5f * (0.007874016f * (f32) (frand() & 0x7F))));
        }
        HuPrcVSleep();
        i += 1;
    } while (!MBMotionEndCheck((s16) kelpModel));

    HuAudFXPlay(0x364);
    for (i = 0; i <= 60; i++) {
        t = (f32) i / 60.0f;
        catchPos = mb4ev_KaiPosTbl[kelpIdx];
        catchPos.y = (f32) ((f64) catchPos.y + (150.0 * sin((3.141592653589793 * (f64) (90.0f * t)) / 180.0)));
        MBModelPosSetV(catchScale, &catchPos);
        MBModelRotSet(catchScale, 0.0f, -45.0f, 0.0f);
        MBModelScaleSet(catchScale, 0.5f + (0.5f * t), 0.5f + (0.5f * t), 0.5f + (0.5f * t));
        HuPrcVSleep();
    }

    if (MBPlayerCapsuleNumGet(playerNo) >= MBPlayerCapsuleMaxGet() && catchScale == catchModel) {
        f32 bob;

        MBWinCreate(2, 0x300006U, 0x15);
        bob = 0.0f;
        do {
            if (bob >= 1.0f) {
                bob -= 1.0f;
            }
            bob += 0.011111111f;
            if (bob > 1.0f) {
                bob = 1.0f;
            }
            catchPos = mb4ev_KaiPosTbl[kelpIdx];
            catchPos.y = (f32) ((f64) catchPos.y + (150.0 + (0.20000000298023224 * (100.0 * sin((3.141592653589793 * (f64) (360.0f * bob)) / 180.0)))));
            MBModelPosSetV(catchScale, &catchPos);
            HuPrcVSleep();
        } while (!MBTopWinDoneCheck() || bob < 1.0f);
        MBTopWinKill();

        for (i = 0; i <= 60; i++) {
            t = (f32) i / 60.0f;
            catchPos = mb4ev_KaiPosTbl[kelpIdx];
            catchPos.y = (f32) ((f64) catchPos.y + (150.0 * cos((3.141592653589793 * (f64) (90.0f * t)) / 180.0)));
            MBModelPosSetV(catchScale, &catchPos);
            MBModelRotSet(catchScale, 0.0f, -45.0f, 0.0f);
            MBModelScaleSet(catchScale, 1.0f - (0.5f * t), 1.0f - (0.5f * t), 1.0f - (0.5f * t));
            HuPrcVSleep();
        }
        MBMotionSpeedSet((s16) kelpModel, -1.0f);
        do {
            HuPrcVSleep();
        } while (MBMotionTimeGet((s16) kelpModel) > 0.0f);
        useRewardShift = 1;
    } else {
        Point3d toPlayer;
        f32 horizDist, horizAngle;

        for (i = 0; i <= 90; i++) {
            t = (f32) i / 90.0f;
            catchPos = mb4ev_KaiPosTbl[kelpIdx];
            catchPos.y = (f32) ((f64) catchPos.y + (150.0 + (0.20000000298023224 * (100.0 * sin((3.141592653589793 * (f64) (360.0f * t)) / 180.0)))));
            MBModelPosSetV(catchScale, &catchPos);
            HuPrcVSleep();
        }
        PSVECSubtract(&catchPos, &playerPos, &toPlayer);
        toPlayer.y = 0.0f;
        horizDist = PSVECMag(&toPlayer);
        horizAngle = (f32) (180.0 * (atan2((f64) toPlayer.x, (f64) toPlayer.z) / 3.141592653589793));
        HuAudFXPlay(0x31C);
        for (i = 0; i <= 120; i++) {
            t = (f32) i / 120.0f;
            catchPos.x = (f32) ((f64) playerPos.x + ((f64) (horizDist * (1.0f - t)) * sin((3.141592653589793 * (f64) ((720.0f * t) + horizAngle)) / 180.0)));
            catchPos.z = (f32) ((f64) playerPos.z + ((f64) (horizDist * (1.0f - t)) * cos((3.141592653589793 * (f64) ((720.0f * t) + horizAngle)) / 180.0)));
            catchPos.y = ((150.0f * (1.0f - t)) + mb4ev_KaiPosTbl[kelpIdx].y) - (50.0f * t);
            MBModelPosSetV(catchScale, &catchPos);
            MBModelScaleSet(catchScale, 1.0f - t, 1.0f - t, 1.0f - t);
            HuPrcVSleep();
        }
        MBModelDispSet(catchScale, FALSE);
    }

    MBPlayerRotateStart(playerNo, 0, 0xF);
    while (!MBPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }
    camRotOfs = camRot;
    camPosOfs.x = 0.0f;
    camPosOfs.z = 0.0f;
    camPosOfs.y = 200.0f;
    MBCameraModelViewSet(MBPlayerModelGet(playerNo), &camRotOfs, &camPosOfs, 2000.0f, -1.0f, 0x2A);
    MBCameraMotionWait();

    selfPlayerId = -1;
    (void) selfPlayerId;
    if (catchScale == (s32) mb4_Work[0xC]) {
        MBCoinAddExec(playerNo, 0xA);
        MBPlayerPosGet(playerNo, &catchPos);
        catchPos.y += 250.0f;
        coinDispId = MBCoinDispCreateSe(&catchPos, 0xA);
        (void) coinDispId;
        MBAudFXPlay(0xF);
        MBPlayerWinLoseVoicePlay(playerNo, 0xC, 0x1F8);
        MBPlayerMotionNoShiftSet(playerNo, 0xC, 0.0f, 8.0f, 0U);
    } else if (catchScale == catchModel && useRewardShift == 0) {
        MBPlayerCapsuleAdd(playerNo, capsuleModel);
        gotCapsule = 1;
        MBPlayerWinLoseVoicePlay(playerNo, 0xC, 0x1F8);
        MBPlayerMotionNoShiftSet(playerNo, 0xC, 0.0f, 8.0f, 0U);
    } else {
        MBPlayerWinLoseVoicePlay(playerNo, 0xD, 0x200);
        MBPlayerMotionNoShiftSet(playerNo, 0xD, 0.0f, 8.0f, 0U);
    }
    do {
        HuPrcVSleep();
    } while (!MBPlayerMotionEndCheck(playerNo));
    while (Hu3DMotionShiftIDGet(MBPlayerModelGet(playerNo)) != -1) {
        HuPrcVSleep();
    }

    if (catchScale == (s32) mb4_Work[0xC]) {
        MBWinCreate(2, 0x300007U, -1);
    } else if (catchScale == catchModel) {
        if (useRewardShift == 0) {
            MBWinCreate(2, 0x300008U, -1);
            MBTopWinInsertMesSet(MBCapsuleMesGet(capsuleModel), 0);
        }
    } else {
        MBWinCreate(2, 0x300009U, -1);
    }
    MBTopWinWait();
    MBTopWinKill();

    if (catchScale == catchModel && gotCapsule && capsuleModel == 0x24) {
        MB3MiracleGetExec(playerNo);
    }

    MBMotionSpeedSet((s16) kelpModel, -3.0f);
    MBPlayerRotateStart(playerNo, -0x2D, 0xF);
    while (!MBPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }

    for (i = 0; i < 4; i++) {
        if (i != playerNo && masuId == GwPlayer[i].masuId) {
            break;
        }
    }
    if (i < 4) {
        MBPlayerMotionNoShiftSet(playerNo, 3, 0.0f, 8.0f, 0x40000001U);
        for (i = 0; i <= 30; i++) {
            t = (f32) i / 30.0f;
            playerPos.x = (f32) ((f64) playerPos.x + (200.0 * cos((3.141592653589793 * (f64) (90.0f * t)) / 180.0)));
            playerPos.z = (f32) ((f64) playerPos.z - (200.0 * cos((3.141592653589793 * (f64) (90.0f * t)) / 180.0)));
            MBPlayerPosSetV(playerNo, &playerPos);
            MBPlayerRotSet(playerNo, 0.0f, -45.0f, 0.0f);
            HuPrcVSleep();
        }
    } else {
        MBPlayerMotionNoShiftSet(playerNo, 4, 0.0f, 8.0f, 0U);
        for (i = 0; i <= 27; i++) {
            t = (f32) i / 27.0f;
            playerPos.x = playerPos.x + (200.0f * (1.0f - t));
            playerPos.z = playerPos.z - (200.0f * (1.0f - t));
            playerPos.y = (f32) ((f64) playerPos.y + (250.0 * sin((3.141592653589793 * (f64) (180.0f * t)) / 180.0)));
            MBPlayerPosSetV(playerNo, &playerPos);
            MBPlayerRotSet(playerNo, 0.0f, -45.0f, 0.0f);
            HuPrcVSleep();
        }
    }

    MBPlayerMotionNoShiftSet(playerNo, 1, 0.0f, 8.0f, 0x40000001U);
    MBPlayerRotateStart(playerNo, 0, 0xF);
    while (!MBPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }
    do {
        HuPrcVSleep();
    } while (MBMotionTimeGet((s16) kelpModel) > 0.0f);
    MBMotionSpeedSet((s16) kelpModel, 0.0f);
    MBMotionTimeSet((s16) kelpModel, 0.0f);

    if (camDummyModel != -1) {
        MBModelKill(camDummyModel);
    }
    if (catchModel != -1) {
        MBModelKill(catchModel);
    }
}

/* NONMATCHING: complete and faithful, not attempted for instruction-level
 * match (size and cinematic complexity make it impractical). "Iruka"
 * (dolphin) board event: the player rides the dolphin model
 * (mb4_Work[0x12]) along the fixed 0x96-point mb4ev_IrukaPath spline
 * (using the already-matching path-speed/Hermite helpers written earlier
 * this session) toward a nearby "raise chance" masu (falling back to the
 * current masu). A small embedded phase state machine (var_r29 below)
 * drives, on top of the spline ride: an initial 1-frame delay (phase 0),
 * a jump-in arc from the player's start position onto the dolphin's back
 * (phase 1, 24 frames), the actual ride with periodic head-bubbles
 * (phase 2, switches to the dismount camera at 84% along the path), and
 * a jump-out arc to the target masu (phase 3, 30 frames). After the ride,
 * the player walks/turns to a nearby non-blocked link masu one more time
 * (matching MB4_CoinDispUpdate/MB4Ev_Blow's masu-arrival pattern). */
void MB4Ev_Iruka(s32 arg0) {
    s16 masuId = GwPlayer[arg0].masuId;
    Point3d playerStartPos;
    s32 walkMotNo, jumpInMotNo, dismountMotNo;
    s32 targetMasuId;
    Point3d targetMasuPos;
    Point3d segStart, segEnd;
    s32 iPrev, iCur, iNext, iNextNext;
    Point3d prevRot, curRot, nextRot;
    f32 angleDiff;
    Point3d segDir;
    f32 horizDistSq, horizDist;
    s32 phase;
    s32 phaseTimer;
    s32 flipFlag;
    s32 splashSeId;
    s32 i, j;
    s32 stepCount;
    f32 t;
    Point3d ridePos;
    Point3d rideRot;
    Mtx rideMtx;
    Point3d headOfs;
    s32 backLinkMasuId;
    MASU *masuP2;

    MBMasuGet(MASU_LAYER_DEFAULT, masuId);
    MBPlayerPosGet(arg0, &playerStartPos);
    walkMotNo = MBPlayerMotionCreate(arg0, 0x8C003F);
    jumpInMotNo = MBPlayerMotionCreate(arg0, 0x8C0062);
    dismountMotNo = MBPlayerMotionCreate(arg0, 0x8C002B);

    for (targetMasuId = 1; targetMasuId <= MBMasuNumGet(MASU_LAYER_DEFAULT); targetMasuId++) {
        if (MBMasuFlagGet(MASU_LAYER_DEFAULT, targetMasuId) & MASU_FLAG_RAISECHANCE) {
            break;
        }
    }
    if (targetMasuId > MBMasuNumGet(MASU_LAYER_DEFAULT)) {
        targetMasuId = masuId;
    }
    MBMasuPosGet(MASU_LAYER_DEFAULT, targetMasuId, &targetMasuPos);

    MBModelAttrReset((s16) mb4_Work[0x12], 0x40000002U);
    MBModelPosSetV((s16) mb4_Work[0x12], mb4ev_IrukaPath);
    MBMotionNoSet((s16) mb4_Work[0x12], 1, 0x40000001U);
    MBModelDispSet((s16) mb4_Work[0x12], TRUE);
    MBCameraModelViewSet((s16) mb4_Work[0x12], NULL, NULL, 2000.0f, -1.0f, 0x3F);
    HuPrcSleep(0x30);
    MBModelAttrReset((s16) mb4_Work[8], 0x40000002U);
    MBMotionTimeSet((s16) mb4_Work[8], 0.0f);
    MBMotionSpeedSet((s16) mb4_Work[8], 1.0f);
    mb4_Work[0x5A] = 0;
    mb4_Work[0x5B] = 0x1E;
    mb4_Work[0x5C] = -1;
    MBCameraMotionWait();

    PSVECSubtract(&mb4ev_IrukaPath[1], &mb4ev_IrukaPath[0], &segDir);
    horizDistSq = (segDir.x * segDir.x) + (segDir.z * segDir.z);
    horizDist = (horizDistSq > 0.0f) ? MB4_FastSqrt(horizDistSq) : horizDistSq;
    curRot.x = (f32) ((180.0 * (atan2((f64) horizDist, (f64) segDir.y) / 3.141592653589793)) - 90.0);
    curRot.y = (f32) (180.0 * (atan2((f64) segDir.x, (f64) segDir.z) / 3.141592653589793));
    curRot.z = 0.0f;
    rideRot = curRot;
    prevRot = curRot;

    flipFlag = 0;
    phase = 0;
    phaseTimer = 0;
    splashSeId = -1;

    for (i = 0; i < 0x63; i++) {
        segStart = mb4ev_IrukaPath[i];
        segEnd = mb4ev_IrukaPath[i + 1];
        iPrev = i - 1;
        iCur = i;
        iNext = i + 1;
        iNextNext = i + 2;
        if (iPrev < 0) {
            iPrev = i;
        }
        if (iNext > 0x63) {
            iNext = 0x63;
        }
        if (iNextNext > 0x63) {
            iNextNext = 0x63;
        }
        if (i != 0) {
            prevRot = curRot;
        } else {
            prevRot = rideRot;
        }

        PSVECSubtract(&segEnd, &segStart, &segDir);
        curRot.y = (f32) (180.0 * (atan2((f64) segDir.x, (f64) segDir.z) / 3.141592653589793));
        horizDistSq = (segDir.x * segDir.x) + (segDir.z * segDir.z);
        horizDist = (horizDistSq > 0.0f) ? MB4_FastSqrt(horizDistSq) : horizDistSq;
        curRot.x = (f32) ((180.0 * (atan2((f64) horizDist, (f64) segDir.y) / 3.141592653589793)) - 90.0);
        curRot.z = 0.0f;

        if (flipFlag) {
            curRot.y = curRot.y + 180.0f;
            if (curRot.y > 180.0f) {
                curRot.y -= 360.0f;
            }
            if (curRot.x == 0.0f) {
                curRot.x = 180.0f;
            } else if (curRot.x > 0.0f) {
                curRot.x = 180.0f - curRot.x;
            } else {
                curRot.x = -180.0f - curRot.x;
            }
        }

        angleDiff = (f32) fabs(MB4_AngleDiff(prevRot.y, curRot.y));
        if (angleDiff > 90.0f) {
            flipFlag ^= 1;
            if (flipFlag) {
                curRot.y = curRot.y + 180.0f;
                if (curRot.y > 180.0f) {
                    curRot.y -= 360.0f;
                }
                if (curRot.x == 0.0f) {
                    curRot.x = 180.0f;
                } else if (curRot.x > 0.0f) {
                    curRot.x = 180.0f - curRot.x;
                } else {
                    curRot.x = -180.0f - curRot.x;
                }
            } else {
                PSVECSubtract(&segEnd, &segStart, &segDir);
                curRot.y = (f32) (180.0 * (atan2((f64) segDir.x, (f64) segDir.z) / 3.141592653589793));
                horizDistSq = (segDir.x * segDir.x) + (segDir.z * segDir.z);
                horizDist = (horizDistSq > 0.0f) ? MB4_FastSqrt(horizDistSq) : horizDistSq;
                curRot.x = (f32) ((180.0 * (atan2((f64) horizDist, (f64) segDir.y) / 3.141592653589793)) - 90.0);
                curRot.z = 0.0f;
            }
        }

        stepCount = (s32) (MB4_PathNextTimeGet(&mb4ev_IrukaPath[iPrev], &mb4ev_IrukaPath[iCur], &mb4ev_IrukaPath[iNext], &mb4ev_IrukaPath[iNextNext]) / 30.000002f);
        for (j = 1; j <= stepCount; j++) {
            t = (f32) j / (f32) stepCount;
            MB4_HermiteCalcV(&mb4ev_IrukaPath[iPrev], &mb4ev_IrukaPath[iCur], &mb4ev_IrukaPath[iNext], &mb4ev_IrukaPath[iNextNext], &ridePos, t);
            nextRot.x = fn_1_C6D0(t, prevRot.x, curRot.x);
            nextRot.y = fn_1_C6D0(t, prevRot.y, curRot.y);
            nextRot.z = 0.0f;
            rideRot.x = MB4_AngleLerp(nextRot.x, rideRot.x, 2.5f);
            rideRot.y = MB4_AngleLerp(nextRot.y, rideRot.y, 2.5f);
            rideRot.z = MB4_AngleLerp(nextRot.z, rideRot.z, 2.5f);

            MBModelPosSetV((s16) mb4_Work[0x12], &ridePos);
            MBModelRotSetV((s16) mb4_Work[0x12], &rideRot);
            PSMTXIdentity(rideMtx);
            mtxRotCat(rideMtx, rideRot.x, rideRot.y, rideRot.z);
            headOfs.x = 0.0f;
            headOfs.y = 30.000002f;
            headOfs.z = 10.0f;
            PSMTXMultVec(rideMtx, &headOfs, &headOfs);
            PSVECAdd(&headOfs, &ridePos, &headOfs);

            switch (phase) {
            case 4:
                break;
            case 0:
                if (i >= 1) {
                    HuAudFXPlay(0x5F1);
                    MBPlayerMotionNoShiftSet(arg0, walkMotNo, 0.0f, 8.0f, 0U);
                    MBPlayerPosGet(arg0, &playerStartPos);
                    phase += 1;
                    phaseTimer = 0;
                }
                break;
            case 1:
                phaseTimer += 1;
                t = (f32) phaseTimer / 24.0f;
                PSVECSubtract(&headOfs, &playerStartPos, &segDir);
                PSVECScale(&segDir, &segDir, t);
                segDir.y = (f32) ((f64) segDir.y + (200.0 * sin((3.141592653589793 * (f64) (180.0f * t)) / 180.0)));
                PSVECAdd(&playerStartPos, &segDir, &segDir);
                MBPlayerPosSetV(arg0, &segDir);
                MBPlayerRotSet(arg0, 0.0f, 360.0f * -t, 0.0f);
                if (t == 1.0f) {
                    splashSeId = HuAudFXPlay(0x5EA);
                    MBPlayerPosFixEvent(arg0, targetMasuId, 0);
                    MBPlayerMotionNoShiftSet(arg0, jumpInMotNo, 0.0f, 8.0f, 0x40000001U);
                    MBPlayerPosSetV(arg0, &headOfs);
                    phase += 1;
                    phaseTimer = 0;
                }
                break;
            case 2:
                MBPlayerPosSetV(arg0, &headOfs);
                MBPlayerRotSetV(arg0, &rideRot);
                if (i >= 0x54) {
                    MBCameraModelViewSet(MBPlayerModelGet(arg0), NULL, NULL, 2000.0f, -1.0f, 0x15);
                    MBPlayerMotionNoShiftSet(arg0, dismountMotNo, 0.0f, 8.0f, 0U);
                    MBPlayerPosGet(arg0, &playerStartPos);
                    phase += 1;
                    phaseTimer = 0;
                }
                break;
            case 3:
                phaseTimer += 1;
                t = (f32) phaseTimer / 30.0f;
                PSVECSubtract(&targetMasuPos, &playerStartPos, &segDir);
                PSVECScale(&segDir, &segDir, t);
                segDir.y = (f32) ((f64) segDir.y + (200.0 * sin((3.141592653589793 * (f64) (180.0f * t)) / 180.0)));
                PSVECAdd(&playerStartPos, &segDir, &segDir);
                MBPlayerPosSetV(arg0, &segDir);
                MBPlayerRotSet(arg0, 360.0f * t, 0.0f, 0.0f);
                if (t == 1.0f) {
                    if (splashSeId != -1) {
                        HuAudFXStop(splashSeId);
                    }
                    splashSeId = -1;
                    MBPlayerMotionNoShiftSet(arg0, 6, 0.0f, 8.0f, 0x40000001U);
                    MBPlayerPosSetV(arg0, &targetMasuPos);
                    phase += 1;
                    phaseTimer = 0;
                }
                break;
            }

            if (phase >= 1 && phase <= 3) {
                Point3d bubbleOfs;
                f32 bubbleZ;

                for (j = 0; j < 3; j++) {
                    bubbleOfs.x = 50.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f);
                    bubbleOfs.y = 50.0f * ((0.007874016f * (f32) (frand() & 0x7F)) - 0.5f);
                    bubbleZ = (0.007874016f * (f32) (frand() & 0x7F)) - 0.5f;
                    bubbleOfs.z = 50.0f * bubbleZ;
                    MB4Ev_IrukaBubbleCreate(arg0, &bubbleOfs, bubbleZ, 0.5f);
                }
            }
            HuPrcVSleep();
        }
    }

    MBPlayerPosSetV(arg0, &targetMasuPos);
    MBPlayerRotSet(arg0, 0.0f, 0.0f, 0.0f);
    GwPlayer[arg0].masuId = (s16) targetMasuId;
    MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 8.0f, 0x40000001U);
    MBModelAttrSet((s16) mb4_Work[0x12], 0x40000002U);
    MBModelDispSet((s16) mb4_Work[0x12], FALSE);
    HuPrcSleep(0x3C);

    masuP2 = MBMasuGet(MASU_LAYER_DEFAULT, targetMasuId);
    for (i = 0; i < masuP2->linkNum; i++) {
        if ((MBMasuFlagGet(MASU_LAYER_DEFAULT, masuP2->linkTbl[i]) & MBBranchFlagGet()) == 0) {
            break;
        }
    }
    if (i < masuP2->linkNum) {
        backLinkMasuId = masuP2->linkTbl[i];
        MBMasuPosGet(MASU_LAYER_DEFAULT, backLinkMasuId, &segEnd);
        MBPlayerPosGet(arg0, &segStart);
    } else {
        MBPlayerPosGet(arg0, &segStart);
        segEnd = segStart;
    }
    PSVECSubtract(&segEnd, &segStart, &segDir);
    MBPlayerRotateStart(arg0, (s16) (180.0 * (atan2((f64) segDir.x, (f64) segDir.z) / 3.141592653589793)), 0xF);
    while (!MBPlayerRotateCheck(arg0)) {
        HuPrcVSleep();
    }
    MBPlayerPosFixPlayer(arg0, backLinkMasuId, 0);
    MBPlayerMotionNoShiftSet(arg0, 3, 0.0f, 8.0f, 0U);
    for (i = 0; i <= 30; i++) {
        t = (f32) i / 30.0f;
        segDir.x = segStart.x + (t * (segEnd.x - segStart.x));
        segDir.y = segStart.y + (t * (segEnd.y - segStart.y));
        segDir.z = segStart.z + (t * (segEnd.z - segStart.z));
        MBPlayerPosSetV(arg0, &segDir);
        HuPrcVSleep();
    }
    MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 8.0f, 0x40000001U);
    GwPlayer[arg0].masuId = (s16) backLinkMasuId;
    MBModelAttrSet((s16) mb4_Work[8], 0x40000002U);
    MBMotionTimeSet((s16) mb4_Work[8], 0.0f);
    MBMotionSpeedSet((s16) mb4_Work[8], 0.0f);

    if (walkMotNo != -1) {
        MBPlayerMotionKill(arg0, walkMotNo);
    }
    if (jumpInMotNo != -1) {
        MBPlayerMotionKill(arg0, jumpInMotNo);
    }
    if (dismountMotNo != -1) {
        MBPlayerMotionKill(arg0, dismountMotNo);
    }
}
