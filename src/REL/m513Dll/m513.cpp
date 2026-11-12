// m513Dll "m513": the module's runtime layer -- camera pose maths, an intrusive
// list of tasks, the game-master singleton plumbing, thin Hu3D/MgTimer wrappers,
// a Mersenne Twister, a sub-thread class and the C++ static initialisers.
#define _MATH_H
extern "C" {
#include "REL/m513Dll.h"
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
#define M513_RODATA __declspec(section ".rodata")
#else
#define M513_RODATA
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


// The inlined form of the assertion handler (singleton.h, model.cpp, ...).
#define M513_ASSERT_INL(cond, expr, file, line)                                      if (cond) {                                                                          (void)0;                                                                     } else {                                                                             char assertBuf[0x400];                                                           sprintf(assertBuf, lbl_1_data_14, (file), (line), (expr));                       OSPanic(lbl_1_data_32, 0x12, assertBuf);                                     }

// ---------------------------------------------------------------- data

char lbl_1_data_0[] = "ptr != NULL";
char lbl_1_data_C[] = "new.cpp";
char lbl_1_data_14[] = "\nASSERTION FAILED at %s:%d\n%s";
char lbl_1_data_32[] = "assert.cpp";
char lbl_1_data_3D[] = "{{0x%08x,0x%08x,0x%08x},{0x%08x,0x%08x,0x%08x,0x%08x}},\n";
char lbl_1_data_76[] = "handle.cpp";
char lbl_1_data_81[] = "n <= MAXCOUNT";
char lbl_1_data_8F[] = "singleton.h";
char lbl_1_data_9B[] = "ms_pointer == 0";
char lbl_1_data_AB[] = "ms_pointer != 0";
char lbl_1_data_BB[] = "model.cpp";
char lbl_1_data_C5[] = "p != NULL";
char lbl_1_data_CF[] = "om.cpp";
char lbl_1_data_D6[] = "subthread.cpp";
char lbl_1_data_E4[] = "::OSGetCurrentThread() == m_th";
char lbl_1_data_103[] = "!::OSIsThreadTerminated(m_th)";
char lbl_1_data_121[] = "thread.cpp";
char lbl_1_data_12C[] = "priority >= OS_PRIORITY_MIN && priority <= OS_PRIORITY_MAX";
char lbl_1_data_167[] = "OSCreateThread() failed.";
char lbl_1_data_180[] = "delete CThread@%p\nstack remain/total = 0x%08x/0x%08x (%f%%)\n";
char lbl_1_data_1BD[] = "stack overflow\n";
char lbl_1_data_1CD[] = "timer.cpp";
char lbl_1_data_1D7[] = "false";

M513SubThread *fn_1_57E8(M513SubThread *self, s16 flag);
M513Task *fn_1_23AC(M513Task *self, s16 flag);
void fn_1_22E0(M513Task *self);
M513Master *fn_1_25D0(M513Master *self, s16 flag);
void fn_1_2970(M513Master *self);
M513Functor *fn_1_4E38(M513Functor *self, s16 flag);
void fn_1_4DC4(M513Functor *self, OMOBJ *obj);
void fn_1_4D78(OMOBJ *obj);

M513ThreadVtbl lbl_1_data_1E0 = { { 0, 0 }, fn_1_57E8, 0 };
M513FunctorVtbl lbl_1_data_1F0 = { { 0, 0 }, fn_1_4E38, fn_1_4DC4 };
M513MasterVtbl lbl_1_data_200 = { { 0, 0 }, fn_1_25D0, 0, fn_1_2970, 0 };
void fn_1_1220C(void);
void fn_1_12208(void);

M513TaskVtblEx lbl_1_data_218 = { { { 0, 0 }, fn_1_23AC, 0, fn_1_22E0, 0 }, { { 0, -1, fn_1_1220C }, { 0, -1, fn_1_12208 } } };

// ---------------------------------------------------------------- bss

u8 lbl_1_bss_0[0xC];
M513Node lbl_1_bss_C;
s32 lbl_1_bss_14;
s32 lbl_1_bss_18;
s32 lbl_1_bss_1C;
s32 lbl_1_bss_20;
M513Rand lbl_1_bss_24;
u8 lbl_1_bss_9E8;
M513Master *lbl_1_bss_9EC;
M513IndHook *lbl_1_bss_9F0;
s32 lbl_1_bss_9F4;

// ---------------------------------------------------------------- code

void fn_1_1D4(const char *expr, const char *file, s32 line);
int fn_1_234(void *unused, const char *fmt, ...);
void *fn_1_6504(M513Node *self, s16 flag);
void fn_1_408(M513Cam *self, Vec *x, Vec *y, Vec *z);
void fn_1_56C(M513Cam *self, Vec *delta);

void *fn_1_0(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M513_ASSERT(p != 0, lbl_1_data_0, lbl_1_data_C, 0x17);
    return p;
}

void *fn_1_64(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M513_ASSERT(p != 0, lbl_1_data_0, lbl_1_data_C, 0x17);
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
void fn_1_2D8(M513Cam *self, f32 pitch, f32 yaw)
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

void fn_1_39C(M513Cam *self, f32 angle)
{
    Mtx m;
    Vec axis;

    PSVECSubtract(&self->target, &self->pos, &axis);
    PSMTXRotAxisRad(m, &axis, 0.017453292f * angle);
    PSMTXMultVec(m, &self->up, &self->up);
}

// Camera basis: the three rows of the look-at matrix.
void fn_1_408(M513Cam *self, Vec *x, Vec *y, Vec *z)
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

void fn_1_4B0(M513Cam *self, Mtx out)
{
    C_MTXLookAt(out, &self->pos, &self->up, &self->target);
    PSMTXInverse(out, out);
}

void fn_1_504(M513Cam *self, Vec *local)
{
    Mtx m;
    Vec world;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    PSMTXMultVecSR(m, local, &world);
    fn_1_56C(self, &world);
}

void fn_1_56C(M513Cam *self, Vec *delta)
{
    PSVECAdd(delta, &self->target, &self->target);
    PSVECAdd(delta, &self->pos, &self->pos);
}

void fn_1_5C0(M513Cam *self)
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

void fn_1_654(M513Cam *self, M513Pose *pose)
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

void fn_1_6F4(M513Cam *self, M513Pose *out)
{
    Mtx m;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    out->pos.x = self->pos.x;
    out->pos.y = self->pos.y;
    out->pos.z = self->pos.z;
    C_QUATMtx(&out->rot, m);
}

void fn_1_76C(M513Cam *self, M513Pose *a, M513Pose *b, f32 t)
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

void fn_1_87C(M513Cam *self, M513Cam *src)
{
    Vec delta;

    PSVECSubtract(&src->pos, &self->pos, &delta);
    self->pos.x = src->pos.x;
    self->pos.y = src->pos.y;
    self->pos.z = src->pos.z;
    PSVECAdd(&self->target, &delta, &self->target);
}

void fn_1_8E8(M513Cam *self, Vec *a, Vec *b, f32 t)
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

void fn_1_9A0(M513Cam *self, u16 camBit)
{
    Hu3DCameraPosSetV(camBit, &self->pos, &self->up, &self->target);
}

void fn_1_9E0(M513Cam *self, u16 camBit)
{
    Hu3DCameraPosGet(camBit, &self->pos, &self->up, &self->target);
}

// Dump the current camera as a pose table row (floats printed as raw words).
void fn_1_A20(M513Cam *self)
{
    Mtx m;
    M513PoseBits out;

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
void fn_1_C08(M513Cam *self, s32 pad)
{
    Mtx inv, rot, rotB, rotA, m, dump;
    M513PoseBits out;
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


M513Node *fn_1_204C(M513Node *list);
M513Node *fn_1_20A0(M513Cmp cmp, M513Node *a, M513Node *b);
M513Node *fn_1_19A4(M513Cmp cmp, M513Node *list);
void fn_1_2874(OMOBJ *obj);
void fn_1_2974(void);
void fn_1_311C(void);
void fn_1_4CE8(void *ptr);

// ---------------------------------------------------------------- handle table



// The id word is a 20 bit slot index over a 12 bit use counter; PC code
// reads/writes the whole word so it does not depend on the bitfield layout.
#ifdef TARGET_PC
#define M513HANDLE_SETINDEX(e, i) ((e)->h.id = (u32)(i) << 12)
#define M513HANDLE_BUMP(e) ((e)->h.id = ((e)->h.id & ~0xFFFu) | (((e)->h.id & 0xFFFu) + 1 & 0xFFFu))
#define M513HANDLE_CLEARCOUNT(e) ((e)->h.id &= ~0xFFFu)
#else
#define M513HANDLE_SETINDEX(e, i) ((e)->h.b.index = (i))
#define M513HANDLE_BUMP(e) ((e)->h.b.count = (e)->h.b.count + 1)
#define M513HANDLE_CLEARCOUNT(e) ((e)->h.b.count = 0)
#endif

M513HandleTbl *fn_1_1264(M513HandleTbl *self, u32 n)
{
    M513HandleEnt *ents;
    M513HandleEnt *p;
    u32 i;

    M513_ASSERT_INL(n <= 0x1000, lbl_1_data_81, lbl_1_data_76, 0x44);
    self->max = n;
    p = (M513HandleEnt *)HuMemDirectMallocNum(HEAP_HEAP, (n + 1) * sizeof(M513HandleEnt), 0x10000000);
    M513_ASSERT_INL(p != 0, lbl_1_data_0, lbl_1_data_C, 0x17);
    self->ents = p;
    ents = self->ents;
    self->tail = ents + n;
    self->tail->next = self->tail;
    self->tail->prev = self->tail;
    for (i = 1; i < n; i++) {
        self->ents[i].next = &self->ents[i + 1];
        self->ents[i].prev = &self->ents[i - 1];
        self->ents[i].obj = 0;
        M513HANDLE_SETINDEX(&self->ents[i], i);
        M513HANDLE_CLEARCOUNT(&self->ents[i]);
    }
    self->ents[1].prev = self->ents;
    self->ents[n - 1].next = self->ents;
    self->ents->next = self->ents + 1;
    self->ents->prev = self->ents + (n - 1);
    return self;
}

M513HandleTbl *fn_1_1468(M513HandleTbl *self, s16 flag)
{
    if (self != 0) {
        if (self->ents != 0) {
            HuMemDirectFree(self->ents);
        }
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

u32 fn_1_14D8(M513HandleTbl *self, void *obj)
{
    M513HandleEnt *e = self->ents->next;

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
    M513HANDLE_BUMP(e);
    return e->h.id;
}

void *fn_1_1568(M513HandleTbl *self, u32 id)
{
    M513HandleEnt *e;
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

void *fn_1_162C(M513HandleTbl *self, u32 id)
{
    M513HandleEnt *e;

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

s32 fn_1_169C(M513Node *self)
{
    s32 n = 0;
    M513Node *p = self->next;

    while (p != self) {
        p = p->next;
        n++;
    }
    return n;
}

s32 fn_1_16D8(M513HandleTbl *self)
{
    M513HandleEnt *tail = self->tail;
    s32 n = 0;
    M513HandleEnt *p = tail->next;

    while (p != tail) {
        p = p->next;
        n++;
    }
    return n;
}

s32 fn_1_1720(M513HandleTbl *self)
{
    M513HandleEnt *head = self->ents;
    s32 n = 0;
    M513HandleEnt *p = head->next;

    while (p != head) {
        p = p->next;
        n++;
    }
    return n;
}

void fn_1_1768(M513Node *self)
{
    self->next = self;
    self->prev = self;
}

void *fn_1_1774(M513Node *self, s16 flag)
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

void fn_1_17E0(M513Node *self)
{
    self->next->prev = self->prev;
    self->prev->next = self->next;
    self->next = self;
    self->prev = self;
}

// Insert `self` right after `other`.
void fn_1_1804(M513Node *self, M513Node *other)
{
    M513Node *at = other;

    self->prev = at;
    self->next = at->next;
    at->next->prev = self;
    at->next = self;
}

// Insert `self` right before `other`.
void fn_1_1834(M513Node *self, M513Node *other)
{
    M513Node *at = other;

    self->next = at;
    self->prev = at->prev;
    at->prev->next = self;
    at->prev = self;
}

// Move every node of `self` in front of `other`.
void fn_1_1864(M513Node *self, M513Node *other)
{
    M513Node *first;
    M513Node *last;
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
void fn_1_18EC(M513Node *self, M513Cmp cmp)
{
    M513Node *node;
    M513Node *prev;
    u8 empty;

    empty = 0;
    if (self->next == self && self->next == self->prev) {
        empty = 1;
    }
    if (empty == 0) {
        self->prev->next = 0;
        node = fn_1_19A4(cmp, self->next);
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

M513Node *fn_1_19A4(M513Cmp cmp, M513Node *list)
{
    M513Node *second;
    M513Node *a;
    M513Node *b;

    if (list == 0) {
        return 0;
    }
    if (list->next == 0) {
        return list;
    }
    second = fn_1_204C(list);
    a = fn_1_19A4(cmp, list);
    b = fn_1_19A4(cmp, second);
    return fn_1_20A0(cmp, a, b);
}

// Cut the list in the middle and return the second half.
M513Node *fn_1_204C(M513Node *list)
{
    M513Node *fast;
    M513Node *second;

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

M513Node *fn_1_20A0(M513Cmp cmp, M513Node *a, M513Node *b)
{
    M513Node head;
    M513Node *tail;

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

void fn_1_214C(s32 bit)
{
    M513Task *t;
    M513Node *end;
    M513Node *b;
    M513Node *c;
    M513Node *nx;

    b = lbl_1_bss_C.next;
    c = b;
    t = (M513Task *)c;
    lbl_1_bss_14 = bit;
    end = &lbl_1_bss_C;
    while ((M513Node *)t != end) {
        t->flags |= 1 << bit;
        t->vtbl->onFlag(t, bit);
        nx = t->link.next;
        t = (M513Task *)nx;
    }
}

s32 fn_1_21E4(void)
{
    M513Task *t;
    M513Node *end;
    M513Node *b;
    M513Node *c;
    M513Node *nx;
    s32 bit;

    b = lbl_1_bss_C.next;
    c = b;
    t = (M513Task *)c;
    end = &lbl_1_bss_C;
    while ((M513Node *)t != end) {
        bit = lbl_1_bss_14;
        if ((t->flags & (1 << bit)) != 0) {
            return 0;
        }
        nx = t->link.next;
        t = (M513Task *)nx;
    }
    return 1;
}

void fn_1_2270(void)
{
    M513Task *t;
    M513Node *end;
    M513Node *b;
    M513Node *c;
    M513Node *nx;

    b = lbl_1_bss_C.next;
    c = b;
    t = (M513Task *)c;
    end = &lbl_1_bss_C;
    while ((M513Node *)t != end) {
        t->vtbl->update(t);
        nx = t->link.next;
        t = (M513Task *)nx;
    }
}

void fn_1_22E0(M513Task *self)
{
}

// Destroy every registered task, last one first.
void fn_1_22E4(void)
{
    M513Task *t;
    u8 empty;

    while (1) {
        empty = 0;
        if (lbl_1_bss_C.next == &lbl_1_bss_C && lbl_1_bss_C.next == lbl_1_bss_C.prev) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        t = (M513Task *)lbl_1_bss_C.prev;
        if (t != 0) {
            t->vtbl->dtor(t, 1);
        }
    }
}

M513Task *fn_1_23AC(M513Task *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_218.vtbl;
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

s32 fn_1_2434(void)
{
    return lbl_1_bss_14;
}

// ---------------------------------------------------------------- game master

M513Master *fn_1_2444(M513Master *self)
{
    s32 i;

    M513_ASSERT_INL(lbl_1_bss_9EC == 0, lbl_1_data_9B, lbl_1_data_8F, 0x35);
    lbl_1_bss_9EC = self;
    self->vtbl = &lbl_1_data_200;
    self->counter = 0;
    i = 0;
    while (i < 4) {
        self->slot[i] = -1;
        i++;
    }
    return self;
}

M513Master *fn_1_2514(M513Master *self, s16 flag)
{
    if (self != 0) {
        M513_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_AB, lbl_1_data_8F, 0x3D);
        lbl_1_bss_9EC = 0;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

M513Master *fn_1_25D0(M513Master *self, s16 flag)
{
    M513Master *m;
    s32 i;
    s32 p;

    if (self != 0) {
        self->vtbl = &lbl_1_data_200;
        if (mgDataTbl[GwSystem.mgNo].type != 3) {
            M513_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_AB, lbl_1_data_8F, 0x27);
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
            M513_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_AB, lbl_1_data_8F, 0x3D);
            lbl_1_bss_9EC = 0;
        }
        if (flag > 0) {
            fn_1_4CE8(self);
        }
    }
    return self;
}

void fn_1_2788(M513Master *self, s32 objMax, s32 prio)
{
    OMOBJMAN *om;

    om = omInitObjMan((s16)objMax, prio);
    omGameSysInit(om);
    self->vtbl->setup(self, om);
    omAddObjEx(om, 0, 0, 0, -1, fn_1_2874);
}

void fn_1_280C(M513Master *self)
{
    OMOBJMAN *om;

    om = MgActorObjectSetup();
    self->vtbl->setup(self, om);
    omAddObjEx(om, 0, 0, 0, -1, fn_1_2874);
}

void fn_1_2904(M513Master *self);

void fn_1_2874(OMOBJ *obj)
{
    OMOBJ *self;
    M513Master *m;
    M513Master *cur;

    self = obj;
    if (omSysExitReq != 0 && WipeCheckEnd() == 0) {
        fn_1_311C();
        HuAudFadeOut(0x3E8);
        self->objFunc = (OMOBJFUNC)fn_1_2974;
        return;
    }
    m = lbl_1_bss_9EC;
    cur = m;
    fn_1_2904(cur);
}

void fn_1_2904(M513Master *self)
{
    self->vtbl->update(self);
    if (self->counter > 0) {
        if (--self->counter == 0) {
        }
    }
}

void fn_1_2970(M513Master *self)
{
}

void fn_1_2974(void)
{
    M513Master *m;
    M513Master *cur;
    M513Master *obj;
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
void fn_1_2A60(s32 player)
{
    s32 *slots;
    s32 i;

    M513_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_AB, lbl_1_data_8F, 0x27);
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

s32 fn_1_2B34(s32 player)
{
    s32 i;

    M513_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_AB, lbl_1_data_8F, 0x27);
    i = 0;
    while (i < 4) {
        if (player == lbl_1_bss_9EC->slot[i]) {
            return 1;
        }
        i++;
    }
    return 0;
}

void fn_1_2C08(void)
{
    M513Master *m;

    M513_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_AB, lbl_1_data_8F, 0x27);
    m = lbl_1_bss_9EC;
    m->mesId = GameMesCreate(2, 0);
}

void fn_1_2CC0(void)
{
    M513Master *m;

    M513_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_AB, lbl_1_data_8F, 0x27);
    m = lbl_1_bss_9EC;
    m->mesId = GameMesCreate(2, 1);
}

s32 fn_1_2D78(void)
{
    M513Master *m;
    s32 ret;

    M513_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_AB, lbl_1_data_8F, 0x27);
    m = lbl_1_bss_9EC;
    ret = 0;
    if (GameMesStatGet(-1) == 0 && (u8)(m->counter <= 0) != 0) {
        ret = 1;
    }
    return ret;
}

// Announce the result: sort the registered players and print their characters.
void fn_1_2E4C(void)
{
    M513Master *m;
    s32 *slots;
    s32 chars[4];
    s32 n;
    s32 i;
    s32 j;
    s32 k;
    s32 tmp;
    s16 charNo;

    M513_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_AB, lbl_1_data_8F, 0x27);
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

void fn_1_3018(s32 mes)
{
    M513Master *m;

    if (_CheckFlag(0x1000F) == 0) {
        M513_ASSERT_INL(lbl_1_bss_9EC != 0, lbl_1_data_AB, lbl_1_data_8F, 0x27);
        m = lbl_1_bss_9EC;
        m->mesId = GameMesCreate(0xE, mes);
        HuAudJinglePlay(0x86);
    }
}

void fn_1_30F0(void)
{
    WipeCreate(1, 5, 0x3C);
}

void fn_1_311C(void)
{
    WipeCreate(2, 6, 0x3C);
}

void fn_1_3148(void)
{
    omSysExitReq = 1;
}

// ---------------------------------------------------------------- Hu3D model wrappers

M513Model *fn_1_315C(M513Model *self, void *data)
{
    self->id = Hu3DModelCreate(data);
    return self;
}

M513Model *fn_1_3198(M513Model *self, s32 dataNum)
{
    self->id = Hu3DModelCreateData(dataNum);
    return self;
}

M513Model *fn_1_31E0(M513Model *self, HU3DMODELHOOK hook)
{
    self->id = Hu3DHookFuncCreate(hook);
    return self;
}

// Build a character model; `type` selects the model variant bit.
M513Model *fn_1_321C(M513Model *self, s32 charNo, s32 type)
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

inline s32 M513ModelIdGet(M513Model *m)
{
    return m->id;
}

M513Model *fn_1_32B8(M513Model *self, M513Model *parent)
{
    self->id = Hu3DModelLink(M513ModelIdGet(parent));
    return self;
}

void fn_1_330C(M513Model *self, u32 attr)
{
    Hu3DModelAttrSet(self->id, attr);
}

void fn_1_3344(M513Model *self, u32 attr)
{
    Hu3DModelAttrReset(self->id, attr);
}

// Apply an object's transform (translation, rotation, scale) to the model.
void fn_1_337C(M513Model *self, OMOBJ *obj)
{
    HU3DMODELID id;

    id = self->id;
    Hu3DModelPosSet(id, obj->trans.x, obj->trans.y, obj->trans.z);
    Hu3DModelRotSet(id, obj->rot.x, obj->rot.y, obj->rot.z);
    Hu3DModelScaleSet(id, obj->scale.x, obj->scale.y, obj->scale.z);
}

void fn_1_33F8(M513Model *self, Vec *pos)
{
    Hu3DModelPosSet(self->id, pos->x, pos->y, pos->z);
}

void fn_1_3440(M513Model *self, Vec *rot)
{
    Hu3DModelRotSet(self->id, rot->x, rot->y, rot->z);
}

void fn_1_3488(M513Model *self, Vec *scale)
{
    Hu3DModelScaleSet(self->id, scale->x, scale->y, scale->z);
}

void fn_1_34D0(M513Model *self, f32 scale)
{
    Hu3DModelScaleSet(self->id, scale, scale, scale);
}

void fn_1_3518(M513Model *self, f32 tpLvl)
{
    Hu3DModelTPLvlSet(self->id, tpLvl);
}

void fn_1_3550(M513Model *self, s16 layer)
{
    Hu3DModelLayerSet(self->id, layer);
}

void fn_1_3588(M513Model *self, u16 camBit)
{
    Hu3DModelCameraSet(self->id, camBit);
}

void fn_1_35C0(M513Model *self, void *data)
{
    Hu3DData[self->id].hookData = data;
}

MtxPtr fn_1_35DC(M513Model *self)
{
    return Hu3DData[self->id].mtx;
}

M513Model *fn_1_35F8(M513Model *self, s16 flag)
{
    if (self != 0) {
        Hu3DModelKill(self->id);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

inline HSFOBJECT *M513ObjGet(M513Model *self, char *name)
{
    HSFOBJECT *obj;

    obj = Hu3DModelObjPtrGet(self->id, name);
    M513_ASSERT_INL(obj != 0, lbl_1_data_C5, lbl_1_data_BB, 0x8A);
    return obj;
}

HSFOBJECT *fn_1_3654(M513Model *self, char *name)
{
    return M513ObjGet(self, name);
}

void fn_1_36E8(M513Model *self)
{
    Hu3DModelShadowSet(self->id);
}

void fn_1_3718(M513Model *self)
{
    Hu3DModelShadowMapSet(self->id);
}

void fn_1_3748(M513Model *self, char *name)
{
    Hu3DModelShadowMapObjSet(self->id, name);
}

void fn_1_3780(M513Model *self)
{
    Hu3DModelShadowMapObjReset(self->id);
}

void fn_1_37B0(M513Model *self, char *name)
{
    HSFOBJECT *obj;
    u32 *flags;

    obj = M513ObjGet(self, name);
    flags = (u32 *)obj->constData;
    *flags &= ~8;
}

void fn_1_385C(M513Model *self, char *name)
{
    HSFOBJECT *obj;

    obj = M513ObjGet(self, name);
    obj->flags |= 0x400;
}

void fn_1_3904(M513Model *self, char *name)
{
    HSFOBJECT *obj;

    obj = M513ObjGet(self, name);
    obj->flags &= ~0x400;
}

void fn_1_39AC(M513Model *self, char *name)
{
    HSFOBJECT *obj;
    HSFTRANSFORM *base;

    obj = M513ObjGet(self, name);
    obj->flags |= 0x400;
    base = &obj->mesh.base;
    base->scale.x = 0.0f;
    base->scale.y = 0.0f;
    base->scale.z = 0.0f;
}

void fn_1_3A88(M513Model *self, char *name)
{
    HSFOBJECT *obj;
    HSFTRANSFORM *base;

    obj = M513ObjGet(self, name);
    obj->flags &= ~0x400;
    base = &obj->mesh.base;
    base->scale.x = 1.0f;
    base->scale.y = 1.0f;
    base->scale.z = 1.0f;
}

void fn_1_3B64(M513Model *self, s32 time)
{
    Hu3DMotionTimeSet(M513ModelIdGet(self), (f32)time);
}

// ---------------------------------------------------------------- Mersenne Twister

extern const u32 lbl_1_rodata_50[2] = { 0, 0x9908B0DF };

void fn_1_3BD0(M513Rand *r, u32 seed)
{
    r->mt[0] = seed;
    r->idx = 1;
    while (r->idx < 0x270) {
        r->mt[r->idx] = r->idx + (((r->mt[r->idx - 1]) ^ ((r->mt[r->idx - 1]) >> 30)) * 0x6C078965);
        r->idx++;
    }
}

void fn_1_3C44(M513Rand *r, u32 *key, u32 len)
{
    u32 i;
    u32 j;
    u32 k;

    fn_1_3BD0(r, 19650218);
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

u32 fn_1_3E08(M513Rand *r)
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

u32 fn_1_3F94(M513Rand *r)
{
    return fn_1_3E08(r) >> 1;
}

f32 fn_1_412C(M513Rand *r)
{
    return 2.3283064e-10f * (f32)fn_1_3E08(r);
}

f32 fn_1_42F0(M513Rand *r)
{
    return 2.3283064e-10f * (f32)fn_1_3E08(r);
}

f32 fn_1_44B4(M513Rand *r)
{
    return 2.3283064e-10f * (0.5f + (f32)fn_1_3E08(r));
}

f64 fn_1_4688(M513Rand *r)
{
    u32 a;
    u32 b;

    a = fn_1_3E08(r) >> 5;
    b = fn_1_3E08(r) >> 6;
    return 1.1102230246251565e-16 * ((67108864.0 * (f64)a) + (f64)b);
}

f32 fn_1_4A00(M513Rand *r)
{
    return 1.1920929e-7f * (f32)(fn_1_3E08(r) >> 9);
}

// ---------------------------------------------------------------- allocation / functor objects

void *fn_1_4BD0(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M513_ASSERT_INL(p != 0, lbl_1_data_0, lbl_1_data_CF, 0x18);
    return p;
}

void *fn_1_4C5C(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M513_ASSERT_INL(p != 0, lbl_1_data_0, lbl_1_data_CF, 0x1F);
    return p;
}

void fn_1_4CE8(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_4D20(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

// Functor owned by an object: the object's data slot points at it.
void fn_1_4D58(M513Functor *self, OMOBJ *obj)
{
    self->vtbl = &lbl_1_data_1F0;
    obj->data = self;
    obj->objFunc = fn_1_4D78;
}

void fn_1_4D78(OMOBJ *obj)
{
    M513Functor *self;

    self = (M513Functor *)obj->data;
    self->vtbl->run(self, obj);
}

void fn_1_4DC4(M513Functor *self, OMOBJ *obj)
{
}

void fn_1_4DC8(OMOBJ *obj)
{
    M513Functor *fn;
    M513Functor *p;

    fn = (M513Functor *)obj->data;
    p = fn;
    if (p != 0) {
        p = p->vtbl->dtor(p, 1);
    }
    omDelObjEx(HuPrcCurrentGet(), obj);
}

M513Functor *fn_1_4E38(M513Functor *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_1F0;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_4E98(OMOBJMAN *man, OMOBJ *obj)
{
    M513Functor *fn;
    M513Functor *p;

    fn = (M513Functor *)obj->data;
    p = fn;
    if (p != 0) {
        p = p->vtbl->dtor(p, 1);
    }
    omDelObjEx(man, obj);
}

// ---------------------------------------------------------------- global random number generator

void fn_1_4F0C(u32 seed)
{
    fn_1_3BD0(&lbl_1_bss_24, seed);
}

u32 fn_1_4FE4(void)
{
    return fn_1_3E08(&lbl_1_bss_24) >> 1;
}

f32 fn_1_5220(void)
{
    return 1.1920929e-7f * (f32)(fn_1_3E08(&lbl_1_bss_24) >> 9);
}

f32 fn_1_5494(f32 scale)
{
    return scale * fn_1_4688(&lbl_1_bss_24);
}

f32 fn_1_54CC(f32 from, f32 to)
{
    return from + (to - from) * fn_1_4688(&lbl_1_bss_24);
}

// ---------------------------------------------------------------- shadow / collision helpers

// Distance at which a 5 degree field of view spans `size`; `out` = target - dir * distance.
f32 fn_1_5520(Vec *out, Vec *dir, Vec *target, f32 size)
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



// Sphere: centre and radius.


inline f32 M513SphereRadius(M513Sphere *s)
{
    return s->radius;
}


// Push two overlapping spheres apart; ka/kb weight the move of each.
s32 fn_1_55C8(M513Sphere *a, M513Sphere *b, f32 ka, f32 kb)
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

s32 fn_1_5748(void)
{
    return OSGetThreadPriority(OSGetCurrentThread());
}

void *fn_1_5A30(void *arg);
M513Thread *fn_1_5A8C(M513Thread *th, void *(*func)(void *), void *arg, u32 size, s32 prio, u16 attr);

M513SubThread *fn_1_576C(M513SubThread *self, u32 stackSize)
{
    self->vtbl = &lbl_1_data_1E0;
    fn_1_5A8C(&self->th, fn_1_5A30, self, stackSize, OSGetThreadPriority(OSGetCurrentThread()), 1);
    OSInitSemaphore(&self->sem, 0);
    return self;
}

M513Thread *fn_1_5D10(M513Thread *th, s16 flag);

M513SubThread *fn_1_57E8(M513SubThread *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_1E0;
        fn_1_5D10(&self->th, -1);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

// CThread::IsTerminated(), inlined with its own `this`.
inline u8 M513ThreadDone(M513Thread *th)
{
    return OSIsThreadTerminated(&th->thread) != 0;
}

u8 fn_1_5854(M513SubThread *self)
{
    M513Thread *th = &self->th;

    return OSIsThreadTerminated(&th->thread) != 0;
}

u32 fn_1_5E44(M513Thread *th);

s32 fn_1_589C(M513SubThread *self, s32 arg)
{
    M513Thread *thIsDone;
    M513Thread *thResume;
    M513Sem *sem;

    thIsDone = &self->th;
    if (OSIsThreadTerminated(&thIsDone->thread) != 0) {
        return self->arg;
    }
    self->arg = arg;
    thResume = &self->th;
    OSResumeThread(&thResume->thread);
    sem = &self->sem;
    OSWaitSemaphore(sem);
    fn_1_5E44(&self->th);
    OSCheckActiveThreads();
    return self->arg;
}

s32 fn_1_592C(M513SubThread *self, s32 arg)
{
    M513Thread *thCur;
    M513Thread *thDone;
    M513Sem *sem;
    M513Thread *thSuspend;

    thCur = &self->th;
    M513_ASSERT_INL(OSGetCurrentThread() == &thCur->thread, lbl_1_data_E4, lbl_1_data_D6, 0x47);
    thDone = &self->th;
    M513_ASSERT_INL(OSIsThreadTerminated(&thDone->thread) == 0, lbl_1_data_103, lbl_1_data_D6, 0x4A);
    self->arg = arg;
    sem = &self->sem;
    OSSignalSemaphore(sem);
    thSuspend = &self->th;
    OSSuspendThread(&thSuspend->thread);
    return self->arg;
}

void *fn_1_5A30(void *arg)
{
    M513SubThread *self;
    M513Sem *sem;

    self = (M513SubThread *)arg;
    self->vtbl->run(self, self->arg);
    sem = &self->sem;
    OSSignalSemaphore(sem);
    return 0;
}

void fn_1_5BEC(M513Thread *th, void *(*func)(void *), void *arg, u32 *stack, u32 words, s32 prio, u16 attr);

M513Thread *fn_1_5A8C(M513Thread *th, void *(*func)(void *), void *arg, u32 size, s32 prio, u16 attr)
{
    u32 words;
    u32 *stack;

    words = size >> 2;
    stack = (u32 *)HuMemDirectMallocNum(HEAP_HEAP, words * 4, 0x10000000);
    M513_ASSERT_INL(stack != 0, lbl_1_data_0, lbl_1_data_C, 0x17);
    th->stack = stack;
    fn_1_5BEC(th, func, arg, stack, words, prio, attr);
    return th;
}

M513Thread *fn_1_5B70(M513Thread *th, void *(*func)(void *), void *arg, u32 *stack, u32 size, s32 prio, u16 attr)
{
    u32 words;

    th->stack = 0;
    words = size >> 2;
    fn_1_5BEC(th, func, arg, stack, words, prio, attr);
    return th;
}

void fn_1_5F58(u32 *p, u32 words);
s32 fn_1_5EE4(u32 *p, s32 mode);

void fn_1_5BEC(M513Thread *th, void *(*func)(void *), void *arg, u32 *stack, u32 words, s32 prio, u16 attr)
{
    u8 ok;

    ok = 0;
    if (prio >= 0 && prio <= 0x1F) {
        ok = 1;
    }
    M513_ASSERT_INL(ok != 0, lbl_1_data_12C, lbl_1_data_121, 0x52);
    fn_1_5F58(stack, words);
    fn_1_5EE4(stack, 0);
    if ((u8)(OSCreateThread(&th->thread, func, arg, &stack[words], words * 4, prio, attr) != 0) == 0) {
        OSPanic(lbl_1_data_121, 0x60, lbl_1_data_167);
    }
}

M513Thread *fn_1_5D10(M513Thread *th, s16 flag)
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
            remain = fn_1_5E44(th);
            printf(lbl_1_data_180, th, remain, total, (100.0 * (f64)remain) / (f64)total);
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

u32 fn_1_5E44(M513Thread *th)
{
    u32 *base;
    u32 *p;

    base = th->thread.stackEnd;
    if (fn_1_5EE4(base, 1) != 0) {
        OSPanic(lbl_1_data_121, 0x7F, lbl_1_data_1BD);
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
s32 fn_1_5EE4(u32 *p, s32 mode)
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

void fn_1_5F58(u32 *p, u32 words)
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

M513TimerObj *fn_1_5FA4(M513TimerObj *self, s32 type)
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
            M513_ASSERT_INL(0, lbl_1_data_1D7, lbl_1_data_1CD, 0x1E);
            break;
    }
    return self;
}

M513TimerObj *fn_1_606C(M513TimerObj *self, s16 flag)
{
    if (self != 0) {
        MgTimerKill(self->timer);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_60C8(M513TimerObj *self, s32 maxTime, s32 endTime, s32 recordTime)
{
    MgTimerParamSet(self->timer, maxTime, endTime, recordTime);
}

void fn_1_6110(M513TimerObj *self)
{
    MgTimerValueGet(self->timer);
}

void fn_1_6140(M513TimerObj *self, f32 x, f32 y)
{
    MgTimerPosSet(self->timer, x, y);
}

void fn_1_6180(M513TimerObj *self, f32 *x, f32 *y)
{
    MgTimerPosGet(self->timer, x, y);
}

void fn_1_61C0(M513TimerObj *self, s32 offType)
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
            M513_ASSERT_INL(0, lbl_1_data_1D7, lbl_1_data_1CD, 0x48);
            return;
    }
}

void fn_1_6284(M513TimerObj *self)
{
    MgTimerModeOffSet(self->timer);
}

// Stop the timer the first time it is asked to.
void fn_1_62B4(M513TimerObj *self)
{
    if (self->timer->stopF == 0) {
        self->timer->stopF = 1;
        self->timer->mode = 1;
    }
}

void fn_1_62E0(M513TimerObj *self)
{
    MgTimerKill(self->timer);
    self->timer = MgTimerCreate(0);
}

u8 fn_1_631C(M513TimerObj *self)
{
    return MgTimerDoneCheck(self->timer) != 0;
}

void fn_1_635C(M513TimerObj *self, u8 r, u8 g, u8 b)
{
    MgTimerWinColorSet(self->timer, r, g, b);
}

// ---------------------------------------------------------------- static initialisers

void fn_1_63A4(void)
{
    lbl_1_bss_C.next = &lbl_1_bss_C;
    lbl_1_bss_C.prev = &lbl_1_bss_C;
#ifdef __MWERKS__
    __register_global_object(&lbl_1_bss_C, (void *)fn_1_6504, lbl_1_bss_0);
#endif
    lbl_1_bss_18 = 0x78;
    lbl_1_bss_1C = 0xD2;
    lbl_1_bss_20 = 0xD2;
    fn_1_3BD0(&lbl_1_bss_24, 0x1571);
}

void *fn_1_6504(M513Node *self, s16 flag)
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
__declspec(section ".ctors") void (*const lbl_ctor_61C4)(void) = fn_1_63A4;
#else
struct M513StaticInit {
    M513StaticInit() { fn_1_63A4(); }
};
static M513StaticInit lbl_static_init;
#endif
}
