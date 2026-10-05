#include "types.h"

typedef struct {
    u8 pad0[0xA0];
    s32 unkA0;
    s32 unkA4;
} GAME_CAM;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} GAME_VEC;

typedef struct {
    s32 x;
    s32 y;
} GAME_VEC2;

typedef struct {
    GAME_VEC unk0;
    GAME_VEC unkC;
} GAME_CAM3;

typedef struct {
    u8 pad0[6];
    s16 unk6;
} GAME_PLAYER;

extern GAME_CAM lbl_1_bss_3F0;
extern u8 lbl_1_bss_58[];
extern s32 lbl_1_bss_54[];
extern GAME_CAM3 lbl_1_bss_3B8;
extern GAME_PLAYER lbl_1_bss_3D0;

s32 fn_1_196C(void);

const GAME_VEC lbl_1_rodata_1C8 = {0, 0, 0};
const GAME_VEC2 lbl_1_rodata_208 = {1, 3};

s32 omCameraViewCheck(s32 arg0);
void omCameraViewMoveSimple(void *arg0, s32 arg1);
void MgSeqModeNext(void);
s32 MgTimerCreate(s32 arg0);
void MgTimerParamSet(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void MgTimerPosSet(s32 arg0, f32 arg1, f32 arg2);

void fn_1_282C(void)
{
    if (omCameraViewCheck(1)) {
        lbl_1_bss_3F0.unkA0++;
        lbl_1_bss_3F0.unkA4 = 0;
        omCameraViewMoveSimple(&lbl_1_bss_58[0x1C], 0x3C);
        MgSeqModeNext();
    }
}

void fn_1_2894(void)
{
    lbl_1_bss_54[0] = MgTimerCreate(0);
    MgTimerParamSet(lbl_1_bss_54[0], 300, 0, 0);
    MgTimerPosSet(lbl_1_bss_54[0], 288.0f, 64.0f);
}

void fn_1_4F44(void)
{
    GAME_VEC v = lbl_1_rodata_1C8;
    GAME_VEC *p = &v;

    lbl_1_bss_3B8.unkC = *p;
    lbl_1_bss_3B8.unk0.x = lbl_1_bss_3B8.unk0.y = lbl_1_bss_3B8.unk0.z = 0;
}

void fn_1_4FC4(GAME_VEC *arg0)
{
    lbl_1_bss_3B8.unk0.x = 1;
    lbl_1_bss_3B8.unk0.y = lbl_1_bss_3B8.unk0.z = 0;
    lbl_1_bss_3B8.unkC = *arg0;
}

void fn_1_5D84(void)
{
    s32 v[2] = {1, 3};
    s32 ret = fn_1_196C();

    if (ret == 0) {
        lbl_1_bss_3F0.unkA0 = v[lbl_1_bss_3D0.unk6];
    }
}
