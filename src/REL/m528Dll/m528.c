// REL/m528Dll/m528.c - decompilation of the m528Dll/REL/m528Dll/m528 unit.
//
// m528 is a Mario Party 5 board minigame REL.  ObjectSetup (not yet
// decompiled) calls omInitObjMan/omGameSysInit, registers one main object with
// omAddObjEx(0x65, fn_1_13D0) and four player objects with
// omAddObjEx(0x64, fn_1_113C), and hangs the state block described below off
// OMOBJ.data (lbl_1_bss_298->data) after
// HuMemDirectMallocNum(0, 0x4D70, 0x1000).  It also hands lbl_1_data_0 -- a
// MGSEQ_PARAM with maxTime 300 -- to MgSeqCreate, which is how fn_1_1E8,
// fn_1_1118 and fn_1_111C are reached (init, fadeOut and close hooks).
//
// Codegen notes worth carrying to the rest of the unit, all measured here:
//  - the unit is compiled -O0,p, so dead stores to stack locals survive and a
//    `volatile` local is the honest way to reproduce one (fn_1_111C, fn_1_1368);
//  - the trailing -O0,p overrides the earlier -O4,p;
//  - objdiff prints rlwinm mask fields MSB-first, so `rlwinm r3, r0, 0, 23, 23`
//    extracts value bit 8, i.e. `x & 0x100` -- MWCC models a single-bit mask as
//    a rotate-mask rather than an andi (fn_1_2704);
//  - `--s16field` avoids the extsh that `s16field = s16field - 1` inserts
//    (fn_1_32D0).

#include "types.h"
#include "game/object.h"
#include "game/mg/seqman.h"
#include "game/pad.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* Two callbacks are stashed in the work block.  unk1C is installed as the
 * fn_1_2080 hook and is called with the block itself; unk3CC is called with the
 * address of the block's 0x20 sub-object and its non-zero result clears the
 * slot, so its return value is observed. */
typedef void (*M528Step)(struct m528Work_s *);
typedef s32 (*M528Probe)(void *);

/* Node of the doubly linked lists hung off M528LIST.  fn_1_4FF4 takes one off
 * unk3E0 and pushes it on unk3DC with the two caller-supplied payloads; fn_1_5048
 * unlinks it again. */
typedef struct m528Node_s {
    /* 0x00 */ struct m528Node_s *prev;
    /* 0x04 */ struct m528Node_s *next;
    /* 0x08 */ s32 val08;
    /* 0x0C */ s32 val0C;
} M528NODE;

typedef struct m528List_s {
    u8 pad000[0x3DC];
    /* 0x3DC */ M528NODE *unk3DC;
    /* 0x3E0 */ M528NODE *unk3E0;
} M528LIST;

/* Per-sequence working state, HuMemDirectMallocNum(0, 0x4D70, 0x1000) in
 * ObjectSetup and stored in OMOBJ.data.  Only the fields this unit touches are
 * named; everything in between is padding, so the offsets below stay exact
 * without claiming to have recovered the whole 0x4D70-byte layout. */
typedef struct m528Work_s {
    /* 0x00 */ u8 pad00[0x1C];
    /* 0x1C */ M528Step unk1C;
    /* 0x20 */ u8 pad20[0x0E];
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ u8 pad38[0x384];
    /* 0x3BC */ u8 pad3BC[0x10];
    /* 0x3CC */ M528Probe unk3CC;
    /* 0x3D0 */ u8 pad3D0[0xC];
    /* 0x3DC */ M528NODE *unk3DC;
    /* 0x3E0 */ M528NODE *unk3E0;
    /* 0x3E4 */ u8 pad3E4[0x48C4];
    /* 0x4CA8 */ s16 unk4CA8;
    /* 0x4CAA */ s16 unk4CAA;
    /* 0x4CAC */ s16 unk4CAC;
    /* 0x4CAE */ u8 pad4CAE[0x4];
    /* 0x4CB2 */ s16 unk4CB2;
    /* 0x4CB4 */ s16 unk4CB4[0x36];
    /* 0x4D20 */ s16 unk4D20[7];
    /* 0x4D2E */ s16 unk4D2E[0x10];
    /* 0x4D4E */ s16 unk4D4E[0x10];
    /* 0x4D6E */ s16 unk4D6E;
} M528WORK;

/* Countdown-then-fade object handed to fn_1_32D0. */
typedef struct m528Fade_s {
    u8 pad00[0x10];
    /* 0x10 */ float alpha;
    /* 0x14 */ u8 pad14[0x4];
    /* 0x18 */ s16 waitFrames;
} M528FADE;

/* ---- bss globals (m528) ---- */
static s32 lbl_1_bss_0;
static s32 lbl_1_bss_4[0x294 / 4];
static OMOBJ *lbl_1_bss_298;
static s32 lbl_1_bss_29C;

void ObjectSetup(void);

int _prolog(void)
{
    const VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void fn_1_1E8(s16 mode, s16 frameNo)
{
    M528WORK *work = lbl_1_bss_298->data;

    work->unk4CAC = 0;
    work->unk4CA8 = -1;
    work->unk4CAA = -1;
    MgSeqModeNext();
}

void fn_1_1118(s16 mode, s16 frameNo) {
}

/* The whole compiled body is the read of the sequence work block off
 * lbl_1_bss_298 into a stack slot that is never read back, so the local has to
 * be memory-resident to reproduce the frame and the spill. */
void fn_1_111C(s16 mode, s16 frameNo) {
    M528WORK *volatile work = lbl_1_bss_298->data;
}

/* The dead read of the main object's work block into a never-used slot comes
 * first, then the object's own block drives the optional hook. */
void fn_1_1368(OMOBJ *obj)
{
    M528WORK *work = obj->data;
    M528WORK *volatile main = lbl_1_bss_298->data;

    if (work->unk1C != 0) {
        work->unk1C(work);
    }
}

void fn_1_13CC(s16 mode, s16 frameNo) {
}

void fn_1_1534(s16 mode, s16 frameNo) {
}

void fn_1_18E0(M528WORK *work) {
    s32 i;

    for (i = 0; i < 7; i++) {
        work->unk4D20[i] = -1;
    }
}

/* Clears the per-player state tables: 0x36 entries from 0x4CB4, then 0x10
 * entries each at 0x4D2E and 0x4D4E. */
void fn_1_1874(M528WORK *work)
{
    s32 i;

    for (i = 0; i < 0x36; i++) {
        work->unk4CB4[i] = -1;
    }

    for (i = 0; i < 0x10; i++) {
        work->unk4D2E[i] = -1;
        work->unk4D4E[i] = -1;
    }
}

/* Reset hook: clear the pending probe at 0x3CC and the two frame counters. */
void fn_1_8E4(void)
{
    M528WORK *work = lbl_1_bss_298->data;

    if (work->unk3CC != 0) {
        if (work->unk3CC(&work->pad20[0]) != 0) {
            work->unk3CC = 0;
        }
    }
    work->unk4CAC = 0;
    work->unk4D6E = 0;
}

/* Either decrement the wait counter or scale the alpha down by 0.95 and snap it
 * to zero once it falls under 0.01. */
void fn_1_32D0(M528FADE *fade)
{
    if (fade->waitFrames != 0) {
        --fade->waitFrames;
        return;
    }

    fade->alpha *= 0.95f;

    if (fade->alpha < 0.01f) {
        fade->alpha = 0.0f;
    }
}

/* Pad-gated input: returns 0 unless the main object has its input block armed
 * (0x4CB2), else returns the matching bits straight out of the given pad's
 * freshly-pressed buttons -- 0x100 here, 0x140 for fn_1_2758 below.  Both
 * callers only test the result against 0, so neither normalises the mask. */
s32 fn_1_2704(M528WORK *work)
{
    M528WORK *main = lbl_1_bss_298->data;

    if (main->unk4CB2 == 0) {
        return 0;
    }
    return HuPadBtnDown[work->unk2E] & 0x100;
}

s32 fn_1_2758(M528WORK *work)
{
    M528WORK *main = lbl_1_bss_298->data;

    if (main->unk4CB2 == 0) {
        return 0;
    }
    return HuPadBtnDown[work->unk2E] & 0x140;
}

/* Move the head of the free list (0x3E0) onto the active list (0x3DC) and hand
 * the caller its two payloads. */
M528NODE *fn_1_4FF4(M528LIST *list, s32 val08, s32 val0C)
{
    M528NODE *node = list->unk3E0;

    if (node != 0) {
        list->unk3E0 = node->next;
        node->val08 = val08;
        node->val0C = val0C;
        node->next = list->unk3DC;

        if (node->next != 0) {
            node->next->prev = node;
        }
        list->unk3DC = node;
    }
    return node;
}

/* Unlink a node from the active list (0x3DC) and push it back onto 0x3E0. */
void fn_1_5048(M528LIST *list, M528NODE *node)
{
    if (node != 0) {
        if (node->prev != 0) {
            node->prev->next = node->next;
        } else {
            list->unk3DC = node->next;
        }

        if (node->next != 0) {
            node->next->prev = node->prev;
        }
        node->prev = 0;
        node->next = list->unk3E0;
        node->val08 = 0;
        list->unk3E0 = node;
        node = 0;
    }
}

void fn_1_5194(M528WORK *work) {
    work->unk30 = 0;
    work->unk36 = -1;
}