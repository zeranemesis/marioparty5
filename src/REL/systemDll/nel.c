#include "REL/systemDll/sysmd.h"
#include "game/flag.h"
#include "game/memory.h"
#include "game/wipe.h"
#include "game/data.h"
#include "game/process.h"
#include "game/charman.h"

/* 0x140: four u16 pad-edge words (NEL_SYS declares them as s32 padStat..unk14C) */
/* 0x10: written with integer stores (NEL_SYS declares it float angSpd) */
#define NEL_INT10(sys) (*(s32 *)&(sys)->angSpd)
#define PADSTAT(sys) (((u16 *)&(sys)->padStat)[0])

typedef struct MdFlowEnt_s MDFLOWENT;
struct MdFlowEnt_s { void (*func)(NEL_SYS *sys, MDFLOWENT *ent); };
typedef struct MdProcEnt_s MDPROCENT;
struct MdProcEnt_s { s32 (*func)(NEL_SYS *sys, MDPROCENT *ent); };

/* The overlay's global work block (0x290 bytes, .bss). */
NEL_SYS sysWork;

/* Runtime work struct for a single "glow" particle effect. */
typedef struct ParticleGlow_s {
    /* 0x00 */ int parManId;
} ParticleGlow;

void MdSpriteDebug(NEL_SYS *sys);
void MdSpriteDispSet(s32 grpNo, s32 dispF);
void MdLayer1Hook(s16 layerNo);
void fn_1_196B4(NEL_SYS *sys, s32 no);
void fn_1_17A2C(NEL_SYS *sys);
void fn_1_19780(NEL_SYS *sys);
void fn_1_19B10(s32 arg);
void fn_1_198E0(s32 modelId, float angle);
void fn_1_193FC(NEL_SYS *sys, s32 arg);
void fn_1_19904(s32 arg);
void fn_1_17930(NEL_SYS *sys);
void fn_1_28F74(s32 arg);
void fn_1_28FAC(s32 a, s32 b);
void SLBoardLoad(void);
void MBPartySaveInit(s32 arg);
void MdRingDispSet(NEL_SYS *sys, s32 no, s32 dispF);
void MdRingModeSet(NEL_SYS *sys, s32 no, s32 mode);
void fn_1_F94(NEL_SYS *sys, volatile float angle);
void NelGuideBillboardUpdate(NEL_SYS *sys, s32 no);
void MdRingMain(NEL_SYS *sys);
void MdFukiMain(NEL_SYS *sys);
void MdBlendDebugMain(void);
void MdSpriteDebugMain(void);
void fn_1_19CCC(void);
void MdObjCameraCreate(OMOBJ *obj);
void MdObjLightCreate(OMOBJ *obj);
void MdObjPadMain(void);
void MdObjDebugFontCreate(OMOBJ *obj);
void MdMathStdCurveInit(void);

void ObjectSetup(void);
void MdInit(NEL_SYS *sys, MDFLOWENT *tbl);
/* shortcut: MdProcTbl dispatch loads r4 once in the original (call-through-arg); register pressure also differs (idx kept in a reg). */
void MdClose(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_1108(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_15EC(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_1BC8(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_2390(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_2C88(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_46F4(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_5B18(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_716C(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_7C88(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_8F18(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_974C(NEL_SYS *sys, MDFLOWENT *tbl);
s32 fn_1_A4D4(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_A554(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_A61C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_A6FC(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_A898(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_A9D8(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_ADE8(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_AF84(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_B120(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_B534(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_B6D0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_B86C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_BC80(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_BDF4(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_BF68(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_C36C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_C4E0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_C654(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_CAA0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_CF8C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_D478(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_DF0C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_E130(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_E354(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_E7C0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_E9E4(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_EC08(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_F074(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_F120(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_F19C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_F378(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_F518(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_F614(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_F710(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_F924(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_FB38(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_FC50(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_FDE0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_FF08(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_100A8(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_10220(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_104C0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_10760(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_108A0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_10B30(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_10C70(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_11188(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_11874(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_1199C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_11B4C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_11C6C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_11F70(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_12268(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_124D8(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_12704(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_129D0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_12C94(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_12F74(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_13254(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_13558(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_13880(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_13AB0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_13DAC(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_13EBC(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_1417C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_1443C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_145C8(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_1472C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_147DC(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_1488C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_14A2C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_14BBC(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_14C80(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_14D44(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_14F24(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_150F4(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_151A4(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_15254(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_153F4(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_15584(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_15CE4(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_16444(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_1664C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_167D0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_168C8(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_169C0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_16AB8(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_16BB0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_16CB0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_16DB0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_17010(NEL_SYS *sys, MDPROCENT *tbl);
void MdMain(void);
void fn_1_638(void);
void LoadProc(void);

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

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


static s32 MdMdlSelTbl[7] = { 0x76, 0x78, 0x7A, 0x7C, 0x7E, 0x80, 0x82 };
static HuVecF MdVecTbl[0x6D] = {
    { 0.0f, 0.0f, 1000.0f },
    { 0.0f, -65.0f, 650.0f },
    { -380.0f, -110.0f, 450.0f },
    { -380.0f, -110.0f, 450.0f },
    { -380.0f, -130.0f, 450.0f },
    { 88.0f, 24.0f, 1.5f },
    { 64.0f, 24.0f, 1.5f },
    { -46.0f, -25.0f, 43.0f },
    { 64.0f, 0.0f, 0.0f },
    { 192.0f, 0.0f, 0.0f },
    { 96.0f, 0.0f, 0.0f },
    { 64.0f, 72.0f, 64.0f },
    { 0.7f, 1.0f, 0.0f },
    { 0.0f, -18.0f, 0.0f },
    { 140.0f, 0.0f, 0.0f },
    { -150.0f, 250.0f, 0.0f },
    { 150.0f, 250.0f, 0.0f },
    { -525.0f, 365.0f, -50.0f },
    { 0.0f, 0.0f, -1000.0f },
    { 0.8f, 0.75f, 1.25f },
    { -435.0f, 330.0f, 0.0f },
    { -345.0f, 200.0f, 0.0f },
    { -115.0f, 200.0f, 0.0f },
    { 115.0f, 200.0f, 0.0f },
    { 345.0f, 200.0f, 0.0f },
    { 0.0f, 400.0f, -400.0f },
    { 0.0f, 0.0f, -1000.0f },
    { 0.0f, -400.0f, -2000.0f },
    { 2.0f, 2.5f, 0.1f },
    { -300.0f, 0.0f, 300.0f },
    { -150.0f, 0.0f, 300.0f },
    { 0.0f, 0.0f, 300.0f },
    { 150.0f, 0.0f, 300.0f },
    { 300.0f, 0.0f, 300.0f },
    { -300.0f, -250.0f, 300.0f },
    { -150.0f, -250.0f, 300.0f },
    { 0.0f, -250.0f, 300.0f },
    { 150.0f, -250.0f, 300.0f },
    { 300.0f, -250.0f, 300.0f },
    { -440.0f, 400.0f, -600.0f },
    { -220.0f, 400.0f, -600.0f },
    { 0.0f, 400.0f, -600.0f },
    { 220.0f, 400.0f, -600.0f },
    { 440.0f, 400.0f, -600.0f },
    { -440.0f, 130.0f, -600.0f },
    { -220.0f, 130.0f, -600.0f },
    { 0.0f, 130.0f, -600.0f },
    { 220.0f, 130.0f, -600.0f },
    { 440.0f, 130.0f, -600.0f },
    { 0.0f, 0.0f, -1000.0f },
    { -360.0f, -160.0f, -600.0f },
    { -120.0f, -160.0f, -600.0f },
    { 120.0f, -160.0f, -600.0f },
    { 360.0f, -160.0f, -600.0f },
    { -360.0f, 100.0f, -600.0f },
    { -120.0f, 100.0f, -600.0f },
    { 120.0f, 100.0f, -600.0f },
    { 360.0f, 100.0f, -600.0f },
    { -360.0f, -160.0f, -600.0f },
    { -120.0f, -160.0f, -600.0f },
    { 120.0f, -160.0f, -600.0f },
    { 360.0f, -160.0f, -600.0f },
    { -460.0f, -80.0f, -600.0f },
    { -220.0f, -80.0f, -600.0f },
    { 220.0f, -80.0f, -600.0f },
    { 460.0f, -80.0f, -600.0f },
    { -460.0f, -160.0f, -600.0f },
    { -220.0f, -160.0f, -600.0f },
    { 220.0f, -160.0f, -600.0f },
    { 460.0f, -160.0f, -600.0f },
    { -460.0f, 100.0f, -600.0f },
    { -220.0f, 100.0f, -600.0f },
    { 220.0f, 100.0f, -600.0f },
    { 460.0f, 100.0f, -600.0f },
    { -460.0f, -160.0f, -600.0f },
    { -220.0f, -160.0f, -600.0f },
    { 220.0f, -160.0f, -600.0f },
    { 460.0f, -160.0f, -600.0f },
    { 0.0f, 180.0f, -200.0f },
    { 1.5f, 0.5f, 2.0f },
    { 50.0f, 300.0f, 0.0f },
    { 0.0f, 145.0f, 400.0f },
    { 525.0f, 365.0f, -50.0f },
    { 435.0f, 330.0f, 0.0f },
    { 0.0f, 145.0f, 400.0f },
    { -525.0f, 180.0f, -50.0f },
    { -315.0f, 125.0f, 0.0f },
    { 0.0f, 145.0f, 400.0f },
    { 525.0f, 180.0f, -50.0f },
    { 315.0f, 125.0f, 0.0f },
    { 0.8f, 1.0f, 0.0f },
    { -315.0f, 0.0f, 300.0f },
    { -105.0f, 0.0f, 300.0f },
    { -105.0f, 0.0f, 300.0f },
    { 315.0f, 0.0f, 300.0f },
    { -150.0f, 200.0f, 200.0f },
    { -50.0f, 200.0f, 200.0f },
    { 50.0f, 200.0f, 200.0f },
    { 150.0f, 200.0f, 200.0f },
    { 0.0f, 150.0f, 500.0f },
    { 0.0f, -50.0f, 0.0f },
    { 0.0f, 150.0f, -500.0f },
    { 0.0f, -120.0f, 0.0f },
    { 0.0f, 75.0f, 0.0f },
    { 0.0f, -120.0f, 0.0f },
    { 60.0f, 0.0f, 270.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, -270.0f, 0.0f },
    { 3.0f, 1.0f, 0.0f },
};

MDFLOWENT MdFlowTbl[13] = {
    MdInit,
    MdClose,
    fn_1_1108,
    fn_1_15EC,
    fn_1_1BC8,
    fn_1_2390,
    fn_1_2C88,
    fn_1_46F4,
    fn_1_5B18,
    fn_1_716C,
    fn_1_7C88,
    fn_1_8F18,
    fn_1_974C,
};

MDPROCENT MdProcTbl[0x5B] = {
    fn_1_A4D4,
    fn_1_A554,
    fn_1_A61C,
    fn_1_A6FC,
    fn_1_A898,
    fn_1_A9D8,
    fn_1_ADE8,
    fn_1_AF84,
    fn_1_B120,
    fn_1_B534,
    fn_1_B6D0,
    fn_1_B86C,
    fn_1_BC80,
    fn_1_BDF4,
    fn_1_BF68,
    fn_1_C36C,
    fn_1_C4E0,
    fn_1_C654,
    fn_1_CAA0,
    fn_1_CF8C,
    fn_1_D478,
    fn_1_DF0C,
    fn_1_E130,
    fn_1_E354,
    fn_1_E7C0,
    fn_1_E9E4,
    fn_1_EC08,
    fn_1_F074,
    fn_1_F120,
    fn_1_F19C,
    fn_1_F378,
    fn_1_F518,
    fn_1_F614,
    fn_1_F710,
    fn_1_F924,
    fn_1_FB38,
    fn_1_FC50,
    fn_1_FDE0,
    fn_1_FF08,
    fn_1_100A8,
    fn_1_10220,
    fn_1_104C0,
    fn_1_10760,
    fn_1_108A0,
    fn_1_10B30,
    fn_1_10C70,
    fn_1_11188,
    fn_1_11874,
    fn_1_1199C,
    fn_1_11B4C,
    fn_1_11C6C,
    fn_1_11F70,
    fn_1_12268,
    fn_1_124D8,
    fn_1_12704,
    fn_1_129D0,
    fn_1_12C94,
    fn_1_12F74,
    fn_1_13254,
    fn_1_13558,
    fn_1_13880,
    fn_1_13AB0,
    fn_1_13DAC,
    fn_1_13EBC,
    fn_1_1417C,
    fn_1_1443C,
    fn_1_145C8,
    fn_1_1472C,
    fn_1_147DC,
    fn_1_1488C,
    fn_1_14A2C,
    fn_1_14BBC,
    fn_1_14C80,
    fn_1_14D44,
    fn_1_14F24,
    fn_1_150F4,
    fn_1_151A4,
    fn_1_15254,
    fn_1_153F4,
    fn_1_15584,
    fn_1_15CE4,
    fn_1_16444,
    fn_1_1664C,
    fn_1_167D0,
    fn_1_168C8,
    fn_1_169C0,
    fn_1_16AB8,
    fn_1_16BB0,
    fn_1_16CB0,
    fn_1_16DB0,
    fn_1_17010,
};

void ObjectSetup(void)
{
    NEL_SYS *sys = &sysWork;

    sys->unk234 = -1;
    sys->unk23D = 0;
    sys->stat = 0;
    sys->timer = 0;
    _ClearFlag(0x1000E);
    fn_1_19CCC();
    sys->objMan = omInitObjMan(0x200, 0x2000);
    sys->ovlHis = omOvlHisGet(0);
    sys->ovlEvtNo = ((OMOVLHIS *)sys->ovlHis)->evtno;
    omGameSysInit(sys->objMan);
    sys->fontObj = omAddObjEx(sys->objMan, 0x64, 0, 0, -1, MdObjDebugFontCreate);
    ((OMOBJ *)sys->fontObj)->data = HuMemDirectMallocNum(HEAP_HEAP, 0x1C, HU_MEMNUM_OVL);
    if (((OMOBJ *)sys->fontObj)->data == 0) {
        OSReport("sys->obj[NEL_OBJ_DEBUGFONT]->data = NULL...\n");
    }
    sys->fontData = (s32 *)((OMOBJ *)sys->fontObj)->data;
    *sys->fontData = 0;
    sys->padObj = omAddObjEx(sys->objMan, 0x65, 0, 0, -1, (OMOBJFUNC)MdObjPadMain);
    sys->cameraObj = omAddObjEx(sys->objMan, 0x66, 0, 0, -1, MdObjCameraCreate);
    ((OMOBJ *)sys->cameraObj)->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(SYSCAMERA_WORK), HU_MEMNUM_OVL);
    if (((OMOBJ *)sys->cameraObj)->data == 0) {
        OSReport("sys->obj[NEL_OBJ_CAMERA]->data = NULL...\n");
    }
    sys->camera = ((OMOBJ *)sys->cameraObj)->data;
    sys->camera->stat = 0;
    sys->lightObj = omAddObjEx(sys->objMan, 0x67, 0, 0, -1, MdObjLightCreate);
    ((OMOBJ *)sys->lightObj)->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(SYSLIGHT), HU_MEMNUM_OVL);
    if (((OMOBJ *)sys->lightObj)->data == 0) {
        OSReport("sys->obj[NEL_OBJ_LIGHT]->data = NULL...\n");
    }
    sys->light = ((OMOBJ *)sys->lightObj)->data;
    sys->light->stat = 0;
    sys->sprite = HuMemDirectMallocNum(HEAP_HEAP, sizeof(SYSSPRITE), HU_MEMNUM_OVL);
    if (sys->sprite == 0) {
        OSReport("sys->sprite = NULL...\n");
    }
    sys->model = HuMemDirectMallocNum(HEAP_HEAP, sizeof(SYSMODEL), HU_MEMNUM_OVL);
    if (sys->model == 0) {
        OSReport("sys->model = NULL...\n");
    }
    sys->texbuf = HuMemDirectMallocNum(HEAP_HEAP, sizeof(SYSTEXBUF), HU_MEMNUM_OVL);
    if (sys->texbuf == 0) {
        OSReport("sys->texbuf = NULL...\n");
    }
    sys->particle = HuMemDirectMallocNum(HEAP_HEAP, sizeof(SYSPARTICLE_WORK), HU_MEMNUM_OVL);
    if (sys->particle == 0) {
        OSReport("sys->particle = NULL...\n");
    }
    sys->ring = HuMemDirectMallocNum(HEAP_HEAP, sizeof(SYSRING_WORK), HU_MEMNUM_OVL);
    if (sys->ring == 0) {
        OSReport("sys->ring = NULL...\n");
    }
    sys->fuki = HuMemDirectMallocNum(HEAP_HEAP, 0x80, HU_MEMNUM_OVL);
    if (sys->fuki == 0) {
        OSReport("sys->fuki = NULL...\n");
    }
    sys->mess = HuMemDirectMallocNum(HEAP_HEAP, sizeof(SYSMESS), HU_MEMNUM_OVL);
    if (sys->mess == 0) {
        OSReport("sys->mess = NULL...\n");
    }
    sys->mess->stat = 0;
    MdMathStdCurveInit();
    sys->messProc = HuPrcChildCreate(MdMessProc, 0x6A, 0x3000, 0, sys->objMan);
    Hu3DBGColorSet(0, 0, 0);
    Hu3DNoSyncSet(0);
    while (1) {
        if (*sys->fontData == 1 && sys->camera->stat == 1 && sys->light->stat == 1 && sys->mess->stat == 1) {
            break;
        }
        HuPrcVSleep();
    }
    sys->mainProc = HuPrcChildCreate(MdMain, 0x68, 0x3000, 0, sys->objMan);
    sys->unk234 = 0;
}

void MdMain(void)
{
    NEL_SYS *sys = &sysWork;

    sys->timer = 0;
    sys->frameCnt = 0;
    sys->stat = 0;
    sys->flowNo = 2;
    sys->exitReqF = 0;
    sys->unk23F = 1;
    sys->unk240 = 0;
    sys->unk288 = 0;
    sys->ovlMode = 0;
    sys->ovlNo = 0x70;
    if (sys->ovlEvtNo == 1) {
        omOvlReturnEx(1, 1);
        HuPrcEnd();
        while (1) {
            HuPrcVSleep();
        }
    }
    MdProcTbl->func(sys, MdProcTbl);
    while (omSysExitReq == 0 && sys->exitReqF == 0) {
        (MdFlowTbl + sys->flowNo)->func(sys, MdFlowTbl + sys->flowNo);
        MdRingMain(sys);
        MdFukiMain(sys);
        NelGuideBillboardUpdate(sys, 0x3B);
        NelGuideBillboardUpdate(sys, sys->menuNo2 + 0x3C);
        NelGuideBillboardUpdate(sys, 0x41);
        NelGuideBillboardUpdate(sys, 0x32);
        NelGuideBillboardUpdate(sys, 0x33);
        sys->frameCnt++;
        HuPrcVSleep();
    }
    MdProcTbl[1].func(sys, MdProcTbl);
}

void fn_1_638(void)
{
    NEL_SYS *sys = &sysWork;
    NEL_BLENDDBG *dbg = &sys->blendDbg;

    sys->timer = 0;
    sys->stat = 0;
    MdProcTbl->func(sys, MdProcTbl);
    WipeCreate(1, 0, 0x3C);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
    while (omSysExitReq == 0) {
        MdBlendDebugMain();
        MdSpriteDebugMain();
        HuPrcVSleep();
    }
    MdProcTbl[1].func(sys, MdProcTbl);
}

void LoadProc(void)
{
    NEL_SYS *sys = &sysWork;
    s32 tick;

    while (1) {
        switch (sys->loadStat) {
        case 0:
            break;
        case 1:
            tick = OSGetTick();
            sys->asyncStat = HuDataDirReadAsync(sys->dataDirNo);
            while (HuDataGetAsyncStat(sys->asyncStat) == 0) {
                HuPrcVSleep();
            }
            OSReport("Load Time:%d\n", OSTicksToMilliseconds((u32)(OSGetTick() - tick)));
            OSReport("Finished!\n");
            sys->loadStat = 2;
            break;
        case 2:
            HuPrcEnd();
            break;
        }
        HuPrcVSleep();
    }
}

void MdInit(NEL_SYS *sys, MDFLOWENT *tbl)
{
    SYSMODEL *model;
    SYSCAMERA_WORK *camera;
    SYSSPRITE *sprite;
    s32 i;

    sprite = (SYSSPRITE *)sys->sprite;
    model = (SYSMODEL *)sys->model;
    camera = (SYSCAMERA_WORK *)sys->camera;
    sys->stat = 0;
    sys->timer = 0;
    sys->angle = 90.0f + 60.0f * sys->unk23D;
    MdMathAngleClamp(&sys->angle);
    MdCameraPosSet(&camera->camera[2], 90.0, 0.0, 1920.0, 0.0, 0.0, 0.0);
    MdCameraUpdate(camera);
    Hu3DModelAttrSet(model->mdlId[0], 1);
    Hu3DModelAttrSet(model->mdlId[1], 1);
    for (i = 0; i < 10; i++) {
        Hu3DModelAttrSet(model->mdlId[i + 2], 1);
    }
    Hu3DModelAttrReset(model->mdlId[12], 1);
    Hu3DModelAttrSet(model->mdlId[15], 1);
    for (i = 0; i < 10; i++) {
        Hu3DModelAttrSet(model->mdlId[i + 16], 1);
    }
    for (i = 0; i < 6; i++) {
        Hu3DModelAttrReset(model->mdlId[i + 26], 1);
    }
    for (i = 0; i < 7; i++) {
        Hu3DModelAttrSet(model->mdlId[i + 32], 1);
    }
    for (i = 0; i < 11; i++) {
        Hu3DModelAttrSet(model->mdlId[i + 39], 1);
    }
    Hu3DModelAttrSet(model->mdlId[50], 1);
    Hu3DModelAttrSet(model->mdlId[51], 1);
    for (i = 0; i < 4; i++) {
        Hu3DModelAttrSet(model->mdlId[i + 52], 1);
    }
    for (i = 0; i < 3; i++) {
        Hu3DModelAttrSet(model->mdlId[i + 56], 1);
    }
    Hu3DModelAttrSet(model->mdlId[59], 1);
    for (i = 0; i < 2; i++) {
        Hu3DModelAttrSet(model->mdlId[i + 60], 1);
    }
    Hu3DModelAttrSet(model->mdlId[65], 1);
    for (i = 0; i < 4; i++) {
        Hu3DModelAttrSet(model->mdlId[i + 66], 1);
    }
    Hu3DModelAttrReset(model->mdlId[13], 1);
    Hu3DModelAttrSet(model->mdlId[74], 1);
    fn_1_F94(sys, sys->angle);
    MdSpriteDispSet(1, 0);
    MdSpriteDispSet(2, 0);
    MdSpriteDispSet(3, 0);
    MdSpriteDispSet(4, 0);
    MdSpriteDispSet(5, 0);
    MdSpriteDispSet(6, 0);
    MdSpriteDispSet(7, 0);
    for (i = 0; i < 6; i++) {
        Hu3DModelLayerSet(model->mdlId[i + 26], 3);
    }
    Hu3DLayerHookSet(8, NULL);
    Hu3DLayerHookSet(8, NULL);
    Hu3DLayerHookSet(9, NULL);
    Hu3DLayerHookSet(8, NULL);
    Hu3DLayerHookSet(2, NULL);
    Hu3DLayerHookSet(3, MdLayer1Hook);
    Hu3DLayerHookSet(2, NULL);
    Hu3DLayerHookSet(10, NULL);
    Hu3DLayerHookSet(7, MdLayer1Hook);
    MdMessDlgMesSet(0, -1);
    MdMessWinKill(2);
    if (sys->unk23F == 0) {
        WipeColorSet(0, 0, 0);
    } else if (sys->unk23F == 1) {
        WipeColorSet(255, 255, 255);
    }
    WipeCreate(1, 5, 30);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
    sys->stat = 0;
    sys->flowNo = 1;
}

void MdClose(NEL_SYS *sys, MDFLOWENT *tbl)
{
    s32 idx;
    s32 volatile ret;
    SYSMODEL *volatile model;
    SYSMESS *mess;

    model = (SYSMODEL *)sys->model;
    mess = (SYSMESS *)sys->mess;
    switch (sys->stat) {
        case 0:
            MdMessDlgMesSet(0, -1);
            MdMessDlgMesSet(1, 0x40000);
            sys->stat = 1;
            return;
        case 1:
            if (mess->mode == 5) {
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(1, sys->unk23D + 0x40002);
                sys->stat = 2;
                return;
            }
            break;
        case 2:
            if ((PADSTAT(sys) & 1) != 0 || (PADSTAT(sys) & 2) != 0) {
                MdMessDlgMesSet(0, -1);
                if ((PADSTAT(sys) & 1) != 0) {
                    sys->stat = 4;
                    return;
                }
                if ((PADSTAT(sys) & 2) != 0) {
                    sys->stat = 5;
                    return;
                }
            } else {
                if ((HuPadBtnDown[0] & PAD_BUTTON_B) != 0) {
                    MdMessDlgMesSet(0, -1);
                    MdMessDlgMesSet(2, 0x40001);
                    sys->stat = 3;
                    return;
                }
                if ((HuPadBtnDown[0] & PAD_BUTTON_A) != 0 && sys->unk23D == 0) {
                    sys->stat = 6;
                    return;
                }
            }
            break;
        case 3:
            if (mess->mode == 5) {
                if (mess->choice == -1 || mess->choice == 1) {
                    MdMessDlgMesSet(0, -1);
                    MdMessDlgMesSet(1, sys->unk23D + 0x40002);
                    sys->stat = 2;
                    return;
                } else if (mess->choice == 0) {
                    sys->stat = 7;
                    return;
                }
            }
            break;
        case 4:
        case 5:
            idx = sys->stat + 0x1D;
            ret = (MdProcTbl + idx)->func(sys, MdProcTbl + idx);
            if (ret == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                if (sys->stat == 4) {
                    if (--sys->unk23D == -1) {
                        sys->unk23D = 5;
                    }
                } else if (sys->stat == 5) {
                    sys->unk23D = (sys->unk23D + 1) % 6;
                }
                MdMathAngleClamp(&sys->angle);
                MdMessDlgMesSet(1, sys->unk23D + 0x40002);
                sys->stat = 2;
                return;
            }
            break;
        case 6:
            WipeColorSet(255, 255, 255);
            WipeCreate(2, 0, 30);
            while (WipeCheckEnd()) {
                HuPrcVSleep();
            }
            sys->stat = 0;
            sys->flowNo = 2;
            return;
        case 7:
            sys->ovlMode = 2;
            sys->ovlNo = 0x61;
            sys->exitReqF = 1;
            break;
        default:
            break;
    }
}

void fn_1_F94(NEL_SYS *sys, volatile float angle)
{
    s32 i;
    SYSMODEL *model;
    float ang;
    float r = 200.0f;
    float y = -200.0f;
    float z;

    ang = 0.0f;
    model = (SYSMODEL *)sys->model;
    for (i = 0; i < 6; i++) {
        ang = angle + 60.0f * i;
        z = (float)(400.0 + r * sin(3.141592653589793 * ang / 180.0));
        Hu3DModelPosSet(model->mdlId[i + 26], r * cos(3.141592653589793 * ang / 180.0), y, z);
    }
}

void fn_1_1108(NEL_SYS *sys, MDFLOWENT *tbl)
{
    SYSSPRITE *sprite;
    SYSMODEL *model;
    SYSCAMERA_WORK *camera;
    s32 i;

    sprite = (SYSSPRITE *)sys->sprite;
    model = (SYSMODEL *)sys->model;
    camera = (SYSCAMERA_WORK *)sys->camera;
    sys->stat = 0;
    sys->timer = 0;
    sys->angle = 0.0f;
    sys->menuNo = 2;
    sys->menuNo2 = 0;
    sys->menuNo3 = 0;
    for (i = 0; i < 4; i++) {
        sys->unk283[i] = 0;
    }
    MdProcTbl[2].func(sys, MdProcTbl);
    MdCameraPosSet(&camera->camera[2], 90.0, 0.0, 1920.0, 0.0, 0.0, 0.0);
    MdCameraUpdate(camera);
    Hu3DModelAttrSet(model->mdlId[0], 1);
    Hu3DModelAttrSet(model->mdlId[1], 1);
    for (i = 0; i < 10; i++) {
        Hu3DModelAttrSet(model->mdlId[i + 2], 1);
    }
    Hu3DModelAttrSet(model->mdlId[15], 1);
    for (i = 0; i < 10; i++) {
        Hu3DModelAttrSet(model->mdlId[i + 16], 1);
    }
    for (i = 0; i < 6; i++) {
        Hu3DModelAttrSet(model->mdlId[i + 26], 1);
    }
    for (i = 0; i < 7; i++) {
        Hu3DModelAttrSet(model->mdlId[i + 32], 1);
    }
    Hu3DModelAttrSet(model->mdlId[50], 1);
    Hu3DModelAttrSet(model->mdlId[51], 1);
    for (i = 0; i < 4; i++) {
        Hu3DModelAttrSet(model->mdlId[i + 52], 1);
    }
    for (i = 0; i < 3; i++) {
        Hu3DModelAttrSet(model->mdlId[i + 56], 1);
    }
    Hu3DModelAttrSet(model->mdlId[59], 1);
    for (i = 0; i < 5; i++) {
        Hu3DModelAttrSet(model->mdlId[i + 60], 1);
    }
    Hu3DModelAttrSet(model->mdlId[65], 1);
    Hu3DModelAttrReset(model->mdlId[12], 1);
    Hu3DModelAttrReset(model->mdlId[26], 1);
    Hu3DModelAttrReset(model->mdlId[74], 1);
    Hu3DModelAttrSet(model->mdlId[12], 1);
    Hu3DModelAttrSet(model->mdlId[13], 1);
    Hu3DModelPosSet(model->mdlId[26], MdVecTbl[0].x, MdVecTbl[0].y, MdVecTbl[0].z);
    fn_1_19B10(0);
    for (i = 0; i < 2; i++) {
        fn_1_198E0(model->mdlId[i + 50], 1.0f);
    }
    for (i = 0; i < 7; i++) {
        fn_1_198E0(model->mdlId[i + 32], 1.0f);
    }
    fn_1_198E0(model->mdlId[59], 1.0f);
    for (i = 0; i < 5; i++) {
        fn_1_198E0(model->mdlId[i + 60], 1.0f);
    }
    fn_1_198E0(model->mdlId[65], 1.0f);
    MdSpriteDispSet(1, 0);
    MdSpriteDispSet(2, 0);
    MdSpriteDispSet(3, 0);
    MdSpriteDispSet(4, 0);
    MdSpriteDispSet(5, 0);
    MdSpriteDispSet(6, 0);
    MdSpriteDispSet(7, 0);
    for (i = 0; i < 11; i++) {
        MdRingDispSet(sys, i, 0);
    }
    Hu3DModelLayerSet(model->mdlId[26], 7);
    fn_1_193FC(sys, 0);
    Hu3DLayerHookSet(8, NULL);
    Hu3DLayerHookSet(8, NULL);
    Hu3DLayerHookSet(9, NULL);
    Hu3DLayerHookSet(8, NULL);
    Hu3DLayerHookSet(2, NULL);
    Hu3DLayerHookSet(3, NULL);
    Hu3DLayerHookSet(2, NULL);
    Hu3DLayerHookSet(10, MdLayer1Hook);
    MdMessDlgMesSet(0, -1);
    MdMessWinKill(2);
    WipeColorSet(255, 255, 255);
    WipeCreate(1, 0, 30);
    fn_1_19904(0);
    sys->flowNo = 3;
}

void fn_1_15EC(NEL_SYS *sys, MDFLOWENT *tbl)
{
    s32 volatile ret;
    SYSMODEL *volatile model;
    SYSMESS *mess;

    model = (SYSMODEL *)sys->model;
    mess = (SYSMESS *)sys->mess;
    switch (sys->stat) {
        case 0:
            if (WipeCheckEnd() == 0) {
                sys->stat = 1;
                return;
            }
            break;
        case 1:
            if (++sys->timer >= 0x78) {
                MdMessDlgMesSet(1, 0x60000);
                fn_1_19904(9);
                fn_1_19B10(1);
                sys->timer = 0;
                sys->stat = 2;
                return;
            }
            break;
        case 2:
            if (mess->mode == 5) {
                if (GwCommon.partyContinue == 1) {
                    sys->stat = 3;
                    return;
                }
                sys->stat = 8;
                return;
            }
            break;
        case 3:
            MdMessDlgMesSet(0, -1);
            MdMessDlgMesSet(2, 0x60001);
            sys->stat = 4;
            return;
        case 4:
            if (mess->mode == 5) {
                if (mess->choice == -1) {
                    MdMessDlgMesSet(0, -1);
                    MdMessDlgMesSet(2, 0x60003);
                    sys->stat = 6;
                    return;
                } else if (mess->choice == 0) {
                    SLBoardLoad();
                    sys->stat = 5;
                    return;
                } else if (mess->choice == 1) {
                    MdMessDlgMesSet(0, -1);
                    MdMessDlgMesSet(1, 0x60002);
                    sys->stat = 8;
                    return;
                }
            }
            break;
        case 5:
            WipeColorSet(255, 255, 255);
            WipeCreate(2, 0, 30);
            fn_1_19904(1);
            while (WipeCheckEnd()) {
                HuPrcVSleep();
            }
            CharDataClose(-1);
            CharMotionInit(GwPlayerConf[0].charNo);
            CharMotionInit(GwPlayerConf[1].charNo);
            CharMotionInit(GwPlayerConf[2].charNo);
            CharMotionInit(GwPlayerConf[3].charNo);
            omOvlHisChg(0, ((OMOVLHIS *)sys->ovlHis)->ovl, 1, ((OMOVLHIS *)sys->ovlHis)->stat);
            sys->ovlMode = 1;
            sys->ovlNo = MdMdlSelTbl[GwSystem.boardNo];
            sys->unk240 = 1;
            sys->exitReqF = 1;
            return;
        case 6:
            if (mess->mode == 5) {
                if (mess->choice == -1 || mess->choice == 1) {
                    sys->stat = 3;
                    return;
                } else if (mess->choice == 0) {
                    sys->stat = 7;
                    return;
                }
            }
            break;
        case 7:
            WipeColorSet(255, 255, 255);
            WipeCreate(2, 0, 30);
            fn_1_19904(1);
            while (WipeCheckEnd()) {
                HuPrcVSleep();
            }
            omOvlHisChg(0, ((OMOVLHIS *)sys->ovlHis)->ovl, 0, ((OMOVLHIS *)sys->ovlHis)->stat);
            sys->ovlMode = 2;
            sys->ovlNo = 0x61;
            sys->unk240 = 1;
            sys->exitReqF = 1;
            return;
        case 8:
            if (mess->mode == 5) {
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(2, 0x60004);
                sys->stat = 9;
                return;
            }
            break;
        case 9:
            if (mess->mode == 5) {
                if (mess->choice == -1) {
                    MdMessDlgMesSet(0, -1);
                    MdMessDlgMesSet(2, 0x60003);
                    sys->stat = 11;
                    return;
                } else if (mess->choice == 0) {
                    sys->stat = 10;
                    return;
                } else if (mess->choice == 1) {
                    MdMessDlgMesSet(0, -1);
                    MdMessDlgMesSet(1, 0x60005);
                    sys->stat = 12;
                    return;
                }
            }
            break;
        case 10:
            WipeColorSet(255, 255, 255);
            WipeCreate(2, 0, 30);
            fn_1_19904(1);
            while (WipeCheckEnd()) {
                HuPrcVSleep();
            }
            CharDataClose(-1);
            CharMotionInit(3);
            CharMotionInit(0);
            CharMotionInit(2);
            CharMotionInit(4);
            MBPartySaveInit(7);
            omOvlHisChg(0, ((OMOVLHIS *)sys->ovlHis)->ovl, 0, ((OMOVLHIS *)sys->ovlHis)->stat);
            _SetFlag(0x1000E);
            GwSystem.teamF = 0;
            sys->ovlMode = 1;
            sys->ovlNo = 0x84;
            sys->unk240 = 1;
            sys->exitReqF = 1;
            return;
        case 11:
            if (mess->mode == 5) {
                if (mess->choice == -1 || mess->choice == 1) {
                    sys->stat = 8;
                    return;
                } else if (mess->choice == 0) {
                    sys->stat = 7;
                    return;
                }
            }
            break;
        case 12:
            if (mess->mode == 5) {
                MdMessDlgMesSet(7, -1);
                sys->stat = 14;
                return;
            }
            break;
        case 14:
            fn_1_19B10(2);
            sys->stat = 13;
            return;
        case 13:
            ret = MdProcTbl[0x24].func(sys, MdProcTbl);
            if (ret == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                MdMessDlgMesSet(0, -1);
                sys->stat = 0;
                sys->flowNo = 4;
            }
            break;
    }
}

void fn_1_1BC8(NEL_SYS *sys, MDFLOWENT *tbl)
{
    SYSMODEL *model;
    s16 grp;
    SYSMESS *mess;
    s8 i;
    SYSSPRITE *sprite;

    sprite = (SYSSPRITE *)sys->sprite;
    model = (SYSMODEL *)sys->model;
    mess = (SYSMESS *)sys->mess;
    switch (sys->stat) {
        case 0:
            Hu3DModelAttrReset(model->mdlId[50], 1);
            Hu3DModelAttrReset(model->mdlId[51], 1);
            Hu3DModelPosSet(model->mdlId[50], MdVecTbl[15].x, MdVecTbl[15].y, MdVecTbl[15].z);
            Hu3DModelPosSet(model->mdlId[51], MdVecTbl[16].x, MdVecTbl[16].y, MdVecTbl[16].z);
            Hu3DModelScaleSet(model->mdlId[50], 0.0f, 0.0f, 0.0f);
            Hu3DModelScaleSet(model->mdlId[51], 0.0f, 0.0f, 0.0f);
            grp = sprite->grpId[1];
            HuSprAttrReset(grp, 0, 4);
            HuSprAttrReset(grp, 1, 4);
            HuSprAttrSet(grp, 2, 4);
            HuSprAttrSet(grp, 3, 4);
            HuSprGrpPosSet(grp, 288.0f, 240.0f);
            HuSprPosSet(grp, 0, 0.0f, -300.0f);
            HuSprScaleSet(grp, 0, 1.0f, 1.0f);
            HuSprPosSet(grp, 1, 0.0f, 300.0f);
            HuSprScaleSet(grp, 1, 1.0f, 1.0f);
            Hu3DLayerHookSet(9, NULL);
            MdMessWinMesSet(2, 0x20004);
            MdMessWinPosSet(2, 78.0f, 340.0f);
            MdMessWinSpeedSet(2, 0);
            fn_1_28F74(2);
            for (i = 0; i < 7; i++) {
                MdRingModeSet(sys, i, 15);
            }
            MdMessDlgMesSet(8, -1);
            sys->stat = 11;
            return;
        case 11:
            sys->stat = 1;
            return;
        case 1:
            if (MdProcTbl[0x26].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                fn_1_19B10(1);
                MdMessDlgMesSet(0, -1);
                if (sys->modeSel == 0) {
                    MdMessDlgMesSet(1, 0x60007);
                } else if (sys->modeSel == 1) {
                    MdMessDlgMesSet(1, 0x60008);
                }
                fn_1_28FAC(2, 0x20004);
                sys->stat = 2;
                return;
            }
            break;
        case 2:
            if (MdProcTbl[3].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 3;
                return;
            }
            break;
        case 3:
            if ((PADSTAT(sys) & 1) != 0 || (PADSTAT(sys) & 2) != 0) {
                if ((PADSTAT(sys) & 1) != 0) {
                    sys->sel = 0;
                    if (--sys->modeSel == -1) {
                        sys->modeSel = 1;
                    }
                } else if ((PADSTAT(sys) & 2) != 0) {
                    sys->sel = 1;
                    sys->modeSel = (sys->modeSel + 1) % 2;
                }
                fn_1_19904(2);
                MdMessDlgMesSet(0, -1);
                switch (sys->modeSel) {
                    case 0:
                        MdMessDlgMesSet(1, 0x60007);
                        sys->stat = 4;
                        return;
                    case 1:
                        MdMessDlgMesSet(1, 0x60008);
                        sys->stat = 5;
                        return;
                }
            } else {
                if ((HuPadBtnDown[0] & PAD_BUTTON_A) != 0) {
                    MdProcTbl[2].func(sys, MdProcTbl);
                    fn_1_17930(sys);
                    fn_1_19904(3);
                    sys->stat = 9;
                    MdMessDlgMesSet(9, -1);
                    fn_1_28F74(2);
                    return;
                }
                if ((HuPadBtnDown[0] & PAD_BUTTON_B) != 0) {
                    MdProcTbl[2].func(sys, MdProcTbl);
                    fn_1_19904(5);
                    fn_1_28F74(2);
                    sys->stat = 6;
                    return;
                }
            }
            break;
        case 4:
        case 5:
            if (MdProcTbl[0x27].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 3;
                return;
            }
            break;
        case 6:
            if (MdProcTbl[4].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(2, 0x60003);
                sys->stat = 7;
                return;
            }
            break;
        case 7:
            if (mess->mode == 5) {
                if (mess->choice == -1 || mess->choice == 1) {
                    MdMessDlgMesSet(0, -1);
                    if (sys->modeSel == 0) {
                        MdMessDlgMesSet(1, 0x60007);
                    } else if (sys->modeSel == 1) {
                        MdMessDlgMesSet(1, 0x60008);
                    }
                    fn_1_28FAC(2, 0x20004);
                    sys->stat = 2;
                    return;
                } else if (mess->choice == 0) {
                    sys->stat = 8;
                    return;
                }
            }
            break;
        case 8:
            WipeColorSet(255, 255, 255);
            WipeCreate(2, 0, 30);
            fn_1_19904(1);
            while (WipeCheckEnd()) {
                HuPrcVSleep();
            }
            omOvlHisChg(0, ((OMOVLHIS *)sys->ovlHis)->ovl, 0, ((OMOVLHIS *)sys->ovlHis)->stat);
            sys->unk240 = 1;
            sys->ovlMode = 2;
            sys->ovlNo = 0x61;
            sys->exitReqF = 1;
            return;
        case 9:
            if (MdProcTbl[4].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 10;
                return;
            }
            break;
        case 10:
            if (MdProcTbl[0x28].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 0;
                sys->flowNo = 5;
            }
            break;
    }
}

void fn_1_2390(NEL_SYS *sys, MDFLOWENT *tbl)
{
    s32 i;
    s16 grp;
    SYSSPRITE *sprite;
    SYSMODEL *model;
    s32 volatile mess;

    sprite = (SYSSPRITE *)sys->sprite;
    model = (SYSMODEL *)sys->model;
    mess = (s32)sys->mess;
    switch (sys->stat) {
        case 0:
            for (i = 0; i < 4; i++) {
                Hu3DModelPosSet(model->mdlId[i + 52], MdVecTbl[i + 21].x, MdVecTbl[i + 21].y, MdVecTbl[i + 21].z);
                Hu3DModelScaleSet(model->mdlId[i + 52], 0.0f, 0.0f, 0.0f);
                if (i <= sys->playerNum) {
                    Hu3DModelAttrReset(model->mdlId[i + 52], 1);
                } else {
                    Hu3DModelAttrSet(model->mdlId[i + 52], 1);
                }
            }
            for (i = 0; i < 3; i++) {
                Hu3DModelPosSet(model->mdlId[i + 56], MdVecTbl[i + 22].x, MdVecTbl[i + 22].y, MdVecTbl[i + 22].z);
                Hu3DModelScaleSet(model->mdlId[i + 56], 0.0f, 0.0f, 0.0f);
                if (i >= sys->playerNum) {
                    Hu3DModelAttrReset(model->mdlId[i + 56], 1);
                } else {
                    Hu3DModelAttrSet(model->mdlId[i + 56], 1);
                }
            }
            for (i = 0; i < 7; i++) {
                NelGuideBillboardUpdate(sys, i + 0x34);
            }
            if (sys->unk242 > 1) {
                grp = sprite->grpId[1];
                HuSprAttrReset(grp, 0, 4);
                HuSprAttrReset(grp, 1, 4);
                HuSprGrpPosSet(grp, 288.0f, 240.0f);
                HuSprPosSet(grp, 0, 0.0f, -300.0f);
                HuSprScaleSet(grp, 0, 1.0f, 1.0f);
                HuSprPosSet(grp, 1, 0.0f, 300.0f);
                HuSprScaleSet(grp, 1, 1.0f, 1.0f);
            }
            sys->stat = 1;
            return;
        case 1:
            if (MdProcTbl[0x2B].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                MdMessDlgMesSet(0, -1);
                if (sys->playerNum == 3) {
                    MdMessDlgMesSet(1, 0x6000E);
                } else {
                    MdMessDlgMesSet(3, 0x6000A);
                    MdMessDlgInsertMesReset();
                    MdMessDlgInsertMesAdd(sys->playerNum + 0x6000B, 0);
                    MdMessDlgInsertMesAdd(0x6000D - sys->playerNum, 1);
                }
                fn_1_28FAC(2, 0x20004);
                if (sys->unk242 == 1) {
                    sys->stat = 3;
                    return;
                }
                sys->stat = 2;
                return;
            }
            break;
        case 2:
            if (MdProcTbl[6].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 3;
                return;
            }
            break;
        case 3:
            NEL_INT10(sys) = 0;
            sys->timer = 0;
            sys->stat = 4;
            return;
        case 4:
            if (sys->unk242 > 1 && ((PADSTAT(sys) & 1) != 0 || (PADSTAT(sys) & 2) != 0)) {
                sys->selSub = sys->playerNum;
                if ((PADSTAT(sys) & 1) != 0) {
                    sys->sel = 0;
                    if (--sys->playerNum == -1) {
                        sys->playerNum = sys->unk242 - 1;
                    }
                } else if ((PADSTAT(sys) & 2) != 0) {
                    sys->sel = 1;
                    sys->playerNum = (sys->playerNum + 1) % sys->unk242;
                }
                fn_1_19904(2);
                NEL_INT10(sys) = 1;
                MdMessDlgMesSet(0, -1);
                if (sys->playerNum == 3) {
                    MdMessDlgMesSet(1, 0x6000E);
                } else {
                    MdMessDlgMesSet(3, 0x6000A);
                    MdMessDlgInsertMesReset();
                    MdMessDlgInsertMesAdd(sys->playerNum + 0x6000B, 0);
                    MdMessDlgInsertMesAdd(0x6000D - sys->playerNum, 1);
                }
                for (i = 0; i < 7; i++) {
                    NelGuideBillboardUpdate(sys, i + 0x34);
                }
                sys->stat = 10;
                return;
            }
            if ((HuPadBtnDown[0] & PAD_BUTTON_A) != 0) {
                fn_1_28F74(2);
                fn_1_19904(3);
                fn_1_17A2C(sys);
                fn_1_19780(sys);
                if (sys->unk242 == 1) {
                    sys->stat = 9;
                    return;
                }
                sys->stat = 8;
                return;
            }
            if ((HuPadBtnDown[0] & PAD_BUTTON_B) != 0) {
                fn_1_28F74(2);
                fn_1_19904(5);
                if (sys->unk242 == 1) {
                    sys->stat = 6;
                    return;
                }
                sys->stat = 5;
                return;
            }
            break;
        case 10:
            if (MdProcTbl[0x2E].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 4;
                return;
            }
            break;
        case 5:
            if (MdProcTbl[7].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 6;
                return;
            }
            break;
        case 6:
            if (MdProcTbl[0x2C].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                Hu3DModelAttrReset(model->mdlId[50], 1);
                Hu3DModelAttrReset(model->mdlId[51], 1);
                sys->stat = 7;
                return;
            }
            break;
        case 7:
            if (MdProcTbl[0x29].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                HuSprAttrReset(sprite->grpId[1], 0, 4);
                HuSprAttrReset(sprite->grpId[1], 1, 4);
                MdMessDlgMesSet(0, -1);
                if (sys->modeSel == 0) {
                    MdMessDlgMesSet(1, 0x60007);
                } else if (sys->modeSel == 1) {
                    MdMessDlgMesSet(1, 0x60008);
                }
                fn_1_28FAC(2, 0x20004);
                sys->stat = 2;
                sys->flowNo = 4;
                return;
            }
            break;
        case 8:
            if (MdProcTbl[7].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 9;
                return;
            }
            break;
        case 9:
            if (MdProcTbl[0x2C].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 0;
                sys->flowNo = 6;
            }
            break;
    }
}

/* fn_1_18BA0 - copy the per-slot "selected model B" table over
 * the "selected model C2" table (both 4 entries). */
void fn_1_18BA0(NEL_SYS *sys)
{
    s32 i;
    for (i = 0; i < 4; i++) {
        sys->mdlSelNoC2[i] = sys->mdlSelNoB[i];
    }
}

/* fn_1_198E0 - reached from the model-init flow with a model id
 * and a 90-degree angle.  The original resolves the shared model
 * block into a local and returns void, so the store is the only
 * observable effect; reproduced as written rather than as a return. */
void fn_1_198E0(s32 modelId, float angle)
{
    NEL_SYS *sys = &sysWork;
    MD_MODEL *model = sys->model;
}

/* fn_1_19C00 - fuki hook: mark slot idx as "mode 2". */
void fn_1_19C00(NEL_SYS *sys, s32 idx)
{
    sys->fukiMode[idx] = 2;
}

/* fn_1_19C70 - fuki hook: step the slot's mode down (2 -> 0,
 * 3 -> 1), leaving anything else alone. */
void fn_1_19C70(NEL_SYS *sys, s32 idx)
{
    if (sys->fukiMode[idx] == 2) {
        sys->fukiMode[idx] = 0;
    } else if (sys->fukiMode[idx] == 3) {
        sys->fukiMode[idx] = 1;
    }
}

/* fn_1_19CB8 - hand back the shared model block for this sys. */
MD_MODEL *fn_1_19CB8(NEL_SYS *sys)
{
    MD_MODEL *model = sys->model;
    return model;
}

/* MdSpriteDebugMain - drop into the sprite debugger when the
 * blend-debug state machine is parked on its "show" state. */
void MdSpriteDebugMain(void)
{
    if (sysWork.blendDbg.stat == 2) {
        MdSpriteDebug(&sysWork);
    }
}

/* fn_1_1A404 - read model slot 0x20's id out of the shared model
 * block.  As in the original the id lands in a local and the
 * function returns void. */
void fn_1_1A404(void)
{
    NEL_SYS *sys = &sysWork;
    MD_MODEL *model = sys->model;
    s16 mdlId = model->mdlId[0x20];
}

/* ParticleGlowKill - tear a glow particle effect down by its
 * particle-manager handle. */
void ParticleGlowKill(ParticleGlow *p)
{
    Hu3DParManKill((s16)p->parManId);
}

/* ParticleGlowPosSet - move a glow to `pos`, then restart it with a
 * two-frame life limit and attribute bit 1. */
void ParticleGlowPosSet(ParticleGlow *p, Vec *pos)
{
    Hu3DParManPosSet((s16)p->parManId, pos->x, pos->y, pos->z);
    Hu3DParManTimeLimitSet((s16)p->parManId, 2);
    Hu3DParManAttrReset((s16)p->parManId, 1);
}
