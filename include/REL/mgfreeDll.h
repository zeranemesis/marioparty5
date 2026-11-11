#ifndef _REL_MGFREEDLL_H
#define _REL_MGFREEDLL_H

#include "dolphin/types.h"
#include "humath.h"
#include "game/process.h"

/* One slot of the "decide" screen's selection table. */
typedef struct MgFreeDecideSlot_s {
    s16 mgNo;
    s8 enable;
    u8 pad;
} MgFreeDecideSlot;

typedef struct MgFreeDecide_s {
    s16 state; /* 0x00 */
    s16 timer; /* 0x02 */
    s32 id[0x20]; /* 0x04: window / sprite group ids */
    MgFreeDecideSlot entry[30]; /* 0x84 */
    s32 count; /* 0xFC */
    s16 idx; /* 0x100 */
    s16 unk102;
    s16 scroll; /* 0x104 */
    s8 col; /* 0x106 */
    u8 sel; /* 0x107 */
    s32 unk108[4];
    HUPROCESS *proc[4]; /* 0x118 */
    HuVec2f dstPos[4]; /* 0x128 */
    s8 unk148;
    s8 list[7]; /* 0x149 */
} MgFreeDecide;

extern MgFreeDecide *lbl_1_bss_974;

void ObjectSetup(void);
void fn_1_1F50(void);
void fn_1_205C(void);
void fn_1_21AC(void);
void fn_1_2C40(void);
void fn_1_37A4(void);
void fn_1_3C8C(void);
void fn_1_45A4(void);
f32 fn_1_2B4(f32 a, f32 b, f32 t);
s32 fn_1_A4DC(void);
s32 fn_1_A544(void);
u8 fn_1_4D4C(void);
s32 fn_1_4D60(s32 idx);
void fn_1_4D20(s32 val);
void fn_1_6B80(s32 *out1, s32 *out2, s32 *out3);
void fn_1_6BC4(s32 a, s32 b, s32 c, s32 d);
void fn_1_A068(void);
void fn_1_A454(void);
void fn_1_15B8(void);
u8 fn_1_218(void);
u32 fn_1_244(void);

#endif
