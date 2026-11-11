#ifdef __MWERKS__
#define _MATH_H
#endif
#include "REL/mdoptionDll/mdoption.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/mgdata.h"

#ifdef __MWERKS__
extern double sin(double x);
#else
#include <math.h>
#endif

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
extern void MdMath3Dto2D(HuVecF *src, s16 cameraBit, HuVecF *dst);
extern void MdMathMtxRotCalc(HuVecF *dir, HuVecF *up, Mtx out, float angle);
extern void MdMathMtxRotGet(Mtx mtx, HuVecF *rot);
extern void fn_1_A930(int no);
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);

void MdOptionSprGrpCreate(MDSPRITE_WORK *sprite, int grpNo);
void MdOptionSprGrpKill(MDSPRITE_WORK *sprite, int grpNo);
void MdOptionModeSpriteKill(MDSPRITE_WORK *sprite);
void MdOptionTextListUpdate(u32 type, int msg, u32 value);
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
void MdOptionListCursorPosSet(int cat, int sub, int ofs);
void MdOptionListCursorClamp(int cat, int sub, int *sel);
void MdOptionMgListWinUpdate(MDSPRITE_WORK *sprite, int cat, int sub, int ofs);
void MdOptionRecordCursorPosSet(int no);
extern void fn_1_1A7C0(void);
extern void fn_1_1AD88(void);
void MdOptionMgListInit(void);
int MdOptionTextListNameGet(int cat, int sub, int no);
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


void MdOptionTextListUpdate(u32 type, int msg, u32 value)
{
    u32 typeNo = type;
    int msgNo = msg;
    char *buf;
    int i;
    buf = HuWinMesPtrGet(msgNo);
    switch (typeNo) {
        case 0:
            for (i = 0; i < 7; i++) {
                if (i < 6) {
                    buf[i] = ' ';
                } else {
                    buf[i] = (value % 10) + '0';
                }
            }
            return;
        case 1:
            for (i = 0; i < 7; i++) {
                switch (i) {
                    default:
                        buf[i] = ' ';
                        break;
                    case 5:
                        buf[i] = ((value % 100) / 10) + '0';
                        break;
                    case 6:
                        buf[i] = (value % 10) + '0';
                        break;
                }
            }
            return;
        case 2:
            for (i = 0; i < 7; i++) {
                switch (i) {
                    default:
                        buf[i] = ' ';
                        break;
                    case 4:
                        buf[i] = ((value % 1000) / 100) + '0';
                        break;
                    case 5:
                        buf[i] = ((value % 100) / 10) + '0';
                        break;
                    case 6:
                        buf[i] = (value % 10) + '0';
                        break;
                }
            }
            return;
        case 3:
            for (i = 0; i < 7; i++) {
                switch (i) {
                    default:
                        buf[i] = ' ';
                        break;
                    case 3:
                        buf[i] = ((value % 10000) / 1000) + '0';
                        break;
                    case 4:
                        buf[i] = ((value % 1000) / 100) + '0';
                        break;
                    case 5:
                        buf[i] = ((value % 100) / 10) + '0';
                        break;
                    case 6:
                        buf[i] = (value % 10) + '0';
                        break;
                }
            }
            return;
        case 4:
            for (i = 0; i < 7; i++) {
                switch (i) {
                    default:
                        buf[i] = ' ';
                        break;
                    case 2:
                        buf[i] = ((value % 100000) / 10000) + '0';
                        break;
                    case 3:
                        buf[i] = ((value % 10000) / 1000) + '0';
                        break;
                    case 4:
                        buf[i] = ((value % 1000) / 100) + '0';
                        break;
                    case 5:
                        buf[i] = ((value % 100) / 10) + '0';
                        break;
                    case 6:
                        buf[i] = (value % 10) + '0';
                        break;
                }
            }
            return;
        case 5:
            for (i = 0; i < 7; i++) {
                switch (i) {
                    case 0:
                        buf[i] = ' ';
                        break;
                    case 1:
                        buf[i] = ((value % 1000000) / 100000) + '0';
                        break;
                    case 2:
                        buf[i] = ((value % 100000) / 10000) + '0';
                        break;
                    case 3:
                        buf[i] = ((value % 10000) / 1000) + '0';
                        break;
                    case 4:
                        buf[i] = ((value % 1000) / 100) + '0';
                        break;
                    case 5:
                        buf[i] = ((value % 100) / 10) + '0';
                        break;
                    default:
                        buf[i] = (value % 10) + '0';
                        break;
                }
            }
            return;
        case 6:
            for (i = 0; i < 7; i++) {
                switch (i) {
                    case 0:
                        buf[i] = ((value % 10000000) / 1000000) + '0';
                        break;
                    case 1:
                        buf[i] = ((value % 1000000) / 100000) + '0';
                        break;
                    case 2:
                        buf[i] = ((value % 100000) / 10000) + '0';
                        break;
                    case 3:
                        buf[i] = ((value % 10000) / 1000) + '0';
                        break;
                    case 4:
                        buf[i] = ((value % 1000) / 100) + '0';
                        break;
                    case 5:
                        buf[i] = ((value % 100) / 10) + '0';
                        break;
                    default:
                        buf[i] = (value % 10) + '0';
                        break;
                }
            }
            return;
        case 7: {
            float frame = 1.6666666f * (float)((value % 3600) % 60);
            for (i = 0; i < 7; i++) {
                switch (i) {
                    case 0:
                        buf[i] = ((value / 3600) % 10) + '0';
                        break;
                    case 1:
                        buf[i] = 0x5C;
                        break;
                    case 2:
                        buf[i] = ((((value % 3600) / 60) % 100) / 10) + '0';
                        break;
                    case 3:
                        buf[i] = (((value % 3600) / 60) % 10) + '0';
                        break;
                    case 4:
                        buf[i] = 0x5B;
                        break;
                    case 5:
                        buf[i] = (((int)frame % 100) / 10) + '0';
                        break;
                    default:
                        buf[i] = ((int)frame % 10) + '0';
                        break;
                }
            }
            return;
        }
        case 8:
            for (i = 0; i < 7; i++) {
                switch (i) {
                    default:
                        buf[i] = ' ';
                        break;
                    case 2:
                        buf[i] = ((value % 10000) / 1000) + '0';
                        break;
                    case 3:
                        buf[i] = ((value % 1000) / 100) + '0';
                        break;
                    case 4:
                        buf[i] = 0x85;
                        break;
                    case 5:
                        buf[i] = ((value % 100) / 10) + '0';
                        break;
                    case 6:
                        buf[i] = (value % 10) + '0';
                        break;
                }
            }
            return;
        case 9:
            for (i = 0; i < 7; i++) {
                if (i == 0) {
                    buf[i] = (value % 10) + '0';
                } else {
                    buf[i] = 0;
                }
            }
            return;
        case 10:
            for (i = 0; i < 7; i++) {
                switch (i) {
                    case 0:
                        buf[i] = ((value % 10000) / 1000) + '0';
                        break;
                    case 1:
                        buf[i] = ((value % 1000) / 100) + '0';
                        break;
                    case 2:
                        buf[i] = ((value % 100) / 10) + '0';
                        break;
                    case 3:
                        buf[i] = (value % 10) + '0';
                        break;
                    default:
                        buf[i] = 0;
                        break;
                }
            }
            return;
        case 11:
            for (i = 0; i < 7; i++) {
                switch (i) {
                    case 0:
                        buf[i] = ((value % 1000) / 100) + '0';
                        break;
                    case 1:
                        buf[i] = ((value % 100) / 10) + '0';
                        break;
                    case 2:
                        buf[i] = (value % 10) + '0';
                        break;
                    default:
                        buf[i] = 0;
                        break;
                }
            }
            return;
        case 12:
            for (i = 0; i < 7; i++) {
                buf[i] = ' ';
            }
            return;
        case 13:
            for (i = 0; i < 7; i++) {
                switch (i) {
                    case 0:
                        buf[i] = ' ';
                        break;
                    case 1:
                        buf[i] = ((value % 100000) / 10000) + '0';
                        break;
                    case 2:
                        buf[i] = ((value % 10000) / 1000) + '0';
                        break;
                    case 3:
                        buf[i] = ((value % 1000) / 100) + '0';
                        break;
                    case 4:
                        buf[i] = 0x85;
                        break;
                    case 5:
                        buf[i] = ((value % 100) / 10) + '0';
                        break;
                    default:
                        buf[i] = (value % 10) + '0';
                        break;
                }
            }
            return;
        default:
            return;
    }
}

void MdOptionMgCopyName(int msg, char *name)
{
    int msgNo = msg;
    char *buf;
    int i;
    buf = HuWinMesPtrGet(msgNo);
    for (i = 0; i < 7; i++) {
        switch (i) {
            case 0:
                buf[i] = name[0];
                break;
            case 1:
                buf[i] = name[1];
                break;
            case 2:
                buf[i] = name[2];
                break;
            case 3:
                buf[i] = name[3];
                break;
            default:
                buf[i] = 0;
                break;
        }
    }
}

void MdOptionMgListUpdate(MDSPRITE_WORK *sprite)
{
    MDMESS_WORK *mess;
    HUSPRGRPID grp0;
    HuVecF *pos0;
    HuVecF *scale0;
    s16 i;
    int j;
    int msg;
    float y;
    int list[8];
    mess = sysP->mess;
    grp0 = sprite->grpId[MDOPT_GRP_BASE];
    pos0 = MdSpriteGrpPosGet(sysP->sprite, MDOPT_GRP_BASE);
    scale0 = MdSpriteGrpScaleGet(sysP->sprite, MDOPT_GRP_BASE);
    if (sysP->list != 8) {
        MdOptionModeSpriteKill(sprite);
        sysP->list = 8;
    }
    HuSprAttrReset(grp0, 0, HUSPR_ATTR_DISPOFF);
    HuSprAttrReset(grp0, 1, HUSPR_ATTR_DISPOFF);
    MdMessWinScaleSet(mess, 2, scale0->x, scale0->y);
    MdMessWinPosSet(mess, 2, -155.0f * scale0->x + pos0->x, -147.0f * scale0->y + pos0->y);
    for (i = 0; i < 2; i++) {
        y = (float)(i * 0xDC - 0x5A);
        HuSprAttrReset(grp0, i + 5, HUSPR_ATTR_DISPOFF);
        HuSprBankSet(grp0, i + 5, i);
        HuSprPosSet(grp0, i + 5, 0.0f, y);
        HuSprAttrReset(grp0, i + 7, HUSPR_ATTR_DISPOFF);
        HuSprTPLvlSet(grp0, i + 7, 0.0f);
        HuSprAttrSet(grp0, i + 7, HUSPR_ATTR_ADDCOL);
        HuSprBankSet(grp0, i + 7, i);
        HuSprPosSet(grp0, i + 7, 0.0f, y);
    }
    MdMessWinAttrReset(mess, 2, 8);
    MdMessWinAttrReset(mess, 3, 8);
    if (sysP->menu == 0) {
        HuSprAttrReset(grp0, 3, HUSPR_ATTR_DISPOFF);
        HuSprAttrReset(grp0, 4, HUSPR_ATTR_DISPOFF);
        MdOptionMgListInit();
        MdOptionListCursorPosSet(0, 0, 0);
        MdMessWinAttrSet(mess, 2, 0x800);
        MdMessWinMesSet(mess, 2, MDOPT_MES(0x39));
        MdOptionTextListUpdate(12, MDOPT_MES(0x43), 0);
        for (j = 0; j < 7; j++) {
            MdMessWinClose(mess, j + 4);
            msg = MdOptionTextListNameGet(0, 0, j);
            MdMessHelpWinCreate(mess, j + 4, msg, 150.0f, 116.0f, 0x104, 0x20, 0.0f, 0);
            MdMessWinAttrSet(mess, j + 4, 0xC00);
            MdMessWinMesSet(mess, j + 4, msg);
            MdMessWinSpeedSet(mess, j + 4, 0);
            MdMessWinScaleSet(mess, j + 4, scale0->x, scale0->y);
            MdMessWinPosSet(mess, j + 4, -138.0f * scale0->x + pos0->x, pos0->y + (-80.0f + 26.0f * (float)j) * scale0->y);
        }
        list[0] = MDOPT_MES(0x3F);
        list[1] = MDOPT_MES(0x40);
        list[2] = MDOPT_MES(0x41);
        list[3] = MDOPT_MES(0x42);
        list[4] = -1;
        list[5] = -1;
        list[6] = -1;
        list[7] = -1;
        MdOptionTextListUpdate(9, list[0], ListMgNumTbl[0] / 10);
        MdOptionTextListUpdate(9, list[1], ListMgNumTbl[0] % 10);
        MdOptionTextListUpdate(9, list[2], ListNumTbl[0] / 10);
        MdOptionTextListUpdate(9, list[3], ListNumTbl[0] % 10);
        MdMessWinClose(mess, 0xB);
        MdMessHelpWinCreate(mess, 0xB, 0x20000B, 402.0f, 300.0f, 0x73, 0x1C, 0.0f, 0);
        MdMessWinInsertMultiMesSet(mess, 0xB, MDOPT_MES(0x4D), list);
        MdMessWinSpeedSet(mess, 0xB, 0);
        MdMessWinScaleSet(mess, 0xB, scale0->x, scale0->y);
        MdMessWinPosSet(mess, 0xB, 114.0f * scale0->x + pos0->x, 108.0f * scale0->y + pos0->y);
        return;
    }
    if (sysP->menu == 2 && sysP->item == 1) {
        fn_1_1A7C0();
        return;
    }
    if (sysP->menu == 3 && sysP->item == 0) {
        list[0] = MDOPT_MES(0x3F);
        list[1] = MDOPT_MES(0x40);
        list[2] = MDOPT_MES(0x41);
        list[3] = MDOPT_MES(0x42);
        list[4] = MDOPT_MES(0x43);
        list[5] = MDOPT_MES(0x44);
        list[6] = MDOPT_MES(0x45);
        list[7] = -1;
        MdOptionRecordCursorPosSet(0);
        HuSprAttrSet(grp0, 3, HUSPR_ATTR_DISPOFF);
        HuSprAttrSet(grp0, 4, HUSPR_ATTR_DISPOFF);
        MdMessWinAttrSet(mess, 2, 0x800);
        MdMessWinMesSet(mess, 2, MDOPT_MES(0x1C));
        for (j = 0; j < 7; j++) {
            MdMessWinClose(mess, j + 0xB);
            if (GWMgUnlockGet(MgRecordTbl[j].mgMsg - 0x430000 + 501)) {
                MdOptionTextListUpdate(MgRecordTbl[j].type, list[j], GwCommon.mgRecord[MgRecordTbl[j].recNo]);
            } else {
                list[j] = MDOPT_MES(0x51);
            }
            MdMessHelpWinCreate(mess, j + 0xB, MDOPT_MES(0x3E), 350.0f, 116.0f, 0x96, 0x20, 0.0f, 0);
            MdMessWinAttrSet(mess, j + 0xB, 0x1000);
            MdMessWinMesSet(mess, j + 0xB, list[j]);
            MdMessWinScaleSet(mess, j + 0xB, scale0->x, scale0->y);
            MdMessWinPosSet(mess, j + 0xB, 62.0f * scale0->x + pos0->x, pos0->y + (-80.0f + 26.0f * (float)j) * scale0->y);
            MdMessWinSpeedSet(mess, j + 0xB, 0);
        }
        MdMessWinClose(mess, 4);
        MdMessHelpWinCreate(mess, 4, MDOPT_MES(0x3E), 60.0f, 116.0f, 0x104, 0xB4, 0.0f, 0);
        for (j = 0; j < 7; j++) {
            if (GWMgUnlockGet(MgRecordTbl[j].mgMsg - 0x430000 + 501)) {
                list[j] = MgRecordTbl[j].nameMsg;
            } else {
                list[j] = MDOPT_MES(0x4F);
            }
        }
        MdMessWinInsertMultiMesSet(mess, 4, MDOPT_MES(0x3E), list);
        MdMessWinScaleSet(mess, 4, scale0->x, scale0->y);
        MdMessWinPosSet(mess, 4, -228.0f * scale0->x + pos0->x, -80.0f * scale0->y + pos0->y);
        MdMessWinSpeedSet(mess, 4, 0);
    }
}

void MdOptionMgListWinUpdate(MDSPRITE_WORK *sprite, int cat, int sub, int ofs)
{
    HUSPRGRPID grp0;
    MDMESS_WORK *mess;
    HuVecF *pos0;
    HuVecF *scale0;
    int j;
    int msg;
    int list[8];
    grp0 = sprite->grpId[MDOPT_GRP_BASE];
    mess = sysP->mess;
    pos0 = MdSpriteGrpPosGet(sysP->sprite, MDOPT_GRP_BASE);
    scale0 = MdSpriteGrpScaleGet(sysP->sprite, MDOPT_GRP_BASE);
    MdMessWinScaleSet(mess, 2, scale0->x, scale0->y);
    MdMessWinPosSet(mess, 2, -155.0f * scale0->x + pos0->x, -147.0f * scale0->y + pos0->y);
    if (sysP->menu == 0) {
        MdOptionTextListUpdate(12, MDOPT_MES(0x43), 0);
        MdMessWinAttrSet(mess, 2, 0x800);
        MdMessWinMesSet(mess, 2, MDOPT_MES(0x39) + sub);
        for (j = 0; j < 7; j++) {
            if (ofs + j >= ListNumTbl[sub + cat * 5]) {
                msg = MDOPT_MES(0x43);
            } else if (GWMgUnlockGet(ListTbl[sub + cat * 5][ofs + j] - 0x430000 + 501)) {
                msg = ListTbl[sub + cat * 5][ofs + j];
            } else {
                msg = MDOPT_MES(0x4F);
            }
            MdMessWinMesSet(mess, j + 4, msg);
            MdMessWinAttrSet(mess, j + 4, 0x800);
            MdMessWinScaleSet(mess, j + 4, scale0->x, scale0->y);
            MdMessWinPosSet(mess, j + 4, -138.0f * scale0->x + pos0->x, pos0->y + (-80.0f + 26.0f * (float)j) * scale0->y);
        }
        list[0] = MDOPT_MES(0x3F);
        list[1] = MDOPT_MES(0x40);
        list[2] = MDOPT_MES(0x41);
        list[3] = MDOPT_MES(0x42);
        list[4] = -1;
        list[5] = -1;
        list[6] = -1;
        list[7] = -1;
        MdOptionTextListUpdate(9, list[0], ListMgNumTbl[sub + cat * 5] / 10);
        MdOptionTextListUpdate(9, list[1], ListMgNumTbl[sub + cat * 5] % 10);
        MdOptionTextListUpdate(9, list[2], ListNumTbl[sub + cat * 5] / 10);
        MdOptionTextListUpdate(9, list[3], ListNumTbl[sub + cat * 5] % 10);
        MdMessWinInsertMultiMesSet(mess, 0xB, MDOPT_MES(0x4D), list);
        MdMessWinScaleSet(mess, 0xB, scale0->x, scale0->y);
        MdMessWinPosSet(mess, 0xB, 114.0f * scale0->x + pos0->x, 108.0f * scale0->y + pos0->y);
        return;
    }
    if (sysP->menu == 2 && sysP->item == 1) {
        fn_1_1AD88();
        return;
    }
    if (sysP->menu == 3 && sysP->item == 0) {
        list[0] = MDOPT_MES(0x3F);
        list[1] = MDOPT_MES(0x40);
        list[2] = MDOPT_MES(0x41);
        list[3] = MDOPT_MES(0x42);
        list[4] = MDOPT_MES(0x43);
        list[5] = MDOPT_MES(0x44);
        list[6] = MDOPT_MES(0x45);
        list[7] = -1;
        for (j = 0; j < 7; j++) {
            if (GWMgUnlockGet(MgRecordTbl[j + cat].mgMsg - 0x430000 + 501)) {
                MdOptionTextListUpdate(MgRecordTbl[j + cat].type, list[j], GwCommon.mgRecord[MgRecordTbl[j + cat].recNo]);
            } else {
                list[j] = MDOPT_MES(0x51);
            }
            MdMessWinMesSet(mess, j + 0xB, list[j]);
            MdMessWinScaleSet(mess, j + 0xB, scale0->x, scale0->y);
            MdMessWinPosSet(mess, j + 0xB, 62.0f * scale0->x + pos0->x, pos0->y + (-80.0f + 26.0f * (float)j) * scale0->y);
        }
        for (j = 0; j < 7; j++) {
            if (GWMgUnlockGet(MgRecordTbl[j + cat].mgMsg - 0x430000 + 501)) {
                list[j] = MgRecordTbl[j + cat].nameMsg;
            } else {
                list[j] = MDOPT_MES(0x4F);
            }
        }
        MdMessWinInsertMultiMesSet(mess, 4, MDOPT_MES(0x3E), list);
        MdMessWinScaleSet(mess, 4, scale0->x, scale0->y);
        MdMessWinPosSet(mess, 4, -228.0f * scale0->x + pos0->x, -80.0f * scale0->y + pos0->y);
    }
}

void MdSpriteGrpDispSet(MDSPRITE_WORK *sprite, int grpNo, BOOL dispF)
{
    MDSPR_GRP_INFO *grpInfo;
    HUSPRGRPID grpId;
    int grpNum;
    s16 i;
    grpInfo = &SpriteGrpInfo[grpNo];
    grpId = sprite->grpId[grpNo];
    grpNum = grpInfo->end - grpInfo->start + 1;
    for (i = 0; i < grpNum; i++) {
        if (dispF == FALSE) {
            HuSprAttrSet(grpId, i, HUSPR_ATTR_DISPOFF);
        } else if (dispF == TRUE) {
            HuSprAttrReset(grpId, i, HUSPR_ATTR_DISPOFF);
        }
    }
}

HuVecF *MdSpriteGrpPosGet(MDSPRITE_WORK *sprite, int grpNo)
{
    HUSPRGRP *grp = &HuSprGrpData[sprite->grpId[grpNo]];
    return (HuVecF *)&grp->pos;
}

HuVecF *MdSpriteGrpScaleGet(MDSPRITE_WORK *sprite, int grpNo)
{
    HUSPRGRP *grp = &HuSprGrpData[sprite->grpId[grpNo]];
    return (HuVecF *)&grp->scale;
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


static u32 StarMotTbl[2] = { 0x00920024, 0x00920025 };

void MdModelCreate(MDOPT_MODEL_WORK *model, MDCAMERA_WORK *camera)
{
    int i;
    int j;
    int idx;
    float scale;
    HU3DMODEL *modelP;
    int dataTbl[26] = {
        0x00920003, 0x00920004, 0x00920005, 0x00920006, 0x00920018, 0x00920007, 0x00920008, 0x00920009,
        0x0092000A, 0x0092000B, 0x0092000C, 0x0092000D, 0x0092000E, 0x00920011, 0x0092000E, 0x0092000F,
        0x00920010, 0x00920013, 0x00920012, 0x00920016, 0x00920017, 0x00920014, 0x00920015, 0x0092000E,
        0x0092000F, 0x00920010
    };
    float zTbl[4] = { -260.0f, -380.0f, -500.0f, -635.0f };
    int wallTbl[2] = { 0x00920019, 0x0092001A };
    int cursorTbl[4] = { 0x0092001E, 0x0092001D, 0x0092001C, 0x0092001B };
    HuVecF waveInd0 = { 0.0f, 0.0f, 0.06f };
    HuVecF waveInd1 = { 0.0f, 0.0f, 0.06f };
    HuVecF indMtxA;
    HuVecF indMtxB;
    GXColor waveColor = { 0, 0, 0, 0 };
    GXColor waveColorCopy;
    HuVecF wavePos = { 288.0f, 200.0f, 1000.0f };
    HuVecF wavePos3d;
    scale = sysP->modelScale;
    model->ambScale = 1.0f;
    for (i = 0; i < 16; i++) {
        model->ambId[i] = -1;
    }
    model->mdlId[0] = Hu3DModelCreateData(0x920000);
    Hu3DModelAttrReset(model->mdlId[0], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[0], 7);
    Hu3DModelCameraSet(model->mdlId[0], camera->camera[MDCAMERA_MAIN].bit);
    Hu3DModelPosSet(model->mdlId[0], 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(model->mdlId[0], 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(model->mdlId[0], 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrSet(model->mdlId[0], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrSet(model->mdlId[0], HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(model->mdlId[0], 1.0f);
    model->mdlId[1] = Hu3DModelCreateData(0x920001);
    Hu3DModelAttrReset(model->mdlId[1], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(model->mdlId[1], HU3D_MOTATTR_LOOP);
    Hu3DModelLayerSet(model->mdlId[1], 0);
    Hu3DModelCameraSet(model->mdlId[1], camera->camera[MDCAMERA_BG].bit);
    Hu3DModelPosSet(model->mdlId[1], 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(model->mdlId[1], 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(model->mdlId[1], scale, scale, scale);
    model->mdlId[2] = Hu3DModelCreateData(0x920002);
    Hu3DModelAttrReset(model->mdlId[2], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(model->mdlId[2], HU3D_MOTATTR_LOOP);
    Hu3DModelLayerSet(model->mdlId[2], 0);
    Hu3DModelCameraSet(model->mdlId[2], camera->camera[MDCAMERA_BG].bit);
    Hu3DModelPosSet(model->mdlId[2], 0.0f, 50.0f, 0.0f);
    Hu3DModelRotSet(model->mdlId[2], 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(model->mdlId[2], 0.0f, 0.0f, 0.0f);
    for (i = 0; i <= 0x19; i++) {
        model->mdlId[i + 3] = Hu3DModelCreateData(dataTbl[i]);
        Hu3DModelAttrReset(model->mdlId[i + 3], HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(model->mdlId[i + 3], HU3D_MOTATTR_LOOP | HU3D_MOTATTR_SHAPE_LOOP);
        Hu3DModelAttrReset(model->mdlId[i + 3], HU3D_MOTATTR_PAUSE | HU3D_MOTATTR_SHAPE_PAUSE);
        Hu3DModelLayerSet(model->mdlId[i + 3], 3);
        Hu3DModelCameraSet(model->mdlId[i + 3], camera->camera[MDCAMERA_BG].bit);
        Hu3DModelRotSet(model->mdlId[i + 3], 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(model->mdlId[i + 3], 0.0f, 0.0f, 0.0f);
        if (i >= 5) {
            Hu3DModelAttrSet(model->mdlId[i + 3], HU3D_ATTR_DISPOFF);
        } else {
            Hu3DModelPosSet(model->mdlId[i + 3], 0.0f, 0.0f, zTbl[i]);
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 0x23; j++) {
            idx = j + i * 0x23 + 0x39;
            if (j == 0) {
                model->mdlId[idx] = Hu3DModelCreateData(wallTbl[i]);
            } else {
                model->mdlId[idx] = Hu3DModelLink(model->mdlId[idx - j]);
            }
            Hu3DModelAttrSet(model->mdlId[idx], HU3D_ATTR_DISPOFF);
            Hu3DModelLayerSet(model->mdlId[idx], 7);
            Hu3DModelCameraSet(model->mdlId[idx], camera->camera[MDCAMERA_BG].bit);
            Hu3DModelPosSet(model->mdlId[idx], 0.0f, 0.0f, 0.0f);
            Hu3DModelRotSet(model->mdlId[idx], 0.0f, 0.0f, 0.0f);
            Hu3DModelScaleSet(model->mdlId[idx], 1.0f, 1.0f, 1.0f);
            Hu3DModelAttrSet(model->mdlId[idx], HU3D_MOTATTR_LOOP);
            Hu3DModelAttrReset(model->mdlId[idx], HU3D_MOTATTR_PAUSE);
            Hu3DMotionSpeedSet(model->mdlId[idx], 1.0f);
        }
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 7; j++) {
            if (j == 0) {
                model->mdlId[i * 7 + j + 0x1D] = Hu3DModelCreateData(cursorTbl[i]);
            } else {
                model->mdlId[i * 7 + j + 0x1D] = Hu3DModelLink(model->mdlId[i * 7 + 0x1D]);
            }
            Hu3DModelAttrSet(model->mdlId[i * 7 + j + 0x1D], HU3D_ATTR_DISPOFF);
            Hu3DModelLayerSet(model->mdlId[i * 7 + j + 0x1D], 7);
            Hu3DModelCameraSet(model->mdlId[i * 7 + j + 0x1D], camera->camera[MDCAMERA_MAIN].bit);
            Hu3DModelPosSet(model->mdlId[i * 7 + j + 0x1D], 0.0f, 0.0f, 0.0f);
            Hu3DModelRotSet(model->mdlId[i * 7 + j + 0x1D], 0.0f, 0.0f, 0.0f);
            Hu3DModelScaleSet(model->mdlId[i * 7 + j + 0x1D], 1.0f, 1.0f, 1.0f);
            Hu3DModelAttrSet(model->mdlId[i * 7 + j + 0x1D], HU3D_MOTATTR_LOOP);
            Hu3DModelAttrReset(model->mdlId[i * 7 + j + 0x1D], HU3D_MOTATTR_PAUSE);
            Hu3DMotionSpeedSet(model->mdlId[i * 7 + j + 0x1D], 1.0f);
        }
    }
    model->mdlId[131] = Hu3DModelCreateData(0x92001F);
    Hu3DModelAttrReset(model->mdlId[131], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[131], 7);
    Hu3DModelCameraSet(model->mdlId[131], camera->camera[MDCAMERA_BG].bit);
    Hu3DModelPosSet(model->mdlId[131], 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(model->mdlId[131], 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(model->mdlId[131], 1.0f, 1.0f, 1.0f);
    Hu3DModelAttrSet(model->mdlId[131], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(model->mdlId[131], HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(model->mdlId[131], 1.0f);
    model->mdlId[128] = Hu3DModelCreateData(0x920021);
    Hu3DModelAttrSet(model->mdlId[128], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[128], 0);
    Hu3DModelCameraSet(model->mdlId[128], camera->camera[MDCAMERA_BG].bit);
    Hu3DModelPosSet(model->mdlId[128], 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(model->mdlId[128], 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(model->mdlId[128], scale, scale, scale);
    Hu3DModelAttrReset(model->mdlId[128], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(model->mdlId[128], HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(model->mdlId[128], 1.0f);
    model->mdlId[130] = Hu3DModelCreateData(0x920023);
    Hu3DModelAttrSet(model->mdlId[130], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[130], 6);
    Hu3DModelCameraSet(model->mdlId[130], camera->camera[MDCAMERA_BG].bit);
    Hu3DModelPosSet(model->mdlId[130], 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(model->mdlId[130], 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(model->mdlId[130], 1.0f, 1.0f, 1.0f);
    Hu3DModelAttrReset(model->mdlId[130], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(model->mdlId[130], HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(model->mdlId[130], 1.0f);
    model->mdlId[129] = Hu3DModelCreateData(0x920022);
    Hu3DModelAttrSet(model->mdlId[129], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[129], 0);
    Hu3DModelCameraSet(model->mdlId[129], camera->camera[MDCAMERA_BG].bit);
    Hu3DModelPosSet(model->mdlId[129], 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(model->mdlId[129], 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(model->mdlId[129], scale, scale, scale);
    Hu3DModelAttrReset(model->mdlId[129], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(model->mdlId[129], HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(model->mdlId[129], 1.0f);
    model->mdlId[132] = Hu3DHookFuncCreate(FontBGFilter);
    Hu3DModelLayerSet(model->mdlId[132], 7);
    Hu3DModelCameraSet(model->mdlId[132], camera->camera[MDCAMERA_BG].bit);
    Hu3DModelAttrReset(model->mdlId[132], HU3D_ATTR_DISPOFF);
    for (i = 0; i < 2; i++) {
        model->motId[i] = Hu3DJointMotionData(model->mdlId[0], StarMotTbl[i]);
    }
    Hu3DMotionSet(model->mdlId[0], model->motId[0]);
    Hu3DModelAttrReset(model->mdlId[0], HU3D_MOTATTR_PAUSE);
    MdOptionWaveEffectModelCreate();
    model->mdlId[127] = Hu3DModelCreateData(0x920020);
    Hu3DModelAttrSet(model->mdlId[127], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(model->mdlId[127], 4);
    Hu3DModelCameraSet(model->mdlId[127], camera->camera[MDCAMERA_BG].bit);
    wavePos3d = wavePos;
    Hu3D2Dto3D(&wavePos3d, camera->camera[MDCAMERA_BG].bit, &wavePos3d);
    Hu3DModelPosSetV(model->mdlId[127], &wavePos3d);
    Hu3DModelRotSet(model->mdlId[127], 90.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(model->mdlId[127], 0.5f, 0.5f, 0.5f);
    Hu3DModelAttrSet(model->mdlId[127], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(model->mdlId[127], HU3D_MOTATTR_PAUSE);
    Hu3DMotionSpeedSet(model->mdlId[127], 1.0f);
    Hu3DModelMatHookSet(model->mdlId[127], WaveEffectDrawHook);
    Hu3DLayerHookSet(0xB, WaveEffectHook);
    indMtxA = waveInd1;
    indMtxB = waveInd0;
    MdOptionWaveEffectIndTexMtxSet(&indMtxB, &indMtxA);
    waveColorCopy = waveColor;
    MdOptionWaveEffectColorSet(&waveColorCopy);
    MdOptionWaveEffectSpeedSet(-0.005f);
    for (i = 0; i < MDOPT_MODEL_MAX; i++) {
        modelP = &Hu3DData[model->mdlId[i]];
        model->posP[i] = &modelP->pos;
        model->rotP[i] = &modelP->rot;
        model->scaleP[i] = &modelP->scale;
    }
    Hu3DLayerHookSet(8, LayerHook);
    Hu3DLayerHookSet(9, LayerHook);
    Hu3DLayerHookSet(0xA, LayerHook);
    Hu3DLayerHookSet(0xD, LayerHook);
    MdModelUpdateAmb(model);
    MdModelRumbleSet((GwCommon.rumbleF ^ 1) * 2);
    if (GwCommon.rumbleF == 0) {
        Hu3DModelAttrSet(model->mdlId[4], HU3D_MOTATTR_PAUSE);
    }
}

void MdModelSoundTypeSet(int type)
{
    MDOPT_MODEL_WORK *model;
    HU3DMODEL *modelP;
    HuVecF *p;
    int i;
    int no;
    float timeTbl[3] = { 0.0f, 12.0f, 0.0f };
    model = sysP->model;
    Hu3DModelAttrSet(model->mdlId[15], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(model->mdlId[26], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(model->mdlId[27], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(model->mdlId[28], HU3D_ATTR_DISPOFF);
    modelP = &Hu3DData[model->mdlId[type + 0x1A]];
    p = model->posP[15];
    modelP->pos.x = p->x;
    modelP->pos.y = p->y;
    modelP->pos.z = p->z;
    model->posP[15] = &modelP->pos;
    p = model->rotP[15];
    modelP->rot.x = p->x;
    modelP->rot.y = p->y;
    modelP->rot.z = p->z;
    model->rotP[15] = &modelP->rot;
    p = model->scaleP[15];
    modelP->scale.x = p->x;
    modelP->scale.y = p->y;
    modelP->scale.z = p->z;
    model->scaleP[15] = &modelP->scale;
    model->mdlId[15] = model->mdlId[type + 0x1A];
    for (i = 0; i < 3; i++) {
        no = (type + i) % 3;
        Hu3DMotionTimeSet(model->mdlId[no + 0x11], timeTbl[no]);
        Hu3DModelAttrSet(model->mdlId[no + 0x11], HU3D_MOTATTR_PAUSE);
    }
    Hu3DModelAttrReset(model->mdlId[type + 0x11], HU3D_MOTATTR_PAUSE);
}

void MdModelRumbleSet(int type)
{
    MDOPT_MODEL_WORK *model = sysP->model;
    SYS *sysWk = sysP;
    if (type == 0) {
        if (sysWk->rumbleState != 0) {
            sysWk->rumbleState = 3;
        }
    } else if (type == 1) {
        if (sysWk->rumbleState == 0) {
            sysWk->rumbleState = 1;
        }
    } else {
        sysWk->rumbleState = 2;
    }
}

void MdModelRumbleUpdate(void)
{
    SYS *sysWk;
    MDOPT_MODEL_WORK *model;
    HuVecF vec;
    sysWk = sysP;
    model = sysWk->model;
    switch (sysWk->rumbleState) {
        case 0:
            Hu3DModelAttrSet(model->mdlId[7], HU3D_ATTR_DISPOFF);
            sysWk->rumbleOut[1] = 0.0f;
            sysWk->rumbleOut[0] = 0.0f;
            sysWk->rumbleT[1] = 0.0f;
            sysWk->rumbleT[0] = 0.0f;
            return;
        case 1:
            Hu3DModelAttrReset(model->mdlId[7], HU3D_ATTR_DISPOFF);
            MdMathOscillate(MDMATH_OSC_90, &sysWk->rumbleT[0], &sysWk->rumbleOut[0], 20.0f);
            MdMathOscillate(MDMATH_OSC_180, &sysWk->rumbleT[1], &sysWk->rumbleOut[1], 20.0f);
            vec.x = model->scaleP[4]->x;
            vec.y = model->scaleP[4]->y;
            PSVECScale(&vec, &vec, sysWk->rumbleOut[0] + 0.75f * sysWk->rumbleOut[1]);
            vec.z = model->scaleP[4]->z;
            *model->scaleP[7] = vec;
            if (sysWk->rumbleT[0] >= 1.0f) {
                sysWk->rumbleState++;
                return;
            }
            return;
        case 2:
            Hu3DModelAttrReset(model->mdlId[7], HU3D_ATTR_DISPOFF);
            *model->scaleP[4] = *model->scaleP[7];
            sysWk->rumbleOut[1] = 0.0f;
            sysWk->rumbleOut[0] = 0.0f;
            sysWk->rumbleT[1] = 0.0f;
            sysWk->rumbleT[0] = 0.0f;
            return;
        case 3:
            Hu3DModelAttrReset(model->mdlId[7], HU3D_ATTR_DISPOFF);
            MdMathOscillate(MDMATH_OSC_90, &sysWk->rumbleT[0], &sysWk->rumbleOut[0], 20.0f);
            MdMathOscillate(MDMATH_OSC_180, &sysWk->rumbleT[1], &sysWk->rumbleOut[1], 20.0f);
            vec.x = model->scaleP[4]->x;
            vec.y = model->scaleP[4]->y;
            PSVECScale(&vec, &vec, (1.0f - sysWk->rumbleOut[0]) + sysWk->rumbleOut[1]);
            vec.z = model->scaleP[4]->z;
            *model->scaleP[7] = vec;
            if (sysWk->rumbleT[0] >= 1.0f) {
                sysWk->rumbleState = 0;
            }
            break;
    }
}

static void FontBGFilter(HU3DMODEL *modelP, Mtx *mtx)
{
    MDDEBUGFONT_WORK *debugFont = sysP->debugFont;
    GXColor color = { 0, 0, 0, 128 };
    s16 width = 0;
    if (debugFont->printF == TRUE) {
        Mtx44 proj;
        width = debugFont->maxLen * (int)(8.0f * debugFont->size);
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
        GXPosition3s16(debugFont->posX, debugFont->posY + debugFont->ofsY, 0);
        GXPosition3s16(debugFont->posX + width, debugFont->posY + debugFont->ofsY, 0);
        GXPosition3s16(debugFont->posX + width, debugFont->posY + debugFont->rowOfsY, 0);
        GXPosition3s16(debugFont->posX, debugFont->posY + debugFont->rowOfsY, 0);
        GXEnd();
    }
}

static void LayerHook(s16 layerNo)
{
    Hu3DZClear();
}

void MdModelAddAmb(MDOPT_MODEL_WORK *model, s16 mdlId)
{
    int i;
    for (i = 0; i < 16; i++) {
        if (model->ambId[i] == -1) {
            model->ambId[i] = mdlId;
            return;
        }
    }
}

void MdModelRemoveAmb(MDOPT_MODEL_WORK *model, s16 mdlId)
{
    int i;
    for (i = 0; i < 16; i++) {
        if (model->ambId[i] == mdlId) {
            model->ambId[i] = -1;
            return;
        }
    }
}

void MdModelUpdateAmb(MDOPT_MODEL_WORK *model)
{
    int i;
    for (i = 0; i < 16; i++) {
        if (model->ambId[i] != -1) {
            Hu3DModelAmbSet(model->ambId[i], model->ambScale, model->ambScale, model->ambScale);
        }
    }
}

void fn_1_17C54(void)
{
}


void MdOptionCursorReset(void)
{
    HUSPRGRPID grpId;
    SYS *sysWk;
    MDSPRITE_WORK *spriteWk;
    sysWk = sysP;
    spriteWk = sysWk->sprite;
    grpId = spriteWk->grpId[MDOPT_GRP_BASE];
    sysWk->cursorState = 0;
    sysWk->cursorOut = 0.0f;
    sysWk->cursorT = 0.0f;
    HuSprScaleSet(grpId, 5, 1.0f, 1.0f);
    HuSprScaleSet(grpId, 7, 1.0f, 1.0f);
    HuSprScaleSet(grpId, 6, 1.0f, 1.0f);
    HuSprScaleSet(grpId, 8, 1.0f, 1.0f);
    HuSprAttrSet(grpId, 7, HUSPR_ATTR_DISPOFF);
    HuSprAttrSet(grpId, 8, HUSPR_ATTR_DISPOFF);
    HuSprPosSet(grpId, 5, 0.0f, -90.0f);
    HuSprPosSet(grpId, 7, 0.0f, -90.0f);
    HuSprPosSet(grpId, 6, 0.0f, 130.0f);
    HuSprPosSet(grpId, 8, 0.0f, 130.0f);
}

void MdOptionCursorSet(int *sel, int dir, int cat, int sub)
{
    MDSPRITE_WORK *spriteWk;
    SYS *sysWk;
    int prevSel;
    sysWk = sysP;
    spriteWk = sysWk->sprite;
    prevSel = *sel;
    if (dir == 0) {
        (*sel)--;
    } else {
        (*sel)++;
    }
    if (sysWk->menu == 0) {
        MdOptionListCursorClamp(cat, sub, sel);
        MdOptionMgListWinUpdate(sysWk->sprite, cat, sub, *sel);
    } else if (sysWk->menu == 3) {
        MdOptionRecordCursorClamp(sel);
        MdOptionMgListWinUpdate(sysWk->sprite, *sel, 0, 0);
    }
    if (prevSel != *sel) {
        fn_1_A930(9);
        MdOptionCursorReset();
        if (sysWk->menu == 0) {
            MdOptionListCursorPosSet(cat, sub, prevSel);
        } else if (sysWk->menu == 3) {
            MdOptionRecordCursorPosSet(prevSel);
        }
    }
    sysWk->cursorState = dir + 1;
}

void MdOptionCursorUpdate(void)
{
    SYS *sysWk;
    HUSPRGRPID grpId;
    int i;
    sysWk = sysP;
    grpId = sysP->sprite->grpId[MDOPT_GRP_BASE];
    if (sysWk->cursorState != 0) {
        MdMathOscillate(MDMATH_OSC_180, &sysWk->cursorT, &sysWk->cursorOut, 20.0f);
        if (sysWk->cursorState == 1) {
            for (i = 0; i < 2; i++) {
                HuSprPosSet(grpId, i * 2 + 5, 0.0f, -90.0f + (-10.0f * sysWk->cursorOut));
                HuSprScaleSet(grpId, i * 2 + 5, 1.0f + (0.6f * sysWk->cursorOut), 1.0f + (0.25f * sysWk->cursorOut));
                if (i == 1) {
                    HuSprTPLvlSet(grpId, i * 2 + 5, sysWk->cursorOut);
                }
            }
        } else {
            for (i = 0; i < 2; i++) {
                HuSprPosSet(grpId, i * 2 + 6, 0.0f, 130.0f + (10.0f * sysWk->cursorOut));
                HuSprScaleSet(grpId, i * 2 + 6, 1.0f + (0.6f * sysWk->cursorOut), 1.0f + (0.25f * sysWk->cursorOut));
                if (i == 1) {
                    HuSprTPLvlSet(grpId, i * 2 + 6, sysWk->cursorOut);
                }
            }
        }
        if (sysWk->cursorT >= 1.0f) {
            if (sysWk->menu == 0) {
                MdOptionListCursorPosSet(sysWk->item, sysWk->sub, sysWk->sel);
            } else if (sysWk->menu == 3) {
                MdOptionRecordCursorPosSet(sysWk->sub);
            }
            sysWk->cursorState = 0;
        }
    }
}

void MdOptionListCursorClamp(int cat, int sub, int *sel)
{
    int num;
    num = ListNumTbl[sub + cat * 5];
    if (*sel < 0) {
        *sel = 0;
        return;
    }
    if (num <= 7) {
        *sel = 0;
        return;
    }
    if (*sel > num - 7) {
        *sel = num - 7;
    }
}

void MdOptionListCursorPosSet(int cat, int sub, int ofs)
{
    int catNo = cat;
    int subNo = sub;
    int num;
    HUSPRGRPID grpId;
    num = ListNumTbl[subNo + catNo * 5];
    grpId = sysP->sprite->grpId[MDOPT_GRP_BASE];
    HuSprAttrReset(grpId, 5, HUSPR_ATTR_DISPOFF);
    HuSprAttrReset(grpId, 6, HUSPR_ATTR_DISPOFF);
    HuSprAttrReset(grpId, 7, HUSPR_ATTR_DISPOFF);
    HuSprAttrReset(grpId, 8, HUSPR_ATTR_DISPOFF);
    HuSprTPLvlSet(grpId, 7, 0.0f);
    HuSprTPLvlSet(grpId, 8, 0.0f);
    if (num <= 7) {
        HuSprAttrSet(grpId, 5, HUSPR_ATTR_DISPOFF);
        HuSprAttrSet(grpId, 6, HUSPR_ATTR_DISPOFF);
        HuSprAttrSet(grpId, 7, HUSPR_ATTR_DISPOFF);
        HuSprAttrSet(grpId, 8, HUSPR_ATTR_DISPOFF);
        return;
    }
    if (ofs == 0) {
        HuSprAttrSet(grpId, 5, HUSPR_ATTR_DISPOFF);
        HuSprAttrSet(grpId, 7, HUSPR_ATTR_DISPOFF);
        return;
    }
    if (ofs >= num - 7) {
        HuSprAttrSet(grpId, 6, HUSPR_ATTR_DISPOFF);
        HuSprAttrSet(grpId, 8, HUSPR_ATTR_DISPOFF);
    }
}

void MdOptionRecordCursorClamp(int *sel)
{
    int num = 12;
    if (*sel < 0) {
        *sel = 0;
        return;
    }
    if (num <= 7) {
        *sel = 0;
        return;
    }
    if (*sel > num - 7) {
        *sel = num - 7;
    }
}

void MdOptionRecordCursorPosSet(int no)
{
    HUSPRGRPID grpId;
    grpId = sysP->sprite->grpId[MDOPT_GRP_BASE];
    HuSprAttrReset(grpId, 5, HUSPR_ATTR_DISPOFF);
    HuSprAttrReset(grpId, 6, HUSPR_ATTR_DISPOFF);
    HuSprAttrReset(grpId, 7, HUSPR_ATTR_DISPOFF);
    HuSprAttrReset(grpId, 8, HUSPR_ATTR_DISPOFF);
    HuSprTPLvlSet(grpId, 7, 0.0f);
    HuSprTPLvlSet(grpId, 8, 0.0f);
    if (no == 0) {
        HuSprAttrSet(grpId, 5, HUSPR_ATTR_DISPOFF);
        HuSprAttrSet(grpId, 7, HUSPR_ATTR_DISPOFF);
        return;
    }
    if (no >= 5) {
        HuSprAttrSet(grpId, 6, HUSPR_ATTR_DISPOFF);
        HuSprAttrSet(grpId, 8, HUSPR_ATTR_DISPOFF);
    }
}

void MdOptionStoryRankUpdate(void)
{
    MDOPT_MODEL_WORK *model;
    int i;
    int j;
    float angle;
    HuVecF vec;
    HuVecF axis0;
    HuVecF axis;
    Mtx mtx;
    Mtx rot;
    angle = 0.0f;
    model = sysP->model;
    switch (sysP->rankPhase) {
        case 0:
            angle = 0.0f;
            sysP->rankFrame++;
            if (sysP->rankFrame >= 180) {
                sysP->rankFrame = 0;
                sysP->rankPhase++;
            }
            break;
        case 1:
            sysP->rankFrame++;
            angle = -(2.0f * (float)sysP->rankFrame);
            if (angle < -20.0f) {
                angle = -20.0f;
                sysP->rankFrame = 0;
                sysP->rankPhase++;
            }
            break;
        case 2:
            sysP->rankFrame++;
            angle = -20.0f + (10.0f * (float)sysP->rankFrame);
            if (angle > 380.0f) {
                angle = 380.0f;
                sysP->rankFrame = 0;
                sysP->rankPhase++;
            }
            break;
        case 3:
            sysP->rankFrame++;
            angle = 380.0f - (float)sysP->rankFrame;
            if (angle < 360.0f) {
                angle = 0.0f;
                sysP->rankPhase = 0;
                sysP->rankFrame = 0;
            }
            break;
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 7; j++) {
            PSVECSubtract(&Hu3DCamera[1].pos, model->posP[i * 7 + j + 0x1D], &vec);
            MdMathMtxRotCalc(&vec, &Hu3DCamera[1].up, mtx, 0.0f);
            axis0.x = 0.0f;
            axis0.y = 0.0f;
            axis0.z = 0.0f;
            axis0.x = mtx[0][1];
            axis0.y = mtx[1][1];
            axis0.z = mtx[2][1];
            axis = axis0;
            PSMTXRotAxisRad(rot, &axis, 0.017453292f * angle);
            PSMTXConcat(rot, mtx, mtx);
            MdMathMtxRotGet(mtx, &vec);
            Hu3DModelRotSetV(model->mdlId[i * 7 + j + 0x1D], &vec);
        }
    }
}

void MdOptionStoryRankStarUpdate(void)
{
    MDOPT_MODEL_WORK *model;
    int i;
    int j;
    float angle;
    float wobble;
    HuVecF vec;
    HuVecF axis0;
    HuVecF axis1;
    HuVecF axis2;
    HuVecF axis3;
    Mtx mtx;
    Mtx rot;
    angle = 0.0f;
    wobble = 0.0f;
    model = sysP->model;
    switch (sysP->starPhase) {
        case 0:
            angle = 0.0f;
            sysP->starFrame++;
            if (sysP->starFrame >= 180) {
                sysP->starFrame = 0;
                sysP->starPhase++;
            }
            wobble = 5.0 * sin(3.141592653589793 * (double)((2160.0f * (float)sysP->starFrame) / 180.0f) / 180.0);
            break;
        case 1:
            sysP->starFrame++;
            angle = -(4.0f * (float)sysP->starFrame);
            if (angle < -40.0f) {
                angle = -40.0f;
                sysP->starFrame = 0;
                sysP->starPhase++;
            }
            break;
        case 2:
            sysP->starFrame++;
            angle = -40.0f + (11.0f * (float)sysP->starFrame);
            if (angle > 400.0f) {
                angle = 400.0f;
                sysP->starFrame = 0;
                sysP->starPhase++;
            }
            break;
        case 3:
            sysP->starFrame++;
            angle = 400.0f - (2.0f * (float)sysP->starFrame);
            if (angle < 360.0f) {
                angle = 0.0f;
                sysP->rankPhase = 0;
                sysP->rankFrame = 0;
                sysP->starPhase = 0;
                sysP->starFrame = 0;
            }
            break;
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 0x23; j++) {
            PSVECSubtract(&Hu3DCamera[0].pos, model->posP[i * 0x23 + j + 0x39], &vec);
            MdMathMtxRotCalc(&vec, &Hu3DCamera[0].up, mtx, 0.0f);
            axis0.x = 0.0f;
            axis0.y = 0.0f;
            axis0.z = 0.0f;
            axis0.x = mtx[0][1];
            axis0.y = mtx[1][1];
            axis0.z = mtx[2][1];
            axis1 = axis0;
            axis2.x = 0.0f;
            axis2.y = 0.0f;
            axis2.z = 0.0f;
            axis2.x = mtx[0][2];
            axis2.y = mtx[1][2];
            axis2.z = mtx[2][2];
            axis3 = axis2;
            PSMTXRotAxisRad(rot, &axis1, 0.017453292f * angle);
            PSMTXConcat(rot, mtx, mtx);
            PSMTXRotAxisRad(rot, &axis3, 0.017453292f * wobble);
            PSMTXConcat(rot, mtx, mtx);
            MdMathMtxRotGet(mtx, &vec);
            Hu3DModelRotSetV(model->mdlId[i * 0x23 + j + 0x39], &vec);
        }
    }
}

void MdOptionTrigSprReset(void)
{
    SYS *sysWk;
    MDSPRITE_WORK *spriteWk;
    HUSPRGRPID grpId;
    sysWk = sysP;
    spriteWk = sysWk->sprite;
    grpId = spriteWk->grpId[MDOPT_GRP_BASE];
    HuSprBankSet(grpId, 3, 0);
    HuSprBankSet(grpId, 4, 2);
    sysWk->trigSprState = sysWk->trigSprFrame = 0;
}

void MdOptionTrigSprSet(int no)
{
    int trigNo = no;
    HUSPRGRPID grpId;
    SYS *sysWk;
    sysWk = sysP;
    grpId = sysWk->sprite->grpId[MDOPT_GRP_BASE];
    MdOptionTrigSprReset();
    sysWk->trigSprState = trigNo + 1;
}

void MdOptionTrigSprUpdate(void)
{
    SYS *sysWk;
    HUSPRGRPID grpId;
    MDSPRITE_WORK *spriteWk;
    sysWk = sysP;
    spriteWk = sysWk->sprite;
    grpId = spriteWk->grpId[MDOPT_GRP_BASE];
    if (sysWk->trigSprState != 0) {
        sysWk->trigSprFrame++;
        if (sysWk->trigSprFrame < 10) {
            if (sysWk->trigSprState == 1) {
                HuSprBankSet(grpId, 3, 1);
                return;
            }
            HuSprBankSet(grpId, 4, 3);
            return;
        }
        HuSprBankSet(grpId, 3, 0);
        HuSprBankSet(grpId, 4, 2);
        sysWk->trigSprState = sysWk->trigSprFrame = 0;
    }
}


void MdOptionWaveEffectSet(int no, int type)
{
    SYS *sysWk = sysP;
    if (sysWk->waveActiveF == 0) {
        sysWk->waveActiveF = 1;
        sysWk->waveNo = no;
        sysWk->waveType = type;
        sysWk->waveTimer = 90;
    }
}

void MdOptionWaveEffectUpdate(void)
{
    SYS *sysWk;
    MDOPT_MODEL_WORK *model;
    MDCAMERA_WORK *camera;
    HuVecF pos3d;
    HuVecF scale3d;
    HuVecF pos2d;
    HuVecF indA0 = { 0.0f, 0.0f, 0.0f };
    HuVecF indA;
    HuVecF indB0 = { 0.0f, 0.0f, 0.0f };
    HuVecF indB;
    sysWk = sysP;
    model = sysWk->model;
    camera = sysWk->camera;
    if (sysWk->waveNo != 0) {
        pos3d = *model->posP[sysWk->waveNo + 2];
        scale3d = *model->scaleP[sysWk->waveNo + 2];
        Hu3DModelAttrReset(model->mdlId[127], HU3D_ATTR_DISPOFF);
        MdMath3Dto2D(&pos3d, camera->camera[MDCAMERA_BG].bit, &pos2d);
        if (sysWk->waveType == 1) {
            pos2d.y -= 15.0f;
        } else if (sysWk->waveType == 2) {
            pos2d.y -= 30.0f;
        }
        pos2d.z *= -1.0f;
        pos2d.z = pos2d.z - 500.0f;
        Hu3D2Dto3D(&pos2d, camera->camera[MDCAMERA_BG].bit, &pos2d);
        Hu3DModelPosSetV(model->mdlId[127], &pos2d);
        if (sysWk->waveType == 0) {
            Hu3DModelRotSet(model->mdlId[127], 90.0f, 0.0f, 0.0f);
        } else if (sysWk->waveType == 1) {
            Hu3DModelRotSet(model->mdlId[127], 45.0f, 0.0f, 0.0f);
        } else if (sysWk->waveType == 2) {
            Hu3DModelRotSet(model->mdlId[127], 30.0f, 0.0f, 0.0f);
        }
        Hu3DModelScaleSet(model->mdlId[127], 0.25f, 1.0f, 0.25f);
        indA0.z = (0.09f * (float)sysWk->waveTimer) / 90.0f;
        indA = indA0;
        indB0.z = (0.09f * (float)sysWk->waveTimer) / 90.0f;
        indB = indB0;
        MdOptionWaveEffectIndTexMtxSet(&indB, &indA);
        MdOptionWaveEffectSpeedSet((-0.02f * (float)sysWk->waveTimer) / 90.0f);
        sysWk->waveTimer--;
        if (sysWk->waveTimer <= 0) {
            sysWk->waveTimer = 0;
            sysWk->waveType = 0;
            sysWk->waveNo = 0;
            Hu3DModelAttrSet(model->mdlId[127], HU3D_ATTR_DISPOFF);
        }
    }
}

void MdOptionWaveEffectModelCreate(void)
{
    SYS *sysWk;
    GXColor color = { 0, 0, 0, 0 };
    sysWk = sysP;
    sysWk->waveTexBuf = HuMemDirectMallocNum(HEAP_MODEL, GXGetTexBufferSize(320, 240, GX_TF_RGB565, GX_FALSE, 0), HU_MEMNUM_OVL);
    sysWk->waveAnim[0] = HuSprAnimRead(HuDataSelHeapReadNum(0x92004C, HU_MEMNUM_OVL, HEAP_MODEL));
    sysWk->waveAnim[1] = HuSprAnimRead(HuDataSelHeapReadNum(0x92004D, HU_MEMNUM_OVL, HEAP_MODEL));
    sysWk->waveIndMtx[1][2] = 0.0f;
    sysWk->waveIndMtx[1][1] = 0.0f;
    sysWk->waveIndMtx[1][0] = 0.0f;
    sysWk->waveIndMtx[0][2] = 0.0f;
    sysWk->waveIndMtx[0][1] = 0.0f;
    sysWk->waveIndMtx[0][0] = 0.0f;
    sysWk->waveColor = color;
    sysWk->waveScroll = 0.0f;
    sysWk->waveSpeed = -0.02f;
}

void MdOptionWaveEffectModelKill(void)
{
    SYS *sysWk;
    sysWk = sysP;
    if (sysWk->waveTexBuf != NULL) {
        HuMemDirectFree(sysWk->waveTexBuf);
        sysWk->waveTexBuf = NULL;
        HuSprAnimKill(sysWk->waveAnim[0]);
        HuSprAnimKill(sysWk->waveAnim[1]);
    }
}

static void WaveEffectHook(s16 layerNo)
{
    HU3DWATER *water;
    SYS *sysWk;
    sysWk = sysP;
    water = &Hu3DWaterData;
    if ((Hu3DCameraBit & 1) != 0) {
        GXSetTexCopySrc(0, 0, 640, 480);
        GXSetTexCopyDst(320, 240, GX_TF_RGB565, GX_TRUE);
        GXCopyTex(sysWk->waveTexBuf, GX_FALSE);
        if (omUPauseFlag + omPauseChk() == 0) {
        }
    }
}

void MdOptionWaveEffectIndTexMtxSet(HuVecF *a, HuVecF *b)
{
    SYS *sysWk = sysP;
    sysWk->waveIndMtx[0][0] = a->x;
    sysWk->waveIndMtx[0][1] = a->y;
    sysWk->waveIndMtx[0][2] = a->z;
    sysWk->waveIndMtx[1][0] = b->x;
    sysWk->waveIndMtx[1][1] = b->y;
    sysWk->waveIndMtx[1][2] = b->z;
}

void MdOptionWaveEffectColorSet(GXColor *color)
{
    sysP->waveColor = *color;
}

void MdOptionWaveEffectSpeedSet(float speed)
{
    sysP->waveSpeed = speed;
}

static void WaveEffectDrawHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    GXTexObj texObj;
    int pauseF;
    GXColor color;
    Mtx proj;
    Mtx inv;
    Mtx tmp;
    Mtx mtx;
    Mtx scl;
    HU3DDRAWOBJ *obj;
    SYS *sysWk;
    obj = drawObj;
    sysWk = sysP;
    pauseF = omUPauseFlag + omPauseChk();
    if (pauseF == 0) {
        sysWk->waveScroll = sysWk->waveScroll + sysWk->waveSpeed;
    }
    GXInitTexObj(&texObj, sysWk->waveTexBuf, 320, 240, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    HuSprTexLoad(sysWk->waveAnim[0], 0, 2, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    HuSprTexLoad(sysWk->waveAnim[1], 0, 3, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    GXSetNumTexGens(3);
    GXSetNumTevStages(3);
    C_MTXLightPerspective(proj, Hu3DCamera->fov, 1.2f, 0.5f, -0.5f, ((float)Hu3DCamera->scissorX + ((float)Hu3DCamera->scissorW * 0.5f)) / 640.0f, 0.5f);
    PSMTXInverse(Hu3DCameraMtx, inv);
    PSMTXConcat(inv, obj->matrix, tmp);
    PSMTXConcat(proj, Hu3DCameraMtx, mtx);
    PSMTXConcat(mtx, tmp, mtx);
    GXLoadTexMtxImm(mtx, 0x21, GX_MTX3x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, 0x21, GX_FALSE, 0x7D);
    PSMTXTrans(tmp, 0.0f, sysWk->waveScroll, 0.0f);
    PSMTXScale(scl, 0.8f, 0.8f, 1.0f);
    PSMTXConcat(scl, tmp, mtx);
    GXLoadTexMtxImm(mtx, 0x1E, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, 0x1E, GX_FALSE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, 0x7D);
    color = sysWk->waveColor;
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD2, GX_TEXMAP0, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP3, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_TEXC, GX_CC_A0, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP2);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_0);
    GXSetIndTexMtx(GX_ITM_0, sysWk->waveIndMtx, -1);
}

void MdOptionCapsuleOrbitUpdate(void)
{
    SYS *sysWk;
    MDOPT_MODEL_WORK *model;
    MDCAMERA_WORK *camera;
    s16 mdlId;
    int i;
    sysWk = sysP;
    model = sysWk->model;
    camera = sysWk->camera;
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            mdlId = model->mdlId[128];
        } else {
            mdlId = model->mdlId[129];
        }
        switch (sysWk->orbitState[i]) {
            case 0:
                Hu3DModelAttrSet(mdlId, HU3D_ATTR_DISPOFF);
                Hu3DModelPosSet(mdlId, 0.0f, -1.0f * (float)frandmod(0x4B0), 0.0f);
                sysWk->orbitTimer[i] = 0;
                sysWk->orbitWait[i] = frandmod(0x960) + 0x960;
                sysWk->orbitState[i]++;
                break;
            case 1:
                sysWk->orbitTimer[i]++;
                if (sysWk->orbitTimer[i] > sysWk->orbitWait[i]) {
                    Hu3DModelAttrReset(mdlId, HU3D_ATTR_DISPOFF);
                    Hu3DMotionTimeSet(mdlId, 0.0f);
                    sysWk->orbitState[i]++;
                }
                break;
            case 2:
                if (Hu3DMotionEndCheck(mdlId) != 0) {
                    sysWk->orbitState[i] = 0;
                }
                break;
        }
    }
}

void MdOptionCapsuleOrbitOutSet(int no)
{
    sysP->orbitOutF = 1;
    sysP->orbitOutNo = no;
}

void MdOptionCapsuleOrbitHideSet(void)
{
    sysP->orbitOutF = 0;
}

void MdOptionCapsuleOrbitOutUpdate(void)
{
    SYS *sysWk;
    MDOPT_MODEL_WORK *model;
    HuVecF vec;
    Mtx mtx;
    sysWk = sysP;
    model = sysWk->model;
    if (sysWk->orbitOutF == 0) {
        Hu3DModelAttrSet(model->mdlId[130], HU3D_ATTR_DISPOFF);
    } else {
        Hu3DModelAttrReset(model->mdlId[130], HU3D_ATTR_DISPOFF);
    }
    PSVECSubtract(&Hu3DCamera[0].pos, model->posP[sysWk->orbitOutNo], &vec);
    PSVECNormalize(&vec, &vec);
    MdMathMtxRotCalc(&vec, &Hu3DCamera[0].up, mtx, 0.0f);
    MdMathMtxRotGet(mtx, &vec);
    Hu3DModelRotSetV(model->mdlId[130], &vec);
    Hu3DModelScaleSet(model->mdlId[130], model->scaleP[sysWk->orbitOutNo]->x, model->scaleP[sysWk->orbitOutNo]->y, model->scaleP[sysWk->orbitOutNo]->z);
    Hu3DModelLayerSet(model->mdlId[130], Hu3DData[model->mdlId[sysWk->orbitOutNo]].layerNo);
    *model->posP[130] = *model->posP[sysWk->orbitOutNo];
}

void MdOptionSpriteTitleUpdate(void)
{
    SYS *sysWk;
    MDSPRITE_WORK *sprite;
    HuVecF pos;
    HuVecF arg0;
    HuVecF arg1;
    sysWk = sysP;
    sprite = sysWk->sprite;
    if (sysWk->unk4D0 == 0) {
        sysWk->titleOut = 0.0f;
        sysWk->titleT = 0.0f;
        return;
    }
    if (sysWk->unk4D0 == 1) {
        MdMathOscillate(MDMATH_OSC_90, &sysWk->titleT, &sysWk->titleOut, 15.0f);
        pos.x = 288.0f;
        pos.y = -128.0f + (192.0f * sysWk->titleOut);
        arg0 = pos;
        MdOptionTitlePosSet(sprite, &arg0);
        if (sysWk->titleT >= 1.0f) {
            sysWk->unk4D0 = 0;
            sysWk->titleOut = 0.0f;
            sysWk->titleT = 0.0f;
        }
    } else {
        MdMathOscillate(MDMATH_OSC_90_REV, &sysWk->titleT, &sysWk->titleOut, 15.0f);
        pos.x = 288.0f;
        pos.y = 64.0f - (192.0f * sysWk->titleOut);
        arg1 = pos;
        MdOptionTitlePosSet(sprite, &arg1);
        if (sysWk->titleT >= 1.0f) {
            sysWk->unk4D0 = 0;
            sysWk->titleOut = 0.0f;
            sysWk->titleT = 0.0f;
        }
    }
}
