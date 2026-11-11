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

void fn_1_7AC(u32 mask)
{
    lbl_1_bss_16818->stat |= mask;
}

void fn_1_7C8(u32 mask)
{
    lbl_1_bss_16818->stat &= ~mask;
}

// NONMATCHING: 45.45%. Target keeps the global pointer in r4 across the or/clrlwi; MWCC reloads it (lis/addi/lwz) before the stb. Register allocation not controllable from C.
void fn_1_7FC(u8 mask)
{
    lbl_1_bss_1681C->flag = (u8)(lbl_1_bss_1681C->flag | mask);
}

// NONMATCHING: 45.45%. Target keeps the global pointer in r4 across the andc/clrlwi; MWCC reloads it before the stb. Register allocation not controllable from C.
void fn_1_81C(u8 mask)
{
    lbl_1_bss_1681C->flag = (u8)(lbl_1_bss_1681C->flag & ~mask);
}

void fn_1_854(u32 mask)
{
    lbl_1_bss_16818->flag |= mask;
}

void fn_1_870(u32 mask)
{
    lbl_1_bss_16818->flag &= ~mask;
}

void fn_1_8A4(void)
{
}

// NONMATCHING: 18.75%. Target reloads the global twice (r4 and r5) and uses lbzx/lhax; MWCC collapses to one register and reorders. Not controllable from C.
s16 fn_1_904(s32 arg)
{
    MGT_FLOW *f = lbl_1_bss_16818;
    s8 idx = (s8)*((u8 *)f + arg + 0x22A);
    return *(s16 *)((u8 *)f + idx * 0x30 + 0x16);
}

// NONMATCHING: 18.75%. Same register reallocation as fn_1_904 (target uses r4/r5, MWCC collapses).
s16 fn_1_938(s32 arg)
{
    MGT_FLOW *f = lbl_1_bss_16818;
    s8 idx = (s8)*((u8 *)f + arg + 0x22A);
    return *(s16 *)((u8 *)f + idx * 0x30 + 0x14);
}

// NONMATCHING: 33.33%. Target emits extsb after the lbz; MWCC omits it (char-return codegen). Not controllable from C.
s8 fn_1_99C(void)
{
    return lbl_1_bss_1681C->mgNum;
}

s8 fn_1_9B4(s32 arg)
{
    return (s8)*(s16 *)((u8 *)lbl_1_bss_16818 + arg * 0x30 + 0x14);
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

// NONMATCHING: 52.63%. Target keeps the current-process pointer in r7 (5th arg) with no frame; MWCC spills it to r31 + a frame. Register allocation not controllable from C.
void fn_1_DA4(void (*arg)(void))
{
    HUPROCESS *parent = HuPrcCurrentGet();
    HuPrcChildCreate(arg, 0x64, 0x1000, 0, parent);
    HuPrcVSleep();
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

void fn_1_D29C(s32);

void fn_1_1DE8(void)
{
    lbl_1_bss_16818->stat |= 0x80;
    lbl_1_bss_16818->stat &= ~0x100;
}

void fn_1_1E1C(void)
{
    char *path = mgDataTbl[(s8)lbl_1_bss_16818->mgTbl[lbl_1_bss_1681C->order[lbl_1_bss_1681C->cur]].mgIdx].movie[0];

    HuTHPSprCreateVol(path, 1, 0, 0.0f);
    Hu3DModelMatHookSet(lbl_1_bss_16818->mdl[5], HuTHPMatHook);
}

void fn_1_1ED4(void)
{
    s8 usednum[256];
    s8 *arr;
    s8 first;
    int num;
    int i, j;
    int no;

    first = *(s8 *)((u8 *)lbl_1_bss_1681C + lbl_1_bss_16818->mgTblNum);
    lbl_1_bss_1681C->cur = 0;
    num = lbl_1_bss_16818->mgTblNum;
    arr = (s8 *)lbl_1_bss_1681C + 1;
    for (i = num; i--;) {
        usednum[i] = arr[i];
    }
    for (i = num; i--;) {
        if (i) {
            no = frandmod(i + 1);
        } else {
            no = 0;
        }
        arr[i] = usednum[no];
        for (j = 0; j <= i; j++) {
            if (j != no && j > no) {
                usednum[j - 1] = usednum[j];
            }
        }
    }
    if (lbl_1_bss_1681C->order[0] == first) {
        lbl_1_bss_1681C->order[0] = lbl_1_bss_1681C->order[lbl_1_bss_16818->mgTblNum / 2];
        lbl_1_bss_1681C->order[lbl_1_bss_16818->mgTblNum / 2] = first;
    }
}

void fn_1_2078(void)
{
    s8 usednum[256];
    int i, j, k, no;
    OMOVLHIS *his;
    s8 *arr;
    int mdl;

    lbl_1_bss_1681C->flag = 0;
    lbl_1_bss_1681C->cur = 0;
    if (lbl_1_bss_16818->stat & 0x20) {
        lbl_1_bss_16818->stat &= ~0x20;
        HuWinExClose(lbl_1_bss_16818->winId);
        HuWinHomeClear(lbl_1_bss_16818->winId);
    }
    his = omOvlHisGet(0);
    omOvlHisChg(0, his->ovl, 0, his->stat);
    lbl_1_bss_1681C->camPos.z += 2000.0f;
    Hu3DCameraViewportSet(0x10, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(0x10, 0, 0, 640, 480);
    Hu3DCameraPosSetV(0x10, &lbl_1_bss_1681C->camPos, &lbl_1_bss_1681C->camUp, &lbl_1_bss_1681C->camTarget);
    Hu3DCameraPerspectiveSet(0x10, lbl_1_bss_1681C->aspect, 20.0f, 60000.0f, 1.2f);
    Hu3DCameraKill(2);
    fn_1_D29C(0);
    lbl_1_bss_16818->stat &= ~0x40000000;
    for (k = 4; k--;) {
        lbl_1_bss_1681C->playerNo[k] = k;
    }
    arr = lbl_1_bss_1681C->playerNo;
    for (i = 4; i--;) {
        usednum[i] = arr[i];
    }
    for (i = 4; i--;) {
        if (i) {
            no = frandmod(i + 1);
        } else {
            no = 0;
        }
        arr[i] = usednum[no];
        for (j = 0; j <= i; j++) {
            if (j != no && j > no) {
                usednum[j - 1] = usednum[j];
            }
        }
    }
    Hu3DModelLayerSet(lbl_1_bss_16818->mdl[4], 3);
    Hu3DModelAttrSet(lbl_1_bss_16818->mdl[4], 1);
    Hu3DModelCameraSet(lbl_1_bss_16818->mdl[4], 4);
    mdl = lbl_1_bss_16818->mdl[2];
    Hu3DMotionStartEndSet(mdl, 0.0f, 28.0f);
    Hu3DMotionTimeSet(mdl, 0.0f);
    Hu3DMotionShapeTimeSet(mdl, 0.0f);
    Hu3DModelAttrSet(lbl_1_bss_16818->mdl[2], 0x40000002);
    lbl_1_bss_16818->stat &= ~0x4000;
    HuTHPClose();
    HuPrcVSleep();
    while (HuTHPProcCheck() != 0) {
        HuPrcVSleep();
    }
}

void fn_1_242C(void)
{
    int mdl;
    int mdl2;

    mdl = lbl_1_bss_16818->mdl[0];
    Hu3DMotionStartEndSet(mdl, 0.0f, 179.0f);
    Hu3DMotionTimeSet(mdl, 0.0f);
    Hu3DMotionShapeTimeSet(mdl, 0.0f);
    Hu3DModelAttrReset(lbl_1_bss_16818->mdl[0], 0x40000082);
    Hu3DModelAttrReset(lbl_1_bss_16818->mdl[0], 0x40000041);
    Hu3DCameraMotionOn(lbl_1_bss_16818->mdl[0], 0x10);
    while (!Hu3DMotionEndCheck(lbl_1_bss_16818->mdl[0])) {
        HuPrcVSleep();
    }
    Hu3DCameraMotionOff(lbl_1_bss_16818->mdl[0]);
    mdl2 = lbl_1_bss_16818->mdl[0];
    Hu3DMotionStartEndSet(mdl2, 180.0f, 359.0f);
    Hu3DMotionTimeSet(mdl2, 180.0f);
    Hu3DMotionShapeTimeSet(mdl2, 180.0f);
    Hu3DModelAttrReset(lbl_1_bss_16818->mdl[0], 0x40000082);
    Hu3DModelAttrSet(lbl_1_bss_16818->mdl[0], 0x40000041);
    lbl_1_bss_16818->stat |= 0x40;
    HuPrcEnd();
}

void fn_1_25E4(void)
{
    lbl_1_bss_16818->stat &= ~0x40;
    HuPrcChildCreate(fn_1_242C, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
}

f32 lbl_1_data_1B4[13] = { 0.0f, 0.0f, 1000.0f, 0.0f, -3000.0f, 1000.0f, 0.0f, 0.0f, -8000.0f, -3000.0f, -3000.0f, -3000.0f, -3000.0f };

void fn_1_263C(int camBit)
{
    f32 spacing = 200.0f;
    int i;
    int mdl;

    for (i = 4; i--;) {
        mdl = lbl_1_bss_16818->mdl[i + 7];
        Hu3DModelPosSet(mdl, -(1.5f * spacing) + i * spacing, 0.0f, lbl_1_data_1B4[lbl_1_bss_16818->player[i].charNo]);
        Hu3DModelAttrReset(mdl, 1);
        Hu3DModelCameraSet(mdl, 2);
        Hu3DModelLayerSet(mdl, 0);
        mdl = lbl_1_bss_16818->mdl[i + 12];
        Hu3DModelAttrReset(mdl, 1);
        Hu3DModelLayerSet(mdl, 4);
        Hu3DModelCameraSet(mdl, camBit);
    }
    Hu3DCameraCreate(2);
    Hu3DCameraPerspectiveSet(2, 1.2f, 15000.0f, 40000.0f, 1.2f);
    Hu3DCameraViewportSet(2, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(2, 0, 0, 640, 480);
    Hu3DCameraPosSet(2, 0.0f, 100.0f, 30000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 100.0f, 0.0f);
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