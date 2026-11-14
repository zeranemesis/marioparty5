#include "REL/sd00dll_sys.h"
#include "game/gamework.h"
#include "game/pad.h"

extern s32 rand8(void);

static f32 ComObjDistGet(SdMachine *com, Vec *out);
static s32 ComNextModeFarGet(SdMachine *com);
static s32 ComNextModeNearGet(SdMachine *com);
static void ComStageObjPosGet(Vec *out, SdMachine *com);
static void ComRandomizeMachine(SdMachine *com, s32 vx, s32 vy, s32 vz);

/* [comPreset][gameType] -> COM level */
static s32 comDataTbl[12][3] = {
    {1, 2, 2},
    {1, 2, 2},
    {2, 1, 2},
    {2, 1, 1},
    {2, 1, 1},
    {2, 3, 3},
    {3, 2, 2},
    {3, 3, 3},
    {3, 3, 3},
    {3, 3, 3},
    {3, 3, 3},
    {5, 5, 5},
};

#define COM_BTN_A 0x100
#define COM_BTN_B 0x200

void SdComMain(SdMachine *machine)
{
    s32 hit[5] = { 0, 0, 0, 0, 0 };
    s32 levelTbl[3] = { 5, 3, 1 };
    SdMachine *target = machine->target;
    Vec up = { 0.0f, 0.0f, 0.0f };
    Vec fwd, back, right, left;
    Vec goalPos, refPos, axis;
    Vec aimPos = { 0.0f, 0.0f, 0.0f };
    Vec aimVec = { 0.0f, 0.0f, 0.0f };
    Vec aimDir;
    Vec approach;
    Vec delta;
    Vec lead;
    Vec scratch;
    Vec scratch2;
    Vec tmp;
    Vec probe;
    Vec randPos;
    Vec objPos;
    Mtx rotMtx;
    SdBullet *bullet;
    f32 aimDist = 0.0f;
    f32 goalDist = 0.0f;
    f32 ang, dev, lim, deadzone, reach, range;
    s32 flagOk = 0;
    s32 allow;
    s32 t;
    s32 i;
    s32 padNo;

    refPos = up;
    axis = up;
    goalPos = up;
    approach = up;
    delta = up;
    scratch = up;
    padNo = machine->padNo;

    HuPadStkX[padNo] = 0;
    HuPadBtnDown[padNo] = 0;
    HuPadBtn[padNo] = 0;
    if (machine->comMode != 0) {
        HuPadBtn[padNo] |= COM_BTN_A;
    }

    /* flat forward / right axes of this machine */
    tmp.x = machine->mtx[0][2];
    tmp.y = machine->mtx[1][2];
    tmp.z = machine->mtx[2][2];
    fwd = tmp;
    fwd.y = 0.0f;
    PSVECNormalize(&fwd, &fwd);
    PSVECScale(&fwd, &back, -1.0f);
    tmp.x = machine->mtx[0][0];
    tmp.y = machine->mtx[1][0];
    tmp.z = machine->mtx[2][0];
    right = tmp;
    right.y = 0.0f;
    PSVECNormalize(&right, &right);
    PSVECScale(&right, &left, -1.0f);

    if (machine->comPreset != -1) {
        machine->comLevel = comDataTbl[machine->comPreset][sys->gameType];
    } else {
        machine->comLevel = levelTbl[sys->stageType];
    }

    if (sys->gameType != 0) {
        if (sys->gameType == 1) {
            flagOk = SdFlagCheck();
            SdFlagPosGet(&goalPos);
            refPos = target->curPos;
            axis = up;
        } else {
            flagOk = SdRabbitCheck();
            SdRabbitPosGet(&goalPos);
            refPos = goalPos;
            SdRabbitZAxisGet(&axis);
        }
        PSVECSubtract(&machine->curPos, &goalPos, &delta);
        goalDist = PSVECMag(&delta);
        if (target->mode >= 2 || target->unk210 != 0) {
            if (flagOk == 0) {
                machine->comMode = 2;
            } else {
                machine->comMode = ComNextModeNearGet(machine);
            }
        } else if (machine->comLevel >= 3) {
            if (flagOk != 0) {
                machine->comMode = ComNextModeNearGet(machine);
            } else {
                machine->comMode = 2;
            }
        } else if (flagOk == 0) {
            machine->comMode = 1;
        } else {
            PSVECSubtract(&target->curPos, &goalPos, &scratch2);
            if (goalDist < PSVECMag(&scratch2)) {
                machine->comMode = ComNextModeNearGet(machine);
            } else {
                machine->comMode = 1;
            }
        }
    } else {
        machine->comMode = ComNextModeFarGet(machine);
    }

    if (machine->comMode == 0) {
        return;
    }

    if (rand8() == 0) {
        if (GwPlayerConf[machine->playerNo].type != 0 && GwPlayerConf[target->playerNo].type != 0) {
            s32 a = rand8() % 5;
            s32 b = rand8() % 5;
            s32 c = rand8() % 5;
            ComRandomizeMachine(machine, c - 2, b - 2, a - 2);
        } else {
            s32 a = rand8() % 2;
            s32 b = rand8() % 2;
            s32 c = rand8() % 2;
            ComRandomizeMachine(machine, c - 1, b, a - 1);
        }
    }
    if (machine->comMode != 7) {
        machine->comObjPos.x = -10000.0f;
    }
    if (machine->comMode != 4) {
        machine->comRandPos.x = -10000.0f;
    }

    switch (machine->comMode) {
    case 1:
        approach = target->curPos;
        aimPos = approach;
        aimVec = target->rotVel;
        PSVECSubtract(&machine->curPos, &approach, &delta);
        aimDist = PSVECMag(&delta);
        break;
    case 2:
        aimPos = target->curPos;
        tmp = up;
        tmp.x = target->mtx[0][0];
        tmp.y = target->mtx[1][0];
        tmp.z = target->mtx[2][0];
        approach = tmp;
        PSVECScale(&approach, &approach, 4.0f * target->hitRadius);
        PSVECAdd(&approach, &target->curPos, &approach);
        aimVec = target->rotVel;
        PSVECSubtract(&machine->curPos, &approach, &delta);
        aimDist = PSVECMag(&delta);
        break;
    case 3:
        PSVECSubtract(&machine->curPos, &target->curPos, &scratch);
        if (PSVECMag(&scratch) == 0.0f) {
            scratch.x = scratch.y = scratch.z = 1.0f;
        }
        PSVECNormalize(&scratch, &scratch);
        PSVECScale(&scratch, &scratch, 300.0f);
        PSVECAdd(&machine->curPos, &scratch, &scratch);
        aimPos = scratch;
        approach = scratch;
        aimVec = up;
        PSVECSubtract(&machine->curPos, &approach, &delta);
        aimDist = PSVECMag(&delta);
        break;
    case 4:
        if (machine->comRandPos.x == -10000.0f || machine->comTimer % 90 == 0) {
            SdStageRandomPosGet(&randPos, -200.0f);
            machine->comRandPos = randPos;
        }
        approach = machine->comRandPos;
        aimPos = approach;
        aimVec = up;
        PSVECSubtract(&machine->curPos, &approach, &delta);
        aimDist = PSVECMag(&delta);
        break;
    case 5:
        aimPos = target->curPos;
        tmp = up;
        tmp.x = target->mtx[0][0];
        tmp.y = target->mtx[1][0];
        tmp.z = target->mtx[2][0];
        approach = tmp;
        PSVECScale(&approach, &approach, 2.0f * target->hitRadius);
        PSVECAdd(&approach, &target->curPos, &approach);
        aimVec = target->rotVel;
        PSVECSubtract(&machine->curPos, &approach, &delta);
        aimDist = PSVECMag(&delta);
        break;
    case 6:
        aimPos = target->curPos;
        tmp = up;
        tmp.x = target->mtx[0][0];
        tmp.y = target->mtx[1][0];
        tmp.z = target->mtx[2][0];
        scratch = tmp;
        PSVECScale(&scratch, &scratch, -1.5f);
        tmp = up;
        tmp.x = target->mtx[0][2];
        tmp.y = target->mtx[1][2];
        tmp.z = target->mtx[2][2];
        lead = tmp;
        PSVECScale(&lead, &lead, -1.0f);
        PSVECAdd(&scratch, &lead, &scratch);
        if (PSVECMag(&scratch) == 0.0f) {
            scratch.x = scratch.y = scratch.z = 1.0f;
        }
        PSVECNormalize(&scratch, &scratch);
        PSVECScale(&scratch, &approach, 2.0f * target->hitRadius);
        PSVECAdd(&approach, &target->curPos, &approach);
        aimVec = target->rotVel;
        PSVECSubtract(&machine->curPos, &approach, &delta);
        aimDist = PSVECMag(&delta);
        break;
    case 7:
        aimPos = target->curPos;
        if (machine->comObjPos.x != -10000.0f) {
            ComStageObjPosGet(&objPos, machine);
            machine->comObjPos = objPos;
        }
        if (machine->comObjPos.x != -10000.0f) {
            approach = machine->comObjPos;
            aimVec = up;
        } else {
            machine->comObjPos = target->curPos;
            approach = machine->comObjPos;
            aimVec = target->rotVel;
        }
        PSVECSubtract(&machine->curPos, &approach, &delta);
        aimDist = PSVECMag(&delta);
        break;
    case 8:
        approach = goalPos;
        aimVec = axis;
        aimDist = goalDist;
        aimPos = refPos;
        break;
    default:
        break;
    }

    if (PSVECMag(&aimVec) == 0.0f) {
        aimVec.x = aimVec.y = aimVec.z = 1.0f;
    }
    PSVECNormalize(&aimVec, &aimDir);
    if (PSVECMag(&delta) == 0.0f) {
        delta.x = delta.y = delta.z = 1.0f;
    }
    PSVECNormalize(&delta, &delta);
    PSVECScale(&delta, &lead, -1.0f);
    (void)PSVECDotProduct(&aimDir, &delta);
    (void)PSVECDotProduct(&fwd, &lead);

    machine->comTimer++;

    /* B button (shoot) */
    if ((machine->bulletType == 1 || machine->bulletType == 6) && machine->comLevel != 5) {
        HuPadBtn[padNo] |= COM_BTN_B;
    }
    if (machine->bulletType == 12) {
        if (machine->comLevel == 5) {
            if (machine->comTimer % 180 == 0) {
                HuPadBtnDown[padNo] |= COM_BTN_B;
            }
        } else if (machine->comLevel >= 3) {
            if (machine->comTimer % 30 == 0) {
                HuPadBtnDown[padNo] |= COM_BTN_B;
            }
        } else {
            HuPadBtnDown[padNo] |= COM_BTN_B;
        }
    }
    if (machine->bulletType == 10) {
        bullet = machine->gun->bullet;
        if (bullet->mode != 0) {
            HuPadBtn[padNo] |= COM_BTN_B;
            PSVECSubtract(&bullet->pos, &aimPos, &delta);
            if (PSVECMag(&delta) <= 600.0f) {
                HuPadBtn[padNo] &= ~COM_BTN_B;
            }
        } else if (aimDist < 600.0f) {
            if ((HuPadBtn[padNo] & COM_BTN_B) == 0) {
                HuPadBtnDown[padNo] |= COM_BTN_B;
                HuPadBtn[padNo] |= COM_BTN_B;
            }
        }
    }
    if (machine->bulletType == 2 || machine->bulletType == 5) {
        bullet = machine->gun->lastBullet;
        if (bullet != NULL && bullet->mode != 0) {
            HuPadBtn[padNo] |= COM_BTN_B;
            PSVECSubtract(&aimPos, &bullet->startPos, &delta);
            reach = PSVECMag(&delta);
            PSVECSubtract(&bullet->pos110, &bullet->startPos, &delta);
            range = PSVECMag(&delta);
            if (range >= reach) {
                HuPadBtn[padNo] &= ~COM_BTN_B;
            }
        }
    }
    if (aimDist < 600.0f && machine->bulletType == 2) {
        if ((HuPadBtn[padNo] & COM_BTN_B) == 0) {
            HuPadBtnDown[padNo] |= COM_BTN_B;
            HuPadBtn[padNo] |= COM_BTN_B;
        }
    }

    /* steering: probe the stage edge / objects ahead */
    machine->comStickDir = 0;
    if (machine->boostType == 1) {
        lim = 150.0f;
    } else if (machine->boostType == 2) {
        lim = 200.0f;
    } else {
        lim = 100.0f;
    }
    if ((sys->stageNo == 0 || sys->stageNo == 7) && (machine->attr & 0x50) == 0) {
        for (i = 0; i < 3; i++) {
            PSVECScale(&fwd, &scratch, machine->hitRadius + lim * (f32)(machine->turnLv + 3));
            if (i == 0) {
                PSVECScale(&right, &lead, machine->hitRadius);
                PSVECAdd(&scratch, &lead, &scratch);
            } else if (i == 2) {
                PSVECScale(&right, &lead, -machine->hitRadius);
                PSVECAdd(&scratch, &lead, &scratch);
            }
            PSVECAdd(&scratch, &machine->curPos, &scratch);
            probe = scratch;
            deadzone = SdStageRadiusCalc();
            if (SdStageDistCalc(&probe) < deadzone) {
                hit[i] = 1;
            }
        }
        for (i = 0; i < 2; i++) {
            if (i == 0) {
                PSVECScale(&right, &scratch, 400.0f + machine->hitRadius);
            } else {
                PSVECScale(&right, &scratch, -(400.0f + machine->hitRadius));
            }
            PSVECAdd(&scratch, &machine->curPos, &scratch);
            probe = scratch;
            dev = SdStageDistCalc(&probe);
            if (dev < SdStageRadiusCalc() || dev == -10000.0f) {
                hit[3 + i] = 1;
            }
        }
    }
    probe = fwd;
    dev = ComObjDistGet(machine, &probe);
    if (dev != -10000.0f) {
        if (dev >= 0.0f) {
            hit[0] = 1;
        } else {
            hit[2] = 1;
        }
    }
    if (hit[3] != 0) {
        machine->comStickDir = 1;
    } else if (hit[4] != 0) {
        machine->comStickDir = 2;
    } else if (hit[0] != 0 && hit[2] != 0) {
        machine->comStickDir = 1;
    } else if (hit[0] != 0) {
        machine->comStickDir = 1;
    } else if (hit[2] != 0) {
        machine->comStickDir = 2;
    }
    if (machine->comMode == 7) {
        machine->comStickDir = 0;
    }
    if (machine->comMode == 8 && aimDist < 150.0f) {
        machine->comStickDir = 0;
    }
    if (machine->comStickDir == 1) {
        HuPadStkX[padNo] = 56;
    }
    if (machine->comStickDir == 2) {
        HuPadStkX[padNo] = -56;
    }

    /* lead the target */
    dev = PSVECMag(&aimVec);
    if (dev > 0.1f) {
        if (machine->comLevel <= 2) {
            dev *= aimDist / (5.0f * (f32)machine->gun->bullet->param->unk8);
            PSVECNormalize(&aimVec, &lead);
            if (machine->bulletType != 2) {
                PSVECScale(&lead, &lead, dev);
            } else {
                PSVECScale(&lead, &lead, dev + (f32)(rand8() % 30));
            }
            PSVECAdd(&aimPos, &lead, &lead);
        } else {
            lead = aimPos;
        }
    } else {
        lead = aimPos;
    }
    PSVECSubtract(&machine->curPos, &lead, &lead);
    lead.y = 0.0f;
    if (PSVECMag(&lead) == 0.0f) {
        lead.x = lead.y = lead.z = 1.0f;
    }
    PSVECNormalize(&lead, &lead);
    ang = (f32)(180.0 * (atan2(back.x, back.z) / 3.141592653589793));
    PSMTXRotRad(rotMtx, 'y', 0.017453292f * -ang);
    PSMTXMultVec(rotMtx, &lead, &lead);
    dev = (f32)(180.0 * (atan2(lead.x, lead.z) / 3.141592653589793));
    if (machine->bulletType == 12 && aimDist < 600.0f) {
        dev += 180.0f;
        if (dev > 180.0f) {
            dev -= 360.0f;
        }
    }

    /* aim tolerance */
    if (sys->gameType == 0 && (machine->bulletType == 3 || machine->bulletType == 9)) {
        if (machine->bulletType == 3) {
            if (machine->comLevel <= 3) {
                deadzone = (aimDist >= 1000.0f) ? 10.0f : 5.0f;
            } else {
                deadzone = 2.0f;
            }
        } else {
            if (machine->comLevel <= 3) {
                deadzone = (aimDist >= 1000.0f) ? 45.0f : 10.0f;
            } else {
                deadzone = 2.0f;
            }
        }
    } else if (machine->bulletType == 4) {
        if (sys->gameType == 1 && machine->comMode == 7) {
            deadzone = 2.0f;
        } else {
            deadzone = 15.0f;
        }
    } else if (sys->gameType == 1 && machine->comMode == 8) {
        deadzone = 0.5f;
    } else {
        deadzone = 2.0f;
    }
    if (aimDist < 600.0f) {
        deadzone += 60.0f * (600.0f - aimDist) / 600.0f;
        if (deadzone > 60.0f) {
            deadzone = 60.0f;
        }
    }

    allow = 0;
    t = machine->comTimer % 180;
    if (machine->comLevel == 5) {
        if (t < 60) {
            allow = 1;
        }
    } else if (machine->comLevel == 4) {
        if (t < 90) {
            allow = 1;
        }
    } else if (machine->comLevel == 3) {
        if (t < 120) {
            allow = 1;
        }
    } else if (machine->comLevel == 2) {
        if (t < 150) {
            allow = 1;
        }
    } else {
        allow = 1;
    }
    if (dev <= deadzone && dev >= -deadzone && allow == 1 && (HuPadBtn[padNo] & COM_BTN_B) == 0) {
        HuPadBtnDown[padNo] |= COM_BTN_B;
        HuPadBtn[padNo] |= COM_BTN_B;
    }

    PSVECSubtract(&machine->curPos, &approach, &delta);
    if (PSVECMag(&delta) <= 200.0f) {
        delta = lead;
    }
    ang = (f32)(180.0 * (atan2(back.x, back.z) / 3.141592653589793));
    PSMTXRotRad(rotMtx, 'y', 0.017453292f * -ang);
    PSMTXMultVec(rotMtx, &delta, &delta);
    dev = (f32)(180.0 * (atan2(delta.x, delta.z) / 3.141592653589793));
    if (dev >= deadzone) {
        if (machine->comSteerState != 1) {
            machine->comSteerState = 1;
            machine->comSteerTime = (s32)(30.0f + 0.25f * (f32)machine->comLevel);
            machine->comSteerTimer = 0;
        }
    } else if (dev <= -deadzone) {
        if (machine->comSteerState != 2) {
            machine->comSteerState = 2;
            machine->comSteerTime = (s32)(30.0f + 0.25f * (f32)machine->comLevel);
            machine->comSteerTimer = 0;
        }
    } else {
        machine->comSteerState = 0;
        machine->comSteerTimer = 0;
        machine->comSteerTime = 0;
    }
    machine->comSteerTimer++;
    if (machine->comSteerTimer >= machine->comSteerTime && machine->comStickDir == 0) {
        switch (machine->comSteerState) {
        case 0:
            machine->comSteerStick *= 0.9f;
            break;
        case 1:
            machine->comSteerStick -= 8.0f;
            break;
        case 2:
            machine->comSteerStick += 8.0f;
            break;
        }
        if (machine->stat[0] < 3) {
            lim = 60.0f - 4.0f * (f32)(3 - machine->stat[0]);
        } else {
            lim = 60.0f;
        }
        if (machine->comSteerStick > lim) {
            dev = lim;
        } else if (machine->comSteerStick < -lim) {
            dev = -lim;
        } else {
            dev = machine->comSteerStick;
        }
        machine->comSteerStick = dev;
        HuPadStkX[padNo] = (s8)(s32)machine->comSteerStick;
    }

    /* crashed/spinning: steer toward the nearest safe point */
    if (machine->mode == 3) {
        if (machine->unk4 == 0) {
            if (sys->stageNo == 0) {
                if (machine->comMode == 8 && aimDist < 1000.0f) {
                    PSVECSubtract(&machine->curPos, &approach, &scratch);
                } else {
                    f32 best = 10000.0f;
                    for (i = 0; i < 5; i++) {
                        if (sys->trialPos[i].x != -10000.0f) {
                            PSVECSubtract(&machine->curPos, &sys->trialPos[i], &tmp);
                            dev = PSVECMag(&tmp);
                            if (best > dev) {
                                best = dev;
                                scratch = tmp;
                            }
                        }
                    }
                }
            } else if (sys->stageNo == 7) {
                scratch = machine->curPos;
                if (PSVECMag(&machine->curPos) >= 1500.0f) {
                    PSVECScale(&scratch, &scratch, -1.0f);
                }
                if (PSVECMag(&scratch) == 0.0f) {
                    scratch.x = scratch.y = scratch.z = 1.0f;
                }
                PSVECNormalize(&scratch, &scratch);
            }
            if (PSVECMag(&scratch) == 0.0f) {
                scratch.x = scratch.y = scratch.z = 1.0f;
            }
            PSVECNormalize(&scratch, &scratch);
            dev = (f32)(180.0 * (atan2(scratch.x, scratch.z) / 3.141592653589793));
            ang = (f32)(180.0 * (atan2(fwd.x, fwd.z) / 3.141592653589793));
            dev = dev - ang;
            if (dev > 180.0f) {
                dev -= 360.0f;
            }
            if (dev < -180.0f) {
                dev = 360.0f + dev;
            }
            machine->comStickX = (f32)(-56.0 * sin(3.141592653589793 * dev / 180.0));
            machine->comStickY = (f32)(56.0 * cos(3.141592653589793 * dev / 180.0));
        } else {
            HuPadStkX[padNo] = (s8)(s32)machine->comStickX;
            HuPadStkY[padNo] = (s8)(s32)machine->comStickY;
        }
    }
}

static f32 ComObjDistGet(SdMachine *com, Vec *out)
{
    Vec tmp;
    Vec tmp2;
    Vec refPos = com->curPos;
    SdStageObj *obj;
    s32 i;
    s32 rest;
        f32 deg;
    f32 angleCos;
    f32 distLimit;
    f32 dotLimit;
    f32 refDeg;
    f32 cosLimit;

    rest = 7 - com->comLevel;

    angleCos = 12.0f * rest;
    if (sys->obj == 0) {
        return 0.0f;
    }
    angleCos = (f32)cos(3.141592653589793 * angleCos / 180.0);
    cosLimit = (f32)cos(1.3089969389957472);
    out->y = 0.0f;
    if (PSVECMag(out) == 0.0f) {
        out->x = out->y = out->z = 1.0f;
    }
    PSVECNormalize(out, out);
    for (i = 0; i < 8; i++) {
        obj = &sys->obj[i];
        if (obj->mdlId == -1) {
            continue;
        }
        if (obj->dispF == 0) {
            continue;
        }
        if (obj->type == 4) {
            continue;
        }
        if (obj->type == 5) {
            continue;
        }
        if (obj->type == 0) {
            distLimit = 150.0f + com->hitRadius;
            dotLimit = cosLimit;
        } else {
            distLimit = com->hitRadius + 100.0f * rest;
            dotLimit = angleCos;
        }
        PSVECAdd(&obj->pos, &obj->typeP->offset, &tmp);
        PSVECSubtract(&tmp, &refPos, &tmp);
        tmp2 = tmp;
        if (obj->typeP->flags == 1) {
            tmp.y = 0.0f;
        }
        deg = PSVECMag(&tmp);
        if (deg > distLimit + obj->radius) {
            continue;
        }
        if (PSVECMag(&tmp) == 0.0f) {
            tmp.x = tmp.y = tmp.z = 1.0f;
        }
        PSVECNormalize(&tmp, &tmp);
        if (PSVECDotProduct(out, &tmp) < dotLimit) {
            continue;
        }
        deg = (f32)(180.0 * (atan2(tmp.y, tmp.z) / 3.141592653589793));
        refDeg = (f32)(180.0 * (atan2(out->x, out->z) / 3.141592653589793));
        deg = deg - refDeg;
        if (deg > 180.0f) {
            deg = deg - 360.0f;
        }
        if (deg < -180.0f) {
            deg = 360.0f + deg;
        }
        return deg;
    }
    return -10000.0f;
}

static s32 ComNextModeFarGet(SdMachine *com)
{
        SdMachine *cur = com->target;
    s32 rnd;
    s32 otherQuot;
    s32 selfQuot;

    otherQuot = com->hp / cur->stat[3];
    selfQuot = cur->hp / cur->stat[3];

    if (GwPlayerConf[cur->playerNo].type == 0) {
        if (com->comLevel == 5) {
            if (cur->camSingleF != 0) {
                return 4;
            }
            if (com->comTimer % 180 == 0) {
                rnd = rand8() % 100;
                if (rnd < 15) {
                    return 1;
                }
                if (rnd < 45) {
                    return 0;
                }
                if (rnd < 90) {
                    return 4;
                }
                return 3;
            }
            return com->comMode;
        }
        if (com->comLevel == 3) {
            if (cur->camSingleF != 0) {
                return 4;
            }
            if (com->comTimer % 180 == 0) {
                rnd = rand8() % 100;
                if (rnd < 20) {
                    return 4;
                }
                if (rnd < 25) {
                    return 3;
                }
                goto mid;
            }
            if (com->comMode == 4) {
                goto retnext;
            }
            if (com->comMode == 3) {
                goto retnext;
            }
            goto mid;
        retnext:
            return com->comMode;
        }
        if (com->comLevel == 2) {
            if (com->comTimer % 180 == 0) {
                rnd = rand8() % 100;
                if (rnd < 10) {
                    return 4;
                }
                goto mid;
            }
            if (com->comMode != 4) {
                goto mid;
            }
            return com->comMode;
        }
        goto mid;
    } else {
        if (com->comMode == 4) {
            if (com->comTimer % 30 == 0) {
                goto mid;
            }
            return com->comMode;
        }
        if (com->comTimer % 240 == 0) {
            return 4;
        }
        goto mid;
    }
mid:
    if (otherQuot > selfQuot) {
        return 1;
    }
    if (otherQuot != selfQuot) {
        goto rank;
    }
    if (cur->unk210 != 0) {
        goto rank;
    }
    if (com->unk210 == 0) {
        goto rank;
    }
    if (com->comLevel < 3) {
        return 1;
    }
    return 2;
rank:
    if (cur->mode >= 2) {
        goto rankA;
    }
    if (cur->unk210 == 0) {
        goto rankB;
    }
rankA:
    if (com->comLevel < 3) {
        return 2;
    }
    return 1;
rankB:
    if (com->unk210 == 0) {
        goto bonus;
    }
    if (com->comLevel <= 3) {
        return 1;
    }
    return 2;
bonus:
    if (com->bulletType == 6 || com->bulletType == 11) {
        if (cur->bulletType != 6 && cur->bulletType != 11 && com->hp > 4) {
            return 7;
        }
    }
    if (com->speedLv > cur->speedLv && com->turnLv < cur->turnLv) {
        if (cur->bulletType == 12) {
            return 5;
        }
        return 6;
    }
    return 1;
}

static s32 ComNextModeNearGet(SdMachine *com)
{
        s32 rnd;
    SdMachine *cur = com->target;

    if (GwPlayerConf[cur->playerNo].type == 0) {
        if (com->comTimer % 60 == 0) {
            rnd = rand8() % 100;
            if (com->comLevel == 5) {
                if (rnd < 80) {
                    return 4;
                } else {
                    return 8;
                }
            }
            if (com->comLevel == 3) {
                if (rnd < 20) {
                    return 4;
                } else {
                    return 8;
                }
            }
            if (com->comLevel == 2) {
                if (rnd < 10) {
                    return 4;
                } else {
                    return 8;
                }
            } else {
                goto mode8;
            }
        } else {
            if (com->comLevel == 1) {
                goto mode8;
            }
            return com->comMode;
        }
    } else {
        if (com->comMode == 4) {
            if (com->comTimer % 30 == 0) {
                goto mode8;
            }
            return com->comMode;
        }
        if (com->comTimer % 240 == 0) {
            return 4;
        } else {
            goto mode8;
        }
    }
mode8:
    /* The original's CodeWarrior build merged the four `return 8` tails of
       this function into a single block; the gotos reproduce that shape. */
    return 8;
}

static void ComStageObjPosGet(Vec *out, SdMachine *com)
{
    SdStageObj *obj;
    s32 i;
    SdMachine *cur = com->target;
    SdStageObj *best = 0;
    Vec fallback = {-10000.0f, 0.0f, 0.0f};
    Vec tmp;
    Vec unused;
    s32 bestReach = 10000;
    f32 bestDist = 10000.0f;
    f32 dist;

    if (cur->stat[3] == 1) {
        *out = cur->curPos;
    } else {
        if (sys->obj != 0) {
            for (i = 0; i < 8; i++) {
                obj = &sys->obj[i];
                if (obj->mdlId == -1) {
                    continue;
                }
                if (obj->dispF == 0) {
                    continue;
                }
                if (obj->type >= 4) {
                    continue;
                }
                if (obj->typeP->damage == 0) {
                    continue;
                }
                if (bestReach < obj->typeP->damage) {
                    continue;
                }
                PSVECSubtract(&com->curPos, &obj->pos, &tmp);
                dist = PSVECMag(&tmp);
                if (bestDist < dist) {
                    continue;
                }
                bestDist = dist;
                bestReach = obj->typeP->damage;
                best = obj;
            }
        }
        if (best != 0) {
            *out = best->pos;
        } else {
            *out = fallback;
        }
    }
}

static void ComRandomizeMachine(SdMachine *com, s32 vx, s32 vy, s32 vz)
{
    SdMachineParamSet(com, com->kind, com->bodyType, com->engineType, com->bulletType);
    com->stat[0] += vx;
    com->stat[1] += vy;
    com->stat[2] += vz;
    com->stat[1] = (com->stat[1] > 8) ? 8 : ((com->stat[1] < 1) ? 1 : com->stat[1]);
    com->stat[0] = (com->stat[0] > 8) ? 8 : ((com->stat[0] < 1) ? 1 : com->stat[0]);
    com->stat[2] = (com->stat[2] > 8) ? 8 : ((com->stat[2] < 1) ? 1 : com->stat[2]);
    com->stat[3] = (com->stat[3] > 8) ? 8 : ((com->stat[3] < 1) ? 1 : com->stat[3]);
    com->stat[4] = (com->stat[4] > 8) ? 8 : ((com->stat[4] < 1) ? 1 : com->stat[4]);
}