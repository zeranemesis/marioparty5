#include "game/audio.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/mg/seqman.h"
#include "math.h"
#include "string.h"

#define W lbl_1_bss_1894
#define C lbl_1_bss_3030

s32 fn_1_6AF0(void)
{
    s32 result = 0;
    s32 v = 0;
    s16 i;
    s32 pidx;

    W.frameCnt++;

    switch (W.phase) {
    case 0:
        v = 1;
        v = fn_1_FB2C();
        if ((s16)v != 0) {
            W.phase++;
        }
        break;

    case 1:
        for (i = 0; i < 2; i++) {
            if (lbl_1_bss_120C[i].wheel != 0) {
                fn_1_155DC(i);
            }
        }
        for (i = 0; i < 2; i++) {
            fn_1_8F4C(i);
        }
        if (W.curPlayer == 0 && W.curPlayer2 == 0) {
            break;
        }
        if (W.curPlayer == 0) {
            if (lbl_1_bss_120C[0].stage > lbl_1_bss_120C[1].stage) {
                W.curPlayer = 2;
            } else if (lbl_1_bss_120C[0].stage < lbl_1_bss_120C[1].stage) {
                W.curPlayer = 1;
            } else {
                W.curPlayer = 0;
            }
            W.winF = 1;
        }
        W.phase++;
        break;

    case 2:
        if (W.curPlayer != 0) {
            MgSeqWinnerSet(C->mdl0[W.curPlayer & 1].charNo, -1, -1, -1);
            pidx = C->mdl0[W.curPlayer & 1].playerNo;
            if (!_CheckFlag(FLAG_MG_PRACTICE)) {
                GwPlayer[pidx].mgCoinBonus = 10;
            }
        } else {
            MgSeqWinnerSet(-1, -1, -1, -1);
        }
        fn_1_2A48(1, 100);
        result = 1;
        break;
    }

    fn_1_A758();
    fn_1_90C4();
    fn_1_9B8C();
    fn_1_CFB8();
    fn_1_14420();
    fn_1_78DC();
    W.motion = 3;
    return result;
}

s16 fn_1_6DF0(void)
{
    s16 result = 0;

    W.phase = 0;
    W.phase2 = 0;
    W.subPhase = 0;
    W.subPhase2 = 0;
    C->seqNo = 0;
    lbl_1_bss_120C[0].motion = 0;
    lbl_1_bss_120C[1].motion = 0;
    fn_1_8F4C(0);
    fn_1_8F4C(1);
    fn_1_A758();
    fn_1_90C4();
    fn_1_9B8C();
    fn_1_CFB8();
    fn_1_14420();
    fn_1_12E04();
    fn_1_78DC();
    W.motion = 4;
    return result;
}

s16 fn_1_6ECC(void)
{
    s16 result = 0;

    result = fn_1_13430();
    fn_1_8F4C(0);
    fn_1_8F4C(1);
    fn_1_A758();
    fn_1_90C4();
    fn_1_9B8C();
    fn_1_CFB8();
    fn_1_14420();
    fn_1_12E04();
    fn_1_78DC();
    W.motion = 5;
    return result;
}

s16 fn_1_6F3C(void)
{
    M555_CTRLWIN *p;
    s16 result = 0;

    if (C->seqNo == 0) {
        if (W.curPlayer != 0) {
            p = &C->win[W.curPlayer & 1];
            p->on = 1;
            p->scale = 1.0f;
            p->value = 0.0f;
            p->alpha = 0;
            p->flags = 0x40000001;
        }
        C->seqNo = 1;
    } else {
        if (W.curPlayer != 0) {
            if (W.playerNo == 0) {
                CharFXPlay(C->mdl0[W.curPlayer & 1].charNo, 0x1FA);
                W.playerNo = 1;
            }
            p = &C->win[W.curPlayer & 1];
            if (p->value >= 38.0f) {
                p->value = 10.0f;
            }
        }
    }

    if (W.curPlayer == 0) {
        fn_1_119EC();
    }
    if (W.curPlayer != 0) {
        fn_1_103D8(W.curPlayer >> 1);
    }
    if (W.curPlayer != 0) {
        fn_1_12CAC(W.curPlayer & 1);
    }
    if (W.winF != 0 && W.curPlayer != 0) {
        fn_1_11DFC(W.curPlayer & 1);
    }

    fn_1_8F4C(0);
    fn_1_8F4C(1);
    fn_1_A758();
    fn_1_CFB8();
    fn_1_12E04();
    fn_1_78DC();
    W.motion = 6;
    return result;
}

s32 fn_1_71B4(void)
{
    M555_CTRLWIN *p;
    s32 result = 1;

    if (W.curPlayer != 0) {
        p = &C->win[W.curPlayer & 1];
        if (p->value >= 38.0f) {
            p->value = 10.0f;
        }
    }
    if (C->seqNo == 1) {
        fn_1_2E60(0xB, 100);
        C->seqNo = 2;
    }
    if (W.curPlayer == 0) {
        fn_1_119EC();
    }
    if (W.curPlayer != 0) {
        fn_1_12CAC(W.curPlayer & 1);
    }
    if (W.curPlayer != 0) {
        fn_1_103D8(W.curPlayer >> 1);
    }
    if (W.winF != 0 && W.curPlayer != 0) {
        fn_1_11DFC(W.curPlayer & 1);
    }
    fn_1_8F4C(0);
    fn_1_8F4C(1);
    fn_1_A758();
    fn_1_CFB8();
    fn_1_78DC();
    W.motion = 7;
    return result;
}

s32 fn_1_7364(void)
{
    s32 ret = 1;

    fn_1_2FF0();
    W.motion = 8;
    return ret;
}

static void fn_1_73A4_body(void)
{
    s16 i;

    C->mdl[0].rot.y -= 0.005f;
    for (i = 0; i < 0xAB; i++) {
        Hu3DModelPosSet(C->mdl[i].mdlId, C->mdl[i].pos.x, C->mdl[i].pos.y, C->mdl[i].pos.z);
        Hu3DModelRotSet(C->mdl[i].mdlId, C->mdl[i].rot.x, C->mdl[i].rot.y, C->mdl[i].rot.z);
    }
    for (i = 0; i < 2; i++) {
        if (C->mdl0[i].charNo == 6) {
            Hu3DModelPosSet(C->mdl[i + 0xA9].mdlId, C->mdl[i + 0xA9].pos.x, C->mdl[i + 0xA9].pos.y,
                15.0f + C->mdl[i + 0xA9].pos.z);
        }
        if (C->mdl0[i].charNo == 8) {
            Hu3DModelPosSet(C->mdl[i + 0xA9].mdlId, C->mdl[i + 0xA9].pos.x, C->mdl[i + 0xA9].pos.y - 20.0f,
                C->mdl[i + 0xA9].pos.z - 20.0f);
        }
        if (C->mdl0[i].charNo == 7) {
            Hu3DModelPosSet(C->mdl[i + 0xA9].mdlId, C->mdl[i + 0xA9].pos.x, 10.0f + C->mdl[i + 0xA9].pos.y,
                C->mdl[i + 0xA9].pos.z);
        }
        if (C->mdl0[i].charNo == 4) {
            Hu3DModelPosSet(C->mdl[i + 0xA9].mdlId, C->mdl[i + 0xA9].pos.x, C->mdl[i + 0xA9].pos.y,
                20.0f + C->mdl[i + 0xA9].pos.z);
        }
    }
}

void fn_1_73A4(void)
{
    fn_1_73A4_body();
}

void fn_1_7840(void)
{
    if (W.fogSet != 0) {
        if (W.fogOn != 0) {
            Hu3DFogSet(W.fogStart, W.fogEnd, W.fogR, W.fogG, W.fogB);
        } else {
            Hu3DFogClear();
        }
    }
    W.fogSet = 0;
}

void fn_1_78DC(void)
{
    fn_1_73A4_body();
    fn_1_964(0);
    fn_1_110C();
    fn_1_150C();
}

void fn_1_7D88(s16 player)
{
    HuVecF *p0 = &C->mdl[player + 0xA9].pos;
    HuVecF *p1 = &C->mdl[player + 0x21].pos;
    HuVecF *r0 = &C->mdl[player + 0xA9].rot;
    HuVecF *r1 = &C->mdl[player + 0x21].rot;
    f32 stk;
    f32 d;

    if (lbl_1_bss_120C[player].state != 0 && lbl_1_bss_120C[player].unk0E == 0) {
        memset(&C->mdl0[player].btn, 0, 0x14);
    }
    if (C->mdl0[player].stickX > 0) {
        lbl_1_bss_120C[player].stickX += 6.0f * ((f32)C->mdl0[player].stickX / 56.0f);
        if (lbl_1_bss_120C[player].stickX > 90.0f) {
            lbl_1_bss_120C[player].stickX = 90.0f;
        }
    } else if (C->mdl0[player].stickX < 0) {
        lbl_1_bss_120C[player].stickX += 6.0f * ((f32)C->mdl0[player].stickX / 56.0f);
        if (lbl_1_bss_120C[player].stickX < -90.0f) {
            lbl_1_bss_120C[player].stickX = -90.0f;
        }
    }
    if (C->mdl0[player].stickY > 0) {
        stk = 6.0f * ((f32)C->mdl0[player].stickY / 56.0f);
        if (lbl_1_bss_120C[player].hookX != 0) {
            lbl_1_bss_120C[player].stickY += stk * -1.0f;
            if (lbl_1_bss_120C[player].stickY < -90.0f) {
                lbl_1_bss_120C[player].stickY = -90.0f;
            }
        } else {
            lbl_1_bss_120C[player].stickY += stk;
            if (lbl_1_bss_120C[player].stickY > 90.0f) {
                lbl_1_bss_120C[player].stickY = 90.0f;
            }
        }
    } else if (C->mdl0[player].stickY < 0) {
        stk = 6.0f * ((f32)C->mdl0[player].stickY / 56.0f);
        if (lbl_1_bss_120C[player].hookX != 0) {
            lbl_1_bss_120C[player].stickY += stk * -1.0f;
            if (lbl_1_bss_120C[player].stickY > 90.0f) {
                lbl_1_bss_120C[player].stickY = 90.0f;
            }
        } else {
            lbl_1_bss_120C[player].stickY += stk;
            if (lbl_1_bss_120C[player].stickY < -90.0f) {
                lbl_1_bss_120C[player].stickY = -90.0f;
            }
        }
    }
    if (C->mdl0[player].stickX == 0) {
        if (lbl_1_bss_120C[player].stickX > 0.0f) {
            if (lbl_1_bss_120C[player].stickX >= 2.5f) {
                d = -2.5f;
            } else {
                d = -lbl_1_bss_120C[player].stickX;
            }
        } else if (lbl_1_bss_120C[player].stickX < 0.0f) {
            if (lbl_1_bss_120C[player].stickX <= -2.5f) {
                d = 2.5f;
            } else {
                d = -lbl_1_bss_120C[player].stickX;
            }
        } else {
            d = 0.0f;
        }
        lbl_1_bss_120C[player].stickX += d;
    }
    if (C->mdl0[player].stickY == 0) {
        if (lbl_1_bss_120C[player].stickY > 0.0f) {
            if (lbl_1_bss_120C[player].stickY >= 2.5f) {
                d = -2.5f;
            } else {
                d = -lbl_1_bss_120C[player].stickY;
            }
        } else if (lbl_1_bss_120C[player].stickY < 0.0f) {
            if (lbl_1_bss_120C[player].stickY <= -2.5f) {
                d = 2.5f;
            } else {
                d = -lbl_1_bss_120C[player].stickY;
            }
        } else {
            d = 0.0f;
        }
        lbl_1_bss_120C[player].stickY += d;
    }
    p0->x = p0->x + 10.0 * sin(3.141592653589793 * lbl_1_bss_120C[player].stickX / 180.0);
    p0->y = p0->y + 10.0 * sin(3.141592653589793 * lbl_1_bss_120C[player].stickY / 180.0);
    p1->x = p1->x + 10.0 * sin(3.141592653589793 * lbl_1_bss_120C[player].stickX / 180.0);
    p1->y = p1->y + 10.0 * sin(3.141592653589793 * lbl_1_bss_120C[player].stickY / 180.0);
    r0->z = -(30.0 * sin(3.141592653589793 * lbl_1_bss_120C[player].stickX / 180.0));
    r0->x = -(30.0 * sin(3.141592653589793 * lbl_1_bss_120C[player].stickY / 180.0));
    r1->z = -(30.0 * sin(3.141592653589793 * lbl_1_bss_120C[player].stickX / 180.0));
    r1->x = -(30.0 * sin(3.141592653589793 * lbl_1_bss_120C[player].stickY / 180.0));
}
