#include "dolphin/types.h"

class CFrame {
public:
    CFrame(s32 frame) { m_frame = frame; }
    s32 m_frame;
};

extern "C" {
void *fn_1_668C(u32 size);
void *fn_1_8A2C(void *self);
void fn_1_4244(void *self, s32 arg1, s32 arg2);

CFrame lbl_1_bss_A30(120);
CFrame lbl_1_bss_A34(210);
CFrame lbl_1_bss_A38(210);
CFrame lbl_1_bss_A3C(300);
CFrame lbl_1_bss_A40(60);

void ObjectSetup(void)
{
    void *mem;
    void *master;

    mem = fn_1_668C(0x368);
    if (mem) {
        mem = fn_1_8A2C(mem);
    }
    master = mem;
    fn_1_4244(master, 1, 0x2000);
}
}
