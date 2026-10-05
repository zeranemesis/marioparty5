#include "types.h"

struct MdDebugFont {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0C;
    s16 unk_10;
    s16 unk_12;
    s16 unk_14;
    s16 unk_16;
    s16 unk_18;
    s16 unk_1A;
    f32 unk_1C;
};

void MdDebugFontUpdate(struct MdDebugFont *font)
{
    font->unk_1A = 0;
    font->unk_16 = font->unk_14;
}

void MdMathLerp(f32 *out, f32 start, f32 end, f32 t)
{
    *out = start + t * (end - start);
}

void MdMathLerpOfs(f32 *out, f32 start, f32 end, f32 t, f32 ofs)
{
    *out = start + (end - start) * (t + ofs);
}

void MdCameraPosSet(void *camera, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2)
{
    ((f32 *)camera)[0x44 / 4] = x1;
    ((f32 *)camera)[0x48 / 4] = y1;
    ((f32 *)camera)[0x4C / 4] = z1;
    ((f32 *)camera)[0x54 / 4] = x2;
    ((f32 *)camera)[0x58 / 4] = y2;
    ((f32 *)camera)[0x5C / 4] = z2;
}

extern void HuWinAllKill(void);

void MdMessKill(void)
{
    HuWinAllKill();
}

void MdMathLerpColor(u8 *out, f32 start, f32 end, f32 t)
{
    *out = (u8)(start + t * (end - start));
}