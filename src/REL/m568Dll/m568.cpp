#include "dolphin/types.h"

/*
 * m568.cpp holds the module-wide glue: the exit request flag, the two
 * Hu3DData slot accessors, the linker pair that is re-inserted at the head
 * of a list, and one virtual-table install.
 *
 * game/hu3d.h is deliberately not included. The two Hu3D accessors only need
 * the element stride (0x134) and one s32 slot, and pulling in humath.h makes
 * MWCC emit unused sqrtf local statics into .rodata. Hu3DData itself is
 * imported from the game module (nm reports it undefined in the original
 * object), so it stays extern and is declared here with the layout derived
 * from the offsets both accessors use.
 *
 * lbl_1_data_214 is a real definition in the original object (nm reports D),
 * so it is defined here rather than declared extern; its two tail entries are
 * module-local handlers that are out of scope for this file and stay extern.
 */

typedef struct {
    u8 unk_00[0xF0];
    void *unk_F0;  // 0xF0
    u8 unk_F4[0x2C];
    s32 unk_120;   // 0x120
    u8 unk_124[0x10];
} M568Model;

extern "C" {

extern s16 omSysExitReq;
extern M568Model Hu3DData[];

// Doubly-linked node, self-referencing once unlinked.
typedef struct {
    void *prev;  // 0x00
    void *next;  // 0x04
} M568Link;

void fn_1_4E8C(void *self);
void fn_1_4F4C(void *self);
void fn_1_4ED8(void *self);

// 0x00 and 0x04 are left zero by the original; only the two handler slots
// are relocated.
void *lbl_1_data_214[4] = { 0, 0, (void *)fn_1_4F4C, (void *)fn_1_4ED8 };

void fn_1_325C(void) {
    omSysExitReq = 1;
}

void fn_1_36D4(s16 *modelId, s32 value) {
    Hu3DData[*modelId].unk_120 = value;
}

void **fn_1_36F0(s16 *modelId) {
    return &Hu3DData[*modelId].unk_F0;
}

void fn_1_4E6C(s32 *vtableSlot, void *self) {
    *vtableSlot = (s32)&lbl_1_data_214[0];
    *(s32 *)((u8 *)self + 0x5C) = (s32)vtableSlot;
    *(void **)((u8 *)self + 0x14) = (void *)fn_1_4E8C;
}

void fn_1_18F4(M568Link *link) {
    // Unlink: the node is spliced out of a doubly-linked circle and left
    // pointing at itself in both directions. No local is taken - the original
    // reloads both neighbours from `link` for each of the two relink stores.
    ((M568Link *)link->prev)->next = link->next;
    ((M568Link *)link->next)->prev = link->prev;
    link->prev = (void *)link;
    link->next = (void *)link;
}

}