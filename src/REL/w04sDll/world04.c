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
#include "game/gamework.h"

#include <math.h>

/*
 * w04sDll - Story Mode variant of the "Deep Bloober Sea" board (w04Dll).
 *
 * Same file layout and helpers as w04Dll (bubble/coin/sakana effects,
 * Bezier/Hermite path helpers), with the Story-mode differences:
 *  - only the dolphin (Iruka) event exists; it follows one of two
 *    100-point paths (mb4ev_Iruka1Path / mb4ev_Iruka2Path);
 *  - fewer decoration models, no MB4Ev_Kihon/Blow/Kaisoo events, no star
 *    path search and no map-view hook.
 */

typedef struct MB4_BubbleTimer_s {
    int timer; //0x00
    int count; //0x04
    int tick; //0x08
} MB4_BUBBLE_TIMER;

typedef struct MB4_CapsuleBubble_s {
    int no; //0x00
    int timer; //0x04
    int tick; //0x08
    int capsuleNo; //0x0C
    int mdlId; //0x10
    int endF; //0x14
} MB4_CAPSULE_BUBBLE;

typedef struct MB4_SakanaSpecies_s {
    Point3d pos; //0x00
    Point3d range; //0x0C
    int num; //0x18
    int effF; //0x1C
    int swimType; //0x20
} MB4_SAKANA_SPECIES;

typedef struct MB4_Sakana_s {
    int mdlId; //0x00
    int effNo; //0x04
    f32 unk08; //0x08
    int bobAngle; //0x0C
    int time; //0x10
    int wobbleAngle; //0x14
    int wobbleTimer; //0x18
    int bubbleNum; //0x1C
    int bubbleTimer; //0x20
    int targetTimer; //0x24
    int swimType; //0x28
    int dispF; //0x2C
    Point3d areaPos; //0x30
    Point3d areaRange; //0x3C
    Point3d target; //0x48
    Point3d pos; //0x54
    Point3d dir; //0x60
    Point3d rot; //0x6C
    Point3d targetRot; //0x78
    f32 speed; //0x84
    f32 scale; //0x88
    f32 unk8C; //0x8C
} MB4_SAKANA; // size 0x90

#define MB4_SAKANA_SPECIES_MAX 14
#define MB4_SAKANA_MAX 16

typedef struct MB4_Work_s {
    int mapMdlId; //0x00
    int mapMdlId2; //0x04
    int unk08[2]; //0x08
    int mdl10; //0x10
    int mdl14; //0x14
    int pukuMdlId; //0x18
    int pukuOpenMdlId; //0x1C
    int mdl20; //0x20
    int kaiMdlId[3]; //0x24
    int mdl30; //0x30
    int mdl34; //0x34
    int mdl38; //0x38
    int mdl3C; //0x3C
    int puku2MdlId; //0x40
    int puku2ShellMdlId; //0x44
    int sango6MdlId; //0x48
    int mdl4C; //0x4C
    int mdl50; //0x50
    int coinShadowMdlId; //0x54
    MB4_BUBBLE_TIMER playerBubble[4]; //0x58
    MB4_CAPSULE_BUBBLE capsuleBubble; //0x88
    int capsuleBubbleFlag; //0xA0
    MB4_CAPSULE_BUBBLE capsuleBubble2; //0xA4
    MB4_BUBBLE_TIMER emitBubble[13]; //0xBC
    MB4_BUBBLE_TIMER puku2Bubble; //0x158
    MB4_BUBBLE_TIMER irukaBubble; //0x164
    MB4_SAKANA sakana[MB4_SAKANA_SPECIES_MAX][MB4_SAKANA_MAX]; //0x170
    int sakanaEffMdlId; //0x7F70
    int sakanaHookMdlId; //0x7F74
} MB4_WORK; // size 0x7F78

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void ObjectSetup(void);

static void MB4_Create(void);
static void MB4_Kill(void);
static void MB4_MainObjUpdate(OMOBJ *obj);
static void MB4_LightSet(void);
static void MB4_LightReset(void);
static void MB4Ev_MasuHatena(int playerNo);
static int MB4Ev_MasuWalkPre(int playerNo);
static int MB4Ev_MasuWalkPost(int playerNo);
static void MB4Ev_Iruka(int playerNo);
static int MB4_MasuPathCheck(s16 id, u32 flag, s16 *linkTbl, BOOL endF);
static void MB4_CoinDispUpdate(int playerNo, int coinNum, int threshold);
static void MB4Ev_IrukaBubbleCreate(int playerNo, Point3d *ofs, f32 unused0, f32 unused1);
static void MB4Ev_PlayerBubbleCreate(int playerNo);
static void MB4_CapsuleEffHook(int capsuleNo, int type, int mdlId, BOOL flag1, BOOL flag2, BOOL flag3);
static void MB4_CapsuleBubbleUpdate(void);
void MB4_ModelStub(HU3DMODEL *a, HU3DMODEL *b, HU3DMODEL *c, HU3DMODEL *d);
f32 MB4_PathTimeGet(f32 time, Point3d *p0, Point3d *p1, Point3d *p2, Point3d *p3);

void MB4_CoinInit(void);
static void MB4_CoinUpdate(OMOBJ *obj);
void MB4_CoinClose(void);
void MB4_CoinEffInit(void);
static void MB4_CoinEffUpdate(OMOBJ *obj);
void MB4_CoinEffClose(void);
void MB4_BubbleEffInit(void);
static void MB4_BubbleEffUpdate(OMOBJ *obj);
void MB4_BubbleEffClose(void);
int MB4_BubbleEffCreate(Point3d *pos, int time, f32 unused, f32 scale);
void MB4_SakanaInit(void);
static void MB4_SakanaDraw(HU3DMODEL *modelP, Mtx *mtx);
void MB4_SakanaClose(void);
int MB4_SakanaCreate(s16 type);
void MB4_SakanaPosSet(int no, f32 x, f32 y, f32 z);
void MB4_SakanaOffsetSet(int no, f32 x, f32 y, f32 z);
void MB4_SakanaLenSet(int no, f32 len);


static Point3d mb4ev_Iruka1Path[100] = {
    { -4600.0f, 1300.0f, -900.0f },
    { -4573.034f, 1215.942f, -794.85f },
    { -4544.73f, 1132.205f, -689.8f },
    { -4513.416f, 1049.226f, -585.01f },
    { -4477.566f, 967.475f, -480.72f },
    { -4435.419f, 887.62f, -377.35f },
    { -4384.58f, 810.739f, -275.66f },
    { -4323.074f, 738.184f, -176.79f },
    { -4249.07f, 672.183f, -81.97f },
    { -4160.644f, 617.657f, 7.49f },
    { -4058.945f, 578.597f, 90.56f },
    { -3944.98f, 561.026f, 164.61f },
    { -3823.432f, 568.291f, 227.2f },
    { -3699.797f, 597.694f, 278.49f },
    { -3578.163f, 647.115f, 318.11f },
    { -3459.058f, 707.638f, 349.44f },
    { -3341.585f, 773.989f, 374.69f },
    { -3224.757f, 843.131f, 395.06f },
    { -3107.58f, 912.766f, 411.43f },
    { -2989.013f, 980.773f, 424.23f },
    { -2867.965f, 1044.797f, 433.81f },
    { -2743.589f, 1102.422f, 440.47f },
    { -2615.203f, 1150.621f, 444.48f },
    { -2483.086f, 1187.473f, 447.73f },
    { -2348.475f, 1213.249f, 454.57f },
    { -2213.311f, 1227.102f, 473.29f },
    { -2081.168f, 1232.415f, 509.11f },
    { -1957.498f, 1231.019f, 567.86f },
    { -1851.292f, 1225.498f, 654.06f },
    { -1763.937f, 1219.431f, 759.23f },
    { -1694.664f, 1214.386f, 877.27f },
    { -1640.016f, 1210.498f, 1002.92f },
    { -1596.901f, 1207.319f, 1133.08f },
    { -1563.512f, 1204.522f, 1266.16f },
    { -1540.919f, 1201.896f, 1401.55f },
    { -1528.823f, 1199.182f, 1538.2f },
    { -1526.122f, 1196.246f, 1675.26f },
    { -1533.445f, 1192.971f, 1812.16f },
    { -1552.651f, 1189.182f, 1948.05f },
    { -1589.148f, 1184.45f, 2080.26f },
    { -1641.263f, 1178.752f, 2206.74f },
    { -1710.92f, 1171.903f, 2324.09f },
    { -1798.882f, 1163.809f, 2428.28f },
    { -1903.649f, 1154.56f, 2516.1f },
    { -2022.11f, 1144.44f, 2584.75f },
    { -2149.236f, 1133.875f, 2635.01f },
    { -2280.944f, 1123.16f, 2670.97f },
    { -2415.632f, 1112.423f, 2693.74f },
    { -2552.093f, 1101.768f, 2703.95f },
    { -2688.82f, 1091.436f, 2697.05f },
    { -2824.026f, 1081.525f, 2676.92f },
    { -2956.389f, 1072.172f, 2643.12f },
    { -3083.968f, 1063.592f, 2594.07f },
    { -3203.715f, 1056.114f, 2527.62f },
    { -3309.018f, 1050.414f, 2439.71f },
    { -3393.361f, 1047.014f, 2331.75f },
    { -3455.068f, 1045.914f, 2209.62f },
    { -3494.793f, 1046.942f, 2078.71f },
    { -3514.608f, 1049.839f, 1943.33f },
    { -3516.697f, 1054.371f, 1806.49f },
    { -3502.782f, 1060.369f, 1670.29f },
    { -3473.965f, 1067.733f, 1536.37f },
    { -3429.652f, 1076.524f, 1406.73f },
    { -3369.243f, 1086.809f, 1283.95f },
    { -3293.864f, 1098.483f, 1170.1f },
    { -3202.549f, 1111.615f, 1069.02f },
    { -3095.122f, 1126.004f, 985.74f },
    { -2974.3f, 1140.465f, 923.35f },
    { -2844.74f, 1153.207f, 880.98f },
    { -2710.435f, 1162.641f, 855.45f },
    { -2573.958f, 1167.527f, 843.04f },
    { -2436.802f, 1166.807f, 840.4f },
    { -2299.857f, 1159.412f, 844.67f },
    { -2163.769f, 1144.101f, 853.37f },
    { -2029.225f, 1119.318f, 864.15f },
    { -1897.25f, 1083.095f, 874.53f },
    { -1769.587f, 1033.109f, 881.67f },
    { -1648.902f, 967.666f, 882.65f },
    { -1535.579f, 890.392f, 877.24f },
    { -1428.51f, 805.261f, 865.81f },
    { -1325.802f, 715.8f, 848.75f },
    { -1225.205f, 625.121f, 826.32f },
    { -1124.34f, 536.198f, 798.69f },
    { -1020.825f, 452.239f, 765.9f },
    { -912.448f, 377.062f, 728.06f },
    { -797.591f, 315.291f, 685.56f },
    { -675.965f, 271.969f, 639.4f },
    { -549.439f, 250.277f, 591.32f },
    { -421.237f, 247.636f, 542.77f },
    { -293.426f, 259.594f, 494.53f },
    { -166.838f, 282.362f, 446.87f },
    { -41.67f, 313.172f, 399.83f },
    { 82.119f, 350.086f, 353.37f },
    { 204.255f, 392.945f, 307.6f },
    { 325.221f, 439.427f, 262.32f },
    { 445.375f, 488.293f, 217.37f },
    { 564.767f, 539.297f, 172.73f },
    { 683.414f, 592.263f, 128.39f },
    { 801.814f, 645.861f, 84.15f },
    { 920.001f, 700.0f, 40.0f },
};

static Point3d mb4ev_Iruka2Path[100] = {
    { 30.0f, 1300.0f, -1720.0f },
    { -47.566f, 1238.304f, -1649.92f },
    { -125.145f, 1176.869f, -1579.63f },
    { -202.737f, 1115.75f, -1509.08f },
    { -280.367f, 1055.518f, -1437.81f },
    { -358.027f, 996.322f, -1365.71f },
    { -435.711f, 938.232f, -1292.75f },
    { -513.417f, 881.719f, -1218.58f },
    { -591.103f, 827.912f, -1142.41f },
    { -668.706f, 776.704f, -1064.41f },
    { -746.137f, 728.8f, -984.19f },
    { -823.236f, 685.211f, -901.23f },
    { -899.702f, 647.404f, -814.89f },
    { -974.847f, 618.029f, -724.2f },
    { -1047.915f, 598.881f, -629.22f },
    { -1118.603f, 591.128f, -530.88f },
    { -1186.982f, 595.538f, -430.72f },
    { -1253.854f, 610.659f, -330.6f },
    { -1320.317f, 634.523f, -231.91f },
    { -1387.563f, 664.981f, -135.59f },
    { -1456.694f, 699.925f, -42.16f },
    { -1528.565f, 737.493f, 48.12f },
    { -1603.721f, 776.158f, 135.23f },
    { -1682.624f, 814.692f, 219.03f },
    { -1765.479f, 852.021f, 299.49f },
    { -1850.618f, 888.18f, 378.07f },
    { -1937.204f, 923.24f, 455.57f },
    { -2024.679f, 957.266f, 532.54f },
    { -2112.543f, 989.893f, 609.67f },
    { -2200.01f, 1021.283f, 687.75f },
    { -2286.826f, 1051.634f, 766.95f },
    { -2372.656f, 1080.806f, 847.67f },
    { -2456.146f, 1108.152f, 931.42f },
    { -2537.599f, 1134.236f, 1017.5f },
    { -2616.829f, 1159.014f, 1106.04f },
    { -2691.322f, 1181.394f, 1199.22f },
    { -2761.465f, 1201.833f, 1296.05f },
    { -2827.299f, 1220.443f, 1396.22f },
    { -2886.777f, 1236.524f, 1500.8f },
    { -2938.752f, 1250.159f, 1609.57f },
    { -2984.883f, 1262.201f, 1721.06f },
    { -3024.678f, 1272.698f, 1835.21f },
    { -3054.541f, 1281.331f, 1952.54f },
    { -3075.056f, 1288.738f, 2071.83f },
    { -3085.615f, 1295.173f, 2192.37f },
    { -3084.213f, 1300.79f, 2313.38f },
    { -3068.217f, 1305.828f, 2433.4f },
    { -3034.502f, 1310.627f, 2549.82f },
    { -2979.893f, 1315.628f, 2658.11f },
    { -2903.412f, 1321.292f, 2752.13f },
    { -2808.775f, 1327.729f, 2827.56f },
    { -2702.295f, 1334.355f, 2884.86f },
    { -2588.762f, 1340.644f, 2926.45f },
    { -2471.083f, 1346.236f, 2954.49f },
    { -2351.045f, 1350.864f, 2970.46f },
    { -2229.864f, 1354.278f, 2975.18f },
    { -2108.797f, 1355.756f, 2966.56f },
    { -1989.408f, 1354.891f, 2945.38f },
    { -1873.189f, 1351.241f, 2911.53f },
    { -1762.601f, 1343.93f, 2863.08f },
    { -1661.895f, 1331.746f, 2797.57f },
    { -1577.712f, 1313.429f, 2713.26f },
    { -1516.148f, 1288.789f, 2612.46f },
    { -1477.864f, 1259.183f, 2501.71f },
    { -1459.951f, 1226.047f, 2386.67f },
    { -1458.94f, 1190.313f, 2270.88f },
    { -1472.018f, 1152.467f, 2156.41f },
    { -1497.278f, 1112.674f, 2044.6f },
    { -1534.048f, 1070.77f, 1936.77f },
    { -1581.927f, 1026.417f, 1834.46f },
    { -1640.686f, 979.221f, 1739.39f },
    { -1711.573f, 928.4f, 1655.1f },
    { -1797.968f, 872.935f, 1590.9f },
    { -1896.463f, 815.329f, 1550.87f },
    { -2002.128f, 758.524f, 1534.83f },
    { -2110.603f, 704.582f, 1539.08f },
    { -2219.414f, 654.16f, 1557.54f },
    { -2328.029f, 606.422f, 1583.09f },
    { -2436.758f, 560.742f, 1611.79f },
    { -2546.105f, 516.521f, 1640.44f },
    { -2656.529f, 473.412f, 1666.5f },
    { -2768.341f, 431.944f, 1689.06f },
    { -2881.765f, 393.235f, 1708.2f },
    { -2996.979f, 358.541f, 1724.05f },
    { -3114.077f, 329.244f, 1736.66f },
    { -3233.005f, 306.908f, 1745.99f },
    { -3353.539f, 294.068f, 1751.71f },
    { -3474.806f, 292.229f, 1753.9f },
    { -3595.535f, 303.276f, 1752.5f },
    { -3713.979f, 328.487f, 1747.56f },
    { -3828.605f, 367.124f, 1739.47f },
    { -3938.836f, 416.572f, 1728.86f },
    { -4044.712f, 474.442f, 1716.29f },
    { -4146.529f, 538.916f, 1702.17f },
    { -4244.685f, 608.609f, 1686.83f },
    { -4339.625f, 682.438f, 1670.51f },
    { -4431.524f, 759.848f, 1653.35f },
    { -4521.49f, 839.392f, 1635.69f },
    { -4610.758f, 919.682f, 1617.84f },
    { -4700.0f, 1000.0f, 1600.0f },
};

static Point3d mb4ev_KaiPosTbl[3] = {
    { 1784.0f, 190.0f, 1017.0f },
    { 1883.0f, 190.0f, 1116.0f },
    { 1983.0f, 190.0f, 1215.0f },
};

static Point3d mb4ev_Puku2Pos = { 2041.0f, 230.0f, 958.0f };

static MB4_SAKANA_SPECIES mb4ev_SakanaTbl[14] = {
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

static Point3d mb4ev_RBokPosTbl[1] = {
    { -3100.0f, -150.0f, 2450.0f },
};

static f32 mb4ev_RBokRotYTbl[1] = { 0.0f };

static Point3d mb4ev_YaziPosTbl[3] = {
    { -2240.0f, 1167.0f, -972.0f },
    { -3724.0f, 1010.0f, -1010.0f },
    { -2511.0f, 515.0f, -78.0f },
};

static f32 mb4ev_YaziRotTbl[3] = { 8.0f, -12.0f, 18.0f };

static Point3d mb4ev_YaziExtPosTbl[1] = {
    { -1303.0f, 7.0f, 545.0f },
};

static f32 mb4ev_YaziExtRotTbl[1] = { 35.0f };

static Point3d mb4ev_HasigoPosTbl[2] = {
    { -3856.0f, 480.0f, -32.0f },
    { -2610.0f, 0.0f, 560.0f },
};

static Point3d mb4ev_HasigoRotTbl[2] = {
    { -10.0f, -5.0f, 0.0f },
    { -5.0f, 30.0f, 0.0f },
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

static f32 mb4ev_Sango1ScaleTbl[3] = { 1.0f, 1.0f, 0.95f };

static Point3d mb4ev_Sango2PosTbl[2] = {
    { -609.0f, -84.0f, 174.0f },
    { -3963.0f, 397.0f, -123.0f },
};

static Point3d mb4ev_Sango2RotTbl[2] = {
    { -16.0f, 180.0f, 0.0f },
    { 34.0f, 9.0f, -14.0f },
};

static f32 mb4ev_Sango2ScaleTbl[2] = { 1.0f, 0.7f };

static Point3d mb4ev_Sango3PosTbl[4] = {
    { -2614.0f, -71.0f, 1954.0f },
    { -3877.0f, -56.0f, 1192.0f },
    { -2702.0f, 1002.0f, -1719.0f },
    { -1320.0f, 692.0f, -1024.0f },
};

static Point3d mb4ev_Sango3RotTbl[4] = {
    { 56.0f, -7.0f, -14.0f },
    { 59.0f, -47.0f, 12.0f },
    { 0.0f, 0.0f, 0.0f },
    { 87.0f, 0.0f, -158.0f },
};

static Point3d mb4ev_Sango3ScaleTbl[4] = {
    { 1.2f, 1.2f, 1.2f },
    { 1.22f, 1.34f, 1.22f },
    { 1.3f, 1.3f, 1.3f },
    { 1.15f, 1.15f, 1.15f },
};

static Point3d mb4ev_Sango4PosTbl[2] = {
    { -2660.0f, 83.0f, 1906.0f },
    { -2600.0f, 985.0f, -1485.0f },
};

static Point3d mb4ev_Sango4RotTbl[2] = {
    { 31.0f, -12.0f, -8.0f },
    { 9.0f, 23.0f, 0.0f },
};

static f32 mb4ev_Sango4ScaleTbl[2] = { 1.0f, 1.35f };

static Point3d mb4ev_Sango5PosTbl[2] = {
    { -1407.0f, 828.0f, -1003.0f },
    { -2140.0f, 1146.0f, -1255.0f },
};

static Point3d mb4ev_Sango5RotTbl[2] = {
    { 50.0f, -7.0f, 0.0f },
    { -76.0f, 246.0f, 67.0f },
};

static f32 mb4ev_Sango5ScaleTbl[2] = { 1.23f, 1.37f };

static Point3d mb4ev_LeftSango5PosTbl[2] = {
    { -3823.0f, 985.0f, -1068.0f },
    { -2380.0f, -109.0f, 2013.0f },
};

static Point3d mb4ev_LeftSango5RotTbl[2] = {
    { 16.0f, -6.0f, 0.0f },
    { 42.0f, 9.0f, -2.0f },
};

static f32 mb4ev_LeftSango5ScaleTbl[2] = { 1.17f, 1.51f };

static Point3d mb4ev_Hitode1PosTbl[3] = {
    { -1452.0f, -201.0f, 2038.0f },
    { -2525.0f, 287.0f, 356.0f },
    { -2327.0f, 298.0f, 252.0f },
};

static Point3d mb4ev_Hitode1RotTbl[3] = {
    { 0.0f, 0.0f, 0.0f },
    { 90.0f, 24.0f, 0.0f },
    { 90.0f, 31.0f, 0.0f },
};

static f32 mb4ev_Hitode1ScaleTbl[3] = { 1.0f, 0.57f, 0.34f };

static Point3d mb4ev_Hitode2PosTbl[3] = {
    { -3313.0f, 1497.0f, -929.0f },
    { -3429.0f, 1445.0f, -950.0f },
    { -3188.0f, 1445.0f, -906.0f },
};

static Point3d mb4ev_Hitode2RotTbl[3] = {
    { 90.0f, -10.0f, 0.0f },
    { 85.0f, -9.0f, 29.0f },
    { 85.0f, 9.0f, 151.0f },
};

static f32 mb4ev_Hitode2ScaleTbl[3] = { 1.0f, 0.65f, 0.65f };

static Point3d mb4ev_KaisooPosTbl[2] = {
    { -1570.0f, 455.0f, 57.0f },
    { -1075.0f, 1089.0f, -1404.0f },
};

static Point3d mb4ev_KaisooRotTbl[2] = {
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
};

static f32 mb4ev_KaisooScaleTbl[2] = { 1.0f, 1.0f };

static Point3d openingRot = { -33.0f, 0.0f, 0.0f };

static Point3d openingOfs = { -2172.0f, 240.0f, 1423.0f };

static f32 openingZoom = 7314.0f;

static Point3d mapViewRot = { -78.0f, 20.0f, 0.0f };

static Point3d mapViewPos = { -2189.0f, 95.0f, 812.0f };

static f32 mapViewZoom = 7511.0f;

static Point3d lbl_1_data_EE4[4] = {
    { -9.0f, 30.000002f, 65.0f },
    { 9.0f, 30.000002f, 65.0f },
    { -9.0f, 30.000002f, 65.0f },
    { 9.0f, 30.000002f, 65.0f },
};

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

static Point3d mb4ev_Puku2EmiterPos = { -2800.0f, 300.0f, 1340.0f };

static s32 mb4ev_IrukaRotYPathNo[2] = { 11, 12 };

static s32 mb4ev_IrukaRotXPathNo[2] = { 19, 16 };

static Point3d mb4ev_IrukaBubbleOfsTbl[13] = {
    { 0.0f, -10.0f, 40.0f },
    { 0.0f, -15.000001f, 35.0f },
    { 0.0f, 20.0f, 10.0f },
    { 0.0f, 50.0f, 30.000002f },
    { 0.0f, 45.0f, 20.0f },
    { 0.0f, 15.000001f, 10.0f },
    { 0.0f, 30.000002f, 30.000002f },
    { 0.0f, -10.0f, 25.0f },
    { 0.0f, 0.0f, 50.0f },
    { 0.0f, -20.0f, 65.0f },
    { 0.0f, -20.0f, 65.0f },
    { 0.0f, -20.0f, 65.0f },
    { 0.0f, -20.0f, 65.0f },
};

static f32 mb4ev_IrukaTPLvlScaleTbl[13] = { 1.1f, 1.0f, 0.8f, 1.1f, 1.2f, 0.7f, 0.9f, 0.8f, 0.7f, 1.2f, 1.1f, 1.1f, 1.3f };


static OMOBJ *mb4_MainObj;
static OMOBJ *mb4_CoinObj;
static OMOBJ *mb4_CoinEffObj;
static OMOBJ *mb4_BubbleEffObj;

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

void ObjectSetup(void)
{
    GWPartyFSet(FALSE);
    MBObjectSetup(3, MB4_Create, MB4_Kill);
}

static void MB4_Kill(void)
{
    if(mb4_MainObj != NULL) {
        mb4_MainObj = NULL;
    }
    MB4_CoinClose();
    MB4_CoinEffClose();
    MB4_BubbleEffClose();
    MB4_SakanaClose();
}

static void MB4_LightSet(void)
{
    HuVecF pos;
    HuVecF dir;
    GXColor color;
    int gLightId;

    Hu3DShadowCreate(45, 1000, 250000);
    Hu3DShadowTPLvlSet(0.3f);
    Hu3DBGColorSet(255, 255, 255);
    pos.x = -3700;
    pos.y = 12000;
    pos.z = 22000;
    dir.x = 3700;
    dir.y = -12000;
    dir.z = -22000;
    color.r = 255;
    color.g = 255;
    color.b = 255;
    gLightId = Hu3DGLightCreate(pos.x, pos.y, pos.z, dir.x, dir.y, dir.z, color.r, color.g, color.b);
    Hu3DGLightInfinitytSet(gLightId);
}

static void MB4_LightReset(void)
{
}

int MB4_MasuStub(void)
{
    return 0;
}

static int MB4Ev_MasuWalkPre(int playerNo)
{
    return 0;
}

static int MB4_MasuPathCheck(s16 id, u32 flag, s16 *linkTbl, BOOL endF)
{
    return 0;
}

void MB4_ModelStub(HU3DMODEL *a, HU3DMODEL *b, HU3DMODEL *c, HU3DMODEL *d)
{
}

void MB4_CoinClose(void)
{
    mb4_CoinObj = NULL;
}

void MB4_CoinEffClose(void)
{
    mb4_CoinEffObj = NULL;
}

void MB4_BubbleEffClose(void)
{
    mb4_BubbleEffObj = NULL;
}
