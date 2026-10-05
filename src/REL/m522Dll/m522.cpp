#include "REL/m522Dll.h"
#include "game/hu3d.h"
#include "dolphin/os/OSThread.h"

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

// NONMATCHING: load order of node->prev/node->next differs from original
extern "C" void fn_1_807C(ListNode *node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;
    node->prev = node;
    node->next = node;
}

extern "C" s32 fn_1_BDBC(void)
{
    return OSGetThreadPriority(OSGetCurrentThread());
}