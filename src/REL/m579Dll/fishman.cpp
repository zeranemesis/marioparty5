#include "dolphin.h"
#include <math.h>
#include "REL/m579Dll/m579types.h"

/* Rope simulation of the fish actor (M579Fish). */
extern "C" {
void fn_1_7B4C(M579Fish *f);
s32 fn_1_7CD8(M579Fish *f, Vec *a);
void fn_1_805C(M579Fish *f, s32 idx, Vec *a, f32 t);
void fn_1_8180(M579Fish *f);
void fn_1_826C(M579Fish *f);
void fn_1_8358(M579Fish *f);
void fn_1_84EC(M579Fish *f, Vec *a, u8 flag);
}

void fn_1_7B4C(M579Fish *f)
{
    s32 i;

    f->ropeVel.x = 0.0f;
    f->ropeVel.y = 0.0f;
    f->ropeVel.z = 0.0f;
    f->ropePos.x = 0.0f;
    f->ropePos.y = -f->ropeLen;
    f->ropePos.z = 0.0f;
    for (i = 0; i < 30; i++) {
        f->ropeFlag[i] = 0;
        f->ropeFrc[i].x = 0.0f;
        f->ropeFrc[i].y = 0.0f;
        f->ropeFrc[i].z = 0.0f;
        f->ropePt[i].x = 0.0f;
        f->ropePt[i].y = (f32) -i * (f->ropeLen / 30.0f);
        f->ropePt[i].z = 0.0f;
    }
}

s32 fn_1_7CD8(M579Fish *f, Vec *a)
{
    Vec saved;
    f32 x;
    f32 y;
    f32 z;
    f32 len;
    s32 ret;

    ret = 0;
    PSVECSubtract(&f->ropePos, a, &f->ropePos);
    saved = f->ropePos;
    f->ropeVel.y = (f32) ((f64) f->ropeVel.y - 0.27240694444444447);
    PSVECAdd(&f->ropeVel, &f->ropePos, &f->ropePos);
    x = f->ropePos.x;
    y = f->ropePos.y;
    z = f->ropePos.z;
    len = (f32) sqrt(z * z + (x * x + y * y));
    if (len > f->ropeLen) {
        f->ropePos.x = f->ropeLen * (x / len);
        f->ropePos.y = f->ropeLen * (y / len);
        f->ropePos.z = f->ropeLen * (z / len);
        ret = 1;
    }
    PSVECSubtract(&f->ropePos, &saved, &f->ropeVel);
    PSVECScale(&f->ropeVel, &f->ropeVel, 0.99f);
    if (f->ropeFlag[29] != 0) {
        PSVECScale(&f->ropeVel, &f->ropeVel, 0.9f);
    } else {
        PSVECScale(&f->ropeVel, &f->ropeVel, 0.99f);
    }
    return ret;
}

void fn_1_805C(M579Fish *f, s32 idx, Vec *a, f32 t)
{
    Vec p0;
    Vec p1;
    Vec p2;

    PSVECScale(a, &p0, t);
    PSVECScale(&f->ropeVel, &p1, (f32) (s32) (1.0 - t));
    PSVECAdd(&p0, &p1, &p2);
    p2.y = (f32) ((f64) p2.y - 0.27240694444444447);
    PSVECAdd(&f->ropeFrc[idx], &p2, &f->ropeFrc[idx]);
    PSVECAdd(&f->ropeFrc[idx], &f->ropePt[idx], &f->ropePt[idx]);
}

void fn_1_8180(M579Fish *f)
{
    Vec d;
    Vec *cur;
    Vec *prev;
    f32 seg;
    f32 mag;
    s32 i;

    seg = f->ropeLen / 29.0f;
    for (i = 1; i < 28; i++) {
        cur = &f->ropePt[i];
        prev = &f->ropePt[i - 1];
        PSVECSubtract(cur, prev, &d);
        mag = PSVECMag(&d);
        if (mag > seg) {
            PSVECScale(&d, cur, seg / mag);
            PSVECAdd(cur, prev, cur);
        }
    }
}

void fn_1_826C(M579Fish *f)
{
    Vec d;
    Vec *cur;
    Vec *next;
    f32 seg;
    f32 mag;
    s32 i;

    seg = f->ropeLen / 29.0f;
    for (i = 28; i > 0; i--) {
        cur = &f->ropePt[i];
        next = &f->ropePt[i + 1];
        PSVECSubtract(cur, next, &d);
        mag = PSVECMag(&d);
        if (mag > seg) {
            PSVECScale(&d, cur, seg / mag);
            PSVECAdd(cur, next, cur);
        }
    }
}

void fn_1_8358(M579Fish *f)
{
    Vec d;
    Vec *cur;
    f32 mag;
    f32 t;
    f32 lim;
    s32 i;

#ifdef TARGET_PC
    d.x = d.y = d.z = 0.0f; /* the original scales this uninitialised vector; its result is overwritten below */
#endif
    f->ropePt[0].x = 0.0f;
    f->ropePt[0].y = 0.0f;
    f->ropePt[0].z = 0.0f;
    f->ropePt[29] = f->ropePos;
    for (i = 1; i < 29; i++) {
        t = (f32) i / 29.0f;
        cur = &f->ropePt[i];
        lim = f->ropeLen * t;
        mag = PSVECMag(cur);
        if (mag > lim) {
            PSVECScale(&d, &d, lim / mag);
        }
        lim = f->ropeLen * (1.0f - t);
        PSVECSubtract(cur, &f->ropePos, &d);
        mag = PSVECMag(&d);
        if (mag > lim) {
            PSVECScale(&d, cur, lim / mag);
            PSVECAdd(cur, &f->ropePos, cur);
        }
    }
}

void fn_1_84EC(M579Fish *f, Vec *a, u8 flag)
{
    Vec old;
    Vec p0;
    Vec p1;
    Vec p2;
    s32 t;
    s32 i;

    for (i = 0; i < 30; i++) {
        old = f->ropePt[i];
        t = (s32) ((f32) i / 29.0f);
        if (flag != 0) {
            PSVECScale(&f->ropePos, &f->ropePt[i], (f32) t);
            PSVECSubtract(&f->ropePt[i], &old, &f->ropeFrc[i]);
        } else {
            PSVECScale(a, &p0, (f32) t);
            PSVECScale(&f->ropeVel, &p1, (f32) (s32) (1.0 - (f64) (f32) t));
            PSVECAdd(&p0, &p1, &p2);
            p2.y = (f32) ((f64) p2.y - 0.27240694444444447);
            PSVECAdd(&f->ropeFrc[i], &p2, &f->ropeFrc[i]);
            PSVECAdd(&f->ropeFrc[i], &f->ropePt[i], &f->ropePt[i]);
        }
    }
    if (flag == 0) {
        fn_1_8358(f);
        fn_1_826C(f);
        fn_1_8180(f);
    }
    for (i = 0; i < 30; i++) {
        if (f->ropeFlag[i] != 0) {
            PSVECScale(&f->ropeFrc[i], &f->ropeFrc[i], 0.7f);
        } else {
            PSVECScale(&f->ropeFrc[i], &f->ropeFrc[i], 0.8f);
        }
    }
}
