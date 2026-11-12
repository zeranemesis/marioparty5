#include <stddef.h> /* size_t for operator new (unsigned long on GC) */
class CTime {
public:
    CTime(int v) : m_time(v) {}
    int m_time;
};

static CTime lbl_1_bss_C0(120);
static CTime lbl_1_bss_C4(210);
static CTime lbl_1_bss_C8(210);
class CMinigameCore {
public:
    void *operator new(size_t size);
    void setup(int a, int b);
};

namespace m501 {
class CGameMaster : public CMinigameCore {
public:
    CGameMaster();
    char pad[0x370];
};
}

extern "C" void ObjectSetup(void)
{
    CMinigameCore *core = new m501::CGameMaster();
    core->setup(1, 0x2000);
}
