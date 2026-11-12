// m515Dll / m515
//
#include "types.h"
#include "game/hu3d.h"
#include "game/esprite.h"
#include "game/object.h"
#include "game/memory.h"
#include "game/audio.h"
#include "game/charman.h"
#include "game/mg/seqman.h"

typedef struct M515Slot_s M515_SLOT;
typedef struct M515Node2_s M515_NODE2;
typedef struct M515Sub_s M515_SUB;
typedef struct M515Ply_s M515_PLY;

// Board cell, stride 0x30, at sub+0x234. Neighbours are reached through the
// four pointers at the start.
struct M515Slot_s {
    M515_SLOT *nbr[4];
    char pad_10[0x8];
    HuVecF vec;
    char pad_24[0x30 - 0x24];
};

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

// Intrusive doubly-linked node (stride 0x14) behind the head/cur lists of M515_SUB.
struct M515Node2_s {
    s16 f00;
    s16 f02;
    s16 f04;
    char pad_06[2];
    s32 (*func)(M515_NODE2 *node);
    M515_NODE2 *prev;
    M515_NODE2 *next;
};

// Per-player continuation hook, stored at ply+0x688.
typedef s32 (*M515_HOOKFN)(M515_NODE2 *node, M515_PLY *self);
s32 fn_1_B0B8(M515_NODE2 *node, M515_PLY *self);
s32 fn_1_9DF8(M515_NODE2 *node, M515_PLY *self);

// Global state block (0xcb0 bytes) hung off the first object's data.
struct M515Sub_s {
    s16 v00;
    char pad_002[0xa];
    s16 modelId;
    char pad_00e[0x1d0 - 0xe];
    s16 v1d0;
    s16 v1d2;
    char pad_1d4[2];
    s16 v1d6;
    char pad_1d8[2];
    s16 v1da;
    s16 v1dc[4][3];
    s32 v1f4;
    char pad_1f8[0x224 - 0x1f8];
    M515_PLY *ply[4];
    M515_SLOT slot[4];
    M515_ENTRY entry[4];
    M515_ESPPAIR esp;
    char pad_338[0x344 - 0x338];
    M515_NODE2 *head;
    M515_NODE2 *cur;
    M515_NODE2 node[0x78];
    s16 v0cac;
};

// Per-player block (0x6a0 bytes).
struct M515Ply_s {
    s16 idx;
    s16 chr;
    char pad_004[0x8];
    s16 modelId;
    s16 v0e;
    char pad_010[0x3f8 - 0x10];
    M515_SLOT *p3f8;
    char pad_3fc[0x44c - 0x3fc];
    HuVecF pos;
    char pad_458[4];
    f32 v45c;
    char pad_460[4];
    HuVecF from;
    HuVecF vec;
    s16 v47c;
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
    char pad_68c[0x6a0 - 0x68c];
};

typedef void (*M515_VOIDFN)(void);
extern const M515_VOIDFN _ctors[];
extern const M515_VOIDFN _dtors[];
void ObjectSetup(void);

// .bss:0x0 / 0x4 / 0x8. bss_4 is the one live object pointer.
void *lbl_1_bss_0;
OMOBJ *lbl_1_bss_4;
OMOBJMAN *lbl_1_bss_8;

#define M515_SUBOF(o) ((M515_SUB *)(o)->data)

// .rodata:0x10, camera bit per player.
#ifdef __MWERKS__
#define M515_RODATA __declspec(section ".rodata")
#else
#define M515_RODATA
#endif
M515_RODATA const u16 lbl_1_rodata_10[4] = { 1, 2, 4, 8 };

void fn_1_32C4(M515_SUB *sub);
void fn_1_CE98(M515_SUB *sub);
void fn_1_9920(M515_ESPPAIR *esp);
void fn_1_9828(M515_ESPPAIR *esp);
void fn_1_9958(M515_ESPPAIR *esp);
void fn_1_CC20(s16 layerNo);
void fn_1_7E5C(M515_SLOT *slot, s16 idx);
void fn_1_7EE0(M515_SLOT *slot);
void fn_1_C958(s32 idx, HuVecF *pos, s32 arg);
void fn_1_1FC(s16 mode, s16 frameNo);
void fn_1_26C(s16 mode, s16 frameNo);
void fn_1_928(s16 mode, s16 frameNo);
void fn_1_968(s16 mode, s16 frameNo);
void fn_1_B08(s16 mode, s16 frameNo);
void fn_1_D30(s16 mode, s16 frameNo);
void fn_1_1B58(s16 mode, s16 frameNo);
void fn_1_1D20(s16 mode, s16 frameNo);
void fn_1_1D24(s16 mode, s16 frameNo);
void fn_1_1D60(OMOBJ *obj);
void fn_1_29D0(OMOBJ *obj);
void fn_1_2AC0(OMOBJ *obj);
void fn_1_2B6C(M515_SUB *sub);
void fn_1_2EB4(M515_SUB *sub);
void fn_1_2F3C(M515_SUB *sub);
void fn_1_5C40(M515_SUB *sub);
void fn_1_5DB0(M515_SUB *sub);
void fn_1_C83C(void);
void fn_1_9804(M515_ENTRY *entry, M515_SLOT *slot, s32 index);

MGSEQ_PARAM lbl_1_data_0 = { 300, 0, fn_1_1FC, fn_1_26C, fn_1_928, fn_1_968, fn_1_B08, fn_1_D30,
    fn_1_1B58, fn_1_1D20, fn_1_1D24 };


s32 _prolog(void)
{
    const M515_VOIDFN *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const M515_VOIDFN *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    OMOBJ *obj;
    M515_SUB *sub;
    s32 i;

    lbl_1_bss_8 = omInitObjMan(500, 0x2000);
    omGameSysInit(lbl_1_bss_8);
    obj = omAddObjEx(lbl_1_bss_8, 0x65, 0, 0, -1, fn_1_29D0);
    lbl_1_bss_4 = obj;
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M515_SUB), HU_MEMNUM_OVL);
    sub = obj->data;
    fn_1_2B6C(sub);
    fn_1_2EB4(sub);
    fn_1_C83C();
    for (i = 0; i < 4; i++) {
        obj = omAddObjEx(lbl_1_bss_8, 0x64, 1, 8, 0, fn_1_1D60);
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M515_PLY), HU_MEMNUM_OVL);
        obj->work[0] = i;
        sub->ply[i] = obj->data;
    }
    fn_1_2F3C(sub);
    Hu3DBGColorSet(0, 0, 0);
    Hu3DNoSyncSet(1);
    MgSeqCreate(&lbl_1_data_0);
    sub->v0cac = 0;
}

void fn_1_1FC(s16 mode, s16 frameNo)
{
    M515_SUB *sub = M515_SUBOF(lbl_1_bss_4);

    sub->v1d0 = 0;
    sub->v1da = 0;
    sub->v1d2 = 0;
    sub->v1d6 = -1;
    sub->v1f4 = 0;
    fn_1_32C4(sub);
    fn_1_CE98(sub);
    MgSeqModeNext();
}

void fn_1_928(s16 mode, s16 frameNo)
{
    M515_SUB *sub = M515_SUBOF(lbl_1_bss_4);

    Hu3DModelAttrSet(sub->modelId, HU3D_ATTR_DISPOFF);
}

void fn_1_1D20(s16 mode, s16 frameNo)
{
}

void fn_1_1D24(s16 mode, s16 frameNo)
{
    M515_SUB *sub = M515_SUBOF(lbl_1_bss_4);

    fn_1_9920(&sub->esp);
}

void fn_1_29CC(void)
{
}

void fn_1_29D0(OMOBJ *obj)
{
    s32 i;
    M515_SUB *sub;
    s32 k;

    sub = obj->data;
    for (i = 0; i < 4; i++) {
        fn_1_7E5C(&sub->slot[i], i);
        fn_1_9804(&sub->entry[i], &sub->slot[i], i);
    }
    fn_1_5C40(sub);
    for (i = 0; i < 4; i++) {
        for (k = 0; k < 3; k++) {
            sub->v1dc[i][k] = 0;
        }
    }
    Hu3DLayerHookSet(1, fn_1_CC20);
    obj->objFunc = fn_1_2AC0;
}

void fn_1_2AC0(OMOBJ *obj)
{
    M515_SUB *sub;
    s32 i;
    HuVecF *pp;
    HuVecF pos;

    sub = obj->data;
    fn_1_5DB0(sub);
    for (i = 0; i < 4; i++) {
        fn_1_7EE0(&sub->slot[i]);
        pos = sub->ply[i]->pos;
        pp = &pos;
        fn_1_C958(i, pp, sub->v1f4);
    }
}

void fn_1_2B68(void)
{
}

void fn_1_2B6C(M515_SUB *sub)
{
    s32 i;
    s32 row;
    s32 col;

    sub->v00 = 0;
    for (i = 0; i < 4; i++) {
        row = i / 2;
        col = i % 2;
        Hu3DCameraCreate(lbl_1_rodata_10[i]);
        Hu3DCameraPerspectiveSet(lbl_1_rodata_10[i], 15.0f, 20.0f, 12000.0f, 1.17f);
        Hu3DCameraPosSet(lbl_1_rodata_10[i], 0.0f, 300.0f, 1000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 150.0f,
            0.0f);
    }
    Hu3DCameraViewportSet(lbl_1_rodata_10[0], 8.0f, 20.0f, 320.0f, 240.0f, 0.0f, 1.0f);
    Hu3DCameraViewportSet(lbl_1_rodata_10[1], 312.0f, 20.0f, 320.0f, 240.0f, 0.0f, 1.0f);
    Hu3DCameraViewportSet(lbl_1_rodata_10[2], 8.0f, 220.0f, 320.0f, 240.0f, 0.0f, 1.0f);
    Hu3DCameraViewportSet(lbl_1_rodata_10[3], 312.0f, 220.0f, 320.0f, 240.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(lbl_1_rodata_10[0], 0, 0, 319, 239);
    Hu3DCameraScissorSet(lbl_1_rodata_10[1], 321, 0, 319, 239);
    Hu3DCameraScissorSet(lbl_1_rodata_10[2], 0, 241, 319, 239);
    Hu3DCameraScissorSet(lbl_1_rodata_10[3], 321, 241, 319, 239);
    fn_1_9828(&sub->esp);
    fn_1_9958(&sub->esp);
}

void fn_1_2EB4(M515_SUB *sub)
{
    HU3DLIGHTID lightId;

    lightId = Hu3DGLightCreate(0.0f, 4800.0f, 3600.0f, 0.0f, -1.0f, -1.0f, 0xFF, 0xFF, 0xFF);
    Hu3DGLightInfinitytSet(lightId);
}

void fn_1_5C40(M515_SUB *sub)
{
    s32 i;

    for (i = 0; i < 0x77; i++) {
        sub->node[i].prev = 0;
        sub->node[i].next = &sub->node[i + 1];
        sub->node[i].func = 0;
    }
    sub->node[i].prev = 0;
    sub->node[i].next = 0;
    sub->node[i].func = 0;
    sub->head = 0;
    sub->cur = &sub->node[0];
}

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

// The dead `node = 0` store is what emits the lone li before the blr.
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
        node->func = 0;
        sub->cur = node;
        node = 0;
    }
}

void fn_1_5DB0(M515_SUB *sub)
{
    M515_NODE2 *node;
    M515_NODE2 *next;

    node = sub->head;
    while (node != 0) {
        next = node->next;
        if (node->func(node) != 0) {
            fn_1_5D48(sub, node);
        }
        node = next;
    }
}

void fn_1_5E88(HU3DMODELID modelId, HuVecF *pos, HuVecF *rot, s16 layerNo, u16 cameraBit)
{
    Hu3DModelAttrSet(modelId, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(modelId, 0x40000002);
    Hu3DModelLayerSet(modelId, layerNo);
    Hu3DModelCameraSet(modelId, cameraBit);
    Hu3DModelPosSet(modelId, pos->x, pos->y, pos->z);
    Hu3DModelRotSet(modelId, rot->x, rot->y, rot->z);
}

void fn_1_5F2C(HU3DMODELID modelId, HuVecF *pos, HuVecF *rot, s16 layerNo, u16 cameraBit)
{
    Hu3DModelAttrSet(modelId, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(modelId, 0x40000080);
    Hu3DModelLayerSet(modelId, layerNo);
    Hu3DModelCameraSet(modelId, cameraBit);
    Hu3DModelPosSet(modelId, pos->x, pos->y, pos->z);
    Hu3DModelRotSet(modelId, rot->x, rot->y, rot->z);
}

s32 fn_1_5FD0(M515_PLY *ply)
{
    f32 t;
    s32 ret;

    ret = 0;
    ply->v484++;
    if (ply->v484 >= 15) {
        ply->v484 = 15;
        ply->from = ply->vec;
        ret = 1;
    }
    t = (f32)ply->v484 / 15.0f;
    ply->pos.x = (1.0f - t) * ply->from.x + t * ply->vec.x;
    ply->pos.y = (1.0f - t) * ply->from.y + t * ply->vec.y;
    ply->pos.z = (1.0f - t) * ply->from.z + t * ply->vec.z;
    return ret;
}

void fn_1_60E8(M515_PLY *ply, M515_SLOT *slot)
{
    if (ply->v484 == 15) {
        ply->vec = slot->vec;
        ply->p3f8 = slot;
        ply->v484 = 0;
    }
}

s32 fn_1_611C(M515_PLY *ply)
{
    s32 ret = 0;
    f32 t;
    f32 ang[4] = { 180.0f, 90.0f, 270.0f, 0.0f };

    ply->v486++;
    if (ply->v486 >= 5) {
        ply->v486 = 5;
        ply->v47c = ply->v47e;
        ret = 1;
    }
    if ((ply->v47c == 2 && ply->v47e == 3) || (ply->v47c == 3 && ply->v47e == 2)) {
        ang[3] += 360.0f;
    }
    if (ply->v0e == 1) {
        if ((ply->v47c == 0 && ply->v47e == 3) || (ply->v47c == 3 && ply->v47e == 0)) {
            ang[3] += 360.0f;
        }
    } else if ((ply->v47c == 2 && ply->v47e == 1) || (ply->v47c == 1 && ply->v47e == 2)) {
        ang[1] += 360.0f;
    }
    t = (f32)ply->v486 / 5.0f;
    ply->v45c = (1.0f - t) * ang[ply->v47c] + t * ang[ply->v47e];
    return ret;
}

void fn_1_62FC(M515_PLY *ply, s16 value)
{
    if (ply->v486 == 5) {
        ply->v47e = value;
        ply->v486 = 0;
    }
}

void fn_1_6318(M515_PLY *ply)
{
    ply->v480 = 0;
}

