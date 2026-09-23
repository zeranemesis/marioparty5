#include "dolphin/types.h"

class CFrame {
public:
    CFrame(s32 frame) { m_frame = frame; }
    s32 m_frame;
};

extern "C" {
void *fn_1_46CC(u32 size);
void *fn_1_7650(void *self);
void fn_1_2284(void *self, s32 arg1, s32 arg2);

CFrame lbl_1_bss_A38(120);
CFrame lbl_1_bss_A3C(210);
CFrame lbl_1_bss_A40(210);

void ObjectSetup(void)
{
    void *mem;
    void *master;

    mem = fn_1_46CC(0x360);
    if (mem) {
        mem = fn_1_7650(mem);
    }
    master = mem;
    fn_1_2284(master, 1, 0x2000);
}
}
