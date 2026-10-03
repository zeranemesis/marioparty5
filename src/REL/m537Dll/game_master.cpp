#include "dolphin/types.h"

extern "C" {
u32 fn_1_0(u32 size);
void fn_1_10230(s32 arg0, s32 arg1);
void fn_1_10590(void);
void fn_1_1C48(s32 arg);
u8 fn_1_1CE0(void);
void fn_1_1D6C(void);
void fn_1_1DE0(void);
void fn_1_1F40(void *self);
void fn_1_20CC(void *self, s32 arg);
void fn_1_2704(void);
void fn_1_27BC(void);
u8 fn_1_2874(void);
void fn_1_2BEC(void);
void fn_1_2C44(void);
void fn_1_47E4(void *self);
void fn_1_4A08(void);
void fn_1_5040(void *self, u32 arg);
void fn_1_50BC(void *self, s32 arg);
void fn_1_5170(s32 self, s32 arg);
void fn_1_5200(s32 self, s32 arg);
void fn_1_5878(void *self, s32 arg);
void fn_1_5940(void *self, s32 arg);
void fn_1_599C(s32 self, s32 arg1, s32 arg2, s32 arg3);
void fn_1_5A94(s32 self, s32 arg);
void fn_1_5B88(s32 self);
u8 fn_1_5BF0(s32 self);
u32 fn_1_8964(u32 self);
u32 fn_1_9290(u32 self);
void fn_1_99D0(void);
void fn_1_9ACC(void);
void fn_1_9B64(void);
void fn_1_9BFC(void);
void fn_1_9D74(void);
void fn_1_9EE4(void);
s32 fn_1_A0C4(void);
u32 fn_1_FF4C(u32 self);
s32 HuAudBGMPlay(s32 arg);
void HuAudFXPlay(s32 arg);
void HuAudJinglePlay(s32 arg);
void HuAudSStreamFadeOut(s32 arg1, s32 arg2);
void Hu3DShadowCreate(f32 fov, f32 near, f32 far);
void Hu3DShadowPosSet(void *pos, void *up, void *target);
void Hu3DShadowSizeSet(u16 size);
void Hu3DShadowTPLvlSet(f32 lvl);
u32 OSGetTick(void);

typedef struct M537GameMaster_s {
    void *unk0;
    void *unk4;
    u8 pad0[0x20 - 0x8];
    void *unk20;
    u8 pad1[0x358 - 0x24];
    void *unk358;
} M537GameMaster;

s32 lbl_1_bss_A00;
s32 lbl_1_bss_A04;
s32 lbl_1_bss_A08;
s32 lbl_1_bss_A0C;
s32 lbl_1_bss_A10;
s32 lbl_1_bss_A14;

typedef struct M537Shadow_s {
    s32 v[9];
} M537Shadow;

typedef struct M537ShadowFov_s {
    f32 v[2];
} M537ShadowFov;

extern const M537ShadowFov lbl_1_rodata_1F8 = {{288.0f, 0.0f}};
extern const M537Shadow lbl_1_rodata_200 = {
    {0x0, 0x45FA0000, 0x0, 0x0, 0x0, 0xBF800000, 0x0, 0x0, 0x0},
};

void *fn_1_83CC(void *self);
void *fn_1_8438(void *self, s16 arg);
void fn_1_84C4(void);
void fn_1_8604(s32 arg);
void fn_1_8640(s32 arg);
u8 fn_1_87C4(s32 arg);
void fn_1_881C(s32 arg);
void fn_1_8848(void);
void fn_1_888C(void);
void fn_1_88C0(void);
void fn_1_88C8(void);

void *lbl_1_data_320[] = {
    0,
    0,
    fn_1_8438,
    fn_1_84C4,
    fn_1_8604,
    fn_1_8848,
    0,
    0,
    fn_1_88C8,
    fn_1_88C0,
    fn_1_881C,
    fn_1_8640,
};

void *fn_1_83CC(void *self)
{
    fn_1_1F40(self);
    fn_1_5040((u8 *)self + 0x20, 0x8000);
    ((M537GameMaster *)self)->unk4 = lbl_1_data_320;
    ((M537GameMaster *)self)->unk20 = (void *)((u8 *)lbl_1_data_320 + 0x18);
    fn_1_5878((u8 *)self + 0x358, 0);
    return self;
}

void *fn_1_8438(void *self, s16 arg)
{
    if (self != 0) {
        ((M537GameMaster *)self)->unk4 = lbl_1_data_320;
        ((M537GameMaster *)self)->unk20 = (void *)((u8 *)lbl_1_data_320 + 0x18);
        fn_1_5940((u8 *)self + 0x358, -1);
        fn_1_50BC((u8 *)self + 0x20, 0);
        fn_1_20CC(self, 0);
        if (arg > 0) {
            fn_1_47E4(self);
        }
    }
    return self;
}

void fn_1_84C4(void)
{
    M537Shadow sp;
    u32 temp1;
    u32 temp2;
    u32 temp3;

    OSGetTick();
    fn_1_4A08();
    sp = lbl_1_rodata_200;
    Hu3DShadowCreate(30.0f, 6800.0f, 7200.0f);
    Hu3DShadowPosSet(&sp.v[0], &sp.v[3], &sp.v[6]);
    Hu3DShadowSizeSet(0xF0);
    Hu3DShadowTPLvlSet(0.4f);
    temp1 = fn_1_0(0x34);
    if (temp1 != 0) {
        temp1 = fn_1_9290(temp1);
    }
    temp2 = fn_1_0(0xB8);
    if (temp2 != 0) {
        temp2 = fn_1_8964(temp2);
    }
    fn_1_99D0();
    temp3 = fn_1_0(0x38);
    if (temp3 != 0) {
        temp3 = fn_1_FF4C(temp3);
    }
}

void fn_1_8604(s32 arg0)
{
    fn_1_5170(arg0 + 0x20, 0);
    fn_1_1D6C();
    fn_1_9ACC();
    fn_1_10590();
}

#pragma dont_inline on
void fn_1_8640(s32 arg0)
{
    s32 var1;
    s32 var2;
    s16 var3;
    s32 var4;

    fn_1_1C48(0);
    fn_1_2BEC();
    HuAudFXPlay(0x7DC);
    while (fn_1_1CE0() == 0) {
        fn_1_5200(arg0 + 0x20, 0);
    }
    var4 = HuAudBGMPlay(0x36);
    fn_1_1C48(1);
    fn_1_2704();
    while (fn_1_2874() == 0) {
        fn_1_5200(arg0 + 0x20, 0);
    }
    fn_1_1C48(2);
    fn_1_9B64();
    fn_1_599C(arg0 + 0x358, 0xE10, 0, 0);
    fn_1_5A94(arg0 + 0x358, 1);
    while (fn_1_87C4(arg0) == 0) {
        fn_1_5200(arg0 + 0x20, 0);
    }
    fn_1_5B88(arg0 + 0x358);
    fn_1_1C48(3);
    fn_1_9BFC();
    fn_1_27BC();
    HuAudSStreamFadeOut(var4, 0x64);
    for (var1 = 0; var1 < 0xB4; var1++) {
        fn_1_5200(arg0 + 0x20, 0);
    }
    fn_1_1C48(4);
    fn_1_9D74();
    if (fn_1_A0C4() >= 4) {
        var3 = 0x7B;
    } else {
        var3 = 0x79;
    }
    HuAudJinglePlay(var3);
    for (var2 = 0; var2 < lbl_1_bss_A08; var2++) {
        fn_1_5200(arg0 + 0x20, 0);
    }
    fn_1_2C44();
    while (1) {
        fn_1_5200(arg0 + 0x20, -1);
    }
}

u8 fn_1_87C4(s32 arg0)
{
    u8 res;

    res = 1;
    if ((fn_1_5BF0(arg0 + 0x358) == 0) && (fn_1_A0C4() > 1)) {
        res = 0;
    }
    return res;
}

void fn_1_881C(s32 arg0)
{
    fn_1_5BF0(arg0 + 0x358);
}

void fn_1_8848(void)
{
    s32 var;

    fn_1_9EE4();
    fn_1_1DE0();
    var = lbl_1_bss_A0C;
    fn_1_10230(var, 1);
}

void fn_1_888C(void)
{
    lbl_1_bss_A00 = 0x78;
    lbl_1_bss_A04 = 0xD2;
    lbl_1_bss_A08 = 0xD2;
}

#pragma dont_inline on
void fn_1_88C0(void)
{
    __asm {
        subi r3, r3, 0x20
        b      fn_1_8640
    }
}
#pragma dont_inline reset

#pragma dont_inline on
void fn_1_88C8(void)
{
    __asm {
        subi r3, r3, 0x20
        b      fn_1_8438
    }
}
#pragma dont_inline reset
}
