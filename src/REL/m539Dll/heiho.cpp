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

// Angle helpers: degrees <-> 16 bit binary angles, inlined by the original.
inline s32 M539DegToAng(f32 deg)
{
    return (s32)(deg * 182.04444444444445);
}

inline s16 M539AngDiff(s32 a, s32 b)
{
    return (u16)(a - b) - 0x8000;
}

inline f32 M539AngToDeg(s16 ang)
{
    return 0.0054931640625 * ang;
}

inline f32 M539Clamp(f32 v, f32 limit)
{
    f32 out;

    if (v < -limit) {
        out = -limit;
    } else if (v > limit) {
        out = limit;
    } else {
        out = v;
    }
    return out;
}

// Signed difference of two headings, clamped to +-limit degrees.
f32 fn_1_9C94(f32 a, f32 b, f32 limit)
{
    s32 ia = M539DegToAng(a);
    s32 ia2 = ia;
    s32 ib = M539DegToAng(b);
    s32 ib2 = ib;
    s16 diff = M539AngDiff(ia2, ib2);
    s16 diff2 = diff;
    f32 angle = M539AngToDeg(diff2);
    f32 angle2 = angle;
    f32 angle3 = angle2;

    return M539Clamp(angle3, limit);
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

// Step along the path: advance, retarget when close, turn toward the heading.
void fn_1_9F6C(M539Heiho *self)
{
    Point3d diff;
    Vec rot;
    f32 turn;

    PSVECAdd(&self->dir, &self->pos, &self->pos);
    PSVECSubtract(&self->target, &self->pos, &diff);
    if (PSVECMag(&diff) < 30.000002f) {
        fn_1_9EC0(self);
    }
    self->heading = (f32)(180.0 * (atan2(self->dir.x, self->dir.z) / 3.141592653589793));
    turn = fn_1_9C94(self->heading, self->angle, 5.0f);
    self->angle += turn;
    fn_1_2EF4(self->actor, &self->pos);
    rot.x = 0.0f;
    rot.y = self->angle;
    rot.z = 0.0f;
    fn_1_2F3C(self->actor, &rot);
}

}
