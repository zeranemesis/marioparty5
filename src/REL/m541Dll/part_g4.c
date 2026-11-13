#include "REL/m541Dll.h"

#include <math.h>

#define G4_PI 3.141592653589793

/* leaf spawn timer: every 300 frames (after 600) one more leaf is placed */
void fn_1_A884(void)
{
    s16 done = 0;

    switch (lbl_1_bss_2F0.spawnMode) {
        case 0:
            if (lbl_1_bss_2F0.spawnTimer >= 600 && (lbl_1_bss_2F0.spawnTimer % 300) == 0 && lbl_1_bss_2F0.spawnCount < 15) {
                lbl_1_bss_2F0.spawnMode++;
            }
            break;
        case 1:
            done = fn_1_A9B8(lbl_1_bss_2F0.spawnCount, 1, 0.0f, 0.0f);
            if (done != 0) {
                lbl_1_bss_2F0.spawnCount++;
                lbl_1_bss_2F0.spawnMode = 0;
            }
            break;
    }
    lbl_1_bss_2F0.spawnTimer++;
}

/* places leaf idx (state 0: pick a position, state 1: fade it in); returns 1 when finished */
s16 fn_1_A9B8(s16 idx, s16 mode, float x, float z)
{
    M541_MODEL *m0 = &lbl_1_bss_3730->model[lbl_1_bss_36A4[idx].a];
    M541_MODEL *m1 = &lbl_1_bss_3730->model[lbl_1_bss_36A4[idx].b];
    M541_MODEL *m2 = &lbl_1_bss_3730->model[lbl_1_bss_36A4[idx].c];
    s16 ret = 0;
    s16 done = 0;
    s16 tries = 0;
    s16 i;
    float radius;
    float ang;
    HuVecF d;
    float dist;
    float rnd;

    switch (lbl_1_bss_20[idx].state) {
        case 0:
            if (mode == 0) {
                    ang = 10.0f * frandmod(0x24);
                    radius = 10.0f * (1.0f + frandmod(8));
                    ang = frandmod(0x168);
                    lbl_1_bss_20[idx].angle = ang;
                    m0->posX = x + radius * cos(G4_PI * ang / 180.0);
                    m0->posY = 0.0f;
                    m0->posZ = z + radius * sin(G4_PI * ang / 180.0);
                } else if (mode == 1) {
                    ang = lbl_1_bss_45C.aiPos[idx].x;
                    radius = lbl_1_bss_45C.aiPos[idx].y;
                    lbl_1_bss_20[idx].angle = lbl_1_bss_45C.aiPos[idx].z;
                    m0->posX = x + radius * cos(G4_PI * ang / 180.0);
                    m0->posY = 0.0f;
                    m0->posZ = z + radius * sin(G4_PI * ang / 180.0);
                } else if (mode == 2) {
                    ang = frandmod(0x168);
                    lbl_1_bss_20[idx].angle = ang;
                    do {
                        ang = frandmod(0x168);
                        radius = 61.0f + frandmod(0x78);
                        m0->posX = radius * cos(G4_PI * ang / 180.0);
                        m0->posY = 0.0f;
                        m0->posZ = radius * sin(G4_PI * ang / 180.0);
                        tries++;
                        done = 0;
                        for (i = 0; i < 15; i++) {
                            if (lbl_1_bss_20[i].exist != 0) {
                                d.x = m0->posX - lbl_1_bss_3730->model[lbl_1_bss_36A4[i].a].posX;
                                d.z = m0->posZ - lbl_1_bss_3730->model[lbl_1_bss_36A4[i].a].posZ;
                                dist = sqrt(d.x * d.x + d.z * d.z);
                                if (dist < 61.0f && tries < 10000) {
                                    done = 1;
                                    break;
                                }
                            }
                        }
                    } while (done != 0);
                }
            m1->pos = m0->pos;
            m2->pos = m0->pos;
            Hu3DModelAttrReset(m0->modelId, 1);
            Hu3DModelAttrReset(m1->modelId, 1);
            Hu3DModelAttrSet(m1->modelId, 0x40000001);
            rnd = frandmod(0x5A);
            Hu3DMotionTimeSet(m1->modelId, rnd);
            Hu3DMotionSpeedSet(m1->modelId, 2.0f);
            lbl_1_bss_3730->model[lbl_1_bss_36A4[idx].a].rotY = 90.0f + -lbl_1_bss_20[idx].angle;
            Hu3DModelTPLvlSet(m0->modelId, lbl_1_bss_20[idx].tpLvl);
            Hu3DModelTPLvlSet(m2->modelId, lbl_1_bss_20[idx].tpLvl);
            Hu3DModelShadowDispOn(m2->modelId);
            lbl_1_bss_20[idx].exist = 1;
            lbl_1_bss_20[idx].state++;
            break;
        case 1:
            done += fn_1_1850(&m0->posY, 90.0f, 0.0f, 200.0f);
            done += fn_1_1850(&m1->posY, 90.0f, 0.0f, 200.0f);
            done += fn_1_1850(&m2->posY, 90.0f, 0.0f, 200.0f);
            lbl_1_bss_20[idx].tpLvl += 0.012f;
            if (lbl_1_bss_20[idx].tpLvl >= 1.0f) {
                lbl_1_bss_20[idx].tpLvl = 1.0f;
            } else {
                done = 1;
            }
            Hu3DModelTPLvlSet(m0->modelId, lbl_1_bss_20[idx].tpLvl);
            Hu3DModelTPLvlSet(m2->modelId, lbl_1_bss_20[idx].tpLvl);
            if (done == 0) {
                lbl_1_bss_20[idx].vel.x = lbl_1_bss_2F0.length * cos(G4_PI * lbl_1_bss_20[idx].angle / 180.0);
                lbl_1_bss_20[idx].vel.y = 0.0f;
                lbl_1_bss_20[idx].vel.z = lbl_1_bss_2F0.length * sin(G4_PI * lbl_1_bss_20[idx].angle / 180.0);
                lbl_1_bss_20[idx].unk0 = 1;
                lbl_1_bss_20[idx].state = 1;
                ret = 1;
            }
            break;
    }
    return ret;
}

/*
 * AI helper: looks for a point on a circle around fighter idx (angles around the
 * direction to the other fighter) that is free of walls and leaves, and stores the
 * move target in lbl_1_bss_2734.  Returns 1 when a target was found.
 */
#define G4_INSIDE(x, z) \
    (-(x) + (z) > -784.0f && (x) < 554.0f && (x) + (z) < 784.0f && (z) < 554.0f \
     && -(x) + (z) < 784.0f && (x) > -554.0f && (x) + (z) > -784.0f && (z) > -554.0f)

s16 fn_1_B4E0(s16 idx)
{
    HU3DMODEL *other = &Hu3DData[lbl_1_bss_3730->model[(idx ^ 1) + 0x6A].modelId];
    HU3DMODEL *me = &Hu3DData[lbl_1_bss_3730->model[idx + 0x6A].modelId];
    s32 ret = 0;
    float minX, maxX, minZ, maxZ;
    float dx, dz, mag, baseAng;
    float ang, ang360, lineA, lineB, lineC;
    float ex, ez, hx, hz, dd;
    float ix, iz, t1, t2;
    float px, pz;
    float sp3C, sp38, flagX, flagZ, xLo, xHi, zLo, zHi, cosA, lenProd, tmp;
    float sp4C, sp48, sp44, sp40;
    float found, spC;
    float cx, cz;
    float dist[20];
    float leafMin[20];
    s16 order[20];
    s16 order2[20];
    HuVecF hit[20];
    s16 i, j, n;
    s16 tmpS;

    minX = other->pos.x - 150.0f;
    maxX = 150.0f + other->pos.x;
    minZ = other->pos.z - 110.0f;
    maxZ = 300.0f + other->pos.z;
    if (me->pos.x >= minX && me->pos.x <= maxX && me->pos.z >= minZ && me->pos.z <= maxZ) {
        dx = me->pos.x - other->pos.x;
        dz = me->pos.z - other->pos.z;
        mag = sqrt(dx * dx + dz * dz);
        spC = mag;
        dx = 180.0 * (acos(dx / (0.1f + spC)) / G4_PI);
        dz = 180.0 * (asin(dz / (0.1f + spC)) / G4_PI);
        if (dz < 0.0f) {
            dx = 360.0f - dx;
        }
        baseAng = dx;
        for (i = 0; i < 20; i++) {
            dist[i] = 2000.0f;
            leafMin[i] = 2000.0f;
            order[i] = i;
            order2[i] = i;
        }
        cx = me->pos.x;
        cz = me->pos.z;
        ang = baseAng - 100.0f;
        n = 0;
        ix = iz = 0.0f;
        for (; ang < 100.0f + baseAng; ang += 10.0f, n++) {
            sp4C = cx + 10.0 * cos(G4_PI * ang / 180.0);
            sp48 = cz + 10.0 * sin(G4_PI * ang / 180.0);
            ez = sp48 - cz;
            ex = cx - sp4C;
            sp3C = -ez * cx - ex * cz;
            for (j = 0; j < lbl_1_bss_45C.simSpawnCount - 1; j++) {
                HuVecF *p0 = &lbl_1_bss_45C.simSpawn[lbl_1_bss_45C.simSpawnIdx[j]];
                HuVecF *p1 = &lbl_1_bss_45C.simSpawn[lbl_1_bss_45C.simSpawnIdx[j + 1]];
                if (p0->x < p1->x) {
                    xLo = p0->x;
                    xHi = p1->x;
                } else {
                    xLo = p1->x;
                    xHi = p0->x;
                }
                if (p0->z < p1->z) {
                    zLo = p0->z;
                    zHi = p1->z;
                } else {
                    zLo = p1->z;
                    zHi = p0->z;
                }
                lineA = lbl_1_data_3E8[j].x;
                lineB = lbl_1_data_3E8[j].y;
                sp38 = lbl_1_data_3E8[j].z;
                lenProd = sqrt(ez * ez + ex * ex) * sqrt(lineA * lineA + lineB * lineB);
                if (lenProd != 0.0f) {
                    cosA = (ez * lineA + ex * lineB) / lenProd;
                } else {
                    cosA = 0.0f;
                }
                if (cosA != 0.0f) {
                    if (ez != 0.0f && ex != 0.0f && lineA != 0.0f && lineB != 0.0f) {
                        t1 = lineA * (-ex / ez) + lineB;
                        t2 = -1.0f * (lineA * (-sp3C / ez)) - sp38;
                        if (t1 != 0.0f) {
                            iz = t2 / t1;
                        } else {
                            iz = 0.0f;
                        }
                        ix = (-lineB * iz - sp38) / lineA;
                    } else if (ez == 0.0f && ex != 0.0f && lineA != 0.0f && lineB != 0.0f) {
                        iz = -sp3C / ex;
                        ix = (-lineB * iz - sp38) / lineA;
                    } else if (ez != 0.0f && ex == 0.0f && lineA != 0.0f && lineB != 0.0f) {
                        ix = -sp3C / ez;
                        iz = (-lineA * ix - sp38) / lineB;
                    } else if (ez != 0.0f && ex != 0.0f && lineA == 0.0f && lineB != 0.0f) {
                        iz = -sp38 / lineB;
                        ix = (-ex * iz - sp3C) / ez;
                    } else if (ez != 0.0f && ex != 0.0f && lineA != 0.0f && lineB == 0.0f) {
                        ix = -sp38 / lineA;
                        iz = (-ez * ix - sp3C) / ex;
                    } else if (ez == 0.0f && ex != 0.0f && lineA != 0.0f && lineB == 0.0f) {
                        iz = -sp3C / ex;
                        ix = -sp38 / lineA;
                    } else if (ez != 0.0f && ex == 0.0f && lineA == 0.0f && lineB != 0.0f) {
                        ix = -sp3C / ez;
                        iz = -sp38 / lineB;
                    }
                    if (ix >= xLo && ix <= xHi && iz >= zLo && iz <= zHi) {
                        hx = ix - cx;
                        hz = iz - cz;
                        flagX = 0.0f;
                        flagZ = 0.0f;
                        ang360 = ang;
                        if (ang < 0.0f) {
                            ang360 = 360.0f + ang;
                        }
                        if ((ang360 >= 0.0f && ang360 < 90.0f) || (ang360 > 270.0f && ang360 <= 360.0f)) {
                            if (hx > 0.0f) {
                                flagX = 1.0f;
                            }
                        } else if (ang360 == 90.0f || ang360 == 270.0f) {
                            flagX = 1.0f;
                        } else if (hx < 0.0f) {
                            flagX = 1.0f;
                        }
                        if (ang360 > 0.0f && ang360 < 180.0f) {
                            if (hz > 0.0f) {
                                flagZ = 1.0f;
                            }
                        } else if (ang360 == 0.0f || ang360 == 180.0f || ang360 == 360.0f) {
                            flagZ = 1.0f;
                        } else if (hz < 0.0f) {
                            flagZ = 1.0f;
                        }
                        if (flagX != 0.0f && flagZ != 0.0f) {
                            dd = sqrt(hx * hx + hz * hz);
                            if (dd < dist[n]) {
                                dist[n] = dd;
                                hit[n].x = ix;
                                hit[n].z = iz;
                            }
                        }
                    }
                } else {
                    dist[n] = 0.0f;
                }
            }
            for (j = 0; j < lbl_1_bss_2F0.spawnCount; j++) {
                ix = sp4C - cx;
                iz = sp48 - cz;
                dd = ix * ix + iz * iz;
                sp38 = -1.0f * (ix * (sp4C - lbl_1_bss_3730->model[lbl_1_bss_36A4[j].a].posX)
                                + iz * (sp48 - lbl_1_bss_3730->model[lbl_1_bss_36A4[j].a].posZ));
                if (dd != 0.0f) {
                    t1 = sp38 / dd;
                } else {
                    t1 = 0.0f;
                }
                sp44 = sp4C + ix * t1;
                sp40 = sp48 + iz * t1;
                hx = sp44 - cx;
                hz = sp40 - cz;
                flagX = 0.0f;
                flagZ = 0.0f;
                ang360 = ang;
                if (ang < 0.0f) {
                    ang360 = 360.0f + ang;
                }
                if ((ang360 >= 0.0f && ang360 < 90.0f) || (ang360 > 270.0f && ang360 <= 360.0f)) {
                    if (hx > 0.0f) {
                        flagX = 1.0f;
                    }
                } else if (ang360 == 90.0f || ang360 == 270.0f) {
                    flagX = 1.0f;
                } else if (hx < 0.0f) {
                    flagX = 1.0f;
                }
                if (ang360 > 0.0f && ang360 < 180.0f) {
                    if (hz > 0.0f) {
                        flagZ = 1.0f;
                    }
                } else if (ang360 == 0.0f || ang360 == 180.0f || ang360 == 360.0f) {
                    flagZ = 1.0f;
                } else if (hz < 0.0f) {
                    flagZ = 1.0f;
                }
                if (flagX != 0.0f && flagZ != 0.0f) {
                    ix = lbl_1_bss_3730->model[lbl_1_bss_36A4[j].a].posX - sp44;
                    iz = lbl_1_bss_3730->model[lbl_1_bss_36A4[j].a].posZ - sp40;
                    spC = sqrt(ix * ix + iz * iz);
                    if (spC < 70.0f) {
                        spC = sqrt(hx * hx + hz * hz);
                        if (spC <= leafMin[n]) {
                            leafMin[n] = spC;
                        }
                    }
                }
            }
        }
        for (i = 0; i < 20; i++) {
            for (j = i; j < 20; j++) {
                if (dist[i] <= dist[j] && dist[j] != 2000.0f) {
                    tmp = dist[i];
                    dist[i] = dist[j];
                    dist[j] = tmp;
                    tmpS = order[i];
                    order[i] = order[j];
                    order[j] = tmpS;
                }
            }
        }
        found = 0.0f;
        for (i = 0; i < 20; i++) {
            if (leafMin[order[i]] >= 150.0f) {
                ang = (baseAng - 100.0f) + 10.0f * order[i];
                px = me->pos.x + 150.0 * cos(G4_PI * ang / 180.0);
                pz = me->pos.z + 150.0 * sin(G4_PI * ang / 180.0);
                if (G4_INSIDE(px, pz)) {
                    if (!(px >= minX && px <= maxX && pz >= minZ && pz <= maxZ)) {
                        break;
                    }
                }
            }
        }
        if (i >= 20) {
            found = 1.0f;
        }
        if (found == 1.0f) {
            for (i = 0; i < 20; i++) {
                ang = (baseAng - 100.0f) + 10.0f * order[i];
                px = me->pos.x + 150.0 * cos(G4_PI * ang / 180.0);
                pz = me->pos.z + 150.0 * sin(G4_PI * ang / 180.0);
                if (G4_INSIDE(px, pz)) {
                    if (!(px >= minX && px <= maxX && pz >= minZ && pz <= maxZ)) {
                        break;
                    }
                }
            }
            found = 0.0f;
            if (i >= 20) {
                found = 1.0f;
            }
        }
        if (found == 1.0f) {
            for (i = 0; i < 20; i++) {
                ang = (baseAng - 100.0f) + 10.0f * order[i];
                px = me->pos.x + 150.0 * cos(G4_PI * ang / 180.0);
                pz = me->pos.z + 150.0 * sin(G4_PI * ang / 180.0);
                if (G4_INSIDE(px, pz)) {
                    break;
                }
            }
            found = 0.0f;
            if (i >= 20) {
                found = 1.0f;
            }
        }
        if (found != 1.0f) {
            lbl_1_bss_2734.moveX = me->pos.x + 100.0 * cos(G4_PI * ang / 180.0);
            lbl_1_bss_2734.moveZ = me->pos.z + 100.0 * sin(G4_PI * ang / 180.0);
            lbl_1_bss_2734.goal.x = lbl_1_bss_2734.moveX + 50.0 * cos(G4_PI * ang / 180.0);
            lbl_1_bss_2734.goal.z = lbl_1_bss_2734.moveZ + 50.0 * sin(G4_PI * ang / 180.0);
            ret = 1;
        } else {
            lbl_1_bss_2734.moveX = 0.0f;
            lbl_1_bss_2734.moveZ = 0.0f;
            lbl_1_bss_2734.goal.x = 0.0f;
            lbl_1_bss_2734.goal.z = 0.0f;
            ret = 0;
        }
        for (i = 0; i < 2; i++) {
            if ((s32)lbl_1_bss_2734.moveX > 700 || (s32)lbl_1_bss_2734.moveX < -700
                || (s32)lbl_1_bss_2734.moveZ > 700 || (s32)lbl_1_bss_2734.moveZ < -700
                || (s32)lbl_1_bss_2734.goal.x > 700 || (s32)lbl_1_bss_2734.goal.x < -700
                || (s32)lbl_1_bss_2734.goal.z > 700 || (s32)lbl_1_bss_2734.goal.z < -700) {
                lbl_1_bss_2734.moveX = 0.0f;
                lbl_1_bss_2734.moveZ = 0.0f;
                lbl_1_bss_2734.goal.x = 0.0f;
                lbl_1_bss_2734.goal.z = 0.0f;
                ret = 0;
            }
        }
    } else {
        ret = 0;
    }
    return ret;
}
