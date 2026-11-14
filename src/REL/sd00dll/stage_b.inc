/* stage part b: collision / floor / wall / distance queries.
   Shared data (defined by part A in stage.c, type notes):
     SdStageColTri *stageColMesh, *stageColWall, *stageColCurTriangle;
     s32 stageColWallNum, stageColFloorNum, stageTrialLavaFloorOnF;
     u8 *stageTrialMapColType;  (128 x 128 surface type map)
     SdStageData stageDataTbl[];  f32 stageWork[8];  */

static s32 StageTrialFloorCheck(Vec *pos, Vec *normal, f32 yOfs);
static s32 StageMainFloorCheck(Vec *pos, Vec *normal, f32 yOfs);
static s32 StageTrialWallCheck(Vec *pos, Vec *normal, f32 radius);
static s32 StageMainWallCheck(Vec *pos, Vec *normal, f32 radius);
static f32 StageTrialDistCalc(Vec pos);
static f32 StageMainDistCalc(Vec pos);

s32 fn_1_237D8(SdStageColTri *tri, Vec *pos);
s32 fn_1_23924(SdStageColTri *tri, Vec *pos);

/* lowest y limit of the current stage (shared by RadiusCalc / SprPosGet / RandomPosGet / EdgeDistRadiusGet) */
static f32 StageLimitYGet(void)
{
    if (sys->stageNo == 0) {
        return stageWork[1] - 25.0f;
    }
    if (sys->stageNo == 2 || sys->stageNo == 7) {
        return -25.0f;
    }
    return -10000.0f;
}

void SdStageMachineCol(SdMachine *machine)
{
    Vec prevPos[4];
    s32 mapCnt[6] = { 0, 0, 0, 0, 0, 0 };
    Vec wallNormal;
    Vec reflect;
    Vec dir;
    Vec tmp;
    Vec vtmp;
    Vec pitchVec;
    Vec rotNew;
    Vec axisA, axisB, axisC;
    Vec planeZ;
    Vec p;
    Mtx mtxTmp;
    s32 grassCnt = 0;
    s32 sandCnt = 0;
    s32 iceCnt = 0;
    s32 mapSet = 0;
    s32 onCnt;
    s32 i;
    f32 lim;
    f32 f31;
    f32 yOfs;
    f32 speed;

    if (machine->attr & 2) {
        machine->curPos.y -= machine->wheelBase;
    }
    machine->hitF = 1;
    onCnt = 0;
    PSVECSubtract(&machine->curPos, &machine->basePos, &dir);
    if (SdStageYOfsWallCheck(&machine->curPos, &wallNormal, machine->hitRadius)) {
        dir.y = 0.0f;
        C_VECReflect(&dir, &wallNormal, &reflect);
        if (machine->mode == 2) {
            planeZ.x = machine->planeMtx[0][2];
            planeZ.y = machine->planeMtx[1][2];
            planeZ.z = machine->planeMtx[2][2];
            vtmp = planeZ;
            C_VECReflect(&vtmp, &wallNormal, &vtmp);
            vtmp.y = 0.0f;
            MathPlaneRotMtxGet(&vtmp, NULL, 0.0f, machine->planeMtx);
        }
        if (PSVECDotProduct(&wallNormal, &reflect) > 0.05f) {
            if (PSVECMag(&machine->rotVel) == 0.0f) {
                machine->rotVel.x = 1.0f;
                machine->rotVel.y = 1.0f;
                machine->rotVel.z = 1.0f;
            }
            PSVECNormalize(&machine->rotVel, &axisB);
            if (PSVECDotProduct(&wallNormal, &axisB) < -0.25f) {
                if (PSVECMag(&machine->rotVel) > 1.5f + (f32)machine->speedLv) {
                    HuAudFXPlayVolPan(machine->playerNo + 0x563, 0x7F, (s16)(machine->playerNo * 32 + 0x30));
                    SdVibrate(machine->playerNo, 0, 5);
                }
            }
            f31 = PSVECMag(&machine->vel);
            if (f31 > 10.0f) {
                speed = 10.0f;
            } else if (f31 < 1.0f) {
                speed = 1.0f;
            } else {
                speed = f31;
            }
            PSVECScale(&reflect, &machine->vel, speed);
        }
    }
    for (i = 0; i < 4; i++) {
        PSMTXTrans(mtxTmp, machine->wheelWorld2[i].x, machine->wheelWorld2[i].y, machine->wheelWorld2[i].z);
        PSMTXConcat(machine->mtx, mtxTmp, mtxTmp);
        tmp.x = mtxTmp[0][3];
        tmp.y = mtxTmp[1][3];
        tmp.z = mtxTmp[2][3];
        machine->wheelPos[i] = tmp;
        PSVECAdd(&machine->wheelPos[i], &machine->curPos, &machine->wheelPos[i]);
        if (machine->launchFlag == 0) {
            machine->wheelPos[i].y -= machine->tireScale[i];
        }
        machine->wheelOn[i] = 0;
        prevPos[i] = machine->wheelPos[i];
        yOfs = (i < 2) ? machine->wheelY0 : machine->wheelY1;
        if (SdStageYOfsFloorCheck(&machine->wheelPos[i], NULL, yOfs)) {
            machine->wheelOn[i] = 1;
            machine->colF = 0;
            machine->hitF = 0;
            PSVECAdd(&machine->force, &stageColCurTriangle->unkC, &machine->force);
            machine->wheelAttr[i] = 0;
            if (stageColCurTriangle->attr & 0x10) {
                machine->wheelAttr[i] = 2;
                grassCnt++;
            } else if (stageColCurTriangle->attr & 0x20) {
                machine->wheelAttr[i] = 3;
                sandCnt++;
            } else if (stageColCurTriangle->attr & 0x40) {
                machine->wheelAttr[i] = 4;
                iceCnt++;
            }
            if (sys->stageNo == 4) {
                machine->wheelAttr[i] = 1;
            } else if (machine->wheelPos[i].y - machine->wheelY0 < -50.0f) {
                if (sys->stageNo == 2) {
                    machine->wheelAttr[i] = 6;
                } else if (sys->stageNo == 7) {
                    machine->wheelAttr[i] = 5;
                }
            }
            onCnt++;
            machine->tireScale[i] = 2.0f;
        }
        machine->tireScale[i] *= 1.06f;
        if (machine->tireScale[i] > 100.0f) {
            machine->tireScale[i] = 100.0f;
        } else if (machine->tireScale[i] < 2.0f) {
            machine->tireScale[i] = 2.0f;
        }
        PSVECSubtract(&machine->wheelPos[i], &prevPos[i], &vtmp);
        PSVECScale(&vtmp, &vtmp, 0.5f);
        PSVECAdd(&prevPos[i], &vtmp, &machine->wheelPos[i]);
        if (machine->attr & 2) {
            if (sys->stageNo == 0) {
                if (machine->wheelPos[i].y - machine->wheelY0 < 100.0f) {
                    machine->wheelPos[i].y = 100.0f + machine->wheelY0;
                    machine->wheelOn[i] = 1;
                    machine->colF = 0;
                    machine->hitF = 0;
                }
            } else if (sys->stageNo == 2 || sys->stageNo == 7) {
                if (machine->wheelPos[i].y - machine->wheelY0 < 0.0f) {
                    machine->wheelPos[i].y = machine->wheelY0;
                    machine->wheelOn[i] = 1;
                    machine->colF = 0;
                    machine->hitF = 0;
                }
            }
        }
    }
    machine->wheelPos[2].y += machine->wheelY0 - machine->wheelY1;
    PSVECSubtract(&machine->wheelPos[0], &machine->wheelPos[2], &axisA);
    PSVECNormalize(&axisA, &axisA);
    PSVECSubtract(&machine->wheelPos[1], &machine->wheelPos[0], &axisB);
    PSVECNormalize(&axisB, &axisB);
    PSVECCrossProduct(&axisB, &axisA, &axisC);
    PSVECNormalize(&axisC, &axisC);
    PSVECCrossProduct(&axisC, &axisA, &axisB);
    PSVECNormalize(&axisB, &axisB);
    lim = (sys->stageNo < 8) ? 45.0f : 65.0f;
    rotNew.y = (f32)(180.0 * (atan2(machine->mtx[0][2], machine->mtx[2][2]) / 3.141592653589793));
    PSMTXIdentity(machine->mtx);
    PSMTXRotRad(mtxTmp, 'y', 0.017453292f * rotNew.y);
    PSMTXConcat(machine->mtx, mtxTmp, machine->mtx);
    PSMTXRotRad(mtxTmp, 'y', 0.017453292f * -rotNew.y);
    PSMTXMultVec(mtxTmp, &axisA, &axisA);
    PSMTXMultVec(mtxTmp, &axisB, &axisB);
    rotNew.x = (f32)-(180.0 * (atan2(axisA.y, axisA.z) / 3.141592653589793));
    if (rotNew.x > lim) {
        rotNew.x = lim;
    } else if (rotNew.x < -lim) {
        rotNew.x = -lim;
    }
    f31 = 1.0f;
    if (machine->hitF) {
        machine->colF++;
        if ((f32)machine->colF > 120.0f) {
            machine->colF = 120;
        }
        f31 = 1.0f - (f32)machine->colF / 120.0f;
    }
    rotNew.x *= f31;
    PSMTXRotRad(mtxTmp, 'x', 0.017453292f * -rotNew.x);
    PSMTXMultVec(mtxTmp, &axisB, &axisB);
    rotNew.z = (f32)(180.0 * (atan2(axisB.y, axisB.x) / 3.141592653589793));
    if (rotNew.z > lim) {
        rotNew.z = lim;
    } else if (rotNew.z < -lim) {
        rotNew.z = -lim;
    }
    rotNew.z *= f31;
    PSVECSubtract(&rotNew, &machine->smoothRot, &rotNew);
    PSVECScale(&rotNew, &rotNew, 0.5f);
    PSVECAdd(&machine->smoothRot, &rotNew, &rotNew);
    PSMTXRotRad(mtxTmp, 'x', 0.017453292f * rotNew.x);
    PSMTXConcat(machine->mtx, mtxTmp, machine->mtx);
    PSMTXRotRad(mtxTmp, 'z', 0.017453292f * rotNew.z);
    PSMTXConcat(machine->mtx, mtxTmp, machine->mtx);
    MathMtxRotGet(machine->mtx, &machine->rot);
    machine->smoothRot = rotNew;
    p = machine->curPos;
    PSVECScale(&machine->wheelWorld2[0], &pitchVec, -1.0f);
    PSMTXMultVec(machine->mtx, &pitchVec, &pitchVec);
    PSVECAdd(&pitchVec, &machine->wheelPos[0], &machine->curPos);
    machine->curPos.x = p.x;
    machine->curPos.z = p.z;
    if (machine->attr & 2) {
        machine->curPos.y += machine->wheelBase;
    }
    if (machine->curPos.y < -5000.0f) {
        machine->curPos.y = -5000.0f;
    }
    if (machine->boostType == 5) {
        machine->boostType = 0;
    }
    for (i = 0; i < 4; i++) {
        PSMTXTrans(mtxTmp, machine->wheelWorld2[i].x, machine->wheelWorld2[i].y, machine->wheelWorld2[i].z);
        PSMTXConcat(machine->mtx, mtxTmp, mtxTmp);
        tmp.x = mtxTmp[0][3];
        tmp.y = mtxTmp[1][3];
        tmp.z = mtxTmp[2][3];
        machine->wheelPos[i] = tmp;
        PSVECAdd(&machine->wheelPos[i], &machine->curPos, &machine->wheelPos[i]);
        if (sys->stageNo == 0) {
            s32 mx, mz;
            u8 colType;
            yOfs = (i < 2) ? machine->wheelY0 : machine->wheelY1;
            if (machine->wheelPos[i].y - yOfs < stageWork[1] - 25.0f && stageTrialLavaFloorOnF != 0) {
                machine->boostType = 5;
                machine->wheelAttr[i] = 5;
                continue;
            }
            if (machine->wheelOn[i] == 0) {
                continue;
            }
            mx = (s32)(2400.0f + machine->wheelPos[i].x);
            mx = (s32)((f32)(mx << 7) / 4800.0f);
            if (mx > 127) {
                mx = 127;
            } else if (mx < 0) {
                mx = 0;
            }
            mz = (s32)(2400.0f + machine->wheelPos[i].z);
            mz = (s32)((f32)(mz << 7) / 4800.0f);
            if (mz > 127) {
                mz = 127;
            } else if (mz < 0) {
                mz = 0;
            }
            colType = stageTrialMapColType[(mz << 7) + mx];
            machine->wheelAttr[i] = colType;
            if (machine->boostType != 5 && mapSet == 0) {
                mapCnt[colType]++;
                if (mapCnt[colType] >= 2) {
                    machine->boostType = colType;
                    mapSet = 1;
                }
            }
        } else {
            if (machine->boostType == 5) {
                continue;
            }
            machine->boostType = 0;
            if (grassCnt != 0) {
                machine->boostType = 2;
            }
            if (sandCnt >= 2) {
                machine->boostType = 3;
            }
            if (iceCnt >= 2) {
                machine->boostType = 4;
            }
            if (sys->stageNo == 4) {
                machine->boostType = 1;
            } else if (machine->wheelPos[i].y - machine->wheelY0 < -50.0f) {
                if (sys->stageNo == 2) {
                    machine->boostType = 4;
                } else if (sys->stageNo == 7) {
                    machine->boostType = 5;
                }
            }
        }
    }
}

s32 fn_1_237D8(SdStageColTri *tri, Vec *pos)
{
    Vec edge;
    s32 i;

    if (pos->y > 0.0f) {
        return 0;
    }
    for (i = 0; i < 3; i++) {
        PSVECSubtract(pos, &tri->vtx2[i], &edge);
        edge.y = 0.0f;
        if (PSVECMag(&edge) == 0.0f) {
            Vec one = { 1.0f, 1.0f, 1.0f };
            edge = one;
        }
        PSVECNormalize(&edge, &edge);
        if (PSVECDotProduct(&tri->edge[i], &edge) < tri->dot2[i]) {
            return 0;
        }
    }
    return 1;
}

s32 fn_1_23924(SdStageColTri *tri, Vec *pos)
{
    Vec edge;
    s32 i;

    for (i = 0; i < 3; i++) {
        PSVECSubtract(pos, &tri->vtx[i], &edge);
        edge.y = 0.0f;
        if (PSVECMag(&edge) == 0.0f) {
            Vec one = { 1.0f, 1.0f, 1.0f };
            edge = one;
        }
        PSVECNormalize(&edge, &edge);
        if (PSVECDotProduct(&tri->dir[i], &edge) < tri->dot[i]) {
            return 0;
        }
    }
    return 1;
}

s32 SdStageFloorCheck(Vec *pos, Vec *normal)
{
    if (sys->stageNo == 0) {
        return StageTrialFloorCheck(pos, normal, 0.0f);
    }
    return StageMainFloorCheck(pos, normal, 0.0f);
}

s32 SdStageYOfsFloorCheck(Vec *pos, Vec *normal, f32 yOfs)
{
    if (sys->stageNo == 0) {
        return StageTrialFloorCheck(pos, normal, yOfs);
    }
    return StageMainFloorCheck(pos, normal, yOfs);
}

static s32 StageTrialFloorCheck(Vec *pos, Vec *normal, f32 yOfs)
{
    Vec diff;
    Vec nearest;
    Vec up;
    SdStageColTri *tri;
    SdStageColTri *best = NULL;
    f32 minDist;
    f32 dist;
    s32 hit = 0;
    s32 xi;
    s32 zi;
    s32 x;
    s32 i;

    xi = (s32)(48.0f + pos->x / 50.0f);
    zi = (s32)(24.0f + pos->z / 100.0f);
    if (zi > 0x2F) {
        zi = 0x2F;
    } else if (zi < 0) {
        zi = 0;
    }
    minDist = 10000.0f;
    for (i = 0; i < 3; i++) {
        x = xi + i - 1;
        if (x > 0x5F) {
            x = 0x5F;
        } else if (x < 0) {
            x = 0;
        }
        tri = &stageColMesh[x + zi * 0x60];
        PSVECSubtract(pos, &tri->pos, &diff);
        dist = PSVECMag(&diff);
        if (minDist > dist) {
            minDist = dist;
            nearest = diff;
            best = tri;
            stageColCurTriangle = best;
        }
    }
    nearest.y -= yOfs;
    PSMTXMultVec(best->invMtx, &nearest, &nearest);
    if (nearest.y <= 0.0f) {
        hit = 1;
        up.x = 0.0f;
        up.y = -nearest.y;
        up.z = 0.0f;
        nearest = up;
        PSMTXMultVec(best->mtx, &nearest, &nearest);
        PSVECAdd(pos, &nearest, pos);
        if (normal != NULL) {
            *normal = best->normal;
        }
    }
    return hit;
}

static s32 StageMainFloorCheck(Vec *pos, Vec *normal, f32 yOfs)
{
    Vec diff;
    Vec flat;
    Vec edge;
    Vec up = { 0.0f, 1.0f, 0.0f };
    Vec upLocal;
    SdStageColTri *tri;
    s32 i, j;

    for (i = 0; i < stageColFloorNum; i++) {
        tri = &stageColMesh[i];
        stageColCurTriangle = tri;
        PSVECSubtract(pos, &tri->pos, &diff);
        flat = diff;
        flat.y = 0.0f;
        if (PSVECMag(&flat) - yOfs > tri->radius) {
            continue;
        }
        for (j = 0; j < 3; j++) {
            PSVECSubtract(&diff, &tri->vtx[j], &edge);
            edge.y = 0.0f;
            if (PSVECMag(&edge) == 0.0f) {
                Vec one = { 1.0f, 1.0f, 1.0f };
                edge = one;
            }
            PSVECNormalize(&edge, &edge);
            if (PSVECDotProduct(&tri->dir[j], &edge) < tri->dot[j]) {
                break;
            }
        }
        if (j != 3) {
            continue;
        }
        diff.y -= yOfs;
        PSMTXMultVec(tri->invMtx, &diff, &diff);
        PSMTXMultVec(tri->invMtx, &up, &upLocal);
        if (diff.y > 0.0f) {
            continue;
        }
        PSVECScale(&up, &flat, -diff.y / upLocal.y);
        PSVECAdd(pos, &flat, pos);
        if (normal != NULL) {
            *normal = tri->normal;
        }
        return 1;
    }
    return 0;
}

s32 SdStageWallCheck(Vec *pos, Vec *normal)
{
    if (sys->stageNo == 0) {
        return StageTrialWallCheck(pos, normal, 0.0f);
    }
    return StageMainWallCheck(pos, normal, 0.0f);
}

s32 SdStageYOfsWallCheck(Vec *pos, Vec *normal, f32 yOfs)
{
    if (sys->stageNo == 0) {
        return StageTrialWallCheck(pos, normal, yOfs);
    }
    return StageMainWallCheck(pos, normal, yOfs);
}

static s32 StageTrialWallCheck(Vec *pos, Vec *normal, f32 radius)
{
    s32 hit = 0;

    if (pos->x + radius >= 2350.0f) {
        if (normal != NULL) {
            Vec n = { -1.0f, 0.0f, 0.0f };
            *normal = n;
        }
        pos->x = 2350.0f - radius;
        hit = 1;
    } else if (pos->x - radius <= -2350.0f) {
        if (normal != NULL) {
            Vec n = { 1.0f, 0.0f, 0.0f };
            *normal = n;
        }
        pos->x = -2350.0f + radius;
        hit = 1;
    }
    if (pos->z + radius >= 2350.0f) {
        if (normal != NULL) {
            Vec n = { 0.0f, 0.0f, -1.0f };
            *normal = n;
        }
        pos->z = 2350.0f - radius;
        hit = 1;
    } else if (pos->z - radius <= -2350.0f) {
        if (normal != NULL) {
            Vec n = { 0.0f, 0.0f, 1.0f };
            *normal = n;
        }
        pos->z = -2350.0f + radius;
        hit = 1;
    }
    return hit;
}

static s32 StageMainWallCheck(Vec *pos, Vec *normal, f32 radius)
{
    Vec flat;
    Vec local;
    Vec n;
    Vec nLocal;
    Vec edge;
    SdStageColTri *tri;
    f32 len;
    f32 limit;
    f32 posY;
    s32 hit;
    s32 i, j;

    if (stageDataTbl[sys->stageNo].type == 1) {
        posY = pos->y;
        flat = *pos;
        flat.y = 0.0f;
        len = PSVECMag(&flat);
        limit = stageDataTbl[sys->stageNo].radius;
        if (len + radius > limit) {
            PSVECNormalize(&flat, &flat);
            PSVECScale(&flat, pos, limit - radius);
            pos->y = posY;
            if (normal != NULL) {
                PSVECScale(&flat, normal, -1.0f);
            }
            return 1;
        }
        return 0;
    }
    hit = 0;
    for (i = 0; i < stageColWallNum; i++) {
        tri = &stageColWall[i];
        local = *pos;
        PSVECSubtract(&local, &tri->pos, &local);
        PSMTXMultVec(tri->invMtx, &local, &local);
        local.y -= radius;
        if (!(tri->attr & 2) && local.y < -40.0f) {
            continue;
        }
        n.x = tri->normal.x;
        n.y = 0.0f;
        n.z = tri->normal.z;
        PSVECNormalize(&n, &n);
        PSMTXMultVec(tri->invMtx, &n, &nLocal);
        if (!(tri->attr & 2)) {
            if (local.y > 0.0f) {
                continue;
            }
            for (j = 0; j < 3; j++) {
                PSVECSubtract(&local, &tri->vtx2[j], &edge);
                edge.y = 0.0f;
                if (PSVECMag(&edge) == 0.0f) {
                    Vec one = { 1.0f, 1.0f, 1.0f };
                    edge = one;
                }
                PSVECNormalize(&edge, &edge);
                if (PSVECDotProduct(&tri->edge[j], &edge) < tri->dot2[j]) {
                    break;
                }
            }
            if (j != 3) {
                continue;
            }
        } else if (local.y > 0.0f) {
            continue;
        }
        if (normal != NULL) {
            *normal = n;
        }
        PSVECScale(&n, &n, (-local.y - 1.0f) / nLocal.y);
        PSVECAdd(pos, &n, pos);
        hit = 1;
    }
    if (hit == 1) {
        return 1;
    }
    return 0;
}

f32 SdStageDistCalc(Vec *pos)
{
    pos->y = -5000.0f;
    if (sys->stageNo == 0) {
        return StageTrialDistCalc(*pos);
    }
    return StageMainDistCalc(*pos);
}

static f32 StageTrialDistCalc(Vec pos)
{
    Vec up = { 0.0f, 1.0f, 0.0f };
    Vec diff;
    Vec nearest;
    Vec upLocal;
    Vec move;
    SdStageColTri *tri;
    SdStageColTri *best = NULL;
    f32 minDist;
    f32 dist;
    s32 xi;
    s32 zi;
    s32 x;
    s32 i;

    xi = (s32)(48.0f + pos.x / 50.0f);
    zi = (s32)(24.0f + pos.z / 100.0f);
    if (zi > 0x2F) {
        zi = 0x2F;
    } else if (zi < 0) {
        zi = 0;
    }
    minDist = 10000.0f;
    for (i = 0; i < 3; i++) {
        x = xi + i - 1;
        if (x > 0x5F) {
            x = 0x5F;
        } else if (x < 0) {
            x = 0;
        }
        tri = &stageColMesh[x + zi * 0x60];
        PSVECSubtract(&pos, &tri->pos, &diff);
        dist = PSVECMag(&diff);
        if (minDist > dist) {
            minDist = dist;
            nearest = diff;
            best = tri;
        }
    }
    if (minDist == 10000.0f) {
        return -10000.0f;
    }
    PSMTXMultVec(best->invMtx, &nearest, &nearest);
    PSMTXMultVec(best->invMtx, &up, &upLocal);
    PSVECScale(&up, &move, -nearest.y / upLocal.y);
    PSVECAdd(&pos, &move, &pos);
    return pos.y;
}

static f32 StageMainDistCalc(Vec pos)
{
    Vec up = { 0.0f, 1.0f, 0.0f };
    Vec diff;
    Vec flat;
    Vec edge;
    Vec upLocal;
    SdStageColTri *tri;
    s32 i, j;

    for (i = 0; i < stageColFloorNum; i++) {
        tri = &stageColMesh[i];
        PSVECSubtract(&pos, &tri->pos, &diff);
        flat = diff;
        flat.y = 0.0f;
        if (PSVECMag(&flat) > tri->radius) {
            continue;
        }
        for (j = 0; j < 3; j++) {
            PSVECSubtract(&diff, &tri->vtx[j], &edge);
            edge.y = 0.0f;
            if (PSVECMag(&edge) == 0.0f) {
                Vec one = { 1.0f, 1.0f, 1.0f };
                edge = one;
            }
            PSVECNormalize(&edge, &edge);
            if (PSVECDotProduct(&tri->dir[j], &edge) < tri->dot[j]) {
                break;
            }
        }
        if (j != 3) {
            continue;
        }
        PSMTXMultVec(tri->invMtx, &diff, &diff);
        PSMTXMultVec(tri->invMtx, &up, &upLocal);
        PSVECScale(&up, &flat, -diff.y / upLocal.y);
        PSVECAdd(&pos, &flat, &pos);
        return pos.y;
    }
    return -10000.0f;
}

f32 SdStageRadiusCalc(void)
{
    return StageLimitYGet();
}

void SdStageSprPosGet(s32 *type, Vec *radius)
{
    *type = stageDataTbl[sys->stageNo].type;
    radius->x = stageDataTbl[sys->stageNo].radius;
    radius->z = stageDataTbl[sys->stageNo].radius2;
    radius->y = StageLimitYGet();
}

void SdStageRandomPosGet(Vec *pos, f32 radius)
{
    Vec out = { 0.0f, 0.0f, 0.0f };
    Vec p;
    Vec ext;
    Mtx rot;
    s32 type;

    type = stageDataTbl[sys->stageNo].type;
    ext.x = stageDataTbl[sys->stageNo].radius;
    ext.z = stageDataTbl[sys->stageNo].radius2;
    ext.y = StageLimitYGet();
    ext.x += radius;
    ext.z += radius;
    if (type == 0) {
        out.x = -ext.x + (f32)frandmod((u32)(2.0f * ext.x));
        out.z = -ext.z + (f32)frandmod((u32)(2.0f * ext.z));
    } else {
        out.x = (f32)frandmod((u32)ext.x);
        PSMTXRotRad(rot, 'y', 0.017453292f * (f32)frandmod(0x168));
        PSMTXMultVec(rot, &out, &out);
    }
    p = out;
    p.y = -5000.0f;
    if (sys->stageNo == 0) {
        out.y = StageTrialDistCalc(p);
    } else {
        out.y = StageMainDistCalc(p);
    }
    *pos = out;
}

f32 SdStageEdgeDistGet(Vec *pos)
{
    Vec p = *pos;
    return SdStageEdgeDistRadiusGet(&p, 0.0f);
}

f32 SdStageEdgeDistRadiusGet(Vec *pos, f32 radius)
{
    Vec ext;
    s32 type;
    f32 dx, dz;

    type = stageDataTbl[sys->stageNo].type;
    ext.x = stageDataTbl[sys->stageNo].radius;
    ext.z = stageDataTbl[sys->stageNo].radius2;
    ext.y = StageLimitYGet();
    ext.x += radius;
    ext.z += radius;
    pos->y = 0.0f;
    if (type == 0) {
        if (pos->x < 0.0f) {
            pos->x *= -1.0f;
        }
        if (pos->z < 0.0f) {
            pos->z *= -1.0f;
        }
        dx = ext.x - pos->x;
        dz = ext.z - pos->z;
        if (dx <= dz) {
            return dx;
        }
        return dz;
    }
    return ext.x - PSVECMag(pos);
}
