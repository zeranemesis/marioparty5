extern "C" {

#ifndef TARGET_PC
int sprintf(char *buf, const char *fmt, ...);
int printf(const char *fmt, ...);
void OSInitSemaphore(OSSemaphore *sem, s32 count);
s32 OSWaitSemaphore(OSSemaphore *sem);
s32 OSSignalSemaphore(OSSemaphore *sem);
void __register_global_object(void *obj, void *(*dtor)(void *, s16), void *reg);
#endif

/* defined by the lower fragments */
extern char lbl_1_data_0[];
extern char lbl_1_data_C[];
extern char lbl_1_data_14[];
extern char lbl_1_data_32[];
extern M579Node lbl_1_bss_C;
extern s32 lbl_1_bss_1C;
void fn_1_36CC(M579Mt *mt, s32 seed);
u32 fn_1_3904(M579Mt *mt);
f64 fn_1_4184(M579Mt *mt);

/* the inlined assertion handler of the original */
#define M579D_ASSERT_INL(cond, expr, file, line) \
    if (cond) { \
        (void)0; \
    } else { \
        char assertBuf[0x400]; \
        sprintf(assertBuf, lbl_1_data_14, (file), (line), (expr)); \
        OSPanic(lbl_1_data_32, 0x12, assertBuf); \
    }


typedef struct M579DFunctor {
    void **vtbl;
} M579DFunctor;

typedef struct M579DCircle {
    Vec pos;
    f32 radius;
} M579DCircle;


typedef void *(*M579DDtorFn)(void *, s16);
typedef void (*M579DRunFn)(void *, OMOBJ *);

char lbl_1_data_B6[7] = "om.cpp";
char lbl_1_data_BD[14] = "subthread.cpp";
char lbl_1_data_CB[31] = "::OSGetCurrentThread() == m_th";
char lbl_1_data_EA[30] = "!::OSIsThreadTerminated(m_th)";
char lbl_1_data_108[11] = "thread.cpp";
char lbl_1_data_113[59] = "priority >= OS_PRIORITY_MIN && priority <= OS_PRIORITY_MAX";
char lbl_1_data_14E[25] = "OSCreateThread() failed.";
char lbl_1_data_167[61] = "delete CThread@%p\nstack remain/total = 0x%08x/0x%08x (%f%%)\n";
char lbl_1_data_1A4[16] = "stack overflow\n";
char lbl_1_data_1B4[10] = "timer.cpp";
char lbl_1_data_1BE[6] = "false";

u8 lbl_1_bss_0[12];
s32 lbl_1_bss_18;
s32 lbl_1_bss_20;
M579Mt lbl_1_bss_24;
u8 lbl_1_bss_9E8;

void *fn_1_552C(void *arg);
void *fn_1_52E4(void *self, s16 flag);
void *fn_1_4934(void *self, s16 flag);
void fn_1_48C0(void *self, OMOBJ *obj);
void fn_1_4874(OMOBJ *obj);
void *fn_1_6000(void *self, s16 flag);

void *lbl_1_data_1C4[4] = { 0, 0, (void *)fn_1_52E4, 0 };
void *lbl_1_data_1D4[4] = { 0, 0, (void *)fn_1_4934, (void *)fn_1_48C0 };

void *fn_1_46CC(u32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M579D_ASSERT_INL(p != 0, lbl_1_data_0, lbl_1_data_B6, 0x18);
    return p;
}

void *fn_1_4758(u32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    M579D_ASSERT_INL(p != 0, lbl_1_data_0, lbl_1_data_B6, 0x1F);
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

void fn_1_4854(M579DFunctor *self, OMOBJ *obj)
{
    self->vtbl = lbl_1_data_1D4;
    obj->data = self;
    obj->objFunc = fn_1_4874;
}

void fn_1_4874(OMOBJ *obj)
{
    M579DFunctor *self = (M579DFunctor *)obj->data;

    ((M579DRunFn)self->vtbl[3])(self, obj);
}

void fn_1_48C0(void *self, OMOBJ *obj)
{
}

void fn_1_48C4(OMOBJ *obj)
{
    M579DFunctor *p = (M579DFunctor *)obj->data;

    if (p != 0) {
        ((M579DDtorFn)p->vtbl[2])(p, 1);
    }
    omDelObjEx(HuPrcCurrentGet(), obj);
}

void *fn_1_4934(void *self, s16 flag)
{
    if (self != 0) {
        ((M579DFunctor *)self)->vtbl = lbl_1_data_1D4;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_4994(OMOBJMAN *man, OMOBJ *obj)
{
    M579DFunctor *p = (M579DFunctor *)obj->data;

    if (p != 0) {
        ((M579DDtorFn)p->vtbl[2])(p, 1);
    }
    omDelObjEx(man, obj);
}

/* global random number generator */
void fn_1_4A08(u32 seed)
{
    fn_1_36CC(&lbl_1_bss_24, seed);
}

s32 fn_1_4AE0(void)
{
    return fn_1_3904(&lbl_1_bss_24) >> 1;
}

f32 fn_1_4D1C(void)
{
    return 1.1920929e-7f * (f32)(fn_1_3904(&lbl_1_bss_24) >> 9);
}

f32 fn_1_4F90(f32 scale)
{
    return scale * fn_1_4184(&lbl_1_bss_24);
}

f32 fn_1_4FC8(f32 from, f32 to)
{
    return from + (to - from) * fn_1_4184(&lbl_1_bss_24);
}

/* circle (x, y, z, radius) overlap test and push-apart */
s32 fn_1_501C(M579DCircle *a, M579DCircle *b)
{
    Vec delta;
    f32 sum;

    PSVECSubtract(&a->pos, &b->pos, &delta);
    sum = a->radius + b->radius;
    return PSVECMag(&delta) < sum;
}

s32 fn_1_50C4(M579DCircle *a, M579DCircle *b, f32 wa, f32 wb)
{
    Vec delta;
    Vec shift;
    f32 sum;
    f32 dist;

    sum = a->radius + b->radius;
    PSVECSubtract(&b->pos, &a->pos, &delta);
    dist = PSVECMag(&delta);
    if (dist >= sum) {
        return 0;
    }
    PSVECScale(&delta, &delta, (wb * (sum - dist)) / dist);
    PSVECScale(&delta, &shift, -wa);
    PSVECAdd(&shift, &a->pos, &a->pos);
    PSVECScale(&delta, &shift, 1.0f - wa);
    PSVECAdd(&shift, &b->pos, &b->pos);
    return 1;
}

/* CSubThread / CThread */
s32 fn_1_5244(void)
{
    return OSGetThreadPriority(OSGetCurrentThread());
}

M579DThread *fn_1_5588(M579DThread *self, void *(*func)(void *), void *arg, u32 stackSize, s32 priority, u16 attr);
void *fn_1_580C(M579DThread *self, s16 flag);
u32 fn_1_5940(M579DThread *self);
void fn_1_56E8(M579DThread *self, void *(*func)(void *), void *arg, u32 *stack, u32 words, s32 priority, u16 attr);
void fn_1_5A54(u32 *p, s32 count);
s32 fn_1_59E0(u32 *stack, s32 mode);

M579DSubThread *fn_1_5268(M579DSubThread *self, u32 stackSize)
{
    self->vtbl = lbl_1_data_1C4;
    fn_1_5588(&self->th, fn_1_552C, self, stackSize, OSGetThreadPriority(OSGetCurrentThread()), 1);
    OSInitSemaphore(&self->sema, 0);
    return self;
}

void *fn_1_52E4(void *obj, s16 flag)
{
    M579DSubThread *self = (M579DSubThread *)obj;

    if (self != 0) {
        self->vtbl = lbl_1_data_1C4;
        fn_1_580C(&self->th, -1);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

u8 fn_1_5350(M579DSubThread *self)
{
    OSThread *th = &self->th.thread;

    return OSIsThreadTerminated(th) != 0;
}

s32 fn_1_5398(M579DSubThread *self, s32 arg)
{
    OSThread *th = &self->th.thread;

    if (OSIsThreadTerminated(th) != 0) {
        return self->arg;
    }
    self->arg = arg;
    OSResumeThread(th);
    OSWaitSemaphore(&self->sema);
    fn_1_5940(&self->th);
    OSCheckActiveThreads();
    return self->arg;
}

s32 fn_1_5428(M579DSubThread *self, s32 arg)
{
    M579D_ASSERT_INL(OSGetCurrentThread() == &self->th.thread, lbl_1_data_CB, lbl_1_data_BD, 0x47);
    M579D_ASSERT_INL(OSIsThreadTerminated(&self->th.thread) == 0, lbl_1_data_EA, lbl_1_data_BD, 0x4A);
    self->arg = arg;
    OSSignalSemaphore(&self->sema);
    OSSuspendThread(&self->th.thread);
    return self->arg;
}

/* thread entry: runs the virtual thread_main(arg), then wakes the waiter */
void *fn_1_552C(void *arg)
{
    M579DSubThread *self = (M579DSubThread *)arg;

    ((void (*)(void *, s32))self->vtbl[3])(self, self->arg);
    OSSignalSemaphore(&self->sema);
    return 0;
}

M579DThread *fn_1_5588(M579DThread *self, void *(*func)(void *), void *arg, u32 stackSize, s32 priority, u16 attr)
{
    u32 words = stackSize >> 2;
    u32 *stack = (u32 *)HuMemDirectMallocNum(HEAP_HEAP, words * 4, 0x10000000);

    M579D_ASSERT_INL(stack != 0, lbl_1_data_0, lbl_1_data_C, 0x17);
    self->stack = stack;
    fn_1_56E8(self, func, arg, stack, words, priority, attr);
    return self;
}

M579DThread *fn_1_566C(M579DThread *self, void *(*func)(void *), void *arg, u32 *stack, u32 stackSize, s32 priority, u16 attr)
{
    self->stack = 0;
    fn_1_56E8(self, func, arg, stack, stackSize >> 2, priority, attr);
    return self;
}

void fn_1_56E8(M579DThread *self, void *(*func)(void *), void *arg, u32 *stack, u32 words, s32 priority, u16 attr)
{
    u8 valid = 0;

    if (priority >= 0 && priority <= 31) {
        valid = 1;
    }
    M579D_ASSERT_INL(valid != 0, lbl_1_data_113, lbl_1_data_108, 0x52);
    fn_1_5A54(stack, words);
    fn_1_59E0(stack, 0);
    if ((u8)(OSCreateThread(&self->thread, func, arg, &stack[words], words * 4, priority, attr) != 0) == 0) {
        OSPanic(lbl_1_data_108, 0x60, lbl_1_data_14E);
    }
}

void *fn_1_580C(M579DThread *self, s16 flag)
{
    if (self != 0) {
        if (OSIsThreadTerminated(&self->thread) == 0) {
            OSCancelThread(&self->thread);
        }
        if (lbl_1_bss_9E8 != 0) {
            u32 total = (u32)(self->thread.stackBase - (u8 *)self->thread.stackEnd);
            u32 remain = fn_1_5940(self);

            printf(lbl_1_data_167, (void *)self, remain, total, 100.0 * (f64)remain / (f64)total);
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

u32 fn_1_5940(M579DThread *self)
{
    u32 *base = (u32 *)self->thread.stackEnd;
    u32 *end = (u32 *)self->thread.stackBase;
    u32 *p;

    if (fn_1_59E0(base, 1) != 0) {
        OSPanic(lbl_1_data_108, 0x7F, lbl_1_data_1A4);
    }
    p = base;
    while (p < end) {
        if (*p != 0xDEADBABE) {
            break;
        }
        p++;
    }
    return (u32)(p - base) * 4;
}

/* mode 0: fill the guard words, mode 1: nonzero when they were overwritten */
s32 fn_1_59E0(u32 *stack, s32 mode)
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

void fn_1_5A54(u32 *p, s32 count)
{
    if (lbl_1_bss_9E8 != 0) {
        u32 *end = p + count;

        while (p != end) {
            *p = 0xDEADBABE;
            p++;
        }
    }
}

/* CTimer */
M579DTimer *fn_1_5AA0(M579DTimer *self, s32 type)
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
        M579D_ASSERT_INL(0, lbl_1_data_1BE, lbl_1_data_1B4, 0x1E);
        break;
    }
    return self;
}

void *fn_1_5B68(M579DTimer *self, s16 flag)
{
    if (self != 0) {
        MgTimerKill(self->timer);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_5BC4(M579DTimer *self, s32 maxTime, s32 endTime, s32 recordTime)
{
    MgTimerParamSet(self->timer, maxTime, endTime, recordTime);
}

s32 fn_1_5C0C(M579DTimer *self)
{
    return MgTimerValueGet(self->timer);
}

void fn_1_5C3C(M579DTimer *self, f32 x, f32 y)
{
    MgTimerPosSet(self->timer, x, y);
}

void fn_1_5C7C(M579DTimer *self, f32 *x, f32 *y)
{
    MgTimerPosGet(self->timer, x, y);
}

void fn_1_5CBC(M579DTimer *self, s32 offType)
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
        M579D_ASSERT_INL(0, lbl_1_data_1BE, lbl_1_data_1B4, 0x48);
        return;
    }
}

void fn_1_5D80(M579DTimer *self)
{
    MgTimerModeOffSet(self->timer);
}

void fn_1_5DB0(M579DTimer *self)
{
    if (self->timer->stopF == 0) {
        self->timer->stopF = 1;
        self->timer->mode = 1;
    }
}

void fn_1_5DDC(M579DTimer *self)
{
    MgTimerKill(self->timer);
    self->timer = MgTimerCreate(0);
}

u8 fn_1_5E18(M579DTimer *self)
{
    return MgTimerDoneCheck(self->timer) != 0;
}

void fn_1_5E58(M579DTimer *self, u8 r, u8 g, u8 b)
{
    MgTimerWinColorSet(self->timer, r, g, b);
}

/* static initialiser: object list head, the three delays, the global RNG seed */
void fn_1_5EA0(void)
{
    M579Node *list = &lbl_1_bss_C;

    list->next = list;
    list->prev = list;
#ifdef __MWERKS__
    __register_global_object(list, fn_1_6000, lbl_1_bss_0);
#endif
    lbl_1_bss_18 = 0x78;
    lbl_1_bss_1C = 0xD2;
    lbl_1_bss_20 = 0xD2;
    fn_1_36CC(&lbl_1_bss_24, 5489);
}

void *fn_1_6000(void *obj, s16 flag)
{
    M579Node *self = (M579Node *)obj;

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

#ifdef __MWERKS__
__declspec(section ".ctors") void (*const lbl_ctor_5EA0)(void) = fn_1_5EA0;
#else
struct M579DStaticInit {
    M579DStaticInit() { fn_1_5EA0(); }
};
static M579DStaticInit lbl_static_init_5EA0;
#endif

} /* extern "C" */
