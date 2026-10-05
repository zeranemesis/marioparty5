#include "types.h"

typedef s16 HU3DMODELID;

extern s16 CharMotionCreate(s16 charNo, unsigned int dataNum);
extern void Hu3DMotionSpeedSet(HU3DMODELID modelId, float speed);
extern void Hu3DMotionTimeSet(HU3DMODELID modelId, float time);

extern void fn_1_8944(void);
extern void fn_1_8F7C(void);
extern void fn_1_94C8(void);
extern void fn_1_9DAC(void);
void fn_1_A7B8(void);

typedef struct {
    s16 motionId;
    f32 speed;
} MgStageChar;

typedef struct {
    u8 pad00[6];
    s16 charId;
    u8 pad08[0x14];
} MgStageSceneryModel;

typedef struct {
    MgStageSceneryModel models[2];
    u8 pad38[4];
    MgStageChar *unk3C;
    s32 motionData[2];
    s32 motionId[2];
    u8 pad50[0xEC];
    s32 unk13C;
    u8 pad140[0xB4];
    s32 unk1F4;
    u8 pad1F8[4];
    s32 unk1FC;
} MgStageScenery;

typedef void (*MgStageSceneryFn)(void);

MgStageScenery *lbl_1_bss_9B8;
MgStageSceneryFn lbl_1_data_3A0[5] = {
    fn_1_8944,
    fn_1_8F7C,
    fn_1_94C8,
    fn_1_9DAC,
    fn_1_A7B8
};

void fn_1_7CE4(s32 a, s32 b)
{
    lbl_1_bss_9B8->motionData[0] = a;
    lbl_1_bss_9B8->motionData[1] = b;
}

void fn_1_7D08(s32 a, s32 b)
{
    lbl_1_bss_9B8->motionId[0] = a;
    lbl_1_bss_9B8->motionId[1] = b;
}

void fn_1_8454(void)
{
    s16 i;
    s16 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 5; j++) {
            ((s16 *)lbl_1_bss_9B8->motionId[i])[j] =
                CharMotionCreate(lbl_1_bss_9B8->models[i].charId,
                                 ((s32 *)lbl_1_bss_9B8->motionData[i])[j]);
        }
    }

    for (i = 0; i < 2; i++) {
        *(s16 *)((u8 *)lbl_1_bss_9B8 + i * 0x44 + 0x52) = -1;
        *(f32 *)((u8 *)lbl_1_bss_9B8 + i * 0x44 + 0x58) = -1.0f;
    }
}

void fn_1_88F0(void)
{
    s32 i;

    for (i = 0; i < 5; i++) {
        lbl_1_data_3A0[i]();
    }
}

void fn_1_A7B8(void)
{
    MgStageChar *chr;

    chr = (MgStageChar *)((u8 *)lbl_1_bss_9B8->unk3C + 0x364);

    switch (lbl_1_bss_9B8->unk1FC) {
    case 0:
        if (lbl_1_bss_9B8->unk13C == 2) {
            lbl_1_bss_9B8->unk1FC++;
            lbl_1_bss_9B8->unk1F4 = 0;
            Hu3DMotionSpeedSet(chr->motionId, 0.5f);
            Hu3DMotionTimeSet(chr->motionId, 0.0f);
        }
        break;
    case 1:
        lbl_1_bss_9B8->unk1F4++;
        chr->speed = (f32)(lbl_1_bss_9B8->unk1F4 * 4000 / 200 - 2000);
        if (lbl_1_bss_9B8->unk1F4 == 200) {
            lbl_1_bss_9B8->unk1FC++;
        }
        break;
    case 2:
        if (lbl_1_bss_9B8->unk13C == 1) {
            lbl_1_bss_9B8->unk1FC = 0;
        }
        break;
    }
}
