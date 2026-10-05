#include "types.h"

// m536 top-level unit. It owns the 16-byte vtable lbl_1_data_1D4 (two null
// slots, then fn_1_4934 and fn_1_48C0), which fn_1_4854 installs into the
// freshly built object. fn_1_30BC and fn_1_30D8 are the two accessors for the
// 0x134-byte Hu3D per-model record: one writes the field at 0x120, the other
// hands back the sub-record based at 0xF0. fn_1_12DC splices a list node into
// itself (both links end up pointing at the node itself).
extern "C" {

typedef void (*M536VtableFn)(void);

void fn_1_4874(void);
void fn_1_48C0(void);
void fn_1_4934(void);

// 16 bytes at .data+0x1D4: two null slots then two handler pointers.
struct M536Vtable {
    void *m_unk0;
    void *m_unk4;
    M536VtableFn m_fn8;
    M536VtableFn m_fnC;
};

M536Vtable lbl_1_data_1D4 = {0, 0, fn_1_4934, fn_1_48C0};

// Doubly-linked list node; fn_1_12DC is the self-splice.
struct M536Node {
    M536Node *m_next;
    M536Node *m_prev;
};

// The 0x30-byte tail block of a Hu3D record, based at offset 0xF0.
struct M536Hu3DPart {
    s32 m_unk00[0x30 / 4];
};

// One per-model Hu3D record, 0x134 bytes.
struct M536Hu3DSlot {
    s32 m_unk00[0xF0 / 4];
    M536Hu3DPart m_part;
    s32 m_value;
    s32 m_unk124[4];
};

extern M536Hu3DSlot Hu3DData[];
extern s16 omSysExitReq;

void fn_1_12DC(M536Node *node)
{
    node->m_next->m_prev = node->m_prev;
    node->m_prev->m_next = node->m_next;
    node->m_next = node;
    node->m_prev = node;
}

void fn_1_2C44(void)
{
    omSysExitReq = 1;
}

void fn_1_30BC(s16 *index, s32 value)
{
    Hu3DData[*index].m_value = value;
}

void *fn_1_30D8(s16 *index)
{
    return &Hu3DData[*index].m_part;
}

// Constructed object: the vtable goes into the object, the object is handed to
// its container at 0x5C, and the container's entry point at 0x14 is set.
struct M536Object {
    void *m_vtable;
};

struct M536Container {
    s32 m_unk00[0x14 / 4];
    M536VtableFn m_fn14;
    s32 m_unk18[(0x5C - 0x18) / 4];
    void *m_unk5C;
};

void fn_1_4854(M536Object *object, M536Container *container)
{
    object->m_vtable = &lbl_1_data_1D4;
    container->m_unk5C = object;
    container->m_fn14 = fn_1_4874;
}

}
