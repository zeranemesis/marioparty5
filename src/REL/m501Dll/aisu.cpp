#include "REL/m501Dll.h"
#include <dolphin/mtx.h>

namespace m501 {

struct HU3DMODEL;
typedef s16 HU3DMODELID;
typedef void (*HU3DMODELHOOK)(HU3DMODEL *, Mtx *);

extern "C" void __dt__9CLinkNodeFv(void *, int);
extern "C" void __dl__FPv(void *);
extern "C" HU3DMODELID Hu3DHookFuncCreate(HU3DMODELHOOK);
extern "C" void Hu3DModelLayerSet(HU3DMODELID, s16);
extern "C" void Hu3DModelCameraSet(HU3DMODELID, u16);
extern "C" void drawHook__Q24m5015CAisuFP11Hu3DModel_sPA3_A4_f(HU3DMODEL *, Mtx *);



extern CLinkList aisuAllList__Q24m5015CAisu;
extern CLinkList aisuFallList__Q24m5015CAisu;
extern CLinkList aisuConeList__Q24m5015CAisu;

CHandleMan *CAisu::aisuHandleMan = 0;

extern "C" void init__Q24m5015CAisuFP10CHandleMan(CHandleMan *);

extern "C" void execAisuList__Q24m5015CAisuFP9CLinkListPFPQ24m5015CAisu_v(CLinkList *, void (*)(CAisu *));
extern "C" void AisuUpdate__Q24m5015CAisuFPQ24m5015CAisu(CAisu *);

void *CAisu::getHandle(int idx)
{
    return aisuHandleMan->getHandlePtr(idx);
}

void CAisu::setPos(Vec *pos, f32 t)
{
    m_pos.x = m_pos.x + t * (pos->x - m_pos.x);
    m_pos.z = m_pos.z + t * (pos->z - m_pos.z);
}

void CAisu::UpdateAll()
{
    execAisuList__Q24m5015CAisuFP9CLinkListPFPQ24m5015CAisu_v(&aisuAllList__Q24m5015CAisu, AisuUpdate__Q24m5015CAisuFPQ24m5015CAisu);
    execAisuList__Q24m5015CAisuFP9CLinkListPFPQ24m5015CAisu_v(&aisuFallList__Q24m5015CAisu, AisuUpdate__Q24m5015CAisuFPQ24m5015CAisu);
    execAisuList__Q24m5015CAisuFP9CLinkListPFPQ24m5015CAisu_v(&aisuConeList__Q24m5015CAisu, AisuUpdate__Q24m5015CAisuFPQ24m5015CAisu);
}

extern "C" void init__Q24m5015CAisuFP10CHandleMan(CHandleMan *handleMan)
{
    CAisu::aisuHandleMan = handleMan;
    HU3DMODELID hookMdl = Hu3DHookFuncCreate(drawHook__Q24m5015CAisuFP11Hu3DModel_sPA3_A4_f);
    Hu3DModelLayerSet(hookMdl, 0);
    Hu3DModelCameraSet(hookMdl, 1);
}

extern "C" CAisu *fn_1_7708(CAisu *this_, s16 flag)
{
    if (this_ != 0) {
        __dt__9CLinkNodeFv(this_, 0);
        if (flag > 0) {
            __dl__FPv(this_);
        }
    }
    return this_;
}

extern "C" CAisu *fn_1_88DC(CAisu *this_, s16 flag)
{
    if (this_ != 0) {
        __dt__9CLinkNodeFv(this_, 0);
        if (flag > 0) {
            __dl__FPv(this_);
        }
    }
    return this_;
}

} // namespace m501