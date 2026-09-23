#include "dolphin/types.h"

class CFrame {
public:
    CFrame(s32 frame) { m_frame = frame; }
    s32 m_frame;
};

extern "C" {
void *fn_1_4CE4(u32 size);
void *fn_1_7F1C(void *self);
void fn_1_289C(void *self, s32 arg1, s32 arg2);

CFrame lbl_1_bss_A18(120);
CFrame lbl_1_bss_A1C(210);
CFrame lbl_1_bss_A20(210);

void ObjectSetup(void)
{
    void *mem;
    void *master;

    mem = fn_1_4CE4(0x360);
    if (mem) {
        mem = fn_1_7F1C(mem);
    }
    master = mem;
    fn_1_289C(master, 1, 0x2000);
}
}
