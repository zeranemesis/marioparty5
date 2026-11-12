#include "types.h"
#include "string.h"
#include "dolphin.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/hu3d.h"
#include "game/mgdata.h"
#include "game/object.h"
#include "game/wipe.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"
#include "REL/m552dll.h"

typedef void (*M552VoidFunc)(void);
extern const M552VoidFunc _ctors[];
extern const M552VoidFunc _dtors[];

/* cube.c */
void fn_1_7888(void);
void fn_1_88EC(void);
/* com.c */
void fn_1_91BC(void);
/* game.c */
void fn_1_1C40(void);
void fn_1_1FF4(void);
void fn_1_2180(void);
void fn_1_26BC(void);
void fn_1_27B4(void);
void fn_1_2890(void);
void fn_1_2930(void);
void fn_1_2AA0(void);
void fn_1_2E48(void);
/* stage.c */
void fn_1_B33C(M552State *state, M552Mdl *models, s32 *dataNums);
void fn_1_B588(s16 arg0);
void fn_1_B810(s32 *arg0, s32 *arg1);
void fn_1_B834(s16 *arg0, s16 *arg1);
void fn_1_B858(void);
void fn_1_BF98(void);
void fn_1_C14C(void);

extern MGTIMER *lbl_1_bss_490C;

void ObjectSetup(void);
void fn_1_148(s16 arg0);
void fn_1_524(void);
void fn_1_568(s16 arg0);
void fn_1_620(void);
void fn_1_664(volatile s16 arg0);
void fn_1_69C(void);
void fn_1_6E0(void);
void fn_1_700(void);
void fn_1_730(void);
void fn_1_734(void);
void fn_1_AE8(void);
void fn_1_B20(void);
void fn_1_132C(s32 arg0);
void fn_1_14D0(M552Obj *obj);
void fn_1_15A4(void);
void fn_1_1914(HU3DMODEL *model, Mtx *mtx);

/* unit-owned .bss (declared in reverse address order) */
M552State lbl_1_bss_4794;
M552Field lbl_1_bss_3700;
s16 lbl_1_bss_36E2[2][7];
s16 lbl_1_bss_33C2[2][200];
s16 lbl_1_bss_33C0;
M552Obj lbl_1_bss_1340[0xD0];
M552Obj lbl_1_bss_1200[8];
M552Obj lbl_1_bss_710[70];
M552Obj lbl_1_bss_530[12];
M552Obj lbl_1_bss_490[4];
M552Mdl lbl_1_bss_84[0x25];
OMCAMERAVIEW lbl_1_bss_14[4];
s16 lbl_1_bss_C[4];
u8 lbl_1_bss_A;
s16 lbl_1_bss_8;
OMOBJ *lbl_1_bss_4;
s32 lbl_1_bss_0;

/* unit-owned .data */
s32 lbl_1_data_0[48] = {
    0x6E0000, 0x6E0001, 0x6E0002, 0x6E0003, 0x6E0004, 0x6E0005, 0x6E0006, 0x6E0007, 0x6E0008, 0x6E0009,
    0x6E000A, 0x6E000B, 0x6E000C, 0x6E000D, 0x6E000E, 0x6E000F, 0x6E0010, 0x6E0011, 0x6E0012, 0x6E0013,
    0x6E0014, 0x6E0015, 0x6E0016, 0x6E0017, 0x6E0018, 0x6E0019, 0x6E001A, 0x6E001B, 0x6E001C, 0x6E001D,
    0x6E001E, 0x6E001F, 0x6E0020, 0x6E0021, 0x6E0022, 0x6E0023, 0x6E0024, 0x6E0025, 0x6E0026, 0x6E0027,
    0x6E0028, 0x6E0029, 0x6E002A, 0x6E002B, 0x6E002C, 0x6E002D, 0x6E002E, 0x6E002F,
};
s32 lbl_1_data_C0[40] = {
    0x6E0030, 0x6E0031, 0x6E0032, 0x6E0033, 0x6E0034, 0x6E0035, 0x6E0036, 0x6E0037, 0x6E0038, 0x6E0039,
    0x6E0044, 0x6E0045, 0x6E0046, 0x6E0047, 0x6E0048, 0x6E0049, 0x6E004A, 0x6E004B, 0x6E004C, 0x6E004D,
    0x6E004E, 0x6E004F, 0x6E0050, 0x6E0051, 0x6E0052, 0x6E0053, 0x6E0054, 0x6E0055, 0x6E0056, 0x6E0057,
    0x6E003A, 0x6E003B, 0x6E003C, 0x6E003D, 0x6E003E, 0x6E003F, 0x6E0040, 0x6E0041, 0x6E0042, 0x6E0043,
};
s32 lbl_1_data_160[13][2] = {
    { 0x8C0000, 0x8C0002 }, { 0x330000, 0x330002 }, { 0xB70000, 0xB70002 }, { 0xED0000, 0xED0002 },
    { 0xE60000, 0xE60002 }, { 0x170000, 0x170002 }, { 0xE00000, 0xE00002 }, { 0x8C0000, 0x8C0002 },
    { 0x8C0000, 0x8C0002 }, { 0x8C0000, 0x8C0002 }, { 0x8C0000, 0x8C0002 }, { 0x8C0000, 0x8C0002 },
    { 0x8C0000, 0x8C0002 },
};
MGSEQ_PARAM lbl_1_data_1C8 = {
    0, 0,
    (MGSEQ_FUNC)fn_1_148, (MGSEQ_FUNC)fn_1_524, (MGSEQ_FUNC)fn_1_568,
    (MGSEQ_FUNC)fn_1_620, (MGSEQ_FUNC)fn_1_664, (MGSEQ_FUNC)fn_1_69C,
    (MGSEQ_FUNC)fn_1_6E0, (MGSEQ_FUNC)fn_1_700, (MGSEQ_FUNC)fn_1_730,
};
M552VoidFunc lbl_1_data_1F0[4] = { fn_1_1C40, fn_1_1FF4, fn_1_2180, fn_1_26BC };
M552VoidFunc lbl_1_data_200[3] = { fn_1_27B4, fn_1_2890, fn_1_2930 };
M552VoidFunc lbl_1_data_20C[1] = { fn_1_2E48 };
char lbl_1_data_210[12] = "m552score01";
char lbl_1_data_21C[12] = "m552score02";
char lbl_1_data_228[12] = "m552score03";
char lbl_1_data_234[12] = "m552score04";
char lbl_1_data_240[16] = "";
char lbl_1_data_250[16] = "TL32m552chara01";
char lbl_1_data_260[16] = "TL32m552chara02";
char lbl_1_data_270[16] = "TL32m552chara03";
char lbl_1_data_280[16] = "TL32m552chara04";
char lbl_1_data_290[16] = "TL32m552chara05";
char lbl_1_data_2A0[16] = "TL32m552chara06";
char lbl_1_data_2B0[16] = "TL32m552chara07";
char lbl_1_data_2C0[16] = "TL32m552chara08";
char lbl_1_data_2D0[16] = "TL32m552chara09";
char lbl_1_data_2E0[16] = "TL32m552chara10";
char lbl_1_data_2F0[16] = "TL32m552chara11";
char lbl_1_data_300[16] = "TL32m552chara12";
char lbl_1_data_310[16] = "TL32m552chara13";
char *lbl_1_data_320[13] = {
    lbl_1_data_250, lbl_1_data_260, lbl_1_data_270, lbl_1_data_280, lbl_1_data_290, lbl_1_data_2A0, lbl_1_data_2B0,
    lbl_1_data_2C0, lbl_1_data_2D0, lbl_1_data_2E0, lbl_1_data_2F0, lbl_1_data_300, lbl_1_data_310,
};
char lbl_1_data_354[12] = "m552count01";

int _prolog(void)
{
    const M552VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const M552VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    s16 i;

    for (i = 0; i < 0x25; i++) {
        lbl_1_bss_84[i].id = -1;
    }
    fn_1_B33C(&lbl_1_bss_4794, lbl_1_bss_84, lbl_1_data_0);
    lbl_1_bss_4794.objman = omInitObjMan(0x32, 0x2000);
    omGameSysInit(lbl_1_bss_4794.objman);
    MgSeqCreate(&lbl_1_data_1C8);
}

void fn_1_148(s16 arg0)
{
    HuVecF camPos;
    HuVecF camUp;
    HuVecF camTarget;
    HU3DLIGHTID light;
    s16 mode;

    mode = arg0;
    fn_1_B588(0);
    fn_1_7888();
    fn_1_B810(lbl_1_data_160[lbl_1_bss_4794.chars[0].charNo], lbl_1_data_160[lbl_1_bss_4794.chars[1].charNo]);
    fn_1_B834(&lbl_1_bss_C[0], &lbl_1_bss_C[2]);
    fn_1_B858();
    fn_1_BF98();
    omAddObjEx(lbl_1_bss_4794.objman, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_C14C);
    fn_1_B20();
    fn_1_734();
    light = Hu3DGLightCreate(0.0f, 1000.0f, 1000.0f, 0.0f, -1.0f, -1.0f, 0xFF, 0xFF, 0xFF);
    Hu3DGLightInfinitytSet(light);
    Hu3DGLightStaticSet(light, 1);
    omAddObjEx(lbl_1_bss_4794.objman, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_15A4);
    Hu3DBGColorSet(0, 0, 0);
    Hu3DShadowCreate(45.0f, 20.0f, 15000.0f);
    camPos.x = 0.0f;
    camPos.y = 3600.0f;
    camPos.z = -10.0f;
    camUp.x = 0.0f;
    camUp.y = 1.0f;
    camUp.z = 0.0f;
    camTarget.x = 0.0f;
    camTarget.y = 0.0f;
    camTarget.z = 0.0f;
    Hu3DShadowTPLvlSet(0.5f);
    Hu3DShadowPosSet(&camPos, &camUp, &camTarget);
    Hu3DModelShadowSet(lbl_1_bss_4794.models[0].id);
    Hu3DModelShadowSet(lbl_1_bss_4794.models[1].id);
    Hu3DModelShadowMapSet(lbl_1_bss_4794.models[2].id);
    lbl_1_bss_4794.slots[0].motion = 1;
    lbl_1_bss_4794.slots[0].speed = 1.0f;
    lbl_1_bss_4794.slots[0].time = 0.0f;
    lbl_1_bss_4794.slots[0].attrSet = 0x40000001;
    lbl_1_bss_4794.slots[1].motion = 1;
    lbl_1_bss_4794.slots[1].speed = 1.0f;
    lbl_1_bss_4794.slots[1].time = 0.0f;
    lbl_1_bss_4794.slots[1].attrSet = 0x40000001;
    lbl_1_bss_3700.plr[0].x = -400.0f;
    lbl_1_bss_3700.plr[1].x = 400.0f;
    lbl_1_bss_3700.plr[0].y = 0.0f;
    lbl_1_bss_3700.plr[1].y = 0.0f;
    lbl_1_bss_3700.plr[0].z = 125.0f;
    lbl_1_bss_3700.plr[1].z = 125.0f;
    MgSeqModeDelaySet(mode + 1, -1);
    MgSeqModeNext();
    lbl_1_bss_0 = MgSubModeGet();
}

void fn_1_524(void)
{
    lbl_1_data_1F0[lbl_1_bss_4794.stat[0]]();
}

void fn_1_568(s16 arg0)
{
    volatile s16 mode;

    mode = arg0;
    lbl_1_bss_4794.stat[0] = 0;
    lbl_1_bss_4794.stat[1] = 0;
    if (mgSubMode == 1) {
        MgTimerParamSet(lbl_1_bss_490C, 0x2A30, 0, 0);
    }
    if (mgSubMode == 2) {
        MgTimerParamSet(lbl_1_bss_490C, 0x4650, 0, 0);
    }
    MgSeqModeDelaySet((s16)(mode + 1), -1);
    fn_1_88EC();
}

void fn_1_620(void)
{
    lbl_1_data_200[lbl_1_bss_4794.stat[0]]();
}

void fn_1_664(volatile s16 arg0)
{
    MgSeqModeDelaySet((s16)(arg0 + 1), -1);
    fn_1_2AA0();
}

void fn_1_69C(void)
{
    lbl_1_data_20C[lbl_1_bss_4794.stat[0]]();
}

void fn_1_6E0(void)
{
    MgSeqModeNext();
}

void fn_1_700(void)
{
    if ((u8)WipeCheckEnd() == 0) {
        MgSeqModeNext();
    }
}

void fn_1_730(void)
{
}

void fn_1_734(void)
{
    s32 camBit[5] = { 1, 2, 4, 8, 0x10 };
    s32 i;

    lbl_1_bss_4 = omAddObjEx(lbl_1_bss_4794.objman, 0x7FDA, 0, 0, -1, omOutViewMulti);
    lbl_1_bss_4->work[0] = 3;
    lbl_1_bss_14[0].center.x = 0.0f;
    lbl_1_bss_14[0].center.y = 500.0f;
    lbl_1_bss_14[0].center.z = 0.0f;
    lbl_1_bss_14[0].rot.x = 0.0f;
    lbl_1_bss_14[0].rot.y = 0.0f;
    lbl_1_bss_14[0].rot.z = 0.0f;
    lbl_1_bss_14[0].zoom = 300.0f;
    lbl_1_bss_14[1].center.x = 0.0f;
    lbl_1_bss_14[1].center.y = 0.0f;
    lbl_1_bss_14[1].center.z = 0.0f;
    lbl_1_bss_14[1].rot.x = 0.0f;
    lbl_1_bss_14[1].rot.y = 0.0f;
    lbl_1_bss_14[1].rot.z = 0.0f;
    lbl_1_bss_14[1].zoom = 1950.0f;
    Hu3DCameraCreate(0x1F);
    for (i = 0; i < 3; i++) {
        Hu3DCameraPerspectiveSet(camBit[i], 45.0f, 20.0f, 50000.0f, 1.2f);
        Hu3DCameraViewportSet(camBit[i], 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
        Hu3DCameraScissorSet(camBit[i], 0, 0, 0x280, 0x1E0);
        CenterM[i].x = 0.0f;
        CenterM[i].y = 0.0f;
        CenterM[i].z = 500.0f;
        CRotM[i].x = -20.0f;
        CRotM[i].y = 0.0f;
        CRotM[i].z = 0.0f;
        CZoomM[i] = 900.0f;
    }
    Hu3DLayerHookSet(0xB, (HU3DLAYERHOOK)fn_1_AE8);
    fn_1_91BC();
}

void fn_1_AE8(void)
{
    if ((Hu3DCameraBit & 4) != 0) {
        Hu3DZClear();
    }
}

void fn_1_B20(void)
{
    HuVecF pos[11] = {
        { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f },
        { -400.0f, 0.0f, 125.0f }, { 400.0f, 0.0f, 125.0f }, { -400.0f, 0.0f, 125.0f }, { 400.0f, 0.0f, 125.0f },
        { -400.0f, 0.0f, 125.0f }, { 400.0f, 0.0f, 125.0f }, { 0.0f, 0.0f, 0.0f },
    };
    s32 i;
    s32 j;
    M552Mdl *mdl;
    M552Obj *obj;

    for (i = 0; i < 0xB; i++) {
        mdl = &lbl_1_bss_84[i + 8];
        mdl->id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_0[i + 0xE], 0x10000000, 2));
        Hu3DModelAttrSet(mdl->id, 1);
        if (i >= 4 && i < 0xA) {
            Hu3DModelCameraSet(mdl->id, 4);
            if (i == 4 || i == 5) {
                Hu3DModelLayerSet(mdl->id, 4);
            } else {
                Hu3DModelLayerSet(mdl->id, 5);
            }
        } else {
            Hu3DModelCameraSet(mdl->id, 1);
        }
        Hu3DModelPosSetV(mdl->id, &pos[i]);
        mdl->pos.x = pos[i].x;
        mdl->pos.y = pos[i].y;
        mdl->pos.z = pos[i].z;
        mdl->rot.x = 0.0f;
        mdl->rot.y = 0.0f;
        mdl->rot.z = 0.0f;
        if (i == 0 || i == 1) {
            Hu3DMotionSpeedSet(mdl->id, 1.0f);
        } else {
            Hu3DMotionSpeedSet(mdl->id, 0.0f);
        }
        Hu3DMotionTimeSet(mdl->id, 0.0f);
        Hu3DModelAttrReset(mdl->id, 0x40000001);
    }
    for (i = 0; i < 4; i++) {
        obj = &lbl_1_bss_1340[i * 0x28];
        for (j = 0; j < 0x28; j++) {
            if (j == 0) {
                obj->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_0[i + 6], 0x10000000, 2));
            } else {
                obj->model = Hu3DModelLink(lbl_1_bss_1340[i * 0x28].model);
            }
            Hu3DModelAttrSet(obj->model, 1);
            Hu3DModelCameraSet(obj->model, 1);
            obj->pos.z = -100.0f;
            obj++;
        }
        obj = &lbl_1_bss_1340[(i * 0xC) + 0xA0];
        for (j = 0; j < 0xC; j++) {
            if (j == 0) {
                obj->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_0[i + 0xA], 0x10000000, 2));
            } else {
                obj->model = Hu3DModelLink(lbl_1_bss_1340[(i * 0xC) + 0xA0].model);
            }
            Hu3DModelAttrSet(obj->model, 1);
            Hu3DModelCameraSet(obj->model, 2);
            obj->pos.z = -100.0f;
            obj++;
        }
        obj = &lbl_1_bss_1200[i * 2];
        for (j = 0; j < 2; j++) {
            obj->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_0[i + 0xA], 0x10000000, 2));
            Hu3DModelCameraSet(obj->model, 1);
            Hu3DModelAttrSet(obj->model, 1);
            obj++;
        }
        obj = &lbl_1_bss_490[i];
        obj->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_0[i + 6], 0x10000000, 2));
        Hu3DModelCameraSet(obj->model, 1);
        Hu3DModelAttrSet(obj->model, 1);
        obj->pos.z = -100.0f;
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 6; j++) {
            mdl = &lbl_1_bss_84[(i * 6) + j + 0x13];
            if (i == 0) {
                mdl->id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_0[j + 0x19], 0x10000000, 2));
            } else {
                mdl->id = Hu3DModelLink(lbl_1_bss_4794.models[j + 0x13].id);
            }
            Hu3DModelAttrSet(mdl->id, 1);
            Hu3DModelCameraSet(mdl->id, 1);
            Hu3DModelPosSet(mdl->id, (i * 0x3FC) - 0x1FE, 0.0f, 0.0f);
            mdl->pos.x = (i * 0x3FC) - 0x1FE;
            mdl->pos.y = 0.0f;
            mdl->pos.z = 0.0f;
            lbl_1_bss_36E2[i][j] = Hu3DTexScrollCreate(mdl->id, lbl_1_data_354);
        }
    }
    for (i = 0; i < 4; i++) {
        mdl = &lbl_1_bss_84[i + 0x1F];
        mdl->id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_0[i + 0x1F], 0x10000000, 2));
        Hu3DModelAttrSet(mdl->id, 1);
        Hu3DModelCameraSet(mdl->id, 1);
        Hu3DModelPosSet(mdl->id, 0.0f, 0.0f, 0.0f);
        mdl->pos.x = 0.0f;
        mdl->pos.y = 0.0f;
        mdl->pos.z = 0.0f;
    }
    for (i = 0; i < 2; i++) {
        mdl = &lbl_1_bss_84[i + 0x23];
        mdl->id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_0[lbl_1_bss_4794.chars[i].charNo + 0x23], 0x10000000, 2));
        Hu3DModelAttrSet(mdl->id, 1);
        Hu3DModelCameraSet(mdl->id, 1);
        Hu3DModelPosSet(mdl->id, (i * 0x320) - 0x190, 0.0f, 0.0f);
        mdl->pos.x = (i * 0x320) - 0x190;
        mdl->pos.y = 0.0f;
        mdl->pos.z = 0.0f;
        lbl_1_bss_36E2[i][6] = Hu3DTexScrollCreate(mdl->id, lbl_1_data_320[lbl_1_bss_4794.chars[i].charNo]);
    }
    for (i = 0; i < 2; i++) {
        fn_1_132C(i);
    }
}

void fn_1_132C(s32 arg0)
{
    s32 i;
    s32 j;
    s32 k;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 5; j++) {
            for (k = 0; k < 0xA; k++) {
                lbl_1_bss_33C2[arg0][(i * 0x32) + ((j * 0xA) + k)] = -1;
                if (j != 0 || k < 4) {
                    lbl_1_bss_33C2[arg0][(i * 0x32) + ((j * 0xA) + k)] = espEntry(lbl_1_data_C0[k + (i * 0xA)], 0xD2, 0);
                    espDispOff(lbl_1_bss_33C2[arg0][(i * 0x32) + ((j * 0xA) + k)]);
                    espDrawNoSet(lbl_1_bss_33C2[arg0][(i * 0x32) + ((j * 0xA) + k)], 0);
                }
            }
        }
    }
    lbl_1_bss_33C0 = espEntry(0x6E0058, 0xF, 0);
    espDispOff(lbl_1_bss_33C0);
}

void fn_1_14D0(M552Obj *obj)
{
    HU3DMODEL *model;
    HSFMATERIAL *material;
    s32 i;

    model = &Hu3DData[obj->model];
    material = model->hsf->material;
    obj->count += 1;
    for (i = 0; i < model->hsf->materialNum; i++) {
        material->invAlpha = (f32)obj->count / 20.0f;
        material++;
    }
    if (obj->count == 0x14) {
        obj->state = 0;
    }
}

void fn_1_15A4(void)
{
    M552Mdl *mdl;
    M552Obj *obj;
    s32 i;

    mdl = lbl_1_bss_84;
    obj = lbl_1_bss_1340;
    for (i = 0; i < 0x25; i++) {
        if (mdl->id >= 0) {
            Hu3DModelPosSetV(mdl->id, &mdl->pos);
            Hu3DModelRotSetV(mdl->id, &mdl->rot);
        }
        mdl++;
    }
    for (i = 0; i < 0xD0; i++) {
        if (obj->state != 0) {
            Hu3DModelPosSetV(obj->model, &obj->pos);
            Hu3DModelRotSetV(obj->model, &obj->rot);
            if (obj->state != 3) {
                obj->count += 1;
            }
            if (obj->count == 0x64) {
                Hu3DMotionTimeSet(obj->model, 0.0f);
                obj->count = 0;
            }
        }
        obj++;
    }
    obj = lbl_1_bss_710;
    for (i = 0; i < 0x46; i++) {
        if (obj->state != 0) {
            Hu3DModelPosSetV(obj->model, &obj->pos);
            Hu3DModelRotSet(obj->model, 0.0f, 0.0f, 0.0f);
            if (obj->state == 2) {
                fn_1_14D0(obj);
                if (obj->state == 0) {
                    Hu3DModelKill(obj->model);
                    obj->count = 0;
                }
            }
        }
        obj++;
    }
    obj = lbl_1_bss_530;
    for (i = 0; i < 0xC; i++) {
        if (obj->state != 0) {
            Hu3DModelPosSetV(obj->model, &obj->pos);
            Hu3DModelRotSetV(obj->model, &obj->rot);
        }
        obj++;
    }
}

void fn_1_1810(s32 arg0, f32 alpha)
{
    HU3DMODEL *model;
    HSFMATERIAL *material;
    s32 i;

    model = &Hu3DData[lbl_1_bss_4794.models[arg0].id];
    material = model->hsf->material;
    for (i = 0; i < model->hsf->materialNum; i++) {
        material->invAlpha = alpha;
        material++;
    }
}

void fn_1_188C(void)
{
    lbl_1_bss_8 = Hu3DHookFuncCreate(fn_1_1914);
    Hu3DModelLayerSet(lbl_1_bss_8, 5);
    Hu3DModelCameraSet(lbl_1_bss_8, 1);
}

void fn_1_18E8(void)
{
    Hu3DModelKill(lbl_1_bss_8);
}

void fn_1_1914(HU3DMODEL *model, Mtx *mtx)
{
    Mtx44 proj;
    GXColor color = { 0, 0, 0, 0 };

    color.a = lbl_1_bss_A;
    GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    C_MTXOrtho(proj, 0.0f, 480.0f, 0.0f, 640.0f, 0.0f, -16777215.0f);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);
    PSMTXIdentity(*mtx);
    GXLoadPosMtxImm(*mtx, GX_PNMTX0);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_SPEC);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_SPEC);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3s16(0, 0, 0);
    GXPosition3s16(640, 0, 0);
    GXPosition3s16(640, 480, 0);
    GXPosition3s16(0, 480, 0);
}
