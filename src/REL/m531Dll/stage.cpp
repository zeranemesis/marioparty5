#include "dolphin/mtx.h"

// Declared here rather than through game/object.h and game/hu3d.h: in C++ that header pulls
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

void *HuDataSelHeapReadNum(int dataNum, s32 num, int heap);
HU3DMODELID Hu3DModelCreate(void *data);
void Hu3DModelKill(HU3DMODELID modelId);
void Hu3DModelCameraSet(HU3DMODELID modelId, u16 cameraBit);
void Hu3DModelLayerSet(HU3DMODELID modelId, s16 layerNo);
void Hu3DModelShadowSet(HU3DMODELID modelId);
void Hu3DModelShadowDispOn(HU3DMODELID modelId);
}

namespace m531 {
class CStage {
protected:
    OMOBJMAN *m_objMan;
    HU3DMODELID m_model;
    OMOBJ *m_obj;
    Vec m_pos;
    Vec m_rot;
    Vec m_scale;

public:
    CStage(OMOBJMAN *objMan, int dataNum, s16 layer, s16 prio, u16 camera);
    static void ObjUpdate(OMOBJ *obj);
    virtual void on_update() {}
    virtual ~CStage();
    void setShadow();
};

void CStage::ObjUpdate(OMOBJ *obj)
{
    CStage *self = (CStage *)obj->data;
    self->on_update();
}

CStage::CStage(OMOBJMAN *objMan, int dataNum, s16 layer, s16 prio, u16 camera)
{
    m_objMan = objMan;
    m_model = Hu3DModelCreate(HuDataSelHeapReadNum(dataNum, 0x10000000, 2));
    m_obj = omAddObjEx(objMan, prio, 16, 16, -1, ObjUpdate);
    m_obj->data = this;
    Hu3DModelLayerSet(m_model, layer);
    Hu3DModelCameraSet(m_model, camera);
    m_pos.x = 0.0f;
    m_pos.y = 0.0f;
    m_pos.z = 0.0f;
    m_rot.x = 0.0f;
    m_rot.y = 0.0f;
    m_rot.z = 0.0f;
    m_scale.x = 1.0f;
    m_scale.y = 1.0f;
    m_scale.z = 1.0f;
}

CStage::~CStage()
{
    Hu3DModelKill(m_model);
    m_obj->data = NULL;
    omDelObjEx(m_objMan, m_obj);
}

void CStage::setShadow()
{
    Hu3DModelShadowSet(m_model);
    Hu3DModelShadowDispOn(m_model);
}
}
