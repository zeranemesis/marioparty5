#include "dolphin/types.h"

extern "C" {
void *fn_1_2B68(void *self);
void *fn_1_7C8(void *self, u32 val);
void *fn_1_4D3C(void *self, s32 val);
void setSeed__8CMT19937Fi(void *self, s32 seed);
void fn_1_4E04(void *self, s32 val);
void *fn_1_9E8(void *self, u32 val);
void *fn_1_2CF4(void *self, u32 val);
void *fn_1_2944(void *self);
void *fn_1_CED0(void *self);
void *fn_1_104(s32 size);
void *fn_1_EB14(void *self);
void *fn_1_DEEC(void *self);
void *fn_1_D98C(void *self);
void *fn_1_5A7C(void *self);
void fn_1_0(const char *str1, const char *str2, s32 line);
void fn_1_3888(u32 val);
void fn_1_381C(void);
void *fn_1_F1FC(void *self, s32 idx);
void fn_1_FF60(void);
void *fn_1_3334(void);
u8 fn_1_34A4(void);
u32 irandom__7CRandomFv(void);
void fn_1_C224(void *self, u32 val);
s32 fn_1_E1A0(void *self);
void fn_1_FA3C(void);
void fn_1_504C(void *self);
void HuAudFXPlay(s32 id);
u32 HuAudBGMPlay(s32 id);
void HuAudSStreamFadeOut(s32 id, s32 time);
void fn_1_33EC(void);
u8 fn_1_3874(void);
void fn_1_3578(void);
void HuAudJinglePlay(s32 id);
void fn_1_CE8(void *self, s32 val);
u32 fn_1_50B4(void *self);
u8 fn_1_F864(u32 val);
void fn_1_F0F4(void);
void *fn_1_3A20(void);
void HuAudFXPlayVol(s32 id, s32 vol);
u32 OSGetTick(void);
void setSeed__7CRandomFi(u32 seed);

void *fn_1_BEC(void *self, u32 val);
void fn_1_39AC(void);
void fn_1_F7E0(void);
void fn_1_4E60(void *self, u32 a, u32 b, u32 c);
void fn_1_4F58(void *self, u32 val);
void fn_1_21C(void *self);
void fn_1_FDB0(void);

extern u32 lbl_1_bss_30;
extern u32 lbl_1_bss_38;
extern u32 lbl_1_bss_3C;
extern u32 lbl_1_bss_40;

const char lbl_1_data_3280[] = "m509::CGameMaster::~CGameMaster()";
const char lbl_1_data_32A2[] = "%s\n";
const char lbl_1_data_32A6[] = "ms_pointer == 0";
const char lbl_1_data_32B6[] = "singleton.h";
const char lbl_1_data_32C2[] = "ms_pointer == 0";

int printf(const char *fmt, ...);

// singleton.h reaches the singleton through its address, so every access is a
// lis/addi pair followed by a load or store at offset 0.
#define SINGLETON (*(u32 *)&lbl_1_bss_30)

typedef void (*VoidFunc)(void *self);

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec;

void fn_1_C3E8(void *self);
void fn_1_C4A8(void *self);
void fn_1_C4E0(void *self);
s32 fn_1_C85C(void *self);
void fn_1_C8B8(void *self);
void fn_1_C8BC(void *self);
void fn_1_C8E0(void);
void fn_1_C914(void *self);
void fn_1_C91C(void *self);
void *__dt__Q24m50911CGameMasterFv(void *self, s16 flag);
void *fn_1_C2D0(void *self);

// Two virtual tables for m509::CGameMaster. The first covers the subobject at
// this+0x4, the second (at +0x18) the subobject at this+0x20; its entries are
// adjustor thunks that give back 0x20 before reaching the real implementation.
VoidFunc lbl_1_data_32D4[13] = {
    (VoidFunc)0,
    (VoidFunc)0,
    (VoidFunc)__dt__Q24m50911CGameMasterFv,
    (VoidFunc)fn_1_C3E8,
    (VoidFunc)fn_1_C4A8,
    (VoidFunc)fn_1_C8BC,
    (VoidFunc)0,
    (VoidFunc)0,
    (VoidFunc)fn_1_C91C,
    (VoidFunc)fn_1_C914,
    (VoidFunc)fn_1_C8B8,
    (VoidFunc)fn_1_C4E0,
};

void *__dt__Q24m50911CGameMasterFv(void *self, s16 flag)
{
    if (self) {
        *(VoidFunc **)((u32)self + 4) = lbl_1_data_32D4;
        *(VoidFunc **)((u32)self + 0x20) = &lbl_1_data_32D4[6];
        printf(lbl_1_data_32A2, lbl_1_data_3280);
        fn_1_4E04((void *)((u32)self + 0x358), -1);
        fn_1_9E8((void *)((u32)self + 0x20), 0);
        fn_1_2CF4(self, 0);
        if (flag > 0) {
            fn_1_2944(self);
        }
    }
    return self;
}

void fn_1_C3E8(void *self)
{
    setSeed__7CRandomFi(OSGetTick());
    void *r30 = fn_1_104(0x14);
    if (r30) {
        r30 = fn_1_EB14(r30);
    }
    void *r29 = fn_1_104(0x48);
    if (r29) {
        r29 = fn_1_DEEC(r29);
    }
    for (s32 i = 0; i < 4; i++) {
        void *r28 = fn_1_104(0x7C);
        if (r28) {
            r28 = fn_1_F1FC(r28, i);
        }
    }
    fn_1_FF60();
}

void fn_1_C4A8(void *self)
{
    fn_1_BEC((void *)((u32)self + 0x20), 0);
    fn_1_39AC();
    fn_1_F7E0();
}

// NONMATCHING: temporaries land three registers lower than the original's.
// The original uses r30/r29/r28 then jumps to r22/r21; MWCC here starts
// at r27/r26/r25. Declaration order decides the ladder (BRIEF 6).
void fn_1_C4E0(void *self)
{
    void *r30 = fn_1_104(0xCC);
    if (r30) {
        r30 = fn_1_CED0(r30);
    }
    void *r29 = fn_1_104(0x28);
    if (r29) {
        r29 = fn_1_D98C(r29);
    }
    void *r28 = fn_1_104(0x418);
    if (r28) {
        r28 = fn_1_5A7C(r28);
    }
    void *r22 = fn_1_104(1);
    if (r22) {
        void *ms = r22;
        if (!lbl_1_bss_30) {
            fn_1_0(lbl_1_data_32A6, lbl_1_data_32B6, 0x35);
        }
        lbl_1_bss_30 = (u32)ms;
    }
    fn_1_3888(0);
    fn_1_381C();
    HuAudFXPlay(0x692);
    for (s32 i = 0; i < 0xB4; i++) {
        fn_1_CE8((void *)((u32)self + 0x20), 0);
    }
    fn_1_3888(1);
    u32 r19 = HuAudBGMPlay(0x3E);
    fn_1_3334();
    while (!fn_1_34A4()) {
        fn_1_CE8((void *)((u32)self + 0x20), 0);
    }
    s32 r23 = 0;
    s32 r25 = 0;
    s32 r26 = 0;
    fn_1_3888(2);
    fn_1_4E60((void *)((u32)self + 0x358), 0xA8C, 0, 0);
    fn_1_4F58((void *)((u32)self + 0x358), 1);
    while (true) {
        s32 r0 = r25 * 0x3C;
        if (r26 < r0) {
            Vec vec = {0.0f, 0.0f, 0.0f};
            if (!lbl_1_bss_30) {
                fn_1_0(lbl_1_data_32C2, lbl_1_data_32B6, 0x27);
            }
            u32 ms = lbl_1_bss_30;
            fn_1_C224((void *)ms, irandom__7CRandomFv());
            HuAudFXPlayVol(0x68F, fn_1_E1A0(&vec));
            fn_1_FA3C();
            r23++;
            r25 += 2;
            r26++;
        } else {
            fn_1_CE8((void *)((u32)self + 0x20), 0);
            if (fn_1_C85C(self) == 0) continue;
            break;
        }
    }
    fn_1_504C((void *)((u32)self + 0x35C));
    fn_1_3888(3);
    HuAudSStreamFadeOut(r19, 0x64);
    fn_1_33EC();
    for (s32 i = 0; i < 0xB4; i++) {
        fn_1_CE8((void *)((u32)self + 0x20), -1);
    }
    fn_1_3888(4);
    fn_1_FDB0();
    s16 r20 = fn_1_3874() ? 0x7B : 0x79;
    HuAudJinglePlay(r20);
    fn_1_3578();
    while (!fn_1_34A4()) {
        fn_1_CE8((void *)((u32)self + 0x20), 0);
    }
    fn_1_3874();
    while (true) {
        fn_1_CE8((void *)((u32)self + 0x20), -1);
    }
}

// NONMATCHING: the singleton.h assert block is out of line in the original
// (beq to the assert, b over it); MWCC 2.6 always inlines it (BRIEF 6).
void *fn_1_C7D0(void *self, s16 arg)
{
    if (self) {
        if (SINGLETON == 0) {
            fn_1_0(lbl_1_data_32C2, lbl_1_data_32B6, 0x3D);
        }
        SINGLETON = 0;
        if (arg > 0) {
            fn_1_21C(self);
        }
    }
    return self;
}

// NONMATCHING: the original keeps the first call's result in r3 and passes it
// straight to the second; MWCC copies a twice-read local into r30 instead.
s32 fn_1_C85C(void *self)
{
    s32 ret = 1;
    u32 val = fn_1_50B4((void *)((u32)self + 0x358));
    if ((u8)val != 0) {
        ret = 0;
    } else if (fn_1_F864(val) == 0) {
        ret = 0;
    }
    return ret;
}

void fn_1_C8B8(void *self)
{
}

void fn_1_C8BC(void *self)
{
    fn_1_F0F4();
    fn_1_3A20();
}

void fn_1_C8E0(void)
{
    lbl_1_bss_38 = 0x78;
    lbl_1_bss_3C = 0xD2;
    lbl_1_bss_40 = 0xD2;
}

// NONMATCHING: virtual-base adjustor thunk. The original tail-branches
// (b, not bl); MWCC -O0 never emits a tail call.
void fn_1_C914(void *self)
{
    fn_1_C4E0((void *)((u32)self - 0x20));
}

// NONMATCHING: virtual-base adjustor thunk, same reason as fn_1_C914.
void fn_1_C91C(void *self)
{
    __dt__Q24m50911CGameMasterFv((void *)((u32)self - 0x20), 1);
}

void *fn_1_C2D0(void *self)
{
    fn_1_2B68(self);
    fn_1_7C8((void *)((u32)self + 0x20), 0x10000);
    *(VoidFunc **)((u32)self + 4) = lbl_1_data_32D4;
    *(VoidFunc **)((u32)self + 0x20) = &lbl_1_data_32D4[6];
    fn_1_4D3C((void *)((u32)self + 0x358), 0);
    setSeed__8CMT19937Fi((void *)((u32)self + 0x35C), 0x1571);
    return self;
}
}