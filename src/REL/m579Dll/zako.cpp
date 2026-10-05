#include "dolphin/types.h"

extern "C" {
typedef struct {
    u8 field_00[0x0C];
    s32 field_0C;
    u8 field_10[0x1C];
    s32 field_2C;
    u8 field_30[0xF0];
    void *field_120;
} M579Zako;

void fn_1_164D0();
void fn_1_645C();
void fn_1_16C68(M579Zako *self);
void fn_1_165A8(M579Zako *self);
void fn_1_17DA8(void *self, s32 arg1);
void __ptmf_scall(void *self, void *ptmf);

void fn_1_161A8()
{
    fn_1_164D0();
    fn_1_645C();
}

void fn_1_161CC(M579Zako *self, s32 arg1)
{
    self->field_0C = ~((u32)1 << arg1) & self->field_0C;
    switch (arg1) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        break;
    }
}

void fn_1_16554(M579Zako *self)
{
    // original passes the PTMF in r12 with a nop after the bl; MWCC uses r4
    __ptmf_scall(self, (void *)&self->field_0C);
    fn_1_16C68(self);
    fn_1_165A8(self);
    self->field_2C += 1;
}

void fn_1_17D64(M579Zako *self, s32 arg1)
{
    void *v = self->field_120;
    fn_1_17DA8(v, arg1);
}
}