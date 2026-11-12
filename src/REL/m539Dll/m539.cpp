// m539Dll "m539": the module's runtime layer -- camera pose maths, an intrusive
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

#ifdef __MWERKS__
#define M539_RODATA __declspec(section ".rodata")
#else
#define M539_RODATA
#endif

extern "C" {

double tan(double);
void OSInitSemaphore(void *sem, s32 count);
s32 OSWaitSemaphore(void *sem);
s32 OSSignalSemaphore(void *sem);
double fabs(double);
void __register_global_object(void *obj, void *dtor, void *reg);
void ObjectSetup(void);
extern void (*_ctors[])(void);
extern void (*_dtors[])(void);

#define M539_ASSERT(cond, expr, file, line) ((cond) ? (void)0 : fn_1_1D4((expr), (file), (line)))

// The inlined form of the assertion handler (singleton.h, model.cpp, ...).
#define M539_ASSERT_INL(cond, expr, file, line)                                      if (cond) {                                                                          (void)0;                                                                     } else {                                                                             char assertBuf[0x400];                                                           sprintf(assertBuf, lbl_1_data_14, (file), (line), (expr));                       OSPanic(lbl_1_data_32, 0x12, assertBuf);                                     }

// ---------------------------------------------------------------- types

typedef struct M539Node_s {
    struct M539Node_s *next;
    struct M539Node_s *prev;
} M539Node;

// Camera pose: eye position, up vector, look-at target.
typedef struct M539Cam_s {
    Vec pos;
    Vec up;
    Vec target;
} M539Cam;

// Stored pose: position plus orientation quaternion.
typedef struct M539Pose_s {
    Vec pos;
    Quaternion rot;
} M539Pose;

// A pose viewed as the seven raw words the pose-table dump prints.
typedef union M539PoseBits_u {
    M539Pose pose;
    u32 w[7];
} M539PoseBits;

// Task registered on the module-wide list; its vtable lives at +8.
struct M539Task_s;
typedef struct M539TaskVtbl_s {
    void *rtti[2];
    struct M539Task_s *(*dtor)(struct M539Task_s *self, s16 flag);
    void (*onFlag)(struct M539Task_s *self, s32 bit);
    void (*update)(struct M539Task_s *self);
} M539TaskVtbl;

typedef struct M539Task_s {
    M539Node link;
    M539TaskVtbl *vtbl;
    u32 flags;
} M539Task;

// Singleton game master (only the base part that m539 touches).
struct M539Master_s;
typedef struct M539MasterVtbl_s {
    void *rtti[2];
    struct M539Master_s *(*dtor)(struct M539Master_s *self, s16 flag);
    void (*setup)(struct M539Master_s *self, OMOBJMAN *objman);
    void (*update)(struct M539Master_s *self);
    void (*finish)(struct M539Master_s *self);
} M539MasterVtbl;

typedef struct M539Master_s {
    s32 pad0;
    M539MasterVtbl *vtbl;
    s32 slot[4];
    s32 mesId;
    s32 counter;
} M539Master;

// Sub-thread: vtable, OSThread, stack pointer, semaphore, argument.
struct M539SubThread_s;
typedef struct M539ThreadVtbl_s {
    void *rtti[2];
    struct M539SubThread_s *(*dtor)(struct M539SubThread_s *self, s16 flag);
    s32 (*run)(struct M539SubThread_s *self, s32 arg);
} M539ThreadVtbl;

typedef struct M539Thread_s {
    OSThread thread;
    u32 *stack;
} M539Thread;

typedef struct M539Sem_s {
    s32 count;
    OSThreadQueue queue;
} M539Sem;

typedef struct M539SubThread_s {
    M539ThreadVtbl *vtbl;
    M539Thread th;
    M539Sem sem;
    s32 arg;
} M539SubThread;

// Mersenne Twister state.
typedef struct M539Rand_s {
    u32 mt[0x270];
    u32 idx;
} M539Rand;

// Functor kept in an object's data slot: slot +0xC runs it.
struct M539Functor_s;
typedef struct M539FunctorVtbl_s {
    void *rtti[2];
    struct M539Functor_s *(*dtor)(struct M539Functor_s *self, s16 flag);
    void (*run)(struct M539Functor_s *self, OMOBJ *obj);
} M539FunctorVtbl;

typedef struct M539Functor_s {
    M539FunctorVtbl *vtbl;
} M539Functor;

// A wrapper that owns a Hu3D model / MgTimer handle.
typedef struct M539Model_s {
    HU3DMODELID id;
} M539Model;

typedef struct M539TimerObj_s {
    MGTIMER *timer;
} M539TimerObj;

// ---------------------------------------------------------------- data

char lbl_1_data_0[] = "ptr != NULL";
char lbl_1_data_C[] = "new.cpp";
char lbl_1_data_14[] = "\nASSERTION FAILED at %s:%d\n%s";
char lbl_1_data_32[] = "assert.cpp";
char lbl_1_data_3D[] = "{{0x%08x,0x%08x,0x%08x},{0x%08x,0x%08x,0x%08x,0x%08x}},\n";
char lbl_1_data_76[] = "singleton.h";
char lbl_1_data_82[] = "ms_pointer == 0";
char lbl_1_data_92[] = "ms_pointer != 0";
char lbl_1_data_A2[] = "model.cpp";
char lbl_1_data_AC[] = "p != NULL";
char lbl_1_data_B6[] = "om.cpp";
char lbl_1_data_BD[] = "subthread.cpp";
char lbl_1_data_CB[] = "::OSGetCurrentThread() == m_th";
char lbl_1_data_EA[] = "!::OSIsThreadTerminated(m_th)";
char lbl_1_data_108[] = "thread.cpp";
char lbl_1_data_113[] = "priority >= OS_PRIORITY_MIN && priority <= OS_PRIORITY_MAX";
char lbl_1_data_14E[] = "OSCreateThread() failed.";
char lbl_1_data_167[] = "delete CThread@%p\nstack remain/total = 0x%08x/0x%08x (%f%%)\n";
char lbl_1_data_1A4[] = "stack overflow\n";
char lbl_1_data_1B4[] = "timer.cpp";
char lbl_1_data_1BE[] = "false";

M539SubThread *fn_1_5608(M539SubThread *self, s16 flag);
M539Task *fn_1_1EA8(M539Task *self, s16 flag);
void fn_1_1DDC(M539Task *self);
M539Master *fn_1_20CC(M539Master *self, s16 flag);
void fn_1_246C(M539Master *self);
M539Functor *fn_1_4934(M539Functor *self, s16 flag);
void fn_1_48C0(M539Functor *self, OMOBJ *obj);
void fn_1_4874(OMOBJ *obj);

M539ThreadVtbl lbl_1_data_1C4 = { { 0, 0 }, fn_1_5608, 0 };
M539FunctorVtbl lbl_1_data_1D4 = { { 0, 0 }, fn_1_4934, fn_1_48C0 };
M539MasterVtbl lbl_1_data_1E4 = { { 0, 0 }, fn_1_20CC, 0, fn_1_246C, 0 };
M539TaskVtbl lbl_1_data_1FC = { { 0, 0 }, fn_1_1EA8, 0, fn_1_1DDC };

// ---------------------------------------------------------------- bss

u8 lbl_1_bss_0[0xC];
M539Node lbl_1_bss_C;
s32 lbl_1_bss_14;
s32 lbl_1_bss_18;
s32 lbl_1_bss_1C;
s32 lbl_1_bss_20;
M539Rand lbl_1_bss_24;
u8 lbl_1_bss_9E8;
M539Master *lbl_1_bss_9EC;

// ---------------------------------------------------------------- code

void fn_1_1D4(const char *expr, const char *file, s32 line);
int fn_1_234(void *unused, const char *fmt, ...);
void *fn_1_6324(M539Node *self, s16 flag);
void fn_1_408(M539Cam *self, Vec *x, Vec *y, Vec *z);
void fn_1_56C(M539Cam *self, Vec *delta);

void *fn_1_0(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M539_ASSERT(p != 0, lbl_1_data_0, lbl_1_data_C, 0x17);
    return p;
}

void *fn_1_64(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M539_ASSERT(p != 0, lbl_1_data_0, lbl_1_data_C, 0x17);
    return p;
}

void fn_1_C8(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_100(void *ptr)
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

void fn_1_1D4(const char *expr, const char *file, s32 line)
{
    char buf[0x40C];

    sprintf(buf, lbl_1_data_14, file, line, expr);
    OSPanic(lbl_1_data_32, 0x12, buf);
}

int fn_1_234(void *unused, const char *fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = vprintf(fmt, ap);
    va_end(ap);
    return ret;
}

// Orbit: rotate about the camera X axis (pitch) then world Y (yaw).
void fn_1_2D8(M539Cam *self, f32 pitch, f32 yaw)
{
    Vec ax, ay, az;
    Mtx rotA, rotB, rot;

    fn_1_408(self, &ax, &ay, &az);
    PSMTXRotAxisRad(rotA, &ax, 0.017453292f * pitch);
    PSMTXRotRad(rotB, 'Y', 0.017453292f * yaw);
    PSMTXConcat(rotA, rotB, rot);
    PSMTXMultVecSR(rot, &az, &az);
    PSMTXMultVecSR(rot, &ay, &self->up);
    PSVECSubtract(&self->pos, &az, &self->target);
}

void fn_1_39C(M539Cam *self, f32 angle)
{
    Mtx m;
    Vec axis;

    PSVECSubtract(&self->target, &self->pos, &axis);
    PSMTXRotAxisRad(m, &axis, 0.017453292f * angle);
    PSMTXMultVec(m, &self->up, &self->up);
}

// Camera basis: the three rows of the look-at matrix.
void fn_1_408(M539Cam *self, Vec *x, Vec *y, Vec *z)
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

void fn_1_4B0(M539Cam *self, Mtx out)
{
    C_MTXLookAt(out, &self->pos, &self->up, &self->target);
    PSMTXInverse(out, out);
}

void fn_1_504(M539Cam *self, Vec *local)
{
    Mtx m;
    Vec world;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    PSMTXMultVecSR(m, local, &world);
    fn_1_56C(self, &world);
}

void fn_1_56C(M539Cam *self, Vec *delta)
{
    PSVECAdd(delta, &self->target, &self->target);
    PSVECAdd(delta, &self->pos, &self->pos);
}

void fn_1_5C0(M539Cam *self)
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

void fn_1_654(M539Cam *self, M539Pose *pose)
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

void fn_1_6F4(M539Cam *self, M539Pose *out)
{
    Mtx m;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    out->pos.x = self->pos.x;
    out->pos.y = self->pos.y;
    out->pos.z = self->pos.z;
    C_QUATMtx(&out->rot, m);
}

void fn_1_76C(M539Cam *self, M539Pose *a, M539Pose *b, f32 t)
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

void fn_1_87C(M539Cam *self, M539Cam *src)
{
    Vec delta;

    PSVECSubtract(&src->pos, &self->pos, &delta);
    self->pos.x = src->pos.x;
    self->pos.y = src->pos.y;
    self->pos.z = src->pos.z;
    PSVECAdd(&self->target, &delta, &self->target);
}

void fn_1_8E8(M539Cam *self, Vec *a, Vec *b, f32 t)
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

void fn_1_9A0(M539Cam *self, u16 camBit)
{
    Hu3DCameraPosSetV(camBit, &self->pos, &self->up, &self->target);
}

void fn_1_9E0(M539Cam *self, u16 camBit)
{
    Hu3DCameraPosGet(camBit, &self->pos, &self->up, &self->target);
}

// Dump the current camera as a pose table row (floats printed as raw words).
void fn_1_A20(M539Cam *self)
{
    Mtx m;
    M539PoseBits out;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    out.pose.pos.x = self->pos.x;
    out.pose.pos.y = self->pos.y;
    out.pose.pos.z = self->pos.z;
    C_QUATMtx(&out.pose.rot, m);
    printf(lbl_1_data_3D, out.w[0], out.w[1], out.w[2], out.w[3], out.w[4], out.w[5], out.w[6]);
}

f32 fn_1_AB8(s32 pad)
{
    return 0.017857144f * HuPadStkX[pad];
}

f32 fn_1_B0C(s32 pad)
{
    return 0.017857144f * HuPadStkY[pad];
}

f32 fn_1_B60(s32 pad)
{
    return 0.016949153f * HuPadSubStkX[pad];
}

f32 fn_1_BB4(s32 pad)
{
    return 0.016949153f * HuPadSubStkY[pad];
}

// Free-fly debug camera driven by pad `pad`.
void fn_1_C08(M539Cam *self, s32 pad)
{
    Mtx inv, rot, rotB, rotA, m, dump;
    M539PoseBits out;
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
    yaw = -4.0f * fn_1_AB8(pad);
    pitch = -4.0f * fn_1_B0C(pad);
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
    mv.x = mv.x + (speed * fn_1_B60(pad));
    mv.z = 0.0f;
    PSMTXMultVecSR(inv, &mv, &mv);
    mv.y = mv.y + (speed * fn_1_BB4(pad));
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
        printf(lbl_1_data_3D, out.w[0], out.w[1], out.w[2], out.w[3], out.w[4], out.w[5], out.w[6]);
    }
}

// ---------------------------------------------------------------- intrusive list

typedef u8 (*M539Cmp)(M539Node *a, M539Node *b);

M539Node *fn_1_1B48(M539Node *list);
M539Node *fn_1_1B9C(M539Cmp cmp, M539Node *a, M539Node *b);
M539Node *fn_1_14A0(M539Cmp cmp, M539Node *list);
void fn_1_2370(OMOBJ *obj);
void fn_1_2470(void);
void fn_1_2C18(void);
void fn_1_47E4(void *ptr);

void fn_1_1264(M539Node *self)
{
    self->next = self;
    self->prev = self;
}

void *fn_1_1270(M539Node *self, s16 flag)
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

void fn_1_12DC(M539Node *self)
{
    self->next->prev = self->prev;
    self->prev->next = self->next;
    self->next = self;
    self->prev = self;
}

// Insert `self` right after `other`.
void fn_1_1300(M539Node *self, M539Node *other)
{
    M539Node *at = other;

    self->prev = at;
    self->next = at->next;
    at->next->prev = self;
    at->next = self;
}

// Insert `self` right before `other`.
void fn_1_1330(M539Node *self, M539Node *other)
{
    M539Node *at = other;

    self->next = at;
    self->prev = at->prev;
    at->prev->next = self;
    at->prev = self;
}

// Move every node of `self` in front of `other`.
void fn_1_1360(M539Node *self, M539Node *other)
{
    M539Node *first;
    M539Node *last;
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
void fn_1_13E8(M539Node *self, M539Cmp cmp)
{
    M539Node *node;
    M539Node *prev;
    u8 empty;

    empty = 0;
    if (self->next == self && self->next == self->prev) {
        empty = 1;
    }
    if (empty == 0) {
        self->prev->next = 0;
        node = fn_1_14A0(cmp, self->next);
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

M539Node *fn_1_14A0(M539Cmp cmp, M539Node *list)
{
    M539Node *second;
    M539Node *a;
    M539Node *b;

    if (list == 0) {
        return 0;
    }
    if (list->next == 0) {
        return list;
    }
    second = fn_1_1B48(list);
    a = fn_1_14A0(cmp, list);
    b = fn_1_14A0(cmp, second);
    return fn_1_1B9C(cmp, a, b);
}

// Cut the list in the middle and return the second half.
M539Node *fn_1_1B48(M539Node *list)
{
    M539Node *fast;
    M539Node *second;

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

M539Node *fn_1_1B9C(M539Cmp cmp, M539Node *a, M539Node *b)
{
    M539Node head;
    M539Node *tail;

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

void fn_1_1C48(s32 bit)
{
    M539Task *t;

    t = (M539Task *)lbl_1_bss_C.next;
    lbl_1_bss_14 = bit;
    while ((M539Node *)t != &lbl_1_bss_C) {
        t->flags |= 1 << bit;
        t->vtbl->onFlag(t, bit);
        t = (M539Task *)t->link.next;
    }
}

s32 fn_1_1CE0(void)
{
    M539Task *t;

    t = (M539Task *)lbl_1_bss_C.next;
    while (1) {
        if ((M539Node *)t == &lbl_1_bss_C) {
            return 1;
        }
        if ((t->flags & (1 << lbl_1_bss_14)) != 0) {
            return 0;
        }
        t = (M539Task *)t->link.next;
    }
}

void fn_1_1D6C(void)
{
    M539Task *t;

    t = (M539Task *)lbl_1_bss_C.next;
    while ((M539Node *)t != &lbl_1_bss_C) {
        t->vtbl->update(t);
        t = (M539Task *)t->link.next;
    }
}

void fn_1_1DDC(M539Task *self)
{
}

// Destroy every registered task, last one first.
void fn_1_1DE0(void)
{
    M539Task *t;
    u8 empty;

    while (1) {
        empty = 0;
        if (lbl_1_bss_C.next == &lbl_1_bss_C && lbl_1_bss_C.next == lbl_1_bss_C.prev) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        t = (M539Task *)lbl_1_bss_C.prev;
        if (t != 0) {
            t->vtbl->dtor(t, 1);
        }
    }
}

M539Task *fn_1_1EA8(M539Task *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_1FC;
        if (self != 0) {
            if (self != 0) {
                self->link.next->prev = self->link.prev;
                self->link.prev->next = self->link.next;
            }
        }
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

s32 fn_1_1F30(void)
{
    return lbl_1_bss_14;
}

// ---------------------------------------------------------------- game master

M539Master *fn_1_1F40(M539Master *self)
{
    s32 i;

    M539_ASSERT_INL(lbl_1_bss_9EC == 0, lbl_1_data_82, lbl_1_data_76, 0x35);
    lbl_1_bss_9EC = self;
    self->vtbl = &lbl_1_data_1E4;
    self->counter = 0;
    i = 0;
    while (i < 4) {
        self->slot[i] = -1;
        i++;
    }
    return self;
}

M539Master *fn_1_2010(M539Master *self, s16 flag)
{
    if (self != 0) {
        M539_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_92, lbl_1_data_76, 0x3D);
        lbl_1_bss_9EC = 0;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

M539Master *fn_1_20CC(M539Master *self, s16 flag)
{
    M539Master *m;
    s32 i;
    s32 p;

    if (self != 0) {
        self->vtbl = &lbl_1_data_1E4;
        if (mgDataTbl[GwSystem.mgNo].type != 3) {
            M539_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_92, lbl_1_data_76, 0x27);
            m = lbl_1_bss_9EC;
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
            M539_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_92, lbl_1_data_76, 0x3D);
            lbl_1_bss_9EC = 0;
        }
        if (flag > 0) {
            fn_1_47E4(self);
        }
    }
    return self;
}

void fn_1_2284(M539Master *self, s32 objMax, s32 prio)
{
    OMOBJMAN *om;

    om = omInitObjMan((s16)objMax, prio);
    omGameSysInit(om);
    self->vtbl->setup(self, om);
    omAddObjEx(om, 0, 0, 0, -1, fn_1_2370);
}

void fn_1_2308(M539Master *self)
{
    OMOBJMAN *om;

    om = MgActorObjectSetup();
    self->vtbl->setup(self, om);
    omAddObjEx(om, 0, 0, 0, -1, fn_1_2370);
}

void fn_1_2400(M539Master *self);

void fn_1_2370(OMOBJ *obj)
{
    if (omSysExitReq != 0 && WipeCheckEnd() == 0) {
        fn_1_2C18();
        HuAudFadeOut(0x3E8);
        obj->objFunc = (OMOBJFUNC)fn_1_2470;
        return;
    }
    fn_1_2400(lbl_1_bss_9EC);
}

void fn_1_2400(M539Master *self)
{
    self->vtbl->update(self);
    if (self->counter > 0) {
        if (--self->counter == 0) {
        }
    }
}

void fn_1_246C(M539Master *self)
{
}

void fn_1_2470(void)
{
    M539Master *m;

    m = lbl_1_bss_9EC;
    if (WipeCheckEnd() != 0) {
        m->vtbl->update(m);
        if (m->counter > 0) {
            if (--m->counter == 0) {
            }
        }
        return;
    }
    m->vtbl->finish(m);
    if (m != 0) {
        m->vtbl->dtor(m, 1);
    }
    CharModelKill(-1);
    omOvlReturnEx(1, 1);
}

// Register a player in the first free slot.
void fn_1_255C(s32 player)
{
    s32 *slots;
    s32 i;

    M539_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_92, lbl_1_data_76, 0x27);
    slots = lbl_1_bss_9EC->slot;
    i = 0;
    while (i < 4) {
        if (slots[i] < 0) {
            slots[i] = player;
            return;
        }
        i++;
    }
}

s32 fn_1_2630(s32 player)
{
    s32 i;

    M539_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_92, lbl_1_data_76, 0x27);
    i = 0;
    while (i < 4) {
        if (player == lbl_1_bss_9EC->slot[i]) {
            return 1;
        }
        i++;
    }
    return 0;
}

void fn_1_2704(void)
{
    M539Master *m;

    M539_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_92, lbl_1_data_76, 0x27);
    m = lbl_1_bss_9EC;
    m->mesId = GameMesCreate(2, 0);
}

void fn_1_27BC(void)
{
    M539Master *m;

    M539_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_92, lbl_1_data_76, 0x27);
    m = lbl_1_bss_9EC;
    m->mesId = GameMesCreate(2, 1);
}

s32 fn_1_2874(void)
{
    M539Master *m;
    s32 ret;

    M539_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_92, lbl_1_data_76, 0x27);
    m = lbl_1_bss_9EC;
    ret = 0;
    if (GameMesStatGet(-1) == 0 && (u8)(m->counter <= 0) != 0) {
        ret = 1;
    }
    return ret;
}

// Announce the result: sort the registered players and print their characters.
void fn_1_2948(void)
{
    M539Master *m;
    s32 *slots;
    s32 chars[4];
    s32 n;
    s32 i;
    s32 j;
    s32 k;
    s32 tmp;
    s16 charNo;

    M539_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_92, lbl_1_data_76, 0x27);
    m = lbl_1_bss_9EC;
    m->counter = lbl_1_bss_1C;
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

void fn_1_2B14(s32 mes)
{
    M539Master *m;

    if (_CheckFlag(0x1000F) == 0) {
        M539_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_92, lbl_1_data_76, 0x27);
        m = lbl_1_bss_9EC;
        m->mesId = GameMesCreate(0xE, mes);
        HuAudJinglePlay(0x86);
    }
}

void fn_1_2BEC(void)
{
    WipeCreate(1, 5, 0x3C);
}

void fn_1_2C18(void)
{
    WipeCreate(2, 6, 0x3C);
}

void fn_1_2C44(void)
{
    omSysExitReq = 1;
}

// ---------------------------------------------------------------- Hu3D model wrappers

M539Model *fn_1_2C58(M539Model *self, void *data)
{
    self->id = Hu3DModelCreate(data);
    return self;
}

M539Model *fn_1_2C94(M539Model *self, s32 dataNum)
{
    self->id = Hu3DModelCreateData(dataNum);
    return self;
}

M539Model *fn_1_2CDC(M539Model *self, HU3DMODELHOOK hook)
{
    self->id = Hu3DHookFuncCreate(hook);
    return self;
}

// Build a character model; `type` selects the model variant bit.
M539Model *fn_1_2D18(M539Model *self, s32 charNo, s32 type)
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

inline s32 M539ModelIdGet(M539Model *m)
{
    return m->id;
}

M539Model *fn_1_2DB4(M539Model *self, M539Model *parent)
{
    self->id = Hu3DModelLink(M539ModelIdGet(parent));
    return self;
}

void fn_1_2E08(M539Model *self, u32 attr)
{
    Hu3DModelAttrSet(self->id, attr);
}

void fn_1_2E40(M539Model *self, u32 attr)
{
    Hu3DModelAttrReset(self->id, attr);
}

// Apply an object's transform (translation, rotation, scale) to the model.
void fn_1_2E78(M539Model *self, OMOBJ *obj)
{
    HU3DMODELID id;

    id = self->id;
    Hu3DModelPosSet(id, obj->trans.x, obj->trans.y, obj->trans.z);
    Hu3DModelRotSet(id, obj->rot.x, obj->rot.y, obj->rot.z);
    Hu3DModelScaleSet(id, obj->scale.x, obj->scale.y, obj->scale.z);
}

void fn_1_2EF4(M539Model *self, Vec *pos)
{
    Hu3DModelPosSet(self->id, pos->x, pos->y, pos->z);
}

void fn_1_2F3C(M539Model *self, Vec *rot)
{
    Hu3DModelRotSet(self->id, rot->x, rot->y, rot->z);
}

void fn_1_2F84(M539Model *self, Vec *scale)
{
    Hu3DModelScaleSet(self->id, scale->x, scale->y, scale->z);
}

void fn_1_2FCC(M539Model *self, f32 scale)
{
    Hu3DModelScaleSet(self->id, scale, scale, scale);
}

void fn_1_3014(M539Model *self, f32 tpLvl)
{
    Hu3DModelTPLvlSet(self->id, tpLvl);
}

void fn_1_304C(M539Model *self, s16 layer)
{
    Hu3DModelLayerSet(self->id, layer);
}

void fn_1_3084(M539Model *self, u16 camBit)
{
    Hu3DModelCameraSet(self->id, camBit);
}

void fn_1_30BC(M539Model *self, void *data)
{
    Hu3DData[self->id].hookData = data;
}

MtxPtr fn_1_30D8(M539Model *self)
{
    return Hu3DData[self->id].mtx;
}

M539Model *fn_1_30F4(M539Model *self, s16 flag)
{
    if (self != 0) {
        Hu3DModelKill(self->id);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

inline HSFOBJECT *M539ObjGet(M539Model *self, char *name)
{
    HSFOBJECT *obj;

    obj = Hu3DModelObjPtrGet(self->id, name);
    M539_ASSERT_INL(obj != 0, lbl_1_data_AC, lbl_1_data_A2, 0x8A);
    return obj;
}

HSFOBJECT *fn_1_3150(M539Model *self, char *name)
{
    return M539ObjGet(self, name);
}

void fn_1_31E4(M539Model *self)
{
    Hu3DModelShadowSet(self->id);
}

void fn_1_3214(M539Model *self)
{
    Hu3DModelShadowMapSet(self->id);
}

void fn_1_3244(M539Model *self, char *name)
{
    Hu3DModelShadowMapObjSet(self->id, name);
}

void fn_1_327C(M539Model *self)
{
    Hu3DModelShadowMapObjReset(self->id);
}

void fn_1_32AC(M539Model *self, char *name)
{
    HSFOBJECT *obj;
    u32 *flags;

    obj = M539ObjGet(self, name);
    flags = (u32 *)obj->constData;
    *flags &= ~8;
}

void fn_1_3358(M539Model *self, char *name)
{
    HSFOBJECT *obj;

    obj = M539ObjGet(self, name);
    obj->flags |= 0x400;
}

void fn_1_3400(M539Model *self, char *name)
{
    HSFOBJECT *obj;

    obj = M539ObjGet(self, name);
    obj->flags &= ~0x400;
}

void fn_1_34A8(M539Model *self, char *name)
{
    HSFOBJECT *obj;
    HSFTRANSFORM *base;

    obj = M539ObjGet(self, name);
    obj->flags |= 0x400;
    base = &obj->mesh.base;
    base->scale.x = 0.0f;
    base->scale.y = 0.0f;
    base->scale.z = 0.0f;
}

void fn_1_3584(M539Model *self, char *name)
{
    HSFOBJECT *obj;
    HSFTRANSFORM *base;

    obj = M539ObjGet(self, name);
    obj->flags &= ~0x400;
    base = &obj->mesh.base;
    base->scale.x = 1.0f;
    base->scale.y = 1.0f;
    base->scale.z = 1.0f;
}

void fn_1_3660(M539Model *self, s32 time)
{
    Hu3DMotionTimeSet(M539ModelIdGet(self), (f32)time);
}

// ---------------------------------------------------------------- Mersenne Twister

extern const u32 lbl_1_rodata_50[2] = { 0, 0x9908B0DF };

void fn_1_36CC(M539Rand *r, u32 seed)
{
    r->mt[0] = seed;
    r->idx = 1;
    while (r->idx < 0x270) {
        r->mt[r->idx] = r->idx + (((r->mt[r->idx - 1]) ^ ((r->mt[r->idx - 1]) >> 30)) * 0x6C078965);
        r->idx++;
    }
}

void fn_1_3740(M539Rand *r, u32 *key, u32 len)
{
    u32 i;
    u32 j;
    u32 k;

    fn_1_36CC(r, 19650218);
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

u32 fn_1_3904(M539Rand *r)
{
    u32 k;
    u32 y;

    if (r->idx >= 0x270) {
        k = 0;
        while (k < 0xE3) {
            y = (r->mt[k] & 0x80000000) | (r->mt[k + 1] & 0x7FFFFFFF);
            r->mt[k] = lbl_1_rodata_50[y & 1] ^ ((y >> 1) ^ r->mt[k + 0x18D]);
            k++;
        }
        while (k < 0x26F) {
            y = (r->mt[k] & 0x80000000) | (r->mt[k + 1] & 0x7FFFFFFF);
            r->mt[k] = lbl_1_rodata_50[y & 1] ^ ((y >> 1) ^ r->mt[k - 0xE3]);
            k++;
        }
        y = (r->mt[0x26F] & 0x80000000) | (r->mt[0] & 0x7FFFFFFF);
        r->mt[0x26F] = (y >> 1) ^ r->mt[0x18C] ^ lbl_1_rodata_50[y & 1];
        r->idx = 0;
    }
    y = r->mt[r->idx++];
    y ^= y >> 11;
    y ^= (y << 7) & 0x9D2C5680;
    y ^= (y << 15) & 0xEFC60000;
    return y ^ (y >> 18);
}

u32 fn_1_3A90(M539Rand *r)
{
    return fn_1_3904(r) >> 1;
}

f32 fn_1_3C28(M539Rand *r)
{
    return 2.3283064e-10f * (f32)fn_1_3904(r);
}

f32 fn_1_3DEC(M539Rand *r)
{
    return 2.3283064e-10f * (f32)fn_1_3904(r);
}

f32 fn_1_3FB0(M539Rand *r)
{
    return 2.3283064e-10f * (0.5f + (f32)fn_1_3904(r));
}

f64 fn_1_4184(M539Rand *r)
{
    u32 a;
    u32 b;

    a = fn_1_3904(r) >> 5;
    b = fn_1_3904(r) >> 6;
    return 1.1102230246251565e-16 * ((67108864.0 * (f64)a) + (f64)b);
}

f32 fn_1_44FC(M539Rand *r)
{
    return 1.1920929e-7f * (f32)(fn_1_3904(r) >> 9);
}

// ---------------------------------------------------------------- allocation / functor objects

void *fn_1_46CC(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M539_ASSERT_INL(p != 0, lbl_1_data_0, lbl_1_data_B6, 0x18);
    return p;
}

void *fn_1_4758(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M539_ASSERT_INL(p != 0, lbl_1_data_0, lbl_1_data_B6, 0x1F);
    return p;
}

void fn_1_47E4(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_481C(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

// Functor owned by an object: the object's data slot points at it.
void fn_1_4854(M539Functor *self, OMOBJ *obj)
{
    self->vtbl = &lbl_1_data_1D4;
    obj->data = self;
    obj->objFunc = fn_1_4874;
}

void fn_1_4874(OMOBJ *obj)
{
    M539Functor *self;

    self = (M539Functor *)obj->data;
    self->vtbl->run(self, obj);
}

void fn_1_48C0(M539Functor *self, OMOBJ *obj)
{
}

void fn_1_48C4(OMOBJ *obj)
{
    M539Functor *fn;
    M539Functor *p;

    fn = (M539Functor *)obj->data;
    p = fn;
    if (p != 0) {
        p = p->vtbl->dtor(p, 1);
    }
    omDelObjEx(HuPrcCurrentGet(), obj);
}

M539Functor *fn_1_4934(M539Functor *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_1D4;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_4994(OMOBJMAN *man, OMOBJ *obj)
{
    M539Functor *fn;
    M539Functor *p;

    fn = (M539Functor *)obj->data;
    p = fn;
    if (p != 0) {
        p = p->vtbl->dtor(p, 1);
    }
    omDelObjEx(man, obj);
}

// ---------------------------------------------------------------- global random number generator

void fn_1_4A08(u32 seed)
{
    fn_1_36CC(&lbl_1_bss_24, seed);
}

u32 fn_1_4AE0(void)
{
    return fn_1_3904(&lbl_1_bss_24) >> 1;
}

f32 fn_1_4D1C(void)
{
    return 1.1920929e-7f * (f32)(fn_1_3904(&lbl_1_bss_24) >> 9);
}

f32 fn_1_4F90(f32 scale)
{
    return scale * fn_1_4184(&lbl_1_bss_24);
}

f32 fn_1_4FC8(f32 from, f32 to)
{
    return from + (to - from) * fn_1_4184(&lbl_1_bss_24);
}

// ---------------------------------------------------------------- shadow / collision helpers

// Distance at which a 5 degree field of view spans `size`; `out` = target - dir * distance.
f32 fn_1_501C(Vec *out, Vec *dir, Vec *target, f32 size)
{
    f32 dist;

    dist = size / tan(0.0872664600610733);
    PSVECScale(dir, out, dist);
    PSVECSubtract(target, out, out);
    return dist;
}

extern const Vec lbl_1_rodata_74[2] = { { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } };

// Create a (multi-camera) shadow projecting along `dir` onto `target`.
void fn_1_50C4(s16 camBit, Vec *dir, Vec *target, f32 width, f32 depth)
{
    Vec dn;
    Vec pos;
    f32 halfDepth;
    f32 dist;
    f32 nearZ;
    f32 farZ;
    s32 upIdx;
    s32 count;
    s32 i;
    const Vec *up;

    halfDepth = depth * 0.5f;
    PSVECNormalize(dir, &dn);
    if (fabs(dn.y) < 0.5f) {
        upIdx = 0;
    } else {
        upIdx = 1;
    }
    up = &lbl_1_rodata_74[upIdx];
    dist = (f32)((width * 0.5f) / tan(0.0872664600610733));
    PSVECScale(&dn, &pos, dist);
    PSVECSubtract(target, &pos, &pos);
    nearZ = dist - halfDepth;
    farZ = dist + halfDepth;
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
        Hu3DShadowPosSet(&pos, (Vec *)up, target);
        Hu3DShadowSizeSet(0x1E0);
        Hu3DShadowTPLvlSet(0.4f);
        return;
    }
    Hu3DShadowMultiCreate(10.0f, nearZ, farZ, camBit);
    Hu3DShadowMultiPosSet(&pos, (Vec *)up, target, camBit);
    Hu3DShadowMultiSizeSet(0x1E0, camBit);
    Hu3DShadowMultiTPLvlSet(0.4f, camBit);
}

// Sphere: centre and radius.
typedef struct M539Sphere_s {
    Vec pos;
    f32 radius;
} M539Sphere;

u32 fn_1_5340(M539Sphere *a, M539Sphere *b)
{
    Vec d;
    f32 sum;

    PSVECSubtract(&a->pos, &b->pos, &d);
    sum = a->radius + b->radius;
    return PSVECMag(&d) < sum;
}

// Push two overlapping spheres apart; ka/kb weight the move of each.
s32 fn_1_53E8(M539Sphere *a, M539Sphere *b, f32 ka, f32 kb)
{
    Vec d;
    Vec off;
    f32 sum;
    f32 dist;

    sum = a->radius + b->radius;
    PSVECSubtract(&b->pos, &a->pos, &d);
    dist = PSVECMag(&d);
    if (dist >= sum) {
        return 0;
    }
    PSVECScale(&d, &d, (kb * (sum - dist)) / dist);
    PSVECScale(&d, &off, -ka);
    PSVECAdd(&off, &a->pos, &a->pos);
    PSVECScale(&d, &off, 1.0f - ka);
    PSVECAdd(&off, &b->pos, &b->pos);
    return 1;
}

// ---------------------------------------------------------------- threads

s32 fn_1_5568(void)
{
    return OSGetThreadPriority(OSGetCurrentThread());
}

s32 fn_1_5850(M539SubThread *self);
void fn_1_58AC(M539Thread *th, void *(*func)(void *), void *arg, u32 size, s32 prio, u16 attr);

M539SubThread *fn_1_558C(M539SubThread *self, u32 stackSize)
{
    self->vtbl = &lbl_1_data_1C4;
    fn_1_58AC(&self->th, (void *(*)(void *))fn_1_5850, self, stackSize, OSGetThreadPriority(OSGetCurrentThread()), 1);
    OSInitSemaphore(&self->sem, 0);
    return self;
}

void fn_1_5B30(M539Thread *th, s16 flag);

M539SubThread *fn_1_5608(M539SubThread *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_1C4;
        fn_1_5B30(&self->th, -1);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

u8 fn_1_5674(M539SubThread *self)
{
    return OSIsThreadTerminated(&self->th.thread) != 0;
}

u32 fn_1_5C64(M539Thread *th);

s32 fn_1_56BC(M539SubThread *self, s32 arg)
{
    if (OSIsThreadTerminated(&self->th.thread) != 0) {
        return self->arg;
    }
    self->arg = arg;
    OSResumeThread(&self->th.thread);
    OSWaitSemaphore(&self->sem);
    fn_1_5C64(&self->th);
    OSCheckActiveThreads();
    return self->arg;
}

s32 fn_1_574C(M539SubThread *self, s32 arg)
{
    M539_ASSERT_INL(OSGetCurrentThread() == &self->th.thread, lbl_1_data_CB, lbl_1_data_BD, 0x47);
    M539_ASSERT_INL(OSIsThreadTerminated(&self->th.thread) == 0, lbl_1_data_EA, lbl_1_data_BD, 0x4A);
    self->arg = arg;
    OSSignalSemaphore(&self->sem);
    OSSuspendThread(&self->th.thread);
    return self->arg;
}

s32 fn_1_5850(M539SubThread *self)
{
    self->vtbl->run(self, self->arg);
    OSSignalSemaphore(&self->sem);
    return 0;
}

void fn_1_5A0C(M539Thread *th, void *(*func)(void *), void *arg, u32 *stack, u32 words, s32 prio, u16 attr);

void fn_1_58AC(M539Thread *th, void *(*func)(void *), void *arg, u32 size, s32 prio, u16 attr)
{
    u32 words;
    u32 *stack;

    words = size >> 2;
    stack = (u32 *)HuMemDirectMallocNum(HEAP_HEAP, words * 4, 0x10000000);
    M539_ASSERT_INL(stack != 0, lbl_1_data_0, lbl_1_data_C, 0x17);
    th->stack = stack;
    fn_1_5A0C(th, func, arg, stack, words, prio, attr);
}

void fn_1_5990(M539Thread *th, void *(*func)(void *), void *arg, u32 *stack, u32 size, s32 prio, u16 attr)
{
    th->stack = 0;
    fn_1_5A0C(th, func, arg, stack, size >> 2, prio, attr);
}

void fn_1_5D78(u32 *p, u32 words);
s32 fn_1_5D04(u32 *p, s32 mode);

void fn_1_5A0C(M539Thread *th, void *(*func)(void *), void *arg, u32 *stack, u32 words, s32 prio, u16 attr)
{
    u8 ok;

    ok = 0;
    if (prio >= 0 && prio <= 0x1F) {
        ok = 1;
    }
    M539_ASSERT_INL(ok != 0, lbl_1_data_113, lbl_1_data_108, 0x52);
    fn_1_5D78(stack, words);
    fn_1_5D04(stack, 0);
    if ((u8)(OSCreateThread(&th->thread, func, arg, &stack[words], words * 4, prio, attr) != 0) == 0) {
        OSPanic(lbl_1_data_108, 0x60, lbl_1_data_14E);
    }
}

void fn_1_5B30(M539Thread *th, s16 flag)
{
    u32 total;
    u32 remain;

    if (th != 0) {
        if (OSIsThreadTerminated(&th->thread) == 0) {
            OSCancelThread(&th->thread);
        }
        if (lbl_1_bss_9E8 != 0) {
            total = (uintptr_t)th->thread.stackBase - (uintptr_t)th->thread.stackEnd;
            remain = fn_1_5C64(th);
            printf(lbl_1_data_167, th, remain, total, (100.0 * (f64)remain) / (f64)total);
        }
        if (th->stack != 0) {
            HuMemDirectFree(th->stack);
        }
        if (flag > 0 && th != 0) {
            HuMemDirectFree(th);
        }
    }
}

u32 fn_1_5C64(M539Thread *th)
{
    u32 *base;
    u32 *p;

    base = th->thread.stackEnd;
    if (fn_1_5D04(base, 1) != 0) {
        OSPanic(lbl_1_data_108, 0x7F, lbl_1_data_1A4);
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
s32 fn_1_5D04(u32 *p, s32 mode)
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

void fn_1_5D78(u32 *p, u32 words)
{
    u32 *end;

    if (lbl_1_bss_9E8 != 0) {
        end = p + words;
        while (p != end) {
            *p = 0xDEADBABE;
            p++;
        }
    }
}

// ---------------------------------------------------------------- MgTimer wrappers

M539TimerObj *fn_1_5DC4(M539TimerObj *self, s32 type)
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
            M539_ASSERT_INL(0, lbl_1_data_1BE, lbl_1_data_1B4, 0x1E);
            break;
    }
    return self;
}

M539TimerObj *fn_1_5E8C(M539TimerObj *self, s16 flag)
{
    if (self != 0) {
        MgTimerKill(self->timer);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_5EE8(M539TimerObj *self, s32 maxTime, s32 endTime, s32 recordTime)
{
    MgTimerParamSet(self->timer, maxTime, endTime, recordTime);
}

void fn_1_5F30(M539TimerObj *self)
{
    MgTimerValueGet(self->timer);
}

void fn_1_5F60(M539TimerObj *self, f32 x, f32 y)
{
    MgTimerPosSet(self->timer, x, y);
}

void fn_1_5FA0(M539TimerObj *self, f32 *x, f32 *y)
{
    MgTimerPosGet(self->timer, x, y);
}

void fn_1_5FE0(M539TimerObj *self, s32 offType)
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
            M539_ASSERT_INL(0, lbl_1_data_1BE, lbl_1_data_1B4, 0x48);
            return;
    }
}

void fn_1_60A4(M539TimerObj *self)
{
    MgTimerModeOffSet(self->timer);
}

// Stop the timer the first time it is asked to.
void fn_1_60D4(M539TimerObj *self)
{
    if (self->timer->stopF == 0) {
        self->timer->stopF = 1;
        self->timer->mode = 1;
    }
}

void fn_1_6100(M539TimerObj *self)
{
    MgTimerKill(self->timer);
    self->timer = MgTimerCreate(0);
}

u8 fn_1_613C(M539TimerObj *self)
{
    return MgTimerDoneCheck(self->timer) != 0;
}

void fn_1_617C(M539TimerObj *self, u8 r, u8 g, u8 b)
{
    MgTimerWinColorSet(self->timer, r, g, b);
}

// ---------------------------------------------------------------- static initialisers

void fn_1_61C4(void)
{
    lbl_1_bss_C.next = &lbl_1_bss_C;
    lbl_1_bss_C.prev = &lbl_1_bss_C;
    __register_global_object(&lbl_1_bss_C, (void *)fn_1_6324, lbl_1_bss_0);
    lbl_1_bss_18 = 0x78;
    lbl_1_bss_1C = 0xD2;
    lbl_1_bss_20 = 0xD2;
    fn_1_36CC(&lbl_1_bss_24, 0x1571);
}

void *fn_1_6324(M539Node *self, s16 flag)
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
}
