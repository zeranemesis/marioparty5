#include "msm/msmse.h"
#include "msm/msmfio.h"
#include "msm/msmmem.h"

#include "string.h"

#define SE_PLAYER_EMIT (1 << 0)

/* SE_PLAYER status, returned as is by msmSeGetStatus */
#define SE_STAT_DONE 0
#define SE_STAT_STOP 1
#define SE_STAT_PLAY 2
#define SE_STAT_PAUSE 3

#define SE_VOLUME(player) ((player)->vol * (player)->baseVol * (player)->fadeVol * (player)->pauseVol / (127 * 127 * 127))

typedef struct SePlayer_s {
    /* 0x00 */ SND_VOICEID vid;
    /* 0x04 */ s32 no;
    /* 0x08 */ s16 seId;
    /* 0x0A */ s8 status;
    /* 0x0B */ s8 busyF;
    /* 0x0C */ s8 vol;
    /* 0x0D */ s8 pan;
    /* 0x0E */ s16 pitch;
    /* 0x10 */ s8 span;
    /* 0x11 */ s8 auxAVol;
    /* 0x12 */ s8 auxBVol;
    /* 0x13 */ u8 flag;
    /* 0x14 */ SND_PARAMETER_INFO paramInfo;
    /* 0x1C */ SND_PARAMETER param[5];
    /* 0x30 */ s8 baseVol;
    /* 0x31 */ s8 basePan;
    /* 0x32 */ s16 basePitch;
    /* 0x34 */ volatile s32 fadeMaxTime;
    /* 0x38 */ s32 fadeTime;
    /* 0x3C */ s8 fadeVol;
    /* 0x40 */ volatile s32 pauseMaxTime;
    /* 0x44 */ s32 pauseTime;
    /* 0x48 */ s8 pauseVol;
    /* 0x4C */ SND_EMITTER emitter;
    /* 0x9C */ SND_FVECTOR emiPos;
    /* 0xA8 */ SND_FVECTOR emiDir;
    /* 0xB4 */ s8 emitterF;
} SE_PLAYER; // Size 0xB8

typedef struct MsmSe_s {
    /* 0x00 */ s32 seMax;
    /* 0x04 */ s8 sfx;
    /* 0x05 */ s8 baseGrpNumPlay;
    /* 0x06 */ s8 numPlay;
    /* 0x08 */ s32 no;
    /* 0x0C */ MSMSE *seData;
    /* 0x10 */ SE_PLAYER *player;
    /* 0x14 */ SND_LISTENER listener;
    /* 0x9C */ SND_FVECTOR listenerPos;
    /* 0xA8 */ SND_FVECTOR listenerDir;
    /* 0xB4 */ SND_FVECTOR listenerHeading;
    /* 0xC0 */ SND_FVECTOR listenerUp;
    /* 0xCC */ float sndDist;
    /* 0xD0 */ u16 groupId;
    /* 0xD2 */ u16 listenerF;
} MSM_SE_WORK; // Size 0xDC

static MSM_SE_WORK se;

static void msmSeFade(SE_PLAYER *player)
{
    u8 vol;

    if (player->status == SE_STAT_PAUSE) {
        return;
    }
    if (player->fadeMaxTime > 0) {
        if (--player->fadeTime == 0) {
            player->fadeMaxTime = 0;
            player->fadeMaxTime = 0;
            if (player->fadeMaxTime != 0) {
                player->fadeTime = player->fadeMaxTime;
                player->fadeVol = 127;
            } else {
                player->status = SE_STAT_STOP;
                if ((player->flag & SE_PLAYER_EMIT) && player->emitterF == TRUE) {
                    sndRemoveEmitter(&player->emitter);
                } else {
                    sndFXKeyOff(player->vid);
                }
            }
        } else {
            player->fadeVol = player->fadeTime * 127 / player->fadeMaxTime;
            vol = SE_VOLUME(player);
            if ((player->flag & SE_PLAYER_EMIT) && player->emitterF == TRUE) {
                player->emiDir.x = player->emiDir.y = player->emiDir.z = 0.0f;
                sndUpdateEmitter(&player->emitter, &player->emiPos, &player->emiDir, vol, NULL);
            } else {
                sndFXVolume(player->vid, vol);
            }
        }
    } else if (player->fadeMaxTime < 0) {
        if (++player->fadeTime >= -player->fadeMaxTime) {
            player->fadeMaxTime = 0;
            player->fadeVol = 127;
        } else {
            player->fadeVol = player->fadeTime * 127 / -player->fadeMaxTime;
        }
        vol = SE_VOLUME(player);
        if ((player->flag & SE_PLAYER_EMIT) && player->emitterF == TRUE) {
            player->emiDir.x = player->emiDir.y = player->emiDir.z = 0.0f;
            sndUpdateEmitter(&player->emitter, &player->emiPos, &player->emiDir, vol, NULL);
        } else {
            sndFXVolume(player->vid, vol);
        }
    }
}

static void msmSePauseSub(SE_PLAYER *player, BOOL pause, s32 speed)
{
    s32 time;

    time = speed / 15;
    if (pause) {
        if (player->status == SE_STAT_PLAY) {
            if (time != 0) {
                player->pauseMaxTime = player->pauseTime = time;
                player->pauseVol = 127;
            } else {
                player->pauseVol = 0;
                sndFXVolume(player->vid, SE_VOLUME(player));
            }
            player->status = SE_STAT_PAUSE;
        }
    } else {
        if (player->status == SE_STAT_PAUSE) {
            if (time != 0) {
                if (player->pauseMaxTime != 0) {
                    player->pauseMaxTime = -time;
                } else {
                    player->pauseMaxTime = -time;
                    player->pauseTime = 0;
                    player->pauseVol = 0;
                }
            } else {
                player->pauseVol = 127;
                sndFXVolume(player->vid, SE_VOLUME(player));
            }
            player->status = SE_STAT_PLAY;
        }
    }
}

static SE_PLAYER *msmSeSearchEntry(s32 seNo)
{
    SE_PLAYER *player;
    s32 i;

    for (i = 0; i < se.sfx; i++) {
        player = &se.player[i];
        if (player->status != SE_STAT_DONE && player->no == seNo) {
            return player;
        }
    }
    return NULL;
}

void msmSePeriodicProc(void)
{
    s32 i;
    SE_PLAYER *player;
    u8 vol;

    se.baseGrpNumPlay = se.numPlay = 0;
    for (i = 0; i < se.sfx; i++) {
        player = &se.player[i];
        if (player->busyF != FALSE) {
            continue;
        }
        switch (player->status) {
            case SE_STAT_STOP:
            case SE_STAT_PLAY:
                if ((player->flag & SE_PLAYER_EMIT) && player->emitterF == TRUE) {
                    if (!sndCheckEmitter(&player->emitter)) {
                        player->status = SE_STAT_DONE;
                    }
                } else {
                    if (sndFXCheck(player->vid) == SND_ID_ERROR) {
                        player->status = SE_STAT_DONE;
                    }
                }
                break;
        }
        if (player->status == SE_STAT_DONE) {
            continue;
        }
        if (msmSysCheckBaseGroup(se.seData[player->seId].groupId) != FALSE) {
            se.baseGrpNumPlay++;
        } else {
            se.numPlay++;
        }
        if (player->pauseMaxTime > 0) {
            if (--player->pauseTime == 0) {
                player->pauseMaxTime = 0;
                player->pauseVol = 0;
            } else {
                player->pauseVol = player->pauseTime * 127 / player->pauseMaxTime;
            }
        } else if (player->pauseMaxTime < 0) {
            if (++player->pauseTime >= -player->pauseMaxTime) {
                player->pauseMaxTime = 0;
                player->pauseVol = 127;
            } else {
                player->pauseVol = player->pauseTime * 127 / -player->pauseMaxTime;
            }
        }
        vol = SE_VOLUME(player);
        if ((player->flag & SE_PLAYER_EMIT) && player->emitterF == TRUE) {
            player->emiDir.x = player->emiDir.y = player->emiDir.z = 0.0f;
            sndUpdateEmitter(&player->emitter, &player->emiPos, &player->emiDir, vol, NULL);
        } else {
            sndFXVolume(player->vid, vol);
        }
        msmSeFade(player);
    }
}

MSMSE *msmSeGetIndexPtr(s32 seId)
{
    if (seId < 0 || seId >= se.seMax) {
        return NULL;
    }
    return &se.seData[seId];
}

void msmSeDelListener(void)
{
    if (se.listenerF != FALSE) {
        sndRemoveListener(&se.listener);
        se.listenerF = FALSE;
    }
}

s32 msmSeUpdataListener(Vec *pos, Vec *heading)
{
    if (se.listenerF == FALSE) {
        return MSM_LISTENER_NO_UPDATA;
    }
    se.listenerDir.x = pos->x - se.listenerPos.x;
    se.listenerDir.y = pos->y - se.listenerPos.y;
    se.listenerDir.z = pos->z - se.listenerPos.z;
    se.listenerPos.x = pos->x;
    se.listenerPos.y = pos->y;
    se.listenerPos.z = pos->z;
    se.listenerHeading.x = heading->x;
    se.listenerHeading.y = heading->y;
    se.listenerHeading.z = heading->z;
    if (!sndUpdateListener(&se.listener, &se.listenerPos, &se.listenerDir, &se.listenerHeading, &se.listenerUp, 127, NULL)) {
        return MSM_LISTENER_NO_UPDATA;
    }
    return 0;
}

s32 msmSeSetListener(Vec *pos, Vec *heading, float sndDist, float sndSpeed, MSM_SELISTENER *listener)
{
    SND_FVECTOR *listenerDir;
    SND_FVECTOR *listenerPos;
    SND_FVECTOR *listenerUp;
    SND_FVECTOR *listenerHeading;
    s32 flag;
    float startDis;
    float frontSurDis;
    float backSurDis;

    listenerPos = &se.listenerPos;
    listenerUp = &se.listenerUp;
    listenerHeading = &se.listenerHeading;
    listenerDir = &se.listenerDir;
    listenerPos->x = pos->x;
    listenerPos->y = pos->y;
    listenerPos->z = pos->z;
    listenerUp->x = listenerUp->z = 0.0f;
    listenerUp->y = 1.0f;
    listenerHeading->x = heading->x;
    listenerHeading->y = heading->y;
    listenerHeading->z = heading->z;
    listenerDir->x = listenerDir->y = listenerDir->z = 0.0f;
    se.sndDist = sndDist;
    flag = (listener != NULL) ? listener->flag : MSM_LISTENER_NONE;
    startDis = (flag & MSM_LISTENER_STARTDIS) ? listener->startDis : 0.0f;
    frontSurDis = (flag & MSM_LISTENER_FRONTSURDIS) ? listener->frontSurDis : (startDis + sndDist * 0.25f);
    backSurDis = (flag & MSM_LISTENER_BACKSURDIS) ? listener->backSurDis : (startDis + sndDist * 0.25f);
    if ((s32)sndAddListenerEx(&se.listener, listenerPos, listenerDir, listenerHeading, listenerUp, frontSurDis, backSurDis, sndSpeed, -startDis,
            SND_EMITTER_CONTINOUS, 127, NULL)
        == FALSE) {
        se.listenerF = FALSE;
        return MSM_LISTENER_NO_UPDATA;
    }
    se.listenerF = TRUE;
    return 0;
}

s32 msmSeGetEntryID(s32 seId, int *seNo)
{
    SE_PLAYER *player;
    s32 i;
    s32 num;

    for (i = num = 0; i < se.sfx; i++) {
        player = &se.player[i];
        if (player->status != SE_STAT_DONE && player->seId == seId) {
            if (seNo != NULL) {
                seNo[num] = player->no;
            }
            num++;
        }
    }
    return num;
}

s32 msmSeGetNumPlay(BOOL baseGrp)
{
    switch (baseGrp) {
        case FALSE:
            return se.numPlay + se.baseGrpNumPlay;
        default:
            return se.numPlay;
    }
}

s32 msmSeGetStatus(int seNo)
{
    SE_PLAYER *player;

    player = msmSeSearchEntry(seNo);
    if (player == NULL) {
        return SE_STAT_DONE;
    }
    return player->status;
}

void msmSeSetMasterVolume(s32 vol)
{
    sndMasterVolume(vol & 127, 0, 0, 1);
}

s32 msmSeSetParam(int seNo, MSM_SEPARAM *param)
{
    SE_PLAYER *player;
    int pan;
    s32 pitch;

    player = msmSeSearchEntry(seNo);
    if (player == NULL) {
        return MSM_ERR_INVALIDSE;
    }
    if (param->flag & MSM_SEPARAM_VOL) {
        player->busyF = TRUE;
        player->vol = param->vol;
        if (!(player->flag & SE_PLAYER_EMIT) || player->emitterF != TRUE) {
            sndFXVolume(player->vid, SE_VOLUME(player));
        }
        player->busyF = FALSE;
    }
    if (!(player->flag & SE_PLAYER_EMIT)) {
        if (param->flag & MSM_SEPARAM_PAN) {
            player->pan = param->pan;
            pan = player->pan + player->basePan - 64;
            if (pan < 0) {
                pan = 0;
            }
            if (pan > 127) {
                pan = 127;
            }
            sndFXPanning(player->vid, pan);
        }
        if (param->flag & MSM_SEPARAM_PITCH) {
            player->pitch = param->pitch;
            pitch = player->pitch + player->basePitch + 8192;
            if (pitch < 0) {
                pitch = 0;
            }
            if (pitch > 16383) {
                pitch = 16383;
            }
            sndFXPitchBend(player->vid, pitch);
        }
        if (param->flag & MSM_SEPARAM_SPAN) {
            player->span = param->span;
            sndFXSurroundPanning(player->vid, player->span);
        }
    } else if (player->emitterF == TRUE) {
        player->vid = sndEmitterVoiceID(&player->emitter);
        if (param->flag & MSM_SEPARAM_POS) {
            player->busyF = TRUE;
            player->emiDir.x = param->pos.x - player->emiPos.x;
            player->emiDir.y = param->pos.y - player->emiPos.y;
            player->emiDir.z = param->pos.z - player->emiPos.z;
            player->emiPos.x = param->pos.x;
            player->emiPos.y = param->pos.y;
            player->emiPos.z = param->pos.z;
            sndUpdateEmitter(&player->emitter, &player->emiPos, &player->emiDir, SE_VOLUME(player), NULL);
            player->busyF = FALSE;
        }
    }
    if (param->flag & MSM_SEPARAM_AUXVOLA) {
        player->auxAVol = param->auxAVol;
        sndFXReverb(player->vid, player->auxAVol);
    }
    if (param->flag & MSM_SEPARAM_AUXVOLB) {
        player->auxBVol = param->auxBVol;
        sndFXChorus(player->vid, player->auxBVol);
    }
    if (param->flag & MSM_SEPARAM_PAD) {
        sndSendMessage(player->vid, param->pad);
    }
    return 0;
}

void msmSePauseAll(BOOL pause, s32 speed)
{
    s32 i;
    SE_PLAYER *player;

    for (i = 0; i < se.sfx; i++) {
        player = &se.player[i];
        player->busyF = TRUE;
        msmSePauseSub(player, pause, speed);
        player->busyF = FALSE;
    }
}

s32 msmSePause(int seNo, BOOL pause, s32 speed)
{
    SE_PLAYER *player;

    player = msmSeSearchEntry(seNo);
    if (player == NULL) {
        return MSM_ERR_INVALIDSE;
    }
    player->busyF = TRUE;
    msmSePauseSub(player, pause, speed);
    player->busyF = FALSE;
    return 0;
}

void msmSeStopAll(BOOL checkGrp, s32 speed)
{
    SE_PLAYER *player;
    s32 i;

    for (i = 0; i < se.sfx; i++) {
        player = &se.player[i];
        if (player->status == SE_STAT_DONE) {
            continue;
        }
        player->busyF = TRUE;
        switch (checkGrp) {
            case FALSE:
                player->fadeMaxTime = speed / 15;
                if (player->fadeMaxTime != 0) {
                    player->fadeTime = player->fadeMaxTime;
                    player->fadeVol = 127;
                } else {
                    player->status = SE_STAT_STOP;
                    if ((player->flag & SE_PLAYER_EMIT) && player->emitterF == TRUE) {
                        sndRemoveEmitter(&player->emitter);
                    } else {
                        sndFXKeyOff(player->vid);
                    }
                }
                break;
            default:
                if (msmSysCheckBaseGroup(se.seData[player->seId].groupId) == FALSE) {
                    player->fadeMaxTime = speed / 15;
                    if (player->fadeMaxTime != 0) {
                        player->fadeTime = player->fadeMaxTime;
                        player->fadeVol = 127;
                    } else {
                        player->status = SE_STAT_STOP;
                        if ((player->flag & SE_PLAYER_EMIT) && player->emitterF == TRUE) {
                            sndRemoveEmitter(&player->emitter);
                        } else {
                            sndFXKeyOff(player->vid);
                        }
                    }
                }
                break;
        }
        player->busyF = FALSE;
    }
}

s32 msmSeStop(int seNo, s32 speed)
{
    SE_PLAYER *player;

    player = msmSeSearchEntry(seNo);
    if (player == NULL) {
        return MSM_ERR_INVALIDSE;
    }
    player->busyF = TRUE;
    player->fadeMaxTime = speed / 15;
    if (player->fadeMaxTime != 0) {
        player->fadeTime = player->fadeMaxTime;
        player->fadeVol = 127;
    } else {
        player->status = SE_STAT_STOP;
        if ((player->flag & SE_PLAYER_EMIT) && player->emitterF == TRUE) {
            sndRemoveEmitter(&player->emitter);
        } else {
            sndFXKeyOff(player->vid);
        }
    }
    player->busyF = FALSE;
    return 0;
}

static inline BOOL msmSeUpdateBaseParam(SE_PLAYER *player, MSM_SEPARAM *param)
{
    BOOL listenerF = FALSE;

    if (param != NULL) {
        if (param->flag & MSM_SEPARAM_VOL) {
            player->vol = param->vol;
        }
        if (param->flag & MSM_SEPARAM_PAN) {
            player->pan = param->pan;
        }
        if (param->flag & MSM_SEPARAM_PITCH) {
            player->pitch = param->pitch;
        }
        if (param->flag & MSM_SEPARAM_SPAN) {
            player->span = param->span;
        }
        if (param->flag & MSM_SEPARAM_AUXVOLA) {
            player->auxAVol = param->auxAVol;
        }
        if (param->flag & MSM_SEPARAM_AUXVOLB) {
            player->auxBVol = param->auxBVol;
        }
        if ((param->flag & MSM_SEPARAM_POS) && se.listenerF != FALSE) {
            listenerF = TRUE;
        }
    }
    player->paramInfo.paraArray = player->param;
    return listenerF;
}

int msmSePlay(int seId, MSM_SEPARAM *param)
{
    SE_PLAYER *player;
    SND_EMITTER *emitter;
    MSMSE *seData;
    int pitch;
    s32 pan;
    s32 i;
    s32 emiFlag;

    if (seId < 0 || seId >= se.seMax) {
        return MSM_ERR_INVALIDID;
    }
    seData = &se.seData[seId];
    if (seData->groupId == 0xFFFF) {
        return MSM_ERR_REMOVEDID;
    }
    for (i = 0; i < se.sfx; i++) {
        player = &se.player[i];
        if (player->status == SE_STAT_DONE) {
            break;
        }
    }
    if (i == se.sfx) {
        return MSM_ERR_CHANLIMIT;
    }
    player->baseVol = seData->vol;
    player->basePan = seData->pan;
    player->basePitch = seData->pitchBend;
    player->vol = 127;
    player->pan = 64;
    player->span = seData->span;
    player->pitch = 0;
    player->auxAVol = seData->reverb;
    player->auxBVol = seData->chorus;
    player->pauseMaxTime = 0;
    player->fadeMaxTime = 0;
    player->pauseVol = 127;
    player->fadeVol = 127;
    player->emitterF = FALSE;
    if (msmSeUpdateBaseParam(player, param)) {
        player->emiPos.x = param->pos.x;
        player->emiPos.y = param->pos.y;
        player->emiPos.z = param->pos.z;
        player->param[0].ctrl = SND_MIDICTRL_REVERB;
        player->param[0].paraData.value7 = player->auxAVol;
        player->param[1].ctrl = SND_MIDICTRL_CHORUS;
        player->param[1].paraData.value7 = player->auxBVol;
        player->paramInfo.numPara = 2;
        player->emiDir.x = player->emiDir.y = player->emiDir.z = 0.0f;
        if (seData->doppler == TRUE) {
            emitter = &player->emitter;
            emiFlag = SND_EMITTER_DOPPLERFX | SND_EMITTER_CONTINOUS;
        } else {
            emitter = NULL;
            emiFlag = SND_EMITTER_DOPPLERFX;
        }
        player->emitterF = seData->doppler;
        player->vid = sndAddEmitterParaEx(emitter, &player->emiPos, &player->emiDir, se.sndDist, seData->comp / 127.0f, emiFlag, seData->fxId,
            se.groupId++, player->vol * player->baseVol / 127, 0, NULL, &player->paramInfo);
        if (emitter != NULL) {
            if (!sndCheckEmitter(emitter)) {
                return MSM_ERR_PLAYFAIL;
            }
        } else {
            if (player->vid == -1) {
                return MSM_ERR_PLAYFAIL;
            }
        }
        player->flag |= SE_PLAYER_EMIT;
    } else {
        player->param[0].ctrl = SND_MIDICTRL_VOLUME;
        player->param[0].paraData.value14 = (player->vol * player->baseVol / 127) * 129;
        pan = player->pan + player->basePan - 64;
        if (pan < 0) {
            pan = 0;
        }
        if (pan > 127) {
            pan = 127;
        }
        player->param[1].ctrl = SND_MIDICTRL_PANNING;
        player->param[1].paraData.value14 = pan * 129;
        player->param[2].ctrl = SND_MIDICTRL_REVERB;
        player->param[2].paraData.value7 = player->auxAVol;
        player->param[3].ctrl = SND_MIDICTRL_CHORUS;
        player->param[3].paraData.value7 = player->auxBVol;
        pitch = player->pitch + player->basePitch + 8192;
        if (pitch > 16383) {
            pitch = 16383;
        } else if (pitch < 0) {
            pitch = 0;
        }
        player->param[4].ctrl = SND_MIDICTRL_PITCHBEND;
        player->param[4].paraData.value14 = pitch;
        player->paramInfo.numPara = 5;
        player->vid = sndFXStartParaInfo(seData->fxId, 0xFF, 0xFF, 0, &player->paramInfo);
        if (player->vid == -1) {
            return MSM_ERR_PLAYFAIL;
        }
        sndFXSurroundPanning(player->vid, player->span);
        player->flag &= ~SE_PLAYER_EMIT;
    }
    player->busyF = TRUE;
    player->seId = seId;
    player->no = se.no++;
    player->status = SE_STAT_PLAY;
    player->busyF = FALSE;
    return player->no;
}

s32 msmSeInit(MSM_SYS *sys, DVDFileInfo *file)
{
    s32 playerSize;

    se.sfx = 0;
    se.seMax = 0;
    se.baseGrpNumPlay = 0;
    se.numPlay = 0;
    se.listenerF = FALSE;
    if (sys->info->seMax == 0) {
        return 0;
    }
    if ((se.seData = msmMemAlloc(sys->header->seSize)) == NULL) {
        return MSM_ERR_OUTOFMEM;
    }
    if (msmFioRead(file, se.seData, sys->header->seSize, sys->header->seOfs) < 0) {
        return MSM_ERR_READFAIL;
    }
    playerSize = sys->info->sfx * sizeof(SE_PLAYER);
    if ((se.player = msmMemAlloc(playerSize)) == NULL) {
        return MSM_ERR_OUTOFMEM;
    }
    memset(se.player, 0, playerSize);
    se.seMax = sys->info->seMax;
    se.sfx = sys->info->sfx;
    se.no = 1;
    return 0;
}
