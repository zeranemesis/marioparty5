#include "dolphin/types.h"

extern "C" {
typedef struct GameMapNode {
    void **vtable;  // 0x00
    s8 unk04[0x2C];
} GameMapNode;  // size 0x30

typedef struct GameBoard {
    void *owner;  // 0x00
    s8 unk04[0x384];
} GameBoard;  // size 0x388

typedef struct GameMaster {
    void **vtable;        // 0x00 base class vtable
    void **vtable2;       // 0x04 own vtable
    s8 unk08[0x18];
    GameMapNode mapNode;  // 0x20
    s8 unk50[0x308];
    GameMapNode mapList;  // 0x358
    GameBoard board;      // 0x388
} GameMaster;

// Node of the intrusive list fn_1_134A8 walks: the container doubles as the
// sentinel its inner scan stops on, and m_last is the outer scan's end.
typedef struct GameNode {
    struct GameNode *next;  // 0x00
    struct GameNode *last;  // 0x04
} GameNode;

typedef void (*GameNodeCb)(GameNode *node1, GameNode *node2);

void __dl__FPv(void *ptr);

void fn_1_117D8(void *obj, s16 flag);
void fn_1_5CD8(void *obj, s16 flag);
void fn_1_6B34(void *obj, s16 flag);
void fn_1_6848(void);
void fn_1_924C(void *obj);
void fn_1_9D4C(GameMapNode *node, s16 flag);
void fn_1_A5D0(GameMapNode *node, s16 flag);

GameBoard *fn_1_11780(GameBoard *self, s16 flag);
GameBoard *fn_1_11AB0(GameBoard *self, s16 flag);
GameMaster *fn_1_11B08(GameMaster *self, s16 flag);
void fn_1_11BA0(GameMaster *self, s16 flag);
void fn_1_11E70(void *self);
void fn_1_13488(void);
void fn_1_134A8(GameNode *self, GameNodeCb cb);
void fn_1_140C8(void *self);
void fn_1_140D0(void *self);
void fn_1_12CA8(void *self);

// Virtual tables of GameMaster: the first is stored at offset 4, the second
// (base at offset 0x20) lives right behind it.
void *lbl_1_data_538[12] = {
    0, 0, (void *)fn_1_11B08, (void *)fn_1_11BA0, (void *)fn_1_11E70, (void *)fn_1_13488,
    0, 0, (void *)fn_1_140D0, (void *)fn_1_140C8, (void *)fn_1_12CA8, 0
};

/* The original keeps this destructor out of line at its one call site
 * (fn_1_11B08); MWCC would otherwise inline the whole body there. */
#pragma dont_inline on
GameBoard *fn_1_11780(GameBoard *self, s16 flag)
{
    if (self != 0) {
        fn_1_117D8(self->owner, 1);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}
#pragma dont_inline reset

GameBoard *fn_1_11AB0(GameBoard *self, s16 flag)
{
    if (self != 0) {
        fn_1_5CD8(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

GameMaster *fn_1_11B08(GameMaster *self, s16 flag)
{
    if (self != 0) {
        self->vtable2 = lbl_1_data_538;
        self->mapNode.vtable = &lbl_1_data_538[6];
        fn_1_11780(&self->board, -1);
        fn_1_A5D0(&self->mapList, -1);
        fn_1_9D4C(&self->mapNode, 0);
        fn_1_6B34(self, 0);
        if (flag > 0) {
            fn_1_924C(self);
        }
    }
    return self;
}

void fn_1_13488(void)
{
    fn_1_6848();
}

void fn_1_134A8(GameNode *self, GameNodeCb cb)
{
    GameNode *node1;
    GameNode *node2;
    GameNode *endNode;
    GameNode *lastNode;
    GameNode *tmp0;
    GameNode *tmp1;
    GameNode *tmp2;
    GameNode *last;
    GameNode *tmp3;
    GameNode *tmp4;
    GameNode *tmp5;

    tmp0 = self->next;
    tmp1 = tmp0;
    node1 = tmp1;
    tmp2 = self;
    endNode = tmp2;
    last = endNode->last;
    lastNode = last;
    while (node1 != lastNode) {
        tmp3 = node1->next;
        node2 = tmp3;
        while (node2 != endNode) {
            cb(node1, node2);
            tmp4 = node2->next;
            node2 = tmp4;
        }
        tmp5 = node1->next;
        node1 = tmp5;
    }
}
}
