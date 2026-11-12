// m522Dll "m522": the module's runtime layer -- camera pose maths, an intrusive
// list of tasks, the game-master singleton plumbing, thin Hu3D/MgTimer wrappers,
// a Mersenne Twister, a sub-thread class and the C++ static initialisers.
#define _MATH_H
#include "REL/m522Dll.h"
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
#define M522R_RODATA __declspec(section ".rodata")
#else
#define M522R_RODATA
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

#define M522R_ASSERT(cond, expr, file, line) ((cond) ? (void)0 : fn_1_6A70((expr), (file), (line)))

// The inlined form of the assertion handler (singleton.h, model.cpp, ...).
#define M522R_ASSERT_INL(cond, expr, file, line)                                      if (cond) {                                                                          (void)0;                                                                     } else {                                                                             char assertBuf[0x400];                                                           sprintf(assertBuf, lbl_1_data_1A4, (file), (line), (expr));                       OSPanic(lbl_1_data_1C2, 0x12, assertBuf);                                     }

// ---------------------------------------------------------------- types

typedef struct M522RNode_s {
    struct M522RNode_s *next;
    struct M522RNode_s *prev;
} M522RNode;

// Camera pose: eye position, up vector, look-at target.
typedef struct M522RCam_s {
    Vec pos;
    Vec up;
    Vec target;
} M522RCam;

// Stored pose: position plus orientation quaternion.
typedef struct M522RPose_s {
    Vec pos;
    Quaternion rot;
} M522RPose;

// A pose viewed as the seven raw words the pose-table dump prints.
typedef union M522RPoseBits_u {
    M522RPose pose;
    u32 w[7];
} M522RPoseBits;

// Task registered on the module-wide list; its vtable lives at +8.
struct M522RTask_s;
typedef struct M522RTaskVtbl_s {
    void *rtti[2];
    struct M522RTask_s *(*dtor)(struct M522RTask_s *self, s16 flag);
    void (*onFlag)(struct M522RTask_s *self, s32 bit);
    void (*update)(struct M522RTask_s *self);
    void *pad;
} M522RTaskVtbl;

typedef struct M522RTask_s {
    M522RNode link;
    M522RTaskVtbl *vtbl;
    s32 flags;
} M522RTask;

// Singleton game master (only the base part that m539 touches).
struct M522RMaster_s;
typedef struct M522RMasterVtbl_s {
    void *rtti[2];
    struct M522RMaster_s *(*dtor)(struct M522RMaster_s *self, s16 flag);
    void (*setup)(struct M522RMaster_s *self, OMOBJMAN *objman);
    void (*update)(struct M522RMaster_s *self);
    void (*finish)(struct M522RMaster_s *self);
} M522RMasterVtbl;

typedef struct M522RMaster_s {
    s32 pad0;
    M522RMasterVtbl *vtbl;
    s32 slot[4];
    s32 mesId;
    s32 counter;
} M522RMaster;

// Sub-thread: vtable, OSThread, stack pointer, semaphore, argument.
struct M522RSubThread_s;
typedef struct M522RThreadVtbl_s {
    void *rtti[2];
    struct M522RSubThread_s *(*dtor)(struct M522RSubThread_s *self, s16 flag);
    s32 (*run)(struct M522RSubThread_s *self, s32 arg);
} M522RThreadVtbl;

typedef struct M522RThread_s {
    OSThread thread;
    u32 *stack;
} M522RThread;

typedef struct M522RSem_s {
    s32 count;
    OSThreadQueue queue;
} M522RSem;

typedef struct M522RSubThread_s {
    M522RThreadVtbl *vtbl;
    M522RThread th;
    M522RSem sem;
    s32 arg;
} M522RSubThread;

// Mersenne Twister state.
typedef struct M522RRand_s {
    u32 mt[0x270];
    u32 idx;
} M522RRand;

// Functor kept in an object's data slot: slot +0xC runs it.
struct M522RFunctor_s;
typedef struct M522RFunctorVtbl_s {
    void *rtti[2];
    struct M522RFunctor_s *(*dtor)(struct M522RFunctor_s *self, s16 flag);
    void (*run)(struct M522RFunctor_s *self, OMOBJ *obj);
} M522RFunctorVtbl;

typedef struct M522RFunctor_s {
    M522RFunctorVtbl *vtbl;
} M522RFunctor;

// A wrapper that owns a Hu3D model / MgTimer handle.
typedef struct M522RModel_s {
    HU3DMODELID id;
} M522RModel;

typedef struct M522RTimerObj_s {
    MGTIMER *timer;
} M522RTimerObj;

// ---------------------------------------------------------------- data

char lbl_1_data_190[] = "ptr != NULL";
char lbl_1_data_19C[] = "new.cpp";
char lbl_1_data_1A4[] = "\nASSERTION FAILED at %s:%d\n%s";
char lbl_1_data_1C2[] = "assert.cpp";
char lbl_1_data_1CD[] = "{{0x%08x,0x%08x,0x%08x},{0x%08x,0x%08x,0x%08x,0x%08x}},\n";
char lbl_1_data_206[] = "handle.cpp";
char lbl_1_data_211[] = "n <= MAXCOUNT";
char lbl_1_data_21F[] = "singleton.h";
char lbl_1_data_22B[] = "ms_pointer == 0";
char lbl_1_data_23B[] = "ms_pointer != 0";
char lbl_1_data_24B[] = "model.cpp";
char lbl_1_data_255[] = "p != NULL";
char lbl_1_data_25F[] = "om.cpp";
char lbl_1_data_266[] = "subthread.cpp";
char lbl_1_data_274[] = "::OSGetCurrentThread() == m_th";
char lbl_1_data_293[] = "!::OSIsThreadTerminated(m_th)";
char lbl_1_data_2B1[] = "thread.cpp";
char lbl_1_data_2BC[] = "priority >= OS_PRIORITY_MIN && priority <= OS_PRIORITY_MAX";
char lbl_1_data_2F7[] = "OSCreateThread() failed.";
char lbl_1_data_310[] = "delete CThread@%p\nstack remain/total = 0x%08x/0x%08x (%f%%)\n";
char lbl_1_data_34D[] = "stack overflow\n";
char lbl_1_data_35D[] = "timer.cpp";
char lbl_1_data_367[] = "false";

M522RSubThread *fn_1_BE5C(M522RSubThread *self, s16 flag);
M522RTask *fn_1_8C48(M522RTask *self, s16 flag);
void fn_1_8B7C(M522RTask *self);
M522RMaster *fn_1_8E6C(M522RMaster *self, s16 flag);
void fn_1_920C(M522RMaster *self);
M522RFunctor *fn_1_B6D4(M522RFunctor *self, s16 flag);
void fn_1_B660(M522RFunctor *self, OMOBJ *obj);
void fn_1_B614(OMOBJ *obj);

M522RThreadVtbl lbl_1_data_370 = { { 0, 0 }, fn_1_BE5C, 0 };
M522RFunctorVtbl lbl_1_data_380 = { { 0, 0 }, fn_1_B6D4, fn_1_B660 };
M522RMasterVtbl lbl_1_data_390 = { { 0, 0 }, fn_1_8E6C, 0, fn_1_920C, 0 };
M522RTaskVtbl lbl_1_data_3A8 = { { 0, 0 }, fn_1_8C48, 0, fn_1_8B7C };

// ---------------------------------------------------------------- bss

u8 lbl_1_bss_20[0xC];
M522RNode lbl_1_bss_2C;
s32 lbl_1_bss_34;
s32 lbl_1_bss_38;
s32 lbl_1_bss_3C;
s32 lbl_1_bss_40;
M522RRand lbl_1_bss_44;
u8 lbl_1_bss_A08;
M522RMaster *lbl_1_bss_A0C;

// ---------------------------------------------------------------- code

void fn_1_6A70(const char *expr, const char *file, s32 line);
int fn_1_6AD0(void *unused, const char *fmt, ...);
void *fn_1_CB78(M522RNode *self, s16 flag);
void fn_1_6CA4(M522RCam *self, Vec *x, Vec *y, Vec *z);
void fn_1_6E08(M522RCam *self, Vec *delta);

void *fn_1_689C(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M522R_ASSERT(p != 0, lbl_1_data_190, lbl_1_data_19C, 0x17);
    return p;
}

void *fn_1_6900(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M522R_ASSERT(p != 0, lbl_1_data_190, lbl_1_data_19C, 0x17);
    return p;
}

void fn_1_6964(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_699C(void *ptr)
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

void fn_1_6A70(const char *expr, const char *file, s32 line)
{
    char buf[0x40C];

    sprintf(buf, lbl_1_data_1A4, file, line, expr);
    OSPanic(lbl_1_data_1C2, 0x12, buf);
}

int fn_1_6AD0(void *unused, const char *fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = vprintf(fmt, ap);
    va_end(ap);
    return ret;
}

// Orbit: rotate about the camera X axis (pitch) then world Y (yaw).
void fn_1_6B74(M522RCam *self, f32 pitch, f32 yaw)
{
    Vec ax, ay, az;
    Mtx rotA, rotB, rot;

    fn_1_6CA4(self, &ax, &ay, &az);
    PSMTXRotAxisRad(rotA, &ax, 0.017453292f * pitch);
    PSMTXRotRad(rotB, 'Y', 0.017453292f * yaw);
    PSMTXConcat(rotA, rotB, rot);
    PSMTXMultVecSR(rot, &az, &az);
    PSMTXMultVecSR(rot, &ay, &self->up);
    PSVECSubtract(&self->pos, &az, &self->target);
}

void fn_1_6C38(M522RCam *self, f32 angle)
{
    Mtx m;
    Vec axis;

    PSVECSubtract(&self->target, &self->pos, &axis);
    PSMTXRotAxisRad(m, &axis, 0.017453292f * angle);
    PSMTXMultVec(m, &self->up, &self->up);
}

// Camera basis: the three rows of the look-at matrix.
void fn_1_6CA4(M522RCam *self, Vec *x, Vec *y, Vec *z)
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

void fn_1_6D4C(M522RCam *self, Mtx out)
{
    C_MTXLookAt(out, &self->pos, &self->up, &self->target);
    PSMTXInverse(out, out);
}

void fn_1_6DA0(M522RCam *self, Vec *local)
{
    Mtx m;
    Vec world;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    PSMTXMultVecSR(m, local, &world);
    fn_1_6E08(self, &world);
}

void fn_1_6E08(M522RCam *self, Vec *delta)
{
    PSVECAdd(delta, &self->target, &self->target);
    PSVECAdd(delta, &self->pos, &self->pos);
}

void fn_1_6E5C(M522RCam *self)
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

void fn_1_6EF0(M522RCam *self, M522RPose *pose)
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

void fn_1_6F90(M522RCam *self, M522RPose *out)
{
    Mtx m;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    out->pos.x = self->pos.x;
    out->pos.y = self->pos.y;
    out->pos.z = self->pos.z;
    C_QUATMtx(&out->rot, m);
}

void fn_1_7008(M522RCam *self, M522RPose *a, M522RPose *b, f32 t)
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

void fn_1_7118(M522RCam *self, M522RCam *src)
{
    Vec delta;

    PSVECSubtract(&src->pos, &self->pos, &delta);
    self->pos.x = src->pos.x;
    self->pos.y = src->pos.y;
    self->pos.z = src->pos.z;
    PSVECAdd(&self->target, &delta, &self->target);
}

void fn_1_7184(M522RCam *self, Vec *a, Vec *b, f32 t)
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

void fn_1_723C(M522RCam *self, u16 camBit)
{
    Hu3DCameraPosSetV(camBit, &self->pos, &self->up, &self->target);
}

void fn_1_727C(M522RCam *self, u16 camBit)
{
    Hu3DCameraPosGet(camBit, &self->pos, &self->up, &self->target);
}

// Dump the current camera as a pose table row (floats printed as raw words).
void fn_1_72BC(M522RCam *self)
{
    Mtx m;
    M522RPoseBits out;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    out.pose.pos.x = self->pos.x;
    out.pose.pos.y = self->pos.y;
    out.pose.pos.z = self->pos.z;
    C_QUATMtx(&out.pose.rot, m);
    printf(lbl_1_data_1CD, out.w[0], out.w[1], out.w[2], out.w[3], out.w[4], out.w[5], out.w[6]);
}

f32 fn_1_7354(s32 pad)
{
    return 0.017857144f * HuPadStkX[pad];
}

f32 fn_1_73A8(s32 pad)
{
    return 0.017857144f * HuPadStkY[pad];
}

f32 fn_1_73FC(s32 pad)
{
    return 0.016949153f * HuPadSubStkX[pad];
}

f32 fn_1_7450(s32 pad)
{
    return 0.016949153f * HuPadSubStkY[pad];
}

// Free-fly debug camera driven by pad `pad`.
void fn_1_74A4(M522RCam *self, s32 pad)
{
    Mtx inv, rot, rotB, rotA, m, dump;
    M522RPoseBits out;
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
    yaw = -4.0f * fn_1_7354(pad);
    pitch = -4.0f * fn_1_73A8(pad);
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
    speed = (u8)fast ? 70.0f : 20.0f;
    C_MTXLookAt(inv, &self->pos, &self->up, &self->target);
    PSMTXInverse(inv, inv);
    mv.x = (speed * (u8)((btn & 2) != 0)) - (speed * (u8)((btn & 1) != 0));
    mv.y = (speed * (u8)((btn & 8) != 0)) - (speed * (u8)((btn & 4) != 0));
    mv.x = mv.x + (speed * fn_1_73FC(pad));
    mv.z = 0.0f;
    PSMTXMultVecSR(inv, &mv, &mv);
    mv.y = mv.y + (speed * fn_1_7450(pad));
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
        printf(lbl_1_data_1CD, out.w[0], out.w[1], out.w[2], out.w[3], out.w[4], out.w[5], out.w[6]);
    }
}

// ---------------------------------------------------------------- intrusive list

typedef u8 (*M522RCmp)(M522RNode *a, M522RNode *b);

M522RNode *fn_1_88E8(M522RNode *list);
M522RNode *fn_1_893C(M522RCmp cmp, M522RNode *a, M522RNode *b);
M522RNode *fn_1_8240(M522RCmp cmp, M522RNode *list);
void fn_1_9110(OMOBJ *obj);
void fn_1_9210(void);
void fn_1_99B8(void);
void fn_1_B584(void *ptr);
// Handle table: the entries live in one block, the used list head is the extra
// entry at the end.  Entry 0 heads the free list.
typedef struct M522RHandleNode_s {
    struct M522RHandleNode_s *next;
    struct M522RHandleNode_s *prev;
    void *ptr;
    union {
        u32 handle;
        struct {
            u32 index : 20;
            u32 gen : 12;
        } b;
    } u;
} M522RHandleNode;

typedef struct M522RHandleMan_s {
    M522RHandleNode *nodes;
    M522RHandleNode *used;
    u32 count;
} M522RHandleMan;

typedef struct M522ListNode_s {
    struct M522ListNode_s *f00;
    struct M522ListNode_s *f04;
    u32 f08;
    u32 f0C;
    u16 f0E_hi : 4;
    u16 f0E_count : 12;
} M522ListNode;

M522_HEAPSEL *fn_1_7B00(M522RHandleMan *self, u32 n)
{
    u32 i;
    void *p;
    void *q;
    void *r;
    void *s;

    M522R_ASSERT_INL(n <= 0x1000, lbl_1_data_211, lbl_1_data_206, 0x44);
    self->count = n;
    p = HuMemDirectMallocNum(HEAP_HEAP, (n + 1) * sizeof(M522RHandleNode), 0x10000000);
    M522R_ASSERT_INL(p != 0, lbl_1_data_190, lbl_1_data_19C, 0x17);
    q = p;
    r = q;
    s = r;
    self->nodes = (M522RHandleNode *)s;
    self->used = &self->nodes[n];
    self->used->next = self->used;
    self->used->prev = self->used;
    for (i = 1; i < n; i++) {
        self->nodes[i].next = &self->nodes[i] + 1;
        self->nodes[i].prev = &self->nodes[i] - 1;
        self->nodes[i].ptr = 0;
        self->nodes[i].u.b.index = i;
        self->nodes[i].u.b.gen = 0;
    }
    self->nodes[1].prev = &self->nodes[0];
    self->nodes[n - 1].next = &self->nodes[0];
    self->nodes[0].next = &self->nodes[1];
    self->nodes[0].prev = &self->nodes[n] - 1;
    return (M522_HEAPSEL *)self;
}

M522_HEAPSEL *fn_1_7D04(M522_HEAPSEL *table, s16 flag)
{
    if (table != 0) {
        M522_HANDLEENT *entries = table->entries;
        if (entries != 0) {
            HuMemDirectFree(entries);
        }
        if (flag > 0) {
            if (table != 0) {
                HuMemDirectFree(table);
            }
        }
    }
    return table;
}

u32 fn_1_7D74(M522RHandleMan *self, void *ptr)
{
    M522RHandleNode *node;

    node = self->nodes[0].next;
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
    node->u.b.gen += 1;
    return node->u.handle;
}

void *fn_1_7E04(M522RHandleMan *self, int handle)
{
    u32 h = handle;
    M522RHandleNode *node;
    void *ptr;

    if (h == 0) {
        return 0;
    }
    if ((h >> 12) >= self->count) {
        return 0;
    }
    node = &self->nodes[h >> 12];
    if (node->u.handle != handle) {
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


void *fn_1_7EC8(M522RHandleMan *self, int handle)
{
    u32 h = handle;
    M522RHandleNode *node;

    if (h == 0) {
        return 0;
    }
    if ((h >> 12) >= self->count) {
        return 0;
    }
    node = &self->nodes[h >> 12];
    if (node->u.handle != handle) {
        return 0;
    }
    return node->ptr;
}

s32 fn_1_7F38(M522ListNode *self)
{
    s32 count = 0;
    M522ListNode *n = self->f00;
    while (n != self) {
        n = n->f00;
        count++;
    }
    return count;
}

s32 fn_1_7F74(M522ListNode *self)
{
    s32 count;
    M522ListNode *head = self->f04;
    count = 0;
    M522ListNode *n = head->f00;
    while (n != head) {
        n = n->f00;
        count++;
    }
    return count;
}

s32 fn_1_7FBC(M522ListNode *self)
{
    s32 count;
    M522ListNode *head = self->f00;
    count = 0;
    M522ListNode *n = head->f00;
    while (n != head) {
        n = n->f00;
        count++;
    }
    return count;
}



void fn_1_8004(M522RNode *self)
{
    self->next = self;
    self->prev = self;
}

void *fn_1_8010(M522RNode *self, s16 flag)
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

void fn_1_807C(M522RNode *self)
{
    self->next->prev = self->prev;
    self->prev->next = self->next;
    self->next = self;
    self->prev = self;
}

// Insert `self` right after `other`.
void fn_1_80A0(M522RNode *self, M522RNode *other)
{
    M522RNode *at = other;

    self->prev = at;
    self->next = at->next;
    at->next->prev = self;
    at->next = self;
}

// Insert `self` right before `other`.
void fn_1_80D0(M522RNode *self, M522RNode *other)
{
    M522RNode *at = other;

    self->next = at;
    self->prev = at->prev;
    at->prev->next = self;
    at->prev = self;
}

// Move every node of `self` in front of `other`.
void fn_1_8100(M522RNode *self, M522RNode *other)
{
    M522RNode *at;
    M522RNode *first;
    M522RNode *last;
    u8 empty;

    empty = 0;
    if (self->next == self && self->next == self->prev) {
        empty = 1;
    }
    if (empty == 0) {
        at = other;
        first = self->next;
        last = self->prev;
        self->next = self;
        self->prev = self;
        first->prev = at->prev;
        last->next = at;
        at->prev->next = first;
        at->prev = last;
    }
}

// Merge sort the circular list `self` with `cmp`, then relink the prev chain.
void fn_1_8188(M522RNode *self, M522RCmp cmp)
{
    M522RNode *node;
    M522RNode *prev;
    u8 empty;

    empty = 0;
    if (self->next == self && self->next == self->prev) {
        empty = 1;
    }
    if (empty == 0) {
        self->prev->next = 0;
        node = fn_1_8240(cmp, self->next);
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

M522RNode *fn_1_8240(M522RCmp cmp, M522RNode *list)
{
    M522RNode *second;
    M522RNode *a;
    M522RNode *b;

    if (list == 0) {
        return 0;
    }
    if (list->next == 0) {
        return list;
    }
    second = fn_1_88E8(list);
    a = fn_1_8240(cmp, list);
    b = fn_1_8240(cmp, second);
    return fn_1_893C(cmp, a, b);
}

// Cut the list in the middle and return the second half.
M522RNode *fn_1_88E8(M522RNode *list)
{
    M522RNode *fast;

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
    fast = list->next;
    list->next = 0;
    return fast;
}

M522RNode *fn_1_893C(M522RCmp cmp, M522RNode *a, M522RNode *b)
{
    M522RNode head;
    M522RNode *tail;

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

void fn_1_89E8(s32 bit)
{
    M522RTask *t;
    M522RNode *end;
    M522RNode *b;
    M522RNode *c;
    M522RNode *nx;

    b = lbl_1_bss_2C.next;
    c = b;
    t = (M522RTask *)c;
    end = &lbl_1_bss_2C;
    lbl_1_bss_34 = bit;
    while ((M522RNode *)t != end) {
        t->flags |= 1 << bit;
        t->vtbl->onFlag(t, bit);
        nx = t->link.next;
        t = (M522RTask *)nx;
    }
}

s32 fn_1_8A80(void)
{
    M522RTask *t;
    M522RNode *end;
    s32 bit;
    M522RNode *b;
    M522RNode *c;
    M522RNode *nx;

    b = lbl_1_bss_2C.next;
    c = b;
    t = (M522RTask *)c;
    end = &lbl_1_bss_2C;
    while ((M522RNode *)t != end) {
        bit = lbl_1_bss_34;
        if ((t->flags & (1 << bit)) != 0) {
            return 0;
        }
        nx = t->link.next;
        t = (M522RTask *)nx;
    }
    return 1;
}

void fn_1_8B0C(void)
{
    M522RTask *t;
    M522RNode *end;
    M522RNode *b;
    M522RNode *c;
    M522RNode *nx;

    b = lbl_1_bss_2C.next;
    c = b;
    t = (M522RTask *)c;
    end = &lbl_1_bss_2C;
    while ((M522RNode *)t != end) {
        t->vtbl->update(t);
        nx = t->link.next;
        t = (M522RTask *)nx;
    }
}

void fn_1_8B7C(M522RTask *self)
{
}

// Destroy every registered task, last one first.
inline u8 M522RIsEmpty(M522RNode *n)
{
    u8 empty;

    empty = 0;
    if (n->next == n && n->next == n->prev) {
        empty = 1;
    }
    return empty;
}

void fn_1_8B80(void)
{
    M522RTask *t;
    M522RNode *p;
    M522RNode *q;

    while (!M522RIsEmpty(&lbl_1_bss_2C)) {
        p = lbl_1_bss_2C.prev;
        q = p;
        t = (M522RTask *)q;
        if (t != 0) {
            t = t->vtbl->dtor(t, 1);
        }
    }
}

M522RTask *fn_1_8C48(M522RTask *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_3A8;
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

s32 fn_1_8CD0(void)
{
    return lbl_1_bss_34;
}

// ---------------------------------------------------------------- game master

// CGameMaster::GetInstance(), inlined with its assertion.
inline M522RMaster *M522RMasterGet(void)
{
    M522R_ASSERT_INL(lbl_1_bss_A0C != 0, lbl_1_data_23B, lbl_1_data_21F, 0x27);
    return lbl_1_bss_A0C;
}

inline M522RMaster *M522RMasterGet2(void)
{
    return M522RMasterGet();
}

M522RMaster *fn_1_8CE0(M522RMaster *self)
{
    s32 i;

    M522R_ASSERT_INL(lbl_1_bss_A0C == 0, lbl_1_data_22B, lbl_1_data_21F, 0x35);
    lbl_1_bss_A0C = self;
    self->vtbl = &lbl_1_data_390;
    self->counter = 0;
    i = 0;
    while (i < 4) {
        self->slot[i] = -1;
        i++;
    }
    return self;
}

M522RMaster *fn_1_8DB0(M522RMaster *self, s16 flag)
{
    if (self != 0) {
        M522R_ASSERT_INL(lbl_1_bss_A0C != 0, lbl_1_data_23B, lbl_1_data_21F, 0x3D);
        lbl_1_bss_A0C = 0;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

M522RMaster *fn_1_8E6C(M522RMaster *self, s16 flag)
{
    M522RMaster *m;
    s32 i;
    s32 p;

    if (self != 0) {
        self->vtbl = &lbl_1_data_390;
        if (mgDataTbl[GwSystem.mgNo].type != 3) {
            m = M522RMasterGet2();
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
            M522R_ASSERT_INL(lbl_1_bss_A0C != 0, lbl_1_data_23B, lbl_1_data_21F, 0x3D);
            lbl_1_bss_A0C = 0;
        }
        if (flag > 0) {
            fn_1_B584(self);
        }
    }
    return self;
}

void fn_1_9024(M522RMaster *self, s32 objMax, s32 prio)
{
    OMOBJMAN *om;

    om = omInitObjMan((s16)objMax, prio);
    omGameSysInit(om);
    self->vtbl->setup(self, om);
    omAddObjEx(om, 0, 0, 0, -1, fn_1_9110);
}

void fn_1_90A8(M522RMaster *self)
{
    OMOBJMAN *om;

    om = MgActorObjectSetup();
    self->vtbl->setup(self, om);
    omAddObjEx(om, 0, 0, 0, -1, fn_1_9110);
}

void fn_1_91A0(M522RMaster *self);

void fn_1_9110(OMOBJ *obj)
{
    OMOBJ *self;
    M522RMaster *m;
    M522RMaster *cur;

    self = obj;
    if (omSysExitReq != 0 && WipeCheckEnd() == 0) {
        fn_1_99B8();
        HuAudFadeOut(0x3E8);
        self->objFunc = (OMOBJFUNC)fn_1_9210;
        return;
    }
    m = lbl_1_bss_A0C;
    cur = m;
    fn_1_91A0(cur);
}

void fn_1_91A0(M522RMaster *self)
{
    self->vtbl->update(self);
    if (self->counter > 0) {
        if (--self->counter == 0) {
        }
    }
}

void fn_1_920C(M522RMaster *self)
{
}

void fn_1_9210(void)
{
    M522RMaster *m;
    M522RMaster *cur;
    M522RMaster *obj;
    s32 c;
    u8 hitZero;

    m = lbl_1_bss_A0C;
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
void fn_1_92FC(s32 player)
{
    M522RMaster *m;
    s32 *slots;
    s32 i;

    m = M522RMasterGet2();
    slots = m->slot;
    i = 0;
    while (i < 4) {
        if (slots[i] < 0) {
            slots[i] = player;
            return;
        }
        i++;
    }
}

s32 fn_1_93D0(s32 player)
{
    s32 i;
    s32 *slots;
    M522RMaster *m;

    m = M522RMasterGet2();
    slots = m->slot;
    i = 0;
    while (i < 4) {
        if (player == slots[i]) {
            return 1;
        }
        i++;
    }
    return 0;
}

void fn_1_94A4(void)
{
    M522RMaster *m;

    m = M522RMasterGet2();
    m->mesId = GameMesCreate(2, 0);
}

void fn_1_955C(void)
{
    M522RMaster *m;

    m = M522RMasterGet2();
    m->mesId = GameMesCreate(2, 1);
}

s32 fn_1_9614(void)
{
    M522RMaster *m;
    s32 ret;

    m = M522RMasterGet2();
    ret = 0;
    if (GameMesStatGet(-1) == 0 && (u8)(m->counter <= 0) != 0) {
        ret = 1;
    }
    return ret;
}

// Announce the result: sort the registered players and print their characters.
void fn_1_96E8(void)
{
    M522RMaster *m;
    s32 *slots;
    s32 chars[4];
    s32 n;
    s32 i;
    s32 j;
    s32 k;
    s32 tmp;
    s32 charNo;

    m = M522RMasterGet2();
    m->counter = lbl_1_bss_3C;
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

void fn_1_98B4(s32 mes)
{
    M522RMaster *m;

    if (_CheckFlag(0x1000F) == 0) {
        m = M522RMasterGet2();
        m->mesId = GameMesCreate(0xE, mes);
        HuAudJinglePlay(0x86);
    }
}

void fn_1_998C(void)
{
    WipeCreate(1, 5, 0x3C);
}

void fn_1_99B8(void)
{
    WipeCreate(2, 6, 0x3C);
}

void fn_1_99E4(void)
{
    omSysExitReq = 1;
}

// ---------------------------------------------------------------- Hu3D model wrappers

M522RModel *fn_1_99F8(M522RModel *self, void *data)
{
    self->id = Hu3DModelCreate(data);
    return self;
}

M522RModel *fn_1_9A34(M522RModel *self, s32 dataNum)
{
    self->id = Hu3DModelCreateData(dataNum);
    return self;
}

M522RModel *fn_1_9A7C(M522RModel *self, HU3DMODELHOOK hook)
{
    self->id = Hu3DHookFuncCreate(hook);
    return self;
}

// Build a character model; `type` selects the model variant bit.
M522RModel *fn_1_9AB8(M522RModel *self, s32 charNo, s32 type)
{
    s32 variant;

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

inline s16 M522RModelIdGet(M522RModel *m)
{
    return m->id;
}

M522RModel *fn_1_9B54(M522RModel *self, M522RModel *parent)
{
    self->id = Hu3DModelLink(M522RModelIdGet(parent));
    return self;
}

void fn_1_9BA8(M522RModel *self, u32 attr)
{
    Hu3DModelAttrSet(self->id, attr);
}

void fn_1_9BE0(M522RModel *self, u32 attr)
{
    Hu3DModelAttrReset(self->id, attr);
}

// Apply an object's transform (translation, rotation, scale) to the model.
void fn_1_9C18(M522RModel *self, OMOBJ *obj)
{
    HU3DMODELID id;

    id = self->id;
    Hu3DModelPosSet(id, obj->trans.x, obj->trans.y, obj->trans.z);
    Hu3DModelRotSet(id, obj->rot.x, obj->rot.y, obj->rot.z);
    Hu3DModelScaleSet(id, obj->scale.x, obj->scale.y, obj->scale.z);
}

void fn_1_9C94(M522RModel *self, Vec *pos)
{
    Hu3DModelPosSet(self->id, pos->x, pos->y, pos->z);
}

void fn_1_9CDC(M522RModel *self, Vec *rot)
{
    Hu3DModelRotSet(self->id, rot->x, rot->y, rot->z);
}

void fn_1_9D24(M522RModel *self, Vec *scale)
{
    Hu3DModelScaleSet(self->id, scale->x, scale->y, scale->z);
}

void fn_1_9D6C(M522RModel *self, f32 scale)
{
    Hu3DModelScaleSet(self->id, scale, scale, scale);
}

void fn_1_9DB4(M522RModel *self, f32 tpLvl)
{
    Hu3DModelTPLvlSet(self->id, tpLvl);
}

void fn_1_9DEC(M522RModel *self, s16 layer)
{
    Hu3DModelLayerSet(self->id, layer);
}

void fn_1_9E24(M522RModel *self, u16 camBit)
{
    Hu3DModelCameraSet(self->id, camBit);
}

void fn_1_9E5C(M522RModel *self, void *data)
{
    Hu3DData[self->id].hookData = data;
}

MtxPtr fn_1_9E78(M522RModel *self)
{
    return Hu3DData[self->id].mtx;
}

M522RModel *fn_1_9E94(M522RModel *self, s16 flag)
{
    if (self != 0) {
        Hu3DModelKill(self->id);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

inline HSFOBJECT *M522RObjGet(M522RModel *self, char *name)
{
    HSFOBJECT *obj;

    obj = Hu3DModelObjPtrGet(self->id, name);
    M522R_ASSERT_INL(obj != 0, lbl_1_data_255, lbl_1_data_24B, 0x8A);
    return obj;
}

HSFOBJECT *fn_1_9EF0(M522RModel *self, char *name)
{
    return M522RObjGet(self, name);
}

void fn_1_9F84(M522RModel *self)
{
    Hu3DModelShadowSet(self->id);
}

void fn_1_9FB4(M522RModel *self)
{
    Hu3DModelShadowMapSet(self->id);
}

void fn_1_9FE4(M522RModel *self, char *name)
{
    Hu3DModelShadowMapObjSet(self->id, name);
}

void fn_1_A01C(M522RModel *self)
{
    Hu3DModelShadowMapObjReset(self->id);
}

void fn_1_A04C(M522RModel *self, char *name)
{
    HSFOBJECT *obj;
    u32 *flags;

    obj = fn_1_9EF0(self, name);
    flags = (u32 *)obj->constData;
    *flags &= ~8;
}

void fn_1_A0F8(M522RModel *self, char *name)
{
    HSFOBJECT *obj;

    obj = fn_1_9EF0(self, name);
    obj->flags |= 0x400;
}

void fn_1_A1A0(M522RModel *self, char *name)
{
    HSFOBJECT *obj;

    obj = fn_1_9EF0(self, name);
    obj->flags &= ~0x400;
}

void fn_1_A248(M522RModel *self, char *name)
{
    HSFOBJECT *obj;
    HSFTRANSFORM *base;

    obj = fn_1_9EF0(self, name);
    obj->flags |= 0x400;
    base = &obj->mesh.base;
    base->scale.x = 0.0f;
    base->scale.y = 0.0f;
    base->scale.z = 0.0f;
}

void fn_1_A324(M522RModel *self, char *name)
{
    HSFOBJECT *obj;
    HSFTRANSFORM *base;

    obj = fn_1_9EF0(self, name);
    obj->flags &= ~0x400;
    base = &obj->mesh.base;
    base->scale.x = 1.0f;
    base->scale.y = 1.0f;
    base->scale.z = 1.0f;
}

void fn_1_A400(M522RModel *self, s32 time)
{
    Hu3DMotionTimeSet(M522RModelIdGet(self), (f32)time);
}

// ---------------------------------------------------------------- Mersenne Twister

M522R_RODATA extern const u32 lbl_1_rodata_360[2] = { 0, 0x9908B0DF };

void fn_1_A46C(M522RRand *r, u32 seed)
{
    r->mt[0] = seed;
    r->idx = 1;
    while (r->idx < 0x270) {
        r->mt[r->idx] = r->idx + (((r->mt[r->idx - 1]) ^ ((r->mt[r->idx - 1]) >> 30)) * 0x6C078965);
        r->idx++;
    }
}

void fn_1_A4E0(M522RRand *r, u32 *key, u32 len)
{
    u32 i;
    u32 j;
    u32 k;

    fn_1_A46C(r, 19650218);
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

u32 fn_1_A6A4(M522RRand *r)
{
    u32 k;
    u32 y;

    if (r->idx >= 0x270) {
        k = 0;
        while (k < 0xE3) {
            y = (r->mt[k] & 0x80000000) | (r->mt[k + 1] & 0x7FFFFFFF);
            r->mt[k] = lbl_1_rodata_360[y & 1] ^ ((y >> 1) ^ r->mt[k + 0x18D]);
            k++;
        }
        while (k < 0x26F) {
            y = (r->mt[k] & 0x80000000) | (r->mt[k + 1] & 0x7FFFFFFF);
            r->mt[k] = lbl_1_rodata_360[y & 1] ^ ((y >> 1) ^ r->mt[k - 0xE3]);
            k++;
        }
        y = (r->mt[0x26F] & 0x80000000) | (r->mt[0] & 0x7FFFFFFF);
        r->mt[0x26F] = (y >> 1) ^ r->mt[0x18C] ^ lbl_1_rodata_360[y & 1];
        r->idx = 0;
    }
    y = r->mt[r->idx++];
    y ^= y >> 11;
    y ^= (y << 7) & 0x9D2C5680;
    y ^= (y << 15) & 0xEFC60000;
    y ^= (y >> 18);
    return y;
}

u32 fn_1_A830(M522RRand *r)
{
    return fn_1_A6A4(r) >> 1;
}

f32 fn_1_A9C8(M522RRand *r)
{
    return 2.3283064e-10f * (f32)fn_1_A6A4(r);
}

f32 fn_1_AB8C(M522RRand *r)
{
    return 2.3283064e-10f * (f32)fn_1_A6A4(r);
}

f32 fn_1_AD50(M522RRand *r)
{
    return 2.3283064e-10f * (0.5f + (f32)fn_1_A6A4(r));
}

f64 fn_1_AF24(M522RRand *r)
{
    u32 a;
    u32 b;

    a = fn_1_A6A4(r) >> 5;
    b = fn_1_A6A4(r) >> 6;
    return 1.1102230246251565e-16 * ((67108864.0 * (f64)a) + (f64)b);
}

f32 fn_1_B29C(M522RRand *r)
{
    u32 v;

    v = fn_1_A6A4(r) >> 9;
    return 1.1920929e-7f * (f32)v;
}

// ---------------------------------------------------------------- allocation / functor objects

void *fn_1_B46C(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M522R_ASSERT_INL(p != 0, lbl_1_data_190, lbl_1_data_25F, 0x18);
    return p;
}

void *fn_1_B4F8(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M522R_ASSERT_INL(p != 0, lbl_1_data_190, lbl_1_data_25F, 0x1F);
    return p;
}

void fn_1_B584(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_B5BC(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

// Functor owned by an object: the object's data slot points at it.
void fn_1_B5F4(M522RFunctor *self, OMOBJ *obj)
{
    self->vtbl = &lbl_1_data_380;
    obj->data = self;
    obj->objFunc = fn_1_B614;
}

void fn_1_B614(OMOBJ *obj)
{
    M522RFunctor *self;

    self = (M522RFunctor *)obj->data;
    self->vtbl->run(self, obj);
}

void fn_1_B660(M522RFunctor *self, OMOBJ *obj)
{
}

void fn_1_B664(OMOBJ *obj)
{
    M522RFunctor *fn;
    M522RFunctor *p;

    fn = (M522RFunctor *)obj->data;
    p = fn;
    if (p != 0) {
        p = p->vtbl->dtor(p, 1);
    }
    omDelObjEx(HuPrcCurrentGet(), obj);
}

M522RFunctor *fn_1_B6D4(M522RFunctor *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_380;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_B734(OMOBJMAN *man, OMOBJ *obj)
{
    M522RFunctor *fn;
    M522RFunctor *p;

    fn = (M522RFunctor *)obj->data;
    p = fn;
    if (p != 0) {
        p = p->vtbl->dtor(p, 1);
    }
    omDelObjEx(man, obj);
}

// ---------------------------------------------------------------- global random number generator

void fn_1_B7A8(u32 seed)
{
    fn_1_A46C(&lbl_1_bss_44, seed);
}

u32 fn_1_B880(void)
{
    return fn_1_A6A4(&lbl_1_bss_44) >> 1;
}

f32 fn_1_BABC(void)
{
    u32 v;

    v = fn_1_A6A4(&lbl_1_bss_44) >> 9;
    return 1.1920929e-7f * (f32)v;
}

f32 fn_1_BD30(f32 scale)
{
    return scale * fn_1_AF24(&lbl_1_bss_44);
}

f32 fn_1_BD68(f32 from, f32 to)
{
    return from + (to - from) * fn_1_AF24(&lbl_1_bss_44);
}

// Sphere: centre and radius.
typedef struct M522RSphere_s {
    Vec pos;
    f32 radius;
} M522RSphere;

inline f32 M522RSphereRadius(M522RSphere *s)
{
    return s->radius;
}

// ---------------------------------------------------------------- threads

s32 fn_1_BDBC(void)
{
    return OSGetThreadPriority(OSGetCurrentThread());
}

void *fn_1_C0A4(void *arg);
M522RThread *fn_1_C100(M522RThread *th, void *(*func)(void *), void *arg, u32 size, s32 prio, u16 attr);

M522RSubThread *fn_1_BDE0(M522RSubThread *self, u32 stackSize)
{
    self->vtbl = &lbl_1_data_370;
    fn_1_C100(&self->th, fn_1_C0A4, self, stackSize, OSGetThreadPriority(OSGetCurrentThread()), 1);
    OSInitSemaphore(&self->sem, 0);
    return self;
}

M522RThread *fn_1_C384(M522RThread *th, s16 flag);

M522RSubThread *fn_1_BE5C(M522RSubThread *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_370;
        fn_1_C384(&self->th, -1);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

// CThread::IsTerminated(), inlined with its own `this`.
inline u8 M522RThreadDone(M522RThread *th)
{
    return OSIsThreadTerminated(&th->thread) != 0;
}

u8 fn_1_BEC8(M522RSubThread *self)
{
    M522RThread *th = &self->th;

    return OSIsThreadTerminated(&th->thread) != 0;
}

u32 fn_1_C4B8(M522RThread *th);

s32 fn_1_BF10(M522RSubThread *self, s32 arg)
{
    M522RThread *thIsDone;
    M522RThread *thResume;
    M522RSem *sem;

    thIsDone = &self->th;
    if (OSIsThreadTerminated(&thIsDone->thread) != 0) {
        return self->arg;
    }
    self->arg = arg;
    thResume = &self->th;
    OSResumeThread(&thResume->thread);
    sem = &self->sem;
    OSWaitSemaphore(sem);
    fn_1_C4B8(&self->th);
    OSCheckActiveThreads();
    return self->arg;
}

s32 fn_1_BFA0(M522RSubThread *self, s32 arg)
{
    M522RThread *thCur;
    M522RThread *thDone;
    M522RSem *sem;
    M522RThread *thSuspend;

    thCur = &self->th;
    M522R_ASSERT_INL(OSGetCurrentThread() == &thCur->thread, lbl_1_data_274, lbl_1_data_266, 0x47);
    thDone = &self->th;
    M522R_ASSERT_INL(OSIsThreadTerminated(&thDone->thread) == 0, lbl_1_data_293, lbl_1_data_266, 0x4A);
    self->arg = arg;
    sem = &self->sem;
    OSSignalSemaphore(sem);
    thSuspend = &self->th;
    OSSuspendThread(&thSuspend->thread);
    return self->arg;
}

void *fn_1_C0A4(void *arg)
{
    M522RSubThread *self;
    M522RSem *sem;

    self = (M522RSubThread *)arg;
    self->vtbl->run(self, self->arg);
    sem = &self->sem;
    OSSignalSemaphore(sem);
    return 0;
}

void fn_1_C260(M522RThread *th, void *(*func)(void *), void *arg, u32 *stack, u32 words, s32 prio, u16 attr);

M522RThread *fn_1_C100(M522RThread *th, void *(*func)(void *), void *arg, u32 size, s32 prio, u16 attr)
{
    u32 words;
    u32 *stack;

    words = size >> 2;
    stack = (u32 *)HuMemDirectMallocNum(HEAP_HEAP, words * 4, 0x10000000);
    M522R_ASSERT_INL(stack != 0, lbl_1_data_190, lbl_1_data_19C, 0x17);
    th->stack = stack;
    fn_1_C260(th, func, arg, stack, words, prio, attr);
    return th;
}

M522RThread *fn_1_C1E4(M522RThread *th, void *(*func)(void *), void *arg, u32 *stack, u32 size, s32 prio, u16 attr)
{
    u32 words;

    th->stack = 0;
    words = size >> 2;
    fn_1_C260(th, func, arg, stack, words, prio, attr);
    return th;
}

void fn_1_C5CC(u32 *p, u32 words);
s32 fn_1_C558(u32 *p, s32 mode);

void fn_1_C260(M522RThread *th, void *(*func)(void *), void *arg, u32 *stack, u32 words, s32 prio, u16 attr)
{
    u8 ok;

    ok = 0;
    if (prio >= 0 && prio <= 0x1F) {
        ok = 1;
    }
    M522R_ASSERT_INL(ok != 0, lbl_1_data_2BC, lbl_1_data_2B1, 0x52);
    fn_1_C5CC(stack, words);
    fn_1_C558(stack, 0);
    if ((u8)(OSCreateThread(&th->thread, func, arg, &stack[words], words * 4, prio, attr) != 0) == 0) {
        OSPanic(lbl_1_data_2B1, 0x60, lbl_1_data_2F7);
    }
}

M522RThread *fn_1_C384(M522RThread *th, s16 flag)
{
    u32 total;
    u32 remain;
    u32 *stack;

    if (th != 0) {
        if (OSIsThreadTerminated(&th->thread) == 0) {
            OSCancelThread(&th->thread);
        }
        if (lbl_1_bss_A08 != 0) {
            total = (uintptr_t)th->thread.stackBase - (uintptr_t)th->thread.stackEnd;
            remain = fn_1_C4B8(th);
            printf(lbl_1_data_310, th, remain, total, (100.0 * (f64)remain) / (f64)total);
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

u32 fn_1_C4B8(M522RThread *th)
{
    u32 *base;
    u32 *p;
    u32 *end;

    base = th->thread.stackEnd;
    if (fn_1_C558(base, 1) != 0) {
        OSPanic(lbl_1_data_2B1, 0x7F, lbl_1_data_34D);
    }
    p = base;
    end = (u32 *)th->thread.stackBase;
    while (p < end) {
        if (*p == 0xDEADBABE) {
            p++;
            continue;
        }
        break;
    }
    return (u32)((uintptr_t)p - (uintptr_t)base);
}

// Stack guard: mode 0 fills the 32 byte guard, mode 1 checks it.
s32 fn_1_C558(u32 *p, s32 mode)
{
    u32 *end;

    end = p + 8;
    switch (mode) {
        case 0:
            do {
                *p = 0xDEADBABE;
                p++;
            } while (p != end);
            break;
        case 1:
            do {
                if (*p++ != 0xDEADBABE) {
                    return 1;
                }
            } while (p != end);
            break;
    }
    return 0;
}

void fn_1_C5CC(u32 *p, u32 words)
{
    u32 *end;

    if (lbl_1_bss_A08 != 0) {
        end = p + words;
        while (p != end) {
            *p = 0xDEADBABE;
            p++;
        }
    }
}

// ---------------------------------------------------------------- MgTimer wrappers

M522RTimerObj *fn_1_C618(M522RTimerObj *self, s32 type)
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
            M522R_ASSERT_INL(0, lbl_1_data_367, lbl_1_data_35D, 0x1E);
            break;
    }
    return self;
}

M522RTimerObj *fn_1_C6E0(M522RTimerObj *self, s16 flag)
{
    if (self != 0) {
        MgTimerKill(self->timer);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_C73C(M522RTimerObj *self, s32 maxTime, s32 endTime, s32 recordTime)
{
    MgTimerParamSet(self->timer, maxTime, endTime, recordTime);
}

void fn_1_C784(M522RTimerObj *self)
{
    MgTimerValueGet(self->timer);
}

void fn_1_C7B4(M522RTimerObj *self, f32 x, f32 y)
{
    MgTimerPosSet(self->timer, x, y);
}

void fn_1_C7F4(M522RTimerObj *self, f32 *x, f32 *y)
{
    MgTimerPosGet(self->timer, x, y);
}

void fn_1_C834(M522RTimerObj *self, s32 offType)
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
            M522R_ASSERT_INL(0, lbl_1_data_367, lbl_1_data_35D, 0x48);
            return;
    }
}

void fn_1_C8F8(M522RTimerObj *self)
{
    MgTimerModeOffSet(self->timer);
}

// Stop the timer the first time it is asked to.
void fn_1_C928(M522RTimerObj *self)
{
    if (self->timer->stopF == 0) {
        self->timer->stopF = 1;
        self->timer->mode = 1;
    }
}

void fn_1_C954(M522RTimerObj *self)
{
    MgTimerKill(self->timer);
    self->timer = MgTimerCreate(0);
}

u8 fn_1_C990(M522RTimerObj *self)
{
    return MgTimerDoneCheck(self->timer) != 0;
}

void fn_1_C9D0(M522RTimerObj *self, u8 r, u8 g, u8 b)
{
    MgTimerWinColorSet(self->timer, r, g, b);
}

// ---------------------------------------------------------------- static initialisers

void fn_1_CA18(void)
{
    lbl_1_bss_2C.next = &lbl_1_bss_2C;
    lbl_1_bss_2C.prev = &lbl_1_bss_2C;
#ifdef __MWERKS__
    __register_global_object(&lbl_1_bss_2C, (void *)fn_1_CB78, lbl_1_bss_20);
#endif
    lbl_1_bss_38 = 0x78;
    lbl_1_bss_3C = 0xD2;
    lbl_1_bss_40 = 0xD2;
    fn_1_A46C(&lbl_1_bss_44, 0x1571);
}

void *fn_1_CB78(M522RNode *self, s16 flag)
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
__declspec(section ".ctors") void (*const lbl_ctor_CA18)(void) = fn_1_CA18;
#else
struct M522RStaticInit {
    M522RStaticInit() { fn_1_CA18(); }
};
static M522RStaticInit lbl_static_init;
#endif
}

