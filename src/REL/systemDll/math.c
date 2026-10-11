#include "types.h"

void MdMathLerp(float *out, float a, float b, float t)
{
    *out = a + t * (b - a);
}

void MathLerpColor(u8 *out, float a, float b, float t)
{
    // NONMATCHING: the original stores the converted int with a bare stb (the low
    // byte); MWCC inserts an extra clrlwi mask before the store.
    *out = (s32)(a + t * (b - a));
}

void MdMathLerpOfs(float *out, float a, float b, float t, float ofs)
{
    *out = a + (b - a) * (t + ofs);
}

void MdMathClamp(float *val, float min, float max)
{
    if (*val <= min) {
        *val = min;
    }
    if (*val >= max) {
        *val = max;
    }
}

void MdMathLerpScale(float *out, float a, float b, float t)
{
    float v = a + t * (b - a);

    out[0] = v;
    out[1] = v;
    out[2] = v;
}

void MdMathLerpScaleOfs(float *out, float a, float b, float t, float ofs)
{
    float v = a + (b - a) * (t + ofs);

    out[0] = v;
    out[1] = v;
    out[2] = v;
}

static const float lbl_1_rodata_374 = 180.0f;
static const float lbl_1_rodata_378 = -180.0f;

void MdMathAngleClamp(float *angle)
{
    if (*angle > lbl_1_rodata_374) {
        *angle = lbl_1_rodata_378 + (*angle - lbl_1_rodata_374);
    } else if (*angle < lbl_1_rodata_378) {
        *angle = lbl_1_rodata_374 + *angle + lbl_1_rodata_374;
    }
}
