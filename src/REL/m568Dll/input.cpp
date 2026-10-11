// m568Dll input unit: the pad-input sub-objects.
//
// Four vtables live in .data, each {0, 0, v0, v1}; MWCC emits the two leading
// null words before the virtual entries:
//   lbl_1_data_268 -> fn_1_7DD8 / fn_1_76BC
//   lbl_1_data_278 -> fn_1_7E44 / fn_1_69E0
//   lbl_1_data_288 -> fn_1_7EB0 / fn_1_6384
//   lbl_1_data_298 -> fn_1_632C / 0          (the common base)
// A derived object's constructor installs the shared base vtable first and
// then overwrites it with its own, so both stores land on offset 0x00.

#include "dolphin/types.h"
#include "game/gamework.h"

struct InputVtbl {
    void *m_unk0;
    void *m_unk4;
    void *m_v0;
    void *m_v1;
};

struct InputObj {
    InputVtbl *m_vtable;  // 0x00
    u8 m_pad04[0xC];
    s32 m_f10;            // 0x10
    s32 m_f14;            // 0x14
    s32 m_f18;            // 0x18
    s32 m_f1C;            // 0x1C
};

// Per-player bit masks, one byte each, read with a dynamic index.
const u8 lbl_1_rodata_C8[8] = { 0x01, 0x09, 0x08, 0x0A, 0x02, 0x06, 0x04, 0x05 };

extern "C" {

void fn_1_6E0(void *self);
void fn_1_7830(void *self);
void fn_1_7DD8(void *self);
void fn_1_76BC(void *self);
void fn_1_7E44(void *self);
void fn_1_69E0(void *self);
void fn_1_7EB0(void *self);
void fn_1_6384(void *self);
InputObj *fn_1_632C(InputObj *self, s16 arg);

char lbl_1_data_250[] = "sm_ptr != 0";
char lbl_1_data_25C[] = "a_set.h";
s32 lbl_1_data_264 = 800;

InputVtbl lbl_1_data_268 = { 0, 0, fn_1_7DD8, fn_1_76BC };
InputVtbl lbl_1_data_278 = { 0, 0, fn_1_7E44, fn_1_69E0 };
InputVtbl lbl_1_data_288 = { 0, 0, fn_1_7EB0, fn_1_6384 };
InputVtbl lbl_1_data_298 = { 0, 0, fn_1_632C, 0 };

extern "C" void fn_1_62F4(InputObj *self, s32 playerNo)
{
    self->m_vtable = &lbl_1_data_298;
    self->m_vtable = &lbl_1_data_288;
    self->m_f10 = playerNo;
    self->m_f14 = GwPlayerConf[playerNo].padNo;
}

extern "C" InputObj *fn_1_632C(InputObj *self, s16 arg)
{
    if (self != 0) {
        self->m_vtable = &lbl_1_data_298;
        if (arg > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

extern "C" void fn_1_6428(InputObj *self)
{
    self->m_vtable = &lbl_1_data_298;
    self->m_vtable = &lbl_1_data_278;
}

extern "C" bool fn_1_67BC(u32 mask, s32 playerNo)
{
    playerNo &= 7;
    return (lbl_1_rodata_C8[playerNo] & mask) != 0;
}

extern "C" InputObj *fn_1_7658(InputObj *self)
{
    self->m_vtable = &lbl_1_data_298;
    self->m_vtable = &lbl_1_data_268;
    self->m_f18 = 0;
    self->m_f1C = 0;
    self->m_f10 = 0;
    fn_1_7830(self);
    return self;
}

}
