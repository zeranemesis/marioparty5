#include "dolphin/types.h"
#include "game/process.h"

float fn_1_5D20(float *arg0, float *arg1)
{
    return arg0[0] * arg1[0] + arg0[1] * arg1[1];
}

float fn_1_5D40(float *arg0, float *arg1)
{
    return arg0[0] * arg1[1] - arg0[1] * arg1[0];
}

void fn_1_7A24(void)
{
    while (1) {
        HuPrcSleep(0);
    }
}

void fn_1_9EF8(void)
{
    while (1) {
        HuPrcVSleep();
    }
}

void fn_1_9F54(float *arg0, float *arg1, float *arg2)
{
    *arg1 = arg0[1];
    *arg2 = arg0[2];
}

float fn_1_A154(float *arg0, float *arg1)
{
    return arg0[0] * arg1[0] + arg0[1] * arg1[1];
}