#include "types.h"

#include "string.h"
#include "game/gamework.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/charman.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/audio.h"
#include "game/pad.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"
#include "game/mg/actman.h"
#include "math.h"
#include "REL/m551Dll.h"

#define MDL(i) (lbl_1_bss_17A00->models[i])
#define PW lbl_1_bss_16A84
#define FL lbl_1_bss_16CB4

/* game.c */
void fn_1_5FF4(M551State *state);
void fn_1_9094(void);
void fn_1_9360(void);
void fn_1_115B0(void);
int fn_1_117B0(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b);
void fn_1_117CC(MGACTOR *actorP, int param);
void fn_1_11D34(s16 arg0);
s32 fn_1_11F4C(s16 arg0);
s16 fn_1_12178(s16 arg0);
s16 fn_1_12510(s16 arg0);
s16 fn_1_13384(s16 arg0);
s16 fn_1_13620(s16 arg0);
s16 fn_1_13AA4(s16 arg0);
s16 fn_1_13F68(s16 arg0);
s16 fn_1_143DC(s16 arg0);
s16 fn_1_14974(s16 arg0);
s32 fn_1_14FC4(s16 arg0);
void fn_1_153A8(M551_PLAYERWORK *work);
s32 fn_1_9654(s16 arg0);
void fn_1_9934(void);
s16 fn_1_A078(s16 arg0, s16 arg1);
s16 fn_1_A5A4(s16 arg0, s16 arg1);
s32 fn_1_A758(s16 arg0);
s16 fn_1_ADA4(s16 arg0, s16 arg1);
s16 fn_1_B560(s16 arg0, s16 arg1);
s16 fn_1_BE48(s16 arg0, s16 arg1);
s16 fn_1_C680(s16 arg0, s16 arg1);
s16 fn_1_EA8C(void);
s16 fn_1_F814(void);

/* unit-owned .bss, declared in reverse address order so the offsets match the original */
M551State *lbl_1_bss_17A00;
M551_ITEMMODEL lbl_1_bss_179A0[12];
s16 lbl_1_bss_1799C[2];
void *lbl_1_bss_17998;

/* unit-owned .data */
s16 (*lbl_1_data_5B0[5])(s16, s16) = { fn_1_A5A4, fn_1_ADA4, fn_1_B560, fn_1_BE48, fn_1_C680 };

void fn_1_5FF4(M551State *state)
{
    MGACTOR_PARAM param;
    HuVecF origin = { 0.0f, 100.0f, 0.0f };
    HuVecF pos;
    s16 cnt[5] = { 0, 0, 0, 0, 0 };
    s16 maxCnt[5] = { 1, 2, 3, 3, 3 };
    s16 used[12];
    s16 ids[10];
    s16 i;
    s16 j;
    s16 k;
    s16 rnd;
    s16 kind;
    s16 found;

    memset(&FL, 0, sizeof(M551_FLOW));
    memset(PW, 0, sizeof(PW));
    memset(lbl_1_bss_16844, 0, sizeof(lbl_1_bss_16844));
    memset(&lbl_1_bss_1679C, 0, sizeof(M551PaperMgr));
    memset(lbl_1_bss_3C, 0, sizeof(lbl_1_bss_3C));
    memset(&lbl_1_bss_0, 0, sizeof(M551_WORK));
    lbl_1_bss_17A00 = state;
    for (i = 0; i < 8; i++) {
        FL.fxOn[i] = -1;
    }
    for (i = 0; i < 12; i++) {
        FL.owner[i] = -1;
        used[i] = 0;
    }
    FL.fadeTp = 0.0f;
    FL.ambScale = 1.0f;
    PW[0].colorScale = 255.0f;
    PW[1].colorScale = 255.0f;
    PW[0].unk30 = 1.0f;
    PW[1].unk30 = 1.0f;
    PW[0].itemIdx = -1;
    PW[1].itemIdx = -1;
    PW[0].carriedBy = -1;
    PW[1].carriedBy = -1;
    for (i = 0; i < 12; i++) {
        k = i * 4 + 6;
        for (j = 0; j < 4; j++, k++) {
            lbl_1_bss_179A0[i].mdl[j] = k;
        }
    }
    do {
        do {
            rnd = frandmod(12);
        } while (used[rnd] != 0);
        used[rnd] = 1;
        do {
            kind = frandmod(5);
        } while (cnt[kind] >= maxCnt[kind]);
        cnt[kind]++;
        lbl_1_bss_16844[rnd].kind = kind;
        if (kind == 0) {
            FL.itemNo = rnd;
        }
        for (i = 0; i < 12; i++) {
            if (used[i] == 0) {
                i = 99;
            }
        }
    } while (i != 12);
    for (i = 0; i < 12; i++) {
        if (lbl_1_bss_16844[i].kind == 0) {
            FL.carryIdx = i;
        }
    }
    for (i = 0; i < 12; i++) {
        do {
            rnd = frandmod(12);
            found = 0;
            for (j = 0; j < 12; j++) {
                if (FL.owner[j] == rnd) {
                    found = 1;
                }
            }
        } while (found != 0);
        FL.owner[i] = rnd;
    }
    for (i = 0; i < 6; i++) {
        MDL(i).id = Hu3DModelCreate(HuDataSelHeapReadNum(state->dataNums[i], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(MDL(i).id, 1);
        Hu3DModelCameraSet(MDL(i).id, 1);
        Hu3DModelLayerSet(MDL(i).id, 2);
        MDL(i).x = 0.0f;
        MDL(i).y = 0.0f;
        MDL(i).z = 0.0f;
        MDL(i).rotX = 0.0f;
        MDL(i).rotY = 0.0f;
        MDL(i).rotZ = 0.0f;
        if (i == 0) {
            Hu3DModelShadowMapSet(MDL(i).id);
        } else {
            if (i == 4) {
                Hu3DMotionSpeedSet(MDL(i).id, 0.5f);
            } else {
                Hu3DMotionSpeedSet(MDL(i).id, 1.0f);
            }
            Hu3DMotionTimeSet(MDL(i).id, 0.0f);
            Hu3DModelAttrSet(MDL(i).id, 0x40000001);
        }
        Hu3DModelAttrReset(MDL(i).id, 1);
        Hu3DModelAttrSet(MDL(i).id, 0x40000001);
    }
    MDL(80).id = Hu3DModelCreate(HuDataSelHeapReadNum(state->dataNums[24], HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(MDL(80).id, 1);
    Hu3DModelCameraSet(MDL(80).id, 1);
    MDL(80).x = 0.0f;
    MDL(80).y = 0.0f;
    MDL(80).z = 0.0f;
    MDL(80).rotX = 0.0f;
    MDL(80).rotY = 0.0f;
    MDL(80).rotZ = 0.0f;
    ids[0] = 6;
    ids[1] = 7;
    ids[2] = 8;
    ids[3] = 0x17;
    j = 0;
    for (i = 6; i <= 0x35; i++, j++) {
        if (j >= 4) {
            j = 0;
        }
        pos.x = origin.x + 20.0 * cos(3.141592653589793 * (30.0f * ((i - 6) / 4)) / 180.0);
        pos.y = 0.1f + origin.y;
        pos.z = origin.z + 20.0 * sin(3.141592653589793 * (30.0f * ((i - 6) / 4)) / 180.0);
        MDL(i).id = Hu3DModelCreate(HuDataSelHeapReadNum(state->dataNums[ids[j]], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(MDL(i).id, 1);
        Hu3DModelCameraSet(MDL(i).id, 1);
        if (j != 0) {
            Hu3DModelLayerSet(MDL(i).id, 0);
        } else {
            Hu3DModelLayerSet(MDL(i).id, 3);
        }
        MDL(i).x = pos.x;
        MDL(i).y = pos.y;
        MDL(i).z = pos.z;
        MDL(i).rotX = 0.0f;
        MDL(i).rotY = 0.0f;
        MDL(i).rotZ = 0.0f;
        if (ids[j] == 7 || ids[j] == 7) {
            Hu3DModelShadowMapSet(MDL(i).id);
        }
        if (i == 0x2B) {
            MDL(i).x = pos.x;
            MDL(i).y = pos.y - 26.0f;
            MDL(i).z = pos.z;
        }
    }
    for (i = 0; i < 12; i++) {
        for (j = 0; j <= 1; j++) {
            Hu3DModelAttrReset(MDL(lbl_1_bss_179A0[i].mdl[j]).id, 1);
        }
    }
    for (i = 0x36; i <= 0x37; i++) {
        MDL(i).id = Hu3DModelCreate(HuDataSelHeapReadNum(state->dataNums[10], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(MDL(i).id, 1);
        Hu3DModelCameraSet(MDL(i).id, 1);
        Hu3DModelLayerSet(MDL(i).id, 3);
        MDL(i).x = 0.0f;
        MDL(i).y = 0.0f;
        MDL(i).z = 0.0f;
        MDL(i).rotX = 0.0f;
        MDL(i).rotY = 0.0f;
        MDL(i).rotZ = 0.0f;
        Hu3DModelShadowSet(MDL(i).id);
        Hu3DModelShadowDispOn(MDL(i).id);
    }
    for (i = 0x38; i <= 0x39; i++) {
        MDL(i).id = Hu3DModelCreate(HuDataSelHeapReadNum(state->dataNums[9], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(MDL(i).id, 1);
        Hu3DModelCameraSet(MDL(i).id, 1);
        Hu3DModelLayerSet(MDL(i).id, 0);
        Hu3DMotionSpeedSet(MDL(i).id, 0.0f);
        Hu3DModelShadowMapSet(MDL(i).id);
        MDL(i).x = 0.0f;
        MDL(i).y = 0.0f;
        MDL(i).z = 0.0f;
        MDL(i).rotX = 0.0f;
        MDL(i).rotY = 0.0f;
        MDL(i).rotZ = 0.0f;
    }
    j = 0xB;
    for (i = 0x3A; i <= 0x3D; i++, j++) {
        if (i == 0x3C) {
            j = 0xB;
        }
        MDL(i).id = Hu3DModelCreate(HuDataSelHeapReadNum(state->dataNums[j], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(MDL(i).id, 1);
        Hu3DModelCameraSet(MDL(i).id, 1);
        Hu3DModelLayerSet(MDL(i).id, 3);
        Hu3DMotionSpeedSet(MDL(i).id, 0.0f);
        MDL(i).x = 0.0f;
        MDL(i).y = 0.0f;
        MDL(i).z = 0.0f;
        MDL(i).rotX = 0.0f;
        MDL(i).rotY = 0.0f;
        MDL(i).rotZ = 0.0f;
    }
    ids[0] = 0x10;
    ids[1] = 0x10;
    ids[2] = 0xD;
    ids[3] = 0xE;
    ids[4] = 0xF;
    ids[5] = 0x10;
    ids[6] = 0x10;
    ids[7] = 0xD;
    ids[8] = 0xE;
    ids[9] = 0xF;
    j = 0;
    for (i = 0x3E; i <= 0x47; i++, j++) {
        MDL(i).id = Hu3DModelCreate(HuDataSelHeapReadNum(state->dataNums[ids[j]], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(MDL(i).id, 1);
        if (j == 0x10) {
            Hu3DModelAttrSet(MDL(i).id, 0x40000001);
            Hu3DModelLayerSet(MDL(i).id, 3);
        } else {
            Hu3DModelLayerSet(MDL(i).id, 3);
        }
        Hu3DModelCameraSet(MDL(i).id, 1);
        Hu3DMotionSpeedSet(MDL(i).id, 0.0f);
        MDL(i).x = 0.0f;
        MDL(i).y = 0.0f;
        MDL(i).z = 0.0f;
        MDL(i).rotX = 0.0f;
        MDL(i).rotY = 0.0f;
        MDL(i).rotZ = 0.0f;
    }
    j = 0x11;
    for (i = 0x48; i <= 0x4B; i++, j++) {
        MDL(i).id = Hu3DModelCreate(HuDataSelHeapReadNum(state->dataNums[j], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(MDL(i).id, 1);
    }
    j = 0;
    for (i = 0x4C; i <= 0x4E; i++, j++) {
        MDL(i).id = Hu3DModelCreate(HuDataSelHeapReadNum(state->dataNums[21], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(MDL(i).id, 1);
        Hu3DModelCameraSet(MDL(i).id, 1);
        Hu3DModelLayerSet(MDL(i).id, 3);
        Hu3DMotionSpeedSet(MDL(i).id, 0.0f);
        Hu3DMotionTimeSet(MDL(i).id, 30.0f * j);
        Hu3DModelAttrSet(MDL(i).id, 0x40000001);
        Hu3DModelTPLvlSet(MDL(i).id, FL.fadeTp);
        MDL(i).x = 0.0f;
        MDL(i).y = 0.0f;
        MDL(i).z = 0.0f;
        MDL(i).rotX = 0.0f;
        MDL(i).rotY = 0.0f;
        MDL(i).rotZ = 0.0f;
    }
    MDL(79).id = Hu3DModelCreate(HuDataSelHeapReadNum(state->dataNums[22], HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(MDL(79).id, 1);
    Hu3DModelCameraSet(MDL(79).id, 1);
    Hu3DModelLayerSet(MDL(79).id, 3);
    Hu3DModelTPLvlSet(MDL(79).id, FL.fadeTp);
    MDL(79).x = 0.0f;
    MDL(79).y = 0.0f;
    MDL(79).z = 0.0f;
    MDL(79).rotX = 0.0f;
    MDL(79).rotY = 0.0f;
    MDL(79).rotZ = 0.0f;
    j = 0;
    for (i = 0; i <= 0xB; i++, j++) {
        lbl_1_bss_0.colMdl[i] = MDL(lbl_1_bss_179A0[j].mdl[3]).id;
    }
    lbl_1_bss_0.colMdl[0xC] = MDL(80).id;
    MgActorColMapInit(lbl_1_bss_0.colMdl, 0xD, 5);
    param.height = 150.0f;
    param.radius = 35.0f;
    param.param = 0;
    param.type = 0;
    param.attr = 0x8000;
    param.narrowHook = fn_1_117B0;
    param.correctHookParam = 0;
    param.correctHook = fn_1_117CC;
    lbl_1_bss_0.player[0] = MgPlayerCreate(state->chars[0].playerNo, &param, 4, 1, -1, NULL);
    lbl_1_bss_0.player[0]->actor->pos.x = -200.0f;
    lbl_1_bss_0.player[0]->actor->pos.y = 140.0f;
    lbl_1_bss_0.player[0]->actor->pos.z = 0.0f;
    Hu3DModelShadowSet(lbl_1_bss_0.player[0]->actor->mdlId);
    Hu3DModelShadowDispOn(lbl_1_bss_0.player[0]->actor->mdlId);
    MDL(81).id = lbl_1_bss_0.player[0]->actor->mdlId;
    param.param = 1;
    lbl_1_bss_0.player[1] = MgPlayerCreate(state->chars[1].playerNo, &param, 4, 1, -1, NULL);
    lbl_1_bss_0.player[1]->actor->pos.x = 200.0f;
    lbl_1_bss_0.player[1]->actor->pos.y = 140.0f;
    lbl_1_bss_0.player[1]->actor->pos.z = 0.0f;
    Hu3DModelShadowSet(lbl_1_bss_0.player[1]->actor->mdlId);
    Hu3DModelShadowDispOn(lbl_1_bss_0.player[1]->actor->mdlId);
    MDL(82).id = lbl_1_bss_0.player[1]->actor->mdlId;
    Hu3DModelLayerSet(MDL(MDL(81).id).id, 2);
    Hu3DModelLayerSet(MDL(MDL(82).id).id, 2);
    CharModelStepFxSet(state->chars[0].charNo, 0);
    CharModelStepFxSet(state->chars[1].charNo, 0);
    CharEffectLayerSet(5);
    fn_1_11D34(0);
    fn_1_11D34(1);
    PW[0].height = CharModelHeightGet(lbl_1_bss_0.player[0]->charNo);
    PW[1].height = CharModelHeightGet(lbl_1_bss_0.player[1]->charNo);
    PW[0].itemHookName = CharModelItemHookGet(lbl_1_bss_0.player[0]->charNo, 4, 4);
    PW[1].itemHookName = CharModelItemHookGet(lbl_1_bss_0.player[1]->charNo, 4, 4);
    PW[0].gravity = lbl_1_bss_0.player[0]->actor->gravity;
    PW[1].gravity = lbl_1_bss_0.player[1]->actor->gravity;
    fn_1_170B0();
    fn_1_5860(0);
    fn_1_5860(1);
    fn_1_2354();
    fn_1_9360();
    FL.timerObj = fn_1_1A44(0, 0x3C, 0, 292.0f, 64.0f);
}

s32 fn_1_805C(M551State *state)
{
    HuVecF pos;
    s32 ret = 1;
    s32 i;

    fn_1_5FF4(state);
    for (i = 0; i < 10; i++) {
        MgPlayerPadSet(lbl_1_bss_0.player[0], 0, 0, 0, 0);
        MgPlayerPadSet(lbl_1_bss_0.player[1], 0, 0, 0, 0);
        pos.x = -200.0f;
        pos.y = 110.0f;
        pos.z = 0.0f;
        MgActorPosUpdate(lbl_1_bss_0.player[0]->actor, &pos);
        pos.x = 200.0f;
        pos.y = 110.0f;
        pos.z = 0.0f;
        MgActorPosUpdate(lbl_1_bss_0.player[1]->actor, &pos);
        MgActorExec();
    }
    MgActorPosGet(lbl_1_bss_0.player[0]->actor, &pos);
    PW[0].pos = pos;
    MgActorPosGet(lbl_1_bss_0.player[1]->actor, &pos);
    PW[1].pos = pos;
    return ret;
}

s32 fn_1_81F4(void)
{
    s32 ret = 0;

    ret = (s16)fn_1_EA8C();
    fn_1_9360();
    return ret;
}

s32 fn_1_822C(void)
{
    s32 ret = 0;

    FL.mode = 0;
    FL.subMode = 0;
    FL.pad0 = 0;
    fn_1_9360();
    return ret;
}

s32 fn_1_828C(void)
{
    HuVecF pos;
    s16 i;
    s32 ret;
    s16 allDone;

    ret = 0;
    allDone = 1;
    switch (FL.mode) {
    case 0:
        fn_1_1AB8(FL.timerObj);
        FL.mode++;
        break;
    case 1:
        for (i = 0; i < 2; i++) {
            if (PW[i].comOn != 0) {
                fn_1_11F4C(i);
            }
            allDone *= fn_1_9654(i);
        }
        if (fn_1_1AE4(FL.timerObj) != 0) {
            FL.pad54 = 1;
        }
        if (allDone != 0) {
            PW[0].canDrop = 1;
            PW[1].canDrop = 1;
            FL.mode++;
        }
        break;
    case 2:
        if (FL.cpuPlayer == 0) {
            fn_1_5C24(1, 100);
        }
        FL.timer = 0;
        FL.mode++;
        break;
    case 3:
        if (FL.timer++ > 30) {
            if (FL.cpuPlayer == 0) {
                fn_1_1B0C(FL.timerObj);
            }
            Hu3DModelAttrReset(MDL(81).id, 1);
            Hu3DModelAttrReset(MDL(82).id, 1);
            if (FL.cpuPlayer != 0) {
                s16 playerNo;

                MgSeqWinnerSet(lbl_1_bss_17A00->chars[FL.cpuPlayer & 1].charNo, -1, -1, -1);
                playerNo = lbl_1_bss_17A00->chars[FL.cpuPlayer & 1].playerNo;
                if (_CheckFlag(0x1000F) == 0) {
                    GwPlayer[playerNo].mgCoinBonus = 10;
                }
            } else {
                MgSeqWinnerSet(-1, -1, -1, -1);
            }
            ret = 1;
        }
        break;
    }
    fn_1_9360();
    for (i = 0; i < 2; i++) {
        if (PW[i].comOn != 0) {
            MgPlayerPadSet(lbl_1_bss_0.player[i], PW[i].stkX, PW[i].stkY, PW[i].btnDown, PW[i].btn);
        }
        if (PW[i].canDrop != 0) {
            MgPlayerPadSet(lbl_1_bss_0.player[i], 0, 0, 0, 0);
        }
    }
    fn_1_9934();
    MgActorPosGet(lbl_1_bss_0.player[0]->actor, &pos);
    PW[0].oldPos.x = pos.x;
    PW[0].oldPos.y = pos.y;
    PW[0].oldPos.z = pos.z;
    MgActorPosGet(lbl_1_bss_0.player[1]->actor, &pos);
    PW[1].oldPos.x = pos.x;
    PW[1].oldPos.y = pos.y;
    PW[1].oldPos.z = pos.z;
    MgActorExec();
    fn_1_115B0();
    for (i = 0; i < 2; i++) {
        if (PW[i].pad8 != 0 || PW[i].itemNo != 0) {
            MgActorPosGet(lbl_1_bss_0.player[i]->actor, &pos);
            pos.x = PW[i].oldPos.x;
            pos.z = PW[i].oldPos.z;
            MgActorPosUpdate(lbl_1_bss_0.player[i]->actor, &pos);
        }
    }
    MgActorPosGet(lbl_1_bss_0.player[0]->actor, &pos);
    PW[0].pos.x = pos.x;
    PW[0].pos.y = pos.y;
    PW[0].pos.z = pos.z;
    MgActorPosGet(lbl_1_bss_0.player[1]->actor, &pos);
    PW[1].pos.x = pos.x;
    PW[1].pos.y = pos.y;
    PW[1].pos.z = pos.z;
    fn_1_153A8(PW);
    pos.x = PW[0].pos.x;
    pos.y = PW[0].pos.y;
    pos.z = PW[0].pos.z;
    MgActorPosUpdate(lbl_1_bss_0.player[0]->actor, &pos);
    pos.x = PW[1].pos.x;
    pos.y = PW[1].pos.y;
    pos.z = PW[1].pos.z;
    MgActorPosUpdate(lbl_1_bss_0.player[1]->actor, &pos);
    for (i = 0; i < 2; i++) {
        Hu3DModelPosSet(MDL(i + 0x51).id, PW[i].pos.x, PW[i].pos.y, PW[i].pos.z);
    }
    return ret;
}

s32 fn_1_89B0(void)
{
    HuVecF pos;
    s32 i;

    FL.mode = 0;
    FL.subMode = 0;
    FL.pad0 = 0;
    FL.timer = 0;
    if (FL.cpuPlayer != 0) {
        fn_1_A758(FL.cpuPlayer & 1);
    }
    MgActorPosGet(lbl_1_bss_0.player[0]->actor, &pos);
    PW[0].oldPos.x = pos.x;
    PW[0].oldPos.y = pos.y;
    PW[0].oldPos.z = pos.z;
    MgActorPosGet(lbl_1_bss_0.player[1]->actor, &pos);
    PW[1].oldPos.x = pos.x;
    PW[1].oldPos.y = pos.y;
    PW[1].oldPos.z = pos.z;
    MgPlayerPadSet(lbl_1_bss_0.player[0], 0, 0, 0, 0);
    MgPlayerPadSet(lbl_1_bss_0.player[1], 0, 0, 0, 0);
    MgActorExec();
    MgActorPosGet(lbl_1_bss_0.player[0]->actor, &pos);
    PW[0].pos.x = pos.x;
    PW[0].pos.y = pos.y;
    PW[0].pos.z = pos.z;
    MgActorPosGet(lbl_1_bss_0.player[1]->actor, &pos);
    PW[1].pos.x = pos.x;
    PW[1].pos.y = pos.y;
    PW[1].pos.z = pos.z;
    fn_1_153A8(PW);
    pos.x = PW[0].pos.x;
    pos.y = PW[0].pos.y;
    pos.z = PW[0].pos.z;
    MgActorPosUpdate(lbl_1_bss_0.player[0]->actor, &pos);
    pos.x = PW[1].pos.x;
    pos.y = PW[1].pos.y;
    pos.z = PW[1].pos.z;
    MgActorPosUpdate(lbl_1_bss_0.player[1]->actor, &pos);
    for (i = 0; i < 2; i++) {
        Hu3DModelPosSet(MDL(i + 0x51).id, PW[i].pos.x, PW[i].pos.y, PW[i].pos.z);
    }
    fn_1_9360();
    return 0;
}

s32 fn_1_8CBC(void)
{
    s32 ret = 0;

    if (FL.cpuPlayer != 0) {
        fn_1_A758(FL.cpuPlayer & 1);
    }
    ret = (s16)fn_1_F814();
    fn_1_9360();
    return ret;
}

s32 fn_1_8D20(void)
{
    HU3DMODEL *model;
    M551StageSlot *slot;

    model = &Hu3DData[MDL((FL.cpuPlayer & 1) + 0x51).id];
    if (lbl_1_bss_17A00->field_178 == 0) {
        if (FL.cpuPlayer != 0) {
            slot = &lbl_1_bss_17A00->slots[FL.cpuPlayer & 1];
            slot->motion = 9;
            slot->speed = 1.0f;
            slot->time = 0.0f;
            slot->attrSet = 0;
            slot->attrReset = 0x40000001;
            slot = &lbl_1_bss_17A00->slots[(FL.cpuPlayer & 2) >> 1];
            slot->motion = 10;
            slot->speed = 1.0f;
            slot->time = 0.0f;
            slot->attrSet = 0;
            slot->attrReset = 0x40000001;
        } else {
            lbl_1_bss_17A00->slots[0].motion = 10;
            lbl_1_bss_17A00->slots[0].speed = 1.0f;
            lbl_1_bss_17A00->slots[0].time = 0.0f;
            lbl_1_bss_17A00->slots[0].attrSet = 0;
            lbl_1_bss_17A00->slots[0].attrReset = 0x40000001;
            lbl_1_bss_17A00->slots[1].motion = 10;
            lbl_1_bss_17A00->slots[1].speed = 1.0f;
            lbl_1_bss_17A00->slots[1].time = 0.0f;
            lbl_1_bss_17A00->slots[1].attrSet = 0;
            lbl_1_bss_17A00->slots[1].attrReset = 0x40000001;
            CharLoseVoicePlay(lbl_1_bss_17A00->chars[0].charNo, lbl_1_bss_17A00->chars[1].charNo, -1, -1);
        }
        lbl_1_bss_17A00->field_178 = 1;
        FL.camY = model->rot.y;
    }
    fn_1_9360();
    if (FL.cpuPlayer != 0) {
        CharModelVoiceVolSet(lbl_1_bss_17A00->chars[FL.cpuPlayer & 1].charNo, 0x7F);
        CharModelVoicePanSet(lbl_1_bss_17A00->chars[FL.cpuPlayer & 1].charNo, 0x40);
    }
    return 0;
}

s16 fn_1_8FF4(void)
{
    s32 ret = 1;

    fn_1_9360();
    return ret;
}

s32 fn_1_9024(void)
{
    s32 ret = 1;

    fn_1_5FD4();
    MgActorKill(lbl_1_bss_0.player[0]->actor);
    MgActorKill(lbl_1_bss_0.player[1]->actor);
    if (lbl_1_bss_1679C.active != 0) {
        fn_1_43CC();
    }
    return ret;
}

void fn_1_9094(void)
{
    HU3DMODEL *model0;
    HU3DMODEL *model1;
    s16 pan;
    s16 i;

    model0 = &Hu3DData[MDL(81).id];
    model1 = &Hu3DData[MDL(82).id];
    for (i = 0; i < 0x51; i++) {
        Hu3DModelPosSet(MDL(i).id, MDL(i).x, MDL(i).y, MDL(i).z);
        Hu3DModelRotSet(MDL(i).id, MDL(i).rotX, MDL(i).rotY, MDL(i).rotZ);
    }
    pan = 64.0f + 0.055652175f * model0->pos.x;
    CharModelVoiceVolSet(lbl_1_bss_17A00->chars[0].charNo, 111.0f + 0.026956521f * model0->pos.z);
    CharModelVoicePanSet(lbl_1_bss_17A00->chars[0].charNo, pan);
    pan = 64.0f + 0.055652175f * model1->pos.x;
    CharModelVoiceVolSet(lbl_1_bss_17A00->chars[1].charNo, 111.0f + 0.026956521f * model1->pos.z);
    CharModelVoicePanSet(lbl_1_bss_17A00->chars[1].charNo, pan);
}

void fn_1_9360(void)
{
    HU3DMODEL *model0;
    HU3DMODEL *model1;
    s16 pan;
    s16 i;

    model0 = &Hu3DData[MDL(81).id];
    model1 = &Hu3DData[MDL(82).id];
    for (i = 0; i < 0x51; i++) {
        Hu3DModelPosSet(MDL(i).id, MDL(i).x, MDL(i).y, MDL(i).z);
        Hu3DModelRotSet(MDL(i).id, MDL(i).rotX, MDL(i).rotY, MDL(i).rotZ);
    }
    pan = 64.0f + 0.055652175f * model0->pos.x;
    CharModelVoiceVolSet(lbl_1_bss_17A00->chars[0].charNo, 111.0f + 0.026956521f * model0->pos.z);
    CharModelVoicePanSet(lbl_1_bss_17A00->chars[0].charNo, pan);
    pan = 64.0f + 0.055652175f * model1->pos.x;
    CharModelVoiceVolSet(lbl_1_bss_17A00->chars[1].charNo, 111.0f + 0.026956521f * model1->pos.z);
    CharModelVoicePanSet(lbl_1_bss_17A00->chars[1].charNo, pan);
    if (lbl_1_bss_1679C.active != 0) {
        fn_1_4CB0();
    }
    fn_1_91C(0);
    fn_1_10C4();
    fn_1_14C4();
}

s32 fn_1_9654(s16 arg0)
{
    s32 ret;

    ret = 0;
    switch (PW[arg0].mode) {
    case 0:
        if (PW[arg0].itemNo != 0) {
            if (lbl_1_bss_16844[PW[arg0].itemIdx].flag != 0) {
                PW[arg0].mode++;
            }
        } else if (FL.cpuPlayer != 0 || FL.pad54 != 0) {
            PW[arg0].canDrop = 1;
            PW[arg0].mode = 2;
        }
        break;
    case 1:
        if (fn_1_A078(arg0, lbl_1_bss_16844[PW[arg0].itemIdx].kind) != 0) {
            if (lbl_1_bss_16844[PW[arg0].itemIdx].kind != 0) {
                lbl_1_bss_16844[PW[arg0].itemIdx].state = 0;
                lbl_1_bss_16844[PW[arg0].itemIdx].flag = 0;
                PW[arg0].itemNo = 0;
                PW[arg0].itemIdx = -1;
            }
            PW[arg0].mode = 0;
            if (FL.cpuPlayer != 0 || FL.pad54 != 0) {
                PW[arg0].canDrop = 1;
                PW[arg0].mode = 2;
            }
        }
        break;
    case 2:
        ret = 1;
        break;
    }
    return ret;
}
