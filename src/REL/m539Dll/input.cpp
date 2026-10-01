#include "dolphin/types.h"
#include "game/gamework.h"
#include "game/pad.h"

// The pad/IRQ objects of m539 are polymorphic: every instance starts with a
// vtable pointer that the module keeps in .data by hand. There is no RTTI in
// the REL and no __cxa_pure_virtual, so a vtable is a plain array of words:
// an offset-to-top of 0, a null typeinfo and then the slots.

extern "C" {

typedef struct M539PadState_s {
    s32 unk0;
    bool unk4;
} M539PadState;

typedef void (*M539Slot)(void);
typedef void *(*M539DtorFn)(void *self, s16 flag);

// One object per pad: the reader keeps the player it belongs to and the pad
// channel it polls, and a "done" byte the IRQ handler raises.
typedef struct M539PadReader_s {
    M539Slot *vptr;
    s32 unk4;
    u8 unk8;
    u8 unk9[3];
    s32 playerNo;
    s32 padNo;
} M539PadReader;

// The IRQ handler: a poll vtable, and a thread subobject at 0x10 whose
// vtable is the one that follows the handler vtable in .data.
typedef struct M539Handler_s {
    M539Slot *vptr;
    s32 unk4;
    u8 unk8;
    u8 unk9[3];
    s32 unkC;
    M539Slot *subVptr;
} M539Handler;

typedef void (*M539PollFn)(M539Handler *self, void *st);

extern M539Slot lbl_1_data_210[];
extern M539Slot lbl_1_data_220[];
extern M539Slot lbl_1_data_22C;
extern M539Slot lbl_1_data_230;
extern M539Slot lbl_1_data_234[];
extern M539Slot lbl_1_data_244[];

s32 fn_1_4AE0(void);
void fn_1_C8(void *ptr);
void fn_1_558C(void *self, s32 prio);
void fn_1_5608(void *self, s16 arg);
void fn_1_56BC(void *self, s32 arg);
void fn_1_574C(void *self, s32 arg);

void fn_1_63E0(M539PadReader *self, s32 playerNo);
void *fn_1_6420(void *self, s16 flag);
void fn_1_6478(M539PadReader *self, M539PadState *st);
void *fn_1_64B8(M539Handler *self);
void fn_1_6524(M539Handler *self, s32 arg);
void fn_1_6558(M539Handler *self, M539PadState *st);
void *fn_1_6600(M539Handler *self, s16 flag);
void *fn_1_6688(M539PadReader *self, s16 flag);
void fn_1_66F4(M539Slot *self, M539PadState *st);
void fn_1_66FC(M539Slot *self, s16 flag);
// The IRQ handler vtable (0x210) and the vtable of its thread subobject
// (0x220), followed by the two adjustor thunks that reach the handler.
M539Slot lbl_1_data_210[] = { 0, 0, (M539Slot)fn_1_6600, (M539Slot)fn_1_6524 };
M539Slot lbl_1_data_220[] = { 0, 0, (M539Slot)fn_1_66FC };
M539Slot lbl_1_data_22C = (M539Slot)fn_1_66F4;
M539Slot lbl_1_data_230 = (M539Slot)fn_1_6558;

// The pad reader vtable (0x234) and the common base one (0x244).
M539Slot lbl_1_data_234[] = { 0, 0, (M539Slot)fn_1_6688, (M539Slot)fn_1_6478 };
M539Slot lbl_1_data_244[] = { 0, 0, (M539Slot)fn_1_6420, 0, 0 };

// NONMATCHING: the indirect call reloads the vtable through r31 (lwz r5, 0(r31))
// where the target reloads it through the incoming r3 (lwz r12, 0(r3)).
void fn_1_6398(M539Handler *self)
{
    self->unk8 = 0;
    ((M539PollFn)self->vptr[3])(self, (void *)&self->unk4);
}

void fn_1_63E0(M539PadReader *self, s32 playerNo)
{
    self->vptr = (M539Slot *)lbl_1_data_244;
    self->unk8 = 0;
    self->vptr = (M539Slot *)lbl_1_data_234;
    self->playerNo = playerNo;
    self->padNo = GwPlayerConf[playerNo].padNo;
}

void *fn_1_6420(void *self, s16 flag)
{
    if (self) {
        ((M539Slot *)self)[0] = (M539Slot)lbl_1_data_244;
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_6478(M539PadReader *self, M539PadState *st)
{
    s32 padNo = self->padNo;

    st->unk4 = (HuPadBtnDown[padNo] & 0x100) != 0;
}

void *fn_1_64B8(M539Handler *self)
{
    self->vptr = (M539Slot *)lbl_1_data_244;
    self->unk8 = 0;
    fn_1_558C(&self->subVptr, 0x8000);
    self->vptr = (M539Slot *)lbl_1_data_210;
    self->subVptr = (M539Slot *)((u8 *)lbl_1_data_210 + 0x10);
    return self;
}

void fn_1_6524(M539Handler *self, s32 arg)
{
    fn_1_56BC(&self->subVptr, arg);
}

// NONMATCHING: the second parameter is copied straight into r29 (mr r29, r4)
// where the target spills it and reloads it (stw r4, 0xc(r1) / lwz r29, 0xc(r1)).
void fn_1_6558(M539Handler *self, M539PadState *st)
{
    s32 i, n;

    for (i = 8; i != 0; i--) {
        for (n = fn_1_4AE0() % 30 + 0x1E; n > 0; n--) {
            fn_1_574C(&self->subVptr, 0);
        }
        st->unk4 = 1;
    }
}

void *fn_1_6600(M539Handler *self, s16 flag)
{
    if (self) {
        self->vptr = (M539Slot *)lbl_1_data_210;
        self->subVptr = (M539Slot *)((u8 *)lbl_1_data_210 + 0x10);
        fn_1_5608(&self->subVptr, 0);
        if (self) {
            self->vptr = (M539Slot *)lbl_1_data_244;
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void *fn_1_6688(M539PadReader *self, s16 flag)
{
    if (self) {
        self->vptr = (M539Slot *)lbl_1_data_234;
        if (self) {
            self->vptr = (M539Slot *)lbl_1_data_244;
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// The two thunks of the handler's thread subobject: they undo the 0x10 this
// adjustment and jump to the handler's own entry points.
void fn_1_66F4(M539Slot *self, M539PadState *st)
{
    __asm {
        subi r3, r3, 0x10
        b fn_1_6558
    }
}

void fn_1_66FC(M539Slot *self, s16 flag)
{
    __asm {
        subi r3, r3, 0x10
        b fn_1_6600
    }
}

}
