#ifndef _M539DLL_H
#define _M539DLL_H

// Declared here rather than through humath.h/dolphin/mtx.h: in C++ those
// headers pull in MSL math.h, whose inline sqrtf emits its own constants
// into .rodata and would shift every literal of the module.
#include "dolphin/types.h"

extern "C" {

typedef struct {
    f32 x, y, z;
} Vec, *VecPtr, Point3d, *Point3dPtr;

void PSVECAdd(const Vec *a, const Vec *b, Vec *dst);
void PSVECSubtract(const Vec *a, const Vec *b, Vec *dst);
void PSVECNormalize(Vec *v, Vec *dst);
void PSVECScale(const Vec *src, Vec *dst, f32 scale);
f32 PSVECMag(const Vec *v);
double atan2(double y, double x);
double sin(double x);
double cos(double x);
double fmod(double x, double y);

// Pointer-to-member-function slot. The GameCube build keeps the MWCC layout
// (this-delta, vtable index, function address); on PC the slot is just the
// function pointer, called directly with the object.
#ifdef TARGET_PC
typedef struct M539Ptmf_s {
    void (*fn)(void *self);
} M539Ptmf;
#define M539_PTMF_INIT(f) { (void (*)(void *))(f) }
#define M539_PTMF_NULL_INIT { 0 }
#define M539_PTMF_SET_FROM(dst, tbl, f) ((dst).fn = (void (*)(void *))(f))
#define M539_PTMF_NULL M539PtmfNull
static const M539Ptmf M539PtmfNull = { 0 };
#define M539PtmfTest(p) ((p)->fn != 0)
#define M539PtmfCall(obj, p) ((p)->fn(obj))
#else
typedef struct M539Ptmf_s {
    s32 delta;
    s32 vidx;
    void *fn;
} M539Ptmf;
#define M539_PTMF_INIT(f) { 0, -1, (void *)(f) }
#define M539_PTMF_NULL_INIT { 0, -1, 0 }
#define M539_PTMF_SET_FROM(dst, tbl, f) ((dst).delta = (tbl).delta, (dst).vidx = (tbl).vidx, (dst).fn = (void *)(f))
#define M539_PTMF_NULL __ptmf_null
extern M539Ptmf __ptmf_null;
s32 __ptmf_test(M539Ptmf *p);
void __ptmf_scall(void *obj, M539Ptmf *p);
#define M539PtmfTest(p) __ptmf_test(p)
#define M539PtmfCall(obj, p) __ptmf_scall((obj), (p))
#endif

typedef struct M539Heiho_s {
    s32 curIdx;
    Vec pos;
    Vec target;
    Vec dir;
    f32 angle;
    f32 heading;
    void *actor;
    Point3d *path;
    s32 pathCount;
} M539Heiho;

void fn_1_C8(void *ptr);
void *fn_1_558C(void *self, s32 prio);
void *fn_1_5608(void *self, s16 arg);
void fn_1_56BC(void *self, s32 arg);
void fn_1_574C(void *self, s32 arg);

void *fn_1_72E8(s32 id);
void fn_1_2E08(void *actor, u32 flags);
void fn_1_2EF4(void *actor, Vec *pos);
void fn_1_2F3C(void *actor, Vec *rot);
void fn_1_9EC0(M539Heiho *self);

}

#endif
