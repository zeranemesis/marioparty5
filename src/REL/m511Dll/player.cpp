#include "dolphin.h"

#include "game/charman.h"
#include "game/audio.h"
#include "game/object.h"
#include "game/hu3d.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
void fn_1_11ECC(struct PlayerWork *work);
void fn_1_15334(void *work, s32 arg1);
void fn_1_17CC8(void *, s32);
void fn_1_1D88(void *);
void fn_1_1D94(void *, s32);
void fn_1_1E00(void *);
void fn_1_1E54(void *, void *);
s32 fn_1_29C8(void);
void fn_1_372C(void *, s32);
void fn_1_37B0(void *, s16, s32);
void fn_1_398C(void *, void *);
void fn_1_39D4(void *, void *);
void fn_1_3B8C(void *, s32);
void fn_1_40D8(void *, void *);
void fn_1_504(void *, void *, f32);
void *fn_1_5578(void);
void fn_1_84CC(void *, void *);
void fn_1_A394(void *, void *, void *);
void fn_1_B2D4(void *, void *, void *);
void fn_1_B6C0(void *, void *, void *);
void fn_1_BA18(void *, void *, void *, f32);
void fn_1_C07C(void *);
void fn_1_C124(void *, s32);
void fn_1_C180(void *, void *, void *);
void fn_1_CF8(void *, void *, s32);
void fn_1_DAB8(void *, void *);
void fn_1_EB7C(void *, void *, void *);
void fn_1_EED8(void *, void *, void *, f32);
void fn_1_F0D8(void *);
void fn_1_10690(struct PlayerWork *, s16);
void fn_1_107AC(void *, s32);
void fn_1_11804(void *);
void fn_1_11ADC(void *);
void fn_1_11BFC(void *);
void fn_1_12570(void *, void *, void *, void *, void *);
void fn_1_1295C(void *);
void fn_1_133E8(void *, s32, s32);
void fn_1_134DC(void *);
void fn_1_13554(void *);
void fn_1_13630(void *);
void fn_1_13704(void *);
void fn_1_13794(void *);
void fn_1_1402C(void *);
void fn_1_14548(void *);
void fn_1_14A40(void *);
void fn_1_14C1C(void *);
void fn_1_152BC(void *, s32);
void fn_1_1533C(void *, void *);
void fn_1_1539C(void *);
void fn_1_153FC(void *, void *, s32, void *);
void fn_1_157AC(void *, s32);
void fn_1_158D4(void *);
void fn_1_1595C(void *);
void *__nw__FUl(u32);
void fn_1_1652C(void *, void *, void *);
void fn_1_16604(void *, void *, void *, void *);
void fn_1_16E84(void *);
void fn_1_170C8(void *, s32);
void fn_1_17544(void *, void *, void *, f32);
void fn_1_17BD8(void *);
void fn_1_17D1C(void *, void *, void *, f32);
void fn_1_17E6C(void *);
void fn_1_19190(void *, void *);

/* These are plain C in the original: m511.o defines them unmangled
   (m511.cpp is NonMatching, so the module links the original object).
   Left as C++ they mangle and the REL link fails with
   "Failed to find symbol fn_1_1D88__FPv in any module". */
extern "C" void *__dl__FPv(void *self);
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
extern void *lbl_1_bss_AC4;
extern void *lbl_1_bss_A24;
extern void *lbl_1_bss_A2C;
extern void *lbl_1_bss_A3C;
extern void *lbl_1_bss_AD0;
extern void *lbl_1_bss_AD8;
extern void *lbl_1_bss_1C;
extern void *lbl_1_data_4F8;
extern void *lbl_1_data_508;
extern void *lbl_1_data_510;
extern void *lbl_1_data_514;
extern void *lbl_1_data_518;
extern void *lbl_1_data_520;
extern void *lbl_1_data_524;
extern void *lbl_1_data_547;
extern void *lbl_1_data_552;
extern void *lbl_1_data_562;
extern void *lbl_1_data_570;
extern void *lbl_1_data_574;
extern void *lbl_1_data_578;
extern void *lbl_1_data_590;
extern void *lbl_1_data_598;
extern void *lbl_1_data_5A0;
extern void *lbl_1_data_5A8;
extern void *lbl_1_data_5B0;
extern void *lbl_1_data_5B4;
extern void *lbl_1_data_5B8;
extern void *lbl_1_data_5C0;
extern void *lbl_1_data_5C8;
extern void *lbl_1_data_5D0;
extern void *lbl_1_data_5D8;
extern void *lbl_1_data_5E0;
extern void *lbl_1_data_5E8;
extern void *lbl_1_data_5F0;
extern void *lbl_1_data_5F4;
extern void *lbl_1_data_5F8;
extern void *lbl_1_data_600;
extern void *lbl_1_data_608;
extern void *lbl_1_data_60C;
extern void *lbl_1_data_610;
extern void *lbl_1_data_614;
extern void *lbl_1_data_618;
extern void *lbl_1_data_620;
extern void *lbl_1_data_6D0;
extern void *lbl_1_data_6D4;
extern void *lbl_1_data_6D8;
extern void *lbl_1_data_6BC;
extern void *lbl_1_data_6C0;
extern void *lbl_1_data_6C8;
extern void *lbl_1_data_6EC;
extern void *lbl_1_data_6F0;
extern void *lbl_1_data_6F8;
extern void *lbl_1_data_700;
extern void *lbl_1_data_708;
extern void *lbl_1_data_710;
extern void *lbl_1_data_748;
extern void *lbl_1_data_750;
extern void *lbl_1_rodata_510;
extern void *lbl_1_rodata_514;
extern void *lbl_1_rodata_518;
extern void *lbl_1_rodata_520;
extern void *lbl_1_rodata_524;
extern void *lbl_1_rodata_547;
extern void *lbl_1_rodata_552;
extern void *lbl_1_rodata_554;
extern void *lbl_1_rodata_562;
extern void *lbl_1_rodata_570;
extern void *lbl_1_rodata_574;
extern void *lbl_1_rodata_578;
extern void *lbl_1_rodata_57C;
extern void *lbl_1_rodata_590;
extern void *lbl_1_rodata_598;
extern void *lbl_1_rodata_5A0;
extern void *lbl_1_rodata_5A8;
extern void *lbl_1_rodata_5B0;
extern void *lbl_1_rodata_5B4;
extern void *lbl_1_rodata_5B8;
extern void *lbl_1_rodata_5C0;
extern void *lbl_1_rodata_5C8;
extern void *lbl_1_rodata_5D0;
extern void *lbl_1_rodata_5D8;
extern void *lbl_1_rodata_5E0;
extern void *lbl_1_rodata_5E8;
extern void *lbl_1_rodata_5F0;
extern void *lbl_1_rodata_5F4;
extern void *lbl_1_rodata_5F8;
extern void *lbl_1_rodata_600;
extern void *lbl_1_rodata_608;
extern void *lbl_1_rodata_60C;
extern void *lbl_1_rodata_610;
extern void *lbl_1_rodata_614;
extern void *lbl_1_rodata_618;
extern void *lbl_1_rodata_620;
extern void *lbl_1_rodata_6D0;
extern void *lbl_1_rodata_6D4;
extern void *lbl_1_rodata_6D8;
extern void *lbl_1_rodata_6BC;
extern void *lbl_1_rodata_6C0;
extern void *lbl_1_rodata_6C8;
extern void *lbl_1_rodata_6EC;
extern void *lbl_1_rodata_6F0;
extern void *lbl_1_rodata_6F8;
extern void *lbl_1_rodata_700;
extern void *lbl_1_rodata_708;
extern void *lbl_1_rodata_710;
extern void *lbl_1_rodata_748;
extern void *lbl_1_rodata_750;


void fn_1_FACC(Vec *out, s32 idx)
{
    extern void *lbl_1_rodata_518;
    extern void *lbl_1_rodata_510;
    extern void *lbl_1_rodata_514;
    extern void *lbl_1_data_4F8;

    s32 val = ((s32 *)lbl_1_data_4F8)[idx];
    s32 temp = val * 0x12c - 0x1c2;
    f64 base = *(f64 *)&lbl_1_rodata_518;
    f64 diff = (f64)temp - base;
    out->x = (f32)diff;
    out->y = *(f32 *)&lbl_1_rodata_510;
    out->z = *(f32 *)&lbl_1_rodata_514;
}

void fn_1_FB50(void)
{
    extern void *lbl_1_data_4F8;
    extern void *lbl_1_data_508;

    void **data_4F8 = (void **)lbl_1_data_4F8;
    void **data_508 = (void **)lbl_1_data_508;

    for (s32 i = 0; i < 4; i++) {
        void *a = fn_1_5578(); // Returns pointer from lbl_1_data_4F8[i]
        s32 idx = (s32)a & 0x3FFFFFFF; // clrlwi r30, r3, 30

        void *ptr1 = data_4F8[i];
        void *ptr2 = data_4F8[idx];
        data_4F8[i] = ptr2;
        data_4F8[idx] = ptr1;

        a = fn_1_5578();
        idx = (s32)a & 0x3FFFFFFF;

        ptr1 = data_508[i];
        ptr2 = data_508[idx];
        data_508[i] = ptr2;
        data_508[idx] = ptr1;
    }
}

void fn_1_FC40(void)
{
    extern void *lbl_1_bss_AC4;

    u32 *bss_ac4 = (u32 *)lbl_1_bss_AC4;
    struct PlayerWork *val = (struct PlayerWork *)bss_ac4[1]; // offset 0x4

    if (val != NULL) {
        fn_1_10690(val, 1);
    }

    struct PlayerWork *check = (struct PlayerWork *)bss_ac4[0];
    if (check == NULL) {
        check = (struct PlayerWork *)bss_ac4[0];
        if (check == (struct PlayerWork *)bss_ac4[1]) {
            val = (struct PlayerWork *)1;
        }
    }

    if (val != NULL) {
        fn_1_10690(val, 1);
    }
}

void fn_1_FCE4(struct PlayerWork *work, s32 playerIdx)
{
    // TODO: Implement from target asm (large init function, 0x67C bytes)
    (void)work; (void)playerIdx;
}


void fn_1_15334(void *work, s32 arg1)
{
    *(s32 *)work = arg1;
}

void fn_1_1533C(void *work, void *arg1)
{
    // TODO: Implement from target asm
    (void)work; (void)arg1;
}

void fn_1_11ECC(struct PlayerWork *work)
{
    // Based on target assembly: fn_1_11ECC at 0x11ECC (size 0x6A4)
    // Computes velocity, distance, calls fn_1_29C8, more physics

    // Velocity = pos - prevPos
    f32 velX = work->pos.x - work->prevPos.x;
    f32 velY = work->pos.y - work->prevPos.y;
    f32 velZ = work->pos.z - work->prevPos.z;

    // Store velocity at 0xA0, 0xA4, 0xA8
    *(f32 *)((u8 *)work + 0xA0) = velX;
    *(f32 *)((u8 *)work + 0xA4) = velY;
    *(f32 *)((u8 *)work + 0xA8) = velZ;

    // Squared distance
    f32 distSq = velX * velX + velY * velY + velZ * velZ;

    // Distance approximation
    extern void *lbl_1_rodata_590;
    extern void *lbl_1_rodata_598;
    extern void *lbl_1_rodata_5A0;
    extern void *lbl_1_rodata_5B8;
    extern void *lbl_1_rodata_6EC;
    extern void *lbl_1_rodata_6F0;

    f32 dist = 0.0f;
    if (distSq > *(f64 *)&lbl_1_rodata_590) {
        dist = sqrtf(distSq);
    } else if (distSq == 0.0f) {
        dist = 0.0f;
    } else {
        dist = 1e30f;
    }

    // Store at 0xAC
    *(f32 *)((u8 *)work + 0xAC) = dist / *(f64 *)&lbl_1_rodata_5B8;

    // Compute at 0xB0
    f32 scale = dist * *(f32 *)&lbl_1_rodata_6EC;
    *(f32 *)((u8 *)work + 0xB0) = scale / *(f32 *)&lbl_1_rodata_6F0;

    // Call fn_1_29C8
    s32 result = fn_1_29C8();

    if (result != 2) return;

    // State 2: more physics
    if (*(s32 *)((u8 *)work + 0xD4) >= 5) return;

    // Call fn_1_DAB8
    f32 temp;
    fn_1_DAB8(work, &temp);

    // More vector math
    f32 f29 = temp - work->pos.x;
    f32 f28 = *(f32 *)((u8 *)work + 0x14) - work->pos.y;
    f32 f1 = f29 * f29 + f28 * f28;

    if (f1 <= *(f64 *)&lbl_1_rodata_590) return;

    // Another sqrt approximation
    f32 dist2 = sqrtf(f1);
    f32 f25 = dist2 * *(f32 *)((u8 *)work + 0xAC);
    // ... rest truncated
}

void fn_1_17CC8(void *arg0, s32 arg1)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1;
}

void fn_1_1D88(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_1D94(void *arg0, s32 arg1)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1;
}

void fn_1_1E00(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_1E54(void *arg0, void *arg1)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1;
}

s32 fn_1_29C8(void)
{
    return 0;
}

void fn_1_372C(void *arg0, s32 arg1)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1;
}

void fn_1_37B0(void *arg0, s16 arg1, s32 arg2)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1; (void)arg2;
}

void fn_1_398C(void *arg0, void *arg1)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1;
}

void fn_1_39D4(void *arg0, void *arg1)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1;
}

void fn_1_3B8C(void *arg0, s32 arg1)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1;
}

void fn_1_40D8(void *arg0, void *arg1)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1;
}

void fn_1_504(void *arg0, void *arg1, f32 arg2)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1; (void)arg2;
}

void *fn_1_5578(void)
{
    // TODO: Implement from target asm
    return NULL;
}

void fn_1_84CC(void *arg0, void *arg1)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1;
}

void fn_1_A394(void *arg0, void *arg1, void *arg2)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1; (void)arg2;
}

void fn_1_B2D4(void *arg0, void *arg1, void *arg2)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1; (void)arg2;
}

void fn_1_B6C0(void *arg0, void *arg1, void *arg2)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1; (void)arg2;
}

void fn_1_BA18(void *arg0, void *arg1, void *arg2, f32 arg3)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1; (void)arg2; (void)arg3;
}

void fn_1_C07C(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_C124(void *arg0, s32 arg1)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1;
}

void fn_1_C180(void *arg0, void *arg1, void *arg2)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1; (void)arg2;
}

void fn_1_CF8(void *arg0, void *arg1, s32 arg2)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1; (void)arg2;
}

void fn_1_DAB8(void *arg0, void *arg1)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1;
}

void fn_1_EB7C(void *arg0, void *arg1, void *arg2)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1; (void)arg2;
}

void fn_1_EED8(void *arg0, void *arg1, void *arg2, f32 arg3)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1; (void)arg2; (void)arg3;
}

void fn_1_F0D8(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_107AC(struct PlayerWork *work, s32 motionType)
{
    // fn_1_107AC at 0x107AC (size 0x7C)
    // Creates a motion for the player
    // r3 = PlayerWork*, r4 = motionType

    s16 modelID = *(s16 *)((u8 *)work + 0x60);
    s16 motionID = CharMotionCreate(modelID, (u32)motionType);
    *(s16 *)((u8 *)work + 0xC) = motionID;

    // Call fn_1_153FC with work+0xF0, work+0xF8, 1
    fn_1_153FC((u8 *)work + 0xF0, (u8 *)work + 0xF8, 1, NULL);
}

void fn_1_11804(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_11ADC(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_11BFC(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_12570(void *arg0, void *arg1, void *arg2, void *arg3, void *arg4)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1; (void)arg2; (void)arg3; (void)arg4;
}

void fn_1_1295C(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_133E8(void *arg0, s32 arg1, s32 arg2)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1; (void)arg2;
}

void fn_1_134DC(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_13554(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_13630(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_13704(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_13794(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_1402C(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_14548(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_14A40(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_14C1C(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_152BC(void *arg0, s32 arg1)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1;
}

void fn_1_1539C(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_153FC(void *arg0, void *arg1, s32 arg2, void *arg3)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1; (void)arg2; (void)arg3;
}

void fn_1_157AC(void **array, s32 index)
{
    // fn_1_157AC at 0x157AC (size 0x128)
    // Inserts element at index in a dynamic array
    // r3 = array pointer (void**), r4 = index

    void *oldArray = array[0];
    s32 count = *(s32 *)oldArray;

    if (index > count) {
        // Error: index out of bounds - print error and abort
        // fprintf(__files + 0xA0, lbl_1_data_5DD); abort();
        return;
    }

    void *newArray = __nw__FUl((count + 1) * 4); // Allocate (count+1)*4 bytes
    if (newArray == NULL) {
        // fprintf(__files + 0xA0, lbl_1_data_5C3); abort();
        return;
    }

    // Insert at index: copy elements before index
    if (index > 0) {
        memcpy(newArray, oldArray, index * 4);
    }

    // Free old array
    __dl__FPv(oldArray);

    // Update array pointer and count
    array[0] = newArray;
    *(s32 *)newArray = count + 1;
}

void fn_1_158D4(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_1595C(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_1652C(void *arg0, void *arg1, void *arg2)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1; (void)arg2;
}

void fn_1_16604(void *arg0, void *arg1, void *arg2, void *arg3)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1; (void)arg2; (void)arg3;
}

void fn_1_16E84(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_170C8(void *arg0, s32 arg1)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1;
}

void fn_1_17544(void *arg0, void *arg1, void *arg2, f32 arg3)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1; (void)arg2; (void)arg3;
}

void fn_1_17BD8(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_17D1C(void *arg0, void *arg1, void *arg2, f32 arg3)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1; (void)arg2; (void)arg3;
}

void fn_1_17E6C(void *arg0)
{
    // TODO: Implement from target asm
    (void)arg0;
}

void fn_1_19190(void *arg0, void *arg1)
{
    // TODO: Implement from target asm
    (void)arg0; (void)arg1;
}

void fn_1_108EC(PlayerWork *work)
{
    work->prevPos = work->pos;
}

void fn_1_11EA4(PlayerWork *work)
{
    fn_1_11ECC(work);
}

void fn_1_103B8(void *work)
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

void fn_1_10360(void **work, s16 arg1)
{
    if (work != NULL) {
        fn_1_17CC8(*work, 1);
        if (arg1 > 0) {
            __dl__FPv(work);
        }
    }
}
