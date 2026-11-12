// m554dll/game.c - decompilation of the m554dll/REL/m554dll/game unit.

#define _MATH_H /* keeps MWCC from adding its 16 bytes of sqrt literals to .rodata */
#include "types.h"
#include "string.h"
#include "game/audio.h"
#include "game/charman.h"
#include "game/flag.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/wipe.h"
#include "game/mg/seqman.h"
#include "REL/m554dll/m554types.h"

double sin(double x);
double cos(double x);
s32 rand8(void);
float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

void fn_1_3210(s32 arg0);
void fn_1_3518(void);
void fn_1_3578(s32 arg0);
s32 fn_1_38D4(s32 arg0);
void fn_1_3AC8(s32 arg0);
s32 fn_1_3B58(s32 arg0);
void fn_1_3D6C(s32 arg0);
void fn_1_4050(s32 arg0);
void fn_1_4D60(s32 arg0);
s32 fn_1_514C(s32 arg0);
void fn_1_5A5C(void);
s32 fn_1_5AB0(void);
void fn_1_5CCC(s32 arg0);

extern void fn_1_78A8(void);
extern void fn_1_7F84(void);
extern void fn_1_A34(void);
extern s32 fn_1_A54(void);

/* ---- data ---- */
s32 lbl_1_data_238 = -1;
s32 lbl_1_data_23C[2] = { -1, -1 };
s16 lbl_1_data_244 = 0x7f;
s16 lbl_1_data_246[3] = { 0, 0, 0 };
s32 lbl_1_data_24C[2] = { 0, 0 };
s32 lbl_1_data_254 = -1;

/* ---- bss (declared in reverse: MWCC lays bss out in reverse order) ---- */
s32 lbl_1_bss_1358[2];
s32 lbl_1_bss_1354;
s32 lbl_1_bss_1350;
s32 lbl_1_bss_134C;
s32 lbl_1_bss_1348;
s32 lbl_1_bss_1340[2];
s16 lbl_1_bss_133C;
u8 lbl_1_bss_1330[0xc];

void fn_1_1F00(void)
{
    s32 i;

    fn_1_3210(0);
    fn_1_5A5C();
    if (lbl_1_data_23C[0] == -1) {
        lbl_1_data_23C[0] = HuAudFXPlay(0x890);
        lbl_1_data_23C[1] = HuAudFXPlay(0x892);
        HuAudFXVolSet(lbl_1_data_23C[1], lbl_1_data_244);
    }
    if (WipeCheckEnd() == 0) {
        lbl_1_data_238 = HuAudBGMPlay(0x36);
        lbl_1_bss_B1C.mode++;
        lbl_1_bss_B1C.f_104 = 0;
        omCameraViewMoveSimple(&lbl_1_bss_98[0], 0x3c);
        for (i = 0; i < 2; i++) {
            if (lbl_1_bss_B1C.player[i].dif != 0) {
                fn_1_3AC8(i);
            }
        }
        for (i = 0; i < 5; i++) {
            lbl_1_bss_B1C.hist[0][(i + 1) * 10] = 1;
            lbl_1_bss_B1C.hist[1][(i + 1) * 10] = 1;
        }
        lbl_1_bss_134C = lbl_1_bss_1358[0] = lbl_1_bss_1354 = lbl_1_bss_1350 = 0;
        lbl_1_bss_1348 = 0;
        lbl_1_bss_1340[0] = rand8() % 20 + 0x14;
        lbl_1_bss_1340[1] = rand8() % 20 + 0x14;
    }
}

void fn_1_2118(void)
{
    M554ENTRY *e;

    e = &lbl_1_bss_B1C.ents[2];
    e->rot.y -= 5.0f;
    if (e->rot.y <= -360.0f) {
        e->rot.y = 0.0f;
        lbl_1_bss_B1C.slot[0].shiftState = 1;
        lbl_1_bss_B1C.slot[0].shiftMot = 2;
        lbl_1_bss_B1C.slot[0].shiftStart = 0.0f;
        lbl_1_bss_B1C.slot[0].shiftEnd = 20.0f;
        lbl_1_bss_B1C.slot[0].shiftAttr = 0;
        lbl_1_bss_B1C.slot[1].shiftState = 1;
        lbl_1_bss_B1C.slot[1].shiftMot = 2;
        lbl_1_bss_B1C.slot[1].shiftStart = 0.0f;
        lbl_1_bss_B1C.slot[1].shiftEnd = 20.0f;
        lbl_1_bss_B1C.slot[1].shiftAttr = 0;
        lbl_1_bss_B1C.mode++;
        omVibrate(lbl_1_bss_B1C.player[0].playerNo, 0xf, 0xc, 0);
        omVibrate(lbl_1_bss_B1C.player[1].playerNo, 0xf, 0xc, 0);
        CharFXPlayPan(lbl_1_bss_B1C.player[0].charNo, 0x1fc, 0x30);
        CharFXPlayPan(lbl_1_bss_B1C.player[1].charNo, 0x1fc, 0x50);
    }
}

void fn_1_22CC(void)
{
    if (lbl_1_bss_B1C.slot[0].shiftState == 0 && lbl_1_bss_B1C.slot[0].maxTime == lbl_1_bss_B1C.slot[0].time) {
        lbl_1_bss_B1C.slot[0].shiftState = 1;
        lbl_1_bss_B1C.slot[0].shiftMot = 1;
        lbl_1_bss_B1C.slot[0].shiftStart = 0.0f;
        lbl_1_bss_B1C.slot[0].shiftEnd = 30.0f;
        lbl_1_bss_B1C.slot[0].shiftAttr = 0x40000001;
        lbl_1_bss_B1C.slot[1].shiftState = 1;
        lbl_1_bss_B1C.slot[1].shiftMot = 1;
        lbl_1_bss_B1C.slot[1].shiftStart = 0.0f;
        lbl_1_bss_B1C.slot[1].shiftEnd = 30.0f;
        lbl_1_bss_B1C.slot[1].shiftAttr = 0x40000001;
        omCameraViewMoveSimple(&lbl_1_bss_98[1], 0x78);
        lbl_1_bss_134C = 1;
        MgSeqModeNext();
    }
}

void fn_1_240C(void)
{
    s32 i;

    fn_1_3518();
    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_B1C.player[i].dif != 0) {
            fn_1_3D6C(i);
        }
        fn_1_3578(i);
    }
    fn_1_3210(0);
    if (lbl_1_bss_B1C.f_134[0][0] < 280.0f || lbl_1_bss_B1C.f_134[1][0] < 280.0f) {
        lbl_1_bss_B1C.mode++;
        lbl_1_bss_B1C.f_104 = 0;
        lbl_1_bss_B1C.f_108 = 0;
        lbl_1_bss_B1C.winner = 0;
        if (lbl_1_bss_B1C.f_134[0][0] < lbl_1_bss_B1C.f_134[1][0]) {
            lbl_1_bss_B1C.winner = 1;
        }
        if (lbl_1_bss_B1C.f_134[0][0] < 280.0f && lbl_1_bss_B1C.f_134[1][0] < 280.0f) {
            lbl_1_bss_B1C.winner = 2;
            lbl_1_bss_134C = 3;
        }
        if (lbl_1_bss_B1C.winner == 2) {
            omVibrate(lbl_1_bss_B1C.player[0].playerNo, 0x3c, 0xc, 0);
            omVibrate(lbl_1_bss_B1C.player[1].playerNo, 0x3c, 0xc, 0);
        } else {
            omVibrate(lbl_1_bss_B1C.player[lbl_1_bss_B1C.winner ^ 1].playerNo, 0x3c, 0xc, 0);
        }
        for (i = 0; i < 4; i++) {
            lbl_1_bss_B1C.count[i] = 0;
        }
        fn_1_7F84();
        HuAudFXPlay(0x88f);
    }
    fn_1_5CCC(0);
}

void fn_1_2654(void)
{
    s32 i;
    s32 done;

    i = 0;
    done = 0;
    for (; i < 2; i++) {
        if (lbl_1_bss_B1C.winner == 2 || (lbl_1_bss_B1C.winner ^ 1) == i) {
            fn_1_4050(i);
        } else {
            fn_1_4D60(i);
        }
        if (lbl_1_bss_B1C.count[i] >= 0x78 && done == 0) {
            done++;
        }
    }
    if (lbl_1_bss_B1C.winner == 2 && lbl_1_bss_B1C.count[0] >= 0x14) {
        lbl_1_bss_134C = 3;
        MgSeqModeNext();
        HuAudSStreamFadeOut(lbl_1_data_238, 0x64);
        lbl_1_bss_B1C.mode = 0;
        lbl_1_bss_B1C.f_104 = 0;
    }
    if (done != 0) {
        if (lbl_1_bss_B1C.winner != 2) {
            lbl_1_bss_134C = 2;
        } else {
            lbl_1_bss_134C = 3;
        }
        MgSeqModeNext();
        HuAudSStreamFadeOut(lbl_1_data_238, 0x64);
        lbl_1_bss_B1C.mode = 0;
        lbl_1_bss_B1C.f_104 = 0;
        lbl_1_bss_B1C.count[0] = 0;
    }
    fn_1_5CCC(0);
}

void fn_1_2800(void)
{
}

void fn_1_2804(void)
{
    s32 i;

    if (lbl_1_bss_B1C.winner == 2) {
        for (i = 0; i < 2; i++) {
            fn_1_4050(i);
        }
        lbl_1_bss_B1C.mode = 0;
    }
    fn_1_5CCC(0);
}

void fn_1_2870(void)
{
    Mtx mtx;
    HuVecF pos;
    HuVecF rot;
    M554ENTRY *hook;
    M554ENTRY *tgt;
    s32 pad;

    if (lbl_1_bss_B1C.winner == 2) {
        lbl_1_bss_133C = GameMesCreate(2, 2);
        lbl_1_bss_134C = 3;
        lbl_1_bss_B1C.count[0] = 0;
        HuAudJinglePlay(0x7b);
    } else {
        hook = &lbl_1_bss_B1C.ents[lbl_1_bss_B1C.winner + 0x4e];
        tgt = &lbl_1_bss_B1C.ents[lbl_1_bss_B1C.winner + 3];
        pad = lbl_1_bss_B1C.player[lbl_1_bss_B1C.winner].playerNo;
        GWMgCoinBonusSet(pad, 10);
        lbl_1_bss_133C = GameMesCreate(4, 3, lbl_1_bss_B1C.player[lbl_1_bss_B1C.winner].charNo, -1, -1, -1);
        HuAudJinglePlay(0x79);
        lbl_1_bss_98[2].center.x = hook->pos.x;
        lbl_1_bss_98[2].center.y = 150.0f + hook->pos.y;
        lbl_1_bss_98[2].center.z = 350.0f + hook->pos.z;
        lbl_1_bss_98[2].rot.x = -32.0f;
        lbl_1_bss_98[2].rot.y = 0.0f;
        lbl_1_bss_98[2].rot.z = 0.0f;
        lbl_1_bss_98[2].zoom = 495.0f;
        omCameraViewMoveSimple(&lbl_1_bss_98[2], 0x3c);
        lbl_1_bss_134C = 0;
        lbl_1_bss_B1C.slot[lbl_1_bss_B1C.winner].shiftState = 1;
        lbl_1_bss_B1C.slot[lbl_1_bss_B1C.winner].shiftMot = 3;
        lbl_1_bss_B1C.slot[lbl_1_bss_B1C.winner].shiftStart = 0.0f;
        lbl_1_bss_B1C.slot[lbl_1_bss_B1C.winner].shiftEnd = 45.0f;
        lbl_1_bss_B1C.slot[lbl_1_bss_B1C.winner].shiftAttr = 0;
        Hu3DModelObjMtxGet(hook->model, CharModelItemHookGet(lbl_1_bss_B1C.player[lbl_1_bss_B1C.winner].charNo, 1, 0), mtx);
        Hu3DModelHookReset(hook->model);
        Hu3DMtxRotGet(mtx, &rot);
        pos.x = mtx[0][3];
        pos.y = mtx[1][3];
        pos.z = mtx[2][3];
        Hu3DModelPosSetV(tgt->model, &pos);
        Hu3DModelRotSetV(tgt->model, &rot);
        tgt->pos.x = pos.x;
        tgt->pos.y = pos.y;
        tgt->pos.z = pos.z;
        tgt->rot.x = rot.x;
        tgt->rot.y = rot.y;
        tgt->rot.z = rot.z;
        fn_1_78A8();
        lbl_1_bss_B1C.count[0] = 0;
        lbl_1_data_246[0] = lbl_1_data_246[1];
    }
    lbl_1_bss_B1C.mode++;
    fn_1_5CCC(1);
}

void fn_1_2C74(void)
{
    M554ENTRY *e;

    lbl_1_bss_B1C.count[0]++;
    if (lbl_1_bss_B1C.winner != 2) {
        e = &lbl_1_bss_B1C.ents[lbl_1_bss_B1C.winner + 0x4e];
        if (lbl_1_bss_B1C.winner != 0) {
            e->rot.y += 8.0f;
            if (e->rot.y > 360.0f) {
                e->rot.y = 360.0f;
            }
        } else {
            e->rot.y -= 8.0f;
            if (e->rot.y < 0.0f) {
                e->rot.y = 0.0f;
            }
        }
        e = &lbl_1_bss_B1C.ents[lbl_1_bss_B1C.winner + 3];
        if (lbl_1_bss_B1C.count[0] <= 0x5a) {
            e->pos.y = e->pos.y + 8.0 * cos(3.141592653589793 * (f64)((lbl_1_bss_B1C.count[0] * 0x5a) / 90) / 180.0);
            e->pos.z = e->pos.z - 8.0 * sin(3.141592653589793 * (f64)((lbl_1_bss_B1C.count[0] * 0x5a) / 90) / 180.0);
            e->rot.x += 10.0f;
            Hu3DModelScaleSet(e->model, (90.0f - (f32)lbl_1_bss_B1C.count[0]) / 90.0f, (90.0f - (f32)lbl_1_bss_B1C.count[0]) / 90.0f, (90.0f - (f32)lbl_1_bss_B1C.count[0]) / 90.0f);
        }
        e = &lbl_1_bss_B1C.ents[lbl_1_bss_B1C.winner + 5];
        if (lbl_1_bss_B1C.winner != 0) {
            e->rot.y -= 0.6428571428571429;
            if (e->rot.y < 0.0f) {
                e->rot.y = 0.0f;
            }
        } else {
            e->rot.y += 0.6428571428571429;
            if (e->rot.y > 0.0f) {
                e->rot.y = 0.0f;
            }
        }
        if (lbl_1_bss_B1C.count[0] % 3 == 0) {
            lbl_1_data_244 -= 1;
            if (lbl_1_data_244 >= 0x40) {
                HuAudFXVolSet(lbl_1_data_23C[1], lbl_1_data_244);
            }
        }
    } else if (lbl_1_bss_B1C.count[0] == 0xb4) {
        HuAudFXFadeOut(lbl_1_data_23C[0], 0x14);
        HuAudFXFadeOut(lbl_1_data_23C[1], 0x14);
    }
    if (GameMesStatGet(lbl_1_bss_133C) == 0 && lbl_1_bss_B1C.count[0] >= 0xd2) {
        GameMesKill(lbl_1_bss_133C);
        MgSeqModeNext();
    }
    fn_1_5CCC(1);
    fn_1_514C(lbl_1_bss_B1C.count[0]);
}

void fn_1_31C8(void)
{
    lbl_1_bss_B1C.count[0]++;
    fn_1_514C(lbl_1_bss_B1C.count[0]);
    fn_1_5CCC(1);
}

void fn_1_3210(s32 arg0)
{
    s32 i;
    s32 end;
    M554ENTRY *a;
    M554ENTRY *b;
    f32 s;

    if (arg0 == 0) {
        i = 0;
        end = 2;
    } else {
        i = arg0 - 1;
        end = arg0;
    }
    for (; i < end; i++) {
        a = &lbl_1_bss_B1C.ents[i + 0x4e];
        b = &lbl_1_bss_B1C.ents[i + 5];
        a->pos.x = lbl_1_bss_B1C.f_134[i][0] * sin(3.141592653589793 * lbl_1_bss_B1C.f_134[i][1] / 180.0);
        a->pos.z = -600.0 + lbl_1_bss_B1C.f_134[i][0] * cos(3.141592653589793 * lbl_1_bss_B1C.f_134[i][1] / 180.0);
        a->rot.y = 180.0f + lbl_1_bss_B1C.f_134[i][1];
        b->pos.x = (lbl_1_bss_B1C.f_134[i][0] - 40.0f) * sin(3.141592653589793 * lbl_1_bss_B1C.f_134[i][1] / 180.0);
        b->pos.z = -600.0 + (lbl_1_bss_B1C.f_134[i][0] - 40.0f) * cos(3.141592653589793 * lbl_1_bss_B1C.f_134[i][1] / 180.0);
        b->rot.y = a->rot.y - 180.0f;
        s = 0.6f + 0.9 * (lbl_1_bss_B1C.f_134[i][0] - 280.0f) / 700.0;
        b->pos.y = a->pos.y - 80.0f;
        Hu3DModelScaleSet(b->model, s, 1.0f, s);
    }
}

void fn_1_3518(void)
{
    M554ENTRY *unused = &lbl_1_bss_B1C.ents[2];

    if (lbl_1_bss_B1C.count[0]++ >= 0x1e) {
        lbl_1_bss_B1C.count[0] = 0;
        lbl_1_bss_B1C.round++;
    }
}

/* remaining diff: the original passes the s32 pan without the extsh that HuAudFXPlayPan(int, s16) adds here */
void fn_1_3578(s32 arg0)
{
    s32 tbl[8][6] = {
        { -3, -2, 0, 1, 2, 3 },
        { -3, -1, -1, 0, 2, 2 },
        { -4, -2, -2, -1, 1, 1 },
        { -5, -3, -3, -2, -1, 0 },
        { -6, -4, -4, -3, -2, -1 },
        { -7, -5, -5, -4, -3, -2 },
        { -8, -6, -6, -5, -4, -3 },
        { -9, -7, -7, -6, -5, -4 },
    };
    f32 mul[6] = { 0.5f, 0.75f, 1.0f, 1.25f, 1.5f, 1.75f };
    M554ENTRY *e;
    HuVecF scr;
    s32 row;
    s32 pan;

    e = &lbl_1_bss_B1C.ents[arg0 + 0x4e];
    lbl_1_bss_B1C.f_124[arg0][0] = fn_1_38D4(arg0);
    row = lbl_1_bss_B1C.round / 5;
    if (row > 7) {
        row = 7;
    }
#ifdef TARGET_PC
    /* fn_1_38D4 can return 6; the GC code reads one past the row / array. Clamp. */
    if (lbl_1_bss_B1C.f_124[arg0][0] > 5) {
        lbl_1_bss_B1C.f_124[arg0][0] = 5;
    }
#endif
    lbl_1_bss_B1C.f_124[arg0][1] = tbl[row][lbl_1_bss_B1C.f_124[arg0][0]];
    lbl_1_bss_B1C.f_134[arg0][0] += (f32)lbl_1_bss_B1C.f_124[arg0][1];
    if (lbl_1_bss_B1C.f_134[arg0][0] >= 1000.0f) {
        lbl_1_bss_B1C.f_134[arg0][0] = 1000.0f;
    }
    lbl_1_bss_B1C.slot[arg0].speed = mul[lbl_1_bss_B1C.f_124[arg0][0]];
    if (lbl_1_bss_B1C.slot[arg0].time >= 10.0f && lbl_1_data_24C[arg0] == 0) {
        lbl_1_data_24C[arg0] = 0;
        Hu3D3Dto2D(&lbl_1_bss_B1C.ents[arg0 + 0x4e].pos, 1, &scr);
        pan = (s32)(32.0f + (64.0f * scr.x) / 576.0f);
        if (arg0 != 0) {
            HuAudFXPlayPan(0x88d, pan);
        } else {
            HuAudFXPlayPan(0x88e, pan);
        }
    }
    if (lbl_1_data_24C[arg0] != 0 && lbl_1_bss_B1C.slot[arg0].time < 2.0f) {
        lbl_1_data_24C[arg0] = 0;
    }
}

s32 fn_1_38D4(s32 arg0)
{
    s32 th[6] = { 0, 2, 5, 7, 10, 0x3d };
    s32 i;
    s32 idx;
    s32 sum;

    lbl_1_bss_B1C.hist[arg0][lbl_1_bss_B1C.count[0]] = 0;
    if ((lbl_1_bss_B1C.player[arg0].btnDown & 0x40) != 0 && lbl_1_data_254 != 0) {
        lbl_1_data_254 = 0;
        lbl_1_bss_B1C.hist[arg0][lbl_1_bss_B1C.count[0]] = 1;
    } else if ((lbl_1_bss_B1C.player[arg0].btnDown & 0x20) != 0 && lbl_1_data_254 != 1) {
        lbl_1_data_254 = 1;
        lbl_1_bss_B1C.hist[arg0][lbl_1_bss_B1C.count[0]] = 1;
    }
    i = 0;
    sum = 0;
    for (; i < 0x3c; i++) {
        idx = lbl_1_bss_B1C.count[0] - i;
        if (idx < 0) {
            idx += 0x3c;
        }
        if (lbl_1_bss_B1C.hist[arg0][idx] != 0) {
            sum++;
        }
    }
    i = 0;
    while (i < 6) {
        if (sum > th[i]) {
            i++;
            continue;
        }
        break;
    }
    return i;
}

void fn_1_3AC8(s32 arg0)
{
    memset(&lbl_1_bss_6C[arg0], 0, 0xc);
    lbl_1_bss_6C[arg0].kind = lbl_1_bss_B1C.player[arg0].dif - 1;
    lbl_1_bss_6C[arg0].count = fn_1_3B58(arg0);
}

s32 fn_1_3B58(s32 arg0)
{
    s32 ret;

#ifdef TARGET_PC
    ret = 0; /* GC returned the stale register when kind is outside 0..3 */
#endif
    if (lbl_1_bss_6C[arg0].kind == 0) {
        if (lbl_1_bss_B1C.round >= 6) {
            ret = rand8() % 8 + 0x19;
        } else {
            ret = rand8() % 5 + 0x14;
        }
    } else if (lbl_1_bss_6C[arg0].kind == 1) {
        if (lbl_1_bss_B1C.round >= 0x12) {
            ret = rand8() % 8 + 6;
        } else {
            ret = rand8() % 4 + 6;
        }
    } else if (lbl_1_bss_6C[arg0].kind == 2) {
        if (lbl_1_bss_B1C.round >= 0x14) {
            ret = rand8() % 3 + 6;
        } else {
            ret = rand8() % 3 + 5;
        }
    } else if (lbl_1_bss_6C[arg0].kind == 3) {
        if (lbl_1_bss_B1C.round >= 0x19) {
            ret = rand8() % 4 + 4;
        } else {
            ret = rand8() % 3 + 3;
        }
    }
    return ret;
}

void fn_1_3D6C(s32 arg0)
{
    lbl_1_bss_6C[arg0].count--;
    if (lbl_1_bss_6C[arg0].count <= 0) {
        if (lbl_1_bss_6C[arg0].flip != 0) {
            lbl_1_bss_B1C.player[arg0].btnDown = 0x40;
        } else {
            lbl_1_bss_B1C.player[arg0].btnDown = 0x20;
        }
        lbl_1_bss_6C[arg0].flip = lbl_1_bss_6C[arg0].flip ^ 1;
        lbl_1_bss_6C[arg0].count = fn_1_3B58(arg0);
    }
}

void fn_1_4050(s32 arg0)
{
    Mtx mtx;
    HuVecF pos;
    HuVecF rot;
    f32 s;
    M554ENTRY *a;
    M554ENTRY *b;
    M554ENTRY *c;

    a = &lbl_1_bss_B1C.ents[arg0 + 0x4e];
    b = &lbl_1_bss_B1C.ents[arg0 + 3];
    c = &lbl_1_bss_B1C.ents[arg0 + 5];
    if (lbl_1_bss_B1C.count[arg0] == 0) {
        Hu3DModelObjMtxGet(a->model, CharModelItemHookGet(lbl_1_bss_B1C.player[arg0].charNo, 1, 0), mtx);
        Hu3DModelHookReset(a->model);
        Hu3DMtxRotGet(mtx, &rot);
        pos.x = mtx[0][3];
        pos.y = mtx[1][3];
        pos.z = mtx[2][3];
        Hu3DModelPosSetV(b->model, &pos);
        Hu3DModelRotSetV(b->model, &rot);
        b->pos.x = pos.x;
        b->pos.y = pos.y;
        b->pos.z = pos.z;
        b->rot.x = rot.x;
        b->rot.y = rot.y;
        b->rot.z = rot.z;
        lbl_1_bss_B1C.f_134[arg0][2] = lbl_1_bss_B1C.f_134[arg0][0];
        lbl_1_bss_B1C.f_134[arg0][3] = lbl_1_bss_B1C.f_134[arg0][1];
        lbl_1_bss_B1C.slot[arg0].shiftState = 1;
        lbl_1_bss_B1C.slot[arg0].shiftMot = 4;
        lbl_1_bss_B1C.slot[arg0].shiftStart = 0.0f;
        lbl_1_bss_B1C.slot[arg0].shiftEnd = 10.0f;
        lbl_1_bss_B1C.slot[arg0].shiftAttr = 0x40000001;
    }
    lbl_1_bss_B1C.count[arg0]++;
    if (lbl_1_bss_B1C.count[arg0] < 0x1e) {
        lbl_1_bss_B1C.f_134[arg0][0] -= 7.5;
        if (lbl_1_bss_B1C.winner == 2) {
            if (arg0 != 0) {
                lbl_1_bss_B1C.f_134[arg0][1] -= 17.75f;
            } else {
                lbl_1_bss_B1C.f_134[arg0][1] -= 22.25f;
            }
        } else {
            lbl_1_bss_B1C.f_134[arg0][1] -= 20.0f;
        }
        a->pos.y += (f32)(lbl_1_bss_B1C.count[arg0] / 4);
        a->rot.y += 15.0f;
    } else if (lbl_1_bss_B1C.count[arg0] < 0x3c) {
        lbl_1_bss_B1C.f_134[arg0][0] += 8.5;
        lbl_1_bss_B1C.f_134[arg0][1] -= 20.0f;
        a->pos.y += (f32)(lbl_1_bss_B1C.count[arg0] / 4);
        a->rot.z += 5.0f;
        a->rot.y += 15.0f;
        a->rot.x += 5.0f;
    } else {
        lbl_1_bss_B1C.f_134[arg0][0] += 30.0 + (f64)((lbl_1_bss_B1C.count[arg0] - 0x3c) * 4);
        lbl_1_bss_B1C.f_134[arg0][1] -= 20.0f;
        a->pos.y += (f32)lbl_1_bss_B1C.count[arg0];
        a->rot.x += 5.0f;
        a->rot.y += 15.0f;
        a->rot.z += 5.0f;
    }
    a->pos.x = lbl_1_bss_B1C.f_134[arg0][0] * sin(3.141592653589793 * lbl_1_bss_B1C.f_134[arg0][1] / 180.0);
    a->pos.z = -600.0 + lbl_1_bss_B1C.f_134[arg0][0] * cos(3.141592653589793 * lbl_1_bss_B1C.f_134[arg0][1] / 180.0);
    if (lbl_1_bss_B1C.count[arg0] < 0xf) {
        if (lbl_1_bss_B1C.count[arg0] % 4 != 0) {
            Hu3DModelAttrReset(c->model, 1);
        } else {
            Hu3DModelAttrSet(c->model, 1);
        }
        s = 0.6f - 0.02f * (f32)lbl_1_bss_B1C.count[arg0];
        Hu3DModelScaleSet(c->model, s, s, s);
    } else if (lbl_1_bss_B1C.count[arg0] < 0x1e) {
        if (lbl_1_bss_B1C.count[arg0] % 2 != 0) {
            Hu3DModelAttrReset(c->model, 1);
        } else {
            Hu3DModelAttrSet(c->model, 1);
        }
        s = 0.6f - 0.02f * (f32)lbl_1_bss_B1C.count[arg0];
        Hu3DModelScaleSet(c->model, s, s, s);
    } else if (lbl_1_bss_B1C.count[arg0] == 0x1e) {
        Hu3DModelAttrSet(c->model, 1);
    }
    if (lbl_1_bss_B1C.count[arg0] < 0x1e) {
        lbl_1_bss_B1C.f_134[arg0][2] -= 7.5;
        if (lbl_1_bss_B1C.winner == 2) {
            if (arg0 != 0) {
                lbl_1_bss_B1C.f_134[arg0][3] -= 12.75f;
            } else {
                lbl_1_bss_B1C.f_134[arg0][3] -= 17.25;
            }
        } else {
            lbl_1_bss_B1C.f_134[arg0][3] -= 15.0f;
        }
        b->pos.y += (f32)(lbl_1_bss_B1C.count[arg0] / 6);
    } else if (lbl_1_bss_B1C.count[arg0] < 0x3c) {
        lbl_1_bss_B1C.f_134[arg0][2] += 8.5;
        lbl_1_bss_B1C.f_134[arg0][3] -= 15.0f;
        b->pos.y += (f32)(lbl_1_bss_B1C.count[arg0] / 6);
    } else if (lbl_1_bss_B1C.count[arg0] < 0x5a) {
        lbl_1_bss_B1C.f_134[arg0][2] += 8.5;
        lbl_1_bss_B1C.f_134[arg0][3] -= 20.0f;
        b->pos.y += (f32)(lbl_1_bss_B1C.count[arg0] / 4);
    } else {
        lbl_1_bss_B1C.f_134[arg0][2] += 30.0 + (f64)((lbl_1_bss_B1C.count[arg0] - 0x3c) * 4);
        lbl_1_bss_B1C.f_134[arg0][3] -= 20.0f;
        b->pos.y += (f32)lbl_1_bss_B1C.count[arg0];
    }
    b->pos.x = lbl_1_bss_B1C.f_134[arg0][2] * sin(3.141592653589793 * lbl_1_bss_B1C.f_134[arg0][3] / 180.0);
    b->pos.z = -600.0 + lbl_1_bss_B1C.f_134[arg0][2] * cos(3.141592653589793 * lbl_1_bss_B1C.f_134[arg0][3] / 180.0);
    b->rot.x -= 5.0f;
    b->rot.y += 10.0f;
    b->rot.z += 5.0f;
}

void fn_1_4D60(s32 arg0)
{
    lbl_1_bss_B1C.f_134[arg0][0] += 4.0f;
    if (lbl_1_bss_B1C.f_134[arg0][0] > 1000.0f) {
        lbl_1_bss_B1C.f_134[arg0][0] = 1000.0f;
    }
    fn_1_3210(arg0 + 1);
    lbl_1_bss_B1C.slot[arg0].speed += 0.05f;
    if (lbl_1_bss_B1C.slot[arg0].speed > 1.5f) {
        lbl_1_bss_B1C.slot[arg0].speed = 1.5f;
    }
}

s32 fn_1_514C(s32 arg0)
{
    f32 f;
    M554ENTRY *e;
    s32 ret;
    s32 v;

    ret = 0;
    e = &lbl_1_bss_B1C.ents[2];
    if (lbl_1_bss_B1C.winner != 2) {
        if (arg0 >= 0xf0) {
            arg0 = 0xf0;
        }
        f = 1.0f + 0.3f * (f32)arg0;
        if (f >= 50.0f) {
            f = 50.0f;
        }
        e->pos.y -= 1.0 + 6.0 * sin(3.141592653589793 * (f64)((arg0 * 0x5a) / 240) / 180.0);
    } else {
        arg0 -= 0x7d;
        if (arg0 > 0) {
            if (arg0 >= 0x3c) {
                arg0 = 0x3c;
            }
            f = -(1.0 + 100.0 * sin(3.141592653589793 * (f64)((arg0 * 0x5a) / 60) / 180.0));
            v = fn_1_A54();
            if (v > 1) {
                fn_1_A34();
            }
        } else {
            f = 0.0f;
        }
    }
    e->pos.z -= f;
    return ret;
}

void fn_1_53B0(void)
{
    s32 a[6] = { 0x12c, 0x258, 0x384, 0x4b0, 0x708, 0x960 };
    s32 b[12] = { 0, 1, 3, 2, 0, 2, 3, 1, 0, 1, 3, 1 };
    f32 t;
    f32 f;
    s32 i;

    if (lbl_1_bss_134C != 0) {
        lbl_1_bss_1354++;
        switch (lbl_1_bss_134C) {
        case 0:
            return;
        case 1:
            for (i = 0; i < 6; i++) {
                if (lbl_1_bss_1354 == a[i]) {
                    lbl_1_bss_1358[0] += 2;
                    Hu3DMotionSpeedSet(lbl_1_bss_B1C.ents[0x4d].model, 1.0 + 0.1 * (f64)lbl_1_bss_1358[0]);
                    lbl_1_data_246[1] += 0x1f4;
                }
            }
            break;
        case 2:
            lbl_1_bss_1350++;
            if (lbl_1_bss_1358[0] > -1 && lbl_1_bss_1350 >= 5) {
                lbl_1_bss_1358[0] -= 1;
                lbl_1_bss_1350 = 0;
            }
            break;
        case 3:
            lbl_1_bss_1350++;
            if (lbl_1_bss_1350 >= 5) {
                if (lbl_1_bss_1358[0] < 0x12) {
                    lbl_1_bss_1358[0] += 1;
                }
                lbl_1_bss_1350 = 0;
            }
            break;
        }
        if (b[lbl_1_bss_1354 % 12] == 0) {
            Center.y = 290.0 - 0.8 * (f64)(lbl_1_bss_1358[0] + 1);
        }
        if (b[lbl_1_bss_1354 % 12] == 1) {
            Center.y = 290.0f;
        }
        if (b[lbl_1_bss_1354 % 12] == 2) {
            Center.y = 290.0 + 0.8 * (f64)(lbl_1_bss_1358[0] + 1);
        }
        if (b[lbl_1_bss_1354 % 12] == 3) {
            Center.y = 290.0 + 1.2 * (f64)(lbl_1_bss_1358[0] + 1);
        }
        if (lbl_1_bss_1348 == 0) {
            t = Hu3DMotionTimeGet(lbl_1_bss_B1C.ents[0x4d].model);
            f = 1.0 + 0.1 * (f64)lbl_1_bss_1358[0];
            if (t + f > 99.0f) {
                Hu3DMotionTimeSet(lbl_1_bss_B1C.ents[0x4d].model, 50.0f + (t + f) - 99.0f);
            }
        }
        if (lbl_1_data_246[0] != lbl_1_data_246[1]) {
            lbl_1_data_246[0] += 0xa;
            if (lbl_1_data_246[0] > lbl_1_data_246[1]) {
                lbl_1_data_246[0] = lbl_1_data_246[1];
            }
            HuAudFXPitchSet(lbl_1_data_23C[1], lbl_1_data_246[0]);
        }
    }
}

void fn_1_5A5C(void)
{
    s32 i;

    for (i = 0; i < 7; i++) {
        memset(&lbl_1_bss_14.row[i], 0, 0xc);
    }
}

s32 fn_1_5AB0(void)
{
    if (lbl_1_bss_B1C.round <= 0xf) {
        return rand8() % 5 * 8 + 0x28 + rand8() % 10;
    }
    if (lbl_1_bss_B1C.round <= 0x14) {
        return rand8() % 5 * 7 + 0x23 + rand8() % 9;
    }
    if (lbl_1_bss_B1C.round <= 0x19) {
        return rand8() % 5 * 6 + 0x1e + rand8() % 8;
    }
    if (lbl_1_bss_B1C.round <= 0x1e) {
        return rand8() % 5 * 5 + 0x19 + rand8() % 7;
    }
    return rand8() % 5 * 4 + 0x14 + rand8() % 6;
}

void fn_1_5CCC(s32 arg0)
{
    s32 i;
    M554ENTRY *e;
    f32 t;

    for (i = 0; i < 7; i++) {
        e = &lbl_1_bss_B1C.ents[i + 7];
        switch (lbl_1_bss_14.row[i].state) {
        case 0:
            lbl_1_bss_14.row[i].state = 1;
            lbl_1_bss_14.row[i].timer = fn_1_5AB0();
            break;
        case 1:
            lbl_1_bss_14.row[i].timer--;
            if (lbl_1_bss_14.row[i].timer <= 0) {
                Hu3DMotionTimeSet(e->model, 0.0f);
                Hu3DMotionSpeedSet(e->model, 1.0f);
                Hu3DModelAttrReset(e->model, 1);
                lbl_1_bss_14.row[i].state = 2;
                if (arg0 != 0) {
                    lbl_1_bss_14.row[i].state = 3;
                }
                lbl_1_bss_14.row[i].maxTime = Hu3DMotionMaxTimeGet(e->model);
            }
            break;
        case 2:
            t = Hu3DMotionTimeGet(e->model);
            if (t >= lbl_1_bss_14.row[i].maxTime) {
                Hu3DModelAttrSet(e->model, 1);
                lbl_1_bss_14.row[i].state = 1;
                if (arg0 != 0) {
                    lbl_1_bss_14.row[i].state = 3;
                }
                lbl_1_bss_14.row[i].timer = fn_1_5AB0();
            }
            break;
        }
    }
    if (arg0 != 0) {
        lbl_1_bss_1348 = 1;
    }
}
