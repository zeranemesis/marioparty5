#define HuMemDirectFree m522_HuMemDirectFree_hdrdecl
#include "REL/m522Dll.h"
#include "game/hu3d.h"
#include "dolphin/os/OSThread.h"
#include "dolphin/os.h"
#undef HuMemDirectFree

extern "C" void HuMemDirectFree(void *ptr);
extern "C" s32 sprintf(char *str, const char *fmt, ...);

#define M522_ASSERT(expr, file, line) \
    do { \
        char m522AssertBuf[0x40C]; \
        sprintf(m522AssertBuf, "\nASSERTION FAILED at %s:%d\n%s", file, line, expr); \
        OSPanic("assert.cpp", 0x12, m522AssertBuf); \
    } while (0)

extern s16 omSysExitReq;

extern "C" void fn_1_99E4(void)
{
    omSysExitReq = 1;
}

extern "C" void fn_1_9E5C(s16 *index, u32 value)
{
    Hu3DData[*index].hookData = (void *)value;
}

extern "C" Mtx *fn_1_9E78(s16 *index)
{
    return &Hu3DData[*index].mtx;
}

typedef void (*VFN)(void *);

extern "C" VFN lbl_1_data_380[4];

extern "C" void fn_1_B614(void *);
extern "C" void fn_1_B660(void *);
extern "C" void fn_1_B6D4(void *);

extern "C" void fn_1_B5F4(void *a, void *b)
{
    *(VFN **)a = lbl_1_data_380;
    *(void **)((u8 *)b + 0x5C) = a;
    *(VFN *)((u8 *)b + 0x14) = fn_1_B614;
}

typedef struct ListNode {
    struct ListNode *prev;
    struct ListNode *next;
} ListNode;

extern "C" void fn_1_807C(ListNode *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->prev = node;
    node->next = node;
}

extern "C" s32 fn_1_BDBC(void)
{
    return OSGetThreadPriority(OSGetCurrentThread());
}

extern "C" void fn_1_8004(ListNode *node)
{
    node->prev = node;
    node->next = node;
}

extern "C" void fn_1_80A0(ListNode *node, ListNode *other)
{
    ListNode *o = other;
    node->next = o;
    node->prev = o->prev;
    o->prev->next = node;
    o->prev = node;
}

extern "C" void fn_1_80D0(ListNode *node, ListNode *other)
{
    ListNode *o = other;
    node->prev = o;
    node->next = o->next;
    o->next->prev = node;
    o->next = node;
}

extern "C" void fn_1_8B7C(void)
{
}

extern u32 lbl_1_bss_34;

extern "C" void *fn_1_8CD0(void)
{
    return (void *)lbl_1_bss_34;
}

extern "C" void fn_1_920C(void)
{
}

extern "C" void WipeCreate(s16 mode, s16 type, s16 maxTime);

extern "C" void fn_1_998C(void)
{
    WipeCreate(1, 5, 0x3c);
}

extern "C" void fn_1_99B8(void)
{
    WipeCreate(2, 6, 0x3c);
}

extern "C" void fn_1_9F84(s16 *arg0)
{
    Hu3DModelShadowSet(arg0[0]);
}

extern "C" void fn_1_9FB4(s16 *arg0)
{
    Hu3DModelShadowMapSet(arg0[0]);
}

extern "C" void fn_1_A01C(s16 *arg0)
{
    Hu3DModelShadowMapObjReset(arg0[0]);
}

extern "C" void fn_1_B660(void *)
{
}

struct MgTimer_s;

extern "C" int MgTimerValueGet(struct MgTimer_s *timer);
extern "C" void MgTimerModeOffSet(struct MgTimer_s *timer);

extern "C" void fn_1_C784(struct MgTimer_s **arg0)
{
    MgTimerValueGet(*arg0);
}

extern "C" void fn_1_C8F8(struct MgTimer_s **arg0)
{
    MgTimerModeOffSet(*arg0);
}

typedef struct M522State0_s {
    s32 f00;
    u8 pad04[0x10];
    s32 f14;
} M522State0;

extern "C" void fn_1_C928(M522State0 **self)
{
    if ((*self)->f14 == 0) {
        (*self)->f14 = 1;
        (*self)->f00 = 1;
    }
}

typedef struct M522Node_s {
    struct M522Node_s *f00;
    struct M522Node_s *f04;
    u32 f08;
    u32 f0C;
    u16 f0E_hi : 4;
    u16 f0E_count : 12;
} M522Node;

extern "C" void HuMemDirectFree(void *ptr);

extern "C" s32 fn_1_7F38(M522Node *self)
{
    s32 count = 0;
    M522Node *n = self->f00;
    while (n != self) {
        n = n->f00;
        count++;
    }
    return count;
}

extern "C" s32 fn_1_7F74(M522Node *self)
{
    s32 count;
    M522Node *head = self->f04;
    count = 0;
    M522Node *n = head->f00;
    while (n != head) {
        n = n->f00;
        count++;
    }
    return count;
}

extern "C" s32 fn_1_7FBC(M522Node *self)
{
    s32 count;
    M522Node *head = self->f00;
    count = 0;
    M522Node *n = head->f00;
    while (n != head) {
        n = n->f00;
        count++;
    }
    return count;
}

extern "C" M522Node *fn_1_8010(M522Node *node, s16 arg1)
{
    if (node != 0) {
        node->f00->f04 = node->f04;
        node->f04->f00 = node->f00;
        if (arg1 > 0) {
            if (node != 0) {
                HuMemDirectFree(node);
            }
        }
    }
    return node;
}

extern "C" M522Node *fn_1_7D04(M522Node *node, s16 arg1)
{
    if (node != 0) {
        M522Node *child = node->f00;
        if (child != 0) {
            HuMemDirectFree(child);
        }
        if (arg1 > 0) {
            if (node != 0) {
                HuMemDirectFree(node);
            }
        }
    }
    return node;
}

extern "C" u32 fn_1_7EC8(M522Node *self, u32 arg1)
{
    if (arg1 == 0) {
        return 0;
    }
    if ((arg1 >> 12) >= self->f08) {
        return 0;
    }
    u8 *e = (u8 *)self->f00 + ((arg1 >> 12) << 4);
    if (*(u32 *)(e + 0xC) != arg1) {
        return 0;
    }
    return *(u32 *)(e + 0x8);
}

extern "C" u32 fn_1_7D74(M522Node *self, u32 arg1)
{
    M522Node *head = self->f00;
    M522Node *n = head->f00;
    if (n == head) {
        return 0;
    }
    n->f00->f04 = n->f04;
    n->f04->f00 = n->f00;
    M522Node *dst = self->f04;
    n->f00 = dst->f00;
    n->f04 = dst;
    dst->f00->f04 = n;
    dst->f00 = n;
    n->f08 = arg1;
    n->f0E_count = n->f0E_count + 1;
    return n->f0C;
}

typedef struct M522ModelRef_s {
    HU3DMODELID modelId;
} M522ModelRef;

extern "C" void *fn_1_9E94(M522ModelRef *arg0, s16 arg1)
{
    if (arg0 != 0) {
        Hu3DModelKill(arg0->modelId);
        if (arg1 > 0) {
            if (arg0 != 0) {
                HuMemDirectFree(arg0);
            }
        }
    }
    return arg0;
}

extern "C" HSFOBJECT *fn_1_9EF0(M522ModelRef *arg0, char *name)
{
    HSFOBJECT *p = Hu3DModelObjPtrGet(arg0->modelId, name);
    if (p == 0) {
        M522_ASSERT("p != NULL", "model.cpp", 0x8a);
    }
    return p;
}