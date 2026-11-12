#include "REL/m501Dll.h"
#include "datanum/m501.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/charman.h"
#include "game/wipe.h"
#include "game/gamemes.h"
#include "game/mgdata.h"
#include "game/gamework.h"
#include "game/audio.h"
#include "game/pad.h"
#include "game/flag.h"
#include "game/process.h"
#include <stdarg.h>

extern "C" {
#include "game/mg/timer.h"
}

extern "C" {
void OSPanic(const char *file, int line, const char *msg, ...);
int sprintf(char *buf, const char *fmt, ...);
int printf(const char *fmt, ...);
int vprintf(const char *fmt, va_list args);
void OSInitSemaphore(OSSemaphore *sem, s32 count);
s32 OSWaitSemaphore(OSSemaphore *sem);
s32 OSSignalSemaphore(OSSemaphore *sem);
void ObjectSetup(void);
OMOBJMAN *MgActorObjectSetup(void);
extern void (*_ctors[])(void);
extern void (*_dtors[])(void);
}

#ifdef __MWERKS__
#define M501_RODATA __declspec(section ".rodata")
#else
#define M501_RODATA
#endif

#define M501_ASSERT(c_, t_, f_, l_) \
    if (c_) { \
    } else { \
        __assert_fail(t_, f_, l_); \
    }

void *operator new(unsigned long size)
{
    void *ptr = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);
    if (ptr != 0) {
    } else {
        __assert_fail("ptr != NULL", "new.cpp", 0x17);
    }
    return ptr;
}

void *operator new[](unsigned long size)
{
    void *ptr = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);
    if (ptr != 0) {
    } else {
        __assert_fail("ptr != NULL", "new.cpp", 0x17);
    }
    return ptr;
}

void operator delete(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

void operator delete[](void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

extern "C" void _prolog(void)
{
    void (**ctor)(void) = _ctors;
    while (*ctor != 0) {
        (*ctor)();
        ctor++;
    }
    ObjectSetup();
}

extern "C" void _epilog(void)
{
    void (**dtor)(void) = _dtors;
    while (*dtor != 0) {
        (*dtor)();
        dtor++;
    }
}

extern "C" void __assert_fail(const char *expr, const char *file, int line)
{
    char buf[1024];
    sprintf(buf, "\nASSERTION FAILED at %s:%d\n%s", file, line, expr);
    OSPanic("assert.cpp", 0x12, buf);
}

#ifndef TARGET_PC /* the host C library already provides fprintf */
extern "C" int fprintf(void *file, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    return vprintf(fmt, args);
}
#endif

void CCamera::rotateCamera(f32 yaw, f32 pitch)
{
    Mtx rotA;
    Mtx rotB;
    Mtx rot;
    Vec right;
    Vec up;
    Vec fwd;

    getLookAtVec(&right, &up, &fwd);
    PSMTXRotAxisRad(rotA, &right, 0.017453292f * yaw);
    PSMTXRotRad(rotB, 'Y', 0.017453292f * pitch);
    PSMTXConcat(rotA, rotB, rot);
    PSMTXMultVecSR(rot, &fwd, &fwd);
    PSMTXMultVecSR(rot, &up, &m_up);
    PSVECSubtract(&m_pos, &fwd, &m_target);
}

void CCamera::rotateCameraUp(f32 angle)
{
    Mtx rot;
    Vec dir;

    PSVECSubtract(&m_target, &m_pos, &dir);
    PSMTXRotAxisRad(rot, &dir, 0.017453292f * angle);
    PSMTXMultVec(rot, &m_up, &m_up);
}

void CCamera::getLookAtVec(Vec *a, Vec *b, Vec *c)
{
    Mtx mtx;

    C_MTXLookAt(mtx, &m_pos, &m_up, &m_target);
    a->x = mtx[0][0];
    a->y = mtx[0][1];
    a->z = mtx[0][2];
    b->x = mtx[1][0];
    b->y = mtx[1][1];
    b->z = mtx[1][2];
    c->x = mtx[2][0];
    c->y = mtx[2][1];
    c->z = mtx[2][2];
}

void CCamera::getLookAtMtxInverse(Mtx *mtx)
{
    C_MTXLookAt(*mtx, &m_pos, &m_up, &m_target);
    PSMTXInverse(*mtx, *mtx);
}

void CCamera::applyOffsetLocal(Vec *offset)
{
    Mtx mtx;
    Vec world;

    C_MTXLookAt(mtx, &m_pos, &m_up, &m_target);
    PSMTXInverse(mtx, mtx);
    PSMTXMultVecSR(mtx, offset, &world);
    applyOffset(&world);
}

void CCamera::applyOffset(Vec *offset)
{
    PSVECAdd(offset, &m_target, &m_target);
    PSVECAdd(offset, &m_pos, &m_pos);
}

void CCamera::resetLookAt()
{
    m_up.x = 0.0f;
    m_up.y = 1.0f;
    m_up.z = 0.0f;
    m_pos.x = 0.0f;
    m_pos.y = 0.0f;
    m_pos.z = 5000.0f;
    m_target.x = 0.0f;
    m_target.y = 0.0f;
    m_target.z = 0.0f;
}

void CCamera::setView(CCameraView *view)
{
    Mtx mtx;

    PSMTXQuat(mtx, &view->m_rot);
    m_up.x = mtx[0][1];
    m_up.y = mtx[1][1];
    m_up.z = mtx[2][1];
    m_pos.x = view->m_pos.x;
    m_pos.y = view->m_pos.y;
    m_pos.z = view->m_pos.z;
    m_target.x = view->m_pos.x - mtx[0][2];
    m_target.y = view->m_pos.y - mtx[1][2];
    m_target.z = view->m_pos.z - mtx[2][2];
}

void CCamera::getView(CCameraView *view)
{
    Mtx mtx;

    C_MTXLookAt(mtx, &m_pos, &m_up, &m_target);
    PSMTXInverse(mtx, mtx);
    view->m_pos.x = m_pos.x;
    view->m_pos.y = m_pos.y;
    view->m_pos.z = m_pos.z;
    C_QUATMtx(&view->m_rot, mtx);
}

void CCamera::lerpView(CCameraView *a, CCameraView *b, f32 t)
{
    Mtx mtx;
    Quaternion q;
    Vec pos;
    Vec scaledB;
    Vec scaledA;

    C_QUATSlerp(&a->m_rot, &b->m_rot, &q, t);
    PSVECScale(&a->m_pos, &scaledA, 1.0f - t);
    PSVECScale(&b->m_pos, &scaledB, t);
    PSVECAdd(&scaledA, &scaledB, &pos);
    PSMTXQuat(mtx, &q);
    m_up.x = mtx[0][1];
    m_up.y = mtx[1][1];
    m_up.z = mtx[2][1];
    m_pos.x = pos.x;
    m_pos.y = pos.y;
    m_pos.z = pos.z;
    m_target.x = pos.x - mtx[0][2];
    m_target.y = pos.y - mtx[1][2];
    m_target.z = pos.z - mtx[2][2];
}

void CCamera::getOffset(Vec *out)
{
    Vec delta;

    PSVECSubtract(out, &m_pos, &delta);
    m_pos.x = out->x;
    m_pos.y = out->y;
    m_pos.z = out->z;
    PSVECAdd(&m_target, &delta, &m_target);
}

extern "C" void getOffsetTo__7CCameraFP3VecP3Vec(CCamera *self, Vec *a, Vec *b, f32 t)
{
    Vec delta;
    Vec scaledB;
    Vec scaledA;

    PSVECScale(a, &scaledA, 1.0f - t);
    PSVECScale(b, &scaledB, t);
    PSVECAdd(&scaledA, &scaledB, &scaledB);
    PSVECSubtract(&scaledB, &self->m_pos, &delta);
    self->m_pos.x = scaledB.x;
    self->m_pos.y = scaledB.y;
    self->m_pos.z = scaledB.z;
    PSVECAdd(&self->m_target, &delta, &self->m_target);
}

void CCamera::setPos(u16 camNo)
{
    Hu3DCameraPosSetV(camNo, &m_pos, &m_up, &m_target);
}

void CCamera::getPos(u16 camNo)
{
    Hu3DCameraPosGet(camNo, &m_pos, &m_up, &m_target);
}

static inline u32 f32Bits(f32 v)
{
    union {
        f32 f;
        u32 u;
    } c;
    c.f = v;
    return c.u;
}

void CCamera::printView()
{
    Mtx mtx;
    Vec pos;
    Quaternion q;

    C_MTXLookAt(mtx, &m_pos, &m_up, &m_target);
    PSMTXInverse(mtx, mtx);
    pos.x = m_pos.x;
    pos.y = m_pos.y;
    pos.z = m_pos.z;
    C_QUATMtx(&q, mtx);
    printf("{{0x%08x,0x%08x,0x%08x},{0x%08x,0x%08x,0x%08x,0x%08x}},\n", f32Bits(pos.x), f32Bits(pos.y), f32Bits(pos.z), f32Bits(q.x), f32Bits(q.y), f32Bits(q.z), f32Bits(q.w));
}

f32 CCamera::GetStickX(int pad)
{
    return 0.017857144f * (f32)HuPadStkX[pad];
}

f32 CCamera::GetStickY(int pad)
{
    return 0.017857144f * (f32)HuPadStkY[pad];
}

f32 CCamera::GetSubStickX(int pad)
{
    return 0.016949153f * (f32)HuPadSubStkX[pad];
}

f32 CCamera::GetSubStickY(int pad)
{
    return 0.016949153f * (f32)HuPadSubStkY[pad];
}

void CCamera::debugView(int pad)
{
    Mtx mtx;
    Mtx rotA;
    Mtx rotB;
    Mtx rot;
    Mtx inv;
    Vec right;
    Vec up;
    Vec fwd;
    Vec move;
    Vec fwdMove;
    Quaternion q;
    f32 speed;
    f32 yaw;
    f32 pitch;
    u16 btn;

    btn = HuPadBtn[pad];
    if ((btn & 0x800) != 0) {
        m_up.x = 0.0f;
        m_up.y = 1.0f;
        m_up.z = 0.0f;
    }
    yaw = -4.0f * (0.017857144f * (f32)HuPadStkX[pad]);
    pitch = -4.0f * (0.017857144f * (f32)HuPadStkY[pad]);
    C_MTXLookAt(mtx, &m_pos, &m_up, &m_target);
    right.x = mtx[0][0];
    right.y = mtx[0][1];
    right.z = mtx[0][2];
    up.x = mtx[1][0];
    up.y = mtx[1][1];
    up.z = mtx[1][2];
    fwd.x = mtx[2][0];
    fwd.y = mtx[2][1];
    fwd.z = mtx[2][2];
    PSMTXRotAxisRad(rotA, &right, 0.017453292f * pitch);
    PSMTXRotRad(rotB, 'Y', 0.017453292f * yaw);
    PSMTXConcat(rotA, rotB, rot);
    PSMTXMultVecSR(rot, &fwd, &fwd);
    PSMTXMultVecSR(rot, &up, &m_up);
    PSVECSubtract(&m_pos, &fwd, &m_target);
    if ((u8)((btn & 0x200) != 0) != 0) {
        speed = 70.0f;
    } else {
        speed = 20.0f;
    }
    C_MTXLookAt(inv, &m_pos, &m_up, &m_target);
    PSMTXInverse(inv, inv);
    move.x = (speed * (f32)(u8)((btn & 2) != 0)) - (speed * (f32)(u8)((btn & 1) != 0));
    move.y = (speed * (f32)(u8)((btn & 8) != 0)) - (speed * (f32)(u8)((btn & 4) != 0));
    move.x = move.x + (speed * (0.016949153f * (f32)HuPadSubStkX[pad]));
    move.z = 0.0f;
    PSMTXMultVecSR(inv, &move, &move);
    move.y = move.y + (speed * (0.016949153f * (f32)HuPadSubStkY[pad]));
    PSVECAdd(&move, &m_target, &m_target);
    PSVECAdd(&move, &m_pos, &m_pos);
    fwdMove.x = 0.0f;
    fwdMove.y = 0.0f;
    fwdMove.z = (f32)((u8)((btn & 0x400) != 0) - (u8)((btn & 0x100) != 0));
    PSMTXMultVecSR(inv, &fwdMove, &fwdMove);
    fwdMove.y = 0.0f;
    f32 mag = PSVECMag(&fwdMove);
    if (mag > 1e-10f) {
        PSVECScale(&fwdMove, &fwdMove, speed / mag);
        PSVECAdd(&fwdMove, &m_target, &m_target);
        PSVECAdd(&fwdMove, &m_pos, &m_pos);
    }
    if ((HuPadBtnDown[pad] & 0x200) != 0) {
        Mtx dump;
        Vec pos;

        C_MTXLookAt(dump, &m_pos, &m_up, &m_target);
        PSMTXInverse(dump, dump);
        pos.x = m_pos.x;
        pos.y = m_pos.y;
        pos.z = m_pos.z;
        C_QUATMtx(&q, dump);
        printf("{{0x%08x,0x%08x,0x%08x},{0x%08x,0x%08x,0x%08x,0x%08x}},\n", f32Bits(pos.x), f32Bits(pos.y), f32Bits(pos.z), f32Bits(q.x), f32Bits(q.y), f32Bits(q.z), f32Bits(q.w));
    }
}

CHandleMan::CHandleMan(unsigned int n)
{
    if (n <= 0x1000) {
    } else {
        __assert_fail("n <= MAXCOUNT", "handle.cpp", 0x44);
    }
    m_count = n;
    CHandleNode *nodes = (CHandleNode *)operator new((n + 1) * sizeof(CHandleNode));
    m_nodes = nodes;
    m_used = &m_nodes[n];
    m_used->m_next = m_used;
    m_used->m_prev = m_used;
    for (u32 i = 1; i < n; i++) {
        m_nodes[i].m_next = &m_nodes[i + 1];
        m_nodes[i].m_prev = &m_nodes[i - 1];
        m_nodes[i].m_ptr = 0;
        m_nodes[i].m_index = i;
        m_nodes[i].m_gen = 0;
    }
    m_nodes[1].m_prev = &m_nodes[0];
    m_nodes[n - 1].m_next = &m_nodes[0];
    m_nodes[0].m_next = &m_nodes[1];
    m_nodes[0].m_prev = &m_nodes[n - 1];
}

CHandleMan::~CHandleMan()
{
    if (m_nodes != 0) {
        HuMemDirectFree(m_nodes);
    }
}

int CHandleMan::insertHandle(void *ptr)
{
    CHandleNode *node = m_nodes[0].m_next;
    if (node == &m_nodes[0]) {
        return 0;
    }
    node->m_next->m_prev = node->m_prev;
    node->m_prev->m_next = node->m_next;
    node->m_next = m_used->m_next;
    node->m_prev = m_used;
    m_used->m_next->m_prev = node;
    m_used->m_next = node;
    node->m_ptr = ptr;
    node->m_gen = (node->m_gen + 1) & 0xFFF;
    return (node->m_index << 12) | node->m_gen;
}

void *CHandleMan::removeHandle(int handle)
{
    u32 h = handle;
    if (h == 0) {
        return 0;
    }
    if ((h >> 12) >= m_count) {
        return 0;
    }
    CHandleNode *node = &m_nodes[h >> 12];
    if (((node->m_index << 12) | node->m_gen) != h) {
        return 0;
    }
    void *ptr = node->m_ptr;
    node->m_ptr = 0;
    node->m_next->m_prev = node->m_prev;
    node->m_prev->m_next = node->m_next;
    node->m_next = m_nodes;
    node->m_prev = m_nodes->m_prev;
    m_nodes->m_prev->m_next = node;
    m_nodes->m_prev = node;
    return ptr;
}

void *CHandleMan::getHandlePtr(int handle)
{
    u32 h = handle;
    if (h == 0) {
        return 0;
    }
    if ((h >> 12) >= m_count) {
        return 0;
    }
    CHandleNode *node = &m_nodes[h >> 12];
    if (((node->m_index << 12) | node->m_gen) != h) {
        return 0;
    }
    return node->m_ptr;
}

int CHandleMan::getHandleNum()
{
    int n = 0;
    CHandleNode *self = (CHandleNode *)this;
    CHandleNode *node = self->m_next;
    while (node != self) {
        node = node->m_next;
        n++;
    }
    return n;
}

int CHandleMan::getEndHandleNum()
{
    CHandleNode *head = m_used;
    int n = 0;
    CHandleNode *node = head->m_next;
    while (node != head) {
        node = node->m_next;
        n++;
    }
    return n;
}

int CHandleMan::getBeginHandleNum()
{
    CHandleNode *head = m_nodes;
    int n = 0;
    CHandleNode *node = head->m_next;
    while (node != head) {
        node = node->m_next;
        n++;
    }
    return n;
}

CLinkNode::CLinkNode()
{
    m_prev = this;
    m_next = this;
}

CLinkNode::~CLinkNode()
{
    m_prev->m_next = m_next;
    m_next->m_prev = m_prev;
}

CLinkNode::CLinkNode(CLinkNode &)
{
    m_prev->m_next = m_next;
    m_next->m_prev = m_prev;
    m_prev = this;
    m_next = this;
}

void CLinkNode::insertFront(CLinkList *list)
{
    CLinkList *l = list;
    m_next = l;
    m_prev = l->m_prev;
    l->m_prev->m_next = this;
    l->m_prev = this;
}

void CLinkNode::insertBack(CLinkList *list)
{
    CLinkList *l = list;
    m_prev = l;
    m_next = l->m_next;
    l->m_next->m_prev = this;
    l->m_next = this;
}

void CLinkNode::copyLink(CLinkList *list)
{
    if (!((CLinkList *)this)->isEmpty()) {
        CLinkNode *first = m_prev;
        CLinkNode *last = m_next;
        m_prev = this;
        m_next = this;
        first->m_next = list->m_next;
        last->m_prev = list;
        list->m_next->m_prev = first;
        list->m_next = last;
    }
}

/* three file-scope delays the original sets up in its static initialiser */
class CInt {
public:
    CInt(int v) : m_value(v) {}
    s32 m_value;
};

static CInt s_delay18(120);
static CInt s_winnerDelay(210);
static CInt s_delay20(210);
static u8 s_objectListChain[12];

/* singly linked merge sort over the forward (offset 0) links */
extern "C" CLinkNode *fn_1_19A4(bool (*cmp)(CLinkNode *, CLinkNode *), CLinkNode *list);
extern "C" CLinkNode *fn_1_204C(CLinkNode *list);
extern "C" CLinkNode *fn_1_20A0(bool (*cmp)(CLinkNode *, CLinkNode *), CLinkNode *a, CLinkNode *b);

extern "C" void sortList__9CLinkListFPFP9CLinkNodeP9CLinkNode_v(CLinkList *list, bool (*cmp)(CLinkNode *, CLinkNode *))
{
    if (!list->isEmpty()) {
        list->m_next->m_prev = 0;
        CLinkNode *node = fn_1_19A4(cmp, list->m_prev);
        list->m_prev = node;
        CLinkNode *prev = list;
        while (node != 0) {
            node->m_next = prev;
            prev = node;
            node = node->m_prev;
        }
        list->m_next = prev;
        prev->m_prev = list;
    }
}

extern "C" CLinkNode *fn_1_19A4(bool (*cmp)(CLinkNode *, CLinkNode *), CLinkNode *list)
{
    if (list == 0) {
        return 0;
    }
    if (list->m_prev == 0) {
        return list;
    }
    CLinkNode *half = fn_1_204C(list);
    CLinkNode *a = fn_1_19A4(cmp, list);
    CLinkNode *b = fn_1_19A4(cmp, half);
    return fn_1_20A0(cmp, a, b);
}

/* splits the list in two and returns the second half */
extern "C" CLinkNode *fn_1_204C(CLinkNode *list)
{
    CLinkNode *slow = list;
    CLinkNode *fast = list->m_prev;
    if (fast != 0) {
        fast = fast->m_prev;
    }
    while (fast != 0) {
        slow = slow->m_prev;
        fast = fast->m_prev;
        if (fast != 0) {
            fast = fast->m_prev;
        }
    }
    CLinkNode *second = slow->m_prev;
    slow->m_prev = 0;
    return second;
}

extern "C" CLinkNode *fn_1_20A0(bool (*cmp)(CLinkNode *, CLinkNode *), CLinkNode *a, CLinkNode *b)
{
    CLinkNode *head;
    CLinkNode **tail = &head;
    while (a != 0 && b != 0) {
        if (cmp(a, b)) {
            *tail = a;
            tail = &a->m_prev;
            a = a->m_prev;
        } else {
            *tail = b;
            tail = &b->m_prev;
            b = b->m_prev;
        }
    }
    if (a == 0) {
        *tail = b;
    } else {
        *tail = a;
    }
    return head;
}

CLinkList CGameObject::ObjectList;
s32 CGameObject::s_updateMode;
CMinigameCore *CMinigameCore::instance;

void CGameObject::SetObjExecMode(int mode)
{
    CGameObject *obj;

    s_updateMode = mode;
    for (obj = (CGameObject *)ObjectList.m_prev; obj != (CGameObject *)&ObjectList; obj = (CGameObject *)obj->m_prev) {
        obj->m_execMode |= (1 << mode);
        obj->on_new_mode(mode);
    }
}

s32 CGameObject::CheckObjExec()
{
    CLinkList *list = &ObjectList;
    CGameObject *first = (CGameObject *)list->m_prev;
    CGameObject *obj = first;
    while (obj != (CGameObject *)list) {
        int mode = s_updateMode;
        if ((obj->m_execMode & (1 << mode)) != 0) {
            return 0;
        }
        CGameObject *next = (CGameObject *)obj->m_prev;
        obj = next;
    }
    return 1;
}

void CGameObject::UpdateAll()
{
    CLinkList *list = &ObjectList;
    CGameObject *first = (CGameObject *)list->m_prev;
    CGameObject *obj = first;
    while (obj != (CGameObject *)list) {
        obj->on_update();
        CGameObject *next = (CGameObject *)obj->m_prev;
        obj = next;
    }
}

void CGameObject::on_update()
{
}

void CGameObject::Close()
{
    while (!ObjectList.isEmpty()) {
        CGameObject *obj = (CGameObject *)ObjectList.m_next;
        if (obj != 0) {
            delete obj;
        }
    }
}

CGameObject::~CGameObject()
{
}

s32 CGameObject::GetUpdateMode()
{
    return s_updateMode;
}

CMinigameCore::CMinigameCore()
{
    M501_ASSERT(instance == 0, "ms_pointer == 0", "singleton.h", 0x35);
    instance = this;
    m_delay = 0;
    for (int i = 0; i < 4; i++) {
        m_winner[i] = -1;
    }
}

/* out-of-line destructor of the singleton base, with the delete flag */
extern "C" void *fn_1_2514(void *self, s16 flag)
{
    if (self != 0) {
        M501_ASSERT(CMinigameCore::instance != 0, "ms_pointer != 0", "singleton.h", 0x3D);
        CMinigameCore::instance = 0;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

CMinigameCore::~CMinigameCore()
{
    if (mgDataTbl[GwSystem.mgNo].type != 3) {
        M501_ASSERT(instance != 0, "ms_pointer != 0", "singleton.h", 0x27);
        CMinigameCore *core = instance;
        for (int i = 0; i < 4; i++) {
            s32 player = core->m_winner[i];
            if (player >= 0 && _CheckFlag(0x1000F) == 0) {
                GwPlayer[player].mgCoinBonus = 10;
            }
        }
    }
    M501_ASSERT(instance != 0, "ms_pointer != 0", "singleton.h", 0x3D);
    instance = 0;
}

void CMinigameCore::setup(int prio, int stackSize)
{
    OMOBJMAN *objMan = omInitObjMan(prio, stackSize);
    omGameSysInit(objMan);
    on_setup(objMan);
    omAddObjEx(objMan, 0, 0, 0, -1, ObjUpdate);
}

void CMinigameCore::actorSetup(int, int)
{
    OMOBJMAN *objMan = MgActorObjectSetup();
    on_setup(objMan);
    omAddObjEx(objMan, 0, 0, 0, -1, ObjUpdate);
}

void CMinigameCore::ObjUpdate(omObj_s *obj)
{
    if (omSysExitReq != 0 && WipeCheckEnd() == 0) {
        StartFadeOut();
        HuAudFadeOut(1000);
        obj->objFunc = MgExit;
        return;
    }
    CMinigameCore *core = instance;
    core->update();
}

void CMinigameCore::update()
{
    on_update();
    if (m_delay > 0) {
        m_delay--;
        if (m_delay == 0) {
        }
    }
}

void CMinigameCore::on_update()
{
}

void CMinigameCore::MgExit(omObj_s *obj)
{
    CMinigameCore *core = instance;
    if (WipeCheckEnd() != 0) {
        core->update();
        return;
    }
    core->on_close();
    if (core != 0) {
        delete core;
    }
    CharModelKill(-1);
    omOvlReturnEx(1, 1);
}

void CMinigameCore::AddWinner(int player)
{
    M501_ASSERT(instance != 0, "ms_pointer != 0", "singleton.h", 0x27);
    s32 *winner = instance->m_winner;
    for (int i = 0; i < 4; i++) {
        if (winner[i] < 0) {
            winner[i] = player;
            return;
        }
    }
}

bool CMinigameCore::CheckWinner(int player)
{
    M501_ASSERT(instance != 0, "ms_pointer != 0", "singleton.h", 0x27);
    for (int i = 0; i < 4; i++) {
        if (player == instance->m_winner[i]) {
            return true;
        }
    }
    return false;
}

void CMinigameCore::StartStartGameMes()
{
    M501_ASSERT(instance != 0, "ms_pointer != 0", "singleton.h", 0x27);
    CMinigameCore *core = instance;
    CMinigameCore *core2 = core;
    core2->m_gameMes = GameMesCreate(2, 0);
}

void CMinigameCore::StartFinishGameMes()
{
    M501_ASSERT(instance != 0, "ms_pointer != 0", "singleton.h", 0x27);
    CMinigameCore *core = instance;
    CMinigameCore *core2 = core;
    core2->m_gameMes = GameMesCreate(2, 1);
}

bool CMinigameCore::CheckGameMes()
{
    M501_ASSERT(instance != 0, "ms_pointer != 0", "singleton.h", 0x27);
    CMinigameCore *core = instance;
    bool done = false;
    if (GameMesStatGet(-1) == 0 && (u8)(core->m_delay <= 0) != 0) {
        done = true;
    }
    return done;
}

void CMinigameCore::StartWinnerGameMes()
{
    s32 chars[4];
    s32 count;
    s32 i;

    M501_ASSERT(instance != 0, "ms_pointer != 0", "singleton.h", 0x27);
    instance->m_delay = s_winnerDelay.m_value;
    s32 *winner = instance->m_winner;
    count = 0;
    while (count < 4 && winner[count] >= 0) {
        count++;
    }
    if (count == 0) {
        GameMesCreate(2, 2);
        return;
    }
    for (i = 1; i < count; i++) {
        s32 j = i;
        while (j >= 1 && winner[j - 1] > winner[j]) {
            winner[j - 1] = winner[j];
            winner[j] = winner[j - 1];
            j--;
        }
    }
    for (i = 0; i < 4; i++) {
        s16 charNo;
        if (winner[i] < 0) {
            charNo = -1;
        } else {
            charNo = GwPlayerConf[winner[i]].charNo;
        }
        chars[i] = charNo;
    }
    GameMesCreate(4, 3, chars[0], chars[1], chars[2], chars[3]);
}

void CMinigameCore::StartRecordGameMes(unsigned int rec)
{
    if (_CheckFlag(0x1000F) == 0) {
        M501_ASSERT(instance != 0, "ms_pointer != 0", "singleton.h", 0x27);
        CMinigameCore *core = instance;
        core->m_gameMes = GameMesCreate(14, rec);
        HuAudJinglePlay(0x86);
    }
}

void CMinigameCore::StartFadeIn()
{
    WipeCreate(1, 5, 60);
}

void CMinigameCore::StartFadeOut()
{
    WipeCreate(2, 6, 60);
}

void CMinigameCore::ExitReq()
{
    omSysExitReq = 1;
}

CModel::CModel(void *data)
{
    m_id = Hu3DModelCreate(data);
}

CModel::CModel(unsigned int dataNum)
{
    void *data = HuDataSelHeapReadNum(dataNum, 0x10000000, HEAP_MODEL);
    m_id = Hu3DModelCreate(data);
}

extern "C" CModel *__ct__6CModelF13HU3DMODELHOOK(CModel *self, HU3DMODELHOOK hook)
{
    self->m_id = Hu3DHookFuncCreate(hook);
    return self;
}

CModel::CModel(int charNo, int mode)
{
    s16 type = 0;
    switch (mode) {
    case 0:
        type = 1;
        break;
    case 1:
        type = 2;
        break;
    case 2:
        type = 4;
        break;
    case 3:
        type = 8;
        break;
    }
    m_id = CharModelCreate((s16)charNo, type);
}

CModel::CModel(CModel &src)
{
    s16 srcId = src.m_id;
    s16 id = srcId;
    m_id = Hu3DModelLink(id);
}

void CModel::setAttr(unsigned int attr)
{
    Hu3DModelAttrSet(m_id, attr);
}

void CModel::resetAttr(unsigned int attr)
{
    Hu3DModelAttrReset(m_id, attr);
}

void CModel::setObjXform(omObj_s *obj)
{
    s16 id = m_id;
    Hu3DModelPosSet(id, obj->trans.x, obj->trans.y, obj->trans.z);
    Hu3DModelRotSet(id, obj->rot.x, obj->rot.y, obj->rot.z);
    Hu3DModelScaleSet(id, obj->scale.x, obj->scale.y, obj->scale.z);
}

void CModel::setPos(Vec *pos)
{
    Hu3DModelPosSet(m_id, pos->x, pos->y, pos->z);
}

void CModel::setRot(Vec *rot)
{
    Hu3DModelRotSet(m_id, rot->x, rot->y, rot->z);
}

void CModel::setScale(Vec *scale)
{
    Hu3DModelScaleSet(m_id, scale->x, scale->y, scale->z);
}

void CModel::setScale(f32 scale)
{
    Hu3DModelScaleSet(m_id, scale, scale, scale);
}

void CModel::setTPLvl(f32 lvl)
{
    Hu3DModelTPLvlSet(m_id, lvl);
}

void CModel::setLayer(s16 layer)
{
    Hu3DModelLayerSet(m_id, layer);
}

void CModel::setCamera(unsigned short camBit)
{
    Hu3DModelCameraSet(m_id, camBit);
}

void CModel::setHookData(void *data)
{
    Hu3DData[m_id].hookData = data;
}

Hu3DMtx *CModel::getMatrix()
{
    return (Hu3DMtx *)&Hu3DData[m_id].mtx;
}

CModel::~CModel()
{
    Hu3DModelKill(m_id);
}

void *CModel::getObjPtr(const char *name)
{
    HSFOBJECT *obj = Hu3DModelObjPtrGet(m_id, (char *)name);
    M501_ASSERT(obj != 0, "p != NULL", "model.cpp", 0x8A);
    return obj;
}

void CModel::set_shadow()
{
    Hu3DModelShadowSet(m_id);
}

void CModel::setShadowMap()
{
    Hu3DModelShadowMapSet(m_id);
}

void CModel::setShadowMapObj(const char *name)
{
    Hu3DModelShadowMapObjSet(m_id, (char *)name);
}

void CModel::resetShadowMapObj()
{
    Hu3DModelShadowMapObjReset(m_id);
}

void CModel::shadowMapOffObj(char *name)
{
    HSFOBJECT *obj = (HSFOBJECT *)getObjPtr(name);
    ((HSFCONSTDATA *)obj->constData)->attr &= ~8;
}

void CModel::shadowOnObj(char *name)
{
    HSFOBJECT *obj = (HSFOBJECT *)getObjPtr(name);
    obj->flags |= 0x400;
}

void CModel::shadowOffObj(char *name)
{
    HSFOBJECT *obj = (HSFOBJECT *)getObjPtr(name);
    obj->flags &= ~0x400;
}

void CModel::shadowOnlyObjOn(char *name)
{
    HSFOBJECT *obj = (HSFOBJECT *)getObjPtr(name);
    obj->flags |= 0x400;
    HSFTRANSFORM *base = &obj->mesh.base;
    base->scale.x = 0.0f;
    base->scale.y = 0.0f;
    base->scale.z = 0.0f;
}

void CModel::shadowOnlyObjOff(char *name)
{
    HSFOBJECT *obj = (HSFOBJECT *)getObjPtr(name);
    obj->flags &= ~0x400;
    HSFTRANSFORM *base = &obj->mesh.base;
    base->scale.x = 1.0f;
    base->scale.y = 1.0f;
    base->scale.z = 1.0f;
}

void CModel::setMotTime(int time)
{
    Hu3DMotionTimeSet(m_id, (f32)time);
}

void CMT19937::setSeed(int seed)
{
    m_state[0] = seed;
    for (m_index = 1; m_index < 624; m_index++) {
        m_state[m_index] = m_index + (0x6C078965 * (m_state[m_index - 1] ^ (m_state[m_index - 1] >> 30)));
    }
}

extern "C" void setSeedDefault__8CMT19937Fv(CMT19937 *self, u32 *key, u32 keyLen)
{
    u32 i;
    u32 j;
    u32 k;

    self->m_state[0] = 0x012BD6AA;
    for (self->m_index = 1; self->m_index < 624; self->m_index++) {
        self->m_state[self->m_index] = self->m_index + (0x6C078965 * (self->m_state[self->m_index - 1] ^ (self->m_state[self->m_index - 1] >> 30)));
    }
    i = 1;
    j = 0;
    k = keyLen < 624 ? 624 : keyLen;
    for (; k != 0; k--) {
        self->m_state[i] = key[j] + ((self->m_state[i] ^ ((self->m_state[i - 1] ^ (self->m_state[i - 1] >> 30)) * 0x19660D)) + j);
        i++;
        j++;
        if (i >= 624) {
            self->m_state[0] = self->m_state[623];
            i = 1;
        }
        if (j >= keyLen) {
            j = 0;
        }
    }
    for (k = 623; k != 0; k--) {
        self->m_state[i] = (self->m_state[i] ^ ((self->m_state[i - 1] ^ (self->m_state[i - 1] >> 30)) * 0x5D588B65)) - i;
        i++;
        if (i >= 624) {
            self->m_state[0] = self->m_state[623];
            i = 1;
        }
    }
    self->m_state[0] = 0x80000000;
}

static M501_RODATA const u32 s_mag01[2] = { 0, 0x9908B0DF };

inline u32 CMT19937::gen()
{
    u32 y;

    if (m_index >= 624) {
        u32 kk = 0;
        for (; kk < 227; kk++) {
            y = (m_state[kk] & 0x80000000) | (m_state[kk + 1] & 0x7FFFFFFF);
            m_state[kk] = s_mag01[y & 1] ^ ((y >> 1) ^ m_state[kk + 397]);
        }
        for (; kk < 623; kk++) {
            y = (m_state[kk] & 0x80000000) | (m_state[kk + 1] & 0x7FFFFFFF);
            m_state[kk] = s_mag01[y & 1] ^ ((y >> 1) ^ m_state[kk - 227]);
        }
        y = (m_state[623] & 0x80000000) | (m_state[0] & 0x7FFFFFFF);
        m_state[623] = (y >> 1) ^ m_state[396] ^ s_mag01[y & 1];
        m_index = 0;
    }
    y = m_state[m_index++];
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9D2C5680;
    y ^= (y << 15) & 0xEFC60000;
    return y ^ (y >> 18);
}

u32 CMT19937::random()
{
    return gen();
}

u32 CMT19937::irandom()
{
    return gen() >> 1;
}

f32 CMT19937::nrandom()
{
    return 2.3283064e-10f * (f32)gen();
}

f32 CMT19937::nrandom2()
{
    return 2.3283064e-10f * (f32)gen();
}

f32 CMT19937::nrandomMid()
{
    return 2.3283064e-10f * (0.5f + (f32)gen());
}

double CMT19937::frandom()
{
    u32 a = gen();
    u32 b = gen();
    return 1.1102230246251565e-16 * ((67108864.0 * (f64)(a >> 5)) + (f64)(b >> 6));
}

f32 CMT19937::randomMantissa()
{
    return 1.1920929e-7f * (f32)(gen() >> 9);
}

void *CMinigameCore::operator new(unsigned long size)
{
    void *ptr = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);
    M501_ASSERT(ptr != 0, "ptr != NULL", "om.cpp", 0x18);
    return ptr;
}

void *CMinigameCore::operator new[](unsigned long size)
{
    void *ptr = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);
    M501_ASSERT(ptr != 0, "ptr != NULL", "om.cpp", 0x1F);
    return ptr;
}

void CMinigameCore::operator delete(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

void CMinigameCore::operator delete[](void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

COMObj::COMObj(omObj_s *obj)
{
    obj->data = this;
    obj->objFunc = OmObjUpdate;
}

void COMObj::OmObjUpdate(omObj_s *obj)
{
    COMObj *self = (COMObj *)obj->data;
    self->on_update(obj);
}

void COMObj::on_update(omObj_s *obj)
{
}

void COMObj::OmObjDelete(omObj_s *obj)
{
    COMObj *self = (COMObj *)obj->data;
    if (self != 0) {
        delete self;
    }
    omDelObjEx(HuPrcCurrentGet(), obj);
}

COMObj::~COMObj()
{
}

void COMObj::OmObjDelete(Process_s *proc, omObj_s *obj)
{
    COMObj *self = (COMObj *)obj->data;
    if (self != 0) {
        delete self;
    }
    omDelObjEx(proc, obj);
}

struct CCircle {
    Vec m_pos;
    f32 m_radius;
};

extern "C" bool fn_1_4F0C(CCircle *a, CCircle *b)
{
    Vec delta;
    f32 sum;

    PSVECSubtract(&a->m_pos, &b->m_pos, &delta);
    f32 ra = b->m_radius;
    f32 rb = a->m_radius;
    sum = rb + ra;
    f32 dist = PSVECMag(&delta);
    return dist < sum;
}

/* pushes two overlapping circles apart, weighted by wa/wb */
extern "C" s32 fn_1_4FB4(CCircle *a, CCircle *b, f32 wa, f32 wb)
{
    Vec delta;
    Vec shift;
    f32 sum;
    f32 dist;

    f32 wa2 = wa;
    f32 rb = b->m_radius;
    f32 ra = a->m_radius;
    sum = ra + rb;
    PSVECSubtract(&b->m_pos, &a->m_pos, &delta);
    f32 d0 = PSVECMag(&delta);
    dist = d0;
    if (dist >= sum) {
        return 0;
    }
    f32 wa3 = wa2;
    f32 inv = 1.0f - wa2;
    PSVECScale(&delta, &delta, (wb * (sum - dist)) / dist);
    PSVECScale(&delta, &shift, -wa3);
    Vec *s1 = &shift;
    Vec *s2 = s1;
    PSVECAdd(s2, &a->m_pos, &a->m_pos);
    PSVECScale(&delta, &shift, inv);
    Vec *s3 = &shift;
    Vec *s4 = s3;
    PSVECAdd(s4, &b->m_pos, &b->m_pos);
    return 1;
}

static u8 s_stackDebug;

s32 CSubThread::GetStartPriority()
{
    return OSGetThreadPriority(OSGetCurrentThread());
}

CSubThread::CSubThread(unsigned long stackSize) :
    m_thread(ThreadMain, this, stackSize, OSGetThreadPriority(OSGetCurrentThread()), 1)
{
    OSInitSemaphore(&m_sema, 0);
}

CSubThread::~CSubThread()
{
}

bool CSubThread::isTerminated()
{
    OSThread *th = &m_thread.m_thread;
    bool terminated = OSIsThreadTerminated(th) != 0;
    return (u8)terminated;
}

s32 CSubThread::wakeupThread(int arg)
{
    int a = arg;
    OSThread *th = &m_thread.m_thread;
    if (OSIsThreadTerminated(th) != 0) {
        return m_arg;
    }
    m_arg = a;
    OSResumeThread(th);
    OSWaitSemaphore(&m_sema);
    m_thread.getStackRemain();
    OSCheckActiveThreads();
    return m_arg;
}

s32 CSubThread::sleepThread(int arg)
{
    M501_ASSERT(OSGetCurrentThread() == &m_thread.m_thread, "::OSGetCurrentThread() == m_th", "subthread.cpp", 0x47);
    M501_ASSERT(OSIsThreadTerminated(&m_thread.m_thread) == 0, "!::OSIsThreadTerminated(m_th)", "subthread.cpp", 0x4A);
    m_arg = arg;
    OSSignalSemaphore(&m_sema);
    OSSuspendThread(&m_thread.m_thread);
    return m_arg;
}

void *CSubThread::ThreadMain(void *self)
{
    CSubThread *thread = (CSubThread *)self;
    s32 arg = thread->m_arg;
    thread->thread_main();
    OSSignalSemaphore(&thread->m_sema);
    return 0;
}

CThread::CThread(void *(*func)(void *), void *arg, u32 stackSize, s32 priority, u16 attr)
{
    u32 words = stackSize >> 2;
    u32 *stack = (u32 *)operator new(words * 4);
    m_stack = stack;
    createThread(func, arg, stack, words, priority, attr);
}

CThread::CThread(void *(*func)(void *), void *arg, u32 *stack, u32 stackSize, s32 priority, u16 attr)
{
    m_stack = 0;
    createThread(func, arg, stack, stackSize >> 2, priority, attr);
}

void CThread::createThread(void *(*func)(void *), void *arg, u32 *stack, u32 stackWords, s32 priority, u16 attr)
{
    bool valid = false;
    if (priority >= 0 && priority <= 31) {
        valid = true;
    }
    M501_ASSERT(valid, "priority >= OS_PRIORITY_MIN && priority <= OS_PRIORITY_MAX", "thread.cpp", 0x52);
    initStack(stack, stackWords);
    checkStackOverflow(stack, 0);
    if ((u8)(OSCreateThread(&m_thread, func, arg, &stack[stackWords], stackWords * 4, priority, attr) != 0) == 0) {
        OSPanic("thread.cpp", 0x60, "OSCreateThread() failed.");
    }
}

CThread::~CThread()
{
    if (OSIsThreadTerminated(&m_thread) == 0) {
        OSCancelThread(&m_thread);
    }
    if (s_stackDebug != 0) {
        u32 total = (u32)(m_thread.stackBase - (u8 *)m_thread.stackEnd);
        u32 remain = getStackRemain();
        f64 pct = 100.0 * (f64)remain;
        printf("delete CThread@%p\nstack remain/total = 0x%08x/0x%08x (%f%%)\n", this, remain, total, pct / (f64)total);
    }
    if (m_stack != 0) {
        HuMemDirectFree(m_stack);
    }
}

u32 CThread::getStackRemain()
{
    u32 *base = m_thread.stackEnd;
    if (checkStackOverflow(base, 1) != 0) {
        OSPanic("thread.cpp", 0x7F, "stack overflow\n");
    }
    u32 *p = base;
    while (p < (u32 *)m_thread.stackBase) {
        if (*p != 0xDEADBABE) {
            break;
        }
        p++;
    }
    return (u32)(p - base) * 4;
}

s32 CThread::checkStackOverflow(u32 *stack, int mode)
{
    u32 *end = stack + 8;
    switch (mode) {
    case 0:
        while (stack != end) {
            *stack = 0xDEADBABE;
            stack++;
        }
        break;
    case 1:
        while (stack != end) {
            u32 v = *stack;
            stack++;
            if (v != 0xDEADBABE) {
                return 1;
            }
        }
        break;
    }
    return 0;
}

void CThread::initStack(u32 *stack, u32 words)
{
    if (s_stackDebug != 0) {
        u32 *end = stack + words;
        while (stack != end) {
            *stack = 0xDEADBABE;
            stack++;
        }
    }
}

CTimer::CTimer(int type)
{
    switch (type) {
    case 0:
        m_timer = MgTimerCreate(0);
        break;
    case 1:
        m_timer = MgTimerCreate(1);
        break;
    case 2:
        m_timer = MgTimerCreate(2);
        break;
    default:
        __assert_fail("false", "timer.cpp", 0x1E);
        break;
    }
}

CTimer::~CTimer()
{
    MgTimerKill(m_timer);
}

void CTimer::setParam(int a, int b, int c)
{
    MgTimerParamSet(m_timer, a, b, c);
}

s32 CTimer::getValue()
{
    return MgTimerValueGet(m_timer);
}

void CTimer::setPos(f32 x, f32 y)
{
    MgTimerPosSet(m_timer, x, y);
}

void CTimer::getPos(f32 *x, f32 *y)
{
    MgTimerPosGet(m_timer, x, y);
}

void CTimer::setModeOn(int mode)
{
    switch (mode) {
    case 0:
        MgTimerModeOnSet(m_timer, 0);
        return;
    case 1:
        MgTimerModeOnSet(m_timer, 1);
        return;
    case 2:
        MgTimerModeOnSet(m_timer, 2);
        return;
    default:
        __assert_fail("false", "timer.cpp", 0x48);
        return;
    }
}

void CTimer::setModeOff()
{
    MgTimerModeOffSet(m_timer);
}

void CTimer::stopTimer()
{
    if (m_timer->stopF == 0) {
        m_timer->stopF = 1;
        m_timer->mode = 1;
    }
}

void CTimer::resetTimer()
{
    MgTimerKill(m_timer);
    m_timer = MgTimerCreate(0);
}

bool CTimer::checkDone()
{
    return (u8)(MgTimerDoneCheck(m_timer) != 0);
}

void CTimer::setWinColor(u8 r, u8 g, u8 b)
{
    MgTimerWinColorSet(m_timer, r, g, b);
}

/* static initialiser of the original: ObjectList plus the three delays */
extern "C" void *fn_1_5E18(void *self, s16 flag);
extern "C" void __register_global_object(void *obj, void *(*dtor)(void *, s16), void *reg);

extern "C" void fn_1_5D90(void)
{
    CGameObject::ObjectList.m_prev = &CGameObject::ObjectList;
    CGameObject::ObjectList.m_next = &CGameObject::ObjectList;
    __register_global_object(&CGameObject::ObjectList, fn_1_5E18, s_objectListChain);
    s_delay18.m_value = 0x78;
    s_winnerDelay.m_value = 0xD2;
    s_delay20.m_value = 0xD2;
}

extern "C" void *fn_1_5E18(void *self, s16 flag)
{
    if (self != 0) {
        if (self != 0) {
            ((CLinkNode *)self)->~CLinkNode();
        }
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}
