#include <math.h>
#include <stdio.h>

#include "dolphin.h"

#include "datadir_enum.h"
#include "game/hu3d.h"

/* These are plain C in the original: m511.o defines them unmangled
   (m511.cpp is NonMatching, so the module links the original object).
   Left as C++ they mangle and the REL link fails with
   "Failed to find symbol fn_1_1D88__FPv in any module". */
extern "C" void *__dl__FPv(void *self);
extern "C" void *__nw__FUl(u32 size);
extern "C" void __register_global_object(void *obj, void *(*dtor)(void *, s16), void *reg);
void *fn_1_18404(void *self);
void fn_1_187B0(void *self, s32 arg1);
void fn_1_18804(void *arg0, Vec *arg1);
f32 fn_1_1A150(s32 arg0, void *arg1);
extern "C" void fn_1_1D88(void *self);
extern "C" void fn_1_1D94(void *self, s32 arg1);
extern "C" void fn_1_1E54(void *self, void **arg1);
extern "C" void fn_1_372C(void *self, s32 arg1);
void fn_1_38A0(void *self, s32 arg1);
void fn_1_38D8(void *self, s32 arg1);
extern "C" void fn_1_398C(void *self, void *arg1);
extern "C" void fn_1_39D4(void *self, void *arg1);
extern "C" void fn_1_3B8C(void *self, s32 arg1);
f32 fn_1_57B4(void);
void fn_1_B864(void *self, s32 arg1, s32 arg2);
extern "C" void fn_1_CF8(char *cond, char *file, s32 line);
void fn_1_DAB8(s32 arg0, f32 *arg1, f64 arg2);

struct Mgr510Kibako;

struct Mgr510KibakoVec {
    f32 unk0;
    f32 unk4;
    f32 unk8;
};

Mgr510Kibako *fn_1_18EFC(Mgr510Kibako *arg0, Mgr510KibakoVec *arg2, Vec *arg_sp0);
void fn_1_19364(void);
void fn_1_19450(void);
void fn_1_19480(Mgr510Kibako *arg0);
void fn_1_194D0(Mgr510Kibako *arg0);
void fn_1_1952C(Mgr510Kibako *arg0);
void fn_1_19648(Mgr510Kibako *arg0);
void fn_1_19694(void **arg0, void (*arg1)(Mgr510Kibako *));
void fn_1_196F4(void *arg0, void **arg1, void (*arg2)(Mgr510Kibako *));
void *fn_1_197AC(void *arg0, s16 arg1);

struct Mgr510Kibako {
    Mgr510Kibako *next;
    void *unk4;
    s32 unk8;
    s16 modelId;
    Vec unkC;
    Vec unk18;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    u32 unk30;
    void *unk34;
};

struct Mgr510KibakoMaster {
    u32 unk0;
    u32 unk4;
    void **vtable;
    s32 unkC;
    s16 modelId;
    u8 pad12[0x10];
};

struct Mgr510KibakoList {
    Mgr510Kibako *next;
    Mgr510Kibako *prev;
};

extern "C" {

extern void *lbl_1_bss_2C[2];
extern u32 lbl_1_bss_1C;

extern void *lbl_1_data_6B0[6];

Mgr510KibakoMaster *lbl_1_bss_A30;
s32 lbl_1_bss_B00[6];
Mgr510KibakoList lbl_1_bss_B0C;

static const s32 lbl_1_rodata_9E8[4] = {0x6B4, 0x6B5, 0x6B6, 0x6B7};

// NONMATCHING: the singleton check and the vtable pointer differ.
Mgr510KibakoMaster *fn_1_18A9C(Mgr510KibakoMaster *arg0)
{
    Mgr510KibakoMaster *var_r29;

    fn_1_1D88(arg0);
    arg0->vtable = &lbl_1_data_6B0[2];
    arg0->unkC = 0;
    fn_1_1E54(arg0, (void **)lbl_1_bss_2C);
    if (lbl_1_bss_A30 == NULL) {
        fn_1_CF8("ms_pointer == 0", "singleton.h", 0x35);
    }
    var_r29 = (Mgr510KibakoMaster *)((u8 *)arg0 + 0x10);
    if (var_r29 != NULL) {
        var_r29 -= 0x10;
    }
    lbl_1_bss_A30 = var_r29;
    arg0->vtable = lbl_1_data_6B0;
    fn_1_372C(&arg0->modelId, DATANUM(DATA_m511, 0x20));
    Hu3DModelAttrSet(arg0->modelId, 1);
    return arg0;
}

Mgr510KibakoMaster *fn_1_18B84(Mgr510KibakoMaster *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        if (lbl_1_bss_A30 == NULL) {
            fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A30 = (Mgr510KibakoMaster *)NULL;
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

// NONMATCHING: the vtable pointer differs.
Mgr510KibakoMaster *fn_1_18C10(Mgr510KibakoMaster *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        arg0->vtable = lbl_1_data_6B0;
        fn_1_19364();
        fn_1_3B8C(&arg0->modelId, -1);
        if ((Mgr510KibakoMaster *)((u8 *)arg0 + 0x10) != NULL) {
            if (lbl_1_bss_A30 == NULL) {
                fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
            }
            lbl_1_bss_A30 = (Mgr510KibakoMaster *)NULL;
        }
        if (arg0 != NULL) {
            arg0->vtable = &lbl_1_data_6B0[2];
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

// NONMATCHING: the box placement loop is folded differently.
void fn_1_18CEC(s32 arg0)
{
    f32 sp14;
    f32 sp10;
    f32 spC;
    s32 sp8;
    Mgr510Kibako *temp_r3;
    f32 temp_f31;
    f64 temp_f1;
    f64 temp_f1_2;
    s32 var_r30;
    s32 var_r31;

    sp8 = arg0;
    sp14 = 0.0f;
    for (var_r31 = 1; var_r31 < 4; var_r31++) {
        for (var_r30 = 0; var_r30 < var_r31; var_r30++) {
            temp_f1 = 3.141592653589793 * ((360.0f * fn_1_57B4()) - 180.0f);
            temp_f31 = (f32)(temp_f1 / 180.0);
            fn_1_DAB8(var_r31, &spC, temp_f1);
            spC = (f32)((f64)spC + (300.0 * ((f64)(var_r30 + 2) * cos((f64)temp_f31))));
            sp10 = 0.0f;
            temp_f1_2 = 300.0 * ((f64)(var_r30 + 2) * sin((f64)temp_f31));
            sp14 = (f32)((f64)sp14 + temp_f1_2);
            temp_r3 = (Mgr510Kibako *)__nw__FUl(0x38);
            if (temp_r3 != NULL) {
                fn_1_18EFC(temp_r3, (Mgr510KibakoVec *)(sp8 + 0x10), (Vec *)&spC);
            }
            printf("%d:% f\n", var_r30 + (var_r31 * 5), spC, sp14);
        }
    }
}

void fn_1_18EB4(void)
{
    fn_1_19450();
}

void fn_1_18ED4(Mgr510KibakoMaster *arg0, s32 arg1)
{
    arg0->unkC &= ~(1 << arg1);
    if (arg1 < 5) {
    }
}

// NONMATCHING: the attribute setup order differs.
Mgr510Kibako *fn_1_18EFC(Mgr510Kibako *arg0, Mgr510KibakoVec *arg2, Vec *arg_sp0)
{
    Mgr510Kibako *var_r29;
    s16 temp_r0;

    fn_1_1D88(arg0);
    arg0->unk8 = 1;
    fn_1_372C(&arg0->modelId, DATANUM(DATA_m511, 0x20));
    arg0->unkC.x = arg2->unk0;
    arg0->unkC.y = arg2->unk4;
    arg0->unkC.z = arg2->unk8;
    arg0->unk24 = 0;
    arg0->unk28 = 0;
    arg0->unk34 = NULL;
    fn_1_1E54(arg0, (void **)&lbl_1_bss_B0C);
    temp_r0 = arg0->modelId;
    Hu3DModelScaleSet(temp_r0, 1.0f, 1.0f, 1.0f);
    Hu3DModelAttrReset(temp_r0, 1);
    Hu3DModelLayerSet(temp_r0, 1);
    Hu3DModelCameraSet(temp_r0, 0xF);
    Hu3DModelAttrSet(temp_r0, 0x40000080);
    Hu3DModelAttrSet(temp_r0, 0x4000);
    Hu3DMotionTimeSet(temp_r0, 0.0f);
    if (lbl_1_bss_1C == 0) {
        fn_1_CF8("ms_pointer != 0", "singleton.h", 0x27);
    }
    arg0->unk30 = lbl_1_bss_1C;
    arg0->unk18.x = 0.0f;
    arg0->unk18.y = 0.0f;
    arg0->unk18.z = 0.0f;
    fn_1_19648(arg0);
    var_r29 = (Mgr510Kibako *)__nw__FUl(0xC);
    if (var_r29 != NULL) {
        var_r29 = (Mgr510Kibako *)fn_1_18404(var_r29);
    }
    if (arg0->unk34 != (void *)var_r29) {
        fn_1_187B0(arg0->unk34, 1);
        arg0->unk34 = (void *)var_r29;
    }
    return arg0;
}

Mgr510Kibako *fn_1_190E0(Mgr510Kibako *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        fn_1_187B0(arg0->unk34, 1);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

Mgr510Kibako *fn_1_19138(Mgr510Kibako *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        fn_1_1D94(arg0, 0);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

// NONMATCHING: the range test and the field types differ.
s32 fn_1_19190(Vec *arg0, s32 arg1)
{
    Mgr510Kibako *var_r31;
    f32 temp_f30;
    f32 temp_f31;
    f32 var_f28;
    f32 var_f29;
    s16 temp_r0;
    s32 var_r28;

    var_r28 = 0;
    var_r31 = lbl_1_bss_B0C.next;
    while (var_r31 != (Mgr510Kibako *)&lbl_1_bss_B0C) {
        if ((u8)var_r31->unk8 != 0) {
            temp_f31 = var_r31->unkC.x - arg0->x;
            if (temp_f31 < 0.0f) {
                var_f29 = -temp_f31;
            }
            else {
                var_f29 = temp_f31;
            }
            temp_f30 = var_r31->unkC.z - arg0->z;
            if (temp_f30 < 0.0f) {
                var_f28 = -temp_f30;
            }
            else {
                var_f28 = temp_f30;
            }
            if ((var_f29 < 200.0f) && (var_f28 < 200.0f)) {
                temp_r0 = var_r31->modelId;
                Hu3DModelAttrReset(temp_r0, 0x40000080);
                Hu3DModelAttrReset(temp_r0, 0x40000040);
                Hu3DMotionSet(temp_r0, Hu3DMotionIDGet(temp_r0));
                Hu3DMotionTimeSet(temp_r0, 0.0f);
                var_r31->unk8 = 0;
                var_r31->unk24 = 0;
                var_r28 = 1;
                var_r31->unk2C = arg1;
            }
        }
        var_r31 = var_r31->next;
    }
    return var_r28;
}

void fn_1_19364(void)
{
    Mgr510Kibako *temp_r31;
    u8 var_r30;

    while (!(lbl_1_bss_B0C.next == (Mgr510Kibako *)&lbl_1_bss_B0C) || (lbl_1_bss_B0C.next != lbl_1_bss_B0C.prev)) {
        temp_r31 = lbl_1_bss_B0C.prev;
        if (temp_r31 != NULL) {
            if (temp_r31->unk34 != NULL) {
                fn_1_187B0(temp_r31->unk34, 1);
            }
            fn_1_3B8C(&temp_r31->modelId, -1);
            if (temp_r31 != NULL) {
                fn_1_1D94(temp_r31, 0);
            }
            __dl__FPv(temp_r31);
        }
    }
}

void fn_1_19450(void)
{
    fn_1_19694((void **)&lbl_1_bss_B0C, (void (*)(Mgr510Kibako *))fn_1_19480);
}

void fn_1_19480(Mgr510Kibako *arg0)
{
    if ((u8)arg0->unk8 != 0) {
        fn_1_194D0(arg0);
    }
    else {
        fn_1_1952C(arg0);
    }
    fn_1_19648(arg0);
}

void fn_1_194D0(Mgr510Kibako *arg0)
{
    arg0->unkC.x = (f32)((f64)arg0->unkC.x + (0.02 * ((f64)fn_1_1A150(arg0->unk30, &arg0->unkC) - arg0->unkC.x)));
}

// NONMATCHING: the fade branch is folded differently.
void fn_1_1952C(Mgr510Kibako *arg0)
{
    if (arg0->unk24 < 60.0f) {
        if ((arg0->unk24 & 1) != 0) {
            fn_1_38D8(&arg0->modelId, 1);
            return;
        }
        if (((u8)arg0->unk28 == 0) && (arg0->unk24 > 42.0f)) {
            fn_1_18804(arg0->unk34, &arg0->unkC);
            arg0->unk28 = 1;
            fn_1_B864(&arg0->unkC, lbl_1_rodata_9E8[arg0->unk2C], arg0->unk2C);
        }
    }
    fn_1_38A0(&arg0->modelId, 1);
}

void fn_1_19648(Mgr510Kibako *arg0)
{
    fn_1_39D4(&arg0->modelId, &arg0->unk18);
    fn_1_398C(&arg0->modelId, &arg0->unkC);
    arg0->unk24 = arg0->unk24 + 1;
}

void fn_1_19694(void **arg0, void (*arg1)(Mgr510Kibako *))
{
    fn_1_196F4(*arg0, arg0, arg1);
}

void fn_1_196F4(void *arg0, void **arg1, void (*arg2)(Mgr510Kibako *))
{
    Mgr510Kibako *var_r31;

    var_r31 = (Mgr510Kibako *)arg0;
    while (var_r31 != (Mgr510Kibako *)arg1) {
        arg2(var_r31);
        var_r31 = var_r31->next;
    }
}

void fn_1_19754(void)
{
    Mgr510KibakoList *var_r31 = &lbl_1_bss_B0C;

    fn_1_1D88(var_r31);
    __register_global_object(var_r31, fn_1_197AC, lbl_1_bss_B00);
}

void *fn_1_197AC(void *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        fn_1_1D94(arg0, 0);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void *lbl_1_data_6B0[6] = {
    NULL,
    NULL,
    (void *)fn_1_18C10,
    (void *)fn_1_18ED4,
    (void *)fn_1_18EB4,
    NULL,
};

}
