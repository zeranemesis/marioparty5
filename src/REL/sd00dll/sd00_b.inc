/* sd00 part b: machine effect / mode / collision / damage functions. */

extern s32 rand8(void);
extern double atan2(double y, double x);
extern void DCStoreRangeNoSync(void *addr, u32 nBytes);

static void MachineEffectUpdate(SdMachine *machine);
static void MachineModeIdle(SdMachine *machine);
static void MachineMotionUpdate(SdMachine *machine);
static void MachineModeDamage(SdMachine *machine);
static void MachineFireCreate(SdMachine *machine);
static void MachineFireHook(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx);
static void MachineModeLaunch(SdMachine *machine);
static void MachineModeDie(SdMachine *machine);
static void MachineColUpdate(void);
static void MachineColSeparate(SdMachine *machine);
void PlayDamageSound(void);
static void UpdateEnginePitch(SdMachine *machine, f32 delta);

static void MachineEffectUpdate(SdMachine *machine)
{
    f32 scale;
    Vec vec;
    f32 t = (f32)machine->effTimer;

    if (t >= 21.0f) {
        return;
    }
    if (t < 6.0f) {
        if (machine->effType == 1) {
            scale = 1.0f + (0.1f * t) / 6.0f;
        } else {
            scale = 1.0f - (0.1f * t) / 6.0f;
        }
    } else if (t < 9.0f) {
        scale = t - 6.0f;
        if (machine->effType == 1) {
            scale = 1.1f - (0.1f * scale) / 3.0f;
        } else {
            scale = 0.9f + (0.1f * scale) / 3.0f;
        }
    } else if (t < 15.0f) {
        scale = t - 9.0f;
        if (machine->effType == 1) {
            scale = 1.0f + (0.1f * scale) / 6.0f;
        } else {
            scale = 1.0f - (0.1f * scale) / 6.0f;
        }
    } else if (t < 18.0f) {
        scale = t - 15.0f;
        if (machine->effType == 1) {
            scale = 1.1f - (0.1f * scale) / 3.0f;
        } else {
            scale = 0.9f + (0.1f * scale) / 3.0f;
        }
    } else {
        scale = 1.0f;
    }
    vec.x = vec.y = vec.z = scale;
    MachineScaleSet(machine, &vec);
}

static void MachineModeIdle(SdMachine *machine)
{
    Vec fwd, right, left, tmp, tmp1, tmp2, out;
    Mtx rot;
    f32 speed;
    f32 stickX;
    f32 stickAbs;
    f32 ang;
    f32 radius;

    if (sys->timeUpF != 0) {
        return;
    }
    if (sys->gameType != 0 && machine->unk210 != 0) {
        return;
    }
    if (GwPlayerConf[machine->playerNo].type != 0) {
        SdComMain(machine);
    }
    SdGunUpdate(machine);
    if (machine->hitF == 0) {
        if (HuPadBtn[machine->padNo] & PAD_BUTTON_A) {
            UpdateEnginePitch(machine, 1.0f);
        } else {
            UpdateEnginePitch(machine, -1.0f);
        }
    }
    speed = machine->speed / 60.0f;
    stickX = (f32)HuPadStkX[machine->padNo];
    stickAbs = (stickX < 0.0f) ? -stickX : stickX;
    if (stickAbs > 60.0f) {
        stickAbs = 60.0f;
    }
    fwd.x = machine->mtx[0][2];
    fwd.y = machine->mtx[1][2];
    fwd.z = machine->mtx[2][2];
    right.x = machine->mtx[0][0];
    right.y = machine->mtx[1][0];
    right.z = machine->mtx[2][0];
    PSVECScale(&right, &left, -1.0f);
    if (stickX == 0.0f) {
        PSVECScale(&fwd, &tmp, speed);
        PSVECAdd(&machine->curPos, &tmp, &machine->curPos);
        ang = 0.0f;
    } else if ((machine->attr & 0xF) == 1 && speed == 0.0f && machine->kind != 5) {
        if (stickX > 0.0f) {
            ang = 0.005f * -(stickAbs * (f32)(9 - machine->turnLv));
        } else {
            ang = 0.005f * (stickAbs * (f32)(9 - machine->turnLv));
        }
        PSMTXRotRad(rot, 'y', 0.017453292f * ang);
        PSMTXConcat(machine->mtx, rot, machine->mtx);
    } else {
        ang = 100.0f + 50.0f * (f32)machine->turnLv;
        radius = ang + (2000.0f - ang) * (1.0f - stickAbs / 60.0f);
        PSVECScale(&right, &right, radius);
        PSVECScale(&left, &left, radius);
        ang = (f32)(3.141592653589793 * (2.0f * radius));
        ang = speed / ang;
        if (stickX > 0.0f) {
            tmp1 = right;
            tmp2 = left;
            ang *= -360.0f;
        } else {
            tmp1 = left;
            tmp2 = right;
            ang *= 360.0f;
        }
        PSMTXRotRad(rot, 'y', 0.017453292f * ang);
        PSMTXMultVec(rot, &tmp1, &out);
        PSVECAdd(&tmp2, &machine->curPos, &tmp2);
        PSVECAdd(&tmp2, &out, &machine->curPos);
        PSMTXConcat(machine->mtx, rot, machine->mtx);
    }
    MathMtxRotGet(machine->mtx, &machine->rot);
}

static void MachineMotionUpdate(SdMachine *machine)
{
    Vec rotOut;
    f32 speed;
    f32 stickX;
    f32 stickAbs;
    f32 ang;
    f32 motSpeed;
    s32 i;

    if (sys->timeUpF != 0) {
        return;
    }
    speed = machine->speed / 60.0f;
    if (machine->mode == 1) {
        stickX = (f32)HuPadStkX[machine->padNo];
        stickAbs = (stickX < 0.0f) ? -stickX : stickX;
        if (stickAbs > 60.0f) {
            stickAbs = 60.0f;
        }
    } else {
        stickX = 0.0f;
        stickAbs = 0.0f;
        UpdateEnginePitch(machine, -1.0f);
    }
    ang = (f32)(3.141592653589793 * (2.0f * machine->wheelY0));
    ang = speed / ang;
    machine->tireRot[0].x += 360.0f * ang;
    ang = (f32)(3.141592653589793 * (2.0f * machine->wheelY1));
    ang = speed / ang;
    machine->tireRot[2].x += 360.0f * ang;
    machine->tireRot[0].y += 0.1f * (0.25f * -stickX - machine->tireRot[0].y);
    machine->tireRot[2].y = -machine->tireRot[0].y;
    machine->tireRot[3].x = machine->tireRot[0].x;
    machine->tireRot[2].x = machine->tireRot[0].x;
    machine->tireRot[1].x = machine->tireRot[0].x;
    machine->tireRot[3].z = machine->tireRot[0].z;
    machine->tireRot[2].z = machine->tireRot[0].z;
    machine->tireRot[1].z = machine->tireRot[0].z;
    machine->tireRot[1].y = machine->tireRot[0].y;
    machine->tireRot[3].y = machine->tireRot[2].y;
    if (machine->bodyType == 6 || machine->bodyType == 4 || machine->bodyType == 1 || machine->bodyType == 7 || machine->bodyType == 0) {
        Hu3DMotionSpeedSet(machine->mdlId, 0.1f * speed);
        Hu3DMotionShapeSpeedSet(machine->mdlId, 0.1f * speed);
    } else if (machine->bodyType == 3) {
        Hu3DMotionShapeSpeedSet(machine->mdlId, 1.0f + 0.2f * speed);
    }
    for (i = 0; i < 4; i++) {
        if (machine->hookMdlId[i] == -1) {
            continue;
        }
        if ((machine->attr & 0xF) == 2) {
            continue;
        }
        if ((machine->attr & 0xF) != 1) {
            MathMtxRotSet(&machine->tireRot[i], &rotOut, NULL);
            machine->tireObj[i]->mesh.base.rot.x = rotOut.x;
            machine->tireObj[i]->mesh.base.rot.y = rotOut.y;
            machine->tireObj[i]->mesh.base.rot.z = rotOut.z;
        } else if (speed > 0.0f) {
            Hu3DMotionShapeSpeedSet(machine->hookMdlId[i], 0.4f * speed);
            Hu3DMotionSpeedSet(machine->hookMdlId[i], 0.4f * speed);
        } else if (machine->kind != 5) {
            if (stickX > 0.0f) {
                motSpeed = 0.01f * (stickAbs * (f32)(9 - machine->turnLv));
            } else {
                motSpeed = -0.01f * (stickAbs * (f32)(9 - machine->turnLv));
            }
            if (i % 2 != 0) {
                motSpeed *= -1.0f;
            }
            Hu3DMotionShapeSpeedSet(machine->hookMdlId[i], motSpeed);
            Hu3DMotionSpeedSet(machine->hookMdlId[i], motSpeed);
        }
    }
}

static void MachineModeDamage(SdMachine *machine)
{
    Vec fwd, dir;
    f32 dist;
    f32 lerp;

    fwd.x = machine->planeMtx[0][2];
    fwd.y = machine->planeMtx[1][2];
    fwd.z = machine->planeMtx[2][2];
    dir = fwd;
    dir.y = 0.0f;
    switch (machine->unk4) {
        case 0:
            PSMTXCopy(machine->mtx, machine->savedMtx);
            machine->dmgOfsB = 0.0f;
            machine->unk4++;
            break;
        case 1:
            machine->modeTimer++;
            lerp = 30.0f * MathLerp(5, (f32)machine->modeTimer / 30.0f);
            dist = 250.0f / 30.0f;
            PSVECNormalize(&dir, &dir);
            PSVECScale(&dir, &dir, dist);
            PSVECAdd(&machine->curPos, &dir, &machine->curPos);
            machine->vel = dir;
            dist = 50.0f * MathLerp(5, (f32)machine->modeTimer / 30.0f);
            machine->curPos.y += dist - machine->dmgOfsB;
            if (dist > 0.0f) {
                machine->dmgOfsB = dist;
            }
            if ((f32)machine->modeTimer >= 30.0f) {
                machine->dmgOfsB = 0.0f;
                machine->unk4++;
                machine->modeTimer = 0;
            }
            break;
        case 2:
            machine->modeTimer++;
            dist = 250.0f / 30.0f;
            PSVECNormalize(&dir, &dir);
            PSVECScale(&dir, &dir, dist);
            PSVECAdd(&machine->curPos, &dir, &machine->curPos);
            dist = 5.0f * MathLerp(5, (f32)machine->modeTimer / 15.0f);
            machine->curPos.y += dist - machine->dmgOfsB;
            if (dist > 0.0f) {
                machine->dmgOfsB = dist;
            }
            if ((f32)machine->modeTimer >= 15.0f) {
                machine->dmgOfsA = 0.0f;
                machine->unk4++;
                machine->modeTimer = 0;
            }
            break;
        case 3:
            machine->modeTimer++;
            dist = 80.0f * MathLerp(1, (f32)machine->modeTimer / 15.0f);
            PSVECNormalize(&dir, &dir);
            PSVECScale(&dir, &dir, dist - machine->dmgOfsA);
            PSVECAdd(&machine->curPos, &dir, &machine->curPos);
            machine->dmgOfsA = dist;
            if ((f32)machine->modeTimer >= 15.0f) {
                machine->modeTimer = 0;
                machine->unk4 = 0;
                machine->unk214 = 0;
                if (machine->hp > 0) {
                    machine->unk210 = 1;
                    machine->mode = 1;
                } else {
                    machine->mode = 4;
                }
            }
            break;
    }
}

static void MachineFireCreate(SdMachine *machine)
{
    HU3DPARTICLE *particle[2];
    HU3DPARTICLEDATA *data;
    Vec pos;
    Vec vel;
    GXColor color;
    s32 i, j;

    particle[0] = (HU3DPARTICLE *)Hu3DData[machine->fireMdlId[0]].hookData;
    particle[1] = (HU3DPARTICLE *)Hu3DData[machine->fireMdlId[1]].hookData;
    for (j = 0; j < 2; j++) {
        data = particle[j]->data;
        for (i = 0; i < 20; i++) {
            if (data[i].time != 0) {
                continue;
            }
            data[i].time = 1;
            data[i].parManId = 0;
            pos.x = pos.y = pos.z = 0.0f;
            pos.x = (machine->curPos.x - 25.0f) + (f32)(rand8() % 50);
            pos.y = machine->curPos.y;
            pos.z = (machine->curPos.z - 25.0f) + (f32)(rand8() % 50);
            data[i].pos = pos;
            data[i].scale = 100.0f + (f32)(rand8() % 40);
            vel.x = 0.0f;
            vel.y = 1.0f;
            vel.z = 0.0f;
            data[i].vel = vel;
            color.r = color.g = color.b = color.a = 0xFF;
            data[i].color = color;
            break;
        }
    }
}

static void MachineFireHook(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx)
{
    HU3DPARTICLEDATA *data = particle->data;
    s32 i;

    for (i = 0; i < 20; i++) {
        if (data[i].time == 0) {
            continue;
        }
        data[i].pos.x += data[i].vel.x;
        data[i].pos.y += data[i].vel.y;
        data[i].pos.z += data[i].vel.z;
        PSVECScale(&data[i].vel, &data[i].vel, 1.01f);
        data[i].scale *= 1.04f;
        if (data[i].scale > 320.0f) {
            data[i].scale = 320.0f;
        }
        if (particle->dataCnt == 0) {
            if (data[i].parManId < 30) {
                data[i].color.r += 5;
                data[i].color.g += 5;
                data[i].color.b += 5;
            } else {
                data[i].color.r -= 15;
                data[i].color.g -= 15;
                data[i].color.b -= 15;
            }
        } else {
            data[i].color.r = (u8)((f32)data[i].color.r * 0.995f);
            data[i].color.g = (u8)((f32)data[i].color.g * 0.98f);
            data[i].color.b = (u8)((f32)data[i].color.b * 0.9f);
            data[i].color.a = (u8)((f32)data[i].color.a * 0.92f);
            if (data[i].color.a < 2) {
                data[i].scale = 0.0f;
            }
        }
        data[i].parManId++;
        if (data[i].parManId >= 40) {
            data[i].parManId = 0;
            data[i].time = 0;
            data[i].scale = 0.0f;
        }
    }
    DCStoreRangeNoSync(data, 0x550);
}

static void MachineModeLaunch(SdMachine *machine)
{
    static const s32 animTbl[2] = { 3, 5 };
    static const s32 blendTbl[2] = { 2, 1 };
    Vec right, fwd, pos, dst, tmpA, tmpB;
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *data;
    f32 total;
    f32 stickX, stickY;
    s32 i, j;
    BOOL done;

    if (GwPlayerConf[machine->playerNo].type != 0) {
        SdComMain(machine);
    }
    done = FALSE;
    switch (machine->unk4) {
        case 0:
            for (i = 0; i < 2; i++) {
                machine->fireMdlId[i] = Hu3DParticleCreate(sys->partAnim[animTbl[i]], 20);
                particle = (HU3DPARTICLE *)Hu3DData[machine->fireMdlId[i]].hookData;
                particle->dataCnt = i;
                data = particle->data;
                Hu3DParticleColSet(machine->fireMdlId[i], 0xFF, 0xFF, 0xFF);
                Hu3DParticleBlendModeSet(machine->fireMdlId[i], (u8)blendTbl[i]);
                Hu3DParticleHookSet(machine->fireMdlId[i], MachineFireHook);
                Hu3DModelLayerSet(machine->fireMdlId[i], 5);
                Hu3DModelPosSet(machine->fireMdlId[i], 0.0f, 0.0f, 0.0f);
                Hu3DParticleAnimModeSet(machine->fireMdlId[i], 0);
                for (j = 0; j < particle->maxCnt; j++) {
                    data->time = 0;
                    data->scale = 0.0f;
                    data->cameraBit = 3;
                    data++;
                }
            }
            machine->fireCnt = 0;
            machine->firePos = machine->curPos;
            machine->launchFlag = 1;
            machine->modeTimer = 0;
            machine->unk4++;
            break;
        case 1:
            total = 90.0f;
            if ((f32)machine->modeTimer >= total * 0.5f) {
                machine->launchFlag = 0;
                for (i = 0; i < 4; i++) {
                    machine->tireScale[i] = 1.0f;
                }
                if (machine->hitF == 0) {
                    SdVibrate(machine->playerNo, 1, 15);
                    HuAudFXPlayVolPan(0x555 + machine->playerNo, 0x7F, (s16)(machine->playerNo * 32 + 0x30));
                    machine->firePos = machine->curPos;
                    machine->modeTimer = 0;
                    machine->unk4++;
                    done = TRUE;
                }
            }
            if (!done) {
                machine->firePos.x = machine->curPos.x;
                machine->firePos.z = machine->curPos.z;
                machine->modeTimer++;
                tmpA = machine->firePos;
                tmpB = machine->firePos;
                MathVecInterpolate(&dst, &tmpB, &tmpA, 1500.0f, (f32)machine->modeTimer / total);
                machine->curPos = dst;
                machine->basePos = machine->curPos;
                machine->curPos.x = machine->firePos.x;
                machine->basePos.x = machine->firePos.x;
                machine->curPos.z = machine->firePos.z;
                machine->basePos.z = machine->firePos.z;
            }
            break;
        case 2:
            total = 30.0f;
            if ((f32)machine->modeTimer >= total * 0.5f && machine->hitF == 0) {
                SdVibrate(machine->playerNo, 1, 5);
                HuAudFXPlayVolPan(0x555 + machine->playerNo, 0x5F, (s16)(machine->playerNo * 32 + 0x30));
                machine->firePos = machine->curPos;
                machine->modeTimer = 0;
                machine->unk4++;
            } else {
                machine->firePos.x = machine->curPos.x;
                machine->firePos.z = machine->curPos.z;
                machine->modeTimer++;
                tmpA = machine->firePos;
                tmpB = machine->firePos;
                MathVecInterpolate(&dst, &tmpB, &tmpA, 50.0f, (f32)machine->modeTimer / total);
                machine->curPos = dst;
                machine->basePos = machine->curPos;
                machine->curPos.x = machine->firePos.x;
                machine->basePos.x = machine->firePos.x;
                machine->curPos.z = machine->firePos.z;
                machine->basePos.z = machine->firePos.z;
            }
            break;
        case 3:
            total = 15.0f;
            if ((f32)machine->modeTimer >= total * 0.5f && machine->hitF == 0) {
                HuAudFXPlayVolPan(0x555 + machine->playerNo, 0x3F, (s16)(machine->playerNo * 32 + 0x30));
                machine->firePos = machine->curPos;
                machine->modeTimer = 0;
                machine->unk4++;
            } else {
                machine->firePos.x = machine->curPos.x;
                machine->firePos.z = machine->curPos.z;
                machine->modeTimer++;
                tmpA = machine->firePos;
                tmpB = machine->firePos;
                MathVecInterpolate(&dst, &tmpB, &tmpA, 25.0f, (f32)machine->modeTimer / total);
                machine->curPos = dst;
                machine->basePos = machine->curPos;
                machine->curPos.x = machine->firePos.x;
                machine->basePos.x = machine->firePos.x;
                machine->curPos.z = machine->firePos.z;
                machine->basePos.z = machine->firePos.z;
            }
            break;
        case 4:
            machine->modeTimer = 0;
            machine->unk4 = 0;
            machine->unk214 = 0;
            Hu3DModelKill(machine->fireMdlId[0]);
            Hu3DModelKill(machine->fireMdlId[1]);
            if (machine->hp > 0) {
                machine->mode = 1;
            } else {
                machine->mode = 4;
            }
            return;
    }
    stickX = (f32)HuPadStkX[machine->padNo] / 56.0f;
    stickY = (f32)HuPadStkY[machine->padNo] / 56.0f;
    if (sys->timeUpF != 0) {
        stickY = 0.0f;
        stickX = stickY;
    }
    right.x = machine->mtx[0][0];
    right.y = machine->mtx[1][0];
    right.z = machine->mtx[2][0];
    fwd.x = machine->mtx[0][2];
    fwd.y = machine->mtx[1][2];
    fwd.z = machine->mtx[2][2];
    pos = right;
    tmpA = fwd;
    PSVECScale(&pos, &pos, -8.0f * stickX);
    PSVECScale(&tmpA, &tmpA, 8.0f * stickY);
    PSVECAdd(&machine->curPos, &pos, &machine->curPos);
    PSVECAdd(&machine->curPos, &tmpA, &machine->curPos);
    machine->fireCnt++;
    if (machine->fireCnt % 2 == 0 && machine->unk4 == 1) {
        MachineFireCreate(machine);
    }
}

static void MachineModeDie(SdMachine *machine)
{
    switch (machine->unk4) {
        case 0:
            UpdateEnginePitch(machine, -1.0f);
            if (machine->speed < 0.001f) {
                machine->unk4++;
            }
            break;
        case 1:
            machine->speed = 0.0f;
            break;
    }
}

static void MachineColUpdate(void)
{
    SdMachine *m0 = &sys->machine[0];
    SdMachine *m1 = &sys->machine[1];
    Vec dir, diff;
    f32 dist;
    f32 approach;
    Vec tmp;

    PSVECSubtract(&m0->curPos, &m1->curPos, &dir);
    dir.y = 0.0f;
    dist = PSVECMag(&dir);
    if (dist > m0->hitRadius + m1->hitRadius) {
        return;
    }
    if (m0->curPos.y - m0->wheelBase > m1->curPos.y + m1->hitHeight) {
        return;
    }
    if (m1->curPos.y - m1->wheelBase > m0->curPos.y + m0->hitHeight) {
        return;
    }
    if (m0->contactF == 0) {
        if (sys->timeUpF != 0 || sys->allDoneF != 0) {
            m1->contactF = 1;
            m0->contactF = 1;
        }
        SdVibrate(m0->playerNo, 0, 5);
        SdVibrate(m1->playerNo, 0, 5);
        HuAudFXPlayVolPan(0x561, 0x7F, 0x40);
    }
    PSVECSubtract(&m0->basePos, &m1->basePos, &diff);
    approach = PSVECMag(&diff);
    PSVECSubtract(&m0->curPos, &m1->curPos, &diff);
    approach -= PSVECMag(&diff);
    if (PSVECMag(&dir) == 0.0f) {
        tmp.x = tmp.y = tmp.z = 1.0f;
        dir = tmp;
    }
    PSVECNormalize(&dir, &dir);
    PSVECScale(&dir, &dir, 0.5f * ((m0->hitRadius + m1->hitRadius) - dist));
    m0->curPos.x += dir.x;
    m0->curPos.z += dir.z;
    m1->curPos.x -= dir.x;
    m1->curPos.z -= dir.z;
    if (PSVECMag(&dir) == 0.0f) {
        tmp.x = tmp.y = tmp.z = 1.0f;
        dir = tmp;
    }
    PSVECNormalize(&dir, &dir);
    if (approach < 5.0f) {
        approach = 5.0f;
    }
    PSVECScale(&dir, &dir, 0.5f * approach);
    tmp.x = dir.x;
    tmp.y = 0.0f;
    tmp.z = dir.z;
    m0->vel = tmp;
    tmp.x = -dir.x;
    tmp.y = 0.0f;
    tmp.z = -dir.z;
    m1->vel = tmp;
}

static void MachineColSeparate(SdMachine *machine)
{
    Vec diff, tmp;
    Mtx rot;
    f64 yawPrev;
    f32 yawDiff;
    f32 diffY;
    f32 mul;
    f32 forceAbs, force, forceLim;
    f32 vecScale;
    BOOL boost = FALSE;

    if (sys->timeUpF != 0 || machine->mode == 4) {
        return;
    }
    PSVECSubtract(&machine->curPos, &machine->basePos, &diff);
    yawPrev = 180.0 * (atan2(machine->storedMtx[0][2], machine->storedMtx[2][2]) / 3.141592653589793);
    yawDiff = (f32)(180.0 * (atan2(machine->mtx[0][2], machine->mtx[2][2]) / 3.141592653589793) - yawPrev);
    if (yawDiff > 180.0f) {
        yawDiff -= 360.0f;
    }
    if (yawDiff < -180.0f) {
        yawDiff += 360.0f;
    }
    diffY = diff.y;
    if ((machine->attr & 0x10) == 0) {
        mul = ((machine->attr & 0x20) == 0) ? 1.0f : 0.5f;
        if (machine->boostType == 1) {
            machine->friction = 60.0f * mul;
            vecScale = (f32)(1.0 + 0.25f * mul);
            force = 1.0f + 1.5f * mul;
            forceLim = 0.087f;
            boost = TRUE;
        } else if (machine->boostType == 2) {
            machine->friction = 120.0f * mul;
            vecScale = 1.0f + 0.5f * mul;
            force = 1.0f + 4.0f * mul;
            forceLim = 0.087f;
            boost = TRUE;
        }
    }
    if (!boost) {
        machine->friction *= 0.95f;
        if (machine->friction < 10.0f) {
            machine->friction = 10.0f;
        }
        vecScale = 1.0f;
        force = 0.2f;
        forceLim = 0.19f;
    }
    PSVECScale(&machine->vel, &machine->vel, machine->friction - 1.0f);
    PSVECAdd(&machine->vel, &diff, &machine->vel);
    machine->yawVel = machine->yawVel * (machine->friction - 1.0f);
    machine->yawVel += yawDiff;
    if (PSVECMag(&machine->force) != 0.0f) {
        if (PSVECMag(&machine->force) == 0.0f) {
            tmp.x = tmp.y = tmp.z = 1.0f;
            machine->force = tmp;
        }
        PSVECNormalize(&machine->force, &machine->force);
        forceAbs = (machine->force.y < 0.0f) ? -machine->force.y : machine->force.y;
        if (forceAbs > forceLim) {
            PSVECScale(&machine->force, &machine->force, force);
            machine->force.y = 0.0f;
            PSVECAdd(&machine->vel, &machine->force, &machine->vel);
        }
    }
    machine->yawVel = machine->yawVel / machine->friction;
    PSVECScale(&machine->vel, &machine->vel, 1.0f / machine->friction);
    machine->vel.y = 0.0f;
    PSVECScale(&machine->vel, &diff, vecScale);
    diff.y = diffY;
    PSVECAdd(&machine->basePos, &diff, &machine->curPos);
    PSMTXRotRad(rot, 'y', 0.017453292f * machine->yawVel);
    PSMTXConcat(machine->storedMtx, rot, machine->mtx);
    MathMtxRotGet(machine->mtx, &machine->rot);
}

s32 SdMachineDamage(SdMachine *machine, Vec *pos, s32 damage)
{
    Vec tmp;
    Mtx rot;
    f32 heading;

    if (sys->timeUpF != 0) {
        return 0;
    }
    if (machine->mode >= 2) {
        return 0;
    }
    if (machine->target->mode >= 4) {
        return 0;
    }
    if (sys->gameType == 1) {
        if (SdFlagDispTimerGet(0) >= 3 || SdFlagDispTimerGet(1) >= 3) {
            return 0;
        }
    } else if (sys->gameType == 2) {
        if (SdRabbitDispTimerGet(0) >= 3 || SdRabbitDispTimerGet(1) >= 3) {
            return 0;
        }
    }
    HuAudFXPlayVolPan(0x55D + machine->playerNo, 0x7F, (s16)(machine->playerNo * 32 + 0x30));
    if (damage <= 2) {
        SdVibrate(machine->playerNo, 0, 15);
    } else if (damage <= 5) {
        SdVibrate(machine->playerNo, 1, 15);
    } else {
        SdVibrate(machine->playerNo, 2, 15);
    }
    pos->y = 0.0f;
    if (PSVECMag(pos) == 0.0f) {
        tmp.x = tmp.y = tmp.z = 1.0f;
        *pos = tmp;
    }
    PSVECNormalize(pos, pos);
    heading = (f32)(180.0 * (atan2(pos->x, pos->z) / 3.141592653589793));
    PSMTXRotRad(rot, 'y', 0.017453292f * heading);
    PSMTXCopy(rot, machine->planeMtx);
    if (sys->gameType == 0) {
        if (machine->target->hp > 0 && machine->damage == 0) {
            machine->damage = damage;
        }
        machine->unk218 = 240;
    } else {
        machine->unk218 = (s32)(60.0f + 15.0f * (f32)damage);
    }
    machine->mode = 2;
    if (machine->boostType == 5 && sys->gameType != 0) {
        machine->mode = 3;
    }
    machine->modeTimer = 0;
    machine->unk4 = 0;
    return 1;
}

void PlayDamageSound(void)
{
    SdMachine *machine;
    s32 i;

    for (i = 0; i < 2; i++) {
        machine = &sys->machine[i];
        if (machine->damage > 0) {
            machine->hp -= machine->damage;
            machine->damage = 0;
            if (machine->hp > 0) {
                SdPlayerSoundPlay(machine, 0);
            } else {
                machine->hp = 0;
                HuAudFXPlayVolPan(0x57B + machine->playerNo, 0x7F, (s16)(machine->playerNo * 32 + 0x30));
                SdPlayerSoundPlay(machine, 1);
            }
        }
    }
}

static void UpdateEnginePitch(SdMachine *machine, f32 delta)
{
    Vec fwd;
    f32 maxSpeed;
    f32 pitch;
    f32 accel;
    f32 limit;
    f32 tilt;

    maxSpeed = 200.0f + 100.0f * (f32)machine->speedLv;
    fwd.x = machine->mtx[0][2];
    fwd.y = machine->mtx[1][2];
    fwd.z = machine->mtx[2][2];
    if (fwd.y > 0.5f) {
        tilt = 0.5f;
    } else {
        tilt = (fwd.y < -0.5f) ? -0.5f : fwd.y;
    }
    pitch = tilt;
    if (maxSpeed < 0.0f) {
        pitch = 1.0f + -pitch / 0.5f;
    }
    if (maxSpeed > 0.0f) {
        if (machine->unk39C & 1) {
            pitch = 1.0f;
        } else {
            pitch = 1.0f - (0.5f * pitch) / 0.5f;
        }
    }
    if (pitch > 2.0f) {
        limit = 2.0f;
    } else {
        limit = (pitch < 0.5f) ? 0.5f : pitch;
    }
    pitch = limit;
    if (machine->accelLv == 10) {
        accel = 0.25f;
    } else if (machine->accelLv == 9) {
        accel = 0.5f;
    } else {
        accel = (f32)(9 - machine->accelLv);
    }
    machine->speed = machine->speed + delta * (maxSpeed / (30.0f * accel));
    if (machine->speed > maxSpeed * pitch) {
        limit = maxSpeed * pitch;
    } else {
        limit = (machine->speed < 0.0f) ? 0.0f : machine->speed;
    }
    machine->speed = limit;
    HuAudFXPitchSet(machine->engineSeId, (s16)(s32)(8191.0f * machine->speed / 1500.0f));
}
