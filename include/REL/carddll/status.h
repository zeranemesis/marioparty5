#ifndef REL_CARDDLL_STATUS_H
#define REL_CARDDLL_STATUS_H

#include "REL/carddll.h"

/* Public entry points of carddll/status.c. arg0 of the *Work functions is the card player work
   struct (0x170 curPlayerNo, 0x174[4] record index, 0x5C-byte records at offset 0). */
s32 fn_1_36C00(CardGame *arg0);
void fn_1_392B8(void);
void fn_1_392E8(s16 arg0, void *arg1, void *arg2);
s32 fn_1_393C4(CardGame *arg0);
s32 fn_1_3D780(void *arg0);
s32 fn_1_3EA44(CardGame *arg0);
void fn_1_3EAA0(void);
s16 fn_1_3ED3C(s32 arg0, Vec *arg1);
void fn_1_3F278(s32 arg0, s16 arg1);
void fn_1_3F380(s32 arg0, s32 arg1, s32 arg2);
void fn_1_3F3D8(s32 arg0, s16 arg1, Vec *arg2);
void fn_1_3F498(s32 arg0, s32 arg1, Vec *arg2);
void fn_1_3F4EC(s32 arg0, s32 arg1, s32 arg2);
void fn_1_3F540(s32 arg0, s16 arg1, s32 arg2);
void fn_1_3F594(s32 arg0, s32 arg1, f32 arg2);
void fn_1_3F658(s32 arg0, s32 arg1, s32 arg2);
void fn_1_3F6B0(s32 arg0, s16 arg1, Vec *arg2);
s32 fn_1_3F714(s32 arg0, s32 arg1);
void fn_1_3F73C(void);
void fn_1_3FBB0(f32 arg0, f32 arg1, f32 arg2);
s32 fn_1_3FC50(CardGame *arg0);
void fn_1_3FD5C(void);
void fn_1_3FDF0(void *arg0);
void fn_1_40F14(void *arg0);
void fn_1_4149C(s32 arg0);
void fn_1_414AC(void *arg0);
void fn_1_424A0(void *arg0);
s32 fn_1_42EDC(CardGame *arg0);
void fn_1_47848(void *arg0, f32 arg1, f32 arg2, f32 arg3);
s32 fn_1_47AA8(CardGame *arg0);
s32 fn_1_48860(void *arg0);
s32 fn_1_48EF8(void *arg0);
s32 fn_1_493F4(void *arg0);
s32 fn_1_4AB58(void *arg0);
s32 fn_1_4B840(void *arg0);
s32 fn_1_4BDD4(void *arg0);
s32 fn_1_4C1E4(void *arg0);
s32 fn_1_4D8C4(void *arg0);
s32 fn_1_4E388(void *arg0);
void fn_1_4E5A4(void);
void fn_1_4E758(void);
void fn_1_4E7A8(s16 layerNo);
void fn_1_4E848(Vec *arg0, Vec *arg1);
void fn_1_4E8AC(void *arg0);
void fn_1_4E8D8(f32 arg0);
void fn_1_4E8E8(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
void fn_1_4EDB8(Vec *arg0, Vec *arg1, f32 arg2, f32 arg3);
f32 fn_1_4EEA0(s32 arg0, f32 arg1);
void fn_1_4F114(void);
void fn_1_4F144(s32 arg0, Vec *arg1, Vec *arg2, s32 arg3, f32 arg4, s32 arg5);
s32 fn_1_4F1EC(s32 arg0, Vec *arg1);
void fn_1_4FBC8(CardGame *arg0, s32 arg1);
void fn_1_4FBFC(CardGame *arg0, s32 arg1);
void fn_1_4FFEC(void);
void fn_1_500D0(void *arg0, s16 arg1);
void fn_1_501B4(s32 arg0);
s32 fn_1_502DC(s32 arg0);
void fn_1_502E0(s32 arg0);
void fn_1_50460(void);

#endif
