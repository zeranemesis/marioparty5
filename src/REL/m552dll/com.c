#include "types.h"

int rand8(void);

void fn_1_A7AC(s32 idx);
void fn_1_A8F4(s32 idx);
void fn_1_B2C0(s32 idx);

extern void *memset(void *dst, int c, unsigned int n);

/* Per-player setup parameters, stride 0x1C. unk08 (s16) is the com message
 * number that gets copied into the live com state by fn_1_91BC. */
typedef struct M552ComParam_s {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s16 msgNo;
    /* 0x0A */ u16 unk0A;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
} M552ComParam;

/* Live com state for one player, stride 0x7C.
 * msgNo   1-based row index into the message time tables.
 * state   0 = start line, 1 = running line, 3 = waiting on a timer.
 * nextState is pushed into state when the timer in `timer` expires. */
typedef struct M552Com_s {
    /* 0x00 */ s32 msgNo;
    /* 0x04 */ s32 state;
    /* 0x08 */ s32 subState;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 nextState;
    /* 0x14 */ s32 timer;
    /* 0x18 */ s32 unk18[25];
} M552Com;

/* One message-time row (four words). Used as a 16-byte copy unit. */
typedef struct M552ComTimeRow_s {
    s32 t0;
    s32 t1;
    s32 t2;
    s32 t3;
} M552ComTimeRow;

extern M552ComParam lbl_1_bss_4794[2];
extern M552Com lbl_1_bss_4930[2];

/* How long each message row stays up, per player. */
const M552ComTimeRow lbl_1_rodata_2B0 = { 75, 30, 10, 0 };
const M552ComTimeRow lbl_1_rodata_2C0 = { 75, 15, 10, 0 };
const M552ComTimeRow lbl_1_rodata_2D0 = { 75, 5, 1, 1 };

void fn_1_91BC(void)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        memset(&lbl_1_bss_4930[i], 0, 0x7C);
        lbl_1_bss_4930[i].msgNo = lbl_1_bss_4794[i].msgNo;
    }
}

void fn_1_9238(s32 idx)
{
    switch (lbl_1_bss_4930[idx].state) {
    case 0:
        fn_1_A7AC(idx);
        break;
    case 1:
        fn_1_A8F4(idx);
        break;
    case 3:
        fn_1_B2C0(idx);
        break;
    }
}

void fn_1_A7AC(s32 idx)
{
    s32 timeTbl[12];
    s32 time;
    s32 value;

    lbl_1_bss_4930[idx].state = 3;
    *(M552ComTimeRow *)&timeTbl[0] = lbl_1_rodata_2B0;
    *(M552ComTimeRow *)&timeTbl[4] = lbl_1_rodata_2C0;
    *(M552ComTimeRow *)&timeTbl[8] = lbl_1_rodata_2D0;
    time = 0;
    time = timeTbl[lbl_1_bss_4930[idx].msgNo - 1];
    time += rand8() % 10;
    value = time;
    lbl_1_bss_4930[idx].timer = value;
    lbl_1_bss_4930[idx].nextState = 1;
}

void fn_1_B2C0(s32 idx)
{
    lbl_1_bss_4930[idx].timer -= 1;
    if (lbl_1_bss_4930[idx].timer > 0) {
        return;
    }
    lbl_1_bss_4930[idx].state = lbl_1_bss_4930[idx].nextState;
    lbl_1_bss_4930[idx].subState = 0;
}