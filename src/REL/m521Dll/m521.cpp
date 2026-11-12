#include <stdarg.h>
#include "dolphin/types.h"
#include "dolphin/mtx.h"
#include "dolphin/os.h"
#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mg/actman.h"
#include "game/mg/timer.h"
#include "game/mgdata.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/wipe.h"
#include "REL/m521Dll/m521Types.h"

extern "C" {
int printf(const char *fmt, ...);
int sprintf(char *buf, const char *fmt, ...);
int vprintf(const char *fmt, va_list args);
void ObjectSetup(void);

void OSInitSemaphore(OSSemaphore *sem, s32 count);
s32 OSWaitSemaphore(OSSemaphore *sem);
s32 OSSignalSemaphore(OSSemaphore *sem);

extern void (*_ctors[])(void);
extern void (*_dtors[])(void);

// Camera as the engine stores it: position, up vector, look-at target.
typedef struct CCam {
    Vec pos;
    Vec up;
    Vec target;
} CCam;

// Camera stored as position plus orientation quaternion.
typedef struct CCamQ {
    Vec pos;
    Quaternion quat;
} CCamQ;

// Task object run by an object manager (vtable slot 2 = destructor, slot 3 = run).
typedef struct CTask {
    void **m_vtbl;
} CTask;

// Position / rotation / scale block the model helpers copy from.
typedef struct CXform {
    f32 m_unk[6];
    Vec m_pos;
    Vec m_rot;
    Vec m_scale;
} CXform;

// Mersenne twister state.
typedef struct CMt {
    u32 m_mt[624];
    u32 m_idx;
} CMt;

// Timer wrapper (class owning an MGTIMER).
typedef struct CTimer {
    MGTIMER *m_timer;
} CTimer;

typedef u8 (*CListCmp)(CListNode *a, CListNode *b);

void fn_1_4C3C(const char *cond, const char *file, s32 line);
void *fn_1_4A68(u32 n);
void fn_1_6844(void);
void fn_1_6ED4(void);
void fn_1_924C(void *p);
void __register_global_object(void *obj, void *dtor, void *reg);

CListNode lbl_1_bss_1C;
void *lbl_1_bss_10[3];
s32 lbl_1_bss_24;
s32 lbl_1_bss_28;
s32 lbl_1_bss_2C;
s32 lbl_1_bss_30;
CMt lbl_1_bss_34;
u8 lbl_1_bss_9F8;
extern CAppBase *lbl_1_bss_C;

CObject *fn_1_6910(CObject *self, s16 flag);
CAppBase *fn_1_6B34(CAppBase *self, s16 flag);
CSubThread *fn_1_9D4C(CSubThread *self, s16 flag);
CTask *fn_1_939C(CTask *self, s16 flag);
void fn_1_9328(void);

// Vtables
void *lbl_1_data_1E4[4] = { 0, 0, (void *)fn_1_9D4C, 0 };
void *lbl_1_data_1F4[4] = { 0, 0, (void *)fn_1_939C, (void *)fn_1_9328 };
void *lbl_1_data_204[6] = { 0, 0, (void *)fn_1_6B34, 0, (void *)fn_1_6ED4, 0 };
void *lbl_1_data_21C[5] = { 0, 0, (void *)fn_1_6910, 0, (void *)fn_1_6844 };

#ifdef __MWERKS__
#define M521_RODATA __declspec(section ".rodata")
#else
#define M521_RODATA
#endif

M521_RODATA const u32 lbl_1_rodata_160[2] = { 0, 0x9908B0DF };

// Reports a failed assertion and halts.
void fn_1_4C3C(const char *cond, const char *file, s32 line)
{
    char buf[0x400];

    sprintf(buf, "\nASSERTION FAILED at %s:%d\n%s", file, line, cond);
    OSPanic("assert.cpp", 0x12, buf);
}

void *fn_1_4A68(u32 n)
{
    void *p;

    p = HuMemDirectMallocNum(HEAP_HEAP, n, 0x10000000);
    if (p != 0) {
    } else {
        fn_1_4C3C("ptr != NULL", "new.cpp", 0x17);
    }
    return p;
}

void *fn_1_4ACC(u32 n)
{
    void *p;

    p = HuMemDirectMallocNum(HEAP_HEAP, n, 0x10000000);
    if (p != 0) {
    } else {
        fn_1_4C3C("ptr != NULL", "new.cpp", 0x17);
    }
    return p;
}

void __dl__FPv(void *p)
{
    if (p != 0) {
        HuMemDirectFree(p);
    }
}

void fn_1_4B68(void *p)
{
    if (p != 0) {
        HuMemDirectFree(p);
    }
}

void _prolog(void)
{
    void (**ctor)(void);

    ctor = _ctors;
    while (*ctor != 0) {
        (*ctor)();
        ctor++;
    }
    ObjectSetup();
}

void _epilog(void)
{
    void (**dtor)(void);

    dtor = _dtors;
    while (*dtor != 0) {
        (*dtor)();
        dtor++;
    }
}



// Prints through vprintf (the stream argument is ignored by this build).
int fn_1_4C9C(void *file, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    return vprintf(fmt, args);
}

// Camera helpers ------------------------------------------------------------

void fn_1_4E70(CCam *cam, Vec *right, Vec *up, Vec *back);
void fn_1_4FD4(CCam *cam, Vec *delta);

void fn_1_4D40(CCam *cam, f32 pitch, f32 yaw)
{
    Vec right;
    Vec up;
    Vec back;
    Mtx rotA;
    Mtx rotB;
    Mtx rot;

    fn_1_4E70(cam, &right, &up, &back);
    PSMTXRotAxisRad(rotA, &right, 0.017453292f * pitch);
    PSMTXRotRad(rotB, 'Y', 0.017453292f * yaw);
    PSMTXConcat(rotA, rotB, rot);
    PSMTXMultVecSR(rot, &back, &back);
    PSMTXMultVecSR(rot, &up, &cam->up);
    PSVECSubtract(&cam->pos, &back, &cam->target);
}

void fn_1_4E04(CCam *cam, f32 roll)
{
    Vec dir;
    Mtx rot;

    PSVECSubtract(&cam->target, &cam->pos, &dir);
    PSMTXRotAxisRad(rot, &dir, 0.017453292f * roll);
    PSMTXMultVec(rot, &cam->up, &cam->up);
}

void fn_1_4E70(CCam *cam, Vec *right, Vec *up, Vec *back)
{
    Mtx view;

    C_MTXLookAt(view, &cam->pos, &cam->up, &cam->target);
    right->x = view[0][0];
    right->y = view[0][1];
    right->z = view[0][2];
    up->x = view[1][0];
    up->y = view[1][1];
    up->z = view[1][2];
    back->x = view[2][0];
    back->y = view[2][1];
    back->z = view[2][2];
}

void fn_1_4F18(CCam *cam, Mtx out)
{
    C_MTXLookAt(out, &cam->pos, &cam->up, &cam->target);
    PSMTXInverse(out, out);
}

void fn_1_4F6C(CCam *cam, Vec *move)
{
    Vec world;
    Mtx view;

    C_MTXLookAt(view, &cam->pos, &cam->up, &cam->target);
    PSMTXInverse(view, view);
    PSMTXMultVecSR(view, move, &world);
    fn_1_4FD4(cam, &world);
}

void fn_1_4FD4(CCam *cam, Vec *delta)
{
    PSVECAdd(delta, &cam->target, &cam->target);
    PSVECAdd(delta, &cam->pos, &cam->pos);
}

void fn_1_5028(CCam *cam)
{
    cam->up.x = 0.0f;
    cam->up.y = 1.0f;
    cam->up.z = 0.0f;
    cam->pos.x = 0.0f;
    cam->pos.y = 0.0f;
    cam->pos.z = 5000.0f;
    cam->target.x = 0.0f;
    cam->target.y = 0.0f;
    cam->target.z = 0.0f;
}

void fn_1_50BC(CCam *cam, CCamQ *q)
{
    Mtx m;

    PSMTXQuat(m, &q->quat);
    cam->up.x = m[0][1];
    cam->up.y = m[1][1];
    cam->up.z = m[2][1];
    cam->pos.x = q->pos.x;
    cam->pos.y = q->pos.y;
    cam->pos.z = q->pos.z;
    cam->target.x = q->pos.x - m[0][2];
    cam->target.y = q->pos.y - m[1][2];
    cam->target.z = q->pos.z - m[2][2];
}

void fn_1_515C(CCam *cam, CCamQ *q)
{
    Mtx m;

    C_MTXLookAt(m, &cam->pos, &cam->up, &cam->target);
    PSMTXInverse(m, m);
    q->pos.x = cam->pos.x;
    q->pos.y = cam->pos.y;
    q->pos.z = cam->pos.z;
    C_QUATMtx(&q->quat, m);
}

void fn_1_51D4(CCam *dst, CCamQ *a, CCamQ *b, f32 t)
{
    Quaternion q;
    Vec p;
    Vec pa;
    Vec pb;
    Mtx m;

    C_QUATSlerp(&a->quat, &b->quat, &q, t);
    PSVECScale(&a->pos, &pa, 1.0f - t);
    PSVECScale(&b->pos, &pb, t);
    PSVECAdd(&pa, &pb, &p);
    PSMTXQuat(m, &q);
    dst->up.x = m[0][1];
    dst->up.y = m[1][1];
    dst->up.z = m[2][1];
    dst->pos.x = p.x;
    dst->pos.y = p.y;
    dst->pos.z = p.z;
    dst->target.x = p.x - m[0][2];
    dst->target.y = p.y - m[1][2];
    dst->target.z = p.z - m[2][2];
}

void fn_1_52E4(CCam *cam, Vec *newPos)
{
    Vec d;

    PSVECSubtract(newPos, &cam->pos, &d);
    cam->pos.x = newPos->x;
    cam->pos.y = newPos->y;
    cam->pos.z = newPos->z;
    PSVECAdd(&cam->target, &d, &cam->target);
}

void fn_1_5350(CCam *cam, Vec *a, Vec *b, f32 t)
{
    Vec pa;
    Vec pb;
    Vec d;

    PSVECScale(a, &pa, 1.0f - t);
    PSVECScale(b, &pb, t);
    PSVECAdd(&pa, &pb, &pb);
    PSVECSubtract(&pb, &cam->pos, &d);
    cam->pos.x = pb.x;
    cam->pos.y = pb.y;
    cam->pos.z = pb.z;
    PSVECAdd(&cam->target, &d, &cam->target);
}

void fn_1_5408(CCam *cam, u16 camBit)
{
    Hu3DCameraPosSetV(camBit, &cam->pos, &cam->up, &cam->target);
}

void fn_1_5448(CCam *cam, u16 camBit)
{
    Hu3DCameraPosGet(camBit, &cam->pos, &cam->up, &cam->target);
}

void fn_1_5488(CCam *cam)
{
    Quaternion q;
    Mtx m;
    f32 x;
    f32 y;
    f32 z;

    C_MTXLookAt(m, &cam->pos, &cam->up, &cam->target);
    PSMTXInverse(m, m);
    x = cam->pos.x;
    y = cam->pos.y;
    z = cam->pos.z;
    C_QUATMtx(&q, m);
    printf("{{0x%08x,0x%08x,0x%08x},{0x%08x,0x%08x,0x%08x,0x%08x}},\n", x, y, z, q.x, q.y, q.z, q.w);
}

f32 fn_1_5520(s32 pad)
{
    return 0.017857144f * (f32)HuPadStkX[pad];
}

f32 fn_1_5574(s32 pad)
{
    return 0.017857144f * (f32)HuPadStkY[pad];
}

f32 fn_1_55C8(s32 pad)
{
    return 0.016949153f * (f32)HuPadSubStkX[pad];
}

f32 fn_1_561C(s32 pad)
{
    return 0.016949153f * (f32)HuPadSubStkY[pad];
}

// Free-fly debug camera driven by the pad.
void fn_1_5670(CCam *cam, s32 pad)
{
    u16 btn;
    f32 yaw;
    f32 pitch;
    f32 speed;
    Mtx view;
    Mtx rotA;
    Mtx rotB;
    Mtx rot;
    Mtx inv;
    Vec axisRight;
    Vec axisUp;
    Vec axisBack;
    Vec mv;
    Vec fwd;
    Quaternion q;
    f32 px;
    f32 py;
    f32 pz;
    f32 mag;

    btn = HuPadBtn[pad];
    if ((btn & 0x800) != 0) {
        cam->up.x = 0.0f;
        cam->up.y = 1.0f;
        cam->up.z = 0.0f;
    }
    yaw = -4.0f * (0.017857144f * (f32)HuPadStkX[pad]);
    pitch = -4.0f * (0.017857144f * (f32)HuPadStkY[pad]);
    C_MTXLookAt(view, &cam->pos, &cam->up, &cam->target);
    axisRight.x = view[0][0];
    axisRight.y = view[0][1];
    axisRight.z = view[0][2];
    axisUp.x = view[1][0];
    axisUp.y = view[1][1];
    axisUp.z = view[1][2];
    axisBack.x = view[2][0];
    axisBack.y = view[2][1];
    axisBack.z = view[2][2];
    PSMTXRotAxisRad(rotA, &axisRight, 0.017453292f * pitch);
    PSMTXRotRad(rotB, 'Y', 0.017453292f * yaw);
    PSMTXConcat(rotA, rotB, rot);
    PSMTXMultVecSR(rot, &axisBack, &axisBack);
    PSMTXMultVecSR(rot, &axisUp, &cam->up);
    PSVECSubtract(&cam->pos, &axisBack, &cam->target);
    if ((u8)((btn & 0x200) != 0) != 0) {
        speed = 70.0f;
    } else {
        speed = 20.0f;
    }
    C_MTXLookAt(inv, &cam->pos, &cam->up, &cam->target);
    PSMTXInverse(inv, inv);
    mv.x = (speed * (f32)(u8)((btn & 2) != 0)) - (speed * (f32)(u8)((btn & 1) != 0));
    mv.y = (speed * (f32)(u8)((btn & 8) != 0)) - (speed * (f32)(u8)((btn & 4) != 0));
    mv.x += speed * (0.016949153f * (f32)HuPadSubStkX[pad]);
    mv.z = 0.0f;
    PSMTXMultVecSR(inv, &mv, &mv);
    mv.y += speed * (0.016949153f * (f32)HuPadSubStkY[pad]);
    PSVECAdd(&mv, &cam->target, &cam->target);
    PSVECAdd(&mv, &cam->pos, &cam->pos);
    fwd.x = 0.0f;
    fwd.y = 0.0f;
    fwd.z = (f32)((u8)((btn & 0x400) != 0) - (u8)((btn & 0x100) != 0));
    PSMTXMultVecSR(inv, &fwd, &fwd);
    fwd.y = 0.0f;
    mag = PSVECMag(&fwd);
    if (mag > 1e-10f) {
        PSVECScale(&fwd, &fwd, speed / mag);
        PSVECAdd(&fwd, &cam->target, &cam->target);
        PSVECAdd(&fwd, &cam->pos, &cam->pos);
    }
    if ((HuPadBtnDown[pad] & 0x200) != 0) {
        C_MTXLookAt(view, &cam->pos, &cam->up, &cam->target);
        PSMTXInverse(view, view);
        px = cam->pos.x;
        py = cam->pos.y;
        pz = cam->pos.z;
        C_QUATMtx(&q, view);
        printf("{{0x%08x,0x%08x,0x%08x},{0x%08x,0x%08x,0x%08x,0x%08x}},\n", px, py, pz, q.x, q.y, q.z, q.w);
    }
}

// Intrusive list ---------------------------------------------------------------

void fn_1_5CCC(CListNode *self)
{
    self->m_next = self;
    self->m_prev = self;
}

CListNode *fn_1_5CD8(CListNode *self, s16 flag)
{
    if (self != 0) {
        self->m_next->m_prev = self->m_prev;
        self->m_prev->m_next = self->m_next;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_5D44(CListNode *self)
{
    self->m_next->m_prev = self->m_prev;
    self->m_prev->m_next = self->m_next;
    self->m_next = self;
    self->m_prev = self;
}

void fn_1_5D68(CListNode *self, CListNode *head)
{
    CListNode *h;

    h = head;
    self->m_prev = h;
    self->m_next = h->m_next;
    h->m_next->m_prev = self;
    h->m_next = self;
}

void fn_1_5D98(CListNode *self, CListNode *list)
{
    CListNode *l;

    l = list;
    self->m_next = l;
    self->m_prev = l->m_prev;
    l->m_prev->m_next = self;
    l->m_prev = self;
}

// Moves every node of a to the end of b.
void fn_1_5DC8(CListNode *a, CListNode *b)
{
    CListNode *first;
    CListNode *last;
    u8 empty;

    empty = 0;
    if (a->m_next == a && a->m_next == a->m_prev) {
        empty = 1;
    }
    if (empty == 0) {
        first = a->m_next;
        last = a->m_prev;
        a->m_next = a;
        a->m_prev = a;
        first->m_prev = b->m_prev;
        last->m_next = b;
        b->m_prev->m_next = first;
        b->m_prev = last;
    }
}

CListNode *fn_1_5F08(CListCmp cmp, CListNode *list);
CListNode *fn_1_65B0(CListNode *list);
CListNode *fn_1_6604(CListCmp cmp, CListNode *a, CListNode *b);

void fn_1_5E50(CListNode *list, CListCmp cmp)
{
    CListNode *node;
    CListNode *prev;
    u8 empty;

    empty = 0;
    if (list->m_next == list && list->m_next == list->m_prev) {
        empty = 1;
    }
    if (empty == 0) {
        list->m_prev->m_next = 0;
        node = fn_1_5F08(cmp, list->m_next);
        list->m_next = node;
        prev = list;
        while (node != 0) {
            node->m_prev = prev;
            prev = node;
            node = node->m_next;
        }
        list->m_prev = prev;
        prev->m_next = list;
    }
}

// Merge sort on a singly linked chain (terminated by NULL).
CListNode *fn_1_5F08(CListCmp cmp, CListNode *list)
{
    CListNode *half;

    if (list == 0) {
        return 0;
    }
    if (list->m_next == 0) {
        return list;
    }
    half = fn_1_65B0(list);
    list = fn_1_5F08(cmp, list);
    half = fn_1_5F08(cmp, half);
    return fn_1_6604(cmp, list, half);
}

// Splits the chain in the middle and returns the second half.
CListNode *fn_1_65B0(CListNode *list)
{
    CListNode *slow;
    CListNode *fast;
    CListNode *half;

    slow = list;
    fast = slow->m_next;
    if (fast != 0) {
        fast = fast->m_next;
    }
    while (fast != 0) {
        slow = slow->m_next;
        fast = fast->m_next;
        if (fast != 0) {
            fast = fast->m_next;
        }
    }
    half = slow->m_next;
    slow->m_next = 0;
    return half;
}

CListNode *fn_1_6604(CListCmp cmp, CListNode *a, CListNode *b)
{
    CListNode head;
    CListNode *tail;

    tail = &head;
    while (a != 0 && b != 0) {
        if (cmp(a, b) != 0) {
            tail->m_next = a;
            tail = a;
            a = a->m_next;
        } else {
            tail->m_next = b;
            tail = b;
            b = b->m_next;
        }
    }
    if (a == 0) {
        tail->m_next = b;
    } else {
        tail->m_next = a;
    }
    return head.m_next;
}

// Object broadcast ---------------------------------------------------------------

// Iteration helpers of the intrusive list.
static inline CListNode *listBegin(CListNode *list)
{
    return list->m_next;
}

static inline CListNode *listNext(CListNode *node)
{
    return node->m_next;
}

static inline CListNode *listFirst(CListNode *list)
{
    return listBegin(list);
}

void fn_1_66B0(s32 bit)
{
    CListNode *it;
    CListNode *end;

    lbl_1_bss_24 = bit;
    for (it = listFirst(&lbl_1_bss_1C), end = &lbl_1_bss_1C; it != end; it = listNext(it)) {
        ((CObject *)it)->m_flags |= 1 << bit;
        ((void (*)(CObject *, s32))((CObject *)it)->m_vtable[3])((CObject *)it, bit);
    }
}

s32 fn_1_6748(void)
{
    CListNode *it;
    CListNode *end;
    s32 bit;

    for (it = listFirst(&lbl_1_bss_1C), end = &lbl_1_bss_1C; it != end; it = listNext(it)) {
        bit = lbl_1_bss_24;
        if ((((CObject *)it)->m_flags & (1 << bit)) != 0) {
            return 0;
        }
    }
    return 1;
}

void fn_1_67D4(void)
{
    CListNode *it;
    CListNode *end;

    for (it = listFirst(&lbl_1_bss_1C), end = &lbl_1_bss_1C; it != end; it = listNext(it)) {
        ((void (*)(CObject *))((CObject *)it)->m_vtable[4])((CObject *)it);
    }
}

void fn_1_6844(void)
{
}

// Deletes every object of the list.
void fn_1_6848(void)
{
    CObject *obj;
    u8 empty;

    while (1) {
        empty = 0;
        if (lbl_1_bss_1C.m_next == &lbl_1_bss_1C && lbl_1_bss_1C.m_next == lbl_1_bss_1C.m_prev) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        obj = (CObject *)lbl_1_bss_1C.m_prev;
        if (obj != 0) {
            ((void (*)(CObject *, s32))obj->m_vtable[2])(obj, 1);
        }
    }
}

CObject *fn_1_6910(CObject *self, s16 flag)
{
    if (self != 0) {
        self->m_vtable = lbl_1_data_21C;
        if (self != 0 && self != 0) {
            self->m_node.m_next->m_prev = self->m_node.m_prev;
            self->m_node.m_prev->m_next = self->m_node.m_next;
        }
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

s32 fn_1_6998(void)
{
    return lbl_1_bss_24;
}

CAppBase *fn_1_69A8(CAppBase *self)
{
    s32 i;

    if (lbl_1_bss_C == 0) {
    } else {
        fn_1_4C3C("ms_pointer == 0", "singleton.h", 0x35);
    }
    lbl_1_bss_C = self;
    self->m_vtbl = lbl_1_data_204;
    self->m_wait = 0;
    for (i = 0; i < 4; i++) {
        self->m_player[i] = -1;
    }
    return self;
}

CAppBase *fn_1_6A78(CAppBase *self, s16 flag)
{
    if (self != 0) {
        if (lbl_1_bss_C != 0) {
        } else {
            fn_1_4C3C("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_C = 0;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

CAppBase *fn_1_6B34(CAppBase *self, s16 flag)
{
    CAppBase *app;
    s32 i;
    s32 id;

    if (self != 0) {
        self->m_vtbl = lbl_1_data_204;
        if (mgDataTbl[GwSystem.mgNo].type != 3) {
            if (lbl_1_bss_C != 0) {
            } else {
                fn_1_4C3C("ms_pointer != 0", "singleton.h", 0x27);
            }
            app = lbl_1_bss_C;
            for (i = 0; i < 4; i++) {
                id = app->m_player[i];
                if (id >= 0 && _CheckFlag(0x1000F) == 0) {
                    GwPlayer[id].mgCoinBonus = 0xA;
                }
            }
        }
        if (self != 0) {
            if (lbl_1_bss_C != 0) {
            } else {
                fn_1_4C3C("ms_pointer != 0", "singleton.h", 0x3D);
            }
            lbl_1_bss_C = 0;
        }
        if (flag > 0) {
            fn_1_924C(self);
        }
    }
    return self;
}

void fn_1_6DD8(OMOBJ *obj);
void fn_1_6E68(CAppBase *app);
void fn_1_6ED8(OMOBJ *obj);
void fn_1_7680(void);

void fn_1_6CEC(CAppBase *self, s16 objMax, s32 prio)
{
    OMOBJMAN *objman;

    objman = omInitObjMan(objMax, prio);
    omGameSysInit(objman);
    ((void (*)(CAppBase *, OMOBJMAN *))self->m_vtbl[3])(self, objman);
    omAddObjEx(objman, 0, 0, 0, -1, fn_1_6DD8);
}

void fn_1_6D70(CAppBase *self)
{
    OMOBJMAN *objman;

    objman = MgActorObjectSetup();
    ((void (*)(CAppBase *, OMOBJMAN *))self->m_vtbl[3])(self, objman);
    omAddObjEx(objman, 0, 0, 0, -1, fn_1_6DD8);
}

void fn_1_6DD8(OMOBJ *obj)
{
    if (omSysExitReq != 0 && WipeCheckEnd() == 0) {
        fn_1_7680();
        HuAudFadeOut(0x3E8);
        obj->objFunc = fn_1_6ED8;
        return;
    }
    fn_1_6E68(lbl_1_bss_C);
}

void fn_1_6E68(CAppBase *app)
{
    ((void (*)(CAppBase *))app->m_vtbl[4])(app);
    if (app->m_wait > 0) {
        if (--app->m_wait == 0) {
        }
    }
}

void fn_1_6ED4(void)
{
}

void fn_1_6ED8(OMOBJ *obj)
{
    CAppBase *app;

    app = lbl_1_bss_C;
    if (WipeCheckEnd() != 0) {
        ((void (*)(CAppBase *))app->m_vtbl[4])(app);
        if (app->m_wait > 0) {
            if (--app->m_wait == 0) {
            }
        }
        return;
    }
    ((void (*)(CAppBase *))app->m_vtbl[5])(app);
    if (app != 0) {
        ((void (*)(CAppBase *, s32))app->m_vtbl[2])(app, 1);
    }
    CharModelKill(-1);
    omOvlReturnEx(1, 1);
}

// Registers a player id in the first free slot.
void fn_1_6FC4(s32 id)
{
    s32 *ids;
    s32 i;

    if (lbl_1_bss_C != 0) {
    } else {
        fn_1_4C3C("ms_pointer != 0", "singleton.h", 0x27);
    }
    ids = lbl_1_bss_C->m_player;
    for (i = 0; i < 4; i++) {
        if (ids[i] < 0) {
            ids[i] = id;
            return;
        }
    }
}

s32 fn_1_7098(s32 id)
{
    s32 i;

    if (lbl_1_bss_C != 0) {
    } else {
        fn_1_4C3C("ms_pointer != 0", "singleton.h", 0x27);
    }
    for (i = 0; i < 4; i++) {
        if (id == lbl_1_bss_C->m_player[i]) {
            return 1;
        }
    }
    return 0;
}

// singleton.h accessor (asserts that the instance exists).
static inline CAppBase *appInstance()
{
    if (lbl_1_bss_C != 0) {
    } else {
        fn_1_4C3C("ms_pointer != 0", "singleton.h", 0x27);
    }
    return lbl_1_bss_C;
}

void fn_1_716C(void)
{
    CAppBase *app;

    app = appInstance();
    app->m_mes = GameMesCreate(2, 0);
}

void fn_1_7224(void)
{
    CAppBase *app;

    app = appInstance();
    app->m_mes = GameMesCreate(2, 1);
}

s32 fn_1_72DC(void)
{
    CAppBase *app;
    s32 result;

    if (lbl_1_bss_C != 0) {
    } else {
        fn_1_4C3C("ms_pointer != 0", "singleton.h", 0x27);
    }
    app = lbl_1_bss_C;
    result = 0;
    if (GameMesStatGet(-1) == 0 && (u8)(app->m_wait <= 0) != 0) {
        result = 1;
    }
    return result;
}

// Shows the winners: the registered players sorted by id.
void fn_1_73B0(void)
{
    s32 chars[4];
    s32 *ids;
    s32 count;
    s32 i;
    s32 j;
    s32 k;
    s32 t;
    s16 charNo;

    if (lbl_1_bss_C != 0) {
    } else {
        fn_1_4C3C("ms_pointer != 0", "singleton.h", 0x27);
    }
    lbl_1_bss_C->m_wait = lbl_1_bss_2C;
    ids = lbl_1_bss_C->m_player;
    count = 0;
    while (count < 4) {
        if (ids[count] >= 0) {
            count++;
        } else {
            break;
        }
    }
    if (count == 0) {
        GameMesCreate(2, 2);
        return;
    }
    for (i = 1; i < count; i++) {
        j = i;
        while (j >= 1 && ids[j - 1] > ids[j]) {
            t = ids[j - 1];
            ids[j - 1] = ids[j];
            ids[j] = t;
            j--;
        }
    }
    for (k = 0; k < 4; k++) {
        if (ids[k] < 0) {
            charNo = -1;
        } else {
            charNo = GwPlayerConf[ids[k]].charNo;
        }
        chars[k] = charNo;
    }
    GameMesCreate(4, 3, chars[0], chars[1], chars[2], chars[3]);
}

void fn_1_757C(s32 arg)
{
    CAppBase *app;

    if (_CheckFlag(0x1000F) == 0) {
        if (lbl_1_bss_C != 0) {
        } else {
            fn_1_4C3C("ms_pointer != 0", "singleton.h", 0x27);
        }
        app = lbl_1_bss_C;
        app->m_mes = GameMesCreate(0xE, arg);
        HuAudJinglePlay(0x86);
    }
}

void fn_1_7654(void)
{
    WipeCreate(1, 5, 0x3C);
}

void fn_1_7680(void)
{
    WipeCreate(2, 6, 0x3C);
}

void fn_1_76AC(void)
{
    omSysExitReq = 1;
}

// CModel ----------------------------------------------------------------------

CModel *fn_1_76C0(CModel *self, void *data)
{
    self->m_id = Hu3DModelCreate(data);
    return self;
}

CModel *__ct__6CModelFUi(CModel *self, u32 dataNum)
{
    self->m_id = Hu3DModelCreate(HuDataSelHeapReadNum(dataNum, 0x10000000, HEAP_MODEL));
    return self;
}

CModel *fn_1_7744(CModel *self, HU3DMODELHOOK hook)
{
    self->m_id = Hu3DHookFuncCreate(hook);
    return self;
}

CModel *fn_1_7780(CModel *self, s16 charNo, s32 kind)
{
    s16 model;

    model = 0;
    switch (kind) {
    case 0:
        model = 1;
        break;
    case 1:
        model = 2;
        break;
    case 2:
        model = 4;
        break;
    case 3:
        model = 8;
        break;
    }
    self->m_id = CharModelCreate(charNo, model);
    return self;
}

CModel *fn_1_781C(CModel *self, CModel *other)
{
    self->m_id = Hu3DModelLink(other->m_id);
    return self;
}

void fn_1_7870(CModel *self, u32 attr)
{
    Hu3DModelAttrSet(self->m_id, attr);
}

void fn_1_78A8(CModel *self, u32 attr)
{
    Hu3DModelAttrReset(self->m_id, attr);
}

void fn_1_78E0(CModel *self, CXform *xf)
{
    s16 id;

    id = self->m_id;
    Hu3DModelPosSet(id, xf->m_pos.x, xf->m_pos.y, xf->m_pos.z);
    Hu3DModelRotSet(id, xf->m_rot.x, xf->m_rot.y, xf->m_rot.z);
    Hu3DModelScaleSet(id, xf->m_scale.x, xf->m_scale.y, xf->m_scale.z);
}

void fn_1_795C(CModel *self, const Vec *pos)
{
    Hu3DModelPosSet(self->m_id, pos->x, pos->y, pos->z);
}

void fn_1_79A4(CModel *self, const Vec *rot)
{
    Hu3DModelRotSet(self->m_id, rot->x, rot->y, rot->z);
}

void fn_1_79EC(CModel *self, const Vec *scale)
{
    Hu3DModelScaleSet(self->m_id, scale->x, scale->y, scale->z);
}

void fn_1_7A34(CModel *self, f32 scale)
{
    Hu3DModelScaleSet(self->m_id, scale, scale, scale);
}

void fn_1_7A7C(CModel *self, f32 level)
{
    Hu3DModelTPLvlSet(self->m_id, level);
}

void fn_1_7AB4(CModel *self, s16 layer)
{
    Hu3DModelLayerSet(self->m_id, layer);
}

void fn_1_7AEC(CModel *self, u16 cameraBit)
{
    Hu3DModelCameraSet(self->m_id, cameraBit);
}

void fn_1_7B24(const HU3DMODELID *modelIdPtr, void *data)
{
    Hu3DData[*modelIdPtr].hookData = data;
}

MtxPtr CModel::getMatrix()
{
    return Hu3DData[m_id].mtx;
}

CModel *__dt__6CModelFv(CModel *self, s16 flag)
{
    if (self != 0) {
        Hu3DModelKill(self->m_id);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

HSFOBJECT *fn_1_7BB8(CModel *self, char *name)
{
    HSFOBJECT *obj;

    obj = Hu3DModelObjPtrGet(self->m_id, name);
    if (obj != 0) {
    } else {
        fn_1_4C3C("p != NULL", "model.cpp", 0x8A);
    }
    return obj;
}

void fn_1_7C4C(CModel *self)
{
    Hu3DModelShadowSet(self->m_id);
}

void fn_1_7C7C(CModel *self)
{
    Hu3DModelShadowMapSet(self->m_id);
}

void fn_1_7CAC(CModel *self, char *name)
{
    Hu3DModelShadowMapObjSet(self->m_id, name);
}

void fn_1_7CE4(CModel *self)
{
    Hu3DModelShadowMapObjReset(self->m_id);
}

void fn_1_7D14(CModel *self, char *name)
{
    HSFOBJECT *obj;
    u32 *flags;

    obj = fn_1_7BB8(self, name);
    flags = (u32 *)obj->constData;
    *flags = *flags & 0xFFFFFFF7;
}

void fn_1_7DC0(CModel *self, char *name)
{
    HSFOBJECT *obj;

    obj = fn_1_7BB8(self, name);
    obj->flags |= 0x400;
}

void fn_1_7E68(CModel *self, char *name)
{
    HSFOBJECT *obj;

    obj = fn_1_7BB8(self, name);
    obj->flags &= 0xFFFFFBFF;
}

// Hides an object of the model (flag plus zero scale).
void fn_1_7F10(CModel *self, char *name)
{
    HSFOBJECT *obj;

    obj = fn_1_7BB8(self, name);
    obj->flags |= 0x400;
    obj->mesh.base.scale.x = 0.0f;
    obj->mesh.base.scale.y = 0.0f;
    obj->mesh.base.scale.z = 0.0f;
}

void fn_1_7FEC(CModel *self, char *name)
{
    HSFOBJECT *obj;

    obj = fn_1_7BB8(self, name);
    obj->flags &= 0xFFFFFBFF;
    obj->mesh.base.scale.x = 1.0f;
    obj->mesh.base.scale.y = 1.0f;
    obj->mesh.base.scale.z = 1.0f;
}

void fn_1_80C8(CModel *self, s32 frame)
{
    Hu3DMotionTimeSet(self->m_id, (f32)frame);
}

// Mersenne twister ----------------------------------------------------------------

void fn_1_8134(CMt *m, u32 seed);
void fn_1_81A8(CMt *m, const u32 *key, u32 len);
u32 fn_1_836C(CMt *m);
u32 fn_1_84F8(CMt *m);
f32 fn_1_8690(CMt *m);
f32 fn_1_8854(CMt *m);
f32 fn_1_8A18(CMt *m);
f64 fn_1_8BEC(CMt *m);
f32 fn_1_8F64(CMt *m);

// Next raw 32 bit output (regenerates the state table when it is used up).
static inline u32 mtNext(CMt *m)
{
    u32 y;
    u32 kk;

    if (m->m_idx >= 0x270) {
        kk = 0;
        while (kk < 0xE3) {
            y = (m->m_mt[kk] & 0x80000000) | (m->m_mt[kk + 1] & 0x7FFFFFFF);
            m->m_mt[kk] = lbl_1_rodata_160[y & 1] ^ ((y >> 1) ^ m->m_mt[kk + 0x18D]);
            kk++;
        }
        while (kk < 0x26F) {
            y = (m->m_mt[kk] & 0x80000000) | (m->m_mt[kk + 1] & 0x7FFFFFFF);
            m->m_mt[kk] = lbl_1_rodata_160[y & 1] ^ ((y >> 1) ^ m->m_mt[kk - 0xE3]);
            kk++;
        }
        y = (m->m_mt[0x26F] & 0x80000000) | (m->m_mt[0] & 0x7FFFFFFF);
        m->m_mt[0x26F] = (y >> 1) ^ m->m_mt[0x18C] ^ lbl_1_rodata_160[y & 1];
        m->m_idx = 0;
    }
    y = m->m_mt[m->m_idx++];
    y = y ^ (y >> 11);
    y = y ^ ((y << 7) & 0x9D2C5680);
    y = y ^ ((y << 15) & 0xEFC60000);
    return y ^ (y >> 18);
}

void fn_1_8134(CMt *m, u32 seed)
{
    m->m_mt[0] = seed;
    m->m_idx = 1;
    while (m->m_idx < 0x270) {
        m->m_mt[m->m_idx] = m->m_idx + ((m->m_mt[m->m_idx - 1] ^ (m->m_mt[m->m_idx - 1] >> 30)) * 0x6C078965);
        m->m_idx++;
    }
}

void fn_1_81A8(CMt *m, const u32 *key, u32 len)
{
    u32 i;
    u32 j;
    u32 k;

    m->m_mt[0] = 0x012BD6AA;
    m->m_idx = 1;
    while (m->m_idx < 0x270) {
        m->m_mt[m->m_idx] = m->m_idx + ((m->m_mt[m->m_idx - 1] ^ (m->m_mt[m->m_idx - 1] >> 30)) * 0x6C078965);
        m->m_idx++;
    }
    i = 1;
    j = 0;
    if (len < 0x270) {
        k = 0x270;
    } else {
        k = len;
    }
    while (k != 0) {
        m->m_mt[i] = key[j] + ((m->m_mt[i] ^ ((m->m_mt[i - 1] ^ (m->m_mt[i - 1] >> 30)) * 0x19660D)) + j);
        i++;
        j++;
        if (i >= 0x270) {
            m->m_mt[0] = m->m_mt[0x26F];
            i = 1;
        }
        if (j >= len) {
            j = 0;
        }
        k--;
    }
    k = 0x26F;
    while (k != 0) {
        m->m_mt[i] = (m->m_mt[i] ^ ((m->m_mt[i - 1] ^ (m->m_mt[i - 1] >> 30)) * 0x5D588B65)) - i;
        i++;
        if (i >= 0x270) {
            m->m_mt[0] = m->m_mt[0x26F];
            i = 1;
        }
        k--;
    }
    m->m_mt[0] = 0x80000000;
}

u32 fn_1_836C(CMt *m)
{
    return mtNext(m);
}

u32 fn_1_84F8(CMt *m)
{
    return mtNext(m) >> 1;
}

f32 fn_1_8690(CMt *m)
{
    return 2.3283064e-10f * (f32)mtNext(m);
}

f32 fn_1_8854(CMt *m)
{
    return 2.3283064e-10f * (f32)mtNext(m);
}

f32 fn_1_8A18(CMt *m)
{
    return 2.3283064e-10f * (0.5f + (f32)mtNext(m));
}

f64 fn_1_8BEC(CMt *m)
{
    u32 a;
    u32 b;

    a = mtNext(m);
    b = mtNext(m);
    return 1.1102230246251565e-16 * ((67108864.0 * (f64)(a >> 5)) + (f64)(b >> 6));
}

f32 fn_1_8F64(CMt *m)
{
    return 1.1920929e-7f * (f32)(mtNext(m) >> 9);
}

// Task objects ---------------------------------------------------------------------

void *fn_1_9134(u32 n)
{
    void *p;

    p = HuMemDirectMallocNum(HEAP_HEAP, n, 0x10000000);
    if (p != 0) {
    } else {
        fn_1_4C3C("ptr != NULL", "om.cpp", 0x18);
    }
    return p;
}

void *fn_1_91C0(u32 n)
{
    void *p;

    p = HuMemDirectMallocNum(HEAP_HEAP, n, 0x10000000);
    if (p != 0) {
    } else {
        fn_1_4C3C("ptr != NULL", "om.cpp", 0x1F);
    }
    return p;
}

void fn_1_924C(void *p)
{
    if (p != 0) {
        HuMemDirectFree(p);
    }
}

void fn_1_9284(void *p)
{
    if (p != 0) {
        HuMemDirectFree(p);
    }
}

void fn_1_92DC(OMOBJ *obj);

void fn_1_92BC(CTask *task, OMOBJ *obj)
{
    task->m_vtbl = lbl_1_data_1F4;
    obj->data = task;
    obj->objFunc = fn_1_92DC;
}

void fn_1_92DC(OMOBJ *obj)
{
    CTask *task;

    task = (CTask *)obj->data;
    ((void (*)(CTask *, OMOBJ *))task->m_vtbl[3])(task, obj);
}

void fn_1_9328(void)
{
}

void fn_1_932C(OMOBJ *obj)
{
    CTask *task;

    task = (CTask *)obj->data;
    if (task != 0) {
        ((void (*)(CTask *, s32))task->m_vtbl[2])(task, 1);
    }
    omDelObjEx((OMOBJMAN *)HuPrcCurrentGet(), obj);
}

CTask *fn_1_939C(CTask *self, s16 flag)
{
    if (self != 0) {
        self->m_vtbl = lbl_1_data_1F4;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_93FC(OMOBJMAN *objman, OMOBJ *obj)
{
    OMOBJMAN *man;
    CTask *task;

    man = objman;
    task = (CTask *)obj->data;
    if (task != 0) {
        ((void (*)(CTask *, s32))task->m_vtbl[2])(task, 1);
    }
    omDelObjEx(man, obj);
}

// Global random generator ---------------------------------------------------------------

void fn_1_9470(u32 seed)
{
    lbl_1_bss_34.m_mt[0] = seed;
    lbl_1_bss_34.m_idx = 1;
    while (lbl_1_bss_34.m_idx < 0x270) {
        lbl_1_bss_34.m_mt[lbl_1_bss_34.m_idx] = lbl_1_bss_34.m_idx
            + ((lbl_1_bss_34.m_mt[lbl_1_bss_34.m_idx - 1] ^ (lbl_1_bss_34.m_mt[lbl_1_bss_34.m_idx - 1] >> 30)) * 0x6C078965);
        lbl_1_bss_34.m_idx++;
    }
}

u32 fn_1_9548(void)
{
    return mtNext(&lbl_1_bss_34) >> 1;
}

f32 fn_1_9784(void)
{
    return 1.1920929e-7f * (f32)(mtNext(&lbl_1_bss_34) >> 9);
}

f32 fn_1_99F8(f32 max)
{
    return (f32)((f64)max * fn_1_8BEC(&lbl_1_bss_34));
}

f32 fn_1_9A30(f32 min, f32 max)
{
    return (f32)((f64)min + ((f64)(max - min) * fn_1_8BEC(&lbl_1_bss_34)));
}

// Sphere overlap test.
u32 fn_1_9A84(const M521Sphere *a, const M521Sphere *b)
{
    Vec d;
    f32 sumR;
    f32 dist;
    u32 hit;

    PSVECSubtract((Vec *)a, (Vec *)b, &d);
    sumR = a->r + b->r;
    dist = PSVECMag(&d);
    hit = dist < sumR;
    return hit;
}

// Pushes two overlapping spheres apart (f0 = share moved by the first, f1 = stiffness).
s32 fn_1_9B2C(M521Sphere *a, M521Sphere *b, f32 f0, f32 f1)
{
    Vec d;
    Vec step;
    f32 sumR;
    f32 dist;

    sumR = a->r + b->r;
    PSVECSubtract((Vec *)b, (Vec *)a, &d);
    dist = PSVECMag(&d);
    if (dist >= sumR) {
        return 0;
    }
    PSVECScale(&d, &d, (f1 * (sumR - dist)) / dist);
    PSVECScale(&d, &step, -f0);
    PSVECAdd(&step, (Vec *)a, (Vec *)a);
    PSVECScale(&d, &step, 1.0f - f0);
    PSVECAdd(&step, (Vec *)b, (Vec *)b);
    return 1;
}

s32 fn_1_9CAC(void)
{
    return OSGetThreadPriority(OSGetCurrentThread());
}

// Threads ---------------------------------------------------------------------------------

void *fn_1_9F94(void *arg);
OSThread *fn_1_9FF0(CThread *th, void *(*entry)(void *), void *arg, u32 size, s32 prio, u16 attr);
void fn_1_A150(CThread *th, void *(*entry)(void *), void *arg, u32 *stack, u32 words, s32 prio, u16 attr);
CThread *fn_1_A274(CThread *th, s16 flag);
u32 fn_1_A3A8(CThread *th);
s32 fn_1_A448(u32 *p, s32 mode);
void fn_1_A4BC(u32 *p, u32 words);

CSubThread *fn_1_9CD0(CSubThread *self, u32 stackSize)
{
    self->m_vtbl = lbl_1_data_1E4;
    fn_1_9FF0(&self->m_thread, fn_1_9F94, self, stackSize, OSGetThreadPriority(OSGetCurrentThread()), 1);
    OSInitSemaphore(&self->m_sem, 0);
    return self;
}

CSubThread *fn_1_9D4C(CSubThread *self, s16 flag)
{
    if (self != 0) {
        self->m_vtbl = lbl_1_data_1E4;
        fn_1_A274(&self->m_thread, -1);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

u8 fn_1_9DB8(CSubThread *self)
{
    return OSIsThreadTerminated(&self->m_thread.m_th) != 0;
}

s32 fn_1_9E00(CSubThread *self, s32 arg)
{
    if (OSIsThreadTerminated(&self->m_thread.m_th) != 0) {
        return self->m_arg;
    }
    self->m_arg = arg;
    OSResumeThread(&self->m_thread.m_th);
    OSWaitSemaphore(&self->m_sem);
    fn_1_A3A8(&self->m_thread);
    OSCheckActiveThreads();
    return self->m_arg;
}

s32 fn_1_9E90(CSubThread *self, s32 arg)
{
    if (OSGetCurrentThread() == &self->m_thread.m_th) {
    } else {
        fn_1_4C3C("::OSGetCurrentThread() == m_th", "subthread.cpp", 0x47);
    }
    if (OSIsThreadTerminated(&self->m_thread.m_th) == 0) {
    } else {
        fn_1_4C3C("!::OSIsThreadTerminated(m_th)", "subthread.cpp", 0x4A);
    }
    self->m_arg = arg;
    OSSignalSemaphore(&self->m_sem);
    OSSuspendThread(&self->m_thread.m_th);
    return self->m_arg;
}

void *fn_1_9F94(void *arg)
{
    CSubThread *self;

    self = (CSubThread *)arg;
    ((void (*)(CSubThread *, s32))self->m_vtbl[3])(self, self->m_arg);
    OSSignalSemaphore(&self->m_sem);
    return 0;
}

OSThread *fn_1_9FF0(CThread *th, void *(*entry)(void *), void *arg, u32 size, s32 prio, u16 attr)
{
    u32 words;
    u32 *stack;

    words = size >> 2;
    stack = (u32 *)fn_1_4A68(words * 4);
    th->m_stack = stack;
    fn_1_A150(th, entry, arg, stack, words, prio, attr);
    return &th->m_th;
}

OSThread *fn_1_A0D4(CThread *th, void *(*entry)(void *), void *arg, u32 *stack, u32 size, s32 prio, u16 attr)
{
    th->m_stack = 0;
    fn_1_A150(th, entry, arg, stack, size >> 2, prio, attr);
    return &th->m_th;
}

void fn_1_A150(CThread *th, void *(*entry)(void *), void *arg, u32 *stack, u32 words, s32 prio, u16 attr)
{
    u8 valid;
    BOOL ok;

    valid = 0;
    if (prio >= 0 && prio <= 0x1F) {
        valid = 1;
    }
    if (valid != 0) {
    } else {
        fn_1_4C3C("priority >= OS_PRIORITY_MIN && priority <= OS_PRIORITY_MAX", "thread.cpp", 0x52);
    }
    fn_1_A4BC(stack, words);
    fn_1_A448(stack, 0);
    ok = OSCreateThread(&th->m_th, entry, arg, &stack[words], words * 4, prio, attr);
    if ((u8)(ok != 0) == 0) {
        OSPanic("thread.cpp", 0x60, "OSCreateThread() failed.");
    }
}

CThread *fn_1_A274(CThread *th, s16 flag)
{
    u32 total;
    u32 remain;
    u32 *stack;

    if (th != 0) {
        if (OSIsThreadTerminated(&th->m_th) == 0) {
            OSCancelThread(&th->m_th);
        }
        if (lbl_1_bss_9F8 != 0) {
            total = (u8 *)th->m_th.stackBase - (u8 *)th->m_th.stackEnd;
            remain = fn_1_A3A8(th);
            printf("delete CThread@%p\nstack remain/total = 0x%08x/0x%08x (%f%%)\n", th, remain, total,
                   (100.0 * (f64)remain) / (f64)total);
        }
        stack = th->m_stack;
        if (stack != 0) {
            HuMemDirectFree(stack);
        }
        if (flag > 0 && th != 0) {
            HuMemDirectFree(th);
        }
    }
    return th;
}

// Free stack bytes (checks the guard words first).
u32 fn_1_A3A8(CThread *th)
{
    u32 *base;
    u32 *p;
    u32 *top;

    base = (u32 *)th->m_th.stackEnd;
    if (fn_1_A448(base, 1) != 0) {
        OSPanic("thread.cpp", 0x7F, "stack overflow\n");
    }
    p = base;
    top = (u32 *)th->m_th.stackBase;
    while (p < top) {
        if (*p == 0xDEADBABE) {
            p++;
        } else {
            break;
        }
    }
    return (u8 *)p - (u8 *)base;
}

s32 fn_1_A448(u32 *p, s32 mode)
{
    u32 *end;

    end = p + 8;
    switch (mode) {
    case 0:
        while (p != end) {
            *p = 0xDEADBABE;
            p++;
        }
        break;
    case 1:
        while (p != end) {
            if (*p++ != 0xDEADBABE) {
                return 1;
            }
        }
        break;
    }
    return 0;
}

void fn_1_A4BC(u32 *p, u32 words)
{
    u32 *end;

    if (lbl_1_bss_9F8 != 0) {
        end = p + words;
        while (p != end) {
            *p = 0xDEADBABE;
            p++;
        }
    }
}

// Timer wrapper -----------------------------------------------------------------------------

CTimer *fn_1_A508(CTimer *self, s32 mode)
{
    switch (mode) {
    case 0:
        self->m_timer = MgTimerCreate(0);
        break;
    case 1:
        self->m_timer = MgTimerCreate(1);
        break;
    case 2:
        self->m_timer = MgTimerCreate(2);
        break;
    default:
        fn_1_4C3C("false", "timer.cpp", 0x1E);
        break;
    }
    return self;
}

CTimer *fn_1_A5D0(CTimer *self, s16 flag)
{
    if (self != 0) {
        MgTimerKill(self->m_timer);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_A62C(CTimer *self, s32 maxTime, s32 endTime, s32 recordTime)
{
    MgTimerParamSet(self->m_timer, maxTime, endTime, recordTime);
}

s32 fn_1_A674(CTimer *self)
{
    return MgTimerValueGet(self->m_timer);
}

void fn_1_A6A4(CTimer *self, f32 x, f32 y)
{
    MgTimerPosSet(self->m_timer, x, y);
}

void fn_1_A6E4(CTimer *self, f32 *x, f32 *y)
{
    MgTimerPosGet(self->m_timer, x, y);
}

void fn_1_A724(CTimer *self, s32 mode)
{
    switch (mode) {
    case 0:
        MgTimerModeOnSet(self->m_timer, 0);
        return;
    case 1:
        MgTimerModeOnSet(self->m_timer, 1);
        return;
    case 2:
        MgTimerModeOnSet(self->m_timer, 2);
        return;
    default:
        fn_1_4C3C("false", "timer.cpp", 0x48);
        return;
    }
}

void fn_1_A7E8(CTimer *self)
{
    MgTimerModeOffSet(self->m_timer);
}

void fn_1_A818(CTimer *self)
{
    if (self->m_timer->stopF == 0) {
        self->m_timer->stopF = 1;
        self->m_timer->mode = 1;
    }
}

void fn_1_A844(CTimer *self)
{
    MgTimerKill(self->m_timer);
    self->m_timer = MgTimerCreate(0);
}

u8 fn_1_A880(CTimer *self)
{
    return MgTimerDoneCheck(self->m_timer) != 0;
}

void fn_1_A8C0(CTimer *self, u8 r, u8 g, u8 b)
{
    MgTimerWinColorSet(self->m_timer, r, g, b);
}

CListNode *fn_1_AA68(CListNode *self, s16 flag);

void fn_1_A908(void)
{
    lbl_1_bss_1C.m_next = &lbl_1_bss_1C;
    lbl_1_bss_1C.m_prev = &lbl_1_bss_1C;
    __register_global_object(&lbl_1_bss_1C, (void *)fn_1_AA68, lbl_1_bss_10);
    lbl_1_bss_28 = 0x78;
    lbl_1_bss_2C = 0xD2;
    lbl_1_bss_30 = 0xD2;
    lbl_1_bss_34.m_mt[0] = 0x1571;
    lbl_1_bss_34.m_idx = 1;
    while (lbl_1_bss_34.m_idx < 0x270) {
        lbl_1_bss_34.m_mt[lbl_1_bss_34.m_idx] = lbl_1_bss_34.m_idx
            + ((lbl_1_bss_34.m_mt[lbl_1_bss_34.m_idx - 1] ^ (lbl_1_bss_34.m_mt[lbl_1_bss_34.m_idx - 1] >> 30)) * 0x6C078965);
        lbl_1_bss_34.m_idx++;
    }
}

CListNode *fn_1_AA68(CListNode *self, s16 flag)
{
    if (self != 0) {
        if (self != 0) {
            self->m_next->m_prev = self->m_prev;
            self->m_prev->m_next = self->m_next;
        }
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}
}
