#include "types.h"
#include "game/audio.h"
#include "game/charman.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mg/seqman.h"
#include "game/object.h"
#include "game/pad.h"
#include "REL/m514Dll/m514types.h"

#include <math.h>
#include <string.h>

#define M514_RAD(x) ((3.141592653589793 * (f64)(x)) / 180.0)

typedef struct M514Mot_s {
    u16 idx;
    f32 shift;
    u32 attr;
} M514MOT;

typedef struct M514Jump_s {
    s32 time;
    f32 velX;
    f32 velY;
    f32 boostX;
    f32 boostYUp;
    f32 boostYDown;
    f32 limit;
} M514JUMP;

typedef struct M514Com_s {
    f32 waitBase;
    f32 waitRange;
    f32 holdBase;
    f32 holdRange;
} M514COM;

void fn_1_1EC4(OMOBJ *obj);
void fn_1_1ED4(OMOBJ *obj);
void fn_1_20E0(OMOBJ *obj);
void fn_1_2338(OMOBJ *obj);
void fn_1_284C(OMOBJ *obj);
void fn_1_2A88(OMOBJ *obj);
void fn_1_2D10(OMOBJ *obj, s32 side);
void fn_1_2E88(OMOBJ *obj);
void fn_1_2E8C(OMOBJ *obj);
void fn_1_37CC(OMOBJ *obj, s32 kind);
s32 fn_1_387C(OMOBJ *obj, HuVecF *target, s32 useX, f32 targetRot);
void fn_1_3CF8(OMOBJ *obj);
void fn_1_3D6C(OMOBJ *obj);
void fn_1_3EC0(OMOBJ *obj, u32 motIdx);
s32 fn_1_3FB8(OMOBJ *obj);

u32 lbl_1_data_B8[8] = {
    0x8C0000, 0x8C0001, 0x8C0002, 0x8C0006, 0x8C0031, 0x8C0003, 0x870021, 0x8C0004,
};

M514MOT lbl_1_data_D8[8] = {
    { 0, 0.2f, 0x40000001 },
    { 1, 0.2f, 0x40000001 },
    { 2, 0.2f, 0x40000001 },
    { 3, 0.2f, 0 },
    { 4, 0.2f, 0 },
    { 5, 0.1f, 0 },
    { 6, 0.2f, 0x40000001 },
    { 7, 0.2f, 0 },
};

HuVecF lbl_1_data_138[10] = {
    { -2100.0f, 0.0f, -500.0f },
    { -1900.0f, 0.0f, -500.0f },
    { -1700.0f, 0.0f, -500.0f },
    { -1500.0f, 0.0f, -500.0f },
    { -3200.0f, 0.0f, -500.0f },
    { -3000.0f, 0.0f, -500.0f },
    { -2800.0f, 0.0f, -500.0f },
    { -2600.0f, 0.0f, -500.0f },
    { -3100.0f, 0.0f, 0.0f },
    { -2570.0f, 0.0f, 0.0f },
};

f32 lbl_1_data_1B0[16] = {
    0.98f, 1.06f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
};

f32 lbl_1_data_1F0[4] = { 270.0f, 270.0f, 270.0f, 270.0f };

M514JUMP lbl_1_data_23C[3] = {
    { 3, 13.333334f, 16.666668f, 0.5833334f, 1.0f, 6.666667f, 1500.0f },
    { 4, 13.333334f, 16.666668f, 0.5833334f, 1.0f, 6.666667f, 3000.0f },
    { 6, 16.666668f, 16.666668f, 0.75000006f, 1.0f, 8.333334f, 5000.0f },
};

s32 lbl_1_data_290[3] = { 0x6DA, 0x6DB, 0x6DC };

M514COM lbl_1_data_2BC[4] = {
    { 6.0f, 1.2f, 9.66f, 0.9f },
    { 4.2f, 3.0f, 6.9f, 3.0f },
    { 3.6f, 3.6f, 6.3f, 3.0f },
    { 3.0f, 4.8f, 5.28f, 3.0f },
};

/* bss: MWCC lays it out in reverse declaration order */
static OMOBJ *lbl_1_bss_80;
static OMOBJ *lbl_1_bss_70[4];

void fn_1_1D9C(OMOBJMAN *objman)
{
    s32 i;

    lbl_1_bss_80 = omAddObjEx(objman, 0x28, 0, 0, 0, fn_1_1EC4);
    omMakeGroupEx(objman, 0, 4);
    for (i = 0; i < 4; i++) {
        OMOBJ *obj = omAddObjEx(objman, 0x1E, 1, 8, 0, fn_1_20E0);
        lbl_1_bss_70[i] = obj;
        obj->work[0] = i;
        obj->work[1] = GwPlayerConf[i].grp;
        obj->work[2] = i;
        fn_1_1838(i, obj->work[1], GwPlayerConf[i].charNo);
    }
}

void fn_1_1EA0(void)
{
    CharModelKill(CHARNO_NONE);
}

void fn_1_1EC4(OMOBJ *obj)
{
    obj->objFunc = fn_1_1ED4;
}

void fn_1_1ED4(OMOBJ *obj)
{
    s32 i;
    s32 j;

    for (i = 0; i < 3; i++) {
        M514PLAYER *a = lbl_1_bss_70[i]->data;
        if (a->active != 0) {
            s32 scoreA = fn_1_95BC(i);
            for (j = i + 1; j < 4; j++) {
                M514PLAYER *b = lbl_1_bss_70[j]->data;
                if (b->active != 0 && !(fabs(a->pos.z - b->pos.z) > 200.0) && !(fabs(a->pos.x - b->pos.x) > 160.0)) {
                    s32 scoreB = fn_1_95BC(j);
                    s32 aLoses = 0;
                    if (scoreA < scoreB) {
                        aLoses = 1;
                    }
                    if (scoreA == scoreB && i > j) {
                        aLoses = 1;
                    }
                    if (aLoses == 0) {
                        a->pos.x = a->pos.x + 5.0;
                        b->pos.x = b->pos.x - 5.0;
                    } else {
                        a->pos.x = a->pos.x - 5.0;
                        b->pos.x = b->pos.x + 5.0;
                    }
                }
            }
        }
    }
}

void fn_1_20E0(OMOBJ *obj)
{
    M514PLAYER *p;
    s16 charNo;
    HuVecF *start;
    s32 i;

    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M514PLAYER), 0x10000000);
    p = obj->data;
    memset(p, 0, sizeof(M514PLAYER));
    obj->stat |= 0x100;
    p->playerNo = obj->work[0];
    p->grp = obj->work[1];
    p->index = obj->work[2];
    charNo = GwPlayerConf[p->playerNo].charNo;
    p->charNo = charNo;
    p->comDif = GwPlayerConf[p->playerNo].dif & 3;
    p->isCom = GwPlayerConf[p->playerNo].type;
    p->unk28 = 0;
    obj->mdlId[0] = CharModelCreate(charNo, 2);
    Hu3DModelLayerSet(obj->mdlId[0], 6);
    Hu3DModelShadowSet(obj->mdlId[0]);
    CharEffectLayerSet(6);
    for (i = 0; i < 8; i++) {
        obj->mtnId[i] = CharMotionCreate(charNo, lbl_1_data_B8[i]);
    }
    CharMotionDataClose(charNo);
    p->motNo = -1;
    fn_1_3EC0(obj, 0);
    start = &lbl_1_data_138[p->index];
    p->pos.x = start->x;
    p->pos.y = start->y;
    p->pos.z = start->z;
    p->vel.z = 0.0f;
    p->vel.y = 0.0f;
    p->vel.x = 0.0f;
    p->rotY = lbl_1_data_1F0[p->index];
    p->effectId = -1;
    p->sfxId = -1;
    p->state = 0;
    p->runState = 0;
    obj->objFunc = fn_1_2338;
}

void fn_1_22C8(OMOBJ *obj)
{
    M514PLAYER *p = obj->data;
    f32 tx = (f32)(f64)p->pos.x;

    Center.x = Center.x + 0.1 * (tx - Center.x);
}

void fn_1_2338(OMOBJ *obj)
{
    M514PLAYER *p = obj->data;
    HuVecF target;

    p->padDown = 0;
    p->padBtn = 0;
    p->stickX = 0.0f;
    p->stickY = 0.0f;
    switch (p->state) {
        case 0:
            if (fn_1_387C(obj, &lbl_1_data_138[p->playerNo + 4], 1, 0.0f) == 0) {
                break;
            }
            p->state++;
            p->hold = 1;
            /* fallthrough */
        case 1:
            if (MgSeqModeGet() != 5) {
                break;
            } else {
                u32 turn = fn_1_1814();
                if (turn != p->playerNo) {
                    s32 idx = (p->playerNo - turn) - 1 + 4;
#ifdef TARGET_PC
                    if (idx < 0) {
                        idx = 0;
                    } else if (idx > 9) {
                        idx = 9;
                    }
#endif
                    fn_1_387C(obj, &lbl_1_data_138[idx], 1, 0.0f);
                    break;
                }
                p->state++;
                p->stateTimer = 0;
                fn_1_37CC(obj, 9);
            }
            /* fallthrough */
        case 2:
            Center.x = Center.x + 0.1 * (p->pos.x - Center.x);
            if (fn_1_387C(obj, &lbl_1_data_138[8], 1, 90.0f) == 0) {
                break;
            }
            p->stateTimer++;
            if ((f32)p->stateTimer < 30.0) {
                break;
            }
            p->state++;
            p->stateTimer = 0;
            p->hold = 0;
            p->active = 1;
            /* fallthrough */
        case 3:
            Center.x = Center.x + 0.1 * (p->pos.x - Center.x);
            if (p->finished != 0) {
                p->state++;
                p->stateTimer = 0;
            } else if (p->isCom != 0) {
                fn_1_3D6C(obj);
            } else {
                s16 pad = GwPlayerConf[p->playerNo].padNo;
                p->padBtn = HuPadBtn[pad];
                p->padDown = HuPadBtnDown[pad];
            }
            break;
        case 4:
            Center.x = Center.x + 0.1 * (p->pos.x - Center.x);
            if (fn_1_50E4() == 0) {
                target.x = p->pos.x;
                target.y = 0.0f;
                target.z = -220.0f;
                p->vel.y = 0.0f;
                p->pos.y = 0.0f;
                if (fn_1_387C(obj, &target, 0, 0.0f) != 0) {
                    p->state++;
                    p->stateTimer = 0;
                }
            }
            break;
        case 5:
            p->stateTimer++;
            if ((f32)p->stateTimer < 30.0) {
                break;
            }
            fn_1_1824(p->playerNo);
            p->state++;
            p->stateTimer = 0;
            p->hold = 1;
            fn_1_9334();
            break;
        case 6:
            p->stateTimer++;
            if ((f32)p->stateTimer < 18.0) {
                break;
            }
            p->state++;
            /* fallthrough */
        case 7:
            if (MgSeqModeGet() == 7) {
                p->state = 0;
                obj->objFunc = fn_1_284C;
            }
            break;
    }
    fn_1_2E8C(obj);
    fn_1_2E88(obj);
    PSVECAdd(&p->pos, &p->vel, &p->pos);
    fn_1_2A88(obj);
    fn_1_3CF8(obj);
}

void fn_1_284C(OMOBJ *obj)
{
    M514PLAYER *p = obj->data;
    HuVecF target;

    p->padDown = 0;
    p->padBtn = 0;
    p->stickX = 0.0f;
    p->stickY = 0.0f;
    p->vel.z = 0.0f;
    p->vel.y = 0.0f;
    p->vel.x = 0.0f;
    switch (p->state) {
        case 0:
            p->hold = 0;
            p->landWait = 0;
            p->state++;
            /* fallthrough */
        case 1:
            if (fn_1_16EC(p->playerNo) < 0
                || (target.x = p->pos.x, target.y = p->pos.y, target.z = -100.0f, fn_1_387C(obj, &target, 0, 0.0f) != 0)) {
                if (MgSeqModeGet() == 8) {
                    p->state++;
                    p->stateTimer = 0;
                }
            }
            break;
        case 2:
            p->rotY = fn_1_1BCC(p->rotY, 0.0f, 0.1f);
            p->stateTimer++;
            if (p->stateTimer >= 10) {
                p->state++;
                p->stateTimer = 0;
            }
            break;
        case 3:
            p->rotY = fn_1_1BCC(p->rotY, 0.0f, 0.1f);
            p->stateTimer++;
            if (p->stateTimer >= 20) {
                if (fn_1_16EC(p->playerNo) >= 0) {
                    fn_1_3EC0(obj, 3);
                } else {
                    fn_1_3EC0(obj, 4);
                    fn_1_1890(p->playerNo);
                }
                p->state++;
            }
            break;
        case 5:
            if (fn_1_3FB8(obj) != 0) {
                p->state++;
            }
            break;
    }
    PSVECAdd(&p->pos, &p->vel, &p->pos);
    fn_1_2A88(obj);
    fn_1_3CF8(obj);
}

void fn_1_2A88(OMOBJ *obj)
{
    M514PLAYER *p = obj->data;

    p->frameNo++;
    fn_1_7C10(p->effectId, &p->pos);
    CharModelVoicePanSet(p->charNo, fn_1_1960(&p->pos));
    CharModelVoiceVolSet(p->charNo, fn_1_19F4());
    if ((p->frameNo % 6) == 0 && p->pos.y == 0.0f && PSVECMag(&p->vel) > 5.000000260770321) {
        p->stepSide = p->stepSide + 1;
        p->stepSide = p->stepSide & 1;
        fn_1_2D10(obj, p->stepSide);
    }
    if (p->motNo == 5 && fn_1_3FB8(obj) != 0) {
        fn_1_3EC0(obj, 6);
    }
    if (p->landWait != 0 && fn_1_3FB8(obj) != 0) {
        p->landWait = 0;
    }
}

void fn_1_2BC8(OMOBJ *obj, HuVecF *out, s32 side)
{
    M514PLAYER *p = obj->data;
    f64 off;
    f32 ang;

    if (side == 0) {
        off = 90.0;
    } else {
        off = -90.0;
    }
    ang = p->rotY + off;
    out->x = p->pos.x + 100.0 * (0.15 * sin(M514_RAD(ang)));
    out->y = p->pos.y;
    out->z = p->pos.z + 100.0 * (0.15 * cos(M514_RAD(ang)));
}

void fn_1_2D10(OMOBJ *obj, s32 side)
{
    HuVecF pos;

    if (((M514PLAYER *)obj->data)->charNo != 8) {
        fn_1_2BC8(obj, &pos, side);
        pos.y = 0.0f;
        fn_1_80C0(&pos, 10.0f);
    }
}

void fn_1_2E88(OMOBJ *obj)
{
}

void fn_1_2E8C(OMOBJ *obj)
{
    M514PLAYER *p = obj->data;
    M514JUMP *jump;
    HuVecF pos;
    u32 mask;

    if (p->moving != 0) {
        return;
    }
    switch (p->runState) {
        case 0:
            if (p->hold != 0) {
                break;
            }
            p->runState++;
            p->runTimer = 0;
            p->jumpNo = 0;
            /* fallthrough */
        case 1:
            p->runTimer++;
            if ((f32)p->runTimer < 18.0) {
                break;
            }
            p->runState++;
            p->runTimer = 0;
            p->unk5C = 0.2f;
            /* fallthrough */
        case 2:
            p->vel.x = p->vel.x + 0.1 * (9.166667f - p->vel.x);
            if (p->pos.x > (f32)(lbl_1_data_138[9].x - 100.0)) {
                fn_1_8228(0);
                p->unk64 = 1;
            }
            if (p->pos.x < lbl_1_data_138[9].x) {
                fn_1_3EC0(obj, 2);
                p->unk5C = p->unk5C + 0.1 * (1.0 - p->unk5C);
#ifdef TARGET_PC
                Hu3DMotionSpeedSet(obj->mdlId[0], p->unk5C * lbl_1_data_1B0[p->charNo & 0xF]);
#else
                Hu3DMotionSpeedSet(obj->mdlId[0], p->unk5C * lbl_1_data_1B0[p->charNo]);
#endif
                break;
            }
            p->runState++;
            p->runTimer = 0;
            /* fallthrough */
        case 3:
#ifdef TARGET_PC
            if (p->jumpNo > 2) {
                break;
            }
#endif
            jump = &lbl_1_data_23C[p->jumpNo];
            p->vel.x = p->vel.x * 0.5;
            p->runTimer++;
            if (p->runTimer >= jump->time) {
                p->runState++;
                p->jumping = 1;
                p->vel.x = jump->velX;
                p->vel.y = jump->velY;
                p->unk60 = -0.6666667f;
                fn_1_3EC0(obj, 5);
                p->effectId = fn_1_79F8(&p->pos, 20.0f);
                p->sfxId = HuAudFXPlay(lbl_1_data_290[p->jumpNo]);
                fn_1_37CC(obj, 0);
            }
            break;
        case 4:
#ifdef TARGET_PC
            if (p->jumpNo > 2) {
                break;
            }
#endif
            jump = &lbl_1_data_23C[p->jumpNo];
            if (p->vel.y >= 0.0f) {
                p->unk60 = p->unk60 - 0.00500000026077032;
            } else {
                p->unk60 = p->unk60 - 0.01666666753590107;
            }
            p->vel.x = p->vel.x * 0.98;
            p->vel.y = p->vel.y + p->unk60;
            if ((p->jumpNo & 1) == 0) {
                mask = 0x100;
            } else {
                mask = 0x200;
            }
            if ((p->padDown & mask) != 0) {
                p->vel.x = p->vel.x + jump->boostX;
                if (p->vel.y > 0.0f) {
                    p->vel.y = p->vel.y + jump->boostYUp;
                } else {
                    p->vel.y = p->vel.y + jump->boostYDown;
                }
                fn_1_1870(p->playerNo);
            }
            if (!(p->vel.y > 0.0f) && !(p->pos.y > 0.0f)) {
                p->pos.y = 0.0f;
                p->vel.y = 0.0f;
                p->vel.x = 0.0f;
                p->jumping = 0;
                CharModelLandDustCreate(p->charNo, &p->pos);
                fn_1_3EC0(obj, 7);
                if (p->charNo != 8) {
                    fn_1_2BC8(obj, &pos, p->jumpNo & 1);
                    pos.y = 0.0f;
                    fn_1_80C0(&pos, 10.0f);
                }
                fn_1_7B8C(p->effectId);
                p->effectId = -1;
                HuAudFXFadeOut(p->sfxId, 0x7D);
                p->sfxId = -1;
                HuAudFXPlay(0x21);
                p->jumpNo++;
                if (p->jumpNo < 3) {
                    fn_1_8228(p->jumpNo & 1);
                    p->runState--;
                    p->runTimer = 0;
                    break;
                }
                fn_1_37CC(obj, p->jumpNo);
                if (p->charNo != 8) {
                    fn_1_2BC8(obj, &pos, p->jumpNo & 1);
                    pos.y = 0.0f;
                    fn_1_80C0(&pos, 10.0f);
                }
                fn_1_8228(-1);
                p->runState++;
                p->runTimer = 0;
            } else {
                break;
            }
            /* fallthrough */
        case 5:
            p->vel.x = p->vel.x * 0.5;
            p->runTimer++;
            if (p->runTimer >= 10) {
                p->runState++;
                p->runTimer = 0;
                p->unk64 = 0;
                p->vel.x = 0.0f;
                fn_1_3EC0(obj, 0);
            }
            break;
        case 6:
            p->runTimer++;
            if ((f32)p->runTimer < 48.0) {
                break;
            }
            p->runState++;
            p->finished = 1;
            break;
    }
    if (p->pos.y > 400.0) {
        p->unk60 = p->unk60 - 0.03333333507180214;
    }
    if (p->pos.y > 500.0) {
        p->unk60 = p->unk60 - 0.03333333507180214;
    }
    if (p->unk64 != 0) {
        f32 dist = p->pos.x - -2500.0;
        if (dist < 0.0f) {
            dist = 0.0f;
        }
        fn_1_9570(p->grp, dist);
        if (p->jumpNo < 3 && dist > lbl_1_data_23C[p->jumpNo].limit) {
            p->unk60 = p->unk60 - 0.3333333507180214;
        }
    }
}

void fn_1_37CC(OMOBJ *obj, s32 kind)
{
    M514PLAYER *p = obj->data;
    s32 playerNo = p->playerNo;

    switch (kind) {
        default:
            omVibrate(playerNo, 10, 6, 6);
            return;
        case 3:
            omVibrate(playerNo, 12, 12, 0);
            return;
        case 9:
            omVibrate(playerNo, 20, 12, 0);
            return;
    }
}

s32 fn_1_387C(OMOBJ *obj, HuVecF *target, s32 useX, f32 targetRot)
{
    M514PLAYER *p = obj->data;
    f32 dx;
    f32 dz;
    f32 dist;
    f32 speed;

    p->moving = 1;
    if (useX != 0) {
        dx = target->x - p->pos.x;
    } else {
        dx = 0.0f;
    }
    dz = target->z - p->pos.z;
    dist = sqrtf(dx * dx + dz * dz);
    if (dist < 10.0) {
        fn_1_3EC0(obj, 0);
        if (useX != 0) {
            p->pos.x = p->pos.x + 0.1 * dx;
        }
        p->pos.z = p->pos.z + 0.1 * dz;
        p->vel.z = 0.0f;
        p->vel.y = 0.0f;
        p->vel.x = 0.0f;
        p->rotY = fn_1_1BCC(p->rotY, targetRot, 0.1f);
        if (dist > 1.0) {
            return 0;
        }
        if (fn_1_1CE8(p->rotY, targetRot) > 2.0) {
            return 0;
        }
        if (useX != 0) {
            p->pos.x = target->x;
        }
        p->pos.y = target->y;
        p->pos.z = target->z;
        p->rotY = targetRot;
        p->moving = 0;
        return 1;
    }
    if (dist < 50.0) {
        fn_1_3EC0(obj, 1);
        speed = 0.5f;
    } else {
        fn_1_3EC0(obj, 2);
        speed = 1.0f;
    }
    p->rotY = fn_1_1BCC(p->rotY, (f32)(180.0 * (atan2(dx, dz) / 3.141592653589793)), 0.2f);
    p->vel.x = 0.01666666753590107 * (550.0 * (speed * sin(M514_RAD(p->rotY))));
    p->vel.z = 0.01666666753590107 * (550.0 * (speed * cos(M514_RAD(p->rotY))));
    if (useX == 0) {
        p->vel.x = 0.0f;
    }
    return 0;
}

void fn_1_3CF8(OMOBJ *obj)
{
    M514PLAYER *p = obj->data;
    HU3DMODELID mdl = obj->mdlId[0];

    Hu3DModelPosSetV(mdl, &p->pos);
    Hu3DModelRotSet(mdl, 0.0f, p->rotY, 0.0f);
}

void fn_1_3D6C(OMOBJ *obj)
{
    M514PLAYER *p = obj->data;
    M514COM *com = &lbl_1_data_2BC[p->comDif];

    switch (p->unk28) {
        case 0:
            p->unk28++;
            p->comTimer = (s32)((f64)com->waitBase + (f64)com->waitRange * (1.52587890625e-05 * (f64)(f32)FRAND16()));
            /* fallthrough */
        case 1:
            p->comTimer--;
            if (p->comTimer <= 0) {
                p->padDown = 0x300;
                p->padBtn = 0x300;
                p->comTimer = (s32)((f64)com->holdBase + (f64)com->holdRange * (1.52587890625e-05 * (f64)(f32)FRAND16()));
            }
            return;
    }
}

void fn_1_3EC0(OMOBJ *obj, u32 motIdx)
{
    M514PLAYER *p = obj->data;
    f32 shift;

    if ((u32)p->motNo != motIdx && motIdx < 8) {
        if (p->motNo < 0) {
            shift = 0.0f;
        } else {
            shift = 60.0f * lbl_1_data_D8[motIdx].shift;
        }
        p->motNo = motIdx;
        CharMotionShiftSet(p->charNo, obj->mtnId[lbl_1_data_D8[motIdx].idx], 0.0f, shift, lbl_1_data_D8[motIdx].attr);
    }
}

s32 fn_1_3FB8(OMOBJ *obj)
{
    M514PLAYER *p = obj->data;

    if (CharMotionEndCheck(p->charNo) != 0 && CharMotionShiftIDGet(p->charNo) < 0) {
        return 1;
    }
    return 0;
}

s32 fn_1_4024(OMOBJ *obj)
{
    s32 result = 0;

    if (CharMotionShiftIDGet(((M514PLAYER *)obj->data)->charNo) < 0) {
        result = 1;
    }
    return result;
}

s32 fn_1_4084(OMOBJ *obj, u32 motIdx)
{
    s32 result = 0;
    M514PLAYER *q = obj->data;
    s32 ready;

    if (CharMotionEndCheck(q->charNo) != 0 && CharMotionShiftIDGet(q->charNo) < 0) {
        ready = 1;
    } else {
        ready = 0;
    }
    if (ready != 0) {
        fn_1_3EC0(obj, motIdx);
        result = 1;
    }
    return result;
}

s32 fn_1_41C4(s32 grp)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        M514PLAYER *p = lbl_1_bss_70[i]->data;
        if (p->grp == grp && p->isCom == 0) {
            return 1;
        }
    }
    return 0;
}

f32 fn_1_4238(s32 idx)
{
    M514PLAYER *p;

#ifdef TARGET_PC
    idx &= 3;
#endif
    p = lbl_1_bss_70[idx]->data;
    return p->pos.x;
}
