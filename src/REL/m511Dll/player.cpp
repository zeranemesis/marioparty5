#include "dolphin.h"

#include "game/charman.h"

extern "C" {
void fn_1_11ECC(struct PlayerWork *work);
void fn_1_15334(struct PlayerWork *work, s32 arg1);
void fn_1_17CC8(s32 arg0, s32 arg1);
/* These are plain C in the original: m511.o defines them unmangled
   (m511.cpp is NonMatching, so the module links the original object).
   Left as C++ they mangle and the REL link fails with
   "Failed to find symbol fn_1_1D88__FPv in any module". */
extern "C" void *__dl__FPv(void *self);
void CharFXPlay(s16 arg0, s16 arg1);
}

struct PlayerWork {
    u8 pad00[0x88];
    Vec pos;       // 0x88
    Vec prevPos;   // 0x94
    u8 pad9C[0x34];
    s32 unkD0;     // 0xD0
    s32 unkD4;     // 0xD4
    s32 unkD8;     // 0xD8
    s32 unkDC;     // 0xDC
    u8 padE0[0xC];
    s8 unkEC;      // 0xEC
    u8 padED[3];
    s32 unkF0;     // 0xF0
    s32 unkF4;     // 0xF4
    s32 unkF8;     // 0xF8
    s32 unkFC;     // 0xFC
    s32 unk100;    // 0x100
    s32 unk104;    // 0x104
    s32 unk108;    // 0x108
    s32 unk10C;    // 0x10C
    s32 unk110;    // 0x110
    s32 unk114;    // 0x114
    s32 unk118;    // 0x118
    s32 unk11C;    // 0x11C
    s32 unk120;    // 0x120
    s32 unk124;    // 0x124
    s32 unk128;    // 0x128
    s32 unk12C;    // 0x12C
    s32 unk130;    // 0x130
    s32 unk134;    // 00x134
    s32 unk138;    // 0x138
    s32 unk13C;    // 0x13C
    s32 unk140;    // 0x140
    s32 unk144;    // 0x144
    s32 unk148;    // 0x148
    s32 unk14C;    // 0x14C
    s32 unk150;    // 0x150
    s32 unk154;    // 0x154
    s32 unk158;    // 0x158
    s32 unk15C;    // 0x15C
    s32 unk160;    // 0x160
    s32 unk164;    // 0x164
    s32 unk168;    // 0x168
    s32 unk16C;    // 0x16C
    s32 unk170;    // 0x170
    s32 unk174;    // 0x174
    s32 unk178;    // 0x178
    s32 unk17C;    // 0x17C
    s32 unk180;    // 0x180
    s32 unk184;    // 0x184
    s32 unk188;    // 0x188
    s32 unk18C;    // 0x18C
    s32 unk190;    // 0x190
    s32 unk194;    // 0x194
    s32 unk198;    // 0x198
    s32 unk19C;    // 0x19C
    s32 unk1A0;    // 0x1A0
    s32 unk1A4;    // 0x1A4
    s32 unk1A8;    // 0x1A8
    s32 unk1AC;    // 0x1AC
    s32 unk1B0;    // 0x1B0
    s32 unk1B4;    // 0x1B4
    s32 unk1B8;    // 0x1B8
    s32 unk1BC;    // 0x1BC
    s32 unk1C0;    // 0x1C0
    s32 unk1C4;    // 0x1C4
    s32 unk1C8;    // 0x1C8
    s32 unk1CC;    // 0x1CC
    s32 unk1D0;    // 0x1D0
    s32 unk1D4;    // 0x1D4
    u8 pad1D8[0x1];
    u8 pad1D9[0x1];
    u8 pad1DA[0x1];
    u8 unk1DB;     // 0x1DB
    u8 pad1DC[4];
    s32 unk1E0;    // 0x1E0
    s32 unk1E4;    // 0x1E4
    s32 unk1E8;    // 0x1E8
    s32 unk1EC;    // 0x1EC
    s32 unk1F0;    // 0x1F0
    s32 unk1F4;    // 0x1F4
    s32 unk1F8;    // 0x1F8
    s32 unk1FC;    // 0x1FC
    s32 unk200;    // 0x200
    s32 unk204;    // 0x204
    s32 unk208;    // 0x208
    s32 unk20C;    // 0x20C
    s32 unk210;    // 0x210
    s32 unk214;    // 0x214
    s32 unk218;    // 0x218
    s32 unk21C;    // 0x21C
    s32 unk220;    // 0x220
    s32 unk224;    // 0x224
    s32 unk228;    // 0x228
    s32 unk22C;    // 0x22C
    s32 unk230;    // 0x230
    s32 unk234;    // 0x234
    s32 unk238;    // 0x238
    s32 unk23C;    // 0x23C
    s32 unk240;    // 0x240
    s32 unk244;    // 0x244
    s32 unk248;    // 0x248
    s32 unk24C;    // 0x24C
    s32 unk250;    // 0x250
    s32 unk254;    // 0x254
    s32 unk258;    // 0x258
    s32 unk25C;    // 0x25C
    s32 unk260;    // 0x260
    s32 unk264;    // 0x264
    s32 unk268;    // 0x268
    s32 unk26C;    // 0x26C
    s32 unk270;    // 0x270
    s32 unk274;    // 0x274
    s32 unk278;    // 0x278
    s32 unk27C;    // 0x27C
    s32 unk280;    // 0x280
    s32 unk284;    // 0x284
    s32 unk288;    // 0x288
    s32 unk28C;    // 0x28C
    s32 unk290;    // 0x290
    s32 unk294;    // 0x294
    s32 unk298;    // 0x298
    s32 unk29C;    // 0x29C
    s32 unk2A0;    // 0x2A0
    s32 unk2A4;    // 0x2A4
    s32 unk2A8;    // 0x2A8
    s32 unk2AC;    // 0x2AC
    s32 unk2B0;    // 0x2B0
    s32 unk2B4;    // 0x2B4
    s32 unk2B8;    // 0x2B8
    s32 unk2BC;    // 0x2BC
    s32 unk2C0;    // 0x2C0
    s32 unk2C4;    // 0x2C4
    s32 unk2C8;    // 0x2C8
    s32 unk2CC;    // 0x2CC
    s32 unk2D0;    // 0x2D0
    s32 unk2D4;    // 0x2D4
    s32 unk2D8;    // 0x2D8
    s32 unk2DC;    // 0x2DC
    s32 unk2E0;    // 0x2E0
    s32 unk2E4;    // 0x2E4
    s32 unk2E8;    // 0x2E8
    s32 unk2EC;    // 0x2EC
    s32 unk2F0;    // 0x2F0
    s32 unk2F4;    // 0x2F4
    s32 unk2F8;    // 0x2F8
    s32 unk2FC;    // 0x2FC
    s32 unk300;    // 0x300
    s32 unk304;    // 0x304
    s32 unk308;    // 0x308
    s32 unk30C;    // 0x30C
    s32 unk310;    // 0x310
    s32 unk314;    // 0x314
    s32 unk318;    // 0x318
    s32 unk31C;    // 0x31C
    s32 unk320;    // 0x320
    s32 unk324;    // 0x324
    s32 unk328;    // 0x328
    s32 unk32C;    // 0x32C
    s32 unk330;    // 0x330
    s32 unk334;    // 0x334
    s32 unk338;    // 0x338
    s32 unk33C;    // 0x33C
    s32 unk340;    // 0x340
    s32 unk344;    // 0x344
    s32 unk348;    // 0x348
    s32 unk34C;    // 0x34C
    s32 unk350;    // 0x350
    s32 unk354;    // 0x354
    s32 unk358;    // 0x358
    s32 unk35C;    // 0x35C
    s32 unk360;    // 0x360
    s32 unk364;    // 0x364
    s32 unk368;    // 0x368
    s32 unk36C;    // 0x36C
    s32 unk370;    // 0x370
};

extern void *lbl_1_bss_AB4;

void fn_1_108EC(PlayerWork *work)
{
    work->prevPos = work->pos;
}

void fn_1_11EA4(PlayerWork *work)
{
    fn_1_11ECC(work);
}

void fn_1_103B8(PlayerWork *work)
{
    fn_1_15334(work, 0);
}

void fn_1_10FA4(void)
{
    void *temp = lbl_1_bss_AB4;
    if (temp != NULL) {
        CharFXPlay((s16)((u32)*(s32 *)((u8 *)temp + 0x60)), 0x1FA);
    }
}

void fn_1_11170(void)
{
    void *temp = lbl_1_bss_AB4;
    if (temp != NULL) {
        CharFXPlay((s16)((u32)*(s32 *)((u8 *)temp + 0x60)), 0x1FA);
    }
}

void fn_1_10360(PlayerWork *work, s16 arg1)
{
    if (work != NULL) {
        fn_1_17CC8(*(s32 *)work, 1);
        if (arg1 > 0) {
            __dl__FPv(work);
        }
    }
}
