#include "REL/m513Dll.h"
#include "dolphin/os.h"

extern "C" {

SHARED_SYM extern s16 omSysExitReq;

void fn_1_3148(void)
{
    omSysExitReq = 1;
}

typedef void *Hu3DParm;

void fn_1_35C0(s16 *modelId, Hu3DParm parm)
{
    Hu3DData[*modelId].hookData = parm;
}

void *fn_1_35DC(s16 *modelId)
{
    return (u8 *)&Hu3DData[*modelId] + 0xF0;
}

extern void *lbl_1_data_1F0[4];
extern void fn_1_4D78(void *, void *);
extern void fn_1_4E38(void *, s16);
extern void fn_1_4DC4(void);

void fn_1_4D58(void **obj, void *arg1)
{
    obj[0] = lbl_1_data_1F0;
    ((void **)arg1)[0x5C / 4] = obj;
    ((void **)arg1)[0x14 / 4] = fn_1_4D78;
}

#pragma push
#pragma optimization_level 0
#pragma peephole off

void fn_1_17E0(M513_OBJHEAD *node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;
    node->next = node;
    node->prev = node;
}

#pragma pop

s32 fn_1_5748(void)
{
    return OSGetThreadPriority(OSGetCurrentThread());
}

}