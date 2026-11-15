// m532Dll "m532": the module's runtime layer -- camera pose maths, an intrusive
// list of tasks, the game-master singleton plumbing, thin Hu3D/MgTimer wrappers,
// a Mersenne Twister, a sub-thread class and the C++ static initialisers.
#define _MATH_H
extern "C" {
#include "dolphin.h"
#include "humath.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/memory.h"
#include "game/gamemes.h"
#include "game/wipe.h"
#ifdef TARGET_PC /* MWCC intrinsic */
#define __fabs(x) ((x) < 0 ? -(x) : (x))
#endif
#include "game/flag.h"
#include "game/gamework.h"
#include "game/data.h"
#include "game/audio.h"
#include "game/pad.h"
#include "game/mgdata.h"
#include "game/charman.h"
#include "game/process.h"
#include "game/mg/timer.h"
#include "game/mg/actman.h"
#include "stdarg.h"
#include "stdio.h"
}
#include "REL/m532Dll/m532.h"

#ifdef __MWERKS__
#define M532_RODATA __declspec(section ".rodata")
#else
#define M532_RODATA
#endif

extern "C" {

double tan(double);
#ifndef TARGET_PC
void OSInitSemaphore(void *sem, s32 count);
s32 OSWaitSemaphore(void *sem);
s32 OSSignalSemaphore(void *sem);
#endif
double fabs(double);
void __register_global_object(void *obj, void *dtor, void *reg);
void ObjectSetup(void);
extern void (*_ctors[])(void);
extern void (*_dtors[])(void);

#define M532_ASSERT(cond, expr, file, line) ((cond) ? (void)0 : fn_1_1D1C((expr), (file), (line)))

// The inlined form of the assertion handler (singleton.h, model.cpp, ...).
#define M532_ASSERT_INL(cond, expr, file, line)                                      if (cond) {                                                                          (void)0;                                                                     } else {                                                                             char assertBuf[0x400];                                                           sprintf(assertBuf, lbl_1_data_DC, (file), (line), (expr));                       OSPanic(lbl_1_data_FA, 0x12, assertBuf);                                     }

// ---------------------------------------------------------------- types

// A pose viewed as the seven raw words the pose-table dump prints.
typedef union M532PoseBits_u {
    M532Pose pose;
    u32 w[7];
} M532PoseBits;

// Singleton game master (only the base part that m532 touches).
struct M532Master_s;
typedef struct M532MasterVtbl_s {
    void *rtti[2];
    struct M532Master_s *(*dtor)(struct M532Master_s *self, s16 flag);
    void (*setup)(struct M532Master_s *self, OMOBJMAN *objman);
    void (*update)(struct M532Master_s *self);
    void (*finish)(struct M532Master_s *self);
} M532MasterVtbl;

typedef struct M532Master_s {
    s32 pad0;
    M532MasterVtbl *vtbl;
    s32 slot[4];
    s32 mesId;
    s32 counter;
} M532Master;

// Sub-thread: vtable, OSThread, stack pointer, semaphore, argument.
struct M532SubThread_s;
typedef struct M532ThreadVtbl_s {
    void *rtti[2];
    struct M532SubThread_s *(*dtor)(struct M532SubThread_s *self, s16 flag);
    s32 (*run)(struct M532SubThread_s *self, s32 arg);
} M532ThreadVtbl;

typedef struct M532Thread_s {
    OSThread thread;
    u32 *stack;
} M532Thread;

#ifdef TARGET_PC
typedef OSSemaphore M532Sem; /* Aurora's semaphore is larger than the GC one */
#else
typedef struct M532Sem_s {
    s32 count;
    OSThreadQueue queue;
} M532Sem;
#endif

typedef struct M532SubThread_s {
    M532ThreadVtbl *vtbl;
    M532Thread th;
    M532Sem sem;
    s32 arg;
} M532SubThread;

// Mersenne Twister state.
typedef struct M532Rand_s {
    u32 mt[0x270];
    u32 idx;
} M532Rand;

// Functor kept in an object's data slot: slot +0xC runs it.
struct M532Functor_s;
typedef struct M532FunctorVtbl_s {
    void *rtti[2];
    struct M532Functor_s *(*dtor)(struct M532Functor_s *self, s16 flag);
    void (*run)(struct M532Functor_s *self, OMOBJ *obj);
} M532FunctorVtbl;

typedef struct M532Functor_s {
    M532FunctorVtbl *vtbl;
} M532Functor;

typedef struct M532TimerObj_s {
    MGTIMER *timer;
} M532TimerObj;

// ---------------------------------------------------------------- data

char lbl_1_data_C8[] = "ptr != NULL";
char lbl_1_data_D4[] = "new.cpp";
char lbl_1_data_DC[] = "\nASSERTION FAILED at %s:%d\n%s";
char lbl_1_data_FA[] = "assert.cpp";
char lbl_1_data_105[] = "{{0x%08x,0x%08x,0x%08x},{0x%08x,0x%08x,0x%08x,0x%08x}},\n";
char lbl_1_data_13E[] = "handle.cpp";
char lbl_1_data_149[] = "n <= MAXCOUNT";
char lbl_1_data_157[] = "singleton.h";
char lbl_1_data_163[] = "ms_pointer == 0";
char lbl_1_data_173[] = "ms_pointer != 0";
char lbl_1_data_183[] = "model.cpp";
char lbl_1_data_18D[] = "p != NULL";
char lbl_1_data_197[] = "om.cpp";
char lbl_1_data_19E[] = "subthread.cpp";
char lbl_1_data_1AC[] = "::OSGetCurrentThread() == m_th";
char lbl_1_data_1CB[] = "!::OSIsThreadTerminated(m_th)";
char lbl_1_data_1E9[] = "thread.cpp";
char lbl_1_data_1F4[] = "priority >= OS_PRIORITY_MIN && priority <= OS_PRIORITY_MAX";
char lbl_1_data_22F[] = "OSCreateThread() failed.";
char lbl_1_data_248[] = "delete CThread@%p\nstack remain/total = 0x%08x/0x%08x (%f%%)\n";
char lbl_1_data_285[] = "stack overflow\n";
char lbl_1_data_295[] = "timer.cpp";
char lbl_1_data_29F[] = "false";

M532SubThread *fn_1_73A0(M532SubThread *self, s16 flag);
M532Master *fn_1_408C(M532Master *self, s16 flag);
void fn_1_442C(M532Master *self);
M532Functor *fn_1_68F4(M532Functor *self, s16 flag);
void fn_1_6880(M532Functor *self, OMOBJ *obj);
void fn_1_6834(OMOBJ *obj);

M532ThreadVtbl lbl_1_data_2A8 = { { 0, 0 }, fn_1_73A0, 0 };
M532FunctorVtbl lbl_1_data_2B8 = { { 0, 0 }, fn_1_68F4, fn_1_6880 };
M532MasterVtbl lbl_1_data_2C8 = { { 0, 0 }, fn_1_408C, 0, fn_1_442C, 0 };

// ---------------------------------------------------------------- bss

u8 lbl_1_bss_18[0xC];
M532Node lbl_1_bss_24;
s32 lbl_1_bss_2C;
s32 lbl_1_bss_30;
s32 lbl_1_bss_34;
s32 lbl_1_bss_38;
M532Rand lbl_1_bss_3C;
u8 lbl_1_bss_A00;
M532Master *lbl_1_bss_A04;

// ---------------------------------------------------------------- code

void fn_1_1D1C(const char *expr, const char *file, s32 line);
int fn_1_1D7C(void *unused, const char *fmt, ...);
void *fn_1_80BC(M532Node *self, s16 flag);
void fn_1_1F50(M532Cam *self, Vec *x, Vec *y, Vec *z);
void fn_1_20B4(M532Cam *self, Vec *delta);

void *fn_1_1B48(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M532_ASSERT(p != 0, lbl_1_data_C8, lbl_1_data_D4, 0x17);
    return p;
}

void *fn_1_1BAC(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M532_ASSERT(p != 0, lbl_1_data_C8, lbl_1_data_D4, 0x17);
    return p;
}

void fn_1_1C10(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_1C48(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

void _prolog(void)
{
    void (**ctor)(void) = _ctors;

    while (*ctor != 0) {
        (*ctor)();
        ctor++;
    }
    ObjectSetup();
}

void _epilog(void)
{
    void (**dtor)(void) = _dtors;

    while (*dtor != 0) {
        (*dtor)();
        dtor++;
    }
}

void fn_1_1D1C(const char *expr, const char *file, s32 line)
{
    char buf[0x40C];

    sprintf(buf, lbl_1_data_DC, file, line, expr);
    OSPanic(lbl_1_data_FA, 0x12, buf);
}

int fn_1_1D7C(void *unused, const char *fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = vprintf(fmt, ap);
    va_end(ap);
    return ret;
}

// Orbit: rotate about the camera X axis (pitch) then world Y (yaw).
void fn_1_1E20(M532Cam *self, f32 pitch, f32 yaw)
{
    Vec ax, ay, az;
    Mtx rotA, rotB, rot;

    fn_1_1F50(self, &ax, &ay, &az);
    PSMTXRotAxisRad(rotA, &ax, 0.017453292f * pitch);
    PSMTXRotRad(rotB, 'Y', 0.017453292f * yaw);
    PSMTXConcat(rotA, rotB, rot);
    PSMTXMultVecSR(rot, &az, &az);
    PSMTXMultVecSR(rot, &ay, &self->up);
    PSVECSubtract(&self->pos, &az, &self->target);
}

void fn_1_1EE4(M532Cam *self, f32 angle)
{
    Mtx m;
    Vec axis;

    PSVECSubtract(&self->target, &self->pos, &axis);
    PSMTXRotAxisRad(m, &axis, 0.017453292f * angle);
    PSMTXMultVec(m, &self->up, &self->up);
}

// Camera basis: the three rows of the look-at matrix.
void fn_1_1F50(M532Cam *self, Vec *x, Vec *y, Vec *z)
{
    Mtx m;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    x->x = m[0][0];
    x->y = m[0][1];
    x->z = m[0][2];
    y->x = m[1][0];
    y->y = m[1][1];
    y->z = m[1][2];
    z->x = m[2][0];
    z->y = m[2][1];
    z->z = m[2][2];
}

void fn_1_1FF8(M532Cam *self, Mtx out)
{
    C_MTXLookAt(out, &self->pos, &self->up, &self->target);
    PSMTXInverse(out, out);
}

void fn_1_204C(M532Cam *self, Vec *local)
{
    Mtx m;
    Vec world;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    PSMTXMultVecSR(m, local, &world);
    fn_1_20B4(self, &world);
}

void fn_1_20B4(M532Cam *self, Vec *delta)
{
    PSVECAdd(delta, &self->target, &self->target);
    PSVECAdd(delta, &self->pos, &self->pos);
}

void fn_1_2108(M532Cam *self)
{
    self->up.x = 0.0f;
    self->up.y = 1.0f;
    self->up.z = 0.0f;
    self->pos.x = 0.0f;
    self->pos.y = 0.0f;
    self->pos.z = 5000.0f;
    self->target.x = 0.0f;
    self->target.y = 0.0f;
    self->target.z = 0.0f;
}

void fn_1_219C(M532Cam *self, M532Pose *pose)
{
    Mtx m;

    PSMTXQuat(m, &pose->rot);
    self->up.x = m[0][1];
    self->up.y = m[1][1];
    self->up.z = m[2][1];
    self->pos.x = pose->pos.x;
    self->pos.y = pose->pos.y;
    self->pos.z = pose->pos.z;
    self->target.x = pose->pos.x - m[0][2];
    self->target.y = pose->pos.y - m[1][2];
    self->target.z = pose->pos.z - m[2][2];
}

void fn_1_223C(M532Cam *self, M532Pose *out)
{
    Mtx m;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    out->pos.x = self->pos.x;
    out->pos.y = self->pos.y;
    out->pos.z = self->pos.z;
    C_QUATMtx(&out->rot, m);
}

void fn_1_22B4(M532Cam *self, M532Pose *a, M532Pose *b, f32 t)
{
    Mtx m;
    Quaternion q;
    Vec p, pa, pb;

    C_QUATSlerp(&a->rot, &b->rot, &q, t);
    PSVECScale(&a->pos, &pa, 1.0f - t);
    PSVECScale(&b->pos, &pb, t);
    PSVECAdd(&pa, &pb, &p);
    PSMTXQuat(m, &q);
    self->up.x = m[0][1];
    self->up.y = m[1][1];
    self->up.z = m[2][1];
    self->pos.x = p.x;
    self->pos.y = p.y;
    self->pos.z = p.z;
    self->target.x = p.x - m[0][2];
    self->target.y = p.y - m[1][2];
    self->target.z = p.z - m[2][2];
}

void fn_1_23C4(M532Cam *self, M532Cam *src)
{
    Vec delta;

    PSVECSubtract(&src->pos, &self->pos, &delta);
    self->pos.x = src->pos.x;
    self->pos.y = src->pos.y;
    self->pos.z = src->pos.z;
    PSVECAdd(&self->target, &delta, &self->target);
}

void fn_1_2430(M532Cam *self, Vec *a, Vec *b, f32 t)
{
    Vec pa, pb, delta;

    PSVECScale(a, &pa, 1.0f - t);
    PSVECScale(b, &pb, t);
    PSVECAdd(&pa, &pb, &pb);
    PSVECSubtract(&pb, &self->pos, &delta);
    self->pos.x = pb.x;
    self->pos.y = pb.y;
    self->pos.z = pb.z;
    PSVECAdd(&self->target, &delta, &self->target);
}

void fn_1_24E8(M532Cam *self, u16 camBit)
{
    Hu3DCameraPosSetV(camBit, &self->pos, &self->up, &self->target);
}

void fn_1_2528(M532Cam *self, u16 camBit)
{
    Hu3DCameraPosGet(camBit, &self->pos, &self->up, &self->target);
}

// Dump the current camera as a pose table row (floats printed as raw words).
void fn_1_2568(M532Cam *self)
{
    Mtx m;
    M532PoseBits out;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    out.pose.pos.x = self->pos.x;
    out.pose.pos.y = self->pos.y;
    out.pose.pos.z = self->pos.z;
    C_QUATMtx(&out.pose.rot, m);
    printf(lbl_1_data_105, out.w[0], out.w[1], out.w[2], out.w[3], out.w[4], out.w[5], out.w[6]);
}

f32 fn_1_2600(s32 pad)
{
    return 0.017857144f * HuPadStkX[pad];
}

f32 fn_1_2654(s32 pad)
{
    return 0.017857144f * HuPadStkY[pad];
}

f32 fn_1_26A8(s32 pad)
{
    return 0.016949153f * HuPadSubStkX[pad];
}

f32 fn_1_26FC(s32 pad)
{
    return 0.016949153f * HuPadSubStkY[pad];
}

// Free-fly debug camera driven by pad `pad`.
void fn_1_2750(M532Cam *self, s32 pad)
{
    Mtx inv, rot, rotB, rotA, m, dump;
    M532PoseBits out;
    Vec mv, fwd, z, y, x;
    f32 yaw, pitch, speed, len;
    u32 btn;
    u8 fast;

    btn = HuPadBtn[pad];
    if (btn & 0x800) {
        self->up.x = 0.0f;
        self->up.y = 1.0f;
        self->up.z = 0.0f;
    }
    yaw = -4.0f * fn_1_2600(pad);
    pitch = -4.0f * fn_1_2654(pad);
    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    x.x = m[0][0];
    x.y = m[0][1];
    x.z = m[0][2];
    y.x = m[1][0];
    y.y = m[1][1];
    y.z = m[1][2];
    z.x = m[2][0];
    z.y = m[2][1];
    z.z = m[2][2];
    PSMTXRotAxisRad(rotA, &x, 0.017453292f * pitch);
    PSMTXRotRad(rotB, 'Y', 0.017453292f * yaw);
    PSMTXConcat(rotA, rotB, rot);
    PSMTXMultVecSR(rot, &z, &z);
    PSMTXMultVecSR(rot, &y, &self->up);
    PSVECSubtract(&self->pos, &z, &self->target);
    fast = (btn & 0x200) != 0;
    speed = fast ? 70.0f : 20.0f;
    C_MTXLookAt(inv, &self->pos, &self->up, &self->target);
    PSMTXInverse(inv, inv);
    mv.x = (speed * (u8)((btn & 2) != 0)) - (speed * (u8)((btn & 1) != 0));
    mv.y = (speed * (u8)((btn & 8) != 0)) - (speed * (u8)((btn & 4) != 0));
    mv.x = mv.x + (speed * fn_1_26A8(pad));
    mv.z = 0.0f;
    PSMTXMultVecSR(inv, &mv, &mv);
    mv.y = mv.y + (speed * fn_1_26FC(pad));
    PSVECAdd(&mv, &self->target, &self->target);
    PSVECAdd(&mv, &self->pos, &self->pos);
    fwd.x = 0.0f;
    fwd.y = 0.0f;
    fwd.z = (f32)((u8)((btn & 0x400) != 0) - (u8)((btn & 0x100) != 0));
    PSMTXMultVecSR(inv, &fwd, &fwd);
    fwd.y = 0.0f;
    len = PSVECMag(&fwd);
    if (len > 1e-10f) {
        PSVECScale(&fwd, &fwd, speed / len);
        PSVECAdd(&fwd, &self->target, &self->target);
        PSVECAdd(&fwd, &self->pos, &self->pos);
    }
    if ((HuPadBtnDown[pad] & 0x200) != 0) {
        C_MTXLookAt(dump, &self->pos, &self->up, &self->target);
        PSMTXInverse(dump, dump);
        out.pose.pos.x = self->pos.x;
        out.pose.pos.y = self->pos.y;
        out.pose.pos.z = self->pos.z;
        C_QUATMtx(&out.pose.rot, dump);
        printf(lbl_1_data_105, out.w[0], out.w[1], out.w[2], out.w[3], out.w[4], out.w[5], out.w[6]);
    }
}

// ---------------------------------------------------------------- intrusive list

typedef u8 (*M532Cmp)(M532Node *a, M532Node *b);

M532Node *fn_1_3B94(M532Node *list);
M532Node *fn_1_3BE8(M532Cmp cmp, M532Node *a, M532Node *b);
M532Node *fn_1_34EC(M532Cmp cmp, M532Node *list);
void fn_1_4330(OMOBJ *obj);
void fn_1_4430(void);
void fn_1_4BD8(void);
void fn_1_67A4(void *ptr);

// ---------------------------------------------------------------- handle manager

// 1-based table of generation-tagged handles: a free ring and a used ring
// share one node array (index 0 is the free-ring head, index n the used head).
M532HandleMan *fn_1_2DAC(M532HandleMan *self, u32 n)
{
    u32 i;
    M532HandleNode *nodes;
    void *mem;

    M532_ASSERT_INL(n <= 0x1000, lbl_1_data_149, lbl_1_data_13E, 0x44);
    self->count = n;
    mem = HuMemDirectMallocNum(HEAP_HEAP, (n + 1) * sizeof(M532HandleNode), 0x10000000);
    M532_ASSERT_INL(mem != 0, lbl_1_data_C8, lbl_1_data_D4, 0x17);
    nodes = (M532HandleNode *)mem;
    self->nodes = nodes;
    self->used = &self->nodes[n];
    self->used->next = self->used;
    self->used->prev = self->used;
    for (i = 1; i < n; i++) {
        self->nodes[i].next = &self->nodes[i + 1];
        self->nodes[i].prev = &self->nodes[i - 1];
        self->nodes[i].ptr = 0;
        self->nodes[i].word = i << 12;
    }
    self->nodes[1].prev = &self->nodes[0];
    self->nodes[n - 1].next = &self->nodes[0];
    self->nodes[0].next = &self->nodes[1];
    self->nodes[0].prev = &self->nodes[n - 1];
    return self;
}

M532HandleMan *fn_1_2FB0(M532HandleMan *self, s16 flag)
{
    if (self != 0) {
        if (self->nodes != 0) {
            HuMemDirectFree(self->nodes);
        }
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

s32 fn_1_3020(M532HandleMan *self, void *ptr)
{
    M532HandleNode *node = self->nodes[0].next;

    if (node == &self->nodes[0]) {
        return 0;
    }
    node->next->prev = node->prev;
    node->prev->next = node->next;
    node->next = self->used->next;
    node->prev = self->used;
    self->used->next->prev = node;
    self->used->next = node;
    node->ptr = ptr;
    node->word = (node->word & ~0xFFF) | ((M532_HANDLE_GEN(node->word) + 1) & 0xFFF);
    return node->word;
}

void *fn_1_30B0(M532HandleMan *self, s32 handle)
{
    u32 h = handle;
    M532HandleNode *node;
    void *ptr;

    if (h == 0) {
        return 0;
    }
    if (M532_HANDLE_INDEX(h) >= self->count) {
        return 0;
    }
    node = &self->nodes[M532_HANDLE_INDEX(h)];
    if (node->word != h) {
        return 0;
    }
    ptr = node->ptr;
    node->ptr = 0;
    node->next->prev = node->prev;
    node->prev->next = node->next;
    node->next = self->nodes;
    node->prev = self->nodes->prev;
    self->nodes->prev->next = node;
    self->nodes->prev = node;
    return ptr;
}

void *fn_1_3174(M532HandleMan *self, s32 handle)
{
    u32 h = handle;
    M532HandleNode *node;

    if (h == 0) {
        return 0;
    }
    if (M532_HANDLE_INDEX(h) >= self->count) {
        return 0;
    }
    node = &self->nodes[M532_HANDLE_INDEX(h)];
    if (node->word != h) {
        return 0;
    }
    return node->ptr;
}

// The original walks from the manager itself as if it were a node (unused).
s32 fn_1_31E4(M532HandleNode *self)
{
    s32 n = 0;
    M532HandleNode *node = self->next;

    while (node != self) {
        node = node->next;
        n++;
    }
    return n;
}

s32 fn_1_3220(M532HandleMan *self)
{
    M532HandleNode *head = self->used;
    s32 n = 0;
    M532HandleNode *node = head->next;

    while (node != head) {
        node = node->next;
        n++;
    }
    return n;
}

s32 fn_1_3268(M532HandleMan *self)
{
    M532HandleNode *head = self->nodes;
    s32 n = 0;
    M532HandleNode *node = head->next;

    while (node != head) {
        node = node->next;
        n++;
    }
    return n;
}

void fn_1_32B0(M532Node *self)
{
    self->next = self;
    self->prev = self;
}

void *fn_1_32BC(M532Node *self, s16 flag)
{
    if (self != 0) {
        self->next->prev = self->prev;
        self->prev->next = self->next;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_3328(M532Node *self)
{
    self->next->prev = self->prev;
    self->prev->next = self->next;
    self->next = self;
    self->prev = self;
}

// Insert `self` right after `other`.
void fn_1_334C(M532Node *self, M532Node *other)
{
    M532Node *at = other;

    self->prev = at;
    self->next = at->next;
    at->next->prev = self;
    at->next = self;
}

// Insert `self` right before `other`.
void fn_1_337C(M532Node *self, M532Node *other)
{
    M532Node *at = other;

    self->next = at;
    self->prev = at->prev;
    at->prev->next = self;
    at->prev = self;
}

// Move every node of `self` in front of `other`.
void fn_1_33AC(M532Node *self, M532Node *other)
{
    M532Node *first;
    M532Node *last;
    u8 empty;

    empty = 0;
    if (self->next == self && self->next == self->prev) {
        empty = 1;
    }
    if (empty == 0) {
        first = self->next;
        last = self->prev;
        self->next = self;
        self->prev = self;
        first->prev = other->prev;
        last->next = other;
        other->prev->next = first;
        other->prev = last;
    }
}

// Merge sort the circular list `self` with `cmp`, then relink the prev chain.
void fn_1_3434(M532Node *self, M532Cmp cmp)
{
    M532Node *node;
    M532Node *prev;
    u8 empty;

    empty = 0;
    if (self->next == self && self->next == self->prev) {
        empty = 1;
    }
    if (empty == 0) {
        self->prev->next = 0;
        node = fn_1_34EC(cmp, self->next);
        self->next = node;
        prev = self;
        while (node != 0) {
            node->prev = prev;
            prev = node;
            node = node->next;
        }
        self->prev = prev;
        prev->next = self;
    }
}

M532Node *fn_1_34EC(M532Cmp cmp, M532Node *list)
{
    M532Node *second;
    M532Node *a;
    M532Node *b;

    if (list == 0) {
        return 0;
    }
    if (list->next == 0) {
        return list;
    }
    second = fn_1_3B94(list);
    a = fn_1_34EC(cmp, list);
    b = fn_1_34EC(cmp, second);
    return fn_1_3BE8(cmp, a, b);
}

// Cut the list in the middle and return the second half.
M532Node *fn_1_3B94(M532Node *list)
{
    M532Node *fast;
    M532Node *second;

    fast = list->next;
    if (fast != 0) {
        fast = fast->next;
    }
    while (fast != 0) {
        list = list->next;
        fast = fast->next;
        if (fast != 0) {
            fast = fast->next;
        }
    }
    second = list->next;
    list->next = 0;
    return second;
}

M532Node *fn_1_3BE8(M532Cmp cmp, M532Node *a, M532Node *b)
{
    M532Node head;
    M532Node *tail;

    tail = &head;
    while (a != 0 && b != 0) {
        if (cmp(a, b) != 0) {
            tail->next = a;
            tail = a;
            a = a->next;
        } else {
            tail->next = b;
            tail = b;
            b = b->next;
        }
    }
    if (a == 0) {
        tail->next = b;
    } else {
        tail->next = a;
    }
    return head.next;
}

// ---------------------------------------------------------------- task list

void fn_1_3C94(s32 bit)
{
    M532Task *t;
    M532Node *end;
    M532Node *b;
    M532Node *c;
    M532Node *nx;

    b = lbl_1_bss_24.next;
    c = b;
    t = (M532Task *)c;
    lbl_1_bss_2C = bit;
    end = &lbl_1_bss_24;
    while ((M532Node *)t != end) {
        t->flags |= 1 << bit;
        t->vtbl->onFlag(t, bit);
        nx = t->link.next;
        t = (M532Task *)nx;
    }
}

s32 fn_1_3D2C(void)
{
    M532Task *t;
    M532Node *end;
    M532Node *b;
    M532Node *c;
    M532Node *nx;
    s32 bit;

    b = lbl_1_bss_24.next;
    c = b;
    t = (M532Task *)c;
    end = &lbl_1_bss_24;
    while ((M532Node *)t != end) {
        bit = lbl_1_bss_2C;
        if ((t->flags & (1 << bit)) != 0) {
            return 0;
        }
        nx = t->link.next;
        t = (M532Task *)nx;
    }
    return 1;
}

void fn_1_3DB8(void)
{
    M532Task *t;
    M532Node *end;
    M532Node *b;
    M532Node *c;
    M532Node *nx;

    b = lbl_1_bss_24.next;
    c = b;
    t = (M532Task *)c;
    end = &lbl_1_bss_24;
    while ((M532Node *)t != end) {
        t->vtbl->update(t);
        nx = t->link.next;
        t = (M532Task *)nx;
    }
}

// Destroy every registered task, last one first.
void fn_1_3E28(void)
{
    M532Task *t;
    u8 empty;

    while (1) {
        empty = 0;
        if (lbl_1_bss_24.next == &lbl_1_bss_24 && lbl_1_bss_24.next == lbl_1_bss_24.prev) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        t = (M532Task *)lbl_1_bss_24.prev;
        if (t != 0) {
            t->vtbl->dtor(t, 1);
        }
    }
}

s32 fn_1_3EF0(void)
{
    return lbl_1_bss_2C;
}

// ---------------------------------------------------------------- game master

M532Master *fn_1_3F00(M532Master *self)
{
    s32 i;

    M532_ASSERT_INL(lbl_1_bss_A04 == 0, lbl_1_data_163, lbl_1_data_157, 0x35);
    lbl_1_bss_A04 = self;
    self->vtbl = &lbl_1_data_2C8;
    self->counter = 0;
    i = 0;
    while (i < 4) {
        self->slot[i] = -1;
        i++;
    }
    return self;
}

M532Master *fn_1_3FD0(M532Master *self, s16 flag)
{
    if (self != 0) {
        M532_ASSERT_INL(lbl_1_bss_A04 != 0, lbl_1_data_173, lbl_1_data_157, 0x3D);
        lbl_1_bss_A04 = 0;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

M532Master *fn_1_408C(M532Master *self, s16 flag)
{
    M532Master *m;
    s32 i;
    s32 p;

    if (self != 0) {
        self->vtbl = &lbl_1_data_2C8;
        if (mgDataTbl[GwSystem.mgNo].type != 3) {
            M532_ASSERT_INL(lbl_1_bss_A04 != 0, lbl_1_data_173, lbl_1_data_157, 0x27);
            m = lbl_1_bss_A04;
            i = 0;
            while (i < 4) {
                p = m->slot[i];
                if (p >= 0 && _CheckFlag(0x1000F) == 0) {
                    GwPlayer[p].mgCoinBonus = 10;
                }
                i++;
            }
        }
        if (self != 0) {
            M532_ASSERT_INL(lbl_1_bss_A04 != 0, lbl_1_data_173, lbl_1_data_157, 0x3D);
            lbl_1_bss_A04 = 0;
        }
        if (flag > 0) {
            fn_1_67A4(self);
        }
    }
    return self;
}

void fn_1_4244(M532Master *self, s32 objMax, s32 prio)
{
    OMOBJMAN *om;

    om = omInitObjMan((s16)objMax, prio);
    omGameSysInit(om);
    self->vtbl->setup(self, om);
    omAddObjEx(om, 0, 0, 0, -1, fn_1_4330);
}

void fn_1_42C8(M532Master *self)
{
    OMOBJMAN *om;

    om = MgActorObjectSetup();
    self->vtbl->setup(self, om);
    omAddObjEx(om, 0, 0, 0, -1, fn_1_4330);
}

void fn_1_43C0(M532Master *self);

void fn_1_4330(OMOBJ *obj)
{
    OMOBJ *self;
    M532Master *m;
    M532Master *cur;

    self = obj;
    if (omSysExitReq != 0 && WipeCheckEnd() == 0) {
        fn_1_4BD8();
        HuAudFadeOut(0x3E8);
        self->objFunc = (OMOBJFUNC)fn_1_4430;
        return;
    }
    m = lbl_1_bss_A04;
    cur = m;
    fn_1_43C0(cur);
}

void fn_1_43C0(M532Master *self)
{
    self->vtbl->update(self);
    if (self->counter > 0) {
        if (--self->counter == 0) {
        }
    }
}

void fn_1_442C(M532Master *self)
{
}

void fn_1_4430(void)
{
    M532Master *m;
    M532Master *cur;
    M532Master *obj;
    s32 c;
    u8 hitZero;

    m = lbl_1_bss_A04;
    cur = m;
    if (WipeCheckEnd() != 0) {
        cur->vtbl->update(cur);
        hitZero = 0;
        if (cur->counter > 0) {
            c = cur->counter - 1;
            cur->counter = c;
            if (c == 0) {
                hitZero = 1;
            }
        }
        return;
    }
    cur->vtbl->finish(cur);
    obj = cur;
    if (obj != 0) {
        obj = obj->vtbl->dtor(obj, 1);
    }
    CharModelKill(-1);
    omOvlReturnEx(1, 1);
}

// Register a player in the first free slot.
void fn_1_451C(s32 player)
{
    s32 *slots;
    s32 i;

    M532_ASSERT_INL(lbl_1_bss_A04 != 0, lbl_1_data_173, lbl_1_data_157, 0x27);
    slots = lbl_1_bss_A04->slot;
    i = 0;
    while (i < 4) {
        if (slots[i] < 0) {
            slots[i] = player;
            return;
        }
        i++;
    }
}

s32 fn_1_45F0(s32 player)
{
    s32 i;

    M532_ASSERT_INL(lbl_1_bss_A04 != 0, lbl_1_data_173, lbl_1_data_157, 0x27);
    i = 0;
    while (i < 4) {
        if (player == lbl_1_bss_A04->slot[i]) {
            return 1;
        }
        i++;
    }
    return 0;
}

void fn_1_46C4(void)
{
    M532Master *m;

    M532_ASSERT_INL(lbl_1_bss_A04 != 0, lbl_1_data_173, lbl_1_data_157, 0x27);
    m = lbl_1_bss_A04;
    m->mesId = GameMesCreate(2, 0);
}

void fn_1_477C(void)
{
    M532Master *m;

    M532_ASSERT_INL(lbl_1_bss_A04 != 0, lbl_1_data_173, lbl_1_data_157, 0x27);
    m = lbl_1_bss_A04;
    m->mesId = GameMesCreate(2, 1);
}

s32 fn_1_4834(void)
{
    M532Master *m;
    s32 ret;

    M532_ASSERT_INL(lbl_1_bss_A04 != 0, lbl_1_data_173, lbl_1_data_157, 0x27);
    m = lbl_1_bss_A04;
    ret = 0;
    if (GameMesStatGet(-1) == 0 && (u8)(m->counter <= 0) != 0) {
        ret = 1;
    }
    return ret;
}

// Announce the result: sort the registered players and print their characters.
void fn_1_4908(void)
{
    M532Master *m;
    s32 *slots;
    s32 chars[4];
    s32 n;
    s32 i;
    s32 j;
    s32 k;
    s32 tmp;
    s16 charNo;

    M532_ASSERT_INL(lbl_1_bss_A04 != 0, lbl_1_data_173, lbl_1_data_157, 0x27);
    m = lbl_1_bss_A04;
    m->counter = lbl_1_bss_34;
    slots = m->slot;
    n = 0;
    while (n < 4) {
        if (slots[n] >= 0) {
            n++;
            continue;
        }
        break;
    }
    if (n == 0) {
        GameMesCreate(2, 2);
        return;
    }
    i = 1;
    while (i < n) {
        j = i;
        while (j >= 1 && slots[j - 1] > slots[j]) {
            tmp = slots[j - 1];
            slots[j - 1] = slots[j];
            slots[j] = tmp;
            j--;
        }
        i++;
    }
    k = 0;
    while (k < 4) {
        if (slots[k] < 0) {
            charNo = -1;
        } else {
            charNo = GwPlayerConf[slots[k]].charNo;
        }
        chars[k] = charNo;
        k++;
    }
    GameMesCreate(4, 3, chars[0], chars[1], chars[2], chars[3]);
}

void fn_1_4AD4(s32 mes)
{
    M532Master *m;

    if (_CheckFlag(0x1000F) == 0) {
        M532_ASSERT_INL(lbl_1_bss_A04 != 0, lbl_1_data_173, lbl_1_data_157, 0x27);
        m = lbl_1_bss_A04;
        m->mesId = GameMesCreate(0xE, mes);
        HuAudJinglePlay(0x86);
    }
}

void fn_1_4BAC(void)
{
    WipeCreate(1, 5, 0x3C);
}

void fn_1_4BD8(void)
{
    WipeCreate(2, 6, 0x3C);
}

void fn_1_4C04(void)
{
    omSysExitReq = 1;
}

// ---------------------------------------------------------------- Hu3D model wrappers

M532Model *fn_1_4C18(M532Model *self, void *data)
{
    self->id = Hu3DModelCreate(data);
    return self;
}

M532Model *fn_1_4C54(M532Model *self, s32 dataNum)
{
    self->id = Hu3DModelCreateData(dataNum);
    return self;
}

M532Model *fn_1_4C9C(M532Model *self, HU3DMODELHOOK hook)
{
    self->id = Hu3DHookFuncCreate(hook);
    return self;
}

// Build a character model; `type` selects the model variant bit.
M532Model *fn_1_4CD8(M532Model *self, s32 charNo, s32 type)
{
    s16 variant;

    variant = 0;
    switch (type) {
        case 0:
            variant = 1;
            break;
        case 1:
            variant = 2;
            break;
        case 2:
            variant = 4;
            break;
        case 3:
            variant = 8;
            break;
    }
    self->id = CharModelCreate((s16)charNo, variant);
    return self;
}

inline s32 M532ModelIdGet(M532Model *m)
{
    return m->id;
}

M532Model *fn_1_4D74(M532Model *self, M532Model *parent)
{
    self->id = Hu3DModelLink(M532ModelIdGet(parent));
    return self;
}

void fn_1_4DC8(M532Model *self, u32 attr)
{
    Hu3DModelAttrSet(self->id, attr);
}

void fn_1_4E00(M532Model *self, u32 attr)
{
    Hu3DModelAttrReset(self->id, attr);
}

// Apply an object's transform (translation, rotation, scale) to the model.
void fn_1_4E38(M532Model *self, OMOBJ *obj)
{
    HU3DMODELID id;

    id = self->id;
    Hu3DModelPosSet(id, obj->trans.x, obj->trans.y, obj->trans.z);
    Hu3DModelRotSet(id, obj->rot.x, obj->rot.y, obj->rot.z);
    Hu3DModelScaleSet(id, obj->scale.x, obj->scale.y, obj->scale.z);
}

void fn_1_4EB4(M532Model *self, Vec *pos)
{
    Hu3DModelPosSet(self->id, pos->x, pos->y, pos->z);
}

void fn_1_4EFC(M532Model *self, Vec *rot)
{
    Hu3DModelRotSet(self->id, rot->x, rot->y, rot->z);
}

void fn_1_4F44(M532Model *self, Vec *scale)
{
    Hu3DModelScaleSet(self->id, scale->x, scale->y, scale->z);
}

void fn_1_4F8C(M532Model *self, f32 scale)
{
    Hu3DModelScaleSet(self->id, scale, scale, scale);
}

void fn_1_4FD4(M532Model *self, f32 tpLvl)
{
    Hu3DModelTPLvlSet(self->id, tpLvl);
}

void fn_1_500C(M532Model *self, s16 layer)
{
    Hu3DModelLayerSet(self->id, layer);
}

void fn_1_5044(M532Model *self, u16 camBit)
{
    Hu3DModelCameraSet(self->id, camBit);
}

void fn_1_507C(M532Model *self, void *data)
{
    Hu3DData[self->id].hookData = data;
}

MtxPtr fn_1_5098(M532Model *self)
{
    return Hu3DData[self->id].mtx;
}

M532Model *fn_1_50B4(M532Model *self, s16 flag)
{
    if (self != 0) {
        Hu3DModelKill(self->id);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

inline HSFOBJECT *M532ObjGet(M532Model *self, char *name)
{
    HSFOBJECT *obj;

    obj = Hu3DModelObjPtrGet(self->id, name);
    M532_ASSERT_INL(obj != 0, lbl_1_data_18D, lbl_1_data_183, 0x8A);
    return obj;
}

HSFOBJECT *fn_1_5110(M532Model *self, char *name)
{
    return M532ObjGet(self, name);
}

void fn_1_51A4(M532Model *self)
{
    Hu3DModelShadowSet(self->id);
}

void fn_1_51D4(M532Model *self)
{
    Hu3DModelShadowMapSet(self->id);
}

void fn_1_5204(M532Model *self, char *name)
{
    Hu3DModelShadowMapObjSet(self->id, name);
}

void fn_1_523C(M532Model *self)
{
    Hu3DModelShadowMapObjReset(self->id);
}

void fn_1_526C(M532Model *self, char *name)
{
    HSFOBJECT *obj;
    u32 *flags;

    obj = M532ObjGet(self, name);
    flags = (u32 *)obj->constData;
    *flags &= ~8;
}

void fn_1_5318(M532Model *self, char *name)
{
    HSFOBJECT *obj;

    obj = M532ObjGet(self, name);
    obj->flags |= 0x400;
}

void fn_1_53C0(M532Model *self, char *name)
{
    HSFOBJECT *obj;

    obj = M532ObjGet(self, name);
    obj->flags &= ~0x400;
}

void fn_1_5468(M532Model *self, char *name)
{
    HSFOBJECT *obj;
    HSFTRANSFORM *base;

    obj = M532ObjGet(self, name);
    obj->flags |= 0x400;
    base = &obj->mesh.base;
    base->scale.x = 0.0f;
    base->scale.y = 0.0f;
    base->scale.z = 0.0f;
}

void fn_1_5544(M532Model *self, char *name)
{
    HSFOBJECT *obj;
    HSFTRANSFORM *base;

    obj = M532ObjGet(self, name);
    obj->flags &= ~0x400;
    base = &obj->mesh.base;
    base->scale.x = 1.0f;
    base->scale.y = 1.0f;
    base->scale.z = 1.0f;
}

void fn_1_5620(M532Model *self, s32 time)
{
    Hu3DMotionTimeSet(M532ModelIdGet(self), (f32)time);
}

// ---------------------------------------------------------------- Mersenne Twister

extern const u32 lbl_1_rodata_120[2] = { 0, 0x9908B0DF };

void fn_1_568C(M532Rand *r, u32 seed)
{
    r->mt[0] = seed;
    r->idx = 1;
    while (r->idx < 0x270) {
        r->mt[r->idx] = r->idx + (((r->mt[r->idx - 1]) ^ ((r->mt[r->idx - 1]) >> 30)) * 0x6C078965);
        r->idx++;
    }
}

void fn_1_5700(M532Rand *r, u32 *key, u32 len)
{
    u32 i;
    u32 j;
    u32 k;

    fn_1_568C(r, 19650218);
    i = 1;
    j = 0;
    k = (len < 0x270) ? 0x270 : len;
    while (k != 0) {
        r->mt[i] = key[j] + ((r->mt[i] ^ ((r->mt[i - 1] ^ (r->mt[i - 1] >> 30)) * 0x19660D)) + j);
        i++;
        j++;
        if (i >= 0x270) {
            r->mt[0] = r->mt[0x26F];
            i = 1;
        }
        if (j >= len) {
            j = 0;
        }
        k--;
    }
    k = 0x26F;
    while (k != 0) {
        r->mt[i] = (r->mt[i] ^ ((r->mt[i - 1] ^ (r->mt[i - 1] >> 30)) * 0x5D588B65)) - i;
        i++;
        if (i >= 0x270) {
            r->mt[0] = r->mt[0x26F];
            i = 1;
        }
        k--;
    }
    r->mt[0] = 0x80000000;
}

u32 fn_1_58C4(M532Rand *r)
{
    u32 k;
    u32 y;

    if (r->idx >= 0x270) {
        k = 0;
        while (k < 0xE3) {
            y = (r->mt[k] & 0x80000000) | (r->mt[k + 1] & 0x7FFFFFFF);
            r->mt[k] = lbl_1_rodata_120[y & 1] ^ ((y >> 1) ^ r->mt[k + 0x18D]);
            k++;
        }
        while (k < 0x26F) {
            y = (r->mt[k] & 0x80000000) | (r->mt[k + 1] & 0x7FFFFFFF);
            r->mt[k] = lbl_1_rodata_120[y & 1] ^ ((y >> 1) ^ r->mt[k - 0xE3]);
            k++;
        }
        y = (r->mt[0x26F] & 0x80000000) | (r->mt[0] & 0x7FFFFFFF);
        r->mt[0x26F] = (y >> 1) ^ r->mt[0x18C] ^ lbl_1_rodata_120[y & 1];
        r->idx = 0;
    }
    y = r->mt[r->idx++];
    y ^= y >> 11;
    y ^= (y << 7) & 0x9D2C5680;
    y ^= (y << 15) & 0xEFC60000;
    return y ^ (y >> 18);
}

u32 fn_1_5A50(M532Rand *r)
{
    return fn_1_58C4(r) >> 1;
}

f32 fn_1_5BE8(M532Rand *r)
{
    return 2.3283064e-10f * (f32)fn_1_58C4(r);
}

f32 fn_1_5DAC(M532Rand *r)
{
    return 2.3283064e-10f * (f32)fn_1_58C4(r);
}

f32 fn_1_5F70(M532Rand *r)
{
    return 2.3283064e-10f * (0.5f + (f32)fn_1_58C4(r));
}

f64 fn_1_6144(M532Rand *r)
{
    u32 a;
    u32 b;

    a = fn_1_58C4(r) >> 5;
    b = fn_1_58C4(r) >> 6;
    return 1.1102230246251565e-16 * ((67108864.0 * (f64)a) + (f64)b);
}

f32 fn_1_64BC(M532Rand *r)
{
    return 1.1920929e-7f * (f32)(fn_1_58C4(r) >> 9);
}

// ---------------------------------------------------------------- allocation / functor objects

void *fn_1_668C(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M532_ASSERT_INL(p != 0, lbl_1_data_C8, lbl_1_data_197, 0x18);
    return p;
}

void *fn_1_6718(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M532_ASSERT_INL(p != 0, lbl_1_data_C8, lbl_1_data_197, 0x1F);
    return p;
}

void fn_1_67A4(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_67DC(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

// Functor owned by an object: the object's data slot points at it.
void fn_1_6814(M532Functor *self, OMOBJ *obj)
{
    self->vtbl = &lbl_1_data_2B8;
    obj->data = self;
    obj->objFunc = fn_1_6834;
}

void fn_1_6834(OMOBJ *obj)
{
    M532Functor *self;

    self = (M532Functor *)obj->data;
    self->vtbl->run(self, obj);
}

void fn_1_6880(M532Functor *self, OMOBJ *obj)
{
}

void fn_1_6884(OMOBJ *obj)
{
    M532Functor *fn;
    M532Functor *p;

    fn = (M532Functor *)obj->data;
    p = fn;
    if (p != 0) {
        p = p->vtbl->dtor(p, 1);
    }
    omDelObjEx(HuPrcCurrentGet(), obj);
}

M532Functor *fn_1_68F4(M532Functor *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_2B8;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_6954(OMOBJMAN *man, OMOBJ *obj)
{
    M532Functor *fn;
    M532Functor *p;

    fn = (M532Functor *)obj->data;
    p = fn;
    if (p != 0) {
        p = p->vtbl->dtor(p, 1);
    }
    omDelObjEx(man, obj);
}

// ---------------------------------------------------------------- global random number generator

void fn_1_69C8(u32 seed)
{
    fn_1_568C(&lbl_1_bss_3C, seed);
}

u32 fn_1_6AA0(void)
{
    return fn_1_58C4(&lbl_1_bss_3C) >> 1;
}

f32 fn_1_6CDC(void)
{
    return 1.1920929e-7f * (f32)(fn_1_58C4(&lbl_1_bss_3C) >> 9);
}

f32 fn_1_6F50(f32 scale)
{
    return scale * fn_1_6144(&lbl_1_bss_3C);
}

f32 fn_1_6F88(f32 from, f32 to)
{
    return from + (to - from) * fn_1_6144(&lbl_1_bss_3C);
}

// ---------------------------------------------------------------- shadow / collision helpers

// Distance at which a 5 degree field of view spans `size`; `out` = target - dir * distance.
f32 fn_1_6FDC(Vec *out, Vec *dir, Vec *target, f32 size)
{
    f32 dist;
    Vec *o1;
    Vec *o2;
    Vec *o3;

    dist = size / tan(0.0872664600610733);
    PSVECScale(dir, out, dist);
    o1 = out;
    o2 = o1;
    o3 = o2;
    PSVECSubtract(target, o3, o3);
    return dist;
}

extern const Vec lbl_1_rodata_144[2] = { { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } };

// Create a (multi-camera) shadow projecting along `dir` onto `target`.
void fn_1_7084(s16 camBit, Vec *dir, Vec *target, f32 width, f32 depth)
{
    Vec dn;
    Vec pos;
    f32 halfWidth;
    f32 halfDepth;
    f32 dist;
    f32 nearZ;
    f32 farZ;
    Vec *posPtr;
    Vec *posRef;
    f32 dist1;
    f32 dist2;
    f32 dist3;
    s32 upIdx;
    s32 count;
    s32 i;
    const Vec *up;

    halfWidth = width * 0.5f;
    halfDepth = depth * 0.5f;
    PSVECNormalize(dir, &dn);
    if (__fabs(dn.y) < 0.5f) {
        upIdx = 0;
    } else {
        upIdx = 1;
    }
    up = &lbl_1_rodata_144[upIdx];
    dist = (f32)(halfWidth / tan(0.0872664600610733));
    PSVECScale(&dn, &pos, dist);
    PSVECSubtract(target, &pos, &pos);
    posPtr = &pos;
    posRef = posPtr;
    dist1 = dist;
    dist2 = dist1;
    dist3 = dist2;
    nearZ = dist3 - halfDepth;
    farZ = dist3 + halfDepth;
    count = 0;
    i = 0;
    while (i < 0x10) {
        if ((camBit & (1 << i)) != 0) {
            count++;
        }
        i++;
    }
    if (count < 2) {
        Hu3DShadowCreate(10.0f, nearZ, farZ);
        Hu3DShadowPosSet(posRef, (Vec *)up, target);
        Hu3DShadowSizeSet(0x1E0);
        Hu3DShadowTPLvlSet(0.4f);
        return;
    }
    Hu3DShadowMultiCreate(10.0f, nearZ, farZ, camBit);
    Hu3DShadowMultiPosSet(posRef, (Vec *)up, target, camBit);
    Hu3DShadowMultiSizeSet(0x1E0, camBit);
    Hu3DShadowMultiTPLvlSet(0.4f, camBit);
}

// ---------------------------------------------------------------- threads

s32 fn_1_7300(void)
{
    return OSGetThreadPriority(OSGetCurrentThread());
}

void *fn_1_75E8(void *arg);
M532Thread *fn_1_7644(M532Thread *th, void *(*func)(void *), void *arg, u32 size, s32 prio, u16 attr);

M532SubThread *fn_1_7324(M532SubThread *self, u32 stackSize)
{
    self->vtbl = &lbl_1_data_2A8;
    fn_1_7644(&self->th, fn_1_75E8, self, stackSize, OSGetThreadPriority(OSGetCurrentThread()), 1);
    OSInitSemaphore(&self->sem, 0);
    return self;
}

M532Thread *fn_1_78C8(M532Thread *th, s16 flag);

M532SubThread *fn_1_73A0(M532SubThread *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_2A8;
        fn_1_78C8(&self->th, -1);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

// CThread::IsTerminated(), inlined with its own `this`.
inline u8 M532ThreadDone(M532Thread *th)
{
    return OSIsThreadTerminated(&th->thread) != 0;
}

u8 fn_1_740C(M532SubThread *self)
{
    M532Thread *th = &self->th;

    return OSIsThreadTerminated(&th->thread) != 0;
}

u32 fn_1_79FC(M532Thread *th);

s32 fn_1_7454(M532SubThread *self, s32 arg)
{
    M532Thread *thIsDone;
    M532Thread *thResume;
    M532Sem *sem;

    thIsDone = &self->th;
    if (OSIsThreadTerminated(&thIsDone->thread) != 0) {
        return self->arg;
    }
    self->arg = arg;
    thResume = &self->th;
    OSResumeThread(&thResume->thread);
    sem = &self->sem;
    OSWaitSemaphore(sem);
    fn_1_79FC(&self->th);
    OSCheckActiveThreads();
    return self->arg;
}

s32 fn_1_74E4(M532SubThread *self, s32 arg)
{
    M532Thread *thCur;
    M532Thread *thDone;
    M532Sem *sem;
    M532Thread *thSuspend;

    thCur = &self->th;
    M532_ASSERT_INL(OSGetCurrentThread() == &thCur->thread, lbl_1_data_1AC, lbl_1_data_19E, 0x47);
    thDone = &self->th;
    M532_ASSERT_INL(OSIsThreadTerminated(&thDone->thread) == 0, lbl_1_data_1CB, lbl_1_data_19E, 0x4A);
    self->arg = arg;
    sem = &self->sem;
    OSSignalSemaphore(sem);
    thSuspend = &self->th;
    OSSuspendThread(&thSuspend->thread);
    return self->arg;
}

void *fn_1_75E8(void *arg)
{
    M532SubThread *self;
    M532Sem *sem;

    self = (M532SubThread *)arg;
    self->vtbl->run(self, self->arg);
    sem = &self->sem;
    OSSignalSemaphore(sem);
    return 0;
}

void fn_1_77A4(M532Thread *th, void *(*func)(void *), void *arg, u32 *stack, u32 words, s32 prio, u16 attr);

M532Thread *fn_1_7644(M532Thread *th, void *(*func)(void *), void *arg, u32 size, s32 prio, u16 attr)
{
    u32 words;
    u32 *stack;

    words = size >> 2;
    stack = (u32 *)HuMemDirectMallocNum(HEAP_HEAP, words * 4, 0x10000000);
    M532_ASSERT_INL(stack != 0, lbl_1_data_C8, lbl_1_data_D4, 0x17);
    th->stack = stack;
    fn_1_77A4(th, func, arg, stack, words, prio, attr);
    return th;
}

M532Thread *fn_1_7728(M532Thread *th, void *(*func)(void *), void *arg, u32 *stack, u32 size, s32 prio, u16 attr)
{
    u32 words;

    th->stack = 0;
    words = size >> 2;
    fn_1_77A4(th, func, arg, stack, words, prio, attr);
    return th;
}

void fn_1_7B10(u32 *p, u32 words);
s32 fn_1_7A9C(u32 *p, s32 mode);

void fn_1_77A4(M532Thread *th, void *(*func)(void *), void *arg, u32 *stack, u32 words, s32 prio, u16 attr)
{
    u8 ok;

    ok = 0;
    if (prio >= 0 && prio <= 0x1F) {
        ok = 1;
    }
    M532_ASSERT_INL(ok != 0, lbl_1_data_1F4, lbl_1_data_1E9, 0x52);
    fn_1_7B10(stack, words);
    fn_1_7A9C(stack, 0);
    if ((u8)(OSCreateThread(&th->thread, func, arg, &stack[words], words * 4, prio, attr) != 0) == 0) {
        OSPanic(lbl_1_data_1E9, 0x60, lbl_1_data_22F);
    }
}

M532Thread *fn_1_78C8(M532Thread *th, s16 flag)
{
    u32 total;
    u32 remain;
    u32 *stack;

    if (th != 0) {
        if (OSIsThreadTerminated(&th->thread) == 0) {
            OSCancelThread(&th->thread);
        }
        if (lbl_1_bss_A00 != 0) {
            total = (uintptr_t)th->thread.stackBase - (uintptr_t)th->thread.stackEnd;
            remain = fn_1_79FC(th);
            printf(lbl_1_data_248, th, remain, total, (100.0 * (f64)remain) / (f64)total);
        }
        stack = th->stack;
        if (stack != 0) {
            HuMemDirectFree(stack);
        }
        if (flag > 0 && th != 0) {
            HuMemDirectFree(th);
        }
    }
    return th;
}

u32 fn_1_79FC(M532Thread *th)
{
    u32 *base;
    u32 *p;

    base = (u32 *)th->thread.stackEnd;
    if (fn_1_7A9C(base, 1) != 0) {
        OSPanic(lbl_1_data_1E9, 0x7F, lbl_1_data_285);
    }
    p = base;
    while (p < (u32 *)th->thread.stackBase) {
        if (*p == 0xDEADBABE) {
            p++;
            continue;
        }
        break;
    }
    return (u32)((uintptr_t)p - (uintptr_t)base);
}

// Stack guard: mode 0 fills the 32 byte guard, mode 1 checks it.
s32 fn_1_7A9C(u32 *p, s32 mode)
{
    u32 *end;
    u32 v;

    end = p + 8;
    switch (mode) {
        case 0:
            while (p != end) {
                *p = 0xDEADBABE;
                p++;
            }
            return 0;
        case 1:
            while (p != end) {
                v = *p;
                p++;
                if (v != 0xDEADBABE) {
                    return 1;
                }
            }
            return 0;
        default:
            return 0;
    }
}

void fn_1_7B10(u32 *p, u32 words)
{
    u32 *end;

    if (lbl_1_bss_A00 != 0) {
        end = p + words;
        while (p != end) {
            *p = 0xDEADBABE;
            p++;
        }
    }
}

// ---------------------------------------------------------------- MgTimer wrappers

M532TimerObj *fn_1_7B5C(M532TimerObj *self, s32 type)
{
    switch (type) {
        case 0:
            self->timer = MgTimerCreate(0);
            break;
        case 1:
            self->timer = MgTimerCreate(1);
            break;
        case 2:
            self->timer = MgTimerCreate(2);
            break;
        default:
            M532_ASSERT_INL(0, lbl_1_data_29F, lbl_1_data_295, 0x1E);
            break;
    }
    return self;
}

M532TimerObj *fn_1_7C24(M532TimerObj *self, s16 flag)
{
    if (self != 0) {
        MgTimerKill(self->timer);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_7C80(M532TimerObj *self, s32 maxTime, s32 endTime, s32 recordTime)
{
    MgTimerParamSet(self->timer, maxTime, endTime, recordTime);
}

void fn_1_7CC8(M532TimerObj *self)
{
    MgTimerValueGet(self->timer);
}

void fn_1_7CF8(M532TimerObj *self, f32 x, f32 y)
{
    MgTimerPosSet(self->timer, x, y);
}

void fn_1_7D38(M532TimerObj *self, f32 *x, f32 *y)
{
    MgTimerPosGet(self->timer, x, y);
}

void fn_1_7D78(M532TimerObj *self, s32 offType)
{
    switch (offType) {
        case 0:
            MgTimerModeOnSet(self->timer, 0);
            return;
        case 1:
            MgTimerModeOnSet(self->timer, 1);
            return;
        case 2:
            MgTimerModeOnSet(self->timer, 2);
            return;
        default:
            M532_ASSERT_INL(0, lbl_1_data_29F, lbl_1_data_295, 0x48);
            return;
    }
}

void fn_1_7E3C(M532TimerObj *self)
{
    MgTimerModeOffSet(self->timer);
}

// Stop the timer the first time it is asked to.
void fn_1_7E6C(M532TimerObj *self)
{
    if (self->timer->stopF == 0) {
        self->timer->stopF = 1;
        self->timer->mode = 1;
    }
}

void fn_1_7E98(M532TimerObj *self)
{
    MgTimerKill(self->timer);
    self->timer = MgTimerCreate(0);
}

u8 fn_1_7ED4(M532TimerObj *self)
{
    return MgTimerDoneCheck(self->timer) != 0;
}

void fn_1_7F14(M532TimerObj *self, u8 r, u8 g, u8 b)
{
    MgTimerWinColorSet(self->timer, r, g, b);
}

// ---------------------------------------------------------------- static initialisers

void fn_1_7F5C(void)
{
    lbl_1_bss_24.next = &lbl_1_bss_24;
    lbl_1_bss_24.prev = &lbl_1_bss_24;
#ifdef __MWERKS__
    __register_global_object(&lbl_1_bss_24, (void *)fn_1_80BC, lbl_1_bss_18);
#endif
    lbl_1_bss_30 = 0x78;
    lbl_1_bss_34 = 0xD2;
    lbl_1_bss_38 = 0xD2;
    fn_1_568C(&lbl_1_bss_3C, 0x1571);
}

void *fn_1_80BC(M532Node *self, s16 flag)
{
    if (self != 0) {
        if (self != 0) {
            self->next->prev = self->prev;
            self->prev->next = self->next;
        }
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

// The module's one static initialiser, hooked into the constructor table.
#ifdef __MWERKS__
__declspec(section ".ctors") void (*const lbl_ctor_61C4)(void) = fn_1_7F5C;
#else
struct M532StaticInit {
    M532StaticInit() { fn_1_7F5C(); }
};
static M532StaticInit lbl_static_init;
#endif
}
