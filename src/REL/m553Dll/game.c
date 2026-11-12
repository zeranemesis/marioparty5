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

    memset(&lbl_1_bss_70C, 0, 0xDC);
    memset(&lbl_1_bss_48C, 0, 0x280);
    memset(&lbl_1_bss_478, 0, 0x12);
    memset(&lbl_1_bss_18, 0, 0x460);
    memset(&lbl_1_bss_0, 0, 0xC);
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
