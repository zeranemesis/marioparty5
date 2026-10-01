#include "dolphin/types.h"

// Declared here rather than through game/object.h: in C++ that header pulls in
// MSL math.h, whose inline std::sqrtf emits constants into .rodata.
extern "C" {
typedef s16 HU3DMODELID;
typedef s16 HU3DMOTID;
typedef struct Process_s OMOBJMAN;
typedef struct omObj_s OMOBJ;
typedef void (*OMOBJFUNC)(OMOBJ *obj);

struct omObj_s {
    u16 stat;
    s16 objNext;
    s16 prio;
    s16 prev;
    s16 next;
    s16 nextNo;
    s16 grpNo;
    u16 memberNo;
    u32 mode;
    OMOBJFUNC objFunc;
    f32 trans[3];
    f32 rot[3];
    f32 scale[3];
    u16 mdlcnt;
    HU3DMODELID *mdlId;
    u16 mtncnt;
    HU3DMOTID *mtnId;
    u32 work[4];
    void *data;
};

OMOBJ *omAddObjEx(OMOBJMAN *objMan, s16 prio, u16 mdlcnt, u16 mtncnt, s16 grpNo, OMOBJFUNC objFunc);
}

class CTime {
public:
    CTime(s32 time) { m_time = time; }
    s32 m_time;
};

namespace m531 {
class CMinigameCore {
protected:
    u32 m_unk0;

public:
    virtual ~CMinigameCore();
    virtual void on_setup(OMOBJMAN *objMan);
    virtual void on_update();
    virtual void on_close();

    void setup(int prio, int stackSize);

protected:
    s32 m_winner[4];
    s32 m_unk18;
    s32 m_unk1C;
};

class CGameMaster : public CMinigameCore {
public:
    CGameMaster() { m_mode = 0; }
    s32 m_mode;
    OMOBJ *m_obj;
    OMOBJMAN *m_objMan;

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

// The object's objFunc is pointed at one of these thunks, which forwards to the
// virtual of the same index. extern "C" so the emitted names stay fn_1_XXXX,
// as recorded in symbols.txt.
extern "C" {
void fn_1_BC64(OMOBJ *obj);
void fn_1_BCB0(OMOBJ *obj);
void fn_1_BCFC(OMOBJ *obj);
void fn_1_BD48(OMOBJ *obj);
void fn_1_BD94(OMOBJ *obj);
void fn_1_BDE0(OMOBJ *obj);
void fn_1_BE2C(OMOBJ *obj);

// Switches to mode `mode` and points the object's objFunc at the matching thunk.
void fn_1_BC40(CGameMaster *self, s32 mode);
}

static const OMOBJFUNC lbl_1_rodata_5D0[8] = {
    fn_1_BC64,
    fn_1_BCB0,
    fn_1_BCFC,
    fn_1_BD48,
    fn_1_BD94,
    fn_1_BDE0,
    fn_1_BE2C,
    (OMOBJFUNC)0,
};

void CGameMaster::on_setup(OMOBJMAN *objMan)
{
    m_objMan = objMan;
    m_obj = omAddObjEx(objMan, 0, 0, 0, -1, fn_1_BC64);
    m_obj->data = this;
    m_obj->objFunc = fn_1_BC64;
}

void fn_1_BC40(CGameMaster *self, s32 mode)
{
    self->m_mode = mode;
    self->m_obj->objFunc = lbl_1_rodata_5D0[mode];
}

void fn_1_BC64(OMOBJ *obj)
{
    CGameMaster *self = (CGameMaster *)obj->data;
    self->mode0();
}

void CGameMaster::mode0()
{
}

void fn_1_BCB0(OMOBJ *obj)
{
    CGameMaster *self = (CGameMaster *)obj->data;
    self->mode1();
}

void CGameMaster::mode1()
{
}

void fn_1_BCFC(OMOBJ *obj)
{
    CGameMaster *self = (CGameMaster *)obj->data;
    self->mode2();
}

void CGameMaster::mode2()
{
}

void fn_1_BD48(OMOBJ *obj)
{
    CGameMaster *self = (CGameMaster *)obj->data;
    self->mode3();
}

void CGameMaster::mode3()
{
}

void fn_1_BD94(OMOBJ *obj)
{
    CGameMaster *self = (CGameMaster *)obj->data;
    self->mode4();
}

void CGameMaster::mode4()
{
}

void fn_1_BDE0(OMOBJ *obj)
{
    CGameMaster *self = (CGameMaster *)obj->data;
    self->mode5();
}

void CGameMaster::mode5()
{
}

void fn_1_BE2C(OMOBJ *obj)
{
    CGameMaster *self = (CGameMaster *)obj->data;
    self->mode6();
}

void CGameMaster::mode6()
{
}
}

static CTime lbl_1_bss_70(120);
static CTime lbl_1_bss_74(210);
static CTime lbl_1_bss_78(210);