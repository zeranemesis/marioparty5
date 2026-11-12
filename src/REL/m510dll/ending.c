#define _MATH_H
#include "REL/m510dll.h"
#include "string.h"

extern double sin(double);
extern double cos(double);

#define M_PI_D 3.141592653589793

// Wraps an angle in degrees into (-180, 180]; a macro in the original, so the
// argument is evaluated several times.
#define WRAP180(a) (((f32)(a) > 180.0f) ? ((a) - 360.0f) : (((f32)(a) <= -180.0f) ? ((a) + 360.0f) : (a)))

// Confetti, 0x18 bytes
typedef struct EndFxA_s {
    s16 state;    //0x00
    s16 timer;    //0x02
    s32 active;   //0x04
    s16 index;    //0x08
    s16 modelId;  //0x0A
    f32 x;        //0x0C
    f32 y;        //0x10
    f32 z;        //0x14
} END_FX_A;

// Streamer, 0x20 bytes
typedef struct EndFxB_s {
    s16 state;    //0x00
    s16 timer;    //0x02
    s32 active;   //0x04
    s16 index;    //0x08
    s16 modelId;  //0x0A
    f32 x;        //0x0C
    f32 y;        //0x10
    f32 z;        //0x14
    f32 angle;    //0x18
    f32 speed;    //0x1C
} END_FX_B;

// Ending work (0x9C0 bytes)
typedef struct EndWork_s {
    s32 frame;             //0x000
    s16 modelId0;          //0x004
    s16 modelId[4];        //0x006
    s16 resModelId[3];     //0x00E
    s16 motionId0;         //0x014
    s16 motionId[4];       //0x016
    u8 pad1E[2];           //0x01E
    END_FX_A fxA[16];      //0x020
    END_FX_B fxB[64];      //0x1A0
    s32 active;            //0x9A0
    s32 score;             //0x9A4
    f32 y1;                //0x9A8
    f32 v1;                //0x9AC
    f32 y2;                //0x9B0
    f32 v2;                //0x9B4
    f32 y3;                //0x9B8
    f32 v3;                //0x9BC
} END_WORK;

// .data:0x0 (absolute 0x450)
M510_VECF lbl_1_data_450 = { 0.0f, 2935.0f, 30070.0f };
// .data:0xC (absolute 0x45C)
M510_VECF lbl_1_data_45C = { 0.0f, 230.0f, 400.0f };
// .data:0x18 (absolute 0x468)
M510_VECF lbl_1_data_468 = { 0.0f, 0.0f, 0.0f };
// .data:0x24 (absolute 0x474)
M510_VECF lbl_1_data_474 = { 0.0f, 0.0f, 0.0f };
// .data:0x30 (absolute 0x480)
s32 lbl_1_data_480 = 1;
// .data:0x54 (absolute 0x4A4)
M510_VECF lbl_1_data_4A4 = { 385.0f, 130.0f, -940.0f };
// .data:0x60 (absolute 0x4B0)
M510_VECF lbl_1_data_4B0 = { 670.0f, 185.0f, -860.0f };
// .data:0x6C (absolute 0x4BC)
M510_VECF lbl_1_data_4BC = { 360.0f, 40.0f, -850.0f };
// .data:0x78 (absolute 0x4C8)
M510_VECF lbl_1_data_4C8 = { 635.0f, 65.0f, -720.0f };

// The REL's global object pointer lives in m510.c
extern M510_OBJ *lbl_1_bss_0;

s16 lbl_1_bss_7820;
END_WORK lbl_1_bss_7824;

void fn_1_9B0C(M510_OBJ *data)
{
    M510_OBJ *obj = data;
    M510_OPEN_CAM *cam = &obj->cam;
    s16 modelId;
    s16 i;
    s16 id;

    memset(&lbl_1_bss_7824, 0, sizeof(END_WORK));

    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x440008, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_7824.modelId0 = modelId;
    Hu3DModelPosSet(modelId, -300.0f, 300.0f, 0.0f);
    Hu3DModelRotSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(modelId, 10.0f, 10.0f, 10.0f);
    Hu3DModelCameraSet(modelId, cam->unk04 & 0xFFFF);
    Hu3DModelLayerSet(modelId, 4);
    Hu3DModelAttrSet(modelId, 1);
    lbl_1_bss_7824.motionId0 = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x440019, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DMotionSet(modelId, lbl_1_bss_7824.motionId0);
    Hu3DModelAttrSet(modelId, 0x40000001);

    lbl_1_bss_7824.modelId[0] = Hu3DModelCreate(HuDataSelHeapReadNum(0x440009, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_7824.modelId[1] = Hu3DModelCreate(HuDataSelHeapReadNum(0x44000A, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_7824.modelId[2] = Hu3DModelCreate(HuDataSelHeapReadNum(0x44000B, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_7824.modelId[3] = Hu3DModelCreate(HuDataSelHeapReadNum(0x44000C, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_7824.modelId[0], 1);
    Hu3DModelAttrSet(lbl_1_bss_7824.modelId[1], 1);
    Hu3DModelAttrSet(lbl_1_bss_7824.modelId[2], 1);
    Hu3DModelAttrSet(lbl_1_bss_7824.modelId[3], 1);
    lbl_1_bss_7824.motionId[0] = Hu3DJointMotion(lbl_1_bss_7824.modelId[0], HuDataSelHeapReadNum(0x44001A, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DMotionSet(lbl_1_bss_7824.modelId[0], lbl_1_bss_7824.motionId[0]);
    Hu3DModelAttrSet(lbl_1_bss_7824.modelId[0], 0x40000001);
    lbl_1_bss_7824.motionId[1] = Hu3DJointMotion(lbl_1_bss_7824.modelId[1], HuDataSelHeapReadNum(0x44001B, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DMotionSet(lbl_1_bss_7824.modelId[1], lbl_1_bss_7824.motionId[1]);
    Hu3DModelAttrSet(lbl_1_bss_7824.modelId[1], 0x40000001);
    lbl_1_bss_7824.motionId[2] = Hu3DJointMotion(lbl_1_bss_7824.modelId[2], HuDataSelHeapReadNum(0x44001C, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DMotionSet(lbl_1_bss_7824.modelId[2], lbl_1_bss_7824.motionId[2]);
    Hu3DModelAttrSet(lbl_1_bss_7824.modelId[2], 0x40000001);
    lbl_1_bss_7824.motionId[3] = Hu3DJointMotion(lbl_1_bss_7824.modelId[3], HuDataSelHeapReadNum(0x44001D, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DMotionSet(lbl_1_bss_7824.modelId[3], lbl_1_bss_7824.motionId[3]);
    Hu3DModelAttrSet(lbl_1_bss_7824.modelId[3], 0x40000001);

    for (i = 0; i < 16; i++) {
        END_FX_A *fx = &lbl_1_bss_7824.fxA[i];

        memset(fx, 0, sizeof(END_FX_A));
        id = Hu3DModelLink(lbl_1_bss_7824.modelId0);
        fx->modelId = id;
        Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(id, 10.0f, 10.0f, 10.0f);
        Hu3DModelCameraSet(id, cam->unk04 & 0xFFFF);
        Hu3DModelLayerSet(id, 4);
        Hu3DModelAttrSet(id, 1);
        fx->index = i;
    }
    for (i = 0; i < 64; i++) {
        END_FX_B *fx = &lbl_1_bss_7824.fxB[i];

        memset(fx, 0, sizeof(END_FX_B));
        if (i % 4 == 0) {
            id = Hu3DModelLink(lbl_1_bss_7824.modelId[0]);
            fx->modelId = id;
        } else if (i % 4 == 1) {
            id = Hu3DModelLink(lbl_1_bss_7824.modelId[1]);
            fx->modelId = id;
        } else if (i % 4 == 2) {
            id = Hu3DModelLink(lbl_1_bss_7824.modelId[2]);
            fx->modelId = id;
        } else {
            id = Hu3DModelLink(lbl_1_bss_7824.modelId[3]);
            fx->modelId = id;
        }
        Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(id, 10.0f, 10.0f, 10.0f);
        Hu3DModelCameraSet(id, cam->unk04 & 0xFFFF);
        Hu3DModelLayerSet(id, 4);
        Hu3DModelAttrSet(id, 1);
        fx->index = i;
    }
    lbl_1_data_480 = 1;

    id = Hu3DModelCreate(HuDataSelHeapReadNum(0x44000F, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_7824.resModelId[0] = id;
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(id, cam->unk04 & 0xFFFF);
    Hu3DModelLayerSet(id, 4);
    Hu3DModelAttrSet(id, 1);

    id = Hu3DModelCreate(HuDataSelHeapReadNum(0x440010, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_7824.resModelId[1] = id;
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(id, cam->unk04 & 0xFFFF);
    Hu3DModelLayerSet(id, 4);
    Hu3DModelAttrSet(id, 1);

    id = Hu3DModelCreate(HuDataSelHeapReadNum(0x440011, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_7824.resModelId[2] = id;
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(id, cam->unk04 & 0xFFFF);
    Hu3DModelLayerSet(id, 4);
    Hu3DModelAttrSet(id, 1);

    lbl_1_bss_7824.y1 = 0.0f;
    lbl_1_bss_7824.v1 = 0.0f;
    lbl_1_bss_7824.v2 = 0.0f;
    lbl_1_bss_7824.y2 = 0.0f;
    lbl_1_bss_7824.v3 = 0.0f;
    lbl_1_bss_7824.y3 = 0.0f;
    lbl_1_bss_7824.score = 0;
    lbl_1_bss_7820 = 0;
}

void fn_1_A454(M510_OBJ *data)
{
    Hu3DModelKill(lbl_1_bss_7824.resModelId[2]);
    Hu3DModelKill(lbl_1_bss_7824.resModelId[1]);
    Hu3DModelKill(lbl_1_bss_7824.resModelId[0]);
    Hu3DModelKill(lbl_1_bss_7824.modelId[0]);
    Hu3DModelKill(lbl_1_bss_7824.modelId0);
    Hu3DMotionKill(lbl_1_bss_7824.motionId[0]);
    Hu3DMotionKill(lbl_1_bss_7824.motionId0);
    memset(&lbl_1_bss_7824, 0, sizeof(END_WORK));
}

s32 fn_1_A4F4(M510_OBJ *data)
{
    M510_OPEN_CAM *cam = &data->cam;
    M510_PLAYER_MODEL_WORK *model;
    M510_SPECTATOR *spec;
    s32 winners[4];
    s32 ret;
    s32 allDone;
    s32 winCnt;
    s32 i;
    f32 t;
    f32 absRot;
    f32 rot;
    s16 playerNo;

    ret = 0;
    switch (data->state) {
    case 0:
        lbl_1_data_468.x = cam->tgt.x;
        lbl_1_data_468.y = cam->tgt.y;
        lbl_1_data_468.z = cam->tgt.z;
        lbl_1_data_474.x = cam->ofs.x;
        lbl_1_data_474.y = cam->ofs.y;
        lbl_1_data_474.z = cam->ofs.z;
        cam->tgt.x = lbl_1_data_450.x;
        cam->tgt.y = lbl_1_data_450.y;
        cam->tgt.z = lbl_1_data_450.z;
        cam->tgt.x = lbl_1_data_468.x;
        cam->ofs.x = lbl_1_data_45C.x;
        cam->ofs.y = lbl_1_data_45C.y;
        cam->ofs.z = lbl_1_data_45C.z;
        cam->ofs.x = lbl_1_data_474.x;
        cam->speed = 0.05f;
        for (i = 0; i < 4; i++) {
            model = &data->players[i].model;
            Hu3DModelHookReset(model->modelId);
            Hu3DModelObjMtxGet(model->modelId, CharModelItemHookGet(model->charNo, 1, 1), Hu3DData[model->itemModelId].mtx);
            Hu3DModelObjPosGet(model->modelId, CharModelItemHookGet(model->charNo, 1, 1), &model->itemPos);
            Hu3DData[model->itemModelId].mtx[2][3] = 0.0f;
            Hu3DData[model->itemModelId].mtx[1][3] = 0.0f;
            Hu3DData[model->itemModelId].mtx[0][3] = 0.0f;
            Hu3DModelPosSet(model->itemModelId, model->itemPos.x, model->itemPos.y, model->itemPos.z);
            Hu3DMotionShiftSet(model->modelId, model->obj->mtnId[1], 0.0f, 4.0f, 0x40000001);
            model->state = 0;
        }
        data->timer1 = 0;
        data->state = 1;
        break;
    case 1:
        allDone = 1;
        t = (f32)data->timer1 / 60.0f;
        if (t > 1.0f) {
            t = 1.0f;
        } else if (t < 0.0f) {
            t = 0.0f;
        }
        for (i = 0; i < 4; i++) {
            model = &data->players[i].model;
            absRot = (model->camRot.y >= 0.0f) ? model->camRot.y : -model->camRot.y;
            if (absRot > 3.0f) {
                model->camRot.y = WRAP180(fn_1_2840(0.0f, WRAP180(model->camRot.y), 0.9f));
                omSetRot(model->obj, 0.0f, model->camRot.y, 0.0f);
                allDone = 0;
            } else if (model->state == 0) {
                Hu3DMotionShiftSet(model->modelId, model->obj->mtnId[0], 0.0f, 4.0f, 0x40000001);
                model->state = 1;
                allDone = 0;
            }
            Hu3DModelRotSet(model->itemModelId, 2.0f * (360.0f * t), 0.0f, 0.0f);
            Hu3DModelPosSet(model->itemModelId, model->itemPos.x, model->itemPos.y + (400.0f * t), model->itemPos.z - (300.0f * t));
            Hu3DModelScaleSet(model->itemModelId, 1.0f - t, 1.0f - t, 1.0f - t);
        }
        data->timer1++;
        if (allDone != 0 && data->timer1 > 60) {
            for (i = 0; i < 4; i++) {
                Hu3DModelAttrSet(data->players[i].model.itemModelId, 1);
            }
            data->timer1 = 0;
            data->state = 2;
        }
        break;
    case 2:
        if (fn_1_B350(data) != 0) {
            data->mesId = GameMesCreate(14, data->record);
            data->timer1 = 0;
            data->state = 3;
            fn_1_75C(0x86);
            if (lbl_1_bss_0->practice == 0) {
                data->score.value = (s16)data->record;
                data->score.dispFlag = 1;
            }
        } else {
            data->timer1 = 0;
            data->state = 5;
        }
        break;
    case 3:
        if (data->mesId >= 0 && GameMesStatGet(data->mesId) == 0) {
            data->mesId = -1;
            data->timer1 = 0;
            data->state = 4;
        }
        break;
    case 4:
        if (data->timer1++ > 30) {
            data->timer1 = 0;
            data->state = 5;
        }
        break;
    case 5:
        winCnt = 0;
        winners[3] = -1;
        winners[2] = -1;
        winners[1] = -1;
        winners[0] = -1;
        for (i = 0; i < 4; i++) {
            model = &data->players[i].model;
            if (model->winner != 0) {
                Hu3DMotionShiftSet(model->modelId, model->obj->mtnId[5], 0.0f, 4.0f, 0);
                winners[winCnt] = model->charNo;
                playerNo = model->playerNo;
                if (_CheckFlag(0x1000F) == 0) {
                    GwPlayer[playerNo].mgCoinBonus = 10;
                }
                winCnt++;
            } else {
                Hu3DMotionShiftSet(model->modelId, model->obj->mtnId[6], 0.0f, 4.0f, 0);
            }
        }
        if (winCnt != 0) {
            data->mesId = GameMesCreate(4, 3, winners[0], winners[1], winners[2], winners[3]);
            lbl_1_bss_7824.active = 1;
            fn_1_5B4(0x69F);
            fn_1_5B4(0x398);
            for (i = 0; i < 16; i++) {
                spec = &data->stage.spec[i];
                Hu3DModelAttrSet(spec->modelId, 0x40000001);
                Hu3DMotionSet(spec->modelId, spec->motion[1]);
            }
            if (data->soundId < 0) {
                data->soundId = fn_1_75C(0x79);
            }
        } else {
            data->mesId = GameMesCreate(2, 2);
            if (data->soundId < 0) {
                data->soundId = fn_1_75C(0x7B);
            }
            CharLoseVoicePlay(data->players[0].model.charNo, data->players[1].model.charNo, data->players[2].model.charNo, data->players[3].model.charNo);
        }
        data->timer1 = 0;
        data->state = 6;
        break;
    case 6:
        data->timer1++;
        if (data->timer1 >= 209) {
            data->mesId = -1;
            data->timer1 = 0;
            data->state = 7;
        }
        break;
    default:
        ret = 1;
        break;
    }

    if (data->state >= 5 && lbl_1_bss_7824.score >= 30) {
        if (lbl_1_bss_7824.y1 <= 0.0f) {
            Hu3DModelAttrReset(lbl_1_bss_7824.resModelId[0], 1);
            Hu3DModelAttrReset(lbl_1_bss_7824.resModelId[1], 1);
            Hu3DModelAttrReset(lbl_1_bss_7824.resModelId[2], 1);
        }
        lbl_1_bss_7820++;
        if (lbl_1_bss_7820 == 60) {
            fn_1_5B4(0x6A0);
        }
        lbl_1_bss_7824.v1 = lbl_1_bss_7824.v1 + 0.02f;
        lbl_1_bss_7824.y1 = lbl_1_bss_7824.y1 + lbl_1_bss_7824.v1;
        Hu3DModelPosSet(lbl_1_bss_7824.resModelId[0], 380.0f, lbl_1_bss_7824.y1 - 200.0f, 0.0f);
        lbl_1_bss_7824.v2 = lbl_1_bss_7824.v2 + 0.03f;
        lbl_1_bss_7824.y2 = lbl_1_bss_7824.y2 + lbl_1_bss_7824.v2;
        Hu3DModelPosSet(lbl_1_bss_7824.resModelId[1], 380.0f, lbl_1_bss_7824.y2 - 200.0f, 0.0f);
        lbl_1_bss_7824.v3 = lbl_1_bss_7824.v3 + 0.04f;
        lbl_1_bss_7824.y3 = lbl_1_bss_7824.y3 + lbl_1_bss_7824.v3;
        Hu3DModelPosSet(lbl_1_bss_7824.resModelId[2], 380.0f, lbl_1_bss_7824.y3 - 200.0f, 0.0f);
    }
    if (lbl_1_bss_7824.active != 0) {
        fn_1_B4A4(data);
        lbl_1_bss_7824.frame += 1;
    }
    return ret;
}

s32 fn_1_B350(M510_OBJ *data)
{
    s32 ret = 0;
    s16 best = 0;
    s32 humanWon = 0;
    s32 i;
    M510_PLAYER_WORK *player;
    M510_PLAYER_MODEL_WORK *model;
    M510_SCORE_WORK *score;

    for (i = 0; i < 4; i++) {
        player = &data->players[i];
        score = &player->score;
        if (best < score->value) {
            best = score->value;
        }
        player->model.winner = 0;
    }
    for (i = 0; i < 4; i++) {
        player = &data->players[i];
        model = &player->model;
        score = &player->score;
        if (best == score->value && best > 0) {
            model->winner = 1;
            if (model->pad.ai.type == 0) {
                humanWon = 1;
            }
        }
    }
    if (best > data->record) {
        data->record = best;
        if (humanWon != 0) {
            if (_CheckFlag(0x1000F) != 0) {
                data->score.value = (s16)data->record;
                data->score.dispFlag = 1;
                ret = 0;
            } else {
                if (lbl_1_bss_0->practice == 0) {
                    GWMgRecordSet(1, data->record);
                }
                ret = 1;
            }
        }
    }
    lbl_1_bss_7824.score = best;
    return ret;
}

void fn_1_B4A4(M510_OBJ *data)
{
    M510_VECF a;
    M510_VECF b;
    M510_VECF c;
    M510_VECF pt;
    f32 t;
    f32 r;
    s32 i;

    if (lbl_1_bss_7824.frame % 30 == 0) {
        fn_1_B8F4(data);
        fn_1_B8F4(data);
    }
    if (lbl_1_data_480 != 0) {
        lbl_1_data_480 = 0;
        for (i = 0; i < 32; i++) {
            t = 0.03125f * (f32)i;
            PSVECSubtract(&lbl_1_data_4B0, &lbl_1_data_4A4, &a);
            PSVECScale(&a, &a, t);
            PSVECAdd(&a, &lbl_1_data_4A4, &a);
            PSVECSubtract(&lbl_1_data_4C8, &lbl_1_data_4BC, &b);
            PSVECScale(&b, &b, t);
            PSVECAdd(&b, &lbl_1_data_4BC, &b);
            r = 0.2f + (0.6f * ((f32)(frand() % 10000) / 10000.0f));
            PSVECSubtract(&b, &a, &c);
            PSVECScale(&c, &c, r);
            PSVECAdd(&c, &a, &c);
            pt.x = c.x;
            pt.y = c.y;
            pt.z = c.z;
            pt.x = 380.0f + c.x;
            fn_1_BE48(data, &pt);
        }
        for (i = 0; i < 32; i++) {
            t = 0.03125f * (f32)i;
            PSVECSubtract(&lbl_1_data_4B0, &lbl_1_data_4A4, &a);
            PSVECScale(&a, &a, t);
            PSVECAdd(&a, &lbl_1_data_4A4, &a);
            PSVECSubtract(&lbl_1_data_4C8, &lbl_1_data_4BC, &b);
            PSVECScale(&b, &b, t);
            PSVECAdd(&b, &lbl_1_data_4BC, &b);
            r = 0.2f + (0.6f * ((f32)(frand() % 10000) / 10000.0f));
            PSVECSubtract(&b, &a, &c);
            PSVECScale(&c, &c, r);
            PSVECAdd(&c, &a, &c);
            pt.x = c.x;
            pt.y = c.y;
            pt.z = c.z;
            pt.x = 380.0f + -c.x;
            fn_1_BE48(data, &pt);
        }
    }
    for (i = 0; i < 16; i++) {
        fn_1_B958(&lbl_1_bss_7824.fxA[i]);
    }
    for (i = 0; i < 64; i++) {
        fn_1_BEC4(&lbl_1_bss_7824.fxB[i]);
    }
}

void fn_1_B8F4(M510_OBJ *data)
{
    s32 i;
    END_FX_A *fx;

    for (i = 0; i < 16; i++) {
        fx = &lbl_1_bss_7824.fxA[i];
        if (fx->active == 0) {
            fx->state = 0;
            fx->active = 1;
            return;
        }
    }
}

void fn_1_B958(void *data)
{
    END_FX_A *fx = data;
    f32 t;
    f32 s;
    f32 sc;

    if (fx->active != 0) {
        switch (fx->state) {
        case 0:
            fx->x = 380.0f + (450.0f * ((f32)(fx->index % 3) - 1.0f));
            fx->x = fx->x + (150.0f * (((f32)(frand() % 10000) / 10000.0f) - 0.5f));
            fx->y = 1500.0f;
            fx->z = 3100.0f + (((fx->index & 1) != 0) ? 300.0f : -300.0f);
            Hu3DModelRotSet(fx->modelId, 0.0f, ((fx->index & 1) != 0) ? 0.0f : 180.0f, 0.0f);
            Hu3DModelScaleSet(fx->modelId, 0.2f, 0.2f, 0.2f);
            Hu3DModelAttrReset(fx->modelId, 1);
            fx->timer = 0;
            fx->state++;
            break;
        case 1:
            t = (f32)((lbl_1_bss_7824.frame + ((fx->index % 4) * 30)) % 90) / 90.0f;
            fx->y = (f32)((f64)fx->y - (6.0 + (5.0 * sin(M_PI_D * (f64)(180.0f * t) / 180.0))));
            fx->x = (f32)((f64)fx->x + (1.5 * sin(M_PI_D * (f64)(360.0f * t) / 180.0)));
            s = (f32)fx->timer / 180.0f;
            if (s > 1.0f) {
                s = 1.0f;
            } else if (s < 0.0f) {
                s = 0.0f;
            }
            sc = 14.0f * (0.2f + (0.8f * s));
            Hu3DModelScaleSet(fx->modelId, sc, sc, sc);
            fx->timer++;
            if (fx->y < -300.0f) {
                Hu3DModelAttrSet(fx->modelId, 1);
                fx->active = 0;
                fx->state = 0;
            }
            break;
        }
        Hu3DModelPosSet(fx->modelId, fx->x, fx->y, fx->z);
    }
}

void fn_1_BE48(M510_OBJ *data, M510_VECF *src)
{
    s32 i;
    END_FX_B *fx;

    for (i = 0; i < 64; i++) {
        fx = &lbl_1_bss_7824.fxB[i];
        if (fx->active == 0) {
            fx->state = 0;
            fx->active = 1;
            *(M510_VECF *)&fx->x = *src;
            return;
        }
    }
}

void fn_1_BEC4(void *data)
{
    END_FX_B *fx = data;
    f32 t;
    f32 s;
    f32 sc;

    if (fx->active != 0) {
        switch (fx->state) {
        case 0:
            fx->angle = 90.0f * (((f32)(frand() % 10000) / 10000.0f) - 0.5f);
            fx->angle = fx->angle + ((fx->x < 380.0f) ? 20.0f : -20.0f);
            Hu3DModelRotSet(fx->modelId, 0.0f, fx->angle - 90.0f, 0.0f);
            Hu3DModelScaleSet(fx->modelId, 0.0f, 0.0f, 0.0f);
            Hu3DModelAttrReset(fx->modelId, 1);
            fx->speed = 5.0f + (5.0f * ((f32)(frand() % 10000) / 10000.0f));
            if ((s32)(frand() % 10000) < 3000) {
                Hu3DMotionSpeedSet(fx->modelId, 1.5f + (2.5f * ((f32)(frand() % 10000) / 10000.0f)));
            } else {
                Hu3DMotionSpeedSet(fx->modelId, 0.3f + (0.7f * ((f32)(frand() % 10000) / 10000.0f)));
            }
            fx->timer = 0;
            fx->state++;
            break;
        case 1:
            t = (f32)fx->timer / 30.0f;
            if (t > 1.0f) {
                t = 1.0f;
            } else if (t < 0.0f) {
                t = 0.0f;
            }
            sc = (7.0f + (f32)(fx->index % 5)) * (0.2f + (0.8f * t));
            Hu3DModelScaleSet(fx->modelId, sc, sc, sc);
            fx->x = (f32)((f64)fx->x + ((f64)t * ((f64)fx->speed * sin(M_PI_D * (f64)fx->angle / 180.0))));
            fx->z = (f32)((f64)fx->z + (3.0 + ((f64)t * ((f64)fx->speed * cos(M_PI_D * (f64)fx->angle / 180.0)))));
            s = (f32)fx->timer / 90.0f;
            if (s > 1.0f) {
                s = 1.0f;
            } else if (s < 0.0f) {
                s = 0.0f;
            }
            fx->y = (f32)((f64)fx->y + (1.0 + ((f64)(4.0f + (2.0f * (f32)(fx->index % 3))) * sin(M_PI_D * (f64)(90.0f * s) / 180.0))));
            fx->timer++;
            if (fx->y > 1200.0f) {
                Hu3DModelAttrSet(fx->modelId, 1);
                fx->active = 0;
                fx->state = 0;
            }
            break;
        }
        Hu3DModelPosSet(fx->modelId, fx->x, fx->y, fx->z);
    }
}
