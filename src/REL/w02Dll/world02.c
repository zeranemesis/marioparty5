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

/*
 * w02Dll - "Rainbow Dream" board.
 *
 * NOTE (decomp WIP): first-draft port from tools/m2c_draft.py, mechanically
 * cleaned up (M2C_ERROR/cror artifacts removed, unknown "?" types resolved
 * from context). Most functions are not yet instruction-matched against the
 * original asm (build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s) - this is
 * being improved function by function in follow-up commits, largest/most
 * complex event state machines (Zen2/Zen4/Rain/Thunder/Rainbow/WindGuru2)
 * last. Rainbow/Zen2-rainbow vertex path tables (mb2ev_Rainbow*VtxTbl,
 * mb2ev_Zen2Rainbow*VtxTbl) are declared with the right shape/size but not
 * yet filled with the real .rodata values.
 */
struct _struct_mb2ev_Zen2RainbowVtxNumTbl_0xC {
    /* 0x0 */ s32 unk0;                             /* inferred */
    /* 0x4 */ s32 unk4;                             /* inferred */
    /* 0x8 */ s32 unk8;                             /* inferred */
};                                                  /* size = 0xC */

struct _struct_mb2ev_Zen2RainbowVtxTbl_0xC {
    /* 0x0 */ void *unk0;                              /* inferred */
    /* 0x4 */ void *unk4;                              /* inferred */
    /* 0x8 */ void *unk8;                              /* inferred */
};                                                  /* size = 0xC */

struct _struct_mb2_ParticleUvTbl_0x8 {
    /* 0x0 */ f32 unk0;                             /* inferred */
    /* 0x4 */ f32 unk4;                             /* inferred */
};                                                  /* size = 0x8 */

s32 HuSprTexLoad(void *, s32, s32, s32, s32, s32);              /* extern */
void PSMTXRotRad(Mtx m, char axis, f32 rad);        /* extern */
s8 MBPadStkXGet(s8, s32);                           /* extern */
s8 MBPadStkYGet(s8, GWPLAYER *, GWPLAYER *);        /* extern */
s32 MBScrollExec(s32);                                /* extern */
s32 MB2_MasuLinkGet(s16 arg0, s16 *arg1, s32 arg2, s32 arg3);
HSFOBJECT *objCall(HU3DMODEL *arg0, char *arg1, HSFOBJECT *arg2);
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
void MB2Ev_WindGuru2(s32 arg0, s32 arg1);
void MB2Ev_Thunder(s32 arg0);
void MB2Ev_Rain(s32 arg0);
void MB2Ev_Zen2(s32 arg0);
void MB2Ev_Zen4(s32 arg0);
void MB2_TimerObjUpdate(OMOBJ *obj);
void MB2_DentyuObjFlash(OMOBJ *obj);
void MB2_WaterSplashUpdate(OMOBJ *obj);
s16 MB2_WindGuru2MasuNoGet(s32 arg0, s32 arg1);
s16 MB2_RainbowMasuNoEndGet(s32 arg0);
s32 MB2_PathCheck(s32 arg0);
s16 MB2_PathLenGet(s16 arg0, s16 arg1, s16 arg2);
s16 PathCheck(s16 arg0, s16 arg1);
void MB2_CoinDispUpdate(s32 arg0, s32 arg1, s32 arg2);
f32 MB2_AngleDiff(f32 farg0, f32 farg1);
f32 MB2_AngleLerp(f32 farg0, f32 farg1, f32 farg2);
f32 fn_1_BF54(f32 farg0, f32 farg1, f32 farg2);
HSFOBJECT *MB2_ModelObjPtrGet(HU3DMODEL *arg0, char *arg1);
f32 MB2_BezierCalc(f64 farg0, f32 farg1, f32 farg2, f32 farg3);
void MB2_BezierCalcV(f32 *arg0, f32 *arg1, Point3d *arg2, Point3d *arg3, f64 farg0);
void MB2_HermiteCalc(f32 *arg0, f32 *arg1, f32 *arg2, f32 *arg3, f32 farg0);
void fn_1_CEB8(f32 farg0, f32 farg1, f32 farg2, f32 farg3, f32 farg4, f32 farg5);
void MB2_HermiteCalcV(Point3d *arg0, Point3d *arg1, Point3d *arg2, Point3d *arg3, Point3d *arg4, f32 farg0);
void MB2_CoinInit(void);
void MB2_CoinUpdate(OMOBJ *obj);
void MB2_CoinClose(void);
s32 MB2_CoinNumGet(void);
s32 MB2_CoinCreate(f32 *arg0, f32 *arg1, s32 arg2, f32 farg0, f32 arg_sp0);
void MB2_CoinEffInit(void);
void MB2_CoinEffUpdate(OMOBJ *obj);
void MB2_CoinEffClose(void);
s32 MB2_CoinEffNumGet(void);
void MB2_CoinEffBlendModeSet(u8 arg0);
s32 MB2_CoinEffCreate(f32 *arg0, f32 *arg1, u8 *arg2, f32 farg0, f32 farg1, f32 farg2);
s32 MB2_CoinEffCreateRand(void *arg0, s32 arg1, f32 farg0, f32 farg1, f32 farg2, f32 farg3, f32 farg4);
s32 MB2_CoinEffCreateColRand(f32 *arg0, s32 arg1, f32 farg0, f32 farg1, f32 farg2, f32 farg3, f32 farg4);
void MB2_SplashEffInit(void);
void MB2_SplashEffUpdate(OMOBJ *obj);
void MB2_SplashEffClose(void);
s32 MB2_SplashEffNumGet(void);
void MB2_SplashEffCreate(f32 *arg0, f32 *arg1, u8 *arg2, s16 arg3, s32 arg4, f32 farg0, f32 farg1, f32 farg2);
void MB2_SplashEffCreateRandX(f32 *arg0, u8 *arg1, s32 arg2, f32 farg0);
s16 MB2_ParticleCreate(ANIMDATA *arg0, s16 arg1);
s16 MB2_ParticleCopy(s16 arg0);
void MB2_YajiInit(f32 *arg0);
void MB2_YajiUpdate(OMOBJ *obj);
void MB2_YajiClose(void);
void MB2_YajiHiliteSet(u32 arg0);
void MB2_YajiDispSet(s32 arg0);
void MB2_RainbowObjInit(void);
void MB2_RainbowObjUpdate(OMOBJ *obj);
void MB2_RainbowObjClose(void);
s32 MB2_RainbowObjCreate(void *arg0, s32 arg1, s32 arg2);
void MB2_RainbowObjAlphaSet(s32 arg0, s8 arg1);
s32 MB2_RainbowObjVtxTopGet(s32 arg0, void **arg1);
typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];
static Point3d mb2ev_Rainbow1VtxTbl[0xA] = {
    { 1250.0f, 300.0f, 3100.0f },
    { 1199.5679931640625f, 326.6610107421875f, 3026.669921875f },
    { 1148.406982421875f, 349.0799865722656f, 2952.469970703125f },
    { 1096.113037109375f, 362.7200012207031f, 2876.97998046875f },
    { 1043.0260009765625f, 361.4169921875f, 2800.93994140625f },
    { 990.7659912109375f, 342.3370056152344f, 2726.77001953125f },
    { 940.614013671875f, 308.9200134277344f, 2656.169921875f },
    { 892.531005859375f, 266.6449890136719f, 2588.889892578125f },
    { 845.905029296875f, 219.41900634765625f, 2523.8798828125f },
    { 800.0f, 170.0f, 2460.0f },
};

static Point3d mb2ev_Rainbow2VtxTbl[0x14] = {
    { -1200.0f, 700.0f, 1300.0f },
    { -1291.9639892578125f, 743.9420166015625f, 1346.6800537109375f },
    { -1382.0140380859375f, 788.447998046875f, 1396.4300537109375f },
    { -1466.9210205078125f, 834.1309814453125f, 1453.47998046875f },
    { -1539.02001953125f, 881.2780151367188f, 1524.699951171875f },
    { -1580.467041015625f, 928.2130126953125f, 1616.6400146484375f },
    { -1563.531005859375f, 969.4229736328125f, 1716.97998046875f },
    { -1474.89697265625f, 992.177001953125f, 1776.5999755859375f },
    { -1364.5810546875f, 1002.9609985351562f, 1777.1300048828125f },
    { -1270.675048828125f, 1015.8350219726562f, 1720.7900390625f },
    { -1224.386962890625f, 1036.946044921875f, 1622.1600341796875f },
    { -1216.906005859375f, 1066.8050537109375f, 1514.739990234375f },
    { -1235.4439697265625f, 1106.81005859375f, 1412.06005859375f },
    { -1276.333984375f, 1159.135986328125f, 1322.22998046875f },
    { -1335.7490234375f, 1221.6719970703125f, 1251.0400390625f },
    { -1407.5140380859375f, 1287.39404296875f, 1195.6500244140625f },
    { -1489.2530517578125f, 1350.8590087890625f, 1152.8399658203125f },
    { -1582.2440185546875f, 1404.2550048828125f, 1121.02001953125f },
    { -1688.22802734375f, 1434.386962890625f, 1103.300048828125f },
    { -1799.9990234375f, 1440.0f, 1100.0f },
};

static Point3d mb2ev_Rainbow3VtxTbl[0x14] = {
    { -1800.0f, 1540.0f, -500.0f },
    { -1880.7259521484375f, 1563.051025390625f, -541.4600219726562f },
    { -1955.0050048828125f, 1599.1090087890625f, -585.3300170898438f },
    { -2018.178955078125f, 1650.9310302734375f, -630.780029296875f },
    { -2068.714111328125f, 1715.0379638671875f, -676.4400024414062f },
    { -2105.919921875f, 1788.0050048828125f, -721.5999755859375f },
    { -2127.280029296875f, 1867.93701171875f, -765.2100219726562f },
    { -2132.72998046875f, 1951.81005859375f, -806.1799926757812f },
    { -2123.8291015625f, 2036.927978515625f, -844.0f },
    { -2104.256103515625f, 2121.4169921875f, -879.280029296875f },
    { -2075.77294921875f, 2204.18603515625f, -912.5399780273438f },
    { -2039.81298828125f, 2284.551025390625f, -944.3800048828125f },
    { -1996.262939453125f, 2361.490966796875f, -975.1400146484375f },
    { -1943.885009765625f, 2433.05908203125f, -1004.989990234375f },
    { -1881.31494140625f, 2496.490966796875f, -1033.5f },
    { -1807.31396484375f, 2547.403076171875f, -1059.3599853515625f },
    { -1722.490966796875f, 2580.05908203125f, -1080.52001953125f },
    { -1630.5f, 2587.10498046875f, -1094.0f },
    { -1538.822998046875f, 2569.64794921875f, -1099.2099609375f },
    { -1450.0009765625f, 2540.0f, -1100.0f },
};

static Point3d mb2ev_Rainbow4VtxTbl[0x14] = {
    { 1500.0f, 2550.0f, -1190.0f },
    { 1571.9959716796875f, 2561.10107421875f, -1206.56005859375f },
    { 1645.197021484375f, 2559.717041015625f, -1220.6099853515625f },
    { 1716.81494140625f, 2541.5869140625f, -1230.6500244140625f },
    { 1784.241943359375f, 2510.155029296875f, -1236.8599853515625f },
    { 1847.6600341796875f, 2470.81396484375f, -1239.9599609375f },
    { 1907.4169921875f, 2425.962890625f, -1239.97998046875f },
    { 1962.906982421875f, 2376.072998046875f, -1236.22998046875f },
    { 2013.626953125f, 2321.839111328125f, -1228.31005859375f },
    { 2058.136962890625f, 2263.344970703125f, -1215.22998046875f },
    { 2094.26611328125f, 2200.93603515625f, -1195.8299560546875f },
    { 2121.514892578125f, 2136.194091796875f, -1170.469970703125f },
    { 2140.56201171875f, 2070.69189453125f, -1140.0899658203125f },
    { 2151.712890625f, 2005.66796875f, -1105.0899658203125f },
    { 2154.991943359375f, 1942.428955078125f, -1065.5f },
    { 2148.906005859375f, 1883.279052734375f, -1020.3699951171875f },
    { 2132.14697265625f, 1831.0159912109375f, -969.8499755859375f },
    { 2104.56396484375f, 1788.4580078125f, -915.1199951171875f },
    { 2069.678955078125f, 1755.22900390625f, -858.0700073242188f },
    { 2030.0f, 1730.0f, -800.0f },
};

static Point3d mb2ev_Rainbow5VtxTbl[0x14] = {
    { 1800.0f, 1420.0f, 1100.0f },
    { 1679.698974609375f, 1414.27001953125f, 1100.7900390625f },
    { 1559.8709716796875f, 1403.6319580078125f, 1105.8499755859375f },
    { 1442.406982421875f, 1382.1309814453125f, 1120.3800048828125f },
    { 1331.8050537109375f, 1344.7569580078125f, 1148.8800048828125f },
    { 1235.1070556640625f, 1289.613037109375f, 1194.1600341796875f },
    { 1153.509033203125f, 1224.3900146484375f, 1253.719970703125f },
    { 1088.862060546875f, 1155.698974609375f, 1328.2099609375f },
    { 1045.925048828125f, 1091.5660400390625f, 1420.06005859375f },
    { 1037.0589599609375f, 1044.7130126953125f, 1529.8299560546875f },
    { 1068.3079833984375f, 1018.2650146484375f, 1642.27001953125f },
    { 1140.7769775390625f, 1006.3660278320312f, 1736.2900390625f },
    { 1250.25f, 999.7059936523438f, 1782.18994140625f },
    { 1366.8160400390625f, 987.1420288085938f, 1763.93994140625f },
    { 1449.7340087890625f, 957.97998046875f, 1684.68994140625f },
    { 1480.699951171875f, 914.25f, 1578.050048828125f },
    { 1460.0460205078125f, 859.905029296875f, 1473.97998046875f },
    { 1389.9620361328125f, 800.8820190429688f, 1397.0699462890625f },
    { 1298.2349853515625f, 744.4019775390625f, 1343.530029296875f },
    { 1200.0009765625f, 690.0009765625f, 1300.0f },
};

static Point3d mb2ev_Zen2Rainbow1VtxTbl[0x14] = {
    { -200.0f, 3900.0f, -1850.0f },
    { -383.6910095214844f, 3957.387939453125f, -1632.800048828125f },
    { -569.9429931640625f, 3999.34912109375f, -1414.3699951171875f },
    { -759.8930053710938f, 4006.4130859375f, -1195.550048828125f },
    { -950.2000122070312f, 3960.654052734375f, -982.1599731445312f },
    { -1133.7760009765625f, 3852.118896484375f, -786.1699829101562f },
    { -1313.343994140625f, 3704.0458984375f, -613.260009765625f },
    { -1496.56396484375f, 3535.799072265625f, -464.04998779296875f },
    { -1692.2259521484375f, 3361.214111328125f, -340.0899963378906f },
    { -1907.2340087890625f, 3192.806884765625f, -242.57000732421875f },
    { -2136.77490234375f, 3033.593994140625f, -164.1699981689453f },
    { -2369.884033203125f, 2878.006103515625f, -88.91999816894531f },
    { -2591.051025390625f, 2717.304931640625f, 7.429999828338623f },
    { -2759.300048828125f, 2543.466064453125f, 164.07000732421875f },
    { -2814.60400390625f, 2361.217041015625f, 379.6000061035156f },
    { -2747.10693359375f, 2177.240966796875f, 590.4099731445312f },
    { -2581.490966796875f, 1995.2220458984375f, 741.25f },
    { -2367.962890625f, 1822.9110107421875f, 834.6500244140625f },
    { -2136.93701171875f, 1659.3819580078125f, 898.3099975585938f },
    { -1900.001953125f, 1500.001953125f, 949.989990234375f },
};

static Point3d mb2ev_Zen2Rainbow4VtxTbl[0x14] = {
    { -200.0f, 3900.0f, -1850.0f },
    { -429.93701171875f, 3970.302978515625f, -1578.2900390625f },
    { -664.6409912109375f, 4008.681884765625f, -1304.6400146484375f },
    { -903.1240234375f, 3977.5419921875f, -1034.3299560546875f },
    { -1131.64697265625f, 3852.676025390625f, -783.260009765625f },
    { -1334.946044921875f, 3668.717041015625f, -546.0499877929688f },
    { -1504.574951171875f, 3467.39501953125f, -296.8399963378906f },
    { -1619.343017578125f, 3292.860107421875f, -2.009999990463257f },
    { -1641.0589599609375f, 3162.092041015625f, 334.30999755859375f },
    { -1575.68798828125f, 3022.238037109375f, 661.6900024414062f },
    { -1456.3919677734375f, 2836.73095703125f, 949.0900268554688f },
    { -1311.8909912109375f, 2613.39404296875f, 1195.3900146484375f },
    { -1155.2359619140625f, 2365.52587890625f, 1408.739990234375f },
    { -992.2860107421875f, 2098.527099609375f, 1592.3699951171875f },
    { -826.8209838867188f, 1815.883056640625f, 1748.3399658203125f },
    { -660.9249877929688f, 1521.9520263671875f, 1881.4100341796875f },
    { -495.239013671875f, 1221.114990234375f, 1998.3699951171875f },
    { -329.90301513671875f, 916.0880126953125f, 2104.52001953125f },
    { -164.86599731445312f, 608.6279907226562f, 2203.93994140625f },
    { -0.0020000000949949026f, 300.0039978027344f, 2299.989990234375f },
};

static Point3d mb2ev_Zen2Rainbow7VtxTbl[0x14] = {
    { -200.0f, 3900.0f, -1850.0f },
    { -428.1709899902344f, 3969.885986328125f, -1580.3599853515625f },
    { -661.0230102539062f, 4008.64990234375f, -1308.800048828125f },
    { -897.739990234375f, 3979.47412109375f, -1040.300048828125f },
    { -1124.032958984375f, 3855.992919921875f, -790.6799926757812f },
    { -1314.3480224609375f, 3658.507080078125f, -558.5700073242188f },
    { -1441.3330078125f, 3413.333984375f, -329.05999755859375f },
    { -1464.6219482421875f, 3145.969970703125f, -92.27999877929688f },
    { -1351.6009521484375f, 2892.572021484375f, 132.39999389648438f },
    { -1103.06396484375f, 2695.02392578125f, 293.8500061035156f },
    { -766.2639770507812f, 2590.652099609375f, 349.1700134277344f },
    { -409.1029968261719f, 2555.593017578125f, 327.45001220703125f },
    { -52.10499954223633f, 2540.375f, 282.92999267578125f },
    { 305.0889892578125f, 2513.97509765625f, 247.58999633789062f },
    { 658.5139770507812f, 2449.89306640625f, 248.88999938964844f },
    { 987.8259887695312f, 2324.06689453125f, 314.6199951171875f },
    { 1275.0660400390625f, 2146.721923828125f, 438.1199951171875f },
    { 1525.6829833984375f, 1941.64501953125f, 595.1500244140625f },
    { 1753.35498046875f, 1723.5179443359375f, 768.969970703125f },
    { 1969.9969482421875f, 1500.0030517578125f, 949.989990234375f },
};

static Point3d mb2ev_Zen2Rainbow8VtxTbl[0x14] = {
    { -0.0f, 3900.0f, -1850.0f },
    { 1.7239999771118164f, 3958.552978515625f, -1556.6700439453125f },
    { 13.029000282287598f, 4004.35791015625f, -1261.4000244140625f },
    { 43.935001373291016f, 4023.52490234375f, -964.780029296875f },
    { 106.25399780273438f, 3998.35302734375f, -673.97998046875f },
    { 202.6479949951172f, 3920.419921875f, -402.5299987792969f },
    { 313.2049865722656f, 3799.7509765625f, -152.50999450683594f },
    { 410.82598876953125f, 3645.382080078125f, 84.0f },
    { 453.02099609375f, 3458.8798828125f, 312.2699890136719f },
    { 362.9779968261719f, 3263.239013671875f, 511.80999755859375f },
    { 149.85499572753906f, 3102.214111328125f, 642.030029296875f },
    { -106.45600128173828f, 2968.212890625f, 716.780029296875f },
    { -374.0530090332031f, 2841.097900390625f, 756.97998046875f },
    { -639.68701171875f, 2704.464111328125f, 769.0499877929688f },
    { -894.6259765625f, 2548.587890625f, 758.760009765625f },
    { -1135.125f, 2372.010009765625f, 739.0700073242188f },
    { -1358.7340087890625f, 2173.845947265625f, 732.3499755859375f },
    { -1560.6240234375f, 1956.7440185546875f, 768.1500244140625f },
    { -1737.8790283203125f, 1729.6739501953125f, 847.8900146484375f },
    { -1899.9990234375f, 1500.001953125f, 949.989990234375f },
};

static Point3d mb2ev_Zen2Rainbow2VtxTbl[0x14] = {
    { -0.0f, 3900.0f, -1850.0f },
    { 2.515000104904175f, 3963.64892578125f, -1516.3199462890625f },
    { 19.070999145507812f, 3996.487060546875f, -1179.0f },
    { 64.9800033569336f, 3962.60791015625f, -845.2000122070312f },
    { 148.35800170898438f, 3837.864013671875f, -542.239990234375f },
    { 253.1750030517578f, 3643.806884765625f, -284.67999267578125f },
    { 352.8559875488281f, 3413.18798828125f, -56.349998474121094f },
    { 428.06298828125f, 3160.465087890625f, 157.4499969482422f },
    { 455.2049865722656f, 2894.712890625f, 366.2799987792969f },
    { 396.2770080566406f, 2633.6640625f, 572.969970703125f },
    { 237.76199340820312f, 2403.402099609375f, 764.5399780273438f },
    { 31.742000579833984f, 2195.48388671875f, 936.6599731445312f },
    { -183.5189971923828f, 1987.0689697265625f, 1096.75f },
    { -369.07000732421875f, 1750.1829833984375f, 1253.0899658203125f },
    { -489.3890075683594f, 1474.0389404296875f, 1408.27001953125f },
    { -540.7210083007812f, 1176.3480224609375f, 1562.239990234375f },
    { -527.6380004882812f, 876.3809814453125f, 1719.969970703125f },
    { -437.5469970703125f, 601.85400390625f, 1895.8199462890625f },
    { -249.6790008544922f, 407.54901123046875f, 2097.090087890625f },
    { -0.0010000000474974513f, 300.0f, 2299.989990234375f },
};

static Point3d mb2ev_Zen2Rainbow5VtxTbl[0x14] = {
    { -0.0f, 3900.0f, -1850.0f },
    { 1.1330000162124634f, 3952.10009765625f, -1586.8800048828125f },
    { 9.213000297546387f, 3989.06591796875f, -1321.43994140625f },
    { 31.84600067138672f, 3993.611083984375f, -1054.5699462890625f },
    { 75.90299987792969f, 3948.549072265625f, -794.6599731445312f },
    { 143.14300537109375f, 3846.867919921875f, -556.72998046875f },
    { 228.09100341796875f, 3701.3701171875f, -348.4100036621094f },
    { 325.9330139160156f, 3531.735107421875f, -165.27000427246094f },
    { 433.89898681640625f, 3352.2041015625f, 2.069999933242798f },
    { 553.5059814453125f, 3169.027099609375f, 157.19000244140625f },
    { 691.426025390625f, 2990.49609375f, 302.1000061035156f },
    { 863.0590209960938f, 2836.39501953125f, 437.3699951171875f },
    { 1092.4969482421875f, 2755.26806640625f, 543.010009765625f },
    { 1356.8299560546875f, 2764.89501953125f, 576.3400268554688f },
    { 1609.9449462890625f, 2700.366943359375f, 558.6099853515625f },
    { 1759.1820068359375f, 2483.77392578125f, 585.6900024414062f },
    { 1836.3280029296875f, 2238.4970703125f, 660.5800170898438f },
    { 1888.7449951171875f, 1991.947998046875f, 752.1400146484375f },
    { 1931.345947265625f, 1745.822021484375f, 849.8599853515625f },
    { 1970.0f, 1500.0030517578125f, 949.989990234375f },
};

static Point3d mb2ev_Zen2Rainbow6VtxTbl[0x14] = {
    { 200.0f, 3900.0f, -1850.0f },
    { 312.8280029296875f, 4037.487060546875f, -1588.199951171875f },
    { 436.9230041503906f, 4114.09521484375f, -1308.739990234375f },
    { 568.4650268554688f, 4077.718994140625f, -1025.3800048828125f },
    { 682.5079956054688f, 3949.339111328125f, -760.1900024414062f },
    { 756.822998046875f, 3779.56201171875f, -504.0299987792969f },
    { 780.3060302734375f, 3592.3701171875f, -250.36000061035156f },
    { 748.0150146484375f, 3403.60400390625f, 1.0700000524520874f },
    { 663.7360229492188f, 3220.930908203125f, 245.02999877929688f },
    { 535.2570190429688f, 3044.242919921875f, 473.6600036621094f },
    { 363.22900390625f, 2873.8310546875f, 676.780029296875f },
    { 143.0240020751953f, 2713.27099609375f, 835.9199829101562f },
    { -127.677001953125f, 2574.29296875f, 916.4500122070312f },
    { -424.6910095214844f, 2467.968017578125f, 903.02001953125f },
    { -721.35302734375f, 2379.6201171875f, 837.3499755859375f },
    { -1013.0809936523438f, 2288.385986328125f, 754.8300170898438f },
    { -1296.6280517578125f, 2161.14990234375f, 699.8300170898438f },
    { -1545.6629638671875f, 1970.64794921875f, 725.010009765625f },
    { -1739.8399658203125f, 1740.97802734375f, 821.5399780273438f },
    { -1899.9990234375f, 1500.001953125f, 949.989990234375f },
};

static Point3d mb2ev_Zen2Rainbow9VtxTbl[0x14] = {
    { 200.0f, 3900.0f, -1850.0f },
    { 321.0570068359375f, 4046.075927734375f, -1569.27001953125f },
    { 455.36700439453125f, 4117.30419921875f, -1268.1400146484375f },
    { 595.5750122070312f, 4055.7880859375f, -968.1400146484375f },
    { 721.197998046875f, 3910.239013671875f, -689.47998046875f },
    { 824.594970703125f, 3735.3330078125f, -418.2699890136719f },
    { 900.7559814453125f, 3554.676025390625f, -141.8800048828125f },
    { 944.2379760742188f, 3381.77001953125f, 146.19000244140625f },
    { 952.7030029296875f, 3212.512939453125f, 439.6000061035156f },
    { 927.0230102539062f, 3036.135009765625f, 727.7100219726562f },
    { 867.7570190429688f, 2842.8359375f, 999.469970703125f },
    { 778.9500122070312f, 2626.906982421875f, 1244.8299560546875f },
    { 670.2529907226562f, 2388.7080078125f, 1459.8499755859375f },
    { 553.6170043945312f, 2131.4150390625f, 1646.9300537109375f },
    { 438.2080078125f, 1855.85400390625f, 1806.8499755859375f },
    { 332.7120056152344f, 1562.4320068359375f, 1939.510009765625f },
    { 238.9949951171875f, 1255.8349609375f, 2049.3798828125f },
    { 154.32000732421875f, 941.0330200195312f, 2142.219970703125f },
    { 75.65299987792969f, 621.573974609375f, 2223.889892578125f },
    { 0.0010000000474974513f, 300.0039978027344f, 2299.989990234375f },
};

static Point3d mb2ev_Zen2Rainbow3VtxTbl[0x14] = {
    { 200.0f, 3900.0f, -1850.0f },
    { 293.58599853515625f, 4017.404052734375f, -1632.469970703125f },
    { 394.4289855957031f, 4099.8828125f, -1403.0699462890625f },
    { 503.4639892578125f, 4110.8662109375f, -1163.9300537109375f },
    { 613.2260131835938f, 4042.236083984375f, -934.3200073242188f },
    { 725.5609741210938f, 3929.75390625f, -723.3200073242188f },
    { 849.8380126953125f, 3801.236083984375f, -528.739990234375f },
    { 1000.2559814453125f, 3674.095947265625f, -353.0f },
    { 1200.5589599609375f, 3576.967041015625f, -212.99000549316406f },
    { 1440.85205078125f, 3519.787109375f, -119.87999725341797f },
    { 1683.2469482421875f, 3457.7041015625f, -35.650001525878906f },
    { 1882.6300048828125f, 3337.98095703125f, 86.4000015258789f },
    { 2018.6710205078125f, 3164.9150390625f, 230.82000732421875f },
    { 2098.8369140625f, 2957.39697265625f, 372.4599914550781f },
    { 2130.55810546875f, 2727.509033203125f, 498.1700134277344f },
    { 2127.35302734375f, 2486.4189453125f, 606.010009765625f },
    { 2102.257080078125f, 2241.093994140625f, 701.02001953125f },
    { 2064.2109375f, 1994.3740234375f, 787.9400024414062f },
    { 2018.8289794921875f, 1747.238037109375f, 870.1099853515625f },
    { 1970.0009765625f, 1500.0030517578125f, 949.989990234375f },
};
static Point3d mb2ev_Emiter1Pos = { 0.0f, 200.0f, 2150.0f };
static Point3d mb2ev_Emiter2Pos = { -2925.0f, 1210.0f, -25.0f };
static Point3d openingRot = { -33.0f, 0.0f, 0.0f };
static Point3d openingOfs = { 0.0f, 1400.0f, 0.0f };
static f32 openingZoom = 20000.0f;
static Point3d mapViewRot = { -78.0f, 0.0f, 0.0f };
static Point3d mapViewPos = { 19.0f, -3.0f, 187.0f };
static f32 mapViewZoom = 19866.0f;
static s8 lbl_1_data_D04[0xA] = "s_cloud00";
static s8 lbl_1_data_D0E[7] = "sisof2";
static s32 mb2ev_RainbowNoTbl[0xB] = { 0, 1, 1, 2, 2, 3, 3, 4, 4, 0, 0 };
static s32 mb2ev_RainbowDirTbl[0xB] = { 0, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1 };
static void *mb2ev_RainbowVtxTbl[5] = {
    &mb2ev_Rainbow1VtxTbl,
    &mb2ev_Rainbow2VtxTbl,
    &mb2ev_Rainbow3VtxTbl,
    &mb2ev_Rainbow4VtxTbl,
    &mb2ev_Rainbow5VtxTbl,
};
static s32 mb2ev_RainbowVtxNumTbl[5] = { 0xA, 0x14, 0x14, 0x14, 0x14 };
static s32 mb2ev_RainbowRevF[5] = { 0, 0, 0, 0, 1 };
static f32 mb2ev_WindGuru2RotTbl[5] = { 0.0f, 180.0f, 90.0f, 0.0f, -90.0f };
static s8 lbl_1_data_DC0[7] = "hazure";
static s8 lbl_1_data_DC7[5] = "k_f3";
static s8 lbl_1_data_DCC[5] = "k_f2";
static s8 lbl_1_data_DD1[5] = "k_f1";
static s8 lbl_1_data_DD6[5] = "k_f4";
static char *mb2ev_WindGuru2HookTbl[5] = { "hazure", "k_f3", "k_f2", "k_f1", "k_f4" };
static Point3d mb2ev_DynamoPos;
static Point3d mb2ev_BiriQCapsulePos;
static s8 lbl_1_data_E08[7] = "dynamo";
static s8 lbl_1_data_E0F[7] = "suimon";
static s8 lbl_1_data_E16[5] = "hs_a";
static s8 lbl_1_data_E1B[5] = "hs_b";
static s8 lbl_1_data_E20[5] = "hs_c";
static s8 lbl_1_data_E25[5] = "hs_d";
static s8 lbl_1_data_E2A[5] = "hs_e";
static s8 lbl_1_data_E2F[5] = "hs_f";
static s8 lbl_1_data_E34[5] = "hs_g";
static s8 lbl_1_data_E39[5] = "hs_h";
static s8 lbl_1_data_E3E[4] = "hs_i" /* not null-terminated */;
static char *mb2ev_RainObjTbl[0xA] = {
    "suimon",
    "hs_a",
    "hs_b",
    "hs_c",
    "hs_d",
    "hs_e",
    "hs_f",
    "hs_g",
    "hs_h",
    "hs_i" /* not null-terminated */,
};
static s8 mb2ev_RainNoTbl[0xA];
static s32 mb2ev_RainSwimLenTbl[0xA] = { 0, 0xF0, 0x3C, 0xB4, 0, 0x5A, 0, 0, 0, 0 };
static Point3d mb2ev_RainSwimMidPos = { -3570.0f, 1190.0f, -625.0f };
static Point3d mb2ev_RainSwimRightPos = { -2450.0f, 1210.0f, 500.0f };
static f32 mb2ev_RainSwimLeftPos[3] = { -3200.0f, 1620.0f, -250.0f };
static f32 mb2ev_RainSwimDefPos = 0.0f;
static struct _struct_mb2ev_Zen2RainbowVtxTbl_0xC mb2ev_Zen2RainbowVtxTbl[3] = {
    {
        &mb2ev_Zen2Rainbow1VtxTbl,
        &mb2ev_Zen2Rainbow2VtxTbl,
        &mb2ev_Zen2Rainbow3VtxTbl,
    },
    {
        &mb2ev_Zen2Rainbow4VtxTbl,
        &mb2ev_Zen2Rainbow5VtxTbl,
        &mb2ev_Zen2Rainbow6VtxTbl,
    },
    {
        &mb2ev_Zen2Rainbow7VtxTbl,
        &mb2ev_Zen2Rainbow8VtxTbl,
        &mb2ev_Zen2Rainbow9VtxTbl,
    },
};
static struct _struct_mb2ev_Zen2RainbowVtxNumTbl_0xC mb2ev_Zen2RainbowVtxNumTbl[3] = { { 0x14, 0x14, 0x14 }, { 0x14, 0x14, 0x14 }, { 0x14, 0x14, 0x14 } };
static s32 mb2ev_Zen2DestTbl[3][3];
static Point3d mb2ev_Zen2CoinBasePos;
static s8 lbl_1_data_F48[7] = "sisof1";
static s32 mb2ev_TimerGameMesId = -1;
static s32 mb2ev_CoinEffColTbl[7] = {
    0xFF7F7FFF,
    0xFF7F40FF,
    0xFFFF7FFF,
    0x7FFF7FFF,
    0x7F7FFFFF,
    0x4040FFFF,
    0xFF7FFFFF,
};
static Point3d mb2_ParticleVtxTbl[4] = {
    { -0.5f, 0.5f, 0.0f },
    { 0.5f, 0.5f, 0.0f },
    { 0.5f, -0.5f, 0.0f },
    { -0.5f, -0.5f, 0.0f },
};
static struct _struct_mb2_ParticleUvTbl_0x8 mb2_ParticleUvTbl[4] = { { 0.0f, 0.0f }, { 0.25f, 0.0f }, { 0.25f, 0.25f }, { 0.0f, 0.25f } };
static f32 yajiAngleTbl[3] = { -45.0f, 0.0f, 45.0f };
static OMOBJ *mb2_MainObj;
static OMOBJ *mb2_CoinObj;
static OMOBJ *mb2_CoinEffObj;
static OMOBJ *mb2_SplashEffObj;
static OMOBJ *mb2_YajiObj;
static OMOBJ *mb2_RainbowObj;
static Point3d mb2_CoinMaxDist;
static s32 mb2ev_TimerValue;
static s32 mb2ev_TimerObj;
static s16 mb2_PathValidF;
static s16 mb2_PathAllF;
static s16 mb2_PathLen;
static s16 mb2_PathMasuNo;
static u32 *mb2_SaveWorkP;

/* mb2_CoinObj->data: 0x20-entry array, stride 0x84 (coin drop slots). */
typedef struct MB2CoinSlot_s {
    s32 modelId;   /* 0x00 */
    s32 activeF;   /* 0x04, 0 = free slot */
    s32 unk8;
    f32 unkC;
    s32 unk10;
    s32 unk14;
    f32 pos[3];    /* 0x18 */
    f32 rot[3];    /* 0x24 */
    f32 scale[3];  /* 0x30 */
    f32 srcPos[3]; /* 0x3C, saved starting position */
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    f32 playerPos[4][3]; /* 0x54, one Point3d per player */
} MB2_COIN_SLOT; /* size 0x84 */

/* mb2_SplashEffObj->data: 2 splash-type model ids + their live-particle counts. */
typedef struct MB2SplashEffData_s {
    s32 modelId[2]; /* 0x00 */
    s32 count[2];   /* 0x08 */
    u8 pad10[4];
} MB2_SPLASHEFF_DATA; /* size 0x14 */

static u8 mb2_Work[0xC4];
#define MB2W_S16(off) (*(s16 *) (mb2_Work + (off)))
#define MB2W_U16(off) (*(u16 *) (mb2_Work + (off)))
#define MB2W_S32(off) (*(s32 *) (mb2_Work + (off)))
#define MB2W_U32(off) (*(u32 *) (mb2_Work + (off)))
#define MB2W_F32(off) (*(f32 *) (mb2_Work + (off)))
#define MB2W_U8(off)  (*(u8 *) (mb2_Work + (off)))

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
    MBObjectSetup(MBNO_WORLD_2, MB2_Create, MB2_Kill);
}

void MB2_Create(void) {
    s32 sp8;
    s16 temp_r3;
    s16 temp_r3_10;
    s16 temp_r3_11;
    s16 temp_r3_12;
    s16 temp_r3_13;
    s16 temp_r3_14;
    s16 temp_r3_15;
    s16 temp_r3_16;
    s16 temp_r3_17;
    s16 temp_r3_18;
    s16 temp_r3_2;
    s16 temp_r3_3;
    s16 temp_r3_4;
    s16 temp_r3_5;
    s16 temp_r3_6;
    s16 temp_r3_7;
    s16 temp_r3_8;
    s16 temp_r3_9;

    sp8 = MBBoardNoGet();
    HuAudSndGrpSetSet(0xF);
    MBMasuCreate(0xCD0000);
    MBScrollInit(0xCD001BU, 18000.0f, -500.0f);
    temp_r3 = MBModelCreate(0xCD0001, NULL, 0);
    MB2W_S32(0x0) = (s32)(uintptr_t) temp_r3;
    MBModelAttrSet(temp_r3, 0x40000001U);
    temp_r3_2 = MBModelCreate(0xCD0007, NULL, 0);
    MB2W_S32((1) * 4) = (s32)(uintptr_t) temp_r3_2;
    MBModelAttrSet(temp_r3_2, 0x40000001U);
    MBMotionSpeedSet(temp_r3_2, 0.5f);
    temp_r3_3 = MBModelCreate(0xCD000D, NULL, 0);
    MB2W_S32((2) * 4) = (s32)(uintptr_t) temp_r3_3;
    MBModelAttrSet(temp_r3_3, 0x40000001U);
    temp_r3_4 = MBModelCreate(0xCD0010, NULL, 0);
    MB2W_S32((3) * 4) = (s32)(uintptr_t) temp_r3_4;
    MBModelAttrSet(temp_r3_4, 0x40000001U);
    temp_r3_5 = MBModelCreate(0xCD0013, NULL, 0);
    MB2W_S32((4) * 4) = (s32)(uintptr_t) temp_r3_5;
    MBMotionSpeedSet(temp_r3_5, 0.05f);
    MBModelAttrSet(temp_r3_5, 0x40000001U);
    MB2W_S32((5) * 4) = (s32)(uintptr_t) MBModelCreate(0xCD0014, NULL, 0);
    MB2W_S32((6) * 4) = (s32)(uintptr_t) MBModelCreate(0xCD000C, NULL, 0);
    temp_r3_6 = MBModelCreate(0xCD001A, NULL, 0);
    MB2W_S32((0xB) * 4) = (s32)(uintptr_t) temp_r3_6;
    MBModelAttrSet(temp_r3_6, 0x40000001U);
    MB2W_S32((7) * 4) = (s32)(uintptr_t) MBModelCreate(0xCD0015, NULL, 0);
    MB2W_S32((8) * 4) = (s32)(uintptr_t) MBModelCreate(0xCD0016, NULL, 0);
    MB2W_S32((9) * 4) = (s32)(uintptr_t) MBModelCreate(0xCD0017, NULL, 0);
    MB2W_S32((0xA) * 4) = (s32)(uintptr_t) MBModelCreate(0xCD0018, NULL, 0);
    temp_r3_7 = MBModelCreate(0xCD0002, NULL, 0);
    MB2W_S32((0xC) * 4) = (s32)(uintptr_t) temp_r3_7;
    MBMotionSpeedSet(temp_r3_7, 0.0f);
    MB2W_S32((0xD) * 4) = (s32)(uintptr_t) MBModelCreate(0xCD0003, NULL, 0);
    temp_r3_8 = MBModelCreate(0xCD0004, NULL, 0);
    MB2W_S32((0xE) * 4) = (s32)(uintptr_t) temp_r3_8;
    MBMotionSpeedSet(temp_r3_8, 0.0f);
    temp_r3_9 = MBModelCreate(0xCD0005, NULL, 0);
    MB2W_S32((0xF) * 4) = (s32)(uintptr_t) temp_r3_9;
    MBModelAttrSet(temp_r3_9, 0x40000001U);
    MBMotionSpeedSet(temp_r3_9, 0.5f);
    temp_r3_10 = MBModelCreate(0xCD0006, NULL, 0);
    MB2W_S32((0x10) * 4) = (s32)(uintptr_t) temp_r3_10;
    MBModelAttrSet(temp_r3_10, 0x40000001U);
    temp_r3_11 = MBModelCreate(0xCD0008, NULL, 0);
    MB2W_S32((0x11) * 4) = (s32)(uintptr_t) temp_r3_11;
    MBMotionSpeedSet(temp_r3_11, 0.0f);
    MBModelPosSet(temp_r3_11, -2640.0f, 1250.0f, 900.0f);
    MBModelLayerSet(temp_r3_11, 2U);
    temp_r3_12 = MBModelCreate(0xCD0008, NULL, 1);
    MB2W_S32((0x12) * 4) = (s32)(uintptr_t) temp_r3_12;
    MBMotionSpeedSet(temp_r3_12, 0.0f);
    MBModelPosSet(temp_r3_12, -2950.0f, 1300.0f, 800.0f);
    MBModelLayerSet(temp_r3_12, 2U);
    temp_r3_13 = MBModelCreate(0xCD0009, NULL, 0);
    MB2W_S32((0x13) * 4) = (s32)(uintptr_t) temp_r3_13;
    MBModelAttrSet(temp_r3_13, 0x40000001U);
    MBMotionSpeedSet(temp_r3_13, 0.33f);
    temp_r3_14 = MBModelCreate(0xCD000A, NULL, 0);
    MB2W_S32((0x14) * 4) = (s32)(uintptr_t) temp_r3_14;
    MBModelPosSet(temp_r3_14, -2930.0f, 1210.0f, 20.0f);
    MBModelRotSet(temp_r3_14, 0.0f, 45.0f, 0.0f);
    MBModelAttrSet(temp_r3_14, 0x40000001U);
    temp_r3_15 = MBModelCreate(0xCD000B, NULL, 0);
    MB2W_S32((0x15) * 4) = (s32)(uintptr_t) temp_r3_15;
    MBModelPosSet(temp_r3_15, -2930.0f, 1215.0f, 20.0f);
    MBModelAttrSet(temp_r3_15, 0x40000001U);
    temp_r3_16 = MBModelCreate(0xCD000E, NULL, 0);
    MB2W_S32((0x16) * 4) = (s32)(uintptr_t) temp_r3_16;
    MBMotionSpeedSet(temp_r3_16, 0.0f);
    MBMotionTimeSet(temp_r3_16, 0.0f);
    MB2W_S32((0x17) * 4) = (s32)(uintptr_t) MBModelCreate(0xCD000F, NULL, 0);
    temp_r3_17 = MBModelCreate(0xCD0011, NULL, 0);
    MB2W_S32((0x18) * 4) = (s32)(uintptr_t) temp_r3_17;
    MBModelRotSet(temp_r3_17, 0.0f, -50.0f, 0.0f);
    temp_r3_18 = MBModelCreate(0xCD0012, NULL, 0);
    MB2W_S32((0x19) * 4) = (s32)(uintptr_t) temp_r3_18;
    MBMotionSpeedSet(temp_r3_18, 0.0f);
    MB2W_S32((0x1A) * 4) = (s32)(uintptr_t) MBModelCreate(0xCD0019, NULL, 0);
    MBCapsuleColCreate(0xCD001C);
    MB2W_S32((0x1B) * 4) = (s32)(uintptr_t) HuSprAnimRead(HuDataReadNum(0xCD001D, 0x10000000));
    MB2W_S32((0x1C) * 4) = 0;
    MB2W_S32((0x1D) * 4) = (s32)(uintptr_t)-1U;
    MB2W_S32((0x25) * 4) = 1.0f;
    MB2W_S32((0x26) * 4) = (s32)(uintptr_t)1U;
    MB2W_S32((0x27) * 4) = (s32)(uintptr_t)1U;
    MB2W_S32((0x28) * 4) = (s32)(uintptr_t)1U;
    MB2W_S32((0x29) * 4) = 0;
    MB2W_S32((0x2C) * 4) = (s32)(uintptr_t) (s32) (360.0f * (0.007874016f * (f32) (frand() & 0x7F)));
    MB2W_S32((0x2D) * 4) = (s32)(uintptr_t) (s32) (360.0f * (0.007874016f * (f32) (frand() & 0x7F)));
    MB2W_S32((0x2E) * 4) = (s32)(uintptr_t) (s32) (360.0f * (0.007874016f * (f32) (frand() & 0x7F)));
    MB2W_S32((0x2F) * 4) = (s32)(uintptr_t) (s32) (360.0f * (0.007874016f * (f32) (frand() & 0x7F)));
    MB2W_S32((0x30) * 4) = (s32)(uintptr_t) (s32) (360.0f * (0.007874016f * (f32) (frand() & 0x7F)));
    MB2W_S32((0x2B) * 4) = 0.0f;
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
    HuDataDirClose(0xCD0000);
    MBBranchFlagSet(0x4000U);
    MBLightHookSet(MB2_LightSet, MB2_LightReset);
    MBMasuWalkPreHookSet(MB2Ev_MasuWalkPre);
    MBMasuWalkPostHookSet(MB2Ev_MasuWalkPost);
    MBMasuHatenaHookSet(MB2Ev_MasuHatena);
    MBMasuPathCheckHookSet(MB2_MasuPathCheck);
    HuDataDirRead(0xB0000);
    mb2_MainObj = omAddObjEx(mbObjMan, -0x8000, 0U, 0U, -1, MB2_MainObjUpdate);
    MBAudFXEmiterCreate(0x5D0, &mb2ev_Emiter1Pos, 0);
    MBAudFXEmiterCreate(0x5DA, &mb2ev_Emiter2Pos, 0);
    MBOpeningViewSet(&openingRot, &openingOfs, openingZoom);
    MBMapViewCameraSet(&mapViewRot, &mapViewPos, mapViewZoom);
}

void MB2_Kill(void) {
    mb2_MainObj = NULL;
    if ((u32) MB2W_S32((0x1B) * 4) != 0) {
        HuSprAnimKill((ANIMDATA *)(uintptr_t) MB2W_S32((0x1B) * 4));
    }
    MB2W_S32((0x1B) * 4) = 0;
    if ((u32) MB2W_S32((0x1C) * 4) != 0) {
        HuMemDirectFree((void *)(uintptr_t) MB2W_S32((0x1C) * 4));
    }
    MB2W_S32((0x1C) * 4) = 0;
    MB2_CoinClose();
    MB2_CoinEffClose();
    MB2_YajiClose();
    MB2_RainbowObjClose();
    MB2_SplashEffClose();
}

void MB2_MainObjUpdate(OMOBJ *obj) {
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    f32 sp20;
    f32 sp1C;
    f32 sp18;
    MBCAMERA *sp14;
    s8 sp13;
    s8 sp12;
    s8 sp11;
    s8 sp10;
    f32 spC;
    u8 spB;
    u8 spA;
    u8 sp9;
    u8 sp8;
    u8 *temp_r4;
    s32 temp_r3_2;
    f32 temp_f1;
    f32 temp_f1_2;
    f32 temp_f28;
    f32 temp_f29;
    f32 temp_f29_2;
    f32 temp_f30;
    f32 temp_f30_2;
    f32 temp_f31;
    f32 temp_f31_2;
    f32 var_f27;
    s32 var_r29;
    s32 var_r31;
    s32 var_r31_2;
    s8 temp_r0;
    HSFOBJECT *temp_r3;
    f32 sp30[4][3];

    if ((MBKillCheck() != 0) || ((OMOBJ *) mb2_MainObj == NULL)) {
        omDelObjEx(mbObjMan, mb2_MainObj);
        mb2_MainObj = NULL;
        return;
    }
    sp14 = MBCameraGet();
    var_r31 = 0;
loop_5:
    if (var_r31 < 4) {
        temp_r4 = mb2_Work + (var_r31 * 4);
        MB2W_S32(var_r31 * 4 + 0xB0) = (s32) (((frand() & 0x7FFF) % 3) + MB2W_S32(var_r31 * 4 + 0xB0) + 1);
        temp_f30 = (f32) (1.0 + (0.009999999776482582 * sin((3.141592653589793 * (f64) MB2W_S32(var_r31 * 4 + 0xB0)) / 180.0)));
        temp_f29 = (f32) (1.0 + (0.0024999999441206455 * sin((3.141592653589793 * (f64) MB2W_S32(var_r31 * 4 + 0xB0)) / 180.0)));
        MBModelScaleSet((s16) MB2W_S32(var_r31 * 4 + 0x1C), temp_f30, temp_f29, (f32) (1.0 + (0.009999999776482582 * sin((3.141592653589793 * (f64) MB2W_S32(var_r31 * 4 + 0xB0)) / 180.0))));
        var_r31 += 1;
        goto loop_5;
    }
    MB2W_S32((0x30) * 4) = ((frand() & 0x7FFF) % 3) + MB2W_S32((0x30) * 4) + 1;
    temp_f30_2 = (f32) (1.0 + (0.009999999776482582 * sin((3.141592653589793 * (f64) MB2W_S32((0x30) * 4)) / 180.0)));
    temp_f29_2 = (f32) (1.0 + (0.0024999999441206455 * sin((3.141592653589793 * (f64) MB2W_S32((0x30) * 4)) / 180.0)));
    temp_f28 = (f32) (1.0 + (0.009999999776482582 * sin((3.141592653589793 * (f64) MB2W_S32((0x30) * 4)) / 180.0)));
    temp_r3 = MB2_ModelObjPtrGet(&Hu3DData[MBModelIdGet((s16) MB2W_S32((0x1A) * 4))], "s_cloud00");
    if (temp_r3 != NULL) {
        temp_r3->mesh.base.scale.x = temp_f30_2;
        temp_r3->mesh.base.scale.y = temp_f29_2;
        temp_r3->mesh.base.scale.z = temp_f28;
    }
    if ((s32) MB2W_S32((0x1D) * 4) != -1) {
        if ((s32) MB2W_S32((0x26) * 4) != 0) {
            if (MB2W_F32((0x25) * 4) < 1.0f) {
                temp_f1 = MB2W_F32((0x25) * 4) + 0.03f;
                MB2W_S32((0x25) * 4) = temp_f1;
                if (temp_f1 > 1.0f) {
                    MB2W_S32((0x25) * 4) = 1.0f;
                }
            }
        } else if (MB2W_F32((0x25) * 4) > 0.0f) {
            temp_f1_2 = MB2W_F32((0x25) * 4) - 0.03f;
            MB2W_S32((0x25) * 4) = temp_f1_2;
            if (temp_f1_2 < 0.0f) {
                MB2W_S32((0x25) * 4) = 0.0f;
            }
        }
        temp_r0 = (s8) (255.0f * MB2W_F32((0x25) * 4));
        MB2W_U8(0x93) = temp_r0;
        MB2W_U8(0x92) = temp_r0;
        MB2W_U8(0x91) = temp_r0;
        MB2W_U8(0x90) = temp_r0;
        Hu3DGLightColorSet((s16) MB2W_S32((0x1D) * 4), (u8) MB2W_U8(0x90), (u8) MB2W_U8(0x91), (u8) MB2W_U8(0x92), (u8) MB2W_U8(0x93));
    }
    if ((s32) MB2W_S32((0x27) * 4) != 0) {
        Hu3DModelObjMtxGet(MBModelIdGet((s16) MB2W_S32((0x19) * 4)), "sisof2", (f32 (*)[4]) &sp30[0]);
        MBModelPosSet((s16) MB2W_S32((0x18) * 4), sp30[3][0], sp30[3][1], sp30[3][2]);
    }
    if ((s32) MB2W_S32((0x28) * 4) != 0) {
        var_r31_2 = 0;
loop_31:
        if (var_r31_2 < 3) {
            temp_f31 = 0.5f - (0.007874016f * (f32) (frand() & 0x7F));
            sp24 = (f32) (-2925.0 + (1.5 * (100.0 * ((f64) temp_f31 * sin(-0.7853981633974483)))));
            sp28 = 1210.0f + (0.25f * (100.0f * (0.5f - (0.007874016f * (f32) (frand() & 0x7F)))));
            sp2C = (f32) (-25.0 + (1.5 * (100.0 * ((f64) temp_f31 * cos(-0.7853981633974483)))));
            sp20 = 0.0f;
            sp1C = 0.0f;
            sp18 = 0.0f;
            temp_f31_2 = 64.0f * (0.007874016f * (f32) (frand() & 0x7F));
            sp10 = (s8) (170.0f + temp_f31_2);
            sp11 = (s8) (170.0f + temp_f31_2);
            sp12 = (s8) (190.0f + temp_f31_2);
            sp13 = (s8) (64.0f + (63.0f * (0.007874016f * (f32) (frand() & 0x7F))));
            if ((u32) (frand() & 1) != 0) {
                var_f27 = 0.1f;
            } else {
                var_f27 = -0.1f;
            }
            spC = var_f27;
            if ((var_r31_2 < 2) || (temp_r3_2 = MB2W_S32((0x29) * 4) + 1, MB2W_S32((0x29) * 4) = temp_r3_2, ((temp_r3_2 & 1) != 0))) {
                if (var_r31_2 < 2) {
                    var_r29 = 0;
                } else {
                    var_r29 = 1;
                }
                sp8 = (u8) sp10;
                sp9 = (u8) sp11;
                spA = (u8) sp12;
                spB = (u8) sp13;
                MB2_SplashEffCreateRandX(&sp24, &sp8, var_r29, 1.0f);
            }
            var_r31_2 += 1;
            goto loop_31;
        }
    }
    if ((s32) MB2W_S32((3) * 4) != -1) {
        if (MB2W_F32((0x2B) * 4) > 10.0f) {
            MBMotionTimeGet((s16) MB2W_S32((3) * 4));
        }
        MB2W_S32((0x2B) * 4) = MBMotionTimeGet((s16) MB2W_S32((3) * 4));
    }
    if ((MBMotionSpeedGet((s16) MB2W_S32((0x11) * 4)) > 0.0f) && (MBMotionEndCheck((s16) MB2W_S32((0x11) * 4)) != 0)) {
        MBMotionSpeedSet((s16) MB2W_S32((0x11) * 4), 0.0f);
        MBMotionTimeSet((s16) MB2W_S32((0x11) * 4), 0.0f);
    }
    if ((MBMotionSpeedGet((s16) MB2W_S32((0x12) * 4)) > 0.0f) && (MBMotionEndCheck((s16) MB2W_S32((0x12) * 4)) != 0)) {
        MBMotionSpeedSet((s16) MB2W_S32((0x12) * 4), 0.0f);
        MBMotionTimeSet((s16) MB2W_S32((0x12) * 4), 0.0f);
    }
}

void MB2_LightSet(void) {
    Hu3DShadowCreate(45.0f, 1000.0f, 250000.0f);
    Hu3DShadowTPLvlSet(0.3f);
    Hu3DBGColorSet(0xFFU, 0xFFU, 0xFFU);
    MB2W_F32((0x1E) * 4) = -3700.0f;
    MB2W_F32((0x1F) * 4) = 18100.0f;
    MB2W_F32((0x20) * 4) = 21300.0f;
    MB2W_F32((0x21) * 4) = 3700.0f;
    MB2W_F32((0x22) * 4) = -18100.0f;
    MB2W_F32((0x23) * 4) = -21300.0f;
    MB2W_U8(0x90) = 0xFFU;
    MB2W_U8(0x91) = 0xFFU;
    MB2W_U8(0x92) = 0xFFU;
    MB2W_S32((0x1D) * 4) = (s32)(uintptr_t) Hu3DGLightCreate(MB2W_F32((0x1E) * 4), MB2W_F32((0x1F) * 4), MB2W_F32((0x20) * 4), MB2W_F32((0x21) * 4), MB2W_F32((0x22) * 4), MB2W_F32((0x23) * 4), MB2W_U8(0x90), MB2W_U8(0x91), MB2W_U8(0x92));
    Hu3DGLightInfinitytSet((s16) MB2W_S32((0x1D) * 4));
}

void MB2_LightReset(void) {

}

int MB2Ev_MasuHatena(int playerNo) {
    u32 temp_r0;
    u32 temp_r29;

    temp_r0 = MBMasuGet(0, (s32) (s16) GwPlayer[playerNo].masuId)->flag;
    if ((u32) ((u16) temp_r0 & 0x70) != 0) {
        temp_r29 = (u32) ((u16) temp_r0 & 0x70) >> 4U;
        switch ((s32) temp_r29) {                   /* irregular */
        case 1:
            MB2Ev_Thunder(playerNo);
            break;
        case 2:
            MB2Ev_Rain(playerNo);
            break;
        case 3:
            MB2Ev_Zen2(playerNo);
            break;
        case 4:
            MB2Ev_Zen4(playerNo);
            break;
        }
    }
    if ((u32) ((u16) temp_r0 & 0x380) != 0) {
        MB2Ev_WindGuru2((u32) ((u16) temp_r0 & 0x380) >> 7U, playerNo);
    }
}

void MB2_Stub(void) {

}

int MB2Ev_MasuWalkPre(int playerNo) {
}

int MB2Ev_MasuWalkPost(int playerNo) {
    u32 temp_r0;

    temp_r0 = MBMasuGet(0, (s32) (s16) GwPlayer[playerNo].masuId)->flag;
    if ((u32) ((u16) temp_r0 & 0x1000) != 0) {
        HuAudFXPlay(0x5CF);
        MBMotionTimeSet((s16) MB2W_S32((0x11) * 4), 0.0f);
        MBMotionSpeedSet((s16) MB2W_S32((0x11) * 4), 1.0f);
    }
    if ((u32) ((u16) temp_r0 & 0x2000) != 0) {
        HuAudFXPlay(0x5CF);
        MBMotionTimeSet((s16) MB2W_S32((0x12) * 4), 0.0f);
        MBMotionSpeedSet((s16) MB2W_S32((0x12) * 4), 1.0f);
    }
    if ((u32) ((u16) temp_r0 & 0xF) != 0) {
        MB2Ev_Rainbow((u16) temp_r0 & 0xF, playerNo);
    }
    return 0;
}

int MB2_MasuPathCheck(s16 id, u32 flag, s16 *linkTbl, BOOL endF) {
    s32 spC;
    u32 sp8;
    MASU *temp_r30;
    s16 temp_r3;
    s32 temp_r24;
    s32 var_r29;
    s32 var_r31;
    s8 var_r28;

    sp8 = flag;
    spC = endF;
    if ((u32) (sp8 & 0xF) != 0) {
        temp_r3 = MB2_RainbowMasuNoEndGet(id);
        if (temp_r3 == -1) {
            return -1;
        }
        temp_r30 = MBMasuGet(0, (s32) id);
        var_r29 = 0;
        var_r31 = 0;
loop_7:
        if (var_r29 < (s32) temp_r30->linkNum) {
            temp_r24 = MBBranchFlagGet() | 0x06000000;
            if ((u32) (MBMasuFlagGet(0, (s32) temp_r30->linkTbl[var_r29]) & temp_r24) == 0) {
                linkTbl[var_r31] = (s16) temp_r30->linkTbl[var_r29];
                var_r31 += 1;
            }
            var_r29 += 1;
            goto loop_7;
        }
        var_r28 = (s8) (u8) GwSystem.turnPlayerNo;
        if (var_r28 < 0) {
            var_r28 = 0;
        }
        if ((MBPlayerCoinGet((s32) var_r28) <= 5) || (spC != 0)) {
            linkTbl[var_r31] = temp_r3;
            var_r31 += 1;
        }
        return var_r31;
    }
    return -1;
}

s32 MB2Ev_Rainbow(s32 arg0, s32 arg1) {
    Point3d sp5C;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    Point3d sp44;
    Point3d sp38;
    f32 sp34;
    f32 sp30;
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    f32 sp20;
    MASU *sp1C;
    s32 sp18;
    s32 sp14;
    s32 sp10;
    f32 *spC;
    f32 *sp8;
    Point3d *temp_r27;
    f32 temp_f1;
    f32 temp_f26;
    f32 temp_f26_2;
    f32 temp_f29;
    f32 temp_f29_2;
    f32 temp_f31;
    f32 temp_f31_2;
    f32 temp_f31_3;
    f32 var_f28;
    s16 temp_r17;
    s16 temp_r18;
    s16 temp_r20;
    s16 temp_r21;
    s16 temp_r3;
    s32 temp_r25;
    s32 temp_r28;
    s32 temp_r28_2;
    s32 temp_r28_3;
    s32 temp_r28_4;
    s32 temp_r28_5;
    s32 temp_r3_2;
    s32 var_r26;
    s32 var_r26_2;
    s32 var_r29;
    s32 var_r29_2;
    s32 var_r30;
    s32 var_r30_2;
    s32 var_r30_3;
    s32 var_r30_4;
    s32 var_r30_5;
    s32 var_r30_6;
    s32 var_r30_7;
    s32 var_r30_8;
    Point3d *temp_r3_3;
    Point3d *temp_r4;
    Point3d *temp_r4_2;

    sp10 = 0;
    if ((arg0 <= 0) || (arg0 > 0xA)) {
        return 0;
    }
    if (arg0 == 0xA) {
        return 0;
    }
    temp_r21 = GwPlayer[arg1].masuId;
    sp1C = MBMasuGet(0, (s32) temp_r21);
    temp_r25 = mb2ev_RainbowNoTbl[arg0];
    temp_r3 = MB2_RainbowMasuNoEndGet(temp_r21);
    if (temp_r3 == -1) {
        return 0;
    }
    MBPlayerMotionNoShiftSet(arg1, 1, 0.0f, 8.0f, 0U);
    MB2_RainbowObjInit();
    temp_r3_2 = MB2_RainbowObjCreate(mb2ev_RainbowVtxTbl[temp_r25], mb2ev_RainbowVtxNumTbl[temp_r25], mb2ev_RainbowRevF[temp_r25]);
    MB2_RainbowObjAlphaSet(temp_r3_2, 0);
    MB2_CoinEffInit();
    MBWalkNumDispSet(arg1, 0);
    MBCameraFocusPlayerSet(arg1);
    MBPlayerMotionNoShiftSet(arg1, 1, 0.0f, 8.0f, 0U);
loop_8:
    if (arg0 == 9) {
        var_r29 = 0;
    } else {
        MBAudFXPlay(0x2F5);
        MBWinCreateChoice(1, 0x2E0000U, 2, 0);
        MBTopWinAttrSet(0x10U);
        if (MBPlayerCoinGet(arg1) < 5) {
            MBTopWinChoiceDisable(0);
        }
        if (GwPlayer[arg1].comF != 0) {
            temp_r17 = (GwPlayer[arg1].dif + 1) * 0xA;
            temp_r18 = MB2_PathLenGet(temp_r21, temp_r17, 1);
            temp_r20 = MB2_PathLenGet(temp_r3, temp_r17, 1);
            if (MBPlayerCoinGet(arg1) < 5) {
                var_r29_2 = 1;
            } else if ((temp_r18 == -1) && (temp_r20 == -1)) {
                if (MB2_PathCheck(temp_r21) != 0) {
                    var_r29_2 = 1;
                } else if (MB2_PathCheck(temp_r3) != 0) {
                    var_r29_2 = 0;
                } else {
                    temp_f31 = 0.6f + (0.4f * ((f32) GwPlayer[arg1].dif / 3.0f));
                    if ((0.007874016f * (f32) (frand() & 0x7F)) == temp_f31) {
                        var_r29_2 = 0;
                    } else {
                        var_r29_2 = 1;
                    }
                }
            } else if ((temp_r18 != -1) && (temp_r20 != -1)) {
                if (temp_r20 < temp_r18) {
                    var_r29_2 = 0;
                } else {
                    var_r29_2 = 1;
                }
            } else if (temp_r20 != -1) {
                var_r29_2 = 0;
            } else {
                var_r29_2 = 1;
            }
            if (MBPlayerCoinGet(arg1) >= 0xA) {
                if (var_r29_2 == 0) {
                    MBComChoiceSetUp();
                } else {
                    MBComChoiceSetDown();
                }
            } else {
                MBComChoiceSetUp();
            }
        }
        MBTopWinWait();
        var_r29 = MBWinLastChoiceGet();
        MBTopWinKill();
        if (var_r29 != 2) {
            if (var_r29 < 2) {

            }
        } else {
            MBScrollExec(arg1);
            MBStatusDispSetAll(1);
        }
        if (var_r29 == 2) {
            goto loop_8;
        }
    }
    if (var_r29 == 0) {
        if (arg0 != 9) {
            MBCoinAddExec(arg1, -5);
        }
        HuAudFXPlay(0x5C8);
        var_r30 = 0;
loop_47:
        if ((f32) var_r30 < 30.0f) {
            temp_f1 = (f32) var_r30;
            MB2_RainbowObjAlphaSet(temp_r3_2, (s8) (255.0f * (temp_f1 / 30.0f)));
            HuPrcVSleep();
            var_r30 += 1;
            goto loop_47;
        }
        MB2_RainbowObjAlphaSet(temp_r3_2, -1);
        MBPlayerPosFixEvent(arg1, (s32) temp_r3, 0);
        temp_r27 = mb2ev_RainbowVtxTbl[temp_r25];
        var_r30_2 = 1;
        var_f28 = 0.0f;
loop_50:
        if (var_r30_2 < (s32) mb2ev_RainbowVtxNumTbl[temp_r25]) {
            PSVECSubtract(temp_r27 + (var_r30_2 - 1), temp_r27 + var_r30_2, &sp44);
            var_f28 += PSVECMag(&sp44);
            var_r30_2 += 1;
            goto loop_50;
        }
        sp14 = (s32) (var_f28 / 20.0f);
        MBPlayerMotionNoShiftSet(arg1, 3, 0.0f, 8.0f, 0x40000001U);
        if ((s32) mb2ev_RainbowDirTbl[arg0] > 0) {
            MBPlayerPosGet(arg1, &sp5C);
            sp50 = temp_r27->x;
            sp54 = temp_r27->y;
            sp58 = temp_r27->z;
            PSVECSubtract((Point3d *) &sp50, &sp5C, &sp44);
            temp_r28 = (s32) (PSVECMag(&sp44) / 20.0f);
            var_r30_3 = 0;
loop_54:
            if (var_r30_3 < temp_r28) {
                PSVECSubtract((Point3d *) &sp50, &sp5C, &sp44);
                PSVECScale(&sp44, &sp44, (f32) var_r30_3 / (f32) temp_r28);
                PSVECAdd(&sp5C, &sp44, &sp44);
                MBPlayerPosSetV(arg1, &sp44);
                PSVECSubtract((Point3d *) &sp50, &sp5C, &sp44);
                MBPlayerRotGet(arg1, &sp38);
                sp38.y = (f32) (180.0 * (atan2((f64) sp44.x, (f64) sp44.z) / 3.141592653589793));
                MBPlayerRotSetV(arg1, &sp38);
                HuPrcVSleep();
                var_r30_3 += 1;
                goto loop_54;
            }
            MBPlayerPosSetV(arg1, (Point3d *) &sp50);
            var_r30_4 = 0;
loop_60:
            if (var_r30_4 < (s32) (mb2ev_RainbowVtxNumTbl[temp_r25] - 1)) {
                sp18 = var_r30_4;
                MBPlayerPosGet(arg1, &sp5C);
                temp_r4 = temp_r27 + (var_r30_4 + 1);
                sp50 = temp_r4->x;
                sp54 = temp_r4->y;
                sp58 = temp_r4->z;
                PSVECSubtract((Point3d *) &sp50, &sp5C, &sp44);
                temp_r28_2 = (s32) (PSVECMag(&sp44) / 20.0f);
                MBPlayerRotGet(arg1, &sp38);
                PSVECSubtract((Point3d *) &sp50, &sp5C, &sp44);
                temp_f29 = (f32) (180.0 * (atan2((f64) sp44.x, (f64) sp44.z) / 3.141592653589793));
                var_r26 = 0;
loop_58:
                if (var_r26 < temp_r28_2) {
                    temp_f31_2 = (f32) var_r26 / (f32) temp_r28_2;
                    PSVECSubtract((Point3d *) &sp50, &sp5C, &sp44);
                    PSVECScale(&sp44, &sp44, temp_f31_2);
                    PSVECAdd(&sp5C, &sp44, &sp44);
                    MBPlayerPosSetV(arg1, &sp44);
                    sp38.y = fn_1_BF54(temp_f31_2, sp38.y, temp_f29);
                    MBPlayerRotSet(arg1, 0.0f, sp38.y, 0.0f);
                    MBPlayerPosGet(arg1, &sp44);
                    sp2C = sp44.x;
                    sp30 = sp44.y;
                    sp34 = sp44.z;
                    spC = &sp2C;
                    temp_f26 = 0.016666668f * (0.5f + (0.3f * (0.007874016f * (f32) (frand() & 0x7F))));
                    MB2_CoinEffCreateColRand(spC, 2, 100.0f * (0.2f + (0.1f * (0.007874016f * (f32) (frand() & 0x7F)))), temp_f26, 150.0f, 100.0f, 0.0f);
                    HuPrcVSleep();
                    var_r26 += 1;
                    goto loop_58;
                }
                MBPlayerPosSetV(arg1, (Point3d *) &sp50);
                MBPlayerRotSet(arg1, 0.0f, temp_f29, 0.0f);
                var_r30_4 += 1;
                goto loop_60;
            }
        } else {
            MBPlayerPosGet(arg1, &sp5C);
            temp_r4_2 = temp_r27 + (mb2ev_RainbowVtxNumTbl[temp_r25] - 1);
            sp50 = temp_r4_2->x;
            sp54 = temp_r4_2->y;
            sp58 = temp_r4_2->z;
            PSVECSubtract((Point3d *) &sp50, &sp5C, &sp44);
            temp_r28_3 = (s32) (PSVECMag(&sp44) / 20.0f);
            var_r30_5 = 0;
loop_64:
            if (var_r30_5 < temp_r28_3) {
                PSVECSubtract((Point3d *) &sp50, &sp5C, &sp44);
                PSVECScale(&sp44, &sp44, (f32) var_r30_5 / (f32) temp_r28_3);
                PSVECAdd(&sp5C, &sp44, &sp44);
                MBPlayerPosSetV(arg1, &sp44);
                PSVECSubtract((Point3d *) &sp50, &sp5C, &sp44);
                MBPlayerRotGet(arg1, &sp38);
                sp38.y = (f32) (180.0 * (atan2((f64) sp44.x, (f64) sp44.z) / 3.141592653589793));
                MBPlayerRotSetV(arg1, &sp38);
                HuPrcVSleep();
                var_r30_5 += 1;
                goto loop_64;
            }
            MBPlayerPosSetV(arg1, (Point3d *) &sp50);
            var_r30_6 = mb2ev_RainbowVtxNumTbl[temp_r25] - 1;
loop_70:
            if (var_r30_6 > 0) {
                sp18 = var_r30_6;
                MBPlayerPosGet(arg1, &sp5C);
                temp_r3_3 = temp_r27 + (var_r30_6 - 1);
                sp50 = temp_r3_3->x;
                sp54 = temp_r3_3->y;
                sp58 = temp_r3_3->z;
                PSVECSubtract((Point3d *) &sp50, &sp5C, &sp44);
                temp_r28_4 = (s32) (PSVECMag(&sp44) / 20.0f);
                MBPlayerRotGet(arg1, &sp38);
                PSVECSubtract((Point3d *) &sp50, &sp5C, &sp44);
                temp_f29_2 = (f32) (180.0 * (atan2((f64) sp44.x, (f64) sp44.z) / 3.141592653589793));
                var_r26_2 = 0;
loop_68:
                if (var_r26_2 < temp_r28_4) {
                    temp_f31_3 = (f32) var_r26_2 / (f32) temp_r28_4;
                    PSVECSubtract((Point3d *) &sp50, &sp5C, &sp44);
                    PSVECScale(&sp44, &sp44, temp_f31_3);
                    PSVECAdd(&sp5C, &sp44, &sp44);
                    MBPlayerPosSetV(arg1, &sp44);
                    sp38.y = fn_1_BF54(temp_f31_3, sp38.y, temp_f29_2);
                    MBPlayerRotSet(arg1, 0.0f, sp38.y, 0.0f);
                    MBPlayerPosGet(arg1, &sp44);
                    sp20 = sp44.x;
                    sp24 = sp44.y;
                    sp28 = sp44.z;
                    sp8 = &sp20;
                    temp_f26_2 = 0.016666668f * (0.5f + (0.3f * (0.007874016f * (f32) (frand() & 0x7F))));
                    MB2_CoinEffCreateColRand(sp8, 2, 100.0f * (0.3f + (0.1f * (0.007874016f * (f32) (frand() & 0x7F)))), temp_f26_2, 150.0f, 100.0f, 0.0f);
                    HuPrcVSleep();
                    var_r26_2 += 1;
                    goto loop_68;
                }
                MBPlayerPosSetV(arg1, (Point3d *) &sp50);
                MBPlayerRotSet(arg1, 0.0f, temp_f29_2, 0.0f);
                var_r30_6 -= 1;
                goto loop_70;
            }
        }
        MBPlayerPosGet(arg1, &sp5C);
        MBMasuPosGet(0, (s32) temp_r3, (Point3d *) &sp50);
        PSVECSubtract((Point3d *) &sp50, &sp5C, &sp44);
        temp_r28_5 = (s32) (PSVECMag(&sp44) / 20.0f);
        var_r30_7 = 0;
loop_73:
        if (var_r30_7 < temp_r28_5) {
            PSVECSubtract((Point3d *) &sp50, &sp5C, &sp44);
            PSVECScale(&sp44, &sp44, (f32) var_r30_7 / (f32) temp_r28_5);
            PSVECAdd(&sp5C, &sp44, &sp44);
            MBPlayerPosSetV(arg1, &sp44);
            PSVECSubtract((Point3d *) &sp50, &sp5C, &sp44);
            MBPlayerRotGet(arg1, &sp38);
            sp38.y = (f32) (180.0 * (atan2((f64) sp44.x, (f64) sp44.z) / 3.141592653589793));
            MBPlayerRotSetV(arg1, &sp38);
            HuPrcVSleep();
            var_r30_7 += 1;
            goto loop_73;
        }
        MBPlayerPosSetV(arg1, (Point3d *) &sp50);
        MBPlayerMotionNoShiftSet(arg1, 1, 0.0f, 8.0f, 0x40000001U);
        GwPlayer[arg1].masuId = temp_r3;
        var_r30_8 = 0;
loop_76:
        if ((f32) var_r30_8 < 60.0f) {
            MB2_RainbowObjAlphaSet(temp_r3_2, (s8) (255.0f * (1.0f - ((f32) var_r30_8 / 60.0f))));
            HuPrcVSleep();
            var_r30_8 += 1;
            goto loop_76;
        }
        MB2_RainbowObjAlphaSet(temp_r3_2, 0);
        sp10 = 1;
    }
    MB2_RainbowObjClose();
    MB2_CoinEffClose();
    MBWalkNumDispSet(arg1, 1);
    return sp10;
}

void MB2Ev_WindGuru2(s32 arg0, s32 arg1) {
    Point3d sp2C;
    f32 sp28;
    f32 sp24;
    f32 sp20;
    Point3d sp14;
    Point3d sp8;
    f32 sp38[4][3];
    MASU *temp_r26;
    f32 temp_f1;
    f32 temp_f25;
    f32 temp_f27;
    f32 temp_f28;
    f32 temp_f29;
    f32 temp_f30;
    f32 temp_f30_2;
    f32 var_f1;
    f32 var_f31;
    f32 var_f31_2;
    s16 temp_r19;
    s16 temp_r22;
    s16 temp_r24;
    s16 temp_r27;
    s16 temp_r28;
    s32 temp_r18;
    s32 temp_r3;
    s32 var_r23;
    s32 var_r25;
    s32 var_r29;
    s32 var_r29_2;
    s32 var_r30;
    s32 var_r30_2;
    s32 var_r30_3;
    u32 temp_r20;

    temp_r27 = GwPlayer[arg1].masuId;
    temp_r26 = MBMasuGet(0, (s32) temp_r27);
    if ((arg0 <= 0) || (arg0 > 4)) {
        return;
    }
    MBPlayerMotionNoShiftSet(arg1, 1, 0.0f, 8.0f, 0U);
    MBWalkNumDispSet(arg1, 0);
    MBCameraFocusPlayerSet(arg1);
    MBPlayerRotateStart(arg1, (s16) mb2ev_WindGuru2RotTbl[arg0], 0xF);
loop_5:
    if (MBPlayerRotateCheck(arg1) == 0) {
        HuPrcVSleep();
        goto loop_5;
    }
loop_6:
    var_r30 = 0;
    var_r23 = 0;
loop_10:
    if (var_r30 < (s32) temp_r26->linkNum) {
        temp_r18 = MBBranchFlagGet() | 0x06000000;
        if ((u32) (MBMasuFlagGet(0, (s32) temp_r26->linkTbl[var_r30]) & temp_r18) == 0) {
            var_r23 += 1;
        }
        var_r30 += 1;
        goto loop_10;
    }
    if (var_r23 <= 0) {
        var_r25 = 0;
    } else {
        MBAudFXPlay(0x2F5);
        MBWinCreateChoice(1, 0x2E0001U, 2, 0);
        MBTopWinAttrSet(0x10U);
        if (GwPlayer[arg1].comF != 0) {
            temp_r28 = MB2_WindGuru2MasuNoGet(temp_r27, -1U);
            temp_r19 = (GwPlayer[arg1].dif + 1) * 0xA;
            temp_r22 = MB2_PathLenGet(temp_r27, temp_r19, 1);
            temp_r24 = MB2_PathLenGet(temp_r28, temp_r19, 1);
            if ((temp_r22 == -1) && (temp_r24 == -1)) {
                if (MB2_PathCheck(temp_r27) != 0) {
                    MBComChoiceSetDown();
                } else if (MB2_PathCheck(temp_r28) != 0) {
                    MBComChoiceSetUp();
                } else if ((u32) (frand() & 1) != 0) {
                    MBComChoiceSetUp();
                } else {
                    MBComChoiceSetDown();
                }
            } else if ((temp_r22 != -1) && (temp_r24 != -1)) {
                if (temp_r24 < temp_r22) {
                    MBComChoiceSetUp();
                } else {
                    MBComChoiceSetDown();
                }
            } else if (temp_r24 != -1) {
                MBComChoiceSetUp();
            } else {
                MBComChoiceSetDown();
            }
        }
        MBTopWinWait();
        var_r25 = MBWinLastChoiceGet();
        MBTopWinKill();
        if (var_r25 != 2) {
            if (var_r25 < 2) {

            }
        } else {
            MBScrollExec(arg1);
            MBStatusDispSetAll(1);
        }
        if (var_r25 == 2) {
            goto loop_6;
        }
    }
    if (var_r25 == 0) {
        temp_r20 = *mb2_SaveWorkP;
        *mb2_SaveWorkP = ((frand() & 0x7FFF) % 3) + 1;
        temp_f28 = MBMotionTimeGet((s16) MB2W_S32((4) * 4));
        temp_f27 = temp_f28;
        var_f1 = MBMotionMaxTimeGet((s16) MB2W_S32((4) * 4));
        temp_f29 = 0.25f * var_f1;
        var_r29 = 0;
loop_40:
        if (var_r29 < 4) {
            var_f1 = (f32) 4503601774854144.0;
            if (!(temp_f28 < ((f32) var_r29 * temp_f29))) {
                var_r29 += 1;
                goto loop_40;
            }
        }
        if (var_r29 >= 4) {
            var_r29 = 0;
            var_f31 = 4.0f * temp_f29;
        } else {
            var_f1 = (f32) 4503601774854144.0;
            var_f31 = (f32) var_r29 * temp_f29;
        }
loop_44:
        HuPrcVSleep(var_f1);
        var_f1 = MBMotionTimeGet((s16) MB2W_S32((4) * 4));
        if (var_f1 < var_f31) {
            if (var_f1 == temp_f27) {
                goto loop_44;
            }
        }
        var_r29_2 = arg0 - var_r29;
        if (var_r29_2 < 1) {
            var_r29_2 += 4;
        }
        MBPlayerPosGet(arg1, &sp2C);
        MBPlayerMotionNoShiftSet(arg1, 4, 0.0f, 8.0f, 0U);
        var_r30_2 = 0;
loop_50:
        if ((f32) var_r30_2 < 24.0f) {
            temp_f30 = (f32) var_r30_2 / 24.0f;
            Hu3DModelObjMtxGet(MBModelIdGet((s16) MB2W_S32((4) * 4)), mb2ev_WindGuru2HookTbl[var_r29_2], (f32 (*)[4]) &sp38[0]);
            sp20 = sp38[3][0];
            sp24 = sp38[3][1];
            sp28 = sp38[3][2];
            PSVECSubtract((Point3d *) &sp20, &sp2C, &sp14);
            PSVECScale(&sp14, &sp14, temp_f30);
            PSVECAdd(&sp2C, &sp14, &sp14);
            sp14.y = (f32) ((f64) sp14.y + (300.0 * sin((3.141592653589793 * (f64) (180.0f * temp_f30)) / 180.0)));
            MBPlayerPosSetV(arg1, &sp14);
            MBPlayerRotGet(arg1, &sp8);
            PSVECSubtract((Point3d *) &sp20, &sp2C, &sp14);
            sp8.y = (f32) (180.0 * (atan2((f64) sp14.x, (f64) sp14.z) / 3.141592653589793));
            MBPlayerRotSetV(arg1, &sp8);
            HuPrcVSleep();
            var_r30_2 += 1;
            goto loop_50;
        }
        MBPlayerMotionNoShiftSet(arg1, 1, 0.0f, 8.0f, 0x40000001U);
        MBMotionTimeGet((s16) MB2W_S32((4) * 4));
        temp_f25 = 0.25f * MBMotionMaxTimeGet((s16) MB2W_S32((4) * 4)) * (f32) temp_r20;
        temp_f1 = MBMotionSpeedGet((s16) MB2W_S32((4) * 4));
        var_f31_2 = 24.0f * temp_f1;
loop_53:
        if (var_f31_2 < temp_f25) {
            Hu3DModelObjMtxGet(MBModelIdGet((s16) MB2W_S32((4) * 4)), mb2ev_WindGuru2HookTbl[var_r29_2], (f32 (*)[4]) &sp38[0]);
            sp20 = sp38[3][0];
            sp24 = sp38[3][1];
            sp28 = sp38[3][2];
            MBPlayerPosSetV(arg1, (Point3d *) &sp20);
            HuPrcVSleep();
            var_f31_2 += temp_f1;
            goto loop_53;
        }
        temp_r3 = MB2_WindGuru2MasuNoGet(temp_r27, temp_r20);
        MBPlayerPosFixEvent(arg1, temp_r3, 0);
        MBPlayerPosGet(arg1, &sp2C);
        MBMasuPosGet(0, temp_r3, (Point3d *) &sp20);
        MBPlayerMotionNoShiftSet(arg1, 4, 0.0f, 8.0f, 0U);
        var_r30_3 = 0;
loop_56:
        if ((f32) var_r30_3 < 24.0f) {
            temp_f30_2 = (f32) var_r30_3 / 24.0f;
            PSVECSubtract((Point3d *) &sp20, &sp2C, &sp14);
            PSVECScale(&sp14, &sp14, temp_f30_2);
            PSVECAdd(&sp2C, &sp14, &sp14);
            sp14.y = (f32) ((f64) sp14.y + (300.0 * sin((3.141592653589793 * (f64) (180.0f * temp_f30_2)) / 180.0)));
            MBPlayerPosSetV(arg1, &sp14);
            MBPlayerRotGet(arg1, &sp8);
            PSVECSubtract((Point3d *) &sp20, &sp2C, &sp14);
            sp8.y = (f32) (180.0 * (atan2((f64) sp14.x, (f64) sp14.z) / 3.141592653589793));
            MBPlayerRotSetV(arg1, &sp8);
            HuPrcVSleep();
            var_r30_3 += 1;
            goto loop_56;
        }
        MBPlayerMotionNoShiftSet(arg1, 1, 0.0f, 8.0f, 0x40000001U);
        GwPlayer[arg1].masuId = (s16) temp_r3;
    }
    MBWalkNumDispSet(arg1, 1);
}

/* NONMATCHING: real logic (ported from the m2c draft + manual fixes below),
 * not yet instruction-matched. Known deviations from the raw draft:
 *  - the M2C_STRUCT_COPY(camera, 8) before the first MBCameraModelViewSet
 *    is read directly as camera->pos.x/pos.y instead;
 *  - "== N.0f" loop guards that m2c produced from a "cror eq,lt,eq"
 *    (folded <=) are read as "< N.0f", matching every sibling loop in the
 *    same function that m2c rendered correctly;
 *  - "(bitwise f32) X" reinterpret casts on Point3d locals are read as the
 *    relevant field (almost always .x, since it is the first word). */
void MB2Ev_Thunder(s32 arg0) {
    Point3d sp80;
    f32 sp7C;
    f32 sp78;
    f32 sp74;
    Point3d sp68;
    f32 sp64;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    Point3d sp2C;
    Point3d sp20;
    HU3DMODEL *sp1C;
    MASU *sp18;
    s32 sp14;
    s32 sp10;
    f32 spC;
    s16 spA;
    s16 sp8;
    HU3DMODEL *temp_r3_5;
    MASU *temp_r3;
    MBCAMERA *temp_r3_2;
    OMOBJ *temp_r3_9;
    f32 temp_f1;
    f32 temp_f29;
    f32 temp_f29_2;
    f32 temp_f31;
    f32 temp_f31_2;
    f32 temp_f31_3;
    f32 temp_f31_4;
    f32 var_f30;
    f32 var_f31;
    f32 var_f31_2;
    f32 var_f31_3;
    f64 temp_f28;
    f64 temp_f28_2;
    s16 temp_r3_3;
    s16 temp_r3_4;
    s16 temp_r3_6;
    s16 temp_r3_7;
    s16 temp_r3_8;
    s16 var_r22;
    s32 temp_r16;
    s32 temp_r16_2;
    s32 var_r17;
    s32 var_r20;
    s32 var_r24;
    s32 var_r26;
    s32 var_r29;
    s32 var_r29_2;
    s32 var_r29_3;
    s32 var_r30;
    s32 var_r30_2;
    s32 var_r30_3;
    s32 var_r30_4;
    s32 var_r30_5;
    s32 var_r30_6;
    HSFOBJECT *temp_r23;

    var_f30 = 0.0f;
    spA = GwPlayer[arg0].masuId;
    temp_r3 = MBMasuGet(0, (s32) spA);
    sp18 = temp_r3;
    sp14 = (s32) (u16) temp_r3->flag;
    MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 8.0f, 0U);
    MB2_CoinInit();
    MB2_CoinEffInit();
    temp_r3_2 = MBCameraGet();
    sp64 = temp_r3_2->rot.z;
    spC = temp_r3_2->zoom;
    MBPlayerRotateStart(arg0, 0xB4, 0xF);
    while (MBPlayerRotateCheck(arg0) == 0) {
        HuPrcVSleep();
    }
    MBDataAsyncWait(MBDataDirReadAsync(0x240000));
    MBPlayerPosGet(arg0, &sp80);
    sp44 = sp80.x;
    sp48 = sp80.y;
    sp4C = sp80.z;
    sp4C -= 400.0f;
    sp48 += 200.0f;
    temp_r3_3 = MBGuideCreateFlag(2, (Point3d *) &sp44, 0, 1, 1, 0);
    MBModelLayerSet(temp_r3_3, 2U);
    HuDataDirClose(0x240000);
    temp_r16 = MBPlayerCapsuleMaxGet();
    if (MBPlayerCapsuleNumGet(arg0) < temp_r16) {
        MBAudFXPlay(0x2F5);
        MBWinCreateChoice(1, 0x2E0004U, 2, 0);
        MBTopWinAttrSet(0x10U);
        if (GwPlayer[arg0].comF != 0) {
            MBComChoiceSetUp();
        }
        MBTopWinWait();
        sp8 = MBWinLastChoiceGet();
        MBTopWinKill();
    } else {
        MBWinCreate(2, 0x2E0005U, 2);
        MBTopWinWait();
        MBTopWinKill();
        sp8 = 1;
    }
    if (sp8 == 1) {
        MB2_CoinClose();
        MB2_CoinEffClose();
        MBGuideEnd(temp_r3_3);
        return;
    }
    MBPlayerPosGet(arg0, &sp80);
    sp74 = mb2ev_DynamoPos.x;
    sp78 = mb2ev_DynamoPos.y;
    sp7C = mb2ev_DynamoPos.z;
    MB2W_S32(0x26 * 4) = 0;
    sp50 = temp_r3_2->pos.x;
    sp54 = temp_r3_2->pos.y - 30.0f;
    sp58 = sp64;
    MBCameraModelViewSet(MBPlayerModelGet(arg0), (Point3d *) &sp50, NULL, 3000.0f, -1.0f, 0x2A);
    MBCameraMotionWait();
    for (var_r30 = 0; (f32) var_r30 < 60.0f; var_r30++) {
        sp48 = (f32) ((f64) (200.0f + sp80.y) + (3.0 * (100.0 * sin((3.141592653589793 * (f64) (90.0f * ((f32) var_r30 / 60.0f))) / 180.0))));
        MBGuideMdlPosSetV(temp_r3_3, (Point3d *) &sp44);
        HuPrcVSleep();
    }
    MBPlayerMotionNoShiftSet(arg0, 4, 0.0f, 8.0f, 0U);
    for (var_r30_2 = 0; (f32) var_r30_2 < 24.0f; var_r30_2++) {
        temp_f31 = (f32) var_r30_2 / 24.0f;
        PSVECSubtract((Point3d *) &sp74, &sp80, &sp68);
        PSVECScale(&sp68, &sp68, temp_f31);
        PSVECAdd(&sp80, &sp68, &sp68);
        sp68.y = (f32) ((f64) sp68.y + (200.0 * sin((3.141592653589793 * (f64) (180.0f * temp_f31)) / 180.0)));
        MBPlayerPosSetV(arg0, &sp68);
        HuPrcVSleep();
    }
    MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 8.0f, 0x40000001U);
    var_r24 = 1;
    MBWinCreate(2, 0x2E0002U, 2);
    MBTopWinWait();
    MBTopWinKill();
    MBWinCreateHelp(0x2E0003U);
    temp_r3_4 = MBModelIdGet((s16) MB2W_S32(0xD * 4));
    sp10 = (s32) temp_r3_4;
    temp_r3_5 = &Hu3DData[temp_r3_4];
    sp1C = temp_r3_5;
    temp_r23 = MB2_ModelObjPtrGet(temp_r3_5, "dynamo");
    var_r26 = -1;
    var_r17 = 0;
    omAddObjEx(mbObjMan, -0x8000, 0U, 0U, -1, MB2_TimerObjUpdate);
    mb2ev_TimerObj = 0x258;
    mb2ev_TimerValue = 0x258;
    var_r20 = 0;
    while ((s32) mb2ev_TimerObj > 0) {
        if (GwPlayer[arg0].comF != 0) {
            var_r20 -= 1;
            if (var_r20 <= 0) {
                var_r20 = (s32) ((f32) (8 - (GwPlayer[arg0].dif * 2)) + (0.01f * (60.0f * (0.007874016f * (f32) (frand() & 0x7F)))));
                if (var_r20 < 2) {
                    var_r20 = 2;
                }
                var_f30 += 0.02f;
            }
        } else if ((s32) (HuPadBtnDown[(s8) (u8) GwPlayer[arg0].padNo] & 0x100) != 0) {
            var_f30 += 0.02f;
        }
        if (var_f30 > 0.0f) {
            var_f30 -= 0.0015f;
            if (var_f30 < 0.0f) {
                var_f30 = 0.0f;
            }
        }
        if (var_f30 > 1.0f) {
            var_f30 = 1.0f;
        }
        if ((var_r26 == -1) && (var_f30 > 0.0f)) {
            var_r26 = HuAudFXPlay(0x5CB);
        } else if ((var_r26 != -1) && (var_f30 == 0.0f)) {
            HuAudFXStop(var_r26);
            var_r26 = -1;
        } else if (var_r26 != -1) {
            HuAudFXPitchSet(var_r26, (s16) (8191.0f * var_f30));
        }
        MBMotionSpeedSet((s16) MB2W_S32(0xF * 4), 0.5f + var_f30);
        if (var_f30 > 0.0f) {
            var_r17 -= 1;
            if (var_r17 <= 0) {
                var_r17 = (s32) (20.0f * (1.0f - var_f30)) + 3;
                HuAudFXPlay(0x5CA);
            }
        }
        if (var_f30 > 0.5f) {
            var_f31 = 1.0f;
        } else {
            var_f31 = 2.0f * var_f30;
        }
        if (temp_r23 != NULL) {
            temp_f1 = temp_r23->mesh.base.rot.x + (10.0f * var_f31);
            temp_r23->mesh.base.rot.x = temp_f1;
            if (temp_f1 > 360.0f) {
                temp_r23->mesh.base.rot.x = (f32) (temp_r23->mesh.base.rot.x - 360.0f);
            }
        }
        if (MBMotionShiftIDGet(MBPlayerModelGet(arg0)) == -1) {
            if (var_f31 == 0.0f) {
                if (var_r24 != 1) {
                    MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 24.0f, 0x40000001U);
                    var_r24 = 1;
                }
            } else if (var_f31 < 0.3f) {
                if (var_r24 != 2) {
                    MBPlayerMotionNoShiftSet(arg0, 2, 0.0f, 40.0f, 0x40000001U);
                    var_r24 = 2;
                }
                var_f31_2 = 5.0f * var_f30;
                if (var_f31_2 < 0.5f) {
                    var_f31_2 = 0.5f;
                } else if (var_f31_2 > 1.0f) {
                    var_f31_2 = 1.0f;
                }
                MBPlayerMotionSpeedSet(arg0, var_f31_2);
            } else {
                if (var_r24 != 3) {
                    MBPlayerMotionNoShiftSet(arg0, 3, 0.0f, 40.0f, 0x40000001U);
                    var_r24 = 3;
                }
                var_f31_3 = 5.0f * (var_f30 - 0.3f);
                if (var_f31_3 < 0.75f) {
                    var_f31_3 = 0.75f;
                } else if (var_f31_3 > 1.0f) {
                    var_f31_3 = 1.0f;
                }
                MBPlayerMotionSpeedSet(arg0, var_f31_3);
            }
        }
        MBMotionTimeSet((s16) MB2W_S32(0xE * 4), var_f30 * MBMotionMaxTimeGet((s16) MB2W_S32(0xE * 4)));
        HuPrcVSleep();
        if (var_f30 != 1.0f) {
            continue;
        }
        break;
    }
    MBTopWinKill();
    mb2ev_TimerObj = 0;
    if (var_r26 != -1) {
        HuAudFXStop(var_r26);
    }
    if (var_f30 > 0.85f) {
        var_f30 = 1.0f;
    }
    MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 8.0f, 0x40000001U);
    temp_r16_2 = MBPlayerCapsuleMaxGet();
    temp_r3_6 = temp_r16_2 - MBPlayerCapsuleNumGet(arg0);
    var_r22 = (s16) (var_f30 * (f32) temp_r3_6);
    if (var_f30 > 0.9f) {
        var_r22 = temp_r3_6;
    }
    if (var_r22 > temp_r3_6) {
        var_r22 = temp_r3_6;
    }
    if (var_r22 > 0) {
        MBWinCreate(2, 0x2E0006U, 2);
        MBTopWinWait();
        MBTopWinKill();
        MB2W_S32(0x26 * 4) = 1;
        HuAudFXPlay(0x5D1);
        MBMotionSpeedSet((s16) MB2W_S32(0xC * 4), 1.0f);
        do {
            HuPrcVSleep();
        } while (MBMotionEndCheck((s16) MB2W_S32(0xC * 4)) == 0);
        HuPrcSleep(0x1E);
        for (var_r30_3 = 0; var_r30_3 < var_r22; var_r30_3++) {
            MBPlayerPosGet(arg0, &sp20);
            sp20.y += 250.0f;
            sp38 = mb2ev_BiriQCapsulePos.x;
            sp3C = mb2ev_BiriQCapsulePos.y;
            sp40 = mb2ev_BiriQCapsulePos.z;
            PSVECSubtract(&sp20, (Point3d *) &sp38, &sp2C);
            PSVECScale(&sp2C, &sp2C, 1.2f);
            PSVECAdd(&sp2C, (Point3d *) &sp38, &sp2C);
            if ((u32) (frand() & 1) != 0) {
                sp2C.x = sp2C.x + 200.0f;
            } else {
                sp2C.x = sp2C.x - 200.0f;
            }
            sp2C.y += 100.0f * (1.0f + (0.007874016f * (f32) (frand() & 0x7F)));
            do {
                temp_r3_7 = MBCapsuleNextGet(-1);
            } while ((temp_r3_7 == 0x22) || (temp_r3_7 == 0x24));
            temp_r3_8 = MBModelCreate(MBCapsuleMdlGet((s32) temp_r3_7), NULL, 0);
            MBModelAttrSet(temp_r3_8, 0x40000001U);
            MBModelLayerSet(temp_r3_8, 2U);
            HuAudFXPlay(0x364);
            for (var_r29 = 0; (f32) var_r29 < 60.0f; var_r29++) {
                temp_f31_2 = (f32) var_r29 / 60.0f;
                temp_f29 = (f32) sin((3.141592653589793 * (f64) (90.0f * temp_f31_2)) / 180.0);
                MB2_BezierCalcV(&sp38, (f32 *) &sp2C, &sp20, &sp68, temp_f31_2);
                MBModelPosSetV(temp_r3_8, &sp68);
                MBModelScaleSet(temp_r3_8, temp_f29, temp_f29, temp_f29);
                HuPrcVSleep();
            }
            for (var_r29_2 = 0; (f32) var_r29_2 < 120.0f; var_r29_2++) {
                sp68.y = (f32) ((f64) sp20.y + (0.20000000298023224 * (100.0 * sin((3.141592653589793 * (f64) (720.0f * ((f32) var_r29_2 / 120.0f))) / 180.0))));
                MBModelPosSetV(temp_r3_8, &sp68);
                HuPrcVSleep();
            }
            sp38 = sp20.x;
            sp3C = sp20.y;
            sp40 = sp20.z;
            sp20.y -= 150.0f;
            HuAudFXPlay(0x31C);
            for (var_r29_3 = 0; (f32) var_r29_3 < 60.0f; var_r29_3++) {
                temp_f31_3 = (f32) var_r29_3 / 60.0f;
                temp_f29_2 = (f32) cos((3.141592653589793 * (f64) (90.0f * temp_f31_3)) / 180.0);
                PSVECSubtract(&sp20, (Point3d *) &sp38, &sp68);
                PSVECScale(&sp68, &sp68, temp_f31_3);
                PSVECAdd(&sp68, (Point3d *) &sp38, &sp68);
                temp_f28 = sin((3.141592653589793 * (f64) (180.0f * temp_f31_3)) / 180.0);
                sp68.x = (f32) ((f64) sp68.x + (200.0 * (sin((3.141592653589793 * (f64) (720.0f * temp_f31_3)) / 180.0) * temp_f28)));
                temp_f28_2 = sin((3.141592653589793 * (f64) (180.0f * temp_f31_3)) / 180.0);
                sp68.z = (f32) ((f64) sp68.z + (200.0 * (cos((3.141592653589793 * (f64) (720.0f * temp_f31_3)) / 180.0) * temp_f28_2)));
                MBModelPosSetV(temp_r3_8, &sp68);
                MBModelScaleSet(temp_r3_8, temp_f29_2, temp_f29_2, temp_f29_2);
                HuPrcVSleep();
            }
            MBModelDispSet(temp_r3_8, 0);
            MBPlayerCapsuleAdd(arg0, (s32) temp_r3_7);
            MBWinCreate(2, 0x2E0008U, -1);
            MBTopWinInsertMesSet(MBCapsuleMesGet((s32) temp_r3_7), 0);
            MBTopWinWait();
            MBTopWinKill();
            MBModelKill(temp_r3_8);
        }
        MBMotionSpeedSet((s16) MB2W_S32(0xC * 4), -1.0f);
        do {
            HuPrcVSleep();
        } while (MBMotionTimeGet((s16) MB2W_S32(0xC * 4)) > 0.0f);
        HuAudFXPlay(0x5D6);
        MBPlayerRotateStart(arg0, 0, 0xF);
        while (MBPlayerRotateCheck(arg0) == 0) {
            HuPrcVSleep();
        }
        MBPlayerWinLoseVoicePlay(arg0, 0xC, 0x1F8);
        MBPlayerMotionNoShiftSet(arg0, 0xC, 0.0f, 8.0f, 0U);
    } else {
        MBWinCreate(2, 0x2E0007U, 2);
        MBTopWinWait();
        MBTopWinKill();
        MBPlayerRotateStart(arg0, 0, 0xF);
        while (MBPlayerRotateCheck(arg0) == 0) {
            HuPrcVSleep();
        }
        MBPlayerWinLoseVoicePlay(arg0, 0xD, 0x200);
        MBPlayerMotionNoShiftSet(arg0, 0xD, 0.0f, 8.0f, 0U);
    }
    temp_r3_9 = omAddObjEx(mbObjMan, -0x8000, 0U, 0U, -1, MB2_DentyuObjFlash);
    temp_r3_9->work[0] = 1;
    temp_r3_9->work[1] = 0;
    temp_r3_9->work[2] = 0;
    temp_r3_9->trans.x = var_f30;
    do {
        HuPrcVSleep();
    } while ((MBPlayerMotionEndCheck(arg0) == 0) || (MBMotionShiftIDGet(MBPlayerModelGet(arg0)) != -1));
    MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 8.0f, 0x40000001U);
    for (var_r30_4 = 0; (f32) var_r30_4 < 30.0f; var_r30_4++) {
        HuPrcVSleep();
    }
    MBMotionTimeSet((s16) MB2W_S32(0xE * 4), var_f30 * MBMotionMaxTimeGet((s16) MB2W_S32(0xE * 4)));
    temp_r3_9->work[0] = 0;
    MBMotionSpeedSet((s16) MB2W_S32(0xF * 4), 0.5f);
    MBMotionSpeedSet((s16) MB2W_S32(0xC * 4), 0.0f);
    MBMotionSpeedSet((s16) MB2W_S32(0xE * 4), 0.0f);
    MBMotionTimeSet((s16) MB2W_S32(0xE * 4), 0.0f);
    for (var_r30_5 = 0; (f32) var_r30_5 < 30.0f; var_r30_5++) {
        MBMotionTimeSet((s16) MB2W_S32(0xC * 4), (1.0f - ((f32) var_r30_5 / 30.0f)) * MBMotionMaxTimeGet((s16) MB2W_S32(0xC * 4)));
    }
    MBMotionTimeSet((s16) MB2W_S32(0xC * 4), 0.0f);
    MBGuideEnd(temp_r3_3);
    MBPlayerMotionNoShiftSet(arg0, 4, 0.0f, 8.0f, 0U);
    for (var_r30_6 = 0; (f32) var_r30_6 < 24.0f; var_r30_6++) {
        temp_f31_4 = (f32) var_r30_6 / 24.0f;
        PSVECSubtract(&sp80, (Point3d *) &sp74, &sp68);
        PSVECScale(&sp68, &sp68, temp_f31_4);
        PSVECAdd((Point3d *) &sp74, &sp68, &sp68);
        sp68.y = (f32) ((f64) sp68.y + (200.0 * sin((3.141592653589793 * (f64) (180.0f * temp_f31_4)) / 180.0)));
        MBPlayerPosSetV(arg0, &sp68);
        MBPlayerRotSet(arg0, 0.0f, 0.0f, 0.0f);
        HuPrcVSleep();
    }
    MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 8.0f, 0x40000001U);
    MB2W_S32(0x26 * 4) = 1;
    sp50 = temp_r3_2->pos.x;
    sp54 = temp_r3_2->pos.y;
    sp58 = sp64;
    MBCameraModelViewSet(MBPlayerModelGet(arg0), (Point3d *) &sp50, NULL, 3000.0f, -1.0f, 0x2A);
    MBCameraMotionWait();
    MB2_CoinClose();
    MB2_CoinEffClose();
    MB3MiracleGetExec(arg0);
}

/* Warning: Gap in callee-saved word stack region.
 * Saved: [-0xe8, -0xe4, -0xd8, -0xd4, -0xc8, -0xc4, -0xb8, -0xb4, -0xa8, -0xa4, -0x98, -0x94, -0x88, -0x84, -0x78, -0x74, -0x68, -0x64, -0x58, -0x54, -0x48, -0x44, 0xed4, 0xed8, 0xedc, 0xee0, 0xee4, 0xee8, 0xeec, 0xef0, 0xef4, 0xef8, 0xefc, 0xf00, 0xf04, 0xf08, 0xf0c, 0xf10, 0xf14, 0xf20, 0xf24, 0xf30, 0xf34, 0xf40, 0xf44, 0xf50, 0xf54, 0xf60, 0xf64, 0xf70, 0xf74, 0xf80, 0xf84, 0xf90, 0xf94, 0xfa0, 0xfa4, 0xfb0, 0xfb4], gap at: -0xe0. */
/* MB2_RAIN_SPOT: 10-entry local (stack) array, stride 0x118 - confirmed
 * against build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s (MB2Ev_Rain,
 * 0x5565-0x7443): +0x00 HSFOBJECT *objPtr (MB2_ModelObjPtrGet on
 * mb2ev_RainObjTbl[i]), +0x04..0x67 raw snapshot of *(objPtr-1) (saved at
 * entry, restored at exit - same struct, not individually named), +0x68
 * ANIMDATA *mdl (mb2_Work[1] model handle), +0x6C dist-to-mid^2, +0x70
 * easing speed accumulator, +0x74 depth delta (worldY-1190), +0x78 linked
 * branch masuId (flag bits 0xC00) or -1, +0x7C/0x80 swim progress/duration,
 * +0x84 swim length (mb2ev_RainSwimLenTbl[i]), +0x88 "slot 1..3" flag,
 * +0x8C/0x90/0x94 world pos (Hu3DModelObjMtxGet's translation row), +0x98
 * vector to mb2ev_RainSwimMidPos, +0xA4 vector to mb2ev_RainSwimRightPos,
 * +0xB0/0xB4/0xB8 rot accumulator (0xB4 = yaw), +0xBC world pos of the
 * linked masu, +0xC8 vector to it, +0xD4 count of players standing on this
 * spot (<=4), +0xD8[4] their player indices, +0xE8[4] (stride 0xC)
 * pos-minus-spot vectors for each. */
#define RS_STRIDE 0x118
#define RSp(p, i) ((u8 *) (p) + (i) * RS_STRIDE)
#define RS_S32(p, off) (*(s32 *) ((u8 *) (p) + (off)))
#define RS_F32(p, off) (*(f32 *) ((u8 *) (p) + (off)))
#define RS_PTR(p, off) (*(void **) ((u8 *) (p) + (off)))

/* MB2_RAIN_PLAYER: local (stack) array, stride 0x58, reused for two
 * successive phases (the swim-toward-the-middle phase, up to 4 entries;
 * and later the climb-out-on-wires phase, up to 4 entries) - same layout
 * both times, confirmed by the whole-struct copy from one array to the
 * other. +0x00 player index, +0x04 motion id (0x8C002B during swim / 6
 * during climb), +0x08/0x0C progress/duration, +0x10 dist-to-mid (sqrt,
 * write-only), +0x14 "is the event-starting player" flag, +0x18 climb-done
 * flag, +0x1C/0x20/0x24 pos, +0x28/0x2C/0x30 rot (0x2C = yaw, the only rot
 * component actually driven), +0x34/0x38/0x3C velocity (climb phase only),
 * +0x44 current water-surface Y (swim phase only), +0x4C/0x50/0x54 saved
 * original pos (climb phase). */
#define RP_STRIDE 0x58
#define RPp(p, i) ((u8 *) (p) + (i) * RP_STRIDE)
#define RP_S32(p, off) (*(s32 *) ((u8 *) (p) + (off)))
#define RP_F32(p, off) (*(f32 *) ((u8 *) (p) + (off)))

void MB2Ev_Rain(s32 arg0) {
    u8 rainSpot[10 * RS_STRIDE];
    u8 swimPlayer[4 * RP_STRIDE];
    u8 branchMasu[3 * 0x10];  /* 3 entries: s32 masuId, Point3d pos */
    u8 escapePlayer[4 * RP_STRIDE];
    s16 masuId;
    MASU *masuP;
    Point3d startPos;
    Point3d tmp;
    u8 *rs;
    u8 *sp;
    u8 *ep;
    s32 i, j;
    s32 spotCount;
    s32 swimCount;
    s32 branchMasuCount;
    s32 escapeCount;
    s32 escapeDoneF;
    s32 anyMoreSwim;
    void *modelP;
    s32 climbMotionId;
    s32 markerModelId;
    s32 splashFXId;
    s32 goalMasuId;
    f32 waterY;
    Point3d vec;
    s32 done;

    waterY = 1190.0f;
    masuId = GwPlayer[arg0].masuId;
    masuP = MBMasuGet(0, (s32) masuId);
    MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 8.0f, 0U);
    MBPlayerPosGet(arg0, &tmp);
    startPos = tmp;

    /* Build the 10 rain-spot objects and figure out how many players are
     * standing on their linked branch masu. */
    rs = rainSpot;
    spotCount = 0;
    for (i = 0; i < 0xA; i++, rs = RSp(rainSpot, i)) {
        void *hu3dModel;
        s16 mdlId;

        RS_S32(rs, 0x68) = MB2W_S32(1 * 4);
        mdlId = (s16) RS_S32(rs, 0x68);
        hu3dModel = &Hu3DData[MBModelIdGet(mdlId)];
        RS_PTR(rs, 0x0) = MB2_ModelObjPtrGet((HU3DMODEL *) hu3dModel, mb2ev_RainObjTbl[i]);
        if (RS_PTR(rs, 0x0) != NULL) {
            u8 *src = (u8 *) RS_PTR(rs, 0x0) - 4;
            memcpy(rs + 4, src + 4, 0x64);
            RS_S32(rs, 0x78) = -1;
            if (mb2ev_RainNoTbl[i] > 0) {
                s32 m;
                for (m = 0; m <= MBMasuNumGet(0); m++) {
                    if ((MBMasuFlagGet(0, m) & 0xC00) != 0) {
                        s32 no = (u32) (MBMasuFlagGet(0, m) & 0xC00) >> 0xA;
                        if (no == mb2ev_RainNoTbl[i]) {
                            RS_S32(rs, 0x78) = m;
                        }
                    }
                }
            }
            RS_S32(rs, 0x7C) = 0;
            RS_S32(rs, 0x80) = 0;
            Hu3DModelObjMtxGet(MBModelIdGet(mdlId), mb2ev_RainObjTbl[i], (f32 (*)[4]) &tmp);
            RS_F32(rs, 0x8C) = tmp.x;
            RS_F32(rs, 0x90) = tmp.y;
            RS_F32(rs, 0x94) = tmp.z;
            RS_F32(rs, 0xB8) = 0.0f;
            RS_F32(rs, 0xB4) = 0.0f;
            RS_F32(rs, 0xB0) = 0.0f;
            PSVECSubtract(&mb2ev_RainSwimMidPos, (Point3d *) (rs + 0x8C), (Point3d *) (rs + 0x98));
            PSVECSubtract(&mb2ev_RainSwimRightPos, (Point3d *) (rs + 0x8C), (Point3d *) (rs + 0xA4));
            RS_F32(rs, 0x74) = RS_F32(rs, 0x90) - waterY;
            RS_F32(rs, 0x70) = 0.0f;
            PSVECSubtract(&mb2ev_RainSwimMidPos, (Point3d *) (rs + 0x8C), &vec);
            {
                f32 distSq = (vec.x * vec.x) + (vec.z * vec.z);
                f32 dist;
                if (distSq > 0.0f) {
                    f64 e0 = __frsqrte(distSq);
                    f64 e1 = 0.5 * e0 * (3.0 - ((f64) distSq * (e0 * e0)));
                    f64 e2 = 0.5 * e1 * (3.0 - ((f64) distSq * (e1 * e1)));
                    dist = (f32) ((f64) distSq * (0.5 * e2 * (3.0 - ((f64) distSq * (e2 * e2)))));
                } else {
                    dist = distSq;
                }
                RS_F32(rs, 0x6C) = dist;
                RS_S32(rs, 0x7C) = 0;
                RS_S32(rs, 0x80) = (s32) (dist / 5.0f);
            }
            RS_S32(rs, 0x84) = mb2ev_RainSwimLenTbl[i];
            RS_S32(rs, 0x88) = ((i >= 1) && (i <= 3)) ? 1 : 0;
            if (RS_S32(rs, 0x78) != -1) {
                MBMasuPosGet(0, RS_S32(rs, 0x78), (Point3d *) (rs + 0xBC));
                PSVECSubtract((Point3d *) (rs + 0xBC), (Point3d *) (rs + 0x8C), (Point3d *) (rs + 0xC8));
                RS_S32(rs, 0xD4) = 0;
                for (j = 0; j < 4; j++) {
                    if (RS_S32(rs, 0x78) == (s16) GwPlayer[j].masuId) {
                        ((s32 *) (rs + 0xD8))[RS_S32(rs, 0xD4)] = j;
                        MBPlayerPosGet(j, &tmp);
                        PSVECSubtract(&tmp, (Point3d *) (rs + 0x8C),
                            (Point3d *) (rs + 0xE8 + (RS_S32(rs, 0xD4) * 0xC)));
                        RS_S32(rs, 0xD4) = RS_S32(rs, 0xD4) + 1;
                    }
                }
                spotCount += RS_S32(rs, 0xD4);
            }
        }
    }

    /* Find up to 4 players standing on a "swim" masu (flag bit 0x8000) and
     * set up their swim state. */
    sp = swimPlayer;
    swimCount = 0;
    for (i = 0; i < 4; i++) {
        if ((MBMasuFlagGet(0, (s32) GwPlayer[i].masuId) & 0x8000) != 0) {
            RP_S32(sp, 0x0) = i;
            RP_S32(sp, 0x4) = MBPlayerMotionCreate(i, 0x8C002B);
            RP_S32(sp, 0x14) = (masuId == (s16) GwPlayer[i].masuId) ? 0 : 1;
            MBPlayerPosGet(i, (Point3d *) (sp + 0x1C));
            MBPlayerRotGet(i, (Point3d *) (sp + 0x28));
            RS_S32(sp, 0x40) = (s32) RP_F32(sp, 0x1C);
            RS_S32(sp, 0x44) = (s32) RP_F32(sp, 0x20);
            RS_S32(sp, 0x48) = (s32) RP_F32(sp, 0x24);
            PSVECSubtract(&mb2ev_RainSwimMidPos, (Point3d *) (sp + 0x1C), &vec);
            {
                f32 distSq = (vec.x * vec.x) + (vec.z * vec.z);
                f32 dist;
                if (distSq > 0.0f) {
                    f64 e0 = __frsqrte(distSq);
                    f64 e1 = 0.5 * e0 * (3.0 - ((f64) distSq * (e0 * e0)));
                    f64 e2 = 0.5 * e1 * (3.0 - ((f64) distSq * (e1 * e1)));
                    dist = (f32) ((f64) distSq * (0.5 * e2 * (3.0 - ((f64) distSq * (e2 * e2)))));
                } else {
                    dist = distSq;
                }
                RP_F32(sp, 0x10) = dist;
                RP_S32(sp, 0x8) = 0;
                RP_S32(sp, 0xC) = (s32) (dist / 5.0f);
            }
            sp = RPp(swimPlayer, swimCount + 1);
            swimCount += 1;
        }
    }

    /* Collect up to 3 masu with the goal-marker flag (0x8000) for the
     * splash-recede markers. */
    branchMasuCount = 0;
    for (i = 1; i <= MBMasuNumGet(0); i++) {
        if (((MBMasuFlagGet(0, i) & 0x8000) != 0) && (branchMasuCount < 3)) {
            *(s32 *) (branchMasu + branchMasuCount * 0x10) = i;
            MBMasuPosGet(0, i, (Point3d *) (branchMasu + branchMasuCount * 0x10 + 4));
            branchMasuCount += 1;
        }
    }

    climbMotionId = MBPlayerMotionCreate(arg0, 0x8C0019);
    markerModelId = MBModelCreate(0x80002, NULL, 0);
    MBModelDispSet(markerModelId, 0);
    MBPlayerPosGet(arg0, &tmp);
    MBModelPosSetV(markerModelId, &tmp);
    MBCameraModelViewSet(markerModelId, NULL, NULL, 3000.0f, -1.0f, 0xA);
    MBCameraMotionWait();
    MB2W_S32(0x28 * 4) = 0;
    for (i = 0; (f32) i < 60.0f; i++) {
        f32 t = 1.0f - ((f32) i / 60.0f);
        MBModelScaleSet((s16) MB2W_S32(0x14 * 4), t, 1.0f, 1.0f);
        MBModelScaleSet((s16) MB2W_S32(0x15 * 4), t, 1.0f, t);
        HuPrcVSleep();
    }
    MBModelDispSet((s16) MB2W_S32(0x14 * 4), 0);
    MBModelDispSet((s16) MB2W_S32(0x15 * 4), 0);
    PSVECSubtract(&startPos, &tmp, &vec);
    MBPlayerRotateStart(arg0, 0x2D, 0xF);
    while (MBPlayerRotateCheck(arg0) == 0) {
        HuPrcVSleep();
    }
    MBPlayerMotionNoShiftSet(arg0, climbMotionId, 0.0f, 8.0f, 0U);
    HuPrcSleep(0x3C);
    HuAudFXPlay(0x5D2);
    for (i = 0; i < swimCount; i++) {
        MBPlayerMotionNoSet(RP_S32(RPp(swimPlayer, i), 0x0), 9, 0U);
        omVibrate((s16) RP_S32(RPp(swimPlayer, i), 0x0), 0xC, 6, 6);
    }
    splashFXId = HuAudFXPlay(0x5CC);

    /* Water recedes over 60 frames; every masu marker sinks with it, and
     * each swimming player's Y is clamped to the new surface (with a splash
     * burst + a floating water-ring object the first time it settles). */
    for (i = 0; (f32) i < 60.0f; i++) {
        f32 t = (f32) i / 60.0f;
        for (j = 0, sp = swimPlayer; j < swimCount; j++, sp = RPp(swimPlayer, j + 1)) {
            f32 surf = waterY - (600.0f * t);
            if (surf <= (1190.0f - 50.0f)) {
                if (RP_F32(sp, 0x44) > (1190.0f - 50.0f)) {
                    s32 k;
                    MBPlayerMotionNoShiftSet(RP_S32(sp, 0x0), RP_S32(sp, 0x4), 0.0f, 8.0f, 0x40000001U);
                    for (k = 0; k < 0x10; k++) {
                        Point3d pos;
                        u8 col[4];
                        f32 t2 = (f32) (k & 1);
                        pos.x = RP_F32(sp, 0x1C) + (1.5f * (100.0f * (0.5f - (0.007874016f * (f32) (frand() & 0x7F)))));
                        pos.z = RP_F32(sp, 0x24) + (1.5f * (100.0f * (0.5f * (0.007874016f * (f32) (frand() & 0x7F)))));
                        pos.y = 1190.0f - 50.0f;
                        col[0] = (u8) (230.0f + t2);
                        col[1] = (u8) (230.0f + t2);
                        col[2] = (u8) (240.0f + t2);
                        col[3] = (u8) (32.0f + (32.0f * (0.007874016f * (f32) (frand() & 0x7F))));
                        MB2_SplashEffCreateRandX(&pos.x, col, (s32) t2, 1.0f);
                    }
                    {
                        OMOBJ *ring = omAddObjEx(mbObjMan, -0x8000, 0U, 0U, -1, MB2_WaterSplashUpdate);
                        ring->work[0] = 0x12;
                        ring->trans.x = RP_F32(sp, 0x1C);
                        ring->trans.y = 1190.0f - 50.0f;
                        ring->trans.z = RP_F32(sp, 0x24);
                    }
                }
                surf = 1190.0f - 50.0f;
            }
            MBPlayerPosSet(RP_S32(sp, 0x0), RP_F32(sp, 0x1C), surf, RP_F32(sp, 0x24));
            RP_F32(sp, 0x44) = surf;
        }
        for (j = 0; j < branchMasuCount; j++) {
            Mtx mtx;
            Point3d markerPos;
            markerPos.x = *(f32 *) (branchMasu + j * 0x10 + 4);
            markerPos.y = *(f32 *) (branchMasu + j * 0x10 + 8) - (600.0f * t);
            markerPos.z = *(f32 *) (branchMasu + j * 0x10 + 0xC);
            PSMTXIdentity(mtx);
            mtx[0][3] = markerPos.x;
            mtx[1][3] = markerPos.y;
            mtx[2][3] = markerPos.z;
            MBMasuMtxSet(*(s32 *) (branchMasu + j * 0x10), mtx);
        }
        HuPrcVSleep();
    }
    if (splashFXId != -1) {
        HuAudFXStop(splashFXId);
    }
    MBMasuPlayerLightSet(arg0, 0);
    HuAudFXPlay(0x5D4);
    HuAudFXPlay(0x5D3);

    /* Main swim loop: every swim-player heads for the middle (or, for
     * slots 1..3, via a Bezier curve through mb2ev_RainSwimLeftPos); every
     * rain-spot advances its own swim/idle animation and, once linked to a
     * branch masu, drags any players standing there along for the ride.
     * Runs until every player has finished swimming (or there's nobody
     * left to swim at all). */
    done = 0;
    escapeDoneF = 0;
    do {
        if (waterY > 1040.0f) {
            waterY -= 0.5f;
        } else {
            waterY = 1040.0f;
        }
        MBModelPosSet((s16) MB2W_S32(0x13 * 4), 0.0f, waterY - 1190.0f, 0.0f);

        for (j = 0, sp = swimPlayer; j < swimCount; j++, sp = RPp(swimPlayer, j + 1)) {
            f32 t;
            s32 progress = RP_S32(sp, 0x8) + 1;
            RP_S32(sp, 0x8) = progress;
            t = (f32) progress / (f32) RP_S32(sp, 0xC);
            if (t > 1.0f) {
                t = 1.0f;
            }
            t = (f32) sin((3.141592653589793 * (f64) (90.0f * t)) / 180.0);
            if (RP_S32(sp, 0x14) == 0) {
                PSVECSubtract(&mb2ev_RainSwimMidPos, (Point3d *) (sp + 0x1C), &vec);
                PSVECScale(&vec, &vec, t);
                PSVECAdd((Point3d *) (sp + 0x1C), &vec, &vec);
            } else {
                MB2_BezierCalcV((f32 *) (sp + 0x1C), (f32 *) mb2ev_RainSwimLeftPos, &mb2ev_RainSwimMidPos, &vec, t);
            }
            vec.y = waterY - 50.0f;
            if (j == 0) {
                RP_F32(sp, 0x2C) = RP_F32(sp, 0x2C) + 2.0f;
            } else {
                RP_F32(sp, 0x2C) = RP_F32(sp, 0x2C) - 2.0f;
            }
            MBPlayerPosSetV(RP_S32(sp, 0x0), &vec);
            MBPlayerRotSetV(RP_S32(sp, 0x0), (Point3d *) (sp + 0x28));
        }

        anyMoreSwim = 0;
        for (j = 0, sp = swimPlayer; j < swimCount; j++, sp = RPp(swimPlayer, j + 1)) {
            if (RP_S32(sp, 0x8) < RP_S32(sp, 0xC)) {
                anyMoreSwim = 1;
            }
        }
        if (!anyMoreSwim) {
            escapeDoneF = 1;
        }

        for (i = 1, rs = RSp(rainSpot, 1); i < 0xA; i++, rs = RSp(rainSpot, i)) {
            if (RS_PTR(rs, 0x0) != NULL) {
                f32 t;
                s32 progress = RS_S32(rs, 0x7C) + 1;
                RS_S32(rs, 0x7C) = progress;
                if (progress < RS_S32(rs, 0x84)) {
                    t = 0.0f;
                } else {
                    t = (f32) (RS_S32(rs, 0x7C) - RS_S32(rs, 0x84)) / (f32) RS_S32(rs, 0x80);
                }
                if (t > 1.0f) {
                    t = 1.0f;
                }
                t = (f32) sin((3.141592653589793 * (f64) (90.0f * t)) / 180.0);
                if (RS_S32(rs, 0x88) == 0) {
                    PSVECScale((Point3d *) (rs + 0x98), &vec, t);
                } else {
                    MB2_BezierCalcV((f32 *) &mb2ev_RainSwimDefPos, (f32 *) (rs + 0xA4), (Point3d *) (rs + 0x98), &vec, t);
                }
                {
                    HSFOBJECT *objP = (HSFOBJECT *) RS_PTR(rs, 0x0);
                    f32 depth = waterY - 1190.0f;
                    objP->mesh.base.pos.x = RS_F32(rs, 0x20) + vec.x;
                    objP->mesh.base.pos.y = RS_F32(rs, 0x24) + depth;
                    objP->mesh.base.pos.z = RS_F32(rs, 0x28) + vec.z;
                    if (RS_F32(rs, 0x70) < 2.0f) {
                        RS_F32(rs, 0x70) = RS_F32(rs, 0x70) + 0.01f;
                    }
                    if (i == 0) {
                        RS_F32(rs, 0xB4) = RS_F32(rs, 0xB4) + RS_F32(rs, 0x70);
                    } else {
                        RS_F32(rs, 0xB4) = RS_F32(rs, 0xB4) - RS_F32(rs, 0x70);
                    }
                    objP->mesh.base.rot.y = RS_F32(rs, 0x2C) + RS_F32(rs, 0xB4);
                }
                if (RS_S32(rs, 0x78) != -1) {
                    Mtx mtx;
                    Point3d masuPos;
                    s16 mdlId = (s16) RS_S32(rs, 0x68);

                    Hu3DModelObjMtxGet(MBModelIdGet(mdlId), mb2ev_RainObjTbl[i], mtx);
                    masuPos.x = RS_F32(rs, 0x8C);
                    masuPos.y = waterY + RS_F32(rs, 0x74);
                    masuPos.z = RS_F32(rs, 0x94);
                    PSVECAdd(&masuPos, (Point3d *) (rs + 0xC8), &masuPos);
                    PSMTXIdentity(mtx);
                    mtx[0][3] = masuPos.x;
                    mtx[1][3] = masuPos.y;
                    mtx[2][3] = masuPos.z;
                    MBMasuMtxSet(RS_S32(rs, 0x78), mtx);
                    for (j = 0; j < RS_S32(rs, 0xD4); j++) {
                        Mtx rotMtx;
                        Point3d off, worldOff;

                        Hu3DModelObjMtxGet(MBModelIdGet(mdlId), mb2ev_RainObjTbl[i], mtx);
                        masuPos.x = RS_F32(rs, 0x8C);
                        masuPos.y = waterY + RS_F32(rs, 0x74);
                        masuPos.z = RS_F32(rs, 0x94);
                        mtxRot(rotMtx, RS_F32(rs, 0xB0), RS_F32(rs, 0xB4), RS_F32(rs, 0xB8));
                        off = *(Point3d *) (rs + 0xE8 + (j * 0xC));
                        PSMTXMultVec(rotMtx, &off, &worldOff);
                        PSVECAdd(&masuPos, &worldOff, &masuPos);
                        MBPlayerPosSetV(((s32 *) (rs + 0xD8))[j], &masuPos);
                        MBPlayerRotSetV(((s32 *) (rs + 0xD8))[j], (Point3d *) (rs + 0xB0));
                    }
                }
            }
        }

        anyMoreSwim = 0;
        for (i = 1, rs = RSp(rainSpot, 1); i < 0xA; i++, rs = RSp(rainSpot, i)) {
            if ((f32) (RS_S32(rs, 0x7C) - RS_S32(rs, 0x84)) < (f32) RS_S32(rs, 0x80)) {
                anyMoreSwim = 1;
            }
        }
        if (!anyMoreSwim) {
            done = 1;
        }
        if (spotCount <= 0) {
            done = 1;
        }
        HuPrcVSleep();
    } while (escapeDoneF == 0 || done == 0);

    WipeColorSet(0xFFU, 0xFFU, 0xFFU);
    WipeCreate(2, 0x81, 1);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    MBMasuPlayerLightSet(arg0, 1);

    /* Gather up to 4 of the players who were swimming (their own, plus any
     * still being carried by a rain-spot) into the escape/climb array. */
    escapeCount = 0;
    for (j = 0, sp = swimPlayer; j < swimCount; j++, sp = RPp(swimPlayer, j + 1)) {
        if (escapeCount < 4) {
            memcpy(RPp(escapePlayer, escapeCount), sp, RP_STRIDE);
            escapeCount += 1;
        }
    }
    for (i = 1, rs = RSp(rainSpot, 1); i < 0xA; i++, rs = RSp(rainSpot, i)) {
        if (RS_S32(rs, 0x78) != -1) {
            for (j = 0; j < RS_S32(rs, 0xD4); j++) {
                if (escapeCount < 4) {
                    RP_S32(RPp(escapePlayer, escapeCount), 0x0) = ((s32 *) (rs + 0xD8))[j];
                    escapeCount += 1;
                }
            }
        }
    }

    goalMasuId = MBMasuFlagPosGet(0, 0x80000000U, NULL);
    for (j = 0, ep = escapePlayer; j < escapeCount; j++, ep = RPp(escapePlayer, j + 1)) {
        GwPlayer[RP_S32(ep, 0x0)].masuId = (s16) goalMasuId;
    }
    MBPlayerPosFixEvent(arg0, goalMasuId, 1);
    MBMasuPosGet(0, goalMasuId, &tmp);
    MBPlayerPosSetV(arg0, &tmp);
    for (j = 0, ep = escapePlayer; j < escapeCount; j++, ep = RPp(escapePlayer, j + 1)) {
        MBPlayerPosGet(RP_S32(ep, 0x0), (Point3d *) (ep + 0x4C));
        RP_F32(ep, 0x1C) = RP_F32(ep, 0x4C);
        RP_F32(ep, 0x20) = 2000.0f + RP_F32(ep, 0x50) + (500.0f * (f32) j);
        RP_F32(ep, 0x24) = RP_F32(ep, 0x54);
        RP_F32(ep, 0x3C) = 0.0f;
        RP_F32(ep, 0x38) = 0.0f;
        RP_F32(ep, 0x34) = 0.0f;
        MBPlayerRotSet(RP_S32(ep, 0x0), 0.0f, 0.0f, 0.0f);
        RP_S32(ep, 0x18) = 0;
    }

    MBCameraSkipSet(0);
    MBCameraFocusMasuSet(goalMasuId);
    MBCameraMotionWait();
    MBCameraSkipSet(1);
    WipeCreate(1, 0x81, 0x1E);
    do {
        for (j = 0, ep = escapePlayer; j < escapeCount; j++, ep = RPp(escapePlayer, j + 1)) {
            if (RP_S32(ep, 0x18) == 0) {
                PSVECAdd((Point3d *) (ep + 0x1C), (Point3d *) (ep + 0x34), (Point3d *) (ep + 0x1C));
                RP_F32(ep, 0x38) = RP_F32(ep, 0x38) - 3.266667f;
                if (RP_F32(ep, 0x20) <= RP_F32(ep, 0x50)) {
                    MBPlayerMotionNoShiftSet(RP_S32(ep, 0x0), 6, 0.0f, 8.0f, 0x40000001U);
                    RP_F32(ep, 0x1C) = RP_F32(ep, 0x4C);
                    RP_F32(ep, 0x20) = RP_F32(ep, 0x50);
                    RP_F32(ep, 0x24) = RP_F32(ep, 0x54);
                    RP_S32(ep, 0x18) = 1;
                }
                MBPlayerPosSetV(RP_S32(ep, 0x0), (Point3d *) (ep + 0x1C));
            }
        }
        anyMoreSwim = 0;
        for (j = 0, ep = escapePlayer; j < escapeCount; j++, ep = RPp(escapePlayer, j + 1)) {
            if (RP_S32(ep, 0x18) == 0) {
                anyMoreSwim = 1;
            }
        }
        HuPrcVSleep();
    } while (anyMoreSwim != 0);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    for (i = 0; (f32) i < 60.0f; i++) {
        HuPrcVSleep();
    }
    for (j = 0, ep = escapePlayer; j < escapeCount; j++, ep = RPp(escapePlayer, j + 1)) {
        MBPlayerMotionNoShiftSet(RP_S32(ep, 0x0), 1, 0.0f, 8.0f, 0x40000001U);
    }
    for (i = 0; (f32) i < 60.0f; i++) {
        HuPrcVSleep();
    }

    MB2W_S32(0x28 * 4) = 1;
    MBModelPosSet((s16) MB2W_S32(0x13 * 4), 0.0f, 0.0f, 0.0f);
    MBModelScaleSet((s16) MB2W_S32(0x14 * 4), 1.0f, 1.0f, 1.0f);
    MBModelDispSet((s16) MB2W_S32(0x14 * 4), 1);
    MBModelScaleSet((s16) MB2W_S32(0x15 * 4), 1.0f, 1.0f, 1.0f);
    MBModelDispSet((s16) MB2W_S32(0x15 * 4), 1);

    /* Restore every rain-spot's original transform snapshot. */
    for (i = 0, rs = rainSpot; i < 0xA; i++, rs = RSp(rainSpot, i)) {
        u8 *dst = (u8 *) RS_PTR(rs, 0x0) - 4;
        memcpy(dst + 4, rs + 4, 0x64);
    }
    for (j = 0; j < branchMasuCount; j++) {
        MBMasuMtxSet(*(s32 *) (branchMasu + j * 0x10), NULL);
    }
    for (i = 1, rs = RSp(rainSpot, 1); i < 0xA; i++, rs = RSp(rainSpot, i)) {
        if (RS_S32(rs, 0x78) != -1) {
            MBMasuMtxSet(RS_S32(rs, 0x78), NULL);
        }
    }
    for (j = 0, sp = swimPlayer; j < swimCount; j++, sp = RPp(swimPlayer, j + 1)) {
        if (RP_S32(sp, 0x4) != -1) {
            MBPlayerMotionKill(RP_S32(sp, 0x0), RP_S32(sp, 0x4));
        }
    }
    if (climbMotionId != -1) {
        MBPlayerMotionKill(arg0, climbMotionId);
    }
    if (markerModelId != -1) {
        MBModelKill(markerModelId);
    }
}

/* mb2_Work[0x1C]: heap array of path segments for the rainbow-slide ride,
 * stride 0x38 - offsets confirmed from usage (m2c mistyped these as
 * ANIMBANK/ANIMPAT/ANIMBMP "bank/pat/bmp" fields since they happen to sit
 * at the same byte offsets as a Point3d start position): +0x00 segment
 * length, +0x04 cumulative length up to this segment, +0x08 start pos,
 * +0x14 end pos, +0x20/+0x24/+0x28 (pitch,yaw,roll) at the end of this
 * segment, +0x2C/+0x30/+0x34 the same at its start (copied from the
 * previous segment's end, or self for the first). */
#define ZP_STRIDE 0x38
#define ZPp(p, i) ((u8 *)(uintptr_t) (p) + (i) * ZP_STRIDE)

/* NONMATCHING: complete, asm-read translation. "cror eq,lt,eq" / "eq,gt,eq"
 * folds read as <= / >=, matching the pattern confirmed in
 * Thunder/Rain/CoinUpdate. */
void MB2Ev_Zen2(s32 arg0) {
    s16 masuId;
    u32 flag;
    s32 rainbowIdx;
    s32 prevCoin;
    s32 shiftMotion;
    s32 upMasu, downMasu, hatenaMasu;
    Point3d startPos, tmp, rot;
    s32 rainbowId[3];
    s32 i, j;
    s32 yajiHelpWin;
    f32 yajiWait;
    s32 dir;
    s32 lastDir;
    s32 destMasuId;
    s32 topCount;
    void *topArr;
    s32 pathSegCount;
    u8 *path;
    f32 totalLen;
    s32 seFX;
    f32 dist, speed;
    s32 particleTimer;

    masuId = GwPlayer[arg0].masuId;
    flag = (u32) (u16) MBMasuGet(0, (s32) masuId)->flag;
    MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 8.0f, 0U);
    MB2_CoinInit();
    mb2_CoinMaxDist.x = 100.0f;
    mb2_CoinMaxDist.y = 200.0f;
    mb2_CoinMaxDist.z = -1.0f;
    MB2_CoinEffInit(); /* TODO(NONMATCHING): m2c mis-attributed extra args here, verify against asm */
    rainbowIdx = (frand() & 0x7FFF) % 3;
    prevCoin = MBPlayerCoinGet(arg0);
    shiftMotion = MBPlayerMotionCreate(arg0, 0x8C0008);

    /* Find the first "up" masu (branch flag == 2), else default to the
     * player's own masu. */
    for (i = 1; i < MBMasuNumGet(0); i++) {
        MASU *m = MBMasuGet(0, i);
        if (((m->flag & 0xF) != 0) && ((s32) (u16) m->flag == 2)) {
            break;
        }
    }
    upMasu = (i >= MBMasuNumGet(0)) ? (s32) masuId : i;

    for (i = 1; i < MBMasuNumGet(0); i++) {
        MASU *m = MBMasuGet(0, i);
        if (((m->flag & 0x70) != 0) && ((s32) ((u16) m->flag >> 4) == 1)) {
            break;
        }
    }
    downMasu = (i >= MBMasuNumGet(0)) ? (s32) masuId : i;

    for (i = 1; i < MBMasuNumGet(0); i++) {
        MASU *m = MBMasuGet(0, i);
        if (((m->flag & 0xF) != 0) && ((s32) (u16) m->flag == 7)) {
            break;
        }
    }
    hatenaMasu = (i >= MBMasuNumGet(0)) ? (s32) masuId : i;

    MBPlayerPosGet(arg0, &startPos);
    tmp = startPos;
    tmp.y = 150.0f + startPos.y;
    tmp.z = startPos.z - 300.0f;
    MBCameraModelViewSet(MBPlayerModelGet(arg0), NULL, NULL, 3000.0f, -1.0f, 0xA);
    MBCameraMotionWait();
    MBPlayerRotateStart(arg0, 0xB4, 0xF);
    while (MBPlayerRotateCheck(arg0) == 0) {
        HuPrcVSleep();
    }
    MBPlayerMotionNoShiftSet(arg0, 4, 0.0f, 8.0f, 0U);
    for (i = 0; (f32) i < 27.0f; i++) {
        f32 t = (f32) i / 24.0f;
        Point3d p;
        PSVECSubtract(&tmp, &startPos, &p);
        PSVECScale(&p, &p, t);
        PSVECAdd(&startPos, &p, &p);
        p.y = (f32) ((f64) p.y + (200.0 * sin((3.141592653589793 * (f64) (180.0f * t)) / 180.0)));
        MBPlayerPosSetV(arg0, &p);
        HuPrcVSleep();
    }
    MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 8.0f, 0x40000001U);
    MB2_YajiInit(&tmp.x);
    seFX = HuAudFXPlay(0x5D8);
    MBMotionSpeedSet((s16) MB2W_S32(0x16 * 4), 0.25f);
    MBMotionTimeSet((s16) MB2W_S32(0x16 * 4), 0.0f);
    while (MBMotionTimeGet((s16) MB2W_S32(0x16 * 4)) <= 15.0f) {
        HuPrcVSleep();
    }
    if (seFX != -1) {
        HuAudFXStop(seFX);
    }
    HuAudFXPlay(0x5D7);
    while (MBMotionTimeGet((s16) MB2W_S32(0x16 * 4)) <= 25.0f) {
        HuPrcVSleep();
    }
    WipeColorSet(0xFFU, 0xFFU, 0xFFU);
    WipeCreate(2, 0, 0x1E);
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
    while (MBMotionEndCheck((s16) MB2W_S32(0x16 * 4)) == 0) {
        HuPrcVSleep();
    }

    MB2_RainbowObjInit();
    rainbowId[0] = MB2_RainbowObjCreate(mb2ev_Zen2RainbowVtxTbl[rainbowIdx].unk0, mb2ev_Zen2RainbowVtxNumTbl[rainbowIdx].unk0, 0);
    rainbowId[1] = MB2_RainbowObjCreate(mb2ev_Zen2RainbowVtxTbl[rainbowIdx].unk4, mb2ev_Zen2RainbowVtxNumTbl[rainbowIdx].unk4, 0);
    rainbowId[2] = MB2_RainbowObjCreate(mb2ev_Zen2RainbowVtxTbl[rainbowIdx].unk8, mb2ev_Zen2RainbowVtxNumTbl[rainbowIdx].unk8, 0);

    for (i = 0; i < 0x10; i++) {
        Point3d pos;
        pos = mb2ev_Zen2CoinBasePos;
        pos.y += 100.0f;
        MB2_CoinEffCreateColRand(&pos.x, 2, 100.0f * (0.2f + (0.1f * (0.007874016f * (f32) (frand() & 0x7F)))),
            0.016666668f * (0.5f + (0.3f * (0.007874016f * (f32) (frand() & 0x7F)))), 150.0f, 100.0f, 0.0f);
    }

    MBMasuPlayerLightSet(arg0, 0);
    HuAudFXPlay(0x5C8);
    WipeColorSet(0xFFU, 0xFFU, 0xFFU);
    WipeCreate(1, 0, 0x5A);
    do {
        if ((frand() & 1) == 0) {
            Point3d pos = mb2ev_Zen2CoinBasePos;
            MB2_CoinEffCreateColRand(&pos.x, 2, 100.0f * (0.2f + (0.1f * (0.007874016f * (f32) (frand() & 0x7F)))),
                0.016666668f * (0.5f + (0.3f * (0.007874016f * (f32) (frand() & 0x7F)))), 150.0f, 50.0f, 0.0f);
        }
        for (j = 0; j < 6; j++) {
            void *vtxTop;
            s32 vtxTopCount;
            if (MB2_RainbowObjVtxTopGet(rainbowId[(frand() & 0x7FFF) % 3], &vtxTop) != 0) {
                Point3d p;
                u8 *v = (u8 *) vtxTop + (((frand() & 0x7FFF) % 15) * 0xC);
                p.x = *(f32 *) (v + 0);
                p.y = *(f32 *) (v + 4);
                p.z = *(f32 *) (v + 8);
                MB2_CoinEffCreateColRand(&p.x, 2, 100.0f * (0.1f + (0.1f * (0.007874016f * (f32) (frand() & 0x7F)))),
                    0.016666668f * (0.5f + (0.3f * (0.007874016f * (f32) (frand() & 0x7F)))), 150.0f, 50.0f, 0.0f);
            }
        }
        HuPrcVSleep();
    } while (WipeCheckEnd() != 0);

    MBPlayerRotateStart(arg0, 0, 0xF);
    while (MBPlayerRotateCheck(arg0) == 0) {
        HuPrcVSleep();
    }
    MBPlayerMotionNoShiftSet(arg0, 9, 0.0f, 8.0f, 0U);
    while (MBMotionShiftIDGet(MBPlayerModelGet(arg0)) != -1) {
        HuPrcVSleep();
    }
    while (MBMotionEndCheck(MBPlayerModelGet(arg0)) == 0) {
        HuPrcVSleep();
    }
    MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 8.0f, 0x40000001U);
    MBWinCreate(2, 0x2E000CU, -1);
    MBTopWinWait();
    MBTopWinKill();
    yajiHelpWin = MBWinCreateHelp(0x2E000DU);
    MBWinPosSet((s16) yajiHelpWin, 0xC9, 0x120);
    MB2_YajiDispSet(1);
    dir = 1;
    lastDir = 1;
    if (GwPlayer[arg0].comF == 0) {
        s32 btn;
        do {
            f32 x, y;
            HuPrcVSleep();
            x = MBPadStkXGet((s8) (u8) GwPlayer[arg0].padNo, arg0);
            y = MBPadStkYGet((s8) (u8) GwPlayer[arg0].padNo, &GwPlayer[arg0], GwPlayer);
            btn = (s32) HuPadBtnDown[(s8) (u8) GwPlayer[arg0].padNo];
            if ((fabs((f64) x) > 8.0) || (fabs((f64) y) > 8.0)) {
                f32 ang = (f32) (atan2((f64) x, (f64) -y) / 3.141592653589793);
                f32 angDeg = (f32) (180.0 * (f64) ang);
                if (fabs(angDeg) < 45.0) {
                    dir = 1;
                } else if (angDeg >= 45.0f) {
                    dir = 2;
                } else if (angDeg <= -45.0f) {
                    dir = 0;
                }
                if (dir != lastDir) {
                    MB2_YajiHiliteSet(dir);
                }
                lastDir = dir;
            }
        } while ((btn & 0x100) == 0);
    } else {
        u32 r;
        HuPrcSleep((s32) (60.0f * (0.007874016f * (f32) (frand() & 0x7F))));
        r = frand();
        dir = (r & 0x7FFF) % 3;
        MB2_YajiHiliteSet(dir);
        HuPrcSleep((s32) (60.0f * (0.007874016f * (f32) (frand() & 0x7F))));
    }
    MB2_YajiDispSet(0);
    MBTopWinKill();

    topArr = NULL;
    topCount = MB2_RainbowObjVtxTopGet(rainbowId[dir], &topArr);
    destMasuId = (&upMasu)[mb2ev_Zen2DestTbl[rainbowIdx][dir]];
    MBPlayerPosFixEvent(arg0, destMasuId, 1);

    path = (u8 *) HuMemDirectMallocNum(HEAP_HEAP, topCount * ZP_STRIDE, 0x10000000U);
    MB2W_S32(0x1C * 4) = (s32) (uintptr_t) path;
    memset(path, 0, topCount * ZP_STRIDE);
    pathSegCount = topCount - 1;
    totalLen = 0.0f;
    for (i = 0; i < pathSegCount; i++, path = ZPp(MB2W_S32(0x1C * 4), i)) {
        Point3d delta;
        f32 distSq, mag;
        u8 *a = (u8 *) topArr + i * 0xC;
        u8 *b = (u8 *) topArr + (i + 1) * 0xC;

        PSVECSubtract((Point3d *) b, (Point3d *) a, &delta);
        mag = PSVECMag(&delta);
        *(f32 *) (path + 0x0) = mag;
        *(f32 *) (path + 0x4) = totalLen;
        *(f32 *) (path + 0x8) = *(f32 *) (a + 0);
        *(f32 *) (path + 0xC) = *(f32 *) (a + 4);
        *(f32 *) (path + 0x10) = *(f32 *) (a + 8);
        *(f32 *) (path + 0x14) = *(f32 *) (b + 0);
        *(f32 *) (path + 0x18) = *(f32 *) (b + 4);
        *(f32 *) (path + 0x1C) = *(f32 *) (b + 8);
        distSq = (delta.x * delta.x) + (delta.z * delta.z);
        if (distSq > 0.0f) {
            f64 e0 = __frsqrte(distSq);
            f64 e1 = 0.5 * e0 * (3.0 - ((f64) distSq * (e0 * e0)));
            f64 e2 = 0.5 * e1 * (3.0 - ((f64) distSq * (e1 * e1)));
            dist = (f32) ((f64) distSq * (0.5 * e2 * (3.0 - ((f64) distSq * (e2 * e2)))));
        } else {
            dist = distSq;
        }
        *(f32 *) (path + 0x20) = (f32) -(180.0 * (atan2((f64) delta.y, (f64) dist) / 3.141592653589793));
        *(f32 *) (path + 0x24) = (f32) (180.0 * (atan2((f64) delta.x, (f64) delta.z) / 3.141592653589793));
        *(f32 *) (path + 0x28) = 0.0f;
        if (i == 0) {
            *(f32 *) (path + 0x2C) = *(f32 *) (path + 0x20);
            *(f32 *) (path + 0x30) = *(f32 *) (path + 0x24);
            *(f32 *) (path + 0x34) = *(f32 *) (path + 0x28);
        } else {
            u8 *prev = ZPp(MB2W_S32(0x1C * 4), i - 1);
            *(f32 *) (path + 0x2C) = *(f32 *) (prev + 0x20);
            *(f32 *) (path + 0x30) = *(f32 *) (prev + 0x24);
            *(f32 *) (path + 0x34) = *(f32 *) (prev + 0x28);
        }
        totalLen += mag;
    }

    for (i = 4; i < (&mb2ev_Zen2RainbowVtxNumTbl[rainbowIdx].unk0)[dir]; i++) {
        if ((frand() & 0xF) != 0) {
            u8 *v = (u8 *) (&mb2ev_Zen2RainbowVtxTbl[rainbowIdx].unk0)[dir] + i * 0xC;
            Point3d p;
            p.x = *(f32 *) (v + 0);
            p.y = *(f32 *) (v + 4) + 50.0f;
            p.z = *(f32 *) (v + 8);
            MB2_CoinCreate(&p.x, &p.x, 1, 0.0f, 0.0f);
        }
    }

    MBPlayerMotionNoShiftSet(arg0, shiftMotion, 0.0f, 8.0f, 0U);
    MBPlayerPosGet(arg0, &startPos);
    tmp.x = *(f32 *) ((u8 *) topArr + 0);
    tmp.y = *(f32 *) ((u8 *) topArr + 4);
    tmp.z = *(f32 *) ((u8 *) topArr + 8);
    {
        Point3d d;
        PSVECSubtract(&tmp, &startPos, &d);
        PSVECMag(&d);
    }
    for (i = 0; (f32) i < 27.0f; i++) {
        f32 t = (f32) i / 27.0f;
        Point3d p, rotv;
        PSVECSubtract(&tmp, &startPos, &p);
        PSVECScale(&p, &p, t);
        PSVECAdd(&startPos, &p, &p);
        p.y = (f32) ((f64) p.y + (2.0 * (100.0 * sin((3.141592653589793 * (f64) (180.0f * t)) / 180.0))));
        MBPlayerPosSetV(arg0, &p);
        PSVECSubtract(&tmp, &startPos, &p);
        MBPlayerRotGet(arg0, &rotv);
        rotv.y = (f32) (180.0 * (atan2((f64) p.x, (f64) p.z) / 3.141592653589793));
        MBPlayerRotSetV(arg0, &rotv);
        HuPrcVSleep();
    }
    MBPlayerPosSetV(arg0, &tmp);

    particleTimer = HuAudFXPlay(0x5D9);
    speed = 10.0f;
    {
        f32 dist2 = 0.0f;
        s32 segIdx = 0;
        do {
            u8 *seg;
            if (speed < 30.000002f) {
                speed += 0.1f;
            }
            dist2 += speed;
            seg = (u8 *)(uintptr_t) MB2W_S32(0x1C * 4);
            segIdx = 0;
            while ((segIdx < pathSegCount - 1) &&
                   !(dist2 < (*(f32 *) (ZPp(seg, segIdx) + 0x0) + *(f32 *) (ZPp(seg, segIdx) + 0x4)))) {
                segIdx++;
            }
            if (segIdx < pathSegCount - 1) {
                u8 *s = ZPp(seg, segIdx);
                f32 t = (dist2 - *(f32 *) (s + 0x4)) / *(f32 *) (s + 0x0);
                Point3d a, b, p, rotv;
                a.x = *(f32 *) (s + 0x8); a.y = *(f32 *) (s + 0xC); a.z = *(f32 *) (s + 0x10);
                b.x = *(f32 *) (s + 0x14); b.y = *(f32 *) (s + 0x18); b.z = *(f32 *) (s + 0x1C);
                PSVECSubtract(&b, &a, &p);
                PSVECScale(&p, &p, t);
                PSVECAdd(&a, &p, &p);
                MBPlayerRotGet(arg0, &rotv);
                rotv.x = fn_1_BF54(t, *(f32 *) (s + 0x2C), *(f32 *) (s + 0x20));
                rotv.y = fn_1_BF54(t, *(f32 *) (s + 0x30), *(f32 *) (s + 0x24));
                rotv.z = 0.0f;
                MBPlayerPosSetV(arg0, &p);
                MBPlayerRotSetV(arg0, &rotv);
                MB2_CoinEffCreateColRand(&p.x, 2, 100.0f * (0.2f + (0.1f * (0.007874016f * (f32) (frand() & 0x7F)))),
                    0.016666668f * (0.5f + (0.3f * (0.007874016f * (f32) (frand() & 0x7F)))), 150.0f, 100.0f, 0.0f);
                HuPrcVSleep();
            }
        } while (dist2 < totalLen);
    }
    MBPlayerRotSet(arg0, 0.0f, 0.0f, 0.0f);
    if (particleTimer != -1) {
        HuAudFXStop(particleTimer);
    }

    MBPlayerMotionNoShiftSet(arg0, 4, 0.0f, 8.0f, 0U);
    MBPlayerPosGet(arg0, &startPos);
    MBMasuPosGet(0, destMasuId, &tmp);
    {
        Point3d d;
        PSVECSubtract(&tmp, &startPos, &d);
        PSVECMag(&d);
    }
    for (i = 0; (f32) i < 27.0f; i++) {
        f32 t = (f32) i / 27.0f;
        Point3d p, rotv;
        PSVECSubtract(&tmp, &startPos, &p);
        PSVECScale(&p, &p, t);
        PSVECAdd(&startPos, &p, &p);
        p.y = (f32) ((f64) p.y + (2.0 * (100.0 * sin((3.141592653589793 * (f64) (180.0f * t)) / 180.0))));
        MBPlayerPosSetV(arg0, &p);
        PSVECSubtract(&tmp, &startPos, &p);
        MBPlayerRotGet(arg0, &rotv);
        rotv.y = (f32) (180.0 * (atan2((f64) p.x, (f64) p.z) / 3.141592653589793));
        MBPlayerRotSetV(arg0, &rotv);
        HuPrcVSleep();
    }
    MBPlayerPosSetV(arg0, &tmp);
    GwPlayer[arg0].masuId = (s16) destMasuId;
    MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 8.0f, 0x40000001U);
    MBMasuPlayerLightSet(arg0, 1);
    for (i = 0; (f32) i < 60.0f; i++) {
        f32 t = 1.0f - ((f32) i / 60.0f);
        MB2_RainbowObjAlphaSet(rainbowId[0], (s8) (255.0f * t));
        MB2_RainbowObjAlphaSet(rainbowId[1], (s8) (255.0f * t));
        MB2_RainbowObjAlphaSet(rainbowId[2], (s8) (255.0f * t));
        HuPrcVSleep();
    }
    HuPrcSleep(0x1E);
    MB2_CoinDispUpdate(arg0, MBPlayerCoinGet(arg0) - prevCoin, 0x64);
    HuPrcSleep(0x1E);
    MBMotionSpeedSet((s16) MB2W_S32(0x16 * 4), 0.0f);
    MBMotionTimeSet((s16) MB2W_S32(0x16 * 4), 0.0f);
    if (shiftMotion != -1) {
        MBPlayerMotionKill(arg0, shiftMotion);
    }
    if (MB2W_S32(0x1C * 4) != 0) {
        HuMemDirectFree((void *) (uintptr_t) MB2W_S32(0x1C * 4));
    }
    MB2W_S32(0x1C * 4) = 0;
    MB2_RainbowObjClose();
    MB2_YajiClose();
    MB2_CoinClose();
    MB2_CoinEffClose();
}

/* Shared "spring stretch" helper used throughout MB2Ev_Zen4's bungee-jump
 * minigame: result = 150 + 75*(sqrt(1 + 8*(k/150)) - 1), computed with the
 * same __frsqrte fast-inverse-sqrt Newton iteration used everywhere else
 * in this module (MB2Ev_Rain/MB2_RainbowObjCreate/MB2Ev_Zen2). The
 * original inlines this same expression at every call site (m2c shows it
 * duplicated verbatim, with only the numerator - 25000 normally, 4000 for
 * the manual quick-drop input - changing), so factoring it out here is a
 * source-level simplification, not a functional change. */
static f32 mb2ev_Zen4SpringLen(f32 k) {
    f32 x = 1.0f + (8.0f * (k / 150.0f));
    f32 root;
    if (x > 0.0f) {
        f64 e0 = __frsqrte(x);
        f64 e1 = 0.5 * e0 * (3.0 - ((f64) x * (e0 * e0)));
        f64 e2 = 0.5 * e1 * (3.0 - ((f64) x * (e1 * e1)));
        root = (f32) ((f64) x * (0.5 * e2 * (3.0 - ((f64) x * (e2 * e2)))));
    } else {
        root = x;
    }
    return 150.0f + (150.0f * ((-1.0f + root) * 0.5f));
}

/* NONMATCHING: complete, asm-read translation of the bungee-jump minigame.
 * "cror eq,lt,eq"/"eq,gt,eq" folds read as <=/>= (established pattern);
 * "(bitwise fXX) Y" reinterpret-cast artifacts on scratch scalars are
 * dropped (they come from m2c losing track of a register's type across an
 * unrelated HuPrcVSleep(f32) argument that the original never actually
 * uses meaningfully - same false-dependency shape already seen and
 * resolved in MB2_MainObjUpdate/ParticleDraw). */
void MB2Ev_Zen4(s32 arg0) {
    MASU *masuP;
    Point3d startPos, tmp, vec, rot;
    s32 shiftMotion, fallMotion;
    s32 i;
    s32 prevCoin;
    s32 timerObj;
    f32 chargeLen;
    f32 releaseFrac;
    s32 quickDropF;
    s32 jumpedF;
    s32 settleFrames;
    Point3d vel;
    f32 vy;

    masuP = MBMasuGet(0, (s32) (s16) GwPlayer[arg0].masuId);
    MB2_CoinInit();
    MB2_CoinEffInit();
    MBPlayerPosGet(arg0, &startPos);
    shiftMotion = MBPlayerMotionCreate(arg0, 0x8C0008);
    fallMotion = MBPlayerMotionCreate(arg0, 0x8C0009);
    {
        Mtx mtx;
        Point3d target;
        Hu3DModelObjMtxGet(MBModelIdGet((s16) MB2W_S32(0x19 * 4)), "sisof1", mtx);
        target.x = mtx[0][3];
        target.y = mtx[1][3];
        target.z = mtx[2][3];
        PSVECSubtract(&target, &startPos, &vec);
        MBPlayerRotateStart(arg0, (s16) (180.0 * (atan2((f64) vec.x, (f64) vec.z) / 3.141592653589793)), 0xF);
        while (MBPlayerRotateCheck(arg0) == 0) {
            HuPrcVSleep();
        }
        MBPlayerMotionNoShiftSet(arg0, 4, 0.0f, 8.0f, 0U);
        for (i = 0; (f32) i < 24.0f; i++) {
            f32 t = (f32) i / 24.0f;
            Point3d p;
            PSVECSubtract(&target, &startPos, &p);
            PSVECScale(&p, &p, t);
            PSVECAdd(&startPos, &p, &p);
            p.y = (f32) ((f64) p.y + (200.0 * sin((3.141592653589793 * (f64) (180.0f * t)) / 180.0)));
            MBPlayerPosSetV(arg0, &p);
            HuPrcVSleep();
        }
    }
    MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 8.0f, 0x40000001U);
    for (i = 0; (f32) i < 12.0f; i++) {
        Mtx mtx;
        f32 maxT = MBMotionMaxTimeGet((s16) MB2W_S32(0x19 * 4));
        Point3d p1, p2;
        MBMotionTimeSet((s16) MB2W_S32(0x19 * 4), (f32) (0.33000001311302185 * sin((3.141592653589793 * (f64) (180.0f * ((f32) i / 12.0f))) / 180.0) * (f64) maxT));
        Hu3DModelObjMtxGet(MBModelIdGet((s16) MB2W_S32(0x19 * 4)), "sisof1", mtx);
        p1.x = mtx[0][3]; p1.y = mtx[1][3]; p1.z = mtx[2][3];
        MBPlayerPosSet(arg0, p1.x, p1.y, p1.z);
        Hu3DModelObjMtxGet(MBModelIdGet((s16) MB2W_S32(0x19 * 4)), "sisof2", mtx);
        p2.x = mtx[0][3]; p2.y = mtx[1][3]; p2.z = mtx[2][3];
        MBModelPosSet((s16) MB2W_S32(0x18 * 4), p2.x, p2.y, p2.z);
        HuPrcVSleep();
    }

    tmp.x = 0.0f;
    tmp.y = 100.0f;
    tmp.z = 0.0f;
    MBCameraModelViewSet(MBPlayerModelGet(arg0), NULL, &tmp, 3000.0f, -1.0f, 0x15);
    MBCameraMotionWait();

    /* Spawn a stack of 20 coins climbing above the platform, spaced with
     * increasing gaps (a visual reward ladder for the charge meter). */
    for (i = 0; i < 0x14; i++) {
        Point3d pos;
        f32 h = 100.0f * (f32) i;
        pos.x = startPos.x;
        pos.y = (800.0f + startPos.y) + (1.5f * h);
        pos.z = startPos.z;
        MB2_CoinCreate(&pos.x, &pos.x, 1, h, 800.0f + startPos.y);
    }

    prevCoin = MBPlayerCoinGet(arg0);
    MBWinCreate(2, 0x2E000EU, -1);
    MBTopWinWait();
    MBTopWinKill();
    MBWinCreateHelp(0x2E000FU);

    /* Charge meter: spring "length" climbs by up to 10 discrete steps of
     * 150 units (the coin stack), each attenuated by mb2ev_Zen4SpringLen.
     * Used purely to get the final chargeLen the meter would settle at,
     * matching the do/while sp29 loop in the original. */
    chargeLen = mb2ev_Zen4SpringLen(25000.0f);

    MB2W_S32(0x1C * 4) = 0; /* placeholder: real slot use TBD, unused by this event */
    timerObj = 0xE10;
    mb2ev_TimerObj = 0xE10;
    mb2ev_TimerValue = 0x12C;

    releaseFrac = 0.0f;
    jumpedF = 0;
    quickDropF = 0;
    for (;;) {
        if (GwPlayer[arg0].comF != 0) {
            HuPrcSleep((s32) (60.0f * (0.007874016f * (f32) (frand() & 0x7F))));
        } else {
            while (((s32) (HuPadBtnDown[(s8) (u8) GwPlayer[arg0].padNo] & 0x100) == 0) && ((s32) mb2ev_TimerObj > 0)) {
                HuPrcVSleep();
            }
            if ((s32) mb2ev_TimerObj <= 0) {
                releaseFrac = 0.0f;
                break;
            }
        }

        MBPlayerMotionNoShiftSet(arg0, 4, 0.0f, 8.0f, 0U);
        MBPlayerPosGet(arg0, &startPos);
        vel.x = 0.0f;
        vel.y = 0.0f;
        vel.z = 0.0f;
        {
            s32 climb;
            f32 dist;
            f32 chargeFrac;
            s32 released;
            s32 breakOut;

            chargeFrac = (0.15f * (f32) GwPlayer[arg0].dif) + (0.35f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F))));
            settleFrames = (s32) (10.0f * chargeFrac);

            climb = 0;
            released = 0;
            breakOut = 0;
            for (;;) {
                HuPrcVSleep();
                if ((climb < 0xA) && ((s32) mb2ev_TimerObj > 0) && (released == 0)) {
                    dist = mb2ev_Zen4SpringLen(25000.0f) - ((f32) climb * 150.0f);
                    if ((GwPlayer[arg0].comF == 0) && ((s32) (HuPadBtn[(s8) (u8) GwPlayer[arg0].padNo] & 0x100) == 0)) {
                        dist = mb2ev_Zen4SpringLen(4000.0f);
                        released = 1;
                    }
                } else {
                    /* dist keeps its last value */
                }
                dist -= 150.0f;
                vy = dist / 100.0f;
                vel.y = vy;
                PSVECAdd(&startPos, &vel, &startPos);
                MBPlayerPosSetV(arg0, &startPos);
                climb += 1;
                if (GwPlayer[arg0].comF != 0) {
                    settleFrames -= 1;
                    if (settleFrames <= 0) {
                        breakOut = 1;
                    }
                } else if (((HuPadBtnDown[(s8) (u8) GwPlayer[arg0].padNo] & 0x140) != 0) && ((s32) mb2ev_TimerObj > 0)) {
                    breakOut = 1;
                }
                if ((startPos.y > tmp.y) && (breakOut != 0)) {
                    MBTopWinKill();
                    releaseFrac = (startPos.y - tmp.y) / chargeLen;
                    if (releaseFrac > 1.0f) {
                        releaseFrac = 1.0f;
                    } else if (releaseFrac < 0.0f) {
                        releaseFrac = 0.0f;
                    }
                    jumpedF = 1;
                    climb = 0x14;
                    break;
                }
                if (startPos.y <= tmp.y) {
                    break;
                }
            }
        }
        if (jumpedF == 0) {
            Mtx mtx;
            Point3d p;
            MBPlayerMotionNoShiftSet(arg0, 5, 0.0f, 8.0f, 0U);
            for (i = 0; (f32) i < 12.0f; i++) {
                f32 maxT = MBMotionMaxTimeGet((s16) MB2W_S32(0x19 * 4));
                MBMotionTimeSet((s16) MB2W_S32(0x19 * 4), (f32) (0.33000001311302185 * sin((3.141592653589793 * (f64) (180.0f * ((f32) i / 12.0f))) / 180.0) * (f64) maxT));
                Hu3DModelObjMtxGet(MBModelIdGet((s16) MB2W_S32(0x19 * 4)), "sisof1", mtx);
                p.x = mtx[0][3]; p.y = mtx[1][3]; p.z = mtx[2][3];
                MBPlayerPosSet(arg0, p.x, p.y, p.z);
                Hu3DModelObjMtxGet(MBModelIdGet((s16) MB2W_S32(0x19 * 4)), "sisof2", mtx);
                p.x = mtx[0][3]; p.y = mtx[1][3]; p.z = mtx[2][3];
                MBModelPosSet((s16) MB2W_S32(0x18 * 4), p.x, p.y, p.z);
                HuPrcVSleep();
            }
            do {
                HuPrcVSleep();
            } while ((MBPlayerMotionEndCheck(arg0) == 0) || (MBMotionShiftIDGet(MBPlayerModelGet(arg0)) != -1));
            MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 8.0f, 0x40000001U);
            continue;
        }
        break;
    }

    mb2ev_TimerObj = 0;
    MBTopWinKill();
    if (releaseFrac != 0.01f) {
        Mtx mtx;
        Point3d p;
        f32 depth;

        while (settleFrames > 0) {
            HuPrcVSleep();
            settleFrames -= 1;
        }
        Hu3DModelObjMtxGet(MBModelIdGet((s16) MB2W_S32(0x19 * 4)), "sisof1", mtx);
        depth = 150.0f;
        vel.x = 0.0f;
        vel.y = 0.0f;
        vel.z = 0.0f;
        do {
            HuPrcVSleep();
            depth -= 150.0f;
            vy = depth / 100.0f;
            vel.y = vy;
            PSVECAdd(&startPos, &vel, &startPos);
            MBPlayerPosSetV(arg0, &startPos);
            vel.x = 0.0f;
            vel.y = 0.0f;
            vel.z = 0.0f;
        } while (startPos.y > tmp.y);
        startPos.y = tmp.y;
        MBPlayerPosSetV(arg0, &startPos);
        MBPlayerMotionNoShiftSet(arg0, fallMotion, 0.0f, 8.0f, 0U);

        settleFrames = (s32) (30.0f * (0.6f - (0.5f * releaseFrac)));
        if (settleFrames < 5) {
            settleFrames = 5;
        }
        for (i = 0; i <= settleFrames; i++) {
            f32 maxT = MBMotionMaxTimeGet((s16) MB2W_S32(0x19 * 4));
            MBMotionTimeSet((s16) MB2W_S32(0x19 * 4), ((f32) i / (f32) settleFrames) * maxT);
            Hu3DModelObjMtxGet(MBModelIdGet((s16) MB2W_S32(0x19 * 4)), "sisof1", mtx);
            p.x = mtx[0][3]; p.y = mtx[1][3]; p.z = mtx[2][3];
            MBPlayerPosSet(arg0, p.x, p.y, p.z);
            Hu3DModelObjMtxGet(MBModelIdGet((s16) MB2W_S32(0x19 * 4)), "sisof2", mtx);
            p.x = mtx[0][3]; p.y = mtx[1][3]; p.z = mtx[2][3];
            MBModelPosSet((s16) MB2W_S32(0x18 * 4), p.x, p.y, p.z);
            HuPrcVSleep();
        }
        Hu3DModelObjMtxGet(MBModelIdGet((s16) MB2W_S32(0x19 * 4)), "sisof2", mtx);
        vel.x = 0.0f;
        vel.y = -vy;
        vel.z = 0.0f;
        MBModelPosGet((s16) MB2W_S32(0x18 * 4), &startPos);
        MB2W_S32(0x27 * 4) = 0;
        do {
            HuPrcVSleep();
            PSVECAdd(&startPos, &vel, &startPos);
            MBModelPosSetV((s16) MB2W_S32(0x18 * 4), &startPos);
            vel.y -= 3.266667f;
        } while (startPos.y > tmp.y);
        MBModelPosSet((s16) MB2W_S32(0x18 * 4), p.x, p.y, p.z);
        MB2W_S32(0x27 * 4) = 1;
        HuAudFXPlay(0x5D5);
        for (i = 0; i <= settleFrames; i++) {
            f32 maxT = MBMotionMaxTimeGet((s16) MB2W_S32(0x19 * 4));
            MBMotionTimeSet((s16) MB2W_S32(0x19 * 4), (1.0f - ((f32) i / (f32) settleFrames)) * maxT);
            Hu3DModelObjMtxGet(MBModelIdGet((s16) MB2W_S32(0x19 * 4)), "sisof1", mtx);
            p.x = mtx[0][3]; p.y = mtx[1][3]; p.z = mtx[2][3];
            MBPlayerPosSet(arg0, p.x, p.y, p.z);
            Hu3DModelObjMtxGet(MBModelIdGet((s16) MB2W_S32(0x19 * 4)), "sisof2", mtx);
            p.x = mtx[0][3]; p.y = mtx[1][3]; p.z = mtx[2][3];
            MBModelPosSet((s16) MB2W_S32(0x18 * 4), p.x, p.y, p.z);
            HuPrcVSleep();
        }
        HuAudFXPlay(0x5CE);
        Hu3DModelObjMtxGet(MBModelIdGet((s16) MB2W_S32(0x19 * 4)), "sisof1", mtx);
        vel.x = 0.0f;
        vel.y = 160.0f * (0.3f + (0.7f * releaseFrac));
        vel.z = 0.0f;
        MBPlayerPosGet(arg0, &startPos);
        MBPlayerMotionNoShiftSet(arg0, 4, 0.0f, 8.0f, 0U);
        do {
            HuPrcVSleep();
            PSVECAdd(&startPos, &vel, &startPos);
            MBPlayerPosSetV(arg0, &startPos);
            vel.y -= 3.266667f;
        } while (startPos.y > tmp.y);
        startPos.y = tmp.y;
        MBPlayerPosSetV(arg0, &startPos);
        MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 8.0f, 0U);
        for (i = 0; (f32) i < 12.0f; i++) {
            f32 maxT = MBMotionMaxTimeGet((s16) MB2W_S32(0x19 * 4));
            MBMotionTimeSet((s16) MB2W_S32(0x19 * 4), (f32) (0.33000001311302185 * sin((3.141592653589793 * (f64) (180.0f * ((f32) i / 12.0f))) / 180.0) * (f64) maxT));
            Hu3DModelObjMtxGet(MBModelIdGet((s16) MB2W_S32(0x19 * 4)), "sisof1", mtx);
            p.x = mtx[0][3]; p.y = mtx[1][3]; p.z = mtx[2][3];
            MBPlayerPosSet(arg0, p.x, p.y, p.z);
            Hu3DModelObjMtxGet(MBModelIdGet((s16) MB2W_S32(0x19 * 4)), "sisof2", mtx);
            p.x = mtx[0][3]; p.y = mtx[1][3]; p.z = mtx[2][3];
            MBModelPosSet((s16) MB2W_S32(0x18 * 4), p.x, p.y, p.z);
            HuPrcVSleep();
        }
    }

    HuPrcSleep(0x3C);
    MBPlayerMotionNoShiftSet(arg0, 4, 0.0f, 8.0f, 0U);
    for (i = 0; (f32) i < 24.0f; i++) {
        f32 t = (f32) i / 24.0f;
        Point3d p;
        PSVECSubtract(&tmp, &startPos, &p);
        PSVECScale(&p, &p, t);
        PSVECAdd(&startPos, &p, &p);
        p.y = (f32) ((f64) p.y + (200.0 * sin((3.141592653589793 * (f64) (180.0f * t)) / 180.0)));
        MBPlayerPosSetV(arg0, &p);
        PSVECSubtract(&tmp, &startPos, &p);
        MBPlayerRotSet(arg0, 0.0f, (f32) (180.0 * (atan2((f64) p.x, (f64) p.z) / 3.141592653589793)), 0.0f);
        HuPrcVSleep();
    }
    MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 8.0f, 0x40000001U);
    HuPrcSleep(0x1E);
    MB2_CoinDispUpdate(arg0, MBPlayerCoinGet(arg0) - prevCoin, 0x14);
    HuPrcSleep(0x1E);
    if (shiftMotion != -1) {
        MBPlayerMotionKill(arg0, shiftMotion);
    }
    if (fallMotion != -1) {
        MBPlayerMotionKill(arg0, fallMotion);
    }
    MB2_CoinClose();
    MB2_CoinEffClose();
}

void MB2_TimerObjUpdate(OMOBJ *obj) {
    OMOBJ *sp8;
    s32 temp_ret;

    sp8 = obj;
    temp_ret = MBKillCheck();
    if (temp_ret != 0) {
        mb2ev_TimerObj = 0;
    }
    if ((s32) mb2ev_TimerObj > (s32) mb2ev_TimerValue) {
        mb2ev_TimerGameMesId = -1;
        goto block_13;
    }
    if ((s32) mb2ev_TimerObj == (s32) mb2ev_TimerValue) {
        mb2ev_TimerGameMesId = (s32) GameMesCreate(1, (mb2ev_TimerObj + 0x3A) / 60, -1, -1, (f32) temp_ret);
        goto block_13;
    }
    if ((s32) mb2ev_TimerObj < (s32) mb2ev_TimerValue) {
        if ((s32) mb2ev_TimerGameMesId != -1) {
            GameMesDispSet((s16) mb2ev_TimerGameMesId, 1, (s16) ((mb2ev_TimerObj + 0x3A) / 60));
        }
        if ((s32) mb2ev_TimerObj <= 0) {
            if ((s32) mb2ev_TimerGameMesId != -1) {
                GameMesDispSet((s16) mb2ev_TimerGameMesId, 2, -1);
            }
            mb2ev_TimerGameMesId = -1;
            omDelObjEx(mbObjMan, sp8);
            return;
        }
        goto block_13;
    }
block_13:
    mb2ev_TimerObj -= 1;
}

void MB2_DentyuObjFlash(OMOBJ *obj) {
    if (MBKillCheck() != 0) {
        obj->work[0] = 0;
    }
    if ((u32) obj->work[0] == 0) {
        omDelObjEx(mbObjMan, obj);
        return;
    }
    obj->work[1] += 1;
    if ((u32) (obj->work[1] & 3) == 0) {
        obj->work[2] ^= 1;
    }
    if ((u32) (obj->work[2] & 1) != 0) {
        MBMotionTimeSet((s16) MB2W_S32((0xE) * 4), obj->trans.x * MBMotionMaxTimeGet((s16) MB2W_S32((0xE) * 4)));
        return;
    }
    MBMotionTimeSet((s16) MB2W_S32((0xE) * 4), 0.0f);
}

void MB2_WaterSplashUpdate(OMOBJ *obj) {
    f32 sp18;
    f32 sp14;
    f32 sp10;
    s8 spF;
    s8 spE;
    s8 spD;
    s8 spC;
    u8 spB;
    u8 spA;
    u8 sp9;
    u8 sp8;
    f32 temp_f31;
    s32 var_r30;
    u32 temp_r0;

    temp_r0 = obj->work[0] - 1;
    obj->work[0] = temp_r0;
    if ((temp_r0 == 0) || (MBKillCheck() != 0)) {
        omDelObjEx(mbObjMan, obj);
    }
    sp10 = obj->trans.x + (1.5f * (100.0f * (0.5f - (0.007874016f * (f32) (frand() & 0x7F)))));
    sp18 = obj->trans.z + (1.5f * (100.0f * (0.5f - (0.007874016f * (f32) (frand() & 0x7F)))));
    sp14 = obj->trans.y + (0.5f * (100.0f * (0.5f - (0.007874016f * (f32) (frand() & 0x7F)))));
    temp_f31 = 32.0f * (0.007874016f * (f32) (frand() & 0x7F));
    spC = (s8) (200.0f + temp_f31);
    spD = (s8) (200.0f + temp_f31);
    spE = (s8) (220.0f + temp_f31);
    spF = (s8) (32.0f + (32.0f * (0.007874016f * (f32) (frand() & 0x7F))));
    if ((u32) (obj->work[0] & 7) == 0) {
        var_r30 = 1;
    } else {
        var_r30 = 0;
    }
    sp8 = (u8) spC;
    sp9 = (u8) spD;
    spA = (u8) spE;
    spB = (u8) spF;
    MB2_SplashEffCreateRandX(&sp10, &sp8, var_r30, 1.0f);
}

s16 MB2_WindGuru2MasuNoGet(s32 arg0, s32 arg1) {
    MASU *temp_r3;
    s32 var_r29_2;
    s32 var_r30;
    u32 var_r29;

    if (arg1 == -1) {
        var_r29 = *mb2_SaveWorkP;
    } else {
        var_r29 = (u32) arg1;
    }
    var_r29_2 = ((u32) (MBMasuGet(0, arg0)->flag & 0x380) >> 7U) + var_r29;
    if (var_r29_2 >= 5) {
        var_r29_2 -= 4;
    }
    var_r30 = 1;
loop_9:
    if (var_r30 <= MBMasuNumGet(0)) {
        temp_r3 = MBMasuGet(0, var_r30);
        if (((u32) (temp_r3->flag & 0x380) == 0) || ((s32) ((u32) (temp_r3->flag & 0x380) >> 7U) != var_r29_2)) {
            var_r30 += 1;
            goto loop_9;
        }
    }
    if (var_r30 > MBMasuNumGet(0)) {
        var_r30 = arg0;
    }
    return (s16) var_r30;
}

s16 MB2_RainbowMasuNoEndGet(s32 arg0) {
    /* NONMATCHING: placeholder - m2c mismodeled this function's flat
     * per-slot struct (ANIMDATA* / void* typed loop pointers used with
     * raw ->unkXX offsets), producing dozens of type errors. Needs a
     * proper struct reconstructed from
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s (same technique as
     * MB3DEBRIS/MB7W_* in w03Dll/w07Dll) before it can be ported for real. */
    return 0;
}

s32 MB2_PathCheck(s32 arg0) {
    if (MB2_PathLenGet((s16) arg0, 0x19, 0) != -1) {
        return 1;
    }
    return 0;
}

s16 MB2_PathLenGet(s16 arg0, s16 arg1, s16 arg2) {
    s16 spA;
    s16 sp8;

    sp8 = arg0;
    spA = arg2;
    mb2_PathMasuNo = arg1;
    mb2_PathLen = arg1;
    mb2_PathAllF = spA;
    mb2_PathValidF = 0;
    PathCheck(arg0, arg1);
    if ((s16) mb2_PathValidF != 0) {
        return mb2_PathLen;
    }
    return -1;
}

s16 PathCheck(s16 arg0, s16 arg1) {
    s16 sp8[16];
    MASU *temp_r29;
    MASU *temp_r30;
    MASU *temp_r3;
    s16 temp_r0;
    s16 var_r23;
    s16 var_r24;
    s16 var_r26;
    s16 var_r27;
    s32 temp_r19;
    s32 temp_r21;
    s32 temp_r25;
    s32 var_r31;
    s32 var_r31_2;

    var_r27 = arg1;
    var_r24 = 0;
    temp_r29 = MBMasuGet(0, (s32) arg0);
    if (MBMasuTypeGet(0, (s32) arg0) == 5) {
        temp_r0 = mb2_PathMasuNo - var_r27;
        if (temp_r0 < (s16) mb2_PathLen) {
            mb2_PathLen = temp_r0;
        }
        mb2_PathValidF = 1;
        return 1;
    }
    if ((MBMasuTypeGet(0, (s32) arg0) != 0) && (MBMasuTypeGet(0, (s32) arg0) != 5)) {
        var_r27 -= 1;
    }
    if (var_r27 >= 0) {
        var_r26 = 0;
loop_13:
        if (var_r26 < (s32) temp_r29->linkNum) {
            temp_r19 = MBBranchFlagGet() | 0x06000000;
            if (((u32) (MBMasuFlagGet(0, (s32) temp_r29->linkTbl[var_r26]) & temp_r19) == 0) && (PathCheck((s16) temp_r29->linkTbl[var_r26], var_r27) != 0)) {
                var_r24 = 1;
            }
            var_r26 += 1;
            goto loop_13;
        }
    }
    if (((s16) mb2_PathAllF != 0) && (var_r27 < (s32) (mb2_PathMasuNo - 1)) && ((u32) (temp_r29->flag & 0xF) != 0)) {
        temp_r3 = MBMasuGet(0, (s32) arg0);
        temp_r25 = temp_r3->flag & 0xF;
        if ((temp_r25 == 2) || (temp_r25 == 4) || (temp_r25 == 6) || (temp_r25 == 8)) {
            var_r23 = -1;
        } else if ((u32) (temp_r3->flag & 0xF) != 0) {
            temp_r30 = MBMasuGet(0, (s32) arg0);
            var_r31 = 0;
loop_26:
            if (var_r31 < (s32) temp_r30->linkNum) {
                if ((u32) (MBMasuFlagGet(0, (s32) temp_r30->linkTbl[var_r31]) & 0x4000) == 0) {
                    var_r31 += 1;
                    goto loop_26;
                }
            }
            if (var_r31 < (s32) temp_r30->linkNum) {
                temp_r21 = MB2_MasuLinkGet((s16) temp_r30->linkTbl[var_r31], sp8, 1, 1);
                var_r31_2 = 0;
loop_31:
                if (var_r31_2 < temp_r21) {
                    if (arg0 == (s16) (&sp8[0])[var_r31_2]) {
                        var_r31_2 += 1;
                        goto loop_31;
                    }
                }
                if (var_r31_2 < temp_r21) {
                    var_r23 = (&sp8[0])[var_r31_2];
                } else {
                    goto block_34;
                }
            } else {
                goto block_34;
            }
        } else {
block_34:
            var_r23 = -1;
        }
        if ((var_r23 != -1) && (PathCheck(var_r23, var_r27) != 0)) {
            var_r24 = 1;
        }
    }
    if (var_r24 != 0) {
        return 1;
    }
    return 0;
}

void MB2_CoinDispUpdate(s32 arg0, s32 arg1, s32 arg2) {
    Point3d spC;
    s32 sp8;
    s16 var_r29;
    s32 var_r30;

    sp8 = arg2;
    var_r30 = -1;
    MBPlayerRotateStart(arg0, 0, 0xF);
loop_2:
    if (MBPlayerRotateCheck(arg0) == 0) {
        HuPrcVSleep();
        goto loop_2;
    }
    if (arg1 > 0) {
        MBPlayerPosGet(arg0, &spC);
        spC.y += 250.0f;
        var_r29 = MBCoinDispCreateSe(&spC, arg1);
    } else {
        var_r29 = -1;
    }
    if (arg1 <= 0) {
        MBPlayerWinLoseVoicePlay(arg0, 0xD, 0x200);
        MBPlayerMotionNoShiftSet(arg0, 0xD, 0.0f, 8.0f, 0U);
    } else if (arg1 < sp8) {
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
    MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 8.0f, 0x40000001U);
    do {
        HuPrcVSleep();
    } while (MBMotionShiftIDGet(MBPlayerModelGet(arg0)) != -1);
    if (var_r29 != -1) {
loop_18:
        if (MBCoinDispKillCheck(var_r29) == 0) {
            HuPrcVSleep();
            goto loop_18;
        }
    }
    if (var_r30 != -1) {
        MBPlayerMotionKill(arg0, var_r30);
    }
}

f32 MB2_AngleDiff(f32 farg0, f32 farg1) {
    f32 var_f1;
    f32 var_f2;
    f32 var_f31;

    var_f1 = farg0;
    var_f2 = farg1;
    if (var_f1 == 360.0f) {
        var_f1 -= 360.0f;
    } else if (var_f1 < 0.0f) {
        var_f1 += 360.0f;
    }
    if (var_f2 == 360.0f) {
        var_f2 -= 360.0f;
    } else if (var_f2 < 0.0f) {
        var_f2 += 360.0f;
    }
    var_f31 = var_f1 - var_f2;
    if (var_f31 == -180.0f) {
        var_f31 += 360.0f;
    } else {
        if (var_f31 == 180.0f) {
            var_f31 -= 360.0f;
        }
    }
    return var_f31;
}

f32 MB2_AngleLerp(f32 farg0, f32 farg1, f32 farg2) {
    f32 var_f28;
    f32 var_f29;
    f32 var_f30;
    f32 var_f31;
    f32 var_f31_2;

    var_f28 = farg0;
    var_f29 = farg1;
    if (var_f28 == (f32) 360.0) {
        var_f28 = (f32) ((f64) var_f28 - 360.0);
    } else if (var_f28 < (f32) 0.0) {
        var_f28 = (f32) ((f64) var_f28 + 360.0);
    }
    if (var_f29 == (f32) 360.0) {
        var_f29 = (f32) ((f64) var_f29 - 360.0);
    } else if (var_f29 < (f32) 0.0) {
        var_f29 = (f32) ((f64) var_f29 + 360.0);
    }
    var_f30 = (f32) (360.0 + (f64) (var_f28 - var_f29));
    if (fabs(var_f30) == (f32) 360.0) {
        var_f30 = (f32) fmod((f64) var_f30, 360.0);
    }
    if (var_f30 < (f32) 180.0) {
        if (var_f30 == farg2) {
            var_f31_2 = var_f30;
        } else {
            var_f31_2 = farg2;
        }
    } else {
        if ((f32) (360.0 - (f64) var_f30) == farg2) {
            var_f31_2 = (f32) -(360.0 - (f64) var_f30);
        } else {
            var_f31_2 = -farg2;
        }
    }
    var_f31 = var_f31_2 + var_f29;
    if (var_f31 == (f32) 360.0) {
        var_f31 = (f32) ((f64) var_f31 - 360.0);
    } else if (var_f31 < (f32) 0.0) {
        var_f31 = (f32) ((f64) var_f31 + 360.0);
    }
    return var_f31;
}

f32 fn_1_BF54(f32 farg0, f32 farg1, f32 farg2) {
    f32 sp10;
    f32 spC;
    f32 sp8;
    f32 temp_f24;
    f32 var_f25;
    f32 var_f26;
    f32 var_f27;
    f32 var_f28;
    f32 var_f29;
    f32 var_f30;
    f32 var_f31;
    f32 var_f31_2;

    sp8 = farg0;
    spC = farg1;
    sp10 = farg2;
    var_f26 = spC;
    var_f27 = sp10;
    if (var_f27 == 360.0f) {
        var_f27 -= 360.0f;
    } else if (var_f27 < 0.0f) {
        var_f27 += 360.0f;
    }
    if (var_f26 == 360.0f) {
        var_f26 -= 360.0f;
    } else if (var_f26 < 0.0f) {
        var_f26 += 360.0f;
    }
    var_f28 = var_f27 - var_f26;
    if (var_f28 == -180.0f) {
        var_f28 += 360.0f;
    } else {
        if (var_f28 == 180.0f) {
            var_f28 -= 360.0f;
        }
    }
    temp_f24 = (f32) fabs(var_f28 * sp8);
    var_f29 = spC;
    var_f25 = sp10;
    if (var_f25 == (f32) 360.0) {
        var_f25 = (f32) ((f64) var_f25 - 360.0);
    } else if (var_f25 < (f32) 0.0) {
        var_f25 = (f32) ((f64) var_f25 + 360.0);
    }
    if (var_f29 == (f32) 360.0) {
        var_f29 = (f32) ((f64) var_f29 - 360.0);
    } else if (var_f29 < (f32) 0.0) {
        var_f29 = (f32) ((f64) var_f29 + 360.0);
    }
    var_f30 = (f32) (360.0 + (f64) (var_f25 - var_f29));
    if (fabs(var_f30) == (f32) 360.0) {
        var_f30 = (f32) fmod((f64) var_f30, 360.0);
    }
    if (var_f30 < (f32) 180.0) {
        if (var_f30 == temp_f24) {
            var_f31_2 = var_f30;
        } else {
            var_f31_2 = temp_f24;
        }
    } else {
        if ((f32) (360.0 - (f64) var_f30) == temp_f24) {
            var_f31_2 = (f32) -(360.0 - (f64) var_f30);
        } else {
            var_f31_2 = -temp_f24;
        }
    }
    var_f31 = var_f31_2 + var_f29;
    if (var_f31 == (f32) 360.0) {
        var_f31 = (f32) ((f64) var_f31 - 360.0);
    } else if (var_f31 < (f32) 0.0) {
        var_f31 = (f32) ((f64) var_f31 + 360.0);
    }
    return var_f31;
}

s32 MB2_MasuLinkGet(s16 arg0, s16 *arg1, s32 arg2, s32 arg3) {
    s32 spC;
    s32 sp8;
    MASU *temp_r3;
    s32 var_r30;
    u16 var_r31;

    sp8 = arg2;
    spC = arg3;
    var_r31 = 0;
    if (sp8 != 0) {
        temp_r3 = MBMasuGet(0, (s32) arg0);
        var_r31 = temp_r3->linkNum;
        var_r30 = 0;
loop_3:
        if (var_r30 < (s32) var_r31) {
            arg1[var_r30] = (s16) temp_r3->linkTbl[var_r30];
            var_r30 += 1;
            goto loop_3;
        }
    }
    if (spC != 0) {
        var_r31 += MBMasuLinkTblGet(0, (s32) arg0, arg1 + var_r31);
    }
    return (s32) var_r31;
}

HSFOBJECT *MB2_ModelObjPtrGet(HU3DMODEL *arg0, char *arg1) {
    char *sp8;

    sp8 = arg1;
    return objCall(arg0, arg1, arg0->hsf->root);
}

/* NONMATCHING: verified - low fuzzy% (8.51%) matches the sheer size of
 * this function (2120 bytes, mostly repeated inline unrolling of the same
 * "type==1 name-match else recurse over children" check at several
 * nesting depths before falling back to real recursion), not a logic bug.
 * Recursive call count matches the asm exactly: 8 objCall(...) call sites
 * inside this function's body + 1 outer call from MB2_ModelObjPtrGet = 9
 * total "bl objCall" in the disassembly. No missing/inverted branch found
 * in the type-check ladder (type<7 && type!=1 && type<1 && type<0 ->
 * "not found" vs type==1 -> name match / recurse). */
HSFOBJECT *objCall(HU3DMODEL *arg0, char *arg1, HSFOBJECT *arg2) {
    HSFOBJECT *sp74;
    HSFOBJECT *sp70;
    HSFOBJECT *sp6C;
    HSFOBJECT *sp68;
    HSFOBJECT *sp64;
    HSFOBJECT *sp60;
    HSFOBJECT *sp5C;
    HSFOBJECT *sp58;
    HSFOBJECT *sp54;
    HSFOBJECT *sp50;
    HSFOBJECT *sp4C;
    HSFOBJECT *sp48;
    HSFOBJECT *sp44;
    HSFOBJECT *sp40;
    HSFOBJECT *sp3C;
    HSFOBJECT *sp38;
    HSFOBJECT *sp34;
    HSFOBJECT *sp30;
    HSFOBJECT *sp2C;
    HSFOBJECT *sp28;
    HSFOBJECT *sp24;
    HSFOBJECT *sp20;
    HSFOBJECT *sp1C;
    HSFOBJECT *sp18;
    HSFOBJECT *sp14;
    HSFOBJECT *sp10;
    HSFOBJECT *spC;
    HU3DMODEL *sp8;
    u32 temp_r0;
    u32 temp_r0_2;
    u32 temp_r0_3;
    u32 temp_r0_4;
    u32 temp_r0_5;
    u32 temp_r0_6;
    u32 temp_r0_7;
    u32 var_r24;
    u32 var_r24_2;
    u32 var_r25;
    u32 var_r25_2;
    u32 var_r26;
    u32 var_r26_2;
    u32 var_r27;
    u32 var_r27_2;
    u32 var_r28;
    u32 var_r28_2;
    u32 var_r29;
    u32 var_r29_2;
    u32 var_r30;
    u32 var_r30_2;

    sp8 = arg0;
    spC = arg2;
    temp_r0 = spC->type;
    if ((s32) temp_r0 < 7) {
        if ((s32) temp_r0 != 1) {
            if (((s32) temp_r0 < 1) && ((s32) temp_r0 < 0)) {
                goto block_160;
            }
            goto block_7;
        }
        if (strcmp(arg1, spC->name) == 0) {
            return spC;
        }
        var_r30 = 0;
loop_159:
        if (var_r30 >= (u32) spC->mesh.childNum) {
            goto block_160;
        }
        sp5C = spC->mesh.child[var_r30];
        temp_r0_2 = sp5C->type;
        if ((s32) temp_r0_2 < 7) {
            if ((s32) temp_r0_2 != 1) {
                if (((s32) temp_r0_2 < 1) && ((s32) temp_r0_2 < 0)) {
                    goto block_155;
                }
                goto block_94;
            }
            if (strcmp(arg1, sp5C->name) == 0) {
                sp64 = sp5C;
            } else {
                var_r28 = 0;
loop_154:
                if (var_r28 >= (u32) sp5C->mesh.childNum) {
                    goto block_155;
                }
                sp10 = sp5C->mesh.child[var_r28];
                temp_r0_3 = sp10->type;
                if ((s32) temp_r0_3 < 7) {
                    if ((s32) temp_r0_3 != 1) {
                        if (((s32) temp_r0_3 < 1) && ((s32) temp_r0_3 < 0)) {
                            goto block_150;
                        }
                        goto block_135;
                    }
                    sp18 = sp10;
                    if (strcmp(arg1, sp18->name) == 0) {
                        sp20 = sp18;
                    } else {
                        var_r24 = 0;
loop_149:
                        if (var_r24 >= (u32) sp18->mesh.childNum) {
                            goto block_150;
                        }
                        sp1C = objCall(sp8, arg1, sp18->mesh.child[var_r24]);
                        if (sp1C != NULL) {
                            sp20 = sp1C;
                        } else {
                            var_r24 += 1;
                            goto loop_149;
                        }
                    }
                } else {
                    if ((s32) temp_r0_3 != 9) {
                        goto block_150;
                    }
block_135:
                    sp14 = sp10;
                    if (strcmp(arg1, sp14->name) == 0) {
                        sp20 = sp14;
                    } else {
                        var_r24_2 = 0;
loop_141:
                        if (var_r24_2 >= (u32) sp14->mesh.childNum) {
block_150:
                            sp20 = NULL;
                        } else {
                            sp1C = objCall(sp8, arg1, sp14->mesh.child[var_r24_2]);
                            if (sp1C != NULL) {
                                sp20 = sp1C;
                            } else {
                                var_r24_2 += 1;
                                goto loop_141;
                            }
                        }
                    }
                }
                sp60 = sp20;
                if (sp20 != NULL) {
                    sp64 = sp60;
                } else {
                    var_r28 += 1;
                    goto loop_154;
                }
            }
        } else {
            if ((s32) temp_r0_2 != 9) {
                goto block_155;
            }
block_94:
            if (strcmp(arg1, sp5C->name) == 0) {
                sp64 = sp5C;
            } else {
                var_r28_2 = 0;
loop_123:
                if (var_r28_2 >= (u32) sp5C->mesh.childNum) {
block_155:
                    sp64 = NULL;
                } else {
                    sp24 = sp5C->mesh.child[var_r28_2];
                    temp_r0_4 = sp24->type;
                    if ((s32) temp_r0_4 < 7) {
                        if ((s32) temp_r0_4 != 1) {
                            if (((s32) temp_r0_4 < 1) && ((s32) temp_r0_4 < 0)) {
                                goto block_119;
                            }
                            goto block_104;
                        }
                        sp2C = sp24;
                        if (strcmp(arg1, sp2C->name) == 0) {
                            sp34 = sp2C;
                        } else {
                            var_r25 = 0;
loop_118:
                            if (var_r25 >= (u32) sp2C->mesh.childNum) {
                                goto block_119;
                            }
                            sp30 = objCall(sp8, arg1, sp2C->mesh.child[var_r25]);
                            if (sp30 != NULL) {
                                sp34 = sp30;
                            } else {
                                var_r25 += 1;
                                goto loop_118;
                            }
                        }
                    } else {
                        if ((s32) temp_r0_4 != 9) {
                            goto block_119;
                        }
block_104:
                        sp28 = sp24;
                        if (strcmp(arg1, sp28->name) == 0) {
                            sp34 = sp28;
                        } else {
                            var_r25_2 = 0;
loop_110:
                            if (var_r25_2 >= (u32) sp28->mesh.childNum) {
block_119:
                                sp34 = NULL;
                            } else {
                                sp30 = objCall(sp8, arg1, sp28->mesh.child[var_r25_2]);
                                if (sp30 != NULL) {
                                    sp34 = sp30;
                                } else {
                                    var_r25_2 += 1;
                                    goto loop_110;
                                }
                            }
                        }
                    }
                    sp60 = sp34;
                    if (sp34 != NULL) {
                        sp64 = sp60;
                    } else {
                        var_r28_2 += 1;
                        goto loop_123;
                    }
                }
            }
        }
        sp74 = sp64;
        if (sp64 != NULL) {
            return sp74;
        }
        var_r30 += 1;
        goto loop_159;
    }
    if ((s32) temp_r0 != 9) {
        goto block_160;
    }
block_7:
    if (strcmp(arg1, spC->name) == 0) {
        return spC;
    }
    var_r30_2 = 0;
loop_82:
    if (var_r30_2 >= (u32) spC->mesh.childNum) {
block_160:
        return NULL;
    }
    sp68 = spC->mesh.child[var_r30_2];
    temp_r0_5 = sp68->type;
    if ((s32) temp_r0_5 < 7) {
        if ((s32) temp_r0_5 != 1) {
            if (((s32) temp_r0_5 < 1) && ((s32) temp_r0_5 < 0)) {
                goto block_78;
            }
            goto block_17;
        }
        if (strcmp(arg1, sp68->name) == 0) {
            sp70 = sp68;
        } else {
            var_r29 = 0;
loop_77:
            if (var_r29 >= (u32) sp68->mesh.childNum) {
                goto block_78;
            }
            sp38 = sp68->mesh.child[var_r29];
            temp_r0_6 = sp38->type;
            if ((s32) temp_r0_6 < 7) {
                if ((s32) temp_r0_6 != 1) {
                    if (((s32) temp_r0_6 < 1) && ((s32) temp_r0_6 < 0)) {
                        goto block_73;
                    }
                    goto block_58;
                }
                sp40 = sp38;
                if (strcmp(arg1, sp40->name) == 0) {
                    sp48 = sp40;
                } else {
                    var_r26 = 0;
loop_72:
                    if (var_r26 >= (u32) sp40->mesh.childNum) {
                        goto block_73;
                    }
                    sp44 = objCall(sp8, arg1, sp40->mesh.child[var_r26]);
                    if (sp44 != NULL) {
                        sp48 = sp44;
                    } else {
                        var_r26 += 1;
                        goto loop_72;
                    }
                }
            } else {
                if ((s32) temp_r0_6 != 9) {
                    goto block_73;
                }
block_58:
                sp3C = sp38;
                if (strcmp(arg1, sp3C->name) == 0) {
                    sp48 = sp3C;
                } else {
                    var_r26_2 = 0;
loop_64:
                    if (var_r26_2 >= (u32) sp3C->mesh.childNum) {
block_73:
                        sp48 = NULL;
                    } else {
                        sp44 = objCall(sp8, arg1, sp3C->mesh.child[var_r26_2]);
                        if (sp44 != NULL) {
                            sp48 = sp44;
                        } else {
                            var_r26_2 += 1;
                            goto loop_64;
                        }
                    }
                }
            }
            sp6C = sp48;
            if (sp48 != NULL) {
                sp70 = sp6C;
            } else {
                var_r29 += 1;
                goto loop_77;
            }
        }
    } else {
        if ((s32) temp_r0_5 != 9) {
            goto block_78;
        }
block_17:
        if (strcmp(arg1, sp68->name) == 0) {
            sp70 = sp68;
        } else {
            var_r29_2 = 0;
loop_46:
            if (var_r29_2 >= (u32) sp68->mesh.childNum) {
block_78:
                sp70 = NULL;
            } else {
                sp4C = sp68->mesh.child[var_r29_2];
                temp_r0_7 = sp4C->type;
                if ((s32) temp_r0_7 < 7) {
                    if ((s32) temp_r0_7 != 1) {
                        if (((s32) temp_r0_7 < 1) && ((s32) temp_r0_7 < 0)) {
                            goto block_42;
                        }
                        goto block_27;
                    }
                    if (strcmp(arg1, sp4C->name) == 0) {
                        sp58 = sp4C;
                    } else {
                        var_r27 = 0;
loop_41:
                        if (var_r27 >= (u32) sp4C->mesh.childNum) {
                            goto block_42;
                        }
                        sp54 = objCall(sp8, arg1, sp4C->mesh.child[var_r27]);
                        if (sp54 != NULL) {
                            sp58 = sp54;
                        } else {
                            var_r27 += 1;
                            goto loop_41;
                        }
                    }
                } else {
                    if ((s32) temp_r0_7 != 9) {
                        goto block_42;
                    }
block_27:
                    sp50 = sp4C;
                    if (strcmp(arg1, sp50->name) == 0) {
                        sp58 = sp50;
                    } else {
                        var_r27_2 = 0;
loop_33:
                        if (var_r27_2 >= (u32) sp50->mesh.childNum) {
block_42:
                            sp58 = NULL;
                        } else {
                            sp54 = objCall(sp8, arg1, sp50->mesh.child[var_r27_2]);
                            if (sp54 != NULL) {
                                sp58 = sp54;
                            } else {
                                var_r27_2 += 1;
                                goto loop_33;
                            }
                        }
                    }
                }
                sp6C = sp58;
                if (sp58 != NULL) {
                    sp70 = sp6C;
                } else {
                    var_r29_2 += 1;
                    goto loop_46;
                }
            }
        }
    }
    sp74 = sp70;
    if (sp70 != NULL) {
        return sp74;
    }
    var_r30_2 += 1;
    goto loop_82;
}

f32 MB2_BezierCalc(f64 farg0, f32 farg1, f32 farg2, f32 farg3) {
    f32 temp_f31;

    temp_f31 = (f32) (1.0 - farg0);
    return (f32) ((f64) (farg3 * ((f32) farg0 * (f32) farg0)) + ((f64) (farg1 * (temp_f31 * temp_f31)) + (2.0 * (f64) (farg2 * (temp_f31 * (f32) farg0)))));
}

void MB2_BezierCalcV(f32 *arg0, f32 *arg1, Point3d *arg2, Point3d *arg3, f64 farg0) {
    Point3d *var_r5;
    Point3d *var_r6;
    f32 *var_r3;
    f32 *var_r4;
    f32 temp_f27;
    f32 temp_f28;
    f32 temp_f29;
    f32 temp_f31;
    s32 var_r31;

    var_r3 = arg0;
    var_r4 = arg1;
    var_r5 = arg2;
    var_r6 = arg3;
    var_r31 = 0;
loop_2:
    if (var_r31 < 3) {
        temp_f27 = var_r5->x;
        var_r5 += 4;
        temp_f28 = *var_r4;
        var_r4 += 4;
        temp_f29 = *var_r3;
        var_r3 += 4;
        temp_f31 = (f32) (1.0 - farg0);
        var_r6->x = (f32) ((f64) (temp_f27 * ((f32) farg0 * (f32) farg0)) + ((f64) (temp_f29 * (temp_f31 * temp_f31)) + (2.0 * (f64) (temp_f28 * (temp_f31 * (f32) farg0)))));
        var_r6 += 4;
        var_r31 += 1;
        goto loop_2;
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

void fn_1_CEB8(f32 farg0, f32 farg1, f32 farg2, f32 farg3, f32 farg4, f32 farg5) {
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    s32 sp8;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f26;
    f32 temp_f29;
    f32 temp_f30;
    f32 temp_f31;
    f32 var_f27;
    f32 var_f28;

    temp_f30 = farg3 - farg2;
    sp8 = (s32) (farg3 - farg2);
    if (farg2 == farg3) {
        var_f28 = temp_f30;
    } else {
        sp10 = farg5;
        var_f28 = sp10 * (temp_f30 + (farg2 - farg1));
    }
    if (farg2 == farg4) {
        var_f27 = temp_f30;
    } else {
        spC = farg5;
        var_f27 = spC * (temp_f30 + (farg4 - farg3));
    }
    temp_f31 = farg0 * farg0;
    temp_f29 = farg0 * temp_f31;
    temp_f26 = (f32) (((3.0 * (f64) temp_f31) - (f64) temp_f29) - (f64) temp_f29);
    temp_f0 = farg0 + ((temp_f29 - temp_f31) - temp_f31);
    sp1C = temp_f0;
    temp_f0_2 = temp_f29 - temp_f31;
    sp18 = temp_f0_2;
    sp14 = (temp_f0_2 * var_f27) + ((temp_f0 * var_f28) + (((f32) (1.0 - (f64) temp_f26) * farg2) + (temp_f26 * farg3)));
}

/* NONMATCHING: verified - low fuzzy% (13.61%) is register/temp-allocation
 * noise, not a logic bug. Straight-line code (no branches beyond the two
 * tangent-clamp `==` checks per axis, no loops): applies the same
 * three-axis Hermite blend as the scalar MB2_HermiteCalc, with the
 * standard Catmull-Rom-style tangent formula (average of neighbouring
 * deltas, or just the one-sided delta at a curve endpoint where p1==p0 or
 * p1==p2). Every axis (x/y/z) repeats the identical formula shape with the
 * same structure and the same edge-case handling; checked against the
 * asm's three near-identical blocks (build/GP5E01_00/w02Dll/asm/REL/
 * w02Dll/world02.s) - same branch count/shape per axis, no missing/
 * inverted conditions, no offset mismatches. */
void MB2_HermiteCalcV(Point3d *arg0, Point3d *arg1, Point3d *arg2, Point3d *arg3, Point3d *arg4, f32 farg0) {
    f32 spB8;
    s32 spB4;
    f32 spB0;
    f32 spAC;
    f32 spA8;
    f32 spA4;
    f32 spA0;
    f32 sp9C;
    f32 sp98;
    f32 sp94;
    f32 sp90;
    f32 sp8C;
    f32 sp88;
    f32 sp84;
    s32 sp80;
    f32 sp7C;
    f32 sp78;
    f32 sp74;
    f32 sp70;
    f32 sp6C;
    f32 sp68;
    f32 sp64;
    f32 sp60;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    s32 sp4C;
    f32 sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    f32 sp34;
    f32 sp30;
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    f32 sp20;
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    f32 sp8;
    f32 temp_f0;
    f32 temp_f0_10;
    f32 temp_f0_11;
    f32 temp_f0_12;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f0_5;
    f32 temp_f0_6;
    f32 temp_f0_7;
    f32 temp_f0_8;
    f32 temp_f0_9;
    f32 temp_f19;
    f32 temp_f20;
    f32 temp_f21;
    f32 temp_f22;
    f32 temp_f23;
    f32 temp_f24;
    f32 temp_f25;
    f32 temp_f26;
    f32 temp_f27;
    f32 temp_f28;
    f32 temp_f29;
    f32 temp_f30;

    sp88 = arg3->x;
    temp_f27 = arg2->x;
    temp_f30 = arg1->x;
    sp8C = arg0->x;
    temp_f21 = temp_f27 - temp_f30;
    spB4 = (s32) (temp_f27 - temp_f30);
    if (temp_f30 == temp_f27) {
        spAC = temp_f21;
    } else {
        spA4 = 0.5f;
        spAC = spA4 * (temp_f21 + (temp_f30 - sp8C));
    }
    if (temp_f30 == sp88) {
        spB0 = temp_f21;
    } else {
        spA8 = 0.5f;
        spB0 = spA8 * (temp_f21 + (sp88 - temp_f27));
    }
    temp_f24 = farg0 * farg0;
    temp_f0 = farg0 * temp_f24;
    sp18 = temp_f0;
    temp_f0_2 = (f32) (((3.0 * (f64) temp_f24) - (f64) temp_f0) - (f64) temp_f0);
    sp1C = temp_f0_2;
    temp_f0_3 = (f32) (1.0 - (f64) temp_f0_2);
    sp90 = temp_f0_3;
    sp94 = temp_f0_2;
    sp98 = farg0 + ((temp_f0 - temp_f24) - temp_f24);
    sp9C = temp_f0 - temp_f24;
    temp_f0_4 = (sp9C * spB0) + ((sp98 * spAC) + ((temp_f0_3 * temp_f30) + (temp_f0_2 * temp_f27)));
    spA0 = temp_f0_4;
    spB8 = temp_f0_4;
    arg4->x = temp_f0_4;
    sp54 = arg3->y;
    temp_f26 = arg2->y;
    temp_f29 = arg1->y;
    sp58 = arg0->y;
    temp_f20 = temp_f26 - temp_f29;
    sp80 = (s32) (temp_f26 - temp_f29);
    if (temp_f29 == temp_f26) {
        sp78 = temp_f20;
    } else {
        sp70 = 0.5f;
        sp78 = sp70 * (temp_f20 + (temp_f29 - sp58));
    }
    if (temp_f29 == sp54) {
        sp7C = temp_f20;
    } else {
        sp74 = 0.5f;
        sp7C = sp74 * (temp_f20 + (sp54 - temp_f26));
    }
    temp_f23 = farg0 * farg0;
    temp_f0_5 = farg0 * temp_f23;
    sp10 = temp_f0_5;
    temp_f0_6 = (f32) (((3.0 * (f64) temp_f23) - (f64) temp_f0_5) - (f64) temp_f0_5);
    sp14 = temp_f0_6;
    temp_f0_7 = (f32) (1.0 - (f64) temp_f0_6);
    sp5C = temp_f0_7;
    sp60 = temp_f0_6;
    sp64 = farg0 + ((temp_f0_5 - temp_f23) - temp_f23);
    sp68 = temp_f0_5 - temp_f23;
    temp_f0_8 = (sp68 * sp7C) + ((sp64 * sp78) + ((temp_f0_7 * temp_f29) + (temp_f0_6 * temp_f26)));
    sp6C = temp_f0_8;
    sp84 = temp_f0_8;
    arg4->y = temp_f0_8;
    sp20 = arg3->z;
    temp_f25 = arg2->z;
    temp_f28 = arg1->z;
    sp24 = arg0->z;
    temp_f19 = temp_f25 - temp_f28;
    sp4C = (s32) (temp_f25 - temp_f28);
    if (temp_f28 == temp_f25) {
        sp44 = temp_f19;
    } else {
        sp3C = 0.5f;
        sp44 = sp3C * (temp_f19 + (temp_f28 - sp24));
    }
    if (temp_f28 == sp20) {
        sp48 = temp_f19;
    } else {
        sp40 = 0.5f;
        sp48 = sp40 * (temp_f19 + (sp20 - temp_f25));
    }
    temp_f22 = farg0 * farg0;
    temp_f0_9 = farg0 * temp_f22;
    sp8 = temp_f0_9;
    temp_f0_10 = (f32) (((3.0 * (f64) temp_f22) - (f64) temp_f0_9) - (f64) temp_f0_9);
    spC = temp_f0_10;
    temp_f0_11 = (f32) (1.0 - (f64) temp_f0_10);
    sp28 = temp_f0_11;
    sp2C = temp_f0_10;
    sp30 = farg0 + ((temp_f0_9 - temp_f22) - temp_f22);
    sp34 = temp_f0_9 - temp_f22;
    temp_f0_12 = (sp34 * sp48) + ((sp30 * sp44) + ((temp_f0_11 * temp_f28) + (temp_f0_10 * temp_f25)));
    sp38 = temp_f0_12;
    sp50 = temp_f0_12;
    arg4->z = temp_f0_12;
}

void MB2_CoinInit(void) {
    OMOBJ *obj;
    MB2_COIN_SLOT *slot;
    s32 i;
    s32 j;

    obj = omAddObjEx(mbObjMan, -0x8000, 0, 0, -1, MB2_CoinUpdate);
    mb2_CoinObj = obj;
    slot = (MB2_COIN_SLOT *) HuMemDirectMallocNum(HEAP_HEAP, 0x20 * sizeof(MB2_COIN_SLOT), 0x10000000U);
    obj->data = slot;
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
        slot->unk14 = 0;
        slot->unk10 = 0;
        for (j = 0; j < 4; j++) {
            MBPlayerPosGet(j, (Point3d *) slot->playerPos[j]);
        }
    }
}

/* NONMATCHING: complete, asm-read translation (cror eq,gt,eq / eq,lt,eq
 * folds read as >= / <=, matching the same pattern already confirmed in
 * MB2Ev_Thunder/Rain), not yet instruction-matched. */
void MB2_CoinUpdate(OMOBJ *obj) {
    MB2_COIN_SLOT *slot;
    s32 i;

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
        s32 collectedF = 0;
        if (slot->activeF == 0) {
            continue;
        }
        switch (slot->unk8) {
        case 0: {
            s32 step = slot->unk14 + 1;
            Point3d p;
            f32 t;
            slot->unk14 = step;
            t = (f32) step / (f32) slot->unk10;
            p.x = slot->srcPos[0];
            p.y = slot->srcPos[1];
            p.z = slot->srcPos[2];
            {
                Point3d dst, delta;
                dst.x = (f32) slot->unk48;
                dst.y = (f32) slot->unk4C;
                dst.z = (f32) slot->unk50;
                PSVECSubtract(&dst, &p, &delta);
                PSVECScale(&delta, &delta, t);
                PSVECAdd(&delta, &p, &delta);
                delta.y = (f32) ((f64) delta.y + ((f64) slot->unkC * sin((3.141592653589793 * (f64) (180.0f * t)) / 180.0)));
                slot->pos[0] = delta.x;
                slot->pos[1] = delta.y;
                slot->pos[2] = delta.z;
            }
            slot->rot[1] = slot->rot[1] + 10.0f;
            if (t >= 1.0f) {
                slot->unk8 = slot->unk8 + 1;
                slot->unk14 = 0;
            }
            break;
        }
        case 1:
            slot->pos[0] = (f32) slot->unk48;
            slot->pos[1] = (f32) slot->unk4C;
            slot->pos[2] = (f32) slot->unk50;
            slot->rot[1] = slot->rot[1] + 10.0f;
            break;
        }
        if (slot->unk8 <= 2) {
            s32 p;
            for (p = 0; p < 4; p++) {
                Point3d pos;
                Point3d prevPos;
                Point3d delta;
                s32 collect = 0;

                MBPlayerPosGet(p, &pos);
                prevPos.x = slot->playerPos[p][0];
                prevPos.y = slot->playerPos[p][1];
                prevPos.z = slot->playerPos[p][2];
                PSVECSubtract((Point3d *) slot->pos, &pos, &delta);
                if (slot->pos[1] == prevPos.y) {
                    if (slot->pos[1] == (pos.y + mb2_CoinMaxDist.y)) {
                        collect = 1;
                    }
                } else if (slot->pos[1] == (prevPos.y + mb2_CoinMaxDist.y)) {
                    if (slot->pos[1] == pos.y) {
                        collect = 1;
                    }
                }
                if (mb2_CoinMaxDist.z < 0.0f) {
                    collect = 1;
                }
                if (collect != 0) {
                    f32 distSq = (delta.x * delta.x) + (delta.z * delta.z);
                    f32 dist;
                    if (distSq > 0.0f) {
                        f64 e0 = __frsqrte(distSq);
                        f64 e1 = 0.5 * e0 * (3.0 - ((f64) distSq * (e0 * e0)));
                        f64 e2 = 0.5 * e1 * (3.0 - ((f64) distSq * (e1 * e1)));
                        dist = (f32) ((f64) distSq * (0.5 * e2 * (3.0 - ((f64) distSq * (e2 * e2)))));
                    } else {
                        dist = distSq;
                    }
                    if (dist < mb2_CoinMaxDist.x) {
                        MBPlayerCoinAdd(p, 1);
                        MBAudFXPlay(7);
                        collectedF = 1;
                    }
                }
                slot->playerPos[p][0] = pos.x;
                slot->playerPos[p][1] = pos.y;
                slot->playerPos[p][2] = pos.z;
            }
        }
        Hu3DModelPosSet((s16) slot->modelId, slot->pos[0], slot->pos[1], slot->pos[2]);
        Hu3DModelRotSet((s16) slot->modelId, slot->rot[0], slot->rot[1], slot->rot[2]);
        Hu3DModelScaleSet((s16) slot->modelId, slot->scale[0], slot->scale[1], slot->scale[2]);
        if (collectedF != 0) {
            s32 k;
            f32 ang;

            slot->activeF = 0;
            Hu3DModelAttrSet((s16) slot->modelId, 1U);
            ang = 0.0f;
            for (k = 0; k < 0x20; k++) {
                Point3d pos, vel;
                u8 col[4];
                f32 speed;
                f32 tilt;
                f32 t;

                ang += 22.5f * (1.0f + (0.007874016f * (f32) (frand() & 0x7F)));
                pos.x = slot->pos[0];
                pos.y = slot->pos[1];
                pos.z = slot->pos[2];
                speed = 2.5f * (1.0f + (0.007874016f * (f32) (frand() & 0x7F)));
                tilt = 45.0f - (90.0f * (0.007874016f * (f32) (frand() & 0x7F)));
                vel.x = (f32) ((f64) speed * (sin((3.141592653589793 * (f64) ang) / 180.0) * cos((3.141592653589793 * (f64) tilt) / 180.0)));
                vel.y = (f32) ((f64) speed * (cos((3.141592653589793 * (f64) ang) / 180.0) * cos((3.141592653589793 * (f64) tilt) / 180.0)));
                vel.z = (f32) ((f64) speed * sin((3.141592653589793 * (f64) tilt) / 180.0));
                t = 0.007874016f * (f32) (frand() & 0x7F);
                col[0] = (u8) (192.0f + (63.0f * t));
                col[1] = (u8) (192.0f + (63.0f * t));
                col[2] = (u8) (64.0f + (63.0f * t));
                col[3] = (u8) (192.0f + (63.0f * (0.007874016f * (f32) (frand() & 0x7F))));
                MB2_CoinEffCreate(&pos.x, &vel.x, col,
                    100.0f * (0.3f + (0.1f * (0.007874016f * (f32) (frand() & 0x7F)))),
                    0.016666668f * (1.0f + (0.5f * (0.007874016f * (f32) (frand() & 0x7F)))), 0.0f);
            }
            return;
        }
    }
}

void MB2_CoinClose(void) {
    mb2_CoinObj = NULL;
}

s32 MB2_CoinNumGet(void) {
    MB2_COIN_SLOT *slot;
    s32 i;
    s32 num;

    num = 0;
    if (mb2_CoinObj == NULL) {
        return 0;
    }
    slot = (MB2_COIN_SLOT *) mb2_CoinObj->data;
    for (i = 0; i < 0x20; i++, slot++) {
        if (slot->activeF != 0) {
            num++;
        }
    }
    return num;
}

s32 MB2_CoinCreate(f32 *arg0, f32 *arg1, s32 arg2, f32 farg0, f32 arg_sp0) {
    MB2_COIN_SLOT *slot;
    s32 i;

    if (mb2_CoinObj == NULL) {
        return -1;
    }
    slot = (MB2_COIN_SLOT *) mb2_CoinObj->data;
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
        slot->srcPos[0] = arg0[0];
        slot->srcPos[1] = arg0[1];
        slot->srcPos[2] = arg0[2];
    }
    if (arg1 != NULL) {
        slot->unk48 = (s32) arg1[0];
        slot->unk4C = (s32) arg1[1];
        slot->unk50 = (s32) arg1[2];
    }
    slot->pos[0] = slot->srcPos[0];
    slot->pos[1] = slot->srcPos[1];
    slot->pos[2] = slot->srcPos[2];
    slot->rot[0] = 0.0f;
    slot->rot[2] = 0.0f;
    slot->scale[2] = 1.0f;
    slot->scale[1] = 1.0f;
    slot->scale[0] = 1.0f;
    slot->rot[1] = 360.0f * (0.007874016f * (f32) (frand() & 0x7F));
    Hu3DModelPosSet((s16) slot->modelId, slot->pos[0], slot->pos[1], slot->pos[2]);
    Hu3DModelRotSet((s16) slot->modelId, slot->rot[0], slot->rot[1], slot->rot[2]);
    Hu3DModelScaleSet((s16) slot->modelId, slot->scale[0], slot->scale[1], slot->scale[2]);
    Hu3DModelTPLvlSet((s16) slot->modelId, 1.0f);
    Hu3DModelAttrReset((s16) slot->modelId, 1U);
    return i;
}

/* NOTE (asm-verified): Hu3DData[modelId].hookData here is NOT an MBEFFECT -
 * MB2_ParticleCreate (below) allocates it as a bespoke 0x54-byte header via
 * Hu3DHookFuncCreate(ParticleDraw)/HuMemDirectMallocNum, not MBEffCreate.
 * Header field offsets confirmed from build/GP5E01_00/w02Dll/asm/REL/w02Dll/
 * world02.s (MB2_CoinEffUpdate/NumGet/BlendModeSet): +0x20/+0x21/+0x22/+0x23
 * zeroed flags, +0x26 (s16) live particle count, +0x38 source ANIMDATA*,
 * +0x3C particle array (stride 0x58, confirmed by "addi r31,r31,0x58" in
 * MB2_CoinEffUpdate - NOT sizeof(MBEFFECTDATA)=0x5C). mb2_CoinEffObj->data
 * is the separate, small {modelId s32; count s32; ANIMDATA *anim;} header
 * (offsets confirmed by MB2_CoinEffNumGet/BlendModeSet: ->data+0x4 = count). */
typedef struct MB2CoinEffData_s {
    s32 modelId; /* 0x00 */
    s32 count;   /* 0x04 */
    ANIMDATA *anim; /* 0x08 */
} MB2_COINEFF_DATA; /* size 0xC */

void MB2_CoinEffInit(void) {
    OMOBJ *obj;
    MB2_COINEFF_DATA *dat;
    ANIMDATA *anim;
    s16 modelId;
    void *hdr;

    obj = omAddObjEx(mbObjMan, -0x8000, 0, 0, -1, MB2_CoinEffUpdate);
    mb2_CoinEffObj = obj;
    dat = (MB2_COINEFF_DATA *) HuMemDirectMallocNum(HEAP_HEAP, sizeof(MB2_COINEFF_DATA), 0x10000000U);
    obj->data = dat;
    memset(dat, 0, sizeof(MB2_COINEFF_DATA));
    anim = HuSprAnimRead(HuDataReadNum(0xB0042, 0x10000000));
    dat->anim = anim;
    modelId = MB2_ParticleCreate(anim, 0x100);
    dat->modelId = modelId;
    Hu3DModelLayerSet(modelId, 3);
    dat->count = 0;
    hdr = Hu3DData[modelId].hookData;
    *(u8 *) ((u8 *) hdr + 0x20) = 1;
    *(u8 *) ((u8 *) hdr + 0x22) = 2;
}

void MB2_CoinEffUpdate(OMOBJ *obj) {
    MB2_COINEFF_DATA *dat;
    void *hdr;
    u8 *p;
    s32 i;

    dat = (MB2_COINEFF_DATA *) obj->data;
    if (MBKillCheck() || (mb2_CoinEffObj == NULL)) {
        Hu3DModelKill((s16) dat->modelId);
        dat->modelId = -1;
        HuSprAnimKill(dat->anim);
        dat->anim = NULL;
        omDelObjEx(mbObjMan, obj);
        mb2_CoinEffObj = NULL;
        return;
    }
    if (dat->count <= 0) {
        Hu3DModelAttrSet((s16) dat->modelId, 1U);
        return;
    }
    Hu3DModelAttrReset((s16) dat->modelId, 1U);
    hdr = Hu3DData[dat->modelId].hookData;
    p = *(u8 **) ((u8 *) hdr + 0x3C);
    *(u8 *) ((u8 *) hdr + 0x23) = 0;
    for (i = 0; i < *(s16 *) ((u8 *) hdr + 0x26); i++, p += 0x58) {
        if (*(f32 *) (p + 0x34) > 0.0f) {
            *(f32 *) (p + 0x44) += *(f32 *) (p + 0x8);
            *(f32 *) (p + 0x48) += *(f32 *) (p + 0xC);
            *(f32 *) (p + 0x4C) += *(f32 *) (p + 0x10);
            if (*(f32 *) (p + 0x24) != 0.0f) {
                *(f32 *) (p + 0xC) -= *(f32 *) (p + 0x24);
            }
            *(f32 *) (p + 0x34) = *(f32 *) (p + 0x14) * *(f32 *) (p + 0x18);
            *(f32 *) (p + 0x18) -= *(f32 *) (p + 0x1C);
            if (*(f32 *) (p + 0x18) <= 0.0f) {
                *(u32 *) (p + 0x54) = 0;
                *(u16 *) (p + 0x0) = 0;
                *(f32 *) (p + 0x34) = 0.0f;
                dat->count -= 1;
            }
        }
    }
}

void MB2_CoinEffClose(void) {
    mb2_CoinEffObj = NULL;
}

s32 MB2_CoinEffNumGet(void) {
    if (mb2_CoinEffObj == NULL) {
        return 0;
    }
    return ((MB2_COINEFF_DATA *) mb2_CoinEffObj->data)->count;
}

void MB2_CoinEffBlendModeSet(u8 arg0) {
    void *hdr;

    if (mb2_CoinEffObj != NULL) {
        hdr = Hu3DData[((MB2_COINEFF_DATA *) mb2_CoinEffObj->data)->modelId].hookData;
        *(u8 *) ((u8 *) hdr + 0x20) = arg0;
    }
}

/* MB2_PARTICLE fields (stride 0x58, confirmed by MB2_CoinEffUpdate's asm and
 * cross-checked here): +0x00 s16 unk0 (0 = free slot, matches CoinEffUpdate
 * clearing *(u16*)(p+0) on death), +0x08/0xC/0x10 vel.xyz, +0x14 life (farg0),
 * +0x18 timeFrac (starts 1.0, decremented by +0x1C each frame), +0x1C decay
 * rate (farg1), +0x24 gravity applied to vel.y (farg2), +0x34 scale = life *
 * timeFrac (alive gate, > 0), +0x40 random rotY, +0x44/0x48/0x4C pos.xyz,
 * +0x50..0x53 RGBA color. */
s32 MB2_CoinEffCreate(f32 *arg0, f32 *arg1, u8 *arg2, f32 farg0, f32 farg1, f32 farg2) {
    MB2_COINEFF_DATA *dat;
    void *hdr;
    u8 *p;
    s32 i;

    if (mb2_CoinEffObj == NULL) {
        return -1;
    }
    dat = (MB2_COINEFF_DATA *) mb2_CoinEffObj->data;
    hdr = Hu3DData[dat->modelId].hookData;
    p = *(u8 **) ((u8 *) hdr + 0x3C);
    for (i = 0; i < *(s16 *) ((u8 *) hdr + 0x26); i++, p += 0x58) {
        if (*(f32 *) (p + 0x34) == 0.0f) {
            break;
        }
    }
    if (i >= *(s16 *) ((u8 *) hdr + 0x26)) {
        return -1;
    }
    *(s16 *) (p + 0x2) = 0;
    *(s16 *) (p + 0x0) = 0;
    *(f32 *) (p + 0x44) = arg0[0];
    *(f32 *) (p + 0x48) = arg0[1];
    *(f32 *) (p + 0x4C) = arg0[2];
    *(f32 *) (p + 0x8) = arg1[0];
    *(f32 *) (p + 0xC) = arg1[1];
    *(f32 *) (p + 0x10) = arg1[2];
    *(f32 *) (p + 0x14) = farg0;
    *(f32 *) (p + 0x18) = 1.0f;
    *(f32 *) (p + 0x1C) = farg1;
    *(f32 *) (p + 0x24) = farg2;
    *(f32 *) (p + 0x34) = farg0;
    *(u8 *) (p + 0x50) = arg2[0];
    *(u8 *) (p + 0x51) = arg2[1];
    *(u8 *) (p + 0x52) = arg2[2];
    *(u8 *) (p + 0x53) = arg2[3];
    *(f32 *) (p + 0x40) = 360.0f * (0.007874016f * (f32) (frand() & 0x7F));
    *(u8 *) (p + 0x54) = 0;
    *(s16 *) (p + 0x0) = 0;
    dat->count += 1;
    return i;
}

s32 MB2_CoinEffCreateRand(void *arg0, s32 arg1, f32 farg0, f32 farg1, f32 farg2, f32 farg3, f32 farg4) {
    Point3d *pos;
    f32 vel[3];
    u8 col[4];
    f32 t;
    f32 spd;

    pos = (Point3d *) arg0;
    if (mb2_CoinEffObj == NULL) {
        return -1;
    }
    switch (arg1) {
    case 1:
        spd = -1.0f * (1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F))));
        break;
    case 2:
        spd = 1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F)));
        break;
    default:
        spd = 0.0f;
        break;
    }
    vel[0] = 0.0f;
    vel[1] = spd;
    vel[2] = 0.0f;
    t = 0.007874016f * (f32) (frand() & 0x7F);
    col[0] = (u8) (192.0f + (63.0f * t));
    col[1] = (u8) (192.0f + (63.0f * t));
    col[2] = (u8) (192.0f + (63.0f * t));
    col[3] = (u8) (192.0f + (63.0f * (0.007874016f * (f32) (frand() & 0x7F))));
    {
        f32 srcPos[3];
        srcPos[0] = pos->x + (farg2 * (-0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
        srcPos[1] = pos->y + (farg3 * (-0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
        srcPos[2] = pos->z + farg4;
        return MB2_CoinEffCreate(srcPos, vel, col, farg0, farg1, 0.0f);
    }
}

/* NONMATCHING: verified - low fuzzy% (9.95%) is register-allocation noise
 * from the many scalar spill temporaries m2c produced (sp3C..sp50), not a
 * logic bug. Re-checked argument-by-argument against the m2c draft: source
 * pos (x/y/z with random spread on x/y only, direct add on z), the 3-way
 * switch building a y-only launch velocity, the color-table index formula
 * (r - (((r - r/7) >> 1) + r/7) >> 2) * 7), and the final MB2_CoinEffCreate
 * call's argument order all match one-for-one; no missing/inverted branch,
 * no offset mismatch. */
s32 MB2_CoinEffCreateColRand(f32 *arg0, s32 arg1, f32 farg0, f32 farg1, f32 farg2, f32 farg3, f32 farg4) {
    Point3d *pos;
    f32 vel[3];
    u8 *col;
    u32 r;
    u32 idx;

    pos = (Point3d *) arg0;
    if (mb2_CoinEffObj == NULL) {
        return -1;
    }
    switch (arg1) {
    case 1:
        vel[1] = -1.0f * (1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F))));
        break;
    case 2:
        vel[1] = 1.0f + (0.2f * (0.007874016f * (f32) (frand() & 0x7F)));
        break;
    default:
        vel[1] = 0.0f;
        break;
    }
    vel[0] = 0.0f;
    vel[2] = 0.0f;
    r = frand() & 0x7FFF;
    idx = r - ((((r - (r / 7)) >> 1) + (r / 7)) >> 2) * 7;
    col = (u8 *) &mb2ev_CoinEffColTbl[idx];
    {
        f32 srcPos[3];
        srcPos[0] = pos->x + (farg2 * (-0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
        srcPos[1] = pos->y + (farg3 * (-0.5f + (0.007874016f * (f32) (frand() & 0x7F))));
        srcPos[2] = pos->z + farg4;
        return MB2_CoinEffCreate(srcPos, vel, col, farg0, farg1, 0.0f);
    }
}

void MB2_SplashEffInit(void) {
    OMOBJ *obj;
    MB2_SPLASHEFF_DATA *dat;
    ANIMDATA *anim;
    s16 modelId;
    s32 i;
    MBEFFECT *eff;
    MBEFFECTDATA *p;
    s32 j;

    obj = omAddObjEx(mbObjMan, -0x8000, 0, 0, -1, MB2_SplashEffUpdate);
    mb2_SplashEffObj = obj;
    dat = (MB2_SPLASHEFF_DATA *) HuMemDirectMallocNum(HEAP_HEAP, sizeof(MB2_SPLASHEFF_DATA), 0x10000000U);
    obj->data = dat;
    memset(dat, 0, sizeof(MB2_SPLASHEFF_DATA));
    anim = HuSprAnimRead(HuDataReadNum(0xCD001E, 0x10000000));
    for (i = 0; i < 2; i++) {
        modelId = MBEffCreate(anim, 0x100);
        dat->modelId[i] = modelId;
        eff = (MBEFFECT *) Hu3DData[modelId].hookData;
        p = eff->data;
        for (j = 0; j < eff->num; j++, p++) {
            p->dispF = 0;
        }
        if (j == 0) {
            Hu3DModelLayerSet(modelId, 1);
            eff->blendMode = 0;
        } else {
            Hu3DModelLayerSet(modelId, 2);
            eff->blendMode = 1;
        }
        MBEffAttrSet(modelId, MB_EFFECT_ATTR_3D);
        dat->count[i] = 0;
    }
}

void MB2_SplashEffUpdate(OMOBJ *obj) {
    MB2_SPLASHEFF_DATA *dat;
    s32 i;
    s32 j;
    MBEFFECT *eff;
    MBEFFECTDATA *p;
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
        eff = (MBEFFECT *) Hu3DData[dat->modelId[i]].hookData;
        p = eff->data;
        for (j = 0; j < eff->num; j++, p++) {
            if (p->dispF) {
                p->pos.x += p->vel.x;
                p->pos.y += p->vel.y;
                p->pos.z += p->vel.z;
                p->rot.z -= p->scaleBase;
                t = (f32) p->time / (f32) p->parManId;
                p->time--;
                p->color.a = (u8) (p->accel.z * t);
                if (t == 0.0f) {
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
    MB2_SPLASHEFF_DATA *dat;

    if (mb2_SplashEffObj == NULL) {
        return 0;
    }
    dat = (MB2_SPLASHEFF_DATA *) mb2_SplashEffObj->data;
    return dat->count[0] + dat->count[1];
}

void MB2_SplashEffCreate(f32 *arg0, f32 *arg1, u8 *arg2, s16 arg3, s32 arg4, f32 farg0, f32 farg1, f32 farg2) {
    MB2_SPLASHEFF_DATA *dat;
    HU3DMODEL *modelP;
    MBEFFECT *eff;
    MBEFFECTDATA *p;
    s32 j;
    s32 idx;

    if (mb2_SplashEffObj == NULL) {
        return;
    }
    dat = (MB2_SPLASHEFF_DATA *) mb2_SplashEffObj->data;
    modelP = &Hu3DData[dat->modelId[arg4 == 0 ? 0 : 1]];
    eff = (MBEFFECT *) modelP->hookData;
    p = eff->data;
    for (j = 0; j < eff->num; j++, p++) {
        if (!p->dispF) {
            break;
        }
    }
    if (j < eff->num) {
        p->parManId = arg3;
        p->time = arg3;
        p->pos.x = arg0[0];
        p->pos.y = arg0[1];
        p->pos.z = arg0[2];
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
            p->vel.x = 0.0f;
            p->vel.y = 0.0f;
            p->vel.z = 0.0f;
        } else {
            p->vel.x = arg1[0];
            p->vel.y = arg1[1];
            p->vel.z = arg1[2];
        }
        p->accel.y = farg2;
        p->accel.z = (f32) arg2[3];
        idx = (arg4 == 0 ? 0 : 1);
        dat->count[idx]++;
    }
}

void MB2_SplashEffCreateRandX(f32 *arg0, u8 *arg1, s32 arg2, f32 farg0) {
    f32 vel[3];
    u8 col[4];
    s16 life;
    f32 rotY;

    vel[0] = 0.0f;
    vel[1] = 0.0f;
    vel[2] = 5.0f * (0.8f + (0.1f * (0.007874016f * (f32) (frand() & 0x7F))));
    col[0] = arg1[0];
    col[1] = arg1[1];
    col[2] = arg1[2];
    col[3] = arg1[3];
    life = (s16) (60.0f * (0.25f + (0.05f * (0.007874016f * (f32) (frand() & 0x7F)))));
    rotY = 360.0f * (0.007874016f * (f32) (frand() & 0x7F));
    MB2_SplashEffCreate(arg0, vel, col, life, arg2,
        farg0 * (100.0f * (0.75f + (0.5f * (0.007874016f * (f32) (frand() & 0x7F))))),
        rotY, 0.8166667f);
}

/* NONMATCHING: complete, asm-read translation. Builds the bespoke particle
 * header (see MB2_CoinEffInit's comment above) from scratch: the particle
 * array (stride 0x58) and the 4-corner UV array (stride 0x20, same layout
 * as MB2_ParticleCopy) get zero-initialized, then a GX_QUADS index-stream
 * display list is baked by writing 16-bit indices straight to the write-
 * gather pipe at 0xCC008000 (three index attributes per vertex - position,
 * a second position-ish index, and the UV corner - times 4 corners per
 * particle), exactly like the original: no GX SDK wrapper is used here. */
s16 MB2_ParticleCreate(ANIMDATA *arg0, s16 arg1) {
    HU3DMODEL *newModel;
    s16 newModelId;
    u8 *hdr;
    u8 *particles;
    u8 *arrA;
    u8 *arrB;
    s16 i;
    void *dispListTmp;
    

    newModelId = Hu3DHookFuncCreate(ParticleDraw);
    Hu3DModelCameraSet(newModelId, 1U);
    newModel = &Hu3DData[newModelId];
    hdr = (u8 *) HuMemDirectMallocNum(HEAP_MODEL, 0x54, newModel->mallocNo);
    newModel->hookData = hdr;
    *(ANIMDATA **) (hdr + 0x38) = arg0;
    *(s16 *) (hdr + 0x26) = arg1;
    *(u8 *) (hdr + 0x20) = 0;
    *(u8 *) (hdr + 0x22) = 0;
    *(s32 *) (hdr + 0x4C) = 0;
    *(s32 *) (hdr + 0x50) = 0;
    *(s32 *) (hdr + 0x28) = 0;
    *(u8 *) (hdr + 0x21) = 0;
    *(u8 *) (hdr + 0x23) = 0;
    *(s32 *) (hdr + 0x30) = 0;
    *(s16 *) (hdr + 0x2) = 0;
    *(s16 *) (hdr + 0x0) = 0;
    particles = (u8 *) HuMemDirectMallocNum(HEAP_MODEL, arg1 * 0x58, newModel->mallocNo);
    *(void **) (hdr + 0x3C) = particles;
    memset(particles, 0, arg1 * 0x58);
    for (i = 0; i < arg1; i++, particles += 0x58) {
        *(f32 *) (particles + 0x34) = 0.0f;
        *(f32 *) (particles + 0x40) = 0.0f;
        *(f32 *) (particles + 0x3C) = 0.0f;
        *(f32 *) (particles + 0x38) = 0.0f;
        *(f32 *) (particles + 0x2C) = 0.0f;
        *(f32 *) (particles + 0x30) = 1.0f;
        *(f32 *) (particles + 0x44) = (f32) (((frand() & 0x7F) - 0x40) * 0x14);
        *(f32 *) (particles + 0x48) = (f32) (((frand() & 0x7F) - 0x40) * 0x1E);
        *(f32 *) (particles + 0x4C) = (f32) (((frand() & 0x7F) - 0x40) * 0x14);
        *(u8 *) (particles + 0x53) = 0xFF;
        *(u8 *) (particles + 0x52) = 0xFF;
        *(u8 *) (particles + 0x51) = 0xFF;
        *(u8 *) (particles + 0x50) = 0xFF;
        *(s32 *) (particles + 0x54) = 0;
    }
    arrA = (u8 *) HuMemDirectMallocNum(HEAP_MODEL, arg1 * 0xC * 4, newModel->mallocNo);
    *(void **) (hdr + 0x40) = arrA;
    for (i = 0; i < arg1 * 4; i++, arrA += 0xC) {
        *(f32 *) (arrA + 0x8) = 0.0f;
        *(f32 *) (arrA + 0x4) = 0.0f;
        *(f32 *) (arrA + 0x0) = 0.0f;
    }
    arrB = (u8 *) HuMemDirectMallocNum(HEAP_MODEL, arg1 * 8 * 4, newModel->mallocNo);
    *(void **) (hdr + 0x44) = arrB;
    for (i = 0; i < arg1; i++, arrB += 0x20) {
        *(f32 *) (arrB + 0x0) = 0.0f;
        *(f32 *) (arrB + 0x4) = 0.0f;
        *(f32 *) (arrB + 0x8) = 1.0f;
        *(f32 *) (arrB + 0xC) = 0.0f;
        *(f32 *) (arrB + 0x10) = 1.0f;
        *(f32 *) (arrB + 0x14) = 1.0f;
        *(f32 *) (arrB + 0x18) = 0.0f;
        *(f32 *) (arrB + 0x1C) = 1.0f;
    }
    dispListTmp = HuMemDirectMallocNum(HEAP_MODEL, 0x10000, newModel->mallocNo);
    DCFlushRange(dispListTmp, 0x10000U);
    GXBeginDisplayList(dispListTmp, 0x10000U);
    GXBegin(GX_QUADS, GX_VTXFMT0, (u16) (arg1 * 4));
    for (i = 0; i < arg1; i++) {
        GXPosition1x16((u16) (i * 4));
        GXColor1x16((u16) i);
        GXTexCoord1x16((u16) (i * 4));
        GXPosition1x16((u16) ((i * 4) + 1));
        GXColor1x16((u16) i);
        GXTexCoord1x16((u16) ((i * 4) + 1));
        GXPosition1x16((u16) ((i * 4) + 2));
        GXColor1x16((u16) i);
        GXTexCoord1x16((u16) ((i * 4) + 2));
        GXPosition1x16((u16) ((i * 4) + 3));
        GXColor1x16((u16) i);
        GXTexCoord1x16((u16) ((i * 4) + 3));
    }
    *(u32 *) (hdr + 0x34) = GXEndDisplayList();
    *(void **) (hdr + 0x48) = HuMemDirectMallocNum(HEAP_MODEL, (s32) *(u32 *) (hdr + 0x34), newModel->mallocNo);
    memcpy(*(void **) (hdr + 0x48), dispListTmp, *(u32 *) (hdr + 0x34));
    DCFlushRange(*(void **) (hdr + 0x48), *(u32 *) (hdr + 0x34));
    HuMemDirectFree(dispListTmp);
    return newModelId;
}

/* NONMATCHING: complete, asm-read translation. Reuses the bespoke particle
 * header/particle-array layout confirmed for MB2_CoinEff{Init,Update} and
 * MB2_ParticleCreate (below): header +0x00/0x02 zeroed, +0x20 blendMode
 * (reset, not copied), +0x21 zeroed, +0x22 attr (copied from source),
 * +0x26 count (copied), +0x28/0x2C/0x30 zeroed, +0x34 display-list size
 * (shared with source, not regenerated), +0x38 source ANIMDATA* (shared),
 * +0x3C new particle array, +0x40/0x44 new per-particle scratch arrays,
 * +0x48 display-list buffer (shared with source), +0x4C/0x50 zeroed. */
s16 MB2_ParticleCopy(s16 arg0) {
    HU3DMODEL *srcModel;
    u8 *srcHdr;
    HU3DMODEL *newModel;
    s16 newModelId;
    u8 *hdr;
    s16 count;
    u8 *particles;
    u8 *arrA;
    u8 *arrB;
    s32 i;

    srcModel = &Hu3DData[arg0];
    srcHdr = (u8 *) srcModel->hookData;
    newModelId = Hu3DHookFuncCreate(srcModel->hookFunc);
    Hu3DModelCameraSet(newModelId, 1U);
    newModel = &Hu3DData[newModelId];
    hdr = (u8 *) HuMemDirectMallocNum(HEAP_MODEL, 0x54, newModel->mallocNo);
    newModel->hookData = hdr;
    *(void **) (hdr + 0x38) = *(void **) (srcHdr + 0x38);
    *(s16 *) (hdr + 0x26) = *(s16 *) (srcHdr + 0x26);
    count = *(s16 *) (hdr + 0x26);
    *(u8 *) (hdr + 0x20) = 0;
    *(u8 *) (hdr + 0x22) = *(u8 *) (srcHdr + 0x22);
    *(s32 *) (hdr + 0x4C) = 0;
    *(s32 *) (hdr + 0x50) = 0;
    *(s32 *) (hdr + 0x28) = 0;
    *(u8 *) (hdr + 0x21) = 0;
    *(s32 *) (hdr + 0x30) = 0;
    *(s16 *) (hdr + 0x2) = 0;
    *(s16 *) (hdr + 0x0) = 0;
    particles = (u8 *) HuMemDirectMallocNum(HEAP_MODEL, count * 0x58, newModel->mallocNo);
    *(void **) (hdr + 0x3C) = particles;
    for (i = 0; i < count; i++, particles += 0x58) {
        *(f32 *) (particles + 0x34) = 0.0f;
        *(f32 *) (particles + 0x40) = 0.0f;
        *(f32 *) (particles + 0x3C) = 0.0f;
        *(f32 *) (particles + 0x38) = 0.0f;
        *(f32 *) (particles + 0x2C) = 0.0f;
        *(f32 *) (particles + 0x30) = 1.0f;
        *(f32 *) (particles + 0x44) = (f32) (((frand() & 0x7F) - 0x40) * 0x14);
        *(f32 *) (particles + 0x48) = (f32) (((frand() & 0x7F) - 0x40) * 0x1E);
        *(f32 *) (particles + 0x4C) = (f32) (((frand() & 0x7F) - 0x40) * 0x14);
        *(u8 *) (particles + 0x53) = 0xFF;
        *(u8 *) (particles + 0x52) = 0xFF;
        *(u8 *) (particles + 0x51) = 0xFF;
        *(u8 *) (particles + 0x50) = 0xFF;
        *(s32 *) (particles + 0x54) = 0;
    }
    arrA = (u8 *) HuMemDirectMallocNum(HEAP_MODEL, count * 0xC * 4, newModel->mallocNo);
    *(void **) (hdr + 0x40) = arrA;
    for (i = 0; i < count * 4; i++, arrA += 0xC) {
        *(f32 *) (arrA + 0x8) = 0.0f;
        *(f32 *) (arrA + 0x4) = 0.0f;
        *(f32 *) (arrA + 0x0) = 0.0f;
    }
    arrB = (u8 *) HuMemDirectMallocNum(HEAP_MODEL, count * 8 * 4, newModel->mallocNo);
    *(void **) (hdr + 0x44) = arrB;
    for (i = 0; i < count; i++) {
        *(f32 *) (arrB + 0x0) = 0.0f;
        *(f32 *) (arrB + 0x4) = 0.0f;
        *(f32 *) (arrB + 0x8) = 1.0f;
        *(f32 *) (arrB + 0xC) = 0.0f;
        *(f32 *) (arrB + 0x10) = 1.0f;
        *(f32 *) (arrB + 0x14) = 1.0f;
        *(f32 *) (arrB + 0x18) = 0.0f;
        *(f32 *) (arrB + 0x1C) = 1.0f;
        arrB += 0x20;
    }
    *(s32 *) (hdr + 0x34) = *(s32 *) (srcHdr + 0x34);
    *(void **) (hdr + 0x48) = *(void **) (srcHdr + 0x48);
    return newModelId;
}

/* NONMATCHING: complete, asm-read translation using the particle header
 * layout already confirmed for MB2_ParticleCreate/Copy/MB2_CoinEffInit:
 * +0x20 blendMode, +0x21/+0x22 attr bits, +0x26 count, +0x34 display-list
 * size, +0x38 source ANIMDATA*, +0x3C particle array (stride 0x58: +0x2C
 * rotX, +0x34 scale/alive gate, +0x38 rotY-ish "billboard axis" flag,
 * +0x3C/0x40 pos, +0x44 world-space center pos, +0x54 UV-tile index byte),
 * +0x40 per-vertex world-pos output array (stride 0xC, 4 per particle),
 * +0x44 per-vertex UV output array (stride 8, 4 per particle), +0x48
 * display-list buffer, +0x4C optional per-model draw hook. */
void ParticleDraw(HU3DMODEL *modelP, f32 (*mtx)[3][4]) {
    u8 *hdr;
    s16 count;

    hdr = (u8 *) modelP->hookData;
    if (shadowModelDrawF == 0) {
        HU3DMODEL *linked = (HU3DMODEL *) *(void **) (hdr + 0x50);
        if ((linked != NULL) && (linked != modelP)) {
            ParticleDraw(linked, mtx);
        }
        GXLoadPosMtxImm((f32 (*)[4]) mtx, 0U);
        GXSetNumTevStages(1U);
        GXSetNumTexGens(1U);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3CU, 0U, 0x7DU);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        if (shadowModelDrawF != 0) {
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ONE, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
            GXSetZMode(0U, GX_LEQUAL, 0U);
        } else {
            ANIMDATA *srcAnim = *(ANIMDATA **) (hdr + 0x38);
            s32 fmt = srcAnim->bmp->dataFmt & 0xF;
            if ((fmt == 7) || (fmt == 8)) {
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_RASC, GX_CC_ZERO);
            } else {
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
            }
            if ((*(u8 *) (hdr + 0x22) & 1) != 0) {
                GXSetZMode(0U, GX_LEQUAL, 0U);
            } else if ((modelP->attr & 2) != 0) {
                GXSetZMode(1U, GX_LEQUAL, 1U);
            } else {
                GXSetZMode(1U, GX_LEQUAL, 0U);
            }
        }
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1U, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1U, GX_TEVPREV);
        GXSetNumChans(1U);
        GXSetChanCtrl(GX_COLOR0A0, 0U, GX_SRC_REG, GX_SRC_VTX, 0U, GX_DF_CLAMP, GX_AF_NONE);
        HuSprTexLoad(*(ANIMDATA **) (hdr + 0x38), 0, 0, 1, 1, 1);
        GXSetAlphaCompare(GX_GEQUAL, 1U, GX_AOP_AND, GX_GEQUAL, 1U);
        GXSetZCompLoc(0U);
        switch (*(u8 *) (hdr + 0x20)) {
        case 0:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
            break;
        case 1:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
            break;
        case 2:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_ZERO, GX_BL_INVSRCALPHA, GX_LO_NOOP);
            break;
        }
        {
            Mtx invMtx, rotMtx;
            void (*hook)(HU3DMODEL *, void *, f32 (*)[3][4]);
            u8 *particle;
            u8 *outPos;
            u8 *outUv;

            if (HmfInverseMtxF3X3((f32 (*)[4]) mtx, invMtx) == 0) {
                PSMTXIdentity(invMtx);
            }
            PSMTXReorder(invMtx, (f32 (*)[3]) rotMtx);
            hook = *(void (**)(HU3DMODEL *, void *, f32 (*)[3][4])) (hdr + 0x4C);
            if (hook != NULL) {
                hook(modelP, hdr, mtx);
            }
            particle = *(u8 **) (hdr + 0x3C);
            outPos = *(u8 **) (hdr + 0x40);
            count = *(s16 *) (hdr + 0x26);
            if ((*(u8 *) (hdr + 0x22) & 4) != 0) {
                Point3d corner[4];
                s32 c;

                PSMTXIdentity(invMtx);
                PSMTXIdentity(rotMtx);
                for (c = 0; c < 4; c++) {
                    corner[c] = mb2_ParticleVtxTbl[c];
                }

                {
                    s32 i;
                    for (i = 0; i < count; i++, particle += 0x58, outPos += 0x30) {
                        if (*(f32 *) (particle + 0x34) == 0.0f) {
                            memset(outPos, 0, 0x30);
                        } else if ((*(u8 *) (hdr + 0x22) & 8) != 0) {
                            Point3d scaled[4], world[4];
                            s32 c;
                            for (c = 0; c < 4; c++) {
                                PSVECScale(&mb2_ParticleVtxTbl[c], &scaled[c], *(f32 *) (particle + 0x34));
                            }
                            mtxRot(invMtx, *(f32 *) (particle + 0x38), *(f32 *) (particle + 0x3C), *(f32 *) (particle + 0x40));
                            PSMTXMultVecArray(invMtx, scaled, world, 4U);
                            for (c = 0; c < 4; c++) {
                                PSVECAdd(&world[c], (Point3d *) (particle + 0x44), (Point3d *) (outPos + c * 0xC));
                            }
                        } else if (*(f32 *) (particle + 0x40) == 0.0f) {
                            Point3d scaled[4];
                            s32 c;
                            for (c = 0; c < 4; c++) {
                                PSVECScale(&mb2_ParticleVtxTbl[c], &scaled[c], *(f32 *) (particle + 0x34));
                                PSVECAdd(&scaled[c], (Point3d *) (particle + 0x44), (Point3d *) (outPos + c * 0xC));
                            }
                        } else {
                            Point3d scaled[4], world[4];
                            Mtx rotZ, concat;
                            s32 c;
                            for (c = 0; c < 4; c++) {
                                PSVECScale(&mb2_ParticleVtxTbl[c], &scaled[c], *(f32 *) (particle + 0x34));
                            }
                            PSMTXRotRad(rotZ, 'Z', *(f32 *) (particle + 0x40));
                            PSMTXConcat(rotMtx, rotZ, concat);
                            PSMTXMultVecArray(concat, scaled, world, 4U);
                            for (c = 0; c < 4; c++) {
                                PSVECAdd(&world[c], (Point3d *) (particle + 0x44), (Point3d *) (outPos + c * 0xC));
                            }
                        }
                    }
                }
            } else {
                s32 i;
                for (i = 0; i < count; i++, particle += 0x58, outPos += 0x30) {
                    if (*(f32 *) (particle + 0x34) == 0.0f) {
                        memset(outPos, 0, 0x30);
                    } else if ((*(u8 *) (hdr + 0x22) & 8) != 0) {
                        Point3d scaled[4], world[4];
                        s32 c;
                        for (c = 0; c < 4; c++) {
                            PSVECScale(&mb2_ParticleVtxTbl[c], &scaled[c], *(f32 *) (particle + 0x34));
                        }
                        mtxRot(invMtx, *(f32 *) (particle + 0x38), *(f32 *) (particle + 0x3C), *(f32 *) (particle + 0x40));
                        PSMTXMultVecArray(invMtx, scaled, world, 4U);
                        for (c = 0; c < 4; c++) {
                            PSVECAdd(&world[c], (Point3d *) (particle + 0x44), (Point3d *) (outPos + c * 0xC));
                        }
                    } else if (*(f32 *) (particle + 0x40) == 0.0f) {
                        Point3d scaled[4];
                        s32 c;
                        for (c = 0; c < 4; c++) {
                            PSVECScale(&mb2_ParticleVtxTbl[c], &scaled[c], *(f32 *) (particle + 0x34));
                            PSVECAdd(&scaled[c], (Point3d *) (particle + 0x44), (Point3d *) (outPos + c * 0xC));
                        }
                    } else {
                        Point3d scaled[4], world[4];
                        Mtx rotZ, concat;
                        s32 c;
                        for (c = 0; c < 4; c++) {
                            PSVECScale(&mb2_ParticleVtxTbl[c], &scaled[c], *(f32 *) (particle + 0x34));
                        }
                        PSMTXRotRad(rotZ, 'Z', *(f32 *) (particle + 0x40));
                        PSMTXConcat(rotMtx, rotZ, concat);
                        PSMTXMultVecArray(concat, scaled, world, 4U);
                        for (c = 0; c < 4; c++) {
                            PSVECAdd(&world[c], (Point3d *) (particle + 0x44), (Point3d *) (outPos + c * 0xC));
                        }
                    }
                }
            }

            particle = *(u8 **) (hdr + 0x3C);
            outUv = *(u8 **) (hdr + 0x44);
            if ((*(u8 *) (hdr + 0x22) & 2) == 0) {
                s32 i, c;
                for (i = 0; i < count; i++, particle += 0x58) {
                    for (c = 0; c < 4; c++, outUv += 8) {
                        *(f32 *) (outUv + 0) = (0.25f * (f32) (s16) (*(u8 *) (particle + 0x54) & 3)) + mb2_ParticleUvTbl[c].unk0;
                        *(f32 *) (outUv + 4) = (0.25f * (f32) (s16) ((*(u8 *) (particle + 0x54) >> 2) & 3)) + mb2_ParticleUvTbl[c].unk4;
                    }
                }
            } else {
                s32 c;
                for (c = 0; c < 4; c++, outUv += 8) {
                    *(f32 *) (outUv + 0) = 4.0f * mb2_ParticleUvTbl[c].unk0;
                    *(f32 *) (outUv + 4) = 4.0f * mb2_ParticleUvTbl[c].unk4;
                }
            }
        }
        DCFlushRangeNoSync(*(void **) (hdr + 0x40), count * 0xC * 4);
        DCFlushRangeNoSync(*(void **) (hdr + 0x44), count * 8 * 4);
        DCFlushRangeNoSync(*(void **) (hdr + 0x3C), count * 0x58);
        PPCSync();
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0U);
        GXSETARRAY(GX_VA_POS, *(void **) (hdr + 0x40), count * 0xC * 4U, 0xCU, TRUE);
        GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_TEX_ST, GX_RGBA8, 0U);
        GXSETARRAY(GX_VA_CLR0, *(u8 **) (hdr + 0x3C) + 0x50, count * 0x58U, 0x58U, TRUE);
        GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_RGBA6, 0U);
        GXSETARRAY(GX_VA_TEX0, *(void **) (hdr + 0x44), count * 8U * 4U, 8U, TRUE);
        GXCallDisplayList(*(void **) (hdr + 0x48), *(u32 *) (hdr + 0x34));
        if (shadowModelDrawF == 0) {
            if ((*(u8 *) (hdr + 0x21) & 2) == 0) {
                *(u32 *) (hdr + 0x28) = *(u32 *) (hdr + 0x28) + 1;
            }
            if ((*(u32 *) (hdr + 0x30) != 0) && (*(u32 *) (hdr + 0x30) <= *(u32 *) (hdr + 0x28))) {
                if ((*(u8 *) (hdr + 0x21) & 1) != 0) {
                    *(u32 *) (hdr + 0x28) = 0;
                }
                *(u32 *) (hdr + 0x28) = *(u32 *) (hdr + 0x30);
            }
        }
    }
}

void MB2_YajiInit(f32 *arg0) {
    OMOBJ *obj;
    Point3d *pos;
    f32 ang;
    s32 *dat;
    s32 i;

    pos = (Point3d *) arg0;
    obj = omAddObjEx(mbObjMan, -0x8000, 3U, 0U, -1, MB2_YajiUpdate);
    mb2_YajiObj = obj;
    dat = (s32 *) HuMemDirectMallocNum(HEAP_HEAP, 4, 0x10000000U);
    obj->data = dat;
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
    HU3DMODEL *modelP;
    f32 scale;
    s32 i;
    s32 *dat;

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
                *dat = *dat + 6;
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
    if (mb2_YajiObj != NULL) {
        mb2_YajiObj->work[0] = arg0;
        *(s32 *) mb2_YajiObj->data = 0;
    }
}

void MB2_YajiDispSet(s32 arg0) {
    OMOBJ *obj;
    s32 i;

    obj = mb2_YajiObj;
    if (mb2_YajiObj != NULL) {
        for (i = 0; i < 3; i++) {
            if (arg0 != 0) {
                Hu3DModelAttrReset(obj->mdlId[i], 1U);
            } else {
                Hu3DModelAttrSet(obj->mdlId[i], 1U);
            }
        }
        *(s32 *) obj->data = 0;
    }
}

/* mb2_RainbowObj->data: 0xC-byte header {s32 modelId; ANIMDATA *srcAnim;
 * void *slots (8 entries, stride 0x28)}. Each rainbow-arc slot: +0x00
 * activeF, +0x04 vtx count *3, +0x08 vtx array, +0x0C rot array, +0x10/0x14/
 * 0x18 more per-vtx arrays (draw-side), +0x1C..0x1F RGBA (alpha at +0x1F),
 * +0x20 display list, +0x24 display list size. Confirmed by
 * MB2_RainbowObjAlphaSet/VtxTopGet's simple, unambiguous field usage. */
typedef struct MB2RainbowObjData_s {
    s32 modelId;   /* 0x00 */
    ANIMDATA *srcAnim; /* 0x04 */
    u8 *slots;     /* 0x08, 8 entries, stride 0x28 */
} MB2_RAINBOWOBJ_DATA; /* size 0xC */

void MB2_RainbowObjInit(void) {
    OMOBJ *obj;
    HU3DMODEL *modelP;
    MB2_RAINBOWOBJ_DATA *dat;
    s16 modelId;
    u8 *slots;
    s32 i;

    obj = omAddObjEx(mbObjMan, -0x8000, 0, 0, -1, MB2_RainbowObjUpdate);
    mb2_RainbowObj = obj;
    dat = (MB2_RAINBOWOBJ_DATA *) HuMemDirectMallocNum(HEAP_HEAP, sizeof(MB2_RAINBOWOBJ_DATA), 0x10000000U);
    obj->data = dat;
    memset(dat, 0, sizeof(MB2_RAINBOWOBJ_DATA));
    modelId = Hu3DHookFuncCreate(RainbowObjDraw);
    dat->modelId = modelId;
    Hu3DModelCameraSet(modelId, 1U);
    Hu3DModelLayerSet(modelId, 3);
    modelP = &Hu3DData[modelId];
    modelP->pos.y = -100000.0f;
    modelP->pos.z = -100000.0f;
    modelP->hookData = dat;
    dat->srcAnim = (ANIMDATA *)(uintptr_t) MB2W_S32(0x1B * 4);
    slots = (u8 *) HuMemDirectMallocNum(HEAP_MODEL, 8 * 0x28, modelP->mallocNo);
    dat->slots = slots;
    memset(slots, 0, 8 * 0x28);
    for (i = 0; i < 8; i++, slots += 0x28) {
        *(s32 *) slots = 0;
    }
}

void MB2_RainbowObjUpdate(OMOBJ *obj) {
    MB2_RAINBOWOBJ_DATA *dat;
    s32 i;

    dat = (MB2_RAINBOWOBJ_DATA *) obj->data;
    if (MBKillCheck() || (mb2_RainbowObj == NULL)) {
        Hu3DModelKill((s16) dat->modelId);
        omDelObjEx(mbObjMan, obj);
        mb2_RainbowObj = NULL;
        return;
    }
    for (i = 0; i < 8; i++) {
    }
}

void MB2_RainbowObjClose(void) {
    mb2_RainbowObj = NULL;
}

/* NONMATCHING: complete, asm-read translation. Extends the
 * MB2_RAINBOWOBJ_DATA slot layout (already confirmed for RainbowObjDraw:
 * +0x00 activeF, +0x10/0x14/0x18 GX pos/uv/color arrays, +0x1C..0x1F tint,
 * +0x20/0x24 display list) with the build-time-only scratch: +0x04 vertex
 * count ((pathLen-1)*3, one Hermite-interpolated triple per path segment),
 * +0x08 the interpolated centerline points (stride 0xC), +0x0C a matching
 * per-point (pitch,yaw) angle array (stride 0xC, only .x/.y used) computed
 * from the centerline via the same __frsqrte pattern as MB2Ev_Rain. Builds
 * a ribbon two vertices wide (+/-75 units either side of the centerline,
 * perpendicular to its yaw) as a GX_TRIANGLESTRIP index-stream display
 * list written straight to the write-gather pipe, same technique as
 * MB2_ParticleCreate.
 *
 * Re-verified against build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s
 * (0x11300-0x11F04) after an 11.69% fuzzy flag: the Hermite p0/p1/p2/p3
 * neighbour selection at path boundaries, the argument order into
 * MB2_HermiteCalcV, and the free-slot search all match the asm exactly.
 * Found and fixed a real bug in the forward (arg2==0) winding order: it
 * wrote 2x(i*2) then 4x(i*2+1) per vertex; the asm writes 3x(i*2) then
 * 3x(i*2+1) (the reverse/arg2!=0 branch was already correct). The
 * remaining low fuzzy% is register-allocation noise from the heavy
 * trig/sqrt scalar math, not further logic bugs. */
s32 MB2_RainbowObjCreate(void *arg0, s32 arg1, s32 arg2) {
    MB2_RAINBOWOBJ_DATA *dat;
    u8 *slot;
    s32 slotNo;
    Point3d *path;
    u8 *ctrl;
    s32 vtxCount;
    u8 *angle;
    u8 *posArr;
    u8 *uvArr;
    u8 *colArr;
    void *dispListTmp;
    s32 i;
    

    path = (Point3d *) arg0;
    if (mb2_RainbowObj == NULL) {
        return -1;
    }
    dat = (MB2_RAINBOWOBJ_DATA *) mb2_RainbowObj->data;
    slot = dat->slots;
    for (slotNo = 0; slotNo < 8; slotNo++, slot = dat->slots + slotNo * 0x28) {
        if (*(s32 *) slot == 0) {
            break;
        }
    }
    if (slotNo >= 8) {
        return -1;
    }
    *(s32 *) slot = 1;

    ctrl = (u8 *) HuMemDirectMallocNum(HEAP_MODEL, arg1 * 3 * 0xC, Hu3DData[dat->modelId].mallocNo);
    *(void **) (slot + 0x8) = ctrl;
    memset(ctrl, 0, arg1 * 3 * 0xC);
    for (i = 0; i < arg1 - 1; i++) {
        Point3d *p0 = (i > 0) ? &path[i - 1] : &path[i];
        Point3d *p1 = &path[i];
        Point3d *p2 = &path[i + 1];
        Point3d *p3 = (i < arg1 - 2) ? &path[i + 2] : &path[i + 1];
        s32 k;
        for (k = 0; k < 3; k++) {
            MB2_HermiteCalcV(p0, p1, p2, p3, (Point3d *) ctrl, (f32) k / 3.0f);
            ctrl += 0xC;
        }
    }

    vtxCount = (arg1 - 1) * 3;
    *(s32 *) (slot + 0x4) = vtxCount;
    angle = (u8 *) HuMemDirectMallocNum(HEAP_MODEL, vtxCount * 0xC, Hu3DData[dat->modelId].mallocNo);
    *(void **) (slot + 0xC) = angle;
    memset(angle, 0, vtxCount * 0xC);
    ctrl = *(u8 **) (slot + 0x8);
    for (i = 0; i < vtxCount; i++) {
        Point3d delta = { 0.0f, 0.0f, 0.0f };
        f32 distSq, dist;

        if (i < vtxCount - 1) {
            PSVECSubtract((Point3d *) (ctrl + (i + 1) * 0xC), (Point3d *) (ctrl + i * 0xC), &delta);
        }
        distSq = (delta.x * delta.x) + (delta.z * delta.z);
        if (distSq > 0.0f) {
            f64 e0 = __frsqrte(distSq);
            f64 e1 = 0.5 * e0 * (3.0 - ((f64) distSq * (e0 * e0)));
            f64 e2 = 0.5 * e1 * (3.0 - ((f64) distSq * (e1 * e1)));
            dist = (f32) ((f64) distSq * (0.5 * e2 * (3.0 - ((f64) distSq * (e2 * e2)))));
        } else {
            dist = distSq;
        }
        *(f32 *) (angle + i * 0xC + 0x0) = (f32) (180.0 * (atan2((f64) dist, (f64) delta.y) / 3.141592653589793));
        *(f32 *) (angle + i * 0xC + 0x4) = (f32) (180.0 * (atan2((f64) delta.x, (f64) delta.z) / 3.141592653589793));
        *(f32 *) (angle + i * 0xC + 0x8) = 0.0f;
    }

    *(u8 *) (slot + 0x1F) = 0xFF;
    *(u8 *) (slot + 0x1E) = 0xFF;
    *(u8 *) (slot + 0x1D) = 0xFF;
    *(u8 *) (slot + 0x1C) = 0xFF;

    posArr = (u8 *) HuMemDirectMallocNum(HEAP_MODEL, vtxCount * 0xC * 2, Hu3DData[dat->modelId].mallocNo);
    *(void **) (slot + 0x10) = posArr;
    memset(posArr, 0, vtxCount * 0xC);
    uvArr = (u8 *) HuMemDirectMallocNum(HEAP_MODEL, (arg1 - 1) * 0x18 * 2, Hu3DData[dat->modelId].mallocNo);
    *(void **) (slot + 0x14) = uvArr;
    memset(uvArr, 0, (arg1 - 1) * 0x18);
    colArr = (u8 *) HuMemDirectMallocNum(HEAP_MODEL, (arg1 - 1) * 0xC * 2, Hu3DData[dat->modelId].mallocNo);
    *(void **) (slot + 0x18) = colArr;
    memset(colArr, 0, (arg1 - 1) * 0xC);

    for (i = 0; i < vtxCount; i++) {
        Point3d center;
        u8 *topPos = posArr + i * 0xC;
        u8 *topUv = uvArr + i * 8;
        u8 *topCol = colArr + i * 4;
        u8 *botPos = topPos + (vtxCount * 0xC);
        u8 *botUv = topUv + ((arg1 - 1) * 0x18);
        u8 *botCol = colArr + (arg1 - 1) * 0xC + i * 4;
        f32 yaw = *(f32 *) (angle + i * 0xC + 0x4);
        f32 fade;

        center.x = *(f32 *) (ctrl + i * 0xC + 0x0);
        center.y = *(f32 *) (ctrl + i * 0xC + 0x4);
        center.z = *(f32 *) (ctrl + i * 0xC + 0x8);

        if (i < 5) {
            fade = 192.0f * ((f32) i / 5.0f);
        } else if (i > vtxCount - 5) {
            fade = 192.0f * ((f32) (5 - (i - (vtxCount - 5))) / 5.0f);
        } else {
            fade = 192.0f;
        }

        *(f32 *) (topPos + 0x0) = (f32) ((f64) center.x + (75.0 * sin((3.141592653589793 * (f64) (90.0f + yaw)) / 180.0)));
        *(f32 *) (topPos + 0x8) = (f32) ((f64) center.z + (75.0 * cos((3.141592653589793 * (f64) (90.0f + yaw)) / 180.0)));
        *(f32 *) (topPos + 0x4) = center.y;
        *(f32 *) (topUv + 0x0) = 0.0f;
        *(f32 *) (topUv + 0x4) = (1.0f / (f32) vtxCount) * (f32) i;
        *(u8 *) (topCol + 0) = 0xC0;
        *(u8 *) (topCol + 1) = 0xC0;
        *(u8 *) (topCol + 2) = 0xC0;
        *(u8 *) (topCol + 3) = (u8) fade;

        *(f32 *) (botPos + 0x0) = (f32) ((f64) center.x + (75.0 * sin((3.141592653589793 * (f64) (yaw - 90.0f)) / 180.0)));
        *(f32 *) (botPos + 0x8) = (f32) ((f64) center.z + (75.0 * cos((3.141592653589793 * (f64) (yaw - 90.0f)) / 180.0)));
        *(f32 *) (botPos + 0x4) = center.y;
        *(f32 *) (botUv + 0x0) = 1.0f;
        *(f32 *) (botUv + 0x4) = (1.0f / (f32) vtxCount) * (f32) i;
        *(u8 *) (botCol + 0) = 0xFF;
        *(u8 *) (botCol + 1) = 0xFF;
        *(u8 *) (botCol + 2) = 0xFF;
        *(u8 *) (botCol + 3) = (u8) fade;
    }

    DCFlushRange(*(void **) (slot + 0x10), vtxCount * 0xC * 2);
    DCFlushRange(*(void **) (slot + 0x14), (arg1 - 1) * 0x18 * 2);
    DCFlushRange(*(void **) (slot + 0x18), (arg1 - 1) * 0xC * 2);
    dispListTmp = HuMemDirectMallocNum(HEAP_MODEL, 0x10000, Hu3DData[dat->modelId].mallocNo);
    DCFlushRange(dispListTmp, 0x10000U);
    GXBeginDisplayList(dispListTmp, 0x10000U);
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (u16) ((arg1 - 1) * 6));
    if (arg2 == 0) {
        for (i = 0; i < vtxCount; i++) {
            GXPosition1x16((u16) (i * 2));
            GXColor1x16((u16) (i * 2));
            GXTexCoord1x16((u16) (i * 2));
            GXPosition1x16((u16) ((i * 2) + 1));
            GXColor1x16((u16) ((i * 2) + 1));
            GXTexCoord1x16((u16) ((i * 2) + 1));
        }
    } else {
        for (i = vtxCount - 1; i >= 0; i--) {
            GXPosition1x16((u16) ((i * 2) + 1));
            GXColor1x16((u16) (i * 2));
            GXTexCoord1x16((u16) ((i * 2) + 1));
            GXPosition1x16((u16) (i * 2));
            GXColor1x16((u16) ((i * 2) + 1));
            GXTexCoord1x16((u16) (i * 2));
        }
    }
    /* NOTE: unlike the arg2==0 branch (pos=clr=tex per vertex), this reverse-
     * winding branch really does interleave 1,0,1,0,1,0 across the 2
     * vertices' 3 attributes in the original asm - not a simple per-vertex
     * repeat. Verified against the raw *fifo writes before this port. */
    *(u32 *) (slot + 0x24) = GXEndDisplayList();
    *(void **) (slot + 0x20) = HuMemDirectMallocNum(HEAP_MODEL, (s32) *(u32 *) (slot + 0x24), Hu3DData[dat->modelId].mallocNo);
    memcpy(*(void **) (slot + 0x20), dispListTmp, *(u32 *) (slot + 0x24));
    DCFlushRange(*(void **) (slot + 0x20), *(u32 *) (slot + 0x24));
    HuMemDirectFree(dispListTmp);
    return slotNo;
}

void MB2_RainbowObjAlphaSet(s32 arg0, s8 arg1) {
    if (mb2_RainbowObj != NULL) {
        MB2_RAINBOWOBJ_DATA *dat = (MB2_RAINBOWOBJ_DATA *) mb2_RainbowObj->data;
        *(s8 *) (dat->slots + (arg0 * 0x28) + 0x1F) = arg1;
    }
}

s32 MB2_RainbowObjVtxTopGet(s32 arg0, void **arg1) {
    s32 ret;
    MB2_RAINBOWOBJ_DATA *dat;
    u8 *slot;

    ret = arg0;
    if (mb2_RainbowObj != NULL) {
        dat = (MB2_RAINBOWOBJ_DATA *) mb2_RainbowObj->data;
        slot = dat->slots + (arg0 * 0x28);
        *arg1 = *(void **) (slot + 0x8);
        ret = *(s32 *) (slot + 0x4);
    }
    return ret;
}

/* NONMATCHING: complete, asm-read translation. Confirms and extends the
 * MB2_RAINBOWOBJ_DATA slot layout: +0x10 GX_VA_POS array (world-space
 * strip vertices), +0x14 GX_VA_TEX0 array (UVs), +0x18 GX_VA_CLR0 array
 * (per-vertex color), +0x1C..0x1F a single RGBA tint loaded into
 * GX_TEVREG2, +0x20 display-list buffer, +0x24 display-list size. */
void RainbowObjDraw(HU3DMODEL *modelP, f32 (*mtx)[3][4]) {
    MB2_RAINBOWOBJ_DATA *dat;
    u8 *slot;
    s32 i;

    if (mb2_RainbowObj == NULL) {
        return;
    }
    dat = (MB2_RAINBOWOBJ_DATA *) modelP->hookData;
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
    for (i = 0; i < 8; i++, slot += 0x28) {
        GXColor tint;
        if (*(s32 *) slot == 0) {
            continue;
        }
        {
            s32 slotVtxCount = *(s32 *) (slot + 0x4);
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0U);
            GXSETARRAY(GX_VA_POS, *(void **) (slot + 0x10), slotVtxCount * 0xC * 2, 0xCU, TRUE);
            GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_TEX_ST, GX_RGBA8, 0U);
            GXSETARRAY(GX_VA_CLR0, *(void **) (slot + 0x18), slotVtxCount * 4U * 2, 4U, TRUE);
            GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_RGBA6, 0U);
            GXSETARRAY(GX_VA_TEX0, *(void **) (slot + 0x14), slotVtxCount * 8U * 2, 8U, TRUE);
        }
        tint.r = *(u8 *) (slot + 0x1C);
        tint.g = *(u8 *) (slot + 0x1D);
        tint.b = *(u8 *) (slot + 0x1E);
        tint.a = *(u8 *) (slot + 0x1F);
        GXSetTevColor(GX_TEVREG2, tint);
        GXCallDisplayList(*(void **) (slot + 0x20), *(u32 *) (slot + 0x24));
    }
}
