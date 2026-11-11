#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mgdata.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/sprite.h"
#include "game/thpmain.h"
#include "game/window.h"
#include <math.h>

extern void fn_1_19C(s32 dirNum);
extern s16 fn_1_1280(s16 sndId, s16 modelId, s16 camBit);

extern s32 lbl_1_bss_C;
extern s32 lbl_1_bss_10;
extern s32 lbl_1_bss_14[10];
extern s16 lbl_1_bss_3C;
extern s16 lbl_1_bss_40[2];

HU3DPARMANPARAM lbl_1_data_38 = {
    30, 0, 50.0f, 10.0f, 180.0f,
    { 0.0f, -0.0500000007f, 0.0f },
    20.0f, 0.800000012f, 10.0f, 1.0f,
    2,
    { { 0xFF, 0xFF, 0xFF, 0xFF }, { 0xFF, 0xFF, 0x40, 0xFF }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 } },
    { { 0xFF, 0x80, 0x80, 0x00 }, { 0xFF, 0x40, 0x20, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 } },
};
HU3DPARMANPARAM lbl_1_data_88 = {
    40, 0, 50.0f, 0.0f, 90.0f,
    { 0.0f, -0.0500000007f, 0.0f },
    35.0f, 0.879999995f, 25.0f, 0.99000001f,
    1,
    { { 0xFF, 0xFF, 0xFF, 0xFF }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 } },
    { { 0xFF, 0x80, 0x80, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 } },
};
HU3DPARMANPARAM lbl_1_data_D8 = {
    30, 0, 30.0f, 60.0f, 180.0f,
    { 0.0f, 0.0f, 0.0f },
    0.200000003f, 1.0f, 100.0f, 1.0f,
    2,
    { { 0xFF, 0xFF, 0xFF, 0x20 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 } },
    { { 0x40, 0x20, 0x20, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 } },
};
HU3DPARMANPARAM lbl_1_data_128 = {
    50, 0, 2.0f, 30.0f, 180.0f,
    { 0.0f, -0.200000003f, 0.0f },
    1.0f, 0.800000012f, 30.0f, 0.949999988f,
    3,
    { { 0x80, 0xC0, 0xFF, 0xFF }, { 0xFF, 0xFF, 0x40, 0xFF }, { 0xFF, 0x60, 0x80, 0xFF }, { 0x00, 0x00, 0x00, 0x00 } },
    { { 0x80, 0xC0, 0xFF, 0x00 }, { 0xFF, 0x40, 0x20, 0x00 }, { 0x80, 0x10, 0x10, 0x00 }, { 0x00, 0x00, 0x00, 0x00 } },
};

HuVecF lbl_1_data_178 = { 288.0f, 240.0f, 1500.0f };
f32 lbl_1_data_184[10] = { 0.0f, 0.0f, 46.87f, -10.54f, -10.89f, 46.87f, 53.7f, -30.43f, 21.96f, -24.16f };
char lbl_1_data_1AC[] = "S3TC_mg_movie";
char lbl_1_data_1BA[] = "itemhook_R";
char lbl_1_data_1C5[] = "ske_head";
char lbl_1_data_1CE[] = "root_motion";
char lbl_1_data_1DA[] = "itemhook_M";
char lbl_1_data_1E5[] = "itemhook_head";
s32 lbl_1_data_1F4[54] = {
    0xC0000, 0xC0003, 0xC0004, 0xC0005, 0xC0006, 0xC001D, 0xC001E, 0xC001F, 0xC0020, 0xC0021, 0xC0022, 0xC0023, 0xC0025, 0xC0008,
    0xC0009, 0xC000B, 0xC000C, 0xC000E, 0xC000F, 0xC0010, 0xC0011, 0xC0012, 0xC0013, 0xC0014, 0xC0015, 0xC0016, 0xC0017, 0xC0018,
    0xC0019, 0xC001A, 0xC001B, 0xC001C, 0xC0002, 0xC0001, 0xC0007, 0xC000A, 0xC000D, 0xC0024, 0xC0026, 0xC0027, 0xC0028, 0xC0029,
    0xC002A, 0xC002B, 0xC002C, 0xC002D, 0xC002E, 0xC002F, 0xC0030, 0xC0031, 0xC0032, 0xC0033, 0xC0034, 0xC0035,
};
s32 lbl_1_data_2CC[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

typedef struct StoryWinWork_s {
    s32 closeF;
    s16 mesSpeed;
    u32 insertMes[8];
    s32 animNum;
    s32 winId;
    ANIMDATA *anim;
    s16 animWinId;
    s32 unk38;
} StoryWinWork;

s32 *lbl_1_bss_58[2];
StoryWinWork lbl_1_bss_60;
u8 lbl_1_bss_9C[12];
f32 lbl_1_bss_A8[4];
s32 lbl_1_bss_B8;
s16 lbl_1_bss_BC[2];
s16 lbl_1_bss_C0[3];
s16 lbl_1_bss_C6[5];
s16 lbl_1_bss_D0[9];
s16 lbl_1_bss_E2;
s32 lbl_1_bss_E4;
s32 lbl_1_bss_E8;
f32 lbl_1_bss_EC;
f32 lbl_1_bss_F0;
s16 lbl_1_bss_F4[16];
s16 lbl_1_bss_114;
s32 lbl_1_bss_118;
s16 lbl_1_bss_11C[2];
s16 lbl_1_bss_120;
s32 lbl_1_bss_124;
s16 lbl_1_bss_128[3];
s16 lbl_1_bss_12E;
s32 lbl_1_bss_130;
s32 lbl_1_bss_134;
s16 lbl_1_bss_138[4];
s16 lbl_1_bss_140;
f32 lbl_1_bss_144[4];
f32 lbl_1_bss_154[2];
f32 lbl_1_bss_15C[4];
s32 lbl_1_bss_16C;
s32 lbl_1_bss_170;
s32 lbl_1_bss_174;
s32 lbl_1_bss_178;
s32 lbl_1_bss_17C;
s32 lbl_1_bss_180;
s32 lbl_1_bss_184;
s32 lbl_1_bss_188;
f32 lbl_1_bss_18C[9];
f32 lbl_1_bss_1B0[3];
s32 lbl_1_bss_1BC[3];

void fn_1_13E8(void);
s32 fn_1_2448(void);
static void fn_1_66C4(s32 charNo, s32 motIdx);
static void fn_1_6924(HuVecF *pos2d);
static void fn_1_69CC(s32 motIdx);
static void fn_1_6AAC(void);
static void fn_1_6B38(void);
static void fn_1_6CBC(f32 *pos2d);
static void fn_1_6D00(s32 motIdx);
static void fn_1_6D90(void);
static void fn_1_6E0C(void);
static void fn_1_6F5C(HuVecF *pos2d);
static void fn_1_6FF8(s32 motIdx);
static void fn_1_7088(void);
static void fn_1_70F4(s32 motIdx);
static void fn_1_7408(f32 *pos2d);
static void fn_1_744C(s32 motIdx);
static void fn_1_75E0(void);
static void fn_1_7660(void);
static void fn_1_7BE4(void);
static void fn_1_7DE0(f32 *pos2d);
static void fn_1_7E24(s32 motIdx);
static void fn_1_7F98(void);
static void fn_1_8018(void);
static s32 fn_1_8150(s32 arg);
static s32 fn_1_8B34(void);
static s32 fn_1_914C(void);
static s32 fn_1_96CC(s32 arg);
static void fn_1_98D0(s32 msgIdx);
void fn_1_9C00(void);
static void fn_1_9CCC(s32 keep);
static void fn_1_9D84(s32 msgIdx);
static s32 fn_1_9EE8(s32 msgIdx);
static void fn_1_A294(s32 index);
static s32 *fn_1_A2B8(ANIMDATA *anim, s16 layer, s16 camera);
static void fn_1_A4F4(s32 *ids, HuVecF *pos);
static void fn_1_A5D0(s32 *ids);
static s32 *fn_1_A61C(ANIMDATA *anim, s16 layer, s16 camera);
static void fn_1_A6F8(s32 *ids, HuVecF *pos);
static void fn_1_A764(s32 *ids);

static void fn_1_66C4(s32 charNo, s32 motIdx)
{
    s16 ovl;

    Hu3DZClearLayerSet(15);
    lbl_1_bss_140 = Hu3DModelCreateData(charNo * 5 + 0xC10009);
    lbl_1_bss_138[0] = Hu3DJointMotionData(lbl_1_bss_140, charNo * 5 + 0xC1000A);
    lbl_1_bss_138[1] = Hu3DJointMotionData(lbl_1_bss_140, charNo * 5 + 0xC1000B);
    lbl_1_bss_138[2] = Hu3DJointMotionData(lbl_1_bss_140, charNo * 5 + 0xC1000C);
    lbl_1_bss_138[3] = Hu3DJointMotionData(lbl_1_bss_140, charNo * 5 + 0xC1000D);
    Hu3DMotionSet(lbl_1_bss_140, lbl_1_bss_138[motIdx - 1]);
    Hu3DModelCameraSet(lbl_1_bss_140, 2);
    Hu3DModelRotSet(lbl_1_bss_140, 0.0f, -60.0f, 0.0f);
    Hu3DModelLayerSet(lbl_1_bss_140, 6);
    if (motIdx != 4) {
        Hu3DModelAttrSet(lbl_1_bss_140, HU3D_MOTATTR_LOOP);
    }
    ovl = Hu3DJointMotionData(lbl_1_bss_140, charNo + 0xC10062);
    Hu3DMotionOverlaySet(lbl_1_bss_140, ovl);
    Hu3DModelAttrSet(lbl_1_bss_140, HU3D_MOTATTR_OVL_LOOP);
    lbl_1_bss_134 = charNo;
    lbl_1_bss_130 = motIdx;
}

static void fn_1_6924(HuVecF *pos2d)
{
    HuVecF pos = *pos2d;
    HuVecF out;
    f32 height;

    height = CharModelHeightGet(lbl_1_bss_134);
    pos.y += 0.5f * height;
    Hu3D2Dto3D(&pos, 2, &out);
    Hu3DModelPosSetV(lbl_1_bss_140, &out);
}

static void fn_1_69CC(s32 motIdx)
{
    if (motIdx != lbl_1_bss_130) {
        if (motIdx != 4) {
            Hu3DMotionShiftSet(lbl_1_bss_140, lbl_1_bss_138[motIdx - 1], 0.0f, 20.0f, HU3D_MOTATTR_LOOP);
        } else {
            Hu3DMotionShiftSet(lbl_1_bss_140, lbl_1_bss_138[motIdx - 1], 0.0f, 30.0f, 0);
        }
    }
    lbl_1_bss_130 = motIdx;
}

static void fn_1_6AAC(void)
{
    Hu3DMotionSet(lbl_1_bss_140, -1);
    Hu3DMotionKill(lbl_1_bss_138[0]);
    Hu3DMotionKill(lbl_1_bss_138[1]);
    Hu3DMotionKill(lbl_1_bss_138[2]);
    Hu3DMotionKill(lbl_1_bss_138[3]);
    Hu3DModelKill(lbl_1_bss_140);
    Hu3DLayerHookSet(14, NULL);
}

static void fn_1_6B38(void)
{
    Hu3DZClearLayerSet(15);
    lbl_1_bss_12E = Hu3DModelCreateData(0xC10057);
    lbl_1_bss_128[0] = Hu3DJointMotionData(lbl_1_bss_12E, 0xC10058);
    lbl_1_bss_128[1] = Hu3DJointMotionData(lbl_1_bss_12E, 0xC10059);
    lbl_1_bss_128[2] = Hu3DJointMotionData(lbl_1_bss_12E, 0xC1005A);
    Hu3DMotionSet(lbl_1_bss_12E, lbl_1_bss_128[0]);
    Hu3DModelCameraSet(lbl_1_bss_12E, 4);
    Hu3DModelRotSet(lbl_1_bss_12E, 0.0f, 30.0f, 0.0f);
    Hu3DModelLayerSet(lbl_1_bss_12E, 7);
    Hu3DModelAttrSet(lbl_1_bss_12E, HU3D_MOTATTR_LOOP);
    lbl_1_bss_124 = 1;
}

static void fn_1_6CBC(f32 *pos2d)
{
    f32 pos3d[3];
    Hu3D2Dto3D((HuVecF *)pos2d, 4, (HuVecF *)pos3d);
    Hu3DModelPosSetV(lbl_1_bss_12E, (HuVecF *)pos3d);
}

static void fn_1_6D00(s32 motIdx)
{
    if (motIdx != lbl_1_bss_124) {
        Hu3DMotionShiftSet(lbl_1_bss_12E, lbl_1_bss_128[motIdx - 1], 0.0f, 20.0f, HU3D_MOTATTR_LOOP);
    }
    lbl_1_bss_124 = motIdx;
}

static void fn_1_6D90(void)
{
    Hu3DMotionSet(lbl_1_bss_12E, -1);
    Hu3DMotionKill(lbl_1_bss_128[0]);
    Hu3DMotionKill(lbl_1_bss_128[1]);
    Hu3DMotionKill(lbl_1_bss_128[2]);
    Hu3DModelKill(lbl_1_bss_12E);
    Hu3DLayerHookSet(15, NULL);
}

static void fn_1_6E0C(void)
{
    Hu3DZClearLayerSet(15);
    lbl_1_bss_120 = Hu3DModelCreateData(0xC1002C);
    lbl_1_bss_11C[0] = Hu3DJointMotionData(lbl_1_bss_120, 0xC1002D);
    lbl_1_bss_11C[1] = Hu3DJointMotionData(lbl_1_bss_120, 0xC1002E);
    Hu3DMotionSet(lbl_1_bss_120, lbl_1_bss_11C[0]);
    Hu3DModelCameraSet(lbl_1_bss_120, 2);
    Hu3DModelRotSet(lbl_1_bss_120, 0.0f, -60.0f, 0.0f);
    Hu3DModelLayerSet(lbl_1_bss_120, 5);
    Hu3DModelAttrSet(lbl_1_bss_120, HU3D_MOTATTR_LOOP);
    lbl_1_bss_118 = 1;
}

static void fn_1_6F5C(HuVecF *pos2d)
{
    HuVecF pos = *pos2d;
    HuVecF out;
    f32 height;

    height = CharModelHeightGet(7);
    pos.y += 0.5f * height;
    Hu3D2Dto3D(&pos, 2, &out);
    Hu3DModelPosSetV(lbl_1_bss_120, &out);
}

static void fn_1_6FF8(s32 motIdx)
{
    if (motIdx != lbl_1_bss_118) {
        Hu3DMotionShiftSet(lbl_1_bss_120, lbl_1_bss_11C[motIdx - 1], 0.0f, 20.0f, HU3D_MOTATTR_LOOP);
    }
    lbl_1_bss_118 = motIdx;
}

static void fn_1_7088(void)
{
    Hu3DMotionSet(lbl_1_bss_120, -1);
    Hu3DMotionKill(lbl_1_bss_11C[0]);
    Hu3DMotionKill(lbl_1_bss_11C[1]);
    Hu3DModelKill(lbl_1_bss_120);
    Hu3DLayerHookSet(13, NULL);
}

static void fn_1_70F4(s32 motIdx)
{
    s32 bank;
    s32 i;
    f32 rotY;

    Hu3DZClearLayerSet(15);
    if (motIdx < 8) {
        lbl_1_bss_114 = Hu3DModelCreateData(0xC10045);
        bank = 0;
    } else {
        lbl_1_bss_114 = Hu3DModelCreateData(0xC1004D);
        bank = 1;
    }
    for (i = 0; i < 16; i++) {
        lbl_1_bss_F4[i] = Hu3DJointMotionData(lbl_1_bss_114, bank + 0xC10000 + i + 0x46);
    }
    Hu3DMotionSet(lbl_1_bss_114, lbl_1_bss_F4[motIdx - 1]);
    Hu3DModelCameraSet(lbl_1_bss_114, 2);
    if (motIdx == 8) {
        rotY = 0.0f;
    } else {
        rotY = 60.0f;
    }
    Hu3DModelRotSet(lbl_1_bss_114, 0.0f, rotY, 0.0f);
    Hu3DModelLayerSet(lbl_1_bss_114, 7);
    if (motIdx == 1 || motIdx == 5 || motIdx == 6 || motIdx == 8 || motIdx == 10 || motIdx == 12 || motIdx == 14 || motIdx == 15
        || motIdx == 16) {
        Hu3DModelAttrSet(lbl_1_bss_114, HU3D_MOTATTR_LOOP);
    } else if (motIdx == 13) {
        Hu3DModelAttrSet(lbl_1_bss_114, HU3D_MOTATTR_LOOP);
        Hu3DMotionAttrSet(lbl_1_bss_F4[motIdx - 1], 1);
    }
    if (motIdx == 8) {
        Hu3DModelAmbSet(lbl_1_bss_114, 0.0f, 0.0f, 0.0f);
        lbl_1_bss_BC[0] = Hu3DModelCreateData(0xC1005C);
        Hu3DModelAttrSet(lbl_1_bss_BC[0], 0x40000002);
        Hu3DModelCameraSet(lbl_1_bss_BC[0], 2);
        Hu3DModelLayerSet(lbl_1_bss_BC[0], 7);
        Hu3DModelHookSet(lbl_1_bss_114, lbl_1_data_1DA, lbl_1_bss_BC[0]);
    }
    lbl_1_bss_E8 = motIdx;
}

static void fn_1_7408(f32 *pos2d)
{
    f32 pos3d[3];
    Hu3D2Dto3D((HuVecF *)pos2d, 2, (HuVecF *)pos3d);
    Hu3DModelPosSetV(lbl_1_bss_114, (HuVecF *)pos3d);
}

static void fn_1_744C(s32 motIdx)
{
    if (motIdx != lbl_1_bss_E8) {
        if (motIdx == 1 || motIdx == 5 || motIdx == 6 || motIdx == 8 || motIdx == 10 || motIdx == 12 || motIdx == 14 || motIdx == 15
            || motIdx == 16) {
            Hu3DMotionShiftSet(lbl_1_bss_114, lbl_1_bss_F4[motIdx - 1], 0.0f, 20.0f, HU3D_MOTATTR_LOOP);
        } else if (motIdx == 13) {
            Hu3DMotionShiftSet(lbl_1_bss_114, lbl_1_bss_F4[motIdx - 1], 0.0f, 20.0f, HU3D_MOTATTR_LOOP);
            Hu3DMotionAttrSet(lbl_1_bss_F4[motIdx - 1], 1);
        } else {
            Hu3DMotionShiftSet(lbl_1_bss_114, lbl_1_bss_F4[motIdx - 1], 0.0f, 20.0f, 0);
        }
    }
    lbl_1_bss_E8 = motIdx;
}

static void fn_1_75E0(void)
{
    s32 i;

    Hu3DMotionSet(lbl_1_bss_114, -1);
    for (i = 0; i < 16; i++) {
        Hu3DMotionKill(lbl_1_bss_F4[i]);
    }
    Hu3DModelKill(lbl_1_bss_114);
    Hu3DLayerHookSet(15, NULL);
}

static void fn_1_7660(void)
{
    if (Hu3DMotionEndCheck(lbl_1_bss_114)) {
        if (lbl_1_bss_E8 == 3 || lbl_1_bss_E8 == 4 || lbl_1_bss_E8 == 7) {
            if (lbl_1_bss_E8 != 1) {
                Hu3DMotionShiftSet(lbl_1_bss_114, lbl_1_bss_F4[0], 0.0f, 20.0f, HU3D_MOTATTR_LOOP);
            }
            lbl_1_bss_E8 = 1;
        } else if (lbl_1_bss_E8 == 9) {
            if (lbl_1_bss_E8 != 8) {
                Hu3DMotionShiftSet(lbl_1_bss_114, lbl_1_bss_F4[7], 0.0f, 20.0f, HU3D_MOTATTR_LOOP);
            }
            lbl_1_bss_E8 = 8;
        } else if (lbl_1_bss_E8 == 11) {
            if (lbl_1_bss_E8 != 12) {
                Hu3DMotionShiftSet(lbl_1_bss_114, lbl_1_bss_F4[11], 0.0f, 20.0f, HU3D_MOTATTR_LOOP);
            }
            lbl_1_bss_E8 = 12;
        }
    }
    if (lbl_1_bss_184 == 6 && HuWinStatGet(lbl_1_bss_60.winId) == 2) {
        if (lbl_1_bss_E8 != 16) {
            Hu3DMotionShiftSet(lbl_1_bss_114, lbl_1_bss_F4[15], 0.0f, 20.0f, HU3D_MOTATTR_LOOP);
        }
        lbl_1_bss_E8 = 16;
    }
    if (lbl_1_bss_184 == 0x130 && Hu3DMotionShiftIDGet(lbl_1_bss_114) == -1) {
        if (lbl_1_bss_E8 == 6 && (HuPadBtnDown[0] & (PAD_BUTTON_A | PAD_BUTTON_B)) != 0) {
            if (lbl_1_bss_E8 != 7) {
                Hu3DMotionShiftSet(lbl_1_bss_114, lbl_1_bss_F4[6], 0.0f, 20.0f, 0);
            }
            lbl_1_bss_E8 = 7;
        }
        if (lbl_1_bss_E8 == 1) {
            lbl_1_bss_1BC[0] = 0;
            lbl_1_bss_17C = 1;
            lbl_1_bss_184 = 0x133;
        }
    }
    if (lbl_1_bss_E8 == 11) {
        lbl_1_bss_E4++;
        if (lbl_1_bss_E4 == 20) {
            HuAudFXPanning(HuAudFXPlay(0x59B), 0x38);
            fn_1_7E24(7);
        }
        if (lbl_1_bss_E4 >= 20) {
            Hu3DModelObjPosGet(lbl_1_bss_114, lbl_1_data_1BA, &Hu3DData[lbl_1_bss_E2].pos);
            Hu3DData[lbl_1_bss_E2].pos.x += 5.0f;
            Hu3DData[lbl_1_bss_E2].pos.y += -85.0f;
            Hu3DData[lbl_1_bss_E2].pos.z += 57.0f;
        }
    } else if (lbl_1_bss_E8 == 12) {
        Hu3DModelObjPosGet(lbl_1_bss_114, lbl_1_data_1BA, &Hu3DData[lbl_1_bss_E2].pos);
        Hu3DData[lbl_1_bss_E2].pos.x += 5.0f;
        Hu3DData[lbl_1_bss_E2].pos.y += -85.0f;
        Hu3DData[lbl_1_bss_E2].pos.z += 57.0f;
    }
    HuPrcVSleep();
}

static void fn_1_7BE4(void)
{
    s32 i;

    Hu3DZClearLayerSet(15);
    lbl_1_bss_E2 = Hu3DModelCreateData(0xC10044);
    for (i = 0; i < 9; i++) {
        lbl_1_bss_D0[i] = Hu3DJointMotionData(lbl_1_bss_E2, i + 0xC1003B);
    }
    Hu3DMotionSet(lbl_1_bss_E2, lbl_1_bss_D0[0]);
    Hu3DModelCameraSet(lbl_1_bss_E2, 2);
    Hu3DModelRotSet(lbl_1_bss_E2, 0.0f, 0.0f, 0.0f);
    Hu3DModelLayerSet(lbl_1_bss_E2, 7);
    Hu3DModelAttrSet(lbl_1_bss_E2, HU3D_MOTATTR_LOOP);
    Hu3DModelAmbSet(lbl_1_bss_E2, 0.0f, 0.0f, 0.0f);
    lbl_1_bss_BC[1] = Hu3DModelCreateData(0xC1005D);
    Hu3DModelAttrSet(lbl_1_bss_BC[1], 0x40000002);
    Hu3DModelCameraSet(lbl_1_bss_BC[1], 2);
    Hu3DModelLayerSet(lbl_1_bss_BC[1], 7);
    Hu3DModelHookSet(lbl_1_bss_E2, lbl_1_data_1E5, lbl_1_bss_BC[1]);
    lbl_1_bss_B8 = 1;
}

static void fn_1_7DE0(f32 *pos2d)
{
    f32 pos3d[3];
    Hu3D2Dto3D((HuVecF *)pos2d, 2, (HuVecF *)pos3d);
    Hu3DModelPosSetV(lbl_1_bss_E2, (HuVecF *)pos3d);
}

static void fn_1_7E24(s32 motIdx)
{
    if (motIdx != lbl_1_bss_B8) {
        if (motIdx == 1 || motIdx == 2 || motIdx == 4 || motIdx == 5 || motIdx == 8) {
            Hu3DMotionShiftSet(lbl_1_bss_E2, lbl_1_bss_D0[motIdx - 1], 0.0f, 20.0f, HU3D_MOTATTR_LOOP);
        } else if (motIdx == 9) {
            Hu3DMotionShiftSet(lbl_1_bss_E2, lbl_1_bss_D0[motIdx - 1], 0.0f, 20.0f, HU3D_MOTATTR_LOOP);
            Hu3DMotionAttrSet(lbl_1_bss_D0[motIdx - 1], 1);
        } else {
            Hu3DMotionShiftSet(lbl_1_bss_E2, lbl_1_bss_D0[motIdx - 1], 0.0f, 20.0f, 0);
        }
    }
    lbl_1_bss_B8 = motIdx;
}

static void fn_1_7F98(void)
{
    s32 i;

    Hu3DMotionSet(lbl_1_bss_E2, -1);
    for (i = 0; i < 9; i++) {
        Hu3DMotionKill(lbl_1_bss_D0[i]);
    }
    Hu3DModelKill(lbl_1_bss_E2);
    Hu3DLayerHookSet(15, NULL);
}

static void fn_1_8018(void)
{
    if (Hu3DMotionEndCheck(lbl_1_bss_E2)) {
        if (lbl_1_bss_B8 == 3 || lbl_1_bss_B8 == 6) {
            if (lbl_1_bss_B8 != 4) {
                Hu3DMotionShiftSet(lbl_1_bss_E2, lbl_1_bss_D0[3], 0.0f, 20.0f, HU3D_MOTATTR_LOOP);
            }
            lbl_1_bss_B8 = 4;
        } else if (lbl_1_bss_B8 == 7) {
            if (lbl_1_bss_B8 != 8) {
                Hu3DMotionShiftSet(lbl_1_bss_E2, lbl_1_bss_D0[7], 0.0f, 20.0f, HU3D_MOTATTR_LOOP);
            }
            lbl_1_bss_B8 = 8;
        }
    }
    HuPrcVSleep();
}

// Remaining diffs: scratch-register choice and branch-B stack slot order.
static s32 fn_1_8150(s32 which)
{
    s32 mdl;
    s32 *parman0;
    s32 *parman1;
    s32 done;
    s32 mdlSrc;
    s32 charNo;

    done = 0;
    if (which == 0) {
        mdlSrc = lbl_1_bss_140;
    } else {
        mdlSrc = lbl_1_bss_120;
    }
    mdl = mdlSrc;
    switch (lbl_1_bss_178) {
        case 0:
            lbl_1_bss_15C[0] = 0.0f;
            lbl_1_bss_15C[1] = 0.0f;
            lbl_1_bss_15C[2] = 0.0f;
            lbl_1_bss_15C[3] = 1.0f;
            if (which == 0) {
                if (lbl_1_bss_130 != 2) {
                    Hu3DMotionShiftSet(lbl_1_bss_140, lbl_1_bss_138[1], 0.0f, 20.0f, HU3D_MOTATTR_LOOP);
                }
                lbl_1_bss_130 = 2;
            } else {
                if (lbl_1_bss_118 != 2) {
                    Hu3DMotionShiftSet(lbl_1_bss_120, lbl_1_bss_11C[1], 0.0f, 20.0f, HU3D_MOTATTR_LOOP);
                }
                lbl_1_bss_118 = 2;
            }
            lbl_1_bss_178++;
            parman0 = lbl_1_bss_58[0];
            Hu3DParManPosSet(*parman0, Hu3DData[mdl].pos.x, Hu3DData[mdl].pos.y, Hu3DData[mdl].pos.z);
            Hu3DParManTimeLimitSet(*parman0, 2);
            Hu3DParManAttrReset(*parman0, 1);
            fn_1_1280(0x580, mdl, 2);
            break;
        case 1: {
            HuVecF pos;
            f32 rotX;
            f32 rotY;
            f32 rotZ;

            pos = lbl_1_data_178;
            pos.x += 120.0f;
            pos.x = pos.x - 288.0 * cos(M_PI * (-90.0f + lbl_1_bss_15C[0]) / 180.0);
            if (which != 0) {
                charNo = 7;
            } else {
                charNo = lbl_1_bss_134;
            }
            pos.y = pos.y + (1.3f * (120.0f + lbl_1_data_184[charNo] * 0.5f)) * sin(M_PI * lbl_1_bss_15C[1] / 180.0);
            if (which == 0) {
                HuVecF pos3d;
                HuVecF tmp = pos;
                f32 height = CharModelHeightGet(lbl_1_bss_134);
                tmp.y += height * 0.5f;
                Hu3D2Dto3D(&tmp, 2, &pos3d);
                Hu3DModelPosSetV(lbl_1_bss_140, &pos3d);
            } else {
                HuVecF tmp = pos;
                HuVecF pos3d;
                f32 height = CharModelHeightGet(7);
                tmp.y += 0.5f * height;
                Hu3D2Dto3D(&tmp, 2, &pos3d);
                Hu3DModelPosSetV(lbl_1_bss_120, &pos3d);
            }
            lbl_1_bss_15C[0] += 1.6f;
            if (lbl_1_bss_15C[0] > 160.0f) {
                lbl_1_bss_15C[0] = 160.0f;
            }
            lbl_1_bss_15C[1] -= 1.6f;
            if (lbl_1_bss_15C[1] > 160.0f) {
                lbl_1_bss_15C[1] = 160.0f;
            }
            lbl_1_bss_15C[2] += 0.9f;
            if (lbl_1_bss_15C[2] > 90.0f) {
                lbl_1_bss_15C[2] = 90.0f;
            }
            rotX = (lbl_1_bss_15C[2] < 45.0f) ? 0.0f : (lbl_1_bss_15C[2] - 45.0f);
            rotY = -60.0 - 180.0 * sin(M_PI * lbl_1_bss_15C[2] / 180.0);
            rotZ = 0.0f;
            Hu3DModelRotSet(mdl, rotX, rotY, rotZ);
            lbl_1_bss_15C[3] -= 0.01f;
            if (lbl_1_bss_15C[3] < 0.0f) {
                lbl_1_bss_15C[3] = 0.0f;
            }
            Hu3DModelScaleSet(mdl, lbl_1_bss_15C[3], lbl_1_bss_15C[3], lbl_1_bss_15C[3]);
            if (lbl_1_bss_15C[3] == 0.0f) {
                if (lbl_1_bss_184 < 300 && which == 0) {
                    lbl_1_bss_178 = 2;
                } else {
                    lbl_1_bss_178 = 0;
                    done = 1;
                }
            }
            if (lbl_1_bss_15C[3] > 0.1f) {
                parman1 = lbl_1_bss_58[0];
                Hu3DParManPosSet(*parman1, Hu3DData[mdl].pos.x, Hu3DData[mdl].pos.y, Hu3DData[mdl].pos.z);
                Hu3DParManTimeLimitSet(*parman1, 2);
                Hu3DParManAttrReset(*parman1, 1);
            }
            if (lbl_1_bss_184 < 300) {
                Hu3DModelRotSet(lbl_1_bss_12E, 0.0f, 30.0 + 120.0 * sin(M_PI * lbl_1_bss_15C[2] / 180.0), 0.0f);
            }
            fn_1_1280(0x8580, mdl, 2);
            break;
        }
        case 2:
            lbl_1_bss_15C[2] -= 0.9f;
            if (lbl_1_bss_15C[2] < 0.0f) {
                lbl_1_bss_15C[2] = 0.0f;
                lbl_1_bss_178 = 0;
                done = 1;
            }
            Hu3DModelRotSet(lbl_1_bss_12E, 0.0f, 30.0 + 120.0 * (1.0 - cos(M_PI * lbl_1_bss_15C[2] / 180.0)), 0.0f);
            break;
    }
    return done;
}

static s32 fn_1_8B34(void)
{
    s16 mdl;
    s32 *parman0;
    s32 *parman1;
    s32 done;

    done = 0;
    mdl = lbl_1_bss_114;
    switch (lbl_1_bss_170) {
        case 0:
            lbl_1_bss_144[0] = 0.0f;
            lbl_1_bss_144[1] = 0.0f;
            lbl_1_bss_144[2] = 0.0f;
            lbl_1_bss_144[3] = 1.0f;
            if (lbl_1_bss_E8 != 2) {
                Hu3DMotionShiftSet(lbl_1_bss_114, lbl_1_bss_F4[1], 0.0f, 20.0f, 0);
            }
            lbl_1_bss_E8 = 2;
            lbl_1_bss_170++;
            parman0 = lbl_1_bss_58[1];
            Hu3DParManPosSet(*parman0, Hu3DData[mdl].pos.x, Hu3DData[mdl].pos.y, Hu3DData[mdl].pos.z);
            Hu3DParManTimeLimitSet(*parman0, 2);
            Hu3DParManAttrReset(*parman0, 1);
            fn_1_1280(0x580, mdl, 2);
            break;
        case 1: {
            HuVecF pos;
            HuVecF pos3d;
            f32 rotX;
            f32 rotY;
            f32 rotZ;

            pos = lbl_1_data_178;
            pos.x -= 140.0f;
            pos.x = pos.x + 336.0 * cos(M_PI * (-90.0f + lbl_1_bss_144[0]) / 180.0);
            pos.y = pos.y + 156.0 * sin(M_PI * lbl_1_bss_144[1] / 180.0);
            Hu3D2Dto3D(&pos, 2, &pos3d);
            Hu3DModelPosSetV(lbl_1_bss_114, &pos3d);
            lbl_1_bss_144[0] += 1.6f;
            if (lbl_1_bss_144[0] > 160.0f) {
                lbl_1_bss_144[0] = 160.0f;
            }
            lbl_1_bss_144[1] -= 1.85f;
            if (lbl_1_bss_144[1] > 185.0f) {
                lbl_1_bss_144[1] = 185.0f;
            }
            lbl_1_bss_144[2] += 0.9f;
            if (lbl_1_bss_144[2] > 90.0f) {
                lbl_1_bss_144[2] = 90.0f;
            }
            rotX = 0.0f;
            rotY = 60.0 + 180.0 * sin(M_PI * lbl_1_bss_144[2] / 180.0);
            rotZ = 0.0f;
            Hu3DModelRotSet(mdl, rotX, rotY, rotZ);
            lbl_1_bss_144[3] -= 0.01f;
            if (lbl_1_bss_144[3] < 0.0f) {
                lbl_1_bss_144[3] = 0.0f;
            }
            Hu3DModelScaleSet(mdl, lbl_1_bss_144[3], lbl_1_bss_144[3], lbl_1_bss_144[3]);
            if (lbl_1_bss_144[3] == 0.0f) {
                lbl_1_bss_170 = 0;
                done = 1;
            }
            if (lbl_1_bss_144[3] > 0.1f) {
                parman1 = lbl_1_bss_58[1];
                Hu3DParManPosSet(*parman1, Hu3DData[mdl].pos.x, Hu3DData[mdl].pos.y, Hu3DData[mdl].pos.z);
                Hu3DParManTimeLimitSet(*parman1, 2);
                Hu3DParManAttrReset(*parman1, 1);
            }
            fn_1_1280(0x8580, mdl, 2);
            break;
        }
    }
    return done;
}

static s32 fn_1_914C(void)
{
    s32 done;
    s32 id;
    s16 mdl;
    f32 t;

    done = 0;
    mdl = lbl_1_bss_114;
    switch (lbl_1_bss_178) {
        case 0:
            lbl_1_bss_15C[0] = 0.0f;
            lbl_1_bss_15C[1] = 0.0f;
            lbl_1_bss_178++;
            break;
        case 1:
            lbl_1_bss_15C[1] += 0.9230769f;
            if (lbl_1_bss_15C[1] > 60.0f) {
                lbl_1_bss_15C[1] = 60.0f;
            }
            Hu3DModelRotSet(mdl, 0.0f, 60.0 + 180.0 * sin(M_PI * lbl_1_bss_15C[1] / 180.0), 0.0f);
            if (lbl_1_bss_15C[1] == 60.0f) {
                lbl_1_bss_15C[0] = 0.0f;
                lbl_1_bss_15C[1] = 0.0f;
                lbl_1_bss_15C[2] = 0.0f;
                lbl_1_bss_15C[3] = 0.0f;
                lbl_1_bss_16C = 0;
                if (lbl_1_bss_E8 != 5) {
                    Hu3DMotionShiftSet(lbl_1_bss_114, lbl_1_bss_F4[4], 0.0f, 20.0f, HU3D_MOTATTR_LOOP);
                }
                lbl_1_bss_E8 = 5;
                lbl_1_bss_178++;
            }
            break;
        case 2: {
            HuVecF pos;
            HuVecF pos3d;

            if (lbl_1_bss_E8 == 5) {
                t = Hu3DMotionTimeGet(lbl_1_bss_114);
                id = (s16)((10.0f == t) ? 0x581 : (60.0f == t) ? 0x582 : (10.0f == lbl_1_bss_15C[3]) ? 0x581 : -1);
                if (id != -1) {
                    fn_1_1280(id, lbl_1_bss_114, 2);
                }
                lbl_1_bss_15C[3] += 1.0f;
            }
            lbl_1_bss_15C[0] += 0.003f;
            if (lbl_1_bss_15C[0] > 1.0f) {
                lbl_1_bss_15C[0] = 1.0f;
            }
            Hu3DModelScaleSet(lbl_1_bss_114, 1.0f - lbl_1_bss_15C[0], 1.0f - lbl_1_bss_15C[0], 1.0f - lbl_1_bss_15C[0]);
            lbl_1_bss_15C[1] += 0.7f;
            lbl_1_bss_15C[2] += 1.0f;
            pos = lbl_1_data_178;
            pos.x -= 140.0f;
            pos.x -= lbl_1_bss_15C[1];
            pos.y -= lbl_1_bss_15C[2];
            Hu3D2Dto3D(&pos, 2, &pos3d);
            Hu3DModelPosSetV(lbl_1_bss_114, &pos3d);
            if ((f32)(++lbl_1_bss_16C) >= 260.0f) {
                lbl_1_bss_178 = 0;
                done = 1;
            }
            break;
        }
    }
    return done;
}

static s32 fn_1_96CC(s32 which)
{
    s32 done;
    s16 mdl;
    HSFOBJECT *obj;
    s16 mdlSrc;

    done = 0;
    if (which == 0) {
        mdlSrc = lbl_1_bss_140;
    } else {
        mdlSrc = lbl_1_bss_120;
    }
    mdl = mdlSrc;
    if (lbl_1_bss_174 == 0) {
        obj = Hu3DModelObjPtrGet(mdl, lbl_1_data_1C5);
        lbl_1_bss_154[1] = obj->mesh.curr.rot.z;
        lbl_1_bss_154[0] = 0.0f;
        lbl_1_bss_174 = 1;
    } else if (lbl_1_bss_174 == 1) {
        lbl_1_bss_154[0] += 1.5f;
        if (lbl_1_bss_154[0] > 18.0f) {
            lbl_1_bss_154[0] = 18.0f;
            lbl_1_bss_174 = 2;
        }
    } else {
        lbl_1_bss_154[0] -= 1.5f;
        if (lbl_1_bss_154[0] < 0.0f) {
            lbl_1_bss_154[0] = 0.0f;
            lbl_1_bss_174 = 0;
            done = 1;
        }
    }
    Hu3DMotionForceSet(mdl, lbl_1_data_1C5, 0x200, lbl_1_bss_154[1] + lbl_1_bss_154[0]);
    return done;
}

static void fn_1_98D0(s32 msgIdx)
{
    s32 i;
    u32 mes;
    HUWIN *w;
    void *animData;
    f32 winY;
    HuVec2f size;

    if (lbl_1_bss_60.winId != -1) {
        HuWinExKill(lbl_1_bss_60.winId);
    }
    if (lbl_1_bss_60.animWinId != -1) {
        HuWinSprKill(lbl_1_bss_60.winId, lbl_1_bss_60.animWinId);
        HuSprAnimKill(lbl_1_bss_60.anim);
    }
    for (i = 0; lbl_1_bss_60.insertMes[i] != 0xFFFFFFFF; i++) {
        HuWinInsertMesSizeGet(lbl_1_bss_60.insertMes[i], i);
    }
    mes = lbl_1_data_1F4[msgIdx];
    HuWinMesMaxSizeGet(1, &size, mes);
    winY = (msgIdx < 41) ? 340.0f : 360.0f;
    lbl_1_bss_60.winId = HuWinExCreateFrame(-10000.0f, winY, size.x, size.y, -1, 0);
    if (lbl_1_bss_60.animNum != -1) {
        animData = HuDataSelHeapReadNum(lbl_1_bss_60.animNum, HU_MEMNUM_OVL, HEAP_MODEL);
        lbl_1_bss_60.anim = HuSprAnimRead(animData);
        lbl_1_bss_60.animWinId = HuWinAnimSet(lbl_1_bss_60.winId, lbl_1_bss_60.anim, 0, 48.0f, 0.5f * size.y);
    }
    if (lbl_1_bss_60.closeF == 0) {
        HuWinExOpen(lbl_1_bss_60.winId);
    } else {
        HuWinDispOn(lbl_1_bss_60.winId);
    }
    for (i = 0; lbl_1_bss_60.insertMes[i] != 0xFFFFFFFF; i++) {
        HuWinInsertMesSet(lbl_1_bss_60.winId, lbl_1_bss_60.insertMes[i], i);
    }
    HuWinMesSpeedSet(lbl_1_bss_60.winId, lbl_1_bss_60.mesSpeed);
    HuWinMesSet(lbl_1_bss_60.winId, mes);
    w = &winData[lbl_1_bss_60.winId];
    w->padMask = 1;
    w->disablePlayer = 14;
}

void fn_1_9C00(void)
{
    if (lbl_1_bss_60.winId != -1) {
        HuWinMesWait(lbl_1_bss_60.winId);
        if (lbl_1_bss_60.closeF == 0) {
            HuWinExClose(lbl_1_bss_60.winId);
        }
        if (lbl_1_bss_60.animWinId != -1) {
            HuWinSprKill(lbl_1_bss_60.winId, lbl_1_bss_60.animWinId);
            HuSprAnimKill(lbl_1_bss_60.anim);
        }
        HuWinExKill(lbl_1_bss_60.winId);
        fn_1_9CCC(1);
    }
}

static void fn_1_9CCC(s32 keep)
{
    s32 i;

    for (i = 0; i < 8; i++) {
        lbl_1_bss_60.insertMes[i] = -1;
    }
    lbl_1_bss_60.winId = -1;
    lbl_1_bss_60.mesSpeed = 1;
    lbl_1_bss_60.closeF = 0;
    lbl_1_bss_60.animNum = lbl_1_bss_60.animWinId = -1;
    lbl_1_bss_60.anim = NULL;
    if (keep == 0) {
        lbl_1_bss_60.unk38 = -1;
    }
}

static void fn_1_9D84(s32 msgIdx)
{
    s32 i;

    fn_1_98D0(msgIdx);
    if (lbl_1_bss_60.winId != -1) {
        HuWinMesWait(lbl_1_bss_60.winId);
        if (lbl_1_bss_60.closeF == 0) {
            HuWinExClose(lbl_1_bss_60.winId);
        }
        if (lbl_1_bss_60.animWinId != -1) {
            HuWinSprKill(lbl_1_bss_60.winId, lbl_1_bss_60.animWinId);
            HuSprAnimKill(lbl_1_bss_60.anim);
        }
        HuWinExKill(lbl_1_bss_60.winId);
        for (i = 0; i < 8; i++) {
            lbl_1_bss_60.insertMes[i] = -1;
        }
        lbl_1_bss_60.winId = -1;
        lbl_1_bss_60.mesSpeed = 1;
        lbl_1_bss_60.closeF = 0;
        lbl_1_bss_60.animNum = lbl_1_bss_60.animWinId = -1;
        lbl_1_bss_60.anim = NULL;
    }
}

static s32 fn_1_9EE8(s32 msgIdx)
{
    s32 i;
    HUWIN *w;
    s32 choice;
    s32 sel;
    s32 dir;

    fn_1_98D0(msgIdx);
    w = &winData[lbl_1_bss_60.winId];
    w->padMask = 1 << GwPlayerConf[0].padNo;
    if (msgIdx == 0x20) {
        sel = 1;
    } else {
        sel = 0;
    }
    choice = HuWinChoiceGet(lbl_1_bss_60.winId, sel);
    if (lbl_1_bss_60.winId != -1) {
        HuWinMesWait(lbl_1_bss_60.winId);
        if (lbl_1_bss_60.closeF == 0) {
            HuWinExClose(lbl_1_bss_60.winId);
        }
        if (lbl_1_bss_60.animWinId != -1) {
            HuWinSprKill(lbl_1_bss_60.winId, lbl_1_bss_60.animWinId);
            HuSprAnimKill(lbl_1_bss_60.anim);
        }
        HuWinExKill(lbl_1_bss_60.winId);
        for (i = 0; i < 8; i++) {
            lbl_1_bss_60.insertMes[i] = -1;
        }
        lbl_1_bss_60.winId = -1;
        lbl_1_bss_60.mesSpeed = 1;
        lbl_1_bss_60.closeF = 0;
        lbl_1_bss_60.animNum = lbl_1_bss_60.animWinId = -1;
        lbl_1_bss_60.anim = NULL;
    }
    if (GwCommon.storyNo != -2 && GwCommon.storyNo < 5 && msgIdx != 0x20 && msgIdx != 0x23 && msgIdx != 0x24 && choice == 0) {
        if (GwCommon.storyNo == -3) {
            GwCommon.storyNo = 0;
        }
        dir = (GwCommon.storyBoardNo[GwCommon.storyNo] == 0)   ? 0xCC0000
              : (GwCommon.storyBoardNo[GwCommon.storyNo] == 1) ? 0xCE0000
              : (GwCommon.storyBoardNo[GwCommon.storyNo] == 2) ? 0xD00000
              : (GwCommon.storyBoardNo[GwCommon.storyNo] == 3) ? 0xD20000
              : (GwCommon.storyBoardNo[GwCommon.storyNo] == 4) ? 0xD40000
              : (GwCommon.storyBoardNo[GwCommon.storyNo] == 5) ? 0xD60000
                                                               : 0xD80000;
        fn_1_19C(dir);
    }
    return choice;
}

static void fn_1_A294(s32 index)
{
    lbl_1_bss_60.insertMes[0] = lbl_1_data_2CC[index];
}

static s32 *fn_1_A2B8(ANIMDATA *anim, s16 layer, s16 camera)
{
    s32 *ids;
    s16 id;
    f32 rotZ;

    ids = HuMemDirectMallocNum(HEAP_HEAP, 12, HU_MEMNUM_OVL);
    id = Hu3DParManCreate(anim, 100, &lbl_1_data_38);
    ids[0] = id;
    Hu3DParManAttrSet(id, 0x65);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(id), 1);
    Hu3DParManRotSet(id, 90.0f, 0.0f, 0.0f);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(id), layer);
    Hu3DModelCameraSet(Hu3DParManModelIDGet(id), camera);
    id = Hu3DParManCreate(anim, 100, &lbl_1_data_88);
    ids[1] = id;
    Hu3DParManAttrSet(id, 0x163);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(id), 1);
    rotZ = frandmod(0x50) - 0x28;
    Hu3DParManRotSet(id, frandmod(0x28) + 0x46, 0.0f, rotZ);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(id), layer);
    Hu3DModelCameraSet(Hu3DParManModelIDGet(id), camera);
    id = Hu3DParManCreate(anim, 100, &lbl_1_data_D8);
    ids[2] = id;
    Hu3DParManAttrSet(id, 0x21);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(id), 1);
    Hu3DParManRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(id), layer);
    Hu3DModelCameraSet(Hu3DParManModelIDGet(id), camera);
    return ids;
}

static void fn_1_A4F4(s32 *ids, HuVecF *pos)
{
    Hu3DParManPosSet(ids[0], pos->x, pos->y, pos->z);
    Hu3DParManTimeLimitSet(ids[0], 2);
    Hu3DParManAttrReset(ids[0], 1);
    Hu3DParManPosSet(ids[1], pos->x, pos->y, pos->z);
    Hu3DParManTimeLimitSet(ids[1], 2);
    Hu3DParManAttrReset(ids[1], 1);
    Hu3DParManPosSet(ids[2], pos->x, pos->y, pos->z);
    Hu3DParManTimeLimitSet(ids[2], 5);
    Hu3DParManAttrReset(ids[2], 1);
}

static void fn_1_A5D0(s32 *ids)
{
    Hu3DParManKill(ids[0]);
    Hu3DParManKill(ids[1]);
    Hu3DParManKill(ids[2]);
}

static s32 *fn_1_A61C(ANIMDATA *anim, s16 layer, s16 camera)
{
    s32 *ids;
    s16 id;

    ids = HuMemDirectMallocNum(HEAP_HEAP, 12, HU_MEMNUM_OVL);
    id = Hu3DParManCreate(anim, 100, &lbl_1_data_128);
    ids[0] = id;
    Hu3DParManAttrSet(id, 0x65);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(id), 1);
    Hu3DParManRotSet(id, 90.0f, 0.0f, 0.0f);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(id), layer);
    Hu3DModelCameraSet(Hu3DParManModelIDGet(id), camera);
    return ids;
}

static void fn_1_A6F8(s32 *ids, HuVecF *pos)
{
    Hu3DParManPosSet(ids[0], pos->x, pos->y, pos->z);
    Hu3DParManTimeLimitSet(ids[0], 2);
    Hu3DParManAttrReset(ids[0], 1);
}

static void fn_1_A764(s32 *ids)
{
    Hu3DParManKill(ids[0]);
}
