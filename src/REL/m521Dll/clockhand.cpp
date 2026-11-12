#include "dolphin/mtx.h"

// Declared here rather than through game/hu3d.h: in C++ that header pulls in
// MSL math.h, whose inline std::sqrtf emits constants into .rodata.
extern "C" {
typedef s16 HU3DMODELID;
void Hu3DModelShadowSet(HU3DMODELID modelId);
void Hu3DModelShadowDispOn(HU3DMODELID modelId);
}

class CModel {
public:
    CModel(unsigned int dataNum);
    ~CModel();
    HU3DMODELID getId() const { return m_id; }
    MtxPtr getMatrix();

private:
    HU3DMODELID m_id;
};

namespace m521 {
class CClockHand {
public:
    CClockHand(unsigned int dataNum, Mtx baseMtx);
    void setAngle(float angle);
    ~CClockHand();

private:
    CModel m_model;
    Mtx m_baseMtx;
    Mtx m_mtx;
    float m_angle;
};

CClockHand::CClockHand(unsigned int dataNum, Mtx baseMtx) : m_model(dataNum)
{
    HU3DMODELID modelId = m_model.getId();
    Hu3DModelShadowSet(modelId);
    Hu3DModelShadowDispOn(modelId);
    PSMTXCopy(baseMtx, m_baseMtx);
    m_angle = 0.0f;
}

void CClockHand::setAngle(float angle)
{
    Mtx rotMtx;
    MtxPtr modelMtx;

    m_angle = angle;
    PSMTXRotRad(rotMtx, 'Y', 0.017453292f * angle);
    PSMTXConcat(m_baseMtx, rotMtx, m_mtx);
    modelMtx = m_model.getMatrix();
    PSMTXCopy(m_mtx, modelMtx);
}

CClockHand::~CClockHand() {}
}

#ifdef TARGET_PC
#include <new>
// The CModel members live in m521.cpp under their MWCC names; clock.cpp calls CClockHand by its MWCC names.
extern "C" {
void *__ct__6CModelFUi(void *self, u32 dataNum);
void *__dt__6CModelFv(void *self, s16 flag);
void HuMemDirectFree(void *ptr);

m521::CClockHand *__ct__Q24m52110CClockHandFUiPA4_f(m521::CClockHand *self, u32 dataNum, Mtx baseMtx)
{
    return new (self) m521::CClockHand(dataNum, baseMtx);
}

void __dt__Q24m52110CClockHandFv(m521::CClockHand *self, s32 freeIt)
{
    if (self != 0) {
        self->~CClockHand();
        if (freeIt > 0) {
            HuMemDirectFree(self);
        }
    }
}

void setAngle__Q24m52110CClockHandFf(m521::CClockHand *self, f32 angle) { self->setAngle(angle); }
}

CModel::CModel(unsigned int dataNum) { __ct__6CModelFUi(this, dataNum); }
CModel::~CModel() { __dt__6CModelFv(this, 0); }
#endif
