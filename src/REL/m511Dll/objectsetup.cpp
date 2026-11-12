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
#ifdef TARGET_PC
unsigned int m511GameMasterSize(void);
#endif
}

extern "C" void ObjectSetup(void)
{
#ifdef TARGET_PC
    void *master = fn_1_5164(m511GameMasterSize()); /* 0x370 on GameCube; the thread is larger on PC */
#else
    void *master = fn_1_5164(0x370);
#endif
    void *core;
    if (master != 0) {
        master = __ct__Q24m51111CGameMasterFv(master);
    }
    core = master;
    fn_1_2D1C(core, 1, 0x2000);
}
