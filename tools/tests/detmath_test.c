/* Self-check of src/port/detmath.c against the C library (1 ulp): cl /O2 /I include tools/tests/detmath_test.c src/port/detmath.c */
#define MP5_DETMATH_IMPL
#include "port/detmath.h"
#include <stdio.h>
#include <float.h>
static double maxerr(const char* n, double (*f)(double), double (*g)(double), double lo, double hi) {
    double m = 0; unsigned s = 12345;
    for (int i = 0; i < 1000000; i++) {
        s = s * 1103515245u + 12345u; double x = lo + (hi - lo) * (s / 4294967296.0);
        double a = f(x), b = g(x), e = fabs(a - b) / (fabs(b) > 1e-300 ? fabs(b) : 1);
        if (fabs(b) < 1e-3) e = fabs(a - b); /* near zeros: absolute */
        if (e > m) m = e;
    }
    printf("%-6s max err %.3g\n", n, m); return m;
}
static double ref_atan2_1(double x) { return atan2(x, 0.7); }
static double my_atan2_1(double x) { return Mp5_atan2(x, 0.7); }
static double ref_atan2_2(double x) { return atan2(0.3, x); }
static double my_atan2_2(double x) { return Mp5_atan2(0.3, x); }
int main(void) {
    double w = 0;
    w = fmax(w, maxerr("sin", Mp5_sin, sin, -100, 100));
    w = fmax(w, maxerr("cos", Mp5_cos, cos, -100, 100));
    w = fmax(w, maxerr("tan", Mp5_tan, tan, -1.5, 1.5));
    w = fmax(w, maxerr("atan", Mp5_atan, atan, -50, 50));
    w = fmax(w, maxerr("atan2a", my_atan2_1, ref_atan2_1, -50, 50));
    w = fmax(w, maxerr("atan2b", my_atan2_2, ref_atan2_2, -50, 50));
    w = fmax(w, maxerr("asin", Mp5_asin, asin, -1, 1));
    w = fmax(w, maxerr("acos", Mp5_acos, acos, -1, 1));
    printf("edge: %g %g %g %g %g\n", Mp5_atan2(0.0, -1.0), Mp5_atan2(-0.0, -1.0), Mp5_acos(1.0), Mp5_acos(-1.0), Mp5_sin(1e7) - sin(1e7));
    printf(w < 1e-13 ? "OK\n" : "FAIL\n");
    return w < 1e-13 ? 0 : 1;
}
