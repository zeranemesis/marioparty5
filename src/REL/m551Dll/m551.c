#include "types.h"

extern s32 fn_1_81F4(void);
extern void fn_1_8FF4(void);
extern void HuAudAllStop(void);
extern void MgSeqModeNext(void);
extern void MgTimerDoneCheck(void *timer);
extern void MgTimerModeOnSet(void *timer, s32 mode);

void fn_1_2F4(void) {
    if (fn_1_81F4() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_3FC(void) {
    fn_1_8FF4();
}

void fn_1_1AB8(void *timer) {
    MgTimerModeOnSet(timer, 1);
}

void fn_1_1AE4(void *timer) {
    MgTimerDoneCheck(timer);
}

void fn_1_5FD4(void) {
    HuAudAllStop();
}
