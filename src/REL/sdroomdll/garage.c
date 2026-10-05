#include "dolphin.h"
#include "game/gamework.h"

/* Helpers that live in the other objects of this REL (sdroom.c / machine.c). */
extern void fn_1_46C4(void *machine, s32 flag);

/*
 * The rest of this file is still undecoded, but the .bss layout has to be
 * reproduced exactly: objdiff resolves static-data relocations by address, so
 * lbl_1_bss_67C only lines up if every symbol before it keeps its size.
 * Addresses below are .bss offsets (the section itself starts at 0x568).
 */
static char lbl_1_bss_568[0x4];    /* 0x568, really 0x2 + 2 bytes of padding */
static char lbl_1_bss_56C[0x2];    /* 0x56C */
static char lbl_1_bss_56E[0x2];    /* 0x56E */
static char lbl_1_bss_570[0x14];   /* 0x570 */
static char lbl_1_bss_584[0x6];    /* 0x584 */
static char lbl_1_bss_58A[0x2];    /* 0x58A */
static char lbl_1_bss_58C[0x20];   /* 0x58C */
static char lbl_1_bss_5AC[0xC];    /* 0x5AC */
static char lbl_1_bss_5B8[0x4];    /* 0x5B8 */
static char lbl_1_bss_5BC[0x8];    /* 0x5BC */
static char lbl_1_bss_5C4[0x2];    /* 0x5C4 */
static char lbl_1_bss_5C6[0x2];    /* 0x5C6 */
static char lbl_1_bss_5C8[0x8];    /* 0x5C8 */
static char lbl_1_bss_5D0[0x6];    /* 0x5D0 */
static char lbl_1_bss_5D6[0x6];    /* 0x5D6 */
static char lbl_1_bss_5DC[0x4];    /* 0x5DC */
static char lbl_1_bss_5E0[0xA];    /* 0x5E0 */
static char lbl_1_bss_5EA[0xA];    /* 0x5EA */
static char lbl_1_bss_5F4[0x2];    /* 0x5F4 */
static char lbl_1_bss_5F6[0xA];    /* 0x5F6 */
static char lbl_1_bss_600[0x4];    /* 0x600 */
static char lbl_1_bss_604[0x2];    /* 0x604 */
static char lbl_1_bss_606[0x2];    /* 0x606 - window id, read with lha */
static char lbl_1_bss_608[0x18];   /* 0x608 */
static char lbl_1_bss_620[0x58];   /* 0x620 */
static char lbl_1_bss_678[0x4];    /* 0x678 */

/* Per-slot live state of one garage machine. */
typedef struct GarageMachine_s {
    /* 0x00 */ u8 pad00[0x1A];
    /* 0x1A */ s16 modelId;
    /* 0x1C */ u8 pad1C[0x10];
} GARAGE_MACHINE;

static GARAGE_MACHINE lbl_1_bss_67C[2]; /* 0x67C, 0x58 bytes total */

static char lbl_1_bss_6D4[0xA];    /* 0x6D4 */
static char lbl_1_bss_6DE[0x20];   /* 0x6DE */
static char lbl_1_bss_6FE[0x2];    /* 0x6FE */
static char lbl_1_bss_700[0x4];    /* 0x700 */
static char lbl_1_bss_704[0x4];    /* 0x704 */
static char lbl_1_bss_708[0x6];    /* 0x708 */
static char lbl_1_bss_70E[0x6];    /* 0x70E */
static char lbl_1_bss_714[0x2];    /* 0x714 */
static char lbl_1_bss_716[0x2];    /* 0x716 */
static char lbl_1_bss_718[0x2];    /* 0x718 */
static char lbl_1_bss_71A[0x2];    /* 0x71A */
static char lbl_1_bss_71C[0x6];    /* 0x71C */
static char lbl_1_bss_722[0x2];    /* 0x722 */
static char lbl_1_bss_724[0x2];    /* 0x724 */
static char lbl_1_bss_726[0x2];    /* 0x726 */
static char lbl_1_bss_728[0xE];    /* 0x728 */
static char lbl_1_bss_736[0x14];   /* 0x736 */

/* Set every machine that is present in the saved data up for redraw. */
void fn_1_7298(void)
{
    s16 i;
    GWSDMACHINE *machine;

    for (i = 0; i < 2; i++) {
        machine = &GwSdCommon.machine[i];
        if (machine->charNo != -1) {
            fn_1_46C4(&lbl_1_bss_67C[i], 1);
        }
    }
}

/* Same walk, but clears the slot instead of drawing it. */
void fn_1_7320(void)
{
    s16 i;
    GWSDMACHINE *machine;

    for (i = 0; i < 2; i++) {
        machine = &GwSdCommon.machine[i];
        if (machine->charNo != -1) {
            fn_1_46C4(&lbl_1_bss_67C[i], 0);
        }
    }
}

/*
 * Transition hook that both garage state machines call with (object, window
 * id); it has no effect at all. The original still carries a full frame,
 * though: it allocates two integer locals and one double to r31/r30/f31 and
 * then drops every store, leaving only the prolog/epilog. Self-assignments
 * are the smallest C that reproduces that - a plain empty body compiles to a
 * bare blr with no frame.
 */
void fn_1_C360(void *obj, s16 winId)
{
    s32 scratch0;
    s32 scratch1;
    f64 scratch2;

    scratch0 = scratch0;
    scratch1 = scratch1;
    scratch2 = scratch2;
}