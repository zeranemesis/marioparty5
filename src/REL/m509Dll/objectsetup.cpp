#include "dolphin/types.h"

class CFrame {
public:
    CFrame(s32 frame) { m_frame = frame; }
    s32 m_frame;
};

extern "C" {
typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

void ObjectSetup(void);

void *fn_1_282C(u32 size);
void *fn_1_C2D0(void *self);
void fn_1_2EB4(void *self, s32 arg1, s32 arg2);

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

CFrame lbl_1_bss_58(120);
CFrame lbl_1_bss_5C(210);
CFrame lbl_1_bss_60(210);

void ObjectSetup(void)
{
    void *mem;
    void *master;

    mem = fn_1_282C(0xD20);
    if (mem) {
        mem = fn_1_C2D0(mem);
    }
    master = mem;
    fn_1_2EB4(master, 1, 0x2000);
}
}
