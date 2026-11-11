#include "REL/mgtourDll.h"
#include "math.h"
extern void fn_1_C76C(void);

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

static inline BOOL MgtAngleSub(float *angle, float end, float speed)
{
    float diff = *angle - end;
    float change = fmodf(diff, 180);
    float wrap = fmodf(diff, 360);
    if (fabsf(wrap) > 180.0f) {
        if (change > 0) {
            change = -(180.0f - fabsf(change));
        } else {
            change = 180.0f - fabsf(change);
        }
    }
    if (fabsf(change) < speed) {
        *angle = end;
        return TRUE;
    }
    if (change > 0) {
        change -= speed;
    } else {
        change += speed;
    }
    *angle = end + change;
    return FALSE;
}

void fn_1_3DE4(void)
{
    Vec dir;
    Vec aimPos;
    MGT_PLAYER *player;
    int i;
    int doneNum;
    float rotEnd;
    for (player = &lbl_1_bss_16818->player[0], i = 0; i < MGTOUR_PLAYER_MAX; i++, player++) {
        switch (lbl_1_bss_16818->mode) {
            case 0:
                player->rotX = 0;
                break;
            case 1:
                aimPos = lbl_1_bss_16818->shakePos;
                aimPos.z += 350;
                VECSubtract(&aimPos, &player->pos, &aimPos);
                aimPos.y = 0;
                VECNormalize(&aimPos, &dir);
                if (dir.z > 1) {
                    dir.z = 1;
                } else if (dir.z < -1) {
                    dir.z = -1;
                }
                rotEnd = acosf(dir.z);
                if (dir.x < 0) {
                    rotEnd = -rotEnd;
                }
                player->rotX = (180 * rotEnd) / M_PI;
                break;
        }
        if (!MgtAngleSub(&player->rotY, player->rotX, 0.01f)) {
            if (player->state != 1) {
                CharMotionShiftSet(player->charNo, player->motId[1], 0, 5, HU3D_MOTATTR_LOOP);
                player->state = 1;
            }
        }
    }
    while (1) {
        doneNum = 0;
        for (player = &lbl_1_bss_16818->player[0], i = MGTOUR_PLAYER_MAX; i--; player++) {
            if (MgtAngleSub(&player->rotY, player->rotX, lbl_1_bss_16818->rotSpeed)) {
                if (player->state != 0) {
                    CharMotionShiftSet(player->charNo, player->motId[0], 0, 5, HU3D_MOTATTR_LOOP);
                    player->state = 0;
                }
                doneNum++;
            }
            Hu3DModelRotSet(player->modelId, 0, player->rotY, 0);
        }
        if (doneNum == 4) {
            break;
        }
        HuPrcVSleep();
    }
    lbl_1_bss_16818->stat |= 0x1000;
    HuPrcEnd();
}

void fn_1_44E0(int mode, float speed)
{
    lbl_1_bss_16818->stat &= ~0x1000;
    lbl_1_bss_16818->mode = mode;
    lbl_1_bss_16818->rotSpeed = speed;
    HuPrcChildCreate(fn_1_3DE4, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
}

void fn_1_456C(void)
{
}

void fn_1_485C(void)
{
    lbl_1_bss_16818->stat |= 0x2000;
    HuPrcChildCreate(fn_1_456C, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
}

void fn_1_48B4(void)
{
}

void fn_1_5540(void)
{
    OMOVLHIS *his;
    if (lbl_1_bss_16818->bgmId >= 0) {
        HuAudSStreamFadeOut(lbl_1_bss_16818->bgmId, 1000);
        lbl_1_bss_16818->bgmId = -1;
    }
    HuDataDirClose(DATA_mgtour);
    HuDataDirClose(DATA_instpic);
    CharModelDataClose(-1);
    fn_1_C76C();
    HuTHPClose();
    GWMgInstFSet(TRUE);
    his = omOvlHisGet(0);
    omOvlHisChg(0, his->ovl, 0, his->stat);
    HuWinExClose(lbl_1_bss_16818->winId);
    HuWinExKill(lbl_1_bss_16818->winId);
    if (lbl_1_bss_16818->seId >= 0) {
        HuAudFXStop(lbl_1_bss_16818->seId);
        lbl_1_bss_16818->seId = -1;
    }
    WipeCreate(WIPE_MODE_OUT, 6, 60);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
    _ClearFlag(FLAG_INST_NOHISCHG);
    _ClearFlag(FLAG_INST_MGMODE);
    memset(lbl_1_bss_1681C, 0, sizeof(MGT_MODEWORK));
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