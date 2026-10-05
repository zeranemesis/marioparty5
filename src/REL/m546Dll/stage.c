#include "types.h"

typedef struct MgStageSlot {
    s16 unk0;
    s16 unk2;
    s16 confIdx; // 0x04
    s16 conf0;   // 0x06
    s16 conf4p1; // 0x08
    s16 conf2;   // 0x0A
    u8 pad_0C[0x10];
} MgStageSlot; // 0x1C

typedef struct MgStage {
    MgStageSlot slots[2]; // 0x00
    u8 pad_038[4];
    s32 unk3C;
    s32 *motSlots[2]; // 0x40
    s16 *motLinks[2]; // 0x48
    u8 pad_050[0x88];
    s32 unkD8;
    u8 pad_0DC[0x58];
    s32 unk134;
    u8 pad_138[0x118];
} MgStage; // 0x250

typedef struct PlayerConf {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
} PlayerConf; // 0xA

extern PlayerConf GwPlayerConf[4];

typedef void (*StageInitFn)(void);

void fn_1_C2BC(void);
void fn_1_C8C8(void);
void fn_1_CE14(void);
void fn_1_D6F8(void);
void fn_1_E14C(void);

static StageInitFn lbl_1_data_578[5] = {
    fn_1_C2BC,
    fn_1_C8C8,
    fn_1_CE14,
    fn_1_D6F8,
    fn_1_E14C,
};

static MgStage *lbl_1_bss_15298;

void fn_1_B15C(MgStage *arg0, s32 arg1, s32 arg2)
{
    s16 x;
    s16 i;
    s16 j;

    lbl_1_bss_15298 = arg0;
    memset(lbl_1_bss_15298, 0, 0x250);
    lbl_1_bss_15298->unk3C = arg1;
    lbl_1_bss_15298->unkD8 = arg2;

    i = 0;
    do {
        for (j = 0; j < 4; j++) {
            if (i != (x = GwPlayerConf[j].unk6)) {
                continue;
            }
            lbl_1_bss_15298->slots[i].confIdx = j;
            i++;
            if (i >= 2) {
                break;
            }
        }
    } while (i < 2);

    for (j = 0; j < 2; j++) {
        lbl_1_bss_15298->slots[j].conf0 = GwPlayerConf[lbl_1_bss_15298->slots[j].confIdx].unk0;
        lbl_1_bss_15298->slots[j].conf2 = GwPlayerConf[lbl_1_bss_15298->slots[j].confIdx].unk2;
        x = GwPlayerConf[lbl_1_bss_15298->slots[j].confIdx].unk8;
        if (x != 0) {
            lbl_1_bss_15298->slots[j].conf4p1 = GwPlayerConf[lbl_1_bss_15298->slots[j].confIdx].unk4 + 1;
        }
    }
}

void fn_1_B654(s32 *arg0, s32 *arg1)
{
    lbl_1_bss_15298->motSlots[0] = arg0;
    lbl_1_bss_15298->motSlots[1] = arg1;
}

void fn_1_B678(s16 *arg0, s16 *arg1)
{
    lbl_1_bss_15298->motLinks[0] = arg0;
    lbl_1_bss_15298->motLinks[1] = arg1;
}

void fn_1_C250(void)
{
    s32 i;

    for (i = 0; i < 5; i++) {
        lbl_1_data_578[i]();
    }
}

void fn_1_C2A4(void)
{
    lbl_1_bss_15298->unk134 = 0;
}