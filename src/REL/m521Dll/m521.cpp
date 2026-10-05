#include "game/object.h"
#include "game/hu3d.h"

struct CListNode {
    CListNode *m_prev;
    CListNode *m_next;
};

struct ListHead {
    u8 pad_0[0x14];
    void (*fn_92DC)(void *);
    u8 pad_14[0x5C - 0x14 - 4];
    CListNode *m_node;
};

extern CListNode lbl_1_data_1F4;
extern void fn_1_92DC(void *);

struct CModel {
    HU3DMODELID m_id;
    MtxPtr getMatrix();
};

void fn_1_5D44(CListNode *self)
{
    self->m_prev->m_next = self->m_next;
    self->m_next->m_prev = self->m_prev;
    self->m_prev = self;
    self->m_next = self;
}

void fn_1_76AC(void)
{
    omSysExitReq = 1;
}

void fn_1_7B24(const HU3DMODELID *modelIdPtr, void *data)
{
    Hu3DData[*modelIdPtr].hookData = data;
}

MtxPtr CModel::getMatrix()
{
    return Hu3DData[m_id].mtx;
}

void fn_1_92BC(CListNode *self, ListHead *head)
{
    self->m_prev = &lbl_1_data_1F4;
    head->m_node = self;
    head->fn_92DC = fn_1_92DC;
}

#include "dolphin/os.h"

s32 fn_1_9CAC(void)
{
    return OSGetThreadPriority(OSGetCurrentThread());
}