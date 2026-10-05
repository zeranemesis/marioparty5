#include "REL/m510dll.h"
#include "string.h"

// One ball, 0x250 bytes
typedef struct M510Ball_s {
    u8 pad00[0x0C];
    s32 state;       //0x0C
    u8 pad10[0x5A - 0x10];
    s16 modelId0;    //0x5A
    s16 modelId1;    //0x5C
    u8 pad5E[0x250 - 0x5E];
} M510_BALL;

// Ball work, 0x6FC bytes (M510_PLAYER_WORK::ballPathWork)
typedef struct M510BallWork_s {
    M510_PLAYER_WORK *owner;  //0x00
    M510_BALL balls[3];       //0x04
    s16 liveCount;            //0x6F4
    s16 modelId0;             //0x6F6
    s16 modelId1;             //0x6F8
    s16 motionId;             //0x6FA
} M510_BALL_WORK;

// REL/m510dll.h types every one of these entry points as `void *`. Casting the
// argument at each use keeps the pointer in the incoming register; aliasing it
// with a local makes MWCC emit a stw/lwz pair for a stack home it does not use.
#define WORK(p) ((M510_BALL_WORK *)(p))
#define BALL(p) ((M510_BALL *)(p))

// fn_1_509C alone is clearer with a named work pointer: the local form puts the
// work in r31 and the model id in r30 exactly as the original does.
#define M510_DATA_BALL_SHADOW 0x44003
#define M510_DATA_BALL_ITEM   0x44004
#define M510_DATA_BALL_MOTION 0x44018

// NONMATCHING: mwcceppc splits 0x44003 into lis 0x4 / addi 0x4003 where the
// original splits it lis 0x44 / addi 0x3, and the named work pointer costs an
// stw/lwz stack-home pair that pushes obj's home from 0x8 to 0xc.
void fn_1_509C(void *data, void *obj)
{
    // REL/m510dll.h declares fn_1_53E0 with one argument but the original passes
    // three; redeclared in block scope rather than editing the header.
    void fn_1_53E0(void *ball, s16 index, void *work);
    M510_BALL_WORK *work;
    HU3DMODELID modelId;
    s32 i;

    work = data;
    memset(work, 0, sizeof(M510_BALL_WORK));
    work->owner = obj;

    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(M510_DATA_BALL_SHADOW, 0x1000, HEAP_MODEL));
    work->modelId0 = modelId;
    Hu3DModelAttrSet(modelId, HU3D_ATTR_DISPOFF);

    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(M510_DATA_BALL_ITEM, 0x1000, HEAP_MODEL));
    work->modelId1 = modelId;
    Hu3DModelAttrSet(modelId, HU3D_ATTR_DISPOFF);

    work->motionId = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(M510_DATA_BALL_MOTION, 0x1000, HEAP_MODEL));
    Hu3DMotionSet(modelId, work->motionId);
    Hu3DModelAttrReset(modelId, 0x40000001);

    for (i = 0; i < 3; i++) {
        fn_1_53E0(&work->balls[i], i, work);
    }
}

// NONMATCHING: work ends in r30 and the loop counter in r31; the original has
// them the other way round (mwcceppc allocates locals before parameters).
void fn_1_51B4(void *data)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        fn_1_557C(&WORK(data)->balls[i]);
    }

    Hu3DMotionKill(WORK(data)->motionId);
    Hu3DModelKill(WORK(data)->modelId1);
    Hu3DModelKill(WORK(data)->modelId0);
    memset(data, 0, sizeof(M510_BALL_WORK));
}

// NONMATCHING: the work pointer and the ball pointer come out swapped (r29 and
// r30); the original keeps work in r30 and the ball pointer in r29.
void fn_1_5230(void *data)
{
    s32 i;
    M510_BALL *ball;

    WORK(data)->liveCount = 0;

    for (i = 0; i < 3; i++) {
        ball = &WORK(data)->balls[i];
        fn_1_55C4(ball);

        if (ball->state != 0) {
            WORK(data)->liveCount += 1;
        }
    }
}

void fn_1_557C(void *data)
{
    Hu3DModelKill(BALL(data)->modelId1);
    Hu3DModelKill(BALL(data)->modelId0);
    memset(data, 0, sizeof(M510_BALL));
}