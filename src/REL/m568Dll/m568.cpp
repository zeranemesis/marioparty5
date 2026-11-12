// m568Dll "m568": the module's runtime layer -- camera pose maths, an intrusive
// list of tasks, the game-master singleton plumbing, thin Hu3D/MgTimer wrappers,
// a Mersenne Twister, a sub-thread class and the C++ static initialisers.
#define _MATH_H
#include "REL/m568Dll.h"
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
#define M568_RODATA __declspec(section ".rodata")
#else
#define M568_RODATA
#endif

extern "C" {

void OSInitSemaphore(void *sem, s32 count);
s32 OSWaitSemaphore(void *sem);
s32 OSSignalSemaphore(void *sem);
double fabs(double);
void __register_global_object(void *obj, void *dtor, void *reg);
void ObjectSetup(void);
extern void (*_ctors[])(void);
extern void (*_dtors[])(void);

#define M568_ASSERT(cond, expr, file, line) ((cond) ? (void)0 : fn_1_7EC((expr), (file), (line)))

// The inlined form of the assertion handler (singleton.h, model.cpp, ...).
#define M568_ASSERT_INL(cond, expr, file, line)                                      if (cond) {                                                                          (void)0;                                                                     } else {                                                                             char assertBuf[0x400];                                                           sprintf(assertBuf, lbl_1_data_64, (file), (line), (expr));                       OSPanic(lbl_1_data_82, 0x12, assertBuf);                                     }

// ---------------------------------------------------------------- types

typedef M568Link M568Node;

// Camera pose: eye position, up vector, look-at target.
typedef struct M568Cam_s {
    Vec pos;
    Vec up;
    Vec target;
} M568Cam;

// Stored pose: position plus orientation quaternion.
typedef struct M568Pose_s {
    Vec pos;
    Quaternion rot;
} M568Pose;

// A pose viewed as the seven raw words the pose-table dump prints.
typedef union M568PoseBits_u {
    M568Pose pose;
    u32 w[7];
} M568PoseBits;

// Task registered on the module-wide list; its vtable lives at +8.
struct M568Task_s;
typedef struct M568TaskVtbl_s {
    void *rtti[2];
    struct M568Task_s *(*dtor)(struct M568Task_s *self, s16 flag);
    void (*onFlag)(struct M568Task_s *self, s32 bit);
    void (*update)(struct M568Task_s *self);
} M568TaskVtbl;

typedef struct M568Task_s {
    M568Node link;
    M568TaskVtbl *vtbl;
    u32 flags;
} M568Task;

// Mersenne Twister state.
typedef struct M568Rand_s {
    u32 mt[0x270];
    u32 idx;
} M568Rand;

// Functor kept in an object's data slot: slot +0xC runs it.
struct M568Functor_s;
typedef struct M568FunctorVtbl_s {
    void *rtti[2];
    struct M568Functor_s *(*dtor)(struct M568Functor_s *self, s16 flag);
    void (*run)(struct M568Functor_s *self, OMOBJ *obj);
} M568FunctorVtbl;

typedef struct M568Functor_s {
    M568FunctorVtbl *vtbl;
} M568Functor;

// A wrapper that owns a Hu3D model / MgTimer handle.

typedef struct M568TimerObj_s {
    MGTIMER *timer;
} M568TimerObj;

// ---------------------------------------------------------------- data

char lbl_1_data_50[] = "ptr != NULL";
char lbl_1_data_5C[] = "new.cpp";
char lbl_1_data_64[] = "\nASSERTION FAILED at %s:%d\n%s";
char lbl_1_data_82[] = "assert.cpp";
char lbl_1_data_8D[] = "{{0x%08x,0x%08x,0x%08x},{0x%08x,0x%08x,0x%08x,0x%08x}},\n";
char lbl_1_data_C6[] = "singleton.h";
char lbl_1_data_D2[] = "ms_pointer == 0";
char lbl_1_data_E2[] = "ms_pointer != 0";
char lbl_1_data_F2[] = "model.cpp";
char lbl_1_data_FC[] = "p != NULL";
char lbl_1_data_106[] = "om.cpp";
char lbl_1_data_10D[] = "subthread.cpp";
char lbl_1_data_11B[] = "::OSGetCurrentThread() == m_th";
char lbl_1_data_13A[] = "!::OSIsThreadTerminated(m_th)";
char lbl_1_data_158[] = "thread.cpp";
char lbl_1_data_163[] = "priority >= OS_PRIORITY_MIN && priority <= OS_PRIORITY_MAX";
char lbl_1_data_19E[] = "OSCreateThread() failed.";
char lbl_1_data_1B7[] = "delete CThread@%p\nstack remain/total = 0x%08x/0x%08x (%f%%)\n";
char lbl_1_data_1F4[] = "stack overflow\n";

M568SubThread *fn_1_58FC(M568SubThread *self, s16 flag);
M568Task *fn_1_24C0(M568Task *self, s16 flag);
void fn_1_23F4(M568Task *self);
M568Master *fn_1_26E4(M568Master *self, s16 flag);
void fn_1_2A84(M568Master *self);
M568Functor *fn_1_4F4C(M568Functor *self, s16 flag);
void fn_1_4ED8(M568Functor *self, OMOBJ *obj);
void fn_1_4E8C(OMOBJ *obj);

M568ThreadVtbl lbl_1_data_204 = { { 0, 0 }, fn_1_58FC, 0 };
M568FunctorVtbl lbl_1_data_214 = { { 0, 0 }, fn_1_4F4C, fn_1_4ED8 };
M568MasterVtbl lbl_1_data_224 = { { 0, 0 }, fn_1_26E4, 0, fn_1_2A84, 0 };
M568TaskVtbl lbl_1_data_23C = { { 0, 0 }, fn_1_24C0, 0, fn_1_23F4 };

// ---------------------------------------------------------------- bss

u8 lbl_1_bss_0[0xC];
M568Node lbl_1_bss_C;
s32 lbl_1_bss_14;
s32 lbl_1_bss_18;
s32 lbl_1_bss_1C;
s32 lbl_1_bss_20;
M568Rand lbl_1_bss_24;
u8 lbl_1_bss_9E8;
M568Master *lbl_1_bss_9EC;

// ---------------------------------------------------------------- code

void fn_1_7EC(const char *expr, const char *file, s32 line);
int fn_1_84C(void *unused, const char *fmt, ...);
void *fn_1_6218(M568Node *self, s16 flag);
void fn_1_A20(M568Cam *self, Vec *x, Vec *y, Vec *z);
void fn_1_B84(M568Cam *self, Vec *delta);

void *fn_1_618(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M568_ASSERT(p != 0, lbl_1_data_50, lbl_1_data_5C, 0x17);
    return p;
}

void *fn_1_67C(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M568_ASSERT(p != 0, lbl_1_data_50, lbl_1_data_5C, 0x17);
    return p;
}

void fn_1_6E0(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_718(void *ptr)
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

void fn_1_7EC(const char *expr, const char *file, s32 line)
{
    char buf[0x40C];

    sprintf(buf, lbl_1_data_64, file, line, expr);
    OSPanic(lbl_1_data_82, 0x12, buf);
}

int fn_1_84C(void *unused, const char *fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = vprintf(fmt, ap);
    va_end(ap);
    return ret;
}

// Orbit: rotate about the camera X axis (pitch) then world Y (yaw).
void fn_1_8F0(M568Cam *self, f32 pitch, f32 yaw)
{
    Vec ax, ay, az;
    Mtx rotA, rotB, rot;

    fn_1_A20(self, &ax, &ay, &az);
    PSMTXRotAxisRad(rotA, &ax, 0.017453292f * pitch);
    PSMTXRotRad(rotB, 'Y', 0.017453292f * yaw);
    PSMTXConcat(rotA, rotB, rot);
    PSMTXMultVecSR(rot, &az, &az);
    PSMTXMultVecSR(rot, &ay, &self->up);
    PSVECSubtract(&self->pos, &az, &self->target);
}

void fn_1_9B4(M568Cam *self, f32 angle)
{
    Mtx m;
    Vec axis;

    PSVECSubtract(&self->target, &self->pos, &axis);
    PSMTXRotAxisRad(m, &axis, 0.017453292f * angle);
    PSMTXMultVec(m, &self->up, &self->up);
}

// Camera basis: the three rows of the look-at matrix.
void fn_1_A20(M568Cam *self, Vec *x, Vec *y, Vec *z)
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

void fn_1_AC8(M568Cam *self, Mtx out)
{
    C_MTXLookAt(out, &self->pos, &self->up, &self->target);
    PSMTXInverse(out, out);
}

void fn_1_B1C(M568Cam *self, Vec *local)
{
    Mtx m;
    Vec world;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    PSMTXMultVecSR(m, local, &world);
    fn_1_B84(self, &world);
}

void fn_1_B84(M568Cam *self, Vec *delta)
{
    PSVECAdd(delta, &self->target, &self->target);
    PSVECAdd(delta, &self->pos, &self->pos);
}

void fn_1_BD8(M568Cam *self)
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

void fn_1_C6C(M568Cam *self, M568Pose *pose)
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

void fn_1_D0C(M568Cam *self, M568Pose *out)
{
    Mtx m;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    out->pos.x = self->pos.x;
    out->pos.y = self->pos.y;
    out->pos.z = self->pos.z;
    C_QUATMtx(&out->rot, m);
}

void fn_1_D84(M568Cam *self, M568Pose *a, M568Pose *b, f32 t)
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

void fn_1_E94(M568Cam *self, M568Cam *src)
{
    Vec delta;

    PSVECSubtract(&src->pos, &self->pos, &delta);
    self->pos.x = src->pos.x;
    self->pos.y = src->pos.y;
    self->pos.z = src->pos.z;
    PSVECAdd(&self->target, &delta, &self->target);
}

void fn_1_F00(M568Cam *self, Vec *a, Vec *b, f32 t)
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

void fn_1_FB8(M568Cam *self, u16 camBit)
{
    Hu3DCameraPosSetV(camBit, &self->pos, &self->up, &self->target);
}

void fn_1_FF8(M568Cam *self, u16 camBit)
{
    Hu3DCameraPosGet(camBit, &self->pos, &self->up, &self->target);
}

// Dump the current camera as a pose table row (floats printed as raw words).
void fn_1_1038(M568Cam *self)
{
    Mtx m;
    M568PoseBits out;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    out.pose.pos.x = self->pos.x;
    out.pose.pos.y = self->pos.y;
    out.pose.pos.z = self->pos.z;
    C_QUATMtx(&out.pose.rot, m);
    printf(lbl_1_data_8D, out.w[0], out.w[1], out.w[2], out.w[3], out.w[4], out.w[5], out.w[6]);
}

f32 fn_1_10D0(s32 pad)
{
    return 0.017857144f * HuPadStkX[pad];
}

f32 fn_1_1124(s32 pad)
{
    return 0.017857144f * HuPadStkY[pad];
}

f32 fn_1_1178(s32 pad)
{
    return 0.016949153f * HuPadSubStkX[pad];
}

f32 fn_1_11CC(s32 pad)
{
    return 0.016949153f * HuPadSubStkY[pad];
}

// Free-fly debug camera driven by pad `pad`.
void fn_1_1220(M568Cam *self, s32 pad)
{
    Mtx inv, rot, rotB, rotA, m, dump;
    M568PoseBits out;
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
    yaw = -4.0f * fn_1_10D0(pad);
    pitch = -4.0f * fn_1_1124(pad);
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
    speed = fast ? 70.0f : 20.0f;  // orig re-extends fast to u8 before the test (1 insn)
    C_MTXLookAt(inv, &self->pos, &self->up, &self->target);
    PSMTXInverse(inv, inv);
    mv.x = (speed * (u8)((btn & 2) != 0)) - (speed * (u8)((btn & 1) != 0));
    mv.y = (speed * (u8)((btn & 8) != 0)) - (speed * (u8)((btn & 4) != 0));
    mv.x = mv.x + (speed * fn_1_1178(pad));
    mv.z = 0.0f;
    PSMTXMultVecSR(inv, &mv, &mv);
    mv.y = mv.y + (speed * fn_1_11CC(pad));
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
        printf(lbl_1_data_8D, out.w[0], out.w[1], out.w[2], out.w[3], out.w[4], out.w[5], out.w[6]);
    }
}

// ---------------------------------------------------------------- intrusive list

typedef u8 (*M568Cmp)(M568Node *a, M568Node *b);

M568Node *fn_1_2160(M568Node *list);
M568Node *fn_1_21B4(M568Cmp cmp, M568Node *a, M568Node *b);
M568Node *fn_1_1AB8(M568Cmp cmp, M568Node *list);
void fn_1_2988(OMOBJ *obj);
void fn_1_2A88(void);
void fn_1_3230(void);
void fn_1_4DFC(void *ptr);

void fn_1_187C(void *link)
{
    ((M568Node *)link)->next = (M568Node *)link;
    ((M568Node *)link)->prev = (M568Node *)link;
}

void *fn_1_1888(void *link, s16 flag)
{
    if (link != 0) {
        ((M568Node *)link)->next->prev = ((M568Node *)link)->prev;
        ((M568Node *)link)->prev->next = ((M568Node *)link)->next;
        if (flag > 0 && link != 0) {
            HuMemDirectFree(link);
        }
    }
    return link;
}

void fn_1_18F4(M568Node *self)
{
    self->next->prev = self->prev;
    self->prev->next = self->next;
    self->next = self;
    self->prev = self;
}

// Insert `self` right after `other`.
void fn_1_1918(M568Node *self, M568Node *other)
{
    M568Node *at = other;

    self->prev = at;
    self->next = at->next;
    at->next->prev = self;
    at->next = self;
}

// Insert `self` right before `other`.
void fn_1_1948(M568Node *self, M568Node *other)
{
    M568Node *at = other;

    self->next = at;
    self->prev = at->prev;
    at->prev->next = self;
    at->prev = self;
}

// Move every node of `self` in front of `other`.
void fn_1_1978(M568Node *self, M568Node *other)
{
    M568Node *dest;
    M568Node *first;
    M568Node *last;
    u8 empty;

    empty = 0;
    if (self->next == self && self->next == self->prev) {
        empty = 1;
    }
    if (empty == 0) {
        dest = other;
        first = self->next;
        last = self->prev;
        self->next = self;
        self->prev = self;
        first->prev = dest->prev;
        last->next = dest;
        dest->prev->next = first;
        dest->prev = last;
    }
}

// Merge sort the circular list `self` with `cmp`, then relink the prev chain.
void fn_1_1A00(M568Node *self, M568Cmp cmp)
{
    M568Node *node;
    M568Node *tail;
    u8 empty;

    empty = 0;
    if (self->next == self && self->next == self->prev) {
        empty = 1;
    }
    if (empty == 0) {
        self->prev->next = 0;
        node = fn_1_1AB8(cmp, self->next);
        self->next = node;
        tail = self;
        while (node != 0) {
            node->prev = tail;
            tail = node;
            node = node->next;
        }
        self->prev = tail;
        tail->next = self;
    }
}

M568Node *fn_1_1AB8(M568Cmp cmp, M568Node *list)
{
    M568Node *second;
    M568Node *a;
    M568Node *b;

    if (list == 0) {
        return 0;
    }
    if (list->next == 0) {
        return list;
    }
    second = fn_1_2160(list);
    a = fn_1_1AB8(cmp, list);
    b = fn_1_1AB8(cmp, second);
    return fn_1_21B4(cmp, a, b);
}

// Cut the list in the middle and return the second half.
M568Node *fn_1_2160(M568Node *list)
{
    M568Node *fast;
    M568Node *second;

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

M568Node *fn_1_21B4(M568Cmp cmp, M568Node *a, M568Node *b)
{
    M568Node head;
    M568Node *tail;

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

void fn_1_2260(s32 bit)
{
    M568Task *t;
    M568Node *end;
    M568Node *b;
    M568Node *c;
    M568Node *nx;

    b = lbl_1_bss_C.next;
    c = b;
    t = (M568Task *)c;
    lbl_1_bss_14 = bit;
    end = &lbl_1_bss_C;
    while ((M568Node *)t != end) {
        t->flags |= 1 << bit;
        t->vtbl->onFlag(t, bit);
        nx = t->link.next;
        t = (M568Task *)nx;
    }
}

s32 fn_1_22F8(void)
{
    M568Task *t;
    M568Node *end;
    M568Node *b;
    M568Node *c;
    M568Node *nx;
    s32 bit;

    b = lbl_1_bss_C.next;
    c = b;
    t = (M568Task *)c;
    end = &lbl_1_bss_C;
    while ((M568Node *)t != end) {
        bit = lbl_1_bss_14;
        if ((t->flags & (1 << bit)) != 0) {
            return 0;
        }
        nx = t->link.next;
        t = (M568Task *)nx;
    }
    return 1;
}

void fn_1_2384(void)
{
    M568Task *t;
    M568Node *end;
    M568Node *b;
    M568Node *c;
    M568Node *nx;

    b = lbl_1_bss_C.next;
    c = b;
    t = (M568Task *)c;
    end = &lbl_1_bss_C;
    while ((M568Node *)t != end) {
        t->vtbl->update(t);
        nx = t->link.next;
        t = (M568Task *)nx;
    }
}

void fn_1_23F4(M568Task *self)
{
}

// Destroy every registered task, last one first.
void fn_1_23F8(void)
{
    M568Task *t;
    u8 empty;

    while (1) {
        empty = 0;
        if (lbl_1_bss_C.next == &lbl_1_bss_C && lbl_1_bss_C.next == lbl_1_bss_C.prev) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        t = (M568Task *)lbl_1_bss_C.prev;
        if (t != 0) {
            t->vtbl->dtor(t, 1);
        }
    }
}

M568Task *fn_1_24C0(M568Task *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_23C;
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

s32 fn_1_2548(void)
{
    return lbl_1_bss_14;
}

// ---------------------------------------------------------------- game master

M568Master *fn_1_2558(M568Master *self)
{
    s32 i;

    M568_ASSERT_INL(lbl_1_bss_9EC == 0, lbl_1_data_D2, lbl_1_data_C6, 0x35);
    lbl_1_bss_9EC = self;
    self->vtbl = &lbl_1_data_224;
    self->counter = 0;
    i = 0;
    while (i < 4) {
        self->slot[i] = -1;
        i++;
    }
    return self;
}

M568Master *fn_1_2628(M568Master *self, s16 flag)
{
    if (self != 0) {
        M568_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_E2, lbl_1_data_C6, 0x3D);
        lbl_1_bss_9EC = 0;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

M568Master *fn_1_26E4(M568Master *self, s16 flag)
{
    M568Master *m;
    s32 i;
    s32 p;

    if (self != 0) {
        self->vtbl = &lbl_1_data_224;
        if (mgDataTbl[GwSystem.mgNo].type != 3) {
            M568_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_E2, lbl_1_data_C6, 0x27);
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
            M568_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_E2, lbl_1_data_C6, 0x3D);
            lbl_1_bss_9EC = 0;
        }
        if (flag > 0) {
            fn_1_4DFC(self);
        }
    }
    return self;
}

void fn_1_289C(M568Master *self, s32 objMax, s32 prio)
{
    OMOBJMAN *om;

    om = omInitObjMan((s16)objMax, prio);
    omGameSysInit(om);
    self->vtbl->setup(self, om);
    omAddObjEx(om, 0, 0, 0, -1, fn_1_2988);
}

void fn_1_2920(M568Master *self)
{
    OMOBJMAN *om;

    om = MgActorObjectSetup();
    self->vtbl->setup(self, om);
    omAddObjEx(om, 0, 0, 0, -1, fn_1_2988);
}

void fn_1_2A18(M568Master *self);

void fn_1_2988(OMOBJ *obj)
{
    OMOBJ *self;
    M568Master *m;
    M568Master *cur;

    self = obj;
    if (omSysExitReq != 0 && WipeCheckEnd() == 0) {
        fn_1_3230();
        HuAudFadeOut(0x3E8);
        self->objFunc = (OMOBJFUNC)fn_1_2A88;
        return;
    }
    m = lbl_1_bss_9EC;
    cur = m;
    fn_1_2A18(cur);
}

void fn_1_2A18(M568Master *self)
{
    self->vtbl->update(self);
    if (self->counter > 0) {
        if (--self->counter == 0) {
        }
    }
}

void fn_1_2A84(M568Master *self)
{
}

void fn_1_2A88(void)
{
    M568Master *m;
    M568Master *cur;
    M568Master *obj;
    s32 c;
    u8 hitZero;

    m = lbl_1_bss_9EC;
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
void fn_1_2B74(s32 player)
{
    s32 *slots;
    s32 i;

    M568_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_E2, lbl_1_data_C6, 0x27);
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

s32 fn_1_2C48(s32 player)
{
    s32 i;

    M568_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_E2, lbl_1_data_C6, 0x27);
    i = 0;
    while (i < 4) {
        if (player == lbl_1_bss_9EC->slot[i]) {
            return 1;
        }
        i++;
    }
    return 0;
}

void fn_1_2D1C(void)
{
    M568Master *m;

    M568_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_E2, lbl_1_data_C6, 0x27);
    m = lbl_1_bss_9EC;
    m->mesId = GameMesCreate(2, 0);
}

void fn_1_2DD4(void)
{
    M568Master *m;

    M568_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_E2, lbl_1_data_C6, 0x27);
    m = lbl_1_bss_9EC;
    m->mesId = GameMesCreate(2, 1);
}

s32 fn_1_2E8C(void)
{
    M568Master *m;
    s32 ret;

    M568_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_E2, lbl_1_data_C6, 0x27);
    m = lbl_1_bss_9EC;
    ret = 0;
    if (GameMesStatGet(-1) == 0 && (u8)(m->counter <= 0) != 0) {
        ret = 1;
    }
    return ret;
}

// Announce the result: sort the registered players and print their characters.
void fn_1_2F60(void)
{
    M568Master *m;
    s32 *slots;
    s32 chars[4];
    s32 n;
    s32 i;
    s32 j;
    s32 k;
    s32 tmp;
    s16 charNo;

    M568_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_E2, lbl_1_data_C6, 0x27);
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

void fn_1_312C(s32 mes)
{
    M568Master *m;

    if (_CheckFlag(0x1000F) == 0) {
        M568_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_E2, lbl_1_data_C6, 0x27);
        m = lbl_1_bss_9EC;
        m->mesId = GameMesCreate(0xE, mes);
        HuAudJinglePlay(0x86);
    }
}

void fn_1_3204(void)
{
    WipeCreate(1, 5, 0x3C);
}

void fn_1_3230(void)
{
    WipeCreate(2, 6, 0x3C);
}

void fn_1_325C(void)
{
    omSysExitReq = 1;
}

// ---------------------------------------------------------------- Hu3D model wrappers

M568Model *fn_1_3270(M568Model *self, void *data)
{
    self->id = Hu3DModelCreate(data);
    return self;
}

M568Model *fn_1_32AC(M568Model *self, s32 dataNum)
{
    self->id = Hu3DModelCreateData(dataNum);
    return self;
}

M568Model *fn_1_32F4(M568Model *self, HU3DMODELHOOK hook)
{
    self->id = Hu3DHookFuncCreate(hook);
    return self;
}

// Build a character model; `type` selects the model variant bit.
M568Model *fn_1_3330(M568Model *self, s32 charNo, s32 type)
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

inline s32 M568ModelIdGet(M568Model *m)
{
    return m->id;
}

M568Model *fn_1_33CC(M568Model *self, M568Model *parent)
{
    self->id = Hu3DModelLink(M568ModelIdGet(parent));
    return self;
}

void fn_1_3420(M568Model *self, u32 attr)
{
    Hu3DModelAttrSet(self->id, attr);
}

void fn_1_3458(M568Model *self, u32 attr)
{
    Hu3DModelAttrReset(self->id, attr);
}

// Apply an object's transform (translation, rotation, scale) to the model.
void fn_1_3490(M568Model *self, OMOBJ *obj)
{
    HU3DMODELID id;

    id = self->id;
    Hu3DModelPosSet(id, obj->trans.x, obj->trans.y, obj->trans.z);
    Hu3DModelRotSet(id, obj->rot.x, obj->rot.y, obj->rot.z);
    Hu3DModelScaleSet(id, obj->scale.x, obj->scale.y, obj->scale.z);
}

void fn_1_350C(M568Model *self, Vec *pos)
{
    Hu3DModelPosSet(self->id, pos->x, pos->y, pos->z);
}

void fn_1_3554(M568Model *self, Vec *rot)
{
    Hu3DModelRotSet(self->id, rot->x, rot->y, rot->z);
}

void fn_1_359C(M568Model *self, Vec *scale)
{
    Hu3DModelScaleSet(self->id, scale->x, scale->y, scale->z);
}

void fn_1_35E4(M568Model *self, f32 scale)
{
    Hu3DModelScaleSet(self->id, scale, scale, scale);
}

void fn_1_362C(M568Model *self, f32 tpLvl)
{
    Hu3DModelTPLvlSet(self->id, tpLvl);
}

void fn_1_3664(M568Model *self, s16 layer)
{
    Hu3DModelLayerSet(self->id, layer);
}

void fn_1_369C(M568Model *self, u16 camBit)
{
    Hu3DModelCameraSet(self->id, camBit);
}

void fn_1_36D4(M568Model *self, void *data)
{
    Hu3DData[self->id].hookData = data;
}

MtxPtr fn_1_36F0(M568Model *self)
{
    return Hu3DData[self->id].mtx;
}

M568Model *fn_1_370C(M568Model *self, s16 flag)
{
    if (self != 0) {
        Hu3DModelKill(self->id);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

inline HSFOBJECT *M568ObjGet(M568Model *self, char *name)
{
    HSFOBJECT *obj;

    obj = Hu3DModelObjPtrGet(self->id, name);
    M568_ASSERT_INL(obj != 0, lbl_1_data_FC, lbl_1_data_F2, 0x8A);
    return obj;
}

HSFOBJECT *fn_1_3768(M568Model *self, char *name)
{
    return M568ObjGet(self, name);
}

void fn_1_37FC(M568Model *self)
{
    Hu3DModelShadowSet(self->id);
}

void fn_1_382C(M568Model *self)
{
    Hu3DModelShadowMapSet(self->id);
}

void fn_1_385C(M568Model *self, char *name)
{
    Hu3DModelShadowMapObjSet(self->id, name);
}

void fn_1_3894(M568Model *self)
{
    Hu3DModelShadowMapObjReset(self->id);
}

void fn_1_38C4(M568Model *self, char *name)
{
    HSFOBJECT *obj;
    u32 *flags;

    obj = M568ObjGet(self, name);
    flags = (u32 *)obj->constData;
    *flags &= ~8;
}

void fn_1_3970(M568Model *self, char *name)
{
    HSFOBJECT *obj;

    obj = M568ObjGet(self, name);
    obj->flags |= 0x400;
}

void fn_1_3A18(M568Model *self, char *name)
{
    HSFOBJECT *obj;

    obj = M568ObjGet(self, name);
    obj->flags &= ~0x400;
}

void fn_1_3AC0(M568Model *self, char *name)
{
    HSFOBJECT *obj;
    HSFTRANSFORM *base;

    obj = M568ObjGet(self, name);
    obj->flags |= 0x400;
    base = &obj->mesh.base;
    base->scale.x = 0.0f;
    base->scale.y = 0.0f;
    base->scale.z = 0.0f;
}

void fn_1_3B9C(M568Model *self, char *name)
{
    HSFOBJECT *obj;
    HSFTRANSFORM *base;

    obj = M568ObjGet(self, name);
    obj->flags &= ~0x400;
    base = &obj->mesh.base;
    base->scale.x = 1.0f;
    base->scale.y = 1.0f;
    base->scale.z = 1.0f;
}

void fn_1_3C78(M568Model *self, s32 time)
{
    Hu3DMotionTimeSet(M568ModelIdGet(self), (f32)time);
}

// ---------------------------------------------------------------- Mersenne Twister

extern const u32 lbl_1_rodata_50[2] = { 0, 0x9908B0DF };

void fn_1_3CE4(M568Rand *r, u32 seed)
{
    r->mt[0] = seed;
    r->idx = 1;
    while (r->idx < 0x270) {
        r->mt[r->idx] = r->idx + (((r->mt[r->idx - 1]) ^ ((r->mt[r->idx - 1]) >> 30)) * 0x6C078965);
        r->idx++;
    }
}

void fn_1_3D58(M568Rand *r, u32 *key, u32 len)
{
    u32 i;
    u32 j;
    u32 k;

    fn_1_3CE4(r, 19650218);
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

u32 fn_1_3F1C(M568Rand *r)
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

u32 fn_1_40A8(M568Rand *r)
{
    return fn_1_3F1C(r) >> 1;
}

f32 fn_1_4240(M568Rand *r)
{
    return 2.3283064e-10f * (f32)fn_1_3F1C(r);
}

f32 fn_1_4404(M568Rand *r)
{
    return 2.3283064e-10f * (f32)fn_1_3F1C(r);
}

f32 fn_1_45C8(M568Rand *r)
{
    return 2.3283064e-10f * (0.5f + (f32)fn_1_3F1C(r));
}

f64 fn_1_479C(M568Rand *r)
{
    u32 a;
    u32 b;

    a = fn_1_3F1C(r) >> 5;
    b = fn_1_3F1C(r) >> 6;
    return 1.1102230246251565e-16 * ((67108864.0 * (f64)a) + (f64)b);
}

f32 fn_1_4B14(M568Rand *r)
{
    return 1.1920929e-7f * (f32)(fn_1_3F1C(r) >> 9);
}

// ---------------------------------------------------------------- allocation / functor objects

void *fn_1_4CE4(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M568_ASSERT_INL(p != 0, lbl_1_data_50, lbl_1_data_106, 0x18);
    return p;
}

void *fn_1_4D70(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M568_ASSERT_INL(p != 0, lbl_1_data_50, lbl_1_data_106, 0x1F);
    return p;
}

void fn_1_4DFC(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_4E34(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

// Functor owned by an object: the object's data slot points at it.
void fn_1_4E6C(M568Functor *self, OMOBJ *obj)
{
    self->vtbl = &lbl_1_data_214;
    obj->data = self;
    obj->objFunc = fn_1_4E8C;
}

void fn_1_4E8C(OMOBJ *obj)
{
    M568Functor *self;

    self = (M568Functor *)obj->data;
    self->vtbl->run(self, obj);
}

void fn_1_4ED8(M568Functor *self, OMOBJ *obj)
{
}

void fn_1_4EDC(OMOBJ *obj)
{
    M568Functor *fn;
    M568Functor *p;

    fn = (M568Functor *)obj->data;
    p = fn;
    if (p != 0) {
        p = p->vtbl->dtor(p, 1);
    }
    omDelObjEx(HuPrcCurrentGet(), obj);
}

M568Functor *fn_1_4F4C(M568Functor *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_214;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_4FAC(OMOBJMAN *man, OMOBJ *obj)
{
    M568Functor *fn;
    M568Functor *p;

    fn = (M568Functor *)obj->data;
    p = fn;
    if (p != 0) {
        p = p->vtbl->dtor(p, 1);
    }
    omDelObjEx(man, obj);
}

// ---------------------------------------------------------------- global random number generator

void fn_1_5020(u32 seed)
{
    fn_1_3CE4(&lbl_1_bss_24, seed);
}

u32 fn_1_50F8(void)
{
    return fn_1_3F1C(&lbl_1_bss_24) >> 1;
}

f32 fn_1_5334(void)
{
    return 1.1920929e-7f * (f32)(fn_1_3F1C(&lbl_1_bss_24) >> 9);
}

f32 fn_1_55A8(f32 scale)
{
    return scale * fn_1_479C(&lbl_1_bss_24);
}

f32 fn_1_55E0(f32 from, f32 to)
{
    return from + (to - from) * fn_1_479C(&lbl_1_bss_24);
}

// ---------------------------------------------------------------- shadow / collision helpers

// Sphere: centre and radius.
typedef struct M568Sphere_s {
    Vec pos;
    f32 radius;
} M568Sphere;

inline f32 M568SphereRadius(M568Sphere *s)
{
    return s->radius;
}


// Do two spheres overlap?
u32 fn_1_5634(M568Sphere *a, M568Sphere *b)
{
    Vec d;
    f32 sum;
    f32 rb;
    f32 ra;

    rb = b->radius;
    ra = a->radius;
    sum = ra + rb;
    PSVECSubtract(&a->pos, &b->pos, &d);
    return PSVECMag(&d) < sum;
}

// Push two overlapping spheres apart; ka/kb weight the move of each.
s32 fn_1_56DC(M568Sphere *a, M568Sphere *b, f32 ka, f32 kb)
{
    Vec d;
    Vec off;
    f32 rb;
    f32 ra;
    f32 sum;
    f32 dist;
    f32 k;
    f32 distCopy;

    rb = b->radius;
    ra = a->radius;
    sum = ra + rb;
    PSVECSubtract(&b->pos, &a->pos, &d);
    dist = PSVECMag(&d);
    distCopy = dist;
    if (distCopy >= sum) {
        return 0;
    }
    k = (kb * (sum - distCopy)) / distCopy;
    PSVECScale(&d, &d, k);
    PSVECScale(&d, &off, -ka);
    PSVECAdd(&off, &a->pos, &a->pos);
    PSVECScale(&d, &off, 1.0f - ka);
    PSVECAdd(&off, &b->pos, &b->pos);
    return 1;
}

// ---------------------------------------------------------------- threads

s32 fn_1_585C(void)
{
    return OSGetThreadPriority(OSGetCurrentThread());
}

void *fn_1_5B44(void *arg);
M568Thread *fn_1_5BA0(M568Thread *th, void *(*func)(void *), void *arg, u32 size, s32 prio, u16 attr);

M568SubThread *fn_1_5880(M568SubThread *self, u32 stackSize)
{
    self->vtbl = &lbl_1_data_204;
    fn_1_5BA0(&self->th, fn_1_5B44, self, stackSize, OSGetThreadPriority(OSGetCurrentThread()), 1);
    OSInitSemaphore(&self->sem, 0);
    return self;
}

M568Thread *fn_1_5E24(M568Thread *th, s16 flag);

M568SubThread *fn_1_58FC(M568SubThread *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_204;
        fn_1_5E24(&self->th, -1);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

// CThread::IsTerminated(), inlined with its own `this`.
inline u8 M568ThreadDone(M568Thread *th)
{
    return OSIsThreadTerminated(&th->thread) != 0;
}

u8 fn_1_5968(M568SubThread *self)
{
    M568Thread *th = &self->th;

    return OSIsThreadTerminated(&th->thread) != 0;
}

u32 fn_1_5F58(M568Thread *th);

s32 fn_1_59B0(M568SubThread *self, s32 arg)
{
    M568Thread *thIsDone;
    M568Thread *thResume;
    M568Sem *sem;

    thIsDone = &self->th;
    if (OSIsThreadTerminated(&thIsDone->thread) != 0) {
        return self->arg;
    }
    self->arg = arg;
    thResume = &self->th;
    OSResumeThread(&thResume->thread);
    sem = &self->sem;
    OSWaitSemaphore(sem);
    fn_1_5F58(&self->th);
    OSCheckActiveThreads();
    return self->arg;
}

s32 fn_1_5A40(M568SubThread *self, s32 arg)
{
    M568Thread *thCur;
    M568Thread *thDone;
    M568Sem *sem;
    M568Thread *thSuspend;

    thCur = &self->th;
    M568_ASSERT_INL(OSGetCurrentThread() == &thCur->thread, lbl_1_data_11B, lbl_1_data_10D, 0x47);
    thDone = &self->th;
    M568_ASSERT_INL(OSIsThreadTerminated(&thDone->thread) == 0, lbl_1_data_13A, lbl_1_data_10D, 0x4A);
    self->arg = arg;
    sem = &self->sem;
    OSSignalSemaphore(sem);
    thSuspend = &self->th;
    OSSuspendThread(&thSuspend->thread);
    return self->arg;
}

void *fn_1_5B44(void *arg)
{
    M568SubThread *self;
    M568Sem *sem;

    self = (M568SubThread *)arg;
    self->vtbl->run(self, self->arg);
    sem = &self->sem;
    OSSignalSemaphore(sem);
    return 0;
}

void fn_1_5D00(M568Thread *th, void *(*func)(void *), void *arg, u32 *stack, u32 words, s32 prio, u16 attr);

M568Thread *fn_1_5BA0(M568Thread *th, void *(*func)(void *), void *arg, u32 size, s32 prio, u16 attr)
{
    u32 words;
    u32 *stack;

    words = size >> 2;
    stack = (u32 *)HuMemDirectMallocNum(HEAP_HEAP, words * 4, 0x10000000);
    M568_ASSERT_INL(stack != 0, lbl_1_data_50, lbl_1_data_5C, 0x17);
    th->stack = stack;
    fn_1_5D00(th, func, arg, stack, words, prio, attr);
    return th;
}

M568Thread *fn_1_5C84(M568Thread *th, void *(*func)(void *), void *arg, u32 *stack, u32 size, s32 prio, u16 attr)
{
    u32 words;

    th->stack = 0;
    words = size >> 2;
    fn_1_5D00(th, func, arg, stack, words, prio, attr);
    return th;
}

void fn_1_606C(u32 *p, u32 words);
s32 fn_1_5FF8(u32 *p, s32 mode);

void fn_1_5D00(M568Thread *th, void *(*func)(void *), void *arg, u32 *stack, u32 words, s32 prio, u16 attr)
{
    u8 ok;

    ok = 0;
    if (prio >= 0 && prio <= 0x1F) {
        ok = 1;
    }
    M568_ASSERT_INL(ok != 0, lbl_1_data_163, lbl_1_data_158, 0x52);
    fn_1_606C(stack, words);
    fn_1_5FF8(stack, 0);
    if ((u8)(OSCreateThread(&th->thread, func, arg, &stack[words], words * 4, prio, attr) != 0) == 0) {
        OSPanic(lbl_1_data_158, 0x60, lbl_1_data_19E);
    }
}

M568Thread *fn_1_5E24(M568Thread *th, s16 flag)
{
    u32 total;
    u32 remain;
    u32 *stack;

    if (th != 0) {
        if (OSIsThreadTerminated(&th->thread) == 0) {
            OSCancelThread(&th->thread);
        }
        if (lbl_1_bss_9E8 != 0) {
            total = (uintptr_t)th->thread.stackBase - (uintptr_t)th->thread.stackEnd;
            remain = fn_1_5F58(th);
            printf(lbl_1_data_1B7, th, remain, total, (100.0 * (f64)remain) / (f64)total);
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

u32 fn_1_5F58(M568Thread *th)
{
    u32 *base;
    u32 *p;

    base = th->thread.stackEnd;
    if (fn_1_5FF8(base, 1) != 0) {
        OSPanic(lbl_1_data_158, 0x7F, lbl_1_data_1F4);
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
s32 fn_1_5FF8(u32 *p, s32 mode)
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

void fn_1_606C(u32 *p, u32 words)
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

// ---------------------------------------------------------------- static initialisers

void fn_1_60B8(void)
{
    lbl_1_bss_C.next = &lbl_1_bss_C;
    lbl_1_bss_C.prev = &lbl_1_bss_C;
#ifdef __MWERKS__
    __register_global_object(&lbl_1_bss_C, (void *)fn_1_6218, lbl_1_bss_0);
#endif
    lbl_1_bss_18 = 0x78;
    lbl_1_bss_1C = 0xD2;
    lbl_1_bss_20 = 0xD2;
    fn_1_3CE4(&lbl_1_bss_24, 0x1571);
}

void *fn_1_6218(M568Node *self, s16 flag)
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
__declspec(section ".ctors") void (*const lbl_ctor_61C4)(void) = fn_1_60B8;
#else
struct M568StaticInit {
    M568StaticInit() { fn_1_60B8(); }
};
static M568StaticInit lbl_static_init;
#endif
}
