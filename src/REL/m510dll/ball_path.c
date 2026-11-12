#define _MATH_H
#include "REL/m510dll.h"
#include "string.h"

void fn_1_C590(M510_PATH *data)
{
    memset(data, 0, sizeof(M510_PATH));
}

void fn_1_C5C0(M510_PATH *data)
{
    memset(data, 0, sizeof(M510_PATH));
}

void fn_1_C5F0(M510_PATH *path)
{
    path->count = 0;
    memset(path->pts, 0, sizeof(path->pts));
    memset(path->seg, 0, sizeof(path->seg));
    memset(path->sum, 0, sizeof(path->sum));
}

void fn_1_C650(M510_PATH *path, M510_VECF *pt, s32 len)
{
    if (path->count < 24) {
        path->pts[path->count] = *pt;
        path->seg[path->count] = len;
        if (path->count != 0) {
            path->sum[path->count] = path->sum[path->count - 1] + path->seg[path->count];
        } else {
            path->sum[path->count] = path->seg[path->count];
        }
        path->count++;
    }
}

void fn_1_C70C(M510_VECF *out, M510_PATH *path, s32 dist, f32 tension)
{
    M510_VECF res = { 0.0f, 0.0f, 0.0f };
    M510_VECF tan0;
    M510_VECF tan1;
    M510_VECF p0;
    M510_VECF p1;
    M510_VECF a;
    M510_VECF b;
    M510_VECF c;
    M510_VECF d;
    f32 t;
    f32 t2;
    s32 clamped;
    s32 i;
    s32 idx;

    if (dist > path->sum[path->count - 1]) {
        clamped = path->sum[path->count - 1];
    } else if (dist < 0) {
        clamped = 0;
    } else {
        clamped = dist;
    }
    for (i = 0; i < path->count; i++) {
        if (clamped <= path->sum[i]) {
            if (i == 0) {
                t = (f32)clamped / (f32)path->sum[0];
            } else {
                t = (f32)(clamped - path->sum[i - 1]) / (f32)(path->sum[i] - path->sum[i - 1]);
            }
            idx = i;
            if (idx == path->count - 1) {
                idx--;
                t = 1.0f;
            }
            p0 = path->pts[idx];
            p1 = path->pts[idx + 1];
            fn_1_CAD0(&tan0, path, idx, tension);
            fn_1_CAD0(&tan1, path, idx + 1, tension);
            if (t > 1.0f) {
                t2 = 1.0f;
            } else if (t < 0.0f) {
                t2 = 0.0f;
            } else {
                t2 = t;
            }
            a = tan1;
            b = tan0;
            c = p1;
            d = p0;
            fn_1_CC58(&res, &d, &c, &b, &a, t2);
            break;
        }
    }
    *out = res;
}

void fn_1_CAD0(M510_VECF *out, M510_PATH *path, s32 idx, f32 tension)
{
    M510_VECF v;

    if (idx <= 0) {
        v.x = path->pts[1].x - path->pts[0].x;
        v.y = path->pts[1].y - path->pts[0].y;
        v.z = path->pts[1].z - path->pts[0].z;
    } else if (idx >= path->count - 1) {
        v.x = path->pts[path->count - 1].x - path->pts[path->count - 2].x;
        v.y = path->pts[path->count - 1].y - path->pts[path->count - 2].y;
        v.z = path->pts[path->count - 1].z - path->pts[path->count - 2].z;
    } else {
        v.x = tension * (path->pts[idx + 1].x - path->pts[idx - 1].x);
        v.y = tension * (path->pts[idx + 1].y - path->pts[idx - 1].y);
        v.z = tension * (path->pts[idx + 1].z - path->pts[idx - 1].z);
    }
    *out = v;
}

void fn_1_CC58(M510_VECF *out, M510_VECF *p0, M510_VECF *p1, M510_VECF *t0, M510_VECF *t1, f32 t)
{
    M510_VECF v;

    v.x = fn_1_CD18(t, p0->x, p1->x, t0->x, t1->x);
    v.y = fn_1_CD18(t, p0->y, p1->y, t0->y, t1->y);
    v.z = fn_1_CD18(t, p0->z, p1->z, t0->z, t1->z);
    *out = v;
}

f32 fn_1_CD18(f32 t, f32 p0, f32 p1, f32 t0, f32 t1)
{
    f32 ret;

    ret = (t1 * ((t - 1.0f) * (t * t)))
        + ((t0 * (t * ((1.0f - t) * (1.0f - t))))
        + ((p0 * ((1.0f + 2.0f * t) * ((t - 1.0f) * (t - 1.0f))))
        + (p1 * ((3.0f - 2.0f * t) * (t * t)))));
    return ret;
}
