#include "types.h"

#ifdef __MWERKS__
/* the original game.c never pulls in math.h: skipping it keeps its sqrtf literal pool out of .rodata */
#define _MATH_H
#endif

#include "string.h"
#include "game/gamework.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/hsfformat.h"
#include "game/charman.h"
#include "game/audio.h"
#include "game/flag.h"
#include "game/gamemes.h"
#include "game/wipe.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"
#include "REL/m548dll.h"

s32 rand8(void);
/* not declared in hu3d.h (the original compiles it as a plain-C function) */
f32 Hu3DMotionMaxTimeGet(HU3DMODELID modelId);
double sin(double x);
double cos(double x);

/* m548.c */
extern M548State lbl_1_bss_3F0;
extern M548Game lbl_1_bss_3D0;
extern M548Fx lbl_1_bss_3B8;
extern M548CryPos lbl_1_bss_384[2];
extern OMCAMERAVIEW lbl_1_bss_58[8];
extern M548Cry lbl_1_bss_0[2];
extern M548Vec3 lbl_1_bss_44;
extern M548Vec3 lbl_1_bss_38;
extern MGTIMER *lbl_1_bss_54;
extern s16 lbl_1_bss_3B4;
extern s16 lbl_1_bss_5D2[3];
extern s16 lbl_1_bss_5D0;
extern M548Vec2 lbl_1_data_288[13];
s32 fn_1_196C(void);
void fn_1_194C(void);

void fn_1_2148(void);
s32 fn_1_4658(s16 arg0);
void fn_1_4AA0(void);
void fn_1_4FC4(M548Vec3 *arg0);
void fn_1_5098(void);

#define M548 lbl_1_bss_3F0

/* unit-owned .bss, declared in reverse address order so the offsets match the original */
s32 lbl_1_bss_5F0;
u8 lbl_1_bss_5EC;
s32 lbl_1_bss_5DC[4];
f32 lbl_1_bss_5D8;

/* unit-owned .data, in original address order */
s32 lbl_1_data_398 = -1;
M548Vec2 lbl_1_data_39C[17] = {
    { -0.75f, -0.125f }, { -0.75f, 0.125f }, { -0.75f, 0.375f }, { -0.75f, 0.625f },
    { -0.5f, 0.0f },     { -0.5f, 0.25f },   { -0.5f, 0.5f },    { -0.25f, -0.125f },
    { -0.25f, 0.0f },    { -0.25f, 0.125f }, { -0.25f, 0.25f },  { -0.25f, 0.375f },
    { -0.25f, 0.5f },    { -0.25f, 0.625f }, { -0.25f, 0.75f },  { 0.0f, -0.125f },
    { 0.0f, 0.0f },
};
f32 lbl_1_data_424[13] = {
    24.0f, 24.0f, 24.0f, 20.0f, 28.0f, 24.0f, 24.0f, 28.0f, 28.0f, 28.0f, 28.0f, 28.0f, 28.0f,
};
M548Vec3 lbl_1_data_458[3] = { { 0.0f, 185.0f, 0.0f }, { -85.0f, 185.0f, 0.0f }, { 85.0f, 185.0f, 0.0f } };
M548Vec3 lbl_1_data_47C[3] = { { -20.0f, 0.0f, 0.0f }, { -20.0f, -21.0f, 0.0f }, { -20.0f, 21.0f, 0.0f } };
f32 lbl_1_data_4A0[3] = { 550.0f, 420.0f, 420.0f };
char lbl_1_data_4AC[] = "test11_tex_we-itemhook-r";
f32 lbl_1_data_4C8[4] = { 0.0f, 80.0f, -80.0f, 0.0f };
f32 lbl_1_data_4D8[4] = { 240.0f, 160.0f, 240.0f, 200.0f };
f32 lbl_1_data_4E8[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 lbl_1_data_4F8[4] = { 2.0f, 2.0f, 1.5f, 1.0f };

void fn_1_1D84(void)
{
    s32 i;
    s32 j;

    if (lbl_1_bss_3D0.unk4 != 9) {
        for (i = 0; i < 2; i++) {
            if ((i != 0 || lbl_1_bss_3D0.unk0 != 0) && M548.slots[i].motion == 1) {
                if (M548.slots[i].time >= 24.0f && M548.slots[i].time < 80.0f) {
                    lbl_1_bss_0[i].unk4++;
                    if (lbl_1_bss_0[i].unk4 == lbl_1_bss_0[i].unk8) {
                        lbl_1_bss_0[i].unk4 = 0;
                        for (j = 0; j < lbl_1_bss_0[i].unkC; j++) {
                            CharModelCryCreate(M548.chars[i].charNo, lbl_1_data_288[M548.chars[i].charNo].x,
                                               lbl_1_data_288[M548.chars[i].charNo].y);
                        }
                        if (lbl_1_bss_0[i].unk0 != 0) {
                            lbl_1_bss_0[i].unk8 = rand8() % 10 + 60;
                            lbl_1_bss_0[i].unk0 = 0;
                        } else {
                            lbl_1_bss_0[i].unk8 = 10;
                            lbl_1_bss_0[i].unk0++;
                        }
                        lbl_1_bss_0[i].unkC = rand8() % 2 + 6;
                    }
                }
            }
        }
    }
}

void fn_1_202C(void)
{
    s16 i;

    for (i = 1; i < 12; i++) {
        Hu3DModelPosSet(M548.models[i].id, M548.models[i].x, M548.models[i].y, M548.models[i].z);
        Hu3DModelRotSet(M548.models[i].id, M548.models[i].rotX, M548.models[i].rotY, M548.models[i].rotZ);
    }
}

void fn_1_2148(void)
{
    s32 i;

    lbl_1_bss_3D0.unk0 = 0;
    lbl_1_bss_3D0.unk2 = 0;
    lbl_1_bss_3D0.unk4 = 9;
    lbl_1_bss_3D0.unk6 = 0;
    lbl_1_bss_3D0.unk8 = 0;
    lbl_1_bss_3D0.unkA = 0;
    lbl_1_bss_3D0.unkC = 0;
    for (i = 0; i < 2; i++) {
        lbl_1_bss_3D0.unk10[i] = lbl_1_data_424[M548.chars[i].charNo] - 8.0f;
        lbl_1_bss_3D0.unk18[i] = lbl_1_data_424[M548.chars[i].charNo] - 8.0f;
    }
    for (i = 0; i < 3; i++) {
        lbl_1_bss_58[i].center.x = lbl_1_data_458[i].x;
        lbl_1_bss_58[i].center.y = lbl_1_data_458[i].y;
        lbl_1_bss_58[i].center.z = lbl_1_data_458[i].z;
        lbl_1_bss_58[i].rot.x = lbl_1_data_47C[i].x;
        lbl_1_bss_58[i].rot.y = lbl_1_data_47C[i].y;
        lbl_1_bss_58[i].rot.z = lbl_1_data_47C[i].z;
        lbl_1_bss_58[i].zoom = lbl_1_data_4A0[i];
    }
}

void fn_1_23A4(void)
{
    u8 idx;

    idx = rand8() % 10;
    if (lbl_1_bss_5EC == idx) {
        idx = (idx + 1) % 10;
    }
    lbl_1_bss_5EC = idx;
    idx += 7;
    if (M548.timerB0++ % 4 == 0) {
        Hu3DTexScrollPosSet(lbl_1_bss_3B4, lbl_1_data_39C[idx].x, lbl_1_data_39C[idx].y, 0.0f);
        HuAudFXPlay(0x83B);
    }
    if (WipeCheckEnd() == 0 && M548.timerB0 > 0x78) {
        M548.timerB0 = 0;
        M548.stateA0++;
        idx = 6;
        Hu3DTexScrollPosSet(lbl_1_bss_3B4, lbl_1_data_39C[idx].x, lbl_1_data_39C[idx].y, 0.0f);
        HuAudFXPlay(0x83C);
        lbl_1_bss_5F0 = 0;
    }
}

void fn_1_257C(void)
{
    if (fn_1_4658(0) != 0) {
        fn_1_2148();
        M548.stateA0++;
        M548.timerB0 = 0;
        omCameraViewMoveSimple(&lbl_1_bss_58[0], 0x3C);
    }
}

void fn_1_282C(void)
{
    if (omCameraViewCheck(1)) {
        M548.stateA0++;
        M548.stateA4 = 0;
        omCameraViewMoveSimple(&lbl_1_bss_58[1], 0x3C);
        MgSeqModeNext();
    }
}

void fn_1_2894(void)
{
    lbl_1_bss_54 = MgTimerCreate(0);
    MgTimerParamSet(lbl_1_bss_54, 300, 0, 0);
    MgTimerPosSet(lbl_1_bss_54, 288.0f, 64.0f);
}

void fn_1_2908(void)
{
    if (lbl_1_bss_5DC[0] == 0) {
        lbl_1_bss_5DC[2] = rand8() % 4 * 30 + 30 + rand8() % 30;
        lbl_1_bss_5DC[0]++;
        return;
    }
    if (lbl_1_bss_5DC[2]-- <= 0) {
        if (lbl_1_bss_3D0.unk2 != 0) {
            if (rand8() % 3 != 0) {
                M548.chars[lbl_1_bss_3D0.unk6].btnDown = 0x100;
            } else {
                M548.chars[lbl_1_bss_3D0.unk6].btnDown = 0x200;
            }
        } else if (rand8() % 3 != 0) {
            M548.chars[lbl_1_bss_3D0.unk6].btnDown = 0x200;
        } else {
            M548.chars[lbl_1_bss_3D0.unk6].btnDown = 0x100;
        }
    }
}

void fn_1_2AD4(void)
{
    s16 prev = lbl_1_bss_3D0.unk4;
    s32 tbl[4] = { 1, 3, 5, 6 };

    if (omCameraViewCheck(1)) {
        if (M548.stateA4 == 0) {
            lbl_1_bss_54 = MgTimerCreate(0);
            MgTimerParamSet(lbl_1_bss_54, 300, 0, 0);
            MgTimerPosSet(lbl_1_bss_54, 288.0f, 64.0f);
            MgTimerModeOnSet(lbl_1_bss_54, 1);
            M548.stateA4++;
            lbl_1_bss_5DC[0] = 0;
            return;
        }
        if (M548.chars[lbl_1_bss_3D0.unk6].cpuLevel != 0) {
            fn_1_2908();
        }
        if (M548.chars[lbl_1_bss_3D0.unk6].btnDown & 0x100) {
            lbl_1_bss_3D0.unk4--;
        } else if (M548.chars[lbl_1_bss_3D0.unk6].btnDown & 0x200) {
            lbl_1_bss_3D0.unk4 -= 2;
        }
        if (prev != lbl_1_bss_3D0.unk4) {
            M548.stateA4 = 0;
            MgTimerKill(lbl_1_bss_54);
            lbl_1_bss_54 = NULL;
            if (lbl_1_bss_3D0.unk4 > 0) {
                lbl_1_bss_3D0.unkC = 0;
                if (lbl_1_bss_3D0.unk2 != 0) {
                    M548.stateA0 = tbl[lbl_1_bss_3D0.unk6];
                    lbl_1_bss_3D0.unkA = 0;
                    lbl_1_bss_3D0.unk8 = 0;
                    if (M548.stateA8 > 3) {
                        if (rand8() % 2 != 0) {
                            M548.stateA0 = tbl[2];
                            return;
                        }
                    }
                    HuAudFXPlay(0x83F);
                    if (fn_1_196C() != 0) {
                        M548.stateA0 = 7;
                    }
                } else {
                    if (M548.stateA8 >= 2) {
                        lbl_1_bss_3D0.unk2 = 1;
                    }
                    HuAudFXPlay(0x83E);
                    lbl_1_bss_3D0.unk8 = prev - lbl_1_bss_3D0.unk4;
                    lbl_1_bss_3D0.unkA = 0;
                    M548.stateA0 = tbl[3];
                    M548.stateA4 = 0;
                }
            } else {
                lbl_1_bss_3D0.unk4 = 0;
                M548.stateA0 = 4;
                lbl_1_bss_3D0.unkA = 0;
                lbl_1_bss_3D0.unk8 = 0;
            }
        } else if (MgTimerDoneCheck(lbl_1_bss_54)) {
            lbl_1_bss_3D0.unk4 = 0;
            M548.stateA0 = 4;
            M548.stateA4 = 0;
            lbl_1_bss_3D0.unkA = 0;
            lbl_1_bss_3D0.unk8 = 0;
        }
    }
}

void fn_1_309C(void)
{
    s32 tbl[2] = { 0, 2 };
    HuVecF objPos;

#ifdef TARGET_PC
    objPos.x = objPos.y = objPos.z = 0.0f;
#endif
    switch (M548.stateA4) {
    case 0:
        fn_1_194C();
        M548.slots[lbl_1_bss_3D0.unk6].speed = 1.0f;
        M548.slots[lbl_1_bss_3D0.unk6].attrReset = 0x40000001;
        M548.stateA4++;
        lbl_1_bss_3D0.unkC = 0;
        return;
    case 1:
        lbl_1_bss_3D0.unkC++;
        Hu3DModelObjPosGet(M548.models[lbl_1_bss_3D0.unk6 + 12].id, lbl_1_data_4AC, &objPos);
        objPos.y -= lbl_1_bss_384[lbl_1_bss_3D0.unk6].unk4;
        lbl_1_bss_44.y = objPos.y + lbl_1_bss_384[lbl_1_bss_3D0.unk6].unk14;
        if (lbl_1_bss_3D0.unk6 != 0) {
            lbl_1_bss_44.x = objPos.x + (lbl_1_bss_384[1].unk10 - lbl_1_bss_384[1].unk0);
        } else {
            lbl_1_bss_44.x = objPos.x + -(lbl_1_bss_384[0].unk10 - lbl_1_bss_384[0].unk0);
        }
        if (lbl_1_bss_3D0.unkC == 0x14) {
            M548.stateA4++;
            lbl_1_bss_3D0.unkC = 0;
            lbl_1_bss_38.y = 300.0f - lbl_1_bss_44.y;
            lbl_1_bss_38.z = lbl_1_bss_44.y;
            lbl_1_bss_38.x = lbl_1_bss_44.x;
            omCameraViewMoveSimple(&lbl_1_bss_58[(lbl_1_bss_3D0.unk6 ^ 1) + 1], 0x18);
            HuAudFXPanning(HuAudFXPlay(0x842), 0x50 - (lbl_1_bss_3D0.unk6 << 5));
            return;
        }
        break;
    case 2:
        lbl_1_bss_3D0.unkC++;
        if (lbl_1_bss_3D0.unkC == 4) {
            M548.slots[lbl_1_bss_3D0.unk6 ^ 1].motion = 1;
            M548.slots[lbl_1_bss_3D0.unk6 ^ 1].time = 1.0f;
            M548.slots[lbl_1_bss_3D0.unk6 ^ 1].speed = 1.0f;
        }
        lbl_1_bss_44.x -= lbl_1_bss_38.x / 12.0f;
        lbl_1_bss_44.y = lbl_1_bss_38.z + lbl_1_bss_38.y * sin(3.141592653589793 * (lbl_1_bss_3D0.unkC * 90 / 12) / 180.0);
        if (lbl_1_bss_3D0.unkC == 12) {
            M548.stateA4++;
            lbl_1_bss_44.x = 0.0f;
            lbl_1_bss_44.y = 300.0f;
            lbl_1_bss_3D0.unkC = 0;
            lbl_1_bss_38.x = lbl_1_bss_384[lbl_1_bss_3D0.unk6 ^ 1].unk8 +
                             (lbl_1_bss_384[lbl_1_bss_3D0.unk6 ^ 1].unk10 - lbl_1_bss_384[lbl_1_bss_3D0.unk6 ^ 1].unk0);
            lbl_1_bss_38.y = (300.0f - lbl_1_bss_384[lbl_1_bss_3D0.unk6 ^ 1].unkC) -
                             (lbl_1_bss_384[lbl_1_bss_3D0.unk6 ^ 1].unk14 - lbl_1_bss_384[lbl_1_bss_3D0.unk6 ^ 1].unk4);
            return;
        }
        break;
    case 3:
        lbl_1_bss_3D0.unkC++;
        if (lbl_1_bss_3D0.unk6 != 0) {
            lbl_1_bss_44.x -= lbl_1_bss_38.x / lbl_1_bss_3D0.unk10[lbl_1_bss_3D0.unk6 ^ 1];
        } else {
            lbl_1_bss_44.x += lbl_1_bss_38.x / lbl_1_bss_3D0.unk10[lbl_1_bss_3D0.unk6 ^ 1];
        }
        lbl_1_bss_44.y = (lbl_1_bss_384[lbl_1_bss_3D0.unk6 ^ 1].unk14 - lbl_1_bss_384[lbl_1_bss_3D0.unk6 ^ 1].unk4) +
                         (lbl_1_bss_384[lbl_1_bss_3D0.unk6 ^ 1].unkC +
                          lbl_1_bss_38.y *
                              sin(3.141592653589793 *
                                  (90.0f + (f32)(lbl_1_bss_3D0.unkC * 90) / lbl_1_bss_3D0.unk10[lbl_1_bss_3D0.unk6 ^ 1]) /
                                  180.0));
        if ((f32)lbl_1_bss_3D0.unkC == lbl_1_bss_3D0.unk18[lbl_1_bss_3D0.unk6 ^ 1]) {
            lbl_1_bss_3D0.unkC = 0;
            M548.stateA4++;
            if (lbl_1_bss_3D0.unk6 != 0) {
                lbl_1_bss_44.x = -lbl_1_bss_38.x;
            } else {
                lbl_1_bss_44.x = lbl_1_bss_38.x;
            }
            lbl_1_bss_44.y = lbl_1_bss_384[lbl_1_bss_3D0.unk6 ^ 1].unkC +
                             (lbl_1_bss_384[lbl_1_bss_3D0.unk6 ^ 1].unk14 - lbl_1_bss_384[lbl_1_bss_3D0.unk6 ^ 1].unk4);
            HuAudFXPlay(0x841);
            return;
        }
        break;
    case 4:
        lbl_1_bss_3D0.unkC++;
        Hu3DModelObjPosGet(M548.models[(lbl_1_bss_3D0.unk6 ^ 1) + 12].id, lbl_1_data_4AC, &objPos);
        if (lbl_1_bss_3D0.unk6 != 0) {
            lbl_1_bss_44.x = objPos.x - (lbl_1_bss_384[0].unk10 - lbl_1_bss_384[0].unk0);
            lbl_1_bss_44.y = objPos.y + (lbl_1_bss_384[0].unk14 - lbl_1_bss_384[0].unk4);
        } else {
            lbl_1_bss_44.x = objPos.x + (lbl_1_bss_384[1].unk10 - lbl_1_bss_384[1].unk0);
            lbl_1_bss_44.y = objPos.y + (lbl_1_bss_384[1].unk14 - lbl_1_bss_384[1].unk4);
        }
        if (M548.slots[lbl_1_bss_3D0.unk6 ^ 1].time == 49.0f) {
            M548.slots[lbl_1_bss_3D0.unk6 ^ 1].speed = 0.0f;
        }
        if (M548.slots[lbl_1_bss_3D0.unk6 ^ 1].time >= 50.0f) {
            M548.stateA4++;
            M548.stateA0 = tbl[lbl_1_bss_3D0.unk6 ^ 1];
            M548.stateA4 = 0;
            M548.stateA8++;
            M548.slots[lbl_1_bss_3D0.unk6 ^ 1].time = 50.0f;
            lbl_1_bss_3D0.unk6 = lbl_1_bss_3D0.unk6 ^ 1;
        }
        if (M548.slots[lbl_1_bss_3D0.unk6].time == 110.0f) {
            lbl_1_bss_3D0.unk0 = 1;
            M548.slots[lbl_1_bss_3D0.unk6].motion = 0;
            M548.slots[lbl_1_bss_3D0.unk6].id = 0;
            M548.slots[lbl_1_bss_3D0.unk6].speed = 1.0f;
            M548.slots[lbl_1_bss_3D0.unk6].attrSet = 0x40000001;
            M548.slots[lbl_1_bss_3D0.unk6].attrCur = 0x40000001;
            Hu3DMotionShiftSet(M548.models[lbl_1_bss_3D0.unk6 + 12].id, M548.motionIds[lbl_1_bss_3D0.unk6][0], 0.0f,
                               10.0f, 0x40000001);
        }
        break;
    }
}

void fn_1_3DCC(void)
{
    switch (M548.stateA4) {
    case 0:
        memcpy(&lbl_1_bss_58[0], &lbl_1_bss_58[lbl_1_bss_3D0.unk6 + 1], sizeof(OMCAMERAVIEW));
        if (lbl_1_bss_3D0.unk6 != 0) {
            lbl_1_bss_58[0].center.x = lbl_1_bss_384[lbl_1_bss_3D0.unk6].unk10;
        } else {
            lbl_1_bss_58[0].center.x = -lbl_1_bss_384[lbl_1_bss_3D0.unk6].unk10;
        }
        lbl_1_bss_58[0].zoom = 250.0f;
        if (M548.chars[lbl_1_bss_3D0.unk6].charNo == 8) {
            lbl_1_bss_58[0].zoom = 400.0f;
        }
        lbl_1_bss_58[0].rot.y = 0.0f;
        omCameraViewMoveSimple(&lbl_1_bss_58[0], 0xF);
        M548.stateA4++;
        return;
    case 1:
        if (omCameraViewCheck(1)) {
            M548.stateA4++;
            M548.timerB0 = 0;
            M548.slots[lbl_1_bss_3D0.unk6 ^ 1].motion = 4;
            M548.slots[lbl_1_bss_3D0.unk6 ^ 1].attrReset = 0x40000001;
            Hu3DModelRotSet(M548_MDLID(&M548, lbl_1_bss_3D0.unk6 ^ 1, 12), 0.0f, 0.0f, 0.0f);
            return;
        }
        break;
    case 2:
        if (M548.timerB0++ > 0x14) {
            M548.stateA4++;
            M548.timerB0 = 0;
            lbl_1_bss_5F0 = 0;
            Hu3DTexScrollPosMoveSet(lbl_1_bss_3B4, 0.0f, -0.015625f, 0.0f);
            return;
        }
        break;
    case 3:
        M548.timerB0++;
        if (M548.timerB0 == 8) {
            Hu3DTexScrollPosSet(lbl_1_bss_3B4, lbl_1_data_39C[0].x, 0.0f, 0.0f);
            Hu3DTexScrollPosMoveSet(lbl_1_bss_3B4, 0.0f, -0.015625f, 0.0f);
        }
        if (M548.timerB0 == 0x10) {
            Hu3DTexScrollPosSet(lbl_1_bss_3B4, lbl_1_data_39C[0].x, lbl_1_data_39C[0].y, 0.0f);
            HuAudFXPlay(0x83D);
            HuAudSStreamFadeOut(lbl_1_data_398, 0x64);
        }
        if (M548.timerB0 >= 0xC && fn_1_4658(0) != 0) {
            M548.stateA4++;
            lbl_1_bss_58[0].zoom = 750.0f;
            lbl_1_bss_58[0].rot.x = -25.0f;
            M548.posCC = lbl_1_bss_3D0.unk6 * 300 - 150;
            M548.posD0 = 100.0 + 0.85 * (750.0 * sin(0.4363323129985824));
            M548.posD4 = 0.85 * (750.0 * cos(0.4363323129985824));
            M548.timeD8 = 0.0f;
            M548.timerB0 = 0;
            M548.camIdxB4 = 0;
            {
                M548Vec3 b;
                M548Vec3 a = { 0.0f, 50.0f, 70.0f };
                M548Vec3 *pa;
                M548Vec3 *pb;

                a.x = -120.0f + lbl_1_bss_3D0.unk6 * 240;
                pa = &a;
                b = *pa;
                pb = &b;
                fn_1_4FC4(pb);
            }
            omCameraViewMoveSimple(&lbl_1_bss_58[0], 0x14);
            omVibrate(M548.chars[lbl_1_bss_3D0.unk6].playerNo, 0x28, 4, 2);
            return;
        }
        break;
    case 4:
        if (omCameraViewCheck(1)) {
            M548.timerB0++;
            if (M548.timerB0 == 0x14) {
                omAddObjEx(M548.objman, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_5098);
                HuAudFXPanning(HuAudFXPlay(0x843), 0x50 - (lbl_1_bss_3D0.unk6 << 5));
            }
            if (M548.timerB0 == 0x1A) {
                omVibrate(M548.chars[lbl_1_bss_3D0.unk6].playerNo, 0x78, 0xC, 0);
                Hu3DModelAttrSet(M548.models[14].id, 1);
                omAddObjEx(M548.objman, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_4AA0);
                M548.fadeC0 = 1.0f;
                M548.slots[lbl_1_bss_3D0.unk6].motion = 2;
                M548.slots[lbl_1_bss_3D0.unk6].speed = 0.0f;
                M548.slots[lbl_1_bss_3D0.unk6].time = 0.0f;
                M548.slots[lbl_1_bss_3D0.unk6].attrReset = 0x40000001;
                Hu3DModelRotSet(M548_MDLID(&M548, lbl_1_bss_3D0.unk6, 12), 0.0f, 0.0f, 0.0f);
            }
            if (M548.timerB0 > 0x78) {
                MgSeqModeNext();
                M548.stateA0 = 0;
                M548.stateA4 = 0;
            }
        }
        break;
    }
}

s32 fn_1_4658(s16 arg0)
{
    f32 unusedTbl[7] = { 5.0f, 0.0f, -5.0f, 0.0f, 5.0f, 0.0f, -5.0f };
    f32 zoomOff[7] = { -5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, 5.0f };
    s32 i;

    switch (lbl_1_bss_5F0) {
    case 0:
        for (i = 0; i < 7; i++) {
            memcpy(&lbl_1_bss_58[i + 1], &lbl_1_bss_58[0], sizeof(OMCAMERAVIEW));
            if (arg0 != 0) {
                lbl_1_bss_58[i + 1].zoom += 5.0f * zoomOff[i];
                lbl_1_bss_58[0].rot.z = lbl_1_bss_3D0.unk6 * 0x32 - 0x19;
                if (i == 3) {
                    lbl_1_bss_58[i + 1].rot.z = lbl_1_bss_3D0.unk6 * 0x50 - 0x2D;
                }
                if (i == 4) {
                    lbl_1_bss_58[i + 1].rot.z = lbl_1_bss_3D0.unk6 * 0x32 - 0x19;
                }
                if (i == 5) {
                    lbl_1_bss_58[i + 1].rot.z = lbl_1_bss_3D0.unk6 * 0x46 - 0x23;
                }
                if (i >= 6) {
                    lbl_1_bss_58[i + 1].rot.z = lbl_1_bss_3D0.unk6 * 0x32 - 0x19;
                }
            } else {
                lbl_1_bss_58[i + 1].zoom += 3.0f * zoomOff[i];
            }
        }
        lbl_1_bss_5F0++;
        M548.camIdxB4 = 0;
        break;
    case 1:
        omCameraViewMoveSimple(&lbl_1_bss_58[M548.camIdxB4], 1);
        M548.camIdxB4 = (M548.camIdxB4 + 1) & 7;
        lbl_1_bss_5F0++;
        break;
    case 2:
        if (omCameraViewCheck(1)) {
            if (M548.camIdxB4 != 0) {
                omCameraViewMoveSimple(&lbl_1_bss_58[M548.camIdxB4], 1);
                M548.camIdxB4 = (M548.camIdxB4 + 1) & 7;
            } else {
                return 1;
            }
        }
        break;
    }
    return 0;
}

void fn_1_4AA0(void)
{
    f32 fade;
    HSFMATERIAL *mat;
    s32 timer;
    HU3DMODEL *modelP;
    s16 player;
    s32 i;
    HSFDATA *hsf;
    s32 unused;

    player = lbl_1_bss_3D0.unk6;
    unused = 1;
    modelP = &Hu3DData[M548_MDLID(&M548, player, 12)];
    hsf = modelP->hsf;
    mat = hsf->material;
    if (M548.fadeC0 == 0.0f) {
        M548.timerB8++;
        timer = M548.timerB8;
        if (timer == 0x19) {
            HuAudFXPlay(0x844);
        }
        if (timer <= 0x19) {
            lbl_1_bss_5F0 = 0;
            M548.slots[lbl_1_bss_3D0.unk6].time = (10.0f * (f32)timer) / 25.0f;
            Hu3DModelPosSet(M548_MDLID(&M548, player, 12), M548.posCC, (M548.posD0 * (f32)timer) / 25.0f,
                            (M548.posD4 * (f32)timer) / 25.0f);
        } else if (M548.timeD8 == 0.0f) {
            if (fn_1_4658(1) != 0) {
                M548.timeD8 = 1.0f;
            }
        } else if (M548.timeD8 < 60.0f) {
            M548.posD0 -= 10.0 * sin(3.141592653589793 * M548.timeD8 / 180.0);
            Hu3DModelPosSet(M548_MDLID(&M548, player, 12), M548.posCC, M548.posD0, M548.posD4);
            M548.timeD8 += 1.0f;
        } else {
            M548.slots[lbl_1_bss_3D0.unk6].attrSet = 1;
        }
        return;
    }
    M548.fadeC0 -= 0.2;
    if (M548.fadeC0 < 0.0f) {
        M548.fadeC0 = 0.0f;
    }
    fade = M548.fadeC0;
    if (fade >= 0.4) {
        for (i = 0; i < modelP->hsf->materialNum; i++, mat++) {
            mat->color[0] = (f32)mat->color[0] * fade;
            mat->color[1] = (f32)mat->color[1] * fade;
            mat->color[2] = (f32)mat->color[2] * fade;
        }
    }
}

void fn_1_4F44(void)
{
    M548Vec3 v = { 0.0f, 0.0f, 0.0f };
    M548Vec3 *p = &v;

    lbl_1_bss_3B8.pos = *p;
    lbl_1_bss_3B8.state = lbl_1_bss_3B8.timer = lbl_1_bss_3B8.idx = 0;
}

void fn_1_4FC4(M548Vec3 *arg0)
{
    lbl_1_bss_3B8.state = 1;
    lbl_1_bss_3B8.timer = lbl_1_bss_3B8.idx = 0;
    lbl_1_bss_3B8.pos = *arg0;
}

void fn_1_5014(s32 arg0)
{
    s32 i;
    s16 model;

    for (i = 0; i < 4; i++) {
        model = M548.models[i + 15].id;
        if (arg0 != 0) {
            Hu3DModelAttrReset(model, 0x400000);
        } else {
            Hu3DModelAttrSet(model, 0x400000);
        }
    }
}

void fn_1_5098(void)
{
    f32 time;
    f32 max;
    s16 model;
    s32 i;

    switch (lbl_1_bss_3B8.state) {
    case 0:
        return;
    case 1:
        model = M548.models[15].id;
        Hu3DModelPosSet(model, lbl_1_bss_3B8.pos.x, lbl_1_bss_3B8.pos.y, lbl_1_bss_3B8.pos.z);
        Hu3DModelScaleSet(model, 1.5f, 1.5f, 1.0f);
        Hu3DModelAttrReset(model, 1);
        Hu3DMotionTimeSet(model, 0.0f);
        lbl_1_bss_3B8.idx = 0;
        lbl_1_bss_3B8.state++;
        break;
    case 2:
        model = M548.models[lbl_1_bss_3B8.idx % 4 + 16].id;
        Hu3DModelPosSet(model, lbl_1_bss_3B8.pos.x + lbl_1_data_4C8[lbl_1_bss_3B8.idx % 4],
                        lbl_1_bss_3B8.pos.y + lbl_1_data_4D8[lbl_1_bss_3B8.idx % 4],
                        lbl_1_bss_3B8.pos.z + lbl_1_data_4E8[lbl_1_bss_3B8.idx % 4]);
        Hu3DModelScaleSet(model, 1.25f, 1.25f, 1.0f);
        Hu3DModelAttrReset(model, 1);
        Hu3DMotionTimeSet(model, 0.0f);
        Hu3DMotionSpeedSet(model, lbl_1_data_4F8[lbl_1_bss_3B8.idx % 4]);
        lbl_1_bss_3B8.timer = 0;
        lbl_1_bss_3B8.state++;
        break;
    case 3:
        if (++lbl_1_bss_3B8.timer >= 6) {
            lbl_1_bss_3B8.idx++;
            if (lbl_1_bss_3B8.idx == 4) {
                lbl_1_bss_3B8.state++;
            } else {
                lbl_1_bss_3B8.state--;
            }
        }
        break;
    case 4:
        time = Hu3DMotionTimeGet(M548.models[19].id);
        max = Hu3DMotionMaxTimeGet(M548.models[19].id);
        if (!(time < max)) {
            lbl_1_bss_3B8.state = 0;
        }
        break;
    }
    for (i = 0; i < 4; i++) {
        model = M548.models[i + 15].id;
        time = Hu3DMotionTimeGet(model);
        max = Hu3DMotionMaxTimeGet(model);
        if (!(time < max)) {
            Hu3DModelAttrSet(model, 1);
        }
    }
}

void fn_1_54C0(void)
{
    s32 tbl1[3] = { 1, 3, 0 };
    s32 tbl2[4] = { 1, 3, 4, 7 };

    switch (M548.stateA4) {
    case 0:
        M548.stateA4 = tbl2[rand8() % 4];
        HuAudFXPlay(0x840);
        M548.timerB0 = 0;
        break;
    case 1:
        M548.stateA4++;
        M548.timerB0 = -0x1E;
        break;
    case 2:
        if (M548.timerB0++ > 0x1E) {
            M548.stateA4 = 0;
        }
        break;
    case 3:
        omVibrate(M548.chars[lbl_1_bss_3D0.unk6].playerNo, 0x14, 0xC, 0);
        M548.timerB0 = -0x1E;
        M548.stateA4 = 2;
        break;
    case 4:
        memcpy(&lbl_1_bss_58[0], &lbl_1_bss_58[lbl_1_bss_3D0.unk6 + 1], sizeof(OMCAMERAVIEW));
        if (lbl_1_bss_3D0.unk6 != 0) {
            lbl_1_bss_58[0].center.x = lbl_1_bss_384[lbl_1_bss_3D0.unk6].unk10;
        } else {
            lbl_1_bss_58[0].center.x = -lbl_1_bss_384[lbl_1_bss_3D0.unk6].unk10;
        }
        lbl_1_bss_58[0].zoom = 250.0f;
        lbl_1_bss_58[0].rot.y = 0.0f;
        if (M548.chars[lbl_1_bss_3D0.unk6].charNo == 8) {
            lbl_1_bss_58[0].zoom = 400.0f;
        }
        omCameraViewMoveSimple(&lbl_1_bss_58[0], 0xF);
        M548.stateA4++;
        break;
    case 5:
        if (omCameraViewCheck(1)) {
            if (M548.timerB0++ > 0x14) {
                memcpy(&lbl_1_bss_58[0], &lbl_1_bss_58[lbl_1_bss_3D0.unk6 + 1], sizeof(OMCAMERAVIEW));
                omCameraViewMoveSimple(&lbl_1_bss_58[0], 0xF);
                M548.stateA4++;
            }
        }
        break;
    case 6:
        if (omCameraViewCheck(1)) {
            M548.stateA4 = 0;
        }
        break;
    case 7:
        Hu3DTexScrollPosMoveSet(lbl_1_bss_3B4, 0.0f, -0.0078125f, 0.0f);
        M548.stateA4++;
        M548.timerB0 = 0;
        break;
    case 8:
        if (M548.timerB0++ > 0xE) {
            Hu3DTexScrollPosSet(lbl_1_bss_3B4, lbl_1_data_39C[1].x, 0.125f + lbl_1_data_39C[1].y, 0.0f);
            Hu3DTexScrollPosMoveSet(lbl_1_bss_3B4, 0.0f, -0.0078125f, 0.0f);
            M548.stateA4++;
            M548.timerB0 = 0;
        }
        break;
    case 9:
        if (M548.timerB0++ > 0xE) {
            Hu3DTexScrollPosSet(lbl_1_bss_3B4, lbl_1_data_39C[1].x, lbl_1_data_39C[1].y, 0.0f);
            M548.timerB0 = 0;
            M548.stateA4 = 2;
        }
        break;
    }
    if (M548.stateA4 == 0) {
        M548.stateA0 = tbl1[lbl_1_bss_3D0.unk6];
        M548.timerB0 = 0;
    }
}

void fn_1_59FC(void)
{
    s32 tbl[2] = { 1, 3 };
    s32 zero = 0;

    switch (M548.stateA4) {
    case 0:
        M548.stateA4++;
        lbl_1_bss_5D2[1] = lbl_1_bss_3D0.unk8 * 0x10;
        lbl_1_bss_5D2[0] = -1;
        if (lbl_1_bss_3D0.unk4 == 6) {
            if (lbl_1_bss_3D0.unk8 == 1) {
                lbl_1_bss_5D2[0] = 8;
            } else {
                lbl_1_bss_5D2[0] = 0x18;
            }
        }
        if (lbl_1_bss_3D0.unk4 == 5 && lbl_1_bss_3D0.unk8 == 2) {
            lbl_1_bss_5D2[0] = 8;
        }
        if (lbl_1_bss_3D0.unk4 == 3) {
            lbl_1_bss_5D2[1] = 0x10;
        }
        if (lbl_1_bss_3D0.unk2 != 0) {
            lbl_1_bss_5D2[0] = 8;
            lbl_1_bss_5D2[1] = 0x10;
        }
        lbl_1_bss_5D8 = -0.015625f;
        Hu3DTexScrollPosMoveSet(lbl_1_bss_3B4, 0.0f, lbl_1_bss_5D8, 0.0f);
        return;
    case 1:
        lbl_1_bss_3D0.unkA++;
        if (lbl_1_bss_3D0.unkA == lbl_1_bss_5D2[0]) {
            if (lbl_1_bss_3D0.unk2 != 0) {
                Hu3DTexScrollPosSet(lbl_1_bss_3B4, lbl_1_data_39C[1].x, 0.125f + lbl_1_data_39C[1].y, 0.0f);
            } else {
                Hu3DTexScrollPosSet(lbl_1_bss_3B4, lbl_1_data_39C[3].x, 0.125f + lbl_1_data_39C[3].y, 0.0f);
            }
            Hu3DTexScrollPosMoveSet(lbl_1_bss_3B4, 0.0f, lbl_1_bss_5D8, 0.0f);
        }
        if (lbl_1_bss_3D0.unkA == lbl_1_bss_5D2[1]) {
            Hu3DTexScrollPosMoveSet(lbl_1_bss_3B4, 0.0f, 0.0f, 0.0f);
            M548.stateA0 = tbl[lbl_1_bss_3D0.unk6];
            M548.stateA4 = 0;
            lbl_1_bss_3D0.unkA = 0;
            lbl_1_bss_3D0.unk8 = 0;
        }
        break;
    }
}

void fn_1_5D84(void)
{
    s32 v[2] = { 1, 3 };
    s32 ret = fn_1_196C();

    if (ret == 0) {
        M548.stateA0 = v[lbl_1_bss_3D0.unk6];
    }
}

void fn_1_5DF4(void)
{
    s32 other;
    s32 playerNo;

    other = lbl_1_bss_3D0.unk6 ^ 1;
    switch (M548.stateA4) {
    case 0:
        lbl_1_bss_58[0].zoom = 400.0f;
        lbl_1_bss_58[0].center.x = other * 300 - 150;
        lbl_1_bss_58[0].rot.x = -25.0f;
        lbl_1_bss_58[0].rot.y = 0.0f;
        lbl_1_bss_58[0].center.y = 200.0f;
        omCameraViewMoveSimple(&lbl_1_bss_58[0], 0x1E);
        M548.stateA4++;
        return;
    case 1:
        if (omCameraViewCheck(1)) {
            M548.slots[other].motion = 3;
            M548.slots[other].id = 3;
            M548.slots[lbl_1_bss_3D0.unk6].attrSet = 0;
            M548.slots[lbl_1_bss_3D0.unk6].attrCur = 0;
            Hu3DMotionShiftSet(M548.models[other + 12].id, M548.motionIds[other][3], 0.0f, 10.0f, 0);
            lbl_1_bss_5D0 = GameMesCreate(4, 3, M548.chars[other].charNo, -1, -1, -1);
            M548.stateA4++;
            HuAudJinglePlay(0x79);
            playerNo = M548.chars[other].playerNo;
            if (_CheckFlag(0x1000F) == 0) {
                GwPlayer[playerNo].mgCoinBonus = 10;
            }
            lbl_1_bss_5D2[0] = 0;
            return;
        }
        break;
    case 2:
        lbl_1_bss_5D2[0]++;
        if (GameMesStatGet(lbl_1_bss_5D0) == 0 && lbl_1_bss_5D2[0] >= 0xD0) {
            GameMesKill(lbl_1_bss_5D0);
            MgSeqModeNext();
        }
        break;
    }
}
