class CTime {
public:
    CTime(int v) : m_time(v) {}
    int m_time;
};

static CTime lbl_1_bss_A20(120);
static CTime lbl_1_bss_A24(210);
static CTime lbl_1_bss_A28(210);

// operator new, CGameMaster constructor and CMinigameCore setup of this module
extern "C" {
void *fn_1_B46C(unsigned long size);
void *fn_1_CBEC(void *self);
void fn_1_90A8(void *self);
}

extern "C" void ObjectSetup(void)
{
    void *master = fn_1_B46C(0x368);
    void *core;
    if (master != 0) {
        master = fn_1_CBEC(master);
    }
    core = master;
    fn_1_90A8(core);
}
