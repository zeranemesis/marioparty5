#include <stdio.h>

#include "dolphin.h"

#include "game/flag.h"
#include "game/gamework.h"
#include "game/mg/timer.h"

typedef u8 bool8;

struct CGameMaster;

/* These are plain C in the original: m511.o defines them unmangled
   (m511.cpp is NonMatching, so the module links the original object).
   Left as C++ they mangle and the REL link fails with
   "Failed to find symbol fn_1_1D88__FPv in any module". */
extern "C" void *__nw__FUl(u32 size);
void fn_1_0(void *self);
void fn_1_10828(void);
bool8 fn_1_10DB4(u8 arg0);
void fn_1_10FA4(void);
bool8 fn_1_10FF0(u8 arg0);
void fn_1_110E0(void);
void fn_1_11170(void);
u32 fn_1_111BC(void);
void *fn_1_159B4(void *self, u32 arg1, u8 arg2);
bool8 fn_1_15E48(void *arg0);
void fn_1_16040(void *self, u8 arg1);
void fn_1_18A9C(void *self);
void fn_1_18CEC(u32 arg0);
void fn_1_19848(void *self);
void fn_1_1B578(void *self);
void fn_1_276C(s32 arg0);
bool8 fn_1_2804(void);
void fn_1_2890(void);
void fn_1_2900(void);
void fn_1_29D8(void *self);
void fn_1_2B64(void *self, s32 arg1);
void fn_1_319C(void);
void fn_1_3254(void);
bool8 fn_1_330C(void);
void fn_1_33E0(void);
void fn_1_35AC(s32 arg0);
void fn_1_3684(void);
void fn_1_36DC(void);
void fn_1_527C(void *self);
void fn_1_54A0(void);
void fn_1_5D00(void *self, s32 arg1);
void fn_1_5D7C(void *self, s32 arg1);
void fn_1_5E30(void *self, s32 arg1);
void fn_1_5EC0(void *self, s32 arg1);
void fn_1_6538(void *self, s32 arg1);
void fn_1_6600(void *self, s32 arg1);
void fn_1_665C(void *self, s32 arg1, s32 arg2, s32 arg3);
void fn_1_66D4(void *self, f32 *arg1, f32 arg2, f32 arg3);
void fn_1_6754(void *self, s32 arg1);
void fn_1_6848(void *self);
bool8 fn_1_68B0(void *self);
void fn_1_97C8(void *self);
void fn_1_BD18(void *self);
void fn_1_C67C(void *self);
extern "C" void fn_1_CF8(char *cond, char *file, s32 line);
void fn_1_D318(void *self);
void fn_1_E8BC(void *self);
void fn_1_F010(void);
void fn_1_F074(void);
void fn_1_FB50(void);
void fn_1_FC40(void);
void fn_1_FCE4(void *self, s32 arg1);

s32 HuAudFXPlay(s32 id);
s32 HuAudBGMPlay(s32 id);
void HuAudFXFadeOut(s32 id, s32 speed);
void HuAudSStreamFadeOut(s32 id, s32 speed);
void HuAudJinglePlay(s16 id);
void GWMgRecordSet(s32 id, u32 value);

struct Mgr510SubA {
    void *unk0;
    u8 pad4[0x334];
};

struct Mgr510SubB {
    u32 unk0;
};

struct Mgr510Rec {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    MGTIMER *unk10;
};

struct CGameMaster {
    u32 unk0;
    void **vtable;
    u8 pad8[0x18];
    Mgr510SubA subA;
    Mgr510SubB subB;
    Mgr510Rec *unk35C;
    s32 unk360;
    u32 unk364;
    u8 unk368;
    u8 pad369[7];
};

extern "C" {

extern void *lbl_1_data_304[13];

CGameMaster *__ct__Q24m51111CGameMasterFv(CGameMaster *arg0);
CGameMaster *__dt__Q24m51111CGameMasterFv(CGameMaster *arg0, s16 arg1);
void on_setup__Q24m51111CGameMasterFP9Process_s(CGameMaster *arg0, void *arg_sp0);
void fn_1_7FB0(CGameMaster *arg0);
void fn_1_7FE8(CGameMaster *arg0);
void fn_1_822C(CGameMaster *arg0);
bool8 fn_1_8304(CGameMaster *arg0);
bool8 fn_1_83C0(CGameMaster *arg0);
void fn_1_841C(void);
void fn_1_8420(void);
void fn_1_8444(void);
void fn_1_8478(CGameMaster *arg0);
void fn_1_8480(CGameMaster *arg0);

static const f32 lbl_1_rodata_1E0[2] = {292.0f, 416.0f};

s32 lbl_1_bss_A18;
s32 lbl_1_bss_A1C;
s32 lbl_1_bss_A20;
s32 lbl_1_bss_A24;
s32 lbl_1_bss_A28;
s32 lbl_1_bss_A2C;
/* lbl_1_bss_A30 is defined by m511.o, which references it as UND here. Defining it too makes
   the linker report it multiply-defined. */
extern "C" u32 lbl_1_bss_A30;
s32 lbl_1_bss_A34;

CGameMaster *__ct__Q24m51111CGameMasterFv(CGameMaster *arg0)
{
    fn_1_29D8(arg0);
    fn_1_5D00(&arg0->subA, 0x8000);
    arg0->vtable = lbl_1_data_304;
    arg0->subA.unk0 = (void *)&lbl_1_data_304[6];
    fn_1_6538(&arg0->subB, 0);
    arg0->unk364 = GWMgRecordGet(2);
    arg0->unk368 = (_CheckFlag(0x30002) != 0);
    return arg0;
}

CGameMaster *__dt__Q24m51111CGameMasterFv(CGameMaster *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        arg0->vtable = lbl_1_data_304;
        arg0->subA.unk0 = (void *)&lbl_1_data_304[6];
        printf("%s\n", "m511::CGameMaster::~CGameMaster()");
        fn_1_6600(&arg0->subB, -1);
        fn_1_5D7C(&arg0->subA, 0);
        fn_1_2B64(arg0, 0);
        if (arg1 > 0) {
            fn_1_527C(arg0);
        }
    }
    return arg0;
}

// NONMATCHING: the constructor allocations and their null checks reorder slightly.
void on_setup__Q24m51111CGameMasterFP9Process_s(CGameMaster *arg0, void *arg_sp0)
{
    s32 var_r30;
    void *temp_r3;
    void *temp_r3_10;
    void *temp_r3_11;
    void *temp_r3_2;
    void *temp_r3_3;
    void *temp_r3_4;
    void *temp_r3_5;
    void *temp_r3_6;
    void *temp_r3_7;
    void *temp_r3_8;
    void *temp_r3_9;
    void *var_r23;

    OSGetTick();
    fn_1_54A0();
    printf("%s\n", "m511::CGameMaster::on_setup(Process_s *)");
    temp_r3 = __nw__FUl(0x24);
    if (temp_r3 != 0) {
        fn_1_BD18(temp_r3);
    }
    temp_r3_2 = __nw__FUl(0x3EC);
    if (temp_r3_2 != 0) {
        fn_1_1B578(temp_r3_2);
    }
    temp_r3_3 = __nw__FUl(0x2434);
    if (temp_r3_3 != 0) {
        fn_1_19848(temp_r3_3);
    }
    temp_r3_4 = __nw__FUl(0x20);
    if (temp_r3_4 != 0) {
        fn_1_C67C(temp_r3_4);
    }
    temp_r3_5 = __nw__FUl(0x168);
    if (temp_r3_5 != 0) {
        fn_1_97C8(temp_r3_5);
    }
    temp_r3_6 = __nw__FUl(0x104);
    if (temp_r3_6 != 0) {
        fn_1_16040(temp_r3_6, arg0->unk368);
    }
    var_r23 = __nw__FUl(0x1C);
    if (var_r23 != 0) {
        var_r23 = fn_1_159B4(var_r23, arg0->unk364, arg0->unk368);
    }
    arg0->unk35C = (Mgr510Rec *)var_r23;
    temp_r3_7 = __nw__FUl(0x10);
    if (temp_r3_7 != 0) {
        fn_1_E8BC(temp_r3_7);
    }
    temp_r3_8 = __nw__FUl(0x10);
    if (temp_r3_8 != 0) {
        fn_1_0(temp_r3_8);
    }
    fn_1_FB50();
    var_r30 = 0;
    loop_22:
    if (var_r30 < 4) {
        temp_r3_9 = __nw__FUl(0x114);
        if (temp_r3_9 != 0) {
            fn_1_FCE4(temp_r3_9, var_r30);
        }
        var_r30 += 1;
        goto loop_22;
    }
    temp_r3_10 = __nw__FUl(0xA0);
    if (temp_r3_10 != 0) {
        fn_1_D318(temp_r3_10);
    }
    temp_r3_11 = __nw__FUl(0x14);
    if (temp_r3_11 != 0) {
        fn_1_18A9C(temp_r3_11);
    }
}

void fn_1_7FB0(CGameMaster *arg0)
{
    fn_1_5E30(&arg0->subA, 0);
    fn_1_2890();
    fn_1_10828();
}

// NONMATCHING: several of the wait loops and the jingle pick differ.
void fn_1_7FE8(CGameMaster *arg0)
{
    s16 var_r28;
    s32 temp_r26;
    s32 temp_r27;
    bool8 temp_r29;
    bool8 var_r30;

    fn_1_276C(0);
    if (lbl_1_bss_A30 == 0) {
        fn_1_CF8("ms_pointer != 0", "singleton.h", 0x27);
    }
    fn_1_18CEC(lbl_1_bss_A30);
    fn_1_3684();
    temp_r27 = HuAudFXPlay(0x6B8);
    HuAudFXPlay(0x6B9);
    while (fn_1_2804() == 0) {
        fn_1_5EC0(&arg0->subA, 0);
    }
    temp_r26 = HuAudBGMPlay(0x35);
    fn_1_276C(1);
    fn_1_319C();
    while (fn_1_330C() == 0) {
        fn_1_5EC0(&arg0->subA, 0);
    }
    var_r30 = 0;
    fn_1_276C(2);
    fn_1_F010();
    arg0->unk360 = 0;
    fn_1_665C(&arg0->subB, 0x708, 0, 0);
    fn_1_66D4(&arg0->subB, (f32 *)lbl_1_rodata_1E0, 292.0f, 416.0f);
    loop_14:
    if (fn_1_8304(arg0) == 0) {
        if ((var_r30 == 0) && (arg0->unk360 > 0x3F48)) {
            fn_1_6754(&arg0->subB, 1);
            var_r30 = 1;
        }
        fn_1_5EC0(&arg0->subA, 0);
        goto loop_14;
    }
    temp_r29 = fn_1_10FF0(arg0->unk368);
    if (var_r30 != 0) {
        fn_1_6848(&arg0->subB);
    }
    fn_1_276C(3);
    fn_1_F074();
    HuAudSStreamFadeOut(temp_r26, 0x64);
    fn_1_822C(arg0);
    fn_1_276C(4);
    if (arg0->unk368 == 0) {
        if (temp_r29 != 0) {
            var_r28 = 0x7B;
        }
        else {
            var_r28 = 0x79;
        }
        HuAudJinglePlay(var_r28);
        if (temp_r29 != 0) {
            fn_1_110E0();
        }
        else {
            fn_1_11170();
        }
        fn_1_33E0();
        fn_1_10FA4();
        while (fn_1_330C() == 0) {
            fn_1_5EC0(&arg0->subA, 0);
        }
    }
    HuAudFXFadeOut(temp_r27, 0x64);
    fn_1_36DC();
    while (1) {
        fn_1_5EC0(&arg0->subA, -1);
    }
}

// NONMATCHING: the record bookkeeping tail differs slightly.
void fn_1_822C(CGameMaster *arg0)
{
    fn_1_3254();
    while (fn_1_330C() == 0) {
        fn_1_5EC0(&arg0->subA, 0);
    }
    if ((fn_1_83C0(arg0) != 0) && (arg0->unk368 == 0)) {
        MgTimerRecordSet(arg0->unk35C->unk10, arg0->unk364);
        if (_CheckFlag(0x1000F) == 0) {
            GWMgRecordSet(2, arg0->unk364);
            fn_1_35AC(arg0->unk364);
            while (fn_1_330C() == 0) {
                fn_1_5EC0(&arg0->subA, 0);
            }
        }
    }
}

bool8 fn_1_8304(CGameMaster *arg0)
{
    bool8 var_r29;
    bool8 var_r30;

    arg0->unk360 = arg0->unk360 + 1;
    if (arg0->unk368 != 0) {
        var_r30 = 1;
        if ((fn_1_15E48(arg0->unk35C) == 0) && (fn_1_10DB4(arg0->unk368) == 0)) {
            var_r30 = 0;
        }
        return var_r30;
    }
    var_r29 = 1;
    if ((fn_1_68B0(&arg0->subB) == 0) && (fn_1_10DB4(arg0->unk368) == 0)) {
        var_r29 = 0;
    }
    return var_r29;
}

bool8 fn_1_83C0(CGameMaster *arg0)
{
    u32 temp_r3;
    bool8 var_r30;

    var_r30 = 0;
    temp_r3 = fn_1_111BC();
    if (arg0->unk364 > temp_r3) {
        arg0->unk364 = temp_r3;
        var_r30 = 1;
    }
    return var_r30;
}

void fn_1_841C(void)
{
}

void fn_1_8420(void)
{
    fn_1_FC40();
    fn_1_2900();
}

void fn_1_8444(void)
{
    lbl_1_bss_A18 = 0x78;
    lbl_1_bss_A1C = 0xD2;
    lbl_1_bss_A20 = 0xD2;
}

void fn_1_8478(CGameMaster *arg0)
{
    fn_1_7FE8((CGameMaster *)((s32)arg0 - 0x20));
}

void fn_1_8480(CGameMaster *arg0)
{
    __dt__Q24m51111CGameMasterFv((CGameMaster *)((s32)arg0 - 0x20), 0);
}

void *lbl_1_data_304[13] = {
    NULL,
    NULL,
    (void *)__dt__Q24m51111CGameMasterFv,
    (void *)on_setup__Q24m51111CGameMasterFP9Process_s,
    (void *)fn_1_7FB0,
    (void *)fn_1_8420,
    NULL,
    NULL,
    (void *)fn_1_8480,
    (void *)fn_1_8478,
    (void *)fn_1_841C,
    (void *)fn_1_7FE8,
    NULL,
};

}
