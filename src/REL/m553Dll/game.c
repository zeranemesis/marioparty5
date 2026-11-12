#include "REL/m553Dll.h"
#include "game/frand.h"
#include "game/mg/actman.h"
#include "game/mg/seqman.h"
#include "game/flag.h"
#include "game/hu3d.h"
#include "string.h"
#include "math.h"

extern float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);
extern void MgActorPosGet(MGACTOR *actor, HuVecF *pos);

extern void fn_1_94C(s16 no);
extern void fn_1_10F4(void);
extern void fn_1_14F4(void);
extern s32 fn_1_1880(f32 *value, f32 time, f32 from, f32 to);
extern MGTIMER *fn_1_1ABC(int type, int maxTime, int endTime, f32 x, f32 y);
extern void fn_1_1B30(MGTIMER *timer);
extern s32 fn_1_1B5C(MGTIMER *timer);
extern void fn_1_1B84(MGTIMER *timer);
extern void fn_1_23CC(void);
extern void fn_1_28D4(HuVecF *pos);
extern void fn_1_2B4C(HuVecF *pos);
extern void fn_1_36E4(void);
extern void fn_1_3714(s16 which, s16 speed);
extern void fn_1_37DC(s16 no);
extern void fn_1_3854(s16 no, s16 vol, s16 pan);
extern void fn_1_3AC4(void);
extern void fn_1_E690(void);

M553STWORK *lbl_1_bss_1EF8;

#define W lbl_1_bss_1EF8
#define MDL(i) (W->model[i])

void fn_1_6F30(void);
void fn_1_7368(void);
s16 fn_1_80B8(s16 arg0);
s16 fn_1_84C8(void);
s32 fn_1_8948(s16 arg0);
s16 fn_1_984C(void);
s16 fn_1_9A70(void);
int fn_1_B5F4(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b);
void fn_1_B654(MGACTOR *actor, int param);
void fn_1_B668(M553GPLAYERSTATE *player);
void fn_1_C650(s16 no);
s32 fn_1_C92C(s16 no);
s32 fn_1_CA98(s16 no);
s16 fn_1_CC28(s16 no);
void fn_1_D070(s16 no);
void fn_1_D1B8(s16 no);

void fn_1_3AE4(M553STWORK *work)
{
    HU3DMODELID tbl[4];
    MGACTOR_PARAM param;
    s16 i, j, k;
    s16 cell;
    s16 dup;

    memset(&lbl_1_bss_70C, 0, sizeof(lbl_1_bss_70C));
    memset(&lbl_1_bss_48C, 0, sizeof(lbl_1_bss_48C));
#ifdef TARGET_PC
    memset(&lbl_1_bss_478, 0, sizeof(lbl_1_bss_478));
#else
    memset(&lbl_1_bss_478, 0, 0x12);
#endif
    memset(&lbl_1_bss_18, 0, sizeof(lbl_1_bss_18));
    memset(&lbl_1_bss_0, 0, sizeof(lbl_1_bss_0));
    W = work;
    for (i = 0; i < 22; i++) {
        lbl_1_bss_70C.seState[i] = -1;
    }
    lbl_1_bss_70C.scaleA = 0.0f;
    lbl_1_bss_70C.scaleB = 0.0f;
    for (i = 0, j = 0; i <= 1; i++, j++) {
        MDL(i).id = Hu3DModelCreateData(W->data[j]);
        Hu3DModelAttrSet(MDL(i).id, 1);
        Hu3DModelCameraSet(MDL(i).id, 1);
        Hu3DModelLayerSet(MDL(i).id, 0);
        MDL(i).pos.x = 0.0f;
        MDL(i).pos.y = 0.0f;
        MDL(i).pos.z = 0.0f;
        MDL(i).rot.x = 0.0f;
        MDL(i).rot.y = 0.0f;
        MDL(i).rot.z = 0.0f;
        Hu3DModelAttrReset(MDL(i).id, 1);
        if (i == 1) {
            MDL(i).rot.x = 0.0f;
            MDL(i).rot.y = 22.5f;
            MDL(i).rot.z = 0.0f;
            Hu3DModelAttrSet(MDL(i).id, 0x40000001);
            Hu3DMotionSpeedSet(MDL(i).id, 0.25f);
            Hu3DMotionTimeSet(MDL(i).id, 0.0f);
        }
    }
    MDL(0x65).id = Hu3DModelCreateData(W->data[0xB]);
    Hu3DModelAttrSet(MDL(0x65).id, 1);
    Hu3DModelCameraSet(MDL(0x65).id, 1);
    MDL(0x65).pos.x = 0.0f;
    MDL(0x65).pos.y = 0.0f;
    MDL(0x65).pos.z = 0.0f;
    MDL(0x65).rot.x = 0.0f;
    MDL(0x65).rot.y = 0.0f;
    MDL(0x65).rot.z = 0.0f;
    lbl_1_bss_0.colMdl = MDL(0x65).id;
    MgActorColMapInit(&lbl_1_bss_0.colMdl, 1, 10);
    param.height = 150.0f;
    param.radius = 35.0f;
    param.param = 0;
    param.type = 0;
    param.attr = 0;
    param.narrowHook = fn_1_B5F4;
    param.correctHookParam = 0;
    param.correctHook = fn_1_B654;
    lbl_1_bss_0.player[0] = MgPlayerCreate(W->player[0].idx, &param, 1, 1, -0x3D, NULL);
    lbl_1_bss_0.player[0]->actor->pos.x = -150.0f;
    lbl_1_bss_0.player[0]->actor->pos.y = 100.0f;
    lbl_1_bss_0.player[0]->actor->pos.z = 0.0f;
    lbl_1_bss_0.player[0]->actor->rotY = 0.0f;
    Hu3DModelLayerSet(lbl_1_bss_0.player[0]->actor->mdlId, 0);
    Hu3DModelShadowSet(lbl_1_bss_0.player[0]->actor->mdlId);
    Hu3DModelShadowDispOn(lbl_1_bss_0.player[0]->actor->mdlId);
    MDL(0x86).id = lbl_1_bss_0.player[0]->actor->mdlId;
    CharModelStepFxSet(W->player[0].charNo, 2);
    param.param = 1;
    lbl_1_bss_0.player[1] = MgPlayerCreate(W->player[1].idx, &param, 1, 1, -0x3D, NULL);
    lbl_1_bss_0.player[1]->actor->pos.x = 150.0f;
    lbl_1_bss_0.player[1]->actor->pos.y = 100.0f;
    lbl_1_bss_0.player[1]->actor->pos.z = 0.0f;
    lbl_1_bss_0.player[1]->actor->rotY = 0.0f;
    Hu3DModelLayerSet(lbl_1_bss_0.player[1]->actor->mdlId, 0);
    Hu3DModelShadowSet(lbl_1_bss_0.player[1]->actor->mdlId);
    Hu3DModelShadowDispOn(lbl_1_bss_0.player[1]->actor->mdlId);
    MDL(0x87).id = lbl_1_bss_0.player[1]->actor->mdlId;
    CharModelStepFxSet(W->player[1].charNo, 2);
    MgActorPosUpdate(lbl_1_bss_0.player[0]->actor, &lbl_1_bss_0.player[0]->actor->pos);
    MgActorPosUpdate(lbl_1_bss_0.player[1]->actor, &lbl_1_bss_0.player[1]->actor->pos);
    for (i = 0x61, j = 0; i <= 0x62; i++, j++) {
        MDL(i).id = Hu3DModelCreateData(W->data[10]);
        Hu3DModelAttrSet(MDL(i).id, 1);
        Hu3DModelCameraSet(MDL(i).id, 1);
        Hu3DModelLayerSet(MDL(i).id, 1);
        Hu3DModelScaleSet(MDL(i).id, 0.0f, 0.0f, 0.0f);
        MDL(i).rot.x = 0.0f;
        MDL(i).rot.y = 0.0f;
        MDL(i).rot.z = 0.0f;
        MDL(i).pos.x = -200.0f + (400.0f * j);
        MDL(i).pos.y = 0.0f;
        MDL(i).pos.z = 0.0f;
    }
    for (i = 0x63, j = 0; i <= 0x64; i++, j++) {
        MDL(i).id = Hu3DModelCreateData(W->data[9]);
        Hu3DModelAttrSet(MDL(i).id, 1);
        Hu3DModelAttrSet(MDL(i).id, 0x04000000);
        Hu3DModelCameraSet(MDL(i).id, 1);
        Hu3DModelLayerSet(MDL(i).id, 1);
        Hu3DModelScaleSet(MDL(i).id, 0.0f, 0.0f, 0.0f);
        MDL(i).rot.x = 0.0f;
        MDL(i).rot.y = 0.0f;
        MDL(i).rot.z = 0.0f;
        MDL(i).pos.x = -200.0f + (400.0f * j);
        MDL(i).pos.y = 0.0f;
        MDL(i).pos.z = 0.0f;
    }
    MDL(2).id = Hu3DModelCreateData(W->data[2]);
    Hu3DModelAttrSet(MDL(2).id, 1);
    Hu3DModelCameraSet(MDL(2).id, 1);
    Hu3DModelLayerSet(MDL(2).id, 1);
    Hu3DModelShadowMapSet(MDL(2).id);
    MDL(2).pos.x = 0.0f;
    MDL(2).pos.y = 0.0f;
    MDL(2).pos.z = 0.0f;
    MDL(2).rot.x = 0.0f;
    MDL(2).rot.y = 0.0f;
    MDL(2).rot.z = 0.0f;
    Hu3DModelAttrSet(MDL(2).id, 0x40000001);
    Hu3DMotionSpeedSet(MDL(2).id, 0.25f);
    Hu3DMotionTimeSet(MDL(2).id, 0.0f);
    Hu3DModelAttrReset(MDL(2).id, 1);
    for (i = 3; i <= 0x25; i++) {
        if (i == 3) {
            MDL(i).id = Hu3DModelCreateData(W->data[3]);
        } else {
            MDL(i).id = Hu3DModelLink(MDL(3).id);
        }
        Hu3DModelAttrSet(MDL(i).id, 1);
        Hu3DModelCameraSet(MDL(i).id, 1);
        Hu3DModelLayerSet(MDL(i).id, 5);
        Hu3DMotionSpeedSet(MDL(i).id, 0.0f);
        Hu3DMotionTimeSet(MDL(i).id, 0.0f);
        MDL(i).pos.x = 0.0f;
        MDL(i).pos.y = 500.0f;
        MDL(i).pos.z = 0.0f;
        MDL(i).rot.x = 0.0f;
        MDL(i).rot.y = 0.0f;
        MDL(i).rot.z = 0.0f;
    }
    for (i = 0x26; i <= 0x39; i++) {
        if (i == 0x26) {
            MDL(i).id = Hu3DModelCreateData(W->data[4]);
        } else {
            MDL(i).id = Hu3DModelLink(MDL(0x26).id);
        }
        Hu3DModelAttrSet(MDL(i).id, 1);
        Hu3DModelCameraSet(MDL(i).id, 1);
        Hu3DModelLayerSet(MDL(i).id, 5);
        Hu3DMotionSpeedSet(MDL(i).id, 0.0f);
        Hu3DMotionTimeSet(MDL(i).id, 0.0f);
        MDL(i).pos.x = 0.0f;
        MDL(i).pos.y = 500.0f;
        MDL(i).pos.z = 0.0f;
        MDL(i).rot.x = 0.0f;
        MDL(i).rot.y = 0.0f;
        MDL(i).rot.z = 0.0f;
    }
    for (i = 0x3A; i <= 0x48; i++) {
        if (i == 0x3A) {
            MDL(i).id = Hu3DModelCreateData(W->data[5]);
        } else {
            MDL(i).id = Hu3DModelLink(MDL(0x3A).id);
        }
        Hu3DModelAttrSet(MDL(i).id, 1);
        Hu3DModelCameraSet(MDL(i).id, 1);
        Hu3DModelLayerSet(MDL(i).id, 5);
        Hu3DMotionSpeedSet(MDL(i).id, 0.0f);
        Hu3DMotionTimeSet(MDL(i).id, 0.0f);
        MDL(i).pos.x = 0.0f;
        MDL(i).pos.y = 500.0f;
        MDL(i).pos.z = 0.0f;
        MDL(i).rot.x = 0.0f;
        MDL(i).rot.y = 0.0f;
        MDL(i).rot.z = 0.0f;
    }
    for (i = 0x49; i <= 0x5C; i++) {
        if (i == 0x49) {
            MDL(i).id = Hu3DModelCreateData(W->data[6]);
        } else {
            MDL(i).id = Hu3DModelLink(MDL(0x49).id);
        }
        Hu3DModelAttrSet(MDL(i).id, 1);
        Hu3DModelCameraSet(MDL(i).id, 1);
        Hu3DModelLayerSet(MDL(i).id, 5);
        Hu3DMotionSpeedSet(MDL(i).id, 0.0f);
        Hu3DMotionTimeSet(MDL(i).id, 0.0f);
        MDL(i).pos.x = 0.0f;
        MDL(i).pos.y = 500.0f;
        MDL(i).pos.z = 0.0f;
        MDL(i).rot.x = 0.0f;
        MDL(i).rot.y = 0.0f;
        MDL(i).rot.z = 0.0f;
    }
    tbl[0] = 7;
    tbl[1] = 7;
    tbl[2] = 8;
    tbl[3] = 8;
    for (i = 0x5D, j = 0; i <= 0x60; i++, j++) {
        MDL(i).id = Hu3DModelCreateData(W->data[tbl[j]]);
        Hu3DModelAttrSet(MDL(i).id, 1);
        Hu3DModelCameraSet(MDL(i).id, 1);
        Hu3DModelLayerSet(MDL(i).id, 5);
        Hu3DMotionSpeedSet(MDL(i).id, 0.0f);
        Hu3DMotionTimeSet(MDL(i).id, 0.0f);
        MDL(i).pos.x = 0.0f;
        MDL(i).pos.y = 0.0f;
        MDL(i).pos.z = 0.0f;
        MDL(i).rot.x = 0.0f;
        MDL(i).rot.y = 0.0f;
        MDL(i).rot.z = 0.0f;
        if (tbl[j] == 7) {
            Hu3DModelShadowMapSet(MDL(i).id);
        }
    }
    lbl_1_bss_478[4] = frandmod(5) + 15;
    lbl_1_bss_478[5] = frandmod(5) + 5;
    lbl_1_bss_478[6] = lbl_1_bss_478[4] + lbl_1_bss_478[5];
    lbl_1_bss_478[4] = 20;
    lbl_1_bss_478[5] = 15;
    lbl_1_bss_478[6] = lbl_1_bss_478[4] + lbl_1_bss_478[5];
    for (i = 0; i < lbl_1_bss_478[4]; i++) {
        do {
            cell = frandmod(0x40);
            dup = 0;
            for (k = 0; k < i; k++) {
                if (cell == lbl_1_bss_18[k].cell) {
                    dup = 1;
                }
            }
        } while (dup != 0);
        lbl_1_bss_18[i].active = 1;
        lbl_1_bss_18[i].vis = 1;
        lbl_1_bss_18[i].cell = cell;
        lbl_1_bss_18[i].mdl = i + 0x26;
        lbl_1_bss_18[i].mdl2 = i + 0x49;
        lbl_1_bss_18[i].mdl3 = i + 3;
        lbl_1_bss_18[i].type = 1;
        lbl_1_bss_18[i].tpLvl = 1.0f;
        MDL(lbl_1_bss_18[i].mdl).pos.x = (100.0f * (cell % 8)) - 360.0f;
        MDL(lbl_1_bss_18[i].mdl).pos.z = (100.0f * (cell / 8)) - 360.0f;
        MDL(lbl_1_bss_18[i].mdl2).pos.x = (100.0f * (cell % 8)) - 360.0f;
        MDL(lbl_1_bss_18[i].mdl2).pos.z = (100.0f * (cell / 8)) - 360.0f;
        MDL(lbl_1_bss_18[i].mdl3).pos.x = (100.0f * (cell % 8)) - 360.0f;
        MDL(lbl_1_bss_18[i].mdl3).pos.z = (100.0f * (cell / 8)) - 360.0f;
    }
    for (i = lbl_1_bss_478[4], j = 0; i < lbl_1_bss_478[6]; i++, j++) {
        do {
            cell = frandmod(0x40);
            dup = 0;
            for (k = 0; k < i; k++) {
                if (cell == lbl_1_bss_18[k].cell) {
                    dup = 1;
                }
            }
        } while (dup != 0);
        lbl_1_bss_18[i].active = 1;
        lbl_1_bss_18[i].vis = 1;
        lbl_1_bss_18[i].cell = cell;
        lbl_1_bss_18[i].mdl = j + 0x3A;
        lbl_1_bss_18[i].mdl2 = -1;
        lbl_1_bss_18[i].mdl3 = i + 3;
        lbl_1_bss_18[i].type = 0;
        lbl_1_bss_18[i].tpLvl = 1.0f;
        MDL(lbl_1_bss_18[i].mdl).pos.x = (100.0f * (cell % 8)) - 360.0f;
        MDL(lbl_1_bss_18[i].mdl).pos.z = (100.0f * (cell / 8)) - 360.0f;
        MDL(lbl_1_bss_18[i].mdl3).pos.x = (100.0f * (cell % 8)) - 360.0f;
        MDL(lbl_1_bss_18[i].mdl3).pos.z = (100.0f * (cell / 8)) - 360.0f;
    }
    MgPlayerAttrSet(lbl_1_bss_0.player[0], 2);
    MgPlayerAttrSet(lbl_1_bss_0.player[1], 2);
    CharEffectLayerSet(2);
    fn_1_C650(0);
    fn_1_C650(1);
    fn_1_E690();
    fn_1_23CC();
    fn_1_6F30();
    lbl_1_bss_70C.timerObj = fn_1_1ABC(0, 15, 0, 292.0f, 64.0f);
}

s32 fn_1_6064(M553STWORK *work)
{
    HU3DMODEL *model1;
    HU3DMODEL *model2;

    fn_1_3AE4(work);
    MgPlayerPadSet(lbl_1_bss_0.player[0], 0, 0, 0, 0);
    MgPlayerPadSet(lbl_1_bss_0.player[1], 0, 0, 0, 0);
    MgActorExec();
    model1 = &Hu3DData[MDL(0x86).id];
    lbl_1_bss_48C[0].pos.x = model1->pos.x;
    lbl_1_bss_48C[0].pos.y = model1->pos.y;
    lbl_1_bss_48C[0].pos.z = model1->pos.z;
    model2 = &Hu3DData[MDL(0x87).id];
    lbl_1_bss_48C[1].pos.x = model2->pos.x;
    lbl_1_bss_48C[1].pos.y = model2->pos.y;
    lbl_1_bss_48C[1].pos.z = model2->pos.z;
    return 1;
}

s32 fn_1_6170(void)
{
    s16 ret = 0;

    ret = fn_1_984C();
    fn_1_6F30();
    return ret;
}

s32 fn_1_61A8(void)
{
    s32 ret = 0;

    lbl_1_bss_70C.mode = 0;
    lbl_1_bss_70C.sub = 0;
    lbl_1_bss_70C.unk04 = 0;
    lbl_1_bss_70C.timer = 0;
    fn_1_6F30();
    return ret;
}

s32 fn_1_6218(void)
{
    HuVecF dbg;
    HuVecF pos;
    s16 i;
    s32 ret;
    s16 flag;
    s16 done;

    ret = 0;
    flag = 0;
    switch (lbl_1_bss_70C.mode) {
        case 0:
            fn_1_1B30(lbl_1_bss_70C.timerObj);
            lbl_1_bss_70C.mode++;
        case 1:
            if ((HuPadBtn[0] & 0x800) != 0) {
                dbg.x = 0.0f;
                dbg.y = 200.0f;
                dbg.z = 15.0f;
                fn_1_2B4C(&dbg);
            }
            done = fn_1_1B5C(lbl_1_bss_70C.timerObj);
            if (lbl_1_bss_478[7] == 0 && done == 0) {
                for (i = 0; i < 2; i++) {
                    if (lbl_1_bss_48C[i].comOn != 0) {
                        if (lbl_1_bss_48C[i].delay <= 0) {
                            fn_1_C92C(i);
                        } else {
                            lbl_1_bss_48C[i].delay--;
                        }
                    }
                }
            }
            if (lbl_1_bss_478[7] != 0 || done != 0) {
                lbl_1_bss_48C[0].stop = 1;
                lbl_1_bss_48C[1].stop = 1;
                if (lbl_1_bss_478[8] == 0) {
                    flag = 1;
                }
            }
            if (flag != 0) {
                if (lbl_1_bss_48C[0].itemCnt == lbl_1_bss_48C[1].itemCnt) {
                    lbl_1_bss_70C.winner = 0;
                } else if (lbl_1_bss_48C[0].itemCnt > lbl_1_bss_48C[1].itemCnt) {
                    lbl_1_bss_70C.winner = 2;
                } else {
                    lbl_1_bss_70C.winner = 1;
                }
                lbl_1_bss_70C.mode++;
            }
            break;
        case 2:
            fn_1_1B84(lbl_1_bss_70C.timerObj);
            if (lbl_1_bss_70C.winner != 0) {
                s16 playerNo;

                MgSeqWinnerSet(W->player[lbl_1_bss_70C.winner & 1].charNo, -1, -1, -1);
                playerNo = W->player[lbl_1_bss_70C.winner & 1].idx;
                if (_CheckFlag(0x1000F) == 0) {
                    GwPlayer[playerNo].mgCoinBonus = 10;
                }
            } else {
                MgSeqWinnerSet(-1, -1, -1, -1);
            }
            fn_1_3714(1, 100);
            ret = 1;
            break;
    }
    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_48C[i].comOn != 0) {
            MgPlayerPadSet(lbl_1_bss_0.player[i], lbl_1_bss_48C[i].padStkX, lbl_1_bss_48C[i].padStkY, lbl_1_bss_48C[i].padBtnDown, lbl_1_bss_48C[i].padBtn);
        }
        if (lbl_1_bss_48C[i].stop != 0) {
            MgPlayerPadSet(lbl_1_bss_0.player[i], 0, 0, 0, 0);
        }
    }
    MgActorPosGet(lbl_1_bss_0.player[0]->actor, &pos);
    lbl_1_bss_48C[0].oldPos.x = pos.x;
    lbl_1_bss_48C[0].oldPos.y = pos.y;
    lbl_1_bss_48C[0].oldPos.z = pos.z;
    MgActorPosGet(lbl_1_bss_0.player[1]->actor, &pos);
    lbl_1_bss_48C[1].oldPos.x = pos.x;
    lbl_1_bss_48C[1].oldPos.y = pos.y;
    lbl_1_bss_48C[1].oldPos.z = pos.z;
    MgActorExec();
    MgActorPosGet(lbl_1_bss_0.player[0]->actor, &pos);
    lbl_1_bss_48C[0].pos.x = pos.x;
    lbl_1_bss_48C[0].pos.y = pos.y;
    lbl_1_bss_48C[0].pos.z = pos.z;
    MgActorPosGet(lbl_1_bss_0.player[1]->actor, &pos);
    lbl_1_bss_48C[1].pos.x = pos.x;
    lbl_1_bss_48C[1].pos.y = pos.y;
    lbl_1_bss_48C[1].pos.z = pos.z;
    fn_1_B668(lbl_1_bss_48C);
    fn_1_7368();
    pos.x = lbl_1_bss_48C[0].pos.x;
    pos.y = lbl_1_bss_48C[0].pos.y;
    pos.z = lbl_1_bss_48C[0].pos.z;
    MgActorPosSet(lbl_1_bss_0.player[0]->actor, &pos);
    pos.x = lbl_1_bss_48C[1].pos.x;
    pos.y = lbl_1_bss_48C[1].pos.y;
    pos.z = lbl_1_bss_48C[1].pos.z;
    MgActorPosSet(lbl_1_bss_0.player[1]->actor, &pos);
    fn_1_6F30();
    return ret;
}

s32 fn_1_6850(void)
{
    s32 ret = 0;

    fn_1_84C8();
    lbl_1_bss_70C.mode = 0;
    lbl_1_bss_70C.sub = 0;
    lbl_1_bss_70C.unk04 = 0;
    lbl_1_bss_70C.timer = 0;
    lbl_1_bss_48C[0].phase = 0;
    lbl_1_bss_48C[1].phase = 0;
    MgPlayerPadSet(lbl_1_bss_0.player[0], 0, 0, 0, 0);
    MgPlayerPadSet(lbl_1_bss_0.player[1], 0, 0, 0, 0);
    MgActorExec();
    fn_1_6F30();
    return ret;
}

s32 fn_1_6928(void)
{
    s16 ret = 0;

    ret = fn_1_9A70();
    fn_1_B668(lbl_1_bss_48C);
    fn_1_6F30();
    return ret;
}

s32 fn_1_6964(void)
{
    M553STMOTCTL *ctl;

    if (W->endFlag == 0) {
        if (lbl_1_bss_70C.winner != 0) {
            ctl = &W->motCtl[lbl_1_bss_70C.winner & 1];
            ctl->motNo = 3;
            ctl->speed = 1.0f;
            ctl->time = 0.0f;
            ctl->attr = 0;
            ctl->attrReset = 0x40000001;
            ctl = &W->motCtl[(lbl_1_bss_70C.winner & 2) >> 1];
            ctl->motNo = 4;
            ctl->speed = 1.0f;
            ctl->time = 0.0f;
            ctl->attr = 0;
            ctl->attrReset = 0x40000001;
        } else if (lbl_1_bss_48C[0].active != 0 || lbl_1_bss_48C[1].active != 0) {
            ctl = &W->motCtl[0];
            ctl->motNo = 4;
            ctl->speed = 1.0f;
            ctl->time = 0.0f;
            ctl->attr = 0;
            ctl->attrReset = 0x40000001;
            ctl = &W->motCtl[1];
            ctl->motNo = 4;
            ctl->speed = 1.0f;
            ctl->time = 0.0f;
            ctl->attr = 0;
            ctl->attrReset = 0x40000001;
            CharLoseVoicePlay(W->player[0].charNo, W->player[1].charNo, -1, -1);
        }
        W->endFlag = 1;
    }
    fn_1_6F30();
    if (lbl_1_bss_70C.winner != 0) {
        CharModelVoicePanSet(W->player[lbl_1_bss_70C.winner & 1].charNo, 0x40);
    }
    return 0;
}

s32 fn_1_6BDC(void)
{
    s32 ret = 1;

    fn_1_6F30();
    return ret;
}

s32 fn_1_6C0C(void)
{
    s32 ret = 1;

    fn_1_3AC4();
    MgActorKill(lbl_1_bss_0.player[0]->actor);
    MgActorKill(lbl_1_bss_0.player[1]->actor);
    return ret;
}

void fn_1_6C64(void)
{
    HU3DMODEL *model1;
    HU3DMODEL *model2;
    s16 pan;
    s16 i;

    model1 = &Hu3DData[MDL(0x86).id];
    model2 = &Hu3DData[MDL(0x87).id];
    for (i = 0; i < 0x86; i++) {
        Hu3DModelPosSet(MDL(i).id, MDL(i).pos.x, MDL(i).pos.y, MDL(i).pos.z);
        Hu3DModelRotSet(MDL(i).id, MDL(i).rot.x, MDL(i).rot.y, MDL(i).rot.z);
    }
    pan = 64.0f + (0.042666666f * model1->pos.x);
    CharModelVoiceVolSet(W->player[0].charNo, 111.0f + (0.020666666f * model1->pos.z));
    CharModelVoicePanSet(W->player[0].charNo, pan);
    pan = 64.0f + (0.042666666f * model2->pos.x);
    CharModelVoiceVolSet(W->player[1].charNo, 111.0f + (0.020666666f * model2->pos.z));
    CharModelVoicePanSet(W->player[1].charNo, pan);
}

void fn_1_6F30(void)
{
    HU3DMODEL *model1;
    HU3DMODEL *model2;
    s16 pan;
    s16 i;

    Hu3DModelAttrSet(MDL(0x75).id, 0x400000);
    Hu3DModelLayerSet(MDL(0x75).id, 7);
    MDL(0x75).pos.x = -1100.0f + lbl_1_bss_14->pos.x;
    MDL(0x75).pos.y = 1150.0f + (lbl_1_bss_14->pos.y - 953.0f);
    MDL(0x75).pos.z = -800.0f + (lbl_1_bss_14->pos.z - 1556.0f);
    model1 = &Hu3DData[MDL(0x86).id];
    model2 = &Hu3DData[MDL(0x87).id];
    for (i = 0; i < 0x86; i++) {
        Hu3DModelPosSet(MDL(i).id, MDL(i).pos.x, MDL(i).pos.y, MDL(i).pos.z);
        Hu3DModelRotSet(MDL(i).id, MDL(i).rot.x, MDL(i).rot.y, MDL(i).rot.z);
    }
    pan = 64.0f + (0.042666666f * model1->pos.x);
    CharModelVoiceVolSet(W->player[0].charNo, 111.0f + (0.020666666f * model1->pos.z));
    CharModelVoicePanSet(W->player[0].charNo, pan);
    pan = 64.0f + (0.042666666f * model2->pos.x);
    CharModelVoiceVolSet(W->player[1].charNo, 111.0f + (0.020666666f * model2->pos.z));
    CharModelVoicePanSet(W->player[1].charNo, pan);
    fn_1_94C(0);
    fn_1_10F4();
    fn_1_14F4();
}

s32 fn_1_7300(s16 no)
{
    M553STMOTCTL *ctl = &W->motCtl[no];
    s16 i = 0;
    s32 ret = 0;

    if (lbl_1_bss_48C[no].active != 0) {
    }
    return ret;
}

void fn_1_7368(void)
{
    s16 i;
    s16 hit = 0;
    s16 active = 0;
    s16 spA = 0;
    s16 sp8 = 0;
    f32 maxT;
    f32 nowT;

    for (i = 0; i < lbl_1_bss_478[6]; i++) {
        if (lbl_1_bss_18[i].active != 0) {
            active = 1;
            if (lbl_1_bss_18[i].hit != 0) {
                lbl_1_bss_48C[0].cellFlag[lbl_1_bss_18[i].cell] = 1;
                lbl_1_bss_48C[1].cellFlag[lbl_1_bss_18[i].cell] = 1;
                hit = 1;
            }
            if (lbl_1_bss_18[i].type != 0) {
                switch (lbl_1_bss_18[i].phase) {
                    case 0:
                        if (lbl_1_bss_18[i].hit != 0) {
                            Hu3DModelAttrSet(MDL(lbl_1_bss_18[i].mdl3).id, 1);
                            Hu3DModelAttrReset(MDL(lbl_1_bss_18[i].mdl).id, 1);
                            Hu3DModelAttrReset(MDL(lbl_1_bss_18[i].mdl).id, 0x40000001);
                            Hu3DMotionSpeedSet(MDL(lbl_1_bss_18[i].mdl).id, 1.0f);
                            Hu3DMotionTimeSet(MDL(lbl_1_bss_18[i].mdl).id, 0.0f);
                            Hu3DModelAttrReset(MDL(lbl_1_bss_18[i].mdl2).id, 1);
                            Hu3DModelAttrReset(MDL(lbl_1_bss_18[i].mdl2).id, 0x40000001);
                            Hu3DMotionSpeedSet(MDL(lbl_1_bss_18[i].mdl2).id, 1.0f);
                            Hu3DMotionTimeSet(MDL(lbl_1_bss_18[i].mdl2).id, 0.0f);
                            lbl_1_bss_48C[lbl_1_bss_18[i].player].item[lbl_1_bss_48C[lbl_1_bss_18[i].player].itemCnt] = lbl_1_bss_18[i].mdl;
                            lbl_1_bss_48C[lbl_1_bss_18[i].player].itemCnt++;
                            fn_1_3854(0, 111.0f + (0.020666666f * MDL(lbl_1_bss_18[i].mdl2).pos.z), 64.0f + (0.042666666f * MDL(lbl_1_bss_18[i].mdl2).pos.x));
                            lbl_1_bss_18[i].phase++;
                        }
                        break;
                    case 1:
                        if (lbl_1_bss_0.player[lbl_1_bss_18[i].player]->actor->velY >= 0.0f && lbl_1_bss_48C[lbl_1_bss_18[i].player].pos.y >= 250.0f) {
                            lbl_1_bss_0.player[lbl_1_bss_18[i].player]->actor->velY = 0.0f;
                            lbl_1_bss_48C[lbl_1_bss_18[i].player].pos.y = 250.0f;
                        } else {
                            lbl_1_bss_48C[lbl_1_bss_18[i].player].hitting = 0;
                            lbl_1_bss_18[i].phase++;
                        }
                        break;
                    case 2:
                        maxT = Hu3DMotionMaxTimeGet(MDL(lbl_1_bss_18[i].mdl).id) - 1.0f;
                        nowT = Hu3DMotionTimeGet(MDL(lbl_1_bss_18[i].mdl).id);
                        if (Hu3DMotionEndCheck(MDL(lbl_1_bss_18[i].mdl).id) != 0) {
                            if (maxT <= nowT) {
                                Hu3DModelAttrSet(MDL(lbl_1_bss_18[i].mdl).id, 1);
                                Hu3DModelAttrSet(MDL(lbl_1_bss_18[i].mdl2).id, 1);
                                lbl_1_bss_18[i].active = 0;
                            }
                        }
                        break;
                }
            } else {
                switch (lbl_1_bss_18[i].phase) {
                    case 0:
                        if (lbl_1_bss_18[i].hit != 0) {
                            Hu3DModelAttrSet(MDL(lbl_1_bss_18[i].mdl3).id, 1);
                            Hu3DModelAttrReset(MDL(lbl_1_bss_18[i].mdl).id, 1);
                            Hu3DModelAttrReset(MDL(lbl_1_bss_18[i].mdl).id, 0x40000001);
                            Hu3DMotionSpeedSet(MDL(lbl_1_bss_18[i].mdl).id, 1.0f);
                            Hu3DMotionTimeSet(MDL(lbl_1_bss_18[i].mdl).id, 0.0f);
                            lbl_1_bss_48C[lbl_1_bss_18[i].player].itemCnt2++;
                            fn_1_3854(1, 111.0f + (0.020666666f * MDL(lbl_1_bss_18[i].mdl).pos.z), 64.0f + (0.042666666f * MDL(lbl_1_bss_18[i].mdl).pos.x));
                            lbl_1_bss_18[i].phase++;
                            lbl_1_bss_18[i].vis = 0;
                        }
                        break;
                    case 1:
                        if (lbl_1_bss_0.player[lbl_1_bss_18[i].player]->actor->velY >= 0.0f) {
                            lbl_1_bss_0.player[lbl_1_bss_18[i].player]->actor->velY = 0.0f;
                            lbl_1_bss_48C[lbl_1_bss_18[i].player].pos.y = 250.0f;
                        } else {
                            lbl_1_bss_48C[lbl_1_bss_18[i].player].hitting = 0;
                            lbl_1_bss_18[i].phase++;
                        }
                        break;
                    case 2:
                        maxT = Hu3DMotionMaxTimeGet(MDL(lbl_1_bss_18[i].mdl).id) - 2.0f;
                        if (maxT <= Hu3DMotionTimeGet(MDL(lbl_1_bss_18[i].mdl).id)) {
                            Hu3DModelAttrSet(MDL(lbl_1_bss_18[i].mdl).id, 1);
                            lbl_1_bss_18[i].active = 0;
                        }
                        break;
                }
            }
        }
    }
    if (active == 0) {
        lbl_1_bss_478[7] = 1;
    }
    if (hit != 0) {
        lbl_1_bss_478[8] = 1;
    } else {
        lbl_1_bss_478[8] = 0;
    }
    fn_1_80B8(0);
}

s16 fn_1_80B8(s16 arg0)
{
    u16 tbl[2] = { 300, 600 };
    s16 ret = 0;
    s16 i;
    s16 idx;

    lbl_1_bss_478[3]++;
    switch (lbl_1_bss_478[0]) {
        case 0:
            idx = lbl_1_bss_478[1];
#ifdef TARGET_PC
            if (idx >= 2) {
                idx = 1;
            }
#endif
            if (lbl_1_bss_478[3] == tbl[idx]) {
                lbl_1_bss_478[0]++;
                lbl_1_bss_478[1]++;
            }
            if (arg0 != 0) {
                lbl_1_bss_478[0]++;
            }
            if (lbl_1_bss_478[0] == 1) {
                fn_1_37DC(2);
            }
            break;
        case 1:
            for (i = 0; i < lbl_1_bss_478[6]; i++) {
                if (lbl_1_bss_18[i].active != 0 && lbl_1_bss_18[i].hit == 0) {
                    Hu3DModelAttrReset(MDL(lbl_1_bss_18[i].mdl3).id, 1);
                    Hu3DMotionSpeedSet(MDL(lbl_1_bss_18[i].mdl3).id, 1.0f);
                    Hu3DMotionTimeSet(MDL(lbl_1_bss_18[i].mdl3).id, 0.0f);
                }
            }
            lbl_1_bss_478[2] = 0;
            lbl_1_bss_478[0]++;
            break;
        case 2:
            if (lbl_1_bss_478[2]++ >= 0x3C) {
                for (i = 0; i < lbl_1_bss_478[6]; i++) {
                    if (lbl_1_bss_18[i].active != 0 && lbl_1_bss_18[i].hit == 0) {
                        Hu3DModelAttrSet(MDL(lbl_1_bss_18[i].mdl3).id, 1);
                        Hu3DMotionSpeedSet(MDL(lbl_1_bss_18[i].mdl3).id, 0.0f);
                        Hu3DMotionTimeSet(MDL(lbl_1_bss_18[i].mdl3).id, 0.0f);
                    }
                }
                if (arg0 != 0) {
                    lbl_1_bss_478[3] = 0;
                } else {
                    fn_1_D070(0);
                    fn_1_D070(1);
                }
                lbl_1_bss_478[0] = 0;
                ret = 1;
            }
            break;
    }
    return ret;
}

s16 fn_1_84C8(void)
{
    HuVecF pos;
    s16 i;
    s16 doneCnt;
    s16 activeCnt;
    s16 ret;

    ret = 0;
    doneCnt = 0;
    i = 0;
    activeCnt = 0;
    for (i = 0; i < lbl_1_bss_478[4]; i++) {
        if (lbl_1_bss_18[i].active != 0) {
            activeCnt++;
            switch (lbl_1_bss_18[i].phase2) {
                case 0:
                    Hu3DModelAttrReset(MDL(lbl_1_bss_18[i].mdl).id, 1);
                    Hu3DModelTPLvlSet(MDL(lbl_1_bss_18[i].mdl).id, 0.0f);
                    lbl_1_bss_18[i].timer = 0;
                    lbl_1_bss_18[i].tpLvl = 0.0f;
                    lbl_1_bss_18[i].phase2++;
                case 1:
                    lbl_1_bss_18[i].tpLvl += 0.05f;
                    if (lbl_1_bss_18[i].tpLvl >= 1.0f) {
                        lbl_1_bss_18[i].tpLvl = 1.0f;
                        lbl_1_bss_18[i].phase2++;
                    }
                    Hu3DModelTPLvlSet(MDL(lbl_1_bss_18[i].mdl).id, lbl_1_bss_18[i].tpLvl);
                    break;
                case 2:
                    if (lbl_1_bss_18[i].timer++ > 0x3C) {
                        pos.x = MDL(lbl_1_bss_18[i].mdl).pos.x;
                        pos.y = MDL(lbl_1_bss_18[i].mdl).pos.y;
                        pos.z = MDL(lbl_1_bss_18[i].mdl).pos.z;
                        fn_1_28D4(&pos);
                        if (lbl_1_bss_70C.sfxFlag == 0) {
                            fn_1_37DC(0x15);
                            lbl_1_bss_70C.sfxFlag = 1;
                        }
                        lbl_1_bss_18[i].phase2++;
                    }
                    break;
                case 3:
                    lbl_1_bss_18[i].tpLvl -= 0.05f;
                    if (lbl_1_bss_18[i].tpLvl <= 0.0f) {
                        Hu3DModelAttrSet(MDL(lbl_1_bss_18[i].mdl).id, 1);
                        lbl_1_bss_18[i].tpLvl = 0.0f;
                        lbl_1_bss_18[i].phase2++;
                    }
                    Hu3DModelTPLvlSet(MDL(lbl_1_bss_18[i].mdl).id, lbl_1_bss_18[i].tpLvl);
                    break;
                case 4:
                    doneCnt++;
                    break;
            }
        }
    }
    if (doneCnt >= activeCnt) {
        ret = 1;
    }
    return ret;
}

s32 fn_1_8948(s16 no)
{
    HU3DMODEL *model;
    M553STMOTCTL *ctl;
    s32 ret;
    f32 tbl[2][3] = { { -150.0f, 0.0f, 0.0f }, { 150.0f, 0.0f, 0.0f } };
    f32 dist;
    f32 dist2;
    f32 scale;
    f32 ang;
    s16 a;
    s16 neg;

    model = &Hu3DData[MDL(no + 0x86).id];
    ctl = &W->motCtl[no];
    ret = 0;
    switch (lbl_1_bss_48C[no].phase) {
        case 0:
            lbl_1_bss_70C.moveFlag[no] = 0;
            lbl_1_bss_48C[no].moveX = tbl[no][0] - model->pos.x;
            lbl_1_bss_48C[no].moveZ = tbl[no][2] - model->pos.z;
            dist = sqrt((lbl_1_bss_48C[no].moveX * lbl_1_bss_48C[no].moveX) + (lbl_1_bss_48C[no].moveZ * lbl_1_bss_48C[no].moveZ));
            if (dist != 0.0f) {
                lbl_1_bss_70C.moveFlag[no] = 1;
                ctl->shiftMode = 1;
                ctl->shiftMotNo = 1;
                ctl->shiftStart = 0.0f;
                ctl->shiftEnd = 4.0f;
                ctl->shiftAttr = 0x40000001;
                ctl->attrReset = 0;
            }
            lbl_1_bss_70C.chaseMode = 1;
            lbl_1_bss_48C[no].phase++;
        case 1:
            lbl_1_bss_48C[no].moveX = tbl[no][0] - model->pos.x;
            lbl_1_bss_48C[no].moveZ = tbl[no][2] - model->pos.z;
            dist = sqrt((lbl_1_bss_48C[no].moveX * lbl_1_bss_48C[no].moveX) + (lbl_1_bss_48C[no].moveZ * lbl_1_bss_48C[no].moveZ));
            if (dist != 0.0f) {
                scale = 7.0f / dist;
                lbl_1_bss_48C[no].moveX *= scale;
                lbl_1_bss_48C[no].moveZ *= scale;
                if (lbl_1_bss_70C.hitCnt >= 0x1E) {
                    if (no == 0) {
                        lbl_1_bss_48C[no].moveZ += 3.0f;
                    } else {
                        lbl_1_bss_48C[no].moveZ -= 3.0f;
                    }
                    dist2 = sqrt((lbl_1_bss_48C[no].moveX * lbl_1_bss_48C[no].moveX) + (lbl_1_bss_48C[no].moveZ * lbl_1_bss_48C[no].moveZ));
                    if (dist2 != 0.0f) {
                        scale = 7.0f / dist2;
                    } else {
                        scale = 0.0f;
                    }
                    lbl_1_bss_48C[no].moveX *= scale;
                    lbl_1_bss_48C[no].moveZ *= scale;
                }
                ang = 180.0 * (acos(lbl_1_bss_48C[no].moveX / (0.1f + 7.0f)) / 3.141592653589793);
                if ((f32)(180.0 * (asin(lbl_1_bss_48C[no].moveZ / (0.1f + 7.0f)) / 3.141592653589793)) < 0.0f) {
                    ang = 360.0f - ang;
                }
                lbl_1_bss_48C[no].rotY = 90.0f + -ang;
                model->pos.x += lbl_1_bss_48C[no].moveX;
                model->pos.z += lbl_1_bss_48C[no].moveZ;
                model->rot.y = lbl_1_bss_48C[no].rotY;
                {
                    f32 dx = tbl[no][0] - model->pos.x;
                    f32 dz = tbl[no][2] - model->pos.z;

                    dist = sqrt((dx * dx) + (dz * dz));
                }
            }
            if (dist < (2.0f + 7.0f)) {
                lbl_1_bss_48C[no].rotY = model->rot.y;
                a = (s16)model->rot.y % 360;
                if (a != 0) {
                    if (a < 0) {
                        a *= -1;
                        neg = 1;
                    } else {
                        neg = 0;
                    }
                    if (a > 0xB4) {
                        a = 0x168 - a;
                    } else {
                        a *= -1;
                    }
                    if (neg != 0) {
                        a *= -1;
                    }
                }
                lbl_1_bss_70C.tween[no] = model->rot.y + (f32)a;
                if (no == 0) {
                    lbl_1_bss_70C.chaseMode = 2;
                }
                lbl_1_bss_48C[no].phase++;
            }
            break;
        case 2:
            if (fn_1_1880(&model->rot.y, 30.0f, lbl_1_bss_48C[no].rotY, lbl_1_bss_70C.tween[no]) == 0) {
                if (lbl_1_bss_70C.moveFlag[no] != 0) {
                    ctl->shiftMode = 1;
                    ctl->shiftMotNo = 0;
                    ctl->shiftStart = 0.0f;
                    ctl->shiftEnd = 4.0f;
                    ctl->shiftAttr = 0x40000001;
                    ctl->attrReset = 0;
                }
                lbl_1_bss_48C[no].pos.x = model->pos.x;
                lbl_1_bss_48C[no].pos.y = model->pos.y;
                lbl_1_bss_48C[no].pos.z = model->pos.z;
                lbl_1_bss_48C[no].phase++;
            }
            break;
        case 3:
            ret = 1;
            break;
    }
    return ret;
}

s16 fn_1_984C(void)
{
    s16 ret = 0;

    switch (lbl_1_bss_70C.mode) {
        case 0:
            fn_1_37DC(0x12);
            lbl_1_bss_70C.mode++;
        case 1:
            if (lbl_1_bss_70C.timer++ >= 0x5A) {
                lbl_1_bss_70C.timer = 0;
                lbl_1_bss_70C.mode++;
            }
            break;
        case 2:
            if (fn_1_1880(&lbl_1_bss_14->zoom, 50.0f, lbl_1_data_190[0].zoom, lbl_1_data_190[1].zoom) == 0) {
                lbl_1_bss_70C.mode++;
            }
            break;
        case 3:
            if (lbl_1_bss_70C.timer++ >= 0x32) {
                lbl_1_bss_70C.timer = 0;
                lbl_1_bss_70C.mode++;
            }
            break;
        case 4:
            if (fn_1_80B8(1) != 0) {
                lbl_1_bss_70C.mode++;
            }
            break;
        case 5:
            if (lbl_1_bss_70C.timer++ >= 0x3C) {
                lbl_1_bss_70C.mode++;
            }
            break;
        case 6:
            fn_1_36E4();
            ret = 1;
            break;
    }
    return ret;
}

s16 fn_1_9A70(void)
{
    f32 ofs[4][2] = { { 4.0f, 0.0f }, { -4.0f, 0.0f }, { 0.0f, 4.0f }, { 0.0f, -4.0f } };
    HU3DMODEL *mdl[2];
    HuVecF dbg;
    s16 ret;
    s16 total;
    s16 i;
    s16 j;
    s16 cnt;
    s16 prev;
    s32 f;

    ret = 0;
    total = 0;
    mdl[0] = &Hu3DData[MDL(0x86).id];
    mdl[1] = &Hu3DData[MDL(0x87).id];
    switch (lbl_1_bss_70C.mode) {
        case 0:
            if (fn_1_84C8() != 0) {
                lbl_1_bss_70C.mode++;
            }
            break;
        case 1:
            for (i = 0; i < 2; i++) {
                total += fn_1_8948(i);
            }
            f = fn_1_1880(&lbl_1_bss_14->rot.x, 30.0f, lbl_1_data_190[1].rot.x, -24.0f);
            f = f + fn_1_1880(&lbl_1_bss_14->zoom, 30.0f, lbl_1_data_190[1].zoom, 917.0f);
            if (total == 2 && f == 0) {
                lbl_1_bss_70C.timer = 0;
                if (lbl_1_bss_48C[0].itemCnt != 0 || lbl_1_bss_48C[1].itemCnt != 0) {
                    lbl_1_bss_70C.mode++;
                } else {
                    lbl_1_bss_70C.mode = 7;
                }
            }
            break;
        case 2:
            if (lbl_1_bss_70C.timer++ > 0x78) {
                lbl_1_bss_70C.timer = 0;
                lbl_1_bss_70C.mode++;
            }
            break;
        case 3:
            if (lbl_1_bss_48C[0].itemCnt != 0 || lbl_1_bss_48C[1].itemCnt != 0) {
                fn_1_37DC(3);
            }
            lbl_1_bss_70C.frame = 0;
            lbl_1_bss_70C.round = 0;
            lbl_1_bss_70C.tween[1] = lbl_1_bss_70C.tween[0] = lbl_1_bss_14->target.y;
            lbl_1_bss_70C.tween[3] = lbl_1_bss_70C.tween[2] = lbl_1_bss_C->target.y;
            lbl_1_bss_70C.mode++;
        case 4:
            lbl_1_bss_70C.frame++;
            for (i = 0; i < 2; i++) {
                lbl_1_bss_48C[i].pos.x = mdl[i]->pos.x;
                lbl_1_bss_48C[i].pos.y = mdl[i]->pos.y;
                lbl_1_bss_48C[i].pos.z = mdl[i]->pos.z;
                if (lbl_1_bss_70C.round < lbl_1_bss_48C[i].itemCnt) {
                    if (lbl_1_bss_70C.frame % 2 == 0) {
                        dbg.x = (-10.0f * i) + (5.0f + lbl_1_bss_48C[i].pos.x);
                        dbg.y = 85.0f;
                        dbg.z = 30.0f + lbl_1_bss_48C[i].pos.z;
                        fn_1_2B4C(&dbg);
                    }
                }
            }
            if (lbl_1_bss_70C.timer++ >= 0x14) {
                for (i = 0; i < 2; i++) {
                    for (j = 0; j < lbl_1_bss_48C[i].itemCnt; j++) {
                        dbg.x = mdl[i]->pos.x;
                        dbg.y = 100.0f + (-100.0f * j);
                        dbg.z = 15.0f + mdl[i]->pos.z;
                        MDL(lbl_1_bss_48C[i].item[j]).pos.x = dbg.x;
                        MDL(lbl_1_bss_48C[i].item[j]).pos.y = dbg.y;
                        MDL(lbl_1_bss_48C[i].item[j]).pos.z = dbg.z;
                        Hu3DModelTPLvlSet(MDL(lbl_1_bss_48C[i].item[j]).id, 1.0f);
                        if (j != 0) {
                            Hu3DModelAttrReset(MDL(lbl_1_bss_48C[i].item[j]).id, 1);
                        } else {
                            Hu3DModelAttrReset(MDL(i + 0x5D).id, 1);
                            Hu3DModelAttrReset(MDL(i + 0x5F).id, 1);
                        }
                    }
                    MDL(i + 0x5D).pos = MDL(lbl_1_bss_48C[i].item[0]).pos;
                    MDL(i + 0x5F).pos = MDL(lbl_1_bss_48C[i].item[0]).pos;
                }
                for (i = 0; i < 2; i++) {
                    if (lbl_1_bss_48C[i].itemCnt != 0) {
                        omVibrate(W->player[i].idx, 10, 12, 0);
                    }
                }
                if (lbl_1_bss_48C[0].itemCnt != 0 || lbl_1_bss_48C[1].itemCnt != 0) {
                    fn_1_37DC(4);
                }
                lbl_1_bss_70C.dirTimer = 0;
                lbl_1_bss_70C.mode++;
                lbl_1_bss_70C.timer++;
            }
            break;
        case 5:
            cnt = 0;
            for (i = 0; i < 2; i++) {
                if (lbl_1_bss_70C.round < lbl_1_bss_48C[i].itemCnt) {
                    for (j = 0; j < lbl_1_bss_48C[i].itemCnt; j++) {
                        dbg.x = 100.0f + (-100.0f * j) + (100.0f * lbl_1_bss_70C.round);
                        dbg.y = 100.0f + dbg.x;
                        total += fn_1_1880(&MDL(lbl_1_bss_48C[i].item[j]).pos.y, 30.0f, dbg.x, dbg.y);
                        MDL(lbl_1_bss_48C[i].item[j]).pos.x += ofs[lbl_1_bss_70C.dirIdx][0];
                        MDL(lbl_1_bss_48C[i].item[j]).pos.z += ofs[lbl_1_bss_70C.dirIdx][1];
                    }
                    dbg.x = 100.0f + (100.0f * lbl_1_bss_70C.round);
                    dbg.y = 100.0f + dbg.x;
                    total += fn_1_1880(&mdl[i]->pos.y, 30.0f, dbg.x, dbg.y);
                    mdl[i]->pos.x += ofs[lbl_1_bss_70C.dirIdx][0];
                    mdl[i]->pos.z += ofs[lbl_1_bss_70C.dirIdx][1];
                } else {
                    cnt++;
                }
                MDL(i + 0x5D).pos = MDL(lbl_1_bss_48C[i].item[0]).pos;
                MDL(i + 0x5F).pos = MDL(lbl_1_bss_48C[i].item[0]).pos;
            }
            lbl_1_bss_70C.dirTimer++;
            if (lbl_1_bss_70C.dirTimer >= 2) {
                lbl_1_bss_70C.dirIdx++;
                if (lbl_1_bss_70C.dirIdx >= 4) {
                    lbl_1_bss_70C.dirIdx = 0;
                }
                lbl_1_bss_70C.dirTimer = 0;
            }
            if (cnt < 2) {
                total += fn_1_1880(&lbl_1_bss_14->target.y, 30.0f, lbl_1_bss_70C.tween[0], lbl_1_bss_70C.tween[1]);
                total += fn_1_1880(&lbl_1_bss_C->target.y, 30.0f, lbl_1_bss_70C.tween[2], lbl_1_bss_70C.tween[3]);
            }
            if (total == 0) {
                lbl_1_bss_70C.timer = 0;
                lbl_1_bss_70C.round++;
                lbl_1_bss_70C.mode++;
                if (cnt < 2) {
                    if (lbl_1_bss_70C.round >= 1) {
                        lbl_1_bss_70C.tween[0] = lbl_1_bss_14->target.y;
                        lbl_1_bss_70C.tween[1] = 100.0f + lbl_1_bss_14->target.y;
                    }
                    if (lbl_1_bss_70C.round >= 5) {
                        lbl_1_bss_70C.tween[2] = lbl_1_bss_C->target.y;
                        lbl_1_bss_70C.tween[3] = 100.0f + lbl_1_bss_C->target.y;
                    }
                }
                if (lbl_1_bss_70C.round < lbl_1_bss_48C[0].itemCnt || lbl_1_bss_70C.round < lbl_1_bss_48C[1].itemCnt) {
                    fn_1_37DC(0x14);
                }
                if (cnt >= 2) {
                    lbl_1_bss_70C.mode = 7;
                }
            }
            break;
        case 6:
            lbl_1_bss_70C.frame++;
            for (i = 0; i < 2; i++) {
                if (lbl_1_bss_70C.round < lbl_1_bss_48C[i].itemCnt) {
                    if (lbl_1_bss_70C.frame % 2 == 0) {
                        dbg.x = (-10.0f * i) + (5.0f + lbl_1_bss_48C[i].pos.x);
                        dbg.y = 85.0f;
                        dbg.z = 30.0f + lbl_1_bss_48C[i].pos.z;
                        fn_1_2B4C(&dbg);
                    }
                    for (j = 0; j < lbl_1_bss_48C[i].itemCnt; j++) {
                        MDL(lbl_1_bss_48C[i].item[j]).pos.x += ofs[lbl_1_bss_70C.dirIdx][0];
                        MDL(lbl_1_bss_48C[i].item[j]).pos.z += ofs[lbl_1_bss_70C.dirIdx][1];
                    }
                    mdl[i]->pos.x += ofs[lbl_1_bss_70C.dirIdx][0];
                    mdl[i]->pos.z += ofs[lbl_1_bss_70C.dirIdx][1];
                    lbl_1_bss_70C.dirTimer++;
                    if (lbl_1_bss_70C.dirTimer >= 2) {
                        lbl_1_bss_70C.dirIdx++;
                        if (lbl_1_bss_70C.dirIdx >= 4) {
                            lbl_1_bss_70C.dirIdx = 0;
                        }
                        lbl_1_bss_70C.dirTimer = 0;
                    }
                }
                MDL(i + 0x5D).pos = MDL(lbl_1_bss_48C[i].item[0]).pos;
                MDL(i + 0x5F).pos = MDL(lbl_1_bss_48C[i].item[0]).pos;
            }
            if (lbl_1_bss_70C.timer++ >= 10) {
                for (i = 0; i < 2; i++) {
                    if (lbl_1_bss_70C.round < lbl_1_bss_48C[i].itemCnt) {
                        omVibrate(W->player[i].idx, 10, 12, 0);
                    }
                }
                if (lbl_1_bss_70C.round < lbl_1_bss_48C[0].itemCnt || lbl_1_bss_70C.round < lbl_1_bss_48C[1].itemCnt) {
                    if (lbl_1_bss_70C.round <= 0xC) {
                        fn_1_37DC(lbl_1_bss_70C.round + 4);
                    } else {
                        fn_1_37DC(0x10);
                    }
                }
                lbl_1_bss_70C.mode = 5;
            }
            break;
        case 7:
            if (lbl_1_bss_70C.timer++ > 0x3C) {
                if (lbl_1_bss_48C[0].itemCnt != 0 || lbl_1_bss_48C[1].itemCnt != 0) {
                    lbl_1_bss_70C.mode = 0xC;
                } else {
                    lbl_1_bss_70C.mode++;
                }
            }
            break;
        case 8:
            for (i = 0x61, j = 0; i <= 0x64; i++, j++) {
                if (i == 0x63) {
                    j = 0;
                }
                MDL(i).pos.x = mdl[j]->pos.x;
                MDL(i).pos.z = mdl[j]->pos.z;
                Hu3DModelAttrReset(MDL(i).id, 1);
            }
            fn_1_37DC(0x11);
            lbl_1_bss_70C.mode++;
            break;
        case 9:
            Hu3DModelScaleSet(MDL(0x61).id, lbl_1_bss_70C.scaleB, 1.0f, lbl_1_bss_70C.scaleB);
            Hu3DModelScaleSet(MDL(0x62).id, lbl_1_bss_70C.scaleB, 1.0f, lbl_1_bss_70C.scaleB);
            Hu3DModelScaleSet(MDL(0x63).id, lbl_1_bss_70C.scaleA, 1.0f, lbl_1_bss_70C.scaleA);
            Hu3DModelScaleSet(MDL(0x64).id, lbl_1_bss_70C.scaleA, 1.0f, lbl_1_bss_70C.scaleA);
            lbl_1_bss_70C.scaleB += 0.15f;
            lbl_1_bss_70C.scaleA += 0.15f;
            if (lbl_1_bss_70C.scaleB >= 1.5f) {
                W->motCtl[0].shiftMode = 1;
                W->motCtl[0].shiftMotNo = 2;
                W->motCtl[0].shiftStart = 0.0f;
                W->motCtl[0].shiftEnd = 8.0f;
                W->motCtl[0].shiftAttr = 0;
                W->motCtl[0].attrReset = 0x40000001;
                W->motCtl[1].shiftMode = 1;
                W->motCtl[1].shiftMotNo = 2;
                W->motCtl[1].shiftStart = 0.0f;
                W->motCtl[1].shiftEnd = 8.0f;
                W->motCtl[1].shiftAttr = 0;
                W->motCtl[1].attrReset = 0x40000001;
                lbl_1_bss_70C.moveFlag[1] = 0;
                lbl_1_bss_70C.mode++;
            }
            break;
        case 10:
            if (lbl_1_bss_70C.moveFlag[1]++ >= 0x19) {
                Hu3DModelShadowDispOff(lbl_1_bss_0.player[0]->actor->mdlId);
                Hu3DModelShadowDispOff(lbl_1_bss_0.player[1]->actor->mdlId);
                Hu3DMotionSpeedSet(MDL(0x86).id, 0.0f);
                Hu3DMotionSpeedSet(MDL(0x87).id, 0.0f);
                fn_1_37DC(0x13);
                lbl_1_bss_70C.mode++;
            }
            break;
        case 11:
            mdl[0]->pos.y -= 10.0f;
            mdl[1]->pos.y -= 10.0f;
            if (mdl[0]->pos.y <= -1000.0f) {
                lbl_1_bss_70C.mode++;
            }
            break;
        case 12:
            if (lbl_1_bss_70C.winner != 0) {
                total = fn_1_1880(&lbl_1_bss_14->target.x, 30.0f, lbl_1_data_190[1].target.x, mdl[lbl_1_bss_70C.winner & 1]->pos.x);
                total += fn_1_1880(&lbl_1_bss_14->target.y, 30.0f, lbl_1_bss_70C.tween[0], 70.0f + mdl[lbl_1_bss_70C.winner & 1]->pos.y);
                total += fn_1_1880(&lbl_1_bss_14->target.z, 30.0f, lbl_1_data_190[1].target.z, mdl[lbl_1_bss_70C.winner & 1]->pos.z);
                total += fn_1_1880(&lbl_1_bss_14->zoom, 30.0f, 917.0f, 500.0f);
            }
            if (total == 0) {
                lbl_1_bss_70C.mode++;
            }
            break;
        case 13:
            ret = 1;
            break;
    }
    return ret;
}

int fn_1_B5F4(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b)
{
    s16 i = 0;

    switch (b->type) {
        case 0:
            lbl_1_bss_48C[b->paramB].hitCnt++;
            lbl_1_bss_70C.hitFlag = 1;
            break;
        case 6:
            break;
    }
    return 0;
}

void fn_1_B654(MGACTOR *actor, int param)
{
    s32 tmp;

    tmp = actor->param;
}

void fn_1_B668(M553GPLAYERSTATE *player)
{
    f32 tbl[6][6] = {
        { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 50.0f },
        { 0.0f, 0.0f, 1.0f, 50.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -50.0f },
        { 0.0f, 0.0f, 1.0f, -50.0f, 0.0f, 0.0f },
        { 0.0f, -1.0f, 0.0f, 0.0f, -50.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f, 50.0f, 0.0f },
    };
    HuVecF cellPos;
    f32 xMin, xMax, zMin, zMax;
    f32 dx, dz, ex, ez, px, pz, t, d, h, s, len, sp10, sp14, sp18, yd;
    HU3DMODEL *m1;
    HU3DMODEL *m2;
    HuVecF *pos;
    M553GPLAYERSTATE *p;
    s16 spC = 0;
    s16 spA = 0;
    s16 sp8 = 0;
    s16 pl;
    s16 c;
    s16 k;
    s16 test;
    s16 side;

    for (pl = 0; pl < 2; pl++) {
        p = &lbl_1_bss_48C[pl];
        pos = &p->pos;
        for (c = 0; c < lbl_1_bss_478[6]; c++) {
            if (lbl_1_bss_18[c].vis != 0 && p->hitting == 0) {
                cellPos.x = MDL(lbl_1_bss_18[c].mdl).pos.x;
                cellPos.y = MDL(lbl_1_bss_18[c].mdl).pos.y;
                cellPos.z = MDL(lbl_1_bss_18[c].mdl).pos.z;
                test = 1;
                yd = 400.0f - (150.0f + pos->y);
                if (yd <= 0.0f && yd >= -20.0f) {
                    xMin = cellPos.x - 50.0f;
                    xMax = 50.0f + cellPos.x;
                    zMin = cellPos.z - 50.0f;
                    zMax = 50.0f + cellPos.z;
                    if (xMin <= pos->x && xMax >= pos->x && zMin <= pos->z && zMax >= pos->z && lbl_1_bss_0.player[pl]->actor->velY > 0.0f) {
                        lbl_1_bss_0.player[pl]->actor->velY = 0.0f;
                        pos->y = 250.0f;
                        lbl_1_bss_18[c].hit = 1;
                        lbl_1_bss_18[c].player = pl;
                        lbl_1_bss_18[c].vis = 0;
                        p->hitting = 1;
                        test = 0;
                    }
                }
                if (test != 0) {
                    for (k = 0; k < 4; k++) {
                        xMin = (cellPos.x + tbl[k][3]) - (50.0f * tbl[k][0]);
                        xMax = cellPos.x + tbl[k][3] + (50.0f * tbl[k][0]);
                        zMin = (cellPos.z + tbl[k][5]) - (50.0f * tbl[k][2]);
                        zMax = cellPos.z + tbl[k][5] + (50.0f * tbl[k][2]);
                        side = 0;
                        switch (k) {
                            case 0:
                                if ((pos->z - 10.0f) > zMin) {
                                    side = 1;
                                }
                                break;
                            case 1:
                                if ((pos->x - 10.0f) > xMin) {
                                    side = 1;
                                }
                                break;
                            case 2:
                                if ((10.0f + pos->z) < zMin) {
                                    side = 1;
                                }
                                break;
                            case 3:
                                if ((10.0f + pos->x) < xMin) {
                                    side = 1;
                                }
                                break;
                        }
                        if ((pos->x + 35.0f) >= xMin && (pos->x - 35.0f) <= xMax && (pos->z + 35.0f) >= zMin && (pos->z - 35.0f) <= zMax) {
                            dx = xMax - xMin;
                            dz = zMax - zMin;
                            sp18 = (dx * dx) + (dz * dz);
                            sp14 = -1.0f * ((dx * (xMax - pos->x)) + (dz * (zMax - pos->z)));
                            if (sp18 != 0.0f) {
                                t = sp14 / sp18;
                            } else {
                                t = 0.0f;
                            }
                            px = xMax + (dx * t);
                            pz = zMax + (dz * t);
                            ex = pos->x - px;
                            ez = pos->z - pz;
                            d = sqrt((ex * ex) + (ez * ez));
                            sp10 = 1.0f;
                            if (d < 35.0f) {
                                yd = 400.0f - (150.0f + pos->y);
                                if (yd <= 0.0f) {
                                    if (lbl_1_bss_0.player[pl]->actor->velY > 0.0f && yd >= -20.0f) {
                                        lbl_1_bss_0.player[pl]->actor->velY = 0.0f;
                                        pos->y = 250.0f;
                                        lbl_1_bss_18[c].hit = 1;
                                        lbl_1_bss_18[c].vis = 0;
                                        lbl_1_bss_18[c].player = pl;
                                        p->hitting = 1;
                                    } else if (side != 0) {
                                        if (d <= 1.0f) {
                                            d = 1.0f;
                                        }
                                        h = (1.0f + 35.0f) / d;
                                        pos->x = px + (sp10 * (ex * h));
                                        pos->z = pz + (sp10 * (ez * h));
                                        if (pl == 0) {
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        dx = -p->pos.x;
        dz = -p->pos.z;
        len = sqrt((dx * dx) + (dz * dz));
        if (len > 580.0f) {
            s = 580.0f / len;
            p->pos.x = -(dx * s);
            p->pos.z = -(dz * s);
        }
    }
    if (lbl_1_bss_70C.chaseMode != 0) {
        m1 = &Hu3DData[MDL(0x86).id];
        m2 = &Hu3DData[MDL(0x87).id];
        dx = m2->pos.x - m1->pos.x;
        dz = m2->pos.z - m1->pos.z;
        len = sqrt((dx * dx) + (dz * dz));
        if (len < 70.0f) {
            if (len <= 1.0f) {
                len = 1.0f;
            }
            s = 70.0f / len;
            dx *= s;
            dz *= s;
            if (lbl_1_bss_70C.chaseMode == 1) {
                m1->pos.x = m2->pos.x - dx;
                m1->pos.z = m2->pos.z - dz;
            } else {
                m2->pos.x = m1->pos.x + dx;
                m2->pos.z = m1->pos.z + dz;
            }
            lbl_1_bss_70C.hitCnt++;
            return;
        }
        lbl_1_bss_70C.hitCnt = 0;
    }
}

void fn_1_C650(s16 no)
{
    f32 spd[4] = { 6.0f, 6.5454545f, 8.0f, 10.285714f };
    f32 rad[4] = { 6.5454545f, 7.2f, 9.0f, 12.0f };
    u16 wait[4] = { 14, 11, 8, 6 };
    u16 skill[4] = { 2, 35, 40, 50 };
    u16 rate[4] = { 1, 5, 10, 100 };

    if (W->player[no].dif != 0) {
        lbl_1_bss_48C[no].comOn = 1;
        lbl_1_bss_48C[no].level = W->player[no].dif - 1;
        lbl_1_bss_48C[no].speed = spd[lbl_1_bss_48C[no].level];
        lbl_1_bss_48C[no].chaseVel = spd[lbl_1_bss_48C[no].level];
        lbl_1_bss_48C[no].chaseRadius = rad[lbl_1_bss_48C[no].level];
        lbl_1_bss_48C[no].comWait = wait[lbl_1_bss_48C[no].level];
        lbl_1_bss_48C[no].skillRate = skill[lbl_1_bss_48C[no].level];
        lbl_1_bss_48C[no].targetRate = rate[lbl_1_bss_48C[no].level];
        fn_1_D070(no);
    }
}

s32 fn_1_C92C(s16 no)
{
    lbl_1_bss_48C[no].padStkX = 0;
    lbl_1_bss_48C[no].padStkY = 0;
    lbl_1_bss_48C[no].padBtnDown = 0;
    lbl_1_bss_48C[no].padBtn = 0;
    switch (lbl_1_bss_48C[no].comPhase) {
        case 0:
            lbl_1_bss_48C[no].comPhase++;
        case 1:
            fn_1_D1B8(no);
            lbl_1_bss_48C[no].comPhase++;
        case 2:
            if (fn_1_CC28(no) != 0) {
                lbl_1_bss_48C[no].comPhase = 1;
            }
            break;
    }
    return 0;
}

s32 fn_1_CA98(s16 no)
{
    s32 ret = 0;

    switch (lbl_1_bss_48C[no].chasePhase) {
        case 0:
            MgPlayerComStkOn(lbl_1_bss_0.player[no]);
            lbl_1_bss_48C[no].chasePhase++;
            break;
        case 1:
            if (MgPlayerVecChase(lbl_1_bss_0.player[no], &lbl_1_bss_48C[no].target, lbl_1_bss_48C[no].chaseVel, lbl_1_bss_48C[no].chaseRadius) != 0) {
                lbl_1_bss_48C[no].chasePhase++;
            }
            break;
        case 2:
            MgPlayerComStkOff(lbl_1_bss_0.player[no]);
            lbl_1_bss_48C[no].chasePhase = 0;
            ret = 1;
            break;
    }
    return ret;
}

s16 fn_1_CC28(s16 no)
{
    s16 ret = 0;
    s16 done;

    switch (lbl_1_bss_48C[no].comMode) {
        case 0:
            done = 0;
            switch (lbl_1_bss_48C[no].chasePhase) {
                case 0:
                    MgPlayerComStkOn(lbl_1_bss_0.player[no]);
                    lbl_1_bss_48C[no].chasePhase++;
                    break;
                case 1:
                    if (MgPlayerVecChase(lbl_1_bss_0.player[no], &lbl_1_bss_48C[no].target, lbl_1_bss_48C[no].chaseVel, lbl_1_bss_48C[no].chaseRadius) != 0) {
                        lbl_1_bss_48C[no].chasePhase++;
                    }
                    break;
                case 2:
                    MgPlayerComStkOff(lbl_1_bss_0.player[no]);
                    lbl_1_bss_48C[no].chasePhase = 0;
                    done = 1;
                    break;
            }
            if (done != 0) {
                lbl_1_bss_48C[no].comMode++;
            } else {
                if (lbl_1_bss_48C[no].hitCnt >= 0x3C) {
                    lbl_1_bss_48C[no].hitCnt = 0;
                    lbl_1_bss_48C[no].comMode = 4;
                }
                if (lbl_1_bss_70C.hitFlag == 0) {
                    lbl_1_bss_48C[no].hitCnt = 0;
                }
            }
            break;
        case 1:
            if (lbl_1_bss_0.player[no]->mode == 0) {
                lbl_1_bss_48C[no].padBtnDown |= 0x100;
                lbl_1_bss_48C[no].comTimer = 0;
                lbl_1_bss_48C[no].comMode++;
            }
            break;
        case 2:
            lbl_1_bss_48C[no].padBtn |= 0x100;
            if (lbl_1_bss_48C[no].comTimer++ >= lbl_1_bss_48C[no].comWait) {
                lbl_1_bss_48C[no].comTimer = 0;
                lbl_1_bss_48C[no].comMode++;
            }
            break;
        case 3:
            if (lbl_1_bss_48C[no].comTimer++ >= lbl_1_bss_48C[no].comWait) {
                lbl_1_bss_48C[no].comMode = 0;
                ret = 1;
            }
            break;
        case 4:
            lbl_1_bss_48C[no].comMode = 0;
            ret = 1;
            break;
    }
    return ret;
}

void fn_1_D070(s16 no)
{
    s16 i;
    s16 j;
    s16 found;

    for (i = 0; i < 0x40; i++) {
        if (lbl_1_bss_48C[no].cellFlag[i] == 0) {
            j = 0;
            found = 0;
            while (j < lbl_1_bss_478[6]) {
                if (lbl_1_bss_18[j].active != 0 && i == lbl_1_bss_18[j].cell) {
                    found = 1;
                    break;
                }
                j++;
            }
            if (found == 0 && frandmod(100) < lbl_1_bss_48C[no].skillRate) {
                lbl_1_bss_48C[no].cellFlag[i] = 1;
            }
        }
    }
}

void fn_1_D1B8(s16 no)
{
    f32 dx;
    f32 dz;
    f32 d;
    f32 best;
    s16 tries;
    s16 cell;
    s16 i;
    s16 found;
    s16 bestCell = 0;

    tries = 0;
    lbl_1_bss_48C[no].cellFlag[0] = 1;
    lbl_1_bss_48C[no].cellFlag[7] = 1;
    lbl_1_bss_48C[no].cellFlag[56] = 1;
    lbl_1_bss_48C[no].cellFlag[63] = 1;
    if (frandmod(lbl_1_bss_48C[no].targetRate) == 0) {
        do {
            tries++;
            cell = frandmod(0x40);
        } while (lbl_1_bss_48C[no].cellFlag[cell] != 0 && tries < 100);
        lbl_1_bss_48C[no].target.x = (100.0f * (cell % 8)) - 360.0f;
        lbl_1_bss_48C[no].target.z = (100.0f * (cell / 8)) - 360.0f;
        lbl_1_bss_48C[no].cellFlag[cell] = 1;
        lbl_1_bss_48C[no].targetCell = cell;
        return;
    }
    i = 0;
    found = 0;
    best = 10000.0f;
    for (i = 0; i < 0x40; i++) {
        if (lbl_1_bss_48C[no].cellFlag[i] == 0) {
            dx = lbl_1_bss_48C[no].pos.x - ((100.0f * (i % 8)) - 360.0f);
            dz = lbl_1_bss_48C[no].pos.z - ((100.0f * (i / 8)) - 360.0f);
            d = sqrt((dx * dx) + (dz * dz));
            if (best >= d) {
                found = 1;
                best = d;
                bestCell = i;
            }
        }
    }
    if (found == 0) {
        cell = frandmod(0x40);
        lbl_1_bss_48C[no].target.x = (100.0f * (cell % 8)) - 360.0f;
        lbl_1_bss_48C[no].target.z = (100.0f * (cell / 8)) - 360.0f;
        lbl_1_bss_48C[no].cellFlag[cell] = 1;
        lbl_1_bss_48C[no].targetCell = cell;
        return;
    }
    lbl_1_bss_48C[no].target.x = (100.0f * (bestCell % 8)) - 360.0f;
    lbl_1_bss_48C[no].target.z = (100.0f * (bestCell / 8)) - 360.0f;
    lbl_1_bss_48C[no].cellFlag[bestCell] = 1;
    lbl_1_bss_48C[no].targetCell = bestCell;
}
