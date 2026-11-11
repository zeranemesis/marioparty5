#include "REL/mdminiDll.h"
#include "datadir_enum.h"

static void FontBGFilter(HU3DMODEL *modelP, Mtx *mtx);
static void LayerPostHook(s16 layerNo);
static void Layer2Hook(s16 layerNo);
static void Layer1Hook(s16 layerNo);
static void Touei1MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Touei2MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Touei3MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Touei4MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Touei5MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Touei6MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Touei7MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Touei8MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Touei9MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Touei10MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);

static void Ring1MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Ring2MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Ring3MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Ring4MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);

static unsigned int SpriteAnimFile[MDMINI_ANIM_MAX] = {
    DATANUM(DATA_mdmini, 0x34),
    DATANUM(DATA_mdmini, 0x35),
    DATANUM(DATA_mdmini, 0x32),
    DATANUM(DATA_mdmini, 0x33),
    DATANUM(DATA_mdmini, 0x39),
    DATANUM(DATA_mdmini, 0x3a),
    DATANUM(DATA_mdmini, 0x3b),
    DATANUM(DATA_mdmini, 0x36),
    DATANUM(DATA_mdmini, 0x3c),
    DATANUM(DATA_mdmini, 0x37),
    DATANUM(DATA_mdmini, 0x38),
    DATANUM(DATA_mdmini, 0x3d),
};

static MDSPR_INFO SpriteInfo[MDSPR_MAX] = {
    { 0, 2, { 0, -28 }, { 72, 1 }, 1.0f },
    { 0, 2, { 0, 32 }, { 72, 1 }, 1.0f },
    { 1, 3, { 0, 0 }, { 1, 8 }, 0.6f },
    { 2, 8, { 64, 0 }, { 1, 1 }, 1.0f },
    { 2, 8, { -64, 0 }, { 1, 1 }, 1.0f },
    { 2, 5, { 64, 0 }, { 1, 1 }, 1.0f },
    { 2, 5, { -64, 0 }, { 1, 1 }, 1.0f },
    { 2, 5, { 64, 0 }, { 1, 1 }, 1.0f },
    { 2, 5, { -64, 0 }, { 1, 1 }, 1.0f },
    { 2, 4, { 64, 0 }, { 1, 1 }, 1.0f },
    { 2, 4, { -64, 0 }, { 1, 1 }, 1.0f },
    { 2, 4, { 64, 0 }, { 1, 1 }, 1.0f },
    { 2, 4, { -64, 0 }, { 1, 1 }, 1.0f },
    { 4, 6, { 0, 0 }, { 1, 1 }, 1.0f },
    { 4, 6, { 0, 0 }, { 1, 1 }, 1.0f },
    { 4, 6, { 0, 0 }, { 1, 1 }, 1.0f },
    { 4, 6, { 0, 0 }, { 1, 1 }, 1.0f },
    { 5, 7, { 0, 0 }, { 1, 1 }, 1.0f },
    { 5, 7, { 0, 0 }, { 1, 1 }, 1.0f },
    { 5, 7, { 0, 0 }, { 1, 1 }, 1.0f },
    { 5, 7, { 0, 0 }, { 1, 1 }, 1.0f },
    { 11, 9, { 0, 0 }, { 1, 1 }, 1.0f },
};

static MDSPR_GRP_INFO SpriteGrpInfo[MDSPR_GRP_MAX] = {
    { 0, 2, 288, 408 },
    { 3, 4, 288, 240 },
    { 5, 12, 288, 240 },
    { 13, 16, 0, 0 },
    { 17, 20, 0, 0 },
    { 21, 21, 288, 240 },
};

static GXColor RingColorTbl[8] = {
    { 255, 128, 0, 0 },
    { 255, 128, 0, 0 },
    { 255, 128, 0, 0 },
    { 255, 128, 0, 0 },
    { 255, 128, 0, 0 },
    { 0, 255, 255, 0 },
    { 255, 0, 0, 0 },
    { 0, 128, 255, 0 },
};

static HU3DMATHOOK ToueiMatHookTbl[10] = {
    Touei1MatHook,
    Touei2MatHook,
    Touei3MatHook,
    Touei4MatHook,
    Touei5MatHook,
    Touei6MatHook,
    Touei7MatHook,
    Touei8MatHook,
    Touei9MatHook,
    Touei10MatHook
};

static HU3DMATHOOK RingMatHookTbl[4] = {
    Ring1MatHook,
    Ring2MatHook,
    Ring3MatHook,
    Ring4MatHook
};

static unsigned int CharMdlTbl[10] = {
    DATANUM(DATA_mdmini, 0x0),
    DATANUM(DATA_mdmini, 0x3),
    DATANUM(DATA_mdmini, 0x6),
    DATANUM(DATA_mdmini, 0x9),
    DATANUM(DATA_mdmini, 0xc),
    DATANUM(DATA_mdmini, 0xf),
    DATANUM(DATA_mdmini, 0x12),
    DATANUM(DATA_mdmini, 0x15),
    DATANUM(DATA_mdmini, 0x18),
    DATANUM(DATA_mdmini, 0x1b)
};

static HuVecF CharMdlPosTbl[10] = {
    { -250, 0, 0 },
    { -125, 0, 0 },
    { 0, 0, 0 },
    { 125, 0, 0 },
    { 250, 0, 0 },
    { -250, -250, 0 },
    { -125, -250, 0 },
    { 0, -250, 0 },
    { 125, -250, -1800 },
    { 250, -250, -1000 }
};

static unsigned int CapManFileTbl[2] = {
    DATANUM(DATA_mdmini, 0x2c),
    DATANUM(DATA_mdmini, 0x2d)
};

static unsigned int CapModeFileTbl[6] = {
    DATANUM(DATA_mdmini, 0x24),
    DATANUM(DATA_mdmini, 0x25),
    DATANUM(DATA_mdmini, 0x26),
    DATANUM(DATA_mdmini, 0x27),
    DATANUM(DATA_mdmini, 0x28),
    DATANUM(DATA_mdmini, 0x29)
};

static unsigned int StarMotFileTbl[3] = {
    DATANUM(DATA_mdmini, 0x2f),
    DATANUM(DATA_mdmini, 0x30),
    DATANUM(DATA_mdmini, 0x31)
};

static unsigned int CharMotTbl[20] = {
    DATANUM(DATA_mdmini, 0x1),
    DATANUM(DATA_mdmini, 0x2),
    DATANUM(DATA_mdmini, 0x4),
    DATANUM(DATA_mdmini, 0x5),
    DATANUM(DATA_mdmini, 0x7),
    DATANUM(DATA_mdmini, 0x8),
    DATANUM(DATA_mdmini, 0xa),
    DATANUM(DATA_mdmini, 0xb),
    DATANUM(DATA_mdmini, 0xd),
    DATANUM(DATA_mdmini, 0xe),
    DATANUM(DATA_mdmini, 0x10),
    DATANUM(DATA_mdmini, 0x11),
    DATANUM(DATA_mdmini, 0x13),
    DATANUM(DATA_mdmini, 0x14),
    DATANUM(DATA_mdmini, 0x16),
    DATANUM(DATA_mdmini, 0x17),
    DATANUM(DATA_mdmini, 0x19),
    DATANUM(DATA_mdmini, 0x1a),
    DATANUM(DATA_mdmini, 0x1c),
    DATANUM(DATA_mdmini, 0x1d)
};

static s16 ToueiAnmNoTbl[6] = {
    6,
    6,
    6,
    6,
    7,
    8
};

static s16 ToueiMdlNoTbl[6] = {
    MDMODEL_FINGER_1P,
    MDMODEL_FINGER_2P,
    MDMODEL_FINGER_3P,
    MDMODEL_FINGER_4P,
    MDMODEL_AUX_0,
    MDMODEL_AUX_1
};

static char *ToueiTexNameTbl[6] = {
    "dummy",
    "dummy",
    "dummy",
    "dummy",
    "IA8",
    "IA8"
};

static HuVecF ToueiTexObjOfs[10] = {
    { 0, 15, 0 },
    { 0, 18, 0 },
    { 0, 12, 0 },
    { 0, 20, 0 },
    { 0, 15, 0 },
    { 0, 6, 0 },
    { 0, -5, 0 },
    { 0, 20, 0 },
    { 0, 70, 0 },
    { 0, 10, 0 },
};

static char *ToueiTexObjHook[10] = {
    "root_head",
    "root_head",
    "ske_head",
    "ske_head",
    "ske_head",
    "ske_head",
    "ske_head",
    "ske_head",
    "ske_head",
    "ske_head",
};

void MdSpriteCreate(MDSPRITE_WORK *sprite)
{
    HUSPRGRPID grpId;
    MDSPR_INFO *sprInfo;
    int i;
    MDSPR_GRP_INFO *grpInfo;
    int j;
    int grpNum;
    for(i=0; i<MDMINI_ANIM_MAX; i++) {
        sprite->animP[i] = HuSprAnimRead(HuDataReadNum(SpriteAnimFile[i], HU_MEMNUM_OVL));
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
    grpId = sprite->grpId[MDSPR_GRP_CURSOR_LR];
    HuSprBankSet(grpId, 0, 3);
    HuSprBankSet(grpId, 1, 2);
    grpId = sprite->grpId[MDSPR_GRP_CURSOR_UDLR];
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
    MdSpriteDispSet(sprite, MDSPR_GRP_WIN, FALSE);
    MdSpriteDispSet(sprite, MDSPR_GRP_CURSOR_LR, FALSE);
    MdSpriteDispSet(sprite, MDSPR_GRP_CURSOR_UDLR, FALSE);
    MdSpriteDispSet(sprite, MDSPR_GRP_3, FALSE);
    MdSpriteDispSet(sprite, MDSPR_GRP_4, FALSE);
    MdSpriteDispSet(sprite, MDSPR_GRP_5, FALSE);
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
    model->mdlId[MDMODEL_STAGE_CHR] = Hu3DModelCreateData(DATANUM(DATA_mdmini, 0x20));
    Hu3DModelAttrSet(model->mdlId[MDMODEL_STAGE_CHR], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[MDMODEL_STAGE_CHR], 0);
    Hu3DModelCameraSet(model->mdlId[MDMODEL_STAGE_CHR], camera->camera[MDCAMERA_BG].bit);
    Hu3DModelPosSet(model->mdlId[MDMODEL_STAGE_CHR], 0, -300, 0);
    Hu3DModelRotSet(model->mdlId[MDMODEL_STAGE_CHR], 0, 0, 0);
    Hu3DModelScaleSet(model->mdlId[MDMODEL_STAGE_CHR], 2, 2, 1);
    Hu3DModelAttrSet(model->mdlId[MDMODEL_STAGE_CHR], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(model->mdlId[MDMODEL_STAGE_CHR], HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(model->mdlId[MDMODEL_STAGE_CHR], 0.5f);
    for(i=0; i<10; i++) {
        model->mdlId[MDMODEL_CHR_00+i] = Hu3DModelCreateData(CharMdlTbl[i]);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_CHR_00+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_CHR_00+i], 1);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_CHR_00+i], camera->camera[MDCAMERA_BG].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_CHR_00+i], CharMdlPosTbl[i].x, CharMdlPosTbl[i].y, CharMdlPosTbl[i].z);
        Hu3DModelRotSet(model->mdlId[MDMODEL_CHR_00+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_CHR_00+i], 0.8f, 0.8f, 0.8f);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_CHR_00+i], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_CHR_00+i], HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(model->mdlId[MDMODEL_CHR_00+i], 1.0f);
    }
    model->mdlId[MDMODEL_STAGE] = Hu3DModelCreateData(DATANUM(DATA_mdmini, 0x1f));
    Hu3DModelAttrSet(model->mdlId[MDMODEL_STAGE], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[MDMODEL_STAGE], 0);
    Hu3DModelCameraSet(model->mdlId[MDMODEL_STAGE], camera->camera[MDCAMERA_MAIN].bit);
    Hu3DModelPosSet(model->mdlId[MDMODEL_STAGE], 0, 0, 0);
    Hu3DModelRotSet(model->mdlId[MDMODEL_STAGE], 0, 0, 0);
    Hu3DModelScaleSet(model->mdlId[MDMODEL_STAGE], 1, 1, 1);
    Hu3DModelAttrSet(model->mdlId[MDMODEL_STAGE], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(model->mdlId[MDMODEL_STAGE], HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(model->mdlId[MDMODEL_STAGE], 1.0f);
    for(i=0; i<4; i++) {
        if(i == 0) {
            model->mdlId[MDMODEL_CAP_MAN_1P+i] = Hu3DModelCreateData(CapManFileTbl[0]);
        } else {
            model->mdlId[MDMODEL_CAP_MAN_1P+i] = Hu3DModelLink(model->mdlId[MDMODEL_CAP_MAN_1P]);
        }
        Hu3DModelAttrSet(model->mdlId[MDMODEL_CAP_MAN_1P+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_CAP_MAN_1P+i], 1);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_CAP_MAN_1P+i], camera->camera[MDCAMERA_MAIN].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_CAP_MAN_1P+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_CAP_MAN_1P+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_CAP_MAN_1P+i], 1, 1, 1);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_CAP_MAN_1P+i], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_CAP_MAN_1P+i], HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(model->mdlId[MDMODEL_CAP_MAN_1P+i], 1.0f);
        if(i == 0) {
            model->mdlId[MDMODEL_CAP_COM_1P+i] = Hu3DModelCreateData(CapManFileTbl[1]);
        } else {
            model->mdlId[MDMODEL_CAP_COM_1P+i] = Hu3DModelLink(model->mdlId[MDMODEL_CAP_COM_1P]);
        }
        Hu3DModelAttrSet(model->mdlId[MDMODEL_CAP_COM_1P+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_CAP_COM_1P+i], 1);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_CAP_COM_1P+i], camera->camera[MDCAMERA_MAIN].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_CAP_COM_1P+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_CAP_COM_1P+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_CAP_COM_1P+i], 1, 1, 1);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_CAP_COM_1P+i], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_CAP_COM_1P+i], HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(model->mdlId[MDMODEL_CAP_COM_1P+i], 1.0f);
    }
    for(i=0; i<10; i++) {
        if(i == 0) {
            model->mdlId[MDMODEL_CAP_CHR_00+i] = Hu3DModelCreateData(DATANUM(DATA_mdmini, 0x21));
        } else {
            model->mdlId[MDMODEL_CAP_CHR_00+i] = Hu3DModelLink(model->mdlId[MDMODEL_CAP_CHR_00]);
        }
        Hu3DModelAttrSet(model->mdlId[MDMODEL_CAP_CHR_00+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_CAP_CHR_00+i], 1);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_CAP_CHR_00+i], camera->camera[MDCAMERA_MAIN].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_CAP_CHR_00+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_CAP_CHR_00+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_CAP_CHR_00+i], 1, 1, 1);
        Hu3DModelMatHookSet(model->mdlId[MDMODEL_CAP_CHR_00+i], ToueiMatHookTbl[i]);
    }
    for(i=0; i<6; i++) {
        model->mdlId[MDMODEL_CAP_MODE_0+i] = Hu3DModelCreateData(CapModeFileTbl[i]);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_CAP_MODE_0+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_CAP_MODE_0+i], 0);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_CAP_MODE_0+i], camera->camera[MDCAMERA_MAIN].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_CAP_MODE_0+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_CAP_MODE_0+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_CAP_MODE_0+i], 1, 1, 1);
    }
    model->mdlId[MDMODEL_AUX_0] = Hu3DModelCreateData(DATANUM(DATA_mdmini, 0x2a));
    Hu3DModelAttrReset(model->mdlId[MDMODEL_AUX_0], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[MDMODEL_AUX_0], 3);
    Hu3DModelCameraSet(model->mdlId[MDMODEL_AUX_0], camera->camera[MDCAMERA_MAIN].bit);
    Hu3DModelPosSet(model->mdlId[MDMODEL_AUX_0], 0, 0, 0);
    Hu3DModelRotSet(model->mdlId[MDMODEL_AUX_0], 0, 0, 0);
    Hu3DModelScaleSet(model->mdlId[MDMODEL_AUX_0], 1, 1, 1);
    model->mdlId[MDMODEL_AUX_1] = Hu3DModelCreateData(DATANUM(DATA_mdmini, 0x2b));
    Hu3DModelAttrReset(model->mdlId[MDMODEL_AUX_1], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[MDMODEL_AUX_1], 3);
    Hu3DModelCameraSet(model->mdlId[MDMODEL_AUX_1], camera->camera[MDCAMERA_MAIN].bit);
    Hu3DModelPosSet(model->mdlId[MDMODEL_AUX_1], 0, 0, 0);
    Hu3DModelRotSet(model->mdlId[MDMODEL_AUX_1], 0, 0, 0);
    Hu3DModelScaleSet(model->mdlId[MDMODEL_AUX_1], 1, 1, 1);
    model->mdlId[MDMODEL_AUX_2] = Hu3DModelCreateData(DATANUM(DATA_mdmini, 0x2e));
    Hu3DModelAttrReset(model->mdlId[MDMODEL_AUX_2], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[MDMODEL_AUX_2], 3);
    Hu3DModelCameraSet(model->mdlId[MDMODEL_AUX_2], camera->camera[MDCAMERA_MAIN].bit);
    Hu3DModelPosSet(model->mdlId[MDMODEL_AUX_2], 0, 0, 0);
    Hu3DModelRotSet(model->mdlId[MDMODEL_AUX_2], 0, 0, 0);
    Hu3DModelScaleSet(model->mdlId[MDMODEL_AUX_2], 1, 1, 1);
    for(i=0; i<4; i++) {
        if(i == 0) {
            model->mdlId[MDMODEL_FINGER_1P+i] = Hu3DModelCreateData(DATANUM(DATA_mdmini, 0x22));
        } else {
            model->mdlId[MDMODEL_FINGER_1P+i] = Hu3DModelLink(model->mdlId[MDMODEL_FINGER_1P]);
        }
        Hu3DModelAttrSet(model->mdlId[MDMODEL_FINGER_1P+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_FINGER_1P+i], 3);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_FINGER_1P+i], camera->camera[MDCAMERA_MAIN].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_FINGER_1P+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_FINGER_1P+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_FINGER_1P+i], 1, 1, 1);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_FINGER_1P+i], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_FINGER_1P+i], HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(model->mdlId[MDMODEL_FINGER_1P+i], 1.0f);
    }
    for(i=0; i<4; i++) {
        if(i == 0) {
            model->mdlId[MDMODEL_RING_1P+i] = Hu3DModelCreateData(DATANUM(DATA_mdmini, 0x23));
        } else {
            model->mdlId[MDMODEL_RING_1P+i] = Hu3DModelLink(model->mdlId[MDMODEL_RING_1P]);
        }
        Hu3DModelAttrSet(model->mdlId[MDMODEL_RING_1P+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_RING_1P+i], 2);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_RING_1P+i], camera->camera[MDCAMERA_MAIN].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_RING_1P+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_RING_1P+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_RING_1P+i], 1, 1, 1);
        Hu3DModelMatHookSet(model->mdlId[MDMODEL_RING_1P+i], RingMatHookTbl[i]);
    }
    model->mdlId[MDMODEL_STAR] = Hu3DModelCreateData(DATANUM(DATA_mdmini, 0x1e));
    Hu3DModelAttrSet(model->mdlId[MDMODEL_STAR], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[MDMODEL_STAR], 0);
    Hu3DModelCameraSet(model->mdlId[MDMODEL_STAR], camera->camera[MDCAMERA_FG].bit);
    Hu3DModelPosSet(model->mdlId[MDMODEL_STAR], 0, 0, 0);
    Hu3DModelRotSet(model->mdlId[MDMODEL_STAR], 0, 0, 0);
    Hu3DModelScaleSet(model->mdlId[MDMODEL_STAR], 1, 1, 1);
    Hu3DModelAttrSet(model->mdlId[MDMODEL_STAR], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(model->mdlId[MDMODEL_STAR], HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(model->mdlId[MDMODEL_STAR], 1.0f);
    model->mdlId[MDMODEL_FONTBGFILTER] = Hu3DHookFuncCreate(FontBGFilter);
    Hu3DModelLayerSet(model->mdlId[MDMODEL_FONTBGFILTER], 7);
    Hu3DModelCameraSet(model->mdlId[MDMODEL_FONTBGFILTER], camera->camera[MDCAMERA_FG].bit);
    Hu3DModelAttrReset(model->mdlId[MDMODEL_FONTBGFILTER], HU3D_ATTR_DISPOFF);
    for(i=0; i<3; i++) {
        model->motId[i] = Hu3DJointMotionData(model->mdlId[MDMODEL_STAR], StarMotFileTbl[i]);
    }
    Hu3DMotionSet(model->mdlId[MDMODEL_STAR], model->motId[0]);
    for(i=0; i<20; i++) {
        model->motId[3+i] = Hu3DJointMotionData(model->mdlId[MDMODEL_CHR_00+(i/2)], CharMotTbl[i]);
        if((i%2) == 0) {
            Hu3DMotionSet(model->mdlId[MDMODEL_CHR_00+(i/2)], model->motId[3+i]);
        }
    }
    for(i=0; i<MDMODEL_MAX; i++) {
        HU3DMODEL *modelP = &Hu3DData[model->mdlId[i]];
        model->posP[i] = &modelP->pos;
        model->rotP[i] = &modelP->rot;
        model->scaleP[i] = &modelP->scale;
    }
    Hu3DLayerHookSet(1+HU3D_LAYER_HOOK_POST, NULL);
    Hu3DLayerHookSet(0+HU3D_LAYER_HOOK_POST, LayerPostHook);
    Hu3DLayerHookSet(1, Layer2Hook);
    Hu3DLayerHookSet(3, Layer2Hook);
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

static void LayerPostHook(s16 layerNo)
{
    if(Hu3DCameraNo == MDCAMERA_BG) {
        Hu3DZClear();
    }
}

static void Layer2Hook(s16 layerNo)
{
    if(Hu3DCameraNo == MDCAMERA_MAIN) {
        Hu3DZClear();
    }
}

static void Layer1Hook(s16 layerNo)
{
    if(Hu3DCameraNo == MDCAMERA_FG) {
        Hu3DZClear();
    }
}

static void TexBufFBCopy(int unitNo);
static void ToueiZBufClear(void);

void MdToueiLayerHook(s16 layerNo)
{
    int i;
    if(Hu3DCameraNo == MDCAMERA_BG) {
        for(i=0; i<10; i++) {
            TexBufFBCopy(i+MDMINI_TEXBUF_MIRROR);
        }
        ToueiZBufClear();
    }
}

static void TexBufSet(int unitNo)
{
    MDTEXBUF_WORK *texbuf = sysP->texbuf;
    GXColor color = { 16, 16, 16, 128 };
    MDTEXBUF_UNIT *unit;
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    unit = &texbuf->unit[unitNo];
    GXLoadTexObj(&unit->texObj, GX_TEXMAP0);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
}

static void Touei1MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    TexBufSet(MDMINI_TEXBUF_MIRROR+0);
}

static void Touei2MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    TexBufSet(MDMINI_TEXBUF_MIRROR+1);
}

static void Touei3MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    TexBufSet(MDMINI_TEXBUF_MIRROR+2);
}

static void Touei4MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    TexBufSet(MDMINI_TEXBUF_MIRROR+3);
}

static void Touei5MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    TexBufSet(MDMINI_TEXBUF_MIRROR+4);
}

static void Touei6MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    TexBufSet(MDMINI_TEXBUF_MIRROR+5);
}

static void Touei7MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    TexBufSet(MDMINI_TEXBUF_MIRROR+6);
}

static void Touei8MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    TexBufSet(MDMINI_TEXBUF_MIRROR+7);
}

static void Touei9MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    TexBufSet(MDMINI_TEXBUF_MIRROR+8);
}

static void Touei10MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    TexBufSet(MDMINI_TEXBUF_MIRROR+9);
}

void MdToueiMdlInit(void)
{
    MDSPRITE_WORK *sprite = sysP->sprite;
    MDMODEL_WORK *model = sysP->model;
    int i; //r30
    
    for(i=0; i<MDMODEL_ANIM_MAX; i++) {
        HU3DANIMID animId = Hu3DAnimCreate(sprite->animP[ToueiAnmNoTbl[i]], model->mdlId[ToueiMdlNoTbl[i]], ToueiTexNameTbl[i]);
        model->animId[i] = animId;
        Hu3DAnimSpeedSet(animId, 1);
        Hu3DAnimAttrSet(animId, HU3D_ANIM_ATTR_ANIMON);
        Hu3DAnimAttrSet(animId, HU3D_ANIM_ATTR_LOOP);
    }
    Hu3DAnmNoSet(model->animId[0], 0);
    Hu3DAnmNoSet(model->animId[1], 0);
    Hu3DAnmNoSet(model->animId[2], 0);
    Hu3DAnmNoSet(model->animId[3], 0);
    Hu3DAnmNoSet(model->animId[4], 0);
    Hu3DAnmNoSet(model->animId[5], 0);
}

void MdToueiTexBufInit(void)
{
    MDTEXBUF_WORK *texbuf = sysP->texbuf;
    MDMODEL_WORK *model = sysP->model;
    int i;
    for(i=0; i<10; i++) {
        MDTEXBUF_UNIT *unit = &texbuf->unit[MDMINI_TEXBUF_MIRROR+i];
        unit->w = MDMINI_TEXBUF_W;
        unit->h = MDMINI_TEXBUF_H;
        unit->texFmt = GX_TF_RGB565;
        MdTexBufObjPosSet(unit, model->mdlId[i+MDMODEL_CHR_00], ToueiTexObjHook[i], sysP->camera->camera[MDCAMERA_BG].bit, &ToueiTexObjOfs[i]);
        unit->addr = HuMemDirectMallocNum(HEAP_MODEL, GXGetTexBufferSize(unit->w, unit->h, unit->texFmt, GX_FALSE, 0), HU_MEMNUM_OVL);
        
        if(!unit->addr) {
            OSReport("sys->texbuf->unit[MDMINI_TEXBUF_MIRROR + %d].addr = NULL...\n", i);
        }
        GXInitTexObj(&unit->texObj, unit->addr, unit->w, unit->h, unit->texFmt, GX_CLAMP, GX_CLAMP, GX_FALSE);
        GXInitTexObjLOD(&unit->texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    }
}

static void TexBufFBCopy(int unitNo)
{
    MDTEXBUF_UNIT *unit = &sysP->texbuf->unit[unitNo];
    Hu3DFbCopyExec(unit->posX, unit->posY, unit->w, unit->h, unit->texFmt,GX_FALSE, unit->addr);
}

void MdTexBufObjPosSet(MDTEXBUF_UNIT *unit, HU3DMODELID modelId, char *objName, u16 cameraBit, HuVecF *ofsP)
{
    HuVecF pos3D; //sp+0x20
    HuVecF pos2D; //sp+0x14
    
    s16 x;
    s16 y;
    
    Hu3DModelObjPosGet(modelId, objName, &pos3D);
    pos3D.x += ofsP->x;
    pos3D.y += ofsP->y;
    pos3D.z += ofsP->z;
    Hu3D3Dto2D(&pos3D, cameraBit, &pos2D);
    x = (int)((pos2D.x/576)*640) & 0xFFFE;
    y = (int)(pos2D.y) & 0xFFFE;
    unit->posX = x-(unit->w/2);
    unit->posY = y-(unit->h/2);
}

void MdTexBufPosSet(MDTEXBUF_UNIT *unit, HuVecF *posP, u16 cameraBit)
{
    HuVecF pos2D; //sp+0x14
    
    s16 x;
    s16 y;
    Hu3D3Dto2D(posP, cameraBit, &pos2D);
    x = (int)((pos2D.x/576)*640) & 0xFFFE;
    y = (int)(pos2D.y) & 0xFFFE;
    unit->posX = x-(unit->w/2);
    unit->posY = y-(unit->h/2);
}

static void ToueiZBufClear(void)
{
    HU3DCAMERA *cameraP = &Hu3DCamera[Hu3DCameraNo];
    Mtx44 proj;
    Mtx modelview;
    Vec pos;
    Vec target;
    Vec up;
    GXColor color;
    float z;
    
    MTXPerspective(proj, cameraP->fov, cameraP->aspect, cameraP->near, cameraP->far);
    GXSetProjection(proj, GX_PERSPECTIVE);
    GXSetViewport(0, 0, 640, 480, 0, 1);
    GXSetScissor(0, 0, 640, 480);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    color.r = color.g = color.b = 0;
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetCullMode(GX_CULL_NONE);
    GXSetAlphaCompare(GX_GEQUAL, 0, GX_AOP_AND, GX_GEQUAL, 0);
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);

    pos.x = pos.y = pos.z = 0;
    target.x = target.y = 0;
    target.z = -100;
    up.x = up.z = 0;
    up.y = 1;
    MTXLookAt(modelview, &pos, &up, &target);
    GXLoadPosMtxImm(modelview, GX_PNMTX0);
    pos.x = (10000*-sin(cameraP->fov/2))*1.2f;
    pos.y = 10000*-sin(cameraP->fov/2)*(1/1.2f);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-pos.x, -pos.y, -10000);
    GXPosition3f32(pos.x, -pos.y, -10000);
    GXPosition3f32(pos.x, pos.y, -10000);
    GXPosition3f32(-pos.x, pos.y, -10000);
}

void MdRingScaleSet(int no, float scale);

void MdRingCreate(void)
{
    MDRING_WORK *ringWork = sysP->ring;
    MDMODEL_WORK *model = sysP->model;
    MDRING *ring;
    int i;
    for(ring=&ringWork->ring[0], i=0; i<4; i++, ring++) {
        ring->disp = FALSE;
        ring->mode = 0;
        ring->mdlNo = i+MDMODEL_RING_1P;
        ring->baseMdlNo = -1;
        ring->posP = &model->posP[ring->mdlNo];
        ring->rotP = &model->rotP[ring->mdlNo];
        ring->scaleP = &model->scaleP[ring->mdlNo];
        MdRingMdlSet(i, -1);
        MdRingOfsSet(i, 0, 0, 0);
        if(i <= 3) {
            ring->color = RingColorTbl[i];
        }
        MdRingColorReset(i);
    }
    ringWork->scale = 0.5f;
}

void MdRingMain(void)
{
    MDRING *ring; //r31
    int i; //r30
    HuVecF *posP; //r29
    HuVecF *basePosP; //r28
    int colorIdx; //r27
    MDRING_WORK *ringWork; //r26
    MDMODEL_WORK *model; //r25
    MDCAMERA_WORK *cameraWork; //r24
    
    HuVecF *rotP; //sp+0x10
    HuVecF *scaleP; //sp+0xC
    MDCAMERA *camera; //sp+0x8
    
    ringWork = sysP->ring;
    model = sysP->model;
    cameraWork = sysP->camera;
    camera = &cameraWork->camera[MDCAMERA_MAIN];
    for(ring=&ringWork->ring[0], i=0; i<4; i++, ring++) {
        posP = *ring->posP;
        rotP = *ring->rotP;
        scaleP = *ring->scaleP;
        if(ring->baseMdlNo != -1) {
            basePosP = model->posP[ring->baseMdlNo];
            *posP = *basePosP;
            posP->x = basePosP->x+ring->baseOfs.x;
            posP->y = basePosP->y+ring->baseOfs.y;
            posP->z = basePosP->z+ring->baseOfs.z;
        }
        switch(ring->mode) {
            case 1:
                ring->baseOfs.x = 0;
                ring->baseOfs.y = 0;
                ring->baseOfs.z = 0;
                MdRingDispSet(i, TRUE);
                MdRingColorReset(i);
                ring->color.r = RingColorTbl[0].r;
                ring->color.g = RingColorTbl[0].g;
                ring->color.b = RingColorTbl[0].b;
                ring->color.a = 0;
                ring->mode = 2;
                break;
            
            case 2:
                MdMathOscillate(MDMATH_OSC_90_REV, &ring->time, &ring->t, 16);
                MdMathLerpColor(&ring->color.a, 0, 192, ring->t);
                if(ring->time >= 1) {
                    MdRingColorReset(i);
                    ring->mode = 3;
                }
                break;
            
            case 3:
                MdMathOscillate(MDMATH_OSC_180, &ring->time, &ring->t, 32);
                MdMathLerpColor(&ring->color.a, 192, 144, ring->t);
                if(ring->time >= 1) {
                    MdRingColorReset(i);
                }
                break;
            
            case 4:
                MdRingColorReset(i);
                ring->endColor = ring->color;
                ring->mode = 5;
                break;
                
            case 5:
                MdMathOscillate(MDMATH_OSC_90, &ring->time, &ring->t, 8);
                MdMathLerpColor(&ring->color.a, ring->endColor.a, 0, ring->t);
                if(ring->time >= 1) {
                    MdRingDispSet(i, FALSE);
                    MdRingColorReset(i);
                    ring->color.r = RingColorTbl[0].r;
                    ring->color.g = RingColorTbl[0].g;
                    ring->color.b = RingColorTbl[0].b;
                    ring->color.a = 0;
                    ring->mode = 0;
                }
                break;
            
            case 6:
                MdRingDispSet(i, TRUE);
                MdRingColorReset(i);
                
                ring->color.a = 0;
                MdRingScaleSet(i, ringWork->scale);
                ring->mode = 7;
                break;
            
            case 7:
                MdMathOscillate(MDMATH_OSC_90, &ring->time, &ring->t, 4);
                MdMathOscillate(MDMATH_OSC_90, &ring->ofsTime, &ring->ofsT, 36);
                MdMathLerpColor(&ring->color.a, 0, 128, ring->t);
                MdMathLerpScale(*ring->scaleP, 0.8f, 1.8f, ring->ofsT);
                if(ring->time >= 1) {
                    ring->time = 0;
                    ring->t = 0;
                    ring->mode = 8;
                }
                break;
            
            case 8:
                MdMathOscillate(MDMATH_OSC_90, &ring->time, &ring->t, 32);
                MdMathOscillate(MDMATH_OSC_90, &ring->ofsTime, &ring->ofsT, 36);
                MdMathLerpColor(&ring->color.a, 128, 0, ring->t);
                MdMathLerpScale(*ring->scaleP, 0.8f, 1.8f, ring->ofsT);
                if(ring->time >= 1) {
                    MdRingDispSet(i, FALSE);
                    MdRingColorReset(i);
                    ring->color.a = 0;
                    ring->mode = 0;
                }
                break;
            
            case 9:
                MdRingColorReset(i);
                ring->endColor = ring->color;
                ring->mode = 10;
                break;
            
            case 10:
                MdMathOscillate(MDMATH_OSC_90, &ring->time, &ring->t, 8);
                MdMathLerpColor(&ring->color.r, ring->endColor.r, RingColorTbl[6].r, ring->t);
                MdMathLerpColor(&ring->color.g, ring->endColor.g, RingColorTbl[6].g, ring->t);
                MdMathLerpColor(&ring->color.b, ring->endColor.b, RingColorTbl[6].b, ring->t);
                MdMathLerpColor(&ring->color.a, ring->endColor.a, 192, ring->t);
                if(ring->time >= 1) {
                    MdRingColorReset(i);
                    ring->mode = 3;
                }
                break;
            
            case 11:
                MdRingColorReset(i);
                ring->endColor = ring->color;
                ring->mode = 12;
                break;
            
            case 12:
                MdMathOscillate(MDMATH_OSC_90, &ring->time, &ring->t, 8);
                MdMathLerpColor(&ring->color.r, ring->endColor.r, RingColorTbl[7].r, ring->t);
                MdMathLerpColor(&ring->color.g, ring->endColor.g, RingColorTbl[7].g, ring->t);
                MdMathLerpColor(&ring->color.b, ring->endColor.b, RingColorTbl[7].b, ring->t);
                MdMathLerpColor(&ring->color.a, ring->endColor.a, 192, ring->t);
                if(ring->time >= 1) {
                    MdRingColorReset(i);
                    ring->mode = 3;
                }
                break;
            
            case 13:
                MdRingColorReset(i);
                ring->endColor = ring->color;
                ring->mode = 14;
                break;
            
            case 14:
                colorIdx = 0;
                colorIdx = i;
                MdMathOscillate(MDMATH_OSC_90, &ring->time, &ring->t, 15);
                MdMathLerpColor(&ring->color.r, ring->endColor.r, RingColorTbl[colorIdx].r, ring->t);
                MdMathLerpColor(&ring->color.g, ring->endColor.g, RingColorTbl[colorIdx].g, ring->t);
                MdMathLerpColor(&ring->color.b, ring->endColor.b, RingColorTbl[colorIdx].b, ring->t);
                MdMathLerpColor(&ring->color.a, ring->endColor.a, 192, ring->t);
                if(ring->time >= 1) {
                    MdRingColorReset(i);
                    ring->mode = 3;
                }
                break;
            
            case 15:
                ring->color = RingColorTbl[i];
                ring->mode = 0;
                break;
        }
    }
    
}

void MdRingModeSet(int no, int mode)
{
    MDRING *ring = &sysP->ring->ring[no];
    ring->mode = mode;
}

void MdRingDispSet(int no, BOOL dispF)
{
    MDRING *ring = &sysP->ring->ring[no];
    MDMODEL_WORK *model = sysP->model;
    ring->disp = dispF;
    if(dispF == FALSE) {
        Hu3DModelDispOff(model->mdlId[ring->mdlNo]);
    } else if(dispF == TRUE) {
        Hu3DModelDispOn(model->mdlId[ring->mdlNo]);
    }
}

void MdRingScaleSet(int no, float scale)
{
    MDRING *ring = &sysP->ring->ring[no];
    (*ring->scaleP)->x = scale;
    (*ring->scaleP)->y = scale;
    (*ring->scaleP)->z = scale;
    
}

void MdRingMdlSet(int no, int mdlNo)
{
    MDRING *ring = &sysP->ring->ring[no];
    ring->baseMdlNo = mdlNo;
}

void MdRingOfsSet(int no, float x, float y, float z)
{
    MDRING *ring = &sysP->ring->ring[no];
    ring->baseOfs.x = x;
    ring->baseOfs.y = y;
    ring->baseOfs.z = z;
}

void MdRingColorReset(int no)
{
    MDRING *ring = &sysP->ring->ring[no];
    ring->unk10 = 0;
    ring->time = 0;
    ring->ofsTime = 0;
    ring->t = 0;
    ring->ofsT = 0;
}


static void RingDraw(int no)
{
    MDRING *ring = &sysP->ring->ring[no];
    Mtx trans;
    Mtx scale;
    Mtx texMtx;
    if(ring->disp) {
        GXSetColorUpdate(TRUE);
        GXSetAlphaUpdate(TRUE);
        GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_INVCOPY);
        GXSetNumTevStages(1);
        GXSetTevColor(GX_TEVREG0, ring->color);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
        MTXTrans(trans, 0.02f, 0.02f, 0);
        MTXScale(scale, 0.99f, 0.99f, 1);
        MTXConcat(scale, trans, texMtx);
        GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    }
}

static void Ring1MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    RingDraw(0);
}

static void Ring2MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    RingDraw(1);
}

static void Ring3MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    RingDraw(2);
}

static void Ring4MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    RingDraw(3);
}
