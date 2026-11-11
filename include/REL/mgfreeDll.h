#ifndef _REL_MGFREEDLL_H
#define _REL_MGFREEDLL_H

#include "dolphin/types.h"

/* One slot of the "decide" screen's selection table. */
typedef struct MgFreeDecideSlot_s {
    s16 mgNo;
    s8 enable;
    u8 pad;
} MgFreeDecideSlot;

typedef struct MgFreeDecide_s {
    s16 unk_00;
    s16 unk_02;
    u8 pad_04[0x80];
    MgFreeDecideSlot entry[0x3E];
    s16 selIdx;
    u8 pad_102[0x2];
    s16 unk_104;
    s8 unk_106;
    u8 unk_107;
    u8 pad_108[0x41];
    s8 unk_149[0x100];
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
