#include "REL/m563dll/math.c"

#include "game/frand.h"
#include "game/hu3d.h"
#include "game/memory.h"

extern s32 lbl_1_bss_3C0[4][30];

s32 fn_1_767C(f32 *arg0, f32 *arg1, f32 *arg2, s32 arg3, u32 *arg4)
{
    f32 var_f31;
    s32 var_r31;
    s32 var_r28;
    u32 temp_r30;

    switch (arg3) {
    case 0:
        var_r31 = 0;
        var_f31 = 1.0f;
        break;
    case 1:
        var_r31 = 0;
        var_f31 = -1.0f;
        break;
    case 2:
        var_r31 = 1;
        var_f31 = 1.0f;
        break;
    case 3:
        var_r31 = 1;
        var_f31 = -1.0f;
        break;
    }
    temp_r30 = frandmod(0x1D);
    *arg0 = var_f31 * lbl_1_data_358[var_r31][temp_r30].x;
    if (lbl_1_data_358[var_r31][temp_r30].y == 75.0f) {
        *arg1 = 0.0f;
        var_r28 = 1;
    } else {
        *arg1 = lbl_1_data_358[var_r31][temp_r30].y - 75.0f;
        var_r28 = 0;
    }
    *arg2 = lbl_1_data_358[var_r31][temp_r30].z;
    *arg4 = temp_r30;
    return var_r28;
}

void fn_1_7818(f32 *arg0, f32 *arg1, f32 *arg2, s32 arg3)
{
    f32 var_f31;
    s32 var_r31;
    s32 var_r28;
    u32 temp_r30;

    switch (arg3) {
    case 0:
        var_r31 = 0;
        var_f31 = 1.0f;
        break;
    case 1:
        var_r31 = 0;
        var_f31 = -1.0f;
        break;
    case 2:
        var_r31 = 1;
        var_f31 = 1.0f;
        break;
    case 3:
        var_r31 = 1;
        var_f31 = -1.0f;
        break;
    }
    temp_r30 = frandmod(0x1D);
    *arg0 = var_f31 * lbl_1_data_358[var_r31][temp_r30].x;
    if (lbl_1_data_358[var_r31][temp_r30].y == 75.0f) {
        *arg1 = 0.0f;
        var_r28 = 1;
    } else {
        *arg1 = lbl_1_data_358[var_r31][temp_r30].y - 75.0f;
        var_r28 = 0;
    }
    *arg2 = lbl_1_data_358[var_r31][temp_r30].z;
}

s32 fn_1_79A8(s32 arg0, s32 arg1)
{
    return lbl_1_bss_3C0[arg0][arg1];
}

void fn_1_79C8(s32 arg0, s32 arg1, s32 arg2)
{
    lbl_1_bss_3C0[arg0][arg1] = arg2;
}

void fn_1_79E8(void)
{
    s32 j;
    s32 i;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 0x1E; j++) {
            lbl_1_bss_3C0[i][j] = 0;
        }
    }
}

void fn_1_7A4C(void)
{
    lbl_1_bss_7AC.prev = NULL;
    lbl_1_bss_7AC.next = NULL;
}

void fn_1_7A70(u16 arg0, s16 arg1)
{
    M563ListNode *head;
    M563ListNode *temp_r3;
    M563ListNode *temp_r29;

    temp_r3 = HuMemDirectMallocNum(HEAP_HEAP, 0xC, 0x10000000U);
    temp_r3->id = arg0;
    temp_r3->value = arg1;
    head = &lbl_1_bss_7AC;
    temp_r29 = head->next;
    head->next = temp_r3;
    if (NULL != temp_r29) {
        temp_r29->prev = temp_r3;
    }
    temp_r3->prev = head;
    temp_r3->next = temp_r29;
}

s16 fn_1_7AF8(s16 arg0)
{
    M563ListNode *var_r31;
    M563ListNode *temp_r29;
    M563ListNode *temp_r30;
    s16 temp_r28;

    var_r31 = lbl_1_bss_7AC.next;
    while (NULL != var_r31) {
        if (arg0 == var_r31->id) {
            temp_r28 = var_r31->value;
            temp_r29 = var_r31->prev;
            temp_r30 = var_r31->next;
            temp_r29->next = temp_r30;
            if (NULL != temp_r30) {
                temp_r30->prev = temp_r29;
            }
            HuMemDirectFree(var_r31);
            return temp_r28;
        }
        var_r31 = var_r31->next;
    }
    return -1;
}

void fn_1_7B9C(void)
{
    s32 i;

    for (i = 0; i < 0x20; i++) {
        lbl_1_bss_5A4[i].modelNum = 0U;
        lbl_1_bss_5A4[i].modelHandle = -1;
    }
}

s16 fn_1_7BF4(u32 arg0)
{
    s32 i;

    for (i = 0; i < 0x20; i++) {
        if (arg0 == lbl_1_bss_5A4[i].modelNum) {
            return Hu3DModelLink(lbl_1_bss_5A4[i].modelHandle);
        }
    }
    for (i = 0; i < 0x20; i++) {
        if (lbl_1_bss_5A4[i].modelNum == 0U) {
            lbl_1_bss_5A4[i].modelNum = arg0;
            lbl_1_bss_5A4[i].modelHandle =
                Hu3DModelCreate(HuDataSelHeapReadNum(arg0, 0x10000000, HEAP_MODEL));
            return lbl_1_bss_5A4[i].modelHandle;
        }
    }
    return Hu3DModelCreate(HuDataSelHeapReadNum(arg0, 0x10000000, HEAP_MODEL));
}
