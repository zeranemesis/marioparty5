#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mg/actman.h"
#include "game/mg/seqman.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/wipe.h"
#include "game/gamework.h"
#include "math.h"

#include "datadir_enum.h"

#ifndef __MWERKS__
#include "game/mg/score.h"
#endif

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* the call site in the binary only sets the three position arguments */
typedef HU3DLIGHTID (*LightCreate3)(float, float, float);
typedef void (*LightColorSet4)(HU3DLIGHTID, u8, u8, u8, u8);
typedef void (*ModelShadowSet2)(HU3DMODELID, s32);

void ObjectSetup(void);
void fn_1_68C(OMOBJ *obj);
void fn_1_BF4(OMOBJ *obj);
void fn_1_D58(OMOBJ *obj);
void fn_1_1078(OMOBJ *obj);
s32 fn_1_1168(OMOBJ *obj);
s32 fn_1_1264(Vec *pos, Vec *out);
void fn_1_1518(void);
void fn_1_1630(void);
void fn_1_16F4(void);
void fn_1_1760(void);
void fn_1_1898(OMOBJ *obj);
void fn_1_18B8(void);
void fn_1_1BA0(void);

// not yet written
s32 fn_1_1D44(s32 arg0);
s32 fn_1_2A14(OMOBJ *obj, s32 arg1);
void fn_1_2D58(void);
void fn_1_2E28(void);
s32 fn_1_2FE0(OMOBJ *obj, s32 arg1);
void fn_1_27E8(OMOBJ *obj, s32 arg1, float arg2, s32 arg3);
void fn_1_2870(s32 arg0);
void fn_1_3520(void);
void fn_1_3700(OMOBJ *obj);
void fn_1_7670(OMOBJ *obj);
void fn_1_7DF8(OMOBJ *obj);

typedef struct M571Pos_s {
    f32 x; //0x0
    f32 y; //0x4
    f32 z; //0x8
} M571Pos;

typedef struct M571Cell_s {
    s32 x; //0x0
    s32 y; //0x4
    s32 z; //0x8
} M571Cell;

typedef struct M571Grid_s {
    M571Pos pos[49]; //0x0 (7x7 board lattice, -360..360 step 120)
    M571Cell cell[49]; //0x24C (per-cell tile table)
} M571Grid;

typedef struct M571Player_s {
    s32 unk0; //0x0 player number (mirror of obj->work[0])
    s32 unk4; //0x4
    s32 unk8; //0x8
    s32 unkC; //0xC
    s32 unk10; //0x10
    s16 unk14; //0x14
    s16 unk16; //0x16
    Vec unk18; //0x18 mirror of the MGACTOR position
    s32 unk24; //0x24 player number again
    s32 unk28; //0x28
    s32 unk2C; //0x2C
    s32 unk30; //0x30
    s32 unk34; //0x34
    s32 unk38; //0x38
    s16 unk3C; //0x3C
    u8 pad3E[10]; //0x3E
    s32 unk48; //0x48
    s32 unk4C; //0x4C
    s32 unk50; //0x50
    s32 unk54; //0x54
    s32 unk58; //0x58
    s32 unk5C; //0x5C character type taken from GwPlayerConf
    s32 unk60; //0x60
    Vec unk64; //0x64
    s32 unk70; //0x70
    s32 unk74; //0x74
    s32 unk78; //0x78
    s32 unk7C; //0x7C
    s32 unk80; //0x80
    s32 unk84; //0x84
    s32 unk88; //0x88
} M571Player; //0x8C

u32 lbl_1_data_0[13] = {
    DATANUM(DATA_mgconst, 0x20), DATANUM(DATA_mgconst, 0x21), DATANUM(DATA_mgconst, 0x22),
    DATANUM(DATA_mgconst, 0x23), DATANUM(DATA_mgconst, 0x24), DATANUM(DATA_mgconst, 0x26),
    DATANUM(DATA_mgconst, 0x27), DATANUM(DATA_mgconst, 0x28), DATANUM(DATA_mgconst, 0x29),
    DATANUM(DATA_mgconst, 0x2A), DATANUM(DATA_mgconst, 0x2B), DATANUM(DATA_mgconst, 0x2C),
    DATANUM(DATA_mgconst, 0x2D)
};

u32 lbl_1_data_34[15] = {
    DATANUM(DATA_m571, 8), DATANUM(DATA_m571, 9), DATANUM(DATA_m571, 10), DATANUM(DATA_m571, 13),
    DATANUM(DATA_m571, 14), DATANUM(DATA_m571, 15), DATANUM(DATA_m571, 16), DATANUM(DATA_m571, 17),
    DATANUM(DATA_m571, 18), DATANUM(DATA_m571, 19), DATANUM(DATA_m571, 20), DATANUM(DATA_m571, 21),
    DATANUM(DATA_m571, 22), DATANUM(DATA_m571, 11), DATANUM(DATA_m571, 12)
};

Vec lbl_1_data_70 = { 0.0f, 100.0f, -750.0f };

float lbl_1_data_7C[8] = { 70.0f, 60.0f, 255.0f, 60.0f, 386.0f, 60.0f, 505.0f, 60.0f };

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
u8 lbl_1_bss_14[0xC8];
MGPLAYER *lbl_1_bss_DC[4];
s32 lbl_1_bss_EC;
s32 lbl_1_bss_F0[25];
s16 lbl_1_bss_F0_pad;
s16 lbl_1_bss_156[0x35];
OMOBJ *lbl_1_bss_1C0;
OMOBJ *lbl_1_bss_1C4;
OMOBJ *lbl_1_bss_1C8[49];
s32 lbl_1_bss_28C[49];
OMOBJ **lbl_1_bss_350;
OMOBJ **lbl_1_bss_354;
s16 lbl_1_bss_358;
s16 lbl_1_bss_35A;
s32 lbl_1_bss_35C;
s32 lbl_1_bss_360;
s16 lbl_1_bss_364;
s16 lbl_1_bss_366;
s32 lbl_1_bss_368[20];
s32 lbl_1_bss_3B8[4];
s32 lbl_1_bss_3C8[2];
s32 lbl_1_bss_3D0[2];
s32 lbl_1_bss_3D8[2];
s32 lbl_1_bss_3E0[2];
s32 lbl_1_bss_3E8[4];
s32 lbl_1_bss_3F8[4];
s32 lbl_1_bss_408[4];
s32 lbl_1_bss_418[4];
s32 lbl_1_bss_428[4];
s32 lbl_1_bss_438;
s32 lbl_1_bss_43C[4];
s16 lbl_1_bss_44C[16];
s32 lbl_1_bss_46C;
s32 lbl_1_bss_470[2];
OMOBJ **lbl_1_bss_478;
u8 lbl_1_bss_47C[0x104];
s32 lbl_1_bss_580;
s32 lbl_1_bss_584[3];
s32 lbl_1_bss_590[12];
s32 lbl_1_bss_5C0[12];
s32 lbl_1_bss_5F0[12];
s32 lbl_1_bss_620[12];
s32 lbl_1_bss_650[3];
s32 lbl_1_bss_65C[3];
s32 lbl_1_bss_668[3];
s32 lbl_1_bss_674[3];
OMOBJMAN *lbl_1_bss_680;
s32 lbl_1_bss_684[3];

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

    lbl_1_bss_12 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 2), DATANUM(DATA_mgfree, 0), 2));
    Hu3DModelAttrSet(lbl_1_bss_12, 1);
    lbl_1_bss_10 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 6), DATANUM(DATA_mgfree, 0), 2));
    Hu3DModelAttrSet(lbl_1_bss_10, 1);

    for (i = 0; i < 49; i++) {
        lbl_1_bss_28C[i] = 0;
    }

    lbl_1_bss_E = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 26), DATANUM(DATA_mgfree, 0), 2));
    lbl_1_bss_C = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 29), DATANUM(DATA_mgfree, 0), 2));
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
        obj->data = HuMemDirectMallocNum(0, 0x8C, DATANUM(DATA_mgfree, 0));
        obj->work[0] = i;
    }

    omMakeGroupEx(lbl_1_bss_680, 5, 1);
    lbl_1_bss_478 = omGetGroupMemberListEx(lbl_1_bss_680, 5);

    lbl_1_bss_478[0] = omAddObjEx(lbl_1_bss_680, 0x6A, 0, 0, 5, fn_1_7670);
    lbl_1_bss_478[0]->data = HuMemDirectMallocNum(0, 0xB0, DATANUM(DATA_mgfree, 0));
    lbl_1_bss_478[0]->work[0] = 0;

    lbl_1_bss_364 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 26), DATANUM(DATA_mgfree, 0), 2));
    Hu3DModelAttrSet(lbl_1_bss_364, 1);

    for (i = 0; i < 20; i++) {
        lbl_1_bss_368[i] = 0;
    }

    lbl_1_bss_35C = -1;

    fn_1_2870(0);

    MgSeqCreatePrio(&lbl_1_data_D48, 1000);
}

void fn_1_68C(OMOBJ *obj)
{
    MGACTOR_PARAM param;
    s32 i;
    s32 j;
    s16 mdlId[49];

    mdlId[0] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 0), DATANUM(DATA_mgfree, 0), 2));
    mdlId[1] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 1), DATANUM(DATA_mgfree, 0), 2));
    lbl_1_bss_156[0] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 3), DATANUM(DATA_mgfree, 0), 2));
    mdlId[2] = lbl_1_bss_156[0];
    lbl_1_bss_156[1] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m571, 4), DATANUM(DATA_mgfree, 0), 2));
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

void fn_1_BF4(OMOBJ *obj)
{
    M571Player *p = (M571Player *)obj->data;
    s32 val;

    p->unk0 = obj->work[0];

    p->unk4 = GwPlayerConf[p->unk0].charNo;
    p->unk8 = GwPlayerConf[p->unk0].padNo;
    p->unkC = 0;
    p->unk30 = 0;
    p->unk7C = 0;
    p->unk78 = 0;

    p->unk5C = GwPlayerConf[p->unk0].dif;
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

        if (GwPlayerConf[p->unk0].type != 0) {
            p->unk70 = 0;
            p->unk74 = 0;
            p->unk78 = 0;
            p->unk7C = 0;

            fn_1_1078(obj);
            MgPlayerPadSet(lbl_1_bss_DC[p->unk0], p->unk70, p->unk74, p->unk78, p->unk7C);
        }

        mp = lbl_1_bss_DC[p->unk0];
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
                    fn_1_2FE0(lbl_1_bss_1C8[tileIdx], p->unk0);
                    fn_1_1D44(tileIdx);
                    omVibrate((s16)p->unk8, 15, 6, 6);
                    p->unk3C = 0;
                }
            }
        }
    }

    mp = lbl_1_bss_DC[p->unk0];

    p->unk18.x = mp->actor->pos.x;
    p->unk18.y = mp->actor->pos.y;
    p->unk18.z = mp->actor->pos.z;
    p->unk24 = 0.0f;
    p->unk28 = mp->actor->rotY;
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
        } else if (p->unk10 & 0xBF80) {
            p->unk78 |= 0x200;
        }
        break;
    case 2:
    case 3:
        p->unk80++;
        if (p->unk80 > p->unk84) {
            p->unk60 = 0;
            p->unk80 = 0;
        }
        break;
    }
}

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
    p->unk74 = (s8)(56.0f * -diff.z);

    return 0;
}

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

void fn_1_1518(void)
{
    s32 i;

    if (MgSeqFrameNoGet() == 0) {
        for (i = 0; i < 4; i++) {
            M571Player *p = (M571Player *)lbl_1_bss_354[i]->data;

            lbl_1_bss_43C[i] = p->unk4;
        }

        lbl_1_bss_438 = 0;

        omAddObjEx(lbl_1_bss_680, 0x65, 0, 0, OM_GRP_NONE, fn_1_7DF8);
    }

    if ((WipeCheckEnd() & 0xFFFFFF00) == 0) {
        MgSeqModeNext();
        MgActorExec();
    }

    lbl_1_bss_35C = HuAudSStreamPlay(0x3D);
}

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
    p->unk28 = 1300.0f;
    p->unk30 = 2;

    MgSeqModeNext();
}

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

void fn_1_1760(void)
{
    s32 i;
    s32 found;

    if (MgSeqFrameNoGet() == 0) {
        HuAudSStreamFadeOut(lbl_1_bss_35C, 0x64);

        lbl_1_bss_1C0 = omAddObjEx(lbl_1_bss_680, 0x64, 0, 0, OM_GRP_NONE, fn_1_1898);
    }

    if (MgSeqFrameNoGet() > 300) {
        found = 0;

        for (i = 0; i < 20; i++) {
            if (lbl_1_bss_368[i] != 0) {
                found = 1;
                break;
            }
        }

        if (found != 0) {
            for (i = 0; i < 4; i++) {
                M571Player *p = (M571Player *)lbl_1_bss_354[i]->data;

                Hu3DModelAttrReset(lbl_1_bss_DC[i]->actor->mdlId, 1);
            }

            MgSeqModeNext();
        }
    }
}

void fn_1_1898(OMOBJ *obj)
{
    MgActorExec();
}

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

            MgActorKill(lbl_1_bss_DC[i]);
            omDelObjEx(lbl_1_bss_680, lbl_1_bss_354[i]);
        }

        CharModelKill(-1);

        for (i = 0; i < 4; i++) {
            OMOBJ *obj = omAddObjEx(lbl_1_bss_680, 0x65, 1, 3, 1, fn_1_3700);

            lbl_1_bss_350[i] = obj;
            obj->data = HuMemDirectMallocNum(0, 0x34, DATANUM(DATA_mgfree, 0));
            obj->work[0] = i;
        }

        fn_1_3520();

        p->unk30 = 0;

        q = (M571Player *)lbl_1_bss_478[0]->data;

        Hu3DMotionSpeedSet(q->unk0, 1.0f);

        if (q->unk2C != 0) {
            q->unk2C = 0;
            Hu3DMotionShiftSet(q->unk0, lbl_1_bss_44C[0], 0.0f, 0.0f, 0x4001);
        }

        Hu3DModelAttrReset(p->unk0, 1);

        Hu3DModelHookObjReset(p->unk54, lbl_1_data_11A);
        Hu3DModelHookObjReset(p->unk54, lbl_1_data_124);
        Hu3DModelHookObjReset(p->unk0, lbl_1_data_100);
        Hu3DModelHookObjReset(p->unk0, lbl_1_data_10D);

        Hu3DModelAttrSet(p->unk54, 1);
        Hu3DModelAttrSet(p->unk58, 1);
        Hu3DModelAttrSet(p->unk56, 1);

        HuPrcVSleep();

        p->unk4 = lbl_1_data_70.x;
        p->unk8 = lbl_1_data_70.y;
        p->unkC = lbl_1_data_70.z;
        p->unk24 = 0.0f;
        p->unk20 = 0.0f;
        p->unk1C = 0.0f;

        WipeCreate(1, 5, 30);

        do {
            HuPrcVSleep();
        } while ((WipeCheckEnd() & 0xFFFFFF00) != 0);

        MgSeqModeNext();
    }
}

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
                    CharFXPlay(q->unk28, 0x200);
                }
            }

            score = (s16)(q->unk30 + GwPlayer[q->unk24].score);

            if (!_CheckFlag(0x1F)) {
                GwPlayer[q->unk24].score = (s16)score;
            }
        }
    }
}