#include "math.h"
#include "game/audio.h"
#include "game/hu3d.h"
#include "game/object.h"

/* Bob the carried model up and down (shared by fn_1_12CAC and fn_1_13430). */
#define M555_WOBBLE(player)                                                                        \
    switch (lbl_1_bss_1894.unk160) {                                                               \
        case 0:                                                                                    \
            lbl_1_bss_1894.unk160++;                                                               \
        case 1:                                                                                    \
            lbl_1_bss_1894.phaseAngle += 2.5f;                                                     \
            lbl_1_bss_3030->mdl[(s16)((player) + 0x21)].pos.y =                                    \
                lbl_1_bss_1894.posX + 10.0 * sin(M_PI * lbl_1_bss_1894.phaseAngle / 180.0);        \
            lbl_1_bss_3030->mdl[(s16)((player) + 0xA9)].pos =                                      \
                lbl_1_bss_3030->mdl[(s16)((player) + 0x21)].pos;                                   \
            break;                                                                                 \
    }

/* Claim the first free item slot for the given kind. */
#define M555_ITEM_SPAWN(kindNo)                                                                    \
    for (i = 0; i < 0x64; i++) {                                                                   \
        if (lbl_1_bss_214[i].active == 0) {                                                        \
            lbl_1_bss_214[i].active = 1;                                                           \
            lbl_1_bss_214[i].type = 0;                                                             \
            lbl_1_bss_214[i].kind = (kindNo);                                                      \
            break;                                                                                 \
        }                                                                                          \
    }

void fn_1_12CAC(s16 player)
{
    M555_WOBBLE(player)
}

void fn_1_12E04(void)
{
    f32 lo[8];
    f32 hi[8];
    s16 moving;
    s16 i;
    s16 j;

    moving = 0;
    lo[0] = -568.0f;
    lo[1] = 568.0f;
    lo[2] = -520.0f;
    lo[3] = 520.0f;
    lo[4] = -520.0f;
    lo[5] = 520.0f;
    lo[6] = -520.0f;
    lo[7] = 520.0f;
    hi[0] = -868.0f;
    hi[1] = 868.0f;
    hi[2] = -820.0f;
    hi[3] = 820.0f;
    hi[4] = -820.0f;
    hi[5] = 820.0f;
    hi[6] = -820.0f;
    hi[7] = 820.0f;
    switch (lbl_1_bss_1894.phase4) {
        case 0:
            if (lbl_1_bss_1894.timer++ > 0x1E) {
                lbl_1_bss_1894.phase4++;
            }
            break;
        case 1: {
            f32 frames = 120.0f;
            for (i = 0xA1, j = 0; i <= 0xA8; i++, j++) {
                moving += fn_1_1898(&lbl_1_bss_3030->mdl[i].pos.x, frames, lo[j], hi[j]);
            }
            if (moving == 0) {
                for (i = 0xA1, j = 0; i <= 0xA8; i++, j++) {
                    Hu3DModelAttrSet(lbl_1_bss_3030->mdl[i].mdlId, HU3D_ATTR_DISPOFF);
                }
                lbl_1_bss_1894.phase4++;
            }
            break;
        }
        case 2:
            for (i = 0xA1, j = 0; i <= 0xA8; i++, j++) {
                Hu3DModelAttrSet(lbl_1_bss_3030->mdl[i].mdlId, HU3D_ATTR_DISPOFF);
            }
            break;
    }
}

s32 fn_1_130D8(void)
{
    s16 i;
    s16 ready;
    s32 result;

    result = 0;
    switch (lbl_1_bss_1894.phase) {
        case 0:
            fn_1_2B10(0xA);
            fn_1_2B10(0xB);
            lbl_1_bss_1894.phase++;
        case 1:
            ready = fn_1_FD64();
            if (ready != 0) {
                lbl_1_bss_11E4.mode = 0;
                lbl_1_bss_1894.phase++;
                M555_ITEM_SPAWN(2)
                M555_ITEM_SPAWN(3)
                M555_ITEM_SPAWN(4)
                M555_ITEM_SPAWN(7)
                omVibrate(lbl_1_bss_3030->mdl0[0].playerNo, 0x1E, 4, 2);
                omVibrate(lbl_1_bss_3030->mdl0[1].playerNo, 0x1E, 4, 2);
                fn_1_2A18();
                result = 1;
            }
            break;
    }
    return result;
}

s32 fn_1_13430(void)
{
    HuVecF diff;
    s16 done;
    s16 i;
    s16 p;
    s32 result;
    M555_CTRLWIN *win;

    result = 0;
    done = 0;
    if (lbl_1_bss_1894.phase >= 2) {
        if (lbl_1_bss_3030->seqNo == 0) {
            if (lbl_1_bss_1894.curPlayer != 0) {
                win = &lbl_1_bss_3030->win[lbl_1_bss_1894.curPlayer & 1];
                win->on38 = 1;
                win->on3A = 1;
                win->val40 = 0.0f;
                win->val44 = 4.0f;
                win->val3C = 0;
                win->flags = 0x40000001;
            }
            lbl_1_bss_3030->seqNo = 1;
        } else if (lbl_1_bss_1894.curPlayer != 0) {
            win = &lbl_1_bss_3030->win[lbl_1_bss_1894.curPlayer & 1];
            if (win->on38 == 0) {
                if (win->value >= 38.0f) {
                    win->value = 10.0f;
                }
            }
        }
    }
    switch (lbl_1_bss_1894.phase) {
        case 0:
            lbl_1_bss_1894.itemIdx = 1;
            lbl_1_bss_1894.itemIdx2 = 1;
            lbl_1_bss_1894.subPhase2 = 0;
            lbl_1_bss_1894.phase++;
        case 1:
            if (lbl_1_bss_1894.curPlayer == 0) {
                done = fn_1_10E20();
            } else {
                done = 1;
            }
            if (lbl_1_bss_1894.subPhase2++ > 0x64 && done != 0) {
                if (lbl_1_bss_1894.curPlayer != 0) {
                    for (i = 0; i < 0x64; i++) {
                        if (lbl_1_bss_214[i].active != 0 && lbl_1_bss_214[i].kind == 7) {
                            lbl_1_bss_214[i].type++;
                            break;
                        }
                    }
                }
                lbl_1_bss_1894.resultF = 1;
                lbl_1_bss_1894.resultOn =
                    0.05818182f * lbl_1_bss_3030->mdl[(lbl_1_bss_1894.curPlayer & 1) + 0x21].pos.x;
                lbl_1_bss_1894.scale = lbl_1_bss_1894.resultOn;
                lbl_1_bss_1894.basePosX = lbl_1_bss_3030->mdl[(lbl_1_bss_1894.curPlayer & 1) + 0x21].pos.x;
                lbl_1_bss_1894.posX = lbl_1_bss_3030->mdl[(lbl_1_bss_1894.curPlayer & 1) + 0x21].pos.y;
                lbl_1_bss_1894.posY = lbl_1_bss_3030->mdl[(lbl_1_bss_1894.curPlayer & 1) + 0x21].pos.z;
                lbl_1_bss_1894.camPos = lbl_1_bss_8->pos;
                lbl_1_bss_1894.subPhase2 = 0x78;
                lbl_1_bss_1894.phase++;
            }
            break;
        case 2:
            if (lbl_1_bss_1894.curPlayer == 0) {
                fn_1_119EC();
            }
            if (lbl_1_bss_1894.curPlayer != 0) {
                f32 frames = 120.0f;

                if (lbl_1_bss_8->rotY >= 90.0f) {
                    p = lbl_1_bss_1894.curPlayer & 1;
                    M555_WOBBLE(p)
                }
                if (lbl_1_bss_1894.subPhase2-- >= 0) {
                    lbl_1_bss_1894.subPhase2 = 0;
                }
                fn_1_2D40((lbl_1_bss_1894.curPlayer & 1) + 2, lbl_1_bss_1894.subPhase2 * 0x42);
                fn_1_1898(&lbl_1_bss_1894.scale, frames, lbl_1_bss_1894.resultOn, 64.0f);
                fn_1_2CB0((lbl_1_bss_1894.curPlayer & 1) + 2, lbl_1_bss_1894.scale);
                done += fn_1_1898(&lbl_1_bss_8->tgt.x, frames, lbl_1_data_150[1][0], lbl_1_bss_1894.basePosX);
                done += fn_1_1898(&lbl_1_bss_8->tgt.y, frames, lbl_1_data_150[1][1], 40.0f + lbl_1_bss_1894.posX);
                done += fn_1_1898(&lbl_1_bss_8->tgt.z, frames, lbl_1_data_150[1][2], lbl_1_bss_1894.posY);
                done += fn_1_1898(&lbl_1_bss_8->rotX, frames, lbl_1_data_150[1][3], -15.0f);
                done += fn_1_1898(&lbl_1_bss_8->rotY, frames, 0.0f, 180.0f);
                done += fn_1_1898(&lbl_1_bss_8->zoom, frames, lbl_1_data_150[1][6], 700.0f);
                diff.x = lbl_1_bss_8->pos.x - lbl_1_bss_1894.camPos.x;
                diff.y = lbl_1_bss_8->pos.y - lbl_1_bss_1894.camPos.y;
                diff.z = lbl_1_bss_8->pos.z - lbl_1_bss_1894.camPos.z;
            } else {
                f32 frames = 120.0f;

                done += fn_1_1898(&lbl_1_bss_8->rotY, frames, 0.0f, 180.0f);
            }
            if (done == 0) {
                lbl_1_bss_1894.subPhase2 = 0;
                lbl_1_bss_1894.phase++;
            }
            break;
        case 3:
            if (lbl_1_bss_1894.curPlayer == 0) {
                fn_1_119EC();
            }
            if (lbl_1_bss_1894.curPlayer != 0) {
                p = lbl_1_bss_1894.curPlayer & 1;
                M555_WOBBLE(p)
            }
            if (done == 0) {
                lbl_1_bss_1894.subPhase2 = 0;
                lbl_1_bss_1894.phase++;
            }
            break;
        case 4:
            if (lbl_1_bss_1894.curPlayer == 0) {
                fn_1_119EC();
            }
            if (lbl_1_bss_1894.curPlayer != 0) {
                p = lbl_1_bss_1894.curPlayer & 1;
                M555_WOBBLE(p)
            }
            if (lbl_1_bss_1894.winF != 0 && lbl_1_bss_1894.curPlayer != 0) {
                fn_1_11DFC(lbl_1_bss_1894.curPlayer & 1);
            }
            if (lbl_1_bss_1894.subPhase2++ > 0x3C) {
                lbl_1_bss_1894.subPhase2 = 0;
                lbl_1_bss_1894.phase++;
            }
            break;
        case 5:
            if (lbl_1_bss_1894.curPlayer == 0) {
                done = fn_1_119EC();
            }
            if (lbl_1_bss_1894.curPlayer != 0) {
                p = lbl_1_bss_1894.curPlayer & 1;
                M555_WOBBLE(p)
            }
            if (done == 0) {
                lbl_1_bss_1894.phase++;
            }
            break;
        case 6:
            if (lbl_1_bss_1894.curPlayer == 0) {
                result = fn_1_119EC();
            }
            if (lbl_1_bss_1894.curPlayer != 0) {
                p = lbl_1_bss_1894.curPlayer & 1;
                M555_WOBBLE(p)
            }
            result = 1;
            break;
    }
    if (lbl_1_bss_1894.curPlayer != 0) {
        fn_1_103D8(lbl_1_bss_1894.curPlayer >> 1);
    }
    return result;
}
