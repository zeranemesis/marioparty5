#include "REL/rescardDll/rescard.h"
#include "datadir_enum.h"

static void FontBGFilter(HU3DMODEL *modelP, Mtx *mtx);
static void Layer1Clear(s16 layerNo);
static void Layer2Clear(s16 layerNo);
static void MdModelAddAmb(MDMODEL_WORK *model, HU3DMODELID mdlId);
void MdModelUpdateAmb(MDMODEL_WORK *model);
void MdSpriteDispSet(MDSPRITE_WORK *sprite, int grpNo, BOOL dispF);
static void RingMatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);

typedef struct MdSprInfo_s {
    s16 animNo; //0x00
    s16 pri; //0x02
    HuVec2f pos; //0x04
    HuVec2f scale; //0x0C
    float tpLvl; //0x14
} MDSPR_INFO;

typedef struct MdSprGrpInfo_s {
    s16 first; //0x00
    s16 last; //0x02
    HuVec2f pos; //0x04
} MDSPR_GRP_INFO;

static unsigned int SpriteAnimFile[MDSPR_ANIM_MAX] = {
    DATANUM(DATA_rescard, 0x33),
    DATANUM(DATA_rescard, 0x34),
    DATANUM(DATA_rescard, 0x35),
    DATANUM(DATA_rescard, 0x36),
    DATANUM(DATA_rescard, 0x37)
};

static MDSPR_INFO SpriteInfo[MDSPR_MAX] = {
    { 1, 2, { 0, 0 }, { 1, 8 }, 0.6f },
    { 0, 1, { 0, -28 }, { 72, 1 }, 1.0f },
    { 0, 1, { 0, 32 }, { 72, 1 }, 1.0f },
    { 2, 3, { 64, 0 }, { 1, 1 }, 1.0f },
    { 2, 3, { -64, 0 }, { 1, 1 }, 1.0f },
    { 3, 4, { -20, 0 }, { 1, 1 }, 1.0f },
    { 3, 4, { 0, 0 }, { 1, 1 }, 1.0f },
    { 3, 4, { 20, 0 }, { 1, 1 }, 1.0f },
    { 3, 4, { -20, 0 }, { 1, 1 }, 1.0f },
    { 3, 4, { 0, 0 }, { 1, 1 }, 1.0f },
    { 3, 4, { 20, 0 }, { 1, 1 }, 1.0f },
    { 3, 4, { -20, 0 }, { 1, 1 }, 1.0f },
    { 3, 4, { 0, 0 }, { 1, 1 }, 1.0f },
    { 3, 4, { 20, 0 }, { 1, 1 }, 1.0f },
    { 3, 4, { -20, 0 }, { 1, 1 }, 1.0f },
    { 3, 4, { 0, 0 }, { 1, 1 }, 1.0f },
    { 3, 4, { 20, 0 }, { 1, 1 }, 1.0f },
    { 3, 5, { 0, 0 }, { 2, 2 }, 1.0f }
};

static MDSPR_GRP_INFO SpriteGrpInfo[MDSPR_GRP_MAX] = {
    { 0, 2, { 288, 408 } },
    { 3, 4, { 288, 240 } },
    { 5, 7, { 288, 240 } },
    { 8, 10, { 288, 240 } },
    { 11, 13, { 288, 240 } },
    { 14, 16, { 288, 240 } },
    { 17, 17, { 288, 240 } }
};

static unsigned int StageFileTbl[6] = {
    DATANUM(DATA_rescard, 0x1),
    DATANUM(DATA_rescard, 0x2),
    DATANUM(DATA_rescard, 0x3),
    DATANUM(DATA_rescard, 0x4),
    DATANUM(DATA_rescard, 0x5),
    DATANUM(DATA_rescard, 0x6)
};

static unsigned int PinFileTbl[21] = {
    DATANUM(DATA_rescard, 0x7),
    DATANUM(DATA_rescard, 0x8),
    DATANUM(DATA_rescard, 0x9),
    DATANUM(DATA_rescard, 0xa),
    DATANUM(DATA_rescard, 0xb),
    DATANUM(DATA_rescard, 0xc),
    DATANUM(DATA_rescard, 0xd),
    DATANUM(DATA_rescard, 0xe),
    DATANUM(DATA_rescard, 0xf),
    DATANUM(DATA_rescard, 0x10),
    DATANUM(DATA_rescard, 0x11),
    DATANUM(DATA_rescard, 0x12),
    DATANUM(DATA_rescard, 0x13),
    DATANUM(DATA_rescard, 0x14),
    DATANUM(DATA_rescard, 0x15),
    DATANUM(DATA_rescard, 0x16),
    DATANUM(DATA_rescard, 0x17),
    DATANUM(DATA_rescard, 0x18),
    DATANUM(DATA_rescard, 0x19),
    DATANUM(DATA_rescard, 0x1a),
    DATANUM(DATA_rescard, 0x1b)
};

static unsigned int CurFileTbl[7] = {
    DATANUM(DATA_rescard, 0x1c),
    DATANUM(DATA_rescard, 0x1d),
    DATANUM(DATA_rescard, 0x1e),
    DATANUM(DATA_rescard, 0x1f),
    DATANUM(DATA_rescard, 0x20),
    DATANUM(DATA_rescard, 0x21),
    DATANUM(DATA_rescard, 0x22)
};

static unsigned int PopFileTbl[2] = {
    DATANUM(DATA_rescard, 0x24),
    DATANUM(DATA_rescard, 0x25)
};

static unsigned int StarMotFileTbl[2] = {
    DATANUM(DATA_rescard, 0x2e),
    DATANUM(DATA_rescard, 0x2f)
};

static unsigned int PinMotFileTbl[3] = {
    DATANUM(DATA_rescard, 0x30),
    DATANUM(DATA_rescard, 0x31),
    DATANUM(DATA_rescard, 0x32)
};

void MdSpriteCreate(MDSPRITE_WORK *sprite)
{
    HUSPRGRPID grpId;
    MDSPR_INFO *info;
    int i;
    MDSPR_GRP_INFO *grpInfo;
    int j;
    int sprNum;

    for(i=0; i<MDSPR_ANIM_MAX; i++) {
        sprite->animP[i] = HuSprAnimRead(HuDataReadNum(SpriteAnimFile[i], HU_MEMNUM_OVL));
    }
    for(i=0; i<MDSPR_MAX; i++) {
        info = &SpriteInfo[i];
        sprite->sprId[i] = HuSprCreate(sprite->animP[info->animNo], 0, 0);
    }
    grpInfo = SpriteGrpInfo;
    for(i=0; i<MDSPR_GRP_MAX; i++, grpInfo++) {
        sprNum = (grpInfo->last-grpInfo->first)+1;
        grpId = HuSprGrpCreate(sprNum);
        sprite->grpId[i] = grpId;
        info = &SpriteInfo[grpInfo->first];
        for(j=0; j<sprNum; j++, info++) {
            HuSprGrpMemberSet(grpId, j, sprite->sprId[grpInfo->first+j]);
            HuSprPriSet(grpId, j, info->pri);
            HuSprAttrReset(grpId, j, HUSPR_ATTR_DISPOFF);
            HuSprPosSet(grpId, j, info->pos.x, info->pos.y);
            HuSprZRotSet(grpId, j, 0);
            HuSprScaleSet(grpId, j, info->scale.x, info->scale.y);
            HuSprColorSet(grpId, j, 255, 255, 255);
            HuSprTPLvlSet(grpId, j, info->tpLvl);
        }
        HuSprGrpPosSet(grpId, grpInfo->pos.x, grpInfo->pos.y);
        HuSprGrpCenterSet(grpId, 0, 0);
        HuSprGrpZRotSet(grpId, 0);
        HuSprGrpScaleSet(grpId, 1, 1);
        HuSprGrpDrawNoSet(grpId, 0);
        HuSprGrpScissorSet(grpId, 0, 0, 640, 480);
    }
    HuSprBankSet(sprite->grpId[1], 0, 3);
    HuSprBankSet(sprite->grpId[1], 1, 2);
    HuSprBankSet(sprite->grpId[2], 0, 10);
    HuSprBankSet(sprite->grpId[2], 1, 1);
    HuSprBankSet(sprite->grpId[2], 2, 2);
    HuSprGrpDrawNoSet(sprite->grpId[2], 64);
    HuSprBankSet(sprite->grpId[3], 0, 10);
    HuSprBankSet(sprite->grpId[3], 1, 1);
    HuSprBankSet(sprite->grpId[3], 2, 2);
    HuSprGrpDrawNoSet(sprite->grpId[3], 64);
    HuSprBankSet(sprite->grpId[4], 0, 10);
    HuSprBankSet(sprite->grpId[4], 1, 1);
    HuSprBankSet(sprite->grpId[4], 2, 2);
    HuSprGrpDrawNoSet(sprite->grpId[4], 64);
    HuSprBankSet(sprite->grpId[5], 0, 10);
    HuSprBankSet(sprite->grpId[5], 1, 1);
    HuSprBankSet(sprite->grpId[5], 2, 2);
    HuSprGrpDrawNoSet(sprite->grpId[5], 64);
    HuSprBankSet(sprite->grpId[6], 0, 0);
    MdSpriteDispSet(sprite, 0, FALSE);
    MdSpriteDispSet(sprite, 1, FALSE);
    MdSpriteDispSet(sprite, 2, FALSE);
    MdSpriteDispSet(sprite, 3, FALSE);
    MdSpriteDispSet(sprite, 4, FALSE);
    MdSpriteDispSet(sprite, 5, FALSE);
    MdSpriteDispSet(sprite, 6, FALSE);
    sprite->fadeAlpha = 0.6f;
    HuSprExecLayerCameraSet(64, sysP->camera->camera[1].bit, 3);
}

void MdSpriteDispSet(MDSPRITE_WORK *sprite, int grpNo, BOOL dispF)
{
    MDSPR_GRP_INFO *grpInfo = &SpriteGrpInfo[grpNo];
    HUSPRGRPID grpId = sprite->grpId[grpNo];
    int sprNum = (grpInfo->last-grpInfo->first)+1;
    int i;

    for(i=0; i<sprNum; i++) {
        if(dispF == FALSE) {
            HuSprAttrSet(grpId, i, HUSPR_ATTR_DISPOFF);
        } else if(dispF == TRUE) {
            HuSprAttrReset(grpId, i, HUSPR_ATTR_DISPOFF);
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

static void RingMatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    GXColor color = { 0, 255, 255, 64 };
    Mtx trans;
    Mtx scale;
    Mtx texMtx;

    GXSetColorUpdate(TRUE);
    GXSetAlphaUpdate(TRUE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_DSTALPHA, GX_LO_INVCOPY);
    GXSetNumTevStages(1);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
    MTXTrans(trans, 0, 0, 0);
    MTXScale(scale, 1, 1, 1);
    MTXConcat(scale, trans, texMtx);
    GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
}

void MdModelCreate(MDMODEL_WORK *model, MDCAMERA_WORK *camera)
{
    struct {
        s32 pinNo;
        s32 pinVar;
    } player[4];
    int i;

    for(i=0; i<4; i++) {
        player[i].pinNo = sysP->score->player[i].pinNo;
        player[i].pinVar = sysP->score->player[i].pinVar;
    }
    model->ambScale = 1.0f;
    for(i=0; i<MDMODEL_AMB_MAX; i++) {
        model->ambId[i] = -1;
    }
    model->mdlId[MDMODEL_BG] = Hu3DModelCreateData(DATANUM(DATA_rescard, 0x0));
    Hu3DModelAttrReset(model->mdlId[MDMODEL_BG], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[MDMODEL_BG], 6);
    Hu3DModelCameraSet(model->mdlId[MDMODEL_BG], camera->camera[1].bit);
    Hu3DModelPosSet(model->mdlId[MDMODEL_BG], 0, 0, 0);
    Hu3DModelRotSet(model->mdlId[MDMODEL_BG], 0, 0, 0);
    Hu3DModelScaleSet(model->mdlId[MDMODEL_BG], 1, 1, 1);
    Hu3DModelAttrSet(model->mdlId[MDMODEL_BG], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(model->mdlId[MDMODEL_BG], HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(model->mdlId[MDMODEL_BG], 1.0f);
    for(i=0; i<6; i++) {
        model->mdlId[MDMODEL_STAGE+i] = Hu3DModelCreateData(StageFileTbl[i]);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_STAGE+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_STAGE+i], 0);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_STAGE+i], camera->camera[0].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_STAGE+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_STAGE+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_STAGE+i], 1, 1, 1);
        MdModelAddAmb(model, model->mdlId[MDMODEL_STAGE+i]);
    }
    for(i=0; i<4; i++) {
        model->mdlId[MDMODEL_PIN+i] = Hu3DModelCreateData(PinFileTbl[player[i].pinVar+(player[i].pinNo*3)]);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_PIN+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_PIN+i], 1);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_PIN+i], camera->camera[0].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_PIN+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_PIN+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_PIN+i], 1, 1, 1);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_PIN+i], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_PIN+i], HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(model->mdlId[MDMODEL_PIN+i], 1.0f);
        MdModelAddAmb(model, model->mdlId[MDMODEL_PIN+i]);
    }
    for(i=0; i<4; i++) {
        model->mdlId[MDMODEL_CUR+i] = Hu3DModelCreateData(CurFileTbl[player[i].pinNo]);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_CUR+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_CUR+i], 5);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_CUR+i], camera->camera[1].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_CUR+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_CUR+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_CUR+i], 1, 1, 1);
    }
    for(i=0; i<4; i++) {
        if(i == 0) {
            model->mdlId[MDMODEL_SPOT+i] = Hu3DModelCreateData(DATANUM(DATA_rescard, 0x23));
        } else {
            model->mdlId[MDMODEL_SPOT+i] = Hu3DModelLink(model->mdlId[MDMODEL_SPOT]);
        }
        Hu3DModelAttrReset(model->mdlId[MDMODEL_SPOT+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_SPOT+i], 1);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_SPOT+i], camera->camera[0].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_SPOT+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_SPOT+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_SPOT+i], 1, 1, 1);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_SPOT+i], HU3D_MOTATTR_PAUSE);
        MdModelAddAmb(model, model->mdlId[MDMODEL_SPOT+i]);
    }
    for(i=0; i<2; i++) {
        model->mdlId[MDMODEL_POP+i] = Hu3DModelCreateData(PopFileTbl[i]);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_POP+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_POP+i], 6);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_POP+i], camera->camera[1].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_POP+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_POP+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_POP+i], 1, 1, 1);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_POP+i], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_POP+i], HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(model->mdlId[MDMODEL_POP+i], 1.0f);
    }
    for(i=0; i<22; i++) {
        model->mdlId[MDMODEL_POP_A+i] = Hu3DModelLink(model->mdlId[MDMODEL_POP]);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_POP_A+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_POP_A+i], 1);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_POP_A+i], camera->camera[0].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_POP_A+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_POP_A+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_POP_A+i], 1, 1, 1);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_POP_A+i], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_POP_A+i], HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(model->mdlId[MDMODEL_POP_A+i], 1.0f);
    }
    for(i=0; i<11; i++) {
        model->mdlId[MDMODEL_POP_B+i] = Hu3DModelLink(model->mdlId[MDMODEL_POP+1]);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_POP_B+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_POP_B+i], 1);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_POP_B+i], camera->camera[0].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_POP_B+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_POP_B+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_POP_B+i], 1, 1, 1);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_POP_B+i], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_POP_B+i], HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(model->mdlId[MDMODEL_POP_B+i], 1.0f);
    }
    for(i=0; i<4; i++) {
        if(i == 0) {
            model->mdlId[MDMODEL_BILL_A+i] = Hu3DModelCreateData(DATANUM(DATA_rescard, 0x26));
        } else {
            model->mdlId[MDMODEL_BILL_A+i] = Hu3DModelLink(model->mdlId[MDMODEL_BILL_A]);
        }
        Hu3DModelAttrReset(model->mdlId[MDMODEL_BILL_A+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_BILL_A+i], 5);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_BILL_A+i], camera->camera[0].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_BILL_A+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_BILL_A+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_BILL_A+i], 1, 1, 1);
        if(i == 0) {
            model->mdlId[MDMODEL_BILL_B+i] = Hu3DModelCreateData(DATANUM(DATA_rescard, 0x27));
        } else {
            model->mdlId[MDMODEL_BILL_B+i] = Hu3DModelLink(model->mdlId[MDMODEL_BILL_B]);
        }
        Hu3DModelAttrReset(model->mdlId[MDMODEL_BILL_B+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_BILL_B+i], 5);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_BILL_B+i], camera->camera[0].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_BILL_B+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_BILL_B+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_BILL_B+i], 1, 1, 1);
        if(i == 0) {
            model->mdlId[MDMODEL_BILL_C+i] = Hu3DModelCreateData(DATANUM(DATA_rescard, 0x28));
        } else {
            model->mdlId[MDMODEL_BILL_C+i] = Hu3DModelLink(model->mdlId[MDMODEL_BILL_C]);
        }
        Hu3DModelAttrReset(model->mdlId[MDMODEL_BILL_C+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_BILL_C+i], 4);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_BILL_C+i], camera->camera[1].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_BILL_C+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_BILL_C+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_BILL_C+i], 1, 1, 1);
    }
    for(i=0; i<8; i++) {
        if(i == 0) {
            model->mdlId[MDMODEL_BILL_D+i] = Hu3DModelLink(model->mdlId[MDMODEL_BILL_A]);
        } else {
            model->mdlId[MDMODEL_BILL_D+i] = Hu3DModelLink(model->mdlId[MDMODEL_BILL_B]);
        }
        Hu3DModelAttrReset(model->mdlId[MDMODEL_BILL_D+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_BILL_D+i], 5);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_BILL_D+i], camera->camera[1].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_BILL_D+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_BILL_D+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_BILL_D+i], 1, 1, 1);
    }
    model->mdlId[MDMODEL_E74] = Hu3DModelCreateData(DATANUM(DATA_rescard, 0x29));
    Hu3DModelAttrReset(model->mdlId[MDMODEL_E74], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[MDMODEL_E74], 6);
    Hu3DModelCameraSet(model->mdlId[MDMODEL_E74], camera->camera[1].bit);
    Hu3DModelPosSet(model->mdlId[MDMODEL_E74], 0, 0, 0);
    Hu3DModelRotSet(model->mdlId[MDMODEL_E74], 0, 0, 0);
    Hu3DModelScaleSet(model->mdlId[MDMODEL_E74], 1, 1, 1);
    Hu3DModelAttrSet(model->mdlId[MDMODEL_E74], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(model->mdlId[MDMODEL_E74], HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(model->mdlId[MDMODEL_E74], 1.0f);
    for(i=0; i<4; i++) {
        if(i == 0) {
            model->mdlId[MDMODEL_E75+i] = Hu3DModelCreateData(DATANUM(DATA_rescard, 0x2a));
        } else {
            model->mdlId[MDMODEL_E75+i] = Hu3DModelLink(model->mdlId[MDMODEL_E75]);
        }
        Hu3DModelAttrReset(model->mdlId[MDMODEL_E75+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_E75+i], 5);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_E75+i], camera->camera[1].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_E75+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_E75+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_E75+i], 1, 1, 1);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_E75+i], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_E75+i], HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(model->mdlId[MDMODEL_E75+i], 1.0f);
    }
    for(i=0; i<4; i++) {
        if(i == 0) {
            model->mdlId[MDMODEL_E79+i] = Hu3DModelCreateData(DATANUM(DATA_rescard, 0x2b));
        } else {
            model->mdlId[MDMODEL_E79+i] = Hu3DModelLink(model->mdlId[MDMODEL_E79]);
        }
        Hu3DModelAttrReset(model->mdlId[MDMODEL_E79+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_E79+i], 0);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_E79+i], camera->camera[0].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_E79+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_E79+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_E79+i], 1, 1, 1);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_E79+i], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_E79+i], HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(model->mdlId[MDMODEL_E79+i], 1.0f);
    }
    model->mdlId[MDMODEL_RING] = Hu3DModelCreateData(DATANUM(DATA_rescard, 0x2c));
    Hu3DModelAttrSet(model->mdlId[MDMODEL_RING], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[MDMODEL_RING], 1);
    Hu3DModelCameraSet(model->mdlId[MDMODEL_RING], camera->camera[0].bit);
    Hu3DModelPosSet(model->mdlId[MDMODEL_RING], 0, 0, 0);
    Hu3DModelRotSet(model->mdlId[MDMODEL_RING], 0, 0, 0);
    Hu3DModelScaleSet(model->mdlId[MDMODEL_RING], 1, 1, 1);
    Hu3DModelMatHookSet(model->mdlId[MDMODEL_RING], RingMatHook);
    for(i=0; i<4; i++) {
        if(i == 0) {
            model->mdlId[MDMODEL_E84+i] = Hu3DModelCreateData(DATANUM(DATA_rescard, 0x2d));
        } else {
            model->mdlId[MDMODEL_E84+i] = Hu3DModelLink(model->mdlId[MDMODEL_E84]);
        }
        Hu3DModelAttrSet(model->mdlId[MDMODEL_E84+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_E84+i], 2);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_E84+i], camera->camera[0].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_E84+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_E84+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_E84+i], 1, 1, 1);
    }
    model->mdlId[MDMODEL_FONTBGFILTER] = Hu3DHookFuncCreate(FontBGFilter);
    Hu3DModelLayerSet(model->mdlId[MDMODEL_FONTBGFILTER], 7);
    Hu3DModelCameraSet(model->mdlId[MDMODEL_FONTBGFILTER], camera->camera[1].bit);
    Hu3DModelAttrReset(model->mdlId[MDMODEL_FONTBGFILTER], HU3D_ATTR_DISPOFF);
    for(i=0; i<2; i++) {
        model->motId[i] = Hu3DJointMotionData(model->mdlId[MDMODEL_BG], StarMotFileTbl[i]);
    }
    Hu3DMotionSet(model->mdlId[MDMODEL_BG], model->motId[0]);
    for(i=0; i<4; i++) {
        model->motId[2+i] = Hu3DJointMotionData(model->mdlId[MDMODEL_PIN+i], PinMotFileTbl[0]);
        model->motId[6+i] = Hu3DJointMotionData(model->mdlId[MDMODEL_PIN+i], PinMotFileTbl[1]);
        model->motId[10+i] = Hu3DJointMotionData(model->mdlId[MDMODEL_PIN+i], PinMotFileTbl[2]);
        Hu3DMotionSet(model->mdlId[MDMODEL_PIN+i], model->motId[6+i]);
    }
    for(i=0; i<MDMODEL_MAX; i++) {
        HU3DMODEL *modelP = &Hu3DData[model->mdlId[i]];
        model->posP[i] = &modelP->pos;
        model->rotP[i] = &modelP->rot;
        model->scaleP[i] = &modelP->scale;
    }
    Hu3DLayerHookSet(1, Layer1Clear);
    Hu3DLayerHookSet(2, Layer1Clear);
    Hu3DLayerHookSet(4, Layer2Clear);
    Hu3DLayerHookSet(5, Layer2Clear);
    Hu3DLayerHookSet(6, Layer2Clear);
    MdModelUpdateAmb(model);
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

static void Layer1Clear(s16 layerNo)
{
    if(Hu3DCameraNo == 0) {
        Hu3DZClear();
    }
}

static void Layer2Clear(s16 layerNo)
{
    if(Hu3DCameraNo == 1) {
        Hu3DZClear();
    }
}

static void MdModelAddAmb(MDMODEL_WORK *model, HU3DMODELID mdlId)
{
    int i;

    for(i=0; i<MDMODEL_AMB_MAX; i++) {
        if(model->ambId[i] == -1) {
            model->ambId[i] = mdlId;
            return;
        }
    }
}

void MdModelRemoveAmb(MDMODEL_WORK *model, HU3DMODELID mdlId)
{
    int i;

    for(i=0; i<MDMODEL_AMB_MAX; i++) {
        if(model->ambId[i] == mdlId) {
            model->ambId[i] = -1;
            return;
        }
    }
}

void MdModelUpdateAmb(MDMODEL_WORK *model)
{
    int i;

    for(i=0; i<MDMODEL_AMB_MAX; i++) {
        if(model->ambId[i] != -1) {
            Hu3DModelAmbSet(model->ambId[i], model->ambScale, model->ambScale, model->ambScale);
        }
    }
}
