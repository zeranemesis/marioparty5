#include "dolphin/types.h"

class CFrame {
public:
    CFrame(s32 frame) { m_frame = frame; }
    s32 m_frame;
};

extern "C" {
void *fn_1_46CC(u32 size);
void *fn_1_8248(void *self);
void fn_1_2284(void *self, s32 arg1, s32 arg2);

CFrame lbl_1_bss_A20(120);
CFrame lbl_1_bss_A24(210);
CFrame lbl_1_bss_A28(210);

void ObjectSetup(void)
{
    void *mem;
    void *master;

    mem = fn_1_46CC(0x380);
    if (mem) {
        mem = fn_1_8248(mem);
    }
    master = mem;
    fn_1_2284(master, 1, 0x2000);
}
}
