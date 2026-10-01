#include "game/charman.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/frand.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mg/score.h"
#include "game/mg/timer.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/printfunc.h"
#include "game/wipe.h"
#include "math.h"

#ifndef __MWERKS__
#include "game/audio.h"
#endif

typedef struct M529Score_s {
    s32 unk0;
    void *unk4;
    void *unk8;
    void *unkC;
    s16 unk10;
    s16 unk12;
    s32 unk14;
    float unk18;
    s32 unk1C;
} M529Score;

typedef struct M529Player_s {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    float unk68;
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    OMOBJ *unk78;
    s32 unk7C;
    s32 unk80;
    Vec unk84;
    float unk90;
    float unk94;
    float unk98;
    float unk9C;
    float unkA0;
    float unkA4;
    float unkA8;
    float unkAC;
    float unkB0;
    float unkB4;
    s32 unkB8;
    s32 unkBC;
    s32 unkC0;
    s32 unkC4;
    s32 unkC8;
    s32 unkCC;
} M529Player;

typedef struct M529ListNode_s {
    u16 unk0;
    s16 unk2;
    struct M529ListNode_s *prev;
    struct M529ListNode_s *next;
} M529ListNode;

typedef struct M529ModelCacheEntry_s {
    u32 dataNum;
    s16 modelId;
    s16 pad2;
} M529ModelCacheEntry;

float Hu3DMotionMaxTimeGet(HU3DMODELID modelId, s16 (*clusterMaxTimeTable)[10]);

void ObjectSetup(void);
void fn_1_5E0(OMOBJ *obj);
void fn_1_B98(OMOBJ *obj);
void fn_1_1780(OMOBJ *obj);
void fn_1_187C(OMOBJ *obj);
void fn_1_1C8C(OMOBJ *obj);
void fn_1_2714(OMOBJ *obj);
void fn_1_2B70(OMOBJ *obj);
void fn_1_2BEC(OMOBJ *obj);
void fn_1_3CE0(void);
void fn_1_4048(void);
void fn_1_4290(OMOBJ *obj);
void fn_1_4370(OMOBJ *obj);
s32 fn_1_444C(OMOBJ *obj);
s32 fn_1_4488(OMOBJ *obj);
void fn_1_44C4(OMOBJ *obj);
void fn_1_4780(OMOBJ *obj);
s32 fn_1_4E48(float arg0, float arg1, float arg2);
s32 fn_1_4E70(M529Player *player);
u16 fn_1_4F10(OMOBJ *obj);
u16 fn_1_500C(OMOBJ *obj);
u16 fn_1_5084(OMOBJ *obj);
u16 fn_1_5170(OMOBJ *obj);
s32 fn_1_51B4(s32 arg0, s32 arg1);
void fn_1_5204(OMOBJ *obj);
void fn_1_553C(OMOBJ *obj);
void fn_1_5680(s16 arg0, u32 arg1);
void fn_1_57BC(void);
void fn_1_595C(void);
s16 fn_1_5960(s16 arg0);
s32 fn_1_59BC(s16 arg0);
void fn_1_5A18(void);
void fn_1_5A84(OMCAMERAVIEW *arg0, Vec *arg1);
void fn_1_5F74(void);
s16 fn_1_6334(s32 arg0);
void fn_1_63E4(void);
void fn_1_64F4(void);
void fn_1_6518(u16 arg0, s16 arg1);
s16 fn_1_65A0(s16 arg0);
void fn_1_6644(void);
s16 fn_1_669C(u32 arg0);
void fn_1_67B0(Vec *arg0, Vec *arg1, Vec *arg2, float farg0);
void fn_1_690C(Vec *arg0, Vec *arg1, Vec *arg2);
s32 fn_1_6998(Vec *arg0, float farg0);
s32 fn_1_6CCC(Vec *arg0, float farg0);
float fn_1_70B0(float farg0, float farg1, float farg2);
void fn_1_71B4(s32 arg0);
void fn_1_7220(s16 arg0, char *arg1, Vec *arg2);
s32 fn_1_7278(s32 arg0, s32 arg1);
s32 fn_1_72B4(Vec *arg0, Vec *arg1, float farg0, float farg1);
s32 fn_1_7314(Vec *arg0, s32 arg1, s32 arg2);
s32 fn_1_73D8(Vec *arg0, s32 arg1, s32 arg2);

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

s32 lbl_1_data_0 = -1;
GXColor lbl_1_data_4 = { 0xFF, 0xFF, 0xFF, 0xFF };
u32 lbl_1_data_8[8] = { 0x008C0000, 0x008C0001, 0x008C0002, 0x00870043, 0x008C0006, 0x008C0007, 0x008C0038, 0x008C003A };

Vec lbl_1_data_28[4] = {
    { -470.0f, 0.0f, 300.0f },
    { -145.0f, 0.0f, 300.0f },
    { 145.0f, 0.0f, 300.0f },
    { 470.0f, 0.0f, 300.0f },
};

Vec lbl_1_data_58[4] = {
    { -265.0f, 0.0f, 500.0f },
    { -115.0f, 0.0f, 500.0f },
    { 115.0f, 0.0f, 500.0f },
    { 265.0f, 0.0f, 500.0f },
};

Vec lbl_1_data_88[4] = {
    { -470.0f, 250.0f, 300.0f },
    { -145.0f, 0.0f, 300.0f },
    { 145.0f, 250.0f, 300.0f },
    { 470.0f, 0.0f, 300.0f },
};

Vec lbl_1_data_B8[4] = {
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 270.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 270.0f, 0.0f },
};

Vec lbl_1_data_E8[2] = {
    { -470.0f, 0.0f, 300.0f },
    { 145.0f, 0.0f, 300.0f },
};

Vec lbl_1_data_100 = { 0.0f, 4.0f, 200.0f };
Vec lbl_1_data_10C = { -1300.0f, -350.0f, -2500.0f };
Vec lbl_1_data_118 = { 0.0f, 30.0f, 0.0f };

Vec lbl_1_data_124[4] = {
    { -600.0f, 600.0f, 600.0f },
    { 0.0f, 600.0f, 600.0f },
    { 0.0f, 0.0f, 600.0f },
    { -600.0f, 0.0f, 600.0f },
};

Vec lbl_1_data_154[4] = {
    { 0.0f, 600.0f, 600.0f },
    { 600.0f, 600.0f, 600.0f },
    { 600.0f, 0.0f, 600.0f },
    { 0.0f, 0.0f, 600.0f },
};

Vec lbl_1_data_184[4] = {
    { -350.0f, 200.0f, 600.0f },
    { 0.0f, 200.0f, 600.0f },
    { 0.0f, 50.0f, 600.0f },
    { -350.0f, 50.0f, 600.0f },
};

Vec lbl_1_data_1B4[4] = {
    { 0.0f, -135.0f, 0.0f },
    { 0.0f, 135.0f, 0.0f },
    { 0.0f, 45.0f, 0.0f },
    { 0.0f, -45.0f, 0.0f },
};

Vec lbl_1_data_1E4[4] = {
    { 0.0f, 200.0f, 600.0f },
    { 350.0f, 200.0f, 600.0f },
    { 350.0f, 50.0f, 600.0f },
    { 0.0f, 50.0f, 600.0f },
};

Vec lbl_1_data_214[4] = {
    { 0.0f, -135.0f, 0.0f },
    { 0.0f, 135.0f, 0.0f },
    { 0.0f, 45.0f, 0.0f },
    { 0.0f, -45.0f, 0.0f },
};

u32 lbl_1_data_244[14] = { 0x00570000, 0x00570001, 0x00570003, 0x00570002, 0x00570004, 0x00570005, 0x0057000A,
    0x00570006, 0x00570007, 0x00570008, 0x00570009, 0x0057000E, 0x0057000E, 0x0057000C };

OMCAMERAVIEW lbl_1_data_27C = { { 0.0f, 10.0f, -100.0f }, { 1.0f, 0.0f, 0.0f }, 0.0f };
Vec lbl_1_data_294 = { -80.0f, 120.0f, -500.0f };
OMCAMERAVIEW lbl_1_data_2A0 = { { 0.0f, 500.0f, 1760.0f }, { 1.0f, 0.0f, 0.0f }, 0.0f };
Vec lbl_1_data_2B8 = { 0.0f, 200.0f, -500.0f };
OMCAMERAVIEW lbl_1_data_2C4 = { { 2050.0f, 1070.0f, 2318.0f }, { 1.0f, 0.0f, 0.0f }, 0.0f };
Vec lbl_1_data_2DC = { -100.0f, 200.0f, 300.0f };
OMCAMERAVIEW lbl_1_data_2E8 = { { 0.0f, 3610.0f, 4689.0f }, { 1.0f, 0.0f, 0.0f }, 0.0f };
Vec lbl_1_data_300 = { 0.0f, 50.0f, -100.0f };

OMCAMERAVIEW lbl_1_data_30C = { { -150.0f, 120.0f, -500.0f }, { 0.0f, 11.4f, 0.0f }, 465.0f };
OMCAMERAVIEW lbl_1_data_328 = { { 0.0f, 200.0f, -500.0f }, { -7.7f, 0.0f, 0.0f }, 2240.2f };
OMCAMERAVIEW lbl_1_data_344 = { { -100.0f, 200.0f, 300.0f }, { -19.4f, 41.0f, 0.0f }, 2775.7f };
OMCAMERAVIEW lbl_1_data_360 = { { 0.0f, 50.0f, -100.0f }, { -18.3f, 0.0f, 0.0f }, 2768.9f };

Vec lbl_1_data_37C = { 0.0f, 4000.0f, 6000.0f };
Vec lbl_1_data_388 = { 0.0f, 0.0f, 0.0f };
s32 lbl_1_data_394 = -1;
Vec lbl_1_data_398 = { 0.0f, 4000.0f, 300.0f };
Vec lbl_1_data_3A4 = { 0.0f, 1.0f, 0.0f };
Vec lbl_1_data_3B0 = { 0.0f, 0.0f, 0.0f };

char lbl_1_data_3BC[] = "******* M529 ObjectSetup *********\n";
char lbl_1_data_3E0[0x20] = "*** player mode error(%d)!!\n";
char lbl_1_data_424[0x1C] = "*** main mode error(%d)!!\n";

char lbl_1_data_46C[] = "static OBJ_CAMINFO CamK = {\n";
char lbl_1_data_489[] = "\t{ %.01ff, %.01ff, %.01ff },\t/* center */\n";
char lbl_1_data_4B4[] = "\t{ %.01ff, %.01ff, %.01ff },\t/* rot */\n";
char lbl_1_data_4DC[] = "\t%.01ff,\t/* zoom */\n";
char lbl_1_data_4F1[] = "};\n";
char lbl_1_data_4F5[] = "\n";
char lbl_1_data_4F7[] = "Center X:%.02f Y:%.02f Z:%.02f";
char lbl_1_data_516[] = "CRot X:%.02f Y:%.02f Z:%.02f";
char lbl_1_data_533[] = "CZoom:%.02f";

s32 lbl_1_bss_0;
s32 lbl_1_bss_4;
s32 lbl_1_bss_8;
s32 lbl_1_bss_C;
s32 lbl_1_bss_10;
MGTIMER *lbl_1_bss_14;
s32 lbl_1_bss_18;
s32 lbl_1_bss_1C;
s32 lbl_1_bss_20[2];
s32 lbl_1_bss_28;
s32 lbl_1_bss_2C;
s16 lbl_1_bss_30;
s16 lbl_1_bss_32;
s16 lbl_1_bss_34;
float lbl_1_bss_38;
float lbl_1_bss_3C;
s16 lbl_1_bss_40;
float lbl_1_bss_44[4];
Vec lbl_1_bss_54[4];
s16 lbl_1_bss_84[4];
float lbl_1_bss_8C[4];
Vec lbl_1_bss_9C[4];
s16 lbl_1_bss_CC[4];
s32 lbl_1_bss_D4;
s16 lbl_1_bss_D8;
s32 lbl_1_bss_DC;
s32 lbl_1_bss_E0;
s32 lbl_1_bss_E4;
s32 lbl_1_bss_E8;
s16 lbl_1_bss_EC[14];
float lbl_1_bss_108;
OMOBJ *lbl_1_bss_10C;
OMOBJ *lbl_1_bss_110;
OMOBJ *lbl_1_bss_114;
OMOBJ *lbl_1_bss_118;
OMOBJ **lbl_1_bss_11C;
M529ModelCacheEntry lbl_1_bss_120[0x20];
float lbl_1_bss_220;
s32 lbl_1_bss_224;
s32 lbl_1_bss_228;
s16 lbl_1_bss_22C;
s16 lbl_1_bss_22E;
s32 lbl_1_bss_230;
Vec lbl_1_bss_234[4];
Vec lbl_1_bss_264[4];
Vec lbl_1_bss_294[4];
Vec lbl_1_bss_2C4[4];
Vec lbl_1_bss_2F4;
Vec lbl_1_bss_300;
Vec lbl_1_bss_30C;
Vec lbl_1_bss_318;
HUPROCESS *lbl_1_bss_324;
M529ListNode lbl_1_bss_328;

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
    OMOBJ *obj;

    lbl_1_bss_22E = -1;
    lbl_1_bss_22C = -1;
    lbl_1_bss_228 = -1;
    lbl_1_bss_20[0] = 0;
    lbl_1_bss_20[1] = 0;
    lbl_1_bss_1C = -1;
    lbl_1_bss_DC = 0;
    OSReport("******* M529 ObjectSetup *********\n");
    lbl_1_bss_324 = omInitObjMan(0xC8, 0x2000);
    omGameSysInit(lbl_1_bss_324);
    lbl_1_bss_230 = 0;
    lbl_1_bss_18 = GWMgRecordGet(6);
    lbl_1_bss_220 = -1.0f;
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(1, 20.0f, 20.0f, 30000.0f, 1.2f);
    omCameraViewSet(&lbl_1_data_30C);
    lbl_1_bss_10C = omAddObjEx(lbl_1_bss_324, 0x7FDA, 0, 0, -1, omOutView);
    lbl_1_bss_10C->work[0] = 1;
    lbl_1_bss_28 = Hu3DGLightCreate(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    Hu3DGLightPosAimSetV(lbl_1_bss_28, &lbl_1_data_37C, &lbl_1_data_388);
    Hu3DGLightStaticSet(lbl_1_bss_28, 1);
    Hu3DGLightColorSet(lbl_1_bss_28, lbl_1_data_4.r, lbl_1_data_4.g, lbl_1_data_4.b, lbl_1_data_4.a);
    Hu3DGLightInfinitytSet(lbl_1_bss_28);
    Hu3DShadowCreate(20.0f, 20.0f, 30000.0f);
    Hu3DShadowColSet(0, 0, 0x40);
    Hu3DShadowTPLvlSet(0.8f);
    Hu3DShadowPosSet(&lbl_1_data_398, &lbl_1_data_3A4, &lbl_1_data_3B0);
    omMakeGroupEx(lbl_1_bss_324, 0, 4);
    lbl_1_bss_11C = omGetGroupMemberListEx(lbl_1_bss_324, 0);
    for (i = 0; i < 4; i++) {
        obj = omAddObjEx(lbl_1_bss_324, 0x64, 1, 8, 0, fn_1_5E0);
        lbl_1_bss_11C[i] = obj;
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M529Player), HU_MEMNUM_OVL);
        obj->work[0] = i;
    }
    lbl_1_bss_118 = omAddObjEx(lbl_1_bss_324, 0x65, 0xE, 0, -1, fn_1_1C8C);
    lbl_1_bss_118->work[0] = 0x3E8;
    lbl_1_bss_114 = omAddObjEx(lbl_1_bss_324, 0x67, 8, 0, -1, fn_1_2714);
    lbl_1_bss_114->work[0] = 0;
    lbl_1_bss_114->work[1] = 0;
    lbl_1_bss_108 = 0.0f;
    lbl_1_bss_110 = omAddObjEx(lbl_1_bss_324, 0x66, 0, 0, -1, fn_1_5204);
    lbl_1_bss_14 = MgTimerCreate(0);
    MgTimerParamSet(lbl_1_bss_14, 0x708, 0, 0);
    lbl_1_bss_10 = -1;
    lbl_1_bss_C = -1;
    lbl_1_bss_8 = -1;
}

void fn_1_5E0(OMOBJ *obj)
{
    s32 i;
    M529Player *temp_r31 = obj->data;
    M529Player *var_r29;
    float temp_f0;
    float temp_f0_2;
    float temp_f0_3;

    temp_r31->unk14 = obj->work[0];
    temp_r31->unk28 = GwPlayerConf[temp_r31->unk14].charNo;
    temp_r31->unk24 = GwPlayerConf[temp_r31->unk14].grp;
    temp_r31->unk18 = GwPlayerConf[temp_r31->unk14].grp;
    temp_r31->unk2C = GwPlayerConf[temp_r31->unk14].padNo;
    temp_r31->unk8 = 0x7D0;
    temp_r31->unkC = 0;
    temp_r31->unk4 = 0;
    temp_r31->unk10 = 0;
    temp_r31->unk38 = 0;
    temp_r31->unk3C = 0;
    temp_r31->unk40 = 0;
    temp_r31->unk44 = 0;
    temp_r31->unk48 = 0;
    temp_r31->unk4C = 0;
    temp_r31->unk50 = 0x1E;
    temp_r31->unk54 = 0;
    temp_r31->unk58 = 0;
    temp_r31->unk5C = 0;
    temp_r31->unk60 = 0;
    temp_r31->unk64 = 0;
    temp_r31->unk68 = 0.0f;
    temp_r31->unk7C = -1;
    temp_r31->unk80 = 0;
    temp_r31->unkB4 = 0.0f;
    temp_r31->unkB0 = 0.0f;
    temp_r31->unkAC = 0.0f;
    temp_r31->unk98 = 0.0f;
    temp_r31->unk94 = 0.0f;
    temp_r31->unk90 = 0.0f;
    temp_r31->unk74 = 0x3C;
    temp_r31->unk70 = 0x3C;
    temp_r31->unk78 = NULL;
    temp_r31->unk6C = 0;
    switch (temp_r31->unk18) {
    case 0:
        temp_r31->unk0 = 0xBB8;
        temp_r31->unk20 = 1;
        break;
    case 1:
        temp_r31->unk0 = 0xBB9;
        temp_r31->unk20 = 0;
        break;
    case 2:
        temp_r31->unk0 = 0xBBA;
        temp_r31->unk20 = 3;
        break;
    case 3:
        temp_r31->unk0 = 0xBBB;
        temp_r31->unk20 = 2;
        break;
    }
    for (i = 0; i < 4; i++) {
        var_r29 = lbl_1_bss_11C[i]->data;
        if ((i != temp_r31->unk14) && (temp_r31->unk20 == GwPlayerConf[i].grp)) {
            temp_r31->unk1C = i;
        }
    }
    temp_r31->unkB8 = GwPlayerConf[temp_r31->unk14].dif;
    temp_r31->unkC4 = 0;
    switch (temp_r31->unkB8) {
    case 0:
        temp_r31->unkC8 = 0x23;
        temp_r31->unkCC = 0x37;
        temp_r31->unkC4 = temp_r31->unkC8;
        temp_r31->unkC0 = 0xA;
        temp_r31->unkBC = 0xA;
        break;
    case 1:
        temp_r31->unkC8 = 0x14;
        temp_r31->unkCC = 0x32;
        temp_r31->unkC4 = temp_r31->unkC8;
        temp_r31->unkC0 = 0x14;
        temp_r31->unkBC = 5;
        break;
    case 2:
        temp_r31->unkC8 = 8;
        temp_r31->unkCC = 0x28;
        temp_r31->unkC4 = temp_r31->unkC8;
        temp_r31->unkC0 = 0xF;
        temp_r31->unkBC = 0x28;
        break;
    case 3:
        temp_r31->unkC8 = 1;
        temp_r31->unkCC = 0x19;
        temp_r31->unkC4 = temp_r31->unkC8;
        temp_r31->unkC0 = 0x32;
        temp_r31->unkBC = 0x32;
        break;
    }
    *obj->mdlId = CharModelCreate(temp_r31->unk28, 2);
    Hu3DModelShadowSet(*obj->mdlId);
    CharModelStepFxSet(temp_r31->unk28, 0);
    for (i = 0; i < 8; i++) {
        obj->mtnId[i] = CharMotionCreate(temp_r31->unk28, lbl_1_data_8[i]);
        CharMotionSet(temp_r31->unk28, obj->mtnId[i]);
    }
    CharModelAttrSet(temp_r31->unk28, 1);
    CharModelAttrSet(temp_r31->unk28, 0x40000001);
    Hu3DModelLayerSet(*obj->mdlId, 3);
    CharMotionSet(temp_r31->unk28, obj->mtnId[0]);
    CharEffectLayerSet(2);
    if ((temp_r31->unk0 == 0xBB8) || (temp_r31->unk0 == 0xBBA)) {
        temp_r31->unkA4 = 0.0f;
    } else {
        temp_r31->unkA4 = 270.0f;
        CharMotionSet(temp_r31->unk28, obj->mtnId[3]);
        CharMotionSpeedSet(temp_r31->unk28, 0.0f);
        CharMotionTimeSet(temp_r31->unk28, 0.0f);
    }
    temp_r31->unkA8 = 0.0f;
    temp_r31->unk84.x = 0.0f;
    temp_r31->unk84.y = 0.0f;
    temp_r31->unk84.z = 0.0f;
    temp_r31->unk9C = 0.0f;
    temp_f0 = lbl_1_data_28[temp_r31->unk18].x;
    temp_r31->unk84.x = temp_f0;
    obj->trans.x = temp_f0;
    temp_f0_2 = lbl_1_data_28[temp_r31->unk18].y;
    temp_r31->unk84.y = temp_f0_2;
    obj->trans.y = temp_f0_2;
    temp_f0_3 = lbl_1_data_28[temp_r31->unk18].z;
    temp_r31->unk84.z = temp_f0_3;
    obj->trans.z = temp_f0_3;
    omSetTra(obj, temp_r31->unk84.x, temp_r31->unk84.y, temp_r31->unk84.z);
    omSetRot(obj, 0.0f, temp_r31->unkA4, 0.0f);
    CharModelAttrReset(temp_r31->unk28, 1);
    CharMotionDataClose(temp_r31->unk28);
    obj->objFunc = fn_1_B98;
}

void fn_1_B98(OMOBJ *obj)
{
    Vec sp20;
    Vec sp14;
    Vec sp8;
OMOBJ *temp_r30;
    M529Player *temp_r31 = obj->data;
    float temp_f0;
    float temp_f0_2;
    float temp_f0_3;
    float temp_f26;
    float temp_f27;
    float var_f28;
    float var_f29;
    float var_f30;
    float var_f31;
    s32 temp_r0;
    s32 temp_r3;

    switch (temp_r31->unk8) {
    case 0x7D0:
        if (lbl_1_bss_118->work[0] >= 0x3EC) {
            temp_r31->unk8 += 1;
        }
        break;
    case 0x7D1:
        if (lbl_1_bss_118->work[0] == 0x3EC) {
            if ((temp_r31->unk0 == 0xBB8) || (temp_r31->unk0 == 0xBBA)) {
                if (GwPlayerConf[temp_r31->unk14].type != 0) {
                    temp_r31->unk30 = fn_1_500C(obj);
                    temp_r31->unk34 = fn_1_4F10(obj);
                } else {
                    temp_r31->unk30 = HuPadBtn[temp_r31->unk2C];
                    temp_r31->unk34 = HuPadBtnDown[temp_r31->unk2C];
                }
                fn_1_187C(obj);
            } else {
                if (GwPlayerConf[temp_r31->unk14].type != 0) {
                    temp_r31->unk34 = 0;
                    temp_r31->unk34 = fn_1_5084(obj);
                } else {
                    temp_r31->unk34 = 0;
                    temp_r31->unk34 = HuPadBtnDown[temp_r31->unk2C];
                }
                if ((temp_r31->unk34 & 0x40) && !(temp_r31->unk34 & 0x20) && (temp_r31->unk34 != 0)) {
                    if (temp_r31->unk34 & 0x40) {
                        if ((temp_r31->unk7C == 1) || (temp_r31->unk7C == -1)) {
                            temp_r31->unk74 = lbl_1_bss_E8 - (s32) temp_r31->unk78;
                            temp_r31->unk78 = (OMOBJ *) lbl_1_bss_E8;
                            temp_r31->unk7C = 0;
                            fn_1_1780(obj);
                        }
                    } else if ((temp_r31->unk34 & 0x20) && ((temp_r31->unk7C == 0) || (temp_r31->unk7C == -1))) {
                        temp_r30 = temp_r31->unk78;
                        temp_r31->unk74 = lbl_1_bss_E8 - (s32) temp_r30;
                        temp_r31->unk78 = (OMOBJ *) lbl_1_bss_E8;
                        temp_r31->unk7C = 1;
                        fn_1_1780(obj);
                    }
                }
                if ((lbl_1_bss_E8 - (s32) temp_r31->unk78) >= 0x3D) {
                    temp_r31->unk80 = 1;
                }
                fn_1_4780(obj);
            }
        }
        if (lbl_1_bss_118->work[0] >= 0x3ED) {
            temp_r31->unk8 = 0x7D2;
        }
        break;
    case 0x7D2:
        if ((temp_r31->unk0 == 0xBB8) || (temp_r31->unk0 == 0xBBA)) {
            CharModelAttrReset(temp_r31->unk28, 1);
            if (fn_1_444C(obj) != 0) {
                fn_1_44C4(obj);
            }
            if (temp_r31->unk4 != 0) {
                temp_r31->unk4 = 0;
                CharMotionShiftSet(temp_r31->unk28, obj->mtnId[temp_r31->unk4], 0.0f, 8.0f, 0x40000001);
            }
        } else if (fn_1_4488(obj) != 0) {
            fn_1_4780(obj);
        } else if (temp_r31->unk0 == 0xBB9) {
            if (lbl_1_bss_10 != -1) {
                HuAudFXStop(lbl_1_bss_10);
                lbl_1_bss_10 = -1;
            }
        } else if (lbl_1_bss_C != -1) {
            HuAudFXStop(lbl_1_bss_C);
            lbl_1_bss_C = -1;
        }
        if (lbl_1_bss_118->work[0] >= 0x3EE) {
            temp_r31->unk8 = 0x7D3;
            temp_r31->unk4 = 0;
            CharMotionShiftSet(temp_r31->unk28, obj->mtnId[0], 0.0f, 8.0f, 0x40000001);
        }
        break;
    case 0x7D3:
        temp_f27 = temp_r31->unkA4;
        var_f31 = fmod(0.0f - temp_f27, 360.0);
        if (var_f31 < 0.0f) {
            var_f31 += 360.0f;
        }
        if (var_f31 > 180.0f) {
            var_f31 -= 360.0f;
        }
        var_f29 = fmod(temp_f27 + var_f31 * 0.5f, 360.0);
        if (var_f29 < 0.0f) {
            var_f29 += 360.0f;
        }
        temp_r31->unkA4 = var_f29;
        if (lbl_1_bss_118->work[0] >= 0x3EF) {
            temp_r31->unk8 = 0x7D4;
        }
        break;
    case 0x7D4:
        if (temp_r31->unk4 != 2) {
            temp_r31->unk4 = 2;
            CharMotionShiftSet(temp_r31->unk28, obj->mtnId[temp_r31->unk4], 0.0f, 8.0f, 0x40000001);
        }
        if (lbl_1_bss_118->work[0] == 0x3F0) {
            temp_r31->unk8 = 0x7D5;
            temp_r31->unk4 = 0;
            CharMotionShiftSet(temp_r31->unk28, obj->mtnId[0], 0.0f, 8.0f, 0x40000001);
        } else if (lbl_1_bss_118->work[0] == 0x3F1) {
            temp_r31->unk8 = 0x7D6;
            temp_r31->unk4 = 0;
            CharMotionShiftSet(temp_r31->unk28, obj->mtnId[0], 0.0f, 8.0f, 0x40000001);
        } else {
            temp_r31->unkAC = (lbl_1_data_58[temp_r31->unk18].x - lbl_1_data_28[temp_r31->unk18].x) / 80.0f;
            temp_r31->unkB0 = (lbl_1_data_58[temp_r31->unk18].y - lbl_1_data_28[temp_r31->unk18].y) / 80.0f;
            temp_r31->unkB4 = (lbl_1_data_58[temp_r31->unk18].z - lbl_1_data_28[temp_r31->unk18].z) / 80.0f;
            temp_r31->unk84.x += temp_r31->unkAC;
            temp_r31->unk84.y += temp_r31->unkB0;
            temp_r31->unk84.z += temp_r31->unkB4;
        }
        break;
    case 0x7D5:
        if (lbl_1_bss_118->work[0] == 0x3F1) {
            temp_r31->unk8 = 0x7D6;
        }
        break;
    case 0x7D6:
        if (lbl_1_bss_118->work[0] >= 0x3F1) {
            temp_r31->unk8 = 0x7D7;
            if ((temp_r31->unk0 == 0xBB9) || (temp_r31->unk0 == 0xBB8)) {
                if (lbl_1_bss_1C == 0) {
                    temp_r31->unk4 = 4;
                    if (lbl_1_bss_4 == -1) {
                        lbl_1_bss_4 = HuAudFXPlay(0x794);
                    }
                } else {
                    temp_r31->unk4 = 5;
                    if ((lbl_1_bss_1C == 2) && (temp_r31->unk14 == 0)) {
                        Hu3D3Dto2D(&temp_r31->unk84, 1, &sp20);
                        CharFXPlayPan(temp_r31->unk28, 0x200, 32.0f + sp20.x / 576.0f * 64.0f);
                    }
                }
                CharMotionShiftSet(temp_r31->unk28, obj->mtnId[temp_r31->unk4], 0.0f, 8.0f, 0);
            } else {
                if (lbl_1_bss_1C == 1) {
                    temp_r31->unk4 = 4;
                    if (lbl_1_bss_0 == -1) {
                        lbl_1_bss_0 = HuAudFXPlay(0x795);
                    }
                } else {
                    temp_r31->unk4 = 5;
                    if (lbl_1_bss_1C == 2) {
                        Hu3D3Dto2D(&temp_r31->unk84, 1, &sp14);
                        CharFXPlayPan(temp_r31->unk28, 0x200, 32.0f + sp14.x / 576.0f * 64.0f);
                    }
                }
                CharMotionShiftSet(temp_r31->unk28, obj->mtnId[temp_r31->unk4], 0.0f, 8.0f, 0);
            }
        }
        break;
    case 0x7D7:
        temp_f26 = temp_r31->unkA4;
        var_f30 = fmod(0.0f - temp_f26, 360.0);
        if (var_f30 < 0.0f) {
            var_f30 += 360.0f;
        }
        if (var_f30 > 180.0f) {
            var_f30 -= 360.0f;
        }
        var_f28 = fmod(temp_f26 + var_f30 * 0.4f, 360.0);
        if (var_f28 < 0.0f) {
            var_f28 += 360.0f;
        }
        temp_r31->unkA4 = var_f28;
        break;
    default:
        OSReport(lbl_1_data_3E0, temp_r31->unk8);
        break;
    }
    if (temp_r31->unk8 > 0x7D2) {
        Hu3D3Dto2D(&temp_r31->unk84, 1, &sp8);
        CharModelVoicePanSet(temp_r31->unk28, 32.0f + sp8.x / 576.0f * 64.0f);
    }
    omSetTra(obj, temp_r31->unk84.x, temp_r31->unk84.y, temp_r31->unk84.z);
    omSetRot(obj, 0.0f, temp_r31->unkA4, 0.0f);
}

void fn_1_1780(OMOBJ *obj)
{
    M529Player *temp_r31 = obj->data;

    if ((temp_r31->unk74 >= 1) && (temp_r31->unk74 <= 7)) {
        temp_r31->unk80 = 5;
        temp_r31->unk54 = 1;
    } else if ((temp_r31->unk74 >= 8) && (temp_r31->unk74 <= 0x14)) {
        temp_r31->unk80 = 4;
        temp_r31->unk54 = 1;
    } else if ((temp_r31->unk74 >= 0x15) && (temp_r31->unk74 <= 0x28)) {
        temp_r31->unk80 = 3;
        temp_r31->unk54 = 1;
    } else if ((temp_r31->unk74 >= 0x29) && (temp_r31->unk74 <= 0x3B)) {
        temp_r31->unk80 = 2;
        temp_r31->unk54 = 1;
    } else {
        temp_r31->unk80 = 1;
        temp_r31->unk54 = 0;
    }
    if ((temp_r31->unk80 != 1) && (temp_r31->unk64 >= temp_r31->unk70)) {
        temp_r31->unk60 = 0;
    }
}

void fn_1_187C(OMOBJ *obj)
{
    Vec sp14;
    Vec sp8;
    M529Player *temp_r31 = obj->data;

    if ((temp_r31->unkC == 0) || (temp_r31->unkC == 2)) {
        if (temp_r31->unkC == 2) {
            temp_r31->unk10 += 1;
            if (temp_r31->unk10 > 0x78) {
                temp_r31->unkC = 0;
                temp_r31->unk10 = 0;
            }
        }
        if ((temp_r31->unk34 & 0x100) && (temp_r31->unk40 == 0)) {
            temp_r31->unk3C = 1;
        }
        fn_1_44C4(obj);
    } else if (temp_r31->unkC == 1) {
        if (temp_r31->unk4 != 7) {
            temp_r31->unk4 = 7;
            CharMotionShiftSet(temp_r31->unk28, obj->mtnId[temp_r31->unk4], 0.0f, 8.0f, 0x40000001);
            if (temp_r31->unk0 == 0xBB8) {
                Hu3DMotionTimeSet(lbl_1_bss_EC[0xB], 0.0f);
                Hu3DMotionSpeedSet(lbl_1_bss_EC[0xB], 1.0f);
                HuAudFXPlay(0x792);
                Hu3D3Dto2D(&temp_r31->unk84, 1, &sp14);
                CharFXPlayPan(temp_r31->unk28, 0x1FD, 32.0f + sp14.x / 576.0f * 64.0f);
            } else {
                Hu3DMotionTimeSet(lbl_1_bss_EC[0xC], 0.0f);
                Hu3DMotionSpeedSet(lbl_1_bss_EC[0xC], 1.0f);
                HuAudFXPlay(0x793);
                Hu3D3Dto2D(&temp_r31->unk84, 1, &sp8);
                CharFXPlayPan(temp_r31->unk28, 0x1FD, 32.0f + sp8.x / 576.0f * 64.0f);
            }
            omVibrate(temp_r31->unk14, 0xC, 0xC, 0);
        }
        temp_r31->unk10 += 1;
        if (temp_r31->unk10 > 0x1E) {
            temp_r31->unkC = 2;
            temp_r31->unk10 = 0;
            if (temp_r31->unk4 != 0) {
                temp_r31->unk4 = 0;
                CharMotionShiftSet(temp_r31->unk28, obj->mtnId[temp_r31->unk4], 0.0f, 8.0f, 0x40000001);
            }
        }
    }
    if ((temp_r31->unkC == 1) || (temp_r31->unkC == 2)) {
        temp_r31->unk4C += 1;
        if ((temp_r31->unk4C & 1) == 0) {
            CharModelAttrSet(temp_r31->unk28, 1);
        } else {
            CharModelAttrReset(temp_r31->unk28, 1);
        }
        if (temp_r31->unk0 == 0xBB8) {
            if (Hu3DMotionEndCheck(lbl_1_bss_EC[0xB])) {
                Hu3DMotionSpeedSet(lbl_1_bss_EC[0xB], 0.0f);
                Hu3DMotionTimeSet(lbl_1_bss_EC[0xB], 0.0f);
            }
        } else if (Hu3DMotionEndCheck(lbl_1_bss_EC[0xC])) {
            Hu3DMotionSpeedSet(lbl_1_bss_EC[0xC], 0.0f);
            Hu3DMotionTimeSet(lbl_1_bss_EC[0xC], 0.0f);
        }
    } else {
        temp_r31->unk4C = 0;
        CharModelAttrReset(temp_r31->unk28, 1);
    }
}

void fn_1_1C8C(OMOBJ *obj)
{
    s32 i;
    s32 j;

    lbl_1_bss_EC[5] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_244[5], HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_EC[5], 1);
    Hu3DModelAttrSet(lbl_1_bss_EC[5], 0x40000001);
    Hu3DModelPosSet(lbl_1_bss_EC[5], 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(lbl_1_bss_EC[5], 0.0f, 0.0f, 0.0f);
    Hu3DMotionSpeedSet(lbl_1_bss_EC[5], 0.02f);
    lbl_1_bss_EC[6] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_244[6], HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_EC[6], 1);
    Hu3DModelAttrSet(lbl_1_bss_EC[6], 0x40000001);
    Hu3DModelRotSet(lbl_1_bss_EC[6], lbl_1_data_118.x, lbl_1_data_118.y, lbl_1_data_118.z);
    Hu3DModelPosSet(lbl_1_bss_EC[6], lbl_1_data_10C.x, lbl_1_data_10C.y, lbl_1_data_10C.z);
    lbl_1_bss_EC[0] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_244[0], HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_EC[0], 1);
    Hu3DModelLayerSet(lbl_1_bss_EC[0], 0);
    Hu3DMotionSpeedSet(lbl_1_bss_EC[0], 0.0f);
    Hu3DMotionTimeSet(lbl_1_bss_EC[0], 0.0f);
    Hu3DModelShadowMapSet(lbl_1_bss_EC[0]);
    Hu3DModelPosSet(lbl_1_bss_EC[0], lbl_1_data_100.x, lbl_1_data_100.y, lbl_1_data_100.z);
    Hu3DModelRotSet(lbl_1_bss_EC[0], 0.0f, 0.0f, 0.0f);
    lbl_1_bss_2C = Hu3DMotionMaxTimeGet(lbl_1_bss_EC[0], (s16 (*)[10]) lbl_1_bss_EC);
    lbl_1_bss_E4 = 0;
    lbl_1_bss_E0 = lbl_1_bss_2C;
    lbl_1_bss_D8 = Hu3DJointMotion(lbl_1_bss_EC[0], HuDataSelHeapReadNum(0x57000B, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_D4 = 0;
    for (i = 0; i < 2; i++) {
        lbl_1_bss_EC[i + 7] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_244[i + 7], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelShadowMapSet(lbl_1_bss_EC[i + 7]);
        Hu3DModelAttrSet(lbl_1_bss_EC[i + 7], 1);
        Hu3DModelLayerSet(lbl_1_bss_EC[i + 7], 1);
        Hu3DModelPosSet(lbl_1_bss_EC[i + 7], lbl_1_data_E8[i].x, lbl_1_data_E8[i].y, lbl_1_data_E8[i].z);
        Hu3DModelRotSet(lbl_1_bss_EC[i + 7], 0.0f, 0.0f, 0.0f);
    }
    for (i = 0; i < 2; i++) {
        lbl_1_bss_EC[i + 0xB] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_244[i + 0xB], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelShadowMapSet(lbl_1_bss_EC[i + 0xB]);
        Hu3DModelAttrSet(lbl_1_bss_EC[i + 0xB], 1);
        Hu3DModelLayerSet(lbl_1_bss_EC[i + 0xB], 2);
        Hu3DModelPosSet(lbl_1_bss_EC[i + 0xB], lbl_1_data_E8[i].x, lbl_1_data_E8[i].y, lbl_1_data_E8[i].z);
        Hu3DModelRotSet(lbl_1_bss_EC[i + 0xB], 0.0f, 0.0f, 0.0f);
        Hu3DMotionSpeedSet(lbl_1_bss_EC[i + 0xB], 0.0f);
        Hu3DMotionTimeSet(lbl_1_bss_EC[i + 0xB], 0.0f);
    }
    for (i = 0; i < 4; i++) {
        lbl_1_bss_EC[i + 1] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_244[i + 1], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelShadowSet(lbl_1_bss_EC[i + 1]);
        Hu3DModelAttrSet(lbl_1_bss_EC[i + 1], 1);
        Hu3DModelAttrSet(lbl_1_bss_EC[i + 1], 0x40000001);
        Hu3DModelLayerSet(lbl_1_bss_EC[i + 1], 4);
        Hu3DMotionSpeedSet(lbl_1_bss_EC[i + 1], 0.0f);
        Hu3DModelPosSet(lbl_1_bss_EC[i + 1], lbl_1_data_88[i].x, lbl_1_data_88[i].y, lbl_1_data_88[i].z);
        Hu3DModelRotSet(lbl_1_bss_EC[i + 1], lbl_1_data_B8[i].x, lbl_1_data_B8[i].y, lbl_1_data_B8[i].z);
        Hu3DMotionSpeedSet(lbl_1_bss_EC[i + 1], 0.0f);
        Hu3DMotionTimeSet(lbl_1_bss_EC[i + 1], 0.0f);
    }
    Hu3DModelAttrReset(lbl_1_bss_EC[5], 1);
    Hu3DModelAttrReset(lbl_1_bss_EC[6], 1);
    Hu3DModelAttrReset(lbl_1_bss_EC[0], 1);
    for (i = 0; i < 2; i++) {
        Hu3DModelAttrReset(lbl_1_bss_EC[i + 7], 1);
    }
    for (i = 0; i < 2; i++) {
        Hu3DModelAttrReset(lbl_1_bss_EC[i + 0xB], 1);
    }
    for (i = 0; i < 4; i++) {
        Hu3DModelAttrReset(lbl_1_bss_EC[i + 1], 1);
    }
    lbl_1_bss_38 = 90.0f;
    lbl_1_bss_3C = 0.0f;
    lbl_1_bss_40 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_244[0xD], HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelPosSet(lbl_1_bss_40, 0.0f, 470.0 * sin(3.141592653589793 * lbl_1_bss_38 / 180.0), 0.0f);
    Hu3DModelRotSet(lbl_1_bss_40, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrSet(lbl_1_bss_40, 0x40000001);
    Hu3DModelShadowSet(lbl_1_bss_40);
    lbl_1_bss_E8 = 0;
    WipeCreate(1, 5, 0x3C);
    obj->objFunc = fn_1_2BEC;
}

void fn_1_2714(OMOBJ *obj)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        lbl_1_bss_CC[i] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_244[9], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelShadowSet(lbl_1_bss_CC[i]);
        Hu3DModelAttrSet(lbl_1_bss_CC[i], 1);
        Hu3DModelAttrSet(lbl_1_bss_CC[i], 0x40000001);
        lbl_1_bss_9C[i].x = lbl_1_data_124[i].x;
        lbl_1_bss_9C[i].y = lbl_1_data_124[i].y;
        lbl_1_bss_9C[i].z = lbl_1_data_124[i].z;
        Hu3DModelPosSet(lbl_1_bss_84[i], lbl_1_bss_9C[i].x, lbl_1_bss_9C[i].y, lbl_1_bss_9C[i].z);
        Hu3DModelRotSet(lbl_1_bss_CC[i], lbl_1_data_1B4[i].x, lbl_1_data_1B4[i].y, lbl_1_data_1B4[i].z);
        lbl_1_bss_8C[i] = 0.5f * i;
    }
    for (i = 0; i < 4; i++) {
        lbl_1_bss_84[i] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_244[0xA], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelShadowSet(lbl_1_bss_84[i]);
        Hu3DModelAttrSet(lbl_1_bss_84[i], 1);
        Hu3DModelAttrSet(lbl_1_bss_84[i], 0x40000001);
        lbl_1_bss_54[i].x = lbl_1_data_154[i].x;
        lbl_1_bss_54[i].y = lbl_1_data_154[i].y;
        lbl_1_bss_54[i].z = lbl_1_data_154[i].z;
        Hu3DModelPosSet(lbl_1_bss_84[i], lbl_1_bss_54[i].x, lbl_1_bss_54[i].y, lbl_1_bss_54[i].z);
        Hu3DModelRotSet(lbl_1_bss_84[i], lbl_1_data_214[i].x, lbl_1_data_214[i].y, lbl_1_data_214[i].z);
        lbl_1_bss_44[i] = 0.5f * i;
    }
    lbl_1_bss_4 = -1;
    lbl_1_bss_0 = -1;
    obj->objFunc = fn_1_2B70;
}

void fn_1_2B70(OMOBJ *obj)
{
    if (obj->work[0] != 1) {
        if (obj->work[0] < 1) {

        }
    } else if (obj->work[1] > 0x3C) {
        obj->work[0] = 2;
        obj->work[1] = 0;
    } else {
        fn_1_3CE0();
        obj->work[1] += 1;
    }
    fn_1_4048();
}

void fn_1_2BEC(OMOBJ *obj)
{
    M529Player *sp1C[4];
    s32 i;
    u32 var_r5;

    for (i = 0; i < 4; i++) {
        sp1C[i] = lbl_1_bss_11C[i]->data;
    }
    if (omSysExitReq != 0) {
        WipeCreate(2, 6, 0x3C);
        HuAudSeqAllFadeOut(0x3E8);
        HuAudSStreamAllFadeOut(0x3E8);
        fn_1_63E4();
        obj->objFunc = fn_1_4370;
    }
    var_r5 = obj->work[0];
    switch (var_r5) {
    case 0x3E8:
        lbl_1_bss_108 += 1.0f;
        if (lbl_1_bss_108 >= 15.0f) {
            obj->work[0] += 1;
            lbl_1_bss_108 = 0.0f;
            lbl_1_bss_30C.x = lbl_1_data_2A0.center.x;
            lbl_1_bss_30C.y = lbl_1_data_2A0.center.y;
            lbl_1_bss_30C.z = lbl_1_data_2A0.center.z;
            lbl_1_bss_2F4.x = lbl_1_data_2B8.x;
            lbl_1_bss_2F4.y = lbl_1_data_2B8.y;
            lbl_1_bss_2F4.z = lbl_1_data_2B8.z;
            Hu3DMotionSpeedSet(lbl_1_bss_EC[0], 1.0f);
            lbl_1_data_0 = HuAudFXPlay(0x797);
        }
        break;
    case 0x3E9:
        if (lbl_1_bss_108 == 0.0f) {
            omCameraViewMoveSimple(&lbl_1_data_328, 0x3C);
        }
        lbl_1_bss_108 += 1.0f;
        if (lbl_1_bss_108 >= 60.0f) {
            obj->work[0] += 1;
            lbl_1_bss_108 = 0.0f;
            lbl_1_bss_30C.x = lbl_1_data_2C4.center.x;
            lbl_1_bss_30C.y = lbl_1_data_2C4.center.y;
            lbl_1_bss_30C.z = lbl_1_data_2C4.center.z;
            lbl_1_bss_2F4.x = lbl_1_data_2DC.x;
            lbl_1_bss_2F4.y = lbl_1_data_2DC.y;
            lbl_1_bss_2F4.z = lbl_1_data_2DC.z;
        }
        break;
    case 0x3EA:
        if (lbl_1_bss_108 == 0.0f) {
            omCameraViewMoveSimple(&lbl_1_data_344, 0x50);
        }
        lbl_1_bss_108 += 1.0f;
        if (lbl_1_bss_108 >= 80.0f) {
            obj->work[0] += 1;
            lbl_1_bss_108 = 0.0f;
        }
        if (lbl_1_bss_108 > 40.0f) {
            lbl_1_bss_38 -= 0.85f;
            if (lbl_1_bss_38 < 0.0f) {
                lbl_1_bss_38 = 0.0f;
            }
        }
        break;
    case 0x3EB:
        lbl_1_bss_38 -= 0.85f;
        if (lbl_1_bss_38 < 0.0f) {
            lbl_1_bss_38 = 0.0f;
        }
        if (lbl_1_bss_22E < 0) {
            lbl_1_bss_22E = GameMesCreate(2, 0, var_r5, lbl_1_bss_38);
            if (lbl_1_bss_228 < 0) {
                lbl_1_bss_228 = HuAudSStreamPlay(0x3C);
            }
        } else if (GameMesStatGet(lbl_1_bss_22E) == 0) {
            obj->work[0] += 1;
            lbl_1_bss_22E = -1;
            MgTimerModeOnSet(lbl_1_bss_14, 1);
        }
        break;
    case 0x3EC:
        lbl_1_bss_E8 += 1;
        if (MgTimerDoneCheck(lbl_1_bss_14)) {
            obj->work[0] = 0x3ED;
            lbl_1_bss_108 = 0.0f;
            if (lbl_1_bss_20[0] > lbl_1_bss_20[1]) {
                lbl_1_bss_1C = 0;
            } else if (lbl_1_bss_20[0] < lbl_1_bss_20[1]) {
                lbl_1_bss_1C = 1;
            } else {
                lbl_1_bss_1C = 2;
            }
        }
        break;
    case 0x3ED:
        if (lbl_1_bss_22E < 0) {
            lbl_1_bss_22E = GameMesCreate(2, 1, var_r5);
            HuAudSStreamFadeOut(lbl_1_bss_228, 0x64);
            lbl_80288A28 = lbl_1_bss_230;
        } else if (GameMesStatGet(lbl_1_bss_22E) == 0) {
            obj->work[0] = 0x3EE;
            lbl_1_bss_22E = -1;
            lbl_1_bss_108 = 0.0f;
        }
        break;
    case 0x3EE:
        lbl_1_bss_108 += 1.0f;
        if (lbl_1_bss_108 >= 30.0f) {
            obj->work[0] = 0x3EF;
            lbl_1_bss_108 = 0.0f;
            lbl_1_bss_30C.x = lbl_1_data_2E8.center.x;
            lbl_1_bss_30C.y = lbl_1_data_2E8.center.y;
            lbl_1_bss_30C.z = lbl_1_data_2E8.center.z;
            lbl_1_bss_2F4.x = lbl_1_data_300.x;
            lbl_1_bss_2F4.y = lbl_1_data_300.y;
            lbl_1_bss_2F4.z = lbl_1_data_300.z;
        }
        break;
    case 0x3EF:
        lbl_1_bss_38 += 1.0f;
        if (lbl_1_bss_38 > 90.0f) {
            lbl_1_bss_38 = 90.0f;
        }
        if (lbl_1_bss_108 == 0.0f) {
            omCameraViewMoveSimple(&lbl_1_data_360, 0x50);
        }
        lbl_1_bss_108 += 1.0f;
        if (lbl_1_bss_108 >= 80.0f) {
            if (lbl_1_bss_1C == 2) {
                if (lbl_1_bss_20[0] > lbl_1_bss_18) {
                    if ((GwPlayerConf[sp1C[0]->unk14].type != 0) && (GwPlayerConf[sp1C[1]->unk14].type != 0)) {
                        obj->work[0] = 0x3F1;
                    } else {
                        GWMgRecordSet(6, lbl_1_bss_20[0]);
                        lbl_1_bss_18 = lbl_1_bss_20[0];
                        obj->work[0] = 0x3F0;
                    }
                } else {
                    obj->work[0] = 0x3F1;
                }
            } else if (lbl_1_bss_20[lbl_1_bss_1C] > lbl_1_bss_18) {
                if (lbl_1_bss_1C == 0) {
                    if ((GwPlayerConf[fn_1_59BC(0)].type != 0) && (GwPlayerConf[fn_1_59BC(1)].type != 0)) {
                        obj->work[0] = 0x3F1;
                    } else {
                        GWMgRecordSet(6, lbl_1_bss_20[lbl_1_bss_1C]);
                        lbl_1_bss_18 = lbl_1_bss_20[lbl_1_bss_1C];
                        obj->work[0] = 0x3F0;
                    }
                } else if ((GwPlayerConf[fn_1_59BC(2)].type != 0) && (GwPlayerConf[fn_1_59BC(3)].type != 0)) {
                    obj->work[0] = 0x3F1;
                } else {
                    GWMgRecordSet(6, lbl_1_bss_20[lbl_1_bss_1C]);
                    lbl_1_bss_18 = lbl_1_bss_20[lbl_1_bss_1C];
                    obj->work[0] = 0x3F0;
                }
            } else {
                obj->work[0] = 0x3F1;
            }
            lbl_1_bss_108 = 0.0f;
        }
        break;
    case 0x3F0:
        if (lbl_1_bss_22E < 0) {
            lbl_1_bss_DC = 1;
            lbl_1_bss_22E = GameMesCreate(0xE, lbl_1_bss_18, &lbl_1_bss_18);
            if (!_CheckFlag(0x1000F)) {
                HuAudJinglePlay(0x86);
            }
        } else if (GameMesStatGet(lbl_1_bss_22E) == 0) {
            obj->work[0] = 0x3F1;
            lbl_1_bss_108 = 0.0f;
            lbl_1_bss_22E = -1;
        }
        break;
    case 0x3F1:
        if (lbl_1_bss_108 > 210.0f) {
            obj->work[0] = 0x3F2;
            lbl_1_bss_108 = 0.0f;
            lbl_1_bss_22E = -1;
        } else {
            if (lbl_1_bss_108 == 0.0f) {
                if (lbl_1_bss_1C != 2) {
                    if (lbl_1_bss_1C == 0) {
                        GwPlayer[fn_1_59BC(0)].mgCoinBonus += 0xA;
                        GwPlayer[fn_1_59BC(1)].mgCoinBonus += 0xA;
                    } else {
                        GwPlayer[fn_1_59BC(2)].mgCoinBonus += 0xA;
                        GwPlayer[fn_1_59BC(3)].mgCoinBonus += 0xA;
                    }
                    lbl_1_bss_22E = GameMesCreate(4, 3, fn_1_5960(0), fn_1_5960(1), -1, -1, 0.0f);
                    HuAudJinglePlay(0x79);
                    lbl_1_bss_E0 = lbl_1_bss_2C;
                } else {
                    lbl_1_bss_22E = GameMesCreate(2, 2, var_r5, 0.0f);
                    HuAudJinglePlay(0x7B);
                }
            }
            if (lbl_1_bss_108 == 60.0f) {
                lbl_1_bss_114->work[0] = 1;
            }
            lbl_1_bss_108 += 1.0f;
        }
        break;
    case 0x3F2:
        lbl_1_bss_108 += 1.0f;
        if (lbl_1_bss_108 > 1.0f) {
            HuAudSeqAllFadeOut(0x3E8);
            HuAudSStreamAllFadeOut(0x3E8);
            fn_1_63E4();
            lbl_1_bss_22E = -1;
            WipeCreate(2, 6, 0x3C);
            obj->objFunc = fn_1_4290;
        }
        break;
    default:
        OSReport(lbl_1_data_424, obj->work[0], var_r5);
        break;
    }
    Hu3DModelPosSet(lbl_1_bss_40, 0.0f, 470.0 * sin(3.141592653589793 * lbl_1_bss_38 / 180.0), 0.0f);
    fn_1_5680(obj->work[0], obj->work[0]);
    if ((obj->work[0] >= 0x3F1) && (lbl_1_bss_1C != 2) && ((lbl_1_bss_20[0] > 0x31) || (lbl_1_bss_20[1] > 0x31))) {
        if (lbl_1_bss_1C == 0) {
            if ((GwPlayerConf[sp1C[0]->unk14].type == 0) || (GwPlayerConf[sp1C[1]->unk14].type == 0)) {
                fn_1_57BC();
            }
        } else if ((GwPlayerConf[sp1C[2]->unk14].type == 0) || (GwPlayerConf[sp1C[3]->unk14].type == 0)) {
            fn_1_57BC();
        }
    }
}

void fn_1_3CE0(void)
{
    s32 i;

    switch (lbl_1_bss_1C) {
    case 0:
        for (i = 0; i < 4; i++) {
            Hu3DModelAttrReset(lbl_1_bss_CC[i], 1);
            lbl_1_bss_9C[i].x += (lbl_1_data_184[i].x - lbl_1_data_124[i].x) / 60.0f;
            lbl_1_bss_9C[i].y += (lbl_1_data_184[i].y - lbl_1_data_124[i].y) / 60.0f;
            lbl_1_bss_9C[i].z += (lbl_1_data_184[i].z - lbl_1_data_124[i].z) / 60.0f;
            Hu3DModelPosSet(lbl_1_bss_CC[i], lbl_1_bss_9C[i].x, lbl_1_bss_9C[i].y, lbl_1_bss_9C[i].z);
        }
        return;
    case 1:
        for (i = 0; i < 4; i++) {
            Hu3DModelAttrReset(lbl_1_bss_84[i], 1);
            lbl_1_bss_54[i].x += (lbl_1_data_1E4[i].x - lbl_1_data_154[i].x) / 60.0f;
            lbl_1_bss_54[i].y += (lbl_1_data_1E4[i].y - lbl_1_data_154[i].y) / 60.0f;
            lbl_1_bss_54[i].z += (lbl_1_data_1E4[i].z - lbl_1_data_214[i].z) / 60.0f;
            Hu3DModelPosSet(lbl_1_bss_84[i], lbl_1_bss_54[i].x, lbl_1_bss_54[i].y, lbl_1_bss_54[i].z);
        }
        return;
    }
}

void fn_1_4048(void)
{
    s32 i;
    float temp_f31;

    switch (lbl_1_bss_1C) {
    case 0:
        for (i = 0; i < 4; i++) {
            temp_f31 = sin(3.141592653589793 * (180.0f * lbl_1_bss_8C[i]) / 180.0);
            Hu3DModelPosSet(lbl_1_bss_CC[i], lbl_1_bss_9C[i].x, temp_f31 * temp_f31 * 25.0f + lbl_1_bss_9C[i].y, lbl_1_bss_9C[i].z);
            lbl_1_bss_8C[i] += 0.011111111f;
        }
        return;
    case 1:
        for (i = 0; i < 4; i++) {
            temp_f31 = sin(3.141592653589793 * (180.0f * lbl_1_bss_44[i]) / 180.0);
            Hu3DModelPosSet(lbl_1_bss_84[i], lbl_1_bss_54[i].x, temp_f31 * temp_f31 * 25.0f + lbl_1_bss_54[i].y, lbl_1_bss_54[i].z);
            lbl_1_bss_44[i] += 0.011111111f;
        }
        return;
    }
}

void fn_1_4290(OMOBJ *obj)
{
    s32 i;
    M529Player *temp_r31;

    if (WipeCheckEnd() == 0) {
        if (lbl_1_bss_22E >= 0) {
            GameMesKill(lbl_1_bss_22E);
        }
        if (lbl_1_bss_22C >= 0) {
            GameMesKill(lbl_1_bss_22C);
        }
        GameMesClose();
        HuAudAllStop();
        for (i = 0; i < 4; i++) {
            temp_r31 = lbl_1_bss_11C[i]->data;
            CharModelKill(temp_r31->unk28);
        }
        MgTimerKill(lbl_1_bss_14);
        omOvlReturnEx(1, 1);
    }
}

void fn_1_4370(OMOBJ *obj)
{
    s32 i;
    M529Player *temp_r31;

    if (WipeCheckEnd() == 0) {
        if (lbl_1_bss_22E >= 0) {
            GameMesKill(lbl_1_bss_22E);
        }
        if (lbl_1_bss_22C >= 0) {
            GameMesKill(lbl_1_bss_22C);
        }
        GameMesClose();
        for (i = 0; i < 4; i++) {
            temp_r31 = lbl_1_bss_11C[i]->data;
            CharModelKill(temp_r31->unk28);
        }
        MgTimerKill(lbl_1_bss_14);
        omOvlReturnEx(1, 1);
    }
}

s32 fn_1_444C(OMOBJ *obj)
{
    M529Player *temp_r31 = obj->data;

    if ((temp_r31->unk40 == 0) && (temp_r31->unk3C != 0)) {
        return 0;
    }
    return 1;
}

s32 fn_1_4488(OMOBJ *obj)
{
    M529Player *temp_r31 = obj->data;

    if ((temp_r31->unk58 == 0) && (temp_r31->unk54 != 0)) {
        return 0;
    }
    return 1;
}

void fn_1_44C4(OMOBJ *obj)
{
    Vec sp8;
    M529Player *temp_r31 = obj->data;

    if ((temp_r31->unk40 == 0) && (temp_r31->unk3C != 0)) {
        temp_r31->unk44 = 0;
        temp_r31->unk40 = 1;
        temp_r31->unk3C = 0;
        temp_r31->unk38 = 0;
        temp_r31->unk94 = 1300.0f;
        temp_r31->unk4 = 3;
        CharMotionShiftSet(temp_r31->unk28, obj->mtnId[3], 0.0f, 4.0f, 0);
        Hu3D3Dto2D(&temp_r31->unk84, 1, &sp8);
        CharFXPlayPan(temp_r31->unk28, 0x9E, 32.0f + sp8.x / 576.0f * 64.0f);
    }
    if (temp_r31->unk40 != 0) {
        if (temp_r31->unk44 == 6) {
            if (temp_r31->unk30 & 0x100) {
                temp_r31->unk38 = 1;
            } else {
                temp_r31->unk38 = 0;
            }
        }
        if (temp_r31->unk38 == 1) {
            temp_r31->unk94 += -36.00000251332922;
        } else {
            temp_r31->unk94 += -146.25001021039995;
        }
        temp_r31->unk84.y += 1.35f * (0.016666668f * temp_r31->unk94);
        temp_r31->unk44 += 1;
        if (temp_r31->unk84.y <= 0.0f) {
            temp_r31->unk40 = 0;
            temp_r31->unk84.y = 0.0f;
            temp_r31->unk38 = 0;
            CharModelLandDustCreate(temp_r31->unk28, &temp_r31->unk84);
        }
        if (temp_r31->unk9C > temp_r31->unk84.y) {
            if ((temp_r31->unk84.y <= 65.0f) && (temp_r31->unk4 != 0)) {
                temp_r31->unk4 = 0;
                CharMotionShiftSet(temp_r31->unk28, obj->mtnId[0], 0.0f, 5.0f, 0x40000001);
            }
        }
        temp_r31->unk9C = temp_r31->unk84.y;
    }
}

void fn_1_4780(OMOBJ *obj)
{
    s16 var_r28;
    s16 var_r29;
    s32 temp_r0;
M529Player *temp_r31 = obj->data;
    M529Player *temp_r30 = lbl_1_bss_11C[temp_r31->unk1C]->data;

    if (temp_r31->unk0 == 0xBB9) {
        var_r29 = 1;
        var_r28 = 2;
    } else {
        var_r29 = 3;
        var_r28 = 4;
    }
    if (temp_r31->unk60 != 0) {
        temp_r31->unk54 = 1;
    }
    switch (temp_r31->unk80) {
    case 5:
        temp_r31->unk68 = 2.0f;
        break;
    case 4:
        temp_r31->unk68 = 1.5f;
        break;
    case 3:
        temp_r31->unk68 = 1.2f;
        break;
    case 2:
        temp_r31->unk68 = 1.0f;
        break;
    case 1:
        temp_r31->unk68 = 1.0f;
        break;
    case 0:
        temp_r31->unk68 = 0.0f;
        temp_r31->unk60 = 0;
        break;
    }
    if ((GwPlayerConf[temp_r31->unk14].type != 0) && (temp_r31->unk6C == 0)) {
        temp_r31->unk68 = 1.0f;
    }
    if (temp_r31->unk0 == 0xBB9) {
        if (lbl_1_bss_10 != -1) {
            HuAudFXPitchSet(lbl_1_bss_10, fn_1_6334(temp_r31->unk14));
        }
    } else if (lbl_1_bss_C != -1) {
        HuAudFXPitchSet(lbl_1_bss_C, fn_1_6334(temp_r31->unk14));
    }
    if ((temp_r31->unk58 == 0) && (temp_r31->unk54 != 0)) {
        temp_r31->unk6C += 1;
        temp_r31->unk48 = 1;
        if (temp_r31->unk68 < 1.0f) {
            temp_r31->unk68 = 1.0f;
        }
        if (temp_r31->unk68 >= 2.0f) {
            temp_r31->unk68 = 2.0f;
        }
        temp_r31->unk70 = (1.0f + Hu3DMotionMaxTimeGet(lbl_1_bss_EC[var_r29], (s16 (*)[10]) lbl_1_bss_EC)) / temp_r31->unk68;
        temp_r31->unk50 = temp_r31->unk70 / 2 - 1;
        temp_r31->unk64 = 0;
        temp_r31->unk58 = 1;
        temp_r31->unk54 = 0;
        Hu3DMotionSpeedSet(lbl_1_bss_EC[var_r29], temp_r31->unk68);
        Hu3DMotionSpeedSet(lbl_1_bss_EC[var_r28], temp_r31->unk68);
        CharMotionSpeedSet(temp_r31->unk28, temp_r31->unk68);
        Hu3DMotionTimeSet(lbl_1_bss_EC[var_r29], 0.0f);
        Hu3DMotionTimeSet(lbl_1_bss_EC[var_r28], 0.0f);
        CharMotionTimeSet(temp_r31->unk28, 0.0f);
    }
    if (temp_r31->unk58 != 0) {
        temp_r31->unk64 += 1;
        if (temp_r31->unk50 + 1 == temp_r31->unk64) {
            temp_r31->unk5C = 1;
        } else {
            temp_r31->unk5C = 0;
        }
        if (temp_r31->unk64 >= temp_r31->unk70) {
            temp_r31->unk58 = 0;
            if (temp_r31->unk60 != 0) {
                Hu3DMotionSpeedSet(lbl_1_bss_EC[var_r29], temp_r31->unk68);
                Hu3DMotionSpeedSet(lbl_1_bss_EC[var_r28], temp_r31->unk68);
                Hu3DMotionSpeedSet(obj->mdlId[0], temp_r31->unk68);
            } else {
                Hu3DMotionSpeedSet(lbl_1_bss_EC[var_r29], 0.0f);
                Hu3DMotionSpeedSet(lbl_1_bss_EC[var_r28], 0.0f);
                Hu3DMotionSpeedSet(obj->mdlId[0], 0.0f);
            }
            Hu3DMotionTimeSet(lbl_1_bss_EC[var_r29], 0.0f);
            Hu3DMotionTimeSet(lbl_1_bss_EC[var_r28], 0.0f);
            CharMotionTimeSet(temp_r31->unk28, 0.0f);
            temp_r31->unk64 = 0;
        }
    }
    if ((temp_r31->unk5C != 0) && ((temp_r30->unkC == 0) || (temp_r30->unkC == 2))) {
        if (temp_r30->unk40 != 0) {
            if (temp_r31->unk8 == 0x7D1) {
                if (temp_r31->unk48 != 0) {
                    if (temp_r31->unk0 == 0xBB9) {
                        lbl_1_bss_20[0] += 1;
                    } else {
                        lbl_1_bss_20[1] += 1;
                    }
                }
                temp_r31->unk48 = 0;
            }
        } else if (temp_r30->unkC == 0) {
            temp_r30->unkC = 1;
        }
    }
    if (temp_r31->unk0 == 0xBB9) {
        if ((temp_r31->unk58 == 0) && (temp_r31->unk54 == 0)) {
            if (lbl_1_bss_10 != -1) {
                HuAudFXStop(lbl_1_bss_10);
                lbl_1_bss_10 = -1;
            }
        } else if (lbl_1_bss_10 == -1) {
            lbl_1_bss_10 = HuAudFXPlay(0x790);
        }
    } else if ((temp_r31->unk58 == 0) && (temp_r31->unk54 == 0)) {
        if (lbl_1_bss_C != -1) {
            HuAudFXStop(lbl_1_bss_C);
            lbl_1_bss_C = -1;
        }
    } else if (lbl_1_bss_C == -1) {
        lbl_1_bss_C = HuAudFXPlay(0x791);
    }
}

s32 fn_1_4E48(float arg0, float arg1, float arg2)
{
    if ((arg0 <= arg1) && (arg1 <= arg2)) {
        return 1;
    }
    return 0;
}

s32 fn_1_4E70(M529Player *player)
{
    if ((player->unk34 & 0x40) && (player->unk34 & 0x20)) {
        return 0;
    }
    if ((player->unk34 & 0x40) && ((player->unk7C == 1) || (player->unk7C == -1))) {
        player->unk7C = 0;
        return 1;
    }
    if ((player->unk34 & 0x20) && ((player->unk7C == 0) || (player->unk7C == -1))) {
        player->unk7C = 1;
        return 1;
    }
    return 0;
}

u16 fn_1_4F10(OMOBJ *obj)
{
M529Player *temp_r31 = obj->data;
    M529Player *temp_r29 = lbl_1_bss_11C[temp_r31->unk1C]->data;
    u16 var_r30 = 0;

    if (temp_r31->unkB8 != 0) {
        if ((temp_r31->unkB8 >= 0) && (temp_r31->unkB8 < 4)) {
            if ((temp_r31->unk40 == 0) && frandmod(temp_r31->unkBC)) {
                if (temp_r29->unk64 == temp_r29->unk50) {
                    var_r30 = 0x100;
                }
            } else {
                var_r30 = 0;
            }
        } else {
            var_r30 = 0;
        }
    } else if (temp_r31->unk40 == 0) {
        temp_r31->unkC4 -= 1;
        if (temp_r31->unkC4 < 0) {
            var_r30 = 0x100;
            temp_r31->unkC4 = frandmod(0x1E) + 0x1E;
        }
    }
    return var_r30;
}

u16 fn_1_500C(OMOBJ *obj)
{
    M529Player *temp_r31 = obj->data;
    M529Player *var_r29 = lbl_1_bss_11C[temp_r31->unk1C]->data;
    u16 var_r30 = 0;

    if (temp_r31->unkB8 == 0) {
        var_r30 = 0x100;
    } else if ((temp_r31->unkB8 >= 0) && (temp_r31->unkB8 < 4)) {
        var_r30 = 0;
    } else {
        var_r30 = 0;
    }
    return var_r30;
}

u16 fn_1_5084(OMOBJ *obj)
{
    M529Player *temp_r31 = obj->data;
    u16 var_r30 = 0;

    if ((temp_r31->unkB8 >= 0) && (temp_r31->unkB8 < 4)) {
        if (temp_r31->unkCC == -1) {
            temp_r31->unkC4 = 0;
        }
        temp_r31->unkC4 -= 1;
        if (temp_r31->unkC4 < 0) {
            if (temp_r31->unk7C == -1) {
                var_r30 = 0x40;
            } else if (frandmod(temp_r31->unkC0)) {
                var_r30 = fn_1_5170(obj);
            } else {
                var_r30 = 0;
            }
            temp_r31->unkC4 = fn_1_51B4(temp_r31->unkC8, temp_r31->unkCC);
        }
    }
    return var_r30;
}

u16 fn_1_5170(OMOBJ *obj)
{
    M529Player *temp_r31 = obj->data;

    if (temp_r31->unk7C == -1) {
        return 0x40;
    }
    if (temp_r31->unk7C == 0) {
        return 0x20;
    }
    return 0x40;
}

s32 fn_1_51B4(s32 arg0, s32 arg1)
{
    if (arg0 > arg1) {
        return 0;
    }
    return arg0 + frandmod(arg1 - arg0);
}

void fn_1_5204(OMOBJ *obj)
{
    M529Score *temp_r31;

    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M529Score), HU_MEMNUM_OVL);
    temp_r31 = obj->data;
    lbl_1_bss_34 = MgScoreWinCreate(0x40, 0x28);
    MgScoreWinColorSet(lbl_1_bss_34, 0xFA, 0, 0x1E);
    MgScoreWinTPLvlSet(lbl_1_bss_34, 0.7f);
    MgScoreWinPosSet(lbl_1_bss_34, 48.0f, 60.0f);
    temp_r31->unk8 = MgScoreCreate(0x960031, -1, 1);
    MgScoreValueSet(temp_r31->unk8, lbl_1_bss_20[0]);
    MgScorePosSet(temp_r31->unk8, 38.0f, 60.0f);
    MgScoreMaxDigitSet(temp_r31->unk8, 2);
    MgScoreDispOff(temp_r31->unk8);
    lbl_1_bss_32 = MgScoreWinCreate(0x40, 0x28);
    MgScoreWinColorSet(lbl_1_bss_32, 0, 0x32, 0xFA);
    MgScoreWinTPLvlSet(lbl_1_bss_32, 0.7f);
    MgScoreWinPosSet(lbl_1_bss_32, 528.0f, 60.0f);
    temp_r31->unkC = MgScoreCreate(0x960031, -1, 1);
    MgScoreValueSet(temp_r31->unkC, lbl_1_bss_20[1]);
    MgScorePosSet(temp_r31->unkC, 518.0f, 60.0f);
    MgScoreMaxDigitSet(temp_r31->unkC, 2);
    MgScoreDispOff(temp_r31->unkC);
    lbl_1_bss_30 = MgScoreWinCreate(0x48, 0x30);
    MgScoreWinTPLvlSet(lbl_1_bss_30, 0.7f);
    MgScoreWinPosSet(lbl_1_bss_30, 288.0f, 416.0f);
temp_r31->unk12 = espEntry(0x960045, 0xA, 0);
    espDispOff(temp_r31->unk12);
    espDrawNoSet(temp_r31->unk12, 0);
    espPosSet(temp_r31->unk12, 269.0f, 416.0f);
    temp_r31->unk4 = MgScoreCreate(0x960030, -1, 1);
    MgScoreValueSet(temp_r31->unk4, lbl_1_bss_18);
    MgScorePosSet(temp_r31->unk4, 290.0f, 416.0f);
    MgScoreMaxDigitSet(temp_r31->unk4, 2);
    MgScoreDispOff(temp_r31->unk4);
    MgScoreColorSet(temp_r31->unk4, 0x42, 0xFF, 0x7A);
    temp_r31->unk14 = 0;
    temp_r31->unk18 = 1.0f;
    obj->objFunc = fn_1_553C;
}

void fn_1_553C(OMOBJ *obj)
{
    M529Score *temp_r31 = lbl_1_bss_110->data;
    float temp_f1;

    MgScoreValueSet(temp_r31->unk4, lbl_1_bss_18);
    if (lbl_1_bss_DC != 0) {
        if (temp_r31->unk14 == 0) {
            temp_f1 = temp_r31->unk18 + 0.02f;
            temp_r31->unk18 = temp_f1;
            if (temp_f1 >= 1.2f) {
                temp_r31->unk18 = 1.2f;
                temp_r31->unk14 = 1;
            }
        } else {
            temp_f1 = temp_r31->unk18 - 0.04f;
            temp_r31->unk18 = temp_f1;
            if (temp_f1 <= 1.0f) {
                temp_r31->unk18 = 1.0f;
                temp_r31->unk14 = 0;
            }
        }
        MgScoreDigitScaleSet(temp_r31->unk4, temp_r31->unk18, temp_r31->unk18);
        espScaleSet(temp_r31->unk12, temp_r31->unk18, temp_r31->unk18);
    }
    MgScoreValueSet(temp_r31->unk8, lbl_1_bss_20[0]);
    MgScoreValueSet(temp_r31->unkC, lbl_1_bss_20[1]);
}

void fn_1_5680(s16 arg0, u32 arg1)
{
    s16 sp8 = arg0;
    M529Score *temp_r30 = lbl_1_bss_110->data;
    s32 i;
    s32 j;
    void **temp_r29 = &temp_r30->unk8;

    if (sp8 >= 0x3EC) {
        for (i = 0; i < 2; i++) {
            MgScoreWinDispSet(lbl_1_bss_34, 1);
            MgScoreWinDispSet(lbl_1_bss_32, 1);
            for (j = 0; j < 2; j++) {
                MgScoreDispOn(temp_r29[j]);
            }
        }
    } else {
        for (i = 0; i < 2; i++) {
            MgScoreWinDispSet(lbl_1_bss_34, 0);
            MgScoreWinDispSet(lbl_1_bss_32, 0);
            for (j = 0; j < 2; j++) {
                MgScoreDispOff(temp_r29[j]);
            }
        }
    }
    MgScoreWinDispSet(lbl_1_bss_30, 1);
    espDispOn(temp_r30->unk12);
    MgScoreDispOn(temp_r30->unk4);
}

void fn_1_57BC(void)
{
    if (lbl_1_bss_8 == -1) {
        lbl_1_bss_8 = HuAudFXPlay(0x796);
    }
    if (lbl_1_bss_D4 == 0) {
        Hu3DMotionSet(lbl_1_bss_EC[0], lbl_1_bss_D8);
    }
    Hu3DMotionTimeSet(lbl_1_bss_EC[0], lbl_1_bss_E0);
    Hu3DMotionSpeedSet(lbl_1_bss_EC[0], 1.5f);
    if (lbl_1_bss_E4 != 0) {
        lbl_1_bss_E0 += 1;
        if (lbl_1_bss_2C < lbl_1_bss_E0) {
            lbl_1_bss_E4 = 0;
        }
    } else {
        lbl_1_bss_E0 -= 1;
        if ((lbl_1_bss_2C - 0x28) > lbl_1_bss_E0) {
            lbl_1_bss_E4 = 1;
        }
    }
}

void fn_1_595C(void)
{
}

s16 fn_1_5960(s16 arg0)
{
    s32 i;
    M529Player *temp_r31;

    for (i = 0; i < 4; i++) {
        temp_r31 = lbl_1_bss_11C[i]->data;
        if (temp_r31->unk24 == arg0) {
            return temp_r31->unk28;
        }
    }
    return arg0;
}

s32 fn_1_59BC(s16 arg0)
{
    s32 i;
    M529Player *temp_r31;

    for (i = 0; i < 4; i++) {
        temp_r31 = lbl_1_bss_11C[i]->data;
        if (temp_r31->unk24 == arg0) {
            return temp_r31->unk14;
        }
    }
    return arg0;
}

void fn_1_5A18(void)
{
    fn_1_5A84(&lbl_1_data_27C, &lbl_1_data_294);
    fn_1_5A84(&lbl_1_data_2A0, &lbl_1_data_2B8);
    fn_1_5A84(&lbl_1_data_2C4, &lbl_1_data_2DC);
    fn_1_5A84(&lbl_1_data_2E8, &lbl_1_data_300);
}

void fn_1_5A84(OMCAMERAVIEW *arg0, Vec *arg1)
{
    float sp50;
    float sp3C;
    float sp34;
    float sp28;
    float sp1C;
    float sp14[16];
    float sp10;
    float spC;
    float sp8;
    float temp_f0;
    float temp_f0_2;
    float temp_f18;
    float temp_f19;
    float temp_f21;
    float temp_f27;
    float temp_f29;
    float temp_f30;
    float temp_f31;
    float var_f26;
    f64 temp_f1;
    f64 temp_f28;
    f64 temp_f28_2;
    f64 temp_f28_3;
    f64 temp_ret;
    s32 i;
    s32 j;

    temp_f31 = arg0->center.x - arg1->x;
    temp_f29 = arg0->center.y - arg1->y;
    temp_f30 = arg0->center.z - arg1->z;
    temp_f19 = arg1->x;
    temp_f18 = arg1->y;
    spC = arg1->z;
    if ((temp_f30 * temp_f30 + (temp_f31 * temp_f31 + temp_f29 * temp_f29)) > 0.0f) {
        temp_f28 = __frsqrte(temp_f30 * temp_f30 + (temp_f31 * temp_f31 + temp_f29 * temp_f29));
        temp_f28_2 = 0.5 * temp_f28 * (3.0 - (temp_f30 * temp_f30 + (temp_f31 * temp_f31 + temp_f29 * temp_f29)) * (temp_f28 * temp_f28));
        temp_f28_3 = 0.5 * temp_f28_2 * (3.0 - (temp_f30 * temp_f30 + (temp_f31 * temp_f31 + temp_f29 * temp_f29)) * (temp_f28_2 * temp_f28_2));
        sp8 = (temp_f30 * temp_f30 + (temp_f31 * temp_f31 + temp_f29 * temp_f29)) *
            (0.5 * temp_f28_3 * (3.0 - (temp_f30 * temp_f30 + (temp_f31 * temp_f31 + temp_f29 * temp_f29)) * (temp_f28_3 * temp_f28_3)));
        var_f26 = sp8;
    } else {
        var_f26 = temp_f30 * temp_f30 + (temp_f31 * temp_f31 + temp_f29 * temp_f29);
    }
    temp_f27 = 180.0 * atan2(temp_f30, temp_f31) / 3.141592653589793;
    temp_f21 = 90.0f - temp_f27;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            sp14[i * 4 + j] = 0.0f;
        }
    }
    temp_f0 = cos(3.141592653589793 * -temp_f27 / 180.0);
    sp3C = temp_f0;
    sp14[0] = temp_f0;
    temp_f0_2 = -sin(3.141592653589793 * -temp_f27 / 180.0);
    sp34 = temp_f0_2;
    sp1C = sin(3.141592653589793 * -temp_f27 / 180.0);
    sp50 = 1.0f;
    sp28 = 1.0f;
    sp10 = temp_f30 * sp3C + (temp_f31 * sp34 + temp_f29 * sp14[0]);
    temp_ret = atan2(temp_f30 * sp1C + (temp_f31 * sp50 + temp_f29 * sp28),
        temp_f30 * temp_f0_2 + (temp_f31 * temp_f0 + temp_f29 * sp14[0]));
    temp_f1 = temp_ret / 3.141592653589793;
    OSReport(lbl_1_data_46C);
    OSReport(lbl_1_data_489, temp_f19, temp_f18, spC);
    OSReport(lbl_1_data_4B4, -(180.0 * temp_f1), temp_f21, 0.0f);
    OSReport(lbl_1_data_4DC, var_f26);
    OSReport(lbl_1_data_4F1);
    OSReport(lbl_1_data_4F5);
}

void fn_1_5F74(void)
{
    GXColor sp8;

    sp8.r = 0;
    sp8.g = 0;
    sp8.b = 0;
    sp8.a = 0x90;
    CZoom += *HuPadTrigL / 5;
    CZoom -= *HuPadTrigR / 5;
    Center.x += 10.0f * (*HuPadSubStkX / 59.0f);
    Center.z -= 10.0f * (*HuPadSubStkY / 59.0f);
    CRot.x += 5.0f * (*HuPadSubStkY / 59.0f);
    CRot.y -= 5.0f * (*HuPadSubStkX / 59.0f);
    printWin(0x10, 0x28, 0x180, 0x24, &sp8);
    fontcolor = 0xE;
    print8(0x10, 0x28, 1.5f, "Center X:%.02f Y:%.02f Z:%.02f", &Center, Center.x, Center.y, Center.z);
    print8(0x10, 0x34, 1.5f, "CRot X:%.02f Y:%.02f Z:%.02f", &CRot, CRot.x, CRot.y, CRot.z);
    print8(0x10, 0x40, 1.5f, "CZoom:%.02f", &CZoom, CZoom);
    if (*HuPadBtnDown & 0x400) {
        omCameraViewSet(&lbl_1_data_30C);
    }
}

s16 fn_1_6334(s32 arg0)
{
    M529Player *temp_r31 = lbl_1_bss_11C[arg0]->data;

if (temp_r31->unk68 == 2.0f) {
        return 0x2000;
    }
    if (temp_r31->unk68 == 1.5f) {
        return 0x1000;
    }
    if (temp_r31->unk68 == 1.2f) {
        return 0x6A4;
    }
    if (temp_r31->unk68 == 1.0f) {
        return 0;
    }
    return 0;
}

void fn_1_63E4(void)
{
    if (HuAudFXStatusGet(lbl_1_data_0)) {
        HuAudFXFadeOut(lbl_1_data_0, 0x3E8);
    }
    if (lbl_1_bss_10 != -1) {
        HuAudFXFadeOut(lbl_1_bss_10, 0x3E8);
    }
    if (lbl_1_bss_C != -1) {
        HuAudFXFadeOut(lbl_1_bss_C, 0x3E8);
    }
    if (lbl_1_bss_8 != -1) {
        HuAudFXFadeOut(lbl_1_bss_8, 0x3E8);
    }
    if (lbl_1_bss_4 != -1) {
        HuAudFXFadeOut(lbl_1_bss_4, 0x3E8);
    }
    if (lbl_1_bss_0 != -1) {
        HuAudFXFadeOut(lbl_1_bss_0, 0x3E8);
    }
}

void fn_1_64F4(void)
{
    lbl_1_bss_328.prev = NULL;
    lbl_1_bss_328.next = NULL;
}

void fn_1_6518(u16 arg0, s16 arg1)
{
    M529ListNode *temp_r3 = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M529ListNode), HU_MEMNUM_OVL);
    M529ListNode *temp_r29;
    M529ListNode *temp_r30;

    temp_r3->unk0 = arg0;
    temp_r3->unk2 = arg1;
    temp_r30 = &lbl_1_bss_328;
    temp_r29 = temp_r30->next;
    temp_r30->next = temp_r3;
    if ((u32) temp_r29 != 0) {
        temp_r29->prev = temp_r3;
    }
    temp_r3->prev = temp_r30;
    temp_r3->next = temp_r29;
}

s16 fn_1_65A0(s16 arg0)
{
    s16 temp_r28;
    M529ListNode *temp_r29;
    M529ListNode *temp_r30;
    M529ListNode *var_r31 = lbl_1_bss_328.next;

    while ((u32) var_r31 != 0) {
        if (arg0 == var_r31->unk0) {
            temp_r28 = var_r31->unk2;
            temp_r29 = var_r31->prev;
            temp_r30 = var_r31->next;
            temp_r29->next = temp_r30;
            if ((u32) temp_r30 != 0) {
                temp_r30->prev = temp_r29;
            }
            HuMemDirectFree(var_r31);
            return temp_r28;
        }
        var_r31 = var_r31->next;
    }
    return -1;
}

void fn_1_6644(void)
{
    s32 i;

    for (i = 0; i < 0x20; i++) {
        lbl_1_bss_120[i].dataNum = 0;
        lbl_1_bss_120[i].modelId = -1;
    }
}

s16 fn_1_669C(u32 arg0)
{
    s32 i;
    s32 j;

    for (i = 0; i < 0x20; i++) {
        if (arg0 == lbl_1_bss_120[i].dataNum) {
            return Hu3DModelLink(lbl_1_bss_120[i].modelId);
        }
    }
    for (j = 0; j < 0x20; j++) {
        if (lbl_1_bss_120[j].dataNum == 0) {
            lbl_1_bss_120[j].dataNum = arg0;
            lbl_1_bss_120[j].modelId = Hu3DModelCreate(HuDataSelHeapReadNum(arg0, HU_MEMNUM_OVL, HEAP_MODEL));
            return lbl_1_bss_120[j].modelId;
        }
    }
    return Hu3DModelCreate(HuDataSelHeapReadNum(arg0, HU_MEMNUM_OVL, HEAP_MODEL));
}

void fn_1_67B0(Vec *arg0, Vec *arg1, Vec *arg2, float farg0)
{
    float var_f30 = farg0;
    float temp_f31;
    f64 temp_f29;

    if (var_f30 > 1.0f) {
        var_f30 = 1.0f;
    }
    temp_f29 = sin(3.141592653589793 * (90.0f * var_f30) / 180.0);
    temp_f31 = sin(3.141592653589793 * (90.0f * var_f30) / 180.0) * temp_f29;
    arg2->x = arg0->x + temp_f31 * (arg1->x - arg0->x);
    arg2->y = arg0->y + temp_f31 * (arg1->y - arg0->y);
    arg2->z = arg0->z + temp_f31 * (arg1->z - arg0->z);
}

void fn_1_690C(Vec *arg0, Vec *arg1, Vec *arg2)
{
    Hu3DCameraPosSetV(1, arg0, arg1, arg2);
    lbl_1_bss_318 = *arg0;
    lbl_1_bss_300 = *arg2;
}

s32 fn_1_6998(Vec *arg0, float farg0)
{
    Vec sp18;
    Vec sp10;
    float var_f30;
    float temp_f28;
    float temp_f29;
    f64 temp_f26;
    f64 temp_f26_2;

    var_f30 = farg0;
    if (var_f30 > 1.0f) {
        var_f30 = 1.0f;
    }
    temp_f26 = sin(3.141592653589793 * (90.0f * var_f30) / 180.0);
    temp_f29 = sin(3.141592653589793 * (90.0f * var_f30) / 180.0) * temp_f26;
    sp18.x = lbl_1_bss_318.x + temp_f29 * (lbl_1_bss_30C.x - lbl_1_bss_318.x);
    sp18.y = lbl_1_bss_318.y + temp_f29 * (lbl_1_bss_30C.y - lbl_1_bss_318.y);
    sp18.z = lbl_1_bss_318.z + temp_f29 * (lbl_1_bss_30C.z - lbl_1_bss_318.z);
    var_f30 = farg0;
    if (var_f30 > 1.0f) {
        var_f30 = 1.0f;
    }
    temp_f26_2 = sin(3.141592653589793 * (90.0f * var_f30) / 180.0);
    temp_f28 = sin(3.141592653589793 * (90.0f * var_f30) / 180.0) * temp_f26_2;
    sp10.x = lbl_1_bss_300.x + temp_f28 * (lbl_1_bss_2F4.x - lbl_1_bss_300.x);
    sp10.y = lbl_1_bss_300.y + temp_f28 * (lbl_1_bss_2F4.y - lbl_1_bss_300.y);
    sp10.z = lbl_1_bss_300.z + temp_f28 * (lbl_1_bss_2F4.z - lbl_1_bss_300.z);
    Hu3DCameraPosSetV(1, &sp18, arg0, &sp10);
    if (1.0f <= farg0) {
        return 1;
    }
    lbl_1_bss_224 = 1;
    return 0;
}

s32 fn_1_6CCC(Vec *arg0, float farg0)
{
    Vec sp20;
    Vec sp18;
    Vec sp10;
    Vec spC;
    float var_f30;
    float var_f31;
    float temp_f28;
    float temp_f29;
    f64 temp_f26;
    f64 temp_f26_2;
    s32 i;

    for (i = 0; i < 4; i++) {
        var_f31 = farg0;
        if (var_f31 > 1.0f) {
            var_f31 = 1.0f;
        }
        temp_f26 = sin(3.141592653589793 * (90.0f * var_f31) / 180.0);
        temp_f29 = sin(3.141592653589793 * (90.0f * var_f31) / 180.0) * temp_f26;
        sp18.x = lbl_1_bss_2C4[i].x + temp_f29 * (lbl_1_bss_294[i].x - lbl_1_bss_2C4[i].x);
        sp18.y = lbl_1_bss_2C4[i].y + temp_f29 * (lbl_1_bss_294[i].y - lbl_1_bss_2C4[i].y);
        sp18.z = lbl_1_bss_2C4[i].z + temp_f29 * (lbl_1_bss_294[i].z - lbl_1_bss_2C4[i].z);
        var_f30 = farg0;
        if (var_f30 > 1.0f) {
            var_f30 = 1.0f;
        }
        temp_f26_2 = sin(3.141592653589793 * (90.0f * var_f30) / 180.0);
        temp_f28 = sin(3.141592653589793 * (90.0f * var_f30) / 180.0) * temp_f26_2;
        spC.x = lbl_1_bss_264[i].x + temp_f28 * (lbl_1_bss_234[i].x - lbl_1_bss_264[i].x);
        sp10.y = lbl_1_bss_264[i].y + temp_f28 * (lbl_1_bss_234[i].y - lbl_1_bss_264[i].y);
        sp10.z = lbl_1_bss_264[i].z + temp_f28 * (lbl_1_bss_234[i].z - lbl_1_bss_264[i].z);
        spC.y = sp10.y;
        spC.z = sp10.z;
        Hu3DCameraPosSetV(1 << i, &sp18, arg0, &spC);
    }
    if (1.0f <= farg0) {
        return 1;
    }
    lbl_1_bss_224 = 1;
    return 0;
}

float fn_1_70B0(float farg0, float farg1, float farg2)
{
    float sp8 = farg1;
    float spC = farg2;
    float var_f30;
    float var_f31;

    var_f31 = fmod(sp8 - farg0, 360.0);
    if (var_f31 < 0.0f) {
        var_f31 += 360.0f;
    }
    if (var_f31 > 180.0f) {
        var_f31 -= 360.0f;
    }
    var_f30 = fmod(farg0 + var_f31 * spC, 360.0);
    if (var_f30 < 0.0f) {
        var_f30 += 360.0f;
    }
    return var_f30;
}

void fn_1_71B4(s32 arg0)
{
    s32 var_r31 = arg0;

    if (lbl_1_bss_228 >= 0) {
        if (var_r31 < 0) {
            var_r31 = 0x3E8;
        }
        HuAudSeqFadeOut(lbl_1_bss_228, var_r31);
    }
    lbl_1_bss_228 = -1;
}

void fn_1_7220(s16 arg0, char *arg1, Vec *arg2)
{
    Mtx sp10;

    Hu3DModelObjMtxGet(arg0, arg1, sp10);
    arg2->x = sp10[0][3];
    arg2->y = sp10[1][3];
    arg2->z = sp10[2][3];
}

s32 fn_1_7278(s32 arg0, s32 arg1)
{
    s32 sp8 = arg1;

    return arg0 + frandmod(sp8 - arg0);
}

s32 fn_1_72B4(Vec *arg0, Vec *arg1, float farg0, float farg1)
{
    if ((arg1->x - arg0->x) * (arg1->x - arg0->x) + (arg1->z - arg0->z) * (arg1->z - arg0->z) <
        (farg0 + farg1) * (farg0 + farg1)) {
        return 1;
    }
    return 0;
}

s32 fn_1_7314(Vec *arg0, s32 arg1, s32 arg2)
{
    Vec sp10;
    s32 spC = arg2;

    Hu3D3Dto2D(arg0, 1, &sp10);
    return (f32) arg1 + (sp10.x / 576.0f) * (f32) (spC - arg1);
}

s32 fn_1_73D8(Vec *arg0, s32 arg1, s32 arg2)
{
    Vec sp10;
    s32 spC = arg2;

    Hu3D3Dto2D(arg0, 1, &sp10);
    return (f32) arg1 + (sp10.y / 480.0f) * (f32) (spC - arg1);
}