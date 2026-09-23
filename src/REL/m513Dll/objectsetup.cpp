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
void *fn_1_4BD0(unsigned long size);
void *fn_1_B570(void *self);
void fn_1_2788(void *self, int a, int b);
}

extern "C" void ObjectSetup(void)
{
    void *master = fn_1_4BD0(0x378);
    void *core;
    if (master != 0) {
        master = fn_1_B570(master);
    }
    core = master;
    fn_1_2788(core, 1, 0x2000);
}
