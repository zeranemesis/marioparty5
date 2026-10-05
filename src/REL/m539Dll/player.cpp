#include "REL/m539Dll.h"

/*
 * m539's "player".
 *
 * The player object keeps a vector of 16 bit entries reachable through a
 * pointer at offset 0x44, indexed by a count-valued field at 0x40, and a small
 * timer-ish float at 0x28 that decides when a state callback fires. fn_1_EA70
 * is the bounds-checked element accessor the vector code uses everywhere:
 * it reports "vector::at index out of range" through the __files assertion
 * helper before calling abort.
 */

extern "C" {

typedef struct M539Player_s {
    u8 pad0[0x1C];
    s32 unk1C;
    u8 pad20[8];
    f32 f28;
    u8 pad2C[0x10];
    void *unk3C;
    s32 unk40;
    s16 *unk44;
} M539Player;

typedef struct M539Vec_s {
    s32 unk0;
    s32 count;
    s16 *items;
} M539Vec;

extern u8 __files[];
void abort(void);

void fn_1_1270(void *ptr, s32 arg1);
void fn_1_DAF0(M539Player *self, s32 arg1);
void fn_1_EAE8(void **a, s16 *b, s32 c, s16 *d);
s16 CharMotionCreate(s16 a, s32 b);
void fn_1_234(void *file, const char *msg, ...);

s32 lbl_1_bss_A48;
s32 lbl_1_bss_A4C;
s32 lbl_1_bss_A50;

char lbl_1_data_4C8[] = "vector::at index out of range";

void *fn_1_D6B8(void *self, s16 arg1)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (arg1 > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_D710(M539Player *self, s32 arg1)
{
    s16 v = CharMotionCreate((s16)self->unk1C, arg1);
    s16 **slot = &self->unk44;
    s16 *elem = *slot + self->unk40;
    fn_1_EAE8(&self->unk3C, elem, 1, &v);
}

void fn_1_E008(M539Player *self)
{
    f32 v = self->f28 - 30.0f;
    if (v <= 0.0f) {
        fn_1_DAF0(self, 3);
        return;
    }
}

s16 *fn_1_EA70(M539Vec *self, u32 index)
{
    s16 **p;

    if (index >= self->count) {
        fn_1_234(__files + 0xA0, lbl_1_data_4C8);
        abort();
    }
    p = &self->items;
    return *p + index;
}

void fn_1_EFC0(void)
{
    lbl_1_bss_A48 = 120;
    lbl_1_bss_A4C = 210;
    lbl_1_bss_A50 = 210;
}

}