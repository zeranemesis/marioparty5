#include "REL/m563dll/math.c"

#include "game/data.h"
#include "game/frand.h"
#include "game/gamemes.h"
#include "game/hu3d.h"
#include "game/hsfex.h"
#include "game/memory.h"
#include "game/mg/actman.h"
#include "game/mg/seqman.h"
#include "game/object.h"
#include "game/wipe.h"

#include "REL/m563dll.h"

#include "datanum/char.h"
#include "datanum/charmot.h"
#include "datanum/mgconst.h"

typedef void (*M563ObjFunc)(OMOBJ *obj);

s32 _prolog(void);
void _epilog(void);
void ObjectSetup(void);

void fn_1_9A0(OMOBJ *obj);
void fn_1_EC0(OMOBJ *obj);
s32 fn_1_12A4(OMOBJ *obj);
void fn_1_1340(s32 arg0, s32 arg1, u32 arg2);
void fn_1_13F4(s32 arg0, s32 arg1);
void fn_1_1488(OMOBJ *obj);
void fn_1_15AC(OMOBJ *obj);
void fn_1_162C(OMOBJ *obj);
void fn_1_16FC(OMOBJ *obj);
void fn_1_18BC(OMOBJ *obj);
void fn_1_1E7C(OMOBJ *obj);
void fn_1_2244(OMOBJ *obj);
s32 fn_1_2998(OMOBJ *obj);
void fn_1_2AC0(OMOBJ *obj);
s32 fn_1_2B28(Vec *arg0);
void fn_1_2B90(s16 mode, s16 frameNo);
void fn_1_2C5C(s16 mode, s16 frameNo);
void fn_1_2EB0(s16 mode, s16 frameNo);
void fn_1_2F34(s16 mode, s16 frameNo);
void fn_1_305C(s16 mode, s16 frameNo);
void fn_1_359C(s16 mode, s16 frameNo);
void fn_1_3A74(void);
void fn_1_3A78(void);
void fn_1_3AC4(s32 arg0);
s32 fn_1_3EF0(void);
void fn_1_4068(OMOBJ *obj);
void fn_1_41B0(OMOBJ *obj);
void fn_1_4334(OMOBJ *obj);
f32 fn_1_473C(f32 arg0, f32 arg1);
f32 fn_1_4868(f32 arg0, f32 arg1);
s32 fn_1_49F4(Vec *arg0, Vec *arg1, f32 arg2, f32 arg3);
void fn_1_4BEC(f32 arg0, f32 arg1, f32 arg2);
void fn_1_4CB8(OMOBJ *obj);
void fn_1_4D2C(OMOBJ *obj);
void fn_1_4E54(OMOBJ *obj);
void fn_1_4F2C(s32 arg0, f32 arg1, f32 arg2, f32 arg3);
void fn_1_5004(OMOBJ *obj);
void fn_1_5078(OMOBJ *obj);
void fn_1_534C(OMOBJ *obj);
s32 fn_1_5424(Vec *arg0, Vec *arg1, f32 arg2);
s32 fn_1_557C(Vec *arg0, Vec *arg1);
s32 fn_1_56C8(Vec *arg0, Vec *arg1, s32 *arg2, f32 arg3);
s32 fn_1_5814(Vec *arg0, Vec *arg1, f32 arg4, s32 *arg2, s32 *arg3);
s32 fn_1_5974(Vec *arg0, Vec *arg1, f32 arg2);
s32 fn_1_5A7C(Vec *arg0);
void fn_1_5B14(s32 arg0);
void fn_1_5BC0(void);
s32 fn_1_5D40();
void fn_1_5F14(OMOBJ *obj, s32 arg1, f32 arg3, u32 arg2);
void fn_1_6038(OMOBJ *obj, s16 arg1);
void fn_1_60B4(OMOBJ *obj);
s32 fn_1_60D4(s32 arg0, s32 arg1);
f32 fn_1_6110(f32 arg0, f32 arg1, f32 arg2, f32 arg3);
void fn_1_6280(void);
void fn_1_660C(OMOBJ *obj);
void fn_1_66D4(void);
void fn_1_66F8(u16 arg0, s16 arg1);
s16 fn_1_6780(s16 arg0);
void fn_1_6824(void);
s16 fn_1_687C(u32 arg0);
void fn_1_6990(f32 arg3, Vec *arg0, Vec *arg1, Vec *arg2);
void fn_1_6AEC(Point3d *arg0, Point3d *arg1, Point3d *arg2);
s32 fn_1_6B78(Point3d *arg0, f32 arg1);
s32 fn_1_6EAC(Point3d *arg0, f32 arg1);
f32 fn_1_7290(f32 arg0, f32 arg1, f32 arg2);
void fn_1_7394(s32 arg0);
void fn_1_7400(s16 arg0, char *arg1, Vec *arg2);
s32 fn_1_7458(s32 arg0, s32 arg1);
s32 fn_1_7494(Vec *arg0, Vec *arg1, f32 arg2, f32 arg3);
s32 fn_1_74F4(Point3d *arg0, s32 arg1, s32 arg2);
s32 fn_1_75B8(Point3d *arg0, s32 arg1, s32 arg2);


typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

typedef struct M563ObjData_s {
    s32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    s16 unk30;
    s16 unk32;
    s16 unk34[2];
    s32 unk38[2];
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
} M563ObjData;

typedef struct M563PlayerData_s {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    f32 unk34;
    f32 unk38;
s32 unk3C;
    s32 unk40;
    s16 unk44[5];
    s16 unk4E;
    s16 unk50;
    s16 unk52[5];
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    s32 unk68;
    s32 unk6C;
    Vec unk70;
    f32 unk7C;
    s32 unk80;
    s32 unk84;
    s32 unk88;
s32 unk8C;
    s32 unk90[2];
    s32 unk98;
    s32 unk9C;
    s32 unkA0;
    s32 unkA4;
    s32 unkA8;
    s32 unkAC;
} M563PlayerData;

typedef struct M563CoinData_s {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    f32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
} M563CoinData;

typedef struct M563ModelSet_s {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
} M563ModelSet;

typedef struct M563ModeHook_s {
    u32 unk0;
    s16 unk4;
} M563ModeHook;

u32 lbl_1_data_0[0xD] = { MGCONST_ANM_marioSmall, MGCONST_ANM_luigiSmall, MGCONST_ANM_peachSmall,
    MGCONST_ANM_yoshiSmall, MGCONST_ANM_warioSmall, MGCONST_ANM_daisySmall, MGCONST_ANM_waluigiSmall,
    MGCONST_ANM_kinopioSmall, MGCONST_ANM_teresaSmall, MGCONST_ANM_minikoopaSmall, MGCONST_ANM_minikoopaRSmall,
    MGCONST_ANM_minikoopaGSmall, MGCONST_ANM_minikoopaBSmall };

u32 lbl_1_data_34[5] = { CHARMOT_HSF_c000m1_300, CHAR_HSF_c000m1_417, CHARMOT_HSF_c000m1_360,
    CHARMOT_HSF_c000m1_301, CHARMOT_HSF_c000m1_302 };

OMCAMERAVIEW lbl_1_data_48[2] = {
    { { 0.0f, 387.0f, -500.0f }, { 0.0f, 0.0f, 0.0f }, 500.0f },
    { { 0.0f, 985.0f, -500.0f }, { 0.0f, 0.0f, 0.0f }, 1009.0f },
};

OMCAMERAVIEW lbl_1_data_80 = { { 0.0f, 210.0f, 50.0f }, { -36.6f, 0.3f, 0.0f }, 2016.0f };
OMCAMERAVIEW lbl_1_data_9C = { { 0.0f, 386.86f, 50.0f }, { 1.57f, 0.3f, 0.0f }, 1522.0f };

Vec lbl_1_data_B8[4] = {
    { -480.0f, 243.0f, -315.0f },
    { 480.0f, 243.0f, -315.0f },
    { -430.0f, 93.0f, 310.0f },
    { 430.0f, 93.0f, 310.0f },
};

Vec lbl_1_data_E8[4] = {
    { -239.66f, 137.86f, 297.0f },
    { -100.36f, 155.07f, 297.0f },
    { 100.36f, 155.07f, 297.0f },
    { 239.66f, 137.86f, 297.0f },
};

Vec lbl_1_data_118[2] = {
    { -50.0f, 300.0f, -895.0f },
    { 50.0f, 300.0f, -895.0f },
};

Vec lbl_1_data_130 = { 0.0f, 300.0f, -895.0f };

u32 lbl_1_data_13C[12] = { DATANUM(DATA_m563, 3), DATANUM(DATA_m563, 4), DATANUM(DATA_m563, 5),
    DATANUM(DATA_m563, 6), DATANUM(DATA_m563, 7), DATANUM(DATA_m563, 8), DATANUM(DATA_m563, 9),
    DATANUM(DATA_m563, 10), DATANUM(DATA_m563, 11), DATANUM(DATA_m563, 12), DATANUM(DATA_m563, 13),
    DATANUM(DATA_m563, 14) };

Point3d lbl_1_data_16C = { -5000.0f, 10000.0f, 0.0f };
Point3d lbl_1_data_178 = { 0.0f, 0.0f, 0.0f };
GXColor lbl_1_data_184 = { 0xFF, 0xFF, 0xFF, 0xFF };
Point3d lbl_1_data_188 = { -100.0f, 8000.0f, 0.0f };
Point3d lbl_1_data_194 = { 0.0f, 1.0f, 0.0f };
Point3d lbl_1_data_1A0 = { 0.0f, 0.0f, 0.0f };

MGSEQ_PARAM lbl_1_data_1AC = { 30, 0, fn_1_2B90, fn_1_2C5C, NULL, fn_1_2EB0, fn_1_2F34, fn_1_305C,
    fn_1_359C, NULL, NULL };

s32 lbl_1_data_1D4[12] = { 17, 18, 20, 1, 6, 12, 17, 18, 20, 1, 6, 12 };

f32 lbl_1_data_204[4][4] = { { -53.0f, -3.625f, 0.0f, 0.0f }, { 0.0f, -3.625f, 700.0f, 0.0f },
    { -53.0f, 0.0f, 0.0f, 3.625f }, { 0.0f, 0.0f, 700.0f, 3.625f } };

Vec lbl_1_data_244[4] = {
    { 900.0f, 2000.0f, 750.0f },
    { -900.0f, 2000.0f, 750.0f },
    { 900.0f, 2000.0f, -750.0f },
    { -900.0f, 2000.0f, -750.0f },
};

s32 lbl_1_data_274[3][4] = { { 0, 1, 2, 3 }, { 1, 0, 3, 2 }, { 0, 3, 2, 1 } };

typedef struct M563Pos2D_s {
    f32 x;
    f32 y;
} M563Pos2D;

M563Pos2D lbl_1_data_2A4[4] = { 64.0f, 60.0f, 168.0f, 60.0f, 408.0f, 60.0f, 512.0f, 60.0f };

char lbl_1_data_2C4[] = "******* M563 ObjectSetup *********\n";
char lbl_1_data_2E8[] = "itemhook_R";
char lbl_1_data_324[] = "%d pdp->SearchWaitMax:%d\n";
char lbl_1_data_33E[] = "%d POINT:%d\n";
char lbl_1_data_34B[] = "FLO:%d\n";

s32 lbl_1_bss_4;
s32 lbl_1_bss_8;
MGPLAYER *lbl_1_bss_14[4];
HU3DMOTID lbl_1_bss_24[0xC];
s32 lbl_1_bss_3C;
s32 lbl_1_bss_40[3];
s32 lbl_1_bss_4C;
s32 lbl_1_bss_50[4];
s32 lbl_1_bss_60[1];
s32 lbl_1_bss_64;
s32 lbl_1_bss_68;
s32 lbl_1_bss_6C;
s32 lbl_1_bss_70;
f32 lbl_1_bss_74;
HU3DLIGHTID lbl_1_bss_78;
s32 lbl_1_bss_7C;
s32 lbl_1_bss_80;
s32 lbl_1_bss_84;
s32 lbl_1_bss_88;
s32 lbl_1_bss_8C;
s32 lbl_1_bss_90;
s32 lbl_1_bss_94;
s32 lbl_1_bss_98;
Point3d lbl_1_bss_9C;
Point3d lbl_1_bss_A8;
Point3d lbl_1_bss_B4;
Point3d lbl_1_bss_C0[4];
HU3DMODELID lbl_1_bss_F0[4];
M563ModelSet lbl_1_bss_F8;
OMOBJ *lbl_1_bss_110;
OMOBJ *lbl_1_bss_118;
OMOBJ **lbl_1_bss_11C;
OMOBJ **lbl_1_bss_124;
OMOBJ **lbl_1_bss_128;
OMOBJ **lbl_1_bss_130;
OMOBJ *lbl_1_bss_134[0x1E];
M563ModeHook lbl_1_bss_1AC[0x20];
s32 lbl_1_bss_2B0;
s32 lbl_1_bss_2B4;
Point3d lbl_1_bss_2C0[4];
Point3d lbl_1_bss_2F0[4];
Point3d lbl_1_bss_320[4];
Point3d lbl_1_bss_350[4];
Point3d lbl_1_bss_380;
Point3d lbl_1_bss_38C;
Point3d lbl_1_bss_398;
Point3d lbl_1_bss_3A4;
HUPROCESS *lbl_1_bss_3B0;
s32 lbl_1_bss_3B4[3];

s32 _prolog(void)
{
    const VoidFunc *ctor = _ctors;

    while (*ctor != NULL) {
        (*ctor)();
        ctor++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtor = _dtors;

    while (*dtor != NULL) {
        (*dtor)();
        dtor++;
    }
}

void ObjectSetup(void)
{
    OMOBJ *obj;
    s32 i;

    OSReport("******* M563 ObjectSetup *********\n");
    lbl_1_bss_3B0 = MgActorObjectSetup();
    lbl_1_bss_2B4 = -1;
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 45.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    omCameraViewSet(&lbl_1_data_48[0]);
    lbl_1_bss_118 = omAddObjEx(lbl_1_bss_3B0, 0x7FDA, 0, 0, -1, omOutView);
    lbl_1_bss_118->work[0] = 2;
    lbl_1_bss_78 = Hu3DGLightCreate(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    Hu3DGLightPosAimSetV(lbl_1_bss_78, &lbl_1_data_16C, &lbl_1_data_178);
    Hu3DGLightStaticSet(lbl_1_bss_78, TRUE);
    Hu3DGLightInfinitytSet(lbl_1_bss_78);
    Hu3DGLightColorSet(lbl_1_bss_78, lbl_1_data_184.r, lbl_1_data_184.g, lbl_1_data_184.b, lbl_1_data_184.a);
    Hu3DShadowCreate(10.0f, 10.0f, 10000.0f);
    Hu3DShadowColSet(0x10, 0x10, 0x10);
    Hu3DShadowPosSet(&lbl_1_data_188, &lbl_1_data_194, &lbl_1_data_1A0);
    Hu3DShadowTPLvlSet(0.3f);
    omAddObjEx(lbl_1_bss_3B0, 0x67, 0xC, 0, -1, fn_1_18BC);
    omAddObjEx(lbl_1_bss_3B0, 0x67, 0, 0, -1, fn_1_1488);
    omMakeGroupEx(lbl_1_bss_3B0, 5, 1);
    lbl_1_bss_11C = omGetGroupMemberListEx(lbl_1_bss_3B0, 5);
    obj = omAddObjEx(lbl_1_bss_3B0, 0x69, 0, 0, 5, fn_1_9A0);
    *lbl_1_bss_11C = obj;
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, 0x50, HU_MEMNUM_OVL);
    obj->work[0] = 0;
lbl_1_bss_3B4[1] = 0;
    lbl_1_bss_3B4[2] = 0;
    for (i = 0; i < 0x1E; i++) {
        lbl_1_bss_134[i] = NULL;
    }
    omMakeGroupEx(lbl_1_bss_3B0, 1, 0x1E);
    omGetGroupMemberListEx(lbl_1_bss_3B0, 1);
    lbl_1_bss_98 = 0;
    lbl_1_bss_90 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 0), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_90, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(lbl_1_bss_90, HU3D_ATTR_NONE);
    lbl_1_bss_84 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 1), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_84, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(lbl_1_bss_84, HU3D_ATTR_NONE);
    omMakeGroupEx(lbl_1_bss_3B0, 2, 0x1E);
    lbl_1_bss_130 = omGetGroupMemberListEx(lbl_1_bss_3B0, 2);
    lbl_1_bss_94 = 0;
    lbl_1_bss_8C = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 12), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_8C, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(lbl_1_bss_8C, HU3D_ATTR_NONE);
    lbl_1_bss_88 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 13), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_88, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(lbl_1_bss_88, HU3D_ATTR_NONE);
    omAddObjEx(lbl_1_bss_3B0, 0x65, 0, 0, -1, fn_1_162C);
    omAddObjEx(lbl_1_bss_3B0, 0x6A, 0, 0, -1, fn_1_660C);
    lbl_1_bss_4 = 0;
    lbl_1_bss_8 = 0;
    omMakeGroupEx(lbl_1_bss_3B0, 0, 4);
    lbl_1_bss_124 = omGetGroupMemberListEx(lbl_1_bss_3B0, 0);
    for (i = 0; i < 4; i++) {
        obj = omAddObjEx(lbl_1_bss_3B0, 0x64, 0, 0, 0, fn_1_1E7C);
        lbl_1_bss_124[i] = obj;
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, 0xB0, HU_MEMNUM_OVL);
        obj->work[0] = i;
    }
    omMakeGroupEx(lbl_1_bss_3B0, 4, 0xA);
    lbl_1_bss_128 = omGetGroupMemberListEx(lbl_1_bss_3B0, 4);
    lbl_1_bss_80 = 0;
    lbl_1_bss_7C = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 14), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_7C, HU3D_ATTR_DISPOFF);
    lbl_1_bss_3C = 0;
    for (i = 0; i < 4; i++) {
        lbl_1_bss_F0[i] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 2), HU_MEMNUM_OVL, HEAP_MODEL));
        lbl_1_bss_C0[i].x = lbl_1_data_E8[i].x;
        lbl_1_bss_C0[i].y = 135.0f + (800.0f + lbl_1_data_E8[i].y);
        lbl_1_bss_C0[i].z = lbl_1_data_E8[i].z;
        Hu3DModelPosSet(lbl_1_bss_F0[i], lbl_1_bss_C0[i].x, lbl_1_bss_C0[i].y, lbl_1_bss_C0[i].z);
        Hu3DModelRotSet(lbl_1_bss_F0[i], 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrSet(lbl_1_bss_F0[i], HU3D_ATTR_DISPOFF);
    }
    MgSeqCreate(&lbl_1_data_1AC);
}

void fn_1_9A0(OMOBJ *obj)
{
    M563ObjData *data = obj->data;
    s32 i;

    data->unk34[0] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 21), HU_MEMNUM_OVL, HEAP_MODEL));
    data->unk34[1] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 22), HU_MEMNUM_OVL, HEAP_MODEL));
    data->unk30 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 3), HU_MEMNUM_OVL, HEAP_MODEL));
    data->unk32 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 4), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(data->unk32, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(data->unk32, 0x40000002);
    Hu3DModelPosSet(data->unk32, 50.0f, 300.0f, -620.0f);
    data->unk18 = 0.0f;
    data->unk1C = 0.0f;
    data->unk20 = 0.0f;
    data->unk24 = 0.0f;
    data->unk28 = 0.0f;
    data->unk2C = 0.0f;
    Hu3DModelPosSet(data->unk30, data->unk18, data->unk1C, data->unk20);
    Hu3DModelRotSet(data->unk30, data->unk24, data->unk28, data->unk2C);
    Hu3DModelHookSet(data->unk34[1], lbl_1_data_2E8, data->unk30);
    for (i = 0; i < 2; i++) {
        Hu3DModelAttrSet((i == 0) ? data->unk34[0] : data->unk34[1], 0x40000001);
        Hu3DModelPosSet((i == 0) ? data->unk34[0] : data->unk34[1], lbl_1_data_118[i].x, lbl_1_data_118[i].y, lbl_1_data_118[i].z);
        Hu3DModelRotSet((i == 0) ? data->unk34[0] : data->unk34[1], 0.0f, 0.0f, 0.0f);
        Hu3DModelShadowSet((i == 0) ? data->unk34[0] : data->unk34[1]);
    }
    lbl_1_bss_24[0] = Hu3DJointMotion(data->unk34[0], HuDataSelHeapReadNum(lbl_1_data_13C[0], HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_24[1] = Hu3DJointMotion(data->unk34[1], HuDataSelHeapReadNum(lbl_1_data_13C[1], HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_24[2] = Hu3DJointMotion(data->unk34[0], HuDataSelHeapReadNum(lbl_1_data_13C[2], HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_24[3] = Hu3DJointMotion(data->unk34[1], HuDataSelHeapReadNum(lbl_1_data_13C[3], HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_24[4] = Hu3DJointMotion(data->unk34[0], HuDataSelHeapReadNum(lbl_1_data_13C[4], HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_24[5] = Hu3DJointMotion(data->unk34[1], HuDataSelHeapReadNum(lbl_1_data_13C[5], HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_24[6] = Hu3DJointMotion(data->unk34[0], HuDataSelHeapReadNum(lbl_1_data_13C[6], HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_24[7] = Hu3DJointMotion(data->unk34[1], HuDataSelHeapReadNum(lbl_1_data_13C[7], HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_24[8] = Hu3DJointMotion(data->unk34[1], HuDataSelHeapReadNum(lbl_1_data_13C[8], HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_24[9] = Hu3DJointMotion(data->unk34[1], HuDataSelHeapReadNum(lbl_1_data_13C[9], HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_24[10] = Hu3DJointMotion(data->unk34[0], HuDataSelHeapReadNum(lbl_1_data_13C[10], HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_24[11] = Hu3DJointMotion(data->unk34[1], HuDataSelHeapReadNum(lbl_1_data_13C[11], HU_MEMNUM_OVL, HEAP_MODEL));
    fn_1_1340(0, 0, 0x40000001);
    fn_1_1340(1, 8, 0x40000001);
    data->unk0 = lbl_1_data_130.x;
    data->unk4 = lbl_1_data_130.y;
    data->unk8 = lbl_1_data_130.z;
    data->unk14 = 0.0f;
    data->unk10 = 0.0f;
    data->unkC = 0.0f;
    data->unk40 = 1;
    data->unk44 = 1;
    data->unk48 = 0;
    obj->objFunc = fn_1_EC0;
}

void fn_1_EC0(OMOBJ *obj)
{
    M563ObjData *data = obj->data;

    switch (data->unk40) {
    case 0:
        fn_1_1340(0, 2, 0x40000001);
        fn_1_1340(1, 3, 0x40000001);
        data->unk8 += 1.8333334f;
        Hu3DModelPosSet(data->unk34[0], data->unk0 - 50.0f, data->unk4, data->unk8);
        Hu3DModelPosSet(data->unk34[1], data->unk0 + 50.0f, data->unk4, data->unk8);
        return;
    case 1:
        fn_1_1340(0, 0, 0x40000001);
        fn_1_1340(1, 9, 0x40000001);
        return;
    case 2:
        fn_1_13F4(1, 8);
        return;
    case 3:
        fn_1_1340(0, 0, 0x40000001);
        fn_1_1340(1, 9, 0);
        return;
    case 4:
        fn_1_1340(0, 0, 0x40000001);
        fn_1_1340(1, 1, 0x40000001);
        return;
    case 5:
        fn_1_1340(0, 0, 0x40000001);
        fn_1_1340(1, 1, 0x40000001);
        data->unk48++;
        if (data->unk48 > 0x3C) {
            data->unk40 = fn_1_12A4(obj);
            data->unk48 = 0;
        }
        return;
    case 6:
        fn_1_1340(0, 4, 0);
        fn_1_1340(1, 5, 0);
        data->unk48++;
        if (data->unk48 > 0x134) {
            data->unk40 = 5;
            data->unk48 = 0;
        }
        return;
    case 7:
        fn_1_1340(0, 4, 0);
        fn_1_1340(1, 7, 0);
        data->unk48++;
        if (data->unk48 > 0x134) {
            data->unk40 = 5;
            data->unk48 = 0;
        }
        return;
    case 8:
        fn_1_1340(0, 6, 0);
        fn_1_1340(1, 7, 0);
        data->unk48++;
        if (data->unk48 > 0x134) {
            data->unk40 = 5;
            data->unk48 = 0;
        }
        return;
    case 9:
        fn_1_1340(0, 10, 0x40000001);
        fn_1_1340(1, 11, 0x40000001);
        return;
    case 10:
        fn_1_1340(0, 2, 0x40000001);
        fn_1_1340(1, 3, 0x40000001);
        data->unk10 -= 6.0f;
        Hu3DModelRotSet(data->unk34[0], data->unkC, data->unk10, data->unk14);
        Hu3DModelRotSet(data->unk34[1], data->unkC, 360.0f - data->unk10, data->unk14);
        return;
    case 11:
        fn_1_1340(0, 2, 0x40000001);
        fn_1_1340(1, 3, 0x40000001);
        data->unk8 -= 2.2916667f;
        Hu3DModelPosSet(data->unk34[0], data->unk0 - 50.0f, data->unk4, data->unk8);
        Hu3DModelPosSet(data->unk34[1], data->unk0 + 50.0f, data->unk4, data->unk8);
        return;
    }
}

s32 fn_1_12A4(OMOBJ *obj)
{
    M563ObjData *data = obj->data;
    s32 result;
    s32 rnd;

    do {
        rnd = frandmod(3);
        switch (rnd) {
        case 0:
            result = 6;
            break;
        case 1:
            result = 7;
            break;
        case 2:
            result = 8;
            break;
        }
    } while (result == data->unk44);

    data->unk44 = result;
    return result;
}

void fn_1_1340(s32 arg0, s32 arg1, u32 arg2)
{
    M563ObjData *data = (*lbl_1_bss_11C)->data;

    if (data->unk38[arg0] != arg1) {
        data->unk38[arg0] = arg1;
        Hu3DMotionShiftSet(data->unk34[arg0], lbl_1_bss_24[arg1], 0.0f, 8.0f, arg2);
    }
}

void fn_1_13F4(s32 arg0, s32 arg1)
{
    M563ObjData *data = (*lbl_1_bss_11C)->data;

    if (data->unk38[arg0] != arg1) {
        data->unk38[arg0] = arg1;
        Hu3DMotionSet(data->unk34[arg0], lbl_1_bss_24[arg1]);
    }
}

void fn_1_1488(OMOBJ *obj)
{
    f32 waterMtx[2][3] = { { 0.01f, 0.0f, 0.0f }, { 0.0f, 0.01f, 0.0f } };
    Vec posMin = { -1.25f, -10.0f, -1.75f };
    Vec posMax = { 500.0f, -10.0f, 800.0f };

    Hu3DWaterCreate(1, HU3D_WATER_ANIM_NONE, HU3D_WATER_ANIM_NONE, HU3D_WATER_ANIM_NONE, TRUE, &posMin, &posMax);
    Hu3DWaterTexScaleSet(1.5f, 3.0f, 1.5f);
    Hu3DWaterHiliteSet(0.3f);
    Hu3DWaterIndTexMtxSet(waterMtx);
    lbl_1_bss_74 = 0.0f;
    obj->objFunc = fn_1_15AC;
}

void fn_1_15AC(OMOBJ *obj)
{
    lbl_1_bss_74 += 1.0f;
    Hu3DWaterTexPosSet(0.0005 * lbl_1_bss_74, 0.0f, 0.0f);
}

void fn_1_162C(OMOBJ *obj)
{
    lbl_1_bss_70 = 0;
    lbl_1_bss_68 = 0;
    lbl_1_bss_64 = 0;
    lbl_1_bss_60[0] = 0;
    lbl_1_bss_4C = 0;
    fn_1_79E8();
    lbl_1_bss_40[0] = fn_1_60D4(1, 0xA);
    lbl_1_bss_40[1] = fn_1_60D4(0xB, 0x14);
    lbl_1_bss_40[2] = fn_1_60D4(0x15, 0x1C);
    obj->objFunc = fn_1_16FC;
}

void fn_1_16FC(OMOBJ *obj)
{
    u32 mode;
    u32 remain;

    if (3U < MgSeqModeGet() && MgSeqModeGet() <= 5U) {
        remain = MgSeqTimerValueGet();
        remain = remain / 60;
        remain = 0x1E - remain;
        if (remain != 0 && remain < 0xB) {
            lbl_1_bss_70 = 0;
            lbl_1_bss_6C = 0x1E;
        } else if (0xA < remain && remain < 0x15) {
            lbl_1_bss_70 = 1;
            lbl_1_bss_6C = 0x14;
        } else if (0x14 < remain && remain < 0x1E) {
            lbl_1_bss_70 = 2;
            lbl_1_bss_6C = 0xF;
        }
        lbl_1_bss_64++;
        if (lbl_1_bss_64 >= 0x5A) {
            lbl_1_bss_64 = 0;
            lbl_1_bss_68++;
            if (lbl_1_bss_68 >= 4) {
                lbl_1_bss_68 = 0;
            }
        }
        lbl_1_bss_60[0]++;
        if (lbl_1_bss_60[0] >= lbl_1_bss_6C) {
            lbl_1_bss_60[0] = 0;
            fn_1_3AC4((s32)(void *)lbl_1_bss_60);
        }
    }
}

void fn_1_18BC(OMOBJ *obj)
{
    M563ModelSet *ms = &lbl_1_bss_F8;
    COL_ATTRPARAM colAttr;
    MGACTOR_PARAM actorParam;
    s16 mdlIds[12];
    s16 *mdl = mdlIds;
    s32 i;

    mdl[0] = ms->unk0 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 5), HU_MEMNUM_OVL, HEAP_MODEL));
    ms->unk2 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 10), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(ms->unk2, 0x40000001);
    mdl[1] = ms->unk4 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 11), HU_MEMNUM_OVL, HEAP_MODEL));
    mdl[2] = ms->unk6 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 6), HU_MEMNUM_OVL, HEAP_MODEL));
    mdl[3] = ms->unk8 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 7), HU_MEMNUM_OVL, HEAP_MODEL));
    mdl[4] = ms->unkA = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 8), HU_MEMNUM_OVL, HEAP_MODEL));
    mdl[5] = ms->unkC = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 9), HU_MEMNUM_OVL, HEAP_MODEL));
    mdl[6] = ms->unkE = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 15), HU_MEMNUM_OVL, HEAP_MODEL));
    mdl[7] = ms->unk10 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 16), HU_MEMNUM_OVL, HEAP_MODEL));
    mdl[8] = ms->unk12 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 17), HU_MEMNUM_OVL, HEAP_MODEL));
    mdl[9] = ms->unk14 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 18), HU_MEMNUM_OVL, HEAP_MODEL));
    mdl[10] = ms->unk16 = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m563, 19), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelShadowMapSet(ms->unk0);
    Hu3DModelAttrSet(ms->unk0, 0x40000001);
    Hu3DModelAttrSet(ms->unk4, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(ms->unk8, HU3D_ATTR_NONE);
    Hu3DMotionSpeedSet(ms->unk8, 0.0f);
    Hu3DMotionClusterSpeedSet(ms->unk8, 0, 0.0f);
    Hu3DModelAttrSet(ms->unkA, 0x40000001);
    Hu3DModelAttrSet(ms->unkE, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(ms->unk10, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(ms->unk12, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(ms->unk14, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(ms->unk16, HU3D_ATTR_DISPOFF);

    actorParam.height = 150.0f;
    actorParam.radius = 50.0f;
    actorParam.param = 0;
    actorParam.type = 0;
    actorParam.narrowHook = NULL;
    actorParam.correctHook = NULL;
    actorParam.correctHookParam = 0;
    actorParam.attr = 0;
    MgActorColMapInit(mdlIds, 5, 9);

    colAttr.type = 2;
    colAttr.speed = 1.0f;
    colAttr.yDeviate = 0.7f;
    colAttr.maxDot = 1.0f;
    MgActorColAttrParamSet(&colAttr, 2);

    colAttr.type = 4;
    colAttr.speed = 1.0f;
    colAttr.yDeviate = 0.0f;
    colAttr.maxDot = 1.0f;
    MgActorColAttrParamSet(&colAttr, 4);

    for (i = 0; i < 4; i++) {
        lbl_1_bss_14[i] = MgPlayerCreate(i, &actorParam, 2, 1, 0x23, NULL);
        MgPlayerVibrateCreate(lbl_1_bss_14[i]);
        actorParam.param++;
    }
    for (i = 0; i < 4; i++) {
        lbl_1_bss_14[i]->actor->pos.x = lbl_1_data_B8[i].x;
        lbl_1_bss_14[i]->actor->pos.y = lbl_1_data_B8[i].y;
        lbl_1_bss_14[i]->actor->pos.z = lbl_1_data_B8[i].z;
    }
    fn_1_5B14(0);
    for (i = 0; i < 4; i++) {
        Hu3DModelShadowSet(lbl_1_bss_14[i]->actor->mdlId);
        Hu3DModelLayerSet(lbl_1_bss_14[i]->actor->mdlId, 1);
    }
    obj->objFunc = NULL;
}

void fn_1_1E7C(OMOBJ *obj)
{
    M563PlayerData *data = obj->data;
    s32 i;

    data->unk0 = obj->work[0];
    data->unk4 = GwPlayerConf[data->unk0].charNo;
    data->unk8 = 0;
    data->unk34 = 0.0f;
    data->unk38 = 0.0f;
    data->unk98 = 0;
    data->unk9C = 0;
    data->unkA0 = 0;
    data->unkA8 = 0;
    data->unkA4 = 0;
    data->unk3C = 0;
    for (i = 0; i < 5; i++) {
        data->unk44[i] = CharMotionCreate(data->unk4, lbl_1_data_34[i]);
    }
    data->unk4E = 5;
    data->unk50 = 0;
    data->unk68 = 2;
    data->unk6C = GwPlayerConf[data->unk0].dif;
    data->unk8C = 0;
    data->unkAC = 0;
    data->unk88 = 0;
    switch (data->unk6C) {
    case 0:
        data->unk7C = 800.0f;
        data->unk84 = fn_1_60D4(0x32, 0x3C);
        break;
    case 1:
        data->unk7C = 800.0f;
        data->unk84 = fn_1_60D4(0x23, 0x2D);
        break;
    case 2:
        data->unk7C = 800.0f;
        data->unk84 = fn_1_60D4(0xA, 0x19);
        break;
    case 3:
        data->unk7C = 800.0f;
        data->unk84 = fn_1_60D4(1, 5);
        break;
    }
    OSReport("%d pdp->SearchWaitMax:%d\n", data->unk0, data->unk84);
    data->unk5C = MgScoreWinCreate(0x60, 0x28);
    MgScoreWinTPLvlSet(data->unk5C, 0.85f);
    MgScoreWinPosSet(data->unk5C, lbl_1_data_2A4[data->unk0].x, lbl_1_data_2A4[data->unk0].y);
    MgScoreWinDispSet(data->unk5C, FALSE);
    data->unk64 = espEntry(lbl_1_data_0[data->unk4], 0, 0);
    espDispOff(data->unk64);
    espDrawNoSet(data->unk64, 0);
    espPosSet(data->unk64, lbl_1_data_2A4[data->unk0].x - 24.0f, lbl_1_data_2A4[data->unk0].y);
    espAttrSet(data->unk64, 1);
    data->unk60 = MgScoreCreate(0x960031, -1, TRUE);
    MgScoreValueSet(data->unk60, data->unk8);
    MgScorePosSet(data->unk60, lbl_1_data_2A4[data->unk0].x + 8.0f, lbl_1_data_2A4[data->unk0].y);
    MgScoreMaxDigitSet(data->unk60, 2);
    MgScoreDispOff(data->unk60);
    obj->objFunc = fn_1_2244;
}

void fn_1_2244(OMOBJ *obj)
{
    M563PlayerData *data = obj->data;
    s32 isWinner;
    u32 mode;
    Vec wavePos;

    if (MgSeqModeGet() == 5) {
        if (lbl_1_bss_14[data->unk0]->motNo == 1 || lbl_1_bss_14[data->unk0]->motNo == 2) {
            isWinner = 1;
        } else {
            isWinner = 0;
        }
        data->unk30 = lbl_1_bss_14[data->unk0]->actor->colGroundAttr;
        if (GwPlayerConf[data->unk0].type != 0) {
            data->unk98 = 0;
            data->unk9C = 0;
            data->unkA0 = 0;
            if (data->unkA8 == 0) {
                data->unkA4 = 0;
            }
            switch (data->unk68) {
            case 0:
                if (fn_1_56C8((Vec *)&data->unkC, (Vec *)&data->unk70, &data->unk80, data->unk7C)) {
                    data->unk68 = 1;
                } else {
                    data->unk68 = 2;
                }
                break;
            case 1:
                if (fn_1_2998(obj)) {
                    data->unk68 = 2;
                } else if ((data->unk30 & 0xBF80) != 0) {
                    if (data->unkAC == 0) {
                        data->unkA0 |= 0x100;
                        data->unkA4 |= 0x100;
                        data->unkAC = 1;
                    }
                } else {
                    data->unkAC = 0;
                    data->unkA8 = 0;
                }
                if (data->unkAC != 0) {
                    if (data->unkA8 < 0xA) {
                        data->unkA8++;
                        data->unkA0 |= 0x100;
                        data->unkA4 |= 0x100;
                    } else {
                        data->unkAC = 0;
                    }
                }
                break;
            case 2:
                data->unk88++;
                if (data->unk88 > data->unk84) {
                    data->unk68 = 0;
                    data->unk88 = 0;
                }
                break;
            }
            MgPlayerPadSet(lbl_1_bss_14[data->unk0], data->unk98, data->unk9C, data->unkA0, data->unkA4);
        }
        if (lbl_1_bss_14[data->unk0]->actor->velY == 0.0f) {
            data->unk2C = lbl_1_bss_14[data->unk0]->actor->colGroundAttr;
        } else {
            data->unk28 = 0;
            data->unk24 = 0;
        }
        data->unk34 = data->unk38;
        data->unk38 = lbl_1_bss_14[data->unk0]->actor->velY;
        if (data->unk34 == 0.0f && 0.0f < data->unk38 && (data->unk2C & 2) != 0) {
            fn_1_4BEC(data->unkC, -10.0f, data->unk14);
            wavePos.x = data->unkC;
            wavePos.y = data->unk10 + 5.0f;
            wavePos.z = data->unk14;
            Hu3DWaterWaveCreate(&wavePos, 100.0f, 300.0f);
        }
        if (lbl_1_bss_14[data->unk0]->actor->velY == 0.0f && (data->unk2C & 2) != 0 && data->unk28 == 0) {
            fn_1_4BEC(data->unkC, -10.0f, data->unk14);
            wavePos.x = data->unkC;
            wavePos.y = data->unk10 + 5.0f;
            wavePos.z = data->unk14;
            Hu3DWaterWaveCreate(&wavePos, 100.0f, 300.0f);
            data->unk28 = 1;
        }
        if (data->unk2C == 2) {
            CharModelFxFlagSet(data->unk4, 0);
            fn_1_6038(lbl_1_bss_124[data->unk0], 3);
            if (isWinner != 0) {
                data->unk3C++;
                if (data->unk3C > 0x1E) {
                    wavePos.x = data->unkC;
                    wavePos.y = data->unk10 + 5.0f;
                    wavePos.z = data->unk14;
                    Hu3DWaterWaveCreate(&wavePos, 100.0f, 300.0f);
                    data->unk3C = 0;
                }
            } else {
                data->unk3C = 0;
            }
        } else {
            data->unk3C = 0;
            CharModelFxFlagSet(data->unk4, 1);
            fn_1_6038(lbl_1_bss_124[data->unk0], 0);
        }
    }
    mode = MgSeqModeGet();
    if (mode == 5 || mode == 6 || mode == 7 || mode == 8) {
        MgScoreWinDispSet(data->unk5C, TRUE);
        espDispOn(data->unk64);
        if (data->unk8 <= 0x63) {
            MgScoreDispOn(data->unk60);
            MgScoreValueSet(data->unk60, data->unk8);
        }
    }
    data->unkC = lbl_1_bss_14[data->unk0]->actor->pos.x;
    data->unk10 = lbl_1_bss_14[data->unk0]->actor->pos.y;
    data->unk14 = lbl_1_bss_14[data->unk0]->actor->pos.z;
    data->unk18 = 0.0f;
    data->unk1C = lbl_1_bss_14[data->unk0]->actor->rotY;
    data->unk20 = 0.0f;
    if (MgSeqModeGet() == 5) {
        Hu3D3Dto2D((HuVecF *)&data->unkC, 1, (HuVecF *)&wavePos);
        CharModelVoicePanSet(data->unk4, 48.0f + ((wavePos.x / 576.0f) * 32.0f));
        Hu3D3Dto2D((HuVecF *)&data->unkC, 1, (HuVecF *)&wavePos);
        CharModelVoiceVolSet(data->unk4, 96.0f + (wavePos.y / 480.0f) * 31.0f);
    }
}

s32 fn_1_2998(OMOBJ *obj)
{
    f32 lim = 1600.001f;
    M563PlayerData *data = obj->data;
    Vec diff;
    Vec v;

    if (lbl_1_bss_134[data->unk80] == NULL) {
        return 1;
    }
    PSVECSubtract(&data->unk70, (Vec *)&data->unkC, &diff);
    diff.y = 0.0f;
    if (PSVECSquareMag(&diff) < lim) {
        return 1;
    }
    PSVECNormalize(&diff, &diff);
    v.x = diff.x;
    v.y = diff.z;
    data->unk98 = (s8)(56.0f * v.x);
    data->unk9C = (s8)(56.0f * -v.y);
    return 0;
}

void fn_1_2AC0(OMOBJ *obj)
{
    M563PlayerData *data = obj->data;
    Vec result;

    fn_1_7818(&result.x, &result.y, &result.z, fn_1_2B28((Vec *)&data->unkC));
    data->unk70.x = result.x;
    data->unk70.y = result.y;
    data->unk70.z = result.z;
}

s32 fn_1_2B28(Vec *arg0)
{
    if (arg0->x < 0.0f) {
        if (arg0->z < 0.0f) {
            return 0;
        }
        return 2;
    }
    if (arg0->z < 0.0f) {
        return 1;
    }
    return 3;
}

void fn_1_2B90(s16 mode, s16 frameNo)
{
    s32 i;
    s32 j;
    M563PlayerData *data;

    for (i = 0; i < 4; i++) {
        data = lbl_1_bss_124[i]->data;
        for (j = 0; j < 5; j++) {
            data->unk44[j] = CharMotionCreate(lbl_1_bss_14[data->unk0]->charNo, lbl_1_data_34[j]);
        }
    }
    if (WipeCheckEnd() == 0) {
        MgActorExec();
        MgSeqModeNext();
    }
}

void fn_1_2C5C(s16 mode, s16 frameNo)
{
    M563ObjData *data = (*lbl_1_bss_11C)->data;
    s32 i;

    HuAudFXPlay(0x8D3);
    for (i = 0; i < 0x1E; i++) {
        HuPrcVSleep();
    }
    Hu3DMotionSpeedSet(lbl_1_bss_F8.unk8, 1.0f);
    Hu3DMotionClusterSpeedSet(lbl_1_bss_F8.unk8, 0, 1.0f);
    HuAudFXPlay(0x8CB);
    for (i = 0; i < 0x78; i++) {
        HuPrcVSleep();
    }
    Hu3DMotionSpeedSet(lbl_1_bss_F8.unk8, 0.0f);
    Hu3DMotionClusterSpeedSet(lbl_1_bss_F8.unk8, 0, 0.0f);
    data->unk40 = 0;
    HuAudFXPlay(0x8CC);
    for (i = 0; i < 0x96; i++) {
        HuPrcVSleep();
    }
    data->unk40 = 3;
    for (i = 0; i < 0x3C; i++) {
        HuPrcVSleep();
    }
    data->unk40 = 2;
    HuPrcVSleep();
    Hu3DModelHookObjReset(data->unk34[1], lbl_1_data_2E8);
    Hu3DModelAttrSet(data->unk30, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(data->unk32, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(data->unk32, 0x40000002);
    Hu3DMotionTimeSet(data->unk32, 0.0f);
    Hu3DMotionSpeedSet(data->unk32, 1.0f);
    for (i = 0; i < 0x82; i++) {
        if (i == 0x2D) {
            HuAudFXPlay(0x8CD);
        }
        HuPrcVSleep();
    }
    Hu3DModelAttrSet(data->unk32, HU3D_ATTR_DISPOFF);
    data->unk40 = 4;
    omCameraViewMoveSimple(&lbl_1_data_80, 0x5A);
    for (i = 0; i < 0x5A; i++) {
        HuPrcVSleep();
    }
    Hu3DCameraPosGet(1, &lbl_1_bss_B4, &lbl_1_bss_A8, &lbl_1_bss_9C);
    fn_1_6280();
    HuAudFXPlay(0x8CF);
    lbl_1_bss_2B4 = HuAudSStreamPlay(0x35);
    MgSeqModeNext();
}

void fn_1_2EB0(s16 mode, s16 frameNo)
{
    M563ObjData *data;
    u32 timer;

    fn_1_3A74();
    fn_1_3A78();
    if (MgSeqFrameNoGet() == 0) {
        data = ((OMOBJ *)*lbl_1_bss_11C)->data;
        data->unk40 = 5;
    }
    timer = MgSeqTimerValueGet();
    if (timer == 0) {
        fn_1_5B14(0);
    } else {
        fn_1_5B14(1);
    }
    MgActorExec();
}

void fn_1_2F34(s16 mode, s16 frameNo)
{
    s32 i;
    M563ObjData *mdl;
    s32 timer = 0;
    s32 prev;

    fn_1_3A78();
    if (MgSeqFrameNoGet() == 0) {
        HuAudSStreamFadeOut(lbl_1_bss_2B4, 0x64);
        fn_1_5B14(0);
        mdl = (M563ObjData *)(*lbl_1_bss_11C)->data;
        mdl->unk40 = 4;
        lbl_1_bss_3C = fn_1_5D40();
        lbl_1_bss_110 = omAddObjEx(lbl_1_bss_3B0, 0x64, 0, 0, -1, fn_1_60B4);
    }
    if (MgSeqFrameNoGet() > 0x12C) {
        for (i = 0; i < 4; i++) {
            prev = (s32)lbl_1_bss_124[i]->data;
            Hu3DModelAttrReset(lbl_1_bss_14[i]->actor->mdlId, HU3D_ATTR_DISPOFF);
        }
        MgSeqModeNext();
    }
}

void fn_1_305C(s16 mode, s16 frameNo)
{
    M563PlayerData *data;
    s32 i;
    s32 j;

    if (_CheckFlag(0x30002) != 0) {
        if (MgSeqFrameNoGet() == 0x1E) {
            for (i = 0; i < 4; i++) {
                data = lbl_1_bss_124[i]->data;
                GwPlayer[data->unk0].mgScore = data->unk8;
                OSReport("%d POINT:%d\n", data->unk0, data->unk8);
            }
            MgSeqModeSet(9);
        }
    } else {
        fn_1_3A78();
        if (MgSeqFrameNoGet() == 0x1E) {
            WipeCreate(2, 6, 0x1E);
            WipeColorSet(0xFF, 0xFF, 0xFF);
            do {
                HuPrcVSleep();
            } while (WipeCheckEnd() != 0);
            omDelObjEx(lbl_1_bss_3B0, lbl_1_bss_110);
            omCameraViewSet(&lbl_1_data_9C);
            if (lbl_1_bss_3C != 0) {
                Hu3DModelAttrSet(lbl_1_bss_F8.unk2, HU3D_ATTR_DISPOFF);
                Hu3DModelAttrReset(lbl_1_bss_F8.unk4, HU3D_ATTR_DISPOFF);
            } else {
                Hu3DModelAttrReset(lbl_1_bss_F8.unk2, HU3D_ATTR_DISPOFF);
                Hu3DModelAttrSet(lbl_1_bss_F8.unk4, HU3D_ATTR_DISPOFF);
            }
            for (i = 0; i < 4; i++) {
                data = lbl_1_bss_124[i]->data;
                data->unkC = lbl_1_data_E8[i].x;
                data->unk10 = lbl_1_data_E8[i].y;
                data->unk14 = lbl_1_data_E8[i].z;
                Hu3DModelPosSet(lbl_1_bss_14[i]->actor->mdlId, lbl_1_data_E8[i].x, lbl_1_data_E8[i].y - 75.0f, lbl_1_data_E8[i].z);
                Hu3DModelRotSet(lbl_1_bss_14[i]->actor->mdlId, 0.0f, 0.0f, 0.0f);
                Hu3DModelScaleSet(lbl_1_bss_14[i]->actor->mdlId, 1.0f, 1.0f, 1.0f);
                Hu3DModelAttrReset(lbl_1_bss_14[i]->actor->mdlId, HU3D_ATTR_DISPOFF);
                fn_1_5F14(lbl_1_bss_124[data->unk0], 0, 0.0f, 0x40000001);
                lbl_1_bss_C0[i].x = lbl_1_data_E8[i].x;
                lbl_1_bss_C0[i].y = 135.0f + (800.0f + lbl_1_data_E8[i].y);
                lbl_1_bss_C0[i].z = lbl_1_data_E8[i].z;
                if (lbl_1_bss_50[data->unk0] != -1) {
                    Hu3DModelAttrReset(lbl_1_bss_F0[data->unk0], HU3D_ATTR_DISPOFF);
                }
            }
            WipeCreate(1, 5, 0x1E);
            do {
                HuPrcVSleep();
            } while (WipeCheckEnd() != 0);
            if (lbl_1_bss_3C == 0) {
                lbl_1_bss_4 = 1;
                for (i = 0; i < 0x1E; i++) {
                    for (j = 0; j < 4; j++) {
                        lbl_1_bss_C0[j].y -= 17.64151f;
                        Hu3DModelPosSet(lbl_1_bss_F0[j], lbl_1_bss_C0[j].x, lbl_1_bss_C0[j].y, lbl_1_bss_C0[j].z);
                    }
                    HuPrcVSleep();
                }
            }
            MgSeqModeNext();
        }
    }
}

void fn_1_359C(s16 mode, s16 frameNo)
{
    M563ObjData *objData = (*lbl_1_bss_11C)->data;
    M563PlayerData *data;
    s32 i;
    char *hook;

    fn_1_3A78();
    if (lbl_1_bss_3C == 0) {
        if (MgSeqFrameNoGet() == 0) {
            for (i = 0; i < 4; i++) {
                data = lbl_1_bss_124[i]->data;
                if (lbl_1_bss_50[data->unk0] == -1) {
                    fn_1_5F14(lbl_1_bss_124[data->unk0], 2, 8.0f, 0);
                } else {
                    fn_1_5F14(lbl_1_bss_124[data->unk0], 1, 8.0f, 0);
                    Hu3D3Dto2D((HuVecF *)&data->unkC, 1, (HuVecF *)&lbl_1_bss_9C);
                    CharFXPlayPan(data->unk4, 0x1FA, 48.0f + ((lbl_1_bss_9C.x / 576.0f) * 32.0f));
                    if (_CheckFlag(0x1000F) == 0) {
                        GwPlayer[data->unk0].mgCoinBonus += 0xA;
                    }
                    objData->unk40 = 9;
                }
            }
        }
        if (MgSeqFrameNoGet() == 0x17) {
            for (i = 0; i < 4; i++) {
                data = lbl_1_bss_124[i]->data;
                if (lbl_1_bss_50[data->unk0] != -1) {
                    CharModelItemHookGet(lbl_1_bss_14[data->unk0]->charNo, 2, 0);
                    OSReport("FLO:%d\n", data->unk0, hook);
                    Hu3DModelPosSet(lbl_1_bss_F0[i], 0.0f, 0.0f, 0.0f);
                    Hu3DModelHookSet(lbl_1_bss_14[data->unk0]->actor->mdlId, hook, lbl_1_bss_F0[data->unk0]);
                }
            }
        }
        if (MgSeqFrameNoGet() < 0x17) {
            for (i = 0; i < 4; i++) {
                lbl_1_bss_C0[i].y -= 15.094339f;
                Hu3DModelPosSet(lbl_1_bss_F0[i], lbl_1_bss_C0[i].x, lbl_1_bss_C0[i].y, lbl_1_bss_C0[i].z);
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            data = lbl_1_bss_124[i]->data;
            fn_1_5F14(lbl_1_bss_124[data->unk0], 2, 8.0f, 0);
            CharFXPlay(data->unk4, 0x200);
        }
        objData->unk40 = 10;
        for (i = 0; i < 0x1E; i++) {
            HuPrcVSleep();
        }
        objData->unk40 = 11;
        for (i = 0; i < 0x1E; i++) {
            HuPrcVSleep();
        }
        Hu3DModelAttrSet(lbl_1_bss_F8.unk8, 0x40000004);
        Hu3DModelClusterAttrSet(lbl_1_bss_F8.unk8, 0, -0x3FFFFFFC);
        Hu3DMotionSpeedSet(lbl_1_bss_F8.unk8, 1.2f);
        Hu3DMotionClusterSpeedSet(lbl_1_bss_F8.unk8, 0, 1.2f);
        for (i = 0; i < 0xA0; i++) {
            if (i == 0x50) {
                HuAudFXPlay(0x8D2);
            }
            HuPrcVSleep();
        }
        MgSeqModeNext();
    }
}

void fn_1_3A74(void)
{
}

void fn_1_3A78(void)
{
    s32 i;
    OMOBJ *values[4];

    for (i = 0; i < 4; i++) {
        values[i] = lbl_1_bss_124[i]->data;
    }
}

void fn_1_3AC4(s32 arg0)
{
    M563CoinData *coin;
    OMOBJ *obj;
    s32 rate;
    Vec spawn;
    Vec target;
    u32 entry;
    f32 frameRate;

    if (lbl_1_bss_98 <= 0x1D) {
        do {
            do {
                rate = fn_1_767C(&spawn.x, &spawn.y, &spawn.z, lbl_1_data_274[lbl_1_bss_70][lbl_1_bss_68], &entry);
            } while (spawn.x == 0.0f && spawn.z == 0.0f);
        } while (fn_1_79A8(lbl_1_data_274[lbl_1_bss_70][lbl_1_bss_68], entry) != 0);
        fn_1_79C8(lbl_1_data_274[lbl_1_bss_70][lbl_1_bss_68], entry, 1);
        target.x = lbl_1_data_244[lbl_1_data_274[lbl_1_bss_70][lbl_1_bss_68]].x;
        target.y = lbl_1_data_244[lbl_1_data_274[lbl_1_bss_70][lbl_1_bss_68]].y;
        target.z = lbl_1_data_244[lbl_1_data_274[lbl_1_bss_70][lbl_1_bss_68]].z;
        if (spawn.x != 0.0f || spawn.z != 0.0f) {
            lbl_1_bss_98++;
            obj = omAddObjEx(lbl_1_bss_3B0, 0x65, 1, 0, 1, fn_1_41B0);
            obj->data = HuMemDirectMallocNum(HEAP_HEAP, 0x68, HU_MEMNUM_OVL);
            obj->work[0] = fn_1_3EF0();
            coin = obj->data;
            coin->unk5C = spawn.x;
            coin->unk60 = spawn.y;
            coin->unk64 = spawn.z;
            coin->unk50 = lbl_1_bss_70;
            coin->unk54 = lbl_1_data_274[lbl_1_bss_70][lbl_1_bss_68];
            coin->unk58 = entry;
            coin->unk48 = 0.0f;
            coin->unk8 = 1;
            coin->unk40 = 0;
            coin->unk24 = coin->unk28 = coin->unk2C = 0.0f;


            coin->unkC = target.x;
            coin->unk10 = target.y;
            coin->unk14 = target.z;
            coin->unk18 = spawn.x;
            coin->unk1C = spawn.y;
            coin->unk20 = spawn.z;
            coin->unk44 = rate;
            frameRate = 90.0f + (0.23529413f * (frand() & 0xFF));
            coin->unk30 = (coin->unk18 - coin->unkC) / frameRate;
            coin->unk34 = (coin->unk1C - coin->unk10) / frameRate;
            coin->unk38 = (coin->unk20 - coin->unk14) / frameRate;
        }
    }
}

s32 fn_1_3EF0(void)
{
    u32 remain;
    s32 result;

    result = 0;
    remain = MgSeqTimerValueGet();
remain /= 60;
remain = 0x1E - remain;
    if (remain != 0 && remain < 0xB) {
        if (remain == lbl_1_bss_40[0] && lbl_1_bss_4C == 0) {
            result = 1;
            lbl_1_bss_4C++;
        } else {
            result = 0;
        }
    } else if (0xA < remain && remain < 0x15) {
        if (remain == lbl_1_bss_40[1] && lbl_1_bss_4C == 1) {
            result = 1;
            lbl_1_bss_4C++;
        } else {
            result = 0;
        }
    } else if (0x14 < remain && remain < 0x1C) {
        if (remain == lbl_1_bss_40[2] && lbl_1_bss_4C == 2) {
            result = 1;
            lbl_1_bss_4C++;
        } else {
            result = 0;
        }
    }
    return result;
}

void fn_1_4068(OMOBJ *obj)
{
    M563CoinData *coin = obj->data;
    s32 i;

    Hu3DModelAttrSet(*obj->mdlId, HU3D_ATTR_DISPOFF);
    Hu3DModelKill(*obj->mdlId);
    lbl_1_bss_98--;
    for (i = 0; i < 0x1E; i++) {
if (obj == lbl_1_bss_134[i]) {
            lbl_1_bss_134[i] = NULL;
            break;
        }
    }
    fn_1_79C8(coin->unk54, coin->unk58, 0);
    omDelObjEx(lbl_1_bss_3B0, obj);
}

void fn_1_41B0(OMOBJ *obj)
{
    M563CoinData *coin = obj->data;
    s32 i;

    if (obj->work[0] == 0) {
        *obj->mdlId = Hu3DModelLink(lbl_1_bss_90);
        coin->unk3C = 1;
    } else {
        *obj->mdlId = Hu3DModelLink(lbl_1_bss_84);
        coin->unk3C = 3;
    }
    Hu3DModelShadowSet(*obj->mdlId);
    Hu3DModelLayerSet(*obj->mdlId, 2);
    omSetTra(obj, coin->unkC, coin->unk10, coin->unk14);
    omSetRot(obj, coin->unk24, coin->unk28, coin->unk2C);
    omSetSca(obj, 1.0f, 1.0f, 1.0f);
    for (i = 0; i < 0x1E; i++) {
if ((OMOBJ *) 0 == lbl_1_bss_134[i]) {
            lbl_1_bss_134[i] = obj;
            coin->unk0 = i;
            break;
        }
    }
    Hu3DModelAttrReset(*obj->mdlId, 0x40000002);
    Hu3DModelAttrReset(*obj->mdlId, HU3D_ATTR_DISPOFF);
    obj->objFunc = fn_1_4334;
}

void fn_1_414C(f32 *arg0, f32 *arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5)
{
    *arg0 = fn_1_473C(arg2, arg4);
    *arg1 = fn_1_473C(arg3, arg5);
}

void fn_1_4334(OMOBJ *obj)
{
    M563CoinData *coin = obj->data;
    M563PlayerData *data;
    Vec wave;
    s32 i;
    s32 j;
    M563CoinData *cdata;

    if (6U <= MgSeqModeGet() && MgSeqFrameNoGet() == 0) {
        coin->unk4 = 0x3C;
        coin->unk8 = 2;
    }
    switch (coin->unk8) {
    case 0:
        coin->unk4--;
        if (coin->unk4 < 0) {
            coin->unk4 = 0x78;
            coin->unk8 = 2;
        }
        break;
    case 1:
        coin->unkC += coin->unk30;
        coin->unk10 += coin->unk34;
        coin->unk14 += coin->unk38;
        coin->unk28 += 10.0f;
        if (coin->unk10 < coin->unk1C) {
            coin->unk10 = coin->unk1C;
            coin->unk8 = 0;
            coin->unk4 = 0x78;
            if (coin->unk44 != 0) {
                wave.x = coin->unkC;
                wave.y = coin->unk10 + 5.0f;
                wave.z = coin->unk14;
                Hu3DWaterWaveCreate(&wave, 100.0f, 300.0f);
            }
        }
        break;
    case 2:
        coin->unk4--;
        if (coin->unk4 < 0) {
            coin->unk4 = 0;
            coin->unk8 = 3;
            return;
        }
        if ((MgSeqFrameNoGet() & 1) != 0) {
            Hu3DModelAttrSet(*obj->mdlId, HU3D_ATTR_DISPOFF);
        } else {
            Hu3DModelAttrReset(*obj->mdlId, HU3D_ATTR_DISPOFF);
        }
        break;
    case 3:
        Hu3DModelAttrSet(*obj->mdlId, HU3D_ATTR_DISPOFF);
        Hu3DModelKill(*obj->mdlId);
        lbl_1_bss_98--;
        cdata = obj->data;
for (i = 0; i < 0x1E; i++) {
            if (obj == lbl_1_bss_134[i]) {
                lbl_1_bss_134[i] = NULL;
                break;
            }
        }
        fn_1_79C8(cdata->unk54, cdata->unk58, 0);
        omDelObjEx(lbl_1_bss_3B0, obj);
        return;
    }
    if (coin->unk8 == 0 || coin->unk8 == 2) {
        coin->unk40 = 1;
    }
    if (MgSeqModeGet() == 5) {
        for (j = 0; j < 4; j++) {
            data = lbl_1_bss_124[j]->data;
            if (fn_1_49F4((Vec *)&coin->unkC, (Vec *)&data->unkC, 40.0f, 50.0f)) {
                data->unk8 += coin->unk3C;
                coin->unk8 = 3;
                fn_1_4F2C(coin->unk3C, coin->unkC, coin->unk10, coin->unk14);
                omVibrate(data->unk0, 0xC, 6, 6);
                return;
            }
        }
    }
    if ((coin->unk8 == 0 || coin->unk8 == 2) && coin->unk44 != 0) {
        coin->unkC += 0.3f * sin((M_PI * coin->unk48) / 180.0);
        coin->unk48 += 2.0f;
    }
    omSetTra(obj, coin->unkC, coin->unk10, coin->unk14);
    omSetRot(obj, coin->unk24, coin->unk28, coin->unk2C);
}

f32 fn_1_473C(f32 arg0, f32 arg1)
{
    f32 result;

    if (arg1 == arg0) {
        result = arg1;
    }
    if (arg0 < 0.0f) {
        arg0 = fabsf(arg0);
        result = arg0 + (arg1 - (0.003921569f * arg0 * (frand() & 0xFF)));
        result = -result;
    } else if (arg0 == 0.0f) {
        result = 0.003921569f * arg1 * (frand() & 0xFF);
    }
    return result;
}

f32 fn_1_4868(f32 arg0, f32 arg1)
{
    f32 result;
    f32 mag0;
    f32 mag1;

    if (arg0 < 0.0f && arg1 < 0.0f) {
        mag0 = fabsf(arg0);
        mag1 = fabsf(arg1);
        result = mag1 + (mag0 - (0.003921569f * mag1 * (frand() & 0xFF)));
    } else if (arg0 > 0.0f && arg1 > 0.0f) {
        result = arg0 + (arg1 - (0.003921569f * arg0 * (frand() & 0xFF)));
    } else if (arg0 == 0.0f && arg1 == 0.0f) {
        result = arg0 + (arg1 - (0.003921569f * arg0 * (frand() & 0xFF)));
    }
    return result;
}

s32 fn_1_49F4(Vec *arg0, Vec *arg1, f32 arg2, f32 arg3)
{
    f32 dx;
    f32 dy;
    f32 dz;
    f32 lim;
    f32 dist;

    dx = arg0->x - arg1->x;
    dy = arg0->y - arg1->y;
    dz = arg0->z - arg1->z;
    dist = sqrtf((dz * dz) + ((dx * dx) + (dy * dy)));
    lim = arg2 + arg3;
    if (lim > dist) {
        return 1;
    }
    return 0;
}

void fn_1_4BEC(f32 arg0, f32 arg1, f32 arg2)
{
    OMOBJ *obj;
    Vec *pos;

    if (lbl_1_bss_80 <= 9) {
        lbl_1_bss_80++;
        obj = omAddObjEx(lbl_1_bss_3B0, 0x68, 1, 0, 4, fn_1_4D2C);
        obj->work[0] = 0;
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, 0xC, HU_MEMNUM_OVL);
        pos = obj->data;
        pos->x = arg0;
        pos->y = arg1;
        pos->z = arg2;
    }
}

void fn_1_4CB8(OMOBJ *obj)
{
    Hu3DModelAttrSet(*obj->mdlId, HU3D_ATTR_DISPOFF);
    Hu3DModelKill(*obj->mdlId);
    lbl_1_bss_80--;
    omDelObjEx(lbl_1_bss_3B0, obj);
}

void fn_1_4D2C(OMOBJ *obj)
{
    Vec *pos = obj->data;

    *obj->mdlId = Hu3DModelLink(lbl_1_bss_7C);
    Hu3DModelLayerSet(*obj->mdlId, 2);
    omSetTra(obj, pos->x, pos->y, pos->z);
    omSetRot(obj, 0.0f, 0.0f, 0.0f);
    omSetSca(obj, 1.0f, 1.0f, 1.0f);
    Hu3DModelAttrReset(*obj->mdlId, HU3D_ATTR_DISPOFF);
    Hu3DMotionTimeSet(*obj->mdlId, 0.0f);
    Hu3DMotionSpeedSet(*obj->mdlId, 1.0f);
    Hu3DModelAttrSet(*obj->mdlId, HU3D_ATTR_NONE);
    obj->objFunc = fn_1_4E54;
}

void fn_1_4E54(OMOBJ *obj)
{
    Vec *pos = obj->data;

    if (Hu3DMotionEndCheck(*obj->mdlId) != 0) {
        Hu3DModelAttrSet(*obj->mdlId, HU3D_ATTR_DISPOFF);
        Hu3DModelKill(*obj->mdlId);
        lbl_1_bss_80--;
        omDelObjEx(lbl_1_bss_3B0, obj);
        return;
    }
    omSetTra(obj, pos->x, pos->y, pos->z);
    omSetRot(obj, 0.0f, 0.0f, 0.0f);
}

void fn_1_4F2C(s32 arg0, f32 arg1, f32 arg2, f32 arg3)
{
    s32 *data;
    OMOBJ *obj;

    if (lbl_1_bss_80 <= 0x1D) {
        lbl_1_bss_94++;
        obj = omAddObjEx(lbl_1_bss_3B0, 0x65, 1, 0, 2, fn_1_5078);
        obj->work[0] = 0;
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, 0x10, HU_MEMNUM_OVL);
        data = obj->data;
        *(f32 *)&data[0] = arg1;
        *(f32 *)&data[1] = arg2;
        *(f32 *)&data[2] = arg3;
        data[3] = arg0;
    }
}

void fn_1_5004(OMOBJ *obj)
{
    Hu3DModelAttrSet(*obj->mdlId, HU3D_ATTR_DISPOFF);
    Hu3DModelKill(*obj->mdlId);
    lbl_1_bss_94--;
    omDelObjEx(lbl_1_bss_3B0, obj);
}

void fn_1_5078(OMOBJ *obj)
{
    s32 *data = obj->data;
    Vec a;
    Vec b;
    Vec c;
    Vec d;
    s32 v1;
    s32 v2;
    s32 v3;
    s32 v4;

    if (data[3] == 1) {
        *obj->mdlId = Hu3DModelLink(lbl_1_bss_8C);
        Hu3D3Dto2D((HuVecF *)data, 1, (HuVecF *)&b);
        v2 = (s32)(48.0f + ((b.x / 576.0f) * 32.0f));
        Hu3D3Dto2D((HuVecF *)data, 1, (HuVecF *)&a);
        v1 = (s32)(96.0f + ((a.y / 480.0f) * 31.0f));
        HuAudFXPlayVolPan(0x8D0, v1, v2);
    } else {
        *obj->mdlId = Hu3DModelLink(lbl_1_bss_88);
        Hu3D3Dto2D((HuVecF *)data, 1, (HuVecF *)&d);
        v3 = (s32)(48.0f + ((d.x / 576.0f) * 32.0f));
        Hu3D3Dto2D((HuVecF *)data, 1, (HuVecF *)&c);
        v4 = (s32)(96.0f + ((c.y / 480.0f) * 31.0f));
        HuAudFXPlayVolPan(0x8D1, v3, v4);
    }
    Hu3DModelLayerSet(*obj->mdlId, 2);
    omSetTra(obj, *(f32 *)&data[0], *(f32 *)&data[1], *(f32 *)&data[2]);
    omSetRot(obj, 0.0f, 0.0f, 0.0f);
    omSetSca(obj, 1.0f, 1.0f, 1.0f);
    Hu3DModelAttrReset(*obj->mdlId, HU3D_ATTR_DISPOFF);
    Hu3DMotionTimeSet(*obj->mdlId, 0.0f);
    Hu3DMotionSpeedSet(*obj->mdlId, 1.0f);
    Hu3DModelAttrSet(*obj->mdlId, HU3D_ATTR_NONE);
    obj->objFunc = fn_1_534C;
}

void fn_1_534C(OMOBJ *obj)
{
    s32 *data = obj->data;

    if (Hu3DMotionEndCheck(*obj->mdlId) != 0) {
        Hu3DModelAttrSet(*obj->mdlId, HU3D_ATTR_DISPOFF);
        Hu3DModelKill(*obj->mdlId);
        lbl_1_bss_94--;
        omDelObjEx(lbl_1_bss_3B0, obj);
        return;
    }
    omSetTra(obj, *(f32 *)&data[0], *(f32 *)&data[1], *(f32 *)&data[2]);
    omSetRot(obj, 0.0f, 0.0f, 0.0f);
}

s32 fn_1_5424(Vec *arg0, Vec *arg1, f32 arg2)
{
    f32 dist;
    f32 bestDist = 1000000.0f;
    Vec best;
    M563CoinData *coin;
    s32 i;

    for (i = 0; i < 0x1E; i++) {
        if (lbl_1_bss_134[i] != NULL) {
            coin = lbl_1_bss_134[i]->data;
            dist = fn_1_6110(arg0->x, arg0->z, coin->unkC, coin->unk14);
            if (dist < arg2 && coin->unk40 != 0 && fn_1_5A7C((Vec *)&coin->unkC) && bestDist > dist) {
                best.x = coin->unkC;
                best.y = coin->unk10;
                best.z = coin->unk14;
            }
        }
    }
    if (bestDist != 1000000.0f) {
        arg1->x = best.x;
        arg1->y = best.y;
        arg1->z = best.z;
        return 1;
    }
    return 0;
}

s32 fn_1_557C(Vec *arg0, Vec *arg1)
{
    f32 bestDist = 1000000.0f;
    f32 dist;
    Vec best;
    M563CoinData *coin;
    s32 i;

    for (i = 0; i < 0x1E; i++) {
        if (lbl_1_bss_134[i] != NULL) {
            coin = lbl_1_bss_134[i]->data;
            if (coin->unk40 != 0 && fn_1_5A7C((Vec *)&coin->unkC)) {
                dist = fn_1_6110(arg0->x, arg0->z, coin->unkC, coin->unk14);
                if (bestDist > dist) {
                    best.x = coin->unkC;
                    best.y = coin->unk10;
                    best.z = coin->unk14;
                    bestDist = dist;
                }
            }
        }
    }
    if (bestDist != 1000000.0f) {
        arg1->x = best.x;
        arg1->y = best.y;
        arg1->z = best.z;
        return 1;
    }
    return 0;
}

s32 fn_1_56C8(Vec *arg0, Vec *arg1, s32 *arg2, f32 arg3)
{
    f32 bestDist = 1000000.0f;
    f32 dist;
    Vec best;
    s32 bestIndex = -1;
    M563CoinData *coin;
    s32 i;

    for (i = 0; i < 0x1E; i++) {
        if (lbl_1_bss_134[i] != NULL) {
            coin = lbl_1_bss_134[i]->data;
            if (coin->unk40 != 0 && fn_1_5A7C((Vec *)&coin->unkC)) {
                dist = fn_1_6110(arg0->x, arg0->z, coin->unkC, coin->unk14);
                if (bestDist > dist) {
                    best.x = coin->unkC;
                    best.y = coin->unk10;
                    best.z = coin->unk14;
                    bestIndex = i;
                    bestDist = dist;
                }
            }
        }
    }
    if (bestDist != 1000000.0f) {
        arg1->x = best.x;
        arg1->y = best.y;
        arg1->z = best.z;
        *arg2 = bestIndex;
        return 1;
    }
    return 0;
}

s32 fn_1_5814(Vec *arg0, Vec *arg1, f32 arg4, s32 *arg2, s32 *arg3)
{
    f32 dist;
    f32 bestDist = 1000000.0f;
    Vec best;
    s32 bestRow;
    s32 bestSlot;
    M563CoinData *coin;
    s32 i;

    for (i = 0; i < 0x1E; i++) {
        if (lbl_1_bss_134[i] != NULL) {
            coin = lbl_1_bss_134[i]->data;
            dist = fn_1_6110(arg0->x, arg0->z, coin->unkC, coin->unk14);
            if (dist < arg4 && coin->unk40 != 0 && fn_1_5A7C((Vec *)&coin->unkC) && bestDist > dist) {
                best.x = coin->unkC;
                best.y = coin->unk10;
                best.z = coin->unk14;
                bestRow = coin->unk54;
                bestSlot = coin->unk58;
            }
        }
    }
    if (bestDist != 1000000.0f) {
        arg1->x = best.x;
        arg1->y = best.y;
        arg1->z = best.z;
        *arg2 = bestRow;
        *arg3 = bestSlot;
        return 1;
    }
    return 0;
}

s32 fn_1_5974(Vec *arg0, Vec *arg1, f32 arg2)
{
    f32 dist;
    M563CoinData *coin;
    s32 i;

for (i = 0; i < 0x1E; i++) {
        if (lbl_1_bss_134[i] != NULL) {
            coin = lbl_1_bss_134[i]->data;
            dist = fn_1_6110(arg0->x, arg0->z, coin->unkC, coin->unk14);
            if (dist < arg2 && coin->unk40 != 0 && fn_1_5A7C((Vec *)&coin->unkC)) {
                arg1->x = coin->unkC;
                arg1->y = coin->unk10;
                arg1->z = coin->unk14;
                return 1;
            }
        }
    }
    return 0;
}

const Vec lbl_1_rodata_164 = { -650.0f, 240.0f, -450.0f };
const Vec lbl_1_rodata_170 = { 650.0f, 240.0f, 700.0f };

s32 fn_1_5A7C(Vec *arg0)
{
    Vec lo = lbl_1_rodata_164;
    Vec hi = lbl_1_rodata_170;

    if (arg0->x > lo.x && arg0->z > lo.z && arg0->x < hi.x && arg0->z < hi.z) {
        return 1;
    }
    return 0;
}

void fn_1_5B14(s32 arg0)
{
    s32 i;
    M563PlayerData *data;

    for (i = 0; i < 4; i++) {
        data = lbl_1_bss_124[i]->data;
        if (arg0 != 0) {
            MgPlayerAttrReset(lbl_1_bss_14[data->unk0], MGPLAYER_ATTR_COMSTK);
        } else {
            MgPlayerAttrSet(lbl_1_bss_14[data->unk0], MGPLAYER_ATTR_COMSTK);
        }
    }
}

void fn_1_5BC0(void)
{
    M563PlayerData *data;
    f32 angle;
    f32 current;
    f32 next;
    f32 tmp;
    s32 i;

    for (i = 0; i < 4; i++) {
        data = lbl_1_bss_124[i]->data;
        current = data->unk1C;
        next = fmod(0.0f - current, 360.0);
        if (0.0f > next) {
            next += 360.0f;
        }
        if (180.0f < next) {
            next -= 360.0f;
        }
        angle = fmod(current + (0.5f * next), 360.0);
        if (0.0f > angle) {
            angle += 360.0f;
        }
        tmp = angle;
        data->unk1C = tmp;
        lbl_1_bss_14[data->unk0]->actor->rotY = data->unk1C;
    }
}

s32 fn_1_5D40(OMOBJ *obj)
{
    s32 scores[4];
    s32 max = 0;
    s32 tieCount = 0;
    s32 isTie = 0;
    s32 i;

    for (i = 0; i < 4; i++) {
        scores[i] = ((M563PlayerData *)lbl_1_bss_124[i]->data)->unk8;
        if (scores[i] > max) {
            max = scores[i];
        }
    }
    for (i = 0; i < 4; i++) {
        if (max == scores[i]) {
            lbl_1_bss_50[i] = ((M563PlayerData *)lbl_1_bss_124[i]->data)->unk4;
        } else {
            lbl_1_bss_50[i] = -1;
        }
    }
    for (i = 0; i < 4; i++) {
        if (max == scores[i]) {
            tieCount++;
        }
    }
    if (tieCount >= 2) {
        if (max < 0xA) {
            isTie = 1;
        } else {
            isTie = 0;
        }
    }
    if (isTie != 0) {
        lbl_1_bss_50[3] = -1;
        lbl_1_bss_50[2] = -1;
        lbl_1_bss_50[1] = -1;
        lbl_1_bss_50[0] = -1;
    }
    MgSeqWinnerSet(lbl_1_bss_50[0], lbl_1_bss_50[1], lbl_1_bss_50[2], lbl_1_bss_50[3]);
    return isTie;
}

void fn_1_5F14(OMOBJ *obj, s32 arg1, f32 arg3, u32 arg2)
{
    M563PlayerData *data = obj->data;

    if (data->unk4E != arg1) {
        data->unk4E = arg1;
        if (arg3 > 0.0f) {
            CharMotionShiftSet(lbl_1_bss_14[data->unk0]->charNo, data->unk44[arg1], 0.0f, arg3, arg2);
            return;
        }
        CharMotionSet(lbl_1_bss_14[data->unk0]->charNo, data->unk44[arg1]);
        Hu3DModelAttrSet(lbl_1_bss_14[data->unk0]->actor->mdlId, arg2);
    }
}

void fn_1_6038(OMOBJ *obj, s16 arg1)
{
    M563PlayerData *data = obj->data;

    if (data->unk50 != arg1) {
        data->unk50 = arg1;
        CharModelStepFxSet(lbl_1_bss_14[data->unk0]->charNo, arg1);
    }
}

void fn_1_60B4(OMOBJ *obj)
{
    MgActorExec();
}

s32 fn_1_60D4(s32 arg0, s32 arg1)
{
    return arg0 + frandmod(arg1 - arg0);
}

f32 fn_1_6110(f32 arg0, f32 arg1, f32 arg2, f32 arg3)
{
    f32 dx;
    f32 dz;
    f32 dist;

    dx = arg0 - arg2;
    dz = arg1 - arg3;
    dist = sqrtf((dx * dx) + (dz * dz));
    return dist;
}

void fn_1_6280(void)
{
    s32 i;
    s32 j;
    s32 pattern;
    s32 half;
    s32 hasWave;
    f32 dirX;
    Vec spawn;
    Vec target;
    OMOBJ *obj;
    M563CoinData *coin;
    f32 frameRate;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            pattern = lbl_1_data_1D4[i * 3 + j];
            switch (i) {
            case 0:
                half = 0;
                dirX = 1.0f;
                break;
            case 1:
                half = 1;
                dirX = 1.0f;
                break;
            case 2:
                half = 0;
                dirX = -1.0f;
                break;
            case 3:
                half = 1;
                dirX = -1.0f;
                break;
            }
            hasWave = (lbl_1_data_358[half][pattern].y == 75.0f);
            spawn.x = dirX * lbl_1_data_358[half][pattern].x;
            spawn.y = lbl_1_data_358[half][pattern].y - 75.0f;
            spawn.z = lbl_1_data_358[half][pattern].z;
            fn_1_79C8(i, pattern, 1);
            target.x = lbl_1_data_244[i].x;
            target.y = lbl_1_data_244[i].y;
            target.z = lbl_1_data_244[i].z;
            lbl_1_bss_98++;
            obj = omAddObjEx(lbl_1_bss_3B0, 0x65, 1, 0, 1, fn_1_41B0);
            obj->data = HuMemDirectMallocNum(HEAP_HEAP, 0x68, HU_MEMNUM_OVL);
            obj->work[0] = 0;
            coin = obj->data;
            coin->unk5C = spawn.x;
            coin->unk60 = spawn.y;
            coin->unk64 = spawn.z;
            coin->unk50 = 0;
            coin->unk54 = i;
            coin->unk58 = pattern;
            coin->unk48 = 0.0f;
            coin->unk8 = 1;
            coin->unk40 = 0;
            coin->unk2C = 0.0f;
            coin->unk28 = 0.0f;
            coin->unk24 = 0.0f;
            coin->unkC = target.x;
            coin->unk10 = target.y;
            coin->unk14 = target.z;
            coin->unk18 = spawn.x;
            coin->unk1C = spawn.y;
            coin->unk20 = spawn.z;
            coin->unk44 = hasWave;
            frameRate = 90.0f + (0.23529413f * frand());
            coin->unk30 = (coin->unk18 - coin->unkC) / frameRate;
            coin->unk34 = (coin->unk1C - coin->unk10) / frameRate;
            coin->unk38 = (coin->unk20 - coin->unk14) / frameRate;
        }
    }
}

void fn_1_660C(OMOBJ *obj)
{
    if (MgSeqModeGet() <= 8U && lbl_1_bss_4 != 0) {
        if (lbl_1_bss_8 != 0) {
            if (lbl_1_bss_8 == 5) {
                HuAudFXPlay(0x8CE);
            } else if ((lbl_1_bss_8 % 65) == 0) {
                HuAudFXPlay(0x8CE);
            }
        }
        lbl_1_bss_8++;
    }
}

void fn_1_66D4(void)
{
    lbl_1_bss_3B4[1] = 0;
    lbl_1_bss_3B4[2] = 0;
}

typedef struct M563ListNode2_s {
    u16 unk0;
    s16 unk2;
    s32 unk4;
    struct M563ListNode2_s *next;
} M563ListNode2;

void fn_1_66F8(u16 arg0, s16 arg1)
{
    M563ListNode2 *node;
    M563ListNode2 *prev;
    s32 *list;

    node = HuMemDirectMallocNum(HEAP_HEAP, 0xC, HU_MEMNUM_OVL);
    node->unk0 = arg0;
    node->unk2 = arg1;
    list = &lbl_1_bss_3B4[0];
    prev = (M563ListNode2 *)list[2];
    list[2] = (s32)node;
    if ((M563ListNode2 *)0 != prev) {
        prev->unk4 = (s32)node;
    }
    node->unk4 = (s32)list;
    node->next = prev;
}

s16 fn_1_6780(s16 arg0)
{
    M563ListNode2 *node;
    M563ListNode2 *prev;
    M563ListNode2 *next;
    s16 value;

    node = (M563ListNode2 *)lbl_1_bss_3B4[2];
    while ((M563ListNode2 *)0 != node) {
        if (arg0 == node->unk0) {
            value = node->unk2;
            prev = (M563ListNode2 *)node->unk4;
            next = node->next;
            prev->next = next;
            if ((M563ListNode2 *)0 != next) {
                next->unk4 = (s32)prev;
            }
            HuMemDirectFree(node);
            return value;
        }
        node = node->next;
    }
    return -1;
}

void fn_1_6824(void)
{
    s32 i;

    for (i = 0; i < 0x20; i++) {
        lbl_1_bss_1AC[i].unk0 = 0;
        lbl_1_bss_1AC[i].unk4 = -1;
    }
}

s16 fn_1_687C(u32 arg0)
{
    s32 i;

    for (i = 0; i < 0x20; i++) {
        if (arg0 == lbl_1_bss_1AC[i].unk0) {
            return Hu3DModelLink(lbl_1_bss_1AC[i].unk4);
        }
    }
    for (i = 0; i < 0x20; i++) {
        if (lbl_1_bss_1AC[i].unk0 == 0) {
            lbl_1_bss_1AC[i].unk0 = arg0;
            lbl_1_bss_1AC[i].unk4 = Hu3DModelCreate(HuDataSelHeapReadNum(arg0, HU_MEMNUM_OVL, HEAP_MODEL));
            return lbl_1_bss_1AC[i].unk4;
        }
    }
    return Hu3DModelCreate(HuDataSelHeapReadNum(arg0, HU_MEMNUM_OVL, HEAP_MODEL));
}

void fn_1_6990(f32 arg3, Vec *arg0, Vec *arg1, Vec *arg2)
{
    f32 t;
    f64 dbl;

    if (1.0f < arg3) {
        arg3 = 1.0f;
    }
    dbl = sin((M_PI * (90.0f * arg3)) / 180.0);
    t = sin((M_PI * (90.0f * arg3)) / 180.0) * dbl;
    arg2->x = arg0->x + (t * (arg1->x - arg0->x));
    arg2->y = arg0->y + (t * (arg1->y - arg0->y));
    arg2->z = arg0->z + (t * (arg1->z - arg0->z));
}

void fn_1_6AEC(Vec *arg0, Vec *arg1, Vec *arg2)
{
    Hu3DCameraPosSetV(1, arg0, arg1, arg2);
    lbl_1_bss_3A4 = *arg0;
    lbl_1_bss_38C = *arg2;
}

s32 fn_1_6B78(Vec *arg0, f32 arg1)
{
Vec from;
    Vec to;
    f32 t;
    f32 t2a;
    f32 t2b;
    f64 dbl;
    f64 dbl2;

    t = arg1;
    if (1.0f > t) {
        t = 1.0f;
    }
    dbl = sin((M_PI * (90.0f * t)) / 180.0);
    t2a = sin((M_PI * (90.0f * t)) / 180.0) * dbl;
    from.x = lbl_1_bss_3A4.x + (t2a * (lbl_1_bss_398.x - lbl_1_bss_3A4.x));
    from.y = lbl_1_bss_3A4.y + (t2a * (lbl_1_bss_398.y - lbl_1_bss_3A4.y));
    from.z = lbl_1_bss_3A4.z + (t2a * (lbl_1_bss_398.z - lbl_1_bss_3A4.z));
    t = arg1;
    if (1.0f > t) {
        t = 1.0f;
    }
    dbl2 = sin((M_PI * (90.0f * t)) / 180.0);
    t2b = sin((M_PI * (90.0f * t)) / 180.0) * dbl2;
    to.x = lbl_1_bss_38C.x + (t2b * (lbl_1_bss_380.x - lbl_1_bss_38C.x));
    to.y = lbl_1_bss_38C.y + (t2b * (lbl_1_bss_380.y - lbl_1_bss_38C.y));
    to.z = lbl_1_bss_38C.z + (t2b * (lbl_1_bss_380.z - lbl_1_bss_38C.z));
    Hu3DCameraPosSetV(1, &from, arg0, &to);
    if (1.0f <= arg1) {
        return 1;
    }
    lbl_1_bss_2B0 = 1;
    return 0;
}

s32 fn_1_6EAC(Vec *arg0, f32 arg1)
{
Vec from;
    Vec to;
    f32 t;
    f64 dbl;
    f32 t2a;
    f64 dbl2;
    f32 t2b;
    s32 i;

    for (i = 0; i < 4; i++) {
        t = arg1;
        if (1.0f > t) {
            t = 1.0f;
        }
        dbl = sin((M_PI * (90.0f * t)) / 180.0);
        t2a = sin((M_PI * (90.0f * t)) / 180.0) * dbl;
        from.x = lbl_1_bss_350[i].x + (t2a * (lbl_1_bss_320[i].x - lbl_1_bss_350[i].x));
        from.y = lbl_1_bss_350[i].y + (t2a * (lbl_1_bss_320[i].y - lbl_1_bss_350[i].y));
        from.z = lbl_1_bss_350[i].z + (t2a * (lbl_1_bss_320[i].z - lbl_1_bss_350[i].z));
        t = arg1;
        if (1.0f > t) {
            t = 1.0f;
        }
        dbl2 = sin((M_PI * (90.0f * t)) / 180.0);
        t2b = sin((M_PI * (90.0f * t)) / 180.0) * dbl2;
        to.x = lbl_1_bss_2F0[i].x + (t2b * (lbl_1_bss_2C0[i].x - lbl_1_bss_2F0[i].x));
        to.y = lbl_1_bss_2F0[i].y + (t2b * (lbl_1_bss_2C0[i].y - lbl_1_bss_2F0[i].y));
        to.z = lbl_1_bss_2F0[i].z + (t2b * (lbl_1_bss_2C0[i].z - lbl_1_bss_2F0[i].z));
        Hu3DCameraPosSetV(1 << i, &from, arg0, &to);
    }
    if (1.0f <= arg1) {
        return 1;
    }
    lbl_1_bss_2B0 = 1;
    return 0;
}

f32 fn_1_7290(f32 arg0, f32 arg1, f32 arg2)
{
    f32 diff;
    f32 result;

diff = fmod(arg1 - arg0, 360.0);
    if (0.0f > diff) {
        diff += 360.0f;
    }
    if (180.0f < diff) {
        diff -= 360.0f;
    }
    result = fmod(arg0 + (diff * arg2), 360.0);
    if (0.0f > result) {
        result += 360.0f;
    }
    return result;
}

void fn_1_7394(s32 arg0)
{
    if (lbl_1_bss_2B4 >= 0) {
        if (arg0 < 0) {
            arg0 = 0x3E8;
        }
        HuAudSeqFadeOut(lbl_1_bss_2B4, arg0);
    }
    lbl_1_bss_2B4 = -1;
}

void fn_1_7400(s16 arg0, char *arg1, Vec *arg2)
{
    Mtx mtx;

    Hu3DModelObjMtxGet(arg0, arg1, mtx);
    arg2->x = mtx[0][3];
    arg2->y = mtx[1][3];
    arg2->z = mtx[2][3];
}

s32 fn_1_7458(s32 arg0, s32 arg1)
{
    return arg0 + frandmod(arg1 - arg0);
}

s32 fn_1_7494(Vec *arg0, Vec *arg1, f32 arg2, f32 arg3)
{
    if ((((arg1->x - arg0->x) * (arg1->x - arg0->x)) + ((arg1->z - arg0->z) * (arg1->z - arg0->z))) < ((arg2 + arg3) * (arg2 + arg3))) {
        return 1;
    }
    return 0;
}

s32 fn_1_74F4(Vec *arg0, s32 arg1, s32 arg2)
{
    Vec screen;

    Hu3D3Dto2D((HuVecF *)arg0, 1, (HuVecF *)&screen);
    return arg1 + ((screen.x / 576.0f) * (arg2 - arg1));
}

s32 fn_1_75B8(Vec *arg0, s32 arg1, s32 arg2)
{
    Vec screen;

    Hu3D3Dto2D((HuVecF *)arg0, 1, (HuVecF *)&screen);
    return arg1 + ((screen.y / 480.0f) * (arg2 - arg1));
}
