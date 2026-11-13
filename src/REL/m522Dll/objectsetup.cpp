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
#ifdef TARGET_PC
unsigned long m522GameMasterSize(void);
#endif
}

extern "C" void ObjectSetup(void)
{
#ifdef TARGET_PC
    /* the master's real PC size (its sub-thread holds a host-sized OSThread and semaphore) */
    void *master = fn_1_B46C(m522GameMasterSize());
#else
    void *master = fn_1_B46C(0x368);
#endif
    void *core;
    if (master != 0) {
        master = fn_1_CBEC(master);
    }
    core = master;
    fn_1_90A8(core);
}
