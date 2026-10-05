// carddll/REL/carddll/cardmain.c - decompilation of the carddll/cardmain unit.
// Real code is being added function by function (smallest first), each verified
// at 100% with tools/fndiff.py before the next one.

#include "types.h"

/* ---- bss globals (carddll/cardmain) ---- */
static s32 lbl_1_bss_2C4;
static f32 lbl_1_bss_2BC[2];

typedef struct {
    u8 data[0x50];
    s32 field_50;
    u8 pad_54[0x8];
} CardEntry;

typedef struct {
    CardEntry entries[4];
    s32 currentIndex;
    s32 selector[4];
} CardMainBssE4;

static CardMainBssE4 lbl_1_bss_E4;

/* ---- rodata globals (carddll/cardmain) ---- */
static f32 lbl_1_rodata_68 = 0.0f;

/* ---- externs ---- */
extern void (*_ctors[])(void);
extern void (*_dtors[])(void);
extern void ObjectSetup(void);

/* fn_1_7DFC: initialize global state to zero */
void fn_1_7DFC(void)
{
    lbl_1_bss_2C4 = 0;
    lbl_1_bss_2BC[0] = lbl_1_rodata_68;
}

/* fn_1_8E88: get field_50 of selected entry */
// NONMATCHING: selector load uses lwz+add vs lwzx+addi (regalloc)
s32 fn_1_8E88(void)
{
    CardMainBssE4 *base = &lbl_1_bss_E4;
    CardEntry *entry = (CardEntry *)((u8 *)base + ((s32 *)((u8 *)base + 0x174))[base->currentIndex] * 0x5C);
    return entry->field_50;
}

/* _prolog: call constructors then ObjectSetup */
// NONMATCHING: loop condition uses r3+cmplw vs r0+cmplwi (regalloc)
s32 _prolog(void)
{
    void (**ctor)(void) = _ctors;
    while (*ctor != NULL) {
        (*ctor)();
        ctor++;
    }
    ObjectSetup();
    return 0;
}

/* _epilog: call destructors */
// NONMATCHING: loop condition uses r3+cmplw vs r0+cmplwi (regalloc)
void _epilog(void)
{
    void (**dtor)(void) = _dtors;
    while (*dtor != NULL) {
        (*dtor)();
        dtor++;
    }
}