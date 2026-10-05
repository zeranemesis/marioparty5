#include "dolphin/types.h"

#include "game/data.h"
#include "game/process.h"
#include "game/sprite.h"

#include "math.h"
#include "string.h"

#include "datanum/mgconst.h"

typedef struct MgScore_s MGSCORE;

MGSCORE *MgScoreCreate(int digitFile, int unitFile, BOOL dispLeadZeroF);
void MgScoreMaxDigitSet(MGSCORE *score, int maxDigit);
void MgScoreDispOn(MGSCORE *score);
void MgScoreColorSet(MGSCORE *score, u8 r, u8 g, u8 b);
void MgScorePosSet(MGSCORE *score, float posX, float posY);
void MgScoreValueSet(MGSCORE *score, int value);

extern u8 lbl_1_bss_EC0[];
extern u8 lbl_1_bss_E74[];
extern u8 lbl_1_bss_E68[];

// NONMATCHING: register allocation (lbz into r0 vs r3)
s8 fn_1_12B4C(void)
{
    return lbl_1_bss_EC0[0x41];
}

// NONMATCHING: extsb vs clrlwi for sign-extension
void fn_1_12AE4(s8 arg0)
{
    lbl_1_bss_EC0[0x3E] = arg0;
    lbl_1_bss_EC0[0x3F] = 0;
}

// NONMATCHING: stack frame and control flow differences
s32 fn_1_12B08(void)
{
    s8 val = lbl_1_bss_EC0[0x3E];

    if (val == 3) return 1;
    if (val >= 6) return 1;
    if (val >= 1) return 0;
    return 1;
}

// NONMATCHING: stack frame size, register allocation, extsb vs clrlwi
void fn_1_15BC8(void)
{
    u8 flag;
    s16 grpId0, grpId1, grpId2;

    flag = lbl_1_bss_E74[6];
    if (flag == 0) {
        return;
    }

    grpId0 = *(s16 *)(lbl_1_bss_E74 + 0);
    grpId1 = *(s16 *)(lbl_1_bss_E74 + 2);
    grpId2 = *(s16 *)(lbl_1_bss_E74 + 4);

    HuSprGrpPosSet(grpId0, 288.0f, 800.0f);
    HuSprGrpPosSet(grpId1, 288.0f, 240.0f);
    HuSprGrpPosSet(grpId2, 288.0f, 240.0f);
}

// NONMATCHING: stack frame size, register allocation, loop structure
s32 fn_1_15C74(void *arg0)
{
    s16 grpId;
    void *animData;
    s32 i;
    s16 *data = (s16 *)arg0;

    grpId = HuSprGrpCreate(5);
    *(s16 *)(lbl_1_bss_E68 + 0) = grpId;

    animData = HuDataSelHeapReadNum(0x98004E, 0x1000, 2);
    animData = HuSprAnimRead(animData);
    HuSprGrpMemberSet(grpId, 0, HuSprCreate(animData, 10, 0));

    for (i = 4; i >= 0; i--) {
        s16 val = data[i * 0x34 / 2 + 0x1A / 2];
        animData = HuDataSelHeapReadNum(0x980040 + val, 0x1000, 2);
        animData = HuSprAnimRead(animData);
        HuSprGrpMemberSet(grpId, i + 1, HuSprCreate(animData, 10, 0));
        HuSprPosSet(grpId, i + 1, 0.0f, -5.0f);
        HuSprAttrSet(grpId, i + 1, 4);
    }

    HuSprGrpDrawNoSet(grpId, 0);
    HuSprGrpTPLvlSet(grpId, 0.0f);

    return grpId;
}

// NONMATCHING: stack frame size, register allocation, 3D math handling
void fn_1_15DB8(void *arg0, void *arg1)
{
    f32 pos[3];
    s32 i;
    s16 *data;
    s32 *arg1Data;
    s16 grpId;
    s16 counter;
    void *animData;

    data = (s16 *)arg0;
    arg1Data = (s32 *)arg1;
    grpId = *(s16 *)(lbl_1_bss_E68 + 0);

    if (arg0 == NULL || arg1 == NULL) {
        HuSprGrpTPLvlSet(grpId, 0.0f);
    } else {
        HuSprGrpTPLvlSet(grpId, 1.0f);

        pos[0] = arg1Data[0];
        pos[1] = arg1Data[1];
        pos[2] = arg1Data[2] + 160.0f;
        Hu3D3Dto2D(pos, pos);
        HuSprGrpPosSet(grpId, pos[0], pos[1]);

        for (i = 4; i >= 0; i--) {
            s16 val = data[i * 0x34 / 2 + 0x1A / 2];
            animData = HuDataSelHeapReadNum(0x980040 + val, 0x1000, 2);
            animData = HuSprAnimRead(animData);
            HuSprAttrSet(grpId, i + 1, 4);
        }

        counter = *(s16 *)(lbl_1_bss_E68 + 0x18);
        counter++;
        *(s16 *)(lbl_1_bss_E68 + 0x18) = counter;
        HuSprAttrReset(grpId, counter, 4);
    }
}