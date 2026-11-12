#ifdef TARGET_PC
#include "REL/m537Dll.h"
#else
#include "dolphin/types.h"
struct M537Main;
#endif

class CFrame {
public:
    CFrame(s32 frame) { m_frame = frame; }
    s32 m_frame;
};

extern "C" {
void *fn_1_46CC(u32 size);
void *fn_1_83CC(void *self);
void fn_1_2284(M537Main *self, s32 arg1, s32 arg2);

CFrame lbl_1_bss_A18(120);
CFrame lbl_1_bss_A1C(210);
CFrame lbl_1_bss_A20(210);

void ObjectSetup(void)
{
    void *mem;
    void *master;

#ifdef TARGET_PC
    mem = fn_1_46CC(sizeof(M537GameMaster));
#else
    mem = fn_1_46CC(0x360);
#endif
    if (mem) {
        mem = fn_1_83CC(mem);
    }
    master = mem;
    /* the minigame core is the first member of the master object */
    fn_1_2284((M537Main *)master, 1, 0x2000);
}
}
