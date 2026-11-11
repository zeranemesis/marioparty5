#ifndef _SYSTEMDLL_SYSMD_H
#define _SYSTEMDLL_SYSMD_H

/* Work structs and prototypes shared by the sibling units of systemDll.
 * NEL_SYS (REL/systemDll.h) owns the global sysWork block; the typed
 * pointers it holds (camera, light, ...) are opaque there, so the units
 * cast them to the full structs below. */
#include "REL/systemDll.h"
#include "game/object.h"
#include "game/printfunc.h"
#include "game/hu3d.h"
#include "game/pad.h"
#include "stdarg.h"
#include "string.h"
#include "game/window.h"
#include "game/sprite.h"

extern NEL_SYS sysWork;

typedef struct SysDebugFont_s {
    /* 0x00 */ s32 createF;
    /* 0x04 */ s32 printF;
    /* 0x08 */ s32 color;
    /* 0x0C */ s16 posX;
    /* 0x0E */ s16 posY;
    /* 0x10 */ s16 ofsY;
    /* 0x12 */ s16 rowOfsY;
    /* 0x14 */ s16 rowH;
    /* 0x16 */ s16 maxLen;
    /* 0x18 */ float size;
} SYSDEBUGFONT;

typedef struct SysCamera_s {
    /* 0x00 */ u16 bit;
    /* 0x04 */ HuVecF pos;
    /* 0x10 */ HuVecF target;
    /* 0x1C */ HuVecF cross;
    /* 0x28 */ HuVecF up;
    /* 0x34 */ HuVecF dir;
    /* 0x40 */ HuVecF hvr;
    /* 0x4C */ float unk4C;
    /* 0x50 */ HuVecF center;
    /* 0x5C */ float unk5C[9];
    /* 0x80 */ float fovy;
} SYSCAMERA;

typedef struct SysCameraWork_s {
    /* 0x000 */ s32 createF;
    /* 0x004 */ SYSCAMERA camera[3];
    /* 0x190 */ s32 selCamera;
} SYSCAMERA_WORK;

typedef struct SysLight_s {
    /* 0x00 */ s32 createF;
    /* 0x04 */ s16 gLightId;
    /* 0x08 */ HuVecF hvr;
    /* 0x14 */ HuVecF center;
    /* 0x20 */ HuVecF pos;
    /* 0x2C */ HuVecF target;
    /* 0x38 */ HuVecF lightPos;
    /* 0x44 */ HuVecF lightDir;
    /* 0x50 */ GXColor rgb;
} SYSLIGHT;

typedef struct SysSprite_s {
    /* 0x00 */ ANIMDATA *animP[31];
    /* 0x7C */ s16 sprId[48];
    /* 0xDC */ s16 grpId[10];
    /* 0xF0 */ float tpLvl;
} SYSSPRITE;

typedef struct SysModel_s {
    /* 0x000 */ s16 mdlId[0x4D];
    /* 0x09A */ s16 motId[0x16];
    /* 0x0C6 */ s16 animId[7];
    /* 0x0D4 */ HuVecF *mdlPos[0x4D];
    /* 0x208 */ HuVecF *mdlRot[0x4D];
    /* 0x33C */ HuVecF *mdlScale[0x4D];
} SYSMODEL;

typedef struct SysTexBufUnit_s {
    /* 0x00 */ void *addr;
    /* 0x04 */ GXTexObj texObj;
    /* 0x24 */ s16 x;
    /* 0x26 */ s16 y;
    /* 0x28 */ u16 w;
    /* 0x2A */ u16 h;
    /* 0x2C */ s32 fmt;
} SYSTEXBUFUNIT;

typedef struct SysTexBuf_s {
    /* 0x000 */ SYSTEXBUFUNIT unit[11];
    /* 0x210 */ s16 unk210;
} SYSTEXBUF;

typedef struct SysRing_s {
    /* 0x00 */ s32 disp;
    /* 0x04 */ s32 mode;
    /* 0x08 */ GXColor color;
    /* 0x0C */ GXColor endColor;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ float time;
    /* 0x18 */ float ofsTime;
    /* 0x1C */ float t;
    /* 0x20 */ float ofsT;
    /* 0x24 */ s32 mdlNo;
    /* 0x28 */ HuVecF **posP;
    /* 0x2C */ HuVecF **rotP;
    /* 0x30 */ HuVecF **scaleP;
    /* 0x34 */ s32 baseMdlNo;
    /* 0x38 */ HuVecF baseOfs;
} SYSRING;

typedef struct SysRingWork_s {
    /* 0x000 */ SYSRING ring[11];
    /* 0x2EC */ float scale;
} SYSRING_WORK;

typedef struct SysParticle_s {
    /* 0x00 */ s32 mode;
    /* 0x04 */ s32 delay;
    /* 0x08 */ float angle;
    /* 0x0C */ float radius;
    /* 0x10 */ float tpLvl;
    /* 0x14 */ float radiusSpeed;
    /* 0x18 */ float angleSpeed;
    /* 0x1C */ float velY;
    /* 0x20 */ float scale;
    /* 0x24 */ float zRot;
    /* 0x28 */ HuVecF pos;
    /* 0x34 */ GXColor color;
} SYSPARTICLE;

typedef struct SysParticleParam_s {
    /* 0x00 */ float baseAlpha;
    /* 0x04 */ float fadeInSpeed2;
    /* 0x08 */ float fadeOutSpeed1;
    /* 0x0C */ float fadeInSpeed1;
    /* 0x10 */ float fadeOutSpeed2;
    /* 0x14 */ float unk14;
    /* 0x18 */ float scaleSpeed1;
    /* 0x1C */ float scaleSpeed2;
    /* 0x20 */ float unk20;
    /* 0x24 */ GXColor color;
} SYSPARTICLE_PARAM;

typedef struct SysParticleBuffer_s SYSPARTICLE_BUFFER;
typedef void (*SYSPARTICLE_UPDATE_HOOK)(SYSPARTICLE_BUFFER *buffer);

struct SysParticleBuffer_s {
    /* 0x00 */ s32 pauseF;
    /* 0x04 */ ANIMDATA *animP;
    /* 0x08 */ s16 mdlId;
    /* 0x0A */ s16 num;
    /* 0x0C */ SYSPARTICLE *particle;
    /* 0x10 */ SYSPARTICLE_PARAM *param;
    /* 0x14 */ HU3DPARTICLE *hookWork;
    /* 0x18 */ HU3DPARTICLEDATA *dataBuf;
    /* 0x1C */ HU3DPARTICLEHOOK hook;
    /* 0x20 */ SYSPARTICLE_UPDATE_HOOK updateHook;
};

typedef struct SysParticleWork_s {
    /* 0x0000 */ SYSPARTICLE data[0x300];
    /* 0xA800 */ SYSPARTICLE_BUFFER buffer[2];
    /* 0xA848 */ SYSPARTICLE_PARAM param;
} SYSPARTICLE_WORK;

typedef void (*SYSFUKI_HOOK)(NEL_SYS *sys, s32 no);

typedef struct SysFuki_s {
    /* 0x00 */ s32 mode;
    /* 0x04 */ s32 grp;
    /* 0x08 */ s32 memberNo;
    /* 0x0C */ float time;
    /* 0x10 */ float ofsTime;
    /* 0x14 */ float t;
    /* 0x18 */ float ofsT;
    /* 0x1C */ SYSFUKI_HOOK hook;
} SYSFUKI;

typedef struct SysMessWin_s {
    /* 0x00 */ s16 winId;
    /* 0x04 */ float unk4;
    /* 0x08 */ float unk8;
    /* 0x0C */ float unkC;
} SYSMESSWIN;

typedef struct SysMessSpr_s {
    /* 0x00 */ s32 mode;
    /* 0x04 */ s32 step;
    /* 0x08 */ float time;
    /* 0x0C */ float ofsTime;
    /* 0x10 */ float t;
    /* 0x14 */ float unk14;
    /* 0x18 */ float pos1;
    /* 0x1C */ float pos2;
    /* 0x20 */ float scale;
} SYSMESSSPR;

typedef struct SysMessInsert_s {
    /* 0x00 */ s32 num;
    /* 0x04 */ s32 mes[16];
    /* 0x44 */ s16 no[16];
} SYSMESSINSERT;

typedef struct SysMess_s {
    /* 0x00 */ s32 createF;
    /* 0x04 */ s32 mode;
    /* 0x08 */ s16 dlgWinId;
    /* 0x0A */ s16 exWinId;
    /* 0x0C */ s16 dlgType;
    /* 0x0E */ s16 choice;
    /* 0x10 */ s32 dlgMesNo;
    /* 0x14 */ SYSMESSINSERT insert;
    /* 0x78 */ SYSMESSWIN win[3];
    /* 0xA8 */ SYSMESSSPR spr;
    /* 0xCC */ s16 dbgSel;
    /* 0xCE */ u8 dbgColor;
} SYSMESS;

void MdMessProc(void);
void MdMessDlgMesSet(int type, s32 mesNo);
void MdMessDlgInsertMesReset(void);
void MdMessDlgInsertMesAdd(s32 mesNo, s16 insertNo);
void MdMessWinMesSet(s32 winNo, s32 mesNo);
void MdMessWinKill(s32 winNo);
void MdMessWinPosSet(s32 winNo, float x, float y);
void MdMessWinSpeedSet(s32 winNo, s16 speed);
void MdMessDlgSprUpdate(SYSMESS *volatile mess);
void MdMessKill(void);

HuVec2f *MdSpritePosGet(int grpNo, int memberNo);
HuVec2f *MdSpriteScaleGet(int grpNo, int memberNo);
void MdMathLerp(float *out, float a, float b, float t);
void MdMathLerpScale(HuVecF *out, float a, float b, float t);
void MathLerpColor(u8 *out, float a, float b, float t);
void MdMathLerpOfs(float *out, float a, float b, float t, float ofs);
void MdMathOscillate(int type, float *in, float *out, float t);

void MdDebugFontCreate(SYSDEBUGFONT *font);
void MdDebugFontUpdate(SYSDEBUGFONT *font);
void MdDebugFontPrint(const char *str, ...);

void MdCameraCreate(SYSCAMERA_WORK *work);
void MdCameraDebug(SYSCAMERA_WORK *work);
void MdCameraUpdate(SYSCAMERA_WORK *work);
void MdCameraUpdateSingle(SYSCAMERA *camera);
void MdCameraPrint(SYSCAMERA_WORK *work);
void MdCameraPosSet(SYSCAMERA *camera, float h, float v, float r, float cx, float cy, float cz);

void MdLightCreate(SYSLIGHT *light);
void MdLightUpdateData(SYSLIGHT *light);
void MdLightDebug(SYSLIGHT *light);
void MdLightPrint(SYSLIGHT *light);

#endif
