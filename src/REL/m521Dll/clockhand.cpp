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
