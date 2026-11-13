// m527Dll/m527 - team grid minigame (two teams of two, each pair works through a 3x3 floor).
// Every function of the unit is rewritten from the original assembly.

#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/flag.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/init.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/printfunc.h"
#include "game/sprite.h"
#include "game/wipe.h"

#include "math.h"
#include "string.h"

/* Mode of the main sequence (lbl_1_bss_AC). */
#define M527_MODE_WIPEIN 0x3E8
#define M527_MODE_INTRO 0x3E9
#define M527_MODE_CAMERA 0x3EA
#define M527_MODE_START 0x3EB
#define M527_MODE_PLAY 0x3EC
#define M527_MODE_RESULT 0x3ED
#define M527_MODE_END 0x3EE

/* Player state (M527PLAYER::state). */
#define M527_STATE_INTRO 1
#define M527_STATE_PLAY 2
#define M527_STATE_FINISH 3
#define M527_STATE_BLOWN 4 /* the losing team is knocked off the floor */
#define M527_STATE_WIN 5 /* the winners cheer */

/* Per-player work, OMOBJ::data of each player object. Players 0/1 are team 0, 2/3 are team 1. */
typedef struct M527Player_s {
    /* 0x00 */ s32 no; /* slot, team = no / 2 */
    /* 0x04 */ s32 playerNo; /* GwPlayerConf index */
    /* 0x08 */ s32 charNoClamp; /* charNo clamped to 9 */
    /* 0x0C */ s32 charNo;
    /* 0x10 */ s32 padNo; /* -1 for a CPU */
    /* 0x14 */ s32 dif;
    /* 0x18 */ s32 motNo; /* motion currently playing */
    /* 0x1C */ s32 motForce; /* motion forced by the end sequence, -1 for none */
    /* 0x20 */ s32 jumpCharge; /* counts the button presses that lengthen the jump */
    /* 0x24 */ s32 landed;
    /* 0x28 */ s32 stomp; /* stomp (ground pound) in progress */
    /* 0x2C */ s32 stompTime;
    /* 0x30 */ s32 state;
    /* 0x34 */ s32 time;
    /* 0x38 */ s32 scoreTime; /* counts down while the team score sprite fades in */
    /* 0x3C */ f32 speed;
    /* 0x40 */ f32 velX;
    /* 0x44 */ f32 velY;
    /* 0x48 */ f32 velZ;
    /* 0x4C */ s32 aiMode;
    /* 0x50 */ s32 aiCell;
    /* 0x54 */ f32 aiX;
    /* 0x58 */ f32 aiZ;
    /* 0x5C */ s32 aiWait;
    /* 0x60 */ s32 hitTime;
    /* 0x64 */ s32 aiFlag;
    /* 0x68 */ s16 lightId;
    /* 0x6A */ s16 pad6A;
} M527PLAYER; /* size = 0x6C */

/* Queued sound effect plays, emptied once per frame by fn_1_A0D0. */
typedef struct M527Se_s {
    s32 num;
    f32 time[3];
    s32 seNo;
} M527SE;

typedef struct M527Esprite_s {
    s16 memberNo;
    s16 animNo;
} M527ESPRITE;

typedef struct M527EspAnim_s {
    unsigned int dataNum;
    u16 useCnt;
    ANIMDATA *anim;
} M527ESPANIM;

/* esprite.c keeps these private; the minigame reads the sprite animation directly. */
SHARED_SYM extern M527ESPRITE esprite[];
SHARED_SYM extern M527ESPANIM espanim[];
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);
float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);
int rand8(void);

typedef void (*M527VOIDFUNC)(void);

extern const M527VOIDFUNC _ctors[];
extern const M527VOIDFUNC _dtors[];

void fn_1_A0(void);
void fn_1_5B4(OMOBJ *obj);
void fn_1_4F98(OMOBJ *obj);
void fn_1_7E5C(HU3DMODEL *modelP, Mtx *mtx);
void fn_1_87F4(HU3DMODEL *modelP, Mtx *mtx);
void fn_1_94F4(HU3DMODEL *modelP, Mtx *mtx);
void fn_1_9FE8(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
void fn_1_A0D0(OMOBJ *obj);
void fn_1_B8C4(OMOBJ *obj);
void fn_1_C408(OMOBJ *obj);
void fn_1_CB68(OMOBJ *obj);
void ObjectSetup(void);
void fn_1_D198(OMOBJ *obj);
void fn_1_D1F0(OMOBJ *obj);

/* sprite files of the 20 score marks: 2 teams x 10 (5 off, 5 lit) */
s32 lbl_1_data_0[20] = { 0x550015, 0x550015, 0x550015, 0x550015, 0x550015, 0x550016, 0x550016, 0x550016, 0x550016, 0x550016,
    0x550014, 0x550014, 0x550014, 0x550014, 0x550014, 0x550016, 0x550016, 0x550016, 0x550016, 0x550016 };
/* espEntry ids of the 20 score marks */
s32 lbl_1_data_50[33] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x890000, 0x300000, 0xB40000, 0xEA0000, 0xE30000, 0x140000,
    0xDD0000, 0x2A0000, 0xC60000, 0xAD0000, 0, 0, 0 };
/* motion files of the 13 character motions */
u32 lbl_1_data_D4[13] = { 0x8C0000, 0x8C0001, 0x8C0002, 0x8C0003, 0x8C0004, 0x8C0008, 0x8C000A, 0x8C0009, 0x8C001A, 0x8C000E, 0x8C0006, 0x8C0031, 0x8C002B };
/* 1 when the motion does not loop */
u8 lbl_1_data_108[13] = { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 };
/* blend time into the motion */
u8 lbl_1_data_115[15] = { 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 0, 0 };
/* start frame of the motion */
s32 lbl_1_data_124[13] = { 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0 };
/* [0] winning team, -1 until decided, 4 for a draw */
s32 lbl_1_data_158[2] = { -1, 0x708 };
s32 lbl_1_data_160 = -1;
s32 lbl_1_data_164 = 1;
/* score of each team */
s32 lbl_1_data_168[2] = { 0, 0 };
u32 lbl_1_data_170[2] = { 0, 0 };
/* floor cell each player of a team has to reach */
s32 lbl_1_data_178[4] = { 0, 0, 0, 0 };
/* tile timer of each team */
u32 lbl_1_data_188[2] = { 60, 60 };
s32 lbl_1_data_190[4] = { 0, 0, 0, 0 };
s32 lbl_1_data_1A0[4] = { 0, 0, 0, 0 };
s32 lbl_1_data_1B0[4] = { 0, 0, 0, 0 };
s32 lbl_1_data_1C0[4] = { 0, 0, 0, 0 };
s32 lbl_1_data_1D0[2] = { -1, -1 };
/* start position of each player: x, y, z, rotY */
f32 lbl_1_data_1D8[17] = { -200.0f, 0.0f, 0.0f, 170.0f, 200.0f, 0.0f, 0.0f, -170.0f, -200.0f, 0.0f, 0.0f, 170.0f, 200.0f, 0.0f, 0.0f, -170.0f, 1.0f };

f32 lbl_1_bss_0;
s32 lbl_1_bss_8;
s32 lbl_1_bss_10;
f32 lbl_1_bss_14[25];
s16 lbl_1_bss_78[2];
f32 lbl_1_bss_7C[2];
f32 lbl_1_bss_84[2];
s32 lbl_1_bss_8C[4];
s32 lbl_1_bss_9C;
s32 lbl_1_bss_A0;
s16 lbl_1_bss_A4;
s16 lbl_1_bss_A6;
OMOBJMAN *lbl_1_bss_A8;
s32 lbl_1_bss_AC;
OMOBJ *lbl_1_bss_B0;
OMOBJ *lbl_1_bss_B4;
OMOBJ *lbl_1_bss_B8[4];
OMOBJ *lbl_1_bss_C8;
OMOBJ *lbl_1_bss_CC;
M527SE lbl_1_bss_D0[16];
s32 lbl_1_bss_210;

/* The cell of the 3x3 floor (0..8) a position is over. */
#define M527_CELL(x, z) ((s32) ((350.0f + (x)) / 233.33333f) + ((s32) ((350.0f + (z)) / 233.33333f) * 3))

/* Queues one play of the sound effect slot `no`, up to three per frame. */
#define M527_SE_PLAY(no)                                                        \
    do {                                                                        \
        if (lbl_1_bss_D0[no].num < 3) {                                         \
            lbl_1_bss_D0[no].time[lbl_1_bss_D0[no].num] = 0.0f;                 \
            lbl_1_bss_D0[no].num++;                                             \
        }                                                                       \
    } while (0)

s32 _prolog(void)
{
    const M527VOIDFUNC *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const M527VOIDFUNC *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

/*
 * Debug camera: nudges the two camera rigs from the pad. The trigger pair moves
 * the pivot height, the face buttons the zoom and the substick the two angles,
 * then the spherical result is converted to an eye position. The result is not
 * handed to any camera.
 */
void fn_1_A0(void)
{
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    HuVecF up;
    HuVecF target;
    HuVecF pos;
    s32 i;

    for (i = 0; i < 2; i++) {
        CenterM[i].y += 20.0 * (0.0078125 * (HuPadTrigL[0] - HuPadTrigR[0]));
        CZoomM[i] += 25.0 * (((HuPadBtn[0] & 0x400) != 0) - ((HuPadBtn[0] & 0x800) != 0));
        CRotM[i].y += HuPadSubStkX[0] * 0.03125;
        CRotM[i].x += HuPadSubStkY[0] / -32.0;
        rotX = CRotM[i].x;
        rotY = CRotM[i].y;
        rotZ = CRotM[i].z;
        pos.x = CenterM[i].x + (CZoomM[i] * (sin((M_PI * rotY) / 180.0) * cos((M_PI * rotX) / 180.0)));
        pos.y = CenterM[i].y + (CZoomM[i] * -sin((M_PI * rotX) / 180.0));
        pos.z = CenterM[i].z + (CZoomM[i] * (cos((M_PI * rotY) / 180.0) * cos((M_PI * rotX) / 180.0)));
        target.x = CenterM[i].x;
        target.y = CenterM[i].y;
        target.z = CenterM[i].z;
        up.x = sin((M_PI * rotY) / 180.0) * sin((M_PI * rotX) / 180.0);
        up.y = cos((M_PI * rotX) / 180.0);
        up.z = cos((M_PI * rotY) / 180.0) * sin((M_PI * rotX) / 180.0);
    }
}

/* the default inline budget is used up before the sqrtf / fabs calls of the movement code */
#pragma inline_max_total_size(10000)

// NONMATCHING: register/float-register allocation and constant hoisting differ; the original also evaluates dead 0.875*vel products in the motion 3 no-move path (no stores)
/*
 * Player update: reads the pad (or runs the CPU), moves the character, runs the
 * jump / stomp motions and, for the odd member of each team, the tile logic
 * (which cells the team has to stomp, the score and the marks).
 */
void fn_1_5B4(OMOBJ *obj)
{
    s32 partnerTbl[4] = { 1, 0, 3, 2 };
    s32 cellTbl[2][9] = { { 1, 2, 3, 4, 5, 6, 7, 8, 9 }, { 9, 3, 2, 6, 5, 4, 8, 7, 1 } };
    OMOBJ *partnerObj;
    M527PLAYER *pl;
    M527PLAYER *partner;
    f32 stickX;
    f32 stickY;
    f32 grndY;
    f32 btnHeld;
    f32 btnPress;
    f32 vx;
    f32 vz;
    f32 mag;
    f32 dx;
    f32 dz;
    f32 dy;
    f32 radius;
    f32 dist;
    f32 t;
    f32 t2;
    f32 e;
    f32 f;
    f32 g;
    f32 pan;
    f32 vol;
    f64 zScale;
    f32 tileA;
    f32 tileB;
    Vec stick;
    Mtx rotMtx;
    s16 newMot;
    s32 i;
    s32 n;
    s32 team;
    s32 slotA;
    s32 slotB;
    s32 own;
    s32 partnerTarget;
    s32 selfOnTarget;
    s32 cell;
    s32 prevTimer;
    s32 scored;
    s32 sgn;
    s32 btnMask;
    s32 oldA;
    s32 oldB;
    s32 flag;
    s32 flag2;
    s32 flag3;
    f64 stompSpd;
    s32 idx;

    partnerObj = lbl_1_bss_B8[partnerTbl[obj->work[0]]];
    pl = obj->data;
    partner = partnerObj->data;
    stickX = 0.0f;
    stickY = 0.0f;
    btnHeld = 0.0f;
    btnPress = 0.0f;
    newMot = -1;
    grndY = 0.0f;
    lbl_1_data_124[9] = (pl->no % 2) * 60;
    if (pl->motNo != 3) {
        pl->velZ = 0.0f;
        pl->velX = 0.0f;
    }
    pl->time++;
    switch (pl->state) {
        case 0:
        case M527_STATE_FINISH:
            break;
        case M527_STATE_INTRO:
            if (lbl_1_bss_AC == M527_MODE_INTRO) {
                newMot = 0;
                if (lbl_1_bss_0 >= 40.0f && lbl_1_bss_0 < 85.0f && pl->no < 2) {
                    newMot = 8;
                }
            }
            else if (lbl_1_bss_AC == M527_MODE_CAMERA) {
                f32 turn = 2.9833333f;

                if (obj->rot.y < 0.0f) {
                    sgn = -1;
                }
                else {
                    if (obj->rot.y > 0.0f) {
                        n = 1;
                    }
                    else {
                        n = 0;
                    }
                    sgn = n;
                }
                e = sgn;
                obj->rot.y += -e * turn;
                if (obj->rot.y < 0.0f) {
                    sgn = -1;
                }
                else {
                    if (obj->rot.y > 0.0f) {
                        n = 1;
                    }
                    else {
                        n = 0;
                    }
                    sgn = n;
                }
                if (e != (f32) sgn) {
                    t = 0.0f;
                }
                else {
                    t = obj->rot.y;
                }
                obj->rot.y = t;
                newMot = 9;
            }
            else if (lbl_1_bss_AC == M527_MODE_START) {
                newMot = 0;
                if (lbl_1_bss_0 == 2.0f) {
                    pl->motNo = 9;
                }
            }
            break;
        case M527_STATE_PLAY:
            if (pl->padNo != -1) {
                stick.x = 0.0f;
                stick.y = 0.0f;
                stick.z = 0.0f;
                btnHeld = (HuPadBtn[pl->padNo] & 0x100) > 0;
                if (pl->motNo == 3) {
                    btnMask = 0x140;
                }
                else {
                    btnMask = 0x100;
                }
                btnPress = (HuPadBtnDown[pl->padNo] & btnMask) > 0;
                stick.x = HuPadStkX[pl->padNo];
                stick.z = -(f32) HuPadStkY[pl->padNo];
                MTXRotRad(rotMtx, 'y', 0.017453292f * CRotM->y);
                MTXMultVec(rotMtx, &stick, &stick);
                stickX = stick.x;
                stickY = -stick.z;
            }
            else {
                Vec aiStick = { 0.0f, 0.0f, 0.0f };

                if (lbl_1_data_188[pl->no / 2] == 29
                    || (pl->aiMode == 2 && pl->aiCell != M527_CELL(obj->trans.x, obj->trans.z) && pl->aiFlag == 0)) {
                    s32 aiTbl[4][2] = { { 180, 60 }, { 120, 60 }, { 90, 60 }, { 30, 60 } };

                    pl->aiMode = 1;
                    pl->aiWait = aiTbl[pl->dif][0] + (rand8() % aiTbl[pl->dif][1]);
                    own = (pl->no % 2) == 1;
                    if (pl->aiFlag > 0 && cellTbl[1][pl->aiCell] == cellTbl[0][pl->aiCell]) {
                        pl->aiFlag = 0;
                    }
                    pl->aiCell = lbl_1_data_178[own + (pl->no & 2)];
                    partnerTarget = lbl_1_data_178[(own == 0) + (pl->no & 2)];
                    if (partner->padNo == -1 && pl->dif <= 1) {
                        selfOnTarget = partnerTarget == M527_CELL(obj->trans.x, obj->trans.z);
                    }
                    if (pl->dif <= 1 && pl->aiFlag > 0) {
                        if (partnerTarget == cellTbl[pl->aiFlag > 0][pl->aiCell] - 1) {
                            pl->aiFlag = 0;
                        }
                        else if (M527_CELL(partnerObj->trans.x, partnerObj->trans.z) == cellTbl[pl->aiFlag > 0][pl->aiCell] - 1) {
                            pl->aiFlag = 0;
                        }
                    }
                }
                if (pl->aiMode == 3) {
                    if (pl->aiWait == 0) {
                        dx = pl->aiX - obj->trans.x;
                        dz = pl->aiZ - obj->trans.z;
                        stickX = dx / 0.25;
                        stickY = -dz / 0.25;
                        if ((dx * dx) + (dz * dz) < 900.0f) {
                            pl->aiMode = 0;
                            pl->aiWait = 0;
                        }
                        else if (pl->hitTime > 120) {
                            pl->aiMode = 0;
                            pl->aiWait = 0;
                        }
                    }
                    else {
                        pl->aiWait--;
                    }
                }
                else if (pl->aiMode == 1) {
                    if (pl->aiWait == 0) {
                        dx = pl->aiX - obj->trans.x;
                        dz = pl->aiZ - obj->trans.z;
                        stickX = dx / 0.25;
                        stickY = -dz / 0.25;
                        if ((dx * dx) + (dz * dz) < 2704.0f) {
                            pl->aiMode = 2;
                            pl->aiWait = 0;
                        }
                        else if (pl->hitTime > (pl->no * 5) + 120) {
                            pl->hitTime = 0;
                            pl->aiFlag = 0;
                            if (pl->aiCell == lbl_1_data_178[(pl->no & 2) + ((pl->no % 2) == 1)]) {
                                n = lbl_1_data_178[(pl->no & 2) + ((pl->no % 2) == 0)];
                            }
                            else {
                                n = lbl_1_data_178[(pl->no & 2) + ((pl->no % 2) == 1)];
                            }
                            pl->aiCell = n;
                            pl->aiX = (700.0f * (f32) (((cellTbl[pl->aiFlag > 0][pl->aiCell] - 1) % 3) - 1)) / 3.0f;
                            pl->aiZ = (700.0f * (f32) (((cellTbl[pl->aiFlag > 0][pl->aiCell] - 1) / 3) - 1)) / 3.0f;
                        }
                    }
                    else {
                        pl->aiWait--;
                        if (pl->aiWait == 0) {
                            if (partner->padNo != -1
                                && pl->aiCell == M527_CELL(lbl_1_bss_B8[pl->no ^ 1]->trans.x, lbl_1_bss_B8[pl->no ^ 1]->trans.z)) {
                                if (pl->aiCell == lbl_1_data_178[pl->no & 2]) {
                                    n = lbl_1_data_178[(pl->no & 2) + 1];
                                }
                                else {
                                    n = lbl_1_data_178[pl->no & 2];
                                }
                                pl->aiCell = n;
                            }
                            if (pl->dif <= 1 && pl->aiFlag > 0
                                && M527_CELL(partnerObj->trans.x, partnerObj->trans.z) == cellTbl[pl->aiFlag > 0][pl->aiCell] - 1) {
                                pl->aiFlag = 0;
                            }
                            pl->aiX = (700.0f * (f32) (((cellTbl[pl->aiFlag > 0][pl->aiCell] - 1) % 3) - 1)) / 3.0f;
                            pl->aiZ = (700.0f * (f32) (((cellTbl[pl->aiFlag > 0][pl->aiCell] - 1) / 3) - 1)) / 3.0f;
                        }
                    }
                }
                else if (pl->aiMode == 2) {
                    switch (pl->aiWait % 75) {
                        case 1:
                            if (partner->padNo == -1 || pl->dif <= 1 || partner->motNo == 3 || partner->motNo == 5
                                || (pl->aiMode == 2 && pl->aiWait == 0)) {
                                pl->aiWait++;
                                if (partner->motNo == 5) {
                                    btnPress = 1.0f;
                                    btnHeld = 1.0f;
                                    pl->aiWait = 15;
                                }
                            }
                            break;
                        case 8:
                            btnPress = 1.0f;
                            btnHeld = 1.0f;
                            pl->aiWait++;
                            break;
                        case 17:
                            btnHeld = 0.0f;
                            pl->aiWait++;
                            break;
                        case 18:
                            btnPress = 1.0f;
                            btnHeld = 1.0f;
                            pl->aiWait++;
                            break;
                        case 54:
                            if (pl->aiFlag == 1
                                || ((partner->motNo == 0 || pl->aiWait > 200)
                                    && pl->aiCell == M527_CELL(lbl_1_bss_B8[pl->no ^ 1]->trans.x, lbl_1_bss_B8[pl->no ^ 1]->trans.z))) {
                                own = (pl->no % 2) == 1;
                                pl->aiCell = lbl_1_data_178[own + (pl->no & 2)];
                                partnerTarget = lbl_1_data_178[(own == 0) + (pl->no & 2)];
                                pl->aiMode = 1;
                                if (pl->aiFlag != 0) {
                                    n = 60;
                                }
                                else {
                                    n = 0;
                                }
                                pl->aiWait = n + 20;
                                if (pl->aiFlag > 0) {
                                    n = -1;
                                }
                                else {
                                    n = 0;
                                }
                                pl->aiFlag += n;
                            }
                            else {
                                pl->aiWait++;
                            }
                            break;
                        case 55:
                            if (pl->padNo == -1 && partner->padNo == -1 && pl->no % 2 == 1) {
                                pl->aiWait += 14;
                            }
                            /* fallthrough */
                        default:
                            btnHeld = (pl->aiWait % 75) < 13;
                            pl->aiWait++;
                            break;
                    }
                }
                aiStick.x = stickX;
                aiStick.z = -stickY;
                MTXRotRad(rotMtx, 'y', 0.01745329238474369 * (45.0 * sin((M_PI * (((pl->hitTime * 3) % 360) + (((rand8() % 2) * 2) - 1))) / 180.0)));
                MTXMultVec(rotMtx, &aiStick, &aiStick);
                stickX = aiStick.x;
                stickY = -aiStick.z;
            }
            break;
        case M527_STATE_BLOWN:
            if (lbl_1_bss_AC == M527_MODE_RESULT) {
                if (lbl_1_data_158[0] == 1 - (pl->no / 2) && pl->motNo == 8) {
                    if (lbl_1_bss_0 >= 95.0f) {
                        grndY = -20.0f * (lbl_1_bss_0 - 95.0f);
                        obj->trans.y = grndY;
                        Hu3DModelAttrSet(obj->mdlId[0], HU3D_MOTATTR_PAUSE);
                        if (lbl_1_bss_0 == 95.0f) {
                            CharFXPlay(pl->charNo, 0x1F9);
                        }
                        if (lbl_1_bss_0 == 95.0f) {
                            if (pl->no % 2 == 0) {
                                M527_SE_PLAY((pl->no / 2) + 13);
                            }
                        }
                    }
                }
            }
            if (lbl_1_bss_AC == M527_MODE_RESULT) {
                if (lbl_1_data_158[0] == 1 - (pl->no / 2)) {
                    if (lbl_1_bss_0 >= 120.0f) {
                        Hu3DModelAttrSet(obj->mdlId[0], HU3D_ATTR_DISPOFF);
                    }
                }
            }
            if (lbl_1_bss_AC >= M527_MODE_END) {
                if (lbl_1_data_158[0] == 1 - (pl->no / 2)) {
                    newMot = 12;
                    Hu3DMotionSpeedSet(obj->mdlId[0], 0.8f);
                    Hu3DModelLayerSet(obj->mdlId[0], 3);
                    Hu3DLLightPosSet(obj->mdlId[0], pl->lightId, 0.0f, -500.0f, 500.0f, 0.0f, 1.0f, -1.0f);
                    Hu3DLLightColorSet(obj->mdlId[0], pl->lightId, 0x80, 0x80, 0x80, 0xFF);
                    if (obj->trans.x > 0.0f) {
                        sgn = 1;
                    }
                    else {
                        sgn = -1;
                    }
                    obj->trans.x = obj->trans.x - (0.75 * (sgn * 2));
                    obj->trans.z = obj->trans.z - 3.0;
                    obj->rot.x = obj->rot.x + 1.875;
                    if (lbl_1_bss_AC == M527_MODE_END && lbl_1_bss_0 <= 10.0f) {
                        Hu3DModelAttrSet(obj->mdlId[0], HU3D_ATTR_DISPOFF);
                    }
                    else {
                        Hu3DModelAttrReset(obj->mdlId[0], HU3D_ATTR_DISPOFF);
                    }
                }
            }
            break;
        case M527_STATE_WIN:
            obj->rot.y = obj->rot.y * 0.3;
            if (lbl_1_bss_AC == M527_MODE_END && pl->motNo == 0) {
                if (lbl_1_data_158[0] == pl->no / 2) {
                    n = 10;
                }
                else {
                    n = 11;
                }
                pl->motForce = n;
            }
            break;
    }
    if (lbl_1_bss_AC == M527_MODE_RESULT) {
        if (lbl_1_data_158[0] == 1 - (pl->no / 2)) {
            if (lbl_1_bss_0 >= 50.0f) {
                if (lbl_1_bss_0 <= 70.0f) {
                    if (lbl_1_bss_0 < 50.0f) {
                        t = 0.0f;
                    }
                    else {
                        if (lbl_1_bss_0 > 70.0f) {
                            t2 = 1.0f;
                        }
                        else {
                            t2 = (lbl_1_bss_0 - 50.0f) / 20.0f;
                        }
                        t = t2;
                    }
                    f = 0.3 + (0.7 * (1.0f - (1.0f - ((1.0f - t) * (1.0f - t)))));
                    Hu3DLLightColorSet(obj->mdlId[0], pl->lightId, (u8) (s32) ((2.0f * (32.0f * f)) / 3.0f),
                        (u8) (s32) ((2.0f * (255.0f * f)) / 3.0f), (u8) (s32) ((2.0f * (200.0f * f)) / 3.0f), 255);
                }
            }
        }
    }
    if (pl->state >= M527_STATE_PLAY) {
        switch (pl->motNo) {
            case 7:
                if (Hu3DData[obj->mdlId[0]].motIdShift == -1 && CharMotionEndCheck(pl->charNo)) {
                    newMot = 0;
                }
                break;
            case 4:
                if (CharMotionEndCheck(pl->charNo)) {
                    newMot = 0;
                }
                /* fallthrough */
            case 0:
            case 1:
            case 2:
                vx = 0.25 * stickX;
                vz = 0.25 * -stickY;
                mag = sqrtf((vx * vx) + (vz * vz));
                if (mag >= 0.5f) {
                    if (mag < 10.0f) {
                        pl->velX = vx;
                        pl->velZ += vz;
                    }
                    else {
                        vx /= mag;
                        vz /= mag;
                        mag = 10.0f;
                        if (pl->motNo != 3) {
                            pl->velX = vx * mag;
                            pl->velZ = vz * mag;
                        }
                        else {
                            pl->velX = (0.875 * pl->velX) + (0.125 * (vx * mag));
                            pl->velZ = (0.875 * pl->velZ) + (0.125 * (vz * mag));
                        }
                    }
                    pl->speed = mag;
                    obj->rot.y = 180.0 * (atan2(pl->velX, pl->velZ) / M_PI);
                    if (pl->speed >= 8.0f) {
                        newMot = 2;
                    }
                    else {
                        newMot = 1;
                    }
                }
                else if (pl->motNo != 7 && pl->motNo != 4 && pl->motNo != 8) {
                    newMot = 0;
                }
                pl->velY = 0.0f;
                if (lbl_1_bss_AC == M527_MODE_RESULT && lbl_1_data_158[0] == 1 - (pl->no / 2) && lbl_1_bss_0 == 70.0f) {
                    newMot = 8;
                    pl->state = M527_STATE_BLOWN;
                }
                else if (grndY != 0.0f && obj->trans.y > grndY) {
                    pl->landed = 0;
                    pl->jumpCharge = 1;
                    pl->velY = 0.0f;
                    newMot = 3;
                }
                else if (btnPress != 0.0f) {
                    pl->landed = 0;
                    pl->jumpCharge = 1;
                    pl->velY = 26.766666f;
                    newMot = 3;
                }
                break;
            case 3:
                vx = 0.25 * stickX;
                vz = 0.25 * -stickY;
                mag = sqrtf((vx * vx) + (vz * vz));
                if (mag >= 0.5f) {
                    if (mag < 10.0f) {
                        pl->velX = vx;
                        pl->velZ = vz;
                    }
                    else {
                        vx /= mag;
                        vz /= mag;
                        mag = 10.0f;
                        if (pl->motNo != 3) {
                            pl->velX = vx * mag;
                            pl->velZ = vz * mag;
                        }
                        else {
                            pl->velX = (0.875 * pl->velX) + (0.125 * (vx * mag));
                            pl->velZ = (0.875 * pl->velZ) + (0.125 * (vz * mag));
                        }
                    }
                    pl->speed = mag;
                    obj->rot.y = 180.0 * (atan2(pl->velX, pl->velZ) / M_PI);
                }
                if (pl->jumpCharge != 0) {
                    if (btnHeld != 0.0f) {
                        pl->jumpCharge++;
                        if (pl->jumpCharge > 4) {
                            pl->jumpCharge = 0;
                        }
                        else {
                            pl->velY += 4.866667f;
                        }
                    }
                    else {
                        pl->jumpCharge = 0;
                    }
                }
                pl->velY += -2.4333334f;
                if (pl->landed != 0) {
                    pl->velY = 0.0f;
                    pl->landed = 1;
                    newMot = 4;
                }
                else if (btnPress != 0.0f) {
                    pl->velY = 0.0f;
                    pl->stomp = 0;
                    newMot = 5;
                    pl->stompTime = 0;
                }
                break;
            case 5:
                if (Hu3DData[obj->mdlId[0]].motIdShift == -1 && CharMotionEndCheck(pl->charNo)) {
                    if (pl->stompTime == 6) {
                        stompSpd = 0.5;
                    }
                    else {
                        if (pl->stompTime >= 6) {
                            n = 1;
                        }
                        else {
                            n = 0;
                        }
                        stompSpd = n;
                    }
                    pl->velY = -26.766666412353516 * stompSpd;
                    pl->stompTime++;
                }
                else {
                    pl->velY = 0.0f;
                }
                if (pl->velY <= 0.0f) {
                    if (obj->trans.y <= grndY) {
                        newMot = 6;
                        pl->stomp = 1;
                        obj->trans.y = grndY;
                        pl->velY = 0.0f;
                        if (pl->padNo != -1) {
                            omVibrate(pl->playerNo, 12, 6, 6);
                        }
                        if (pl->state == M527_STATE_PLAY && lbl_1_bss_AC == M527_MODE_PLAY) {
                            if (obj->trans.x < -116.666664f) {
                                n = 0;
                            }
                            else {
                                if (obj->trans.x < 116.666664f) {
                                    cell = 1;
                                }
                                else {
                                    cell = 2;
                                }
                                n = cell;
                            }
                            lbl_1_data_1A0[pl->no] = n;
                            if (obj->trans.z < -116.666664f) {
                                n = 0;
                            }
                            else {
                                if (obj->trans.z < 116.666664f) {
                                    cell = 1;
                                }
                                else {
                                    cell = 2;
                                }
                                n = cell;
                            }
                            lbl_1_data_1A0[pl->no] += n * 3;
                            scored = 1;
                            if (lbl_1_data_1A0[pl->no] != lbl_1_data_178[pl->no & 2] && lbl_1_data_1A0[pl->no] != lbl_1_data_178[(pl->no & 2) + 1]) {
                                scored = 0;
                            }
                            if (scored != 0) {
                                n = 1;
                            }
                            else {
                                n = -1;
                            }
                            lbl_1_data_1C0[pl->no] = n;
                            if (lbl_1_data_1C0[pl->no] == 1) {
                                n = 30;
                            }
                            else {
                                n = 20;
                            }
                            lbl_1_data_1B0[pl->no] = n;
                            if (lbl_1_data_1C0[pl->no] == 1) {
                                M527_SE_PLAY((pl->no / 2) + 5);
                            }
                            if (lbl_1_data_1C0[pl->no] == -1) {
                                M527_SE_PLAY((pl->no / 2) + 11);
                            }
                        }
                    }
                }
                break;
            case 6:
                newMot = 6;
                if (Hu3DData[obj->mdlId[0]].motIdShift == -1 && CharMotionEndCheck(pl->charNo)) {
                    pl->velY = grndY;
                    pl->stomp = 0;
                    newMot = 7;
                }
                break;
            case 10:
            case 11:
                obj->rot.y = 2.4f + (0.8f * obj->rot.y);
                break;
        }
    }
    obj->trans.x += pl->velX;
    if (pl->landed == 0) {
        obj->trans.y += pl->velY;
    }
    obj->trans.z += pl->velZ;
    dx = lbl_1_bss_B8[partnerTbl[pl->no]]->trans.x - obj->trans.x;
    dz = lbl_1_bss_B8[partnerTbl[pl->no]]->trans.z - obj->trans.z;
    dy = fabs(lbl_1_bss_B8[partnerTbl[pl->no]]->trans.y - obj->trans.y);
    if (dy > 160.0f) {
        radius = 0.0f;
    }
    else {
        if (dy > 110.0f) {
            t = 50.0f * (1.0f - ((dy - 110.0f) / 50.0f));
        }
        else {
            t = 50.0f;
        }
        radius = t;
    }
    if (radius != 0.0f) {
        if (dx == 0.0f && dz == 0.0f) {
            dz = -1.0f;
        }
        dist = sqrtf((dx * dx) + (dz * dz));
        if (dist < 50.0f + radius) {
            obj->trans.x = lbl_1_bss_B8[partnerTbl[pl->no]]->trans.x - (dx * ((50.0f + radius) / dist));
            obj->trans.z = lbl_1_bss_B8[partnerTbl[pl->no]]->trans.z - (dz * ((50.0f + radius) / dist));
            pl->hitTime++;
        }
        else {
            pl->hitTime = 0;
        }
    }
    else {
        pl->hitTime = 0;
    }
    if (pl->state != M527_STATE_BLOWN) {
        if (obj->trans.x < -300.0f) {
            t = -300.0f;
        }
        else {
            if (obj->trans.x > 300.0f) {
                t2 = 300.0f;
            }
            else {
                t2 = obj->trans.x;
            }
            t = t2;
        }
        obj->trans.x = t;
        if (obj->trans.z < -300.0f) {
            t = -300.0f;
        }
        else {
            if (obj->trans.z > 300.0f) {
                t2 = 300.0f;
            }
            else {
                t2 = obj->trans.z;
            }
            t = t2;
        }
        obj->trans.z = t;
        if (obj->trans.x == lbl_1_bss_B8[pl->no ^ 1]->trans.x && obj->trans.y == lbl_1_bss_B8[pl->no ^ 1]->trans.y
            && obj->trans.z == lbl_1_bss_B8[pl->no ^ 1]->trans.z) {
            f32 sq = (obj->trans.x * obj->trans.x) + (obj->trans.z * obj->trans.z);

            dist = sqrtf(sq);
            obj->trans.x = obj->trans.x - (2.0f * (50.0f * (obj->trans.x / dist)));
            obj->trans.z = obj->trans.z - (2.0f * (50.0f * (obj->trans.x / dist)));
        }
    }
    if (obj->trans.y < grndY) {
        obj->trans.y = grndY;
        pl->landed = 1;
    }
    if (pl->motForce >= 0) {
        newMot = pl->motForce;
    }
    if (newMot >= 0 && newMot != pl->motNo) {
        Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[newMot], lbl_1_data_124[newMot], lbl_1_data_115[newMot],
            lbl_1_data_108[newMot] != 0 ? 0 : HU3D_MOTATTR_LOOP);
        pl->motNo = newMot;
    }
    lbl_1_data_1B0[pl->no] -= lbl_1_data_1B0[pl->no] > 0;
    if (lbl_1_bss_AC >= M527_MODE_PLAY) {
        if (pl->no % 2 == 1) {
            team = pl->no / 2;
            slotA = team * 2;
            slotB = slotA + 1;
            prevTimer = lbl_1_data_188[team];
            if (lbl_1_data_188[team] == 30 && lbl_1_bss_AC == M527_MODE_RESULT && lbl_1_bss_0 == 30.0f) {
                lbl_1_data_188[team] = 31;
                Hu3DMotionTimeSet(obj->mdlId[lbl_1_data_178[slotA] + 2], Hu3DMotionMaxTimeGet(obj->mdlId[lbl_1_data_178[slotA] + 2]));
                Hu3DMotionTimeSet(obj->mdlId[lbl_1_data_178[slotB] + 2], Hu3DMotionMaxTimeGet(obj->mdlId[lbl_1_data_178[slotB] + 2]));
            }
            lbl_1_data_170[team] += lbl_1_data_170[team] < 0xFFFFFFFF;
            flag3 = 0;
            flag2 = 0;
            flag = 0;
            if (lbl_1_data_188[team] != 30 && lbl_1_data_188[team] < 0xFFFFFFFF) {
                flag = 1;
            }
            if (flag != 0) {
                n = 0;
                if (lbl_1_data_188[team] == 59 && lbl_1_data_158[0] != -1) {
                    n = 1;
                }
                if (n == 0) {
                    flag2 = 1;
                }
            }
            if (flag2 != 0) {
                n = 1;
                if (lbl_1_data_188[team] == 31) {
                    t = Hu3DMotionMaxTimeGet(obj->mdlId[lbl_1_data_178[pl->no] + 2]) - 1.0f;
                    if (!(Hu3DMotionTimeGet(obj->mdlId[lbl_1_data_178[pl->no] + 2]) >= t)) {
                        n = 0;
                    }
                }
                if (n != 0) {
                    flag3 = 1;
                }
            }
            lbl_1_data_188[team] += flag3;
            if (lbl_1_data_188[team] >= 60) {
                oldA = lbl_1_data_178[slotA];
                oldB = lbl_1_data_178[slotB];
                Hu3DMotionTimeSet(obj->mdlId[lbl_1_data_178[slotA] + 2], 0.0f);
                Hu3DMotionTimeSet(obj->mdlId[lbl_1_data_178[slotB] + 2], 0.0f);
                lbl_1_data_188[team] = 0;
                do {
                    n = rand8() % 9;
                    lbl_1_data_178[slotB] = n;
                    lbl_1_data_178[slotA] = n;
                    while (lbl_1_data_178[slotA] == lbl_1_data_178[slotB]) {
                        lbl_1_data_178[slotB] = rand8() % 9;
                    }
                } while (oldA == lbl_1_data_178[slotA] || oldB == lbl_1_data_178[slotB]);
                n = (rand8() % 3) - 1;
                lbl_1_data_190[slotB] = n;
                lbl_1_data_190[slotA] = n;
                while (lbl_1_data_190[slotA] == lbl_1_data_190[slotB]) {
                    lbl_1_data_190[slotB] = (rand8() % 3) - 1;
                }
            }
            for (i = 0; i < 2; i++) {
                if (Hu3DMotionTimeGet(obj->mdlId[lbl_1_data_178[pl->no - i] + 2]) >= 58.0f && lbl_1_data_188[team] == 30) {
                    Hu3DMotionTimeSet(obj->mdlId[lbl_1_data_178[pl->no - i] + 2], 0.0f);
                }
            }
            if (lbl_1_data_158[0] == -1 && lbl_1_data_1B0[slotA] != 0 && lbl_1_data_1B0[slotB] != 0 && lbl_1_data_188[team] == 30
                && lbl_1_data_188[team] == 30 && lbl_1_data_1B0[slotA] > 0 && lbl_1_data_1C0[slotA] == 1 && lbl_1_data_1B0[slotB] > 0
                && lbl_1_data_1C0[slotB] == 1 && lbl_1_data_1A0[slotA] != lbl_1_data_1A0[slotB]) {
                lbl_1_data_188[team] = 31;
                lbl_1_data_168[team]++;
                if (lbl_1_data_168[team] == 5) {
                    lbl_1_data_158[0] = team;
                }
                pl->scoreTime = 30;
                pl->aiMode = 0;
                partner->aiMode = 0;
                if (pl->dif == 0) {
                    n = (rand8() % 10) < 5;
                }
                else {
                    if (pl->dif == 1) {
                        flag = (rand8() % 10) < 3;
                    }
                    else {
                        flag = 0;
                    }
                    n = flag;
                }
                pl->aiFlag = n;
                if (partner->dif == 0) {
                    n = (rand8() % 10) < 5;
                }
                else {
                    if (partner->dif == 1) {
                        flag = (rand8() % 10) < 3;
                    }
                    else {
                        flag = 0;
                    }
                    n = flag;
                }
                partner->aiFlag = n;
                if (pl->padNo == -1 && partner->padNo != -1) {
                    pl->aiMode = 3;
                    pl->aiWait = 60;
                    if (obj->trans.x < 0.0f) {
                        sgn = -1;
                    }
                    else {
                        sgn = 1;
                    }
                    pl->aiX = 116.666664f * sgn;
                    if (obj->trans.z < 0.0f) {
                        sgn = -1;
                    }
                    else {
                        sgn = 1;
                    }
                    pl->aiZ = 116.666664f * sgn;
                }
                if (partner->padNo == -1 && pl->padNo != -1) {
                    partner->aiMode = 3;
                    partner->aiWait = 60;
                    if (partnerObj->trans.x < 0.0f) {
                        sgn = -1;
                    }
                    else {
                        sgn = 1;
                    }
                    partner->aiX = 116.666664f * sgn;
                    if (partnerObj->trans.z < 0.0f) {
                        sgn = -1;
                    }
                    else {
                        sgn = 1;
                    }
                    partner->aiZ = 116.666664f * sgn;
                }
                Hu3DMotionTimeSet(obj->mdlId[lbl_1_data_178[slotA] + 2], 60.0f);
                Hu3DMotionTimeSet(obj->mdlId[lbl_1_data_178[slotB] + 2], 60.0f);
                M527_SE_PLAY(team + 7);
            }
            if (prevTimer == 0 && lbl_1_data_188[team] == 1) {
                M527_SE_PLAY(team + 1);
            }
            if (prevTimer == 31 && lbl_1_data_188[team] == 31) {
                t = Hu3DMotionMaxTimeGet(obj->mdlId[lbl_1_data_178[pl->no] + 2]) - 85.0f;
                if (Hu3DMotionTimeGet(obj->mdlId[lbl_1_data_178[pl->no] + 2]) == t) {
                    M527_SE_PLAY(team + 9);
                }
            }
            if (prevTimer == 32 && lbl_1_data_188[team] == 33) {
                M527_SE_PLAY(team + 3);
            }
            for (i = 0; i < 9; i++) {
                Hu3DModelAttrSet(obj->mdlId[i + 2], HU3D_ATTR_DISPOFF);
            }
            if (lbl_1_data_188[team] <= 30) {
                e = lbl_1_data_188[team] / 30.0f;
            }
            else {
                if (lbl_1_data_188[team] <= 33) {
                    t = 1.0f;
                }
                else {
                    t = 1.0f - ((f32) (lbl_1_data_188[team] - 33) / 27.0f);
                }
                e = t;
            }
            tileA = e;
            tileB = 1.0f - e;
            for (i = 0; i < 2; i++) {
                if (lbl_1_bss_AC <= M527_MODE_RESULT) {
                    Hu3DModelAttrReset(obj->mdlId[lbl_1_data_178[slotA + i] + 2], HU3D_ATTR_DISPOFF);
                }
                if (lbl_1_data_190[slotA + i] == 0) {
                    zScale = 1.7;
                }
                else {
                    zScale = 1.0;
                }
                Hu3DModelPosSet(obj->mdlId[lbl_1_data_178[slotA + i] + 2], ((i * 2) - 1) * (222.0 + (tileB * (tileB * (500.0 * zScale)))),
                    340.0f + (tileB * (tileB * (f32) (lbl_1_data_190[slotA + i] * 500))), -850.0f - (100.0f * (1.0f - (tileA * tileA))));
            }
            if (pl->scoreTime > 0 && lbl_1_data_168[team] <= 5) {
                f = 1.0 - ((pl->scoreTime - 1) / 30.0);
                idx = lbl_1_data_168[team] + (team * 10) + 4;
                g = (0.5 * f) + (0.5 * (1.0f - ((1.0f - f) * (1.0f - f))));
                if (g < 0.75) {
                    stompSpd = g / 0.75;
                }
                else {
                    stompSpd = 1.0;
                }
                t = stompSpd;
                e = 1.0f - ((1.0f - g) * (1.0f - g));
                t = 1.0f - ((1.0f - t) * (1.0f - t));
                if (pl->scoreTime == 30) {
                    espDispOn(lbl_1_data_50[idx]);
                    espDispOff(lbl_1_data_50[lbl_1_data_168[team] + (team * 10) - 1]);
                }
                espTPLvlSet(lbl_1_data_50[idx], t);
                espScaleSet(lbl_1_data_50[idx], 4.0f - (3.0f * e), 4.0f - (3.0f * e));
                pl->scoreTime--;
            }
        }
    }
    if (lbl_1_bss_AC == M527_MODE_RESULT) {
        if (lbl_1_data_158[0] == 1 - (pl->no / 2)) {
            if (lbl_1_bss_0 <= 20.0f) {
                if ((s32) lbl_1_bss_0 % 2 == 0) {
                    Hu3DModelAttrSet(obj->mdlId[1], HU3D_ATTR_DISPOFF);
                }
                else {
                    Hu3DModelAttrReset(obj->mdlId[1], HU3D_ATTR_DISPOFF);
                }
            }
        }
    }
    Hu3DModelPosSet(obj->mdlId[1], obj->trans.x, 0.0f, obj->trans.z);
    pan = obj->trans.x / 350.0f;
    vol = obj->trans.z / 350.0f;
    if (pan < -1.0f) {
        t = -1.0f;
    }
    else {
        if (pan > 1.0f) {
            t2 = 1.0f;
        }
        else {
            t2 = pan;
        }
        t = t2;
    }
    pan = t;
    if (vol < -1.0f) {
        t = -1.0f;
    }
    else {
        if (vol > 1.0f) {
            t2 = 1.0f;
        }
        else {
            t2 = vol;
        }
        t = t2;
    }
    vol = t;
    if (lbl_1_bss_AC > M527_MODE_CAMERA && lbl_1_bss_AC != M527_MODE_END) {
        if (pl->no < 2) {
            sgn = -1;
        }
        else {
            sgn = 1;
        }
        pan = (0.5 * pan) + (0.5 * sgn);
    }
    CharModelVoicePanSet(pl->charNo, 64.0f + (16.0f * pan));
    CharModelVoiceVolSet(pl->charNo, 127.0f + (15.0f * ((0.5f * vol) - 0.5f)));
}

s32 lbl_1_data_24C = 0x8A;

// NONMATCHING: register allocation and scheduling differ (the original re-reads lbl_1_data_158[0] and keeps a double for the ease value)
/*
 * Per-frame update of the stage: camera and light animation of each mode, the
 * float of the winner's trophy model and the colour of the character lights.
 */
void fn_1_4F98(OMOBJ *obj)
{
    M527PLAYER *player[4];
    f32 col[3][3][3] = { 0 };
    f32 t;
    f32 t2;
    f32 e;
    f32 k;
    f32 half;
    f32 c1;
    f32 c2;
    f32 x1;
    f32 x2;
    f32 y1;
    f32 y2;
    f32 sc0;
    f32 sc1;
    f32 sc2;
    f32 sc3;
    f32 zoom;
    f32 posY;
    f32 fx;
    f32 fz;
    f32 frx;
    f32 frz;
    f32 wx[2];
    f32 wz[2];
    f32 sumR;
    f32 sumG;
    f32 sumB;
    s32 xi[2];
    s32 zi[2];
    s32 ix;
    s32 iz;
    s32 w;
    s32 side;
    s32 i;
    s32 j;
    s32 a;
    s32 b;
    s32 team;
    s32 n;
    s32 cell;
    s32 c;
    s32 cB;
    s32 cA;
    s32 camNo;
    s32 *cellP;

    for (i = 0; i < 4; i++) {
        player[i] = lbl_1_bss_B8[i]->data;
    }
    if (lbl_1_bss_AC == M527_MODE_INTRO) {
        t = lbl_1_bss_0 / 115.0f;
        if (t > 1.0f) {
            e = 1.0;
        }
        else {
            if (t >= 0.75f) {
                e = 0.66 + (0.34 * ((t - 0.75) / 0.25));
            }
            else {
                e = 0.66 * (t / 0.75);
            }
        }
        if (lbl_1_bss_0 <= 5.0f) {
            Hu3DMotionTimeSet(obj->mdlId[5], 0.0f);
        }
        Hu3DModelPosSet(obj->mdlId[5], 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(obj->mdlId[5], 1.0f, 1.0f, 1.0f);
        Hu3DModelRotSet(obj->mdlId[5], 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrReset(obj->mdlId[5], HU3D_ATTR_DISPOFF);
        for (i = 0; i < 4; i++) {
            s32 tbl[4] = { 1, 4, 2, 3 };

            Hu3DLLightPosSet(obj->mdlId[tbl[i]], lbl_1_bss_8C[i], 0.0f, -500.0 * -sin((M_PI * (148.5 * e)) / 180.0),
                -500.0 * cos((M_PI * (148.5 * e)) / 180.0), 0.0f, -sin((M_PI * (148.5 * e)) / 180.0), cos((M_PI * (148.5 * e)) / 180.0));
        }
    }
    else {
        Hu3DModelAttrSet(obj->mdlId[5], HU3D_ATTR_DISPOFF);
    }
    if (lbl_1_bss_AC == M527_MODE_CAMERA) {
        if (lbl_1_bss_0 > 60.0f) {
            t = 1.0f;
        }
        else {
            t = lbl_1_bss_0 / 60.0f;
        }
        t2 = sin((M_PI * (120.0f * t)) / 180.0) / sin(2.0943951023931953);
        c1 = 320.0f - (160.0f * t);
        c2 = 320.0f + (160.0f * t);
        half = 320.0f - (160.0f * t);
        x1 = c1 - half;
        x2 = c1 + half;
        y1 = c2 - half;
        y2 = c2 + half;
        Hu3DCameraPerspectiveSet(1, 40.0f, 20.0f, 10000.0f, (576.0f * ((2.0f * half) / 640.0f)) / 480.0f);
        Hu3DCameraViewportSet(1, x1, 0.0f, x2 - x1, 480.0f, 0.0f, 1.0f);
        Hu3DCameraPerspectiveSet(2, 40.0f, 20.0f, 10000.0f, (576.0f * ((2.0f * half) / 640.0f)) / 480.0f);
        Hu3DCameraViewportSet(2, y1, 0.0f, y2 - y1, 480.0f, 0.0f, 1.0f);
        if (x2 > y1) {
            sc3 = x2;
        }
        else {
            sc3 = y1;
        }
        if (x1 < 0.0f) {
            sc0 = 0.0f;
        }
        else {
            sc0 = x1;
        }
        if (sc3 < 0.0f) {
            sc1 = 0.0f;
        }
        else {
            sc1 = sc3;
        }
        if (x2 > 640.0f) {
            sc2 = 640.0f;
        }
        else {
            sc2 = x2;
        }
        if (y2 > 640.0f) {
            sc3 = 640.0f;
        }
        else {
            sc3 = y2;
        }
        Hu3DCameraScissorSet(1, (u32) sc0, 0, (u32) ((sc2 - sc0) - 2.0f), 480);
        Hu3DCameraScissorSet(2, (u32) (2.0f + sc1), 0, (u32) ((sc3 - sc1) - 2.0f), 480);
    }
    if (lbl_1_bss_AC == M527_MODE_START && lbl_1_bss_0 == 10.0f) {
        Hu3DMotionSet(obj->mdlId[5], obj->mtnId[0]);
        Hu3DModelAttrSet(obj->mdlId[5], HU3D_MOTATTR_LOOP);
    }
    if (lbl_1_bss_AC == M527_MODE_RESULT && lbl_1_data_158[0] != 4 && lbl_1_bss_0 == 50.0f) {
        w = lbl_1_data_158[0];
        if (lbl_1_bss_B8[(1 - w) * 2]->trans.z < lbl_1_bss_B8[((1 - w) * 2) + 1]->trans.z) {
            side = 0;
        }
        else {
            side = 1;
        }
        for (i = 0; i < 2; i++) {
            if (side == 0) {
                j = i;
            }
            else {
                j = 1 - i;
            }
            lbl_1_bss_84[i] = lbl_1_bss_B8[((1 - w) * 2) + j]->trans.x;
            if (side == 0) {
                n = i;
            }
            else {
                n = 1 - i;
            }
            lbl_1_bss_7C[i] = lbl_1_bss_B8[((1 - w) * 2) + n]->trans.z;
            if (lbl_1_bss_84[i] < -270.0f) {
                t = -270.0f;
            }
            else {
                if (lbl_1_bss_84[i] > 270.0f) {
                    t2 = 270.0f;
                }
                else {
                    t2 = lbl_1_bss_84[i];
                }
                t = t2;
            }
            lbl_1_bss_84[i] = t;
            if (lbl_1_bss_7C[i] < -270.0f) {
                t = -270.0f;
            }
            else {
                if (lbl_1_bss_7C[i] > 270.0f) {
                    t2 = 270.0f;
                }
                else {
                    t2 = lbl_1_bss_7C[i];
                }
                t = t2;
            }
            lbl_1_bss_7C[i] = t;
        }
    }
    if (lbl_1_bss_AC == M527_MODE_RESULT && lbl_1_data_158[0] != 4) {
        k = 1.0f;
        if (lbl_1_bss_0 < 50.0f) {
            t = 0.0f;
        }
        else {
            if (lbl_1_bss_0 > 70.0f) {
                t2 = 1.0f;
            }
            else {
                t2 = (lbl_1_bss_0 - 50.0f) / 20.0f;
            }
            t = t2;
        }
        e = t;
        k *= 1.0f - ((1.0f - t) * (1.0f - e));
        for (i = 0; i < 2; i++) {
            x1 = lbl_1_bss_84[i];
            y1 = lbl_1_bss_7C[i];
            Hu3DModelPosSet(obj->mdlId[i + 10], x1, 0.0f, y1);
            Hu3DModelScaleSet(obj->mdlId[i + 10], 2.07 * k, 1.0f, 2.07 * k);
            Hu3DModelAttrReset(obj->mdlId[i + 10], HU3D_ATTR_DISPOFF);
            if (lbl_1_data_158[0] == 1) {
                camNo = 1;
            }
            else {
                camNo = 2;
            }
            Hu3DModelCameraSet(obj->mdlId[i + 10], camNo);
            if (lbl_1_data_158[0] == 1) {
                camNo = 1;
            }
            else {
                camNo = 2;
            }
            Hu3DModelCameraSet(obj->mdlId[i + 14], camNo);
            Hu3DModelAttrReset(obj->mdlId[i + 14], HU3D_ATTR_DISPOFF);
        }
    }
    if (lbl_1_bss_AC == M527_MODE_RESULT && lbl_1_data_158[0] != 4) {
        if (lbl_1_bss_0 < 110.0f) {
            t = 0.0f;
        }
        else {
            if (lbl_1_bss_0 > 170.0f) {
                t2 = 1.0f;
            }
            else {
                t2 = (lbl_1_bss_0 - 110.0f) / 60.0f;
            }
            t = t2;
        }
        e = 1.0f - t;
        c1 = 320.0f - (160.0f * e);
        c2 = 320.0f + (160.0f * e);
        half = 320.0f - (160.0f * e);
        x1 = c1 - half;
        x2 = c1 + half;
        y1 = c2 - half;
        y2 = c2 + half;
        Hu3DCameraPerspectiveSet(1, 40.0f, 20.0f, 10000.0f, (576.0f * ((2.0f * half) / 640.0f)) / 480.0f);
        Hu3DCameraViewportSet(1, x1, 0.0f, x2 - x1, 480.0f, 0.0f, 1.0f);
        Hu3DCameraPerspectiveSet(2, 40.0f, 20.0f, 10000.0f, (576.0f * ((2.0f * half) / 640.0f)) / 480.0f);
        Hu3DCameraViewportSet(2, y1, 0.0f, y2 - y1, 480.0f, 0.0f, 1.0f);
        if (lbl_1_data_158[0] == 0) {
            if (x2 > y1) {
                sc3 = x2;
            }
            else {
                sc3 = y1;
            }
            y1 = sc3;
        }
        else {
            if (x2 > y1) {
                sc3 = y1;
            }
            else {
                sc3 = x2;
            }
            x2 = sc3;
        }
        if (x1 < 0.0f) {
            sc0 = 0.0f;
        }
        else {
            sc0 = x1;
        }
        if (y1 < 0.0f) {
            sc1 = 0.0f;
        }
        else {
            sc1 = y1;
        }
        if (x2 > 640.0f) {
            sc2 = 640.0f;
        }
        else {
            sc2 = x2;
        }
        if (y2 > 640.0f) {
            sc3 = 640.0f;
        }
        else {
            sc3 = y2;
        }
        Hu3DCameraScissorSet(1, (u32) sc0, 0, (u32) ((sc2 - sc0) - 2.0f), 480);
        Hu3DCameraScissorSet(2, (u32) (2.0f + sc1), 0, (u32) ((sc3 - sc1) - 2.0f), 480);
    }
    if (lbl_1_bss_AC >= M527_MODE_END && lbl_1_data_158[0] != 4) {
        zoom = 1.25f;
        if (lbl_1_bss_0 > 60.0f) {
            t = 0.0f;
        }
        else {
            t = 1.0f - (lbl_1_bss_0 / 60.0f);
        }
        e = t;
        posY = 270.0f - (500.0f * (t * e));
        if (lbl_1_bss_B4->mdlId[5] == lbl_1_data_1D0[lbl_1_data_158[0]]) {
            lbl_1_data_24C++;
            if (lbl_1_data_24C > 0x10C) {
                n = 14;
            }
            else {
                n = 0;
            }
            lbl_1_data_24C -= n;
            Hu3DMotionTimeSet(lbl_1_data_1D0[lbl_1_data_158[0]], lbl_1_data_24C);
            if (lbl_1_bss_AC == M527_MODE_END && lbl_1_bss_0 == 70.0f) {
                M527_SE_PLAY(15);
            }
        }
        else {
            lbl_1_data_24C++;
            if (lbl_1_data_24C - 0x8A >= 0x8C) {
                n = 0x30;
            }
            else {
                n = 0;
            }
            lbl_1_data_24C -= n;
            Hu3DMotionTimeSet(lbl_1_data_1D0[lbl_1_data_158[0]], lbl_1_data_24C - 0x8A);
        }
        Hu3DModelPosSet(lbl_1_data_1D0[lbl_1_data_158[0]], 0.0f, posY, -1000.0f);
        Hu3DModelScaleSet(lbl_1_data_1D0[lbl_1_data_158[0]], zoom, zoom, zoom);
        Hu3DModelRotSet(lbl_1_data_1D0[lbl_1_data_158[0]], 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrReset(lbl_1_data_1D0[lbl_1_data_158[0]], HU3D_ATTR_DISPOFF);
    }
    else if (lbl_1_bss_AC >= M527_MODE_END && lbl_1_data_158[0] == 4) {
        zoom = 0.75f;
        lbl_1_bss_10++;
        lbl_1_bss_14[0] += 1.0 + (0.15000000596046448 * sin((M_PI * (lbl_1_bss_10 * 4)) / 180.0));
        for (i = 0; i < 2; i++) {
            Hu3DMotionTimeSet(lbl_1_data_1D0[i], lbl_1_bss_10 % 60);
            Hu3DModelPosSet(lbl_1_data_1D0[i], (30.0 * sin((M_PI * ((lbl_1_bss_10 * 3) % 360)) / 180.0)) + (15.0 * sin((M_PI * (9.75 * lbl_1_bss_14[0])) / 180.0)),
                770.0 + (-750.0 * (lbl_1_bss_10 / 180.0)), -1000.0f);
            Hu3DModelRotSet(lbl_1_data_1D0[i], 0.0f, 0.0f, 6.0f);
            Hu3DModelScaleSet(lbl_1_data_1D0[i], zoom, zoom, zoom);
            if (lbl_1_bss_10 < 0xF0) {
                Hu3DModelAttrReset(lbl_1_data_1D0[i], HU3D_ATTR_DISPOFF);
            }
            else {
                Hu3DModelAttrSet(lbl_1_data_1D0[i], HU3D_ATTR_DISPOFF);
            }
        }
    }
    if (lbl_1_bss_AC == M527_MODE_PLAY || lbl_1_bss_AC == M527_MODE_RESULT) {
        for (team = 0; team < 2; team++) {
            for (a = 0; a < 3; a++) {
                for (b = 0; b < 3; b++) {
                    col[a][b][0] = 32.0f;
                    col[a][b][1] = 255.0f;
                    col[a][b][2] = 200.0f;
                }
            }
            for (j = 0; j < 2; j++) {
                cellP = &lbl_1_data_1A0[j + (team * 2)];
                if (lbl_1_data_1C0[j + (team * 2)] != 0 && lbl_1_data_1B0[j + (team * 2)] > 0
                    && (lbl_1_data_1A0[((team * 2) + 1) - j] != lbl_1_data_1A0[j + (team * 2)] || lbl_1_data_1B0[((team * 2) + 1) - j] == 0
                        || ((0.5 * j) + lbl_1_data_1B0[j + (team * 2)]) > ((0.5 + lbl_1_data_1B0[((team * 2) + 1) - j]) - (0.5 * j)))) {
                    if (lbl_1_data_1C0[j + (team * 2)] == 1) {
                        if (lbl_1_data_1B0[j + (team * 2)] >= 29) {
                            t = 1.0f;
                        }
                        else {
                            if (lbl_1_data_1B0[j + (team * 2)] == 28) {
                                t2 = 0.0f;
                            }
                            else {
                                t2 = lbl_1_data_1B0[j + (team * 2)] / 27.0f;
                            }
                            t = t2;
                        }
                        if ((29 - lbl_1_data_1B0[j + (team * 2)]) % 2 == 0) {
                            e = t;
                        }
                        else {
                            e = 0.0f;
                        }
                        c = (s32) (255.0f * e);
                        col[*cellP % 3][*cellP / 3][0] += c / 2;
                        col[*cellP % 3][*cellP / 3][1] += c;
                        col[*cellP % 3][*cellP / 3][2] += c;
                        if (col[*cellP % 3][*cellP / 3][0] > 375.0f) {
                            t = 375.0f;
                        }
                        else {
                            t = col[*cellP % 3][*cellP / 3][0];
                        }
                        col[*cellP % 3][*cellP / 3][0] = t;
                        if (col[*cellP % 3][*cellP / 3][1] > 375.0f) {
                            t = 375.0f;
                        }
                        else {
                            t = col[*cellP % 3][*cellP / 3][1];
                        }
                        col[*cellP % 3][*cellP / 3][1] = t;
                        if (col[*cellP % 3][*cellP / 3][2] > 375.0f) {
                            t = 375.0f;
                        }
                        else {
                            t = col[*cellP % 3][*cellP / 3][2];
                        }
                        col[*cellP % 3][*cellP / 3][2] = t;
                    }
                    else {
                        if (lbl_1_data_1B0[j + (team * 2)] >= 19) {
                            t = 1.0f;
                        }
                        else {
                            if (lbl_1_data_1B0[j + (team * 2)] == 18) {
                                t2 = 0.0f;
                            }
                            else {
                                t2 = lbl_1_data_1B0[j + (team * 2)] / 17.0f;
                            }
                            t = t2;
                        }
                        c = (s32) (255.0f * t);
                        cA = 255 - c;
                        if (c < 0x80) {
                            cB = c;
                        }
                        else {
                            cB = 255 - c;
                        }
                        col[*cellP % 3][*cellP / 3][0] = ((c * (255.0f - col[*cellP % 3][*cellP / 3][0])) / 255.0f) + ((cA * col[*cellP % 3][*cellP / 3][0]) / 255.0f);
                        col[*cellP % 3][*cellP / 3][1] = ((c * (255.0f - col[*cellP % 3][*cellP / 3][1])) / 255.0f) + ((cA * col[*cellP % 3][*cellP / 3][1]) / 255.0f);
                        col[*cellP % 3][*cellP / 3][2] = ((cB * (255.0f - col[*cellP % 3][*cellP / 3][2])) / 255.0f) + ((cA * col[*cellP % 3][*cellP / 3][2]) / 255.0f);
                    }
                }
            }
            for (j = 0; j < 2; j++) {
                fx = 0.5f + (lbl_1_bss_B8[j + (team * 2)]->trans.x / 933.3333f);
                if (fx < 0.0f) {
                    t = 0.0f;
                }
                else {
                    if (fx > 1.0f) {
                        t2 = 1.0f;
                    }
                    else {
                        t2 = fx;
                    }
                    t = t2;
                }
                n = (s32) (t / 0.25f);
                if (n >= 4) {
                    ix = 3;
                }
                else {
                    ix = n;
                }
                frx = (4.0f * t) - (s32) (t / 0.25f);
                fz = 0.5f + (lbl_1_bss_B8[j + (team * 2)]->trans.z / 933.3333f);
                if (fz < 0.0f) {
                    t = 0.0f;
                }
                else {
                    if (fz > 1.0f) {
                        t2 = 1.0f;
                    }
                    else {
                        t2 = fz;
                    }
                    t = t2;
                }
                n = (s32) (t / 0.25f);
                if (n >= 4) {
                    iz = 3;
                }
                else {
                    iz = n;
                }
                frz = (4.0f * t) - (s32) (t / 0.25f);
                if (ix == 0) {
                    cell = 0;
                }
                else {
                    cell = ix - 1;
                }
                xi[0] = cell;
                if (ix == 3) {
                    cell = 2;
                }
                else {
                    cell = ix;
                }
                xi[1] = cell;
                wx[0] = 1.0f - frx;
                wx[1] = frx;
                if (iz == 0) {
                    cell = 0;
                }
                else {
                    cell = iz - 1;
                }
                zi[0] = cell;
                if (iz == 3) {
                    cell = 2;
                }
                else {
                    cell = iz;
                }
                zi[1] = cell;
                wz[0] = 1.0f - frz;
                wz[1] = frz;
                sumB = 0.0f;
                sumG = 0.0f;
                sumR = 0.0f;
                for (i = 0; i < 4; i++) {
                    sumR += wz[i / 2] * (wx[i % 2] * col[xi[i % 2]][zi[i / 2]][0]);
                    sumG += wz[i / 2] * (wx[i % 2] * col[xi[i % 2]][zi[i / 2]][1]);
                    sumB += wz[i / 2] * (wx[i % 2] * col[xi[i % 2]][zi[i / 2]][2]);
                }
                Hu3DLLightColorSet(lbl_1_bss_B8[j + (team * 2)]->mdlId[0], ((M527PLAYER *) lbl_1_bss_B8[j + (team * 2)]->data)->lightId,
                    (u8) (s32) ((2.0f * sumR) / 3.0f), (u8) (s32) ((2.0f * sumG) / 3.0f), (u8) (s32) ((2.0f * sumB) / 3.0f), 255);
            }
        }
    }
}

// NONMATCHING: the model test is scheduled later than in the original
/*
 * GX hook of the floor border: flat white quads on the cells the lit team has
 * to reach, drawn with the colour buffer masked so only z is written, plus the
 * team marker (a textured quad) that is raised during the result sequence.
 * Finally a full screen quad darkens the screen.
 */
void fn_1_7E5C(HU3DMODEL *modelP, Mtx *mtx)
{
    f32 corner[4][3] = { { -1.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 1.0f }, { -1.0f, 0.0f, 1.0f } };
    Mtx44 proj;
    Mtx id;
    f32 ease;
    f32 ease2;
    f32 w;
    s32 team;
    s32 i;
    s32 j;

    if (modelP == &Hu3DData[lbl_1_bss_B4->mdlId[6]]) {
        team = 0;
    }
    else {
        team = 1;
    }
    GXLoadPosMtxImm(*mtx, 0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZCompLoc(GX_FALSE);
    GXSetColorUpdate(GX_FALSE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    if (lbl_1_bss_AC == M527_MODE_RESULT && team == 1 - lbl_1_data_158[0]) {
        GXSetColorUpdate(GX_FALSE);
        GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_AND, GX_GEQUAL, 0x80);
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
        HuSprTexLoad(espanim[esprite[lbl_1_bss_210].animNo].anim, 0, 0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
        for (i = 0; i < 2; i++) {
            w = 133.33333f;
            if (lbl_1_bss_0 < 50.0f) {
                ease = 0.0f;
            }
            else {
                if (lbl_1_bss_0 > 70.0f) {
                    ease2 = 1.0f;
                }
                else {
                    ease2 = (lbl_1_bss_0 - 50.0f) / 20.0f;
                }
                ease = ease2;
            }
            ease2 = ease;
            w *= 1.0f - ((1.0f - ease2) * (1.0f - ease2));
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            for (j = 0; j < 4; j++) {
                GXPosition3f32(lbl_1_bss_84[i] + (w * corner[j][0]), 1.5f, lbl_1_bss_7C[i] + (w * corner[j][2]));
                GXColor4u8(255, 255, 255, 255);
                GXTexCoord2f32((1.0f + corner[j][0]) * 0.5f, (1.0f + corner[j][2]) * 0.5f);
            }
            GXEnd();
        }
        GXPixModeSync();
    }
    GXSetColorUpdate(GX_TRUE);
    MTXOrtho(proj, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);
    MTXIdentity(id);
    GXLoadPosMtxImm(id, 0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ONE);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTCLR, GX_BL_ONE, GX_LO_NOOP);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    for (i = 0; i < 1; i++) {
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(0.0f, 0.0f, 0.0f);
        GXColor4u8(255, 255, 255, 255);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(1.0f, 0.0f, 0.0f);
        GXColor4u8(255, 255, 255, 255);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(1.0f, 1.0f, 0.0f);
        GXColor4u8(255, 255, 255, 255);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(0.0f, 1.0f, 0.0f);
        GXColor4u8(255, 255, 255, 255);
        GXTexCoord2f32(0.0f, 1.0f);
        GXEnd();
    }
    GXSetColorUpdate(GX_TRUE);
    GXSetZCompLoc(GX_TRUE);
}

/* wraps an angle into 0..360 (done twice, like the original) */
#define M527_WRAP360(a)                \
    do {                               \
        s32 add_;                      \
        if ((a) < 0.0f) {              \
            add_ = 360;                \
        }                              \
        else if ((a) >= 360.0f) {      \
            add_ = -360;               \
        }                              \
        else {                         \
            add_ = 0;                  \
        }                              \
        (a) += add_;                   \
    } while (0)

// NONMATCHING: register allocation differs (the original keeps the arc range in a double register)
/*
 * GX hook of the winner's marker: a 12 sided disc (triangle strip, 26 vertices)
 * that grows around the winning team's first tile during the result sequence.
 * The alpha test cuts a sector out of it that points at the other tile.
 */
void fn_1_87F4(HU3DMODEL *modelP, Mtx *mtx)
{
    f32 corner[4][3] = { { -1.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 1.0f }, { -1.0f, 0.0f, 1.0f } };
    f32 circle[12][2] = { { 50.0f, 0.0f }, { 43.30127f, -25.0f }, { 24.999998f, -43.30127f }, { -2e-06f, -49.999996f }, { -25.0f, -43.301266f },
        { -43.301266f, -24.999996f }, { -49.999992f, 1e-06f }, { -43.30126f, 24.999998f }, { -24.999992f, 43.301266f }, { 5e-06f, 49.999992f },
        { 25.0f, 43.30126f }, { 43.301266f, 24.999992f } };
    f32 dx;
    f32 dz;
    f32 ang;
    f32 dist;
    f32 range;
    f32 angMin;
    f32 angMax;
    f32 ease;
    f32 ease2;
    f32 size;
    f32 cx;
    f32 cz;
    f32 r;
    s32 team;
    s32 i;
    s32 k;

    if (modelP == &Hu3DData[lbl_1_bss_B4->mdlId[14]]) {
        team = 0;
    }
    else {
        team = 1;
    }
    dx = lbl_1_bss_84[1 - team] - lbl_1_bss_84[team];
    dz = lbl_1_bss_7C[1 - team] - lbl_1_bss_7C[team];
    ang = (180.0 * (atan2(dx, dz) / M_PI)) - 90.0;
    M527_WRAP360(ang);
    M527_WRAP360(ang);
    dist = sqrtf((dx * dx) + (dz * dz));
    dist /= 200.0f;
    if (dist > 1.0f) {
        range = -1.0;
    }
    else {
        range = 90.0 * (1.0 - (2.0 * ((f32) asin(dist) / M_PI)));
    }
    angMin = ang - range;
    angMax = ang + range;
    M527_WRAP360(angMin);
    M527_WRAP360(angMin);
    M527_WRAP360(angMax);
    M527_WRAP360(angMax);
    GXLoadPosMtxImm(*mtx, 0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ONE);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    if (range == -1.0f || (s32) ((255.0f * angMin) / 360.0f) == (s32) ((255.0f * angMax) / 360.0f)) {
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    }
    else if (angMin < angMax) {
        GXSetAlphaCompare(GX_LEQUAL, (u8) (s32) ((255.0f * angMin) / 360.0f), GX_AOP_OR, GX_GEQUAL, (u8) (s32) ((255.0f * angMax) / 360.0f));
    }
    else {
        GXSetAlphaCompare(GX_LEQUAL, (u8) (s32) ((255.0f * angMin) / 360.0f), GX_AOP_AND, GX_GEQUAL, (u8) (s32) ((255.0f * angMax) / 360.0f));
    }
    GXSetZCompLoc(GX_FALSE);
    GXSetColorUpdate(GX_FALSE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    if (lbl_1_bss_AC == M527_MODE_RESULT) {
        for (i = 0; i < 1; i++) {
            if ((lbl_1_bss_0 - 1.0f) < 50.0f) {
                ease = 0.0f;
            }
            else {
                if ((lbl_1_bss_0 - 1.0f) > 70.0f) {
                    ease2 = 1.0f;
                }
                else {
                    ease2 = ((lbl_1_bss_0 - 1.0f) - 50.0f) / 20.0f;
                }
                ease = ease2;
            }
            size = 2.07f * (1.0f - ((1.0f - ease) * (1.0f - ease)));
            cx = lbl_1_bss_84[team];
            cz = lbl_1_bss_7C[team];
            if (size < 0.005f) {
                r = 0.0f;
            }
            else {
                r = size - 0.005f;
            }
            GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 26);
            for (k = 0; k < 26; k++) {
                GXPosition3f32(cx + (r * circle[(k / 2) % 12][0]), ((k & 1) == 1) ? -400.391602 : 0.0, cz + (r * circle[(k / 2) % 12][1]));
                GXColor4u8(255, 255, 255, (u8) (((k / 2) * 255) / 12));
                GXTexCoord2f32(0.0f, 0.0f);
            }
            GXEnd();
        }
        GXPixModeSync();
    }
    GXSetColorUpdate(GX_TRUE);
    GXSetZCompLoc(GX_TRUE);
}

// NONMATCHING: register allocation and load scheduling differ
/*
 * GX hook drawn over everything else: while a team's tile is flashing, a quad
 * is laid on that cell. In the first phase it is a coloured light (additive),
 * in the second a fading dark one (inverse blend). One hook per team.
 */
void fn_1_94F4(HU3DMODEL *modelP, Mtx *mtx)
{
    f32 corner[4][3] = { { -1.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 1.0f }, { -1.0f, 0.0f, 1.0f } };
    f32 x0;
    f32 z0;
    f32 x1;
    f32 z1;
    f32 t;
    f32 t2;
    s32 alpha;
    s32 r;
    s32 g;
    s32 b;
    s32 team;
    s32 i;
    s32 *cell;

    if (modelP == &Hu3DData[lbl_1_bss_B4->mdlId[8]]) {
        team = 0;
    }
    else {
        team = 1;
    }
    GXLoadPosMtxImm(*mtx, 0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_INVSRCCLR, GX_BL_SRCALPHA, GX_LO_NOOP);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    for (i = 0; i < 2; i++) {
        cell = &lbl_1_data_1A0[i + (team * 2)];
        if (lbl_1_data_1C0[i + (team * 2)] != 0 && lbl_1_data_1B0[i + (team * 2)] > 0
            && (lbl_1_data_1A0[((team * 2) + 1) - i] != lbl_1_data_1A0[i + (team * 2)] || lbl_1_data_1B0[((team * 2) + 1) - i] == 0
                || ((0.5 * i) + lbl_1_data_1B0[i + (team * 2)]) > ((0.5 + lbl_1_data_1B0[((team * 2) + 1) - i]) - (0.5 * i)))) {
            x0 = 116.666664f * (((*cell % 3) * 2) - 3);
            z0 = 116.666664f * (((*cell / 3) * 2) - 3);
            x1 = 116.666664f * (((*cell % 3) * 2) - 1);
            z1 = 116.666664f * (((*cell / 3) * 2) - 1);
            if (lbl_1_data_1C0[i + (team * 2)] == 1) {
                if (lbl_1_data_1B0[i + (team * 2)] >= 29) {
                    t = 1.0f;
                }
                else {
                    if (lbl_1_data_1B0[i + (team * 2)] == 28) {
                        t2 = 0.0f;
                    }
                    else {
                        t2 = lbl_1_data_1B0[i + (team * 2)] / 27.0f;
                    }
                    t = t2;
                }
                if ((29 - lbl_1_data_1B0[i + (team * 2)]) % 2 == 0) {
                    t2 = t;
                }
                else {
                    t2 = 0.0f;
                }
                GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
                g = (s32) (255.0f * t2);
                alpha = 255 - g;
                r = g / 2;
                b = g;
            }
            else {
                if (lbl_1_data_1B0[i + (team * 2)] >= 19) {
                    t = 1.0f;
                }
                else {
                    if (lbl_1_data_1B0[i + (team * 2)] == 18) {
                        t2 = 0.0f;
                    }
                    else {
                        t2 = lbl_1_data_1B0[i + (team * 2)] / 17.0f;
                    }
                    t = t2;
                }
                GXSetBlendMode(GX_BM_BLEND, GX_BL_INVSRCCLR, GX_BL_SRCALPHA, GX_LO_NOOP);
                g = (s32) (255.0f * t);
                alpha = 255 - g;
                r = g;
                if (g < 0x80) {
                    b = g;
                }
                else {
                    b = 255 - g;
                }
            }
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(x0, 0.0f, z0);
            GXColor4u8((u8) r, (u8) g, (u8) b, alpha);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(x1, 0.0f, z0);
            GXColor4u8((u8) r, (u8) g, (u8) b, alpha);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(x1, 0.0f, z1);
            GXColor4u8((u8) r, (u8) g, (u8) b, alpha);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(x0, 0.0f, z1);
            GXColor4u8((u8) r, (u8) g, (u8) b, alpha);
            GXTexCoord2f32(0.0f, 1.0f);
            GXEnd();
        }
    }
    GXSetColorUpdate(GX_TRUE);
    GXSetZCompLoc(GX_TRUE);
}

/* Material hook: the water-surface tev setup used by the shared models. */
void fn_1_9FE8(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    Hu3DMatLightSet(drawObj->model, 0, material->hiliteScale);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_BL_INVSRCALPHA);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_DIVIDE_2, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetZMode(GX_TRUE, GX_GREATER, GX_FALSE);
}

/*
 * Main sequence: plays the queued sound effects, then runs the mode machine
 * (wipe in, intro, camera zoom, start message, play, result, end).
 */
void fn_1_A0D0(OMOBJ *obj)
{
    M527PLAYER *player[4];
    s32 i;
    s32 n;
    s32 w;
    s32 side;
    s32 r;
    s32 camNo;
    s32 coinPlayer;

    for (i = 0; i < 4; i++) {
        player[i] = lbl_1_bss_B8[i]->data;
    }
    for (i = 0; i < 16; i++) {
        s32 seNo[16] = { 0x776, 0x777, 0x778, 0x779, 0x77A, 0x77B, 0x77C, 0x77D, 0x77E, 0x77F, 0x780, 0x781, 0x782, 0x783, 0x784, 0x785 };

        if (lbl_1_bss_D0[i].num > 3) {
            lbl_1_bss_D0[i].num = 3;
        }
        if (lbl_1_bss_D0[i].num > 0) {
            lbl_1_bss_D0[i].seNo = HuAudFXPlay(seNo[i]);
            lbl_1_bss_D0[i].num--;
        }
    }
    if (omSysExitReq != 0 && WipeCheckEnd() == 0 && lbl_1_bss_8 == 0) {
        HuAudFadeOut(1000);
        lbl_1_data_164 = 1;
        lbl_1_bss_8 = 1;
    }
    switch (lbl_1_bss_AC) {
        case M527_MODE_WIPEIN:
            lbl_1_bss_0 += 1.0f;
            if (WipeCheckEnd() == 0) {
                lbl_1_bss_AC++;
                lbl_1_bss_0 = 0.0f;
            }
            break;
        case M527_MODE_INTRO:
            if (lbl_1_bss_0 == 1.0f) {
                M527_SE_PLAY(0);
            }
            lbl_1_bss_0 += 1.0f;
            if ((s32) lbl_1_bss_0 % 10 == 1) {
                n = (s32) (lbl_1_bss_0 - 1.0f) / 10;
                if (n > 5) {
                    r = 5 - (n - 5);
                }
                else {
                    r = n;
                }
                n = r;
                if (n < 0) {
                    r = 0;
                }
                else {
                    r = n;
                }
                n = r;
                for (i = 0; i < 4; i++) {
                    if (player[i]->padNo != -1) {
                        s32 vibTbl[6][2] = { { 1, 9 }, { 2, 8 }, { 3, 7 }, { 4, 6 }, { 5, 5 }, { 6, 4 } };

                        omVibrate(player[i]->playerNo, 10, vibTbl[n][0], vibTbl[n][1]);
                    }
                }
            }
            if (lbl_1_bss_0 >= 135.0f) {
                lbl_1_bss_AC++;
                lbl_1_bss_0 = 0.0f;
            }
            break;
        case M527_MODE_CAMERA:
            lbl_1_bss_0 += 1.0f;
            if (lbl_1_bss_0 > 60.0f) {
                lbl_1_bss_AC++;
                lbl_1_bss_0 = 0.0f;
            }
            break;
        case M527_MODE_START:
            lbl_1_bss_0 += 1.0f;
            if (lbl_1_bss_0 == 5.0f && lbl_1_bss_A6 < 0) {
                lbl_1_bss_A6 = GameMesMgStartCreate();
            }
            if (lbl_1_bss_0 >= 5.0f && lbl_1_data_160 == -1) {
                lbl_1_data_160 = HuAudSStreamPlay(0x3C);
            }
            if (lbl_1_bss_0 >= 5.0f && GameMesStatGet(lbl_1_bss_A6) == 0 && lbl_1_bss_A6 >= 0) {
                lbl_1_bss_AC++;
                lbl_1_bss_A0 = 3600;
                lbl_1_bss_A6 = -1;
                lbl_1_bss_0 = 0.0f;
                for (i = 0; i < 4; i++) {
                    player[i]->state = M527_STATE_PLAY;
                }
                for (i = 0; i < 20; i++) {
                    if (i % 10 >= 0 && i % 10 <= 4) {
                        espDispOn(lbl_1_data_50[i]);
                    }
                }
                MgScoreWinDispSet(lbl_1_bss_78[0], 1);
                MgScoreWinDispSet(lbl_1_bss_78[1], 1);
            }
            break;
        case M527_MODE_PLAY:
            lbl_1_bss_0 += 1.0f;
            if (lbl_1_bss_A0 == 3600) {
                lbl_1_bss_A4 = GameMesTimerCreate(lbl_1_bss_A0 / 60);
                GameMesPosSet(lbl_1_bss_A4, 288.0f, 64.0f);
            }
            if (lbl_1_bss_A0 <= 3600) {
                GameMesTimerValueSet(lbl_1_bss_A4, (lbl_1_bss_A0 + 58) / 60);
            }
            lbl_1_bss_A0--;
            if (lbl_1_bss_A0 <= 0 || lbl_1_data_158[0] != -1) {
                lbl_1_bss_AC = M527_MODE_RESULT;
                lbl_1_bss_0 = 0.0f;
                if (lbl_1_data_158[0] != -1) {
                    w = lbl_1_data_158[0];
                }
                else {
                    if (lbl_1_data_168[0] > lbl_1_data_168[1]) {
                        r = 0;
                    }
                    else {
                        if (lbl_1_data_168[0] < lbl_1_data_168[1]) {
                            n = 1;
                        }
                        else {
                            n = 4;
                        }
                        r = n;
                    }
                    w = r;
                }
                lbl_1_data_158[0] = w;
                for (i = 0; i < 4; i++) {
                    if (player[i]->state == M527_STATE_PLAY) {
                        r = M527_STATE_FINISH;
                    }
                    else {
                        r = player[i]->state;
                    }
                    player[i]->state = r;
                }
                if (lbl_1_data_158[0] != 4 && lbl_1_bss_A0 >= 0x925) {
                    lbl_1_data_1D0[1] = lbl_1_bss_B4->mdlId[5];
                    lbl_1_data_1D0[0] = lbl_1_bss_B4->mdlId[5];
                    break;
                }
                for (i = 0; i < 2; i++) {
                    do {
                        r = rand8() % 9;
                    } while (r == lbl_1_data_178[i * 2] || r == lbl_1_data_178[(i * 2) + 1]);
                    lbl_1_data_1D0[i] = lbl_1_bss_B8[(i * 2) + 1]->mdlId[r + 2];
                }
            }
            break;
        case M527_MODE_RESULT:
            if (lbl_1_bss_A6 < 0 && lbl_1_bss_0 == 0.0f) {
                GameMesTimerEnd(lbl_1_bss_A4);
                lbl_1_bss_A6 = GameMesMgFinishCreate();
                if (lbl_1_data_160 != -1) {
                    HuAudSStreamFadeOut(lbl_1_data_160, 100);
                }
            }
            else if (GameMesStatGet(lbl_1_bss_A6) == 0) {
                if (lbl_1_data_158[0] != 4) {
                    n = 30;
                }
                else {
                    n = 0;
                }
                if (lbl_1_bss_0 >= (f32) (n + 180)) {
                    lbl_1_bss_AC++;
                    lbl_1_bss_0 = 0.0f;
                    if (lbl_1_data_158[0] != 4) {
                        w = lbl_1_data_158[0];
                        side = lbl_1_bss_B8[(1 - w) * 2]->trans.x > lbl_1_bss_B8[((1 - w) * 2) + 1]->trans.x;
                        coinPlayer = player[w * 2]->playerNo;
                        if (!_CheckFlag(FLAG_MG_PRACTICE)) {
                            GwPlayer[coinPlayer].mgCoinBonus = 10;
                        }
                        coinPlayer = player[(w * 2) + 1]->playerNo;
                        if (!_CheckFlag(FLAG_MG_PRACTICE)) {
                            GwPlayer[coinPlayer].mgCoinBonus = 10;
                        }
                        lbl_1_bss_A6 = GameMesMgWinnerCreate(player[w * 2]->charNo, player[(w * 2) + 1]->charNo, -1, -1);
                        HuAudSStreamPlay(0x79);
                        lbl_1_bss_B8[(1 - w) * 2]->trans.x = ((side * 2) - 1) * ((650.0 + (100.0 * ((f64) rand8() / 255.0))) - 50.0);
                        lbl_1_bss_B8[((1 - w) * 2) + 1]->trans.x = ((side * 2) - 1) * -((650.0 + (100.0 * ((f64) rand8() / 255.0))) - 50.0);
                        lbl_1_bss_B8[(1 - w) * 2]->trans.x = ((side * 2) - 1) * 850;
                        lbl_1_bss_B8[((1 - w) * 2) + 1]->trans.x = ((side * 2) - 1) * -850;
                        for (i = 0; i < 2; i++) {
                            lbl_1_bss_B8[i + ((1 - w) * 2)]->trans.y = 100.0f;
                            lbl_1_bss_B8[i + ((1 - w) * 2)]->trans.z = -850.0f;
                            lbl_1_bss_B8[i + ((1 - w) * 2)]->rot.y = 0.0f;
                            lbl_1_bss_B8[i + ((1 - w) * 2)]->rot.y = 0.0f;
                            lbl_1_bss_B8[i + ((1 - w) * 2)]->rot.x = 360.0f * ((f32) (rand8() % 256) * 0.00390625f);
                            if (lbl_1_data_158[0] == 0) {
                                camNo = 1;
                            }
                            else {
                                camNo = 2;
                            }
                            Hu3DModelCameraSet(lbl_1_bss_B8[i + ((1 - w) * 2)]->mdlId[0], camNo);
                        }
                        if (lbl_1_data_158[0] == 0) {
                            camNo = 1;
                        }
                        else {
                            camNo = 2;
                        }
                        Hu3DModelCameraSet(lbl_1_bss_B4->mdlId[0], camNo);
                        if (lbl_1_data_158[0] == 0) {
                            camNo = 1;
                        }
                        else {
                            camNo = 2;
                        }
                        Hu3DModelCameraSet(lbl_1_bss_B4->mdlId[1], camNo);
                        if (lbl_1_data_158[0] == 0) {
                            n = 1;
                        }
                        else {
                            n = 2;
                        }
                        Hu3DModelCameraSet(lbl_1_bss_B4->mdlId[4], n);
                        if (lbl_1_data_158[0] == 0) {
                            r = 1;
                        }
                        else {
                            r = 2;
                        }
                        Hu3DModelCameraSet(lbl_1_bss_B4->mdlId[5], r);
                        Hu3DModelAttrSet(lbl_1_bss_B4->mdlId[3 - lbl_1_data_158[0]], HU3D_ATTR_DISPOFF);
                        Hu3DModelAttrSet(lbl_1_bss_B4->mdlId[9 - lbl_1_data_158[0]], HU3D_ATTR_DISPOFF);
                    }
                    else {
                        lbl_1_bss_A6 = GameMesMgDrawCreate();
                        HuAudSStreamPlay(0x7B);
                        for (i = 0; i < 4; i++) {
                            CharFXPlay(player[i]->charNo, 0x200);
                        }
                    }
                }
            }
            lbl_1_bss_0 += 1.0f;
            if (lbl_1_data_158[0] != 4 && lbl_1_bss_0 == 40.0f
                && (player[(1 - lbl_1_data_158[0]) * 2]->motNo != 0 || player[((1 - lbl_1_data_158[0]) * 2) + 1]->motNo != 0)) {
                lbl_1_bss_0 -= 1.0f;
            }
            break;
        case M527_MODE_END:
            lbl_1_bss_0 += 1.0f;
            if ((s32) lbl_1_bss_0 % 10 == 1) {
                n = (s32) (lbl_1_bss_0 - 1.0f) / 5;
                if (n > 5) {
                    r = 5 - (n - 5);
                }
                else {
                    r = n;
                }
                n = r;
                for (i = 0; i < 4; i++) {
                    if (lbl_1_data_158[0] == i / 2 && player[i]->padNo != -1 && n >= 0 && n < 6) {
                        s32 vibTbl[6][2] = { { 1, 9 }, { 2, 8 }, { 3, 7 }, { 4, 6 }, { 5, 5 }, { 6, 4 } };

                        omVibrate(player[i]->playerNo, 10, vibTbl[n][0], vibTbl[n][1]);
                    }
                }
            }
            for (i = 0; i < 4; i++) {
                if (lbl_1_data_158[0] == 4 || lbl_1_data_158[0] == i / 2) {
                    if (player[i]->state == M527_STATE_FINISH) {
                        r = M527_STATE_WIN;
                    }
                    else {
                        r = player[i]->state;
                    }
                    player[i]->state = r;
                }
            }
            if (lbl_1_bss_0 > 210.0f && lbl_1_bss_8 == 0) {
                lbl_1_bss_A6 = -1;
                lbl_1_bss_8 = 1;
            }
            break;
        default:
            OSReport("*** main mode error(%d)!!\n", lbl_1_bss_AC);
            break;
    }
}

/*
 * Creates the floor and its helper models (hooks, lights, shadow), the 20 score
 * marks and the two score windows, then hands over to the update function.
 */
void fn_1_B8C4(OMOBJ *obj)
{
    s32 i;
    s32 team;
    s32 idx;
    s32 file;
    s32 col;
    s16 lightId;

    idx = 0;
    obj->mdlId[idx] = Hu3DModelCreate(HuDataSelHeapReadNum(0x550000, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(obj->mdlId[idx], 4);
    Hu3DModelCameraSet(obj->mdlId[idx], 3);
    Hu3DModelAttrSet(obj->mdlId[idx], HU3D_MOTATTR_LOOP);
    idx = 1;
    obj->mdlId[idx] = Hu3DModelCreate(HuDataSelHeapReadNum(0x550001, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(obj->mdlId[idx], 5);
    Hu3DModelCameraSet(obj->mdlId[idx], 3);
    Hu3DModelAttrSet(obj->mdlId[idx], HU3D_MOTATTR_LOOP);
    lightId = Hu3DLLightCreate(obj->mdlId[idx], 0.0f, 0.0f, 500.0f, 0.0f, 0.0f, 4.0f, 255, 255, 255);
    lbl_1_bss_8C[0] = lightId;
    Hu3DLLightInfinitytSet(obj->mdlId[idx], lightId);
    idx = 4;
    obj->mdlId[idx] = Hu3DModelCreate(HuDataSelHeapReadNum(0x550004, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(obj->mdlId[idx], 5);
    Hu3DModelCameraSet(obj->mdlId[idx], 3);
    Hu3DModelAttrSet(obj->mdlId[idx], HU3D_MOTATTR_LOOP);
    Hu3DModelShadowMapSet(obj->mdlId[idx]);
    lightId = Hu3DLLightCreate(obj->mdlId[idx], 0.0f, 0.0f, 500.0f, 0.0f, 0.0f, 4.0f, 255, 255, 255);
    lbl_1_bss_8C[1] = lightId;
    Hu3DLLightInfinitytSet(obj->mdlId[idx], lightId);
    for (i = 0; i < 2; i++) {
        idx = (i == 0) ? 2 : 3;
        file = (i == 0) ? 0x550002 : 0x550003;
        obj->mdlId[idx] = Hu3DModelCreate(HuDataSelHeapReadNum(file, HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelLayerSet(obj->mdlId[idx], 4);
        Hu3DModelCameraSet(obj->mdlId[idx], (i == 0) ? 1 : 2);
        Hu3DModelAttrSet(obj->mdlId[idx], HU3D_MOTATTR_LOOP);
        lightId = Hu3DLLightCreate(obj->mdlId[idx], 0.0f, 0.0f, 500.0f, 0.0f, 0.0f, 4.0f, 255, 255, 255);
        lbl_1_bss_8C[i + 2] = lightId;
        Hu3DLLightInfinitytSet(obj->mdlId[idx], lightId);
        idx = (i == 0) ? 6 : 7;
        obj->mdlId[idx] = Hu3DHookFuncCreate(fn_1_7E5C);
        Hu3DModelLayerSet(obj->mdlId[idx], 3);
        Hu3DModelCameraSet(obj->mdlId[idx], (i == 0) ? 1 : 2);
        idx = (i == 0) ? 8 : 9;
        obj->mdlId[idx] = Hu3DHookFuncCreate(fn_1_94F4);
        Hu3DModelLayerSet(obj->mdlId[idx], 6);
        Hu3DModelCameraSet(obj->mdlId[idx], (i == 0) ? 1 : 2);
        idx = i + 10;
        if (i == 0) {
            obj->mdlId[idx] = Hu3DModelCreate(HuDataSelHeapReadNum(0x550007, HU_MEMNUM_OVL, HEAP_MODEL));
        }
        else {
            obj->mdlId[idx] = Hu3DModelLink(obj->mdlId[idx - 1]);
        }
        Hu3DModelLayerSet(obj->mdlId[idx], 1);
        Hu3DModelCameraSet(obj->mdlId[idx], 3);
        Hu3DModelAttrSet(obj->mdlId[idx], HU3D_ATTR_DISPOFF);
        Hu3DLLightInfinitytSet(obj->mdlId[idx], Hu3DLLightCreate(obj->mdlId[idx], 0.0f, 0.0f, 500.0f, 0.0f, 1.0f, -4.0f, 255, 255, 255));
        Hu3DModelMatHookSet(obj->mdlId[idx], fn_1_9FE8);
        Hu3DModelAmbSet(obj->mdlId[idx], 0.5f, 0.5f, 0.5f);
        idx = i + 14;
        obj->mdlId[idx] = Hu3DHookFuncCreate(fn_1_87F4);
        Hu3DModelLayerSet(obj->mdlId[idx], 0);
        Hu3DModelAttrSet(obj->mdlId[idx], HU3D_ATTR_DISPOFF);
    }
    idx = 5;
    obj->mdlId[idx] = Hu3DModelCreate(HuDataSelHeapReadNum(0x550008, HU_MEMNUM_OVL, HEAP_MODEL));
    obj->mtnId[0] = Hu3DJointMotion(obj->mdlId[idx], HuDataSelHeapReadNum(0x550012, HU_MEMNUM_OVL, HEAP_MODEL));
    obj->mtnId[1] = Hu3DJointMotion(obj->mdlId[idx], HuDataSelHeapReadNum(0x550013, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DMotionSet(obj->mdlId[idx], obj->mtnId[1]);
    Hu3DModelLayerSet(obj->mdlId[idx], 4);
    Hu3DModelCameraSet(obj->mdlId[idx], 3);
    Hu3DModelAttrReset(obj->mdlId[idx], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrSet(obj->mdlId[idx], HU3D_ATTR_DISPOFF);
    lightId = Hu3DLLightCreate(obj->mdlId[idx], 0.0f, 500.0f, -500.0f, 0.0f, -1.0f, 1.0f, 255, 255, 255);
    Hu3DLLightInfinitytSet(obj->mdlId[idx], lightId);
    Hu3DLLightStaticSet(obj->mdlId[idx], lightId, 1);
    for (i = 0; i < 20; i++) {
        s32 baseX[2] = { 82, 494 };
        s32 baseY[2] = { 54, 54 };
        s32 ofsX[20] = { -48, -24, 0, 24, 48 };
        s32 ofsY[20] = { 0 };

        lbl_1_data_50[i] = espEntry(lbl_1_data_0[i], 0, 0);
        espDispOff(lbl_1_data_50[i]);
        espAttrSet(lbl_1_data_50[i], 1);
        espBankSet(lbl_1_data_50[i], 0);
        col = i % 10;
        if (col >= 5) {
            col = col - 5;
        }
        team = (i < 10) ? 0 : 1;
        espPosSet(lbl_1_data_50[i], ofsX[col] + baseX[(i < 10) ? 0 : 1], ofsY[col] + baseY[team]);
    }
    lbl_1_bss_78[0] = MgScoreWinCreate(0x88, 0x24);
    MgScoreWinColorSet(lbl_1_bss_78[0], 250, 0, 30);
    MgScoreWinPosSet(lbl_1_bss_78[0], 82.0f, 56.0f);
    MgScoreWinDispSet(lbl_1_bss_78[0], 0);
    lbl_1_bss_78[1] = MgScoreWinCreate(0x88, 0x24);
    MgScoreWinColorSet(lbl_1_bss_78[1], 0, 50, 250);
    MgScoreWinPosSet(lbl_1_bss_78[1], 494.0f, 56.0f);
    MgScoreWinDispSet(lbl_1_bss_78[1], 0);
    lbl_1_bss_210 = espEntry(0x550006, 0, 0);
    espDispOff(lbl_1_bss_210);
    obj->objFunc = fn_1_4F98;
}

// NONMATCHING: register allocation differs (the original keeps the ambient factor in f31 and zero-inits its aiStick Vec from .rodata)
/*
 * Creates one player: sorts the players by team, fills the work record, builds
 * the character model, its motions and the shadow. The odd slot of each team
 * also builds the 9 floor tile models.
 */
void fn_1_C408(OMOBJ *obj)
{
    s32 order[4] = { 0, 1, 2, 3 };
    M527PLAYER *pl;
    s32 i;
    s32 j;
    s32 n;
    s32 camNo;
    HU3DMODELID tile;

    n = 0;
    pl = obj->data;
    memset(pl, 0, sizeof(M527PLAYER));
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (i == GwPlayerConf[j].grp) {
                order[n] = j;
                n++;
            }
        }
    }
    pl->no = obj->work[0];
    pl->playerNo = order[pl->no];
    if (pl->playerNo == 4) {
        pl->playerNo = pl->no;
    }
    pl->charNo = GwPlayerConf[pl->playerNo].charNo;
    pl->charNoClamp = (pl->charNo >= 9) ? 9 : pl->charNo;
    pl->padNo = (GwPlayerConf[pl->playerNo].type != 0) ? -1 : GwPlayerConf[pl->playerNo].padNo;
    pl->dif = (pl->padNo != -1) ? 0 : GwPlayerConf[pl->playerNo].dif;
    pl->state = M527_STATE_INTRO;
    pl->aiMode = 0;
    if (pl->dif == 0) {
        pl->aiFlag = (rand8() % 10) < 5;
    }
    else {
        if (pl->dif == 1) {
            n = (rand8() % 10) < 3;
        }
        else {
            n = 0;
        }
        pl->aiFlag = n;
    }
    obj->mdlId[0] = CharModelCreate(pl->charNo, 2);
    Hu3DModelLayerSet(obj->mdlId[0], 2);
    camNo = (pl->no < 2) ? 1 : 2;
    Hu3DModelCameraSet(obj->mdlId[0], camNo);
    Hu3DModelAttrReset(obj->mdlId[0], HU3D_MOTATTR_PAUSE);
    Hu3DModelAttrSet(obj->mdlId[0], HU3D_MOTATTR_LOOP);
    Hu3DModelShadowSet(obj->mdlId[0]);
    CharEffectLayerSet(7);
    pl->lightId = Hu3DLLightCreate(obj->mdlId[0], 0.0f, -500.0f, 0.0f, 0.0f, 3.0f, -1.0f, 0x15, 0xAA, 0x85);
    Hu3DLLightInfinitytSet(obj->mdlId[0], pl->lightId);
    Hu3DLLightStaticSet(obj->mdlId[0], pl->lightId, 1);
    if (pl->charNoClamp == 5) {
        Hu3DModelAmbSet(obj->mdlId[0], (2.0f * 0.75f) / 3.0f, (2.0f * 0.75f) / 3.0f, (2.0f * 0.75f) / 3.0f);
    }
    else {
        Hu3DModelAmbSet(obj->mdlId[0], 0.75f, 0.75f, 0.75f);
    }
    pl->motNo = 0;
    pl->motForce = -1;
    for (i = 0; i < 13; i++) {
        obj->mtnId[i] = CharMotionCreate(pl->charNo, lbl_1_data_D4[i]);
    }
    CharMotionVoiceOnSet(pl->charNo, (u16) lbl_1_data_D4[12], 0);
    Hu3DMotionSet(obj->mdlId[0], obj->mtnId[0]);
    Hu3DModelAttrSet(obj->mdlId[0], HU3D_MOTATTR_LOOP);
    obj->trans.x = lbl_1_data_1D8[pl->no * 4];
    obj->trans.y = lbl_1_data_1D8[(pl->no * 4) + 1];
    obj->trans.z = lbl_1_data_1D8[(pl->no * 4) + 2];
    obj->rot.y = lbl_1_data_1D8[(pl->no * 4) + 3];
    obj->scale.z = 1.0f;
    obj->scale.y = 1.0f;
    obj->scale.x = 1.0f;
    obj->rot.z = 0.0f;
    obj->rot.x = 0.0f;
    obj->mdlId[1] = Hu3DModelCreate(HuDataSelHeapReadNum(0x550005, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(obj->mdlId[1], 6);
    camNo = (pl->no / 2 == 0) ? 1 : 2;
    Hu3DModelCameraSet(obj->mdlId[1], camNo);
    Hu3DModelAttrSet(obj->mdlId[1], HU3D_MOTATTR_LOOP);
    Hu3DModelPosSet(obj->mdlId[1], obj->trans.x, 0.0f, obj->trans.z);
    if (pl->no % 2 == 1) {
        for (i = 0; i < 9; i++) {
            if (pl->no == 1) {
                obj->mdlId[i + 2] = Hu3DModelCreate(HuDataSelHeapReadNum(i + 0x550009, HU_MEMNUM_OVL, HEAP_MODEL));
            }
            else {
                obj->mdlId[i + 2] = Hu3DModelLink(lbl_1_bss_B8[1]->mdlId[i + 2]);
            }
            tile = obj->mdlId[i + 2];
            Hu3DModelLayerSet(tile, 4);
            camNo = (pl->no / 2 == 0) ? 1 : 2;
            Hu3DModelCameraSet(tile, camNo);
            Hu3DModelAttrSet(tile, HU3D_ATTR_DISPOFF);
            Hu3DLLightInfinitytSet(tile, Hu3DLLightCreate(tile, 0.0f, 0.0f, 500.0f, 3.0f, -4.0f, -2.0f, 255, 255, 255));
        }
    }
    obj->objFunc = fn_1_5B4;
}

void fn_1_CB68(OMOBJ *obj)
{
    lbl_1_bss_AC = M527_MODE_WIPEIN;
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_PREVTYPE, 60);
    obj->objFunc = fn_1_A0D0;
}

// NONMATCHING: local vector/colour initialisation is scheduled differently
void ObjectSetup(void)
{
    HuVecF pos = { 0.0f, 5000.0f, 0.0f };
    HuVecF dir = { 0.75f, -1.0f, -0.5f };
    GXColor color = { 0, 0, 0, 0 };
    HuVecF up;
    HuVecF target;
    s32 i;

    OSReport("******* M527 ObjectSetup *********\n");
    lbl_1_bss_A8 = omInitObjMan(0x258, 0x2000);
    omGameSysInit(lbl_1_bss_A8);
    Hu3DCameraCreate(3);
    Hu3DCameraPerspectiveSet(1, 40.0f, 20.0f, 10000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(1, 0, 0, 640, 480);
    Hu3DCameraPerspectiveSet(2, 40.0f, 20.0f, 10000.0f, 0.6f);
    Hu3DCameraViewportSet(2, 322.0f, 0.0f, 318.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(2, 0, 0, 0, 480);
    for (i = 0; i < 2; i++) {
        CZoomM[i] = 1775.0f;
        CenterM[i].x = 0.0f;
        CenterM[i].y = 340.0f;
        CenterM[i].z = 0.0f;
        CRotM[i].x = -30.0f;
        CRotM[i].y = 0.0f;
        CRotM[i].z = 0.0f;
    }
    Hu3DLighInit();
    lbl_1_bss_9C = Hu3DGLightCreateV(&pos, &dir, &color);
    Hu3DGLightStaticSet(lbl_1_bss_9C, 1);
    Hu3DGLightInfinitytSet(lbl_1_bss_9C);
    pos.x = 345.0f;
    pos.y = 10000.0 * sin(M_PI / 2);
    pos.z = 10000.0 * cos(M_PI / 2);
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    target.x = 346.0f;
    target.y = 0.0f;
    target.z = 0.0f;
    Hu3DShadowMultiCreate(12.0f, 7000.0f, 12000.0f, 3);
    Hu3DShadowMultiTPLvlSet(0.5f, -1);
    Hu3DShadowMultiPosSet(&pos, &up, &target, -1);
    lbl_1_bss_A6 = -1;
    lbl_1_data_158[0] = -1;
    lbl_1_bss_B4 = omAddObjEx(lbl_1_bss_A8, 0x65, 0x10, 2, 1, fn_1_B8C4);
    for (i = 0; i < 4; i++) {
        lbl_1_bss_B8[i] = omAddObjEx(lbl_1_bss_A8, 0x64, 0xB, 0xD, 0, fn_1_C408);
        lbl_1_bss_B8[i]->data = HuMemDirectMallocNum(HEAP_HEAP, 0x6C, HU_MEMNUM_OVL);
        lbl_1_bss_B8[i]->work[0] = i;
    }
    lbl_1_bss_C8 = omAddObjEx(lbl_1_bss_A8, 0x7FDA, 0, 0, -1, omOutViewMulti);
    lbl_1_bss_C8->work[0] = 2;
    lbl_1_bss_CC = omAddObjEx(lbl_1_bss_A8, 0x66, 0, 0, -1, fn_1_CB68);
    lbl_1_bss_B0 = omAddObjEx(lbl_1_bss_A8, 0x67, 1, 0, -1, fn_1_D198);
}

void fn_1_D198(OMOBJ *obj)
{
    if (lbl_1_bss_8 != 0) {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_WHITEFADE, 60);
        obj->objFunc = fn_1_D1F0;
    }
}

void fn_1_D1F0(OMOBJ *obj)
{
    s32 i;
    M527PLAYER *player;

    if (WipeCheckEnd() == 0) {
        for (i = 0; i < 4; i++) {
            player = lbl_1_bss_B8[i]->data;
            CharModelKill(player->charNo);
        }
        if (lbl_1_bss_A6 >= 0) {
            GameMesKill(lbl_1_bss_A6);
        }
        GameMesClose();
        HuAudAllStop();
        omOvlReturnEx(lbl_1_data_164, 1);
    }
}
