class CTime {
public:
    CTime(int v) : m_time(v) {}
    int m_time;
};

static CTime lbl_1_bss_A40(120);
static CTime lbl_1_bss_A44(210);
static CTime lbl_1_bss_A48(210);

// operator new, CGameMaster constructor and CMinigameCore setup of this module
extern "C" {
void *fn_1_5164(unsigned long size);
void *__ct__Q24m51111CGameMasterFv(void *self);
void fn_1_2D1C(void *self, int a, int b);
}

extern "C" void ObjectSetup(void)
{
    void *master = fn_1_5164(0x370);
    void *core;
    if (master != 0) {
        master = __ct__Q24m51111CGameMasterFv(master);
    }
    core = master;
    fn_1_2D1C(core, 1, 0x2000);
}
