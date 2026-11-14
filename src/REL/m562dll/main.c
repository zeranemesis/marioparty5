/* m562dll/main: the Bowser boss battle's owner object - setup/teardown of every sub-object, the per-frame
   update, the phase switches and a small immediate-mode debug-line drawer. */

#include <math.h>
#include <string.h>

#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"
#include "game/mgdata.h"
#include "game/sprite.h"
#include "game/window.h"
#include "game/wipe.h"

#include "REL/m562dll.h"

/* sprite (HUD) objects: the sprite unit declares them after this block is read */
extern void fn_1_23ECC(M562MainSprite *self, s32 kind, s16 life, s32 flag, M562Work *owner);
extern void fn_1_24178(M562MainSprite *self);
extern void fn_1_241EC(M562MainSprite *self);

void fn_1_10A4(M562MainTriple *self, M562Work *owner);
void fn_1_12E0(M562MainTriple *self);
s32 fn_1_1330(M562Work *work, s32 arg);
static s32 fn_1_14A4(M562Work *work);
void fn_1_21E4(s32 arg0, Mtx mtx);
s32 fn_1_21DC(void);

/* the debug geometry: line strips of 2, 4 (triangle) and 5 (quad) points */
typedef struct M562DbgLine {
    Vec p[2];
} M562DbgLine;

typedef struct M562DbgTri {
    Vec p[3];
} M562DbgTri;

typedef struct M562DbgQuad {
    Vec p[4];
} M562DbgQuad;

static const Vec lbl_1_rodata_18 = { 0.0f, 2250.0f, -500.0f };
static const Vec lbl_1_rodata_24 = { 0.0f, 2300.0f, -1100.0f };
static const Vec lbl_1_rodata_30 = { 0.0f, 1000.0f, 1400.0f };
static const Vec lbl_1_rodata_3C = { 0.0f, 100.0f, -200.0f };
static const Vec lbl_1_rodata_48 = { 0.0f, 0.0f, -1.0f };

static Vec lbl_1_data_30 = { 0.0f, 3000.0f, 100.0f };
static Vec lbl_1_data_3C = { 0.0f, 1.0f, 0.0f };
static Vec lbl_1_data_48 = { 0.0f, 0.0f, 0.0f };
u16 lbl_1_data_54[6] = { 2, 5, 0xF, 8, 0, 0 };

static u16 lbl_1_bss_8;
static u16 lbl_1_bss_A;
static u16 lbl_1_bss_C;
static GXColor lbl_1_bss_E[0x100];
static GXColor lbl_1_bss_40E[0x100];
static GXColor lbl_1_bss_80E[0x100];
static M562DbgQuad lbl_1_bss_C10[0x100];
static M562DbgTri lbl_1_bss_3C10[0x100];
static M562DbgLine lbl_1_bss_6010[0x100];

void fn_1_AC4(M562Work *work, OMOBJMAN *manager)
{
    Vec heading;
    Vec v;
    HuVec2f size;
    CAMERA *cam0;
    CAMERA *cam1;
    s16 diff;

    memset(work, 0, sizeof(M562Work));
    work->manager = manager;
    work->timeLeft = 0x5DC;
    work->mesId = -1;
    work->bgmId = -1;
    work->seId = -1;
    switch (GwPlayerConf[0].dif) {
    case 3:
        diff = 0;
        break;
    case 2:
        diff = 1;
        break;
    case 1:
        diff = 2;
        break;
    default:
        diff = 3;
        break;
    }
    work->diff = diff;
    fn_1_2253C(&work->light, (LightStage *)work);
    cam0 = &work->camera[0];
    cam1 = &work->camera[1];
    fn_1_20CE0(cam1, 1, 1, (CAMSTAGE *)work);
    v = lbl_1_rodata_18;
    cam1->pos = v;
    cam1->posGoal = cam1->pos;
    v = lbl_1_rodata_24;
    cam1->tgt = v;
    cam1->tgtGoal = cam1->tgt;
    fn_1_20CE0(cam0, 2, 1, (CAMSTAGE *)work);
    v = lbl_1_rodata_30;
    cam0->pos = v;
    cam0->posGoal = cam0->pos;
    v = lbl_1_rodata_3C;
    cam0->tgt = v;
    cam0->tgtGoal = cam0->tgt;
    heading = lbl_1_rodata_48;
    fn_1_8C4(&cam0->pos, &heading, 30000.0f, 1000.0f);
    cam0->state = 0;
    cam0->mode = 2;
    fn_1_6B28(&work->player, 0, work);
    CharEffectLayerSet(4);
    fn_1_3808(&work->back, work);
    fn_1_5688(&work->stage, (M562StageParent *)work);
    Hu3DShadowCreate(45.0f, 20.0f, 50000.0f);
    Hu3DShadowTPLvlSet(0.5f);
    Hu3DShadowSizeSet(0xF0);
    Hu3DShadowPosSet((HuVecF *)&lbl_1_data_30, (HuVecF *)&lbl_1_data_3C, (HuVecF *)&lbl_1_data_48);
    fn_1_13CFC(&work->fire, work);
    work->fire.run = 1;
    fn_1_B2E0(&work->koopa, work);
    fn_1_18E88(&work->fireRing, work);
    work->fireRing.active = 0;
    Hu3DLayerHookSet(0xF, fn_1_508C);
    work->fbTex = HuMemDirectMallocNum(HEAP_MODEL, GXGetTexBufferSize(0x140, 0xF0, 4, 0, 0), HU_MEMNUM_OVL);
    work->fbAnim = HuSprAnimRead(HuDataSelHeapReadNum(0x75006D, HU_MEMNUM_OVL, HEAP_MODEL));
    fn_1_177EC(&work->mecha, work);
    work->mecha.active = 0;
    fn_1_23ECC(&work->sprite[0], work->player.charNo, work->player.hp, 0, work);
    fn_1_23ECC(&work->sprite[1], 0xF, work->boss.life, 1, work);
    work->sprite[0].active = 1;
    work->sprite[1].active = 0;
    HuWinInit(1);
    HuWinMesMaxSizeBetGet(&size, 0x110000, 0x110001);
    work->winId = HuWinExCreate(-10000.0f, 350.0f, (s16)size.x, (s16)size.y, -1);
    fn_1_10A4(&work->triple, work);
    fn_1_2DB24();
    fn_1_2EB08();
    Hu3DBGColorSet(0, 0, 0);
    WipeCreate(1, 5, 0x3C);
    Hu3DNoSyncSet(1);
    fn_1_1330(work, 0);
    m562CtrlNo = 1;
}

void fn_1_FB0(M562Work *work)
{
    fn_1_2DB74();
    fn_1_12E0(&work->triple);
    HuWinExClose(work->winId);
    HuWinExKill(work->winId);
    fn_1_24178(&work->sprite[0]);
    fn_1_17984(&work->mecha);
    if (work->bossMode == 0) {
        fn_1_140E4(&work->fire);
        fn_1_B4F8(&work->koopa);
        fn_1_18F5C(&work->fireRing);
        HuMemDirectFree(work->fbTex);
        work->fbTex = NULL;
    } else {
        fn_1_10018(&work->boss);
    }
    fn_1_6FBC(&work->player);
    fn_1_5BE4(&work->stage);
    fn_1_41B4(&work->back);
    fn_1_2102C(&work->camera[1]);
    fn_1_2102C(&work->camera[0]);
    fn_1_22620(&work->light);
    if (work->mesId >= 0) {
        GameMesKill((GAMEMESID)work->mesId);
    }
    GameMesClose();
    memset(work, 0, sizeof(M562Work));
    HuAudAllStop();
}

void fn_1_10A4(M562MainTriple *self, M562Work *owner)
{
    HU3DMODELID model;
    HU3DMODELID model2;

    memset(self, 0, sizeof(M562MainTriple));
    self->owner = owner;
    model = Hu3DModelCreateData(0x750021);
    self->model = model;
    Hu3DModelScaleSet(model, 15.0f, 15.0f, 15.0f);
    Hu3DModelPosSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(model, owner->camera[0].camMask);
    Hu3DModelLayerSet(model, 5);
    Hu3DModelAttrSet(model, HU3D_ATTR_DISPOFF);
    self->motion = Hu3DJointMotionData(model, 0x750056);
    Hu3DMotionSpeedSet(model, 9.16f);
    Hu3DMotionSet(model, self->motion);
    Hu3DModelAttrSet(model, HU3D_MOTATTR_LOOP);
    model2 = Hu3DModelCreateData(0x750027);
    self->model2 = model2;
    Hu3DModelScaleSet(model2, 1.0f, 1.0f, 1.0f);
    Hu3DModelPosSet(model2, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(model2, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(model2, owner->camera[0].camMask);
    Hu3DModelLayerSet(model2, 5);
    Hu3DModelAttrSet(model2, HU3D_ATTR_DISPOFF);
}

void fn_1_12E0(M562MainTriple *self)
{
    Hu3DModelKill(self->model2);
    Hu3DMotionKill(self->motion);
    Hu3DModelKill(self->model);
    memset(self, 0, sizeof(M562MainTriple));
}

s32 fn_1_1330(M562Work *work, s32 arg)
{
    s32 done;

    done = fn_1_14A4(work);
    if (done != 0 && work->step == 4) {
        work->winner = 0;
    }
    fn_1_22690();
    fn_1_2105C(&work->camera[0]);
    fn_1_2105C(&work->camera[1]);
    fn_1_427C(&work->back);
    fn_1_5CAC(&work->stage);
    if (work->bossMode == 0) {
        fn_1_14190(&work->fire);
        fn_1_B578(&work->koopa);
        fn_1_18FC8(&work->fireRing);
    } else {
        fn_1_11054(&work->boss);
    }
    fn_1_17A28(&work->mecha);
    fn_1_7174(&work->player);
    work->sprite[0].life = work->player.hp;
    work->sprite[1].life = work->boss.life;
    fn_1_241EC(&work->sprite[0]);
    fn_1_241EC(&work->sprite[1]);
    if (work->player.hp <= 0) {
        M562TimerKill(work);
        work->winner = 1;
        done = 1;
    }
    if (MgSeqModeGet() == MGSEQ_MODE_MAIN && work->timeLeft != 0) {
        work->timeLeft--;
    }
    work->frame++;
    return done;
}

static s32 fn_1_14A4(M562Work *work)
{
    FireRingMgr *ring = &work->fireRing;
    Koopa *koopa = &work->koopa;
    s32 done = 0;

    switch (work->mode) {
    case 5:
        break;
    case 1:
        if (work->fire.count <= 0) {
            done = 1;
        }
        break;
    case 2:
        if (ring->state >= 2 && work->timer == NULL) {
            work->timer = MgTimerCreate(0);
            MgTimerParamSet(work->timer, 0x708, 0, 0);
            MgTimerPosSet(work->timer, 525.0f, 64.0f);
        }
        if (ring->state == 4) {
            M562TimerKill(work);
            if (ring->timer < 0x3C) {
                f32 value;

                if (ring->timer == 0 && ring->sound >= 0) {
                    fn_1_864(ring->sound, 1000);
                }
                value = 40.0f + 1000.0f * ((f32)ring->timer / 60.0f);
                ring->ring[2].pos.y = value;
                ring->ring[1].pos.y = value;
                ring->ring[0].pos.y = value;
                ring->timer++;
            } else if (ring->sound >= 0) {
                fn_1_83C(ring->sound);
                ring->sound = -1;
            }
            done = 1;
        }
        break;
    case 3:
        if (koopa->state == 3 && koopa->sub >= 7) {
            done = 1;
        }
        break;
    case 4:
        if (work->boss.life <= 0) {
            done = 1;
        }
        break;
    }
    return done;
}

/* phase switch: show/hide the sub-objects for battle part `mode` (0..3) */
void fn_1_16F0(M562EvtWork *evt, s16 mode)
{
    M562Work *work = (M562Work *)evt;
    M562Back *back = &work->back;
    CAMERA *cam0 = &work->camera[0];

    if (work->bossMode == 0 && mode == 3) {
        fn_1_140E4(&work->fire);
        fn_1_B4F8(&work->koopa);
        fn_1_18F5C(&work->fireRing);
        HuMemDirectFree(work->fbTex);
        work->fbTex = NULL;
        fn_1_F784(&work->boss, work);
        work->bossMode = 1;
    } else if (work->bossMode != 0 && mode != 3) {
        fn_1_10018(&work->boss);
        fn_1_13CFC(&work->fire, work);
        work->fire.run = 1;
        fn_1_B2E0(&work->koopa, work);
        fn_1_18E88(&work->fireRing, work);
        work->fireRing.active = 0;
        Hu3DLayerHookSet(0xF, fn_1_508C);
        work->fbTex = HuMemDirectMallocNum(HEAP_MODEL, GXGetTexBufferSize(0x140, 0xF0, 4, 0, 0), HU_MEMNUM_OVL);
        work->fbAnim = HuSprAnimRead(HuDataSelHeapReadNum(0x75006D, HU_MEMNUM_OVL, HEAP_MODEL));
        work->bossMode = 0;
    }
    switch (mode) {
    case 0:
        m562CtrlNo = 1;
        work->fire.run = 1;
        work->fire.enable = 0;
        work->fireRing.active = 0;
        work->fireRing.moveOn = 0;
        work->mecha.active = 0;
        work->koopa.visibleReq = 1;
        work->koopa.state = 1;
        work->koopa.sub = 0;
        work->koopa.active = 0;
        work->boss.visibleReq = 0;
        work->boss.active = 0;
        cam0->state = 0;
        cam0->mode = 2;
        work->sprite[0].active = 1;
        work->sprite[1].active = 0;
        Hu3DModelAttrReset(back->modelC, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrReset(back->modelE, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(back->model10, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(back->model12, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(back->model14, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrReset(back->model1A, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrReset(back->model1C, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(back->model20, HU3D_ATTR_DISPOFF);
        MgSeqMaxTimeSet(0x3C);
        break;
    case 1:
        m562CtrlNo = 2;
        work->fire.run = 0;
        work->fire.enable = 0;
        work->fireRing.moveOn = 0;
        work->mecha.active = 0;
        work->koopa.visibleReq = 1;
        work->koopa.state = 2;
        work->koopa.sub = 0;
        work->koopa.active = 0;
        work->boss.visibleReq = 0;
        work->boss.active = 0;
        cam0->state = 0;
        cam0->mode = 2;
        work->sprite[0].active = 1;
        work->sprite[1].active = 0;
        M562TimerKill(work);
        Hu3DModelAttrReset(back->modelC, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrReset(back->modelE, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(back->model10, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(back->model12, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(back->model14, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrReset(back->model1A, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrReset(back->model1C, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(back->model20, HU3D_ATTR_DISPOFF);
        MgSeqMaxTimeSet(0);
        break;
    case 2:
        m562CtrlNo = 3;
        work->fire.run = 0;
        work->fire.enable = 0;
        work->fireRing.active = 0;
        work->fireRing.moveOn = 0;
        work->mecha.active = 1;
        work->koopa.visibleReq = 1;
        work->koopa.state = 3;
        work->koopa.sub = 0;
        work->koopa.active = 0;
        work->boss.visibleReq = 0;
        work->boss.active = 0;
        cam0->state = 0;
        cam0->mode = 2;
        work->sprite[0].active = 1;
        work->sprite[1].active = 0;
        Hu3DModelAttrReset(back->modelC, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrReset(back->modelE, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(back->model10, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(back->model12, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(back->model14, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrReset(back->model1A, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrReset(back->model1C, HU3D_ATTR_DISPOFF);
        MgSeqMaxTimeSet(0x12C);
        M562TimerKill(work);
        break;
    case 3:
        m562CtrlNo = 4;
        work->fire.run = 0;
        work->fire.enable = 0;
        work->fireRing.active = 0;
        work->fireRing.moveOn = 0;
        work->mecha.active = 1;
        work->koopa.visibleReq = 0;
        work->koopa.active = 0;
        work->boss.visibleReq = 1;
        work->boss.active = 0;
        cam0->state = 0;
        cam0->mode = 3;
        work->sprite[0].active = 1;
        work->sprite[1].active = 1;
        Hu3DModelAttrSet(back->modelC, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(back->modelE, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrReset(back->model10, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrReset(back->model12, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrReset(back->model14, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(back->model1A, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(back->model1C, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(back->model20, HU3D_ATTR_DISPOFF);
        MgSeqMaxTimeSet(0x12C);
        break;
    }
}

void fn_1_1CB0(M562Work *work, s16 mode, s32 on)
{
    switch (mode) {
    case 0:
        work->fire.enable = on;
        work->fireRing.moveOn = 0;
        work->koopa.active = on;
        work->boss.active = 0;
        break;
    case 1:
        work->fire.enable = 0;
        work->fireRing.moveOn = on;
        work->koopa.active = on;
        work->boss.active = 0;
        break;
    case 2:
        work->fire.enable = 0;
        work->fireRing.moveOn = 0;
        work->koopa.active = on;
        work->boss.active = 0;
        break;
    case 3:
        work->fire.enable = 0;
        work->fireRing.moveOn = 0;
        work->koopa.active = 0;
        work->boss.active = on;
        break;
    }
}

/* end of the battle: put every sub-object into its closing state */
void fn_1_1D9C(M562EvtWork *evt)
{
    M562Work *work = (M562Work *)evt;
    KoopaBoss *boss = &work->boss;
    s32 i;

    switch (work->mode) {
    case 1:
        for (i = 0; i < 0x23; i++) {
            Fire *fire = &work->fire.fire[i];

            if (fire->state < 6) {
                fn_1_162A0(fire);
            }
        }
        break;
    case 2:
        work->fireRing.state = 4;
        break;
    case 3:
        work->koopa.sub = 1;
        work->koopa.reqMotion = 0;
        work->koopa.loopMotion = 1;
        break;
    case 4:
        Hu3DModelAttrSet(boss->glowModel, HU3D_ATTR_DISPOFF);
        if (boss->orbState != 1 && boss->orbState == 2) {
            boss->orbTimer = 0;
            boss->orbState = 5;
        }
        for (i = 0; i < 5; i++) {
            KbBombSlot *slot = &boss->bombs.slot[i];

            if (slot->active != 0 && slot->state < 4) {
                slot->unk8 = 0;
                slot->state = 8;
            } else if (slot->active != 0 && slot->state >= 4 && slot->state < 8) {
                slot->unk8 = 0;
                slot->state = 9;
            }
        }
        if (boss->sndHandle >= 0) {
            fn_1_83C(boss->sndHandle);
            boss->sndHandle = -1;
        }
        break;
    }
}

s32 fn_1_1F54(M562Work *work)
{
    M562EvtWork *evt = (M562EvtWork *)work;
    s32 ret = 1;

    switch (work->phase) {
    case 0:
        ret = fn_1_2446C(evt);
        break;
    case 1:
        ret = fn_1_252B4(evt);
        break;
    case 2:
        ret = fn_1_260B8(evt);
        break;
    case 3:
        ret = fn_1_27950(evt);
        break;
    case 4:
        ret = fn_1_280C8(evt);
        break;
    case 5:
        ret = fn_1_29DF8(evt);
        break;
    }
    if (ret != 0) {
        work->phase++;
    }
    return ret;
}

s32 fn_1_2034(M562Work *work)
{
    work->player.inputOn = 1;
    work->player.state = 1;
    switch (work->mode) {
    case 1:
        work->fire.enable = 1;
        work->fireRing.moveOn = 0;
        work->koopa.active = 1;
        work->boss.active = 0;
        break;
    case 2:
        work->fire.enable = 0;
        work->fireRing.moveOn = 1;
        work->koopa.active = 1;
        work->boss.active = 0;
        break;
    case 3:
        work->fire.enable = 0;
        work->fireRing.moveOn = 0;
        work->koopa.active = 1;
        work->boss.active = 0;
        break;
    case 4:
        work->fire.enable = 0;
        work->fireRing.moveOn = 0;
        work->koopa.active = 0;
        work->boss.active = 1;
        break;
    }
    work->winner = 2;
    return 1;
}

s32 fn_1_21DC(void)
{
    return 1;
}

/* draw and clear the queued debug lines, triangles and quads (never called in the shipped game) */
void fn_1_21E4(s32 arg0, Mtx mtx)
{
    s32 i;
    s32 j;

    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT0, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
    GXSetAlphaUpdate(GX_TRUE);
    for (i = 0; i < lbl_1_bss_8; i++) {
        GXBegin(GX_LINESTRIP, GX_VTXFMT0, 2);
        for (j = 0; j < 2; j++) {
            GXPosition3f32(lbl_1_bss_6010[i].p[j].x, lbl_1_bss_6010[i].p[j].y, lbl_1_bss_6010[i].p[j].z);
            GXColor4u8(lbl_1_bss_80E[i].r, lbl_1_bss_80E[i].g, lbl_1_bss_80E[i].b, lbl_1_bss_80E[i].a);
        }
    }
    for (i = 0; i < lbl_1_bss_A; i++) {
        GXBegin(GX_LINESTRIP, GX_VTXFMT0, 4);
        for (j = 0; j < 4; j++) {
            Vec *p = &lbl_1_bss_3C10[i].p[j % 3];

            GXPosition3f32(p->x, p->y, p->z);
            GXColor4u8(lbl_1_bss_40E[i].r, lbl_1_bss_40E[i].g, lbl_1_bss_40E[i].b, lbl_1_bss_40E[i].a);
        }
    }
    for (i = 0; i < lbl_1_bss_C; i++) {
        GXBegin(GX_LINESTRIP, GX_VTXFMT0, 5);
        for (j = 0; j < 5; j++) {
            Vec *p = &lbl_1_bss_C10[i].p[j % 4];

            GXPosition3f32(p->x, p->y, p->z);
            GXColor4u8(lbl_1_bss_E[i].r, lbl_1_bss_E[i].g, lbl_1_bss_E[i].b, lbl_1_bss_E[i].a);
        }
    }
    lbl_1_bss_C = 0;
    lbl_1_bss_A = 0;
    lbl_1_bss_8 = 0;
}

void fn_1_2FF4(Vec *a, Vec *b, GXColor *color)
{
    if (lbl_1_bss_8 < 0x100) {
        lbl_1_bss_6010[lbl_1_bss_8].p[0] = *a;
        lbl_1_bss_6010[lbl_1_bss_8].p[1] = *b;
        lbl_1_bss_80E[lbl_1_bss_8] = *color;
        lbl_1_bss_8++;
    }
}

void fn_1_30CC(Vec *center, GXColor *color, f32 size)
{
    Vec p0;
    Vec p1;
    Vec p2;

    if (lbl_1_bss_A < 0x100) {
        p0.x = center->x;
        p0.y = center->y;
        p0.z = center->z - size;
        p1.x = center->x + size;
        p1.y = center->y;
        p1.z = center->z + size;
        p2.x = center->x - size;
        p2.y = center->y;
        p2.z = center->z + size;
        lbl_1_bss_3C10[lbl_1_bss_A].p[0] = p0;
        lbl_1_bss_3C10[lbl_1_bss_A].p[1] = p1;
        lbl_1_bss_3C10[lbl_1_bss_A].p[2] = p2;
        lbl_1_bss_40E[lbl_1_bss_A] = *color;
        lbl_1_bss_A++;
    }
}

void fn_1_323C(Vec *a, Vec *b, Vec *c, Vec *d, GXColor *color)
{
    if (lbl_1_bss_C < 0x100) {
        lbl_1_bss_C10[lbl_1_bss_C].p[0] = *a;
        lbl_1_bss_C10[lbl_1_bss_C].p[1] = *b;
        lbl_1_bss_C10[lbl_1_bss_C].p[2] = *c;
        lbl_1_bss_C10[lbl_1_bss_C].p[3] = *d;
        lbl_1_bss_E[lbl_1_bss_C] = *color;
        lbl_1_bss_C++;
    }
}

/* the shortest-arc quaternion that turns direction `from` onto `to` */
void fn_1_337C(Quaternion *q, Vec *from, Vec *to)
{
    Vec axis;
    Quaternion r;
    f32 dot;
    f32 absDot;
    f32 s;

    PSVECCrossProduct(from, to, &axis);
    dot = PSVECDotProduct(from, to);
    if (dot >= 0.0f) {
        absDot = dot;
    } else {
        absDot = -dot;
    }
    if (absDot < 0.9999999f) {
        s = sqrtf(2.0f * (1.0f + dot));
        r.x = axis.x / s;
        r.y = axis.y / s;
        r.z = axis.z / s;
        r.w = s * 0.5f;
    } else if (dot < 0.0f) {
        r.x = 0.0f;
        r.y = 1.0f;
        r.z = 0.0f;
        r.w = 0.0f;
    } else {
        r.x = 0.0f;
        r.y = 0.0f;
        r.z = 0.0f;
        r.w = 1.0f;
    }
    q->x = r.x;
    q->y = r.y;
    q->z = r.z;
    q->w = r.w;
}

/* from + t * (shortest signed angular difference to `to`), in degrees [0, 360) */
f32 fn_1_36EC(f32 from, f32 to, f32 t)
{
    f32 diff;
    f32 res;

    diff = (f32)fmod((f64)(to - from), 360.0);
    if (diff < 0.0f) {
        diff += 360.0f;
    }
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    res = (f32)fmod((f64)(from + t * diff), 360.0);
    if (res < 0.0f) {
        res += 360.0f;
    }
    return res;
}
