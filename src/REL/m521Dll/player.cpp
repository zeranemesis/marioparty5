#include "dolphin/types.h"

extern "C" {

typedef struct PlayerConf {
    s8 unk00[0x08];
    s16 unk08;
} PlayerConf;  // size 0xA (10 bytes per entry)

typedef struct Player {
    s8 unk00[0x0C];
    f32 unk0C;
    s8 unk10[0x08];
    s32 unk18;
    s8 unk1C[0x64];
    s32 unk80;
} Player;

extern PlayerConf GwPlayerConf[];
extern s32 fn_1_17218(Player *self);

extern const f32 lbl_1_rodata_338[1] = { 0.7f };

s32 lbl_1_bss_AA8;
s32 lbl_1_bss_AAC;
s32 lbl_1_bss_AB0;

bool fn_1_15554(Player *self)
{
    return self->unk0C >= lbl_1_rodata_338[0];
}

bool fn_1_15D68(Player *self)
{
    return GwPlayerConf[self->unk18].unk08 != 0;
}

void fn_1_173B8(Player *self)
{
    if ((fn_1_17218(self) & 0xFF) != 0u) {
        return;
    }
}

s32 fn_1_17760(Player *self)
{
    switch (self->unk80) {
    case 2:
    case 3:
    case 4:
        return 1;
    default:
        return 0;
    }
}

void fn_1_17F44(void)
{
    lbl_1_bss_AA8 = 0x78;
    lbl_1_bss_AAC = 0xD2;
    lbl_1_bss_AB0 = 0xD2;
}
}