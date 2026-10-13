#include "game/audio.h"
#include "game/charman.h"
#include "game/esprite.h"
#include "game/frand.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/mg/timer.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/printfunc.h"
#include "game/sprite.h"
#include "game/wipe.h"

#include "math.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern s16 omSysExitReq;

typedef struct M524Player_s {
    s16 index; //0x00
    s16 charNo; //0x02
    s16 padNo; //0x04
    s16 grp; //0x06
    s16 dif; //0x08
    s16 pad0A; //0x0A
    s32 type; //0x0C
    f32 traX; //0x10
    f32 traY; //0x14
    f32 traZ; //0x18
    f32 rotX; //0x1C
    f32 rotY; //0x20
    f32 rotZ; //0x24
    s32 state; //0x28
    f32 aimAng; //0x2C
    f32 padAng; //0x30
    s32 dir; //0x34
    f32 motTime; //0x38
    s32 landedF; //0x3C
    f32 shotX; //0x40
    f32 shotY; //0x44
    f32 shotZ; //0x48
    f32 shotPad; //0x4C
    f32 shotVelY; //0x50
    f32 shotVelZ; //0x54
    u16 timer; //0x58
    s16 effectId; //0x5A
    s32 objNum; //0x5C
    s32 doneF; //0x60
    s32 endF; //0x64
} M524Player;

typedef struct M524Fall_s {
    f32 pad0; //0x00
    f32 posX; //0x04
    f32 posY; //0x08
    f32 posZ; //0x0C
    f32 vel0; //0x10
    f32 velY; //0x14
    f32 velZ; //0x18
} M524Fall;

typedef struct M524Camera_s {
    s32 camNo; //0x00
    s32 mode; //0x04
    f32 nearZ; //0x08
    f32 fov; //0x0C
    f32 farZ; //0x10
    Vec pos; //0x14
    Vec up; //0x20
    Vec target; //0x2C
    f32 angX; //0x38
    f32 angY; //0x3C
    f32 angZ; //0x40
    f32 dist; //0x44
} M524Camera;

typedef struct M524CamMove_s {
    M524Camera *cam; //0x00
    M524Camera start; //0x04
    M524Camera end; //0x4C
    f32 totalTime; //0x94
    f32 time; //0x98
    s32 moveF; //0x9C
} M524CamMove;

typedef void (*M524FlowFunc)(OMOBJ *obj, void *flow);
typedef struct M524Flow_s {
    s32 index; //0x00
    M524FlowFunc *funcTbl; //0x04
    s32 funcNum; //0x08
    s16 gameMesId; //0x0C
    s16 pad0E; //0x0E
    MGTIMER *timer; //0x10
    s32 timerType; //0x14
    s32 maxTime; //0x18
    s32 endTime; //0x1C
    s32 recordTime; //0x20
    s32 pad24; //0x24
    s32 work[16]; //0x28
} M524Flow;

typedef struct M524Light_s {
    Point3d pos;
    Point3d target;
    GXColor color;
} M524Light;

extern void ObjectSetup(void);
s16 fn_1_A0(Vec *pos);
void fn_1_438(void);
void fn_1_478(void);
void fn_1_6D8(void);
void fn_1_848(OMOBJ *obj);
void fn_1_AD0(OMOBJ *obj);
void fn_1_C24(OMOBJ *obj);
void fn_1_D08(OMOBJ *obj);
void fn_1_E2C(OMOBJ *obj);
void fn_1_F84(OMOBJ *obj);
void fn_1_11D4(OMOBJ *obj);
f32 fn_1_1568(f32 ang0, f32 ang1);
void fn_1_161C(OMOBJ *obj);
void fn_1_1CBC(OMOBJ *obj);
void fn_1_1DF8(OMOBJ *obj);
void fn_1_1FB8(OMOBJ *obj);
void fn_1_20A0(OMOBJ *obj);
void fn_1_21BC(OMOBJ *obj);
void fn_1_23C8(OMOBJ *obj, Vec *pos, s32 num);
void fn_1_2630(OMOBJ *obj);
void fn_1_2760(OMOBJ *obj);
void fn_1_27D8(OMOBJ *obj);
void fn_1_28C8(OMOBJ *obj, Vec *pos);
void fn_1_29F4(OMOBJ *obj);
void fn_1_2D30(OMOBJ *obj);
void fn_1_2DB4(OMOBJ *obj);
void fn_1_2EEC(OMOBJ *obj);
void fn_1_2FB0(OMOBJ *obj, void *flow);
void fn_1_30BC(OMOBJ *obj, void *flow);
void fn_1_31EC(OMOBJ *obj, void *flow);
void fn_1_32F0(OMOBJ *obj, void *flow);
void fn_1_33B4(OMOBJ *obj, void *flow);
void fn_1_3554(void);
M524Camera *fn_1_35A8(M524Camera *param);
void fn_1_3694(M524Camera *cam);
HU3DLIGHTID fn_1_36E4(M524Light *light);
void fn_1_3740(OMOBJ *obj);
void fn_1_3CF4(M524Camera *cam);
void fn_1_3DA0(M524Camera *cam);
void fn_1_3DD0(M524Camera *cam);
void fn_1_4494(M524Camera *cam);
void fn_1_4898(OMOBJ *obj);
OMOBJ *MgCameraMoveEntry(M524Camera *cam, M524Camera *param, f32 time);
s32 fn_1_4BB0(OMOBJ *obj);
void fn_1_4BE4(OMOBJ *obj);
void fn_1_4C18(OMOBJ *obj);
void fn_1_4D60(M524Camera *cam, s32 time, s32 num);
f32 fn_1_4E1C(s16 padNo);
f32 fn_1_4FA0(s16 padNo);
void fn_1_50B0(OMOBJ *obj);
void fn_1_5160(OMOBJ *obj);
OMOBJ *fn_1_5198(M524FlowFunc *funcTbl);
void MgFlowSet(OMOBJ *obj, s32 index);
void MgFlowNext(OMOBJ *obj);
s32 fn_1_5338(OMOBJ *obj);
s32 fn_1_5354(OMOBJ *obj, s32 index);
void fn_1_53A0(OMOBJ *obj, s32 type, s32 maxTime, s32 endTime, s32 recordTime);
BOOL fn_1_53D4(OMOBJ *obj);
void fn_1_5410(OMOBJ *obj);
void fn_1_5450(void);
void fn_1_5454(OMOBJ *obj, void *flow);
void fn_1_54CC(OMOBJ *obj, void *flow);
void fn_1_5530(OMOBJ *obj, void *flow, s16 mesNo, s16 mesArg);
void fn_1_55EC(OMOBJ *obj, void *flow);
void fn_1_56A0(OMOBJ *obj, void *flow);
void fn_1_5754(OMOBJ *obj, void *flow);
void fn_1_57F4(M524Player *player, s16 index);
s32 fn_1_58C0(Vec *pos, Vec *target, Vec *angle, f32 dist);
f32 fn_1_59E4(f32 ang0, f32 ang1, f32 max);
s32 fn_1_5B54(Vec *pos, Vec *cur, Vec *target, f32 dist);
void fn_1_5D8C(Vec *pos, Vec *target, Vec *cur, Vec *out);
f32 fn_1_5E04(Vec *a, Vec *b, Vec *c);
s32 fn_1_5E48(Vec *dir, Vec *pos, Vec *target, Vec *out, Vec *result);

char lbl_1_data_0[] = "pos : x=%.2f y=%.2f z=%.2f";
char lbl_1_data_1B[] = "int : x=%.2f y=%.2f z=%.2f";
char lbl_1_data_36[] = "up  : x=%.2f y=%.2f z=%.2f";
char lbl_1_data_51[] = "ang : x=%f y=%f";
char lbl_1_data_61[] = "dis : %.2f";
char lbl_1_data_6C[] = "mod : pos";
char lbl_1_data_76[] = "mod : ang";
char lbl_1_data_80[] = "MgCameraMoveEntry : Camera Number Different\n";
char lbl_1_data_AD[] = "MgFlowSet : Value is Max Over\n";
char lbl_1_data_CC[] = "MgFlowNext : Value is Max Over\n";

M524Light lbl_1_data_EC = { { -800.0f, 1500.0f, 800.0f }, { 800.0f, -1500.0f, -800.0f }, { 0xFF, 0xFF, 0xFF, 0xFF } };

M524Camera lbl_1_data_108 = { 1, 1, 20.0f, 20.0f, 7000.0f, { 0.0f, 0.99f, -0.17f }, { 0.0f, 150.0f, 0.0f }, { -9.0f, 0.0f, 0.0f }, 0.0f, 0.0f, 0.0f, 5000.0f };

M524Camera lbl_1_data_150 = { 1, 1, 20.0f, 20.0f, 7000.0f, { 0.0f, 1.0f, 0.0f }, { 0.0f, 300.0f, 0.0f }, { -50.0f, 0.0f, 0.0f }, 0.0f, 0.0f, 0.0f, 5400.0f };

M524Camera lbl_1_data_198 = { 1, 0, 20.0f, 20.0f, 7000.0f, { 0.0f, 1.0f, 0.0f }, { 0.0f, 500.0f, 0.0f }, { -10.0f, 0.0f, 0.0f }, 0.0f, 0.0f, 0.0f, 5000.0f };

Point3d lbl_1_data_1E0 = { 0.0f, 0.0f, 300.0f };

M524FlowFunc lbl_1_data_1EC[11] = {
    fn_1_5454,
    fn_1_2FB0,
    fn_1_55EC,
    fn_1_5754,
    fn_1_30BC,
    fn_1_31EC,
    fn_1_32F0,
    fn_1_56A0,
    fn_1_33B4,
    fn_1_54CC,
    NULL,
};

f32 lbl_1_data_218[4] = { -300.0f, -100.0f, 100.0f, 300.0f };

s16 lbl_1_data_228[4] = { 0x28, 0x38, 0x48, 0x58 };
s16 lbl_1_data_230[4] = { 0x754, 0x755, 0x756, 0x757 };
s16 lbl_1_data_238[4] = { 0x758, 0x759, 0x75A, 0x75B };
s16 lbl_1_data_240[4] = { 0x75C, 0x75D, 0x75E, 0x75F };
s16 lbl_1_data_248[4] = { 0x760, 0x761, 0x762, 0x763 };
s16 lbl_1_data_250[4] = { 0x764, 0x765, 0x766, 0x767 };

HU3DPARMANPARAM lbl_1_data_258 = {
    0x32,
    0,
    20.0f,
    100.0f,
    0.0f,
    { 0.0f, 0.0f, 0.0f },
    1.0f,
    0.0f,
    400.0f,
    0.97f,
    1,
    { { 0xFF, 0xFF, 0xFF, 0xFF }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 0xFF, 0xFF, 0xFF, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};

unsigned int lbl_1_data_2A8[10] = { 0x008C0000, 0x008C0001, 0x008C000D, 0x008C000E, 0x008C0038, 0x00870049, 0x008C001B, 0x008C0039, 0x008C0006, 0 };

s32 lbl_1_bss_0;
s32 lbl_1_bss_4;
OMOBJ *lbl_1_bss_8;
s16 lbl_1_bss_C;
s16 lbl_1_bss_E;
s16 lbl_1_bss_10[4];
M524Camera *lbl_1_bss_18;
OMOBJ **lbl_1_bss_1C;
OMOBJ *lbl_1_bss_20;
HU3DMODELID lbl_1_bss_24[6];
s32 lbl_1_bss_30;
M524Camera lbl_1_bss_34;
s16 lbl_1_bss_7C;
OMOBJMAN *lbl_1_bss_80;

const u32 lbl_1_rodata_94[5] = { 0x00520010, 0x00520000, 0x00520001, 0x00520003, 0x00520002 };
const u32 lbl_1_rodata_A8[4] = { 0x00520006, 0x00520007, 0x00520008, 0x00520009 };
const u32 lbl_1_rodata_B8[4] = { 0x0052000A, 0x0052000B, 0x0052000C, 0x0052000D };
const s16 lbl_1_rodata_C8[8] = { 1, 0, 3, 2, 0, 0, 3, 2 };
const s16 lbl_1_rodata_D8[4] = { 1, 0, 20, 10 };

// NONMATCHING: ctor loop is rebuilt from m2c; the original stores and reloads the ctors pointer differently
int _prolog(void) {
    const VoidFunc *ctors = _ctors;
    int i;

    for (i = 0; ctors[i] != NULL; i++) {
        ctors[i]();
    }
    ObjectSetup();
    return 0;
}

// NONMATCHING: dtor loop rebuilt from m2c; same divergence as _prolog
void _epilog(void) {
    const VoidFunc *dtors = _dtors;
    int i;

    for (i = 0; dtors[i] != NULL; i++) {
        dtors[i]();
    }
}

// NONMATCHING: parman creation and layer call order differ from the original
s16 fn_1_A0(Vec *pos) {
    HU3DPARMANID parManId;

    parManId = Hu3DParManCreate(HuSprAnimRead(HuDataSelHeapReadNum(0x210002, 0x10000000, HEAP_MODEL)), 0x64, &lbl_1_data_258);
    Hu3DParManRotSet(parManId, 0.0f, 0.0f, 0.0f);
    Hu3DParManPosSet(parManId, pos->x, pos->y, pos->z);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(parManId), 2);
    return parManId;
}

// NONMATCHING: data block and field initialisation order still differ from the original
void ObjectSetup(void) {
    OMOBJ *obj;
    OMOBJ *flowObj;
    M524Flow *flow;
    HU3DLIGHTID lightId;
    s32 i;
    s32 n;
    s32 j;

    lbl_1_bss_80 = omInitObjMan(0xC8, 0x1000);
    lbl_1_bss_7C = -1;
    omGameSysInit(lbl_1_bss_80);
    fn_1_438();
    lbl_1_bss_18 = fn_1_35A8(&lbl_1_data_108);
    lightId = Hu3DGLightCreateV(&lbl_1_data_EC.pos, &lbl_1_data_EC.target, &lbl_1_data_EC.color);
    Hu3DGLightStaticSet(lightId, 1);
    Hu3DGLightInfinitytSet(lightId);
    Hu3DShadowCreate(20.0f, 20.0f, 7000.0f);
    Hu3DShadowTPLvlSet(0.35f);
    Hu3DShadowPosSet(&lbl_1_data_EC.pos, &lbl_1_data_108.up, &lbl_1_data_1E0);
    Hu3DShadowSizeSet(0x140);
    fn_1_478();
    omMakeGroupEx(lbl_1_bss_80, 0, 4);
    lbl_1_bss_1C = omGetGroupMemberListEx(lbl_1_bss_80, 0);
    for (i = 0; i < 4; i++) {
        obj = omAddObjEx(lbl_1_bss_80, 0x2000, 5, 9, 0, fn_1_848);
        lbl_1_bss_1C[i] = obj;
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, 0x68, 0x10000000);
        obj->work[0] = i;
    }
    for (n = 0; lbl_1_data_1EC[n] != NULL; n++) {
    }
    flowObj = omAddObjEx(lbl_1_bss_80, 0x2000, 0, 0, -1, fn_1_50B0);
    flowObj->data = HuMemDirectMallocNum(HEAP_HEAP, 0x68, 0x10000000);
    flow = (M524Flow *)flowObj->data;
    flow->funcNum = n;
    flow->funcTbl = lbl_1_data_1EC;
    flow->index = 0;
    flow->gameMesId = -1;
    for (j = 0; j < 0x10; j++) {
        flow->work[j] = 0;
    }
    lbl_1_bss_20 = flowObj;
    flow = (M524Flow *)lbl_1_bss_20->data;
    flow->timerType = 0;
    flow->maxTime = 0x12C;
    flow->endTime = 0;
    flow->recordTime = 0;
    WipeCreate(1, 5, 0x3C);
}

void fn_1_438(void) {
    fn_1_6D8();
    lbl_1_bss_30 = 0;
    lbl_1_bss_C = -1;
}

void fn_1_478(void) {
    s32 i;

    for (i = 0; i < 5; i++) {
        lbl_1_bss_24[i] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_rodata_94[i], 0x10000000, HEAP_MODEL));
        Hu3DModelAttrSet(lbl_1_bss_24[i], 1);
        Hu3DModelAttrSet(lbl_1_bss_24[i], 0x40000002);
        Hu3DModelPosSet(lbl_1_bss_24[i], 0.0f, 0.0f, 0.0f);
        switch (i) {
            case 0:
                Hu3DCameraMotionOn(lbl_1_bss_24[i], (u16)lbl_1_data_108.camNo);
                Hu3DModelPosSet(lbl_1_bss_24[i], lbl_1_data_218[1], 0.0f, 200.0f);
                break;
            case 1:
                Hu3DModelShadowMapSet(lbl_1_bss_24[i]);
                break;
            case 4:
                Hu3DModelAttrReset(lbl_1_bss_24[i], 0x40000002);
                Hu3DModelAttrSet(lbl_1_bss_24[i], 0x40000001);
                break;
        }
        Hu3DModelRotSet(lbl_1_bss_24[i], 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrReset(lbl_1_bss_24[i], 1);
    }
    lbl_1_bss_E = Hu3DModelCreate(HuDataSelHeapReadNum(0x52000E, 0x10000000, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_E, 1);
}

// NONMATCHING: shuffle loops produce a different register sequence
void fn_1_6D8(void) {
    s32 rand;
    s32 i;
    s32 j;
    s32 k;
    s16 r;
    s16 tmp;

    rand = frandmod(2);
    for (i = 0; i < 4; i++) {
        lbl_1_bss_10[i] = lbl_1_rodata_C8[rand * 4 + i * 2];
    }
    for (j = 0; j < 4; j++) {
        r = frandmod(4);
        tmp = lbl_1_bss_10[j];
        lbl_1_bss_10[j] = lbl_1_bss_10[r];
        lbl_1_bss_10[r] = tmp;
    }
    for (k = 0; k < 4; k++) {
        GwPlayer[k].mgCoin = lbl_1_rodata_D8[lbl_1_bss_10[k]];
    }
}

// NONMATCHING: player field initialisation order differs
void fn_1_848(OMOBJ *obj) {
    M524Player *player;
    s16 charNo;

    player = (M524Player *)obj->data;
    player->index = (s16)obj->work[0];
    player->charNo = GwPlayerConf[player->index].charNo;
    player->padNo = GwPlayerConf[player->index].padNo;
    player->grp = GwPlayerConf[player->index].grp;
    player->type = GwPlayerConf[player->index].type;
    player->dif = GwPlayerConf[player->index].dif;
    player->state = 0;
    player->traZ = 0.0f;
    player->traY = 0.0f;
    player->traX = 0.0f;
    player->rotZ = 0.0f;
    player->rotY = 0.0f;
    player->rotX = 0.0f;
    player->motTime = 0.0f;
    player->dir = 0;
    player->landedF = 0;
    player->doneF = 0;
    player->endF = 0;
    player->objNum = -1;
    charNo = player->charNo;
    obj->mdlId[0] = CharModelMotListCreate(charNo, 2, lbl_1_data_2A8, obj->mtnId);
    Hu3DModelShadowSet(obj->mdlId[0]);
    CharModelAttrSet(charNo, 0x40000001);
    CharMotionSet(charNo, obj->mtnId[0]);
    CharEffectLayerSet(2);
    player->traX = lbl_1_data_218[player->index] - 70.0f;
    player->traY = 0.0f;
    player->traZ = 320.0f;
    player->rotX = 0.0f;
    player->rotY = 90.0f;
    player->rotZ = 0.0f;
    omSetTra(obj, player->traX, player->traY, player->traZ);
    omSetRot(obj, player->rotX, player->rotY, player->rotZ);
    fn_1_AD0(obj);
    fn_1_C24(obj);
    fn_1_D08(obj);
    CharModelVoicePanSet(player->charNo, lbl_1_data_228[player->index]);
    obj->work[0] = 0;
    obj->objFunc = fn_1_E2C;
}

void fn_1_AD0(OMOBJ *obj) {
    M524Player *player;
    HU3DMODELID mdlId;

    player = (M524Player *)obj->data;
    mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(0x520004, 0x10000000, HEAP_MODEL));
    obj->mdlId[1] = mdlId;
    Hu3DModelAttrSet(mdlId, 0x40000002);
    Hu3DModelAttrSet(mdlId, 0x40000001);
    Hu3DModelPosSet(mdlId, lbl_1_data_218[player->index], 0.0f, 200.0f);
    Hu3DMotionStartEndSet(mdlId, 0.0f, 60.0f);
    mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(0x520005, 0x10000000, HEAP_MODEL));
    obj->mdlId[2] = mdlId;
    Hu3DModelAttrSet(mdlId, 0x40000002);
    Hu3DModelPosSet(mdlId, lbl_1_data_218[player->index] - 6.0f, 75.0f, 238.0f);
}

// NONMATCHING: model creation call sequence differs
void fn_1_C24(OMOBJ *obj) {
    M524Player *player;
    HU3DMODELID mdlId;

    player = (M524Player *)obj->data;
    mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_rodata_B8[lbl_1_bss_10[player->index]], 0x10000000, HEAP_MODEL));
    obj->mdlId[3] = mdlId;
    Hu3DModelAttrSet(mdlId, 0x40000002);
    Hu3DModelAttrSet(mdlId, 1);
    Hu3DModelPosSet(mdlId, lbl_1_data_218[player->index], 0.0f, 200.0f);
}

// NONMATCHING: shot vector is kept in r28 by the original and reloaded here
void fn_1_D08(OMOBJ *obj) {
    M524Player *player;
    HU3DMODELID mdlId;

    player = (M524Player *)obj->data;
    mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_rodata_A8[lbl_1_bss_10[player->index]], 0x10000000, HEAP_MODEL));
    obj->mdlId[4] = mdlId;
    Hu3DModelAttrSet(mdlId, 0x40000002);
    Hu3DModelAttrSet(mdlId, 1);
    player->shotX = lbl_1_data_218[player->index];
    player->shotY = 1000.0f;
    player->shotZ = -400.0f;
    player->shotPad = 0.0f;
    player->shotVelY = -40.0f;
    player->shotVelZ = 0.0f;
    Hu3DModelPosSetV(mdlId, (Vec *)&player->shotX);
}

// NONMATCHING: state dispatch differs from the original jump table
void fn_1_E2C(OMOBJ *obj) {
    M524Player *player;
    M524Flow *flow;

    player = (M524Player *)obj->data;
    switch (player->state) {
        case 0:
            fn_1_F84(obj);
            break;
        case 1:
            flow = (M524Flow *)lbl_1_bss_20->data;
            if (flow->index == 3) {
                player->state = 2;
            }
            break;
        case 2:
            fn_1_11D4(obj);
            if (player->state != 2) {
                obj->work[0] = 0;
                obj->work[1] = -1;
            }
            break;
        case 3:
            fn_1_161C(obj);
            if (player->state != 3) {
                obj->work[0] = 0;
            }
            break;
        case 4:
            fn_1_1CBC(obj);
            break;
        case 5:
            fn_1_1DF8(obj);
            break;
        case 6:
            fn_1_1FB8(obj);
            break;
        case 7:
            fn_1_20A0(obj);
            break;
        case 8:
            fn_1_21BC(obj);
            break;
        case 9:
            fn_1_2D30(obj);
            break;
        case 10:
            fn_1_2DB4(obj);
            break;
        case 11:
            fn_1_2EEC(obj);
            break;
    }
}

// NONMATCHING: float conversion sequence differs from the original
void fn_1_F84(OMOBJ *obj) {
    M524Player *player;
    M524Flow *flow;

    player = (M524Player *)obj->data;
    if (obj->work[0] == 0) {
        flow = (M524Flow *)lbl_1_bss_20->data;
        if (flow->index == 2) {
            Hu3DModelAttrReset(obj->mdlId[1], 0x40000002);
            CharModelAttrReset(player->charNo, 0x40000001);
            CharMotionSet(player->charNo, obj->mtnId[2]);
            CharMotionStartEndSet(player->charNo, 61.0f, 120.0f);
            CharMotionTimeSet(player->charNo, 61.0f);
            CharMotionShiftSet(player->charNo, obj->mtnId[1], 0.0f, 60.0f, 0x40000001);
        }
    }
    obj->work[0]++;
    player->rotY += 1.5f;
    player->traX = 1.1666666f * obj->work[0] + (lbl_1_data_218[player->index] - 70.0f);
    omSetTra(obj, player->traX, player->traY, player->traZ);
    omSetRot(obj, player->rotX, player->rotY, player->rotZ);
    if (obj->work[0] == 0x3C) {
        player->traX = lbl_1_data_218[player->index];
        omSetTra(obj, player->traX, player->traY, player->traZ);
        CharModelAttrSet(player->charNo, 0x40000002);
        CharMotionShiftSet(player->charNo, obj->mtnId[4], 0.0f, 15.0f, 0x40000002);
        obj->work[0] = 0;
        player->state = 1;
    }
}

void fn_1_11D4(OMOBJ *obj) {
    M524Player *player;
    M524Flow *flow;
    OMOBJ *flowObj;
    s32 index;
    s16 padNo;
    s16 padNo2;
    f32 len;
    f32 mag;
    f32 aim;
    f32 root;
    f32 ang;

    player = (M524Player *)obj->data;
    index = player->index;
    flowObj = lbl_1_bss_20;
    flow = (M524Flow *)flowObj->data;
    if (MgTimerDoneCheck(flow->timer)) {
        player->state = 4;
        return;
    }
    if (player->type != 0) {
        if (obj->work[0] == 0) {
            obj->work[0] = 1;
            obj->work[1] = frandmod(20) * 5;
        }
        if (obj->work[1] == 0) {
            player->aimAng = 0.0f;
            obj->work[0] = 0;
            player->state = 3;
        }
        obj->work[1]--;
        return;
    }
    padNo = player->padNo;
    len = (f32)(HuPadStkX[padNo] * HuPadStkX[padNo] + HuPadStkY[padNo] * HuPadStkY[padNo]);
    mag = len;
    root = sqrtf(mag);
    if (root >= 50.0f) {
        padNo2 = player->padNo;
        ang = (f32)(180.0 * (atan2((f32)HuPadStkY[padNo2], (f32)HuPadStkX[padNo2]) / M_PI) - 90.0);
        if (ang < 0.0f) {
            ang += 360.0f;
        }
        aim = ang;
        player->aimAng = aim;
        player->state = 3;
    }
}

f32 fn_1_1568(f32 ang0, f32 ang1) {
    f32 diff;

    if (ang1 > 180.0f) {
        ang1 -= 360.0f;
    }
    if (ang0 > 180.0f) {
        ang0 -= 360.0f;
    }
    diff = ang1 - ang0;
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    if (diff < -180.0f) {
        diff += 360.0f;
    }
    return diff;
}

void fn_1_161C(OMOBJ *obj) {
    M524Player *player;
    M524Flow *flow;
    OMOBJ *flowObj;
    s32 index;
    s16 padNo;
    f32 len;
    f32 diff;
    f32 padAngL;
    f32 mag;
    f32 ang;
    f32 diffA;
    f32 aimAngT;
    f32 padAngT;
    f32 mag2;
    f32 root;
    f32 diffB;
    f32 maxTime;

    player = (M524Player *)obj->data;
    index = player->index;
    padNo = player->padNo;
    flowObj = lbl_1_bss_20;
    flow = (M524Flow *)flowObj->data;
    if (MgTimerDoneCheck(flow->timer)) {
        player->state = 4;
        return;
    }
    if (player->type != 0) {
        mag = 20.0f;
        player->padAng = player->aimAng + (s32)frandmod(150) / 10.0f;
    } else {
        len = (f32)(HuPadStkX[padNo] * HuPadStkX[padNo] + HuPadStkY[padNo] * HuPadStkY[padNo]);
        mag2 = len;
        root = sqrtf(mag2);
        mag = root;
        ang = (f32)(180.0 * (atan2((f32)HuPadStkY[padNo], (f32)HuPadStkX[padNo]) / M_PI) - 90.0);
        if (ang < 0.0f) {
            ang += 360.0f;
        }
        padAngL = ang;
        player->padAng = padAngL;
    }
    if (mag < 10.0f) {
        player->state = 2;
        return;
    }
    padAngT = player->padAng;
    aimAngT = player->aimAng;
    if (padAngT > 180.0f) {
        padAngT -= 360.0f;
    }
    if (aimAngT > 180.0f) {
        aimAngT -= 360.0f;
    }
    diff = padAngT - aimAngT;
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    if (diff < -180.0f) {
        diff += 360.0f;
    }
    diffA = diff;
    diffB = diffA;
    player->aimAng = player->padAng;
    if (player->dir == 0) {
        if (fabs(diffB) < 10.0) {
            return;
        }
        if (diffB < 0.0f) {
            player->dir = -1;
            player->motTime = CharMotionMaxTimeGet(player->charNo);
        } else {
            player->dir = 1;
        }
    } else if (player->dir * diffB <= 0.0f) {
        return;
    }
    if (fabs(diffB) >= 50.0) {
        return;
    }
    maxTime = CharMotionMaxTimeGet(player->charNo);
    player->motTime = player->motTime + diffB / (360.0f / maxTime);
    if (obj->work[1] == -1) {
        obj->work[1] = HuAudFXPlay(lbl_1_data_230[player->index]);
    } else if (HuAudFXStatusGet(obj->work[1]) == 0) {
        obj->work[1] = -1;
    }
    if ((player->dir == 1 && player->motTime >= maxTime) || (player->dir == -1 && player->motTime <= 0.0f)) {
        player->motTime = 0.0f;
        CharMotionTimeSet(player->charNo, maxTime);
        Hu3DMotionTimeSet(obj->mdlId[2], 60.0f);
        Hu3DModelAttrReset(obj->mdlId[1], 0x40000001);
        player->state = 5;
    } else {
        CharMotionTimeSet(player->charNo, player->motTime);
        Hu3DMotionTimeSet(obj->mdlId[2], player->motTime);
    }
}

void fn_1_1CBC(OMOBJ *obj) {
    M524Player *player;

    player = (M524Player *)obj->data;
    if (obj->work[0] == 0) {
        CharModelAttrReset(player->charNo, 0x40000002);
        Hu3DModelAttrReset(obj->mdlId[2], 0x40000002);
        Hu3DModelAttrReset(obj->mdlId[1], 0x40000001);
        if (player->dir == -1) {
            CharModelAttrSet(player->charNo, 0x40000004);
            Hu3DModelAttrSet(obj->mdlId[2], 0x40000004);
        }
        obj->work[0] = 1;
        obj->work[2] = 0;
    }
    if (obj->work[1] != -1 && HuAudFXStatusGet(obj->work[1]) == 0) {
        obj->work[1] = -1;
    }
    if (obj->work[1] == -1) {
        obj->work[1] = HuAudFXPlay(lbl_1_data_230[player->index]);
    }
    if (CharMotionEndCheck(player->charNo) != 0) {
        obj->work[0] = 0;
        player->state = 5;
    }
    obj->work[2]++;
}

// NONMATCHING: one redundant li here shifts every branch target by 4 bytes
void fn_1_1DF8(OMOBJ *obj) {
    M524Player *player;

    player = (M524Player *)obj->data;
    switch (obj->work[0]) {
        case 0:
            Hu3DMotionStartEndSet(obj->mdlId[1], 60.0f, 100.0f);
            Hu3DMotionTimeSet(obj->mdlId[1], 60.0f);
            Hu3DModelAttrReset(obj->mdlId[3], 1);
            Hu3DModelAttrReset(obj->mdlId[3], 0x40000002);
            obj->work[0] = 1;
            obj->work[1] = 0x37;
            break;
        case 1:
            if (obj->work[1] != 0) {
                obj->work[1]--;
                if (obj->work[1] == 0xD) {
                    HuAudFXPlay(lbl_1_data_238[player->index]);
                }
                if (obj->work[1] == 0) {
                    CharMotionShiftSet(player->charNo, obj->mtnId[5], 0.0f, 30.0f, 0);
                    obj->work[0] = 2;
                    obj->work[1] = 0x3C;
                }
            }
            break;
        case 2:
            obj->work[1]--;
            if (obj->work[1] == 0) {
                CharMotionShiftSet(player->charNo, obj->mtnId[0], 0.0f, 20.0f, 0x40000001);
                player->landedF = 1;
                obj->work[1] = 0;
                obj->work[0] = 0;
                player->state = 6;
            }
            break;
    }
}

// NONMATCHING: register allocation differs; the original saves r25-r31
void fn_1_1FB8(OMOBJ *obj) {
    M524Player *player;
    M524Flow *flow;

    player = (M524Player *)obj->data;
    flow = (M524Flow *)lbl_1_bss_20->data;
    if (flow->index == 6) {
        if (obj->work[0] == 0) {
            obj->work[0] = 1;
            obj->work[1] = player->index * 40;
        }
        if (obj->work[1] == 0) {
            obj->work[0] = 0;
            HuAudFXPlay(lbl_1_data_240[player->index]);
            Hu3DModelAttrReset(obj->mdlId[4], 1);
            player->state = 7;
        } else {
            obj->work[1]--;
        }
    }
}

// NONMATCHING: register copies and stack frame size differ from the original
void fn_1_21BC(OMOBJ *obj) {
    M524Player *player;
    HU3DPARMANID parManId;
    Vec pos;
    u16 id;
    void *spriteData;
    ANIMDATA *anim;

    player = (M524Player *)obj->data;
    pos.x = player->shotX;
    pos.y = player->shotY;
    pos.z = 150.0f + player->shotZ;
    spriteData = HuDataSelHeapReadNum(0x210002, 0x10000000, HEAP_MODEL);
    anim = HuSprAnimRead(spriteData);
    parManId = Hu3DParManCreate(anim, 0x64, &lbl_1_data_258);
    Hu3DParManRotSet(parManId, 0.0f, 0.0f, 0.0f);
    Hu3DParManPosSet(parManId, pos.x, pos.y, pos.z);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(parManId), 2);
    id = parManId;
    player->effectId = id;
    Hu3DParManTimeLimitSet(player->effectId, 1);
    Hu3DModelKill(obj->mdlId[4]);
    HuAudFXPlay(lbl_1_data_248[player->index]);
    switch (lbl_1_bss_10[player->index]) {
        case 0:
            player->objNum = 1;
            fn_1_28C8(obj, (Vec *)&player->shotX);
            break;
        case 1:
            player->objNum = 1;
            fn_1_23C8(obj, (Vec *)&player->shotX, player->objNum);
            break;
        case 2:
            player->objNum = 10;
            fn_1_23C8(obj, (Vec *)&player->shotX, player->objNum);
            break;
        case 3:
            player->objNum = 20;
            fn_1_23C8(obj, (Vec *)&player->shotX, player->objNum);
            break;
    }
    CharMotionShiftSet(player->charNo, obj->mtnId[6], 0.0f, 10.0f, 0);
    player->timer = 10;
    player->state = 9;
}

// NONMATCHING: branch label layout differs, instruction stream is identical
void fn_1_20A0(OMOBJ *obj) {
    M524Player *player;

    player = (M524Player *)obj->data;
    if (Hu3DMotionEndCheck(obj->mdlId[4]) != 0) {
        player->state = 8;
        return;
    }
    if (Hu3DMotionTimeGet(obj->mdlId[4]) > 60.0f) {
        return;
    }
    if (player->shotY <= 760.0f && player->shotVelZ == 0.0f) {
        player->shotY = 760.0f;
        player->shotVelY = -5.2f;
        player->shotVelZ = 12.333333f;
        Hu3DModelAttrReset(obj->mdlId[4], 0x40000002);
    }
    player->shotY += player->shotVelY;
    player->shotZ += player->shotVelZ;
    Hu3DModelPosSetV(obj->mdlId[4], (Vec *)&player->shotX);
}

void fn_1_23C8(OMOBJ *obj, Vec *pos, s32 num) {
    M524Player *player;
    HU3DMODELID mdlId;
    s32 i;
    OMOBJ *newObj;
    M524Fall *fall;

    player = (M524Player *)obj->data;
    for (i = 0; i < num; i++) {
        newObj = omAddObjEx(lbl_1_bss_80, 0x2000, 1, 0, -1, fn_1_2760);
        newObj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M524Fall), HU_MEMNUM_OVL);
        newObj->work[0] = player->index;
        newObj->work[1] = i;
        newObj->work[2] = i * 3;
        fall = (M524Fall *)newObj->data;
        mdlId = Hu3DModelLink(lbl_1_bss_E);
        newObj->mdlId[0] = mdlId;
        fall->posX = pos->x + (s32)frandmod(80) - 40.0f;
        fall->posY = pos->y + (s32)frandmod(200) - 100.0f;
        fall->posZ = pos->z + (s32)frandmod(100) - 50.0f;
        fall->vel0 = 0.0f;
        fall->velY = 0.0f;
        fall->velZ = 0.0f;
        omSetTra(newObj, fall->posX, fall->posY, fall->posZ);
        omSetSca(newObj, 1.0f, 1.0f, 1.0f);
        Hu3DMotionTimeSet(mdlId, (s32)frandmod(58));
        Hu3DModelAttrSet(mdlId, 0x40000001);
        Hu3DModelAttrReset(mdlId, 1);
        Hu3DModelShadowSet(mdlId);
    }
}

void fn_1_2630(OMOBJ *obj) {
    M524Fall *fall;
    M524Player *player;
    Vec pos;

    fall = (M524Fall *)obj->data;
    if (obj->work[2] != 0) {
        obj->work[2]--;
    } else {
        player = (M524Player *)lbl_1_bss_1C[obj->work[0]]->data;
        fall->velY += 1.96f;
        fall->posY -= fall->velY;
        omSetTra(obj, fall->posX, fall->posY, fall->posZ);
        if (fall->posY <= CharModelHeightGet(player->charNo)) {
            pos.x = player->traX;
            pos.y = fall->posY;
            pos.z = player->traZ;
            HuAudFXPlayPan(7, lbl_1_data_228[player->index]);
            CharEffectCoinGlowCreate(lbl_1_data_108.camNo, &pos);
            player->objNum--;
            obj->objFunc = fn_1_27D8;
        }
    }
}

void fn_1_2760(OMOBJ *obj) {
    M524Player *player;

    player = (M524Player *)lbl_1_bss_1C[obj->work[0]]->data;
    if (player->timer == 0) {
        if (lbl_1_bss_10[player->index] == 0) {
            obj->objFunc = fn_1_29F4;
        } else {
            obj->objFunc = fn_1_2630;
        }
    }
}

void fn_1_27D8(OMOBJ *obj) {
    M524Player *player;

    player = (M524Player *)lbl_1_bss_1C[obj->work[0]]->data;
    if (lbl_1_bss_10[player->index] != 0) {
        Hu3DModelKill(obj->mdlId[0]);
        omDelObjEx(lbl_1_bss_80, obj);
    } else if (obj->work[1] != 0) {
        obj->work[1]--;
        if (obj->work[1] == 0) {
            Hu3DMotionTimeSet(obj->mdlId[0], (s32)frandmod(150) + 50);
        }
    }
}

void fn_1_28C8(OMOBJ *obj, Vec *pos) {
    M524Player *player;
    HU3DMODELID mdlId;
    OMOBJ *newObj;
    M524Fall *fall;

    player = (M524Player *)obj->data;
    mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(0x52000F, 0x10000000, HEAP_MODEL));
    newObj = omAddObjEx(lbl_1_bss_80, 0x2000, 1, 0, -1, fn_1_2760);
    newObj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M524Fall), HU_MEMNUM_OVL);
    newObj->work[0] = player->index;
    fall = (M524Fall *)newObj->data;
    newObj->mdlId[0] = mdlId;
    Hu3DModelAttrSet(mdlId, 0x40000002);
    Hu3DModelShadowSet(mdlId);
    fall->posX = pos->x;
    fall->posY = pos->y - 100.0f;
    fall->posZ = pos->z;
    fall->vel0 = 0.0f;
    fall->velY = 0.0f;
    fall->velZ = 0.0f;
    omSetTra(newObj, fall->posX, fall->posY, fall->posZ);
}

void fn_1_29F4(OMOBJ *obj) {
    M524Fall *fall;
    s16 index;
    OMOBJ *playerObj;
    M524Player *player;
    M524Camera *cam;
    OMOBJ *newObj2;
    M524Camera *cam2;

    fall = (M524Fall *)obj->data;
    index = obj->work[0];
    playerObj = lbl_1_bss_1C[index];
    player = (M524Player *)lbl_1_bss_1C[index]->data;
    fall->velY += 1.96f;
    fall->posY -= fall->velY;
    omSetTra(obj, fall->posX, fall->posY, fall->posZ);
    if (fall->posY < CharModelHeightGet(player->charNo) && player->doneF == 0) {
        CharModelAttrReset(player->charNo, 0x40000002);
        CharMotionShiftSet(player->charNo, playerObj->mtnId[7], 0.0f, 5.0f, 0);
        Hu3DMotionShiftStartEndSet(playerObj->mdlId[0], 0.0f, 30.0f);
        Hu3DModelAttrReset(playerObj->mdlId[1], 0x40000002);
        Hu3DModelAttrReset(playerObj->mdlId[1], 0x40000001);
        Hu3DMotionStartEndSet(playerObj->mdlId[1], 101.0f, 105.0f);
        Hu3DMotionTimeSet(playerObj->mdlId[1], 101.0f);
        player->doneF = 1;
    }
    omSetTra(obj, fall->posX, fall->posY, fall->posZ);
    if (fall->posY <= 0.0f) {
        fall->posY = 0.0f;
        omSetTra(obj, fall->posX, fall->posY, fall->posZ);
        player->objNum--;
        Hu3DModelAttrReset(obj->mdlId[0], 0x40000002);
        Hu3DModelAttrSet(obj->mdlId[0], 0x40000001);
        fall->posZ += 150.0f;
        CharEffectSmokeCreate(lbl_1_data_108.camNo, (Vec *)&fall->posX);
        cam = lbl_1_bss_18;
        newObj2 = omAddObjEx(lbl_1_bss_80, 0x2000, 0, 0, -1, fn_1_4C18);
        newObj2->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M524Camera), HU_MEMNUM_OVL);
        cam2 = (M524Camera *)newObj2->data;
        newObj2->work[0] = 0xF;
        newObj2->work[1] = 5;
        *cam2 = *cam;
        newObj2->work[2] = (u32)cam;
        omVibrate(player->index, 15, 12, 0);
        HuAudFXPlayPan(0x2C1, lbl_1_data_228[player->index]);
        HuAudFXPlay(lbl_1_data_250[player->index]);
        obj->work[1] = 0x32;
        obj->objFunc = fn_1_27D8;
    }
}
void fn_1_2D30(OMOBJ *obj) {
    M524Player *player;
    s32 i;

    player = (M524Player *)obj->data;
    if (player->timer != 0) {
        player->timer--;
    }
    for (i = 0; i < 4; i++) {
        player = (M524Player *)lbl_1_bss_1C[i]->data;
        if (player->objNum != 0) {
            return;
        }
    }
    player = (M524Player *)obj->data;
    player->endF = 1;
    player->state = 10;
}

void fn_1_2DB4(OMOBJ *obj) {
    M524Player *player;

    player = (M524Player *)obj->data;
    if (lbl_1_bss_10[player->index] != 0) {
        if (obj->work[0] == 0) {
            obj->work[0] = 1;
            CharModelAttrReset(player->charNo, 0x40000002);
            CharMotionShiftSet(player->charNo, obj->mtnId[3], 1.0f, 20.0f, 0);
            Hu3DMotionShiftStartEndSet(obj->mdlId[0], 1.0f, 60.0f);
        } else {
            player->rotY -= 3.0f;
            omSetRot(obj, player->rotX, player->rotY, player->rotZ);
            if (CharMotionShiftIDGet(player->charNo) == -1 && CharMotionEndCheck(player->charNo)) {
                obj->work[0] = 0;
                player->state = 0xB;
            }
        }
    } else {
        player->state = 0xB;
    }
}

void fn_1_2EEC(OMOBJ *obj) {
    M524Player *player;
    OMOBJ *flowObj;
    void *flowData;
    s32 indexIsEight;
    M524Flow *flow;
    s32 flowIndex;
    s32 flowIndexCopy;

    player = (M524Player *)obj->data;
    flowObj = lbl_1_bss_20;
    flowData = flowObj->data;
    flow = (M524Flow *)flowObj->data;
    flowIndex = flow->index;
    flowIndexCopy = flowIndex;
    if (flowIndexCopy == 8) {
        indexIsEight = 1;
    } else {
        indexIsEight = 0;
    }
    if (indexIsEight != 0) {
        if (lbl_1_bss_10[player->index] != 0) {
            CharMotionShiftSet(player->charNo, obj->mtnId[8], 0.0f, 20.0f, 0);
        }
        player->state = 0xC;
    }
}

void fn_1_2FB0(OMOBJ *obj, void *flowArg) {
    M524Flow *flow;

    if (lbl_1_bss_0 == 0) {
        Hu3DCameraMotionStart(lbl_1_bss_24[0], (u16)lbl_1_data_108.camNo);
        Hu3DModelAttrReset(lbl_1_bss_24[0], 0x40000002);
        lbl_1_bss_7C = HuAudSStreamPlay(18);
        HuAudFXPlay(0x753);
        lbl_1_bss_0 = 1;
    }
    if (Hu3DMotionEndCheck(lbl_1_bss_24[0])) {
        Hu3DModelKill(lbl_1_bss_24[0]);
        flow = (M524Flow *)obj->data;
        if (flow->index == flow->funcNum - 1) {
            OSReport(lbl_1_data_CC);
        } else {
            flow->index++;
        }
    }
}

void fn_1_30BC(OMOBJ *obj, void *flowArg) {
    M524Player *player;
    M524Flow *flow;
    M524Flow *flow2;
    s32 i;

    if (lbl_1_bss_4 == 0) {
        for (i = 0; i < 4; i++) {
            player = (M524Player *)lbl_1_bss_1C[i]->data;
            if (player->state < 5) {
                break;
            }
        }
        if (i == 4) {
            flow = (M524Flow *)obj->data;
            if (flow->timer->stopF == 0) {
                flow->timer->stopF = 1;
                flow->timer->mode = 1;
            }
            lbl_1_bss_4 = 1;
        }
    }
    for (i = 0; i < 4; i++) {
        player = (M524Player *)lbl_1_bss_1C[i]->data;
        if (player->landedF == 0) {
            return;
        }
    }
    flow2 = (M524Flow *)obj->data;
    if (flow2->index == flow2->funcNum - 1) {
        OSReport(lbl_1_data_CC);
    } else {
        flow2->index++;
    }
}

void fn_1_31EC(OMOBJ *obj, void *flowArg) {
    M524Flow *flow;
    OMOBJ *cameraObj;
    M524CamMove *move;
    s32 moveActive;
    OMOBJ *cameraObjDelete;

    if (lbl_1_bss_8 == NULL) {
        lbl_1_bss_8 = MgCameraMoveEntry(lbl_1_bss_18, &lbl_1_data_150, 30.0f);
    }
    cameraObj = lbl_1_bss_8;
    move = (M524CamMove *)cameraObj->data;
    if ((move->moveF & 2) != 0) {
        moveActive = 1;
    } else {
        moveActive = 0;
    }
    if (moveActive == 0) {
        cameraObjDelete = lbl_1_bss_8;
        omDelObjEx(lbl_1_bss_80, cameraObjDelete);
        flow = (M524Flow *)obj->data;
        if (flow->index == flow->funcNum - 1) {
            OSReport(lbl_1_data_CC);
        } else {
            flow->index++;
        }
    }
}

void fn_1_32F0(OMOBJ *obj, void *flowArg) {
    M524Player *player;
    M524Flow *flow;
    s32 i;

    for (i = 0; i < 4; i++) {
        player = (M524Player *)lbl_1_bss_1C[i]->data;
        if (player->endF == 0) {
            return;
        }
    }
    HuAudSStreamFadeOut(lbl_1_bss_7C, 100);
    flow = (M524Flow *)obj->data;
    if (flow->index == flow->funcNum - 1) {
        OSReport(lbl_1_data_CC);
    } else {
        flow->index++;
    }
}

void fn_1_33B4(OMOBJ *obj, void *flowArg) {
    M524Flow *flow;

    if (obj->work[0] == 0) {
        obj->work[0] = 1;
        obj->work[1] = 0xD2;
        lbl_1_bss_C = HuAudJinglePlay(0x79);
    }
    obj->work[1]--;
    if (obj->work[1] != 0) {
        return;
    }
    WipeCreate(2, 6, 0x3C);
    lbl_1_bss_18->mode = 0;
    lbl_1_data_198 = *lbl_1_bss_18;
    fn_1_3DD0(&lbl_1_data_198);
    lbl_1_data_198.pos.z += 1000.0f;
    lbl_1_data_198.target.y += 400.0f;
    lbl_1_data_198.pos.y -= 200.0f;
    MgCameraMoveEntry(lbl_1_bss_18, &lbl_1_data_198, 120.0f);
    flow = (M524Flow *)obj->data;
    if (flow->index == flow->funcNum - 1) {
        OSReport(lbl_1_data_CC);
    } else {
        flow->index++;
    }
}

void fn_1_3554(void) {
    lbl_1_bss_80 = omInitObjMan(0xC8, 0x1000);
    lbl_1_bss_7C = -1;
    omGameSysInit(lbl_1_bss_80);
}

M524Camera *fn_1_35A8(M524Camera *param) {
    M524Camera *cam;

    cam = HuMemDirectMallocNum(HEAP_HEAP, 0x48, HU_MEMNUM_OVL);
    *cam = *param;
    Hu3DCameraCreate(param->camNo);
    Hu3DCameraViewportSet(param->camNo, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(param->camNo, param->nearZ, param->fov, param->farZ, 1.2f);
    fn_1_3694(param);
    return cam;
}


void fn_1_3694(M524Camera *cam)
{
    if (cam->mode == 1) {
        fn_1_3DD0(cam);
    }
    Hu3DCameraPosSetV(cam->camNo, &cam->pos, &cam->up, &cam->target);
}

s16 fn_1_36E4(M524Light *light)
{
    HU3DLIGHTID lightId;

    lightId = Hu3DGLightCreateV(&light->pos, &light->target, &light->color);
    Hu3DGLightStaticSet(lightId, 1);
    Hu3DGLightInfinitytSet(lightId);
    return lightId;
}

s32 fn_1_4BB0(OMOBJ *obj)
{
    M524CamMove *move = (M524CamMove *)obj->data;

    if (move->moveF & 0x40000000) {
        return 1;
    }
    return 0;
}

void fn_1_4BE4(OMOBJ *obj)
{
    omDelObjEx(lbl_1_bss_80, obj);
}

void fn_1_5160(OMOBJ *obj)
{
    if (!WipeCheckEnd()) {
        omOvlReturnEx(1, 1);
    }
}

void MgFlowSet(OMOBJ *obj, s32 index)
{
    M524Flow *flow = (M524Flow *)obj->data;

    if (index >= flow->funcNum) {
        OSReport(lbl_1_data_AD);
    } else {
        flow->index = index;
    }
}

void MgFlowNext(OMOBJ *obj)
{
    M524Flow *flow = (M524Flow *)obj->data;

    if (flow->index == flow->funcNum - 1) {
        OSReport(lbl_1_data_CC);
    } else {
        flow->index++;
    }
}

s32 fn_1_5338(OMOBJ *obj)
{
    M524Flow *flow = (M524Flow *)obj->data;

    return flow->index;
}

s32 fn_1_5354(OMOBJ *obj, s32 index)
{
    M524Flow *flow = (M524Flow *)obj->data;

    if (flow->index == index) {
        return 1;
    }
    return 0;
}

void fn_1_53A0(OMOBJ *obj, s32 type, s32 maxTime, s32 endTime, s32 recordTime)
{
    M524Flow *flow = (M524Flow *)obj->data;

    flow->timerType = type;
    flow->maxTime = maxTime * 60;
    flow->endTime = endTime * 60;
    flow->recordTime = recordTime * 60;
}

BOOL fn_1_53D4(OMOBJ *obj)
{
    M524Flow *flow = (M524Flow *)obj->data;

    return MgTimerDoneCheck(flow->timer);
}

void fn_1_5410(OMOBJ *obj)
{
    M524Flow *flow = (M524Flow *)obj->data;

    if (flow->timer->stopF == 0) {
        flow->timer->stopF = 1;
        flow->timer->mode = 1;
    }
}

void fn_1_5450(void)
{
}

f32 fn_1_5E04(Vec *a, Vec *b, Vec *c)
{
    Vec vec;

    PSVECSubtract(c, b, &vec);
    return PSVECDotProduct(a, &vec);
}

void fn_1_3DA0(M524Camera *cam)
{
    lbl_1_bss_34 = *cam;
}

void fn_1_54CC(OMOBJ *obj, void *flow)
{
    if (lbl_1_bss_7C != -1) {
        HuAudFadeOut(0x3e8);
        lbl_1_bss_7C = -1;
    }
    if (!WipeCheckEnd()) {
        omOvlReturnEx(1, 1);
    }
}

void fn_1_5D8C(Vec *pos, Vec *target, Vec *cur, Vec *out)
{
    Vec sp10;
    Vec sp1C;

    PSVECSubtract(cur, target, &sp1C);
    PSVECSubtract(target, pos, &sp10);
    PSVECCrossProduct(&sp1C, &sp10, out);
    PSVECNormalize(out, out);
}

void fn_1_5454(OMOBJ *obj, void *flow)
{
    if (!WipeCheckEnd()) {
        M524Flow *flowP = (M524Flow *)obj->data;

        if (flowP->index == flowP->funcNum - 1) {
            OSReport(lbl_1_data_CC);
        } else {
            flowP->index++;
        }
    }
}

void fn_1_55EC(OMOBJ *obj, void *flow)
{
    M524Flow *flowP = (M524Flow *)obj->data;

    if (flowP->gameMesId == -1) {
        flowP->gameMesId = GameMesCreate(2, 0);
    } else if (!GameMesStatGet(flowP->gameMesId)) {
        flowP->gameMesId = -1;
        if (flowP->index == flowP->funcNum - 1) {
            OSReport(lbl_1_data_CC);
        } else {
            flowP->index++;
        }
    }
}

void fn_1_56A0(OMOBJ *obj, void *flow)
{
    M524Flow *flowP = (M524Flow *)obj->data;

    if (flowP->gameMesId == -1) {
        flowP->gameMesId = GameMesCreate(2, 1);
    } else if (!GameMesStatGet(flowP->gameMesId)) {
        flowP->gameMesId = -1;
        if (flowP->index == flowP->funcNum - 1) {
            OSReport(lbl_1_data_CC);
        } else {
            flowP->index++;
        }
    }
}

void fn_1_5754(OMOBJ *obj, void *flow)
{
    M524Flow *flowP = (M524Flow *)obj->data;

    flowP->timer = MgTimerCreate(flowP->timerType);
    MgTimerParamSet(flowP->timer, flowP->maxTime, flowP->endTime, flowP->recordTime);
    MgTimerModeOnSet(flowP->timer, 1);
    if (flowP->index == flowP->funcNum - 1) {
        OSReport(lbl_1_data_CC);
    } else {
        flowP->index++;
    }
}

void fn_1_5530(OMOBJ *obj, void *flow, s16 mesNo, s16 mesArg)
{
    M524Flow *flowP = (M524Flow *)obj->data;

    if (flowP->gameMesId == -1) {
        flowP->gameMesId = GameMesCreate(mesNo, mesArg);
    } else if (!GameMesStatGet(flowP->gameMesId)) {
        flowP->gameMesId = -1;
        if (flowP->index == flowP->funcNum - 1) {
            OSReport(lbl_1_data_CC);
        } else {
            flowP->index++;
        }
    }
}

f32 fn_1_4E1C(s16 padNo)
{
    s32 sum;
    f32 mag;

    sum = (s32)HuPadStkX[padNo] * (s32)HuPadStkX[padNo] + (s32)HuPadStkY[padNo] * (s32)HuPadStkY[padNo];
    mag = (f32) sum;
    if (mag > 0.0f) {
        mag = sqrtf(mag);
    }
    return mag;
}

f32 fn_1_4FA0(s16 padNo)
{
    f32 angle;

    angle = (f32) (180.0 * (atan2((f64) (s8) HuPadStkY[padNo], (f64) (s8) HuPadStkX[padNo]) / 3.141592653589793) - 90.0);
    if (angle < 0.0f) {
        angle += 360.0f;
    }
    return angle;
}

void fn_1_57F4(M524Player *player, s16 index)
{
    player->index = index;
    player->charNo = GwPlayerConf[index].charNo;
    player->padNo = GwPlayerConf[index].padNo;
    player->grp = GwPlayerConf[index].grp;
    player->type = GwPlayerConf[index].type;
    player->dif = GwPlayerConf[index].dif;
    player->state = 0;
    player->traX = 0.0f;
    player->traY = 0.0f;
    player->traZ = 0.0f;
    player->rotX = 0.0f;
    player->rotY = 0.0f;
    player->rotZ = 0.0f;
}

f32 fn_1_59E4(f32 ang0, f32 ang1, f32 max)
{
    f32 result;
    f32 diff;

    diff = (f32) fmod((f64) (ang1 - ang0), 360.0);
    if (diff < 0.0f) {
        diff += 360.0f;
    }
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    if (fabs(diff) > max) {
        if (diff < 0.0f) {
            diff = -max;
        } else {
            diff = max;
        }
    }
    result = (f32) fmod((f64) (ang0 + diff), 360.0);
    if (result < 0.0f) {
        result += 360.0f;
    }
    return result;
}

s32 fn_1_58C0(Vec *pos, Vec *target, Vec *angle, f32 dist)
{
    Vec vec;
    f32 mag;

    PSVECSubtract(target, pos, &vec);
    mag = PSVECMag(&vec);
    if (mag == 0.0f) {
        return 1;
    }
    PSVECNormalize(&vec, &vec);
    PSVECScale(&vec, &vec, dist);
    if (PSVECMag(&vec) >= mag) {
        *pos = *target;
        return 1;
    }
    PSVECAdd(pos, &vec, pos);
    angle->y = (f32) (180.0 * (atan2((f64) vec.x, (f64) vec.z) / 3.141592653589793));
    return 0;
}

s32 fn_1_5B54(Vec *pos, Vec *target, Vec *cur, f32 dist)
{
    Vec sp20;
    Vec sp14;
    Vec sp8;
    f32 temp_f1;
    f32 temp_f30;
    f32 temp_f31;

    sp8 = *target;
    while (TRUE) {
        PSVECSubtract(target, pos, &sp20);
        if (PSVECMag(&sp20) == 0.0f) {
            return 0;
        }
        PSVECSubtract(target, cur, &sp14);
        temp_f1 = PSVECMag(&sp14);
        if (temp_f1 >= dist) {
            return 0;
        }
        if (temp_f1 == 0.0f) {
            *target = *pos;
        } else {
            PSVECNormalize(&sp14, &sp14);
            PSVECScale(&sp14, &sp14, dist);
            PSVECAdd(cur, &sp14, target);
        }
        PSVECSubtract(target, pos, &sp14);
        if (PSVECMag(&sp14) >= 0.001f) {
            return 1;
        }
        temp_f31 = PSVECMag(&sp20);
        temp_f30 = (f32) (180.0 * (atan2((f64) sp14.z, (f64) sp14.x) / 3.141592653589793));
        temp_f30 += 30.0f;
        sp20.x = (f32) ((f64) temp_f31 * sin((3.141592653589793 * (f64) temp_f30) / 180.0));
        sp20.z = (f32) ((f64) temp_f31 * cos((3.141592653589793 * (f64) temp_f30) / 180.0));
        PSVECAdd(pos, &sp20, target);
    }
}

s32 fn_1_5E48(Vec *dir, Vec *pos, Vec *target, Vec *out, Vec *result)
{
    Vec sp28;
    Vec sp1C;
    Vec sp10;
    f32 temp_f1;
    f32 temp_f30;

    PSVECSubtract(out, target, &sp28);
    PSVECSubtract(target, pos, &sp1C);
    temp_f30 = PSVECDotProduct(dir, &sp1C);
    PSVECSubtract(target, out, &sp10);
    temp_f1 = PSVECDotProduct(dir, &sp10);
    if (temp_f1 == 0.0f) {
        return 0;
    }
    PSVECScale(&sp28, &sp28, temp_f30 / temp_f1);
    PSVECAdd(target, &sp28, result);
    return 1;
}