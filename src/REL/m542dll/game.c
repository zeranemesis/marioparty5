// m542dll/game.c - decompilation of the m542dll/REL/m542dll/game unit.

#define _MATH_H /* keeps MWCC from adding its 16 bytes of sqrt literals to .rodata */
#include "types.h"
#include "string.h"
#include "game/audio.h"
#include "game/charman.h"
#include "game/flag.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/wipe.h"
#include "game/mg/seqman.h"
#include "REL/m542dll/m542types.h"

double sin(double x);
double cos(double x);
s32 rand8(void);

void fn_1_3178(void);
void fn_1_328C(void);
void fn_1_3700(void);
void fn_1_37B0(s32 arg0);
void fn_1_39E0(void);
void fn_1_4008(s32 arg0);
s32 fn_1_4B50(s32 arg0);
void fn_1_4D48(s32 arg0);
void fn_1_771C(s32 arg0);
void fn_1_7778(s32 arg0);
void fn_1_77EC(void);
void fn_1_7848(s32 arg0);
void fn_1_7E90(s32 arg0);
s32 fn_1_800C(s32 arg0);
void fn_1_8180(s32 arg0);
s32 fn_1_835C(s32 arg0);

/* ---- data ---- */
s32 lbl_1_data_330 = -1;
s32 lbl_1_data_334[2] = { -1, -1 };

struct {
    f32 tbl[14][2];
    f32 pad;                   /* lbl_1_data_33C is 0x74 bytes long in the original */
} lbl_1_data_33C = { {
    { 0.0f, 0.0f }, { 0.0f, 0.25f }, { 0.0f, 0.5f }, { 0.0f, 0.75f },
    { 0.25f, 0.0f }, { 0.25f, 0.25f }, { 0.25f, 0.5f }, { 0.25f, 0.75f },
    { 0.5f, 0.0f }, { 0.5f, 0.25f }, { 0.5f, 0.5f }, { 0.0f, 0.0f },
    { 0.25f, 0.0f }, { 0.75f, 0.75f },
}, 0.0f };

HuVecF lbl_1_data_3B0[6] = {
    { 0.0f, -100.0f, 0.0f }, { 0.0f, -100.0f, 0.0f }, { 0.0f, -100.0f, 0.0f },
    { 0.0f, -100.0f, 0.0f }, { 0.0f, -100.0f, 0.0f }, { 0.0f, -100.0f, 0.0f },
};

/* ---- bss (declared in reverse: MWCC lays bss out in reverse order) ---- */
u8 lbl_1_bss_1B2A[0xe]; /* tail of the original lbl_1_bss_1B28 (0x10 bytes) */
s16 lbl_1_bss_1B28;
M542EMIT lbl_1_bss_1AA8[4];
M542WORK lbl_1_bss_1954;
s16 lbl_1_bss_1940[2][5];
M542ENTRY lbl_1_bss_1588[0x22];
M542PART lbl_1_bss_E8[4][30];

void fn_1_2858(void)
{
    if ((u8)WipeCheckEnd() == 0) {
        fn_1_3178();
        Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[7].model, 2.0f);
        Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[8].model, 2.0f);
        Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[9].model, 2.0f);
        Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[10].model, 2.0f);
        HuAudFXPlay(0x815);
        HuAudFXPlay(0x816);
        lbl_1_data_330 = HuAudBGMPlay(0x3e);
        lbl_1_bss_1954.mode++;
        lbl_1_bss_1954.timer = 0;
        CharModelVoicePanSet(lbl_1_bss_1954.player[0].charNo, 0x30);
        CharModelVoicePanSet(lbl_1_bss_1954.player[1].charNo, 0x50);
    }
}

void fn_1_2978(void)
{
    if (lbl_1_bss_1954.timer++ >= 45) {
        lbl_1_bss_1954.mode++;
        lbl_1_bss_1954.timer = 0;
        Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[0xf].model, 1.0f);
        Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[0x10].model, 1.0f);
    }
}

void fn_1_2A14(void)
{
    lbl_1_bss_1954.timer++;
    if (lbl_1_bss_1954.timer >= 10) {
        Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[0xf].model, 0.0f);
        Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[0x10].model, 0.0f);
    }
    if (lbl_1_bss_1954.timer >= 20) {
        omCameraViewMoveSimple(&lbl_1_bss_78[0], 60);
        lbl_1_bss_1954.mode++;
    }
}

void fn_1_2AD0(void)
{
    if (omCameraViewCheck(1) != 0) {
        lbl_1_bss_1954.mode = 0;
        MgSeqModeNext();
        fn_1_3178();
        lbl_1_bss_1954.wait[0] = (rand8() % 3) * 30 + (rand8() % 60) + 60;
        lbl_1_bss_1954.wait[1] = (rand8() % 3) * 30 + (rand8() % 60) + 60;
    }
}

void fn_1_2BDC(void)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        Hu3DMotionTimeSet(lbl_1_bss_1954.ents[i + 7].model, 0.0f);
        Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[i + 7].model, 2.0f);
        Hu3DMotionTimeSet(lbl_1_bss_1954.ents[i + 9].model, 0.0f);
        Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[i + 9].model, 2.0f);
        lbl_1_bss_1954.anim[i] = 0;
        lbl_1_bss_1954.gauge[i] = 10.0f;
        lbl_1_bss_1954.ang[i][0] = (rand8() % 2) * 180;
        lbl_1_bss_1954.ang[i][1] = (rand8() % 2) * 180;
        Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[i * 5 + 0x15].model, 1.0f);
        fn_1_771C(-1);
    }
    HuAudFXPlay(0x815);
    HuAudFXPlay(0x816);
    lbl_1_bss_1954.mode++;
    lbl_1_bss_4 = 1;
}

void fn_1_2DAC(void)
{
    s32 i;
    s32 s;

    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_1954.player[i].dif != 0) {
            fn_1_4D48(i);
        }
        fn_1_4008(i);
        if (lbl_1_bss_1954.st[i + 2] == 0) {
            if (lbl_1_bss_1954.wait[i]-- <= 0) {
                s = rand8() % 2;
                fn_1_7778(s + i * 2);
                if (rand8() % 3 != 0) {
                    fn_1_7778((s ^ 1) + i * 2);
                }
                lbl_1_bss_1954.wait[i] = (rand8() % 3) * 15 + 0x42;
                if (i == 0) {
                    HuAudFXPlay(0x813);
                } else {
                    HuAudFXPlay(0x814);
                }
            }
        }
    }
    if (lbl_1_bss_1954.fall[0] != 0 && lbl_1_bss_1954.fall[1] != 0) {
        lbl_1_bss_1954.mode++;
        MgSeqModeNext();
        HuAudSStreamFadeOut(lbl_1_data_330, 1000);
        lbl_1_bss_4 = 0;
    }
    fn_1_39E0();
    lbl_1_bss_C++;
}

void fn_1_2F90(void)
{
    lbl_1_bss_1954.mode = 0;
    MgSeqModeNext();
    HuAudSStreamFadeOut(lbl_1_data_330, 100);
    fn_1_39E0();
}

void fn_1_2FD8(s32 arg0, s32 arg1)
{
    s32 digit[4];
    s32 i;

    if (arg1 != 9999) {
        digit[0] = arg1 / 1000;
        digit[1] = (arg1 % 1000) / 100;
        digit[2] = (arg1 % 100) / 10;
        digit[3] = arg1 % 10;
    } else {
        for (i = 0; i < 4; i++) {
            digit[i] = 10;
        }
    }
    for (i = 0; i < 4; i++) {
        Hu3DTexScrollPosSet(lbl_1_bss_1940[arg0][i], lbl_1_data_33C.tbl[digit[i]][0], lbl_1_data_33C.tbl[digit[i]][1], 0.0f);
    }
}

void fn_1_3178(void)
{
    s32 digit[4];
    s32 j;
    s32 i;

    for (i = 0; i < 2; i++) {
        digit[0] = 1;
        digit[1] = 0;
        digit[2] = 0;
        digit[3] = 0;
        for (j = 0; j < 4; j++) {
            Hu3DTexScrollPosSet(lbl_1_bss_1940[i][j], lbl_1_data_33C.tbl[digit[j]][0], lbl_1_data_33C.tbl[digit[j]][1], 0.0f);
        }
        lbl_1_bss_1954.st[i + 2] = 0;
        lbl_1_bss_1954.power[i] = 0;
    }
}

void fn_1_328C(void)
{
    s32 digitC[4];
    s32 digitB[4];
    s32 digitA[4];
    s32 val;
    s32 i;
    s32 j;
    s32 l;
    s32 k;

    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_1954.st[i + 2] == 0) {
            lbl_1_bss_1954.power[i]++;
            if (lbl_1_bss_1954.power[i] > 600) {
                lbl_1_bss_1954.power[i] = 601;
                lbl_1_bss_1954.st[i + 2] = 1;
                if (i == 0) {
                    HuAudFXPlay(0x80e);
                } else {
                    HuAudFXPlay(0x80f);
                }
                Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[i * 5 + 0x15].model, 0.0f);
                Hu3DMotionTimeSet(lbl_1_bss_1954.ents[i * 5 + 0x15].model, 30.0f);
            }
        }
    }
    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_1954.power[i] == 600) {
            digitC[0] = 0;
            digitC[1] = 0;
            digitC[2] = 0;
            digitC[3] = 1;
            for (j = 0; j < 4; j++) {
                Hu3DTexScrollPosSet(lbl_1_bss_1940[i][j], lbl_1_data_33C.tbl[digitC[j]][0], lbl_1_data_33C.tbl[digitC[j]][1], 0.0f);
            }
        } else if (lbl_1_bss_1954.power[i] < 600) {
            val = 1000 - (lbl_1_bss_1954.power[i] * 1000) / 600;
            if (val != 9999) {
                digitB[0] = val / 1000;
                digitB[1] = (val % 1000) / 100;
                digitB[2] = (val % 100) / 10;
                digitB[3] = val % 10;
            } else {
                for (k = 0; k < 4; k++) {
                    digitB[k] = 10;
                }
            }
            for (k = 0; k < 4; k++) {
                Hu3DTexScrollPosSet(lbl_1_bss_1940[i][k], lbl_1_data_33C.tbl[digitB[k]][0], lbl_1_data_33C.tbl[digitB[k]][1], 0.0f);
            }
        } else {
            for (l = 0; l < 4; l++) {
                digitA[l] = 10;
            }
            for (l = 0; l < 4; l++) {
                Hu3DTexScrollPosSet(lbl_1_bss_1940[i][l], lbl_1_data_33C.tbl[digitA[l]][0], lbl_1_data_33C.tbl[digitA[l]][1], 0.0f);
            }
        }
    }
}

void fn_1_3700(void)
{
    s32 i;
    M542ENTRY *chr;
    M542ENTRY *spot;

    for (i = 0; i < 2; i++) {
        chr = &lbl_1_bss_1954.ents[i + 0x20];
        spot = &lbl_1_bss_1954.ents[i + 0xb];
        if (chr->pos.y >= 170.0f) {
            spot->pos.y = 0.0f;
        } else {
            spot->pos.y = chr->pos.y - 170.0f;
        }
    }
}

void fn_1_37B0(s32 arg0)
{
    lbl_1_bss_1954.ang[arg0][0] = (lbl_1_bss_1954.ang[arg0][0] + (rand8() % 10) + 2) % 360;
    lbl_1_bss_1954.ents[arg0 + 0xf].rot.x = 2.0 * sin(3.141592653589793 * (f64)lbl_1_bss_1954.ang[arg0][0] / 180.0);
    lbl_1_bss_1954.ang[arg0][1] = (lbl_1_bss_1954.ang[arg0][1] + (rand8() % 10) + 2) % 360;
    lbl_1_bss_1954.ents[arg0 + 0xf].rot.z = 2.0 * sin(3.141592653589793 * (f64)lbl_1_bss_1954.ang[arg0][0] / 180.0);
}

void fn_1_39E0(void)
{
    s32 i;

    fn_1_3700();
    fn_1_328C();
    fn_1_37B0(0);
    fn_1_37B0(1);
    fn_1_77EC();
    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_1954.anim[i]++ >= 50) {
            lbl_1_bss_1954.anim[i] = 0;
            if (lbl_1_bss_1954.st[i + 2] == 0) {
                Hu3DMotionTimeSet(lbl_1_bss_1954.ents[i + 7].model, 0.0f);
                Hu3DMotionTimeSet(lbl_1_bss_1954.ents[i + 9].model, 0.0f);
                if (i == 0) {
                    HuAudFXPlay(0x815);
                } else {
                    HuAudFXPlay(0x816);
                }
            }
        }
        if ((lbl_1_bss_1954.st[i + 2] == 0) | (lbl_1_bss_1954.power[i] > 600)) {
            lbl_1_bss_1954.gauge[i] += 0.1;
            if (lbl_1_bss_1954.gauge[i] > 85.0f) {
                lbl_1_bss_1954.gauge[i] = 85.0f;
            }
            Hu3DMotionTimeSet(lbl_1_bss_1954.ents[i + 0xf].model, lbl_1_bss_1954.gauge[i]);
        }
    }
}

void fn_1_4008(s32 arg0)
{
    M542ENTRY *chr;

    chr = &lbl_1_bss_1954.ents[arg0 + 0x20];
    switch (lbl_1_bss_1954.jump[arg0].state) {
    case 0:
        if (lbl_1_bss_1954.st[arg0 + 2] == 0 && (lbl_1_bss_1954.player[arg0].btnDown & 0x100) != 0) {
            lbl_1_bss_1954.jump[arg0].state = 1;
            lbl_1_bss_1954.jump[arg0].cnt = 0;
            Hu3DMotionShiftSet(chr->model, lbl_1_bss_1954.motId[arg0][1], 0.0f, 2.0f, 0);
            lbl_1_bss_1954.vel[arg0][0] = 25.0f;
            return;
        }
        return;
    case 1:
        lbl_1_bss_1954.jump[arg0].cnt++;
        chr->pos.y += lbl_1_bss_1954.vel[arg0][0];
        lbl_1_bss_1954.vel[arg0][0] *= 0.85;
        if (lbl_1_bss_1954.vel[arg0][0] < 1.0f) {
            lbl_1_bss_1954.vel[arg0][0] = 1.0f;
        }
        if ((lbl_1_bss_1954.player[arg0].btn & 0x100) == 0 && lbl_1_bss_1954.jump[arg0].cnt > 4) {
            lbl_1_bss_1954.jump[arg0].state = 2;
            lbl_1_bss_1954.vel[arg0][0] *= 0.5f;
        }
        if (lbl_1_bss_1954.jump[arg0].cnt > 16) {
            lbl_1_bss_1954.jump[arg0].state = 2;
            lbl_1_bss_1954.vel[arg0][0] *= 0.5f;
        }
        if (lbl_1_bss_1954.jump[arg0].cnt > 3 && lbl_1_bss_1954.st[arg0 + 2] == 0
            && ((lbl_1_bss_1954.player[arg0].btnDown & 0x100) != 0 || (lbl_1_bss_1954.player[arg0].btnDown & 0x40) != 0)) {
            lbl_1_bss_1954.jump[arg0].state = 3;
            Hu3DMotionShiftSet(chr->model, lbl_1_bss_1954.motId[arg0][3], 0.0f, 8.0f, 0);
            lbl_1_bss_1954.jump[arg0].cnt = 0;
            lbl_1_bss_1954.vel[arg0][0] *= 0.5f;
            return;
        }
        break;
    case 2:
        chr->pos.y -= lbl_1_bss_1954.vel[arg0][0];
        lbl_1_bss_1954.vel[arg0][0] *= 1.1764705f;
        if (lbl_1_bss_1954.vel[arg0][0] > 30.0f) {
            lbl_1_bss_1954.vel[arg0][0] = 30.0f;
        }
        if (chr->pos.y <= 170.0f) {
            chr->pos.y = 170.0f;
            lbl_1_bss_1954.jump[arg0].state = 0;
            Hu3DMotionShiftSet(chr->model, lbl_1_bss_1954.motId[arg0][0], 0.0f, 5.0f, 0);
            lbl_1_bss_1954.vel[arg0][0] = 25.0f;
            lbl_1_bss_1954.jump[arg0].cnt = 0;
            CharModelLandDustCreate(lbl_1_bss_1954.player[arg0].charNo, &chr->pos);
            return;
        }
        if (lbl_1_bss_1954.st[arg0 + 2] == 0
            && ((lbl_1_bss_1954.player[arg0].btnDown & 0x100) != 0 || (lbl_1_bss_1954.player[arg0].btnDown & 0x40) != 0)) {
            lbl_1_bss_1954.jump[arg0].state = 3;
            Hu3DMotionShiftSet(chr->model, lbl_1_bss_1954.motId[arg0][3], 0.0f, 8.0f, 0);
            lbl_1_bss_1954.jump[arg0].cnt = 0;
            lbl_1_bss_1954.vel[arg0][0] *= 0.5f;
            return;
        }
        break;
    case 3:
        if (lbl_1_bss_1954.jump[arg0].cnt++ > 15) {
            chr->pos.y -= lbl_1_bss_1954.vel[arg0][0];
            lbl_1_bss_1954.vel[arg0][0] *= 1.1764705f;
            if (lbl_1_bss_1954.vel[arg0][0] > 45.0f) {
                lbl_1_bss_1954.vel[arg0][0] = 45.0f;
            }
            if (chr->pos.y <= 150.0f) {
                chr->pos.y = 150.0f;
                if (lbl_1_bss_1954.st[arg0 + 2] == 0) {
                    if (arg0 == 0) {
                        HuAudFXPlay(0x80c);
                    } else {
                        HuAudFXPlay(0x80d);
                    }
                }
                Hu3DMotionShiftSet(chr->model, lbl_1_bss_1954.motId[arg0][5], 0.0f, 4.0f, 0);
                lbl_1_bss_1954.st[arg0 + 2] = 1;
                Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[arg0 * 5 + 0x15].model, 0.0f);
                Hu3DMotionTimeSet(lbl_1_bss_1954.ents[arg0 * 5 + 0x15].model, 30.0f);
                lbl_1_bss_1954.jump[arg0].state = 4;
                lbl_1_bss_1954.jump[arg0].cnt = 0;
                omVibrate(lbl_1_bss_1954.player[arg0].playerNo, 20, 4, 2);
                return;
            }
        }
        break;
    case 4:
        if (lbl_1_bss_1954.jump[arg0].cnt++ > 15) {
            Hu3DMotionShiftSet(chr->model, lbl_1_bss_1954.motId[arg0][4], 0.0f, 4.0f, 0);
            lbl_1_bss_1954.jump[arg0].state = 5;
            lbl_1_bss_1954.jump[arg0].cnt = 0;
        }
        /* fallthrough */
    case 5:
        if (lbl_1_bss_1954.jump[arg0].cnt++ > 30) {
            Hu3DMotionShiftSet(chr->model, lbl_1_bss_1954.motId[arg0][0], 0.0f, 2.0f, 0x40000001);
            lbl_1_bss_1954.jump[arg0].state = 6;
        }
        break;
    }
}

void fn_1_4994(void)
{
    s32 tbl[4] = { 0x1b6, 0x1f8, 0x210, 0x21c };
    s32 i;

    for (i = 0; i < 2; i++) {
        memset(&lbl_1_bss_14.ai[i], 0, sizeof(M542AI));
        lbl_1_bss_14.ai[i].kind = lbl_1_bss_1954.player[i].dif - 1;
#ifdef TARGET_PC
        /* human players have dif == 0, so kind is -1 here and the original reads tbl[-1] */
        lbl_1_bss_14.ai[i].target = tbl[lbl_1_bss_14.ai[i].kind < 0 ? 0 : lbl_1_bss_14.ai[i].kind];
#else
        lbl_1_bss_14.ai[i].target = tbl[lbl_1_bss_14.ai[i].kind];
#endif
        if (lbl_1_bss_14.ai[i].kind == 3) {
            lbl_1_bss_14.ai[i].target -= rand8() % 3;
        } else {
            lbl_1_bss_14.ai[i].target -= rand8() % 18;
        }
        lbl_1_bss_14.ai[i].wait = rand8() % 120 + 120;
    }
}

s32 fn_1_4B50(s32 arg0)
{
    s32 ret;

    ret = 0;
    if (lbl_1_bss_1954.power[arg0] >= lbl_1_bss_14.ai[arg0].target) {
        lbl_1_bss_14.ai[arg0].holdMax = 15 - rand8() % 6;
        lbl_1_bss_14.ai[arg0].holdTime = lbl_1_bss_14.ai[arg0].holdMax + rand8() % 3 + 5;
        ret = 1;
    } else if (lbl_1_bss_1954.power[arg0] < lbl_1_bss_14.ai[arg0].target - 45) {
        lbl_1_bss_14.ai[arg0].wait--;
        if (lbl_1_bss_14.ai[arg0].wait <= 0) {
            lbl_1_bss_14.ai[arg0].holdMax = rand8() % 6 + 10;
            lbl_1_bss_14.ai[arg0].holdTime = 0;
            lbl_1_bss_14.ai[arg0].wait = rand8() % 60 + 60;
            ret = 1;
        }
    }
    return ret;
}

void fn_1_4D48(s32 arg0)
{
    switch (lbl_1_bss_14.ai[arg0].step) {
    case 0:
        if (fn_1_4B50(arg0) != 0) {
            lbl_1_bss_14.ai[arg0].timer = 0;
            lbl_1_bss_14.ai[arg0].step++;
            lbl_1_bss_1954.player[arg0].btnDown = 0x100;
            lbl_1_bss_1954.player[arg0].btn = 0x100;
        }
        break;
    case 1:
        lbl_1_bss_1954.player[arg0].btn = 0x100;
        lbl_1_bss_14.ai[arg0].timer++;
        if (lbl_1_bss_14.ai[arg0].timer >= lbl_1_bss_14.ai[arg0].holdMax) {
            if (lbl_1_bss_14.ai[arg0].holdTime != 0) {
                lbl_1_bss_14.ai[arg0].step++;
            } else {
                lbl_1_bss_14.ai[arg0].step = 0;
            }
        }
        break;
    case 2:
        lbl_1_bss_14.ai[arg0].timer++;
        if (lbl_1_bss_14.ai[arg0].timer == lbl_1_bss_14.ai[arg0].holdTime) {
            lbl_1_bss_1954.player[arg0].btnDown = 0x100;
            lbl_1_bss_14.ai[arg0].step++;
        }
        break;
    }
}

void fn_1_5120(void)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        fn_1_4008(i);
        fn_1_37B0(i);
        fn_1_3700();
    }
    lbl_1_bss_1954.mode = 0;
    fn_1_77EC();
}

void fn_1_5420(void)
{
    s32 i;
    s32 cnt;

    for (i = 0, cnt = 0; i < 2; i++) {
        fn_1_4008(i);
        if (lbl_1_bss_1954.jump[i].state == 0) {
            cnt++;
        }
        if (lbl_1_bss_1954.jump[i].state == 6) {
            cnt++;
        }
    }
    if (cnt == 2) {
        if (lbl_1_bss_1954.power[0] == lbl_1_bss_1954.power[1]) {
            lbl_1_bss_1954.mode = 2;
            lbl_1_bss_1954.timer = 0;
            if (lbl_1_bss_1954.power[0] > 600) {
                lbl_1_bss_1954.timer = 1;
            }
            for (i = 0; i < 2; i++) {
                lbl_1_bss_1954.anim[i] = 0;
                Hu3DMotionTimeSet(lbl_1_bss_1954.ents[i + 7].model, 0.0f);
                Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[i + 7].model, 3.0f);
                Hu3DMotionTimeSet(lbl_1_bss_1954.ents[i + 9].model, 0.0f);
                Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[i + 9].model, 3.0f);
                lbl_1_bss_1954.st[i + 2] = 0;
                Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[i * 5 + 0x15].model, 2.0f);
                Hu3DMotionShiftSet(lbl_1_bss_1954.ents[i + 0x20].model, lbl_1_bss_1954.motId[i][8], 0.0f, 10.0f, 0);
            }
            HuAudFXPlay(0x815);
            HuAudFXPlay(0x816);
            lbl_1_data_334[0] = HuAudFXPlay(0x817);
            lbl_1_data_334[1] = HuAudFXPlay(0x818);
            HuAudFXPlay(0x1d);
        } else {
            lbl_1_bss_1954.mode = 1;
            if (lbl_1_bss_1954.power[0] > lbl_1_bss_1954.power[1]) {
                if (lbl_1_bss_1954.power[0] > 600) {
                    lbl_1_bss_1954.timer = 1;
                } else {
                    lbl_1_bss_1954.timer = 0;
                }
            } else if (lbl_1_bss_1954.power[1] > 600) {
                lbl_1_bss_1954.timer = 0;
            } else {
                lbl_1_bss_1954.timer = 1;
            }
            lbl_1_bss_1954.anim[lbl_1_bss_1954.timer ^ 1] = 0;
            Hu3DMotionTimeSet(lbl_1_bss_1954.ents[(lbl_1_bss_1954.timer ^ 1) + 7].model, 0.0f);
            Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[(lbl_1_bss_1954.timer ^ 1) + 7].model, 3.0f);
            Hu3DMotionTimeSet(lbl_1_bss_1954.ents[(lbl_1_bss_1954.timer ^ 1) + 9].model, 0.0f);
            Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[(lbl_1_bss_1954.timer ^ 1) + 9].model, 3.0f);
            if (lbl_1_bss_1954.timer == 0) {
                HuAudFXPlay(0x816);
                lbl_1_data_334[1] = HuAudFXPlay(0x818);
            } else {
                HuAudFXPlay(0x815);
                lbl_1_data_334[0] = HuAudFXPlay(0x817);
            }
            HuAudFXPlay(0x1d);
            lbl_1_bss_1954.st[(lbl_1_bss_1954.timer ^ 1) + 2] = 0;
            Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[(lbl_1_bss_1954.timer ^ 1) * 5 + 0x15].model, 2.0f);
            Hu3DMotionShiftSet(lbl_1_bss_1954.ents[(lbl_1_bss_1954.timer ^ 1) + 0x20].model,
                               lbl_1_bss_1954.motId[lbl_1_bss_1954.timer ^ 1][8], 0.0f, 10.0f, 0);
        }
        lbl_1_bss_1954.phase = 0;
    }
    fn_1_37B0(0);
    fn_1_37B0(1);
}

void fn_1_5D30(void)
{
    s32 w;
    s16 pl;

    w = lbl_1_bss_1954.timer ^ 1;
    if (lbl_1_bss_1954.anim[w]++ >= 33) {
        lbl_1_bss_1954.anim[w] = 0;
        Hu3DMotionTimeSet(lbl_1_bss_1954.ents[w + 7].model, 0.0f);
        Hu3DMotionTimeSet(lbl_1_bss_1954.ents[w + 9].model, 0.0f);
        if (lbl_1_bss_1954.phase < 1) {
            if (w == 0) {
                HuAudFXPlay(0x815);
            } else {
                HuAudFXPlay(0x816);
            }
        }
    }
    switch (lbl_1_bss_1954.phase) {
    case 0:
        lbl_1_bss_1954.gauge[w] += 0.2;
        Hu3DMotionTimeSet(lbl_1_bss_1954.ents[w + 0xf].model, lbl_1_bss_1954.gauge[w]);
        if (lbl_1_bss_1954.gauge[w] > 99.0f) {
            lbl_1_bss_1954.gauge[w] = 99.0f;
            lbl_1_bss_1954.phase++;
            Hu3DMotionShiftSet(lbl_1_bss_1954.ents[w + 0x20].model, lbl_1_bss_1954.motId[w][7], 0.0f, 4.0f, 0);
            omVibrate(lbl_1_bss_1954.player[w].playerNo, 80, 12, 0);
            Hu3DModelAttrSet(lbl_1_bss_1954.ents[w + 0xf].model, 1);
            Hu3DModelAttrReset(lbl_1_bss_1954.ents[w + 0x1b].model, 1);
            Hu3DMotionTimeSet(lbl_1_bss_1954.ents[w + 0x1b].model, 0.0f);
            Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[w + 0x1b].model, 4.0f);
            Hu3DModelAttrReset(lbl_1_bss_1954.ents[w + 0x1b].model, 0x40000001);
            Hu3DModelAttrSet(lbl_1_bss_1954.ents[w + 0xf].model, 1);
            Hu3DModelAttrReset(lbl_1_bss_1954.ents[w + 0x1e].model, 1);
            Hu3DMotionTimeSet(lbl_1_bss_1954.ents[w + 0x1e].model, 0.0f);
            Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[w + 0x1e].model, 4.0f);
            HuAudFXPlay(0x1e);
            if (w == 0) {
                HuAudFXStop(lbl_1_data_334[0]);
                HuAudFXPlay(0x810);
            } else {
                HuAudFXStop(lbl_1_data_334[1]);
                HuAudFXPlay(0x811);
            }
            fn_1_7778(w + 4);
        }
        break;
    case 1:
        lbl_1_bss_1954.ents[w + 0x20].pos.z += 30.0f;
        lbl_1_bss_1954.ents[w + 0x20].pos.y += 6.0;
        lbl_1_bss_1954.ents[w + 0x20].pos.x += (f32)(6 - w * 12);
        if (lbl_1_bss_1954.ents[w + 0x20].pos.z >= 2000.0f) {
            lbl_1_bss_1954.phase++;
            lbl_1_bss_78[0].center.x = (f32)(lbl_1_bss_1954.timer * 0x1a4 - 0xd2);
            lbl_1_bss_78[0].center.z = 254.0f;
            lbl_1_bss_78[0].rot.x = -5.0f;
            lbl_1_bss_78[0].zoom = 600.0f;
            omCameraViewMoveSimple(&lbl_1_bss_78[0], 60);
        }
        break;
    case 2:
        if (omCameraViewCheck(1) != 0) {
            CharModelVoicePanSet(lbl_1_bss_1954.player[0].charNo, 0x40);
            CharModelVoicePanSet(lbl_1_bss_1954.player[1].charNo, 0x40);
            lbl_1_bss_1B28 = GameMesCreate(4, 3, lbl_1_bss_1954.player[lbl_1_bss_1954.timer].charNo, -1, -1, -1);
            Hu3DMotionShiftSet(lbl_1_bss_1954.ents[lbl_1_bss_1954.timer + 0x20].model,
                               lbl_1_bss_1954.motId[lbl_1_bss_1954.timer][6], 0.0f, 8.0f, 0);
            pl = lbl_1_bss_1954.player[lbl_1_bss_1954.timer].playerNo;
            if (_CheckFlag(FLAG_MG_PRACTICE) == 0) {
                GWMgCoinSet(pl, 10);
            }
            HuAudJinglePlay(0x79);
            lbl_1_bss_1954.phase++;
            lbl_1_bss_1954.f_c0 = 0;
        }
        break;
    case 3:
        lbl_1_bss_1954.f_c0++;
        if (GameMesStatGet(lbl_1_bss_1B28) == 0 && lbl_1_bss_1954.f_c0 >= 210) {
            GameMesKill(lbl_1_bss_1B28);
            MgSeqModeNext();
        }
        break;
    }
    fn_1_37B0(0);
    fn_1_37B0(1);
    fn_1_37B0(lbl_1_bss_1954.timer ^ 1);
    fn_1_328C();
    fn_1_328C();
    fn_1_77EC();
}

void fn_1_6A30(void)
{
    s32 i;
    s32 cnt;

    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_1954.anim[i]++ >= 33) {
            lbl_1_bss_1954.anim[i] = 0;
            Hu3DMotionTimeSet(lbl_1_bss_1954.ents[i + 7].model, 0.0f);
            Hu3DMotionTimeSet(lbl_1_bss_1954.ents[i + 9].model, 0.0f);
            if (lbl_1_bss_1954.phase < 1) {
                if (i == 0) {
                    HuAudFXPlay(0x815);
                } else {
                    HuAudFXPlay(0x816);
                }
            }
        }
    }
    switch (lbl_1_bss_1954.phase) {
    case 0:
        cnt = 0;
        for (i = 0; i < 2; i++) {
            lbl_1_bss_1954.gauge[i] += 0.2;
            if (lbl_1_bss_1954.gauge[i] > 99.0f) {
                lbl_1_bss_1954.gauge[i] = 99.0f;
                cnt++;
            }
            Hu3DMotionTimeSet(lbl_1_bss_1954.ents[i + 0xf].model, lbl_1_bss_1954.gauge[i]);
        }
        if (cnt == 2) {
            lbl_1_bss_1954.phase++;
            for (i = 0; i < 2; i++) {
                Hu3DMotionShiftSet(lbl_1_bss_1954.ents[i + 0x20].model, lbl_1_bss_1954.motId[i][7], 0.0f, 8.0f, 0);
                omVibrate(lbl_1_bss_1954.player[i].playerNo, 80, 12, 0);
                Hu3DModelAttrSet(lbl_1_bss_1954.ents[i + 0xf].model, 1);
                Hu3DModelAttrReset(lbl_1_bss_1954.ents[i + 0x1b].model, 1);
                Hu3DMotionTimeSet(lbl_1_bss_1954.ents[i + 0x1b].model, 0.0f);
                Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[i + 0x1b].model, 4.0f);
                Hu3DModelAttrReset(lbl_1_bss_1954.ents[i + 0x1b].model, 0x40000001);
                Hu3DModelAttrReset(lbl_1_bss_1954.ents[i + 0x1e].model, 1);
                Hu3DMotionTimeSet(lbl_1_bss_1954.ents[i + 0x1e].model, 0.0f);
                Hu3DMotionSpeedSet(lbl_1_bss_1954.ents[i + 0x1e].model, 4.0f);
                fn_1_7778(i + 4);
            }
            HuAudFXPlay(0x1e);
            HuAudFXStop(lbl_1_data_334[0]);
            HuAudFXStop(lbl_1_data_334[1]);
            HuAudFXPlay(0x810);
            HuAudFXPlay(0x811);
        }
        break;
    case 1:
        for (i = 0; i < 2; i++) {
            lbl_1_bss_1954.ents[i + 0x20].pos.z += 30.0f;
            lbl_1_bss_1954.ents[i + 0x20].pos.y += 6.0;
            lbl_1_bss_1954.ents[i + 0x20].pos.x += (f32)(6 - i * 12);
        }
        if (lbl_1_bss_1954.ents[0x20].pos.z >= 2000.0f) {
            lbl_1_bss_1954.phase++;
        }
        break;
    case 2:
        lbl_1_bss_1B28 = GameMesCreate(2, 2);
        HuAudJinglePlay(0x7b);
        lbl_1_bss_1954.phase++;
        lbl_1_bss_1954.f_c0 = 0;
        break;
    case 3:
        lbl_1_bss_1954.f_c0++;
        if (GameMesStatGet(lbl_1_bss_1B28) == 0 && lbl_1_bss_1954.f_c0 >= 210) {
            GameMesKill(lbl_1_bss_1B28);
            MgSeqModeNext();
        }
        break;
    }
    for (i = 0; i < 2; i++) {
        fn_1_37B0(0);
        fn_1_37B0(1);
    }
    fn_1_328C();
    fn_1_328C();
    fn_1_77EC();
}

void fn_1_737C(void)
{
    fn_1_77EC();
    fn_1_37B0(0);
    fn_1_37B0(1);
}

void fn_1_771C(s32 arg0)
{
    s32 i;
    s32 lim;

    i = arg0;
    lim = i + 1;
    if (i < 0) {
        i = 0;
        lim = 4;
    }
    for (; i < lim; i++) {
#ifdef TARGET_PC
        lbl_1_bss_1AA8[i].step = 0;
#else
        lbl_1_bss_1AA8[arg0].step = 0;
#endif
    }
}

void fn_1_7778(s32 arg0)
{
    if (arg0 < 4) {
        lbl_1_bss_1AA8[arg0].state = 1;
        lbl_1_bss_1AA8[arg0].step = 0;
    } else {
        lbl_1_bss_1AA8[arg0 - 4].state = 1;
        lbl_1_bss_1AA8[arg0 - 4].step = arg0;
    }
}

void fn_1_77EC(void)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_1AA8[i].state != 0) {
            fn_1_7848(i);
        }
    }
}

void fn_1_7848(s32 arg0)
{
    switch (lbl_1_bss_1AA8[arg0].step) {
    case 0:
        lbl_1_bss_1AA8[arg0].cnt = 0;
        lbl_1_bss_1AA8[arg0].idx = 0;
        lbl_1_bss_1AA8[arg0].step++;
        switch (arg0) {
        case 2:
        case 0:
            lbl_1_bss_1AA8[arg0].pos.x = -400 - ((rand8() * 16) % 6);
            lbl_1_bss_1AA8[arg0].pos.y = 170 - (rand8() % 40);
            lbl_1_bss_1AA8[arg0].pos.z = 400.0f;
            lbl_1_bss_1AA8[arg0].ang = 50 - (rand8() % 30);
            break;
        case 3:
            lbl_1_bss_1AA8[arg0].pos.x = -500 - ((rand8() * 16) % 6);
            lbl_1_bss_1AA8[arg0].pos.y = (rand8() % 20) + 300;
            lbl_1_bss_1AA8[arg0].pos.z = 200.0f;
            lbl_1_bss_1AA8[arg0].ang = -(20 - (rand8() % 10));
            break;
        }
        if (arg0 >= 2) {
            lbl_1_bss_1AA8[arg0].pos.x = -lbl_1_bss_1AA8[arg0].pos.x;
            if (arg0 == 2) {
                lbl_1_bss_1AA8[arg0].ang = 180.0f - lbl_1_bss_1AA8[arg0].ang;
            } else {
                lbl_1_bss_1AA8[arg0].ang = -180.0f - lbl_1_bss_1AA8[arg0].ang;
            }
        }
        break;
    case 1:
        lbl_1_bss_1AA8[arg0].cnt++;
        if (lbl_1_bss_1AA8[arg0].cnt >= 30) {
            lbl_1_bss_1AA8[arg0].step++;
            return;
        }
        fn_1_7E90(arg0);
        fn_1_800C(arg0);
        break;
    case 2:
        if (fn_1_800C(arg0) == 0) {
            lbl_1_bss_1AA8[arg0].step++;
            return;
        }
        break;
    case 3:
        lbl_1_bss_1AA8[arg0].state = 0;
        break;
    default:
        lbl_1_bss_1AA8[arg0].cnt = 0;
        lbl_1_bss_1AA8[arg0].idx = 0;
        lbl_1_bss_1AA8[arg0].pos.x = (lbl_1_bss_1AA8[arg0].step - 4) * 0x244 - 0x122;
        lbl_1_bss_1AA8[arg0].pos.y = 132.0f;
        lbl_1_bss_1AA8[arg0].pos.z = -128.0f;
        lbl_1_bss_1AA8[arg0].ang = 90.0f;
        lbl_1_bss_1AA8[arg0].step = 6;
        break;
    case 6:
        lbl_1_bss_1AA8[arg0].cnt++;
        if (lbl_1_bss_1AA8[arg0].cnt % 5 == 0) {
            fn_1_8180(arg0);
        }
        fn_1_835C(arg0);
        break;
    }
}

void fn_1_7E90(s32 arg0)
{
    M542PART *p;

    p = &lbl_1_bss_E8[arg0][lbl_1_bss_1AA8[arg0].idx];
    p->active++;
    p->ang = (rand8() % 48 - 24) / 4;
    p->cnt = 0;
    Hu3DModelAttrReset(p->model, 1);
    Hu3DMotionTimeSet(p->model, 0.0f);
    Hu3DMotionSpeedSet(p->model, 3.33f);
    Hu3DModelPosSetV(p->model, &lbl_1_bss_1AA8[arg0].pos);
    p->pos.x = lbl_1_bss_1AA8[arg0].pos.x;
    p->pos.y = lbl_1_bss_1AA8[arg0].pos.y;
    p->pos.z = lbl_1_bss_1AA8[arg0].pos.z;
    lbl_1_bss_1AA8[arg0].idx++;
}

s32 fn_1_800C(s32 arg0)
{
    f32 spd = 13.5f;
    M542PART *p;
    s32 ret;
    s32 i;

    ret = 0;
    p = lbl_1_bss_E8[arg0];
    for (i = 0; i < 30; i++, p++) {
        if (p->active != 0) {
            if (p->cnt < 29) {
                p->pos.x += spd * cos(3.141592653589793 * (f64)(p->ang + lbl_1_bss_1AA8[arg0].ang) / 180.0);
                p->pos.y += spd * sin(3.141592653589793 * (f64)(p->ang + lbl_1_bss_1AA8[arg0].ang) / 180.0);
                p->cnt++;
            } else {
                p->active = 0;
                Hu3DModelAttrSet(p->model, 1);
            }
            ret++;
        }
    }
    return ret;
}

void fn_1_8180(s32 arg0)
{
    M542PART *p;

    p = &lbl_1_bss_E8[arg0][lbl_1_bss_1AA8[arg0].idx];
    p->active++;
    p->ang = (rand8() % 24 - 12) / 4;
    p->cnt = 0;
    Hu3DModelAttrReset(p->model, 1);
    Hu3DMotionTimeSet(p->model, 0.0f);
    Hu3DMotionSpeedSet(p->model, 3.3f);
    Hu3DModelScaleSet(p->model, 0.3f, 0.3f, 1.0f);
    Hu3DModelPosSetV(p->model, &lbl_1_bss_1AA8[arg0].pos);
    p->pos.x = lbl_1_bss_1AA8[arg0].pos.x;
    p->pos.y = lbl_1_bss_1AA8[arg0].pos.y;
    p->pos.z = lbl_1_bss_1AA8[arg0].pos.z;
    lbl_1_bss_1AA8[arg0].idx = (lbl_1_bss_1AA8[arg0].idx + 1) % 30;
}

s32 fn_1_835C(s32 arg0)
{
    f32 spd = 4.0f;
    M542PART *p;
    s32 ret;
    s32 i;

    ret = 0;
    p = lbl_1_bss_E8[arg0];
    for (i = 0; i < 30; i++, p++) {
        if (p->active != 0) {
            if (p->cnt < 30) {
                p->pos.x += spd * cos(3.141592653589793 * (f64)(p->ang + lbl_1_bss_1AA8[arg0].ang) / 180.0);
                p->pos.y += spd * sin(3.141592653589793 * (f64)(p->ang + lbl_1_bss_1AA8[arg0].ang) / 180.0);
                p->cnt++;
            } else {
                p->active = 0;
                Hu3DModelAttrSet(p->model, 1);
            }
            ret++;
        }
    }
    return ret;
}
