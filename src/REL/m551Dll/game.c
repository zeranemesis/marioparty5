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
void fn_1_153A8();
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

#define VOL(z) (111.0f + 0.026956521f * (z))
#define PAN(x) (64.0f + 0.055652175f * (x))

void fn_1_9934(void)
{
    f32 dist;
    s16 i;

    for (i = 0; i < 12; i++) {
        if (lbl_1_bss_16844[i].state != lbl_1_bss_16844[i].oldState) {
            switch (lbl_1_bss_16844[i].state) {
            case 0:
                lbl_1_bss_16844[i].baseY = 100.1f;
                lbl_1_bss_16844[i].speed = 1.8f;
                break;
            case 1:
                lbl_1_bss_16844[i].baseY = 96.5f;
                if (MDL(lbl_1_bss_179A0[i].mdl[1]).y >= 96.5f) {
                    lbl_1_bss_16844[i].speed = -1.8f;
                } else {
                    lbl_1_bss_16844[i].speed = 1.8f;
                }
                break;
            case 2:
                lbl_1_bss_16844[i].baseY = 82.1f;
                lbl_1_bss_16844[i].speed = -1.8f;
                Hu3DModelAttrReset(MDL(lbl_1_bss_179A0[i].mdl[2]).id, 1);
                Hu3DModelAttrSet(MDL(lbl_1_bss_179A0[i].mdl[2]).id, 0x40000001);
                Hu3DMotionSpeedSet(MDL(lbl_1_bss_179A0[i].mdl[2]).id, 1.5f);
                Hu3DMotionTimeSet(MDL(lbl_1_bss_179A0[i].mdl[2]).id, 0.0f);
                break;
            }
            lbl_1_bss_16844[i].timer = 1;
            lbl_1_bss_16844[i].oldState = lbl_1_bss_16844[i].state;
            lbl_1_bss_16844[i].pad0 = 0;
        }
        if (lbl_1_bss_16844[i].timer != 0) {
            if ((MDL(lbl_1_bss_179A0[i].mdl[1]).y - lbl_1_bss_16844[i].baseY) > 0.0f) {
                dist = MDL(lbl_1_bss_179A0[i].mdl[1]).y - lbl_1_bss_16844[i].baseY;
            } else {
                dist = -1.0f * (MDL(lbl_1_bss_179A0[i].mdl[1]).y - lbl_1_bss_16844[i].baseY);
            }
            MDL(lbl_1_bss_179A0[i].mdl[1]).y += lbl_1_bss_16844[i].speed;
            MDL(lbl_1_bss_179A0[i].mdl[2]).y += lbl_1_bss_16844[i].speed;
            MDL(lbl_1_bss_179A0[i].mdl[3]).y += lbl_1_bss_16844[i].speed;
            if (dist <= 2.0f) {
                MDL(lbl_1_bss_179A0[i].mdl[1]).y = lbl_1_bss_16844[i].baseY;
                MDL(lbl_1_bss_179A0[i].mdl[2]).y = lbl_1_bss_16844[i].baseY;
                MDL(lbl_1_bss_179A0[i].mdl[3]).y = lbl_1_bss_16844[i].baseY;
                lbl_1_bss_16844[i].timer = 0;
                lbl_1_bss_16844[i].flag = 1;
            }
        }
    }
    for (i = 0; i < 12; i++) {
        if (lbl_1_bss_16844[i].state == 1) {
            lbl_1_bss_16844[i].state = 0;
        }
    }
}

s16 fn_1_A078(s16 arg0, s16 arg1)
{
    s16 tbl[2][5] = { { 0, 0x36, 0, 0x3A, 0x3E }, { 0, 0x37, 0, 0x3C, 0x43 } };
    s16 ret;

    ret = 0;
    switch (PW[arg0].phase) {
    case 0:
        PW[arg0].canDrop = 1;
        PW[arg0].owner[PW[arg0].itemIdx]++;
        if (arg1 == 0) {
            fn_1_5D64(7, VOL(MDL(arg0 + 0x51).z), PAN(MDL(arg0 + 0x51).x));
        }
        PW[arg0].phase++;
        break;
    case 1:
        if (lbl_1_bss_0.player[arg0]->mode == 0) {
            if (arg1 == 0) {
                fn_1_5C24(1, 1000);
            }
            PW[arg0].phase++;
        }
        break;
    case 2:
        lbl_1_bss_0.player[arg0]->actor->gravity = 0.0f;
        PW[arg0].phase++;
        break;
    case 3:
        if (PW[arg0].timer++ > 20) {
            if (arg1 != 0) {
                Hu3DModelAttrSet(MDL(lbl_1_bss_179A0[PW[arg0].itemIdx].mdl[2]).id, 1);
            }
            PW[arg0].timer = 0;
            PW[arg0].phase++;
        }
        if (arg1 == 0) {
            PW[arg0].timer = 0;
            PW[arg0].phase++;
        }
        break;
    case 4:
        if (lbl_1_data_5B0[arg1](arg0, tbl[arg0][arg1]) != 0) {
            if (arg1 != 0) {
                lbl_1_bss_0.player[arg0]->actor->gravity = PW[arg0].gravity;
                MgPlayerAttrReset(lbl_1_bss_0.player[arg0], 4);
                MgPlayerAttrReset(lbl_1_bss_0.player[arg0 ^ 1], 2);
            }
            PW[arg0].phase++;
        }
        break;
    case 5:
        PW[arg0].canDrop = 0;
        PW[arg0].phase = 0;
        ret = 1;
        break;
    }
    return ret;
}

s16 fn_1_A5A4(s16 arg0, s16 arg1)
{
    s16 ret;

    ret = 0;
    switch (PW[arg0].step) {
    case 0: {
        HuVecF tmp;
        HuVecF pos;

        tmp.x = MDL(lbl_1_bss_179A0[FL.carryIdx].mdl[0]).x;
        tmp.y = 1000.0f;
        tmp.z = MDL(lbl_1_bss_179A0[FL.carryIdx].mdl[0]).z;
        pos = tmp;
        fn_1_41C8(lbl_1_bss_17A00, 0x48, &pos);
        PW[arg0].step++;
        break;
    }
    case 1:
        PW[arg0].timer = 0;
        PW[arg0].step = 0;
        FL.cpuPlayer = arg0 | ((arg0 ^ 1) * 2);
        ret = 1;
        break;
    }
    return ret;
}

s32 fn_1_A758(s16 arg0)
{
    HuVecF pos;
    f32 step;
    s16 dec;
    s16 cnt;
    s16 i;

    cnt = 0;
    switch (PW[arg0].subPhase) {
    case 0:
        pos.x = MDL(lbl_1_bss_179A0[FL.carryIdx].mdl[0]).x;
        pos.y = 0.0f;
        pos.z = MDL(lbl_1_bss_179A0[FL.carryIdx].mdl[0]).z;
        for (i = 0x4C; i <= 0x4E; i++) {
            Hu3DModelAttrReset(MDL(i).id, 1);
            Hu3DMotionSpeedSet(MDL(i).id, 1.0f);
            MDL(i).x = pos.x;
            MDL(i).y = pos.y;
            MDL(i).z = pos.z;
        }
        Hu3DModelAttrReset(MDL(79).id, 1);
        MDL(79).x = pos.x;
        MDL(79).y = pos.y;
        MDL(79).z = pos.z;
        fn_1_1B0C(FL.timerObj);
        FL.ambLevel = 1.0f;
        PW[arg0].subPhase++;
        /* fallthrough */
    case 1:
        dec = 200 / 60;
        lbl_1_bss_34->color.r -= dec;
        if (lbl_1_bss_34->color.r <= 0x37) {
            lbl_1_bss_34->color.r = 0x37;
            cnt = 1;
        }
        lbl_1_bss_34->color.g -= dec;
        if (lbl_1_bss_34->color.g <= 0x37) {
            lbl_1_bss_34->color.g = 0x37;
            cnt += 1;
        }
        lbl_1_bss_34->color.b -= dec;
        if (lbl_1_bss_34->color.b <= 0x37) {
            lbl_1_bss_34->color.b = 0x37;
            cnt += 1;
        }
        lbl_1_bss_34->color.a -= dec;
        if (lbl_1_bss_34->color.a <= 0x37) {
            lbl_1_bss_34->color.a = 0x37;
            cnt += 1;
        }
        step = 0.3f / 60;
        FL.ambLevel += 2.0f * step;
        FL.ambScale -= step;
        if (FL.ambScale <= 0.7f) {
            FL.ambLevel -= 2.0f * step;
            FL.ambScale = 0.7f;
            cnt += 1;
        }
        Hu3DModelAmbSet(MDL(arg0 + 0x51).id, FL.ambLevel, FL.ambLevel, FL.ambLevel);
        Hu3DAmbColorSet(FL.ambScale, FL.ambScale, FL.ambScale);
        FL.fadeTp += 1.0f / 120;
        if (FL.fadeTp >= 1.0f) {
            FL.fadeTp = 1.0f;
            cnt += 1;
        }
        Hu3DModelTPLvlSet(MDL(76).id, FL.fadeTp);
        Hu3DModelTPLvlSet(MDL(77).id, FL.fadeTp);
        Hu3DModelTPLvlSet(MDL(78).id, FL.fadeTp);
        Hu3DModelTPLvlSet(MDL(79).id, FL.fadeTp);
        if (cnt >= 6) {
            PW[arg0].subPhase++;
        }
        break;
    case 2:
        PW[arg0].subPhase++;
        break;
    }
    return 0;
}

s16 fn_1_ADA4(s16 arg0, s16 arg1)
{
    HU3DMODEL *model;
    M551StageSlot *slot;
    s16 ret;

    model = &Hu3DData[MDL(arg0 + 0x51).id];
    slot = &lbl_1_bss_17A00->slots[arg0];
    ret = 0;
    if ((u32)PW[arg0].step <= 7) {
        switch (PW[arg0].step) {
        case 0:
            MDL(arg1).x = lbl_1_bss_0.player[arg0]->actor->pos.x;
            MDL(arg1).y = 1200.0f;
            MDL(arg1).z = lbl_1_bss_0.player[arg0]->actor->pos.z;
            PW[arg0].unk30 = 1.0f;
            Hu3DModelTPLvlSet(MDL(arg1).id, PW[arg0].unk30);
            Hu3DModelAttrReset(MDL(arg1).id, 1);
            slot->motion = 6;
            slot->speed = 1.0f;
            slot->time = 0.0f;
            slot->attrSet = 0;
            slot->attrReset = 0x40000001;
            PW[arg0].unk2C = -40.0f;
            PW[arg0].step++;
            break;
        case 1:
            if (slot->endFlag != 0) {
                fn_1_5D64(3, VOL(MDL(arg1).z), PAN(MDL(arg1).x));
                PW[arg0].step++;
            }
            break;
        case 2:
            MDL(arg1).y += PW[arg0].unk2C;
            if (MDL(arg1).y <= 100.0f) {
                MDL(arg1).y = 100.0f;
                fn_1_29DC((M551Vec3 *)&MDL(arg1).x);
                model->pos.y += 1.0f;
                slot->motion = 0xB;
                slot->speed = 0.0f;
                slot->time = 10.0f;
                slot->attrSet = 0;
                slot->attrReset = 0x40000001;
                omVibrate(lbl_1_bss_17A00->chars[arg0].playerNo, 0x14, 0xC, 0);
                PW[arg0].step++;
            }
            break;
        case 3:
            if (PW[arg0].timer++ > 30) {
                fn_1_3270((M551Vec3 *)&MDL(arg1).x);
                PW[arg0].timer = 0;
                PW[arg0].step++;
            }
            break;
        case 4:
            if (PW[arg0].timer++ > 5) {
                PW[arg0].timer = 0;
                PW[arg0].step++;
            }
            break;
        case 5:
            PW[arg0].unk30 -= 0.05f;
            if (PW[arg0].unk30 <= 0.0f) {
                Hu3DModelAttrSet(MDL(arg1).id, 1);
                PW[arg0].unk30 = 1.0f;
                PW[arg0].step++;
                slot->speed = 2.0f;
            }
            Hu3DModelTPLvlSet(MDL(arg1).id, PW[arg0].unk30);
            break;
        case 6:
            if (slot->endFlag != 0) {
                model->pos.y -= 1.0f;
                slot->shiftState = 1;
                slot->shiftMotion = 0;
                slot->shiftStart = 0.0f;
                slot->shiftEnd = 8.0f;
                slot->shiftAttr = 0;
                slot->attrReset = 0x40000001;
                PW[arg0].step++;
            }
            break;
        case 7:
            if (slot->shiftState == 3) {
                slot->motion = 0;
                slot->speed = 1.0f;
                slot->time = 0.0f;
                slot->attrSet = 0x40000001;
                slot->attrReset = 0;
                PW[arg0].step = 0;
                ret = 1;
            }
            break;
        }
    }
    return ret;
}

s16 fn_1_B560(s16 arg0, s16 arg1)
{
    HuVecF pos;
    M551StageSlot *slot;
    s16 ret;

    slot = &lbl_1_bss_17A00->slots[arg0];
    ret = 0;
    MgActorPosGet(lbl_1_bss_0.player[arg0]->actor, &pos);
    PW[arg0].pos.x = pos.x;
    PW[arg0].pos.y = pos.y;
    PW[arg0].pos.z = pos.z;
    switch (PW[arg0].step) {
    case 0:
        slot->shiftState = 1;
        slot->shiftMotion = 8;
        slot->shiftStart = 0.0f;
        slot->shiftEnd = 8.0f;
        slot->shiftAttr = 0x40000001;
        slot->attrReset = 0;
        Hu3DModelAttrReset(MDL(arg0 + 0x38).id, 1);
        Hu3DMotionSpeedSet(MDL(arg0 + 0x38).id, 1.0f);
        Hu3DMotionTimeSet(MDL(arg0 + 0x38).id, 0.0f);
        *(HuVecF *)&MDL(arg0 + 0x38).x = *(HuVecF *)&MDL(lbl_1_bss_179A0[PW[arg0].itemIdx].mdl[1]).x;
        Hu3DModelAttrSet(MDL(lbl_1_bss_179A0[PW[arg0].itemIdx].mdl[1]).id, 1);
        Hu3DModelAttrSet(MDL(lbl_1_bss_179A0[PW[arg0].itemIdx].mdl[2]).id, 1);
        PW[arg0].unk2C = 44.72f;
        omVibrate(lbl_1_bss_17A00->chars[arg0].playerNo, 0x14, 0xC, 0);
        fn_1_5D64(4, VOL(MDL(arg0 + 0x38).z), PAN(MDL(arg0 + 0x38).x));
        PW[arg0].step++;
        break;
    case 1:
        PW[arg0].pos.y += PW[arg0].unk2C;
        PW[arg0].unk2C -= 1.0f;
        if (PW[arg0].unk2C <= 0.0f) {
            PW[arg0].unk2C = 0.0f;
            PW[arg0].step++;
        }
        break;
    case 2:
        PW[arg0].pos.y += PW[arg0].unk2C;
        PW[arg0].unk2C -= 1.0f;
        if (PW[arg0].pos.y <= 125.0f) {
            slot->shiftState = 1;
            slot->shiftMotion = 5;
            slot->shiftStart = 0.0f;
            slot->shiftEnd = 8.0f;
            slot->shiftAttr = 0;
            slot->attrReset = 0x40000001;
            PW[arg0].pos.y = 125.0f;
            omVibrate(lbl_1_bss_17A00->chars[arg0].playerNo, 0x14, 0xC, 0);
            PW[arg0].step++;
        }
        break;
    case 3:
        if (slot->shiftState == 3) {
            slot->motion = 4;
            slot->speed = 1.0f;
            slot->time = 0.0f;
            slot->attrSet = 0;
            slot->attrReset = 0;
            PW[arg0].step++;
        }
        break;
    case 4:
        if (slot->endFlag != 0) {
            slot->motion = 0;
            slot->speed = 1.0f;
            slot->time = 0.0f;
            slot->attrSet = 0x40000001;
            slot->attrReset = 0;
            PW[arg0].step = 0;
            PW[arg0].timer = 0;
            Hu3DModelAttrSet(MDL(arg0 + 0x38).id, 1);
            Hu3DMotionSpeedSet(MDL(arg0 + 0x38).id, 0.0f);
            Hu3DMotionTimeSet(MDL(arg0 + 0x38).id, 0.0f);
            Hu3DModelAttrReset(MDL(lbl_1_bss_179A0[PW[arg0].itemIdx].mdl[1]).id, 1);
            Hu3DModelAttrSet(MDL(lbl_1_bss_179A0[PW[arg0].itemIdx].mdl[2]).id, 1);
            ret = 1;
        }
        break;
    }
    pos.x = PW[arg0].pos.x;
    pos.y = PW[arg0].pos.y;
    pos.z = PW[arg0].pos.z;
    MgActorPosUpdate(lbl_1_bss_0.player[arg0]->actor, &pos);
    return ret;
}

s16 fn_1_BE48(s16 arg0, s16 arg1)
{
    HuVecF pos;
    M551StageSlot *slot;
    s16 ret;

    slot = &lbl_1_bss_17A00->slots[arg0];
    ret = 0;
    switch (PW[arg0].step) {
    case 0:
        slot->motion = 7;
        slot->speed = 1.0f;
        slot->time = 0.0f;
        slot->attrSet = 0;
        slot->attrReset = 0x40000001;
        PW[arg0].step++;
        break;
    case 1:
        if (slot->time >= 10.0f) {
            Hu3DModelAttrReset(MDL(arg1).id, 1);
            Hu3DMotionSpeedSet(MDL(arg1).id, 4.0f);
            Hu3DModelAttrSet(MDL(arg1).id, 0x40000001);
            Hu3DMotionTimeSet(MDL(arg1).id, 0.0f);
            Hu3DModelObjPosGet(MDL(arg0 + 0x51).id, PW[arg0].itemHookName, &pos);
            MDL(arg1).x = pos.x;
            MDL(arg1).y = pos.y;
            MDL(arg1).z = pos.z - 50.0f;
            Hu3DModelAttrReset(MDL(arg1 + 1).id, 1);
            Hu3DMotionSpeedSet(MDL(arg1 + 1).id, 4.0f);
            Hu3DModelAttrSet(MDL(arg1 + 1).id, 0x40000001);
            Hu3DMotionTimeSet(MDL(arg1 + 1).id, 0.0f);
            MDL(arg1 + 1).x = pos.x;
            MDL(arg1 + 1).y = pos.y;
            MDL(arg1 + 1).z = pos.z - 20.0f;
            omVibrate(lbl_1_bss_17A00->chars[arg0].playerNo, 0x46, 0xC, 0);
            fn_1_5D64(5, VOL(MDL(arg0 + 0x38).z), PAN(MDL(arg0 + 0x38).x));
            PW[arg0].pad10 = 0;
            PW[arg0].step++;
        }
        break;
    case 2:
        Hu3DModelObjPosGet(MDL(arg0 + 0x51).id, PW[arg0].itemHookName, &pos);
        MDL(arg1).x = pos.x;
        MDL(arg1).y = pos.y;
        MDL(arg1).z = pos.z - 50.0f;
        MDL(arg1 + 1).x = pos.x;
        MDL(arg1 + 1).y = pos.y;
        MDL(arg1 + 1).z = pos.z - 50.0f;
        if (PW[arg0].timer++ == 0) {
            fn_1_5958(arg0, 0.25f);
        } else if (PW[arg0].timer > 3) {
            if (PW[arg0].timer <= 6) {
                fn_1_5AC8(arg0);
            } else {
                PW[arg0].pad10 = 0;
                PW[arg0].timer = 0;
            }
        }
        if (slot->time >= 80.0f) {
            Hu3DModelAttrSet(MDL(arg1).id, 1);
            Hu3DModelAttrSet(MDL(arg1 + 1).id, 1);
            fn_1_5AC8(arg0);
            PW[arg0].step++;
        }
        break;
    case 3:
        if (slot->endFlag != 0) {
            slot->motion = 0;
            slot->speed = 1.0f;
            slot->time = 0.0f;
            slot->attrSet = 0x40000001;
            slot->attrReset = 0;
            PW[arg0].timer = 0;
            PW[arg0].step = 0;
            ret = 1;
        }
        break;
    }
    return ret;
}

s16 fn_1_C680(s16 arg0, s16 arg1)
{
    M551StageSlot *slot;
    f32 t;
    s16 ret;

    slot = &lbl_1_bss_17A00->slots[arg0];
    ret = 0;
    switch (PW[arg0].step) {
    case 0:
        MDL(arg1 + 2).x = lbl_1_bss_0.player[arg0]->actor->pos.x;
        MDL(arg1 + 2).y = 25.0f + (100.0f + PW[arg0].height);
        MDL(arg1 + 2).z = lbl_1_bss_0.player[arg0]->actor->pos.z;
        MDL(arg1 + 3).x = lbl_1_bss_0.player[arg0]->actor->pos.x;
        MDL(arg1 + 3).y = 25.0f + (100.0f + PW[arg0].height);
        MDL(arg1 + 3).z = lbl_1_bss_0.player[arg0]->actor->pos.z;
        MDL(arg1 + 4).x = lbl_1_bss_0.player[arg0]->actor->pos.x;
        MDL(arg1 + 4).y = 25.0f + (100.0f + PW[arg0].height);
        MDL(arg1 + 4).z = lbl_1_bss_0.player[arg0]->actor->pos.z;
        MDL(arg1).x = lbl_1_bss_0.player[arg0]->actor->pos.x;
        MDL(arg1).y = 25.0f + (100.0f + PW[arg0].height);
        MDL(arg1).z = lbl_1_bss_0.player[arg0]->actor->pos.z;
        MDL(arg1 + 1).x = lbl_1_bss_0.player[arg0]->actor->pos.x;
        MDL(arg1 + 1).y = 25.0f + (100.0f + PW[arg0].height);
        MDL(arg1 + 1).z = lbl_1_bss_0.player[arg0]->actor->pos.z;
        Hu3DMotionSpeedSet(MDL(arg1 + 2).id, 2.5f);
        Hu3DMotionSpeedSet(MDL(arg1 + 3).id, 0.0f);
        Hu3DMotionSpeedSet(MDL(arg1 + 4).id, 0.0f);
        Hu3DMotionSpeedSet(MDL(arg1).id, 0.0f);
        Hu3DMotionSpeedSet(MDL(arg1 + 1).id, 0.0f);
        Hu3DMotionTimeSet(MDL(arg1 + 2).id, 0.0f);
        Hu3DMotionTimeSet(MDL(arg1 + 3).id, 0.0f);
        Hu3DMotionTimeSet(MDL(arg1 + 4).id, 0.0f);
        Hu3DMotionTimeSet(MDL(arg1).id, 0.0f);
        Hu3DMotionTimeSet(MDL(arg1 + 1).id, 75.0f);
        Hu3DModelAttrReset(MDL(arg1 + 2).id, 1);
        slot->motion = 6;
        slot->speed = 1.0f;
        slot->time = 0.0f;
        slot->attrSet = 0;
        slot->attrReset = 0x40000001;
        PW[arg0].step++;
        break;
    case 1:
        if (slot->endFlag != 0) {
            PW[arg0].step++;
            omVibrate(lbl_1_bss_17A00->chars[arg0].playerNo, 0x78, 6, 6);
            fn_1_5D64(6, VOL(MDL(arg0 + 0x38).z), PAN(MDL(arg0 + 0x38).x));
        }
        break;
    case 2:
        if (Hu3DMotionTimeGet(MDL(arg1 + 2).id) >= 80.0f && PW[arg0].flag28 == 0) {
            Hu3DMotionSpeedSet(MDL(arg1).id, 1.25f);
            Hu3DModelAttrReset(MDL(arg1).id, 1);
            Hu3DModelAttrSet(MDL(arg1).id, 0x40000001);
            PW[arg0].flag28 = 1;
        }
        if (Hu3DMotionEndCheck(MDL(arg1 + 2).id) != 0) {
            Hu3DModelAttrSet(MDL(arg1 + 2).id, 1);
            Hu3DMotionSpeedSet(MDL(arg1 + 3).id, 2.5f);
            Hu3DModelAttrReset(MDL(arg1 + 3).id, 1);
            PW[arg0].flag28 = 0;
            PW[arg0].pad10 = 0;
            PW[arg0].step++;
        }
        break;
    case 3:
        fn_1_5958(arg0, 0.983f);
        if (Hu3DMotionTimeGet(MDL(arg1).id) >= 13.0f && PW[arg0].flag28 == 0) {
            Hu3DMotionSpeedSet(MDL(arg1 + 1).id, 1.25f);
            Hu3DModelAttrReset(MDL(arg1 + 1).id, 1);
            Hu3DModelAttrSet(MDL(arg1 + 1).id, 0x40000001);
            PW[arg0].flag28 = 1;
        }
        if (Hu3DMotionEndCheck(MDL(arg1 + 3).id) != 0) {
            Hu3DModelAttrSet(MDL(arg1 + 3).id, 1);
            Hu3DMotionSpeedSet(MDL(arg1 + 4).id, 2.5f);
            Hu3DModelAttrReset(MDL(arg1 + 4).id, 1);
            PW[arg0].pad10 = 0;
            PW[arg0].step++;
        }
        break;
    case 4:
        fn_1_5958(arg0, 0.983f);
        if (Hu3DMotionTimeGet(MDL(arg1 + 4).id) >= 50.0f) {
            t = Hu3DMotionTimeGet(MDL(arg1).id);
            if (((s16)t % 25) >= 0x18 || ((s16)t % 25) <= 1) {
                Hu3DModelAttrSet(MDL(arg1).id, 1);
            }
            t = Hu3DMotionTimeGet(MDL(arg1 + 1).id);
            if (((s16)t % 25) >= 0x18 || ((s16)t % 25) <= 1) {
                Hu3DModelAttrSet(MDL(arg1 + 1).id, 1);
            }
        }
        if (Hu3DMotionEndCheck(MDL(arg1 + 4).id) != 0) {
            Hu3DModelAttrSet(MDL(arg1 + 4).id, 1);
            Hu3DModelAttrSet(MDL(arg1).id, 1);
            Hu3DModelAttrSet(MDL(arg1 + 1).id, 1);
            slot->shiftState = 1;
            slot->shiftMotion = 0;
            slot->shiftStart = 0.0f;
            slot->shiftEnd = 8.0f;
            slot->shiftAttr = 0;
            slot->attrReset = 0x40000001;
            PW[arg0].timer = 0;
            PW[arg0].step++;
        }
        break;
    case 5:
        if (slot->shiftState == 3) {
            slot->motion = 0;
            slot->speed = 1.0f;
            slot->time = 0.0f;
            slot->attrSet = 0x40000001;
            slot->attrReset = 0;
            fn_1_5AC8(arg0);
            PW[arg0].timer = 0;
            PW[arg0].flag28 = 0;
            PW[arg0].step = 0;
            ret = 1;
        }
        break;
    }
    return ret;
}

s32 fn_1_D6A0(s16 arg0)
{
    M551StageSlot *slot;
    f32 sq;
    f32 len;
    f32 k;
    f32 ang;
    f32 dx;
    f32 dz;
    f32 steps;
    s16 d;
    s16 neg;
    s32 ret;

    slot = &lbl_1_bss_17A00->slots[arg0];
    ret = 0;
    switch (FL.winCount[arg0]) {
    case 0:
        PW[arg0].target.x = -200.0f + 400.0f * arg0;
        PW[arg0].target.z = 0.0f;
        PW[arg0].velX = PW[arg0].target.x - PW[arg0].pos.x;
        PW[arg0].velZ = PW[arg0].target.z - PW[arg0].pos.z;
        sq = PW[arg0].velX * PW[arg0].velX + PW[arg0].velZ * PW[arg0].velZ;
        len = sqrt(sq);
        if (len != 0.0f) {
            FL.flag28[arg0] = 1;
            slot->shiftState = 1;
            slot->shiftMotion = 1;
            slot->shiftStart = 0.0f;
            slot->shiftEnd = 4.0f;
            slot->shiftAttr = 0x40000001;
            slot->attrReset = 0;
        }
        FL.unk2C[1] = 1;
        FL.winCount[arg0]++;
        /* fallthrough */
    case 1:
        PW[arg0].velX = PW[arg0].target.x - PW[arg0].pos.x;
        PW[arg0].velZ = PW[arg0].target.z - PW[arg0].pos.z;
        sq = PW[arg0].velX * PW[arg0].velX + PW[arg0].velZ * PW[arg0].velZ;
        len = sqrt(sq);
        if (len != 0.0f) {
            k = 7.0f / len;
            PW[arg0].velX *= k;
            PW[arg0].velZ *= k;
            if (FL.unk2C[0] >= 0x1E) {
                if (arg0 == 0) {
                    PW[arg0].velZ += 3.0f;
                } else {
                    PW[arg0].velZ -= 3.0f;
                }
                sq = PW[arg0].velX * PW[arg0].velX + PW[arg0].velZ * PW[arg0].velZ;
                len = sqrt(sq);
                if (len != 0.0f) {
                    k = 7.0f / len;
                } else {
                    k = 0.0f;
                }
                PW[arg0].velX *= k;
                PW[arg0].velZ *= k;
            }
            ang = 180.0 * (acos(PW[arg0].velX / (0.1f + 7.0f)) / 3.141592653589793);
            if ((f32)(180.0 * (asin(PW[arg0].velZ / (0.1f + 7.0f)) / 3.141592653589793)) < 0.0f) {
                ang = 360.0f - ang;
            }
            PW[arg0].angle = 90.0f + -ang;
            if (FL.unk2C[0] >= 0xB4 && arg0 == 0) {
                PW[arg0].velX = 0.0f;
                PW[arg0].velZ = 0.0f;
            }
            PW[arg0].pos.x += PW[arg0].velX;
            PW[arg0].pos.z += PW[arg0].velZ;
            lbl_1_bss_0.player[arg0]->actor->rotY = PW[arg0].angle;
            dx = PW[arg0].target.x - PW[arg0].pos.x;
            dz = PW[arg0].target.z - PW[arg0].pos.z;
            len = sqrt(dx * dx + dz * dz);
        }
        if (len < (2.0f + 7.0f)) {
            PW[arg0].angle = lbl_1_bss_0.player[arg0]->actor->rotY;
            d = (s16)lbl_1_bss_0.player[arg0]->actor->rotY % 360;
            if (d != 0) {
                if (d < 0) {
                    d *= -1;
                    neg = 1;
                } else {
                    neg = 0;
                }
                if (d > 0xB4) {
                    d = 0x168 - d;
                } else {
                    d *= -1;
                }
                if (neg != 0) {
                    d *= -1;
                }
            }
            FL.unk20[arg0] = lbl_1_bss_0.player[arg0]->actor->rotY + (f32)d;
            if (arg0 == 0) {
                FL.unk2C[1] = 2;
            }
            FL.winCount[arg0]++;
        }
        break;
    case 2:
        steps = 30.0f;
        if (fn_1_1850(&lbl_1_bss_0.player[arg0]->actor->rotY, steps, PW[arg0].angle, FL.unk20[arg0]) == 0) {
            if (FL.flag28[arg0] != 0) {
                slot->shiftState = 1;
                slot->shiftMotion = 0;
                slot->shiftStart = 0.0f;
                slot->shiftEnd = 4.0f;
                slot->shiftAttr = 0x40000001;
                slot->attrReset = 0;
            }
            FL.winCount[arg0]++;
        }
        break;
    case 3:
        ret = 1;
        break;
    }
    return ret;
}

s16 fn_1_E738(void)
{
    HuVecF pos;
    s16 ret;
    s16 sum;
    s16 i;

    ret = 0;
    sum = 0;
    MgPlayerPadSet(lbl_1_bss_0.player[0], 0, 0, 0, 0);
    MgPlayerPadSet(lbl_1_bss_0.player[1], 0, 0, 0, 0);
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
    MgActorPosGet(lbl_1_bss_0.player[0]->actor, &pos);
    PW[0].pos.x = pos.x;
    PW[0].pos.y = pos.y;
    PW[0].pos.z = pos.z;
    MgActorPosGet(lbl_1_bss_0.player[1]->actor, &pos);
    PW[1].pos.x = pos.x;
    PW[1].pos.y = pos.y;
    PW[1].pos.z = pos.z;
    for (i = 0; i < 2; i++) {
        sum += fn_1_D6A0(i);
        if (FL.winCount[i] >= 2) {
            PW[i].pos = PW[i].oldPos;
        }
    }
    if (sum >= 2) {
        ret = 1;
    }
    fn_1_153A8();
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

s16 fn_1_EA8C(void)
{
    f32 tbl[6][3] = {
        { 74.0f, 100.0f, 5.0f },
        { 100.0f, 85.0f, 5.0f },
        { 85.0f, 100.0f, 5.0f },
        { 100.0f, 95.0f, 5.0f },
        { 95.0f, 100.0f, 5.0f },
        { -1.0f, 0.0f, 0.0f },
    };
    f32 steps;
    s32 chk;
    s16 ret;
    s16 i;

    ret = 0;
    if ((u32)FL.mode <= 8) {
        switch (FL.mode) {
        case 0:
            fn_1_5BF4();
            FL.mode++;
            /* fallthrough */
        case 1:
            if (FL.timer++ > 0x1E) {
                for (i = 0; i < 12; i++) {
                    Hu3DMotionSpeedSet(MDL(lbl_1_bss_179A0[i].mdl[2]).id, 0.0f);
                    Hu3DMotionTimeSet(MDL(lbl_1_bss_179A0[i].mdl[2]).id, 0.0f);
                }
                fn_1_5D64(1, VOL(MDL(43).z), PAN(MDL(43).x));
                FL.timer = 0;
                FL.mode++;
            }
            break;
        case 2:
            if (fn_1_1850(&MDL(43).y, tbl[FL.subMode][2], tbl[FL.subMode][0], tbl[FL.subMode][1]) == 0) {
                FL.subMode++;
                if (tbl[FL.subMode][0] < 0.0f) {
                    FL.subMode = 0;
                    FL.mode++;
                }
            }
            break;
        case 3:
            if (FL.timer++ > 0x1E) {
                FL.timer = 0;
                FL.mode++;
            }
            break;
        case 4:
            steps = 30.0f;
            chk = fn_1_1850(&lbl_1_bss_38->center.x, steps, lbl_1_data_368[0].center.x, lbl_1_data_368[1].center.x);
            chk += fn_1_1850(&lbl_1_bss_38->center.y, steps, lbl_1_data_368[0].center.y, lbl_1_data_368[1].center.y);
            chk += fn_1_1850(&lbl_1_bss_38->center.z, steps, lbl_1_data_368[0].center.z, lbl_1_data_368[1].center.z);
            chk += fn_1_1850(&lbl_1_bss_38->rotX, steps, lbl_1_data_368[0].rotX, lbl_1_data_368[1].rotX);
            chk += fn_1_1850(&lbl_1_bss_38->rotY, steps, lbl_1_data_368[0].rotY, lbl_1_data_368[1].rotY);
            chk += fn_1_1850(&lbl_1_bss_38->zoom, steps, lbl_1_data_368[0].zoom, lbl_1_data_368[1].zoom);
            if ((s16)chk == 0) {
                FL.subMode = frandmod(3);
                if (FL.subMode != 2) {
                    FL.unk10 = frandmod(12);
                    FL.unk12 = FL.unk10;
                } else {
                    FL.unk10 = FL.owner[0];
                    FL.unk12 = -1;
                    FL.unk14 = 0;
                }
                Hu3DModelAttrReset(MDL(lbl_1_bss_179A0[FL.unk10].mdl[2]).id, 1);
                Hu3DModelAttrReset(MDL(lbl_1_bss_179A0[FL.unk10].mdl[2]).id, 0x40000001);
                Hu3DMotionSpeedSet(MDL(lbl_1_bss_179A0[FL.unk10].mdl[2]).id, 2.0f);
                Hu3DMotionTimeSet(MDL(lbl_1_bss_179A0[FL.unk10].mdl[2]).id, 0.0f);
                fn_1_5D64(2, VOL(MDL(lbl_1_bss_179A0[FL.unk10].mdl[2]).z), PAN(MDL(lbl_1_bss_179A0[FL.unk10].mdl[2]).x));
                FL.unk2C[3] = 1;
                FL.mode++;
            }
            break;
        case 5:
            if (FL.timer++ > 5) {
                if (FL.subMode == 0) {
                    if (++FL.unk10 > 0xB) {
                        FL.unk10 = 0;
                    }
                } else if (FL.subMode == 1) {
                    if (--FL.unk10 < 0) {
                        FL.unk10 = 0xB;
                    }
                } else {
                    FL.unk14++;
                    if (FL.unk14 < 0xC) {
                        FL.unk10 = FL.owner[FL.unk14];
                    } else {
                        FL.unk10 = 0;
                        FL.unk12 = 0;
                    }
                }
                Hu3DModelAttrReset(MDL(lbl_1_bss_179A0[FL.unk10].mdl[2]).id, 1);
                Hu3DModelAttrReset(MDL(lbl_1_bss_179A0[FL.unk10].mdl[2]).id, 0x40000001);
                Hu3DMotionSpeedSet(MDL(lbl_1_bss_179A0[FL.unk10].mdl[2]).id, 2.0f);
                Hu3DMotionTimeSet(MDL(lbl_1_bss_179A0[FL.unk10].mdl[2]).id, 0.0f);
                fn_1_5D64(2, VOL(MDL(lbl_1_bss_179A0[FL.unk10].mdl[2]).z), PAN(MDL(lbl_1_bss_179A0[FL.unk10].mdl[2]).x));
                FL.timer = 0;
                if (++FL.unk2C[3] >= 0xC) {
                    FL.mode++;
                }
            }
            break;
        case 6:
            if (FL.timer++ > 0x32) {
                for (i = 0; i < 12; i++) {
                    Hu3DModelAttrReset(MDL(lbl_1_bss_179A0[i].mdl[2]).id, 1);
                    Hu3DModelAttrReset(MDL(lbl_1_bss_179A0[i].mdl[2]).id, 0x40000001);
                    Hu3DMotionSpeedSet(MDL(lbl_1_bss_179A0[i].mdl[2]).id, 2.0f);
                    Hu3DMotionTimeSet(MDL(lbl_1_bss_179A0[i].mdl[2]).id, 0.0f);
                }
                fn_1_5CEC(2);
                FL.timer = 0;
                FL.mode++;
            }
            break;
        case 7:
            if (FL.timer++ > 0x32) {
                for (i = 0; i < 12; i++) {
                    Hu3DModelAttrSet(MDL(lbl_1_bss_179A0[i].mdl[2]).id, 0x40000001);
                    Hu3DMotionSpeedSet(MDL(lbl_1_bss_179A0[i].mdl[2]).id, 0.0f);
                    Hu3DMotionTimeSet(MDL(lbl_1_bss_179A0[i].mdl[2]).id, 0.0f);
                }
                FL.mode++;
            }
            break;
        case 8:
            ret = 1;
            break;
        }
    }
    return ret;
}
