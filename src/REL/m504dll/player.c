#include "types.h"
#include "string.h"

#include "REL/m504dll.h"

typedef struct M504Work_s {
    OMOBJMAN *objMan;
    u8 pad4[0x1218 - 0x4];
    s32 unk1218;
} M504WORK;

typedef struct M504Model_s {
    u8 pad0[0x30];
    s16 modelId;
    s32 unk34;
} M504MODEL;

typedef struct M504Ctrl_s {
    void *unk0;
    u8 pad4[0xDC - 0x4];
    M504MODEL model;
    u8 pad114[0x15C - 0x114];
    Vec *unk15C;
    u8 pad160[0x3BC - 0x160];
    Vec *unk3BC;
    u8 pad3C0[0x46C - 0x3C0];
    s32 unk46C;
} M504CTRL;

typedef struct M504Player_s {
    M504CTRL *ctrl;
    s16 state;
    s16 unk6;
    s16 unk8;
    u8 padA[6];
    u32 charNo;
    u8 pad14[4];
    OMOBJ *obj;
    u8 pad1C[4];
    INPUTMAIN input;
    Vec pos;
    Vec camPos;
    u8 pad78[0x9C - 0x78];
    s16 unk9C;
    s16 unk9E;
    Vec posA0;
    Vec posAC;
    Vec posB8;
    s32 unkC4;
    s16 mdlC8;
    s16 mtnCA;
    s16 mdlCC;
} M504PLAYER;

void fn_1_B898(INPUTMAIN *main);
void fn_1_B8D8(INPUTMAIN *main);
void fn_1_5F58(M504PLAYER *player);
void fn_1_9334(M504PLAYER *player);
s32 fn_1_B52C(M504MODEL *model, Vec *outPos);

static const Vec m504ZeroVec = { 0.0f, 0.0f, 0.0f };

void fn_1_5E1C(M504PLAYER *player)
{
    register M504CTRL *ctrl;
    register M504WORK *work;

    ctrl = player->ctrl;
    work = ctrl->unk0;

    fn_1_B898(&player->input);
    Hu3DModelHookReset(player->obj->mdlId[0]);
    Hu3DModelKill(player->mdlCC);
    Hu3DMotionKill(player->mtnCA);
    Hu3DModelKill(player->mdlC8);

    if (player->obj != 0) {
        CharModelKill((s16) player->charNo);
        omDelObjEx(work->objMan, player->obj);
        player->obj = NULL;
    }
    memset(player, 0, 0xD4);
}

s32 fn_1_5EC8(M504PLAYER *player)
{
    register M504WORK *work;

    work = player->ctrl->unk0;

    if (player->unkC4 != 0) {
        fn_1_B8D8(&player->input);
    } else {
        player->input.btn = player->input.btnDown = player->input.stkX = player->input.stkY = 0;
    }

    player->state = 0;

    if (work->unk1218 == 0) {
        fn_1_5F58(player);
        fn_1_9334(player);
    }
    return 0;
}

float fn_1_68E8(float cur, float from, float mul)
{
    register f32 ang;
    register f32 wrap;
    register f32 res;

    wrap = fmod(from - cur, 360);
    ang = wrap;
    if (ang < 0.0f) {
        ang += 360.0f;
    }
    if (ang > 180.0f) {
        ang -= 360.0f;
    }
    res = fmod(cur + mul * ang, 360);
    ang = res;
    if (ang < 0.0f) {
        ang += 360.0f;
    }
    return ang;
}

void fn_1_6A04(M504PLAYER *player)
{
    register M504CTRL *ctrl;
    register M504MODEL *model;
    Vec **posTblA;
    Vec **posTblB;

    ctrl = player->ctrl;
    posTblA = &ctrl->unk3BC;
    posTblB = &ctrl->unk15C;
    model = &ctrl->model;

    Hu3DModelPosSet(model->modelId, player->pos.x, 200.0f + player->pos.y, player->pos.z);
    Hu3DMotionTimeSet(model->modelId, 0.0f);
    Hu3DModelAttrReset(model->modelId, 1);
    model->unk34 = 1;
}

void fn_1_91B0(M504PLAYER *player)
{
    register M504CTRL *ctrl;
    register M504MODEL *model;
    Vec mid;
    Vec modelPos;
    Vec zero;
    Vec *zeroP;

    ctrl = player->ctrl;
    model = &ctrl->model;

    if (fn_1_B52C(model, &modelPos) == 0) {
        player->camPos.x = 0.0f;
        player->camPos.y = 40.0f;
        player->camPos.z = 0.0f;
        player->unk8 = 1;
    } else {
        player->unk9C = 0;
        player->unk9E = 0;

        player->posA0 = player->pos;
        player->posB8 = modelPos;

        PSVECSubtract(&player->posB8, &player->posA0, &mid);
        PSVECScale(&mid, &mid, 0.5f);
        PSVECAdd(&mid, &player->posA0, &mid);

        mid.y += 600.0f;
        player->posAC = mid;

        zero = m504ZeroVec;
        zeroP = &zero;
        player->camPos = *zeroP;
        player->unk8 = 4;
    }
}