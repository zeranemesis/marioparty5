#ifndef REL_CARDDLL_DICE_H
#define REL_CARDDLL_DICE_H

#include "REL/carddll.h"

/* dice.c: dice block / bonus roulette effects of the Card Party bonus mode. */
extern s32 lbl_1_bss_11DC; /* state of the result banner (fn_1_358F8) */
extern s32 lbl_1_bss_1298[4];

s32 fn_1_325F4(CardGame *work);
s32 fn_1_32D70(CardGame *work);
s32 fn_1_33234(s32 kind);
s32 fn_1_337C8(CardGame *work);
s32 fn_1_33C94(CardGame *work);
s32 fn_1_345B4(CardGame *work);
void fn_1_34CC8(CardGame *work);
s32 fn_1_34DA4(CardGame *work);
s32 fn_1_34E24(CardGame *work);
void fn_1_351C4(s32 playerNo);
void fn_1_352D0(CardGame *work);
void fn_1_358F8(CardGame *work);
void fn_1_3678C(CardGame *work);
void fn_1_36828(void);
s16 fn_1_36A60(void);
void fn_1_36A70(Vec *src);
void fn_1_36B10(s32 motion);
void fn_1_36B78(void);

#endif
