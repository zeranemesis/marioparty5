// sdroomdll/filename ' the name-entry screen: initialises the message window,
// then on close either keeps what the player typed or rolls a default name.
#include "types.h"
#include "game/sprite.h"
#include "game/window.h"

s32 frandmod(s32 n);

/* The buffer the player typed into, and the pool of preset names (stride 0x11
 * = 17 bytes, so eight chars plus a terminator). */
extern char nameMes[];
extern char nameDefault[];

/* One keyboard-key entry: an ARAM file number, 0 terminating the table. */
extern s32 keyboardTbl[];
extern s16 nameGrpId;
extern s16 nameKeyboardNum;
extern u16 lbl_1_bss_A88;

static const f32 kGrpX = 288.0f;
static const f32 kGrpY = 186.0f;

// NONMATCHING: the original materialises the 0x1000 heap size with lis r4 at each
// call site; a literal gives a single li in a caller-saved register instead,
// which also shifts the anim/sprite registers (r26/r27) by one.
// Builds the name-entry sprite group: the two cursor sprites, every key on the
// on-screen keyboard, then the default-name and saved-name buffers.
void NameEnterInit(void)
{
    HUSPRGRPID grp;
    ANIMDATA *anim;
    HUSPRID spr;
    s16 name;
    s16 c;

    grp = HuSprGrpCreate(5);
    nameGrpId = grp;

    anim = HuSprAnimRead(HuDataSelHeapReadNum((s32)0xBE001B, 0x1000, 2));
    spr = HuSprCreate(anim, 0x2D, 0);
    HuSprGrpMemberSet(grp, 0, spr);
    HuSprColorSet(grp, 0, 0xFF, 0xFF, 0);
    HuSprAttrSet(grp, 0, 4);

    anim = HuSprAnimRead(HuDataSelHeapReadNum((s32)0xBE001C, 0x1000, 2));
    spr = HuSprCreate(anim, 0x2D, 0);
    HuSprGrpMemberSet(grp, 1, spr);
    HuSprColorSet(grp, 1, 0xFF, 0xFF, 0);
    HuSprAttrSet(grp, 1, 4);

    nameKeyboardNum = 0;
    while (keyboardTbl[nameKeyboardNum] != 0) {
        anim = HuSprAnimRead(HuDataSelHeapReadNum(keyboardTbl[nameKeyboardNum], 0x1000, 2));
        spr = HuSprCreate(anim, 0x32, 0);
        HuSprGrpMemberSet(grp, nameKeyboardNum + 2, spr);
        HuSprAttrSet(grp, nameKeyboardNum + 2, 4);
        nameKeyboardNum = nameKeyboardNum + 1;
    }

    HuSprGrpPosSet(grp, kGrpX, kGrpY);

    lbl_1_bss_A88 = 0;
    for (name = 0; name < 8; name++) {
        for (c = 0; c < 0x11; c++) {
            nameDefault[name * 0x11 + c] = '\0';
        }
    }

    for (name = 0; name < 8; name++) {
        char *mes = HuWinMesPtrGet(0x84 + name);

        for (c = 0; mes[c] != '\0'; c++) {
            if (mes[c] >= '0') {
                nameDefault[name * 0x11 + c] = mes[c];
            }
        }
    }
}

void NameEnterMain(s16 arg1, s16 arg2)
{
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