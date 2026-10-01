#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/sprite.h"
#include "game/wipe.h"
#include "game/mg/seqman.h"

#include "math.h"
#include "humath.h"

#include "datanum/charmot.h"
#include "datadir_enum.h"

#include "REL/m518dll.h"

#define M518_GRID_SIZE 12

/* Sleep for a number of frames; leave the current mode function as soon as its mode changes. */
#define M518_WAIT(frames)                                                        \
    {                                                                            \
        int i;                                                                   \
        s16 mode;                                                                \
        s16 prevMode;                                                            \
        int time = (frames);                                                     \
        for (i = 0; i < time; i++) {                                             \
            HuPrcVSleep();                                                       \
            mode = ((M518_OBJHEAD *)HuPrcCurrentGet()->property)->mode;          \
            prevMode = ((M518_OBJHEAD *)HuPrcCurrentGet()->property)->prevMode;  \
            ((M518_OBJHEAD *)HuPrcCurrentGet()->property)->prevMode = mode;      \
            if (mode != prevMode) {                                              \
                return;                                                          \
            }                                                                    \
        }                                                                        \
    }

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

void ObjectSetup(void);
void fn_1_184(void);
void fn_1_1F4(s16 mode, s16 frameNo);
void fn_1_728(s16 mode, s16 frameNo);
void fn_1_1C3C(void);
void fn_1_1CD8(s16 mode, s16 frameNo);
void fn_1_1D54(s16 mode, s16 frameNo);
void fn_1_1E28(s16 mode, s16 frameNo);
void fn_1_1EFC(s16 mode, s16 frameNo);
void fn_1_2C6C(s16 mode, s16 frameNo);
void fn_1_2CFC(void);

M518_PLAYER *fn_1_2D24(s16 playerNo, s16 no, s16 type);
void fn_1_31F0(void);
void fn_1_3230(M518_PLAYER *player, HuVecF pos);
void fn_1_32C8(M518_PLAYER *player, HuVecF rot);
void fn_1_3440(M518_PLAYER *player, HuVecF scale);
void fn_1_34D8(M518_PLAYER *player, float tpLvl);
void fn_1_3554(M518_PLAYER *player, M518_CELL cell);
void fn_1_37D0(M518_PLAYER *player, M518_CELL cell, u16 value);
void fn_1_3904(M518_PLAYER *player);
M518_CELL fn_1_4674(M518_PLAYER *player);
void fn_1_468C(M518_PLAYER *player);
void fn_1_4698(M518_PLAYER *player);
void fn_1_4728(M518_PLAYER *player);
void fn_1_4734(float *a, float *b);
void fn_1_4760(HuVecF *rot, HuVecF *axis);
void fn_1_4B80(M518_PLAYER *player);
void fn_1_6C0C(M518_PLAYER *player);
void fn_1_6CFC(M518_PLAYER *player);
void fn_1_6D84(M518_PLAYER *player);
void fn_1_6E08(M518_PLAYER *player);
void fn_1_7908(M518_PLAYER *player);
void fn_1_83E8(M518_PLAYER *player, s16 time, s16 delay);
void fn_1_83FC(M518_PLAYER *player);
void fn_1_88B4(M518_PLAYER *player, s16 time, s16 delay);
void fn_1_88C8(M518_PLAYER *player);
void fn_1_8E20(M518_PLAYER *player);
void fn_1_8E2C(M518_PLAYER *player);
void fn_1_8FA8(M518_PLAYER *player);
void fn_1_9014(M518_PLAYER *player);
s16 fn_1_9080(M518_PLAYER *player, M518_CELL cell);
s16 fn_1_9258(M518_PLAYER *player, M518_QUEUE *queue, M518_CELL cell, s16 dist, s32 dir, s16 len);

M518_MAP *fn_1_9860(void);
void fn_1_9BB0(void);
void fn_1_9BF0(M518_MAP *map);
void fn_1_9C4C(M518_MAP *map, M518_CELL cell, u16 value);
u16 fn_1_9CBC(M518_MAP *map, M518_CELL cell);
void fn_1_9D30(M518_MAP *map);
void fn_1_9D98(M518_MAP *map);
void fn_1_9E04(M518_MAP *map);
void fn_1_9E70(M518_MAP *map);
void fn_1_9ECC(M518_MAP *map);
void fn_1_9ED8(M518_MAP *map);
void fn_1_A200(M518_MAP *map, M518_CELL cell, u16 value);
u16 fn_1_A268(M518_MAP *map, M518_CELL cell);
void fn_1_A2D8(M518_MAP *map, M518_CELL cell, s16 maxDist);
void fn_1_A480(M518_MAP *map, M518_CELL cell, s16 dist);

M518_CHAR *fn_1_A9A4(s16 playerNo);
void fn_1_AB3C(void);
void fn_1_AB7C(M518_CHAR *chr, HuVecF target, s16 time, s16 delay);
float fn_1_ABA8(float start, float end, float t);
void fn_1_ACAC(M518_CHAR *chr);
void fn_1_B600(M518_CHAR *chr);
void fn_1_B60C(M518_CHAR *chr);
void fn_1_B6AC(M518_CHAR *chr);
void fn_1_B6B8(M518_CHAR *chr);
void fn_1_B7A0(M518_CHAR *chr);
void fn_1_B7AC(M518_CHAR *chr);
void fn_1_BB00(M518_CHAR *chr, HuVecF pos);
void fn_1_BB60(M518_CHAR *chr, HuVecF rot);
void fn_1_BBC0(M518_CHAR *chr);
void fn_1_BBF4(M518_CHAR *chr);

M518_PODIUM *fn_1_BC28(void);
void fn_1_BD88(void);
void fn_1_BDC8(M518_PODIUM *podium, HuVecF pos);
void fn_1_BE60(M518_PODIUM *podium);
void fn_1_BE6C(M518_PODIUM *podium);
void fn_1_BEFC(M518_PODIUM *podium);
void fn_1_BF54(M518_PODIUM *podium);

M518_CROWD *fn_1_BFAC(s16 no, s16 type);
void fn_1_C224(void);
void fn_1_C264(M518_CROWD *crowd);
void fn_1_C298(M518_CROWD *crowd, HuVecF pos);
void fn_1_C2F8(M518_CROWD *crowd, HuVecF rot);
void fn_1_C358(M518_CROWD *crowd, HuVecF scale);
void fn_1_C3B8(M518_CROWD *crowd);
void fn_1_C3C4(M518_CROWD *crowd);
void fn_1_C454(M518_CROWD *crowd, HuVecF target, s16 time, s16 delay);
void fn_1_C480(M518_CROWD *crowd);

M518_BLOCK *fn_1_C9F4(s16 no, s16 type);
void fn_1_CB9C(void);
void fn_1_CBDC(M518_BLOCK *block);
void fn_1_CBE8(M518_BLOCK *block);
void fn_1_CCC0(M518_BLOCK *block, s16 time, s16 delay);
void fn_1_CCD4(M518_BLOCK *block);
void fn_1_CE54(M518_BLOCK *block, float depth, s16 time, s16 delay, s16 playerNo);
void fn_1_CE78(M518_BLOCK *block, float depth, s16 time, s16 delay);
void fn_1_CEA0(M518_BLOCK *block);
void fn_1_D204(M518_BLOCK *block, s16 time, s16 delay);
void fn_1_D218(M518_BLOCK *block);
void fn_1_D578(M518_BLOCK *block, s16 time, s16 delay);
void fn_1_D58C(M518_BLOCK *block);
void fn_1_D958(M518_BLOCK *block, M518_CELL cell);
void fn_1_DA7C(M518_BLOCK *block, HuVecF pos);
void fn_1_DADC(M518_BLOCK *block, HuVecF rot);

M518_QUEUE *fn_1_DC14(int size);
BOOL fn_1_DC84(M518_QUEUE *queue, s32 value);
BOOL fn_1_DCBC(M518_QUEUE *queue, s32 *value);
void fn_1_DCF8(M518_QUEUE *queue);
BOOL fn_1_DD0C(M518_QUEUE *queue);

M518_EFFECT *fn_1_DD40(void);
void fn_1_DE2C(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
void fn_1_DE8C(M518_EFFECT *effect);
void fn_1_DEF0(M518_EFFECT *effect, HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
void fn_1_DEF4(M518_EFFECT *effect);
void fn_1_E2A0(M518_EFFECT *effect, HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
BOOL fn_1_E508(HuVecF pos, float radius);
s16 fn_1_E5BC(HuVecF pos);
s16 fn_1_E6B8(HuVecF pos);
int fn_1_E7B4(int seId, HuVecF pos);

typedef struct M518LightData_s {
    HuVecF pos;
    HuVecF target;
    GXColor color;
} M518_LIGHTDATA;

static const M518_LIGHTDATA lbl_1_rodata_10 = { { -4000.0f, 10000.0f, 4000.0f }, { 0.0f, 0.0f, -2000.0f }, { 255, 255, 255, 255 } };
static const HuVecF lbl_1_rodata_2C[3] = { { -2500.0f, 6500.0f, 2500.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
static const s32 lbl_1_rodata_50[4] = { 1, 0, 3, 2 };
static const M518_CELL lbl_1_rodata_60[5] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 }, { 0, 0 } };

static const s8 lbl_1_rodata_6A[7][M518_GRID_SIZE * M518_GRID_SIZE] = {
    {
        7, 7, 7, 7, 7, 9, 9, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 9, 9, 9, 9, 7, 7, 7, 7,
        5, 7, 9, 1, 9, 9, 9, 9, 1, 9, 7, 5,
        5, 5, 1, 0, 1, 9, 9, 1, 0, 1, 5, 5,
        5, 9, 9, 0, 1, 1, 1, 1, 0, 9, 9, 5,
        9, 9, 9, 0, 1, 9, 9, 1, 0, 9, 9, 9,
        9, 9, 9, 0, 0, 9, 9, 0, 0, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        5, 9, 9, 9, 0, 0, 0, 0, 9, 9, 9, 5,
        5, 5, 5, 0, 0, 0, 0, 0, 0, 5, 5, 5,
        5, 1, 1, 1, 0, 0, 0, 0, 0, 5, 5, 5,
        1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 5, 5,
    },
    {
        3, 3, 3, 3, 3, 5, 5, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 0, 5, 5, 5, 3, 3, 3, 3,
        3, 3, 3, 0, 0, 5, 5, 5, 5, 3, 3, 3,
        3, 3, 3, 0, 0, 0, 5, 5, 5, 3, 3, 3,
        3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3,
        3, 3, 0, 0, 5, 5, 0, 0, 0, 0, 3, 3,
        3, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 3,
        3, 0, 0, 5, 5, 5, 5, 0, 0, 5, 5, 3,
        3, 0, 0, 5, 5, 5, 5, 0, 5, 5, 5, 3,
        3, 0, 0, 0, 5, 5, 0, 0, 5, 5, 5, 3,
        3, 3, 0, 0, 0, 0, 0, 5, 5, 5, 3, 3,
        3, 3, 3, 0, 0, 0, 0, 5, 5, 3, 3, 3,
    },
    {
        3, 3, 3, 4, 4, 4, 4, 3, 3, 3, 3, 3,
        3, 3, 3, 4, 4, 4, 4, 4, 0, 0, 3, 3,
        3, 0, 2, 0, 2, 2, 2, 0, 0, 0, 3, 3,
        0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 3, 3,
        0, 1, 0, 1, 0, 2, 0, 0, 0, 2, 4, 4,
        0, 1, 0, 1, 0, 2, 2, 2, 2, 2, 4, 4,
        0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 4,
        3, 4, 4, 2, 2, 2, 2, 2, 2, 2, 4, 4,
        3, 1, 1, 4, 2, 2, 2, 2, 2, 3, 4, 4,
        3, 4, 4, 0, 0, 0, 0, 2, 2, 3, 3, 3,
        3, 3, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    },
    {
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        7, 0, 0, 0, 7, 2, 2, 2, 7, 7, 7, 7,
        7, 0, 0, 2, 2, 2, 2, 2, 2, 7, 7, 7,
        7, 2, 2, 2, 0, 1, 0, 9, 9, 7, 7, 7,
        7, 2, 0, 0, 9, 0, 0, 9, 0, 9, 7, 7,
        7, 2, 2, 9, 9, 9, 0, 0, 9, 9, 7, 7,
        7, 7, 2, 2, 0, 0, 0, 0, 9, 7, 7, 7,
        7, 7, 2, 2, 3, 2, 2, 3, 2, 2, 2, 7,
        7, 7, 7, 3, 2, 2, 3, 2, 2, 2, 2, 7,
        7, 7, 7, 4, 2, 2, 4, 2, 2, 0, 0, 7,
        7, 7, 7, 3, 3, 3, 3, 3, 3, 0, 0, 7,
        7, 7, 3, 3, 3, 3, 3, 3, 3, 3, 7, 7,
    },
    {
        3, 3, 4, 3, 3, 3, 3, 3, 3, 4, 3, 3,
        3, 4, 8, 4, 3, 3, 3, 3, 4, 8, 4, 3,
        4, 8, 8, 8, 4, 3, 3, 4, 8, 8, 8, 4,
        4, 8, 8, 8, 8, 4, 4, 8, 8, 8, 8, 4,
        4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4,
        4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4,
        3, 4, 8, 8, 8, 8, 8, 8, 8, 8, 4, 3,
        3, 4, 8, 8, 8, 8, 8, 8, 8, 8, 4, 3,
        3, 3, 4, 8, 8, 8, 8, 8, 8, 4, 3, 3,
        3, 3, 3, 4, 8, 8, 8, 8, 4, 3, 3, 3,
        3, 3, 3, 3, 4, 8, 8, 4, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 4, 4, 3, 3, 3, 3, 3,
    },
    {
        6, 6, 6, 7, 7, 3, 3, 7, 7, 6, 6, 6,
        6, 6, 7, 3, 4, 4, 4, 4, 3, 7, 6, 6,
        6, 7, 3, 4, 1, 4, 4, 1, 4, 3, 7, 6,
        6, 7, 3, 4, 1, 4, 4, 1, 4, 3, 7, 6,
        6, 6, 7, 3, 4, 4, 4, 4, 3, 7, 6, 6,
        6, 6, 7, 7, 3, 3, 3, 3, 7, 7, 6, 6,
        6, 6, 6, 6, 7, 7, 7, 7, 6, 6, 6, 6,
        6, 6, 6, 6, 6, 6, 5, 6, 6, 6, 6, 6,
        6, 5, 6, 6, 6, 6, 5, 6, 6, 6, 5, 6,
        6, 6, 5, 5, 6, 5, 6, 6, 5, 5, 6, 6,
        6, 6, 6, 5, 5, 5, 6, 5, 5, 6, 6, 6,
        6, 6, 6, 6, 6, 5, 5, 6, 6, 6, 6, 6,
    },
    {
        7, 7, 7, 7, 2, 0, 0, 2, 7, 7, 7, 7,
        7, 7, 2, 2, 0, 0, 0, 0, 2, 2, 7, 7,
        7, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 7,
        2, 0, 2, 2, 2, 0, 0, 2, 2, 2, 0, 2,
        0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0,
        0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0,
        0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0,
        2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 2,
        7, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 7,
        7, 7, 0, 0, 1, 0, 0, 1, 0, 0, 7, 7,
        7, 7, 0, 0, 1, 0, 0, 1, 0, 0, 7, 7,
        7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7,
    },
};

static const s8 lbl_1_rodata_45A[10] = { 0, 0, 0, 1, 0, 1, 0, 1, 0, 0 };

OMOBJ *lbl_1_bss_4F0;
M518_PLAYER *lbl_1_bss_4E0[4];
M518_MAP *lbl_1_bss_4DC;
M518_CROWD *lbl_1_bss_3AC[76];
M518_CHAR *lbl_1_bss_39C[4];
M518_PODIUM *lbl_1_bss_390[3];
M518_BLOCK *lbl_1_bss_150[M518_GRID_SIZE * M518_GRID_SIZE];
M518_EFFECT *lbl_1_bss_50[64];
OMCAMERAVIEW lbl_1_bss_34;
s16 lbl_1_bss_30;
s32 lbl_1_bss_2C;
s32 lbl_1_bss_28;
s32 lbl_1_bss_24;
s32 lbl_1_bss_20;
s32 lbl_1_bss_1C;
s16 lbl_1_bss_18;
s16 lbl_1_bss_4[10];
BOOL lbl_1_bss_0;

int _prolog(void)
{
    const VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    OMOBJMAN *objman;
    MGSEQ_PARAM param;

    objman = omInitObjMan(1, 0x2000);
    lbl_1_bss_4F0 = omAddObjEx(objman, 0x1000, 0, 0, 0, omOutView);
    omGameSysInit(objman);
    param.maxTime = 30;
    param.timerPos = MGSEQ_TIMER_TOP;
    param.initHook = fn_1_1F4;
    param.fadeInHook = fn_1_728;
    param.startHook = fn_1_1CD8;
    param.mainHook = fn_1_1D54;
    param.finishHook = fn_1_1E28;
    param.preWinnerHook = fn_1_1EFC;
    param.winnerHook = fn_1_2C6C;
    param.fadeOutHook = NULL;
    param.closeHook = NULL;
    MgSeqCreate(&param);
}

void fn_1_184(void)
{
    while (omSysExitReq == 0) {
        HuPrcVSleep();
    }
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
    omOvlReturnEx(1, TRUE);
    HuPrcEnd();
}

void fn_1_1F4(s16 mode, s16 frameNo)
{
    int i;
    int num;
    s16 layout;
    s16 unused;
    s16 grp[4];

    for (i = 0; i < 4; i++) {
        grp[i] = GwPlayerConf[i].grp;
    }
    num = 0;
    for (i = 0; i < 4; i++) {
        if (grp[i] == 0) {
            num++;
        }
    }
    if (num == 1) {
        for (i = 0; i < 4; i++) {
            if (grp[i] != 0) {
                grp[i] = 1;
            }
        }
    }
    else {
        grp[0] = 0;
        grp[1] = 1;
        grp[2] = 1;
        grp[3] = 1;
    }
    lbl_1_bss_4DC = fn_1_9860();
    unused = frandmod(10);
    layout = frandmod(7);
    for (i = 0; i < M518_GRID_SIZE * M518_GRID_SIZE; i++) {
        lbl_1_bss_150[i] = fn_1_C9F4(i, lbl_1_rodata_6A[layout][i]);
    }
    num = 0;
    for (i = 0; i < 4; i++) {
        if (grp[i] == 0) {
            lbl_1_bss_4E0[num] = fn_1_2D24(i, num, 0);
            lbl_1_bss_39C[num] = fn_1_A9A4(i);
            num++;
            break;
        }
    }
    for (i = 0; i < 4; i++) {
        if (grp[i] == 1) {
            lbl_1_bss_4E0[num] = fn_1_2D24(i, num, 1);
            lbl_1_bss_39C[num] = fn_1_A9A4(i);
            num++;
        }
    }
    for (i = 0; i < 3; i++) {
        lbl_1_bss_390[i] = fn_1_BC28();
        fn_1_BF54(lbl_1_bss_390[i]);
    }
    for (i = 0; i < 76; i++) {
        lbl_1_bss_3AC[i] = fn_1_BFAC(i, frandmod(2));
    }
    for (i = 0; i < 64; i++) {
        lbl_1_bss_50[i] = fn_1_DD40();
    }
    lbl_1_bss_30 = Hu3DGLightCreate(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    Hu3DGLightPosAimSetV(lbl_1_bss_30, (HuVecF *)&lbl_1_rodata_10.pos, (HuVecF *)&lbl_1_rodata_10.target);
    Hu3DGLightColorSet(lbl_1_bss_30, lbl_1_rodata_10.color.r, lbl_1_rodata_10.color.g, lbl_1_rodata_10.color.b, lbl_1_rodata_10.color.a);
    Hu3DGLightStaticSet(lbl_1_bss_30, TRUE);
    Hu3DShadowMultiCreate(40.0f, 10.0f, 20000.0f, HU3D_CAM0);
    Hu3DShadowMultiTPLvlSet(0.5f, HU3D_CAM0);
    Hu3DShadowMultiPosSet((HuVecF *)&lbl_1_rodata_2C[0], (HuVecF *)&lbl_1_rodata_2C[1], (HuVecF *)&lbl_1_rodata_2C[2], HU3D_CAM0);
    Hu3DCameraCreate(HU3D_CAM0);
    Hu3DCameraPerspectiveSet(HU3D_CAM0, 40.0f, 100.0f, 10000.0f, 1.2f);
    Hu3DCameraViewportSet(HU3D_CAM0, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(HU3D_CAM0, 0, 0, 640, 480);
    lbl_1_bss_2C = 0;
    lbl_1_bss_20 = 1;
    lbl_1_bss_1C = 0;
    MgSeqModeNext();
}

static const M518_CELL lbl_1_rodata_490[4] = { { 0, 0 }, { 10, 0 }, { 0, 10 }, { 10, 10 } };

typedef struct M518CharStart_s {
    HuVecF pos;
    HuVecF rot;
} M518_CHARSTART;

static const M518_CHARSTART lbl_1_rodata_498[4] = {
    { { -1325.0f, 0.0f, -1100.0f }, { 0.0f, 90.0f, 0.0f } },
    { { 3000.0f, 0.0f, 3000.0f }, { 0.0f, -90.0f, 0.0f } },
    { { 3000.0f, 0.0f, 3000.0f }, { 0.0f, -90.0f, 0.0f } },
    { { 3000.0f, 0.0f, 3000.0f }, { 0.0f, -90.0f, 0.0f } },
};

void fn_1_728(s16 mode, s16 frameNo)
{
    int j;
    int i;
    int k;
    float angle;
    HuVecF pos;
    HuVecF rot;
    HuVecF scale;
    M518_CELL cell;

    switch (frameNo) {
        case 0:
            MgSeqModeChangeOff();
            for (i = 0; i < 4; i++) {
                fn_1_3554(lbl_1_bss_4E0[i], lbl_1_rodata_490[i]);
                pos = lbl_1_bss_4E0[i]->pos;
                pos.y += 8500.0f;
                fn_1_3230(lbl_1_bss_4E0[i], pos);
                fn_1_88B4(lbl_1_bss_4E0[i], 90, 90);
            }
            for (i = 0; i < 4; i++) {
                fn_1_BB00(lbl_1_bss_39C[i], lbl_1_rodata_498[i].pos);
                fn_1_BB60(lbl_1_bss_39C[i], lbl_1_rodata_498[i].rot);
            }
            k = 0;
            for (j = 0; j < M518_GRID_SIZE; j++) {
                for (i = 0; i < M518_GRID_SIZE; i++, k++) {
                    cell.x = i;
                    cell.z = j;
                    fn_1_D958(lbl_1_bss_150[k], cell);
                    pos = lbl_1_bss_150[k]->pos;
                    pos.y += 4500.0f + (s32)frandmod(3000);
                    fn_1_DA7C(lbl_1_bss_150[k], pos);
                    rot.x = 90.0f * (s32)frandmod(3);
                    rot.y = 90.0f * (s32)frandmod(3);
                    rot.z = 90.0f * (s32)frandmod(3);
                    fn_1_DADC(lbl_1_bss_150[k], rot);
                    fn_1_D578(lbl_1_bss_150[k], frandmod(90) + 60, 0);
                }
            }
            k = 0;
            for (j = 0; j < 4; j++) {
                for (i = 0; i < j + 18; i++) {
                    angle = 180.0f * ((1.0f / (j + 18)) * i);
                    if (angle >= 60.0f && angle < 120.0f) {
                        pos.x = (3000.0f + 260.0f * j) * HuCos(angle);
                        pos.z = -(1900.0f + 260.0f * j) * HuSin(angle);
                        pos.y = -200.0f;
                        fn_1_C298(lbl_1_bss_3AC[k], pos);
                        rot.y = 180.0 + HuAtan(pos.x, pos.z);
                        rot.x = 0.0f;
                        rot.z = 0.0f;
                        fn_1_C2F8(lbl_1_bss_3AC[k], rot);
                        scale.x = 1.0f - 0.02f * j;
                        scale.y = 1.0f - 0.02f * j;
                        scale.z = 1.0f - 0.02f * j;
                        fn_1_C358(lbl_1_bss_3AC[k], scale);
                        k++;
                    }
                }
            }
            for (j = 0; j < 2; j++) {
                for (i = 0; i < j + 18; i++) {
                    angle = 180.0f * ((1.0f / (j + 18)) * i);
                    if (angle >= 60.0f && angle < 120.0f) {
                        pos.x = (3000.0f + 260.0f * j) * HuCos(180.0f + angle);
                        pos.z = -(1900.0f + 260.0f * j) * HuSin(180.0f + angle);
                        pos.y = -200.0f;
                        fn_1_C298(lbl_1_bss_3AC[k], pos);
                        rot.y = 180.0 + HuAtan(pos.x, pos.z);
                        rot.x = 0.0f;
                        rot.z = 0.0f;
                        fn_1_C2F8(lbl_1_bss_3AC[k], rot);
                        scale.x = 1.0f - 0.02f * j;
                        scale.y = 1.0f - 0.02f * j;
                        scale.z = 1.0f - 0.02f * j;
                        fn_1_C358(lbl_1_bss_3AC[k], scale);
                        k++;
                    }
                }
            }
            for (j = 0; j < 3; j++) {
                for (i = 0; i < j + 18; i++) {
                    angle = 180.0f * ((1.0f / (j + 18)) * i);
                    if (angle >= 60.0f && angle < 120.0f) {
                        pos.x = (1900.0f + 260.0f * j) * HuCos(90.0f + angle);
                        pos.z = -(3000.0f + 260.0f * j) * HuSin(90.0f + angle);
                        pos.y = -200.0f;
                        fn_1_C298(lbl_1_bss_3AC[k], pos);
                        rot.y = 180.0 + HuAtan(pos.x, pos.z);
                        rot.x = 0.0f;
                        rot.z = 0.0f;
                        fn_1_C2F8(lbl_1_bss_3AC[k], rot);
                        scale.x = 1.0f - 0.02f * j;
                        scale.y = 1.0f - 0.02f * j;
                        scale.z = 1.0f - 0.02f * j;
                        fn_1_C358(lbl_1_bss_3AC[k], scale);
                        k++;
                    }
                }
            }
            for (j = 0; j < 3; j++) {
                for (i = 0; i < j + 18; i++) {
                    angle = 180.0f * ((1.0f / (j + 18)) * i);
                    if (angle >= 60.0f && angle < 120.0f) {
                        pos.x = (1900.0f + 260.0f * j) * HuCos(-90.0f + angle);
                        pos.z = -(3000.0f + 260.0f * j) * HuSin(-90.0f + angle);
                        pos.y = -200.0f;
                        fn_1_C298(lbl_1_bss_3AC[k], pos);
                        rot.y = 180.0 + HuAtan(pos.x, pos.z);
                        rot.x = 0.0f;
                        rot.z = 0.0f;
                        fn_1_C2F8(lbl_1_bss_3AC[k], rot);
                        scale.x = 1.0f - 0.02f * j;
                        scale.y = 1.0f - 0.02f * j;
                        scale.z = 1.0f - 0.02f * j;
                        fn_1_C358(lbl_1_bss_3AC[k], scale);
                        k++;
                    }
                }
            }
            lbl_1_bss_34.center.x = 0.0f;
            lbl_1_bss_34.center.y = 0.0f;
            lbl_1_bss_34.center.z = 0.0f;
            lbl_1_bss_34.rot.x = -80.0f;
            lbl_1_bss_34.rot.y = 0.0f;
            lbl_1_bss_34.rot.z = 0.0f;
            lbl_1_bss_34.zoom = 5500.0f;
            omCameraViewSet(&lbl_1_bss_34);
            lbl_1_bss_34.center.z = 100.0f;
            lbl_1_bss_34.rot.x = -60.0f;
            lbl_1_bss_34.zoom = 3800.0f;
            omCameraViewMove(&lbl_1_bss_34, 150, 3);
            break;
        case 184:
            HuAudFXPlay(0x705);
            HuAudFXPlay(0x704);
            fn_1_1C3C();
            fn_1_B7A0(lbl_1_bss_39C[0]);
            break;
        case 212:
            fn_1_9D30(lbl_1_bss_4DC);
            fn_1_8E20(lbl_1_bss_4E0[0]);
            break;
        case 242:
            HuAudFXPlay(0x706);
            break;
        case 272:
            HuAudFXPlay(0x706);
            break;
        case 280:
            MgSeqModeChangeOn();
            break;
    }
}

void fn_1_1C3C(void)
{
    int x;
    int z;
    M518_BLOCK *block;

    for (z = 0; z < M518_GRID_SIZE; z++) {
        for (x = 0; x < M518_GRID_SIZE; x++) {
            block = lbl_1_bss_150[x + z * M518_GRID_SIZE];
            if (block->mode != 1) {
                fn_1_CCC0(block, 10, (x + z) * 2);
            }
        }
    }
}

void fn_1_1CD8(s16 mode, s16 frameNo)
{
    int i;

    switch (frameNo) {
        case 0:
            lbl_1_bss_18 = HuAudSStreamPlay(50);
            for (i = 0; i < 4; i++) {
                fn_1_BBF4(lbl_1_bss_39C[i]);
            }
            break;
    }
}

void fn_1_1D54(s16 mode, s16 frameNo)
{
    int i;

    switch (frameNo) {
        case 0:
            lbl_1_bss_2C = 1;
            for (i = 0; i < 4; i++) {
                fn_1_4728(lbl_1_bss_4E0[i]);
            }
            lbl_1_bss_28 = 0;
            lbl_1_bss_24 = 0;
            break;
        default:
            if (lbl_1_bss_24) {
                lbl_1_bss_2C = 0;
            }
            if (lbl_1_bss_28) {
                MgSeqModeNext();
            }
            break;
    }
}

void fn_1_1E28(s16 mode, s16 frameNo)
{
    switch (frameNo) {
        case 0:
            lbl_1_bss_2C = 0;
            HuAudSStreamFadeOut(lbl_1_bss_18, 100);
            break;
        case 40:
            if (lbl_1_bss_20) {
                MgSeqWinnerSet(lbl_1_bss_39C[0]->charNo, CHARNO_NONE, CHARNO_NONE, CHARNO_NONE);
            }
            else {
                MgSeqWinnerSet(lbl_1_bss_39C[1]->charNo, lbl_1_bss_39C[2]->charNo, lbl_1_bss_39C[3]->charNo, CHARNO_NONE);
            }
            break;
    }
}

void fn_1_1EFC(s16 mode, s16 frameNo)
{
    int i;
    int k;
    int j;
    float angle;
    HuVecF pos;
    HuVecF rot;
    HuVecF scale;

    switch (frameNo) {
        case 0:
            WipeColorSet(255, 255, 255);
            WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
            break;
        case 30:
            Hu3DFogSet(4800.0f, 8000.0f, 255, 255, 255);
            for (i = 0; i < 4; i++) {
                fn_1_468C(lbl_1_bss_4E0[i]);
                fn_1_9014(lbl_1_bss_4E0[i]);
            }
            fn_1_9E70(lbl_1_bss_4DC);
            k = 0;
            for (j = 0; j < M518_GRID_SIZE; j++) {
                for (i = 0; i < M518_GRID_SIZE; i++, k++) {
                    fn_1_CBDC(lbl_1_bss_150[k]);
                    pos = lbl_1_bss_150[k]->pos;
                    pos.y = -100.0f;
                    fn_1_DA7C(lbl_1_bss_150[k], pos);
                }
            }
            k = 0;
            for (j = 0; j < 20; j++) {
                for (i = 0; i < 20; i++) {
                    angle = 180.0f * ((1.0f / (j + 20)) * i);
                    if (angle >= 80.0f && angle < 100.0f) {
                        pos.x = 2000.0 + ((800.0f + 260.0f * j) * HuCos(-90.0f + angle));
                        pos.z = -2500.0 - ((1500.0f + 260.0f * j) * HuSin(-90.0f + angle));
                        pos.y = -200.0f;
                        fn_1_C298(lbl_1_bss_3AC[k], pos);
                        scale.x = 0.9f;
                        scale.y = 0.9f;
                        scale.z = 0.9f;
                        fn_1_C358(lbl_1_bss_3AC[k], scale);
                        pos.x = -2500.0f;
                        fn_1_C454(lbl_1_bss_3AC[k], pos, 380, j * 10 + 60 + frandmod(6));
                        k++;
                    }
                }
            }
            OSReport("------------------------> %d\n", k);
            for (; k < 76; k++) {
                fn_1_C264(lbl_1_bss_3AC[k]);
            }
            OSReport("------------------------> %d\n", k);
            if (lbl_1_bss_20) {
                GWMgCoinSet(lbl_1_bss_4E0[0]->playerNo, 10);
                lbl_1_bss_34.center.x = 0.0f;
                lbl_1_bss_34.center.y = 239.16f;
                lbl_1_bss_34.center.z = 100.09f;
                lbl_1_bss_34.rot.x = -18.8f;
                lbl_1_bss_34.rot.y = 0.0f;
                lbl_1_bss_34.rot.z = 0.0f;
                lbl_1_bss_34.zoom = 1186.0f;
                omCameraViewSet(&lbl_1_bss_34);
                lbl_1_bss_34.zoom = 710.89f;
                omCameraViewMove(&lbl_1_bss_34, 600, 0);
                pos.x = 0.0f;
                pos.y = 0.0f;
                pos.z = 0.0f;
                fn_1_BDC8(lbl_1_bss_390[0], pos);
                fn_1_BEFC(lbl_1_bss_390[0]);
                pos.x = 0.0f;
                pos.y = 200.0f;
                pos.z = 0.0f;
                fn_1_BB00(lbl_1_bss_39C[0], pos);
                rot.x = 0.0f;
                rot.y = 0.0f;
                rot.z = 0.0f;
                fn_1_BB60(lbl_1_bss_39C[0], rot);
                fn_1_BBC0(lbl_1_bss_39C[0]);
                fn_1_B600(lbl_1_bss_39C[0]);
                for (i = 0; i < 3; i++) {
                    fn_1_BBC0(lbl_1_bss_39C[i + 1]);
                    pos.x = 2000.0f + 200.0f * i;
                    pos.y = -200.0f;
                    pos.z = -2400.0f - 100.0f * i;
                    fn_1_BB00(lbl_1_bss_39C[i + 1], pos);
                    pos.x -= 3600.0f;
                    fn_1_AB7C(lbl_1_bss_39C[i + 1], pos, 240, 0);
                }
            }
            else {
                GWMgCoinSet(lbl_1_bss_4E0[1]->playerNo, 10);
                GWMgCoinSet(lbl_1_bss_4E0[2]->playerNo, 10);
                GWMgCoinSet(lbl_1_bss_4E0[3]->playerNo, 10);
                lbl_1_bss_34.center.x = 0.0f;
                lbl_1_bss_34.center.y = 334.32f;
                lbl_1_bss_34.center.z = 100.09f;
                lbl_1_bss_34.rot.x = -18.8f;
                lbl_1_bss_34.rot.y = 0.0f;
                lbl_1_bss_34.rot.z = 0.0f;
                lbl_1_bss_34.zoom = 1600.0f;
                omCameraViewSet(&lbl_1_bss_34);
                lbl_1_bss_34.zoom = 1124.0f;
                omCameraViewMove(&lbl_1_bss_34, 600, 0);
                for (i = 0; i < 3; i++) {
                    pos.x = -400.0f + 400.0f * i;
                    pos.y = 0.0f;
                    pos.z = 0.0f;
                    fn_1_BDC8(lbl_1_bss_390[i], pos);
                    fn_1_BEFC(lbl_1_bss_390[i]);
                    pos.x = -400.0f + 400.0f * i;
                    pos.y = 200.0f;
                    pos.z = 0.0f;
                    fn_1_BB00(lbl_1_bss_39C[i + 1], pos);
                    rot.x = 0.0f;
                    rot.y = 0.0f;
                    rot.z = 0.0f;
                    fn_1_BB60(lbl_1_bss_39C[i + 1], rot);
                    fn_1_BBC0(lbl_1_bss_39C[i + 1]);
                    fn_1_B600(lbl_1_bss_39C[i + 1]);
                }
                fn_1_BBC0(lbl_1_bss_39C[0]);
                pos.x = 2000.0f;
                pos.y = -200.0f;
                pos.z = -2500.0f;
                fn_1_BB00(lbl_1_bss_39C[0], pos);
                pos.x -= 3600.0f;
                fn_1_AB7C(lbl_1_bss_39C[0], pos, 240, 0);
            }
            break;
        case 40:
            WipeColorSet(255, 255, 255);
            WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
            break;
        case 80:
            MgSeqModeNext();
            break;
    }
}

void fn_1_2C6C(s16 mode, s16 frameNo)
{
    int i;

    switch (frameNo) {
        case 0:
            if (lbl_1_bss_20) {
                fn_1_B6AC(lbl_1_bss_39C[0]);
            }
            else {
                for (i = 0; i < 3; i++) {
                    fn_1_B6AC(lbl_1_bss_39C[i + 1]);
                }
            }
            break;
    }
}

void fn_1_2CFC(void)
{
    omOvlReturnEx(0, TRUE);
}

typedef void (*M518_PLAYERFUNC)(M518_PLAYER *player);

M518_PLAYERFUNC lbl_1_data_20[5] = { fn_1_4698, fn_1_4B80, fn_1_83FC, fn_1_8E2C, fn_1_88C8 };

static const s32 lbl_1_rodata_5BC[13][2] = {
    { DATANUM(DATA_m518, 0x14), DATANUM(DATA_m518, 0x22) },
    { DATANUM(DATA_m518, 0x15), DATANUM(DATA_m518, 0x22) },
    { DATANUM(DATA_m518, 0x16), DATANUM(DATA_m518, 0x22) },
    { DATANUM(DATA_m518, 0x17), DATANUM(DATA_m518, 0x22) },
    { DATANUM(DATA_m518, 0x18), DATANUM(DATA_m518, 0x22) },
    { DATANUM(DATA_m518, 0x19), DATANUM(DATA_m518, 0x22) },
    { DATANUM(DATA_m518, 0x1A), DATANUM(DATA_m518, 0x22) },
    { DATANUM(DATA_m518, 0x1B), DATANUM(DATA_m518, 0x22) },
    { DATANUM(DATA_m518, 0x1C), DATANUM(DATA_m518, 0x22) },
    { DATANUM(DATA_m518, 0x1D), DATANUM(DATA_m518, 0x22) },
    { DATANUM(DATA_m518, 0x1E), DATANUM(DATA_m518, 0x22) },
    { DATANUM(DATA_m518, 0x1F), DATANUM(DATA_m518, 0x22) },
    { DATANUM(DATA_m518, 0x20), DATANUM(DATA_m518, 0x22) },
};

static const s32 lbl_1_rodata_624[13][2] = {
    { DATANUM(DATA_m518, 0x21), HU_DATANUM_NONE },
    { DATANUM(DATA_m518, 0x21), HU_DATANUM_NONE },
    { DATANUM(DATA_m518, 0x21), HU_DATANUM_NONE },
    { DATANUM(DATA_m518, 0x21), HU_DATANUM_NONE },
    { DATANUM(DATA_m518, 0x21), HU_DATANUM_NONE },
    { DATANUM(DATA_m518, 0x21), HU_DATANUM_NONE },
    { DATANUM(DATA_m518, 0x21), HU_DATANUM_NONE },
    { DATANUM(DATA_m518, 0x21), HU_DATANUM_NONE },
    { DATANUM(DATA_m518, 0x21), HU_DATANUM_NONE },
    { DATANUM(DATA_m518, 0x21), HU_DATANUM_NONE },
    { DATANUM(DATA_m518, 0x21), HU_DATANUM_NONE },
    { DATANUM(DATA_m518, 0x21), HU_DATANUM_NONE },
    { DATANUM(DATA_m518, 0x21), HU_DATANUM_NONE },
};

typedef struct M518PlayerParam_s {
    s16 kind;
    s16 size;
    s16 stepMax;
    s16 stepWait;
} M518_PLAYERPARAM;

static const M518_PLAYERPARAM lbl_1_rodata_68C[2] = {
    { 0, 1, 8, 1 },
    { 1, 2, 40, 20 },
};

M518_PLAYER *fn_1_2D24(s16 playerNo, s16 no, s16 type)
{
    M518_PLAYER *player;
    int i;

    player = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M518_PLAYER), HU_MEMNUM_OVL);
    player->pos.x = 0.0f;
    player->pos.y = 0.0f;
    player->pos.z = 0.0f;
    player->rot.x = 0.0f;
    player->rot.y = 0.0f;
    player->rot.z = 0.0f;
    player->scale.x = 1.0f;
    player->scale.y = 1.0f;
    player->scale.z = 1.0f;
    player->prevPos.x = 0.0f;
    player->prevPos.y = 0.0f;
    player->prevPos.z = 0.0f;
    player->prevRot.x = 0.0f;
    player->prevRot.y = 0.0f;
    player->prevRot.z = 0.0f;
    player->prevScale.x = 1.0f;
    player->prevScale.y = 1.0f;
    player->prevScale.z = 1.0f;
    player->prevCell.x = 0;
    player->prevCell.z = 0;
    player->no = no;
    player->playerNo = playerNo;
    player->padNo = GwPlayerConf[playerNo].padNo;
    player->stkX = 0;
    player->stkY = 0;
    player->btnDown = 0;
    player->type = type;
    player->charNo = GwPlayerConf[playerNo].charNo;
    player->comF = GwPlayerConf[playerNo].type;
    player->dif = GwPlayerConf[playerNo].dif;
    player->kind = lbl_1_rodata_68C[type].kind;
    player->size = lbl_1_rodata_68C[type].size;
    player->stepMax = lbl_1_rodata_68C[type].stepMax;
    player->stepWait = lbl_1_rodata_68C[type].stepWait;
    player->step = 0;
    player->crushF = 0;
    player->unkBC = 0;
    player->unkCA = 0;
    player->comWait = 0;
    player->moveNum = 0;
    player->path = fn_1_DC14(40);
    for (i = 0; i < 4; i++) {
        player->pathTry[i] = fn_1_DC14(40);
    }
    switch (player->type) {
        case 0:
            for (i = 0; i < 2; i++) {
                player->modelId[i] = HU3D_MODELID_NONE;
                if (lbl_1_rodata_5BC[player->charNo][i] != HU_DATANUM_NONE) {
                    player->modelId[i] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_rodata_5BC[player->charNo][i], HU_MEMNUM_OVL, HEAP_MODEL));
                }
            }
            Hu3DModelAttrSet(player->modelId[0], HU3D_MOTATTR_PAUSE);
            Hu3DModelAttrSet(player->modelId[1], HU3D_ATTR_DISPOFF);
            break;
        case 1:
            for (i = 0; i < 2; i++) {
                player->modelId[i] = HU3D_MODELID_NONE;
                if (lbl_1_rodata_624[player->charNo][i] != HU_DATANUM_NONE) {
                    player->modelId[i] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_rodata_624[player->charNo][i], HU_MEMNUM_OVL, HEAP_MODEL));
                }
            }
            Hu3DModelAttrSet(player->modelId[0], HU3D_MOTATTR_PAUSE);
            Hu3DMotionTimeSet(player->modelId[0], player->charNo);
            break;
    }
    Hu3DModelShadowSet(player->modelId[0]);
    player->mode = 0;
    player->prevMode = 0;
    player->process = HuPrcChildCreate(fn_1_31F0, no + 0x1000, 0x3000, 0, HuPrcCurrentGet());
    player->process->property = player;
    return player;
}

void fn_1_31F0(void)
{
    M518_PLAYER *player = HuPrcCurrentGet()->property;

    while (TRUE) {
        lbl_1_data_20[player->mode](player);
    }
}

void fn_1_3230(M518_PLAYER *player, HuVecF pos)
{
    int i;

    for (i = 0; i < 2; i++) {
        if (player->modelId[i] != HU3D_MODELID_NONE) {
            Hu3DModelPosSet(player->modelId[i], pos.x, pos.y, pos.z);
        }
    }
    player->pos = pos;
}

void fn_1_32C8(M518_PLAYER *player, HuVecF rot)
{
    int i;
    float *mtx;
    Mtx rotMtx;

    for (i = 0; i < 2; i++) {
        if (player->modelId[i] != HU3D_MODELID_NONE) {
            mtx = (float *)Hu3DData[player->modelId[i]].mtx;
            MTXIdentity((MtxPtr)mtx);
            MTXIdentity(rotMtx);
            MTXRotRad(rotMtx, 'x', MTXDegToRad(rot.x));
            MTXConcat((MtxPtr)mtx, rotMtx, (MtxPtr)mtx);
            MTXRotRad(rotMtx, 'y', MTXDegToRad(rot.y));
            MTXConcat((MtxPtr)mtx, rotMtx, (MtxPtr)mtx);
            MTXRotRad(rotMtx, 'z', MTXDegToRad(rot.z));
            MTXConcat((MtxPtr)mtx, rotMtx, (MtxPtr)mtx);
            Hu3DModelRotSet(player->modelId[i], 0.0f, 0.0f, 0.0f);
        }
    }
    player->rot = rot;
}

void fn_1_3440(M518_PLAYER *player, HuVecF scale)
{
    int i;

    for (i = 0; i < 2; i++) {
        if (player->modelId[i] != HU3D_MODELID_NONE) {
            Hu3DModelScaleSet(player->modelId[i], scale.x, scale.y, scale.z);
        }
    }
    player->scale = scale;
}

void fn_1_34D8(M518_PLAYER *player, float tpLvl)
{
    int i;

    for (i = 0; i < 2; i++) {
        if (player->modelId[i] != HU3D_MODELID_NONE) {
            Hu3DModelTPLvlSet(player->modelId[i], tpLvl);
        }
    }
    player->tpLvl = tpLvl;
}

void fn_1_3554(M518_PLAYER *player, M518_CELL cell)
{
    HuVecF pos;

    if (player->cell.x != cell.x || player->cell.z != cell.z) {
        player->prevCell = cell;
    }
    player->cell = cell;
    pos.x = 200.0f * (player->cell.x - 6) + 200.0f * player->size / 2;
    pos.z = 200.0f * (player->cell.z - 6) + 200.0f * player->size / 2;
    pos.y = -30.0f + 200.0f * player->size / 2;
    fn_1_3230(player, pos);
}

void fn_1_37D0(M518_PLAYER *player, M518_CELL cell, u16 value)
{
    u16 prev;
    s16 owner;
    M518_BLOCK *block;

    prev = fn_1_A268(lbl_1_bss_4DC, cell);
    if (prev & 0x40) {
        owner = prev & 0xF;
        if (owner != player->no) {
            return;
        }
    }
    fn_1_A200(lbl_1_bss_4DC, cell, value);
    block = lbl_1_bss_150[cell.x + cell.z * M518_GRID_SIZE];
    if ((value & 0x40) && (block->mode == 2 || block->mode == 0)) {
        fn_1_CE54(block, 30.0f, 10, player->stepMax - player->step, player->no);
    }
}

static const u8 lbl_1_rodata_6A8[5][2] = { { 1, 0 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 0, 1 } };
static const u8 lbl_1_rodata_6B2[5][4] = { { 1, 1, 0, 0 }, { 1, 1, 1, 0 }, { 1, 1, 1, 1 }, { 0, 1, 1, 1 }, { 0, 0, 1, 1 } };

void fn_1_3904(M518_PLAYER *player)
{
    int j;
    int i;
    u16 value;
    const u8 *tbl;
    int speed;
    float angle;
    int unused;
    M518_CELL cell;

    angle = HuAbs(player->rot.z - player->prevRot.z) + (HuAbs(player->rot.x - player->prevRot.x) + HuAbs(player->rot.y - player->prevRot.y));
    if (angle == 0.0f) {
        speed = 0;
    }
    else if (angle > 0.0f && angle < 28.0f) {
        speed = 1;
    }
    else if (angle >= 28.0f && angle < 62.0f) {
        speed = 2;
    }
    else if (angle >= 62.0f && angle < 90.0f) {
        speed = 3;
    }
    else {
        speed = 4;
    }
    switch (player->size) {
        case 1:
            tbl = lbl_1_rodata_6A8[speed];
            break;
        case 2:
            tbl = lbl_1_rodata_6B2[speed];
            break;
    }
    value = player->no | 0x40;
    unused = 0;
    switch (player->dir) {
        case 0:
            for (i = 0; i < player->size; i++) {
                for (j = 0; j < player->size * 2; j++) {
                    cell.x = player->size - 1 + (player->cell.x - j);
                    cell.z = player->cell.z + i;
                    if (tbl[j]) {
                        fn_1_37D0(player, cell, value);
                    }
                    else {
                        fn_1_37D0(player, cell, 0);
                    }
                }
            }
            break;
        case 1:
            for (i = 0; i < player->size; i++) {
                for (j = 0; j < player->size * 2; j++) {
                    cell.x = player->cell.x + j;
                    cell.z = player->cell.z + i;
                    if (tbl[j]) {
                        fn_1_37D0(player, cell, value);
                    }
                    else {
                        fn_1_37D0(player, cell, 0);
                    }
                }
            }
            break;
        case 2:
            for (i = 0; i < player->size * 2; i++) {
                for (j = 0; j < player->size; j++) {
                    cell.x = player->cell.x + j;
                    cell.z = player->size - 1 + (player->cell.z - i);
                    if (tbl[i]) {
                        fn_1_37D0(player, cell, value);
                    }
                    else {
                        fn_1_37D0(player, cell, 0);
                    }
                }
            }
            break;
        case 3:
            for (i = 0; i < player->size * 2; i++) {
                for (j = 0; j < player->size; j++) {
                    cell.x = player->cell.x + j;
                    cell.z = player->cell.z + i;
                    if (tbl[i]) {
                        fn_1_37D0(player, cell, value);
                    }
                    else {
                        fn_1_37D0(player, cell, 0);
                    }
                }
            }
            break;
        case 4:
            for (i = 0; i < player->size; i++) {
                for (j = 0; j < player->size; j++) {
                    cell.x = player->cell.x + j;
                    cell.z = player->cell.z + i;
                    fn_1_37D0(player, cell, value);
                }
            }
            break;
    }
}

M518_CELL fn_1_4674(M518_PLAYER *player)
{
    return player->cell;
}

void fn_1_468C(M518_PLAYER *player)
{
    player->mode = 0;
}

void fn_1_4698(M518_PLAYER *player)
{
    while (TRUE) {
        M518_WAIT(1);
    }
}

void fn_1_4728(M518_PLAYER *player)
{
    player->mode = 1;
}

void fn_1_4734(float *a, float *b)
{
    float temp = *a;

    *a = *b;
    *b = temp;
}

void fn_1_4760(HuVecF *rot, HuVecF *axis)
{
    HuVecF r;

    r.x = fmod(rot->x, 360.0);
    r.y = fmod(rot->y, 360.0);
    r.z = fmod(rot->z, 360.0);
    if (r.x == 90.0f || r.x == -270.0f) {
        fn_1_4734(&axis->z, &axis->y);
    }
    if (r.x == -90.0f || r.x == 270.0f) {
        fn_1_4734(&axis->z, &axis->y);
        axis->y *= -1.0f;
    }
    if (r.y == 90.0f || r.y == -270.0f) {
        fn_1_4734(&axis->z, &axis->x);
    }
    if (r.y == -90.0f || r.y == 270.0f) {
        fn_1_4734(&axis->z, &axis->x);
        axis->x *= -1.0f;
    }
    if (r.x == -180.0f || r.x == 180.0f) {
        axis->z *= -1.0f;
    }
    if (r.y == -180.0f || r.y == 180.0f) {
        axis->z *= -1.0f;
    }
    if ((r.x == -270.0f && r.y == -90.0f) || (r.x == 270.0f && r.y == -90.0f) || (r.x == -90.0f && r.y == 270.0f)
        || (r.x == 90.0f && r.y == 270.0f) || (r.x == -90.0f && r.y == -90.0f) || (r.x == 90.0f && r.y == -90.0f)
        || (r.x == -270.0f && r.y == 270.0f) || (r.x == 270.0f && r.y == 270.0f)) {
        axis->z *= -1.0f;
    }
}
static const u16 lbl_1_rodata_768[5][18 * 18] = {
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 3, 0xE, 0xE, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 3, 0xE, 0xE0, 0xE0, 0xE, 3, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x80, 0xE0, 0xE, 3, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0, 0, 0, 0, 0,
        0, 3, 0xE, 0xE0, 0x80, 0x100, 0x500, 0x500, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0, 0, 0, 0,
        3, 0xE, 0xE0, 0x80, 0x100, 0x500, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0x80, 0xE0, 0xE, 3, 0, 0, 0, 0,
        3, 0xE, 0xE0, 0x80, 0x100, 0x500, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0x80, 0xE0, 0xE, 3, 0, 0, 0, 0,
        0, 3, 0xE, 0xE0, 0x80, 0x100, 0x500, 0x500, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0, 0, 0, 0,
        0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x80, 0xE0, 0xE, 3, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 3, 0xE, 0xE0, 0xE0, 0xE, 3, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 3, 0xE, 0xE, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    },
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0xE, 0xE, 3, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 3, 0xE, 0xE0, 0xE0, 0xE, 3, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x80, 0xE0, 0xE, 3, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0,
        0, 0, 0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0x500, 0x500, 0x100, 0x80, 0xE0, 0xE, 3, 0,
        0, 0, 0, 0, 3, 0xE, 0xE0, 0x80, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0x500, 0x100, 0x80, 0xE0, 0xE, 3,
        0, 0, 0, 0, 3, 0xE, 0xE0, 0x80, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0x500, 0x100, 0x80, 0xE0, 0xE, 3,
        0, 0, 0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0x500, 0x500, 0x100, 0x80, 0xE0, 0xE, 3, 0,
        0, 0, 0, 0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x80, 0xE0, 0xE, 3, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 3, 0xE, 0xE0, 0xE0, 0xE, 3, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0xE, 0xE, 3, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    },
    {
        0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 3, 0xE, 0xE, 3, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 3, 0xE, 0xE0, 0xE0, 0xE, 3, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x80, 0xE0, 0xE, 3, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0, 0, 0,
        0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0x500, 0x500, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0, 0,
        0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0x500, 0xFFF, 0xFFF, 0x500, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0,
        0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0x500, 0xFFF, 0xFFF, 0x500, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0,
        0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0xFFF, 0xFFF, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0, 0,
        0, 0, 0, 0, 3, 0xE, 0xE0, 0x80, 0xFFF, 0xFFF, 0x80, 0xE0, 0xE, 3, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x80, 0xE0, 0xE, 3, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 3, 0xE, 0xE0, 0xE0, 0xE, 3, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 3, 0xE, 0xE, 3, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    },
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 3, 0xE, 0xE, 3, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 3, 0xE, 0xE0, 0xE0, 0xE, 3, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x80, 0xE0, 0xE, 3, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0, 0, 0,
        0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0x500, 0x500, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0, 0,
        0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0x500, 0xFFF, 0xFFF, 0x500, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0,
        0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0x500, 0xFFF, 0xFFF, 0x500, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0,
        0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0xFFF, 0xFFF, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0, 0,
        0, 0, 0, 0, 3, 0xE, 0xE0, 0x80, 0xFFF, 0xFFF, 0x80, 0xE0, 0xE, 3, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x80, 0xE0, 0xE, 3, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 3, 0xE, 0xE0, 0xE0, 0xE, 3, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 3, 0xE, 0xE, 3, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0,
    },
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 3, 0xE, 0xE, 3, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 3, 0xE, 0xE0, 0xE0, 0xE, 3, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x80, 0xE0, 0xE, 3, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0, 0, 0,
        0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0x500, 0x500, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0, 0,
        0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0x500, 0xFFF, 0xFFF, 0x500, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0,
        0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0x500, 0xFFF, 0xFFF, 0x500, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0,
        0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0x500, 0x500, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0, 0,
        0, 0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x100, 0x100, 0x80, 0xE0, 0xE, 3, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 3, 0xE, 0xE0, 0x80, 0x80, 0xE0, 0xE, 3, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 3, 0xE, 0xE0, 0xE0, 0xE, 3, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 3, 0xE, 0xE, 3, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    },
};

void fn_1_6C0C(M518_PLAYER *player)
{
    int x;
    int z;
    M518_CELL cell;

    if (player->type == 1) {
        for (z = 0; z < 18; z++) {
            for (x = 0; x < 18; x++) {
                cell.x = player->cell.x - 8 + x;
                cell.z = player->cell.z - 8 + z;
                fn_1_9C4C(lbl_1_bss_4DC, cell, lbl_1_rodata_768[player->dir][x + z * 18]);
            }
        }
    }
}

void fn_1_6CFC(M518_PLAYER *player)
{
    player->btnDown = 0;
    player->stkX = 0;
    player->stkY = 0;
    if (lbl_1_bss_2C) {
        player->btnDown = HuPadBtnDown[player->padNo];
        player->stkX = HuPadStkX[player->padNo];
        player->stkY = HuPadStkY[player->padNo];
    }
}

void fn_1_6D84(M518_PLAYER *player)
{
    player->btnDown = 0;
    player->stkX = 0;
    player->stkY = 0;
    if (lbl_1_bss_2C) {
        switch (player->kind) {
            case 0:
                fn_1_6E08(player);
                break;
            case 1:
                fn_1_7908(player);
                break;
        }
    }
}


void fn_1_83E8(M518_PLAYER *player, s16 time, s16 delay)
{
    player->time = time;
    player->delay = delay;
    player->mode = 2;
}

void fn_1_88B4(M518_PLAYER *player, s16 time, s16 delay)
{
    player->time = time;
    player->delay = delay;
    player->mode = 4;
}

void fn_1_8E20(M518_PLAYER *player)
{
    player->mode = 3;
}

void fn_1_8E2C(M518_PLAYER *player)
{
    omVibrate(player->playerNo, 0x46, 0xC, 0);
    fn_1_E7B4(0x6F7, player->pos);
    Hu3DModelAttrReset(player->modelId[1], 1);
    Hu3DMotionTimeSet(player->modelId[1], 0.0f);
    while (!Hu3DMotionEndCheck(player->modelId[1])) {
        M518_WAIT(1);
    }
    Hu3DModelAttrSet(player->modelId[1], 1);
    Hu3DModelAttrReset(player->modelId[0], HU3D_MOTATTR_PAUSE);
    Hu3DMotionTimeSet(player->modelId[0], 0.0f);
    player->mode = 0;
    while (TRUE) {
        M518_WAIT(1);
    }
}

void fn_1_8FA8(M518_PLAYER *player)
{
    int i;

    for (i = 0; i < 2; i++) {
        if (player->modelId[i] != -1) {
            Hu3DModelAttrReset(player->modelId[i], 1);
        }
    }
}

void fn_1_9014(M518_PLAYER *player)
{
    int i;

    for (i = 0; i < 2; i++) {
        if (player->modelId[i] != -1) {
            Hu3DModelAttrSet(player->modelId[i], 1);
        }
    }
}


M518_PLAYERFUNC lbl_1_data_60[1] = { (M518_PLAYERFUNC)fn_1_9ED8 };

static const s32 lbl_1_rodata_1450[20] = {
    DATANUM(DATA_m518, 0x00), DATANUM(DATA_m518, 0x02), DATANUM(DATA_m518, 0x01), DATANUM(DATA_m518, 0x01), DATANUM(DATA_m518, 0x01),
    DATANUM(DATA_m518, 0x13), DATANUM(DATA_m518, 0x13), DATANUM(DATA_m518, 0x13), DATANUM(DATA_m518, 0x12), DATANUM(DATA_m518, 0x12),
    DATANUM(DATA_m518, 0x12), DATANUM(DATA_m518, 0x12), DATANUM(DATA_m518, 0x12), DATANUM(DATA_m518, 0x11), DATANUM(DATA_m518, 0x11),
    DATANUM(DATA_m518, 0x11), DATANUM(DATA_m518, 0x11), DATANUM(DATA_m518, 0x11), DATANUM(DATA_m518, 0x11), DATANUM(DATA_m518, 0x11),
};

M518_MAP *fn_1_9860(void)
{
    M518_MAP *map;
    int i;

    map = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M518_MAP), HU_MEMNUM_OVL);
    fn_1_9E70(map);
    for (i = 0; i < 20; i++) {
        map->modelId[i] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_rodata_1450[i], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(map->modelId[i], HU3D_ATTR_NOCULL);
    }
    for (i = 5; i <= 19; i++) {
        Hu3DModelAttrSet(map->modelId[i], HU3D_MOTATTR_LOOP);
    }
    Hu3DMotionSpeedSet(map->modelId[5], 0.03f);
    Hu3DMotionSpeedSet(map->modelId[6], 0.035f);
    Hu3DMotionSpeedSet(map->modelId[7], 0.04f);
    Hu3DMotionSpeedSet(map->modelId[8], 0.02f);
    Hu3DMotionSpeedSet(map->modelId[9], 0.024f);
    Hu3DMotionSpeedSet(map->modelId[10], 0.028f);
    Hu3DMotionSpeedSet(map->modelId[11], 0.03f);
    Hu3DMotionSpeedSet(map->modelId[12], 0.032f);
    Hu3DMotionSpeedSet(map->modelId[13], 0.01f);
    Hu3DMotionSpeedSet(map->modelId[14], 0.012f);
    Hu3DMotionSpeedSet(map->modelId[15], 0.014f);
    Hu3DMotionSpeedSet(map->modelId[16], 0.016f);
    Hu3DMotionSpeedSet(map->modelId[17], 0.018f);
    Hu3DMotionSpeedSet(map->modelId[18], 0.02f);
    Hu3DMotionSpeedSet(map->modelId[19], 0.022f);
    Hu3DModelShadowMapSet(map->modelId[0]);
    Hu3DModelShadowSet(map->modelId[1]);
    Hu3DModelShadowSet(map->modelId[2]);
    Hu3DModelShadowSet(map->modelId[3]);
    Hu3DModelShadowSet(map->modelId[4]);
    Hu3DModelAttrSet(map->modelId[1], HU3D_MOTATTR_PAUSE);
    Hu3DModelAttrSet(map->modelId[2], HU3D_MOTATTR_PAUSE);
    Hu3DModelAttrSet(map->modelId[3], HU3D_MOTATTR_PAUSE);
    Hu3DModelAttrSet(map->modelId[4], HU3D_MOTATTR_PAUSE);
    Hu3DModelRotSet(map->modelId[1], 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(map->modelId[2], 0.0f, 90.0f, 0.0f);
    Hu3DModelRotSet(map->modelId[3], 0.0f, 180.0f, 0.0f);
    Hu3DModelRotSet(map->modelId[4], 0.0f, 270.0f, 0.0f);
    map->mode = 0;
    map->prevMode = 0;
    map->process = HuPrcChildCreate(fn_1_9BB0, 0x1000, 0x3000, 0, HuPrcCurrentGet());
    map->process->property = map;
    return map;
}

void fn_1_9BB0(void)
{
    M518_MAP *map = HuPrcCurrentGet()->property;

    while (TRUE) {
        lbl_1_data_60[map->mode]((M518_PLAYER *)map);
    }
}

void fn_1_9BF0(M518_MAP *map)
{
    int x;
    int z;

    for (z = 0; z < M518_GRID_SIZE; z++) {
        for (x = 0; x < M518_GRID_SIZE; x++) {
            map->weight[x + z * M518_GRID_SIZE] = 0;
        }
    }
}

void fn_1_9C4C(M518_MAP *map, M518_CELL cell, u16 value)
{
    if (cell.x >= 0 && cell.x < M518_GRID_SIZE && cell.z >= 0 && cell.z < M518_GRID_SIZE) {
        map->weight[cell.x + cell.z * M518_GRID_SIZE] += value;
    }
}

u16 fn_1_9CBC(M518_MAP *map, M518_CELL cell)
{
    if (cell.x >= 0 && cell.x < M518_GRID_SIZE && cell.z >= 0 && cell.z < M518_GRID_SIZE) {
        return map->weight[cell.x + cell.z * M518_GRID_SIZE];
    }
    return 0xFFFF;
}

void fn_1_9D30(M518_MAP *map)
{
    Hu3DModelAttrReset(map->modelId[1], HU3D_MOTATTR_PAUSE);
    Hu3DModelAttrReset(map->modelId[2], HU3D_MOTATTR_PAUSE);
    Hu3DModelAttrReset(map->modelId[3], HU3D_MOTATTR_PAUSE);
    Hu3DModelAttrReset(map->modelId[4], HU3D_MOTATTR_PAUSE);
}

void fn_1_9D98(M518_MAP *map)
{
    int x;
    int z;

    for (z = 0; z < M518_GRID_SIZE; z++) {
        for (x = 0; x < M518_GRID_SIZE; x++) {
            map->gridSave[x + z * M518_GRID_SIZE] = map->grid[x + z * M518_GRID_SIZE];
        }
    }
}

void fn_1_9E04(M518_MAP *map)
{
    int x;
    int z;

    for (z = 0; z < M518_GRID_SIZE; z++) {
        for (x = 0; x < M518_GRID_SIZE; x++) {
            map->grid[x + z * M518_GRID_SIZE] = map->gridSave[x + z * M518_GRID_SIZE];
        }
    }
}

void fn_1_9E70(M518_MAP *map)
{
    int x;
    int z;

    for (z = 0; z < M518_GRID_SIZE; z++) {
        for (x = 0; x < M518_GRID_SIZE; x++) {
            map->grid[x + z * M518_GRID_SIZE] = 0;
        }
    }
}

void fn_1_9ECC(M518_MAP *map)
{
    map->mode = 0;
}

void fn_1_9ED8(M518_MAP *map)
{
    int i;
    float pos[15];

    pos[0] = 1800.0f;
    pos[1] = 0.0f;
    pos[2] = -1800.0f;
    pos[3] = 3000.0f;
    pos[4] = 1500.0f;
    pos[5] = 0.0f;
    pos[6] = -1500.0f;
    pos[7] = 3000.0f;
    pos[8] = 3000.0f;
    pos[9] = 2000.0f;
    pos[10] = 1000.0f;
    pos[11] = 0.0f;
    pos[12] = -1000.0f;
    pos[13] = -2000.0f;
    pos[14] = -3000.0f;
    while (TRUE) {
        pos[0] += 2.0f;
        pos[1] += 2.0f;
        pos[2] += 2.0f;
        pos[3] += 0.6f;
        pos[4] += 0.6f;
        pos[5] += 0.6f;
        pos[6] += 0.6f;
        pos[7] += 0.6f;
        pos[8] += 0.3f;
        pos[9] += 0.3f;
        pos[10] += 0.3f;
        pos[11] += 0.3f;
        pos[12] += 0.3f;
        pos[13] += 0.3f;
        pos[14] += 0.3f;
        for (i = 0; i < 15; i++) {
            Hu3DModelPosSet(map->modelId[i + 5], pos[i], 0.0f, 0.0f);
        }
        M518_WAIT(1);
    }
}

void fn_1_A200(M518_MAP *map, M518_CELL cell, u16 value)
{
    if (cell.x >= 0 && cell.x < M518_GRID_SIZE && cell.z >= 0 && cell.z < M518_GRID_SIZE) {
        map->grid[cell.x + cell.z * M518_GRID_SIZE] = value;
    }
}

u16 fn_1_A268(M518_MAP *map, M518_CELL cell)
{
    if (cell.x >= 0 && cell.x < M518_GRID_SIZE && cell.z >= 0 && cell.z < M518_GRID_SIZE) {
        return map->grid[cell.x + cell.z * M518_GRID_SIZE];
    }
    return 0x80;
}

void fn_1_A2D8(M518_MAP *map, M518_CELL cell, s16 maxDist)
{
    int x;
    int z;
    u16 value;
    M518_CELL pos;

    for (z = 0; z < M518_GRID_SIZE; z++) {
        for (x = 0; x < M518_GRID_SIZE; x++) {
            pos.x = x;
            pos.z = z;
            value = fn_1_A268(map, pos);
            value &= 0xFF;
            value &= ~0x20;
            fn_1_A200(map, pos, value);
        }
    }
    map->unk3B0 = 0;
    map->maxDist = maxDist;
    fn_1_A480(map, cell, 0);
}

typedef struct M518Dir_s {
    s16 x;
    s16 z;
} M518_DIR;

static const M518_DIR lbl_1_rodata_14F8[4] = { { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };

// NONMATCHING: register allocation of the spread flags and of the inlined A268 results
inline void fn_1_A480(M518_MAP *map, M518_CELL cell, s16 dist)
{
    int i;
    u16 value;
    M518_CELL next;
    BOOL spread;
    BOOL free;
    BOOL open;
    BOOL farther;
    BOOL ok;

    value = fn_1_A268(map, cell);
    value |= 0x20;
    value = (value & 0xFF) | (dist << 8);
    fn_1_A200(map, cell, value);
    if (dist < map->maxDist) {
        for (i = 0; i < 4; i++) {
            next.x = cell.x + lbl_1_rodata_14F8[i].x;
            next.z = cell.z + lbl_1_rodata_14F8[i].z;
            value = fn_1_A268(map, next);
            spread = TRUE;
            free = FALSE;
            open = FALSE;
            if (!(value & 0x80) && !(value & 0x20)) {
                open = TRUE;
            }
            if (open && ((value & 0xF) > 0) == FALSE) {
                free = TRUE;
            }
            if (!free) {
                farther = FALSE;
                if ((value & 0x20) && (value >> 8) > dist) {
                    farther = TRUE;
                }
                if (!farther) {
                    spread = FALSE;
                }
            }
            if (spread) {
                ok = TRUE;
            }
            else {
                ok = FALSE;
            }
            if (ok) {
                fn_1_A480(map, next, dist + 1);
            }
        }
    }
}

typedef void (*M518_CHARFUNC)(M518_CHAR *chr);

M518_CHARFUNC lbl_1_data_64[4] = { fn_1_B60C, fn_1_B6B8, fn_1_B7AC, fn_1_ACAC };

#define M518_CHARMOT(charName)                                                                                                   \
    {                                                                                                                            \
        CHARMOT_DATANUM(CHARMOT_HSF_c000m1_300, charName), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_303, charName),                    \
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_304, charName), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_358, charName),                \
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_302, charName), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_320, charName),                \
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_319, charName),                                                                   \
    }

static const s32 lbl_1_rodata_1508[13][7] = {
    M518_CHARMOT(mario),
    M518_CHARMOT(luigi),
    M518_CHARMOT(peach),
    M518_CHARMOT(yoshi),
    M518_CHARMOT(wario),
    M518_CHARMOT(daisy),
    M518_CHARMOT(waluigi),
    M518_CHARMOT(kinopio),
    M518_CHARMOT(teresa),
    M518_CHARMOT(minikoopa),
    M518_CHARMOT(minikoopa),
    M518_CHARMOT(minikoopa),
    M518_CHARMOT(minikoopa),
};

M518_CHAR *fn_1_A9A4(s16 playerNo)
{
    M518_CHAR *chr;
    int i;

    chr = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M518_CHAR), HU_MEMNUM_OVL);
    chr->playerNo = playerNo;
    chr->pos.x = 0.0f;
    chr->pos.y = 0.0f;
    chr->pos.z = 0.0f;
    chr->comF = GwPlayerConf[playerNo].type;
    chr->dif = GwPlayerConf[playerNo].dif;
    chr->charNo = GwPlayerConf[playerNo].charNo;
    chr->modelId = CharModelCreate(chr->charNo, CHAR_MODEL1);
    for (i = 0; i < 7; i++) {
        chr->motId[i] = CharMotionCreate(chr->charNo, lbl_1_rodata_1508[chr->charNo][i]);
    }
    CharMotionDataClose(chr->charNo);
    Hu3DModelShadowSet(chr->modelId);
    chr->mode = 0;
    chr->prevMode = 0;
    Hu3DMotionSet(chr->modelId, chr->motId[0]);
    Hu3DModelAttrSet(chr->modelId, HU3D_MOTATTR_LOOP);
    chr->process = HuPrcChildCreate(fn_1_AB3C, 0x1100, 0x1800, 0, HuPrcCurrentGet());
    chr->process->property = chr;
    return chr;
}

void fn_1_AB3C(void)
{
    M518_CHAR *chr = HuPrcCurrentGet()->property;

    while (TRUE) {
        lbl_1_data_64[chr->mode](chr);
    }
}

void fn_1_AB7C(M518_CHAR *chr, HuVecF target, s16 time, s16 delay)
{
    chr->target = target;
    chr->time = time;
    chr->delay = delay;
    chr->mode = 3;
}

float fn_1_ABA8(float start, float end, float t)
{
    float diff;
    float angle;

    diff = fmod(end - start, 360.0);
    if (diff < 0.0f) {
        diff += 360.0f;
    }
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    angle = fmod(start + diff * t, 360.0);
    if (angle < 0.0f) {
        angle += 360.0f;
    }
    return angle;
}

static inline float fn_1_EaseOut(float t)
{
    return HuSin(90.0f * t);
}

// NONMATCHING: float register allocation of the inlined angle lerp temporaries
void fn_1_ACAC(M518_CHAR *chr)
{
    int i;
    float t;
    HuVecF start;
    HuVecF end;
    HuVecF pos;
    HuVecF rot;
    HuVecF dir;

    M518_WAIT(chr->delay);
    Hu3DMotionSet(chr->modelId, chr->motId[4]);
    Hu3DModelAttrSet(chr->modelId, HU3D_MOTATTR_LOOP);
    CharModelStepFxSet(chr->charNo, 2);
    start = chr->pos;
    end = chr->target;
    for (i = 0; i < chr->time - 60; i++) {
        t = (1.0f / chr->time) * (i + 1);
        pos.x = start.x + t * (end.x - start.x);
        pos.z = start.z + t * (end.z - start.z);
        pos.y = start.y + t * (end.y - start.y);
        dir.x = pos.x - chr->pos.x;
        dir.z = pos.z - chr->pos.z;
        rot.y = fn_1_ABA8(chr->rot.y, HuAtan(dir.x, dir.z), 0.4f);
        rot.x = 0.0f;
        rot.z = 0.0f;
        fn_1_BB00(chr, pos);
        fn_1_BB60(chr, rot);
        CharModelVoicePanSet(chr->charNo, fn_1_E6B8(chr->pos));
        CharModelVoiceVolSet(chr->charNo, fn_1_E5BC(chr->pos));
        M518_WAIT(1);
    }
    Hu3DMotionShiftSet(chr->modelId, chr->motId[6], 0.0f, 8.0f, HU3D_MOTATTR_NONE);
    start = chr->pos;
    end = chr->pos;
    end.x -= 400.0f;
    for (i = 0; i < 60; i++) {
        t = fn_1_EaseOut((1.0f / 60) * (i + 1));
        pos.x = start.x + t * (end.x - start.x);
        pos.z = start.z + t * (end.z - start.z);
        pos.y = start.y + t * (end.y - start.y);
        dir.x = pos.x - chr->pos.x;
        dir.z = pos.z - chr->pos.z;
        rot.y = fn_1_ABA8(chr->rot.y, HuAtan(dir.x, dir.z), 0.4f);
        rot.x = 0.0f;
        rot.z = 0.0f;
        fn_1_BB00(chr, pos);
        fn_1_BB60(chr, rot);
        if (i == 24) {
            CharEffectDustCreate(HU3D_CAM0, 4.0f, &pos);
        }
        CharModelVoicePanSet(chr->charNo, fn_1_E6B8(chr->pos));
        CharModelVoiceVolSet(chr->charNo, fn_1_E5BC(chr->pos));
        M518_WAIT(1);
    }
    M518_WAIT(15);
    Hu3DMotionShiftSet(chr->modelId, chr->motId[5], 0.0f, 8.0f, HU3D_MOTATTR_NONE);
    Hu3DMotionShiftSpeedSet(chr->modelId, 0.3f);
    while (TRUE) {
        M518_WAIT(1);
    }
}

void fn_1_B600(M518_CHAR *chr)
{
    chr->mode = 0;
}

void fn_1_B60C(M518_CHAR *chr)
{
    Hu3DMotionSet(chr->modelId, chr->motId[0]);
    Hu3DModelAttrSet(chr->modelId, HU3D_MOTATTR_LOOP);
    while (TRUE) {
        M518_WAIT(1);
    }
}

void fn_1_B6AC(M518_CHAR *chr)
{
    chr->mode = 1;
}

void fn_1_B6B8(M518_CHAR *chr)
{
    CharModelVoicePanSet(chr->charNo, fn_1_E6B8(chr->pos));
    CharModelVoiceVolSet(chr->charNo, 127);
    Hu3DMotionShiftSet(chr->modelId, chr->motId[3], 0.0f, 8.0f, HU3D_MOTATTR_NONE);
    while (TRUE) {
        M518_WAIT(1);
    }
}

void fn_1_B7A0(M518_CHAR *chr)
{
    chr->mode = 2;
}

static const float lbl_1_rodata_1688[13] = {
    225.0f, 225.0f, 225.0f, 225.0f, 225.0f, 225.0f, 225.0f, 225.0f, 225.0f, 250.0f, 250.0f, 250.0f, 250.0f,
};

void fn_1_B7AC(M518_CHAR *chr)
{
    int i;
    float t;
    float s;
    HuVecF start;
    HuVecF end;
    HuVecF pos;

    Hu3DMotionShiftSet(chr->modelId, chr->motId[1], 0.0f, 8.0f, HU3D_MOTATTR_NONE);
    Hu3DModelAttrReset(chr->modelId, HU3D_MOTATTR_LOOP);
    start = chr->pos;
    start.y = 0.0f;
    end = chr->pos;
    end.x += lbl_1_rodata_1688[chr->charNo];
    end.y += 200.0f;
    for (i = 0; i < 40; i++) {
        t = (1.0f / 40) * (i + 1);
        s = HuSin(180.0f * t);
        pos.x = start.x + t * (end.x - start.x);
        pos.y = start.y + s * (end.y - start.y);
        pos.z = end.z;
        if (i < 20 && pos.y < chr->pos.y) {
            pos.y = chr->pos.y;
        }
        fn_1_BB00(chr, pos);
        CharModelVoicePanSet(chr->charNo, fn_1_E6B8(chr->pos));
        CharModelVoiceVolSet(chr->charNo, fn_1_E5BC(chr->pos));
        M518_WAIT(1);
        if (i == 36) {
            Hu3DMotionShiftSet(chr->modelId, chr->motId[2], 0.0f, 8.0f, HU3D_MOTATTR_NONE);
            Hu3DModelAttrReset(chr->modelId, HU3D_MOTATTR_LOOP);
        }
    }
    while (TRUE) {
        M518_WAIT(1);
    }
}

void fn_1_BB00(M518_CHAR *chr, HuVecF pos)
{
    Hu3DModelPosSet(chr->modelId, pos.x, pos.y, pos.z);
    chr->pos = pos;
}

void fn_1_BB60(M518_CHAR *chr, HuVecF rot)
{
    Hu3DModelRotSet(chr->modelId, rot.x, rot.y, rot.z);
    chr->rot = rot;
}

void fn_1_BBC0(M518_CHAR *chr)
{
    Hu3DModelAttrReset(chr->modelId, HU3D_ATTR_DISPOFF);
}

void fn_1_BBF4(M518_CHAR *chr)
{
    Hu3DModelAttrSet(chr->modelId, HU3D_ATTR_DISPOFF);
}

typedef void (*M518_PODIUMFUNC)(M518_PODIUM *podium);

M518_PODIUMFUNC lbl_1_data_74[1] = { fn_1_BE6C };

static const s32 lbl_1_rodata_16C0[3] = { DATANUM(DATA_m518, 0x0F), DATANUM(DATA_m518, 0x03), DATANUM(DATA_m518, 0x10) };

M518_PODIUM *fn_1_BC28(void)
{
    M518_PODIUM *podium;
    int i;

    podium = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M518_PODIUM), HU_MEMNUM_OVL);
    podium->pos.x = 0.0f;
    podium->pos.y = 0.0f;
    podium->pos.z = 0.0f;
    for (i = 0; i < 3; i++) {
        podium->modelId[i] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_rodata_16C0[i], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(podium->modelId[i], HU3D_ATTR_NOCULL);
        Hu3DModelAttrSet(podium->modelId[i], HU3D_MOTATTR_LOOP);
    }
#ifdef TARGET_PC
    Hu3DMotionTimeSet(podium->modelId[2], (s32)frandmod((u32)Hu3DMotionMaxTimeGet(podium->modelId[2])));
#else
    Hu3DMotionTimeSet(podium->modelId[2], (s32)frandmod(Hu3DMotionMaxTimeGet(podium->modelId[2])));
#endif
    Hu3DModelShadowSet(podium->modelId[0]);
    podium->mode = 0;
    podium->prevMode = 0;
    podium->process = HuPrcChildCreate(fn_1_BD88, 0x1300, 0x1000, 0, HuPrcCurrentGet());
    podium->process->property = podium;
    return podium;
}

void fn_1_BD88(void)
{
    M518_PODIUM *podium = HuPrcCurrentGet()->property;

    while (TRUE) {
        lbl_1_data_74[podium->mode](podium);
    }
}

void fn_1_BDC8(M518_PODIUM *podium, HuVecF pos)
{
    Hu3DModelPosSet(podium->modelId[0], pos.x, pos.y, pos.z);
    Hu3DModelPosSet(podium->modelId[2], pos.x, pos.y, pos.z);
    Hu3DModelPosSet(podium->modelId[1], pos.x, 201.0f + pos.y, pos.z);
    podium->pos = pos;
}

void fn_1_BE60(M518_PODIUM *podium)
{
    podium->mode = 0;
}

void fn_1_BE6C(M518_PODIUM *podium)
{
    while (TRUE) {
        M518_WAIT(1);
    }
}

void fn_1_BEFC(M518_PODIUM *podium)
{
    int i;

    for (i = 0; i < 3; i++) {
        Hu3DModelAttrReset(podium->modelId[i], HU3D_ATTR_DISPOFF);
    }
}

void fn_1_BF54(M518_PODIUM *podium)
{
    int i;

    for (i = 0; i < 3; i++) {
        Hu3DModelAttrSet(podium->modelId[i], HU3D_ATTR_DISPOFF);
    }
}

typedef void (*M518_CROWDFUNC)(M518_CROWD *crowd);

M518_CROWDFUNC lbl_1_data_78[2] = { fn_1_C3C4, fn_1_C480 };

static const s32 lbl_1_rodata_16D0[1] = { DATANUM(DATA_m518, 0x23) };
static const s32 lbl_1_rodata_16D4[2] = { DATANUM(DATA_m518, 0x24), DATANUM(DATA_m518, 0x25) };

M518_CROWD *fn_1_BFAC(s16 no, s16 type)
{
    M518_CROWD *crowd;
    int i;

    crowd = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M518_CROWD), HU_MEMNUM_OVL);
    crowd->type = type;
    crowd->no = no;
    crowd->pos.x = 0.0f;
    crowd->pos.y = 0.0f;
    crowd->pos.z = 0.0f;
    crowd->rot.x = 0.0f;
    crowd->rot.y = 0.0f;
    crowd->rot.z = 0.0f;
    crowd->scale.x = 1.0f;
    crowd->scale.y = 1.0f;
    crowd->scale.z = 1.0f;
    if (no == 0) {
        for (i = 0; i < 1; i++) {
            (&crowd->modelId)[i] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_rodata_16D0[i], HU_MEMNUM_OVL, HEAP_MODEL));
            Hu3DModelAttrSet((&crowd->modelId)[i], HU3D_ATTR_NOCULL);
        }
    }
    else {
        for (i = 0; i < 1; i++) {
            (&crowd->modelId)[i] = Hu3DModelLink((&lbl_1_bss_3AC[0]->modelId)[i]);
            Hu3DModelAttrSet((&crowd->modelId)[i], HU3D_ATTR_NOCULL);
        }
    }
    crowd->motId = Hu3DJointMotion(crowd->modelId, HuDataSelHeapReadNum(lbl_1_rodata_16D4[type], HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DMotionSet(crowd->modelId, crowd->motId);
    Hu3DModelAttrSet(crowd->modelId, HU3D_MOTATTR_LOOP);
#ifdef TARGET_PC
    Hu3DMotionTimeSet(crowd->modelId, (s32)frandmod((u32)Hu3DMotionMaxTimeGet(crowd->modelId)));
#else
    Hu3DMotionTimeSet(crowd->modelId, (s32)frandmod(Hu3DMotionMaxTimeGet(crowd->modelId)));
#endif
    Hu3DModelShadowSet(crowd->modelId);
    crowd->mode = 0;
    crowd->prevMode = 0;
    crowd->process = HuPrcChildCreate(fn_1_C224, 0x1500, 0x1000, 0, HuPrcCurrentGet());
    crowd->process->property = crowd;
    return crowd;
}

void fn_1_C224(void)
{
    M518_CROWD *crowd = HuPrcCurrentGet()->property;

    while (TRUE) {
        lbl_1_data_78[crowd->mode](crowd);
    }
}

void fn_1_C264(M518_CROWD *crowd)
{
    Hu3DModelAttrSet(crowd->modelId, HU3D_ATTR_DISPOFF);
}

void fn_1_C298(M518_CROWD *crowd, HuVecF pos)
{
    Hu3DModelPosSet(crowd->modelId, pos.x, pos.y, pos.z);
    crowd->pos = pos;
}

void fn_1_C2F8(M518_CROWD *crowd, HuVecF rot)
{
    Hu3DModelRotSet(crowd->modelId, rot.x, rot.y, rot.z);
    crowd->rot = rot;
}

void fn_1_C358(M518_CROWD *crowd, HuVecF scale)
{
    Hu3DModelScaleSet(crowd->modelId, scale.x, scale.y, scale.z);
    crowd->scale = scale;
}

void fn_1_C3B8(M518_CROWD *crowd)
{
    crowd->mode = 0;
}

void fn_1_C3C4(M518_CROWD *crowd)
{
    while (TRUE) {
        M518_WAIT(1);
    }
}

void fn_1_C454(M518_CROWD *crowd, HuVecF target, s16 time, s16 delay)
{
    crowd->target = target;
    crowd->time = time;
    crowd->delay = delay;
    crowd->mode = 1;
}

void fn_1_C480(M518_CROWD *crowd)
{
    int seNo;
    int i;
    float t;
    HuVecF start;
    HuVecF end;
    HuVecF pos;
    HuVecF rot;
    HuVecF dir;

    seNo = -1;
    M518_WAIT(crowd->delay);
    if (crowd->no == 0) {
        seNo = fn_1_E7B4(0x707, crowd->pos);
    }
    start = crowd->pos;
    end = crowd->target;
    for (i = 0; i < crowd->time; i++) {
        t = (1.0f / crowd->time) * (i + 1);
        pos.x = start.x + t * (end.x - start.x);
        pos.z = start.z + t * (end.z - start.z);
        pos.y = start.y + t * (end.y - start.y);
        dir.x = pos.x - crowd->pos.x;
        dir.z = pos.z - crowd->pos.z;
        rot.y = fn_1_ABA8(crowd->rot.y, HuAtan(dir.x, dir.z), 0.4f);
        rot.x = 0.0f;
        rot.z = 0.0f;
        fn_1_C298(crowd, pos);
        fn_1_C2F8(crowd, rot);
        M518_WAIT(1);
        if (seNo != -1) {
            HuAudFXPanning(seNo, fn_1_E6B8(crowd->pos));
        }
        if (i % 10 == 0) {
            switch (frandmod(2)) {
                case 0:
                    pos.x += 100.0f;
                    CharEffectSmokeCreate(HU3D_CAM0, &pos);
                    break;
                case 1:
                    pos.y -= 100.0f;
                    fn_1_E508(pos, 200.0f);
                    break;
            }
        }
    }
    crowd->mode = 0;
    while (TRUE) {
        M518_WAIT(1);
    }
}

typedef void (*M518_BLOCKFUNC)(M518_BLOCK *block);

M518_BLOCKFUNC lbl_1_data_80[5] = { fn_1_CBE8, fn_1_CEA0, fn_1_D218, fn_1_D58C, fn_1_CCD4 };

static const s32 lbl_1_rodata_16DC[10] = {
    DATANUM(DATA_m518, 0x05), DATANUM(DATA_m518, 0x06), DATANUM(DATA_m518, 0x07), DATANUM(DATA_m518, 0x08), DATANUM(DATA_m518, 0x09),
    DATANUM(DATA_m518, 0x0A), DATANUM(DATA_m518, 0x0B), DATANUM(DATA_m518, 0x0C), DATANUM(DATA_m518, 0x0D), DATANUM(DATA_m518, 0x0E),
};

M518_BLOCK *fn_1_C9F4(s16 no, s16 type)
{
    M518_BLOCK *block;
    int i;

    block = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M518_BLOCK), HU_MEMNUM_OVL);
    block->pos.x = 0.0f;
    block->pos.y = 0.0f;
    block->pos.z = 0.0f;
    block->no = no;
    block->type = type;
    if (!lbl_1_bss_0) {
        for (i = 0; i < 10; i++) {
            lbl_1_bss_4[i] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_rodata_16DC[i], HU_MEMNUM_OVL, HEAP_MODEL));
            Hu3DModelAttrSet(lbl_1_bss_4[i], HU3D_ATTR_DISPOFF);
        }
        lbl_1_bss_0 = TRUE;
    }
    block->modelId = Hu3DModelLink(lbl_1_bss_4[type]);
    Hu3DModelAttrReset(block->modelId, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(block->modelId, HU3D_ATTR_NOCULL);
    Hu3DModelAttrSet(block->modelId, HU3D_MOTATTR_PAUSE);
    Hu3DModelShadowMapSet(block->modelId);
    block->mode = 0;
    block->prevMode = 0;
    block->process = HuPrcChildCreate(fn_1_CB9C, 0x2000, 0x1400, 0, HuPrcCurrentGet());
    block->process->property = block;
    return block;
}

void fn_1_CB9C(void)
{
    M518_BLOCK *block = HuPrcCurrentGet()->property;

    while (TRUE) {
        lbl_1_data_80[block->mode](block);
    }
}

void fn_1_CBDC(M518_BLOCK *block)
{
    block->mode = 0;
}

void fn_1_CBE8(M518_BLOCK *block)
{
    Hu3DModelShadowMapSet(block->modelId);
    Hu3DModelShadowReset(block->modelId);
    Hu3DMotionStartEndSet(block->modelId, 0.0f, 1.0f);
    Hu3DModelAttrSet(block->modelId, HU3D_MOTATTR_PAUSE);
    Hu3DMotionTimeSet(block->modelId, 0.0f);
    while (TRUE) {
        M518_WAIT(1);
    }
}

void fn_1_CCC0(M518_BLOCK *block, s16 time, s16 delay)
{
    block->time = time;
    block->delay = delay;
    block->mode = 4;
}

void fn_1_CCD4(M518_BLOCK *block)
{
    Hu3DModelShadowMapSet(block->modelId);
    Hu3DModelShadowReset(block->modelId);
    M518_WAIT(block->delay);
    Hu3DMotionStartEndSet(block->modelId, 0.0f, 10.0f);
    Hu3DModelAttrReset(block->modelId, HU3D_MOTATTR_PAUSE);
    Hu3DMotionTimeSet(block->modelId, 0.0f);
    M518_WAIT(block->time);
    block->mode = 0;
    while (TRUE) {
        M518_WAIT(1);
    }
}

void fn_1_CE54(M518_BLOCK *block, float depth, s16 time, s16 delay, s16 playerNo)
{
    block->time = time;
    block->delay = delay;
    block->depth = depth;
    block->playerNo = playerNo;
    block->mode = 1;
    block->prevMode = 2;
}

void fn_1_CE78(M518_BLOCK *block, float depth, s16 time, s16 delay)
{
    block->time = time;
    block->delay = delay;
    block->depth = depth;
    block->playerNo = -1;
    block->mode = 1;
    block->prevMode = 2;
}

void fn_1_CEA0(M518_BLOCK *block)
{
    int i;
    HuVecF start;
    HuVecF end;
    HuVecF pos;

    Hu3DModelShadowMapSet(block->modelId);
    Hu3DModelShadowReset(block->modelId);
    M518_WAIT(block->delay);
    start = block->pos;
    end = block->pos;
    end.y = -100.0f - block->depth;
    pos = block->pos;
    Hu3DMotionStartEndSet(block->modelId, 0.0f, 5.0f);
    Hu3DModelAttrReset(block->modelId, HU3D_MOTATTR_PAUSE);
    Hu3DMotionTimeSet(block->modelId, 0.0f);
    Hu3DMotionSpeedSet(block->modelId, 0.5f);
    for (i = 0; i < block->time; i++) {
        pos.y = start.y + (end.y - start.y) * ((1.0f / block->time) * (i + 1));
        fn_1_DA7C(block, pos);
        M518_WAIT(1);
    }
    while (TRUE) {
        if (!(fn_1_A268(lbl_1_bss_4DC, block->cell) & 0x40)) {
            fn_1_D204(block, 20, 0);
        }
        M518_WAIT(1);
    }
}

void fn_1_D204(M518_BLOCK *block, s16 time, s16 delay)
{
    block->time = time;
    block->delay = delay;
    block->mode = 2;
}

void fn_1_D218(M518_BLOCK *block)
{
    int i;
    HuVecF start;
    HuVecF end;
    HuVecF pos;

    Hu3DModelShadowMapSet(block->modelId);
    Hu3DModelShadowReset(block->modelId);
    M518_WAIT(block->delay);
    start = block->pos;
    end = block->pos;
    end.y = -100.0f;
    pos = block->pos;
    Hu3DMotionStartEndSet(block->modelId, 5.0f, 10.0f);
    Hu3DModelAttrReset(block->modelId, HU3D_MOTATTR_PAUSE);
    Hu3DMotionTimeSet(block->modelId, 0.0f);
    Hu3DMotionSpeedSet(block->modelId, 0.5f);
    switch (block->playerNo) {
        case 1:
            fn_1_E7B4(0x701, block->pos);
            break;
        case 2:
            fn_1_E7B4(0x700, block->pos);
            break;
        case 3:
            fn_1_E7B4(0x702, block->pos);
            break;
    }
    for (i = 0; i < block->time; i++) {
        pos.y = start.y + (end.y - start.y) * ((1.0f / block->time) * (i + 1));
        fn_1_DA7C(block, pos);
        M518_WAIT(1);
    }
    block->mode = 0;
    while (TRUE) {
        M518_WAIT(1);
    }
}

void fn_1_D578(M518_BLOCK *block, s16 time, s16 delay)
{
    block->time = time;
    block->delay = delay;
    block->mode = 3;
}

void fn_1_D58C(M518_BLOCK *block)
{
    int i;
    HuVecF start;
    HuVecF end;
    HuVecF pos;

    Hu3DModelShadowSet(block->modelId);
    Hu3DModelShadowMapObjReset(block->modelId);
    M518_WAIT(block->delay);
    if (block->delay > 5) {
        fn_1_E7B4(0x703, block->pos);
    }
    start = block->pos;
    end = block->pos;
    end.y = -100.0f;
    pos = block->pos;
    Hu3DMotionStartEndSet(block->modelId, 0.0f, 5.0f);
    Hu3DModelAttrReset(block->modelId, HU3D_MOTATTR_PAUSE);
    Hu3DMotionTimeSet(block->modelId, 0.0f);
    Hu3DMotionSpeedSet(block->modelId, 0.5f);
    for (i = 0; i < block->time; i++) {
        pos.y = start.y + (end.y - start.y) * (1.0 - HuCos(90.0f * ((1.0f / block->time) * (i + 1))));
        fn_1_DA7C(block, pos);
        M518_WAIT(1);
    }
    pos.x = block->pos.x;
    pos.z = block->pos.z;
    pos.y = 4.0f;
    CharEffectDustCreate(HU3D_CAM0, 8.0f + (s32)frandmod(4), &pos);
    HuAudFXPlay(0x704);
    fn_1_CE78(block, 60.0f, 10, 0);
    while (TRUE) {
        M518_WAIT(1);
    }
}

void fn_1_D958(M518_BLOCK *block, M518_CELL cell)
{
    HuVecF pos;

    pos.x = -1100.0f + 200.0f * cell.x;
    pos.y = -100.0f;
    pos.z = -1100.0f + 200.0f * cell.z;
    fn_1_DA7C(block, pos);
    block->cell = cell;
}

void fn_1_DA7C(M518_BLOCK *block, HuVecF pos)
{
    Hu3DModelPosSet(block->modelId, pos.x, pos.y, pos.z);
    block->pos = pos;
}

void fn_1_DADC(M518_BLOCK *block, HuVecF rot)
{
    float *mtx;
    Mtx rotMtx;

    mtx = (float *)Hu3DData[block->modelId].mtx;
    MTXIdentity((MtxPtr)mtx);
    MTXIdentity(rotMtx);
    MTXRotRad(rotMtx, 'x', MTXDegToRad(rot.x));
    MTXConcat((MtxPtr)mtx, rotMtx, (MtxPtr)mtx);
    MTXRotRad(rotMtx, 'y', MTXDegToRad(rot.y));
    MTXConcat((MtxPtr)mtx, rotMtx, (MtxPtr)mtx);
    MTXRotRad(rotMtx, 'z', MTXDegToRad(rot.z));
    MTXConcat((MtxPtr)mtx, rotMtx, (MtxPtr)mtx);
    Hu3DModelRotSet(block->modelId, 0.0f, 0.0f, 0.0f);
    block->rot = rot;
}

M518_QUEUE *fn_1_DC14(int size)
{
    M518_QUEUE *queue;

    queue = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M518_QUEUE), HU_MEMNUM_OVL);
    queue->data = HuMemDirectMallocNum(HEAP_HEAP, size * sizeof(s32), HU_MEMNUM_OVL);
    queue->size = size;
    queue->tail = 0;
    queue->head = 0;
    return queue;
}

BOOL fn_1_DC84(M518_QUEUE *queue, s32 value)
{
    if (queue->tail == queue->size) {
        return FALSE;
    }
    queue->data[queue->tail++] = value;
    return TRUE;
}

BOOL fn_1_DCBC(M518_QUEUE *queue, s32 *value)
{
    if (queue->tail == queue->head) {
        return FALSE;
    }
    *value = queue->data[queue->head++];
    return TRUE;
}

void fn_1_DCF8(M518_QUEUE *queue)
{
    queue->tail = 0;
    queue->head = 0;
}

BOOL fn_1_DD0C(M518_QUEUE *queue)
{
    BOOL result;

    if (queue->tail == queue->head) {
        result = TRUE;
    }
    else {
        result = FALSE;
    }
    return result;
}

M518_EFFECT *fn_1_DD40(void)
{
    M518_EFFECT *effect;
    HU3DMODEL *model;
    HU3DPARTICLE *particle;

    effect = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M518_EFFECT), HU_MEMNUM_OVL);
    effect->activeF = FALSE;
    effect->anim = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_effect, 2), HU_MEMNUM_OVL, HEAP_MODEL));
    effect->modelId = Hu3DParticleCreate(effect->anim, 32);
    Hu3DParticleBlendModeSet(effect->modelId, 0);
    Hu3DModelScaleSet(effect->modelId, 1.0f, 1.0f, 1.0f);
    model = &Hu3DData[effect->modelId];
    particle = model->hookData;
    particle->work = effect;
    Hu3DParticleHookSet(effect->modelId, fn_1_DE2C);
    fn_1_DE8C(effect);
    return effect;
}

typedef void (*M518_EFFECTFUNC)(M518_EFFECT *effect, HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);

M518_EFFECTFUNC lbl_1_data_94[2] = { fn_1_DEF0, fn_1_E2A0 };

void fn_1_DE2C(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    lbl_1_data_94[particleP->dataCnt](particleP->work, modelP, particleP, mtx);
}

void fn_1_DE8C(M518_EFFECT *effect)
{
    HU3DPARTICLE *particle;
    HU3DMODEL *model;

    Hu3DModelAttrSet(effect->modelId, HU3D_ATTR_DISPOFF);
    model = &Hu3DData[effect->modelId];
    particle = model->hookData;
    particle->dataCnt = 0;
}

void fn_1_DEF0(M518_EFFECT *effect, HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
}

void fn_1_DEF4(M518_EFFECT *effect)
{
    HU3DMODEL *model;
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *data;
    int i;
    float angle;
    float radius;

    Hu3DModelAttrReset(effect->modelId, HU3D_ATTR_DISPOFF);
    Hu3DModelPosSet(effect->modelId, effect->pos.x, effect->pos.y, effect->pos.z);
    Hu3DModelLayerSet(effect->modelId, 3);
    Hu3DModelScaleSet(effect->modelId, 1.0f, 1.0f, 1.0f);
    Hu3DParticleColSet(effect->modelId, 255, 255, 255);
    Hu3DParticleScaleSet(effect->modelId, 0.0f);
    model = &Hu3DData[effect->modelId];
    particle = model->hookData;
    data = particle->data;
    particle->emitCnt = 0;
    for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
        data->accel.x = 0.0f;
        data->scale = 0.0f;
        angle = (360.0f / particle->maxCnt) * i;
        radius = effect->radius + (s32)frandmod((s32)(effect->radius / 4));
        data->pos.x = radius * HuSin(angle);
        data->pos.z = radius * HuCos(angle);
        data->pos.y = (s32)frandmod(25);
        data->vel.x = 5.0 * HuSin(angle);
        data->vel.y = 9.0f + (s32)frandmod(2);
        data->vel.z = 5.0 * HuCos(angle);
        data->accel.x = 30.0f;
        data->accel.y = 15.0f;
        data->accel.z = 17.0f;
        data->scale = 80.0f + (s32)frandmod(20);
        data->color.a = 200;
    }
    particle->dataCnt = 1;
}

void fn_1_E2A0(M518_EFFECT *effect, HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *data;
    int alpha;
    int i;

    for (i = 0, data = particleP->data; i < particleP->maxCnt; i++, data++) {
        if (data->accel.x != 0.0f) {
            data->pos.x += data->vel.x;
            data->pos.y += data->vel.y;
            data->pos.z += data->vel.z;
            data->vel.x *= 0.95f;
            data->vel.z *= 0.95f;
            if (data->vel.y > 0.1f) {
                data->vel.y -= 0.2f;
            }
            if (data->vel.y < 0.0f) {
                data->vel.y = 0.0f;
            }
            data->scale += data->accel.y;
            if (data->accel.y > 2.0f) {
                data->accel.y -= 1.5f;
            }
            alpha = data->color.a;
            alpha -= data->accel.z;
            if (data->accel.z > 3.0f) {
                data->accel.z -= 1.0f;
            }
            if (alpha < 0) {
                alpha = 0;
                data->accel.x = 0.0f;
            }
            data->color.a = alpha;
            if (data->accel.x == 0.0f) {
                data->scale = 0.0f;
            }
        }
    }
    if (particleP->emitCnt++ > 60) {
        effect->activeF = FALSE;
        fn_1_DE8C(effect);
    }
}

BOOL fn_1_E508(HuVecF pos, float radius)
{
    int i;
    M518_EFFECT *effect;

    for (i = 0; i < 64; i++) {
        effect = lbl_1_bss_50[i];
        if (!effect->activeF) {
            break;
        }
    }
    if (i == 64) {
        return FALSE;
    }
    effect->activeF = TRUE;
    effect->pos = pos;
    effect->radius = radius;
    fn_1_DEF4(effect);
    return TRUE;
}

float lbl_1_data_9C = 1800.0f;
float lbl_1_data_A0 = 1200.0f;

s16 fn_1_E5BC(HuVecF pos)
{
    float z;
    float absZ;
    int vol;
    s16 result;

    z = pos.z;
    if (z < 0.0f) {
        absZ = -z;
    }
    else {
        absZ = z;
    }
    if (absZ < lbl_1_data_9C) {
        vol = (s16)((15.0f / lbl_1_data_9C) * z) + 110;
    }
    else {
        vol = (z > 0.0f) ? 127 : 96;
    }
    result = vol;
    if (result > 127) {
        result = 127;
    }
    if (result < 96) {
        result = 96;
    }
    return result;
}

s16 fn_1_E6B8(HuVecF pos)
{
    float x;
    float absX;
    int pan;
    s16 result;

    x = pos.x;
    if (x < 0.0f) {
        absX = -x;
    }
    else {
        absX = x;
    }
    if (absX < lbl_1_data_A0) {
        pan = (s16)((32.0f / lbl_1_data_A0) * x) + 64;
    }
    else {
        pan = (x > 0.0f) ? 96 : 32;
    }
    result = pan;
    if (result > 96) {
        result = 96;
    }
    if (result < 32) {
        result = 32;
    }
    return result;
}

int fn_1_E7B4(int seId, HuVecF pos)
{
    return HuAudFXPlayVolPan(seId, fn_1_E5BC(pos), fn_1_E6B8(pos));
}
