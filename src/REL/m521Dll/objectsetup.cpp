#include "dolphin/types.h"

class CFrame {
public:
    CFrame(s32 frame) { m_frame = frame; }
    s32 m_frame;
};

extern "C" {
void *fn_1_9134(u32 size);
void *fn_1_11520(void *self);
void fn_1_6CEC(void *self, s32 arg1, s32 arg2);

CFrame lbl_1_bss_A88(120);
CFrame lbl_1_bss_A8C(210);
CFrame lbl_1_bss_A90(210);

void ObjectSetup(void)
{
    void *mem;
    void *master;

    mem = fn_1_9134(0x3B0);
    if (mem) {
        mem = fn_1_11520(mem);
    }
    master = mem;
    fn_1_6CEC(master, 1, 0x2000);
}
}
