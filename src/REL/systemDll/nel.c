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
typedef struct ParticleGlowWork_s {
    /* 0x00 */ int parManId;
} ParticleGlowWork;

/* Per-pad particle work: three dot particles and one glow particle. */
typedef struct ParticleDotWork_s {
    /* 0x00 */ s32 parManId[3];
} ParticleDotWork;

struct {
    struct {
        ParticleDotWork *dot;
        ParticleGlowWork *glow;
    } ent[4];
    s32 pad;
} ParWork;

void MdSpriteDebug(NEL_SYS *sys);
void MdSpriteDispSet(s32 grpNo, s32 dispF);
void MdLayer1Hook(s16 layerNo);
void fn_1_1923C(NEL_SYS *sys);
void fn_1_18910(NEL_SYS *sys);
void fn_1_18084(NEL_SYS *sys);
void fn_1_18BA0(NEL_SYS *sys);
s32 fn_1_1835C(NEL_SYS *sys, s32 idx, s32 flag);
void fn_1_17BD8(NEL_SYS *sys);
void fn_1_17CE4(NEL_SYS *sys);
void fn_1_18834(NEL_SYS *sys);
void fn_1_17FC4(NEL_SYS *sys, s32 a, s32 b);
void fn_1_19818(NEL_SYS *sys);
void fn_1_19C10(NEL_SYS *sys, s32 idx);
void MdFukiGrpSet(NEL_SYS *sys, s32 no, s32 grp);
void MdFukiMemberNoSet(NEL_SYS *sys, s32 no, s32 memberNo);
void MdFukiModeSet(NEL_SYS *sys, s32 no, s32 mode);
void MdFukiHookSet(NEL_SYS *sys, s32 no, SYSFUKI_HOOK hook);
void MdRingMdlSet(NEL_SYS *sys, s32 no, s32 mdlNo);
void MdRingOfsSet(NEL_SYS *sys, s32 no, float x, float y, float z);
void MdToueiLayerHook(s16 layerNo);
void fn_1_19C00(NEL_SYS *sys, s32 idx);
void fn_1_19C70(NEL_SYS *sys, s32 idx);
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

void fn_1_18C68(NEL_SYS *sys, s32 arg);
void MdFukiInit(NEL_SYS *sys);
void fn_1_17354(NEL_SYS *sys);
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
void fn_1_18BD8(NEL_SYS *sys);
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


static HU3DPARMANPARAM ParticleDot1 = {
    30, 0, 50.0f, 10.0f, 180.0f,
    { 0.0f, -0.05f, 0.0f },
    20.0f, 0.8f, 10.0f, 1.0f,
    2,
    { { 255, 255, 255, 255 }, { 255, 255, 64, 255 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 255, 128, 128, 0 }, { 255, 64, 32, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};
static HU3DPARMANPARAM ParticleDot2 = {
    40, 0, 50.0f, 0.0f, 90.0f,
    { 0.0f, -0.05f, 0.0f },
    35.0f, 0.88f, 25.0f, 0.99f,
    1,
    { { 255, 255, 255, 255 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 255, 128, 128, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};
static HU3DPARMANPARAM ParticleDot3 = {
    30, 0, 30.0f, 60.0f, 180.0f,
    { 0.0f, 0.0f, 0.0f },
    0.2f, 1.0f, 100.0f, 1.0f,
    2,
    { { 255, 255, 255, 32 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 64, 32, 32, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};
static HU3DPARMANPARAM ParticleGlow = {
    50, 0, 2.0f, 30.0f, 180.0f,
    { 0.0f, -0.2f, 0.0f },
    1.0f, 0.8f, 30.0f, 0.95f,
    3,
    { { 128, 192, 255, 255 }, { 255, 255, 64, 255 }, { 255, 96, 128, 255 }, { 0, 0, 0, 0 } },
    { { 128, 192, 255, 0 }, { 255, 64, 32, 0 }, { 128, 16, 16, 0 }, { 0, 0, 0, 0 } },
};

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

float MdCurveTbl[5] = { 0.0f, 16.0f, 64.0f, 32.0f, -65.0f };

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

/* ring/player slot currently being edited */
#define RP() (sys->ringMdlNo[sys->selNo])

void fn_1_2C88(NEL_SYS *sys, MDFLOWENT *tbl)
{
    s32 i;
    s32 max;
    SYSSPRITE *sprite;
    SYSMODEL *model;
    SYSFUKI *fuki;

    sprite = (SYSSPRITE *)sys->sprite;
    model = (SYSMODEL *)sys->model;
    fuki = (SYSFUKI *)sys->fuki;
    switch (sys->stat) {
        case 0:
            Hu3DModelAttrReset(model->mdlId[1], 1);
            for (i = 0; i < 10; i++) {
                Hu3DModelAttrReset(model->mdlId[i + 2], 1);
            }
            for (i = 0; i < 10; i++) {
                Hu3DModelAttrReset(model->mdlId[i + 16], 1);
            }
            Hu3DModelPosSet(model->mdlId[1], MdVecTbl[27].x, MdVecTbl[27].y, MdVecTbl[27].z);
            Hu3DModelScaleSet(model->mdlId[1], MdVecTbl[28].x, MdVecTbl[28].y, MdVecTbl[28].z);
            for (i = 0; i < 10; i++) {
                fn_1_196B4(sys, i + 2);
            }
            for (i = 0; i < 10; i++) {
                Hu3DModelPosSet(model->mdlId[i + 16], MdVecTbl[i + 39].x, MdVecTbl[i + 39].y, MdVecTbl[i + 39].z);
                Hu3DModelScaleSet(model->mdlId[i + 16], 0.0f, 0.0f, 0.0f);
            }
            MdSpriteDispSet(1, 0);
            MdSpriteDispSet(2, 0);
            MdSpriteDispSet(3, 0);
            MdSpriteDispSet(4, 0);
            HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
            HuSprGrpPosSet(sprite->grpId[2], 0.0f, 0.0f);
            HuSprGrpPosSet(sprite->grpId[3], 0.0f, 0.0f);
            HuSprGrpPosSet(sprite->grpId[4], 0.0f, 0.0f);
            Hu3DLayerHookSet(9, MdToueiLayerHook);
            MdMessDlgMesSet(0, -1);
            MdMessDlgMesSet(1, 0x6000F);
            sys->stat = 1;
            break;
        case 1:
            if (MdProcTbl[0x30].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                fn_1_28FAC(2, 0x20004);
                sys->stat = 2;
            }
            break;
        case 2:
            if (MdProcTbl[0x1B].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 3;
            }
            break;
        case 3:
            if (sys->fukiMode[0] == 0 && fuki[0].mode == 0 && (HuPadBtnDown[0] & PAD_BUTTON_B) != 0) {
                fn_1_28F74(2);
                fn_1_19904(5);
                sys->stat = 4;
            } else {
                for (i = 0; i < 4; i++) {
                    if (sys->fukiMode[i] != 1 && (fuki[i].mode == 0 || fuki[i].mode == 3)) {
                        if (fn_1_1835C(sys, i, 0) == 1) {
                            fn_1_19904(2);
                            fn_1_17BD8(sys);
                        } else if ((HuPadBtnDown[i] & PAD_BUTTON_B) != 0 && sys->fukiMode[i] == 2 && fuki[i].mode == 3) {
                            MdFukiGrpSet(sys, i, 3);
                            MdFukiMemberNoSet(sys, i, i);
                            MdFukiHookSet(sys, i, fn_1_19C10);
                            MdFukiModeSet(sys, i, 4);
                            Hu3DMotionShiftSet(model->mdlId[sys->mdlNo[i] + 2], model->motId[sys->mdlNo[i] * 2], 0.0f, 16.0f, 0x40000001);
                            fn_1_19904(5);
                            MdRingModeSet(sys, i, 4);
                        } else if ((HuPadBtnDown[i] & PAD_BUTTON_A) != 0 && sys->fukiMode[i] == 0 && fuki[i].mode == 0) {
                            MdFukiGrpSet(sys, i, 3);
                            MdFukiMemberNoSet(sys, i, i);
                            MdFukiHookSet(sys, i, fn_1_19C00);
                            MdFukiModeSet(sys, i, 1);
                            Hu3DModelAttrSet(model->mdlId[i + 0x42], 1);
                            Hu3DMotionShiftSet(model->mdlId[sys->mdlNo[i] + 2], model->motId[sys->mdlNo[i] * 2 + 1], 0.0f, 8.0f, 0);
                            fn_1_19904(4);
                            MdRingModeSet(sys, i, 1);
                            MdRingOfsSet(sys, i, 0.0f, 0.0f, 0.0f);
                            MdRingMdlSet(sys, i, sys->mdlNo[i] + 0x10);
                        }
                    }
                }
                if (((sys->fukiMode[0] == 2 && fuki[0].mode == 3) || sys->fukiMode[0] == 1)
                    && ((sys->fukiMode[1] == 2 && fuki[1].mode == 3) || sys->fukiMode[1] == 1)
                    && ((sys->fukiMode[2] == 2 && fuki[2].mode == 3) || sys->fukiMode[2] == 1)
                    && ((sys->fukiMode[3] == 2 && fuki[3].mode == 3) || sys->fukiMode[3] == 1)) {
                    if (sys->playerNum == 3) {
                        fn_1_28F74(2);
                        if (sys->modeSel == 0) {
                            sys->stat = 9;
                        } else if (sys->modeSel == 1) {
                            sys->stat = 0;
                            sys->flowNo = 8;
                        }
                    } else {
                        sys->selNo = 0;
                        fn_1_18834(sys);
                        fn_1_17CE4(sys);
                        Hu3DModelAttrReset(model->mdlId[RP() + 0x42], 1);
                        MdMessDlgMesSet(0, -1);
                        MdMessDlgMesSet(3, 0x60010);
                        MdMessDlgInsertMesReset();
                        MdMessDlgInsertMesAdd(0x6000D - sys->playerNum - sys->selNo, 0);
                        sys->stat = 6;
                    }
                }
            }
            break;
        case 4:
            if (MdProcTbl[0x1C].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 5;
            }
            break;
        case 5:
            if (MdProcTbl[0x31].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 0;
                sys->flowNo = 5;
            }
            break;
        case 14:
            if (fuki[0].mode == 0) {
                sys->stat = 2;
            }
            break;
        case 6:
            if (sys->selNo == 0 && (HuPadBtnDown[0] & PAD_BUTTON_B) != 0) {
                for (i = 0; i < 4; i++) {
                    if (sys->fukiMode[i] == 2 || sys->fukiMode[i] == 3) {
                        Hu3DMotionShiftSet(model->mdlId[sys->mdlNo[i] + 2], model->motId[sys->mdlNo[i] * 2], 0.0f, 16.0f, 0x40000001);
                        MdRingModeSet(sys, i, 4);
                        MdFukiHookSet(sys, i, fn_1_19C70);
                        MdFukiModeSet(sys, i, 4);
                    }
                }
                Hu3DModelAttrSet(model->mdlId[RP() + 0x42], 1);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(1, 0x6000F);
                fn_1_19904(5);
                sys->stat = 14;
            } else if (fn_1_1835C(sys, RP(), 1) == 1) {
                fn_1_19904(2);
                fn_1_17CE4(sys);
            } else if ((HuPadBtnDown[0] & PAD_BUTTON_B) != 0 && sys->selNo > 0) {
                Hu3DModelAttrSet(model->mdlId[RP() + 0x42], 1);
                sys->selNo--;
                Hu3DMotionShiftSet(model->mdlId[sys->mdlNo[RP()] + 2], model->motId[sys->mdlNo[RP()] * 2], 0.0f, 16.0f, 0x40000001);
                fn_1_19904(5);
                MdRingModeSet(sys, sys->selNo + 4, 4);
                if (sys->modeSel == 0) {
                    MdFukiGrpSet(sys, RP(), 4);
                    MdFukiMemberNoSet(sys, RP(), sys->selNo);
                } else if (sys->modeSel == 1) {
                    MdFukiGrpSet(sys, RP(), 3);
                    MdFukiMemberNoSet(sys, RP(), sys->selNo + 4);
                }
                MdFukiHookSet(sys, RP(), NULL);
                MdFukiModeSet(sys, RP(), 4);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(3, 0x60010);
                MdMessDlgInsertMesReset();
                MdMessDlgInsertMesAdd(0x6000D - sys->playerNum - sys->selNo, 0);
                sys->stat = 17;
            } else if ((HuPadBtnDown[0] & PAD_BUTTON_A) != 0) {
                if (sys->modeSel == 0) {
                    MdFukiGrpSet(sys, RP(), 3);
                    MdFukiMemberNoSet(sys, RP(), sys->selNo + 4);
                    MdFukiHookSet(sys, RP(), NULL);
                    MdFukiModeSet(sys, RP(), 1);
                    MdMessDlgMesSet(0, -1);
                    MdMessDlgMesSet(1, 0x60011);
                    Hu3DModelAttrSet(model->mdlId[RP() + 0x42], 1);
                    HuSprAttrReset(sprite->grpId[1], 0, 4);
                    HuSprAttrReset(sprite->grpId[1], 1, 4);
                    HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
                    HuSprPosSet(sprite->grpId[1], 0, -100.0f, 0.0f);
                    HuSprPosSet(sprite->grpId[1], 1, -100.0f, 0.0f);
                    Hu3DMotionShiftSet(model->mdlId[sys->mdlNo[RP()] + 2], model->motId[sys->mdlNo[RP()] * 2 + 1], 0.0f, 8.0f, 0);
                    MdRingModeSet(sys, sys->selNo + 4, 1);
                    MdRingOfsSet(sys, sys->selNo + 4, 0.0f, 0.0f, 0.0f);
                    MdRingMdlSet(sys, sys->selNo + 4, sys->mdlNo[RP()] + 0x10);
                    fn_1_19904(4);
                    sys->stat = 15;
                } else if (sys->modeSel == 1) {
                    MdFukiGrpSet(sys, RP(), 3);
                    MdFukiMemberNoSet(sys, RP(), sys->selNo + 4);
                    MdFukiHookSet(sys, RP(), NULL);
                    MdFukiModeSet(sys, RP(), 1);
                    Hu3DModelAttrSet(model->mdlId[RP() + 0x42], 1);
                    Hu3DMotionShiftSet(model->mdlId[sys->mdlNo[RP()] + 2], model->motId[sys->mdlNo[RP()] * 2 + 1], 0.0f, 8.0f, 0);
                    MdRingModeSet(sys, sys->selNo + 4, 1);
                    MdRingOfsSet(sys, sys->selNo + 4, 0.0f, 0.0f, 0.0f);
                    MdRingMdlSet(sys, sys->selNo + 4, sys->mdlNo[RP()] + 0x10);
                    if (sys->selNo >= 2 - sys->playerNum) {
                        fn_1_28F74(2);
                        fn_1_19904(3);
                    } else {
                        fn_1_19904(4);
                    }
                    sys->stat = 16;
                }
            }
            break;
        case 15:
            if (fuki[RP()].mode == 3) {
                HuVec2f *scale = MdSpriteScaleGet(4, sys->selNo);
                scale->x = 1.0f;
                scale->y = 1.0f;
                sys->stat = 10;
            }
            break;
        case 16:
            if (fuki[RP()].mode == 3) {
                sys->fukiMode[RP()] = 3;
                sys->selNo++;
                if (sys->selNo < 3 - sys->playerNum) {
                    fn_1_18834(sys);
                    fn_1_17CE4(sys);
                    Hu3DModelAttrReset(model->mdlId[RP() + 0x42], 1);
                }
                if (sys->selNo >= 3 - sys->playerNum) {
                    sys->stat = 0;
                    sys->flowNo = 8;
                } else {
                    MdMessDlgMesSet(0, -1);
                    MdMessDlgMesSet(3, 0x60010);
                    MdMessDlgInsertMesReset();
                    MdMessDlgInsertMesAdd(0x6000D - sys->playerNum - sys->selNo, 0);
                    sys->stat = 6;
                }
            }
            break;
        case 17:
            if (fuki[RP()].mode == 0) {
                Hu3DModelAttrReset(model->mdlId[RP() + 0x42], 1);
                sys->fukiMode[RP()] = 1;
                fn_1_17CE4(sys);
                sys->stat = 6;
            }
            break;
        case 10:
            if (MdProcTbl[0x15].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                HuSprAttrSet(sprite->grpId[3], (s16)(sys->selNo + 4), 4);
                HuSprAttrReset(sprite->grpId[4], sys->selNo, 4);
                fn_1_17FC4(sys, RP(), sys->selNo);
                sys->stat = 8;
            }
            break;
        case 11:
            if (MdProcTbl[0x17].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 8;
            }
            break;
        case 12:
            if (MdProcTbl[0x16].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                Hu3DModelAttrReset(model->mdlId[RP() + 0x42], 1);
                sys->stat = 6;
            }
            break;
        case 13:
            if (MdProcTbl[0x16].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->selNo++;
                if (sys->selNo < 3 - sys->playerNum) {
                    fn_1_18834(sys);
                    fn_1_17CE4(sys);
                    Hu3DModelAttrReset(model->mdlId[RP() + 0x42], 1);
                    sys->stat = 6;
                } else if (sys->selNo >= 3 - sys->playerNum) {
                    sys->stat = 9;
                }
            }
            break;
        case 7:
            if (((SYSMESS *)sys->mess)->mode == 5) {
                HuSprAttrSet(sprite->grpId[3], (s16)(sys->selNo + 4), 4);
                HuSprAttrReset(sprite->grpId[4], sys->selNo, 4);
                HuSprAttrReset(sprite->grpId[1], 0, 4);
                HuSprAttrReset(sprite->grpId[1], 1, 4);
                fn_1_17FC4(sys, RP(), sys->selNo);
                sys->stat = 8;
            }
            break;
        case 8:
            if (GwCommon.veryHardUnlock == 1) {
                max = 4;
            } else {
                max = 3;
            }
            if ((PADSTAT(sys) & 1) != 0 || (PADSTAT(sys) & 2) != 0) {
                if ((PADSTAT(sys) & 1) != 0) {
                    sys->sel = 0;
                    if (--sys->mdlSelNo[RP()] == -1) {
                        sys->mdlSelNo[RP()] = max - 1;
                    }
                } else if ((PADSTAT(sys) & 2) != 0) {
                    sys->sel = 1;
                    sys->mdlSelNo[RP()]++;
                    sys->mdlSelNo[RP()] = sys->mdlSelNo[RP()] % max;
                }
                fn_1_19904(2);
                fn_1_17FC4(sys, RP(), sys->selNo);
                sys->stat = 11;
            } else if ((HuPadBtnDown[0] & PAD_BUTTON_B) != 0) {
                MdFukiGrpSet(sys, RP(), 4);
                MdFukiMemberNoSet(sys, RP(), sys->selNo);
                MdFukiHookSet(sys, RP(), NULL);
                MdFukiModeSet(sys, RP(), 4);
                Hu3DMotionShiftSet(model->mdlId[sys->mdlNo[RP()] + 2], model->motId[sys->mdlNo[RP()] * 2], 0.0f, 16.0f, 0x40000001);
                MdRingModeSet(sys, sys->selNo + 4, 4);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(3, 0x60010);
                MdMessDlgInsertMesReset();
                MdMessDlgInsertMesAdd(0x6000D - sys->playerNum - sys->selNo, 0);
                fn_1_19904(5);
                sys->stat = 12;
            } else if ((HuPadBtnDown[0] & PAD_BUTTON_A) != 0) {
                sys->fukiMode[RP()] = 3;
                if (sys->selNo + 1 < 3 - sys->playerNum) {
                    MdMessDlgMesSet(0, -1);
                    MdMessDlgMesSet(3, 0x60010);
                    MdMessDlgInsertMesReset();
                    MdMessDlgInsertMesAdd(0x6000C - sys->playerNum - sys->selNo, 0);
                }
                if (sys->selNo >= 2 - sys->playerNum) {
                    fn_1_28F74(2);
                    fn_1_19904(3);
                } else {
                    fn_1_19904(4);
                }
                sys->stat = 13;
            }
            break;
        case 9:
            if (MdProcTbl[0x32].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 0;
                sys->flowNo = 9;
            }
            break;
    }
    fn_1_19818(sys);
}


void fn_1_46F4(NEL_SYS *sys, MDFLOWENT *tbl)
{
    s8 i;
    s16 grp;
    SYSSPRITE *sprite;
    SYSMODEL *model;
    SYSMESS *mess;

    sprite = (SYSSPRITE *)sys->sprite;
    model = (SYSMODEL *)sys->model;
    mess = (SYSMESS *)sys->mess;
    switch (sys->stat) {
        case 0:
            sys->selSubNo = 0;
            fn_1_18910(sys);
            fn_1_18BA0(sys);
            MdSpriteDispSet(1, 0);
            MdSpriteDispSet(4, 0);
            MdSpriteDispSet(5, 0);
            MdSpriteDispSet(6, 0);
            MdSpriteDispSet(7, 0);
            grp = sprite->grpId[1];
            HuSprAttrReset(grp, 0, 4);
            HuSprAttrReset(grp, 1, 4);
            HuSprGrpPosSet(sprite->grpId[3], 0.0f, 0.0f);
            HuSprGrpPosSet(sprite->grpId[4], 0.0f, 0.0f);
            HuSprGrpPosSet(sprite->grpId[1], 288.0f, 240.0f);
            HuSprGrpPosSet(sprite->grpId[5], 288.0f, 160.0f);
            grp = sprite->grpId[5];
            HuSprAttrReset(grp, 0, 4);
            HuSprAttrReset(grp, 1, 4);
            HuSprPosSet(grp, 0, -400.0f, 0.0f);
            HuSprPosSet(grp, 1, 400.0f, 0.0f);
            grp = sprite->grpId[1];
            HuSprAttrReset(grp, 0, 4);
            HuSprAttrReset(grp, 1, 4);
            HuSprAttrSet(grp, 2, 4);
            HuSprAttrSet(grp, 3, 4);
            HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
            HuSprPosSet(grp, 0, -100.0f, 0.0f);
            HuSprPosSet(grp, 1, -100.0f, 0.0f);
            MdMessWinKill(0);
            MdMessWinKill(1);
            sys->stat = 1;
            return;
        case 1:
            if (MdProcTbl[0x3A].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                for (i = 0; i < 4; i++) {
                    if (sys->fukiMode[i] == 0 || sys->fukiMode[i] == 2) {
                        if (sys->mdlSelNoB[i] < 2) {
                            MdRingModeSet(sys, i, 9);
                        } else {
                            MdRingModeSet(sys, i, 11);
                        }
                    } else if (sys->fukiMode[i] == 1 || sys->fukiMode[i] == 3) {
                        if (sys->mdlSelNoB[i] < 2) {
                            MdRingModeSet(sys, sys->mdlSelNo[i + 4] + 4, 9);
                        } else {
                            MdRingModeSet(sys, sys->mdlSelNo[i + 4] + 4, 11);
                        }
                    }
                }
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(1, 0x60012);
                sys->stat = 2;
                return;
            }
            break;
        case 2:
            if (mess->mode == 5) {
                sys->stat = 3;
                return;
            }
            break;
        case 3:
            if (MdProcTbl[9].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 4;
                return;
            }
            break;
        case 4:
            if ((PADSTAT(sys) & 1) != 0 || (PADSTAT(sys) & 2) != 0) {
                if ((PADSTAT(sys) & 1) != 0) {
                    sys->sel = 0;
                    if (--sys->selSubNo == -1) {
                        sys->selSubNo = 2;
                    }
                } else if ((PADSTAT(sys) & 2) != 0) {
                    sys->sel = 1;
                    sys->selSubNo = (sys->selSubNo + 1) % 3;
                }
                fn_1_18BA0(sys);
                fn_1_18910(sys);
                sys->stat = 7;
                return;
            }
            if ((HuPadBtnDown[0] & PAD_BUTTON_B) != 0) {
                sys->stat = 5;
                return;
            }
            if ((HuPadBtnDown[0] & PAD_BUTTON_A) != 0) {
                if (sys->playerNum == 3) {
                    MdMessWinMesSet(0, sys->winMes0 + 0x80000);
                    MdMessWinMesSet(1, sys->winMes1 + 0x80000);
                    sys->stat = 11;
                    return;
                }
                sys->selNo = 0;
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(1, 0x60011);
                sys->stat = 8;
                return;
            }
            break;
        case 5:
            if (MdProcTbl[10].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                for (i = 0; i < 4; i++) {
                    if (sys->fukiMode[i] == 0 || sys->fukiMode[i] == 2) {
                        MdRingModeSet(sys, i, 13);
                    } else if (sys->fukiMode[i] == 1 || sys->fukiMode[i] == 3) {
                        MdRingModeSet(sys, sys->mdlSelNo[i + 4] + 4, 13);
                    }
                }
                sys->stat = 6;
                return;
            }
            break;
        case 6:
            if (MdProcTbl[0x3B].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                if (sys->playerNum == 3) {
                    for (i = 0; i < 4; i++) {
                        if (sys->fukiMode[i] == 2 || sys->fukiMode[i] == 3) {
                            if (sys->fukiMode[i] == 2) {
                                sys->fukiMode[i] = 0;
                            } else if (sys->fukiMode[i] == 3) {
                                sys->fukiMode[i] = 1;
                            }
                            Hu3DMotionShiftSet(model->mdlId[sys->mdlNo[i] + 2], model->motId[sys->mdlNo[i] * 2], 0.0f, 16.0f, 0x40000001);
                            MdRingModeSet(sys, i, 4);
                        }
                    }
                    sys->stat = 2;
                    sys->flowNo = 6;
                    return;
                }
                for (i = 0; i < 4; i++) {
                    if (sys->fukiMode[i] == 3) {
                        sys->fukiMode[i] = 1;
                        Hu3DMotionShiftSet(model->mdlId[sys->mdlNo[i] + 2], model->motId[sys->mdlNo[i] * 2], 0.0f, 16.0f, 0x40000001);
                        MdRingModeSet(sys, sys->mdlSelNo[i + 4] + 4, 4);
                        HuSprAttrSet(sprite->grpId[3], (s16)(sys->mdlSelNo[i + 4] + 4), 4);
                    }
                }
                sys->selNo = 0;
                fn_1_18834(sys);
                fn_1_17CE4(sys);
                Hu3DModelAttrReset(model->mdlId[sys->ringMdlNo[sys->selNo] + 0x42], 1);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(3, 0x60010);
                MdMessDlgInsertMesReset();
                MdMessDlgInsertMesAdd(0x6000D - sys->playerNum - sys->selNo, 0);
                sys->stat = 6;
                sys->flowNo = 6;
                return;
            }
            break;
        case 7:
            if (MdProcTbl[0x3C].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                for (i = 0; i < 4; i++) {
                    if (sys->fukiMode[i] == 0 || sys->fukiMode[i] == 2) {
                        if (sys->mdlSelNoB[i] < 2) {
                            MdRingModeSet(sys, i, 9);
                        } else {
                            MdRingModeSet(sys, i, 11);
                        }
                    } else if (sys->fukiMode[i] == 1 || sys->fukiMode[i] == 3) {
                        if (sys->mdlSelNoB[i] < 2) {
                            MdRingModeSet(sys, sys->mdlSelNo[i + 4] + 4, 9);
                        } else {
                            MdRingModeSet(sys, sys->mdlSelNo[i + 4] + 4, 11);
                        }
                    }
                }
                sys->stat = 4;
                return;
            }
            break;
        case 8:
            if (mess->mode == 5) {
                sys->stat = 9;
                return;
            }
            break;
        case 9:
            if (MdProcTbl[10].func(sys, MdProcTbl) == 1) {
                HuSprAttrSet(sprite->grpId[3], (s16)(sys->mdlSelNo[sys->mdlSelNoC[sys->selNo] + 4] + 4), 4);
                HuSprAttrReset(sprite->grpId[4], sys->mdlSelNo[sys->mdlSelNoC[sys->selNo] + 4], 4);
                HuSprAttrReset(sprite->grpId[1], 0, 4);
                HuSprAttrReset(sprite->grpId[1], 1, 4);
                HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
                HuSprPosSet(sprite->grpId[1], 0, -100.0f, 0.0f);
                HuSprPosSet(sprite->grpId[1], 1, -100.0f, 0.0f);
                fn_1_18084(sys);
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 16;
                return;
            }
            break;
        case 16:
            if (MdProcTbl[0x18].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 10;
                return;
            }
            break;
        case 17:
            if (MdProcTbl[0x19].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 2;
                return;
            }
            break;
        case 18:
            if (MdProcTbl[0x1A].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 10;
                return;
            }
            break;
        case 19:
            if (MdProcTbl[0x19].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->selNo++;
                if (sys->selNo < 3 - sys->playerNum) {
                    HuSprAttrSet(sprite->grpId[3], (s16)(sys->mdlSelNo[sys->mdlSelNoC[sys->selNo] + 4] + 4), 4);
                    HuSprAttrReset(sprite->grpId[4], sys->mdlSelNo[sys->mdlSelNoC[sys->selNo] + 4], 4);
                    fn_1_18084(sys);
                    sys->stat = 16;
                    return;
                }
                if (sys->selNo >= 3 - sys->playerNum) {
                    HuSprAttrSet(sprite->grpId[1], 0, 4);
                    HuSprAttrSet(sprite->grpId[1], 1, 4);
                    MdMessWinMesSet(0, sys->winMes0 + 0x80000);
                    MdMessWinMesSet(1, sys->winMes1 + 0x80000);
                    sys->stat = 11;
                    return;
                }
            }
            break;
        case 20:
            if (MdProcTbl[0x19].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                HuSprAttrReset(sprite->grpId[3], (s8)(s16)(sys->mdlSelNo[sys->mdlSelNoC[sys->selNo] + 4] + 4), 4);
                HuSprAttrSet(sprite->grpId[4], (s16)sys->mdlSelNo[sys->mdlSelNoC[sys->selNo] + 4], 4);
                sys->selNo--;
                fn_1_18084(sys);
                sys->stat = 16;
                return;
            }
            break;
        case 10:
            if (sys->selNo == 0 && (HuPadBtnDown[0] & PAD_BUTTON_B) != 0) {
                HuSprAttrReset(sprite->grpId[3], (s8)(s16)(sys->mdlSelNo[sys->mdlSelNoC[sys->selNo] + 4] + 4), 4);
                HuSprAttrSet(sprite->grpId[4], (s16)sys->mdlSelNo[sys->mdlSelNoC[sys->selNo] + 4], 4);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(1, 0x60012);
                sys->stat = 17;
                return;
            }
            if ((PADSTAT(sys) & 1) != 0 || (PADSTAT(sys) & 2) != 0) {
                if ((PADSTAT(sys) & 1) != 0) {
                    sys->sel = 0;
                    if (--sys->mdlSelNo[sys->mdlSelNoC[sys->selNo]] == -1) {
                        sys->mdlSelNo[sys->mdlSelNoC[sys->selNo]] = 3;
                    }
                } else if ((PADSTAT(sys) & 2) != 0) {
                    sys->sel = 1;
                    sys->mdlSelNo[sys->mdlSelNoC[sys->selNo]]++;
                    sys->mdlSelNo[sys->mdlSelNoC[sys->selNo]] = sys->mdlSelNo[sys->mdlSelNoC[sys->selNo]] % 4;
                }
                fn_1_18084(sys);
                sys->stat = 18;
                return;
            }
            if (sys->selNo > 0 && (HuPadBtnDown[0] & PAD_BUTTON_B) != 0) {
                sys->stat = 20;
                return;
            }
            if ((HuPadBtnDown[0] & PAD_BUTTON_A) != 0) {
                sys->stat = 19;
                return;
            }
            break;
        case 11:
            if (MdProcTbl[0x1F].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(4, 0x60013);
                MdMessDlgInsertMesReset();
                MdMessDlgInsertMesAdd(sys->winMes0 + 0x80000, 0);
                MdMessDlgInsertMesAdd(sys->winMes1 + 0x80000, 1);
                sys->stat = 12;
                return;
            }
            break;
        case 12:
            if (mess->mode == 5) {
                if (mess->choice == -1 || mess->choice == 1) {
                    sys->stat = 13;
                    return;
                } else if (mess->choice == 0) {
                    sys->stat = 14;
                    return;
                }
            }
            break;
        case 13:
            if (MdProcTbl[0x20].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                MdMessWinKill(0);
                MdMessWinKill(1);
                if (sys->playerNum == 3) {
                    for (grp = 0; grp < 3 - sys->playerNum; grp++) {
                        HuSprAttrSet(sprite->grpId[4], grp, 4);
                        HuSprAttrReset(sprite->grpId[3], (s8)(s16)(grp + 4), 4);
                    }
                    HuSprAttrReset(sprite->grpId[1], 0, 4);
                    HuSprAttrReset(sprite->grpId[1], 1, 4);
                    HuSprGrpPosSet(sprite->grpId[1], 288.0f, 240.0f);
                    HuSprPosSet(sprite->grpId[1], 0, -300.0f, 0.0f);
                    HuSprPosSet(sprite->grpId[1], 1, 300.0f, 0.0f);
                    MdMessDlgMesSet(0, -1);
                    MdMessDlgMesSet(1, 0x60012);
                    sys->stat = 2;
                    return;
                }
                for (grp = 0; grp < 3 - sys->playerNum; grp++) {
                    HuSprAttrSet(sprite->grpId[4], grp, 4);
                    HuSprAttrReset(sprite->grpId[3], (s8)(s16)(grp + 4), 4);
                }
                sys->selNo = 0;
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(1, 0x60011);
                sys->stat = 8;
                return;
            }
            break;
        case 14:
            if (MdProcTbl[0x20].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                MdMessWinKill(0);
                MdMessWinKill(1);
                sys->stat = 15;
                return;
            }
            break;
        case 15:
            if (MdProcTbl[0x34].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 0;
                sys->flowNo = 9;
            }
            break;
    }
}


void fn_1_5B18(NEL_SYS *sys, MDFLOWENT *tbl)
{
    s8 i;
    s16 grp;
    s32 max;
    SYSSPRITE *sprite;
    SYSMODEL *model;
    SYSMESS *mess;

    sprite = (SYSSPRITE *)sys->sprite;
    model = (SYSMODEL *)sys->model;
    mess = (SYSMESS *)sys->mess;
    switch (sys->stat) {
        case 0:
            sys->selSubNo = 0;
            fn_1_18910(sys);
            fn_1_18BA0(sys);
            MdSpriteDispSet(1, 0);
            MdSpriteDispSet(4, 0);
            MdSpriteDispSet(5, 0);
            MdSpriteDispSet(6, 0);
            MdSpriteDispSet(7, 0);
            grp = sprite->grpId[1];
            HuSprAttrReset(grp, 0, 4);
            HuSprAttrReset(grp, 1, 4);
            HuSprGrpPosSet(sprite->grpId[3], 0.0f, 0.0f);
            HuSprGrpPosSet(sprite->grpId[4], 0.0f, 0.0f);
            HuSprGrpPosSet(sprite->grpId[1], 288.0f, 240.0f);
            HuSprGrpPosSet(sprite->grpId[5], 288.0f, 160.0f);
            grp = sprite->grpId[5];
            HuSprAttrReset(grp, 0, 4);
            HuSprAttrReset(grp, 1, 4);
            HuSprPosSet(grp, 0, -500.0f, 0.0f);
            HuSprPosSet(grp, 1, 500.0f, 0.0f);
            grp = sprite->grpId[1];
            HuSprAttrReset(grp, 0, 4);
            HuSprAttrReset(grp, 1, 4);
            HuSprAttrSet(grp, 2, 4);
            HuSprAttrSet(grp, 3, 4);
            HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
            HuSprPosSet(grp, 0, -100.0f, 0.0f);
            HuSprPosSet(grp, 1, -100.0f, 0.0f);
            MdMessWinKill(0);
            MdMessWinKill(1);
            sys->stat = 1;
            return;
        case 1:
            if (MdProcTbl[0x3A].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                for (i = 0; i < 4; i++) {
                    if (sys->fukiMode[i] == 0 || sys->fukiMode[i] == 2) {
                        if (sys->mdlSelNoB[i] < 2) {
                            MdRingModeSet(sys, i, 9);
                        } else {
                            MdRingModeSet(sys, i, 11);
                        }
                    } else if (sys->fukiMode[i] == 1 || sys->fukiMode[i] == 3) {
                        if (sys->mdlSelNoB[i] < 2) {
                            MdRingModeSet(sys, sys->mdlSelNo[i + 4] + 4, 9);
                        } else {
                            MdRingModeSet(sys, sys->mdlSelNo[i + 4] + 4, 11);
                        }
                    }
                }
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(1, 0x60012);
                fn_1_28FAC(2, 0x20004);
                sys->sel = 0;
                sys->stat = 2;
                return;
            }
            break;
        case 2:
            if (MdProcTbl[9].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                MdMessWinMesSet(0, 0x8002D);
                MdMessWinMesSet(1, 0x8002D);
                sys->stat = 3;
                return;
            }
            break;
        case 3:
            if (MdProcTbl[0x1F].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 4;
                return;
            }
            break;
        case 4:
            if ((PADSTAT(sys) & 1) != 0 || (PADSTAT(sys) & 2) != 0) {
                if ((PADSTAT(sys) & 1) != 0) {
                    sys->sel = 0;
                    if (--sys->selSubNo == -1) {
                        sys->selSubNo = 2;
                    }
                } else if ((PADSTAT(sys) & 2) != 0) {
                    sys->sel = 1;
                    sys->selSubNo = (sys->selSubNo + 1) % 3;
                }
                fn_1_19904(2);
                fn_1_18BA0(sys);
                fn_1_18910(sys);
                sys->stat = 7;
                return;
            }
            if ((HuPadBtnDown[0] & PAD_BUTTON_B) != 0) {
                fn_1_19904(5);
                fn_1_28F74(2);
                sys->sel = 0;
                sys->stat = 5;
                return;
            }
            if ((HuPadBtnDown[0] & PAD_BUTTON_A) != 0) {
                if (sys->playerNum == 3) {
                    MdMessWinKill(0);
                    MdMessWinKill(1);
                    MdMessWinMesSet(0, sys->winMes0 + 0x80000);
                    MdMessWinMesSet(1, sys->winMes1 + 0x80000);
                    fn_1_1923C(sys);
                    fn_1_19904(3);
                    sys->stat = 17;
                    return;
                }
                sys->selNo = 0;
                MdMessWinKill(0);
                MdMessWinKill(1);
                MdMessWinMesSet(0, sys->winMes0 + 0x80000);
                MdMessWinMesSet(1, sys->winMes1 + 0x80000);
                fn_1_1923C(sys);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(1, 0x60011);
                fn_1_19904(3);
                sys->stat = 9;
                return;
            }
            break;
        case 5:
            if (MdProcTbl[0x20].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                MdMessWinKill(0);
                MdMessWinKill(1);
                sys->stat = 6;
                return;
            }
            break;
        case 6:
            if (MdProcTbl[10].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                for (i = 0; i < 4; i++) {
                    if (sys->fukiMode[i] == 0 || sys->fukiMode[i] == 2) {
                        MdRingModeSet(sys, i, 13);
                    } else if (sys->fukiMode[i] == 1 || sys->fukiMode[i] == 3) {
                        MdRingModeSet(sys, sys->mdlSelNo[i + 4] + 4, 13);
                    }
                }
                sys->stat = 8;
                return;
            }
            break;
        case 7:
            if (MdProcTbl[0x3C].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                for (i = 0; i < 4; i++) {
                    if (sys->fukiMode[i] == 0 || sys->fukiMode[i] == 2) {
                        if (sys->mdlSelNoB[i] < 2) {
                            MdRingModeSet(sys, i, 9);
                        } else {
                            MdRingModeSet(sys, i, 11);
                        }
                    } else if (sys->fukiMode[i] == 1 || sys->fukiMode[i] == 3) {
                        if (sys->mdlSelNoB[i] < 2) {
                            MdRingModeSet(sys, sys->mdlSelNo[i + 4] + 4, 9);
                        } else {
                            MdRingModeSet(sys, sys->mdlSelNo[i + 4] + 4, 11);
                        }
                    }
                }
                sys->stat = 4;
                return;
            }
            break;
        case 8:
            if (MdProcTbl[0x3B].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                if (sys->playerNum == 3) {
                    for (i = 0; i < 4; i++) {
                        if (sys->fukiMode[i] == 2 || sys->fukiMode[i] == 3) {
                            if (sys->fukiMode[i] == 2) {
                                sys->fukiMode[i] = 0;
                            } else if (sys->fukiMode[i] == 3) {
                                sys->fukiMode[i] = 1;
                            }
                            Hu3DMotionShiftSet(model->mdlId[sys->mdlNo[i] + 2], model->motId[sys->mdlNo[i] * 2], 0.0f, 16.0f, 0x40000001);
                            MdRingModeSet(sys, i, 4);
                        }
                    }
                    MdMessDlgMesSet(0, -1);
                    MdMessDlgMesSet(1, 0x6000F);
                    fn_1_28FAC(2, 0x20004);
                    sys->stat = 2;
                    sys->flowNo = 6;
                    return;
                }
                for (i = 0; i < 4; i++) {
                    if (sys->fukiMode[i] == 3) {
                        sys->fukiMode[i] = 1;
                        Hu3DMotionShiftSet(model->mdlId[sys->mdlNo[i] + 2], model->motId[sys->mdlNo[i] * 2], 0.0f, 16.0f, 0x40000001);
                        MdRingModeSet(sys, sys->mdlSelNo[i + 4] + 4, 4);
                        HuSprAttrSet(sprite->grpId[3], (s16)(sys->mdlSelNo[i + 4] + 4), 4);
                    }
                }
                sys->selNo = 0;
                fn_1_18834(sys);
                fn_1_17CE4(sys);
                Hu3DModelAttrReset(model->mdlId[sys->ringMdlNo[sys->selNo] + 0x42], 1);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(3, 0x60010);
                MdMessDlgInsertMesReset();
                MdMessDlgInsertMesAdd(0x6000D - sys->playerNum - sys->selNo, 0);
                sys->stat = 6;
                sys->flowNo = 6;
                return;
            }
            break;
        case 9:
            if (MdProcTbl[10].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                HuSprAttrSet(sprite->grpId[3], (s16)(sys->mdlSelNo[sys->mdlSelNoC[sys->selNo] + 4] + 4), 4);
                HuSprAttrReset(sprite->grpId[4], sys->mdlSelNo[sys->mdlSelNoC[sys->selNo] + 4], 4);
                HuSprAttrReset(sprite->grpId[1], 0, 4);
                HuSprAttrReset(sprite->grpId[1], 1, 4);
                HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
                HuSprPosSet(sprite->grpId[1], 0, -100.0f, 0.0f);
                HuSprPosSet(sprite->grpId[1], 1, -100.0f, 0.0f);
                fn_1_18084(sys);
                sys->stat = 10;
                return;
            }
            break;
        case 10:
            if (MdProcTbl[0x18].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 11;
                return;
            }
            break;
        case 11:
            if (sys->selNo == 0 && (HuPadBtnDown[0] & PAD_BUTTON_B) != 0) {
                HuSprAttrReset(sprite->grpId[3], (s8)(s16)(sys->mdlSelNo[sys->mdlSelNoC[sys->selNo] + 4] + 4), 4);
                HuSprAttrSet(sprite->grpId[4], (s16)sys->mdlSelNo[sys->mdlSelNoC[sys->selNo] + 4], 4);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(1, 0x60012);
                fn_1_19904(5);
                sys->stat = 12;
                return;
            }
            if ((PADSTAT(sys) & 1) != 0 || (PADSTAT(sys) & 2) != 0) {
                if (GwCommon.veryHardUnlock == 1) {
                    max = 4;
                } else {
                    max = 3;
                }
                if ((PADSTAT(sys) & 1) != 0) {
                    sys->sel = 0;
                    if (--sys->mdlSelNo[sys->mdlSelNoC[sys->selNo]] == -1) {
                        sys->mdlSelNo[sys->mdlSelNoC[sys->selNo]] = max - 1;
                    }
                } else if ((PADSTAT(sys) & 2) != 0) {
                    sys->sel = 1;
                    sys->mdlSelNo[sys->mdlSelNoC[sys->selNo]]++;
                    sys->mdlSelNo[sys->mdlSelNoC[sys->selNo]] = sys->mdlSelNo[sys->mdlSelNoC[sys->selNo]] % max;
                }
                fn_1_18084(sys);
                fn_1_19904(2);
                sys->stat = 14;
                return;
            }
            if (sys->selNo > 0 && (HuPadBtnDown[0] & PAD_BUTTON_B) != 0) {
                fn_1_19904(5);
                sys->stat = 15;
                return;
            }
            if ((HuPadBtnDown[0] & PAD_BUTTON_A) != 0) {
                if (sys->selNo >= 2 - sys->playerNum) {
                    fn_1_19904(3);
                } else {
                    fn_1_19904(4);
                }
                sys->stat = 16;
                return;
            }
            break;
        case 12:
            if (MdProcTbl[0x19].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 13;
                return;
            }
            break;
        case 13:
            if (MdProcTbl[9].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                MdMessWinKill(0);
                MdMessWinKill(1);
                MdMessWinMesSet(0, 0x8002D);
                MdMessWinMesSet(1, 0x8002D);
                fn_1_1923C(sys);
                sys->stat = 4;
                return;
            }
            break;
        case 14:
            if (MdProcTbl[0x1A].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 11;
                return;
            }
            break;
        case 15:
            if (MdProcTbl[0x19].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                HuSprAttrReset(sprite->grpId[3], (s8)(s16)(sys->mdlSelNo[sys->mdlSelNoC[sys->selNo] + 4] + 4), 4);
                HuSprAttrSet(sprite->grpId[4], (s16)sys->mdlSelNo[sys->mdlSelNoC[sys->selNo] + 4], 4);
                sys->selNo--;
                fn_1_18084(sys);
                sys->stat = 10;
                return;
            }
            break;
        case 16:
            if (MdProcTbl[0x19].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->selNo++;
                if (sys->selNo < 3 - sys->playerNum) {
                    HuSprAttrSet(sprite->grpId[3], (s16)(sys->mdlSelNo[sys->mdlSelNoC[sys->selNo] + 4] + 4), 4);
                    HuSprAttrReset(sprite->grpId[4], sys->mdlSelNo[sys->mdlSelNoC[sys->selNo] + 4], 4);
                    fn_1_18084(sys);
                    sys->stat = 10;
                    return;
                }
                if (sys->selNo >= 3 - sys->playerNum) {
                    HuSprAttrSet(sprite->grpId[1], 0, 4);
                    HuSprAttrSet(sprite->grpId[1], 1, 4);
                    sys->stat = 17;
                    return;
                }
            }
            break;
        case 17:
            MdMessDlgMesSet(0, -1);
            MdMessDlgMesSet(4, 0x60013);
            MdMessDlgInsertMesReset();
            MdMessDlgInsertMesAdd(sys->winMes0 + 0x80000, 0);
            MdMessDlgInsertMesAdd(sys->winMes1 + 0x80000, 1);
            sys->stat = 18;
            return;
        case 18:
            if (mess->mode == 5) {
                if (mess->choice == -1 || mess->choice == 1) {
                    if (sys->playerNum == 3) {
                        for (grp = 0; grp < 3 - sys->playerNum; grp++) {
                            HuSprAttrSet(sprite->grpId[4], grp, 4);
                            HuSprAttrReset(sprite->grpId[3], (s8)(s16)(grp + 4), 4);
                        }
                        HuSprAttrReset(sprite->grpId[1], 0, 4);
                        HuSprAttrReset(sprite->grpId[1], 1, 4);
                        HuSprGrpPosSet(sprite->grpId[1], 288.0f, 240.0f);
                        HuSprPosSet(sprite->grpId[1], 0, -300.0f, 0.0f);
                        HuSprPosSet(sprite->grpId[1], 1, 300.0f, 0.0f);
                        MdMessDlgMesSet(0, -1);
                        MdMessDlgMesSet(1, 0x60012);
                        MdMessWinKill(0);
                        MdMessWinKill(1);
                        MdMessWinMesSet(0, 0x8002D);
                        MdMessWinMesSet(1, 0x8002D);
                        fn_1_1923C(sys);
                        sys->stat = 2;
                        return;
                    }
                    for (grp = 0; grp < 3 - sys->playerNum; grp++) {
                        HuSprAttrSet(sprite->grpId[4], grp, 4);
                        HuSprAttrReset(sprite->grpId[3], (s8)(s16)(grp + 4), 4);
                    }
                    sys->selNo = 0;
                    HuSprAttrSet(sprite->grpId[3], (s16)(sys->mdlSelNo[sys->mdlSelNoC[sys->selNo] + 4] + 4), 4);
                    HuSprAttrReset(sprite->grpId[4], sys->mdlSelNo[sys->mdlSelNoC[sys->selNo] + 4], 4);
                    MdMessDlgMesSet(0, -1);
                    MdMessDlgMesSet(1, 0x60011);
                    sys->stat = 9;
                    return;
                } else if (mess->choice == 0) {
                    sys->sel = 1;
                    sys->stat = 19;
                    return;
                }
            }
            break;
        case 19:
            if (MdProcTbl[0x20].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                MdMessWinKill(0);
                MdMessWinKill(1);
                sys->stat = 20;
                return;
            }
            break;
        case 20:
            if (MdProcTbl[0x34].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 0;
                sys->flowNo = 9;
            }
            break;
    }
}

void fn_1_716C(NEL_SYS *sys, MDFLOWENT *tbl)
{
    s16 grp;
    s32 ret;
    s16 j;
    s8 i;
    HuVecF *pos;
    HuVecF *scl;
    SYSSPRITE *sprite;
    SYSMODEL *model;
    SYSMESS *volatile mess;

    sprite = (SYSSPRITE *)sys->sprite;
    model = (SYSMODEL *)sys->model;
    mess = (SYSMESS *)sys->mess;
    switch (sys->stat) {
        case 0:
            sys->selNo3 = 0;
            sys->unk27C = MdVecTbl[0x50].y;
            sys->unk274 = 90.0f + 51.42857f * sys->selNo3;
            for (i = 0; i < 7; i++) {
                Hu3DModelAttrReset(model->mdlId[i + 0x20], 1);
            }
            for (i = 0; i < 7; i++) {
                scl = model->mdlScale[i + 0x20];
                pos = model->mdlPos[i + 0x20];
                pos->x = MdVecTbl[0x4E].x + sys->unk27C * cos(3.141592653589793 * (sys->unk274 - 51.42857f * i) / 180.0);
                pos->z = MdVecTbl[0x4E].z + sys->unk27C * sin(3.141592653589793 * (sys->unk274 - 51.42857f * i) / 180.0);
                pos->y = MdVecTbl[0x4E].y;
                scl->x = 0.0f;
                scl->y = 0.0f;
                scl->z = 0.0f;
            }
            grp = sprite->grpId[1];
            HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
            HuSprAttrReset(grp, 0, 4);
            HuSprAttrReset(grp, 1, 4);
            HuSprAttrSet(grp, 2, 4);
            HuSprAttrSet(grp, 3, 4);
            HuSprPosSet(grp, 0, -100.0f, 0.0f);
            HuSprPosSet(grp, 1, -100.0f, 0.0f);
            sys->stat = 1;
            return;
        case 1:
            if ((ret = MdProcTbl[0x3D].func(sys, MdProcTbl)) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                fn_1_18BD8(sys);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(1, sys->selNo3 * 2 + 0x70001);
                sys->stat = 2;
                return;
            }
            break;
        case 2:
            NEL_INT10(sys) = 0;
            sys->timer = 0;
            sys->stat = 3;
            return;
        case 3:
            if ((ret = MdProcTbl[0xC].func(sys, MdProcTbl)) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 4;
                return;
            }
            break;
        case 4:
            if ((PADSTAT(sys) & 1) != 0 || (PADSTAT(sys) & 2) != 0) {
                sys->selPrevNo = sys->selNo3;
                if ((PADSTAT(sys) & 1) != 0) {
                    sys->sel = 0;
                    sys->selNo3++;
                    sys->selNo3 = sys->selNo3 % 7;
                    sys->stat = 9;
                } else if ((PADSTAT(sys) & 2) != 0) {
                    sys->sel = 1;
                    if (--sys->selNo3 == -1) {
                        sys->selNo3 = 6;
                    }
                    sys->stat = 10;
                }
                fn_1_19904(2);
                return;
            }
            if ((HuPadBtnDown[0] & 0x100) != 0) {
                fn_1_19904(3);
                sys->stat = 11;
                return;
            }
            if ((HuPadBtnDown[0] & 0x200) != 0) {
                sys->stat = 5;
                fn_1_19904(5);
                return;
            }
            break;
        case 5:
            if ((ret = MdProcTbl[0xD].func(sys, MdProcTbl)) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 6;
                return;
            }
            break;
        case 6:
            if ((ret = MdProcTbl[0x3E].func(sys, MdProcTbl)) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                HuSprAttrSet(sprite->grpId[6], sys->selNo3, 4);
                switch (sys->modeSel) {
                    case 0:
                        sys->stat = 7;
                        return;
                    case 1:
                        sys->stat = 8;
                        return;
                }
            }
            break;
        case 7:
            if ((ret = MdProcTbl[0x33].func(sys, MdProcTbl)) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                if (sys->playerNum == 3) {
                    for (i = 0; i < 4; i++) {
                        if (sys->fukiMode[i] == 2 || sys->fukiMode[i] == 3) {
                            if (sys->fukiMode[i] == 2) {
                                sys->fukiMode[i] = 0;
                            } else if (sys->fukiMode[i] == 3) {
                                sys->fukiMode[i] = 1;
                            }
                            Hu3DMotionShiftSet(model->mdlId[sys->mdlNo[i] + 2], model->motId[sys->mdlNo[i] * 2], 0.0f, 16.0f, 0x40000001);
                            MdRingModeSet(sys, i, 4);
                        }
                    }
                    for (j = 0; j < 3 - sys->playerNum; j++) {
                        HuSprAttrSet(sprite->grpId[3], j + 4, 4);
                        HuSprAttrSet(sprite->grpId[4], j, 4);
                    }
                    for (j = 0; j < 4; j++) {
                        HuSprAttrSet(sprite->grpId[3], j, 4);
                    }
                    MdMessDlgMesSet(0, -1);
                    MdMessDlgMesSet(1, 0x6000F);
                    sys->stat = 2;
                    sys->flowNo = 6;
                    return;
                }
                for (i = 0; i < 4; i++) {
                    if (sys->fukiMode[i] == 3) {
                        sys->fukiMode[i] = 1;
                        Hu3DMotionShiftSet(model->mdlId[sys->mdlNo[i] + 2], model->motId[sys->mdlNo[i] * 2], 0.0f, 16.0f, 0x40000001);
                        MdRingModeSet(sys, sys->mdlSelNo[i + 4] + 4, 4);
                    }
                }
                for (j = 0; j < 3 - sys->playerNum; j++) {
                    HuSprAttrSet(sprite->grpId[4], j, 4);
                }
                sys->selNo = 0;
                fn_1_18834(sys);
                fn_1_17CE4(sys);
                Hu3DModelAttrReset(model->mdlId[sys->ringMdlNo[sys->selNo] + 0x42], 1);
                fn_1_19818(sys);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(3, 0x60010);
                MdMessDlgInsertMesReset();
                MdMessDlgInsertMesAdd(0x6000D - sys->playerNum - sys->selNo, 0);
                sys->stat = 6;
                sys->flowNo = 6;
                return;
            }
            break;
        case 8:
            if ((ret = MdProcTbl[0x35].func(sys, MdProcTbl)) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(1, 0x60012);
                for (j = 0; j < 3 - sys->playerNum; j++) {
                    HuSprAttrReset(sprite->grpId[3], j + 4, 4);
                    HuSprAttrSet(sprite->grpId[4], j, 4);
                }
                sys->sel = 1;
                sys->stat = 2;
                sys->flowNo = 8;
                return;
            }
            break;
        case 9:
        case 10:
            if ((ret = MdProcTbl[sys->stat + 0x36].func(sys, &MdProcTbl[sys->stat + 0x36])) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                fn_1_18BD8(sys);
                NEL_INT10(sys) = 1;
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(1, sys->selNo3 * 2 + 0x70001);
                sys->stat = 4;
                return;
            }
            break;
        case 11:
            if ((ret = MdProcTbl[0xD].func(sys, MdProcTbl)) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 12;
                return;
            }
            break;
        case 12:
            if ((ret = MdProcTbl[0x41].func(sys, MdProcTbl)) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 0;
                sys->flowNo = 10;
            }
            break;
    }
}

void fn_1_7C88(NEL_SYS *sys, MDFLOWENT *tbl)
{
    s8 i;
    SYSSPRITE *sprite;
    SYSMODEL *model;
    SYSMESS *mess;

    sprite = (SYSSPRITE *)sys->sprite;
    model = (SYSMODEL *)sys->model;
    mess = (SYSMESS *)sys->mess;
    switch (sys->stat) {
        case 0:
            Hu3DModelAttrReset(model->mdlId[0x3B], 1);
            for (i = 0; i < 5; i++) {
                if (i == sys->menuNo2) {
                    Hu3DModelAttrReset(model->mdlId[i + 0x3C], 1);
                } else {
                    Hu3DModelAttrSet(model->mdlId[i + 0x3C], 1);
                }
            }
            Hu3DModelAttrReset(model->mdlId[0x41], 1);
            Hu3DAnmNoSet(model->animId[0], sys->menuNo);
            Hu3DAnmNoSet(model->animId[2], sys->menuNo3);
            model->mdlPos[0x3B]->x = MdVecTbl[0x51].x;
            model->mdlPos[0x3B]->y = MdVecTbl[0x51].y;
            model->mdlPos[0x3B]->z = MdVecTbl[0x51].z;
            for (i = 0; i < 5; i++) {
                model->mdlPos[i + 0x3C]->x = MdVecTbl[0x54].x;
                model->mdlPos[i + 0x3C]->y = MdVecTbl[0x54].y;
                model->mdlPos[i + 0x3C]->z = MdVecTbl[0x54].z;
            }
            model->mdlPos[0x41]->x = MdVecTbl[0x57].x;
            model->mdlPos[0x41]->y = MdVecTbl[0x57].y;
            model->mdlPos[0x41]->z = MdVecTbl[0x57].z;
            model->mdlScale[0x3B]->x = 0.0f;
            model->mdlScale[0x3B]->y = 0.0f;
            model->mdlScale[0x3B]->z = 0.0f;
            for (i = 0; i < 5; i++) {
                model->mdlScale[i + 0x3C]->x = 0.0f;
                model->mdlScale[i + 0x3C]->y = 0.0f;
                model->mdlScale[i + 0x3C]->z = 0.0f;
            }
            model->mdlScale[0x41]->x = 0.0f;
            model->mdlScale[0x41]->y = 0.0f;
            model->mdlScale[0x41]->z = 0.0f;
            HuSprGrpPosSet(sprite->grpId[1], 288.0f, 240.0f);
            HuSprAttrReset(sprite->grpId[1], 0, 4);
            HuSprAttrReset(sprite->grpId[1], 1, 4);
            HuSprPosSet(sprite->grpId[1], 0, -300.0f, 0.0f);
            HuSprPosSet(sprite->grpId[1], 1, 300.0f, 0.0f);
            if (sys->menuNo == 0) {
                HuSprAttrSet(sprite->grpId[1], 0, 4);
            } else if (sys->menuNo == 8) {
                HuSprAttrSet(sprite->grpId[1], 1, 4);
            } else {
                HuSprAttrReset(sprite->grpId[1], 0, 4);
                HuSprAttrReset(sprite->grpId[1], 1, 4);
            }
            sys->stat = 1;
            return;
        case 1:
            if (MdProcTbl[0x43].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(3, 0x60016);
                MdMessDlgInsertMesReset();
                MdMessDlgInsertMesAdd(sys->menuNo + 0x60017, 0);
                sys->stat = 2;
                return;
            }
            break;
        case 2:
            if (mess->mode == 5) {
                NEL_INT10(sys) = 0;
                sys->timer = 0;
                sys->stat = 3;
                return;
            }
            break;
        case 3:
            if (MdProcTbl[0xF].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 4;
                return;
            }
            break;
        case 4:
            if (((PADSTAT(sys) & 1) != 0 && sys->menuNo != 0) || ((PADSTAT(sys) & 2) != 0 && sys->menuNo != 8)
                || ((HuPadBtnDown[0] & 0x40) != 0 && sys->menuNo != 0) || ((HuPadBtnDown[0] & 0x20) != 0 && sys->menuNo != 8)) {
                if ((HuPadBtnDown[0] & 0x40) != 0) {
                    sys->sel = 0;
                    sys->menuNo = 0;
                } else if ((HuPadBtnDown[0] & 0x20) != 0) {
                    sys->sel = 1;
                    sys->menuNo = 8;
                } else if ((PADSTAT(sys) & 1) != 0) {
                    sys->sel = 0;
                    if (--sys->menuNo == -1) {
                        sys->menuNo = 0;
                    }
                } else if ((PADSTAT(sys) & 2) != 0) {
                    sys->sel = 1;
                    if (++sys->menuNo == 9) {
                        sys->menuNo = 8;
                    }
                }
                Hu3DAnmNoSet(model->animId[0], sys->menuNo);
                if (sys->menuNo == 0) {
                    HuSprAttrSet(sprite->grpId[1], 0, 4);
                    HuSprAttrReset(sprite->grpId[1], 1, 4);
                } else if (sys->menuNo == 8) {
                    HuSprAttrSet(sprite->grpId[1], 1, 4);
                    HuSprAttrReset(sprite->grpId[1], 0, 4);
                } else {
                    HuSprAttrReset(sprite->grpId[1], 0, 4);
                    HuSprAttrReset(sprite->grpId[1], 1, 4);
                }
                NEL_INT10(sys) = 1;
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(3, 0x60016);
                MdMessDlgInsertMesReset();
                MdMessDlgInsertMesAdd(sys->menuNo + 0x60017, 0);
                fn_1_19904(2);
                sys->selSub = 1;
                sys->stat = 0x1C;
                return;
            }
            if ((HuPadBtnDown[0] & 0x100) != 0) {
                fn_1_19904(3);
                sys->stat = 8;
                return;
            }
            if ((HuPadBtnDown[0] & 0x200) != 0) {
                sys->stat = 5;
                fn_1_19904(5);
                return;
            }
            break;
        case 0x1C:
            if (MdProcTbl[0x11].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 4;
                return;
            }
            break;
        case 5:
            if (MdProcTbl[0x10].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 6;
                return;
            }
            break;
        case 6:
            if (MdProcTbl[0x44].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 7;
                return;
            }
            break;
        case 7:
            if (MdProcTbl[0x42].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                HuSprAttrReset(sprite->grpId[1], 0, 4);
                HuSprAttrReset(sprite->grpId[1], 1, 4);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(1, sys->selNo3 * 2 + 0x70001);
                sys->stat = 2;
                sys->flowNo = 9;
                return;
            }
            break;
        case 8:
            if (MdProcTbl[0x10].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 9;
                return;
            }
            break;
        case 9:
            if (MdProcTbl[0x45].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(1, sys->menuNo2 + 0x60020);
                sys->stat = 10;
                return;
            }
            break;
        case 10:
            if (MdProcTbl[0x47].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 11;
                return;
            }
            break;
        case 11:
            if (mess->mode == 5) {
                HuSprAttrReset(sprite->grpId[1], 0, 4);
                HuSprAttrReset(sprite->grpId[1], 1, 4);
                sys->stat = 12;
                return;
            }
            break;
        case 12:
            if (MdProcTbl[0xF].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 13;
                return;
            }
            break;
        case 13:
            if ((PADSTAT(sys) & 1) != 0 || (PADSTAT(sys) & 2) != 0) {
                if ((PADSTAT(sys) & 1) != 0) {
                    sys->sel = 0;
                    if (--sys->menuNo2 == -1) {
                        sys->menuNo2 = 4;
                    }
                } else if ((PADSTAT(sys) & 2) != 0) {
                    sys->sel = 1;
                    sys->menuNo2++;
                    sys->menuNo2 = sys->menuNo2 % 5;
                }
                fn_1_19904(2);
                for (i = 0; i < 5; i++) {
                    if (i == sys->menuNo2) {
                        Hu3DModelAttrReset(model->mdlId[i + 0x3C], 1);
                        model->mdlScale[i + 0x3C]->x = 1.0f;
                        model->mdlScale[i + 0x3C]->y = 1.0f;
                        model->mdlScale[i + 0x3C]->z = 1.0f;
                    } else {
                        Hu3DModelAttrSet(model->mdlId[i + 0x3C], 1);
                        model->mdlScale[i + 0x3C]->x = 0.0f;
                        model->mdlScale[i + 0x3C]->y = 0.0f;
                        model->mdlScale[i + 0x3C]->z = 0.0f;
                    }
                }
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(1, sys->menuNo2 + 0x60020);
                sys->selSub = 0;
                sys->stat = 0x1D;
                return;
            }
            if ((HuPadBtnDown[0] & 0x100) != 0) {
                sys->stat = 0x11;
                fn_1_19904(3);
                return;
            }
            if ((HuPadBtnDown[0] & 0x200) != 0) {
                sys->stat = 0xE;
                fn_1_19904(5);
                return;
            }
            break;
        case 0x1D:
            if (MdProcTbl[0x11].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 13;
                return;
            }
            break;
        case 14:
            if (MdProcTbl[0x10].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 15;
                return;
            }
            break;
        case 15:
            if (MdProcTbl[0x48].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(3, 0x60016);
                MdMessDlgInsertMesReset();
                MdMessDlgInsertMesAdd(sys->menuNo + 0x60017, 0);
                sys->stat = 16;
                return;
            }
            break;
        case 16:
            if (MdProcTbl[0x46].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                if (sys->menuNo == 0) {
                    HuSprAttrSet(sprite->grpId[1], 0, 4);
                } else if (sys->menuNo == 8) {
                    HuSprAttrSet(sprite->grpId[1], 1, 4);
                } else {
                    HuSprAttrReset(sprite->grpId[1], 0, 4);
                    HuSprAttrReset(sprite->grpId[1], 1, 4);
                }
                sys->stat = 3;
                return;
            }
            break;
        case 17:
            if (MdProcTbl[0x10].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 0x12;
                return;
            }
            break;
        case 18:
            if (MdProcTbl[0x49].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(1, sys->menuNo3 + 0x60025);
                sys->stat = 0x13;
                return;
            }
            break;
        case 19:
            if (MdProcTbl[0x4B].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 0x14;
                return;
            }
            break;
        case 20:
            if (mess->mode == 5) {
                sys->stat = 0x15;
                return;
            }
            break;
        case 21:
            if (MdProcTbl[0xF].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 0x16;
                return;
            }
            break;
        case 22:
            if ((PADSTAT(sys) & 1) != 0 || (PADSTAT(sys) & 2) != 0) {
                if ((PADSTAT(sys) & 1) != 0) {
                    sys->sel = 0;
                    if (--sys->menuNo3 == -1) {
                        sys->menuNo3 = 1;
                    }
                } else if ((PADSTAT(sys) & 2) != 0) {
                    sys->sel = 1;
                    sys->menuNo3++;
                    sys->menuNo3 = sys->menuNo3 % 2;
                }
                Hu3DAnmNoSet(model->animId[2], sys->menuNo3);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(1, sys->menuNo3 + 0x60025);
                fn_1_19904(2);
                sys->selSub = 0;
                sys->stat = 0x1E;
                return;
            }
            if ((HuPadBtnDown[0] & 0x100) != 0) {
                sys->stat = 0x1A;
                fn_1_19904(3);
                return;
            }
            if ((HuPadBtnDown[0] & 0x200) != 0) {
                sys->stat = 0x17;
                fn_1_19904(5);
                return;
            }
            break;
        case 0x1E:
            if (MdProcTbl[0x11].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 0x16;
                return;
            }
            break;
        case 23:
            if (MdProcTbl[0x10].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 0x18;
                return;
            }
            break;
        case 24:
            if (MdProcTbl[0x4C].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(1, sys->menuNo2 + 0x60020);
                sys->stat = 0x19;
                return;
            }
            break;
        case 25:
            if (MdProcTbl[0x4A].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 0xB;
                return;
            }
            break;
        case 26:
            if (MdProcTbl[0x10].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 0x1B;
                return;
            }
            break;
        case 27:
            if (MdProcTbl[0x4D].func(sys, MdProcTbl) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 0;
                sys->flowNo = 0xB;
            }
            break;
    }
}

void fn_1_8F18(NEL_SYS *sys, MDFLOWENT *tbl)
{
    s16 grp;
    s16 i;
    s32 ret;
    SYSSPRITE *sprite;
    SYSMODEL *volatile model;
    SYSMESS *volatile mess;

    sprite = (SYSSPRITE *)sys->sprite;
    model = (SYSMODEL *)sys->model;
    mess = (SYSMESS *)sys->mess;
    switch (sys->stat) {
        case 0:
            sys->unk287 = 0;
            MdSpriteDispSet(1, 1);
            MdSpriteDispSet(7, 1);
            grp = sprite->grpId[1];
            HuSprGrpPosSet(grp, 0.0f, 0.0f);
            for (i = 0; i < 8; i++) {
                HuSprAttrReset(grp, i, 4);
                HuSprPosSet(grp, i, -100.0f, -100.0f);
            }
            grp = sprite->grpId[6];
            HuSprGrpPosSet(grp, 0.0f, 0.0f);
            for (i = 0; i < 8; i++) {
                HuSprAttrReset(grp, i, 4);
                HuSprPosSet(grp, i, -100.0f, -100.0f);
            }
            sys->stat = 1;
            return;
        case 1:
            if ((ret = MdProcTbl[0x36].func(sys, MdProcTbl)) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 2;
                return;
            }
            break;
        case 2:
            if ((ret = MdProcTbl[0x12].func(sys, MdProcTbl)) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(3, 0x60027);
                MdMessDlgInsertMesReset();
                if (sys->modeSel == 0) {
                    MdMessDlgInsertMesAdd(sys->mdlNo[sys->unk287], 0);
                } else if (sys->modeSel == 1) {
                    MdMessDlgInsertMesAdd((&sys->winMes0)[sys->unk287] + 0x80000, 0);
                }
                fn_1_18C68(sys, 0);
                sys->stat = 3;
                return;
            }
            break;
        case 3:
            if ((PADSTAT(sys) & 1) != 0 || (PADSTAT(sys) & 2) != 0) {
                if (sys->modeSel == 0) {
                    if ((PADSTAT(sys) & 1) != 0) {
                        sys->sel = 0;
                        if (--sys->unk287 == -1) {
                            sys->unk287 = 3;
                        }
                    } else if ((PADSTAT(sys) & 2) != 0) {
                        sys->sel = 1;
                        sys->unk287++;
                        sys->unk287 = sys->unk287 % 4;
                    }
                } else if (sys->modeSel == 1) {
                    if ((PADSTAT(sys) & 1) != 0) {
                        sys->sel = 0;
                        if (--sys->unk287 == -1) {
                            sys->unk287 = 1;
                        }
                    } else if ((PADSTAT(sys) & 2) != 0) {
                        sys->sel = 1;
                        sys->unk287++;
                        sys->unk287 = sys->unk287 % 2;
                    }
                }
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(3, 0x60027);
                MdMessDlgInsertMesReset();
                if (sys->modeSel == 0) {
                    MdMessDlgInsertMesAdd(sys->mdlNo[sys->unk287], 0);
                } else if (sys->modeSel == 1) {
                    MdMessDlgInsertMesAdd((&sys->winMes0)[sys->unk287] + 0x80000, 0);
                }
                fn_1_18C68(sys, 0);
                fn_1_19904(2);
                sys->stat = 4;
                return;
            }
            if ((PADSTAT(sys) & 8) != 0 || (PADSTAT(sys) & 4) != 0) {
                if ((PADSTAT(sys) & 8) != 0) {
                    sys->sel = 2;
                    sys->unk283[sys->unk287]++;
                    sys->unk283[sys->unk287] = sys->unk283[sys->unk287] % 10;
                } else if ((PADSTAT(sys) & 4) != 0) {
                    sys->sel = 3;
                    if (--sys->unk283[sys->unk287] == -1) {
                        sys->unk283[sys->unk287] = 9;
                    }
                }
                fn_1_18C68(sys, 0);
                fn_1_19904(2);
                sys->stat = 4;
                return;
            }
            if ((HuPadBtnDown[0] & 0x100) != 0) {
                sys->stat = 8;
                fn_1_19904(3);
                return;
            }
            if ((HuPadBtnDown[0] & 0x200) != 0) {
                grp = sprite->grpId[6];
                for (i = 0; i < 8; i++) {
                    HuSprPosSet(grp, i, -100.0f, -100.0f);
                }
                fn_1_19904(5);
                sys->stat = 5;
                return;
            }
            break;
        case 4:
            if ((ret = MdProcTbl[0x14].func(sys, MdProcTbl)) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 3;
                return;
            }
            break;
        case 5:
            if ((ret = MdProcTbl[0x13].func(sys, MdProcTbl)) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 6;
                return;
            }
            break;
        case 6:
            if ((ret = MdProcTbl[0x37].func(sys, MdProcTbl)) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 7;
                return;
            }
            break;
        case 7:
            if ((ret = MdProcTbl[0x4E].func(sys, MdProcTbl)) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                MdMessDlgMesSet(0, -1);
                MdMessDlgMesSet(1, sys->menuNo3 + 0x60025);
                grp = sprite->grpId[1];
                HuSprGrpPosSet(grp, 288.0f, 240.0f);
                for (i = 0; i < 8; i++) {
                    HuSprPosSet(grp, i, 0.0f, -350.0f);
                }
                sys->stat = 0x14;
                sys->flowNo = 0xA;
                return;
            }
            break;
        case 8:
            if ((ret = MdProcTbl[0x13].func(sys, MdProcTbl)) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 9;
                return;
            }
            break;
        case 9:
            if ((ret = MdProcTbl[0x38].func(sys, MdProcTbl)) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 0;
                sys->flowNo = 0xC;
            }
            break;
    }
}

void fn_1_974C(NEL_SYS *sys, MDFLOWENT *tbl)
{
    s8 i;
    s32 j;
    s32 ret;
    s16 camBit;
    s16 parId;
    float rotY;
    SYSSPRITE *sprite;
    SYSMODEL *model;
    SYSMESS *mess;
    ParticleDotWork *dot;
    ParticleGlowWork *glow;
    HuVecF *pos;

    sprite = (SYSSPRITE *)sys->sprite;
    model = (SYSMODEL *)sys->model;
    mess = (SYSMESS *)sys->mess;
    switch (sys->stat) {
        case 0:
            MdMessDlgMesSet(0, -1);
            MdMessDlgMesSet(2, 0x60028);
            fn_1_19904(0xA);
            sys->stat = 1;
            return;
        case 1:
            if (mess->mode == 5) {
                switch (mess->choice) {
                    case -1:
                    case 1:
                        MdMessDlgMesSet(0, -1);
                        MdMessDlgMesSet(2, 0x60029);
                        sys->stat = 2;
                        return;
                    case 0:
                        MdMessDlgMesSet(0, -1);
                        MdMessDlgMesSet(1, 0x6002A);
                        fn_1_19B10(1);
                        sys->stat = 4;
                        return;
                }
            } else {
                return;
            }
            break;
        case 9:
            if ((ret = MdProcTbl[0x39].func(sys, MdProcTbl)) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 2;
                sys->flowNo = 0xB;
                return;
            }
            break;
        case 2:
            if (mess->mode == 5) {
                switch (mess->choice) {
                    case -1:
                    case 1:
                        sys->stat = 9;
                        return;
                    case 0:
                        MdSpriteDispSet(3, 0);
                        MdSpriteDispSet(4, 0);
                        MdSpriteDispSet(6, 0);
                        MdSpriteDispSet(7, 0);
                        for (i = 0; i < 4; i++) {
                            if (sys->fukiMode[i] == 2) {
                                MdRingModeSet(sys, i, 4);
                            } else if (sys->fukiMode[i] == 3) {
                                MdRingModeSet(sys, sys->mdlSelNo[i + 4] + 4, 4);
                            }
                        }
                        sys->stat = 3;
                        return;
                }
            }
            break;
        case 3:
            if ((ret = MdProcTbl[0x5A].func(sys, MdProcTbl)) == 1) {
                MdProcTbl[2].func(sys, MdProcTbl);
                HuSprAttrSet(sprite->grpId[1], 4, 4);
                HuSprAttrSet(sprite->grpId[1], 5, 4);
                HuSprAttrSet(sprite->grpId[1], 6, 4);
                HuSprAttrSet(sprite->grpId[1], 7, 4);
                for (j = 0; j < 0xB; j++) {
                    MdRingDispSet(sys, j, 1);
                }
                Hu3DModelAttrSet(model->mdlId[1], 1);
                for (j = 0; j < 0xA; j++) {
                    Hu3DModelAttrSet(model->mdlId[j + 2], 1);
                }
                Hu3DLayerHookSet(9, NULL);
                MdFukiInit(sys);
                sys->stat = 0;
                sys->flowNo = 4;
                return;
            }
            break;
        case 4:
            Hu3DModelAttrSet(model->mdlId[1], 1);
            for (j = 0; j < 0xA; j++) {
                Hu3DModelAttrSet(model->mdlId[j + 2], 1);
            }
            for (j = 0; j < 0xA; j++) {
                Hu3DModelAttrSet(model->mdlId[j + 0x10], 1);
            }
            for (j = 0; j < 4; j++) {
                Hu3DModelAttrReset(model->mdlId[sys->mdlNo[j] + 2], 1);
                Hu3DModelLayerSet(model->mdlId[sys->mdlNo[j] + 2], 5);
                Hu3DModelCameraSet(model->mdlId[sys->mdlNo[j] + 2], ((SYSCAMERA_WORK *)sys->camera)->camera[2].bit);
                Hu3DModelPosSet(model->mdlId[sys->mdlNo[j] + 2], MdVecTbl[j + 0x5B].x, MdVecTbl[j + 0x5B].y, MdVecTbl[j + 0x5B].z);
            }
            Hu3DLayerHookSet(9, NULL);
            for (j = 0; j < 4; j++) {
                camBit = ((SYSCAMERA_WORK *)sys->camera)->camera[2].bit;
                dot = HuMemDirectMallocNum(0, 0xC, 0x10000000);
                parId = Hu3DParManCreate(sprite->animP[29], 100, &ParticleDot1);
                dot->parManId[0] = parId;
                Hu3DParManAttrSet(parId, 0x65);
                Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parId), 1);
                Hu3DParManRotSet(parId, 90.0f, 0.0f, 0.0f);
                Hu3DModelLayerSet(Hu3DParManModelIDGet(parId), 5);
                Hu3DModelCameraSet(Hu3DParManModelIDGet(parId), camBit);
                parId = Hu3DParManCreate(sprite->animP[29], 100, &ParticleDot2);
                dot->parManId[1] = parId;
                Hu3DParManAttrSet(parId, 0x163);
                Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parId), 1);
                rotY = frandmod(0x50) - 0x28;
                Hu3DParManRotSet(parId, frandmod(0x28) + 0x46, 0.0f, rotY);
                Hu3DModelLayerSet(Hu3DParManModelIDGet(parId), 5);
                Hu3DModelCameraSet(Hu3DParManModelIDGet(parId), camBit);
                parId = Hu3DParManCreate(sprite->animP[29], 100, &ParticleDot3);
                dot->parManId[2] = parId;
                Hu3DParManAttrSet(parId, 0x21);
                Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parId), 1);
                Hu3DParManRotSet(parId, 0.0f, 0.0f, 0.0f);
                Hu3DModelLayerSet(Hu3DParManModelIDGet(parId), 5);
                Hu3DModelCameraSet(Hu3DParManModelIDGet(parId), camBit);
                ParWork.ent[j].dot = dot;
                pos = model->mdlPos[sys->mdlNo[j] + 2];
                dot = ParWork.ent[j].dot;
                Hu3DParManPosSet(dot->parManId[0], pos->x, pos->y, pos->z);
                Hu3DParManTimeLimitSet(dot->parManId[0], 2);
                Hu3DParManAttrReset(dot->parManId[0], 1);
                Hu3DParManPosSet(dot->parManId[1], pos->x, pos->y, pos->z);
                Hu3DParManTimeLimitSet(dot->parManId[1], 2);
                Hu3DParManAttrReset(dot->parManId[1], 1);
                Hu3DParManPosSet(dot->parManId[2], pos->x, pos->y, pos->z);
                Hu3DParManTimeLimitSet(dot->parManId[2], 5);
                Hu3DParManAttrReset(dot->parManId[2], 1);
                camBit = ((SYSCAMERA_WORK *)sys->camera)->camera[2].bit;
                glow = HuMemDirectMallocNum(0, 0xC, 0x10000000);
                parId = Hu3DParManCreate(sprite->animP[27], 100, &ParticleGlow);
                glow->parManId = parId;
                Hu3DParManAttrSet(parId, 0x65);
                Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parId), 1);
                Hu3DParManRotSet(parId, 90.0f, 0.0f, 0.0f);
                Hu3DModelLayerSet(Hu3DParManModelIDGet(parId), 5);
                Hu3DModelCameraSet(Hu3DParManModelIDGet(parId), camBit);
                ParWork.ent[j].glow = glow;
                pos = model->mdlPos[sys->mdlNo[j] + 2];
                glow = ParWork.ent[j].glow;
                Hu3DParManPosSet(glow->parManId, pos->x, pos->y, pos->z);
                Hu3DParManTimeLimitSet(glow->parManId, 2);
                Hu3DParManAttrReset(glow->parManId, 1);
            }
            fn_1_19904(7);
            MdSpriteDispSet(3, 0);
            MdSpriteDispSet(4, 0);
            MdSpriteDispSet(7, 0);
            for (i = 0; i < 4; i++) {
                if (sys->fukiMode[i] == 2) {
                    MdRingModeSet(sys, i, 4);
                } else if (sys->fukiMode[i] == 3) {
                    MdRingModeSet(sys, sys->mdlSelNo[i + 4] + 4, 4);
                }
            }
            MdMessWinKill(2);
            sys->stat = 5;
            return;
        case 5:
            for (j = 0; j < 4; j++) {
                pos = model->mdlPos[sys->mdlNo[j] + 2];
                glow = ParWork.ent[j].glow;
                Hu3DParManPosSet(glow->parManId, pos->x, pos->y, pos->z);
                Hu3DParManTimeLimitSet(glow->parManId, 2);
                Hu3DParManAttrReset(glow->parManId, 1);
            }
            WipeColorSet(0xFF, 0xFF, 0xFF);
            WipeCreate(2, 0, 0x3C);
            fn_1_19904(1);
            sys->stat = 6;
            return;
        case 7:
            if ((ret = MdProcTbl[0x59].func(sys, MdProcTbl)) == 1) {
                WipeColorSet(0xFF, 0xFF, 0xFF);
                WipeCreate(2, 0, 0x3C);
                fn_1_19904(1);
                MdProcTbl[2].func(sys, MdProcTbl);
                sys->stat = 6;
            }
            for (j = 0; j < 4; j++) {
                pos = model->mdlPos[sys->mdlNo[j] + 2];
                glow = ParWork.ent[j].glow;
                Hu3DParManPosSet(glow->parManId, pos->x, pos->y, pos->z);
                Hu3DParManTimeLimitSet(glow->parManId, 2);
                Hu3DParManAttrReset(glow->parManId, 1);
            }
            return;
        case 6:
            MdProcTbl[0x59].func(sys, MdProcTbl);
            for (j = 0; j < 4; j++) {
                pos = model->mdlPos[sys->mdlNo[j] + 2];
                glow = ParWork.ent[j].glow;
                Hu3DParManPosSet(glow->parManId, pos->x, pos->y, pos->z);
                Hu3DParManTimeLimitSet(glow->parManId, 2);
                Hu3DParManAttrReset(glow->parManId, 1);
            }
            if (WipeCheckEnd() == 0) {
                MdProcTbl[2].func(sys, MdProcTbl);
                fn_1_17354(sys);
                CharDataClose(-1);
                CharMotionInit(GwPlayerConf[0].charNo);
                CharMotionInit(GwPlayerConf[1].charNo);
                CharMotionInit(GwPlayerConf[2].charNo);
                CharMotionInit(GwPlayerConf[3].charNo);
                omOvlHisChg(0, ((OMOVLHIS *)sys->ovlHis)->ovl, 1, ((OMOVLHIS *)sys->ovlHis)->stat);
                sys->ovlMode = 1;
                sys->ovlNo = MdMdlSelTbl[sys->selNo3];
                sys->unk240 = 1;
                sys->exitReqF = 1;
                return;
            }
            break;
        case 8:
            if (sys->loadStat == 2) {
                CharDataClose(-1);
                CharMotionInit(GwPlayerConf[0].charNo);
                CharMotionInit(GwPlayerConf[1].charNo);
                CharMotionInit(GwPlayerConf[2].charNo);
                CharMotionInit(GwPlayerConf[3].charNo);
                omOvlHisChg(0, ((OMOVLHIS *)sys->ovlHis)->ovl, 1, ((OMOVLHIS *)sys->ovlHis)->stat);
                sys->ovlMode = 1;
                sys->ovlNo = MdMdlSelTbl[sys->selNo3];
                sys->unk240 = 1;
                sys->exitReqF = 1;
            }
            break;
    }
}

s32 fn_1_A4D4(NEL_SYS *sys, MDPROCENT *tbl)
{
    NEL_BLENDDBG *volatile blend = &sys->blendDbg;

    MdSpriteCreate(sys);
    HuPrcVSleep();
    MdRingCreate(sys);
    MdModelCreate(sys);
    MdToueiTexBufInit(sys);
    HuPrcVSleep();
    MdParticleCreate(sys);
    MdToueiMdlInit(sys);
    MdFukiInit(sys);
    HuPrcVSleep();
    HuDataDirClose(0xC30000);
    return 0;
}

s32 fn_1_A554(NEL_SYS *sys, MDPROCENT *tbl)
{
    if (sys->unk240 == 0) {
        WipeCreate(2, 0, 0x3C);
        while (WipeCheckEnd() != 0) {
            HuPrcVSleep();
        }
    }
    fn_1_19904(0xB);
    MdMessKill();
    if (sys->ovlMode == 0) {
        omOvlGotoEx(sys->ovlNo, 1, 0, 0);
    } else if (sys->ovlMode == 1) {
        omOvlCallEx(sys->ovlNo, 1, 0, 0);
    } else if (sys->ovlMode == 2) {
        omOvlReturnEx(1, 1);
    }
    HuPrcEnd();
    while (1) {
        HuPrcVSleep();
    }
}

s32 fn_1_A61C(NEL_SYS *sys, MDPROCENT *tbl)
{
    sys->initF1 = 0;
    sys->initF2 = 0;
    sys->initF3 = 0;
    sys->oscTime1 = 0.0f;
    sys->oscTime1b = 0.0f;
    sys->oscTime1c = 0.0f;
    sys->oscTime2 = 0.0f;
    sys->oscTime2b = 0.0f;
    sys->oscVal1 = 0.0f;
    sys->oscVal1b = 0.0f;
    sys->oscVal1c = 0.0f;
    sys->oscVal2 = 0.0f;
    sys->oscVal2b = 0.0f;
    sys->sclVal35 = 0.0f;
    sys->sclVal36 = 0.0f;
    return 0;
}

s32 fn_1_A6FC(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    float *p;

    if (sys->initF1 == 0) {
        MdMathPoint3Dto2D(&MdVecTbl[0xF], 2, &pt);
        sys->pos0x = pt.x - MdVecTbl[5].x;
        sys->pos0y = pt.y;
        MdMathPoint3Dto2D(&MdVecTbl[0x10], 2, &pt);
        sys->pos1y = pt.x + MdVecTbl[5].x;
        sys->pos2x = pt.y;
        HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
        sys->initF1 = 1;
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(1, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    p = (float *)MdSpritePosGet(1, 0);
    MdMathLerp(p, -12.0f, sys->pos0x, sys->oscVal1);
    p[1] = sys->pos0y;
    p = (float *)MdSpritePosGet(1, 1);
    MdMathLerp(p, 588.0f, sys->pos1y, sys->oscVal1);
    p[1] = sys->pos2x;
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_A898(NEL_SYS *sys, MDPROCENT *tbl)
{
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *volatile sprite = (SYSSPRITE *)sys->sprite;
    float *p;

    if (sys->initF1 == 0) {
        p = (float *)MdSpritePosGet(1, 0);
        sys->pos0x = p[0];
        sys->pos0y = p[1];
        p = (float *)MdSpritePosGet(1, 1);
        sys->pos1y = p[0];
        sys->pos2x = p[1];
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(2, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    p = (float *)MdSpritePosGet(1, 0);
    MdMathLerp(p, sys->pos0x, -12.0f, sys->oscVal1);
    p[1] = sys->pos0y;
    p = (float *)MdSpritePosGet(1, 1);
    MdMathLerp(p, sys->pos1y, 588.0f, sys->oscVal1);
    p[1] = sys->pos2x;
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_A9D8(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    HuVec2f *p;

    if (sys->initF2 == 0) {
        MdMathPoint3Dto2D(&MdVecTbl[sys->sel + 0xF], 2, &pt);
        if (sys->sel == 0) {
            sys->sclPos0x = pt.x - MdVecTbl[5].x;
            HuSprAttrReset(sprite->grpId[1], 4, 4);
        } else if (sys->sel == 1) {
            sys->sclPos0x = pt.x + MdVecTbl[5].x;
            HuSprAttrReset(sprite->grpId[1], 5, 4);
        }
        sys->sclPos0y = pt.y;
        HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
        sys->initF2 = 1;
    }
    MdMathOscillate(3, &sys->oscTime2, &sys->oscVal2, sys->oscSpd);
    if (sys->sel == 0) {
        p = MdSpritePosGet(1, 0);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x - MdVecTbl[5].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpritePosGet(1, 4);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x - MdVecTbl[5].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpriteScaleGet(1, 0);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[5].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[5].z, sys->oscVal2);
        p = MdSpriteScaleGet(1, 4);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[5].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[5].z, sys->oscVal2);
        HuSprTPLvlSet(1, 4, sys->oscVal2);
    } else if (sys->sel == 1) {
        p = MdSpritePosGet(1, 1);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x + MdVecTbl[5].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpritePosGet(1, 5);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x + MdVecTbl[5].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpriteScaleGet(1, 1);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[5].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[5].z, sys->oscVal2);
        p = MdSpriteScaleGet(1, 5);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[5].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[5].z, sys->oscVal2);
        HuSprTPLvlSet(1, 5, sys->oscVal2);
    }
    if (sys->oscTime2 >= 1.0f) {
        if (sys->sel == 0) {
            HuSprAttrSet(sprite->grpId[1], 4, 4);
        } else {
            HuSprAttrSet(sprite->grpId[1], 5, 4);
        }
        return 1;
    }
    return 0;
}

s32 fn_1_ADE8(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    float *p;

    if (sys->initF1 == 0) {
        MdMathPoint3Dto2D(&MdVecTbl[0x15], 2, &pt);
        sys->pos0x = pt.x - MdVecTbl[6].x;
        sys->pos0y = pt.y;
        MdMathPoint3Dto2D(&MdVecTbl[0x18], 2, &pt);
        sys->pos1y = pt.x + MdVecTbl[6].x;
        sys->pos2x = pt.y;
        HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
        sys->initF1 = 1;
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(1, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    p = (float *)MdSpritePosGet(1, 0);
    MdMathLerp(p, -12.0f, sys->pos0x, sys->oscVal1);
    p[1] = sys->pos0y;
    p = (float *)MdSpritePosGet(1, 1);
    MdMathLerp(p, 588.0f, sys->pos1y, sys->oscVal1);
    p[1] = sys->pos2x;
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_AF84(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    float *p;

    if (sys->initF1 == 0) {
        MdMathPoint3Dto2D(&MdVecTbl[0x15], 2, &pt);
        sys->pos0x = pt.x - MdVecTbl[6].x;
        sys->pos0y = pt.y;
        MdMathPoint3Dto2D(&MdVecTbl[0x18], 2, &pt);
        sys->pos1y = pt.x + MdVecTbl[6].x;
        sys->pos2x = pt.y;
        HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
        sys->initF1 = 1;
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(1, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    p = (float *)MdSpritePosGet(1, 0);
    MdMathLerp(p, sys->pos0x, -12.0f, sys->oscVal1);
    p[1] = sys->pos0y;
    p = (float *)MdSpritePosGet(1, 1);
    MdMathLerp(p, sys->pos1y, 588.0f, sys->oscVal1);
    p[1] = sys->pos2x;
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_B120(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    HuVec2f *p;

    if (sys->initF2 == 0) {
        MdMathPoint3Dto2D(&MdVecTbl[sys->sel * 3 + 0x15], 2, &pt);
        if (sys->sel == 0) {
            sys->sclPos0x = pt.x - MdVecTbl[6].x;
            HuSprAttrReset(sprite->grpId[1], 4, 4);
        } else if (sys->sel == 1) {
            sys->sclPos0x = pt.x + MdVecTbl[6].x;
            HuSprAttrReset(sprite->grpId[1], 5, 4);
        }
        sys->sclPos0y = pt.y;
        HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
        sys->initF2 = 1;
    }
    MdMathOscillate(3, &sys->oscTime2, &sys->oscVal2, sys->oscSpd);
    if (sys->sel == 0) {
        p = MdSpritePosGet(1, 0);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x - MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpritePosGet(1, 4);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x - MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpriteScaleGet(1, 0);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        p = MdSpriteScaleGet(1, 4);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        HuSprTPLvlSet(1, 4, sys->oscVal2);
    } else if (sys->sel == 1) {
        p = MdSpritePosGet(1, 1);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x + MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpritePosGet(1, 5);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x + MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpriteScaleGet(1, 1);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        p = MdSpriteScaleGet(1, 5);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        HuSprTPLvlSet(1, 5, sys->oscVal2);
    }
    if (sys->oscTime2 >= 1.0f) {
        if (sys->sel == 0) {
            HuSprAttrSet(sprite->grpId[1], 4, 4);
        } else {
            HuSprAttrSet(sprite->grpId[1], 5, 4);
        }
        return 1;
    }
    return 0;
}

s32 fn_1_B534(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    float *p;

    if (sys->initF1 == 0) {
        MdMathPoint3Dto2D(&MdVecTbl[62], 2, &pt);
        sys->pos0x = pt.x - MdVecTbl[8].x;
        sys->pos0y = pt.y;
        MdMathPoint3Dto2D(&MdVecTbl[65], 2, &pt);
        sys->pos1y = pt.x + MdVecTbl[8].x;
        sys->pos2x = pt.y;
        HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
        sys->initF1 = 1;
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(1, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    p = (float *)MdSpritePosGet(1, 0);
    MdMathLerp(p, -12.0f, sys->pos0x, sys->oscVal1);
    p[1] = sys->pos0y;
    p = (float *)MdSpritePosGet(1, 1);
    MdMathLerp(p, 588.0f, sys->pos1y, sys->oscVal1);
    p[1] = sys->pos2x;
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_B6D0(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    float *p;

    if (sys->initF1 == 0) {
        MdMathPoint3Dto2D(&MdVecTbl[62], 2, &pt);
        sys->pos0x = pt.x - MdVecTbl[8].x;
        sys->pos0y = pt.y;
        MdMathPoint3Dto2D(&MdVecTbl[65], 2, &pt);
        sys->pos1y = pt.x + MdVecTbl[8].x;
        sys->pos2x = pt.y;
        HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
        sys->initF1 = 1;
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(1, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    p = (float *)MdSpritePosGet(1, 0);
    MdMathLerp(p, sys->pos0x, -12.0f, sys->oscVal1);
    p[1] = sys->pos0y;
    p = (float *)MdSpritePosGet(1, 1);
    MdMathLerp(p, sys->pos1y, 588.0f, sys->oscVal1);
    p[1] = sys->pos2x;
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_B86C(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    HuVec2f *p;

    if (sys->initF2 == 0) {
        MdMathPoint3Dto2D(&MdVecTbl[sys->sel * 3 + 0x3E], 2, &pt);
        if (sys->sel == 0) {
            sys->sclPos0x = pt.x - MdVecTbl[8].x;
            HuSprAttrReset(sprite->grpId[1], 4, 4);
        } else if (sys->sel == 1) {
            sys->sclPos0x = pt.x + MdVecTbl[8].x;
            HuSprAttrReset(sprite->grpId[1], 5, 4);
        }
        sys->sclPos0y = pt.y;
        HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
        sys->initF2 = 1;
    }
    MdMathOscillate(3, &sys->oscTime2, &sys->oscVal2, sys->oscSpd);
    if (sys->sel == 0) {
        p = MdSpritePosGet(1, 0);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x - MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpritePosGet(1, 4);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x - MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpriteScaleGet(1, 0);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        p = MdSpriteScaleGet(1, 4);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        HuSprTPLvlSet(1, 4, sys->oscVal2);
    } else if (sys->sel == 1) {
        p = MdSpritePosGet(1, 1);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x + MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpritePosGet(1, 5);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x + MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpriteScaleGet(1, 1);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        p = MdSpriteScaleGet(1, 5);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        HuSprTPLvlSet(1, 5, sys->oscVal2);
    }
    if (sys->oscTime2 >= 1.0f) {
        if (sys->sel == 0) {
            HuSprAttrSet(sprite->grpId[1], 4, 4);
        } else {
            HuSprAttrSet(sprite->grpId[1], 5, 4);
        }
        return 1;
    }
    return 0;
}

s32 fn_1_BC80(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    float *p;

    if (sys->initF1 == 0) {
        MdMathPoint3Dto2D(&MdVecTbl[78], 2, &pt);
        sys->pos0x = pt.x;
        sys->pos0y = pt.y;
        HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
        sys->initF1 = 1;
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(1, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    p = (float *)MdSpritePosGet(1, 0);
    MdMathLerp(p, -12.0f, sys->pos0x - MdVecTbl[9].x, sys->oscVal1);
    p[1] = sys->pos0y;
    p = (float *)MdSpritePosGet(1, 1);
    MdMathLerp(p, 588.0f, sys->pos0x + MdVecTbl[9].x, sys->oscVal1);
    p[1] = sys->pos0y;
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_BDF4(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    float *p;

    if (sys->initF1 == 0) {
        MdMathPoint3Dto2D(&MdVecTbl[78], 2, &pt);
        sys->pos0x = pt.x;
        sys->pos0y = pt.y;
        HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
        sys->initF1 = 1;
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(1, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    p = (float *)MdSpritePosGet(1, 0);
    MdMathLerp(p, sys->pos0x - MdVecTbl[9].x, -12.0f, sys->oscVal1);
    p[1] = sys->pos0y;
    p = (float *)MdSpritePosGet(1, 1);
    MdMathLerp(p, sys->pos0x + MdVecTbl[9].x, 588.0f, sys->oscVal1);
    p[1] = sys->pos0y;
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_BF68(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    HuVec2f *p;

    if (sys->initF2 == 0) {
        MdMathPoint3Dto2D(&MdVecTbl[0x4E], 2, &pt);
        if (sys->sel == 0) {
            sys->sclPos0x = pt.x - MdVecTbl[9].x;
            HuSprAttrReset(sprite->grpId[1], 4, 4);
        } else if (sys->sel == 1) {
            sys->sclPos0x = pt.x + MdVecTbl[9].x;
            HuSprAttrReset(sprite->grpId[1], 5, 4);
        }
        sys->sclPos0y = pt.y;
        HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
        sys->initF2 = 1;
    }
    MdMathOscillate(3, &sys->oscTime2, &sys->oscVal2, sys->oscSpd);
    if (sys->sel == 0) {
        p = MdSpritePosGet(1, 0);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x - MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpritePosGet(1, 4);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x - MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpriteScaleGet(1, 0);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        p = MdSpriteScaleGet(1, 4);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        HuSprTPLvlSet(1, 4, sys->oscVal2);
    } else if (sys->sel == 1) {
        p = MdSpritePosGet(1, 1);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x + MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpritePosGet(1, 5);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x + MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpriteScaleGet(1, 1);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        p = MdSpriteScaleGet(1, 5);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        HuSprTPLvlSet(1, 5, sys->oscVal2);
    }
    if (sys->oscTime2 >= 1.0f) {
        if (sys->sel == 0) {
            HuSprAttrSet(sprite->grpId[1], 4, 4);
        } else {
            HuSprAttrSet(sprite->grpId[1], 5, 4);
        }
        return 1;
    }
    return 0;
}

s32 fn_1_C36C(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    float *p;

    if (sys->initF1 == 0) {
        MdMathPoint3Dto2D(&MdVecTbl[81], 2, &pt);
        sys->pos0x = pt.x;
        sys->pos0y = pt.y;
        HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
        sys->initF1 = 1;
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(1, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    p = (float *)MdSpritePosGet(1, 0);
    MdMathLerp(p, -12.0f, sys->pos0x - MdVecTbl[10].x, sys->oscVal1);
    p[1] = sys->pos0y;
    p = (float *)MdSpritePosGet(1, 1);
    MdMathLerp(p, 588.0f, sys->pos0x + MdVecTbl[10].x, sys->oscVal1);
    p[1] = sys->pos0y;
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_C4E0(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    float *p;

    if (sys->initF1 == 0) {
        MdMathPoint3Dto2D(&MdVecTbl[81], 2, &pt);
        sys->pos0x = pt.x;
        sys->pos0y = pt.y;
        HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
        sys->initF1 = 1;
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(1, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    p = (float *)MdSpritePosGet(1, 0);
    MdMathLerp(p, sys->pos0x - MdVecTbl[10].x, -12.0f, sys->oscVal1);
    p[1] = sys->pos0y;
    p = (float *)MdSpritePosGet(1, 1);
    MdMathLerp(p, sys->pos0x + MdVecTbl[10].x, 588.0f, sys->oscVal1);
    p[1] = sys->pos0y;
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_C654(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    HuVec2f *p;

    if (sys->initF2 == 0) {
        MdMathPoint3Dto2D(&MdVecTbl[0x51], 2, &pt);
        if (sys->sel == 0) {
            sys->sclPos0x = pt.x - MdVecTbl[10].x;
            if (sys->selSub != 1 || sys->menuNo != 0) {
                HuSprAttrReset(sprite->grpId[1], 4, 4);
            }
        } else if (sys->sel == 1) {
            sys->sclPos0x = pt.x + MdVecTbl[10].x;
            if (sys->selSub != 1 || sys->menuNo != 8) {
                HuSprAttrReset(sprite->grpId[1], 5, 4);
            }
        }
        sys->sclPos0y = pt.y;
        HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
        sys->initF2 = 1;
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(3, &sys->oscTime2, &sys->oscVal2, sys->oscSpd);
    if (sys->sel == 0) {
        p = MdSpritePosGet(1, 0);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x - MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpritePosGet(1, 4);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x - MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpriteScaleGet(1, 0);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        p = MdSpriteScaleGet(1, 4);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        HuSprTPLvlSet(1, 4, sys->oscVal2);
    } else if (sys->sel == 1) {
        p = MdSpritePosGet(1, 1);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x + MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpritePosGet(1, 5);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x + MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpriteScaleGet(1, 1);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        p = MdSpriteScaleGet(1, 5);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        HuSprTPLvlSet(1, 5, sys->oscVal2);
    }
    if (sys->oscTime2 >= 1.0f) {
        if (sys->sel == 0) {
            HuSprAttrSet(sprite->grpId[1], 4, 4);
        } else {
            HuSprAttrSet(sprite->grpId[1], 5, 4);
        }
        return 1;
    }
    return 0;
}

s32 fn_1_CAA0(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt[4];
    s32 j;
    SYSSPRITE *volatile sprite = (SYSSPRITE *)sys->sprite;
    float *p;

    if (sys->initF1 == 0) {
        if (sys->modeSel == 0) {
            for (j = 0; j < 4; j++) {
                MdMathModel3Dto2D(sys->mdlNo[j] + 0x10, 2, &pt[j]);
            }
            sys->pos0x = pt[0].x - MdVecTbl[0xB].x;
            sys->pos0y = pt[0].y;
            sys->pos1y = pt[3].x + MdVecTbl[0xB].x;
            sys->pos2x = pt[3].y;
            sys->pos3x = pt[sys->unk287].x;
            sys->pos3y = pt[sys->unk287].y - MdVecTbl[0xB].y;
            sys->pos4y = pt[sys->unk287].x;
            sys->pos5x = MdVecTbl[0xB].z + pt[sys->unk287].y;
        } else if (sys->modeSel == 1) {
            for (j = 0; j < 2; j++) {
                MdMathModel3Dto2D(sys->mdlNo[sys->ringMdlGrp[j * 2]] + 0x10, 2, &pt[j * 2]);
                MdMathModel3Dto2D(sys->mdlNo[sys->ringMdlGrp[j * 2 + 1]] + 0x10, 2, &pt[j * 2 + 1]);
            }
            sys->pos0x = pt[0].x - MdVecTbl[0xB].x;
            sys->pos0y = pt[0].y;
            sys->pos1y = pt[3].x + MdVecTbl[0xB].x;
            sys->pos2x = pt[3].y;
            sys->pos3x = (pt[sys->unk287 * 2].x + pt[sys->unk287 * 2 + 1].x) * 0.5f;
            sys->pos3y = (pt[sys->unk287 * 2].y + pt[sys->unk287 * 2 + 1].y) * 0.5f - MdVecTbl[0xB].y;
            sys->pos4y = (pt[sys->unk287 * 2].x + pt[sys->unk287 * 2 + 1].x) * 0.5f;
            sys->pos5x = MdVecTbl[0xB].z + (pt[sys->unk287 * 2].y + pt[sys->unk287 * 2 + 1].y) * 0.5f;
        }
        sys->initF1 = 1;
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(1, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    p = (float *)MdSpritePosGet(1, 0);
    MdMathLerp(&p[0], -12.0f, sys->pos0x, sys->oscVal1);
    MdMathLerp(&p[1], sys->pos0y, sys->pos0y, sys->oscVal1);
    p = (float *)MdSpritePosGet(1, 1);
    MdMathLerp(&p[0], 588.0f, sys->pos1y, sys->oscVal1);
    MdMathLerp(&p[1], sys->pos2x, sys->pos2x, sys->oscVal1);
    p = (float *)MdSpritePosGet(1, 2);
    MdMathLerp(&p[0], sys->pos3x, sys->pos3x, sys->oscVal1);
    MdMathLerp(&p[1], -60.0f, sys->pos3y, sys->oscVal1);
    p = (float *)MdSpritePosGet(1, 3);
    MdMathLerp(&p[0], sys->pos3x, sys->pos4y, sys->oscVal1);
    MdMathLerp(&p[1], 540.0f, sys->pos5x, sys->oscVal1);
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}


s32 fn_1_CF8C(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt[4];
    s32 j;
    SYSSPRITE *volatile sprite = (SYSSPRITE *)sys->sprite;
    float *p;

    if (sys->initF1 == 0) {
        if (sys->modeSel == 0) {
            for (j = 0; j < 4; j++) {
                MdMathModel3Dto2D(sys->mdlNo[j] + 0x10, 2, &pt[j]);
            }
            sys->pos0x = pt[0].x - MdVecTbl[0xB].x;
            sys->pos0y = pt[0].y;
            sys->pos1y = pt[3].x + MdVecTbl[0xB].x;
            sys->pos2x = pt[3].y;
            sys->pos3x = pt[sys->unk287].x;
            sys->pos3y = pt[sys->unk287].y - MdVecTbl[0xB].y;
            sys->pos4y = pt[sys->unk287].x;
            sys->pos5x = MdVecTbl[0xB].z + pt[sys->unk287].y;
        } else if (sys->modeSel == 1) {
            for (j = 0; j < 2; j++) {
                MdMathModel3Dto2D(sys->mdlNo[sys->ringMdlGrp[j * 2]] + 0x10, 2, &pt[j * 2]);
                MdMathModel3Dto2D(sys->mdlNo[sys->ringMdlGrp[j * 2 + 1]] + 0x10, 2, &pt[j * 2 + 1]);
            }
            sys->pos0x = pt[0].x - MdVecTbl[0xB].x;
            sys->pos0y = pt[0].y;
            sys->pos1y = pt[3].x + MdVecTbl[0xB].x;
            sys->pos2x = pt[3].y;
            sys->pos3x = (pt[sys->unk287 * 2].x + pt[sys->unk287 * 2 + 1].x) * 0.5f;
            sys->pos3y = (pt[sys->unk287 * 2].y + pt[sys->unk287 * 2 + 1].y) * 0.5f - MdVecTbl[0xB].y;
            sys->pos4y = (pt[sys->unk287 * 2].x + pt[sys->unk287 * 2 + 1].x) * 0.5f;
            sys->pos5x = MdVecTbl[0xB].z + (pt[sys->unk287 * 2].y + pt[sys->unk287 * 2 + 1].y) * 0.5f;
        }
        sys->initF1 = 1;
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(1, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    p = (float *)MdSpritePosGet(1, 0);
    MdMathLerp(&p[0], sys->pos0x, -12.0f, sys->oscVal1);
    MdMathLerp(&p[1], sys->pos0y, sys->pos0y, sys->oscVal1);
    p = (float *)MdSpritePosGet(1, 1);
    MdMathLerp(&p[0], sys->pos1y, 588.0f, sys->oscVal1);
    MdMathLerp(&p[1], sys->pos2x, sys->pos2x, sys->oscVal1);
    p = (float *)MdSpritePosGet(1, 2);
    MdMathLerp(&p[0], sys->pos3x, sys->pos3x, sys->oscVal1);
    MdMathLerp(&p[1], sys->pos3y, -60.0f, sys->oscVal1);
    p = (float *)MdSpritePosGet(1, 3);
    MdMathLerp(&p[0], sys->pos4y, sys->pos3x, sys->oscVal1);
    MdMathLerp(&p[1], sys->pos5x, 540.0f, sys->oscVal1);
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_D478(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt[4];
    s32 j;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    HuVec2f *p;

    if (sys->initF2 == 0) {
        if (sys->modeSel == 0) {
            for (j = 0; j < 4; j++) {
                MdMathModel3Dto2D(sys->mdlNo[j] + 0x10, 2, &pt[j]);
            }
            if (sys->sel == 0) {
                sys->sclPos0x = pt[0].x - MdVecTbl[0xB].x;
                sys->sclPos0y = pt[0].y;
            } else if (sys->sel == 1) {
                sys->sclPos0x = pt[3].x + MdVecTbl[0xB].x;
                sys->sclPos0y = pt[3].y;
            } else if (sys->sel == 2) {
                sys->sclPos0x = pt[sys->unk287].x;
                sys->sclPos0y = pt[sys->unk287].y - MdVecTbl[0xB].y;
            } else if (sys->sel == 3) {
                sys->sclPos0x = pt[sys->unk287].x;
                sys->sclPos0y = MdVecTbl[0xB].z + pt[sys->unk287].y;
            }
        } else if (sys->modeSel == 1) {
            for (j = 0; j < 2; j++) {
                MdMathModel3Dto2D(sys->mdlNo[sys->ringMdlGrp[j * 2]] + 0x10, 2, &pt[j * 2]);
                MdMathModel3Dto2D(sys->mdlNo[sys->ringMdlGrp[j * 2 + 1]] + 0x10, 2, &pt[j * 2 + 1]);
            }
            if (sys->sel == 0) {
                sys->sclPos0x = pt[0].x - MdVecTbl[0xB].x;
                sys->sclPos0y = pt[0].y;
            } else if (sys->sel == 1) {
                sys->sclPos0x = pt[3].x + MdVecTbl[0xB].x;
                sys->sclPos0y = pt[3].y;
            } else if (sys->sel == 2) {
                sys->sclPos0x = (pt[sys->unk287 * 2].x + pt[sys->unk287 * 2 + 1].x) * 0.5f;
                sys->sclPos0y = (pt[sys->unk287 * 2].y + pt[sys->unk287 * 2 + 1].y) * 0.5f - MdVecTbl[0xB].y;
            } else if (sys->sel == 3) {
                sys->sclPos0x = (pt[sys->unk287 * 2].x + pt[sys->unk287 * 2 + 1].x) * 0.5f;
                sys->sclPos0y = MdVecTbl[0xB].z + (pt[sys->unk287 * 2].y + pt[sys->unk287 * 2 + 1].y) * 0.5f;
            }
        }
        if (sys->sel == 0) {
            HuSprAttrReset(sprite->grpId[1], 4, 4);
        } else if (sys->sel == 1) {
            HuSprAttrReset(sprite->grpId[1], 5, 4);
        } else if (sys->sel == 2) {
            HuSprAttrReset(sprite->grpId[1], 6, 4);
        } else if (sys->sel == 3) {
            HuSprAttrReset(sprite->grpId[1], 7, 4);
        }
        HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
        sys->initF2 = 1;
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(3, &sys->oscTime2, &sys->oscVal2, sys->oscSpd);
    if (sys->sel == 0) {
        p = MdSpritePosGet(1, 0);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x - MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpritePosGet(1, 4);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x - MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpriteScaleGet(1, 0);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        p = MdSpriteScaleGet(1, 4);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        HuSprTPLvlSet(1, 4, sys->oscVal2);
    } else if (sys->sel == 1) {
        p = MdSpritePosGet(1, 1);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x + MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpritePosGet(1, 5);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x + MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpriteScaleGet(1, 1);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        p = MdSpriteScaleGet(1, 5);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        HuSprTPLvlSet(1, 5, sys->oscVal2);
    } else if (sys->sel == 2) {
        p = MdSpritePosGet(1, 2);
        p->x = sys->sclPos0x;
        MdMathLerp(&p->y, sys->sclPos0y, sys->sclPos0y - MdVecTbl[6].y, sys->oscVal2);
        p = MdSpritePosGet(1, 6);
        p->x = sys->sclPos0x;
        MdMathLerp(&p->y, sys->sclPos0y, sys->sclPos0y - MdVecTbl[6].y, sys->oscVal2);
        p = MdSpriteScaleGet(1, 2);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        p = MdSpriteScaleGet(1, 6);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        HuSprTPLvlSet(1, 6, sys->oscVal2);
    } else if (sys->sel == 3) {
        p = MdSpritePosGet(1, 3);
        p->x = sys->sclPos0x;
        MdMathLerp(&p->y, sys->sclPos0y, sys->sclPos0y + MdVecTbl[6].y, sys->oscVal2);
        p = MdSpritePosGet(1, 7);
        p->x = sys->sclPos0x;
        MdMathLerp(&p->y, sys->sclPos0y, sys->sclPos0y + MdVecTbl[6].y, sys->oscVal2);
        p = MdSpriteScaleGet(1, 3);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        p = MdSpriteScaleGet(1, 7);
        MdMathLerp(&p->x, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        MdMathLerp(&p->y, 1.0f, MdVecTbl[6].z, sys->oscVal2);
        HuSprTPLvlSet(1, 7, sys->oscVal2);
    }
    if (sys->oscTime2 >= 1.0f) {
        if (sys->sel == 0) {
            HuSprAttrSet(sprite->grpId[1], 4, 4);
        } else if (sys->sel == 1) {
            HuSprAttrSet(sprite->grpId[1], 5, 4);
        } else if (sys->sel == 2) {
            HuSprAttrSet(sprite->grpId[1], 6, 4);
        } else if (sys->sel == 3) {
            HuSprAttrSet(sprite->grpId[1], 7, 4);
        }
        return 1;
    }
    return 0;
}

s32 fn_1_DF0C(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    HuVec2f *p;

    if (sys->initF1 == 0) {
        MdMathModel3Dto2D(sys->mdlNo[sys->ringMdlNo[sys->selNo]] + 0x10, 2, &pt);
        sys->pos0x = MdVecTbl[7].x + (pt.x + MdVecTbl[0xD].x);
        sys->pos0y = MdVecTbl[7].y + (pt.y + MdVecTbl[0xD].y);
        sys->pos1x = MdVecTbl[7].z + (pt.x + MdVecTbl[0xD].x);
        HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
        p = MdSpriteScaleGet(1, 0);
        p->y = MdVecTbl[0xC].x;
        p->x = MdVecTbl[0xC].x;
        p = MdSpriteScaleGet(1, 1);
        p->y = MdVecTbl[0xC].x;
        p->x = MdVecTbl[0xC].x;
        sys->initF1 = 1;
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(1, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    p = MdSpritePosGet(1, 0);
    MdMathLerp(&p->x, -12.0f, sys->pos0x, sys->oscVal1);
    p->y = sys->pos0y;
    p = MdSpritePosGet(1, 1);
    MdMathLerp(&p->x, 588.0f, sys->pos1x, sys->oscVal1);
    p->y = sys->pos0y;
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_E130(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    HuVec2f *p;

    if (sys->initF1 == 0) {
        MdMathModel3Dto2D(sys->mdlNo[sys->ringMdlNo[sys->selNo]] + 0x10, 2, &pt);
        sys->pos0x = MdVecTbl[7].x + (pt.x + MdVecTbl[0xD].x);
        sys->pos0y = MdVecTbl[7].y + (pt.y + MdVecTbl[0xD].y);
        sys->pos1x = MdVecTbl[7].z + (pt.x + MdVecTbl[0xD].x);
        HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
        sys->initF1 = 1;
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(2, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    p = MdSpritePosGet(1, 0);
    MdMathLerp(&p->x, sys->pos0x, -12.0f, sys->oscVal1);
    p->y = sys->pos0y;
    p = MdSpritePosGet(1, 1);
    MdMathLerp(&p->x, sys->pos1x, 588.0f, sys->oscVal1);
    p->y = sys->pos0y;
    if (sys->oscTime1 >= 1.0f) {
        p = MdSpriteScaleGet(1, 0);
        p->y = 1.0f;
        p->x = 1.0f;
        p = MdSpriteScaleGet(1, 1);
        p->y = 1.0f;
        p->x = 1.0f;
        return 1;
    }
    return 0;
}

s32 fn_1_E354(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    HuVec2f *p;

    if (sys->initF2 == 0) {
        MdMathModel3Dto2D(sys->mdlNo[sys->ringMdlNo[sys->selNo]] + 0x10, 2, &pt);
        if (sys->sel == 0) {
            sys->sclPos0x = MdVecTbl[7].x + (pt.x + MdVecTbl[0xD].x);
            HuSprAttrReset(sprite->grpId[1], 4, 4);
        } else if (sys->sel == 1) {
            sys->sclPos0x = MdVecTbl[7].z + (pt.x + MdVecTbl[0xD].x);
            HuSprAttrReset(sprite->grpId[1], 5, 4);
        }
        sys->sclPos0y = MdVecTbl[7].y + (pt.y + MdVecTbl[0xD].y);
        HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
        sys->initF2 = 1;
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(3, &sys->oscTime2, &sys->oscVal2, sys->oscSpd);
    if (sys->sel == 0) {
        p = MdSpritePosGet(1, 0);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x - MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpritePosGet(1, 4);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x - MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpriteScaleGet(1, 0);
        MdMathLerp(&p->x, MdVecTbl[0xC].x, MdVecTbl[0xC].y, sys->oscVal2);
        MdMathLerp(&p->y, MdVecTbl[0xC].x, MdVecTbl[0xC].y, sys->oscVal2);
        p = MdSpriteScaleGet(1, 4);
        MdMathLerp(&p->x, MdVecTbl[0xC].x, MdVecTbl[0xC].y, sys->oscVal2);
        MdMathLerp(&p->y, MdVecTbl[0xC].x, MdVecTbl[0xC].y, sys->oscVal2);
        HuSprTPLvlSet(1, 4, sys->oscVal2);
    } else if (sys->sel == 1) {
        p = MdSpritePosGet(1, 1);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x + MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpritePosGet(1, 5);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x + MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpriteScaleGet(1, 1);
        MdMathLerp(&p->x, MdVecTbl[0xC].x, MdVecTbl[0xC].y, sys->oscVal2);
        MdMathLerp(&p->y, MdVecTbl[0xC].x, MdVecTbl[0xC].y, sys->oscVal2);
        p = MdSpriteScaleGet(1, 5);
        MdMathLerp(&p->x, MdVecTbl[0xC].x, MdVecTbl[0xC].y, sys->oscVal2);
        MdMathLerp(&p->y, MdVecTbl[0xC].x, MdVecTbl[0xC].y, sys->oscVal2);
        HuSprTPLvlSet(1, 5, sys->oscVal2);
    }
    if (sys->oscTime2 >= 1.0f) {
        if (sys->sel == 0) {
            HuSprAttrSet(sprite->grpId[1], 4, 4);
        } else {
            HuSprAttrSet(sprite->grpId[1], 5, 4);
        }
        return 1;
    }
    return 0;
}

s32 fn_1_E7C0(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    HuVec2f *p;

    if (sys->initF1 == 0) {
        MdMathModel3Dto2D(sys->mdlNo[sys->mdlSelNoC[sys->selNo]] + 0x10, 2, &pt);
        sys->pos0x = MdVecTbl[7].x + (pt.x + MdVecTbl[0xD].x);
        sys->pos0y = MdVecTbl[7].y + (pt.y + MdVecTbl[0xD].y);
        sys->pos1x = MdVecTbl[7].z + (pt.x + MdVecTbl[0xD].x);
        HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
        p = MdSpriteScaleGet(1, 0);
        p->y = MdVecTbl[0xC].x;
        p->x = MdVecTbl[0xC].x;
        p = MdSpriteScaleGet(1, 1);
        p->y = MdVecTbl[0xC].x;
        p->x = MdVecTbl[0xC].x;
        sys->initF1 = 1;
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(1, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    p = MdSpritePosGet(1, 0);
    MdMathLerp(&p->x, -12.0f, sys->pos0x, sys->oscVal1);
    p->y = sys->pos0y;
    p = MdSpritePosGet(1, 1);
    MdMathLerp(&p->x, 588.0f, sys->pos1x, sys->oscVal1);
    p->y = sys->pos0y;
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_E9E4(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    HuVec2f *p;

    if (sys->initF1 == 0) {
        MdMathModel3Dto2D(sys->mdlNo[sys->mdlSelNoC[sys->selNo]] + 0x10, 2, &pt);
        sys->pos0x = MdVecTbl[7].x + (pt.x + MdVecTbl[0xD].x);
        sys->pos0y = MdVecTbl[7].y + (pt.y + MdVecTbl[0xD].y);
        sys->pos1x = MdVecTbl[7].z + (pt.x + MdVecTbl[0xD].x);
        HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
        sys->initF1 = 1;
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(2, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    p = MdSpritePosGet(1, 0);
    MdMathLerp(&p->x, sys->pos0x, -12.0f, sys->oscVal1);
    p->y = sys->pos0y;
    p = MdSpritePosGet(1, 1);
    MdMathLerp(&p->x, sys->pos1x, 588.0f, sys->oscVal1);
    p->y = sys->pos0y;
    if (sys->oscTime1 >= 1.0f) {
        p = MdSpriteScaleGet(1, 0);
        p->y = 1.0f;
        p->x = 1.0f;
        p = MdSpriteScaleGet(1, 1);
        p->y = 1.0f;
        p->x = 1.0f;
        return 1;
    }
    return 0;
}

s32 fn_1_EC08(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    HuVec2f *p;

    if (sys->initF2 == 0) {
        MdMathModel3Dto2D(sys->mdlNo[sys->mdlSelNoC[sys->selNo]] + 0x10, 2, &pt);
        if (sys->sel == 0) {
            sys->sclPos0x = MdVecTbl[7].x + (pt.x + MdVecTbl[0xD].x);
            HuSprAttrReset(sprite->grpId[1], 4, 4);
        } else if (sys->sel == 1) {
            sys->sclPos0x = MdVecTbl[7].z + (pt.x + MdVecTbl[0xD].x);
            HuSprAttrReset(sprite->grpId[1], 5, 4);
        }
        sys->sclPos0y = MdVecTbl[7].y + (pt.y + MdVecTbl[0xD].y);
        HuSprGrpPosSet(sprite->grpId[1], 0.0f, 0.0f);
        sys->initF2 = 1;
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(3, &sys->oscTime2, &sys->oscVal2, sys->oscSpd);
    if (sys->sel == 0) {
        p = MdSpritePosGet(1, 0);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x - MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpritePosGet(1, 4);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x - MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpriteScaleGet(1, 0);
        MdMathLerp(&p->x, MdVecTbl[0xC].x, MdVecTbl[0xC].y, sys->oscVal2);
        MdMathLerp(&p->y, MdVecTbl[0xC].x, MdVecTbl[0xC].y, sys->oscVal2);
        p = MdSpriteScaleGet(1, 4);
        MdMathLerp(&p->x, MdVecTbl[0xC].x, MdVecTbl[0xC].y, sys->oscVal2);
        MdMathLerp(&p->y, MdVecTbl[0xC].x, MdVecTbl[0xC].y, sys->oscVal2);
        HuSprTPLvlSet(1, 4, sys->oscVal2);
    } else if (sys->sel == 1) {
        p = MdSpritePosGet(1, 1);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x + MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpritePosGet(1, 5);
        MdMathLerp(&p->x, sys->sclPos0x, sys->sclPos0x + MdVecTbl[6].y, sys->oscVal2);
        p->y = sys->sclPos0y;
        p = MdSpriteScaleGet(1, 1);
        MdMathLerp(&p->x, MdVecTbl[0xC].x, MdVecTbl[0xC].y, sys->oscVal2);
        MdMathLerp(&p->y, MdVecTbl[0xC].x, MdVecTbl[0xC].y, sys->oscVal2);
        p = MdSpriteScaleGet(1, 5);
        MdMathLerp(&p->x, MdVecTbl[0xC].x, MdVecTbl[0xC].y, sys->oscVal2);
        MdMathLerp(&p->y, MdVecTbl[0xC].x, MdVecTbl[0xC].y, sys->oscVal2);
        HuSprTPLvlSet(1, 5, sys->oscVal2);
    }
    if (sys->oscTime2 >= 1.0f) {
        if (sys->sel == 0) {
            HuSprAttrSet(sprite->grpId[1], 4, 4);
        } else {
            HuSprAttrSet(sprite->grpId[1], 5, 4);
        }
        return 1;
    }
    return 0;
}


s32 fn_1_F074(NEL_SYS *sys, MDPROCENT *tbl)
{
    s32 i;
    SYSSPRITE *volatile sprite = (SYSSPRITE *)sys->sprite;
    SYSMODEL *model = (SYSMODEL *)sys->model;

    MdSpriteDispSet(3, 0);
    fn_1_17BD8(sys);
    for (i = 0; i < 4; i++) {
        Hu3DModelAttrSet(model->mdlId[i + 0x42], 1);
        if (sys->fukiMode[i] != 1) {
            Hu3DModelAttrReset(model->mdlId[i + 0x42], 1);
        }
    }
    return 1;
}

s32 fn_1_F120(NEL_SYS *sys, MDPROCENT *tbl)
{
    s32 i;
    SYSSPRITE *volatile sprite = (SYSSPRITE *)sys->sprite;
    SYSMODEL *model = (SYSMODEL *)sys->model;

    MdSpriteDispSet(3, 0);
    for (i = 0; i < 4; i++) {
        Hu3DModelAttrSet(model->mdlId[i + 0x42], 1);
    }
    return 1;
}

s32 fn_1_F19C(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    s32 j;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    s16 grp = ((SYSSPRITE *)sys->sprite)->grpId[3];
    HuVec2f *p;

    if (sys->initF1 == 0) {
        for (j = 0; j < 4; j++) {
            MdMathModel3Dto2D(j + 0x10, 2, &pt);
            HuSprAttrReset(grp, j, 4);
            p = MdSpritePosGet(3, j);
            p->x = pt.x + MdVecTbl[0xD].x;
            p->y = pt.y + MdVecTbl[0xD].y;
        }
        HuSprGrpPosSet(grp, 0.0f, 0.0f);
        sys->initF1 = 1;
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(1, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    MdMathOscillate(3, &sys->oscTime1b, &sys->oscVal1b, sys->oscSpd);
    for (j = 0; j < 4; j++) {
        p = MdSpriteScaleGet(3, j);
        MdMathLerpOfs(&p->x, 0.0f, 1.0f, sys->oscVal1, 0.5f * sys->oscVal1b);
        MdMathLerpOfs(&p->y, 0.0f, 1.0f, sys->oscVal1, 0.5f * sys->oscVal1b);
    }
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_F378(NEL_SYS *sys, MDPROCENT *tbl)
{
    HuVecF pt;
    s32 j;
    SYSMODEL *volatile model = (SYSMODEL *)sys->model;
    s16 grp = ((SYSSPRITE *)sys->sprite)->grpId[3];
    HuVec2f *p;

    if (sys->initF1 == 0) {
        for (j = 0; j < 4; j++) {
            MdMathModel3Dto2D(j + 0x10, 2, &pt);
            HuSprAttrReset(grp, j, 4);
            p = MdSpritePosGet(3, j);
            p->x = pt.x + MdVecTbl[0xD].x;
            p->y = pt.y + MdVecTbl[0xD].y;
        }
        HuSprGrpPosSet(grp, 0.0f, 0.0f);
        sys->initF1 = 1;
    }
    sys->oscSpd = 15.0f;
    MdMathOscillate(1, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    for (j = 0; j < 4; j++) {
        p = MdSpriteScaleGet(3, j);
        MdMathLerp(&p->x, 1.0f, 0.0f, sys->oscVal1);
        MdMathLerp(&p->y, 1.0f, 0.0f, sys->oscVal1);
    }
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_F518(NEL_SYS *sys, MDPROCENT *tbl)
{
    sys->oscSpd = 30.0f;
    MdMathOscillate(1, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    MdMathLerp((float *)MdSpritePosGet(5, 0), -450.0f, -MdVecTbl[0xE].x, sys->oscVal1);
    MdMathLerp((float *)MdSpritePosGet(5, 1), 450.0f, MdVecTbl[0xE].x, sys->oscVal1);
    fn_1_1923C(sys);
    if (sys->sel != 1) {
        MdProcTbl[0x51].func(sys, MdProcTbl);
    }
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_F614(NEL_SYS *sys, MDPROCENT *tbl)
{
    sys->oscSpd = 30.0f;
    MdMathOscillate(2, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    MdMathLerp((float *)MdSpritePosGet(5, 0), -MdVecTbl[0xE].x, -450.0f, sys->oscVal1);
    MdMathLerp((float *)MdSpritePosGet(5, 1), MdVecTbl[0xE].x, 450.0f, sys->oscVal1);
    fn_1_1923C(sys);
    if (sys->sel != 1) {
        MdProcTbl[0x52].func(sys, MdProcTbl);
    }
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_F710(NEL_SYS *sys, MDPROCENT *tbl)
{
    s32 i;
    SYSMODEL *model;
    float angle;
    float ang;
    float r = 200.0f;
    float y = -200.0f;
    float z;

    if (sys->initF1 == 0) {
        sys->pos0x = sys->angle;
        sys->pos0y = 60.0f;
        sys->initF1 = 1;
    }
    sys->oscSpd = 30.0f;
    MdMathOscillate(1, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    MdMathLerp(&sys->angle, sys->pos0x, sys->pos0x + sys->pos0y, sys->oscVal1);
    angle = sys->angle;
    ang = 0.0f;
    model = (SYSMODEL *)sys->model;
    for (i = 0; i < 6; i++) {
        ang = angle + 60.0f * i;
        z = (float)(400.0 + r * sin(3.141592653589793 * ang / 180.0));
        Hu3DModelPosSet(model->mdlId[i + 0x1A], r * cos(3.141592653589793 * ang / 180.0), y, z);
    }
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_F924(NEL_SYS *sys, MDPROCENT *tbl)
{
    s32 i;
    SYSMODEL *model;
    float angle;
    float ang;
    float r = 200.0f;
    float y = -200.0f;
    float z;

    if (sys->initF1 == 0) {
        sys->pos0x = sys->angle;
        sys->pos0y = 60.0f;
        sys->initF1 = 1;
    }
    sys->oscSpd = 30.0f;
    MdMathOscillate(1, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    MdMathLerp(&sys->angle, sys->pos0x, sys->pos0x - sys->pos0y, sys->oscVal1);
    angle = sys->angle;
    ang = 0.0f;
    model = (SYSMODEL *)sys->model;
    for (i = 0; i < 6; i++) {
        ang = angle + 60.0f * i;
        z = (float)(400.0 + r * sin(3.141592653589793 * ang / 180.0));
        Hu3DModelPosSet(model->mdlId[i + 0x1A], r * cos(3.141592653589793 * ang / 180.0), y, z);
    }
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_FB38(NEL_SYS *sys, MDPROCENT *tbl)
{
    SYSMODEL *model = (SYSMODEL *)sys->model;
    HuVecF *pos;
    HuVecF *rot;

    sys->oscSpd = 60.0f;
    MdMathOscillate(2, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    MdMathOscillate(3, &sys->oscTime1b, &sys->oscVal1b, sys->oscSpd);
    pos = model->mdlPos[0x1A];
    rot = model->mdlRot[0x1A];
    MdMathLerp(&pos->x, MdVecTbl[0].x, 30.0f, sys->oscVal1b);
    MdMathLerp(&pos->z, MdVecTbl[0].z, MdVecTbl[1].z, sys->oscVal1);
    MdMathLerp(&rot->y, 0.0f, 360.0f, sys->oscVal1);
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_FC50(NEL_SYS *sys, MDPROCENT *tbl)
{
    SYSMODEL *model = (SYSMODEL *)sys->model;
    HuVecF *pos;
    HuVecF *rot;

    if (sys->initF1 == 0) {
        MdCurveTbl[4] = MdVecTbl[1].y;
        MdMathCurveInit(5, &sys->sclVal4, MdCurveTbl, &sys->sclVal12);
        sys->initF1 = 1;
    }
    sys->oscSpd = 60.0f;
    MdMathOscillate(2, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    MdMathOscillate(3, &sys->oscTime1b, &sys->oscVal1b, sys->oscSpd);
    MdMathOscillate(0, &sys->oscTime1c, &sys->oscVal1c, sys->oscSpd);
    pos = model->mdlPos[0x1A];
    rot = model->mdlRot[0x1A];
    MdMathLerp(&pos->x, MdVecTbl[0].x, 30.0f, sys->oscVal1b);
    MdMathCurveGet(&pos->y, 5, &sys->sclVal4, MdCurveTbl, &sys->sclVal12, sys->oscVal1c);
    MdMathLerp(&pos->z, MdVecTbl[0].z, MdVecTbl[1].z, sys->oscVal1);
    MdMathLerp(&rot->y, 0.0f, 360.0f, sys->oscVal1);
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_FDE0(NEL_SYS *sys, MDPROCENT *tbl)
{
    s32 i;
    SYSMODEL *model = (SYSMODEL *)sys->model;
    HuVecF *scl;

    sys->oscSpd = 15.0f;
    MdMathOscillate(1, &sys->oscTime1, &sys->oscVal1, sys->oscSpd);
    MdMathOscillate(3, &sys->oscTime1b, &sys->oscVal1b, sys->oscSpd);
    for (i = 0; i < 2; i++) {
        scl = model->mdlScale[i + 0x32];
        if (i == sys->modeSel) {
            MdMathLerpScaleOfs(scl, 0.0f, MdVecTbl[0x13].z, sys->oscVal1, sys->oscVal1b);
        } else {
            MdMathLerpScale(scl, 0.0f, MdVecTbl[0x13].y, sys->oscVal1);
        }
    }
    if (sys->oscTime1 >= 1.0f) {
        return 1;
    }
    return 0;
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
void ParticleGlowKill(ParticleGlowWork *p)
{
    Hu3DParManKill((s16)p->parManId);
}

/* ParticleGlowPosSet - move a glow to `pos`, then restart it with a
 * two-frame life limit and attribute bit 1. */
void ParticleGlowPosSet(ParticleGlowWork *p, Vec *pos)
{
    Hu3DParManPosSet((s16)p->parManId, pos->x, pos->y, pos->z);
    Hu3DParManTimeLimitSet((s16)p->parManId, 2);
    Hu3DParManAttrReset((s16)p->parManId, 1);
}
