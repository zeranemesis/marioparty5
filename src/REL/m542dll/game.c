#include "types.h"

void MgSeqModeNext(void);
void HuAudSStreamFadeOut(s32 streamId, s32 fadeOutTime);
void fn_1_39E0(void);
void fn_1_7848(s32 slotIdx);

typedef struct MgSlot_s {
    s32 state;
    s32 timer;
    s32 unk8;
    s32 unkC;
    f32 f10;
    f32 f14;
    f32 f18;
    f32 f1C;
} MgSlot;

typedef struct MgSeqData_s {
    s32 v[85];
} MgSeqData;

typedef struct MgMotion_s {
    u8 pad0[0x1A4];
    s16 unk1A4;
    u8 pad1A6[0x1A];
    s16 unk1C0;
} MgMotion;

void Hu3DMotionSpeedSet(s16 motionId, f32 speed);

s32 g_mgAudStreamId = -1;
MgSlot g_mgSlot[4];
MgSeqData g_mgSeqData;

void fn_1_2978(void)
{
    if (g_mgSeqData.v[41]++ >= 45) {
        g_mgSeqData.v[40]++;
        g_mgSeqData.v[41] = 0;
        Hu3DMotionSpeedSet(((MgMotion *)g_mgSeqData.v[15])->unk1A4, 1.0f);
        Hu3DMotionSpeedSet(((MgMotion *)g_mgSeqData.v[15])->unk1C0, 1.0f);
    }
}

void fn_1_2F90(void)
{
    g_mgSeqData.v[40] = 0;
    MgSeqModeNext();
    HuAudSStreamFadeOut(g_mgAudStreamId, 100);
    fn_1_39E0();
}

void fn_1_771C(s32 arg0)
{
    s32 i;
    s32 lim;

    i = arg0;
    lim = i + 1;
    if (i < 0) {
        i = 0;
        lim = 4;
    }
    for (; i < lim; i++) {
        g_mgSlot[arg0].timer = 0;
    }
}

void fn_1_7778(s32 arg0)
{
    if (arg0 < 4) {
        g_mgSlot[arg0].state = 1;
        g_mgSlot[arg0].timer = 0;
    } else {
        g_mgSlot[arg0 - 4].state = 1;
        g_mgSlot[arg0 - 4].timer = arg0;
    }
}

void fn_1_77EC(void)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        if (g_mgSlot[i].state != 0) {
            fn_1_7848(i);
        }
    }
}
