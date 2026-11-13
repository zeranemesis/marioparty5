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
#define G7_SIDE(fx, fz, xp, xm, zp, zm)                                                 \
    if ((ang >= 0.0f && ang < 90.0f) || (ang > 270.0f && ang <= 360.0f)) { \
        if ((xp) > 0.0f) {                                            \
            fx = 1.0f;                                                     \
        }                                                                  \
    } else if (ang == 90.0f || ang == 270.0f) {                            \
        fx = 1.0f;                                                         \
    } else if ((xm) < 0.0f) {                                         \
        fx = 1.0f;                                                         \
    }                                                                      \
    if (ang > 0.0f && ang < 180.0f) {                                      \
        if ((zp) > 0.0f) {                                            \
            fz = 1.0f;                                                     \
        }                                                                  \
    } else if (ang == 0.0f || ang == 180.0f || ang == 360.0f) {            \
        fz = 1.0f;                                                         \
    } else if ((zm) < 0.0f) {                                         \
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

s32 fn_1_14EB0(s16 idx)
{
    HuVec2f dist[36];
    HuVecF hit[36];
    s16 seg[36][2];
    s16 list[36];
    s16 dead198[36];
    s16 flg[36];
    float scl = 60.0f;
    float c30 = 30.0f;
    float c30b = 30.000002f;
    float lim = 90.0f;
    s32 ret = 0;
    s16 cnt = 0;
    s16 i;
    s16 j;
    s16 n;
    s16 found;
    s16 cand;
    s16 nearFlag;
    float px, pz;
    float ang, step;
    float p2x, p2z, cosA, sinA;
    float a1, b1, c1;
    float lenA, lenB;
    float f31, f30, f29, f28, f22;
    float a2, b2, c2;
    float f19, f18;
    float margin;
    float maxX, maxZ;
    float prod;

    for (i = 0; i < 36; i++) {
        dist[i].x = 2000.0f;
        dist[i].y = 2000.0f;
        dead198[i] = 0;
        flg[i] = 0;
    }
    px = lbl_1_bss_2614[idx].pos.x;
    pz = lbl_1_bss_2614[idx].pos.z;
    ang = 0.0f;
    n = 0;
    step = 10.0f;
    for (; ang < 360.0f; ang += step, n++) {
        p2x = px + 10.0 * HuCos(ang);
        p2z = pz + 10.0 * HuSin(ang);
        cosA = HuCos(ang);
        sinA = HuSin(ang);
        a1 = p2z - pz;
        b1 = px - p2x;
        c1 = (-a1) * px - b1 * pz;
        lenA = sqrt(a1 * a1 + b1 * b1);
        f31 = px - p2x;
        f30 = pz - p2z;
        lenB = sqrt(f31 * f31 + f30 * f30);
        for (i = 0; i < 17; i++) {
            margin = 0.0f;
            if (i == lbl_1_bss_45C.simCount) {
                i = 15;
            } else if (i == 16) {
                if (lbl_1_bss_45C.g7Flag200A == 0) {
                    if (lbl_1_bss_2F0.ready == 0) {
                        break;
                    }
                }
                margin = 1.0f;
            }
            for (j = 0; j < G7_HCNT[i] - 1; j++) {
                found = 0;
                nearFlag = 0;
                if (i != 15) {
                    f31 = G7_PT(i, j).x;
                    f30 = G7_PT(i, j).z;
                    f19 = 0.0f;
                    f18 = 0.0f;
                    f29 = f31 - px;
                    f28 = f30 - pz;
                    G7_SIDE(f19, f18, f29 + lim, f29 - lim, f28 + lim, f28 - lim)
                    f22 = sqrt(f29 * f29 + f28 * f28);
                    if (f22 <= lim) {
                        cnt++;
                        nearFlag = 1;
                    }
                    if (f19 != 0.0f && f18 != 0.0f && f22 > lim) {
                        f22 = G7_ABS(c1 + (a1 * f31 + b1 * f30));
                        if (lenA) {
                            f22 = f22 / lenA;
                        } else {
                            f22 = 0.0f;
                        }
                        if (lim >= f22) {
                            f22 = sqrt((f31 - px) * (f31 - px) + (f30 - pz) * (f30 - pz));
                            if (f22 < dist[n].x) {
                                dist[n].x = f22;
                                if (lenB) {
                                    f22 = f22 / lenB;
                                } else {
                                    f22 = 0.0f;
                                }
                                hit[n].x = f31;
                                hit[n].z = f30;
                                seg[n][0] = i;
                                seg[n][1] = j;
                                found = 1;
                            }
                        }
                    }
                    f31 = G7_PT(i, j + 1).x;
                    f30 = G7_PT(i, j + 1).z;
                    f19 = 0.0f;
                    f18 = 0.0f;
                    f29 = f31 - px;
                    f28 = f30 - pz;
                    G7_SIDE(f19, f18, f29 + lim, f29 - lim, f28 + lim, f28 - lim)
                    f22 = sqrt(f29 * f29 + f28 * f28);
                    if (f22 <= lim) {
                        cnt++;
                        nearFlag = 1;
                    }
                    if (f19 != 0.0f && f18 != 0.0f && f22 > lim) {
                        f22 = G7_ABS(c1 + (a1 * f31 + b1 * f30));
                        if (lenA) {
                            f22 = f22 / lenA;
                        } else {
                            f22 = 0.0f;
                        }
                        if (lim >= f22) {
                            f22 = sqrt((f31 - px) * (f31 - px) + (f30 - pz) * (f30 - pz));
                            if (f22 < dist[n].x) {
                                dist[n].x = f22;
                                if (lenB) {
                                    f22 = f22 / lenB;
                                } else {
                                    f22 = 0.0f;
                                }
                                hit[n].x = f31;
                                hit[n].z = f30;
                                seg[n][0] = i;
                                seg[n][1] = j;
                                found++;
                            }
                        }
                    }
                    if (nearFlag != 0) {
                        f31 = G7_PT(i, j + 1).x - G7_PT(i, j).x;
                        f30 = G7_PT(i, j + 1).z - G7_PT(i, j).z;
                        f31 = f31 * 0.5f;
                        f30 = f30 * 0.5f;
                        f31 = f31 + G7_PT(i, j).x;
                        f30 = f30 + G7_PT(i, j).z;
                        f19 = 0.0f;
                        f18 = 0.0f;
                        f29 = f31 - px;
                        f28 = f30 - pz;
                        G7_SIDE(f19, f18, f29 + lim, f29 - lim, f28 + lim, f28 - lim)
                        f22 = sqrt(f29 * f29 + f28 * f28);
                        if (f19 != 0.0f && f18 != 0.0f && f22 > lim) {
                            f22 = G7_ABS(c1 + (a1 * f31 + b1 * f30));
                            if (lenA) {
                                f22 = f22 / lenA;
                            } else {
                                f22 = 0.0f;
                            }
                            f29 = sqrt((f31 - px) * (f31 - px) + (f30 - pz) * (f30 - pz));
                            if (f29 - 0.5f >= f22) {
                                f22 = f29;
                                if (f22 < dist[n].x) {
                                    dist[n].x = f22;
                                    if (lenB) {
                                        f22 = f22 / lenB;
                                    } else {
                                        f22 = 0.0f;
                                    }
                                    hit[n].x = f31;
                                    hit[n].z = f30;
                                    seg[n][0] = i;
                                    seg[n][1] = j;
                                }
                            }
                        }
                    }
                }
                if (found != 0 || nearFlag != 0) {
                    flg[i] = 1;
                }
                if (G7_PT(i, j).x < G7_PT(i, j + 1).x) {
                    f19 = G7_PT(i, j).x;
                    maxX = G7_PT(i, j + 1).x;
                } else {
                    f19 = G7_PT(i, j + 1).x;
                    maxX = G7_PT(i, j).x;
                }
                if (G7_PT(i, j).z < G7_PT(i, j + 1).z) {
                    f18 = G7_PT(i, j).z;
                    maxZ = G7_PT(i, j + 1).z;
                } else {
                    f18 = G7_PT(i, j + 1).z;
                    maxZ = G7_PT(i, j).z;
                }
                if (i != 15) {
                    f31 = G7_PT(i, j).x;
                    f30 = G7_PT(i, j).z;
                    f29 = G7_PT(i, j + 1).x + margin;
                    f28 = G7_PT(i, j + 1).z;
                    a2 = f28 - f30;
                    b2 = f31 - f29;
                    c2 = (-a2) * f31 - b2 * f30;
                } else {
                    a2 = lbl_1_data_3E8[j].x;
                    b2 = lbl_1_data_3E8[j].y;
                    c2 = lbl_1_data_3E8[j].z;
                }
                prod = sqrt(a1 * a1 + b1 * b1) * sqrt(a2 * a2 + b2 * b2);
                if (prod) {
                    margin = (a1 * a2 + b1 * b2) / prod;
                } else {
                    margin = 0.0f;
                }
                if (margin < 0.984f && margin > -0.984f) {
                    if (a1 != 0.0f && b1 != 0.0f && a2 != 0.0f && b2 != 0.0f) {
                        f29 = a2 * (-b1 / a1);
                        f28 = -1.0f * (a2 * (-c1 / a1));
                        f29 = f29 + b2;
                        f28 = f28 - c2;
                        if (f29 != 0.0f) {
                            f30 = f28 / f29;
                        } else {
                            f30 = 0.0f;
                        }
                        f31 = ((-b2) * f30 - c2) / a2;
                    } else if (a1 == 0.0f && b1 != 0.0f && a2 != 0.0f && b2 != 0.0f) {
                        f30 = (-c1) / b1;
                        f31 = ((-b2) * f30 - c2) / a2;
                    } else if (a1 != 0.0f && b1 == 0.0f && a2 != 0.0f && b2 != 0.0f) {
                        f31 = (-c1) / a1;
                        f30 = ((-a2) * f31 - c2) / b2;
                    } else if (a1 != 0.0f && b1 != 0.0f && a2 == 0.0f && b2 != 0.0f) {
                        f30 = (-c2) / b2;
                        f31 = ((-b1) * f30 - c1) / a1;
                    } else if (a1 != 0.0f && b1 != 0.0f && a2 != 0.0f && b2 == 0.0f) {
                        f31 = (-c2) / a2;
                        f30 = ((-a1) * f31 - c1) / b1;
                    } else if (a1 == 0.0f && b1 != 0.0f && a2 != 0.0f && b2 == 0.0f) {
                        f30 = (-c1) / b1;
                        f31 = (-c2) / a2;
                    } else if (a1 != 0.0f && b1 == 0.0f && a2 == 0.0f && b2 != 0.0f) {
                        f31 = (-c1) / a1;
                        f30 = (-c2) / b2;
                    }
                    if (f31 >= f19 && f31 <= maxX && f30 >= f18 && f30 <= maxZ) {
                        f29 = f31 - px;
                        f28 = f30 - pz;
                        f19 = 0.0f;
                        f18 = 0.0f;
                        G7_SIDE(f19, f18, f29, f29, f28, f28)
                        if (f19 != 0.0f && f18 != 0.0f) {
                            f29 = sqrt(f29 * f29 + f28 * f28);
                            if (f29 < dist[n].x) {
                                dist[n].x = f29;
                                hit[n].x = f31;
                                hit[n].z = f30;
                                seg[n][0] = i;
                                seg[n][1] = j;
                            }
                        }
                    }
                }
            }
        }
    }
    i = 0;
    j = 0;
    found = 0;
    cand = 0;
    f19 = dist[0].x;
    maxX = dist[0].x;
    for (; i < n; i++) {
        if ((s32)hit[i].x < 700 && (s32)hit[i].x > -700 && (s32)hit[i].z < 700 && (s32)hit[i].z > -700
            && 2000.0f != dist[i].x) {
            if (f19 <= dist[i].x) {
                f19 = dist[i].x;
                j = i;
            }
            if (maxX >= dist[i].x) {
                maxX = dist[i].x;
            }
            if (dist[i].x >= 600.0f) {
                list[cand] = i;
                cand++;
            }
        } else {
            dist[i].x = -1.0f;
            found++;
            return 0;
        }
    }
    if (cand != 0) {
        i = frandmod(cand);
        j = list[i];
    }
    if (dist[lbl_1_bss_2614[idx].targetIdx].x >= 350.0f) {
        j = lbl_1_bss_2614[idx].targetIdx;
    }
    lbl_1_bss_2614[idx].targetIdx = j;
    lbl_1_bss_2614[idx].targetPosV.x = hit[j].x;
    lbl_1_bss_2614[idx].targetPosV.z = hit[j].z;
    lbl_1_bss_2614[idx].targetDist = f19;
    lbl_1_bss_2614[idx].chaseMode = seg[j][0];
    px -= lbl_1_bss_2614[idx].targetPosV.x;
    pz -= lbl_1_bss_2614[idx].targetPosV.z;
    if (lbl_1_bss_2614[idx].targetDist) {
        p2x = scl / lbl_1_bss_2614[idx].targetDist;
    } else {
        p2x = 0.0f;
    }
    px *= p2x;
    pz *= p2x;
    if ((s32)(px + lbl_1_bss_2614[idx].targetPosV.x) > 700 || (s32)(px + lbl_1_bss_2614[idx].targetPosV.x) < -700
        || (s32)(pz + lbl_1_bss_2614[idx].targetPosV.z) > 700 || (s32)(pz + lbl_1_bss_2614[idx].targetPosV.z) < -700) {
        return 0;
    }
    lbl_1_bss_2614[idx].chasePos.x = px + lbl_1_bss_2614[idx].targetPosV.x;
    lbl_1_bss_2614[idx].chasePos.y = 200.0f;
    lbl_1_bss_2614[idx].chasePos.z = pz + lbl_1_bss_2614[idx].targetPosV.z;
    if (maxX >= 300.0f) {
        lbl_1_bss_2614[idx].chaseRadius = 0.01f;
    } else {
        lbl_1_bss_2614[idx].chaseRadius = lbl_1_bss_2614[idx].timerScale;
    }
    return ret;
}
