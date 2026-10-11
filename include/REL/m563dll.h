#ifndef _M563DLL_H
#define _M563DLL_H

#include "dolphin.h"

typedef struct M563ModelSlot_s {
    u32 modelNum;
    s16 modelHandle;
    s16 pad_6[1];
} M563ModelSlot;

typedef struct M563ListNode_s {
    u16 id;
    s16 value;
    struct M563ListNode_s *prev;
    struct M563ListNode_s *next;
} M563ListNode;

/* NOTE: util.o owns every one of these in the original, so util.c defines them
 * and this header must not redeclare them extern (a conflicting `extern` makes
 * CodeWarrior reject the definition and the REL link then fails to resolve). */
/* The 79C definition above replaces the former `extern Vec lbl_1_bss_79C`;
 * math.c reaches it as lbl_1_bss_79C.v.x, which the diff below shows. */
extern Vec lbl_1_bss_784;
extern Vec lbl_1_bss_778;
extern Vec lbl_1_bss_790;

/* lbl_1_bss_79C is 0x10 bytes in the original: a Vec plus one word of trailing
 * padding. It needs a named type because math.c and util.c must agree on it. */
typedef struct M563VecPad_s {
    Vec v;
    u32 pad;
} M563VecPad;
extern M563VecPad lbl_1_bss_79C;
extern Vec lbl_1_bss_6B8[4];
extern Vec lbl_1_bss_6E8[4];
extern Vec lbl_1_bss_718[4];
extern Vec lbl_1_bss_748[4];
extern s32 lbl_1_bss_6A8;
extern s32 lbl_1_bss_6AC;
extern s32 lbl_1_bss_3C0[4][30];
extern M563ModelSlot lbl_1_bss_5A4[0x20];
extern Vec lbl_1_data_358[2][30];
extern M563ListNode lbl_1_bss_7AC;

s32 fn_1_767C(f32 *arg0, f32 *arg1, f32 *arg2, s32 arg3, u32 *arg4);
void fn_1_7818(f32 *arg0, f32 *arg1, f32 *arg2, s32 arg3);
s32 fn_1_79A8(s32 arg0, s32 arg1);
void fn_1_79C8(s32 arg0, s32 arg1, s32 arg2);
void fn_1_79E8(void);
void fn_1_7A4C(void);
void fn_1_7A70(u16 arg0, s16 arg1);
s16 fn_1_7AF8(s16 arg0);
void fn_1_7B9C(void);
s16 fn_1_7BF4(u32 arg0);

void fn_1_7D08(float var_f30, Vec *arg0, Vec *arg1, Vec *arg2);
void fn_1_7E64(Vec *arg0, Vec *arg1, Vec *arg2);
s32 fn_1_7EF0(float var_f27, Vec *arg0);
s32 fn_1_8224(float var_f27, Vec *arg0);
float fn_1_8608(float var_f29, float arg1, float arg2);
void fn_1_870C(s32 arg0);
void fn_1_8778(s16 arg0, char *arg1, Vec *arg2);
u32 fn_1_87D0(u32 arg0, u32 arg1);
s32 fn_1_880C(Vec *arg0, Vec *arg1, float arg2, float arg3);
s32 fn_1_886C(Vec *arg0, s32 arg1, s32 arg2);
s32 fn_1_8930(Vec *arg0, s32 arg1, s32 arg2);

#endif