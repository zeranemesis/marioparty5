// m513Dll score: the timer / record display of the minigame. In team mode
// there is one timer window per player, otherwise a single timer.
#define _MATH_H
#include "REL/m513Dll.h"
extern "C" {
#include "game/gamework.h"
}
extern "C" {
#include "game/gamemes.h"
}
extern "C" {
#include "game/esprite.h"
}

#ifdef __MWERKS__
#define M513_RODATA __declspec(section ".rodata")
#else
#define M513_RODATA
#endif



extern "C" {
extern M513Score *lbl_1_bss_A20;


void fn_1_1220C(void);
void fn_1_12208(void);

M513Score *fn_1_13304(M513Score *self, s16 flag);
void fn_1_13EA4(M513Score *self, s32 bit);
void fn_1_13430(M513Score *self);
void fn_1_13AA4(M513Score *self, s32 idx, s32 charNo, s32 x, s32 y);

M513_RODATA const s32 lbl_1_rodata_580[8] = { 0x4A, 0x36, 0x1F4, 0x36, 0x4A, 0x1A8, 0x1F4, 0x1A8 };

M513Ptmf lbl_1_data_918 = { 0, -1, (void (*)(void *))fn_1_1220C };
M513Ptmf lbl_1_data_924 = { 0, -1, (void (*)(void *))fn_1_12208 };
char lbl_1_data_930[] = "ms_pointer == 0";
char lbl_1_data_940[] = "singleton.h";
char lbl_1_data_94C[] = "ms_pointer != 0";
u32 lbl_1_data_95C[0xF] = {
    0x960020, 0x960021, 0x960022, 0x960023, 0x960024, 0x960026, 0x960027, 0x960028,
    0x960029, 0x96002A, 0x96002B, 0x96002C, 0x96002D, 0x96002A, 0x96002A,
};
void *lbl_1_data_998[5] = { 0, 0, (void *)fn_1_13304, (void *)fn_1_13EA4, (void *)fn_1_22E0 };

M513Score *fn_1_13174(M513Score *self, s32 recordTime, u8 multi)
{
    M513Score *ms;

    fn_1_1768(&self->link);
    self->vtbl = (void **)&lbl_1_data_218;
    self->flags = 0;
    fn_1_1834(&self->link, &lbl_1_bss_C);
    M513_ASSERT(lbl_1_bss_A20 == 0, lbl_1_data_930, lbl_1_data_940, 0x35);
    ms = (M513Score *)((char *)self + 0x10);
    if (ms != 0) {
        ms = (M513Score *)((char *)ms - 0x10);
    }
    lbl_1_bss_A20 = ms;
    self->vtbl = lbl_1_data_998;
    self->recordTime = recordTime;
    self->created = 0;
    self->multi = multi;
    if (self->multi == 0) {
        fn_1_13430(self);
    }
    return self;
}

M513Score *fn_1_13278(M513Score *self, s16 flag)
{
    if (self != 0) {
        M513_ASSERT(lbl_1_bss_A20 != 0, lbl_1_data_94C, lbl_1_data_940, 0x3D);
        lbl_1_bss_A20 = 0;
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M513Score *fn_1_13304(M513Score *self, s16 flag)
{
    s32 i;

    if (self != 0) {
        self->vtbl = lbl_1_data_998;
        if (self->created != 0) {
            if (self->multi != 0) {
                for (i = 0; i < 4; i++) {
                    MgTimerKill(self->timers[i]);
                    espKill(self->esp[i]);
                }
            } else {
                MgTimerKill(self->timer);
            }
        }
        if ((void *)((char *)self + 0x10) != 0) {
            M513_ASSERT(lbl_1_bss_A20 != 0, lbl_1_data_94C, lbl_1_data_940, 0x3D);
            lbl_1_bss_A20 = 0;
        }
        if (self != 0) {
            self->vtbl = (void **)&lbl_1_data_218;
            if (self != 0) {
                fn_1_1774(&self->link, 0);
            }
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// Create the timer window(s).
void fn_1_13430(M513Score *self)
{
    s32 pos[8];
    s32 i;

    pos[0] = lbl_1_rodata_580[0];
    pos[1] = lbl_1_rodata_580[1];
    pos[2] = lbl_1_rodata_580[2];
    pos[3] = lbl_1_rodata_580[3];
    pos[4] = lbl_1_rodata_580[4];
    pos[5] = lbl_1_rodata_580[5];
    pos[6] = lbl_1_rodata_580[6];
    pos[7] = lbl_1_rodata_580[7];
    if (self->multi != 0) {
        for (i = 0; i < 4; i++) {
            s16 win = MgScoreWinCreate(0x78, 0x20);

            MgScoreWinPosSet(win, (f32)pos[i * 2], (f32)pos[i * 2 + 1]);
            MgScoreWinDispSet(win, 1);
            self->win[i] = win;
            self->timers[i] = MgTimerCreate(2);
            MgTimerParamSet(self->timers[i], 0, 0xE10, self->recordTime);
            MgTimerPosSet(self->timers[i], 24.0f + (f32)pos[i * 2], 8.0f + (f32)pos[i * 2 + 1]);
            MgTimerWinColorSet(self->timers[i], 0, 0, 0);
            MgTimerRecordDispOn(self->timers[i]);
            MgScoreWinDispSet(self->timers[i]->winGrpId, 0);
            self->timers[i]->winDispF = 0;
            fn_1_13AA4(self, i, GwPlayerConf[i].charNo, (s32)((f32)pos[i * 2] - 42.0f), pos[i * 2 + 1]);
        }
    } else {
        self->timer = MgTimerCreate(1);
        MgTimerParamSet(self->timer, 0, 0x4650, self->recordTime);
        MgTimerPosSet(self->timer, 292.0f, 70.0f);
        MgTimerWinColorSet(self->timer, 0, 0, 0);
        MgTimerRecordDispOn(self->timer);
    }
    self->created = 1;
}

// Same as fn_1_13430 but only when the timers do not exist yet.
void fn_1_13764(M513Score *self)
{
    s32 pos[8];
    s32 i;

    if (self->created == 0) {
        pos[0] = lbl_1_rodata_580[0];
        pos[1] = lbl_1_rodata_580[1];
        pos[2] = lbl_1_rodata_580[2];
        pos[3] = lbl_1_rodata_580[3];
        pos[4] = lbl_1_rodata_580[4];
        pos[5] = lbl_1_rodata_580[5];
        pos[6] = lbl_1_rodata_580[6];
        pos[7] = lbl_1_rodata_580[7];
        if (self->multi != 0) {
            for (i = 0; i < 4; i++) {
                s16 win = MgScoreWinCreate(0x78, 0x20);

                MgScoreWinPosSet(win, (f32)pos[i * 2], (f32)pos[i * 2 + 1]);
                MgScoreWinDispSet(win, 1);
                self->win[i] = win;
                self->timers[i] = MgTimerCreate(2);
                MgTimerParamSet(self->timers[i], 0, 0xE10, self->recordTime);
                MgTimerPosSet(self->timers[i], 24.0f + (f32)pos[i * 2], 8.0f + (f32)pos[i * 2 + 1]);
                MgTimerWinColorSet(self->timers[i], 0, 0, 0);
                MgTimerRecordDispOn(self->timers[i]);
                MgScoreWinDispSet(self->timers[i]->winGrpId, 0);
                self->timers[i]->winDispF = 0;
                fn_1_13AA4(self, i, GwPlayerConf[i].charNo, (s32)((f32)pos[i * 2] - 42.0f), pos[i * 2 + 1]);
            }
        } else {
            self->timer = MgTimerCreate(1);
            MgTimerParamSet(self->timer, 0, 0x4650, self->recordTime);
            MgTimerPosSet(self->timer, 292.0f, 70.0f);
            MgTimerWinColorSet(self->timer, 0, 0, 0);
            MgTimerRecordDispOn(self->timer);
        }
        self->created = 1;
    }
}

// Player portrait next to a timer window.
void fn_1_13AA4(M513Score *self, s32 idx, s32 charNo, s32 x, s32 y)
{
    self->esp[idx] = espEntry(lbl_1_data_95C[charNo], 0, 0);
    espBankSet(self->esp[idx], 0);
    espPosSet(self->esp[idx], (f32)x, (f32)y);
    espDispOn(self->esp[idx]);
}

void fn_1_13B98(M513Score *self)
{
    s32 i;

    if (self->multi != 0) {
        for (i = 0; i < 4; i++) {
            MgTimerModeOnSet(self->timers[i], 2);
        }
        return;
    }
    MgTimerModeOnSet(self->timer, 0);
}

void fn_1_13C0C(M513Score *self)
{
    s32 i;

    if (self->multi != 0) {
        for (i = 0; i < 4; i++) {
            MgTimerModeOffSet(self->timers[i]);
        }
        return;
    }
    MgTimerModeOffSet(self->timer);
}

void fn_1_13C78(M513Score *self, s32 idx)
{
    if (self->multi != 0) {
        MgTimerModeOffSet(self->timers[idx]);
        self->timers[idx]->flashFlag = 1;
    }
}

void fn_1_13CDC(M513Score *self)
{
    s32 i;

    if (self->multi != 0) {
        for (i = 0; i < 4; i++) {
            if (self->timers[i]->stopF == 0) {
                self->timers[i]->stopF = 1;
                self->timers[i]->mode = 1;
            }
        }
        return;
    }
    if (self->timer->stopF == 0) {
        self->timer->stopF = 1;
        self->timer->mode = 1;
    }
}

s32 fn_1_13D7C(M513Score *self)
{
    s32 i;
    s32 done;

    if (self->multi != 0) {
        for (i = 0;; i++) {
            if (i >= 4) {
                return 0;
            }
            if ((MgTimerDoneCheck(self->timers[i]) != 0) && (self->created != 0)) {
                return 1;
            }
        }
    }
    done = 0;
    if ((MgTimerDoneCheck(self->timer) != 0) && (self->created != 0)) {
        done = 1;
    }
    return done;
}

s32 fn_1_13E34(M513Score *self, s32 idx)
{
    s32 val;

    if (self->multi != 0) {
        val = MgTimerValueGet(self->timers[idx]);
    } else {
        val = MgTimerValueGet(self->timer);
    }
    return val;
}

// Virtual update(): bit 1 creates the timers, bit 2 shows, bit 3 hides.
void fn_1_13EA4(M513Score *self, s32 bit)
{
    s32 pos[8];
    s32 i;

    self->flags &= ~(1 << bit);
    switch (bit) {
        case 1:
            if (self->created == 0) {
                pos[0] = lbl_1_rodata_580[0];
                pos[1] = lbl_1_rodata_580[1];
                pos[2] = lbl_1_rodata_580[2];
                pos[3] = lbl_1_rodata_580[3];
                pos[4] = lbl_1_rodata_580[4];
                pos[5] = lbl_1_rodata_580[5];
                pos[6] = lbl_1_rodata_580[6];
                pos[7] = lbl_1_rodata_580[7];
                if (self->multi != 0) {
                    for (i = 0; i < 4; i++) {
                        s16 win = MgScoreWinCreate(0x78, 0x20);
                        s32 y;
                        s32 x;

                        MgScoreWinPosSet(win, (f32)pos[i * 2], (f32)pos[i * 2 + 1]);
                        MgScoreWinDispSet(win, 1);
                        self->win[i] = win;
                        self->timers[i] = MgTimerCreate(2);
                        MgTimerParamSet(self->timers[i], 0, 0xE10, self->recordTime);
                        MgTimerPosSet(self->timers[i], 24.0f + (f32)pos[i * 2], 8.0f + (f32)pos[i * 2 + 1]);
                        MgTimerWinColorSet(self->timers[i], 0, 0, 0);
                        MgTimerRecordDispOn(self->timers[i]);
                        MgScoreWinDispSet(self->timers[i]->winGrpId, 0);
                        self->timers[i]->winDispF = 0;
                        y = pos[i * 2 + 1];
                        x = (s32)((f32)pos[i * 2] - 42.0f);
                        self->esp[i] = espEntry(lbl_1_data_95C[GwPlayerConf[i].charNo], 0, 0);
                        espBankSet(self->esp[i], 0);
                        espPosSet(self->esp[i], (f32)x, (f32)y);
                        espDispOn(self->esp[i]);
                    }
                } else {
                    self->timer = MgTimerCreate(1);
                    MgTimerParamSet(self->timer, 0, 0x4650, self->recordTime);
                    MgTimerPosSet(self->timer, 292.0f, 70.0f);
                    MgTimerWinColorSet(self->timer, 0, 0, 0);
                    MgTimerRecordDispOn(self->timer);
                }
                self->created = 1;
                return;
            }
            // fall through
        case 4:
        case 0:
            return;
        case 2:
            if (self->multi != 0) {
                for (i = 0; i < 4; i++) {
                    MgTimerModeOnSet(self->timers[i], 2);
                }
                return;
            }
            MgTimerModeOnSet(self->timer, 0);
            return;
        case 3:
            if (self->multi != 0) {
                for (i = 0; i < 4; i++) {
                    MgTimerModeOffSet(self->timers[i]);
                }
                return;
            }
            MgTimerModeOffSet(self->timer);
            break;
    }
}

}
