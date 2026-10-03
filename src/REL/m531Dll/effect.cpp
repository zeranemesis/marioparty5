#include "dolphin/mtx.h"

// Declared here rather than through game/hu3d.h and game/sprite.h: those headers
// drag in inline helpers that emit 0.5f and 3.0f into .rodata, and this unit has
// no .rodata section at all.
extern "C" {
typedef s16 HU3DMODELID;
typedef s16 HU3DMOTID;
typedef struct Process_s OMOBJMAN;
typedef struct omObj_s OMOBJ;
typedef void (*OMOBJFUNC)(OMOBJ *obj);
typedef struct Hu3DModel_s HU3DMODEL;
typedef struct Hu3DParticle_s HU3DPARTICLE;
typedef struct ANIMDATA ANIMDATA;

// Matches game/hu3d.h: sizeof(HU3DMODEL) is 0x134 and hookData sits at 0x120,
// which is how Hu3DData is indexed here.
struct Hu3DModel_s {
    u8 unk000[0x120];
    void *hookData;
    u8 unk124[0x134 - 0x124];
};
extern HU3DMODEL Hu3DData[];

// Matches game/hu3d.h: the back-pointer to the owning effect is at 0x1C (named
// `work` there, which is a MWCC keyword, hence the spelling here).
struct Hu3DParticle_s {
    s16 dataCnt;
    s16 emitCnt;
    f32 unk04[6];
    void *ownerP;
};

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
void *__dl__FPv(void *ptr);
void *HuDataSelHeapReadNum(int dataNum, s32 num, int heap);
HU3DMODELID Hu3DHookFuncCreate(void *hookFunc);
void Hu3DModelKill(HU3DMODELID modelId);
void Hu3DModelAttrSet(HU3DMODELID modelId, u32 attr);
void Hu3DModelAttrReset(HU3DMODELID modelId, u32 attr);
void Hu3DModelCameraSet(HU3DMODELID modelId, u16 cameraBit);
void Hu3DModelLayerSet(HU3DMODELID modelId, s16 layerNo);
HU3DMODELID Hu3DParticleCreate(ANIMDATA *anim, s16 maxCnt);
ANIMDATA *HuSprAnimRead(void *data);
void HuSprAnimLock(ANIMDATA *anim);
void HuSprAnimKill(ANIMDATA *anim);
}

// MWCC emits the vtable pointer after the data of the class itself, so both
// effect objects carry their table pointer at offset 0xC. Vtable slots follow
// declaration order after two zero words of header.
class CEffectParticle {
public:
    ANIMDATA *m_anim;
    HU3DMODELID m_model;
    HU3DPARTICLE *m_workPtr;

public:
    CEffectParticle(int dataNum, s32 maxCnt, u16 camera);
    virtual ~CEffectParticle();
    virtual void fn_1_29BC(Mtx *mtx);
};

class CEffectModel {
public:
    OMOBJMAN *m_objMan;
    OMOBJ *m_obj;
    HU3DMODELID m_model;

public:
    CEffectModel(OMOBJMAN *objMan, s16 layer, s16 prio, u16 camera);
    virtual void fn_1_2734();
    virtual void fn_1_2788(Mtx *mtx);
    virtual ~CEffectModel();
};

extern "C" {
void fn_1_26EC(OMOBJ *obj);
void fn_1_2738(HU3DMODEL *modelP, Mtx *mtx);
void fn_1_296C(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx *mtx);
void fn_1_28CC(CEffectModel *self, u16 camera);
void fn_1_2904(CEffectModel *self);
void fn_1_2938(CEffectModel *self);
void fn_1_2AD0(CEffectParticle *self, u16 camera);
void fn_1_2B08(CEffectParticle *self);
void fn_1_2B3C(CEffectParticle *self);
}

// The object's objFunc points at this thunk, which forwards to the object's
// first virtual.
void fn_1_26EC(OMOBJ *obj)
{
    CEffectModel *self = (CEffectModel *)obj->data;
    self->fn_1_2734();
}

void CEffectModel::fn_1_2734()
{
}

// The model's Hu3D hook, registered through Hu3DHookFuncCreate.
void fn_1_2738(HU3DMODEL *modelP, Mtx *mtx)
{
    CEffectModel *self = (CEffectModel *)modelP->hookData;
    self->fn_1_2788(mtx);
}

void CEffectModel::fn_1_2788(Mtx *mtx)
{
}

CEffectModel::CEffectModel(OMOBJMAN *objMan, s16 layer, s16 prio, u16 camera)
{
    m_objMan = objMan;
    m_obj = omAddObjEx(objMan, prio, 0, 0, -1, fn_1_26EC);
    m_obj->data = this;
    m_model = Hu3DHookFuncCreate(fn_1_2738);
    HU3DMODEL *modelP = &Hu3DData[m_model];
    modelP->hookData = this;
    Hu3DModelLayerSet(m_model, layer);
    Hu3DModelCameraSet(m_model, camera);
}

CEffectModel::~CEffectModel()
{
    m_obj->data = 0;
    omDelObjEx(m_objMan, m_obj);
    Hu3DModelKill(m_model);
}

void fn_1_28CC(CEffectModel *self, u16 camera)
{
    Hu3DModelCameraSet(self->m_model, camera);
}

void fn_1_2904(CEffectModel *self)
{
    Hu3DModelAttrReset(self->m_model, 1);
}

void fn_1_2938(CEffectModel *self)
{
    Hu3DModelAttrSet(self->m_model, 1);
}

// The particle's Hu3D hook, registered through Hu3DParticleHookSet.
void fn_1_296C(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx *mtx)
{
    CEffectParticle *self = (CEffectParticle *)particleP->ownerP;
    self->fn_1_29BC(mtx);
}

void CEffectParticle::fn_1_29BC(Mtx *mtx)
{
}

CEffectParticle::CEffectParticle(int dataNum, s32 maxCnt, u16 camera)
{
    m_anim = HuSprAnimRead(HuDataSelHeapReadNum(dataNum, 0x10000000, 2));
    HuSprAnimLock(m_anim);
    m_model = Hu3DParticleCreate(m_anim, maxCnt);
    m_workPtr = (HU3DPARTICLE *)Hu3DData[m_model].hookData;
    m_workPtr->ownerP = this;
    fn_1_2AD0(this, camera);
}

CEffectParticle::~CEffectParticle()
{
    HuSprAnimKill(m_anim);
    Hu3DModelKill(m_model);
}

void fn_1_2AD0(CEffectParticle *self, u16 camera)
{
    Hu3DModelCameraSet(self->m_model, camera);
}

void fn_1_2B08(CEffectParticle *self)
{
    Hu3DModelAttrReset(self->m_model, 1);
}

void fn_1_2B3C(CEffectParticle *self)
{
    Hu3DModelAttrSet(self->m_model, 1);
}
