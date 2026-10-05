#include "types.h"

#include "game/charman.h"
#include "game/gamework.h"
#include "game/object.h"
#include "game/process.h"

/* per-player board state owned by m502dll */
typedef struct M502PlayerState_s {
    u8 unknown[0x58];
} M502_PLAYER_STATE;

typedef struct M502PlayerWork_s {
    s32 unk0;
    s32 unk4;
    u16 state;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    s32 charNo;
    s32 unk18;
    u32 obj;
    s32 unk20;
    M502_PLAYER_STATE sub;
} M502_PLAYER_WORK;

/* per-player configuration copy, mirrors GwPlayerConf */
typedef struct M502PlayerConf_s {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
} M502_PLAYER_CONF;

typedef struct M502PlayerBody_s M502_PLAYER_BODY;

typedef struct M502PlayerUnit_s {
    M502_PLAYER_BODY *body;
    u16 kind;
    s16 unk6;
    s32 unk8;
    s32 unkC;
} M502_PLAYER_UNIT;

typedef struct M502PlayerBody_s {
    M502_PLAYER_UNIT *master;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s8 unk24;
    s8 unk25[3];
} M502_PLAYER_BODY;

void fn_1_6478(M502_PLAYER_STATE *state);
s32 fn_1_44B8(M502_PLAYER_WORK *work);
void fn_1_4A1C(M502_PLAYER_WORK *work);
s32 fn_1_4E70(M502_PLAYER_WORK *work);
void fn_1_58E0(M502_PLAYER_UNIT *unit);
void fn_1_5EB8(M502_PLAYER_UNIT *unit);

void fn_1_3B88(M502_PLAYER_WORK *work)
{
    fn_1_6478(&work->sub);

    if (work->obj != 0) {
        CharModelKill(work->charNo);
        omDelObjEx(HuPrcCurrentGet(), (OMOBJ *)work->obj);
        work->obj = 0;
    }

    memset(work, 0, sizeof(M502_PLAYER_WORK));
}

// NONMATCHING: every instruction matches; objdiff scores 91.43% only because
// the three bl targets (fn_1_44B8/fn_1_4A1C/fn_1_4E70) are defined in this same
// original object, so their relocations carry an address ours cannot until
// those three functions are written here too.
s32 fn_1_3FA0(M502_PLAYER_WORK *work)
{
    s32 ret = 0;

    switch (work->state) {
    case 0:
        fn_1_44B8(work);
        break;
    case 3:
        fn_1_4A1C(work);
        break;
    case 4:
        if (fn_1_4E70(work)) {
            ret = 1;
        }
        break;
    }
    return ret;
}

void fn_1_57B4(M502_PLAYER_CONF *conf, s32 playerNo, s32 charNo)
{
    memset(conf, 0, sizeof(M502_PLAYER_CONF));

    conf->unk0 = charNo;
    conf->unkC = GwPlayerConf[playerNo].type;
}

void fn_1_5818(M502_PLAYER_CONF *conf)
{
    memset(conf, 0, sizeof(M502_PLAYER_CONF));
}

void fn_1_5848(M502_PLAYER_UNIT *unit)
{
    M502_PLAYER_BODY *body = unit->body;
    M502_PLAYER_UNIT *master = body->master;

    if (unit->unkC != 0 && master->kind == 1) {
        body->unk1C = body->unk20 = body->unk24 = 0;

        if (unit->kind == 0) {
            fn_1_58E0(unit);
        }
        else if (unit->kind == 1) {
            fn_1_5EB8(unit);
        }
    }
}