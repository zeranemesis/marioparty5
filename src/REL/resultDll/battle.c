#include "REL/resultDll.h"

s32 Result_FastF;
static s32 BattleSkipF;

void Result_BattleCoinMdlMain(void)
{
    float f1;
    float f2;
    float f3;
    s16 i;

    f1 = 0.0f;
    f2 = 0.0f;
    f3 = 0.0f;

    while (1) {
        if (BattleSkipF != 0) {
            Result_FastF = 1;
        } else {
            for(i=0; i<4; i++) {
                if (HuPadStatGet(i) == 0) {
                    if (HuPadBtn[i] & 0x100) {
                        Result_FastF = 1;
                    } else {
                        Result_FastF = 0;
                    }
                }
            }
        }
        HuPrcVSleep();
    }
}