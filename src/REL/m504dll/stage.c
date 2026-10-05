#define _MATH_H

#include "REL/m504dll.h"
#include "string.h"

typedef struct StageOwner_s {
    u8 pad0[0x1210];
    s32 unk1210;
} STAGEOWNER;

typedef struct StageWork_s {
    STAGEOWNER *owner;
    u8 pad4[0x46C - 4];
    s32 unk46C;
} STAGEWORK;

typedef struct Stage_s {
    STAGEWORK *work;
    s32 unk4;
    u8 pad8[0x22 - 8];
    s16 mtnId;
    s16 mtnIdB;
    s16 mtnIdC;
    s16 mtnIdD;
    u16 subState;
    s16 timer;
    u8 pad2E[0x72 - 0x2E];
    s16 mtnId2;
    s16 unk74;
    s16 shapeMotId;
    u16 state;
    u8 pad7A[0xB8 - 0x7A];
    s32 unkB8;
    u8 padBC[0xCC - 0xBC];
    s32 unkCC;
    u8 padD0[0x46C - 0xD0];
    s32 unk46C;
    u8 pad470[0x474 - 0x470];
} STAGE;

typedef struct StageRange_s {
    s32 unk0;
    f32 f4;
} STAGERANGE;

extern s32 fn_1_5EC(s32 code);
extern void fn_1_255C(void *a, s32 b, void *c);
extern void fn_1_3B9C(void *a);
extern void fn_1_3D9C(void *a);
extern void fn_1_51B4(void *a, s32 b);
extern void fn_1_53AC(void *a);
extern void fn_1_5934(void *a, s32 b, void *c);
extern s32 fn_1_9680(void *a, void *b);
extern void fn_1_9C10(void *a);
extern s32 fn_1_9CAC(void *a);
extern void fn_1_5E1C(void *a);
extern void fn_1_5EC8(void *a);
extern void fn_1_C4F4(void *a, s32 b, void *c);
extern void fn_1_C71C(void *a);
extern void fn_1_C74C(void *a);
void fn_1_4FEC(STAGE *self);
void fn_1_5168(STAGE *self);
void fn_1_572C(STAGE *self);

void fn_1_23DC(STAGE *self, s32 type, s32 arg)
{
    s32 flag;

    memset(self, 0, 0x474);
    self->work = (STAGEWORK *)arg;
    self->unk4 = type;

    if (type & 1) {
        flag = 0;
    } else {
        flag = 1;
    }
    self->unk46C = flag;

    fn_1_C4F4((u8 *)self + 0x3BC, type, self);
    fn_1_255C((u8 *)self + 0x15C, type, self);
    fn_1_5934((u8 *)self + 0x8, type, self);
    self->unkCC = 0;
    fn_1_9680((u8 *)self + 0xDC, self);
    self->unkCC = 0;
}

void fn_1_24A0(STAGE *self)
{
    fn_1_9C10((u8 *)self + 0xDC);
    fn_1_5E1C((u8 *)self + 0x8);
    fn_1_3B9C((u8 *)self + 0x15C);
    fn_1_C71C((u8 *)self + 0x3BC);
    memset(self, 0, 0x474);
}

s32 fn_1_24F8(STAGE *self)
{
    s32 ret;

    ret = 0;
    fn_1_5EC8((u8 *)self + 0x8);

    if (fn_1_9CAC((u8 *)self + 0xDC) != 0) {
        ret = 1;
    }
    fn_1_3D9C((u8 *)self + 0x15C);
    fn_1_C74C((u8 *)self + 0x3BC);
    return ret;
}

s32 fn_1_3FCC(STAGERANGE *self)
{
    f32 lim;

    lim = 96.0f;

    /* NONMATCHING: 3 rows - MWCC branches over the first return block with bge
     * instead of branching forward to it with blt.
     */
    if (self->f4 < -lim) {
        return 0;
    }

    if (self->f4 >= lim + 576.0f) {
        return 0;
    }
    return 1;
}

void fn_1_4FA4(STAGE *self)
{
    fn_1_4FEC(self);
    fn_1_5168(self);
    fn_1_53AC(self);
    fn_1_572C(self);
}

void fn_1_4FEC(STAGE *self)
{
    STAGEWORK *work;
    STAGEOWNER *owner;

    work = self->work;
    owner = work->owner;

    /* NONMATCHING: 6 rows, all in the dispatch - for the {3,4} subtree MWCC
     * emits beq+bge where CodeWarrior emits a single bge. Every case body is
     * byte-identical, including both 0x40000001 attr masks.
     */
    switch (self->subState) {
    case 0:
        break;

    case 1:
        Hu3DMotionShiftSet(self->mtnId, self->mtnIdD, 0.0f, 4.0f, 0);
        self->subState++;

        if (owner->unk1210 != 0) {
            if (work->unk46C != 0) {
                fn_1_5EC(0x653);
            } else {
                fn_1_5EC(0x654);
            }
        }
        break;

    case 2:
        if (Hu3DMotionShiftIDGet(self->mtnId) >= 0) {
            break;
        }

        Hu3DMotionShiftSet(self->mtnId, self->mtnIdC, 0.0f, 30.0f,
            0x40000000 | HU3D_ATTR_DISPOFF);
        self->timer = 360;
        self->subState++;
        break;

    case 3:
        if (self->timer-- > 0) {
            break;
        }

        Hu3DMotionShiftSet(self->mtnId, self->mtnIdB, 0.0f, 10.0f,
            0x40000000 | HU3D_ATTR_DISPOFF);
        self->timer = 0;
        self->subState = 0;
        break;

    case 4:
        break;
    }
}

void fn_1_5168(STAGE *self)
{
    fn_1_51B4(self, 0);
    fn_1_51B4(self, 1);
    fn_1_51B4(self, 2);
}

void fn_1_572C(STAGE *self)
{
    f32 len;
    f32 maxTime;
    STAGEWORK *work;
    STAGEOWNER *owner;

    work = self->work;
    owner = work->owner;

    /* NONMATCHING: 8 rows, all in the switch dispatch tail - MWCC emits one
     * extra beq/bge pair in the >=3 subtree. Every instruction from the case-3
     * body down is byte-identical, including the 0x40000001 attr mask.
     */
    switch (self->state) {
    case 0:
        break;

    case 3:
        Hu3DMotionShapeSet(self->mtnId2, self->shapeMotId);
        Hu3DMotionShapeSpeedSet(self->mtnId2, 1.0f);
        Hu3DModelAttrReset(self->mtnId2, 0x40000000 | HU3D_ATTR_DISPOFF);
        self->unkB8 = 0;

        if (owner->unk1210 != 0) {
            if (work->unk46C != 0) {
                fn_1_5EC(0x655);
            } else {
                fn_1_5EC(0x656);
            }
        }
        self->state++;
        break;

    case 2:
        len = *(f32 *)((u8 *)&Hu3DData[self->mtnId2] + 0x94);
        maxTime = Hu3DMotionShapeMaxTimeGet(self->mtnId2);

        if (len >= maxTime) {
            Hu3DModelAttrSet(self->mtnId2, 1);
            self->state++;
        }
        break;

    case 4:
        break;
    }
}