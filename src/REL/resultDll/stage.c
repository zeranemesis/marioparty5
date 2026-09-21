#include "REL/resultDll.h"

#ifndef __MWERKS__
#include "game/esprite.h"
#endif

void StageModelCreate(STAGE_MODEL *model)
{
    STAGE_MODEL *modelP;
    HU3DMODELID mdlId;
    s16 i;
    s16 j;
    void *data;
    HuVecF pos;

    modelP = model;
    for(j=0; j<STAGE_MDL_MAX; j++) {
        StageMdlId[j] = StageMotId[j] = HU3D_MODELID_NONE;
    }
    for(i=0; modelP->dataNum != HU_DATANUM_NONE; modelP++, i++) {
        if(modelP->type == STAGE_MDL_TYPE_MODEL) {
            data = HuDataSelHeapReadNum(modelP->dataNum, HU_MEMNUM_OVL, HEAP_MODEL);
            mdlId = Hu3DModelCreate(data);
            StageMdlId[i] = mdlId;
            Hu3DModelCameraSet(mdlId, modelP->cameraBit);
            Hu3D2Dto3D(&modelP->pos, HU3D_CAM1, &pos);
            Hu3DModelAttrSet(mdlId, modelP->attr);
            Hu3DModelPosSetV(mdlId, &pos);
            Hu3DModelRotSetV(mdlId, &modelP->rot);
            Hu3DModelScaleSetV(mdlId, &modelP->scale);
            Hu3DModelLayerSet(mdlId, modelP->layer);
        } else if(modelP->type == STAGE_MDL_TYPE_MOTION) {
            data = HuDataSelHeapReadNum(modelP->dataNum, HU_MEMNUM_OVL, HEAP_MODEL);
            StageMotId[i] = Hu3DJointMotion(StageMdlId[modelP->motMdl], data);
        }
    }
    modelP = model;
    for(i=0; modelP->dataNum != HU_DATANUM_NONE; modelP++, i++) {
        if(modelP->type == STAGE_MDL_TYPE_LINK) {
            mdlId = Hu3DModelLink(StageMdlId[modelP->linkMdl]);
            StageMdlId[i] = mdlId;
            Hu3DModelCameraSet(mdlId, modelP->cameraBit);
            Hu3D2Dto3D(&modelP->pos, HU3D_CAM1, &pos);
            Hu3DModelAttrSet(mdlId, modelP->attr);
            Hu3DModelPosSetV(mdlId, &pos);
            Hu3DModelRotSetV(mdlId, &modelP->rot);
            Hu3DModelScaleSetV(mdlId, &modelP->scale);
            Hu3DModelLayerSet(mdlId, modelP->layer);
        }
    }
}

void StageSpriteCreate(STAGE_SPRITE *sprite)
{
    STAGE_SPRITE *sprP = sprite;
    HUSPRID sprId;
    s16 i;
    s16 j;
    for(i=0; i<STAGE_SPR_MAX; i++) {
        StageSprId[i] = HUSPR_NONE;
    }
    for(j=0; sprP->dataNum != 0; j++, sprP++) {
        sprId = espEntry(sprP->dataNum, 100, 0);
        StageSprId[j] = sprId;
        espPosSet(sprId, sprP->posX, sprP->posY);
        espColorSet(sprId, sprP->color.r, sprP->color.g, sprP->color.b);
        espTPLvlSet(sprId, sprP->color.a/255.0f);
        espPriSet(sprId, sprP->pri);
        espAttrSet(sprId, sprP->attr);
        espDrawNoSet(sprId, 64);
        HuSprExecLayerCameraSet(64, HU3D_CAM1, 1);
    }
}

void StageSpriteKill(void)
{
    s16 i;
    for(i=0; i<STAGE_SPR_MAX; i++) {
        if(StageSprId[i] != HUSPR_NONE) {
            espKill(StageSprId[i]);
        }
    }
}
