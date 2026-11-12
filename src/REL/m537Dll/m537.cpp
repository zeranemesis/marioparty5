#include "REL/m537Dll.h"
#include <stdarg.h>
#include <stdio.h>
extern "C" {
#include "game/wipe.h"
#include "game/gamemes.h"
#include "game/mg/timer.h"
}

// m537 top-level unit: runtime support (allocator, assert, list helpers, a look-at
// camera helper), the module main object and the Hu3D wrapper functions.
extern "C" {

void OSPanic(const char *file, int line, const char *msg, ...);
void ObjectSetup(void);
OMOBJMAN *MgActorObjectSetup(void);

extern void (*_ctors[])(void);
extern void (*_dtors[])(void);

// Raised assertion text shared by every inlined singleton check.
#define M537_ASSERT_FAIL(cond, file, line)                                           \
    do {                                                                             \
        char buf[1024];                                                              \
        sprintf(buf, "\nASSERTION FAILED at %s:%d\n%s", file, line, cond);           \
        OSPanic("assert.cpp", 0x12, buf);                                            \
    } while (0)

// A work object as seen by the list at lbl_1_bss_C: node, vtable, flag word.
struct M537Base {
    M537_NODE m_node;   /* 0x00 */
    void **m_vptr;      /* 0x08 */
    s32 m_flag;         /* 0x0C */
};

// Look-at camera: position, up vector and target.
struct M537CamLook {
    Vec m_pos;      /* 0x00 */
    Vec m_up;       /* 0x0C */
    Vec m_target;   /* 0x18 */
};

// Camera as position plus orientation quaternion.
struct M537CamQuat {
    Vec m_pos;          /* 0x00 */
    Quaternion m_rot;   /* 0x0C */
};

// The module main object (singleton at lbl_1_bss_9EC).
struct M537Main {
    s32 m_unk0;         /* 0x00 */
    void **m_vptr;      /* 0x04 */
    s32 m_player[4];    /* 0x08 */
    s32 m_msg;          /* 0x18 */
    s32 m_timer;        /* 0x1C */
};

// Polymorphic work object owned by an om object.
struct M537VObj {
    void **m_vptr;
};

// Intrusive singly linked list node used by the merge sort.
struct M537Link {
    M537Link *m_next;
};

typedef u8 (*M537Compare)(M537Link *, M537Link *);

M537Base *fn_1_1EA8(M537Base *self, s16 freeIt);
void fn_1_1DDC(void);
M537Main *fn_1_20CC(M537Main *self, s16 freeIt);
void fn_1_246C(void);
void fn_1_2C18(void);
void fn_1_48C0(void);
M537VObj *fn_1_4934(M537VObj *self, s16 freeIt);
void *fn_1_50BC(void *obj, s16 freeIt);
void fn_1_2370(OMOBJ *obj);
void fn_1_2400(M537Main *self);
void fn_1_2470(void);
void fn_1_56C(M537CamLook *cam, Vec *delta);
void fn_1_408(M537CamLook *cam, Vec *right, Vec *up, Vec *fwd);
void fn_1_A20(M537CamLook *cam);
void fn_1_47E4(void *ptr);
M537Link *fn_1_14A0(M537Compare cmp, M537Link *head);
M537Link *fn_1_1B48(M537Link *head);
M537Link *fn_1_1B9C(M537Compare cmp, M537Link *a, M537Link *b);

void *lbl_1_data_1C4[4] = { 0, 0, (void *)fn_1_50BC, 0 };
void *lbl_1_data_1D4[4] = { 0, 0, (void *)fn_1_4934, (void *)fn_1_48C0 };
void *lbl_1_data_1E4[6] = { 0, 0, (void *)fn_1_20CC, 0, (void *)fn_1_246C, 0 };
void *lbl_1_data_1FC[5] = { 0, 0, (void *)fn_1_1EA8, 0, (void *)fn_1_1DDC };

u8 lbl_1_bss_0[12];
M537_NODE lbl_1_bss_C;
s32 lbl_1_bss_14;
s32 lbl_1_bss_18;
s32 lbl_1_bss_1C;
s32 lbl_1_bss_20;
u32 lbl_1_bss_24[0x271];
u8 lbl_1_bss_9E8;
M537Main *lbl_1_bss_9EC;

void *fn_1_0(u32 size)
{
    void *p;

    p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);
    if (p != 0) {
    } else {
        fn_1_1D4("ptr != NULL", "new.cpp", 0x17);
    }
    return p;
}

void *fn_1_64(u32 size)
{
    void *p;

    p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);
    if (p != 0) {
    } else {
        fn_1_1D4("ptr != NULL", "new.cpp", 0x17);
    }
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

void fn_1_1D4(const char *cond, const char *file, s32 line)
{
    char buf[1024];

    sprintf(buf, "\nASSERTION FAILED at %s:%d\n%s", file, line, cond);
    OSPanic("assert.cpp", 0x12, buf);
}

// printf-style entry point; the stream argument is ignored.
int fn_1_234(void *file, const char *fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = vprintf(fmt, ap);
    va_end(ap);
    return ret;
}

// Orbit the camera around its target by (yaw, pitch) degrees.
void fn_1_2D8(M537CamLook *cam, f32 yaw, f32 pitch)
{
    Mtx rotA;
    Mtx rotB;
    Mtx cat;
    Vec right;
    Vec up;
    Vec fwd;

    fn_1_408(cam, &right, &up, &fwd);
    PSMTXRotAxisRad(rotA, &right, 0.017453292f * yaw);
    PSMTXRotRad(rotB, 'Y', 0.017453292f * pitch);
    PSMTXConcat(rotA, rotB, cat);
    PSMTXMultVecSR(cat, &fwd, &fwd);
    PSMTXMultVecSR(cat, &up, &cam->m_up);
    PSVECSubtract(&cam->m_pos, &fwd, &cam->m_target);
}

void fn_1_39C(M537CamLook *cam, f32 roll)
{
    Mtx rot;
    Vec axis;

    PSVECSubtract(&cam->m_target, &cam->m_pos, &axis);
    PSMTXRotAxisRad(rot, &axis, 0.017453292f * roll);
    PSMTXMultVec(rot, &cam->m_up, &cam->m_up);
}

// Camera basis (the three rows of the look-at matrix).
void fn_1_408(M537CamLook *cam, Vec *right, Vec *up, Vec *fwd)
{
    Mtx m;

    C_MTXLookAt(m, &cam->m_pos, &cam->m_up, &cam->m_target);
    right->x = m[0][0];
    right->y = m[0][1];
    right->z = m[0][2];
    up->x = m[1][0];
    up->y = m[1][1];
    up->z = m[1][2];
    fwd->x = m[2][0];
    fwd->y = m[2][1];
    fwd->z = m[2][2];
}

void fn_1_4B0(M537CamLook *cam, Mtx out)
{
    C_MTXLookAt(out, &cam->m_pos, &cam->m_up, &cam->m_target);
    PSMTXInverse(out, out);
}

// Move the camera by a delta given in camera space.
void fn_1_504(M537CamLook *cam, Vec *delta)
{
    Mtx m;
    Vec world;

    C_MTXLookAt(m, &cam->m_pos, &cam->m_up, &cam->m_target);
    PSMTXInverse(m, m);
    PSMTXMultVecSR(m, delta, &world);
    fn_1_56C(cam, &world);
}

void fn_1_56C(M537CamLook *cam, Vec *delta)
{
    PSVECAdd(delta, &cam->m_target, &cam->m_target);
    PSVECAdd(delta, &cam->m_pos, &cam->m_pos);
}

void fn_1_5C0(M537CamLook *cam)
{
    cam->m_up.x = 0.0f;
    cam->m_up.y = 1.0f;
    cam->m_up.z = 0.0f;
    cam->m_pos.x = 0.0f;
    cam->m_pos.y = 0.0f;
    cam->m_pos.z = 5000.0f;
    cam->m_target.x = 0.0f;
    cam->m_target.y = 0.0f;
    cam->m_target.z = 0.0f;
}

void fn_1_654(M537CamLook *cam, M537CamQuat *src)
{
    Mtx m;

    PSMTXQuat(m, &src->m_rot);
    cam->m_up.x = m[0][1];
    cam->m_up.y = m[1][1];
    cam->m_up.z = m[2][1];
    cam->m_pos.x = src->m_pos.x;
    cam->m_pos.y = src->m_pos.y;
    cam->m_pos.z = src->m_pos.z;
    cam->m_target.x = src->m_pos.x - m[0][2];
    cam->m_target.y = src->m_pos.y - m[1][2];
    cam->m_target.z = src->m_pos.z - m[2][2];
}

void fn_1_6F4(M537CamLook *cam, M537CamQuat *dst)
{
    Mtx m;

    C_MTXLookAt(m, &cam->m_pos, &cam->m_up, &cam->m_target);
    PSMTXInverse(m, m);
    dst->m_pos.x = cam->m_pos.x;
    dst->m_pos.y = cam->m_pos.y;
    dst->m_pos.z = cam->m_pos.z;
    C_QUATMtx(&dst->m_rot, m);
}

// Interpolate two quaternion cameras into a look-at camera.
void fn_1_76C(M537CamLook *out, M537CamQuat *a, M537CamQuat *b, f32 t)
{
    Mtx m;
    Quaternion q;
    Vec p0;
    Vec p1;
    Vec p;

    C_QUATSlerp(&a->m_rot, &b->m_rot, &q, t);
    PSVECScale(&a->m_pos, &p0, 1.0f - t);
    PSVECScale(&b->m_pos, &p1, t);
    PSVECAdd(&p0, &p1, &p);
    PSMTXQuat(m, &q);
    out->m_up.x = m[0][1];
    out->m_up.y = m[1][1];
    out->m_up.z = m[2][1];
    out->m_pos.x = p.x;
    out->m_pos.y = p.y;
    out->m_pos.z = p.z;
    out->m_target.x = p.x - m[0][2];
    out->m_target.y = p.y - m[1][2];
    out->m_target.z = p.z - m[2][2];
}

// Move the camera to `pos`, dragging the target along.
void fn_1_87C(M537CamLook *cam, Vec *pos)
{
    Vec delta;

    PSVECSubtract(pos, &cam->m_pos, &delta);
    cam->m_pos.x = pos->x;
    cam->m_pos.y = pos->y;
    cam->m_pos.z = pos->z;
    PSVECAdd(&cam->m_target, &delta, &cam->m_target);
}

void fn_1_8E8(M537CamLook *cam, Vec *a, Vec *b, f32 t)
{
    Vec p0;
    Vec p1;
    Vec delta;

    PSVECScale(a, &p0, 1.0f - t);
    PSVECScale(b, &p1, t);
    PSVECAdd(&p0, &p1, &p1);
    PSVECSubtract(&p1, &cam->m_pos, &delta);
    cam->m_pos.x = p1.x;
    cam->m_pos.y = p1.y;
    cam->m_pos.z = p1.z;
    PSVECAdd(&cam->m_target, &delta, &cam->m_target);
}

void fn_1_9A0(M537CamLook *cam, u16 cameraBit)
{
    Hu3DCameraPosSetV(cameraBit, &cam->m_pos, &cam->m_up, &cam->m_target);
}

void fn_1_9E0(M537CamLook *cam, u16 cameraBit)
{
    Hu3DCameraPosGet(cameraBit, &cam->m_pos, &cam->m_up, &cam->m_target);
}

// Prints the camera as a position plus quaternion (raw float bits).
void fn_1_A20(M537CamLook *cam)
{
    union {
        f32 f;
        u32 u;
    } v[7];
    Mtx m;
    Quaternion q;

    C_MTXLookAt(m, &cam->m_pos, &cam->m_up, &cam->m_target);
    PSMTXInverse(m, m);
    v[0].f = cam->m_pos.x;
    v[1].f = cam->m_pos.y;
    v[2].f = cam->m_pos.z;
    C_QUATMtx(&q, m);
    v[3].f = q.x;
    v[4].f = q.y;
    v[5].f = q.z;
    v[6].f = q.w;
    printf("{{0x%08x,0x%08x,0x%08x},{0x%08x,0x%08x,0x%08x,0x%08x}},\n", v[0].u, v[1].u, v[2].u, v[3].u, v[4].u, v[5].u,
        v[6].u);
}

f32 fn_1_AB8(s32 pad)
{
    return 0.017857144f * (f32)HuPadStkX[pad];
}

f32 fn_1_B0C(s32 pad)
{
    return 0.017857144f * (f32)HuPadStkY[pad];
}

f32 fn_1_B60(s32 pad)
{
    return 0.016949153f * (f32)HuPadSubStkX[pad];
}

f32 fn_1_BB4(s32 pad)
{
    return 0.016949153f * (f32)HuPadSubStkY[pad];
}

// Free-fly debug camera driven by pad `pad`.
void fn_1_C08(Vec *camVec, s32 pad)
{
    M537CamLook *cam = (M537CamLook *)camVec;
    Mtx look;
    Mtx rotA;
    Mtx rotB;
    Mtx cat;
    Mtx inv;
    Vec right;
    Vec up;
    Vec fwd;
    Vec move;
    Vec zmove;
    f32 yaw;
    f32 pitch;
    f32 speed;
    f32 mag;
    u16 btn;

    btn = HuPadBtn[pad];
    if ((btn & 0x800) != 0) {
        cam->m_up.x = 0.0f;
        cam->m_up.y = 1.0f;
        cam->m_up.z = 0.0f;
    }
    yaw = -4.0f * (0.017857144f * (f32)HuPadStkX[pad]);
    pitch = -4.0f * (0.017857144f * (f32)HuPadStkY[pad]);
    C_MTXLookAt(look, &cam->m_pos, &cam->m_up, &cam->m_target);
    right.x = look[0][0];
    right.y = look[0][1];
    right.z = look[0][2];
    up.x = look[1][0];
    up.y = look[1][1];
    up.z = look[1][2];
    fwd.x = look[2][0];
    fwd.y = look[2][1];
    fwd.z = look[2][2];
    PSMTXRotAxisRad(rotA, &right, 0.017453292f * pitch);
    PSMTXRotRad(rotB, 'Y', 0.017453292f * yaw);
    PSMTXConcat(rotA, rotB, cat);
    PSMTXMultVecSR(cat, &fwd, &fwd);
    PSMTXMultVecSR(cat, &up, &cam->m_up);
    PSVECSubtract(&cam->m_pos, &fwd, &cam->m_target);
    if ((u8)((btn & 0x200) != 0) != 0) {
        speed = 70.0f;
    } else {
        speed = 20.0f;
    }
    C_MTXLookAt(inv, &cam->m_pos, &cam->m_up, &cam->m_target);
    PSMTXInverse(inv, inv);
    move.x = speed * (f32)(u8)((btn & 2) != 0) - speed * (f32)(u8)((btn & 1) != 0);
    move.y = speed * (f32)(u8)((btn & 8) != 0) - speed * (f32)(u8)((btn & 4) != 0);
    move.x = move.x + speed * (0.016949153f * (f32)HuPadSubStkX[pad]);
    move.z = 0.0f;
    PSMTXMultVecSR(inv, &move, &move);
    move.y = move.y + speed * (0.016949153f * (f32)HuPadSubStkY[pad]);
    PSVECAdd(&move, &cam->m_target, &cam->m_target);
    PSVECAdd(&move, &cam->m_pos, &cam->m_pos);
    zmove.x = 0.0f;
    zmove.y = 0.0f;
    zmove.z = (f32)((u8)((btn & 0x400) != 0) - (u8)((btn & 0x100) != 0));
    PSMTXMultVecSR(inv, &zmove, &zmove);
    zmove.y = 0.0f;
    mag = PSVECMag(&zmove);
    if (mag > 1e-10f) {
        PSVECScale(&zmove, &zmove, speed / mag);
        PSVECAdd(&zmove, &cam->m_target, &cam->m_target);
        PSVECAdd(&zmove, &cam->m_pos, &cam->m_pos);
    }
    if ((HuPadBtnDown[pad] & 0x200) != 0) {
        fn_1_A20(cam);
    }
}

// ---- circular list primitives

void fn_1_1264(void *node)
{
    M537_NODE *n = (M537_NODE *)node;

    n->next = n;
    n->prev = n;
}

void *fn_1_1270(void *node, s32 freeIt)
{
    M537_NODE *n = (M537_NODE *)node;

    if (n != 0) {
        n->next->prev = n->prev;
        n->prev->next = n->next;
        if (freeIt > 0 && n != 0) {
            HuMemDirectFree(n);
        }
    }
    return n;
}

void fn_1_12DC(void *node)
{
    M537_NODE *n = (M537_NODE *)node;

    n->next->prev = n->prev;
    n->prev->next = n->next;
    n->next = n;
    n->prev = n;
}

// Insert `node` after `at`.
void fn_1_1300(M537_NODE *node, M537_NODE *at)
{
    M537_NODE *anchor = at;

    node->prev = anchor;
    node->next = anchor->next;
    anchor->next->prev = node;
    anchor->next = node;
}

// Insert `node` before `at`.
void fn_1_1330(void *node, void *at)
{
    M537_NODE *n = (M537_NODE *)node;
    M537_NODE *a = (M537_NODE *)at;
    M537_NODE *anchor = a;

    n->next = a;
    n->prev = a->prev;
    a->prev->next = n;
    a->prev = n;
}

// Splice the contents of list `src` in front of `dst`.
void fn_1_1360(M537_NODE *src, M537_NODE *dst)
{
    M537_NODE *first;
    M537_NODE *last;
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
        first->prev = dst->prev;
        last->next = dst;
        dst->prev->next = first;
        dst->prev = last;
    }
}

// Sort a circular list: break it open, merge sort it, relink the back pointers.
void fn_1_13E8(M537_NODE *list, M537Compare cmp)
{
    M537Link *sorted;
    M537_NODE *prev;
    u8 empty;

    empty = 0;
    if (list->next == list && list->next == list->prev) {
        empty = 1;
    }
    if (empty == 0) {
        list->prev->next = 0;
        sorted = fn_1_14A0(cmp, (M537Link *)list->next);
        list->next = (M537_NODE *)sorted;
        prev = list;
        while (sorted != 0) {
            ((M537_NODE *)sorted)->prev = prev;
            prev = (M537_NODE *)sorted;
            sorted = sorted->m_next;
        }
        list->prev = prev;
        prev->next = list;
    }
}

// One merge-sort level, fully expanded three deep in the original; the lowest
// level calls fn_1_14A0 itself.
#define M537_SORT_L3(cmp, in, out)                                                  \
    do {                                                                            \
        M537Link *t3;                                                               \
        M537Link *u3;                                                               \
        t3 = (in);                                                                  \
        if (t3 == 0) {                                                              \
            (out) = 0;                                                              \
        } else if (t3->m_next == 0) {                                               \
            (out) = t3;                                                             \
        } else {                                                                    \
            u3 = fn_1_1B48(t3);                                                     \
            t3 = fn_1_14A0(cmp, t3);                                                \
            u3 = fn_1_14A0(cmp, u3);                                                \
            (out) = fn_1_1B9C(cmp, t3, u3);                                         \
        }                                                                           \
    } while (0)

#define M537_SORT_L2(cmp, in, out)                                                  \
    do {                                                                            \
        M537Link *t2;                                                               \
        M537Link *u2;                                                               \
        M537Link *l2;                                                               \
        M537Link *r2;                                                               \
        t2 = (in);                                                                  \
        if (t2 == 0) {                                                              \
            (out) = 0;                                                              \
        } else if (t2->m_next == 0) {                                               \
            (out) = t2;                                                             \
        } else {                                                                    \
            u2 = fn_1_1B48(t2);                                                     \
            M537_SORT_L3(cmp, t2, l2);                                              \
            M537_SORT_L3(cmp, u2, r2);                                              \
            (out) = fn_1_1B9C(cmp, l2, r2);                                         \
        }                                                                           \
    } while (0)

#define M537_SORT_L1(cmp, in, out)                                                  \
    do {                                                                            \
        M537Link *t1;                                                               \
        M537Link *u1;                                                               \
        M537Link *l1;                                                               \
        M537Link *r1;                                                               \
        t1 = (in);                                                                  \
        if (t1 == 0) {                                                              \
            (out) = 0;                                                              \
        } else if (t1->m_next == 0) {                                               \
            (out) = t1;                                                             \
        } else {                                                                    \
            u1 = fn_1_1B48(t1);                                                     \
            M537_SORT_L2(cmp, t1, l1);                                              \
            M537_SORT_L2(cmp, u1, r1);                                              \
            (out) = fn_1_1B9C(cmp, l1, r1);                                         \
        }                                                                           \
    } while (0)

M537Link *fn_1_14A0(M537Compare cmp, M537Link *head)
{
    M537Link *half;
    M537Link *left;
    M537Link *right;
    M537Link *res;

    if (head == 0) {
        return 0;
    }
    if (head->m_next == 0) {
        return head;
    }
    half = fn_1_1B48(head);
    M537_SORT_L1(cmp, head, left);
    M537_SORT_L1(cmp, half, right);
    res = fn_1_1B9C(cmp, left, right);
    return res;
}

// Cut the list in the middle and return the second half.
M537Link *fn_1_1B48(M537Link *head)
{
    M537Link *slow;
    M537Link *fast;
    M537Link *second;

    slow = head;
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
    second = slow->m_next;
    slow->m_next = 0;
    return second;
}

// Merge two sorted singly linked lists.
M537Link *fn_1_1B9C(M537Compare cmp, M537Link *a, M537Link *b)
{
    M537Link dummy;
    M537Link *tail;

    tail = &dummy;
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
    return dummy.m_next;
}

// ---- work object list at lbl_1_bss_C

void fn_1_1C48(s32 bit)
{
    M537Base *obj;
    M537Base *p0;
    M537Base *p1;
    M537Base *end;
    M537Base *nx;

    p0 = (M537Base *)lbl_1_bss_C.next;
    p1 = p0;
    obj = p1;
    end = (M537Base *)&lbl_1_bss_C;
    lbl_1_bss_14 = bit;
    while (obj != end) {
        obj->m_flag = obj->m_flag | (1 << bit);
        ((void (*)(M537Base *, s32))obj->m_vptr[3])(obj, bit);
        nx = (M537Base *)obj->m_node.next;
        obj = nx;
    }
}

s32 fn_1_1CE0(void)
{
    M537Base *obj;
    M537Base *p0;
    M537Base *p1;
    M537Base *end;
    M537Base *nx;

    p0 = (M537Base *)lbl_1_bss_C.next;
    p1 = p0;
    obj = p1;
    end = (M537Base *)&lbl_1_bss_C;
    while (obj != end) {
        if ((obj->m_flag & (1 << lbl_1_bss_14)) != 0) {
            return 0;
        }
        nx = (M537Base *)obj->m_node.next;
        obj = nx;
    }
    return 1;
}

void fn_1_1D6C(void)
{
    M537Base *obj;
    M537Base *p0;
    M537Base *p1;
    M537Base *end;
    M537Base *nx;

    p0 = (M537Base *)lbl_1_bss_C.next;
    p1 = p0;
    obj = p1;
    end = (M537Base *)&lbl_1_bss_C;
    while (obj != end) {
        ((void (*)(M537Base *))obj->m_vptr[4])(obj);
        nx = (M537Base *)obj->m_node.next;
        obj = nx;
    }
}

void fn_1_1DDC(void)
{
}

void fn_1_1DE0(void)
{
    u8 empty;
    M537Base *last;

    for (;;) {
        empty = 0;
        if (lbl_1_bss_C.next == &lbl_1_bss_C && lbl_1_bss_C.next == lbl_1_bss_C.prev) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        last = (M537Base *)lbl_1_bss_C.prev;
        if (last != 0) {
            ((void (*)(M537Base *, s32))last->m_vptr[2])(last, 1);
        }
    }
}

M537Base *fn_1_1EA8(M537Base *self, s16 freeIt)
{
    if (self != 0) {
        self->m_vptr = lbl_1_data_1FC;
        if (self != 0 && self != 0) {
            self->m_node.next->prev = self->m_node.prev;
            self->m_node.prev->next = self->m_node.next;
        }
        if (freeIt > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

s32 fn_1_1F30(void)
{
    return lbl_1_bss_14;
}

M537Main *fn_1_1F40(M537Main *self)
{
    s32 i;

    if (lbl_1_bss_9EC == 0) {
    } else {
        M537_ASSERT_FAIL("ms_pointer == 0", "singleton.h", 0x35);
    }
    lbl_1_bss_9EC = self;
    self->m_vptr = lbl_1_data_1E4;
    self->m_timer = 0;
    for (i = 0; i < 4; i++) {
        self->m_player[i] = -1;
    }
    return self;
}

M537Main *fn_1_2010(M537Main *self, s16 freeIt)
{
    if (self != 0) {
        if (lbl_1_bss_9EC != 0) {
        } else {
            M537_ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_9EC = 0;
        if (freeIt > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

M537Main *fn_1_20CC(M537Main *self, s16 freeIt)
{
    M537Main *ms;
    s32 i;
    s32 player;

    if (self != 0) {
        self->m_vptr = lbl_1_data_1E4;
        if (mgDataTbl[GwSystem.mgNo].type != 3) {
            if (lbl_1_bss_9EC != 0) {
            } else {
                M537_ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x27);
            }
            ms = lbl_1_bss_9EC;
            for (i = 0; i < 4; i++) {
                player = ms->m_player[i];
                if (player >= 0 && _CheckFlag(0x1000F) == 0) {
                    GwPlayer[player].mgCoinBonus = 0xA;
                }
            }
        }
        if (self != 0) {
            if (lbl_1_bss_9EC != 0) {
            } else {
                M537_ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x3D);
            }
            lbl_1_bss_9EC = 0;
        }
        if (freeIt > 0) {
            fn_1_47E4(self);
        }
    }
    return self;
}

void fn_1_2284(M537Main *self, s32 objMax, s32 prio)
{
    OMOBJMAN *objman;

    objman = omInitObjMan(objMax, prio);
    omGameSysInit(objman);
    ((void (*)(M537Main *, OMOBJMAN *))self->m_vptr[3])(self, objman);
    omAddObjEx(objman, 0, 0, 0, -1, fn_1_2370);
}

void fn_1_2308(M537Main *self)
{
    OMOBJMAN *objman;

    objman = MgActorObjectSetup();
    ((void (*)(M537Main *, OMOBJMAN *))self->m_vptr[3])(self, objman);
    omAddObjEx(objman, 0, 0, 0, -1, fn_1_2370);
}

void fn_1_2370(OMOBJ *obj)
{
    M537Main *ms0;
    M537Main *ms1;

    if (omSysExitReq != 0 && WipeCheckEnd() == 0) {
        fn_1_2C18();
        HuAudFadeOut(1000);
        obj->objFunc = (OMOBJFUNC)fn_1_2470;
        return;
    }
    ms0 = lbl_1_bss_9EC;
    ms1 = ms0;
    fn_1_2400(ms1);
}

void fn_1_2400(M537Main *self)
{
    ((void (*)(M537Main *))self->m_vptr[4])(self);
    if (self->m_timer > 0) {
        if (--self->m_timer == 0) {
        }
    }
}

void fn_1_246C(void)
{
}

void fn_1_2470(void)
{
    M537Main *ms;
    M537Main *ms0;

    ms0 = lbl_1_bss_9EC;
    ms = ms0;
    if (WipeCheckEnd() != 0) {
        ((void (*)(M537Main *))ms->m_vptr[4])(ms);
        if (ms->m_timer > 0) {
            if (--ms->m_timer == 0) {
            }
        }
        return;
    }
    ((void (*)(M537Main *))ms->m_vptr[5])(ms);
    if (ms != 0) {
        ((void (*)(M537Main *, s32))ms->m_vptr[2])(ms, 1);
    }
    CharModelKill(-1);
    omOvlReturnEx(1, 1);
}

// ---- module main object helpers (player slots, game messages)

void fn_1_255C(s32 playerId)
{
    M537Main *ms;
    s32 *slots;
    s32 i;

    if (lbl_1_bss_9EC != 0) {
    } else {
        M537_ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x27);
    }
    slots = lbl_1_bss_9EC->m_player;
    for (i = 0; i < 4; i++) {
        if (slots[i] < 0) {
            slots[i] = playerId;
            return;
        }
    }
}

s32 fn_1_2630(s32 playerId)
{
    s32 i;

    if (lbl_1_bss_9EC != 0) {
    } else {
        M537_ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x27);
    }
    for (i = 0; i < 4; i++) {
        if (playerId == lbl_1_bss_9EC->m_player[i]) {
            return 1;
        }
    }
    return 0;
}

void fn_1_2704(void)
{
    M537Main *ms;
    M537Main *ms0;
    M537Main *ms1;

    if (lbl_1_bss_9EC != 0) {
    } else {
        M537_ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x27);
    }
    ms0 = lbl_1_bss_9EC;
    ms1 = ms0;
    ms = ms1;
    ms->m_msg = GameMesCreate(2, 0);
}

void fn_1_27BC(void)
{
    M537Main *ms;
    M537Main *ms0;
    M537Main *ms1;

    if (lbl_1_bss_9EC != 0) {
    } else {
        M537_ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x27);
    }
    ms0 = lbl_1_bss_9EC;
    ms1 = ms0;
    ms = ms1;
    ms->m_msg = GameMesCreate(2, 1);
}

s32 fn_1_2874(void)
{
    M537Main *ms;
    s32 ret;

    if (lbl_1_bss_9EC != 0) {
    } else {
        M537_ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x27);
    }
    ms = lbl_1_bss_9EC;
    ret = 0;
    if (GameMesStatGet(-1) == 0 && (u8)(ms->m_timer <= 0) != 0) {
        ret = 1;
    }
    return ret;
}

// Announces the result: the players that joined are sorted by player number.
void fn_1_2948(void)
{
    s32 *slots;
    s32 chars[4];
    s32 n;
    s32 i;
    s32 j;
    s32 t;
    s16 charNo;

    if (lbl_1_bss_9EC != 0) {
    } else {
        M537_ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x27);
    }
    lbl_1_bss_9EC->m_timer = lbl_1_bss_1C;
    slots = lbl_1_bss_9EC->m_player;
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
    for (i = 1; i < n; i++) {
        j = i;
        while (j >= 1 && slots[j - 1] > slots[j]) {
            t = slots[j - 1];
            slots[j - 1] = slots[j];
            slots[j] = t;
            j--;
        }
    }
    for (i = 0; i < 4; i++) {
        if (slots[i] < 0) {
            charNo = -1;
        } else {
            charNo = GwPlayerConf[slots[i]].charNo;
        }
        chars[i] = charNo;
    }
    GameMesCreate(4, 3, chars[0], chars[1], chars[2], chars[3]);
}

void fn_1_2B14(s32 result)
{
    M537Main *ms;

    if (_CheckFlag(0x1000F) == 0) {
        if (lbl_1_bss_9EC != 0) {
        } else {
            M537_ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x27);
        }
        ms = lbl_1_bss_9EC;
        ms->m_msg = GameMesCreate(0xE, result);
        HuAudJinglePlay(0x86);
    }
}

// ---- Hu3D wrappers that take a pointer to the model id

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

s16 *fn_1_2C58(s16 *out, void *data)
{
    *out = Hu3DModelCreate(data);
    return out;
}

s16 *fn_1_2C94(s16 *out, int dataNum)
{
    *out = Hu3DModelCreate(HuDataSelHeapReadNum(dataNum, 0x10000000, HEAP_MODEL));
    return out;
}

s16 *fn_1_2CDC(s16 *out, void *hookFunc)
{
    *out = Hu3DHookFuncCreate((HU3DMODELHOOK)hookFunc);
    return out;
}

s16 *fn_1_2D18(s16 *out, s16 charNo, s16 type)
{
    s16 attr;

    attr = 0;
    switch (type) {
    case 0:
        attr = 1;
        break;
    case 1:
        attr = 2;
        break;
    case 2:
        attr = 4;
        break;
    case 3:
        attr = 8;
        break;
    }
    *out = CharModelCreate(charNo, attr);
    return out;
}

s16 *fn_1_2DB4(s16 *out, s16 *modelId)
{
    s16 id = (s16)*modelId;

    *out = Hu3DModelLink(id);
    return out;
}

void fn_1_2E08(s16 *modelId, u32 attr)
{
    Hu3DModelAttrSet(*modelId, attr);
}

void fn_1_2E40(s16 *modelId, u32 attr)
{
    Hu3DModelAttrReset(*modelId, attr);
}

// Placement block: pad, then position, rotation and scale triples.
struct M537Xform {
    u8 m_pad[0x18];
    Vec m_pos;
    Vec m_rot;
    Vec m_scale;
};

void fn_1_2E78(s16 *modelId, M537Xform *xform)
{
    s16 id = *modelId;

    Hu3DModelPosSet(id, xform->m_pos.x, xform->m_pos.y, xform->m_pos.z);
    Hu3DModelRotSet(id, xform->m_rot.x, xform->m_rot.y, xform->m_rot.z);
    Hu3DModelScaleSet(id, xform->m_scale.x, xform->m_scale.y, xform->m_scale.z);
}

void fn_1_2EF4(s16 *modelId, Vec *pos)
{
    Hu3DModelPosSet(*modelId, pos->x, pos->y, pos->z);
}

void fn_1_2F3C(s16 *modelId, Vec *rot)
{
    Hu3DModelRotSet(*modelId, rot->x, rot->y, rot->z);
}

void fn_1_2F84(s16 *modelId, Vec *scale)
{
    Hu3DModelScaleSet(*modelId, scale->x, scale->y, scale->z);
}

void fn_1_2FCC(s16 *modelId, f32 scale)
{
    Hu3DModelScaleSet(*modelId, scale, scale, scale);
}

void fn_1_3014(s16 *modelId, f32 lvl)
{
    Hu3DModelTPLvlSet(*modelId, lvl);
}

void fn_1_304C(s16 *modelId, s16 layerNo)
{
    Hu3DModelLayerSet(*modelId, layerNo);
}

void fn_1_3084(s16 *modelId, u16 cameraBit)
{
    Hu3DModelCameraSet(*modelId, cameraBit);
}

void fn_1_30BC(s16 *modelId, void *hookData)
{
    Hu3DData[*modelId].hookData = hookData;
}

Mtx *fn_1_30D8(s16 *modelId)
{
    return &Hu3DData[*modelId].mtx;
}

s16 *fn_1_30F4(s16 *modelId, s16 freeIt)
{
    if (modelId != 0) {
        Hu3DModelKill(*modelId);
        if (freeIt > 0 && modelId != 0) {
            HuMemDirectFree(modelId);
        }
    }
    return modelId;
}

HSFOBJECT *fn_1_3150(s16 *modelId, char *objName)
{
    HSFOBJECT *obj;

    obj = Hu3DModelObjPtrGet(*modelId, objName);
    if (obj != 0) {
    } else {
        M537_ASSERT_FAIL("p != NULL", "model.cpp", 0x8A);
    }
    return obj;
}

void fn_1_31E4(s16 *modelId)
{
    Hu3DModelShadowSet(*modelId);
}

void fn_1_3214(s16 *modelId)
{
    Hu3DModelShadowMapSet(*modelId);
}

void fn_1_3244(s16 *modelId, char *objName)
{
    Hu3DModelShadowMapObjSet(*modelId, objName);
}

void fn_1_327C(s16 *modelId)
{
    Hu3DModelShadowMapObjReset(*modelId);
}

void fn_1_32AC(s16 *modelId, char *objName)
{
    HSFOBJECT *obj;
    u32 *flags;

    obj = Hu3DModelObjPtrGet(*modelId, objName);
    if (obj != 0) {
    } else {
        M537_ASSERT_FAIL("p != NULL", "model.cpp", 0x8A);
    }
    flags = (u32 *)obj->constData;
    *flags &= 0xFFFFFFF7;
}

void fn_1_3358(s16 *modelId, char *objName)
{
    HSFOBJECT *obj;

    obj = Hu3DModelObjPtrGet(*modelId, objName);
    if (obj != 0) {
    } else {
        M537_ASSERT_FAIL("p != NULL", "model.cpp", 0x8A);
    }
    obj->flags = obj->flags | 0x400;
}

void fn_1_3400(s16 *modelId, char *objName)
{
    HSFOBJECT *obj;

    obj = Hu3DModelObjPtrGet(*modelId, objName);
    if (obj != 0) {
    } else {
        M537_ASSERT_FAIL("p != NULL", "model.cpp", 0x8A);
    }
    obj->flags = obj->flags & 0xFFFFFBFF;
}

void fn_1_34A8(s16 *modelId, char *objName)
{
    HSFOBJECT *obj;
    HSFTRANSFORM *base;

    obj = Hu3DModelObjPtrGet(*modelId, objName);
    if (obj != 0) {
    } else {
        M537_ASSERT_FAIL("p != NULL", "model.cpp", 0x8A);
    }
    obj->flags = obj->flags | 0x400;
    base = &obj->mesh.base;
    base->scale.x = 0.0f;
    base->scale.y = 0.0f;
    base->scale.z = 0.0f;
}

void fn_1_3584(s16 *modelId, char *objName)
{
    HSFOBJECT *obj;
    HSFTRANSFORM *base;

    obj = Hu3DModelObjPtrGet(*modelId, objName);
    if (obj != 0) {
    } else {
        M537_ASSERT_FAIL("p != NULL", "model.cpp", 0x8A);
    }
    obj->flags = obj->flags & 0xFFFFFBFF;
    base = &obj->mesh.base;
    base->scale.x = 1.0f;
    base->scale.y = 1.0f;
    base->scale.z = 1.0f;
}

void fn_1_3660(s16 *modelId, s32 time)
{
    Hu3DMotionTimeSet(*modelId, (f32)time);
}

// ---- Mersenne Twister (MT19937): state is 0x270 words followed by the index.

M537_RODATA const u32 lbl_1_rodata_50[2] = { 0, 0x9908B0DF };

// Generates the next tempered 32-bit output of the generator at `mt` into `out`.
#define M537_MT_NEXT(mt, out)                                                        \
    do {                                                                             \
        u32 k;                                                                       \
        u32 y;                                                                       \
        u32 idx;                                                                     \
        if ((mt)[0x270] >= 0x270) {                                                  \
            k = 0;                                                                   \
            while (k < 0xE3) {                                                       \
                y = ((mt)[k] & 0x80000000) | ((mt)[k + 1] & 0x7FFFFFFF);             \
                (mt)[k] = lbl_1_rodata_50[y & 1] ^ ((y >> 1) ^ (mt)[k + 0x18D]);     \
                k++;                                                                 \
            }                                                                        \
            while (k < 0x26F) {                                                      \
                y = ((mt)[k] & 0x80000000) | ((mt)[k + 1] & 0x7FFFFFFF);             \
                (mt)[k] = lbl_1_rodata_50[y & 1] ^ ((y >> 1) ^ (mt)[k - 0xE3]);      \
                k++;                                                                 \
            }                                                                        \
            y = ((mt)[0x26F] & 0x80000000) | ((mt)[0] & 0x7FFFFFFF);                 \
            (mt)[0x26F] = (y >> 1) ^ (mt)[0x18C] ^ lbl_1_rodata_50[y & 1];           \
            (mt)[0x270] = 0;                                                         \
        }                                                                            \
        idx = (mt)[0x270];                                                           \
        (mt)[0x270] = idx + 1;                                                       \
        y = (mt)[idx];                                                               \
        y = y ^ (y >> 11);                                                           \
        y = y ^ ((y << 7) & 0x9D2C5680);                                             \
        y = y ^ ((y << 15) & 0xEFC60000);                                            \
        (out) = y ^ (y >> 18);                                                       \
    } while (0)

void fn_1_36CC(u32 *mt, u32 seed)
{
    mt[0] = seed;
    mt[0x270] = 1;
    while (mt[0x270] < 0x270) {
        mt[mt[0x270]] = mt[0x270] + 0x6C078965 * (mt[mt[0x270] - 1] ^ (mt[mt[0x270] - 1] >> 30));
        mt[0x270]++;
    }
}

// init_by_array
void fn_1_3740(u32 *mt, u32 *key, u32 keyLen)
{
    u32 i;
    u32 j;
    u32 k;

    fn_1_36CC(mt, 0x012BD6AA);
    i = 1;
    j = 0;
    if (keyLen < 0x270) {
        k = 0x270;
    } else {
        k = keyLen;
    }
    while (k != 0) {
        mt[i] = key[j] + ((mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 0x19660D)) + j);
        i++;
        j++;
        if (i >= 0x270) {
            mt[0] = mt[0x26F];
            i = 1;
        }
        if (j >= keyLen) {
            j = 0;
        }
        k--;
    }
    k = 0x26F;
    while (k != 0) {
        mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 0x5D588B65)) - i;
        i++;
        if (i >= 0x270) {
            mt[0] = mt[0x26F];
            i = 1;
        }
        k--;
    }
    mt[0] = 0x80000000;
}

// 32-bit integer
u32 fn_1_3904(u32 *mt)
{
    u32 r;

    M537_MT_NEXT(mt, r);
    return r;
}

// 31-bit integer
u32 fn_1_3A90(u32 *mt)
{
    u32 r;

    M537_MT_NEXT(mt, r);
    return r >> 1;
}

// real in [0,1]
f32 fn_1_3C28(u32 *mt)
{
    u32 r;

    M537_MT_NEXT(mt, r);
    return 2.3283064e-10f * (f32)r;
}

f32 fn_1_3DEC(u32 *mt)
{
    u32 r;

    M537_MT_NEXT(mt, r);
    return 2.3283064e-10f * (f32)r;
}

// real in (0,1)
f32 fn_1_3FB0(u32 *mt)
{
    u32 r;

    M537_MT_NEXT(mt, r);
    return 2.3283064e-10f * (0.5f + (f32)r);
}

// 53-bit resolution real in [0,1)
f64 fn_1_4184(u32 *mt)
{
    u32 a;
    u32 b;

    M537_MT_NEXT(mt, a);
    M537_MT_NEXT(mt, b);
    return 1.1102230246251565e-16 * (67108864.0 * (f64)(a >> 5) + (f64)(b >> 6));
}

// 24-bit resolution real in [0,1)
f32 fn_1_44FC(u32 *mt)
{
    u32 r;

    M537_MT_NEXT(mt, r);
    return 1.1920929e-7f * (f32)(r >> 9);
}

// ---- allocators with their own assert text

void *fn_1_46CC(u32 size)
{
    void *p;

    p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);
    if (p != 0) {
    } else {
        M537_ASSERT_FAIL("ptr != NULL", "om.cpp", 0x18);
    }
    return p;
}

void *fn_1_4758(u32 size)
{
    void *p;

    p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);
    if (p != 0) {
    } else {
        M537_ASSERT_FAIL("ptr != NULL", "om.cpp", 0x1F);
    }
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

// ---- object manager glue: an om object owns a polymorphic work object (om->data)

void fn_1_4874(OMOBJ *om);

void fn_1_4854(M537VObj *object, OMOBJ *om)
{
    object->m_vptr = lbl_1_data_1D4;
    om->data = object;
    om->objFunc = (OMOBJFUNC)fn_1_4874;
}

void fn_1_4874(OMOBJ *om)
{
    M537VObj *object;

    object = (M537VObj *)om->data;
    ((void (*)(M537VObj *, OMOBJ *))object->m_vptr[3])(object, om);
}

void fn_1_48C0(void)
{
}

void fn_1_48C4(OMOBJ *om)
{
    M537VObj *object;

    object = (M537VObj *)om->data;
    if (object != 0) {
        ((void (*)(M537VObj *, s32))object->m_vptr[2])(object, 1);
    }
    omDelObjEx(HuPrcCurrentGet(), om);
}

M537VObj *fn_1_4934(M537VObj *self, s16 freeIt)
{
    if (self != 0) {
        self->m_vptr = lbl_1_data_1D4;
        if (freeIt > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_4994(HUPROCESS *proc, OMOBJ *om)
{
    M537VObj *object;

    object = (M537VObj *)om->data;
    if (object != 0) {
        ((void (*)(M537VObj *, s32))object->m_vptr[2])(object, 1);
    }
    omDelObjEx(proc, om);
}

// ---- shared random number generator (instance at lbl_1_bss_24)

void fn_1_4A08(u32 seed)
{
    lbl_1_bss_24[0] = seed;
    lbl_1_bss_24[0x270] = 1;
    while (lbl_1_bss_24[0x270] < 0x270) {
        lbl_1_bss_24[lbl_1_bss_24[0x270]] =
            lbl_1_bss_24[0x270] + 0x6C078965 * (lbl_1_bss_24[lbl_1_bss_24[0x270] - 1] ^ (lbl_1_bss_24[lbl_1_bss_24[0x270] - 1] >> 30));
        lbl_1_bss_24[0x270]++;
    }
}

s32 fn_1_4AE0(void)
{
    u32 r;

    M537_MT_NEXT(lbl_1_bss_24, r);
    return r >> 1;
}

f32 fn_1_4D1C(void)
{
    u32 r;

    M537_MT_NEXT(lbl_1_bss_24, r);
    return 1.1920929e-7f * (f32)(r >> 9);
}

f32 fn_1_4F90(f32 scale)
{
    return (f32)((f64)scale * fn_1_4184(lbl_1_bss_24));
}

f32 fn_1_4FC8(f32 lo, f32 hi)
{
    return (f32)((f64)lo + (f64)(hi - lo) * fn_1_4184(lbl_1_bss_24));
}

// ---- threads

struct M537Semaphore {
    s32 m_count;
    OSThreadQueue m_queue;
};

void OSInitSemaphore(M537Semaphore *sem, s32 count);
s32 OSWaitSemaphore(M537Semaphore *sem);
s32 OSSignalSemaphore(M537Semaphore *sem);

// OS thread plus the heap block holding its stack.
struct M537Thread {
    OSThread m_os;      /* 0x000 */
    u32 *m_stack;       /* 0x318 */
};

// Thread object: vtable (slot 3 is the thread body), the thread and a semaphore
// the creator waits on.
struct M537CThread {
    void **m_vptr;          /* 0x000 */
    M537Thread m_th;        /* 0x008 */
    M537Semaphore m_sem;    /* 0x328 */
    s32 m_value;            /* 0x334 */
};

M537Thread *fn_1_5360(M537Thread *th, void *(*entry)(void *), void *param, u32 stackSize, s32 priority, u16 attr);
void fn_1_54C0(M537Thread *th, void *(*entry)(void *), void *param, u32 *stack, u32 words, s32 priority, u16 attr);
M537Thread *fn_1_55E4(M537Thread *th, s16 freeIt);
u32 fn_1_5718(M537Thread *th);
s32 fn_1_57B8(u32 *stack, s32 mode);
void fn_1_582C(u32 *stack, u32 words);
void *fn_1_5304(void *self);
void *fn_1_5DD8(void *self, s16 freeIt);

s32 fn_1_501C(void)
{
    return OSGetThreadPriority(OSGetCurrentThread());
}

M537CThread *fn_1_5040(M537CThread *self, u32 stackSize)
{
    self->m_vptr = lbl_1_data_1C4;
    fn_1_5360(&self->m_th, fn_1_5304, self, stackSize, OSGetThreadPriority(OSGetCurrentThread()), 1);
    OSInitSemaphore(&self->m_sem, 0);
    return self;
}

void *fn_1_50BC(void *obj, s16 freeIt)
{
    M537CThread *self = (M537CThread *)obj;

    if (self != 0) {
        self->m_vptr = lbl_1_data_1C4;
        fn_1_55E4(&self->m_th, -1);
        if (freeIt > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

u8 fn_1_5128(M537CThread *self)
{
    OSThread *th;

    th = &self->m_th.m_os;
    return (u8)(OSIsThreadTerminated(th) != 0);
}

// Resume the thread with `value` and wait until it hands control back.
s32 fn_1_5170(M537CThread *self, s32 value)
{
    OSThread *th0;
    OSThread *th1;
    M537Semaphore *sem;

    th0 = &self->m_th.m_os;
    if (OSIsThreadTerminated(th0) != 0) {
        return self->m_value;
    }
    self->m_value = value;
    th1 = &self->m_th.m_os;
    OSResumeThread(th1);
    sem = &self->m_sem;
    OSWaitSemaphore(sem);
    fn_1_5718(&self->m_th);
    OSCheckActiveThreads();
    return self->m_value;
}

// Called from inside the thread: hand `value` back to the creator and suspend.
s32 fn_1_5200(M537CThread *self, s32 value)
{
    if (OSGetCurrentThread() == &self->m_th.m_os) {
    } else {
        M537_ASSERT_FAIL("::OSGetCurrentThread() == m_th", "subthread.cpp", 0x47);
    }
    if (OSIsThreadTerminated(&self->m_th.m_os) == 0) {
    } else {
        M537_ASSERT_FAIL("!::OSIsThreadTerminated(m_th)", "subthread.cpp", 0x4A);
    }
    self->m_value = value;
    OSSignalSemaphore(&self->m_sem);
    OSSuspendThread(&self->m_th.m_os);
    return self->m_value;
}

void *fn_1_5304(void *obj)
{
    M537CThread *self = (M537CThread *)obj;

    ((void (*)(M537CThread *, s32))self->m_vptr[3])(self, self->m_value);
    OSSignalSemaphore(&self->m_sem);
    return 0;
}

// Thread with a heap-allocated stack.
M537Thread *fn_1_5360(M537Thread *th, void *(*entry)(void *), void *param, u32 stackSize, s32 priority, u16 attr)
{
    u32 words;
    u32 *stack;

    words = stackSize >> 2;
    stack = (u32 *)HuMemDirectMallocNum(HEAP_HEAP, words * 4, 0x10000000);
    if (stack != 0) {
    } else {
        M537_ASSERT_FAIL("ptr != NULL", "new.cpp", 0x17);
    }
    th->m_stack = stack;
    fn_1_54C0(th, entry, param, stack, words, priority, attr);
    return th;
}

// Thread on a caller-supplied stack.
M537Thread *fn_1_5444(M537Thread *th, void *(*entry)(void *), void *param, u32 *stack, u32 stackSize, s32 priority, u16 attr)
{
    th->m_stack = 0;
    fn_1_54C0(th, entry, param, stack, stackSize >> 2, priority, attr);
    return th;
}

void fn_1_54C0(M537Thread *th, void *(*entry)(void *), void *param, u32 *stack, u32 words, s32 priority, u16 attr)
{
    u8 ok;

    ok = 0;
    if (priority >= 0 && priority <= 0x1F) {
        ok = 1;
    }
    if (ok != 0) {
    } else {
        M537_ASSERT_FAIL("priority >= OS_PRIORITY_MIN && priority <= OS_PRIORITY_MAX", "thread.cpp", 0x52);
    }
    fn_1_582C(stack, words);
    fn_1_57B8(stack, 0);
    if ((u8)(OSCreateThread(&th->m_os, entry, param, &stack[words], words * 4, priority, attr) != 0) == 0) {
        OSPanic("thread.cpp", 0x60, "OSCreateThread() failed.");
    }
}

M537Thread *fn_1_55E4(M537Thread *th, s16 freeIt)
{
    u32 *stack;
    u32 total;
    u32 remain;

    if (th != 0) {
        if (OSIsThreadTerminated(&th->m_os) == 0) {
            OSCancelThread(&th->m_os);
        }
        if (lbl_1_bss_9E8 != 0) {
            total = (u32)(th->m_os.stackBase - (u8 *)th->m_os.stackEnd);
            remain = fn_1_5718(th);
            printf("delete CThread@%p\nstack remain/total = 0x%08x/0x%08x (%f%%)\n", th, remain, total,
                (100.0 * (f64)remain) / (f64)total);
        }
        stack = th->m_stack;
        if (stack != 0) {
            HuMemDirectFree(stack);
        }
        if (freeIt > 0 && th != 0) {
            HuMemDirectFree(th);
        }
    }
    return th;
}

// Bytes of stack never touched (checks the canary at the end of the stack first).
u32 fn_1_5718(M537Thread *th)
{
    u32 *end;
    u32 *p;

    end = th->m_os.stackEnd;
    if (fn_1_57B8(end, 1) != 0) {
        OSPanic("thread.cpp", 0x7F, "stack overflow\n");
    }
    p = end;
    while (p < (u32 *)th->m_os.stackBase) {
        if (*p == 0xDEADBABE) {
            p++;
            continue;
        }
        break;
    }
    return (u32)((u8 *)p - (u8 *)end);
}

// mode 0 writes the 8-word canary, mode 1 returns non-zero when it was overwritten.
s32 fn_1_57B8(u32 *stack, s32 mode)
{
    u32 *p;
    u32 *end;

    p = stack;
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
            if (*p++ != 0xDEADBABE) {
                return 1;
            }
        }
        return 0;
    default:
        return 0;
    }
}

void fn_1_582C(u32 *stack, u32 words)
{
    u32 *p;
    u32 *end;

    p = stack;
    if (lbl_1_bss_9E8 != 0) {
        end = p + words;
        while (p != end) {
            *p = 0xDEADBABE;
            p++;
        }
    }
}

// ---- timer handle wrappers

MGTIMER **fn_1_5878(MGTIMER **out, s32 type)
{
    switch (type) {
    case 0:
        *out = MgTimerCreate(0);
        break;
    case 1:
        *out = MgTimerCreate(1);
        break;
    case 2:
        *out = MgTimerCreate(2);
        break;
    default:
        M537_ASSERT_FAIL("false", "timer.cpp", 0x1E);
        break;
    }
    return out;
}

MGTIMER **fn_1_5940(MGTIMER **timer, s16 freeIt)
{
    if (timer != 0) {
        MgTimerKill(*timer);
        if (freeIt > 0 && timer != 0) {
            HuMemDirectFree(timer);
        }
    }
    return timer;
}

void fn_1_599C(MGTIMER **timer, s32 maxTime, s32 endTime, s32 recordTime)
{
    MgTimerParamSet(*timer, maxTime, endTime, recordTime);
}

void fn_1_59E4(MGTIMER **timer)
{
    MgTimerValueGet(*timer);
}

void fn_1_5A14(MGTIMER **timer, f32 x, f32 y)
{
    MgTimerPosSet(*timer, x, y);
}

void fn_1_5A54(MGTIMER **timer, f32 *x, f32 *y)
{
    MgTimerPosGet(*timer, x, y);
}

void fn_1_5A94(MGTIMER **timer, s32 offType)
{
    switch (offType) {
    case 0:
        MgTimerModeOnSet(*timer, 0);
        return;
    case 1:
        MgTimerModeOnSet(*timer, 1);
        return;
    case 2:
        MgTimerModeOnSet(*timer, 2);
        return;
    default:
        M537_ASSERT_FAIL("false", "timer.cpp", 0x48);
        return;
    }
}

void fn_1_5B58(MGTIMER **timer)
{
    MgTimerModeOffSet(*timer);
}

void fn_1_5B88(MGTIMER **timer)
{
    if ((*timer)->stopF == 0) {
        (*timer)->stopF = 1;
        (*timer)->mode = 1;
    }
}

void fn_1_5BB4(MGTIMER **timer)
{
    MgTimerKill(*timer);
    *timer = MgTimerCreate(0);
}

u8 fn_1_5BF0(MGTIMER **timer)
{
    return (u8)(MgTimerDoneCheck(*timer) != 0);
}

void fn_1_5C30(MGTIMER **timer, u8 r, u8 g, u8 b)
{
    MgTimerWinColorSet(*timer, r, g, b);
}

// Static initializer: the work object list, the default speeds and the RNG seed.
void fn_1_5C78(void)
{
    lbl_1_bss_C.next = &lbl_1_bss_C;
    lbl_1_bss_C.prev = &lbl_1_bss_C;
    __register_global_object(&lbl_1_bss_C, (void *)fn_1_5DD8, &lbl_1_bss_0);
    lbl_1_bss_18 = 0x78;
    lbl_1_bss_1C = 0xD2;
    lbl_1_bss_20 = 0xD2;
    fn_1_4A08(0x1571);
}

void *fn_1_5DD8(void *obj, s16 freeIt)
{
    M537_NODE *self = (M537_NODE *)obj;

    if (self != 0) {
        if (self != 0) {
            self->next->prev = self->prev;
            self->prev->next = self->next;
        }
        if (freeIt > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

}
