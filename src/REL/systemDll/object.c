#include "REL/systemDll/sysmd.h"

static const float lbl_1_rodata_158 = 1.5f;

void MdObjCameraMain(OMOBJ *obj);
void MdObjLightMain(OMOBJ *obj);
void MdObjDebugFontMain(OMOBJ *obj);

#ifdef TARGET_PC
#define CTRL_NEXT(n) ((*ctrl + 1) % (n))
#else
#define CTRL_NEXT(n) (++*ctrl % (n))
#endif

static s32 MdlFileTbl[10][2] = {
    { 0x8B, 0x8C },
    { 0x32, 0x33 },
    { 0xB6, 0xB7 },
    { 0xEC, 0xED },
    { 0xE5, 0xE6 },
    { 0x16, 0x17 },
    { 0xDF, 0xE0 },
    { 0x2C, 0x2D },
    { 0xC8, 0xC9 },
    { 0xAF, 0xB0 },
};

char *MdToueiHookNameTbl[10] = {
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

void MdObjDebugFontCreate(OMOBJ *obj)
{
    SYSDEBUGFONT *font = obj->data;

    MdDebugFontCreate(font);
    obj->objFunc = MdObjDebugFontMain;
}

void MdObjDebugFontMain(OMOBJ *obj)
{
    static char *ctrlStr[5] = {
        "nop",
        "camera",
        "mess",
        "filter",
        "light"
    };
    NEL_SYS *sys = &sysWork;
    SYSDEBUGFONT *font = obj->data;
    s32 *ctrl = &sys->blendDbg.stat;

    MdDebugFontUpdate(font);
    if (sys->unk234 == 0) {
        if (HuPadBtnDown[0] & PAD_BUTTON_LEFT) {
            if (--*ctrl == -1) {
                *ctrl = 1;
            }
        }
        if (HuPadBtnDown[0] & PAD_BUTTON_RIGHT) {
            *ctrl = CTRL_NEXT(2);
        }
        MdDebugFontPrint("<- %s ->", ctrlStr[*ctrl]);
        MdDebugFontPrint("X : FONT COLOR");
        MdDebugFontPrint("Y : FONT ONOFF");
    } else if (sys->unk234 == 1) {
        if (HuPadBtnDown[0] & PAD_BUTTON_LEFT) {
            if (--*ctrl == -1) {
                *ctrl = 4;
            }
        }
        if (HuPadBtnDown[0] & PAD_BUTTON_RIGHT) {
            *ctrl = CTRL_NEXT(5);
        }
        MdDebugFontPrint("<- %s ->", ctrlStr[*ctrl]);
        MdDebugFontPrint("X : FONT COLOR");
        MdDebugFontPrint("Y : FONT ONOFF");
    }
}

#pragma dont_inline on
void MdDebugFontCreate(SYSDEBUGFONT *font)
{
    font->createF = 1;
    font->printF = 0;
    font->color = 14;
    font->posX = 16;
    font->posY = 40;
    font->ofsY = 216;
    font->rowOfsY = 0;
    font->rowH = 15;
    font->size = lbl_1_rodata_158;
    font->maxLen = 0;
}
#pragma dont_inline reset

void MdDebugFontUpdate(SYSDEBUGFONT *font)
{
    font->maxLen = 0;
    font->rowOfsY = font->ofsY;
}

void MdDebugFontPrint(const char *str, ...)
{
    SYSDEBUGFONT *font;
    size_t len;
    NEL_SYS *sys;
    char outBuf[160];
    va_list arg;

    sys = &sysWork;
    font = (SYSDEBUGFONT *)sys->fontData;
    len = 0;
    if (font->printF) {
        va_start(arg, str);
        vsprintf(outBuf, str, arg);
        len = strlen(outBuf);
        if (font->maxLen < len) {
            font->maxLen = len;
        }
        fontcolor = font->color;
        print8(font->posX, font->posY + font->rowOfsY, font->size, "%s", outBuf);
        va_end(arg);
        font->rowOfsY += font->rowH;
    }
}

void MdObjPadMain(void)
{
    NEL_SYS *sys = &sysWork;
    s32 i;

    for (i = 0; i < 4; i++) {
#ifdef TARGET_PC
        u16 *pad = (u16 *)&sys->padStat;

        pad[i] = HuPadDStk[i] & ~pad[4 + i];
        pad[4 + i] = HuPadDStk[i];
#else
        *(u16 *)((u8 *)sys + 0x140 + i * 2) = HuPadDStk[i] & ~*(u16 *)((u8 *)sys + 0x148 + i * 2);
        *(u16 *)((u8 *)sys + 0x148 + i * 2) = HuPadDStk[i];
#endif
    }
}

void MdObjCameraCreate(OMOBJ *obj)
{
    SYSCAMERA_WORK *camera = obj->data;

    MdCameraCreate(camera);
    obj->objFunc = MdObjCameraMain;
}

void MdObjCameraMain(OMOBJ *obj)
{
    NEL_SYS *sys = &sysWork;
    SYSCAMERA_WORK *camera = obj->data;
    s32 *ctrl = &sys->blendDbg.stat;

    MdCameraUpdate(camera);
    if (sys->unk234 != -1 && *ctrl == 1) {
        MdCameraPrint(camera);
    }
}

void MdObjLightCreate(OMOBJ *obj)
{
    SYSLIGHT *light = obj->data;

    MdLightCreate(light);
    obj->objFunc = MdObjLightMain;
}

void MdObjLightMain(OMOBJ *obj)
{
    NEL_SYS *sys = &sysWork;
    SYSLIGHT *light = obj->data;
    s32 *ctrl = &sys->blendDbg.stat;

    if (sys->unk234 != -1 && *ctrl == 4) {
        MdLightDebug(light);
    }
    MdLightUpdateData(light);
    if (sys->unk234 != -1 && *ctrl == 4) {
        MdLightPrint(light);
    }
}
