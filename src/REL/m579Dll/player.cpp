#include "dolphin/types.h"

extern "C" {
typedef struct {
    u8 field_00[0x14];
    void *field_14;
    u8 field_18[0x50];
    s32 field_68;
    s32 field_6c;
} M579Player;

void fn_1_E524(M579Player *self, s32 a, s32 b);
void fn_1_E15C(M579Player *self);
void fn_1_FE34(M579Player *self);
void fn_1_E0AC(M579Player *self);
void fn_1_1270(M579Player *self, s32 a);
void fn_1_C8(M579Player *self);
void __ptmf_scall(void *self, void *ptmf);

void fn_1_DFBC(volatile M579Player *self)
{
    fn_1_E524((M579Player *) self, 5, 1);
}

void fn_1_DFEC(volatile M579Player *self)
{
    fn_1_E524((M579Player *) self, 6, 1);
}

void fn_1_E01C(M579Player *self)
{
    // original passes the PTMF in r12 with a nop after the bl; MWCC uses r4
    __ptmf_scall(self, &self->field_14);
    self->field_68 += 1;
}

void fn_1_E060(M579Player *self)
{
    if (self->field_6c != 0) {
        fn_1_E15C(self);
        fn_1_FE34(self);
    }
    fn_1_E0AC(self);
}

M579Player *fn_1_10C2C(M579Player *self, s16 arg1)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (arg1 > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}
}
