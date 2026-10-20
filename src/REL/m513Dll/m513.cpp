extern "C" {
#include "REL/m513Dll.h"
#include "dolphin/os.h"

SHARED_SYM extern s16 omSysExitReq;

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

void ObjectSetup(void);
void fn_1_100(void *);

s32 sprintf(char *str, const char *fmt, ...);

void fn_1_1D4(char *expr, char *file, int line)
{
    char buf[0x40C];

    sprintf(buf, "\nASSERTION FAILED at %s:%d\n%s", file, line, expr);
    OSPanic("assert.cpp", 0x12, buf);
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

void *fn_1_0(u32 size);

void *fn_1_0(u32 size)
{
    void *ptr = HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);

    if (ptr == 0) {
        fn_1_1D4("ptr != NULL", "new.cpp", 0x17);
    }
    return ptr;
}

void _prolog(void)
{
    const VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
}

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

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

void fn_1_1768(M513_OBJHEAD *node)
{
    node->next = node;
    node->prev = node;
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

void fn_1_22E0(void)
{
}

void fn_1_2970(void)
{
}

void fn_1_4DC4(void)
{
}

s32 fn_1_5748(void)
{
    return OSGetThreadPriority(OSGetCurrentThread());
}

}