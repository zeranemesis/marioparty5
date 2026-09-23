#include "REL/m540Dll.h"

#include "REL/m540Dll/math.c"
#include "REL/m540Dll/lib.c"

static void fn_1_15C(void);
static void fn_1_1D4(s16 mode, s16 frameNo);
static void fn_1_FFC(s16 mode, s16 frameNo);
static void fn_1_2C20(s16 mode, s16 frameNo);
static void fn_1_2C48(s16 mode, s16 frameNo);
static void fn_1_566C(s16 mode, s16 frameNo);
static void fn_1_618C(s16 mode, s16 frameNo);

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

void ObjectSetup(void);

int _prolog(void)
{
    const VoidFunc *ctors = _ctors;
    while(*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;
    while(*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    OMOBJMAN *objman;
    MGSEQ_PARAM seqParam;
    objman = omInitObjMan(1, 8192);
    omGameSysInit(objman);
    HuWinInit(0);
    seqParam.maxTime = 0;
    seqParam.timerPos = 0;
    seqParam.initHook = fn_1_1D4;
    seqParam.fadeInHook = fn_1_FFC;
    seqParam.startHook = fn_1_2C20;
    seqParam.mainHook = fn_1_2C48;
    seqParam.finishHook = fn_1_566C;
    seqParam.preWinnerHook = fn_1_618C;
    seqParam.winnerHook = NULL;
    seqParam.fadeOutHook = NULL;
    seqParam.closeHook = NULL;
    MgSeqCreate(&seqParam);
}

static void fn_1_15C(void)
{
    while(!omSysExitReq) {
        HuPrcVSleep();
    }
    HuAudFadeOut(1000);
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_WHITEFADE, 60);
    while(WipeCheckEnd()) {
        HuPrcVSleep();
    }
    omOvlReturnEx(1, 1);
    HuPrcEnd();
}
