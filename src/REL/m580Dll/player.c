#include "types.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/charman.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/memory.h"
#include "game/pad.h"
#include "game/mg/seqman.h"
#include "humath.h"
#include "datadir_enum.h"
#include <string.h>

#include "REL/m580Dll.h"

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

double atan2(double, double);
double fmod(double, double);
double sin(double);
double cos(double);

#define M580_PLAYER_MAX 4

/* work block of one player object (0x148 bytes on GC) */
typedef struct M580PlayerWork_s {
    /* 0x00 */ s32 playerNo;
    /* 0x04 */ s32 team;
    /* 0x08 */ s32 slot; /* position index 0..3 (team * 2 + member) */
    /* 0x0C */ s32 charNo;
    /* 0x10 */ s32 motion;
    /* 0x14 */ s32 motionFrame;
    /* 0x18 */ s32 padDown;
    /* 0x1C */ s32 padHold;
    /* 0x20 */ f32 stickAng;
    /* 0x24 */ f32 stickMag;
    /* 0x28 */ s32 isCpu;
    /* 0x2C */ s32 dif;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ u8 pad34[0x34];
    /* 0x68 */ s32 state;
    /* 0x6C */ s32 timer;
    /* 0x70 */ s32 frame;
    /* 0x74 */ HuVecF pos;
    /* 0x80 */ HuVecF vel;
    /* 0x8C */ f32 rotY;
    /* 0x90 */ s32 unk90;
    /* 0x94 */ s32 busy;
    /* 0x98 */ s32 autoMove;
    /* 0x9C */ s32 defend;
    /* 0xA0 */ s32 foot;
    /* 0xA4 */ s32 unkA4;
    /* 0xA8 */ s32 unkA8;
    /* 0xAC */ s32 unkAC;
    /* 0xB0 */ s32 unkB0;
    /* 0xB4 */ s32 unkB4;
    /* 0xB8 */ s32 unkB8;
    /* 0xBC */ s32 unkBC;
    /* 0xC0 */ s32 unkC0;
    /* 0xC4 */ s32 unkC4;
    /* 0xC8 */ s32 unkC8;
    /* 0xCC */ s32 unkCC;
    /* 0xD0 */ s32 unkD0;
    /* 0xD4 */ s32 voice;
    /* 0xD8 */ s32 seId;
    /* 0xDC */ s32 unkDC;
    /* 0xE0 */ s32 shown;
    /* 0xE4 */ f32 alpha;
    /* 0xE8 */ M580_SHADOW shadow;
    /* 0x108 */ M580_BODY body;
} M580_PLAYERWORK;

typedef struct M580PlayerMotion_s {
    /* 0x00 */ u16 index;
    /* 0x04 */ f32 shift;
    /* 0x08 */ u32 attr;
} M580_PLAYERMOTION;


s32 lbl_1_data_90[8] = {
    DATANUM(DATA_mario, 0xD5), DATANUM(DATA_mario, 0xD6), DATANUM(DATA_mario, 0xD7), DATANUM(DATA_mario, 0xD8),
    DATANUM(DATA_mario, 0xD9), DATANUM(DATA_mariomot, 0x8E), DATANUM(DATA_mariomot, 0x8F), DATANUM(DATA_mariomot, 0x90),
};

M580_PLAYERMOTION lbl_1_data_B0[9] = {
    { 0, 0.2f, HU3D_MOTATTR_LOOP }, { 1, 0.2f, HU3D_MOTATTR_LOOP }, { 2, 0.1f, 0 },
    { 3, 0.1f, 0 },                 { 4, 0.2f, 0 },                 { 5, 0.15f, 0 },
    { 6, 0.2f, HU3D_MOTATTR_LOOP }, { 6, 0.2f, HU3D_MOTATTR_LOOP }, { 7, 0.2f, 0 },
};

HuVecF lbl_1_data_11C[4] = {
    { -550.0f, 0.0f, -100.0f }, { -250.0f, 0.0f, 100.0f }, { 250.0f, 0.0f, 100.0f }, { 550.0f, 0.0f, -100.0f },
};
f32 lbl_1_data_14C[4] = { 90.0f, 90.0f, 270.0f, 270.0f };
HuVecF lbl_1_data_15C[2] = { { 0.0f, -10.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
HuVecF lbl_1_data_174[2] = { { -1200.0f, 0.0f, 0.0f }, { 1200.0f, 0.0f, 0.0f } };
HuVecF lbl_1_data_18C[4] = {
    { -140.0f, 0.0f, 0.0f }, { 140.0f, 0.0f, 0.0f }, { -600.0f, 0.0f, 0.0f }, { 600.0f, 0.0f, 0.0f },
};
HuVecF lbl_1_data_1BC[2] = { { 200.0f, 0.0f, 0.0f }, { -200.0f, 0.0f, 0.0f } };
s32 lbl_1_data_1D4[2] = { DATANUM(DATA_m580, 0x0B), DATANUM(DATA_m580, 0x0C) };
s32 lbl_1_data_1DC[2] = { DATANUM(DATA_m580, 0x0D), DATANUM(DATA_m580, 0x0E) };

OMOBJ *lbl_1_bss_9C[5];
s16 lbl_1_bss_98;
s16 lbl_1_bss_94[2];
HuVecF lbl_1_bss_64[4];
f32 lbl_1_bss_54[4];
s32 lbl_1_bss_50;

void fn_1_2A10(OMOBJ *obj);
void fn_1_2EA0(OMOBJ *obj);
void fn_1_37C0(OMOBJ *obj);
void fn_1_3B30(OMOBJ *obj);
void fn_1_3E94(OMOBJ *obj);
void fn_1_4124(OMOBJ *obj, s32 seId);
void fn_1_4198(OMOBJ *obj);
void fn_1_4488(OMOBJ *obj);
void fn_1_4DDC(OMOBJ *obj);
s32 fn_1_4F58(OMOBJ *obj, HuVecF *target, f32 rot, f32 speed);
void fn_1_5A14(OMOBJ *obj);
void fn_1_5B94(OMOBJ *obj);
void fn_1_7634(OMOBJ *obj, u32 motion);
s32 fn_1_774C(OMOBJ *obj);
OMOBJ *fn_1_7818(OMOBJ *obj);
s32 fn_1_58D8(M580_BODY *self, M580_BODY *other);

void fn_1_1E14(s32 index, s16 team, s16 charNo);
void fn_1_1EC4(u32 index);
s32 fn_1_1EE4(void);
void fn_1_1F34(void);
s16 fn_1_1D40(s32 index);
s32 fn_1_1D04(void);
s32 fn_1_2004(HuVecF *pos);
s32 fn_1_2098(HuVecF *pos);
void fn_1_212C(s32 seId, HuVecF *pos);
void fn_1_2318(s32 seId, HuVecF *pos);
void fn_1_246C(s32 time);
f32 fn_1_2634(f32 from, f32 to, f32 t);
f32 fn_1_2750(f32 from, f32 to);
s32 fn_1_89A8(s32 index, f32 *pos, f32 *angle);
s32 fn_1_D054(HuVecF *center, f32 dir, f32 halfAngle, f32 range);
OMOBJ *fn_1_D30C(HuVecF *pos, HuVecF *vel);
OMOBJ *fn_1_D390(HuVecF *pos, HuVecF *vel, OMOBJ *hitId, f32 lerp);
s32 fn_1_D544(void);
s32 fn_1_D6EC(void);
s32 fn_1_D714(OMOBJ *hitId);
s32 fn_1_D7B8(void);
void fn_1_EBCC(HuVecF *pos);
void fn_1_ED34(M580_SHADOW *shadow);
s32 fn_1_11DC4(M580_BODY *body);

void fn_1_2804(OMOBJMAN *objMan)
{
    s32 count[2] = { 0, 0 };
    s32 slot[2] = { 0, 2 };
    s32 team[M580_PLAYER_MAX];
    s32 bad = 0;
    s32 i;
    OMOBJ *obj;
    s32 t;

    for (i = 0; i < M580_PLAYER_MAX; i++) {
        s32 grp = GwPlayerConf[i].grp;

        team[i] = grp;
        if (grp >= 2) {
            bad = 1;
        } else {
            count[grp]++;
        }
    }
    if (count[0] != 2 || count[1] != 2) {
        bad = 1;
    }
    if (bad != 0) {
        for (i = 0; i < M580_PLAYER_MAX; i++) {
            team[i] = i >> 1;
        }
    }
    omMakeGroupEx(objMan, 0, M580_PLAYER_MAX);
    lbl_1_bss_98 = -1;
    lbl_1_bss_94[1] = -1;
    lbl_1_bss_94[0] = -1;
    lbl_1_bss_50 = 0;
    for (i = 0; i < M580_PLAYER_MAX; i++) {
        obj = lbl_1_bss_9C[i] = omAddObjEx(objMan, 0x32, 2, 8, 0, fn_1_2A10);
        obj->work[0] = i;
        t = team[i];
        obj->work[1] = t;
        obj->work[2] = slot[t]++;
        fn_1_1E14(i, t, GwPlayerConf[i].charNo);
    }
}

void fn_1_29EC(void)
{
    CharModelKill(-1);
}

void fn_1_2A10(OMOBJ *obj)
{
    M580_PLAYERWORK *work;
    HU3DMODELID model;
    HU3DMODELID hook;
    s32 playerNo;
    s32 team;
    s32 slot;
    s16 charNo;
    s32 i;

    work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M580_PLAYERWORK), HU_MEMNUM_OVL);
    obj->data = work;
    memset(work, 0, sizeof(M580_PLAYERWORK));
    obj->stat |= 0x100;
    playerNo = obj->work[0];
    work->playerNo = playerNo;
    team = obj->work[1];
    work->team = team;
    slot = obj->work[2];
    work->slot = slot;
    charNo = GwPlayerConf[playerNo].charNo;
    work->charNo = charNo;
    work->dif = GwPlayerConf[playerNo].dif & 3;
    work->isCpu = GwPlayerConf[playerNo].type;
    work->unk30 = 0;
    model = CharModelCreate(charNo, 8);
    obj->mdlId[0] = model;
    Hu3DModelLayerSet(model, 5);
    CharEffectLayerSet(5);
    for (i = 0; i < 8; i++) {
        HU3DMOTID mot = CharMotionCreate(charNo, lbl_1_data_90[i]);

        obj->mtnId[i] = mot;
        if (i == 6) {
            Hu3DMotionAttrSet(mot, 1);
        }
    }
    CharMotionDataClose(charNo);
    if (lbl_1_bss_98 < 0) {
        lbl_1_bss_98 = Hu3DModelCreateData(DATANUM(DATA_m580, 0x0A));
        hook = lbl_1_bss_98;
    } else {
        hook = Hu3DModelLink(lbl_1_bss_98);
    }
    Hu3DModelHookSet(model, CharModelItemHookGet(charNo, 8, 2), hook);
    hook = Hu3DModelLink(lbl_1_bss_98);
    Hu3DModelHookSet(model, CharModelItemHookGet(charNo, 8, 3), hook);
    if (lbl_1_bss_94[team] < 0) {
        lbl_1_bss_94[team] = Hu3DModelCreateData(lbl_1_data_1D4[team]);
        hook = lbl_1_bss_94[team];
    } else {
        hook = Hu3DModelLink(lbl_1_bss_94[team]);
    }
    Hu3DModelHookSet(model, CharModelItemHookGet(charNo, 8, 0), hook);
    obj->mdlId[1] = Hu3DModelCreateData(lbl_1_data_1DC[team]);
    Hu3DModelAttrSet(obj->mdlId[1], HU3D_MOTATTR_LOOP);
    Hu3DModelLayerSet(obj->mdlId[1], 5);
    work->shown = 0;
    work->alpha = 0.0f;
    work->motion = -1;
    fn_1_7634(obj, 0);
    work->pos = lbl_1_data_11C[slot];
    work->vel.z = 0.0f;
    work->vel.y = 0.0f;
    work->vel.x = 0.0f;
    work->rotY = lbl_1_data_14C[slot];
    work->defend = ((work->slot ^ lbl_1_bss_50) & 1) == 0;
    work->shadow.active = 1;
    work->shadow.charNo = charNo;
    work->shadow.team = team;
    work->shadow.pos = &work->pos;
    work->shadow.ofs = lbl_1_data_15C;
    work->shadow.rot = &work->rotY;
    fn_1_ED34(&work->shadow);
    work->body.flags = 0;
    work->body.mask = 7;
    work->body.kind = 0;
    work->body.id = playerNo;
    work->body.type = 0;
    work->body.obj = obj;
    work->body.update = fn_1_5A14;
    work->body.hit = fn_1_58D8;
    work->body.u.circle.pos = &work->pos;
    work->body.u.circle.vel = &work->vel;
    work->body.u.circle.prev = *work->body.u.circle.pos;
    work->body.u.circle.radius = 40.0f;
    work->body.u.circle.weight = 0.5f;
    fn_1_11DC4(&work->body);
    work->state = 0;
    work->voice = 1;
    work->seId = -1;
    obj->objFunc = fn_1_2EA0;
}

void fn_1_2EA0(OMOBJ *obj)
{
    M580_PLAYERWORK *work = obj->data;
    M580_PLAYERWORK *mate = fn_1_7818(obj)->data;
    HuVecF start;
    HuVecF target;
    f32 stickX;
    f32 stickY;
    s32 idx;
    s16 pad;

    work->padDown = 0;
    work->padHold = 0;
    work->stickAng = 0.0f;
    work->stickMag = 0.0f;
    switch (work->state) {
    case 0:
        start = work->pos;
        if (fn_1_89A8(work->slot, &work->pos.x, &work->rotY) != 0) {
            VECSubtract(&work->pos, &start, &work->vel);
            if (work->frame == 0 || VECMag(&work->vel) < 1.0) {
                fn_1_7634(obj, 0);
            } else {
                fn_1_7634(obj, 1);
            }
            fn_1_3B30(obj);
            return;
        }
        lbl_1_bss_64[work->slot] = work->pos;
        lbl_1_bss_54[work->slot] = work->rotY;
        work->shown = 1;
        work->body.flags |= 1;
        work->state++;
        work->timer = 0;
    case 1:
        work->state++;
        work->timer = 0;
        work->defend = ((work->slot ^ lbl_1_bss_50) & 1) == 0;
        work->unk30 = 0;
    case 2:
        if (fn_1_1D04() < 2) {
            work->state = 7;
            break;
        }
        if (MgSeqModeGet() == 6) {
            work->state = 7;
            break;
        }
        idx = (work->slot & 3) ^ (lbl_1_bss_50 & 1);
        target = lbl_1_bss_64[idx];
        work->timer++;
        if (work->timer < 180.0 && fn_1_4F58(obj, &target, lbl_1_bss_54[idx], 3.0f) == 0) {
            break;
        }
        work->state++;
        work->timer = 0;
        work->autoMove = 0;
    case 3:
        if (MgSeqModeGet() == 6) {
            work->state = 7;
            break;
        }
        idx = (work->slot & 3) ^ (lbl_1_bss_50 & 1);
        if ((idx & 1) == 0) {
            target = lbl_1_data_18C[work->team];
        } else {
            target = lbl_1_bss_64[idx];
        }
        fn_1_4F58(obj, &target, lbl_1_bss_54[idx], 2.0f);
        if (fn_1_D544() != 0) {
            break;
        }
        work->autoMove = 0;
        if (work->defend != 0) {
            work->state++;
            work->timer = 0;
        } else {
            work->state += 2;
            work->timer = 0;
        }
    case 4:
        if (MgSeqModeGet() == 6) {
            work->state = 7;
            break;
        }
        if (work->isCpu != 0) {
            fn_1_5B94(obj);
        } else {
            pad = GwPlayerConf[work->playerNo].padNo;
            work->padHold = HuPadBtn[pad];
            work->padDown = HuPadBtnDown[pad];
        }
        if (fn_1_D6EC() != 0) {
            break;
        }
        work->state++;
        work->timer = 0;
    case 5:
        if (++work->timer < 18.0) {
            break;
        }
        work->state++;
        work->timer = 0;
    case 6:
        if (MgSeqModeGet() == 6) {
            work->state++;
        } else if (fn_1_D544() != 0) {
            work->state = 1;
            work->timer = 0;
            if (work->playerNo == 0) {
                lbl_1_bss_50 ^= 1;
                HuAudFXPlay(0x965);
                fn_1_246C(0xB4);
            }
            if (work->team == fn_1_D7B8()) {
                fn_1_7634(obj, 6);
                work->busy = 1;
                fn_1_4124(obj, 0x1F8);
            }
        } else if (work->isCpu != 0) {
            fn_1_5B94(obj);
        } else {
            pad = GwPlayerConf[work->playerNo].padNo;
            stickX = (s8)HuPadStkX[pad];
            stickY = -(s8)HuPadStkY[pad];
            work->padHold = HuPadBtn[pad];
            work->padDown = HuPadBtnDown[pad];
            work->stickAng = 180.0 * (atan2(stickX, stickY) / M_PI);
            work->stickMag = 0.017857144f * HuMagPoint2D(stickX, stickY);
            if (work->stickMag > 1.0f) {
                work->stickMag = 1.0f;
            }
        }
        break;
    case 7:
        if (MgSeqModeGet() == 5) {
            VECScale(&work->vel, &work->vel, 0.98f);
            break;
        }
        work->state++;
        work->timer = 0;
        work->unk90 = 1;
        work->shown = 0;
    case 8:
        if (++work->timer >= 10) {
            fn_1_7634(obj, 0);
            work->state = 0;
            obj->objFunc = fn_1_37C0;
        }
        break;
    }
    if (work->motion == 6 && work->motionFrame > 60.0) {
        work->busy = 0;
    }
    fn_1_4488(obj);
    fn_1_4198(obj);
    VECAdd(&work->pos, &work->vel, &work->pos);
    fn_1_3B30(obj);
}

void fn_1_37C0(OMOBJ *obj)
{
    M580_PLAYERWORK *work = obj->data;
    s32 t;

    work->padDown = 0;
    work->padHold = 0;
    work->stickAng = 0.0f;
    work->stickMag = 0.0f;
    switch (work->state) {
    case 0:
        fn_1_7634(obj, 0);
        work->unk90 = 1;
        work->busy = 0;
        work->state++;
    case 1:
        VECScale(&work->vel, &work->vel, 0.98f);
        if (MgSeqModeGet() == 6) {
            break;
        }
        work->state++;
        work->timer = 0;
    case 2:
        t = work->timer++;
        if ((work->pos.x > -800.0f && work->pos.x < 800.0f) || t > 60.0) {
            work->body.flags &= ~1;
        }
        if (t > 120.0) {
            work->body.flags |= 1;
        }
        if (fn_1_4F58(obj, &lbl_1_data_11C[work->slot], 0.0f, 1.5f) != 0 || !(t < 180.0)) {
            work->state++;
            work->body.flags |= 1;
            fn_1_1EC4(work->playerNo);
        }
        break;
    case 3:
        fn_1_4F58(obj, &lbl_1_data_11C[work->slot], 0.0f, 1.0f);
        if (MgSeqModeGet() == 8 && fn_1_1EE4() != 0) {
            work->voice = 0;
            CharModelVoiceVolSet(work->charNo, 0x7F);
            if (fn_1_1D40(work->playerNo) >= 0) {
                fn_1_7634(obj, 7);
                CharFXPlayPan(work->charNo, 0x1FA, fn_1_2004(&work->pos));
            } else {
                fn_1_7634(obj, 8);
                fn_1_1F34();
            }
            work->state++;
        }
        break;
    case 4:
        if (fn_1_774C(obj) != 0) {
            work->state++;
        }
        break;
    }
    VECAdd(&work->pos, &work->vel, &work->pos);
    fn_1_3B30(obj);
}

void fn_1_3B30(OMOBJ *obj)
{
    M580_PLAYERWORK *work = obj->data;
    f32 x;
    f32 z;
    f32 ang;
    f32 radius;

    work->frame++;
    work->motionFrame++;
    if (work->busy != 0 && fn_1_774C(obj) != 0) {
        work->busy = 0;
    }
    if (fn_1_D30C(NULL, NULL) == obj) {
        work->body.flags |= 0x10;
    } else {
        work->body.flags &= ~0x10;
    }
    if (work->shown != 0) {
        if ((work->alpha += 0.05) > 1.0) {
            work->alpha = 1.0f;
        }
    } else {
        if ((work->alpha -= 0.05) <= 0.0) {
            work->alpha = 0.0f;
        }
    }
    if (fn_1_D6EC() != 0 && work->defend == 0) {
        radius = 350.0f;
        x = work->pos.x;
        z = work->pos.z;
        if (HuMagPoint2D(x, z) < radius) {
            ang = 180.0 * (atan2(x, z) / M_PI);
            work->pos.x = radius * sin(M_PI * ang / 180.0);
            work->pos.z = radius * cos(M_PI * ang / 180.0);
        }
    }
    fn_1_3E94(obj);
}

void fn_1_3E94(OMOBJ *obj)
{
    M580_PLAYERWORK *work = obj->data;
    HuVecF footL;
    HuVecF footR;
    char *hookL;
    char *hookR;
    HU3DMODELID model;
    s32 foot;

    CharModelVoicePanSet(work->charNo, fn_1_2004(&work->pos));
    if (work->voice != 0) {
        CharModelVoiceVolSet(work->charNo, fn_1_2098(&work->pos));
    }
    if (((work->frame ^ work->playerNo) & 1) != 0 && VECMag(&work->vel) > 2.0) {
        s32 charNo = work->charNo;

        model = obj->mdlId[0];
        hookL = CharModelItemHookGet(charNo, 8, 2);
        hookR = CharModelItemHookGet(charNo, 8, 3);
        Hu3DModelObjPosGet(model, hookL, &footL);
        Hu3DModelObjPosGet(model, hookR, &footR);
        if (footL.y < footR.y) {
            fn_1_EBCC(&footL);
            foot = 1;
        } else {
            fn_1_EBCC(&footR);
            foot = 0;
        }
        if (work->frame > 30.0 && work->foot != foot) {
            fn_1_212C(0x961, &work->pos);
        }
        work->foot = foot;
    }
    if (MgSeqModeGet() == 5) {
        if (work->unkC0 != 0) {
            work->unkC4 = 0;
        } else if (++work->unkC4 < 0x12) {
            work->unkC0 = work->unkBC;
        }
        if (work->unkBC == 0 && work->unkC0 != 0) {
            fn_1_212C(0x967, &work->pos);
        }
        work->unkBC = work->unkC0;
        work->unkC0 = 0;
        if (work->unkCC != 0) {
            work->unkD0 = 0;
        } else if (++work->unkD0 < 0x12) {
            work->unkCC = work->unkC8;
        }
        if (work->unkC8 == 0 && work->unkCC != 0) {
            fn_1_212C(0x968, &work->pos);
        }
        work->unkC8 = work->unkCC;
        work->unkCC = 0;
    }
    if (work->seId != -1) {
        if (HuAudFXStatusGet(work->seId) == 2) {
            fn_1_2318(work->seId, &work->pos);
            return;
        }
        work->seId = -1;
    }
}

void fn_1_4124(OMOBJ *obj, s32 seId)
{
    M580_PLAYERWORK *work = obj->data;
    s16 pan = fn_1_2004(&work->pos);

    work->seId = CharFXPlayVolPan(work->charNo, seId, fn_1_2098(&work->pos), pan);
}
