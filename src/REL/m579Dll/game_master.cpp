#include "dolphin/types.h"

extern "C" {
typedef void (*VoidFunc)(void);

class M579GameMaster {
public:
    u32 field_0;
    void **vtable;
    u8 field_8[0x18];
    void *field_20;
    u8 field_24[0x334];
    u8 field_358[0x40];
};

void *fn_1_0(s32 size);

void *fn_1_97CC(void *self);
void *fn_1_B2C8(void *self);
void *fn_1_BA70(void *self);
void *fn_1_8B0C(void *self);
void *fn_1_10C84(void *self);
void *fn_1_14208(void *self);
void *fn_1_148AC(void *self);
void *fn_1_15AD8(void *self);
void *fn_1_16F3C(void *self);
void fn_1_CA2C(void);
void *fn_1_CAD0(void *self, s32 index);
void fn_1_D2C0(void);

void fn_1_1C48(s32 mode);
u32 fn_1_1CE0(void);
void fn_1_1D6C(void);
void fn_1_1DE0(void);
void fn_1_1F40(void *self);
void fn_1_20CC(void *self, s32 flag);
void fn_1_2704(void);
void fn_1_27BC(void);
u32 fn_1_2874(void);
void fn_1_2948(void);
void fn_1_2BEC(void);
void fn_1_2C44(void);
void *fn_1_47E4(void *self);
void fn_1_4A08(void);
void fn_1_5268(void *self, s32 value);
void fn_1_52E4(void *self, s32 value);
void fn_1_5398(void *self, s32 value);
void fn_1_5428(void *self, s32 value);
void *fn_1_5AA0(void *self, s32 value);
void *fn_1_5B68(void *self, s32 value);
void fn_1_5BC4(void *self, s32 a, s32 b, s32 c);
void fn_1_5CBC(void *self, s32 value);
void fn_1_5C3C(void *self, f32 a, f32 b);
void fn_1_5DB0(void *self);
u32 fn_1_5E18(void *self);
void fn_1_D358(void);
void fn_1_D594(void);
u32 fn_1_DE88(void);

void CharEffectLayerSet(s32 layer);
s32 HuAudBGMPlay(s32 id);
void HuAudFXPlay(s32 id);
void HuAudJinglePlay(s16 id);
void HuAudSStreamFadeOut(s32 seq, s32 fade);
u32 OSGetTick(void);

u32 lbl_1_bss_A08;
u32 lbl_1_bss_A0C;
u32 lbl_1_bss_A10;
u32 lbl_1_bss_A14;
u32 lbl_1_bss_A18;
u32 lbl_1_bss_A1C;
u32 lbl_1_bss_A20;
u32 lbl_1_bss_A24;

void *fn_1_7650(M579GameMaster *self);
void *fn_1_76BC(M579GameMaster *self, s16 flag);
void fn_1_7748(void);
void fn_1_78DC(void *self);
void fn_1_7914(M579GameMaster *self);
u32 fn_1_7AB4(M579GameMaster *self);
void fn_1_7AE0(void);
void fn_1_7AE4(M579GameMaster *self);
void fn_1_7B08(void);
void fn_1_7B3C(void *self);
void fn_1_7B44(void *self);

const f32 lbl_1_rodata_170 = 292.0f;
const f32 lbl_1_rodata_174 = 416.0f;

void *lbl_1_data_280[12] = {
    0, 0, fn_1_76BC, fn_1_7748, fn_1_78DC, fn_1_7AE4,
    0, 0, fn_1_7B44, fn_1_7B3C, fn_1_7AE0, fn_1_7914
};

void *fn_1_7650(M579GameMaster *self)
{
    fn_1_1F40(self);
    fn_1_5268(&self->field_20, 0x8000);
    self->vtable = lbl_1_data_280;
    self->field_20 = &lbl_1_data_280[6];
    fn_1_5AA0(self->field_358, 0);
    return self;
}

void *fn_1_76BC(M579GameMaster *self, s16 flag)
{
    if (self != 0) {
        self->vtable = lbl_1_data_280;
        self->field_20 = &lbl_1_data_280[6];
        fn_1_5B68(self->field_358, -1);
        fn_1_52E4(&self->field_20, 0);
        fn_1_20CC(self, 0);
        if (flag > 0) {
            fn_1_47E4(self);
        }
    }
    return self;
}

void fn_1_7748(void)
{
    s32 i;
    void *p1;
    void *p2;
    void *p3;
    void *p4;
    void *p5;
    void *p6;
    void *p7;
    void *p8;
    void *p9;
    void *p10;

    OSGetTick();
    fn_1_4A08();

    p1 = fn_1_0(0x1c);
    if (p1 != 0) {
        p1 = fn_1_97CC(p1);
    }

    p2 = fn_1_0(0x10);
    if (p2 != 0) {
        p2 = fn_1_BA70(p2);
    }

    p3 = fn_1_0(0x14);
    if (p3 != 0) {
        p3 = fn_1_B2C8(p3);
    }

    p4 = fn_1_0(0x74);
    if (p4 != 0) {
        p4 = fn_1_8B0C(p4);
    }

    p5 = fn_1_0(0x84);
    if (p5 != 0) {
        p5 = fn_1_14208(p5);
    }

    p6 = fn_1_0(0x10);
    if (p6 != 0) {
        p6 = fn_1_10C84(p6);
    }

    p7 = fn_1_0(0x20);
    if (p7 != 0) {
        p7 = fn_1_15AD8(p7);
    }

    p8 = fn_1_0(0x130);
    if (p8 != 0) {
        p8 = fn_1_148AC(p8);
    }

    p9 = fn_1_0(0x68);
    if (p9 != 0) {
        p9 = fn_1_16F3C(p9);
    }

    CharEffectLayerSet(3);

    for (i = 0; i < 4; i++) {
        p10 = fn_1_0(0x80);
        if (p10 != 0) {
            p10 = fn_1_CAD0(p10, i);
        }
    }
}

void fn_1_78DC(void *self)
{
    fn_1_5398((void *) ((u8 *) self + 0x20), 0);
    fn_1_1D6C();
    fn_1_D2C0();
}

void fn_1_7914(M579GameMaster *self)
{
    s32 i;
    s32 jingle;
    s32 bgm;
    u32 result;

    fn_1_1C48(0);
    fn_1_2BEC();
    bgm = HuAudBGMPlay(0x4d);
    HuAudFXPlay(0x95e);

    while (((fn_1_1CE0() >> 8) & 0xff) == 0) {
        fn_1_5428(&self->field_20, 0);
    }

    fn_1_1C48(1);
    fn_1_2704();

    while (((fn_1_2874() >> 8) & 0xff) == 0) {
        fn_1_5428(&self->field_20, 0);
    }

    fn_1_1C48(2);
    fn_1_5BC4(self->field_358, 0x708, 0, 0);
    fn_1_5CBC(self->field_358, 1);
    fn_1_5C3C(self->field_358, lbl_1_rodata_170, lbl_1_rodata_174);

    while (((fn_1_7AB4(self) >> 8) & 0xff) == 0) {
        fn_1_5428(&self->field_20, 0);
    }

    fn_1_5DB0(self->field_358);
    fn_1_D358();
    fn_1_D594();
    fn_1_1C48(3);
    HuAudSStreamFadeOut(bgm, 0x64);
    fn_1_27BC();

    for (i = 0; i < 0xb4; i++) {
        fn_1_5428(&self->field_20, 0);
    }

    fn_1_1C48(4);
    result = fn_1_DE88();
    if (((result >> 8) & 0xff) != 0) {
        jingle = 0x7b;
    } else {
        jingle = 0x79;
    }
    HuAudJinglePlay((s16) jingle);
    fn_1_2948();

    while (((fn_1_2874() >> 8) & 0xff) == 0) {
        fn_1_5428(&self->field_20, 0);
    }

    fn_1_2C44();

    while (1) {
        fn_1_5428(&self->field_20, -1);
    }
}

u32 fn_1_7AB4(M579GameMaster *self)
{
    return fn_1_5E18(self->field_358);
}

void fn_1_7AE0(void)
{
}

void fn_1_7AE4(M579GameMaster *self)
{
    fn_1_CA2C();
    fn_1_1DE0();
}

void fn_1_7B08(void)
{
    lbl_1_bss_A18 = 0x78;
    lbl_1_bss_A1C = 0xd2;
    lbl_1_bss_A20 = 0xd2;
}

void fn_1_7B3C(void *self)
{
    __asm {
        subi r3, r3, 0x20
        b fn_1_7914
    }
}

void fn_1_7B44(void *self)
{
    __asm {
        subi r3, r3, 0x20
        b fn_1_76BC
    }
}
}