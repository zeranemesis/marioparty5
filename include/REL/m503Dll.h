#ifndef _M503DLL_H
#define _M503DLL_H

#include "types.h"
#include "game/object.h"

/* Game state kept in lbl_1_bss_20->data (0x80 bytes). */
typedef struct {
    s32 state;
    s32 stateTime;
    s32 lock;
    s32 wipeDone;
    s32 unk10;
    u32 time14;
    s32 phase;
    s32 phaseTime;
    s32 unk20;
    u32 time24;
    s32 unk28[4];
    s32 unk38[4];
    s32 unk48[4];
    s32 unk58;
    s32 unk5C;
    s32 unk60[4];
    s32 unk70;
    s32 unk74;
    s32 unk78;
    s32 unk7C;
} M503Data;

extern OMOBJ *lbl_1_bss_20;

void fn_1_430(OMOBJ *obj);
void fn_1_5B0(OMOBJ *obj);
s32 fn_1_1BFC(s32 mode);
void fn_1_2AD4(void *motData);
s32 fn_1_2B4C(void);

#endif
