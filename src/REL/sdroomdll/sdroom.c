#include "Runtime.PPCEABI.H/NMWException.h"
#include <stddef.h>
#include "game/object.h"
#include "game/hu3d.h"
#include "game/sprite.h"
#include "game/data.h"
#include "game/process.h"
#include <math.h>

extern void (*_ctors[])(void);
extern void (*_dtors[])(void);
extern void ObjectSetup(void);

extern s16 lbl_1_bss_58C[16];
extern u32 lbl_1_bss_8;

extern void fn_1_7298(void);
extern void fn_1_16064(void);
extern void fn_1_7320(void);
extern void fn_1_16024(void);

int _prolog(void) {
    void (**ctor)(void) = _ctors;
    while (*ctor != NULL) {
        (*ctor)();
        ctor++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void) {
    void (**dtor)(void) = _dtors;
    while (*dtor != NULL) {
        (*dtor)();
        dtor++;
    }
}

void fn_1_17B0(void) {
    f32 angle;

    angle = CRotM[2].y;
    if (angle < 0.0f) {
        angle += 360.0f;
    }
    if (angle >= 360.0f) {
        angle -= 360.0f;
    }
    if (angle < 180.0f) {
        Hu3DModelAttrReset(lbl_1_bss_58C[3], 1);
        Hu3DModelAttrReset(lbl_1_bss_58C[4], 1);
        Hu3DModelAttrSet(lbl_1_bss_58C[2], 1);
        Hu3DModelAttrSet(lbl_1_bss_58C[6], 1);
        fn_1_7298();
        fn_1_16064();
    } else {
        Hu3DModelAttrSet(lbl_1_bss_58C[3], 1);
        Hu3DModelAttrSet(lbl_1_bss_58C[4], 1);
        Hu3DModelAttrReset(lbl_1_bss_58C[2], 1);
        Hu3DModelAttrReset(lbl_1_bss_58C[6], 1);
        fn_1_7320();
        fn_1_16024();
    }
}

// NONMATCHING: complex FP math with int-to-double conversion trick doesn't match
void fn_1_154C(void) {
    HUSPRGRPID grpId;
    HUSPRID sprId;
    s16 i;
    f64 val;
    f64 angle;
    f32 yPos;

    lbl_1_bss_8 = 1;
    grpId = HuSprGrpCreate(1);
    sprId = HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xBE002E, 0x1000, HEAP_MODEL)), 100, 0);
    HuSprGrpMemberSet(grpId, 0, sprId);
    HuSprGrpPosSet(grpId, 0.0f, 0.0f);

    for (i = 1; i <= 10; i++) {
        angle = (f64)i * M_PI * 90.0 / 1800.0;
        yPos = 64.0f - 120.0f * (1.0f - sin(angle));
        HuSprPosSet(grpId, 0, 288.0f, yPos);
        HuPrcVSleep();
    }

    while (lbl_1_bss_8 != 0) {
        HuPrcVSleep();
    }

    for (i = 1; i <= 10; i++) {
        angle = (f64)i * M_PI * 90.0 / 1800.0;
        yPos = 64.0f - 120.0f * (1.0f - cos(angle));
        HuSprPosSet(grpId, 0, 288.0f, yPos);
        HuPrcVSleep();
    }

    HuSprGrpKill(grpId);
    HuPrcEnd();

    while (1) {
        HuPrcVSleep();
    }
}