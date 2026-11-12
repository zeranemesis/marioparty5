#include <stdarg.h>

/* the DOL functions are plain C symbols */
extern "C" {
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
#include "game/mg/timer.h"
}

#ifdef __MWERKS__
#define M509_RODATA __declspec(section ".rodata")
#else
#define M509_RODATA
#endif

/* Nodes of the circular intrusive list every m509 object hangs on; the real objects continue with
 * a vtable at +8 and a flag word at +0xC. */
typedef struct M509Node {
    M509Node *next;
    M509Node *prev;
} M509Node;

typedef struct M509Obj {
    M509Node node;
    void **vtbl;
    u32 flags;
} M509Obj;

typedef struct M509Sema {
    OSThreadQueue queue;
    s32 count;
} M509Sema;

typedef struct M509Thread {
    OSThread th;
    u32 *stack;
} M509Thread;

typedef struct M509SubThread {
    void **vtbl;
    M509Thread thread;
    M509Sema sema;
    s32 arg;
} M509SubThread;

typedef struct M509Core {
    void **vtbl0;
    void **vtbl;
    s32 winner[4];
    s32 gameMes;
    s32 delay;
} M509Core;

typedef struct M509Cam {
    Vec pos;
    Vec up;
    Vec target;
} M509Cam;

typedef struct M509CamView {
    Vec pos;
    Quaternion rot;
} M509CamView;

class CMT19937 {
public:
    CMT19937(int seed = 5489) { setSeed(seed); }
    void setSeed(int seed);
    inline u32 gen();
    u32 irandom();
    double frandom();
    double randomMantissa();

    u32 m_state[624];
    s32 m_index;
};

extern "C" {
void OSPanic(const char *file, int line, const char *msg, ...);
int sprintf(char *buf, const char *fmt, ...);
int printf(const char *fmt, ...);
int vprintf(const char *fmt, va_list args);
void OSInitSemaphore(M509Sema *sem, s32 count);
s32 OSWaitSemaphore(M509Sema *sem);
s32 OSSignalSemaphore(M509Sema *sem);
void __register_global_object(void *obj, void *(*dtor)(void *, s16), void *reg);
OMOBJMAN *MgActorObjectSetup(void);
}

extern "C" {
void fn_1_0(const char *cond, const char *file, s32 line);
int fn_1_60(void *file, const char *fmt, ...);
void *fn_1_104(s32 size);
void *fn_1_190(s32 size);
void fn_1_21C(void *ptr);
void fn_1_254(void *ptr);
M509Thread *fn_1_28C(M509Thread *self, void *(*func)(void *), void *arg, u32 stackSize, s32 priority, u16 attr);
M509Thread *fn_1_370(M509Thread *self, void *(*func)(void *), void *arg, u32 *stack, u32 stackSize, s32 priority, u16 attr);
void fn_1_3EC(M509Thread *self, void *(*func)(void *), void *arg, u32 *stack, u32 words, s32 priority, u16 attr);
M509Thread *fn_1_510(M509Thread *self, s16 flag);
u32 fn_1_644(M509Thread *self);
s32 fn_1_6E4(u32 *stack, s32 mode);
void fn_1_758(u32 *stack, u32 words);
void fn_1_7A4(void);
void *fn_1_7C8(M509SubThread *self, u32 stackSize);
void *fn_1_9E8(M509SubThread *self, s16 flag);
u8 fn_1_BA4(M509SubThread *self);
s32 fn_1_BEC(M509SubThread *self, s32 arg);
s32 fn_1_CE8(M509SubThread *self, s32 arg);
void *fn_1_DEC(void *self);
void fn_1_E48(M509Node *node);
void *fn_1_E54(M509Node *node, s16 flag);
void fn_1_EC0(M509Node *node);
void fn_1_EE4(M509Node *node, M509Node *head);
void fn_1_F14(M509Node *node, M509Node *head);
void fn_1_F44(M509Node *from, M509Node *to);
void fn_1_FCC(M509Node *head, u8 (*cmp)(M509Node *, M509Node *));
M509Node *fn_1_1084(u8 (*cmp)(M509Node *, M509Node *), M509Node *list);
M509Node *fn_1_172C(M509Node *list);
M509Node *fn_1_1780(u8 (*cmp)(M509Node *, M509Node *), M509Node *a, M509Node *b);
void fn_1_18A0(CMT19937 *self, u32 *key, u32 keyLen);
u32 fn_1_1A64(CMT19937 *self);
f32 fn_1_1D88(CMT19937 *self);
f32 fn_1_1F4C(CMT19937 *self);
f32 fn_1_2110(CMT19937 *self);
void *fn_1_282C(s32 size);
void *fn_1_28B8(s32 size);
void fn_1_2944(void *ptr);
void fn_1_297C(void *ptr);
void fn_1_29B4(void **self, OMOBJ *obj);
void fn_1_29D4(OMOBJ *obj);
void fn_1_2A20(void);
void fn_1_2A24(OMOBJ *obj);
void **fn_1_2A94(void **self, s16 flag);
void fn_1_2AF4(HUPROCESS *proc, OMOBJ *obj);
void *fn_1_2B68(M509Core *self);
void *fn_1_2C38(M509Core *self, s16 flag);
void *fn_1_2CF4(M509Core *self, s16 flag);
void fn_1_2EB4(M509Core *self, s32 prio, s32 stackSize);
void fn_1_2F38(M509Core *self);
void fn_1_2FA0(OMOBJ *obj);
void fn_1_3030(M509Core *self);
void fn_1_309C(void);
void fn_1_30A0(OMOBJ *obj);
void fn_1_318C(s32 player);
s32 fn_1_3260(s32 player);
void fn_1_3334(void);
void fn_1_33EC(void);
s32 fn_1_34A4(void);
void fn_1_3578(void);
void fn_1_3744(s32 rec);
void fn_1_381C(void);
void fn_1_3848(void);
void fn_1_3874(void);
void fn_1_3888(s32 mode);
s32 fn_1_3920(void);
void fn_1_39AC(void);
void fn_1_3A1C(void);
void fn_1_3A20(void);
void *fn_1_3AE8(M509Obj *self, s16 flag);
s32 fn_1_3B70(void);
s16 *fn_1_3B80(s16 *model, void *data);
s16 *fn_1_3BBC(s16 *model, s32 dataNum);
s16 *fn_1_3C04(s16 *model, void (*hook)(HU3DMODEL *, Mtx *));
s16 *fn_1_3C40(s16 *model, s32 charNo, s32 mode);
s16 *fn_1_3CDC(s16 *model, s16 *src);
void fn_1_3D30(s16 *model, u32 attr);
void fn_1_3D68(s16 *model, u32 attr);
void fn_1_3DA0(s16 *model, OMOBJ *obj);
void fn_1_3E1C(s16 *model, Vec *pos);
void fn_1_3E64(s16 *model, Vec *rot);
void fn_1_3EAC(s16 *model, Vec *scale);
void fn_1_3EF4(s16 *model, f32 scale);
void fn_1_3F3C(s16 *model, f32 lvl);
void fn_1_3F74(s16 *model, s16 layer);
void fn_1_3FAC(s16 *model, u16 camBit);
void fn_1_3FE4(s16 *model, void *data);
f32 *fn_1_4000(s16 *model);
s16 *fn_1_401C(s16 *model, s16 flag);
HSFOBJECT *fn_1_4078(s16 *model, char *name);
void fn_1_410C(s16 *model);
void fn_1_413C(s16 *model);
void fn_1_416C(s16 *model, char *name);
void fn_1_41A4(s16 *model);
void fn_1_41D4(s16 *model, char *name);
void fn_1_4280(s16 *model, char *name);
void fn_1_4328(s16 *model, char *name);
void fn_1_43D0(s16 *model, char *name);
void fn_1_44AC(s16 *model, char *name);
void fn_1_4588(s16 *model, s32 time);
void fn_1_45F4(M509Cam *cam, f32 yaw, f32 pitch);
void fn_1_46B8(M509Cam *cam, f32 angle);
void fn_1_4724(M509Cam *cam, Vec *a, Vec *b, Vec *c);
void fn_1_47CC(M509Cam *cam, Mtx mtx);
void fn_1_4820(M509Cam *cam, Vec *offset);
void fn_1_4888(M509Cam *cam, Vec *offset);
void fn_1_48DC(M509Cam *cam);
void fn_1_4970(M509Cam *cam, M509CamView *view);
void fn_1_4A10(M509Cam *cam, M509CamView *view);
void fn_1_4A88(M509Cam *cam, M509CamView *a, M509CamView *b, f32 t);
void fn_1_4B98(M509Cam *cam, Vec *pos);
void fn_1_4C04(M509Cam *cam, Vec *a, Vec *b, f32 t);
void fn_1_4CBC(M509Cam *cam, u16 camNo);
void fn_1_4CFC(M509Cam *cam, u16 camNo);
MGTIMER **fn_1_4D3C(MGTIMER **self, s32 type);
MGTIMER **fn_1_4E04(MGTIMER **self, s16 flag);
void fn_1_4E60(MGTIMER **self, s32 a, s32 b, s32 c);
void fn_1_4EA8(MGTIMER **self);
void fn_1_4ED8(MGTIMER **self, f32 x, f32 y);
void fn_1_4F18(MGTIMER **self, f32 *x, f32 *y);
void fn_1_4F58(MGTIMER **self, s32 mode);
void fn_1_501C(MGTIMER **self);
void fn_1_504C(MGTIMER **self);
void fn_1_5078(MGTIMER **self);
u8 fn_1_50B4(MGTIMER **self);
void fn_1_50F4(MGTIMER **self, u8 r, u8 g, u8 b);
void fn_1_513C(M509Cam *cam);
f32 fn_1_51D4(s32 pad);
f32 fn_1_5228(s32 pad);
f32 fn_1_527C(s32 pad);
f32 fn_1_52D0(s32 pad);
void fn_1_5324(M509Cam *cam, s32 pad);
void fn_1_5980(void);
void *fn_1_5A08(M509Obj *self, s16 flag);
}

char lbl_1_data_0[] = "\nASSERTION FAILED at %s:%d\n%s";
char lbl_1_data_1E[] = "assert.cpp";
char lbl_1_data_29[] = "new.cpp";
char lbl_1_data_31[] = "ptr != NULL";
char lbl_1_data_3D[] = "thread.cpp";
char lbl_1_data_48[] = "priority >= OS_PRIORITY_MIN && priority <= OS_PRIORITY_MAX";
char lbl_1_data_83[] = "OSCreateThread() failed.";
char lbl_1_data_9C[] = "delete CThread@%p\nstack remain/total = 0x%08x/0x%08x (%f%%)\n";
char lbl_1_data_D9[] = "stack overflow\n";
char lbl_1_data_E9[] = "subthread.cpp";
char lbl_1_data_F7[] = "::OSGetCurrentThread() == m_th";
char lbl_1_data_116[] = "!::OSIsThreadTerminated(m_th)";
char lbl_1_data_134[] = "om.cpp";
char lbl_1_data_13B[] = "singleton.h";
char lbl_1_data_147[] = "ms_pointer == 0";
char lbl_1_data_157[] = "ms_pointer != 0";
char lbl_1_data_167[] = "model.cpp";
char lbl_1_data_171[] = "p != NULL";
char lbl_1_data_17B[] = "timer.cpp";
char lbl_1_data_185[] = "false";
char lbl_1_data_18B[] = "{{0x%08x,0x%08x,0x%08x},{0x%08x,0x%08x,0x%08x,0x%08x}},\n";
void *lbl_1_data_1C4[5] = { 0, 0, (void *)fn_1_3AE8, 0, (void *)fn_1_3A1C };
void *lbl_1_data_1D8[6] = { 0, 0, (void *)fn_1_2CF4, 0, (void *)fn_1_309C, 0 };
void *lbl_1_data_1F0[4] = { 0, 0, (void *)fn_1_2A94, (void *)fn_1_2A20 };
void *lbl_1_data_200[4] = { 0, 0, (void *)fn_1_9E8, 0 };

u8 lbl_1_bss_0;
s32 lbl_1_bss_4;
s32 lbl_1_bss_8;
s32 lbl_1_bss_C;
u8 lbl_1_bss_10[12];
M509Node lbl_1_bss_1C;
s32 lbl_1_bss_24;
M509Core *lbl_1_bss_28;

#define M509_ASSERT(c_, t_, f_, l_) ((c_) ? (void)0 : fn_1_0(t_, f_, l_))

#define M509_DEG2RAD 0.017453292f

#define f32Bits(v_) (*(u32 *)&(v_))

static inline bool M509IsEmpty(M509Node *list)
{
    bool empty = false;
    if (list->next == list && list->next == list->prev) {
        empty = true;
    }
    return empty;
}

void fn_1_0(const char *cond, const char *file, s32 line)
{
    char buf[1024];
    sprintf(buf, lbl_1_data_0, file, line, cond);
    OSPanic(lbl_1_data_1E, 0x12, buf);
}

int fn_1_60(void *file, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    return vprintf(fmt, args);
}

void *fn_1_104(s32 size)
{
    void *ptr = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);
    if (ptr != 0) {
    } else {
        fn_1_0(lbl_1_data_31, lbl_1_data_29, 0x17);
    }
    return ptr;
}

void *fn_1_190(s32 size)
{
    void *ptr = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);
    if (ptr != 0) {
    } else {
        fn_1_0(lbl_1_data_31, lbl_1_data_29, 0x17);
    }
    return ptr;
}

void fn_1_21C(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_254(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

M509Thread *fn_1_28C(M509Thread *self, void *(*func)(void *), void *arg, u32 stackSize, s32 priority, u16 attr)
{
    u32 words = stackSize >> 2;
    u32 *stack = (u32 *)fn_1_104(words * 4);
    self->stack = stack;
    fn_1_3EC(self, func, arg, stack, words, priority, attr);
    return self;
}

M509Thread *fn_1_370(M509Thread *self, void *(*func)(void *), void *arg, u32 *stack, u32 stackSize, s32 priority, u16 attr)
{
    self->stack = 0;
    fn_1_3EC(self, func, arg, stack, stackSize >> 2, priority, attr);
    return self;
}

void fn_1_3EC(M509Thread *self, void *(*func)(void *), void *arg, u32 *stack, u32 words, s32 priority, u16 attr)
{
    bool valid = false;
    if (priority >= 0 && priority <= 31) {
        valid = true;
    }
    M509_ASSERT(valid, lbl_1_data_48, lbl_1_data_3D, 0x52);
    fn_1_758(stack, words);
    fn_1_6E4(stack, 0);
    if ((u8)(OSCreateThread(&self->th, func, arg, &stack[words], words * 4, priority, attr) != 0) == 0) {
        OSPanic(lbl_1_data_3D, 0x60, lbl_1_data_83);
    }
}

M509Thread *fn_1_510(M509Thread *self, s16 flag)
{
    if (self != 0) {
        if (OSIsThreadTerminated(&self->th) == 0) {
            OSCancelThread(&self->th);
        }
        if (lbl_1_bss_0 != 0) {
            u32 total = (u32)(self->th.stackBase - (u8 *)self->th.stackEnd);
            u32 remain = fn_1_644(self);
            f64 pct = 100.0 * (f64)remain;
            printf(lbl_1_data_9C, self, remain, total, pct / (f64)total);
        }
        if (self->stack != 0) {
            HuMemDirectFree(self->stack);
        }
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

u32 fn_1_644(M509Thread *self)
{
    u32 *base = self->th.stackEnd;
    if (fn_1_6E4(base, 1) != 0) {
        OSPanic(lbl_1_data_3D, 0x7F, lbl_1_data_D9);
    }
    u32 *p = base;
    while (p < (u32 *)self->th.stackBase) {
        if (*p != 0xDEADBABE) {
            break;
        }
        p++;
    }
    return (u32)(p - base) * 4;
}

s32 fn_1_6E4(u32 *stack, s32 mode)
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

void fn_1_758(u32 *stack, u32 words)
{
    if (lbl_1_bss_0 != 0) {
        u32 *end = stack + words;
        while (stack != end) {
            *stack = 0xDEADBABE;
            stack++;
        }
    }
}

void fn_1_7A4(void)
{
    OSGetThreadPriority(OSGetCurrentThread());
}

void *fn_1_7C8(M509SubThread *self, u32 stackSize)
{
    s32 priority;
    u32 words;
    u32 *stack;

    self->vtbl = lbl_1_data_200;
    priority = OSGetThreadPriority(OSGetCurrentThread());
    words = stackSize >> 2;
    stack = (u32 *)fn_1_104(words * 4);
    self->thread.stack = stack;
    fn_1_3EC(&self->thread, fn_1_DEC, self, stack, words, priority, 1);
    OSInitSemaphore(&self->sema, 0);
    return self;
}

void *fn_1_9E8(M509SubThread *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_200;
        if (&self->thread != 0) {
            fn_1_510(&self->thread, 0);
        }
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

u8 fn_1_BA4(M509SubThread *self)
{
    OSThread *th = &self->thread.th;
    bool terminated = OSIsThreadTerminated(th) != 0;
    return (u8)terminated;
}

s32 fn_1_BEC(M509SubThread *self, s32 arg)
{
    int a = arg;
    OSThread *th = &self->thread.th;
    if (OSIsThreadTerminated(th) != 0) {
        return self->arg;
    }
    self->arg = a;
    OSResumeThread(th);
    OSWaitSemaphore(&self->sema);
    fn_1_644(&self->thread);
    OSCheckActiveThreads();
    return self->arg;
}

s32 fn_1_CE8(M509SubThread *self, s32 arg)
{
    M509_ASSERT(OSGetCurrentThread() == &self->thread.th, lbl_1_data_F7, lbl_1_data_E9, 0x47);
    M509_ASSERT(OSIsThreadTerminated(&self->thread.th) == 0, lbl_1_data_116, lbl_1_data_E9, 0x4A);
    self->arg = arg;
    OSSignalSemaphore(&self->sema);
    OSSuspendThread(&self->thread.th);
    return self->arg;
}

void *fn_1_DEC(void *p)
{
    M509SubThread *self = (M509SubThread *)p;
    s32 arg = self->arg;
    ((void (*)(M509SubThread *, s32))self->vtbl[3])(self, arg);
    OSSignalSemaphore(&self->sema);
    return 0;
}

void fn_1_E48(M509Node *node)
{
    node->next = node;
    node->prev = node;
}

void *fn_1_E54(M509Node *node, s16 flag)
{
    if (node != 0) {
        node->next->prev = node->prev;
        node->prev->next = node->next;
        if (flag > 0 && node != 0) {
            HuMemDirectFree(node);
        }
    }
    return node;
}

void fn_1_EC0(M509Node *node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;
    node->next = node;
    node->prev = node;
}

void fn_1_EE4(M509Node *node, M509Node *head)
{
    M509Node *l = head;
    node->prev = l;
    node->next = l->next;
    l->next->prev = node;
    l->next = node;
}

void fn_1_F14(M509Node *node, M509Node *head)
{
    M509Node *l = head;
    node->next = l;
    node->prev = l->prev;
    l->prev->next = node;
    l->prev = node;
}

void fn_1_F44(M509Node *from, M509Node *to)
{
    bool empty = false;
    if (from->next == from && from->next == from->prev) {
        empty = true;
    }
    if (empty == false) {
        M509Node *first = from->next;
        M509Node *last = from->prev;
        from->next = from;
        from->prev = from;
        first->prev = to->prev;
        last->next = to;
        to->prev->next = first;
        to->prev = last;
    }
}

void fn_1_FCC(M509Node *head, u8 (*cmp)(M509Node *, M509Node *))
{
    bool empty = false;
    M509Node *n;
    M509Node *last;

    if (head->next == head && head->next == head->prev) {
        empty = true;
    }
    if (empty == false) {
        head->prev->next = 0;
        n = fn_1_1084(cmp, head->next);
        head->next = n;
        last = head;
        while (n != 0) {
            n->prev = last;
            last = n;
            n = n->next;
        }
        head->prev = last;
        last->next = head;
    }
}

M509Node *fn_1_1084(u8 (*cmp)(M509Node *, M509Node *), M509Node *list)
{
    M509Node *second;

    if (list == 0) {
        return 0;
    }
    if (list->next == 0) {
        return list;
    }
    second = fn_1_172C(list);
    list = fn_1_1084(cmp, list);
    second = fn_1_1084(cmp, second);
    return fn_1_1780(cmp, list, second);
}

M509Node *fn_1_172C(M509Node *list)
{
    M509Node *slow = list;
    M509Node *fast = slow->next;
    M509Node *rest;

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
    rest = slow->next;
    slow->next = 0;
    return rest;
}

M509Node *fn_1_1780(u8 (*cmp)(M509Node *, M509Node *), M509Node *a, M509Node *b)
{
    M509Node *head;
    M509Node **tail = &head;

    while (a != 0 && b != 0) {
        if (cmp(a, b) != 0) {
            *tail = a;
            tail = &a->next;
            a = a->next;
        } else {
            *tail = b;
            tail = &b->next;
            b = b->next;
        }
    }
    if (a == 0) {
        *tail = b;
    } else {
        *tail = a;
    }
    return head;
}

void CMT19937::setSeed(int seed)
{
    m_state[0] = seed;
    for (m_index = 1; m_index < 624; m_index++) {
        m_state[m_index] = m_index + (0x6C078965 * (m_state[m_index - 1] ^ (m_state[m_index - 1] >> 30)));
    }
}

void fn_1_18A0(CMT19937 *self, u32 *key, u32 keyLen)
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

M509_RODATA const u32 lbl_1_rodata_10[2] = { 0, 0x9908B0DF };

inline u32 CMT19937::gen()
{
    u32 y;

    if (m_index >= 624) {
        u32 kk = 0;
        for (; kk < 227; kk++) {
            y = (m_state[kk] & 0x80000000) | (m_state[kk + 1] & 0x7FFFFFFF);
            m_state[kk] = lbl_1_rodata_10[y & 1] ^ ((y >> 1) ^ m_state[kk + 397]);
        }
        for (; kk < 623; kk++) {
            y = (m_state[kk] & 0x80000000) | (m_state[kk + 1] & 0x7FFFFFFF);
            m_state[kk] = lbl_1_rodata_10[y & 1] ^ ((y >> 1) ^ m_state[kk - 227]);
        }
        y = (m_state[623] & 0x80000000) | (m_state[0] & 0x7FFFFFFF);
        m_state[623] = (y >> 1) ^ m_state[396] ^ lbl_1_rodata_10[y & 1];
        m_index = 0;
    }
    y = m_state[m_index++];
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9D2C5680;
    y ^= (y << 15) & 0xEFC60000;
    return y ^ (y >> 18);
}

u32 fn_1_1A64(CMT19937 *self)
{
    return self->gen();
}

u32 CMT19937::irandom()
{
    return gen() >> 1;
}

f32 fn_1_1D88(CMT19937 *self)
{
    return 2.3283064e-10f * (f32)self->gen();
}

f32 fn_1_1F4C(CMT19937 *self)
{
    return 2.3283064e-10f * (f32)self->gen();
}

f32 fn_1_2110(CMT19937 *self)
{
    return 2.3283064e-10f * (0.5f + (f32)self->gen());
}

double CMT19937::frandom()
{
    u32 a = gen();
    u32 b = gen();
    return 1.1102230246251565e-16 * ((67108864.0 * (f64)(a >> 5)) + (f64)(b >> 6));
}

double CMT19937::randomMantissa()
{
    return 1.1920929e-7f * (f32)(gen() >> 9);
}

void *fn_1_282C(s32 size)
{
    void *ptr = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);
    M509_ASSERT(ptr != 0, lbl_1_data_31, lbl_1_data_134, 0x18);
    return ptr;
}

void *fn_1_28B8(s32 size)
{
    void *ptr = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);
    M509_ASSERT(ptr != 0, lbl_1_data_31, lbl_1_data_134, 0x1F);
    return ptr;
}

void fn_1_2944(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_297C(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_29B4(void **self, OMOBJ *obj)
{
    *self = lbl_1_data_1F0;
    obj->data = self;
    obj->objFunc = fn_1_29D4;
}

void fn_1_29D4(OMOBJ *obj)
{
    void **self = (void **)obj->data;
    ((void (*)(void **, OMOBJ *))((void **)*self)[3])(self, obj);
}

void fn_1_2A20(void)
{
}

void fn_1_2A24(OMOBJ *obj)
{
    void **self = (void **)obj->data;
    if (self != 0) {
        ((void (*)(void **, s32))((void **)*self)[2])(self, 1);
    }
    omDelObjEx(HuPrcCurrentGet(), obj);
}

void **fn_1_2A94(void **self, s16 flag)
{
    if (self != 0) {
        *self = lbl_1_data_1F0;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_2AF4(HUPROCESS *proc, OMOBJ *obj)
{
    void **self = (void **)obj->data;
    if (self != 0) {
        ((void (*)(void **, s32))((void **)*self)[2])(self, 1);
    }
    omDelObjEx(proc, obj);
}

void *fn_1_2B68(M509Core *self)
{
    s32 i;

    M509_ASSERT(lbl_1_bss_28 == 0, lbl_1_data_147, lbl_1_data_13B, 0x35);
    lbl_1_bss_28 = self;
    self->vtbl = lbl_1_data_1D8;
    self->delay = 0;
    for (i = 0; i < 4; i++) {
        self->winner[i] = -1;
    }
    return self;
}

void *fn_1_2C38(M509Core *self, s16 flag)
{
    if (self != 0) {
        M509_ASSERT(lbl_1_bss_28 != 0, lbl_1_data_157, lbl_1_data_13B, 0x3D);
        lbl_1_bss_28 = 0;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void *fn_1_2CF4(M509Core *self, s16 flag)
{
    s32 i;

    if (self != 0) {
        self->vtbl = lbl_1_data_1D8;
        if (mgDataTbl[GwSystem.mgNo].type != 3) {
            M509_ASSERT(lbl_1_bss_28 != 0, lbl_1_data_157, lbl_1_data_13B, 0x27);
            M509Core *core = lbl_1_bss_28;
            for (i = 0; i < 4; i++) {
                s32 player = core->winner[i];
                if (player >= 0 && _CheckFlag(0x1000F) == 0) {
                    GwPlayer[player].mgCoinBonus = 10;
                }
            }
        }
        if (self != 0) {
            M509_ASSERT(lbl_1_bss_28 != 0, lbl_1_data_157, lbl_1_data_13B, 0x3D);
            lbl_1_bss_28 = 0;
        }
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_2EB4(M509Core *self, s32 prio, s32 stackSize)
{
    OMOBJMAN *objMan = omInitObjMan(prio, stackSize);
    omGameSysInit(objMan);
    ((void (*)(M509Core *, OMOBJMAN *))self->vtbl[3])(self, objMan);
    omAddObjEx(objMan, 0, 0, 0, -1, fn_1_2FA0);
}

void fn_1_2F38(M509Core *self)
{
    OMOBJMAN *objMan = MgActorObjectSetup();
    ((void (*)(M509Core *, OMOBJMAN *))self->vtbl[3])(self, objMan);
    omAddObjEx(objMan, 0, 0, 0, -1, fn_1_2FA0);
}

void fn_1_2FA0(OMOBJ *obj)
{
    if (omSysExitReq != 0 && WipeCheckEnd() == 0) {
        fn_1_3848();
        HuAudFadeOut(1000);
        obj->objFunc = fn_1_30A0;
        return;
    }
    M509Core *core = lbl_1_bss_28;
    M509Core *core2 = core;
    fn_1_3030(core2);
}

void fn_1_3030(M509Core *self)
{
    ((void (*)(M509Core *))self->vtbl[4])(self);
    bool hit = false;
    if (self->delay > 0) {
        self->delay--;
        if (self->delay == 0) {
            hit = true;
        }
    }
}

void fn_1_309C(void)
{
}

void fn_1_30A0(OMOBJ *obj)
{
    M509Core *core = lbl_1_bss_28;
    M509Core *core2 = core;
    if (WipeCheckEnd() != 0) {
        fn_1_3030(core2);
        return;
    }
    ((void (*)(M509Core *))core2->vtbl[5])(core2);
    core = core2;
    if (core != 0) {
        core = (M509Core *)((void *(*)(M509Core *, s32))core->vtbl[2])(core, 1);
    }
    CharModelKill(-1);
    omOvlReturnEx(1, 1);
}

void fn_1_318C(s32 player)
{
    s32 i;

    M509_ASSERT(lbl_1_bss_28 != 0, lbl_1_data_157, lbl_1_data_13B, 0x27);
    s32 *winner = lbl_1_bss_28->winner;
    for (i = 0; i < 4; i++) {
        if (winner[i] < 0) {
            winner[i] = player;
            return;
        }
    }
}

s32 fn_1_3260(s32 player)
{
    s32 i;

    M509_ASSERT(lbl_1_bss_28 != 0, lbl_1_data_157, lbl_1_data_13B, 0x27);
    for (i = 0; i < 4; i++) {
        if (player == lbl_1_bss_28->winner[i]) {
            return 1;
        }
    }
    return 0;
}

void fn_1_3334(void)
{
    M509_ASSERT(lbl_1_bss_28 != 0, lbl_1_data_157, lbl_1_data_13B, 0x27);
    M509Core *core = lbl_1_bss_28;
    M509Core *core2 = core;
    core2->gameMes = GameMesCreate(2, 0);
}

void fn_1_33EC(void)
{
    M509_ASSERT(lbl_1_bss_28 != 0, lbl_1_data_157, lbl_1_data_13B, 0x27);
    M509Core *core = lbl_1_bss_28;
    M509Core *core2 = core;
    core2->gameMes = GameMesCreate(2, 1);
}

s32 fn_1_34A4(void)
{
    M509_ASSERT(lbl_1_bss_28 != 0, lbl_1_data_157, lbl_1_data_13B, 0x27);
    M509Core *core = lbl_1_bss_28;
    bool done = false;
    if (GameMesStatGet(-1) == 0 && (u8)(core->delay <= 0) != 0) {
        done = true;
    }
    return done;
}

void fn_1_3578(void)
{
    s32 chars[4];
    s32 count;
    s32 i;

    M509_ASSERT(lbl_1_bss_28 != 0, lbl_1_data_157, lbl_1_data_13B, 0x27);
    lbl_1_bss_28->delay = lbl_1_bss_8;
    s32 *winner = lbl_1_bss_28->winner;
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

void fn_1_3744(s32 rec)
{
    if (_CheckFlag(0x1000F) == 0) {
        M509_ASSERT(lbl_1_bss_28 != 0, lbl_1_data_157, lbl_1_data_13B, 0x27);
        M509Core *core = lbl_1_bss_28;
        core->gameMes = GameMesCreate(14, rec);
        HuAudJinglePlay(0x86);
    }
}

void fn_1_381C(void)
{
    WipeCreate(1, 5, 60);
}

void fn_1_3848(void)
{
    WipeCreate(2, 6, 60);
}

void fn_1_3874(void)
{
    omSysExitReq = 1;
}

void fn_1_3888(s32 mode)
{
    M509Obj *obj;

    lbl_1_bss_24 = mode;
    for (obj = (M509Obj *)lbl_1_bss_1C.next; obj != (M509Obj *)&lbl_1_bss_1C; obj = (M509Obj *)obj->node.next) {
        obj->flags |= (1 << mode);
        ((void (*)(M509Obj *, s32))obj->vtbl[3])(obj, mode);
    }
}

s32 fn_1_3920(void)
{
    M509Node *list = &lbl_1_bss_1C;
    M509Obj *first = (M509Obj *)list->next;
    M509Obj *obj = first;
    while (obj != (M509Obj *)list) {
        int mode = lbl_1_bss_24;
        if ((obj->flags & (1 << mode)) != 0) {
            return 0;
        }
        M509Obj *next = (M509Obj *)obj->node.next;
        obj = next;
    }
    return 1;
}

void fn_1_39AC(void)
{
    M509Node *list = &lbl_1_bss_1C;
    M509Obj *first = (M509Obj *)list->next;
    M509Obj *obj = first;
    while (obj != (M509Obj *)list) {
        ((void (*)(M509Obj *))obj->vtbl[4])(obj);
        M509Obj *next = (M509Obj *)obj->node.next;
        obj = next;
    }
}

void fn_1_3A1C(void)
{
}

void fn_1_3A20(void)
{
    while (!M509IsEmpty(&lbl_1_bss_1C)) {
        M509Obj *first = (M509Obj *)lbl_1_bss_1C.prev;
        M509Obj *second = first;
        M509Obj *obj = second;
        if (obj != 0) {
            obj = (M509Obj *)((void *(*)(M509Obj *, s32))obj->vtbl[2])(obj, 1);
        }
    }
}

void *fn_1_3AE8(M509Obj *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_1C4;
        if (self != 0) {
            if (self != 0) {
                self->node.next->prev = self->node.prev;
                self->node.prev->next = self->node.next;
            }
        }
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

s32 fn_1_3B70(void)
{
    return lbl_1_bss_24;
}

s16 *fn_1_3B80(s16 *model, void *data)
{
    *model = Hu3DModelCreate(data);
    return model;
}

s16 *fn_1_3BBC(s16 *model, s32 dataNum)
{
    *model = Hu3DModelCreate(HuDataSelHeapReadNum(dataNum, 0x10000000, HEAP_MODEL));
    return model;
}

s16 *fn_1_3C04(s16 *model, void (*hook)(HU3DMODEL *, Mtx *))
{
    *model = Hu3DHookFuncCreate(hook);
    return model;
}

s16 *fn_1_3C40(s16 *model, s32 charNo, s32 mode)
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
    *model = CharModelCreate(charNo, type);
    return model;
}

s16 *fn_1_3CDC(s16 *model, s16 *src)
{
    s16 srcId = *src;
    s16 id = srcId;
    *model = Hu3DModelLink(id);
    return model;
}

void fn_1_3D30(s16 *model, u32 attr)
{
    Hu3DModelAttrSet(*model, attr);
}

void fn_1_3D68(s16 *model, u32 attr)
{
    Hu3DModelAttrReset(*model, attr);
}

void fn_1_3DA0(s16 *model, OMOBJ *obj)
{
    s16 id = *model;
    Hu3DModelPosSet(id, obj->trans.x, obj->trans.y, obj->trans.z);
    Hu3DModelRotSet(id, obj->rot.x, obj->rot.y, obj->rot.z);
    Hu3DModelScaleSet(id, obj->scale.x, obj->scale.y, obj->scale.z);
}

void fn_1_3E1C(s16 *model, Vec *pos)
{
    Hu3DModelPosSet(*model, pos->x, pos->y, pos->z);
}

void fn_1_3E64(s16 *model, Vec *rot)
{
    Hu3DModelRotSet(*model, rot->x, rot->y, rot->z);
}

void fn_1_3EAC(s16 *model, Vec *scale)
{
    Hu3DModelScaleSet(*model, scale->x, scale->y, scale->z);
}

void fn_1_3EF4(s16 *model, f32 scale)
{
    Hu3DModelScaleSet(*model, scale, scale, scale);
}

void fn_1_3F3C(s16 *model, f32 lvl)
{
    Hu3DModelTPLvlSet(*model, lvl);
}

void fn_1_3F74(s16 *model, s16 layer)
{
    Hu3DModelLayerSet(*model, layer);
}

void fn_1_3FAC(s16 *model, u16 camBit)
{
    Hu3DModelCameraSet(*model, camBit);
}

void fn_1_3FE4(s16 *model, void *data)
{
    Hu3DData[*model].hookData = data;
}

f32 *fn_1_4000(s16 *model)
{
    return Hu3DData[*model].mtx[0];
}

s16 *fn_1_401C(s16 *model, s16 flag)
{
    if (model != 0) {
        Hu3DModelKill(*model);
        if (flag > 0 && model != 0) {
            HuMemDirectFree(model);
        }
    }
    return model;
}

HSFOBJECT *fn_1_4078(s16 *model, char *name)
{
    HSFOBJECT *obj = Hu3DModelObjPtrGet(*model, name);
    M509_ASSERT(obj != 0, lbl_1_data_171, lbl_1_data_167, 0x8A);
    return obj;
}

void fn_1_410C(s16 *model)
{
    Hu3DModelShadowSet(*model);
}

void fn_1_413C(s16 *model)
{
    Hu3DModelShadowMapSet(*model);
}

void fn_1_416C(s16 *model, char *name)
{
    Hu3DModelShadowMapObjSet(*model, name);
}

void fn_1_41A4(s16 *model)
{
    Hu3DModelShadowMapObjReset(*model);
}

void fn_1_41D4(s16 *model, char *name)
{
    HSFOBJECT *obj = fn_1_4078(model, name);
    *(u32 *)obj->constData &= ~8;
}

void fn_1_4280(s16 *model, char *name)
{
    HSFOBJECT *obj = fn_1_4078(model, name);
    obj->flags |= 0x400;
}

void fn_1_4328(s16 *model, char *name)
{
    HSFOBJECT *obj = fn_1_4078(model, name);
    obj->flags &= ~0x400;
}

void fn_1_43D0(s16 *model, char *name)
{
    HSFOBJECT *obj = fn_1_4078(model, name);
    obj->flags |= 0x400;
    obj->mesh.base.scale.x = 0.0f;
    obj->mesh.base.scale.y = 0.0f;
    obj->mesh.base.scale.z = 0.0f;
}

void fn_1_44AC(s16 *model, char *name)
{
    HSFOBJECT *obj = fn_1_4078(model, name);
    obj->flags &= ~0x400;
    obj->mesh.base.scale.x = 1.0f;
    obj->mesh.base.scale.y = 1.0f;
    obj->mesh.base.scale.z = 1.0f;
}

void fn_1_4588(s16 *model, s32 time)
{
    Hu3DMotionTimeSet(*model, (f32)time);
}

void fn_1_45F4(M509Cam *cam, f32 yaw, f32 pitch)
{
    Mtx rotA;
    Mtx rotB;
    Mtx rot;
    Vec right;
    Vec up;
    Vec fwd;

    fn_1_4724(cam, &right, &up, &fwd);
    PSMTXRotAxisRad(rotA, &right, M509_DEG2RAD * yaw);
    PSMTXRotRad(rotB, 'Y', M509_DEG2RAD * pitch);
    PSMTXConcat(rotA, rotB, rot);
    PSMTXMultVecSR(rot, &fwd, &fwd);
    PSMTXMultVecSR(rot, &up, &cam->up);
    PSVECSubtract(&cam->pos, &fwd, &cam->target);
}

void fn_1_46B8(M509Cam *cam, f32 angle)
{
    Mtx rot;
    Vec dir;

    PSVECSubtract(&cam->target, &cam->pos, &dir);
    PSMTXRotAxisRad(rot, &dir, M509_DEG2RAD * angle);
    PSMTXMultVec(rot, &cam->up, &cam->up);
}

void fn_1_4724(M509Cam *cam, Vec *a, Vec *b, Vec *c)
{
    Mtx mtx;

    C_MTXLookAt(mtx, &cam->pos, &cam->up, &cam->target);
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

void fn_1_47CC(M509Cam *cam, Mtx mtx)
{
    C_MTXLookAt(mtx, &cam->pos, &cam->up, &cam->target);
    PSMTXInverse(mtx, mtx);
}

void fn_1_4820(M509Cam *cam, Vec *offset)
{
    Mtx mtx;
    Vec world;

    C_MTXLookAt(mtx, &cam->pos, &cam->up, &cam->target);
    PSMTXInverse(mtx, mtx);
    PSMTXMultVecSR(mtx, offset, &world);
    fn_1_4888(cam, &world);
}

void fn_1_4888(M509Cam *cam, Vec *offset)
{
    PSVECAdd(offset, &cam->target, &cam->target);
    PSVECAdd(offset, &cam->pos, &cam->pos);
}

void fn_1_48DC(M509Cam *cam)
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

void fn_1_4970(M509Cam *cam, M509CamView *view)
{
    Mtx mtx;

    PSMTXQuat(mtx, &view->rot);
    cam->up.x = mtx[0][1];
    cam->up.y = mtx[1][1];
    cam->up.z = mtx[2][1];
    cam->pos.x = view->pos.x;
    cam->pos.y = view->pos.y;
    cam->pos.z = view->pos.z;
    cam->target.x = view->pos.x - mtx[0][2];
    cam->target.y = view->pos.y - mtx[1][2];
    cam->target.z = view->pos.z - mtx[2][2];
}

void fn_1_4A10(M509Cam *cam, M509CamView *view)
{
    Mtx mtx;

    C_MTXLookAt(mtx, &cam->pos, &cam->up, &cam->target);
    PSMTXInverse(mtx, mtx);
    view->pos.x = cam->pos.x;
    view->pos.y = cam->pos.y;
    view->pos.z = cam->pos.z;
    C_QUATMtx(&view->rot, mtx);
}

void fn_1_4A88(M509Cam *cam, M509CamView *a, M509CamView *b, f32 t)
{
    Mtx mtx;
    Quaternion q;
    Vec pos;
    Vec scaledB;
    Vec scaledA;

    C_QUATSlerp(&a->rot, &b->rot, &q, t);
    PSVECScale(&a->pos, &scaledA, 1.0f - t);
    PSVECScale(&b->pos, &scaledB, t);
    PSVECAdd(&scaledA, &scaledB, &pos);
    PSMTXQuat(mtx, &q);
    cam->up.x = mtx[0][1];
    cam->up.y = mtx[1][1];
    cam->up.z = mtx[2][1];
    cam->pos.x = pos.x;
    cam->pos.y = pos.y;
    cam->pos.z = pos.z;
    cam->target.x = pos.x - mtx[0][2];
    cam->target.y = pos.y - mtx[1][2];
    cam->target.z = pos.z - mtx[2][2];
}

void fn_1_4B98(M509Cam *cam, Vec *pos)
{
    Vec delta;

    PSVECSubtract(pos, &cam->pos, &delta);
    cam->pos.x = pos->x;
    cam->pos.y = pos->y;
    cam->pos.z = pos->z;
    PSVECAdd(&cam->target, &delta, &cam->target);
}

void fn_1_4C04(M509Cam *cam, Vec *a, Vec *b, f32 t)
{
    Vec delta;
    Vec scaledB;
    Vec scaledA;

    PSVECScale(a, &scaledA, 1.0f - t);
    PSVECScale(b, &scaledB, t);
    PSVECAdd(&scaledA, &scaledB, &scaledB);
    PSVECSubtract(&scaledB, &cam->pos, &delta);
    cam->pos.x = scaledB.x;
    cam->pos.y = scaledB.y;
    cam->pos.z = scaledB.z;
    PSVECAdd(&cam->target, &delta, &cam->target);
}

void fn_1_4CBC(M509Cam *cam, u16 camNo)
{
    Hu3DCameraPosSetV(camNo, &cam->pos, &cam->up, &cam->target);
}

void fn_1_4CFC(M509Cam *cam, u16 camNo)
{
    Hu3DCameraPosGet(camNo, &cam->pos, &cam->up, &cam->target);
}

MGTIMER **fn_1_4D3C(MGTIMER **self, s32 type)
{
    switch (type) {
    case 0:
        *self = MgTimerCreate(0);
        break;
    case 1:
        *self = MgTimerCreate(1);
        break;
    case 2:
        *self = MgTimerCreate(2);
        break;
    default:
        fn_1_0(lbl_1_data_185, lbl_1_data_17B, 0x1E);
        break;
    }
    return self;
}

MGTIMER **fn_1_4E04(MGTIMER **self, s16 flag)
{
    if (self != 0) {
        MgTimerKill(*self);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_4E60(MGTIMER **self, s32 a, s32 b, s32 c)
{
    MgTimerParamSet(*self, a, b, c);
}

void fn_1_4EA8(MGTIMER **self)
{
    MgTimerValueGet(*self);
}

void fn_1_4ED8(MGTIMER **self, f32 x, f32 y)
{
    MgTimerPosSet(*self, x, y);
}

void fn_1_4F18(MGTIMER **self, f32 *x, f32 *y)
{
    MgTimerPosGet(*self, x, y);
}

void fn_1_4F58(MGTIMER **self, s32 mode)
{
    switch (mode) {
    case 0:
        MgTimerModeOnSet(*self, 0);
        return;
    case 1:
        MgTimerModeOnSet(*self, 1);
        return;
    case 2:
        MgTimerModeOnSet(*self, 2);
        return;
    default:
        fn_1_0(lbl_1_data_185, lbl_1_data_17B, 0x48);
        return;
    }
}

void fn_1_501C(MGTIMER **self)
{
    MgTimerModeOffSet(*self);
}

void fn_1_504C(MGTIMER **self)
{
    if ((*self)->stopF == 0) {
        (*self)->stopF = 1;
        (*self)->mode = 1;
    }
}

void fn_1_5078(MGTIMER **self)
{
    MgTimerKill(*self);
    *self = MgTimerCreate(0);
}

u8 fn_1_50B4(MGTIMER **self)
{
    return (u8)(MgTimerDoneCheck(*self) != 0);
}

void fn_1_50F4(MGTIMER **self, u8 r, u8 g, u8 b)
{
    MgTimerWinColorSet(*self, r, g, b);
}

void fn_1_513C(M509Cam *cam)
{
    Mtx mtx;
    Vec pos;
    Quaternion q;

    C_MTXLookAt(mtx, &cam->pos, &cam->up, &cam->target);
    PSMTXInverse(mtx, mtx);
    pos.x = cam->pos.x;
    pos.y = cam->pos.y;
    pos.z = cam->pos.z;
    C_QUATMtx(&q, mtx);
    printf(lbl_1_data_18B, f32Bits(pos.x), f32Bits(pos.y), f32Bits(pos.z), f32Bits(q.x), f32Bits(q.y), f32Bits(q.z), f32Bits(q.w));
}

f32 fn_1_51D4(s32 pad)
{
    return 0.017857144f * (f32)HuPadStkX[pad];
}

f32 fn_1_5228(s32 pad)
{
    return 0.017857144f * (f32)HuPadStkY[pad];
}

f32 fn_1_527C(s32 pad)
{
    return 0.016949153f * (f32)HuPadSubStkX[pad];
}

f32 fn_1_52D0(s32 pad)
{
    return 0.016949153f * (f32)HuPadSubStkY[pad];
}

void fn_1_5324(M509Cam *cam, s32 pad)
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
        cam->up.x = 0.0f;
        cam->up.y = 1.0f;
        cam->up.z = 0.0f;
    }
    yaw = -4.0f * (0.017857144f * (f32)HuPadStkX[pad]);
    pitch = -4.0f * (0.017857144f * (f32)HuPadStkY[pad]);
    C_MTXLookAt(mtx, &cam->pos, &cam->up, &cam->target);
    right.x = mtx[0][0];
    right.y = mtx[0][1];
    right.z = mtx[0][2];
    up.x = mtx[1][0];
    up.y = mtx[1][1];
    up.z = mtx[1][2];
    fwd.x = mtx[2][0];
    fwd.y = mtx[2][1];
    fwd.z = mtx[2][2];
    PSMTXRotAxisRad(rotA, &right, M509_DEG2RAD * pitch);
    PSMTXRotRad(rotB, 'Y', M509_DEG2RAD * yaw);
    PSMTXConcat(rotA, rotB, rot);
    PSMTXMultVecSR(rot, &fwd, &fwd);
    PSMTXMultVecSR(rot, &up, &cam->up);
    PSVECSubtract(&cam->pos, &fwd, &cam->target);
    if ((u8)((btn & 0x200) != 0) != 0) {
        speed = 70.0f;
    } else {
        speed = 20.0f;
    }
    C_MTXLookAt(inv, &cam->pos, &cam->up, &cam->target);
    PSMTXInverse(inv, inv);
    move.x = (speed * (f32)(u8)((btn & 2) != 0)) - (speed * (f32)(u8)((btn & 1) != 0));
    move.y = (speed * (f32)(u8)((btn & 8) != 0)) - (speed * (f32)(u8)((btn & 4) != 0));
    move.x = move.x + (speed * (0.016949153f * (f32)HuPadSubStkX[pad]));
    move.z = 0.0f;
    PSMTXMultVecSR(inv, &move, &move);
    move.y = move.y + (speed * (0.016949153f * (f32)HuPadSubStkY[pad]));
    PSVECAdd(&move, &cam->target, &cam->target);
    PSVECAdd(&move, &cam->pos, &cam->pos);
    fwdMove.x = 0.0f;
    fwdMove.y = 0.0f;
    fwdMove.z = (f32)((u8)((btn & 0x400) != 0) - (u8)((btn & 0x100) != 0));
    PSMTXMultVecSR(inv, &fwdMove, &fwdMove);
    fwdMove.y = 0.0f;
    f32 mag = PSVECMag(&fwdMove);
    if (mag > 1e-10f) {
        PSVECScale(&fwdMove, &fwdMove, speed / mag);
        PSVECAdd(&fwdMove, &cam->target, &cam->target);
        PSVECAdd(&fwdMove, &cam->pos, &cam->pos);
    }
    if ((HuPadBtnDown[pad] & 0x200) != 0) {
        Mtx dump;
        Vec pos;

        C_MTXLookAt(dump, &cam->pos, &cam->up, &cam->target);
        PSMTXInverse(dump, dump);
        pos.x = cam->pos.x;
        pos.y = cam->pos.y;
        pos.z = cam->pos.z;
        C_QUATMtx(&q, dump);
        printf(lbl_1_data_18B, f32Bits(pos.x), f32Bits(pos.y), f32Bits(pos.z), f32Bits(q.x), f32Bits(q.y), f32Bits(q.z), f32Bits(q.w));
    }
}

void fn_1_5980(void)
{
    lbl_1_bss_4 = 0x78;
    lbl_1_bss_8 = 0xD2;
    lbl_1_bss_C = 0xD2;
    lbl_1_bss_1C.next = &lbl_1_bss_1C;
    lbl_1_bss_1C.prev = &lbl_1_bss_1C;
    __register_global_object(&lbl_1_bss_1C, (void *(*)(void *, s16))fn_1_5A08, lbl_1_bss_10);
}

void *fn_1_5A08(M509Obj *self, s16 flag)
{
    if (self != 0) {
        if (self != 0) {
            self->node.next->prev = self->node.prev;
            self->node.prev->next = self->node.next;
        }
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}
