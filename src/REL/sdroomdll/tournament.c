#include "types.h"

extern s16 Hu3DCameraNo;
extern void Hu3DFbCopyExec(s16 x, s16 y, s16 w, s16 h, s32 texFmt, s16 mipmapF, void *buf);
extern void Hu3DZClear(void);
extern void Hu3DModelRotSet(s16 modelId, float rotX, float rotY, float rotZ);
extern void HuPrcVSleep(void);
extern u32 frandmod(u32);
extern double cos(double);
extern void HuSprGrpKill(s16 grpId);
extern void HuWinKill(s16 winId);
extern void Hu3DModelKill(s16 modelId);
extern s32 HuPrcKill(void *process);
extern void HuSprAnimKill(void *anim);
extern void HuMemDirectFree(void *ptr);
extern void Hu3DLayerHookReset(s16 layerNo);
extern void HuSprScaleSet(s16 grpId, s16 memberNo, float scaleX, float scaleY);
extern void *lbl_1_bss_C0C;
extern void *lbl_1_bss_C08;
extern void *lbl_1_bss_C14;
extern void *lbl_1_bss_C18;
extern s16 lbl_1_bss_C00;
extern s16 lbl_1_bss_C02;
extern float lbl_1_bss_C04;
extern s16 lbl_1_bss_C20;
extern s16 lbl_1_bss_C22[6];
extern s16 lbl_1_bss_C2E[2];
extern s16 lbl_1_bss_C32;
extern s32 sdWorkInt[16];
extern s16 lbl_1_data_1D2C[3];
extern s16 lbl_1_data_1D32[2];
extern s16 lbl_1_data_1D36[4];
extern s16 lbl_1_data_1D48[3];
extern s16 lbl_1_data_1D4E[3];
extern s16 lbl_1_data_1D54[3];
extern s16 lbl_1_data_1D64[3];
extern s16 lbl_1_data_1D6A[2];
extern s16 lbl_1_data_1D6E[4];

void fn_1_1E090(void)
{
    if (Hu3DCameraNo != 0) {
        return;
    }

    Hu3DFbCopyExec(0, 0, 0x1E0, 0x168, 5, 0, lbl_1_bss_C0C);
    Hu3DZClear();
}

void fn_1_1E0EC(void)
{
    float angle = 0.0f;

    while (1) {
        Hu3DModelRotSet(lbl_1_bss_C20, 0.0f, angle, 0.0f);
        angle += 1.0;
        HuPrcVSleep();
    }
}

void fn_1_1F78C(s16 *array, s16 count)
{
    s16 i;

    if (count == 1) {
        return;
    }

    for (i = 0; i < 100; i++) {
        s16 idx1 = (s16)frandmod(count);
        s16 idx2 = (s16)frandmod(count);
        s16 temp = array[idx1];
        array[idx1] = array[idx2];
        array[idx2] = temp;
    }
}

void fn_1_1DFA8(void)
{
    s16 i;

    HuSprGrpKill(lbl_1_bss_C32);
    HuWinKill(lbl_1_bss_C2E[0]);
    HuWinKill(lbl_1_bss_C2E[1]);
    Hu3DModelKill(lbl_1_bss_C20);
    HuPrcKill(lbl_1_bss_C18);

    for (i = 0; i < 6; i++) {
        Hu3DModelKill(lbl_1_bss_C22[i]);
    }

    HuSprAnimKill(lbl_1_bss_C08);
    HuMemDirectFree(lbl_1_bss_C0C);
    HuPrcKill(lbl_1_bss_C14);
    Hu3DLayerHookReset(2);
    Hu3DLayerHookReset(10);
}

void fn_1_20760(void)
{
    float angle = 0.0f;

    while (1) {
        if (lbl_1_bss_C00 == 0) {
            lbl_1_bss_C04 = cos(angle * 3.141592653589793 / 180.0) * 0.2 + 0.8;
            HuSprScaleSet(lbl_1_bss_C32, lbl_1_bss_C02 + 0xF, lbl_1_bss_C04, lbl_1_bss_C04);
            angle += 10.0f;
            if (angle > 360.0f) {
                angle -= 360.0f;
            }
        } else {
            angle = 0.0f;
        }
        HuPrcVSleep();
    }
}

s16 fn_1_1F82C(s16 arg0)
{
    s16 *arr1;
    s16 *arr2;
    s16 *arr3;
    s16 i;

    if (arg0 == 0x2B) {
        return 0;
    }

    if (sdWorkInt[3] == 0) {
        arr1 = lbl_1_data_1D32;
        arr2 = lbl_1_data_1D36;
        arr3 = lbl_1_data_1D2C;
    } else if (sdWorkInt[3] == 1) {
        arr1 = lbl_1_data_1D4E;
        arr2 = lbl_1_data_1D54;
        arr3 = lbl_1_data_1D48;
    } else {
        arr1 = lbl_1_data_1D6A;
        arr2 = lbl_1_data_1D6E;
        arr3 = lbl_1_data_1D64;
    }

    for (i = 0; i < arr3[0]; i++) {
        if (arg0 == arr1[i]) {
            return 3;
        }
    }

    for (i = 0; i < arr3[1]; i++) {
        if (arg0 == arr2[i]) {
            return 2;
        }
    }

    return 1;
}