#include "REL/m574Dll.h"
#include "game/audio.h"
#include "game/data.h"

#ifdef __MWERKS__
#define M574_RODATA __declspec(section ".rodata")
#else
#define M574_RODATA
#endif

#define M574_BALL_MAX 80

typedef struct M574Ball_s {
    /* 0x00 */ s32 state;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 flag;
    /* 0x10 */ s32 area;
    /* 0x14 */ s16 modelId;
    /* 0x18 */ HuVecF pos;
    /* 0x24 */ HuVecF vel;
    /* 0x30 */ u8 pad30[0x0C];
    /* 0x3C */ M574_COLLIDER collider;
} M574_BALL;

void fn_1_1F9C(s32 grp);
void fn_1_77D4(s32 grp, s32 num);

static void fn_1_66F4(OMOBJ *obj);
static void fn_1_6730(OMOBJ *obj);
static void fn_1_6758(OMOBJ *obj);
static void fn_1_686C(OMOBJ *obj);
static s32 fn_1_6B38(M574_BALL *ball);
static void fn_1_6BD4(M574_BALL *ball);

static OMOBJ *ballObj;

void fn_1_6650(OMOBJMAN *om)
{
    OMOBJ *obj;
    void *data;

    obj = omAddObjEx(om, 0x1E, 0x50, 0, -1, fn_1_66F4);
    ballObj = obj;
    obj->stat |= OM_STAT_MODELPAUSE;
    data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M574_BALL) * M574_BALL_MAX, HU_MEMNUM_OVL);
    obj->data = data;
    memset(data, 0, sizeof(M574_BALL) * M574_BALL_MAX);
    obj->work[0] = 0;
}

void fn_1_66F0(void)
{
}

static void fn_1_66F4(OMOBJ *obj)
{
    fn_1_6758(obj);
    obj->objFunc = fn_1_6730;
}

static void fn_1_6730(OMOBJ *obj)
{
    fn_1_686C(obj);
}

static void fn_1_6758(OMOBJ *obj)
{
    M574_BALL *ball;
    M574_COLLIDER *col;
    s16 model;
    s32 i;
    s16 baseModel;
    M574_BALL *balls;

    balls = obj->data;
    for (i = 0; i < M574_BALL_MAX; i++) {
        ball = &balls[i];
        if (i == 0) {
            baseModel = Hu3DModelCreateData(0x810007);
            model = baseModel;
        }
        else {
            model = Hu3DModelLink(baseModel);
        }
        ball->modelId = model;
        Hu3DModelLayerSet(model, 4);
        Hu3DModelAttrSet(model, 1);
        col = &ball->collider;
        col->flag = 0;
        col->shape = 0;
        col->id = i;
        col->kind = 0;
        col->user = ball;
        col->hook = (void (*)(void *))fn_1_6BD4;
        col->pos = &ball->pos;
        col->vel = &ball->vel;
        col->prevPos = ball->pos;
        col->radius = 40.0f;
        fn_1_8AD0(col);
    }
}

static void fn_1_686C(OMOBJ *obj)
{
    M574_BALL *balls;
    M574_BALL *ball;
    s32 area;
    s16 model;
    s32 i;

    balls = obj->data;
    for (i = 0; i < M574_BALL_MAX; i++) {
        ball = &balls[i];
        model = ball->modelId;
        switch (ball->state) {
            case 0:
                break;
            case 1:
                ball->state++;
                ball->unk08 = 0;
                Hu3DModelAttrReset(model, 1);
                HuAudFXPlay(0x915);
                // fallthrough
            case 2:
                ball->vel.z -= 20.0;
                if (ball->pos.z + ball->vel.z <= 0.0f) {
                    ball->state++;
                    ball->pos.z = 0.0f;
                    ball->vel.z = 0.0f;
                    ball->collider.flag |= 1;
                }
                // fallthrough
            case 3:
                if ((ball->collider.flag & 2) == 0) {
                    if (ball->flag != 0) {
                        if (ball->pos.y < 1550.0) {
                            ball->flag = 0;
                        }
                    }
                    else if (ball->pos.y > 1600.0) {
                        ball->flag = 1;
                        fn_1_77D4(ball->pos.x < 0.0f ? 0 : 1, 0xC);
                    }
                    ball->vel.x *= 0.98;
                    ball->vel.y -= 1.666666753590107;
                    if (ball->vel.y < 0.0f && ball->pos.y < -200.0) {
                        fn_1_1F9C(ball->pos.x < 0.0f ? 0 : 1);
                        ball->state = 0;
                        ball->collider.flag = 0;
                        Hu3DModelAttrSet(model, 1);
                        if (ball->vel.x < 0.0f) {
                            HuAudFXPlay(0x92D);
                        }
                        else {
                            HuAudFXPlay(0x92E);
                        }
                    }
                }
                break;
        }
        if (ball->state != 0) {
            PSVECAdd(&ball->pos, &ball->vel, &ball->pos);
            area = fn_1_6B38(ball);
            if (ball->area != area) {
                ball->area = area;
                HuAudFXPlay(0x91D);
            }
        }
    }
}

static s32 fn_1_6B38(M574_BALL *ball)
{
    HuVecF *pos = &ball->pos;

    if (pos->x < -170.0) {
        return 0;
    }
    if (pos->x > 170.0) {
        return 0;
    }
    if (pos->y < 598.0) {
        return 0;
    }
    if (pos->y > 817.0) {
        return 0;
    }
    return 1;
}

static void fn_1_6BD4(M574_BALL *ball)
{
    s16 model = ball->modelId;

    Hu3DModelPosSetV(model, &ball->pos);
    ball->collider.prevPos = ball->pos;
}

void fn_1_6C2C(HuVecF *pos, HuVecF *vel)
{
    M574_BALL *balls = ballObj->data;
    M574_BALL *ball;
    s32 i;

    for (i = 0; i < M574_BALL_MAX; i++) {
        ball = &balls[i];
        if (ball->state == 0) {
            ball->state++;
            ball->pos = *pos;
            ball->vel = *vel;
            return;
        }
    }
}

s32 fn_1_6CC8(s32 grp)
{
    M574_BALL *balls = ballObj->data;
    M574_BALL *ball;
    s32 cnt = 0;
    s32 i;

    for (i = 0; i < M574_BALL_MAX; i++) {
        ball = &balls[i];
        if (ball->state != 0) {
            if (grp == 0) {
                if (ball->pos.x < 0.0f) {
                    cnt++;
                }
            }
            else if (ball->pos.x >= 0.0f) {
                cnt++;
            }
        }
    }
    return cnt;
}

s32 fn_1_6D7C(s32 grp, const HuVecF *pos, f32 ofs, f32 half, f32 maxDist)
{
    M574_BALL *balls;
    M574_BALL *ball;
    f32 best;
    f32 dx;
    f32 dy;
    f32 ang;
    f32 tmp;
    s32 found;
    s32 i;

    balls = ballObj->data;
    best = 100000.0f;
    found = -1;
    for (i = 0; i < M574_BALL_MAX; i++) {
        ball = &balls[i];
        if (ball->state == 0) {
            continue;
        }
        if (grp == 0) {
            if (ball->pos.x >= 0.0f) {
                continue;
            }
        }
        else if (ball->pos.x < 0.0f) {
            continue;
        }
        dx = ball->pos.x - pos->x;
        dy = ball->pos.y - pos->y;
        if (half < 180.0) {
            ang = HuAtan(dx, dy);
            tmp = fmod(ang - ofs, 360.0);
            ang = tmp;
            if (ang < 0.0f) {
                ang += 360.0f;
            }
            if (ang > half && ang < 360.0f - half) {
                continue;
            }
        }
        ang = sqrtf(dx * dx + dy * dy);
        if (!(ang > maxDist) && ang < best) {
            best = ang;
            found = i;
        }
    }
    return found;
}

void *fn_1_70B0(s32 idx, HuVecF *pos, HuVecF *vel)
{
    M574_BALL *balls = ballObj->data;
    M574_BALL *ball = &balls[idx];

    if (pos != NULL) {
        *pos = ball->pos;
    }
    if (vel != NULL) {
        *vel = ball->vel;
    }
    return &ball->collider;
}
