#include "REL/m536Dll.h"
#include <stdarg.h>
#include <stdio.h>
extern "C" {
#include "game/wipe.h"
#include "game/gamemes.h"
}

// m536 top-level unit: runtime support (allocator, assert, list helpers, a look-at
// camera helper), the module main object and the Hu3D wrapper functions.
extern "C" {

void OSPanic(const char *file, int line, const char *msg, ...);
void ObjectSetup(void);
OMOBJMAN *MgActorObjectSetup(void);

extern void (*_ctors[])(void);
extern void (*_dtors[])(void);

// Raised assertion text shared by every inlined singleton check.
#define M536_ASSERT_FAIL(cond, file, line)                                           \
    do {                                                                             \
        char buf[1024];                                                              \
        sprintf(buf, "\nASSERTION FAILED at %s:%d\n%s", file, line, cond);           \
        OSPanic("assert.cpp", 0x12, buf);                                            \
    } while (0)

// A work object as seen by the list at lbl_1_bss_C: node, vtable, flag word.
struct M536Base {
    M536_NODE m_node;   /* 0x00 */
    void **m_vptr;      /* 0x08 */
    s32 m_flag;         /* 0x0C */
};

// Look-at camera: position, up vector and target.
struct M536CamLook {
    Vec m_pos;      /* 0x00 */
    Vec m_up;       /* 0x0C */
    Vec m_target;   /* 0x18 */
};

// Camera as position plus orientation quaternion.
struct M536CamQuat {
    Vec m_pos;          /* 0x00 */
    Quaternion m_rot;   /* 0x0C */
};

// The module main object (singleton at lbl_1_bss_9EC).
struct M536Main {
    s32 m_unk0;         /* 0x00 */
    void **m_vptr;      /* 0x04 */
    s32 m_player[4];    /* 0x08 */
    s32 m_msg;          /* 0x18 */
    s32 m_timer;        /* 0x1C */
};

// Intrusive singly linked list node used by the merge sort.
struct M536Link {
    M536Link *m_next;
};

typedef u8 (*M536Compare)(M536Link *, M536Link *);

M536Base *fn_1_1EA8(M536Base *self, s16 freeIt);
void fn_1_1DDC(void);
M536Main *fn_1_20CC(M536Main *self, s16 freeIt);
void fn_1_246C(void);
void fn_1_2C18(void);
void fn_1_48C0(void);
void fn_1_4934(void);
void fn_1_52E4(void);
void fn_1_2370(OMOBJ *obj);
void fn_1_2400(M536Main *self);
void fn_1_2470(void);
void fn_1_56C(M536CamLook *cam, Vec *delta);
void fn_1_408(M536CamLook *cam, Vec *right, Vec *up, Vec *fwd);
void fn_1_A20(M536CamLook *cam);
void fn_1_47E4(void *ptr);
M536Link *fn_1_14A0(M536Compare cmp, M536Link *head);
M536Link *fn_1_1B48(M536Link *head);
M536Link *fn_1_1B9C(M536Compare cmp, M536Link *a, M536Link *b);

void *lbl_1_data_1C4[4] = { 0, 0, (void *)fn_1_52E4, 0 };
void *lbl_1_data_1D4[4] = { 0, 0, (void *)fn_1_4934, (void *)fn_1_48C0 };
void *lbl_1_data_1E4[6] = { 0, 0, (void *)fn_1_20CC, 0, (void *)fn_1_246C, 0 };
void *lbl_1_data_1FC[5] = { 0, 0, (void *)fn_1_1EA8, 0, (void *)fn_1_1DDC };

u8 lbl_1_bss_0[12];
M536_NODE lbl_1_bss_C;
s32 lbl_1_bss_14;
s32 lbl_1_bss_18;
s32 lbl_1_bss_1C;
s32 lbl_1_bss_20;
u32 lbl_1_bss_24[0x271];
u8 lbl_1_bss_9E8;
M536Main *lbl_1_bss_9EC;

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
void fn_1_2D8(M536CamLook *cam, f32 yaw, f32 pitch)
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

void fn_1_39C(M536CamLook *cam, f32 roll)
{
    Mtx rot;
    Vec axis;

    PSVECSubtract(&cam->m_target, &cam->m_pos, &axis);
    PSMTXRotAxisRad(rot, &axis, 0.017453292f * roll);
    PSMTXMultVec(rot, &cam->m_up, &cam->m_up);
}

// Camera basis (the three rows of the look-at matrix).
void fn_1_408(M536CamLook *cam, Vec *right, Vec *up, Vec *fwd)
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

void fn_1_4B0(M536CamLook *cam, Mtx out)
{
    C_MTXLookAt(out, &cam->m_pos, &cam->m_up, &cam->m_target);
    PSMTXInverse(out, out);
}

// Move the camera by a delta given in camera space.
void fn_1_504(M536CamLook *cam, Vec *delta)
{
    Mtx m;
    Vec world;

    C_MTXLookAt(m, &cam->m_pos, &cam->m_up, &cam->m_target);
    PSMTXInverse(m, m);
    PSMTXMultVecSR(m, delta, &world);
    fn_1_56C(cam, &world);
}

void fn_1_56C(M536CamLook *cam, Vec *delta)
{
    PSVECAdd(delta, &cam->m_target, &cam->m_target);
    PSVECAdd(delta, &cam->m_pos, &cam->m_pos);
}

void fn_1_5C0(M536CamLook *cam)
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

void fn_1_654(M536CamLook *cam, M536CamQuat *src)
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

void fn_1_6F4(M536CamLook *cam, M536CamQuat *dst)
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
void fn_1_76C(M536CamLook *out, M536CamQuat *a, M536CamQuat *b, f32 t)
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
void fn_1_87C(M536CamLook *cam, Vec *pos)
{
    Vec delta;

    PSVECSubtract(pos, &cam->m_pos, &delta);
    cam->m_pos.x = pos->x;
    cam->m_pos.y = pos->y;
    cam->m_pos.z = pos->z;
    PSVECAdd(&cam->m_target, &delta, &cam->m_target);
}

void fn_1_8E8(M536CamLook *cam, Vec *a, Vec *b, f32 t)
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

void fn_1_9A0(M536CamLook *cam, u16 cameraBit)
{
    Hu3DCameraPosSetV(cameraBit, &cam->m_pos, &cam->m_up, &cam->m_target);
}

void fn_1_9E0(M536CamLook *cam, u16 cameraBit)
{
    Hu3DCameraPosGet(cameraBit, &cam->m_pos, &cam->m_up, &cam->m_target);
}

// Prints the camera as a position plus quaternion (raw float bits).
void fn_1_A20(M536CamLook *cam)
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
    M536CamLook *cam = (M536CamLook *)camVec;
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
    M536_NODE *n = (M536_NODE *)node;

    n->next = n;
    n->prev = n;
}

void *fn_1_1270(void *node, s32 freeIt)
{
    M536_NODE *n = (M536_NODE *)node;

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
    M536_NODE *n = (M536_NODE *)node;

    n->next->prev = n->prev;
    n->prev->next = n->next;
    n->next = n;
    n->prev = n;
}

// Insert `node` after `at`.
void fn_1_1300(M536_NODE *node, M536_NODE *at)
{
    M536_NODE *anchor = at;

    node->prev = anchor;
    node->next = anchor->next;
    anchor->next->prev = node;
    anchor->next = node;
}

// Insert `node` before `at`.
void fn_1_1330(void *node, void *at)
{
    M536_NODE *n = (M536_NODE *)node;
    M536_NODE *a = (M536_NODE *)at;
    M536_NODE *anchor = a;

    n->next = a;
    n->prev = a->prev;
    a->prev->next = n;
    a->prev = n;
}

// Splice the contents of list `src` in front of `dst`.
void fn_1_1360(M536_NODE *src, M536_NODE *dst)
{
    M536_NODE *first;
    M536_NODE *last;
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
void fn_1_13E8(M536_NODE *list, M536Compare cmp)
{
    M536Link *sorted;
    M536_NODE *prev;
    u8 empty;

    empty = 0;
    if (list->next == list && list->next == list->prev) {
        empty = 1;
    }
    if (empty == 0) {
        list->prev->next = 0;
        sorted = fn_1_14A0(cmp, (M536Link *)list->next);
        list->next = (M536_NODE *)sorted;
        prev = list;
        while (sorted != 0) {
            ((M536_NODE *)sorted)->prev = prev;
            prev = (M536_NODE *)sorted;
            sorted = sorted->m_next;
        }
        list->prev = prev;
        prev->next = list;
    }
}

// One merge-sort level, fully expanded three deep in the original; the lowest
// level calls fn_1_14A0 itself.
#define M536_SORT_L3(cmp, in, out)                                                  \
    do {                                                                            \
        M536Link *t3;                                                               \
        M536Link *u3;                                                               \
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

#define M536_SORT_L2(cmp, in, out)                                                  \
    do {                                                                            \
        M536Link *t2;                                                               \
        M536Link *u2;                                                               \
        M536Link *l2;                                                               \
        M536Link *r2;                                                               \
        t2 = (in);                                                                  \
        if (t2 == 0) {                                                              \
            (out) = 0;                                                              \
        } else if (t2->m_next == 0) {                                               \
            (out) = t2;                                                             \
        } else {                                                                    \
            u2 = fn_1_1B48(t2);                                                     \
            M536_SORT_L3(cmp, t2, l2);                                              \
            M536_SORT_L3(cmp, u2, r2);                                              \
            (out) = fn_1_1B9C(cmp, l2, r2);                                         \
        }                                                                           \
    } while (0)

#define M536_SORT_L1(cmp, in, out)                                                  \
    do {                                                                            \
        M536Link *t1;                                                               \
        M536Link *u1;                                                               \
        M536Link *l1;                                                               \
        M536Link *r1;                                                               \
        t1 = (in);                                                                  \
        if (t1 == 0) {                                                              \
            (out) = 0;                                                              \
        } else if (t1->m_next == 0) {                                               \
            (out) = t1;                                                             \
        } else {                                                                    \
            u1 = fn_1_1B48(t1);                                                     \
            M536_SORT_L2(cmp, t1, l1);                                              \
            M536_SORT_L2(cmp, u1, r1);                                              \
            (out) = fn_1_1B9C(cmp, l1, r1);                                         \
        }                                                                           \
    } while (0)

M536Link *fn_1_14A0(M536Compare cmp, M536Link *head)
{
    M536Link *half;
    M536Link *left;
    M536Link *right;
    M536Link *res;

    if (head == 0) {
        return 0;
    }
    if (head->m_next == 0) {
        return head;
    }
    half = fn_1_1B48(head);
    M536_SORT_L1(cmp, head, left);
    M536_SORT_L1(cmp, half, right);
    res = fn_1_1B9C(cmp, left, right);
    return res;
}

// Cut the list in the middle and return the second half.
M536Link *fn_1_1B48(M536Link *head)
{
    M536Link *slow;
    M536Link *fast;
    M536Link *second;

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
M536Link *fn_1_1B9C(M536Compare cmp, M536Link *a, M536Link *b)
{
    M536Link dummy;
    M536Link *tail;

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
    M536Base *obj;
    M536Base *p0;
    M536Base *p1;
    M536Base *end;
    M536Base *nx;

    p0 = (M536Base *)lbl_1_bss_C.next;
    p1 = p0;
    obj = p1;
    end = (M536Base *)&lbl_1_bss_C;
    lbl_1_bss_14 = bit;
    while (obj != end) {
        obj->m_flag = obj->m_flag | (1 << bit);
        ((void (*)(M536Base *, s32))obj->m_vptr[3])(obj, bit);
        nx = (M536Base *)obj->m_node.next;
        obj = nx;
    }
}

s32 fn_1_1CE0(void)
{
    M536Base *obj;
    M536Base *p0;
    M536Base *p1;
    M536Base *end;
    M536Base *nx;

    p0 = (M536Base *)lbl_1_bss_C.next;
    p1 = p0;
    obj = p1;
    end = (M536Base *)&lbl_1_bss_C;
    while (obj != end) {
        if ((obj->m_flag & (1 << lbl_1_bss_14)) != 0) {
            return 0;
        }
        nx = (M536Base *)obj->m_node.next;
        obj = nx;
    }
    return 1;
}

void fn_1_1D6C(void)
{
    M536Base *obj;
    M536Base *p0;
    M536Base *p1;
    M536Base *end;
    M536Base *nx;

    p0 = (M536Base *)lbl_1_bss_C.next;
    p1 = p0;
    obj = p1;
    end = (M536Base *)&lbl_1_bss_C;
    while (obj != end) {
        ((void (*)(M536Base *))obj->m_vptr[4])(obj);
        nx = (M536Base *)obj->m_node.next;
        obj = nx;
    }
}

void fn_1_1DDC(void)
{
}

void fn_1_1DE0(void)
{
    u8 empty;
    M536Base *last;

    for (;;) {
        empty = 0;
        if (lbl_1_bss_C.next == &lbl_1_bss_C && lbl_1_bss_C.next == lbl_1_bss_C.prev) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        last = (M536Base *)lbl_1_bss_C.prev;
        if (last != 0) {
            ((void (*)(M536Base *, s32))last->m_vptr[2])(last, 1);
        }
    }
}

M536Base *fn_1_1EA8(M536Base *self, s16 freeIt)
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

M536Main *fn_1_1F40(M536Main *self)
{
    s32 i;

    if (lbl_1_bss_9EC == 0) {
    } else {
        M536_ASSERT_FAIL("ms_pointer == 0", "singleton.h", 0x35);
    }
    lbl_1_bss_9EC = self;
    self->m_vptr = lbl_1_data_1E4;
    self->m_timer = 0;
    for (i = 0; i < 4; i++) {
        self->m_player[i] = -1;
    }
    return self;
}

M536Main *fn_1_2010(M536Main *self, s16 freeIt)
{
    if (self != 0) {
        if (lbl_1_bss_9EC != 0) {
        } else {
            M536_ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_9EC = 0;
        if (freeIt > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

M536Main *fn_1_20CC(M536Main *self, s16 freeIt)
{
    M536Main *ms;
    s32 i;
    s32 player;

    if (self != 0) {
        self->m_vptr = lbl_1_data_1E4;
        if (mgDataTbl[GwSystem.mgNo].type != 3) {
            if (lbl_1_bss_9EC != 0) {
            } else {
                M536_ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x27);
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
                M536_ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x3D);
            }
            lbl_1_bss_9EC = 0;
        }
        if (freeIt > 0) {
            fn_1_47E4(self);
        }
    }
    return self;
}

void fn_1_2284(M536Main *self, s32 objMax, s32 prio)
{
    OMOBJMAN *objman;

    objman = omInitObjMan(objMax, prio);
    omGameSysInit(objman);
    ((void (*)(M536Main *, OMOBJMAN *))self->m_vptr[3])(self, objman);
    omAddObjEx(objman, 0, 0, 0, -1, fn_1_2370);
}

void fn_1_2308(M536Main *self)
{
    OMOBJMAN *objman;

    objman = MgActorObjectSetup();
    ((void (*)(M536Main *, OMOBJMAN *))self->m_vptr[3])(self, objman);
    omAddObjEx(objman, 0, 0, 0, -1, fn_1_2370);
}

void fn_1_2370(OMOBJ *obj)
{
    M536Main *ms0;
    M536Main *ms1;

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

void fn_1_2400(M536Main *self)
{
    ((void (*)(M536Main *))self->m_vptr[4])(self);
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
    M536Main *ms;
    M536Main *ms0;

    ms0 = lbl_1_bss_9EC;
    ms = ms0;
    if (WipeCheckEnd() != 0) {
        ((void (*)(M536Main *))ms->m_vptr[4])(ms);
        if (ms->m_timer > 0) {
            if (--ms->m_timer == 0) {
            }
        }
        return;
    }
    ((void (*)(M536Main *))ms->m_vptr[5])(ms);
    if (ms != 0) {
        ((void (*)(M536Main *, s32))ms->m_vptr[2])(ms, 1);
    }
    CharModelKill(-1);
    omOvlReturnEx(1, 1);
}

}
