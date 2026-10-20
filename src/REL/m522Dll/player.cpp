#include "REL/m522Dll.h"

extern "C" {
void fn_1_13CE8(void *);
void fn_1_148B4(void *);
void fn_1_119E8(void *);
void fn_1_13A10(void *);
void fn_1_14DF4(void *);
void fn_1_13DCC(void *);

void Hu3D3Dto2D(void *, s32, f32 *);
extern f32 lbl_1_rodata_4C8;
extern f32 lbl_1_rodata_4CC;

void fn_1_F604(void);
extern s16 GwPlayerConf[];
void *fn_1_689C(s32);
void fn_1_12530(void *, s32, void *);
void CharEffectLayerSet(s32);
extern f32 lbl_1_rodata_4D0;

void fn_1_F6E4(void);
extern void *lbl_1_bss_A64;
void fn_1_14FC0(void *, void (*)(void *));
void MgActorExec(void);
void fn_1_807C(void *);
void fn_1_80D0(void *, void *);

void fn_1_F76C(void);
extern void *lbl_1_data_5A0;
void _savegpr_27(void);
void _restgpr_27(void);
void fn_1_F800(void);
extern void *lbl_1_data_5AC;

void fn_1_F894(void);
void fn_1_8004(void *);

void fn_1_F904(void);

void fn_1_F974(void);
void fn_1_FEA8(void *, s16);

s32 fn_1_FA18(void);
extern s32 lbl_1_bss_A54;

s32 fn_1_FA28(void);
s32 fn_1_12984(void);

void *fn_1_FA84(void *);
void *fn_1_FAF8(void *);

void *fn_1_FB6C(void *, s32);
void fn_1_8004(void *);
extern void *lbl_1_data_760;
extern void *lbl_1_data_5B8;
extern void *lbl_1_bss_A64;
void fn_1_80D0(void *, void *);

void fn_1_FCAC(void *, s16);
void fn_1_6964(void *);
void fn_1_8010(void *, s32);

void fn_1_FD40(void *, s16);

void fn_1_FDCC(void *, s16);

void fn_1_FE50(void *, s16);

void fn_1_FEA8(void *, s16);

void fn_1_FF70(void *);

void fn_1_FFF8(void *, s32);
void CharMotionCreate(s16, s32);
void fn_1_15080(void *, void *, s32);
void fn_1_9AB8(void *, s32, s32);
void fn_1_9A34(void *, s32);
void HuDataSelHeapReadNum(s32, s32, s32);
void Hu3DJointMotion(s16, s32);
void Hu3DModelHookSet(s16, s32, s32);
void fn_1_9DEC(void *, s32);
void fn_1_9BA8(void *, s32);
void fn_1_F1B8(void *);
void fn_1_F194(void *);

void fn_1_10070(void *, s32, u8);
void CharMotionEndCheck(s16);
void CharModelAttrSet(s16, s32);
void fn_1_14F48(void *, s32);
void CharMotionShiftSet(s16, s16, f32, f32, s32);

void fn_1_10160(void *);
void CharMotionShiftIDGet(s16);
void CharMotionShiftTimeGet(s16);
extern f32 lbl_1_rodata_4FC;
void CharMotionTimeGet(s16);

void fn_1_101CC(void *, s32);
void CharModelAttrReset(s16, s32);
void CharModelFxFlagSet(s16, s32);

void fn_1_10240(void *, s32);
void omVibrate(s16, s32, s32, s32);

void fn_1_10324(void *, s32);
void fn_1_9AB8(void *, s32, s32);
void fn_1_9A34(void *, s32);
extern void *lbl_1_data_740;
extern f32 lbl_1_rodata_4F4;
extern f32 lbl_1_rodata_4F8;
extern f32 lbl_1_rodata_500;
extern f32 lbl_1_rodata_504;
void fn_1_9DEC(void *, s32);
extern void *lbl_1_bss_18;
extern void *lbl_1_data_5EB;
extern void *lbl_1_data_5FB;
void fn_1_6A70(void *, void *, s32);
void fn_1_12368(void *);

void fn_1_107E0(void *, s16);
void fn_1_9E94(void *, s32);
void fn_1_8010(void *, s32);
void fn_1_6964(void *);

s32 fn_1_108F8(void *);
extern f32 lbl_1_rodata_510;

void fn_1_109A4(void *);

void fn_1_10A00(void *);
extern f32 lbl_1_rodata_518;
extern f32 lbl_1_rodata_520;
void fn_1_12028(void *);
void fn_1_F194(void *);
void fn_1_6A70(void *, void *, s32);

void fn_1_10DF8(void *);
extern f32 lbl_1_rodata_540;
extern f32 lbl_1_rodata_548;
extern f32 lbl_1_rodata_528;
extern void *lbl_1_data_614;
extern void *lbl_1_data_620;
void HuAudFXPlayPan(s32, s16);

void fn_1_1103C(void *);
extern f32 lbl_1_rodata_54C;
extern f32 lbl_1_rodata_550;
void CharMotionMaxTimeGet(s16);
void fn_1_F1B8(void *);
void fn_1_EF38(void *, void *);
void CharEffectHitCreate(s32, void *, void *);
extern void *lbl_1_data_62C;
}

void fn_1_12A1C(void *arg0)
{
    fn_1_13CE8(arg0);
}

s16 fn_1_F564(void *arg0)
{
    f32 temp;
    s32 result;

    Hu3D3Dto2D(arg0, 1, &temp);
    temp = temp * lbl_1_rodata_4C8;
    temp = temp + lbl_1_rodata_4CC;
    result = (s32)temp;
    if (result < 0) {
        result = 0;
    } else if (result > 0x7F) {
        result = 0x7F;
    }
    return (s16)result;
}

void fn_1_F604(void)
{
    s32 i;
    void *ptr;
    void *rodata_ptr;

    lbl_1_bss_A54 = 0;
    rodata_ptr = (void *)&lbl_1_rodata_4D0;

    for (i = 0; i < 4; i++) {
        s16 *conf = &GwPlayerConf[i * 0xA];
        if (conf[3] == 0) {
            ptr = fn_1_689C(0x90);
            if (ptr != NULL) {
                fn_1_10324(ptr, i);
            }
        } else {
            ptr = fn_1_689C(0xB4);
            if (ptr != NULL) {
                fn_1_12530(ptr, i, rodata_ptr);
                rodata_ptr = (u8 *)rodata_ptr + 0xC;
            }
        }
    }

    CharEffectLayerSet(5);
}

void fn_1_F6E4(void)
{
    void *list_head;
    void *current;
    void *next;

    fn_1_14FC0(&lbl_1_bss_A64, fn_1_FF70);
    MgActorExec();

    list_head = lbl_1_bss_A64;
    current = *(void **)list_head;

    while (current != list_head) {
        next = *(void **)current;
        fn_1_807C(current);
        fn_1_80D0(current, &lbl_1_bss_A64);
        current = next;
    }
}

void fn_1_F76C(void)
{
    void *list_head;
    void *current;
    void *next;
    s32 *data_ptr;

    _savegpr_27();

    list_head = lbl_1_bss_A64;
    current = *(void **)list_head;
    data_ptr = (s32 *)&lbl_1_data_5A0;

    while (current != list_head) {
        next = *(void **)current;
        *(s32 *)((u8 *)current + 0x8) = data_ptr[0];
        *(s32 *)((u8 *)current + 0xC) = data_ptr[1];
        *(s32 *)((u8 *)current + 0x10) = data_ptr[2];
        data_ptr += 3;
        current = next;
    }

    _restgpr_27();
}

void fn_1_F800(void)
{
    void *list_head;
    void *current;
    void *next;
    s32 *data_ptr;

    _savegpr_27();

    list_head = lbl_1_bss_A64;
    current = *(void **)list_head;
    data_ptr = (s32 *)&lbl_1_data_5AC;

    while (current != list_head) {
        next = *(void **)current;
        *(s32 *)((u8 *)current + 0x8) = data_ptr[0];
        *(s32 *)((u8 *)current + 0xC) = data_ptr[1];
        *(s32 *)((u8 *)current + 0x10) = data_ptr[2];
        data_ptr += 3;
        current = next;
    }

    _restgpr_27();
}

void fn_1_F894(void)
{
    void *list_head;
    void *current;
    void *next;
    void (*vfunc)(void *);

    _savegpr_27();

    list_head = lbl_1_bss_A64;
    current = *(void **)list_head;

    while (current != list_head) {
        next = *(void **)current;
        vfunc = *(void (**)(void *))(*(u32 *)((u8 *)current + 0x14) + 0x18);
        vfunc(current);
        current = next;
    }

    _restgpr_27();
}

void fn_1_F904(void)
{
    void *list_head;
    void *current;
    void *next;
    void (*vfunc)(void *);

    _savegpr_27();

    list_head = lbl_1_bss_A64;
    current = *(void **)list_head;

    while (current != list_head) {
        next = *(void **)current;
        vfunc = *(void (**)(void *))(*(u32 *)((u8 *)current + 0x14) + 0x14);
        vfunc(current);
        current = next;
    }

    _restgpr_27();
}

void fn_1_F974(void)
{
    void *actor;
    void *ptr;
    s32 val;

    actor = lbl_1_bss_A64;
    if (actor == NULL) {
        return;
    }

    ptr = *(void **)((u8 *)actor + 0x4);
    if (ptr == NULL) {
        return;
    }

    while (1) {
        fn_1_FEA8(ptr, 1);

        val = *(s32 *)lbl_1_bss_A64;
        if (val != 0) {
            break;
        }

        val = *(s32 *)lbl_1_bss_A64;
        if (val != *(s32 *)((u8 *)lbl_1_bss_A64 + 0x4)) {
            break;
        }

        val = *(s32 *)lbl_1_bss_A64;
        if (val != *(s32 *)((u8 *)lbl_1_bss_A64 + 0x4)) {
            break;
        }
    }
}

s32 fn_1_FA18(void)
{
    return *(s32 *)lbl_1_bss_A54;
}

s32 fn_1_FA28(void)
{
    s32 val = *(s32 *)lbl_1_bss_A54;
    if (val <= 0) {
        return 0;
    }

    s32 result = fn_1_12984();
    val = *(s32 *)lbl_1_bss_A54;
    if (result == val) {
        return 1;
    }
    return -1;
}

void *fn_1_FA84(void *arg)
{
    void *list_head = lbl_1_bss_A64;
    void *current = *(void **)list_head;
    void *end = list_head;

    while (current != end) {
        if (*(void **)((u8 *)current + 0x18) == arg) {
            return current;
        }
        current = *(void **)current;
    }
    return NULL;
}

void *fn_1_FAF8(void *arg)
{
    void *list_head = lbl_1_bss_A64;
    void *current = *(void **)list_head;
    void *end = list_head;

    while (current != end) {
        if (*(void **)((u8 *)current + 0x20) == NULL) {
            return current;
        }
        current = *(void **)current;
    }
    return NULL;
}

// Stub implementations for remaining functions
void *fn_1_FB6C(void *arg0, s32 arg1) { return NULL; }
void fn_1_FCAC(void *arg0, s16 arg1) {}
void fn_1_FD40(void *arg0, s16 arg1) {}
void fn_1_FDCC(void *arg0, s16 arg1) {}
void fn_1_FE50(void *arg0, s16 arg1) {}
void fn_1_FEA8(void *arg0, s16 arg1) {}
void fn_1_FF70(void *arg0) {}
void fn_1_FFF8(void *arg0, s32 arg1) {}
void fn_1_10070(void *arg0, s32 arg1, u8 arg2) {}
void fn_1_10160(void *arg0) {}
void fn_1_101CC(void *arg0, s32 arg1) {}
void fn_1_10240(void *arg0, s32 arg1) {}
void fn_1_10324(void *arg0, s32 arg1, void *arg2) {}
void fn_1_107E0(void *arg0, s16 arg1) {}
s32 fn_1_108F8(void *arg0) { return 0; }
void fn_1_109A4(void *arg0) {}
void fn_1_10A00(void *arg0) {}
void fn_1_10DF8(void *arg0) {}
void fn_1_1103C(void *arg0) {}
void fn_1_113BC(void *arg0) {}
void fn_1_114CC(void *arg0) {}
void fn_1_11694(void *arg0) {}
void fn_1_117EC(void *arg0) {}
void fn_1_11998(void *arg0) {}
void fn_1_119E8(void *arg0) {}
void fn_1_11A20(void *arg0) {}
void fn_1_11D60(void *arg0) {}
void fn_1_11E34(void *arg0) {}
void fn_1_12028(void *arg0) {}
void fn_1_12084(void *arg0) {}
void fn_1_120DC(void *arg0) {}
void fn_1_12368(void *arg0) {}
void fn_1_12530(void *arg0, s32 arg1, void *arg2) {}
void fn_1_1289C(void *arg0) {}
s32 fn_1_12984(void) { return 0; }
void fn_1_12A44(void *arg0) {}
void fn_1_12AE4(void *arg0) {}
void fn_1_12B68(void *arg0) {}
void fn_1_12EB0(void *arg0) {}
void fn_1_130D0(void *arg0) {}
void fn_1_133F8(void *arg0) {}
void fn_1_133FC(void *arg0) {}
void fn_1_13550(void *arg0) {}
void fn_1_13698(void *arg0) {}
void fn_1_1370C(void *arg0) {}
void fn_1_137D0(void *arg0) {}
void fn_1_1384C(void *arg0) {}
void fn_1_13A10(void *arg0) {}
void fn_1_13A48(void *arg0) {}
void fn_1_13BBC(void *arg0) {}
void fn_1_13CE8(void *arg0) {}
void fn_1_13DCC(void *arg0) {}
void fn_1_13E18(void *arg0) {}
void fn_1_144D4(void *arg0) {}
void fn_1_14708(void *arg0) {}
void fn_1_14868(void *arg0) {}
void fn_1_148B4(void *arg0) {}
void fn_1_148DC(void *arg0) {}
void fn_1_14B20(void *arg0) {}
void fn_1_14CFC(void *arg0) {}
void fn_1_14D00(void *arg0) {}
void fn_1_14D04(void *arg0) {}
void fn_1_14DF4(void *arg0) {}
void fn_1_14E30(void *arg0) {}
void fn_1_14EA4(void *arg0) {}
void fn_1_14F40(void *arg0) {}
void fn_1_14F48(void *arg0, s32 arg1) {}
void fn_1_14FC0(void *arg0, void (*arg1)(void *)) {}
void fn_1_15020(void *arg0) {}
void fn_1_15080(void *arg0, void *arg1, s32 arg2) {}
void fn_1_15430(void *arg0) {}
void fn_1_15648(void *arg0) {}
void fn_1_156D0(void *arg0) {}