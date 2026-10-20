#include <dolphin/types.h>

extern s32 lbl_1_data_2CC[10];
extern u8 lbl_1_bss_60[0x3C];
extern s16 lbl_1_bss_114;
extern s16 lbl_1_bss_E2;
extern s16 lbl_1_bss_12E;
extern s16 lbl_1_bss_124;
extern s16 lbl_1_bss_D0;
extern s16 lbl_1_bss_B8;
extern s16 lbl_1_bss_E8;
extern s16 lbl_1_bss_128;
extern s32 lbl_1_bss_C;
extern u8 lbl_1_rodata_A4[4];
extern u8 lbl_1_rodata_168[4];

extern void Hu3DParManKill(s16 id);
extern void Hu3D2Dto3D(f32* pos2d, s32 mode, f32* pos3d);
extern void Hu3DModelPosSetV(s16 modelId, f32* pos);
extern void Hu3DMotionShiftSet(s16 modelId, s16 motionId, f32 startFrame, f32 endFrame, u32 flags);

void fn_1_A294(s32 index)
{
    *(s32 *)&lbl_1_bss_60[8] = lbl_1_data_2CC[index];
}

void fn_1_A764(s32* ptr)
{
    s32 val = *ptr;
    Hu3DParManKill((s16)val);
}

void fn_1_7408(f32* pos2d)
{
    f32 pos3d[3];
    Hu3D2Dto3D(pos2d, 2, pos3d);
    Hu3DModelPosSetV(lbl_1_bss_114, pos3d);
}

void fn_1_7DE0(f32* pos2d)
{
    f32 pos3d[3];
    Hu3D2Dto3D(pos2d, 2, pos3d);
    Hu3DModelPosSetV(lbl_1_bss_E2, pos3d);
}

void fn_1_6CBC(f32* pos2d)
{
    f32 pos3d[3];
    Hu3D2Dto3D(pos2d, 4, pos3d);
    Hu3DModelPosSetV(lbl_1_bss_12E, pos3d);
}

void fn_1_A5D0(s16* ids)
{
    Hu3DParManKill(ids[0]);
    Hu3DParManKill(ids[1]);
    Hu3DParManKill(ids[2]);
}