#include "dolphin/types.h"
#include "game/window.h"

void MdObjMessMain(void *obj)
{
    u32 val;
    val = *(u32 *)((u8 *)obj + 0x5C);
}

void MdDebugFontUpdate(void *obj)
{
    *(u16 *)((u8 *)obj + 0x1A) = 0;
    *(s16 *)((u8 *)obj + 0x16) = *(s16 *)((u8 *)obj + 0x14);
}

void MdCameraPosSet(void *obj, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f)
{
    *(f32 *)((u8 *)obj + 0x44) = a;
    *(f32 *)((u8 *)obj + 0x48) = b;
    *(f32 *)((u8 *)obj + 0x4C) = c;
    *(f32 *)((u8 *)obj + 0x54) = d;
    *(f32 *)((u8 *)obj + 0x58) = e;
    *(f32 *)((u8 *)obj + 0x5C) = f;
}

void MdMathLerp(f32 *out, f32 a, f32 b, f32 t)
{
    *out = a + t * (b - a);
}

void MdMathLerpOfs(f32 *out, f32 a, f32 b, f32 t, f32 ofs)
{
    *out = a + (b - a) * (t + ofs);
}

void MdMessKill(void)
{
    HuWinAllKill();
}