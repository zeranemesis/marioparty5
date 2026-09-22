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
static Point3d mb2ev_Zen2DestTbl[3];
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
    u8 pad18[4];
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
    if (MB2W_S32((0x1B) * 4) != 0) {
        HuSprAnimKill((ANIMDATA *)(uintptr_t) MB2W_S32((0x1B) * 4));
    }
    MB2W_S32((0x1B) * 4) = 0;
    if (MB2W_S32((0x1C) * 4) != 0) {
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
    MB2W_S32((0x1E) * 4) = -3700.0f;
    MB2W_S32((0x1F) * 4) = 18100.0f;
    MB2W_S32((0x20) * 4) = 21300.0f;
    MB2W_S32((0x21) * 4) = 3700.0f;
    MB2W_S32((0x22) * 4) = -18100.0f;
    MB2W_S32((0x23) * 4) = -21300.0f;
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
    return 0;
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
void MB2Ev_Rain(s32 arg0) {
    /* NONMATCHING: placeholder - full body not yet ported from asm.
     * The m2c draft for this event produced many unresolved stack-spill
     * temporaries (undefined "unkspXX" locals) that need a manual,
     * instruction-by-instruction read of
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s to model correctly
     * (see docs/decomp_workflow.md). For now this keeps the player's turn
     * from softlocking: it just clears any shift motion. */
    MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 8.0f, 0x40000001U);
}

void MB2Ev_Zen2(s32 arg0) {
    /* NONMATCHING: placeholder - full body not yet ported from asm.
     * The m2c draft for this event produced many unresolved stack-spill
     * temporaries (undefined "unkspXX" locals) that need a manual,
     * instruction-by-instruction read of
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s to model correctly
     * (see docs/decomp_workflow.md). For now this keeps the player's turn
     * from softlocking: it just clears any shift motion. */
    MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 8.0f, 0x40000001U);
}

void MB2Ev_Zen4(s32 arg0) {
    /* NONMATCHING: placeholder - full body not yet ported from asm.
     * The m2c draft for this event produced many unresolved stack-spill
     * temporaries (undefined "unkspXX" locals) that need a manual,
     * instruction-by-instruction read of
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s to model correctly
     * (see docs/decomp_workflow.md). For now this keeps the player's turn
     * from softlocking: it just clears any shift motion. */
    MBPlayerMotionNoShiftSet(arg0, 1, 0.0f, 8.0f, 0x40000001U);
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
    s32 sp8;

    sp8 = arg0;
    if (MB2_PathLenGet((s16) sp8, 0x19, 0) != -1) {
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

void MB2_CoinUpdate(OMOBJ *obj) {
    /* NONMATCHING: placeholder - real body not yet read from asm; needs
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s instruction-by-instruction
     * (MB2_COIN_SLOT is reconstructed above - CoinInit/CoinCreate/CoinNumGet
     * already use it for real). */
    if (MBKillCheck() || (mb2_CoinObj == NULL)) {
        omDelObjEx(mbObjMan, obj);
        mb2_CoinObj = NULL;
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

void MB2_CoinEffInit(void) {
    /* NONMATCHING: placeholder - m2c mismodeled this function's flat
     * per-slot struct (ANIMDATA* / void* typed loop pointers used with
     * raw ->unkXX offsets), producing dozens of type errors. Needs a
     * proper struct reconstructed from
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s (same technique as
     * MB3DEBRIS/MB7W_* in w03Dll/w07Dll) before it can be ported for real. */
}

void MB2_CoinEffUpdate(OMOBJ *obj) {
    /* NONMATCHING: placeholder - m2c mismodeled this function's flat
     * per-slot struct (ANIMDATA* / void* typed loop pointers used with
     * raw ->unkXX offsets), producing dozens of type errors. Needs a
     * proper struct reconstructed from
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s (same technique as
     * MB3DEBRIS/MB7W_* in w03Dll/w07Dll) before it can be ported for real. */
}

void MB2_CoinEffClose(void) {
    mb2_CoinEffObj = NULL;
}

s32 MB2_CoinEffNumGet(void) {
    /* NONMATCHING: placeholder - m2c mismodeled this function's flat
     * per-slot struct (ANIMDATA* / void* typed loop pointers used with
     * raw ->unkXX offsets), producing dozens of type errors. Needs a
     * proper struct reconstructed from
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s (same technique as
     * MB3DEBRIS/MB7W_* in w03Dll/w07Dll) before it can be ported for real. */
    return 0;
}

void MB2_CoinEffBlendModeSet(u8 arg0) {
    /* NONMATCHING: placeholder - m2c mismodeled this function's flat
     * per-slot struct (ANIMDATA* / void* typed loop pointers used with
     * raw ->unkXX offsets), producing dozens of type errors. Needs a
     * proper struct reconstructed from
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s (same technique as
     * MB3DEBRIS/MB7W_* in w03Dll/w07Dll) before it can be ported for real. */
}

s32 MB2_CoinEffCreate(f32 *arg0, f32 *arg1, u8 *arg2, f32 farg0, f32 farg1, f32 farg2) {
    /* NONMATCHING: placeholder - m2c mismodeled this function's flat
     * per-slot struct (ANIMDATA* / void* typed loop pointers used with
     * raw ->unkXX offsets), producing dozens of type errors. Needs a
     * proper struct reconstructed from
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s (same technique as
     * MB3DEBRIS/MB7W_* in w03Dll/w07Dll) before it can be ported for real. */
    return 0;
}

s32 MB2_CoinEffCreateRand(void *arg0, s32 arg1, f32 farg0, f32 farg1, f32 farg2, f32 farg3, f32 farg4) {
    /* NONMATCHING: placeholder - m2c mismodeled this function's flat
     * per-slot struct (ANIMDATA* / void* typed loop pointers used with
     * raw ->unkXX offsets), producing dozens of type errors. Needs a
     * proper struct reconstructed from
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s (same technique as
     * MB3DEBRIS/MB7W_* in w03Dll/w07Dll) before it can be ported for real. */
    return 0;
}

s32 MB2_CoinEffCreateColRand(f32 *arg0, s32 arg1, f32 farg0, f32 farg1, f32 farg2, f32 farg3, f32 farg4) {
    /* NONMATCHING: placeholder - m2c mismodeled this function's flat
     * per-slot struct (ANIMDATA* / void* typed loop pointers used with
     * raw ->unkXX offsets), producing dozens of type errors. Needs a
     * proper struct reconstructed from
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s (same technique as
     * MB3DEBRIS/MB7W_* in w03Dll/w07Dll) before it can be ported for real. */
    return 0;
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

s16 MB2_ParticleCreate(ANIMDATA *arg0, s16 arg1) {
    /* NONMATCHING: placeholder - m2c mismodeled this function's flat
     * per-slot struct (ANIMDATA* / void* typed loop pointers used with
     * raw ->unkXX offsets), producing dozens of type errors. Needs a
     * proper struct reconstructed from
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s (same technique as
     * MB3DEBRIS/MB7W_* in w03Dll/w07Dll) before it can be ported for real. */
    return 0;
}

s16 MB2_ParticleCopy(s16 arg0) {
    /* NONMATCHING: placeholder - m2c mismodeled this function's flat
     * per-slot struct (ANIMDATA* / void* typed loop pointers used with
     * raw ->unkXX offsets), producing dozens of type errors. Needs a
     * proper struct reconstructed from
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s (same technique as
     * MB3DEBRIS/MB7W_* in w03Dll/w07Dll) before it can be ported for real. */
    return 0;
}

void ParticleDraw(HU3DMODEL *modelP, f32 (*mtx)[3][4]) {
    /* NONMATCHING: placeholder - m2c mismodeled the flat per-particle
     * struct here (raw ->unkXX offsets on a void* / ANIMDATA* loop pointer).
     * Needs a proper struct reconstructed from
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s before porting for real. */
}

void MB2_YajiInit(f32 *arg0) {
    /* NONMATCHING: placeholder - m2c mismodeled this function's flat
     * per-slot struct (ANIMDATA* / void* typed loop pointers used with
     * raw ->unkXX offsets), producing dozens of type errors. Needs a
     * proper struct reconstructed from
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s (same technique as
     * MB3DEBRIS/MB7W_* in w03Dll/w07Dll) before it can be ported for real. */
}

void MB2_YajiUpdate(OMOBJ *obj) {
    /* NONMATCHING: placeholder - m2c mismodeled this function's flat
     * per-slot struct (ANIMDATA* / void* typed loop pointers used with
     * raw ->unkXX offsets), producing dozens of type errors. Needs a
     * proper struct reconstructed from
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s (same technique as
     * MB3DEBRIS/MB7W_* in w03Dll/w07Dll) before it can be ported for real. */
}

void MB2_YajiClose(void) {
    mb2_YajiObj = NULL;
}

void MB2_YajiHiliteSet(u32 arg0) {
    /* NONMATCHING: placeholder - m2c mismodeled this function's flat
     * per-slot struct (ANIMDATA* / void* typed loop pointers used with
     * raw ->unkXX offsets), producing dozens of type errors. Needs a
     * proper struct reconstructed from
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s (same technique as
     * MB3DEBRIS/MB7W_* in w03Dll/w07Dll) before it can be ported for real. */
}

void MB2_YajiDispSet(s32 arg0) {
    /* NONMATCHING: placeholder - m2c mismodeled this function's flat
     * per-slot struct (ANIMDATA* / void* typed loop pointers used with
     * raw ->unkXX offsets), producing dozens of type errors. Needs a
     * proper struct reconstructed from
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s (same technique as
     * MB3DEBRIS/MB7W_* in w03Dll/w07Dll) before it can be ported for real. */
}

void MB2_RainbowObjInit(void) {
    /* NONMATCHING: placeholder - m2c mismodeled this function's flat
     * per-slot struct (ANIMDATA* / void* typed loop pointers used with
     * raw ->unkXX offsets), producing dozens of type errors. Needs a
     * proper struct reconstructed from
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s (same technique as
     * MB3DEBRIS/MB7W_* in w03Dll/w07Dll) before it can be ported for real. */
}

void MB2_RainbowObjUpdate(OMOBJ *obj) {
    /* NONMATCHING: placeholder - m2c mismodeled this function's flat
     * per-slot struct (ANIMDATA* / void* typed loop pointers used with
     * raw ->unkXX offsets), producing dozens of type errors. Needs a
     * proper struct reconstructed from
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s (same technique as
     * MB3DEBRIS/MB7W_* in w03Dll/w07Dll) before it can be ported for real. */
}

void MB2_RainbowObjClose(void) {
    mb2_RainbowObj = NULL;
}

s32 MB2_RainbowObjCreate(void *arg0, s32 arg1, s32 arg2) {
    /* NONMATCHING: placeholder - m2c mismodeled this function's flat
     * per-slot struct (ANIMDATA* / void* typed loop pointers used with
     * raw ->unkXX offsets), producing dozens of type errors. Needs a
     * proper struct reconstructed from
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s (same technique as
     * MB3DEBRIS/MB7W_* in w03Dll/w07Dll) before it can be ported for real. */
    return 0;
}

void MB2_RainbowObjAlphaSet(s32 arg0, s8 arg1) {
    /* NONMATCHING: placeholder - m2c mismodeled this function's flat
     * per-slot struct (ANIMDATA* / void* typed loop pointers used with
     * raw ->unkXX offsets), producing dozens of type errors. Needs a
     * proper struct reconstructed from
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s (same technique as
     * MB3DEBRIS/MB7W_* in w03Dll/w07Dll) before it can be ported for real. */
}

s32 MB2_RainbowObjVtxTopGet(s32 arg0, void **arg1) {
    /* NONMATCHING: placeholder - m2c mismodeled this function's flat
     * per-slot struct (ANIMDATA* / void* typed loop pointers used with
     * raw ->unkXX offsets), producing dozens of type errors. Needs a
     * proper struct reconstructed from
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s (same technique as
     * MB3DEBRIS/MB7W_* in w03Dll/w07Dll) before it can be ported for real. */
    return 0;
}

void RainbowObjDraw(HU3DMODEL *modelP, f32 (*mtx)[3][4]) {
    /* NONMATCHING: placeholder - m2c mismodeled the flat per-particle
     * struct here (raw ->unkXX offsets on a void* / ANIMDATA* loop pointer).
     * Needs a proper struct reconstructed from
     * build/GP5E01_00/w02Dll/asm/REL/w02Dll/world02.s before porting for real. */
}
