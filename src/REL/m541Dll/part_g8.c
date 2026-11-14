#include "REL/m541Dll.h"

#include <math.h>

/* Raw views of lbl_1_bss_45C (the header names only part of it). */
#define B45C ((u8 *)&lbl_1_bss_45C)
#define W16(off) (*(s16 *)(B45C + (off)))
#define VEC(off, i) (*(HuVecF *)(B45C + (off) + (i) * 0xC))

#define P_CNT W16(0x6)
#define P_SPEED (*(float *)(B45C + 0x8))
#define P_STATE W16(0x2004)
#define P_TIMER W16(0x2006)
#define P_NUM W16(0x2008)
#define P_ACTIVE W16(0x200A)
#define P_SEL(i) W16(0x202A + (i) * 2)
#define P_TMR(i) W16(0x2052 + (i) * 2)
#define P_LEAFPOS(i) VEC(0x1C8C, i)
#define P_LEAFVEL(i) VEC(0x1DA0, i)
#define P_LEAFVELSAVE(i) VEC(0x1EB4, i)

void fn_1_194B4(void)
{
    s16 i;
    s32 done;

    switch (W16(0x2178)) {
        case 0:
            if (W16(0x2176) >= 600 && (W16(0x2176) % 300) == 0 && P_CNT < 15) {
                W16(0x2178)++;
            }
            break;
        case 1:
            done = fn_1_19738(P_CNT, 0.0f, 0.0f);
            if ((s16)done != 0) {
                for (i = 0; i < 30; i++) {
                    VEC(0xC, P_CNT * 30 + i) = P_LEAFPOS(P_CNT);
                }
                W16(0x17F4 + P_CNT * 0x3C) = 0;
                W16(0x17F6 + P_CNT * 0x3C) = 29;
                W16(0x1BF0 + P_CNT * 2) = 2;
                W16(0x1C4E + P_CNT * 2) = 0;
                W16(0x1C12 + P_CNT * 2) = 29;
                W16(0x1C30 + P_CNT * 2) = 0;
                P_CNT++;
                W16(0x2178) = 0;
            }
            break;
    }
    W16(0x2176)++;
}

s32 fn_1_19738(s16 idx, float baseX, float baseZ)
{
    s32 ret = 0;
    float ang;
    float len;

    switch (W16(0x2066 + idx * 2)) {
        case 0:
            W16(0x2084 + idx * 2) = 1;
            ang = *(float *)(B45C + 0x20A4 + idx * 0xC);
            len = *(float *)(B45C + 0x20A8 + idx * 0xC);
            *(float *)(B45C + 0x1C8C + idx * 0xC) = baseX + len * cos(3.141592653589793 * ang / 180.0);
            *(float *)(B45C + 0x1C90 + idx * 0xC) = 0.0f;
            *(float *)(B45C + 0x1C94 + idx * 0xC) = baseZ + len * sin(3.141592653589793 * ang / 180.0);
            W16(0x2158 + idx * 2) = 0;
            W16(0x2066 + idx * 2)++;
            break;
        case 1:
            W16(0x2158 + idx * 2)++;
            if (W16(0x2158 + idx * 2) >= 90) {
                *(float *)(B45C + 0x1FC8 + idx * 4) = *(float *)(B45C + 0x20AC + idx * 0xC);
                *(float *)(B45C + 0x1DA0 + idx * 0xC) = P_SPEED * cos(3.141592653589793 * *(float *)(B45C + 0x1FC8 + idx * 4) / 180.0);
                *(float *)(B45C + 0x1DA4 + idx * 0xC) = 0.0f;
                *(float *)(B45C + 0x1DA8 + idx * 0xC) = P_SPEED * sin(3.141592653589793 * *(float *)(B45C + 0x1FC8 + idx * 4) / 180.0);
                W16(0x2084 + idx * 2) = 0;
                W16(0x2066 + idx * 2) = 1;
                ret = 1;
            }
            break;
    }
    return ret;
}

/* distance between leaves a and b */
#define LEAFDIST(a, b, out) \
    do { \
        float dx_ = P_LEAFPOS(a).x - P_LEAFPOS(b).x; \
        float dz_ = P_LEAFPOS(a).z - P_LEAFPOS(b).z; \
        out = sqrt(dx_ * dx_ + dz_ * dz_); \
    } while (0)

s32 fn_1_187F8(void)
{
    float range[22] = { 0.0f, 0.0f, 400.0f, 650.0f, 350.0f, 600.0f, 300.0f, 550.0f,
                        250.0f, 500.0f, 200.0f, 450.0f, 150.0f, 400.0f, 100.0f, 350.0f,
                        50.0f, 300.0f, 0.0f, 250.0f, 0.0f, 200.0f };
    s32 ret = 0;
    s16 found;
    s16 k, v, c;
    float d;

    found = 0;
    switch (P_STATE) {
        case 0:
            P_TIMER = P_TMR(P_NUM);
            P_NUM++;
            P_STATE++;
            break;
        case 1:
            if (P_TIMER-- < 0) {
                P_TIMER = 0;
                P_STATE++;
            }
            W16(0x2180)++;
            break;
        case 2:
            W16(0x217A)++;
            for (k = 0; k < 10; k++) {
                P_SEL(k) = -1;
            }
            P_SEL(0) = W16(0x203E + P_NUM * 2) + 15;
            for (k = 1; k <= P_NUM; k++) {
                for (c = 0; c < P_CNT; c++) {
                    found = 0;
                    for (v = 0; v < 10; v++) {
                        if (P_SEL(v) == c) {
                            found = 1;
                        }
                    }
                    if (found == 0) {
                        LEAFDIST(c, P_SEL(k - 1), d);
                        if (range[P_NUM * 2] < d && range[P_NUM * 2 + 1] > d) {
                            P_SEL(k) = c;
                            break;
                        }
                    }
                }
                if (P_SEL(k) == -1) {
                    found = 1;
                    break;
                }
            }
            if (found == 0) {
                for (c = 15; c < 23; c++) {
                    if (P_SEL(0) != c) {
                        LEAFDIST(c, P_SEL(P_NUM), d);
                        if (range[P_NUM * 2] < d && range[P_NUM * 2 + 1] > d) {
                            P_SEL(P_NUM + 1) = c;
                            break;
                        }
                    }
                }
                for (k = 0; k <= P_NUM + 1; k++) {
                    if (P_SEL(k) == -1) {
                        k = 99;
                        for (v = 0; v < 10; v++) {
                            P_SEL(v) = -1;
                        }
                    }
                }
                if (P_SEL(0) != -1) {
                    P_STATE++;
                }
            }
            break;
        case 3:
            for (k = 1; k < P_NUM + 1; k++) {
                W16(0x200C + P_SEL(k) * 2) = 1;
                P_LEAFVELSAVE(P_SEL(k)) = P_LEAFVEL(P_SEL(k));
                P_LEAFVEL(P_SEL(k)).x = 0.0f;
                P_LEAFVEL(P_SEL(k)).y = 0.0f;
                P_LEAFVEL(P_SEL(k)).z = 0.0f;
            }
            P_TIMER = 60;
            P_STATE++;
            break;
        case 4:
            if (P_TIMER-- < 0) {
                for (k = 0; k <= P_NUM + 1; k++) {
                    VEC(0x168C, k) = P_LEAFPOS(P_SEL(k));
                    W16(0x1BB4 + k * 2) = k;
                }
                W16(0x1C10) = P_NUM + 2;
                P_ACTIVE = 1;
                P_TIMER = 120;
                P_STATE++;
            }
            break;
        case 5:
            if (P_TIMER-- < 0) {
                for (k = 1; k < P_NUM + 1; k++) {
                    W16(0x200C + P_SEL(k) * 2) = 0;
                    P_LEAFVEL(P_SEL(k)) = P_LEAFVELSAVE(P_SEL(k));
                }
                P_ACTIVE = 0;
                P_STATE = 0;
                ret = 1;
            }
            break;
    }
    return ret;
}
