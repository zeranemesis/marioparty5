// m550dll/game.c - decompilation of the m550dll/game unit.

#include "types.h"

#include "game/audio.h"
#include "game/charman.h"
#include "game/flag.h"
#include "game/gamemes.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"
#include "game/object.h"
#include "game/wipe.h"

#include "REL/m550dll.h"

#include "math.h"
#include "string.h"

int rand8(void);

/* Defined in m550.c. */
extern MGTIMER *lbl_1_bss_0;
extern M550CPU lbl_1_bss_4[2];
extern OMCAMERAVIEW lbl_1_bss_38[4];
extern M550STATE lbl_1_bss_14AC;

/* lbl_1_bss_1EC0: per-player input-sequence state. */
s32 lbl_1_bss_1EC0[2];
/* lbl_1_bss_1EC8: the winner message id. */
s16 lbl_1_bss_1EC8;
/* lbl_1_bss_1ECC: 30-wide rows of slot ids, row picked by the argument.
   The original object reserves 0xF4 bytes, four more than the two rows. */
union {
    s32 rows[2][0x1E];
    u8 raw[0xF4];
} lbl_1_bss_1ECC;

/* lbl_1_data_250: attack effect offsets, one per score step. */
HuVecF lbl_1_data_250[0x1E] = {
    { -35.0f, 220.0f, 75.0f },
    { 80.0f, 145.0f, 0.0f },
    { -80.0f, 145.0f, 0.0f },
    { -29.0f, 165.0f, 90.0f },
    { 29.0f, 165.0f, 90.0f },
    { 70.0f, 190.0f, 55.0f },
    { 50.0f, 190.0f, -50.0f },
    { -50.0f, 190.0f, -50.0f },
    { -70.0f, 190.0f, 55.0f },
    { 0.0f, 260.0f, 0.0f },
    { 30.0f, 235.0f, -28.0f },
    { -30.0f, 235.0f, -28.0f },
    { -30.0f, 235.0f, 28.0f },
    { 30.0f, 235.0f, 28.0f },
    { 60.0f, 190.0f, 0.0f },
    { 0.0f, 200.0f, -60.0f },
    { -60.0f, 190.0f, 0.0f },
    { 0.0f, 200.0f, 60.0f },
    { 0.0f, 210.0f, 0.0f },
    { 29.0f, 180.0f, 28.0f },
    { 29.0f, 180.0f, -28.0f },
    { -29.0f, 180.0f, -28.0f },
    { -29.0f, 180.0f, 28.0f },
    { 50.0f, 135.0f, 30.0f },
    { 50.0f, 135.0f, -30.0f },
    { 0.0f, 135.0f, -55.0f },
    { -50.0f, 135.0f, -30.0f },
    { -50.0f, 135.0f, 30.0f },
    { 0.0f, 135.0f, 55.0f },
    { 0.0f, 120.0f, 0.0f },
};

/* lbl_1_data_3B8: last BGM stream id, seeded to -1. */
s32 lbl_1_data_3B8 = -1;

/* MWCC constant-folds a plain `const double` use into an anonymous rodata
   copy, which loses the named @ha/@l relocation the original makes. Reading
   through volatile keeps the named symbol and the load. */
#define RODATA_D(name) (*(volatile f64 *)&(name))

/* lbl_1_rodata_110 (5.5) then lbl_1_rodata_118 (pi/8). */
const f64 lbl_1_rodata_110 = 5.5;
const f64 lbl_1_rodata_118 = 0.39269908169872414;

void fn_1_D64(void);
void fn_1_F7C(void);
void fn_1_10BC(s32 side);
void fn_1_A00(s32 side, HuVecF *pos, HuVecF *rot);
void fn_1_6D8C(void);
void fn_1_7C00(void);
void fn_1_7E94(void);
void fn_1_8190(void);

void fn_1_3904(s32 side);
void fn_1_3DD8(s32 side);
void fn_1_4494(s32 side);
void fn_1_4688(s32 side);
void fn_1_4898(s32 side, s32 step);
void fn_1_49F0(s32 side, s32 idx);
void fn_1_4C00(void);
void fn_1_4D94(void);

void fn_1_1C84(void)
{
    s32 i;

    lbl_1_bss_14AC.modes[0]++;
    lbl_1_bss_14AC.modes[1] = 0;
    lbl_1_bss_14AC.timer = 0x3F48;
    lbl_1_bss_0 = 0;
    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_14AC.player[i].cpuLevel != 0) {
            fn_1_4494(i);
        }
    }
    for (i = 0; i < 0x1E; i++) {
        lbl_1_bss_1ECC.rows[0][i] = 0;
        lbl_1_bss_1ECC.rows[1][i] = 0;
    }
    omCameraViewMoveSimple(&lbl_1_bss_38[1], 0xB4);
    HuAudFXPlay(0x860);
    fn_1_D64();
}

void fn_1_1D88(void)
{
    fn_1_D64();
    if ((u8)WipeCheckEnd() == 0) {
        lbl_1_bss_14AC.modes[0]++;
    }
}

void fn_1_1DCC(void)
{
    if (omCameraViewCheck(1)) {
        lbl_1_bss_14AC.modes[0] = 0;
        lbl_1_bss_14AC.modes[1] = 0;
        MgSeqModeNext();
        lbl_1_data_3B8 = HuAudBGMPlay(0x4B);
    }
    fn_1_D64();
}

void fn_1_1E34(void) { }

void fn_1_1E38(void)
{
    s32 i;
    M550MODEL *model;
    s32 loser;

    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_14AC.player[i].cpuLevel != 0) {
            fn_1_4688(i);
        }
        fn_1_3904(i);
    }
    if (lbl_1_bss_14AC.winner == 0) {
        if (lbl_1_bss_14AC.score[0] >= 0x1E || lbl_1_bss_14AC.score[1] >= 0x1E) {
            if (lbl_1_bss_14AC.score[0] == lbl_1_bss_14AC.score[1]) {
                i = rand8() % 2;
                lbl_1_bss_14AC.score[i]--;
            }
            i = 0;
            if (lbl_1_bss_14AC.score[1] >= 0x1E) {
                i = 1;
            }
            lbl_1_bss_14AC.winner = i + 1;
            if (lbl_1_bss_0 != 0) {
                MgTimerKill(lbl_1_bss_0);
                lbl_1_bss_0 = 0;
            }
        }
        if (lbl_1_bss_14AC.timer > 0) {
            lbl_1_bss_14AC.timer--;
        } else if (lbl_1_bss_0 != 0) {
            if (MgTimerDoneCheck(lbl_1_bss_0)) {
                lbl_1_bss_14AC.modes[0] = 1;
                lbl_1_bss_14AC.score[0] = lbl_1_bss_14AC.score[2];
                lbl_1_bss_14AC.score[1] = lbl_1_bss_14AC.score[3];
                MgSeqModeNext();
                HuAudSStreamFadeOut(lbl_1_data_3B8, 0x3E8);
            }
        } else if (lbl_1_bss_14AC.timer == 0) {
            lbl_1_bss_0 = MgTimerCreate(0);
            MgTimerParamSet(lbl_1_bss_0, 0x708, 0, 0);
            MgTimerPosSet(lbl_1_bss_0, 288.0f, 64.0f);
            MgTimerModeOnSet(lbl_1_bss_0, 1);
            lbl_1_bss_14AC.timer--;
        }
    }
    if (lbl_1_bss_14AC.winner != 0) {
        loser = (lbl_1_bss_14AC.winner - 1) ^ 1;
        switch (lbl_1_bss_14AC.winStep) {
            case 0:
                if (lbl_1_bss_14AC.score[lbl_1_bss_14AC.winner - 1] == lbl_1_bss_14AC.score[lbl_1_bss_14AC.winner + 1]) {
                    lbl_1_bss_14AC.winStep++;
                }
                break;
            case 1:
                if (lbl_1_bss_14AC.modes[lbl_1_bss_14AC.winner] == 0) {
                    lbl_1_bss_14AC.winStep++;
                    /* fxVec[0].y/.z double as the loser's shrink scale and a spare */
                    lbl_1_bss_14AC.fxVec[0].y = 1.0f;
                    lbl_1_bss_14AC.fxVec[0].z = 0.0f;
                }
                break;
            case 3:
                model = &lbl_1_bss_14AC.model[loser + 4];
                model->rot.z += (f32)(loser * 0x28 - 0x14);
                model->pos.x -= lbl_1_bss_14AC.fxVec[0].y * (f32)(3 - loser * 6);
                model->pos.y += 6.0f * lbl_1_bss_14AC.fxVec[0].y;
                lbl_1_bss_14AC.fxVec[0].y *= 0.95;
                Hu3DModelScaleSet(lbl_1_bss_14AC.model[loser + 4].id, lbl_1_bss_14AC.fxVec[0].y, lbl_1_bss_14AC.fxVec[0].y, lbl_1_bss_14AC.fxVec[0].y);
                break;
        }
    }
    for (i = 0; i < 2; i++) {
        fn_1_3DD8(i);
    }
    fn_1_4C00();
    fn_1_D64();
}

void fn_1_235C(void) { }

void fn_1_2360(void) { }

void fn_1_2364(void)
{
    M550MODEL *model;
    s32 i;
    s32 loser;

    loser = (lbl_1_bss_14AC.winner - 1) ^ 1;
    if (lbl_1_bss_14AC.winner != 0) {
        switch (lbl_1_bss_14AC.winStep) {
            case 4:
                break;
            case 0:
                if (lbl_1_bss_14AC.score[lbl_1_bss_14AC.winner - 1] != lbl_1_bss_14AC.score[lbl_1_bss_14AC.winner + 1]) {
                    lbl_1_bss_14AC.winStep++;
                }
                break;
            case 1:
                if (lbl_1_bss_14AC.modes[lbl_1_bss_14AC.winner] == 0) {
                    lbl_1_bss_14AC.winStep++;
                    lbl_1_bss_14AC.fxVec[0].y = 1.0f;
                    lbl_1_bss_14AC.fxVec[0].z = 0.0f;
                }
                break;
            case 3:
                model = &lbl_1_bss_14AC.model[loser + 4];
                model->rot.z += (f32)(loser * 0x28 - 0x14);
                model->pos.x -= lbl_1_bss_14AC.fxVec[0].y * (f32)(3 - loser * 6);
                model->pos.y += 6.0f * lbl_1_bss_14AC.fxVec[0].y;
                lbl_1_bss_14AC.fxVec[0].y *= 0.95;
                Hu3DModelScaleSet(lbl_1_bss_14AC.model[loser + 4].id, lbl_1_bss_14AC.fxVec[0].y, lbl_1_bss_14AC.fxVec[0].y, lbl_1_bss_14AC.fxVec[0].y);
                break;
        }
    }
    for (i = 0; i < 2; i++) {
        fn_1_3DD8(i);
    }
    fn_1_4C00();
    fn_1_D64();
}

void fn_1_2618(void)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        fn_1_3DD8(i);
    }
    lbl_1_bss_14AC.modes[0] = 4;
    fn_1_4C00();
    fn_1_D64();
}

void fn_1_2670(void)
{
    s32 winner;
    s32 loser;

    Center.x = 0.0f;
    Center.y = 2000.0f;
    Center.z = 0.0f;
    CRot.x = -90.0f;
    CRot.y = 0.0f;
    CRot.z = 0.0f;
    CZoom = 300.0f;
    omOutView(NULL);
    winner = lbl_1_bss_14AC.winner - 1;
    loser = winner ^ 1;
    Hu3DModelHookReset(lbl_1_bss_14AC.model[winner + 2].id);
    Hu3DModelAttrSet(lbl_1_bss_14AC.model[winner + 4].id, 1);
    Hu3DModelPosSet(lbl_1_bss_14AC.model[winner + 2].id, -200.0f + 400.0f * (f32)winner, 2300.0f, 300.0f);
    lbl_1_bss_14AC.model[winner + 2].pos.x = -200.0f + 400.0f * (f32)winner;
    lbl_1_bss_14AC.model[winner + 2].pos.y = 2300.0f;
    lbl_1_bss_14AC.model[winner + 2].pos.z = 300.0f;
    Hu3DModelRotSet(lbl_1_bss_14AC.model[winner + 2].id, 0.0f, 0.0f, 0.0f);
    lbl_1_bss_14AC.model[winner + 2].rot.x = 0.0f;
    lbl_1_bss_14AC.model[winner + 2].rot.y = 0.0f;
    lbl_1_bss_14AC.model[winner + 2].rot.z = 0.0f;
    lbl_1_bss_38[1].center.x = -200.0f + 400.0f * (f32)winner;
    lbl_1_bss_38[1].center.y = 2400.0f;
    lbl_1_bss_38[1].center.z = 300.0f;
    lbl_1_bss_38[1].rot.x = -15.0f;
    lbl_1_bss_38[1].rot.y = 0.0f;
    lbl_1_bss_38[1].rot.z = 0.0f;
    lbl_1_bss_38[1].zoom = 600.0f;
    Hu3DMotionSet(lbl_1_bss_14AC.model[winner + 2].id, lbl_1_bss_14AC.motionId[winner][2]);
    Hu3DMotionTimeSet(lbl_1_bss_14AC.model[winner + 2].id, 0.0f);
    Hu3DMotionSpeedSet(lbl_1_bss_14AC.model[winner + 2].id, 1.0f);
    Hu3DModelAttrSet(lbl_1_bss_14AC.model[winner + 2].id, 0x40000001);
    Hu3DMotionAttrSet(lbl_1_bss_14AC.motionId[winner][2], 1);
    Hu3DModelAttrSet(lbl_1_bss_14AC.model[loser + 4].id, 1);
    Hu3DModelPosSet(lbl_1_bss_14AC.model[loser + 2].id, 0.0f, 1900.0f, 0.0f);
    lbl_1_bss_14AC.model[loser + 2].pos.x = 0.0f;
    lbl_1_bss_14AC.model[loser + 2].pos.y = 1900.0f;
    lbl_1_bss_14AC.model[loser + 2].pos.z = 0.0f;
    Hu3DModelRotSet(lbl_1_bss_14AC.model[loser + 2].id, 0.0f, 0.0f, 0.0f);
    lbl_1_bss_14AC.model[loser + 2].rot.x = 0.0f;
    lbl_1_bss_14AC.model[loser + 2].rot.y = 0.0f;
    lbl_1_bss_14AC.model[loser + 2].rot.z = 0.0f;
    Hu3DMotionSet(lbl_1_bss_14AC.model[loser + 2].id, lbl_1_bss_14AC.motionId[loser][4]);
    Hu3DModelAttrSet(lbl_1_bss_14AC.model[loser + 2].id, 0x40000001);
    Hu3DMotionTimeSet(lbl_1_bss_14AC.model[loser + 2].id, 0.0f);
    Hu3DMotionSpeedSet(lbl_1_bss_14AC.model[loser + 2].id, 1.0f);
    lbl_1_bss_14AC.modes[0]++;
    lbl_1_bss_14AC.fxVec[0].x = 1.0f;
    fn_1_D64();
    fn_1_7C00();
    fn_1_7E94();
    fn_1_8190();
}

void fn_1_2D98(void)
{
    s32 sfx[13] = { 0x85C, 0x85D, 0x85A, 0x856, 0x858, 0x85F, 0x857, 0x85E, 0x859, 0x85B, 0x85B, 0x85B, 0x85B };
    s32 loser;
    s32 winner;

    winner = lbl_1_bss_14AC.winner - 1;
    loser = winner ^ 1;
    if (lbl_1_bss_14AC.fxVec[0].x == 1.0f) {
        HuAudFXPlay(sfx[lbl_1_bss_14AC.player[loser].charNo]);
    }
    lbl_1_bss_14AC.model[loser + 2].pos.y -= 8.0f;
    lbl_1_bss_14AC.model[loser + 2].rot.y -= 1.0f;
    lbl_1_bss_14AC.fxVec[0].x *= 0.96f;
    if (lbl_1_bss_14AC.fxVec[0].x < 0.03f) {
        lbl_1_bss_14AC.fxVec[0].x = 0.0f;
    }
    Hu3DModelScaleSet(lbl_1_bss_14AC.model[loser + 2].id, lbl_1_bss_14AC.fxVec[0].x, 1.0f, lbl_1_bss_14AC.fxVec[0].x);
    if (lbl_1_bss_14AC.model[loser + 2].pos.y < -100.0f || lbl_1_bss_14AC.fxVec[0].x <= 0.0f) {
        Hu3DModelAttrSet(lbl_1_bss_14AC.model[loser + 2].id, 1);
        lbl_1_bss_14AC.modes[0]++;
        lbl_1_bss_14AC.modes[1] = 0;
        lbl_1_bss_14AC.score[0] = lbl_1_bss_14AC.score[1] = 0;
        lbl_1_bss_14AC.timer = lbl_1_bss_14AC.unk114 = 0;
        fn_1_4D94();
        omVibrate(lbl_1_bss_14AC.player[loser].pno, 0x1E, 6, 6);
        HuAudFXPlay(0x855);
    }
    fn_1_D64();
}

void fn_1_3088(void)
{
    M550MODEL *cam;

    cam = &lbl_1_bss_14AC.model[0x94];
    /* Read through pointers: a directly-named const double is folded into an
       anonymous rodata copy by MWCC, losing the named relocation. */
    cam->pos.x += RODATA_D(lbl_1_rodata_110) * sin(RODATA_D(lbl_1_rodata_118));
    cam->pos.z += RODATA_D(lbl_1_rodata_110) * cos(RODATA_D(lbl_1_rodata_118));
    if (lbl_1_bss_14AC.timer++ > 0x3C) {
        omCameraViewMoveSimple(&lbl_1_bss_38[1], 0xB4);
        lbl_1_bss_14AC.modes[0]++;
        lbl_1_bss_14AC.timer2 = 0;
    }
    fn_1_D64();
}

void fn_1_3174(void)
{
    s32 pno;
    s32 winner;

    winner = lbl_1_bss_14AC.winner - 1;
    switch (lbl_1_bss_14AC.modes[1]) {
        case 0:
            if (lbl_1_bss_14AC.timer2++ > 0x78) {
                lbl_1_bss_14AC.modes[1]++;
                lbl_1_bss_1EC8 = GameMesMgWinnerCreate(lbl_1_bss_14AC.player[winner].charNo, -1, -1, -1);
                pno = lbl_1_bss_14AC.player[winner].pno;
                if (_CheckFlag(0x1000F) == 0) {
                    GwPlayer[pno].mgCoinBonus = 10;
                }
                lbl_1_bss_14AC.timer = 0;
                HuAudJinglePlay(0x79);
                CharFXPlay(lbl_1_bss_14AC.player[winner].charNo, 0x1FA);
            }
            break;
        case 1:
            lbl_1_bss_14AC.timer++;
            if (GameMesStatGet(lbl_1_bss_1EC8) == 0 && lbl_1_bss_14AC.timer >= 0xD0) {
                GameMesKill(lbl_1_bss_1EC8);
                lbl_1_bss_14AC.modes[0] = 0;
                MgSeqModeNext();
            }
            break;
    }
    fn_1_D64();
}

void fn_1_3324(void)
{
    Mtx mtx;
    HuVecF pos;
    HuVecF rot;
    s32 i;
    M550MODEL *model;

    switch (lbl_1_bss_14AC.modes[1]) {
        case 0:
            lbl_1_bss_1EC8 = GameMesMgDrawCreate();
            lbl_1_bss_14AC.timer = 0;
            lbl_1_bss_14AC.modes[1]++;
            fn_1_10BC(0);
            fn_1_10BC(1);
            omVibrate(lbl_1_bss_14AC.player[0].pno, 0x28, 0xC, 0);
            omVibrate(lbl_1_bss_14AC.player[1].pno, 0x28, 0xC, 0);
            HuAudJinglePlay(0x7B);
            break;
        case 1:
            lbl_1_bss_14AC.timer++;
            if (GameMesStatGet(lbl_1_bss_1EC8) == 0 && lbl_1_bss_14AC.timer >= 0xD0) {
                GameMesKill(lbl_1_bss_1EC8);
                MgSeqModeNext();
                lbl_1_bss_14AC.modes[0] = 1;
            }
            break;
    }
    if (lbl_1_bss_14AC.timer == 8) {
        for (i = 0; i < 2; i++) {
            lbl_1_bss_14AC.motion[i].shiftState = 1;
            lbl_1_bss_14AC.motion[i].shiftMotNo = 3;
            lbl_1_bss_14AC.motion[i].shiftStart = 0.0f;
            lbl_1_bss_14AC.motion[i].shiftEnd = 8.0f;
            lbl_1_bss_14AC.motion[i].shiftAttr = 0x40000001;
            Hu3DModelObjMtxGet(lbl_1_bss_14AC.model[i + 2].id, CharModelItemHookGet(lbl_1_bss_14AC.player[i].charNo, 1, 0), mtx);
            Hu3DModelHookReset(lbl_1_bss_14AC.model[i + 2].id);
            model = &lbl_1_bss_14AC.model[i + 4];
            Hu3DMtxRotGet(mtx, &rot);
            pos.x = mtx[0][3];
            pos.y = mtx[1][3];
            pos.z = mtx[2][3];
            Hu3DModelPosSetV(model->id, &pos);
            Hu3DModelRotSetV(model->id, &rot);
            model->pos.x = pos.x;
            model->pos.y = pos.y;
            model->pos.z = pos.z;
            model->rot.x = rot.x;
            model->rot.y = rot.y;
            model->rot.z = rot.z;
            fn_1_F7C();
            lbl_1_bss_14AC.scale[0] = 1.0f;
            lbl_1_bss_14AC.scale[1] = 1.0f;
        }
    }
    if (lbl_1_bss_14AC.timer > 8) {
        for (i = 0; i < 2; i++) {
            fn_1_A00(i, &lbl_1_bss_14AC.fxVec[i * 2], &lbl_1_bss_14AC.fxVec[i * 2 + 1]);
            lbl_1_bss_14AC.fxVec[i * 2].y += 3.0 + 0.1 * (lbl_1_bss_14AC.timer - 8);
            lbl_1_bss_14AC.fxVec[i * 2 + 1].y += 2.0 - (i * 4);
            model = &lbl_1_bss_14AC.model[i + 4];
            model->rot.z += (f32)(i * 0x28 - 0x14);
            model->pos.x -= lbl_1_bss_14AC.scale[i] * (f32)(3 - i * 6);
            model->pos.y += 6.0f * lbl_1_bss_14AC.scale[i];
            lbl_1_bss_14AC.scale[i] *= 0.95;
            Hu3DModelScaleSet(lbl_1_bss_14AC.model[i + 4].id, lbl_1_bss_14AC.scale[i], lbl_1_bss_14AC.scale[i], lbl_1_bss_14AC.scale[i]);
            lbl_1_bss_14AC.model[i + 2].pos.y -= 20.0f;
        }
        return;
    }
    fn_1_D64();
}

void fn_1_3904(s32 side)
{
    lbl_1_bss_14AC.player[side].btnDown &= 0x300;
    if (lbl_1_bss_14AC.player[side].btnDown == 0x300) {
        lbl_1_bss_1EC0[side] = 0;
        if (lbl_1_bss_14AC.winner == 0) {
            lbl_1_bss_14AC.score[side]++;
            return;
        }
        if (lbl_1_bss_14AC.score[side] < 0x1D) {
            lbl_1_bss_14AC.score[side]++;
        }
    } else {
        switch (lbl_1_bss_1EC0[side]) {
            case 0:
                if (lbl_1_bss_14AC.player[side].btnDown == 0x100) {
                    lbl_1_bss_1EC0[side] = 1;
                }
                if (lbl_1_bss_14AC.player[side].btnDown == 0x200) {
                    lbl_1_bss_1EC0[side] = 2;
                    return;
                }
                return;
            case 1:
                if (lbl_1_bss_14AC.player[side].btnDown == 0x200) {
                    if (lbl_1_bss_14AC.winner == 0) {
                        lbl_1_bss_14AC.score[side]++;
                    } else if (lbl_1_bss_14AC.score[side] < 0x1D) {
                        lbl_1_bss_14AC.score[side]++;
                    }
                }
                lbl_1_bss_1EC0[side] = 0;
                return;
            case 2:
                if (lbl_1_bss_14AC.player[side].btnDown == 0x100) {
                    if (lbl_1_bss_14AC.winner == 0) {
                        lbl_1_bss_14AC.score[side]++;
                    } else if (lbl_1_bss_14AC.score[side] < 0x1D) {
                        lbl_1_bss_14AC.score[side]++;
                    }
                }
                lbl_1_bss_1EC0[side] = 0;
                break;
        }
    }
}

s32 fn_1_3BB0(s32 side)
{
    Mtx mtx;
    HuVecF pos;
    HuVecF rot;
    s32 ret;
    M550MODEL *model;

    ret = 0;
    if (lbl_1_bss_14AC.winStep > 1 && side != lbl_1_bss_14AC.winner - 1) {
        lbl_1_bss_14AC.modes[side + 1] = 5;
        lbl_1_bss_14AC.motion[side].shiftState = 1;
        lbl_1_bss_14AC.motion[side].shiftMotNo = 3;
        lbl_1_bss_14AC.motion[side].shiftStart = 0.0f;
        lbl_1_bss_14AC.motion[side].shiftEnd = 8.0f;
        lbl_1_bss_14AC.motion[side].shiftAttr = 0x40000001;
        Hu3DModelObjMtxGet(lbl_1_bss_14AC.model[side + 2].id, CharModelItemHookGet(lbl_1_bss_14AC.player[side].charNo, 1, 0), mtx);
        Hu3DModelHookReset(lbl_1_bss_14AC.model[side + 2].id);
        model = &lbl_1_bss_14AC.model[side + 4];
        Hu3DMtxRotGet(mtx, &rot);
        pos.x = mtx[0][3];
        pos.y = mtx[1][3];
        pos.z = mtx[2][3];
        Hu3DModelPosSetV(model->id, &pos);
        Hu3DModelRotSetV(model->id, &rot);
        model->pos.x = pos.x;
        model->pos.y = pos.y;
        model->pos.z = pos.z;
        model->rot.x = rot.x;
        model->rot.y = rot.y;
        model->rot.z = rot.z;
        lbl_1_bss_14AC.winStep = 3;
        ret = 1;
        if (side != 0) {
            HuAudFXPlay(0x853);
        } else {
            HuAudFXPlay(0x854);
        }
    }
    return ret;
}

void fn_1_3DD8(s32 side)
{
    s16 *modelId;

    modelId = &lbl_1_bss_14AC.model[side + 4].id;
    switch (lbl_1_bss_14AC.modes[side + 1]) {
        case 0:
            if (fn_1_3BB0(side) == 0 && lbl_1_bss_14AC.score[side] > lbl_1_bss_14AC.score[side + 2]) {
                lbl_1_bss_14AC.score[side + 2]++;
                lbl_1_bss_14AC.motion[side].shiftState = 1;
                lbl_1_bss_14AC.motion[side].shiftMotNo = 1;
                lbl_1_bss_14AC.motion[side].shiftStart = 0.0f;
                lbl_1_bss_14AC.motion[side].shiftEnd = 4.0f;
                lbl_1_bss_14AC.motion[side].shiftAttr = 0;
                lbl_1_bss_14AC.modes[side + 1] = 1;
                return;
            }
        default:
            return;
        case 1:
            if (lbl_1_bss_14AC.motion[side].shiftState == 0) {
                lbl_1_bss_14AC.modes[side + 1] = 2;
                return;
            }
            break;
        case 2:
            if (fn_1_3BB0(side) == 0) {
                if (lbl_1_bss_14AC.motion[side].time >= 8.0f) {
                    lbl_1_bss_14AC.modes[side + 1] = 3;
                    Hu3DMotionTimeSet(*modelId, 0.0f);
                    if (side != 0) {
                        HuAudFXPlay(0x84F);
                        return;
                    }
                    HuAudFXPlay(0x850);
                    return;
                }
            }
            break;
        case 3:
            if (fn_1_3BB0(side) == 0) {
                if (lbl_1_bss_14AC.motion[side].time >= 10.0f) {
                    Hu3DMotionSpeedSet(*modelId, 1.0f);
                }
                if (lbl_1_bss_14AC.motion[side].time >= 14.0f) {
                    fn_1_4898(side, lbl_1_bss_14AC.score[side + 2]);
                    Hu3DMotionTimeSet(*modelId, 0.0f);
                    Hu3DMotionSpeedSet(*modelId, 0.0f);
                    if (lbl_1_bss_14AC.score[side] != lbl_1_bss_14AC.score[side + 2]) {
                        lbl_1_bss_14AC.motion[side].time = 8.0f;
                        lbl_1_bss_14AC.modes[side + 1] = 2;
                        lbl_1_bss_14AC.score[side + 2]++;
                        return;
                    }
                    lbl_1_bss_14AC.motion[side].shiftState = 1;
                    lbl_1_bss_14AC.motion[side].shiftMotNo = 0;
                    lbl_1_bss_14AC.motion[side].shiftStart = 0.0f;
                    lbl_1_bss_14AC.motion[side].shiftEnd = 6.0f;
                    lbl_1_bss_14AC.motion[side].shiftAttr = 0x40000001;
                    lbl_1_bss_14AC.modes[side + 1] = 4;
                    return;
                }
            }
            break;
        case 4:
            if (lbl_1_bss_14AC.motion[side].shiftState == 0) {
                lbl_1_bss_14AC.modes[side + 1] = 0;
                return;
            }
            break;
        case 5:
            if (lbl_1_bss_14AC.motion[side].shiftState == 0) {
                lbl_1_bss_14AC.modes[side + 1] = 6;
            }
            lbl_1_bss_14AC.model[side + 2].pos.y -= 20.0f;
            return;
        case 6:
            lbl_1_bss_14AC.model[side + 2].pos.y -= 20.0f;
            break;
    }
}

s32 fn_1_4308(s32 side)
{
    s32 ret;

    ret = 0;
    switch (lbl_1_bss_4[side].level) {
        case 0:
            ret = 0xD;
            ret += rand8() % 5;
            break;
        case 1:
            ret = 9;
            ret += rand8() % 4;
            break;
        case 2:
            ret = 7;
            lbl_1_bss_4[side].count++;
            ret += rand8() % 4;
            break;
        case 3:
            ret = 4;
            lbl_1_bss_4[side].count++;
            if (lbl_1_bss_4[side].count % 5 == 0) {
                ret--;
            }
            ret += rand8() % 3;
            break;
    }
    return ret;
}

void fn_1_4494(s32 side)
{
    memset(&lbl_1_bss_4[side], 0, sizeof(M550CPU));
    lbl_1_bss_4[side].level = lbl_1_bss_14AC.player[side].cpuLevel - 1;
    lbl_1_bss_4[side].wait = fn_1_4308(side);
}

void fn_1_4688(s32 side)
{
    lbl_1_bss_4[side].wait--;
    if (lbl_1_bss_4[side].wait <= 0) {
        lbl_1_bss_14AC.player[side].btnDown = 0x300;
        lbl_1_bss_14AC.player[side].btn = 0x300;
        lbl_1_bss_4[side].wait = fn_1_4308(side);
    }
}

void fn_1_4898(s32 side, s32 step)
{
    M550MODEL *model;

    model = &lbl_1_bss_14AC.model[6 + (side ^ 1) * 0x1E + (0x1D - (step - 1))];
    Hu3DModelAttrSet(model->id, 1);
    if (step > 0x13) {
        model = &lbl_1_bss_14AC.model[0x7E + (side ^ 1) * 0xB + (0xA - (step - 0x14))];
        Hu3DMotionSpeedSet(model->id, 1.0f);
    }
    fn_1_49F0(side ^ 1, step - 1);
    if (step == 0x1E) {
        omVibrate(lbl_1_bss_14AC.player[side ^ 1].pno, 0x32, 0xC, 0);
    }
}

s32 fn_1_4990(s32 side)
{
    s32 i;

    for (i = 0; i < 0x1E; i++) {
        if (lbl_1_bss_1ECC.rows[side][i] == 0) {
            break;
        }
    }
    if (i == 0x1E) {
        return -1;
    }
    return i;
}

void fn_1_49F0(s32 side, s32 idx)
{
    Mtx transMtx;
    Mtx mtx;
    s32 slot;
    M550MODEL *model;
    f32 x;
    f32 y;
    f32 z;

    slot = fn_1_4990(side);
    if (slot >= 0) {
        lbl_1_bss_1ECC.rows[side][slot] = 1;
        Hu3DModelObjMtxGet(lbl_1_bss_14AC.model[side + 2].id, CharModelItemHookGet(lbl_1_bss_14AC.player[side].charNo, 1, 4), mtx);
        PSMTXTrans(transMtx, lbl_1_data_250[idx].x, lbl_1_data_250[idx].y, lbl_1_data_250[idx].z);
        PSMTXConcat(mtx, transMtx, transMtx);
        x = transMtx[0][3];
        y = transMtx[1][3];
        z = transMtx[2][3];
        model = &lbl_1_bss_14AC.model[0x42 + side * 0x1E + slot];
        Hu3DModelAttrReset(model->id, 1);
        Hu3DModelPosSet(model->id, x, y, z);
        model->pos.x = x;
        model->pos.y = y;
        model->pos.z = z;
        Hu3DMotionTimeSet(model->id, 0.0f);
        Hu3DMotionSpeedSet(model->id, 1.0f);
        if (side != 0) {
            HuAudFXPlay(0x851);
            return;
        }
        HuAudFXPlay(0x852);
    }
}

void fn_1_4C00(void)
{
    f32 time = -1.0f;
    s32 side;
    s32 i;
    s16 *modelId;
    s32 count;

    for (side = 0; side < 2; side++) {
        count = 0;
        for (i = 0; i < 0x1E; i++) {
            if (lbl_1_bss_1ECC.rows[side][i] != 0) {
                modelId = &lbl_1_bss_14AC.model[0x42 + side * 0x1E + i].id;
                time = Hu3DMotionTimeGet(*modelId);
                if (time >= 18.0f) {
                    lbl_1_bss_1ECC.rows[side][i] = 0;
                    Hu3DModelAttrSet(*modelId, 1);
                }
                count++;
            }
        }
        if (count == 0 && lbl_1_bss_14AC.score[(side ^ 1) + 2] == 0x1E && (s32)MgSeqModeGet() == 5 && lbl_1_bss_14AC.modes[(side ^ 1) + 1] == 0) {
            MgSeqModeNext();
            HuAudSStreamFadeOut(lbl_1_data_3B8, 0x3E8);
        }
    }
}

void fn_1_4D94(void)
{
    M550MODEL *cam;

    cam = &lbl_1_bss_14AC.model[0x94];
    Hu3DModelAttrReset(cam->id, 1);
    Hu3DMotionTimeSet(cam->id, 0.0f);
    Hu3DMotionSpeedSet(cam->id, 1.5f);
    Hu3DModelPosSet(cam->id, 0.0f, 20.0f, 0.0f);
    cam->pos.x = 0.0f;
    cam->pos.y = 20.0f;
    cam->pos.z = 0.0f;
    Hu3DModelScaleSet(cam->id, 0.3f, 0.3f, 0.3f);
    fn_1_6D8C();
}

void fn_1_4E88(void) { }

/* Index tables the stage setup (stage.c, fn_1_50D8) copies into locals; the
   original object keeps them at the end of this unit's .rodata. */
const M550IDX3 lbl_1_rodata_158 = { { 0xA0, 0xA7, 0xA8 } };
const M550IDX3 lbl_1_rodata_164 = { { 0x32, 0x37, 0x38 } };
