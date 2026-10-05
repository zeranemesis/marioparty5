// m515Dll / m515
//
#include "types.h"
#include "game/hu3d.h"
#include "game/esprite.h"
#include "game/mg/seqman.h"

// The unit has no source in the main checkout yet, so this file is being built
// up function by function. Everything here is compiled with the REL flags
// (-O0,p -sym on), which is why the bodies below are deliberately literal.
//
// Three object layouts are needed so far. None is fully known, so the
// unobserved regions are explicit padding: the field offsets are what the
// emitted lha/sth/lwz/stw displacements depend on, and the field WIDTHS decide
// between lha/lhz and sth, and lwz and stw, so those must be right.
//
//   M515_SLOT  stride 0x30, addressed as obj->slot[i] at obj+0x234
//   M515_SUB   the per-object state block, reached through obj->sub (obj+0x5c)
//   M515_ENTRY stride 0x10, addressed as obj->entry[i] at obj+0x2f4
//
// fn_1_60E8 copies obj->sub->vec from a slot's vec field with a single struct
// assignment, which is what puts the first two words in volatile temps
// (r5/r0) instead of loading and storing each word through r0. Keeping the two
// sides as separate embedded structs of the same type is load-bearing, not
// cosmetic.

typedef struct M515V3_s {
    s32 x;
    s32 y;
    s32 z;
} M515_V3;

typedef struct M515Slot_s {
    char pad_00[0x18];
    M515_V3 vec;
} M515_SLOT;

typedef struct M515Entry_s {
    s16 e00;
    s16 e02;
    M515_SLOT *slot;
    s32 e08;
    s32 e0c;
} M515_ENTRY;

typedef struct M515EspPair_s {
    s16 id0;
    s16 id1;
} M515_ESPPAIR;

// Model-ID table read by fn_1_CAF0. Only the three offsets it touches matter.
typedef struct M515IdTriple_s {
    s16 first;
    char pad_02[4];
} M515_IDTRIPLE;

typedef struct M515Ids_s {
    char pad_00[0x26];
    s16 run1[0x40];
    s16 run2[8];
    M515_IDTRIPLE run3[4];
} M515_IDS;

// Doubly-linked node behind the two 0x344/0x348 list heads in M515_SUB.
// fn_1_5CE4 pops off 0x348 and pushes onto 0x344; fn_1_5D48 splices a node out
// and pushes it onto 0x348.
typedef struct M515Node2_s {
    s16 f00;
    s16 f02;
    s16 f04;
    char pad_06[2];
    s32 f08;
    struct M515Node2_s *prev;
    struct M515Node2_s *next;
} M515_NODE2;

typedef struct M515Sub_s M515_SUB;

// Per-node continuation hook, stored at sub+0x688 and called with the node and
// the owning sub. fn_1_B0B8 is one of the two functions ever stored there.
typedef s32 (*M515_HOOKFN)(M515_NODE2 *node, M515_SUB *self);
s32 fn_1_B0B8(M515_NODE2 *node, M515_SUB *self);

typedef struct M515Sub_s {
    char pad_000[0xc];
    s16 modelId;
    char pad_00e[0x1d0 - 0xe];
    s16 v1d0;
    s16 v1d2;
    char pad_1d4[2];
    s16 v1d6;
    char pad_1d8[2];
    s16 v1da;
    char pad_1dc[0x1f4 - 0x1dc];
    s32 v1f4;
    char pad_1f8[0x334 - 0x1f8];
    M515_ESPPAIR esp;
    char pad_338[0x344 - 0x338];
    M515_NODE2 *head;
    M515_NODE2 *cur;
    char pad_34c[0x3f8 - 0x34c];
    M515_SLOT *p3f8;
    char pad_3fc[0x74];
    M515_V3 vec;
    char pad_47c[2];
    s16 v47e;
    s16 v480;
    s16 v482;
    s16 v484;
    s16 v486;
    char pad_488[0x680 - 0x488];
    u16 v680;
    s16 v682;
    M515_NODE2 *p684;
    M515_HOOKFN hook;
    char pad_68c[0xcac - 0x68c];
    s16 v0cac;
} M515_SUB;

typedef struct M515Obj_s {
    char pad_000[0x5c];
    M515_SUB *sub;
} M515_OBJ;

// .bss:0x4, 4 bytes. fn_1_928 and fn_1_2684 both go through it rather than
// through their own argument, so it is the one live object pointer.
M515_OBJ *lbl_1_bss_4;

// .rodata:0x54. Naming it keeps the lis/addi/lfs triple comparing equal to the
// target; a bare 0.0f literal becomes an unnamed pool entry (@359) instead.
f32 lbl_1_rodata_54 = 0.0f;

// The remaining float literals fn_1_2EB4 loads. Only the values matter; the
// .rodata offsets they end up at are not compared.
f32 lbl_1_rodata_F0 = 4800.0f;
f32 lbl_1_rodata_F4 = 3600.0f;
f32 lbl_1_rodata_F8 = -1.0f;

// 00000928, 64 bytes. Third entry of lbl_1_data_0; hides the object behind the
// global pointer and turns the model's display attribute off. The table's hook
// type is void(*)(s16 mode, s16 frameNo), so the incoming pair is ignored.
void fn_1_928(void)
{
    M515_SUB *sub = lbl_1_bss_4->sub;

    Hu3DModelAttrSet(sub->modelId, HU3D_ATTR_DISPOFF);
}

// Defined below: both take M515_SUB and are far too big to land in this pass,
// so only their prototypes live here for now.
void fn_1_32C4(M515_SUB *sub);
void fn_1_CE98(M515_SUB *sub);

// Defined below, at 00009920; fn_1_1D24 calls it from further up the file.
void fn_1_9920(M515_ESPPAIR *esp);

// Handler table at .data:0x0 (lbl_1_data_0) ends with these two entries:
//   .4byte fn_1_1B58
//   .4byte fn_1_1D20
//   .4byte fn_1_1D24
// fn_1_1D20 is the last-but-one slot of that table and does nothing: the
// original body is a single `blr`, i.e. a leaf function with no frame and no
// locals.
void fn_1_1D20(void)
{
}

// 000001FC, 112 bytes. Startup: clears the four 0x1d0-range halfwords and the
// 0x1f4 word in a non-sequential order (0x1d0, 0x1da, 0x1d2, 0x1d6), then builds
// the two big tables and hands the sequence manager on. The odd store order is
// in the original, so the assignments stay in that order.
void fn_1_1FC(void)
{
    M515_SUB *sub = lbl_1_bss_4->sub;

    sub->v1d0 = 0;
    sub->v1da = 0;
    sub->v1d2 = 0;
    sub->v1d6 = -1;
    sub->v1f4 = 0;
    fn_1_32C4(sub);
    fn_1_CE98(sub);
    MgSeqModeNext();
}

// 00001D24, 60 bytes. Last entry of lbl_1_data_0; tears down the two ESPs
// recorded at sub+0x334.
void fn_1_1D24(void)
{
    M515_SUB *sub = lbl_1_bss_4->sub;

    fn_1_9920(&sub->esp);
}

// fn_1_29CC and fn_1_2B68 are the same shape (4 bytes, one `blr`) but are not
// referenced from lbl_1_data_0; they are reached only through the DLL's own
// internal dispatch, so they stay anonymous here too.

// Sized by the original: 000029CC, 4 bytes, `blr`.
void fn_1_29CC(void)
{
}

// Sized by the original: 00002B68, 4 bytes, `blr`.
void fn_1_2B68(void)
{
}

// 00002AC0, 168 bytes.
void fn_1_2AC0(M515_OBJ *obj);

// 00005E88, 164 bytes. Six-parameter helper: the fifth and sixth arguments are
// spilled to 0x8(r1)/0xa(r1) by the prologue and reloaded with lha/lhz, so the
// declared types have to be s16 and u16 respectively -- two s16 parameters would
// reload both of them with lha.
void fn_1_5E88(HU3DMODELID modelId, HuVecF *pos, HuVecF *rot, s16 layerNo, u16 cameraBit)
{
    Hu3DModelAttrSet(modelId, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(modelId, 0x40000002);
    Hu3DModelLayerSet(modelId, layerNo);
    Hu3DModelCameraSet(modelId, cameraBit);
    Hu3DModelPosSet(modelId, pos->x, pos->y, pos->z);
    Hu3DModelRotSet(modelId, rot->x, rot->y, rot->z);
}

// 00005F2C, 164 bytes. Same shape as fn_1_5E88 with the second attribute set to
// 0x40000080 instead of 0x40000002.
void fn_1_5F2C(HU3DMODELID modelId, HuVecF *pos, HuVecF *rot, s16 layerNo, u16 cameraBit)
{
    Hu3DModelAttrSet(modelId, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(modelId, 0x40000080);
    Hu3DModelLayerSet(modelId, layerNo);
    Hu3DModelCameraSet(modelId, cameraBit);
    Hu3DModelPosSet(modelId, pos->x, pos->y, pos->z);
    Hu3DModelRotSet(modelId, rot->x, rot->y, rot->z);
}

// 00007768, 172 bytes. Shows every model ID of row `r` whose ID is non-negative.
// The index has to be written as a byte expression: MWCC then emits
// `mulli r,0x32` / `slwi i,1` / `add` / `addi 0x5ea` and folds nothing. The same
// index written as a plain s16 subscript gives `mulli r,0x19` / `add` /
// `addi 0x2f5` / `slwi 1` instead -- same address, four different rows.
void fn_1_7768(s16 *ids, s32 r)
{
    s32 i;

    for (i = 0; i < 25; i++) {
        if (*(s16 *)((char *)ids + r * 0x32 + i * 2 + 0x5ea) >= 0) {
            Hu3DModelAttrReset(*(s16 *)((char *)ids + r * 0x32 + i * 2 + 0x5ea),
                               HU3D_ATTR_DISPOFF);
        }
    }
}

// 000077F0, 172 bytes. fn_1_7768 with Hu3DModelAttrSet instead of AttrReset.
void fn_1_77F0(s16 *ids, s32 r)
{
    s32 i;

    for (i = 0; i < 25; i++) {
        if (*(s16 *)((char *)ids + r * 0x32 + i * 2 + 0x5ea) >= 0) {
            Hu3DModelAttrSet(*(s16 *)((char *)ids + r * 0x32 + i * 2 + 0x5ea),
                             HU3D_ATTR_DISPOFF);
        }
    }
}

// 00005C40, 164 bytes.
void fn_1_5C40(M515_SUB *sub);

// 00007878, 172 bytes. Runs the object's continuation hook once. If it reports
// work done and the global object's 0xcac flag is clear the hook is dropped and
// 1 is returned; otherwise the hook is replaced by fn_1_B0B8 and the node's
// first halfword is cleared.
s32 fn_1_7878(M515_SUB *self)
{
    s32 ret = 0;
    M515_SUB *global = lbl_1_bss_4->sub;

    if (self->hook != 0) {
        if (self->hook(self->p684, self) != 0) {
            if (global->v0cac == 0) {
                self->hook = 0;
                ret = 1;
            } else {
                self->hook = fn_1_B0B8;
                self->p684->f00 = 0;
            }
        }
    }

    return ret;
}

// 00002EB4, 136 bytes. Builds the key light and marks it infinite. Returns
// nothing: the original stores the light ID in r31, passes it to the setter and
// then falls out without moving it back to r3.
void fn_1_2EB4(void)
{
    HU3DLIGHTID lightId;

    lightId = Hu3DGLightCreate(lbl_1_rodata_54, lbl_1_rodata_F0, lbl_1_rodata_F4,
                               lbl_1_rodata_54, lbl_1_rodata_F8, lbl_1_rodata_F8,
                               0xFF, 0xFF, 0xFF);
    Hu3DGLightInfinitytSet(lightId);
}

// 00005CE4, 100 bytes. Pops the node at sub->cur off the free list and pushes it
// onto sub->head. `node->next` is read three times -- once for the assignment,
// once for the test and once for the store address -- so it must stay three
// separate references rather than a cached pointer.
M515_NODE2 *fn_1_5CE4(M515_SUB *sub)
{
    M515_NODE2 *node = sub->cur;

    if (node != 0) {
        sub->cur = node->next;
        node->f00 = -1;
        node->f02 = -1;
        node->f04 = 0;
        node->next = sub->head;
        if (node->next != 0) {
            node->next->prev = node;
        }
        sub->head = node;
    }

    return node;
}

// 00005D48, 104 bytes. Splices a node out of sub->head and pushes it onto
// sub->cur, reinitialising the payload fields on the way. The original returns
// void and its last statement is a dead `node = 0`; that assignment is what
// emits the lone `li r4,0x0` before the shared `blr`, because at -O0 MWCC
// materialises a store to a parameter even when nothing reads it back. Dropping
// it and returning `0` instead moves the constant to r3 and costs 10 rows.
void fn_1_5D48(M515_SUB *sub, M515_NODE2 *node)
{
    if (node != 0) {
        if (node->prev != 0) {
            node->prev->next = node->next;
        } else {
            sub->head = node->next;
        }

        if (node->next != 0) {
            node->next->prev = node->prev;
        }

        node->prev = 0;
        node->next = sub->cur;
        node->f08 = 0;
        sub->cur = node;
        node = 0;
    }
}

// 000060E8, 52 bytes. Adopts the slot at +0x3f8 only while the 0x484 field is
// still sitting on 15, and consumes the slot by clearing it. Not called from
// anywhere inside this unit and not in lbl_1_data_0, so the argument pair is
// taken from the surrounding state machine's shape rather than from a caller.
void fn_1_60E8(M515_SUB *sub, M515_SLOT *slot)
{
    if (sub->v484 == 15) {
        sub->vec = slot->vec;
        sub->p3f8 = slot;
        sub->v484 = 0;
    }
}

// 000062FC, 28 bytes. Writes the halfword argument through only while the 0x486
// field is still 5, then consumes that field. Like fn_1_60E8 it has no in-unit
// caller and is not in lbl_1_data_0, so the second argument is read off the
// state machine rather than off a call site.
void fn_1_62FC(M515_SUB *sub, s16 value)
{
    if (sub->v486 == 5) {
        sub->v47e = value;
        sub->v486 = 0;
    }
}

// 00006318, 12 bytes. Entering state 3 clears the 0x480 field; fn_1_2684 does
// `sub->v482 = 3; fn_1_6318(sub); sub->v682 = 0;`.
void fn_1_6318(M515_SUB *sub)
{
    sub->v480 = 0;
}

// 00006980, 56 bytes. Counts the 0x682 timer down, then reloads it to 5 when
// bit 4 of the 0x680 flags word is set. `rlwinm r0,r0,0,27,27` is MWCC's way of
// spelling `& 0x10`: rlwinm numbers its mask bits from the MSB, so printed bit
// 27 is value bit 4.
void fn_1_6980(M515_SUB *sub)
{
    if (sub->v682 > 0) {
        sub->v682--;
    }

    switch (sub->v680 & 0x10) {
    case 0x10:
        sub->v682 = 5;
        break;
    }
}

// 00009804, 36 bytes. Resets one entry of the obj+0x2f4 array and points it at
// the matching slot of the obj+0x234 array. The three zero stores are three
// separate `li r0,0` in the original, so they must stay three separate
// assignments in source order.
void fn_1_9804(M515_ENTRY *entry, M515_SLOT *slot, s32 index)
{
    entry->e02 = 0;
    entry->e00 = 0;
    entry->slot = slot;
    entry->e0c = index;
    entry->e08 = 0;
}

// 00009920, 56 bytes. Kills both sprite IDs of an entry. Called on sub+0x334,
// so the argument type is a two-halfword ESP record, not M515_ENTRY.
void fn_1_9920(M515_ESPPAIR *esp)
{
    espKill(esp->id0);
    espKill(esp->id1);
}

// 0000C26C, 68 bytes. Returns 1, hiding a live model first.
s32 fn_1_C26C(M515_ENTRY *entry)
{
    if (entry->e00 >= 0) {
        Hu3DModelAttrSet(entry->e00, HU3D_ATTR_DISPOFF);
    }

    return 1;
}

// 0000C0D0, 128 bytes. Starts the entry's motion at time 0.0f and flags the
// model. Returns 1 unconditionally.
s32 fn_1_C0D0(M515_ENTRY *entry)
{
    if (entry->e00 >= 0) {
        Hu3DModelAttrReset(entry->e00, HU3D_ATTR_DISPOFF);
        entry->e02 = Hu3DMotionIDGet(entry->e00);
        if (entry->e02 >= 0) {
            Hu3DMotionTimeSet(entry->e00, lbl_1_rodata_54);
            Hu3DModelAttrSet(entry->e00, 0x40000002);
        }
    }

    return 1;
}

// 0000CAF0, 112 bytes. Turns the display attribute back on for the three model
// IDs belonging to entry `i`. All three displacements are struct-relative
// offsets folded into the lhax index, so the base is a struct with an s16 run at
// 0x26, another at 0xa6 and a six-byte-stride group at 0xb6 -- not a bare s16
// array, which makes MWCC scale the index before adding the offset instead.
void fn_1_CAF0(M515_IDS *ids, s32 i)
{
    Hu3DModelAttrReset(ids->run3[i].first, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(ids->run1[i], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(ids->run2[i], HU3D_ATTR_DISPOFF);
}