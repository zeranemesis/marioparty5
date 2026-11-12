#include "REL/m504dll.h"
#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/flag.h"
#include "game/gamemes.h"
#include "game/memory.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"
#include "game/wipe.h"
#include "string.h"

/* Debug line drawing: raw FIFO writes on GC, GX vertex calls on PC. */
#ifdef TARGET_PC
#define M504_POS(v) GXPosition3f32((v).x, (v).y, (v).z)
#define M504_COL(c) GXColor4u8((c).r, (c).g, (c).b, (c).a)
#else
#define M504_POS(v)           \
    do {                      \
        GXWGFifo.f32 = (v).x; \
        GXWGFifo.f32 = (v).y; \
        GXWGFifo.f32 = (v).z; \
    } while (0)
#define M504_COL(c)          \
    do {                     \
        GXWGFifo.u8 = (c).r; \
        GXWGFifo.u8 = (c).g; \
        GXWGFifo.u8 = (c).b; \
        GXWGFifo.u8 = (c).a; \
    } while (0)
#endif

typedef struct DbgLine_s {
    Vec a;
    Vec b;
} DBGLINE;

typedef struct DbgTri_s {
    Vec a;
    Vec b;
    Vec c;
} DBGTRI;

typedef struct DbgQuad_s {
    Vec a;
    Vec b;
    Vec c;
    Vec d;
} DBGQUAD;

extern s32 fn_1_E14C(M504WORK *work);
extern s32 fn_1_118E8(M504WORK *work);
extern void fn_1_128E0(M504WORK *work);
extern void fn_1_23DC(M504SLOT *slot, s32 type, M504WORK *work);
extern void fn_1_24A0(M504SLOT *slot);
extern s32 fn_1_24F8(M504SLOT *slot);
extern void fn_1_D03C(M504LIGHT *light);
extern void fn_1_D0B4(M504LIGHT *light);
extern void fn_1_D10C(void);
extern void fn_1_D15C(void);
extern void fn_1_DA68(void);

void fn_1_114(void);
void fn_1_190(void);
void fn_1_1EC(void);
void fn_1_25C(void);
void fn_1_298(void);
void fn_1_2F0(void);
void fn_1_37C(void);
void fn_1_460(void);
void fn_1_59C(void);
void fn_1_614(s32 seNo);
void fn_1_63C(s32 seNo, s32 speed);
s32 fn_1_794(s16 streamId);
void fn_1_71C(s32 streamNo, s32 speed);
void fn_1_74C(void);
void fn_1_7EC(M504WORK *work, HUPROCESS *proc);
void fn_1_9F0(M504WORK *work);
void fn_1_AA8(M504WORK *work);
void fn_1_AF0(M504WORK *work);
void fn_1_B38(s16 *mdl);
void fn_1_C08(s16 *mdl);
s32 fn_1_C64(M504WORK *work, s32 arg1);
s32 fn_1_F34(M504WORK *work);
s32 fn_1_1080(M504WORK *work);
s32 fn_1_10C4(M504WORK *work);

MGSEQ_PARAM lbl_1_data_0 = {
    300,
    0,
    (MGSEQ_FUNC)fn_1_114,
    (MGSEQ_FUNC)fn_1_1EC,
    (MGSEQ_FUNC)fn_1_25C,
    (MGSEQ_FUNC)fn_1_298,
    (MGSEQ_FUNC)fn_1_2F0,
    (MGSEQ_FUNC)fn_1_37C,
    (MGSEQ_FUNC)fn_1_460,
    (MGSEQ_FUNC)fn_1_59C,
    (MGSEQ_FUNC)fn_1_190,
};
s32 lbl_1_data_28 = 1;
s32 lbl_1_data_2C[19] = {
    0x3E0000, 0x3E0001, 0x3E0002, 0x3E0003, 0x3E0004, 0x3E0005, 0x3E0006, 0x3E0007, 0x3E0008, 0x3E0009,
    0x3E000A, 0x3E000B, 0x3E000C, 0x3E000D, 0x3E000E, 0x3E000F, 0x3E0010, 0x3E0011, 0x3E0012,
};
HuVecF lbl_1_data_78[4] = {
    { 16.0f, 40.0f, 0.0f },
    { 464.0f, 40.0f, 0.0f },
    { 16.0f, 408.0f, 0.0f },
    { 464.0f, 408.0f, 0.0f },
};

M504WORK *lbl_1_bss_0;
HUPROCESS *lbl_1_bss_4;
u16 lbl_1_bss_8;
u16 lbl_1_bss_A;
u16 lbl_1_bss_C;
GXColor lbl_1_bss_E[0x100];
GXColor lbl_1_bss_40E[0x100];
GXColor lbl_1_bss_80E[0x100];
DBGQUAD lbl_1_bss_C10[0x100];
DBGTRI lbl_1_bss_3C10[0x100];
DBGLINE lbl_1_bss_6010[0x100];

int _prolog(void)
{
    const VoidFunc *ctorPtr = _ctors;

    for (; *ctorPtr != 0; ctorPtr++) {
        (*ctorPtr)();
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtorPtr = _dtors;

    while (*dtorPtr != 0) {
        (*dtorPtr)();
        dtorPtr++;
    }
}

void ObjectSetup(void)
{
    lbl_1_bss_4 = omInitObjMan(0x200, 0x2000);
    omGameSysInit(lbl_1_bss_4);
    if (_CheckFlag(0x30002)) {
        lbl_1_data_0.maxTime = 0;
    }
    MgSeqCreate(&lbl_1_data_0);
}

void fn_1_114(void)
{
    M504WORK *work;

    work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M504WORK), 0x10000000);
    lbl_1_bss_0 = work;
    memset(lbl_1_bss_0, 0, sizeof(M504WORK));
    fn_1_7EC(work, lbl_1_bss_4);
    work->winner = -1;
    MgSeqModeNext();
}

void fn_1_190(void)
{
    fn_1_9F0(lbl_1_bss_0);
    HuMemDirectFree(lbl_1_bss_0);
    lbl_1_bss_0 = NULL;
    MgSeqModeNext();
}

void fn_1_1EC(void)
{
    M504WORK *work = lbl_1_bss_0;

    MgSeqModeChangeOff();
    if (fn_1_F34(work) != 0) {
        if (work->unk1234 >= 0) {
            fn_1_614(work->unk1234);
            work->unk1234 = -1;
        }
        MgSeqModeNext();
    }
    fn_1_C64(work, 0);
}

void fn_1_25C(void)
{
    fn_1_C64(lbl_1_bss_0, 0);
}

void fn_1_298(void)
{
    M504WORK *work = lbl_1_bss_0;

    work->unk1210 = 1;
    if (fn_1_C64(work, 1) != 0) {
        work->unk1210 = 0;
        MgSeqModeNext();
    }
}

void fn_1_2F0(void)
{
    M504WORK *work = lbl_1_bss_0;
    s32 mesId;

    work->unk1210 = 0;
    mesId = MgSeqGameMesIdGet();
    if (work->unk1230 >= 0 && (GameMesStatGet(mesId) & 0x10) != 0) {
        fn_1_71C(work->unk1230, 100);
        work->unk1230 = -1;
    }
    fn_1_C64(work, 0);
}

void fn_1_37C(void)
{
    M504WORK *work = lbl_1_bss_0;
    s32 i;
    s32 allDone;
    M504SLOT *slot;

    MgSeqModeChangeOff();
    if (lbl_1_bss_0->unk122C != 0) {
        allDone = 1;
        for (i = 0; i < 4; i++) {
            slot = &work->slot[i];
            if (slot->player.mode == 2 || slot->player.mode == 3) {
                allDone = 0;
            }
        }
        if (allDone != 0) {
            WipeCreate(2, 6, 60);
            MgSeqModeNext();
        }
    } else if (fn_1_10C4(work) != 0) {
        MgSeqModeNext();
    }
    fn_1_C64(work, 0);
}

void fn_1_460(void)
{
    M504WORK *work = lbl_1_bss_0;
    M504SLOT *slot;
    s32 i;
    s32 score;
    s32 winner;

    MgSeqModeChangeOff();
    if (lbl_1_bss_0->unk122C != 0) {
        if (WipeCheckEnd() == 0) {
            for (i = 0; i < 4; i++) {
                slot = &work->slot[i];
                score = MgTimerValueGet(slot->timer);
                if (slot->player.doneD0 != 0) {
                    GwPlayer[slot->player.playerNo].mgScore = score;
                } else {
                    GwPlayer[slot->player.playerNo].mgScore = 0;
                }
            }
            MgSeqModeSet(10);
            return;
        }
    } else {
        if (fn_1_1080(work) != 0) {
            if (work->winner >= 0) {
                winner = work->winner;
                if (_CheckFlag(0x1000F) == 0) {
                    GwPlayer[winner].mgCoinBonus = 10;
                }
            }
            MgSeqModeNext();
        }
    }
    fn_1_C64(work, 0);
}

void fn_1_59C(void)
{
    M504WORK *work = lbl_1_bss_0;

    if (work->winner >= 0) {
        fn_1_1080(work);
    }
    fn_1_C64(work, 0);
}

void fn_1_5EC(s32 seId)
{
    HuAudFXPlay(seId);
}

void fn_1_614(s32 seNo)
{
    HuAudFXStop(seNo);
}

void fn_1_63C(s32 seNo, s32 speed)
{
    HuAudFXFadeOut(seNo, speed);
}

void fn_1_66C(s32 seNo, s16 pan)
{
    HuAudFXPanning(seNo, pan);
}

void fn_1_69C(Vec *pos, Vec *heading, f32 sndDist, f32 sndSpeed, f32 startDis, f32 frontSurDis, f32 backSurDis)
{
    HuAudFXListnerSetEX(pos, heading, sndDist, sndSpeed, startDis, frontSurDis, backSurDis);
}

void fn_1_6F4(s16 streamId)
{
    HuAudSStreamPlay(streamId);
}

void fn_1_71C(s32 streamNo, s32 speed)
{
    HuAudSStreamFadeOut(streamNo, speed);
}

void fn_1_74C(void)
{
    HuAudAllStop();
}

void fn_1_76C(s16 streamId)
{
    HuAudBGMPlay(streamId);
}

s32 fn_1_794(s16 streamId)
{
    return HuAudJinglePlay(streamId);
}

void fn_1_7BC(s16 streamId, s16 chanNo)
{
    HuAudSStreamChanPlay(streamId, chanNo);
}

void fn_1_7EC(M504WORK *work, HUPROCESS *proc)
{
    u16 i;

    memset(work, 0, sizeof(M504WORK));
    work->proc = proc;
    work->timer = 10800;
    work->unk122C = _CheckFlag(0x30002);
    work->gameMes = -1;
    work->unk1230 = -1;
    work->unk1234 = -1;
    work->esp121C = espEntry(0x3E004A, 10, 0);
    espDispOff(work->esp121C);
    espDrawNoSet(work->esp121C, 0);
    espPosSet(work->esp121C, 576.0f, 240.0f);
    espAttrSet(work->esp121C, 1);
    work->esp121E = espEntry(0x3E004B, 10, 0);
    espDispOff(work->esp121E);
    espDrawNoSet(work->esp121E, 0);
    espPosSet(work->esp121E, 576.0f, 240.0f);
    espAttrSet(work->esp121E, 1);
    fn_1_AA8(work);
    fn_1_D03C(&work->light);
    for (i = 0; i < 4; i++) {
        fn_1_23DC(&work->slot[i], i, work);
    }
    Hu3DShadowMultiCreate(20.0f, 20.0f, 30000.0f, 15);
    Hu3DShadowMultiTPLvlSet(0.5f, -1);
    Hu3DShadowMultiSizeSet(192, -1);
    fn_1_D10C();
    fn_1_DA68();
    work->winner = -1;
    lbl_1_data_28 = 1;
    Hu3DBGColorSet(0, 0, 0);
    WipeCreate(1, 5, 60);
    Hu3DNoSyncSet(1);
}

void fn_1_9F0(M504WORK *work)
{
    u16 i;

    fn_1_128E0(work);
    fn_1_D15C();
    for (i = 0; i < 4; i++) {
        fn_1_24A0(&work->slot[i]);
    }
    fn_1_D0B4(&work->light);
    fn_1_AF0(work);
    if (work->gameMes >= 0) {
        GameMesKill(work->gameMes);
    }
    GameMesClose();
    espKill(work->esp121C);
    espKill(work->esp121E);
    memset(work, 0, sizeof(M504WORK));
    fn_1_74C();
}

void fn_1_AA8(M504WORK *work)
{
    s16 *mdl = work->mdl;

    memset(mdl, 0, sizeof(work->mdl));
    fn_1_B38(mdl);
}

void fn_1_AF0(M504WORK *work)
{
    s16 *mdl = work->mdl;

    fn_1_C08(mdl);
    memset(mdl, 0, sizeof(work->mdl));
}

void fn_1_B38(s16 *mdl)
{
    s32 i;

    for (i = 0; i < 19; i++) {
        mdl[i] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_2C[i], 0x10000000, HEAP_MODEL));
        switch (lbl_1_data_2C[i]) {
            case 0x3E0007:
            case 0x3E000C:
                Hu3DModelShadowMapSet(mdl[i]);
                break;
        }
        Hu3DModelAttrSet(mdl[i], 1);
    }
}

void fn_1_C08(s16 *mdl)
{
    s32 i;

    for (i = 0; i < 19; i++) {
        Hu3DModelKill(mdl[i]);
        mdl[i] = 0;
    }
}

s32 fn_1_C64(M504WORK *work, s32 arg1)
{
    u16 i;
    s32 ret;
    s32 allDone;
    M504SLOT *slot;

    if (MgSeqModeGet() >= 5 && lbl_1_data_28 != 0) {
        if (lbl_1_bss_0->unk122C != 0) {
            for (i = 0; i < 4; i++) {
                slot = &work->slot[i];
                MgTimerParamSet(slot->timer, 0, 5400, 0);
                MgTimerModeOnSet(slot->timer, 2);
            }
        }
        lbl_1_data_28 = 0;
    }
    ret = 0;
    if (lbl_1_bss_0->unk122C == 0) {
        for (i = 0; i < 4; i++) {
            slot = &work->slot[i];
            if (fn_1_24F8(slot) != 0) {
                ret = 1;
                if (arg1 != 0) {
                    work->winner = i;
                    work->unk1210 = 0;
                    break;
                }
            }
            if (work->unk1210 != 0) {
                slot->player.active = 1;
                slot->wanwan.active = 1;
            } else {
                slot->player.active = 0;
                slot->wanwan.active = 0;
            }
        }
    } else {
        allDone = 1;
        for (i = 0; i < 4; i++) {
            slot = &work->slot[i];
            if (work->unk1210 != 0) {
                if (slot->player.doneD0 == 0) {
                    slot->player.active = 1;
                    slot->wanwan.active = 1;
                }
            } else {
                slot->player.active = 0;
                slot->wanwan.active = 0;
            }
            if (fn_1_24F8(slot) != 0 && MgSeqModeGet() >= 5) {
                slot->player.active = 0;
                slot->wanwan.active = 0;
                if (MgTimerModeGet(slot->timer) != 0) {
                    if (slot->timer->stopF == 0) {
                        slot->timer->stopF = 1;
                        slot->timer->mode = 1;
                    }
                    slot->player.doneD0 = 1;
                }
            }
            if (MgSeqModeGet() >= 5 && MgTimerModeGet(slot->timer) != 0) {
                allDone = 0;
            }
        }
        if (MgSeqModeGet() >= 5 && allDone != 0) {
            for (i = 0; i < 4; i++) {
                slot = &work->slot[i];
                slot->player.active = 0;
                slot->wanwan.active = 0;
            }
            ret = 1;
        }
    }
    if (work->timer != 0) {
        work->timer--;
    }
    return ret;
}

s32 fn_1_F34(M504WORK *work)
{
    s32 ret;
    u16 i;
    M504SLOT *slot;

    ret = 0;
    if (fn_1_E14C(work) != 0) {
        for (i = 0; i < 4; i++) {
            slot = &work->slot[i];
            Hu3DModelAttrSet(slot->stage.mdl22, 1);
            Hu3DModelAttrSet(slot->player.mdlC8, 0x400000);
            if (lbl_1_bss_0->unk122C != 0) {
                slot->timer = MgTimerCreate(2);
                MgTimerPosSet(slot->timer, 56.0f + lbl_1_data_78[i].x, 24.0f + lbl_1_data_78[i].y);
                MgTimerDigitColorSet(slot->timer, 255, 255, 255);
                MgTimerParamSet(slot->timer, 0, 0, 0);
                MgTimerModeOnSet(slot->timer, 0);
            }
        }
        ret = 1;
    }
    return ret;
}

s32 fn_1_1080(M504WORK *work)
{
    s32 ret = 0;

    if (fn_1_118E8(work) != 0) {
        ret = 1;
    }
    return ret;
}

s32 fn_1_10C4(M504WORK *work)
{
    s32 i;

    if (work->winner >= 0) {
        return 1;
    }
    for (i = 0; i < 4; i++) {
        if (work->slot[i].wanwan.state < 5) {
            return 0;
        }
    }
    for (i = 0; i < 4; i++) {
        if (work->slot[i].player.mode == 2 || work->slot[i].player.mode == 3) {
            return 0;
        }
    }
    if (work->gameMes < 0) {
        work->gameMes = GameMesCreate(2, 2);
        work->unk1210 = 0;
        work->unk10 = 0;
        for (i = 0; i < 4; i++) {
            work->slot[i].player.unk8C = 1;
            work->slot[i].wanwan.unk24 = 1;
        }
    } else {
        if (work->unk1230 < 0) {
            work->unk1230 = fn_1_794(0x7B);
        }
        if (work->unk10 == 0) {
            CharLoseVoicePlay(work->slot[0].player.charNo, work->slot[1].player.charNo, work->slot[2].player.charNo, work->slot[3].player.charNo);
        }
        work->unk10++;
        if (work->unk10 >= 210) {
            MgSeqModeSet(9);
            work->gameMes = -1;
            for (i = 0; i < 4; i++) {
                if (work->slot[i].stage.unk5C >= 0) {
                    fn_1_63C(work->slot[i].stage.unk5C, 1000);
                    work->slot[i].stage.unk5C = -1;
                }
            }
        }
    }
    return 0;
}

void fn_1_12A8(Mtx mtx)
{
    s32 i;

    GXLoadPosMtxImm(mtx, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetZMode(0, GX_ALWAYS, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_VTX, GX_SRC_VTX, 1, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetZCompLoc(0);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
    GXSetAlphaUpdate(1);
    for (i = 0; i < lbl_1_bss_8; i++) {
        GXBegin(GX_LINESTRIP, GX_VTXFMT0, 2);
        M504_POS(lbl_1_bss_6010[i].a);
        M504_COL(lbl_1_bss_80E[i]);
        M504_POS(lbl_1_bss_6010[i].b);
        M504_COL(lbl_1_bss_80E[i]);
#ifdef TARGET_PC
        GXEnd();
#endif
    }
    for (i = 0; i < lbl_1_bss_A; i++) {
        GXBegin(GX_LINESTRIP, GX_VTXFMT0, 4);
        M504_POS(lbl_1_bss_3C10[i].a);
        M504_COL(lbl_1_bss_40E[i]);
        M504_POS(lbl_1_bss_3C10[i].b);
        M504_COL(lbl_1_bss_40E[i]);
        M504_POS(lbl_1_bss_3C10[i].c);
        M504_COL(lbl_1_bss_40E[i]);
        M504_POS(lbl_1_bss_3C10[i].a);
        M504_COL(lbl_1_bss_40E[i]);
#ifdef TARGET_PC
        GXEnd();
#endif
    }
    for (i = 0; i < lbl_1_bss_C; i++) {
        GXBegin(GX_LINESTRIP, GX_VTXFMT0, 5);
        M504_POS(lbl_1_bss_C10[i].a);
        M504_COL(lbl_1_bss_E[i]);
        M504_POS(lbl_1_bss_C10[i].b);
        M504_COL(lbl_1_bss_E[i]);
        M504_POS(lbl_1_bss_C10[i].c);
        M504_COL(lbl_1_bss_E[i]);
        M504_POS(lbl_1_bss_C10[i].d);
        M504_COL(lbl_1_bss_E[i]);
        M504_POS(lbl_1_bss_C10[i].a);
        M504_COL(lbl_1_bss_E[i]);
#ifdef TARGET_PC
        GXEnd();
#endif
    }
    lbl_1_bss_C = 0;
    lbl_1_bss_A = 0;
    lbl_1_bss_8 = 0;
}

void fn_1_20B8(Vec *a, Vec *b, GXColor *color)
{
    if (lbl_1_bss_8 < 0x100) {
        lbl_1_bss_6010[lbl_1_bss_8].a = *a;
        lbl_1_bss_6010[lbl_1_bss_8].b = *b;
        lbl_1_bss_80E[lbl_1_bss_8] = *color;
        lbl_1_bss_8++;
    }
}

void fn_1_2190(Vec *a, Vec *b, Vec *c, GXColor *color)
{
    if (lbl_1_bss_A < 0x100) {
        lbl_1_bss_3C10[lbl_1_bss_A].a = *a;
        lbl_1_bss_3C10[lbl_1_bss_A].b = *b;
        lbl_1_bss_3C10[lbl_1_bss_A].c = *c;
        lbl_1_bss_40E[lbl_1_bss_A] = *color;
        lbl_1_bss_A++;
    }
}

void fn_1_229C(Vec *a, Vec *b, Vec *c, Vec *d, GXColor *color)
{
    if (lbl_1_bss_C < 0x100) {
        lbl_1_bss_C10[lbl_1_bss_C].a = *a;
        lbl_1_bss_C10[lbl_1_bss_C].b = *b;
        lbl_1_bss_C10[lbl_1_bss_C].c = *c;
        lbl_1_bss_C10[lbl_1_bss_C].d = *d;
        lbl_1_bss_E[lbl_1_bss_C] = *color;
        lbl_1_bss_C++;
    }
}
