#include "types.h"

s32 fn_1_4FFC(void);
s32 fn_1_50A4(void);
void fn_1_5A00(void);
void MgSeqModeNext(void);
void MgTimerDoneCheck(s32 timerIdx);
void HuAudAllStop(void);

void fn_1_354(void) {
    if (fn_1_4FFC() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_380(void) {
    if (fn_1_50A4() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_45C(void) {
    fn_1_5A00();
}

void fn_1_1B8C(s32 timerIdx) {
    MgTimerDoneCheck(timerIdx);
}

void fn_1_3B54(void) {
    HuAudAllStop();
}
