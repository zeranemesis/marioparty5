// sdroomdll/filename ' the name-entry screen: initialises the message window,
// then on close either keeps what the player typed or rolls a default name.
#include "types.h"
#include "game/sprite.h"
#include "game/window.h"
#include "game/audio.h"
#include "game/process.h"
#include "humath.h"

s32 frandmod(s32 n);

/* The buffer the player typed into, and the pool of preset names (stride 0x11
 * = 17 bytes, so eight chars plus a terminator). */
extern char nameMes[];
extern char nameDefault[];

/* One keyboard-key entry: an ARAM file number, 0 terminating the table.
 * Stored as pairs: an end marker followed by the ARAM file number. */
extern u32 keyboardTbl[];
extern s16 nameGrpId;
extern s16 nameKeyboardNum;
extern u16 lbl_1_bss_A88;
extern s16 lbl_1_bss_56E;
extern s16 lbl_1_bss_570;
extern u16 nameMesTemp[];
extern s16 nameCharNo;

/* Held-directions (bit0 Left, bit1 Right, bit2 Down, bit3 Up). */
extern u16 fn_1_507C(u16 pad);
/* Freshly-pressed buttons (A=0x0100, B=0x0200, L=0x0020, R=0x0040, Start=0x1000). */
extern u16 fn_1_4FD0(u16 pad);

s32 NameEnterClose(s16 winId);

/* One on-screen key: an x/y grid position and the character code it types. */
typedef struct NameKeyboard_s {
    s16 x;
    s16 y;
    u16 val;
} NAMEKEYBOARD;

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
        anim = HuSprAnimRead(HuDataSelHeapReadNum(keyboardTbl[nameKeyboardNum * 2 + 1], 0x10000000, 2));
        spr = HuSprCreate(anim, 0x32, 0);
        HuSprGrpMemberSet(grp, nameKeyboardNum + 2, spr);
        HuSprAttrSet(grp, nameKeyboardNum + 2, 4);
    }

    HuSprGrpPosSet(grp, kGrpX, kGrpY);

    lbl_1_bss_A88 = 0;
    for (name = 0; name < 8; name++) {
        for (c = 0; c < 0x11; c++) {
            *(nameDefault + name * 0x11 + c) = '\0';
        }
    }

    for (name = 0; name < 8; name++) {
        mes = HuWinMesPtrGet(name + 0x19006B);

        for (c = 0; *mes != '\0'; c++, mes++) {
            if (*mes >= '0') {
                *(nameDefault + name * 0x11 + c) = *mes;
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
    keyboard = (NAMEKEYBOARD *)(uintptr_t)keyboardTbl[keyboardNo * 2];
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
                HuWinMesSet(nameWinId, (u32)nameMes);
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
            HuWinMesSet(nameWinId, (u32)nameMes);
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
            keyboard = (NAMEKEYBOARD *)(uintptr_t)keyboardTbl[keyboardNo * 2];
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
        strcpy(nameMes, &nameDefault[frandmod(8) * 0x11]);
        HuWinMesSet(winId, (u32)nameMes);
        return (s16)strlen(nameMes);
    }
    return 0;
}