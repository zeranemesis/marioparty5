#ifndef _M530DLL_H
#define _M530DLL_H

#include "game/object.h"

extern HUPROCESS *lbl_1_bss_B8;
extern s32 lbl_1_data_6C[0x48];
extern unsigned int lbl_1_data_C0[15];

/* implemented in REL/m530Dll/m530.c */
void fn_1_10E8(OMOBJ *obj, s32 arg1);
void fn_1_1A50(f32 *pos, s16 *vol, s16 *pan);
void fn_1_1B78(f32 *pos, s32 fxId);
f32 fn_1_4414(s32 *field, s32 *cells, f32 *pos);
s32 fn_1_4458(s32 *field, s32 *cells, f32 *pos, f32 *pos2, void *out);
s32 fn_1_6518(f32 *pos, f32 radius);
void fn_1_66D8(s32 cellNo);
s32 fn_1_6B80(f32 *pos, s16 side);

/* implemented in REL/m530Dll/effect.c */
void fn_1_8848(void);
void fn_1_8958(OMOBJ *obj);
s32 fn_1_8E2C(void *work, void *arg1);

#endif