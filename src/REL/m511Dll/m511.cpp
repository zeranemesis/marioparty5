#include "dolphin.h"
#include "game/object.h"
#include "game/hu3d.h"

extern "C" {

extern void fn_1_530C(void);
extern void *lbl_1_data_22C[];

void fn_1_36DC(void)
{
    omSysExitReq = 1;
}

void fn_1_3B54(s16 *modelIdPtr, void *data)
{
    *(void **)((u8 *)&Hu3DData[*modelIdPtr] + 0x120) = data;
}

void *fn_1_3B70(s16 *modelIdPtr)
{
    return (void *)((u8 *)&Hu3DData[*modelIdPtr] + 0xF0);
}

void fn_1_52EC(void **outPtr, void *structPtr)
{
    *outPtr = lbl_1_data_22C;
    *(void **)((u8 *)structPtr + 0x5C) = outPtr;
    *(void **)((u8 *)structPtr + 0x14) = fn_1_530C;
}

typedef struct ListNode {
    struct ListNode *prev;
    struct ListNode *next;
} ListNode;

void fn_1_1E00(ListNode *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->prev = node;
    node->next = node;
}

s32 fn_1_5CDC(void)
{
    return OSGetThreadPriority(OSGetCurrentThread());
}

}
