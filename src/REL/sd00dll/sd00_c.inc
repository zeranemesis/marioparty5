/* sd00 part c: machine model/pos/rot helpers, cameras, vibration. */
#include "game/charman.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/gamework.h"
#include "game/object.h"

extern s32 rand8(void);

static s32 p1MotFileTbl[12] = {
    0x00BC0071, 0x00BC0073, 0x00BC0075, 0x00BC0077, 0x00BC0079, 0x00BC007B,
    0x00BC007D, 0x00BC007F, 0x00BC0081, 0x00BC0083, 0x00BC0086, 0x00BC0089,
};

static s32 p2MotFileTbl[12] = {
    0x00BC0072, 0x00BC0074, 0x00BC0076, 0x00BC0078, 0x00BC007A, 0x00BC007C,
    0x00BC007E, 0x00BC0080, 0x00BC0082, 0x00BC0084, 0x00BC0087, 0x00BC008A,
};

static f32 cameraYOfsTbl[10] = {
    400.0f, 0.0f, 1000.0f, 800.0f, 500.0f, 1000.0f, 1000.0f, 1000.0f, 500.0f, 0.0f,
};

void MachineDispSet(SdMachine *machine, s32 disp)
{
    s32 i;

    if (disp == 0) {
        Hu3DModelAttrSet(machine->mdl3B4, 1);
        Hu3DModelAttrSet(machine->mdlId, 1);
        Hu3DModelAttrSet(machine->gunMdlId, 1);
        Hu3DModelAttrSet(machine->mdl3AE, 1);
        for (i = 0; i < 4; i++) {
            if (machine->hookMdlId[i] != -1) {
                Hu3DModelAttrSet(machine->hookMdlId[i], 1);
            }
        }
    } else {
        Hu3DModelAttrReset(machine->mdl3B4, 1);
        Hu3DModelAttrReset(machine->mdlId, 1);
        Hu3DModelAttrReset(machine->gunMdlId, 1);
        Hu3DModelAttrReset(machine->mdl3AE, 1);
        for (i = 0; i < 4; i++) {
            if (machine->hookMdlId[i] != -1) {
                Hu3DModelAttrReset(machine->hookMdlId[i], 1);
            }
        }
    }
}

void MachineScaleSet(SdMachine *machine, Vec *scale)
{
    s32 i;

    Hu3DModelScaleSet(machine->mdlId, scale->x, scale->y, scale->z);
    Hu3DModelScaleSet(machine->gunMdlId, scale->x, scale->y, scale->z);
    Hu3DModelScaleSet(machine->mdl3AE, scale->x, scale->y, scale->z);
    for (i = 0; i < 4; i++) {
        if (machine->hookMdlId[i] != -1) {
            Hu3DModelScaleSet(machine->hookMdlId[i], scale->x, scale->y, scale->z);
        }
    }
}

void MachineCameraBitSet(SdMachine *machine, u16 cameraBit)
{
    s32 i;

    Hu3DModelCameraSet(machine->mdlId, cameraBit);
    Hu3DModelCameraSet(machine->gunMdlId, cameraBit);
    Hu3DModelCameraSet(machine->mdl3AE, cameraBit);
    for (i = 0; i < 4; i++) {
        if (machine->hookMdlId[i] != -1) {
            Hu3DModelCameraSet(machine->hookMdlId[i], cameraBit);
        }
    }
    Hu3DModelCameraSet(machine->mdl3B4, cameraBit);
}

void MachineCameraInit(SdMachine *machine, s32 mode)
{
    SdCamera *cam;
    Vec eye;
    Vec look;
    Vec up;
    Vec fwd;
    Vec dir;
    f32 dot;
    f32 follow;

    cam = &sys->camera[machine->cameraNo];
    if (cam->active == 0) {
        return;
    }
    cam->offsetOn = 1;
    cam->offsetDist = 1000.0f;
    MachineVelocityApply(machine, &eye, &look);
    if (mode == 0) {
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        CameraSetViewPos(machine->cameraNo, &eye, &up, &look);
        return;
    }
    PSVECSubtract(&Hu3DCamera[machine->cameraNo].target, &Hu3DCamera[machine->cameraNo].pos, &dir);
    fwd.x = machine->mtx[0][2];
    fwd.y = machine->mtx[1][2];
    fwd.z = machine->mtx[2][2];
    fwd.y = 0.0f;
    dir.y = 0.0f;
    PSVECNormalize(&dir, &dir);
    PSVECNormalize(&fwd, &fwd);
    dot = PSVECDotProduct(&dir, &fwd);
    follow = 0.25f + 0.75f * (1.0f - dot);
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    SdCameraViewSet(machine->cameraNo, &eye, &up, &look, follow);
}

void MachineVelocityApply(SdMachine *machine, Vec *eye, Vec *look)
{
    Vec v;

    v.x = machine->mtx[0][2];
    v.y = machine->mtx[1][2];
    v.z = machine->mtx[2][2];
    v.y = 0.0f;
    PSVECNormalize(&v, &v);
    PSVECScale(&v, &v, -500.0f);
    v.y += 350.0f;
    PSVECAdd(&machine->curPos, &v, eye);
    *look = machine->curPos;
    look->y += 300.0f;
}

void MachineViewUpdate(s32 cameraNo, s32 flip, Vec *from, Vec *to, Vec *out, f32 t)
{
    SdCamera *cam;
    Vec v0;
    Vec v1;
    Vec r;
    Mtx m;
    f32 mag0;
    f32 mag1;
    f32 ang0;
    f32 ang1;
    f32 dAng;
    f32 dMag;
    f32 dY;

    cam = &sys->camera[cameraNo];
    if (t >= 1.0f) {
        cam->goalPos = *to;
        cam->pos = cam->goalPos;
        return;
    }
    cam->active = 0;
    PSVECSubtract(from, out, &v0);
    v0.y = 0.0f;
    mag0 = PSVECMag(&v0);
    if (0.0f == PSVECMag(&v0)) {
        v0.x = 1.0f;
        v0.y = 1.0f;
        v0.z = 1.0f;
    }
    PSVECNormalize(&v0, &v0);
    ang0 = (f32)(180.0 * (atan2(v0.x, v0.z) / 3.141592653589793));

    PSVECSubtract(to, out, &v1);
    v1.y = 0.0f;
    mag1 = PSVECMag(&v1);
    if (0.0f == PSVECMag(&v1)) {
        v1.x = 1.0f;
        v1.y = 1.0f;
        v1.z = 1.0f;
    }
    PSVECNormalize(&v1, &v1);
    ang1 = (f32)(180.0 * (atan2(v1.x, v1.z) / 3.141592653589793));

    dAng = ang1 - ang0;
    dMag = mag1 - mag0;
    if (flip == 1) {
        dAng *= -1.0f;
    }
    out->y = 0.0f;
    r.x = 0.0f;
    r.y = 0.0f;
    r.z = mag0 + dMag * t;
    PSMTXRotRad(m, 'y', 0.017453292f * (ang0 + dAng * t));
    PSMTXMultVec(m, &r, &r);
    PSVECAdd(&r, out, out);
    dY = to->y - from->y;
    out->y = from->y + dY * t;
    cam->goalPos = *out;
    cam->pos = cam->goalPos;
}

void MachinePosSet(SdMachine *machine, Vec *pos)
{
    machine->basePos = *pos;
    machine->curPos = machine->basePos;
    machine->curPos.y += machine->wheelBase;
    Hu3DModelPosSetV(machine->mdlId, &machine->curPos);
}

void MachinePosInit(SdMachine *machine)
{
    Vec pos;
    Vec rot;

    pos.x = 0.0f;
    pos.y = 300.0f;
    pos.z = 0.0f;
    if (sys->stageNo == 7 || sys->stageNo == 6) {
        pos.x = -1400.0f + 100.0f * (f32)(machine->playerNo * 28);
        pos.z = -1400.0f + 100.0f * (f32)(machine->playerNo * 28);
    } else if (sys->stageNo == 5) {
        pos.x = -1900.0f + 100.0f * (f32)(machine->playerNo * 38);
        pos.z = -1400.0f + 100.0f * (f32)(machine->playerNo * 28);
    } else if (sys->stageNo == 3) {
        if (machine->kind == 9) {
            pos.x = -1904.9999f + 100.0f * (38.1f * (f32)machine->playerNo);
            pos.z = -1900.0f + 100.0f * (f32)(machine->playerNo * 38);
        } else {
            pos.x = -1900.0f + 100.0f * (f32)(machine->playerNo * 38);
            pos.z = -1900.0f + 100.0f * (f32)(machine->playerNo * 38);
        }
    } else {
        pos.x = -1900.0f + 100.0f * (f32)(machine->playerNo * 38);
        pos.z = -1900.0f + 100.0f * (f32)(machine->playerNo * 38);
    }
    MachinePosSet(machine, &pos);
    rot.x = 0.0f;
    rot.y = 90.0f + 180.0f * (f32)machine->playerNo;
    rot.z = 0.0f;
    MachineRotSet(machine, &rot);
}

void MachineRotSet(SdMachine *machine, Vec *rot)
{
    MathMtxRotSet(rot, &machine->rot, &machine->mtx);
    Hu3DModelRotSetV(machine->mdlId, &machine->rot);
    PSMTXCopy(machine->mtx, machine->storedMtx);
    machine->storedRot = machine->rot;
}

void MachineRotApply(SdMachine *machine, Vec *rot)
{
    Mtx m;

    PSMTXRotRad(m, 'y', 0.017453292f * rot->y);
    PSMTXConcat(machine->mtx, m, machine->mtx);
    PSMTXRotRad(m, 'x', 0.017453292f * rot->x);
    PSMTXConcat(machine->mtx, m, machine->mtx);
    PSMTXRotRad(m, 'z', 0.017453292f * rot->z);
    PSMTXConcat(machine->mtx, m, machine->mtx);
    MathMtxRotGet(machine->mtx, &machine->rot);
    Hu3DModelRotSetV(machine->mdlId, &machine->rot);
    PSMTXCopy(machine->mtx, machine->storedMtx);
    machine->storedRot = machine->rot;
}

void MachineRotAngleSet(SdMachine *machine, Vec *rot)
{
    MathMtxRotSet(rot, &machine->rot, NULL);
    Hu3DModelRotSetV(machine->mdlId, &machine->rot);
    machine->storedRot = machine->rot;
}

void MachineRotAngleApply(SdMachine *machine, Vec *rot)
{
    Mtx result;
    Mtx m;

    PSMTXIdentity(result);
    PSMTXRotRad(m, 'y', 0.017453292f * rot->y);
    PSMTXConcat(result, m, result);
    PSMTXRotRad(m, 'x', 0.017453292f * rot->x);
    PSMTXConcat(result, m, result);
    PSMTXRotRad(m, 'z', 0.017453292f * rot->z);
    PSMTXConcat(result, m, result);
    PSMTXConcat(result, machine->mtx, result);
    MathMtxRotGet(result, &machine->rot);
    Hu3DModelRotSetV(machine->mdlId, &machine->rot);
    machine->storedRot = machine->rot;
}

void SdCharInit(SdMachine *machine1, SdMachine *machine2)
{
    SdMachine *machine;
    s32 i;
    s32 modelData;
    void *motData;

    for (i = 0; i < 2; i++) {
        machine = (i == 0) ? machine1 : machine2;
        if (machine->charNo < 10) {
            modelData = charDataDirTbl[machine->charNo][3];
        } else if (machine->charNo == 10) {
            modelData = 0x00BC0085;
        } else {
            modelData = 0x00BC0088;
        }
        machine->charMdlId = Hu3DModelCreateData(modelData);
        if (i == 0) {
            motData = HuDataSelHeapReadNum(p1MotFileTbl[machine->charNo], HU_MEMNUM_OVL, HEAP_MODEL);
        } else {
            motData = HuDataSelHeapReadNum(p2MotFileTbl[machine->charNo], HU_MEMNUM_OVL, HEAP_MODEL);
        }
        machine->charMotId = Hu3DJointMotion(machine->charMdlId, motData);
        Hu3DMotionSet(machine->charMdlId, machine->charMotId);
        Hu3DMotionTimeSet(machine->charMdlId, 1.0f);
        Hu3DModelShadowSet(machine->charMdlId);
    }
}

void SdCameraCreate(void)
{
    SdCamera *cam;
    s32 i;

    for (i = 0; i < 2; i++) {
        cam = &sys->camera[i];
        cam->bit = sdCameraBitTbl[i];
        cam->up.x = 0.0f;
        cam->up.y = 1.0f;
        cam->up.z = 0.0f;
        cam->goalPos.x = 0.0f;
        cam->goalPos.y = 0.0f;
        cam->goalPos.z = 2000.0f;
        cam->pos.x = 0.0f;
        cam->pos.y = 0.0f;
        cam->pos.z = 500.0f;
        cam->follow = 0.1f;
        Hu3DCameraCreate(sdCameraBitTbl[i]);
        Hu3DCameraPosSetV(sdCameraBitTbl[i], &cam->pos, &cam->up, &cam->target);
    }
    Hu3DCameraViewportSet(sdCameraBitTbl[0], 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(sdCameraBitTbl[0], 0, 0, 640, 480);
    Hu3DCameraPerspectiveSet(sdCameraBitTbl[0], 60.0f, 20.0f, 35000.0f, 1.2f);
    Hu3DCameraViewportSet(sdCameraBitTbl[1], 640.0f, 0.0f, 32.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(sdCameraBitTbl[1], 640, 0, 32, 480);
    Hu3DCameraPerspectiveSet(sdCameraBitTbl[1], 60.0f, 20.0f, 35000.0f, 0.060000002f);
}

void SdCameraSplitSet(f32 split)
{
    f32 x = 0.0f;
    f32 w = split;
    f32 w2;

    if (w < 32.0f) {
        w = 32.0f;
        x = split - 32.0f;
    }
    Hu3DCameraViewportSet(sdCameraBitTbl[0], x, 0.0f, w, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(sdCameraBitTbl[0], 0, 0, (u32)split, 480);
    Hu3DCameraPerspectiveSet(sdCameraBitTbl[0], 60.0f, 20.0f, 35000.0f, 1.2f * (w / 640.0f));
    w2 = 640.0f - split;
    if (w2 < 32.0f) {
        w2 = 32.0f;
    }
    Hu3DCameraViewportSet(sdCameraBitTbl[1], split, 0.0f, w2, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(sdCameraBitTbl[1], (u32)split, 0, (u32)w2, 480);
    Hu3DCameraPerspectiveSet(sdCameraBitTbl[1], 60.0f, 20.0f, 35000.0f, 1.2f * (w2 / 640.0f));
    espPosSet(sys->cameraSprId, 0.9f * split, 240.0f);
}

void SdCameraUpdate(void)
{
    SdCamera *cam;
    Vec dir;
    Vec dPos;
    Vec dTgt;
    Vec eye;
    Vec eyeCopy;
    Vec toEye;
    f32 ang;
    f32 edge;
    f32 sign;
    s32 i;

    for (i = 0; i < 2; i++) {
        PSVECSubtract(&Hu3DCamera[i].target, &Hu3DCamera[i].pos, &dir);
        PSVECNormalize(&dir, &dir);
        ang = (f32)(180.0 * (atan2(dir.x, dir.z) / 3.141592653589793));
        cam = &sys->camera[i];
        PSVECSubtract(&cam->goalPos, &cam->pos, &dPos);
        PSVECSubtract(&cam->goalTarget, &cam->target, &dTgt);
        PSVECScale(&dPos, &dPos, cam->follow);
        PSVECScale(&dTgt, &dTgt, cam->follow);
        PSVECAdd(&cam->pos, &dPos, &cam->pos);
        PSVECAdd(&cam->target, &dTgt, &cam->target);
        eye = cam->pos;
        eyeCopy = eye;
        edge = SdStageEdgeDistRadiusGet(&eyeCopy, 500.0f);
        if (edge < 0.0f) {
            edge = 0.0f;
        }
        if (edge < 1500.0f) {
            eye.y += cameraYOfsTbl[sys->stageNo] * MathLerp(0, (1500.0f - edge) / 1500.0f);
        }
        if (cam->offsetOn != 0) {
            PSVECSubtract(&eye, &cam->target, &toEye);
            PSVECNormalize(&toEye, &toEye);
            PSVECScale(&toEye, &toEye, cam->offsetDist);
            PSVECAdd(&toEye, &cam->target, &eye);
        }
        Hu3DCameraPosSetV(cam->bit, &eye, &cam->up, &cam->target);
        PSVECSubtract(&Hu3DCamera[i].target, &Hu3DCamera[i].pos, &dir);
        PSVECNormalize(&dir, &dir);
        SdStageBackRotYSet(i, (f32)(180.0 * (atan2(dir.x, dir.z) / 3.141592653589793)), ang);
        if (cam->shake > 0.0f) {
            sign = (rand8() % 2 != 0) ? -1.0f : 1.0f;
            Hu3DCamera[i].pos.x += cam->shake * sign;
            Hu3DCamera[i].target.x += cam->shake * sign;
            sign = (rand8() % 2 != 0) ? -1.0f : 1.0f;
            Hu3DCamera[i].pos.y += cam->shake * sign;
            Hu3DCamera[i].target.y += cam->shake * sign;
            sign = (rand8() % 2 != 0) ? -1.0f : 1.0f;
            Hu3DCamera[i].pos.z += cam->shake * sign;
            Hu3DCamera[i].target.z += cam->shake * sign;
            cam->shake *= 0.9f;
        }
    }
    if (sys->stageNo == 2) {
        Hu3DCamera[2] = Hu3DCamera[0];
        Hu3DCamera[3] = Hu3DCamera[1];
    }
}

void SdSinLerp(Vec *out, Vec *a, Vec *b, f32 t)
{
    Vec v;
    f64 s;
    f32 k;

    if (t > 1.0f) {
        t = 1.0f;
    }
    s = sin(3.141592653589793 * (f64)(90.0f * t) / 180.0);
    k = (f32)(sin(3.141592653589793 * (f64)(90.0f * t) / 180.0) * s);
    v.x = a->x + k * (b->x - a->x);
    v.y = a->y + k * (b->y - a->y);
    v.z = a->z + k * (b->z - a->z);
    *out = v;
}

void CameraSetViewPos(s32 cameraNo, Vec *pos, Vec *up, Vec *target)
{
    SdCamera *cam = &sys->camera[cameraNo];

    cam->up = *up;
    cam->beginPos = *pos;
    cam->pos = cam->beginPos;
    cam->goalPos = cam->pos;
    cam->beginTarget = *target;
    cam->target = cam->beginTarget;
    cam->goalTarget = cam->target;
}

void SdCameraViewSet(s32 cameraNo, Vec *pos, Vec *up, Vec *target, f32 follow)
{
    SdCamera *cam = &sys->camera[cameraNo];

    cam->up = *up;
    cam->goalPos = *pos;
    cam->goalTarget = *target;
    cam->follow = follow;
}

void SdCameraViewBeginSet(s32 cameraNo, Vec *pos, Vec *up, Vec *target)
{
    SdCamera *cam = &sys->camera[cameraNo];

    cam->up = *up;
    cam->beginPos = *pos;
    cam->beginTarget = *target;
}

void SdCameraViewEndSet(s32 cameraNo, Vec *pos, Vec *up, Vec *target)
{
    SdCamera *cam = &sys->camera[cameraNo];

    cam->up = *up;
    cam->endPos = *pos;
    cam->endTarget = *target;
    cam->viewTime = 0.0f;
}

void SdCameraViewSpeedSet(s32 cameraNo, f32 time)
{
    SdCamera *cam = &sys->camera[cameraNo];

    cam->viewTime = time;
}

s32 SdCameraViewUpdate(s32 cameraNo, Vec *up, f32 duration)
{
    SdCamera *cam = &sys->camera[cameraNo];
    Vec pos;
    Vec target;
    f32 t;
    f64 s;
    f32 k;

    cam->viewTime += 1.0f / duration;
    t = cam->viewTime;
    if (t > 1.0f) {
        t = 1.0f;
    }
    s = sin(3.141592653589793 * (f64)(90.0f * t) / 180.0);
    k = (f32)(sin(3.141592653589793 * (f64)(90.0f * t) / 180.0) * s);
    pos.x = cam->beginPos.x + k * (cam->endPos.x - cam->beginPos.x);
    pos.y = cam->beginPos.y + k * (cam->endPos.y - cam->beginPos.y);
    pos.z = cam->beginPos.z + k * (cam->endPos.z - cam->beginPos.z);

    t = cam->viewTime;
    if (t > 1.0f) {
        t = 1.0f;
    }
    s = sin(3.141592653589793 * (f64)(90.0f * t) / 180.0);
    k = (f32)(sin(3.141592653589793 * (f64)(90.0f * t) / 180.0) * s);
    target.x = cam->beginTarget.x + k * (cam->endTarget.x - cam->beginTarget.x);
    target.y = cam->beginTarget.y + k * (cam->endTarget.y - cam->beginTarget.y);
    target.z = cam->beginTarget.z + k * (cam->endTarget.z - cam->beginTarget.z);

    cam->up = *up;
    cam->pos = pos;
    cam->goalPos = cam->pos;
    cam->target = target;
    cam->goalTarget = cam->target;
    if (1.0f <= cam->viewTime) {
        return 1;
    }
    return 0;
}

void SdVibrate(s32 playerNo, s32 level, s32 time)
{
    s32 start = 0;

    if (GwPlayerConf[playerNo].type != 0) {
        return;
    }
    if (sys->vibrate[playerNo][0] < level) {
        start = 1;
    } else if (sys->vibrate[playerNo][0] == level && sys->vibrate[playerNo][1] < time) {
        start = 1;
    }
    if (start == 0) {
        return;
    }
    switch (level) {
        case 0:
            omVibrate(sys->machine[playerNo].playerNo, time, 6, 6);
            break;
        case 1:
            omVibrate(sys->machine[playerNo].playerNo, time, 4, 2);
            break;
        case 2:
            omVibrate(sys->machine[playerNo].playerNo, time, 12, 0);
            break;
    }
    sys->vibrate[playerNo][0] = level;
    sys->vibrate[playerNo][1] = time;
}

void SdVibrateUpdate(void)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        sys->vibrate[i][1]--;
        if (sys->vibrate[i][1] < 0) {
            sys->vibrate[i][1] = 0;
            sys->vibrate[i][0] = -1;
        }
    }
}
