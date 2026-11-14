#include "REL/m562dll.h"
#include "game/frand.h"
#include "datadir_enum.h"
#include <math.h>

float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

/* data of this unit, in binary order */
f32 lbl_1_data_C8[16] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
Vec lbl_1_data_108 = { 0.0f, -3300.0f, -26000.0f };

void fn_1_5688(M562StageField *field, M562StageParent *parent)
{
    M562StageCam *cam = &parent->cam;
    s32 i;
    s16 x, z;
    s16 mdl;

    memset(field, 0, sizeof(M562StageField));
    field->parent = parent;
    field->soundId = -1;

    field->modelA = Hu3DModelCreateData(DATANUM(DATA_m562, 0));
    Hu3DModelAttrSet(field->modelA, 1);
    field->modelB = Hu3DModelCreateData(DATANUM(DATA_m562, 1));
    Hu3DModelAttrSet(field->modelB, 1);
    field->modelC = Hu3DModelCreateData(DATANUM(DATA_m562, 2));
    Hu3DModelAttrSet(field->modelC, 1);

    mdl = Hu3DModelCreateData(DATANUM(DATA_m562, 5));
    field->modelD = mdl;
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(mdl, (u16) cam->cameraBit);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelShadowMapSet(mdl);
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrSet(mdl, 1);

    mdl = Hu3DModelCreateData(DATANUM(DATA_m562, 6));
    field->modelE = mdl;
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(mdl, (u16) cam->cameraBit);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelShadowMapSet(mdl);
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrSet(mdl, 1);
    field->motionE = Hu3DJointMotionData(mdl, DATANUM(DATA_m562, 0x2A));
    Hu3DMotionSet(mdl, field->motionE);
    Hu3DMotionSpeedSet(mdl, 0.0f);
    Hu3DModelAttrReset(mdl, 0x40000001);

    mdl = Hu3DModelCreateData(DATANUM(DATA_m562, 7));
    field->modelF = mdl;
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(mdl, (u16) cam->cameraBit);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelShadowMapSet(mdl);
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrSet(mdl, 1);

    mdl = Hu3DModelCreateData(DATANUM(DATA_m562, 3));
    field->modelG = mdl;
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(mdl, (u16) cam->cameraBit);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelAttrSet(mdl, 1);
    Hu3DMotionSpeedSet(mdl, 0.4f);
    field->motionG = Hu3DJointMotionData(mdl, DATANUM(DATA_m562, 0x28));
    Hu3DMotionSet(mdl, field->motionG);
    Hu3DModelAttrReset(mdl, 0x40000001);
    field->gActive = 0;

    mdl = Hu3DModelCreateData(DATANUM(DATA_m562, 4));
    field->modelH = mdl;
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(mdl, (u16) cam->cameraBit);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelAttrSet(mdl, 1);
    field->motionH = Hu3DJointMotionData(mdl, DATANUM(DATA_m562, 0x29));
    Hu3DMotionSet(mdl, field->motionH);
    Hu3DModelAttrReset(mdl, 0x40000001);

    i = 0;
    for (z = 0; z < 4; z++) {
        for (x = 0; x < 5; x++) {
            fn_1_60F8(&field->tile[i], x, z, field);
            i++;
        }
    }
}

void fn_1_5BE4(M562StageField *field)
{
    s32 i;

    for (i = 0; i < 20; i++) {
        fn_1_62A8(&field->tile[i]);
    }
    Hu3DMotionKill(field->motionH);
    Hu3DModelKill(field->modelH);
    Hu3DMotionKill(field->motionG);
    Hu3DModelKill(field->modelG);
    Hu3DModelKill(field->modelF);
    Hu3DMotionKill(field->motionE);
    Hu3DModelKill(field->modelE);
    Hu3DModelKill(field->modelD);
    Hu3DModelKill(field->modelC);
    Hu3DModelKill(field->modelB);
    Hu3DModelKill(field->modelA);
    memset(field, 0, sizeof(M562StageField));
}

void fn_1_5CAC(M562StageField *field)
{
    M562StageParent *parent = field->parent;
    s32 i;

    switch (field->state) {
        case 0:
            if (parent->phase != 4) {
                if (parent->phase == 3) {
                    for (i = 0; i < 20; i++) {
                        M562StageTile *tile = &field->tile[i];
                        fn_1_62E8(tile);
                        if (field->quakeTimer != 0) {
                            f32 t = (f32) field->quakeTimer / 120.0f;
                            f32 inner = 2000.0f * t;
                            f32 d;
                            f32 c;
                            f32 k;
                            Vec diff;

                            PSVECSubtract(&tile->pos, &field->quakePos, &diff);
                            diff.z = 0.0f;
                            d = (PSVECMag(&diff) - inner) / ((400.0f + inner) - inner);
                            if (d > 1.0f) {
                                c = 1.0f;
                            } else {
                                if (d < 0.0f) {
                                    k = 0.0f;
                                } else {
                                    k = d;
                                }
                                c = k;
                            }
                            tile->pos.y = (1.0f - t) * (60.0 * sin(3.141592653589793 * (180.0f * c) / 180.0));
                        }
                    }
                    if (field->quakeTimer != 0) {
                        if (field->quakeTimer < 80) {
                            if (field->soundId < 0) {
                                field->soundId = fn_1_814(0x8A8);
                            }
                        } else if (field->soundId >= 0) {
                            fn_1_83C(field->soundId);
                            field->soundId = -1;
                        }
                        field->quakeTimer++;
                        if (field->quakeTimer >= 120) {
                            field->quakeTimer = 0;
                        }
                    }
                }
            } else {
                for (i = 0; i < 20; i++) {
                    Hu3DModelAttrSet(field->tile[i].modelId, 1);
                }
                Hu3DModelAttrReset(field->modelD, 1);
                Hu3DModelAttrReset(field->modelE, 1);
                Hu3DModelAttrReset(field->modelF, 1);
                Hu3DModelAttrSet(field->modelG, 1);
                Hu3DModelAttrSet(field->modelH, 1);
                field->state = 1;
            }
            break;
        case 1:
            if (parent->phase != 4) {
                for (i = 0; i < 20; i++) {
                    Hu3DModelAttrReset(field->tile[i].modelId, 1);
                }
                Hu3DModelAttrSet(field->modelD, 1);
                Hu3DModelAttrSet(field->modelE, 1);
                Hu3DModelAttrSet(field->modelF, 1);
                field->state = 0;
            }
            break;
    }
    if (field->gActive != 0) {
        f32 maxTime = Hu3DMotionMaxTimeGet(field->modelG);
        if (Hu3DMotionTimeGet(field->modelG) >= maxTime) {
            Hu3DModelAttrSet(field->modelG, 1);
            field->gActive = 0;
        }
    }
    if (field->hActive != 0) {
        f32 maxTime = Hu3DMotionMaxTimeGet(field->modelH);
        if (Hu3DMotionTimeGet(field->modelH) >= maxTime) {
            Hu3DModelAttrSet(field->modelH, 1);
            field->hActive = 0;
        }
    }
}

void fn_1_60F8(M562StageTile *tile, s16 x, s16 z, M562StageField *field)
{
    M562StageCam *cam = &field->parent->cam;
    s16 mdl;
    f32 px, py, pz;

    memset(tile, 0, sizeof(M562StageTile));
    tile->field = field;
    tile->x = x;
    tile->z = z;
    mdl = Hu3DModelLink(field->modelA);
    tile->modelId = mdl;
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(mdl, (u16) cam->cameraBit);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelAttrReset(mdl, 1);
    Hu3DModelAttrSet(mdl, 0x1000);
    Hu3DModelShadowMapSet(mdl);
    px = -500.0f + 250.0f * (f32) x;
    py = 0.0f;
    pz = -375.0f + 250.0f * (f32) z;
    Hu3DModelPosSet(mdl, px, py, pz);
    tile->pos.x = px;
    tile->pos.y = py;
    tile->pos.z = pz;
}

void fn_1_62A8(M562StageTile *tile)
{
    Hu3DModelKill(tile->modelId);
    memset(tile, 0, sizeof(M562StageTile));
}

/* tile state: 0..2 swap the model when flagged, 10/11 fall, 20/21 fly back in */
void fn_1_62E8(M562StageTile *tile)
{
    M562StageField *field = tile->field;
    M562StageParent *parent = field->parent;
    M562StageCam *cam = &parent->cam;
    M562StageAmb *amb = &parent->amb;
    f32 r;
    f32 mag, d, c, k;
    Vec sp8;
    s16 mdl;

    switch (tile->state) {
        case 0:
            if (tile->flag != 0) {
                Hu3DModelShadowMapObjReset(tile->modelId);
                Hu3DModelKill(tile->modelId);
                mdl = Hu3DModelLink(field->modelB);
                tile->modelId = mdl;
                Hu3DModelAmbSet(tile->modelId, amb->r, amb->g, amb->b);
                Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
                Hu3DModelCameraSet(mdl, (u16) cam->cameraBit);
                Hu3DModelLayerSet(mdl, 0);
                Hu3DModelAttrReset(mdl, 1);
                Hu3DModelAttrSet(mdl, 0x1000);
                Hu3DModelShadowMapSet(mdl);
                Hu3DModelPosSet(mdl, tile->pos.x, tile->pos.y, tile->pos.z);
                Hu3DMotionTimeSet(field->modelG, 0.0f);
                Hu3DModelPosSet(field->modelG, tile->pos.x, tile->pos.y, tile->pos.z);
                Hu3DModelAttrReset(field->modelG, 1);
                field->gActive = 1;
                tile->flag = 0;
                tile->state++;
            }
            break;
        case 1:
            if (tile->flag != 0) {
                Hu3DModelShadowMapObjReset(tile->modelId);
                Hu3DModelKill(tile->modelId);
                mdl = Hu3DModelLink(field->modelC);
                tile->modelId = mdl;
                Hu3DModelAmbSet(tile->modelId, amb->r, amb->g, amb->b);
                Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
                Hu3DModelCameraSet(mdl, (u16) cam->cameraBit);
                Hu3DModelLayerSet(mdl, 0);
                Hu3DModelAttrReset(mdl, 1);
                Hu3DModelAttrSet(mdl, 0x1000);
                Hu3DModelShadowMapSet(mdl);
                Hu3DModelPosSet(mdl, tile->pos.x, tile->pos.y, tile->pos.z);
                Hu3DMotionTimeSet(field->modelG, 0.0f);
                Hu3DModelPosSet(field->modelG, tile->pos.x, tile->pos.y, tile->pos.z);
                Hu3DModelAttrReset(field->modelG, 1);
                field->gActive = 1;
                tile->flag = 0;
                tile->state++;
            }
            break;
        case 2:
            if (tile->flag != 0) {
                Hu3DModelShadowMapObjReset(tile->modelId);
                Hu3DModelKill(tile->modelId);
                mdl = Hu3DModelLink(field->modelA);
                tile->modelId = mdl;
                Hu3DModelAmbSet(tile->modelId, amb->r, amb->g, amb->b);
                Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
                Hu3DModelCameraSet(mdl, (u16) cam->cameraBit);
                Hu3DModelLayerSet(mdl, 0);
                Hu3DModelAttrSet(mdl, 0x1000);
                Hu3DModelShadowMapSet(mdl);
                Hu3DModelPosSet(mdl, tile->pos.x, tile->pos.y, tile->pos.z);
                Hu3DMotionTimeSet(field->modelH, 0.0f);
                Hu3DModelPosSet(field->modelH, tile->pos.x, tile->pos.y, tile->pos.z);
                Hu3DModelAttrReset(field->modelH, 1);
                field->hActive = 1;
                Hu3DModelAttrSet(mdl, 1);
                tile->flag = 0;
                tile->state = 3;
            }
            break;
        case 10:
            tile->vy = 0.0f;
            r = (f32) (frand() % 10000);
            tile->rotVel.x = 3.0f * (r / 10000.0f);
            r = (f32) (frand() % 10000);
            tile->rotVel.y = 3.0f * (r / 10000.0f);
            r = (f32) (frand() % 10000);
            tile->rotVel.z = 3.0f * (r / 10000.0f);
            tile->state++;
            /* fallthrough */
        case 11:
            tile->vy -= 1.5f;
            tile->pos.y += tile->vy;
            if (tile->pos.y < -1000.0f) {
                tile->vy = 0.0f;
                tile->state = 20;
            }
            break;
        case 20:
            tile->pos.y = -5000.0f;
            r = (f32) (frand() % 10000);
            tile->vel.x = 200.0f * (r / 10000.0f - 0.5f);
            r = (f32) (frand() % 10000);
            tile->vel.y = 50.0f * (r / 10000.0f - 0.5f);
            tile->vel.z = 0.0f;
            tile->state++;
            /* fallthrough */
        case 21:
            PSVECSubtract(&lbl_1_data_108, &tile->pos, &sp8);
            mag = PSVECMag(&sp8);
            d = mag / 20000.0f;
            if (d > 1.0f) {
                c = 1.0f;
            } else {
                if (d < 0.0f) {
                    k = 0.0f;
                } else {
                    k = d;
                }
                c = k;
            }
            d = 1.0f - c;
            d = d * d;
            d = 0.1f + 0.9f * d;
            PSVECNormalize(&sp8, &sp8);
            PSVECScale(&sp8, &sp8, 100.0f);
            tile->vel.x += sp8.x * d;
            tile->vel.y += sp8.y * d;
            tile->vel.z += 0.2f * (sp8.z * d);
            PSVECAdd(&tile->pos, &tile->vel, &tile->pos);
            if (mag < 1000.0f || tile->pos.z < lbl_1_data_108.z) {
                Hu3DModelAttrSet(tile->modelId, 1);
                tile->state = 30;
            }
            break;
    }
    tile->rot.x += tile->rotVel.x;
    tile->rot.y += tile->rotVel.y;
    tile->rot.z += tile->rotVel.z;
    Hu3DModelPosSet(tile->modelId, tile->pos.x, tile->pos.y, tile->pos.z);
    Hu3DModelRotSet(tile->modelId, tile->rot.x, tile->rot.y, tile->rot.z);
}
