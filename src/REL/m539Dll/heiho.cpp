#include "REL/m539Dll.h"

/*
 * m539's "heiho": a model that walks a list of points. It keeps its own
 * heading, turns at most five degrees per step towards the direction of the
 * next point, and steps over to the following point when it gets close
 * enough.
 *
 * A heading is kept in turns: 182.04444444444445 is 0x10000 / 360, so
 * multiplying by it and truncating gives the heading in 1/65536 of a turn.
 * The difference of two headings is taken with the usual 0x8000 bias and
 * scaled back to degrees by 0.0054931640625.
 */

extern "C" {

// NONMATCHING: register allocation only - the target keeps the 16-bit
// difference in r26/r29 without the extsh pair this build emits, and holds
// more temporaries in callee-saved registers.
f32 fn_1_9BDC(f32 a, f32 b)
{
    s32 ia = (s32)(a * 182.04444444444445);
    s32 ib = (s32)(b * 182.04444444444445);
    s16 diff = (u16)(ia - ib) - 0x8000;

    return 0.0054931640625 * diff;
}

// NONMATCHING: same extra extsh and a different split of the temporaries
// across f26..f31 than the target.
f32 fn_1_9C94(f32 a, f32 b, f32 limit)
{
    s32 ia = (s32)(a * 182.04444444444445);
    s32 ib = (s32)(b * 182.04444444444445);
    s16 diff = (u16)(ia - ib) - 0x8000;
    f32 angle = 0.0054931640625 * diff;
    f32 out;

    if (angle < -limit) {
        out = -limit;
    } else if (angle > limit) {
        out = limit;
    } else {
        out = angle;
    }
    return out;
}

// NONMATCHING: register allocation only.
void *fn_1_9DF8(M539Heiho *self, Point3d *start, s32 count)
{
    self->actor = fn_1_72E8(0x61000A);
    self->path = start;
    self->pathCount = count;
    fn_1_2E08(self->actor, 0x40000041);
    self->angle = 0.0f;
    self->heading = 0.0f;
    self->pos.x = start->x;
    self->pos.y = start->y;
    self->pos.z = start->z;
    self->curIdx = 0;
    fn_1_9EC0(self);
    return self;
}

// NONMATCHING: register allocation only.
void fn_1_9EC0(M539Heiho *self)
{
    s32 idx = (self->curIdx + 1) % self->pathCount;
    Point3d *next;

    self->curIdx = idx;
    next = &self->path[idx];
    self->target.x = next->x;
    self->target.y = next->y;
    self->target.z = next->z;
    PSVECSubtract(&self->target, &self->pos, &self->dir);
    PSVECNormalize(&self->dir, &self->dir);
    PSVECScale(&self->dir, &self->dir, 3.0f);
}

// NONMATCHING: register allocation only - the inlined waypoint update and
// the heading maths match, but the temporaries land in other registers.
void fn_1_9F6C(M539Heiho *self)
{
    Point3d diff;
    Vec rot;
    s32 ia, ib;
    s16 turn16;
    f32 turn;

    PSVECAdd(&self->dir, &self->pos, &self->pos);
    PSVECSubtract(&self->target, &self->pos, &diff);
    if (PSVECMag(&diff) < 30.000002f) {
        // fn_1_9EC0, inlined
        s32 idx = (self->curIdx + 1) % self->pathCount;
        Point3d *next;

        self->curIdx = idx;
        next = &self->path[idx];
        self->target.x = next->x;
        self->target.y = next->y;
        self->target.z = next->z;
        PSVECSubtract(&self->target, &self->pos, &self->dir);
        PSVECNormalize(&self->dir, &self->dir);
        PSVECScale(&self->dir, &self->dir, 3.0f);
    }
    self->heading = (f32)(180.0 * (atan2(self->dir.x, self->dir.z) / 3.141592653589793));
    ia = (s32)(self->heading * 182.04444444444445);
    ib = (s32)(self->angle * 182.04444444444445);
    turn16 = (u16)(ia - ib) - 0x8000;
    turn = 0.0054931640625 * turn16;
    if (turn < -5.0f) {
        turn = -5.0f;
    } else if (turn > 5.0f) {
        turn = 5.0f;
    }
    self->angle += turn;
    fn_1_2EF4(self->actor, &self->pos);
    rot.x = 0.0f;
    rot.y = self->angle;
    rot.z = 0.0f;
    fn_1_2F3C(self->actor, &rot);
}

}
