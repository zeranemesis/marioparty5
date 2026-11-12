#include "dolphin/types.h"
#include "dolphin/mtx.h"
#include <math.h>

// Quaternion-style 4-float record: x/y/z imaginary part, w = unkC.
struct M568Quat {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};

static inline bool NonNeg(f32 v) { return v >= 0.0f; }

extern "C" {

void fn_1_7EC(const char *expr, const char *file, int line);

char lbl_1_data_538[] = "Invalid Axis";
char lbl_1_data_545[] = "!\"Invalid Axis\"";
char lbl_1_data_555[] = "misc.cpp";

// Builds the rotation (axis * 1/sin, half-angle sine) taking a onto b.
void fn_1_ED98(M568Quat *out, Point3d *volatile a, Point3d *volatile b)
{
    Point3d na, nb, cross;
    f64 dot;
    f64 s;
    f64 inv;

    PSVECNormalize(a, &na);
    PSVECNormalize(b, &nb);
    dot = PSVECDotProduct(&na, &nb);
    s = sqrt(2.0 + 2.0 * dot);
    out->w = s * 0.5;
    inv = 1.0 / s;
    PSVECCrossProduct(&na, &nb, &cross);
    out->x = cross.x * inv;
    out->y = cross.y * inv;
    out->z = cross.z * inv;
}

// Rotates the unit axis selected by 'X'/'Y'/'Z' (negative = inverted) by quat.
void fn_1_F0F4(Point3d *out, M568Quat *q, u32 axis)
{
    f32 x = q->x;
    f32 y = q->y;
    f32 z = q->z;
    f32 w = q->w;
    u8 neg = axis >> 31;
    s32 code = neg ? -(s32)axis : (s32)axis;
    f64 ox, oy, oz;

    switch (code) {
    case 0x58:
        ox = 1.0 + -2.0 * (y * y + z * z);
        oy = 2.0 * (x * y - w * z);
        oz = 2.0 * (x * z + w * y);
        break;
    case 0x59:
        ox = 2.0 * (x * y + w * z);
        oy = 1.0 + -2.0 * (x * x - z * z);
        oz = 2.0 * (y * z + w * x);
        break;
    case 0x5A:
        ox = 2.0 * (x * z + w * y);
        oy = 2.0 * (y * z - w * x);
        oz = 1.0 + -2.0 * (x * x + y * y);
        break;
    default:
        if (lbl_1_data_538 == NULL) {
        } else {
            fn_1_7EC(lbl_1_data_545, lbl_1_data_555, 0x52);
        }
#ifdef TARGET_PC
        out->x = out->y = out->z = 0.0f;
#endif
        return;
    }
    if (neg) {
        ox = -ox;
        oy = -oy;
        oz = -oz;
    }
    out->x = ox;
    out->y = oy;
    out->z = oz;
}

// Reflects v about the normal n: out = v - 2 (n . v) n.
void fn_1_F384(Point3d *out, Point3d *n, Point3d *v)
{
    f32 d = PSVECDotProduct(n, v);
    f32 k = -2.0f * d;
    PSVECScale(n, out, k);
    PSVECAdd(v, out, out);
}

// Plane (normal in xyz, offset in w) through three points.
void fn_1_F424(M568Quat *plane, Point3d *p0, Point3d *p1, Point3d *volatile p2)
{
    Point3d d0, d1;
    
    PSVECSubtract(p0, p1, &d0);
    PSVECSubtract(p1, p2, &d1);
    PSVECCrossProduct(&d0, &d1, (Point3d *)plane);
    PSVECNormalize((Point3d *)plane, (Point3d *)plane);
    f32 d = PSVECDotProduct((Point3d *)plane, p0);
    plane->w = -d;
}

// Signed distance from the plane to a point.
f32 fn_1_F4C4(M568Quat *plane, Point3d *volatile p)
{
    f32 d = PSVECDotProduct((Point3d *)plane, p);
    return plane->w + d;
}

// Point on segment a->b where the signed distance crosses zero.
void fn_1_F518(Point3d *out, Point3d *a, Point3d *b, f32 da, volatile f32 db)
{
    Point3d d;
    f32 range = db - da;

    if (range != 0.0f) {
        PSVECSubtract(b, a, &d);
        PSVECScale(&d, &d, -da / range);
        PSVECAdd(a, &d, out);
        return;
    }
    out->x = b->x;
    out->y = b->y;
    out->z = b->z;
}

// Tests whether direction n lies inside the cone/triangle a,b,c seen from o.
s32 fn_1_F5EC(Point3d *volatile a, Point3d *volatile b, Point3d *volatile c, Point3d *o, Point3d *n)
{
    Point3d va, vb, vc, cross;
    s32 result;
    u8 inside;
    f32 d;

    PSVECSubtract(a, o, &va);
    PSVECSubtract(b, o, &vb);
    PSVECSubtract(c, o, &vc);
    result = 0;
    inside = 0;
    PSVECCrossProduct(&va, &vb, &cross);
    if (NonNeg(d = PSVECDotProduct(&cross, n))) {
        PSVECCrossProduct(&vb, &vc, &cross);
        if (NonNeg(d = PSVECDotProduct(&cross, n))) {
            inside = 1;
        }
    }
    if (inside) {
        PSVECCrossProduct(&vc, &va, &cross);
        if (NonNeg(d = PSVECDotProduct(&cross, n))) {
            result = 1;
        }
    }
    return result;
}

}
