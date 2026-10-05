#include "dolphin/types.h"

extern "C" {

extern s16 omSysExitReq;

typedef struct {
    s16 id;
} M579Hu3DRef;

typedef struct {
    u8 pad_00[0xF0];
    void *unk_F0;
    u8 pad_F4[0x2C];
    s32 unk_120;
    u8 pad_124[0x10];
} M579Hu3DModel;

extern M579Hu3DModel Hu3DData[];

typedef struct M579Node M579Node;

struct M579Node {
    M579Node *unk_00;
    M579Node *unk_04;
};

typedef struct {
    u8 pad_00[0x14];
    void *unk_14;
    u8 pad_18[0x44];
    void *unk_5C;
} M579Thing;

typedef struct {
    void *unk_00;
} M579VtblHolder;

typedef void *M579Vtbl[4];

void fn_1_4874(M579Thing *self);
void fn_1_48C0(M579Thing *self);
void fn_1_4934(M579Thing *self);

M579Vtbl lbl_1_data_1D4 = { 0, 0, (void *)fn_1_4934, (void *)fn_1_48C0 };

void fn_1_12DC(M579Node *self)
{
    self->unk_00->unk_04 = self->unk_04;
    self->unk_04->unk_00 = self->unk_00;
    self->unk_00 = self;
    self->unk_04 = self;
}

void fn_1_2C44(void)
{
    omSysExitReq = 1;
}

void fn_1_30BC(M579Hu3DRef *self, s32 val)
{
    Hu3DData[self->id].unk_120 = val;
}

void *fn_1_30D8(M579Hu3DRef *self)
{
    return &Hu3DData[self->id].unk_F0;
}

void fn_1_4854(M579VtblHolder *obj, M579Thing *self)
{
    obj->unk_00 = &lbl_1_data_1D4;
    self->unk_5C = obj;
    self->unk_14 = &fn_1_4874;
}

}