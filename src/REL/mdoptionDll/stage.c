#include "REL/mdoptionDll/mdoption.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/mgdata.h"

#define MDOPT_MES(no) (0x001F0000 | (no))

typedef struct MdSprInfo_s {
    s16 animNo; //0x00
    s16 pri; //0x02
    HuVec2f pos; //0x04
    HuVec2f scale; //0x0C
    float tpLvl; //0x14
    int bank; //0x18
} MDSPR_INFO;

typedef struct MdSprGrpInfo_s {
    s16 start; //0x00
    s16 end; //0x02
    float posX; //0x04
    float posY; //0x08
} MDSPR_GRP_INFO;

typedef struct MdOptMgRecord_s {
    s32 mgMsg; //0x00
    s32 nameMsg; //0x04
    int type; //0x08
    int recNo; //0x0C
} MDOPT_MG_RECORD;

/* sprite groups (MDSPRITE_WORK.grpId) */
enum {
    MDOPT_GRP_BASE, //0 window frame
    MDOPT_GRP_FADE, //1
    MDOPT_GRP_BOARD, //2 board records
    MDOPT_GRP_STORY, //3 story records
    MDOPT_GRP_SD, //4 sd records
    MDOPT_GRP_DECA, //5 decathlon records
    MDOPT_GRP_DECA_TOTAL, //6
    MDOPT_GRP_TITLE //7
};

extern void MdMessWinClose(MDMESS_WORK *mess, int winNo);
extern void MdMessWinPosSet(MDMESS_WORK *mess, int winNo, float posX, float posY);
extern void MdMessWinScaleSet(MDMESS_WORK *mess, int winNo, float scaleX, float scaleY);
extern void MdMessWinSpeedSet(MDMESS_WORK *mess, int winNo, s16 speed);
extern void MdMessWinAttrSet(MDMESS_WORK *mess, int winNo, u32 attr);
extern void MdMessWinAttrReset(MDMESS_WORK *mess, int winNo, u32 attr);
extern void MdMessWinMesSet(MDMESS_WORK *mess, int winNo, int msg);
extern void MdMessWinHomeClear(MDMESS_WORK *mess, int winNo);
extern void MdMessHelpWinCreate(MDMESS_WORK *mess, int winNo, int msg, float x, float y, int w, int h, float z, int type);
extern void MdMessWinInsertMesSet(MDMESS_WORK *mess, int winNo, int msg, int insert);
extern void MdMessWinInsertMultiMesSet(MDMESS_WORK *mess, int winNo, int msg, int *list);
extern void MdMathOscillate(int type, float *in, float *out, float t);

void MdOptionSprGrpCreate(MDSPRITE_WORK *sprite, int grpNo);
void MdOptionSprGrpKill(MDSPRITE_WORK *sprite, int grpNo);
void MdOptionModeSpriteKill(MDSPRITE_WORK *sprite);
void MdOptionTextListUpdate(u32 type, int msg, u16 value);
HuVecF *MdSpriteGrpPosGet(MDSPRITE_WORK *sprite, int grpNo);
HuVecF *MdSpriteGrpScaleGet(MDSPRITE_WORK *sprite, int grpNo);
void MdOptionWaveEffectModelCreate(void);
void MdModelUpdateAmb(MDOPT_MODEL_WORK *model);
void MdModelRumbleSet(int type);
static void FontBGFilter(HU3DMODEL *modelP, Mtx *mtx);
static void LayerHook(s16 layerNo);
static void WaveEffectHook(s16 layerNo);
static void WaveEffectDrawHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
void MdOptionWaveEffectIndTexMtxSet(HuVecF *a, HuVecF *b);
void MdOptionWaveEffectColorSet(GXColor *color);
void MdOptionWaveEffectSpeedSet(float speed);
void MdOptionListCursorPosSet(int a, int b, int c);
void MdOptionRecordCursorPosSet(int a, int *list);
void MdOptionRecordCursorClamp(int *sel);

static unsigned int SpriteAnimFile[MDSPR_ANIM_MAX] = {
    0x00920026, 0x00920027, 0x00920033, 0x0092004B,
    0x00920028, 0x0092004E, 0x0092002C, 0x0092002D,
    0x0092002E, 0x0092002F, 0x00920029, 0x0092002A,
    0x0092002B, 0x00920030, 0x00920031, 0x00920032,
    0x00920034, 0x00920035, 0x00920036, 0x00920037,
    0x00920038, 0x00920039, 0x0092003A, 0x0092003B,
    0x0092003C, 0x0092003D, 0x0092003E, 0x0092003F,
    0x00920040, 0x00920041, 0x00920042, 0x0092004A,
    0x00920043, 0x00920044, 0x00920045, 0x00920046,
    0x00920048, 0x00920047, 0x00920049, 0x0092004F,
};

static MDSPR_INFO SpriteInfo[MDSPR_MAX] = {
    { 0, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 1, 2, { 0.0f, -126.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 2, 2, { 0.0f, -128.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 4, 2, { -206.0f, -108.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 4, 2, { 206.0f, -108.0f }, { 1.0f, 1.0f }, 1.0f, 2 },
    { 16, 2, { -188.0f, -132.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 16, 2, { 188.0f, -132.0f }, { 1.0f, 1.0f }, 1.0f, 2 },
    { 17, 2, { -188.0f, -132.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 17, 2, { 188.0f, -132.0f }, { 1.0f, 1.0f }, 1.0f, 2 },
    { 3, 2, { 0.0f, 0.0f }, { 72.0f, 60.0f }, 0.5f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 15, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 5, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 5, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 5, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 5, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 5, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 5, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 5, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 6, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 7, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 8, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 14, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 14, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 13, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 13, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 13, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 13, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 13, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 13, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 13, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 13, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 13, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 13, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 28, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 13, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 18, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 11, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 29, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 30, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 31, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 32, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 33, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 34, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 35, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 36, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 37, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 38, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 10, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
    { 39, 2, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 1.0f, 0 },
};

static MDSPR_GRP_INFO SpriteGrpInfo[MDSPR_GRP_MAX] = {
    { 0, 8, 288.0f, 190.0f },
    { 9, 9, 288.0f, 240.0f },
    { 10, 49, 288.0f, 190.0f },
    { 50, 89, 288.0f, 190.0f },
    { 90, 118, 288.0f, 190.0f },
    { 119, 188, 288.0f, 190.0f },
    { 189, 245, 288.0f, 190.0f },
    { 246, 246, 288.0f, -60.0f },
};

void MdSpriteCreate(MDSPRITE_WORK *sprite)
{
    int i;
    for (i = 0; i < MDSPR_ANIM_MAX; i++) {
        sprite->animP[i] = HuSprAnimRead(HuDataReadNum(SpriteAnimFile[i], HU_MEMNUM_OVL));
        HuSprAnimLock(sprite->animP[i]);
    }
    MdOptionSprGrpCreate(sprite, 0);
    MdOptionSprGrpCreate(sprite, 1);
    MdOptionSprGrpCreate(sprite, 7);
    HuSprDrawNoSet(sprite->grpId[1], 0, 0x3F);
    sprite->fadeAlpha = 0.6f;
    HuSprExecLayerCameraSet(0x40, sysP->camera->camera[MDCAMERA_BG].bit, 5);
    HuSprExecLayerCameraSet(0x3F, sysP->camera->camera[MDCAMERA_BG].bit, 2);
}

void MdSpriteFadeDispSet(BOOL dispF)
{
    BOOL f = dispF;
    if (f == FALSE) {
        HuSprAttrSet(sysP->sprite->grpId[1], 0, HUSPR_ATTR_DISPOFF);
        return;
    }
    HuSprAttrReset(sysP->sprite->grpId[1], 0, HUSPR_ATTR_DISPOFF);
}

void MdOptionModeSpriteKill(MDSPRITE_WORK *sprite)
{
    MDMESS_WORK *mess;
    MDOPT_MODEL_WORK *model;
    HUSPRGRPID grpId;
    int i;
    int j;
    mess = sysP->mess;
    model = sysP->model;
    grpId = sprite->grpId[MDOPT_GRP_BASE];
    switch (sysP->list) {
        case 1:
        case 2:
            MdOptionSprGrpKill(sprite, MDOPT_GRP_BOARD);
            break;
        case 3:
        case 4:
            MdOptionSprGrpKill(sprite, MDOPT_GRP_STORY);
            break;
        case 7:
            MdOptionSprGrpKill(sprite, MDOPT_GRP_SD);
            break;
        case 5:
            MdOptionSprGrpKill(sprite, MDOPT_GRP_DECA);
            break;
        case 6:
            MdOptionSprGrpKill(sprite, MDOPT_GRP_DECA_TOTAL);
            break;
        case 0:
        case 8:
            break;
    }
    HuSprAttrSet(grpId, 0, HUSPR_ATTR_DISPOFF);
    HuSprAttrSet(grpId, 1, HUSPR_ATTR_DISPOFF);
    HuSprAttrSet(grpId, 2, HUSPR_ATTR_DISPOFF);
    HuSprAttrSet(grpId, 3, HUSPR_ATTR_DISPOFF);
    HuSprAttrSet(grpId, 4, HUSPR_ATTR_DISPOFF);
    HuSprAttrSet(grpId, 5, HUSPR_ATTR_DISPOFF);
    HuSprAttrSet(grpId, 6, HUSPR_ATTR_DISPOFF);
    HuSprAttrSet(grpId, 7, HUSPR_ATTR_DISPOFF);
    HuSprAttrSet(grpId, 8, HUSPR_ATTR_DISPOFF);
    for (i = 2; i < 18; i++) {
        MdMessWinAttrSet(mess, i, 8);
        MdMessWinScaleSet(mess, i, 0.0f, 0.0f);
        MdMessWinHomeClear(mess, i);
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 7; j++) {
            Hu3DModelAttrSet(model->mdlId[i * 7 + j + 0x1D], HU3D_ATTR_DISPOFF);
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 0x23; j++) {
            Hu3DModelAttrSet(model->mdlId[i * 0x23 + j + 0x39], HU3D_ATTR_DISPOFF);
        }
    }
    sysP->list = 0;
}

void MdOptionSprGrpCreate(MDSPRITE_WORK *sprite, int grpNo)
{
    MDSPR_GRP_INFO *grpInfo;
    MDSPR_INFO *sprInfo;
    HUSPRGRPID grpId;
    s16 grpNum;
    s16 i;
    int j;
    grpInfo = &SpriteGrpInfo[grpNo];
    grpNum = grpInfo->end - grpInfo->start + 1;
    grpId = HuSprGrpCreate(grpNum);
    sprite->grpId[grpNo] = grpId;
    sprInfo = &SpriteInfo[grpInfo->start];
    switch (grpNo) {
        case 4:
            sprInfo[1].animNo = 7;
            sprInfo[2].animNo = 8;
            break;
        case 5:
            for (j = 0; j < 10; j++) {
                if (GwCommon.decaScore[j].charNo == -1) {
                    sprInfo[j * 7 + 1].animNo = 0xC;
                } else {
                    sprInfo[j * 7 + 1].animNo = 0x1C;
                }
            }
            break;
        case 6:
            sprInfo[1].animNo = GwCommon.decaScore[sysP->sub].charNo + 0x12;
            break;
    }
    for (i = 0; i < grpNum; i++, sprInfo++) {
        sprite->sprId[i + grpInfo->start] = HuSprCreate(sprite->animP[sprInfo->animNo], 0, sprInfo->bank);
        HuSprGrpMemberSet(grpId, i, sprite->sprId[i + grpInfo->start]);
        HuSprPriSet(grpId, i, sprInfo->pri);
        HuSprAttrSet(grpId, i, HUSPR_ATTR_DISPOFF);
        HuSprPosSet(grpId, i, sprInfo->pos.x, sprInfo->pos.y);
        HuSprZRotSet(grpId, i, 0.0f);
        HuSprScaleSet(grpId, i, sprInfo->scale.x, sprInfo->scale.y);
        HuSprColorSet(grpId, i, 255, 255, 255);
        HuSprTPLvlSet(grpId, i, sprInfo->tpLvl);
    }
    HuSprGrpPosSet(grpId, grpInfo->posX, grpInfo->posY);
    HuSprGrpCenterSet(grpId, 0.0f, 0.0f);
    HuSprGrpZRotSet(grpId, 0.0f);
    HuSprGrpScaleSet(grpId, 1.0f, 1.0f);
    HuSprGrpDrawNoSet(grpId, 0x40);
    HuSprGrpScissorSet(grpId, 0, 0, 640, 480);
}

void MdOptionSprGrpKill(MDSPRITE_WORK *sprite, int grpNo)
{
    int no = grpNo;
    HuSprGrpKill(sprite->grpId[no]);
}

void MdOptionTitlePosSet(MDSPRITE_WORK *sprite, HuVecF *pos)
{
    HUSPRGRPID grpId = sprite->grpId[MDOPT_GRP_TITLE];
    HuVecF *grpPos = MdSpriteGrpPosGet(sprite, MDOPT_GRP_TITLE);
    grpPos->x = pos->x;
    grpPos->y = pos->y;
    if (pos->y < -56.0f) {
        HuSprAttrSet(grpId, 0, HUSPR_ATTR_DISPOFF);
        return;
    }
    HuSprAttrReset(grpId, 0, HUSPR_ATTR_DISPOFF);
}

static HuVecF BoardRecordSprPosTbl[4] = {
    { 0.0f, 0.0f, 0.0f },
    { -16.0f, 46.0f, 0.0f },
    { 0.0f, 46.0f, 0.0f },
    { 16.0f, 46.0f, 0.0f },
};

void MdOptionBoardRecordUpdate(MDSPRITE_WORK *sprite, int board)
{
    MDMESS_WORK *mess;
    HUSPRGRPID grp0;
    HUSPRGRPID recGrp;
    HuVecF *pos0;
    HuVecF *scale0;
    HuVecF *recPos;
    s16 i;
    int j;
    u16 val;
    mess = sysP->mess;
    grp0 = sprite->grpId[MDOPT_GRP_BASE];
    pos0 = MdSpriteGrpPosGet(sysP->sprite, MDOPT_GRP_BASE);
    scale0 = MdSpriteGrpScaleGet(sysP->sprite, MDOPT_GRP_BASE);
    if (sysP->list != 1) {
        MdOptionModeSpriteKill(sprite);
        MdOptionSprGrpCreate(sprite, MDOPT_GRP_BOARD);
        HuSprAttrReset(grp0, 0, HUSPR_ATTR_DISPOFF);
        HuSprAttrReset(grp0, 1, HUSPR_ATTR_DISPOFF);
        HuSprAttrReset(grp0, 3, HUSPR_ATTR_DISPOFF);
        HuSprAttrReset(grp0, 4, HUSPR_ATTR_DISPOFF);
        MdMessWinAttrReset(mess, 2, 8);
        MdMessWinAttrReset(mess, 3, 8);
        MdMessWinAttrSet(mess, 2, 0x800);
        MdMessWinAttrSet(mess, 3, 0x800);
        sysP->list = 1;
    }
    recGrp = sprite->grpId[MDOPT_GRP_BOARD];
    recPos = MdSpriteGrpPosGet(sysP->sprite, MDOPT_GRP_BOARD);
    *recPos = *pos0;
    *MdSpriteGrpScaleGet(sysP->sprite, MDOPT_GRP_BOARD) = *scale0;
    for (i = 0; i < 10; i++) {
        HuSprAttrReset(recGrp, i, HUSPR_ATTR_DISPOFF);
        HuSprPosSet(recGrp, i, BoardRecordSprPosTbl[0].x + (float)((i % 5) * 95 - 190), BoardRecordSprPosTbl[0].y + (float)((i / 5) * 110 - 40));
        HuSprBankSet(recGrp, i, i);
        val = GwCommon.charPlayNum[board][i];
        if (val > 999) {
            val = 999;
        }
        for (j = 0; j < 3; j++) {
            HuSprAttrReset(recGrp, i * 3 + j + 10, HUSPR_ATTR_DISPOFF);
            HuSprPosSet(recGrp, i * 3 + j + 10, (float)((i % 5) * 95 - 190) + BoardRecordSprPosTbl[j + 1].x, (float)((i / 5) * 110 - 40) + BoardRecordSprPosTbl[j + 1].y);
            switch (j) {
                case 0:
                    HuSprBankSet(recGrp, i * 3 + j + 10, val / 100);
                    break;
                case 1:
                    HuSprBankSet(recGrp, i * 3 + j + 10, (val % 100) / 10);
                    break;
                case 2:
                    HuSprBankSet(recGrp, i * 3 + j + 10, val % 10);
                    break;
            }
        }
    }
    MdMessWinScaleSet(mess, 2, scale0->x, scale0->y);
    MdMessWinPosSet(mess, 2, -155.0f * scale0->x + pos0->x, -147.0f * scale0->y + pos0->y);
    MdMessWinScaleSet(mess, 3, scale0->x, scale0->y);
    MdMessWinPosSet(mess, 3, -288.0f * scale0->x + pos0->x, -112.0f * scale0->y + pos0->y);
    if (board == 6 && GwCommon.w07Unlock == 0) {
        MdMessWinMesSet(mess, 2, MDOPT_MES(0x51));
    } else {
        MdMessWinMesSet(mess, 2, MDOPT_MES(0x1D) + board);
    }
    val = GwCommon.playNum[board];
    if (val > 9999) {
        val = 9999;
    }
    MdOptionTextListUpdate(10, MDOPT_MES(0x3F), val);
    MdMessWinInsertMesSet(mess, 3, MDOPT_MES(0x25), MDOPT_MES(0x3F));
}

void MdOptionBoardTotalRecordUpdate(MDSPRITE_WORK *sprite)
{
    MDMESS_WORK *mess;
    HUSPRGRPID grp0;
    HUSPRGRPID recGrp;
    HuVecF *pos0;
    HuVecF *scale0;
    HuVecF *recPos;
    s16 i;
    int j;
    int k;
    int total;
    mess = sysP->mess;
    grp0 = sprite->grpId[MDOPT_GRP_BASE];
    pos0 = MdSpriteGrpPosGet(sysP->sprite, MDOPT_GRP_BASE);
    scale0 = MdSpriteGrpScaleGet(sysP->sprite, MDOPT_GRP_BASE);
    if (sysP->list != 2) {
        MdOptionModeSpriteKill(sprite);
        MdOptionSprGrpCreate(sprite, MDOPT_GRP_BOARD);
        HuSprAttrReset(grp0, 0, HUSPR_ATTR_DISPOFF);
        HuSprAttrReset(grp0, 1, HUSPR_ATTR_DISPOFF);
        HuSprAttrReset(grp0, 3, HUSPR_ATTR_DISPOFF);
        HuSprAttrReset(grp0, 4, HUSPR_ATTR_DISPOFF);
        MdMessWinAttrReset(mess, 2, 8);
        MdMessWinAttrReset(mess, 3, 8);
        MdMessWinAttrSet(mess, 2, 0x800);
        MdMessWinAttrSet(mess, 3, 0x800);
        MdMessWinMesSet(mess, 2, MDOPT_MES(0x24));
        total = 0;
        for (j = 0; j < 7; j++) {
            total += GwCommon.playNum[j];
        }
        if (total > 9999) {
            total = 9999;
        }
        MdOptionTextListUpdate(10, MDOPT_MES(0x3F), total);
        MdMessWinInsertMesSet(mess, 3, MDOPT_MES(0x25), MDOPT_MES(0x3F));
        sysP->list = 2;
    }
    recGrp = sprite->grpId[MDOPT_GRP_BOARD];
    recPos = MdSpriteGrpPosGet(sysP->sprite, MDOPT_GRP_BOARD);
    *recPos = *pos0;
    *MdSpriteGrpScaleGet(sysP->sprite, MDOPT_GRP_BOARD) = *scale0;
    for (i = 0; i < 10; i++) {
        HuSprAttrReset(recGrp, i, HUSPR_ATTR_DISPOFF);
        HuSprPosSet(recGrp, i, BoardRecordSprPosTbl[0].x + (float)((i % 5) * 95 - 190), BoardRecordSprPosTbl[0].y + (float)((i / 5) * 110 - 40));
        HuSprBankSet(recGrp, i, i);
        total = 0;
        for (j = 0; j < 7; j++) {
            total += GwCommon.charPlayNum[j][i];
        }
        if (total > 999) {
            total = 999;
        }
        for (k = 0; k < 3; k++) {
            HuSprAttrReset(recGrp, i * 3 + k + 10, HUSPR_ATTR_DISPOFF);
            HuSprPosSet(recGrp, i * 3 + k + 10, (float)((i % 5) * 95 - 190) + BoardRecordSprPosTbl[k + 1].x, (float)((i / 5) * 110 - 40) + BoardRecordSprPosTbl[k + 1].y);
            switch (k) {
                case 0:
                    HuSprBankSet(recGrp, i * 3 + k + 10, total / 100);
                    break;
                case 1:
                    HuSprBankSet(recGrp, i * 3 + k + 10, (total % 100) / 10);
                    break;
                case 2:
                    HuSprBankSet(recGrp, i * 3 + k + 10, total % 10);
                    break;
            }
        }
    }
    MdMessWinScaleSet(mess, 2, scale0->x, scale0->y);
    MdMessWinPosSet(mess, 2, -155.0f * scale0->x + pos0->x, -147.0f * scale0->y + pos0->y);
    MdMessWinScaleSet(mess, 3, scale0->x, scale0->y);
    MdMessWinPosSet(mess, 3, -288.0f * scale0->x + pos0->x, -112.0f * scale0->y + pos0->y);
}

int MdOptionStoryRankCalc(int charNo)
{
    int i;
    int total = 0;
    int rank;
    for (i = 0; i < 7; i++) {
        if (GwCommon.charStoryPlayNum[i][charNo] != 0) {
            total += GwCommon.charStoryRank[i][charNo] + 1;
        }
    }
    if (total == 0) {
        rank = 0;
    } else if (total <= 8) {
        rank = 1;
    } else if (total <= 15) {
        rank = 2;
    } else if (total <= 22) {
        rank = 3;
    } else if (total <= 27) {
        rank = 4;
    } else {
        rank = 5;
    }
    return rank;
}


static HuVecF StoryRecordSprPosTbl[9] = {
    { 0.0f, 0.0f, 0.0f },
    { -16.0f, 43.0f, 0.0f },
    { 0.0f, 43.0f, 0.0f },
    { 16.0f, 43.0f, 0.0f },
    { -18.0f, -34.0f, 0.0f },
    { 0.0f, -34.0f, 0.0f },
    { 18.0f, -34.0f, 0.0f },
    { -9.0f, -49.0f, 0.0f },
    { 9.0f, -49.0f, 0.0f },
};

void MdOptionStoryRecordUpdate(MDSPRITE_WORK *sprite)
{
    MDOPT_MODEL_WORK *model;
    MDCAMERA_WORK *camera;
    MDMESS_WORK *mess;
    HUSPRGRPID grp0;
    HuVecF *pos0;
    HuVecF *scale0;
    HUSPRGRPID recGrp;
    HuVecF *recPos;
    HuVecF *recScale;
    s16 i;
    int j;
    int total;
    int rank;
    int idx;
    u16 val;
    float x;
    float y;
    HuVecF pos2d;
    HuVecF pos3d;
    model = sysP->model;
    camera = sysP->camera;
    mess = sysP->mess;
    grp0 = sprite->grpId[MDOPT_GRP_BASE];
    pos0 = MdSpriteGrpPosGet(sysP->sprite, MDOPT_GRP_BASE);
    scale0 = MdSpriteGrpScaleGet(sysP->sprite, MDOPT_GRP_BASE);
    if (sysP->list != 3) {
        MdOptionModeSpriteKill(sprite);
        MdOptionSprGrpCreate(sprite, MDOPT_GRP_STORY);
        HuSprAttrReset(grp0, 0, HUSPR_ATTR_DISPOFF);
        HuSprAttrReset(grp0, 1, HUSPR_ATTR_DISPOFF);
        MdMessWinAttrReset(mess, 2, 8);
        MdMessWinAttrReset(mess, 3, 8);
        MdMessWinAttrSet(mess, 2, 0x800);
        MdMessWinAttrSet(mess, 3, 0x800);
        MdMessWinMesSet(mess, 2, MDOPT_MES(0x28));
        total = 0;
        for (j = 0; j < 7; j++) {
            total += GwCommon.storyPlayNum[j];
        }
        if (total > 9999) {
            total = 9999;
        }
        MdOptionTextListUpdate(10, MDOPT_MES(0x3F), total);
        MdMessWinInsertMesSet(mess, 3, MDOPT_MES(0x25), MDOPT_MES(0x3F));
        sysP->list = 3;
    }
    recGrp = sprite->grpId[MDOPT_GRP_STORY];
    recPos = MdSpriteGrpPosGet(sysP->sprite, MDOPT_GRP_STORY);
    recScale = MdSpriteGrpScaleGet(sysP->sprite, MDOPT_GRP_STORY);
    *recPos = *pos0;
    *recScale = *scale0;
    for (i = 0; i < 7; i++) {
        x = (float)((i % 4) * 0x60 + (i / 4) * 0x30 - 0x90);
        y = (float)((i / 4) * 0x6E - 0x12);
        HuSprAttrReset(recGrp, i * 4 + 1, HUSPR_ATTR_DISPOFF);
        HuSprPosSet(recGrp, i * 4 + 1, x + StoryRecordSprPosTbl[0].x, y + StoryRecordSprPosTbl[0].y);
        HuSprBankSet(recGrp, i * 4 + 1, i);
        val = GwCommon.storyPlayNum[i];
        if (val > 999) {
            val = 999;
        }
        for (j = 0; j < 3; j++) {
            switch (j) {
                case 0:
                    HuSprBankSet(recGrp, i * 4 + j + 2, val / 100);
                    break;
                case 1:
                    HuSprBankSet(recGrp, i * 4 + j + 2, (val % 100) / 10);
                    break;
                default:
                    HuSprBankSet(recGrp, i * 4 + j + 2, val % 10);
                    break;
            }
            HuSprAttrReset(recGrp, i * 4 + j + 2, HUSPR_ATTR_DISPOFF);
            HuSprPosSet(recGrp, i * 4 + j + 2, x + StoryRecordSprPosTbl[j + 1].x, y + StoryRecordSprPosTbl[j + 1].y);
        }
        rank = MdOptionStoryRankCalc(i);
        for (j = 0; j < 5; j++) {
            if (j < rank) {
                idx = i * 5 + j + 0x39;
            } else {
                idx = i * 5 + j + 0x5C;
            }
            Hu3DModelAttrReset(model->mdlId[idx], HU3D_ATTR_DISPOFF);
            pos2d.x = 0.0f;
            pos2d.y = 0.0f;
            pos2d.z = 2000.0f;
            pos2d.x = pos0->x + scale0->x * (x + StoryRecordSprPosTbl[j + 4].x);
            pos2d.y = pos0->y + scale0->y * (y + StoryRecordSprPosTbl[j + 4].y);
            pos3d = pos2d;
            Hu3D2Dto3D(&pos3d, camera->camera[MDCAMERA_BG].bit, &pos3d);
            Hu3DModelPosSetV(model->mdlId[idx], &pos3d);
            Hu3DModelScaleSet(model->mdlId[idx], 0.28f * scale0->x, 0.28f * scale0->y, 0.28f * scale0->y);
        }
    }
    MdMessWinScaleSet(mess, 2, scale0->x, scale0->y);
    MdMessWinPosSet(mess, 2, -155.0f * scale0->x + pos0->x, -147.0f * scale0->y + pos0->y);
    MdMessWinScaleSet(mess, 3, scale0->x, scale0->y);
    MdMessWinPosSet(mess, 3, -288.0f * scale0->x + pos0->x, -112.0f * scale0->y + pos0->y);
}

static HuVecF StoryTotalRecordSprPosTbl[11] = {
    { 0.0f, 0.0f, 0.0f },
    { -16.0f, 52.0f, 0.0f },
    { 0.0f, 52.0f, 0.0f },
    { 16.0f, 52.0f, 0.0f },
    { 288.0f, 144.0f, 0.0f },
    { 260.0f, 140.0f, 0.0f },
    { 288.0f, 140.0f, 0.0f },
    { 316.0f, 140.0f, 0.0f },
    { 274.0f, 114.0f, 0.0f },
    { 302.0f, 114.0f, 0.0f },
    { 0.0f, -10.0f, 0.0f },
};

void MdOptionStoryTotalRecordUpdate(MDSPRITE_WORK *sprite, s16 charNo)
{
    MDOPT_MODEL_WORK *model;
    MDCAMERA_WORK *camera;
    MDMESS_WORK *mess;
    HUSPRGRPID grp0;
    HUSPRGRPID recGrp;
    int i;
    int j;
    int rank;
    int idx;
    int bank;
    int clearF;
    u16 val;
    float x;
    float y;
    HuVecF pos2d;
    HuVecF pos3d;
    model = sysP->model;
    camera = sysP->camera;
    mess = sysP->mess;
    grp0 = sprite->grpId[MDOPT_GRP_BASE];
    if (sysP->list != 4) {
        MdOptionModeSpriteKill(sprite);
        MdOptionSprGrpCreate(sprite, MDOPT_GRP_STORY);
        MdMessWinScaleSet(mess, 2, 1.0f, 1.0f);
        MdMessWinAttrReset(mess, 2, 8);
        MdMessWinAttrSet(mess, 2, 0x800);
        MdMessWinMesSet(mess, 2, MDOPT_MES(0x29));
        sysP->list = 4;
        recGrp = sprite->grpId[MDOPT_GRP_STORY];
        HuSprAttrReset(grp0, 0, HUSPR_ATTR_DISPOFF);
        HuSprAttrReset(grp0, 1, HUSPR_ATTR_DISPOFF);
        HuSprAttrReset(recGrp, 0, HUSPR_ATTR_DISPOFF);
        HuSprPosSet(recGrp, 0, -204.0f + StoryTotalRecordSprPosTbl[10].x, 15.0f + StoryTotalRecordSprPosTbl[10].y);
        HuSprAttrReset(recGrp, 0x1D, HUSPR_ATTR_DISPOFF);
        HuSprPosSet(recGrp, 0x1D, -204.0f + StoryTotalRecordSprPosTbl[0].x, 15.0f + StoryTotalRecordSprPosTbl[0].y);
        HuSprBankSet(recGrp, 0x1D, charNo);
        val = GwCommon.storyPlayNum[charNo];
        if (val > 999) {
            val = 999;
        }
        rank = MdOptionStoryRankCalc(charNo);
        for (i = 0; i < 5; i++) {
            if (i < rank) {
                idx = i + 0x39;
            } else {
                idx = i + 0x5C;
            }
            Hu3DModelAttrReset(model->mdlId[idx], HU3D_ATTR_DISPOFF);
            pos2d.x = 0.0f;
            pos2d.y = 0.0f;
            pos2d.z = 2000.0f;
            pos2d.x = -204.0f + StoryTotalRecordSprPosTbl[i + 5].x;
            pos2d.y = 15.0f + StoryTotalRecordSprPosTbl[i + 5].y;
            pos3d = pos2d;
            Hu3D2Dto3D(&pos3d, camera->camera[MDCAMERA_BG].bit, &pos3d);
            Hu3DModelPosSetV(model->mdlId[idx], &pos3d);
            Hu3DModelScaleSet(model->mdlId[idx], 0.4f, 0.4f, 0.4f);
        }
        for (i = 0; i < 3; i++) {
            switch (i) {
                case 0:
                    HuSprBankSet(recGrp, i + 0x1E, val / 100);
                    break;
                case 1:
                    HuSprBankSet(recGrp, i + 0x1E, (val % 100) / 10);
                    break;
                default:
                    HuSprBankSet(recGrp, i + 0x1E, val % 10);
                    break;
            }
            HuSprAttrReset(recGrp, i + 0x1E, HUSPR_ATTR_DISPOFF);
            HuSprPosSet(recGrp, i + 0x1E, -204.0f + StoryTotalRecordSprPosTbl[i + 1].x, 15.0f + StoryTotalRecordSprPosTbl[i + 1].y);
        }
        clearF = 0;
        for (i = 0; i < 7; i++) {
            if (GwCommon.charStoryPlayNum[6][i] != 0) {
                clearF = 1;
            }
        }
        for (i = 0; i < 7; i++) {
            if (i < 3) {
                x = (float)(i * 0x68 - 0x50);
                y = -10.0f;
            } else {
                x = (float)((i - 3) * 0x68 - 0x80);
                y = 115.0f;
            }
            HuSprAttrReset(recGrp, i + 0x21, HUSPR_ATTR_DISPOFF);
            HuSprPosSet(recGrp, i + 0x21, x + StoryTotalRecordSprPosTbl[0].x, y + StoryTotalRecordSprPosTbl[0].y);
            if (i == 6 && clearF == 0) {
                bank = 7;
            } else {
                bank = i;
            }
            HuSprBankSet(recGrp, i + 0x21, bank);
            if (GwCommon.charStoryPlayNum[i][charNo] != 0) {
                idx = GwCommon.charStoryRank[i][charNo] * 7 + 0x1D;
                Hu3DModelAttrReset(model->mdlId[idx + i], HU3D_ATTR_DISPOFF);
                pos2d.x = 0.0f;
                pos2d.y = 0.0f;
                pos2d.z = 1000.0f;
                pos2d.x = x + StoryTotalRecordSprPosTbl[4].x;
                pos2d.y = y + StoryTotalRecordSprPosTbl[4].y;
                pos3d = pos2d;
                Hu3D2Dto3D(&pos3d, camera->camera[MDCAMERA_MAIN].bit, &pos3d);
                Hu3DModelPosSetV(model->mdlId[idx + i], &pos3d);
                Hu3DModelScaleSet(model->mdlId[idx + i], 0.75f, 0.75f, 0.75f);
            }
        }
    }
}

int MdOptionDecaGetBestRecord(int no)
{
    int i;
    int best = 0;
    for (i = 0; i <= no; i++) {
        if (i > 0 && GwCommon.decaScore[i - 1].finalScore > GwCommon.decaScore[i].finalScore) {
            best = i;
        }
    }
    return best;
}

static HuVecF DecaRecordSprPosTbl[7] = {
    { 0.0f, 0.0f, 0.0f },
    { -32.0f, 44.0f, 0.0f },
    { -16.0f, 44.0f, 0.0f },
    { 0.0f, 44.0f, 0.0f },
    { 16.0f, 44.0f, 0.0f },
    { 32.0f, 44.0f, 0.0f },
    { 0.0f, -44.0f, 0.0f },
};

void MdOptionDecaRecordUpdate(MDSPRITE_WORK *sprite)
{
    MDMESS_WORK *mess;
    HUSPRGRPID grp0;
    HuVecF *pos0;
    HuVecF *scale0;
    HUSPRGRPID recGrp;
    HuVecF *recPos;
    GWDECASCORE *score;
    int i;
    int j;
    u32 digit;
    float x;
    float y;
    mess = sysP->mess;
    grp0 = sprite->grpId[MDOPT_GRP_BASE];
    pos0 = MdSpriteGrpPosGet(sysP->sprite, MDOPT_GRP_BASE);
    scale0 = MdSpriteGrpScaleGet(sysP->sprite, MDOPT_GRP_BASE);
    if (sysP->list != 5) {
        MdOptionModeSpriteKill(sprite);
        MdOptionSprGrpCreate(sprite, MDOPT_GRP_DECA);
        HuSprAttrReset(grp0, 0, HUSPR_ATTR_DISPOFF);
        HuSprAttrReset(grp0, 1, HUSPR_ATTR_DISPOFF);
        MdMessWinAttrReset(mess, 2, 8);
        MdMessWinAttrSet(mess, 2, 0x800);
        MdMessWinMesSet(mess, 2, MDOPT_MES(0x2C));
        sysP->list = 5;
    }
    recGrp = sprite->grpId[MDOPT_GRP_DECA];
    recPos = MdSpriteGrpPosGet(sysP->sprite, MDOPT_GRP_DECA);
    *recPos = *pos0;
    *MdSpriteGrpScaleGet(sysP->sprite, MDOPT_GRP_DECA) = *scale0;
    for (i = 0; i < 10; i++) {
        score = &GwCommon.decaScore[i];
        x = (float)((i % 5) * 0x60 - 0xC2);
        y = (float)((i / 5) * 0x7E - 0x2A);
        HuSprAttrReset(recGrp, i * 7 + 1, HUSPR_ATTR_DISPOFF);
        HuSprPosSet(recGrp, i * 7 + 1, x + DecaRecordSprPosTbl[0].x, y + DecaRecordSprPosTbl[0].y);
        if (score->charNo != -1) {
            HuSprBankSet(recGrp, i * 7 + 1, score->charNo);
        }
        HuSprAttrReset(recGrp, i * 7, HUSPR_ATTR_DISPOFF);
        HuSprPosSet(recGrp, i * 7, x + DecaRecordSprPosTbl[6].x, y + DecaRecordSprPosTbl[6].y);
        HuSprBankSet(recGrp, i * 7, MdOptionDecaGetBestRecord(i));
        for (j = 0; j < 5; j++) {
            switch (j) {
                case 0:
                    digit = score->finalScore / 10000;
                    break;
                case 1:
                    digit = (score->finalScore % 10000) / 1000;
                    break;
                case 2:
                    digit = (score->finalScore % 1000) / 100;
                    break;
                case 3:
                    digit = (score->finalScore % 100) / 10;
                    break;
                default:
                    digit = score->finalScore % 10;
                    break;
            }
            HuSprAttrReset(recGrp, i * 7 + j + 2, HUSPR_ATTR_DISPOFF);
            HuSprPosSet(recGrp, i * 7 + j + 2, x + DecaRecordSprPosTbl[j + 1].x, y + DecaRecordSprPosTbl[j + 1].y);
            HuSprBankSet(recGrp, i * 7 + j + 2, digit);
        }
    }
    MdMessWinScaleSet(mess, 2, scale0->x, scale0->y);
    MdMessWinPosSet(mess, 2, -155.0f * scale0->x + pos0->x, -147.0f * scale0->y + pos0->y);
}

static HuVecF DecaTotalRecordSprPosTbl[12] = {
    { 0.0f, 0.0f, 0.0f },
    { -24.0f, 54.0f, 0.0f },
    { -8.0f, 54.0f, 0.0f },
    { 8.0f, 54.0f, 0.0f },
    { 24.0f, 54.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 60.0f, 0.0f, 0.0f },
    { 112.0f, 0.0f, 0.0f },
    { 132.0f, 0.0f, 0.0f },
    { 152.0f, 0.0f, 0.0f },
    { 172.0f, 0.0f, 0.0f },
    { 192.0f, 0.0f, 0.0f },
};

void MdOptionDecaTotalRecordUpdate(MDSPRITE_WORK *sprite, int charNo)
{
    MDMESS_WORK *mess;
    HUSPRGRPID grp0;
    HuVecF *pos0;
    HuVecF *scale0;
    HUSPRGRPID recGrp;
    GWDECASCORE *score;
    s16 i;
    s16 best;
    int j;
    int calc;
    u32 digit;
    s16 digit2;
    float x;
    float y;
    mess = sysP->mess;
    grp0 = sprite->grpId[MDOPT_GRP_BASE];
    pos0 = MdSpriteGrpPosGet(sysP->sprite, MDOPT_GRP_BASE);
    scale0 = MdSpriteGrpScaleGet(sysP->sprite, MDOPT_GRP_BASE);
    if (sysP->list != 6) {
        MdOptionModeSpriteKill(sprite);
        MdOptionSprGrpCreate(sprite, MDOPT_GRP_DECA_TOTAL);
        HuSprAttrReset(grp0, 0, HUSPR_ATTR_DISPOFF);
        HuSprAttrReset(grp0, 1, HUSPR_ATTR_DISPOFF);
        MdMessWinScaleSet(mess, 2, 1.0f, 1.0f);
        MdMessWinAttrReset(mess, 2, 8);
        MdMessWinAttrSet(mess, 2, 0x800);
        MdMessWinMesSet(mess, 2, MDOPT_MES(0x2D));
        sysP->list = 6;
        recGrp = sprite->grpId[MDOPT_GRP_DECA_TOTAL];
        best = MdOptionDecaGetBestRecord(charNo);
        score = &GwCommon.decaScore[charNo];
        HuSprAttrReset(recGrp, 0, HUSPR_ATTR_DISPOFF);
        HuSprPosSet(recGrp, 0, -94.0f + DecaTotalRecordSprPosTbl[5].x, -88.0f + DecaTotalRecordSprPosTbl[5].y);
        HuSprBankSet(recGrp, 0, best);
        HuSprAttrReset(recGrp, 1, HUSPR_ATTR_DISPOFF);
        HuSprPosSet(recGrp, 1, -94.0f + DecaTotalRecordSprPosTbl[6].x, -88.0f + DecaTotalRecordSprPosTbl[6].y);
        for (j = 0; j < 5; j++) {
            switch (j) {
                case 0:
                    digit = score->finalScore / 10000;
                    break;
                case 1:
                    digit = (score->finalScore % 10000) / 1000;
                    break;
                case 2:
                    digit = (score->finalScore % 1000) / 100;
                    break;
                case 3:
                    digit = (score->finalScore % 100) / 10;
                    break;
                default:
                    digit = score->finalScore % 10;
                    break;
            }
            HuSprAttrReset(recGrp, j + 2, HUSPR_ATTR_DISPOFF);
            HuSprPosSet(recGrp, j + 2, -94.0f + DecaTotalRecordSprPosTbl[j + 7].x, -88.0f + DecaTotalRecordSprPosTbl[j + 7].y);
            HuSprBankSet(recGrp, j + 2, digit);
        }
        for (i = 0; i < 10; i++) {
            x = (float)((i % 5) * 0x60 - 0xC2);
            y = (float)((i / 5) * 0x6E - 0x1A);
            HuSprAttrReset(recGrp, i * 5 + 7, HUSPR_ATTR_DISPOFF);
            HuSprPosSet(recGrp, i * 5 + 7, x + DecaTotalRecordSprPosTbl[0].x, y + DecaTotalRecordSprPosTbl[0].y);
            HuSprScaleSet(recGrp, i * 5 + 7, 0.84f, 0.84f);
            calc = MgDecaScoreCalc(i, score->mgScore[i]);
            for (j = 0; j < 4; j++) {
                switch (j) {
                    case 0:
                        digit2 = (calc % 10000) / 1000;
                        break;
                    case 1:
                        digit2 = (calc % 1000) / 100;
                        break;
                    case 2:
                        digit2 = (calc % 100) / 10;
                        break;
                    default:
                        digit2 = calc % 10;
                        break;
                }
                HuSprAttrReset(recGrp, i * 5 + j + 8, HUSPR_ATTR_DISPOFF);
                HuSprPosSet(recGrp, i * 5 + j + 8, x + DecaTotalRecordSprPosTbl[j + 1].x, y + DecaTotalRecordSprPosTbl[j + 1].y);
                HuSprBankSet(recGrp, i * 5 + j + 8, digit2);
            }
        }
    }
}

static HuVecF SdRecordSprPosTbl[5] = {
    { 0.0f, 0.0f, 0.0f },
    { -40.0f, 66.0f, 0.0f },
    { 0.0f, 66.0f, 0.0f },
    { 20.0f, 66.0f, 0.0f },
    { 40.0f, 66.0f, 0.0f },
};

void MdOptionSdRecordUpdate(MDSPRITE_WORK *sprite)
{
    MDMESS_WORK *mess;
    HUSPRGRPID grp0;
    HuVecF *pos0;
    HuVecF *scale0;
    HUSPRGRPID recGrp;
    HuVecF *recPos;
    s16 i;
    s16 k;
    int j;
    u16 val;
    u32 digit;
    float x;
    mess = sysP->mess;
    grp0 = sprite->grpId[MDOPT_GRP_BASE];
    pos0 = MdSpriteGrpPosGet(sysP->sprite, MDOPT_GRP_BASE);
    scale0 = MdSpriteGrpScaleGet(sysP->sprite, MDOPT_GRP_BASE);
    if (sysP->list != 7) {
        MdOptionModeSpriteKill(sprite);
        MdOptionSprGrpCreate(sprite, MDOPT_GRP_SD);
        HuSprAttrReset(grp0, 0, HUSPR_ATTR_DISPOFF);
        HuSprAttrReset(grp0, 1, HUSPR_ATTR_DISPOFF);
        MdMessWinAttrReset(mess, 2, 8);
        MdMessWinAttrSet(mess, 2, 0x800);
        MdMessWinAttrSet(mess, 3, 0x800);
        MdMessWinMesSet(mess, 2, MDOPT_MES(0x30));
        MdMessWinAttrReset(mess, 3, 8);
        val = GwCommon.sdVsPlayCnt;
        if (val > 999) {
            val = 999;
        }
        MdOptionTextListUpdate(11, MDOPT_MES(0x3F), val);
        MdMessWinInsertMesSet(mess, 3, MDOPT_MES(0x31), MDOPT_MES(0x3F));
        sysP->list = 7;
    }
    recGrp = sprite->grpId[MDOPT_GRP_SD];
    recPos = MdSpriteGrpPosGet(sysP->sprite, MDOPT_GRP_SD);
    *recPos = *pos0;
    *MdSpriteGrpScaleGet(sysP->sprite, MDOPT_GRP_SD) = *scale0;
    for (i = 0; i < 2; i++) {
        HuSprAttrReset(recGrp, i + 0x1B, HUSPR_ATTR_DISPOFF);
        HuSprPosSet(recGrp, i + 0x1B, -180.0f, (float)(i * 0x32 + 0x2C));
        HuSprBankSet(recGrp, i + 0x1B, i);
    }
    for (k = 0; k < 3; k++) {
        x = (float)(k * 0x78 - 0x50);
        HuSprAttrReset(recGrp, k, HUSPR_ATTR_DISPOFF);
        HuSprPosSet(recGrp, k, x + SdRecordSprPosTbl[0].x, -20.0f + SdRecordSprPosTbl[0].y);
        for (j = 0; j < 4; j++) {
            val = GwCommon.sdVsWinCnt[k][0];
            if (val > 999) {
                val = 999;
            }
            switch (j) {
                case 0:
                    digit = 10;
                    break;
                case 1:
                    digit = val / 100;
                    break;
                case 2:
                    digit = (val % 100) / 10;
                    break;
                default:
                    digit = val % 10;
                    break;
            }
            HuSprBankSet(recGrp, k * 4 + j + 3, digit);
            HuSprAttrReset(recGrp, k * 4 + j + 3, HUSPR_ATTR_DISPOFF);
            HuSprPosSet(recGrp, k * 4 + j + 3, x + SdRecordSprPosTbl[j + 1].x, -20.0f + SdRecordSprPosTbl[j + 1].y);
            val = GwCommon.sdVsWinCnt[k][1];
            if (val > 999) {
                val = 999;
            }
            switch (j) {
                case 0:
                    digit = 10;
                    break;
                case 1:
                    digit = val / 100;
                    break;
                case 2:
                    digit = (val % 100) / 10;
                    break;
                default:
                    digit = val % 10;
                    break;
            }
            HuSprBankSet(recGrp, k * 4 + j + 0xF, digit);
            HuSprAttrReset(recGrp, k * 4 + j + 0xF, HUSPR_ATTR_DISPOFF);
            HuSprPosSet(recGrp, k * 4 + j + 0xF, x + SdRecordSprPosTbl[j + 1].x, 50.0f + (-20.0f + SdRecordSprPosTbl[j + 1].y));
        }
    }
    MdMessWinScaleSet(mess, 2, scale0->x, scale0->y);
    MdMessWinPosSet(mess, 2, -155.0f * scale0->x + pos0->x, -147.0f * scale0->y + pos0->y);
    MdMessWinScaleSet(mess, 3, scale0->x, scale0->y);
    MdMessWinPosSet(mess, 3, -288.0f * scale0->x + pos0->x, -112.0f * scale0->y + pos0->y);
}

static MDOPT_MG_RECORD MgRecordTbl[13] = {
    { 0x00430005, 0x00430005, 2, 0 },
    { 0x00430009, 0x00430009, 1, 1 },
    { 0x0043000A, 0x0043000A, 7, 2 },
    { 0x0043000B, 0x0043000B, 7, 3 },
    { 0x0043000C, 0x0043000C, 7, 4 },
    { 0x0043000D, 0x0043000D, 13, 5 },
    { 0x0043001C, 0x0043001C, 1, 6 },
    { 0x00430049, 0x00430049, 4, 9 },
    { 0x0043002E, 0x0043002E, 2, 7 },
    { 0x00430033, 0x00430033, 5, 8 },
    { 0x00430033, 0x001F0052, 5, 10 },
    { 0x00430033, 0x001F0053, 5, 11 },
    { -1, -1, 0, 0 },
};

s32 lbl_1_data_25C0[24] = {
    0x00430000, 0x00430001, 0x00430002, 0x00430003, 0x0043003E, 0x0043003F, 0x00430040, 0x0043004B, 0x00430004, 0x00430005, 0x00430006, 0x00430007, 0x00430008, 0x00430009, 0x0043004E, 0x0043000A, 0x0043000B, 0x0043000C, 0x0043000D, 0x0043000E, 0x00430017, 0x0043000F, 0x00430041, -1
};

s32 lbl_1_data_2620[13] = {
    0x00430010, 0x00430011, 0x00430012, 0x00430013, 0x00430015, 0x00430016, 0x00430018, 0x00430042, 0x00430043, 0x00430044, 0x00430045, 0x0043004C, -1
};

s32 lbl_1_data_2654[13] = {
    0x00430019, 0x0043001A, 0x0043001B, 0x00430014, 0x0043001C, 0x0043001D, 0x0043001E, 0x00430049, 0x0043001F, 0x00430020, 0x00430021, 0x0043004A, -1
};

s32 lbl_1_data_2688[7] = {
    0x00430022, 0x00430023, 0x00430024, 0x00430025, 0x00430026, 0x00430027, -1
};

s32 lbl_1_data_26A4[16] = {
    0x00430028, 0x00430029, 0x0043002A, 0x0043002B, 0x0043002C, 0x0043002D, 0x0043002E, 0x0043002F, 0x00430030, 0x00430031, 0x00430032, 0x00430033, 0x00430034, 0x00430035, 0x00430036, -1
};

s32 lbl_1_data_26E4[19] = {
    0x00430000, 0x00430001, 0x00430002, 0x0043003E, 0x0043003F, 0x00430040, 0x0043004B, 0x00430004, 0x00430005, 0x00430006, 0x00430007, 0x00430008, 0x00430009, 0x0043004E, 0x0043000C, 0x00430017, 0x0043000F, 0x00430041, -1
};

s32 lbl_1_data_2730[10] = {
    0x00430010, 0x00430011, 0x00430013, 0x00430015, 0x00430016, 0x00430043, 0x00430044, 0x00430045, 0x0043004C, -1
};

s32 lbl_1_data_2758[10] = {
    0x00430019, 0x0043001B, 0x00430014, 0x0043001C, 0x00430049, 0x0043001F, 0x00430020, 0x00430021, 0x0043004A, -1
};

s32 lbl_1_data_2780[5] = {
    0x00430022, 0x00430025, 0x00430026, 0x00430027, -1
};

s32 lbl_1_data_2794[11] = {
    0x00430028, 0x00430029, 0x0043002B, 0x0043002D, 0x0043002E, 0x0043002F, 0x00430031, 0x00430032, 0x00430034, 0x00430035, -1
};

s32 lbl_1_data_27C0[11] = {
    0x00430001, 0x00430002, 0x0043003E, 0x0043003F, 0x00430040, 0x0043004B, 0x0043000A, 0x0043000B, 0x00430017, 0x0043000F, -1
};

s32 lbl_1_data_27EC[7] = {
    0x00430010, 0x00430011, 0x00430012, 0x00430013, 0x00430043, 0x00430044, -1
};

s32 lbl_1_data_2808[7] = {
    0x00430019, 0x0043001A, 0x00430014, 0x0043001E, 0x00430021, 0x0043004A, -1
};

s32 lbl_1_data_2824[4] = {
    0x00430023, 0x00430024, 0x00430025, -1
};

s32 lbl_1_data_2834[6] = {
    0x00430028, 0x0043002A, 0x00430030, 0x00430032, 0x00430034, -1
};

s32 lbl_1_data_284C[11] = {
    0x00430000, 0x00430003, 0x00430004, 0x00430005, 0x00430006, 0x00430008, 0x00430009, 0x0043000E, 0x00430017, 0x0043000F, -1
};

s32 lbl_1_data_2878[7] = {
    0x00430015, 0x00430016, 0x00430018, 0x00430042, 0x00430044, 0x0043004C, -1
};

s32 lbl_1_data_2894[7] = {
    0x00430019, 0x0043001B, 0x0043001C, 0x0043001D, 0x0043001F, 0x0043004A, -1
};

s32 lbl_1_data_28B0[4] = {
    0x00430022, 0x00430026, 0x00430027, -1
};

s32 lbl_1_data_28C0[6] = {
    0x00430029, 0x0043002B, 0x0043002F, 0x00430033, 0x00430036, -1
};

s32 lbl_1_data_28D8[11] = {
    0x00430000, 0x00430001, 0x00430007, 0x0043004E, 0x0043000C, 0x0043000D, 0x0043000E, 0x00430017, 0x0043000F, 0x00430041, -1
};

s32 lbl_1_data_2904[7] = {
    0x00430011, 0x00430015, 0x00430016, 0x00430042, 0x00430044, 0x00430045, -1
};

s32 lbl_1_data_2920[7] = {
    0x0043001A, 0x0043001C, 0x00430049, 0x00430020, 0x00430021, 0x0043004A, -1
};

s32 lbl_1_data_293C[4] = {
    0x00430023, 0x00430024, 0x00430027, -1
};

s32 lbl_1_data_294C[6] = {
    0x0043002C, 0x0043002D, 0x0043002E, 0x00430031, 0x00430035, -1
};

static s32 *ListTbl[25] = {
    lbl_1_data_25C0, lbl_1_data_2620, lbl_1_data_2654, lbl_1_data_2688, lbl_1_data_26A4,
    lbl_1_data_26E4, lbl_1_data_2730, lbl_1_data_2758, lbl_1_data_2780, lbl_1_data_2794,
    lbl_1_data_27C0, lbl_1_data_27EC, lbl_1_data_2808, lbl_1_data_2824, lbl_1_data_2834,
    lbl_1_data_284C, lbl_1_data_2878, lbl_1_data_2894, lbl_1_data_28B0, lbl_1_data_28C0,
    lbl_1_data_28D8, lbl_1_data_2904, lbl_1_data_2920, lbl_1_data_293C, lbl_1_data_294C,
};

static s32 ListNumTbl[25] = { 0 };

static s32 ListMgNumTbl[25] = { 0 };

void MdOptionMgListInit(void)
{
    int i;
    int j;
    int k;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            ListMgNumTbl[i * 5 + j] = 0;
            for (k = 0; ListTbl[i * 5 + j][k] != -1; k++) {
                if (GWMgUnlockGet(ListTbl[i * 5 + j][k] - 0x430000 + 501)) {
                    ListMgNumTbl[i * 5 + j]++;
                }
            }
            ListNumTbl[i * 5 + j] = k;
        }
    }
}

int MdOptionTextListNameGet(int cat, int sub, int no)
{
    if (no >= ListNumTbl[cat * 5 + sub]) {
        return MDOPT_MES(0x43);
    }
    if (GWMgUnlockGet(ListTbl[cat * 5 + sub][no] - 0x430000 + 501)) {
        return ListTbl[cat * 5 + sub][no];
    }
    return MDOPT_MES(0x4F);
}
