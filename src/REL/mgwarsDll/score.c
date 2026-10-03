#include "dolphin/types.h"

#include "game/data.h"
#include "game/process.h"
#include "game/sprite.h"

#include "math.h"
#include "string.h"

#include "datanum/mgconst.h"

typedef struct MgScore_s MGSCORE;

MGSCORE *MgScoreCreate(int digitFile, int unitFile, BOOL dispLeadZeroF);
void MgScoreMaxDigitSet(MGSCORE *score, int maxDigit);
void MgScoreDispOn(MGSCORE *score);
void MgScoreColorSet(MGSCORE *score, u8 r, u8 g, u8 b);
void MgScorePosSet(MGSCORE *score, float posX, float posY);
void MgScoreValueSet(MGSCORE *score, int value);

typedef struct MgWarsScoreEntry_s {
    s16 timer;
    s8 state;
    s8 mode;
    s32 value;
    HUSPRGRPID grpId;
    s16 pad0;
} MGWARSSCOREENTRY;

typedef struct MgWarsScoreObj_s {
    char pad0[0xC];
    MGSCORE *score;
} MGWARSSCOREOBJ;

typedef struct MgWarsScorePos_s {
    f32 padX;
    f32 padY;
    f32 x;
    f32 y;
} MGWARSSCOREPOS;

static MGWARSSCOREENTRY mgWarsScoreEntry[4];

static MGWARSSCOREOBJ mgWarsScoreObj[4];

static MGWARSSCOREPOS mgWarsScorePos[4] = {
    { 100.0f, 130.0f, -350.0f, 130.0f },
    { 480.0f, 130.0f, 800.0f, 130.0f },
    { 100.0f, 255.0f, -350.0f, 255.0f },
    { 480.0f, 255.0f, 800.0f, 255.0f },
};

static s32 mgWarsScoreDigitTbl[15] = {
    0x9B002F, 0x9B0030, 0x9B0031, 0x9B0032, 0x9B0033,
    0x9B0034, 0x9B0035, 0x9B0036, 0x9B0037, 0x9B0038,
    0x9B0039, 0x9B003A, 0x9B003B, 0x9B0038, 0x9B0038,
};

static s32 mgWarsScoreAnimTbl[4] = {
    0x9B002B, 0x9B002C, 0x9B002D, 0x9B002E
};

void fn_1_F0B8(void);

// NONMATCHING: the original keeps the MGSCORE pointer inside the same 12-byte
// table as the scalar fields, which C cannot express; the pointer lives in a
// parallel table here, so its index multiply is 16 instead of 12.
void fn_1_EF70(int index, HuVec2f *pos)
{
HuSprGrpPosSet(mgWarsScoreEntry[index].grpId, pos->x, pos->y);

    switch (index) {
    case 0:
    case 1:
        MgScorePosSet(mgWarsScoreObj[index].score, pos->x - 10.0f, 6.0f + pos->y);
        break;

    case 2:
    case 3:
        MgScorePosSet(mgWarsScoreObj[index].score, pos->x - 10.0f, pos->y - 6.0f);
        break;
    }
}

void fn_1_F068(void)
{
    memset(mgWarsScoreEntry, 0, 0x34);
    HuPrcChildCreate(fn_1_F0B8, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

// NONMATCHING: register allocation only - the parallel MGSCORE pointer table
// shifts every callee-saved register assignment in the process body.
void fn_1_F0B8(void)
{
s16 grp;
    f32 x0;
    f32 y0;
    f32 x1;
    f32 y1;
    s32 i;
    s32 j;

    for (;;) {
        switch (mgWarsScoreEntry[0].state) {
        case 0:
            for (i = 0; i < 4; i++) {
                mgWarsScoreEntry[i].grpId = HuSprGrpCreate(1);
                mgWarsScoreObj[i].score = MgScoreCreate(DATANUM(DATA_mgconst, 0x31), -1, 1);
                MgScoreMaxDigitSet(mgWarsScoreObj[i].score, 2);
                MgScoreDispOn(mgWarsScoreObj[i].score);
                MgScoreColorSet(mgWarsScoreObj[i].score, 0xFF, 0xD8, 0x15);
                HuSprGrpMemberSet(mgWarsScoreEntry[i].grpId, 0,
                    HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(mgWarsScoreAnimTbl[i],
                        HU_MEMNUM_OVL, HEAP_MODEL)), 0x64, 0));
                for (j = 0; j < 1; j++) {
                    HuSprAttrSet(mgWarsScoreEntry[i].grpId, j, 1);
                }
                HuSprGrpPosSet(mgWarsScoreEntry[i].grpId, mgWarsScorePos[i].x, mgWarsScorePos[i].y);
                switch (i) {
                case 0:
                case 1:
                    MgScorePosSet(mgWarsScoreObj[i].score,
                        mgWarsScorePos[i].x - 10.0f, 6.0f + mgWarsScorePos[i].y);
                    break;

                case 2:
                case 3:
                    MgScorePosSet(mgWarsScoreObj[i].score,
                        mgWarsScorePos[i].x - 10.0f, mgWarsScorePos[i].y - 6.0f);
                    break;
                }
            }
            mgWarsScoreEntry[0].mode = 2;
            mgWarsScoreEntry[0].state = 1;
            break;

        case 1:
            switch (mgWarsScoreEntry[0].mode) {
            case 0: {
                f32 ratio = (f32)sin(3.141592653589793 * (f64)((f32)mgWarsScoreEntry[0].timer / 60.0f) * 0.5);
                ratio = (f32)sin(3.141592653589793 * (f64)ratio * 0.5);

                if (mgWarsScoreEntry[0].timer > 0x3C) {
                    mgWarsScoreEntry[0].timer = 0;
                    mgWarsScoreEntry[0].mode = 2;
                    break;
                }
                i = 4;
                while (i != 0) {
                    i--;
                    x1 = mgWarsScorePos[i].x + (ratio * (mgWarsScorePos[i].padX - mgWarsScorePos[i].x));
                    y0 = mgWarsScorePos[i].padY + (ratio * (mgWarsScorePos[i].y - mgWarsScorePos[i].padY));
                    HuSprGrpPosSet(mgWarsScoreEntry[i].grpId, x0, y0);
                    switch (i) {
                    case 0:
                    case 1:
                        MgScorePosSet(mgWarsScoreObj[i].score, x0 - 10.0f, 6.0f + y0);
                        break;

                    case 2:
                    case 3:
                        MgScorePosSet(mgWarsScoreObj[i].score, x0 - 10.0f, y0 - 6.0f);
                        break;
                    }
                }
                mgWarsScoreEntry[0].timer++;
                break;
            }

            case 1: {
                f32 ratio = (f32)sin(3.141592653589793 * (f64)((f32)mgWarsScoreEntry[0].timer / 60.0f) * 0.5);
                ratio = (f32)sin(3.141592653589793 * (f64)ratio * 0.5);

                if (mgWarsScoreEntry[0].timer > 0x3C) {
                    mgWarsScoreEntry[0].timer = 0;
                    mgWarsScoreEntry[0].mode = 2;
                    break;
                }
                i = 4;
                while (i != 0) {
                    i--;
                    x0 = mgWarsScorePos[i].padX + (ratio * (mgWarsScorePos[i].x - mgWarsScorePos[i].padX));
                    y0 = mgWarsScorePos[i].padY + (ratio * (mgWarsScorePos[i].y - mgWarsScorePos[i].padY));
                    HuSprGrpPosSet(mgWarsScoreEntry[i].grpId, x0, y0);
                    switch (i) {
                    case 0:
                    case 1:
                        MgScorePosSet(mgWarsScoreObj[i].score, x0 - 10.0f, 6.0f + y0);
                        break;

                    case 2:
                    case 3:
                        MgScorePosSet(mgWarsScoreObj[i].score, x0 - 10.0f, y0 - 6.0f);
                        break;
                    }
                }
                mgWarsScoreEntry[0].timer++;
                break;
            }
            }

            for (i = 0; i < 4; i++) {
                s32 v;
                grp = mgWarsScoreEntry[i].grpId;
                v = mgWarsScoreEntry[i].value;
                if (v > 0x63) {
                    v = 0x63;
                }
                MgScoreValueSet(mgWarsScoreObj[i].score, v);
            }
            break;
        }

        HuPrcVSleep();
    }
}

void fn_1_F808(int index)
{
    mgWarsScoreEntry[index].value += 1;
}

void fn_1_F828(int index)
{
    mgWarsScoreEntry[index].value -= 1;
}

void fn_1_F848(int index, int value)
{
    mgWarsScoreEntry[index].value = value;
}

void fn_1_F860(void)
{
    mgWarsScoreEntry[0].mode = 0;
    mgWarsScoreEntry[0].timer = 0;
}

void fn_1_F884(void)
{
    mgWarsScoreEntry[0].mode = 1;
    mgWarsScoreEntry[0].timer = 0;
}


