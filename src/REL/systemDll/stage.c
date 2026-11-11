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
static void MdSpriteDispSet(s32 grpNo, s32 dispF);

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

static void MdSpriteDispSet(s32 grpNo, s32 dispF)
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
