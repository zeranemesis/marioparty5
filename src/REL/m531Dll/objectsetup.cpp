#include "dolphin/types.h"

typedef struct Process_s OMOBJMAN;
typedef struct omObj_s OMOBJ;

class CTime {
public:
    CTime(s32 time) { m_time = time; }
    s32 m_time;
};

class CTimer {
public:
    CTimer(int type);
    ~CTimer();

private:
    void *m_timer;
};

class CMinigameCore {
protected:
    u32 m_unk0;

public:
    void *operator new(unsigned long size);
    void operator delete(void *ptr);

    CMinigameCore();
    virtual ~CMinigameCore();
    virtual void on_setup(OMOBJMAN *objMan) = 0;
    virtual void on_update();
    virtual void on_close() = 0;

    void setup(int prio, int stackSize);

protected:
    s32 m_winner[4];
    s32 m_unk18;
    s32 m_unk1C;
};

namespace m531 {
class CGameMaster : public CMinigameCore {
protected:
    s32 m_mode;
    OMOBJ *m_obj;
    OMOBJMAN *m_objMan;

public:
    CGameMaster() { m_mode = 0; }
    virtual ~CGameMaster();
    virtual void on_setup(OMOBJMAN *objMan);
    virtual void mode0();
    virtual void mode1();
    virtual void mode2();
    virtual void mode3();
    virtual void mode4();
    virtual void mode5();
    virtual void mode6();
};

// Defined outside the class: with the destructor as the first virtual and
// inline, MWCC emits the vtable (weak) in every unit that needs it.
inline CGameMaster::~CGameMaster() {}

class CFlow : public CGameMaster {
public:
    CFlow() : m_timer(0) {}
    virtual ~CFlow();
    virtual void on_close();
    virtual void mode0();
    virtual void mode1();
    virtual void mode2();
    virtual void mode3();
    virtual void mode4();
    virtual void mode5();
    virtual void mode6();

private:
    u8 m_unk2C[0x50 - 0x2C];
    CTimer m_timer;
    u8 m_unk54[0x208 - 0x54];
};
}

CTime lbl_1_bss_60(120);
CTime lbl_1_bss_64(210);
CTime lbl_1_bss_68(210);

extern "C" void ObjectSetup(void)
{
    CMinigameCore *core = new m531::CFlow();
    core->setup(50, 0x2000);
}
