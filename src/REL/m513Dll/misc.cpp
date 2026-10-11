#include "REL/m513Dll.h"

/* The vector helpers at the tail of this unit work on a three component vector
 * held in double precision, at offsets 0x00 / 0x08 / 0x10. */
typedef struct M513VecD_s {
    /* 0x00 */ f64 x;
    /* 0x08 */ f64 y;
    /* 0x10 */ f64 z;
} M513_VECD;

extern "C" {

/* Multiply a double precision vector by a scalar and keep the result. */
M513_VECD *fn_1_114CC(const M513_VECD *vec, M513_VECD *dst, f64 scale)
{
    dst->x = vec->x * scale;
    dst->y = vec->y * scale;
    dst->z = vec->z * scale;
    return dst;
}

/* Multiply a double precision vector by a scalar and keep the result. */
M513_VECD *fn_1_114F8(const M513_VECD *vec, M513_VECD *dst, f64 scale)
{
    dst->x = vec->x * scale;
    dst->y = vec->y * scale;
    dst->z = vec->z * scale;
    return dst;
}

/* Dot product of two double precision vectors. */
f64 fn_1_11744(const M513_VECD *a, const M513_VECD *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

/* Dot product of two double precision vectors. */
f64 fn_1_11774(const M513_VECD *a, const M513_VECD *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

/* Dot product of a double precision vector with a single precision one. */
f64 fn_1_117A4(const M513_VECD *a, const HuVecF *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

} // extern "C"
