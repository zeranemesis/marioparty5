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
f32 lbl_1_bss_9C[3];
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
HuVecF lbl_1_bss_18C[3];
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
static void fn_1_A294(s32 index, s32 unused);
static s32 *fn_1_A2B8(ANIMDATA *anim, s16 layer, s16 camera);
static void fn_1_A4F4(s32 *ids, HuVecF *pos);
static void fn_1_A5D0(s32 *ids);
static s32 *fn_1_A61C(ANIMDATA *anim, s16 layer, s16 camera);
static void fn_1_A6F8(s32 *ids, HuVecF *pos);
static void fn_1_A764(s32 *ids);

#ifdef __MWERKS__
#define STORY_BOARD_PREV(n) GwCommon.storyBoardNo[(n)-1]
#else
// the original reads one byte before the array when storyNo is 0
#define STORY_BOARD_PREV(n) GwCommon.storyBoardNo[((n) < 1) ? 0 : ((n)-1)]
#endif

static const HuVecF lbl_1_rodata_B0 = { 0.0f, 1.0f, 0.0f };

void fn_1_13E8(void)
{
    s32 storyNo;
    s32 state;
    s16 parId0;
    s16 parId1;
    s32 modelId;
#ifdef TARGET_PC
    s32 flag = 0;
#else
    s32 flag;
#endif
    s32 *ids0;
    s32 *ids1;
    HuVecF *upP;
    HuVecF pos;
    HuVecF up;
    ANIMDATA *anim0;
    ANIMDATA *anim1;
    ANIMDATA *anim2;
    s32 *tmp0;
    s32 *tmp1;
    s32 *tmp2;
    s32 *tmp3;

    storyNo = GwCommon.storyNo;
    HuWinInit(1);
    fn_1_9CCC(0);
    if (storyNo == -2) {
        state = 0;
    } else if (storyNo == -3) {
        state = 100;
    } else if (storyNo == 5) {
        state = 300;
        flag = 0;
    } else if (storyNo < 6) {
        state = 200;
    } else if (GwPlayer[0].mgCoinBonus != 0) {
        state = 400;
    } else {
        state = 300;
        flag = 1;
    }
    lbl_1_bss_188 = GwPlayerConf[0].charNo;
    switch (state) {
        case 0:
            lbl_1_bss_184 = 1;
            lbl_1_bss_17C = 1;
            fn_1_70F4(8);
            pos = lbl_1_data_178;
            pos.x += 60.0f;
            pos.y += 90.0f;
            fn_1_7408(&pos.x);
            pos = Hu3DData[lbl_1_bss_114].pos;
            lbl_1_bss_C0[0] = Hu3DLLightCreate(lbl_1_bss_114, pos.x, pos.y, pos.z, 0.0f, 0.0f, -1.0f, 0, 0, 0);
            Hu3DLLightStaticSet(lbl_1_bss_114, lbl_1_bss_C0[0], 1);
            Hu3DLLightInfinitytSet(lbl_1_bss_114, lbl_1_bss_C0[0]);
            lbl_1_bss_E4 = 0;
            HuPrcChildCreate(fn_1_7660, 100, 0x2000, 0, HuPrcCurrentGet());
            fn_1_7BE4();
            Hu3DData[lbl_1_bss_E2].pos.x = Hu3DData[lbl_1_bss_114].pos.x - 128.72514f;
            Hu3DData[lbl_1_bss_E2].pos.y = Hu3DData[lbl_1_bss_114].pos.y - 1.991341f;
            Hu3DData[lbl_1_bss_E2].pos.z = 142.54988f + Hu3DData[lbl_1_bss_114].pos.z;
            pos = Hu3DData[lbl_1_bss_E2].pos;
            lbl_1_bss_C0[1] = Hu3DLLightCreate(lbl_1_bss_E2, pos.x, pos.y, pos.z, 0.0f, 0.0f, -1.0f, 0, 0, 0);
            Hu3DLLightStaticSet(lbl_1_bss_E2, lbl_1_bss_C0[1], 1);
            Hu3DLLightInfinitytSet(lbl_1_bss_E2, lbl_1_bss_C0[1]);
            HuPrcChildCreate(fn_1_8018, 100, 0x2000, 0, HuPrcCurrentGet());
            lbl_1_bss_18C[0] = Hu3DData[lbl_1_bss_114].pos;
            lbl_1_bss_18C[0].y += 170.0f;
            lbl_1_bss_18C[1] = lbl_1_bss_18C[0];
            lbl_1_bss_18C[1].z += 480.0f;
            up = lbl_1_rodata_B0;
            upP = &up;
            lbl_1_bss_18C[2] = *upP;
            Hu3DCameraPosSetV(2, &lbl_1_bss_18C[0], &lbl_1_bss_18C[2], &lbl_1_bss_18C[1]);
            Hu3DGLightColorSet(lbl_1_bss_40[0], 0, 0, 0, 0xFF);
            Hu3DGLightColorSet(lbl_1_bss_40[1], 0, 0, 0, 0xFF);
            modelId = Hu3DModelCreateData(0xC10061);
            Hu3DModelAttrSet(modelId, 0x40000001);
            Hu3DModelCameraSet(modelId, 2);
            Hu3DModelLayerSet(modelId, 7);
            pos = Hu3DData[lbl_1_bss_E2].pos;
            pos.x = 0.0f;
            pos.y -= 10.0f;
            Hu3DModelPosSetV(modelId, &pos);
            lbl_1_bss_C = 0x1C;
            break;
        case 100:
            lbl_1_bss_184 = 0x65;
            fn_1_66C4(lbl_1_bss_188, 1);
            pos = lbl_1_data_178;
            pos.x += 120.0f;
            fn_1_6924(&pos);
            fn_1_6B38();
            pos = lbl_1_data_178;
            pos.x -= 120.0f;
            fn_1_6CBC(&pos.x);
            fn_1_6E0C();
            pos = lbl_1_data_178;
            pos.x += 420.0f;
            fn_1_6F5C(&pos);
            lbl_1_bss_C = 0x1D;
            break;
        case 200:
            if (GWPlayerCoinGet(0) > 0) {
                if (storyNo < 4) {
                    lbl_1_bss_184 = (STORY_BOARD_PREV(storyNo) == 0)   ? 0xCC
                                    : (STORY_BOARD_PREV(storyNo) == 1) ? 0xCD
                                    : (STORY_BOARD_PREV(storyNo) == 2) ? 0xCE
                                    : (STORY_BOARD_PREV(storyNo) == 3) ? 0xCF
                                    : (STORY_BOARD_PREV(storyNo) == 4) ? 0xD0
                                                                       : 0xD1;
                } else {
                    lbl_1_bss_184 = 0xD4;
                }
                lbl_1_bss_17C = 1;
                fn_1_66C4(lbl_1_bss_188, 1);
            } else {
                lbl_1_bss_184 = 0xC9;
                fn_1_66C4(lbl_1_bss_188, 3);
            }
            pos = lbl_1_data_178;
            pos.x += 120.0f;
            fn_1_6924(&pos);
            fn_1_6B38();
            pos = lbl_1_data_178;
            pos.x -= 120.0f;
            fn_1_6CBC(&pos.x);
            fn_1_6E0C();
            pos = lbl_1_data_178;
            pos.x += 420.0f;
            fn_1_6F5C(&pos);
            lbl_1_bss_C = 0x1D;
            break;
        case 300:
            lbl_1_bss_184 = (flag == 0) ? 0x12D : 0x131;
            fn_1_66C4(lbl_1_bss_188, (flag == 0) ? 1 : 3);
            pos = lbl_1_data_178;
            pos.x += 120.0f;
            fn_1_6924(&pos);
            fn_1_70F4(1);
            pos = lbl_1_data_178;
            pos.x -= 140.0f;
            fn_1_7408(&pos.x);
            HuPrcChildCreate(fn_1_7660, 100, 0x2000, 0, HuPrcCurrentGet());
            anim0 = HuSprAnimRead(HuDataReadNumHeapShortForce(mgDataTbl[61].instPic[0], HU_MEMNUM_OVL, HEAP_MODEL));
            Hu3DAnimCreate(anim0, lbl_1_bss_3C, lbl_1_data_1AC);
            HuTHPSprCreateVol(mgDataTbl[61].movie[0], 1, 100, 0);
            Hu3DModelMatHookSet(lbl_1_bss_3C, HuTHPMatHook);
            lbl_1_bss_C = 0x1E;
            break;
        case 400:
            lbl_1_bss_184 = 0x191;
            fn_1_66C4(lbl_1_bss_188, 1);
            pos = lbl_1_data_178;
            pos.x += 120.0f;
            fn_1_6924(&pos);
            fn_1_70F4(1);
            pos = lbl_1_data_178;
            pos.x -= 140.0f;
            fn_1_7408(&pos.x);
            HuPrcChildCreate(fn_1_7660, 100, 0x2000, 0, HuPrcCurrentGet());
            fn_1_6B38();
            pos = lbl_1_data_178;
            pos.x -= 420.0f;
            fn_1_6CBC(&pos.x);
            fn_1_6E0C();
            pos = lbl_1_data_178;
            pos.x += 420.0f;
            fn_1_6F5C(&pos);
            fn_1_6FF8(2);
            lbl_1_bss_C = 0x1F;
            break;
    }
    lbl_1_bss_1BC[0] = lbl_1_bss_1BC[1] = lbl_1_bss_1BC[2] = 0;
    lbl_1_bss_1B0[0] = lbl_1_bss_1B0[1] = lbl_1_bss_1B0[2] = 0.0f;
    lbl_1_bss_178 = 0;
    lbl_1_bss_154[0] = lbl_1_bss_154[1] = 0.0f;
    lbl_1_bss_174 = 0;
    if (GwCommon.storyNo == -2) {
        lbl_1_bss_C6[0] = espEntry(0xC1005E, 0, 0);
        lbl_1_bss_C6[1] = espEntry(0xC1005F, 0, 0);
        lbl_1_bss_C6[2] = espEntry(0xC1005F, 0, 0);
        lbl_1_bss_C6[3] = espEntry(0xC10060, 0, 0);
        lbl_1_bss_C6[4] = espEntry(0xC10060, 0, 0);
        espScaleSet(lbl_1_bss_C6[0], 0.0f, 0.0f);
        espScaleSet(lbl_1_bss_C6[1], 0.0f, 0.0f);
        espScaleSet(lbl_1_bss_C6[2], 0.0f, 0.0f);
        espScaleSet(lbl_1_bss_C6[3], 0.0f, 0.0f);
        espScaleSet(lbl_1_bss_C6[4], 0.0f, 0.0f);
    }
    if (GwCommon.storyNo != -2) {
        anim1 = HuSprAnimRead(HuDataReadNum(0xC1006C, HU_MEMNUM_OVL));
        ids0 = HuMemDirectMallocNum(HEAP_HEAP, 12, HU_MEMNUM_OVL);
        parId0 = Hu3DParManCreate(anim1, 100, &lbl_1_data_128);
        ids0[0] = parId0;
        Hu3DParManAttrSet(parId0, 0x65);
        Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parId0), 1);
        Hu3DParManRotSet(parId0, 90.0f, 0.0f, 0.0f);
        Hu3DModelLayerSet(Hu3DParManModelIDGet(parId0), 4);
        Hu3DModelCameraSet(Hu3DParManModelIDGet(parId0), 2);
        tmp0 = ids0;
        tmp1 = tmp0;
        lbl_1_bss_58[0] = tmp1;
        anim2 = HuSprAnimRead(HuDataReadNum(0xC1006C, HU_MEMNUM_OVL));
        ids1 = HuMemDirectMallocNum(HEAP_HEAP, 12, HU_MEMNUM_OVL);
        parId1 = Hu3DParManCreate(anim2, 100, &lbl_1_data_128);
        ids1[0] = parId1;
        Hu3DParManAttrSet(parId1, 0x65);
        Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parId1), 1);
        Hu3DParManRotSet(parId1, 90.0f, 0.0f, 0.0f);
        Hu3DModelLayerSet(Hu3DParManModelIDGet(parId1), 4);
        Hu3DModelCameraSet(Hu3DParManModelIDGet(parId1), 2);
        tmp2 = ids1;
        tmp3 = tmp2;
        lbl_1_bss_58[1] = tmp3;
    }
}

s32 fn_1_2448(void)
{
    s32 ret;
    s32 storyNo;
    f32 degA;
    f32 degB;
    HuVecF scr;
    HuVecF vB;
    HuVecF vC;
    HuVecF vD;
    HuVecF vE;
    HuVecF vF;
    HSFOBJECT *obj;

    ret = 0;
    storyNo = GwCommon.storyNo;
    switch (lbl_1_bss_184) {
        case 1:
            if (lbl_1_bss_17C == 1) {
                if (lbl_1_bss_1BC[0] == 0) {
                    HuAudFXPanning(HuAudFXPlay(0x2C9), 0x38);
                }
                if (++lbl_1_bss_1BC[0] > 30) {
                    degA = 180.0f;
                    vB = lbl_1_bss_18C[0];
                    vC = lbl_1_bss_18C[1];
                    vB.x = vB.x - lbl_1_bss_18C[0].x * sin(M_PI * lbl_1_bss_1B0[0] / 180.0);
                    vB.y = vB.y - lbl_1_bss_18C[0].y * sin(M_PI * lbl_1_bss_1B0[0] / 180.0);
                    vB.z = vB.z + (1550.0f - lbl_1_bss_18C[0].z) * sin(M_PI * lbl_1_bss_1B0[0] / 180.0);
                    vC.x = vC.x - lbl_1_bss_18C[1].x * sin(M_PI * lbl_1_bss_1B0[0] / 180.0);
                    vC.y = vC.y - lbl_1_bss_18C[1].y * sin(M_PI * lbl_1_bss_1B0[0] / 180.0);
                    Hu3DCameraPosSetV(2, &vB, &lbl_1_bss_18C[2], &vC);
                    lbl_1_bss_1B0[0] += 90.0f / degA;
                    if (lbl_1_bss_1B0[0] >= 90.0f) {
                        lbl_1_bss_17C = 2;
                        lbl_1_bss_1BC[0] = 0;
                        lbl_1_bss_9C[0] = 0.0f;
                        lbl_1_bss_9C[1] = 0.0f;
                        lbl_1_bss_9C[2] = 0.0f;
                        Hu3DModelAttrReset(lbl_1_bss_BC[0], 0x40000002);
                        Hu3DModelAttrReset(lbl_1_bss_BC[1], 0x40000002);
                    }
                }
            } else if (lbl_1_bss_17C == 2) {
                lbl_1_bss_9C[1] += 9.0f;
                lbl_1_bss_9C[2] += 12.0f;
                Hu3DLLightColorSet(lbl_1_bss_114, lbl_1_bss_C0[0], lbl_1_bss_9C[0], lbl_1_bss_9C[1], lbl_1_bss_9C[2], 0xFF);
                Hu3DLLightColorSet(lbl_1_bss_E2, lbl_1_bss_C0[1], lbl_1_bss_9C[0], lbl_1_bss_9C[1], lbl_1_bss_9C[2], 0xFF);
                if (++lbl_1_bss_1BC[0] == 20) {
                    lbl_1_bss_17C = 0;
                }
            } else {
                HuAudFXPanning(HuAudFXPlay(0x2C9), 0x38);
                fn_1_744C(9);
                fn_1_9D84(0x29);
                lbl_1_bss_1BC[0] = (lbl_1_bss_E8 == 8) ? 10 : 0;
                lbl_1_bss_184 = 2;
            }
            break;
        case 2:
            if (++lbl_1_bss_1BC[0] == 20) {
                fn_1_744C(10);
                fn_1_9D84(0x2A);
                fn_1_744C(8);
                lbl_1_bss_1BC[0] = 0;
                lbl_1_bss_184 = 3;
            }
            break;
        case 3:
            if (++lbl_1_bss_1BC[0] == 10) {
                fn_1_7E24(2);
                fn_1_9D84(0x2B);
                fn_1_7E24(1);
                lbl_1_bss_1BC[0] = 0;
                lbl_1_bss_184 = 4;
            }
            break;
        case 4:
            if (++lbl_1_bss_1BC[0] == 10) {
                fn_1_744C(10);
                fn_1_9D84(0x2C);
                lbl_1_bss_1BC[0] = 0;
                lbl_1_bss_184 = 5;
            }
            break;
        case 5:
            if (++lbl_1_bss_1BC[0] == 10) {
                HuAudFXPanning(HuAudFXPlay(0x2CB), 0x38);
                fn_1_744C(13);
                fn_1_9D84(0x2D);
                lbl_1_bss_1BC[0] = 0;
                lbl_1_bss_184 = 6;
            }
            break;
        case 6:
            if (++lbl_1_bss_1BC[0] == 15) {
                fn_1_7E24(5);
                fn_1_9D84(0x2E);
                fn_1_7E24(4);
                lbl_1_bss_1BC[0] = 0;
                lbl_1_bss_184 = 7;
            }
            break;
        case 7:
            if (++lbl_1_bss_1BC[0] == 10) {
                fn_1_744C(15);
                fn_1_9D84(0x2F);
                fn_1_744C(14);
                lbl_1_bss_184 = 8;
            }
            break;
        case 8:
            HuAudFXPanning(HuAudFXPlay(0x237), 0x38);
            fn_1_7E24(6);
            fn_1_9D84(0x30);
            fn_1_7E24(4);
            lbl_1_bss_1BC[0] = 0;
            lbl_1_bss_184 = 9;
            break;
        case 9:
            if (++lbl_1_bss_1BC[0] == 10) {
                fn_1_744C(11);
                fn_1_9D84(0x31);
                fn_1_744C(12);
                fn_1_7E24(8);
                lbl_1_bss_17C = 1;
                lbl_1_bss_1BC[0] = 0;
                lbl_1_bss_184 = 0xB;
            }
            break;
        case 10:
            if (++lbl_1_bss_1BC[0] == 60) {
                HuAudFXPanning(HuAudFXPlay(0x59C), 0x38);
                fn_1_7E24(9);
                fn_1_9D84(0x32);
                fn_1_7E24(8);
                lbl_1_bss_1BC[0] = 0;
                lbl_1_bss_184 = 0xB;
            }
            break;
        case 11:
            if (lbl_1_bss_17C == 1) {
                if (++lbl_1_bss_1BC[0] == 15) {
                    espDrawNoSet(lbl_1_bss_C6[0], 0);
                    espPriSet(lbl_1_bss_C6[0], 0x5DC);
                    espDrawNoSet(lbl_1_bss_C6[1], 0);
                    espPriSet(lbl_1_bss_C6[1], 0x5DC);
                    espDrawNoSet(lbl_1_bss_C6[2], 0);
                    espPriSet(lbl_1_bss_C6[2], 0x5DC);
                    espDrawNoSet(lbl_1_bss_C6[3], 0);
                    espPriSet(lbl_1_bss_C6[3], 0x5DC);
                    espDrawNoSet(lbl_1_bss_C6[4], 0);
                    espPriSet(lbl_1_bss_C6[4], 0x5DC);
                    espScaleSet(lbl_1_bss_C6[0], 4.0f, 4.0f);
                    espScaleSet(lbl_1_bss_C6[1], 1.0f, 4.0f);
                    espScaleSet(lbl_1_bss_C6[2], 1.0f, 4.0f);
                    espScaleSet(lbl_1_bss_C6[3], 4.0f, 1.0f);
                    espScaleSet(lbl_1_bss_C6[4], 4.0f, 1.0f);
                    Hu3DModelObjPosGet(lbl_1_bss_114, lbl_1_data_1BA, &lbl_1_bss_18C[0]);
                    lbl_1_bss_18C[0].x += 5.0f;
                    lbl_1_bss_18C[0].y -= 25.0f;
                    Hu3D3Dto2D(&lbl_1_bss_18C[0], 2, &scr);
                    lbl_1_bss_A8[0] = (scr.x - 384.0f) - 127.0f;
                    lbl_1_bss_A8[1] = 127.0f + (384.0f + scr.x);
                    lbl_1_bss_A8[2] = (scr.y - 384.0f) - 127.0f;
                    lbl_1_bss_A8[3] = 127.0f + (384.0f + scr.y);
                    espPosSet(lbl_1_bss_C6[0], scr.x, scr.y);
                    espPosSet(lbl_1_bss_C6[1], lbl_1_bss_A8[0], scr.y);
                    espPosSet(lbl_1_bss_C6[2], lbl_1_bss_A8[1], scr.y);
                    espPosSet(lbl_1_bss_C6[3], scr.x, lbl_1_bss_A8[2]);
                    espPosSet(lbl_1_bss_C6[4], scr.x, lbl_1_bss_A8[3]);
                    lbl_1_bss_17C++;
                    lbl_1_bss_1BC[0] = 0;
                    lbl_1_bss_1B0[0] = 4.0f;
                }
            } else if (lbl_1_bss_17C == 2) {
                lbl_1_bss_1B0[0] -= 0.05f;
                if (lbl_1_bss_1B0[0] < 1.0f) {
                    lbl_1_bss_1B0[0] = 1.0f;
                }
                lbl_1_bss_1B0[1] = 1024.0f - 256.0f * lbl_1_bss_1B0[0];
                espScaleSet(lbl_1_bss_C6[0], lbl_1_bss_1B0[0], lbl_1_bss_1B0[0]);
                espScaleSet(lbl_1_bss_C6[1], lbl_1_bss_1B0[1], 4.0f);
                espScaleSet(lbl_1_bss_C6[2], lbl_1_bss_1B0[1], 4.0f);
                espScaleSet(lbl_1_bss_C6[3], 4.0f, lbl_1_bss_1B0[1]);
                espScaleSet(lbl_1_bss_C6[4], 4.0f, lbl_1_bss_1B0[1]);
                Hu3D3Dto2D(&lbl_1_bss_18C[0], 2, &scr);
                espPosSet(lbl_1_bss_C6[1], lbl_1_bss_A8[0] - lbl_1_bss_1B0[1] * 0.5f, scr.y);
                espPosSet(lbl_1_bss_C6[2], lbl_1_bss_A8[1] - lbl_1_bss_1B0[1] * 0.5f, scr.y);
                espPosSet(lbl_1_bss_C6[3], scr.x, lbl_1_bss_A8[2] - lbl_1_bss_1B0[1] * 0.5f);
                espPosSet(lbl_1_bss_C6[4], scr.x, lbl_1_bss_A8[3] - lbl_1_bss_1B0[1] * 0.5f);
                if (lbl_1_bss_1B0[0] == 1.0f) {
                    lbl_1_bss_17C++;
                    lbl_1_bss_1BC[0] = 0;
                    lbl_1_bss_184 = 10;
                }
            } else if (lbl_1_bss_17C == 3) {
                if (++lbl_1_bss_1BC[0] == 15) {
                    HuAudSStreamFadeOut(lbl_1_bss_10, 500);
                    lbl_1_bss_17C++;
                }
            } else if (lbl_1_bss_17C == 4) {
                lbl_1_bss_1B0[0] -= 0.08f;
                if (lbl_1_bss_1B0[0] < 0.0f) {
                    lbl_1_bss_1B0[0] = 0.0f;
                }
                lbl_1_bss_1B0[1] = 1024.0f - 256.0f * lbl_1_bss_1B0[0];
                espScaleSet(lbl_1_bss_C6[0], lbl_1_bss_1B0[0], lbl_1_bss_1B0[0]);
                espScaleSet(lbl_1_bss_C6[1], lbl_1_bss_1B0[1], 4.0f);
                espScaleSet(lbl_1_bss_C6[2], lbl_1_bss_1B0[1], 4.0f);
                espScaleSet(lbl_1_bss_C6[3], 4.0f, lbl_1_bss_1B0[1]);
                espScaleSet(lbl_1_bss_C6[4], 4.0f, lbl_1_bss_1B0[1]);
                Hu3D3Dto2D(&lbl_1_bss_18C[0], 2, &scr);
                espPosSet(lbl_1_bss_C6[1], lbl_1_bss_A8[0] - lbl_1_bss_1B0[1] * 0.5f, scr.y);
                espPosSet(lbl_1_bss_C6[2], lbl_1_bss_A8[1] - lbl_1_bss_1B0[1] * 0.5f, scr.y);
                espPosSet(lbl_1_bss_C6[3], scr.x, lbl_1_bss_A8[2] - lbl_1_bss_1B0[1] * 0.5f);
                espPosSet(lbl_1_bss_C6[4], scr.x, lbl_1_bss_A8[3] - lbl_1_bss_1B0[1] * 0.5f);
                if (lbl_1_bss_1B0[0] == 0.0f) {
                    lbl_1_bss_17C = 0;
                }
            } else {
                if (HuAudSStreamStatGet(lbl_1_bss_10) == 0) {
                    ret = -1;
                    lbl_1_bss_184 = 1000;
                }
            }
            break;
        case 0x65:
            if (++lbl_1_bss_1BC[0] > 120) {
                HuAudFXPanning(HuAudFXPlay(0x2F3), 0x38);
                fn_1_6D00(2);
                fn_1_9D84(0);
                lbl_1_bss_17C = 1;
                lbl_1_bss_184 = 0x66;
            }
            break;
        case 0x66:
            if (lbl_1_bss_17C == 1) {
                if (fn_1_9EE8(0x21) != 0) {
                    lbl_1_bss_180 = lbl_1_bss_184;
                    lbl_1_bss_184 = 0x1F4;
                } else {
                    lbl_1_bss_17C = 0;
                }
            } else if (fn_1_96CC(0) != 0) {
                lbl_1_bss_17C = 1;
                fn_1_6D00(1);
                lbl_1_bss_184 = 0x67;
            }
            break;
        case 0x67:
            if (lbl_1_bss_17C != 0) {
                Hu3DModelAttrReset(lbl_1_bss_3C, 1);
                lbl_1_bss_1B0[0] += 0.025f;
                if (lbl_1_bss_1B0[0] > 1.0f) {
                    lbl_1_bss_1B0[0] = 1.0f;
                }
                Hu3DModelScaleSet(lbl_1_bss_3C, lbl_1_bss_1B0[0], lbl_1_bss_1B0[0], lbl_1_bss_1B0[0]);
                if (lbl_1_bss_1B0[0] == 1.0f) {
                    fn_1_6D00(2);
                    lbl_1_bss_17C = 0;
                }
            } else {
                fn_1_A294(lbl_1_bss_188, 0);
                fn_1_9D84(1);
                fn_1_6D00(1);
                lbl_1_bss_184 = 0x68;
            }
            break;
        case 0x68:
            if (fn_1_8150(0) != 0) {
                lbl_1_bss_1BC[0] = 0;
                lbl_1_bss_1B0[0] = 0.0f;
                lbl_1_bss_1B0[1] = 150.0f;
                lbl_1_bss_17C = 1;
                lbl_1_bss_184 = 0x69;
            }
            break;
        case 0x69:
            if (++lbl_1_bss_1BC[0] > 60) {
                if (lbl_1_bss_17C != 0) {
                    lbl_1_bss_1B0[0] += 1.0f;
                    if (lbl_1_bss_1B0[0] > 90.0f) {
                        lbl_1_bss_1B0[0] = 90.0f;
                    }
                    scr = lbl_1_data_178;
                    scr.x += 420.0f;
                    scr.x = scr.x - 300.0 * sin(M_PI * lbl_1_bss_1B0[0] / 180.0);
                    scr.y -= lbl_1_bss_1B0[1];
                    if (scr.x <= 616.0f) {
                        lbl_1_bss_1B0[1] = lbl_1_bss_1B0[1] - lbl_1_bss_1B0[1] / 9.0f;
                        if (scr.y > lbl_1_data_178.y) {
                            scr.y = lbl_1_data_178.y;
                        }
                    }
                    fn_1_6F5C(&scr);
                    if (lbl_1_bss_1B0[0] == 90.0f) {
                        lbl_1_bss_17C = 0;
                    }
                } else {
                    HuAudFXPanning(HuAudFXPlay(0x1D0), 0x38);
                    fn_1_A294(lbl_1_bss_188, 0);
                    fn_1_9D84(2);
                    fn_1_6D00(2);
                    lbl_1_bss_184 = 0x6A;
                }
            }
            break;
        case 0x6A:
            fn_1_9D84(3);
            fn_1_6D00(1);
            lbl_1_bss_17C = 1;
            lbl_1_bss_184 = 0x6B;
            break;
        case 0x6B:
            if (lbl_1_bss_17C == 1) {
                if (fn_1_96CC(1) != 0) {
                    lbl_1_bss_1BC[0] = 0;
                    lbl_1_bss_17C = 2;
                }
            } else if (lbl_1_bss_17C == 2) {
                if (++lbl_1_bss_1BC[0] > 30 && fn_1_8150(1) != 0) {
                    lbl_1_bss_1BC[0] = 0;
                    lbl_1_bss_17C = 0;
                }
            } else {
                if (++lbl_1_bss_1BC[0] > 60) {
                    ret = 1;
                    lbl_1_bss_184 = 1000;
                }
            }
            break;
        case 0xC9:
            if (++lbl_1_bss_1BC[0] > 60) {
                HuAudFXPanning(HuAudFXPlay(0x2F3), 0x38);
                lbl_1_bss_17C = 1;
                fn_1_6D00(2);
                fn_1_A294(lbl_1_bss_188, 0);
                fn_1_9D84(4);
                lbl_1_bss_184 = (storyNo < 4) ? 0xCA : 0xCB;
            }
            break;
        case 0xCA:
            if (lbl_1_bss_17C == 1) {
                if (fn_1_9EE8(0x22) != 0) {
                    lbl_1_bss_180 = lbl_1_bss_184;
                    lbl_1_bss_184 = 0x1F4;
                } else {
                    lbl_1_bss_1BC[0] = 0;
                    lbl_1_bss_17C = 2;
                    fn_1_69CC(1);
                }
            } else if (lbl_1_bss_17C == 2) {
                if (++lbl_1_bss_1BC[0] > 30) {
                    lbl_1_bss_17C = 0;
                }
            } else if (fn_1_96CC(0) != 0) {
                lbl_1_bss_17C = 1;
                fn_1_6D00(1);
                lbl_1_bss_184 = 0x67;
            }
            break;
        case 0xCB:
            if (lbl_1_bss_17C == 1) {
                if (fn_1_9EE8(0x26) != 0) {
                    lbl_1_bss_180 = lbl_1_bss_184;
                    lbl_1_bss_184 = 0x1F4;
                } else {
                    lbl_1_bss_1BC[0] = 0;
                    lbl_1_bss_17C = 2;
                    fn_1_69CC(1);
                }
            } else if (lbl_1_bss_17C == 2) {
                if (++lbl_1_bss_1BC[0] > 30) {
                    lbl_1_bss_17C = 0;
                }
            } else if (fn_1_96CC(0) != 0) {
                lbl_1_bss_17C = 1;
                fn_1_6D00(1);
                lbl_1_bss_184 = 0x67;
            }
            break;
        case 0xCC:
            HuAudFXPanning(HuAudFXPlay(0x2F1), 0x38);
            fn_1_A294(lbl_1_bss_188, 0);
            fn_1_9D84(5);
            lbl_1_bss_184 = (storyNo < 4) ? 0xD2 : 0xD4;
            break;
        case 0xCD:
            HuAudFXPanning(HuAudFXPlay(0x2F1), 0x38);
            fn_1_A294(lbl_1_bss_188, 0);
            fn_1_9D84(6);
            lbl_1_bss_184 = (storyNo < 4) ? 0xD2 : 0xD4;
            break;
        case 0xCE:
            HuAudFXPanning(HuAudFXPlay(0x2F1), 0x38);
            fn_1_A294(lbl_1_bss_188, 0);
            fn_1_9D84(7);
            lbl_1_bss_184 = (storyNo < 4) ? 0xD2 : 0xD4;
            break;
        case 0xCF:
            HuAudFXPanning(HuAudFXPlay(0x2F1), 0x38);
            fn_1_A294(lbl_1_bss_188, 0);
            fn_1_9D84(8);
            lbl_1_bss_184 = (storyNo < 4) ? 0xD2 : 0xD4;
            break;
        case 0xD0:
            HuAudFXPanning(HuAudFXPlay(0x2F1), 0x38);
            fn_1_A294(lbl_1_bss_188, 0);
            fn_1_9D84(9);
            lbl_1_bss_184 = (storyNo < 4) ? 0xD2 : 0xD4;
            break;
        case 0xD1:
            HuAudFXPanning(HuAudFXPlay(0x2F1), 0x38);
            fn_1_A294(lbl_1_bss_188, 0);
            fn_1_9D84(10);
            lbl_1_bss_184 = (storyNo < 4) ? 0xD2 : 0xD4;
            break;
        case 0xD2:
            fn_1_9D84(11);
            lbl_1_bss_17C = 1;
            lbl_1_bss_184 = 0xD3;
            break;
        case 0xD3:
            if (lbl_1_bss_17C == 1) {
                if (fn_1_9EE8(0x25) != 0) {
                    lbl_1_bss_180 = lbl_1_bss_184;
                    lbl_1_bss_184 = 0x1F4;
                } else {
                    lbl_1_bss_17C = 0;
                }
            } else if (fn_1_96CC(0) != 0) {
                lbl_1_bss_17C = 1;
                fn_1_69CC(1);
                fn_1_6D00(1);
                lbl_1_bss_184 = 0x67;
            }
            break;
        case 0xD4:
            HuAudFXPanning(HuAudFXPlay(0x2F1), 0x38);
            fn_1_A294(lbl_1_bss_188, 0);
            fn_1_9D84(12);
            lbl_1_bss_17C = 1;
            lbl_1_bss_184 = 0xD5;
            break;
        case 0xD5:
            if (lbl_1_bss_17C == 1) {
                if (fn_1_9EE8(0x26) != 0) {
                    lbl_1_bss_180 = lbl_1_bss_184;
                    lbl_1_bss_184 = 0x1F4;
                } else {
                    lbl_1_bss_17C = 0;
                }
            } else if (fn_1_96CC(0) != 0) {
                lbl_1_bss_17C = 1;
                fn_1_69CC(1);
                fn_1_6D00(1);
                lbl_1_bss_184 = 0x67;
            }
            break;
        case 0x12D:
            if (++lbl_1_bss_1BC[0] > 60) {
                HuAudFXPanning(HuAudFXPlay(0x2C9), 0x38);
                fn_1_744C(7);
                fn_1_A294(lbl_1_bss_188, 0);
                fn_1_9D84(13);
                fn_1_744C(6);
                lbl_1_bss_184 = 0x12E;
            }
            break;
        case 0x12E:
            if (Hu3DMotionShiftIDGet(lbl_1_bss_114) == -1) {
                fn_1_9D84(14);
                lbl_1_bss_17C = 1;
                fn_1_744C(1);
                lbl_1_bss_184 = 0x12F;
            }
            break;
        case 0x12F:
            if (Hu3DMotionShiftIDGet(lbl_1_bss_114) == -1) {
                if (lbl_1_bss_17C == 1) {
                    if (fn_1_9EE8(0x23) != 0) {
                        lbl_1_bss_180 = lbl_1_bss_184;
                        lbl_1_bss_184 = 0x1F4;
                    } else {
                        lbl_1_bss_17C = 0;
                    }
                } else if (fn_1_96CC(0) != 0) {
                    lbl_1_bss_17C = 0;
                    fn_1_744C(6);
                    lbl_1_bss_184 = 0x130;
                }
            }
            break;
        case 0x130:
            if (Hu3DMotionShiftIDGet(lbl_1_bss_114) == -1 && lbl_1_bss_17C == 0) {
                HuAudFXPanning(HuAudFXPlay(0x2CB), 0x38);
                fn_1_A294(lbl_1_bss_188, 0);
                fn_1_9D84(15);
                lbl_1_bss_17C = 1;
            }
            break;
        case 0x131:
            if (++lbl_1_bss_1BC[0] > 60) {
                HuAudFXPanning(HuAudFXPlay(0x2C9), 0x38);
                fn_1_744C(7);
                fn_1_A294(lbl_1_bss_188, 0);
                fn_1_9D84(16);
                lbl_1_bss_17C = 1;
                fn_1_744C(1);
                lbl_1_bss_184 = 0x132;
            }
            break;
        case 0x132:
            if (Hu3DMotionShiftIDGet(lbl_1_bss_114) == -1) {
                if (lbl_1_bss_17C == 1) {
                    if (fn_1_9EE8(0x24) != 0) {
                        lbl_1_bss_180 = lbl_1_bss_184;
                        lbl_1_bss_184 = 0x1F4;
                    } else {
                        lbl_1_bss_1BC[0] = 0;
                        lbl_1_bss_17C = 2;
                        fn_1_69CC(1);
                    }
                } else if (lbl_1_bss_17C == 2) {
                    if (++lbl_1_bss_1BC[0] > 30) {
                        lbl_1_bss_17C = 0;
                    }
                } else if (fn_1_96CC(0) != 0) {
                    lbl_1_bss_17C = 0;
                    fn_1_744C(6);
                    lbl_1_bss_184 = 0x130;
                }
            }
            break;
        case 0x133:
            if (lbl_1_bss_17C == 1 && fn_1_8B34() != 0) {
                lbl_1_bss_17C = 0;
            }
            if (++lbl_1_bss_1BC[0] > 30 && fn_1_8150(0) != 0) {
                ret = 1;
                lbl_1_bss_184 = 1000;
            }
            break;
        case 0x191:
            HuAudFXPanning(HuAudFXPlay(0x2CA), 0x38);
            fn_1_744C(3);
            fn_1_9D84(17);
            fn_1_744C(1);
            lbl_1_bss_184 = 0x192;
            break;
        case 0x192:
            if (Hu3DMotionShiftIDGet(lbl_1_bss_114) == -1) {
                fn_1_9D84(18);
                lbl_1_bss_17C = 1;
                lbl_1_bss_184 = 0x193;
            }
            break;
        case 0x193:
            if (lbl_1_bss_17C == 1) {
                if (Hu3DMotionShiftIDGet(lbl_1_bss_114) == -1) {
                    fn_1_744C(4);
                    fn_1_9D84(19);
                    fn_1_744C(1);
                    lbl_1_bss_17C = 0;
                }
            } else if (fn_1_96CC(0) != 0) {
                lbl_1_bss_14[0] = 0;
                lbl_1_bss_184 = 0x194;
                fn_1_744C(6);
            }
            break;
        case 0x194:
            if (Hu3DMotionShiftIDGet(lbl_1_bss_114) == -1) {
                fn_1_9D84(20);
                lbl_1_bss_14[0] = 0;
                lbl_1_bss_184 = 0x195;
            }
            break;
        case 0x195:
            if (Hu3DMotionShiftIDGet(lbl_1_bss_114) == -1) {
                HuAudFXPanning(HuAudFXPlay(0x2CB), 0x38);
                fn_1_744C(4);
                fn_1_9D84(21);
                fn_1_744C(1);
                lbl_1_bss_184 = 0x196;
            }
            break;
        case 0x196:
            if (Hu3DMotionShiftIDGet(lbl_1_bss_114) == -1) {
                HuAudFXPanning(HuAudFXPlay(0x2C9), 0x38);
                fn_1_744C(7);
                fn_1_9D84(22);
                fn_1_744C(1);
                lbl_1_bss_17C = 0;
                lbl_1_bss_184 = 0x197;
            }
            break;
        case 0x197:
            if (lbl_1_bss_17C == 0) {
                if (Hu3DMotionShiftIDGet(lbl_1_bss_114) == -1) {
                    fn_1_744C(6);
                    fn_1_A294(lbl_1_bss_188, 0);
                    fn_1_9D84(23);
                    lbl_1_bss_1BC[0] = 0;
                    lbl_1_bss_17C = 1;
                    fn_1_744C(1);
                }
            } else {
                if (++lbl_1_bss_1BC[0] > 15) {
                    obj = Hu3DModelObjPtrGet(lbl_1_bss_140, lbl_1_data_1C5);
                    lbl_1_bss_EC = obj->mesh.curr.rot.z;
                    lbl_1_bss_1B0[1] = lbl_1_bss_EC;
                    if (lbl_1_bss_134 == 2 || lbl_1_bss_134 == 5) {
                        obj = Hu3DModelObjPtrGet(lbl_1_bss_140, lbl_1_data_1CE);
                        lbl_1_bss_F0 = obj->mesh.curr.rot.x;
                        lbl_1_bss_1B0[2] = lbl_1_bss_F0;
                    }
                    lbl_1_bss_1B0[0] = 0.0f;
                    lbl_1_bss_184 = 0x198;
                }
            }
            break;
        case 0x198:
            if (fn_1_914C() != 0) {
                lbl_1_bss_1BC[0] = 0;
                lbl_1_bss_17C = 1;
                lbl_1_bss_184 = 0x199;
            }
            if (lbl_1_bss_E8 == 5) {
                if (lbl_1_bss_134 == 2 || lbl_1_bss_134 == 5) {
                    lbl_1_bss_1B0[1] = lbl_1_bss_1B0[1] - 0.03846154f;
                    Hu3DMotionForceSet(lbl_1_bss_140, lbl_1_data_1C5, 0x200, lbl_1_bss_1B0[1]);
                    lbl_1_bss_1B0[2] = lbl_1_bss_1B0[2] - 0.07692308f;
                    Hu3DMotionForceSet(lbl_1_bss_140, lbl_1_data_1CE, 0x80, lbl_1_bss_1B0[2]);
                } else {
                    lbl_1_bss_1B0[1] = lbl_1_bss_1B0[1] - 0.115384616f;
                    Hu3DMotionForceSet(lbl_1_bss_140, lbl_1_data_1C5, 0x200, lbl_1_bss_1B0[1]);
                }
            } else {
                lbl_1_bss_1B0[0] += 0.7f;
                Hu3DModelRotSet(lbl_1_bss_140, 0.0f, -60.0f - lbl_1_bss_1B0[0], 0.0f);
            }
            break;
        case 0x199:
            if (lbl_1_bss_17C == 1) {
                if (++lbl_1_bss_1BC[0] > 65) {
                    lbl_1_bss_1B0[0] += 0.6923077f;
                    if (lbl_1_bss_1B0[0] > 90.0f) {
                        lbl_1_bss_1B0[0] = 90.0f;
                    }
                    scr = lbl_1_data_178;
                    scr.x -= 420.0f;
                    scr.x = scr.x + 300.0 * sin(M_PI * lbl_1_bss_1B0[0] / 180.0);
                    scr.y += lbl_1_bss_1B0[1];
                    if (scr.x >= -40.0f) {
                        lbl_1_bss_1B0[1] = lbl_1_bss_1B0[1] - lbl_1_bss_1B0[1] / 20.0f;
                        if (scr.y < lbl_1_data_178.y) {
                            scr.y = lbl_1_data_178.y;
                        }
                    }
                    fn_1_6CBC(&scr.x);
                    if (lbl_1_bss_1B0[0] == 90.0f) {
                        lbl_1_bss_1B0[0] = 0.0f;
                        lbl_1_bss_1B0[1] = 200.0f;
                        lbl_1_bss_17C = 2;
                    }
                } else {
                    lbl_1_bss_1B0[0] -= 0.8f;
                    if (lbl_1_bss_1B0[0] < 0.0f) {
                        lbl_1_bss_1B0[0] = 0.0f;
                    }
                    Hu3DModelRotSet(lbl_1_bss_140, 0.0f, -60.0f - lbl_1_bss_1B0[0], 0.0f);
                    if (lbl_1_bss_134 == 2 || lbl_1_bss_134 == 5) {
                        lbl_1_bss_1B0[1] = lbl_1_bss_1B0[1] + 0.15384616f;
                        if (lbl_1_bss_1B0[1] > lbl_1_bss_EC) {
                            lbl_1_bss_1B0[1] = lbl_1_bss_EC;
                        }
                        Hu3DMotionForceSet(lbl_1_bss_140, lbl_1_data_1C5, 0x200, lbl_1_bss_1B0[1]);
                        lbl_1_bss_1B0[2] = lbl_1_bss_1B0[2] + 0.30769232f;
                        if (lbl_1_bss_1B0[2] > lbl_1_bss_F0) {
                            lbl_1_bss_1B0[2] = lbl_1_bss_F0;
                        }
                        Hu3DMotionForceSet(lbl_1_bss_140, lbl_1_data_1CE, 0x80, lbl_1_bss_1B0[2]);
                    } else {
                        lbl_1_bss_1B0[1] = lbl_1_bss_1B0[1] + 0.46153846f;
                        if (lbl_1_bss_1B0[1] > lbl_1_bss_EC) {
                            lbl_1_bss_1B0[1] = lbl_1_bss_EC;
                        }
                        Hu3DMotionForceSet(lbl_1_bss_140, lbl_1_data_1C5, 0x200, lbl_1_bss_1B0[1]);
                    }
                    if (lbl_1_bss_1BC[0] == 65) {
                        lbl_1_bss_1B0[1] = 150.0f;
                    }
                }
            } else if (lbl_1_bss_17C == 2) {
                lbl_1_bss_1B0[0] += 1.0f;
                if (lbl_1_bss_1B0[0] > 90.0f) {
                    lbl_1_bss_1B0[0] = 90.0f;
                }
                scr = lbl_1_data_178;
                scr.x += 420.0f;
                scr.x = scr.x - 420.0 * sin(M_PI * lbl_1_bss_1B0[0] / 180.0);
                scr.y -= lbl_1_bss_1B0[1];
                if (scr.x <= 616.0f) {
                    lbl_1_bss_1B0[1] = lbl_1_bss_1B0[1] - lbl_1_bss_1B0[1] / 9.0f;
                    if (scr.y > lbl_1_data_178.y) {
                        scr.y = lbl_1_data_178.y;
                    }
                }
                scr.y += 30.0f;
                scr.z += 700.0f;
                fn_1_6F5C(&scr);
                Hu3DModelRotSet(lbl_1_bss_120, 0.0f, -90.0f + lbl_1_bss_1B0[0], 0.0f);
                if (lbl_1_bss_1B0[0] == 90.0f) {
                    lbl_1_bss_1BC[0] = 0;
                    lbl_1_bss_1B0[0] = 0.0f;
                    lbl_1_bss_17C = 0;
                    fn_1_6FF8(1);
                }
            } else {
                if (++lbl_1_bss_1BC[0] > 60) {
                    HuAudFXPanning(HuAudFXPlay(0x1D0), 0x38);
                    fn_1_A294(lbl_1_bss_188, 0);
                    fn_1_9D84(24);
                    lbl_1_bss_17C = 1;
                    fn_1_6D00(3);
                    lbl_1_bss_184 = 0x19A;
                } else {
                    lbl_1_bss_1B0[0] += 0.7f;
                    Hu3DModelRotSet(lbl_1_bss_140, 0.0f, -60.0f - lbl_1_bss_1B0[0], 0.0f);
                }
            }
            break;
        case 0x19A:
            if (lbl_1_bss_17C == 1) {
                HuAudFXPanning(HuAudFXPlay(0x2F1), 0x38);
                fn_1_A294(lbl_1_bss_188, 0);
                fn_1_9D84(25);
                lbl_1_bss_17C = 2;
            } else if (lbl_1_bss_17C == 2) {
                if (fn_1_96CC(1) != 0) {
                    lbl_1_bss_1BC[0] = 0;
                    lbl_1_bss_17C = 0;
                }
            } else {
                if (++lbl_1_bss_1BC[0] > 30) {
                    obj = Hu3DModelObjPtrGet(lbl_1_bss_140, lbl_1_data_1C5);
                    lbl_1_bss_EC = obj->mesh.curr.rot.z;
                    lbl_1_bss_1B0[1] = lbl_1_bss_EC;
                    if (lbl_1_bss_134 == 2 || lbl_1_bss_134 == 5) {
                        obj = Hu3DModelObjPtrGet(lbl_1_bss_140, lbl_1_data_1CE);
                        lbl_1_bss_F0 = obj->mesh.curr.rot.x;
                        lbl_1_bss_1B0[2] = lbl_1_bss_F0;
                    }
                    lbl_1_bss_1BC[0] = 0;
                    lbl_1_bss_1B0[0] = 0.0f;
                    lbl_1_bss_17C = 1;
                    fn_1_6D00(1);
                    fn_1_6FF8(2);
                    lbl_1_bss_184 = 0x19B;
                }
            }
            break;
        case 0x19B:
            if (lbl_1_bss_17C == 1) {
                if (++lbl_1_bss_1BC[0] >= 15) {
                    lbl_1_bss_1B0[0] += 2.6153846f;
                    if (lbl_1_bss_1B0[0] > 340.0f) {
                        lbl_1_bss_1B0[0] = 340.0f;
                    }
                    scr = lbl_1_data_178;
                    scr.y -= lbl_1_bss_1B0[0];
                    scr.y += 30.0f;
                    scr.z += 700.0f;
                    fn_1_6F5C(&scr);
                    if (lbl_1_bss_134 == 2 || lbl_1_bss_134 == 5) {
                        lbl_1_bss_1B0[1] = lbl_1_bss_1B0[1] - 0.07692308f;
                        if (lbl_1_bss_1B0[1] < lbl_1_bss_EC - 10.0f) {
                            lbl_1_bss_1B0[1] = lbl_1_bss_EC - 10.0f;
                        }
                        Hu3DMotionForceSet(lbl_1_bss_140, lbl_1_data_1C5, 0x200, lbl_1_bss_1B0[1]);
                        lbl_1_bss_1B0[2] = lbl_1_bss_1B0[2] - 0.07692308f;
                        if (lbl_1_bss_1B0[2] < lbl_1_bss_F0 - 10.0f) {
                            lbl_1_bss_1B0[2] = lbl_1_bss_F0 - 10.0f;
                        }
                        Hu3DMotionForceSet(lbl_1_bss_140, lbl_1_data_1CE, 0x80, lbl_1_bss_1B0[2]);
                        if (lbl_1_bss_1B0[1] == lbl_1_bss_EC - 10.0f && lbl_1_bss_1B0[2] == lbl_1_bss_F0 - 10.0f) {
                            lbl_1_bss_1BC[0] = 0;
                            lbl_1_bss_17C = 2;
                        }
                    } else {
                        lbl_1_bss_1B0[1] = lbl_1_bss_1B0[1] - 0.1923077f;
                        if (lbl_1_bss_1B0[1] < lbl_1_bss_EC - 25.0f) {
                            lbl_1_bss_1B0[1] = lbl_1_bss_EC - 25.0f;
                        }
                        Hu3DMotionForceSet(lbl_1_bss_140, lbl_1_data_1C5, 0x200, lbl_1_bss_1B0[1]);
                        if (lbl_1_bss_1B0[1] == lbl_1_bss_EC - 25.0f) {
                            lbl_1_bss_1BC[0] = 0;
                            lbl_1_bss_17C = 2;
                        }
                    }
                }
            } else if (lbl_1_bss_17C == 2) {
                if (++lbl_1_bss_1BC[0] > 30) {
                    if (lbl_1_bss_134 == 2 || lbl_1_bss_134 == 5) {
                        lbl_1_bss_1B0[1] = lbl_1_bss_1B0[1] + 0.115384616f;
                        if (lbl_1_bss_1B0[1] > lbl_1_bss_EC) {
                            lbl_1_bss_1B0[1] = lbl_1_bss_EC;
                        }
                        Hu3DMotionForceSet(lbl_1_bss_140, lbl_1_data_1C5, 0x200, lbl_1_bss_1B0[1]);
                        if (lbl_1_bss_1B0[1] == lbl_1_bss_EC) {
                            lbl_1_bss_17C = 0;
                            lbl_1_bss_1B0[0] = 42.0f;
                        }
                        lbl_1_bss_1B0[2] = lbl_1_bss_1B0[2] + 0.115384616f;
                        if (lbl_1_bss_1B0[2] > lbl_1_bss_F0) {
                            lbl_1_bss_1B0[2] = lbl_1_bss_F0;
                        }
                        Hu3DMotionForceSet(lbl_1_bss_140, lbl_1_data_1CE, 0x80, lbl_1_bss_1B0[2]);
                    } else {
                        lbl_1_bss_1B0[1] = lbl_1_bss_1B0[1] + 0.23076923f;
                        if (lbl_1_bss_1B0[1] > lbl_1_bss_EC) {
                            lbl_1_bss_1B0[1] = lbl_1_bss_EC;
                        }
                        Hu3DMotionForceSet(lbl_1_bss_140, lbl_1_data_1C5, 0x200, lbl_1_bss_1B0[1]);
                        if (lbl_1_bss_1B0[1] == lbl_1_bss_EC) {
                            lbl_1_bss_17C = 0;
                            lbl_1_bss_1B0[0] = 42.0f;
                        }
                    }
                }
            } else {
                lbl_1_bss_1B0[0] -= 0.7f;
                if (lbl_1_bss_1B0[0] < 0.0f) {
                    lbl_1_bss_1B0[0] = 0.0f;
                }
                Hu3DModelRotSet(lbl_1_bss_140, 0.0f, -60.0f - lbl_1_bss_1B0[0], 0.0f);
                if (lbl_1_bss_1B0[0] == 0.0f) {
                    fn_1_6D00(2);
                    lbl_1_bss_184 = 0x19C;
                }
            }
            break;
        case 0x19C:
            fn_1_A294(lbl_1_bss_188, 0);
            fn_1_9D84(26);
            fn_1_6D00(3);
            lbl_1_bss_184 = 0x19D;
            break;
        case 0x19D:
            fn_1_9D84(27);
            lbl_1_bss_184 = 0x19E;
            break;
        case 0x19E:
            fn_1_9D84(28);
            lbl_1_bss_1BC[0] = 0;
            fn_1_6D00(1);
            lbl_1_bss_184 = 0x19F;
            break;
        case 0x19F:
            if (++lbl_1_bss_1BC[0] > 90) {
                fn_1_9D84(29);
                lbl_1_bss_184 = 0x1A0;
            }
            break;
        case 0x1A0:
            fn_1_9D84(30);
            lbl_1_bss_184 = 0x1A1;
            break;
        case 0x1A1:
            fn_1_9D84(31);
            lbl_1_bss_184 = 0x1A2;
            Hu3DCameraPosGet(2, &lbl_1_bss_18C[0], &lbl_1_bss_18C[2], &lbl_1_bss_18C[1]);
            lbl_1_bss_1BC[0] = 0;
            lbl_1_bss_1B0[0] = 0.0f;
            lbl_1_bss_1B0[1] = 0.0f;
            break;
        case 0x1A2:
            if (++lbl_1_bss_1BC[0] > 15) {
                degB = 180.0f;
                vD = Hu3DData[lbl_1_bss_140].pos;
                vE = lbl_1_bss_18C[0];
                vF = lbl_1_bss_18C[1];
                vE.x = vE.x + (vD.x - lbl_1_bss_18C[0].x) * sin(M_PI * lbl_1_bss_1B0[0] / 180.0);
                if (lbl_1_bss_134 != 3) {
                    vE.y = vE.y - 15.0 * sin(M_PI * lbl_1_bss_1B0[0] / 180.0);
                } else {
                    vE.y = vE.y - -150.0 * sin(M_PI * lbl_1_bss_1B0[0] / 180.0);
                }
                vE.z = vE.z - 600.0 * sin(M_PI * lbl_1_bss_1B0[0] / 180.0);
                vF.x += vE.x;
                vF.y = vF.y - 20.0 * sin(M_PI * lbl_1_bss_1B0[0] / 180.0);
                Hu3DCameraPosSetV(1, &vE, &lbl_1_bss_18C[2], &vF);
                Hu3DCameraPosSetV(2, &vE, &lbl_1_bss_18C[2], &vF);
                Hu3DCameraPosSetV(4, &vE, &lbl_1_bss_18C[2], &vF);
                lbl_1_bss_1B0[1] = 60.0 * sin(M_PI * lbl_1_bss_1B0[0] / 180.0);
                Hu3DModelRotSet(lbl_1_bss_140, 0.0f, -60.0f + lbl_1_bss_1B0[1], 0.0f);
                lbl_1_bss_1B0[0] += 90.0f / degB;
                if (lbl_1_bss_1B0[0] >= 90.0f) {
                    fn_1_69CC(4);
                    ret = 1;
                    lbl_1_bss_184 = 1000;
                }
            }
            break;
        case 0x1F4:
            if (fn_1_9EE8(0x20) != 0) {
                lbl_1_bss_184 = lbl_1_bss_180;
            } else {
                ret = -1;
                lbl_1_bss_184 = 1000;
            }
            break;
    }
    return ret;
}

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

static void fn_1_A294(s32 index, s32 unused)
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
