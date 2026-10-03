#include "dolphin.h"

#include "datadir_enum.h"
#include "game/hu3d.h"

void *__dl__FPv(void *self);
void *__nw__FUl(u32 size);
void __register_global_object(void *obj, void *(*dtor)(void *, s16), void *reg);
void fn_1_1D88(void *self);
void fn_1_1D94(void *self, s32 arg1);
void fn_1_1E54(void *self, void **arg1);
void fn_1_372C(void *self, s32 arg1);
void fn_1_398C(void *self, void *arg1);
void fn_1_39D4(void *self, void *arg1);
void fn_1_3B8C(void *self, s32 arg1);
void fn_1_CF8(char *cond, char *file, s32 line);

extern "C" {

struct Mgr510Boat;

Mgr510Boat *fn_1_374(Mgr510Boat *arg0, s32 arg1, s32 arg2);
void fn_1_598(void);
void fn_1_66C(void);
void fn_1_69C(Mgr510Boat *arg0);
void fn_1_6C4(Mgr510Boat *arg0);
void fn_1_714(void **arg0, void (*arg1)(Mgr510Boat *));
void fn_1_774(void *arg0, void *arg1, void (*arg2)(Mgr510Boat *));
void *fn_1_82C(void *arg0, s16 arg1);

struct Mgr510Boat {
    Mgr510Boat *next;
    void *unk4;
    union {
        s16 modelId;
        void *vtable;
    } unk8;
    s32 unkC;
    Vec unk10;
    Vec unk1C;
    s32 unk28;
};

struct Mgr510BoatList {
    Mgr510Boat *next;
    Mgr510Boat *prev;
};

extern void *lbl_1_bss_2C[2];
extern void *lbl_1_data_2C[5];
extern void *lbl_1_data_40[6];

s32 lbl_1_bss_0[3];
Mgr510BoatList lbl_1_bss_C;
Mgr510Boat *lbl_1_bss_14;
s32 lbl_1_bss_18;
s32 lbl_1_bss_1C;

static const s32 lbl_1_rodata_10[4] = {
    DATANUM(DATA_m511, 0x0D),
    DATANUM(DATA_m511, 0x0E),
    DATANUM(DATA_m511, 0x0F),
    DATANUM(DATA_m511, 0x10),
};

// NONMATCHING: the singleton check and the allocation loop layout differ.
Mgr510Boat *fn_1_0(Mgr510Boat *arg0)
{
    Mgr510Boat *temp_r3;
    Mgr510Boat *var_r29;
    s32 var_r30;

    fn_1_1D88(arg0);
    arg0->unk8.vtable = lbl_1_data_40;
    arg0->unkC = 0;
    fn_1_1E54(arg0, (void **)lbl_1_bss_2C);
    if (lbl_1_bss_14 == NULL) {
        fn_1_CF8("ms_pointer == 0", "singleton.h", 0x35);
    }
    var_r29 = (Mgr510Boat *)((u8 *)arg0 + 0x10);
    if (var_r29 != NULL) {
        var_r29 -= 0x10;
    }
    lbl_1_bss_14 = var_r29;
    arg0->unk8.vtable = lbl_1_data_2C;
    for (var_r30 = 0; var_r30 < 4; var_r30++) {
        temp_r3 = (Mgr510Boat *)__nw__FUl(0x2C);
        if (temp_r3 != NULL) {
            fn_1_374(temp_r3, lbl_1_rodata_10[var_r30], var_r30);
        }
    }
    return arg0;
}

Mgr510Boat *fn_1_10C(Mgr510Boat *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        if (lbl_1_bss_14 == NULL) {
            fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_14 = (Mgr510Boat *)NULL;
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

Mgr510Boat *fn_1_198(Mgr510Boat *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        arg0->unk8.vtable = lbl_1_data_40;
        if (arg0 != NULL) {
            fn_1_1D94(arg0, 0);
        }
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

Mgr510Boat *fn_1_204(Mgr510Boat *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        fn_1_1D94(arg0, 0);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

Mgr510Boat *fn_1_25C(Mgr510Boat *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        arg0->unk8.vtable = lbl_1_data_2C;
        fn_1_598();
        if ((Mgr510Boat *)((u8 *)arg0 + 0x10) != NULL) {
            if (lbl_1_bss_14 == NULL) {
                fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
            }
            lbl_1_bss_14 = (Mgr510Boat *)NULL;
        }
        if (arg0 != NULL) {
            arg0->unk8.vtable = lbl_1_data_40;
            if (arg0 != NULL) {
                fn_1_1D94(arg0, 0);
            }
        }
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_32C(void)
{
    fn_1_66C();
}

// NONMATCHING: the unused range check is folded differently.
void fn_1_34C(Mgr510Boat *arg0, s32 arg1)
{
    arg0->unkC &= ~(1 << arg1);
    if (arg1 < 5) {
        if (arg1 >= 0) {
        }
    }
}

Mgr510Boat *fn_1_374(Mgr510Boat *arg0, s32 arg1, s32 arg2)
{
    fn_1_1D88(arg0);
    fn_1_372C(&arg0->unk8, arg1);
    arg0->unkC = arg2;
    arg0->unk28 = 0;
    fn_1_1E54(arg0, (void **)&lbl_1_bss_C);
    Hu3DModelScaleSet(arg0->unk8.modelId, 1.0f, 1.0f, 1.0f);
    Hu3DModelAttrReset(arg0->unk8.modelId, 1);
    Hu3DModelAttrSet(arg0->unk8.modelId, 0x4000);
    Hu3DModelLayerSet(arg0->unk8.modelId, 3);
    Hu3DModelCameraSet(arg0->unk8.modelId, 0xF);
    Hu3DMotionTimeSet(arg0->unk8.modelId, 0.0f);
    arg0->unk1C.x = 0.0f;
    arg0->unk1C.y = 0.0f;
    arg0->unk1C.z = 0.0f;
    fn_1_6C4(arg0);
    return arg0;
}

Mgr510Boat *fn_1_4AC(Mgr510Boat *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        fn_1_1D94(arg0, 0);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_504(s32 arg0, Vec *arg1, f32 farg0, f32 farg1)
{
    Mgr510Boat *var_r31;

    var_r31 = lbl_1_bss_C.next;
    while (var_r31 != (Mgr510Boat *)&lbl_1_bss_C) {
        if (var_r31->unkC == arg0) {
            var_r31->unk10.x = (f32)arg1->x;
            var_r31->unk10.y = (f32)arg1->y;
            var_r31->unk10.z = (f32)arg1->z;
            var_r31->unk1C.x = farg0;
            var_r31->unk1C.y = farg1;
            return;
        }
        var_r31 = var_r31->next;
    }
}

void fn_1_598(void)
{
    Mgr510Boat *temp_r31;

    while (!(lbl_1_bss_C.next == (Mgr510Boat *)&lbl_1_bss_C) || (lbl_1_bss_C.next != lbl_1_bss_C.prev)) {
        temp_r31 = lbl_1_bss_C.prev;
        if (temp_r31 != NULL) {
            fn_1_3B8C(&temp_r31->unk8, -1);
            if (temp_r31 != NULL) {
                fn_1_1D94(temp_r31, 0);
            }
            __dl__FPv(temp_r31);
        }
    }
}

void fn_1_66C(void)
{
    fn_1_714((void **)&lbl_1_bss_C.next, (void (*)(Mgr510Boat *))fn_1_69C);
}

void fn_1_69C(Mgr510Boat *arg0)
{
    fn_1_6C4(arg0);
}

void fn_1_6C4(Mgr510Boat *arg0)
{
    fn_1_39D4(&arg0->unk8, &arg0->unk1C);
    fn_1_398C(&arg0->unk8, &arg0->unk10);
    arg0->unk28 = arg0->unk28 + 1;
}

void fn_1_710(void)
{
}

void fn_1_714(void **arg0, void (*arg1)(Mgr510Boat *))
{
    fn_1_774(*arg0, arg0, arg1);
}

void fn_1_774(void *arg0, void *arg1, void (*arg2)(Mgr510Boat *))
{
    Mgr510Boat *var_r31;

    var_r31 = (Mgr510Boat *)arg0;
    while (var_r31 != (Mgr510Boat *)arg1) {
        arg2(var_r31);
        var_r31 = var_r31->next;
    }
}

void fn_1_7D4(void)
{
    Mgr510BoatList *var_r31 = &lbl_1_bss_C;

    fn_1_1D88(var_r31);
    __register_global_object(var_r31, fn_1_82C, lbl_1_bss_0);
}

void *fn_1_82C(void *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        fn_1_1D94(arg0, 0);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void *lbl_1_data_2C[5] = {
    NULL,
    NULL,
    (void *)fn_1_25C,
    (void *)fn_1_34C,
    (void *)fn_1_32C,
};

void *lbl_1_data_40[6] = {
    NULL,
    NULL,
    (void *)fn_1_198,
    NULL,
    (void *)fn_1_710,
    NULL,
};

}