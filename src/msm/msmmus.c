#include "msm/msmmus.h"
#include "msm/msmfio.h"
#include "msm/msmmem.h"

#define MUS_VOLUME(player) ((player)->baseVol * (player)->vol * (player)->fadeVol * (player)->pauseOnVol / (127 * 127 * 127))

typedef struct MusPlayer_s {
    /* 0x00 */ s16 musId;
    /* 0x02 */ s8 baseVol;
    /* 0x03 */ s8 vol;
    /* 0x04 */ s8 status;
    /* 0x05 */ s8 busyF;
    /* 0x08 */ SND_SEQID seqId;
    /* 0x0C */ void *songBuf;
    /* 0x10 */ void *arrfile;
    /* 0x14 */ volatile s32 fadeMaxTime;
    /* 0x18 */ s32 fadeTime;
    /* 0x1C */ s8 fadeVol;
    /* 0x20 */ volatile s32 pauseOnMaxTime;
    /* 0x24 */ s32 pauseOnTime;
    /* 0x28 */ s8 pauseOnVol;
    /* 0x2C */ volatile s32 pauseOffMaxTime;
    /* 0x30 */ s32 pauseOffTime;
    /* 0x34 */ s8 pauseOffVol;
    /* 0x35 */ volatile s8 startVol;
    /* 0x36 */ s8 targetVol;
    /* 0x38 */ SND_PLAYPARA playPara;
} MUS_PLAYER; // Size 0x58

typedef struct MsmMus_s {
    /* 0x000 */ s16 musMax;
    /* 0x002 */ s8 musChanMax;
    /* 0x003 */ s8 baseGrpNumPlay;
    /* 0x004 */ s8 numPlay;
    /* 0x008 */ s32 msmEntryNum;
    /* 0x00C */ MSM_MUS *musData;
    /* 0x010 */ s32 dummyMusOfs;
    /* 0x014 */ void *musBuf;
    /* 0x018 */ MUS_PLAYER player[MSM_MUS_MAX];
} MSM_MUS_WORK; // Size 0x178

static MSM_MUS_WORK mus;

static void msmMusPauseSub(MUS_PLAYER *player, BOOL pause, s32 speed)
{
    s32 time;

    time = speed / 15;
    if (pause) {
        if (player->status == MSM_MUS_PLAY) {
            if (time != 0) {
                player->pauseOnTime = time;
                player->pauseOnMaxTime = time;
                player->pauseOnVol = 127;
            } else {
                sndSeqPause(player->seqId);
                player->pauseOnVol = 0;
            }
            player->status = MSM_MUS_PAUSE;
        }
    } else {
        if (player->status == MSM_MUS_PAUSE) {
            if (time != 0) {
                if (player->pauseOnMaxTime != 0) {
                    player->pauseOnMaxTime = -time;
                } else {
                    sndSeqContinue(player->seqId);
                    player->pauseOnMaxTime = -time;
                    player->pauseOnTime = 0;
                    player->pauseOnVol = 0;
                }
            } else {
                sndSeqContinue(player->seqId);
                player->pauseOnVol = 127;
                sndSeqVolume(MUS_VOLUME(player), 0, player->seqId, 0);
            }
            player->status = MSM_MUS_PLAY;
        }
    }
}

static void msmMusPauseFade(MUS_PLAYER *player)
{
    if (player->pauseOnMaxTime > 0) {
        if (--player->pauseOnTime == 0) {
            player->pauseOnMaxTime = 0;
            player->pauseOnVol = 0;
            sndSeqPause(player->seqId);
        } else {
            player->pauseOnVol = player->pauseOnTime * 127 / player->pauseOnMaxTime;
            sndSeqVolume(MUS_VOLUME(player), 0, player->seqId, 0);
        }
    } else if (player->pauseOnMaxTime < 0) {
        if (++player->pauseOnTime >= -player->pauseOnMaxTime) {
            player->pauseOnMaxTime = 0;
            player->pauseOnVol = 127;
        } else {
            player->pauseOnVol = player->pauseOnTime * 127 / -player->pauseOnMaxTime;
        }
        sndSeqVolume(MUS_VOLUME(player), 0, player->seqId, 0);
    }
}

static void msmMusFade(MUS_PLAYER *player)
{
    if (player->status != MSM_MUS_PLAY) {
        return;
    }
    if (player->fadeMaxTime > 0) {
        if (--player->fadeTime == 0) {
            if (player->status != MSM_MUS_DONE) {
                player->fadeMaxTime = 0;
                if (player->fadeMaxTime != 0) {
                    player->fadeTime = player->fadeMaxTime;
                    player->fadeVol = 127;
                } else {
                    sndSeqStop(player->seqId);
                    player->status = MSM_MUS_STOP;
                }
            }
        } else {
            player->fadeVol = player->fadeTime * 127 / player->fadeMaxTime;
            sndSeqVolume(MUS_VOLUME(player), 0, player->seqId, 0);
        }
    } else if (player->fadeMaxTime < 0) {
        if (++player->fadeTime >= -player->fadeMaxTime) {
            player->fadeMaxTime = 0;
            player->fadeVol = 127;
        } else {
            player->fadeVol = player->fadeTime * 127 / -player->fadeMaxTime;
        }
        sndSeqVolume(MUS_VOLUME(player), 0, player->seqId, 0);
    }
}

void msmMusFdoutEnd(void)
{
}

void msmMusPeriodicProc(void)
{
    s32 i;
    MUS_PLAYER *player;

    mus.numPlay = mus.baseGrpNumPlay = 0;
    for (i = 0; i < mus.musChanMax; i++) {
        player = &mus.player[i];
        if (player->busyF != FALSE) {
            continue;
        }
        switch (player->status) {
            case MSM_MUS_STOP:
            case MSM_MUS_PLAY:
                if (sndSeqGetValid(player->seqId) == FALSE) {
                    player->status = MSM_MUS_DONE;
                }
                break;
        }
        if (player->status == MSM_MUS_DONE) {
            continue;
        }
        if (msmSysCheckBaseGroup(mus.musData[player->musId].sgid) != FALSE) {
            mus.numPlay++;
        } else {
            mus.baseGrpNumPlay++;
        }
        msmMusPauseFade(player);
        if (player->status == MSM_MUS_PLAY && player->pauseOffMaxTime != 0) {
            if (++player->pauseOffTime >= player->pauseOffMaxTime) {
                player->pauseOffMaxTime = 0;
                player->baseVol = player->targetVol;
            } else {
                player->baseVol = player->startVol + player->pauseOffTime * (player->targetVol - player->startVol) / player->pauseOffMaxTime;
            }
            sndSeqVolume(MUS_VOLUME(player), 0, player->seqId, 0);
        }
        msmMusFade(player);
    }
}

s32 msmMusGetMidiCtrl(int musNo, s32 channel, s32 ctrl)
{
    MUS_PLAYER *player;

    if (musNo < 0 || musNo >= mus.musChanMax) {
        return MSM_ERR_OUTOFMUS;
    }
    player = &mus.player[musNo];
    if (sndSeqGetValid(player->seqId) == TRUE) {
        return sndSeqGetMidiCtrl(player->seqId, channel & 0xF, ctrl);
    }
    return 0;
}

s32 msmMusGetNumPlay(BOOL baseGrp)
{
    switch (baseGrp) {
        case FALSE:
            return mus.baseGrpNumPlay + mus.numPlay;
        default:
            return mus.baseGrpNumPlay;
    }
}

s32 msmMusGetStatus(int musNo)
{
    if (musNo < 0 || musNo >= mus.musChanMax) {
        return MSM_ERR_OUTOFMUS;
    }
    return mus.player[musNo].status;
}

void msmMusSetMasterVolume(int volume)
{
    sndMasterVolume(volume & 127, 0, 1, 0);
}

void msmMusPauseAll(BOOL pause, s32 speed)
{
    s32 i;
    MUS_PLAYER *player;

    for (i = 0; i < mus.musChanMax; i++) {
        player = &mus.player[i];
        if (player->status != MSM_MUS_DONE) {
            player->busyF = TRUE;
            msmMusPauseSub(player, pause, speed);
            player->busyF = FALSE;
        }
    }
}

void msmMusStopAll(BOOL checkGrp, s32 speed)
{
    s32 i;
    MUS_PLAYER *player;

    for (i = 0; i < mus.musChanMax; i++) {
        player = &mus.player[i];
        if (player->status == MSM_MUS_DONE) {
            continue;
        }
        player->busyF = TRUE;
        switch (checkGrp) {
            case FALSE:
                if (player->status != MSM_MUS_DONE) {
                    player->fadeMaxTime = speed / 15;
                    if (player->fadeMaxTime != 0) {
                        player->fadeTime = player->fadeMaxTime;
                        player->fadeVol = 127;
                    } else {
                        sndSeqStop(player->seqId);
                        player->status = MSM_MUS_STOP;
                    }
                }
                break;
            default:
                if (msmSysCheckBaseGroup(mus.musData[player->musId].sgid) == FALSE && player->status != MSM_MUS_DONE) {
                    player->fadeMaxTime = speed / 15;
                    if (player->fadeMaxTime != 0) {
                        player->fadeTime = player->fadeMaxTime;
                        player->fadeVol = 127;
                    } else {
                        sndSeqStop(player->seqId);
                        player->status = MSM_MUS_STOP;
                    }
                }
                break;
        }
        player->busyF = FALSE;
    }
}

s32 msmMusStop(int musNo, s32 speed)
{
    MUS_PLAYER *player;

    if (musNo < 0 || musNo >= mus.musChanMax) {
        return MSM_ERR_OUTOFMUS;
    }
    player = &mus.player[musNo];
    player->busyF = TRUE;
    if (player->status != MSM_MUS_DONE) {
        player->fadeMaxTime = speed / 15;
        if (player->fadeMaxTime != 0) {
            player->fadeTime = player->fadeMaxTime;
            player->fadeVol = 127;
        } else {
            sndSeqStop(player->seqId);
            player->status = MSM_MUS_STOP;
        }
    }
    player->busyF = FALSE;
    return 0;
}

int msmMusPlay(int musId, MSM_MUSPARAM *musParam)
{
    s32 flag;
    int chan;
    SND_SEQID seqId;
    MSM_GRP_HEAD *grpData;
    MSM_MUS *musData;
    MUS_PLAYER *player;
    DVDFileInfo file;

    if (musId < 0 || musId >= mus.musMax) {
        return MSM_ERR_INVALIDID;
    }
    musData = &mus.musData[musId];
    if (musData->sgid == 0xFFFF) {
        return MSM_ERR_REMOVEDID;
    }
    if (msmSysCheckLoadGroupID(musData->sgid) == FALSE) {
        return MSM_ERR_GRP_NOTLOADED;
    }
    flag = (musParam != NULL) ? musParam->flag : MSM_MUSPARAM_NONE;
    chan = (flag & MSM_MUSPARAM_CHAN) ? musParam->chan : 0;
    if (chan < 0 || chan >= mus.musChanMax) {
        return MSM_ERR_OUTOFMUS;
    }
    player = &mus.player[chan];
    if (player->status != MSM_MUS_DONE) {
        sndSeqStop(player->seqId);
    }
    if (musData->songGrp < 0) {
        if (player->musId != musId) {
            if (msmFioOpen(mus.msmEntryNum, &file) != TRUE) {
                return MSM_ERR_OPENFAIL;
            }
            if (msmFioRead(&file, player->songBuf, musData->songSize, musData->songOfs + mus.dummyMusOfs) < 0) {
                msmFioClose(&file);
                return MSM_ERR_READFAIL;
            }
            msmFioClose(&file);
            player->arrfile = player->songBuf;
        }
    } else {
        grpData = msmSysGetGroupDataPtr(musData->songGrp);
        if (grpData == NULL) {
            return MSM_ERR_MUSGRP_NOTLOADED;
        }
        player->arrfile = (void *)((u32)grpData + grpData->sngOfs + musData->songOfs);
    }
    player->busyF = TRUE;
    player->vol = musData->vol;
    player->pauseOffMaxTime = 0;
    player->pauseOnMaxTime = 0;
    player->fadeMaxTime = 0;
    player->pauseOnVol = 127;
    player->fadeVol = 127;
    player->playPara.flags = SND_PLAYPARA_VOLUME;
    player->playPara.volume.time = 0;
    player->baseVol = (flag & MSM_MUSPARAM_VOL) ? musParam->vol : 127;
    player->playPara.volume.target = player->vol * player->fadeVol * player->baseVol / (127 * 127);
    if (flag & MSM_MUSPARAM_PAUSE) {
        player->playPara.flags |= SND_PLAYPARA_PAUSE;
    }
    if (flag & MSM_MUSPARAM_SPEED) {
        player->playPara.flags |= SND_PLAYPARA_SPEED;
        player->playPara.speed = musParam->speed * 256 / 100;
    }
    if (flag & MSM_MUSPARAM_FADESPEED) {
        player->fadeMaxTime = -(musParam->fadeSpeed / 15);
        if (player->fadeMaxTime != 0) {
            player->fadeTime = 0;
            player->fadeVol = 0;
            player->playPara.volume.target = 0;
        }
    }
    seqId = sndSeqPlay(musData->sgid, musData->sid, player->arrfile, &player->playPara);
    if (seqId == SND_ID_ERROR) {
        player->busyF = FALSE;
        return MSM_ERR_PLAYFAIL;
    }
    player->seqId = seqId;
    player->musId = musId;
    player->status = (player->playPara.flags & SND_PLAYPARA_PAUSE) ? MSM_MUS_PAUSE : MSM_MUS_PLAY;
    player->busyF = FALSE;
    return chan;
}

s32 msmMusInit(MSM_SYS *sys, DVDFileInfo *file)
{
    s32 size;
    s32 i;

    mus.musMax = 0;
    mus.musChanMax = 0;
    mus.numPlay = 0;
    mus.baseGrpNumPlay = 0;
    if (sys->info->musMax == 0) {
        return 0;
    }
    size = sys->header->musSize;
    if (size == 0) {
        return 0;
    }
    if ((mus.musData = msmMemAlloc(size)) == NULL) {
        return MSM_ERR_OUTOFMEM;
    }
    if (msmFioRead(file, mus.musData, sys->header->musSize, sys->header->musOfs) < 0) {
        return MSM_ERR_READFAIL;
    }
    size = sys->info->dummyMusSize;
    if (size != 0) {
        if ((mus.musBuf = msmMemAlloc(size * sys->info->musChanMax)) == NULL) {
            return MSM_ERR_OUTOFMEM;
        }
    } else {
        mus.musBuf = NULL;
    }
    mus.musMax = sys->info->musMax;
    mus.musChanMax = sys->info->musChanMax;
    mus.dummyMusOfs = sys->header->dummyMusOfs;
    mus.msmEntryNum = sys->msmEntryNum;
    for (i = 0; i < mus.musChanMax; i++) {
        mus.player[i].songBuf = (void *)((u32)mus.musBuf + sys->info->dummyMusSize * i);
        mus.player[i].musId = -1;
        mus.player[i].busyF = FALSE;
    }
    return 0;
}
