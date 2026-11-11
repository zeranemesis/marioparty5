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
