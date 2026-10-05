#include "REL/m543Dll.h"
#include "game/gamemes.h"
#include "game/mgdata.h"
#include "game/wipe.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void ObjectSetup(void);

int _prolog(void) {
    const VoidFunc* ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void) {
    const VoidFunc* dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
}

void fn_1_3FC(void)
{
    fn_1_64AC();
}

void fn_1_3CCC(void)
{
    HuAudAllStop();
}

BOOL fn_1_1B2C(MGTIMER* timer)
{
    return MgTimerDoneCheck(timer);
}

void fn_1_2F4(void)
{
    if (fn_1_5B78() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_320(void)
{
    if (fn_1_5BB0() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_34C(void)
{
    if (fn_1_5C20() != 0) {
        MgSeqModeNext();
    }
}