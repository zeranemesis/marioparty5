#include <stdarg.h>
#include <stdio.h>

#include "dolphin.h"

#include "game/audio.h"
#include "game/charman.h"
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

#ifndef TARGET_PC
/* Not declared by the GameCube headers of this project. */
typedef struct OSSemaphore {
    s32 count;
    OSThreadQueue queue;
} OSSemaphore;

extern "C" void OSInitSemaphore(OSSemaphore *sem, s32 count);
extern "C" s32 OSWaitSemaphore(OSSemaphore *sem);
extern "C" s32 OSSignalSemaphore(OSSemaphore *sem);
#endif

/* Layout shared by the camera helpers: position, up vector, target. */
struct Mgr511Pose {
    Vec pos;
    Vec up;
    Vec target;
};

/* Position and orientation of a camera. */
struct Mgr511Xform {
    Vec pos;
    Quaternion rot;
};

/* Object list node: link pointers, vtable and pending-flag mask. */
struct Mgr511Node {
    Mgr511Node *next;
    Mgr511Node *prev;
    void **vtable;
    u32 flags;
};

/* Head of the global object list. */
struct Mgr511NodeList {
    Mgr511Node *next;
    Mgr511Node *prev;
};

#define LIST_END ((Mgr511Node *)&lbl_1_bss_2C)

/* Mersenne Twister state: 624 words and the read index. */
struct Mgr511Mt {
    u32 mt[0x270];
    u32 index;
};

typedef union {
    f32 f;
    u32 u;
} Mgr511FloatBits;

extern "C" {

void ObjectSetup(void);
#ifndef TARGET_PC
extern void (*_ctors[])(void);
extern void (*_dtors[])(void);
#endif
void __register_global_object(void *obj, void *(*dtor)(void *, s16), void *reg);

void fn_1_CF8(char *cond, char *file, s32 line);
void *__nw__FUl(unsigned long size);
void *__nwa__FUl(unsigned long size);
void *__dl__FPv(void *p);
void *__dla__FPv(void *p);

/* Globals owned by this unit. */
u8 lbl_1_bss_20[12];
Mgr511NodeList lbl_1_bss_2C;
s32 lbl_1_bss_34;
s32 lbl_1_bss_38;
s32 lbl_1_bss_3C;
s32 lbl_1_bss_40;
Mgr511Mt lbl_1_bss_44;
u8 lbl_1_bss_A08;
void *lbl_1_bss_A0C;

extern void *lbl_1_data_21C[4];
extern void *lbl_1_data_22C[4];
extern void *lbl_1_data_23C[7];

void *__nw__FUl(unsigned long size)
{
    void *p;

    p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);
    if (p != 0) {
    } else {
        fn_1_CF8("ptr != NULL", "new.cpp", 0x17);
    }
    return p;
}

void *__nwa__FUl(unsigned long size)
{
    void *p;

    p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);
    if (p != 0) {
    } else {
        fn_1_CF8("ptr != NULL", "new.cpp", 0x17);
    }
    return p;
}

void *__dl__FPv(void *p)
{
    if (p != 0) {
        HuMemDirectFree(p);
    }
    return p;
}

void *__dla__FPv(void *p)
{
    if (p != 0) {
        HuMemDirectFree(p);
    }
    return p;
}

void _prolog(void)
{
#ifdef TARGET_PC
    ObjectSetup();
#else
    void (**ctor)(void);

    ctor = _ctors;
    while (*ctor != 0) {
        (*ctor)();
        ctor++;
    }
    ObjectSetup();
#endif
}

void _epilog(void)
{
#ifndef TARGET_PC
    void (**dtor)(void);

    dtor = _dtors;
    while (*dtor != 0) {
        (*dtor)();
        dtor++;
    }
#endif
}

void fn_1_CF8(char *cond, char *file, s32 line)
{
    char buf[0x400];

    OSPanic("assert.cpp", 0x12, buf, sprintf(buf, "\nASSERTION FAILED at %s:%d\n%s", file, line, cond));
}

#ifndef TARGET_PC
/* MSL stream functions are not available: only the format string is used. */
int fprintf(void *fp, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    return vprintf(fmt, args);
}
#endif

void fn_1_F2C(Mgr511Pose *p, Vec *x, Vec *y, Vec *z);
void fn_1_1090(Mgr511Pose *p, Vec *d);
void fn_1_1544(Mgr511Pose *p);

void fn_1_DFC(Mgr511Pose *p, f32 yaw, f32 pitch)
{
    Vec right;
    Vec upv;
    Vec fwd;
    Mtx m1;
    Mtx m2;
    Mtx m3;

    fn_1_F2C(p, &right, &upv, &fwd);
    PSMTXRotAxisRad(m1, &right, 0.017453292f * yaw);
    PSMTXRotRad(m2, 'Y', 0.017453292f * pitch);
    PSMTXConcat(m1, m2, m3);
    PSMTXMultVecSR(m3, &fwd, &fwd);
    PSMTXMultVecSR(m3, &upv, &p->up);
    PSVECSubtract(&p->pos, &fwd, &p->target);
}

void fn_1_EC0(Mgr511Pose *p, f32 deg)
{
    Vec d;
    Mtx m;

    PSVECSubtract(&p->target, &p->pos, &d);
    PSMTXRotAxisRad(m, &d, 0.017453292f * deg);
    PSMTXMultVec(m, &p->up, &p->up);
}

void fn_1_F2C(Mgr511Pose *p, Vec *x, Vec *y, Vec *z)
{
    Mtx m;

    C_MTXLookAt(m, &p->pos, &p->up, &p->target);
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

void fn_1_FD4(Mgr511Pose *p, Mtx out)
{
    C_MTXLookAt(out, &p->pos, &p->up, &p->target);
    PSMTXInverse(out, out);
}

void fn_1_1028(Mgr511Pose *p, Vec *delta)
{
    Mtx m;
    Vec t;

    C_MTXLookAt(m, &p->pos, &p->up, &p->target);
    PSMTXInverse(m, m);
    PSMTXMultVecSR(m, delta, &t);
    fn_1_1090(p, &t);
}

void fn_1_1090(Mgr511Pose *p, Vec *d)
{
    PSVECAdd(d, &p->target, &p->target);
    PSVECAdd(d, &p->pos, &p->pos);
}

void fn_1_10E4(Mgr511Pose *p)
{
    p->up.x = 0.0f;
    p->up.y = 1.0f;
    p->up.z = 0.0f;
    p->pos.x = 0.0f;
    p->pos.y = 0.0f;
    p->pos.z = 5000.0f;
    p->target.x = 0.0f;
    p->target.y = 0.0f;
    p->target.z = 0.0f;
}

void fn_1_1178(Mgr511Pose *p, Mgr511Xform *x)
{
    Mtx m;

    PSMTXQuat(m, &x->rot);
    p->up.x = m[0][1];
    p->up.y = m[1][1];
    p->up.z = m[2][1];
    p->pos.x = x->pos.x;
    p->pos.y = x->pos.y;
    p->pos.z = x->pos.z;
    p->target.x = x->pos.x - m[0][2];
    p->target.y = x->pos.y - m[1][2];
    p->target.z = x->pos.z - m[2][2];
}

void fn_1_1218(Mgr511Pose *p, Mgr511Xform *x)
{
    Mtx m;

    C_MTXLookAt(m, &p->pos, &p->up, &p->target);
    PSMTXInverse(m, m);
    x->pos.x = p->pos.x;
    x->pos.y = p->pos.y;
    x->pos.z = p->pos.z;
    C_QUATMtx(&x->rot, m);
}

void fn_1_1290(Mgr511Pose *p, Mgr511Xform *a, Mgr511Xform *b, f32 t)
{
    Quaternion q;
    Vec pos;
    Vec pa;
    Vec pb;
    Mtx m;

    C_QUATSlerp(&a->rot, &b->rot, &q, t);
    PSVECScale(&a->pos, &pa, 1.0f - t);
    PSVECScale(&b->pos, &pb, t);
    PSVECAdd(&pa, &pb, &pos);
    PSMTXQuat(m, &q);
    p->up.x = m[0][1];
    p->up.y = m[1][1];
    p->up.z = m[2][1];
    p->pos.x = pos.x;
    p->pos.y = pos.y;
    p->pos.z = pos.z;
    p->target.x = pos.x - m[0][2];
    p->target.y = pos.y - m[1][2];
    p->target.z = pos.z - m[2][2];
}

void fn_1_13A0(Mgr511Pose *p, Vec *np)
{
    Vec d;

    PSVECSubtract(np, &p->pos, &d);
    p->pos.x = np->x;
    p->pos.y = np->y;
    p->pos.z = np->z;
    PSVECAdd(&p->target, &d, &p->target);
}

void fn_1_140C(Mgr511Pose *p, Vec *a, Vec *b, f32 t)
{
    Vec s1;
    Vec s2;
    Vec d;
    Vec *pa;
    Vec *pb;

    pa = a;
    pb = b;
    PSVECScale(pa, &s1, 1.0f - t);
    PSVECScale(pb, &s2, t);
    PSVECAdd(&s1, &s2, &s2);
    PSVECSubtract(&s2, &p->pos, &d);
    p->pos.x = s2.x;
    p->pos.y = s2.y;
    p->pos.z = s2.z;
    PSVECAdd(&p->target, &d, &p->target);
}

void fn_1_14C4(Mgr511Pose *p, u16 cameraBit)
{
    Hu3DCameraPosSetV(cameraBit, &p->pos, &p->up, &p->target);
}

void fn_1_1504(Mgr511Pose *p, u16 cameraBit)
{
    Hu3DCameraPosGet(cameraBit, &p->pos, &p->up, &p->target);
}

void fn_1_1544(Mgr511Pose *p)
{
    Mtx m;
    Quaternion q;
    Mgr511FloatBits px;
    Mgr511FloatBits py;
    Mgr511FloatBits pz;
    Mgr511FloatBits qx;
    Mgr511FloatBits qy;
    Mgr511FloatBits qz;
    Mgr511FloatBits qw;

    C_MTXLookAt(m, &p->pos, &p->up, &p->target);
    PSMTXInverse(m, m);
    px.f = p->pos.x;
    py.f = p->pos.y;
    pz.f = p->pos.z;
    C_QUATMtx(&q, m);
    qx.f = q.x;
    qy.f = q.y;
    qz.f = q.z;
    qw.f = q.w;
    printf("{{0x%08x,0x%08x,0x%08x},{0x%08x,0x%08x,0x%08x,0x%08x}},\n", px.u, py.u, pz.u, qx.u, qy.u, qz.u, qw.u);
}

f32 fn_1_15DC(s32 pad)
{
    return 0.017857144f * (f32)(s8)HuPadStkX[pad];
}

f32 fn_1_1630(s32 pad)
{
    return 0.017857144f * (f32)(s8)HuPadStkY[pad];
}

f32 fn_1_1684(s32 pad)
{
    return 0.016949153f * (f32)(s8)HuPadSubStkX[pad];
}

f32 fn_1_16D8(s32 pad)
{
    return 0.016949153f * (f32)(s8)HuPadSubStkY[pad];
}

void fn_1_172C(Mgr511Pose *cam, s32 pad)
{
    Mtx look;
    Mtx m1;
    Mtx m2;
    Mtx m3;
    Mtx inv;
    Vec x;
    Vec y;
    Vec z;
    Vec mv;
    Vec fw;
    f32 yaw;
    f32 pitch;
    f32 speed;
    f32 mag;
    u16 btn;

    btn = HuPadBtn[pad];
    if ((btn & 0x800) != 0) {
        cam->up.x = 0.0f;
        cam->up.y = 1.0f;
        cam->up.z = 0.0f;
    }
    yaw = -4.0f * (0.017857144f * (f32)(s8)HuPadStkX[pad]);
    pitch = -4.0f * (0.017857144f * (f32)(s8)HuPadStkY[pad]);
    C_MTXLookAt(look, &cam->pos, &cam->up, &cam->target);
    x.x = look[0][0];
    x.y = look[0][1];
    x.z = look[0][2];
    y.x = look[1][0];
    y.y = look[1][1];
    y.z = look[1][2];
    z.x = look[2][0];
    z.y = look[2][1];
    z.z = look[2][2];
    PSMTXRotAxisRad(m1, &x, 0.017453292f * pitch);
    PSMTXRotRad(m2, 'Y', 0.017453292f * yaw);
    PSMTXConcat(m1, m2, m3);
    PSMTXMultVecSR(m3, &z, &z);
    PSMTXMultVecSR(m3, &y, &cam->up);
    PSVECSubtract(&cam->pos, &z, &cam->target);
    if ((u8)((btn & 0x200) != 0) != 0) {
        speed = 70.0f;
    } else {
        speed = 20.0f;
    }
    C_MTXLookAt(inv, &cam->pos, &cam->up, &cam->target);
    PSMTXInverse(inv, inv);
    mv.x = (speed * (f32)(u8)((btn & 2) != 0)) - (speed * (f32)(u8)((btn & 1) != 0));
    mv.y = (speed * (f32)(u8)((btn & 8) != 0)) - (speed * (f32)(u8)((btn & 4) != 0));
    mv.x += speed * (0.016949153f * (f32)(s8)HuPadSubStkX[pad]);
    mv.z = 0.0f;
    PSMTXMultVecSR(inv, &mv, &mv);
    mv.y += speed * (0.016949153f * (f32)(s8)HuPadSubStkY[pad]);
    PSVECAdd(&mv, &cam->target, &cam->target);
    PSVECAdd(&mv, &cam->pos, &cam->pos);
    fw.x = 0.0f;
    fw.y = 0.0f;
    fw.z = (f32)((u8)((btn & 0x400) != 0) - (u8)((btn & 0x100) != 0));
    PSMTXMultVecSR(inv, &fw, &fw);
    fw.y = 0.0f;
    mag = PSVECMag(&fw);
    if (mag > 1e-10f) {
        PSVECScale(&fw, &fw, speed / mag);
        PSVECAdd(&fw, &cam->target, &cam->target);
        PSVECAdd(&fw, &cam->pos, &cam->pos);
    }
    if ((HuPadBtnDown[pad] & 0x200) != 0) {
        fn_1_1544(cam);
    }
}

/* Master object of a minigame (singleton, see lbl_1_bss_A0C). */
struct Mgr511Master {
    void *unk0;
    void **vtable;
    s32 player[4];
    s32 mes;
    s32 timer;
};

typedef void (*Mgr511MasterFunc)(Mgr511Master *);
typedef void *(*Mgr511NodeDtor)(Mgr511Node *, s32);
typedef u8 (*Mgr511NodeCmp)(Mgr511Node *, Mgr511Node *);

/* Singleton accessor shared by every function of the master. */
static inline Mgr511Master *MasterGet(void)
{
    if (lbl_1_bss_A0C != 0) {
    } else {
        fn_1_CF8("ms_pointer != 0", "singleton.h", 0x27);
    }
    return (Mgr511Master *)lbl_1_bss_A0C;
}

static inline Mgr511Master *MasterPtr(void)
{
    return (Mgr511Master *)lbl_1_bss_A0C;
}

#define MASTER_GET(m) ((m) = MasterGet())

void fn_1_2E08(OMOBJ *obj);
void fn_1_2E98(Mgr511Master *m);
void fn_1_2F04(void);
void fn_1_2F08(OMOBJ *obj);
void fn_1_36B0(void);
Mgr511Node *fn_1_1FC4(Mgr511NodeCmp cmp, Mgr511Node *n);
Mgr511Node *fn_1_266C(Mgr511Node *n);
Mgr511Node *fn_1_26C0(Mgr511NodeCmp cmp, Mgr511Node *a, Mgr511Node *b);
void *fn_1_2B64(Mgr511Master *m, s16 flag);
void fn_1_527C(void *p);

void fn_1_1D88(Mgr511Node *n)
{
    n->next = n;
    n->prev = n;
}

Mgr511Node *fn_1_1D94(Mgr511Node *n, s16 flag)
{
    if (n != 0) {
        n->next->prev = n->prev;
        n->prev->next = n->next;
        if (flag > 0 && n != 0) {
            HuMemDirectFree(n);
        }
    }
    return n;
}

void fn_1_1E00(Mgr511Node *n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;
    n->next = n;
    n->prev = n;
}

void fn_1_1E24(Mgr511Node *n, Mgr511Node *head)
{
    Mgr511Node *w;

    w = head;
    n->prev = w;
    n->next = w->next;
    w->next->prev = n;
    w->next = n;
}

void fn_1_1E54(Mgr511Node *n, Mgr511Node *head)
{
    Mgr511Node *w;

    w = head;
    n->next = w;
    n->prev = w->prev;
    w->prev->next = n;
    w->prev = n;
}

void fn_1_1E84(Mgr511Node *src, Mgr511Node *where)
{
    Mgr511Node *first;
    Mgr511Node *last;
    u8 empty;

    empty = 0;
    if (src->next == src && src->next == src->prev) {
        empty = 1;
    }
    if (empty == 0) {
        first = src->next;
        last = src->prev;
        src->next = src;
        src->prev = src;
        first->prev = where->prev;
        last->next = where;
        where->prev->next = first;
        where->prev = last;
    }
}

void fn_1_1F0C(Mgr511Node *head, Mgr511NodeCmp cmp)
{
    Mgr511NodeCmp c;
    Mgr511Node *n;
    Mgr511Node *prev;
    u8 empty;

    c = cmp;
    empty = 0;
    if (head->next == head && head->next == head->prev) {
        empty = 1;
    }
    if (empty == 0) {
        head->prev->next = 0;
        n = fn_1_1FC4(c, head->next);
        head->next = n;
        prev = head;
        while (n != 0) {
            n->prev = prev;
            prev = n;
            n = n->next;
        }
        head->prev = prev;
        prev->next = head;
    }
}

Mgr511Node *fn_1_1FC4(Mgr511NodeCmp cmp, Mgr511Node *n)
{
    Mgr511Node *second;

    if (n == 0) {
        return 0;
    }
    if (n->next == 0) {
        return n;
    }
    second = fn_1_266C(n);
    n = fn_1_1FC4(cmp, n);
    second = fn_1_1FC4(cmp, second);
    return fn_1_26C0(cmp, n, second);
}

Mgr511Node *fn_1_266C(Mgr511Node *n)
{
    Mgr511Node *slow;
    Mgr511Node *fast;
    Mgr511Node *second;

    slow = n;
    fast = slow->next;
    if (fast != 0) {
        fast = fast->next;
    }
    while (fast != 0) {
        slow = slow->next;
        fast = fast->next;
        if (fast != 0) {
            fast = fast->next;
        }
    }
    second = slow->next;
    slow->next = 0;
    return second;
}

Mgr511Node *fn_1_26C0(Mgr511NodeCmp cmp, Mgr511Node *a, Mgr511Node *b)
{
    Mgr511NodeCmp c;
    Mgr511Node head;
    Mgr511Node *tail;

    c = cmp;
    tail = &head;
    while (a != 0 && b != 0) {
        if (c(a, b) != 0) {
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

void fn_1_276C(s32 mode)
{
    Mgr511Node *n;

    n = lbl_1_bss_2C.next;
    lbl_1_bss_34 = mode;
    while (n != LIST_END) {
        n->flags = n->flags | (1 << mode);
        ((void (*)(Mgr511Node *, s32))n->vtable[3])(n, mode);
        n = n->next;
    }
}

s32 fn_1_2804(void)
{
    Mgr511Node *first;
    Mgr511Node *it;
    Mgr511Node *p;
    Mgr511Node *end;
    Mgr511Node *nx;
    s32 bit;

    first = lbl_1_bss_2C.next;
    it = first;
    p = it;
    end = LIST_END;
    while (p != end) {
        bit = lbl_1_bss_34;
        if ((p->flags & (1 << bit)) != 0) {
            return 0;
        }
        nx = p->next;
        p = nx;
    }
    return 1;
}

void fn_1_2890(void)
{
    Mgr511Node *first;
    Mgr511Node *it;
    Mgr511Node *p;
    Mgr511Node *end;
    Mgr511Node *nx;

    first = lbl_1_bss_2C.next;
    it = first;
    p = it;
    end = LIST_END;
    while (p != end) {
        ((void (*)(Mgr511Node *))p->vtable[4])(p);
        nx = p->next;
        p = nx;
    }
}

void fn_1_2900(void)
{
    u8 empty;

    do {
        empty = 0;
        if (lbl_1_bss_2C.next == LIST_END && lbl_1_bss_2C.next == lbl_1_bss_2C.prev) {
            empty = 1;
        }
        if (empty == 0) {
            if (lbl_1_bss_2C.prev != 0) {
                ((void (*)(Mgr511Node *, s32))lbl_1_bss_2C.prev->vtable[2])(lbl_1_bss_2C.prev, 1);
            }
        }
    } while (empty == 0);
}

s32 fn_1_29C8(void)
{
    return lbl_1_bss_34;
}

Mgr511Master *fn_1_29D8(Mgr511Master *m)
{
    s32 i;

    if (lbl_1_bss_A0C == 0) {
    } else {
        fn_1_CF8("ms_pointer == 0", "singleton.h", 0x35);
    }
    lbl_1_bss_A0C = m;
    m->vtable = lbl_1_data_23C;
    m->timer = 0;
    for (i = 0; i < 4; i++) {
        m->player[i] = -1;
    }
    return m;
}

Mgr511Master *fn_1_2AA8(Mgr511Master *m, s16 flag)
{
    if (m != 0) {
        if (lbl_1_bss_A0C != 0) {
        } else {
            fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A0C = 0;
        if (flag > 0 && m != 0) {
            HuMemDirectFree(m);
        }
    }
    return m;
}

void *fn_1_2B64(Mgr511Master *m, s16 flag)
{
    Mgr511Master *master;
    s32 i;
    s32 p;

    if (m != 0) {
        m->vtable = lbl_1_data_23C;
        if (mgDataTbl[GwSystem.mgNo].type != 3) {
            MASTER_GET(master);
            for (i = 0; i < 4; i++) {
                p = master->player[i];
                if (p >= 0 && _CheckFlag(0x1000F) == 0) {
                    GwPlayer[p].mgCoinBonus = 0xA;
                }
            }
        }
        if (m != 0) {
            if (lbl_1_bss_A0C != 0) {
            } else {
                fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
            }
            lbl_1_bss_A0C = 0;
        }
        if (flag > 0) {
            fn_1_527C(m);
        }
    }
    return m;
}

void fn_1_2D1C(Mgr511Master *m, s16 objMax, s32 prio)
{
    OMOBJMAN *man;

    man = omInitObjMan(objMax, prio);
    omGameSysInit(man);
    ((void (*)(Mgr511Master *, OMOBJMAN *))m->vtable[3])(m, man);
    omAddObjEx(man, 0, 0, 0, -1, fn_1_2E08);
}

void fn_1_2DA0(Mgr511Master *m)
{
    OMOBJMAN *man;

    man = MgActorObjectSetup();
    ((void (*)(Mgr511Master *, OMOBJMAN *))m->vtable[3])(m, man);
    omAddObjEx(man, 0, 0, 0, -1, fn_1_2E08);
}

void fn_1_2E08(OMOBJ *obj)
{
    if (omSysExitReq != 0 && WipeCheckEnd() == 0) {
        fn_1_36B0();
        HuAudFadeOut(0x3E8);
        obj->objFunc = fn_1_2F08;
        return;
    }
    {
        Mgr511Master *m = MasterPtr();
        Mgr511Master *m2 = m;

        fn_1_2E98(m2);
    }
}

void fn_1_2E98(Mgr511Master *m)
{
    s32 t;

    ((void (*)(Mgr511Master *))m->vtable[4])(m);
    if (m->timer > 0) {
        t = m->timer - 1;
        m->timer = t;
        if (t == 0) {
        }
    }
}

void fn_1_2F04(void)
{
}

void fn_1_2F08(OMOBJ *obj)
{
    Mgr511Master *m;
    s32 t;

    m = MasterPtr();
    if (WipeCheckEnd() != 0) {
        ((void (*)(Mgr511Master *))m->vtable[4])(m);
        if (m->timer > 0) {
            t = m->timer - 1;
            m->timer = t;
            if (t == 0) {
            }
        }
        return;
    }
    ((void (*)(Mgr511Master *))m->vtable[5])(m);
    if (m != 0) {
        ((void (*)(Mgr511Master *, s32))m->vtable[2])(m, 1);
    }
    CharModelKill(-1);
    omOvlReturnEx(1, 1);
}

void fn_1_2FF4(s32 idx)
{
    Mgr511Master *m;
    s32 *pl;
    s32 i;

    MASTER_GET(m);
    pl = &m->player[0];
    i = 0;
    while (1) {
        if (i >= 4) {
            return;
        }
        if (pl[i] < 0) {
            pl[i] = idx;
            return;
        }
        i++;
    }
}

s32 fn_1_30C8(s32 idx)
{
    Mgr511Master *m;
    s32 i;

    MASTER_GET(m);
    i = 0;
    while (1) {
        if (i >= 4) {
            return 0;
        }
        if (idx == m->player[i]) {
            return 1;
        }
        i++;
    }
}

void fn_1_319C(void)
{
    Mgr511Master *m;

    MASTER_GET(m);
    m->mes = GameMesCreate(2, 0);
}

void fn_1_3254(void)
{
    Mgr511Master *m;

    MASTER_GET(m);
    m->mes = GameMesCreate(2, 1);
}

s32 fn_1_330C(void)
{
    Mgr511Master *m;
    s32 r;

    MASTER_GET(m);
    r = 0;
    if (GameMesStatGet(-1) == 0 && (u8)(m->timer <= 0) != 0) {
        r = 1;
    }
    return r;
}

void fn_1_33E0(void)
{
    Mgr511Master *m;
    s32 *pl;
    s32 chars[4];
    s32 n;
    s32 i;
    s32 j;
    s32 k;
    s32 t;
    s16 c;

    MASTER_GET(m);
    m->timer = lbl_1_bss_3C;
    pl = &m->player[0];
    n = 0;
    while (n < 4 && pl[n] >= 0) {
        n++;
    }
    if (n == 0) {
        GameMesCreate(2, 2);
        return;
    }
    for (i = 1; i < n; i++) {
        j = i;
        while (j >= 1 && pl[j - 1] > pl[j]) {
            t = pl[j - 1];
            pl[j - 1] = pl[j];
            pl[j] = t;
            j--;
        }
    }
    for (k = 0; k < 4; k++) {
        if (pl[k] < 0) {
            c = -1;
        } else {
            c = GwPlayerConf[pl[k]].charNo;
        }
        chars[k] = c;
    }
    GameMesCreate(4, 3, chars[0], chars[1], chars[2], chars[3]);
}

void fn_1_35AC(s32 arg)
{
    Mgr511Master *m;

    if (_CheckFlag(0x1000F) == 0) {
        MASTER_GET(m);
        m->mes = GameMesCreate(0xE, arg);
        HuAudJinglePlay(0x86);
    }
}

void fn_1_3684(void)
{
    WipeCreate(1, 5, 0x3C);
}

void fn_1_36B0(void)
{
    WipeCreate(2, 6, 0x3C);
}

void fn_1_36DC(void)
{
    omSysExitReq = 1;
}

/* Layout of a placed object as seen by the model wrappers. */
struct Mgr511ModelXf {
    u8 pad00[0x18];
    Vec pos;
    Vec rot;
    Vec scale;
};

s16 *fn_1_36F0(s16 *r, void *data)
{
    void *d;

    d = data;
    *r = Hu3DModelCreate(d);
    return r;
}

s16 *fn_1_372C(s16 *r, s32 dataNum)
{
    *r = Hu3DModelCreate(HuDataSelHeapReadNum(dataNum, HU_MEMNUM_OVL, HEAP_MODEL));
    return r;
}

s16 *fn_1_3774(s16 *r, HU3DMODELHOOK hook)
{
    HU3DMODELHOOK h;

    h = hook;
    *r = Hu3DHookFuncCreate(h);
    return r;
}

s16 *fn_1_37B0(s16 *r, s32 charNo, s32 kind)
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
    *r = CharModelCreate((s16)charNo, model);
    return r;
}

s16 *fn_1_384C(s16 *r, s16 *src)
{
    s16 id;

    id = *src;
    *r = Hu3DModelLink(id);
    return r;
}

void fn_1_38A0(s16 *r, u32 attr)
{
    Hu3DModelAttrSet(*r, attr);
}

void fn_1_38D8(s16 *r, u32 attr)
{
    Hu3DModelAttrReset(*r, attr);
}

void fn_1_3910(s16 *r, Mgr511ModelXf *xf)
{
    s16 id;

    id = *r;
    Hu3DModelPosSet(id, xf->pos.x, xf->pos.y, xf->pos.z);
    Hu3DModelRotSet(id, xf->rot.x, xf->rot.y, xf->rot.z);
    Hu3DModelScaleSet(id, xf->scale.x, xf->scale.y, xf->scale.z);
}

void fn_1_398C(s16 *r, Vec *v)
{
    Hu3DModelPosSet(*r, v->x, v->y, v->z);
}

void fn_1_39D4(s16 *r, Vec *v)
{
    Hu3DModelRotSet(*r, v->x, v->y, v->z);
}

void fn_1_3A1C(s16 *r, Vec *v)
{
    Hu3DModelScaleSet(*r, v->x, v->y, v->z);
}

void fn_1_3A64(s16 *r, f32 s)
{
    Hu3DModelScaleSet(*r, s, s, s);
}

void fn_1_3AAC(s16 *r, f32 lvl)
{
    f32 l;

    l = lvl;
    Hu3DModelTPLvlSet(*r, lvl);
}

void fn_1_3AE4(s16 *r, s16 layer)
{
    Hu3DModelLayerSet(*r, layer);
}

void fn_1_3B1C(s16 *r, u16 bit)
{
    Hu3DModelCameraSet(*r, bit);
}

void fn_1_3B54(s16 *r, void *data)
{
    Hu3DData[*r].hookData = data;
}

f32 *fn_1_3B70(s16 *r)
{
    return (f32 *)Hu3DData[*r].mtx;
}

s16 *fn_1_3B8C(s16 *r, s16 flag)
{
    if (r != 0) {
        Hu3DModelKill(*r);
        if (flag > 0 && r != 0) {
            HuMemDirectFree(r);
        }
    }
    return r;
}

HSFOBJECT *fn_1_3BE8(s16 *r, char *name)
{
    HSFOBJECT *o;

    o = Hu3DModelObjPtrGet(*r, name);
    if (o != 0) {
    } else {
        fn_1_CF8("p != NULL", "model.cpp", 0x8A);
    }
    return o;
}

void fn_1_3C7C(s16 *r)
{
    Hu3DModelShadowSet(*r);
}

void fn_1_3CAC(s16 *r)
{
    Hu3DModelShadowMapSet(*r);
}

void fn_1_3CDC(s16 *r, char *name)
{
    Hu3DModelShadowMapObjSet(*r, name);
}

void fn_1_3D14(s16 *r)
{
    Hu3DModelShadowMapObjReset(*r);
}

void fn_1_3D44(s16 *r, char *name)
{
    HSFOBJECT *o;
    u32 *flags;

    o = Hu3DModelObjPtrGet(*r, name);
    if (o != 0) {
    } else {
        fn_1_CF8("p != NULL", "model.cpp", 0x8A);
    }
    flags = (u32 *)o->constData;
    *flags = *flags & 0xFFFFFFF7;
}

void fn_1_3DF0(s16 *r, char *name)
{
    HSFOBJECT *o;

    o = Hu3DModelObjPtrGet(*r, name);
    if (o != 0) {
    } else {
        fn_1_CF8("p != NULL", "model.cpp", 0x8A);
    }
    o->flags |= 0x400;
}

void fn_1_3E98(s16 *r, char *name)
{
    HSFOBJECT *o;

    o = Hu3DModelObjPtrGet(*r, name);
    if (o != 0) {
    } else {
        fn_1_CF8("p != NULL", "model.cpp", 0x8A);
    }
    o->flags &= 0xFFFFFBFF;
}

void fn_1_3F40(s16 *r, char *name)
{
    HSFOBJECT *o;

    o = Hu3DModelObjPtrGet(*r, name);
    if (o != 0) {
    } else {
        fn_1_CF8("p != NULL", "model.cpp", 0x8A);
    }
    o->flags |= 0x400;
    o->mesh.base.scale.x = 0.0f;
    o->mesh.base.scale.y = 0.0f;
    o->mesh.base.scale.z = 0.0f;
}

void fn_1_401C(s16 *r, char *name)
{
    HSFOBJECT *o;

    o = Hu3DModelObjPtrGet(*r, name);
    if (o != 0) {
    } else {
        fn_1_CF8("p != NULL", "model.cpp", 0x8A);
    }
    o->flags &= 0xFFFFFBFF;
    o->mesh.base.scale.x = 1.0f;
    o->mesh.base.scale.y = 1.0f;
    o->mesh.base.scale.z = 1.0f;
}

void fn_1_40F8(s16 *r, s32 t)
{
    Hu3DMotionTimeSet((s16)*r, (f32)t);
}

/* Sphere used by the collision helpers. */
struct Mgr511Sphere {
    Vec pos;
    f32 radius;
};

/* OSThread plus the heap-allocated stack it runs on. */
struct Mgr511CThread {
    OSThread thread;
    u32 *stack;
};

/* Sub-thread wrapper: runs virtual method 3 on its own stack. */
struct Mgr511SubThread {
    void **vtable;
    Mgr511CThread cthread;
    OSSemaphore sem;
    s32 result;
};

/* Timer wrapper. */
struct Mgr511Timer {
    MGTIMER *timer;
};

const u32 lbl_1_rodata_A0[2] = { 0, 0x9908B0DF };

void fn_1_5CDC(void);
void fn_1_6180(Mgr511CThread *t, void *(*entry)(void *), void *arg, u32 *stk, u32 words, s32 prio, u16 attr);
Mgr511CThread *fn_1_6020(Mgr511CThread *t, void *(*entry)(void *), void *arg, u32 stackSize, s32 prio, u16 attr);
Mgr511CThread *fn_1_62A4(Mgr511CThread *t, s16 flag);
u32 fn_1_63D8(Mgr511CThread *t);
s32 fn_1_6478(u32 *p, s32 mode);
void fn_1_64EC(u32 *p, u32 words);
void *fn_1_5FC4(void *arg);
void *fn_1_5D7C(Mgr511SubThread *t, s16 flag);
f64 fn_1_4C1C(Mgr511Mt *m);
void *fn_1_53CC(void **self, s16 flag);
void fn_1_5358(void);
void fn_1_530C(OMOBJ *obj);
void *fn_1_6A98(Mgr511Node *n, s16 flag);

/* genrand_int32 of the reference implementation. */
static inline u32 MtNext(Mgr511Mt *m)
{
    u32 kk;
    u32 y;

    if (m->index >= 0x270) {
        kk = 0;
        while (kk < 0xE3) {
            y = (m->mt[kk] & 0x80000000) | (m->mt[kk + 1] & 0x7FFFFFFF);
            m->mt[kk] = lbl_1_rodata_A0[y & 1] ^ ((y >> 1) ^ m->mt[kk + 0x18D]);
            kk++;
        }
        while (kk < 0x26F) {
            y = (m->mt[kk] & 0x80000000) | (m->mt[kk + 1] & 0x7FFFFFFF);
            m->mt[kk] = lbl_1_rodata_A0[y & 1] ^ ((y >> 1) ^ m->mt[kk - 0xE3]);
            kk++;
        }
        y = (m->mt[0x26F] & 0x80000000) | (m->mt[0] & 0x7FFFFFFF);
        m->mt[0x26F] = (y >> 1) ^ m->mt[0x18C] ^ lbl_1_rodata_A0[y & 1];
        m->index = 0;
    }
    y = m->mt[m->index];
    m->index = m->index + 1;
    y = y ^ (y >> 0xB);
    y = y ^ ((y << 7) & 0x9D2C5680);
    y = y ^ ((y << 0xF) & 0xEFC60000);
    return y ^ (y >> 0x12);
}

void fn_1_4164(Mgr511Mt *m, s32 seed)
{
    m->mt[0] = seed;
    m->index = 1;
    while (m->index < 0x270) {
        m->mt[m->index] = m->index + ((m->mt[m->index - 1] ^ (m->mt[m->index - 1] >> 0x1E)) * 0x6C078965);
        m->index = m->index + 1;
    }
}

void fn_1_41D8(Mgr511Mt *m, u32 *key, u32 len)
{
    u32 i;
    u32 j;
    u32 k;

    m->mt[0] = 0x012BD6AA;
    m->index = 1;
    while (m->index < 0x270) {
        m->mt[m->index] = m->index + ((m->mt[m->index - 1] ^ (m->mt[m->index - 1] >> 0x1E)) * 0x6C078965);
        m->index = m->index + 1;
    }
    i = 1;
    j = 0;
    if (len < 0x270) {
        k = 0x270;
    } else {
        k = len;
    }
    for (; k != 0; k--) {
        m->mt[i] = key[j] + ((m->mt[i] ^ ((m->mt[i - 1] ^ (m->mt[i - 1] >> 0x1E)) * 0x19660D)) + j);
        i++;
        j++;
        if (i >= 0x270) {
            m->mt[0] = m->mt[0x26F];
            i = 1;
        }
        if (j >= len) {
            j = 0;
        }
    }
    for (k = 0x26F; k != 0; k--) {
        m->mt[i] = (m->mt[i] ^ ((m->mt[i - 1] ^ (m->mt[i - 1] >> 0x1E)) * 0x5D588B65)) - i;
        i++;
        if (i >= 0x270) {
            m->mt[0] = m->mt[0x26F];
            i = 1;
        }
    }
    m->mt[0] = 0x80000000;
}

s32 fn_1_439C(Mgr511Mt *m)
{
    return MtNext(m);
}

u32 fn_1_4528(Mgr511Mt *m)
{
    return MtNext(m) >> 1;
}

f32 fn_1_46C0(Mgr511Mt *m)
{
    return 2.3283064e-10f * (f32)MtNext(m);
}

f32 fn_1_4884(Mgr511Mt *m)
{
    return 2.3283064e-10f * (f32)MtNext(m);
}

f32 fn_1_4A48(Mgr511Mt *m)
{
    return 2.3283064e-10f * (0.5f + (f32)MtNext(m));
}

f64 fn_1_4C1C(Mgr511Mt *m)
{
    u32 a;
    u32 b;

    a = MtNext(m);
    b = MtNext(m);
    return 1.1102230246251565e-16 * ((67108864.0 * (f64)(a >> 5)) + (f64)(b >> 6));
}

f32 fn_1_4F94(Mgr511Mt *m)
{
    return 1.1920929e-7f * (f32)(MtNext(m) >> 9);
}

void *fn_1_5164(s32 size)
{
    void *p;
    s32 s;

    s = size;
    p = HuMemDirectMallocNum(HEAP_HEAP, s, 0x10000000);
    if (p != 0) {
    } else {
        fn_1_CF8("ptr != NULL", "om.cpp", 0x18);
    }
    return p;
}

void *fn_1_51F0(s32 size)
{
    void *p;
    s32 s;

    s = size;
    p = HuMemDirectMallocNum(HEAP_HEAP, s, 0x10000000);
    if (p != 0) {
    } else {
        fn_1_CF8("ptr != NULL", "om.cpp", 0x1F);
    }
    return p;
}

void fn_1_527C(void *p)
{
    if (p != 0) {
        HuMemDirectFree(p);
    }
}

void fn_1_52B4(void *p)
{
    if (p != 0) {
        HuMemDirectFree(p);
    }
}

void fn_1_52EC(void **self, OMOBJ *obj)
{
    *self = lbl_1_data_22C;
    obj->data = self;
    obj->objFunc = fn_1_530C;
}

void fn_1_530C(OMOBJ *obj)
{
    void **o;

    o = (void **)obj->data;
    ((void (*)(void **, OMOBJ *))((void **)*o)[3])(o, obj);
}

void fn_1_5358(void)
{
}

void fn_1_535C(OMOBJ *obj)
{
    void **o;

    o = (void **)obj->data;
    if (o != 0) {
        ((void (*)(void **, s32))((void **)*o)[2])(o, 1);
    }
    omDelObjEx(HuPrcCurrentGet(), obj);
}

void *fn_1_53CC(void **self, s16 flag)
{
    if (self != 0) {
        *self = lbl_1_data_22C;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_542C(HUPROCESS *proc, OMOBJ *obj)
{
    HUPROCESS *p;
    void **o;

    p = proc;
    o = (void **)obj->data;
    if (o != 0) {
        ((void (*)(void **, s32))((void **)*o)[2])(o, 1);
    }
    omDelObjEx((OMOBJMAN *)p, obj);
}

void fn_1_54A0(u32 seed)
{
    lbl_1_bss_44.mt[0] = seed;
    lbl_1_bss_44.index = 1;
    while (lbl_1_bss_44.index < 0x270) {
        lbl_1_bss_44.mt[lbl_1_bss_44.index] = lbl_1_bss_44.index + ((lbl_1_bss_44.mt[lbl_1_bss_44.index - 1] ^ (lbl_1_bss_44.mt[lbl_1_bss_44.index - 1] >> 0x1E)) * 0x6C078965);
        lbl_1_bss_44.index = lbl_1_bss_44.index + 1;
    }
}

u32 fn_1_5578(void)
{
    return MtNext(&lbl_1_bss_44) >> 1;
}

f32 fn_1_57B4(void)
{
    return 1.1920929e-7f * (f32)(MtNext(&lbl_1_bss_44) >> 9);
}

f32 fn_1_5A28(f32 scale)
{
    f32 s;

    s = scale;
    return (f32)((f64)s * fn_1_4C1C(&lbl_1_bss_44));
}

f32 fn_1_5A60(f32 a, f32 b)
{
    f32 hi;

    hi = b;
    return (f32)((f64)a + ((f64)(hi - a) * fn_1_4C1C(&lbl_1_bss_44)));
}

s32 fn_1_5AB4(Mgr511Sphere *a, Mgr511Sphere *b)
{
    Vec d;
    f32 rb;
    f32 ra;
    f32 sum;
    f32 dist;

    PSVECSubtract(&a->pos, &b->pos, &d);
    rb = b->radius;
    ra = a->radius;
    sum = ra + rb;
    dist = PSVECMag(&d);
    return dist < sum;
}

s32 fn_1_5B5C(Mgr511Sphere *a, Mgr511Sphere *b, f32 w, f32 k)
{
    Vec d;
    Vec t;
    f32 kk;
    f32 rb;
    f32 ra;
    f32 sum;
    f32 dist;
    f32 scale;
    f32 negW;
    f32 rest;

    kk = k;
    rb = b->radius;
    ra = a->radius;
    sum = ra + rb;
    PSVECSubtract(&b->pos, &a->pos, &d);
    dist = PSVECMag(&d);
    if (dist >= sum) {
        return 0;
    }
    scale = (kk * (sum - dist)) / dist;
    PSVECScale(&d, &d, scale);
    negW = -w;
    PSVECScale(&d, &t, negW);
    PSVECAdd(&t, &a->pos, &a->pos);
    rest = 1.0f - w;
    PSVECScale(&d, &t, rest);
    PSVECAdd(&t, &b->pos, &b->pos);
    return 1;
}

void fn_1_5CDC(void)
{
    OSGetThreadPriority(OSGetCurrentThread());
}

Mgr511SubThread *fn_1_5D00(Mgr511SubThread *t, u32 stackSize)
{
    u32 s;

    s = stackSize;
    t->vtable = lbl_1_data_21C;
    fn_1_6020(&t->cthread, fn_1_5FC4, t, s, OSGetThreadPriority(OSGetCurrentThread()), 1);
    OSInitSemaphore(&t->sem, 0);
    return t;
}

void *fn_1_5D7C(Mgr511SubThread *t, s16 flag)
{
    if (t != 0) {
        t->vtable = lbl_1_data_21C;
        fn_1_62A4(&t->cthread, -1);
        if (flag > 0 && t != 0) {
            HuMemDirectFree(t);
        }
    }
    return t;
}

u8 fn_1_5DE8(Mgr511SubThread *t)
{
    OSThread *th;

    th = &t->cthread.thread;
    return (u8)(OSIsThreadTerminated(th) != 0);
}

static inline OSThread *SubThreadOf(Mgr511SubThread *t)
{
    return &t->cthread.thread;
}

static inline OSSemaphore *SubSemOf(Mgr511SubThread *t)
{
    return &t->sem;
}

s32 fn_1_5E30(Mgr511SubThread *t, s32 arg)
{
    s32 a;

    a = arg;
    if (OSIsThreadTerminated(SubThreadOf(t)) != 0) {
        return t->result;
    }
    t->result = a;
    OSResumeThread(SubThreadOf(t));
    OSWaitSemaphore(SubSemOf(t));
    fn_1_63D8(&t->cthread);
    OSCheckActiveThreads();
    return t->result;
}

s32 fn_1_5EC0(Mgr511SubThread *t, s32 arg)
{
    s32 a;

    a = arg;
    if (OSGetCurrentThread() == &t->cthread.thread) {
    } else {
        fn_1_CF8("::OSGetCurrentThread() == m_th", "subthread.cpp", 0x47);
    }
    if (OSIsThreadTerminated(&t->cthread.thread) == 0) {
    } else {
        fn_1_CF8("!::OSIsThreadTerminated(m_th)", "subthread.cpp", 0x4A);
    }
    t->result = a;
    OSSignalSemaphore(&t->sem);
    OSSuspendThread(&t->cthread.thread);
    return t->result;
}

void *fn_1_5FC4(void *arg)
{
    Mgr511SubThread *t;

    t = (Mgr511SubThread *)arg;
    ((void (*)(Mgr511SubThread *, s32))t->vtable[3])(t, t->result);
    OSSignalSemaphore(&t->sem);
    return 0;
}

Mgr511CThread *fn_1_6020(Mgr511CThread *t, void *(*entry)(void *), void *arg, u32 stackSize, s32 prio, u16 attr)
{
    void *(*e)(void *);
    void *a;
    u32 words;
    u32 *stk;

    e = entry;
    a = arg;
    words = stackSize >> 2;
    stk = (u32 *)__nw__FUl(words * 4);
    t->stack = stk;
    fn_1_6180(t, e, a, stk, words, prio, attr);
    return t;
}

Mgr511CThread *fn_1_6104(Mgr511CThread *t, void *(*entry)(void *), void *arg, u32 *stk, u32 size, s32 prio, u16 attr)
{
    t->stack = 0;
    fn_1_6180(t, entry, arg, stk, size >> 2, prio, attr);
    return t;
}

void fn_1_6180(Mgr511CThread *t, void *(*entry)(void *), void *arg, u32 *stk, u32 words, s32 prio, u16 attr)
{
    u8 ok;
    s32 r;

    ok = 0;
    if (prio >= 0 && prio <= 0x1F) {
        ok = 1;
    }
    if (ok != 0) {
    } else {
        fn_1_CF8("priority >= OS_PRIORITY_MIN && priority <= OS_PRIORITY_MAX", "thread.cpp", 0x52);
    }
    fn_1_64EC(stk, words);
    fn_1_6478(stk, 0);
    r = OSCreateThread(&t->thread, entry, arg, &stk[words], words * 4, prio, attr);
    if ((u8)(r != 0) == 0) {
        OSPanic("thread.cpp", 0x60, "OSCreateThread() failed.");
    }
}

Mgr511CThread *fn_1_62A4(Mgr511CThread *t, s16 flag)
{
    u32 total;
    u32 remain;
    u32 *stk;

    if (t != 0) {
        if (OSIsThreadTerminated(&t->thread) == 0) {
            OSCancelThread(&t->thread);
        }
        if (lbl_1_bss_A08 != 0) {
            total = (u32)t->thread.stackBase - (u32)t->thread.stackEnd;
            remain = fn_1_63D8(t);
            printf("delete CThread@%p\nstack remain/total = 0x%08x/0x%08x (%f%%)\n", t, remain, total, (100.0 * (f64)remain) / (f64)total);
        }
        stk = t->stack;
        if (stk != 0) {
            HuMemDirectFree(stk);
        }
        if (flag > 0 && t != 0) {
            HuMemDirectFree(t);
        }
    }
    return t;
}

u32 fn_1_63D8(Mgr511CThread *t)
{
    u32 *base;
    u32 *p;
    s32 r;

    base = t->thread.stackEnd;
    r = fn_1_6478(base, 1);
    if (r != 0) {
        OSPanic("thread.cpp", 0x7F, "stack overflow\n");
    }
    p = base;
    while (p < (u32 *)t->thread.stackBase) {
        if (*p == 0xDEADBABE) {
            p++;
        } else {
            break;
        }
    }
    return (u32)p - (u32)base;
}

s32 fn_1_6478(u32 *p, s32 mode)
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
        while (1) {
            if (p == end) {
                return 0;
            }
            v = *p;
            p++;
            if (v != 0xDEADBABE) {
                return 1;
            }
        }
    default:
        return 0;
    }
}

void fn_1_64EC(u32 *p, u32 words)
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

Mgr511Timer *fn_1_6538(Mgr511Timer *t, s32 type)
{
    s32 ty;

    ty = type;
    switch (ty) {
    case 0:
        t->timer = MgTimerCreate(0);
        break;
    case 1:
        t->timer = MgTimerCreate(1);
        break;
    case 2:
        t->timer = MgTimerCreate(2);
        break;
    default:
        fn_1_CF8("false", "timer.cpp", 0x1E);
        break;
    }
    return t;
}

Mgr511Timer *fn_1_6600(Mgr511Timer *t, s16 flag)
{
    if (t != 0) {
        MgTimerKill(t->timer);
        if (flag > 0 && t != 0) {
            HuMemDirectFree(t);
        }
    }
    return t;
}

void fn_1_665C(Mgr511Timer *t, s32 a, s32 b, s32 c)
{
    MgTimerParamSet(t->timer, a, b, c);
}

void fn_1_66A4(Mgr511Timer *t)
{
    MgTimerValueGet(t->timer);
}

void fn_1_66D4(Mgr511Timer *t, f32 x, f32 y)
{
    MgTimerPosSet(t->timer, x, y);
}

void fn_1_6714(Mgr511Timer *t, f32 *x, f32 *y)
{
    MgTimerPosGet(t->timer, x, y);
}

void fn_1_6754(Mgr511Timer *t, s32 mode)
{
    s32 m;

    m = mode;
    switch (m) {
    case 0:
        MgTimerModeOnSet(t->timer, 0);
        return;
    case 1:
        MgTimerModeOnSet(t->timer, 1);
        return;
    case 2:
        MgTimerModeOnSet(t->timer, 2);
        return;
    default:
        fn_1_CF8("false", "timer.cpp", 0x48);
        return;
    }
}

void fn_1_6818(Mgr511Timer *t)
{
    MgTimerModeOffSet(t->timer);
}

void fn_1_6848(Mgr511Timer *t)
{
    if (t->timer->stopF == 0) {
        t->timer->stopF = 1;
        t->timer->mode = 1;
    }
}

void fn_1_6874(Mgr511Timer *t)
{
    MgTimerKill(t->timer);
    t->timer = MgTimerCreate(0);
}

u8 fn_1_68B0(Mgr511Timer *t)
{
    return (u8)(MgTimerDoneCheck(t->timer) != 0);
}

void fn_1_68F0(Mgr511Timer *t, u8 r, u8 g, u8 b)
{
    MgTimerWinColorSet(t->timer, r, g, b);
}

void fn_1_6938(void)
{
    lbl_1_bss_2C.next = LIST_END;
    lbl_1_bss_2C.prev = LIST_END;
    __register_global_object(&lbl_1_bss_2C, (void *(*)(void *, s16))fn_1_6A98, &lbl_1_bss_20);
    lbl_1_bss_38 = 0x78;
    lbl_1_bss_3C = 0xD2;
    lbl_1_bss_40 = 0xD2;
    fn_1_4164(&lbl_1_bss_44, 0x1571);
}

void *fn_1_6A98(Mgr511Node *n, s16 flag)
{
    if (n != 0) {
        if (n != 0) {
            n->next->prev = n->prev;
            n->prev->next = n->next;
        }
        if (flag > 0 && n != 0) {
            HuMemDirectFree(n);
        }
    }
    return n;
}

void *lbl_1_data_21C[4] = { 0, 0, (void *)fn_1_5D7C, 0 };
void *lbl_1_data_22C[4] = { 0, 0, (void *)fn_1_53CC, (void *)fn_1_5358 };
void *lbl_1_data_23C[7] = { 0, 0, (void *)fn_1_2B64, 0, (void *)fn_1_2F04, 0, 0 };

}
