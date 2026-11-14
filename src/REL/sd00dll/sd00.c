#include "REL/sd00dll_sys.h"
#include "game/audio.h"
#include "game/esprite.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"
#include "game/mgdata.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/wipe.h"
#include "game/window.h"
#include "string.h"


/* GC layout (checked): sizeof(SdSys)=0xFF4, SdMachine=0x3F8, SdMachineTrack=0x110, SdCamera=0x88, SdSprWork=0xC0 */

/* ---- functions of other units (exact asm signatures) ---- */
extern void SdFlagClose(void);
extern void SdRabbitClose(void);
extern void SdFlagInit(void);
extern void SdFlagUpdate(void);
extern void SdRabbitInit(void);
extern void SdRabbitUpdate(void);
extern void SdSprCreate(void);
extern void SdSprUpdate(void);
extern void SdSprHideAll(void);
extern void SdHpSprCreate(void);
extern void SdHpSprKill(void);
extern void SdSprScissorUpdate(s16 sprId, s32 playerNo);
extern void SdMachineConfigSet(s32 playerNo);
extern void SdMachineEffectCreate(void);
extern void SdMachineCreate(SdMachine *machine);
extern void SdMachineFxUpdate(SdMachine *machine);
extern void SdMachineFxStop(SdMachine *machine);
extern void SdBulletUpdate(SdMachine *machine);
extern void SdImpactUpdate(void);
extern s32 rand8(void);

/* ---- statics of sd00 parts B/C (defined in the .inc files below) ---- */
static void MachineEffectUpdate(SdMachine *machine);
static void MachineModeIdle(SdMachine *machine);
static void MachineMotionUpdate(SdMachine *machine);
static void MachineModeDamage(SdMachine *machine);
static void MachineModeLaunch(SdMachine *machine);
static void MachineModeDie(SdMachine *machine);
static void MachineColUpdate(void);
static void MachineColSeparate(SdMachine *machine);
void PlayDamageSound(void);

static void SdSeqInit(s16 mode, s16 frameNo);
static void SdSeqFadeIn(s16 mode, s16 frameNo);
static void SdSeqStart(s16 mode, s16 frameNo);
static void SdSeqMain(s16 mode, s16 frameNo);
static void SdSeqFinish(s16 mode, s16 frameNo);
static void SdSeqPreWinner(s16 mode, s16 frameNo);
static void SdSeqWinner(s16 mode, s16 frameNo);
static void SdSeqFadeOut(s16 mode, s16 frameNo);
static void SdSeqClose(s16 mode, s16 frameNo);
static void SdSeqUpdate(void);
static void SdSeqMachineSet(SdMachine *machine, s32 no, s32 kind, s32 bodyType, s32 engineType, s32 bulletType, s32 charNo);
static s32 SdSeqMachineUpdate(SdMachine *machine);
static void MachineParticleUpdate(SdMachine *machine);
static void MachineExhaustUpdate(SdMachine *machine);
static void MachineExhaustAdd(SdMachine *machine);
static void MachineTrackEffUpdate(SdMachine *machine);
static void MachineTrackEffAdd(SdMachine *machine);

extern void (*_dtors[])(void);
extern void (*_ctors[])(void);
extern void ObjectSetup(void);

/* ---- data ---- */
SdSys *sys;

s32 sdCameraBitTbl[8] = { 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80 };

static MGSEQ_PARAM seqModeTbl = {
    0,
    MGSEQ_TIMER_TOP,
    SdSeqInit,
    SdSeqFadeIn,
    SdSeqStart,
    SdSeqMain,
    SdSeqFinish,
    SdSeqPreWinner,
    SdSeqWinner,
    SdSeqFadeOut,
    SdSeqClose,
};

/* winner voice delay per charNo */
static u8 winnerSoundDelayTbl[12] = { 0x0D, 0x00, 0x17, 0x69, 0x0D, 0x1D, 0x14, 0x0D, 0x0D, 0x0D, 0x00, 0x00 };

/* exhaust particle color per bodyType */
static GXColor particleColorTbl[11] = {
    { 0xFF, 0xFF, 0xFF, 0xB4 },
    { 0xFF, 0xC0, 0x00, 0xB4 },
    { 0xFF, 0xC4, 0xDC, 0xB4 },
    { 0xFF, 0xFF, 0xFF, 0xB4 },
    { 0x00, 0xFF, 0x4C, 0xB4 },
    { 0xFF, 0xA8, 0x7D, 0xB4 },
    { 0xFF, 0xFF, 0xFF, 0xB4 },
    { 0xC6, 0x00, 0xFF, 0xB4 },
    { 0xBE, 0x71, 0x35, 0xB4 },
    { 0x00, 0xFF, 0xFF, 0xB4 },
    { 0x40, 0x40, 0x96, 0xB4 },
};

int _prolog(void)
{
    void (**ctors)(void) = _ctors;
    while (*ctors) {
        (*ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    void (**dtors)(void) = _dtors;
    while (*dtors) {
        (*dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    sys = HuMemDirectMallocNum(HEAP_MODEL, sizeof(SdSys), HU_MEMNUM_OVL);
    memset(sys, 0, sizeof(SdSys));
    sys->objman = omInitObjMan(0xC8, 0x2000);
    omGameSysInit(sys->objman);
    SdCameraCreate();
    sys->cameraObj = omAddObjEx(sys->objman, 0, 0, 0, -1, (OMOBJFUNC)SdCameraUpdate);
    sys->baseShadowMdlId = -1;
    sys->lightId = -1;
    sys->fxId = -1;
    sys->bgmStream = -1;
    MgSeqCreate(&seqModeTbl);
    HuWinInit(1);
}

static void SdSeqInit(s16 mode, s16 frameNo)
{
    s32 i;
    SdMachine *machine;
    s32 comPreset;
    Vec dir;
    Vec eye;
    Vec look;
    Vec up;

    Hu3DBGColorSet(0, 0, 0);
    Hu3DShadowMultiCreate(60.0f, 20.0f, 35000.0f, 3);
    Hu3DShadowMultiTPLvlSet(0.5f, 3);
    Hu3DShadowMultiSizeSet(0x80, 3);
    sys->gameType = GwSdBattle.gameType;
    sys->seqStepA = GwSdBattle.stageNo;
    if (GwSdBattle.stageType1 != -1) {
        sys->stageType = GwSdBattle.stageType1;
    } else {
        sys->stageType = GwSdBattle.stageType2;
    }
    for (i = 0; i < 2; i++) {
        SdMachineConfigSet(i);
        sys->padNo[i] = GwSdBattle.padNo[i];
    }
    sys->seqStep = 0;
    SdMachineEffectCreate();
    SdStageInit(sys->seqStepA);
    sys->seqStepA = 0;
    for (i = 0; i < 2; i++) {
        machine = &sys->machine[i];
        comPreset = machine->comPreset;
        SdSeqMachineSet(machine, i, machine->kind, machine->bodyType, machine->engineType, machine->bulletType, machine->charNo);
        machine->comPreset = comPreset;
        MachinePosInit(machine);
        MachineCameraInit(machine, 0);
    }
    SdMachineParticleCreate();
    SdStageObjCreate();
    SdSprCreate();
    SdCameraSplitSet(640.0f);
    for (i = 0; i < 60; i++) {
        SdSeqUpdate();
    }
    for (i = 0; i < 2; i++) {
        machine = &sys->machine[i];
        dir.x = machine->mtx[0][2];
        dir.y = machine->mtx[1][2];
        dir.z = machine->mtx[2][2];
        PSVECNormalize(&dir, &dir);
        PSVECScale(&dir, &dir, 1500.0f);
        PSVECAdd(&machine->curPos, &dir, &dir);
        dir.y += 300.0f;
        eye = dir;
        look = machine->curPos;
        look.y += 100.0f;
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        CameraSetViewPos(i, &eye, &up, &look);
    }
    SdFlagInit();
    SdRabbitInit();
    MgSeqModeNext();
}

static void SdSeqFadeIn(s16 mode, s16 frameNo)
{
    SdSprWork *spr = &sys->spr;
    s32 i;
    s32 j;
    s32 k;
    s32 flag;
    s32 hp;
    s32 base;
    s32 off;
    s32 step;
    f32 t;
    f32 v;
    Vec eye;
    Vec look;
    Vec from;
    Vec to;
    Vec cur;

    switch (sys->seqStep) {
        case 0:
            SdAuxSet();
            HuAudJinglePlay(0x85);
            SdHpSprCreate();
            sys->seqStep++;
            /* fallthrough */
        case 1:
            for (i = 0; i < 2; i++) {
                sys->camera[i].offsetOn = 1;
                sys->camera[i].offsetDist = 500.0f;
            }
            sys->seqTimerC = 0;
            sys->seqTimerB = 0;
            sys->seqTimerA = 0;
            sys->seqTimer = 0;
            sys->seqStepC = 0;
            sys->seqStepB = 0;
            sys->seqStepA = 0;
            sys->seqStep++;
            break;
        case 2:
            sys->seqTimer++;
            t = MathLerp(2, (f32)sys->seqTimer / 234.0f);
            for (i = 0; i < 2; i++) {
                sys->camera[i].offsetDist = 500.0f + 500.0f * t;
                MachineVelocityApply(&sys->machine[i], &eye, &look);
                v = sys->machine[i].curPos.y + 300.0f * t;
                sys->camera[i].target.y = v;
                sys->camera[i].goalTarget.y = v;
                cur.x = 0.0f;
                cur.y = 0.0f;
                cur.z = 0.0f;
                cur.x = (300.0f + sys->machine[i].curPos.x) - 600.0f * (f32)i;
                cur.y = 100.0f + sys->machine[i].curPos.y;
                cur.z = sys->machine[i].curPos.z;
                from = cur;
                to = sys->machine[i].curPos;
                MachineViewUpdate(i, i, &from, &eye, &to, t);
            }
            if (t == 1.0f) {
                sys->camera[1].active = 1;
                sys->camera[0].active = 1;
            }
            if ((f32)sys->seqTimer == 60.0f && sys->seqStepA == 0) {
                sys->seqStepA = 1;
            }
            if ((f32)sys->seqTimer == 108.0f && sys->seqStepB == 0 && sys->gameType == 0) {
                sys->seqStepB = 1;
            }
            if ((f32)sys->seqTimer == 138.0f && sys->seqStepC == 0) {
                sys->seqStepC = 1;
            }
            if ((f32)sys->seqTimer > 234.0f) {
                sys->seqStep++;
            }
            break;
        case 3:
            sys->seqTimer++;
            SdHpSprKill();
            sys->seqStep++;
            break;
        case 4:
            sys->seqTimer++;
            break;
    }
    switch (sys->seqStepA) {
        case 0:
        case 4:
            break;
        case 1:
            espDispOn(spr->mapSpr);
            espScaleSet(spr->mapSpr, 0.5f * ((f32)sys->seqTimerA / 12.0f), 0.05f);
            sys->seqTimerA++;
            if (!((f32)sys->seqTimerA < 12.0f)) {
                sys->seqTimerA = 0;
                sys->seqStepA++;
            }
            break;
        case 2:
            espScaleSet(spr->mapSpr, 0.5f, 0.05f + 0.45f * ((f32)sys->seqTimerA / 9.0f));
            sys->seqTimerA++;
            if (!((f32)sys->seqTimerA < 9.0f)) {
                espScaleSet(spr->mapSpr, 0.5f, 0.5f);
                sys->seqTimerA = 0;
                sys->seqStepA++;
            }
            break;
        case 3:
            flag = 0;
            for (i = 0; i < 2; i++) {
                for (j = 0; j < 3; j++) {
                    if ((f32)sys->seqTimerA == 6.0f * (f32)j) {
                        espDispOn(spr->zoomSpr[i][j]);
                    }
                    step = (s32)((f32)sys->seqTimerA - 6.0f * (f32)j);
                    v = (f32)step / 12.0f;
                    if (v > 1.0f) {
                        v = 1.0f;
                        if (j == 2) {
                            flag = 1;
                        }
                    }
                    espScaleSet(spr->zoomSpr[i][j], 2.5f - 2.0f * v, 2.5f - 2.0f * v);
                    espTPLvlSet(spr->zoomSpr[i][j], 0.1f + 0.9f * v);
                }
            }
            sys->seqTimerA++;
            if (flag != 0) {
                for (i = 0; i < 2; i++) {
                    for (j = 0; j < 3; j++) {
                        espDispOff(spr->zoomSpr[i][j]);
                    }
                    espDispOn(spr->playerSpr[i]);
                }
                sys->seqTimerA = 0;
                sys->seqStepA++;
            }
            break;
    }
    if (sys->seqStepB == 1) {
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 8; j++) {
                hp = sys->machine[i].hp - j * 4;
                base = (s32)(10.0f * (f32)j);
                flag = 0;
                for (k = 0; k < 3; k++) {
                    off = (s32)(6.0f * (f32)k);
                    if (sys->seqTimerB == base + off && hp > 0) {
                        espDispOn(spr->zoomHpIcon[i][j][k]);
                    }
                    v = (f32)(sys->seqTimerB - (base + off)) / 12.0f;
                    if (v > 1.0f) {
                        v = 1.0f;
                        if (k == 2) {
                            flag = 1;
                            if (j == 7) {
                                flag = 2;
                            }
                        }
                    }
                    espScaleSet(spr->zoomHpIcon[i][j][k], 2.6f - 2.0f * v, 2.6f - 2.0f * v);
                    espTPLvlSet(spr->zoomHpIcon[i][j][k], 0.1f + 0.9f * v);
                }
                if (flag != 0) {
                    if (hp > 0) {
                        espDispOn(spr->hpIcon[i][j]);
                    }
                    for (k = 0; k < 3; k++) {
                        espDispOff(spr->zoomHpIcon[i][j][k]);
                    }
                }
                for (k = 0; k < 3; k++) {
                    if (spr->zoomHpIcon[i][j][k] != -1) {
                        SdSprScissorUpdate(spr->zoomHpIcon[i][j][k], i);
                    }
                }
                SdSprScissorUpdate(spr->hpIcon[i][j], i);
            }
        }
        sys->seqTimerB++;
        if (flag == 2) {
            sys->seqTimerB = 0;
            sys->seqStepB++;
        }
    }
    if (sys->seqStepC == 1) {
        sys->seqTimerC++;
        SdCameraSplitSet(640.0f - 320.0f * MathLerp(1, (f32)sys->seqTimerC / 90.0f));
        if ((f32)sys->seqTimerC > 90.0f) {
            SdCameraSplitSet(320.0f);
            sys->seqTimerC = 0;
            sys->seqStepC++;
        }
    }
    SdSeqUpdate();
}

static void SdSeqStart(s16 mode, s16 frameNo)
{
    sys->seqTimer++;
    sys->seqStep = 0;
    if (sys->bgmStream == -1 && (GameMesStatGet(-1) & 0x10) != 0) {
        sys->bgmStream = HuAudBGMPlay(sys->gameType + 0x2C);
    }
    SdSeqUpdate();
    if (sys->timer == NULL) {
        sys->timer = MgTimerCreate(0);
        MgTimerParamSet(sys->timer, 0x1518, 0, 0);
    }
}

static void SdSeqMain(s16 mode, s16 frameNo)
{
    s32 end;
    s32 winner;
    s32 allDone;
    s32 i;

    if (MgSeqFrameNoGet() == 0) {
        sys->seqStep = 0;
        if (sys->noTimerF == 0) {
            MgTimerModeOnSet(sys->timer, 1);
        }
    }
    sys->seqTimer++;
    sys->timeSec = MgTimerValueGet(sys->timer);
    sys->timeSec = (s32)((f32)sys->timeSec / 60.0f);
    SdSeqUpdate();
    end = 0;
    winner = 0;
    if (sys->gameType == 0) {
        if (sys->machine[0].hp <= 0 || sys->machine[1].hp <= 0) {
            end = 1;
        }
    } else if (sys->gameType == 1) {
        if (SdFlagDispTimerGet(0) >= 3) {
            end = 1;
            winner = 1;
        } else if (SdFlagDispTimerGet(1) >= 3) {
            end = 2;
            winner = 2;
        }
    } else if (sys->gameType == 2) {
        if (SdRabbitDispTimerGet(0) >= 3) {
            end = 1;
            winner = 1;
        } else if (SdRabbitDispTimerGet(1) >= 3) {
            end = 2;
            winner = 2;
        }
    }
    if (winner != 0 && sys->machine[0].mode < 2 && sys->machine[1].mode < 2) {
        if (winner == 1) {
            sys->machine[0].mode = 5;
            sys->machine[1].mode = 4;
        } else {
            sys->machine[1].mode = 5;
            sys->machine[0].mode = 4;
        }
    }
    allDone = 1;
    if (sys->machine[0].mode == 4 && sys->machine[1].mode != 4) {
        MgTimerModeOffSet(sys->timer);
        sys->machine[1].mode = 5;
    }
    if (sys->machine[1].mode == 4 && sys->machine[0].mode != 4) {
        MgTimerModeOffSet(sys->timer);
        sys->machine[0].mode = 5;
    }
    for (i = 0; i < 2; i++) {
        if (sys->machine[i].mode < 4) {
            allDone = 0;
            break;
        }
        if (sys->machine[i].unk4 == 0) {
            allDone = 0;
            break;
        }
    }
    if (end == 0 && MgTimerDoneCheck(sys->timer) != 0) {
        sys->timeUpF = 1;
    }
    if (allDone == 1 || sys->timeUpF != 0) {
        if (allDone == 1 && sys->timeUpF == 0) {
            sys->allDoneF = 1;
        }
        if (sys->noTimerF == 0 && sys->timer->stopF == 0) {
            sys->timer->stopF = 1;
            sys->timer->mode = 1;
        }
        MgSeqModeNext();
    }
}

static void SdSeqFinish(s16 mode, s16 frameNo)
{
    SdSeqUpdate();
    sys->spr.winnerNo = -1;
    if (sys->machine[0].mode == 5) {
        sys->spr.winnerNo = 0;
    } else if (sys->machine[1].mode == 5) {
        sys->spr.winnerNo = 1;
    }
    if (sys->bgmStream != -1) {
        HuAudSStreamFadeOut(sys->bgmStream, 0x64);
        sys->bgmStream = -1;
    }
    if (sys->spr.winnerNo == -1) {
        SdMachineFxStop(&sys->machine[0]);
        SdMachineFxStop(&sys->machine[1]);
    }
    GwSdBattle.winnerNo = sys->spr.winnerNo;
    if (sys->gameType == 0) {
        sdHealthLoss[0] = sys->machine[0].stat[4] * 4 - sys->machine[0].hp;
        sdHealthLoss[1] = sys->machine[1].stat[4] * 4 - sys->machine[1].hp;
        return;
    }
    if (sys->gameType == 1) {
        sdHealthLoss[0] = SdFlagDispTimerGet(0);
        sdHealthLoss[1] = SdFlagDispTimerGet(1);
        return;
    }
    sdHealthLoss[0] = SdRabbitDispTimerGet(0);
    sdHealthLoss[1] = SdRabbitDispTimerGet(1);
}

static void SdSeqPreWinner(s16 mode, s16 frameNo)
{
    s32 winner = sys->spr.winnerNo;
    SdMachine *w;
    SdMachine *l;
    SdCamera *cam;
    s32 i;
    s32 k;
    s32 charNo;
    f32 x;
    f32 y;
    f32 z;
    f32 y2;
    f32 f;
    Vec pos;
    Vec rot;
    SdMachine *m;

    if (sys->timeUpF != 0
        && (sys->machine[0].mode == 2 || sys->machine[0].mode == 3 || sys->machine[0].hitF != 0 || sys->machine[1].mode == 2
            || sys->machine[1].mode == 3 || sys->machine[1].hitF != 0)) {
        SdSeqUpdate();
        return;
    }
    if (winner == -1) {
        sys->stageClosedF = 1;
        MgSeqWinnerSet(-1, -1, -1, -1);
        MgSeqModeNext();
        return;
    }
    w = &sys->machine[winner];
    l = &sys->machine[winner ^ 1];
    cam = &sys->camera[winner];
    MgSeqModeChangeOff();
    switch (sys->seqStep) {
        case 0:
            WipeCreate(2, 0, 0x2D);
            WipeColorSet(0xFF, 0xFF, 0xFF);
            sys->seqTimerA = 0;
            sys->seqStepA = 0;
            sys->seqStep++;
            break;
        case 1:
            if (WipeCheckEnd() == 0) {
                sys->stageClosedF = 1;
                cam->offsetOn = 0;
                SdStageClose();
                SdSprHideAll();
                SdCharInit(w, l);
                Hu3DModelCameraSet(sys->stageMdlId, sdCameraBitTbl[winner]);
                if (sys->shadowMdlId != -1) {
                    Hu3DModelCameraSet(sys->shadowMdlId, sdCameraBitTbl[winner]);
                }
                if (sys->backMdlId[winner ^ 1] != -1) {
                    Hu3DModelAttrSet(sys->backMdlId[winner ^ 1], 1);
                }
                if (sys->stageNo == 7) {
                    SdParticleCameraSet(winner);
                }
                for (i = 0; i < 2; i++) {
                    m = (i == 0) ? w : l;
                    SdMachineFxStop(m);
                    Hu3DModelPosSetV(m->charMdlId, &w->curPos);
                    Hu3DModelAttrSet(m->mdl3AE, 1);
                    Hu3DModelAttrReset(m->charMdlId, 1);
                    Hu3DModelAttrSet(m->charMdlId, 0x40000002);
                    if (sys->stageNo != 7 && sys->stageNo != 2) {
                        Hu3DModelAttrSet(m->mdl3B4, 1);
                        Hu3DModelShadowSet(m->mdlId);
                        Hu3DModelShadowSet(m->gunMdlId);
                    }
                    Hu3DMotionTimeSet(m->mdlId, 0.0f);
                    Hu3DMotionSpeedSet(m->mdlId, 0.0f);
                    Hu3DMotionShapeTimeSet(m->mdlId, 0.0f);
                    Hu3DMotionShapeSpeedSet(m->mdlId, 0.0f);
                    Hu3DModelAttrSet(m->gunMdlId, 0x40000082);
                    for (k = 0; k < 4; k++) {
                        if (m->hookMdlId[k] != -1 && sys->stageNo != 7 && sys->stageNo != 2) {
                            Hu3DModelShadowSet(m->hookMdlId[k]);
                        }
                    }
                    MachineCameraBitSet(m, sdCameraBitTbl[winner]);
                    Hu3DModelCameraSet(m->charMdlId, sdCameraBitTbl[winner]);
                }
                Hu3DModelAttrSet(sys->kinokoMdlId, 1);
                Hu3DModelAttrSet(sys->kinokoMdlId2, 1);
                Hu3DModelAttrSet(sys->togezoMdlId, 1);
                Hu3DModelAttrSet(sys->togezoMdlId2, 1);
                MachineCameraBitSet(w, 3);
                MachineCameraBitSet(l, 3);
                z = 0.0f;
                y = 0.0f;
                x = 0.0f;
                if (sys->stageNo == 2 || sys->stageNo == 7) {
                    z = -1500.0f;
                } else if (sys->stageNo == 5) {
                    x = 150.0f;
                } else if (sys->stageNo == 6) {
                    y = 130.0f;
                }
                if (w->bodyType == 3 && w->kind == 5) {
                    y += 10.0f;
                }
                if ((w->attr & 2) != 0) {
                    pos.x = x - 170.0f;
                    pos.y = y + w->wheelY0;
                    pos.z = z + (60.0f + w->hitRadius);
                } else {
                    pos.x = x - 170.0f;
                    pos.y = y;
                    pos.z = z + (60.0f + w->hitRadius);
                }
                MachinePosSet(w, &pos);
                rot.x = 0.0f;
                rot.y = 200.0f;
                rot.z = 0.0f;
                MachineRotSet(w, &rot);
                Hu3DModelPosSet(w->charMdlId, x, y, z);
                Hu3DModelRotSet(w->charMdlId, 0.0f, 180.0f, 0.0f);
                cam->goalTarget.x = x;
                cam->goalTarget.y = 150.0f + y;
                cam->goalTarget.z = z;
                cam->target = cam->goalTarget;
                cam->goalPos.x = x;
                cam->goalPos.y = 250.0f + y;
                cam->goalPos.z = z - 400.0f;
                cam->pos = cam->goalPos;
                y2 = 0.0f;
                if (sys->stageNo == 6) {
                    y2 = 130.0f;
                }
                if (l->bodyType == 3 && l->kind == 5) {
                    y2 += 10.0f;
                }
                rot.x = 0.0f;
                rot.y = -60.0f;
                rot.z = 0.0f;
                MachineRotSet(l, &rot);
                if ((l->attr & 2) != 0) {
                    pos.x = 260.0f + x;
                    pos.y = y2 + l->wheelY0;
                    pos.z = 15.0f + (l->wheelY0 + ((300.0f + z) + l->hitRadius));
                } else {
                    pos.x = 260.0f + x;
                    pos.y = y2;
                    pos.z = 15.0f + (l->wheelY0 + ((300.0f + z) + l->hitRadius));
                }
                MachinePosSet(l, &pos);
                if (w->charNo != 0xB) {
                    Hu3DModelPosSet(l->charMdlId, 200.0f + x, y2, 250.0f + z);
                } else {
                    Hu3DModelPosSet(l->charMdlId, 250.0f + x, y2, 200.0f + z);
                }
                Hu3DModelRotSet(l->charMdlId, 0.0f, 200.0f, 0.0f);
                if (sys->stageNo != 7 && sys->stageNo != 2) {
                    Hu3DShadowMultiSizeSet(0xF0, 3);
                } else {
                    Hu3DModelPosSetV(w->mdl3B4, &w->curPos);
                    Hu3DModelPosSetV(l->mdl3B4, &l->curPos);
                }
                if (sys->stageNo != 3) {
                    Hu3DShadowBuf[winner].camTarget.x = x;
                    Hu3DShadowBuf[winner].camTarget.y = y2;
                    Hu3DShadowBuf[winner].camTarget.z = 0.0f;
                    Hu3DShadowBuf[winner].camPos.x = x;
                    Hu3DShadowBuf[winner].camPos.y = 1250.0f + y2;
                    Hu3DShadowBuf[winner].camPos.z = 0.0f;
                } else {
                    Hu3DShadowBuf[winner].camTarget.x = x;
                    Hu3DShadowBuf[winner].camTarget.y = y2 - 1000.0f;
                    Hu3DShadowBuf[winner].camTarget.z = 1000.0f;
                    Hu3DShadowBuf[winner].camPos.x = x;
                    Hu3DShadowBuf[winner].camPos.y = 1000.0f + y2;
                    Hu3DShadowBuf[winner].camPos.z = -1000.0f;
                }
                Hu3DShadowMultiTPLvlSet(0.35f, 3);
                Hu3DShadowBuf[winner].camTarget.z += z;
                Hu3DShadowBuf[winner].camPos.z += z;
                WipeCreate(1, 0, 0x2D);
                sys->seqStep++;
            }
            break;
        case 2:
            if (WipeCheckEnd() == 0) {
                sys->seqTimerA = 0;
                sys->seqTimer = 0;
                Hu3DModelAttrReset(w->charMdlId, 0x40000002);
                Hu3DModelAttrReset(l->charMdlId, 0x40000002);
                charNo = sys->machine[winner].charNo;
                if (charNo < 10) {
                    charNo = charNo;
                } else if (charNo == 10) {
                    charNo = 13;
                } else {
                    charNo = 14;
                }
                MgSeqWinnerSet(charNo, -1, -1, -1);
                WipeColorSet(0, 0, 0);
                MgSeqModeDelaySet(8, 0x104);
                MgSeqModeNext();
            }
            break;
    }
    if (sys->seqStepA == 0 && winner != -1) {
        sys->seqTimerA++;
        f = 320.0f * MathLerp(0, (f32)sys->seqTimerA / 45.0f);
        if (winner == 0) {
            SdCameraSplitSet(320.0f + f);
        } else {
            SdCameraSplitSet(320.0f - f);
        }
        if ((f32)sys->seqTimerA > 45.0f) {
            SdCameraSplitSet(640.0f - 640.0f * (f32)winner);
            sys->seqTimerA = 0;
            sys->seqStepA++;
        }
    }
    SdSeqUpdate();
}

static void SdSeqWinner(s16 mode, s16 frameNo)
{
    if (sys->spr.winnerNo != -1 && sys->seqTimerA == winnerSoundDelayTbl[sys->machine[sys->spr.winnerNo].charNo]) {
        SdPlayerSoundPlay(&sys->machine[sys->spr.winnerNo], 5);
    }
    sys->seqTimerA++;
    SdSeqUpdate();
}

static void SdSeqFadeOut(s16 mode, s16 frameNo)
{
    if (sys->fxId != -1) {
        HuAudFXStop(sys->fxId);
        sys->fxId = -1;
    }
    SdSeqUpdate();
}

static void SdSeqClose(s16 mode, s16 frameNo)
{
    HuAudAUXSet(0, 1);
    SdFlagClose();
    SdRabbitClose();
    GameMesClose();
    HuAudAllStop();
    Hu3DModelAllKill();
    Hu3DLightAllKill();
}

static void SdSeqUpdate(void)
{
    s32 i;
    SdMachine *machine;
    Vec zero;

    SdStageUpdate();
    for (i = 0; i < 2; i++) {
        if (MgSeqModeGet() >= 5 && MgSeqModeGet() <= 7 && sys->stageClosedF == 0) {
            SdSeqMachineUpdate(&sys->machine[i]);
        }
        if (MgSeqModeGet() >= 5 && MgSeqModeGet() < 7) {
            SdBulletUpdate(&sys->machine[i]);
        }
    }
    if (MgSeqModeGet() <= 7 && sys->stageClosedF == 0) {
        MachineColUpdate();
    }
    for (i = 0; i < 2; i++) {
        machine = &sys->machine[i];
        if (MgSeqModeGet() <= 5) {
            MachineColSeparate(machine);
        }
        if (MgSeqModeGet() <= 7 && sys->stageClosedF == 0) {
            SdStageMachineCol(machine);
            Hu3DModelPosSetV(machine->mdlId, &machine->curPos);
            Hu3DModelRotSetV(machine->mdlId, &machine->rot);
            Hu3DModelPosSetV(machine->mdl3B4, &machine->curPos);
            MachineCameraInit(machine, 1);
            if (MgSeqModeGet() < 5) {
                PSMTXCopy(machine->mtx, machine->storedMtx);
                machine->basePos = machine->curPos;
                machine->storedRot = machine->rot;
                machine->unk15C = machine->speed;
                zero.x = 0.0f;
                zero.y = 0.0f;
                zero.z = 0.0f;
                machine->vel = zero;
                machine->force = machine->vel;
            }
            PSVECSubtract(&machine->curPos, &machine->basePos, &machine->rotVel);
        }
        if (MgSeqModeGet() > 1 && MgSeqModeGet() < 7) {
            MachineParticleUpdate(&sys->machine[i]);
        }
    }
    if (MgSeqModeGet() == 5) {
        PlayDamageSound();
    }
    SdVibrateUpdate();
    SdSprUpdate();
    SdFlagUpdate();
    SdRabbitUpdate();
    SdImpactUpdate();
}

static void SdSeqMachineSet(SdMachine *machine, s32 no, s32 kind, s32 bodyType, s32 engineType, s32 bulletType, s32 charNo)
{
    memset(machine, 0, sizeof(SdMachine));
    machine->mode = 1;
    machine->playerNo = no;
    machine->cameraNo = no;
    machine->padNo = sys->padNo[no];
    machine->kind = kind;
    machine->bodyType = bodyType;
    machine->engineType = engineType;
    machine->bulletType = bulletType;
    machine->charNo = charNo;
    SdMachineCreate(machine);
    PSMTXIdentity(machine->mtx);
    PSMTXCopy(machine->mtx, machine->storedMtx);
}

static s32 clampLv(s32 v, s32 max)
{
    if (v > max) {
        return max;
    }
    if (v < 1) {
        return 1;
    }
    return v;
}

static s32 SdSeqMachineUpdate(SdMachine *machine)
{
    s32 ret = 0;
    s32 v;
    s32 i;
    HU3DMODELID mdl1;
    HU3DMODELID mdl2;
    Vec dir;
    Vec rot;
    Vec scale;

    PSMTXCopy(machine->mtx, machine->storedMtx);
    machine->basePos = machine->curPos;
    machine->storedRot = machine->rot;
    machine->unk15C = machine->speed;
    machine->speedLv = machine->stat[1];
    machine->accelLv = machine->stat[2];
    machine->turnLv = machine->stat[0];
    if ((machine->attr & 0x10) == 0) {
        if (machine->boostType == 1) {
            machine->accelLv += 1;
            machine->speedLv += 1;
        } else if (machine->boostType == 2) {
            machine->accelLv += 2;
            machine->speedLv += 2;
        }
    }
    if ((machine->attr & 0x50) != 0x50) {
        if (machine->boostType == 3) {
            machine->turnLv -= 1;
            machine->speedLv -= 1;
        } else if (machine->boostType == 4) {
            machine->turnLv -= 2;
            machine->speedLv -= 2;
        }
    }
    if ((machine->attr & 0x40) == 0 && machine->boostType == 5 && machine->mode == 1 && machine->unk210 == 0) {
        dir.x = machine->mtx[0][2];
        dir.y = machine->mtx[1][2];
        dir.z = machine->mtx[2][2];
        PSVECScale(&dir, &dir, -1.0f);
        if (machine->bodyType == 0xA) {
            SdMachineDamage(machine, &dir, 2);
        } else {
            SdMachineDamage(machine, &dir, 4);
        }
    }
    machine->speedLv = clampLv(machine->speedLv, 8);
    machine->accelLv = clampLv(machine->accelLv, 8);
    machine->turnLv = clampLv(machine->turnLv, 8);
    if (GwPlayerConf[machine->playerNo].type != 0 && machine->comPreset == 0) {
        if (sys->timeSec >= 0x2D) {
            v = 9;
        } else {
            v = sys->timeSec / 5;
        }
        machine->turnLv = 10 - v;
        machine->accelLv = v;
        machine->speedLv = v;
        machine->turnLv = clampLv(machine->turnLv, 8);
        machine->speedLv = clampLv(machine->speedLv, 9);
        machine->accelLv = clampLv(machine->accelLv, 9);
    }
    if (machine->effType != 0) {
        if (machine->effType == 1) {
            machine->speedLv += 2;
            machine->turnLv -= 1;
            machine->accelLv += 2;
            machine->speedLv = clampLv(machine->speedLv, 10);
            machine->accelLv = clampLv(machine->accelLv, 10);
            machine->turnLv = clampLv(machine->turnLv, 10);
        } else {
            machine->speedLv -= 2;
            machine->turnLv += 1;
            machine->accelLv -= 2;
            machine->speedLv = clampLv(machine->speedLv, 8);
            machine->accelLv = clampLv(machine->accelLv, 8);
            machine->turnLv = clampLv(machine->turnLv, 8);
        }
        machine->effTimer++;
        if (machine->effType == 1) {
            mdl1 = sys->kinokoMdlId;
        } else {
            mdl1 = sys->togezoMdlId;
        }
        if (machine->effType == 1) {
            mdl2 = sys->kinokoMdlId2;
        } else {
            mdl2 = sys->togezoMdlId2;
        }
        if ((f32)machine->effTimer < 540.0f) {
            Hu3DModelAttrReset(mdl1, 1);
            Hu3DModelAttrSet(mdl2, 1);
        } else {
            Hu3DModelAttrSet(mdl1, 1);
            Hu3DModelAttrReset(mdl2, 1);
        }
        if ((f32)machine->effTimer == 60.0f) {
            Hu3DModelAttrSet(mdl1, 0x40000001);
        } else if ((f32)machine->effTimer == 540.0f) {
            Hu3DModelAttrReset(mdl1, 0x40000001);
            Hu3DMotionTimeSet(mdl2, 0.0f);
        } else if ((f32)machine->effTimer == 600.0f) {
            Hu3DModelAttrSet(sys->kinokoMdlId, 1);
            Hu3DModelAttrSet(sys->kinokoMdlId2, 1);
            Hu3DModelAttrSet(sys->togezoMdlId, 1);
            Hu3DModelAttrSet(sys->togezoMdlId2, 1);
            machine->effType = 0;
            machine->effTimer = 0;
        }
        if (machine->camSingleF == 1) {
            Hu3DModelCameraSet(mdl1, sdCameraBitTbl[machine->playerNo]);
            Hu3DModelCameraSet(mdl2, sdCameraBitTbl[machine->playerNo]);
        } else {
            Hu3DModelCameraSet(mdl1, 3);
            Hu3DModelCameraSet(mdl2, 3);
        }
        MathMtxRotGet(machine->mtx, &rot);
        Hu3DModelPosSetV(mdl1, &machine->curPos);
        Hu3DModelRotSetV(mdl1, &rot);
        Hu3DModelPosSetV(mdl2, &machine->curPos);
        Hu3DModelRotSetV(mdl2, &rot);
        MachineEffectUpdate(machine);
    }
    if (machine->bodyType == 7) {
        if ((sys->timeSec % 10) < 5) {
            MachineCameraBitSet(machine, sdCameraBitTbl[machine->playerNo]);
            machine->camSingleF = 1;
        } else {
            MachineCameraBitSet(machine, 3);
            machine->camSingleF = 0;
        }
    }
    switch (machine->mode) {
        case 1:
            MachineModeIdle(machine);
            break;
        case 2:
            MachineModeDamage(machine);
            break;
        case 3:
            MachineModeLaunch(machine);
            break;
        case 4:
            MachineModeDie(machine);
            break;
        case 5:
            MachineModeDie(machine);
            break;
    }
    if (machine->unk210 == 1) {
        machine->unk214++;
        if ((machine->unk214 % 2) != 0) {
            MachineDispSet(machine, 1);
        } else {
            MachineDispSet(machine, 0);
        }
        if (machine->unk214 >= machine->unk218) {
            MachineDispSet(machine, 1);
            machine->unk214 = 0;
            machine->unk210 = 0;
        }
    }
    MachineMotionUpdate(machine);
    SdMachineFxUpdate(machine);
    if (machine->hp == 1) {
        HuAudFXVolSet(machine->hitSeId, 0x7F);
    } else {
        HuAudFXVolSet(machine->hitSeId, 0);
    }
    if (MgSeqModeGet() > 5 && sys->stageClosedF == 0) {
        MachineDispSet(machine, 1);
        scale.x = 1.0f;
        scale.y = 1.0f;
        scale.z = 1.0f;
        MachineScaleSet(machine, &scale);
        for (i = 0; i < 4; i++) {
            if (machine->hookMdlId[i] != -1) {
                Hu3DMotionSpeedSet(machine->hookMdlId[i], 0.0f);
                Hu3DMotionShapeSpeedSet(machine->hookMdlId[i], 0.0f);
            }
        }
    }
    return ret;
}

static void MachineParticleUpdate(SdMachine *machine)
{
    if (MgSeqModeGet() <= 5) {
        if (machine->bodyType != 1 && machine->bodyType != 9 && machine->bodyType != 0xA) {
            if ((sys->seqTimer % 2) != 0) {
                MachineExhaustAdd(machine);
            }
        } else {
            MachineExhaustAdd(machine);
        }
        if ((sys->seqTimer % 2) != 0) {
            MachineTrackEffAdd(machine);
        }
    }
    MachineExhaustUpdate(machine);
    MachineTrackEffUpdate(machine);
}

static void MachineExhaustUpdate(SdMachine *machine)
{
    HU3DPARTICLE *dustPar = (HU3DPARTICLE *)Hu3DData[sys->dustMdlId].hookData;
    HU3DPARTICLE *kind1Par = (HU3DPARTICLE *)Hu3DData[sys->partMdlId[10]].hookData;
    HU3DPARTICLE *par;
    HU3DPARTICLEDATA *base;
    s32 kind1;
    s32 maxLife;
    s32 i;
    s32 j;

    if (machine->bodyType != 1 && machine->bodyType != 9 && machine->bodyType != 0xA) {
        kind1 = 0;
        par = dustPar;
        maxLife = 60;
    } else {
        kind1 = 1;
        par = kind1Par;
        maxLife = 30;
    }
    for (i = 0; i < 30; i++) {
        if (par->data[machine->corner[0].effIdx + i].time == 0) {
            continue;
        }
        for (j = 0; j < machine->cornerNum; j++) {
            base = &par->data[machine->corner[j].effIdx];
            base[i].pos.x += base[i].vel.x;
            base[i].pos.y += base[i].vel.y + base[i].accel.y;
            base[i].pos.z += base[i].vel.z;
            PSVECScale((Vec *)&base[i].vel, (Vec *)&base[i].vel, 0.9f);
            if (machine->bodyType == 3) {
                base[i].accel.y *= 1.02f;
                base[i].scale *= 1.04f;
                if (base[i].scale > 180.0f) {
                    base[i].scale = 180.0f;
                }
                base[i].color.a = (u8)((f32)base[i].color.a * 0.9f);
            } else {
                base[i].accel.y *= 1.01f;
                if (machine->bodyType == 1 && j >= 2) {
                    base[i].scale *= 1.02f;
                } else {
                    base[i].scale *= 1.01f;
                }
                if (base[i].scale > 128.0f) {
                    base[i].scale = 128.0f;
                }
                if (kind1 == 0) {
                    base[i].color.a = (u8)((f32)base[i].color.a * 0.95f);
                } else {
                    if (base[i].parManId <= 8) {
                        base[i].color.r = 0xFF - ((s32)((0xFF - particleColorTbl[machine->bodyType].r) * base[i].parManId) / 8);
                        base[i].color.g = 0xFF - ((s32)((0xFF - particleColorTbl[machine->bodyType].g) * base[i].parManId) / 8);
                        base[i].color.b = 0xFF - ((s32)((0xFF - particleColorTbl[machine->bodyType].b) * base[i].parManId) / 8);
                    }
                    base[i].color.a = (u8)((f32)base[i].color.a * 0.9f);
                }
            }
            base[i].parManId++;
            if (base[i].parManId >= maxLife) {
                base[i].parManId = 0;
                base[i].time = 0;
                base[i].scale = 0.0f;
            }
        }
    }
}

static void MachineExhaustAdd(SdMachine *machine)
{
    HU3DPARTICLE *dustPar = (HU3DPARTICLE *)Hu3DData[sys->dustMdlId].hookData;
    HU3DPARTICLE *kind1Par = (HU3DPARTICLE *)Hu3DData[sys->partMdlId[10]].hookData;
    HU3DPARTICLE *par;
    HU3DPARTICLEDATA *base;
    HU3DPARTICLEDATA *p;
    Vec posTbl[4];
    Vec dirTbl[4];
    Vec tmp;
    GXColor col;
    s32 kind1;
    s32 i;
    s32 j;
    s32 a;

    if (machine->bodyType != 1 && machine->bodyType != 9 && machine->bodyType != 0xA) {
        kind1 = 0;
        par = dustPar;
    } else {
        kind1 = 1;
        par = kind1Par;
    }
    for (i = 0; i < machine->cornerNum; i++) {
        if (machine->bodyType == 3) {
            tmp.x = 0.0f;
            tmp.y = 0.0f;
            tmp.z = 0.0f;
            tmp.x = machine->mtx[0][1];
            tmp.y = machine->mtx[1][1];
            tmp.z = machine->mtx[2][1];
            dirTbl[i] = tmp;
        } else {
            tmp.x = 0.0f;
            tmp.y = 0.0f;
            tmp.z = 0.0f;
            tmp.x = machine->mtx[0][2];
            tmp.y = machine->mtx[1][2];
            tmp.z = machine->mtx[2][2];
            dirTbl[i] = tmp;
            PSVECScale(&dirTbl[i], &dirTbl[i], -1.0f);
        }
        posTbl[i] = machine->corner[i].pos;
        PSMTXMultVec(machine->mtx, &posTbl[i], &posTbl[i]);
        PSVECAdd(&machine->curPos, &posTbl[i], &posTbl[i]);
    }
    for (i = 0; i < 30; i++) {
        if (kind1 == 0) {
            col = particleColorTbl[machine->bodyType];
        } else {
            a = (s32)(0.2f * machine->speed);
            if (a > 0xFF) {
                a = 0xFF;
            } else if (a < 0) {
                a = 0;
            }
            col.r = 0xFF;
            col.g = 0xFF;
            col.b = 0xFF;
            col.a = 0;
            col.a = (u8)a;
        }
        if (par->data[machine->corner[0].effIdx + i].time != 0) {
            continue;
        }
        for (j = 0; j < machine->cornerNum; j++) {
            base = &par->data[machine->corner[j].effIdx];
            p = &base[i];
            if (machine->camSingleF == 1) {
                p->cameraBit = sdCameraBitTbl[machine->playerNo];
            } else {
                p->cameraBit = 3;
            }
            p->time = 1;
            p->parManId = 0;
            p->vel = *(HuVecF *)&dirTbl[j];
            if (machine->bodyType == 3) {
                tmp.x = 0.0f;
                tmp.y = 0.0f;
                tmp.z = 0.0f;
                tmp.x = (posTbl[j].x - 5.0f) + (f32)(rand8() % 10);
                tmp.y = (posTbl[j].y - 5.0f) + (f32)(rand8() % 10);
                tmp.z = (posTbl[j].z - 5.0f) + (f32)(rand8() % 10);
                p->pos = *(HuVecF *)&tmp;
                PSVECScale((Vec *)&p->vel, (Vec *)&p->vel, 10.0f);
                p->scale = 30.0f + (f32)(rand8() % 40);
                tmp.x = 0.0f;
                tmp.y = 5.0f;
                tmp.z = 0.0f;
                p->accel = *(HuVecF *)&tmp;
            } else if (kind1 == 0) {
                tmp.x = 0.0f;
                tmp.y = 0.0f;
                tmp.z = 0.0f;
                tmp.x = (posTbl[j].x - 2.0f) + (f32)(rand8() % 5);
                tmp.y = (posTbl[j].y - 2.0f) + (f32)(rand8() % 5);
                tmp.z = (posTbl[j].z - 2.0f) + (f32)(rand8() % 5);
                p->pos = *(HuVecF *)&tmp;
                p->scale = 30.0f + (f32)(rand8() % 10);
                tmp.x = 0.0f;
                tmp.y = 3.0f;
                tmp.z = 0.0f;
                p->accel = *(HuVecF *)&tmp;
                PSVECScale((Vec *)&p->vel, (Vec *)&p->vel, 5.0f);
            } else {
                tmp = posTbl[j];
                p->pos = *(HuVecF *)&tmp;
                if (machine->bodyType == 1 && j >= 2) {
                    p->scale = 60.0f;
                } else {
                    p->scale = 30.0f;
                }
                tmp.x = 0.0f;
                tmp.y = 0.0f;
                tmp.z = 0.0f;
                p->accel = *(HuVecF *)&tmp;
                PSVECScale((Vec *)&p->vel, (Vec *)&p->vel, 0.0f);
            }
            p->color = col;
        }
        return;
    }
}

static void MachineTrackEffUpdate(SdMachine *machine)
{
    SdMachineTrack *track = machine->track;
    HU3DPARTICLEDATA *p;
    s32 i;
    s32 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 30; j++) {
            p = track->eff[i][j];
            if (p == NULL || p->time == 0) {
                continue;
            }
            p->pos.x += p->vel.x;
            p->pos.y += p->vel.y + p->accel.y;
            p->pos.z += p->vel.z;
            PSVECScale((Vec *)&p->vel, (Vec *)&p->vel, 0.9f);
            switch (p->parManId) {
                case 2:
                    p->color.r = (u8)(s32)((f32)p->color.r * 0.995f);
                    p->color.g = (u8)(s32)((f32)p->color.g * 0.98f);
                    p->color.b = (u8)(s32)((f32)p->color.b * 0.9f);
                    p->color.a = (u8)(s32)((f32)p->color.a * 0.92f);
                    p->scale *= 1.01f;
                    p->accel.y *= 1.01f;
                    if (p->scale > 64.0f) {
                        p->scale = 64.0f;
                    }
                    break;
                case 4:
                    if (p->time <= 0x1E) {
                        p->scale *= 1.005f;
                    } else {
                        p->scale *= 0.95f;
                    }
                    p->accel.y *= 1.06f;
                    if (p->scale > 128.0f) {
                        p->scale = 128.0f;
                    }
                    break;
                case 6:
                    p->color.a = (u8)(s32)((f32)p->color.a * 0.9f);
                    if (p->time <= 0x1E) {
                        p->scale *= 1.005f;
                    } else {
                        p->scale *= 0.95f;
                        if (p->pos.y < 0.0f) {
                            p->scale = 0.0f;
                            p->color.a = 0;
                        }
                    }
                    p->accel.y *= 1.06f;
                    if (p->scale > 128.0f) {
                        p->scale = 128.0f;
                    }
                    break;
                default:
                    p->color.a = (u8)(s32)((f32)p->color.a * 0.95f);
                    if (p->parManId == 1) {
                        p->scale *= 1.02f;
                        p->accel.y *= 1.03f;
                    } else {
                        p->scale *= 1.04f;
                        p->accel.y *= 1.01f;
                    }
                    if (p->scale > 128.0f) {
                        p->scale = 128.0f;
                    }
                    break;
            }
            p->time++;
            if (p->time >= 0x3C) {
                p->parManId = 0;
                p->time = 0;
                p->scale = 0.0f;
                track->eff[i][j] = NULL;
            }
        }
    }
}

static void MachineTrackEffAdd(SdMachine *machine)
{
    SdMachineTrack *track = machine->track;
    HU3DPARTICLE *dustPar = (HU3DPARTICLE *)Hu3DData[sys->dustMdlId].hookData;
    HU3DPARTICLE *trackPar = (HU3DPARTICLE *)Hu3DData[sys->partMdlId[2]].hookData;
    HU3DPARTICLE *trackPar2 = (HU3DPARTICLE *)Hu3DData[sys->partMdlId[9]].hookData;
    HU3DPARTICLE *trackPar3 = (HU3DPARTICLE *)Hu3DData[sys->partMdlId[11]].hookData;
    HU3DPARTICLEDATA *base;
    HU3DPARTICLEDATA *p;
    Vec dirTbl[2];
    Vec posTbl[2];
    Vec tmp;
    GXColor col;
    f32 spd;
    f32 accelY;
    f32 zRot;
    s32 i;
    s32 k;
    s32 s;
    s32 attr;

    if ((machine->attr & 2) != 0) {
        return;
    }
    spd = PSVECMag(&machine->rotVel);
    if (spd < 1.0f) {
        return;
    }
    for (i = 0; i < 2; i++) {
        tmp.x = 0.0f;
        tmp.y = 0.0f;
        tmp.z = 0.0f;
        tmp.x = machine->mtx[0][2];
        tmp.y = machine->mtx[1][2];
        tmp.z = machine->mtx[2][2];
        dirTbl[i] = tmp;
        PSVECScale(&dirTbl[i], &dirTbl[i], -1.0f);
        if (machine->wheelAttr[i + 2] == 1 || machine->wheelAttr[i + 2] == 4) {
            dirTbl[i].y += 0.25f * spd;
        } else if (machine->wheelAttr[i + 2] == 6) {
            dirTbl[i].y += 0.25f * spd;
        }
        posTbl[i] = machine->wheelPos[i + 2];
        posTbl[i].y -= machine->wheelY1;
    }
    for (i = 0; i < 2; i++) {
        if (machine->wheelOn[i + 2] == 0 || machine->wheelAttr[i + 2] == 5) {
            continue;
        }
        attr = machine->wheelAttr[i + 2];
        base = NULL;
        accelY = -1.0f;
        zRot = 0.0f;
        col.r = col.g = col.b = col.a = 0xFF;
        if (attr == 0 || attr == 3) {
            base = &dustPar->data[track->effIdx[i][0]];
            if (attr == 0) {
                col.r = 0xDA;
                col.g = 0xCB;
                col.b = 0x9E;
                col.a = 0x64;
            } else {
                col.r = 0xDA;
                col.g = 0xCB;
                col.b = 0x80;
                col.a = 0x64;
            }
            zRot = 0.0f;
            accelY = 1.0f;
        } else if (attr == 4) {
            u8 c;
            base = &trackPar->data[track->effIdx[i][1]];
            c = (u8)(rand8() % 0x40 + 0x80);
            col.r = c;
            col.g = c;
            col.b = c;
            col.a = 0xFF;
            zRot = 0.017453292f * (4.0f * (f32)(rand8() % 90));
            accelY = -1.0f;
        } else if (attr == 6) {
            base = &trackPar3->data[track->effIdx[i][3]];
            zRot = 0.017453292f * (4.0f * (f32)(rand8() % 90));
            col.r = 0xFF;
            col.g = 0xFF;
            col.b = 0xFF;
            col.a = 0xFF;
            accelY = -1.0f;
        } else if (attr == 1 || attr == 2) {
            base = &trackPar2->data[track->effIdx[i][2]];
            zRot = 0.017453292f * (4.0f * (f32)(rand8() % 90));
            if (attr == 1) {
                col.r = (u8)(rand8() % 256);
                col.g = (u8)(rand8() % 128 + 0x80);
                col.b = 0xFF;
                col.a = 0xFF;
            } else {
                col.r = 0xFF;
                col.g = (u8)(rand8() % 192 + 0x40);
                col.b = (u8)(rand8() % 64);
                col.a = 0xFF;
                if (col.g < col.b) {
                    col.b = col.g;
                }
            }
            accelY = -1.0f;
        }
        if (base == NULL) {
            continue;
        }
        for (k = 0; k < 30; k++) {
            if (base[k].time != 0) {
                continue;
            }
            for (s = 0; s < 30; s++) {
                if (track->eff[i][s] == NULL) {
                    track->eff[i][s] = &base[k];
                    break;
                }
            }
            if (s == 30) {
                continue;
            }
            p = &base[k];
            if (machine->camSingleF == 1) {
                p->cameraBit = sdCameraBitTbl[machine->playerNo];
            } else {
                p->cameraBit = 3;
            }
            p->time = 1;
            p->parManId = machine->wheelAttr[i + 2];
            p->vel = *(HuVecF *)&dirTbl[i];
            tmp.x = 0.0f;
            tmp.y = accelY;
            tmp.z = 0.0f;
            p->accel = *(HuVecF *)&tmp;
            p->color = col;
            p->zRot = zRot;
            if (p->parManId == 6) {
                PSVECScale((Vec *)&p->vel, (Vec *)&p->vel, 0.5f);
            } else {
                PSVECScale((Vec *)&p->vel, (Vec *)&p->vel, 2.5f);
            }
            tmp.x = (posTbl[i].x - 20.0f) + (f32)(rand8() % 40);
            tmp.y = posTbl[i].y + (f32)(rand8() % 20);
            tmp.z = (posTbl[i].z - 20.0f) + (f32)(rand8() % 40);
            p->pos = *(HuVecF *)&tmp;
            p->scale = (4.0f * spd) + (f32)(rand8() % 20);
            break;
        }
    }
}

#include "REL/sd00dll/sd00_b.inc"

#include "REL/sd00dll/sd00_c.inc"
