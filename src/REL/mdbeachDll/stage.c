#include "REL/mdbeachDll.h"
#include "datadir_enum.h"
#include "port/widescreen.h"

static void FontBGFilter(HU3DMODEL *modelP, Mtx *mtx);
static void Layer2Hook(s16 layerNo);
static void Layer1Hook(s16 layerNo);

static unsigned int SpriteAnimFile[MDSPR_ANIM_MAX] = {
    DATANUM(DATA_mdbeach, 0x4C),
    DATANUM(DATA_mdbeach, 0x4D),
    DATANUM(DATA_mdbeach, 0x4A),
    DATANUM(DATA_mdbeach, 0x4B),
    DATANUM(DATA_mdbeach, 0x59),
    DATANUM(DATA_mdbeach, 0x5A),
    DATANUM(DATA_mdbeach, 0x5B),
    DATANUM(DATA_mdbeach, 0x4E),
    DATANUM(DATA_mdbeach, 0x4F),
    DATANUM(DATA_mdbeach, 0x50),
    DATANUM(DATA_mdbeach, 0x51),
    DATANUM(DATA_mdbeach, 0x52),
    DATANUM(DATA_mdbeach, 0x57),
    DATANUM(DATA_mdbeach, 0x58),
    DATANUM(DATA_mdbeach, 0x53),
    DATANUM(DATA_mdbeach, 0x54),
    DATANUM(DATA_mdbeach, 0x55),
    DATANUM(DATA_mdbeach, 0x56),
};

static MDSPR_INFO SpriteInfo[MDSPR_MAX] = {
    { 0, 5, { 0, -28 }, { 72, 1 }, 1 },
    { 0, 5, { 0, 32 }, { 72, 1 }, 1 },
    { 1, 6, { 0, 0 }, { 1, 8 }, 0.6f },
    { 2, 14, { 64, 0 }, { 1, 1 }, 1 },
    { 2, 14, { -64, 0 }, { 1, 1 }, 1 },
    { 2, 8, { 64, 0 }, { 1, 1 }, 1 },
    { 2, 8, { -64, 0 }, { 1, 1 }, 1 },
    { 2, 8, { 64, 0 }, { 1, 1 }, 1 },
    { 2, 8, { -64, 0 }, { 1, 1 }, 1 },
    { 2, 7, { 64, 0 }, { 1, 1 }, 1 },
    { 2, 7, { -64, 0 }, { 1, 1 }, 1 },
    { 2, 7, { 64, 0 }, { 1, 1 }, 1 },
    { 2, 7, { -64, 0 }, { 1, 1 }, 1 },
    { 4, 9, { 0, 0 }, { 1, 1 }, 1 },
    { 4, 9, { 0, 0 }, { 1, 1 }, 1 },
    { 4, 9, { 0, 0 }, { 1, 1 }, 1 },
    { 4, 9, { 0, 0 }, { 1, 1 }, 1 },
    { 5, 10, { 0, 0 }, { 1, 1 }, 1 },
    { 5, 10, { 0, 0 }, { 1, 1 }, 1 },
    { 5, 10, { 0, 0 }, { 1, 1 }, 1 },
    { 5, 10, { 0, 0 }, { 1, 1 }, 1 },
    { 9, 16, { 0, 0 }, { 0.9f, 0.93333334f }, 1 },
    { 10, 16, { 0, 0 }, { 0.9f, 0.93333334f }, 1 },
    { 11, 15, { 0, 0 }, { 72, 60 }, 0 },
    { 12, 11, { 0, 0 }, { 1, 1 }, 1 },
    { 13, 11, { 0, 0 }, { 1, 1 }, 1 },
    { 14, 13, { 0, 0 }, { 1, 1 }, 1 },
    { 16, 12, { 3, 10 }, { 1, 1 }, 1 },
    { 15, 13, { 0, 0 }, { 1, 1 }, 1 },
    { 17, 12, { -3, -3 }, { 1, 1 }, 1 },
};

static MDSPR_GRP_INFO SpriteGrpInfo[MDSPR_GRP_MAX] = {
    { 0, 2, 288, 408 },
    { 3, 4, 288, 240 },
    { 5, 12, 288, 240 },
    { 13, 16, 0, 0 },
    { 17, 20, 0, 0 },
    { 21, 23, 288, 240 },
    { 24, 25, 0, 0 },
    { 26, 27, 0, 0 },
    { 28, 29, 0, 0 },
};

static unsigned int CharMdlTbl[10] = {
    DATANUM(DATA_mdbeach, 0x00), DATANUM(DATA_mdbeach, 0x04), DATANUM(DATA_mdbeach, 0x08), DATANUM(DATA_mdbeach, 0x0C), DATANUM(DATA_mdbeach, 0x10), DATANUM(DATA_mdbeach, 0x14), DATANUM(DATA_mdbeach, 0x18), DATANUM(DATA_mdbeach, 0x1C), DATANUM(DATA_mdbeach, 0x20), DATANUM(DATA_mdbeach, 0x24)
};

static unsigned int CharVolleyMdlTbl[10] = {
    DATANUM(DATA_mdbeach, 0x28), DATANUM(DATA_mdbeach, 0x29), DATANUM(DATA_mdbeach, 0x2A), DATANUM(DATA_mdbeach, 0x2B), DATANUM(DATA_mdbeach, 0x2C), DATANUM(DATA_mdbeach, 0x2D), DATANUM(DATA_mdbeach, 0x2E), DATANUM(DATA_mdbeach, 0x2F), DATANUM(DATA_mdbeach, 0x30), DATANUM(DATA_mdbeach, 0x31)
};

static unsigned int CharHockeyMdlTbl[10] = {
    DATANUM(DATA_mdbeach, 0x32), DATANUM(DATA_mdbeach, 0x33), DATANUM(DATA_mdbeach, 0x34), DATANUM(DATA_mdbeach, 0x35), DATANUM(DATA_mdbeach, 0x36), DATANUM(DATA_mdbeach, 0x37), DATANUM(DATA_mdbeach, 0x38), DATANUM(DATA_mdbeach, 0x39), DATANUM(DATA_mdbeach, 0x3A), DATANUM(DATA_mdbeach, 0x3B)
};

static unsigned int CapManFileTbl[2] = {
    DATANUM(DATA_mdbeach, 0x43), DATANUM(DATA_mdbeach, 0x44)
};

static unsigned int VsFileTbl[3] = {
    DATANUM(DATA_mdbeach, 0x45), DATANUM(DATA_mdbeach, 0x46), 0
};

static unsigned int BallMdlTbl[3] = {
    DATANUM(DATA_mdbeach, 0x3E), DATANUM(DATA_mdbeach, 0x3F), DATANUM(DATA_mdbeach, 0x40)
};

static unsigned int StarMotFileTbl[3] = {
    DATANUM(DATA_mdbeach, 0x47), DATANUM(DATA_mdbeach, 0x48), DATANUM(DATA_mdbeach, 0x49)
};

static unsigned int CharMotTbl[30] = {
    DATANUM(DATA_mdbeach, 0x01), DATANUM(DATA_mdbeach, 0x02), DATANUM(DATA_mdbeach, 0x03), DATANUM(DATA_mdbeach, 0x05), DATANUM(DATA_mdbeach, 0x06), DATANUM(DATA_mdbeach, 0x07), DATANUM(DATA_mdbeach, 0x09), DATANUM(DATA_mdbeach, 0x0A), DATANUM(DATA_mdbeach, 0x0B), DATANUM(DATA_mdbeach, 0x0D), DATANUM(DATA_mdbeach, 0x0E), DATANUM(DATA_mdbeach, 0x0F), DATANUM(DATA_mdbeach, 0x11), DATANUM(DATA_mdbeach, 0x12), DATANUM(DATA_mdbeach, 0x13), DATANUM(DATA_mdbeach, 0x15), DATANUM(DATA_mdbeach, 0x16), DATANUM(DATA_mdbeach, 0x17), DATANUM(DATA_mdbeach, 0x19), DATANUM(DATA_mdbeach, 0x1A), DATANUM(DATA_mdbeach, 0x1B), DATANUM(DATA_mdbeach, 0x1D), DATANUM(DATA_mdbeach, 0x1E), DATANUM(DATA_mdbeach, 0x1F), DATANUM(DATA_mdbeach, 0x21), DATANUM(DATA_mdbeach, 0x22), DATANUM(DATA_mdbeach, 0x23), DATANUM(DATA_mdbeach, 0x25), DATANUM(DATA_mdbeach, 0x26), DATANUM(DATA_mdbeach, 0x27)
};

static s16 ToueiAnmNoTbl[4] = { 6, 6, 6, 6 };

static s16 ToueiMdlNoTbl[4] = { 39, 40, 41, 42 };

static char *ToueiTexNameTbl[4] = { "dummy", "dummy", "dummy", "dummy" };

// NONMATCHING: register allocation of loop locals differs (same instruction sequence)
void MdSpriteCreate(MDSPRITE_WORK *sprite)
{
    HUSPRGRPID grpId;
    MDSPR_GRP_INFO *grpInfo;
    MDSPR_INFO *sprInfo;
    int i;
    int j;
    int grpNum;
    for(i=0; i<MDSPR_ANIM_MAX; i++) {
        sprite->animP[i] = HuSprAnimRead(HuDataSelHeapReadNum(SpriteAnimFile[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    for(i=0; i<MDSPR_MAX; i++) {
        sprInfo = &SpriteInfo[i];
        sprite->sprId[i] = HuSprCreate(sprite->animP[sprInfo->animNo], 0, 0);
    }
    for(grpInfo=&SpriteGrpInfo[0], i=0; i<MDSPR_GRP_MAX; i++, grpInfo++) {
        grpNum = grpInfo->end-grpInfo->start+1;
        grpId = HuSprGrpCreate(grpNum);
        sprite->grpId[i] = grpId;
        for(sprInfo=&SpriteInfo[grpInfo->start], j=0; j<grpNum; j++, sprInfo++) {
            HuSprGrpMemberSet(grpId, j, sprite->sprId[grpInfo->start+j]);
            HuSprPriSet(grpId, j, sprInfo->pri);
            HuSprDispOn(grpId, j);
            HuSprPosSet(grpId, j, sprInfo->pos.x, sprInfo->pos.y);
            HuSprZRotSet(grpId, j, 0);
            HuSprScaleSet(grpId, j, sprInfo->scale.x, sprInfo->scale.y);
            HuSprColorSet(grpId, j, 255, 255, 255);
            HuSprTPLvlSet(grpId, j, sprInfo->tpLvl);
        }
        HuSprGrpPosSet(grpId, grpInfo->posX, grpInfo->posY);
        HuSprGrpCenterSet(grpId, 0, 0);
        HuSprGrpZRotSet(grpId, 0);
        HuSprGrpScaleSet(grpId, 1, 1);
        HuSprGrpDrawNoSet(grpId, 0);
        HuSprGrpScissorSet(grpId, 0, 0, 640, 480);
    }
    grpId = sprite->grpId[1];
    HuSprBankSet(grpId, 0, 3);
    HuSprBankSet(grpId, 1, 2);
    grpId = sprite->grpId[2];
    HuSprBankSet(grpId, 0, 3);
    HuSprBankSet(grpId, 1, 2);
    HuSprBankSet(grpId, 2, 0);
    HuSprBankSet(grpId, 3, 1);
    HuSprBankSet(grpId, 4, 3);
    HuSprBankSet(grpId, 5, 2);
    HuSprBankSet(grpId, 6, 0);
    HuSprBankSet(grpId, 7, 1);
    HuSprAttrSet(grpId, 4, HUSPR_ATTR_ADDCOL);
    HuSprAttrSet(grpId, 5, HUSPR_ATTR_ADDCOL);
    HuSprAttrSet(grpId, 6, HUSPR_ATTR_ADDCOL);
    HuSprAttrSet(grpId, 7, HUSPR_ATTR_ADDCOL);
    HuSprGrpDrawNoSet(sprite->grpId[5], 127);
    HuSprBankSet(sprite->grpId[7], 1, 0);
    HuSprBankSet(sprite->grpId[8], 1, 0);
    for(i=0; i<MDSPR_GRP_MAX; i++) {
        MdSpriteDispSet(sprite, i, FALSE);
    }
}

void MdSpriteDispSet(MDSPRITE_WORK *sprite, int grpNo, BOOL dispF)
{
    int i;
    MDSPR_GRP_INFO *grpInfo;
    HUSPRGRPID grpId;
    int grpNum;
    grpInfo = &SpriteGrpInfo[grpNo];
    grpId = sprite->grpId[grpNo];
    grpNum = grpInfo->end-grpInfo->start+1;
    for(i=0; i<grpNum; i++) {
        if(dispF == FALSE) {
            HuSprDispOff(grpId, i);
        } else if(dispF == TRUE) {
            HuSprDispOn(grpId, i);
        }
    }
}

HuVec2f *MdSpriteGrpPosGet(MDSPRITE_WORK *sprite, int grpNo)
{
    HUSPRGRP *grp = &HuSprGrpData[sprite->grpId[grpNo]];
    return &grp->pos;
}

HuVec2f *MdSpriteGrpScaleGet(MDSPRITE_WORK *sprite, int grpNo)
{
    HUSPRGRP *grp = &HuSprGrpData[sprite->grpId[grpNo]];
    return &grp->scale;
}

HuVec2f *MdSpritePosGet(MDSPRITE_WORK *sprite, int grpNo, int memberNo)
{
    HUSPRGRP *grp = &HuSprGrpData[sprite->grpId[grpNo]];
    HUSPRITE *spr = &HuSprData[grp->sprId[memberNo]];
    return &spr->pos;
}

HuVec2f *MdSpriteScaleGet(MDSPRITE_WORK *sprite, int grpNo, int memberNo)
{
    HUSPRGRP *grp = &HuSprGrpData[sprite->grpId[grpNo]];
    HUSPRITE *spr = &HuSprData[grp->sprId[memberNo]];
    return &spr->scale;
}

float *MdSpriteAlphaGet(MDSPRITE_WORK *sprite, int grpNo, int memberNo)
{
    HUSPRGRP *grp = &HuSprGrpData[sprite->grpId[grpNo]];
    HUSPRITE *spr = &HuSprData[grp->sprId[memberNo]];
    return &spr->a;
}

void MdModelCreate(MDMODEL_WORK *model, MDCAMERA_WORK *camera)
{
    int i;
    for(i=0; i<10; i++) {
        model->mdlId[MDMODEL_CHAR_START+i] = Hu3DModelCreateData(CharMdlTbl[i]);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_CHAR_START+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_CHAR_START+i], 1);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_CHAR_START+i], camera->camera[0].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_CHAR_START+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_CHAR_START+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_CHAR_START+i], 1, 1, 1);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_CHAR_START+i], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_CHAR_START+i], HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(model->mdlId[MDMODEL_CHAR_START+i], 1);
    }
    for(i=0; i<10; i++) {
        model->mdlId[MDMODEL_VOLLEY_START+i] = Hu3DModelCreateData(CharVolleyMdlTbl[i]);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_VOLLEY_START+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_VOLLEY_START+i], 0);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_VOLLEY_START+i], camera->camera[0].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_VOLLEY_START+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_VOLLEY_START+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_VOLLEY_START+i], 1, 1, 1);
    }
    for(i=0; i<10; i++) {
        model->mdlId[MDMODEL_HOCKEY_START+i] = Hu3DModelCreateData(CharHockeyMdlTbl[i]);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_HOCKEY_START+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_HOCKEY_START+i], 0);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_HOCKEY_START+i], camera->camera[0].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_HOCKEY_START+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_HOCKEY_START+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_HOCKEY_START+i], 1, 1, 1);
    }
    for(i=0; i<4; i++) {
        if(i == 0) {
            model->mdlId[MDMODEL_CAPMAN_START+i] = Hu3DModelCreateData(CapManFileTbl[0]);
        } else {
            model->mdlId[MDMODEL_CAPMAN_START+i] = Hu3DModelLink(model->mdlId[MDMODEL_CAPMAN_START]);
        }
        Hu3DModelAttrSet(model->mdlId[MDMODEL_CAPMAN_START+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_CAPMAN_START+i], 1);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_CAPMAN_START+i], camera->camera[0].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_CAPMAN_START+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_CAPMAN_START+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_CAPMAN_START+i], 1, 1, 1);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_CAPMAN_START+i], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_CAPMAN_START+i], HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(model->mdlId[MDMODEL_CAPMAN_START+i], 1);
        if(i == 0) {
            model->mdlId[MDMODEL_CAPCOM_START+i] = Hu3DModelCreateData(CapManFileTbl[1]);
        } else {
            model->mdlId[MDMODEL_CAPCOM_START+i] = Hu3DModelLink(model->mdlId[MDMODEL_CAPCOM_START]);
        }
        Hu3DModelAttrSet(model->mdlId[MDMODEL_CAPCOM_START+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_CAPCOM_START+i], 1);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_CAPCOM_START+i], camera->camera[0].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_CAPCOM_START+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_CAPCOM_START+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_CAPCOM_START+i], 1, 1, 1);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_CAPCOM_START+i], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_CAPCOM_START+i], HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(model->mdlId[MDMODEL_CAPCOM_START+i], 1);
    }
    for(i=0; i<4; i++) {
        if(i == 0) {
            model->mdlId[MDMODEL_FINGER_START+i] = Hu3DModelCreateData(DATANUM(DATA_mdbeach, 0x3D));
        } else {
            model->mdlId[MDMODEL_FINGER_START+i] = Hu3DModelLink(model->mdlId[MDMODEL_FINGER_START]);
        }
        Hu3DModelAttrSet(model->mdlId[MDMODEL_FINGER_START+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_FINGER_START+i], 4);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_FINGER_START+i], camera->camera[0].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_FINGER_START+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_FINGER_START+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_FINGER_START+i], 1, 1, 1);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_FINGER_START+i], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_FINGER_START+i], HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(model->mdlId[MDMODEL_FINGER_START+i], 1);
    }
    for(i=0; i<4; i++) {
        if(i == 0) {
            model->mdlId[MDMODEL_FLASH_START+i] = Hu3DModelCreateData(DATANUM(DATA_mdbeach, 0x41));
        } else {
            model->mdlId[MDMODEL_FLASH_START+i] = Hu3DModelLink(model->mdlId[MDMODEL_FLASH_START]);
        }
        Hu3DModelAttrSet(model->mdlId[MDMODEL_FLASH_START+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_FLASH_START+i], 1);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_FLASH_START+i], camera->camera[0].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_FLASH_START+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_FLASH_START+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_FLASH_START+i], 0.8f, 0.8f, 0.8f);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_FLASH_START+i], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_FLASH_START+i], HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(model->mdlId[MDMODEL_FLASH_START+i], 1);
    }
    for(i=0; i<2; i++) {
        model->mdlId[MDMODEL_VS_VOLLEY+i] = Hu3DModelCreateData(VsFileTbl[i]);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_VS_VOLLEY+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_VS_VOLLEY+i], 1);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_VS_VOLLEY+i], camera->camera[0].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_VS_VOLLEY+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_VS_VOLLEY+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_VS_VOLLEY+i], 1, 1, 1);
    }
    for(i=0; i<3; i++) {
        model->mdlId[MDMODEL_BALL_START+i] = Hu3DModelCreateData(BallMdlTbl[i]);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_BALL_START+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_BALL_START+i], 1);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_BALL_START+i], camera->camera[0].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_BALL_START+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_BALL_START+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_BALL_START+i], 1, 1, 1);
    }
    model->mdlId[MDMODEL_STAGE] = Hu3DModelCreateData(DATANUM(DATA_mdbeach, 0x3C));
    Hu3DModelAttrSet(model->mdlId[MDMODEL_STAGE], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[MDMODEL_STAGE], 0);
    Hu3DModelCameraSet(model->mdlId[MDMODEL_STAGE], camera->camera[1].bit);
    Hu3DModelPosSet(model->mdlId[MDMODEL_STAGE], 0, 0, 0);
    Hu3DModelRotSet(model->mdlId[MDMODEL_STAGE], 0, 0, 0);
    /* the tunnel backdrop ends at the 4:3 edges: widen it to the window */
    Hu3DModelScaleSet(model->mdlId[MDMODEL_STAGE], HU_WIDE(1), 1, 1);
    Hu3DModelAttrSet(model->mdlId[MDMODEL_STAGE], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(model->mdlId[MDMODEL_STAGE], HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(model->mdlId[MDMODEL_STAGE], 1);
    model->mdlId[MDMODEL_FONTBGFILTER] = Hu3DHookFuncCreate(FontBGFilter);
    Hu3DModelLayerSet(model->mdlId[MDMODEL_FONTBGFILTER], 7);
    Hu3DModelCameraSet(model->mdlId[MDMODEL_FONTBGFILTER], camera->camera[1].bit);
    Hu3DModelAttrReset(model->mdlId[MDMODEL_FONTBGFILTER], HU3D_ATTR_DISPOFF);
    for(i=0; i<3; i++) {
        model->motId[i] = Hu3DJointMotion(model->mdlId[MDMODEL_STAGE], HuDataSelHeapReadNum(StarMotFileTbl[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    Hu3DMotionSet(model->mdlId[MDMODEL_STAGE], model->motId[0]);
    for(i=0; i<30; i++) {
        model->motId[3+i] = Hu3DJointMotion(model->mdlId[MDMODEL_CHAR_START+i/3], HuDataSelHeapReadNum(CharMotTbl[i], HU_MEMNUM_OVL, HEAP_MODEL));
        if(i%3 == 0) {
            Hu3DMotionSet(model->mdlId[MDMODEL_CHAR_START+i/3], model->motId[3+i]);
        }
    }
    for(i=0; i<MDMODEL_MAX; i++) {
        HU3DMODEL *modelP = &Hu3DData[model->mdlId[i]];
        model->posP[i] = &modelP->pos;
        model->rotP[i] = &modelP->rot;
        model->scaleP[i] = &modelP->scale;
    }
    Hu3DLayerHookSet(1, Layer2Hook);
    Hu3DLayerHookSet(2, Layer2Hook);
    Hu3DLayerHookSet(4, Layer2Hook);
    Hu3DLayerHookSet(0, Layer1Hook);
}

static void FontBGFilter(HU3DMODEL *modelP, Mtx *mtx)
{
    MDDEBUGFONT_WORK *debugFont = sysP->debugFont;
    GXColor color = { 0, 0, 0, 128 };
    s16 width = 0;
    if(debugFont->printF == TRUE) {
        Mtx44 proj;
        width = debugFont->maxLen*(int)(8.0f*debugFont->size);
        GXSetViewport(0, 0, HU_FB_WIDTH, HU_FB_HEIGHT, 0, 1);
        MTXOrtho(proj, 0, 480, 0, 640, 0, -16777215);
        GXSetProjection(proj, GX_ORTHOGRAPHIC);
        MTXIdentity(*mtx);
        GXLoadPosMtxImm(*mtx, GX_PNMTX0);
        GXSetColorUpdate(TRUE);
        GXSetAlphaUpdate(FALSE);
        GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
        GXSetNumChans(1);
        GXSetNumTexGens(0);
        GXSetNumTevStages(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_SPEC);
        GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_SPEC);
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
        GXPosition3s16(debugFont->posX, debugFont->posY+debugFont->ofsY, 0);
        GXPosition3s16(debugFont->posX+width, debugFont->posY+debugFont->ofsY, 0);
        GXPosition3s16(debugFont->posX+width, debugFont->posY+debugFont->rowOfsY, 0);
        GXPosition3s16(debugFont->posX, debugFont->posY+debugFont->rowOfsY, 0);
        GXEnd();
    }
}

static void Layer2Hook(s16 layerNo)
{
    if(Hu3DCameraNo == 0) {
        Hu3DZClear();
    }
}

static void Layer1Hook(s16 layerNo)
{
    if(Hu3DCameraNo == 1) {
        Hu3DZClear();
    }
}

void MdToueiMdlInit(void)
{
    int i;
    MDSPRITE_WORK *sprite = sysP->sprite;
    MDMODEL_WORK *model = sysP->model;
    for(i=0; i<4; i++) {
        HU3DANIMID anmId = Hu3DAnimCreate(sprite->animP[ToueiAnmNoTbl[i]], model->mdlId[ToueiMdlNoTbl[i]], ToueiTexNameTbl[i]);
        model->toueiAnmId[i] = anmId;
        Hu3DAnimSpeedSet(anmId, 1);
        Hu3DAnimAttrSet(anmId, HU3D_ANIM_ATTR_ANIMON);
        Hu3DAnimAttrSet(anmId, HU3D_ANIM_ATTR_LOOP);
    }
    Hu3DAnmNoSet(model->toueiAnmId[0], 0);
    Hu3DAnmNoSet(model->toueiAnmId[1], 0);
    Hu3DAnmNoSet(model->toueiAnmId[2], 0);
    Hu3DAnmNoSet(model->toueiAnmId[3], 0);
}
