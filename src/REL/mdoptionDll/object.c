#include "game/object.h"

typedef struct MdDebugFontData_s {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0C;
    u16 unk_10;
    u16 unk_12;
    s16 unk_14;
    s16 unk_16;
    u16 unk_18;
    u16 unk_1A;
    float unk_1C;
} MDDEBUGFONT_DATA;

void MdObjMessMain(OMOBJ *obj)
{
    void *data = obj->data;
    (void)data;
}
// NONMATCHING: r0 vs r31 usage, stack store offset differs

void MdDebugFontUpdate(MDDEBUGFONT_DATA *data)
{
    data->unk_1A = 0;
    data->unk_16 = data->unk_14;
}

void MdMathLerp(float *out, float start, float end, float t)
{
    *out = start + t * (end - start);
}

void MdMathLerpOfs(float *out, float start, float end, float t, float ofs)
{
    *out = start + (end - start) * (t + ofs);
}

typedef struct MdCamera_s {
    u8 pad_000[0x44];
    float unk_44;
    float unk_48;
    float unk_4C;
    u8 pad_50[0x4];
    float unk_54;
    float unk_58;
    float unk_5C;
} MDCAMERA;

void MdCameraPosSet(MDCAMERA *cam, float a, float b, float c, float d, float e, float f)
{
    cam->unk_44 = a;
    cam->unk_48 = b;
    cam->unk_4C = c;
    cam->unk_54 = d;
    cam->unk_58 = e;
    cam->unk_5C = f;
}

extern void HuWinAllKill(void);

void MdMessKill(void)
{
    HuWinAllKill();
}