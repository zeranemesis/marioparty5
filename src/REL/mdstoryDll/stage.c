#include "REL/mdstoryDll.h"
#include "datadir_enum.h"
#include "math.h"

#define MDSTORY_DATA(n) DATANUM(DATA_mdstory, n)

typedef struct MdSprInfo_s {
    s16 animNo; //0x00
    s16 pri; //0x02
    HuVec2f pos; //0x04
    HuVec2f scale; //0xC
    float tpLvl; //0x14
} MDSPR_INFO;

typedef struct MdSprGrpInfo_s {
    s16 start; //0x00
    s16 end; //0x02
    float posX; //0x04
    float posY; //0x08
} MDSPR_GRP_INFO;

static void Touei1MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Touei2MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Touei3MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Touei4MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Touei5MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Touei6MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Touei7MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void RingMatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);

static unsigned int SpriteAnimFile[MDSPR_ANIM_MAX] = {
    MDSTORY_DATA(0x2D),
    MDSTORY_DATA(0x2E),
    MDSTORY_DATA(0x2B),
    MDSTORY_DATA(0x2C),
    MDSTORY_DATA(0x31),
    MDSTORY_DATA(0x32),
    MDSTORY_DATA(0x33),
    MDSTORY_DATA(0x2F),
    MDSTORY_DATA(0x30),
    MDSTORY_DATA(0x34)
};

static MDSPR_INFO SpriteInfo[MDSPR_MAX] = {
    { 0, 2, { 0, -28 }, { 72, 1 }, 1.0f },
    { 0, 2, { 0, 32 }, { 72, 1 }, 1.0f },
    { 1, 3, { 0, 0 }, { 1, 8 }, 0.6f },
    { 2, 7, { 64, 0 }, { 1, 1 }, 1.0f },
    { 2, 7, { -64, 0 }, { 1, 1 }, 1.0f },
    { 2, 5, { 64, 0 }, { 1, 1 }, 1.0f },
    { 2, 5, { -64, 0 }, { 1, 1 }, 1.0f },
    { 2, 5, { 64, 0 }, { 1, 1 }, 1.0f },
    { 2, 5, { -64, 0 }, { 1, 1 }, 1.0f },
    { 2, 4, { 64, 0 }, { 1, 1 }, 1.0f },
    { 2, 4, { -64, 0 }, { 1, 1 }, 1.0f },
    { 2, 4, { 64, 0 }, { 1, 1 }, 1.0f },
    { 2, 4, { -64, 0 }, { 1, 1 }, 1.0f },
    { 4, 6, { 0, 0 }, { 1, 1 }, 1.0f },
    { 9, 8, { 0, 0 }, { 1, 1 }, 1.0f },
};

static MDSPR_GRP_INFO SpriteGrpInfo[MDSPR_GRP_MAX] = {
    { 0, 2, 288, 408 },
    { 3, 4, 288, 240 },
    { 5, 12, 288, 240 },
    { 13, 13, 288, 240 },
    { 14, 14, 288, 240 },
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

static HU3DMATHOOK ToueiMatHookTbl[7] = {
    Touei1MatHook,
    Touei2MatHook,
    Touei3MatHook,
    Touei4MatHook,
    Touei5MatHook,
    Touei6MatHook,
    Touei7MatHook
};

static HU3DMATHOOK RingMatHookTbl = RingMatHook;

static unsigned int CharMdlTbl[7] = {
    MDSTORY_DATA(0),
    MDSTORY_DATA(4),
    MDSTORY_DATA(8),
    MDSTORY_DATA(12),
    MDSTORY_DATA(16),
    MDSTORY_DATA(20),
    MDSTORY_DATA(24)
};

static HuVecF CharMdlPosTbl[7] = {
    { -250, 0, 0 },
    { -125, 0, 0 },
    { 0, 0, 0 },
    { 125, 0, 0 },
    { 250, 0, 0 },
    { -250, -250, 0 },
    { -125, -250, 0 }
};

static unsigned int ToueiMdlTbl[5] = {
    MDSTORY_DATA(0x23),
    MDSTORY_DATA(0x24),
    MDSTORY_DATA(0x25),
    MDSTORY_DATA(0x26),
    MDSTORY_DATA(0x27)
};

static unsigned int StarMotTbl[3] = {
    MDSTORY_DATA(0x28),
    MDSTORY_DATA(0x29),
    MDSTORY_DATA(0x2A)
};

static unsigned int CharMotTbl[21] = {
    MDSTORY_DATA(1), MDSTORY_DATA(2), MDSTORY_DATA(3),
    MDSTORY_DATA(5), MDSTORY_DATA(6), MDSTORY_DATA(7),
    MDSTORY_DATA(9), MDSTORY_DATA(10), MDSTORY_DATA(11),
    MDSTORY_DATA(13), MDSTORY_DATA(14), MDSTORY_DATA(15),
    MDSTORY_DATA(17), MDSTORY_DATA(18), MDSTORY_DATA(19),
    MDSTORY_DATA(21), MDSTORY_DATA(22), MDSTORY_DATA(23),
    MDSTORY_DATA(25), MDSTORY_DATA(26), MDSTORY_DATA(27)
};

static s16 ToueiAnmNoTbl[2] = { 5, 6 };
static s16 ToueiMdlNoTbl[2] = { 23, 22 };
static char *ToueiTexNameTbl[2] = { "dummy", "IA8_level" };

static HuVecF ToueiTexObjOfs[7] = {
    { 0, 15, 0 },
    { 0, 18, 0 },
    { 0, 12, 0 },
    { 0, 20, 0 },
    { 0, 15, 0 },
    { 0, 6, 0 },
    { 0, -5, 0 }
};

static char *ToueiTexObjHook[7] = {
    "root_head",
    "root_head",
    "ske_head",
    "ske_head",
    "ske_head",
    "ske_head",
    "ske_head"
};

void MdSpriteCreate(MDSPRITE_WORK *sprite)
{
    HUSPRGRPID grpId;
    MDSPR_INFO *sprInfo;
    int i;
    MDSPR_GRP_INFO *grpInfo;
    int j;
    int grpNum;
    for(i=0; i<MDSPR_ANIM_MAX; i++) {
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
    grpId = sprite->grpId[3];
    HuSprBankSet(grpId, 0, 0);
    MdSpriteDispSet(sprite, 0, FALSE);
    MdSpriteDispSet(sprite, 1, FALSE);
    MdSpriteDispSet(sprite, 2, FALSE);
    MdSpriteDispSet(sprite, 3, FALSE);
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

static void FontBGFilter(HU3DMODEL *modelP, Mtx *mtx);
static void LayerPostHook(s16 layerNo);
static void Layer2Hook(s16 layerNo);
static void Layer1Hook(s16 layerNo);

void MdModelCreate(MDMODEL_WORK *model, MDCAMERA_WORK *camera)
{
    int i;
    model->mdlId[MDMODEL_STAGE_CHR] = Hu3DModelCreate(HuDataSelHeapReadNum(MDSTORY_DATA(0x1E), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(model->mdlId[MDMODEL_STAGE_CHR], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[MDMODEL_STAGE_CHR], 0);
    Hu3DModelCameraSet(model->mdlId[MDMODEL_STAGE_CHR], camera->camera[MDCAMERA_BG].bit);
    Hu3DModelPosSet(model->mdlId[MDMODEL_STAGE_CHR], 0, -300, 0);
    Hu3DModelRotSet(model->mdlId[MDMODEL_STAGE_CHR], 0, 0, 0);
    Hu3DModelScaleSet(model->mdlId[MDMODEL_STAGE_CHR], 2, 2, 1);
    Hu3DModelAttrSet(model->mdlId[MDMODEL_STAGE_CHR], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(model->mdlId[MDMODEL_STAGE_CHR], HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(model->mdlId[MDMODEL_STAGE_CHR], 0.5f);
    for(i=0; i<MDSTORY_CHR_MAX; i++) {
        model->mdlId[MDMODEL_MARIO+i] = Hu3DModelCreate(HuDataSelHeapReadNum(CharMdlTbl[i], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(model->mdlId[MDMODEL_MARIO+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_MARIO+i], 1);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_MARIO+i], camera->camera[MDCAMERA_BG].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_MARIO+i], CharMdlPosTbl[i].x, CharMdlPosTbl[i].y, CharMdlPosTbl[i].z);
        Hu3DModelRotSet(model->mdlId[MDMODEL_MARIO+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_MARIO+i], 0.8f, 0.8f, 0.8f);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_MARIO+i], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_MARIO+i], HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(model->mdlId[MDMODEL_MARIO+i], 1.0f);
    }
    model->mdlId[MDMODEL_STAGE] = Hu3DModelCreate(HuDataSelHeapReadNum(MDSTORY_DATA(0x1D), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(model->mdlId[MDMODEL_STAGE], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[MDMODEL_STAGE], 0);
    Hu3DModelCameraSet(model->mdlId[MDMODEL_STAGE], camera->camera[MDCAMERA_MAIN].bit);
    Hu3DModelPosSet(model->mdlId[MDMODEL_STAGE], 0, 0, 0);
    Hu3DModelRotSet(model->mdlId[MDMODEL_STAGE], 0, 0, 0);
    Hu3DModelScaleSet(model->mdlId[MDMODEL_STAGE], 1, 1, 1);
    Hu3DModelAttrSet(model->mdlId[MDMODEL_STAGE], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(model->mdlId[MDMODEL_STAGE], HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(model->mdlId[MDMODEL_STAGE], 1.0f);
    for(i=0; i<MDSTORY_CHR_MAX; i++) {
        if(i == 0) {
            model->mdlId[MDMODEL_CAP_CHR_00+i] = Hu3DModelCreate(HuDataSelHeapReadNum(MDSTORY_DATA(0x1F), HU_MEMNUM_OVL, HEAP_MODEL));
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
    for(i=0; i<MDSTORY_PACK_MAX; i++) {
        model->mdlId[MDMODEL_CAP_PACK0+i] = Hu3DModelCreate(HuDataSelHeapReadNum(ToueiMdlTbl[i], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrReset(model->mdlId[MDMODEL_CAP_PACK0+i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(model->mdlId[MDMODEL_CAP_PACK0+i], 1);
        Hu3DModelCameraSet(model->mdlId[MDMODEL_CAP_PACK0+i], camera->camera[MDCAMERA_MAIN].bit);
        Hu3DModelPosSet(model->mdlId[MDMODEL_CAP_PACK0+i], 0, 0, 0);
        Hu3DModelRotSet(model->mdlId[MDMODEL_CAP_PACK0+i], 0, 0, 0);
        Hu3DModelScaleSet(model->mdlId[MDMODEL_CAP_PACK0+i], 1, 1, 1);
        Hu3DModelAttrSet(model->mdlId[MDMODEL_CAP_PACK0+i], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrReset(model->mdlId[MDMODEL_CAP_PACK0+i], HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(model->mdlId[MDMODEL_CAP_PACK0+i], 1.0f);
    }
    model->mdlId[MDMODEL_CAP_LV] = Hu3DModelCreate(HuDataSelHeapReadNum(MDSTORY_DATA(0x22), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrReset(model->mdlId[MDMODEL_CAP_LV], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[MDMODEL_CAP_LV], 3);
    Hu3DModelCameraSet(model->mdlId[MDMODEL_CAP_LV], camera->camera[MDCAMERA_MAIN].bit);
    Hu3DModelPosSet(model->mdlId[MDMODEL_CAP_LV], 0, 0, 0);
    Hu3DModelRotSet(model->mdlId[MDMODEL_CAP_LV], 0, 0, 0);
    Hu3DModelScaleSet(model->mdlId[MDMODEL_CAP_LV], 1, 1, 1);
    model->mdlId[MDMODEL_FINGER_1P] = Hu3DModelCreate(HuDataSelHeapReadNum(MDSTORY_DATA(0x20), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(model->mdlId[MDMODEL_FINGER_1P], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[MDMODEL_FINGER_1P], 3);
    Hu3DModelCameraSet(model->mdlId[MDMODEL_FINGER_1P], camera->camera[MDCAMERA_MAIN].bit);
    Hu3DModelPosSet(model->mdlId[MDMODEL_FINGER_1P], 0, 0, 0);
    Hu3DModelRotSet(model->mdlId[MDMODEL_FINGER_1P], 0, 0, 0);
    Hu3DModelScaleSet(model->mdlId[MDMODEL_FINGER_1P], 1, 1, 1);
    Hu3DModelAttrSet(model->mdlId[MDMODEL_FINGER_1P], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(model->mdlId[MDMODEL_FINGER_1P], HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(model->mdlId[MDMODEL_FINGER_1P], 1.0f);
    model->mdlId[MDMODEL_RING_1P] = Hu3DModelCreate(HuDataSelHeapReadNum(MDSTORY_DATA(0x21), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(model->mdlId[MDMODEL_RING_1P], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[MDMODEL_RING_1P], 2);
    Hu3DModelCameraSet(model->mdlId[MDMODEL_RING_1P], camera->camera[MDCAMERA_MAIN].bit);
    Hu3DModelPosSet(model->mdlId[MDMODEL_RING_1P], 0, 0, 0);
    Hu3DModelRotSet(model->mdlId[MDMODEL_RING_1P], 0, 0, 0);
    Hu3DModelScaleSet(model->mdlId[MDMODEL_RING_1P], 1, 1, 1);
    Hu3DModelMatHookSet(model->mdlId[MDMODEL_RING_1P], RingMatHookTbl);
    model->mdlId[MDMODEL_STAR] = Hu3DModelCreate(HuDataSelHeapReadNum(MDSTORY_DATA(0x1C), HU_MEMNUM_OVL, HEAP_MODEL));
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
        model->animId[i] = Hu3DJointMotion(model->mdlId[MDMODEL_STAR], HuDataSelHeapReadNum(StarMotTbl[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    Hu3DMotionSet(model->mdlId[MDMODEL_STAR], model->animId[0]);
    for(i=0; i<21; i++) {
        model->animId[i+3] = Hu3DJointMotion(model->mdlId[MDMODEL_MARIO+(i/3)], HuDataSelHeapReadNum(CharMotTbl[i], HU_MEMNUM_OVL, HEAP_MODEL));
        if((i%3) == 0) {
            Hu3DMotionSet(model->mdlId[MDMODEL_MARIO+(i/3)], model->animId[i+3]);
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
        for(i=0; i<MDSTORY_CHR_MAX; i++) {
            TexBufFBCopy(i);
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
    TexBufSet(0);
}

static void Touei2MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    TexBufSet(1);
}

static void Touei3MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    TexBufSet(2);
}

static void Touei4MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    TexBufSet(3);
}

static void Touei5MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    TexBufSet(4);
}

static void Touei6MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    TexBufSet(5);
}

static void Touei7MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    TexBufSet(6);
}

void MdToueiMdlInit(void)
{
    MDSPRITE_WORK *sprite = sysP->sprite;
    MDMODEL_WORK *model = sysP->model;
    int i;
    for(i=0; i<2; i++) {
        HU3DANIMID animId = Hu3DAnimCreate(sprite->animP[ToueiAnmNoTbl[i]], model->mdlId[ToueiMdlNoTbl[i]], ToueiTexNameTbl[i]);
        model->toueiAnimId[i] = animId;
        Hu3DAnimSpeedSet(animId, 1.0f);
        Hu3DAnimAttrSet(animId, HU3D_ANIM_ATTR_ANIMON);
        Hu3DAnimAttrSet(animId, HU3D_ANIM_ATTR_LOOP);
    }
    Hu3DAnmNoSet(model->toueiAnimId[0], 0);
    Hu3DAnmNoSet(model->toueiAnimId[1], 0);
}

void MdToueiTexBufInit(void)
{
    MDTEXBUF_WORK *texbuf = sysP->texbuf;
    MDMODEL_WORK *model = sysP->model;
    int i;
    for(i=0; i<MDSTORY_CHR_MAX; i++) {
        MDTEXBUF_UNIT *unit = &texbuf->unit[i];
        unit->w = 64;
        unit->h = 64;
        unit->texFmt = GX_TF_RGB565;
        MdTexBufObjPosSet(unit, model->mdlId[i+MDMODEL_MARIO], ToueiTexObjHook[i], sysP->camera->camera[MDCAMERA_BG].bit, &ToueiTexObjOfs[i]);
        unit->addr = HuMemDirectMallocNum(HEAP_MODEL, GXGetTexBufferSize(unit->w, unit->h, unit->texFmt, GX_FALSE, 0), HU_MEMNUM_OVL);
        if(!unit->addr) {
            OSReport("sys->texbuf->unit[MDSTORY_TEXBUF_MIRROR + %d].addr = NULL...\n", i);
        }
        GXInitTexObj(&unit->texObj, unit->addr, unit->w, unit->h, unit->texFmt, GX_CLAMP, GX_CLAMP, GX_FALSE);
        GXInitTexObjLOD(&unit->texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    }
}

static void TexBufFBCopy(int unitNo)
{
    MDTEXBUF_UNIT *unit = &sysP->texbuf->unit[unitNo];
    Hu3DFbCopyExec(unit->posX, unit->posY, unit->w, unit->h, unit->texFmt, GX_FALSE, unit->addr);
}

void MdTexBufObjPosSet(MDTEXBUF_UNIT *unit, HU3DMODELID modelId, char *objName, u16 cameraBit, HuVecF *ofsP)
{
    HuVecF pos3D;
    HuVecF pos2D;
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
    HuVecF pos2D;
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

void MdRingCreate(void)
{
    MDRING_WORK *ringWork = sysP->ring;
    MDMODEL_WORK *model = sysP->model;
    MDRING *ring;
    int i;
    for(ring=&ringWork->ring[0], i=0; i<MDSTORY_RING_MAX; i++, ring++) {
        ring->disp = FALSE;
        ring->mode = 0;
        ring->mdlNo = i+MDMODEL_RING_1P;
        ring->baseMdlNo = -1;
        ring->posP = &model->posP[ring->mdlNo];
        ring->rotP = &model->rotP[ring->mdlNo];
        ring->scaleP = &model->scaleP[ring->mdlNo];
        MdRingMdlSet(i, -1);
        MdRingOfsSet(i, 0, 0, 0);
        if(i <= 0) {
            ring->color = RingColorTbl[i];
        }
        MdRingColorReset(i);
    }
    ringWork->scale = 0.5f;
}

void MdRingMain(void)
{
    MDRING *ring;
    int i;
    HuVecF *pos;
    HuVecF *basePos;
    int colorNo;
    MDRING_WORK *ringWork = sysP->ring;
    MDMODEL_WORK *model = sysP->model;
    MDCAMERA_WORK *cameraWork = sysP->camera;
    HuVecF *rot;
    HuVecF *scale;
    MDCAMERA *camera = &cameraWork->camera[MDCAMERA_MAIN];
    for(ring=&ringWork->ring[0], i=0; i<MDSTORY_RING_MAX; i++, ring++) {
        pos = *ring->posP;
        rot = *ring->rotP;
        scale = *ring->scaleP;
        if(ring->baseMdlNo != -1) {
            basePos = model->posP[ring->baseMdlNo];
            *pos = *basePos;
            pos->x = basePos->x+ring->baseOfs.x;
            pos->y = basePos->y+ring->baseOfs.y;
            pos->z = basePos->z+ring->baseOfs.z;
        }
        switch(ring->mode) {
            case 1:
                ring->baseOfs.x = 8;
                ring->baseOfs.y = 0;
                ring->baseOfs.z = 0;
                MdRingDispSet(i, TRUE);
                MdRingColorReset(i);
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
                ring->endColor.r = ring->color.r;
                ring->endColor.g = ring->color.g;
                ring->endColor.b = ring->color.b;
                ring->endColor.a = ring->color.a;
                ring->mode = 5;
                break;

            case 5:
                MdMathOscillate(MDMATH_OSC_90, &ring->time, &ring->t, 8);
                MdMathLerpColor(&ring->color.a, ring->endColor.a, 0, ring->t);
                if(ring->time >= 1) {
                    MdRingDispSet(i, FALSE);
                    MdRingColorReset(i);
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
                ring->endColor.r = ring->color.r;
                ring->endColor.g = ring->color.g;
                ring->endColor.b = ring->color.b;
                ring->endColor.a = ring->color.a;
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
                ring->endColor.r = ring->color.r;
                ring->endColor.g = ring->color.g;
                ring->endColor.b = ring->color.b;
                ring->endColor.a = ring->color.a;
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
                ring->endColor.r = ring->color.r;
                ring->endColor.g = ring->color.g;
                ring->endColor.b = ring->color.b;
                ring->endColor.a = ring->color.a;
                ring->mode = 14;
                break;

            case 14:
                colorNo = 0;
                colorNo = i;
                MdMathOscillate(MDMATH_OSC_90, &ring->time, &ring->t, 15);
                MdMathLerpColor(&ring->color.r, ring->endColor.r, RingColorTbl[colorNo].r, ring->t);
                MdMathLerpColor(&ring->color.g, ring->endColor.g, RingColorTbl[colorNo].g, ring->t);
                MdMathLerpColor(&ring->color.b, ring->endColor.b, RingColorTbl[colorNo].b, ring->t);
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
        MTXTrans(trans, 0.02f, 0.03f, 0);
        MTXScale(scale, 0.97f, 0.97f, 1);
        MTXConcat(scale, trans, texMtx);
        GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    }
}

static void RingMatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    RingDraw(0);
}
