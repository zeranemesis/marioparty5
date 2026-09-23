#include "dolphin/mtx.h"

// Declared here rather than through game/object.h: in C++ that header pulls
// in MSL math.h, whose inline std::sqrtf emits constants into .rodata.
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
    Vec trans;
    Vec rot;
    Vec scale;
    u16 mdlcnt;
    HU3DMODELID *mdlId;
    u16 mtncnt;
    HU3DMOTID *mtnId;
    u32 work[4];
    void *data;
};

OMOBJ *omAddObjEx(OMOBJMAN *objMan, s16 prio, u16 mdlcnt, u16 mtncnt, s16 grpNo, OMOBJFUNC objFunc);
void omDelObjEx(OMOBJMAN *objMan, OMOBJ *obj);
}

namespace m531 {
class CScoreBase {
protected:
    OMOBJMAN *m_objMan;
    OMOBJ *m_obj;

public:
    CScoreBase(OMOBJMAN *objMan);
    static void ObjUpdate(OMOBJ *obj);
    virtual void on_update();
    virtual ~CScoreBase();
};

void CScoreBase::ObjUpdate(OMOBJ *obj)
{
    CScoreBase *self = (CScoreBase *)obj->data;
    self->on_update();
}

void CScoreBase::on_update() {}

CScoreBase::CScoreBase(OMOBJMAN *objMan)
{
    m_objMan = objMan;
    m_obj = omAddObjEx(objMan, 7, 0, 0, -1, ObjUpdate);
    m_obj->data = this;
}

CScoreBase::~CScoreBase()
{
    m_obj->data = NULL;
    omDelObjEx(m_objMan, m_obj);
}
}
