#define _MATH_H

#include "dolphin/types.h"
#include "game/hu3d.h"
#include "game/object.h"

extern "C" {

struct M537ListNode_s {
    struct M537ListNode_s *prev;
    struct M537ListNode_s *next;
};

struct M537Vtbl_s {
    void *unk0;
    void *unk4;
    void (*unk8)(void *self);
    void (*unkC)(void *self, void *owner);
};

struct M537Node_s {
    struct M537Vtbl_s *vtable;
};

struct M537Owner_s {
    u8 pad0[0x14];
    void (*unk14)(struct M537Owner_s *owner);
    u8 pad18[0x5C - 0x18];
    struct M537Node_s *unk5C;
};

struct M537MotId_s {
    s16 motId;
};

void fn_1_4934(void *self);
void fn_1_48C0(void *self, void *owner);
void fn_1_4874(struct M537Owner_s *owner);

struct M537Vtbl_s lbl_1_data_1D4 = { 0, 0, fn_1_4934, fn_1_48C0 };

void fn_1_12DC(struct M537ListNode_s *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->prev = node;
    node->next = node;
}

void fn_1_2C44(void)
{
    omSysExitReq = 1;
}

void fn_1_30BC(struct M537MotId_s *model, void *hookData)
{
    Hu3DData[model->motId].hookData = hookData;
}

Mtx *fn_1_30D8(struct M537MotId_s *model)
{
    return &Hu3DData[model->motId].mtx;
}

void fn_1_4854(struct M537Node_s *self, struct M537Owner_s *owner)
{
    self->vtable = &lbl_1_data_1D4;
    owner->unk5C = self;
    owner->unk14 = fn_1_4874;
}

}
