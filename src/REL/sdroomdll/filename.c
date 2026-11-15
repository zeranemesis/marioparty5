// sdroomdll/filename ' the name-entry screen: initialises the message window,
// then on close either keeps what the player typed or rolls a default name.
#include "types.h"
#include "game/sprite.h"
#include "game/window.h"
#include "game/audio.h"
#include "game/process.h"
#include "humath.h"

s32 frandmod(s32 n);

#include "REL/sdroomdll/sdroom.h"
#include "string.h"

typedef struct NameKeyboard_s {
    s16 x;
    s16 y;
    u16 val;
} NAMEKEYBOARD;

static NAMEKEYBOARD keyboardMain[] = {
    { 65, 80, 'A' },
    { 95, 80, 'B' },
    { 125, 80, 'C' },
    { 155, 80, 'D' },
    { 185, 80, 'E' },
    { 215, 80, 'F' },
    { 245, 80, 'G' },
    { 275, 80, 'H' },
    { 305, 80, 'I' },
    { 335, 80, 'J' },
    { 365, 80, 'K' },
    { 395, 80, 'L' },
    { 425, 80, 'M' },
    { 455, 80, 'N' },
    { 65, 110, 'O' },
    { 95, 110, 'P' },
    { 125, 110, 'Q' },
    { 155, 110, 'R' },
    { 185, 110, 'S' },
    { 215, 110, 'T' },
    { 245, 110, 'U' },
    { 275, 110, 'V' },
    { 305, 110, 'W' },
    { 335, 110, 'X' },
    { 365, 110, 'Y' },
    { 395, 110, 'Z' },
    { 425, 110, ' ' },
    { 455, 110, ' ' },
    { 65, 140, 'a' },
    { 95, 140, 'b' },
    { 125, 140, 'c' },
    { 155, 140, 'd' },
    { 185, 140, 'e' },
    { 215, 140, 'f' },
    { 245, 140, 'g' },
    { 275, 140, 'h' },
    { 305, 140, 'i' },
    { 335, 140, 'j' },
    { 365, 140, 'k' },
    { 395, 140, 'l' },
    { 425, 140, 'm' },
    { 455, 140, 'n' },
    { 65, 170, 'o' },
    { 95, 170, 'p' },
    { 125, 170, 'q' },
    { 155, 170, 'r' },
    { 185, 170, 's' },
    { 215, 170, 't' },
    { 245, 170, 'u' },
    { 275, 170, 'v' },
    { 305, 170, 'w' },
    { 335, 170, 'x' },
    { 365, 170, 'y' },
    { 395, 170, 'z' },
    { 425, 170, 194 },
    { 455, 170, 195 },
    { 81, 246, 0x100 },
    { 141, 246, 0x200 },
    { 277, 244, 0x400 },
    { 407, 243, 0x500 },
    { 0, 0, 0 },
};

static NAMEKEYBOARD keyboardNumSpc[] = {
    { 125, 80, '1' },
    { 155, 80, '2' },
    { 185, 80, '3' },
    { 215, 80, '4' },
    { 245, 80, '5' },
    { 275, 80, '6' },
    { 305, 80, '7' },
    { 335, 80, '8' },
    { 365, 80, '9' },
    { 395, 80, '0' },
    { 425, 80, 134 },
    { 125, 110, '[' },
    { 155, 110, '\\' },
    { 185, 110, '~' },
    { 215, 110, 127 },
    { 245, 110, '<' },
    { 275, 110, '=' },
    { 305, 110, '_' },
    { 335, 110, ']' },
    { 365, 110, '^' },
    { 395, 110, 130 },
    { 425, 110, 133 },
    { 125, 140, 0xE00|3 },
    { 155, 140, 0xE00|4 },
    { 185, 140, 0xE00|5 },
    { 215, 140, 0xE00|6 },
    { 245, 140, 0xE00|9 },
    { 275, 140, 0xE00|7 },
    { 305, 140, 0xE00|1 },
    { 335, 140, 0xE00|11 },
    { 365, 140, ';' },
    { 395, 140, ':' },
    { 425, 140, '}' },
    { 81, 246, 0x100 },
    { 141, 246, 0x200 },
    { 277, 244, 0x400 },
    { 407, 243, 0x500 },
    { 0, 0, 0 },
};

/* pairs of (key table, ARAM file number of its sprite); a null table ends the list */
static NAMEKEYBOARD *keyboardTbl[3 * 2] = {
    keyboardMain,
    (NAMEKEYBOARD *)(uintptr_t)0xBE0019,
    keyboardNumSpc,
    (NAMEKEYBOARD *)(uintptr_t)0xBE001A,
};

static char nameMes[50];
static s16 nameCharNo;
static u16 nameMesTemp[22];
static char nameDefault[8][17];
static u16 lbl_1_bss_A88;
static s16 nameKeyboardNum;
static s16 nameGrpId;

/* Held-directions (bit0 Left, bit1 Right, bit2 Down, bit3 Up). */
extern u16 fn_1_507C(u16 pad);
/* Freshly-pressed buttons (A=0x0100, B=0x0200, L=0x0020, R=0x0040, Start=0x1000). */
extern u16 fn_1_4FD0(u16 pad);

s32 NameEnterClose(s16 winId);


static const f32 kGrpX = 288.0f;
static const f32 kGrpY = 186.0f;

// Builds the name-entry sprite group: the two cursor sprites, every key on the
// on-screen keyboard, then the default-name and saved-name buffers.
void NameEnterInit(void)
{
    ANIMDATA *anim;
    HUSPRID spr;
    s16 c;
    char *mes;
    HUSPRGRPID grp;
    s16 name;

    grp = HuSprGrpCreate(5);
    nameGrpId = grp;

    anim = HuSprAnimRead(HuDataSelHeapReadNum((s32)0xBE001B, 0x10000000, 2));
    spr = HuSprCreate(anim, 0x2D, 0);
    HuSprGrpMemberSet(grp, 0, spr);
    HuSprColorSet(grp, 0, 0xFF, 0xFF, 0);
    HuSprAttrSet(grp, 0, 4);

    anim = HuSprAnimRead(HuDataSelHeapReadNum((s32)0xBE001C, 0x10000000, 2));
    spr = HuSprCreate(anim, 0x2D, 0);
    HuSprGrpMemberSet(grp, 1, spr);
    HuSprColorSet(grp, 1, 0xFF, 0xFF, 0);
    HuSprAttrSet(grp, 1, 4);

    for (nameKeyboardNum = 0; keyboardTbl[nameKeyboardNum * 2] != 0; nameKeyboardNum++) {
        anim = HuSprAnimRead(HuDataSelHeapReadNum((u32)(uintptr_t)keyboardTbl[nameKeyboardNum * 2 + 1], 0x10000000, 2));
        spr = HuSprCreate(anim, 0x32, 0);
        HuSprGrpMemberSet(grp, nameKeyboardNum + 2, spr);
        HuSprAttrSet(grp, nameKeyboardNum + 2, 4);
    }

    HuSprGrpPosSet(grp, kGrpX, kGrpY);

    lbl_1_bss_A88 = 0;
    for (name = 0; name < 8; name++) {
        for (c = 0; c < 0x11; c++) {
            nameDefault[name][c] = '\0';
        }
    }

    for (name = 0; name < 8; name++) {
        mes = HuWinMesPtrGet(name + 0x19006B);

        for (c = 0; *mes != '\0'; c++, mes++) {
            if (*mes >= '0') {
                nameDefault[name][c] = *mes;
            }
        }
    }
}

// The interactive name-entry loop. `nameOut` receives the confirmed name,
// `pad` selects the controller whose buttons drive the cursor.
s32 NameEnterMain(char *nameOut, s16 pad)
{
    NAMEKEYBOARD *keyboard;
    s16 i;
    s16 keyNo;
    s16 tempIdx;
    s16 keyboardNo;
    s16 cursorIdx;
    HUWINID nameWinId;
    s16 prevKeyboard;
    s16 prevKeyNo;
    s32 result;
    s16 prevCharNo;
    float moveX;
    float moveY;
    float cursorX;
    float cursorY;
    float normX;
    float normY;
    float scale;
    float cursorMag;
    float prevCursorMag;
    float normMag;

    prevKeyboard = keyboardNo = 0;
    keyNo = 0;
    cursorIdx = 0;
    nameCharNo = 0;
    nameMes[0] = 0;
    HuSprDispOn(nameGrpId, keyboardNo + 2);
    HuSprDispOn(nameGrpId, cursorIdx);
    keyboard = keyboardTbl[keyboardNo * 2];
    HuSprPosSet(nameGrpId, cursorIdx, keyboard[keyNo].x - 260, keyboard[keyNo].y - 131);
    for (i = 1; i <= 10; i++) {
        scale = 1.0 - sin(M_PI * (90.0 * (i / 10.0)) / 180.0);
        HuSprGrpPosSet(nameGrpId, 288.0f, 186.0f - 400.0f * scale);
        HuPrcVSleep();
    }
    HuWinExOpen(lbl_1_bss_570);
    HuWinMesSet(lbl_1_bss_570, 0x19062);
    HuWinMesSet(lbl_1_bss_56E, 0x2003);
    HuWinDispOn(lbl_1_bss_56E);
    nameWinId = HuWinCreate(196.0f, 61.0f, 176, 26, 0);
    HuWinBGTPLvlSet(nameWinId, 0.0f);
    HuWinMesSpeedSet(nameWinId, 0);
    HuWinPriSet(nameWinId, 40);
    while (1) {
        moveX = moveY = 0;
        if (fn_1_507C(pad) & 0x1) {
            moveX = -1.0f;
        } else if (fn_1_507C(pad) & 0x2) {
            moveX = 1.0f;
        }
        if (fn_1_507C(pad) & 0x8) {
            moveY = -1.0f;
        } else if (fn_1_507C(pad) & 0x4) {
            moveY = 1.0f;
        }
        if (moveX || moveY) {
            scale = HuMagPoint2D(moveX, moveY);
            normX = moveX / scale;
            normY = moveY / scale;
            prevCursorMag = 409600;
            prevKeyNo = -1;
            for (i = 0; keyboard[i].val; i++) {
                if (i != keyNo) {
                    cursorX = keyboard[i].x - keyboard[keyNo].x;
                    cursorY = keyboard[i].y - keyboard[keyNo].y;
                    cursorMag = HuMagPoint2D(cursorX, cursorY);
                    cursorX = cursorX / cursorMag;
                    cursorY = cursorY / cursorMag;
                    normMag = HuMagPoint2D(cursorX - normX, cursorY - normY);
                    if (keyboard[keyNo].val >= 0x100 && keyboard[keyNo].val <= 0x500 &&
                        keyboard[i].val >= 0x100 && keyboard[i].val <= 0x500 &&
                        normMag < 0.1 && cursorMag < 55 + prevCursorMag) {
                        prevCursorMag = cursorMag;
                        prevKeyNo = i;
                    } else if (normMag < 0.8 && cursorMag < prevCursorMag) {
                        prevCursorMag = cursorMag;
                        prevKeyNo = i;
                    }
                }
            }
            if (prevKeyNo != -1) {
                HuAudFXPlay(0);
                keyNo = prevKeyNo;
                if (keyboard[keyNo].val == 0x400 || keyboard[keyNo].val == 0x500) {
                    HuSprDispOff(nameGrpId, 0);
                    HuSprDispOn(nameGrpId, 1);
                    cursorIdx = 1;
                } else {
                    HuSprDispOn(nameGrpId, 0);
                    HuSprDispOff(nameGrpId, 1);
                    cursorIdx = 0;
                }
            }
        }
        if (fn_1_4FD0(pad) & 0x1000) {
            HuAudFXPlay(0);
            for (keyNo = 0; keyboard[keyNo].val && keyboard[keyNo].val != 0x500; keyNo++)
                ;
            HuSprDispOff(nameGrpId, 0);
            HuSprDispOn(nameGrpId, 1);
            cursorIdx = 1;
            i = NameEnterClose(nameWinId);
            if (i) {
                nameCharNo = i;
                prevCharNo = i;
                for (i = tempIdx = 0; i < prevCharNo; tempIdx++, i++) {
                    if (nameMes[i + 1] == 0x80 || nameMes[i + 1] == 0x81) {
                        nameMesTemp[tempIdx] = (nameMes[i] << 8) | nameMes[i + 1];
                        nameCharNo--;
                        i++;
                    } else {
                        nameMesTemp[tempIdx] = nameMes[i];
                    }
                }
            }
        } else if (fn_1_4FD0(pad) & 0x100) {
            if ((keyboard[keyNo].val < 0x100 || keyboard[keyNo].val > 0x500) && nameCharNo < 8) {
                nameMesTemp[nameCharNo] = keyboard[keyNo].val;
                nameCharNo++;
                for (i = tempIdx = 0; i < nameCharNo; i++) {
                    if (nameMesTemp[i] > 255) {
                        nameMes[tempIdx++] = nameMesTemp[i] >> 8;
                        nameMes[tempIdx++] = nameMesTemp[i];
                    } else {
                        nameMes[tempIdx++] = nameMesTemp[i];
                    }
                }
                nameMes[tempIdx] = 0;
                HuWinMesSet(nameWinId, MESSNUM_PTR(nameMes));
                if (nameCharNo >= 8) {
                    for (keyNo = 0; keyboard[keyNo].val && keyboard[keyNo].val != 0x500; keyNo++)
                        ;
                    HuSprDispOff(nameGrpId, 0);
                    HuSprDispOn(nameGrpId, 1);
                    cursorIdx = 1;
                }
            } else {
                if (keyboard[keyNo].val == 0x100) {
                    keyboardNo = 0;
                } else if (keyboard[keyNo].val == 0x200) {
                    keyboardNo = 1;
                } else if (keyboard[keyNo].val == 0x300) {
                    keyboardNo = 2;
                } else if (keyboard[keyNo].val == 0x400) {
                    goto cancel;
                } else if (keyboard[keyNo].val == 0x500) {
                    HuAudFXPlay(2);
                    if (NameEnterClose(nameWinId)) {
                        HuPrcSleep(30);
                    }
                    i = 0;
                    while (1) {
                        nameOut[i] = nameMes[i];
                        if (nameMes[i] == 0) {
                            break;
                        }
                        i++;
                    }
                    result = 1;
                    goto done;
                }
            }
            HuAudFXPlay(1);
        } else if (fn_1_4FD0(pad) & 0x200) {
        cancel:
            nameCharNo--;
            if (nameCharNo < 0) {
                result = 0;
                goto done;
            }
            HuAudFXPlay(3);
            for (i = tempIdx = 0; i < nameCharNo; i++) {
                if (nameMesTemp[i] > 255) {
                    nameMes[tempIdx++] = nameMesTemp[i] >> 8;
                    nameMes[tempIdx++] = nameMesTemp[i];
                } else {
                    nameMes[tempIdx++] = nameMesTemp[i];
                }
            }
            nameMes[tempIdx] = 0;
            HuWinMesSet(nameWinId, MESSNUM_PTR(nameMes));
        } else if (fn_1_4FD0(pad) & 0x20) {
            keyboardNo--;
            if (keyboardNo < 0) {
                keyboardNo = nameKeyboardNum - 1;
            }
            for (keyNo = 0; keyboard[keyNo].val; keyNo++) {
                if (keyboard[keyNo].val == (keyboardNo + 1) * 256) {
                    break;
                }
            }
            cursorIdx = 0;
        } else if (fn_1_4FD0(pad) & 0x40) {
            keyboardNo++;
            if (keyboardNo >= nameKeyboardNum) {
                keyboardNo = 0;
            }
            for (keyNo = 0; keyboard[keyNo].val; keyNo++) {
                if (keyboard[keyNo].val == (keyboardNo + 1) * 256) {
                    break;
                }
            }
            cursorIdx = 0;
        }
        if (keyboardNo != prevKeyboard) {
            tempIdx = keyboard[keyNo].val;
            HuSprDispOff(nameGrpId, prevKeyboard + 2);
            keyboard = keyboardTbl[keyboardNo * 2];
            HuSprDispOn(nameGrpId, keyboardNo + 2);
            for (keyNo = 0; keyboard[keyNo].val; keyNo++) {
                if (tempIdx == keyboard[keyNo].val) {
                    break;
                }
            }
            HuSprDispOn(nameGrpId, 0);
            HuSprDispOff(nameGrpId, 1);
            HuSprPosSet(nameGrpId, cursorIdx, keyboard[keyNo].x - 260, keyboard[keyNo].y - 131);
            prevKeyboard = keyboardNo;
        }
        HuSprPosSet(nameGrpId, cursorIdx, keyboard[keyNo].x - 260, keyboard[keyNo].y - 131);
        HuPrcVSleep();
    }
done:
    HuWinHomeClear(lbl_1_bss_56E);
    HuWinHomeClear(lbl_1_bss_570);
    HuWinExClose(lbl_1_bss_570);
    HuWinDispOff(lbl_1_bss_56E);
    HuWinKill(nameWinId);
    for (i = 1; i <= 10; i++) {
        scale = 1.0 - cos(M_PI * (90.0 * (i / 10.0)) / 180.0);
        HuSprGrpPosSet(nameGrpId, 288.0f, 186.0f - 400.0f * scale);
        HuPrcVSleep();
    }
    HuSprDispOff(nameGrpId, 0);
    HuSprDispOff(nameGrpId, 1);
    HuSprDispOff(nameGrpId, keyboardNo + 2);
    HuPrcVSleep();
    return result;
}

// Counts the characters the player actually typed ' spaces are not counted '
// and if the result is zero, copies one of the eight preset names in and
// republishes it to the window. Returns the typed length, or 0 when the
// default was substituted.
s32 NameEnterClose(s16 winId)
{
    s16 count;
    s16 i;

    count = 0;
    i = count;
    for (; nameMes[i] != '\0'; i++) {
        if (nameMes[i] != '\0' && nameMes[i] != ' ') {
            count++;
        }
    }

    if (count == 0) {
        strcpy(nameMes, nameDefault[frandmod(8)]);
        HuWinMesSet(winId, MESSNUM_PTR(nameMes));
        return (s16)strlen(nameMes);
    }
    return 0;
}