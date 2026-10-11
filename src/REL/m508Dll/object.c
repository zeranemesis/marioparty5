#include "game/process.h"
#include "game/object.h"
#include "game/hu3d.h"

void fn_1_BE58(void);
void fn_1_C630(OMOBJ *obj);
void fn_1_C660(OMOBJ *obj);
void fn_1_C690(OMOBJ *obj);
void fn_1_E32C(void);
void fn_1_E388(void *arg0, float *arg1, float *arg2);

void fn_1_BE58(void) {
    while (TRUE) {
        HuPrcSleep(0);
    }
}

void fn_1_C630(OMOBJ *obj) {
    // NONMATCHING: register allocation (r31 vs r3 stack save)
    Hu3DModelShadowSet(*(s16 *)((u8 *)obj + 0x184));
}

void fn_1_C660(OMOBJ *obj) {
    // NONMATCHING: register allocation (r31 vs r3 stack save)
    Hu3DModelShadowReset(*(s16 *)((u8 *)obj + 0x184));
}

void fn_1_C690(OMOBJ *obj) {
    // NONMATCHING: register allocation (r31 vs r3 stack save)
    Hu3DModelShadowMapSet(*(s16 *)((u8 *)obj + 0x184));
}

void fn_1_E32C(void) {
    while (TRUE) {
        HuPrcVSleep();
    }
}

void fn_1_E388(void *arg0, float *arg1, float *arg2) {
    *arg1 = *(float *)((u8 *)arg0 + 0x4);
    *arg2 = *(float *)((u8 *)arg0 + 0x8);
}