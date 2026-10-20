#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/frand.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mg/actman.h"
#include "game/mg/seqman.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/printfunc.h"
#include "game/process.h"
#include "game/wipe.h"
#include "game/gamework.h"
#include "math.h"

#include "datadir_enum.h"
#include "REL/m571Dll.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* the call site in the binary only sets the three position arguments */
typedef HU3DLIGHTID (*LightCreate3)(float, float, float);
typedef void (*LightColorSet4)(HU3DLIGHTID, u8, u8, u8, u8);
typedef void (*ModelShadowSet2)(HU3DMODELID, s32);

u32 lbl_1_data_0[13] = {
    DATANUM(DATA_mgconst, 0x20), DATANUM(DATA_mgconst, 0x21), DATANUM(DATA_mgconst, 0x22),
    DATANUM(DATA_mgconst, 0x23), DATANUM(DATA_mgconst, 0x24), DATANUM(DATA_mgconst, 0x26),
    DATANUM(DATA_mgconst, 0x27), DATANUM(DATA_mgconst, 0x28), DATANUM(DATA_mgconst, 0x29),
    DATANUM(DATA_mgconst, 0x2A), DATANUM(DATA_mgconst, 0x2B), DATANUM(DATA_mgconst, 0x2C),
    DATANUM(DATA_mgconst, 0x2D)
};

u32 lbl_1_data_34[15] = {
    DATANUM(DATA_m571, 8), DATANUM(DATA_m571, 9), DATANUM(DATA_m571, 10), DATANUM(DATA_m571, 13),
    DATANUM(DATA_m571, 14), DATANUM(DATA_m571, 15), DATANUM(DATA_m571, 16), DATANUM(DATA_m572, 17),
    DATANUM(DATA_m571, 18), DATANUM(DATA_m571, 19), DATANUM(DATA_m571, 20), DATANUM(DATA_m571, 21),
    DATANUM(DATA_m571, 22), DATANUM(DATA_m571, 11), DATANUM(DATA_m571, 12)
};

Vec lbl_1_data_70 = { 0.0f, 100.0f, -750.0f };

M571SpawnPt lbl_1_data_7C[4] = { {70.0f, 60.0f}, {255.0f, 60.0f}, {386.0f, 60.0f}, {505.0f, 60.0f} };

Vec lbl_1_data_9C[2] = { { -3456.0f, 0.0f, -280.0f }, { 3500.0f, 0.0f, -280.0f } };

Vec lbl_1_data_B4[2] = { { 0.0f, 90.0f, 0.0f }, { 0.0f, 270.0f, 0.0f } };

Vec lbl_1_data_CC[2] = { { -47.0f, 0.0f, -280.0f }, { 47.0f, 0.0f, -280.0f } };

Vec lbl_1_data_E4 = { 0.0f, 10000.0f, 10000.0f };

Vec lbl_1_data_F0 = { 0.0f, 0.0f, 0.0f };

u8 lbl_1_data_FC[4] = { 0xFF, 0xFF, 0xFF, 0xFF };

char lbl_1_data_100[] = "a-itemhook-l";
char lbl_1_data_10D[] = "a-itemhook-r";
char lbl_1_data_11A[] = "itemhook1";
char lbl_1_data_124[] = "itemhook2";

u32 lbl_1_data_1A4[5] = {
    DATANUM(DATA_mariomot, 0), DATANUM(DATA_mariomot, 6), DATANUM(DATA_mariomot, 0x31),
    DATANUM(DATA_mariomot, 1), DATANUM(DATA_mariomot, 2)
};

OMCAMERAVIEW lbl_1_data_1B8 = { { 0.0f, 250.0f, -498.0f }, { 4.4f, 0.0f, 0.0f }, 530.0f };

OMCAMERAVIEW lbl_1_data_1D4 = { { 0.0f, 222.95f, 270.0f }, { -29.4f, 0.9f, 0.0f }, 1750.0f };

OMCAMERAVIEW lbl_1_data_1F0 = { { 0.0f, 250.0f, -498.0f }, { 4.4f, 0.0f, 0.0f }, 1200.0f };

float lbl_1_data_20C[12] = {
    -475.0f, 50.0f, -470.0f, 475.0f, 50.0f, -470.0f,
    -400.0f, 50.0f, 470.0f, 400.0f, 50.0f, 470.0f
};

Vec lbl_1_data_23C[4] = {
    { -240.0f, 0.0f, -450.0f }, { -80.36f, 0.0f, -450.0f },
    { 80.36f, 0.0f, -450.0f }, { 240.66f, 0.0f, -450.0f }
};

M571Grid lbl_1_data_26C = {
    { { -360.0f, 0.0f, -360.0f },
      { -240.0f, 0.0f, -360.0f },
      { -120.0f, 0.0f, -360.0f },
      { 0.0f, 0.0f, -360.0f },
      { 120.0f, 0.0f, -360.0f },
      { 240.0f, 0.0f, -360.0f },
      { 360.0f, 0.0f, -360.0f },
      { -360.0f, 0.0f, -240.0f },
      { -240.0f, 0.0f, -240.0f },
      { -120.0f, 0.0f, -240.0f },
      { 0.0f, 0.0f, -240.0f },
      { 120.0f, 0.0f, -240.0f },
      { 240.0f, 0.0f, -240.0f },
      { 360.0f, 0.0f, -240.0f },
      { -360.0f, 0.0f, -120.0f },
      { -240.0f, 0.0f, -120.0f },
      { -120.0f, 0.0f, -120.0f },
      { 0.0f, 0.0f, -120.0f },
      { 120.0f, 0.0f, -120.0f },
      { 240.0f, 0.0f, -120.0f },
      { 360.0f, 0.0f, -120.0f },
      { -360.0f, 0.0f, 0.0f },
      { -240.0f, 0.0f, 0.0f },
      { -120.0f, 0.0f, 0.0f },
      { 0.0f, 0.0f, 0.0f },
      { 120.0f, 0.0f, 0.0f },
      { 240.0f, 0.0f, 0.0f },
      { 360.0f, 0.0f, 0.0f },
      { -360.0f, 0.0f, 120.0f },
      { -240.0f, 0.0f, 120.0f },
      { -120.0f, 0.0f, 120.0f },
      { 0.0f, 0.0f, 120.0f },
      { 120.0f, 0.0f, 120.0f },
      { 240.0f, 0.0f, 120.0f },
      { 360.0f, 0.0f, 120.0f },
      { -360.0f, 0.0f, 240.0f },
      { -240.0f, 0.0f, 240.0f },
      { -120.0f, 0.0f, 240.0f },
      { 0.0f, 0.0f, 240.0f },
      { 120.0f, 0.0f, 240.0f },
      { 240.0f, 0.0f, 240.0f },
      { 360.0f, 0.0f, 240.0f },
      { -360.0f, 0.0f, 360.0f },
      { -240.0f, 0.0f, 360.0f },
      { -120.0f, 0.0f, 360.0f },
      { 0.0f, 0.0f, 360.0f },
      { 120.0f, 0.0f, 360.0f },
      { 240.0f, 0.0f, 360.0f },
      { 360.0f, 0.0f, 360.0f } },
    { { 1, 0, 1 },
      { 0, 1, 1 },
      { 1, 0, 1 },
      { 5, 1, 0 },
      { 1, 0, 1 },
      { 1, 0, 0 },
      { 1, 5, 1 },
      { 1, 0, 1 },
      { 1, 1, 0 },
      { 1, 0, 1 },
      { 0, 1, 5 },
      { 0, 1, 1 },
      { 5, 1, 0 },
      { 1, 1, 1 },
      { 1, 1, 0 },
      { 1, 1, 0 },
      { 1, 0, 1 },
      { 0, 1, 1 },
      { 1, 1, 0 },
      { 1, 1, 0 },
      { 1, 0, 5 },
      { 1, 1, 1 },
      { 1, 0, 1 },
      { 0, 5, 1 },
      { 0, 0, 1 },
      { 1, 1, 1 },
      { 1, 1, 1 },
      { 1, 1, 5 },
      { 0, 1, 1 },
      { 0, 1, 0 },
      { 1, 1, 1 },
      { 0, 5, 0 },
      { 1, 0, 1 },
      { 1, 1, 0 },
      { 0, 0, 0 },
      { 1, 0, 0 },
      { 1, 1, 1 },
      { 1, 5, 0 },
      { 1, 5, 1 },
      { 1, 1, 1 },
      { 0, 1, 1 },
      { 1, 0, 1 },
      { 0, 1, 5 },
      { 1, 0, 5 },
      { 1, 0, 1 },
      { 1, 1, 1 },
      { 0, 1, 1 },
      { 0, 0, 1 },
      { 1, 1, 1 } }
};

s32 lbl_1_data_704[8][49] = {
    { -1, -1, -1, 0, -1, -1, -1, -1, -1, 5, 1, 0, -1, -1, -1, 5, 0, 1, 1, 5, -1, 0, 1, 5, 5, 1, 1, 0, -1, 5, 0, 1, 5, 5, -1, -1, -1, 1, 0, 1, -1, -1, -1, -1, -1, 1, -1, -1, -1 },
    { -1, -1, -1, 5, -1, -1, -1, -1, -1, 0, 1, 0, -1, -1, -1, 0, 5, 1, 5, 1, -1, 5, 1, 0, 1, 0, 1, 5, -1, 5, 1, 1, 5, 0, -1, -1, -1, 1, 5, 1, -1, -1, -1, -1, -1, 0, -1, -1, -1 },
    { 0, 1, -1, 5, -1, -1, 0, 1, -1, 5, 1, -1, 5, -1, 0, -1, 0, 1, 1, -1, -1, 5, -1, 1, 0, 5, -1, -1, 0, -1, 0, 1, 0, -1, -1, 1, -1, 1, 5, -1, 5, -1, 0, 5, -1, 1, -1, -1, 0 },
    { 0, 5, -1, 1, -1, -1, 1, 1, -1, 0, 5, -1, 0, -1, 0, -1, 1, 5, 1, -1, -1, 0, -1, 5, 0, 0, -1, -1, 5, -1, 0, 1, 1, -1, -1, 1, -1, 0, 1, -1, 5, -1, 1, 0, -1, 5, -1, -1, 5 },
    { -1, -1, -1, 5, -1, -1, -1, -1, -1, 5, 1, 0, -1, -1, -1, 0, 1, 0, 1, 5, -1, 1, 5, 1, 0, 1, 5, 1, -1, -1, 1, 5, 0, -1, -1, -1, -1, 0, 0, 5, -1, -1, -1, -1, 5, 1, 1, -1, -1 },
    { -1, -1, -1, 0, -1, -1, -1, -1, -1, 1, 0, 5, -1, -1, -1, 1, 5, 1, 5, 1, -1, 5, 0, 1, 5, 0, 1, 1, -1, -1, 5, 0, 5, -1, -1, -1, -1, 1, 5, 0, -1, -1, -1, -1, 1, 0, 1, -1, -1 },
    { -1, 0, -1, -1, -1, 0, -1, 0, 1, 5, -1, 5, 1, 0, -1, 1, -1, 5, -1, 5, -1, -1, -1, 1, 1, 1, -1, -1, -1, 5, -1, 5, -1, 1, -1, 0, 5, 0, -1, 5, 1, 1, -1, 1, -1, -1, -1, 0, -1 },
    { -1, 1, -1, -1, -1, 5, -1, 5, 5, 0, -1, 0, 0, 0, -1, 0, -1, 5, -1, 0, -1, -1, -1, 5, 1, 5, -1, -1, -1, 1, -1, 5, -1, 1, -1, 1, 0, 1, -1, 1, 5, 1, -1, 1, -1, -1, -1, 1, -1 },
};

Vec lbl_1_data_D24 = { -100.0f, 8000.0f, 0.0f };

Vec lbl_1_data_D30 = { 0.0f, 1.0f, 0.0f };

Vec lbl_1_data_D3C = { 0.0f, 0.0f, 0.0f };

MGSEQ_PARAM lbl_1_data_D48 = {
    0,
    10,
    (MGSEQ_FUNC)fn_1_1518,
    (MGSEQ_FUNC)fn_1_1630,
    NULL,
    (MGSEQ_FUNC)fn_1_16F4,
    (MGSEQ_FUNC)fn_1_1760,
    (MGSEQ_FUNC)fn_1_18B8,
    (MGSEQ_FUNC)fn_1_1BA0,
    NULL,
    NULL
};

char lbl_1_data_D70[] = "******* M571 ObjectSetup *********\n";
char lbl_1_data_D94[] = "SOUNDMODE:%d\n";
char lbl_1_data_DA2[] = "Center.x:%.02f";
char lbl_1_data_DB1[] = "Center.y:%.02f";
char lbl_1_data_DC0[] = "Center.z:%.02f";
char lbl_1_data_DCF[] = "CRot.x:%.02f";
char lbl_1_data_DDC[] = "CRot.y:%.02f";
char lbl_1_data_DE9[] = "CZoom:%.02f";

OMOBJ *lbl_1_bss_0;
s32 lbl_1_bss_4;
s32 lbl_1_bss_8;
s16 lbl_1_bss_C;
s16 lbl_1_bss_E;
s16 lbl_1_bss_10;
s16 lbl_1_bss_12;
s16 lbl_1_bss_14[0x64];
MGPLAYER *lbl_1_bss_DC[4];
s32 lbl_1_bss_EC;
s32 lbl_1_bss_F0[25];
s16 lbl_1_bss_F0_pad;
s16 lbl_1_bss_156[0x35];
OMOBJ *lbl_1_bss_1C0;
OMOBJ *lbl_1_bss_1C4;
OMOBJ *lbl_1_bss_1C8[49];
OMOBJ *lbl_1_bss_28C[49];
OMOBJ **lbl_1_bss_350;
OMOBJ **lbl_1_bss_354;
s16 lbl_1_bss_358;
s16 lbl_1_bss_35A;
s32 lbl_1_bss_35C;
s32 lbl_1_bss_360;
s16 lbl_1_bss_364;
s16 lbl_1_bss_366;
OMOBJ *lbl_1_bss_368[20];
MGSCORE *lbl_1_bss_3B8[4];
s16 lbl_1_bss_3C8[4];
s16 lbl_1_bss_3D0[4];
s16 lbl_1_bss_3D8[4];
s16 lbl_1_bss_3E0[4];
f32 lbl_1_bss_3E8[4];
s32 lbl_1_bss_3F8[4];
s32 lbl_1_bss_408[4];
s32 lbl_1_bss_418[4];
s32 lbl_1_bss_428[4];
s32 lbl_1_bss_438;
s32 lbl_1_bss_43C[4];
s16 lbl_1_bss_44C[16];
s32 lbl_1_bss_46C;
OMOBJ *lbl_1_bss_470[2];
OMOBJ **lbl_1_bss_478;
M571Slot lbl_1_bss_47C[0x20];
u8 lbl_1_bss_57C[4];
s32 lbl_1_bss_580;
s32 lbl_1_bss_584[3];
Vec lbl_1_bss_590[4];
Vec lbl_1_bss_5C0[4];
Vec lbl_1_bss_5F0[4];
Vec lbl_1_bss_620[4];
Vec lbl_1_bss_650;
Vec lbl_1_bss_65C;
Vec lbl_1_bss_668;
Vec lbl_1_bss_674;
OMOBJMAN *lbl_1_bss_680;
s32 lbl_1_bss_684[3];

static const f32 m571Zero = 0.0f;
static const f32 m571Sca = 0.1f;
static const f32 m571ModelY = -1000.0f;
static const f32 m571PanBias = 32.0f;
static const f32 m571PanScale = 64.0f;
static const f32 m571VolBias = 96.0f;
static const f32 m571VolScale = 31.0f;
static const f32 m571ScreenXDiv = 576.0f;
static const f32 m571ScreenYDiv = 480.0f;
static const f64 m571Turn = 360.0;
static const f32 m571TurnF = 360.0f;
static const f32 m571HalfTurn = 180.0f;

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

// NONMATCHING: register allocation and expression form differ from the target build
void _epilog(void)
{
    const VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

// NONMATCHING: register init order and a few Hu3D call arities differ from the target build
void ObjectSetup(void)
{
    s32 i;

    OSReport(lbl_1_data_D70);
    OSReport(lbl_1_data_D94, OSGetSoundMode());
    lbl_1_bss_680 = MgActorObjectSetup();

    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 30.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    omCameraViewSet(&lbl_1_data_1B8);
    lbl_1_bss_1C4 = omAddObjEx(lbl_1_bss_680, 0x7FDA, 0, 0, OM_GRP_NONE, omOutView);

    lbl_1_bss_358 = ((LightCreate3)Hu3DGLightCreate)(0.0f, 0.0f, 0.0f);
    Hu3DGLightPosAimSetV(lbl_1_bss_358, &lbl_1_data_E4, &lbl_1_data_F0);
    Hu3DGLightStaticSet(lbl_1_bss_358, 1);
    Hu3DGLightInfinitytSet(lbl_1_bss_358);
    ((LightColorSet4)Hu3DGLightColorSet)(lbl_1_bss_358, lbl_1_data_FC[0], lbl_1_data_FC[1], lbl_1_data_FC[2],
        lbl_1_data_FC[3]);

    Hu3DShadowCreate(10.0f, 10.0f, 10000.0f);
    Hu3DShadowColSet(16, 16, 16);
    Hu3DShadowPosSet(&lbl_1_data_D24, &lbl_1_data_D30, &lbl_1_data_D3C);
    Hu3DShadowTPLvlSet(0.6f);

    fn_1_2D58();

    lbl_1_bss_12 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 2), HU_MEMNUM_OVL, 2));
    Hu3DModelAttrSet(lbl_1_bss_12, 1);
    lbl_1_bss_10 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 6), HU_MEMNUM_OVL, 2));
    Hu3DModelAttrSet(lbl_1_bss_10, 1);

    for (i = 0; i < 49; i++) {
        lbl_1_bss_28C[i] = 0;
    }

    lbl_1_bss_E = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 26), HU_MEMNUM_OVL, 2));
    lbl_1_bss_C = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 29), HU_MEMNUM_OVL, 2));
    Hu3DModelAttrSet(lbl_1_bss_E, 1);
    Hu3DModelAttrSet(lbl_1_bss_C, 1);

    lbl_1_bss_F0[0] = 0;
    lbl_1_bss_EC = 0;

    fn_1_2E28();

    lbl_1_bss_0 = omAddObjEx(lbl_1_bss_680, 0x66, 0x36, 0, OM_GRP_NONE, fn_1_68C);

    HuPrcVSleep();

    omMakeGroupEx(lbl_1_bss_680, 1, 4);
    lbl_1_bss_350 = omGetGroupMemberListEx(lbl_1_bss_680, 1);
    omMakeGroupEx(lbl_1_bss_680, 0, 4);
    lbl_1_bss_354 = omGetGroupMemberListEx(lbl_1_bss_680, 0);

    for (i = 0; i < 4; i++) {
        OMOBJ *obj = omAddObjEx(lbl_1_bss_680, 0x64, 0, 0, 0, fn_1_BF4);

        lbl_1_bss_354[i] = obj;
        obj->data = HuMemDirectMallocNum(0, 0x8C, 0x1000);
        obj->work[0] = i;
    }

    omMakeGroupEx(lbl_1_bss_680, 5, 1);
    lbl_1_bss_478 = omGetGroupMemberListEx(lbl_1_bss_680, 5);

    lbl_1_bss_478[0] = omAddObjEx(lbl_1_bss_680, 0x6A, 0, 0, 5, fn_1_7670);
    lbl_1_bss_478[0]->data = HuMemDirectMallocNum(0, 0xB0, 0x1000);
    lbl_1_bss_478[0]->work[0] = 0;

    lbl_1_bss_364 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 26), HU_MEMNUM_OVL, 2));
    Hu3DModelAttrSet(lbl_1_bss_364, 1);

    for (i = 0; i < 20; i++) {
        lbl_1_bss_368[i] = NULL;
    }

    lbl_1_bss_35C = -1;

    fn_1_2870(0);

    MgSeqCreatePrio(&lbl_1_data_D48, 1000);
}

// NONMATCHING: draft reconstruction, MGPLAYER field order not fully resolved
void fn_1_68C(OMOBJ *obj)
{
    MGACTOR_PARAM param;
    s32 i;
    s32 j;
    s16 mdlId[49];

    mdlId[0] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 0), HU_MEMNUM_OVL, 2));
    mdlId[1] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 1), HU_MEMNUM_OVL, 2));
    lbl_1_bss_156[0] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 3), HU_MEMNUM_OVL, 2));
    mdlId[2] = lbl_1_bss_156[0];
    lbl_1_bss_156[1] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 4), HU_MEMNUM_OVL, 2));
    mdlId[3] = lbl_1_bss_156[1];

    for (i = 0; i < 49; i++) {
        f32 y;
        Vec pos;

        mdlId[i + 4] = Hu3DModelLink(lbl_1_bss_12);
        lbl_1_bss_156[i + 2] = mdlId[i + 4];

        if (lbl_1_data_704[lbl_1_bss_8][i] == -1) {
            y = -1000.0f;
        } else {
            y = lbl_1_data_26C.cell[i].y;
        }

        pos.x = lbl_1_data_26C.pos[i].x;
        pos.z = lbl_1_data_26C.pos[i].z;
        Hu3DModelPosSet(mdlId[i + 4], pos.x, y, pos.z);
    }

    param.height = 150.0f;
    param.radius = 50.0f;
    param.param = 0;
    param.type = 0;
    param.narrowHook = NULL;
    param.attr = 0;
    param.correctHookParam = 0;
    param.correctHook = NULL;

    MgActorColMapInit(&lbl_1_bss_156[0], 0x33, 0x37);

    for (i = 0; i < 4; i++) {
        lbl_1_bss_DC[i] = MgPlayerCreate(i, &param, 4, 1, -3, NULL);
        param.param++;
    }

    for (i = 0; i < 4; i++) {
        lbl_1_bss_DC[i]->actor->pos.x = lbl_1_data_20C[i * 3 + 0];
        lbl_1_bss_DC[i]->actor->pos.y = lbl_1_data_20C[i * 3 + 1];
        lbl_1_bss_DC[i]->actor->pos.z = lbl_1_data_20C[i * 3 + 2];
    }

    MgPlayerVibrateCreate(lbl_1_bss_DC[0]);
    MgPlayerVibrateCreate(lbl_1_bss_DC[1]);
    MgPlayerVibrateCreate(lbl_1_bss_DC[2]);
    MgPlayerVibrateCreate(lbl_1_bss_DC[3]);

    MgActorColCylSet();

    Hu3DModelShadowMapSet(mdlId[0]);
    Hu3DModelAttrSet(mdlId[1], 0x4001);
    CharEffectLayerSet(5);
    Hu3DModelAttrSet(mdlId[2], 1);
    Hu3DModelAttrSet(mdlId[3], 1);

    for (i = 0; i < 49; i++) {
        Hu3DModelAttrSet(mdlId[i + 4], 1);
    }

    for (i = 0; i < 4; i++) {
        ((ModelShadowSet2)Hu3DModelShadowSet)(mdlId[i + 4], 1);
    }

    obj->objFunc = NULL;
}

// NONMATCHING: GwPlayerConf field picks and frandmod branches need fndiff verification
void fn_1_BF4(OMOBJ *obj)
{
    M571Player *p = (M571Player *)obj->data;
    s32 val;

    p->unk0.playerNo = obj->work[0];

    p->unk4 = GwPlayerConf[p->unk0.playerNo].charNo;
    p->unk8 = GwPlayerConf[p->unk0.playerNo].padNo;
    p->unkC = 0;
    p->unk30 = 0;
    p->hookCycle = 0;
    p->unk78 = 0;

    p->unk5C = GwPlayerConf[p->unk0.playerNo].dif;
    p->unk60 = 0;
    p->unk80 = 0;

    switch (p->unk5C) {
    case 0:
        val = frandmod(20) + 60;
        p->unk84 = val;
        p->unk88 = 2;
        break;
    case 1:
        val = frandmod(15) + 30;
        p->unk84 = val;
        p->unk88 = 7;
        break;
    case 2:
        val = frandmod(15) + 10;
        p->unk84 = val;
        p->unk88 = 15;
        break;
    case 3:
        val = frandmod(4) + 1;
        p->unk84 = val;
        p->unk88 = 0;
        break;
    }

    obj->objFunc = fn_1_D58;
}

// NONMATCHING: actor field offsets and the voice-pan expression are approximations
void fn_1_D58(OMOBJ *obj)
{
    M571Player *p = (M571Player *)obj->data;
    MGPLAYER *mp;
    s32 tileIdx;
    HuVecF screenPos;

    if (MgSeqModeGet() != 5) {
        fn_1_2870(0);
    } else {
        fn_1_2870(1);

        if (GwPlayerConf[p->unk0.playerNo].type != 0) {
            p->unk70 = 0;
            p->unk74.padY = 0;
            p->unk78 = 0;
            p->hookCycle = 0;

            fn_1_1078(obj);
            MgPlayerPadSet(lbl_1_bss_DC[p->unk0.playerNo], p->unk70, p->unk74.padY, p->unk78, p->hookCycle);
        }

        mp = lbl_1_bss_DC[p->unk0.playerNo];
        p->unk10 = mp->actor->colGroundAttr;
        p->unk14 = (s16)mp->actor->colMesh;
        p->unk16 = (s16)(*(s32 *)&mp->actor->vel.z >> 16);

        if (p->unk16 != 3) {
            p->unk34 = 0;
            p->unk3C = 1;
        } else if (p->unk18.y < 190.0f) {
            if (p->unk3C != 0) {
                p->unk34++;

                tileIdx = fn_1_2A14(obj, 0);

                if (tileIdx != -1) {
                    fn_1_2FE0(lbl_1_bss_1C8[tileIdx], p->unk0.playerNo);
                    fn_1_1D44(tileIdx);
                    omVibrate((s16)p->unk8, 15, 6, 6);
                    p->unk3C = 0;
                }
            }
        }
    }

    mp = lbl_1_bss_DC[p->unk0.playerNo];

    p->unk18.x = mp->actor->pos.x;
    p->unk18.y = mp->actor->pos.y;
    p->unk18.z = mp->actor->pos.z;
    p->unk24 = 0.0f;
    p->unk28.rotY = mp->actor->rotY;
    p->unk2C = 0.0f;
    p->unk48 = mp->actor->velY;

    Hu3D3Dto2D(&p->unk18, 1, &screenPos);

    CharModelVoicePanSet(p->unk4, (s16)(32.0f + (screenPos.x / 576.0f) * 64.0f));
}

void fn_1_1078(OMOBJ *obj)
{
    M571Player *p = (M571Player *)obj->data;

    switch (p->unk60) {
    case 0:
        if (fn_1_1264(&p->unk18, &p->unk64) != 0) {
            p->unk60 = 1;
        } else {
            p->unk60 = 2;
        }
        break;
    case 1:
        if (fn_1_1168(obj) != 0) {
            p->unk60 = 2;
        } else if ((p->unk10 & 0xBF80U) != 0) {
            p->unk78 |= 0x200;
        }
        break;
    case 2:
        p->unk80++;
        if (p->unk80 > p->unk84) {
            p->unk60 = 0;
            p->unk80 = 0;
        }
        break;
    }
}

// NONMATCHING: draft reconstruction
s32 fn_1_1168(OMOBJ *obj)
{
    M571Player *p = (M571Player *)obj->data;
    Vec diff;
    f32 sqMag;

    PSVECSubtract(&p->unk64, &p->unk18, &diff);
    diff.y = 0.0f;

    sqMag = PSVECSquareMag(&diff);

    if (sqMag < 3600.001f) {
        return 1;
    }

    PSVECNormalize(&diff, &diff);

    p->unk70 = (s8)(56.0f * diff.x);
    p->unk74.padY = (s8)(56.0f * -diff.z);

    return 0;
}

// NONMATCHING: draft reconstruction
s32 fn_1_1264(Vec *pos, Vec *out)
{
    f32 best = 1000000.0f;
    Vec bestPos = { 0.0f, 0.0f, 0.0f };
    f32 dist;
    s32 i;

    for (i = 0; i < 49; i++) {
        f32 dx;
        f32 dz;

        if (lbl_1_bss_1C8[i] == NULL) {
            continue;
        }

        dz = lbl_1_data_26C.pos[i].z - pos->z;
        dx = lbl_1_data_26C.pos[i].x - pos->x;
        dist = dx * dx + dz * dz;

        if (dist > 0.0f) {
            dist = 1.0f / sqrt(dist);
        }

        if (best > dist) {
            bestPos.x = lbl_1_data_26C.pos[i].x;
            bestPos.y = lbl_1_data_26C.pos[i].y;
            bestPos.z = lbl_1_data_26C.pos[i].z;
            best = dist;
        }
    }

    if (best != 1000000.0f) {
        *out = bestPos;
        return 1;
    }

    return 0;
}

// The four collected types are staged in a local, and the three locals that
// feed the last three stores are assigned in descending order: that is what
// makes MWCC hoist those loads into r27..r29 and save them with _savegpr_27.
void fn_1_1518(void)
{
    M571Quad work;
    s32 i;
    s32 v1;
    s32 v2;
    s32 v3;

    if (MgSeqFrameNoGet() == 0) {
        for (i = 0; i < 4; i++) {
            M571Player *p = (M571Player *)lbl_1_bss_354[i]->data;

            work.v[i] = p->unk4;
        }

        v3 = work.v[3];
        v2 = work.v[2];
        v1 = work.v[1];

        lbl_1_bss_43C[0] = work.v[0];
        lbl_1_bss_43C[1] = v1;
        lbl_1_bss_43C[2] = v2;
        lbl_1_bss_43C[3] = v3;

        lbl_1_bss_438 = 0;

        omAddObjEx(lbl_1_bss_680, 0x65, 0, 0, OM_GRP_NONE, fn_1_7DF8);
    }

    if (WipeCheckEnd() == 0) {
        MgSeqModeNext();
        MgActorExec();
    }

    lbl_1_bss_35C = HuAudSStreamPlay(0x3D);
}

// NONMATCHING: draft reconstruction
void fn_1_1630(void)
{
    M571Player *p;
    s32 i;

    p = (M571Player *)lbl_1_bss_478[0]->data;

    for (i = 0; i < 30; i++) {
        HuPrcVSleep();
    }

    p->unk30 = 1;

    for (i = 0; i < 150; i++) {
        HuPrcVSleep();
    }

    omCameraViewMoveSimple(&lbl_1_data_1D4, 0x5A);

    for (i = 0; i < 30; i++) {
        HuPrcVSleep();
    }

    p->unk38 = 0;
    p->unk28.rotY = 1300.0f;
    p->unk30 = 2;

    MgSeqModeNext();
}

// NONMATCHING: draft reconstruction
void fn_1_16F4(void)
{
    M571Player *p;

    p = (M571Player *)lbl_1_bss_478[0]->data;

    if (MgSeqFrameNoGet() == 0) {
        p->unk30 = 3;
    }

    fn_1_2D54();

    MgActorExec();

    if (lbl_1_bss_4 == 0) {
        MgSeqModeNext();
    }
}

// NONMATCHING: draft reconstruction
void fn_1_1760(void)
{
    s32 n;
    s32 i;
    s32 found;

    if (MgSeqFrameNoGet() == 0) {
        HuAudSStreamFadeOut(lbl_1_bss_35C, 0x64);

        lbl_1_bss_1C0 = omAddObjEx(lbl_1_bss_680, 0x64, 0, 0, OM_GRP_NONE, fn_1_1898);
    }

    if (MgSeqFrameNoGet() > 300) {
        for (i = 0; i < 20; i++) {
            if (lbl_1_bss_368[i] != NULL) {
                found = 1;
                break;
            }
        }

        if (found != 0) {
            for (n = 0; n < 4; n++) {
                M571Player *p = (M571Player *)lbl_1_bss_354[n]->data;

                Hu3DModelAttrReset(lbl_1_bss_DC[n]->actor->mdlId, 1);
            }

            MgSeqModeNext();
        }
    }
}

// NONMATCHING: register allocation and expression form differ from the target build
void fn_1_1898(OMOBJ *obj)
{
    MgActorExec();
}

// NONMATCHING: wipe/reset sequence ordering is a draft reconstruction
void fn_1_18B8(void)
{
    M571Player *p;
    M571Player *q;
    s32 i;

    p = (M571Player *)lbl_1_bss_478[0]->data;

    if (MgSeqFrameNoGet() == 30) {
        WipeCreate(2, 6, 30);
        WipeColorSet(0xFF, 0xFF, 0xFF);

        do {
            HuPrcVSleep();
        } while ((WipeCheckEnd() & 0xFFFFFF00) != 0);

        omDelObjEx(lbl_1_bss_680, lbl_1_bss_1C0);
        omCameraViewSet(&lbl_1_data_1F0);

        for (i = 0; i < 4; i++) {
            M571Player *r = (M571Player *)lbl_1_bss_354[i]->data;

            MgActorKill((MGACTOR *)lbl_1_bss_DC[i]);
            omDelObjEx(lbl_1_bss_680, lbl_1_bss_354[i]);
        }

        CharModelKill(-1);

        for (i = 0; i < 4; i++) {
            OMOBJ *obj = omAddObjEx(lbl_1_bss_680, 0x65, 1, 3, 1, fn_1_3700);

            lbl_1_bss_350[i] = obj;
            obj->data = HuMemDirectMallocNum(0, 0x34, HU_MEMNUM_OVL);
            obj->work[0] = i;
        }

        fn_1_3520();

        p->unk30 = 0;

        q = (M571Player *)lbl_1_bss_478[0]->data;

        Hu3DMotionSpeedSet(q->unk0.modelId, 1.0f);

        if (q->unk2C != 0) {
            q->unk2C = 0;
            Hu3DMotionShiftSet(q->unk0.modelId, lbl_1_bss_44C[0], 0.0f, 0.0f, 0x40000001);
        }

        Hu3DModelAttrReset(p->unk0.modelId, 1);

        Hu3DModelHookObjReset(p->hookL, lbl_1_data_11A);
        Hu3DModelHookObjReset(p->hookL, lbl_1_data_124);
        Hu3DModelHookObjReset(p->unk0.modelId, lbl_1_data_100);
        Hu3DModelHookObjReset(p->unk0.modelId, lbl_1_data_10D);

        Hu3DModelAttrSet(p->hookL, 1);
        Hu3DModelAttrSet(p->hookSwap, 1);
        Hu3DModelAttrSet(p->hookR, 1);

        HuPrcVSleep();

        p->unk4 = lbl_1_data_70.x;
        p->unk8 = lbl_1_data_70.y;
        p->unkC = lbl_1_data_70.z;
        p->unk24 = 0.0f;
        p->unk18.z = 0.0f;
        p->unk18.y = 0.0f;

        WipeCreate(1, 5, 30);

        do {
            HuPrcVSleep();
        } while ((WipeCheckEnd() & 0xFFFFFF00) != 0);

        MgSeqModeNext();
    }
}

// NONMATCHING: draft reconstruction of the scoring pass
void fn_1_1BA0(void)
{
    M571Player *p;
    s32 i;

    p = (M571Player *)lbl_1_bss_478[0]->data;

    if (MgSeqFrameNoGet() == 0) {
        p->unk30 = 6;
        p->unk38 = 0;

        for (i = 0; i < 4; i++) {
            M571Player *q = (M571Player *)lbl_1_bss_350[i]->data;
            s32 score;

            if (q->unk30 >= 1) {
                fn_1_27E8(lbl_1_bss_350[i], 1, 8.0f, 0);
            } else {
                fn_1_27E8(lbl_1_bss_350[i], 2, 8.0f, 0);

                if (lbl_1_bss_428[0] == 0 && lbl_1_bss_428[1] == 0 && lbl_1_bss_428[2] == 0
                    && lbl_1_bss_428[3] == 0) {
                    CharFXPlay((s16)q->unk28.raw, 0x200);
                }
            }

            score = (s16)(q->unk30 + (s16)GwPlayer[q->unk24].mgScore);

            if (!_CheckFlag(0x1000F)) {
                GwPlayer[q->unk24].mgScore = score;
            }
        }
    }
}

// NONMATCHING: target uses a larger stack frame and separately retains converted audio values in GPRs
void fn_1_1D44(s32 tileIdx)
{
    OMOBJ *obj;
    M571Tile *tile;
    HuVecF screenY;
    HuVecF screenX;
    s32 vol;
    s32 pan;

    if (lbl_1_bss_F0[0] <= 0x30) {
        lbl_1_bss_F0[0]++;
        obj = omAddObjEx(lbl_1_bss_680, 0x68, 1, 0, -1, fn_1_1F74);
        obj->work[0] = 0;
        tile = (M571Tile *)HuMemDirectMallocNum(0, 0x14, 0x1000);
        obj->data = tile;
        tile->pos.x = lbl_1_data_26C.pos[tileIdx].x;
        tile->pos.y = lbl_1_data_26C.pos[tileIdx].y;
        tile->pos.z = lbl_1_data_26C.pos[tileIdx].z;
        tile->state = 0;
        Hu3D3Dto2D(&tile->pos, 1, &screenY);
        vol = 96.0f + (screenY.y / 480.0f) * 31.0f;
        Hu3D3Dto2D(&tile->pos, 1, &screenX);
        pan = 32.0f + (screenX.x / 576.0f) * 64.0f;
        HuAudFXPlayVolPan(0x96B, vol, pan);
    }
}

// NONMATCHING: register allocation and expression form differ from the target build
void fn_1_1F00(OMOBJ *obj)
{
    Hu3DModelAttrSet(obj->mdlId[0], 1);
    Hu3DModelKill(obj->mdlId[0]);

    lbl_1_bss_F0[0]--;
    omDelObjEx(lbl_1_bss_680, obj);
}

// NONMATCHING: register allocation and expression form differ from the target build
void fn_1_1F74(OMOBJ *obj)
{
    M571Tile *tile = (M571Tile *)obj->data;

    obj->mdlId[0] = Hu3DModelLink(lbl_1_bss_10);
    omSetTra(obj, tile->pos.x, tile->pos.y, tile->pos.z);
    omSetRot(obj, 0.0f, 0.0f, 0.0f);
    omSetSca(obj, 1.0f, 1.0f, 1.0f);
    Hu3DModelAttrReset(obj->mdlId[0], 1);
    Hu3DMotionTimeSet(obj->mdlId[0], 0.0f);
    Hu3DMotionSpeedSet(obj->mdlId[0], 1.0f);
    Hu3DModelAttrSet(obj->mdlId[0], 0);

    obj->objFunc = fn_1_2088;
}

// NONMATCHING: register allocation and expression form differ from the target build
void fn_1_2088(OMOBJ *obj)
{
    M571Tile *tile = (M571Tile *)obj->data;

    switch (tile->state) {
    case 0:
        if (Hu3DMotionEndCheck(obj->mdlId[0])) {
            tile->timer = 0;
            tile->state = 1;
        }
        break;
    case 1:
        if (tile->timer > 30) {
            Hu3DModelAttrSet(obj->mdlId[0], 1);
            Hu3DModelKill(obj->mdlId[0]);

            lbl_1_bss_F0[0]--;
            omDelObjEx(lbl_1_bss_680, obj);
            return;
        }

        tile->timer++;

        if (tile->timer & 1) {
            Hu3DModelAttrSet(obj->mdlId[0], 1);
        } else {
            Hu3DModelAttrReset(obj->mdlId[0], 1);
        }
        break;
    }

    omSetTra(obj, tile->pos.x, tile->pos.y, tile->pos.z);
    omSetRot(obj, 0.0f, 0.0f, 0.0f);
}

// NONMATCHING: register allocation and expression form differ from the target build
void fn_1_21DC(s32 src, f32 x, f32 y, f32 z, s32 slot, s32 arg)
{
    OMOBJ *obj;
    M571Item *item;

    if (lbl_1_bss_EC <= 0x30) {
        lbl_1_bss_EC++;

        obj = omAddObjEx(lbl_1_bss_680, 0x69, 1, 0, -1, fn_1_2388);
        item = (M571Item *)HuMemDirectMallocNum(0, 0x3C, 0x1000);
        obj->data = item;
        obj->work[0] = src;

        item->pos.x = x;
        item->pos.y = y;
        item->pos.z = z;
        item->rot.x = 0.0f;
        item->rot.y = 0.0f;
        item->rot.z = 0.0f;
        item->src = src;
        item->sca.x = 0.1f;
        item->sca.y = 0.1f;
        item->sca.z = 0.1f;
        item->state = 1;
        item->timer = 0;
        item->phase = 0.0f;
        item->slot = slot;
        item->arg = arg;
    }
}

// NONMATCHING: register allocation and expression form differ from the target build
void fn_1_231C(OMOBJ *obj)
{
    void *data = obj->data;

    Hu3DModelKill(obj->mdlId[0]);

    lbl_1_bss_EC--;
    omDelObjEx(lbl_1_bss_680, obj);
}

// NONMATCHING: register allocation and expression form differ from the target build
void fn_1_2388(OMOBJ *obj)
{
    M571Item *item = (M571Item *)obj->data;

    if (obj->work[0] == 0) {
        obj->mdlId[0] = Hu3DModelLink(lbl_1_bss_E);
    } else {
        obj->mdlId[0] = Hu3DModelLink(lbl_1_bss_C);
    }

    Hu3DModelShadowSet(obj->mdlId[0]);
    omSetTra(obj, item->pos.x, item->pos.y, item->pos.z);
    omSetRot(obj, item->rot.x, item->rot.y, item->rot.z);
    omSetSca(obj, item->sca.x, item->sca.y, item->sca.z);

    lbl_1_bss_28C[item->slot] = obj;

    obj->objFunc = fn_1_2468;
}

/* The 0x18-byte block spawned below: a kind tag at 0x00 (1 for the first
 * variant, 5 for the second), a timer at 0x04, the originating tile index at
 * 0x08, and a Vec at 0x0C holding the item's position at spawn time. */
typedef struct M571Eff_s {
    s32 kind;    /* 0x00 */
    s32 timer;   /* 0x04 */
    s32 arg;     /* 0x08 */
    Vec pos;     /* 0x0C */
} M571Eff;       /* 0x18 */

void fn_1_2468(OMOBJ *obj)
{
    M571Item *item = (M571Item *) obj->data;
    M571Eff *eff;
    OMOBJ *effObj;
    f32 s;

    switch (item->state) {
    case 1:
        Hu3DModelAttrReset(obj->mdlId[0], 1);
        item->timer++;
        if (item->timer > 0x1E) {
            item->timer = 0;
            item->sca.x = 1.0f;
            item->sca.y = 1.0f;
            item->sca.z = 1.0f;
            item->state = 0;
        } else {
            s = (f32) sin((M_PI * item->phase) / 180.0);
            item->phase += 4.3333335f;
            item->sca.x = (f32) (1.25 * s);
            item->sca.y = (f32) (1.25 * s);
            item->sca.z = (f32) (1.25 * s);
            if (item->phase <= 90.0f) {
                item->pos.y += 10.0f;
            }
        }
        break;

    case 2:
        if (item->timer > 0x1E) {
            item->state = 3;
        } else {
            item->timer++;
            if (item->timer & 1) {
                Hu3DModelAttrSet(obj->mdlId[0], 1);
            } else {
                Hu3DModelAttrReset(obj->mdlId[0], 1);
            }
        }
        break;

    case 0:
        if (item->timer > 0xA) {
            item->timer = 0;
            if (item->arg != -1) {
                if (item->src == 0) {
                    effObj = omAddObjEx(lbl_1_bss_680, 0x66, 0, 0, -1, fn_1_8BA8);
                    eff = HuMemDirectMallocNum(0, sizeof(M571Eff), HU_MEMNUM_OVL);
                    effObj->data = eff;
                    eff->pos.x = item->pos.x;
                    eff->pos.y = item->pos.y;
                    eff->pos.z = item->pos.z;
                    eff->arg = item->arg;
                    eff->kind = 1;
                    eff->timer = 0;
                } else if (item->src == 1) {
                    effObj = omAddObjEx(lbl_1_bss_680, 0x66, 0, 0, -1, fn_1_8BA8);
                    eff = HuMemDirectMallocNum(0, sizeof(M571Eff), HU_MEMNUM_OVL);
                    effObj->data = eff;
                    eff->pos.x = item->pos.x;
                    eff->pos.y = item->pos.y;
                    eff->pos.z = item->pos.z;
                    eff->arg = item->arg;
                    eff->kind = 5;
                    eff->timer = 0;
                }
            }
            item->state = 3;
        } else {
            item->timer++;
        }
        break;

    case 3:
        Hu3DModelKill(obj->mdlId[0]);
        lbl_1_bss_EC--;
        omDelObjEx(lbl_1_bss_680, obj);
        return;
    }

    omSetTra(obj, item->pos.x, item->pos.y, item->pos.z);
    omSetRot(obj, item->rot.x, item->rot.y, item->rot.z);
    omSetSca(obj, item->sca.x, item->sca.y, item->sca.z);
}

// NONMATCHING: register allocation and expression form differ from the target build
void fn_1_27E8(OMOBJ *obj, s32 motNo, f32 speed, s32 attr)
{
    M571Spin *spin = (M571Spin *)obj->data;

    if (spin->curMot != motNo) {
        spin->curMot = (s16)motNo;
        CharMotionShiftSet((s16)spin->charNo, spin->motId[motNo], 0.0f, speed, attr);
    }
}

// NONMATCHING: register allocation and expression form differ from the target build
void fn_1_2870(s32 flag)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        if (flag) {
            MgPlayerAttrReset(lbl_1_bss_DC[i], 1);
        } else {
            MgPlayerAttrSet(lbl_1_bss_DC[i], 1);
        }
    }
}

s32 fn_1_28F4(HuVecF *pos)
{
    s32 i;
    s32 flag;
    M571Pos tmp;
    Vec p;

    for (i = 0; i < 0x31; i++) {
        if (lbl_1_bss_1C8[i] == NULL) {
            continue;
        }

        tmp.x = lbl_1_data_26C.pos[i].x;
        tmp.y = lbl_1_data_26C.pos[i].y;
        tmp.z = lbl_1_data_26C.pos[i].z;

        p.x = pos->x;
        p.y = pos->y;
        p.z = pos->z;

        if ((p.x - tmp.x) * (p.x - tmp.x) + (p.z - tmp.z) * (p.z - tmp.z) < 8100.0f) {
            flag = 1;
        } else {
            flag = 0;
        }

        if (flag) {
            return i;
        }
    }

    return -1;
}

// NONMATCHING: geometry is reconstructed; local/register layout and stack frame differ from target
s32 fn_1_2A14(OMOBJ *obj, s32 useAltOffset)
{
    M571Player *player = (M571Player *)obj->data;
    Vec pos;
    Vec cellPos;
    s32 i;
    s32 result;

    if (useAltOffset == 0) {
        pos.x = (f32)(player->unk34 * sin(3.141592653589793 * player->unk28.rotY / 180.0) * 3.5);
        pos.y = 0.0f;
        pos.z = (f32)(player->unk34 * cos(3.141592653589793 * player->unk28.rotY / 180.0) * 3.5);
    } else {
        pos.x = (f32)(player->unk38 * sin(3.141592653589793 * player->unk28.rotY / 180.0) * 3.5);
        pos.y = 0.0f;
        pos.z = (f32)(player->unk38 * cos(3.141592653589793 * player->unk28.rotY / 180.0) * 3.5);
    }

    pos.x += player->unk18.x;
    pos.z += player->unk18.z;

    for (i = 0; i < 0x31; i++) {
        if (lbl_1_bss_1C8[i] == NULL) {
            continue;
        }

        cellPos.x = lbl_1_data_26C.pos[i].x;
        cellPos.y = lbl_1_data_26C.pos[i].y;
        cellPos.z = lbl_1_data_26C.pos[i].z;

        if ((pos.x - cellPos.x) * (pos.x - cellPos.x) + (pos.z - cellPos.z) * (pos.z - cellPos.z) < 8100.0f) {
            result = i;
            goto found;
        }
    }

    result = -1;
found:
    return result;
}

// NONMATCHING: register allocation and expression form differ from the target build
void fn_1_2D54(void)
{
}

// NONMATCHING: local index/register form differs from the target build
void fn_1_2D58(void)
{
    s32 i;

    omMakeGroupEx(lbl_1_bss_680, 2, 0x31);

    lbl_1_bss_14[0] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 5), HU_MEMNUM_OVL, 2));
    Hu3DModelAttrSet(lbl_1_bss_14[0], 1);

    lbl_1_bss_1C8[0] = (OMOBJ *)omGetGroupMemberListEx(lbl_1_bss_680, 2);

    lbl_1_bss_4 = 0;

    for (i = 0; i < 0x31; i++) {
        lbl_1_bss_1C8[i] = NULL;
    }
}

// NONMATCHING: local index/register form differs from the target build
void fn_1_2E28(void)
{
    s32 map;
    s32 i;

    map = frandmod(8);
    lbl_1_bss_8 = map;

    for (i = 0; i < 0x31; i++) {
        if (lbl_1_data_704[map][i] == -1) {
            continue;
        }

        fn_1_2F04(lbl_1_data_704[map][i], lbl_1_data_26C.pos[i].x, lbl_1_data_26C.pos[i].y,
            lbl_1_data_26C.pos[i].z, i);
    }
}

// NONMATCHING: local index/register form differs from the target build
void fn_1_2F04(s32 type, f32 x, f32 y, f32 z, s32 idx)
{
    OMOBJ *obj;
    M571Tile *tile;

    if (lbl_1_bss_4 > 0x30) {
        return;
    }

    obj = omAddObjEx(lbl_1_bss_680, 0x67, 1, 0, 2, fn_1_3594);

    tile = HuMemDirectMallocNum(0, 0x10, 0x1000);
    obj->data = tile;
    obj->work[0] = type;

    tile->pos.x = x;
    tile->pos.y = y;
    tile->pos.z = z;
    tile->state = idx;

    lbl_1_bss_4++;
}

// NONMATCHING: register allocation and expression form differ from the target build
s32 fn_1_2FE0(OMOBJ *obj, s32 arg)
{
    M571Tile *tile = (M571Tile *)obj->data;
    OMOBJ *fx;
    M571Item *item;
    HuVecF pos;
    HuVecF scr;
    s32 type;

    pos.x = lbl_1_data_26C.pos[tile->state].x;
    pos.y = lbl_1_data_26C.pos[tile->state].y;
    pos.z = lbl_1_data_26C.pos[tile->state].z;

    type = lbl_1_data_704[lbl_1_bss_8][tile->state];

    Hu3DModelKill(obj->mdlId[0]);
    Hu3DModelPosSet(lbl_1_bss_156[tile->state + 2], pos.x, m571ModelY, pos.z);

    if (arg != -1 && (type == 1 || type == 5)) {
        if (lbl_1_bss_EC <= 0x30) {
            lbl_1_bss_EC++;

            fx = omAddObjEx(lbl_1_bss_680, 0x69, 1, 0, -1, fn_1_2388);
            item = HuMemDirectMallocNum(0, 0x3C, 0x1000);
            fx->data = item;
            fx->work[0] = (type == 1) ? 0 : 1;

            item->pos.x = pos.x;
            item->pos.y = pos.y;
            item->pos.z = pos.z;
            item->rot.x = m571Zero;
            item->rot.y = m571Zero;
            item->rot.z = m571Zero;
            item->sca.x = m571Sca;
            item->sca.y = m571Sca;
            item->sca.z = m571Sca;
            item->src = (type == 1) ? 0 : 1;
            item->slot = tile->state;
            item->state = 1;
            item->timer = 0;
            item->phase = m571Zero;
            item->arg = arg;
        }

        Hu3D3Dto2D(&pos, 1, &scr);

        HuAudFXPlayVolPan(0x96C, (s16)(m571VolBias + m571VolScale * (scr.y / m571ScreenYDiv)),
            (s16)(m571PanBias + m571PanScale * (scr.x / m571ScreenXDiv)));
    }

    lbl_1_bss_1C8[tile->state] = NULL;
    lbl_1_bss_4--;

    omDelObjEx(lbl_1_bss_680, obj);

    return type;
}

// NONMATCHING: local index/register form differs from the target build
void fn_1_3520(void)
{
    s32 i;

    for (i = 0; i < 0x31; i++) {
        if (lbl_1_bss_1C8[i] == NULL) {
            continue;
        }

        fn_1_2FE0(lbl_1_bss_1C8[i], -1);
    }
}

void fn_1_3594(OMOBJ *obj)
{
    M571Tile *tile = (M571Tile *)obj->data;

    obj->mdlId[0] = Hu3DModelLink(lbl_1_bss_14[0]);
    Hu3DModelShadowMapSet(obj->mdlId[0]);
    omSetTra(obj, tile->pos.x, tile->pos.y, tile->pos.z);
    omSetRot(obj, 0.0f, (float)(frand() & 0xFF) * 1.4117647f, 0.0f);
    omSetSca(obj, 1.0f, 1.0f, 1.0f);

    lbl_1_bss_1C8[tile->state] = obj;

    Hu3DModelAttrReset(obj->mdlId[0], 1);
    obj->objFunc = fn_1_36B8;
}

void fn_1_36B8(OMOBJ *obj)
{
    M571Pos *pos = (M571Pos *)obj->data;

    omSetTra(obj, pos->x, pos->y, pos->z);
}

// NONMATCHING: register allocation and expression form differ from the target build
void fn_1_3700(OMOBJ *obj)
{
    M571Spin *spin = (M571Spin *)obj->data;
    HuVecF scr;
    s32 i;

    spin->playerNo = obj->work[0];
    spin->charNo = GwPlayerConf[spin->playerNo].padNo;
    spin->score = lbl_1_bss_428[spin->playerNo];

    obj->mdlId[0] = CharModelCreate((s16)spin->charNo, 2);

    for (i = 0; i < 3; i++) {
        spin->motId[i] = CharMotionCreate((s16)spin->charNo, lbl_1_data_1A4[i]);
    }

    CharMotionSet((s16)spin->charNo, spin->motId[0]);

    spin->curMot = 3;
    spin->modelId = obj->mdlId[0];

    spin->pos.x = lbl_1_data_23C[spin->playerNo].x;
    spin->pos.y = lbl_1_data_23C[spin->playerNo].y;
    spin->pos.z = lbl_1_data_23C[spin->playerNo].z;

    spin->rot.x = m571Zero;
    spin->rot.y = m571Zero;
    spin->rot.z = m571Zero;

    Hu3DModelAttrSet(spin->modelId, 0x40001);
    Hu3DModelShadowSet(spin->modelId);

    Hu3D3Dto2D(&spin->pos, 1, &scr);

    CharModelVoicePanSet((s16)spin->charNo,
        (s16)(m571PanBias + m571PanScale * (scr.x / m571ScreenXDiv)));

    obj->objFunc = fn_1_38E8;
}

// NONMATCHING: register allocation differs from the target build
void fn_1_38E8(OMOBJ *obj)
{
    M571Spin *spin = (M571Spin *)obj->data;

    omSetTra(obj, spin->pos.x, spin->pos.y, spin->pos.z);
    omSetRot(obj, spin->rot.x, spin->rot.y, spin->rot.z);
}

// Debug camera controls, rebuilt from the pad state.
void fn_1_3944(void)
{
    GXColor color;

    color.r = 0;
    color.g = 0;
    color.b = 0x90;
    color.a = 0xC0;

    CZoom += (f32)(HuPadTrigL[0] / 5);
    CZoom -= (f32)(HuPadTrigR[0] / 5);

    Center.x += (f32)HuPadSubStkX[0] / 59.0f * 10.0f;
    Center.z -= (f32)HuPadSubStkY[0] / 59.0f * 10.0f;

    CRot.x += (f32)HuPadSubStkY[1] / 59.0f * 5.0f;
    CRot.y -= (f32)HuPadSubStkX[1] / 59.0f * 5.0f;

    printWin(0x10, 0x28, 0x144, 0x54, &color);
    fontcolor = 0xE;

    print8(0x10, 0x28, 1.5f, lbl_1_data_DA2, Center.x);
    print8(0x10, 0x34, 1.5f, lbl_1_data_DB1, Center.y);
    print8(0x10, 0x40, 1.5f, lbl_1_data_DC0, Center.z);
    print8(0x10, 0x4C, 1.5f, lbl_1_data_DCF, CRot.x);
    print8(0x10, 0x58, 1.5f, lbl_1_data_DDC, CRot.y);
    print8(0x10, 0x64, 1.5f, lbl_1_data_DDC, CRot.z);
    print8(0x10, 0x70, 1.5f, lbl_1_data_DE9, CZoom);

    if (HuPadBtnDown[0] & 0x400) {
        omCameraViewSet(&lbl_1_data_1D4);
    }
}

// NONMATCHING: register allocation and expression form differ from the target build
void fn_1_3D94(void)
{
    lbl_1_bss_684[1] = 0;
    lbl_1_bss_684[2] = 0;
}

// NONMATCHING: register allocation differs from the target build
void fn_1_3DB8(u16 arg0, s16 arg1)
{
    M571ListNode *node;
    M571ListNode *sentinel = (M571ListNode *)&lbl_1_bss_684[0];
    M571ListNode *old;

    node = HuMemDirectMallocNum(0, 0xC, 0x1000);
    node->id = arg0;
    node->pad02 = arg1;

    old = sentinel->next;
    sentinel->next = node;

    if (old != NULL) {
        old->prev = node;
    }

    node->prev = sentinel;
    node->next = old;
}

// NONMATCHING: register allocation and expression form differ from the target build
s32 fn_1_3E40(s16 id)
{
    M571ListNode *node;

    node = (M571ListNode *) (uintptr_t) lbl_1_bss_684[2];

    while (node != NULL) {
        if (id == node->id) {
            M571ListNode *prev = node->prev;
            M571ListNode *next = node->next;

            prev->next = next;

            if (next != NULL) {
                next->prev = prev;
            }

            HuMemDirectFree(node);
            return node->id;
        }

        node = node->next;
    }

    return -1;
}

// NONMATCHING: local index/register form differs from the target build
void fn_1_3EE4(void)
{
    s32 i;

    for (i = 0; i < 0x20; i++) {
        lbl_1_bss_47C[i].val = 0;
        lbl_1_bss_47C[i].flag = -1;
    }
}

HU3DMODELID fn_1_3F3C(u32 playerNo)
{
    s32 i;

    for (i = 0; i < 0x20; i++) {
        if (lbl_1_bss_47C[i].val == playerNo) {
            return Hu3DModelLink(lbl_1_bss_47C[i].flag);
        }
    }

    for (i = 0; i < 0x20; i++) {
        if (lbl_1_bss_47C[i].val == 0) {
            lbl_1_bss_47C[i].val = playerNo;
            lbl_1_bss_47C[i].flag = Hu3DModelCreate(HuDataSelHeapReadNum(playerNo, HU_MEMNUM_OVL, 2));
            return lbl_1_bss_47C[i].flag;
        }
    }

    return Hu3DModelCreate(HuDataSelHeapReadNum(playerNo, HU_MEMNUM_OVL, 2));
}

void fn_1_4050(f32 t, HuVecF *to, HuVecF *from, HuVecF *out)
{
    f32 k;

    if (1.0f < t) {
        t = 1.0f;
    }

    k = sin(3.141592653589793 * (90.0f * t) / 180.0) * sin(3.141592653589793 * (90.0f * t) / 180.0);

    out->x = to->x + k * (from->x - to->x);
    out->y = to->y + k * (from->y - to->y);
    out->z = to->z + k * (from->z - to->z);
}

void fn_1_41AC(Vec *pos, Vec *up, Vec *target)
{
    Hu3DCameraPosSetV(1, pos, up, target);

    lbl_1_bss_674 = *pos;
    lbl_1_bss_65C = *target;
}

// Two copies of the easing helper as inlined by the original build; the float
// parameter is declared first because it lands in f1 either way, and that order
// is what makes MWCC emit the FP save before the pointer saves.
s32 fn_1_4238(f32 t, Vec *up)
{
    f32 kA;
    f32 kB;
    f32 wA;
    f32 wB;
    Vec pos;
    Vec tgt;

    kA = t;
    if (1.0f < kA) {
        kA = 1.0f;
    }
    wA = sin(3.141592653589793 * (90.0f * kA) / 180.0) * sin(3.141592653589793 * (90.0f * kA) / 180.0);

    pos.x = lbl_1_bss_674.x + wA * (lbl_1_bss_668.x - lbl_1_bss_674.x);
    pos.y = lbl_1_bss_674.y + wA * (lbl_1_bss_668.y - lbl_1_bss_674.y);
    pos.z = lbl_1_bss_674.z + wA * (lbl_1_bss_668.z - lbl_1_bss_674.z);

    kB = t;
    if (1.0f < kB) {
        kB = 1.0f;
    }
    wB = sin(3.141592653589793 * (90.0f * kB) / 180.0) * sin(3.141592653589793 * (90.0f * kB) / 180.0);

    tgt.x = lbl_1_bss_65C.x + wB * (lbl_1_bss_650.x - lbl_1_bss_65C.x);
    tgt.y = lbl_1_bss_65C.y + wB * (lbl_1_bss_650.y - lbl_1_bss_65C.y);
    tgt.z = lbl_1_bss_65C.z + wB * (lbl_1_bss_650.z - lbl_1_bss_65C.z);

    Hu3DCameraPosSetV(1, &pos, up, &tgt);

    if (1.0f <= t) {
        return 1;
    }

    lbl_1_bss_580 = 1;
    return 0;
}

s32 fn_1_456C(f32 t, Vec *up)
{
    f32 kA;
    f32 kB;
    f32 wA;
    f32 wB;
    Vec pos;
    Vec tgt;
    s32 i;

    for (i = 0; i < 4; i++) {
        kA = t;
        if (1.0f < kA) {
            kA = 1.0f;
        }
        wA = sin(3.141592653589793 * (90.0f * kA) / 180.0) * sin(3.141592653589793 * (90.0f * kA) / 180.0);

        pos.x = lbl_1_bss_620[i].x + wA * (lbl_1_bss_5F0[i].x - lbl_1_bss_620[i].x);
        pos.y = lbl_1_bss_620[i].y + wA * (lbl_1_bss_5F0[i].y - lbl_1_bss_620[i].y);
        pos.z = lbl_1_bss_620[i].z + wA * (lbl_1_bss_5F0[i].z - lbl_1_bss_620[i].z);

        kB = t;
        if (1.0f < kB) {
            kB = 1.0f;
        }
        wB = sin(3.141592653589793 * (90.0f * kB) / 180.0) * sin(3.141592653589793 * (90.0f * kB) / 180.0);

        tgt.x = lbl_1_bss_5C0[i].x + wB * (lbl_1_bss_590[i].x - lbl_1_bss_5C0[i].x);
        tgt.y = lbl_1_bss_5C0[i].y + wB * (lbl_1_bss_590[i].y - lbl_1_bss_5C0[i].y);
        tgt.z = lbl_1_bss_5C0[i].z + wB * (lbl_1_bss_590[i].z - lbl_1_bss_5C0[i].z);

        Hu3DCameraPosSetV(1 << i, &pos, up, &tgt);
    }

    if (1.0f <= t) {
        return 1;
    }

    lbl_1_bss_580 = 1;
    return 0;
}

// NONMATCHING: register allocation and expression form differ from the target build
s32 fn_1_4B18(s32 base, s32 range)
{
    return base + frandmod(range - base);
}

f32 fn_1_4950(f32 arg1, f32 arg2, f32 arg3)
{
    f32 delta = (f32)fmod(arg2 - arg1, m571Turn);

    if (m571Zero > delta) {
        delta += m571TurnF;
    }

    if (delta > m571HalfTurn) {
        delta -= m571TurnF;
    }

    return (f32)fmod(arg1 + delta * arg3, m571Turn);
}

// NONMATCHING: local register form differs from the target build
void fn_1_4A54(s32 fadeSpeed)
{
    if (lbl_1_bss_584[0] >= 0) {
        if (fadeSpeed < 0) {
            fadeSpeed = 1000;
        }

        HuAudSeqFadeOut(lbl_1_bss_584[0], fadeSpeed);
    }

    lbl_1_bss_584[0] = -1;
}

// NONMATCHING: local index/register form differs from the target build
void fn_1_4AC0(s16 modelId, s32 arg1, Vec *out)
{
    Mtx mtx;

    Hu3DModelObjMtxGet(modelId, (char *) (uintptr_t) arg1, mtx);

    out->x = mtx[0][3];
    out->y = mtx[1][3];
    out->z = mtx[2][3];
}

s32 fn_1_4B54(Vec *arg0, Vec *arg1, f32 arg2, f32 arg3)
{
    if ((arg1->x - arg0->x) * (arg1->x - arg0->x) + (arg1->z - arg0->z) * (arg1->z - arg0->z)
        < (arg2 + arg3) * (arg2 + arg3)) {
        return 1;
    } else {
        return 0;
    }
}

// NONMATCHING: the inline integer-to-double conversions differ from the target build
s32 fn_1_4BB4(HuVecF *pos, s32 t0, s32 t1)
{
    HuVecF scr;

    Hu3D3Dto2D(pos, 1, &scr);

    return (s32)((scr.x / m571ScreenXDiv) * (t1 - t0) + t0);
}

// NONMATCHING: the inline integer-to-double conversions differ from the target build
s32 fn_1_4C78(HuVecF *pos, s32 t0, s32 t1)
{
    HuVecF scr;

    Hu3D3Dto2D(pos, 1, &scr);

    return (s32)((scr.y / m571ScreenYDiv) * (t1 - t0) + t0);
}

// NONMATCHING: local register form differs from the target build
void fn_1_4D3C(OMOBJ *obj, s32 motNo, f32 start, u32 attr)
{
    M571Spin *spin = (M571Spin *)obj->data;

    if (motNo == 8) {
        Hu3DMotionSpeedSet(spin->curMot, 2.0f);
    } else {
        Hu3DMotionSpeedSet(spin->curMot, 1.0f);
    }

    if (spin->pad2C == motNo) {
        return;
    }

    if (motNo == 2 || motNo == 0xD) {
        HuAudFXPlay(0x2DB);
    } else if (motNo == 0xE) {
        HuAudFXPlay(0x2DD);
    }

    spin->pad2C = motNo;
    Hu3DMotionShiftSet(spin->curMot, lbl_1_bss_44C[motNo], m571Zero, start, attr);

    if (motNo == 0xE) {
        HuAudJinglePlay(0x7A);
    }

    if (motNo == 0xD) {
        HuAudJinglePlay(0x79);
    }
}

// NONMATCHING: local register form differs from the target build
void fn_1_4E44(OMOBJ *obj, s32 motNo, f32 start, u32 attr)
{
    M571Spin *spin = (M571Spin *)obj->data;

    spin->pad2C = motNo;
    Hu3DMotionShiftSet(spin->curMot, lbl_1_bss_44C[motNo], m571Zero, start, attr);
}

// NONMATCHING: register allocation differs from the target build
void fn_1_4EC0(OMOBJ *obj, s32 motNo)
{
    M571Spin *spin = (M571Spin *)obj->data;

    spin->pad2C = motNo;
    Hu3DMotionSet(spin->curMot, lbl_1_bss_44C[motNo]);
}

// NONMATCHING: local register form and constant load differ from the target build
void fn_1_4F20(OMOBJ *obj)
{
    M571Player *p = (M571Player *)obj->data;

    if (p->hookCycle % 3 == 0) {
        Hu3DModelHookObjReset(p->unk0.modelId, lbl_1_data_100);
        Hu3DModelHookSet(p->unk0.modelId, lbl_1_data_10D, p->hookR);
        Hu3DModelAttrReset(p->hookR, 1);
        Hu3DModelAttrSet(p->hookSwap, 1);
    } else if (p->hookCycle % 3 == 1) {
        Hu3DModelHookObjReset(p->unk0.modelId, lbl_1_data_10D);
        Hu3DModelHookSet(p->unk0.modelId, lbl_1_data_100, p->hookSwap);
        Hu3DModelAttrReset(p->hookSwap, 1);
        Hu3DModelAttrSet(p->hookSwap, HU3D_MOTATTR_PAUSE);
        Hu3DMotionTimeSet(p->hookSwap, m571Zero);
        Hu3DModelAttrSet(p->hookR, 1);
    } else if (p->hookCycle % 3 == 2) {
        Hu3DModelHookObjReset(p->unk0.modelId, lbl_1_data_10D);
        Hu3DModelHookObjReset(p->unk0.modelId, lbl_1_data_100);
        Hu3DModelAttrSet(p->hookR, 1);
        Hu3DModelAttrSet(p->hookSwap, 1);
    }
}

void fn_1_50A8(OMOBJ *obj)
{
    M571Player *p = (M571Player *)obj->data;

    if (p->hookCycle % 3 == 0) {
        p->unk18.z = 90.0f;
        Hu3DModelHookObjReset(p->hookL, lbl_1_data_11A);
        Hu3DModelHookSet(p->hookL, lbl_1_data_124, p->unk0.modelId);
        p->unk74.hookAngle = 180.0f;
    } else if (p->hookCycle % 3 == 1) {
        p->unk18.z = 270.0f;
        Hu3DModelHookObjReset(p->hookL, lbl_1_data_124);
        Hu3DModelHookSet(p->hookL, lbl_1_data_11A, p->unk0.modelId);
        p->unk74.hookAngle = 0.0f;
    } else if (p->hookCycle % 3 == 2) {
        p->unk18.z = 90.0f;
        Hu3DModelHookObjReset(p->hookL, lbl_1_data_11A);
        Hu3DModelHookSet(p->hookL, lbl_1_data_124, p->unk0.modelId);
        p->unk74.hookAngle = 180.0f;
    }
}

// NONMATCHING: 29-state presentation machine, reconstructed from the target;
// the inlined hook-swap and motion-shift copies still allocate registers
// differently from MWCC.
void fn_1_5228(OMOBJ *obj)
{
    M571Hook *p = (M571Hook *)obj->data;

    switch (p->state) {
    case 1:
        Hu3DMotionSpeedSet(p->modelId, 1.0f);
        if (p->motionNo != 2) {
            HuAudFXPlay(0x2DB);
            p->motionNo = 2;
            Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[2], 0.0f, 8.0f, 0);
        }
        break;

    case 2:
        if (p->timer == 0) {
            Hu3DMotionSpeedSet(p->modelId, 1.0f);
            if (p->motionNo != 3) {
                p->motionNo = 3;
                Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[3], 0.0f, 8.0f, 0);
            }
            HuAudFXPlay(0x969);
        } else if (p->timer > 5) {
            if (p->timer > 0x1E) {
                p->state = 0;
                Hu3DMotionSpeedSet(p->modelId, 1.0f);
                if (p->motionNo != 0) {
                    p->motionNo = 0;
                    Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[0], 0.0f, 8.0f, 0x40000001);
                }
                Hu3DModelAttrSet(p->modelId, 1);
            } else {
                p->posY = (f32)((f64)p->baseY + (300.0 * sin((3.141592653589793 * (f64)p->angle) / 180.0)));
                p->angle = p->angle + 3.6f;
            }
        }
        p->timer++;
        break;

    case 3:
        p->rotY = 90.0f;
        p->posZ = 0.0f;
        p->posY = 0.0f;
        p->posX = 0.0f;
        p->cycle = 0;

        if (p->cycle % 3 == 0) {
            p->rotY = 90.0f;
            Hu3DModelHookObjReset(p->hookL, lbl_1_data_11A);
            Hu3DModelHookSet(p->hookL, lbl_1_data_124, p->modelId);
            p->hookRotY = 180.0f;
        } else if (p->cycle % 3 == 1) {
            p->rotY = 270.0f;
            Hu3DModelHookObjReset(p->hookL, lbl_1_data_124);
            Hu3DModelHookSet(p->hookL, lbl_1_data_11A, p->modelId);
            p->hookRotY = 0.0f;
        } else if (p->cycle % 3 == 2) {
            p->rotY = 90.0f;
            Hu3DModelHookObjReset(p->hookL, lbl_1_data_11A);
            Hu3DModelHookSet(p->hookL, lbl_1_data_124, p->modelId);
            p->hookRotY = 180.0f;
        }

        if (p->cycle % 3 == 0) {
            Hu3DModelHookObjReset(p->modelId, lbl_1_data_100);
            Hu3DModelHookSet(p->modelId, lbl_1_data_10D, p->hookR);
            Hu3DModelAttrReset(p->hookR, 1);
            Hu3DModelAttrSet(p->hookSwap, 1);
        } else if (p->cycle % 3 == 1) {
            Hu3DModelHookObjReset(p->modelId, lbl_1_data_10D);
            Hu3DModelHookSet(p->modelId, lbl_1_data_100, p->hookSwap);
            Hu3DModelAttrReset(p->hookSwap, 1);
            Hu3DModelAttrSet(p->hookSwap, 0x40000002);
            Hu3DMotionTimeSet(p->hookSwap, 0.0f);
            Hu3DModelAttrSet(p->hookR, 1);
        } else if (p->cycle % 3 == 2) {
            Hu3DModelHookObjReset(p->modelId, lbl_1_data_10D);
            Hu3DModelHookObjReset(p->modelId, lbl_1_data_100);
            Hu3DModelAttrSet(p->hookR, 1);
            Hu3DModelAttrSet(p->hookSwap, 1);
        }

        {
            s32 motNo = (p->cycle % 3) + 5;

            if (motNo == 8) {
                Hu3DMotionSpeedSet(p->modelId, 2.0f);
            } else {
                Hu3DMotionSpeedSet(p->modelId, 1.0f);
            }

            if (p->motionNo != motNo) {
                switch (motNo) {
                case 2:
                case 13:
                    HuAudFXPlay(0x2DB);
                    break;
                case 14:
                    HuAudFXPlay(0x2DD);
                    break;
                }
                p->motionNo = motNo;
                Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[motNo], 0.0f, 0.0f, 0x40000002);
                if (motNo == 0xE) {
                    HuAudJinglePlay(0x7A);
                }
                if (motNo == 0xD) {
                    HuAudJinglePlay(0x79);
                }
            }
        }

        Hu3DMotionSpeedSet(p->modelId, 0.0f);
        p->hookRotZOff = -60.0f;
        p->hookRotZ = 0.0f;
        p->hookPhase = -90.0f;
        p->hookRotX = 0.0f;
        p->hookRotY = 180.0f;
        Hu3DModelRotSet(p->hookL, p->hookRotX, p->hookRotY, p->hookRotZ + p->hookRotZOff);
        Hu3DModelAttrReset(p->modelId, 1);
        Hu3DModelAttrReset(p->hookL, 1);
        p->state = 4;
        break;

    case 4:
        p->hookRotZOff = (f32)((90.0 * sin((3.141592653589793 * (f64)p->hookPhase) / 180.0)) / 1.5);
        p->hookPhase = p->hookPhase + 1.0f;
        p->unk60 = p->unk60 + 1;
        Hu3DModelRotSet(p->hookL, p->hookRotX, p->hookRotY, p->hookRotZ + p->hookRotZOff);
        if (p->unk60 > 0xB4) {
            p->state = 5;
            p->timer = 0;
        } else {
            if (p->unk60 == 0x1E) {
                Hu3DModelAttrReset(p->modelId, 0x40000002);
                Hu3DMotionSpeedSet(p->modelId, 1.0f);
                if (p->cycle % 3 == 1) {
                    Hu3DModelAttrReset(p->hookSwap, 0x40000002);
                    Hu3DModelAttrSet(p->hookSwap, 0);
                }
            }
            if (p->cycle == 1 && p->unk60 == 0x66) {
                HuAudFXPlay(0x96A);
            }
        }
        break;

    case 5:
        if (p->timer > 0x1E) {
            p->state = 4;
            p->timer = 0;
            p->unk60 = 0;
            p->cycle = p->cycle + 1;

            if (p->cycle % 3 == 0) {
                p->rotY = 90.0f;
                Hu3DModelHookObjReset(p->hookL, lbl_1_data_11A);
                Hu3DModelHookSet(p->hookL, lbl_1_data_124, p->modelId);
                p->hookRotY = 180.0f;
            } else if (p->cycle % 3 == 1) {
                p->rotY = 270.0f;
                Hu3DModelHookObjReset(p->hookL, lbl_1_data_124);
                Hu3DModelHookSet(p->hookL, lbl_1_data_11A, p->modelId);
                p->hookRotY = 0.0f;
            } else if (p->cycle % 3 == 2) {
                p->rotY = 90.0f;
                Hu3DModelHookObjReset(p->hookL, lbl_1_data_11A);
                Hu3DModelHookSet(p->hookL, lbl_1_data_124, p->modelId);
                p->hookRotY = 180.0f;
            }

            if (p->cycle % 3 == 0) {
                Hu3DModelHookObjReset(p->modelId, lbl_1_data_100);
                Hu3DModelHookSet(p->modelId, lbl_1_data_10D, p->hookR);
                Hu3DModelAttrReset(p->hookR, 1);
                Hu3DModelAttrSet(p->hookSwap, 1);
            } else if (p->cycle % 3 == 1) {
                Hu3DModelHookObjReset(p->modelId, lbl_1_data_10D);
                Hu3DModelHookSet(p->modelId, lbl_1_data_100, p->hookSwap);
                Hu3DModelAttrReset(p->hookSwap, 1);
                Hu3DModelAttrSet(p->hookSwap, 0x40000002);
                Hu3DMotionTimeSet(p->hookSwap, 0.0f);
                Hu3DModelAttrSet(p->hookR, 1);
            } else if (p->cycle % 3 == 2) {
                Hu3DModelHookObjReset(p->modelId, lbl_1_data_10D);
                Hu3DModelHookObjReset(p->modelId, lbl_1_data_100);
                Hu3DModelAttrSet(p->hookR, 1);
                Hu3DModelAttrSet(p->hookSwap, 1);
            }

            {
                s32 motNo = (p->cycle % 3) + 5;

                if (motNo == 8) {
                    Hu3DMotionSpeedSet(p->modelId, 2.0f);
                } else {
                    Hu3DMotionSpeedSet(p->modelId, 1.0f);
                }

                if (p->motionNo != motNo) {
                    switch (motNo) {
                    case 2:
                    case 13:
                        HuAudFXPlay(0x2DB);
                        break;
                    case 14:
                        HuAudFXPlay(0x2DD);
                        break;
                    }
                    p->motionNo = motNo;
                    Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[motNo], 0.0f, 0.0f, 0x40000002);
                    if (motNo == 0xE) {
                        HuAudJinglePlay(0x7A);
                    }
                    if (motNo == 0xD) {
                        HuAudJinglePlay(0x79);
                    }
                }
            }

            Hu3DMotionSpeedSet(p->modelId, 0.0f);
            if (p->cycle >= 3) {
                p->state = 0;
            }
        } else {
            p->timer++;
        }
        break;

    case 6:
        if (p->timer == 0) {
            if (fn_1_7A78() == 0) {
                Hu3DMotionSpeedSet(p->modelId, 1.0f);
                if (p->motionNo != 0xE) {
                    HuAudFXPlay(0x2DD);
                    p->motionNo = 0xE;
                    Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[0xE], 0.0f, 8.0f, 0);
                    HuAudJinglePlay(0x7A);
                }
            } else {
                Hu3DMotionSpeedSet(p->modelId, 1.0f);
                if (p->motionNo != 0xD) {
                    HuAudFXPlay(0x2DB);
                    p->motionNo = 0xD;
                    Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[0xD], 0.0f, 8.0f, 0);
                    HuAudJinglePlay(0x79);
                }
            }
        }
        break;

    case 7:
        p->startX = p->posX;
        p->startY = p->posY;
        p->startZ = p->posZ;
        p->stepX = (f32)((p->tgtX - p->startX) / (f32)p->duration);
        p->stepY = (f32)((p->tgtY - p->startY) / (f32)p->duration);
        p->stepZ = (f32)((p->tgtZ - p->startZ) / (f32)p->duration);
        p->rotY = (f32)(180.0 * (atan2((f64)(p->tgtX - p->posX), (f64)(p->tgtZ - p->posZ)) / 3.141592653589793));
        p->state = 8;
        Hu3DMotionSpeedSet(p->modelId, 1.0f);
        if (p->motionNo != 1) {
            p->motionNo = 1;
            Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[1], 0.0f, 8.0f, 0x40000001);
        }
        break;

    case 8:
        if (p->timer > p->duration) {
            if (p->returnState != -1) {
                p->state = p->returnState;
                p->returnState = -1;
                p->timer = 0;
            } else {
                p->state = 0;
            }
        } else {
            p->timer++;
            p->posX = p->posX + p->stepX;
            p->posY = p->posY + p->stepY;
            p->posZ = p->posZ + p->stepZ;
        }
        break;

    case 9:
        p->angle = 0.0f;
        p->baseY = p->posY;
        p->state = 0xA;
        break;

    case 10:
        if (p->timer > p->duration2) {
            if (p->holdTime > 0) {
                p->state = 0xB;
                p->timer = 0;
            } else if (p->returnState != -1) {
                p->state = p->returnState;
                p->returnState = -1;
                p->timer = 0;
            } else {
                p->state = 0;
            }
            Hu3DMotionSpeedSet(p->modelId, 1.0f);
            if (p->motionNo != 0) {
                p->motionNo = 0;
                Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[0], 0.0f, 8.0f, 0x40000001);
            }
        } else {
            Hu3DMotionSpeedSet(p->modelId, 1.0f);
            if (p->motionNo != 3) {
                p->motionNo = 3;
                Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[3], 0.0f, 8.0f, 0);
            }
            p->posY = (f32)((f64)p->baseY + (100.0 * sin((3.141592653589793 * (f64)p->angle) / 180.0)));
            p->posX = (f32)((f64)p->posX + (((f64)p->speed * sin((3.141592653589793 * (f64)p->rotY) / 180.0)) / (f64)p->duration2));
            p->posZ = (f32)((f64)p->posZ + (((f64)p->speed * cos((3.141592653589793 * (f64)p->rotY) / 180.0)) / (f64)p->duration2));
            p->timer++;
            p->angle = (f32)(p->angle + (180.0f / (f32)p->duration2));
        }
        break;

    case 11:
        if (p->timer > p->holdTime) {
            if (p->returnState != -1) {
                p->state = p->returnState;
                p->returnState = -1;
                p->timer = 0;
            } else {
                p->state = 0;
            }
        } else {
            p->timer++;
            p->posY = (f32)((f64)p->baseY + (100.0 * sin((3.141592653589793 * (f64)(p->angle + (f32)p->timer)) / 180.0)));
        }
        break;

    case 12:
        Hu3DMotionSpeedSet(p->modelId, 1.0f);
        if (p->motionNo != 1) {
            p->motionNo = 1;
            Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[1], 0.0f, 8.0f, 0x40000001);
        }
        if (p->timer > 0x1E) {
            p->state = 7;
            p->tgtX = 180.0f;
            p->tgtY = p->posY;
            p->tgtZ = -800.0f;
            p->duration = 0x1E;
            p->returnState = 0xD;
            p->timer = 0;
        } else {
            p->timer++;
            p->rotY = p->rotY + 3.6666667f;
        }
        break;

    case 13:
        p->state = 9;
        p->returnState = 0xF;
        p->duration2 = 0x28;
        p->holdTime = 0;
        p->speed = 200.0f;
        p->timer = 0;
        break;

    case 14:
        p->state = 7;
        p->tgtX = 410.0f;
        p->tgtY = 0.0f;
        p->tgtZ = -880.0f;
        p->duration = 0x1E;
        p->returnState = 0xF;
        p->timer = 0;
        break;

    case 15:
        Hu3DMotionSpeedSet(p->modelId, 2.0f);
        if (p->motionNo != 8) {
            p->motionNo = 8;
            Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[8], 0.0f, 8.0f, 0x40000001);
        }
        if (p->timer > 0xB4) {
            p->rotX = 0.0f;
            p->state = 9;
            p->returnState = 0x10;
            p->duration2 = 0x2D;
            p->holdTime = 0xF;
            p->speed = 100.0f;
            p->timer = 0;
        } else {
            p->timer++;
            p->posX = p->posX + -0.075f;
            p->posY = p->posY + 4.3333335f;
            p->posZ = p->posZ + -0.575f;
            p->rotY = 215.0f;
            p->rotX = 10.0f;
        }
        break;

    case 16:
        p->state = 7;
        p->tgtX = 220.0f;
        p->tgtY = 860.0f;
        p->tgtZ = -1050.0f;
        p->duration = 0x1E;
        p->returnState = 0x11;
        p->timer = 0;
        break;

    case 17:
        Hu3DMotionSpeedSet(p->modelId, 1.0f);
        if (p->motionNo != 0xA) {
            p->motionNo = 0xA;
            Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[0xA], 0.0f, 8.0f, 0x40000001);
        }
        if (p->timer > 0x78) {
            p->state = 7;
            p->tgtX = 10.0f;
            p->tgtY = 860.0f;
            p->tgtZ = -1070.0f;
            p->duration = 0x1E;
            p->returnState = 0x12;
            p->timer = 0;
        } else {
            f32 delta = (f32)fmod((f64)(0.0f - p->rotY), m571Turn);

            if (delta < 0.0f) {
                delta += m571TurnF;
            }
            if (delta > m571HalfTurn) {
                delta -= m571TurnF;
            }
            delta = (f32)fmod((f64)(p->rotY + (delta * 0.5f)), m571Turn);
            if (delta < 0.0f) {
                delta += m571TurnF;
            }
            p->rotY = delta;
            p->timer++;
        }
        break;

    case 18:
        Hu3DMotionSpeedSet(p->modelId, 2.0f);
        if (p->motionNo != 8) {
            p->motionNo = 8;
            Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[8], 0.0f, 8.0f, 0x40000001);
        }
        if (p->timer > 0xB4) {
            p->rotX = 0.0f;
            p->state = 9;
            p->returnState = 0x13;
            p->duration2 = 0x2D;
            p->holdTime = 0x14;
            p->speed = 100.0f;
            p->timer = 0;
        } else {
            p->timer++;
            p->posY = p->posY + 3.2777777f;
            p->rotY = -161.0f;
            p->rotX = 10.0f;
        }
        break;

    case 19:
        p->state = 7;
        p->tgtX = -180.0f;
        p->tgtY = 1450.0f;
        p->tgtZ = -1150.0f;
        p->duration = 0x1E;
        p->returnState = 0x15;
        p->baseY = p->posY;
        p->angle = 0.0f;
        p->timer = 0;
        break;

    case 20:
        Hu3DMotionSpeedSet(p->modelId, 1.0f);
        if (p->motionNo != 9) {
            p->motionNo = 9;
            Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[9], 0.0f, 8.0f, 0x40000001);
        }
        if (p->timer > 0x78) {
            p->state = 0;
            p->timer = 0;
        } else {
            f32 delta = (f32)fmod((f64)(0.0f - p->rotY), m571Turn);

            if (delta < 0.0f) {
                delta += m571TurnF;
            }
            if (delta > m571HalfTurn) {
                delta -= m571TurnF;
            }
            delta = (f32)fmod((f64)(p->rotY + (delta * 0.5f)), m571Turn);
            if (delta < 0.0f) {
                delta += m571TurnF;
            }
            p->rotY = delta;
            p->timer++;
        }
        break;

    case 21:
        if ((f32)p->timer > 60.0f) {
            p->state = 0x16;
            Hu3DMotionSpeedSet(p->modelId, 1.0f);
            if (p->motionNo != 4) {
                p->motionNo = 4;
                Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[4], 0.0f, 8.0f, 0);
            }
            p->timer = 0;
        } else {
            Hu3DMotionSpeedSet(p->modelId, 1.0f);
            if (p->motionNo != 3) {
                p->motionNo = 3;
                Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[3], 0.0f, 8.0f, 0);
            }
            p->posY = (f32)((f64)p->baseY + (450.0 * sin((3.141592653589793 * (f64)p->angle) / 180.0)));
            p->posX = (f32)((f64)p->posX + ((200.0 * sin((3.141592653589793 * (f64)p->rotY) / 180.0)) / 90.0));
            p->posZ = (f32)((f64)p->posZ + ((200.0 * cos((3.141592653589793 * (f64)p->rotY) / 180.0)) / 90.0));
            p->timer++;
            p->angle = p->angle + 2.0f;
        }
        break;

    case 22:
        p->timer++;
        if (p->timer > 0x1E) {
            Hu3DMotionSpeedSet(p->modelId, 1.0f);
            if (p->motionNo != 9) {
                p->motionNo = 9;
                Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[9], 0.0f, 8.0f, 0x40000001);
            }
        }
        {
            f32 delta = (f32)fmod((f64)(0.0f - p->rotY), m571Turn);

            if (delta < 0.0f) {
                delta += m571TurnF;
            }
            if (delta > m571HalfTurn) {
                delta -= m571TurnF;
            }
            delta = (f32)fmod((f64)(p->rotY + (delta * 0.5f)), m571Turn);
            if (delta < 0.0f) {
                delta += m571TurnF;
            }
            p->rotY = delta;
        }
        break;

    case 24:
        if (p->timer > 0x28) {
            p->timer = 0;
            p->state = 0x19;
        } else {
            p->timer++;
        }
        break;

    case 25:
        p->posX = lbl_1_data_9C[p->idx].x;
        p->posY = lbl_1_data_9C[p->idx].y;
        p->posZ = lbl_1_data_9C[p->idx].z;
        p->rotX = lbl_1_data_B4[p->idx].x;
        p->rotY = lbl_1_data_B4[p->idx].y;
        p->rotZ = lbl_1_data_B4[p->idx].z;
        Hu3DModelAttrReset(p->modelId, 1);
        p->state = 0x1A;
        p->motionNo = 1;
        Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[1], 0.0f, 8.0f, 0x40000001);
        Hu3DModelAttrReset(p->hookMdl[p->idx], 0x40000002);
        Hu3DMotionTimeSet(p->hookMdl[p->idx], 0.0f);
        if (p->idx == 0) {
            HuAudFXPlay(0x978);
        } else if (p->idx == 1) {
            HuAudFXPlay(0x979);
        }
        break;

    case 26:
        if (p->idx % 2 == 0) {
            p->posX = p->posX + 2.0f;
        } else {
            p->posX = p->posX - 2.0f;
        }
        if (p->timer > 0x64) {
            p->timer = 0;
            p->state = 0x1C;
            if (p->idx % 2 == 0) {
                Hu3DMotionSpeedSet(p->modelId, 1.0f);
                if (p->motionNo != 0xB) {
                    p->motionNo = 0xB;
                    Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[0xB], 0.0f, 8.0f, 0);
                }
                Hu3DModelHookSet(p->modelId, lbl_1_data_100, p->itemMdl);
                Hu3DModelAttrReset(p->itemMdl, 1);
                Hu3DModelAttrReset(p->itemMdl, 0x40000002);
            } else {
                Hu3DMotionSpeedSet(p->modelId, 1.0f);
                if (p->motionNo != 0xC) {
                    p->motionNo = 0xC;
                    Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[0xC], 0.0f, 8.0f, 0);
                }
            }
        } else {
            p->timer++;
        }
        break;

    case 27:
        if (p->idx % 2 == 0) {
            p->posX = p->posX - 3.3333333f;
        } else {
            p->posX = p->posX + 3.3333333f;
        }
        if (p->timer > 0x3C) {
            p->timer = 0;
            p->state = 0x18;
            p->idx = p->idx + 1;
            if (p->idx >= 2) {
                p->idx = 0;
                p->motionNo = 0;
                Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[0], 0.0f, 0.0f, 0);
                p->state = 0x17;
            }
        } else {
            p->timer++;
        }
        break;

    case 28:
        if (p->timer > 0x70) {
            p->timer = 0;
            p->state = 0x1B;
            p->motionNo = 1;
            Hu3DMotionShiftSet(p->modelId, lbl_1_bss_44C[1], 0.0f, 8.0f, 0);
            Hu3DModelHookObjReset(p->modelId, lbl_1_data_100);
            Hu3DModelAttrSet(p->itemMdl, 1);
        } else {
            p->timer++;
        }
        if (p->idx == 0 && p->timer == 0x34) {
            HuAudFXPlayPan(0x96A, 0x20);
        }
        break;
    }

    Hu3DModelPosSet(p->modelId, p->posX, p->posY, p->posZ);
    Hu3DModelRotSet(p->modelId, p->rotX, p->rotY, p->rotZ);
}
// NONMATCHING: register allocation and expression form differ from the target build
// NONMATCHING: draft reconstruction of the item-hook object setup
void fn_1_7670(OMOBJ *obj)
{
    M571Hook *p = (M571Hook *)obj->data;
    s32 i;

    p->modelId = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 7), HU_MEMNUM_OVL, HEAP_MODEL));

    switch (obj->work[0]) {
    case 1:
        break;

    case 0:
        p->hookL = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 0x17), HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(p->hookL, 1);
        Hu3DModelPosSet(p->hookL, 0.0f, 1050.0f, 300.0f);
        Hu3DModelRotSet(p->hookL, 0.0f, 180.0f, 0.0f);
        Hu3DMotionClusterSpeedSet(p->hookL, 0, 1.0f);
        Hu3DModelClusterAttrSet(p->hookL, 0, 0xC0000001);

        p->hookR = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 0x1D), HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(p->hookR, 1);

        p->hookSwap = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 0x1B), HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(p->hookSwap, 0x40000002);
        Hu3DModelAttrSet(p->hookSwap, 1);
        break;

    case 2:
        for (i = 0; i < 2; i++) {
            p->hookMdl[i] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m573, 2), HU_MEMNUM_OVL, HEAP_MODEL));
            Hu3DModelPosSet(p->hookMdl[i], lbl_1_data_CC[i].x, lbl_1_data_CC[i].y, lbl_1_data_CC[i].z);
            Hu3DModelAttrSet(p->hookMdl[i], 0x40000002);
            if (i == 0) {
                Hu3DModelScaleSet(p->hookMdl[i], 1.0f, 1.0f, 1.0f);
            } else {
                Hu3DModelScaleSet(p->hookMdl[i], -1.0f, 1.0f, 1.0f);
            }
        }

        p->itemMdl = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 0x1C), HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(p->itemMdl, 0x40000002);
        Hu3DModelAttrSet(p->itemMdl, 1);
        break;
    }

    for (i = 0; i < 0xF; i++) {
        lbl_1_bss_44C[i] = Hu3DJointMotion(p->modelId, HuDataSelHeapReadNum(lbl_1_data_34[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }

    p->motionNo = 0;
    Hu3DMotionSet(p->modelId, lbl_1_bss_44C[0]);
    Hu3DModelAttrSet(p->modelId, 0x40000001);
    Hu3DModelShadowSet(p->modelId);

    p->posX = lbl_1_data_70.x;
    p->posY = lbl_1_data_70.y;
    p->posZ = lbl_1_data_70.z;
    p->rotZ = 0.0f;
    p->rotY = 0.0f;
    p->rotX = 0.0f;
    p->state = 0;
    p->returnState = -1;
    p->timer = 0;
    p->unk28 = 0.0f;
    p->unk60 = 0;
    p->cycle = 0;
    p->baseY = p->posY;
    p->angle = 0.0f;
    p->idx = 0;

    obj->objFunc = fn_1_5228;
}

s32 fn_1_7A78(void)
{
    s32 i;
    s32 total = 0;

    for (i = 0; i < 4; i++) {
        total += lbl_1_bss_428[i];
    }

    return total;
}

// NONMATCHING: register allocation and expression form differ from the target build
// NONMATCHING: score display refresh, loop form and float compares differ from the target build
void fn_1_7AC8(OMOBJ *obj)
{
    s32 i;

    if (MgSeqModeGet() >= 5) {
        for (i = 0; i < 4; i++) {
            MgScoreWinDispSet(lbl_1_bss_3E0[i], 1);
            espDispOn(lbl_1_bss_3D8[i]);
            espDispOn(lbl_1_bss_3D0[i]);
            espDispOn(lbl_1_bss_3C8[i]);
            MgScoreDispOn(lbl_1_bss_3B8[i]);

            if (lbl_1_bss_428[i] > 0x63) {
                MgScoreValueSet(lbl_1_bss_3B8[i], 0x63);
            } else {
                MgScoreValueSet(lbl_1_bss_3B8[i], lbl_1_bss_428[i]);
            }

            if (lbl_1_bss_408[i] != 0) {
                if (lbl_1_bss_418[i] > 0xC) {
                    lbl_1_bss_408[i] = 0;
                    lbl_1_bss_418[i] = 0;
                    lbl_1_bss_3E8[i] = 1.0f;
                    MgScoreDigitScaleSet(lbl_1_bss_3B8[i], lbl_1_bss_3E8[i], lbl_1_bss_3E8[i]);
                } else {
                    if (lbl_1_bss_418[i] < 4) {
                        lbl_1_bss_3E8[i] = lbl_1_bss_3E8[i] + 0.2f;
                        if (lbl_1_bss_3E8[i] >= 1.8f) {
                            lbl_1_bss_3E8[i] = 1.8f;
                        }
                    } else {
                        lbl_1_bss_3E8[i] = lbl_1_bss_3E8[i] - 0.1f;
                        if (lbl_1_bss_3E8[i] <= 1.0f) {
                            lbl_1_bss_3E8[i] = 1.0f;
                        }
                    }

                    lbl_1_bss_418[i] = lbl_1_bss_418[i] + 1;
                    MgScoreDigitScaleSet(lbl_1_bss_3B8[i], lbl_1_bss_3E8[i], lbl_1_bss_3E8[i]);
                }
            }
        }
    }
}

// NONMATCHING: draft reconstruction of the score display setup
void fn_1_7DF8(OMOBJ *obj)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        lbl_1_bss_3E0[i] = MgScoreWinCreate(0x70, 0x28);
        MgScoreWinTPLvlSet(lbl_1_bss_3E0[i], 0.85f);
        MgScoreWinPosSet(lbl_1_bss_3E0[i], lbl_1_data_7C[i].x, lbl_1_data_7C[i].y);
        MgScoreWinDispSet(lbl_1_bss_3E0[i], 0);

        lbl_1_bss_3D8[i] = espEntry(lbl_1_data_0[lbl_1_bss_43C[i]], 0x63, 0);
        espDispOff(lbl_1_bss_3D8[i]);
        espDrawNoSet(lbl_1_bss_3D8[i], 0);
        espPosSet(lbl_1_bss_3D8[i], lbl_1_data_7C[i].x - 38.0f, lbl_1_data_7C[i].y);
        espAttrSet(lbl_1_bss_3D8[i], 1);

        if (lbl_1_bss_438 == 0) {
            lbl_1_bss_3D0[i] = espEntry(DATANUM(DATA_m571, 0x1E), 0x63, 0);
        } else if (lbl_1_bss_438 == 1) {
            lbl_1_bss_3D0[i] = espEntry(DATANUM(DATA_m572, 0x14), 0x63, 0);
        } else {
            lbl_1_bss_3D0[i] = espEntry(DATANUM(DATA_m573, 6), 0x63, 0);
        }
        espDispOff(lbl_1_bss_3D0[i]);
        espDrawNoSet(lbl_1_bss_3D0[i], 0);
        espPosSet(lbl_1_bss_3D0[i], lbl_1_data_7C[i].x - 15.0f, lbl_1_data_7C[i].y);
        espAttrSet(lbl_1_bss_3D0[i], 1);

        lbl_1_bss_3C8[i] = espEntry(DATANUM(DATA_mgconst, 0x30), 0x63, 0xF);
        espDispOff(lbl_1_bss_3C8[i]);
        espDrawNoSet(lbl_1_bss_3C8[i], 0);
        espPosSet(lbl_1_bss_3C8[i], 4.0f + lbl_1_data_7C[i].x, lbl_1_data_7C[i].y);
        espAttrSet(lbl_1_bss_3C8[i], 1);

        lbl_1_bss_3B8[i] = MgScoreCreate(DATANUM(DATA_mgconst, 0x31), -1, 1);
        MgScoreValueSet(lbl_1_bss_3B8[i], 0);
        MgScorePosSet(lbl_1_bss_3B8[i], 22.0f + lbl_1_data_7C[i].x, lbl_1_data_7C[i].y);
        MgScoreMaxDigitSet(lbl_1_bss_3B8[i], 2);
        MgScoreDispOff(lbl_1_bss_3B8[i]);
        MgScorePriSet(lbl_1_bss_3B8[i], 0x63);

        lbl_1_bss_408[i] = 0;
        lbl_1_bss_418[i] = 0;
        lbl_1_bss_3F8[i] = 0;
        lbl_1_bss_3E8[i] = 1.0f;
    }

    obj->objFunc = fn_1_7AC8;
}

void fn_1_832C(void)
{
    if (lbl_1_bss_46C != 0) {
        MgActorExec();
    }
}

// NONMATCHING: register allocation differs from the target build
void fn_1_8360(void)
{
    lbl_1_bss_46C = 1;
    lbl_1_bss_470[0] = omAddObjEx(lbl_1_bss_680, 0x64, 0, 0, -1, (OMOBJFUNC)fn_1_832C);
}

// NONMATCHING: register allocation and expression form differ from the target build
void fn_1_83C0(s32 arg0)
{
    lbl_1_bss_46C = arg0;
}

// NONMATCHING: register allocation differs from the target build
void fn_1_83D0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    lbl_1_bss_43C[0] = arg0;
    lbl_1_bss_43C[1] = arg1;
    lbl_1_bss_43C[2] = arg2;
    lbl_1_bss_43C[3] = arg3;
    lbl_1_bss_438 = arg4;

    omAddObjEx(lbl_1_bss_680, 0x65, 0, 0, -1, fn_1_7DF8);
}

// NONMATCHING: register allocation and expression form differ from the target build
void fn_1_8478(s32 index, s32 value)
{
    lbl_1_bss_428[index] = value;
}

void fn_1_8490(s32 type)
{
    s32 i;

    if (type == 0) {
        lbl_1_bss_364 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 26), HU_MEMNUM_OVL, 2));
    } else if (type == 1) {
        lbl_1_bss_364 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m572, 17), HU_MEMNUM_OVL, 2));
    } else {
        lbl_1_bss_364 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m573, 3), HU_MEMNUM_OVL, 2));
    }

    Hu3DModelAttrSet(lbl_1_bss_364, 1);

    for (i = 0; i < 20; i++) {
        lbl_1_bss_368[i] = NULL;
    }
}

// NONMATCHING: draft reconstruction, one popup is spawned every 15 frames
void fn_1_8BA8(OMOBJ *obj)
{
    M571PopupGroup *work = (M571PopupGroup *)obj->data;
    OMOBJ *popup;
    M571Cursor *cursor;
    s32 variant;

    if (work->timer % 15 == 0) {
        variant = work->variant;

        lbl_1_bss_360 = lbl_1_bss_360 + 1;

        popup = omAddObjEx(lbl_1_bss_680, 0x66, 1, 0, OM_GRP_NONE, fn_1_8948);
        popup->data = HuMemDirectMallocNum(HEAP_HEAP, 0x48, HU_MEMNUM_OVL);
        cursor = (M571Cursor *)popup->data;

        cursor->unk0 = 0;
        cursor->timer = 0;
        cursor->spawnX = lbl_1_data_7C[variant].x;
        cursor->spawnY = 20.0f + lbl_1_data_7C[variant].y;
        cursor->spawnZ = 1500.0f;
        cursor->rot.z = 0.0f;
        cursor->rot.y = 0.0f;
        cursor->rot.x = 0.0f;
        cursor->pos.x = work->pos.x;
        cursor->pos.y = work->pos.y;
        cursor->pos.z = work->pos.z;
        cursor->step.z = 0.0f;
        cursor->tgt.y = 0.0f;
        cursor->step.x = 0.0f;
        cursor->variant = variant;

        work->count = work->count - 1;

        if (work->count <= 0) {
            omDelObjEx(lbl_1_bss_680, obj);
        }
    }

    work->timer = work->timer + 1;
}

// NONMATCHING: draft reconstruction of the popup group spawner
void fn_1_8D54(Vec *pos, s32 variant, s32 count)
{
    OMOBJ *obj = omAddObjEx(lbl_1_bss_680, 0x66, 0, 0, OM_GRP_NONE, fn_1_8BA8);
    M571PopupGroup *work = (M571PopupGroup *)HuMemDirectMallocNum(HEAP_HEAP, 0x18, HU_MEMNUM_OVL);

    obj->data = work;

    work->pos.x = pos->x;
    work->pos.y = pos->y;
    work->pos.z = pos->z;
    work->variant = variant;
    work->count = count;
    work->timer = 0;
}

s32 fn_1_8E08(void)
{
    s32 i;

    for (i = 0; i < 0x14; i++) {
        if (lbl_1_bss_368[i] != NULL) {
            return 1;
        }
    }

    return 1;
}

void fn_1_8584(OMOBJ *obj)
{
    M571Tile *tile = (M571Tile *)obj->data;
    s32 i;

    Hu3DModelAttrSet(obj->mdlId[0], 1);
    Hu3DModelKill(obj->mdlId[0]);

    lbl_1_bss_360 = lbl_1_bss_360 - 1;

    for (i = 0; i < 20; i++) {
        if (obj == lbl_1_bss_368[i]) {
            lbl_1_bss_368[i] = NULL;
            break;
        }
    }

    omDelObjEx(lbl_1_bss_680, obj);
}

// NONMATCHING: local index form differs from the target build
// NONMATCHING: the target inlines the popup teardown and stores the target Y three times
void fn_1_8654(OMOBJ *obj)
{
    M571Cursor *p = (M571Cursor *)obj->data;
    s32 i;

    if (p->unk0 == 0) {
        Hu3D2Dto3D((HuVecF *)&p->spawnX, 1, (HuVecF *)&p->tgt);
        p->step.x = (p->tgt.x - p->pos.x) / (30.0f - (f32)p->timer);
        p->step.y = (p->tgt.y - p->pos.y) / (30.0f - (f32)p->timer);
        p->step.z = (p->tgt.z - p->pos.z) / (30.0f - (f32)p->timer);
        p->pos.x = p->pos.x + p->step.x;
        p->pos.y = p->pos.y + p->step.y;
        p->pos.z = p->pos.z + p->step.z;
        p->rot.y = p->rot.y + 10.0f;
        p->timer = p->timer + 1;

        if ((f32)p->timer > 30.0f) {
            p->pos.y = p->tgt.y;
            p->pos.y = p->tgt.y;
            p->pos.y = p->tgt.y;

            Hu3DModelAttrSet(obj->mdlId[0], 1);
            Hu3DModelKill(obj->mdlId[0]);

            lbl_1_bss_360 = lbl_1_bss_360 - 1;

            for (i = 0; i < 0x14; i++) {
                if (obj == lbl_1_bss_368[i]) {
                    lbl_1_bss_368[i] = NULL;
                    break;
                }
            }

            omDelObjEx(lbl_1_bss_680, obj);
            lbl_1_bss_428[p->variant] = lbl_1_bss_428[p->variant] + 1;
            lbl_1_bss_408[p->variant] = 1;
            HuAudFXPlay(0x96D);
        }
    }

    omSetTra(obj, p->pos.x, p->pos.y, p->pos.z);
    omSetRot(obj, p->rot.x, p->rot.y, p->rot.z);
}

void fn_1_8948(OMOBJ *obj)
{
    M571Cursor *cursor = (M571Cursor *)obj->data;
    s32 i;

    obj->mdlId[0] = Hu3DModelLink(lbl_1_bss_364);
    omSetTra(obj, cursor->pos.x, cursor->pos.y, cursor->pos.z);
    omSetRot(obj, cursor->rot.x, cursor->rot.y, cursor->rot.z);
    omSetSca(obj, 1.0f, 1.0f, 1.0f);

    for (i = 0; i < 0x14; i++) {
        if (NULL == lbl_1_bss_368[i]) {
            lbl_1_bss_368[i] = obj;
            break;
        }
    }

    Hu3DModelAttrReset(obj->mdlId[0], 1);
    obj->objFunc = fn_1_8654;
}

// NONMATCHING: CodeWarrior hoists one rodata load for 3 identical 0.0f stores; MWCC 1.3.2 repeats it (4 rows)
void fn_1_8A58(Vec *pos, s32 variant)
{
    OMOBJ *obj;
    M571Cursor *cursor;

    lbl_1_bss_360 = lbl_1_bss_360 + 1;

    obj = omAddObjEx(lbl_1_bss_680, 0x66, 1, 0, OM_GRP_NONE, fn_1_8948);
    obj->data = HuMemDirectMallocNum(0, 0x48, HU_MEMNUM_OVL);
    cursor = (M571Cursor *)obj->data;

    cursor->unk0 = 0;
    cursor->timer = 0;

    cursor->spawnX = lbl_1_data_7C[variant].x;
    cursor->spawnY = 20.0f + lbl_1_data_7C[variant].y;
    cursor->spawnZ = 1500.0f;

    cursor->rot.z = m571Zero;
    cursor->rot.y = cursor->rot.z;
    cursor->rot.x = cursor->rot.z;

    cursor->pos.x = pos->x;
    cursor->pos.y = pos->y;
    cursor->pos.z = pos->z;

    cursor->step.z = m571Zero;
    cursor->tgt.y = cursor->step.z;
    cursor->step.x = cursor->step.z;

    cursor->variant = variant;
}

// NONMATCHING: register allocation and expression form differ from the target build
void fn_1_8E5C(void)
{
    lbl_1_bss_35C = -1;
}

// NONMATCHING: register allocation and expression form differ from the target build
void fn_1_8E70(void)
{
    lbl_1_bss_35C = HuAudSStreamPlay(0x3D);
}

// NONMATCHING: register allocation and expression form differ from the target build
void fn_1_8EA0(void)
{
    HuAudSStreamFadeOut(lbl_1_bss_35C, 100);
}

// NONMATCHING: the inlined square root still allocates its guess differently
s32 fn_1_8ED0(Vec *arg0, Vec *arg1, f32 arg2, f32 arg3)
{
    f32 dx = arg0->x - arg1->x;
    f32 dy = arg0->y - arg1->y;
    f32 dz = arg0->z - arg1->z;
    f32 sum = dz * dz + (dx * dx + dy * dy);
    f32 dist = sum;

    if (sum > 0.0f) {
        dist = sqrtf(sum);
    }

    if (arg2 + arg3 > dist) {
        return 1;
    }

    return 0;
}

// NONMATCHING: the inlined square root still allocates its guess differently
f32 fn_1_9128(f32 arg0, f32 arg1, f32 arg2, f32 arg3)
{
    f32 dx = arg0 - arg2;
    f32 dy = arg1 - arg3;
    f32 sum = dx * dx + dy * dy;
    f32 dist = sum;

    if (sum > 0.0f) {
        dist = sqrtf(sum);
    }

    return dist;
}
s32 fn_1_90C8(Vec *arg0, Vec *arg1, f32 arg2, f32 arg3)
{
    if ((arg1->x - arg0->x) * (arg1->x - arg0->x) + (arg1->z - arg0->z) * (arg1->z - arg0->z)
        < (arg2 + arg3) * (arg2 + arg3)) {
        return 1;
    } else {
        return 0;
    }
}

// NONMATCHING: register allocation and expression form differ from the target build
s32 fn_1_9298(s32 base, s32 range)
{
    return base + frandmod(range - base);
}

s16 fn_1_92D4(HuVecF *pos)
{
    HuVecF scr;
    f32 pan;

    Hu3D3Dto2D(pos, 1, &scr);

    pan = m571PanBias + m571PanScale * (scr.x / m571ScreenXDiv);

    return (s16)pan;
}

// NONMATCHING: local variable form differs from the target build
s16 fn_1_9350(HuVecF *pos)
{
    HuVecF scr;
    f32 vol;

    Hu3D3Dto2D(pos, 1, &scr);

    vol = m571VolBias + m571VolScale * (scr.y / m571ScreenYDiv);

    return (s16)vol;
}
