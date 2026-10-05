#include "dolphin.h"

typedef struct MdDebugFont {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    float unk1C;
} MdDebugFont;

typedef struct MdCamera {
    char unk0[0x44];
    float pos[3];
    float unk50;
    float pos2[3];
} MdCamera;

void MdDebugFontUpdate(MdDebugFont *font)
{
    font->unk1A = 0;
    font->unk16 = font->unk14;
}

void MdMathLerp(float *out, float start, float end, float t)
{
    *out = start + t * (end - start);
}

void MdMathLerpOfs(float *out, float start, float end, float a, float b)
{
    *out = start + (end - start) * (a + b);
}

void MdCameraPosSet(MdCamera *cam, float x1, float y1, float z1, float x2, float y2, float z2)
{
    cam->pos[0] = x1;
    cam->pos[1] = y1;
    cam->pos[2] = z1;
    cam->pos2[0] = x2;
    cam->pos2[1] = y2;
    cam->pos2[2] = z2;
}

void HuWinAllKill(void);

void MdMessKill(void)
{
    HuWinAllKill();
}

// NONMATCHING: extra clrlwi r0, r0, 24 before stb
void MdMathLerpColor(u8 *out, float start, float end, float t)
{
    *out = (u8)(s32)(start + t * (end - start));
}