/* m562dll: Bowser boss battle. Module prolog/epilog, the minigame sequence hooks and the audio wrappers. */

#include "game/audio.h"
#include "game/memory.h"
#include "game/mg/actman.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"
#include "game/object.h"

#include "REL/m562dll.h"

/* the event units' view of the owner object, as their prototypes take it */
#define EVT(w) ((M562EvtWork *)(w))

typedef void (*M562VoidFunc)(void);
extern const M562VoidFunc _ctors[];
extern const M562VoidFunc _dtors[];

s32 _prolog(void);
void _epilog(void);
void ObjectSetup(void);

void fn_1_E8(s16 mode, s16 frameNo);
void fn_1_1B0(s16 mode, s16 frameNo);
void fn_1_20C(s16 mode, s16 frameNo);
void fn_1_2D0(s16 mode, s16 frameNo);
void fn_1_380(s16 mode, s16 frameNo);
void fn_1_410(s16 mode, s16 frameNo);
void fn_1_460(s16 mode, s16 frameNo);
void fn_1_5DC(s16 mode, s16 frameNo);
void fn_1_6DC(s16 mode, s16 frameNo);
void fn_1_7C8(s16 mode, s16 frameNo);

s32 lbl_1_data_0 = 1;
s32 lbl_1_data_4 = 1;

static MGSEQ_PARAM lbl_1_data_8 = {
    0,
    MGSEQ_TIMER_RIGHT,
    fn_1_E8,
    fn_1_20C,
    fn_1_2D0,
    fn_1_410,
    fn_1_460,
    fn_1_5DC,
    fn_1_6DC,
    fn_1_7C8,
    fn_1_1B0,
};

M562Work *lbl_1_bss_0;
OMOBJMAN *lbl_1_bss_4;

s32 _prolog(void)
{
    const M562VoidFunc *ctor = _ctors;

    while (*ctor != NULL) {
        (*ctor)();
        ctor++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const M562VoidFunc *dtor = _dtors;

    while (*dtor != NULL) {
        (*dtor)();
        dtor++;
    }
}

void ObjectSetup(void)
{
    lbl_1_bss_4 = MgActorObjectSetup();
    MgSeqCreate(&lbl_1_data_8);
    MgSeqModeHookAdd(MGSEQ_MODE_PREMAIN, fn_1_380);
}

void fn_1_E8(s16 mode, s16 frameNo)
{
    M562Work *work;

    work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M562Work), HU_MEMNUM_OVL);
    lbl_1_bss_0 = work;
    memset(lbl_1_bss_0, 0, sizeof(M562Work));
    fn_1_AC4(work, lbl_1_bss_4);
    work->step = 0;
    work->mode = 1;
    fn_1_16F0(EVT(work), 0);
    fn_1_22694(&work->light, 0, 1.0f);
    fn_1_4748(&work->back, 0, 1.0f);
    MgSeqStatBitReset(0x20);
    MgSeqModeNext();
}

void fn_1_1B0(s16 mode, s16 frameNo)
{
    M562Work *work = lbl_1_bss_0;

    fn_1_FB0(work);
    HuMemDirectFree(lbl_1_bss_0);
    lbl_1_bss_0 = NULL;
    MgSeqModeNext();
}

void fn_1_20C(s16 mode, s16 frameNo)
{
    M562Work *work = lbl_1_bss_0;

    MgSeqModeChangeOff();
    if (fn_1_1F54(work) != 0) {
        switch (work->mode) {
        case 1:
            fn_1_16F0(EVT(work), 0);
            break;
        case 2:
            fn_1_16F0(EVT(work), 1);
            break;
        case 3:
            fn_1_16F0(EVT(work), 2);
            break;
        case 4:
            fn_1_16F0(EVT(work), 3);
            break;
        }
        MgSeqModeNext();
    }
    work->step = 1;
    fn_1_1330(work, 0);
}

void fn_1_2D0(s16 mode, s16 frameNo)
{
    M562Work *work = lbl_1_bss_0;

    if (work->mode == 1 && work->bgmId < 0) {
        work->bgmId = fn_1_A44(0x40);
        fn_1_9BC(work->bgmId, 0x7F, 0);
    } else if (work->mode == 4 && work->bgmId < 0) {
        work->bgmId = fn_1_A44(0x41);
        fn_1_9BC(work->bgmId, 0x7F, 0);
    }
    work->step = 2;
    fn_1_1330(work, 0);
}

void fn_1_380(s16 mode, s16 frameNo)
{
    M562Work *work = lbl_1_bss_0;

    fn_1_2034(work);
    work->step = 3;
    fn_1_1330(work, 0);
    lbl_1_data_0 = 1;
    if (work->mode == 2 || work->mode == 3) {
        if (work->bgmId >= 0) {
            fn_1_9BC(work->bgmId, 0x7F, 1000);
        }
    }
}

void fn_1_410(s16 mode, s16 frameNo)
{
    M562Work *work = lbl_1_bss_0;

    work->step = 4;
    if (fn_1_1330(work, 1) != 0) {
        MgSeqModeNext();
    }
}

void fn_1_460(s16 mode, s16 frameNo)
{
    M562Work *work = lbl_1_bss_0;

    if (lbl_1_data_0 != 0) {
        work->player.hitFlag = 0;
        switch (work->mode) {
        case 1:
            fn_1_1CB0(work, 0, 0);
            break;
        case 2:
            fn_1_1CB0(work, 1, 0);
            break;
        case 3:
            fn_1_1CB0(work, 2, 0);
            break;
        case 4:
            fn_1_1CB0(work, 3, 0);
            break;
        }
        M562TimerKill(work);
        if (work->mode == 4) {
            fn_1_1D9C(EVT(work));
        }
        lbl_1_data_4 = 1;
        lbl_1_data_0 = 0;
        if (work->mode == 3 || work->mode == 4) {
            fn_1_9F4(work->bgmId, 100);
            work->bgmId = -1;
        }
    }
    work->step = 5;
    fn_1_1330(work, 0);
}

void fn_1_5DC(s16 mode, s16 frameNo)
{
    M562Work *work = lbl_1_bss_0;

    MgSeqModeChangeOff();
    if (lbl_1_data_4 != 0) {
        if (work->mode == 1 || work->mode == 2) {
            if (work->bgmId >= 0) {
                fn_1_9BC(work->bgmId, 0x60, 1000);
            }
        }
        lbl_1_data_4 = 0;
    }
    if (work->winner == 0 && (work->mode == 3 || work->mode == 4)) {
        if (fn_1_1F54(work) != 0) {
            MgSeqModeNext();
            work->state = 0;
            work->evtTimer = 0;
        }
    } else {
        MgSeqModeNext();
        work->state = 0;
        work->evtTimer = 0;
    }
    work->step = 6;
    fn_1_1330(work, 0);
}

void fn_1_6DC(s16 mode, s16 frameNo)
{
    M562Work *work = lbl_1_bss_0;

    MgSeqModeChangeOff();
    if (work->winner == 0) {
        if (work->mode == 4) {
            MgSeqModeNext();
        } else {
            switch (work->mode) {
            case 1:
                work->mode = 2;
                MgSeqStatBitSet(0x20);
                break;
            case 2:
                work->mode = 3;
                MgSeqStatBitSet(0x20);
                break;
            case 3:
                work->mode = 4;
                MgSeqStatBitReset(0x20);
                break;
            }
            MgSeqModeSet(MGSEQ_MODE_FADEIN);
        }
    } else if (fn_1_2C118(EVT(work)) != 0) {
        MgSeqModeSet(MGSEQ_MODE_CLOSE);
    }
    work->step = 7;
    fn_1_1330(work, 0);
}

void fn_1_7C8(s16 mode, s16 frameNo)
{
    M562Work *work = lbl_1_bss_0;

    fn_1_1F54(work);
    work->step = 8;
    fn_1_1330(work, 0);
}

s32 fn_1_814(s32 seId)
{
    return HuAudFXPlay(seId);
}

void fn_1_83C(s32 seNo)
{
    HuAudFXStop(seNo);
}

void fn_1_864(s32 seNo, s32 speed)
{
    HuAudFXFadeOut(seNo, speed);
}

void fn_1_894(s32 seNo, s16 pan)
{
    HuAudFXPanning(seNo, pan);
}

void fn_1_8C4(Vec *pos, Vec *heading, f32 sndDist, f32 sndSpeed)
{
    HuAudFXListnerSet(pos, heading, sndDist, sndSpeed);
}

void fn_1_904(Vec *pos, Vec *heading)
{
    HuAudFXListnerUpdate(pos, heading);
}

s32 fn_1_934(s32 seId, Vec *pos)
{
    return HuAudFXEmiterPlay(seId, pos);
}

void fn_1_964(s32 seNo, Vec *pos)
{
    HuAudFXEmiterUpDate(seNo, pos);
}

s32 fn_1_994(s16 streamId)
{
    return HuAudSStreamPlay(streamId);
}

void fn_1_9BC(s32 streamNo, u8 vol, s32 speed)
{
    HuAudSStreamVolSet(streamNo, vol, speed);
}

void fn_1_9F4(s32 streamNo, s32 speed)
{
    HuAudSStreamFadeOut(streamNo, speed);
}

void fn_1_A24(void)
{
    HuAudAllStop();
}

s32 fn_1_A44(s16 streamId)
{
    return HuAudBGMPlay(streamId);
}

void fn_1_A6C(s16 streamId)
{
    HuAudJinglePlay(streamId);
}

s32 fn_1_A94(s16 streamId, s16 chanNo)
{
    return HuAudSStreamChanPlay(streamId, chanNo);
}
