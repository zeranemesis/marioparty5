#include "REL/m563dll/math.c"

#include "game/frand.h"
#include "game/hu3d.h"
#include "game/memory.h"

/* util.o owns lbl_1_bss_3C0 in the original (0x1E0 bytes = 4*30 s32). */
s32 lbl_1_bss_3C0[4][30];

/* Same for the list head at 0x3EC: m563dll.h declares it extern, but util.o
 * defines it, and the functions below are the ones that write it. */
M563ListNode lbl_1_bss_7AC;

/* And the model slot array at 0x1E4: 0x104 bytes = 0x20 slots of 8 bytes,
   which is exactly M563ModelSlot (u32 + s16 + s16). */
M563ModelSlot lbl_1_bss_5A4[0x20];

/* The rest of util.o's .bss. m563dll.h declares all of these extern, but the
 * original defines them here, so declaring them extern again left the REL link
 * with "Failed to find symbol lbl_1_bss_79C in any module" and friends. Offsets
 * and sizes are the original's: 6A8=4, 6AC=0xC, 6B8=0x30, 6E8=0x30, 718=0x30,
 * 748=0x30, 778=0xC, 784=0xC, 790=0xC, 79C=0x10. */
s32 lbl_1_bss_6A8;
/* 6AC is a single s32 (math.c compares and assigns it directly); the 0xC the
 * original reserves is trailing padding, not two more cells. */
s32 lbl_1_bss_6AC;
u32 lbl_1_bss_6AC_pad[2];
Vec lbl_1_bss_6B8[4];
Vec lbl_1_bss_6E8[4];
Vec lbl_1_bss_718[4];
Vec lbl_1_bss_748[4];
Vec lbl_1_bss_778;
Vec lbl_1_bss_784;
Vec lbl_1_bss_790;
/* 79C is 0x10 bytes in the original: a Vec followed by one word of padding.
 * The pad is part of the same declaration so CodeWarrior cannot sort it to the
 * front of .bss and shift every offset after it. */
M563VecPad lbl_1_bss_79C;

/* Wave spawn table, recovered byte-for-byte from the original util.o .data:
   60 Vec3 rows of 12 bytes (0x2d0 total), indexed [half][pattern]. fn_1_767C
   and fn_1_7758 pick the row with frandmod(0x1D), so pattern is [0, 29], and
   var_r31 / half is [0, 1]. util.o owns this table (m563.o references it UND),
   so it has to be a real definition for the REL link to resolve. */
Vec lbl_1_data_358[2][30] = {
    {
        {-656.369995f, 275.000000f, -374.579987f},
        {-611.320007f, 275.000000f, -269.540009f},
        {-580.760010f, 275.000000f, -449.209991f},
        {-410.119995f, 275.000000f, -438.820007f},
        {-405.869995f, 275.000000f, -318.709991f},
        {-299.850006f, 275.000000f, -423.989990f},
        {-191.970001f, 275.000000f, -428.290009f},
        {-496.529999f, 275.000000f, -378.350006f},
        {-640.119995f, 275.000000f, -178.550003f},
        {-505.989990f, 275.000000f, -267.829987f},
        {-521.599976f, 275.000000f, -149.119995f},
        {-599.500000f, 275.000000f, -59.400002f},
        {-71.510002f, 275.000000f, -443.269989f},
        {-115.309998f, 225.000000f, -247.720001f},
        {-48.840000f, 225.000000f, -324.950012f},
        {-210.000000f, 225.000000f, -290.929993f},
        {-272.559998f, 175.000000f, -158.899994f},
        {-326.640015f, 175.000000f, -243.110001f},
        {-329.660004f, 125.000000f, -50.169998f},
        {-423.809998f, 125.000000f, -83.330002f},
        {-215.250000f, 75.000000f, 18.770000f},
        {-43.950001f, 75.000000f, -89.690002f},
        {-159.759995f, 75.000000f, -99.500000f},
        {0.000000f, 0.000000f, 0.000000f},
        {0.000000f, 0.000000f, 0.000000f},
        {0.000000f, 0.000000f, 0.000000f},
        {0.000000f, 0.000000f, 0.000000f},
        {0.000000f, 0.000000f, 0.000000f},
        {0.000000f, 0.000000f, 0.000000f},
        {0.000000f, 0.000000f, 0.000000f},
    },
    {
        {-645.030029f, 225.000000f, 124.540001f},
        {-552.229980f, 225.000000f, 67.480003f},
        {-553.210022f, 175.000000f, 333.570007f},
        {-645.570007f, 175.000000f, 267.779999f},
        {-37.070000f, 158.089996f, 289.809998f},
        {-153.199997f, 157.699997f, 310.959991f},
        {-262.040009f, 156.169998f, 271.299988f},
        {-611.530029f, 125.000000f, 479.410004f},
        {-510.200012f, 125.000000f, 467.179993f},
        {-634.239990f, 125.000000f, 591.549988f},
        {-632.270020f, 125.000000f, 705.619995f},
        {-492.899994f, 125.000000f, 687.640015f},
        {-495.519989f, 125.000000f, 577.200012f},
        {-367.339996f, 125.000000f, 675.429993f},
        {-375.269989f, 125.000000f, 540.359985f},
        {-407.869995f, 125.000000f, 428.109985f},
        {-439.369995f, 125.000000f, 295.019989f},
        {-330.239990f, 125.000000f, 339.339996f},
        {-90.000000f, 75.000000f, 688.369995f},
        {-206.750000f, 75.000000f, 701.419983f},
        {-100.239998f, 75.000000f, 576.489990f},
        {-217.410004f, 75.000000f, 592.599976f},
        {-206.529999f, 75.000000f, 479.420013f},
        {-431.309998f, 75.000000f, 99.330002f},
        {-315.660004f, 75.000000f, 91.260002f},
        {-148.880005f, 75.000000f, 105.099998f},
        {-45.200001f, 75.000000f, 51.369999f},
        {-84.730003f, 75.000000f, 456.829987f},
        {0.000000f, 0.000000f, 0.000000f},
        {0.000000f, 0.000000f, 0.000000f},
    },
};

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

