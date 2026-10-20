#include "REL/mgtourDll.h"

HUWINID fn_1_660(void)
{
    return lbl_1_bss_16818->winId;
}

u32 fn_1_7E4(u32 mask)
{
    return lbl_1_bss_16818->stat & mask;
}

u32 fn_1_83C(u32 mask)
{
    return lbl_1_bss_1681C->flag & mask;
}

u32 fn_1_88C(u32 mask)
{
    return lbl_1_bss_16818->flag & mask;
}

int fn_1_96C(void)
{
    return lbl_1_bss_16818->turnNo;
}

int fn_1_984(void)
{
    return lbl_1_bss_16818->charNo;
}

void fn_1_A0(s8* arr1, s8* arr2, s8 len)
{
    s8 i;
    s8 j;
    s8 temp1;
    s8 temp2;

    i = 0;
    if (len <= 1) {
        return;
    }
    while (i < len - 1) {
        j = i + 1;
        while (j < len) {
            if (arr1[i] > arr1[j]) {
                temp1 = arr1[i];
                arr1[i] = arr1[j];
                arr1[j] = temp1;
                if (arr2 != NULL) {
                    temp2 = arr2[i];
                    arr2[i] = arr2[j];
                    arr2[j] = temp2;
                }
            }
            j++;
        }
        i++;
    }
}

void fn_1_140(void)
{
}

void fn_1_204(void)
{
}

void fn_1_40C(void)
{
}

void fn_1_448(void)
{
}

void fn_1_49C(void)
{
}

void fn_1_4F0(void)
{
}

void fn_1_5CC(void)
{
}

void fn_1_674(void)
{
}

void fn_1_708(void)
{
}

void fn_1_750(void)
{
}

void fn_1_7AC(void)
{
}

void fn_1_7C8(void)
{
}

void fn_1_7FC(void)
{
}

void fn_1_81C(void)
{
}

void fn_1_854(void)
{
}

void fn_1_870(void)
{
}

void fn_1_8A4(void)
{
}

void fn_1_904(void)
{
}

void fn_1_938(void)
{
}

void fn_1_99C(void)
{
}

void fn_1_9B4(void)
{
}

void fn_1_9D4(void)
{
}

void fn_1_A14(void)
{
}

void fn_1_A7C(void)
{
}

void fn_1_B98(void)
{
}

void fn_1_BEC(void)
{
}

void fn_1_C30(void)
{
}

void fn_1_D5C(void)
{
}

void fn_1_DA4(void)
{
}

void fn_1_DE4(void)
{
}

void fn_1_13D4(void)
{
}

void fn_1_1880(void)
{
}

void fn_1_18E0(void)
{
}

void fn_1_1938(void)
{
}

void fn_1_1C68(void)
{
}

void fn_1_1DE8(void)
{
}

void fn_1_1E1C(void)
{
}

void fn_1_1ED4(void)
{
}

void fn_1_2078(void)
{
}

void fn_1_242C(void)
{
}

void fn_1_25E4(void)
{
}

void fn_1_263C(void)
{
}

void fn_1_28C0(void)
{
}

void fn_1_2CE8(void)
{
}

void fn_1_2E68(void)
{
}

void fn_1_3B98(void)
{
}

void fn_1_3BF0(void)
{
}

void fn_1_3D2C(void)
{
}

void fn_1_3DE4(void)
{
}

void fn_1_44E0(void)
{
}

void fn_1_456C(void)
{
}

void fn_1_485C(void)
{
}

void fn_1_48B4(void)
{
}

void fn_1_5540(void)
{
}

void fn_1_56C0(void)
{
}

void fn_1_6538(void)
{
}

void fn_1_7450(void)
{
}

void fn_1_7CB8(void)
{
}

void fn_1_9094(void)
{
}

void fn_1_A5D8(void)
{
}

void fn_1_A5DC(void)
{
}

void fn_1_A644(void)
{
}

void fn_1_A6BC(void)
{
}

void fn_1_A910(void)
{
}

void fn_1_A94C(void)
{
}

void fn_1_B318(void)
{
}

void fn_1_B678(void)
{
}

void ObjectSetup(void)
{
}