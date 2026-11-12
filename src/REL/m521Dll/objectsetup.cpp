#include "dolphin/types.h"

// The game master is 0x3B0 bytes on the GameCube; pointer members make it larger on PC.
#ifdef TARGET_PC
extern "C" u32 m521GameMasterSize(void);
#define MASTER_ALLOC_SIZE m521GameMasterSize()
#else
#define MASTER_ALLOC_SIZE 0x3B0
#endif

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

    mem = fn_1_9134(MASTER_ALLOC_SIZE);
    if (mem) {
        mem = fn_1_11520(mem);
    }
    master = mem;
    fn_1_6CEC(master, 1, 0x2000);
}
}
