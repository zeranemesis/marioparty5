class CTime {
public:
    CTime(int v) : m_time(v) {}
    int m_time;
};

static CTime lbl_1_bss_A18(120);
static CTime lbl_1_bss_A1C(210);
static CTime lbl_1_bss_A20(210);

// operator new, CGameMaster constructor and CMinigameCore setup of this module
extern "C" {
void *fn_1_46CC(unsigned long size);
void *__ct__Q24m51211CGameMasterFv(void *self);
void fn_1_2284(void *self, int a, int b);
}

extern "C" void ObjectSetup(void)
{
    void *master = fn_1_46CC(0x370);
    void *core;
    if (master != 0) {
        master = __ct__Q24m51211CGameMasterFv(master);
    }
    core = master;
    fn_1_2284(core, 1, 0x2000);
}
