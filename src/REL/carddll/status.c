// carddll/status.c - Card Party bonus mode: scene state machines, effects, message window.
// Functions are faithful to the asm but not byte-compared (Equivalent).
#include "types.h"
#include "game/process.h"
#include "game/audio.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/esprite.h"
#include "game/pad.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/window.h"
#include "game/wipe.h"
#include "game/sprite.h"
#include "game/memory.h"
#include "game/object.h"
#include "dolphin/mtx.h"
#include "REL/carddll.h"
#include "REL/carddll/status.h"

void *memset(void *dst, s32 c, u32 n);

/* ---- type definitions ---- */
typedef struct {
    s16 model;
    s16 light;
    s32 f_4;
    s32 f_8;
    s32 f_C;
    s32 f_10;
    s32 f_14;
} StatusSlot;

typedef struct {
    u8 a, b, c, d;
} Byte4;

typedef struct {
    s32 f_0;
    s32 f_4;
    s32 f_8;
    s32 f_C;
    f32 f_10;
    s32 f_14;
    s32 f_18;
    s32 f_1C;
    s32 f_20;
    s32 f_24;
    s32 f_28;
    Byte4 f_2C;
} Status1944;

typedef struct {
    s32 f_0;
    s32 f_4;
    s32 f_8;
    StatusSlot *f_C;
} StatusSlotRef;

/* ---- BSS symbols in REVERSE target address order (highest first)
     MWCC -sdata 0 lays out BSS in reverse declaration order:
     last declared = lowest address.
     Target BSS addresses from config/GP5E01_00/rels/carddll/symbols.txt: */
static u8 lbl_1_bss_21D8[0xAC];
static u8 lbl_1_bss_2034[0x1A4];
static StatusSlot lbl_1_bss_201C[1];
static StatusSlot lbl_1_bss_1FD4[3];
static StatusSlot lbl_1_bss_1FBC[1];
static StatusSlot lbl_1_bss_1FA4[1];
static StatusSlot lbl_1_bss_1F44[5];
static StatusSlot lbl_1_bss_1F2C[1];
static StatusSlot lbl_1_bss_1F14[1];
static StatusSlot lbl_1_bss_1EFC[1];
static StatusSlot lbl_1_bss_1ECC[2];
static StatusSlot lbl_1_bss_1E9C[2];
static StatusSlot lbl_1_bss_1E6C[2];
static StatusSlot lbl_1_bss_1E54[1];
static StatusSlot lbl_1_bss_1E3C[1];
static StatusSlot lbl_1_bss_1E24[1];
static StatusSlot lbl_1_bss_1E0C[1];
static StatusSlot lbl_1_bss_1B3C[30];
static StatusSlot lbl_1_bss_1AF4[3];
static StatusSlot lbl_1_bss_1A7C[6];
static StatusSlot lbl_1_bss_1A1C[5];
static StatusSlot lbl_1_bss_1A04[1];
static StatusSlot lbl_1_bss_19EC[1];
static StatusSlot lbl_1_bss_19D4[1];
static StatusSlot lbl_1_bss_19BC[1];
static StatusSlot lbl_1_bss_19A4[1];
static StatusSlot lbl_1_bss_198C[1];
static StatusSlot lbl_1_bss_1974[1];
/* Pointer-bearing blobs (texture/sprite/window pointers): 64-bit builds need more room. */
#ifdef TARGET_PC
static u8 lbl_1_bss_1944[0x60];
#else
static u8 lbl_1_bss_1944[0x30];
#endif
static s32 lbl_1_bss_18DC[0x68 / 4];
static u8 lbl_1_bss_1848[0x94];
static u8 lbl_1_bss_17AC[0x9C];
static u8 lbl_1_bss_12FC[0x4B0];
#ifdef TARGET_PC
u8 lbl_1_bss_12A8[0x68];
#else
u8 lbl_1_bss_12A8[0x54];
#endif

/* ---- lbl_1_data_8D8: 26 records of {s32, s32, s32, StatusSlot *}, 0x10 each.
     DATA symbols use forward declaration order (first declared = lowest address).
     Forward references to BSS require (u32)&sym casts. ---- */
static StatusSlotRef lbl_1_data_8D8[26] = {
    { 1, 0, 1, lbl_1_bss_201C },
    { 3, 1, 1, lbl_1_bss_1FD4 },
    { 1, 1, 1, lbl_1_bss_1FBC },
    { 1, 0, 0, lbl_1_bss_1FA4 },
    { 4, 1, 0, lbl_1_bss_1F44 },
    { 1, 1, 0, lbl_1_bss_1F2C },
    { 1, 1, 1, lbl_1_bss_1F14 },
    { 1, 1, 1, lbl_1_bss_1EFC },
    { 2, 0, 1, lbl_1_bss_1ECC },
    { 2, 1, 1, lbl_1_bss_1E9C },
    { 2, 1, 1, lbl_1_bss_1E6C },
    { 1, 1, 0, lbl_1_bss_1E54 },
    { 1, 0, 0, lbl_1_bss_1E3C },
    { 1, 0, 0, lbl_1_bss_1E24 },
    { 1, 1, 0, lbl_1_bss_1E0C },
    { 30, 0, 1, lbl_1_bss_1B3C },
    { 3, 1, 1, lbl_1_bss_1AF4 },
    { 5, 1, 1, lbl_1_bss_1A7C },
    { 4, 2, 1, lbl_1_bss_1A1C },
    { 1, 0, 1, lbl_1_bss_1A04 },
    { 1, 0, 1, lbl_1_bss_19EC },
    { 1, 0, 1, lbl_1_bss_19D4 },
    { 1, 1, 1, lbl_1_bss_19BC },
    { 1, 1, 1, lbl_1_bss_19A4 },
    { 1, 1, 1, lbl_1_bss_198C },
    { 1, 1, 1, lbl_1_bss_1974 },
};


/* ---- remaining .data of this unit (offsets are REL .data addresses) ---- */
static Vec lbl_1_data_810[3] = {
    { 500.0f, 1000.0f, 500.0f },
    { -800.0f, 400.0f, 800.0f },
    { 300.0f, 800.0f, -1200.0f }
};
static f32 lbl_1_data_834[0x15] = {
    0.24149999022483826f,
    0.12449999153614044f,
    0.24149999022483826f,
    90.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    90.0f,
    0.0f,
    0.0f,
    -90.0f,
    0.0f,
    0.0f,
    180.0f,
    -90.0f,
    0.0f,
    0.0f
};
static char lbl_1_data_888[9] = "dice_jnt";
static f32 lbl_1_data_A78[4][6] = {
    { -50.0f, 300.0f, -1200.0f, -50.0f, 125.0f, 0.0f },
    { -300.0f, 150.0f, -1000.0f, -50.0f, 125.0f, -200.0f },
    { 300.0f, 150.0f, -1000.0f, 0.0f, 125.0f, -200.0f },
    { 0.0f, 150.0f, -400.0f, 0.0f, 225.0f, 0.0f }
};
static f32 lbl_1_data_AF8[6] = { 1000.0f, 300.0f, -1000.0f, 0.0f, 150.0f, -200.0f };
static char lbl_1_data_B98[0x10] = "itemhook_M_1";
static s32 lbl_1_data_BA8[0x43] = {
    0x130000,
    0x130001,
    0x130002,
    0x130007,
    0x130009,
    0x13000A,
    0x13000B,
    0x13000C,
    0x13000D,
    0x13000E,
    0x13000F,
    0x130010,
    0x130011,
    0x130012,
    0x130013,
    0x130014,
    0x130015,
    0x130016,
    0x130017,
    0x130018,
    0x130019,
    0x13001A,
    0x13001B,
    0x13001C,
    0x13001D,
    0x13001E,
    0x13001F,
    0x130020,
    0x130021,
    0x130022,
    0x130023,
    0x130008,
    0x130003,
    0x130004,
    0x130005,
    0x130006,
    0x130024,
    0x130025,
    0x130026,
    0x130027,
    0x130028,
    0x130029,
    0x13002A,
    0x13002B,
    0x13002C,
    0x13002D,
    0x13002E,
    0x13002F,
    0x130030,
    0x130031,
    0x130032,
    0x130033,
    0x130034,
    0x130035,
    0x130036,
    0x130037,
    0x130038,
    0x130039,
    0x13003A,
    0x13003B,
    0x13003C,
    0x13003D,
    0x13003E,
    0x13003F,
    0x130040,
    0x130041,
    0x130042,
};
static s32 lbl_1_data_CB4[0x1B] = {
    0x20000,
    0x20001,
    0x20002,
    0x20003,
    0x20004,
    0x20005,
    0x20006,
    0x20007,
    0x20008,
    0x20009,
    0x2000A,
    0x2000B,
    0x2000C,
    0x2000D,
    0x2000E,
    0x2000F,
    0x20010,
    0x20011,
    0x20012,
    0x20013,
    0x20014,
    0x20015,
    0x20016,
    0x20017,
    0x20018,
    0x20019,
    0x2001A,
};

/* ---- chunk A ---- */
/* ---- chunk A: fn_1_36C00 (card status-effect scene state machine), fn_1_392B8, fn_1_392E8 ---- */

/* externs owned by other carddll units (signatures derived from the call sites) */
double sin(double x);
double cos(double x);

/* view of lbl_1_bss_21D8 (0xAC bytes): status effect scene work */
typedef struct {
    /* 0x00 */ s16 model[2];     /* the two models of the scene */
    /* 0x04 */ s32 timer;
    /* 0x08 */ s32 effId;        /* effect/object id passed to fn_1_3ED3C / fn_1_3F498 / fn_1_3F714 */
    /* 0x0C */ f32 baseY;        /* initial y of the card model */
    /* 0x10 */ Vec pos;          /* card model position */
    /* 0x1C */ Vec rot;          /* card model rotation */
    /* 0x28 */ Vec dest[2];      /* per model target position */
    /* 0x40 */ f32 spin;
    /* 0x44 */ f32 scale;
    /* 0x48 */ f32 scaleBase;
    /* 0x4C */ s32 kind;         /* 0xB / 0xD / 0xE: effect object kind */
    /* 0x50 */ s32 result;       /* 0 / 1 / 2: outcome, indexes the record counters */
    /* 0x54 */ s32 step;         /* camera step counter */
    /* 0x58 */ Vec start3d[2];   /* model positions at start of the fly-in */
    /* 0x70 */ Vec screen[2];    /* same positions projected to 2D */
    /* 0x88 */ Vec target2d;     /* 2D fly-in target */
    /* 0x94 */ Vec camPos;       /* saved camera position */
    /* 0xA0 */ Vec camTarget;    /* saved camera target */
} XA_Eff;
#define A_EFF ((XA_Eff *) lbl_1_bss_21D8)

/* view of lbl_1_bss_12A8 (0x54 bytes) */
typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 f_8;
    /* 0x0C */ u8 pad1[0x1C];
    /* 0x28 */ s32 f_28;
    /* 0x2C */ u8 pad2[0x54 - 0x2C];
} XA_Blk12A8;
#define A_BLK12A8 ((XA_Blk12A8 *) lbl_1_bss_12A8)

/* view of one 0x5C-byte record of the player work (arg0) */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 row;
    /* 0x14 */ s32 col;
    /* 0x18 */ u8 pad1[0x2C - 0x18];
    /* 0x2C */ s32 f_2C;
    /* 0x30 */ u8 pad2[0x40 - 0x30];
    /* 0x40 */ s32 f_40;
    /* 0x44 */ s32 cnt[3];       /* counters indexed by result */
    /* 0x50 */ u8 pad3[0x5C - 0x50];
} XA_Rec;

/* view of one 0x7C-byte entry of lbl_1_bss_2034 */
typedef struct {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ Vec v28;
    /* 0x34 */ Vec v34;
    /* 0x40 */ u8 pad1[0x58 - 0x40];
    /* 0x58 */ f32 f_58;
    /* 0x5C */ u8 pad2[0x7C - 0x5C];
} XA_Slot;

/* player work: 0x170 = current player no, 0x174 + n*4 = record index of player n */
#define A_CURPLR(w) (*(s32 *) ((u8 *) (w) + 0x170))
#define A_RECIDX(w) (*(s32 *) ((u8 *) (w) + 0x174 + A_CURPLR(w) * 4))

/* pick the character specific voice id into lbl_1_bss_20[1] (left untouched for char nos > 6) */
static void A_SetVoice(s32 charNo, s32 result)
{
    static const s32 normal[7] = { 0x1F6, 0x1E2, 0x25A, 0x2AA, 0x282, 0x1BA, 0x296 };
    static const s32 special[7] = { 0x200, 0x1EC, 0x264, 0x2B4, 0x28C, 0x1C4, 0x2A0 };

    if (charNo >= 0 && charNo <= 6) {
        lbl_1_bss_20[1] = (result != 2) ? normal[charNo] : special[charNo];
    }
}

// NONMATCHING: not byte-compared against the original
s32 fn_1_36C00(CardGame *arg0)
{
    s32 done = 0;
    s32 kind;                /* 0: normal, 1: plus, 2: minus (from card flags 0x1000 / 0x2000) */
    s32 cnt = 1;             /* number of models taking part (2 for kind 1) */
    s32 i;
    s32 plr;
    XA_Rec *rec;
    Vec tmp;
    Vec tmp2;
    f32 ease;
    f32 cosv;
    f32 sinv;
    f32 x;
    f32 y;

    rec = (XA_Rec *) ((u8 *) arg0 + A_RECIDX(arg0) * 0x5C);
    if ((lbl_1_bss_8C4[rec->row][rec->col] & 0x1000) != 0) {
        kind = 0;
    } else if ((lbl_1_bss_8C4[rec->row][rec->col] & 0x2000) != 0) {
        kind = 1;
        cnt = 2;
    } else {
        kind = 2;
    }
    switch (lbl_1_bss_1168[1]) {
    case 0:
        memset(lbl_1_bss_21D8, 0, 0xAC);
        Hu3DCamera[1] = Hu3DCamera[0];
        A_EFF->effId = 9;
        if (kind == 0) {
            A_EFF->model[0] = Hu3DModelCreateData(lbl_1_data_B0[0x138 / 4]);
            A_EFF->model[1] = Hu3DModelCreateData(lbl_1_data_B0[0x13C / 4]);
        } else if (kind == 1) {
            A_EFF->model[0] = Hu3DModelCreateData(lbl_1_data_B0[0x138 / 4]);
            A_EFF->model[1] = Hu3DModelCreateData(lbl_1_data_B0[0x138 / 4]);
        } else {
            A_EFF->effId = 0xA;
            A_EFF->model[0] = Hu3DModelCreateData(lbl_1_data_B0[0x13C / 4]);
            A_EFF->model[1] = Hu3DModelCreateData(lbl_1_data_B0[0x138 / 4]);
        }
        fn_1_9C74(&tmp, rec->row, rec->col, 1, 1);
        for (i = 0; i < 2; i++) {
            Hu3DModelPosSetV(A_EFF->model[i], &tmp);
            Hu3DModelRotSet(A_EFF->model[i], -90.0f, 0.0f, 0.0f);
            Hu3DModelCameraSet(A_EFF->model[i], 2);
            Hu3DModelLayerSet(A_EFF->model[i], 7);
            Hu3DModelAttrSet(A_EFF->model[i], 1);
        }
        A_EFF->pos = Hu3DData[lbl_1_bss_478[rec->row][rec->col]].pos;
        A_EFF->rot = Hu3DData[lbl_1_bss_478[rec->row][rec->col]].rot;
        A_EFF->baseY = A_EFF->pos.y;
        A_EFF->scaleBase = 1.0f;
        A_EFF->scale = 1.0f;
        lbl_1_bss_1168[1]++;
        break;
    case 1:
        A_EFF->pos.y += 4.0f;
        Hu3DModelPosSetV(lbl_1_bss_478[rec->row][rec->col], &A_EFF->pos);
        A_EFF->rot.y -= 12.0f;
        if (A_EFF->rot.y < 0.0f) {
            A_EFF->rot.y += 360.0f;
        }
        A_EFF->timer++;
        if (A_EFF->timer >= 0x3C) {
            A_EFF->rot.y = 0.0f;
            for (i = 0; i < cnt; i++) {
                tmp.x = 0.0f;
                tmp.y = 0.0f;
                tmp.z = 0.0f;
                tmp.x = A_EFF->pos.x;
                tmp.y = 100.0f + A_EFF->pos.y;
                tmp.z = A_EFF->pos.z;
                A_EFF->dest[i] = tmp;
                A_EFF->dest[i].x += 6.0f;
                A_EFF->dest[i].z += 8.0f;
                tmp = A_EFF->dest[i];
                fn_1_3ED3C(A_EFF->effId, (void *) &tmp);
                tmp2.x = -90.0f;
                tmp2.y = 0.0f;
                tmp2.z = 0.0f;
                tmp = tmp2;
                fn_1_3F498(A_EFF->effId, i, &tmp);
            }
            A_EFF->timer = 0;
            lbl_1_bss_1168[1]++;
            fn_1_8D04((kind != 2) ? 0x44C : 0x44D, lbl_1_bss_478[rec->row][rec->col], 1);
        }
        Hu3DModelRotSetV(lbl_1_bss_478[rec->row][rec->col], &A_EFF->rot);
        break;
    case 2:
        if (cnt == 2) {
            A_EFF->dest[0].x -= 1.0f;
            A_EFF->dest[1].x += 1.0f;
            tmp = A_EFF->dest[0];
            fn_1_3F3D8(A_EFF->effId, 0, &tmp);
            tmp = A_EFF->dest[1];
            fn_1_3F3D8(A_EFF->effId, 1, &tmp);
        }
        A_EFF->timer++;
        if (A_EFF->timer > 0x37) {
            for (i = 0; i < cnt; i++) {
                Hu3DModelPosSetV(A_EFF->model[i], &A_EFF->dest[i]);
                Hu3DModelAttrReset(A_EFF->model[i], 1);
            }
            A_EFF->timer = 0;
            lbl_1_bss_1168[1]++;
        }
        break;
    case 3:
        A_EFF->timer++;
        if (A_EFF->timer < 0x28 && cnt == 2) {
            A_EFF->dest[0].x -= 1.0f;
            A_EFF->dest[1].x += 1.0f;
            Hu3DModelPosSetV(A_EFF->model[0], &A_EFF->dest[0]);
            Hu3DModelPosSetV(A_EFF->model[1], &A_EFF->dest[1]);
            tmp = A_EFF->dest[0];
            fn_1_3F3D8(A_EFF->effId, 0, &tmp);
            tmp = A_EFF->dest[1];
            fn_1_3F3D8(A_EFF->effId, 1, &tmp);
        }
        if (fn_1_3F714(A_EFF->effId, 0) != 0) {
            A_EFF->result = 1;
            A_EFF->timer = 0;
            if (kind != 1) {
                s32 rnd = frandmod(0x80);

                if (rnd < 2) {
                    lbl_1_bss_1168[1]++;
                    if (kind == 0) {
                        A_EFF->kind = 0xB;
                        A_EFF->result = 2;
                    } else if (rnd == 0) {
                        A_EFF->kind = 0xD;
                        A_EFF->result = 0;
                    } else {
                        A_EFF->kind = 0xE;
                        A_EFF->result = 2;
                    }
                    break;
                }
                A_EFF->result = kind;
            }
            A_EFF->timer = 0;
            lbl_1_bss_1168[1] = 0xA;
        }
        break;
    case 4:
        A_EFF->timer++;
        if (A_EFF->timer > 0x1E) {
            Hu3DModelAttrSet(A_EFF->model[0], 1);
            tmp = A_EFF->dest[0];
            fn_1_3ED3C(A_EFF->kind, (void *) &tmp);
            tmp2.x = -90.0f;
            tmp2.y = 0.0f;
            tmp2.z = 0.0f;
            tmp = tmp2;
            fn_1_3F498(A_EFF->kind, 0, &tmp);
            A_EFF->timer = 0;
            lbl_1_bss_1168[1]++;
            if (A_EFF->result == 2) {
                omVibrate(A_RECIDX(arg0), 0x32, 6, 6);
                if (A_EFF->kind == 0xB) {
                    fn_1_8D04(0x41E, lbl_1_bss_478[rec->row][rec->col], 1);
                }
            }
        }
        break;
    case 5:
        if (A_EFF->result == 0) {
            if (A_EFF->timer == 0) {
                omVibrate(A_RECIDX(arg0), 0x50, 6, 6);
            }
            if (A_EFF->timer == 0x50) {
                omVibrate(A_RECIDX(arg0), 0xA0, 4, 2);
            }
            if (A_EFF->timer == 0x28) {
                fn_1_8D04(0x420, lbl_1_bss_478[rec->row][rec->col], 1);
            }
        }
        if (A_EFF->kind == 0xE) {
            if (A_EFF->timer == 0x28) {
                omVibrate(A_RECIDX(arg0), 0x19, 4, 2);
            }
            if (A_EFF->timer == 0x41) {
                omVibrate(A_RECIDX(arg0), 0x28, 6, 6);
            }
            if (A_EFF->timer == 0x26) {
                fn_1_8D04(0x421, lbl_1_bss_478[rec->row][rec->col], 1);
            }
        }
        A_EFF->timer++;
        if (A_EFF->timer == 0x50 && A_EFF->kind == 0xD) {
            A_EFF->effId = 9;
            tmp = A_EFF->dest[0];
            fn_1_3ED3C(A_EFF->effId, (void *) &tmp);
            tmp2.x = -90.0f;
            tmp2.y = 0.0f;
            tmp2.z = 0.0f;
            tmp = tmp2;
            fn_1_3F498(A_EFF->effId, 0, &tmp);
            fn_1_8D04(0x44C, lbl_1_bss_478[rec->row][rec->col], 1);
        }
        if (fn_1_3F714(A_EFF->kind, 0) != 0) {
            A_EFF->timer = 0;
            fn_1_3F278(A_EFF->kind, 0);
            if (A_EFF->kind == 0xB) {
                tmp = A_EFF->dest[0];
                fn_1_3ED3C(0xC, (void *) &tmp);
                tmp2.x = -90.0f;
                tmp2.y = 0.0f;
                tmp2.z = 0.0f;
                tmp = tmp2;
                fn_1_3F498(0xC, 0, &tmp);
                A_EFF->timer = 0;
                A_EFF->effId = 0xA;
                A_EFF->camPos = Hu3DCamera[0].pos;
                A_EFF->camTarget = Hu3DCamera[0].target;
                A_EFF->step = 0;
                lbl_1_bss_1168[1]++;
            } else {
                if (A_EFF->kind == 0xD) {
                    Hu3DModelPosSetV(A_EFF->model[1], &A_EFF->dest[0]);
                    Hu3DModelAttrReset(A_EFF->model[1], 1);
                    lbl_1_bss_1168[1] = 7;
                } else {
                    Hu3DModelPosSetV(A_EFF->model[0], &A_EFF->dest[0]);
                    Hu3DModelAttrReset(A_EFF->model[0], 1);
                    lbl_1_bss_1168[1] = 0xA;
                }
                A_EFF->timer = 0;
            }
        }
        break;
    case 6:
        if (A_EFF->timer == 0xF) {
            omVibrate(A_RECIDX(arg0), 0xF, 0xC, 0);
            fn_1_8D04(0x41F, A_EFF->model[1], 2);
        }
        A_EFF->timer++;
        if (A_EFF->timer >= 0x1E) {
            if (A_EFF->step < 4) {
                fn_1_3F278(0xC, 0);
                tmp = A_EFF->dest[0];
                fn_1_3ED3C(0xC, (void *) &tmp);
                tmp2.x = -90.0f;
                tmp2.y = 0.0f;
                tmp2.z = 0.0f;
                tmp = tmp2;
                fn_1_3F498(0xC, 0, &tmp);
                if (A_EFF->step < 3) {
                    A_EFF->timer = 0;
                    Hu3DCamera[0].target = A_EFF->dest[0];
                    PSVECAdd(&lbl_1_data_810[A_EFF->step], &Hu3DCamera[0].target, &Hu3DCamera[0].pos);
                } else {
                    Hu3DCamera[0].pos = A_EFF->camPos;
                    Hu3DCamera[0].target = A_EFF->camTarget;
                }
                A_EFF->step++;
            } else {
                if (A_EFF->timer == 0x2D) {
                    tmp = A_EFF->dest[0];
                    fn_1_3ED3C(A_EFF->effId, (void *) &tmp);
                    tmp2.x = -90.0f;
                    tmp2.y = 0.0f;
                    tmp2.z = 0.0f;
                    tmp = tmp2;
                    fn_1_3F498(A_EFF->effId, 0, &tmp);
                    omVibrate(A_RECIDX(arg0), 0xAA, 4, 2);
                    fn_1_8D04(0x41F, A_EFF->model[1], 2);
                    fn_1_8D04(0x44D, lbl_1_bss_478[rec->row][rec->col], 1);
                }
                if (fn_1_3F714(0xC, 0) != 0) {
                    fn_1_3F278(0xC, 0);
                    Hu3DModelPosSetV(A_EFF->model[1], &A_EFF->dest[0]);
                    Hu3DModelAttrReset(A_EFF->model[1], 1);
                    A_EFF->timer = 0;
                    lbl_1_bss_1168[1] = 7;
                }
            }
        }
        break;
    case 7:
        if (fn_1_3F714(A_EFF->effId, 0) != 0) {
            A_EFF->timer = 0;
            lbl_1_bss_1168[1] = 8;
        }
        break;
    case 8:
        A_EFF->timer++;
        if (A_EFF->timer >= 0x1E) {
            if (A_EFF->timer == 0x1E) {
                fn_1_8D04((A_EFF->kind == 0xD) ? 0x44E : 0x455, lbl_1_bss_478[rec->row][rec->col], 1);
            }
            ease = 0.5f * fn_1_4EEA0(2, (f32) (A_EFF->timer - 0x1E) / 30.0f);
            for (i = 0; i < 2; i++) {
                Hu3DModelScaleSet(A_EFF->model[i], 1.0f + ease, 1.0f + ease, 1.0f + ease);
            }
            if (A_EFF->timer >= 0x5A) {
                A_EFF->scaleBase = 1.5f;
                A_EFF->timer = 0;
                lbl_1_bss_1168[1] = 0xA;
            }
        }
        break;
    case 0xA:
        tmp.x = 0.0f;
        tmp.y = 0.0f;
        tmp.z = 0.0f;
        tmp.x = lbl_1_data_528[A_CURPLR(arg0)][0] - 7.0f;
        tmp.y = lbl_1_data_528[A_CURPLR(arg0)][1] - 12.0f;
        A_EFF->target2d = tmp;
        for (i = 0; i < 2; i++) {
            A_EFF->start3d[i] = Hu3DData[A_EFF->model[i]].pos;
            Hu3D3Dto2D(&Hu3DData[A_EFF->model[i]].pos, 1, &A_EFF->screen[i]);
        }
        lbl_1_bss_1168[1]++;
        fn_1_8D04(0x353, A_EFF->model[0], 1);
        break;
    case 0xB:
        fn_1_8D04(0x8353, A_EFF->model[0], 1);
        A_EFF->spin -= 10.0f + 5.5f * fn_1_4EEA0(0, (f32) A_EFF->timer / 60.0f);
        A_EFF->scale = A_EFF->scaleBase - (A_EFF->scaleBase - 0.2f) * fn_1_4EEA0(0, (f32) A_EFF->timer / 60.0f);
        for (i = 0; i < 2; i++) {
            Hu3DModelRotSet(A_EFF->model[i], -90.0f, A_EFF->spin, 0.0f);
            Hu3DModelScaleSet(A_EFF->model[i], A_EFF->scale, A_EFF->scale, A_EFF->scale);
            cosv = (f32) cos(3.141592653589793 * (f64) (90.0f * ((f32) A_EFF->timer / 60.0f)) / 180.0);
            x = A_EFF->screen[i].x + (1.0f - cosv) * (A_EFF->target2d.x - A_EFF->screen[i].x);
            sinv = (f32) sin(3.141592653589793 * (f64) (90.0f * ((f32) A_EFF->timer / 60.0f)) / 180.0);
            y = A_EFF->screen[i].y + sinv * (A_EFF->target2d.y - A_EFF->screen[i].y);
            tmp = A_EFF->start3d[i];
            fn_1_4EDB8(&tmp2, &tmp, x, y);
            Hu3DData[A_EFF->model[i]].pos = tmp2;
        }
        A_EFF->timer++;
        if (A_EFF->timer > 0x3C) {
            Hu3DModelAttrSet(A_EFF->model[0], 1);
            Hu3DModelAttrSet(A_EFF->model[1], 1);
            rec->cnt[A_EFF->result]++;
            rec->f_40 = rec->cnt[0] + (rec->cnt[1] * 2 + -rec->cnt[2]);
            lbl_1_bss_1168[2] = 0;
            lbl_1_bss_1168[1]++;
            fn_1_2FFAC(arg0);
            plr = A_RECIDX(arg0);
            if (A_EFF->result != 2) {
                Hu3DMotionShiftSet(lbl_1_bss_468[plr], lbl_1_bss_430[plr][0], 0.0f, 10.0f, 0);
            } else {
                Hu3DMotionShiftSet(lbl_1_bss_468[plr], lbl_1_bss_430[plr][1], 0.0f, 10.0f, 0);
            }
            A_SetVoice(GwPlayerConf[plr].charNo, A_EFF->result);
            fn_1_8D04(lbl_1_bss_20[1], lbl_1_bss_468[plr], 1);
            lbl_1_bss_20[1] = 0;
            HuAudFXPlay(8);
            if (A_EFF->result == 1) {
                A_EFF->timer = 0;
            }
        }
        if (A_EFF->timer == 0x3A) {
            omVibrate(A_RECIDX(arg0), 0xF, 0xC, 0);
        }
        break;
    case 0xC:
        fn_1_1328C(A_EFF->result);
        if (A_EFF->result == 1) {
            A_EFF->timer++;
            if (A_EFF->timer == 0x14) {
                HuAudFXPlay(8);
            }
            if (A_EFF->timer >= 0x14 && lbl_1_bss_1168[2] == 2) {
                lbl_1_bss_1168[1]++;
            }
        } else if (lbl_1_bss_1168[2] == 2) {
            lbl_1_bss_1168[1]++;
        }
        break;
    case 0xD:
        if (Hu3DMotionEndCheck(lbl_1_bss_468[A_RECIDX(arg0)]) != 0) {
            lbl_1_bss_1168[1]++;
        }
        break;
    case 0xE:
        A_BLK12A8->f_28 = 0x3C;
        A_BLK12A8->f_8 = fn_1_502DC(rec->f_2C);
        if (A_EFF->kind == 0xB) {
            fn_1_4FBC8(arg0, 0x27);
        } else if (A_EFF->kind == 0xD) {
            fn_1_4FBC8(arg0, 0x26);
        } else {
            fn_1_4FBC8(arg0, A_EFF->result + 0x1A);
        }
        lbl_1_bss_1168[2]++;
        lbl_1_bss_1168[1]++;
        break;
    case 0xF:
    case 0x10:
        if (lbl_1_bss_1168[1] == 0xF && fn_1_1328C(A_EFF->result) != 0) {
            lbl_1_bss_1168[1] = 0x10;
        }
        A_EFF->pos.y -= 8.0f;
        Hu3DModelPosSetV(lbl_1_bss_478[rec->row][rec->col], &A_EFF->pos);
        if (A_EFF->pos.y < A_EFF->baseY) {
            A_EFF->pos.y = A_EFF->baseY;
            tmp = A_EFF->pos;
            fn_1_3ED3C(0, (void *) &tmp);
            Hu3DModelPosSetV(lbl_1_bss_478[rec->row][rec->col], &A_EFF->pos);
            lbl_1_bss_1168[1] = 0;
            Hu3DModelKill(A_EFF->model[0]);
            Hu3DModelKill(A_EFF->model[1]);
            fn_1_8D04(0x454, lbl_1_bss_478[rec->row][rec->col], 1);
            lbl_1_bss_1168[1] = 0x63;
            Hu3DModelShadowMapSet(lbl_1_bss_478[rec->row][rec->col]);
        }
        break;
    }
    if (lbl_1_bss_8[1] != 0) {
        if (HuAudSStreamStatGet(lbl_1_bss_8[0]) == 0) {
            HuAudSStreamVolSet(0, 0x7F, 0x3E8);
            lbl_1_bss_8[1] = 0;
            lbl_1_bss_8[0] = 0;
            if (lbl_1_bss_1168[1] == 0x63) {
                lbl_1_bss_1168[1] = 0;
                done = 1;
            }
        }
    } else if (lbl_1_bss_1168[1] == 0x63) {
        lbl_1_bss_1168[1] = 0;
        done = 1;
    }
    return done;
}

// NONMATCHING: not byte-compared against the original
void fn_1_392B8(void)
{
    memset(lbl_1_bss_2034, 0, 0x1A4);
}

// NONMATCHING: not byte-compared against the original
void fn_1_392E8(s16 arg0, void *arg1, void *arg2)
{
    XA_Slot *slot = (XA_Slot *) lbl_1_bss_2034 + arg0;

    slot->v28 = *(Vec *) arg1;
    slot->v34 = *(Vec *) arg2;
    /* frandmod is unsigned: the original converts (rand - 45) unsigned (u32 -> f32) */
    slot->f_58 = (f32) (frandmod(0x5A) - 0x2D);
}

/* ---- chunk B ---- */
/* ---- chunk B: fn_1_393C4 (card bonus: dice roll / result scene state machine) ---- */

/* functions of other carddll units (signatures from the call sites in fn_1_393C4) */

/* One die (0x7C bytes); dice[] starts at lbl_1_bss_2034 + 0x10. */
typedef struct {
    /* 0x00 */ s16 model;
    /* 0x02 */ s16 modelB;
    /* 0x04 */ s16 modelC;
    /* 0x06 */ s16 modelD;
    /* 0x08 */ s16 fxId;
    /* 0x0A */ s16 light;
    /* 0x0C */ s32 f_C;
    /* 0x10 */ s32 f_10;
    /* 0x14 */ s32 f_14;
    /* 0x18 */ Vec posA;
    /* 0x24 */ Vec posB;
    /* 0x30 */ Vec posC;
    /* 0x3C */ Vec posD;
    /* 0x48 */ f32 rotY;
    /* 0x4C */ s32 f_4C;
    /* 0x50 */ s32 f_50;
    /* 0x54 */ s32 f_54;
    /* 0x58 */ Vec force;
    /* 0x64 */ Vec posE;
    /* 0x70 */ f32 f_70;
    /* 0x74 */ f32 f_74;
    /* 0x78 */ s32 f_78;
} XB_Die;

/* View of lbl_1_bss_2034 (0x1A4 bytes). */
typedef struct {
    /* 0x000 */ s32 state;
    /* 0x004 */ s32 f_4;
    /* 0x008 */ s32 f_8;  /* number of dice */
    /* 0x00C */ s32 f_C;  /* dice kind (data_B0[0x39 + f_C]) */
    /* 0x010 */ XB_Die dice[3];
    /* 0x184 */ s16 spr;
    /* 0x186 */ s16 pad186;
    /* 0x188 */ s32 f_188;
    /* 0x18C */ s32 f_18C;
    /* 0x190 */ s32 f_190;
    /* 0x194 */ Vec pos194;
    /* 0x1A0 */ f32 f_1A0;
} XB_Work;

/* Card record of the current player (0x5C bytes, arg0 + recIdx * 0x5C). */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 row;
    /* 0x14 */ s32 col;
    /* 0x18 */ u8 pad18[0x1C];
    /* 0x34 */ s32 f_34;
    /* 0x38 */ u8 pad38[8];
    /* 0x40 */ s32 f_40;
    /* 0x44 */ s32 f_44;
    /* 0x48 */ s32 f_48;
    /* 0x4C */ s32 f_4C;
    /* 0x50 */ s32 flags;
    /* 0x54 */ u8 pad54[8];
} XB_Rec;

#define XB_B ((XB_Work *) lbl_1_bss_2034)
#define XB_W32(a, off) (*(s32 *) ((u8 *) (a) + (off)))
#define XB_RECIDX(a) XB_W32((a), 0x174 + XB_W32((a), 0x170) * 4)

static void XB_Proj(f32 x, f32 y, f32 z, Vec *out)
{
    Vec v;

    v.x = x;
    v.y = y;
    v.z = z;
    Hu3D2Dto3D(&v, 1, out);
}

static void XB_KillDice(XB_Work *B)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        if (B->f_C == 3) {
            Hu3DLLightKill(B->dice[i].modelD, B->dice[i].light);
            Hu3DModelKill(B->dice[i].modelB);
            Hu3DModelKill(B->dice[i].modelC);
            Hu3DModelKill(B->dice[i].modelD);
        }
        Hu3DModelKill(B->dice[i].model);
        fn_1_3F658(0x10, i, 4);
        fn_1_3F278(0x10, i);
    }
}

// NONMATCHING: not byte-compared against the original
s32 fn_1_393C4(CardGame *arg0)
{
    XB_Work *B;
    XB_Rec *rec;
    s32 i;
    s32 rnd;
    s32 all;
    f32 ease;
    Vec tmpV;
    Vec rotV;
    Vec scaleV;
    Vec zeroV;
    Vec pos2D;
    Vec target;
    Vec from;
    Vec outV;
    f32 scales[3];
    s32 layers[3];

    B = XB_B;
    rec = (XB_Rec *) ((u8 *) arg0 + XB_RECIDX(arg0) * 0x5C);
    scales[0] = 0.24149999f;
    scales[1] = 0.12449999f;
    scales[2] = 0.24149999f;
    layers[0] = 0;
    layers[1] = 2;
    layers[2] = 0;
    switch (B->state) {
    case 0:
        Hu3DZClearLayerSet(0xF);
        if (B->f_188 == 0) {
            B->f_18C = 1;
            B->f_8 = (rec->flags & 2) ? 2 : 1;
            if (rec->flags & 2) {
                B->f_C = 1;
            } else {
                B->f_C = (rec->flags & 4) ? 2 : 0;
            }
            if (rec->flags & 0x800) {
                B->f_8++;
            }
            if (rec->flags & 0x2000) {
                B->f_18C = 0;
                B->f_C = 3;
                B->f_8 = 2;
            }
            if ((rec->flags & 0x1000) && B->f_190 == 0) {
                B->f_190 = 1;
                B->f_18C = 0;
                B->f_C = 4;
                B->f_8 = 1;
            }
        } else {
            B->f_18C = 0;
            espDrawNoSet(lbl_1_bss_F9E[0], 0x40);
            espDrawNoSet(lbl_1_bss_F9E[1], 0x40);
            B->spr = espEntry(lbl_1_data_2B4[11], 0, 0);
            espDrawNoSet(B->spr, 0x40);
            espPosSet(B->spr, 0.0f, 0.0f);
            espBankSet(B->spr, 0);
            espDispOff(B->spr);
            fn_1_9C74(&B->pos194, rec->row, rec->col, 1, 1);
            B->pos194.x += -50.0f;
            B->pos194.y += 100.0f;
            B->f_8 = 1;
            B->f_C = 0;
        }
        for (i = 0; i < 3; i++) {
            B->dice[i].model = Hu3DModelCreateData(lbl_1_data_B0[0x39 + B->f_C]);
            Hu3DModelPosSet(B->dice[i].model, 0.0f, 0.0f, 0.0f);
            Hu3DModelAttrSet(B->dice[i].model, 1);
            Hu3DModelCameraSet(B->dice[i].model, 2);
            Hu3DModelLayerSet(B->dice[i].model, (s16) layers[i]);
            if (B->f_C == 3) {
                B->dice[i].modelB = Hu3DModelCreateData(lbl_1_data_B0[0x3E]);
                B->dice[i].modelC = Hu3DModelCreateData(lbl_1_data_B0[0x3F]);
                B->dice[i].modelD = Hu3DModelCreateData(lbl_1_data_B0[0x4E]);
                B->dice[i].light = Hu3DLLightCreate(B->dice[i].modelD, Hu3DCamera[0].pos.x, Hu3DCamera[0].pos.y,
                                                    Hu3DCamera[0].pos.z, 0.0f, 0.0f, -1.0f, 0xFF, 0xFF, 0xFF);
                Hu3DLLightInfinitytSet(B->dice[i].modelD, B->dice[i].light);
                Hu3DModelAttrSet(B->dice[i].modelB, 1);
                Hu3DModelAttrSet(B->dice[i].modelC, 1);
                Hu3DModelAttrSet(B->dice[i].modelD, 1);
                Hu3DModelLayerSet(B->dice[i].modelB, (s16) layers[i]);
                Hu3DModelLayerSet(B->dice[i].modelC, (s16) layers[i]);
                Hu3DModelLayerSet(B->dice[i].modelD, 4);
            }
            Hu3DModelScaleSet(B->dice[i].model, scales[i], scales[i], scales[i]);
            B->dice[i].f_78 = -1;
            B->dice[i].f_14 = i * 10;
        }
        if (B->f_8 == 1) {
            XB_Proj(80.0f, 380.0f, 1000.0f, &B->dice[0].posA);
            XB_Proj(280.0f, 240.0f, 1000.0f, &B->dice[0].posB);
            B->dice[0].rotY = (f32) (frandmod(90) - 45);
            XB_Proj(80.0f, 380.0f, 1000.0f, &B->dice[0].posC);
            XB_Proj(280.0f, 240.0f, 1000.0f, &B->dice[0].posD);
        } else if (B->f_8 == 2) {
            XB_Proj(60.0f, 360.0f, 1000.0f, &B->dice[0].posA);
            XB_Proj(260.0f, 220.0f, 1000.0f, &B->dice[0].posB);
            B->dice[0].rotY = (f32) (frandmod(90) - 45);
            XB_Proj(60.0f, 360.0f, 1000.0f, &B->dice[0].posC);
            XB_Proj(260.0f, 220.0f, 1000.0f, &B->dice[0].posD);
            XB_Proj(100.0f, 400.0f, 500.0f, &B->dice[1].posA);
            XB_Proj(300.0f, 260.0f, 500.0f, &B->dice[1].posB);
            B->dice[1].rotY = (f32) (frandmod(90) - 45);
            XB_Proj(100.0f, 400.0f, 500.0f, &B->dice[1].posC);
            XB_Proj(300.0f, 260.0f, 500.0f, &B->dice[1].posD);
        } else {
            XB_Proj(60.0f, 360.0f, 1000.0f, &B->dice[0].posA);
            XB_Proj(260.0f, 220.0f, 1000.0f, &B->dice[0].posB);
            B->dice[0].rotY = (f32) (frandmod(90) - 45);
            XB_Proj(60.0f, 360.0f, 1000.0f, &B->dice[0].posC);
            XB_Proj(260.0f, 220.0f, 1000.0f, &B->dice[0].posD);
            B->dice[0].f_14 = 10;
            XB_Proj(100.0f, 400.0f, 500.0f, &B->dice[1].posA);
            XB_Proj(300.0f, 260.0f, 500.0f, &B->dice[1].posB);
            B->dice[1].rotY = (f32) (frandmod(90) - 45);
            XB_Proj(100.0f, 400.0f, 500.0f, &B->dice[1].posC);
            XB_Proj(300.0f, 260.0f, 500.0f, &B->dice[1].posD);
            B->dice[1].f_14 = 20;
            XB_Proj(135.0f, 355.0f, 1000.0f, &B->dice[2].posA);
            XB_Proj(335.0f, 215.0f, 1000.0f, &B->dice[2].posB);
            B->dice[2].rotY = (f32) (frandmod(90) - 45);
            XB_Proj(135.0f, 355.0f, 1000.0f, &B->dice[2].posC);
            XB_Proj(335.0f, 215.0f, 1000.0f, &B->dice[2].posD);
            B->dice[2].f_14 = 0;
        }
        for (i = 0; i < B->f_8; i++) {
            Hu3DMotionTimeSet(B->dice[i].model, 1.0f);
            Hu3DMotionSpeedSet(B->dice[i].model, 0.0f);
            if (B->f_188 != 0) {
                Vec tmp;
                Hu3D3Dto2D(&B->pos194, 1, &B->pos194);
                tmp.x = B->pos194.x;
                tmp.y = B->pos194.y;
                tmp.z = 1000.0f;
                Hu3D2Dto3D(&tmp, 1, &B->pos194);
                Hu3DModelPosSetV(B->dice[i].model, &B->pos194);
            } else {
                Hu3DModelPosSetV(B->dice[i].model, &B->dice[i].posA);
            }
            Hu3DModelRotSet(B->dice[i].model, 0.0f, B->dice[i].rotY, 0.0f);
            B->dice[i].force.x = 0.0f;
            B->dice[i].force.y = 0.0f;
            B->dice[i].force.z = 0.0f;
            rnd = frandmod(100);
            if (rec->flags & 4) {
                if (rnd < 0x50) {
                    B->dice[i].f_78 = frandmod(2);
                } else {
                    B->dice[i].f_78 = 2;
                }
            } else if (rec->flags & 2) {
                if (rnd < 10) {
                    B->dice[i].f_78 = frandmod(2);
                } else if (rnd < 20) {
                    B->dice[i].f_78 = 2;
                } else {
                    B->dice[i].f_78 = frandmod(3) + 3;
                }
            } else {
                if (rnd < 10) {
                    B->dice[i].f_78 = frandmod(2);
                } else {
                    B->dice[i].f_78 = frandmod(4) + 2;
                }
            }
            if (B->f_188 != 0) {
                if (rnd < 10) {
                    B->dice[i].f_78 = frandmod(2);
                } else {
                    B->dice[i].f_78 = frandmod(4) + 2;
                }
            }
            B->dice[i].f_4C = frandmod(3) + 2;
            B->dice[i].f_50 = 1;
            B->dice[i].f_54 = 1;
            if (B->f_188 != 0) {
                Hu3D3Dto2D(&B->pos194, 1, &tmpV);
                tmpV.z = 1000.0f;
                Hu3D2Dto3D(&tmpV, 1, &tmpV);
                fn_1_3ED3C(0x10, &tmpV);
                omVibrate((s16) XB_RECIDX(arg0), 0xF, 0xC, 0);
            } else {
                tmpV = B->dice[i].posC;
                fn_1_3ED3C(0x10, &tmpV);
            }
            rotV.x = 0.0f;
            rotV.y = B->dice[i].rotY;
            rotV.z = 0.0f;
            fn_1_3F498(0x10, i, &rotV);
            fn_1_3F380(0x10, i, 2);
            fn_1_3F658(0x10, i, layers[i] + 1);
            scaleV.x = lbl_1_data_834[i];
            scaleV.y = lbl_1_data_834[i];
            scaleV.z = lbl_1_data_834[i];
            fn_1_3F6B0(0x10, i, &scaleV);
            B->f_1A0 = 2.0f;
            fn_1_3F594(0x10, i, B->f_1A0);
            Hu3DMotionForceSet(B->dice[i].model, lbl_1_data_888, 0x80, B->dice[i].force.x);
            Hu3DMotionForceSet(B->dice[i].model, lbl_1_data_888, 0x100, B->dice[i].force.y);
            Hu3DMotionForceSet(B->dice[i].model, lbl_1_data_888, 0x200, B->dice[i].force.z);
        }
        fn_1_8D04(0x442, B->dice[0].model, 1);
        B->f_4 = 0;
        if (B->f_188 != 0) {
            B->state = 11;
            break;
        }
        B->state++;
        break;
    case 1:
        B->f_4++;
        if ((f32) B->f_4 == 12.0f / B->f_1A0) {
            for (i = 0; i < B->f_8; i++) {
                Hu3DModelAttrReset(B->dice[i].model, 1);
            }
        }
        if ((f32) B->f_4 >= 60.0f / B->f_1A0) {
            if (GwPlayerConf[XB_RECIDX(arg0)].type == 0) {
                if (B->f_18C == 1) {
                    if (rec->f_34 == -1 || (rec->flags & 0x20)) {
                        fn_1_502E0(0xF);
                    } else {
                        fn_1_502E0(0x10);
                    }
                } else {
                    fn_1_502E0(0x18);
                }
            }
            B->state++;
            HuAudFXPlay(0x35C);
            lbl_1_bss_1C = fn_1_8D04(0x2F7, B->dice[0].model, 1);
        }
        break;
    case 2:
        for (i = 0; i < B->f_8; i++) {
            B->dice[i].force.x += 50.0f;
            B->dice[i].force.y += 30.0f;
            B->dice[i].force.z += 50.0f;
            Hu3DMotionForceSet(B->dice[i].model, lbl_1_data_888, 0x80, B->dice[i].force.x);
            Hu3DMotionForceSet(B->dice[i].model, lbl_1_data_888, 0x100, B->dice[i].force.y);
            Hu3DMotionForceSet(B->dice[i].model, lbl_1_data_888, 0x200, B->dice[i].force.z);
        }
        if (HuPadBtnDown[XB_RECIDX(arg0)] & 0x100) {
            for (i = 0; i < B->f_8; i++) {
                B->dice[i].f_10 = 0;
                B->dice[i].f_C = 0;
            }
            fn_1_50460();
            B->state++;
            fn_1_8D04(0x446, B->dice[0].model, 1);
            HuAudFXStop(lbl_1_bss_1C);
            lbl_1_bss_20[8] = 0;
            break;
        }
        if (B->f_18C == 0) {
            break;
        }
        if (HuPadBtnDown[XB_RECIDX(arg0)] & 0x1000) {
            B->state = 0;
            for (i = 0; i < B->f_8; i++) {
                Hu3DModelAttrSet(B->dice[i].model, 1);
                fn_1_3F278(0x10, i);
            }
            Hu3DLayerHookSet(0xF, NULL);
            fn_1_50460();
            XB_W32(arg0, 0x188) = 0xCA;
            HuAudFXPlay(0x3FA);
            HuAudSStreamVolSet(0, 0x60, 0x1F4);
        } else if (HuPadBtnDown[XB_RECIDX(arg0)] & 0x200) {
            if (rec->f_34 == -1) {
                break;
            }
            if (rec->flags & 0x20) {
                break;
            }
            B->state = 0;
            for (i = 0; i < B->f_8; i++) {
                Hu3DModelAttrSet(B->dice[i].model, 1);
                fn_1_3F278(0x10, i);
            }
            fn_1_50460();
            Hu3DLayerHookSet(0xF, NULL);
            XB_W32(arg0, 0x188) = 0xCB;
            XB_W32(arg0, 0x19C) = 7;
            HuAudFXPlay(0x3F9);
            HuAudSStreamVolSet(0, 0x60, 0x1F4);
        } else if (HuPadBtnDown[XB_RECIDX(arg0)] & 0x800) {
            B->state = 0;
            for (i = 0; i < B->f_8; i++) {
                Hu3DModelAttrSet(B->dice[i].model, 1);
                fn_1_3F278(0x10, i);
            }
            fn_1_50460();
            Hu3DLayerHookSet(0xF, NULL);
            XB_W32(arg0, 0x188) = 0xCD;
            HuAudFXPlay(0x3FB);
            HuAudSStreamVolSet(0, 0x60, 0x1F4);
        } else if (HuPadBtnDown[XB_RECIDX(arg0)] & 0x20) {
            B->state = 0;
            for (i = 0; i < B->f_8; i++) {
                Hu3DModelAttrSet(B->dice[i].model, 1);
                fn_1_3F278(0x10, i);
            }
            fn_1_50460();
            Hu3DLayerHookSet(0xF, NULL);
            XB_W32(arg0, 0x188) = 0xCC;
            HuAudFXPlay(0x3FB);
            HuAudSStreamVolSet(0, 0x60, 0x1F4);
        }
        if (B->state != 0) {
            break;
        }
        XB_KillDice(B);
        if (B->f_188 != 0) {
            espKill(B->spr);
            espDrawNoSet(lbl_1_bss_F9E[0], 0);
            espDrawNoSet(lbl_1_bss_F9E[1], 0);
        }
        HuAudFXStop(lbl_1_bss_1C);
        break;
    case 3:
        lbl_1_bss_20[8]++;
        if (lbl_1_bss_20[8] < 0x2F) {
            fn_1_8D04(0x8446, B->dice[0].model, 1);
        }
        all = 1;
        for (i = 0; i < B->f_8; i++) {
            lbl_1_bss_20[9] = i + 1;
            if (fn_1_3D780(&B->dice[i]) == 0) {
                all = 0;
            }
        }
        if (all != 0) {
            B->state++;
            B->f_4 = 0;
            lbl_1_bss_20[9] = 0;
            lbl_1_bss_20[8] = 0;
        }
        break;
    case 4:
        B->f_4++;
        if (B->f_4 < 0x2D) {
            break;
        }
        all = 0;
        for (i = 0; i < B->f_8; i++) {
            tmpV = B->dice[i].posD;
            fn_1_3ED3C(0x10, &tmpV);
            rotV.x = 0.0f;
            rotV.y = B->dice[i].rotY;
            rotV.z = 0.0f;
            fn_1_3F498(0x10, i, &rotV);
            fn_1_3F380(0x10, i, 2);
            scaleV.x = lbl_1_data_834[i];
            scaleV.y = lbl_1_data_834[i];
            scaleV.z = lbl_1_data_834[i];
            fn_1_3F6B0(0x10, i, &scaleV);
            if (B->f_188 != 0) {
                fn_1_3F658(0x10, i, 7);
            } else {
                fn_1_3F658(0x10, i, layers[i] + 1);
            }
            fn_1_3F594(0x10, i, B->f_1A0);
            if (B->f_C == 3 && B->dice[i].f_78 == 3) {
                fn_1_3F4EC(0x10, i, 1);
                Hu3DModelAttrSet(B->dice[i].model, 1);
                Hu3DModelAttrReset(B->dice[i].modelB, 1);
                Hu3DMotionShapeTimeSet(B->dice[i].modelB, 0.0f);
                Hu3DMotionShapeSpeedSet(B->dice[i].modelB, 2.0f);
                Hu3DData[B->dice[i].modelC].pos = Hu3DData[B->dice[i].model].pos;
                Hu3DData[B->dice[i].modelB].pos = Hu3DData[B->dice[i].modelC].pos;
                Hu3DData[B->dice[i].modelC].rot = Hu3DData[B->dice[i].model].rot;
                Hu3DData[B->dice[i].modelB].rot = Hu3DData[B->dice[i].modelC].rot;
                zeroV.x = 0.0f;
                zeroV.y = 0.0f;
                zeroV.z = 0.0f;
                Hu3DData[B->dice[i].modelD].rot = zeroV;
                Hu3DData[B->dice[i].modelD].scale = Hu3DData[B->dice[i].model].scale;
                Hu3DData[B->dice[i].modelC].scale = Hu3DData[B->dice[i].modelD].scale;
                Hu3DData[B->dice[i].modelB].scale = Hu3DData[B->dice[i].modelC].scale;
                if (i == 0) {
                    tmpV.x = 260.0f;
                    tmpV.y = 215.0f;
                    tmpV.z = 900.0f;
                } else {
                    tmpV.x = 300.0f;
                    tmpV.y = 255.0f;
                    tmpV.z = 450.0f;
                }
                Hu3D2Dto3D(&tmpV, 1, &tmpV);
                Hu3DData[B->dice[i].modelD].pos = tmpV;
                all = 1;
            }
        }
        if (B->f_C != 3 || B->dice[0].f_78 != 3 || B->dice[1].f_78 != 3) {
            fn_1_8D04(0x445, B->dice[0].model, 1);
        }
        B->f_4 = 0;
        if (all == 0) {
            B->state++;
        } else {
            B->state += 2;
        }
        break;
    case 5:
        if ((f32) B->f_4 == 12.0f / B->f_1A0) {
            for (i = 0; i < B->f_8; i++) {
                Hu3DModelAttrSet(B->dice[i].model, 1);
            }
            if (B->f_188 != 0) {
                espPosSet(B->spr, 286.0f, 238.0f);
                espBankSet(B->spr, (s16) (B->dice[0].f_78 + 1));
                espDispOn(B->spr);
                B->f_4 = 0;
                B->state = 13;
            }
        }
        B->f_4++;
        if ((f32) B->f_4 < 60.0f / B->f_1A0) {
            break;
        }
        XB_W32(arg0, 0x184) = B->dice[0].f_78 + (B->dice[1].f_78 + B->dice[2].f_78 + 3);
        B->state = 0x14;
        if (B->f_C == 3) {
            XB_W32(lbl_1_bss_12A8, 8) = GwPlayerConf[XB_RECIDX(arg0)].charNo;
            fn_1_4FBC8(arg0, 0x35);
        }
        break;
    case 6:
        if ((f32) B->f_4 == 12.0f / B->f_1A0) {
            for (i = 0; i < B->f_8; i++) {
                if (B->dice[i].f_78 != 3) {
                    Hu3DModelAttrSet(B->dice[i].model, 1);
                }
            }
        }
        B->f_4++;
        if (B->f_4 < 0x14) {
            break;
        }
        for (i = 0; i < 2; i++) {
            if (B->dice[i].f_78 == 3) {
                Hu3DModelAttrSet(B->dice[i].modelB, 1);
                Hu3DModelAttrReset(B->dice[i].modelC, 1);
                Hu3DMotionShapeTimeSet(B->dice[i].modelC, 0.0f);
                Hu3DMotionShapeSpeedSet(B->dice[i].modelC, 2.0f);
                tmpV = Hu3DData[B->dice[i].modelC].pos;
                B->dice[i].fxId = (s16) fn_1_3ED3C(9, &tmpV);
                scaleV.x = scales[i];
                scaleV.y = scales[i];
                scaleV.z = scales[i];
                fn_1_3F6B0(9, B->dice[i].fxId, &scaleV);
                fn_1_8D04(0x42F, B->dice[i].modelC, 1);
                fn_1_8D04(0x457, B->dice[i].modelC, 1);
            }
        }
        B->f_4 = 0;
        B->state++;
        break;
    case 7:
        B->f_4++;
        if (B->f_4 < 0x37) {
            break;
        }
        for (i = 0; i < 2; i++) {
            if (B->dice[i].f_78 == 3) {
                Hu3DModelAttrReset(B->dice[i].modelD, 1);
                Hu3DModelCameraSet(B->dice[i].modelD, 2);
                Hu3DModelLayerSet(B->dice[i].modelD, 7);
            }
        }
        B->f_4 = 0;
        B->state++;
        break;
    case 8:
        all = 0;
        for (i = 0; i < 2; i++) {
            if (B->dice[i].f_78 == 3 && fn_1_3F714(9, B->dice[i].fxId) != 0) {
                B->dice[i].posE = Hu3DData[B->dice[i].modelD].pos;
                Hu3D3Dto2D(&Hu3DData[B->dice[i].modelD].pos, 1, &pos2D);
                target.x = lbl_1_data_528[XB_W32(arg0, 0x170)][0] - 12.0f;
                target.y = lbl_1_data_528[XB_W32(arg0, 0x170)][1] - 12.0f;
                target.z = 0.0f;
                fn_1_4F144(i, &pos2D, &target, 0x3C, 0.0f, 0x21);
                B->dice[i].f_74 = scales[i];
                B->dice[i].f_70 = 0.0f;
                all = 1;
                lbl_1_bss_20[0] = i;
            }
        }
        if (all == 1) {
            B->f_4 = 0;
            B->state++;
            fn_1_8D04(0x8353, B->dice[lbl_1_bss_20[0]].modelD, 1);
        }
        break;
    case 9:
        fn_1_8D04(0x8353, B->dice[lbl_1_bss_20[0]].modelD, 1);
        B->f_4++;
        all = 0;
        for (i = 0; i < 2; i++) {
            if (B->dice[i].f_78 == 3) {
                ease = fn_1_4EEA0(0, (f32) B->f_4 / 60.0f);
                B->dice[i].f_74 = scales[i] - (0.6f * scales[i]) * ease;
                ease = fn_1_4EEA0(0, (f32) B->f_4 / 60.0f);
                B->dice[i].f_70 = B->dice[i].f_70 - (10.0f + 5.5f * ease);
                Hu3DModelRotSet(B->dice[i].modelD, 0.0f, 0.0f, B->dice[i].f_70);
                Hu3DModelScaleSet(B->dice[i].modelD, B->dice[i].f_74, B->dice[i].f_74, B->dice[i].f_74);
                if (fn_1_4F1EC(i, &pos2D) != 0) {
                    all = 1;
                }
                tmpV = B->dice[i].posE;
                fn_1_4EDB8(&outV, &tmpV, pos2D.x, pos2D.y);
                Hu3DData[B->dice[i].modelD].pos = outV;
            }
        }
        if (all == 1) {
            XB_W32(arg0, 0x184) = 0;
            if (B->dice[0].f_78 == 3) {
                XB_W32(arg0, 0x184)++;
                rec->f_44++;
            }
            if (B->dice[1].f_78 == 3) {
                XB_W32(arg0, 0x184)++;
                rec->f_44++;
            }
            rec->f_40 = rec->f_44 + (rec->f_48 * 2 + -rec->f_4C);
            fn_1_2FFAC(arg0);
            Hu3DMotionShiftSet(lbl_1_bss_468[XB_RECIDX(arg0)], lbl_1_bss_430[XB_RECIDX(arg0)][0], 0.0f, 10.0f, 0);
            if (GwPlayerConf[XB_RECIDX(arg0)].charNo == 0) {
                lbl_1_bss_20[1] = 0x1F6;
            }
            if (GwPlayerConf[XB_RECIDX(arg0)].charNo == 1) {
                lbl_1_bss_20[1] = 0x1E2;
            }
            if (GwPlayerConf[XB_RECIDX(arg0)].charNo == 2) {
                lbl_1_bss_20[1] = 0x25A;
            }
            if (GwPlayerConf[XB_RECIDX(arg0)].charNo == 3) {
                lbl_1_bss_20[1] = 0x2AA;
            }
            if (GwPlayerConf[XB_RECIDX(arg0)].charNo == 4) {
                lbl_1_bss_20[1] = 0x282;
            }
            if (GwPlayerConf[XB_RECIDX(arg0)].charNo == 5) {
                lbl_1_bss_20[1] = 0x1BA;
            }
            if (GwPlayerConf[XB_RECIDX(arg0)].charNo == 6) {
                lbl_1_bss_20[1] = 0x296;
            }
            fn_1_8D04(lbl_1_bss_20[1], lbl_1_bss_468[XB_RECIDX(arg0)], 1);
            lbl_1_bss_20[1] = 0;
            for (i = 0; i < 2; i++) {
                Hu3DModelAttrSet(B->dice[i].modelD, 1);
            }
            B->state = 16;
            HuAudFXPlay(8);
            if (B->dice[0].f_78 == 3 && B->dice[1].f_78 == 3) {
                B->f_4 = 0;
            }
            lbl_1_bss_20[0] = 0;
        }
        if (B->f_4 == 0x3A) {
            omVibrate((s16) XB_RECIDX(arg0), 0xF, 0xC, 0);
        }
        break;
    case 10:
        if (Hu3DMotionEndCheck(lbl_1_bss_468[XB_RECIDX(arg0)]) == 0) {
            break;
        }
        B->state = 0x14;
        break;
    case 11:
        B->f_4++;
        if ((f32) B->f_4 == 12.0f / B->f_1A0) {
            Hu3DModelAttrReset(B->dice[0].model, 1);
        }
        if ((f32) B->f_4 < 60.0f / B->f_1A0) {
            break;
        }
        HuAudFXPlay(0x413);
        fn_1_4FBC8(arg0, 0x3E);
        Hu3D3Dto2D(&B->pos194, 1, &pos2D);
        target.x = 80.0f;
        target.y = 380.0f;
        target.z = 0.0f;
        fn_1_4F144(0, &pos2D, &target, 0x3C, 200.0f, 0x355);
        B->state++;
        fn_1_8D04(0x446, B->dice[0].model, 1);
        break;
    case 12:
        fn_1_8D04(0x8446, B->dice[0].model, 1);
        if (fn_1_4F1EC(0, &pos2D) != 0) {
            if (GwPlayerConf[XB_RECIDX(arg0)].type == 0) {
                fn_1_502E0(0x18);
            }
            B->state = 2;
            HuAudFXPlay(0x35C);
            lbl_1_bss_1C = fn_1_8D04(0x2F7, B->dice[0].model, 1);
        }
        tmpV = B->pos194;
        fn_1_4EDB8(&outV, &tmpV, pos2D.x, pos2D.y);
        Hu3DData[B->dice[0].model].pos = outV;
        B->dice[0].force.y += 24.0f;
        Hu3DMotionForceSet(B->dice[0].model, lbl_1_data_888, 0x100, B->dice[0].force.y);
        break;
    case 13:
        B->f_4++;
        if ((f32) B->f_4 < 30.0f / B->f_1A0) {
            break;
        }
        from.x = 286.0f;
        from.y = 238.0f;
        from.z = 0.0f;
        target.x = 210.0f;
        target.y = 90.0f;
        target.z = 0.0f;
        fn_1_4F144(0, &from, &target, 0x3C, 100.0f, 0x355);
        B->f_4 = 0;
        B->state++;
        break;
    case 14:
        B->f_4++;
        if (B->f_4 == 0x30) {
            tmpV.x = 210.0f;
            tmpV.y = 90.0f;
            tmpV.z = 700.0f;
            Hu3D2Dto3D(&tmpV, 1, &tmpV);
            fn_1_3ED3C(1, &tmpV);
            fn_1_3F380(1, 0, 2);
            fn_1_3F658(1, 0, 7);
            scaleV.x = 0.6f;
            scaleV.y = 0.6f;
            scaleV.z = 0.6f;
            fn_1_3F6B0(1, 0, &scaleV);
        }
        if (fn_1_4F1EC(0, &pos2D) != 0) {
            XB_W32(arg0, 0x184) = B->dice[0].f_78 + XB_W32(arg0, 0x184);
            espDispOff(B->spr);
            fn_1_30B00(arg0, 1);
            B->f_4 = 0;
            B->state++;
        }
        espScaleSet(B->spr, HuCos(18.0f * (f32) B->f_4), 1.0f);
        espPosSet(B->spr, pos2D.x, pos2D.y);
        break;
    case 15:
        B->f_4++;
        if (B->f_4 < 0x1E) {
            break;
        }
        B->state = 0x14;
        break;
    case 16:
        if (B->dice[0].f_78 == 3 && B->dice[1].f_78 == 3) {
            B->f_4++;
            if (B->f_4 != 0x14) {
                break;
            }
            HuAudFXPlay(8);
        }
        XB_W32(lbl_1_bss_12A8, 8) = GwPlayerConf[XB_RECIDX(arg0)].charNo;
        fn_1_4FBC8(arg0, (XB_W32(arg0, 0x184) == 1) ? 0x40 : 0x41);
        B->state = 10;
        break;
    default:
        goto cleanup;
    }
    Hu3DCamera[1] = Hu3DCamera[0];
    return 0;

cleanup:
    XB_KillDice(B);
    if (B->f_188 != 0) {
        espKill(B->spr);
        espDrawNoSet(lbl_1_bss_F9E[0], 0);
        espDrawNoSet(lbl_1_bss_F9E[1], 0);
    }
    B->state = 0;
    Hu3DLayerHookSet(0xF, NULL);
    return 1;
}

/* ---- chunk C ---- */
// carddll/status.c - chunk C: fn_1_3D780 .. fn_1_3FDF0 (dice roll, status effect model slots, effect camera scene).
#include <math.h>

/* ---- externs owned by other carddll units ---- */

/* ---- view of one model slot of lbl_1_data_8D8[n].f_C (0x18 bytes) ---- */
typedef struct {
    s16 model;      /* 0x00 HU3D model id */
    s16 light;      /* 0x02 local light id, -1 = none */
    s32 active;     /* 0x04 slot in use */
    s32 timer;      /* 0x08 frames since fn_1_3ED3C */
    Vec pos;        /* 0x0C */
} XC_Slot;

#define XC_SLOT(kind, idx) (&((XC_Slot *) lbl_1_data_8D8[(kind)].f_C)[(idx)])

/* ---- view of the dice work object passed to fn_1_3D780 ---- */
typedef struct {
    s16 model;      /* 0x00 */
    u8 pad_02[0xA];
    s32 state;      /* 0x0C */
    s32 count;      /* 0x10 */
    s32 limit;      /* 0x14 */
    Vec start;      /* 0x18 */
    Vec end;        /* 0x24 */
    u8 pad_30[0x1C];
    s32 turn[3];    /* 0x4C extra full turns per axis */
    f32 rot[3];     /* 0x58 */
    u8 pad_64[0x14];
    s32 face;       /* 0x78 index into the face rotation table */
} XC_Dice;

/* ---- view of the card player work object (arg0 of the *Work functions) ---- */
typedef struct {
    u8 pad_00[0x10];
    s32 row;        /* 0x10 */
    s32 col;        /* 0x14 */
    f32 f18;        /* 0x18 */
    u8 pad_1C[0x28];
    s32 n44;        /* 0x44 */
    s32 n48;        /* 0x48 */
    s32 n4C;        /* 0x4C */
    s32 flags;      /* 0x50 */
    s16 pad_54;
    s16 slot;       /* 0x56 index into the lbl_1_data_8D8 slot arrays */
    u8 pad_58[4];
} XC_Rec;

typedef struct {
    XC_Rec rec[4];  /* 0x000 */
    s32 curPlayer;  /* 0x170 */
    s32 recIdx[4];  /* 0x174 */
} XC_Work;

#define XC_CUR(w) (&(w)->rec[(w)->recIdx[(w)->curPlayer]])

/* ---- view of lbl_1_bss_18DC (0x68 bytes): effect scene work ---- */
typedef struct {
    s16 model[3];   /* 0x00 */
    s16 plane[3];   /* 0x06 */
    s16 motion[6];  /* 0x0C */
    s16 light;      /* 0x18 */
    u8 pad_1A[2];
    Vec pos;        /* 0x1C */
    Vec pos2;       /* 0x28 */
    Vec camPos;     /* 0x34 saved camera position */
    u8 pad_40[0xC];
    s32 state;      /* 0x4C */
    u8 pad_50[4];
    s32 count;      /* 0x54 */
    s32 total;      /* 0x58 */
    s32 f5C;        /* 0x5C */
    s32 f60;        /* 0x60 */
    f32 dist;       /* 0x64 */
} XC_Fx;

#define XC_FX ((XC_Fx *) lbl_1_bss_18DC)

/* lbl_1_bss_2034 +0x188 flag, lbl_1_bss_12A8 +0x28 counter */
typedef struct {
    u8 pad_00[0x188];
    s32 f188;
} XC_Eff2034;

typedef struct {
    u8 pad_00[0x28];
    s32 f28;
} XC_Fade;

/* Dice roll animation: state 0 = spin, 1 = settle onto the face, 2 = done. */
// NONMATCHING: not byte-compared against the original
s32 fn_1_3D780(void *arg0)
{
    XC_Dice *dice = (XC_Dice *) arg0;
    Vec faceRot[6] = {
        { 90.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 90.0f },
        { 0.0f, 0.0f, -90.0f },
        { 0.0f, 0.0f, 180.0f },
        { -90.0f, 0.0f, 0.0f }
    };
    Vec v;
    Vec tmp;
    f32 endTime = 84.0f;
    f32 t;
    f32 *face;

    switch (dice->state) {
    case 0:
        dice->rot[0] += 50.0f;
        dice->rot[1] += 30.0f;
        dice->rot[2] += 50.0f;
        Hu3DMotionForceSet(dice->model, lbl_1_data_888, 0x80, dice->rot[0]);
        Hu3DMotionForceSet(dice->model, lbl_1_data_888, 0x100, dice->rot[1]);
        Hu3DMotionForceSet(dice->model, lbl_1_data_888, 0x200, dice->rot[2]);
        dice->count++;
        if (dice->count >= dice->limit) {
            Hu3DMotionSpeedSet(dice->model, 1.0f);
            dice->state++;
        }
        return 0;
    case 1:
        t = Hu3DMotionTimeGet(dice->model);
        if (t > endTime) {
            t = endTime;
            dice->count = 0;
            dice->state++;
        }
        face = &faceRot[dice->face].x;
        if (t <= 47.0f) {
            tmp.x = tmp.y = tmp.z = 0.0f;
            tmp.x = dice->start.x + (0.6f * (t * (dice->end.x - dice->start.x))) / 47.0f;
            tmp.y = dice->start.y + (0.6f * (t * (dice->end.y - dice->start.y))) / 47.0f;
            tmp.z = dice->start.z + (0.6f * (t * (dice->end.z - dice->start.z))) / 47.0f;
            v = tmp;
            dice->rot[0] = -(0.7f * (t * (360.0f * (f32) dice->turn[0] + face[0]))) / 47.0f;
            dice->rot[1] = (0.7f * (t * (360.0f * (f32) dice->turn[1] + face[1]))) / 47.0f;
            dice->rot[2] = (0.7f * (t * (360.0f * (f32) dice->turn[2] + face[2]))) / 47.0f;
        } else if (t <= 67.0f) {
            tmp.x = tmp.y = tmp.z = 0.0f;
            tmp.x = dice->start.x + 0.6f * (dice->end.x - dice->start.x);
            tmp.y = dice->start.y + 0.6f * (dice->end.y - dice->start.y);
            tmp.z = dice->start.z + 0.6f * (dice->end.z - dice->start.z);
            v = tmp;
            v.x += (0.25f * ((t - 47.0f) * (dice->end.x - dice->start.x))) / 20.0f;
            v.y += (0.25f * ((t - 47.0f) * (dice->end.y - dice->start.y))) / 20.0f;
            v.z += (0.25f * ((t - 47.0f) * (dice->end.z - dice->start.z))) / 20.0f;
            dice->rot[0] = -(0.7f * (360.0f * (f32) dice->turn[0] + face[0]));
            dice->rot[1] = 0.7f * (360.0f * (f32) dice->turn[1] + face[1]);
            dice->rot[2] = 0.7f * (360.0f * (f32) dice->turn[2] + face[2]);
            dice->rot[0] = dice->rot[0] - (0.2f * ((t - 47.0f) * (360.0f * (f32) dice->turn[0] + face[0]))) / 20.0f;
            dice->rot[1] = dice->rot[1] + (0.2f * ((t - 47.0f) * (360.0f * (f32) dice->turn[1] + face[1]))) / 20.0f;
            dice->rot[2] = dice->rot[2] + (0.2f * ((t - 47.0f) * (360.0f * (f32) dice->turn[2] + face[2]))) / 20.0f;
        } else if (t <= 77.0f) {
            tmp.x = tmp.y = tmp.z = 0.0f;
            tmp.x = dice->start.x + 0.85f * (dice->end.x - dice->start.x);
            tmp.y = dice->start.y + 0.85f * (dice->end.y - dice->start.y);
            tmp.z = dice->start.z + 0.85f * (dice->end.z - dice->start.z);
            v = tmp;
            v.x += (0.1f * ((t - 67.0f) * (dice->end.x - dice->start.x))) / 10.0f;
            v.y += (0.1f * ((t - 67.0f) * (dice->end.y - dice->start.y))) / 10.0f;
            v.z += (0.1f * ((t - 67.0f) * (dice->end.z - dice->start.z))) / 10.0f;
            dice->rot[0] = -(0.9f * (360.0f * (f32) dice->turn[0] + face[0]));
            dice->rot[1] = 0.9f * (360.0f * (f32) dice->turn[1] + face[1]);
            dice->rot[2] = 0.9f * (360.0f * (f32) dice->turn[2] + face[2]);
            dice->rot[0] = dice->rot[0] - (0.075f * ((t - 67.0f) * (360.0f * (f32) dice->turn[0] + face[0]))) / 10.0f;
            dice->rot[1] = dice->rot[1] + (0.075f * ((t - 67.0f) * (360.0f * (f32) dice->turn[1] + face[1]))) / 10.0f;
            dice->rot[2] = dice->rot[2] + (0.075f * ((t - 67.0f) * (360.0f * (f32) dice->turn[2] + face[2]))) / 10.0f;
        } else {
            tmp.x = tmp.y = tmp.z = 0.0f;
            tmp.x = dice->start.x + 0.95000005f * (dice->end.x - dice->start.x);
            tmp.y = dice->start.y + 0.95000005f * (dice->end.y - dice->start.y);
            tmp.z = dice->start.z + 0.95000005f * (dice->end.z - dice->start.z);
            v = tmp;
            v.x += (0.049999952f * ((t - 77.0f) * (dice->end.x - dice->start.x))) / (endTime - 77.0f);
            v.y += (0.049999952f * ((t - 77.0f) * (dice->end.y - dice->start.y))) / (endTime - 77.0f);
            v.z += (0.049999952f * ((t - 77.0f) * (dice->end.z - dice->start.z))) / (endTime - 77.0f);
            dice->rot[0] = -(0.97499996f * (360.0f * (f32) dice->turn[0] + face[0]));
            dice->rot[1] = 0.97499996f * (360.0f * (f32) dice->turn[1] + face[1]);
            dice->rot[2] = 0.97499996f * (360.0f * (f32) dice->turn[2] + face[2]);
            dice->rot[0] = dice->rot[0] - (0.025000036f * ((t - 77.0f) * (360.0f * (f32) dice->turn[0] + face[0]))) / (endTime - 77.0f);
            dice->rot[1] = dice->rot[1] + (0.025000036f * ((t - 77.0f) * (360.0f * (f32) dice->turn[1] + face[1]))) / (endTime - 77.0f);
            dice->rot[2] = dice->rot[2] + (0.025000036f * ((t - 77.0f) * (360.0f * (f32) dice->turn[2] + face[2]))) / (endTime - 77.0f);
        }
        Hu3DModelPosSetV(dice->model, &v);
        Hu3DMotionForceSet(dice->model, lbl_1_data_888, 0x80, dice->rot[0]);
        Hu3DMotionForceSet(dice->model, lbl_1_data_888, 0x100, dice->rot[1]);
        Hu3DMotionForceSet(dice->model, lbl_1_data_888, 0x200, dice->rot[2]);
        if (t == 47.0f || t == 67.0f || t == 77.0f) {
            if (fn_1_8E88() & 0x2000) {
                fn_1_8D04(0x42E, dice->model, 1);
            } else if (lbl_1_bss_20[9] == 1) {
                fn_1_8D04(0x444, dice->model, 1);
            } else {
                fn_1_8D04(0x44B, dice->model, 1);
            }
        }
        return 0;
    case 2:
        return 1;
    default:
        return 0;
    }
}

// NONMATCHING: not byte-compared against the original
s32 fn_1_3EA44(CardGame *arg0)
{
    ((XC_Eff2034 *) lbl_1_bss_2034)->f188 = 1;
    if (fn_1_393C4(arg0) == 0) {
        return 0;
    }
    ((XC_Eff2034 *) lbl_1_bss_2034)->f188 = 0;
    return 1;
}

/* Creates the model of every slot of all 26 status effect kinds (hidden). */
// NONMATCHING: not byte-compared against the original
void fn_1_3EAA0(void)
{
    s32 i;
    s32 j;
    XC_Slot *slot;

    fn_1_4E5A4();
    Hu3DLayerHookSet(0xC, (HU3DLAYERHOOK) fn_1_4E7A8);
    for (i = 0; i < 0x1A; i++) {
        for (j = 0; j < lbl_1_data_8D8[i].f_0; j++) {
            slot = &XC_SLOT(i, 0)[j];
            memset(slot, 0, 0x18);
            slot->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_344[i], 0x10000000, 2));
            Hu3DModelPosSet(slot->model, -900.0f + 500.0f * (f32) (i % 6), 1000.0f - 500.0f * (f32) (i / 6), 0.0f);
            Hu3DModelRotSet(slot->model, 0.0f, 0.0f, 0.0f);
            Hu3DModelAttrSet(slot->model, 1);
            Hu3DModelCameraSet(slot->model, 1);
            Hu3DModelLayerSet(slot->model, 4);
            slot->light = -1;
        }
    }
}

/* Starts a free slot of effect kind arg0 at *arg1; returns the slot index or -1. */
// NONMATCHING: not byte-compared against the original
s16 fn_1_3ED3C(s32 arg0, Vec *arg1)
{
    s32 i;
    XC_Slot *slot;

    for (i = 0; i < lbl_1_data_8D8[arg0].f_0; i++) {
        slot = &XC_SLOT(arg0, 0)[i];
        if (slot->active != 0) {
            continue;
        }
        slot->active = 1;
        slot->timer = 0;
        Hu3DModelRotSet(slot->model, 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(slot->model, 1);
        Hu3DModelLayerSet(slot->model, 4);
        Hu3DModelScaleSet(slot->model, 1.0f, 1.0f, 1.0f);
        if (arg0 == 0x12) {
            Hu3DModelTPLvlSet(slot->model, 0.2f);
            arg1->y -= 12.0f;
        }
        if (arg0 == 4) {
            Hu3DModelAttrSet(slot->model, 0x40000001);
            arg1->y += 150.0f;
        }
        if (arg0 == 3) {
            Hu3DModelAttrSet(slot->model, 0x40000040);
            arg1->y += 100.0f;
        }
        slot->pos = *arg1;
        Hu3DModelPosSetV(slot->model, &slot->pos);
        Hu3DModelAttrReset(slot->model, 1);
        if (lbl_1_data_8D8[arg0].f_4 == 1) {
            Hu3DMotionTimeSet(slot->model, 0.0f);
        } else if (lbl_1_data_8D8[arg0].f_4 == 0) {
            Hu3DMotionShapeTimeSet(slot->model, 0.0f);
        }
        if (arg0 == 3 || arg0 == 8 || arg0 == 9 || arg0 == 0xB || arg0 == 0xC || arg0 == 0xD || arg0 == 0xE) {
            slot->light = Hu3DLLightCreate(slot->model, Hu3DCamera[0].pos.x, Hu3DCamera[0].pos.y, Hu3DCamera[0].pos.z,
                                           0.0f, 0.0f, -1.0f, 0xFF, 0xFF, 0xFF);
            Hu3DLLightInfinitytSet(slot->model, slot->light);
        }
        if (arg0 == 1 || arg0 == 2) {
            if (fn_1_8E88() & 0x4000) {
                fn_1_8D04(0x42C, slot->model, 1);
            } else {
                fn_1_8D04(0x319, slot->model, 1);
            }
        }
        return i;
    }
    return -1;
}

/* Releases slot arg1 of effect kind arg0: hide the model and kill its light. */
// NONMATCHING: not byte-compared against the original
void fn_1_3F278(s32 arg0, s16 arg1)
{
    XC_Slot *slot = &XC_SLOT(arg0, 0)[arg1];

    slot->active = 0;
    Hu3DModelAttrSet(slot->model, 1);
    if (slot->light != -1) {
        Hu3DLLightKill(slot->model, slot->light);
        slot->light = -1;
    }
}

// NONMATCHING: not byte-compared against the original
void fn_1_3F380(s32 arg0, s32 arg1, s32 arg2)
{
    Hu3DModelCameraSet(XC_SLOT(arg0, 0)[arg1].model, (u16) arg2);
}

/* Moves slot arg1 of kind arg0 to *arg2 (applies the per-kind height offset to *arg2 first). */
// NONMATCHING: not byte-compared against the original
void fn_1_3F3D8(s32 arg0, s16 arg1, Vec *arg2)
{
    if (arg0 == 0x12) {
        arg2->y -= 12.0f;
    }
    if (arg0 == 4) {
        arg2->y += 150.0f;
    }
    if (arg0 == 3) {
        arg2->y += 100.0f;
    }
    Hu3DModelPosSetV(XC_SLOT(arg0, 0)[arg1].model, arg2);
}

// NONMATCHING: not byte-compared against the original
void fn_1_3F498(s32 arg0, s32 arg1, Vec *arg2)
{
    Hu3DModelRotSetV(XC_SLOT(arg0, 0)[arg1].model, arg2);
}

// NONMATCHING: not byte-compared against the original
void fn_1_3F4EC(s32 arg0, s32 arg1, s32 arg2)
{
    Hu3DModelAttrSet(XC_SLOT(arg0, 0)[arg1].model, arg2);
}

// NONMATCHING: not byte-compared against the original
void fn_1_3F540(s32 arg0, s16 arg1, s32 arg2)
{
    Hu3DModelAttrReset(XC_SLOT(arg0, 0)[arg1].model, arg2);
}

/* Sets the playback speed of the slot's motion (f_4 == 1) or shape motion (f_4 == 0). */
// NONMATCHING: not byte-compared against the original
void fn_1_3F594(s32 arg0, s32 arg1, f32 arg2)
{
    if (lbl_1_data_8D8[arg0].f_4 == 1) {
        Hu3DMotionSpeedSet(XC_SLOT(arg0, 0)[arg1].model, arg2);
        return;
    }
    if (lbl_1_data_8D8[arg0].f_4 == 0) {
        Hu3DMotionShapeSpeedSet(XC_SLOT(arg0, 0)[arg1].model, arg2);
    }
}

// NONMATCHING: not byte-compared against the original
void fn_1_3F658(s32 arg0, s32 arg1, s32 arg2)
{
    Hu3DModelLayerSet(XC_SLOT(arg0, 0)[arg1].model, (s16) arg2);
}

// NONMATCHING: not byte-compared against the original
void fn_1_3F6B0(s32 arg0, s16 arg1, Vec *arg2)
{
    Hu3DModelScaleSet(XC_SLOT(arg0, 0)[arg1].model, arg2->x, arg2->y, arg2->z);
}

/* Returns 1 when slot arg1 of kind arg0 is free. */
// NONMATCHING: not byte-compared against the original
s32 fn_1_3F714(s32 arg0, s32 arg1)
{
    return XC_SLOT(arg0, 0)[arg1].active ^ 1;
}

/* Per-frame update of all active effect slots: billboard some kinds towards the camera,
   release a slot when its (shape) motion ended and the kind is flagged one-shot (f_8 == 1). */
// NONMATCHING: not byte-compared against the original
void fn_1_3F73C(void)
{
    s32 i;
    s32 j;
    XC_Slot *slot;
    Vec dir;
    Mtx m;
    f32 ang;
    f32 cur;
    f32 maxTime;

    for (i = 0; i < 0x1A; i++) {
        for (j = 0; j < lbl_1_data_8D8[i].f_0; j++) {
            slot = &XC_SLOT(i, 0)[j];
            if (slot->active == 0) {
                continue;
            }
            slot->timer++;
            if (slot->timer < 3) {
                continue;
            }
            if (i == 3 || i == 4 || i == 0x13 || i == 0x14 || i == 0x15) {
                PSVECSubtract(&Hu3DCamera[0].pos, &Hu3DCamera[0].target, &dir);
                PSVECNormalize(&dir, &dir);
                ang = (f32) (180.0 * (atan2(dir.x, dir.z) / 3.141592653589793));
                Hu3DData[slot->model].rot.y = ang;
                PSMTXIdentity(m);
                PSMTXRotRad(m, 'y', 0.017453292f * -ang);
                PSMTXMultVec(m, &dir, &dir);
                ang = (f32) -(180.0 * (atan2(dir.y, dir.z) / 3.141592653589793));
                if (ang < -45.0f) {
                    ang = -45.0f;
                }
                Hu3DData[slot->model].rot.x = ang;
            }
            if (lbl_1_data_8D8[i].f_4 == 1) {
                if (Hu3DMotionEndCheck(slot->model) != 0) {
                    slot->active = 0;
                }
            } else if (lbl_1_data_8D8[i].f_4 == 0) {
                cur = Hu3DData[slot->model].motShapeWork.time;
                maxTime = Hu3DMotionShapeMaxTimeGet(slot->model);
                if (cur >= maxTime) {
                    slot->active = 0;
                }
            }
            if (slot->active == 0 && lbl_1_data_8D8[i].f_8 == 1) {
                slot->active = 0;
                Hu3DModelAttrSet(slot->model, 1);
                if (slot->light != -1) {
                    Hu3DLLightKill(slot->model, slot->light);
                    slot->light = -1;
                }
            }
        }
    }
}

/* Sets the ambient colour of every effect model. */
// NONMATCHING: not byte-compared against the original
void fn_1_3FBB0(f32 arg0, f32 arg1, f32 arg2)
{
    s32 i;
    s32 j;

    for (i = 0; i < 0x1A; i++) {
        for (j = 0; j < lbl_1_data_8D8[i].f_0; j++) {
            Hu3DModelAmbSet(XC_SLOT(i, 0)[j].model, arg0, arg1, arg2);
        }
    }
}

/* Camera-scene dispatcher: lbl_1_bss_1168[2] selects the step; the camera 0 state is mirrored to camera 1. */
// NONMATCHING: not byte-compared against the original
s32 fn_1_3FC50(CardGame *arg0)
{
    switch (lbl_1_bss_1168[2]) {
    case 0:
        memset(lbl_1_bss_18DC, 0, 0x68);
        lbl_1_bss_1168[2]++;
        break;
    case 1:
        fn_1_3FDF0(arg0);
        break;
    case 2:
        fn_1_40F14(arg0);
        break;
    case 3:
        fn_1_414AC(arg0);
        break;
    case 4:
        fn_1_424A0(arg0);
        break;
    default:
        lbl_1_bss_1168[2] = 0;
        return 1;
    }
    Hu3DCamera[1] = Hu3DCamera[0];
    return 0;
}

// NONMATCHING: not byte-compared against the original
void fn_1_3FD5C(void)
{
    Hu3DModelAttrReset(XC_FX->model[1], 1);
    Hu3DModelPosSetV(XC_FX->model[1], &Hu3DData[XC_FX->model[0]].pos);
    Hu3DMotionTimeSet(XC_FX->model[1], 0.0f);
    fn_1_8D04(0x319, XC_FX->model[1], 2);
}

/* Effect scene step 1: set up models/camera (state 0), pull the camera in (1), zoom (2), swing the camera around (3). */
// NONMATCHING: not byte-compared against the original
void fn_1_3FDF0(void *arg0)
{
    XC_Work *work = (XC_Work *) arg0;
    XC_Rec *cur = XC_CUR(work);
    XC_Fx *fx = XC_FX;
    Vec a;
    Vec b;
    Vec c;
    Vec d;
    Vec e;
    Vec f;
    Vec g;
    Vec h;
    Vec dir;
    Vec toModel;
    Mtx m;
    f32 s;
    s32 i;

    switch (fx->state) {
    case 0:
        fx->f60 = 0;
        fx->f5C = -1;
        fn_1_9C74(&fx->pos, cur->row, cur->col, 1, 1);
        fx->pos.x += -50.0f;
        for (i = 0; i < 3; i++) {
            fx->plane[i] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[0x21 + i], 0x10000000, 2));
            Hu3DModelPosSet(fx->plane[i], 0.0f, 0.0f, 0.0f);
            Hu3DModelAttrSet(fx->plane[i], 1);
            Hu3DModelAttrSet(fx->plane[i], 0x400000);
            Hu3DModelLayerSet(fx->plane[i], 7);
            Hu3DModelCameraSet(fx->plane[i], 2);
            Hu3DModelScaleSet(fx->plane[i], 0.0625f, 0.0375f, 0.0625f);
        }
        for (i = 0; i < 3; i++) {
            fx->model[i] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_3AC[i], 0x10000000, 2));
            Hu3DModelPosSetV(fx->model[i], &fx->pos);
            Hu3DModelAttrSet(fx->model[i], 0x40000003);
            Hu3DModelAttrSet(fx->model[i], 1);
            Hu3DModelLayerSet(fx->model[i], 3);
            Hu3DModelCameraSet(fx->model[i], 1);
        }
        Hu3DModelPosSet(fx->model[0], fx->pos.x, fx->pos.y - 50.0f, fx->pos.z);
        Hu3DModelCameraSet(fx->model[2], 2);
        Hu3DModelScaleSet(fx->model[2], 1.0f, 1.0f, 1.0f);
        Hu3DModelMatHookSet(fx->model[2], (HU3DMATHOOK) fn_1_4E8E8);
        for (i = 0; i < 6; i++) {
            fx->motion[i] = Hu3DJointMotion(fx->model[0], HuDataSelHeapReadNum(lbl_1_data_3B8[i], 0x10000000, 2));
        }
        Hu3DMotionSet(fx->model[0], fx->motion[0]);
        Hu3DModelAttrReset(fx->model[2], 1);
        dir = Hu3DCamera[0].pos;
        dir.y -= 1400.0f;
        fn_1_317F0(&Hu3DCamera[0].pos, &dir, &Hu3DCamera[0].target, &Hu3DCamera[0].target);
        fx->light = Hu3DLLightCreate(fx->model[0], Hu3DCamera[0].pos.x, Hu3DCamera[0].pos.y, Hu3DCamera[0].pos.z,
                                     0.0f, 0.0f, -1.0f, 0x80, 0x80, 0x80);
        Hu3DLLightInfinitytSet(fx->model[0], fx->light);
        Hu3DLLightStaticSet(fx->model[0], fx->light, 1);
        Hu3DModelScaleSet(fx->model[1], 0.9f, 0.9f, 0.9f);
        Hu3DModelCameraSet(lbl_1_bss_468[work->recIdx[work->curPlayer]], 2);
        fx->state++;
        fn_1_8D04(0x449, lbl_1_bss_478[cur->row][cur->col], 1);
        break;
    case 1:
        fx->count += 2;
        if (fx->count > 0x5A) {
            fx->count = 0x5A;
        }
        a.x = 0.0f;
        a.y = 0.0f;
        a.z = 0.0f;
        a.z = 0.0015f * (f32) fx->count;
        b = a;
        c.x = 0.0f;
        c.y = 0.0f;
        c.z = 0.0f;
        c.z = 0.0015f * (f32) fx->count;
        d = c;
        fn_1_4E848(&d, &b);
        if (fn_1_318E4() != 0) {
            fx->count = 0x5A;
            Hu3DModelAttrReset(fx->model[0], 0x40000003);
            Hu3DModelAttrReset(fx->model[0], 1);
            Hu3DModelCameraSet(fx->model[0], 3);
            Hu3DMotionTimeSet(fx->model[0], 0.0f);
            fx->pos2 = fx->pos;
            fx->state++;
        }
        break;
    case 2:
        if (fx->count == 0x50) {
            fn_1_8D04(0x419, lbl_1_bss_478[cur->row][cur->col], 1);
        }
        fx->count--;
        if (Hu3DMotionEndCheck(fx->model[0]) == 0) {
            fx->pos2.y = 150.0f - 200.0f * fn_1_4EEA0(0, (f32) fx->count / 90.0f);
        }
        Hu3DModelPosSetV(fx->model[0], &fx->pos2);
        e.x = 0.0f;
        e.y = 0.0f;
        e.z = 0.0f;
        e.z = 0.0015f * (f32) fx->count;
        f = e;
        g.x = 0.0f;
        g.y = 0.0f;
        g.z = 0.0f;
        g.z = 0.0015f * (f32) fx->count;
        h = g;
        fn_1_4E848(&h, &f);
        if (fx->count == 0x3C) {
            Hu3DModelCameraSet(fx->model[0], 2);
        }
        if (fx->count <= 0) {
            Hu3DModelAttrSet(fx->model[2], 1);
            Hu3DModelCameraSet(fx->model[0], 1);
            fx->camPos = Hu3DCamera[0].pos;
            PSVECSubtract(&Hu3DData[fx->model[0]].pos, &Hu3DCamera[0].pos, &toModel);
            fx->dist = PSVECMag(&toModel);
            PSVECSubtract(&Hu3DCamera[0].target, &Hu3DCamera[0].pos, &dir);
            PSVECNormalize(&dir, &dir);
            PSVECScale(&dir, &dir, fx->dist);
            PSVECAdd(&dir, &Hu3DCamera[0].pos, &Hu3DCamera[0].target);
            fx->total = 0;
            for (i = 0; i < 4; i++) {
                fx->total = work->rec[i].n44 + (work->rec[i].n48 + work->rec[i].n4C + fx->total);
            }
            fx->total -= cur->n44 + (cur->n48 + cur->n4C);
            Hu3DMotionShiftSet(fx->model[0], fx->motion[5], 0.0f, 10.0f, 0x40000001);
            for (i = 0; i < 4; i++) {
                if ((work->rec[i].flags & 0x200) == 0) {
                    if ((work->rec[i].flags & 2) != 0) {
                        Hu3DModelAttrSet(XC_SLOT(3, 0)[work->rec[i].slot].model, 1);
                    }
                    if ((work->rec[i].flags & 4) != 0) {
                        Hu3DModelAttrSet(XC_SLOT(4, 0)[work->rec[i].slot].model, 1);
                    }
                }
            }
            if (cur->f18 != 180.0f) {
                if ((cur->flags & 2) != 0) {
                    Hu3DModelAttrReset(XC_SLOT(3, 0)[cur->slot].model, 1);
                }
                if ((cur->flags & 4) != 0) {
                    Hu3DModelAttrReset(XC_SLOT(4, 0)[cur->slot].model, 1);
                }
            }
            fx->count = 0;
            fx->state++;
        }
        break;
    case 3:
        s = 1.0f - (f32) fx->count / 60.0f;
        for (i = 0; i < 4; i++) {
            if ((work->rec[i].flags & 0x200) == 0) {
                Hu3DModelScaleSet(lbl_1_bss_468[i], s, s, s);
            }
        }
        if (cur->f18 != 180.0f) {
            Hu3DModelScaleSet(lbl_1_bss_468[work->recIdx[work->curPlayer]], 1.0f, 1.0f, 1.0f);
        }
        fx->count++;
        PSVECSubtract(&fx->camPos, &Hu3DCamera[0].target, &dir);
        PSMTXIdentity(m);
        PSMTXRotRad(m, 'x', 0.017453292f * (85.0f * fn_1_4EEA0(2, (f32) fx->count / 60.0f)));
        PSMTXMultVec(m, &dir, &dir);
        PSVECAdd(&dir, &Hu3DCamera[0].target, &Hu3DCamera[0].pos);
        if ((f32) fx->count >= 60.0f) {
            fn_1_8D04(0x272, lbl_1_bss_478[cur->row][cur->col], 1);
            fx->state = 0;
            ((XC_Fade *) lbl_1_bss_12A8)->f28 = 0x3C;
            fn_1_4FBC8(arg0, 0x12);
            if (fx->total == 0) {
                fx->state = 0;
                fx->count = 0;
                ((XC_Fade *) lbl_1_bss_12A8)->f28 = 0x3C;
                fn_1_4FBC8(arg0, 0x13);
                lbl_1_bss_1168[2] = 4;
            } else {
                lbl_1_bss_1168[2] = 2;
            }
        }
        Hu3DModelCameraSet(lbl_1_bss_468[work->recIdx[work->curPlayer]], 1);
        break;
    }
    Hu3DCamera[1] = Hu3DCamera[0];
}

/* ---- chunk D ---- */
/* ---- chunk D: fn_1_40F14, fn_1_4149C, fn_1_414AC, fn_1_424A0 (card exchange cutscene steps) ---- */

/* callees owned by other carddll units (signatures derived from the call sites) */

/* View of lbl_1_bss_18DC (0x68 bytes): state of the card exchange cutscene. */
typedef struct {
    /* 0x00 */ s16 model[3];      /* [0] main model, [1] effect model, [2] prop model */
    /* 0x06 */ s16 charModel[3];  /* per-kind models, indexed by idx */
    /* 0x0C */ s16 motion[6];
    /* 0x18 */ s16 lightA;        /* light killed by fn_1_424A0 */
    /* 0x1A */ s16 lightB;        /* light created/killed by fn_1_414AC */
    /* 0x1C */ Vec propPos;
    /* 0x28 */ Vec modelPos;
    /* 0x34 */ Vec camPos;
    /* 0x40 */ Vec camTarget;
    /* 0x4C */ s32 state;
    /* 0x50 */ s32 pad50;
    /* 0x54 */ s32 timer;
    /* 0x58 */ s32 idx;
    /* 0x5C */ s32 target;        /* record index of the other player, -1 = none */
    /* 0x60 */ s32 flag;
    /* 0x64 */ f32 camZOffset;
} XD_State;

#define XD_ST ((XD_State *)lbl_1_bss_18DC)
#define XD_12A8_28 (*(s32 *)(lbl_1_bss_12A8 + 0x28))

/* 0x5C-byte player record inside the work object (arg0). */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 row;
    /* 0x14 */ s32 col;
    /* 0x18 */ f32 f_18;
    /* 0x1C */ u8 pad1C[0x40 - 0x1C];
    /* 0x40 */ s32 cnt[4];        /* [0] = weighted total, [1..3] = per-kind counts */
    /* 0x50 */ u32 flags;
    /* 0x54 */ u8 pad54[2];
    /* 0x56 */ s16 slot;
    /* 0x58 */ u8 pad58[4];
} XD_Rec;

typedef struct {
    XD_Rec rec[4];
    s32 curPlayer;                /* 0x170 */
    s32 recIdx[4];                /* 0x174 */
} XD_Work;

/* s16 model id at offset 0 of slot <idx> of data_8D8[rec] */
#define XD_SLOT_MODEL(rec, idx) (*(s16 *)((u8 *)lbl_1_data_8D8[rec].f_C + (idx) * sizeof(StatusSlot)))

// NONMATCHING: not byte-compared against the original
void fn_1_40F14(void *arg0)
{
    XD_State *st = XD_ST;
    s32 i;
    s32 ret;
    s32 done;

    switch (st->state) {
    case 0:
        Hu3DMotionShiftSet(st->model[0], st->motion[1], 0.0f, 40.0f, 0);
        Hu3DModelCameraSet(st->model[0], 2);
        Hu3DZClearLayerSet(0xF);
        st->state++;
        break;
    case 1:
        if (st->motion[1] == Hu3DData[st->model[0]].motId) {
            if (Hu3DMotionTimeGet(st->model[0]) >= 115.0f) {
                Hu3DModelAttrReset(st->model[1], 0x40000003);
                Hu3DModelAttrReset(st->model[1], 1);
                Hu3DModelPosSetV(st->model[1], &Hu3DData[st->model[0]].pos);
                Hu3DMotionTimeSet(st->model[1], 0.0f);
                st->state++;
                fn_1_8D04(0x319, st->model[1], 2);
            }
        }
        break;
    case 2:
        if (Hu3DMotionTimeGet(st->model[0]) >= 118.0f) {
            Hu3DModelAttrSet(st->model[0], 1);
            Hu3DModelCameraSet(st->model[0], 1);
            Hu3DLayerHookSet(0xF, NULL);
        }
        if (Hu3DMotionTimeGet(st->model[1]) >= 12.0f) {
            st->timer = 0;
            st->state++;
            lbl_1_bss_BF4 = Hu3DData[st->model[0]].pos;
            lbl_1_bss_C00 = 0.38f;
            lbl_1_bss_BE8 = st->model[1];
            lbl_1_bss_1168[1] = 0;
        }
        break;
    case 3:
        ret = fn_1_227A4(0, arg0);
        if (ret != 0) {
            if (ret == 1) {
                st->flag = 1;
            }
            st->timer = 0;
            st->state++;
        }
        break;
    case 4:
        if (Hu3DMotionTimeGet(st->model[1]) >= 10.0f) {
            for (i = 0; i < 3; i++) {
                Hu3DModelAttrSet(lbl_1_bss_1050[i], 1);
            }
            Hu3DMotionSet(st->model[0], st->motion[2]);
            Hu3DModelAttrReset(st->model[0], 1);
            Hu3DMotionTimeSet(st->model[0], 0.0f);
            st->state++;
            fn_1_8D04(0x319, st->model[1], 2);
        }
        break;
    case 5:
        done = 0;
        if (Hu3DMotionEndCheck(st->model[0]) != 0) {
            done++;
        }
        if (Hu3DMotionEndCheck(st->model[1]) != 0) {
            Hu3DModelAttrSet(st->model[1], 1);
            done++;
        }
        if (done == 2) {
            st->state = 0;
            st->timer = 0;
            if (st->target != -1) {
                lbl_1_bss_1168[2] = 3;
            } else {
                Hu3DMotionShiftSet(st->model[0], st->motion[5], 0.0f, 10.0f, 0x40000001);
                if (st->flag == 0) {
                    XD_12A8_28 = 0x3C;
                    fn_1_4FBC8(arg0, 0x14);
                }
                lbl_1_bss_1168[2] = 4;
            }
        }
        break;
    }
}

// NONMATCHING: not byte-compared against the original
void fn_1_4149C(s32 arg0)
{
    XD_ST->target = arg0;
}

/* Card flies from the target player's panel to the current player's panel, counts are exchanged. */
// NONMATCHING: not byte-compared against the original
void fn_1_414AC(void *arg0)
{
    XD_Work *work = (XD_Work *)arg0;
    XD_State *st = XD_ST;
    s32 i;
    s32 cur;
    s32 tgt;
    s32 ch;
    f32 e;
    Vec src;
    Vec dst;
    Vec cur2d;
    Vec camPos;
    Vec out;

    switch (st->state) {
    case 0:
        st->flag = 1;
        Hu3DModelCameraSet(st->model[0], 2);
        Hu3DMotionShiftSet(st->model[0], st->motion[4], 0.0f, 10.0f, 0);
        st->timer = 0;
        for (i = 1; i < 4; i++) {
            if (work->rec[st->target].cnt[i] != 0) {
                st->idx = i - 1;
                break;
            }
        }
        st->lightB = Hu3DLLightCreate(st->charModel[st->idx],
            Hu3DCamera[0].pos.x, Hu3DCamera[0].pos.y, Hu3DCamera[0].pos.z,
            0.0f, 0.0f, -1.0f, 0xFF, 0xFF, 0xFF);
        Hu3DLLightInfinitytSet(st->charModel[st->idx], st->lightB);
        Hu3DLLightStaticSet(st->charModel[st->idx], st->lightB, 1);
        Hu3DModelAttrReset(st->charModel[st->idx], 1);
        Hu3DModelRotSet(st->charModel[st->idx], 90.0f, 0.0f, 0.0f);

        /* screen position of the target player's panel */
        i = 0;
        while (work->recIdx[i] != st->target) {
            i++;
        }
        src.x = 0.0f;
        src.y = 0.0f;
        src.z = 0.0f;
        src.x = lbl_1_data_528[i][0] - 7.0f;
        src.y = lbl_1_data_528[i][1] - 12.0f;

        /* screen position of the current player's panel */
        i = 0;
        while (work->recIdx[i] != work->recIdx[work->curPlayer]) {
            i++;
        }
        dst.x = 0.0f;
        dst.y = 0.0f;
        dst.z = 0.0f;
        dst.x = lbl_1_data_528[i][0] - 7.0f;
        dst.y = lbl_1_data_528[i][1] - 12.0f;

        fn_1_4F144(0, &src, &dst, 0x78, 420.0f, 0x155);
        st->timer = 0;
        st->state++;
        /* fall through */
    case 1:
        if (st->timer == 4) {
            fn_1_8D04(0x456, st->charModel[st->idx], 1);
        } else {
            fn_1_8D04(0x8456, st->charModel[st->idx], 1);
        }
        if (fn_1_4F1EC(0, &cur2d) != 0) {
            Hu3DModelAttrSet(st->charModel[st->idx], 1);
            tgt = st->target;
            cur = work->recIdx[work->curPlayer];
            work->rec[tgt].cnt[st->idx + 1]--;
            work->rec[cur].cnt[st->idx + 1]++;
            work->rec[tgt].cnt[0] = work->rec[tgt].cnt[1] + (work->rec[tgt].cnt[2] * 2 + work->rec[tgt].cnt[3] * -1);
            work->rec[cur].cnt[0] = work->rec[cur].cnt[1] + (work->rec[cur].cnt[2] * 2 + work->rec[cur].cnt[3] * -1);
            if (st->idx != 2) {
                Hu3DMotionShiftSet(lbl_1_bss_468[cur], lbl_1_bss_430[cur][0], 0.0f, 10.0f, 0);
                Hu3DMotionShiftSet(lbl_1_bss_468[tgt], lbl_1_bss_430[tgt][1], 0.0f, 10.0f, 0);
                ch = GwPlayerConf[cur].charNo;
                if (ch == 0) lbl_1_bss_20[1] = 0x1F6;
                if (ch == 1) lbl_1_bss_20[1] = 0x1E2;
                if (ch == 2) lbl_1_bss_20[1] = 0x25A;
                if (ch == 3) lbl_1_bss_20[1] = 0x2AA;
                if (ch == 4) lbl_1_bss_20[1] = 0x282;
                if (ch == 5) lbl_1_bss_20[1] = 0x1BA;
                if (ch == 6) lbl_1_bss_20[1] = 0x296;
                fn_1_8D04(lbl_1_bss_20[1], lbl_1_bss_468[cur], 1);
                lbl_1_bss_20[1] = 0;
            } else {
                Hu3DMotionShiftSet(lbl_1_bss_468[cur], lbl_1_bss_430[cur][1], 0.0f, 10.0f, 0);
                Hu3DMotionShiftSet(lbl_1_bss_468[tgt], lbl_1_bss_430[tgt][0], 0.0f, 10.0f, 0);
                ch = GwPlayerConf[cur].charNo;
                if (ch == 0) lbl_1_bss_20[1] = 0x200;
                if (ch == 1) lbl_1_bss_20[1] = 0x1EC;
                if (ch == 2) lbl_1_bss_20[1] = 0x264;
                if (ch == 3) lbl_1_bss_20[1] = 0x2B4;
                if (ch == 4) lbl_1_bss_20[1] = 0x28C;
                if (ch == 5) lbl_1_bss_20[1] = 0x1C4;
                if (ch == 6) lbl_1_bss_20[1] = 0x2A0;
                fn_1_8D04(lbl_1_bss_20[1], lbl_1_bss_468[cur], 1);
                lbl_1_bss_20[1] = 0;
            }
            fn_1_2FFAC(arg0);
            HuAudFXPlay(8);
            if (st->idx == 1) {
                st->timer = 0;
            }
            st->state++;
        }
        e = fn_1_4EEA0(5, (f32)st->timer / 120.0f);
        Hu3DData[st->charModel[st->idx]].rot.y += 30.0f - 26.0f * e;
        Hu3DModelScaleSet(st->charModel[st->idx],
            0.0625f * (1.0f + 3.0f * e), 0.0375f * (1.0f + 3.0f * e), 0.0625f * (1.0f + 3.0f * e));
        st->timer++;
        camPos = Hu3DCamera[0].pos;
        camPos.z -= st->camZOffset;
        fn_1_4EDB8(&out, &camPos, cur2d.x, cur2d.y);
        Hu3DData[st->charModel[st->idx]].pos = out;
        break;
    case 2:
        if (st->idx == 1) {
            st->timer++;
            if (st->timer == 0x14) {
                HuAudFXPlay(8);
            }
            if (st->timer < 0x14) {
                break;
            }
        }
        if (Hu3DMotionEndCheck(st->model[0]) != 0) {
            Hu3DMotionShiftSet(st->model[0], st->motion[5], 0.0f, 10.0f, 0x40000001);
            Hu3DLLightKill(st->charModel[st->idx], st->lightB);
            Hu3DModelCameraSet(st->model[0], 1);
            st->state = 0;
            st->timer = 0;
            lbl_1_bss_1168[2] = 4;
        }
        break;
    }
    Hu3DCamera[1] = Hu3DCamera[0];
}

// NONMATCHING: not byte-compared against the original
void fn_1_424A0(void *arg0)
{
    XD_Work *work = (XD_Work *)arg0;
    XD_State *st = XD_ST;
    XD_Rec *rec = &work->rec[work->recIdx[work->curPlayer]];
    s32 i;
    f32 t;
    Vec a;
    Vec b;

    switch (st->state) {
    case 0:
        if (st->flag == 1) {
            fn_1_8D04(0x272, lbl_1_bss_478[rec->row][rec->col], 1);
            XD_12A8_28 = 0x3C;
            fn_1_4FBC8(arg0, 0x15);
        }
        st->timer = 0;
        st->state++;
        st->modelPos = Hu3DData[st->model[0]].pos;
        st->camTarget = Hu3DCamera[0].target;
        st->camPos = Hu3DCamera[0].pos;
        Hu3DMotionShiftSet(st->model[0], st->motion[3], 0.0f, 25.0f, 0x40000001);
        Hu3DModelAttrReset(st->model[2], 1);
        Hu3DModelPosSet(st->model[2], st->propPos.x, st->propPos.y, st->propPos.z);
        Hu3DModelCameraSet(st->model[0], 3);
        fn_1_8D04(0x449, lbl_1_bss_478[rec->row][rec->col], 1);
        break;
    case 1:
        if (st->timer == 10) {
            fn_1_8D04(0x41A, lbl_1_bss_478[rec->row][rec->col], 1);
        }
        if (st->timer <= 0x5A) {
            Hu3DCamera[0].target.y = st->camTarget.y - 50.0f * fn_1_4EEA0(2, (f32)st->timer / 90.0f);
            Hu3DCamera[0].pos.y = st->camPos.y + 50.0f * fn_1_4EEA0(2, (f32)st->timer / 90.0f);
        }
        Hu3DCamera[1] = Hu3DCamera[0];
        st->timer++;
        st->modelPos.y -= 1.5f;
        Hu3DModelPosSetV(st->model[0], &st->modelPos);
        a.x = 0.0f;
        a.y = 0.0f;
        a.z = 0.001f * (f32)st->timer;
        b.x = 0.0f;
        b.y = 0.0f;
        b.z = 0.001f * (f32)st->timer;
        fn_1_4E848(&b, &a);
        if (st->timer >= 0x78) {
            st->timer = 0x78;
            st->state++;
        }
        break;
    case 2:
        st->timer--;
        a.x = 0.0f;
        a.y = 0.0f;
        a.z = 0.001f * (f32)st->timer;
        b.x = 0.0f;
        b.y = 0.0f;
        b.z = 0.001f * (f32)st->timer;
        fn_1_4E848(&b, &a);
        if (st->timer <= 0) {
            Hu3DModelAttrSet(st->model[2], 1);
            Hu3DModelAttrSet(st->model[0], 1);
            st->timer = 0;
            Hu3DLLightKill(st->model[0], st->lightA);
            Hu3DMotionSet(st->model[0], -1);
            for (i = 0; i < 6; i++) {
                Hu3DMotionKill(st->motion[i]);
            }
            for (i = 0; i < 3; i++) {
                Hu3DModelKill(st->charModel[i]);
            }
            for (i = 0; i < 3; i++) {
                Hu3DModelKill(st->model[i]);
            }
            st->state++;
        }
        break;
    case 3:
        st->timer++;
        t = (f32)st->timer / 30.0f;
        for (i = 0; i < 4; i++) {
            if ((work->rec[i].flags & 0x200) == 0) {
                Hu3DModelScaleSet(lbl_1_bss_468[i], t, t, t);
            }
        }
        if (180.0f != rec->f_18) {
            Hu3DModelScaleSet(lbl_1_bss_468[work->recIdx[work->curPlayer]], 1.0f, 1.0f, 1.0f);
        }
        if (st->timer >= 0x1E) {
            for (i = 0; i < 4; i++) {
                if ((work->rec[i].flags & 0x200) == 0) {
                    if ((work->rec[i].flags & 2) != 0) {
                        Hu3DModelAttrReset(XD_SLOT_MODEL(3, work->rec[i].slot), 1);
                    }
                    if ((work->rec[i].flags & 4) != 0) {
                        Hu3DModelAttrReset(XD_SLOT_MODEL(4, work->rec[i].slot), 1);
                    }
                    Hu3DModelScaleSet(lbl_1_bss_468[i], 1.0f, 1.0f, 1.0f);
                }
            }
            fn_1_317F0(&Hu3DCamera[0].pos, &Hu3DCamera[0].pos, &Hu3DCamera[0].target, &Hu3DCamera[0].target);
            st->state = 0;
            lbl_1_bss_1168[2]++;
        }
        break;
    }
}

/* ---- chunk E ---- */
/* ---- chunk E: fn_1_42EDC (card exchange / coin steal scene step machine) ---- */

/* callees owned by other carddll units (signatures derived from the call sites) */

/* view of one 0x5C-byte record of the player work (arg0) */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 row;
    /* 0x14 */ s32 col;
    /* 0x18 */ f32 rot;            /* board facing in degrees: 0 / 90 / 180 / 270 */
    /* 0x1C */ u8 pad1[0x2C - 0x1C];
    /* 0x2C */ s32 f2C;
    /* 0x30 */ u8 pad2[0x40 - 0x30];
    /* 0x40 */ s32 f40;            /* cnt[0] + 2 * cnt[1] - cnt[2] */
    /* 0x44 */ s32 cnt[3];
    /* 0x50 */ s32 flags;          /* 0x200: absent, 2 / 4: slot groups 3 / 4 in use */
    /* 0x54 */ u8 pad3[2];
    /* 0x56 */ s16 slot;           /* index into the slot group arrays */
    /* 0x58 */ u8 pad4[4];
} XE_Rec;

/* view of the player work object arg0 */
typedef struct {
    /* 0x000 */ XE_Rec rec[4];
    /* 0x170 */ s32 cur;           /* current player number */
    /* 0x174 */ s32 recIdx[4];     /* record index per player number */
} XE_Work;

/* view of a slot of the data_8D8 slot groups: first halfword is a model id */
typedef struct {
    /* 0x00 */ s16 id;
    /* 0x02 */ u8 pad[0x16];
} XE_Slot;

/* view of lbl_1_bss_1848 (0x94 bytes): steal scene work */
typedef struct {
    /* 0x00 */ s16 modelA[3];      /* [0] actor, [1] actor 2, [2] prop with mat hook */
    /* 0x06 */ s16 modelB[3];      /* per outcome models */
    /* 0x0C */ s16 mot[4];
    /* 0x14 */ Vec pos;            /* scene origin (card position) */
    /* 0x20 */ Vec v20;
    /* 0x2C */ u8 pad2C[0x4C - 0x2C];
    /* 0x4C */ s32 timer;
    /* 0x50 */ f32 f50;
    /* 0x54 */ Vec rot;
    /* 0x60 */ Vec v60;
    /* 0x6C */ Vec v6c;
    /* 0x78 */ s16 light[3];
    /* 0x7E */ s16 pad7E;
    /* 0x80 */ Vec v80;
    /* 0x8C */ s32 f8C;            /* outcome 0 / 1 / 2, -1 = none */
    /* 0x90 */ s32 f90;            /* record index of the victim player */
} XE_Fx;
#define E_FX ((XE_Fx *) lbl_1_bss_1848)

/* view of lbl_1_bss_12A8 (0x54 bytes) */
typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 f08;
    /* 0x0C */ u8 pad1[0x28 - 0x0C];
    /* 0x28 */ s32 f28;
    /* 0x2C */ u8 pad2[0x54 - 0x2C];
} XE_12A8;
#define E_12A8 ((XE_12A8 *) lbl_1_bss_12A8)

#define E_CURMDL lbl_1_bss_478[rec->row][rec->col]

/* show (set != 0) or hide the slot models of a record */
static void E_SlotsAttr(XE_Rec *r, s32 set)
{
    if (r->flags & 2) {
        s16 m = ((XE_Slot *) lbl_1_data_8D8[3].f_C)[r->slot].id;
        if (set) {
            Hu3DModelAttrSet(m, 1);
        } else {
            Hu3DModelAttrReset(m, 1);
        }
    }
    if (r->flags & 4) {
        s16 m = ((XE_Slot *) lbl_1_data_8D8[4].f_C)[r->slot].id;
        if (set) {
            Hu3DModelAttrSet(m, 1);
        } else {
            Hu3DModelAttrReset(m, 1);
        }
    }
}

// NONMATCHING: not byte-compared against the original
s32 fn_1_42EDC(CardGame *arg0)
{
    XE_Work *w = (XE_Work *) arg0;
    XE_Rec *rec;
    XE_Rec *other;
    s32 curIdx;
    s32 result = 0;
    s32 i, k, sum, r;
    f32 scl;
    Mtx mtx;
    Vec va = { 0.0f, 1.0f, 0.0f };
    Vec vb, vc, vd, ve, vf, vg;

    curIdx = w->recIdx[w->cur];
    rec = &w->rec[curIdx];

    /* ---- step machine ---- */
    switch (lbl_1_bss_1168[1]) {
    case 0:
        E_FX->light[2] = -1;
        E_FX->light[1] = -1;
        E_FX->light[0] = -1;
        E_FX->f8C = -1;
        fn_1_9C74(&E_FX->pos, rec->row, rec->col, 1, 1);
        E_FX->pos.x += -50.0f;
        PSMTXIdentity(mtx);
        PSMTXRotRad(mtx, 'y', 0.017453292f * rec->rot);
        va.x = lbl_1_data_A78[0][0];
        va.y = lbl_1_data_A78[0][1];
        va.z = lbl_1_data_A78[0][2];
        vb.x = lbl_1_data_A78[0][3];
        vb.y = lbl_1_data_A78[0][4];
        vb.z = lbl_1_data_A78[0][5];
        PSMTXMultVec(mtx, &va, &va);
        PSVECAdd(&E_FX->pos, &va, &va);
        PSMTXMultVec(mtx, &vb, &vb);
        PSVECAdd(&E_FX->pos, &vb, &vb);
        fn_1_317F0(&Hu3DCamera[0].pos, &va, &Hu3DCamera[0].target, &vb);
        vc.x = 0.0f;
        vc.y = 0.0f;
        vc.z = 1.0f;
        va = vc;
        PSMTXMultVec(mtx, &va, &va);
        for (i = 0; i < 3; i++) {
            E_FX->modelB[i] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[0x21 + i], 0x10000000, 2));
            Hu3DModelPosSet(E_FX->modelB[i], 0.0f, 0.0f, 0.0f);
            Hu3DModelAttrSet(E_FX->modelB[i], 1);
            Hu3DModelLayerSet(E_FX->modelB[i], 3);
            Hu3DModelCameraSet(E_FX->modelB[i], 1);
            Hu3DModelScaleSet(E_FX->modelB[i], 0.5f, 0.3f, 0.5f);
        }
        for (i = 0; i < 3; i++) {
            E_FX->modelA[i] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_3D0[i], 0x10000000, 2));
            Hu3DModelPosSet(E_FX->modelA[i], 0.0f, 0.0f, 0.0f);
            Hu3DModelAttrSet(E_FX->modelA[i], 0x40000003);
            Hu3DModelAttrSet(E_FX->modelA[i], 1);
            Hu3DModelLayerSet(E_FX->modelA[i], 3);
            Hu3DModelCameraSet(E_FX->modelA[i], 1);
        }
        Hu3DModelCameraSet(E_FX->modelA[2], 2);
        Hu3DModelMatHookSet(E_FX->modelA[2], (HU3DMATHOOK) fn_1_4E8E8);
        for (i = 0; i < 4; i++) {
            E_FX->mot[i] = Hu3DJointMotion(E_FX->modelA[0], HuDataSelHeapReadNum(lbl_1_data_3DC[i], 0x10000000, 2));
        }
        Hu3DMotionSet(E_FX->modelA[0], E_FX->mot[0]);
        E_FX->light[0] = Hu3DLLightCreate(E_CURMDL, Hu3DCamera[0].pos.x, Hu3DCamera[0].pos.y, Hu3DCamera[0].pos.z,
                                          va.x, va.y, va.z, 255, 255, 255);
        Hu3DLLightInfinitytSet(E_CURMDL, E_FX->light[0]);
        Hu3DLLightStaticSet(E_CURMDL, E_FX->light[0], 1);
        E_FX->light[1] = Hu3DLLightCreate(E_FX->modelA[0], Hu3DCamera[0].pos.x, Hu3DCamera[0].pos.y, Hu3DCamera[0].pos.z,
                                          va.x, va.y, va.z, 255, 255, 255);
        Hu3DLLightInfinitytSet(E_FX->modelA[0], E_FX->light[1]);
        Hu3DLLightStaticSet(E_FX->modelA[0], E_FX->light[1], 1);
        Hu3DModelPosSetV(E_FX->modelA[0], &E_FX->pos);
        Hu3DModelPosSetV(E_FX->modelA[2], &E_FX->pos);
        Hu3DMotionSet(E_FX->modelA[0], E_FX->mot[0]);
        Hu3DModelCameraSet(lbl_1_bss_468[curIdx], 2);
        for (i = 0; i < 4; i++) {
            if (!(w->rec[i].flags & 0x200)) {
                E_SlotsAttr(&w->rec[i], 1);
            }
        }
        E_SlotsAttr(rec, 0);
        if (rec->rot == 0.0f) {
            E_FX->v20.x = Hu3DCamera[0].pos.x;
            E_FX->v20.y = Hu3DCamera[0].pos.y;
            E_FX->v20.z = Hu3DCamera[0].pos.z;
            E_FX->timer = 0;
            lbl_1_bss_1168[1] = 0x64;
        } else {
            lbl_1_bss_1168[1]++;
        }
        break;

    case 1:
        E_FX->timer++;
        scl = 1.0f - (f32) E_FX->timer / 60.0f;
        for (i = 0; i < 4; i++) {
            if (!(w->rec[i].flags & 0x200)) {
                Hu3DModelScaleSet(lbl_1_bss_468[i], scl, scl, scl);
            }
        }
        Hu3DModelScaleSet(lbl_1_bss_468[curIdx], 1.0f, 1.0f, 1.0f);
        if (fn_1_318E4() != 0) {
            E_FX->timer = 0;
            E_FX->rot.x = 0.0f;
            E_FX->rot.y = 0.0f;
            E_FX->rot.z = 0.0f;
            E_FX->v60 = E_FX->rot;
            lbl_1_bss_1168[1]++;
        }
        break;

    case 2:
        E_FX->v60.x = E_FX->pos.x;
        E_FX->v60.y = E_FX->pos.y;
        E_FX->v60.z = E_FX->pos.z;
        E_FX->timer++;
        if (rec->rot != 180.0f) {
            if (rec->rot == 0.0f) {
                E_FX->rot.y = 180.0f * fn_1_4EEA0(2, (f32) E_FX->timer / 30.0f);
            } else if (rec->rot == 90.0f) {
                E_FX->rot.y = -90.0f * fn_1_4EEA0(2, (f32) E_FX->timer / 30.0f);
            } else {
                E_FX->rot.y = 90.0f * fn_1_4EEA0(2, (f32) E_FX->timer / 30.0f);
            }
            E_FX->v60.y += 50.0f * fn_1_4EEA0(5, (f32) E_FX->timer / 30.0f);
            Hu3DModelPosSetV(E_CURMDL, &E_FX->v60);
            Hu3DModelRotSetV(E_CURMDL, &E_FX->rot);
            if (E_FX->timer < 30) {
                break;
            }
            vc = E_FX->pos;
            fn_1_3ED3C(0, &vc);
            fn_1_8D04(0x454, E_CURMDL, 1);
        }
        E_FX->timer = 0;
        lbl_1_bss_1168[1]++;
        break;

    case 3:
        E_FX->timer++;
        if (E_FX->timer >= 15) {
            E_FX->timer = 0;
            lbl_1_bss_1168[1]++;
            fn_1_8D04(0x41b, E_CURMDL, 1);
        }
        break;

    case 4:
        E_FX->timer++;
        E_FX->v60.x = 0.0f;
        E_FX->v60.y = 0.0f;
        E_FX->v60.z = 0.0f;
        E_FX->v60.y += 150.0f * fn_1_4EEA0(1, (f32) E_FX->timer / 30.0f);
        E_FX->v60.z += 140.0f * ((f32) E_FX->timer / 30.0f);
        if (rec->rot == 0.0f) {
            E_FX->rot.x += 3.0f;
        } else if (rec->rot == 90.0f) {
            E_FX->rot.z += 3.0f;
        } else if (rec->rot == 180.0f) {
            E_FX->rot.x += 3.0f;
        } else {
            E_FX->rot.z -= 3.0f;
        }
        PSMTXIdentity(mtx);
        PSMTXRotRad(mtx, 'y', 0.017453292f * rec->rot);
        PSMTXMultVec(mtx, &E_FX->v60, &E_FX->v60);
        PSVECAdd(&E_FX->v60, &E_FX->pos, &E_FX->v60);
        Hu3DModelRotSetV(E_CURMDL, &E_FX->rot);
        Hu3DModelPosSetV(E_CURMDL, &E_FX->v60);
        if (E_FX->timer >= 30) {
            E_FX->timer = 0;
            Hu3DModelAttrReset(E_FX->modelA[2], 1);
            Hu3DModelRotSetV(E_FX->modelA[2], &E_FX->rot);
            Hu3DModelPosSetV(E_FX->modelA[2], &E_FX->v60);
            Hu3DCamera[1] = Hu3DCamera[0];
            lbl_1_bss_1168[1]++;
            fn_1_8D04(0x449, E_CURMDL, 1);
        }
        break;

    case 5:
        E_FX->timer++;
        vc.x = 0.0f;
        vc.y = 0.0f;
        vc.z = 0.0015f * (f32) E_FX->timer;
        fn_1_4E848(&vc, &vc);
        if (E_FX->timer < 90) {
            break;
        }
        va.x = 0.0f;
        va.y = 0.0f;
        va.z = -50.0f;
        vd.x = 0.0f;
        vd.y = 180.0f + rec->rot;
        vd.z = 0.0f;
        PSMTXIdentity(mtx);
        PSMTXRotRad(mtx, 'y', 0.017453292f * (180.0f + rec->rot));
        PSMTXMultVec(mtx, &va, &va);
        PSVECAdd(&va, &E_FX->v60, &va);
        Hu3DModelPosSetV(E_FX->modelA[0], &va);
        Hu3DModelRotSetV(E_FX->modelA[0], &vd);
        Hu3DModelAttrReset(E_FX->modelA[0], 0x40000003);
        Hu3DModelAttrReset(E_FX->modelA[0], 1);
        Hu3DModelCameraSet(E_FX->modelA[0], 3);
        Hu3DMotionTimeSet(E_FX->modelA[0], 0.0f);
        va.x = 0.0f;
        va.y = 0.0f;
        va.z = 1.0f;
        PSMTXMultVec(mtx, &va, &va);
        E_FX->v6c = va;
        E_FX->v20 = E_FX->pos;
        E_FX->timer = 90;
        lbl_1_bss_1168[1]++;
        break;

    case 6:
        if (E_FX->timer == 90) {
            fn_1_8D04(0x419, E_CURMDL, 1);
        }
        PSVECScale(&E_FX->v6c, &va, 0.035f * (f32) E_FX->timer);
        PSVECAdd(&va, &Hu3DData[E_FX->modelA[0]].pos, &Hu3DData[E_FX->modelA[0]].pos);
        if (E_FX->timer == 60) {
            Hu3DMotionShiftSet(lbl_1_bss_468[curIdx], lbl_1_bss_430[curIdx][2], 0.0f, 10.0f, 0);
            if (GwPlayerConf[curIdx].charNo == 0) {
                lbl_1_bss_20[1] = 0x1FD;
            }
            if (GwPlayerConf[curIdx].charNo == 1) {
                lbl_1_bss_20[1] = 0x1E9;
            }
            if (GwPlayerConf[curIdx].charNo == 2) {
                lbl_1_bss_20[1] = 0x261;
            }
            if (GwPlayerConf[curIdx].charNo == 3) {
                lbl_1_bss_20[1] = 0x2B1;
            }
            if (GwPlayerConf[curIdx].charNo == 4) {
                lbl_1_bss_20[1] = 0x289;
            }
            if (GwPlayerConf[curIdx].charNo == 5) {
                lbl_1_bss_20[1] = 0x1C1;
            }
            if (GwPlayerConf[curIdx].charNo == 6) {
                lbl_1_bss_20[1] = 0x29D;
            }
            fn_1_8D04(lbl_1_bss_20[1], lbl_1_bss_468[curIdx], 1);
            lbl_1_bss_20[1] = 0;
        }
        E_FX->timer--;
        vc.x = 0.0f;
        vc.y = 0.0f;
        vc.z = 0.0015f * (f32) E_FX->timer;
        fn_1_4E848(&vc, &vc);
        if (E_FX->timer > 0) {
            break;
        }
        fn_1_8D04(0x272, E_CURMDL, 1);
        Hu3DMotionShiftSet(E_FX->modelA[0], E_FX->mot[3], 0.0f, 10.0f, 0x40000001);
        E_12A8->f28 = 60;
        fn_1_4FBC8(arg0, 0x16);
        E_FX->timer = 0;
        Hu3DModelAttrSet(E_FX->modelA[2], 1);
        Hu3DModelCameraSet(E_FX->modelA[0], 1);
        E_FX->v20 = Hu3DData[E_FX->modelA[0]].pos;
        if (rec->cnt[0] + rec->cnt[1] + rec->cnt[2] == 0) {
            E_12A8->f28 = 60;
            fn_1_4FBC8(arg0, 0x17);
            Hu3DMotionShiftSet(E_FX->modelA[0], E_FX->mot[0], 0.0f, 10.0f, 0);
            Hu3DModelAttrReset(E_FX->modelA[2], 1);
            lbl_1_bss_1168[1] = 0x11;
        } else {
            Hu3DMotionShiftSet(E_FX->modelA[0], E_FX->mot[1], 0.0f, 10.0f, 0);
            lbl_1_bss_1168[1]++;
        }
        break;

    case 7:
    case 8:
    case 9:
    case 10:
        E_FX->timer++;
        PSVECScale(&E_FX->v6c, &va, 5.0f);
        PSVECAdd(&va, &Hu3DData[E_FX->modelA[0]].pos, &Hu3DData[E_FX->modelA[0]].pos);
        Hu3DData[E_FX->modelA[0]].pos.y = E_FX->v20.y;
        Hu3DData[E_FX->modelA[0]].pos.y += 2.0f * (f32) E_FX->timer;
        Hu3DData[E_FX->modelA[0]].pos.y += 100.0f * fn_1_4EEA0(5, (f32) E_FX->timer / 30.0f);
        if (lbl_1_bss_1168[1] == 10) {
            Hu3DCamera[0].target = Hu3DData[E_FX->modelA[0]].pos;
        }
        if (E_FX->timer >= 30) {
            E_FX->timer = 0;
            lbl_1_bss_1168[1] += 4;
        }
        if (E_FX->timer == 28) {
            s32 vib = (lbl_1_bss_1168[1] < 10) ? 15 : 90;
            omVibrate((s16) curIdx, (s16) vib, 12, 0);
            HuAudFXPlay(0x453);
        }
        break;

    case 11:
    case 12:
    case 13:
    case 14:
        E_FX->timer++;
        if (E_FX->timer < 30) {
            break;
        }
        E_FX->timer = 0;
        if (lbl_1_bss_1168[1] < 14) {
            Hu3DData[E_FX->modelA[0]].pos = E_FX->v20;
            Hu3DMotionTimeSet(E_FX->modelA[0], 0.0f);
            PSMTXIdentity(mtx);
            PSMTXRotRad(mtx, 'y', 0.017453292f * rec->rot);
            va.x = lbl_1_data_A78[lbl_1_bss_1168[1] - 10][0];
            va.y = lbl_1_data_A78[lbl_1_bss_1168[1] - 10][1];
            va.z = lbl_1_data_A78[lbl_1_bss_1168[1] - 10][2];
            vb.x = lbl_1_data_A78[lbl_1_bss_1168[1] - 10][3];
            vb.y = lbl_1_data_A78[lbl_1_bss_1168[1] - 10][4];
            vb.z = lbl_1_data_A78[lbl_1_bss_1168[1] - 10][5];
            PSMTXMultVec(mtx, &va, &va);
            PSVECAdd(&E_FX->pos, &va, &Hu3DCamera[0].pos);
            PSMTXMultVec(mtx, &vb, &vb);
            PSVECAdd(&E_FX->pos, &vb, &Hu3DCamera[0].target);
            if (lbl_1_bss_1168[1] == 13) {
                Hu3DModelAttrSet(lbl_1_bss_468[curIdx], 1);
                E_SlotsAttr(rec, 1);
            }
            lbl_1_bss_1168[1] -= 3;
        } else {
            WipeCreate(2, 0, 60);
            WipeColorSet(0, 0, 0);
            lbl_1_bss_1168[1]++;
        }
        break;

    case 15:
        if (WipeCheckEnd() != 0) {
            break;
        }
        PSMTXIdentity(mtx);
        PSMTXRotRad(mtx, 'y', 0.017453292f * rec->rot);
        va.x = lbl_1_data_A78[0][0];
        va.y = lbl_1_data_A78[0][1];
        va.z = lbl_1_data_A78[0][2];
        vb.x = lbl_1_data_A78[0][3];
        vb.y = lbl_1_data_A78[0][4];
        vb.z = lbl_1_data_A78[0][5];
        PSMTXMultVec(mtx, &va, &va);
        PSVECAdd(&E_FX->pos, &va, &Hu3DCamera[0].pos);
        PSMTXMultVec(mtx, &vb, &vb);
        PSVECAdd(&E_FX->pos, &vb, &Hu3DCamera[0].target);
        Hu3DData[E_FX->modelA[0]].pos = E_FX->v20;
        Hu3DModelAttrReset(lbl_1_bss_468[curIdx], 1);
        E_SlotsAttr(rec, 0);
        Hu3DMotionSet(E_FX->modelA[0], E_FX->mot[2]);
        Hu3DModelAttrSet(E_FX->modelA[0], 0x40000002);
        Hu3DMotionTimeSet(E_FX->modelA[0], 0.0f);
        WipeCreate(1, 0, 60);
        WipeColorSet(0, 0, 0);
        lbl_1_bss_1168[1]++;
        break;

    case 16:
        if (WipeCheckEnd() != 0) {
            break;
        }
        sum = rec->cnt[1] + rec->cnt[2] + rec->cnt[0];
        Hu3DModelAttrReset(E_FX->modelA[0], 0x40000002);
        r = frandmod(sum);
        if (r < rec->cnt[0]) {
            E_FX->f8C = 0;
        } else if (r - rec->cnt[0] < rec->cnt[1]) {
            E_FX->f8C = 1;
        } else {
            E_FX->f8C = 2;
        }
        PSVECScale(&E_FX->v6c, &va, -1.0f);
        E_FX->light[2] = Hu3DLLightCreate(E_FX->modelB[E_FX->f8C], Hu3DCamera[0].pos.x, Hu3DCamera[0].pos.y,
                                          Hu3DCamera[0].pos.z, va.x, va.y, va.z, 255, 255, 255);
        Hu3DLLightInfinitytSet(E_FX->modelB[E_FX->f8C], E_FX->light[2]);
        Hu3DLLightStaticSet(E_FX->modelB[E_FX->f8C], E_FX->light[2], 1);
        lbl_1_bss_1168[1] = 0x13;
        break;

    case 17:
        if (E_FX->timer == 0) {
            fn_1_8D04(0x449, E_CURMDL, 1);
        }
        if (E_FX->timer == 10) {
            fn_1_8D04(0x41A, E_CURMDL, 1);
        }
        E_FX->timer++;
        vc.x = 0.0f;
        vc.y = 0.0f;
        vc.z = 0.002f * (f32) E_FX->timer;
        fn_1_4E848(&vc, &vc);
        PSVECScale(&E_FX->v6c, &va, -0.06f * (f32) E_FX->timer);
        PSVECAdd(&va, &Hu3DData[E_FX->modelA[0]].pos, &Hu3DData[E_FX->modelA[0]].pos);
        if (E_FX->timer <= 30) {
            Hu3DData[E_FX->modelA[0]].rot.y += 6.0f;
        }
        if (E_FX->timer >= 80) {
            Hu3DModelAttrSet(E_FX->modelA[0], 1);
            lbl_1_bss_1168[1]++;
        }
        break;

    case 18:
        E_FX->timer--;
        vc.x = 0.0f;
        vc.y = 0.0f;
        vc.z = 0.002f * (f32) E_FX->timer;
        fn_1_4E848(&vc, &vc);
        if (E_FX->timer > 0) {
            break;
        }
        Hu3DModelAttrSet(E_FX->modelA[2], 1);
        lbl_1_bss_1168[1] = 0x17;
        break;

    case 19:
        if (Hu3DMotionTimeGet(E_FX->modelA[0]) >= 20.0f) {
            Hu3DModelPosSetV(E_FX->modelA[1], &Hu3DData[E_FX->modelA[0]].pos);
            Hu3DModelRotSetV(E_FX->modelA[1], &Hu3DData[E_FX->modelA[0]].rot);
            Hu3DModelAttrReset(E_FX->modelA[1], 0x40000003);
            Hu3DModelAttrReset(E_FX->modelA[1], 1);
            Hu3DMotionTimeSet(E_FX->modelA[1], 0.0f);
            lbl_1_bss_1168[1]++;
            fn_1_8D04(0x319, E_FX->modelA[1], 2);
        }
        break;

    case 20:
        if (Hu3DMotionTimeGet(E_FX->modelA[1]) >= 5.0f) {
            Hu3DModelAttrSet(E_FX->modelA[0], 1);
        }
        if (Hu3DMotionTimeGet(E_FX->modelA[1]) >= 17.0f) {
            Hu3DModelPosSetV(E_FX->modelB[E_FX->f8C], &Hu3DData[E_FX->modelA[0]].pos);
            Hu3DModelRotSet(E_FX->modelB[E_FX->f8C], 90.0f, Hu3DData[E_FX->modelA[0]].rot.y, 0.0f);
            Hu3DModelAttrReset(E_FX->modelB[E_FX->f8C], 1);
            lbl_1_bss_1168[1]++;
        }
        break;

    case 21:
        if (Hu3DMotionEndCheck(E_FX->modelA[1]) == 0) {
            break;
        }
        /* pick a random player other than the current one */
        do {
            E_FX->f90 = frandmod(100) / 25;
        } while (E_FX->f90 == curIdx);
        E_12A8->f28 = 60;
        E_12A8->f08 = fn_1_502DC(w->rec[E_FX->f90].f2C);
        fn_1_4FBC8(arg0, 0x18);
        E_FX->v80 = Hu3DData[E_FX->modelB[E_FX->f8C]].pos;
        Hu3D3Dto2D(&Hu3DData[E_FX->modelB[E_FX->f8C]].pos, 1, &ve);
        for (k = 0; w->recIdx[k] != E_FX->f90; k++) {
        }
        vf.x = 0.0f;
        vf.y = 0.0f;
        vf.z = 0.0f;
        vf.x = lbl_1_data_528[k][0] - 7.0f;
        vf.y = lbl_1_data_528[k][1] - 12.0f;
        fn_1_4F144(0, &ve, &vf, 60, 0.0f, 0x21);
        Hu3DModelAttrSet(E_FX->modelA[1], 1);
        E_FX->f50 = 1.0f;
        E_FX->timer = 0;
        lbl_1_bss_1168[1]++;
        fn_1_8D04(0x353, E_FX->modelB[E_FX->f8C], 1);
        break;

    case 22:
        fn_1_8D04(0x8353, E_FX->modelB[E_FX->f8C], 1);
        E_FX->f50 = 1.0f - 0.85f * fn_1_4EEA0(1, (f32) E_FX->timer / 60.0f);
        Hu3DModelScaleSet(E_FX->modelB[E_FX->f8C], 0.5f * E_FX->f50, 0.3f * E_FX->f50, 0.5f * E_FX->f50);
        Hu3DData[E_FX->modelB[E_FX->f8C]].rot.y =
            Hu3DData[E_FX->modelB[E_FX->f8C]].rot.y - (10.0f + (f32) E_FX->timer * 0.5f);
        fn_1_4F1EC(0, &va);
        vc = E_FX->v80;
        fn_1_4EDB8(&vg, &vc, va.x, va.y);
        Hu3DData[E_FX->modelB[E_FX->f8C]].pos = vg;
        if (E_FX->timer == 35) {
            Hu3DModelLayerSet(E_FX->modelB[E_FX->f8C], 7);
            Hu3DModelCameraSet(E_FX->modelB[E_FX->f8C], 2);
        }
        E_FX->timer++;
        if (E_FX->timer <= 60) {
            break;
        }
        Hu3DModelAttrSet(E_FX->modelB[E_FX->f8C], 1);
        E_FX->timer = 0;
        rec->cnt[E_FX->f8C]--;
        rec->f40 = rec->cnt[0] + (rec->cnt[1] * 2 - rec->cnt[2]);
        other = &w->rec[E_FX->f90];
        other->cnt[E_FX->f8C]++;
        other->f40 = other->cnt[0] + (other->cnt[1] * 2 - other->cnt[2]);
        if (E_FX->f8C != 2) {
            Hu3DMotionShiftSet(lbl_1_bss_468[curIdx], lbl_1_bss_430[curIdx][1], 0.0f, 10.0f, 0);
            Hu3DMotionShiftSet(lbl_1_bss_468[E_FX->f90], lbl_1_bss_430[E_FX->f90][0], 0.0f, 10.0f, 0);
            if (GwPlayerConf[curIdx].charNo == 0) {
                lbl_1_bss_20[1] = 0x200;
            }
            if (GwPlayerConf[curIdx].charNo == 1) {
                lbl_1_bss_20[1] = 0x1EC;
            }
            if (GwPlayerConf[curIdx].charNo == 2) {
                lbl_1_bss_20[1] = 0x264;
            }
            if (GwPlayerConf[curIdx].charNo == 3) {
                lbl_1_bss_20[1] = 0x2B4;
            }
            if (GwPlayerConf[curIdx].charNo == 4) {
                lbl_1_bss_20[1] = 0x28C;
            }
            if (GwPlayerConf[curIdx].charNo == 5) {
                lbl_1_bss_20[1] = 0x1C4;
            }
            if (GwPlayerConf[curIdx].charNo == 6) {
                lbl_1_bss_20[1] = 0x2A0;
            }
            fn_1_8D04(lbl_1_bss_20[1], lbl_1_bss_468[curIdx], 1);
            lbl_1_bss_20[1] = 0;
        } else {
            Hu3DMotionShiftSet(lbl_1_bss_468[curIdx], lbl_1_bss_430[curIdx][0], 0.0f, 10.0f, 0);
            Hu3DMotionShiftSet(lbl_1_bss_468[E_FX->f90], lbl_1_bss_430[E_FX->f90][1], 0.0f, 10.0f, 0);
            if (GwPlayerConf[curIdx].charNo == 0) {
                lbl_1_bss_20[1] = 0x1F6;
            }
            if (GwPlayerConf[curIdx].charNo == 1) {
                lbl_1_bss_20[1] = 0x1E2;
            }
            if (GwPlayerConf[curIdx].charNo == 2) {
                lbl_1_bss_20[1] = 0x25A;
            }
            if (GwPlayerConf[curIdx].charNo == 3) {
                lbl_1_bss_20[1] = 0x2AA;
            }
            if (GwPlayerConf[curIdx].charNo == 4) {
                lbl_1_bss_20[1] = 0x282;
            }
            if (GwPlayerConf[curIdx].charNo == 5) {
                lbl_1_bss_20[1] = 0x1BA;
            }
            if (GwPlayerConf[curIdx].charNo == 6) {
                lbl_1_bss_20[1] = 0x296;
            }
            fn_1_8D04(lbl_1_bss_20[1], lbl_1_bss_468[curIdx], 1);
            lbl_1_bss_20[1] = 0;
        }
        fn_1_2FFAC(arg0);
        HuAudFXPlay(8);
        if (E_FX->f8C == 1) {
            E_FX->timer = 0;
        }
        lbl_1_bss_1168[1]++;
        break;

    case 23:
        if (E_FX->f8C == 1) {
            E_FX->timer++;
            if (E_FX->timer != 20) {
                break;
            }
            HuAudFXPlay(8);
            E_FX->timer = 0;
        }
        if (E_FX->f8C >= 0) {
            fn_1_8D04(0x272, E_CURMDL, 1);
            E_12A8->f28 = 60;
            fn_1_4FBC8(arg0, 0x19);
        }
        E_FX->v60 = Hu3DData[E_CURMDL].pos;
        E_FX->rot = Hu3DData[E_CURMDL].rot;
        lbl_1_bss_1168[1]++;
        fn_1_8D04(0x41C, E_CURMDL, 1);
        break;

    case 24:
        E_FX->timer++;
        va.x = 0.0f;
        va.y = 0.0f;
        va.z = 0.0f;
        va.y -= 150.0f * fn_1_4EEA0(0, (f32) E_FX->timer / 30.0f);
        va.z -= 140.0f * ((f32) E_FX->timer / 30.0f);
        if (rec->rot == 0.0f) {
            E_FX->rot.x -= 3.0f;
        } else if (rec->rot == 90.0f) {
            E_FX->rot.z -= 3.0f;
        } else if (rec->rot == 180.0f) {
            E_FX->rot.x -= 3.0f;
        } else {
            E_FX->rot.z += 3.0f;
        }
        PSMTXIdentity(mtx);
        PSMTXRotRad(mtx, 'y', 0.017453292f * rec->rot);
        PSMTXMultVec(mtx, &va, &va);
        PSVECAdd(&E_FX->v60, &va, &va);
        Hu3DModelRotSetV(E_CURMDL, &E_FX->rot);
        Hu3DModelPosSetV(E_CURMDL, &va);
        if (E_FX->timer >= 30) {
            E_FX->timer = 0;
            lbl_1_bss_1168[1]++;
        }
        break;

    case 25:
        E_FX->timer++;
        if (E_FX->timer >= 15) {
            E_FX->timer = 0;
            lbl_1_bss_1168[1]++;
        }
        break;

    case 26:
        E_FX->v60.x = E_FX->pos.x;
        E_FX->v60.y = E_FX->pos.y;
        E_FX->v60.z = E_FX->pos.z;
        E_FX->timer++;
        if (rec->rot != 180.0f) {
            if (rec->rot == 0.0f) {
                E_FX->rot.y = 180.0f - 180.0f * fn_1_4EEA0(2, (f32) E_FX->timer / 30.0f);
            } else if (rec->rot == 90.0f) {
                E_FX->rot.y = -90.0f + 90.0f * fn_1_4EEA0(2, (f32) E_FX->timer / 30.0f);
            } else {
                E_FX->rot.y = 90.0f - 90.0f * fn_1_4EEA0(2, (f32) E_FX->timer / 30.0f);
            }
            E_FX->v60.y += 50.0f * fn_1_4EEA0(5, (f32) E_FX->timer / 30.0f);
            Hu3DModelPosSetV(E_CURMDL, &E_FX->v60);
            Hu3DModelRotSetV(E_CURMDL, &E_FX->rot);
            if (E_FX->timer < 30) {
                break;
            }
            vc = E_FX->pos;
            fn_1_3ED3C(0, &vc);
            fn_1_8D04(0x454, E_CURMDL, 1);
        }
        E_FX->timer = 0;
        Hu3DLLightKill(E_CURMDL, E_FX->light[0]);
        Hu3DLLightKill(E_FX->modelA[0], E_FX->light[1]);
        if (E_FX->light[2] != -1) {
            Hu3DLLightKill(E_FX->modelB[E_FX->f8C], E_FX->light[2]);
        }
        Hu3DMotionSet(E_FX->modelA[0], -1);
        for (i = 0; i < 4; i++) {
            Hu3DMotionKill(E_FX->mot[i]);
        }
        for (i = 0; i < 3; i++) {
            Hu3DModelKill(E_FX->modelA[i]);
        }
        for (i = 0; i < 3; i++) {
            Hu3DModelKill(E_FX->modelB[i]);
        }
        Hu3DModelCameraSet(lbl_1_bss_468[curIdx], 1);
        if (rec->rot == 0.0f) {
            PSVECSubtract(&Hu3DCamera[0].pos, &Hu3DCamera[0].target, &E_FX->v20);
            lbl_1_bss_1168[1] = 0x65;
        } else {
            lbl_1_bss_1168[1] = 0x1B;
        }
        break;

    case 27:
        E_FX->timer++;
        scl = (f32) E_FX->timer / 30.0f;
        for (i = 0; i < 4; i++) {
            if (!(w->rec[i].flags & 0x200)) {
                Hu3DModelScaleSet(lbl_1_bss_468[i], scl, scl, scl);
            }
        }
        Hu3DModelScaleSet(lbl_1_bss_468[curIdx], 1.0f, 1.0f, 1.0f);
        if ((f32) E_FX->timer < 30.0f) {
            break;
        }
        for (i = 0; i < 4; i++) {
            if (!(w->rec[i].flags & 0x200)) {
                E_SlotsAttr(&w->rec[i], 0);
                Hu3DModelScaleSet(lbl_1_bss_468[i], 1.0f, 1.0f, 1.0f);
            }
        }
        fn_1_317F0(&Hu3DCamera[0].pos, &Hu3DCamera[0].pos, &Hu3DCamera[0].target, &Hu3DCamera[0].target);
        lbl_1_bss_1168[1] = 0;
        result = 1;
        break;

    case 100:
        E_FX->timer += 3;
        Hu3DCamera[0].up.x = 0.0f;
        Hu3DCamera[0].up.y = 0.0f;
        Hu3DCamera[0].up.z = 1.0f;
        PSMTXIdentity(mtx);
        Hu3DCamera[0].pos.z -= 5.0f;
        PSMTXRotRad(mtx, 'y', 0.017453292f * (180.0f - 180.0f * fn_1_4EEA0(2, (f32) E_FX->timer / 180.0f)));
        PSMTXMultVec(mtx, &Hu3DCamera[0].up, &Hu3DCamera[0].up);
        if (E_FX->timer != 180) {
            break;
        }
        Hu3DCamera[0].up.x = 0.0f;
        Hu3DCamera[0].up.y = 1.0f;
        Hu3DCamera[0].up.z = 0.0f;
        E_FX->timer = 0;
        PSMTXIdentity(mtx);
        PSMTXRotRad(mtx, 'y', 0.017453292f * rec->rot);
        va.x = lbl_1_data_A78[0][0];
        va.y = lbl_1_data_A78[0][1];
        va.z = lbl_1_data_A78[0][2];
        vb.x = lbl_1_data_A78[0][3];
        vb.y = lbl_1_data_A78[0][4];
        vb.z = lbl_1_data_A78[0][5];
        PSMTXMultVec(mtx, &va, &va);
        PSVECAdd(&E_FX->pos, &va, &va);
        PSMTXMultVec(mtx, &vb, &vb);
        PSVECAdd(&E_FX->pos, &vb, &vb);
        fn_1_317F0(&Hu3DCamera[0].pos, &va, &Hu3DCamera[0].target, &vb);
        lbl_1_bss_1168[1] = 1;
        break;

    case 101:
        E_FX->timer += 3;
        PSMTXIdentity(mtx);
        Hu3DCamera[0].pos.z -= 5.0f;
        if (Hu3DCamera[0].target.x >= 0.0f) {
            PSMTXRotRad(mtx, 'y', 0.017453292f * (180.0f * fn_1_4EEA0(2, (f32) E_FX->timer / 180.0f)));
        } else {
            PSMTXRotRad(mtx, 'y', 0.017453292f * -(180.0f * fn_1_4EEA0(2, (f32) E_FX->timer / 180.0f)));
        }
        PSMTXMultVec(mtx, &E_FX->v20, &va);
        PSVECAdd(&Hu3DCamera[0].target, &va, &Hu3DCamera[0].pos);
        Hu3DCamera[0].target.x += 1.0f;
        if (E_FX->timer >= 180) {
            E_FX->timer = 0;
            lbl_1_bss_1168[1] = 0x1B;
        }
        break;
    }
    Hu3DCamera[1] = Hu3DCamera[0];
    return result;
}

/* ---- chunk F ---- */
// carddll/status.c - chunk F: fn_1_47848 .. fn_1_493F4 (card effect "status" camera/fade scenes).
#include <math.h>

/* ---- externs owned by other carddll units ---- */

/* ---- view of the card player work object (arg0), created by player.c ---- */
typedef struct {
    u8 pad_00[0x10];
    s32 row;            /* 0x10 */
    s32 col;            /* 0x14 */
    f32 angle;          /* 0x18 facing angle in degrees */
    u8 pad_1C[0x50 - 0x1C];
    u32 flags;          /* 0x50 bit1 = item A, bit2 = item B, bit9 = skip */
    u8 pad_54[2];
    s16 slot;           /* 0x56 index of the item slot of this player */
    u8 pad_58[4];
} XF_Rec;               /* 0x5C bytes */

typedef struct {
    XF_Rec rec[4];      /* 0x000 */
    s32 curPlayer;      /* 0x170 */
    s32 recIdx[4];      /* 0x174 */
    u8 pad_184[0x1D4 - 0x184];
    s32 f_1D4;          /* 0x1D4 */
} XF_Work;

/* one entry of the lbl_1_data_8D8 slot arrays: 0x18 bytes, model id first */
typedef struct {
    s16 mdl;            /* 0x00 */
    s16 light;          /* 0x02 */
    s32 f_4;            /* 0x04 */
    u8 pad_08[0x10];
} XF_Slot;

/* ---- lbl_1_bss_17AC: state of the status camera scene (0x9C bytes) ---- */
typedef struct {
    s16 mdl[5];         /* 0x00 effect models */
    u8 pad_0A[6];
    s16 mot[10];        /* 0x10 joint motions of the effect models */
    u8 pad_24[4];
    s16 light[2];       /* 0x28 point lights */
    u8 pad_2C[6];
    s16 mdlEff;         /* 0x32 extra effect model */
    Vec pos;            /* 0x34 board position of the current player */
    Vec camRel;         /* 0x40 camera pos - camera target at start */
    Vec posRel;         /* 0x4C */
    Vec savePos;        /* 0x58 saved camera position */
    Vec saveTgt;        /* 0x64 saved camera target / offset */
    s32 state;          /* 0x70 */
    u8 pad_74[8];
    s32 timer;          /* 0x7C fade level / frame counter */
    s32 timer2;         /* 0x80 */
    s32 done;           /* 0x84 */
    u8 pad_88[8];
    f32 angle;          /* 0x90 */
    f32 scale;          /* 0x94 */
    u8 pad_98[4];
} XF_Blob;

#define XF_BLOB ((XF_Blob *) lbl_1_bss_17AC)
#define XF_REC(w) (&(w)->rec[(w)->recIdx[(w)->curPlayer]])
#define XF_SLOT(grp, i) (&((XF_Slot *) lbl_1_data_8D8[grp].f_C)[i])

/* scale/ambient colour used by the scene fades: set the ambient colour of every model and
   the global/local light colours from fade value f in [0,1]. */
// NONMATCHING: not byte-compared against the original
void fn_1_47848(void *arg0, f32 arg1, f32 arg2, f32 arg3)
{
    XF_Work *w = arg0;
    HU3DMODEL *m;
    s32 i, j;
    s32 lvl;
    s32 col;
    f32 sp8 = arg2;
    f32 spC = arg3;

    m = Hu3DData;
    lvl = (s32) (255.0f * arg1);
    for (i = 0; i < 0x200; i++, m++) {
        if (m->hsf != NULL) {
            m->ambR = arg1;
            m->ambG = sp8;
            m->ambB = spC;
        }
    }
    for (i = 0; i < 5; i++) {
        Hu3DModelAmbSet(XF_BLOB->mdl[i], 1.0f, 1.0f, 1.0f);
    }
    Hu3DModelAmbSet(lbl_1_bss_468[w->recIdx[w->curPlayer]], 1.0f, 1.0f, 1.0f);
    for (i = 0; i < 26; i++) {
        for (j = 0; j < lbl_1_data_8D8[i].f_0; j++) {
            Hu3DModelAmbSet(XF_SLOT(i, j)->mdl, 1.0f, 1.0f, 1.0f);
        }
    }
    col = (s32) (64.0f + 0.75f * (f32) (u8) lvl);
    Hu3DGLightColorSet((s16) lbl_1_bss_314, col, col, col, col);
    col = (u8) (0xFF - lvl);
    Hu3DGLightColorSet(XF_BLOB->light[0], col, col, col, col);
}

/* Scene 1: the camera swoops towards the current player's piece while the screen fades. */
// NONMATCHING: not byte-compared against the original
s32 fn_1_47AA8(CardGame *arg0)
{
    XF_Work *w = (XF_Work *) arg0;
    XF_Rec *rec;
    Vec v = { 0.0f, 1.0f, 0.0f };
    s32 done = 0;
    s32 i;
    f32 fade;

    rec = XF_REC(w);
    switch (lbl_1_bss_1168[1]) {
    case 0:
        memset(lbl_1_bss_17AC, 0, 0x9C);
        fn_1_9C74(&XF_BLOB->pos, rec->row, rec->col, 1, 1);
        XF_BLOB->pos.x += -50.0f;
        for (i = 0; i < 5; i++) {
            XF_BLOB->mdl[i] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_3EC[i], 0x10000000, 2));
            Hu3DModelPosSet(XF_BLOB->mdl[i], 0.0f, 0.0f, 0.0f);
            Hu3DModelAttrSet(XF_BLOB->mdl[i], 1);
            Hu3DModelLayerSet(XF_BLOB->mdl[i], 3);
            Hu3DModelCameraSet(XF_BLOB->mdl[i], 1);
        }
        for (i = 0; i < 6; i++) {
            XF_BLOB->mot[i] = Hu3DJointMotion(XF_BLOB->mdl[0], HuDataSelHeapReadNum(lbl_1_data_400[i], 0x10000000, 2));
        }
        Hu3DMotionSet(XF_BLOB->mdl[0], XF_BLOB->mot[0]);
        for (; i < 10; i++) {
            XF_BLOB->mot[i] = Hu3DJointMotion(XF_BLOB->mdl[1], HuDataSelHeapReadNum(lbl_1_data_400[i], 0x10000000, 2));
        }
        PSVECSubtract(&Hu3DData[lbl_1_bss_468[w->recIdx[w->curPlayer]]].pos, &XF_BLOB->pos, &v);
        PSVECScale(&v, &v, 0.75f);
        PSVECAdd(&XF_BLOB->pos, &v, &v);
        v.y += 100.0f;
        XF_BLOB->light[0] = Hu3DGLightCreate(v.x, v.y, v.z, 0.0f, 1.0f, 0.0f, 0, 0, 0);
        Hu3DGLightPointSet(XF_BLOB->light[0], 200.0f, 1.0f, 2);
        Hu3DGLightStaticSet(XF_BLOB->light[0], 1);
        XF_BLOB->light[1] = Hu3DGLightCreate(v.x, v.y, v.z, 0.0f, 1.0f, 0.0f, 0, 0, 0);
        Hu3DGLightPointSet(XF_BLOB->light[1], 200.0f, 1.0f, 2);
        Hu3DGLightStaticSet(XF_BLOB->light[1], 1);
        Hu3DMotionSet(XF_BLOB->mdl[1], XF_BLOB->mot[6]);
        XF_BLOB->timer = 0;
        lbl_1_bss_1168[1]++;
        break;
    case 1:
        lbl_1_bss_1168[1]++;
        break;
    case 2:
        if (fn_1_48860(arg0) != 0) {
            lbl_1_bss_5E = 0;
            Hu3DZClearLayerSet(0xF);
            XF_BLOB->timer = 0xFF;
            lbl_1_bss_1168[1]++;
        }
        break;
    case 3:
        XF_BLOB->timer -= 8;
        if (XF_BLOB->timer <= 0x10) {
            XF_BLOB->timer = 0x10;
            if (frandmod(10) >= 7) {
                lbl_1_bss_1168[1] += 1;
            } else {
                lbl_1_bss_1168[1] += 2;
            }
        }
        fade = (f32) XF_BLOB->timer / 255.0f;
        fn_1_47848(arg0, fade, fade, fade);
        break;
    case 4:
        if (fn_1_493F4(arg0) != 0) {
            if (XF_BLOB->done == 0) {
                lbl_1_bss_1168[1] = 7;
            } else {
                lbl_1_bss_1168[1]++;
            }
        }
        break;
    case 5:
        if (fn_1_4AB58(arg0) != 0) {
            XF_BLOB->timer = 0x10;
            lbl_1_bss_1168[1]++;
        }
        break;
    case 6:
        if (XF_BLOB->timer == 0x10) {
            if (lbl_1_bss_8[0] != -1) {
                HuAudSStreamStop(lbl_1_bss_8[0]);
                HuAudSStreamVolSet(0, 0x7F, 0x3E8);
            }
            lbl_1_bss_8[0] = 0;
        }
        XF_BLOB->timer += 8;
        if (XF_BLOB->timer >= 0xFF) {
            XF_BLOB->timer = 0xFF;
            lbl_1_bss_1168[1]++;
        }
        fade = (f32) XF_BLOB->timer / 255.0f;
        fn_1_47848(arg0, fade, fade, fade);
        break;
    case 7:
        if (fn_1_48EF8(arg0) != 0) {
            Hu3DMotionSet(XF_BLOB->mdl[0], -1);
            Hu3DMotionSet(XF_BLOB->mdl[1], -1);
            for (i = 0; i < 10; i++) {
                Hu3DMotionKill(XF_BLOB->mot[i]);
            }
            for (i = 0; i < 5; i++) {
                Hu3DModelKill(XF_BLOB->mdl[i]);
            }
            Hu3DGLightKill(XF_BLOB->light[0]);
            Hu3DGLightKill(XF_BLOB->light[1]);
            Hu3DLayerHookSet(0xF, NULL);
            lbl_1_bss_1168[1] = 0;
            done = 1;
        }
        break;
    }
    Hu3DCamera[1] = Hu3DCamera[0];
    return done;
}

/* Scene 1, camera part: pull the camera around the player's piece (4 states). */
// NONMATCHING: not byte-compared against the original
s32 fn_1_48860(void *arg0)
{
    XF_Work *w = arg0;
    XF_Rec *rec;
    Mtx m;
    Vec v1;
    Vec v2;
    Vec up;
    s32 done = 0;
    s32 i;
    f32 sc;

    rec = XF_REC(w);
    switch (XF_BLOB->state) {
    case 0:
        XF_BLOB->savePos = Hu3DCamera[0].pos;
        XF_BLOB->saveTgt = Hu3DCamera[0].target;
        XF_BLOB->timer = 0;
        if (rec->angle == 0.0f) {
            XF_BLOB->angle = 130.0f;
        } else if (rec->angle == 90.0f) {
            XF_BLOB->angle = -140.0f;
        } else if (rec->angle == 180.0f) {
            XF_BLOB->angle = -50.0f;
        } else {
            XF_BLOB->angle = 40.0f;
        }
        /* hide the item models of the other players, show the current player's */
        for (i = 0; i < 4; i++) {
            if ((w->rec[i].flags & 0x200) == 0) {
                if (w->rec[i].flags & 2) {
                    Hu3DModelAttrSet(XF_SLOT(3, w->rec[i].slot)->mdl, 1);
                }
                if (w->rec[i].flags & 4) {
                    Hu3DModelAttrSet(XF_SLOT(4, w->rec[i].slot)->mdl, 1);
                }
            }
        }
        if (rec->flags & 2) {
            Hu3DModelAttrReset(XF_SLOT(3, rec->slot)->mdl, 1);
        }
        if (rec->flags & 4) {
            Hu3DModelAttrReset(XF_SLOT(4, rec->slot)->mdl, 1);
        }
        XF_BLOB->state++;
        break;
    case 1:
        XF_BLOB->timer++;
        up.x = 0.0f;
        up.y = 0.0f;
        up.z = -1.0f;
        Hu3DCamera[0].up = up;
        PSMTXIdentity(m);
        PSMTXRotRad(m, 'y', 0.017453292f * (XF_BLOB->angle * fn_1_4EEA0(2, (f32) XF_BLOB->timer / 45.0f)));
        PSMTXMultVec(m, &Hu3DCamera[0].up, &Hu3DCamera[0].up);
        PSVECSubtract(&XF_BLOB->savePos, &XF_BLOB->saveTgt, &v1);
        v1.z = v1.z + (f32) XF_BLOB->timer;
        PSMTXMultVec(m, &v1, &v1);
        PSVECAdd(&v1, &XF_BLOB->saveTgt, &Hu3DCamera[0].pos);
        sc = 1.0f - ((f32) XF_BLOB->timer / 45.0f);
        for (i = 0; i < 4; i++) {
            if ((w->rec[i].flags & 0x200) == 0) {
                Hu3DModelScaleSet(lbl_1_bss_468[i], sc, sc, sc);
            }
        }
        Hu3DModelScaleSet(lbl_1_bss_468[w->recIdx[w->curPlayer]], 1.0f, 1.0f, 1.0f);
        if (XF_BLOB->timer >= 0x2D) {
            up.x = 0.0f;
            up.y = 1.0f;
            up.z = 0.0f;
            Hu3DCamera[0].up = up;
            XF_BLOB->timer = 0;
            XF_BLOB->state++;
        }
        break;
    case 2:
        PSMTXIdentity(m);
        PSMTXRotRad(m, 'y', 0.017453292f * rec->angle);
        PSMTXMultVec(m, (Vec *) &lbl_1_data_AF8[0], &v1);
        PSVECAdd(&v1, &XF_BLOB->pos, &v1);
        PSMTXMultVec(m, (Vec *) &lbl_1_data_AF8[3], &v2);
        PSVECAdd(&v2, &XF_BLOB->pos, &v2);
        fn_1_317F0(&Hu3DCamera[0].pos, &v1, &Hu3DCamera[0].target, &v2);
        XF_BLOB->state++;
        break;
    case 3:
        if (fn_1_318E4() != 0) {
            XF_BLOB->state = 0;
            done = 1;
        }
        break;
    }
    return done;
}

/* Scene 1, camera return: swing the camera back to where it was. */
// NONMATCHING: not byte-compared against the original
s32 fn_1_48EF8(void *arg0)
{
    XF_Work *w = arg0;
    Mtx m;
    Vec v;
    s32 done = 0;
    s32 i;
    f32 sc;

    switch (XF_BLOB->state) {
    case 0:
        PSVECSubtract(&Hu3DCamera[0].pos, &Hu3DCamera[0].target, &XF_BLOB->camRel);
        PSVECSubtract(&XF_BLOB->pos, &Hu3DCamera[0].target, &XF_BLOB->saveTgt);
        XF_BLOB->saveTgt.x -= -75.0f;
        XF_BLOB->saveTgt.y += 150.0f;
        PSVECScale(&XF_BLOB->saveTgt, &XF_BLOB->saveTgt, 0.016666668f);
        XF_BLOB->angle = (f32) (180.0 * (atan2(XF_BLOB->camRel.z, XF_BLOB->camRel.x) / 3.141592653589793) - 90.0);
        XF_BLOB->timer = 0;
        XF_BLOB->state++;
        break;
    case 1:
        XF_BLOB->timer++;
        PSMTXIdentity(m);
        PSMTXRotRad(m, 'y', 0.017453292f * (XF_BLOB->angle * fn_1_4EEA0(2, (f32) XF_BLOB->timer / 45.0f)));
        PSMTXMultVec(m, &XF_BLOB->camRel, &v);
        PSVECAdd(&XF_BLOB->saveTgt, &Hu3DCamera[0].target, &Hu3DCamera[0].target);
        PSVECAdd(&v, &Hu3DCamera[0].target, &Hu3DCamera[0].pos);
        sc = (f32) XF_BLOB->timer / 45.0f;
        if (w->f_1D4 != 10) {
            for (i = 0; i < 4; i++) {
                if ((w->rec[i].flags & 0x200) == 0) {
                    Hu3DModelScaleSet(lbl_1_bss_468[i], sc, sc, sc);
                }
            }
            Hu3DModelScaleSet(lbl_1_bss_468[w->recIdx[w->curPlayer]], 1.0f, 1.0f, 1.0f);
        }
        if (XF_BLOB->timer >= 0x2D) {
            if (w->f_1D4 != 10) {
                for (i = 0; i < 4; i++) {
                    if ((w->rec[i].flags & 0x200) == 0) {
                        if (w->rec[i].flags & 2) {
                            Hu3DModelAttrReset(XF_SLOT(3, w->rec[i].slot)->mdl, 1);
                        }
                        if (w->rec[i].flags & 4) {
                            Hu3DModelAttrReset(XF_SLOT(4, w->rec[i].slot)->mdl, 1);
                        }
                        Hu3DModelScaleSet(lbl_1_bss_468[i], 1.0f, 1.0f, 1.0f);
                    }
                }
            }
            fn_1_317F0(&Hu3DCamera[0].pos, &Hu3DCamera[0].pos, &Hu3DCamera[0].target, &Hu3DCamera[0].target);
            XF_BLOB->timer = 0;
            XF_BLOB->state = 0;
            done = 1;
        }
        break;
    }
    return done;
}

/* Scene 1, main part: the effect model rises, the item appears, the fade and the Mii cursor move. */
// NONMATCHING: not byte-compared against the original
s32 fn_1_493F4(void *arg0)
{
    XF_Work *w = arg0;
    XF_Rec *rec;
    HU3DMODEL *pm;
    Mtx m;
    Vec v90;            /* 2D position of the effect model (output of Hu3D3Dto2D) */
    Vec v84;
    Vec v78;
    Vec v6C;
    Vec v60;
    Vec v54;
    Vec v48;
    Vec v3C;
    Vec v30;
    Vec v24;
    s32 done = 0;
    s32 slot;
    f32 f;
    f32 fade;

    v90.x = v90.y = v90.z = 0.0f;
    rec = XF_REC(w);
    switch (XF_BLOB->state) {
    case 0:
        Hu3DModelPosSet(XF_BLOB->mdl[1], XF_BLOB->pos.x, 400.0f, XF_BLOB->pos.z);
        Hu3DModelRotSet(XF_BLOB->mdl[1], 0.0f, 180.0f + rec->angle, 0.0f);
        Hu3DModelAttrReset(XF_BLOB->mdl[1], 1);
        Hu3DModelAttrSet(XF_BLOB->mdl[1], 0x40000001);
        Hu3DMotionTimeSet(XF_BLOB->mdl[1], 0.0f);
        Hu3DModelPosSet(XF_BLOB->mdl[2], XF_BLOB->pos.x, 400.0f, XF_BLOB->pos.z);
        Hu3DModelRotSet(XF_BLOB->mdl[2], 0.0f, 180.0f + rec->angle, 0.0f);
        Hu3DModelAttrReset(XF_BLOB->mdl[2], 1);
        XF_BLOB->timer = 0;
        XF_BLOB->state++;
        break;
    case 1:
        XF_BLOB->timer++;
        f = 400.0f - 250.0f * fn_1_4EEA0(1, (f32) XF_BLOB->timer / 60.0f);
        Hu3DData[XF_BLOB->mdl[2]].pos.y = f;
        Hu3DData[XF_BLOB->mdl[1]].pos.y = f;
        if (XF_BLOB->timer >= 0x3C) {
            XF_BLOB->timer = 0;
            XF_BLOB->state++;
        }
        break;
    case 2:
        PSVECSubtract(&Hu3DData[lbl_1_bss_468[w->recIdx[w->curPlayer]]].pos, &XF_BLOB->pos, &v90);
        PSVECScale(&v90, &v90, 7.0f);
        v90.y = 300.0f;
        PSMTXIdentity(m);
        PSMTXRotRad(m, 'y', 0.017453292f * rec->angle);
        v6C.x = -75.0f;
        v6C.y = 0.0f;
        v6C.z = 0.0f;
        v78 = v6C;
        PSMTXMultVec(m, &v78, &v78);
        PSVECAdd(&v90, &XF_BLOB->pos, &v90);
        PSVECAdd(&v90, &v78, &v90);
        v84 = XF_BLOB->pos;
        v84.y += 150.0f;
        PSVECAdd(&v84, &v78, &v84);
        fn_1_317F0(&Hu3DCamera[0].pos, &v90, &Hu3DCamera[0].target, &v84);
        XF_BLOB->timer = 0;
        XF_BLOB->state++;
        break;
    case 3:
        XF_BLOB->timer += 4;
        if (XF_BLOB->timer >= 0xFF) {
            XF_BLOB->timer = 0xFF;
        }
        fade = (f32) XF_BLOB->timer / 255.0f;
        fn_1_47848(arg0, fade, fade, fade);
        if (fn_1_318E4() != 0) {
            HuAudSStreamVolSet(0, 0x7F, 0x3E8);
            fn_1_8D04(0x237, XF_BLOB->mdl[1], 1);
            XF_BLOB->timer = 0;
            fn_1_4FBC8(arg0, 0x30);
            if (frandmod(100) < 0x3C) {
                XF_BLOB->state++;
            } else {
                XF_BLOB->state = 10;
            }
            if (fn_1_5710(arg0, w->curPlayer, 2) != 0) {
                XF_BLOB->state = 10;
            }
        }
        break;
    case 4:
        Hu3DMotionShiftSet(XF_BLOB->mdl[1], XF_BLOB->mot[7], 0.0f, 10.0f, 0);
        fn_1_4FBC8(arg0, 0x32);
        XF_BLOB->state++;
        break;
    case 5:
        if (XF_BLOB->mot[7] == Hu3DData[XF_BLOB->mdl[1]].motId && Hu3DMotionEndCheck(XF_BLOB->mdl[1]) != 0) {
            Hu3DMotionShiftSet(XF_BLOB->mdl[1], XF_BLOB->mot[6], 0.0f, 10.0f, 0x40000001);
            XF_BLOB->mdlEff = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[0x19C / 4], 0x10000000, 2));
            pm = &Hu3DData[lbl_1_bss_468[w->recIdx[w->curPlayer]]];
            Hu3DModelPosSet(XF_BLOB->mdlEff, pm->pos.x, 400.0f, pm->pos.z);
            Hu3DModelCameraSet(XF_BLOB->mdlEff, 1);
            Hu3DModelRotSet(XF_BLOB->mdlEff, 0.0f, 180.0f + rec->angle, 0.0f);
            Hu3DModelAttrReset(XF_BLOB->mdlEff, 1);
            XF_BLOB->state++;
        }
        break;
    case 6:
        if (lbl_1_bss_20[0] == 0 && fn_1_8D04(0x319, XF_BLOB->mdlEff, 1) != -1) {
            lbl_1_bss_20[0] = 1;
        }
        Hu3DData[XF_BLOB->mdlEff].pos.y -= 5.0f;
        if (Hu3DData[XF_BLOB->mdlEff].pos.y <= 250.0f) {
            Hu3DData[XF_BLOB->mdlEff].pos.y = 250.0f;
            XF_BLOB->timer = 0;
            XF_BLOB->scale = 1.0f;
            XF_BLOB->state++;
        }
        break;
    case 7:
        XF_BLOB->timer++;
        if (XF_BLOB->timer >= 0x1E) {
            if (XF_BLOB->timer == 0x1E) {
                fn_1_8D04(0x31B, XF_BLOB->mdlEff, 1);
                lbl_1_bss_20[0] = 0;
            }
            Hu3DData[XF_BLOB->mdlEff].pos.y -= 6.0f;
            XF_BLOB->scale *= 0.85f;
            Hu3DModelScaleSet(XF_BLOB->mdlEff, XF_BLOB->scale, XF_BLOB->scale, XF_BLOB->scale);
            if (XF_BLOB->timer > 0x2D) {
                Hu3DModelKill(XF_BLOB->mdlEff);
                if ((rec->flags & 4) == 0) {
                    if (rec->flags & 2) {
                        rec->flags &= ~2;
                        slot = rec->slot;
                        XF_SLOT(3, slot)->f_4 = 0;
                        Hu3DModelAttrSet(XF_SLOT(3, slot)->mdl, 1);
                        if (XF_SLOT(3, slot)->light != -1) {
                            Hu3DLLightKill(XF_SLOT(3, slot)->mdl, XF_SLOT(3, slot)->light);
                            XF_SLOT(3, slot)->light = -1;
                        }
                    }
                    v60 = Hu3DData[lbl_1_bss_468[w->recIdx[w->curPlayer]]].pos;
                    rec->slot = fn_1_3ED3C(4, &v60);
                    rec->flags |= 4;
                    w->f_1D4 = 0x14;
                }
                XF_BLOB->timer = 0;
                XF_BLOB->state++;
            }
        }
        break;
    case 8:
        XF_BLOB->timer++;
        if (XF_BLOB->timer >= 5) {
            Hu3DMotionShiftSet(XF_BLOB->mdl[1], XF_BLOB->mot[9], 0.0f, 10.0f, 0x40000001);
            fn_1_8D04(0x237, XF_BLOB->mdl[1], 1);
            fn_1_4FBC8(arg0, 0x33);
            XF_BLOB->timer = 0;
            XF_BLOB->state++;
        }
        break;
    case 9:
        XF_BLOB->timer++;
        f = 150.0f + 250.0f * fn_1_4EEA0(0, (f32) XF_BLOB->timer / 60.0f);
        Hu3DData[XF_BLOB->mdl[2]].pos.y = f;
        Hu3DData[XF_BLOB->mdl[1]].pos.y = f;
        if (XF_BLOB->timer >= 0x3C) {
            Hu3DModelAttrSet(XF_BLOB->mdl[1], 1);
            Hu3DModelAttrSet(XF_BLOB->mdl[2], 1);
            XF_BLOB->timer = 0;
            XF_BLOB->state = 0;
            done = 1;
            lbl_1_bss_8[0] = -1;
        }
        break;
    case 10:
        fn_1_4FBC8(arg0, 0x31);
        fn_1_8D04(0x236, XF_BLOB->mdl[1], 1);
        Hu3DMotionShiftSet(XF_BLOB->mdl[1], XF_BLOB->mot[8], 0.0f, 10.0f, 0);
        XF_BLOB->state++;
        break;
    case 11:
        if (XF_BLOB->mot[8] == Hu3DData[XF_BLOB->mdl[1]].motId && Hu3DMotionEndCheck(XF_BLOB->mdl[1]) != 0) {
            Hu3DMotionShiftSet(XF_BLOB->mdl[1], XF_BLOB->mot[6], 0.0f, 10.0f, 0x40000001);
            XF_BLOB->posRel = Hu3DData[XF_BLOB->mdl[1]].pos;
            Hu3D3Dto2D(&Hu3DData[XF_BLOB->mdl[1]].pos, 1, &v90);
            v54.x = -120.0f;
            v54.y = 140.0f;
            v54.z = 0.0f;
            v84 = v54;
            v3C = v84;
            v48 = v90;
            fn_1_4F144(0, &v48, &v3C, 0x3C, 60.0f, 0x455);
            XF_BLOB->timer = 0xFF;
            XF_BLOB->timer2 = 0;
            XF_BLOB->state++;
            HuAudSStreamVolSet(0, 0, 0x3E8);
        }
        break;
    case 12:
        XF_BLOB->timer2++;
        if (XF_BLOB->timer2 < 0x1E) {
            Hu3DData[XF_BLOB->mdl[1]].rot.y -= 1.5f;
            Hu3DData[XF_BLOB->mdl[2]].rot.y -= 1.5f;
        }
        XF_BLOB->timer -= 8;
        if (XF_BLOB->timer <= 0x10) {
            XF_BLOB->timer = 0x10;
        }
        fade = (f32) XF_BLOB->timer / 255.0f;
        fn_1_47848(arg0, fade, fade, fade);
        if (fn_1_4F1EC(0, &v90) != 0) {
            XF_BLOB->state++;
        }
        v24 = XF_BLOB->posRel;
        fn_1_4EDB8(&v30, &v24, v90.x, v90.y);
        Hu3DData[XF_BLOB->mdl[2]].pos = v30;
        Hu3DData[XF_BLOB->mdl[1]].pos = Hu3DData[XF_BLOB->mdl[2]].pos;
        break;
    case 13:
        Hu3DModelAttrSet(XF_BLOB->mdl[1], 1);
        Hu3DModelAttrSet(XF_BLOB->mdl[2], 1);
        XF_BLOB->state = 0;
        XF_BLOB->done = 1;
        done = 1;
        break;
    }
    return done;
}

/* ---- chunk G ---- */
/* ===== chunk G: fn_1_4AB58 .. fn_1_4C1E4 (card-power-up "bonus" event sequence) ===== */

/* Externs of other carddll units (signatures derived from the call sites in this chunk). */
extern s32 fn_1_8D04(s32 seId, s16 modelId, s32 flag);   /* model sound effect, returns sound handle */
extern void fn_1_31AC0(Vec *a, Vec *b);                  /* camera shake init */

/* ---- local views of shared data ---- */

/* lbl_1_bss_17AC (0x9C bytes): state of the "bonus event" sequence. */
typedef struct {
    /* 0x00 */ s16 model;       /* main (dice) model */
    /* 0x02 */ u8 pad02[0x10];
    /* 0x12 */ s16 motIdle;
    /* 0x14 */ s16 mot14;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s16 mot18;
    /* 0x1A */ s16 mot1A;
    /* 0x1C */ u8 pad1C[8];
    /* 0x24 */ s16 llight[2];   /* local light ids of fxModel[0..1] */
    /* 0x28 */ u8 pad28[2];
    /* 0x2A */ s16 glight;      /* global light id */
    /* 0x2C */ s16 esp;         /* effect sprite id */
    /* 0x2E */ s16 fxModel[2];
    /* 0x32 */ u8 pad32[2];
    /* 0x34 */ Vec pos34;
    /* 0x40 */ Vec pos40;
    /* 0x4C */ Vec pos4C;
    /* 0x58 */ u8 pad58[0x18];
    /* 0x70 */ s32 stage;
    /* 0x74 */ s32 step;
    /* 0x78 */ s32 pick;
    /* 0x7C */ s32 timer7C;
    /* 0x80 */ s32 timer80;
    /* 0x84 */ s32 f84;
    /* 0x88 */ s32 target;      /* player that gets hit */
    /* 0x8C */ u8 pad8C[4];
    /* 0x90 */ f32 spin;
    /* 0x94 */ f32 scale;
    /* 0x98 */ u8 alpha;
    /* 0x99 */ u8 pad99[3];
} XG_Fx;
#define G_FX ((XG_Fx *) lbl_1_bss_17AC)

/* player work record (0x5C bytes) and player work (arg0) */
typedef struct {
    /* 0x00 */ u8 pad00[0x18];
    /* 0x18 */ f32 angle;
    /* 0x1C */ u8 pad1C[0x18];
    /* 0x34 */ s32 cards[3];
    /* 0x40 */ s32 f40;
    /* 0x44 */ s32 f44;
    /* 0x48 */ s32 f48;
    /* 0x4C */ s32 f4C;
    /* 0x50 */ u32 flags;
    /* 0x54 */ u8 pad54[8];
} XG_Rec;

typedef struct {
    /* 0x000 */ XG_Rec rec[4];
    /* 0x170 */ s32 cur;
    /* 0x174 */ s32 idx[4];
} XG_Work;

/* element of the slot arrays referenced by lbl_1_data_8D8[n].f_C (0x18 bytes each) */
typedef struct {
    /* 0x00 */ s16 model;
    /* 0x02 */ s16 light;
    /* 0x04 */ s32 flag;
    /* 0x08 */ u8 pad08[0x10];
} XG_Slot;
#define G_SLOT(n) ((XG_Slot *) lbl_1_data_8D8[n].f_C)

/* lbl_1_bss_12A8 + 8 receives the character number of the hit player */
#define G_12A8_CHAR (*(s32 *) (lbl_1_bss_12A8 + 8))

#define G_CUR_PLAYER(w) ((w)->idx[(w)->cur])

/* fn_1_4AB58: bonus event main state machine. Returns 1 when finished. */
// NONMATCHING: not byte-compared against the original
s32 fn_1_4AB58(void *arg0)
{
    XG_Work *w = arg0;
    XG_Rec *rec;
    Vec v2C;
    Vec v20;
    Vec v14;
    Vec off;
    Mtx m;
    s32 ret;
    s32 shifted;
    s32 pick;

    ret = 0;
    rec = &w->rec[G_CUR_PLAYER(w)];
    switch (G_FX->stage) {
    case 0:
        Hu3DModelPosSet(G_FX->model, G_FX->pos34.x, 0.0f, G_FX->pos34.z);
        Hu3DModelRotSet(G_FX->model, 0.0f, 180.0f + rec->angle, 0.0f);
        Hu3DModelAttrReset(G_FX->model, 1);
        Hu3DMotionTimeSet(G_FX->model, 100.0f);
        G_FX->stage++;
        break;
    case 1:
        if (Hu3DMotionTimeGet(G_FX->model) >= 136.0f) {
            Hu3DMotionShiftSet(lbl_1_bss_468[G_CUR_PLAYER(w)], lbl_1_bss_430[G_CUR_PLAYER(w)][2], 0.0f, 10.0f, 0);
            /* per-character voice for the player's character */
            switch (GwPlayerConf[G_CUR_PLAYER(w)].charNo) {
            case 0: lbl_1_bss_20[1] = 0x1FD; break;
            case 1: lbl_1_bss_20[1] = 0x1E9; break;
            case 2: lbl_1_bss_20[1] = 0x261; break;
            case 3: lbl_1_bss_20[1] = 0x2B1; break;
            case 4: lbl_1_bss_20[1] = 0x289; break;
            case 5: lbl_1_bss_20[1] = 0x1C1; break;
            case 6: lbl_1_bss_20[1] = 0x29D; break;
            }
            fn_1_8D04(lbl_1_bss_20[1], lbl_1_bss_468[G_CUR_PLAYER(w)], 1);
            lbl_1_bss_20[1] = 0;
            fn_1_8D04(0x2C7, G_FX->model, 1);
            fn_1_31AC0(&lbl_1_bss_E68[1], &lbl_1_bss_E68[2]);
            lbl_1_bss_F3C[0] = 50;
            G_FX->stage++;
            omVibrate(0, 15, 12, 0);
            omVibrate(1, 15, 12, 0);
            omVibrate(2, 15, 12, 0);
            omVibrate(3, 15, 12, 0);
        }
        break;
    case 2:
        shifted = 0;
        if (Hu3DMotionEndCheck(G_FX->model)) {
            Hu3DMotionShiftSet(G_FX->model, G_FX->motIdle, 0.0f, 10.0f, 0x40000001);
            shifted = 1;
        }
        /* shake the two camera points */
        v2C.x = lbl_1_bss_E68[1].x + (f32) frandmod(lbl_1_bss_F3C[0]);
        v2C.y = lbl_1_bss_E68[1].y + (f32) frandmod(lbl_1_bss_F3C[0]);
        v2C.z = lbl_1_bss_E68[1].z + (f32) frandmod(lbl_1_bss_F3C[0]);
        fn_1_31710(&v2C);
        v2C.x = lbl_1_bss_E68[2].x + (f32) frandmod(lbl_1_bss_F3C[0]);
        v2C.y = lbl_1_bss_E68[2].y + (f32) frandmod(lbl_1_bss_F3C[0]);
        v2C.z = lbl_1_bss_E68[2].z + (f32) frandmod(lbl_1_bss_F3C[0]);
        fn_1_31788(&v2C);
        if (lbl_1_bss_F3C[0] == 5) {
            omVibrate(0, 60, 4, 2);
            omVibrate(1, 60, 4, 2);
            omVibrate(2, 60, 4, 2);
            omVibrate(3, 60, 4, 2);
        }
        lbl_1_bss_F3C[0] -= 3;
        if (lbl_1_bss_F3C[0] < 3) {
            lbl_1_bss_F3C[0] = 3;
            if (shifted != 0) {
                fn_1_31710(&lbl_1_bss_E68[1]);
                fn_1_31788(&lbl_1_bss_E68[2]);
                G_FX->timer7C = 0;
                if (G_FX->f84 == 0) {
                    G_FX->stage++;
                } else {
                    G_FX->stage = 5;
                }
                if (G_FX->stage == 5) {
                    lbl_1_bss_8[0] = HuAudSStreamChanPlay(16, 2);
                }
            }
        }
        break;
    case 3:
        PSVECSubtract(&Hu3DData[lbl_1_bss_468[G_CUR_PLAYER(w)]].pos, &G_FX->pos34, &v2C);
        PSVECScale(&v2C, &v2C, 7.0f);
        v2C.y = 300.0f;
        PSMTXIdentity(m);
        PSMTXRotRad(m, 'y', 0.017453292f * rec->angle);
        off.x = -75.0f;
        off.y = 0.0f;
        off.z = 0.0f;
        v14 = off;
        PSMTXMultVec(m, &v14, &v14);
        PSVECAdd(&v2C, &G_FX->pos34, &v2C);
        PSVECAdd(&v2C, &v14, &v2C);
        v20 = G_FX->pos34;
        v20.y = v20.y + 150.0f;
        PSVECAdd(&v20, &v14, &v20);
        fn_1_317F0(&Hu3DCamera[0].pos, &v2C, &Hu3DCamera[0].target, &v20);
        G_FX->stage++;
        break;
    case 4:
        if (fn_1_318E4() != 0) {
            G_FX->stage++;
        }
        if (G_FX->stage == 5) {
            lbl_1_bss_8[0] = HuAudSStreamChanPlay(16, 2);
        }
        break;
    case 5:
        fn_1_8D04(0x2C9, G_FX->model, 1);
        Hu3DMotionShiftSet(G_FX->model, G_FX->mot18, 0.0f, 10.0f, 0);
        fn_1_4FBC8(arg0, 0x29);
        if (G_FX->mot18 == Hu3DData[G_FX->model].motId && Hu3DMotionEndCheck(G_FX->model)) {
            Hu3DMotionShiftSet(G_FX->model, G_FX->motIdle, 0.0f, 10.0f, 0x40000001);
        }
        fn_1_4FBC8(arg0, 0x2A);
        if (G_FX->mot18 == Hu3DData[G_FX->model].motId && Hu3DMotionEndCheck(G_FX->model)) {
            Hu3DMotionShiftSet(G_FX->model, G_FX->motIdle, 0.0f, 10.0f, 0x40000001);
        }
        fn_1_4FBC8(arg0, 0x2B);
        Hu3DMotionShiftSet(G_FX->model, G_FX->mot14, 0.0f, 10.0f, 0);
        fn_1_8D04(0x2CB, G_FX->model, 1);
        if ((rec->flags & 0x1000) != 0) {
            pick = frandmod(2);
        } else {
            pick = frandmod(3);
        }
        G_FX->pick = pick;
        if (G_FX->pick == 0 && w->rec[0].cards[0] == -1 && w->rec[1].cards[0] == -1
            && w->rec[0].cards[2] == -1 && w->rec[3].cards[0] == -1) {
            G_FX->pick = 1;
        }
        G_FX->stage++;
        break;
    case 6:
        if (fn_1_4B840(arg0) != 0) {
            G_FX->stage = G_FX->stage + G_FX->pick + 1;
        }
        break;
    case 7:
        if (fn_1_4BDD4(arg0) != 0) {
            G_FX->stage = 10;
        }
        break;
    case 8:
        if (fn_1_4C1E4(arg0) != 0) {
            G_FX->stage = 10;
        }
        break;
    case 9:
        if (fn_1_4D8C4(arg0) != 0) {
            G_FX->stage = 10;
        }
        break;
    case 10:
        if (fn_1_4E388(arg0) != 0) {
            G_FX->stage = 0;
            ret = 1;
        }
        break;
    }
    return ret;
}

/* fn_1_4B840: "!" announcement sprite (grows, wobbles, shrinks). arg0 is unused.
   Returns 1 when finished. */
// NONMATCHING: not byte-compared against the original
s32 fn_1_4B840(void *arg0)
{
    /* the original table has 10 entries but is read with index 0..10; the 11th is
       stack garbage there, so it is made an explicit 0 here */
    s32 wobble[11] = { 3, 4, 3, 0, 2, 3, 2, 0, 1, 0, 0 };
    s32 ret;

    ret = 0;
    switch (G_FX->step) {
    case 0:
        if (G_FX->mot14 == Hu3DData[G_FX->model].motId && Hu3DMotionEndCheck(G_FX->model)) {
            G_FX->esp = espEntry(lbl_1_data_2B4[G_FX->pick + 0x12], 0, 0);
            espDrawNoSet(G_FX->esp, 0);
            espPosSet(G_FX->esp, 210.0f, 100.0f);
            espScaleSet(G_FX->esp, 3.0f, 3.0f);
            G_FX->timer80 = 30;
            G_FX->step++;
        }
        break;
    case 1:
        G_FX->timer80--;
        if (G_FX->timer80 <= 10) {
            G_FX->timer7C = 0;
            G_FX->timer80 = 10;
            G_FX->step++;
            HuAudFXPlay(0x438);
        }
        espScaleSet(G_FX->esp, 0.1f * (f32) G_FX->timer80, 0.1f * (f32) G_FX->timer80);
        break;
    case 2:
        espScaleSet(G_FX->esp, 1.0f + 0.05f * (f32) wobble[G_FX->timer7C], 1.0f + 0.05f * (f32) wobble[G_FX->timer7C]);
        G_FX->timer7C++;
        if (G_FX->timer7C > 10) {
            espScaleSet(G_FX->esp, 1.0f, 1.0f);
            G_FX->timer7C = 0;
            G_FX->timer80 = 10;
            G_FX->step++;
        }
        break;
    case 3:
        G_FX->timer7C++;
        if (G_FX->timer7C >= 30) {
            G_FX->timer80--;
            if (G_FX->timer80 <= 0) {
                G_FX->timer80 = 0;
                G_FX->step++;
            }
            espScaleSet(G_FX->esp, 0.1f * (f32) G_FX->timer80, 0.1f * (f32) G_FX->timer80);
        }
        break;
    case 4:
        espKill((s16) lbl_1_data_2B4[G_FX->pick + 0x12]);
        Hu3DMotionShiftSet(G_FX->model, G_FX->motIdle, 0.0f, 30.0f, 0x40000001);
        G_FX->step = 0;
        ret = 1;
        break;
    }
    return ret;
}

/* fn_1_4BDD4: outcome "redistribute cards" (white flash, shuffle every player's
   stored cards among the players). Returns 1 when finished. */
// NONMATCHING: not byte-compared against the original
s32 fn_1_4BDD4(void *arg0)
{
    XG_Work *w = arg0;
    XG_Rec *rec;
    s32 used[4];
    s32 cnt[4];
    s32 cards[12];
    s32 total;
    s32 perPlayer;
    s32 i;
    s32 j;
    s32 k;
    u32 p;
    s32 ret;

    ret = 0;
    switch (G_FX->step) {
    case 0:
        fn_1_4FBC8(arg0, 0x2C);
        Hu3DMotionShiftSet(G_FX->model, G_FX->mot1A, 0.0f, 10.0f, 0);
        G_FX->step++;
        lbl_1_bss_20[0] = 0;
        break;
    case 1:
        lbl_1_bss_20[0]++;
        if (lbl_1_bss_20[0] == 30) {
            fn_1_8D04(0x2CC, G_FX->model, 1);
        }
        if (G_FX->mot1A == Hu3DData[G_FX->model].motId && Hu3DMotionEndCheck(G_FX->model)) {
            WipeCreate(2, 0, 60);
            WipeColorSet(255, 255, 255);
            G_FX->step++;
            omVibrate(0, 60, 6, 6);
            omVibrate(1, 60, 6, 6);
            omVibrate(2, 60, 6, 6);
            omVibrate(3, 60, 6, 6);
            HuAudFXPlay(0x439);
            lbl_1_bss_20[0] = 0;
        }
        break;
    case 2:
        if (WipeCheckEnd() == 0) {
            memset(cards, -1, sizeof(cards));
            total = 0;
            /* collect every player's stored cards and clear the slots */
            for (i = 0; i < 4; i++) {
                used[i] = 0;
                cnt[i] = 0;
                rec = &w->rec[i];
                for (j = 0; j < 3; j++) {
                    if (rec->cards[j] == -1) {
                        break;
                    }
                    cards[total] = rec->cards[j];
                    total++;
                }
                rec->cards[2] = -1;
                rec->cards[1] = -1;
                rec->cards[0] = -1;
            }
            perPlayer = total / 4;
            /* deal an equal share to every player */
            for (k = 0; k < perPlayer * 4; k++) {
                do {
                    p = frandmod(100) / 25;
                } while (cnt[p] >= perPlayer);
                w->rec[p].cards[cnt[p]] = cards[k];
                cnt[p]++;
            }
            /* the remainder goes to distinct random players */
            for (k = perPlayer * 4; k < total; k++) {
                do {
                    p = frandmod(100) / 25;
                } while (used[p] != 0);
                w->rec[p].cards[cnt[p]] = cards[k];
                used[p] = 1;
            }
            fn_1_2FFAC(arg0);
            WipeCreate(1, 0, 60);
            G_FX->step++;
        }
        break;
    case 3:
        if (WipeCheckEnd() == 0) {
            G_FX->step = 0;
            ret = 1;
        }
        break;
    }
    return ret;
}

/* fn_1_4C1E4: outcome "lightning strike on a random player" (two effect models
   with lights, a projectile that hits player G_FX->target, who gets a card
   removed/penalty). Returns 1 when finished. */
// NONMATCHING: not byte-compared against the original
s32 fn_1_4C1E4(void *arg0)
{
    XG_Work *w = arg0;
    XG_Rec *rec;
    XG_Rec *hit;
    Vec rot;
    Vec v;
    Vec out;
    Vec v2d;
    Vec org;
    Mtx m;
    Mtx m2;
    s32 ret;
    s32 i;
    s32 j;

    ret = 0;
    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    rec = &w->rec[G_CUR_PLAYER(w)];
    switch (G_FX->step) {
    case 0:
        fn_1_4FBC8(arg0, 0x2D);
        Hu3DMotionShiftSet(G_FX->model, G_FX->mot1A, 0.0f, 10.0f, 0);
        G_FX->step++;
        lbl_1_bss_20[0] = 0;
        break;
    case 1:
        lbl_1_bss_20[0]++;
        if (lbl_1_bss_20[0] == 30) {
            fn_1_8D04(0x2CC, G_FX->model, 1);
        }
        if (G_FX->mot1A == Hu3DData[G_FX->model].motId && Hu3DMotionEndCheck(G_FX->model)) {
            for (i = 0; i < 2; i++) {
                G_FX->fxModel[i] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[0x4E + i], 0x10000000, 2));
                Hu3DModelPosSet(G_FX->fxModel[i], 0.0f, 0.0f, 0.0f);
                Hu3DModelAttrSet(G_FX->fxModel[i], 1);
                Hu3DModelLayerSet(G_FX->fxModel[i], 3);
                Hu3DModelCameraSet(G_FX->fxModel[i], 1);
                Hu3DModelScaleSet(G_FX->fxModel[i], 0.5f, 0.5f, 1.0f);
            }
            /* light/effect position: fixed offset rotated by the player's angle */
            org.x = 0.0f;
            org.y = 200.0f;
            org.z = -150.0f;
            v = org;
            PSMTXIdentity(m);
            PSMTXRotRad(m, 'y', 0.017453292f * rec->angle);
            PSMTXMultVec(m, &v, &v);
            PSVECAdd(&v, &G_FX->pos34, &G_FX->pos40);
            Hu3DGLightPosSet(G_FX->glight, G_FX->pos40.x, G_FX->pos40.y, G_FX->pos40.z, 0.0f, 1.0f, 0.0f);
            G_FX->llight[0] = Hu3DLLightCreate(G_FX->fxModel[0], Hu3DCamera[0].pos.x, Hu3DCamera[0].pos.y, Hu3DCamera[0].pos.z,
                                               0.0f, 0.0f, -1.0f, 255, 255, 255);
            Hu3DLLightInfinitytSet(G_FX->fxModel[0], G_FX->llight[0]);
            G_FX->llight[1] = Hu3DLLightCreate(G_FX->fxModel[1], Hu3DCamera[0].pos.x, Hu3DCamera[0].pos.y, Hu3DCamera[0].pos.z,
                                               0.0f, 0.0f, -1.0f, 255, 255, 255);
            Hu3DLLightInfinitytSet(G_FX->fxModel[1], G_FX->llight[1]);
            v = G_FX->pos40;
            fn_1_3ED3C(9, &v);
            rot.y = 180.0f + rec->angle;
            Hu3DModelRotSetV(G_SLOT(9)->model, &rot);
            Hu3DModelScaleSet(G_SLOT(9)->model, 0.5f, 0.5f, 1.0f);
            G_FX->timer80 = 0;
            G_FX->step++;
            G_FX->alpha = 0;
            fn_1_8D04(0x44C, G_FX->model, 1);
            lbl_1_bss_20[0] = 0;
        }
        break;
    case 2:
        G_FX->alpha += 4;
        if (G_FX->alpha > 0xFF) {
            G_FX->alpha = 0xFF;
        }
        Hu3DGLightColorSet(G_FX->glight, G_FX->alpha, G_FX->alpha, G_FX->alpha, G_FX->alpha);
        G_FX->timer80++;
        if (G_FX->timer80 > 55) {
            Hu3DModelPosSetV(G_FX->fxModel[0], &G_FX->pos40);
            rot.y = 180.0f + rec->angle;
            Hu3DModelRotSetV(G_FX->fxModel[0], &rot);
            Hu3DModelAttrReset(G_FX->fxModel[0], 1);
            G_FX->timer7C = 0;
            G_FX->timer80 = 0;
            G_FX->step++;
        }
        break;
    case 3:
        G_FX->timer7C++;
        if (G_FX->timer7C >= 150) {
            if (G_FX->alpha >= 8) {
                G_FX->alpha -= 8;
            }
            Hu3DGLightColorSet(G_FX->glight, G_FX->alpha, G_FX->alpha, G_FX->alpha, G_FX->alpha);
        }
        if ((G_SLOT(9)->flag ^ 1) != 0) {
            G_FX->timer80++;
            if (G_FX->timer80 > 30) {
                Hu3DModelAttrSet(G_FX->fxModel[0], 1);
                v = G_FX->pos40;
                fn_1_3ED3C(11, &v);
                rot.x = 0.0f;
                rot.y = 180.0f + rec->angle;
                rot.z = 0.0f;
                Hu3DModelRotSetV(G_SLOT(11)->model, &rot);
                Hu3DModelScaleSet(G_SLOT(11)->model, 0.5f, 0.5f, 1.0f);
                G_FX->timer80 = 0;
                G_FX->step++;
                omVibrate(w->idx[w->cur], 50, 6, 6);
                fn_1_8D04(0x41E, G_FX->fxModel[0], 1);
            }
        }
        break;
    case 4:
        if ((G_SLOT(11)->flag ^ 1) != 0) {
            G_SLOT(11)->flag = 0;
            Hu3DModelAttrSet(G_SLOT(11)->model, 1);
            if (G_SLOT(11)->light != -1) {
                Hu3DLLightKill(G_SLOT(11)->model, G_SLOT(11)->light);
                G_SLOT(11)->light = -1;
            }
            v = G_FX->pos40;
            fn_1_3ED3C(12, &v);
            rot.x = 0.0f;
            rot.y = 180.0f + rec->angle;
            rot.z = 0.0f;
            Hu3DModelRotSetV(G_SLOT(12)->model, &rot);
            Hu3DModelScaleSet(G_SLOT(12)->model, 0.5f, 0.5f, 1.0f);
            G_FX->timer80 = 0;
            G_FX->step++;
        }
        break;
    case 5:
        G_FX->timer80++;
        if (G_FX->timer80 == 10) {
            v = G_FX->pos40;
            fn_1_3ED3C(10, &v);
            rot.x = 0.0f;
            rot.y = 180.0f + rec->angle;
            rot.z = 0.0f;
            Hu3DModelRotSetV(G_SLOT(10)->model, &rot);
            Hu3DModelScaleSet(G_SLOT(10)->model, 0.5f, 0.5f, 1.0f);
            omVibrate(w->idx[w->cur], 15, 12, 0);
            fn_1_8D04(0x41F, G_FX->fxModel[0], 1);
            fn_1_8D04(0x44D, G_FX->fxModel[0], 1);
        }
        if ((G_SLOT(12)->flag ^ 1) != 0) {
            G_SLOT(12)->flag = 0;
            Hu3DModelAttrSet(G_SLOT(12)->model, 1);
            if (G_SLOT(12)->light != -1) {
                Hu3DLLightKill(G_SLOT(12)->model, G_SLOT(12)->light);
                G_SLOT(12)->light = -1;
            }
            Hu3DModelPosSetV(G_FX->fxModel[1], &G_FX->pos40);
            rot.y = 180.0f + rec->angle;
            Hu3DModelRotSetV(G_FX->fxModel[1], &rot);
            Hu3DModelCameraSet(G_FX->fxModel[1], 1);
            Hu3DModelLayerSet(G_FX->fxModel[1], 7);
            Hu3DModelAttrReset(G_FX->fxModel[1], 1);
            G_FX->step++;
        }
        break;
    case 6:
        if ((G_SLOT(10)->flag ^ 1) != 0) {
            Hu3DMotionShiftSet(G_FX->model, G_FX->motIdle, 0.0f, 30.0f, 0x40000001);
            G_FX->target = frandmod(100) / 25;
            G_FX->pos4C = Hu3DData[G_FX->fxModel[1]].pos;
            Hu3D3Dto2D(&Hu3DData[G_FX->fxModel[1]].pos, 1, &v2d);
            /* position of the hit player's panel */
            j = 0;
            while (w->idx[j] != G_FX->target) {
                j++;
            }
            org.x = lbl_1_data_528[j][0] - 7.0f;
            org.y = lbl_1_data_528[j][1] - 12.0f;
            org.z = 0.0f;
            fn_1_4F144(0, &v2d, &org, 60, 0.0f, 33);
            G_FX->spin = 0.0f;
            G_FX->scale = 1.0f;
            G_FX->timer80 = 0;
            G_FX->step++;
            fn_1_8D04(0x353, G_FX->fxModel[1], 1);
        }
        break;
    case 7:
        fn_1_8D04(0x8353, G_FX->fxModel[1], 1);
        G_FX->scale = 0.5f - 0.4f * fn_1_4EEA0(0, (f32) G_FX->timer80 / 60.0f);
        G_FX->spin = G_FX->spin - (10.0f + 5.5f * fn_1_4EEA0(0, (f32) G_FX->timer80 / 60.0f));
        PSMTXIdentity(m);
        PSMTXRotRad(m2, 'y', 0.017453292f * (180.0f + rec->angle));
        PSMTXConcat(m, m2, m);
        PSMTXRotRad(m2, 'z', 0.017453292f * G_FX->spin);
        PSMTXConcat(m, m2, m);
        Hu3DMtxRotGet(m, &rot);
        Hu3DModelRotSetV(G_FX->fxModel[1], &rot);
        Hu3DModelScaleSet(G_FX->fxModel[1], G_FX->scale, G_FX->scale, G_FX->scale);
        fn_1_4F1EC(0, &v2d);
        v = G_FX->pos4C;
        fn_1_4EDB8(&out, &v, v2d.x, v2d.y);
        Hu3DData[G_FX->fxModel[1]].pos = out;
        if (G_FX->timer80 == 40) {
            Hu3DModelCameraSet(G_FX->fxModel[1], 2);
        }
        G_FX->timer80++;
        if (G_FX->timer80 > 60) {
            Hu3DModelAttrSet(G_FX->fxModel[1], 1);
            hit = &w->rec[G_FX->target];
            hit->f4C++;
            hit->f40 = hit->f44 + (hit->f48 * 2 + hit->f4C * -1);
            fn_1_2FFAC(arg0);
            Hu3DMotionShiftSet(lbl_1_bss_468[G_FX->target], lbl_1_bss_430[G_FX->target][1], 0.0f, 10.0f, 0);
            if (G_FX->target == G_CUR_PLAYER(w)) {
                /* per-character voice for the player's character */
                switch (GwPlayerConf[G_CUR_PLAYER(w)].charNo) {
                case 0: lbl_1_bss_20[1] = 0x200; break;
                case 1: lbl_1_bss_20[1] = 0x1EC; break;
                case 2: lbl_1_bss_20[1] = 0x264; break;
                case 3: lbl_1_bss_20[1] = 0x2B4; break;
                case 4: lbl_1_bss_20[1] = 0x28C; break;
                case 5: lbl_1_bss_20[1] = 0x1C4; break;
                case 6: lbl_1_bss_20[1] = 0x2A0; break;
                }
                fn_1_8D04(lbl_1_bss_20[1], lbl_1_bss_468[G_CUR_PLAYER(w)], 1);
                lbl_1_bss_20[1] = 0;
            }
            G_FX->step++;
            HuAudFXPlay(8);
        }
        if (G_FX->timer80 == 58) {
            omVibrate(G_FX->target, 15, 12, 0);
        }
        break;
    case 8:
        if (Hu3DMotionEndCheck(lbl_1_bss_468[G_FX->target])) {
            Hu3DLLightKill(G_FX->fxModel[0], G_FX->llight[0]);
            Hu3DLLightKill(G_FX->fxModel[1], G_FX->llight[1]);
            for (i = 0; i < 2; i++) {
                Hu3DModelKill(G_FX->fxModel[i]);
            }
            G_FX->step = 0;
            ret = 1;
            G_12A8_CHAR = GwPlayerConf[G_FX->target].charNo;
            fn_1_4FBC8(arg0, 0x42);
        }
        break;
    }
    return ret;
}

/* ---- chunk H ---- */
// carddll/status.c - chunk H: fn_1_4D8C4 .. fn_1_50460 (ending cutscene steps, capture-texture
// effect, easing/tween helpers, message window handling).
#include "math.h"
#include "dolphin/gx.h"
#include "game/data.h"

/* ---- externs of other carddll units ---- */
void fn_1_2FBCC(CardGame *arg0);                     /* card-hand animation (player.c) */
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, int wrapS, int wrapT, int filter);

/* ---- view of the arg0 'player work' object ---- */
typedef struct {
    u8 pad_0[0x170];
    s32 curPlayerNo;  /* 0x170 */
    s32 recIdx[4];    /* 0x174 */
} HH_Work;
#define HH_REC(w) (((HH_Work *) (w))->recIdx[((HH_Work *) (w))->curPlayerNo])

/* ---- lbl_1_bss_17AC: ending cutscene state (only the parts used here) ---- */
typedef struct {
    s16 model0;       /* 0x00 */
    s16 pad_2[2];     /* 0x02 */
    s16 modelA;       /* 0x06 */
    s16 modelB;       /* 0x08 */
    s16 pad_A[3];     /* 0x0A */
    s16 mot10;        /* 0x10 */
    s16 mot12;        /* 0x12 */
    s16 pad_14;       /* 0x14 */
    s16 mot16;        /* 0x16 */
    s16 mot18;        /* 0x18 */
    u8 pad_1A[0x10];  /* 0x1A */
    s16 lightId;      /* 0x2A */
    u8 pad_2C[0x48];  /* 0x2C */
    s32 state;        /* 0x74 */
    u8 pad_78[0x20];  /* 0x78 */
    u8 lightLevel;    /* 0x98 */
} HH_Fin;
#define HH_FIN ((HH_Fin *) lbl_1_bss_17AC)

/* ---- lbl_1_bss_1944 (0x30): water-surface capture effect ---- */
typedef struct {
    void *tex;          /* 0x00 capture buffer */
    ANIMDATA *spr1;     /* 0x04 */
    ANIMDATA *spr2;     /* 0x08 */
    f32 scroll;         /* 0x0C */
    f32 scrollSpd;      /* 0x10 */
    f32 indMtx[2][3];   /* 0x14 */
    u8 color[4];        /* 0x2C */
} HH_Cap;
#define HH_CAP ((HH_Cap *) &lbl_1_bss_1944)

/* ---- lbl_1_bss_12FC: 30 tween records of 0x28 bytes ---- */
typedef struct {
    s32 cnt;      /* 0x00 current frame */
    Vec from;     /* 0x04 */
    Vec to;       /* 0x10 */
    s32 dur;      /* 0x1C frames */
    f32 amp;      /* 0x20 swing amplitude */
    s32 mode;     /* 0x24 bits 0-3: x ease, 4-7: y ease, 8-11: swing */
} HH_Tween;
#define HH_TWEEN ((HH_Tween *) lbl_1_bss_12FC)

/* ---- lbl_1_bss_12A8 (0x54): message window record ---- */
typedef struct {
    s32 open;         /* 0x00 nonzero: window shown with HuWinDispOn instead of ExOpen/ExClose */
    s16 speed;        /* 0x04 message speed */
    s16 pad_6;
    s32 ins[8];       /* 0x08 insert message ids, -1 terminated */
    s32 f28;          /* 0x28 */
    s32 fixedSize;    /* 0x2C force 448x94 */
    s32 idMode;       /* 0x30 message id = arg + 0x13FFFF instead of table lookup */
    s32 f34;          /* 0x34 */
    s32 keyWait;      /* 0x38 */
    s32 frameSprNo;   /* 0x3C data number of window frame sprite, -1 none */
    s32 disabled;     /* 0x40 nonzero: all message functions are no-ops */
    s32 win;          /* 0x44 main window id, -1 none */
    ANIMDATA *anim;   /* 0x48 */
    s16 animSpr;      /* 0x4C */
    s16 pad_4E;
    s32 subWin;       /* 0x50 secondary window id, -1 none */
} HH_Win;
#define HH_WIN ((HH_Win *) lbl_1_bss_12A8)

/* Ending cutscene, state machine on HH_FIN->state. Returns 1 when finished. */
// NONMATCHING: not byte-compared against the original
s32 fn_1_4D8C4(void *arg0)
{
    Mtx mtx;
    Vec pos;
    Vec rot;
    s32 done = 0;
    s32 lightG;
    s16 cur;

    switch (HH_FIN->state) {
    case 0:
        fn_1_4FBC8(arg0, 0x2E);
        Hu3DMotionShiftSet(HH_FIN->model0, HH_FIN->mot16, 0.0f, 10.0f, 0);
        Hu3DMotionTimeSet(HH_FIN->modelA, 0.0f);
        Hu3DMotionTimeSet(HH_FIN->modelB, 0.0f);
        Hu3DModelAttrSet(HH_FIN->modelA, 1);
        Hu3DModelAttrSet(HH_FIN->modelB, 1);
        Hu3DModelAttrSet(HH_FIN->modelA, 0x40000002);
        Hu3DModelAttrSet(HH_FIN->modelB, 0x40000002);
        HH_FIN->state++;
        break;
    case 1:
        if (HH_FIN->mot16 != Hu3DData[HH_FIN->model0].motId) {
            break;
        }
        if (!(Hu3DMotionTimeGet(HH_FIN->model0) >= 50.0f)) {
            break;
        }
        cur = HH_REC(arg0);
        Hu3DMotionShiftSet(lbl_1_bss_468[cur], lbl_1_bss_430[cur][2], 0.0f, 10.0f, 0x40000001);
        if (GwPlayerConf[HH_REC(arg0)].charNo == 0) {
            lbl_1_bss_20[1] = 0x1F9;
        }
        if (GwPlayerConf[HH_REC(arg0)].charNo == 1) {
            lbl_1_bss_20[1] = 0x1E5;
        }
        if (GwPlayerConf[HH_REC(arg0)].charNo == 2) {
            lbl_1_bss_20[1] = 0x25D;
        }
        if (GwPlayerConf[HH_REC(arg0)].charNo == 3) {
            lbl_1_bss_20[1] = 0x2AD;
        }
        if (GwPlayerConf[HH_REC(arg0)].charNo == 4) {
            lbl_1_bss_20[1] = 0x285;
        }
        if (GwPlayerConf[HH_REC(arg0)].charNo == 5) {
            lbl_1_bss_20[1] = 0x1BD;
        }
        if (GwPlayerConf[HH_REC(arg0)].charNo == 6) {
            lbl_1_bss_20[1] = 0x299;
        }
        fn_1_8D04(lbl_1_bss_20[1], lbl_1_bss_468[HH_REC(arg0)], 1);
        lbl_1_bss_20[1] = 0;
        Hu3DModelObjMtxGet(HH_FIN->model0, lbl_1_data_B98, mtx);
        pos.x = mtx[0][3];
        pos.y = mtx[1][3];
        pos.z = mtx[2][3];
        Hu3DMtxRotGet(mtx, (HuVecF *) &rot);
        Hu3DModelPosSetV(HH_FIN->modelA, (HuVecF *) &pos);
        Hu3DModelRotSetV(HH_FIN->modelA, (HuVecF *) &rot);
        Hu3DModelPosSetV(HH_FIN->modelB, (HuVecF *) &pos);
        Hu3DModelRotSetV(HH_FIN->modelB, (HuVecF *) &rot);
        Hu3DModelAttrReset(HH_FIN->modelA, 1);
        Hu3DModelAttrReset(HH_FIN->modelA, 0x40000002);
        HH_FIN->lightLevel = 0;
        HH_FIN->state++;
        omVibrate(0, 0xAA, 4, 2);
        omVibrate(1, 0xAA, 4, 2);
        omVibrate(2, 0xAA, 4, 2);
        omVibrate(3, 0xAA, 4, 2);
        lbl_1_bss_20[0] = fn_1_8D04(0x43A, HH_FIN->modelB, 1);
        break;
    case 2:
        HH_FIN->lightLevel += 0xC;
        if (HH_FIN->lightLevel > 0xC8) {
            HH_FIN->lightLevel = 0xC8;
        }
        lightG = HH_FIN->lightLevel / 2;
        Hu3DGLightColorSet(HH_FIN->lightId, HH_FIN->lightLevel, (u8) lightG, 0, 0x7F);
        Hu3DModelObjMtxGet(HH_FIN->model0, lbl_1_data_B98, mtx);
        pos.x = mtx[0][3];
        pos.y = mtx[1][3];
        pos.z = mtx[2][3];
        Hu3DMtxRotGet(mtx, (HuVecF *) &rot);
        Hu3DModelPosSetV(HH_FIN->modelA, (HuVecF *) &pos);
        Hu3DModelRotSetV(HH_FIN->modelA, (HuVecF *) &rot);
        Hu3DModelPosSetV(HH_FIN->modelB, (HuVecF *) &pos);
        Hu3DModelRotSetV(HH_FIN->modelB, (HuVecF *) &rot);
        fn_1_8D04(0x843A, HH_FIN->modelB, 1);
        if (Hu3DMotionTimeGet(HH_FIN->modelA) >= 26.0f) {
            Hu3DModelAttrSet(HH_FIN->modelA, 1);
            Hu3DModelAttrReset(HH_FIN->modelB, 1);
            Hu3DModelAttrReset(HH_FIN->modelB, 0x40000002);
            Hu3DModelAttrSet(HH_FIN->modelB, 0x40000001);
            HH_FIN->state++;
        }
        break;
    case 3:
        Hu3DModelObjMtxGet(HH_FIN->model0, lbl_1_data_B98, mtx);
        pos.x = mtx[0][3];
        pos.y = mtx[1][3];
        pos.z = mtx[2][3];
        Hu3DMtxRotGet(mtx, (HuVecF *) &rot);
        Hu3DModelPosSetV(HH_FIN->modelB, (HuVecF *) &pos);
        Hu3DModelRotSetV(HH_FIN->modelB, (HuVecF *) &rot);
        fn_1_8D04(0x843A, HH_FIN->modelB, 1);
        if (Hu3DMotionTimeGet(HH_FIN->model0) >= 170.0f) {
            WipeCreate(2, 0, 0x3C);
            WipeColorSet(0, 0, 0);
            HH_FIN->state++;
            HuAudFXFadeOut(lbl_1_bss_20[0], 0x64);
        }
        break;
    case 4:
        Hu3DModelObjMtxGet(HH_FIN->model0, lbl_1_data_B98, mtx);
        pos.x = mtx[0][3];
        pos.y = mtx[1][3];
        pos.z = mtx[2][3];
        Hu3DMtxRotGet(mtx, (HuVecF *) &rot);
        Hu3DModelPosSetV(HH_FIN->modelB, (HuVecF *) &pos);
        Hu3DModelRotSetV(HH_FIN->modelB, (HuVecF *) &rot);
        if (WipeCheckEnd() == 0) {
            fn_1_2FBCC((CardGame *) arg0);
            Hu3DMotionSet(lbl_1_bss_468[HH_REC(arg0)], lbl_1_bss_430[HH_REC(arg0)][6]);
            Hu3DGLightColorSet(HH_FIN->lightId, 0, 0, 0, 0);
            Hu3DMotionSet(HH_FIN->model0, HH_FIN->mot12);
            Hu3DModelAttrSet(HH_FIN->model0, 0x40000001);
            Hu3DModelAttrSet(HH_FIN->modelB, 1);
            Hu3DModelHookReset(HH_FIN->model0);
            WipeCreate(1, 0, 0x3C);
            HH_FIN->state++;
            HuAudFXStop(lbl_1_bss_20[0]);
        }
        break;
    case 5:
        if (WipeCheckEnd() == 0) {
            HH_FIN->state = 0;
            done = 1;
        }
        break;
    }
    return done;
}

/* Second ending step (motion change + SEs). Returns 1 when finished. */
// NONMATCHING: not byte-compared against the original
s32 fn_1_4E388(void *arg0)
{
    s32 done = 0;

    switch (HH_FIN->state) {
    case 0:
        Hu3DMotionShiftSet(HH_FIN->model0, HH_FIN->mot18, 0.0f, 10.0f, 0);
        HH_FIN->state++;
        break;
    case 1:
        fn_1_8D04(0x2C9, HH_FIN->model0, 1);
        fn_1_4FBC8(arg0, 0x2F);
        Hu3DMotionShiftSet(HH_FIN->model0, HH_FIN->mot10, 0.0f, 10.0f, 0);
        HuAudSStreamFadeOut(lbl_1_bss_8[0], 0x3E8);
        HH_FIN->state++;
        lbl_1_bss_20[0] = 0;
        break;
    case 2:
        lbl_1_bss_20[0]++;
        if (lbl_1_bss_20[0] == 0x32) {
            fn_1_8D04(0x2CC, HH_FIN->model0, 1);
        }
        if (HH_FIN->mot10 == Hu3DData[HH_FIN->model0].motId) {
            if (Hu3DMotionTimeGet(HH_FIN->model0) >= 100.0f) {
                Hu3DModelAttrSet(HH_FIN->model0, 1);
                HH_FIN->state = 0;
                done = 1;
                lbl_1_bss_20[0] = 0;
            }
        }
        break;
    }
    return done;
}

/* Create the capture buffer and load the two water sprites; reset effect parameters. */
// NONMATCHING: not byte-compared against the original
void fn_1_4E5A4(void)
{
    HH_CAP->tex = HuMemDirectMallocNum(2, GXGetTexBufferSize(0x140, 0xF0, 4, 0, 0), 0x10000000);
    HH_CAP->spr1 = HuSprAnimRead(HuDataSelHeapReadNum(0x1100DC, 0x10000000, 2));
    HH_CAP->spr2 = HuSprAnimRead(HuDataSelHeapReadNum(0x1100DD, 0x10000000, 2));
    HH_CAP->indMtx[0][0] = 0.0f;
    HH_CAP->indMtx[0][1] = 0.0f;
    HH_CAP->indMtx[0][2] = 0.0f;
    HH_CAP->indMtx[1][0] = 0.0f;
    HH_CAP->indMtx[1][1] = 0.0f;
    HH_CAP->indMtx[1][2] = 0.0f;
    HH_CAP->color[0] = 0;
    HH_CAP->color[1] = 0;
    HH_CAP->color[2] = 0;
    HH_CAP->color[3] = 0;
    HH_CAP->scroll = 0.0f;
    HH_CAP->scrollSpd = -0.02f;
}

/* Free the capture buffer. */
// NONMATCHING: not byte-compared against the original
void fn_1_4E758(void)
{
    if (HH_CAP->tex != NULL) {
        HuMemDirectFree(HH_CAP->tex);
        HH_CAP->tex = NULL;
    }
}

/* Layer hook (layer 0xC): copy the frame buffer into the capture texture. */
// NONMATCHING: not byte-compared against the original
void fn_1_4E7A8(s16 layerNo)
{
    s32 paused;

    if ((Hu3DCameraBit & 1) != 0) {
        GXSetTexCopySrc(0, 0, 0x280, 0x1E0);
        GXSetTexCopyDst(0x140, 0xF0, 4, 1);
        GXCopyTex(HH_CAP->tex, 0);
        paused = omUPauseFlag + omPauseChk();
        if (paused == 0) {
        }
    }
}

/* Set the two rows of the indirect texture matrix. */
// NONMATCHING: not byte-compared against the original
void fn_1_4E848(Vec *arg0, Vec *arg1)
{
    HH_CAP->indMtx[0][0] = arg0->x;
    HH_CAP->indMtx[0][1] = arg0->y;
    HH_CAP->indMtx[0][2] = arg0->z;
    HH_CAP->indMtx[1][0] = arg1->x;
    HH_CAP->indMtx[1][1] = arg1->y;
    HH_CAP->indMtx[1][2] = arg1->z;
}

/* Set the TEV register color (RGBA bytes). */
// NONMATCHING: not byte-compared against the original
void fn_1_4E8AC(void *arg0)
{
    u8 *c = (u8 *) arg0;

    HH_CAP->color[0] = c[0];
    HH_CAP->color[1] = c[1];
    HH_CAP->color[2] = c[2];
    HH_CAP->color[3] = c[3];
}

/* Set the scroll speed of the capture effect. */
// NONMATCHING: not byte-compared against the original
void fn_1_4E8D8(f32 arg0)
{
    HH_CAP->scrollSpd = arg0;
}

/* Material hook: projected capture texture + two scrolling sprite layers + indirect warp. */
// NONMATCHING: not byte-compared against the original
void fn_1_4E8E8(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    Mtx camInv;
    Mtx proj;
    Mtx texMtx;
    Mtx scaleMtx;
    Mtx transMtx;
    GXTexObj texObj;
    GXColor color;
    s32 paused;
    HU3DCAMERA *cam = &Hu3DCamera[Hu3DCameraNo];

    paused = omUPauseFlag + omPauseChk();
    if (paused == 0) {
        HH_CAP->scroll = HH_CAP->scroll + HH_CAP->scrollSpd;
    }
    GXInitTexObj(&texObj, HH_CAP->tex, 0x140, 0xF0, 4, 0, 0, 0);
    GXInitTexObjLOD(&texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(&texObj, 1);
    HuSprTexLoad(HH_CAP->spr1, 0, 2, 1, 1, 1);
    HuSprTexLoad(HH_CAP->spr2, 0, 3, 1, 1, 1);
    GXSetNumTexGens(3);
    GXSetNumTevStages(3);
    C_MTXLightPerspective(proj, cam->fov, 1.2f, 0.5f, -0.5f, 0.5f, 0.5f);
    PSMTXInverse(Hu3DCameraMtx, camInv);
    PSMTXConcat(camInv, drawObj->matrix, transMtx);
    PSMTXConcat(proj, Hu3DCameraMtx, texMtx);
    PSMTXConcat(texMtx, transMtx, texMtx);
    GXLoadTexMtxImm(texMtx, 0x21, 0);
    GXSetTexCoordGen2(0, 0, 0, 0x21, 0, 0x7D);
    PSMTXTrans(transMtx, 0.0f, HH_CAP->scroll, 0.0f);
    PSMTXScale(scaleMtx, 0.8f, 0.8f, 1.0f);
    PSMTXConcat(scaleMtx, transMtx, texMtx);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    GXSetTexCoordGen2(1, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(2, 1, 4, 0x3C, 0, 0x7D);
    color.r = HH_CAP->color[0];
    color.g = HH_CAP->color[1];
    color.b = HH_CAP->color[2];
    color.a = HH_CAP->color[3];
    GXSetTevColor(1, color);
    GXSetTevOrder(0, 0, 1, 0);
    GXSetTevColorIn(0, 8, 0xF, 0xF, 2);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(0, 7, 7, 7, 6);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevOrder(1, 2, 0, 0);
    GXSetTevColorIn(1, 0, 8, 2, 0xF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(1, 7, 7, 7, 6);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevOrder(2, 1, 3, 0);
    GXSetTevColorIn(2, 0xF, 8, 3, 0);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(2, 7, 7, 7, 6);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(0, 1, 2);
    GXSetIndTexCoordScale(0, 0, 0);
    GXSetTevIndWarp(0, 0, 1, 0, 1);
    GXSetIndTexMtx(1, HH_CAP->indMtx, -1);
}

/* Project screen position (x, y) at the distance of 'target' from camera 0 into world space. */
// NONMATCHING: not byte-compared against the original
void fn_1_4EDB8(Vec *arg0, Vec *arg1, f32 arg2, f32 arg3)
{
    Vec diff;
    Vec screen;
    Vec world;
    f32 dist;

    PSVECSubtract(&Hu3DCamera[0].pos, arg1, &diff);
    dist = PSVECMag(&diff);
    screen.x = arg2;
    screen.y = arg3;
    screen.z = dist;
    Hu3D2Dto3D((HuVecF *) &screen, 1, (HuVecF *) &world);
    *arg0 = world;
}

/* Easing curve selected by arg0 (0..5) for t in [0,1]. */
// NONMATCHING: not byte-compared against the original
f32 fn_1_4EEA0(s32 arg0, f32 arg1)
{
    f32 t = arg1;
    f32 r = 0.0f;

    if (t >= 1.0f) {
        t = 1.0f;
    }
    switch (arg0) {
    case 0:
        r = (f32) (1.0 - cos(3.141592653589793 * (90.0f * t) / 180.0));
        break;
    case 1:
        r = (f32) sin(3.141592653589793 * (90.0f * t) / 180.0);
        break;
    case 2:
        r = (f32) ((1.0 - cos(3.141592653589793 * (180.0f * t) / 180.0)) * 0.5);
        break;
    case 3:
        if (t < 0.5) {
            r = (f32) sin(3.141592653589793 * (90.0f * t) / 180.0);
        } else {
            r = (f32) (2.0 - cos(3.141592653589793 * (90.0f * t) / 180.0));
        }
        r = r * 0.5f;
        break;
    case 4:
        r = t;
        break;
    case 5:
        r = (f32) sin(3.141592653589793 * (180.0f * t) / 180.0);
        break;
    }
    return r;
}

/* Clear all tween records. */
// NONMATCHING: not byte-compared against the original
void fn_1_4F114(void)
{
    memset(lbl_1_bss_12FC, 0, 0x4B0);
}

/* Start tween 'idx': move from *arg1 to *arg2 over arg3 frames; arg4 = swing amplitude, arg5 = ease/swing mode bits. */
// NONMATCHING: not byte-compared against the original
void fn_1_4F144(s32 arg0, Vec *arg1, Vec *arg2, s32 arg3, f32 arg4, s32 arg5)
{
    HH_Tween *t = &HH_TWEEN[arg0];

    t->cnt = 0;
    t->from = *arg1;
    t->to = *arg2;
    t->dur = arg3;
    t->amp = arg4;
    t->mode = arg5;
}

/* Advance tween 'idx' one frame and write the current position to *arg1. Returns 1 when done. */
// NONMATCHING: not byte-compared against the original
s32 fn_1_4F1EC(s32 arg0, Vec *arg1)
{
    HH_Tween *t = &HH_TWEEN[arg0];
    f32 prog = (f32) t->cnt / (f32) t->dur;
    s32 easeX;
    s32 easeY;
    s32 swing;

    *arg1 = t->from;

    easeX = t->mode & 0xF;
    if (easeX != 0) {
        arg1->x = arg1->x + fn_1_4EEA0(easeX - 1, prog) * (t->to.x - t->from.x);
    }
    easeY = (t->mode & 0xF0) >> 4;
    if (easeY != 0) {
        arg1->y = arg1->y + fn_1_4EEA0(easeY - 1, prog) * (t->to.y - t->from.y);
    }
    swing = (t->mode & 0xF00) >> 8;
    switch (swing) {
    case 0:
        break;
    case 1:
        arg1->x = arg1->x - t->amp * fn_1_4EEA0(5, prog);
        break;
    case 2:
        arg1->x = arg1->x + t->amp * fn_1_4EEA0(5, prog);
        break;
    case 3:
        arg1->y = arg1->y - t->amp * fn_1_4EEA0(5, prog);
        break;
    default:
        arg1->y = arg1->y + t->amp * fn_1_4EEA0(5, prog);
        break;
    }
    t->cnt++;
    if (t->cnt > t->dur) {
        t->cnt = t->dur;
        return 1;
    }
    return 0;
}

/* Show message arg1 in the main window, then wait for it and close the window. */
// NONMATCHING: not byte-compared against the original
void fn_1_4FBC8(CardGame *arg0, s32 arg1)
{
    fn_1_4FBFC(arg0, arg1);
    fn_1_4FFEC();
}

/* Create the main message window and start message arg1 (does not wait). */
// NONMATCHING: not byte-compared against the original
void fn_1_4FBFC(CardGame *arg0, s32 arg1)
{
    HuVec2f size;
    f32 y;
    s32 msgId;
    s16 i;

    if (HH_WIN->disabled != 0) {
        return;
    }
    if (HH_WIN->win != -1) {
        HuWinExKill((s16) HH_WIN->win);
    }
    if (HH_WIN->animSpr != -1) {
        HuWinSprKill((s16) HH_WIN->win, HH_WIN->animSpr);
        HuSprAnimKill(HH_WIN->anim);
    }
    for (i = 0; HH_WIN->ins[i] != -1; i++) {
        HuWinInsertMesSizeGet(HH_WIN->ins[i], i);
    }
    if (HH_WIN->idMode == 0) {
        msgId = lbl_1_data_BA8[arg1 - 1];
    } else {
        msgId = arg1 + 0x13FFFF;
    }
    HuWinMesMaxSizeGet(1, &size, msgId);
    if (HH_WIN->fixedSize != 0 || HH_WIN->idMode != 0) {
        size.x = 448.0f;
        size.y = 94.0f;
    }
    y = 340.0f;
    if (HH_WIN->idMode != 0 && HH_WIN->f34 != 0) {
        y = 48.0f;
    }
    HH_WIN->win = HuWinExCreateFrame(-10000.0f, y, (s32) size.x, (s32) size.y, -1, 1);
    if (HH_WIN->frameSprNo != -1) {
        HH_WIN->anim = HuSprAnimRead(HuDataSelHeapReadNum(HH_WIN->frameSprNo, 0x10000000, 2));
        HH_WIN->animSpr = HuWinAnimSet((s16) HH_WIN->win, HH_WIN->anim, 0, 48.0f, size.y * 0.5f);
    }
    if (HH_WIN->open == 0) {
        HuWinExOpen((s16) HH_WIN->win);
    } else {
        HuWinDispOn((s16) HH_WIN->win);
    }
    HuWinMesPalSet((s16) HH_WIN->win, 7, 0, 0, 0);
    for (i = 0; HH_WIN->ins[i] != -1; i++) {
        HuWinInsertMesSet((s16) HH_WIN->win, HH_WIN->ins[i], i);
    }
    if (HH_WIN->keyWait != 0) {
        fn_1_500D0(arg0, (s16) HH_WIN->win);
    }
    HuWinMesSpeedSet((s16) HH_WIN->win, HH_WIN->speed);
    HuWinMesSet((s16) HH_WIN->win, msgId);
}

/* Wait for the main message to finish, then close and reset the window record. */
// NONMATCHING: not byte-compared against the original
void fn_1_4FFEC(void)
{
    if (HH_WIN->disabled == 0 && HH_WIN->win != -1) {
        HuWinMesWait((s16) HH_WIN->win);
        if (HH_WIN->open == 0) {
            HuWinExClose((s16) HH_WIN->win);
        }
        if (HH_WIN->animSpr != -1) {
            HuWinSprKill((s16) HH_WIN->win, HH_WIN->animSpr);
            HuSprAnimKill(HH_WIN->anim);
        }
        HuWinExKill((s16) HH_WIN->win);
        HuWinComKeyReset();
        fn_1_501B4(1);
    }
}

/* Restrict window input to the current player pad; CPU players auto-press after a delay. */
// NONMATCHING: not byte-compared against the original
void fn_1_500D0(void *arg0, s16 arg1)
{
    HUWIN *win = &winData[arg1];
    s16 i;

    win->padMask = (u8) (1 << GwPlayerConf[HH_REC(arg0)].padNo);
    if (GwPlayerConf[HH_REC(arg0)].type != 0) {
        for (i = 0; i < 10; i++) {
            HuWinComKeyWait(0x100, 0x100, 0x100, 0x100, (s16) GWComKeyDelayGet());
        }
    }
}

/* Reset the message window record; arg0 == 0 also resets the persistent fields. */
// NONMATCHING: not byte-compared against the original
void fn_1_501B4(s32 arg0)
{
    s32 i;

    for (i = 0; i < 8; i++) {
        HH_WIN->ins[i] = -1;
    }
    HH_WIN->win = -1;
    HH_WIN->speed = 1;
    HH_WIN->open = 0;
    HH_WIN->f28 = 0;
    HH_WIN->fixedSize = 0;
    HH_WIN->f34 = 0;
    HH_WIN->animSpr = -1;
    HH_WIN->frameSprNo = -1;
    HH_WIN->anim = NULL;
    if (arg0 == 0) {
        HH_WIN->idMode = 0;
        HH_WIN->keyWait = 0;
        HH_WIN->subWin = -1;
        HH_WIN->disabled = 0;
        HuWinComKeyReset();
    }
}

/* A bare blr in the original binary: returns its first argument unchanged (r3 untouched). */
// NONMATCHING: not byte-compared against the original
s32 fn_1_502DC(s32 arg0)
{
    return arg0;
}

/* Create the secondary message window with message arg0 from the table. */
// NONMATCHING: not byte-compared against the original
void fn_1_502E0(s32 arg0)
{
    HuVec2f size;
    f32 y;

    if (HH_WIN->disabled == 0 && HH_WIN->subWin == -1) {
        HuWinMesMaxSizeGet(1, &size, lbl_1_data_CB4[arg0 - 1]);
        if (HH_WIN->fixedSize == 0) {
            y = 432.0f - size.y;
        } else {
            y = 305.0f;
        }
        HH_WIN->subWin = HuWinCreate(-10000.0f, y, (s32) size.x, (s32) size.y, 0);
        HuWinBGTPLvlSet((s16) HH_WIN->subWin, 0.0f);
        HuWinMesSpeedSet((s16) HH_WIN->subWin, 0);
        HuWinMesSet((s16) HH_WIN->subWin, lbl_1_data_CB4[arg0 - 1]);
    }
}

/* Kill the secondary message window. */
// NONMATCHING: not byte-compared against the original
void fn_1_50460(void)
{
    if (HH_WIN->disabled == 0 && HH_WIN->subWin != -1) {
        HuWinKill((s16) HH_WIN->subWin);
        HH_WIN->subWin = -1;
    }
}
