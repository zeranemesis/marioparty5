#include "REL/m541Dll.h"

void fn_1_11710(s16 idx)
{
    float speedTbl[4] = { 7.2f, 8.0f, 9.0f, 10.285714f };
    s16 timeTbl[8] = { 120, 60, 540, 180, 1500, 300, 3600, 60 };
    s16 posTbl[8] = { 7, 7, 6, 6, 2, 2, 1, 1 };
    s16 i;
    s16 val;

    if (lbl_1_bss_3730->player[idx].dif != 0) {
        lbl_1_bss_2614[idx].waitMot = 1;
        lbl_1_bss_2614[idx].waitCount = lbl_1_bss_3730->player[idx].dif - 1;
        lbl_1_bss_2614[idx].timerScale = speedTbl[lbl_1_bss_2614[idx].waitCount];
        lbl_1_bss_2614[idx].chaseVel = 11.0f;
        val = timeTbl[lbl_1_bss_2614[idx].waitCount * 2] + frandmod(timeTbl[lbl_1_bss_2614[idx].waitCount * 2 + 1]);
        lbl_1_bss_2614[idx].targetMot = val;
        lbl_1_bss_2614[idx].targetTime = 10;
        val = posTbl[lbl_1_bss_2614[idx].waitCount * 2] + frandmod(posTbl[lbl_1_bss_2614[idx].waitCount * 2 + 1]);
        lbl_1_bss_2614[idx].targetPos = val;
    }
    lbl_1_bss_45C.simSpawnCount = 0;
    for (i = 0; i < 8; i++) {
        lbl_1_bss_45C.simSpawn[lbl_1_bss_45C.simSpawnCount].x = lbl_1_data_368[i][0];
        lbl_1_bss_45C.simSpawn[lbl_1_bss_45C.simSpawnCount].y = 0.0f;
        lbl_1_bss_45C.simSpawn[lbl_1_bss_45C.simSpawnCount].z = lbl_1_data_368[i][1];
        lbl_1_bss_45C.simSpawnIdx[i] = i;
        lbl_1_bss_45C.simSpawnCount++;
    }
    lbl_1_bss_45C.simSpawn[lbl_1_bss_45C.simSpawnCount].x = lbl_1_data_368[0][0];
    lbl_1_bss_45C.simSpawn[lbl_1_bss_45C.simSpawnCount].y = 0.0f;
    lbl_1_bss_45C.simSpawn[lbl_1_bss_45C.simSpawnCount].z = lbl_1_data_368[0][1];
    lbl_1_bss_45C.simSpawnIdx[i] = i;
    lbl_1_bss_45C.simSpawnCount++;
}

void fn_1_11B7C(s16 arg0)
{
    switch (lbl_1_bss_45C.simState) {
        case 0:
            fn_1_12008(29);
            lbl_1_bss_45C.simState++;
        case 1:
            lbl_1_bss_45C.simTimer++;
            fn_1_12830();
            break;
    }
}

s32 fn_1_11BF8(s16 arg0)
{
    s32 ret = 0;

    lbl_1_bss_2614[arg0].padStkX = 0;
    lbl_1_bss_2614[arg0].padStkY = 0;
    lbl_1_bss_2614[arg0].padBtnDown = 0;
    lbl_1_bss_2614[arg0].padBtn = 0;
    switch (lbl_1_bss_2614[arg0].otherMode) {
        case 0:
            lbl_1_bss_2614[arg0].otherMode++;
            lbl_1_bss_2614[arg0].otherSub = arg0 * 5;
        case 1:
            if (lbl_1_bss_2614[arg0].otherSub++ >= lbl_1_bss_2614[arg0].targetTime) {
                fn_1_14EB0(arg0);
                lbl_1_bss_2614[arg0].otherSub = 0;
                if (lbl_1_bss_45C.simTimer > lbl_1_bss_2614[arg0].targetMot) {
                    lbl_1_bss_2614[arg0].targetTime += lbl_1_bss_2614[arg0].targetPos;
                }
            }
            fn_1_11DE8(arg0);
            break;
    }
    return ret;
}

s32 fn_1_11DE8(s16 idx)
{
    s32 ret = 0;
    s16 chased = 0;

    switch (lbl_1_bss_2614[idx].chaseSub) {
        case 0:
            MgPlayerComStkOn(lbl_1_bss_0.player[idx]);
            lbl_1_bss_2614[idx].chaseSub++;
            break;
        case 1:
            if ((s32)lbl_1_bss_2614[idx].chasePos.x > 700 || (s32)lbl_1_bss_2614[idx].chasePos.x < -700
                || (s32)lbl_1_bss_2614[idx].chasePos.z > 700 || (s32)lbl_1_bss_2614[idx].chasePos.z < -700) {
                return 0;
            }
            chased = MgPlayerVecChase(lbl_1_bss_0.player[idx], &lbl_1_bss_2614[idx].chasePos, lbl_1_bss_2614[idx].chaseRadius, lbl_1_bss_2614[idx].chaseVel);
            break;
        case 2:
            MgPlayerComStkOff(lbl_1_bss_0.player[idx]);
            lbl_1_bss_2614[idx].chaseSub = 0;
            ret = 1;
            break;
    }
    return ret;
}

s32 fn_1_12008(s16 steps)
{
    s32 ret = 0;
    s16 i;
    s16 j;
    s16 k;

    lbl_1_bss_45C.simCount = 0;
    for (i = 0; i < 15; i++) {
        if (lbl_1_bss_20[i].unk0 != 0) {
            M541_MODEL *mdl = &lbl_1_bss_3730->model[lbl_1_bss_36A4[i].a];
            lbl_1_bss_45C.leafPos[i].x = mdl->posX;
            lbl_1_bss_45C.leafPos[i].y = mdl->posY;
            lbl_1_bss_45C.leafPos[i].z = mdl->posZ;
            lbl_1_bss_45C.simVel[i] = lbl_1_bss_20[i].vel;
            lbl_1_bss_45C.simAngle[i] = lbl_1_bss_20[i].angle;
            lbl_1_bss_45C.simCount++;
        }
        lbl_1_bss_45C.simHistCount[i] = 0;
    }
    lbl_1_bss_45C.simSpeed = lbl_1_bss_2F0.length;
    for (i = 0; i < lbl_1_bss_45C.simCount; i++) {
        lbl_1_bss_45C.simTrail[i][0] = lbl_1_bss_45C.leafPos[i];
        lbl_1_bss_45C.simHist[i][lbl_1_bss_45C.simHistCount[i]] = 0;
        lbl_1_bss_45C.simHistCount[i]++;
    }
    for (k = 0; k < steps; k++) {
        fn_1_194B4();
        fn_1_187F8();
        for (j = 0; j < lbl_1_bss_45C.simCount; j++) {
            lbl_1_bss_45C.simHit[j] = 0;
        }
        fn_1_12F40();
        for (j = 0; j < lbl_1_bss_45C.simCount; j++) {
            if (lbl_1_bss_45C.simSpeed <= 10.0f) {
                lbl_1_bss_45C.simVel[j].x = lbl_1_bss_45C.simSpeed * cos(3.141592653589793 * lbl_1_bss_45C.simAngle[j] / 180.0);
                lbl_1_bss_45C.simVel[j].y = 0.0f;
                lbl_1_bss_45C.simVel[j].z = lbl_1_bss_45C.simSpeed * sin(3.141592653589793 * lbl_1_bss_45C.simAngle[j] / 180.0);
            }
            if (lbl_1_bss_45C.simFixed[j] != 0) {
                lbl_1_bss_45C.simVel[j].x = 0.0f;
                lbl_1_bss_45C.simVel[j].y = 0.0f;
                lbl_1_bss_45C.simVel[j].z = 0.0f;
            }
            lbl_1_bss_45C.leafPos[j].x += lbl_1_bss_45C.simVel[j].x;
            lbl_1_bss_45C.leafPos[j].y += lbl_1_bss_45C.simVel[j].y;
            lbl_1_bss_45C.leafPos[j].z += lbl_1_bss_45C.simVel[j].z;
            lbl_1_bss_45C.simTrail[j][k] = lbl_1_bss_45C.leafPos[j];
            if (lbl_1_bss_45C.simHit[j] != 0 && k != 0) {
                lbl_1_bss_45C.simHist[j][lbl_1_bss_45C.simHistCount[j]] = k;
                lbl_1_bss_45C.simHistCount[j]++;
            }
        }
        lbl_1_bss_45C.simSpeed += 0.002f;
        if (lbl_1_bss_45C.simSpeed > 10.0f) {
            lbl_1_bss_45C.simSpeed = 10.0f;
        }
    }
    for (i = 0; i < lbl_1_bss_45C.simCount; i++) {
        if (lbl_1_bss_45C.simHit[i] != 0) {
            lbl_1_bss_45C.simFlag[i] = 1;
        } else {
            lbl_1_bss_45C.simHist[i][lbl_1_bss_45C.simHistCount[i]] = steps - 1;
            lbl_1_bss_45C.simHistCount[i]++;
            lbl_1_bss_45C.simFlag[i] = 0;
        }
    }
    for (i = 0; i < lbl_1_bss_45C.simCount; i++) {
        lbl_1_bss_45C.simHead[i] = steps;
        if (lbl_1_bss_45C.simHead[i] >= 30) {
            lbl_1_bss_45C.simHead[i] = 0;
        }
        lbl_1_bss_45C.simTail[i] = lbl_1_bss_45C.simHead[i] + 1;
        if (lbl_1_bss_45C.simTail[i] >= 30) {
            lbl_1_bss_45C.simTail[i] = 0;
        }
    }
    return ret;
}

s32 fn_1_12830(void)
{
    s32 ret = 0;
    s16 i;
    s16 j;

    fn_1_194B4();
    fn_1_187F8();
    for (i = 0; i < lbl_1_bss_45C.simCount; i++) {
        lbl_1_bss_45C.simHit[i] = 0;
    }
    fn_1_12F40();
    for (i = 0; i < lbl_1_bss_45C.simCount; i++) {
        if (lbl_1_bss_45C.simSpeed <= 10.0f) {
            lbl_1_bss_45C.simVel[i].x = lbl_1_bss_45C.simSpeed * cos(3.141592653589793 * lbl_1_bss_45C.simAngle[i] / 180.0);
            lbl_1_bss_45C.simVel[i].y = 0.0f;
            lbl_1_bss_45C.simVel[i].z = lbl_1_bss_45C.simSpeed * sin(3.141592653589793 * lbl_1_bss_45C.simAngle[i] / 180.0);
        }
        if (lbl_1_bss_45C.simFixed[i] != 0) {
            lbl_1_bss_45C.simVel[i].x = 0.0f;
            lbl_1_bss_45C.simVel[i].y = 0.0f;
            lbl_1_bss_45C.simVel[i].z = 0.0f;
        }
        lbl_1_bss_45C.leafPos[i].x += lbl_1_bss_45C.simVel[i].x;
        lbl_1_bss_45C.leafPos[i].y += lbl_1_bss_45C.simVel[i].y;
        lbl_1_bss_45C.leafPos[i].z += lbl_1_bss_45C.simVel[i].z;
        lbl_1_bss_45C.simTrail[i][lbl_1_bss_45C.simHead[i]] = lbl_1_bss_45C.leafPos[i];
        if (lbl_1_bss_45C.simHit[i] != 0) {
            if (lbl_1_bss_45C.simFlag[i] != 0) {
                lbl_1_bss_45C.simHistCount[i]++;
            }
            lbl_1_bss_45C.simHist[i][lbl_1_bss_45C.simHistCount[i] - 1] = lbl_1_bss_45C.simHead[i];
            lbl_1_bss_45C.simFlag[i] = 1;
        } else {
            if (lbl_1_bss_45C.simFlag[i] != 0) {
                lbl_1_bss_45C.simHistCount[i]++;
            }
            lbl_1_bss_45C.simHist[i][lbl_1_bss_45C.simHistCount[i] - 1] = lbl_1_bss_45C.simHead[i];
            lbl_1_bss_45C.simFlag[i] = 0;
        }
    }
    lbl_1_bss_45C.simSpeed += 0.002f;
    if (lbl_1_bss_45C.simSpeed > 10.0f) {
        lbl_1_bss_45C.simSpeed = 10.0f;
    }
    for (i = 0; i < lbl_1_bss_45C.simCount; i++) {
        lbl_1_bss_45C.simHist[i][0] = lbl_1_bss_45C.simTail[i];
        if (lbl_1_bss_45C.simHist[i][0] == lbl_1_bss_45C.simHist[i][1]) {
            for (j = 1; j < lbl_1_bss_45C.simHistCount[i]; j++) {
                lbl_1_bss_45C.simHist[i][j - 1] = lbl_1_bss_45C.simHist[i][j];
            }
            lbl_1_bss_45C.simHistCount[i]--;
        }
        lbl_1_bss_45C.simHead[i]++;
        if (lbl_1_bss_45C.simHead[i] >= 30) {
            lbl_1_bss_45C.simHead[i] = 0;
        }
        lbl_1_bss_45C.simTail[i] = lbl_1_bss_45C.simHead[i] + 1;
        if (lbl_1_bss_45C.simTail[i] >= 30) {
            lbl_1_bss_45C.simTail[i] = 0;
        }
    }
    return ret;
}

/* Re-derive the movement angle (degrees) of leaf i from its velocity. */
void fn_1_12F40(void)
{
    float ang[8] = { 90.0f, 180.0f, -90.0f, 0.0f, 90.0f, -180.0f, -90.0f, 0.0f };
    float radA = 30.0f;
    float radB = 30.000002f;
    s16 i;
    s16 j;
    s16 k;
    float dx, dz, dist, f, nx, nz;
    float a1, a2, b1, b2;
    float mi, mj, si, sj, angA;
    float cx, cz, rx, rz, t, len2;

    for (i = 0; i < lbl_1_bss_45C.simCount; i++) {
        if (lbl_1_bss_45C.simFixed[i] != 0) {
            continue;
        }
        for (j = 0; j < lbl_1_bss_45C.simCount; j++) {
            if (i == j) {
                continue;
            }
            dx = lbl_1_bss_45C.leafPos[j].x - lbl_1_bss_45C.leafPos[i].x;
            dz = lbl_1_bss_45C.leafPos[j].z - lbl_1_bss_45C.leafPos[i].z;
            dist = sqrt(dx * dx + dz * dz);
            if (dist < radA + radB) {
                if (dist <= 1.0f) {
                    dist = 1.0f;
                }
                f = (10.0f + (radA + radB)) / dist;
                lbl_1_bss_45C.leafPos[i].x = lbl_1_bss_45C.leafPos[j].x - dx * f;
                lbl_1_bss_45C.leafPos[i].z = lbl_1_bss_45C.leafPos[j].z - dz * f;
                dist = sqrt(
                    (lbl_1_bss_45C.leafPos[j].x - lbl_1_bss_45C.leafPos[i].x) * (lbl_1_bss_45C.leafPos[j].x - lbl_1_bss_45C.leafPos[i].x)
                    + (lbl_1_bss_45C.leafPos[j].z - lbl_1_bss_45C.leafPos[i].z) * (lbl_1_bss_45C.leafPos[j].z - lbl_1_bss_45C.leafPos[i].z));
                if (dist <= 1.0f) {
                    dist = 1.0f;
                }
                nz = (lbl_1_bss_45C.leafPos[j].z - lbl_1_bss_45C.leafPos[i].z) / dist;
                nx = (lbl_1_bss_45C.leafPos[j].x - lbl_1_bss_45C.leafPos[i].x) / dist;
                if (nz == 0.0f && nx == 0.0f) {
                    nz = 1.0f;
                    nx = 0.0f;
                }
                a1 = lbl_1_bss_45C.simVel[j].x * nx + lbl_1_bss_45C.simVel[j].z * nz;
                a2 = -lbl_1_bss_45C.simVel[i].x * nz + lbl_1_bss_45C.simVel[i].z * nx;
                b1 = lbl_1_bss_45C.simVel[i].x * nx + lbl_1_bss_45C.simVel[i].z * nz;
                b2 = -lbl_1_bss_45C.simVel[j].x * nz + lbl_1_bss_45C.simVel[j].z * nx;
                lbl_1_bss_45C.simVel[i].x = a1 * nx - a2 * nz;
                lbl_1_bss_45C.simVel[i].z = a2 * nx + a1 * nz;
                lbl_1_bss_45C.simVel[j].x = b1 * nx - b2 * nz;
                lbl_1_bss_45C.simVel[j].z = b2 * nx + b1 * nz;
                mi = sqrt(lbl_1_bss_45C.simVel[i].x * lbl_1_bss_45C.simVel[i].x + lbl_1_bss_45C.simVel[i].z * lbl_1_bss_45C.simVel[i].z);
                mj = sqrt(lbl_1_bss_45C.simVel[j].x * lbl_1_bss_45C.simVel[j].x + lbl_1_bss_45C.simVel[j].z * lbl_1_bss_45C.simVel[j].z);
                if (mi != 0.0f) {
                    si = lbl_1_bss_45C.simSpeed / mi;
                } else {
                    si = 0.0f;
                }
                if (mj != 0.0f) {
                    sj = lbl_1_bss_45C.simSpeed / mj;
                } else {
                    sj = 0.0f;
                }
                lbl_1_bss_45C.simVel[i].x *= si;
                lbl_1_bss_45C.simVel[i].z *= si;
                lbl_1_bss_45C.simVel[j].x *= sj;
                lbl_1_bss_45C.simVel[j].z *= sj;
                angA = 180.0 * (acos(lbl_1_bss_45C.simVel[i].x / (0.1f + lbl_1_bss_45C.simSpeed)) / 3.141592653589793);
                if ((float)(180.0 * (asin(lbl_1_bss_45C.simVel[i].z / (0.1f + lbl_1_bss_45C.simSpeed)) / 3.141592653589793)) < 0.0f) {
                    angA = 360.0f - angA;
                }
                lbl_1_bss_45C.simAngle[i] = angA;
                lbl_1_bss_45C.simVel[i].x = lbl_1_bss_45C.simSpeed * cos(3.141592653589793 * lbl_1_bss_45C.simAngle[i] / 180.0);
                lbl_1_bss_45C.simVel[i].y = 0.0f;
                lbl_1_bss_45C.simVel[i].z = lbl_1_bss_45C.simSpeed * sin(3.141592653589793 * lbl_1_bss_45C.simAngle[i] / 180.0);
                if (lbl_1_bss_45C.simFixed[j] == 0) {
                    angA = 180.0 * (acos(lbl_1_bss_45C.simVel[j].x / (0.1f + lbl_1_bss_45C.simSpeed)) / 3.141592653589793);
                    if ((float)(180.0 * (asin(lbl_1_bss_45C.simVel[j].z / (0.1f + lbl_1_bss_45C.simSpeed)) / 3.141592653589793)) < 0.0f) {
                        angA = 360.0f - angA;
                    }
                    lbl_1_bss_45C.simAngle[j] = angA;
                    lbl_1_bss_45C.simVel[j].x = lbl_1_bss_45C.simSpeed * cos(3.141592653589793 * lbl_1_bss_45C.simAngle[j] / 180.0);
                    lbl_1_bss_45C.simVel[j].y = 0.0f;
                    lbl_1_bss_45C.simVel[j].z = lbl_1_bss_45C.simSpeed * sin(3.141592653589793 * lbl_1_bss_45C.simAngle[j] / 180.0);
                    lbl_1_bss_45C.simHit[j] = 1;
                }
                lbl_1_bss_45C.simHit[i] = 1;
            }
        }
        for (k = 0; k < 8; k++) {
            dx = lbl_1_data_368[k][0] - lbl_1_data_368[k][2];
            dz = lbl_1_data_368[k][1] - lbl_1_data_368[k][3];
            len2 = dx * dx + dz * dz;
            if (len2 != 0.0f) {
                t = (-1.0f * (dx * (lbl_1_data_368[k][0] - lbl_1_bss_45C.leafPos[i].x) + dz * (lbl_1_data_368[k][1] - lbl_1_bss_45C.leafPos[i].z))) / len2;
            } else {
                t = 0.0f;
            }
            cx = dx * t + lbl_1_data_368[k][0];
            cz = dz * t + lbl_1_data_368[k][1];
            rx = lbl_1_bss_45C.leafPos[i].x - cx;
            rz = lbl_1_bss_45C.leafPos[i].z - cz;
            dist = sqrt(rx * rx + rz * rz);
            if (dist < radB) {
                if (dist <= 1.0f) {
                    dist = 1.0f;
                }
                f = radB / dist;
                lbl_1_bss_45C.leafPos[i].x = cx + rx * f;
                lbl_1_bss_45C.leafPos[i].z = cz + rz * f;
                lbl_1_bss_45C.simAngle[i] = 360.0f - lbl_1_bss_45C.simAngle[i];
                lbl_1_bss_45C.simAngle[i] += ang[k];
                if (lbl_1_bss_45C.simAngle[i] >= 360.0f) {
                    lbl_1_bss_45C.simAngle[i] -= 360.0f;
                }
                lbl_1_bss_45C.simVel[i].x = lbl_1_bss_45C.simSpeed * cos(3.141592653589793 * lbl_1_bss_45C.simAngle[i] / 180.0);
                lbl_1_bss_45C.simVel[i].y = 0.0f;
                lbl_1_bss_45C.simVel[i].z = lbl_1_bss_45C.simSpeed * sin(3.141592653589793 * lbl_1_bss_45C.simAngle[i] / 180.0);
                lbl_1_bss_45C.simHit[i] = 1;
            }
        }
    }
}
