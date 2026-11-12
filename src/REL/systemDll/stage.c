#include "REL/systemDll/sysmd.h"

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
static void Ring5MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Ring6MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Ring7MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Ring8MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Ring9MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Ring10MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void Ring11MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
void MdSpriteDispSet(s32 grpNo, s32 dispF);

typedef struct SpriteInfo_s {
    s16 animNo;
    s16 pri;
    float x;
    float y;
    float scaleX;
    float scaleY;
    float tpLvl;
} SPRITEINFO;

typedef struct SpriteGrpInfo_s {
    s16 start;
    s16 end;
    float x;
    float y;
} SPRITEGRPINFO;

static s32 SpriteAnimFile[31] = {
    0xC30044, 0xC30045, 0xC30046, 0xC30047, 0xC30048, 0xC3005D, 0xC30049, 0xC3004A,
    0xC3004B, 0xC3004C, 0xC3004D, 0xC3004E, 0xC3004F, 0xC30050, 0xC30051, 0xC30052,
    0xC30053, 0xC30054, 0xC30055, 0xC30056, 0xC30057, 0xC30058, 0xC30059, 0xC3005A,
    0xC3005B, 0xC3005C, 0xC30061, 0xC3005F, 0xC30060, 0x25001A, 0xC3005E,
};

static SPRITEINFO SpriteInfo[48] = {
    { 1, 2, 0.0f, 0.0f, 1.0f, 8.0f, 0.6f },
    { 0, 1, 0.0f, -28.0f, 72.0f, 1.0f, 1.0f },
    { 0, 1, 0.0f, 32.0f, 72.0f, 1.0f, 1.0f },
    { 2, 4, -64.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    { 2, 4, 64.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    { 2, 4, 0.0f, -32.0f, 1.0f, 1.0f, 1.0f },
    { 2, 4, 0.0f, 32.0f, 1.0f, 1.0f, 1.0f },
    { 3, 3, -64.0f, 0.0f, 1.0f, 1.0f, 0.5f },
    { 3, 3, 64.0f, 0.0f, 1.0f, 1.0f, 0.5f },
    { 3, 3, 0.0f, -32.0f, 1.0f, 1.0f, 0.5f },
    { 3, 3, 0.0f, 32.0f, 1.0f, 1.0f, 0.5f },
    { 4, 6, -192.0f, -120.0f, 1.0f, 1.0f, 1.0f },
    { 4, 6, -128.0f, -120.0f, 1.0f, 1.0f, 1.0f },
    { 4, 6, -64.0f, -120.0f, 1.0f, 1.0f, 1.0f },
    { 4, 6, 0.0f, -120.0f, 1.0f, 1.0f, 1.0f },
    { 4, 6, 64.0f, -120.0f, 1.0f, 1.0f, 1.0f },
    { 4, 6, 128.0f, -120.0f, 1.0f, 1.0f, 1.0f },
    { 4, 6, 192.0f, -120.0f, 1.0f, 1.0f, 1.0f },
    { 6, 7, -192.0f, -80.0f, 1.0f, 1.0f, 1.0f },
    { 6, 7, -128.0f, -80.0f, 1.0f, 1.0f, 1.0f },
    { 6, 7, -64.0f, -80.0f, 1.0f, 1.0f, 1.0f },
    { 6, 7, 0.0f, -80.0f, 1.0f, 1.0f, 1.0f },
    { 6, 7, 64.0f, -80.0f, 1.0f, 1.0f, 1.0f },
    { 6, 7, 128.0f, -80.0f, 1.0f, 1.0f, 1.0f },
    { 6, 7, 192.0f, -80.0f, 1.0f, 1.0f, 1.0f },
    { 7, 8, -64.0f, -40.0f, 1.0f, 1.0f, 1.0f },
    { 7, 8, 0.0f, -40.0f, 1.0f, 1.0f, 1.0f },
    { 7, 8, 64.0f, -40.0f, 1.0f, 1.0f, 1.0f },
    { 8, 5, -64.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    { 9, 5, 64.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    { 10, 9, 0.0f, -176.0f, 1.0f, 1.0f, 1.0f },
    { 11, 9, 0.0f, -176.0f, 1.0f, 1.0f, 1.0f },
    { 12, 9, 0.0f, -176.0f, 1.0f, 1.0f, 1.0f },
    { 13, 9, 0.0f, -176.0f, 1.0f, 1.0f, 1.0f },
    { 14, 9, 0.0f, -176.0f, 1.0f, 1.0f, 1.0f },
    { 15, 9, 0.0f, -176.0f, 1.0f, 1.0f, 1.0f },
    { 16, 9, 0.0f, -176.0f, 1.0f, 1.0f, 1.0f },
    { 18, 11, -120.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    { 18, 11, -40.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    { 18, 11, 40.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    { 18, 11, 120.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    { 17, 10, -104.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    { 17, 10, -24.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    { 17, 10, 56.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    { 17, 10, 136.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    { 26, 12, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    { 2, 13, 64.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    { 2, 13, -64.0f, 0.0f, 1.0f, 1.0f, 1.0f },
};

static SPRITEGRPINFO SpriteGrpInfo[10] = {
    { 0, 2, 288.0f, 408.0f },
    { 3, 10, 288.0f, 240.0f },
    { 11, 17, 288.0f, 240.0f },
    { 18, 24, 288.0f, 240.0f },
    { 25, 27, 288.0f, 240.0f },
    { 28, 29, 288.0f, 240.0f },
    { 30, 36, 288.0f, 240.0f },
    { 37, 44, 288.0f, 240.0f },
    { 45, 45, 288.0f, 240.0f },
    { 46, 47, 288.0f, 240.0f },
};

static GXColor RingColorTbl[8] = {
    { 255, 128, 0, 0 }, { 0, 128, 255, 0 }, { 128, 255, 0, 0 }, { 0, 255, 0, 0 },
    { 255, 128, 0, 0 }, { 0, 255, 255, 0 }, { 255, 0, 0, 0 }, { 0, 128, 255, 0 },
};

static HU3DMATHOOK ToueiMatHookTbl[10] = {
    Touei1MatHook, Touei2MatHook, Touei3MatHook, Touei4MatHook, Touei5MatHook,
    Touei6MatHook, Touei7MatHook, Touei8MatHook, Touei9MatHook, Touei10MatHook,
};

static HU3DMATHOOK RingMatHookTbl[11] = {
    Ring1MatHook, Ring2MatHook, Ring3MatHook, Ring4MatHook, Ring5MatHook, Ring6MatHook,
    Ring7MatHook, Ring8MatHook, Ring9MatHook, Ring10MatHook, Ring11MatHook,
};

void MdSpriteCreate(NEL_SYS *sys)
{
    s16 grpId;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    s32 i;
    SPRITEINFO *sprInfo;
    SPRITEGRPINFO *grpInfo;
    s16 j;
    s16 grpNum;

    for (i = 0; i < 31; i++) {
        sprite->animP[i] = HuSprAnimRead(HuDataReadNum(SpriteAnimFile[i], HU_MEMNUM_OVL));
    }
    for (i = 0; i < 48; i++) {
        sprite->sprId[i] = HuSprCreate(sprite->animP[SpriteInfo[i].animNo], 0, 0);
    }
    grpInfo = SpriteGrpInfo;
    for (i = 0; i < 10; i++, grpInfo++) {
        grpNum = grpInfo->end - grpInfo->start + 1;
        grpId = HuSprGrpCreate(grpNum);
        sprite->grpId[i] = grpId;
        sprInfo = &SpriteInfo[grpInfo->start];
        for (j = 0; j < grpNum; j++, sprInfo++) {
            HuSprGrpMemberSet(grpId, j, sprite->sprId[grpInfo->start + j]);
            HuSprPriSet(grpId, j, sprInfo->pri);
            HuSprAttrReset(grpId, j, 4);
            HuSprPosSet(grpId, j, sprInfo->x, sprInfo->y);
            HuSprZRotSet(grpId, j, 0.0f);
            HuSprScaleSet(grpId, j, sprInfo->scaleX, sprInfo->scaleY);
            HuSprColorSet(grpId, j, 255, 255, 255);
            HuSprTPLvlSet(grpId, j, sprInfo->tpLvl);
        }
        HuSprGrpPosSet(grpId, grpInfo->x, grpInfo->y);
        HuSprGrpCenterSet(grpId, 0.0f, 0.0f);
        HuSprGrpZRotSet(grpId, 0.0f);
        HuSprGrpScaleSet(grpId, 1.0f, 1.0f);
        HuSprGrpDrawNoSet(grpId, 0);
        HuSprGrpScissorSet(grpId, 0, 0, 640, 480);
    }
    grpId = sprite->grpId[1];
    HuSprBankSet(grpId, 2, 0);
    HuSprBankSet(grpId, 3, 1);
    HuSprBankSet(grpId, 1, 2);
    HuSprBankSet(grpId, 0, 3);
    HuSprBankSet(grpId, 6, 0);
    HuSprBankSet(grpId, 7, 1);
    HuSprBankSet(grpId, 5, 2);
    HuSprBankSet(grpId, 4, 3);
    HuSprAttrSet(grpId, 6, 0x80);
    HuSprAttrSet(grpId, 7, 0x80);
    HuSprAttrSet(grpId, 5, 0x80);
    HuSprAttrSet(grpId, 4, 0x80);
    grpId = sprite->grpId[2];
    HuSprBankSet(grpId, 0, 0);
    HuSprBankSet(grpId, 1, 1);
    HuSprBankSet(grpId, 2, 2);
    HuSprBankSet(grpId, 3, 3);
    HuSprBankSet(grpId, 4, 4);
    HuSprBankSet(grpId, 5, 4);
    HuSprBankSet(grpId, 6, 4);
    grpId = sprite->grpId[3];
    HuSprBankSet(grpId, 0, 0);
    HuSprBankSet(grpId, 1, 1);
    HuSprBankSet(grpId, 2, 2);
    HuSprBankSet(grpId, 3, 3);
    HuSprBankSet(grpId, 4, 4);
    HuSprBankSet(grpId, 5, 4);
    HuSprBankSet(grpId, 6, 4);
    grpId = sprite->grpId[4];
    HuSprBankSet(grpId, 0, 0);
    HuSprBankSet(grpId, 1, 1);
    HuSprBankSet(grpId, 2, 2);
    grpId = sprite->grpId[7];
    HuSprBankSet(grpId, 4, 0);
    HuSprBankSet(grpId, 5, 1);
    HuSprBankSet(grpId, 6, 2);
    HuSprBankSet(grpId, 7, 3);
    grpId = sprite->grpId[9];
    HuSprBankSet(grpId, 0, 3);
    HuSprBankSet(grpId, 1, 2);
    MdSpriteDispSet(1, 0);
    MdSpriteDispSet(2, 0);
    MdSpriteDispSet(3, 0);
    MdSpriteDispSet(4, 0);
    MdSpriteDispSet(5, 0);
    MdSpriteDispSet(6, 0);
    MdSpriteDispSet(7, 0);
    MdSpriteDispSet(8, 0);
    MdSpriteDispSet(9, 0);
    sprite->tpLvl = 0.6f;
}

void MdSpriteDispSet(s32 grpNo, s32 dispF)
{
    NEL_SYS *sys = &sysWork;
    SPRITEGRPINFO *grpInfo;
    SYSSPRITE *sprite;
    s16 grpId;
    s32 grpNum;
    s16 i;

    grpInfo = &SpriteGrpInfo[grpNo];
    sprite = (SYSSPRITE *)sys->sprite;
    grpId = sprite->grpId[grpNo];
    grpNum = grpInfo->end - grpInfo->start + 1;

    for (i = 0; i < grpNum; i++) {
        if (dispF == 0) {
            HuSprAttrSet(grpId, i, 4);
        } else if (dispF == 1) {
            HuSprAttrReset(grpId, i, 4);
        }
    }
}

HuVec2f *MdSpriteGrpPosGet(int grpNo)
{
    NEL_SYS *sys = &sysWork;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    HUSPRGRP *grp = &HuSprGrpData[sprite->grpId[grpNo]];
    return &grp->pos;
}

HuVec2f *MdSpriteGrpScaleGet(int grpNo)
{
    NEL_SYS *sys = &sysWork;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    HUSPRGRP *grp = &HuSprGrpData[sprite->grpId[grpNo]];
    return &grp->scale;
}

HuVec2f *MdSpritePosGet(int grpNo, int memberNo)
{
    NEL_SYS *sys = &sysWork;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    HUSPRGRP *grp = &HuSprGrpData[sprite->grpId[grpNo]];
    HUSPRITE *spr = &HuSprData[grp->sprId[memberNo]];
    return &spr->pos;
}

HuVec2f *MdSpriteScaleGet(int grpNo, int memberNo)
{
    NEL_SYS *sys = &sysWork;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    HUSPRGRP *grp = &HuSprGrpData[sprite->grpId[grpNo]];
    HUSPRITE *spr = &HuSprData[grp->sprId[memberNo]];
    return &spr->scale;
}

float *MdSpriteAlphaGet(int grpNo, int memberNo)
{
    NEL_SYS *sys = &sysWork;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    HUSPRGRP *grp = &HuSprGrpData[sprite->grpId[grpNo]];
    HUSPRITE *spr = &HuSprData[grp->sprId[memberNo]];
    return &spr->a;
}

void MdSpriteDebug(NEL_SYS *sys)
{
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    s16 grpId = sprite->grpId[0];

    if (HuPadStkX[0] <= -36) {
        sprite->tpLvl -= 0.01f;
        if (sprite->tpLvl < 0.0f) {
            sprite->tpLvl = 0.0f;
        }
        HuSprTPLvlSet(grpId, 0, sprite->tpLvl);
    } else if (HuPadStkX[0] >= 36) {
        sprite->tpLvl += 0.01f;
        if (sprite->tpLvl > 1.0f) {
            sprite->tpLvl = 1.0f;
        }
        HuSprTPLvlSet(grpId, 0, sprite->tpLvl);
    }
    MdDebugFontPrint("tpl(%.2f)", sprite->tpLvl);
}

typedef struct CharFileInfo_s {
    s32 model;
    s32 idle;
    s32 win;
} CHARFILEINFO;

static CHARFILEINFO CharFileTbl[10] = {
    { 0xC30000, 0xC30001, 0xC30002 }, { 0xC30003, 0xC30004, 0xC30005 }, { 0xC30006, 0xC30007, 0xC30008 },
    { 0xC30009, 0xC3000A, 0xC3000B }, { 0xC3000C, 0xC3000D, 0xC3000E }, { 0xC3000F, 0xC30010, 0xC30011 },
    { 0xC30012, 0xC30013, 0xC30014 }, { 0xC30015, 0xC30016, 0xC30017 }, { 0xC30018, 0xC30019, 0xC3001A },
    { 0xC3001B, 0xC3001C, 0xC3001D },
};

static HuVecF CharPosTbl[10] = {
    { -250.0f, 0.0f, 0.0f }, { -125.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f },
    { 125.0f, 0.0f, 0.0f }, { 250.0f, 0.0f, 0.0f }, { -250.0f, -250.0f, 0.0f },
    { -125.0f, -250.0f, 0.0f }, { 0.0f, -250.0f, 0.0f }, { 125.0f, -250.0f, -1800.0f },
    { 250.0f, -250.0f, -1000.0f },
};

static s32 StarFileTbl[6] = { 0xC3001E, 0xC3001F, 0xC30020, 0xC30021, 0xC30022, 0xC30023 };
static s32 MapFileTbl[7] = { 0xC3002F, 0xC30030, 0xC30031, 0xC30032, 0xC30033, 0xC30034, 0xC30035 };
static s32 PackFileTbl[5] = { 0xC30036, 0xC30037, 0xC30038, 0xC30039, 0xC3003A };
static s16 ToueiAnmNoTbl[7] = { 0x18, 0x17, 0x19, 5, 5, 5, 5 };
static s16 ToueiMdlNoTbl[7] = { 0x3B, 0x3C, 0x41, 0x42, 0x43, 0x44, 0x45 };

static char *ToueiTexNameTbl[7] = {
    "number", "normalpack", "bonusyes", "dummy", "dummy", "dummy", "dummy",
};

static HuVecF ToueiTexObjOfs[10] = {
    { 0.0f, 15.0f, 0.0f }, { 0.0f, 18.0f, 0.0f }, { 0.0f, 12.0f, 0.0f }, { 0.0f, 20.0f, 0.0f },
    { 0.0f, 15.0f, 0.0f }, { 0.0f, 6.0f, 0.0f }, { 0.0f, -5.0f, 0.0f }, { 0.0f, 20.0f, 0.0f },
    { 0.0f, 70.0f, 0.0f }, { 0.0f, 10.0f, 0.0f },
};

#define MDL(i) model->mdlId[i]
#define CAMBIT(n) (((SYSCAMERA_WORK *)sys->camera)->camera[n].bit)

static void DrawMirror(HU3DMODEL *modelP, Mtx *mtx);
static void ToneDownFilter(HU3DMODEL *modelP, Mtx *mtx);
static void FontBGFilter(HU3DMODEL *modelP, Mtx *mtx);
void MdCopyWarpFilter(s16 layerNo);
static void TexBufFBCopy(NEL_SYS *sys, s32 no);
static void ToueiZClear(void);

void MdModelCreate(NEL_SYS *sys)
{
    SYSMODEL *model = (SYSMODEL *)sys->model;
    s32 i;
    HU3DMODEL *modelP;
    float f;

    MDL(0) = Hu3DModelCreateData(0xC30027);
    Hu3DMotionSpeedSet(MDL(0), -0.2f);
    Hu3DModelAttrSet(MDL(0), 1);
    Hu3DModelAttrReset(MDL(0), HU3D_MOTATTR_PAUSE);
    Hu3DModelAttrSet(MDL(0), HU3D_MOTATTR_LOOP);
    Hu3DModelLayerSet(MDL(0), 0);
    Hu3DModelCameraSet(MDL(0), CAMBIT(0));
    Hu3DModelPosSet(MDL(0), 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(MDL(0), 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(MDL(0), 1.0f, 1.0f, 1.0f);
    MDL(1) = Hu3DModelCreateData(0xC30029);
    Hu3DMotionSpeedSet(MDL(1), 0.5f);
    Hu3DModelAttrSet(MDL(1), 1);
    Hu3DModelAttrReset(MDL(1), HU3D_MOTATTR_PAUSE);
    Hu3DModelAttrSet(MDL(1), HU3D_MOTATTR_LOOP);
    Hu3DModelLayerSet(MDL(1), 0);
    Hu3DModelCameraSet(MDL(1), CAMBIT(1));
    Hu3DModelPosSet(MDL(1), 0.0f, -300.0f, 0.0f);
    Hu3DModelRotSet(MDL(1), 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(MDL(1), 2.0f, 2.0f, 1.0f);
    for (i = 0; i < 10; i++) {
        MDL(i + 2) = Hu3DModelCreate(HuDataSelHeapReadNum(CharFileTbl[i].model, HU_MEMNUM_OVL, HEAP_MODEL));
        model->motId[i * 2] = Hu3DJointMotionData(MDL(i + 2), CharFileTbl[i].idle);
        model->motId[i * 2 + 1] = Hu3DJointMotionData(MDL(i + 2), CharFileTbl[i].win);
        Hu3DMotionSet(MDL(i + 2), model->motId[i * 2]);
        Hu3DMotionSpeedSet(MDL(i + 2), 1.0f);
        Hu3DModelAttrSet(MDL(i + 2), 1);
        Hu3DModelAttrReset(MDL(i + 2), HU3D_MOTATTR_PAUSE);
        Hu3DModelAttrSet(MDL(i + 2), HU3D_MOTATTR_LOOP);
        Hu3DModelLayerSet(MDL(i + 2), 1);
        Hu3DModelCameraSet(MDL(i + 2), CAMBIT(1));
        Hu3DModelPosSet(MDL(i + 2), CharPosTbl[i].x, CharPosTbl[i].y, CharPosTbl[i].z);
        Hu3DModelRotSet(MDL(i + 2), 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(MDL(i + 2), 0.8f, 0.8f, 0.8f);
    }
    MDL(12) = Hu3DModelCreateData(0xC30042);
    Hu3DMotionSpeedSet(MDL(12), 0.2f);
    Hu3DModelAttrSet(MDL(12), 1);
    Hu3DModelAttrReset(MDL(12), HU3D_MOTATTR_PAUSE);
    Hu3DModelAttrSet(MDL(12), HU3D_MOTATTR_LOOP);
    Hu3DModelLayerSet(MDL(12), 0);
    Hu3DModelCameraSet(MDL(12), CAMBIT(2));
    Hu3DModelPosSet(MDL(12), 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(MDL(12), 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(MDL(12), 1.0f, 1.0f, 1.0f);
    MDL(15) = Hu3DHookFuncCreate(DrawMirror);
    Hu3DModelLayerSet(MDL(15), 1);
    Hu3DModelCameraSet(MDL(15), CAMBIT(2));
    Hu3DModelAttrSet(MDL(15), 1);
    for (i = 0; i < 10; i++) {
        if (i == 0) {
            MDL(16) = Hu3DModelCreateData(0xC30028);
        } else {
            MDL(i + 16) = Hu3DModelLink(MDL(16));
        }
        Hu3DModelAttrSet(MDL(i + 16), 1);
        Hu3DModelLayerSet(MDL(i + 16), 3);
        Hu3DModelCameraSet(MDL(i + 16), CAMBIT(2));
        if (i < 5) {
            Hu3DModelPosSet(MDL(i + 16), -500.0f + 250.0f * i, 350.0f, -1000.0f);
        } else {
            Hu3DModelPosSet(MDL(i + 16), -500.0f + 250.0f * (i - 5), 100.0f, -1000.0f);
        }
        Hu3DModelRotSet(MDL(i + 16), 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(MDL(i + 16), 1.0f, 1.0f, 0.1f);
        Hu3DModelMatHookSet(MDL(i + 16), ToueiMatHookTbl[i]);
    }
    for (i = 0; i < 6; i++) {
        MDL(i + 26) = Hu3DModelCreateData(StarFileTbl[i]);
        Hu3DModelAttrReset(MDL(i + 26), 1);
        Hu3DModelAttrSet(MDL(i + 26), HU3D_MOTATTR_LOOP);
        Hu3DMotionSpeedSet(MDL(i + 26), 1.0f);
        Hu3DModelLayerSet(MDL(i + 26), 3);
        Hu3DModelCameraSet(MDL(i + 26), CAMBIT(2));
        f = (float)(200.0 * sin((3.141592653589793 * (60.0f * i)) / 180.0));
        Hu3DModelPosSet(MDL(i + 26), 200.0 * cos((3.141592653589793 * (60.0f * i)) / 180.0), -200.0f, f);
        Hu3DModelRotSet(MDL(i + 26), 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(MDL(i + 26), 1.0f, 1.0f, 1.0f);
    }
    model->motId[20] = Hu3DJointMotionData(MDL(26), 0xC30025);
    model->motId[21] = Hu3DJointMotionData(MDL(26), 0xC30026);
    Hu3DMotionSet(MDL(26), model->motId[20]);
    Hu3DMotionSpeedSet(MDL(26), 1.0f);
    Hu3DModelAttrSet(MDL(26), HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(MDL(26), HU3D_MOTATTR_PAUSE);
    for (i = 0; i < 7; i++) {
        MDL(i + 32) = Hu3DModelCreateData(MapFileTbl[i]);
        Hu3DModelAttrSet(MDL(i + 32), 1);
        Hu3DModelLayerSet(MDL(i + 32), 2);
        Hu3DModelCameraSet(MDL(i + 32), CAMBIT(2));
        Hu3DModelPosSet(MDL(i + 32), -600.0f + 300.0f * i, 0.0f, 500.0f);
        Hu3DModelRotSet(MDL(i + 32), 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(MDL(i + 32), 1.0f, 1.0f, 1.0f);
    }
    for (i = 0; i < 11; i++) {
        if (i == 0) {
            MDL(39) = Hu3DModelCreateData(0xC3002C);
        } else {
            MDL(i + 39) = Hu3DModelLink(MDL(39));
        }
        Hu3DModelAttrSet(MDL(i + 39), 1);
        Hu3DModelLayerSet(MDL(i + 39), 6);
        Hu3DModelCameraSet(MDL(i + 39), CAMBIT(2));
        Hu3DModelPosSet(MDL(i + 39), -450.0f + 150.0f * i, 0.0f, 30.0f * i);
        Hu3DModelRotSet(MDL(i + 39), 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(MDL(i + 39), 1.0f, 1.0f, 1.0f);
        Hu3DModelMatHookSet(MDL(i + 39), RingMatHookTbl[i]);
    }
    MDL(50) = Hu3DModelCreateData(0xC3002A);
    Hu3DModelAttrSet(MDL(50), 1);
    Hu3DModelLayerSet(MDL(50), 4);
    Hu3DModelCameraSet(MDL(50), CAMBIT(2));
    Hu3DModelPosSet(MDL(50), -500.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(MDL(50), 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(MDL(50), 1.0f, 1.0f, 1.0f);
    Hu3DModelAttrSet(MDL(50), HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(MDL(50), HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(MDL(50), 1.0f);
    MDL(51) = Hu3DModelCreateData(0xC3002B);
    Hu3DModelAttrSet(MDL(51), 1);
    Hu3DModelLayerSet(MDL(51), 4);
    Hu3DModelCameraSet(MDL(51), CAMBIT(2));
    Hu3DModelPosSet(MDL(51), 500.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(MDL(51), 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(MDL(51), 1.0f, 1.0f, 1.0f);
    Hu3DModelAttrSet(MDL(51), HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(MDL(51), HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(MDL(51), 1.0f);
    for (i = 0; i < 4; i++) {
        if (i == 0) {
            MDL(52) = Hu3DModelCreateData(0xC3002D);
        } else {
            MDL(i + 52) = Hu3DModelLink(MDL(52));
        }
        Hu3DModelAttrSet(MDL(i + 52), 1);
        Hu3DModelLayerSet(MDL(i + 52), 4);
        Hu3DModelCameraSet(MDL(i + 52), CAMBIT(2));
        Hu3DModelPosSet(MDL(i + 52), -300.0f - 200.0f * i, 0.0f, -250.0f);
        Hu3DModelRotSet(MDL(i + 52), 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(MDL(i + 52), 1.0f, 1.0f, 1.0f);
        Hu3DModelAttrSet(MDL(i + 52), HU3D_MOTATTR_LOOP);
        Hu3DModelAttrReset(MDL(i + 52), HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(MDL(i + 52), 1.0f);
    }
    for (i = 0; i < 3; i++) {
        if (i == 0) {
            MDL(56) = Hu3DModelCreateData(0xC3002E);
        } else {
            MDL(i + 56) = Hu3DModelLink(MDL(56));
        }
        Hu3DModelAttrSet(MDL(i + 56), 1);
        Hu3DModelLayerSet(MDL(i + 56), 4);
        Hu3DModelCameraSet(MDL(i + 56), CAMBIT(2));
        Hu3DModelPosSet(MDL(i + 56), 300.0f + 200.0f * i, 0.0f, -250.0f);
        Hu3DModelRotSet(MDL(i + 56), 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(MDL(i + 56), 1.0f, 1.0f, 1.0f);
        Hu3DModelAttrSet(MDL(i + 56), HU3D_MOTATTR_LOOP);
        Hu3DModelAttrReset(MDL(i + 56), HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(MDL(i + 56), 1.0f);
    }
    MDL(59) = Hu3DModelCreateData(0xC3003B);
    Hu3DModelAttrSet(MDL(59), 1);
    Hu3DModelLayerSet(MDL(59), 5);
    Hu3DModelCameraSet(MDL(59), CAMBIT(2));
    Hu3DModelPosSet(MDL(59), -300.0f, 0.0f, 250.0f);
    Hu3DModelRotSet(MDL(59), 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(MDL(59), 1.0f, 1.0f, 1.0f);
    for (i = 0; i < 5; i++) {
        MDL(i + 60) = Hu3DModelCreateData(PackFileTbl[i]);
        Hu3DModelAttrSet(MDL(i + 60), 1);
        Hu3DModelLayerSet(MDL(i + 60), 5);
        Hu3DModelCameraSet(MDL(i + 60), CAMBIT(2));
        Hu3DModelPosSet(MDL(i + 60), 0.0f, 0.0f, 250.0f);
        Hu3DModelRotSet(MDL(i + 60), 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(MDL(i + 60), 1.0f, 1.0f, 1.0f);
        Hu3DModelAttrSet(MDL(i + 60), HU3D_MOTATTR_LOOP);
        Hu3DModelAttrReset(MDL(i + 60), HU3D_MOTATTR_PAUSE);
        Hu3DMotionSpeedSet(MDL(i + 60), 1.0f);
    }
    MDL(65) = Hu3DModelCreateData(0xC3003C);
    Hu3DModelAttrSet(MDL(65), 1);
    Hu3DModelLayerSet(MDL(65), 5);
    Hu3DModelCameraSet(MDL(65), CAMBIT(2));
    Hu3DModelPosSet(MDL(65), 300.0f, 0.0f, 250.0f);
    Hu3DModelRotSet(MDL(65), 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(MDL(65), 1.0f, 1.0f, 1.0f);
    for (i = 0; i < 4; i++) {
        if (i == 0) {
            MDL(66) = Hu3DModelCreateData(0xC3003D);
        } else {
            MDL(i + 66) = Hu3DModelLink(MDL(66));
        }
        Hu3DModelAttrSet(MDL(i + 66), 1);
        Hu3DModelLayerSet(MDL(i + 66), 6);
        Hu3DModelCameraSet(MDL(i + 66), CAMBIT(2));
        Hu3DModelAmbSet(MDL(i + 66), 1.0f, 1.0f, 1.0f);
        Hu3DModelPosSet(MDL(i + 66), -150.0f + 100.0f * i, -200.0f, 500.0f);
        Hu3DModelRotSet(MDL(i + 66), 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(MDL(i + 66), 4.0f, 4.0f, 4.0f);
        Hu3DModelAttrSet(MDL(i + 66), HU3D_MOTATTR_LOOP);
        Hu3DModelAttrReset(MDL(i + 66), HU3D_MOTATTR_PAUSE);
    }
    MDL(70) = Hu3DModelCreateData(0xC3003E);
    Hu3DModelAttrSet(MDL(70), 1);
    Hu3DModelLayerSet(MDL(70), 5);
    Hu3DModelCameraSet(MDL(70), CAMBIT(2));
    Hu3DModelPosSet(MDL(70), 300.0f, 0.0f, 250.0f);
    Hu3DModelRotSet(MDL(70), 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(MDL(70), 1.0f, 1.0f, 1.0f);
    MDL(71) = Hu3DModelCreateData(0xC3003F);
    Hu3DModelAttrSet(MDL(71), 1);
    Hu3DModelLayerSet(MDL(71), 5);
    Hu3DModelCameraSet(MDL(71), CAMBIT(2));
    Hu3DModelPosSet(MDL(71), -300.0f, 0.0f, 250.0f);
    Hu3DModelRotSet(MDL(71), 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(MDL(71), 1.0f, 1.0f, 1.0f);
    MDL(72) = Hu3DModelCreateData(0xC30040);
    Hu3DModelAttrSet(MDL(72), 1);
    Hu3DModelLayerSet(MDL(72), 5);
    Hu3DModelCameraSet(MDL(72), CAMBIT(2));
    Hu3DModelPosSet(MDL(72), 0.0f, 0.0f, 250.0f);
    Hu3DModelRotSet(MDL(72), 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(MDL(72), 1.0f, 1.0f, 1.0f);
    MDL(73) = Hu3DModelCreateData(0xC30041);
    Hu3DModelAttrSet(MDL(73), 1);
    Hu3DModelLayerSet(MDL(73), 5);
    Hu3DModelCameraSet(MDL(73), CAMBIT(2));
    Hu3DModelPosSet(MDL(73), 300.0f, 0.0f, 250.0f);
    Hu3DModelRotSet(MDL(73), 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(MDL(73), 1.0f, 1.0f, 1.0f);
    MDL(74) = Hu3DModelCreateData(0xC30043);
    Hu3DModelAttrSet(MDL(74), 1);
    Hu3DModelLayerSet(MDL(74), 0);
    Hu3DModelCameraSet(MDL(74), CAMBIT(2));
    Hu3DModelPosSet(MDL(74), 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(MDL(74), 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(MDL(74), 1.0f, 1.0f, 1.0f);
    Hu3DModelAttrSet(MDL(74), HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(MDL(74), HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(MDL(74), 1.0f);
    MDL(75) = Hu3DHookFuncCreate(ToneDownFilter);
    Hu3DModelLayerSet(MDL(75), 1);
    Hu3DModelCameraSet(MDL(75), CAMBIT(2));
    Hu3DModelAttrSet(MDL(75), 1);
    MDL(76) = Hu3DHookFuncCreate(FontBGFilter);
    Hu3DModelLayerSet(MDL(76), 7);
    Hu3DModelCameraSet(MDL(76), CAMBIT(2));
    Hu3DModelAttrReset(MDL(76), 1);
    if (sys->unk234 == 1) {
        Hu3DModelAttrReset(MDL(15), 1);
        Hu3DModelAttrReset(MDL(0), 1);
        Hu3DLayerHookSet(8, MdCopyWarpFilter);
    }
    for (i = 0; i < 0x4D; i++) {
        modelP = &Hu3DData[MDL(i)];
        model->mdlPos[i] = &modelP->pos;
        model->mdlRot[i] = &modelP->rot;
        model->mdlScale[i] = &modelP->scale;
    }
}

void TexBufSet(volatile s32 no)
{
    NEL_SYS *sys = &sysWork;
    SYSTEXBUF *texbuf = (SYSTEXBUF *)sys->texbuf;
    SYSTEXBUFUNIT *unit;
    GXColor color = { 0x10, 0x10, 0x10, 0x80 };

    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    unit = &texbuf->unit[no];
    GXLoadTexObj(&unit->texObj, GX_TEXMAP0);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, 0x7D);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(0, 0, 0, 0);
    GXSetTevColorIn(0, 8, 0xF, 0xF, 2);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(0, 7, 7, 7, 6);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
}

static void Touei1MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    NEL_SYS *sys = &sysWork;
    SYSTEXBUF *texbufDummy = (SYSTEXBUF *)sys->texbuf;
    NEL_SYS *sys2 = &sysWork;
    SYSTEXBUFUNIT *unit;
    SYSTEXBUF *texbuf = (SYSTEXBUF *)sys2->texbuf;
    GXColor color = { 0x10, 0x10, 0x10, 0x80 };

    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    unit = &texbuf->unit[0];
    GXLoadTexObj(&unit->texObj, GX_TEXMAP0);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, 0x7D);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(0, 0, 0, 0);
    GXSetTevColorIn(0, 8, 0xF, 0xF, 2);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(0, 7, 7, 7, 6);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
}

static void Touei2MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    NEL_SYS *sys = &sysWork;
    SYSTEXBUF *texbufDummy = (SYSTEXBUF *)sys->texbuf;
    NEL_SYS *sys2 = &sysWork;
    SYSTEXBUFUNIT *unit;
    SYSTEXBUF *texbuf = (SYSTEXBUF *)sys2->texbuf;
    GXColor color = { 0x10, 0x10, 0x10, 0x80 };

    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    unit = &texbuf->unit[1];
    GXLoadTexObj(&unit->texObj, GX_TEXMAP0);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, 0x7D);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(0, 0, 0, 0);
    GXSetTevColorIn(0, 8, 0xF, 0xF, 2);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(0, 7, 7, 7, 6);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
}

static void Touei3MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    NEL_SYS *sys = &sysWork;
    SYSTEXBUF *texbufDummy = (SYSTEXBUF *)sys->texbuf;
    NEL_SYS *sys2 = &sysWork;
    SYSTEXBUFUNIT *unit;
    SYSTEXBUF *texbuf = (SYSTEXBUF *)sys2->texbuf;
    GXColor color = { 0x10, 0x10, 0x10, 0x80 };

    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    unit = &texbuf->unit[2];
    GXLoadTexObj(&unit->texObj, GX_TEXMAP0);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, 0x7D);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(0, 0, 0, 0);
    GXSetTevColorIn(0, 8, 0xF, 0xF, 2);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(0, 7, 7, 7, 6);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
}

static void Touei4MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    NEL_SYS *sys = &sysWork;
    SYSTEXBUF *texbufDummy = (SYSTEXBUF *)sys->texbuf;
    NEL_SYS *sys2 = &sysWork;
    SYSTEXBUFUNIT *unit;
    SYSTEXBUF *texbuf = (SYSTEXBUF *)sys2->texbuf;
    GXColor color = { 0x10, 0x10, 0x10, 0x80 };

    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    unit = &texbuf->unit[3];
    GXLoadTexObj(&unit->texObj, GX_TEXMAP0);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, 0x7D);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(0, 0, 0, 0);
    GXSetTevColorIn(0, 8, 0xF, 0xF, 2);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(0, 7, 7, 7, 6);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
}

static void Touei5MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    NEL_SYS *sys = &sysWork;
    SYSTEXBUF *texbufDummy = (SYSTEXBUF *)sys->texbuf;
    NEL_SYS *sys2 = &sysWork;
    SYSTEXBUFUNIT *unit;
    SYSTEXBUF *texbuf = (SYSTEXBUF *)sys2->texbuf;
    GXColor color = { 0x10, 0x10, 0x10, 0x80 };

    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    unit = &texbuf->unit[4];
    GXLoadTexObj(&unit->texObj, GX_TEXMAP0);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, 0x7D);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(0, 0, 0, 0);
    GXSetTevColorIn(0, 8, 0xF, 0xF, 2);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(0, 7, 7, 7, 6);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
}

static void Touei6MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    NEL_SYS *sys = &sysWork;
    SYSTEXBUF *texbufDummy = (SYSTEXBUF *)sys->texbuf;
    NEL_SYS *sys2 = &sysWork;
    SYSTEXBUFUNIT *unit;
    SYSTEXBUF *texbuf = (SYSTEXBUF *)sys2->texbuf;
    GXColor color = { 0x10, 0x10, 0x10, 0x80 };

    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    unit = &texbuf->unit[5];
    GXLoadTexObj(&unit->texObj, GX_TEXMAP0);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, 0x7D);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(0, 0, 0, 0);
    GXSetTevColorIn(0, 8, 0xF, 0xF, 2);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(0, 7, 7, 7, 6);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
}

static void Touei7MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    NEL_SYS *sys = &sysWork;
    SYSTEXBUF *texbufDummy = (SYSTEXBUF *)sys->texbuf;
    NEL_SYS *sys2 = &sysWork;
    SYSTEXBUFUNIT *unit;
    SYSTEXBUF *texbuf = (SYSTEXBUF *)sys2->texbuf;
    GXColor color = { 0x10, 0x10, 0x10, 0x80 };

    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    unit = &texbuf->unit[6];
    GXLoadTexObj(&unit->texObj, GX_TEXMAP0);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, 0x7D);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(0, 0, 0, 0);
    GXSetTevColorIn(0, 8, 0xF, 0xF, 2);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(0, 7, 7, 7, 6);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
}

static void Touei8MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    NEL_SYS *sys = &sysWork;
    SYSTEXBUF *texbufDummy = (SYSTEXBUF *)sys->texbuf;
    NEL_SYS *sys2 = &sysWork;
    SYSTEXBUFUNIT *unit;
    SYSTEXBUF *texbuf = (SYSTEXBUF *)sys2->texbuf;
    GXColor color = { 0x10, 0x10, 0x10, 0x80 };

    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    unit = &texbuf->unit[7];
    GXLoadTexObj(&unit->texObj, GX_TEXMAP0);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, 0x7D);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(0, 0, 0, 0);
    GXSetTevColorIn(0, 8, 0xF, 0xF, 2);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(0, 7, 7, 7, 6);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
}

static void Touei9MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    NEL_SYS *sys = &sysWork;
    SYSTEXBUF *texbufDummy = (SYSTEXBUF *)sys->texbuf;
    NEL_SYS *sys2 = &sysWork;
    SYSTEXBUFUNIT *unit;
    SYSTEXBUF *texbuf = (SYSTEXBUF *)sys2->texbuf;
    GXColor color = { 0x10, 0x10, 0x10, 0x80 };

    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    unit = &texbuf->unit[8];
    GXLoadTexObj(&unit->texObj, GX_TEXMAP0);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, 0x7D);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(0, 0, 0, 0);
    GXSetTevColorIn(0, 8, 0xF, 0xF, 2);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(0, 7, 7, 7, 6);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
}

static void Touei10MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    NEL_SYS *sys = &sysWork;
    SYSTEXBUF *texbufDummy = (SYSTEXBUF *)sys->texbuf;
    NEL_SYS *sys2 = &sysWork;
    SYSTEXBUFUNIT *unit;
    SYSTEXBUF *texbuf = (SYSTEXBUF *)sys2->texbuf;
    GXColor color = { 0x10, 0x10, 0x10, 0x80 };

    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    unit = &texbuf->unit[9];
    GXLoadTexObj(&unit->texObj, GX_TEXMAP0);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, 0x7D);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(0, 0, 0, 0);
    GXSetTevColorIn(0, 8, 0xF, 0xF, 2);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(0, 7, 7, 7, 6);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
}

void RingDraw(volatile s32 no)
{
    SYSRING *ring;
    NEL_SYS *sys = &sysWork;
    NEL_BLENDDBG *dbg = &sys->blendDbg;
    Mtx trans;
    Mtx scale;
    Mtx texMtx;

    ring = (SYSRING *)sys->ring + no;
    if (ring->disp != 0) {
        GXSetColorUpdate(1);
        GXSetAlphaUpdate(1);
        GXSetAlphaCompare(6, 1, 0, 6, 1);
        GXSetBlendMode(1, 4, 1, 0xC);
        GXSetNumTevStages(1);
        GXSetTevColor(1, ring->color);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        MTXTrans(trans, 0.02f, 0.02f, 0.0f);
        MTXScale(scale, 0.99f, 0.99f, 1.0f);
        MTXConcat(scale, trans, texMtx);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXSetTevOrder(0, 0, 0, 4);
        GXSetTevColorIn(0, 0xF, 8, 2, 0xF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaIn(0, 7, 4, 1, 7);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
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

static void Ring5MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    RingDraw(4);
}

static void Ring6MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    RingDraw(5);
}

static void Ring7MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    RingDraw(6);
}

static void Ring8MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    RingDraw(7);
}

static void Ring9MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    RingDraw(8);
}

static void Ring10MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    RingDraw(9);
}

static void Ring11MatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    RingDraw(10);
}

void InitGxWarpFilter(void)
{
    NEL_SYS *sys = &sysWork;
    GXColor color = { 0xFF, 0xFF, 0xFF, 0xFF };

    GXSetColorUpdate(1);
    GXSetAlphaUpdate(1);
    GXSetAlphaCompare(6, 1, 0, 6, 1);
    GXSetBlendMode(1, 4, 1, 0xC);
    GXSetNumTevStages(1);
    GXSetTevColor(1, color);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorIn(0, 0xF, 8, 2, 0xF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(0, 7, 4, 1, 7);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
}

void DrawWarpFilter(HU3DMODEL *modelP, Mtx *mtx)
{
    NEL_SYS *sys = &sysWork;

    if (Hu3DCameraNo == 2) {
        s16 x = 0;
        s16 y = 0;
        SYSTEXBUFUNIT *unit;
        s16 w = 640;
        s16 h = 480;
        Mtx44 proj;
        Mtx modelview;
        s16 x0;
        s16 x1;
        s16 y0;
        s16 y1;

        GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
        MTXOrtho(proj, 0.0f, 480.0f, 0.0f, 640.0f, 0.0f, -0.0f);
        GXSetProjection(proj, 1);
        MTXIdentity(modelview);
        GXLoadPosMtxImm(modelview, 0);
        GXSetColorUpdate(1);
        GXSetAlphaUpdate(1);
        GXSetAlphaCompare(6, 1, 0, 6, 1);
        GXSetBlendMode(1, 1, 1, 0xC);
        GXSetNumChans(1);
        GXSetNumTexGens(1);
        GXSetNumTevStages(1);
        GXSetChanCtrl(4, 0, 1, 1, 0, 0, 2);
        unit = &((SYSTEXBUF *)sys->texbuf)->unit[10];
        GXLoadTexObj(&unit->texObj, 0);
        GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
        GXSetTevOrder(0, 0, 0, 4);
        GXSetTevColorIn(0, 0xF, 0xA, 8, 0xF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaIn(0, 7, 5, 4, 7);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetVtxAttrFmt(1, 9, 1, 3, 0);
        GXSetVtxAttrFmt(1, 0xB, 1, 5, 0);
        GXSetVtxAttrFmt(1, 0xD, 1, 4, 0);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 1);
        GXSetVtxDesc(0xB, 1);
        GXSetVtxDesc(0xD, 1);
        x0 = x - 1;
        x1 = x + w;
        y0 = y - 1;
        y1 = y + h;
        GXBegin(0x80, 1, 4);
        GXPosition3s16(0, 0, 0);
        GXColor4u8(0xFF, 0xFF, 0, 0x20);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3s16(640, 0, 0);
        GXColor4u8(0xFF, 0xFF, 0, 0x20);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3s16(640, 480, 0);
        GXColor4u8(0xFF, 0xFF, 0, 0x20);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3s16(0, 480, 0);
        GXColor4u8(0xFF, 0xFF, 0, 0x20);
        GXTexCoord2f32(0.0f, 1.0f);
        GXEnd();
    }
}

void MdLayer1Hook(s16 layerNo)
{
    if (Hu3DCameraNo == 2) {
        Hu3DZClear();
    }
}

void MdCopyWarpFilter(s16 layerNo)
{
    NEL_SYS *sys = &sysWork;

    if (Hu3DCameraNo == 0) {
        TexBufFBCopy(sys, 10);
        ToueiZClear();
    }
}

void MdToueiLayerHook(s16 layerNo)
{
    NEL_SYS *sys = &sysWork;
    SYSTEXBUF *texbuf = (SYSTEXBUF *)sys->texbuf;
    SYSMODEL *model = (SYSMODEL *)sys->model;
    s32 i;

    if (Hu3DCameraNo == 1) {
        for (i = 0; i < 10; i++) {
            TexBufFBCopy(sys, i);
        }
        ToueiZClear();
        ((SYSTEXBUF *)sys->texbuf)->unk210 = 0;
    }
}

void MdLayer2Hook(s16 layerNo)
{
    NEL_SYS *sys = &sysWork;

    if (Hu3DCameraNo == 0) {
        TexBufFBCopy(sys, 10);
        ToueiZClear();
    }
}

static void DrawMirror(HU3DMODEL *modelP, Mtx *volatile mtx)
{
    NEL_SYS *sys = &sysWork;
    NEL_BLENDDBG *dbg = &sys->blendDbg;
    GXColor color = { 0x00, 0xFF, 0x00, 0x90 };
    Mtx44 proj;
    SYSTEXBUFUNIT *unit;

    if (dbg->dispTimer != 0) {
        color.r = dbg->col[0];
        color.g = dbg->col[1];
        color.b = dbg->col[2];
        color.a = dbg->col[3];
        GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
        MTXOrtho(proj, 0.0f, 480.0f, 0.0f, 640.0f, 0.0f, -16777215.0f);
        GXSetProjection(proj, 1);
        MTXIdentity(*mtx);
        GXLoadPosMtxImm(*mtx, 0);
        GXSetColorUpdate(1);
        GXSetAlphaUpdate(1);
        GXSetAlphaCompare(6, 1, 0, 6, 1);
        GXSetBlendMode(dbg->type, dbg->unk10, dbg->unk14, dbg->logic);
        GXSetNumChans(1);
        GXSetNumTexGens(1);
        GXSetNumTevStages(1);
        GXSetChanCtrl(4, 0, 1, 1, 0, 0, 2);
        unit = &((SYSTEXBUF *)sys->texbuf)->unit[10];
        GXLoadTexObj(&unit->texObj, 0);
        GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
        GXSetTevOrder(0, 0, 0, 4);
        GXSetTevColorIn(0, 0xF, 0xA, 8, 0xF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaIn(0, 7, 5, 4, 7);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetVtxAttrFmt(1, 9, 1, 3, 0);
        GXSetVtxAttrFmt(1, 0xB, 1, 5, 0);
        GXSetVtxAttrFmt(1, 0xD, 1, 4, 0);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 1);
        GXSetVtxDesc(0xB, 1);
        GXSetVtxDesc(0xD, 1);
        GXBegin(0x80, 1, 4);
        GXPosition3s16(0, 0, 0);
        GXColor4u8(color.r, color.g, color.b, color.a);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3s16(640, 0, 0);
        GXColor4u8(color.r, color.g, color.b, color.a);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3s16(640, 480, 0);
        GXColor4u8(color.r, color.g, color.b, color.a);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3s16(0, 480, 0);
        GXColor4u8(color.r, color.g, color.b, color.a);
        GXTexCoord2f32(0.0f, 1.0f);
        GXEnd();
    }
}

static void FontBGFilter(HU3DMODEL *modelP, Mtx *mtx)
{
    NEL_SYS *sys = &sysWork;
    SYSDEBUGFONT *font = (SYSDEBUGFONT *)sys->fontData;
    GXColor color = { 0, 0, 0, 128 };
    s16 width = 0;
    Mtx44 proj;

    if (font->printF == 1) {
        width = font->maxLen * (s32)(8.0f * font->size);
        GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
        MTXOrtho(proj, 0.0f, 480.0f, 0.0f, 640.0f, 0.0f, -16777215.0f);
        GXSetProjection(proj, 1);
        MTXIdentity(*mtx);
        GXLoadPosMtxImm(*mtx, 0);
        GXSetColorUpdate(1);
        GXSetAlphaUpdate(0);
        GXSetAlphaCompare(6, 1, 0, 6, 1);
        GXSetBlendMode(1, 4, 5, 5);
        GXSetNumChans(1);
        GXSetNumTexGens(0);
        GXSetNumTevStages(1);
        GXSetChanCtrl(4, 0, 0, 1, 0, 0, 0);
        GXSetChanCtrl(5, 0, 0, 1, 0, 0, 0);
        GXSetTevOrder(0, 0xFF, 0xFF, 0xFF);
        GXSetTevColor(1, color);
        GXSetTevColorIn(0, 2, 0xF, 0xF, 0xF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaIn(0, 1, 7, 7, 7);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetVtxAttrFmt(0, 9, 1, 3, 0);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 1);
        GXBegin(0x80, 0, 4);
        GXPosition3s16(font->posX, font->posY + font->ofsY, 0);
        GXPosition3s16(font->posX + width, font->posY + font->ofsY, 0);
        GXPosition3s16(font->posX + width, font->posY + font->rowOfsY, 0);
        GXPosition3s16(font->posX, font->posY + font->rowOfsY, 0);
        GXEnd();
    }
}

static void ToneDownFilter(HU3DMODEL *modelP, Mtx *volatile mtx)
{
    NEL_SYS *sys = &sysWork;
    NEL_BLENDDBG *dbg = &sys->blendDbg;
    GXColor color = { 0x00, 0xFF, 0x00, 0x90 };
    Mtx44 proj;
    SYSTEXBUFUNIT *unit;

    if (dbg->dispTimer != 0) {
        color.r = dbg->col[0];
        color.g = dbg->col[1];
        color.b = dbg->col[2];
        color.a = dbg->col[3];
        GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
        MTXOrtho(proj, 0.0f, 480.0f, 0.0f, 640.0f, 0.0f, -16777215.0f);
        GXSetProjection(proj, 1);
        MTXIdentity(*mtx);
        GXLoadPosMtxImm(*mtx, 0);
        GXSetColorUpdate(1);
        GXSetAlphaUpdate(1);
        GXSetAlphaCompare(6, 1, 0, 6, 1);
        GXSetBlendMode(dbg->type, dbg->unk10, dbg->unk14, dbg->logic);
        GXSetNumChans(1);
        GXSetNumTexGens(1);
        GXSetNumTevStages(1);
        GXSetChanCtrl(4, 0, 1, 1, 0, 0, 2);
        unit = &((SYSTEXBUF *)sys->texbuf)->unit[10];
        GXLoadTexObj(&unit->texObj, 0);
        GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
        GXSetTevOrder(0, 0, 0, 4);
        GXSetTevColorIn(0, 0xF, 0xA, 8, 0xF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaIn(0, 7, 5, 4, 7);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetVtxAttrFmt(1, 9, 1, 3, 0);
        GXSetVtxAttrFmt(1, 0xB, 1, 5, 0);
        GXSetVtxAttrFmt(1, 0xD, 1, 4, 0);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 1);
        GXSetVtxDesc(0xB, 1);
        GXSetVtxDesc(0xD, 1);
        GXBegin(0x80, 1, 4);
        GXPosition3s16(0, 0, 0);
        GXColor4u8(color.r, color.g, color.b, color.a);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3s16(640, 0, 0);
        GXColor4u8(color.r, color.g, color.b, color.a);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3s16(640, 480, 0);
        GXColor4u8(color.r, color.g, color.b, color.a);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3s16(0, 480, 0);
        GXColor4u8(color.r, color.g, color.b, color.a);
        GXTexCoord2f32(0.0f, 1.0f);
        GXEnd();
    }
}

extern char *MdToueiHookNameTbl[10];
static void MdTexBufObjPosSet(SYSTEXBUFUNIT *unit, s16 mdlId, char *objName, u16 cameraBit, HuVecF *ofs);

void MdToueiMdlInit(NEL_SYS *sys)
{
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    SYSMODEL *model = (SYSMODEL *)sys->model;
    s32 i;
    s16 animId;

    for (i = 0; i < 7; i++) {
        animId = Hu3DAnimCreate(sprite->animP[ToueiAnmNoTbl[i]], model->mdlId[ToueiMdlNoTbl[i]], ToueiTexNameTbl[i]);
        model->animId[i] = animId;
        Hu3DAnimSpeedSet(animId, 1.0f);
        Hu3DAnimAttrSet(animId, 1);
        Hu3DAnimAttrSet(animId, 2);
    }
    Hu3DAnmNoSet(model->animId[3], 0);
    Hu3DAnmNoSet(model->animId[4], 1);
    Hu3DAnmNoSet(model->animId[5], 2);
    Hu3DAnmNoSet(model->animId[6], 3);
}

void MdToueiTextureNext(NEL_SYS *sys)
{
    SYSMODEL *model = (SYSMODEL *)sys->model;
    static s32 anmNo1;
    static s32 anmNo2;
    static s32 anmNo3;
    static s32 anmNo4;

    anmNo1++;
    if (anmNo1 % 60 == 1) {
        anmNo2 = ++anmNo2 % 9;
        Hu3DAnmNoSet(model->animId[0], anmNo2);
    }
    if (anmNo1 % 60 == 1) {
        anmNo3 = ++anmNo3 % 2;
        Hu3DAnmNoSet(model->animId[1], anmNo3);
    }
    if (anmNo1 % 60 == 1) {
        anmNo4 = ++anmNo4 % 2;
        Hu3DAnmNoSet(model->animId[2], anmNo4);
    }
}

void MdToueiTexBufInit(NEL_SYS *sys)
{
    SYSTEXBUFUNIT *unit;
    s32 i;
    SYSTEXBUF *texbuf = (SYSTEXBUF *)sys->texbuf;
    SYSMODEL *model = (SYSMODEL *)sys->model;

    for (i = 0; i < 10; i++) {
        unit = &texbuf->unit[i];
        unit->w = 64;
        unit->h = 64;
        unit->fmt = 4;
        MdTexBufObjPosSet(unit, model->mdlId[i + 2], MdToueiHookNameTbl[i], ((SYSCAMERA_WORK *)sys->camera)->camera[1].bit, &ToueiTexObjOfs[i]);
        unit->addr = HuMemDirectMallocNum(HEAP_MODEL, GXGetTexBufferSize(unit->w, unit->h, unit->fmt, 0, 0), HU_MEMNUM_OVL);
        if (unit->addr == 0) {
            OSReport("sys->texbuf->unit[NEL_TEXBUF_MIRROR + %d].addr = NULL...\n", i);
        }
        GXInitTexObj(&unit->texObj, unit->addr, unit->w, unit->h, unit->fmt, 0, 0, 0);
        GXInitTexObjLOD(&unit->texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    }
    unit = &texbuf->unit[10];
    unit->x = 0;
    unit->y = 0;
    unit->w = 640;
    unit->h = 480;
    unit->fmt = 4;
    unit->addr = HuMemDirectMallocNum(HEAP_MODEL, GXGetTexBufferSize(unit->w, unit->h, unit->fmt, 0, 0), HU_MEMNUM_OVL);
    if (unit->addr == 0) {
        OSReport("sys->texbuf->unit[NEL_TEXBUF_WARPFILTER].addr = NULL...\n");
    }
    GXInitTexObj(&unit->texObj, unit->addr, unit->w, unit->h, unit->fmt, 0, 0, 0);
    GXInitTexObjLOD(&unit->texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
}

static void TexBufFBCopy(NEL_SYS *sys, volatile s32 no)
{
    SYSTEXBUFUNIT *unit = &((SYSTEXBUF *)sys->texbuf)->unit[no];

    Hu3DFbCopyExec(unit->x, unit->y, unit->w, unit->h, unit->fmt, 0, unit->addr);
}

static void MdTexBufObjPosSet(SYSTEXBUFUNIT *unit, s16 mdlId, char *objName, u16 cameraBit, HuVecF *ofs)
{
    HuVecF pos3D;
    HuVecF pos2D;
    s16 x;
    s16 y;

    Hu3DModelObjPosGet(mdlId, objName, &pos3D);
    pos3D.x += ofs->x;
    pos3D.y += ofs->y;
    pos3D.z += ofs->z;
    Hu3D3Dto2D(&pos3D, cameraBit, &pos2D);
    x = (s32)(640.0f * (pos2D.x / 576.0f)) & 0xFFFE;
    y = (s32)pos2D.y & 0xFFFE;
    unit->x = x - unit->w / 2;
    unit->y = y - unit->h / 2;
}

void MdTexBufPosSet(SYSTEXBUFUNIT *unit, volatile HuVecF *pos, volatile u16 cameraBit)
{
    HuVecF pos2D;
    s16 x;
    s16 y;

    Hu3D3Dto2D((HuVecF *)pos, cameraBit, &pos2D);
    x = (s32)(640.0f * (pos2D.x / 576.0f)) & 0xFFFE;
    y = (s32)pos2D.y & 0xFFFE;
    unit->x = x - unit->w / 2;
    unit->y = y - unit->h / 2;
}

static void ParticleHook(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
static void ParticleHook2(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
static void ParticleUpdate(SYSPARTICLE_BUFFER *buffer);
static void TickParticle(SYSPARTICLE_BUFFER *buffer, s32 num);
static void UpdateParticleData(SYSPARTICLE_BUFFER *buffer);

void MdParticleCreate(NEL_SYS *sys)
{
    SYSPARTICLE_BUFFER *buffer;
    SYSPARTICLE *data;
    SYSPARTICLE_PARAM *param;
    s32 i;
    SYSMODEL *model;
    SYSPARTICLE_WORK *particle;
    SYSSPRITE *sprite;

    sprite = (SYSSPRITE *)sys->sprite;
    model = (SYSMODEL *)sys->model;
    particle = (SYSPARTICLE_WORK *)sys->particle;
    param = &particle->param;
    param->baseAlpha = 224.0f;
    param->fadeInSpeed2 = 8.0f;
    param->fadeOutSpeed1 = 128.0f;
    param->fadeInSpeed1 = 4.0f;
    param->fadeOutSpeed2 = 16.0f;
    param->unk14 = 0.6f;
    param->scaleSpeed1 = 2.0f;
    param->scaleSpeed2 = 5.0f;
    param->unk20 = 0.3f;
    param->color.r = 80;
    param->color.g = 198;
    param->color.b = 255;
    data = &particle->data[0];
    for (i = 0; i < 0x300; i++, data++) {
        data->mode = 0;
        data->delay = 0;
        data->angle = 0.0f;
        data->radius = 0.0f;
        data->tpLvl = 0.0f;
        data->radiusSpeed = 0.0f;
        data->angleSpeed = 0.0f;
        data->velY = 0.0f;
        data->scale = 0.0f;
        data->zRot = 0.0f;
        data->pos.x = 0.0f;
        data->pos.y = 0.0f;
        data->pos.z = 0.0f;
        data->color.r = data->color.g = data->color.b = data->color.a = 0;
    }
    buffer = &particle->buffer[0];
    buffer->pauseF = 0;
    buffer->animP = sprite->animP[27];
    buffer->num = 0x180;
    buffer->particle = &particle->data[0];
    buffer->param = param;
    buffer->hook = ParticleHook;
    buffer->updateHook = ParticleUpdate;
    model->mdlId[13] = buffer->mdlId = Hu3DParticleCreate(buffer->animP, buffer->num);
    buffer->hookWork = (HU3DPARTICLE *)Hu3DData[buffer->mdlId].hookData;
    buffer->dataBuf = buffer->hookWork->data;
    Hu3DParticleHookSet(buffer->mdlId, buffer->hook);
    Hu3DParticleBlendModeSet(buffer->mdlId, 1);
    Hu3DModelLayerSet(buffer->mdlId, 2);
    Hu3DModelCameraSet(buffer->mdlId, CAMBIT(2));
    TickParticle(buffer, 240);
    UpdateParticleData(buffer);
    buffer = &particle->buffer[1];
    buffer->pauseF = 0;
    buffer->animP = sprite->animP[28];
    buffer->num = 0x180;
    buffer->particle = &particle->data[0x180];
    buffer->param = param;
    buffer->hook = ParticleHook2;
    buffer->updateHook = ParticleUpdate;
    model->mdlId[14] = buffer->mdlId = Hu3DParticleCreate(buffer->animP, buffer->num);
    buffer->hookWork = (HU3DPARTICLE *)Hu3DData[buffer->mdlId].hookData;
    buffer->dataBuf = buffer->hookWork->data;
    Hu3DParticleHookSet(buffer->mdlId, buffer->hook);
    Hu3DParticleBlendModeSet(buffer->mdlId, 1);
    Hu3DModelLayerSet(buffer->mdlId, 2);
    Hu3DModelCameraSet(buffer->mdlId, CAMBIT(2));
    UpdateParticleData(buffer);
    Hu3DModelAttrSet(model->mdlId[14], 1);
}

static void UpdateParticleData(SYSPARTICLE_BUFFER *buffer)
{
    SYSPARTICLE *particle;
    HU3DPARTICLEDATA *dataBuf;
    s32 i;

    particle = &buffer->particle[0];
    dataBuf = &buffer->dataBuf[0];
    for (i = 0; i < buffer->num; i++, particle++, dataBuf++) {
        dataBuf->scale = particle->scale;
        dataBuf->zRot = particle->zRot;
        dataBuf->pos = particle->pos;
        dataBuf->color = particle->color;
    }
    DCStoreRangeNoSync(&buffer->dataBuf[0], sizeof(HU3DPARTICLEDATA) * buffer->num);
}

static void TickParticle(SYSPARTICLE_BUFFER *buffer, volatile s32 num)
{
    s32 i;

    for (i = 0; i < num; i++) {
        buffer->updateHook(buffer);
    }
}

void MdParticleDataUpdate(volatile s32 bufIdx)
{
    NEL_SYS *sys = &sysWork;
    SYSPARTICLE_BUFFER *buffer = &((SYSPARTICLE_WORK *)sys->particle)->buffer[bufIdx];

    buffer->updateHook(buffer);
    UpdateParticleData(buffer);
}

static void ParticleHook(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    MdParticleDataUpdate(0);
}

static void ParticleHook2(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    MdParticleDataUpdate(1);
}

static void ParticleUpdate(SYSPARTICLE_BUFFER *buffer)
{
    SYSPARTICLE *particle;
    SYSPARTICLE_PARAM *param;
    s32 i;
    s32 third;
    s32 border;

    param = buffer->param;
    third = 0.33f * buffer->num;
    border = buffer->num - 16;
    if (buffer->pauseF != 1) {
        particle = buffer->particle;
        for (i = 0; i < buffer->num; i++, particle++) {
            switch (particle->mode) {
                case 0:
                    particle->mode++;
                    break;
                case 1:
                    particle->delay = frandmod(256);
                    particle->mode++;
                    break;
                case 2:
                    if (--particle->delay < 0) {
                        particle->delay = 0;
                        particle->mode++;
                    }
                    break;
                case 3:
                    particle->tpLvl = 0.0f;
                    particle->radius = 0.0f;
                    particle->angle = frandmod(360);
                    if (i < third) {
                        particle->velY = 0.5f + ((frandmod(100) + 1) / 500.0f);
                        particle->radiusSpeed = 0.5f + ((frandmod(100) + 1) / 200.0f);
                        particle->angleSpeed = -0.5f + ((frandmod(100) + 1) / 100.0f);
                    } else {
                        particle->velY = 0.5f + ((frandmod(100) + 1) / 100.0f);
                        particle->radiusSpeed = 0.5f + ((frandmod(100) + 1) / 200.0f);
                        particle->angleSpeed = -0.5f + ((frandmod(100) + 1) / 100.0f);
                    }
                    if (i >= border) {
                        particle->radius = 384.0f;
                        particle->velY = 0.0f;
                        particle->radiusSpeed = 0.0f;
                        particle->angleSpeed = -0.5f + ((frandmod(100) + 1) / 50.0f);
                    }
                    if (i < third) {
                        particle->color.r = 128;
                        particle->color.g = 64;
                        particle->color.b = 255;
                    } else {
                        particle->color.r = param->color.r;
                        particle->color.g = param->color.g;
                        particle->color.b = param->color.b;
                    }
                    particle->color.a = 0;
                    particle->scale = 0.0f;
                    particle->zRot = 0.0f;
                    particle->pos.x = 0.0f;
                    particle->pos.y = 0.0f;
                    particle->pos.z = 0.0f;
                    if (i >= border) {
                        particle->pos.y = -200.0f + ((frandmod(4000) + 1) / 10.0f);
                    }
                    particle->mode++;
                    break;
                case 4:
                case 5:
                case 6:
                    if (particle->mode == 4) {
                        if (i < third) {
                            particle->scale += param->scaleSpeed2;
                        } else {
                            particle->scale += param->scaleSpeed1;
                        }
                    }
                    particle->pos.x = particle->radius * HuCos(particle->angle);
                    particle->pos.y += particle->velY;
                    particle->pos.z = particle->radius * HuSin(particle->angle);
                    particle->radius += particle->radiusSpeed;
                    particle->angle += particle->angleSpeed;
                    if (particle->mode == 5) {
                        if (i < third) {
                            if (particle->pos.y >= 10.0f) {
                                particle->mode = 6;
                            }
                        } else if (i < border) {
                            if (particle->pos.y >= 250.0f) {
                                particle->mode = 6;
                            }
                        }
                    }
                    if (particle->mode == 4) {
                        if (i < third) {
                            particle->tpLvl += 1.0f / param->fadeInSpeed1;
                        } else {
                            particle->tpLvl += 1.0f / param->fadeInSpeed2;
                        }
                        if (particle->tpLvl >= 1.0f) {
                            particle->tpLvl = 1.0f;
                            particle->mode = 5;
                        }
                    } else if (particle->mode == 6) {
                        if (i < third) {
                            particle->tpLvl -= 1.0f / param->fadeOutSpeed2;
                        } else {
                            particle->tpLvl -= 1.0f / param->fadeOutSpeed1;
                        }
                        if (particle->tpLvl <= 0.0f) {
                            particle->tpLvl = 0.0f;
                        }
                    }
                    particle->color.a = param->baseAlpha * particle->tpLvl;
                    if (particle->mode == 6) {
                        if (particle->tpLvl <= 0.0f) {
                            particle->tpLvl = 0.0f;
                            particle->mode = 3;
                        }
                    }
                    break;
            }
        }
    }
}

void MdParticleDebug(NEL_SYS *sys)
{
    SYSPARTICLE_WORK *particle = (SYSPARTICLE_WORK *)sys->particle;
    SYSPARTICLE_PARAM *param = &particle->param;

    if (HuPadStkX[0] <= -36) {
        param->baseAlpha -= 1.0f;
    } else if (HuPadStkX[0] >= 36) {
        param->baseAlpha += 1.0f;
    }
    if (HuPadStkX[1] <= -36) {
        param->fadeInSpeed2 -= 1.0f;
    } else if (HuPadStkX[1] >= 36) {
        param->fadeInSpeed2 += 1.0f;
    }
    if (HuPadStkY[1] <= -36) {
        param->fadeOutSpeed1 -= 1.0f;
    } else if (HuPadStkY[1] >= 36) {
        param->fadeOutSpeed1 += 1.0f;
    }
    if ((HuPadBtn[0] & 1) != 0) {
        param->color.r++;
    }
    if ((HuPadBtn[0] & 2) != 0) {
        param->color.g++;
    }
    if ((HuPadBtn[0] & 8) != 0) {
        param->color.b++;
    }
    MdDebugFontPrint("pdebug->alpha  (%.2f)", param->baseAlpha);
    MdDebugFontPrint("pdebug->dalpha0(%.2f)", param->fadeInSpeed2);
    MdDebugFontPrint("pdebug->dalpha1(%.2f)", param->fadeOutSpeed1);
    MdDebugFontPrint("pdebug->len    (%.2f)", param->unk14);
    MdDebugFontPrint("pdebug->scale  (%.2f)", param->scaleSpeed1);
    MdDebugFontPrint("pdebug->dy     (%.2f)", param->unk20);
    MdDebugFontPrint("pdebug->red    (%d)", param->color.r);
    MdDebugFontPrint("pdebug->green  (%d)", param->color.g);
    MdDebugFontPrint("pdebug->blue   (%d)", param->color.b);
}

void MdRingMdlSet(NEL_SYS *sys, s32 no, s32 mdlNo);
void MdRingOfsSet(NEL_SYS *sys, s32 no, float x, float y, float z);
static void MdRingColorReset(NEL_SYS *sys, s32 no);
void MdRingDispSet(NEL_SYS *sys, volatile s32 no, s32 dispF);
static void MdRingScaleSet(NEL_SYS *sys, s32 no, float scale);

void MdRingCreate(NEL_SYS *sys)
{
    SYSRING *ring;
    s32 i;
    SYSRING_WORK *ringWork;
    SYSMODEL *model;

    ringWork = (SYSRING_WORK *)sys->ring;
    model = (SYSMODEL *)sys->model;
    ring = &ringWork->ring[0];
    for (i = 0; i < 11; i++, ring++) {
        ring->disp = 0;
        ring->mode = 0;
        ring->mdlNo = i + 0x27;
        ring->posP = &model->mdlPos[ring->mdlNo];
        ring->rotP = &model->mdlRot[ring->mdlNo];
        ring->scaleP = &model->mdlScale[ring->mdlNo];
        MdRingMdlSet(sys, i, -1);
        MdRingOfsSet(sys, i, 0.0f, 0.0f, 0.0f);
        if (i <= 3) {
            ring->color = RingColorTbl[i];
        } else if (i <= 6) {
            ring->color = RingColorTbl[4];
        } else {
            ring->color = RingColorTbl[5];
        }
        MdRingColorReset(sys, i);
    }
    ringWork->scale = 0.5f;
}

void MdRingMain(NEL_SYS *sys)
{
    SYSRING *ring;
    s32 i;
    HuVecF *posP;
    HuVecF *basePosP;
    s32 colorIdx;
    SYSRING_WORK *ringWork;
    SYSMODEL *model;
    SYSCAMERA_WORK *cameraWork;
    HuVecF *rotP;
    HuVecF *scaleP;
    SYSCAMERA *camera;

    ringWork = (SYSRING_WORK *)sys->ring;
    model = (SYSMODEL *)sys->model;
    cameraWork = (SYSCAMERA_WORK *)sys->camera;
    camera = &cameraWork->camera[2];
    ring = &ringWork->ring[0];
    for (i = 0; i < 11; i++, ring++) {
        posP = *ring->posP;
        rotP = *ring->rotP;
        scaleP = *ring->scaleP;
        if (ring->baseMdlNo != -1) {
            basePosP = model->mdlPos[ring->baseMdlNo];
            *posP = *basePosP;
            posP->x = basePosP->x + ring->baseOfs.x;
            posP->y = basePosP->y + ring->baseOfs.y;
            posP->z = basePosP->z + ring->baseOfs.z;
        }
        switch (ring->mode) {
            case 1:
                ring->baseOfs.x = 0.0f;
                ring->baseOfs.y = 0.0f;
                ring->baseOfs.z = 0.0f;
                MdRingDispSet(sys, i, 1);
                MdRingColorReset(sys, i);
                ring->color.a = 0;
                ring->mode = 2;
                break;
            case 2:
                MdMathOscillate(2, &ring->time, &ring->t, 16.0f);
                MathLerpColor(&ring->color.a, 0.0f, 192.0f, ring->t);
                if (ring->time >= 1.0f) {
                    MdRingColorReset(sys, i);
                    ring->mode = 3;
                }
                break;
            case 3:
                MdMathOscillate(3, &ring->time, &ring->t, 32.0f);
                MathLerpColor(&ring->color.a, 192.0f, 144.0f, ring->t);
                if (ring->time >= 1.0f) {
                    MdRingColorReset(sys, i);
                }
                break;
            case 4:
                MdRingColorReset(sys, i);
                ring->endColor = ring->color;
                ring->mode = 5;
                break;
            case 5:
                MdMathOscillate(1, &ring->time, &ring->t, 8.0f);
                MathLerpColor(&ring->color.a, ring->endColor.a, 0.0f, ring->t);
                if (ring->time >= 1.0f) {
                    MdRingDispSet(sys, i, 0);
                    MdRingColorReset(sys, i);
                    ring->color.a = 0;
                    ring->mode = 0;
                }
                break;
            case 6:
                MdRingDispSet(sys, i, 1);
                MdRingColorReset(sys, i);
                ring->color.a = 0;
                MdRingScaleSet(sys, i, ringWork->scale);
                ring->mode = 7;
                break;
            case 7:
                MdMathOscillate(1, &ring->time, &ring->t, 4.0f);
                MdMathOscillate(1, &ring->ofsTime, &ring->ofsT, 36.0f);
                MathLerpColor(&ring->color.a, 0.0f, 128.0f, ring->t);
                MdMathLerpScale(*ring->scaleP, 0.8f, 1.8f, ring->ofsT);
                if (ring->time >= 1.0f) {
                    ring->time = 0.0f;
                    ring->t = 0.0f;
                    ring->mode = 8;
                }
                break;
            case 8:
                MdMathOscillate(1, &ring->time, &ring->t, 32.0f);
                MdMathOscillate(1, &ring->ofsTime, &ring->ofsT, 36.0f);
                MathLerpColor(&ring->color.a, 128.0f, 0.0f, ring->t);
                MdMathLerpScale(*ring->scaleP, 0.8f, 1.8f, ring->ofsT);
                if (ring->time >= 1.0f) {
                    MdRingDispSet(sys, i, 0);
                    MdRingColorReset(sys, i);
                    ring->color.a = 0;
                    ring->mode = 0;
                }
                break;
            case 9:
                MdRingColorReset(sys, i);
                ring->endColor = ring->color;
                ring->mode = 10;
                break;
            case 10:
                MdMathOscillate(1, &ring->time, &ring->t, 8.0f);
                MathLerpColor(&ring->color.r, ring->endColor.r, RingColorTbl[6].r, ring->t);
                MathLerpColor(&ring->color.g, ring->endColor.g, RingColorTbl[6].g, ring->t);
                MathLerpColor(&ring->color.b, ring->endColor.b, RingColorTbl[6].b, ring->t);
                MathLerpColor(&ring->color.a, ring->endColor.a, 192.0f, ring->t);
                if (ring->time >= 1.0f) {
                    MdRingColorReset(sys, i);
                    ring->mode = 3;
                }
                break;
            case 11:
                MdRingColorReset(sys, i);
                ring->endColor = ring->color;
                ring->mode = 12;
                break;
            case 12:
                MdMathOscillate(1, &ring->time, &ring->t, 8.0f);
                MathLerpColor(&ring->color.r, ring->endColor.r, RingColorTbl[7].r, ring->t);
                MathLerpColor(&ring->color.g, ring->endColor.g, RingColorTbl[7].g, ring->t);
                MathLerpColor(&ring->color.b, ring->endColor.b, RingColorTbl[7].b, ring->t);
                MathLerpColor(&ring->color.a, ring->endColor.a, 192.0f, ring->t);
                if (ring->time >= 1.0f) {
                    MdRingColorReset(sys, i);
                    ring->mode = 3;
                }
                break;
            case 13:
                MdRingColorReset(sys, i);
                ring->endColor = ring->color;
                ring->mode = 14;
                break;
            case 14:
                colorIdx = 0;
                if (i <= 3) {
                    colorIdx = i;
                } else if (i <= 6) {
                    colorIdx = 4;
                } else {
                    colorIdx = 5;
                }
                MdMathOscillate(1, &ring->time, &ring->t, 15.0f);
                MathLerpColor(&ring->color.r, ring->endColor.r, RingColorTbl[colorIdx].r, ring->t);
                MathLerpColor(&ring->color.g, ring->endColor.g, RingColorTbl[colorIdx].g, ring->t);
                MathLerpColor(&ring->color.b, ring->endColor.b, RingColorTbl[colorIdx].b, ring->t);
                MathLerpColor(&ring->color.a, ring->endColor.a, 192.0f, ring->t);
                if (ring->time >= 1.0f) {
                    MdRingColorReset(sys, i);
                    ring->mode = 3;
                }
                break;
            case 15:
                if (i <= 3) {
                    ring->color = RingColorTbl[i];
                } else if (i <= 6) {
                    ring->color = RingColorTbl[4];
                }
                ring->mode = 0;
                break;
        }
    }
}

void MdRingModeSet(NEL_SYS *sys, s32 no, s32 mode)
{
    SYSRING *ring = &((SYSRING_WORK *)sys->ring)->ring[no];

    ring->mode = mode;
}

void MdRingDispSet(NEL_SYS *sys, volatile s32 no, s32 dispF)
{
    SYSRING *ring = &((SYSRING_WORK *)sys->ring)->ring[no];
    SYSMODEL *model = (SYSMODEL *)sys->model;

    ring->disp = dispF;
    if (dispF == 0) {
        Hu3DModelAttrSet(model->mdlId[ring->mdlNo], 1);
    } else if (dispF == 1) {
        Hu3DModelAttrReset(model->mdlId[ring->mdlNo], 1);
    }
}

static void MdRingScaleSet(NEL_SYS *sys, s32 no, float scale)
{
    SYSRING *ring = &((SYSRING_WORK *)sys->ring)->ring[no];

    (*ring->scaleP)->x = scale;
    (*ring->scaleP)->y = scale;
    (*ring->scaleP)->z = scale;
}

void MdRingMdlSet(NEL_SYS *sys, s32 no, s32 mdlNo)
{
    SYSRING *ring = &((SYSRING_WORK *)sys->ring)->ring[no];

    ring->baseMdlNo = mdlNo;
}

void MdRingOfsSet(NEL_SYS *sys, s32 no, float x, float y, float z)
{
    SYSRING *ring = &((SYSRING_WORK *)sys->ring)->ring[no];

    ring->baseOfs.x = x;
    ring->baseOfs.y = y;
    ring->baseOfs.z = z;
}

static void MdRingColorReset(NEL_SYS *sys, s32 no)
{
    SYSRING *ring = &((SYSRING_WORK *)sys->ring)->ring[no];

    ring->unk10 = 0;
    ring->time = 0.0f;
    ring->ofsTime = 0.0f;
    ring->t = 0.0f;
    ring->ofsT = 0.0f;
}

void MdFukiGrpSet(NEL_SYS *sys, s32 no, s32 grp);
void MdFukiMemberNoSet(NEL_SYS *sys, s32 no, s32 memberNo);
void MdFukiModeSet(NEL_SYS *sys, s32 no, s32 mode);

void MdFukiInit(NEL_SYS *sys)
{
    SYSFUKI *fuki;
    s32 i;
    SYSFUKI *fukiWork;

    fukiWork = (SYSFUKI *)sys->fuki;
    fuki = fukiWork;
    for (i = 0; i < 4; i++, fuki++) {
        fuki->mode = 0;
        fuki->time = 0.0f;
        fuki->ofsTime = 0.0f;
        fuki->t = 0.0f;
        fuki->ofsT = 0.0f;
        MdFukiGrpSet(sys, i, 3);
        MdFukiMemberNoSet(sys, i, i);
        MdFukiModeSet(sys, i, 0);
        fuki->hook = 0;
    }
}

void MdFukiMain(NEL_SYS *sys)
{
    SYSFUKI *fuki;
    HuVec2f *scale;
    s32 i;
    s16 grpId;
    SYSSPRITE *sprite;
    SYSFUKI *fukiWork;

    fukiWork = (SYSFUKI *)sys->fuki;
    sprite = (SYSSPRITE *)sys->sprite;
    fuki = fukiWork;
    for (i = 0; i < 4; i++, fuki++) {
        grpId = sprite->grpId[fuki->grp];
        scale = MdSpriteScaleGet(fuki->grp, fuki->memberNo);
        switch (fuki->mode) {
            case 0:
            case 3:
                break;
            case 1:
                HuSprAttrReset(grpId, fuki->memberNo, 4);
                scale->x = 0.0f;
                scale->y = 0.0f;
                fuki->time = 0.0f;
                fuki->ofsTime = 0.0f;
                fuki->t = 0.0f;
                fuki->ofsT = 0.0f;
                fuki->mode = 2;
                break;
            case 2:
                MdMathOscillate(1, &fuki->time, &fuki->t, 15.0f);
                MdMathOscillate(3, &fuki->ofsTime, &fuki->ofsT, 15.0f);
                MdMathLerpOfs(&scale->x, 0.0f, 1.0f, fuki->t, 0.5f * fuki->ofsT);
                scale->y = scale->x;
                if (fuki->time >= 1.0f) {
                    if (fuki->hook != 0) {
                        fuki->hook(sys, i);
                    }
                    fuki->mode = 3;
                }
                break;
            case 4:
                fuki->time = 0.0f;
                fuki->ofsTime = 0.0f;
                fuki->t = 0.0f;
                fuki->ofsT = 0.0f;
                fuki->mode = 5;
                break;
            case 5:
                MdMathOscillate(1, &fuki->time, &fuki->t, 15.0f);
                MdMathLerp(&scale->x, 1.0f, 0.0f, fuki->t);
                scale->y = scale->x;
                if (fuki->time >= 1.0f) {
                    HuSprAttrSet(grpId, fuki->memberNo, 4);
                    if (fuki->hook != 0) {
                        fuki->hook(sys, i);
                    }
                    fuki->mode = 0;
                }
                break;
        }
    }
}

void MdFukiGrpSet(NEL_SYS *sys, s32 no, s32 grp)
{
    SYSFUKI *fuki = &((SYSFUKI *)sys->fuki)[no];

    fuki->grp = grp;
}

void MdFukiMemberNoSet(NEL_SYS *sys, s32 no, s32 memberNo)
{
    SYSFUKI *fuki = &((SYSFUKI *)sys->fuki)[no];

    fuki->memberNo = memberNo;
}

void MdFukiModeSet(NEL_SYS *sys, s32 no, s32 mode)
{
    SYSFUKI *fuki = &((SYSFUKI *)sys->fuki)[no];

    fuki->mode = mode;
}

void MdFukiHookSet(NEL_SYS *sys, s32 no, SYSFUKI_HOOK hook)
{
    SYSFUKI *fuki = &((SYSFUKI *)sys->fuki)[no];

    fuki->hook = hook;
}

s32 MdFukiModeGet(NEL_SYS *sys, s32 no)
{
    SYSFUKI *fuki = &((SYSFUKI *)sys->fuki)[no];

    return fuki->mode;
}

static void ToueiZClear(void)
{
    HU3DCAMERA *cameraP;
    Mtx44 proj;
    Mtx modelview;
    HuVecF pos;
    HuVecF target;
    HuVecF up;
    GXColor color;

    cameraP = &Hu3DCamera[Hu3DCameraNo];
    MTXPerspective(proj, cameraP->fov, cameraP->aspect, cameraP->near, cameraP->far);
    GXSetProjection(proj, 0);
    GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    GXSetScissor(0, 0, 640, 480);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    color.r = color.g = color.b = 0;
#ifdef TARGET_PC
    color.a = 0;
#endif
    GXSetTevColor(1, color);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 2);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(0, 7, 7, 7, 6);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetCullMode(0);
    GXSetAlphaCompare(6, 0, 0, 6, 0);
    GXSetZMode(1, 7, 1);
    pos.x = pos.y = pos.z = 0.0f;
    target.x = target.y = 0.0f;
    target.z = -100.0f;
    up.x = up.z = 0.0f;
    up.y = 1.0f;
    MTXLookAt(modelview, &pos, &up, &target);
    GXLoadPosMtxImm(modelview, 0);
    pos.x = 1.2000000476837158 * (10000.0 * -sin(cameraP->fov * 0.5f));
    pos.y = 0.8333333134651184 * (10000.0 * -sin(cameraP->fov * 0.5f));
    GXBegin(0x80, 0, 4);
    GXPosition3f32(-pos.x, -pos.y, -10000.0f);
    GXPosition3f32(pos.x, -pos.y, -10000.0f);
    GXPosition3f32(pos.x, pos.y, -10000.0f);
    GXPosition3f32(-pos.x, pos.y, -10000.0f);
}
