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
#include "REL/m545Dll.h"

double sin(double x);
double cos(double x);

/* m545.c */
extern M545Game lbl_1_bss_19C;
extern M545Plr lbl_1_bss_C[2];
extern M545Cam *lbl_1_bss_8;
void fn_1_8C4(s16 arg0);
void fn_1_106C(void);
void fn_1_1474(void);
void fn_1_17EC(void);
void fn_1_1980(M545Vec3 *arg0);
void fn_1_2368(void);
void fn_1_2398(s16 arg0, s16 arg1);
void fn_1_2460(s16 arg0);
void fn_1_24D8(s16 arg0, s16 arg1, s16 arg2);

/* absolute value of a float expression, as the original macro expands it */
#define M545_ABS(x) (((x) > 0.0f) ? (x) : -1.0f * (x))

void fn_1_40AC(M545State *state);
void fn_1_44D8(M545State *state);
s16 fn_1_499C(M545State *state, s16 arg1);
s16 fn_1_4B2C(M545State *state, s16 arg1, s16 arg2);
s16 fn_1_5794(M545State *state, s16 arg1);
s16 fn_1_5BC4(M545State *state, s16 arg1);
void fn_1_6590(M545State *state);
void fn_1_6668(M545State *state, f32 speed);
void fn_1_6BC4(M545State *state, s16 p, f32 speed);
s32 fn_1_6D40(void);
s32 fn_1_70E8(M545State *state);
void fn_1_762C(M545State *state, s16 p);

/* unit-owned .bss, declared in reverse address order so the offsets match the original */
s16 lbl_1_bss_99C[14];
struct {
    s16 id;
    s16 timer;
} lbl_1_bss_980[7];
M545Vec3 lbl_1_bss_8D8[14];

/* unit-owned .data */
s16 lbl_1_data_2D8[4][7] = {
    { 100, 80, 40, 0, 0, 0, 0 },
    { 100, 100, 80, 60, 40, 20, 0 },
    { 100, 100, 100, 80, 60, 40, 20 },
    { 100, 100, 100, 100, 80, 60, 40 },
};

#define MID(p, k) lbl_1_bss_99C[(p) * 6 + (k)]

s32 fn_1_26BC(M545State *state)
{
    s16 i;
    s16 j;
    s16 cnt;
    s16 a;
    s16 b;
    s16 t;

    memset(&lbl_1_bss_19C, 0, sizeof(M545Game));
    memset(lbl_1_bss_C, 0, sizeof(lbl_1_bss_C));
    for (i = 0; i < 12; i++) {
        lbl_1_bss_19C.fxFlag[i] = -1;
    }
    lbl_1_bss_C[0].scale.x = 1.0f;
    lbl_1_bss_C[0].scale.y = 1.0f;
    lbl_1_bss_C[0].scale.z = 1.0f;
    lbl_1_bss_C[1].scale.x = 1.0f;
    lbl_1_bss_C[1].scale.y = 1.0f;
    lbl_1_bss_C[1].scale.z = 1.0f;
    lbl_1_bss_C[0].alive = 1;
    lbl_1_bss_C[1].alive = 1;
    lbl_1_bss_C[0].mode = 1;
    lbl_1_bss_C[1].mode = 1;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            lbl_1_bss_C[i].hook[j] = CharModelItemHookGet(state->chars[i].charNo, 1, j);
        }
    }
    for (i = 0; i < 7; i++) {
        lbl_1_bss_980[i].id = -1;
        lbl_1_bss_980[i].timer = 0;
    }
    for (i = 1; i < 7; i++) {
        do {
            lbl_1_bss_980[i].id = frandmod(6);
            cnt = 0;
            for (j = 1; j < 7; j++) {
                if (lbl_1_bss_980[i].id == lbl_1_bss_980[j].id) {
                    cnt++;
                }
            }
        } while (cnt > 1);
    }
    for (i = 0; i < 7; i++) {
        lbl_1_bss_980[i].timer = frandmod(0x79) + 0x3C;
    }
    for (i = 0; i < 6; i++) {
        t = lbl_1_bss_980[i + 1].id * 2;
        lbl_1_bss_99C[i] = t;
        lbl_1_bss_99C[i + 6] = t + 1;
    }
    i = 0;
    a = 0;
    b = 12;
    for (; i < 12; i++, a++, b++) {
        s16 rot;

        state->models[a].id = Hu3DModelCreate(HuDataSelHeapReadNum(state->dataNums[i / 2], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(state->models[a].id, 1);
        Hu3DModelCameraSet(state->models[a].id, 1);
        Hu3DModelLayerSet(state->models[a].id, 1);
        rot = frandmod(0xF) - 7;
        state->models[a].rotX = 0.0f;
        state->models[a].rotY = rot;
        state->models[a].rotZ = 0.0f;
        state->models[b].id = Hu3DModelCreate(HuDataSelHeapReadNum(state->dataNums[6], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(state->models[b].id, 9);
        Hu3DModelCameraSet(state->models[b].id, 1);
        Hu3DModelLayerSet(state->models[b].id, 5);
        Hu3DModelShadowSet(state->models[b].id);
        Hu3DModelShadowDispOn(state->models[b].id);
        *(M545Vec3 *)&state->models[b].rotX = *(M545Vec3 *)&state->models[a].rotX;
        lbl_1_bss_19C.f0C[i] = 0.0f;
        Hu3DModelTPLvlSet(state->models[b].id, lbl_1_bss_19C.f0C[i]);
    }
    for (i = 0; i < 6; i++) {
        lbl_1_bss_8D8[lbl_1_bss_99C[i]].x = 0.0f;
        lbl_1_bss_8D8[lbl_1_bss_99C[i]].y = 600.0f + 100.0f * i;
        lbl_1_bss_8D8[lbl_1_bss_99C[i]].z = -70.0f;
        lbl_1_bss_8D8[lbl_1_bss_99C[i + 6]].x = 0.0f;
        lbl_1_bss_8D8[lbl_1_bss_99C[i + 6]].y = 600.0f + 100.0f * i;
        lbl_1_bss_8D8[lbl_1_bss_99C[i + 6]].z = -70.0f;
    }
    fn_1_40AC(state);
    fn_1_17EC();
    return 1;
}

s32 fn_1_2E88(M545State *state)
{
    s16 i;
    s32 ret;
    M545StageSlot *slot;

    ret = 0;
    switch (state->stepF4) {
    case 0:
        if (lbl_1_bss_19C.flag52 == 0) {
            fn_1_2368();
            lbl_1_bss_19C.flag52 = 1;
        }
        if (state->timerF0++ > 0x64) {
            state->stepF4++;
        }
        break;
    case 1:
        if (fn_1_6D40() != 0) {
            state->stepF4++;
        }
        break;
    case 2:
        for (i = 0; i < 2; i++) {
            slot = &state->slots[i];
            slot->shiftState = 1;
            slot->shiftMotion = 1;
            slot->shiftStart = 0.0f;
            slot->shiftEnd = 30.0f;
            slot->shiftAttr = 0x40000001;
        }
        state->stepF4++;
        break;
    case 3:
        ret = 1;
        break;
    }
    fn_1_40AC(state);
    return ret;
}

s32 fn_1_2FE8(M545State *state)
{
    s32 ret = 0;
    state->timerF0 = 0;
    fn_1_40AC(state);
    return ret;
}

s32 fn_1_3030(M545State *state)
{
    s32 ret;
    s32 sp8;
    s32 spC;
    s16 i;
    s16 t;
    s16 pn;

    ret = 0;
    spC = 0;
    sp8 = 0;
    switch (state->phase) {
    case 0:
        if (fn_1_4B2C(state, lbl_1_bss_C[0].count, lbl_1_bss_C[0].count) != 0) {
            state->phase = 4;
        }
        break;
    case 1:
        fn_1_44D8(state);
        fn_1_6590(state);
        t = --lbl_1_bss_980[lbl_1_bss_C[0].count].timer;
        if (t <= 0 && lbl_1_bss_C[0].flag5C == 1 && lbl_1_bss_C[0].count < 6) {
            state->phase = 2;
        }
        if (lbl_1_bss_C[0].alive == 0 || lbl_1_bss_C[1].alive == 0) {
            state->phase = 5;
            lbl_1_bss_19C.winner = -1;
            if (lbl_1_bss_C[0].alive == 1) {
                lbl_1_bss_19C.winner = 0;
            } else if (lbl_1_bss_C[1].alive == 1) {
                lbl_1_bss_19C.winner = 1;
            }
        }
        break;
    case 2:
        fn_1_6590(state);
        if (fn_1_499C(state, 0) != 0) {
            state->phase = 3;
        }
        break;
    case 3:
        if (fn_1_4B2C(state, lbl_1_bss_C[0].count, lbl_1_bss_C[0].count) != 0) {
            state->phase = 4;
        }
        fn_1_6590(state);
        break;
    case 4:
        t = lbl_1_bss_19C.timer4E;
        lbl_1_bss_19C.timer4E = t + 1;
        if (t > 0x1E) {
            lbl_1_bss_19C.timer4E = 0;
            state->phase = 1;
        }
        fn_1_6590(state);
        break;
    case 5:
        if (fn_1_499C(state, 1) != 0) {
            for (i = 0; i < 2; i++) {
                if (lbl_1_bss_C[i].alive == 0) {
                    CharFXPlayVolPan(state->chars[i].charNo, 0x202, 0x7F, (i << 5) + 0x30);
                }
            }
            state->phase = 6;
        }
        break;
    case 6:
        for (i = 0; i < 2; i++) {
            if (lbl_1_bss_C[i].alive == 0 && fn_1_5794(state, i) != 0) {
                state->phase = 7;
            }
        }
        break;
    case 7:
        if (lbl_1_bss_C[0].alive == 0 && lbl_1_bss_C[1].alive == 0) {
            MgSeqWinnerSet(-1, -1, -1, -1);
        } else if (lbl_1_bss_C[0].alive == 0) {
            MgSeqWinnerSet(state->chars[1].charNo, -1, -1, -1);
            pn = state->chars[1].playerNo;
            if (_CheckFlag(0x1000F) == 0) {
                GwPlayer[pn].mgCoinBonus = 10;
            }
        } else if (lbl_1_bss_C[1].alive == 0) {
            MgSeqWinnerSet(state->chars[0].charNo, -1, -1, -1);
            pn = state->chars[0].playerNo;
            if (_CheckFlag(0x1000F) == 0) {
                GwPlayer[pn].mgCoinBonus = 10;
            }
        }
        lbl_1_bss_19C.flag52 = 0;
        state->phase = 0;
        ret = 1;
        break;
    }
    fn_1_40AC(state);
    return ret;
}

s32 fn_1_3480(M545State *state)
{
    s32 ret = 0;
    if (lbl_1_bss_19C.flag52 == 0) {
        fn_1_2398(1, 0x64);
        lbl_1_bss_19C.flag52 = 1;
    }
    fn_1_40AC(state);
    ret = 1;
    return ret;
}

s32 fn_1_34EC(M545State *state)
{
    s32 ret = 0;
    ret = fn_1_70E8(state);
    fn_1_40AC(state);
    return ret;
}

s32 fn_1_3538(M545State *state)
{
    s32 ret = 0;
    M545StageSlot *slot;

    if (state->flagE4 == 0) {
        slot = &state->slots[lbl_1_bss_19C.winner];
        slot->motion = 4;
        slot->speed = 1.0f;
        slot->attrSet = 0;
        slot->attrReset = 0x40000001;
        slot->time = 0.0f;
        state->flagE4 = 1;
    }
    fn_1_40AC(state);
    return ret;
}

s32 fn_1_3600(M545State *state)
{
    s32 ret = 1;
    fn_1_40AC(state);
    return ret;
}

s32 fn_1_3638(void)
{
    s32 val = 1;
    return val;
}

void fn_1_3654(M545State *state)
{
    s16 i;

    for (i = 0; i < 0x26; i++) {
        Hu3DModelPosSet(state->models[i].id, state->models[i].x, state->models[i].y, state->models[i].z);
        Hu3DModelRotSet(state->models[i].id, state->models[i].rotX, state->models[i].rotY, state->models[i].rotZ);
    }
}

void fn_1_373C(M545State *state)
{
    s16 p;
    s16 k;
    HU3DMODEL *model;

    for (p = 0; p < 2; p++) {
        Hu3DModelObjMtxGet(state->models[p + 36].id, lbl_1_bss_C[p].hook[0], lbl_1_bss_C[p].mtx);
        lbl_1_bss_C[p].pos.x = lbl_1_bss_C[p].mtx[0][3];
        lbl_1_bss_C[p].pos.y = lbl_1_bss_C[p].mtx[1][3];
        lbl_1_bss_C[p].pos.z = lbl_1_bss_C[p].mtx[2][3];
    }
    for (p = 0; p < 2; p++) {
        for (k = 0; k < 6; k++) {
            model = &Hu3DData[state->models[MID(p, k)].id];
            switch (lbl_1_bss_C[p].mode) {
            case 0:
                if (k < lbl_1_bss_C[p].count) {
                    PSMTXCopy(lbl_1_bss_C[p].mtx, model->mtx);
                } else {
                    PSMTXIdentity(model->mtx);
                    state->models[MID(p, k)].x = lbl_1_bss_C[p].pos.x;
                    state->models[MID(p, k)].y = lbl_1_bss_C[p].pos.y;
                    state->models[MID(p, k)].z = lbl_1_bss_C[p].pos.z;
                    if (state->models[MID(p, k)].y <= 150.0f) {
                        state->models[MID(p, k)].y = 150.0f;
                    }
                }
                break;
            case 1:
                PSMTXIdentity(model->mtx);
                state->models[MID(p, k)].x = lbl_1_bss_C[p].pos.x;
                state->models[MID(p, k)].y = lbl_1_bss_C[p].pos.y;
                state->models[MID(p, k)].z = lbl_1_bss_C[p].pos.z;
                if (state->models[MID(p, k)].y <= 150.0f) {
                    state->models[MID(p, k)].y = 150.0f;
                }
                break;
            case 2:
                PSMTXIdentity(model->mtx);
                state->models[MID(p, k)].x = 0.0f;
                state->models[MID(p, k)].z = 0.0f;
                state->models[MID(p, k)].y = lbl_1_bss_C[p].pos.y;
                if (state->models[MID(p, k)].y <= 150.0f) {
                    state->models[MID(p, k)].y = 150.0f;
                }
                break;
            case 3:
                PSMTXIdentity(model->mtx);
                state->models[MID(p, k)].x = 0.0f;
                state->models[MID(p, k)].y = 0.0f;
                state->models[MID(p, k)].z = 0.0f;
                break;
            }
            state->models[MID(p, k)].x += lbl_1_bss_8D8[MID(p, k)].x;
            state->models[MID(p, k)].y += lbl_1_bss_8D8[MID(p, k)].y;
            state->models[MID(p, k)].z += lbl_1_bss_8D8[MID(p, k)].z;
            state->models[MID(p, k) + 12].x = state->models[MID(p, k)].x;
            state->models[MID(p, k) + 12].y = state->models[MID(p, k)].y;
            state->models[MID(p, k) + 12].z = state->models[MID(p, k)].z;
            state->models[MID(p, k) + 12].y = 101.0f;
        }
    }
}

void fn_1_40AC(M545State *state)
{
    s16 i;

    fn_1_373C(state);
    for (i = 0; i < 0x26; i++) {
        Hu3DModelPosSet(state->models[i].id, state->models[i].x, state->models[i].y, state->models[i].z);
        Hu3DModelRotSet(state->models[i].id, state->models[i].rotX, state->models[i].rotY, state->models[i].rotZ);
    }
    fn_1_8C4(0);
    fn_1_106C();
    fn_1_1474();
    lbl_1_bss_C[0].counter98++;
    lbl_1_bss_C[1].counter98++;
}

void fn_1_41D4(M545State *state, s16 p)
{
    if (state->chars[p].btnDown & 0x100) {
        lbl_1_bss_C[p].counts[0]++;
        lbl_1_bss_C[p].totals[0]++;
    }
    if (state->chars[p].btnDown & 0x200) {
        lbl_1_bss_C[p].counts[1]++;
        lbl_1_bss_C[p].totals[1]++;
    }
    if (state->chars[p].btnDown & 0x800) {
        lbl_1_bss_C[p].counts[3]++;
        lbl_1_bss_C[p].totals[3]++;
    }
    if (state->chars[p].btnDown & 0x400) {
        lbl_1_bss_C[p].counts[2]++;
        lbl_1_bss_C[p].totals[2]++;
    }
    if (state->chars[p].btnDown & 0x40) {
        lbl_1_bss_C[p].counts[4]++;
        lbl_1_bss_C[p].totals[4]++;
    }
    if (state->chars[p].btnDown & 0x20) {
        lbl_1_bss_C[p].counts[5]++;
        lbl_1_bss_C[p].totals[5]++;
    }
}

s32 fn_1_4400(s16 p)
{
    s16 k;
    s32 ret;

    ret = 1;
    for (k = 1; k <= lbl_1_bss_C[p].count; k++) {
        if (lbl_1_bss_C[p].counts[lbl_1_bss_980[k].id] < 2) {
            ret = 0;
        }
    }
    for (k = 0; k < 6; k++) {
        lbl_1_bss_C[p].counts[k] = 0;
    }
    return ret;
}

void fn_1_44D8(M545State *state)
{
    s16 p;
    s16 k;
    s16 pending;
    s16 ok;

    for (p = 0; p < 2; p++) {
        if (state->chars[p].cpuLevel == 0) {
            fn_1_41D4(state, p);
        }
        pending = 0;
        for (k = 1; k <= lbl_1_bss_C[p].count; k++) {
            if (lbl_1_bss_C[p].counts[lbl_1_bss_980[k].id] < 2) {
                pending = 1;
            }
        }
        if (pending == 1) {
            omVibrate(state->chars[p].playerNo, 1, 0xC, 0);
        }
        if (++lbl_1_bss_C[p].timer9C >= 0x3C) {
            if (state->chars[p].cpuLevel != 0) {
                fn_1_762C(state, p);
            }
            ok = 1;
            for (k = 1; k <= lbl_1_bss_C[p].count; k++) {
                if (lbl_1_bss_C[p].counts[lbl_1_bss_980[k].id] < 2) {
                    ok = 0;
                }
            }
            for (k = 0; k < 6; k++) {
                lbl_1_bss_C[p].counts[k] = 0;
            }
            lbl_1_bss_C[p].alive = ok;
            lbl_1_bss_C[p].flag5C = 1;
            lbl_1_bss_C[p].timer9C = 0;
            lbl_1_bss_C[p].unkA0 = 0;
        } else {
            lbl_1_bss_C[p].flag5C = 0;
        }
    }
}

s16 fn_1_499C(M545State *state, s16 arg1)
{
    f32 sp8;
    f32 spC;
    s16 t;
    s16 ret;
    s16 i;

    ret = 0;
    if (state->slots[0].motion != 2) {
        lbl_1_bss_19C.timer50 = 0;
        ret = 1;
    } else {
        if (arg1 == 0) {
            sp8 = 13.0f;
            spC = 13.9f;
        } else if (arg1 == 1) {
            sp8 = 1.0f;
            spC = 1.9f;
        }
        if ((state->slots[0].time >= sp8 && state->slots[0].time <= spC)
            || (t = lbl_1_bss_19C.timer50, lbl_1_bss_19C.timer50 = t + 1, t > 0x258)) {
            lbl_1_bss_19C.timer50 = 0;
            ret = 1;
        }
    }
    if (arg1 == 1 && ret == 1) {
        for (i = 0; i < 2; i++) {
            if (lbl_1_bss_C[i].alive == 0) {
                omVibrate(state->chars[i].playerNo, 0x40, 0xC, 0);
            }
        }
    }
    return ret;
}

s16 fn_1_4B2C(M545State *state, s16 arg1, s16 arg2)
{
    M545Vec3 sp;
    f32 tgt;
    f32 base;
    f32 d1;
    f32 d2;
    s16 ret;

    ret = 0;
    switch (state->stepE8) {
    case 0:
        Hu3DModelAttrReset(state->models[lbl_1_bss_99C[arg1]].id, 1);
        Hu3DModelAttrReset(state->models[lbl_1_bss_99C[arg1 + 6]].id, 1);
        Hu3DModelAttrReset(state->models[lbl_1_bss_99C[arg1] + 12].id, 1);
        Hu3DModelAttrReset(state->models[lbl_1_bss_99C[arg1 + 6] + 12].id, 1);
        lbl_1_bss_19C.fallSpeed = -8.259f;
        fn_1_2460(0);
        state->stepE8++;
        break;
    case 1:
        tgt = 600.0f + 100.0f * arg2;
        base = 100.0f * arg2;
        lbl_1_bss_19C.fallSpeed -= 0.753f;
        lbl_1_bss_8D8[lbl_1_bss_99C[arg1]].y += lbl_1_bss_19C.fallSpeed;
        lbl_1_bss_8D8[lbl_1_bss_99C[arg1 + 6]].y += lbl_1_bss_19C.fallSpeed;
        d1 = tgt - base;
        d1 = M545_ABS(d1);
        d2 = tgt - lbl_1_bss_8D8[lbl_1_bss_99C[arg1]].y;
        d2 = M545_ABS(d2);
        if (d1 <= d2) {
            lbl_1_bss_8D8[lbl_1_bss_99C[arg1]].y = base;
            lbl_1_bss_8D8[lbl_1_bss_99C[arg1 + 6]].y = base;
            state->stepE8++;
            lbl_1_bss_C[0].count++;
            lbl_1_bss_C[1].count++;
        }
        lbl_1_bss_8->center.y += (430.0f - 250.0f) / (6.0f * 30.0f);
        lbl_1_bss_8->zoom += (1400.0f - 800.0f) / (6.0f * 30.0f);
        state->models[lbl_1_bss_99C[arg1] + 12].x = state->models[lbl_1_bss_99C[arg1]].x;
        state->models[lbl_1_bss_99C[arg1] + 12].y = state->models[lbl_1_bss_99C[arg1]].y;
        state->models[lbl_1_bss_99C[arg1] + 12].z = state->models[lbl_1_bss_99C[arg1]].z;
        state->models[lbl_1_bss_99C[arg1 + 6] + 12].x = state->models[lbl_1_bss_99C[arg1 + 6]].x;
        state->models[lbl_1_bss_99C[arg1 + 6] + 12].y = state->models[lbl_1_bss_99C[arg1 + 6]].y;
        state->models[lbl_1_bss_99C[arg1 + 6] + 12].z = state->models[lbl_1_bss_99C[arg1 + 6]].z;
        state->models[lbl_1_bss_99C[arg1] + 12].y = 101.0f;
        state->models[lbl_1_bss_99C[arg1 + 6] + 12].y = 101.0f;
        lbl_1_bss_19C.f0C[lbl_1_bss_99C[arg1]] += 0.034f;
        lbl_1_bss_19C.f0C[lbl_1_bss_99C[arg1 + 6]] += 0.034f;
        if (lbl_1_bss_19C.f0C[lbl_1_bss_99C[arg1]] >= 1.0f) {
            lbl_1_bss_19C.f0C[lbl_1_bss_99C[arg1]] = 1.0f;
            lbl_1_bss_19C.f0C[lbl_1_bss_99C[arg1 + 6]] = 1.0f;
        }
        Hu3DModelTPLvlSet(state->models[lbl_1_bss_99C[arg1] + 12].id, lbl_1_bss_19C.f0C[lbl_1_bss_99C[arg1]]);
        Hu3DModelTPLvlSet(state->models[lbl_1_bss_99C[arg1 + 6] + 12].id, lbl_1_bss_19C.f0C[lbl_1_bss_99C[arg1 + 6]]);
        if (state->stepE8 == 2) {
            omVibrate(state->chars[0].playerNo, 0xC, 0xC, 0);
            omVibrate(state->chars[1].playerNo, 0xC, 0xC, 0);
            if (arg2 != 0) {
                sp.x = state->models[36].x;
                sp.y = state->models[36].y;
                sp.z = state->models[36].z;
                sp.y = lbl_1_bss_C[0].pos.y - 60.0f + (f32)(arg2 * 100);
                sp.z = lbl_1_bss_C[0].pos.z - 110.0f;
                fn_1_1980(&sp);
                sp.x += -50.0f;
                fn_1_1980(&sp);
                sp.x += 100.0f;
                fn_1_1980(&sp);
                sp.x = state->models[37].x;
                sp.y = state->models[37].y;
                sp.z = state->models[37].z;
                sp.y = lbl_1_bss_C[1].pos.y - 60.0f + (f32)(arg2 * 100);
                sp.z = lbl_1_bss_C[1].pos.z - 110.0f;
                fn_1_1980(&sp);
                sp.x += -50.0f;
                fn_1_1980(&sp);
                sp.x += 100.0f;
                fn_1_1980(&sp);
                fn_1_2460(arg2 + 1);
            } else {
                fn_1_2460(1);
            }
            state->slots[0].speed = 0.0f;
            state->slots[1].speed = 0.0f;
            lbl_1_bss_C[0].mode = 1;
            lbl_1_bss_C[1].mode = 1;
        }
        break;
    case 2:
        lbl_1_bss_C[0].scale.y -= 0.016f;
        lbl_1_bss_C[1].scale.y -= 0.016f;
        if (lbl_1_bss_C[0].scale.y <= 0.92f) {
            state->stepE8++;
        }
        Hu3DModelScaleSetV(state->models[36].id, (HuVecF *)&lbl_1_bss_C[0].scale);
        Hu3DModelScaleSetV(state->models[37].id, (HuVecF *)&lbl_1_bss_C[1].scale);
        break;
    case 3:
        lbl_1_bss_C[0].scale.y += 0.016f;
        lbl_1_bss_C[1].scale.y += 0.016f;
        if (lbl_1_bss_C[0].scale.y >= 1.0f) {
            lbl_1_bss_19C.fallSpeed = 0.0f;
            state->stepE8 = 0;
            state->slots[0].speed = 1.0f;
            state->slots[1].speed = 1.0f;
            lbl_1_bss_C[0].mode = 1;
            lbl_1_bss_C[1].mode = 1;
            ret = 1;
        }
        Hu3DModelScaleSetV(state->models[36].id, (HuVecF *)&lbl_1_bss_C[0].scale);
        Hu3DModelScaleSetV(state->models[37].id, (HuVecF *)&lbl_1_bss_C[1].scale);
        break;
    }
    return ret;
}

s16 fn_1_5794(M545State *state, s16 arg1)
{
    M545Vec3 sp;
    s16 ret;
    s16 k;

    ret = 0;
    lbl_1_bss_C[arg1].mode = 1;
    lbl_1_bss_C[arg1].scale.y -= 1.0f / 10.0f;
    if (lbl_1_bss_C[arg1].scale.y <= 0.1f) {
        lbl_1_bss_C[arg1].scale.x = 0.1f;
        lbl_1_bss_C[arg1].scale.y = 0.1f;
        lbl_1_bss_C[arg1].scale.z = 0.1f;
        ret = 1;
    }
    Hu3DModelScaleSetV(state->models[arg1 + 36].id, (HuVecF *)&lbl_1_bss_C[arg1].scale);
    if (ret == 1) {
        sp.x = state->models[arg1 + 36].x;
        sp.y = state->models[arg1 + 36].y;
        sp.z = state->models[arg1 + 36].z;
        sp.z = lbl_1_bss_C[arg1].pos.z - 40.0f;
        fn_1_1980(&sp);
        sp.x += -50.0f;
        fn_1_1980(&sp);
        sp.x += 100.0f;
        fn_1_1980(&sp);
        lbl_1_bss_C[arg1].mode = 3;
        fn_1_24D8(9, 0x7F, (arg1 << 5) + 0x30);
        for (k = 0; k < lbl_1_bss_C[arg1].count; k++) {
            lbl_1_bss_8D8[MID(arg1, k)].x = lbl_1_bss_C[arg1].pos.x + lbl_1_bss_8D8[MID(arg1, k)].x;
            lbl_1_bss_8D8[MID(arg1, k)].y = 150.0f + 100.0f * k;
            lbl_1_bss_8D8[MID(arg1, k)].z = lbl_1_bss_C[arg1].pos.z + lbl_1_bss_8D8[MID(arg1, k)].z;
        }
    }
    return ret;
}

s16 fn_1_5BC4(M545State *state, s16 arg1)
{
    f32 sp8;
    f32 tgt;
    f32 base;
    f32 d1;
    f32 d2;
    s16 ret;
    s16 k;
    s16 m;
    M545StageSlot *slot;

    ret = 0;
    sp8 = 50.0f;
    switch (state->stepEC) {
    case 2:
        break;
    case 0:
        lbl_1_bss_19C.fallSpeed = 35.0f;
        slot = &state->slots[arg1];
        slot->shiftState = 1;
        slot->shiftMotion = 3;
        slot->shiftStart = 0.0f;
        slot->shiftEnd = 10.0f;
        slot->shiftAttr = 0;
        slot->attrReset = 0x40000001;
        lbl_1_bss_C[arg1].mode = 2;
        for (k = 0; k < lbl_1_bss_C[arg1].count; k++) {
            lbl_1_bss_8D8[MID(arg1, k)].x = lbl_1_bss_C[arg1].pos.x + lbl_1_bss_8D8[MID(arg1, k)].x;
            lbl_1_bss_8D8[MID(arg1, k)].y += 10.0f;
            lbl_1_bss_8D8[MID(arg1, k)].z = lbl_1_bss_C[arg1].pos.z + lbl_1_bss_8D8[MID(arg1, k)].z;
        }
        fn_1_2460(arg1 + 10);
        state->stepEC++;
        break;
    case 1:
        slot = &state->slots[arg1];
        if (slot->shiftState == 0) {
            slot->motion = 3;
            slot->id = 3;
            slot->speed = 0.0f;
            lbl_1_bss_C[arg1].mode = 3;
            for (k = 0; k < lbl_1_bss_C[arg1].count; k++) {
                lbl_1_bss_8D8[MID(arg1, k)].y = lbl_1_bss_C[arg1].pos.y + lbl_1_bss_8D8[MID(arg1, k)].y;
            }
            state->stepEC++;
        }
        break;
    case 3:
        slot = &state->slots[arg1];
        slot->speed = 1.0f;
        slot->shiftState = 1;
        slot->shiftMotion = 0;
        slot->shiftStart = 0.0f;
        slot->shiftEnd = 10.0f;
        slot->shiftAttr = 0;
        slot->attrReset = 0x40000001;
        state->stepEC++;
        break;
    case 4:
        if (state->slots[arg1].shiftState == 0) {
            slot = &state->slots[arg1];
            slot->motion = 0;
            slot->attrSet = 0x40000001;
            ret = 1;
        }
        break;
    }
    if (state->stepEC == 2) {
        for (k = 0; k < lbl_1_bss_C[arg1].count; k++) {
            m = MID(arg1, k);
            lbl_1_bss_8D8[m].y += lbl_1_bss_19C.fallSpeed;
            tgt = 250.0f + (f32)(k * 100);
            base = 1300.0f + (f32)(k * 100);
            if (k != 0) {
                if (k % 2 != 0) {
                    lbl_1_bss_8D8[m].x += 4.0f + 3.0f * (f32)(k / 2);
                } else {
                    lbl_1_bss_8D8[m].x -= 4.0f + 3.0f * (f32)(k / 2);
                }
            }
            d1 = tgt - base;
            d1 = M545_ABS(d1);
            d2 = tgt - lbl_1_bss_8D8[m].y;
            d2 = M545_ABS(d2);
            if (d1 <= d2) {
                lbl_1_bss_8D8[m].y = base;
                state->stepEC = 3;
            }
            lbl_1_bss_19C.f0C[m] -= 0.034f;
            if (lbl_1_bss_19C.f0C[m] <= 0.0f) {
                lbl_1_bss_19C.f0C[m] = 0.0f;
            }
            Hu3DModelTPLvlSet(state->models[m + 12].id, lbl_1_bss_19C.f0C[m]);
        }
        lbl_1_bss_19C.fallSpeed -= 0.4f;
    }
    return ret;
}

void fn_1_6590(M545State *state)
{
    switch (lbl_1_bss_C[0].count) {
    case 2:
        fn_1_6668(state, 0.7f);
        return;
    case 3:
        fn_1_6668(state, 0.8f);
        return;
    case 4:
        fn_1_6668(state, 0.9f);
        return;
    case 5:
        fn_1_6668(state, 1.0f);
        return;
    case 6:
        fn_1_6668(state, 1.1f);
        return;
    }
}

void fn_1_6668(M545State *state, f32 speed)
{
    s16 p;
    s16 k;
    M545StageSlot *slot;

    switch (lbl_1_bss_19C.stage2) {
    case 0:
        for (p = 0; p < 2; p++) {
            slot = &state->slots[p];
            slot->shiftState = 1;
            slot->shiftMotion = 2;
            slot->shiftStart = 0.0f;
            slot->shiftEnd = 14.0f;
            slot->shiftAttr = 0;
            slot->attrReset = 0x40000001;
            lbl_1_bss_C[p].mode = 1;
            for (k = 0; k < 6; k++) {
                lbl_1_bss_C[p].angle[k] = 30.0f * k + 180.0f * p;
            }
        }
        lbl_1_bss_19C.stage2++;
        return;
    case 1:
        for (p = 0; p < 2; p++) {
            if (state->slots[p].shiftState == 0) {
                slot = &state->slots[p];
                slot->motion = 2;
                slot->speed = speed;
                slot->time = 7.0f * p;
                slot->attrSet = 0x40000001;
                lbl_1_bss_19C.stage2 = 2;
            }
            lbl_1_bss_C[p].mode = 1;
        }
        return;
    case 2:
        for (p = 0; p < 2; p++) {
            slot = &state->slots[p];
            slot->speed = speed;
            lbl_1_bss_C[p].mode = 1;
            fn_1_6BC4(state, p, slot->speed);
        }
        return;
    }
}

void fn_1_695C(M545State *state, s16 arg1, f32 ang)
{
    M545Vec3 sp;
    f32 a;
    s16 k;
    M545StageModel *m;

    a = ang + 90.0f;
    for (k = 0; k < lbl_1_bss_C[arg1].count; k++) {
        m = &state->models[MID(arg1, k)];
        sp.x = m->x;
        sp.y = m->y;
        sp.z = m->z;
        sp.x = 100.0 * (k * cos(1.5707963267948966));
        sp.y = 100.0 * (k * sin(1.5707963267948966));
        m->x = 100.0 * (k * cos(3.141592653589793 * a / 180.0));
        m->y = 100.0 * (k * sin(3.141592653589793 * a / 180.0));
        PSVECSubtract((Vec *)&m->x, (Vec *)&sp, (Vec *)&sp);
    }
}

void fn_1_6BC4(M545State *state, s16 p, f32 speed)
{
    f32 sp8;
    s16 k;

    sp8 = speed;
    for (k = 0; k < lbl_1_bss_C[p].count; k++) {
        lbl_1_bss_8D8[MID(p, k)].x = 1.5 * ((f32)k * sin(3.141592653589793 * lbl_1_bss_C[p].angle[k] / 180.0));
    }
    for (k = 0; k < 6; k++) {
        lbl_1_bss_C[p].angle[k] += 20.0f * sp8;
    }
}

s32 fn_1_6D40(void)
{
    f32 d1;
    f32 d2;
    s32 ret;

    ret = 0;
    lbl_1_bss_8->center.y += (250.0f - 430.0f) / 100.0f;
    d1 = 430.0f - 250.0f;
    d1 = M545_ABS(d1);
    d2 = 430.0f - lbl_1_bss_8->center.y;
    d2 = M545_ABS(d2);
    if (d1 <= d2) {
        lbl_1_bss_8->center.y = 250.0f;
    }
    lbl_1_bss_8->zoom += (800.0f - 1400.0f) / 100.0f;
    d1 = 1400.0f - 800.0f;
    d1 = M545_ABS(d1);
    d2 = 1400.0f - lbl_1_bss_8->zoom;
    d2 = M545_ABS(d2);
    if (d1 <= d2) {
        lbl_1_bss_8->zoom = 800.0f;
    }
    lbl_1_bss_8->rotX += (0.0f - -20.0f) / 100.0f;
    d1 = -20.0f - 0.0f;
    d1 = M545_ABS(d1);
    d2 = -20.0f - lbl_1_bss_8->rotX;
    d2 = M545_ABS(d2);
    if (d1 <= d2) {
        lbl_1_bss_8->rotX = 0.0f;
        ret = 1;
    }
    return ret;
}

s32 fn_1_70E8(M545State *state)
{
    f32 tx;
    f32 d1;
    f32 d2;
    s16 t;
    s16 lim;
    s32 ret;

    ret = 0;
    switch (state->phase) {
    case 0:
        if (lbl_1_bss_19C.winner == -1) {
            lim = 0xB4;
        } else {
            lim = 0x3C;
        }
        t = lbl_1_bss_19C.timer6;
        lbl_1_bss_19C.timer6 = t + 1;
        if (t > lim) {
            state->phase++;
        }
        break;
    case 1:
        if (lbl_1_bss_19C.winner == -1) {
            state->phase = 0x3E7;
        } else if (fn_1_5BC4(state, lbl_1_bss_19C.winner) != 0) {
            state->phase++;
            lbl_1_bss_19C.camY = lbl_1_bss_8->center.y;
            lbl_1_bss_19C.camZoom = lbl_1_bss_8->zoom;
        }
        break;
    case 2:
        lbl_1_bss_8->center.y += (250.0f - lbl_1_bss_19C.camY) / 30.0f;
        d1 = lbl_1_bss_19C.camY - 250.0f;
        d1 = M545_ABS(d1);
        d2 = lbl_1_bss_19C.camY - lbl_1_bss_8->center.y;
        d2 = M545_ABS(d2);
        if (d1 <= d2) {
            lbl_1_bss_8->center.y = 250.0f;
        }
        tx = -150.0f + 300.0f * lbl_1_bss_19C.winner;
        lbl_1_bss_8->center.x += (tx - 0.0f) / 30.0f;
        d1 = 0.0f - tx;
        d1 = M545_ABS(d1);
        d2 = 0.0f - lbl_1_bss_8->center.x;
        d2 = M545_ABS(d2);
        if (d1 <= d2) {
            lbl_1_bss_8->center.x = tx;
            state->phase++;
            ret = 1;
        }
        lbl_1_bss_8->zoom += (550.0f - lbl_1_bss_19C.camZoom) / 30.0f;
        d1 = lbl_1_bss_19C.camZoom - 550.0f;
        d1 = M545_ABS(d1);
        d2 = lbl_1_bss_19C.camZoom - lbl_1_bss_8->zoom;
        d2 = M545_ABS(d2);
        if (d1 <= d2) {
            lbl_1_bss_8->zoom = 550.0f;
        }
        break;
    case 3:
        state->phase = 0x3E7;
        state->flagE4 = 0;
        ret = 1;
        break;
    default:
        ret = 1;
        break;
    }
    return ret;
}

void fn_1_762C(M545State *state, s16 p)
{
    s16 lvl;
    s16 r;
    s16 cnt;
    s16 k;

    lvl = state->chars[p].cpuLevel - 1;
    r = frandmod(0x65);
    cnt = lbl_1_bss_C[p].count;
    if (cnt >= 6) {
        lbl_1_bss_C[p].unk6++;
    }
    if (lvl == 0 && cnt >= 3 && lbl_1_bss_980[cnt].timer < 0x3C) {
        r = 0x65;
    }
    if (lbl_1_data_2D8[lvl][lbl_1_bss_C[p].count - 1] >= r && lbl_1_bss_C[p].unk6 < 0x14) {
        for (k = 0; k <= 5; k++) {
            lbl_1_bss_C[p].counts[k] = 3;
        }
        return;
    }
    for (k = 0; k <= 5; k++) {
        lbl_1_bss_C[p].counts[k] = 0;
    }
}
