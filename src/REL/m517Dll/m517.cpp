// m517Dll "m517": the module's runtime layer -- camera pose maths, an intrusive
// list of tasks, the game-master singleton plumbing, thin Hu3D/MgTimer wrappers,
// a Mersenne Twister, a sub-thread class and the C++ static initialisers.
#define _MATH_H
extern "C" {
#include "REL/m517Dll.h"
#include "dolphin.h"
#include "humath.h"
extern "C" {
#include "game/hu3d.h"
}
extern "C" {
#include "game/object.h"
}
extern "C" {
#include "game/memory.h"
}
extern "C" {
#include "game/gamemes.h"
}
extern "C" {
#include "game/wipe.h"
}
extern "C" {
#include "game/flag.h"
}
extern "C" {
#include "game/gamework.h"
}
extern "C" {
#include "game/data.h"
}
extern "C" {
#include "game/audio.h"
}
extern "C" {
#include "game/pad.h"
}
extern "C" {
#include "game/mgdata.h"
}
extern "C" {
#include "game/charman.h"
}
extern "C" {
#include "game/process.h"
}
extern "C" {
#include "game/mg/timer.h"
}
extern "C" {
#include "game/mg/actman.h"
}
#include "stdarg.h"
#include "stdio.h"
}

#ifdef __MWERKS__
#define M517_RODATA __declspec(section ".rodata")
#else
#define M517_RODATA
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


// The inlined form of the assertion handler (singleton.h, model.cpp, ...).
#define M517_ASSERT_INL(cond, expr, file, line)                                      if (cond) {                                                                          (void)0;                                                                     } else {                                                                             char assertBuf[0x400];                                                           sprintf(assertBuf, lbl_1_data_94, (file), (line), (expr));                       OSPanic(lbl_1_data_B2, 0x12, assertBuf);                                     }

// ---------------------------------------------------------------- data

char lbl_1_data_80[] = "ptr != NULL";
char lbl_1_data_8C[] = "new.cpp";
char lbl_1_data_94[] = "\nASSERTION FAILED at %s:%d\n%s";
char lbl_1_data_B2[] = "assert.cpp";
char lbl_1_data_BD[] = "handle.cpp";
char lbl_1_data_C8[] = "n <= MAXCOUNT";
char lbl_1_data_D6[] = "singleton.h";
char lbl_1_data_E2[] = "ms_pointer == 0";
char lbl_1_data_F2[] = "ms_pointer != 0";
char lbl_1_data_102[] = "model.cpp";
char lbl_1_data_10C[] = "p != NULL";
char lbl_1_data_116[] = "om.cpp";
char lbl_1_data_11D[] = "subthread.cpp";
char lbl_1_data_12B[] = "::OSGetCurrentThread() == m_th";
char lbl_1_data_14A[] = "!::OSIsThreadTerminated(m_th)";
char lbl_1_data_168[] = "thread.cpp";
char lbl_1_data_173[] = "priority >= OS_PRIORITY_MIN && priority <= OS_PRIORITY_MAX";
char lbl_1_data_1AE[] = "OSCreateThread() failed.";
char lbl_1_data_1C7[] = "delete CThread@%p\nstack remain/total = 0x%08x/0x%08x (%f%%)\n";
char lbl_1_data_204[] = "stack overflow\n";
char lbl_1_data_214[] = "timer.cpp";
char lbl_1_data_21E[] = "false";

M517SubThread *fn_1_6D9C(M517SubThread *self, s16 flag);
M517Master *fn_1_3B84(M517Master *self, s16 flag);
void fn_1_3F24(M517Master *self);
M517Functor *fn_1_63EC(M517Functor *self, s16 flag);
void fn_1_6378(M517Functor *self, OMOBJ *obj);
void fn_1_632C(OMOBJ *obj);

M517ThreadVtbl lbl_1_data_224 = { { 0, 0 }, fn_1_6D9C, 0 };
M517FunctorVtbl lbl_1_data_234 = { { 0, 0 }, fn_1_63EC, fn_1_6378 };
M517MasterVtbl lbl_1_data_244 = { { 0, 0 }, fn_1_3B84, 0, fn_1_3F24, 0, 0 };

// ---------------------------------------------------------------- bss

u8 lbl_1_bss_18[0xC];
M517Node lbl_1_bss_24;
s32 lbl_1_bss_2C;
s32 lbl_1_bss_30;
s32 lbl_1_bss_34;
s32 lbl_1_bss_38;
M517Rand lbl_1_bss_3C;

// ---------------------------------------------------------------- code

void fn_1_2058(const char *expr, const char *file, s32 line);
int fn_1_20B8(void *unused, const char *fmt, ...);
void *fn_1_7AB8(M517Node *self, s16 flag);
void fn_1_228C(M517Cam *self, Vec *x, Vec *y, Vec *z);
void fn_1_23F0(M517Cam *self, Vec *delta);

void *fn_1_1EE8(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M517_ASSERT(p != 0, lbl_1_data_80, lbl_1_data_8C, 0x17);
    return p;
}

void fn_1_1F4C(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_1F84(void *ptr)
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

void fn_1_2058(const char *expr, const char *file, s32 line)
{
    char buf[0x40C];

    sprintf(buf, lbl_1_data_94, file, line, expr);
    OSPanic(lbl_1_data_B2, 0x12, buf);
}

int fn_1_20B8(void *unused, const char *fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = vprintf(fmt, ap);
    va_end(ap);
    return ret;
}

// Orbit: rotate about the camera X axis (pitch) then world Y (yaw).
void fn_1_215C(M517Cam *self, f32 pitch, f32 yaw)
{
    Vec ax, ay, az;
    Mtx rotA, rotB, rot;

    fn_1_228C(self, &ax, &ay, &az);
    PSMTXRotAxisRad(rotA, &ax, 0.017453292f * pitch);
    PSMTXRotRad(rotB, 'Y', 0.017453292f * yaw);
    PSMTXConcat(rotA, rotB, rot);
    PSMTXMultVecSR(rot, &az, &az);
    PSMTXMultVecSR(rot, &ay, &self->up);
    PSVECSubtract(&self->pos, &az, &self->target);
}

void fn_1_2220(M517Cam *self, f32 angle)
{
    Mtx m;
    Vec axis;

    PSVECSubtract(&self->target, &self->pos, &axis);
    PSMTXRotAxisRad(m, &axis, 0.017453292f * angle);
    PSMTXMultVec(m, &self->up, &self->up);
}

// Camera basis: the three rows of the look-at matrix.
void fn_1_228C(M517Cam *self, Vec *x, Vec *y, Vec *z)
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

void fn_1_2334(M517Cam *self, Mtx out)
{
    C_MTXLookAt(out, &self->pos, &self->up, &self->target);
    PSMTXInverse(out, out);
}

void fn_1_2388(M517Cam *self, Vec *local)
{
    Mtx m;
    Vec world;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    PSMTXMultVecSR(m, local, &world);
    fn_1_23F0(self, &world);
}

void fn_1_23F0(M517Cam *self, Vec *delta)
{
    PSVECAdd(delta, &self->target, &self->target);
    PSVECAdd(delta, &self->pos, &self->pos);
}

void fn_1_2444(M517Cam *self)
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

void fn_1_24D8(M517Cam *self, M517Pose *pose)
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

void fn_1_2578(M517Cam *self, M517Pose *out)
{
    Mtx m;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    out->pos.x = self->pos.x;
    out->pos.y = self->pos.y;
    out->pos.z = self->pos.z;
    C_QUATMtx(&out->rot, m);
}

void fn_1_25F0(M517Cam *self, M517Pose *a, M517Pose *b, f32 t)
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

void fn_1_2700(M517Cam *self, M517Cam *src)
{
    Vec delta;

    PSVECSubtract(&src->pos, &self->pos, &delta);
    self->pos.x = src->pos.x;
    self->pos.y = src->pos.y;
    self->pos.z = src->pos.z;
    PSVECAdd(&self->target, &delta, &self->target);
}

void fn_1_276C(M517Cam *self, Vec *a, Vec *b, f32 t)
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

void fn_1_2824(M517Cam *self, u16 camBit)
{
    Hu3DCameraPosSetV(camBit, &self->pos, &self->up, &self->target);
}

void fn_1_2864(M517Cam *self, u16 camBit)
{
    Hu3DCameraPosGet(camBit, &self->pos, &self->up, &self->target);
}

// ---------------------------------------------------------------- intrusive list


M517Node *fn_1_368C(M517Node *list);
M517Node *fn_1_36E0(M517Cmp cmp, M517Node *a, M517Node *b);
M517Node *fn_1_2FE4(M517Cmp cmp, M517Node *list);
void fn_1_3E28(OMOBJ *obj);
void fn_1_3F28(void);
void fn_1_46D0(void);
void fn_1_629C(void *ptr);

// ---------------------------------------------------------------- handle table



// The id word is a 20 bit slot index over a 12 bit use counter; PC code
// reads/writes the whole word so it does not depend on the bitfield layout.
#ifdef TARGET_PC
#define M517HANDLE_SETINDEX(e, i) ((e)->h.id = (u32)(i) << 12)
#define M517HANDLE_BUMP(e) ((e)->h.id = ((e)->h.id & ~0xFFFu) | (((e)->h.id & 0xFFFu) + 1 & 0xFFFu))
#define M517HANDLE_CLEARCOUNT(e) ((e)->h.id &= ~0xFFFu)
#else
#define M517HANDLE_SETINDEX(e, i) ((e)->h.b.index = (i))
#define M517HANDLE_BUMP(e) ((e)->h.b.count = (e)->h.b.count + 1)
#define M517HANDLE_CLEARCOUNT(e) ((e)->h.b.count = 0)
#endif

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
M517HandleTbl *fn_1_28A4(M517HandleTbl *self, u32 n)
{
    M517HandleEnt *ents;
    M517HandleEnt *p;
    u32 i;

    M517_ASSERT_INL(n <= 0x1000, lbl_1_data_C8, lbl_1_data_BD, 0x44);
    self->max = n;
    p = (M517HandleEnt *)HuMemDirectMallocNum(HEAP_HEAP, (n + 1) * sizeof(M517HandleEnt), 0x10000000);
    M517_ASSERT_INL(p != 0, lbl_1_data_80, lbl_1_data_8C, 0x17);
    self->ents = p;
    ents = self->ents;
    self->tail = ents + n;
    self->tail->next = self->tail;
    self->tail->prev = self->tail;
    for (i = 1; i < n; i++) {
        self->ents[i].next = &self->ents[i + 1];
        self->ents[i].prev = &self->ents[i - 1];
        self->ents[i].obj = 0;
        M517HANDLE_SETINDEX(&self->ents[i], i);
        M517HANDLE_CLEARCOUNT(&self->ents[i]);
    }
    self->ents[1].prev = self->ents;
    self->ents[n - 1].next = self->ents;
    self->ents->next = self->ents + 1;
    self->ents->prev = self->ents + (n - 1);
    return self;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
M517HandleTbl *fn_1_2AA8(M517HandleTbl *self, s16 flag)
{
    if (self != 0) {
        void *ents = self->ents;
        if (ents != 0) {
            HuMemDirectFree(ents);
        }
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
u32 fn_1_2B18(M517HandleTbl *self, void *obj)
{
    M517HandleEnt *e = self->ents->next;

    if (e == self->ents) {
        return 0;
    }
    e->next->prev = e->prev;
    e->prev->next = e->next;
    e->next = self->tail->next;
    e->prev = self->tail;
    self->tail->next->prev = e;
    self->tail->next = e;
    e->obj = obj;
    M517HANDLE_BUMP(e);
    return e->h.id;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void *fn_1_2BA8(M517HandleTbl *self, u32 id)
{
    M517HandleEnt *e;
    void *obj;

    if (id == 0) {
        return 0;
    }
    if ((id >> 12) >= self->max) {
        return 0;
    }
    e = self->ents + (id >> 12);
    if (e->h.id != id) {
        return 0;
    }
    obj = e->obj;
    e->obj = 0;
    e->prev->next = e->next;
    e->next->prev = e->prev;
    e->next = self->ents;
    e->prev = self->ents->prev;
    self->ents->prev->next = e;
    self->ents->prev = e;
    return obj;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void *fn_1_2C6C(M517HandleTbl *self, u32 id)
{
    M517HandleEnt *e;

    if (id == 0) {
        return 0;
    }
    if ((id >> 12) >= self->max) {
        return 0;
    }
    e = self->ents + (id >> 12);
    if (e->h.id != id) {
        return 0;
    }
    return e->obj;
}

s32 fn_1_2CDC(M517Node *self)
{
    s32 n = 0;
    M517Node *p = self->next;

    while (p != self) {
        p = p->next;
        n++;
    }
    return n;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
s32 fn_1_2D18(M517HandleTbl *self)
{
    M517HandleEnt *tail = self->tail;
    s32 n = 0;
    M517HandleEnt *p = tail->next;

    while (p != tail) {
        p = p->next;
        n++;
    }
    return n;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
s32 fn_1_2D60(M517HandleTbl *self)
{
    M517HandleEnt *head = self->ents;
    s32 n = 0;
    M517HandleEnt *p = head->next;

    while (p != head) {
        p = p->next;
        n++;
    }
    return n;
}

void fn_1_2DA8(M517Node *self)
{
    self->next = self;
    self->prev = self;
}

void *fn_1_2DB4(M517Node *self, s16 flag)
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

void fn_1_2E20(M517Node *self)
{
    self->next->prev = self->prev;
    self->prev->next = self->next;
    self->next = self;
    self->prev = self;
}

// Insert `self` right after `other`.
void fn_1_2E44(M517Node *self, M517Node *other)
{
    M517Node *at = other;

    self->prev = at;
    self->next = at->next;
    at->next->prev = self;
    at->next = self;
}

// Insert `self` right before `other`.
void fn_1_2E74(M517Node *self, M517Node *other)
{
    M517Node *at = other;

    self->next = at;
    self->prev = at->prev;
    at->prev->next = self;
    at->prev = self;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
// Move every node of `self` in front of `other`.
void fn_1_2EA4(M517Node *self, M517Node *other)
{
    M517Node *first;
    M517Node *last;
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
void fn_1_2F2C(M517Node *self, M517Cmp cmp)
{
    M517Node *node;
    M517Node *prev;
    u8 empty;

    empty = 0;
    if (self->next == self && self->next == self->prev) {
        empty = 1;
    }
    if (empty == 0) {
        self->prev->next = 0;
        node = fn_1_2FE4(cmp, self->next);
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

// NONMATCHING: the original inlines its own recursion several levels deep
M517Node *fn_1_2FE4(M517Cmp cmp, M517Node *list)
{
    M517Node *second;
    M517Node *a;
    M517Node *b;

    if (list == 0) {
        return 0;
    }
    if (list->next == 0) {
        return list;
    }
    second = fn_1_368C(list);
    a = fn_1_2FE4(cmp, list);
    b = fn_1_2FE4(cmp, second);
    return fn_1_36E0(cmp, a, b);
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
// Cut the list in the middle and return the second half.
M517Node *fn_1_368C(M517Node *list)
{
    M517Node *fast;
    M517Node *second;

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

M517Node *fn_1_36E0(M517Cmp cmp, M517Node *a, M517Node *b)
{
    M517Node head;
    M517Node *tail;

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

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void fn_1_378C(s32 bit)
{
    M517Task *t;
    M517Node *end;
    M517Node *b;
    M517Node *c;
    M517Node *nx;

    b = lbl_1_bss_24.next;
    c = b;
    t = (M517Task *)c;
    lbl_1_bss_2C = bit;
    end = &lbl_1_bss_24;
    while ((M517Node *)t != end) {
        t->flags |= 1 << bit;
        t->vtbl->onFlag(t, bit);
        nx = t->link.next;
        t = (M517Task *)nx;
    }
}

// NONMATCHING: flag test compiles as cmpwi instead of cmplwi
s32 fn_1_3824(void)
{
    M517Task *t;
    M517Node *end;
    M517Node *b;
    M517Node *c;
    M517Node *nx;
    s32 bit;

    b = lbl_1_bss_24.next;
    c = b;
    t = (M517Task *)c;
    end = &lbl_1_bss_24;
    while ((M517Node *)t != end) {
        bit = lbl_1_bss_2C;
        if ((t->flags & (1 << bit)) != 0) {
            return 0;
        }
        nx = t->link.next;
        t = (M517Task *)nx;
    }
    return 1;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void fn_1_38B0(void)
{
    M517Task *t;
    M517Node *end;
    M517Node *b;
    M517Node *c;
    M517Node *nx;

    b = lbl_1_bss_24.next;
    c = b;
    t = (M517Task *)c;
    end = &lbl_1_bss_24;
    while ((M517Node *)t != end) {
        t->vtbl->update(t);
        nx = t->link.next;
        t = (M517Task *)nx;
    }
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
// Destroy every registered task, last one first.
void fn_1_3920(void)
{
    M517Task *t;
    u8 empty;

    while (1) {
        empty = 0;
        if (lbl_1_bss_24.next == &lbl_1_bss_24 && lbl_1_bss_24.next == lbl_1_bss_24.prev) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        t = (M517Task *)lbl_1_bss_24.prev;
        if (t != 0) {
            t->vtbl->dtor(t, 1);
        }
    }
}

s32 fn_1_39E8(void)
{
    return lbl_1_bss_2C;
}

// ---------------------------------------------------------------- game master

M517Master *fn_1_39F8(M517Master *self)
{
    s32 i;

    M517_ASSERT_INL(lbl_1_bss_A04 == 0, lbl_1_data_E2, lbl_1_data_D6, 0x35);
    lbl_1_bss_A04 = self;
    self->vtbl = &lbl_1_data_244;
    self->counter = 0;
    i = 0;
    while (i < 4) {
        self->slot[i] = -1;
        i++;
    }
    return self;
}

M517Master *fn_1_3AC8(M517Master *self, s16 flag)
{
    if (self != 0) {
        M517_ASSERT_INL(lbl_1_bss_A04 != 0, lbl_1_data_F2, lbl_1_data_D6, 0x3D);
        lbl_1_bss_A04 = 0;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
M517Master *fn_1_3B84(M517Master *self, s16 flag)
{
    M517Master *m;
    s32 i;
    s32 p;

    if (self != 0) {
        self->vtbl = &lbl_1_data_244;
        if (mgDataTbl[GwSystem.mgNo].type != 3) {
            M517_ASSERT_INL(lbl_1_bss_A04 != 0, lbl_1_data_F2, lbl_1_data_D6, 0x27);
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
            M517_ASSERT_INL(lbl_1_bss_A04 != 0, lbl_1_data_F2, lbl_1_data_D6, 0x3D);
            lbl_1_bss_A04 = 0;
        }
        if (flag > 0) {
            fn_1_629C(self);
        }
    }
    return self;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void fn_1_3D3C(M517Master *self, s32 objMax, s32 prio)
{
    OMOBJMAN *om;

    om = omInitObjMan((s16)objMax, prio);
    omGameSysInit(om);
    self->vtbl->setup(self, om);
    omAddObjEx(om, 0, 0, 0, -1, fn_1_3E28);
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void fn_1_3DC0(M517Master *self)
{
    OMOBJMAN *om;

    om = MgActorObjectSetup();
    self->vtbl->setup(self, om);
    omAddObjEx(om, 0, 0, 0, -1, fn_1_3E28);
}

void fn_1_3EB8(M517Master *self);

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void fn_1_3E28(OMOBJ *obj)
{
    OMOBJ *self;
    M517Master *m;
    M517Master *cur;

    self = obj;
    if (omSysExitReq != 0 && WipeCheckEnd() == 0) {
        fn_1_46D0();
        HuAudFadeOut(0x3E8);
        self->objFunc = (OMOBJFUNC)fn_1_3F28;
        return;
    }
    m = lbl_1_bss_A04;
    cur = m;
    fn_1_3EB8(cur);
}

// NONMATCHING: the unused zero test is a cntlzw sequence here
void fn_1_3EB8(M517Master *self)
{
    self->vtbl->update(self);
    if (self->counter > 0) {
        if (--self->counter == 0) {
        }
    }
}

void fn_1_3F24(M517Master *self)
{
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void fn_1_3F28(void)
{
    M517Master *m;
    M517Master *cur;
    M517Master *obj;
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

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
// Register a player in the first free slot.
void fn_1_4014(s32 player)
{
    s32 *slots;
    s32 i;

    M517_ASSERT_INL(lbl_1_bss_A04 != 0, lbl_1_data_F2, lbl_1_data_D6, 0x27);
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

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
s32 fn_1_40E8(s32 player)
{
    s32 i;

    M517_ASSERT_INL(lbl_1_bss_A04 != 0, lbl_1_data_F2, lbl_1_data_D6, 0x27);
    i = 0;
    while (i < 4) {
        if (player == lbl_1_bss_A04->slot[i]) {
            return 1;
        }
        i++;
    }
    return 0;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void fn_1_41BC(void)
{
    M517Master *m;

    M517_ASSERT_INL(lbl_1_bss_A04 != 0, lbl_1_data_F2, lbl_1_data_D6, 0x27);
    m = lbl_1_bss_A04;
    m->mesId = GameMesCreate(2, 0);
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void fn_1_4274(void)
{
    M517Master *m;

    M517_ASSERT_INL(lbl_1_bss_A04 != 0, lbl_1_data_F2, lbl_1_data_D6, 0x27);
    m = lbl_1_bss_A04;
    m->mesId = GameMesCreate(2, 1);
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
s32 fn_1_432C(void)
{
    M517Master *m;
    s32 ret;

    M517_ASSERT_INL(lbl_1_bss_A04 != 0, lbl_1_data_F2, lbl_1_data_D6, 0x27);
    m = lbl_1_bss_A04;
    ret = 0;
    if (GameMesStatGet(-1) == 0 && (u8)(m->counter <= 0) != 0) {
        ret = 1;
    }
    return ret;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
// Announce the result: sort the registered players and print their characters.
void fn_1_4400(void)
{
    M517Master *m;
    s32 *slots;
    s32 chars[4];
    s32 n;
    s32 i;
    s32 j;
    s32 k;
    s32 tmp;
    s16 charNo;

    M517_ASSERT_INL(lbl_1_bss_A04 != 0, lbl_1_data_F2, lbl_1_data_D6, 0x27);
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

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void fn_1_45CC(s32 mes)
{
    M517Master *m;

    if (_CheckFlag(0x1000F) == 0) {
        M517_ASSERT_INL(lbl_1_bss_A04 != 0, lbl_1_data_F2, lbl_1_data_D6, 0x27);
        m = lbl_1_bss_A04;
        m->mesId = GameMesCreate(0xE, mes);
        HuAudJinglePlay(0x86);
    }
}

void fn_1_46A4(void)
{
    WipeCreate(1, 5, 0x3C);
}

void fn_1_46D0(void)
{
    WipeCreate(2, 6, 0x3C);
}

void fn_1_46FC(void)
{
    omSysExitReq = 1;
}

// ---------------------------------------------------------------- Hu3D model wrappers

M517Model *fn_1_4710(M517Model *self, void *data)
{
    self->id = Hu3DModelCreate(data);
    return self;
}

M517Model *fn_1_474C(M517Model *self, s32 dataNum)
{
    self->id = Hu3DModelCreateData(dataNum);
    return self;
}

M517Model *fn_1_4794(M517Model *self, HU3DMODELHOOK hook)
{
    self->id = Hu3DHookFuncCreate(hook);
    return self;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
// Build a character model; `type` selects the model variant bit.
M517Model *fn_1_47D0(M517Model *self, s32 charNo, s32 type)
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

inline s32 M517ModelIdGet(M517Model *m)
{
    return m->id;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
M517Model *fn_1_486C(M517Model *self, M517Model *parent)
{
    self->id = Hu3DModelLink(M517ModelIdGet(parent));
    return self;
}

void fn_1_48C0(M517Model *self, u32 attr)
{
    Hu3DModelAttrSet(self->id, attr);
}

void fn_1_48F8(M517Model *self, u32 attr)
{
    Hu3DModelAttrReset(self->id, attr);
}

// Apply an object's transform (translation, rotation, scale) to the model.
void fn_1_4930(M517Model *self, OMOBJ *obj)
{
    HU3DMODELID id;

    id = self->id;
    Hu3DModelPosSet(id, obj->trans.x, obj->trans.y, obj->trans.z);
    Hu3DModelRotSet(id, obj->rot.x, obj->rot.y, obj->rot.z);
    Hu3DModelScaleSet(id, obj->scale.x, obj->scale.y, obj->scale.z);
}

void fn_1_49AC(M517Model *self, Vec *pos)
{
    Hu3DModelPosSet(self->id, pos->x, pos->y, pos->z);
}

void fn_1_49F4(M517Model *self, Vec *rot)
{
    Hu3DModelRotSet(self->id, rot->x, rot->y, rot->z);
}

void fn_1_4A3C(M517Model *self, Vec *scale)
{
    Hu3DModelScaleSet(self->id, scale->x, scale->y, scale->z);
}

void fn_1_4A84(M517Model *self, f32 scale)
{
    Hu3DModelScaleSet(self->id, scale, scale, scale);
}

void fn_1_4ACC(M517Model *self, f32 tpLvl)
{
    Hu3DModelTPLvlSet(self->id, tpLvl);
}

void fn_1_4B04(M517Model *self, s16 layer)
{
    Hu3DModelLayerSet(self->id, layer);
}

void fn_1_4B3C(M517Model *self, u16 camBit)
{
    Hu3DModelCameraSet(self->id, camBit);
}

void fn_1_4B74(M517Model *self, void *data)
{
    Hu3DData[self->id].hookData = data;
}

MtxPtr fn_1_4B90(M517Model *self)
{
    return Hu3DData[self->id].mtx;
}

M517Model *fn_1_4BAC(M517Model *self, s16 flag)
{
    if (self != 0) {
        Hu3DModelKill(self->id);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

inline HSFOBJECT *M517ObjGet(M517Model *self, char *name)
{
    HSFOBJECT *obj;

    obj = Hu3DModelObjPtrGet(self->id, name);
    M517_ASSERT_INL(obj != 0, lbl_1_data_10C, lbl_1_data_102, 0x8A);
    return obj;
}

HSFOBJECT *fn_1_4C08(M517Model *self, char *name)
{
    return M517ObjGet(self, name);
}

void fn_1_4C9C(M517Model *self)
{
    Hu3DModelShadowSet(self->id);
}

void fn_1_4CCC(M517Model *self)
{
    Hu3DModelShadowMapSet(self->id);
}

void fn_1_4CFC(M517Model *self, char *name)
{
    Hu3DModelShadowMapObjSet(self->id, name);
}

void fn_1_4D34(M517Model *self)
{
    Hu3DModelShadowMapObjReset(self->id);
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void fn_1_4D64(M517Model *self, char *name)
{
    HSFOBJECT *obj;
    u32 *flags;

    obj = M517ObjGet(self, name);
    flags = (u32 *)obj->constData;
    *flags &= ~8;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void fn_1_4E10(M517Model *self, char *name)
{
    HSFOBJECT *obj;

    obj = M517ObjGet(self, name);
    obj->flags |= 0x400;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void fn_1_4EB8(M517Model *self, char *name)
{
    HSFOBJECT *obj;

    obj = M517ObjGet(self, name);
    obj->flags &= ~0x400;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void fn_1_4F60(M517Model *self, char *name)
{
    HSFOBJECT *obj;
    HSFTRANSFORM *base;

    obj = M517ObjGet(self, name);
    obj->flags |= 0x400;
    base = &obj->mesh.base;
    base->scale.x = 0.0f;
    base->scale.y = 0.0f;
    base->scale.z = 0.0f;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void fn_1_503C(M517Model *self, char *name)
{
    HSFOBJECT *obj;
    HSFTRANSFORM *base;

    obj = M517ObjGet(self, name);
    obj->flags &= ~0x400;
    base = &obj->mesh.base;
    base->scale.x = 1.0f;
    base->scale.y = 1.0f;
    base->scale.z = 1.0f;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void fn_1_5118(M517Model *self, s32 time)
{
    Hu3DMotionTimeSet(M517ModelIdGet(self), (f32)time);
}

// ---------------------------------------------------------------- Mersenne Twister

extern const u32 lbl_1_rodata_B0[2] = { 0, 0x9908B0DF };

void fn_1_5184(M517Rand *r, u32 seed)
{
    r->mt[0] = seed;
    r->idx = 1;
    while (r->idx < 0x270) {
        r->mt[r->idx] = r->idx + (((r->mt[r->idx - 1]) ^ ((r->mt[r->idx - 1]) >> 30)) * 0x6C078965);
        r->idx++;
    }
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void fn_1_51F8(M517Rand *r, u32 *key, u32 len)
{
    u32 i;
    u32 j;
    u32 k;

    fn_1_5184(r, 19650218);
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

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
u32 fn_1_53BC(M517Rand *r)
{
    u32 k;
    u32 y;

    if (r->idx >= 0x270) {
        k = 0;
        while (k < 0xE3) {
            y = (r->mt[k] & 0x80000000) | (r->mt[k + 1] & 0x7FFFFFFF);
            r->mt[k] = lbl_1_rodata_B0[y & 1] ^ ((y >> 1) ^ r->mt[k + 0x18D]);
            k++;
        }
        while (k < 0x26F) {
            y = (r->mt[k] & 0x80000000) | (r->mt[k + 1] & 0x7FFFFFFF);
            r->mt[k] = lbl_1_rodata_B0[y & 1] ^ ((y >> 1) ^ r->mt[k - 0xE3]);
            k++;
        }
        y = (r->mt[0x26F] & 0x80000000) | (r->mt[0] & 0x7FFFFFFF);
        r->mt[0x26F] = (y >> 1) ^ r->mt[0x18C] ^ lbl_1_rodata_B0[y & 1];
        r->idx = 0;
    }
    y = r->mt[r->idx++];
    y ^= y >> 11;
    y ^= (y << 7) & 0x9D2C5680;
    y ^= (y << 15) & 0xEFC60000;
    return y ^ (y >> 18);
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
u32 fn_1_5548(M517Rand *r)
{
    return fn_1_53BC(r) >> 1;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
f32 fn_1_56E0(M517Rand *r)
{
    return 2.3283064e-10f * (f32)fn_1_53BC(r);
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
f32 fn_1_58A4(M517Rand *r)
{
    return 2.3283064e-10f * (f32)fn_1_53BC(r);
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
f32 fn_1_5A68(M517Rand *r)
{
    return 2.3283064e-10f * (0.5f + (f32)fn_1_53BC(r));
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
f64 fn_1_5C3C(M517Rand *r)
{
    u32 a;
    u32 b;

    a = fn_1_53BC(r) >> 5;
    b = fn_1_53BC(r) >> 6;
    return 1.1102230246251565e-16 * ((67108864.0 * (f64)a) + (f64)b);
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
f32 fn_1_5FB4(M517Rand *r)
{
    return 1.1920929e-7f * (f32)(fn_1_53BC(r) >> 9);
}

// ---------------------------------------------------------------- allocation / functor objects

void *fn_1_6184(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M517_ASSERT_INL(p != 0, lbl_1_data_80, lbl_1_data_116, 0x18);
    return p;
}

void *fn_1_6210(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M517_ASSERT_INL(p != 0, lbl_1_data_80, lbl_1_data_116, 0x1F);
    return p;
}

void fn_1_629C(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_62D4(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

// Functor owned by an object: the object's data slot points at it.
void fn_1_630C(M517Functor *self, OMOBJ *obj)
{
    self->vtbl = &lbl_1_data_234;
    obj->data = self;
    obj->objFunc = fn_1_632C;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void fn_1_632C(OMOBJ *obj)
{
    M517Functor *self;

    self = (M517Functor *)obj->data;
    self->vtbl->run(self, obj);
}

void fn_1_6378(M517Functor *self, OMOBJ *obj)
{
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void fn_1_637C(OMOBJ *obj)
{
    M517Functor *fn;
    M517Functor *p;

    fn = (M517Functor *)obj->data;
    p = fn;
    if (p != 0) {
        p = p->vtbl->dtor(p, 1);
    }
    omDelObjEx(HuPrcCurrentGet(), obj);
}

M517Functor *fn_1_63EC(M517Functor *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_234;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void fn_1_644C(OMOBJMAN *man, OMOBJ *obj)
{
    M517Functor *fn;
    M517Functor *p;

    fn = (M517Functor *)obj->data;
    p = fn;
    if (p != 0) {
        p = p->vtbl->dtor(p, 1);
    }
    omDelObjEx(man, obj);
}

// ---------------------------------------------------------------- global random number generator

void fn_1_64C0(u32 seed)
{
    fn_1_5184(&lbl_1_bss_3C, seed);
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
u32 fn_1_6598(void)
{
    return fn_1_53BC(&lbl_1_bss_3C) >> 1;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
f32 fn_1_67D4(void)
{
    return 1.1920929e-7f * (f32)(fn_1_53BC(&lbl_1_bss_3C) >> 9);
}

f32 fn_1_6A48(f32 scale)
{
    return scale * fn_1_5C3C(&lbl_1_bss_3C);
}

f32 fn_1_6A80(f32 from, f32 to)
{
    return from + (to - from) * fn_1_5C3C(&lbl_1_bss_3C);
}

// ---------------------------------------------------------------- shadow / collision helpers

// NONMATCHING: register allocation, result is clrlwi-ed
// Sphere overlap test: true when the distance between centres is below the radius sum.
s32 fn_1_6AD4(M517Sphere *a, M517Sphere *b)
{
    Vec d;
    f32 sum;
    f32 dist;

    PSVECSubtract(&a->pos, &b->pos, &d);
    sum = a->radius + b->radius;
    dist = PSVECMag(&d);
    return dist < sum;
}


// Sphere: centre and radius.


inline f32 M517SphereRadius(M517Sphere *s)
{
    return s->radius;
}


// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
// Push two overlapping spheres apart; ka/kb weight the move of each.
s32 fn_1_6B7C(M517Sphere *a, M517Sphere *b, f32 ka, f32 kb)
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

s32 fn_1_6CFC(void)
{
    return OSGetThreadPriority(OSGetCurrentThread());
}

void *fn_1_6FE4(void *arg);
M517Thread *fn_1_7040(M517Thread *th, void *(*func)(void *), void *arg, u32 size, s32 prio, u16 attr);

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
M517SubThread *fn_1_6D20(M517SubThread *self, u32 stackSize)
{
    self->vtbl = &lbl_1_data_224;
    fn_1_7040(&self->th, fn_1_6FE4, self, stackSize, OSGetThreadPriority(OSGetCurrentThread()), 1);
    OSInitSemaphore(&self->sem, 0);
    return self;
}

M517Thread *fn_1_72C4(M517Thread *th, s16 flag);

M517SubThread *fn_1_6D9C(M517SubThread *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_224;
        fn_1_72C4(&self->th, -1);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

// CThread::IsTerminated(), inlined with its own `this`.
inline u8 M517ThreadDone(M517Thread *th)
{
    return OSIsThreadTerminated(&th->thread) != 0;
}

u8 fn_1_6E08(M517SubThread *self)
{
    M517Thread *th = &self->th;

    return OSIsThreadTerminated(&th->thread) != 0;
}

u32 fn_1_73F8(M517Thread *th);

s32 fn_1_6E50(M517SubThread *self, s32 arg)
{
    M517Thread *thIsDone;
    M517Thread *thResume;
    M517Sem *sem;

    thIsDone = &self->th;
    if (OSIsThreadTerminated(&thIsDone->thread) != 0) {
        return self->arg;
    }
    self->arg = arg;
    thResume = &self->th;
    OSResumeThread(&thResume->thread);
    sem = &self->sem;
    OSWaitSemaphore(sem);
    fn_1_73F8(&self->th);
    OSCheckActiveThreads();
    return self->arg;
}

s32 fn_1_6EE0(M517SubThread *self, s32 arg)
{
    M517Thread *thCur;
    M517Thread *thDone;
    M517Sem *sem;
    M517Thread *thSuspend;

    thCur = &self->th;
    M517_ASSERT_INL(OSGetCurrentThread() == &thCur->thread, lbl_1_data_12B, lbl_1_data_11D, 0x47);
    thDone = &self->th;
    M517_ASSERT_INL(OSIsThreadTerminated(&thDone->thread) == 0, lbl_1_data_14A, lbl_1_data_11D, 0x4A);
    self->arg = arg;
    sem = &self->sem;
    OSSignalSemaphore(sem);
    thSuspend = &self->th;
    OSSuspendThread(&thSuspend->thread);
    return self->arg;
}

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void *fn_1_6FE4(void *arg)
{
    M517SubThread *self;
    M517Sem *sem;

    self = (M517SubThread *)arg;
    self->vtbl->run(self, self->arg);
    sem = &self->sem;
    OSSignalSemaphore(sem);
    return 0;
}

void fn_1_71A0(M517Thread *th, void *(*func)(void *), void *arg, u32 *stack, u32 words, s32 prio, u16 attr);

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
M517Thread *fn_1_7040(M517Thread *th, void *(*func)(void *), void *arg, u32 size, s32 prio, u16 attr)
{
    u32 words;
    u32 *stack;

    words = size >> 2;
    stack = (u32 *)HuMemDirectMallocNum(HEAP_HEAP, words * 4, 0x10000000);
    M517_ASSERT_INL(stack != 0, lbl_1_data_80, lbl_1_data_8C, 0x17);
    th->stack = stack;
    fn_1_71A0(th, func, arg, stack, words, prio, attr);
    return th;
}

M517Thread *fn_1_7124(M517Thread *th, void *(*func)(void *), void *arg, u32 *stack, u32 size, s32 prio, u16 attr)
{
    u32 words;

    th->stack = 0;
    words = size >> 2;
    fn_1_71A0(th, func, arg, stack, words, prio, attr);
    return th;
}

void fn_1_750C(u32 *p, u32 words);
s32 fn_1_7498(u32 *p, s32 mode);

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void fn_1_71A0(M517Thread *th, void *(*func)(void *), void *arg, u32 *stack, u32 words, s32 prio, u16 attr)
{
    u8 ok;

    ok = 0;
    if (prio >= 0 && prio <= 0x1F) {
        ok = 1;
    }
    M517_ASSERT_INL(ok != 0, lbl_1_data_173, lbl_1_data_168, 0x52);
    fn_1_750C(stack, words);
    fn_1_7498(stack, 0);
    if ((u8)(OSCreateThread(&th->thread, func, arg, &stack[words], words * 4, prio, attr) != 0) == 0) {
        OSPanic(lbl_1_data_168, 0x60, lbl_1_data_1AE);
    }
}

M517Thread *fn_1_72C4(M517Thread *th, s16 flag)
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
            remain = fn_1_73F8(th);
            printf(lbl_1_data_1C7, th, remain, total, (100.0 * (f64)remain) / (f64)total);
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

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
u32 fn_1_73F8(M517Thread *th)
{
    u32 *base;
    u32 *p;

    base = (u32 *)th->thread.stackEnd;
    if (fn_1_7498(base, 1) != 0) {
        OSPanic(lbl_1_data_168, 0x7F, lbl_1_data_204);
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

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
// Stack guard: mode 0 fills the 32 byte guard, mode 1 checks it.
s32 fn_1_7498(u32 *p, s32 mode)
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

void fn_1_750C(u32 *p, u32 words)
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

M517TimerObj *fn_1_7558(M517TimerObj *self, s32 type)
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
            M517_ASSERT_INL(0, lbl_1_data_21E, lbl_1_data_214, 0x1E);
            break;
    }
    return self;
}

M517TimerObj *fn_1_7620(M517TimerObj *self, s16 flag)
{
    if (self != 0) {
        MgTimerKill(self->timer);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_767C(M517TimerObj *self, s32 maxTime, s32 endTime, s32 recordTime)
{
    MgTimerParamSet(self->timer, maxTime, endTime, recordTime);
}

void fn_1_76C4(M517TimerObj *self)
{
    MgTimerValueGet(self->timer);
}

void fn_1_76F4(M517TimerObj *self, f32 x, f32 y)
{
    MgTimerPosSet(self->timer, x, y);
}

void fn_1_7734(M517TimerObj *self, f32 *x, f32 *y)
{
    MgTimerPosGet(self->timer, x, y);
}

void fn_1_7774(M517TimerObj *self, s32 offType)
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
            M517_ASSERT_INL(0, lbl_1_data_21E, lbl_1_data_214, 0x48);
            return;
    }
}

void fn_1_7838(M517TimerObj *self)
{
    MgTimerModeOffSet(self->timer);
}

// Stop the timer the first time it is asked to.
void fn_1_7868(M517TimerObj *self)
{
    if (self->timer->stopF == 0) {
        self->timer->stopF = 1;
        self->timer->mode = 1;
    }
}

void fn_1_7894(M517TimerObj *self)
{
    MgTimerKill(self->timer);
    self->timer = MgTimerCreate(0);
}

u8 fn_1_78D0(M517TimerObj *self)
{
    return MgTimerDoneCheck(self->timer) != 0;
}

void fn_1_7910(M517TimerObj *self, u8 r, u8 g, u8 b)
{
    MgTimerWinColorSet(self->timer, r, g, b);
}

// ---------------------------------------------------------------- static initialisers

// NONMATCHING: verified equivalent, MWCC register allocation / scheduling / inlining differs
void fn_1_7958(void)
{
    M517Node *node = &lbl_1_bss_24;
    M517Node *obj;
    node->next = node;
    node->prev = node;
    obj = node;
#ifdef __MWERKS__
    __register_global_object(obj, (void *)fn_1_7AB8, lbl_1_bss_18);
#endif
    lbl_1_bss_30 = 0x78;
    lbl_1_bss_34 = 0xD2;
    lbl_1_bss_38 = 0xD2;
    fn_1_5184(&lbl_1_bss_3C, 0x1571);
}

void *fn_1_7AB8(M517Node *self, s16 flag)
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
__declspec(section ".ctors") void (*const lbl_ctor_61C4)(void) = fn_1_7958;
#else
struct M517StaticInit {
    M517StaticInit() { fn_1_7958(); }
};
static M517StaticInit lbl_static_init;
#endif
}
