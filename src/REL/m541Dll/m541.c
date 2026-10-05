extern void fn_1_6E20(void);
extern void HuAudAllStop(void);
extern int MgTimerDoneCheck(int);
extern int fn_1_5DA4(void);
extern void MgSeqModeNext(void);

void fn_1_3FC(void) {
    fn_1_6E20();
}

void fn_1_2AB4(void) {
    HuAudAllStop();
}

int fn_1_1A10(int arg) {
    return MgTimerDoneCheck(arg);
}

void fn_1_2F4(void) {
    if (fn_1_5DA4() != 0) {
        MgSeqModeNext();
    }
}