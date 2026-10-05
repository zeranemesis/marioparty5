#include "types.h"

extern void fn_1_6AC4(void);
extern s32 fn_1_6CD4(void);
extern s32 fn_1_6E94(void);
extern void HuAudAllStop(void);
extern u32 MgSeqModeNext(void);
extern BOOL MgTimerDoneCheck(void *timer);

void fn_1_350(void) {
    fn_1_6AC4();
}

void fn_1_39C(void) {
    fn_1_6CD4();
    MgSeqModeNext();
}

void fn_1_418(void) {
    fn_1_6E94();
}

void fn_1_197C(void *timer) {
    MgTimerDoneCheck(timer);
}

void fn_1_58B0(void) {
    HuAudAllStop();
}