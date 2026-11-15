// carddll/dice.c - dice block / roulette effects of the Card Party bonus mode.

#include "types.h"
#include "game/hu3d.h"
#include "game/esprite.h"
#include "game/audio.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/pad.h"
#include "REL/carddll/dice.h"

/* ---- bss owned by dice.c ---- */
static s16 lbl_1_bss_11B8;     /* light id of the 2D dice model */
static s16 lbl_1_bss_11BA[2];  /* motion ids of the 2D dice model */
static s16 lbl_1_bss_11BE;     /* model id of the 2D dice model */
static s16 lbl_1_bss_11C0[6];  /* sprite ids of the result banner */
static f32 lbl_1_bss_11CC[3];  /* banner x offset, banner alpha, dice y offset */
static s32 lbl_1_bss_11D8;
s32 lbl_1_bss_11DC;
static s16 lbl_1_bss_11E0;
static s16 lbl_1_bss_11E2;
static s16 lbl_1_bss_11E4;
static s16 lbl_1_bss_11E6;
static s16 lbl_1_bss_11E8;
static s32 lbl_1_bss_11EC;
static s32 lbl_1_bss_11F0;
static s16 lbl_1_bss_11F4;
static f32 lbl_1_bss_11F8[10];
static s32 lbl_1_bss_1220[10]; /* [0] = counter */
static f32 lbl_1_bss_1248[10];
static s32 lbl_1_bss_1270[10]; /* [0] = counter */
s32 lbl_1_bss_1298[4];         /* [0] = main state, [1] = sub state */

#define CUR_PLAYER(work) (&(work)->pl[(work)->order[(work)->curPlayer]])
#define CELL_FLAGS(p) (lbl_1_bss_8C4[(p)->row][(p)->col])

/* Dice block at the player's cell: roulette that picks a result. */
// NONMATCHING: the original keeps each Vec copy in its own stack temp (larger frame, _savegpr_17)
s32 fn_1_325F4(CardGame *work) {
    Vec pos;
    Vec dir;
    Vec scr;
    Vec tmp;
    s32 ret = 0;
    s32 kind;
    CardPlayer *p = CUR_PLAYER(work);

    if (CELL_FLAGS(p) & 0x100) {
        kind = 0;
    } else if (CELL_FLAGS(p) & 0x200) {
        kind = 1;
    } else if (CELL_FLAGS(p) & 0x400) {
        kind = 2;
    } else {
        kind = -1;
    }
    if (kind == -1) {
        return 1;
    }
    switch (lbl_1_bss_1298[0]) {
    case 0:
        if (kind == 2) {
            lbl_1_bss_1220[0] = 0;
            lbl_1_bss_11F8[9] = 600.0f;
            PSVECSubtract(&Hu3DCamera[0].pos, &Hu3DCamera[0].target, &dir);
            dir.y = 0.0f;
            PSVECNormalize(&dir, &dir);
            PSVECScale(&dir, &dir, 100.0f);
            fn_1_9C74(&pos, p->row, p->col, 1, 1);
            pos.x += -50.0f;
            pos.y += lbl_1_bss_11F8[9];
            PSVECAdd(&dir, &pos, &pos);
            lbl_1_bss_11F0 = (kind == 0) ? 0x13 : (kind == 1) ? 0x14 : 0x15;
            tmp = pos;
            lbl_1_bss_11F4 = fn_1_3ED3C(lbl_1_bss_11F0, &tmp);
            fn_1_3F594(lbl_1_bss_11F0, 0, 4.0f);
            lbl_1_bss_1298[0] = 1;
        } else {
            fn_1_9C74(&pos, p->row, p->col, 1, 1);
            pos.x += -50.0f;
            lbl_1_bss_11EC = (kind == 0) ? 0x16 : (kind == 1) ? 0x17 : 0x18;
            tmp = pos;
            fn_1_3ED3C(lbl_1_bss_11EC, &tmp);
            fn_1_3F594(lbl_1_bss_11EC, 0, 2.0f);
            lbl_1_bss_1298[0] = 2;
            fn_1_8D04(0x441, lbl_1_bss_478[p->row][p->col], 1);
        }
        break;
    case 1:
        if (lbl_1_bss_1220[0] < 0x32) {
            lbl_1_bss_1220[0]++;
            lbl_1_bss_11F8[9] = 600.0f - 4.0f * (3.0f * (f32)lbl_1_bss_1220[0]);
        }
        PSVECSubtract(&Hu3DCamera[0].pos, &Hu3DCamera[0].target, &dir);
        dir.y = 0.0f;
        PSVECNormalize(&dir, &dir);
        PSVECScale(&dir, &dir, 100.0f);
        fn_1_9C74(&pos, p->row, p->col, 1, 1);
        pos.x += -50.0f;
        pos.y += lbl_1_bss_11F8[9];
        PSVECAdd(&dir, &pos, &pos);
        tmp = pos;
        fn_1_3F3D8(lbl_1_bss_11F0, lbl_1_bss_11F4, &tmp);
        Hu3D3Dto2D(&pos, 1, &scr);
        if (lbl_1_bss_20[0] == 0) {
            if (scr.y >= 0.0f) {
                fn_1_8D04(0x3FE, lbl_1_bss_478[p->row][p->col], 1);
                lbl_1_bss_20[0] = 1;
            }
        }
        if (lbl_1_bss_1220[0] == 0x32) {
            fn_1_9C74(&pos, p->row, p->col, 1, 1);
            pos.x += -50.0f;
            lbl_1_bss_11EC = (kind == 0) ? 0x16 : (kind == 1) ? 0x17 : 0x18;
            tmp = pos;
            fn_1_3ED3C(lbl_1_bss_11EC, &tmp);
            fn_1_3F594(lbl_1_bss_11EC, 0, 3.0f);
            lbl_1_bss_1220[0]++;
            fn_1_8D04(0x441, lbl_1_bss_478[p->row][p->col], 1);
        }
        if (fn_1_3F714(lbl_1_bss_11F0, 0) != 0) {
            fn_1_3F278(lbl_1_bss_11F0, lbl_1_bss_11F4);
            lbl_1_bss_1298[0]++;
            lbl_1_bss_20[0] = 0;
        }
        break;
    case 2:
        if (fn_1_3F714(lbl_1_bss_11EC, 0) != 0) {
            lbl_1_bss_1298[0] = 0;
            ret = 1;
        }
        break;
    }
    return ret;
}

/* Bonus roulette of the acting player: picks one of the four results and plays it. */
// NONMATCHING: the original switch compiles to an if chain; stack temp for dicePos copy differs
s32 fn_1_32D70(CardGame *work) {
    Vec dicePos = { 288.0f, 240.0f, 1500.0f };
    s32 ret = 0;
    s32 kind;
    s32 i;
    CardPlayer *p = CUR_PLAYER(work);

    switch (lbl_1_bss_1298[0]) {
    case 0:
        *(s32 *)(lbl_1_bss_12A8 + 8) = GwPlayerConf[work->order[work->curPlayer]].charNo;
        fn_1_4FBC8(work, 0x36);
        fn_1_36B10(0);
        if ((&p->f34)[2] != -1) {
            kind = frandmod(3);
        } else {
            kind = frandmod(4);
        }
        switch (kind) {
        case 0:
            p->f50 |= 0x800;
            break;
        case 1:
            p->f50 |= 0x1000;
            break;
        case 2:
            p->f50 |= 0x2000;
            break;
        default:
            p->f50 |= 0x4000;
            break;
        }
        lbl_1_bss_1298[0]++;
        break;
    case 1:
        if (p->f50 & 0x800) {
            kind = 0;
        } else if (p->f50 & 0x1000) {
            kind = 2;
        } else if (p->f50 & 0x2000) {
            kind = 3;
        } else {
            kind = 1;
        }
        if (fn_1_33234(kind) != 0) {
            fn_1_36B10(1);
            switch (kind) {
            case 0:
                fn_1_4FBC8(work, 0x37);
                break;
            case 2:
                fn_1_4FBC8(work, 0x39);
                break;
            case 3:
                fn_1_4FBC8(work, 0x3A);
                break;
            default:
                fn_1_4FBC8(work, 0x38);
                break;
            }
            lbl_1_bss_1298[0] = 10;
            fn_1_36B10(0);
            lbl_1_bss_1248[8] = 0.0f;
            lbl_1_bss_1248[9] = 0.0f;
        }
        break;
    case 2:
        if (p->f50 & 0x800) {
            if (fn_1_337C8(work) != 0) {
                lbl_1_bss_1298[0]++;
            }
        } else if (p->f50 & 0x1000) {
            if (fn_1_33C94(work) != 0) {
                lbl_1_bss_1298[0]++;
            }
        } else if (p->f50 & 0x2000) {
            if (fn_1_34DA4(work) != 0) {
                p->f50 &= ~0x2000;
                lbl_1_bss_1298[0]++;
            }
        } else if ((p->f50 & 0x4000) && fn_1_34E24(work) != 0) {
            p->f50 &= ~0x4000;
            lbl_1_bss_1298[0]++;
        }
        break;
    case 3:
        lbl_1_bss_1298[0] = 0;
        ret = 1;
        work->f188 = 200;
        for (i = 0; i < 4; i++) {
            work->pl[i].f50 |= 0x8000;
        }
        break;
    case 10:
        lbl_1_bss_1248[8] += 10.0f;
        dicePos.y = dicePos.y - lbl_1_bss_1248[8];
        fn_1_36A70(&dicePos);
        lbl_1_bss_1248[9] += 15.0f;
        Hu3DModelRotSet(fn_1_36A60(), 0.0f, lbl_1_bss_1248[9], 0.0f);
        if (lbl_1_bss_1248[8] >= 400.0f) {
            fn_1_36B78();
            lbl_1_bss_1298[0] = 2;
        }
        break;
    }
    return ret;
}

/* Result sprite of the roulette (kind 0..3): pops in, spins out. */
// NONMATCHING: instruction scheduling of the bss_1248 base address
s32 fn_1_33234(s32 kind) {
    s32 ret = 0;

    switch (lbl_1_bss_1298[1]) {
    case 0:
        lbl_1_bss_11E8 = espEntry(lbl_1_data_2B4[kind + 0x15], 0, 0);
        espDrawNoSet(lbl_1_bss_11E8, 0);
        espPosSet(lbl_1_bss_11E8, 210.0f, 100.0f);
        lbl_1_bss_1248[0] = 5.0f;
        espScaleSet(lbl_1_bss_11E8, 1.0f + lbl_1_bss_1248[0], 1.0f + lbl_1_bss_1248[0]);
        lbl_1_bss_1248[1] = 0.0f;
        espTPLvlSet(lbl_1_bss_11E8, lbl_1_bss_1248[1]);
        lbl_1_bss_1298[1]++;
        break;
    case 1:
        lbl_1_bss_1248[0] -= 0.1f;
        if (lbl_1_bss_1248[0] < 0.0f) {
            lbl_1_bss_1248[0] = 0.0f;
        }
        if (lbl_1_bss_20[0] == 0 && lbl_1_bss_1248[0] == 0.0f) {
            HuAudFXPlay(0x42B);
            lbl_1_bss_20[0] = 1;
        }
        espScaleSet(lbl_1_bss_11E8, 1.0f + lbl_1_bss_1248[0], 1.0f + lbl_1_bss_1248[0]);
        lbl_1_bss_1248[1] += 0.015f;
        if (lbl_1_bss_1248[1] > 1.0f) {
            lbl_1_bss_1248[1] = 1.0f;
        }
        espTPLvlSet(lbl_1_bss_11E8, lbl_1_bss_1248[1]);
        if (lbl_1_bss_1248[0] == 0.0f && lbl_1_bss_1248[1] == 1.0f) {
            lbl_1_bss_1270[0] = 0x1E;
            lbl_1_bss_1298[1]++;
            lbl_1_bss_20[0] = 0;
        }
        break;
    case 2:
        lbl_1_bss_1270[0]--;
        if (lbl_1_bss_1270[0] == 0) {
            lbl_1_bss_1248[0] = 1.0f;
            lbl_1_bss_1248[1] = 0.0f;
            lbl_1_bss_1248[2] = 1.0f;
            lbl_1_bss_1298[1]++;
        }
        break;
    case 3:
        lbl_1_bss_1248[0] = (f32)((f64)lbl_1_bss_1248[0] - 0.02);
        if (lbl_1_bss_1248[0] < 0.0f) {
            lbl_1_bss_1248[0] = 0.0f;
        }
        espScaleSet(lbl_1_bss_11E8, lbl_1_bss_1248[0], lbl_1_bss_1248[0]);
        lbl_1_bss_1248[1] += 10.0f;
        if (lbl_1_bss_1248[1] > 350.0f) {
            lbl_1_bss_1248[1] = 0.0f;
        }
        espZRotSet(lbl_1_bss_11E8, lbl_1_bss_1248[1]);
        lbl_1_bss_1248[2] = (f32)((f64)lbl_1_bss_1248[2] - 0.02);
        if (lbl_1_bss_1248[2] < 0.0f) {
            lbl_1_bss_1248[2] = 0.0f;
            espKill(lbl_1_bss_11E8);
            lbl_1_bss_1298[1] = 0;
            ret = 1;
        }
        if (ret == 0) {
            espTPLvlSet(lbl_1_bss_11E8, lbl_1_bss_1248[2]);
        }
        break;
    }
    return ret;
}

/* Result 0: a big block falls on the board. */
// NONMATCHING: the original copies the 1.2 scale Vec from rodata with integer loads
s32 fn_1_337C8(CardGame *work) {
    Vec pos;
    Vec dir;
    Vec tmp;
    Vec scale;
    s16 slot;
    s32 ret = 0;

    fn_1_9DA8(&pos, work);
    switch (lbl_1_bss_1298[1]) {
    case 0:
        lbl_1_bss_11E2 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[0xE4 / 4], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelCameraSet(lbl_1_bss_11E2, 1);
        Hu3DModelPosSet(lbl_1_bss_11E2, -50.0f + pos.x, 300.0f + pos.y, pos.z);
        Hu3DModelRotSet(lbl_1_bss_11E2, 0.0f, 45.0f, 0.0f);
        Hu3DModelScaleSet(lbl_1_bss_11E2, 0.4f, 0.4f, 0.4f);
        Hu3DModelAttrSet(lbl_1_bss_11E2, 1);
        Hu3DMotionSpeedSet(lbl_1_bss_11E2, 0.0f);
        lbl_1_bss_1270[0] = 0;
        pos.x += -50.0f;
        pos.y += 300.0f;
        scale.x = 1.2f;
        scale.y = 1.2f;
        scale.z = 1.2f;
        PSVECSubtract(&Hu3DCamera[0].pos, &Hu3DCamera[0].target, &dir);
        PSVECNormalize(&dir, &dir);
        PSVECScale(&dir, &dir, 100.0f);
        PSVECAdd(&dir, &pos, &pos);
        tmp = pos;
        slot = fn_1_3ED3C(1, &tmp);
        fn_1_3F6B0(1, slot, &scale);
        lbl_1_bss_1298[1]++;
        break;
    case 1:
        lbl_1_bss_1270[0]++;
        if (lbl_1_bss_1270[0] == 12) {
            Hu3DModelAttrReset(lbl_1_bss_11E2, 1);
        }
        if (lbl_1_bss_1270[0] == 60) {
            lbl_1_bss_1270[0] = 0;
            lbl_1_bss_1248[0] = 0.4f;
            lbl_1_bss_1298[1]++;
            fn_1_8D04(0x31A, lbl_1_bss_11E2, 1);
        }
        break;
    case 2:
        lbl_1_bss_1270[0] += 10;
        lbl_1_bss_1248[0] -= 0.02f;
        if (lbl_1_bss_1248[0] < 0.0f) {
            lbl_1_bss_1248[0] = 0.0f;
        }
        Hu3DModelPosSet(lbl_1_bss_11E2, -50.0f + pos.x, (300.0f + pos.y) - (f32)lbl_1_bss_1270[0], pos.z);
        Hu3DModelScaleSet(lbl_1_bss_11E2, lbl_1_bss_1248[0], lbl_1_bss_1248[0], lbl_1_bss_1248[0]);
        if (lbl_1_bss_1270[0] == 300) {
            Hu3DModelKill(lbl_1_bss_11E2);
            lbl_1_bss_1298[1] = 0;
            ret = 1;
        }
        break;
    }
    return ret;
}

/* Result 1: other players' blocks rise, a counter banner slides in. */
// NONMATCHING: operand order of the player index scaling in the model loop
s32 fn_1_33C94(CardGame *work) {
    Vec pos;
    s32 ret = 0;
    s32 i;
    s32 row;
    s32 col;
    s16 id;

    switch (lbl_1_bss_1298[1]) {
    case 0:
        fn_1_351C4(work->order[work->curPlayer]);
        if (fn_1_393C4(work) != 0) {
            work->f1B0[3] = work->f184;
            work->f184 = 0;
            lbl_1_bss_C48[0] = 0;
            lbl_1_bss_C48[1] = 0;
            lbl_1_bss_1298[1]++;
        }
        break;
    case 1:
        lbl_1_bss_11E6 = espEntry(lbl_1_data_2B4[0x64 / 4], 0, 0);
        espDrawNoSet(lbl_1_bss_11E6, 0);
        espPriSet(lbl_1_bss_11E6, 0xF0);
        lbl_1_bss_1270[0] = 200;
        espPosSet(lbl_1_bss_11E6, 210.0f, 79.0f - (f32)lbl_1_bss_1270[0]);
        lbl_1_bss_11E4 = espEntry(lbl_1_data_2B4[0x68 / 4], 0, 0);
        espDrawNoSet(lbl_1_bss_11E4, 0);
        espPriSet(lbl_1_bss_11E4, 0xE0);
        espBankSet(lbl_1_bss_11E4, (s16)(work->f1B0[3] - 1));
        lbl_1_bss_11E0 = espEntry(lbl_1_data_2B4[0x8C / 4], 0, 0);
        espDrawNoSet(lbl_1_bss_11E0, 0);
        espPriSet(lbl_1_bss_11E0, 0xF0);
        espPosSet(lbl_1_bss_11E0, 210.0f, 79.0f - (f32)lbl_1_bss_1270[0]);
        espPosSet(lbl_1_bss_11E4, 102.0f, 79.0f);
        lbl_1_bss_1248[0] = 0.0f;
        espScaleSet(lbl_1_bss_11E4, lbl_1_bss_1248[0], 1.0f);
        for (i = 0; i < 4; i++) {
            if (i != work->order[work->curPlayer]) {
                work->pl[i].f58 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[0x1D8 / 4], HU_MEMNUM_OVL, HEAP_MODEL));
                Hu3DModelCameraSet(work->pl[i].f58, 1);
                Hu3DModelLayerSet(work->pl[i].f58, 2);
                row = work->pl[i].row;
                col = work->pl[i].col;
                if (work->pl[i].f00 == 1) {
                    row--;
                } else if (work->pl[i].f00 == 2) {
                    row++;
                } else if (work->pl[i].f00 == 3) {
                    col--;
                } else if (work->pl[i].f00 == 4) {
                    col++;
                }
                fn_1_9C74(&pos, row, col, work->pl[i].f20, work->pl[i].f24);
                Hu3DModelPosSet(work->pl[i].f58, -50.0f + pos.x, pos.y, pos.z);
                Hu3DModelScaleSet(work->pl[i].f58, 1.0f, 0.0f, 1.0f);
            }
        }
        lbl_1_bss_5E = 0;
        lbl_1_bss_1248[1] = 0.0f;
        lbl_1_bss_1298[1]++;
        HuAudFXPlay(0x432);
        break;
    case 2:
        lbl_1_bss_1270[0] -= 5;
        if (lbl_1_bss_1270[0] < 0) {
            lbl_1_bss_1270[0] = 0;
        }
        if (work->f1B0[3] != 1) {
            id = lbl_1_bss_11E6;
        } else {
            id = lbl_1_bss_11E0;
        }
        espPosSet(id, 210.0f, 79.0f - (f32)lbl_1_bss_1270[0]);
        lbl_1_bss_1248[1] += 0.06666667f;
        if (lbl_1_bss_1248[1] > 1.0f) {
            lbl_1_bss_1248[1] = 1.0f;
        }
        for (i = 0; i < 4; i++) {
            if (i != work->order[work->curPlayer] && !(work->pl[i].f50 & 0x200)) {
                Hu3DModelScaleSet(work->pl[i].f58, 1.0f, lbl_1_bss_1248[1], 1.0f);
            }
        }
        if (lbl_1_bss_1270[0] == 0) {
            if (lbl_1_bss_1248[0] == 0.0f) {
                HuAudFXPlay(0x433);
            }
            lbl_1_bss_1248[0] += 0.05f;
            if (lbl_1_bss_1248[0] > 1.0f) {
                lbl_1_bss_1248[0] = 1.0f;
                lbl_1_bss_1270[0] = 60;
                lbl_1_bss_1298[1]++;
            }
            espScaleSet(lbl_1_bss_11E4, lbl_1_bss_1248[0], 1.0f);
        }
        lbl_1_bss_5E = (lbl_1_bss_5E <= 0x8C) ? lbl_1_bss_5E + 10 : 0x96;
        fn_1_305B4(work, lbl_1_bss_5E);
        break;
    case 3:
        lbl_1_bss_1270[0]--;
        if (lbl_1_bss_1270[0] == 0) {
            lbl_1_bss_1270[0] = 0;
            lbl_1_bss_1298[1]++;
        }
        break;
    case 4:
        lbl_1_bss_1270[0] += 5;
        if (lbl_1_bss_1270[0] > 200) {
            lbl_1_bss_1270[0] = 200;
            lbl_1_bss_1298[1] = 0;
            ret = 1;
        }
        if (work->f1B0[3] != 1) {
            id = lbl_1_bss_11E6;
        } else {
            id = lbl_1_bss_11E0;
        }
        espPosSet(id, 210.0f, 79.0f - (f32)lbl_1_bss_1270[0]);
        espPosSet(lbl_1_bss_11E4, 102.0f, 79.0f - (f32)lbl_1_bss_1270[0]);
        break;
    }
    return ret;
}

/* Result 1, second part: count the counter down, one step per call. */
// NONMATCHING: register allocation (_savegpr_24 vs _savegpr_26)
s32 fn_1_345B4(CardGame *work) {
    s32 ret = 0;
    s32 i;
    s16 id;
    CardPlayer *p = CUR_PLAYER(work);

    switch (lbl_1_bss_1298[1]) {
    case 0:
        work->f1B0[3]--;
        if (work->f1B0[3] > 0) {
            lbl_1_bss_1270[0] = 200;
            if (work->f1B0[3] != 1) {
                id = lbl_1_bss_11E6;
            } else {
                id = lbl_1_bss_11E0;
            }
            espPosSet(id, 210.0f, 79.0f - (f32)lbl_1_bss_1270[0]);
            espPosSet(lbl_1_bss_11E4, 102.0f, 79.0f);
            espBankSet(lbl_1_bss_11E4, (s16)(work->f1B0[3] - 1));
            lbl_1_bss_1248[0] = 0.0f;
            espScaleSet(lbl_1_bss_11E4, lbl_1_bss_1248[0], 1.0f);
            lbl_1_bss_1298[1]++;
        } else {
            lbl_1_bss_5E = 0x96;
            lbl_1_bss_1248[0] = 1.0f;
            lbl_1_bss_1298[1] = 4;
            HuAudFXPlay(0x434);
        }
        break;
    case 1:
        lbl_1_bss_1270[0] -= 5;
        if (lbl_1_bss_1270[0] < 0) {
            lbl_1_bss_1270[0] = 0;
        }
        if (work->f1B0[3] != 1) {
            id = lbl_1_bss_11E6;
        } else {
            id = lbl_1_bss_11E0;
        }
        espPosSet(id, 210.0f, 79.0f - (f32)lbl_1_bss_1270[0]);
        if (lbl_1_bss_1270[0] == 0) {
            if (lbl_1_bss_1248[0] == 0.0f) {
                HuAudFXPlay(0x433);
            }
            lbl_1_bss_1248[0] += 0.05f;
            if (lbl_1_bss_1248[0] > 1.0f) {
                lbl_1_bss_1248[0] = 1.0f;
                lbl_1_bss_1270[0] = 60;
                lbl_1_bss_1298[1]++;
            }
            espScaleSet(lbl_1_bss_11E4, lbl_1_bss_1248[0], 1.0f);
        }
        break;
    case 2:
        lbl_1_bss_1270[0]--;
        if (lbl_1_bss_1270[0] == 0) {
            lbl_1_bss_1270[0] = 0;
            lbl_1_bss_1298[1]++;
        }
        break;
    case 3:
        lbl_1_bss_1270[0] += 5;
        if (lbl_1_bss_1270[0] > 200) {
            lbl_1_bss_1270[0] = 200;
            lbl_1_bss_1298[1] = 0;
            ret = -1;
        }
        if (work->f1B0[3] != 1) {
            id = lbl_1_bss_11E6;
        } else {
            id = lbl_1_bss_11E0;
        }
        espPosSet(id, 210.0f, 79.0f - (f32)lbl_1_bss_1270[0]);
        espPosSet(lbl_1_bss_11E4, 102.0f, 79.0f - (f32)lbl_1_bss_1270[0]);
        break;
    case 4:
        lbl_1_bss_5E = (lbl_1_bss_5E >= 10) ? lbl_1_bss_5E - 10 : 0;
        fn_1_305B4(work, lbl_1_bss_5E);
        lbl_1_bss_1248[0] -= 0.06666667f;
        if (lbl_1_bss_1248[0] < 0.0f) {
            lbl_1_bss_1248[0] = 0.0f;
        }
        for (i = 0; i < 4; i++) {
            if (i != work->order[work->curPlayer] && !(work->pl[i].f50 & 0x200)) {
                Hu3DModelScaleSet(work->pl[i].f58, 1.0f, lbl_1_bss_1248[0], 1.0f);
            }
        }
        if (lbl_1_bss_5E == 0) {
            p->f50 &= ~0x1000;
            espKill(lbl_1_bss_11E6);
            espKill(lbl_1_bss_11E4);
            espKill(lbl_1_bss_11E0);
            for (i = 0; i < 4; i++) {
                if (i != work->order[work->curPlayer]) {
                    Hu3DModelKill(work->pl[i].f58);
                    work->pl[i].f58 = -1;
                }
            }
            lbl_1_bss_1298[1] = 0;
            ret = 1;
        }
        break;
    }
    return ret;
}

/* Cleanup of result 1 when the mode is aborted. */
// NONMATCHING: register allocation, array index computed with add/lwz instead of lwzx
void fn_1_34CC8(CardGame *work) {
    s32 i;

    if (CUR_PLAYER(work)->f50 & 0x1000) {
        espKill(lbl_1_bss_11E6);
        espKill(lbl_1_bss_11E4);
        espKill(lbl_1_bss_11E0);
        for (i = 0; i < 4; i++) {
            if (i != work->order[work->curPlayer]) {
                Hu3DModelKill(work->pl[i].f58);
                work->pl[i].f58 = -1;
            }
        }
    }
}

/* Result 2: wait for the (COM) button press. */
s32 fn_1_34DA4(CardGame *work) {
    s32 ret = 0;

    fn_1_351C4(work->order[work->curPlayer]);
    if (fn_1_393C4(work) != 0) {
        lbl_1_bss_C48[0] = 0;
        lbl_1_bss_C48[1] = 0;
        ret = 1;
    }
    return ret;
}

/* Result 3: a hidden item is revealed on a 3D camera. */
// NONMATCHING: the original selects the position table at each use instead of once
s32 fn_1_34E24(CardGame *work) {
    Vec scale = { 0.35f, 0.35f, 0.35f };
    Vec scr;
    Vec pos3d;
    Vec tmp;
    Vec scaleTmp;
    f32 (*tbl)[2];
    s32 ret = 0;
    s32 cnt;
    s32 r;
    s16 slot;
    CardPlayer *p = CUR_PLAYER(work);

    cnt = 0;
    if ((&p->f34)[0] != -1) {
        cnt = 1;
        if ((&p->f34)[1] != -1) {
            cnt = 2;
            if ((&p->f34)[2] != -1) {
                cnt = 3;
            }
        }
    }
    Hu3DCamera[1] = Hu3DCamera[0];
    Hu3DZClearLayerSet(0xF);
    switch (lbl_1_bss_1298[1]) {
    case 0:
        if (cnt == 0) {
            tbl = lbl_1_data_548;
        } else if (cnt == 1) {
            tbl = lbl_1_data_568;
        } else {
            tbl = lbl_1_data_588;
        }
        scr.x = tbl[work->curPlayer][0];
        scr.y = tbl[work->curPlayer][1];
        scr.z = 1000.0f;
        Hu3D2Dto3D(&scr, 1, &pos3d);
        pos3d.x -= 3.0f;
        tmp = pos3d;
        slot = fn_1_3ED3C(1, &tmp);
        scaleTmp = scale;
        fn_1_3F6B0(1, slot, &scaleTmp);
        fn_1_3F658(1, slot, 7);
        fn_1_3F380(1, slot, 2);
        lbl_1_bss_1270[0] = 0;
        lbl_1_bss_1298[1]++;
        break;
    case 1:
        lbl_1_bss_1270[0]++;
        if (lbl_1_bss_1270[0] == 12) {
            r = frandmod(100);
            (&p->f34)[cnt] = (r < 50) ? 0 : (r < 80) ? 1 : 2;
            fn_1_2FFAC(work);
            lbl_1_bss_1270[0] = 0;
            lbl_1_bss_1298[1]++;
        }
        break;
    case 2:
        lbl_1_bss_1270[0]++;
        if (lbl_1_bss_1270[0] == 40) {
            lbl_1_bss_1298[1] = 0;
            if (cnt == 3) {
                ret = 1;
            }
        }
        break;
    }
    Hu3DLayerHookSet(0xF, NULL);
    return ret;
}

/* COM players press the A button after a random delay. */
void fn_1_351C4(s32 playerNo) {
    if (GwPlayerConf[playerNo].type != 0) {
        HuPadBtnDown[playerNo] = 0;
        HuPadDStkRep[playerNo] = 0;
        lbl_1_bss_C48[0] += 1;
        lbl_1_bss_C48[0] %= 0x15E;
        if (lbl_1_bss_C48[0] > frandmod(0x12C) + 0x1F) {
            HuPadBtnDown[playerNo] = 0x100;
            lbl_1_bss_C48[0] = 0;
        }
    }
}

/* Picks a random free cell (preferring cells next to a 0x400 cell) and puts a block there. */
// NONMATCHING: return structure of the final branch, register allocation
void fn_1_352D0(CardGame *work) {
    s32 list[30];
    s32 total;
    s32 pick;
    s32 idx;
    s32 cnt = 0;
    s32 i;
    s32 j;
    s32 adj;
    Vec pos;

    if (work->cnt1D0 > 2) {
        return;
    }
    total = 0;
    for (i = 0; i < work->boardSize; i++) {
        for (j = 0; j < work->boardSize; j++) {
            if (!(lbl_1_bss_8C4[i][j] & 0x10) && !(lbl_1_bss_8C4[i][j] & 0xF00)
                && (work->pl[0].f08 != i || work->pl[0].f0C != j)
                && (work->pl[1].f08 != i || work->pl[1].f0C != j)
                && (work->pl[2].f08 != i || work->pl[2].f0C != j)
                && (work->pl[3].f08 != i || work->pl[3].f0C != j)) {
                adj = 0;
                if (i == 0) {
                    if (lbl_1_bss_8C4[i + 1][j] & 0x400) {
                        adj = 1;
                    }
                } else if (lbl_1_bss_8C4[i - 1][j] & 0x400) {
                    adj = 1;
                }
                if (i == work->boardSize - 1) {
                    if (lbl_1_bss_8C4[i - 1][j] & 0x400) {
                        adj = 1;
                    }
                } else if (lbl_1_bss_8C4[i + 1][j] & 0x400) {
                    adj = 1;
                }
                if (j == 0) {
                    if (lbl_1_bss_8C4[i][j + 1] & 0x400) {
                        adj = 1;
                    }
                } else if (lbl_1_bss_8C4[i][j - 1] & 0x400) {
                    adj = 1;
                }
                if (j == work->boardSize - 1) {
                    if (lbl_1_bss_8C4[i][j - 1] & 0x400) {
                        adj = 1;
                    }
                } else if (lbl_1_bss_8C4[i][j + 1] & 0x400) {
                    adj = 1;
                }
                if (adj != 0 && cnt < 30) {
                    list[cnt] = total;
                    cnt++;
                }
                total++;
            }
        }
    }
    if (total == 0) {
        work->f1D4 = -1;
        return;
    }
    pick = frandmod(total);
    if (cnt > 0 && frandmod(2) != 0) {
        pick = list[frandmod(cnt)];
    }
    idx = 0;
    for (i = 0; i < work->boardSize; i++) {
        for (j = 0; j < work->boardSize; j++) {
            if (!(lbl_1_bss_8C4[i][j] & 0x10) && !(lbl_1_bss_8C4[i][j] & 0xF00)
                && (work->pl[0].f08 != i || work->pl[0].f0C != j)
                && (work->pl[1].f08 != i || work->pl[1].f0C != j)
                && (work->pl[2].f08 != i || work->pl[2].f0C != j)
                && (work->pl[3].f08 != i || work->pl[3].f0C != j)) {
                if (pick == idx) {
                    lbl_1_bss_8C4[i][j] |= 0x800;
                    Hu3DModelShadowMapObjReset(lbl_1_bss_478[i][j]);
                    Hu3DModelKill(lbl_1_bss_478[i][j]);
                    lbl_1_bss_478[i][j] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[0x7C / 4], HU_MEMNUM_OVL, HEAP_MODEL));
                    Hu3DModelCameraSet(lbl_1_bss_478[i][j], 1);
                    fn_1_9C74(&pos, i, j, 1, 1);
                    Hu3DModelPosSet(lbl_1_bss_478[i][j], -50.0f + pos.x, pos.y, pos.z);
                    Hu3DModelRotSet(lbl_1_bss_478[i][j], 0.0f, 0.0f, 180.0f);
                    Hu3DModelLayerSet(lbl_1_bss_478[i][j], 2);
                    Hu3DModelShadowMapSet(lbl_1_bss_478[i][j]);
                    work->f1D4 = 1;
                    j = work->boardSize;
                    i = j;
                } else {
                    idx++;
                }
            }
        }
    }
}

/* Result banner (turn counter, e.g. "3/10") sliding in and out of the corner. */
// NONMATCHING: sprite id loads are scheduled before the float constant loads
void fn_1_358F8(CardGame *work) {
    Vec dicePos = { 485.0f, 405.0f, 1000.0f };
    f32 x;
    f32 y;

    switch (lbl_1_bss_11DC) {
    case 0:
        if (work->f188 == 0xCA || work->f188 == 0xCC || work->f188 == 0xCD) {
            lbl_1_bss_11D8 = work->f188;
            lbl_1_bss_11DC++;
        }
        return;
    case 1:
        lbl_1_bss_11C0[0] = espEntry(lbl_1_data_2B4[0x88 / 4], 0, 0);
        espDrawNoSet(lbl_1_bss_11C0[0], 0x40);
        lbl_1_bss_11CC[0] = 128.0f;
        espPosSet(lbl_1_bss_11C0[0], 480.0f, 360.4f + lbl_1_bss_11CC[0]);
        lbl_1_bss_11CC[1] = 0.0f;
        espTPLvlSet(lbl_1_bss_11C0[0], lbl_1_bss_11CC[1]);
        espPriSet(lbl_1_bss_11C0[0], 0x1F0);
        lbl_1_bss_11C0[1] = espEntry(lbl_1_data_2B4[0x1C / 4], 0, 0);
        espDrawNoSet(lbl_1_bss_11C0[1], 0x40);
        espPriSet(lbl_1_bss_11C0[1], 0x1E0);
        if (work->boardSize == 10) {
            espBankSet(lbl_1_bss_11C0[1], (s16)(work->cnt1D0 / 10 + 3));
            x = 484.0f;
            if (work->cnt1D0 / 10 == 0) {
                espDispOff(lbl_1_bss_11C0[1]);
            }
        } else {
            espBankSet(lbl_1_bss_11C0[1], (s16)(work->cnt1D0 + 3));
            x = 492.0f;
        }
        espPosSet(lbl_1_bss_11C0[1], x, 336.0f + lbl_1_bss_11CC[0]);
        espTPLvlSet(lbl_1_bss_11C0[1], lbl_1_bss_11CC[1]);
        lbl_1_bss_11C0[2] = espEntry(lbl_1_data_2B4[0x1C / 4], 0, 0);
        espDrawNoSet(lbl_1_bss_11C0[2], 0x40);
        espPriSet(lbl_1_bss_11C0[2], 0x1E0);
        if (work->boardSize == 10) {
            espBankSet(lbl_1_bss_11C0[2], (s16)(work->cnt1D0 % 10 + 3));
            if (work->cnt1D0 / 10 == 0) {
                x = 492.0f;
                y = 336.0f;
            } else {
                x = 500.0f;
                y = 336.0f;
            }
        } else {
            espBankSet(lbl_1_bss_11C0[2], (s16)(work->boardSize + 3));
            x = 516.0f;
            y = 362.0f;
        }
        espPosSet(lbl_1_bss_11C0[2], x, y + lbl_1_bss_11CC[0]);
        espTPLvlSet(lbl_1_bss_11C0[2], lbl_1_bss_11CC[1]);
        if (work->boardSize == 10) {
            lbl_1_bss_11C0[3] = espEntry(lbl_1_data_2B4[0x1C / 4], 0, 0);
            espDrawNoSet(lbl_1_bss_11C0[3], 0x40);
            espBankSet(lbl_1_bss_11C0[3], 4);
            espPriSet(lbl_1_bss_11C0[3], 0x1E0);
            espPosSet(lbl_1_bss_11C0[3], 512.0f, 362.0f + lbl_1_bss_11CC[0]);
            espTPLvlSet(lbl_1_bss_11C0[3], lbl_1_bss_11CC[1]);
            lbl_1_bss_11C0[4] = espEntry(lbl_1_data_2B4[0x1C / 4], 0, 0);
            espDrawNoSet(lbl_1_bss_11C0[4], 0x40);
            espBankSet(lbl_1_bss_11C0[4], 3);
            espPriSet(lbl_1_bss_11C0[4], 0x1E0);
            espPosSet(lbl_1_bss_11C0[4], 528.0f, 362.0f + lbl_1_bss_11CC[0]);
            espTPLvlSet(lbl_1_bss_11C0[4], lbl_1_bss_11CC[1]);
        }
        fn_1_36828();
        lbl_1_bss_11CC[2] = 128.0f;
        dicePos.y += lbl_1_bss_11CC[2];
        fn_1_36A70(&dicePos);
        Hu3DModelScaleSet(fn_1_36A60(), 0.45f, 0.45f, 0.45f);
        Hu3DModelRotSet(fn_1_36A60(), 0.0f, -10.0f, 0.0f);
        lbl_1_bss_11DC++;
        return;
    case 2:
        lbl_1_bss_11CC[0] -= 8.0f;
        if (lbl_1_bss_11CC[0] < 0.0f) {
            lbl_1_bss_11CC[0] = 0.0f;
        }
        espPosSet(lbl_1_bss_11C0[0], 480.0f, 360.4f + lbl_1_bss_11CC[0]);
        lbl_1_bss_11CC[1] += 0.0625f;
        if (lbl_1_bss_11CC[1] > 1.0f) {
            lbl_1_bss_11CC[1] = 1.0f;
        }
        espTPLvlSet(lbl_1_bss_11C0[0], lbl_1_bss_11CC[1]);
        if (work->boardSize == 10) {
            x = 484.0f;
        } else {
            x = 492.0f;
        }
        espPosSet(lbl_1_bss_11C0[1], x, 336.0f + lbl_1_bss_11CC[0]);
        espTPLvlSet(lbl_1_bss_11C0[1], lbl_1_bss_11CC[1]);
        if (work->boardSize == 10) {
            if (work->cnt1D0 / 10 == 0) {
                x = 492.0f;
                y = 336.0f;
            } else {
                x = 500.0f;
                y = 336.0f;
            }
        } else {
            x = 516.0f;
            y = 362.0f;
        }
        espPosSet(lbl_1_bss_11C0[2], x, y + lbl_1_bss_11CC[0]);
        espTPLvlSet(lbl_1_bss_11C0[2], lbl_1_bss_11CC[1]);
        if (work->boardSize == 10) {
            espPosSet(lbl_1_bss_11C0[3], 512.0f, 362.0f + lbl_1_bss_11CC[0]);
            espTPLvlSet(lbl_1_bss_11C0[3], lbl_1_bss_11CC[1]);
            espPosSet(lbl_1_bss_11C0[4], 528.0f, 362.0f + lbl_1_bss_11CC[0]);
            espTPLvlSet(lbl_1_bss_11C0[4], lbl_1_bss_11CC[1]);
        }
        lbl_1_bss_11CC[2] -= 8.0f;
        dicePos.y += lbl_1_bss_11CC[2];
        fn_1_36A70(&dicePos);
        if (lbl_1_bss_11CC[0] == 0.0f && lbl_1_bss_11CC[1] == 1.0f) {
            lbl_1_bss_11DC++;
            return;
        }
        break;
    case 3:
        if (work->f188 != lbl_1_bss_11D8) {
            lbl_1_bss_11DC++;
            return;
        }
        break;
    case 4:
        lbl_1_bss_11CC[0] += 8.0f;
        if (lbl_1_bss_11CC[0] > 128.0f) {
            lbl_1_bss_11CC[0] = 128.0f;
        }
        espPosSet(lbl_1_bss_11C0[0], 480.0f, 360.0f + lbl_1_bss_11CC[0]);
        lbl_1_bss_11CC[1] -= 0.0625f;
        if (lbl_1_bss_11CC[1] < 0.0f) {
            lbl_1_bss_11CC[1] = 0.0f;
        }
        espTPLvlSet(lbl_1_bss_11C0[0], lbl_1_bss_11CC[1]);
        if (work->boardSize == 10) {
            x = 484.0f;
        } else {
            x = 492.0f;
        }
        espPosSet(lbl_1_bss_11C0[1], x, 336.0f + lbl_1_bss_11CC[0]);
        espTPLvlSet(lbl_1_bss_11C0[1], lbl_1_bss_11CC[1]);
        if (work->boardSize == 10) {
            if (work->cnt1D0 / 10 == 0) {
                x = 492.0f;
                y = 336.0f;
            } else {
                x = 500.0f;
                y = 336.0f;
            }
        } else {
            x = 516.0f;
            y = 362.0f;
        }
        espPosSet(lbl_1_bss_11C0[2], x, y + lbl_1_bss_11CC[0]);
        espTPLvlSet(lbl_1_bss_11C0[2], lbl_1_bss_11CC[1]);
        if (work->boardSize == 10) {
            espPosSet(lbl_1_bss_11C0[3], 512.0f, 362.0f + lbl_1_bss_11CC[0]);
            espTPLvlSet(lbl_1_bss_11C0[3], lbl_1_bss_11CC[1]);
            espPosSet(lbl_1_bss_11C0[4], 528.0f, 362.0f + lbl_1_bss_11CC[0]);
            espTPLvlSet(lbl_1_bss_11C0[4], lbl_1_bss_11CC[1]);
        }
        lbl_1_bss_11CC[2] += 8.0f;
        dicePos.y += lbl_1_bss_11CC[2];
        fn_1_36A70(&dicePos);
        if (lbl_1_bss_11CC[0] == 128.0f && lbl_1_bss_11CC[1] == 0.0f) {
            espKill(lbl_1_bss_11C0[0]);
            espKill(lbl_1_bss_11C0[1]);
            espKill(lbl_1_bss_11C0[2]);
            if (work->boardSize == 10) {
                espKill(lbl_1_bss_11C0[3]);
                espKill(lbl_1_bss_11C0[4]);
            }
            fn_1_36B78();
            lbl_1_bss_11DC = 0;
        }
        break;
    }
}

void fn_1_3678C(CardGame *work) {
    if (lbl_1_bss_11DC == 3) {
        espKill(lbl_1_bss_11C0[0]);
        espKill(lbl_1_bss_11C0[1]);
        espKill(lbl_1_bss_11C0[2]);
        if (work->boardSize == 10) {
            espKill(lbl_1_bss_11C0[3]);
            espKill(lbl_1_bss_11C0[4]);
        }
        fn_1_36B78();
    }
}

/* Creates the 2D dice model on its own camera (layer 15). */
void fn_1_36828(void) {
    Vec camPos = { 0.0f, 0.0f, 0.0f };
    Vec camUp = { 0.0f, 1.0f, 0.0f };
    Vec camTarget = { 0.0f, 0.0f, -100.0f };

    Hu3DZClearLayerSet(0xF);
    lbl_1_bss_11BE = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[0x1DC / 4], HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_11BA[0] = Hu3DJointMotion(lbl_1_bss_11BE, HuDataSelHeapReadNum(lbl_1_data_B0[0x1E0 / 4], HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_11BA[1] = Hu3DJointMotion(lbl_1_bss_11BE, HuDataSelHeapReadNum(lbl_1_data_B0[0x1E4 / 4], HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DMotionSet(lbl_1_bss_11BE, lbl_1_bss_11BA[0]);
    Hu3DModelCameraSet(lbl_1_bss_11BE, 2);
    Hu3DCameraPosSetV(2, &camPos, &camUp, &camTarget);
    Hu3DModelLayerSet(lbl_1_bss_11BE, 7);
    Hu3DModelAttrSet(lbl_1_bss_11BE, 0x40000001);
    lbl_1_bss_11B8 = Hu3DLLightCreate(lbl_1_bss_11BE, 0.0f, 0.0f, 100.0f, 0.0f, 0.0f, -1.0f, 0x30, 0x30, 0x30);
    Hu3DLLightStaticSet(lbl_1_bss_11BE, lbl_1_bss_11B8, 1);
    Hu3DLLightInfinitytSet(lbl_1_bss_11BE, lbl_1_bss_11B8);
}

s16 fn_1_36A60(void) {
    return lbl_1_bss_11BE;
}

/* Moves the 2D dice model and its light to the 3D point under screen position src. */
void fn_1_36A70(Vec *src) {
    Vec pos;

    Hu3D2Dto3D(src, 2, &pos);
    Hu3DModelPosSetV(lbl_1_bss_11BE, &pos);
    Hu3DLLightPosSet(lbl_1_bss_11BE, lbl_1_bss_11B8, pos.x, pos.y, pos.z + 100.0f, 0.0f, 0.0f, -1.0f);
}

void fn_1_36B10(s32 motion) {
    Hu3DMotionShiftSet(lbl_1_bss_11BE, lbl_1_bss_11BA[motion], 0.0f, 10.0f, 0x40000001);
}

void fn_1_36B78(void) {
    Hu3DLLightKill(lbl_1_bss_11BE, lbl_1_bss_11B8);
    Hu3DMotionSet(lbl_1_bss_11BE, -1);
    Hu3DMotionKill(lbl_1_bss_11BA[0]);
    Hu3DMotionKill(lbl_1_bss_11BA[1]);
    Hu3DModelKill(lbl_1_bss_11BE);
    Hu3DLayerHookSet(0xF, NULL);
}
