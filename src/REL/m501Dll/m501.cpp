#include "REL/m501Dll.h"
#include "datanum/m501.h"
#include "game/object.h"
#include "game/hu3d.h"
#include <dolphin/os/OSThread.h>

extern "C" void ExitReq__13CMinigameCoreFv(void)
{
    omSysExitReq = 1;
}

void CModel::setHookData(void *data)
{
    Hu3DData[m_id].hookData = data;
}

Hu3DMtx *CModel::getMatrix()
{
    return (Hu3DMtx *)&Hu3DData[m_id].mtx;
}

void COMObj::OmObjUpdateStatic(struct omObj_s *obj)
{
    COMObj *self = (COMObj *)obj->data;
    if (self != 0) {
        self->OmObjUpdate(obj);
    }
}

COMObj::COMObj(struct omObj_s *obj)
{
    obj->data = this;
    obj->objFunc = (OMOBJFUNC)OmObjUpdateStatic;
}

CLinkNode::CLinkNode(CLinkNode &)
{
    m_prev->m_next = m_next;
    m_next->m_prev = m_prev;
    m_prev = this;
    m_next = this;
}

s32 CSubThread::GetStartPriority()
{
    return OSGetThreadPriority(OSGetCurrentThread());
}