#include "REL/mdcardDll/mdcard.h"
#include "datadir_enum.h"

#define MDCARD_ANIM_MAX 6

typedef struct MdSprInfo_s {
    s16 animNo; //0x00
    s16 pri; //0x02
    HuVec2f pos; //0x04
    HuVec2f scale; //0x0C
    float tpLvl; //0x14
} MDSPR_INFO;

typedef struct MdSprGrpInfo_s {
    s16 start; //0x00
    s16 end; //0x02
    float posX; //0x04
    float posY; //0x08
} MDSPR_GRP_INFO;

void MdSpriteDispSet(MDSPRITE_WORK *sprite, int grpNo, BOOL dispF);
void MdModelUpdateAmb(MDMODEL_WORK *model);
static void FontBGFilter(HU3DMODEL *modelP, Mtx *mtx);
static void Layer1Hook(s16 layerNo);

static unsigned int SpriteAnimFile[MDCARD_ANIM_MAX] = {
    DATANUM(DATA_mdcard, 0x24),
    DATANUM(DATA_mdcard, 0x25),
    DATANUM(DATA_mdcard, 0x26),
    DATANUM(DATA_mdcard, 0x27),
    DATANUM(DATA_mdcard, 0x28),
    DATANUM(DATA_mdcard, 0x29),
};

static MDSPR_INFO SpriteInfo[MDSPR_MAX] = {
    { 1, 2, { 0, 0 }, { 1, 8 }, 0.6f },
    { 0, 1, { 0, -28 }, { 72, 1 }, 1.0f },
    { 0, 1, { 0, 32 }, { 72, 1 }, 1.0f },
    { 2, 4, { 0, 0 }, { 1, 1 }, 1.0f },
    { 2, 4, { 0, 0 }, { 1, 1 }, 1.0f },
    { 3, 3, { 0, 0 }, { 1, 1 }, 1.0f },
    { 3, 3, { 0, 0 }, { 1, 1 }, 1.0f },
    { 5, 5, { 0, 0 }, { 1, 1 }, 1.0f },
    { 5, 5, { 0, 0 }, { 1, 1 }, 1.0f },
    { 5, 5, { 0, 0 }, { 1, 1 }, 1.0f },
    { 5, 5, { 0, 0 }, { 1, 1 }, 1.0f },
    { 2, 6, { 64, 0 }, { 1, 1 }, 1.0f },
    { 2, 6, { -64, 0 }, { 1, 1 }, 1.0f },
};

static MDSPR_GRP_INFO SpriteGrpInfo[MDSPR_GRP_MAX] = {
    { 0, 2, 288, 408 },
    { 3, 6, 288, 240 },
    { 7, 10, 288, 240 },
    { 11, 12, 288, 240 },
};

static unsigned int PinFileTbl[21] = {
    DATANUM(DATA_mdcard, 0x03), DATANUM(DATA_mdcard, 0x04), DATANUM(DATA_mdcard, 0x05), DATANUM(DATA_mdcard, 0x06),
    DATANUM(DATA_mdcard, 0x07), DATANUM(DATA_mdcard, 0x08), DATANUM(DATA_mdcard, 0x09), DATANUM(DATA_mdcard, 0x0A),
    DATANUM(DATA_mdcard, 0x0B), DATANUM(DATA_mdcard, 0x0C), DATANUM(DATA_mdcard, 0x0D), DATANUM(DATA_mdcard, 0x0E),
    DATANUM(DATA_mdcard, 0x0F), DATANUM(DATA_mdcard, 0x10), DATANUM(DATA_mdcard, 0x11), DATANUM(DATA_mdcard, 0x12),
    DATANUM(DATA_mdcard, 0x13), DATANUM(DATA_mdcard, 0x14), DATANUM(DATA_mdcard, 0x15), DATANUM(DATA_mdcard, 0x16),
    DATANUM(DATA_mdcard, 0x17),
};

static unsigned int ManFileTbl[2] = {
    DATANUM(DATA_mdcard, 0x1E),
    DATANUM(DATA_mdcard, 0x1F),
};

static unsigned int CardFileTbl[5] = {
    DATANUM(DATA_mdcard, 0x19),
    DATANUM(DATA_mdcard, 0x1A),
    DATANUM(DATA_mdcard, 0x1B),
    DATANUM(DATA_mdcard, 0x1C),
    DATANUM(DATA_mdcard, 0x1D),
};

static unsigned int StarMotFileTbl[2] = {
    DATANUM(DATA_mdcard, 0x20),
    DATANUM(DATA_mdcard, 0x21),
};

static s16 ToueiAnmNoTbl[1] = {
    4
};

static s16 ToueiMdlNoTbl[1] = {
    32
};

static char *ToueiTexNameTbl[1] = {
    "dummy"
};

void MdSpriteCreate(MDSPRITE_WORK *sprite)
{
    HUSPRGRPID grpId;
    MDSPR_INFO *sprInfo;
    int i;
    MDSPR_GRP_INFO *grpInfo;
    int j;
    int grpNum;
    for(i=0; i<MDCARD_ANIM_MAX; i++) {
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
    grpId = sprite->grpId[1];
    HuSprBankSet(grpId, 0, 3);
    HuSprBankSet(grpId, 1, 2);
    HuSprBankSet(grpId, 2, 3);
    HuSprBankSet(grpId, 3, 2);
    HuSprAttrSet(grpId, 2, HUSPR_ATTR_ADDCOL);
    HuSprAttrSet(grpId, 3, HUSPR_ATTR_ADDCOL);
    grpId = sprite->grpId[2];
    HuSprBankSet(grpId, 0, 0);
    HuSprBankSet(grpId, 1, 1);
    HuSprBankSet(grpId, 2, 2);
    HuSprBankSet(grpId, 3, 3);
    grpId = sprite->grpId[3];
    HuSprBankSet(grpId, 0, 3);
    HuSprBankSet(grpId, 1, 2);
    MdSpriteDispSet(sprite, 0, FALSE);
    MdSpriteDispSet(sprite, 1, FALSE);
    MdSpriteDispSet(sprite, 2, FALSE);
    MdSpriteDispSet(sprite, 3, FALSE);
    sprite->fadeAlpha = 0.6f;
    HuSprExecLayerCameraSet(0x40, sysP->camera->camera[0].bit, 1);
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
    model->ambScale = 1.0f;
    for(i=0; i<16; i++) {
        model->ambId[i] = -1;
    }
    model->mdlId[0] = Hu3DModelCreateData(DATANUM(DATA_mdcard, 0x00));
    Hu3DModelAttrReset(model->mdlId[0], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[0], 2);
    Hu3DModelCameraSet(model->mdlId[0], camera->camera[1].bit);
    Hu3DModelPosSet(model->mdlId[0], 0, 0, 0);
    Hu3DModelRotSet(model->mdlId[0], 0, 0, 0);
    Hu3DModelScaleSet(model->mdlId[0], 1, 1, 1);
    Hu3DModelAttrSet(model->mdlId[0], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrSet(model->mdlId[0], HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(model->mdlId[0], 1);
    model->mdlId[1] = Hu3DModelCreateData(DATANUM(DATA_mdcard, 0x02));
    Hu3DModelAttrReset(model->mdlId[1], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[1], 0);
    Hu3DModelCameraSet(model->mdlId[1], camera->camera[0].bit);
    Hu3DModelPosSet(model->mdlId[1], 0, 0, 0);
    Hu3DModelRotSet(model->mdlId[1], 0, 0, 0);
    Hu3DModelScaleSet(model->mdlId[1], 1, 1, 1);
    Hu3DModelShadowMapObjSet(model->mdlId[1], "stage-cloth");
    model->mdlId[2] = Hu3DModelCreateData(DATANUM(DATA_mdcard, 0x01));
    Hu3DModelAttrReset(model->mdlId[2], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[2], 0);
    Hu3DModelCameraSet(model->mdlId[2], camera->camera[0].bit);
    Hu3DModelPosSet(model->mdlId[2], 0, 0, 0);
    Hu3DModelRotSet(model->mdlId[2], 0, 0, 0);
    Hu3DModelScaleSet(model->mdlId[2], 1, 1, 1);
    Hu3DModelAttrReset(model->mdlId[2], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrSet(model->mdlId[2], HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(model->mdlId[2], 1);
    Hu3DModelShadowSet(model->mdlId[2]);
    for(i=0; i<21; i++) {
        model->mdlId[3+i] = Hu3DModelCreateData(PinFileTbl[i]);
        Hu3DModelAttrReset(model->mdlId[3+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[3+i], 0);
        Hu3DModelCameraSet(model->mdlId[3+i], camera->camera[0].bit);
        Hu3DModelPosSet(model->mdlId[3+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[3+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[3+i], 1, 1, 1);
        Hu3DModelAttrSet(model->mdlId[3+i], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrSet(model->mdlId[3+i], HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(model->mdlId[3+i], 1);
        Hu3DModelShadowSet(model->mdlId[3+i]);
    }
    for(i=0; i<4; i++) {
        if(i == 0) {
            model->mdlId[24+i] = Hu3DModelCreateData(ManFileTbl[0]);
        } else {
            model->mdlId[24+i] = Hu3DModelLink(model->mdlId[24]);
        }
        Hu3DModelAttrReset(model->mdlId[24+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[24+i], 2);
        Hu3DModelCameraSet(model->mdlId[24+i], camera->camera[1].bit);
        Hu3DModelPosSet(model->mdlId[24+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[24+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[24+i], 1, 1, 1);
        Hu3DModelAttrSet(model->mdlId[24+i], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrReset(model->mdlId[24+i], HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(model->mdlId[24+i], 1);
        if(i == 0) {
            model->mdlId[28+i] = Hu3DModelCreateData(ManFileTbl[1]);
        } else {
            model->mdlId[28+i] = Hu3DModelLink(model->mdlId[28]);
        }
        Hu3DModelAttrReset(model->mdlId[28+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[28+i], 2);
        Hu3DModelCameraSet(model->mdlId[28+i], camera->camera[1].bit);
        Hu3DModelPosSet(model->mdlId[28+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[28+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[28+i], 1, 1, 1);
        Hu3DModelAttrSet(model->mdlId[28+i], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrReset(model->mdlId[28+i], HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(model->mdlId[28+i], 1);
    }
    model->mdlId[32] = Hu3DModelCreateData(DATANUM(DATA_mdcard, 0x18));
    Hu3DModelAttrReset(model->mdlId[32], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[32], 2);
    Hu3DModelCameraSet(model->mdlId[32], camera->camera[0].bit);
    Hu3DModelPosSet(model->mdlId[32], 0, 0, 0);
    Hu3DModelRotSet(model->mdlId[32], 0, 0, 0);
    Hu3DModelScaleSet(model->mdlId[32], 1, 1, 1);
    Hu3DModelAttrSet(model->mdlId[32], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(model->mdlId[32], HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(model->mdlId[32], 1);
    for(i=0; i<5; i++) {
        model->mdlId[33+i] = Hu3DModelCreateData(CardFileTbl[i]);
        Hu3DModelAttrReset(model->mdlId[33+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[33+i], 2);
        Hu3DModelCameraSet(model->mdlId[33+i], camera->camera[1].bit);
        Hu3DModelPosSet(model->mdlId[33+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[33+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[33+i], 1, 1, 1);
    }
    model->mdlId[38] = Hu3DHookFuncCreate(FontBGFilter);
    Hu3DModelLayerSet(model->mdlId[38], 6);
    Hu3DModelCameraSet(model->mdlId[38], camera->camera[0].bit);
    Hu3DModelAttrReset(model->mdlId[38], HU3D_ATTR_DISPOFF);
    for(i=0; i<2; i++) {
        model->motId[i] = Hu3DJointMotion(model->mdlId[0], HuDataSelHeapReadNum(StarMotFileTbl[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    Hu3DMotionSet(model->mdlId[0], model->motId[0]);
    for(i=0; i<21; i++) {
        model->motId[2+i] = Hu3DJointMotion(model->mdlId[3+i], HuDataSelHeapReadNum(DATANUM(DATA_mdcard, 0x22), HU_MEMNUM_OVL, HEAP_MODEL));
        model->motId[23+i] = Hu3DJointMotion(model->mdlId[3+i], HuDataSelHeapReadNum(DATANUM(DATA_mdcard, 0x23), HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DMotionSet(model->mdlId[3+i], model->motId[23+i]);
    }
    for(i=0; i<MDMODEL_MAX; i++) {
        HU3DMODEL *modelP = &Hu3DData[model->mdlId[i]];
        model->posP[i] = &modelP->pos;
        model->rotP[i] = &modelP->rot;
        model->scaleP[i] = &modelP->scale;
    }
    Hu3DLayerHookSet(9, Layer1Hook);
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

static void Layer1Hook(s16 layerNo)
{
    Hu3DZClear();
}

void MdModelAddAmb(MDMODEL_WORK *model, s16 mdlId)
{
    int i;
    for(i=0; i<16; i++) {
        if(model->ambId[i] == -1) {
            model->ambId[i] = mdlId;
            return;
        }
    }
}

void MdModelRemoveAmb(MDMODEL_WORK *model, s16 mdlId)
{
    int i;
    for(i=0; i<16; i++) {
        if(model->ambId[i] == mdlId) {
            model->ambId[i] = -1;
            return;
        }
    }
}

void MdModelUpdateAmb(MDMODEL_WORK *model)
{
    int i;
    for(i=0; i<16; i++) {
        if(model->ambId[i] != -1) {
            Hu3DModelAmbSet(model->ambId[i], model->ambScale, model->ambScale, model->ambScale);
        }
    }
}

void MdToueiMdlInit(void)
{
    MDSPRITE_WORK *sprite = sysP->sprite;
    MDMODEL_WORK *model = sysP->model;
    int i;

    for(i=0; i<1; i++) {
        HU3DANIMID animId = Hu3DAnimCreate(sprite->animP[ToueiAnmNoTbl[i]], model->mdlId[ToueiMdlNoTbl[i]], ToueiTexNameTbl[i]);
        model->animId[i] = animId;
        Hu3DAnimSpeedSet(animId, 1);
        Hu3DAnimAttrSet(animId, HU3D_ANIM_ATTR_ANIMON);
        Hu3DAnimAttrSet(animId, HU3D_ANIM_ATTR_LOOP);
    }
    Hu3DAnmNoSet(model->animId[0], 0);
}
