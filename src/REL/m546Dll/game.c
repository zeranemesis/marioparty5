#include "types.h"

#include "string.h"
#include "game/gamework.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/charman.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/audio.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"
#include "REL/m546Dll.h"

#ifdef __MWERKS__
#define M546_RODATA __declspec(section ".rodata")
#else
#define M546_RODATA
#endif

double sin(double x);
double cos(double x);

/* m546.c */
extern M546Scrap lbl_1_bss_1F0[1000];
extern M546Scrap lbl_1_bss_4C[5];
extern M546Cam *lbl_1_bss_48;
extern M546Kami lbl_1_bss_14AB8;
extern M546Kami lbl_1_bss_14A10;
extern M546Plr lbl_1_bss_14B60[2];
extern M546Game lbl_1_bss_14C40;
void fn_1_884(s16 arg0);
void fn_1_102C(void);
void fn_1_1434(void);
s32 fn_1_17BC(f32 *arg0, f32 steps, f32 start, f32 target);
MGTIMER *fn_1_18DC(s32 type, s32 maxSec, s32 endSec, f32 x, f32 y);
void fn_1_1950(MGTIMER *timer);
BOOL fn_1_197C(MGTIMER *timer);
void fn_1_19A4(MGTIMER *timer);
void fn_1_19DC(void);
void fn_1_1F4C(M546Vec3 *arg0);
void fn_1_2E18(M546State *state, s16 idx, M546Vec3 *pos);
void fn_1_301C(void);
void fn_1_38A0(void);
void fn_1_3E2C(M546State *state, s16 idx, M546Vec3 *pos, s16 count);
void fn_1_4010(void);
void fn_1_487C(void);
void fn_1_554C(void);
void fn_1_557C(void);
void fn_1_55AC(s16 arg0, s16 arg1);
void fn_1_5674(s16 arg0);
void fn_1_56EC(s16 arg0, s16 arg1, s16 arg2);
void fn_1_58B0(void);

/* stage.c */
void fn_1_C2A4(void);

s32 fn_1_58D0(M546State *state);
s32 fn_1_6A90(void);
s32 fn_1_6AC4(void);
s32 fn_1_6B2C(void);
s32 fn_1_6CD4(void);
s16 fn_1_6D14(void);
s32 fn_1_6D4C(void);
s32 fn_1_6E94(void);
s32 fn_1_6EC4(void);
void fn_1_6F24(void);
void fn_1_7190(void);
s16 fn_1_743C(s16 arg0);
s16 fn_1_765C(s16 arg0);
void fn_1_7C60(s16 arg0);
s16 fn_1_7D70(s16 arg0);
s16 fn_1_8750(s16 arg0);
s16 fn_1_8CE4(s16 arg0);
s16 fn_1_9044(void);
s16 fn_1_9178(void);
s32 fn_1_9250(s16 arg0);
s32 fn_1_9920(s16 arg0);
s32 fn_1_9B7C(s16 arg0);
void fn_1_9F78(void);
s16 fn_1_A0BC(s16 arg0, s8 arg1);
s16 fn_1_A2E4(s16 arg0, f32 arg1);
s32 fn_1_A4EC(void);
s16 fn_1_AC58(void);
void fn_1_AE14(s16 arg0);

typedef struct M546CharPos_s {
    f32 x;
    f32 y;
} M546CharPos;

/* unit-owned .bss, declared in reverse address order so the offsets match the original */
s32 lbl_1_bss_15294;
s32 lbl_1_bss_15290;
s32 lbl_1_bss_1528C_pad; /* original symbol lbl_1_bss_15288 is 8 bytes long */
M546State *lbl_1_bss_15288;

/* unit-owned .data, in original address order */
M546CharPos lbl_1_data_450[13] = {
    { 34.0f, -42.0f }, { 34.0f, -42.0f }, { 34.0f, -42.0f }, { 34.0f, -42.0f }, { 34.0f, -42.0f },
    { 34.0f, -42.0f }, { 34.0f, -42.0f }, { 34.0f, -42.0f }, { 34.0f, -42.0f }, { 34.0f, -42.0f },
    { 34.0f, -42.0f }, { 34.0f, -42.0f }, { 34.0f, -42.0f },
};
f32 lbl_1_data_4B8[3] = { 34.0f, 0.0f, 60.0f };
f32 lbl_1_data_4C4 = 50.0f;

typedef struct M546F13_s {
    f32 v[13];
} M546F13;

typedef struct M546F15_s {
    M546Vec3 v[5];
} M546F15;

/* named rodata of the original unit */
M546_RODATA const M546F13 lbl_1_rodata_198 = {
    { 150.0f, 170.0f, 188.0f, 169.0f, 164.0f, 180.0f, 210.0f, 150.0f, 150.0f, 150.0f, 150.0f, 150.0f, 150.0f }
};
M546_RODATA const M546F15 lbl_1_rodata_1E4 = {
    { { 60.0f, 1000.0f, 125.0f }, { 30.0f, 125.0f, 800.0f }, { 30.0f, 125.0f, 800.0f }, { 18.0f, 125.0f, 100.0f },
      { 18.0f, 100.0f, 125.0f } }
};

s32 fn_1_58D0(M546State *state)
{
    s16 i;
    s16 j;
    s16 cnt;
    s16 v28;
    s32 ret = 1;
    s16 v26;

#ifdef TARGET_PC
    v28 = 0;
    v26 = 0;
#endif
    memset(&lbl_1_bss_14C40, 0, sizeof(M546Game));
    memset(lbl_1_bss_14B60, 0, sizeof(lbl_1_bss_14B60));
    memset(&lbl_1_bss_14AB8, 0, sizeof(M546Kami));
    memset(&lbl_1_bss_14A10, 0, sizeof(M546Kami));
    memset(lbl_1_bss_1F0, 0, sizeof(lbl_1_bss_1F0));
    memset(lbl_1_bss_4C, 0, sizeof(lbl_1_bss_4C));
    lbl_1_bss_15288 = state;
    for (i = 0; i < 7; i++) {
        lbl_1_bss_14C40.fxFlag[i] = -1;
    }
    for (i = 0; i < 2; i++) {
        lbl_1_bss_14B60[i].col = 0;
    }
    lbl_1_bss_14B60[0].scale.x = 1.0f;
    lbl_1_bss_14B60[0].scale.y = 1.0f;
    lbl_1_bss_14B60[0].scale.z = 1.0f;
    lbl_1_bss_14B60[1].scale.x = 1.0f;
    lbl_1_bss_14B60[1].scale.y = 1.0f;
    lbl_1_bss_14B60[1].scale.z = 1.0f;
    for (i = 0; i < 2; i++) {
        lbl_1_bss_14B60[i].hook = CharModelItemHookGet(lbl_1_bss_15288->chars[i].charNo, 1, 0);
    }
    lbl_1_bss_14C40.timer = frandmod(4);
    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_15288->chars[i].cpuLevel != 0) {
            lbl_1_bss_14B60[i].cnt68 = frandmod(4) + 1;
            lbl_1_bss_14B60[i].prob6A = frandmod(45) + 50;
        }
    }
    for (i = 0; i < 3; i++) {
        lbl_1_bss_14C40.order[i] = -1;
    }
    for (i = 0; i < 3; i++) {
        do {
            lbl_1_bss_14C40.order[i] = frandmod(3);
            j = 0;
            cnt = 0;
            for (; j < 3; j++) {
                if (lbl_1_bss_14C40.order[i] == lbl_1_bss_14C40.order[j]) {
                    cnt++;
                }
            }
        } while (cnt > 1);
        if (lbl_1_bss_14C40.order[i] == 0) {
            v28 = i;
        }
        if (lbl_1_bss_14C40.order[i] == 2) {
            v26 = i;
        }
    }
    lbl_1_bss_15288->models[0].id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_15288->dataNums[4], 0x10000000, 2));
    Hu3DModelAttrSet(lbl_1_bss_15288->models[0].id, 1);
    Hu3DModelCameraSet(lbl_1_bss_15288->models[0].id, 1);
    Hu3DModelLayerSet(lbl_1_bss_15288->models[0].id, 1);
    lbl_1_bss_15288->models[0].x = -300.0f + 300.0f * v28;
    lbl_1_bss_15288->models[0].y = 1000.0f;
    lbl_1_bss_15288->models[0].z = 0.0f;
    lbl_1_bss_15288->models[0].rotX = 0.0f;
    lbl_1_bss_15288->models[0].rotY = 0.0f;
    lbl_1_bss_15288->models[0].rotZ = 0.0f;
    lbl_1_bss_15288->models[11].id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_15288->dataNums[7], 0x10000000, 2));
    Hu3DModelAttrSet(lbl_1_bss_15288->models[11].id, 9);
    Hu3DModelCameraSet(lbl_1_bss_15288->models[11].id, 1);
    Hu3DModelLayerSet(lbl_1_bss_15288->models[11].id, 6);
    lbl_1_bss_15288->models[11].x = -300.0f + 300.0f * v28;
    lbl_1_bss_15288->models[11].y = 1000.0f;
    lbl_1_bss_15288->models[11].z = 0.0f;
    lbl_1_bss_15288->models[11].rotX = 0.0f;
    lbl_1_bss_15288->models[11].rotY = 0.0f;
    lbl_1_bss_15288->models[11].rotZ = 0.0f;
    Hu3DModelShadowSet(lbl_1_bss_15288->models[11].id);
    Hu3DModelShadowDispOff(lbl_1_bss_15288->models[11].id);
    Hu3DModelAttrReset(lbl_1_bss_15288->models[11].id, 1);
    lbl_1_bss_14C40.tp48 = 0.0f;
    Hu3DModelTPLvlSet(lbl_1_bss_15288->models[11].id, lbl_1_bss_14C40.tp48);
    i = 1;
    j = 0;
    for (; i <= 3; i++, j++) {
        lbl_1_bss_15288->models[i].id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_15288->dataNums[5], 0x10000000, 2));
        Hu3DModelAttrSet(lbl_1_bss_15288->models[i].id, 1);
        Hu3DModelCameraSet(lbl_1_bss_15288->models[i].id, 1);
        Hu3DModelLayerSet(lbl_1_bss_15288->models[i].id, 1);
        lbl_1_bss_15288->models[i].x = -300.0f + 300.0f * j;
        lbl_1_bss_15288->models[i].y = 1000.0f;
        lbl_1_bss_15288->models[i].z = 0.0f;
        lbl_1_bss_15288->models[i].rotX = 0.0f;
        lbl_1_bss_15288->models[i].rotY = 0.0f;
        lbl_1_bss_15288->models[i].rotZ = 0.0f;
        Hu3DModelAttrReset(lbl_1_bss_15288->models[i].id, 1);
    }
    i = 12;
    j = 0;
    for (; i <= 14; i++, j++) {
        lbl_1_bss_15288->models[i].id =
            Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_15288->dataNums[8], 0x10000000, 2));
        Hu3DModelAttrSet(lbl_1_bss_15288->models[i].id, 9);
        Hu3DModelCameraSet(lbl_1_bss_15288->models[i].id, 1);
        Hu3DModelLayerSet(lbl_1_bss_15288->models[i].id, 2);
        lbl_1_bss_15288->models[i].x = -300.0f + 300.0f * j;
        lbl_1_bss_15288->models[i].y = 101.0f;
        lbl_1_bss_15288->models[i].z = 0.0f;
        lbl_1_bss_15288->models[i].rotX = 0.0f;
        lbl_1_bss_15288->models[i].rotY = 0.0f;
        lbl_1_bss_15288->models[i].rotZ = 0.0f;
        Hu3DModelShadowSet(lbl_1_bss_15288->models[i].id);
        Hu3DModelShadowDispOn(lbl_1_bss_15288->models[i].id);
        Hu3DModelAttrReset(lbl_1_bss_15288->models[i].id, 1);
        Hu3DModelTPLvlSet(lbl_1_bss_15288->models[i].id, 0.0f);
        lbl_1_bss_14C40.tp4C[j] = 0.0f;
    }
    i = 8;
    j = 0;
    for (; i <= 10; i++, j++) {
        lbl_1_bss_15288->models[i].id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_15288->dataNums[6], 0x10000000, 2));
        Hu3DModelAttrSet(lbl_1_bss_15288->models[i].id, 0x40000001);
        Hu3DModelCameraSet(lbl_1_bss_15288->models[i].id, 1);
        Hu3DModelLayerSet(lbl_1_bss_15288->models[i].id, 1);
        lbl_1_bss_15288->models[i].x = -50.0f + 50.0f * j;
        lbl_1_bss_15288->models[i].y = 700.0f;
        lbl_1_bss_15288->models[i].z = -42.0f;
        lbl_1_bss_15288->models[i].rotX = 0.0f;
        lbl_1_bss_15288->models[i].rotY = 0.0f;
        lbl_1_bss_15288->models[i].rotZ = 0.0f;
        Hu3DModelAttrReset(lbl_1_bss_15288->models[i].id, 1);
        lbl_1_bss_14C40.tp34[j] = 0.0f;
        Hu3DModelTPLvlSet(lbl_1_bss_15288->models[i].id, lbl_1_bss_14C40.tp34[j]);
        Hu3DMotionTimeSet(lbl_1_bss_15288->models[i].id, 20.0f * j);
    }
    i = 4;
    j = 0;
    for (; i <= 7; i++, j++) {
        lbl_1_bss_15288->models[i].id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_15288->dataNums[j], 0x10000000, 2));
        Hu3DModelAttrSet(lbl_1_bss_15288->models[i].id, 1);
        lbl_1_bss_15288->models[i].x = -300.0f + 300.0f * v26;
        lbl_1_bss_15288->models[i].y = 1000.0f;
        lbl_1_bss_15288->models[i].z = 0.0f;
    }
    for (i = 0; i < 2; i++) {
        lbl_1_bss_15288->models[i + 27].x = -150.0f + 300.0f * i + lbl_1_data_4B8[0];
        lbl_1_bss_15288->models[i + 27].y = 100.0f;
        lbl_1_bss_15288->models[i + 27].z = -200.0f;
    }
    fn_1_7190();
    fn_1_19DC();
    return ret;
}

s32 fn_1_6A90(void)
{
    s32 ret = 0;

    fn_1_A4EC();
    fn_1_7190();
    return ret;
}

s32 fn_1_6AC4(void)
{
    s32 ret = 1;

    lbl_1_bss_14C40.phase = 0;
    if (lbl_1_bss_14C40.bgmFlag == 0) {
        fn_1_557C();
        lbl_1_bss_14C40.bgmFlag = 1;
    }
    fn_1_7190();
    return ret;
}

s32 fn_1_6B2C(void)
{
    s16 i;
    s16 playerNo;
    s32 ret;
    s32 prod;

    ret = 0;
    prod = 1;
    switch (lbl_1_bss_15288->phase) {
    case 0:
        for (i = 0; i < 2; i++) {
            prod *= fn_1_743C(i);
        }
        if (prod != 0) {
            lbl_1_bss_14B60[0].sub = 0;
            lbl_1_bss_14B60[1].sub = 0;
            lbl_1_bss_15288->phase = 1;
        }
        break;
    case 1:
        if (fn_1_9044() != 0) {
            lbl_1_bss_15288->phase = 2;
        }
        break;
    case 2:
        MgSeqWinnerSet(lbl_1_bss_15288->chars[lbl_1_bss_14C40.win].charNo, -1, -1, -1);
        playerNo = lbl_1_bss_15288->chars[lbl_1_bss_14C40.win].playerNo;
        if (_CheckFlag(0x1000F) == 0) {
            GwPlayer[playerNo].mgCoinBonus = 10;
        }
        lbl_1_bss_14C40.bgmFlag = 0;
        ret = 1;
        break;
    }
    fn_1_7190();
    return ret;
}

s32 fn_1_6CD4(void)
{
    s32 ret = 0;

    if (lbl_1_bss_14C40.bgmFlag == 0) {
        lbl_1_bss_14C40.bgmFlag = 1;
    }
    return ret;
}

s16 fn_1_6D14(void)
{
    s16 ret = 0;

    ret = fn_1_AC58();
    fn_1_7190();
    return ret;
}

s32 fn_1_6D4C(void)
{
    M546StageSlot *slot;
    s32 ret = 0;

    if (lbl_1_bss_14C40.flag2 == 0) {
        slot = &lbl_1_bss_15288->slots[lbl_1_bss_14C40.win];
        slot->attrReset = 0x40000013;
        slot->motion = 7;
        slot->speed = 1.0f;
        slot->time = 0.0f;
        slot->attrSet = 0;
        slot = &lbl_1_bss_15288->slots[lbl_1_bss_14C40.lose];
        slot->attrReset = 0x40000001;
        slot->motion = 8;
        slot->speed = 1.0f;
        slot->time = 0.0f;
        slot->attrSet = 0;
        lbl_1_bss_14C40.flag2 = 1;
    }
    fn_1_7190();
    CharModelVoicePanSet(lbl_1_bss_15288->chars[lbl_1_bss_14C40.win].charNo, 0x40);
    return ret;
}

s32 fn_1_6E94(void)
{
    s32 ret = 1;

    fn_1_7190();
    return ret;
}

s32 fn_1_6EC4(void)
{
    s32 ret = 1;

    fn_1_58B0();
    if (lbl_1_bss_14AB8.active != 0) {
        fn_1_301C();
    }
    if (lbl_1_bss_14A10.active != 0) {
        fn_1_4010();
    }
    return ret;
}

void fn_1_6F24(void)
{
    s16 i;
    s32 pan;
    HU3DMODEL *m0;
    HU3DMODEL *m1;
    f32 f;

    m0 = &Hu3DData[lbl_1_bss_15288->models[27].id];
    m1 = &Hu3DData[lbl_1_bss_15288->models[28].id];
    for (i = 0; i < 29; i++) {
        if (i != 4 && i != 5 && i != 6 && i != 7) {
            Hu3DModelPosSet(lbl_1_bss_15288->models[i].id, lbl_1_bss_15288->models[i].x,
                            lbl_1_bss_15288->models[i].y, lbl_1_bss_15288->models[i].z);
            Hu3DModelRotSet(lbl_1_bss_15288->models[i].id, lbl_1_bss_15288->models[i].rotX,
                            lbl_1_bss_15288->models[i].rotY, lbl_1_bss_15288->models[i].rotZ);
        }
    }
    f = 0.10666667f * m0->pos.x;
    pan = 64.0f + f;
    CharModelVoicePanSet(lbl_1_bss_15288->chars[0].charNo, pan);
    f = 0.10666667f * m1->pos.x;
    pan = 64.0f + f;
    CharModelVoicePanSet(lbl_1_bss_15288->chars[1].charNo, pan);
}

void fn_1_7190(void)
{
    s16 i;
    HU3DMODEL *m0;
    HU3DMODEL *m1;

    if (lbl_1_bss_14AB8.active != 0) {
        fn_1_38A0();
    }
    if (lbl_1_bss_14A10.active != 0) {
        fn_1_487C();
    }
    fn_1_6F24();
    fn_1_884(0);
    fn_1_102C();
    fn_1_1434();
}

s16 fn_1_743C(s16 arg0)
{
    s16 ret;

    ret = 0;
    switch (lbl_1_bss_14B60[arg0].state) {
    case 0:
        if (lbl_1_bss_14C40.cur == arg0 && fn_1_765C(arg0) != 0) {
            lbl_1_bss_14B60[arg0].state = 1;
        }
        break;
    case 1:
        if (fn_1_7D70(arg0) != 0) {
            lbl_1_bss_14B60[arg0].state = 2;
        }
        break;
    case 2:
        if (fn_1_8750(arg0) != 0) {
            lbl_1_bss_14B60[arg0].state = 3;
        }
        break;
    case 3:
        if (arg0 == 0) {
            lbl_1_bss_14C40.cur = 1;
        } else if (arg0 == 1) {
            lbl_1_bss_14B60[0].state = 4;
            lbl_1_bss_14B60[1].state = 4;
            fn_1_9F78();
            fn_1_56EC(0, 0x7F, (lbl_1_bss_14C40.diff28 << 5) + 0x40);
        }
        break;
    case 4:
        if (arg0 == 0 && fn_1_A0BC(1, lbl_1_bss_14C40.diff28) != 0) {
            lbl_1_bss_14B60[0].state = 5;
            lbl_1_bss_14B60[1].state = 5;
        }
        break;
    case 5:
        if (fn_1_8CE4(arg0) != 0) {
            ret = 1;
        }
        break;
    }
    return ret;
}

s16 fn_1_765C(s16 arg0)
{
    M546StageSlot *slot;
    s16 ret;
    s8 mdl;

    slot = &lbl_1_bss_15288->slots[arg0];
    ret = 0;
    mdl = arg0 + 27;
    switch (lbl_1_bss_14B60[arg0].sub) {
    case 0:
        lbl_1_bss_14B60[arg0].f64 = lbl_1_data_4B8[0];
        if (arg0 == 0) {
            lbl_1_bss_14B60[arg0].f5C = 180.0f;
        } else if (lbl_1_bss_14B60[0].col == 0) {
            lbl_1_bss_14B60[arg0].f5C = 180.0f;
            lbl_1_bss_14B60[arg0].f64 += 300.0f;
            lbl_1_bss_14B60[arg0].col = 1;
        } else {
            lbl_1_bss_14B60[arg0].f5C = 0.0f;
        }
        lbl_1_bss_14B60[arg0].sub = 1;
        break;
    case 1:
        lbl_1_bss_15288->models[mdl].x =
            lbl_1_bss_14B60[arg0].f64 + 150.0 * cos(3.141592653589793 * lbl_1_bss_14B60[arg0].f5C / 180.0);
        lbl_1_bss_15288->models[mdl].z =
            -200.0 + (200.0f + lbl_1_data_4B8[2]) * sin(3.141592653589793 * lbl_1_bss_14B60[arg0].f5C / 180.0);
        if (arg0 == 0 || lbl_1_bss_14B60[0].col == 0) {
            lbl_1_bss_14B60[arg0].f5C -= 1.8f;
            lbl_1_bss_15288->models[mdl].rotY += 1.0f;
        } else {
            lbl_1_bss_14B60[arg0].f5C += 1.8f;
            lbl_1_bss_15288->models[mdl].rotY -= 1.0f;
        }
        fn_1_A2E4(arg0, 60.0f);
        if (lbl_1_bss_14B60[arg0].timer++ >= 50) {
            lbl_1_bss_15288->models[mdl].x = lbl_1_bss_14B60[arg0].f64;
            lbl_1_bss_15288->models[mdl].z = lbl_1_data_4B8[2];
            lbl_1_bss_14B60[arg0].sub = 2;
        }
        break;
    case 2:
        if (arg0 == 0 || lbl_1_bss_14B60[0].col == 0) {
            lbl_1_bss_15288->models[mdl].rotY -= 5.0f;
        } else {
            lbl_1_bss_15288->models[mdl].rotY += 5.0f;
        }
        if (fn_1_A2E4(arg0, 60.0f) != 0) {
            lbl_1_bss_15288->models[mdl].rotY = 0.0f;
            lbl_1_bss_14B60[arg0].sub = 3;
        }
        break;
    case 3:
        lbl_1_bss_14C40.timerId = fn_1_18DC(0, 10, 0, 292.0f, 64.0f);
        fn_1_1950(lbl_1_bss_14C40.timerId);
        lbl_1_bss_14B60[arg0].sub = 0;
        lbl_1_bss_14B60[arg0].timer = 0;
        ret = 1;
        break;
    }
    return ret;
}

void fn_1_7C60(s16 arg0)
{
    s16 t;

    if (fn_1_197C(lbl_1_bss_14C40.timerId) != 0) {
        if (arg0 == 1 && lbl_1_bss_14B60[0].col == lbl_1_bss_14B60[1].col) {
            do {
                t = frandmod(3) - 1;
            } while (lbl_1_bss_14B60[0].col == t);
            if (lbl_1_bss_14B60[1].col > t) {
                lbl_1_bss_15288->chars[1].stkX = -1;
                return;
            }
            lbl_1_bss_15288->chars[1].stkX = 1;
            return;
        }
        lbl_1_bss_15288->chars[arg0].btnDown = 0x100;
    }
}

s16 fn_1_7D70(s16 arg0)
{
    s16 ret;
    s8 mdl;
    s8 moved;
    u8 stkX;
    s16 t;
    f32 ten;

    ret = 0;
    mdl = arg0 + 27;
    moved = 0;
    ten = 10.0f;
    switch (lbl_1_bss_14B60[arg0].sub) {
    case 0:
        fn_1_AE14(arg0);
        fn_1_7C60(arg0);
        stkX = *(u8 *)&lbl_1_bss_15288->chars[arg0].stkX;
        if ((lbl_1_bss_15288->chars[arg0].btnDown & 0x100) == 0) {
            lbl_1_bss_14B60[arg0].from = *(M546Vec3 *)&lbl_1_bss_15288->models[mdl].x;
            lbl_1_bss_14B60[arg0].to = *(M546Vec3 *)&lbl_1_bss_15288->models[mdl].x;
            lbl_1_bss_14B60[arg0].rot = *(M546Vec3 *)&lbl_1_bss_15288->models[mdl].rotX;
            if ((s8)stkX < 0 && lbl_1_bss_14B60[arg0].col != -1) {
                if (arg0 == 0 || lbl_1_bss_14B60[0].col != lbl_1_bss_14B60[arg0].col - 1) {
                    lbl_1_bss_14B60[arg0].f4C = -90.0f;
                    lbl_1_bss_14B60[arg0].to.x -= 300.0f;
                    lbl_1_bss_14B60[arg0].f10 = 50.0f;
                    lbl_1_bss_14B60[arg0].col--;
                    moved = 1;
                } else if (lbl_1_bss_14B60[arg0].col == 1) {
                    lbl_1_bss_14B60[arg0].f4C = -90.0f;
                    lbl_1_bss_14B60[arg0].to.x -= 600.0f;
                    lbl_1_bss_14B60[arg0].f10 = 100.0f;
                    lbl_1_bss_14B60[arg0].col = -1;
                    moved = 1;
                }
            }
            if ((s8)stkX > 0 && lbl_1_bss_14B60[arg0].col != 1) {
                if (arg0 == 0 || lbl_1_bss_14B60[0].col != lbl_1_bss_14B60[arg0].col + 1) {
                    lbl_1_bss_14B60[arg0].f4C = 90.0f;
                    lbl_1_bss_14B60[arg0].to.x += 300.0f;
                    lbl_1_bss_14B60[arg0].f10 = 50.0f;
                    lbl_1_bss_14B60[arg0].col++;
                    moved = 1;
                } else if (lbl_1_bss_14B60[arg0].col == -1) {
                    lbl_1_bss_14B60[arg0].f4C = 90.0f;
                    lbl_1_bss_14B60[arg0].to.x += 600.0f;
                    lbl_1_bss_14B60[arg0].f10 = 100.0f;
                    lbl_1_bss_14B60[arg0].col = 1;
                    moved = 1;
                }
            }
            if (moved != 0) {
                lbl_1_bss_14B60[arg0].sub = 1;
            }
        } else if (arg0 == 0 || (arg0 == 1 && lbl_1_bss_14B60[0].col != lbl_1_bss_14B60[1].col)) {
            lbl_1_bss_14B60[arg0].sub = 0;
            ret = 1;
        }
        break;
    case 1:
        fn_1_A2E4(arg0, 5.0f + lbl_1_bss_14B60[arg0].f10);
        fn_1_17BC(&lbl_1_bss_15288->models[mdl].x, lbl_1_bss_14B60[arg0].f10, lbl_1_bss_14B60[arg0].from.x,
                  lbl_1_bss_14B60[arg0].to.x);
        if (fn_1_17BC(&lbl_1_bss_15288->models[mdl].rotY, ten, lbl_1_bss_14B60[arg0].rot.y,
                      lbl_1_bss_14B60[arg0].f4C) == 0) {
            lbl_1_bss_14B60[arg0].sub = 2;
        }
        break;
    case 2:
        fn_1_A2E4(arg0, 5.0f + lbl_1_bss_14B60[arg0].f10);
        if ((f32)fn_1_17BC(&lbl_1_bss_15288->models[mdl].x, lbl_1_bss_14B60[arg0].f10,
                           lbl_1_bss_14B60[arg0].from.x, lbl_1_bss_14B60[arg0].to.x) <= (ten - ten * 0.5f)) {
            lbl_1_bss_14B60[arg0].sub = 3;
        }
        break;
    case 3:
        fn_1_17BC(&lbl_1_bss_15288->models[mdl].x, lbl_1_bss_14B60[arg0].f10, lbl_1_bss_14B60[arg0].from.x,
                  lbl_1_bss_14B60[arg0].to.x);
        fn_1_17BC(&lbl_1_bss_15288->models[mdl].rotY, ten, lbl_1_bss_14B60[arg0].f4C, 0.0f);
        if (fn_1_A2E4(arg0, 5.0f + lbl_1_bss_14B60[arg0].f10) != 0) {
            lbl_1_bss_14B60[arg0].sub = 0;
        }
        break;
    }
    return ret;
}

s16 fn_1_8750(s16 arg0)
{
    M546StageSlot *slot;
    s16 ret;
    s8 mdl;
    f32 fx;
    f32 fy;
    f32 fr;
    f32 fd;

    slot = &lbl_1_bss_15288->slots[arg0];
    ret = 0;
    mdl = arg0 + 27;
    switch (lbl_1_bss_14B60[arg0].sub) {
    case 0:
        fn_1_19A4(lbl_1_bss_14C40.timerId);
        lbl_1_bss_14B60[arg0].f5C = 90.0f;
        lbl_1_bss_14B60[arg0].f60 = 90.0f;
        lbl_1_bss_14B60[arg0].sub = 1;
        break;
    case 1:
        fr = lbl_1_data_4C4;
        fx = lbl_1_data_450[lbl_1_bss_15288->chars[arg0].charNo].x + 300.0f * lbl_1_bss_14B60[arg0].col;
        fd = lbl_1_data_4B8[2] - lbl_1_data_450[lbl_1_bss_15288->chars[arg0].charNo].y;
        fy = lbl_1_data_450[lbl_1_bss_15288->chars[arg0].charNo].y;
        lbl_1_bss_15288->models[mdl].x =
            fx + fr * cos(3.141592653589793 * lbl_1_bss_14B60[arg0].f60 / 180.0);
        lbl_1_bss_15288->models[mdl].z =
            fy + fd * sin(3.141592653589793 * lbl_1_bss_14B60[arg0].f5C / 180.0);
        lbl_1_bss_14B60[arg0].f5C -= 3.375f;
        lbl_1_bss_14B60[arg0].f60 -= 2.25f;
        lbl_1_bss_15288->models[mdl].rotY += 4.5f;
        if (fn_1_A2E4(arg0, 80.0f) != 0) {
            lbl_1_bss_15288->models[mdl].x =
                lbl_1_data_450[lbl_1_bss_15288->chars[arg0].charNo].x + 300.0f * lbl_1_bss_14B60[arg0].col;
            lbl_1_bss_15288->models[mdl].z = lbl_1_data_450[lbl_1_bss_15288->chars[arg0].charNo].y;
            lbl_1_bss_15288->models[mdl].rotY = 0.0f;
            lbl_1_bss_14B60[arg0].sub = 2;
        }
        break;
    case 2:
        slot->shiftState = 1;
        slot->shiftMotion = 5;
        slot->shiftStart = 0.0f;
        slot->shiftEnd = 16.0f;
        slot->shiftAttr = 0x40000001;
        lbl_1_bss_14B60[arg0].sub = 3;
        break;
    case 3:
        lbl_1_bss_14B60[arg0].sub = 0;
        ret = 1;
        break;
    }
    return ret;
}

s16 fn_1_8CE4(s16 arg0)
{
    M546StageSlot *slot;
    s16 ret;

    slot = &lbl_1_bss_15288->slots[arg0];
    ret = 0;
    switch (lbl_1_bss_14B60[arg0].sub) {
    case 0:
        lbl_1_bss_14B60[arg0].timer = 0;
        lbl_1_bss_14B60[arg0].sub = 1;
        break;
    case 1:
        if (lbl_1_bss_14B60[arg0].timer++ >= 0x78) {
            fn_1_56EC(1, 0x7F, (lbl_1_bss_14B60[arg0].col << 5) + 0x40);
            fn_1_55AC(1, 0x3E8);
            slot->attrReset = 0x40000001;
            slot->shiftState = 1;
            slot->shiftMotion = 6;
            slot->shiftStart = 0.0f;
            slot->shiftEnd = 4.0f;
            slot->shiftAttr = 0;
            lbl_1_bss_14B60[arg0].sub = 2;
        }
        break;
    case 2:
        if (slot->shiftState == 3) {
            lbl_1_bss_14B60[arg0].sub = 3;
        }
        break;
    case 3:
        if (fn_1_A0BC(3, lbl_1_bss_14B60[arg0].col) != 0) {
            lbl_1_bss_14B60[arg0].sub = 4;
        }
        break;
    case 4:
        if (fn_1_A0BC(4, lbl_1_bss_14B60[arg0].col) != 0) {
            lbl_1_bss_14B60[arg0].sub = 5;
        }
        break;
    case 5:
        if (lbl_1_bss_14B60[arg0].timer++ >= 0x1E) {
            slot->shiftState = 1;
            slot->shiftMotion = 0;
            slot->shiftStart = 0.0f;
            slot->shiftEnd = 20.0f;
            slot->shiftAttr = 0x40000001;
            fn_1_56EC(0, 0x7F, (lbl_1_bss_14B60[arg0].col << 5) + 0x40);
            lbl_1_bss_14B60[arg0].sub = 6;
        }
        break;
    case 6:
        if (fn_1_A0BC(2, lbl_1_bss_14B60[arg0].col) != 0) {
            ret = 1;
        }
        break;
    }
    return ret;
}

s16 fn_1_9044(void)
{
    s16 ret;
    s16 sp8;

    ret = 0;
    sp8 = 0;
    switch (lbl_1_bss_15288->flagE4) {
    case 0:
        lbl_1_bss_14B60[0].timer = 0;
        lbl_1_bss_14B60[1].timer = 0;
        lbl_1_bss_14C40.timer = 10;
        if (lbl_1_bss_14C40.timer == 0) {
            lbl_1_bss_14C40.timer = 180;
        }
        lbl_1_bss_15288->flagE4 = 1;
        break;
    case 1:
        if (lbl_1_bss_14C40.timer-- <= 0) {
            lbl_1_bss_15288->flagE4 = 2;
        }
        break;
    case 2:
        if (fn_1_9178() != 0) {
            lbl_1_bss_15288->flagE4 = 0;
            ret = 1;
        }
        break;
    }
    return ret;
}

s16 fn_1_9178(void)
{
    s16 i;
    s16 ret;

    ret = 1;
    for (i = 0; i < 2; i++) {
        switch (lbl_1_bss_14C40.order[lbl_1_bss_14B60[i].col + 1]) {
        case 0:
            ret *= fn_1_9250(i);
            break;
        case 1:
            ret *= fn_1_9920(i);
            break;
        case 2:
            ret *= fn_1_9B7C(i);
            break;
        }
    }
    return ret;
}

s32 fn_1_9250(s16 arg0)
{
    M546F13 tbl;
    s32 ret;
    s8 mdl;

    ret = 0;
    mdl = arg0 + 27;
    tbl = lbl_1_rodata_198;
    switch (lbl_1_bss_14B60[arg0].sub) {
    case 0:
        lbl_1_bss_14C40.fallVel = -8.259f;
        fn_1_56EC(2, 0x7F, (lbl_1_bss_14B60[arg0].col << 5) + 0x40);
        lbl_1_bss_14B60[arg0].timer = 0;
        lbl_1_bss_14B60[arg0].sub = 1;
        break;
    case 1:
        if (lbl_1_bss_14B60[arg0].timer++ >= 0x3C) {
            lbl_1_bss_14B60[arg0].timer = 0;
            lbl_1_bss_14B60[arg0].sub = 2;
        }
        break;
    case 2:
        Hu3DModelAttrReset(lbl_1_bss_15288->models[0].id, 1);
        Hu3DModelShadowDispOn(lbl_1_bss_15288->models[11].id);
        lbl_1_bss_15288->models[0].x = lbl_1_bss_15288->models[mdl].x;
        lbl_1_bss_15288->models[0].y = 1000.0f;
        lbl_1_bss_15288->models[0].z = lbl_1_bss_15288->models[mdl].z;
        *(M546Vec3 *)&lbl_1_bss_15288->models[11].x = *(M546Vec3 *)&lbl_1_bss_15288->models[0].x;
        lbl_1_bss_15288->models[11].y = 101.0f;
        lbl_1_bss_14B60[arg0].sub = 3;
        break;
    case 3:
        lbl_1_bss_14C40.fallVel -= 0.753f;
        lbl_1_bss_15288->models[0].y += lbl_1_bss_14C40.fallVel;
        if (lbl_1_bss_15288->models[0].y <= 100.0f) {
            lbl_1_bss_15288->models[0].y = 100.0f;
            fn_1_1F4C((M546Vec3 *)&lbl_1_bss_15288->models[0].x);
            fn_1_5804(2);
            fn_1_56EC(3, 0x7F, (lbl_1_bss_14B60[arg0].col << 5) + 0x40);
            omVibrate(lbl_1_bss_15288->chars[arg0].playerNo, 0x20, 0xC, 0);
            lbl_1_bss_14B60[arg0].sub = 4;
        }
        if (100.0f + tbl.v[lbl_1_bss_15288->chars[arg0].charNo] >= lbl_1_bss_15288->models[0].y) {
            if (lbl_1_bss_14B60[arg0].flag54 == 0) {
                CharFXPlayVolPan(lbl_1_bss_15288->chars[arg0].charNo, 0x202, 0x7F,
                                 (lbl_1_bss_14B60[arg0].col << 5) + 0x40);
                lbl_1_bss_14B60[arg0].flag54 = 1;
            }
            lbl_1_bss_14B60[arg0].scale.y -= 0.2f;
            if (lbl_1_bss_14B60[arg0].scale.y <= 0.1f) {
                lbl_1_bss_14B60[arg0].scale.x = 0.1f;
                lbl_1_bss_14B60[arg0].scale.y = 0.1f;
                lbl_1_bss_14B60[arg0].scale.z = 0.1f;
            }
            Hu3DModelScaleSetV(lbl_1_bss_15288->models[mdl].id, (HuVecF *)&lbl_1_bss_14B60[arg0].scale);
        }
        if ((lbl_1_bss_14C40.tp48 += 0.025f) >= 1.0f) {
            lbl_1_bss_14C40.tp48 = 1.0f;
        }
        Hu3DModelTPLvlSet(lbl_1_bss_15288->models[11].id, lbl_1_bss_14C40.tp48);
        break;
    case 4:
        if (lbl_1_bss_14B60[arg0].timer++ >= 0x3C) {
            ret = 1;
        }
        break;
    }
    return ret;
}

s32 fn_1_9920(s16 arg0)
{
    M546Vec3 sp14;
    M546Vec3 sp8;
    s16 vol;
    s32 ret;

    ret = 0;
    switch (lbl_1_bss_14B60[arg0].sub) {
    case 0:
        if (lbl_1_bss_14B60[arg0].timer++ > 0x14) {
            lbl_1_bss_14B60[arg0].sub = 1;
        }
        break;
    case 1:
        sp14 = *(M546Vec3 *)&lbl_1_bss_15288->models[arg0 + 27].x;
        sp14.y = 700.0f;
        sp8 = sp14;
        fn_1_3E2C(lbl_1_bss_15288, 4, &sp8, 5);
        switch (lbl_1_bss_14B60[arg0].col) {
        case -1:
            vol = 0x28;
            break;
        case 0:
            vol = 0x40;
            break;
        case 1:
            vol = 0x58;
            break;
        }
        if (lbl_1_bss_14C40.order[lbl_1_bss_14B60[arg0 ^ 1].col + 1] != 2) {
            fn_1_56EC(4, 0x7F, vol);
        }
        lbl_1_bss_14B60[arg0].timer = 0;
        lbl_1_bss_14B60[arg0].sub = 2;
        break;
    case 2:
        if (lbl_1_bss_14B60[arg0].timer++ > 0x78) {
            ret = 1;
        }
        break;
    }
    return ret;
}

s32 fn_1_9B7C(s16 arg0)
{
    M546Vec3 sp14;
    M546Vec3 sp8;
    s16 vol;
    s16 i;
    s32 ret;

    ret = 0;
    switch (lbl_1_bss_14B60[arg0].sub) {
    case 0:
        if (lbl_1_bss_14B60[arg0].timer++ > 0x14) {
            lbl_1_bss_14B60[arg0].sub = 1;
        }
        break;
    case 1:
        sp14 = *(M546Vec3 *)&lbl_1_bss_15288->models[arg0 + 27].x;
        sp14.y = 700.0f;
        lbl_1_bss_15288->models[8].x = sp14.x - 50.0f;
        lbl_1_bss_15288->models[8].y = 700.0f;
        lbl_1_bss_15288->models[8].z = sp14.z;
        lbl_1_bss_15288->models[9].x = sp14.x;
        lbl_1_bss_15288->models[9].y = 700.0f;
        lbl_1_bss_15288->models[9].z = sp14.z;
        lbl_1_bss_15288->models[10].x = 50.0f + sp14.x;
        lbl_1_bss_15288->models[10].y = 700.0f;
        lbl_1_bss_15288->models[10].z = sp14.z;
        sp8 = sp14;
        fn_1_2E18(lbl_1_bss_15288, 4, &sp8);
        switch (lbl_1_bss_14B60[arg0].col) {
        case -1:
            vol = 0x28;
            break;
        case 0:
            vol = 0x40;
            break;
        case 1:
            vol = 0x58;
            break;
        }
        fn_1_56EC(5, 0x7F, vol);
        lbl_1_bss_14B60[arg0].timer = 0;
        lbl_1_bss_14B60[arg0].sub = 2;
        break;
    case 2:
        if ((lbl_1_bss_14AB8.active & lbl_1_bss_14C40.mask30) != 0) {
            for (i = 0; i < 3; i++) {
                lbl_1_bss_14C40.tp34[i] += 0.01f;
                if (lbl_1_bss_14C40.tp34[i] >= 1.0f) {
                    lbl_1_bss_14C40.tp34[i] = 1.0f;
                }
                Hu3DModelTPLvlSet(lbl_1_bss_15288->models[i + 8].id, lbl_1_bss_14C40.tp34[i]);
            }
        }
        if (lbl_1_bss_14B60[arg0].timer++ > 0x78) {
            ret = 1;
        }
        break;
    }
    return ret;
}

void fn_1_9F78(void)
{
    s16 a;
    s16 b;

    a = lbl_1_bss_14C40.order[lbl_1_bss_14B60[0].col + 1];
    b = lbl_1_bss_14C40.order[lbl_1_bss_14B60[1].col + 1];
    if (a > b) {
        lbl_1_bss_14C40.win = 0;
        lbl_1_bss_14C40.lose = 1;
    } else if (a < b) {
        lbl_1_bss_14C40.win = 1;
        lbl_1_bss_14C40.lose = 0;
    }
    switch (lbl_1_bss_14B60[0].col + lbl_1_bss_14B60[1].col) {
    case -1:
        lbl_1_bss_14C40.diff28 = 1;
        break;
    case 0:
        lbl_1_bss_14C40.diff28 = 0;
        break;
    case 1:
        lbl_1_bss_14C40.diff28 = -1;
        break;
    }
}

s16 fn_1_A0BC(s16 arg0, s8 arg1)
{
    M546F15 tbl;
    f32 v;
    s16 ret;
    s8 idx2;
    s8 idx;

    idx2 = arg1 + 0xD;
    ret = 0;
    idx = arg1 + 2;
    tbl = lbl_1_rodata_1E4;
    if (fn_1_17BC(&lbl_1_bss_15288->models[idx].y, tbl.v[arg0].x, tbl.v[arg0].y, tbl.v[arg0].z) == 0) {
        ret = 1;
    }
    v = (800.0f - lbl_1_bss_15288->models[idx].y) / 675.0f;
    if (v <= 0.0f) {
        v = 0.0f;
    }
    if (v >= 1.0f) {
        v = 1.0f;
    }
    lbl_1_bss_14C40.tp4C[arg1 + 1] = v;
    Hu3DModelTPLvlSet(lbl_1_bss_15288->models[idx2].id, lbl_1_bss_14C40.tp4C[arg1 + 1]);
    return ret;
}

s16 fn_1_A2E4(s16 arg0, f32 arg1)
{
    M546StageSlot *slot;
    s16 ret;

    ret = 0;
    slot = &lbl_1_bss_15288->slots[arg0];
    lbl_1_bss_14B60[arg0].f58 += 1.0f;
    switch (lbl_1_bss_14B60[arg0].flag56) {
    case 0:
        slot->attrReset = 0x40000001;
        slot->shiftState = 1;
        slot->shiftMotion = 2;
        slot->shiftStart = 0.0f;
        slot->shiftEnd = 8.0f;
        slot->shiftAttr = 0x40000001;
        lbl_1_bss_14B60[arg0].f58 = 0.0f;
        lbl_1_bss_14B60[arg0].flag56 = 1;
        break;
    case 1:
        if (10.0f + lbl_1_bss_14B60[arg0].f58 >= arg1) {
            slot->shiftState = 1;
            slot->shiftMotion = 0;
            slot->shiftStart = 0.0f;
            slot->shiftEnd = 10.0f;
            slot->shiftAttr = 0x40000001;
            lbl_1_bss_14B60[arg0].flag56 = 2;
        }
        break;
    case 2:
        if (lbl_1_bss_14B60[arg0].f58 >= arg1) {
            lbl_1_bss_14B60[arg0].flag56 = 0;
            ret = 1;
        }
        break;
    }
    return ret;
}

s32 fn_1_A4EC(void)
{
    s32 ret;
    s16 sum;
    s32 r2;
    s32 r3;

    ret = 0;
    sum = 0;

    switch (lbl_1_bss_14C40.phase) {
    case 0:
        fn_1_554C();
        lbl_1_bss_14C40.phase = 1;
        /* fallthrough */
    case 1:
        lbl_1_bss_48->rotX = -2.0f;
        lbl_1_bss_48->zoom = 1800.0f;
        if (lbl_1_bss_15288->timerF0 == 0x3C) {
            fn_1_C2A4();
        }
        if (lbl_1_bss_15288->timerF0++ > 0xC8) {
            lbl_1_bss_48->rotX = -2.0f;
            lbl_1_bss_48->zoom = 1800.0f;
            lbl_1_bss_15288->timerF0 = 0;
            lbl_1_bss_14C40.phase = 2;
        }
        break;
    case 2:
        sum += fn_1_17BC(&lbl_1_bss_48->rotX, 60.0f, -2.0f, -6.0f);
        sum += fn_1_17BC(&lbl_1_bss_48->zoom, 60.0f, 1800.0f, 830.0f);
        if (sum == 0) {
            lbl_1_bss_14C40.phase = 3;
        }
        break;
    case 3:
        sum = 1;
        sum *= fn_1_A0BC(0, 0);
        if (lbl_1_bss_14C40.cnt20++ == 0x1E) {
            fn_1_5674(0);
        }
        if (lbl_1_bss_15288->timerF0++ > 0x14) {
            r2 = fn_1_A0BC(0, -1);
            r3 = fn_1_A0BC(0, 1);
            sum = (s16)(sum * r2) * r3;
        }
        if (sum != 0) {
            lbl_1_bss_14C40.phase = 4;
        }
        break;
    case 4:
        lbl_1_bss_15288->timerF0 = 0;
        break;
    }
    return ret;
}

s16 fn_1_AC58(void)
{
    s16 ret;
    s16 sum;
    s8 mdl;

    ret = 0;
    sum = 0;
    mdl = lbl_1_bss_14C40.win + 0x1B;
    switch (lbl_1_bss_14C40.phase) {
    case 0:
        if (lbl_1_bss_15288->timerF0++ > 0x78) {
            lbl_1_bss_15288->timerF0 = 0;
            lbl_1_bss_14C40.phase = 1;
        }
        break;
    case 1:
        sum += fn_1_17BC(&lbl_1_bss_48->center.x, 30.0f,
                         lbl_1_data_450[lbl_1_bss_15288->chars[lbl_1_bss_14C40.win].charNo].x,
                         lbl_1_bss_15288->models[mdl].x);
        sum += fn_1_17BC(&lbl_1_bss_48->zoom, 30.0f, 830.0f, 500.0f);
        if (sum == 0) {
            lbl_1_bss_14C40.phase = 2;
        }
        break;
    case 2:
        lbl_1_bss_14C40.fx68++;
        ret = 1;
        break;
    }
    return ret;
}

void fn_1_AE14(s16 arg0)
{
    switch (lbl_1_bss_14B60[arg0].state6C) {
    case 0:
        lbl_1_bss_14B60[arg0].timer = frandmod(0x3C) + 0x1E;
        lbl_1_bss_14B60[arg0].state6C = 1;
        return;
    case 1:
        if (lbl_1_bss_14B60[arg0].timer-- <= 0) {
            lbl_1_bss_14B60[arg0].state6C = 2;
            return;
        }
        return;
    case 2:
        if (lbl_1_bss_15288->chars[arg0].cpuLevel != 0) {
            if (frandmod(0x64) <= lbl_1_bss_14B60[arg0].prob6A && lbl_1_bss_14B60[arg0].cnt68 > 0) {
                if (arg0 == 0) {
                    if (lbl_1_bss_14B60[arg0].col == 0) {
                        if (frandmod(2) != 0) {
                            lbl_1_bss_15288->chars[arg0].stkX = 1;
                        } else {
                            lbl_1_bss_15288->chars[arg0].stkX = -1;
                        }
                    } else {
                        lbl_1_bss_15288->chars[arg0].stkX = lbl_1_bss_14B60[arg0].col * -1;
                    }
                } else {
                    switch (lbl_1_bss_14B60[arg0].col) {
                    case -1:
                        lbl_1_bss_15288->chars[arg0].stkX = 1;
                        break;
                    case 0:
                        lbl_1_bss_15288->chars[arg0].stkX = lbl_1_bss_14B60[0].col * -1;
                        break;
                    case 1:
                        lbl_1_bss_15288->chars[arg0].stkX = -1;
                        break;
                    }
                }
                lbl_1_bss_14B60[arg0].cnt68--;
                lbl_1_bss_14B60[arg0].state6C = 0;
                return;
            }
            lbl_1_bss_15288->chars[arg0].btnDown = 0x100;
        }
        break;
    }
}
