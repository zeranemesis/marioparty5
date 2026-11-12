// m513Dll misc: vector / quaternion helpers and the double precision "mtx64"
// plane and triangle maths.
#include "REL/m513Dll.h"
#include "math.h"

extern "C" {
void PSVECNormalize(const Vec *src, Vec *dst);
f32 PSVECDotProduct(const Vec *a, const Vec *b);
void PSVECCrossProduct(const Vec *a, const Vec *b, Vec *dst);
}

typedef struct M513Vec64_s {
    f64 x;
    f64 y;
    f64 z;
} M513Vec64;

// Plane: unit normal and offset.
typedef struct M513Plane64_s {
    M513Vec64 n;
    f64 d;
} M513Plane64;

extern "C" {

char lbl_1_data_7A0[] = "Invalid Axis";
char lbl_1_data_7AD[] = "!\"Invalid Axis\"";
char lbl_1_data_7BD[] = "misc.cpp";
char lbl_1_data_7C6[] = "mag != 0.0";
char lbl_1_data_7D1[] = "mtx64.h";

M513Vec64 *fn_1_114CC(const M513Vec64 *vec, M513Vec64 *dst, f64 scale);
M513Vec64 *fn_1_114F8(const M513Vec64 *vec, M513Vec64 *dst, f64 scale);
f64 fn_1_11524(const M513Vec64 *vec);
f32 fn_1_116A8(const M513Vec64 *vec);
M513Vec64 *fn_1_116DC(const M513Vec64 *a, const M513Vec64 *b, M513Vec64 *dst);
f64 fn_1_11744(const M513Vec64 *a, const M513Vec64 *b);
f32 fn_1_11774(const M513Vec64 *a, const M513Vec64 *b);
f64 fn_1_117A4(const M513Vec64 *a, const Vec *b);
M513Vec64 *fn_1_117D4(const Vec *a, const Vec *b, M513Vec64 *dst);
M513Vec64 *fn_1_1180C(const Vec *a, const M513Vec64 *b, M513Vec64 *dst);
void fn_1_11440(M513Plane64 *src, M513Plane64 *dst);

// Point on the line (a -> c) offset from a by a weighted spring/length term.
Vec fn_1_10964(Vec *a, Vec *b, Vec *c, f32 k, f32 w1, f32 w2, f32 len)
{
    Vec ab;
    Vec cb;
    Vec cbScaled;
    Vec res;
    f32 mag;
    f32 dot;
    f32 t;

    PSVECSubtract(a, b, &ab);
    PSVECSubtract(c, b, &cb);
    PSVECScale(&cb, &cbScaled, k);
    mag = PSVECMag(&ab);
    dot = PSVECDotProduct(&ab, &cbScaled);
    t = (w1 * (len - PSVECMag(&ab))) + (w2 * (dot / mag));
    PSVECNormalize(&ab, &ab);
    PSVECScale(&ab, &ab, t);
    PSVECAdd(&ab, a, &res);
    return res;
}

// Quaternion rotating direction a onto direction b.
void fn_1_10AB8(Quaternion *q, Vec *a, Vec *b)
{
    Vec na;
    Vec nb;
    Vec axis;
    f64 dot;
    f64 s;
    f64 inv;

    PSVECNormalize(a, &na);
    PSVECNormalize(b, &nb);
    dot = PSVECDotProduct(&na, &nb);
    s = sqrt(2.0 + (2.0 * dot));
    q->w = s * 0.5;
    inv = 1.0 / s;
    PSVECCrossProduct(&na, &nb, &axis);
    q->x = axis.x * inv;
    q->y = axis.y * inv;
    q->z = axis.z * inv;
}

// Rotated unit axis of a quaternion; axis is 'X'/'Y'/'Z', negated when the
// code is negative.
void fn_1_10E14(Vec *out, Quaternion *q, u32 axisCode)
{
    f32 qx = q->x;
    f32 qy = q->y;
    f32 qz = q->z;
    f32 qw = q->w;
    u32 neg = axisCode >> 31;
    s32 axis;
    f64 x;
    f64 y;
    f64 z;

    if ((u8)neg != 0) {
        axis = -(s32)axisCode;
    } else {
        axis = (s32)axisCode;
    }
    switch (axis) {
        case 'X':
            x = 1.0 + (-2.0 * (((f64)qy * (f64)qy) + ((f64)qz * (f64)qz)));
            y = 2.0 * (((f64)qx * (f64)qy) - ((f64)qw * (f64)qz));
            z = 2.0 * (((f64)qx * (f64)qz) + ((f64)qw * (f64)qy));
            break;
        case 'Y':
            x = 2.0 * (((f64)qx * (f64)qy) + ((f64)qw * (f64)qz));
            y = 1.0 + (-2.0 * (((f64)qx * (f64)qx) - ((f64)qz * (f64)qz)));
            z = 2.0 * (((f64)qy * (f64)qz) + ((f64)qw * (f64)qx));
            break;
        case 'Z':
            x = 2.0 * (((f64)qx * (f64)qz) + ((f64)qw * (f64)qy));
            y = 2.0 * (((f64)qy * (f64)qz) - ((f64)qw * (f64)qx));
            z = 1.0 + (-2.0 * (((f64)qx * (f64)qx) + ((f64)qy * (f64)qy)));
            break;
        default:
            M513_ASSERT(!"Invalid Axis", lbl_1_data_7AD, lbl_1_data_7BD, 0x50);
            return;
    }
    if ((u8)neg != 0) {
        x = -x;
        y = -y;
        z = -z;
    }
    out->x = x;
    out->y = y;
    out->z = z;
}

// Reflect: dst = v - 2 (n . v) n
void fn_1_110A4(Vec *dst, Vec *n, Vec *v)
{
    f32 k;
    f32 d = PSVECDotProduct(n, v);

    k = -2.0f * d;

    PSVECScale(n, dst, k);
    PSVECAdd(v, dst, dst);
}

// Plane through three points.
void fn_1_11144(M513Plane64 *plane, Vec *a, Vec *b, Vec *c)
{
    M513Vec64 ab;
    M513Vec64 bc;

    fn_1_117D4(a, b, &ab);
    fn_1_117D4(b, c, &bc);
    fn_1_116DC(&ab, &bc, &plane->n);
    fn_1_11440(plane, plane);
    plane->d = -fn_1_117A4(&plane->n, a);
}

// Signed distance of a point from the plane.
f64 fn_1_111D8(M513Plane64 *plane, Vec *p)
{
    return plane->d + fn_1_117A4(&plane->n, p);
}

// Intersection of the segment a-b with a plane given by the two distances.
void fn_1_11218(Vec *out, Vec *a, Vec *b, f64 d0, f64 d1)
{
    M513Vec64 v;
    f64 diff = d1 - d0;

    if (0.0 != diff) {
        fn_1_117D4(b, a, &v);
        fn_1_114F8(&v, &v, -d0 / diff);
        fn_1_1180C(a, &v, &v);
        out->x = v.x;
        out->y = v.y;
        out->z = v.z;
        return;
    }
    out->x = b->x;
    out->y = b->y;
    out->z = b->z;
}

// Point-in-triangle test against the plane normal.
s32 fn_1_11310(Vec *a, Vec *b, Vec *c, Vec *p, M513Vec64 *normal)
{
    M513Vec64 pa;
    M513Vec64 pb;
    M513Vec64 pc;
    M513Vec64 cross;
    s32 inside;
    u8 ok;

    fn_1_117D4(a, p, &pa);
    fn_1_117D4(b, p, &pb);
    fn_1_117D4(c, p, &pc);
    inside = 0;
    ok = 0;
    fn_1_116DC(&pa, &pb, &cross);
    if (fn_1_11774(&cross, normal) >= 0.0) {
        fn_1_116DC(&pb, &pc, &cross);
        if (fn_1_11774(&cross, normal) >= 0.0) {
            ok = 1;
        }
    }
    if (ok != 0) {
        fn_1_116DC(&pc, &pa, &cross);
        if (fn_1_11774(&cross, normal) >= 0.0) {
            inside = 1;
        }
    }
    return inside;
}

// Normalise the plane normal (offset left untouched).
void fn_1_11440(M513Plane64 *src, M513Plane64 *dst)
{
    M513Plane64 *s = src;
    f32 mag = fn_1_11524(&dst->n);

    (void)s;
    M513_ASSERT(0.0 != mag, lbl_1_data_7C6, lbl_1_data_7D1, 0x70);
    fn_1_114CC(&src->n, &dst->n, 1.0 / (f64)mag);
}

M513Vec64 *fn_1_114CC(const M513Vec64 *vec, M513Vec64 *dst, f64 scale)
{
    dst->x = vec->x * scale;
    dst->y = vec->y * scale;
    dst->z = vec->z * scale;
    return dst;
}

M513Vec64 *fn_1_114F8(const M513Vec64 *vec, M513Vec64 *dst, f64 scale)
{
    dst->x = vec->x * scale;
    dst->y = vec->y * scale;
    dst->z = vec->z * scale;
    return dst;
}

// Length of a double vector (sqrtf of the squared length).
f64 fn_1_11524(const M513Vec64 *vec)
{
    return sqrt(fn_1_116A8(vec));
}

f32 fn_1_116A8(const M513Vec64 *vec)
{
    return fn_1_11744(vec, vec);
}

M513Vec64 *fn_1_116DC(const M513Vec64 *a, const M513Vec64 *b, M513Vec64 *dst)
{
    dst->x = (a->y * b->z) - (a->z * b->y);
    dst->y = (a->z * b->x) - (a->x * b->z);
    dst->z = (a->x * b->y) - (a->y * b->x);
    return dst;
}

f64 fn_1_11744(const M513Vec64 *a, const M513Vec64 *b)
{
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

f32 fn_1_11774(const M513Vec64 *a, const M513Vec64 *b)
{
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

f64 fn_1_117A4(const M513Vec64 *a, const Vec *b)
{
    return (a->x * (f64)b->x) + (a->y * (f64)b->y) + (a->z * (f64)b->z);
}

M513Vec64 *fn_1_117D4(const Vec *a, const Vec *b, M513Vec64 *dst)
{
    dst->x = (f64)a->x - (f64)b->x;
    dst->y = (f64)a->y - (f64)b->y;
    dst->z = (f64)a->z - (f64)b->z;
    return dst;
}

M513Vec64 *fn_1_1180C(const Vec *a, const M513Vec64 *b, M513Vec64 *dst)
{
    dst->x = (f64)a->x + b->x;
    dst->y = (f64)a->y + b->y;
    dst->z = (f64)a->z + b->z;
    return dst;
}

} // extern "C"
