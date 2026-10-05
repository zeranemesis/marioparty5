#define _MATH_H

#include "game/data.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/process.h"

#include "REL/e3setupdll.h"

extern s16 lbl_1_bss_3C[4];

typedef void (*VoidFunc)(void);

extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

void _prolog(void) {
    const VoidFunc *ctorPtr = _ctors;

    for (; *ctorPtr != NULL; ctorPtr++) {
        (*ctorPtr)();
    }
    ObjectSetup(NULL);
}

void _epilog(void) {
    const VoidFunc *dtorPtr = _dtors;
    while (*dtorPtr != NULL) {
        (*dtorPtr)();
        dtorPtr++;
    }
}

void fn_1_2E08(void) {
    s16 i;
    while (1) {
        for (i = 0; i < 4; i++) {
            Hu3DModelAttrSet(lbl_1_bss_3C[i], 1);
        }
        HuPrcVSleep();
    }
}

void fn_1_ABB4(void *arg) {
    // NONMATCHING: compiler uses lha instead of lwz+extsh for halfword loads
    Hu3DParManKill(((s16 *)arg)[0]);
    Hu3DParManKill(((s16 *)arg)[1]);
    Hu3DParManKill(((s16 *)arg)[2]);
}

void fn_1_ACDC(void *arg, void *pos) {
    // NONMATCHING: compiler uses lha instead of lwz+extsh for halfword loads
    Hu3DParManPosSet(*(s16 *)arg, ((float *)pos)[0], ((float *)pos)[1], ((float *)pos)[2]);
    Hu3DParManTimeLimitSet(*(s16 *)arg, 2);
    Hu3DParManAttrReset(*(s16 *)arg, 1);
}

void fn_1_AD48(void *arg) {
    // NONMATCHING: register allocation differs (uses r3 instead of r31 for pointer)
    Hu3DParManKill((s16)*(u32 *)arg);
}