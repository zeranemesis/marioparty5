#include "dolphin.h"

u8 lbl_801D7FF0[64] = {
    0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x4B, 0x4C, 0x4D, 0x4E, 0x50, 0x51, 0x52, 0x53,
    0x54, 0x55, 0x56, 0x57, 0x59, 0x5A, 0x91, 0x92,
    0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A,
    0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2,
    0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA,
    0xAB, 0xAC, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3,
    0xB4, 0xB5, 0xB6, 0xB7, 0xB9, 0xBA, 0xBC, 0xBD,
};

s32 lbl_801D8030[24] = {
    13, 8, 16, 2, 18, 4, 22, 9,
    20, 17, 6, 15, 11, 23, 5, 10,
    0, 12, 3, 14, 7, 1, 21, 19,
};

u8 lbl_80287F20[0xE0];

void fn_801A80A8(void);

void fn_801A7E90(u32 value, u8 *out)
{
    u32 code;
    u32 bits;
    u32 sum;
    u32 packed;
    s32 i;

    code = value + 0x18EF;
    bits = code;
    sum = 0;
    for (i = 0; i < 7; i++) {
        sum += bits & 3;
        bits >>= 2;
    }
    sum &= 3;
    bits = code + (sum << 14);
    sum = (u8)((bits & 0xFF) + ((bits >> 8) & 0xFF));
    bits += sum << 16;
    packed = 0;
    for (i = 0; i < 24; i++) {
        packed |= (bits & 1) << lbl_801D8030[i];
        bits >>= 1;
    }
    for (i = 0; i < 4; i++) {
        out[i] = lbl_801D7FF0[packed & 0x3F];
        packed >>= 6;
    }
}

s32 fn_801A7F78(u8 *in)
{
    s32 i;
    u32 bits;
    u32 packed;
    u32 sum;
    u32 c;

    fn_801A80A8();
    packed = 0;
    for (i = 0; i < 4; i++) {
        packed <<= 6;
        c = lbl_80287F20[*(in + 3 - i) - 0x20];
        if (c > 64) {
            return -1;
        }
        packed |= c;
    }
    bits = 0;
    for (i = 23; i >= 0; i--) {
        bits <<= 1;
        bits |= (packed >> lbl_801D8030[i]) & 1;
    }
    sum = (u8)((bits & 0xFF) + ((bits >> 8) & 0xFF));
    if (sum != ((bits >> 16) & 0xFF)) {
        return -1;
    }
    bits &= 0xFFFF;
    packed = bits;
    sum = 0;
    for (i = 0; i < 7; i++) {
        sum += bits & 3;
        bits >>= 2;
    }
    sum &= 3;
    if (sum != ((packed >> 14) & 3)) {
        return -1;
    }
    return (packed & 0x3FFF) - 0x18EF;
}

void fn_801A80A8(void)
{
    s32 i;

    for (i = 0; i < 0xE0; i++) {
        lbl_80287F20[i] = 0xFF;
    }
    for (i = 0; i < 64; i++) {
        lbl_80287F20[lbl_801D7FF0[i] - 0x20] = i;
    }
}
