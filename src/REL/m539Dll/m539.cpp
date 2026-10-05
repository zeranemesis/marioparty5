#include "REL/m539Dll.h"

extern s16 omSysExitReq;

extern "C" {

typedef struct M539Hu3DData_s {
    u8 unk_000[0x120];
    void *hookData;
    u8 unk_124[0x134 - 0x124];
} M539Hu3DData;

extern M539Hu3DData Hu3DData[];

void fn_1_2C44(void)
{
    omSysExitReq = 1;
}

// NONMATCHING: register allocation - uses r31/stack frame instead of r0
void fn_1_30BC(void *arg0, void *arg1)
{
    s16 index = *(s16 *)arg0;
    Hu3DData[index].hookData = arg1;
}

// NONMATCHING: register allocation - uses r31/stack frame instead of r0
void *fn_1_30D8(void *arg0)
{
    s16 index = *(s16 *)arg0;
    return (u8 *)&Hu3DData[index] + 0xF0;
}

typedef struct M539UnkStructA_s {
    void *vtable;
    u8 unk_04[0x10];
    void (*func_14)(void *, void *);
    u8 unk_18[0x44];
    void *unk_5C;
} M539UnkStructA;

typedef struct M539UnkStructB_s {
    u8 unk_00[0x14];
    void (*func_14)(void *, void *);
    u8 unk_18[0x44];
    M539UnkStructA *unk_5C;
} M539UnkStructB;

extern void *lbl_1_data_1D4[];
extern void fn_1_4874(void *, void *);
extern void fn_1_48C0(void);
extern void fn_1_4934(void *, s16);

void fn_1_4854(M539UnkStructA *arg0, M539UnkStructB *arg1)
{
    arg0->vtable = lbl_1_data_1D4;
    arg1->unk_5C = arg0;
    arg1->func_14 = fn_1_4874;
}

typedef struct M539ListNode_s {
    struct M539ListNode_s *prev;
    struct M539ListNode_s *next;
} M539ListNode;

// NONMATCHING: register allocation - uses stack frame and r31/r30 instead of r0/r4
void fn_1_12DC(M539ListNode *node)
{
    M539ListNode *prev = node->prev;
    M539ListNode *next = node->next;

    prev->next = next;
    next->prev = prev;
    node->prev = node;
    node->next = node;
}

void *OSGetCurrentThread(void);
s32 OSGetThreadPriority(void *thread);

// NONMATCHING: register allocation - saves r31 unnecessarily
s32 fn_1_5568(void)
{
    void *thread = OSGetCurrentThread();
    return OSGetThreadPriority(thread);
}

}