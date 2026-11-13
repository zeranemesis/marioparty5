#include "REL/m541Dll.h"

/* lbl_1_bss_45C holds 17 paths: 15 leaf trails followed by the two spawn paths (simSpawn[0..29], [30..59]).
 * G7PATHS is the flat [17][30] view of that memory that the original code indexes. */
typedef struct G7Paths_s {
    s16 hdr[6];         /* 0x0000 */
    HuVecF trail[17][30]; /* 0x000C */
    s16 hist[17][30];   /* 0x17F4 */
    s16 histCount[17];  /* 0x1BF0 */
} G7PATHS;
#define G7_PATHS (*(G7PATHS *)&lbl_1_bss_45C)
#define G7_TRAIL G7_PATHS.trail
#define G7_HIST G7_PATHS.hist
#define G7_HCNT G7_PATHS.histCount
/* point k of path i */
#define G7_PT(i, k) G7_TRAIL[i][G7_HIST[i][k]]

/* absolute value as the original macro: argument is evaluated twice */
#define G7_ABS(e) ((e) > 0.0f ? (e) : -1.0f * (e))

/* "is the point in front of the ray" flags: fx for the x axis, fz for the z axis (angle in degrees) */
#define G7_SIDE(fx, fz)                                                    \
    if ((ang >= 0.0f && ang < 90.0f) || (ang > 270.0f && ang <= 360.0f)) { \
        if (dxp + lim > 0.0f) {                                            \
            fx = 1.0f;                                                     \
        }                                                                  \
    } else if (ang == 90.0f || ang == 270.0f) {                            \
        fx = 1.0f;                                                         \
    } else if (dxp - lim < 0.0f) {                                         \
        fx = 1.0f;                                                         \
    }                                                                      \
    if (ang > 0.0f && ang < 180.0f) {                                      \
        if (dzp + lim > 0.0f) {                                            \
            fz = 1.0f;                                                     \
        }                                                                  \
    } else if (ang == 0.0f || ang == 180.0f || ang == 360.0f) {            \
        fz = 1.0f;                                                         \
    } else if (dzp - lim < 0.0f) {                                         \
        fz = 1.0f;                                                         \
    }

s32 fn_1_14714(s16 idx)
{
    float rad = 30.0f;
    float margin = 30.000002f;
    s16 i;
    s16 j;
    s32 ret = 0;
    float minX, maxX, minZ, maxZ;
    float dx, dz, t, u, dist;
    float lenSq;

    for (i = 0; i < lbl_1_bss_45C.simCount; i++) {
        for (j = 0; j < G7_HCNT[i] - 1; j++) {
            if (G7_TRAIL[i][j].x < G7_TRAIL[i][j + 1].x) {
                minX = G7_TRAIL[i][j].x;
                maxX = G7_TRAIL[i][j + 1].x;
            } else {
                minX = G7_TRAIL[i][j + 1].x;
                maxX = G7_TRAIL[i][j].x;
            }
            if (G7_TRAIL[i][j].z < G7_TRAIL[i][j + 1].z) {
                minZ = G7_TRAIL[i][j].z;
                maxZ = G7_TRAIL[i][j + 1].z;
            } else {
                minZ = G7_TRAIL[i][j + 1].z;
                maxZ = G7_TRAIL[i][j].z;
            }
            if (rad + lbl_1_bss_2614[idx].pos.x >= minX && lbl_1_bss_2614[idx].pos.x - rad <= maxX
                && rad + lbl_1_bss_2614[idx].pos.z >= minZ && lbl_1_bss_2614[idx].pos.z - rad <= maxZ) {
                dx = G7_TRAIL[i][j].x - G7_TRAIL[i][j + 1].x;
                dz = G7_TRAIL[i][j].z - G7_TRAIL[i][j + 1].z;
                lenSq = dx * dx + dz * dz;
                if (0.0f == lenSq) {
                    lenSq = 1.0f;
                }
                t = -1.0f
                    * (dx * (G7_TRAIL[i][j].x - lbl_1_bss_2614[idx].pos.x) + dz * (G7_TRAIL[i][j].z - lbl_1_bss_2614[idx].pos.z));
                u = t / lenSq;
                minX = G7_TRAIL[i][j].x + dx * u;
                minZ = G7_TRAIL[i][j].z + dz * u;
                dx = lbl_1_bss_2614[idx].pos.x - minX;
                dz = lbl_1_bss_2614[idx].pos.z - minZ;
                dist = sqrt(dx * dx + dz * dz);
                if (dist < rad + margin) {
                    ret = 1;
                }
            }
        }
    }
    return ret;
}
