/*
 * Deterministic sin/cos/tan/atan/atan2/asin/acos (include/port/detmath.h): fdlibm's algorithms, double precision,
 * only + - * / and sqrt, so the PC and the phone compute the same bits. Compiled without FP contraction on every
 * platform (MSVC /fp:precise, clang -ffp-contract=off): a fused multiply-add would round differently.
 *
 * shortcut: three-part Cody-Waite reduction by pi/2, exact for |x| < 2^19 * pi/2 (the game's angles are a few
 * turns); larger arguments are first brought into [-pi, pi] with fmod (exact), which costs some accuracy.
 */
#define MP5_DETMATH_IMPL
#include "port/detmath.h"

static const double invpio2 = 6.36619772367581382433e-01;
static const double pio2_1 = 1.57079632673412561417e+00;  /* first 33 bits of pi/2 */
static const double pio2_2 = 6.07710050630396597660e-11;  /* next 33 bits */
static const double pio2_2t = 2.02226624879595063154e-21; /* pi/2 - (pio2_1 + pio2_2) */
static const double pi = 3.14159265358979311600e+00;
static const double pi_o_2 = 1.57079632679489655800e+00;
static const double twopi = 6.28318530717958623200e+00;

static const double S1 = -1.66666666666666324348e-01, S2 = 8.33333333332248946124e-03,
                    S3 = -1.98412698298579493134e-04, S4 = 2.75573137070700676789e-06,
                    S5 = -2.50507602534068634195e-08, S6 = 1.58969099521155010221e-10;
static const double C1 = 4.16666666666666019037e-02, C2 = -1.38888888888741095749e-03,
                    C3 = 2.48015872894767294178e-05, C4 = -2.75573143513906633035e-07,
                    C5 = 2.08757232129817482790e-09, C6 = -1.13596475577881948265e-11;

static double ksin(double y)
{
    const double z = y * y;
    return y + y * z * (S1 + z * (S2 + z * (S3 + z * (S4 + z * (S5 + z * S6)))));
}

static double kcos(double y)
{
    const double z = y * y;
    const double r = z * (C1 + z * (C2 + z * (C3 + z * (C4 + z * (C5 + z * C6)))));
    const double hz = 0.5 * z;
    const double w = 1.0 - hz;
    return w + (((1.0 - w) - hz) + z * r);
}

/* x = n * pi/2 + y, |y| <= pi/4; returns n mod 4 */
static int reduce(double x, double *y)
{
    double n;
    if (fabs(x) > 823549.6) { /* 2^19 * pi/2 */
        x = fmod(x, twopi);
    }
    n = floor(x * invpio2 + 0.5);
    *y = ((x - n * pio2_1) - n * pio2_2) - n * pio2_2t;
    return (int)((long long)n & 3);
}

double Mp5_sin(double x)
{
    double y;
    if (x != x || isinf(x)) {
        return x - x;
    }
    if (fabs(x) < 7.85398163397448278999e-01) {
        return ksin(x);
    }
    switch (reduce(x, &y)) {
    case 0: return ksin(y);
    case 1: return kcos(y);
    case 2: return -ksin(y);
    default: return -kcos(y);
    }
}

double Mp5_cos(double x)
{
    double y;
    if (x != x || isinf(x)) {
        return x - x;
    }
    if (fabs(x) < 7.85398163397448278999e-01) {
        return kcos(x);
    }
    switch (reduce(x, &y)) {
    case 0: return kcos(y);
    case 1: return -ksin(y);
    case 2: return -kcos(y);
    default: return ksin(y);
    }
}

double Mp5_tan(double x)
{
    double y, s, c;
    int n;
    if (x != x || isinf(x)) {
        return x - x;
    }
    n = fabs(x) < 7.85398163397448278999e-01 ? (y = x, 0) : reduce(x, &y);
    s = ksin(y);
    c = kcos(y);
    return (n & 1) ? -c / s : s / c;
}

static const double atanhi[] = {4.63647609000806093515e-01, 7.85398163397448278999e-01, 9.82793723247329054082e-01,
                                1.57079632679489655800e+00};
static const double atanlo[] = {2.26987774529616870924e-17, 3.06161699786838301793e-17, 1.39033110312309984516e-17,
                                6.12323399573676603587e-17};
static const double aT[] = {3.33333333333329318027e-01,  -1.99999999998764832476e-01, 1.42857142725034663711e-01,
                            -1.11111104054623557880e-01, 9.09088713343650656196e-02,  -7.69187620504482999495e-02,
                            6.66107313738753120669e-02,  -5.83357013379057348645e-02, 4.97687799461593236017e-02,
                            -3.65315727442169155270e-02, 1.62858201153657823623e-02};

double Mp5_atan(double x)
{
    double ax, z, w, s1, s2;
    int id;
    if (x != x) {
        return x;
    }
    ax = fabs(x);
    if (ax >= 7.378697629483821e19) { /* 2^66: atan(x) is pi/2 to the last bit */
        return x > 0 ? atanhi[3] + atanlo[3] : -atanhi[3] - atanlo[3];
    }
    if (ax < 0.4375) {
        id = -1;
    } else if (ax < 0.6875) {
        id = 0;
        ax = (2.0 * ax - 1.0) / (2.0 + ax);
    } else if (ax < 1.1875) {
        id = 1;
        ax = (ax - 1.0) / (ax + 1.0);
    } else if (ax < 2.4375) {
        id = 2;
        ax = (ax - 1.5) / (1.0 + 1.5 * ax);
    } else {
        id = 3;
        ax = -1.0 / ax;
    }
    z = ax * ax;
    w = z * z;
    s1 = z * (aT[0] + w * (aT[2] + w * (aT[4] + w * (aT[6] + w * (aT[8] + w * aT[10])))));
    s2 = w * (aT[1] + w * (aT[3] + w * (aT[5] + w * (aT[7] + w * aT[9]))));
    if (id < 0) {
        z = ax - ax * (s1 + s2);
    } else {
        z = atanhi[id] - ((ax * (s1 + s2) - atanlo[id]) - ax);
    }
    return x < 0 ? -z : z;
}

double Mp5_atan2(double y, double x)
{
    double z;
    if (x != x || y != y) {
        return x + y;
    }
    if (y == 0.0) {
        /* atan2(+-0, +x or +0) = +-0, atan2(+-0, -x or -0) = +-pi */
        return signbit(x) ? (signbit(y) ? -pi : pi) : y;
    }
    if (x == 0.0) {
        return y > 0 ? pi_o_2 : -pi_o_2;
    }
    if (isinf(x)) {
        if (isinf(y)) {
            z = x > 0 ? pi / 4 : 3 * pi / 4;
        } else {
            z = x > 0 ? 0.0 : pi;
        }
        return y > 0 ? z : -z;
    }
    if (isinf(y)) {
        return y > 0 ? pi_o_2 : -pi_o_2;
    }
    z = Mp5_atan(fabs(y / x));
    if (x < 0) {
        z = pi - z;
    }
    return y < 0 ? -z : z;
}

double Mp5_asin(double x)
{
    return Mp5_atan2(x, sqrt((1.0 - x) * (1.0 + x)));
}

double Mp5_acos(double x)
{
    return Mp5_atan2(sqrt((1.0 - x) * (1.0 + x)), x);
}

float Mp5_sinf(float x) { return (float)Mp5_sin(x); }
float Mp5_cosf(float x) { return (float)Mp5_cos(x); }
float Mp5_tanf(float x) { return (float)Mp5_tan(x); }
float Mp5_atanf(float x) { return (float)Mp5_atan(x); }
float Mp5_atan2f(float y, float x) { return (float)Mp5_atan2(y, x); }
float Mp5_asinf(float x) { return (float)Mp5_asin(x); }
float Mp5_acosf(float x) { return (float)Mp5_acos(x); }

/* a*b is exact in double (24 + 24 bits < 53), so this rounds twice (to double, then to float) where a fused fmaf
   rounds once: a rare last-bit difference from the hardware, the same on every platform */
float Mp5_fmaf(float a, float b, float c) { return (float)((double)a * (double)b + (double)c); }
