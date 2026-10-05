#include "types.h"

/* m580dll player object.  The object is created by omAddObjEx() and owns a
   0x148-byte work block hung off +0x5C; the work block mirrors the fields of
   the per-player configuration GwPlayerConf[] (stride 0xA), so +0x00 is the
   player index and +0x0C the character number taken from that config. */
typedef struct M580_PLAYERWORK {
    s32 playerNo; /* 0x00 */
    s32 unk04;    /* 0x04 */
    s32 unk08;    /* 0x08 */
    s32 charNo;   /* 0x0C */
    u8 pad10[0x3C];
    s32 unk4C;    /* 0x4C */
    u8 pad50[4];
    s32 unk54;    /* 0x54 */
    s32 unk58;    /* 0x58 */
    float unk5C;  /* 0x5C */
    s32 unk60;    /* 0x60 */
    s32 unk64;    /* 0x64 */
} M580_PLAYERWORK;

typedef struct M580_PLAYER {
    u8 pad00[0x4C];
    s32 playerNo;         /* 0x4C */
    s32 unk50;            /* 0x50 */
    s32 unk54;            /* 0x54 */
    u8 pad58[4];
    M580_PLAYERWORK *work; /* 0x5C */
} M580_PLAYER;

extern void CharModelKill(s16 charNo);
extern s32 CharMotionEndCheck(s16 charNo);
extern s16 CharMotionShiftIDGet(s16 charNo);

void fn_1_29EC(void)
{
    CharModelKill(-1);
}

void fn_1_5C64(M580_PLAYER *player, s32 value)
{
    M580_PLAYERWORK *work = player->work;

    work->unk4C = value;
    work->unk54 = 0;
    work->unk58 = 0;
    work->unk5C = 0.0f;
    work->unk60 = 0;
    work->unk64 = 0;
}

s32 fn_1_774C(M580_PLAYER *player)
{
    M580_PLAYERWORK *work = player->work;

    if (CharMotionEndCheck((s16) work->charNo) != 0) {
        if (CharMotionShiftIDGet((s16) work->charNo) < 0) {
            return 1;
        }
    }
    return 0;
}

s32 fn_1_77B8(M580_PLAYER *player)
{
    M580_PLAYERWORK *work = player->work;
    s32 ret = 0;

    if (CharMotionShiftIDGet((s16) work->charNo) < 0) {
        ret = 1;
    }
    return ret;
}

s32 fn_1_7E2C(M580_PLAYER *player)
{
    M580_PLAYERWORK *work = player->work;

    return work->playerNo;
}