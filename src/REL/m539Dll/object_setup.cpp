#define _MATH_H
#define M539_USE_DOLPHIN_HEADERS
extern "C" {
#include "dolphin.h"
#include "game/mg/timer.h"
}
#include "REL/m539Dll.h"

class CFrame {
public:
    CFrame(s32 frame) { m_frame = frame; }
    s32 m_frame;
};

extern "C" {
void *fn_1_46CC(u32 size);
M539GM *fn_1_8248(M539GM *self);
void fn_1_2284(void *self, s32 arg1, s32 arg2);

CFrame lbl_1_bss_A20(120);
CFrame lbl_1_bss_A24(210);
CFrame lbl_1_bss_A28(210);

void ObjectSetup(void)
{
    M539GM *mem;
    M539GM *master;

    mem = (M539GM *)fn_1_46CC(sizeof(M539GM));
    if (mem) {
        mem = fn_1_8248(mem);
    }
    master = mem;
    fn_1_2284(master, 1, 0x2000);
}
}
