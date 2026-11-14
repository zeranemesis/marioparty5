#ifdef TARGET_PC
#include "REL/m517Dll.h"
#define M517_MASTER_ALLOC sizeof(M517GameMaster)
#else
#define M517_MASTER_ALLOC 0x370
#endif

class CTime {
public:
    CTime(int v) : m_time(v) {}
    int m_time;
};

static CTime lbl_1_bss_A28(120);
static CTime lbl_1_bss_A2C(210);
static CTime lbl_1_bss_A30(210);

// operator new, CGameMaster constructor and CMinigameCore setup of this module
extern "C" {
void *fn_1_6184(unsigned long size);
void *fn_1_A168(void *self);
void fn_1_3D3C(void *self, int a, int b);
}

extern "C" void ObjectSetup(void)
{
    void *master = fn_1_6184(M517_MASTER_ALLOC);
    void *core;
    if (master != 0) {
        master = fn_1_A168(master);
    }
    core = master;
    fn_1_3D3C(core, 1, 0x2000);
}
