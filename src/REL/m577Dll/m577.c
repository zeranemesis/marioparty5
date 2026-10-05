#include "dolphin/types.h"

typedef struct M577Mgr_s {
    u8 pad00[0x5C];
    s32 unk5C; /* 0x5C */
} M577Mgr;

typedef struct M577List_s {
    struct M577List_s *prev; /* 0x00 */
    struct M577List_s *next; /* 0x04 */
    u8 pad08[4];             /* 0x08 */
    s32 argC;                /* 0x0C */
    s32 arg10;               /* 0x10 */
} M577List;

typedef struct M577Path_s {
    f32 f00; /* 0x00 */
    f32 f04; /* 0x04 */
    f32 f08; /* 0x08 */
    f32 f0C; /* 0x0C */
    f32 f10; /* 0x10 */
    f32 f14; /* 0x14 */
    u8 pad18[0x1C];
    s16 v34; /* 0x34 */
} M577Path;

typedef struct M577Pos_s {
    u8 pad00[0x30];
    f32 prevY; /* 0x30 */
    f32 y;     /* 0x34 */
} M577Pos;

static M577Mgr *lbl_1_bss_C;

void fn_1_B26C(s32 arg0);

void fn_1_1790(void)
{
    s32 v;

    v = lbl_1_bss_C->unk5C;
    fn_1_B26C(v);
}

void fn_1_1CF4(u8 *arg0)
{
    s32 i;

    for (i = 0; i < 0x19; i++) {
        *(s16 *)(arg0 + i * 2 + 0x1B68) = -1;
    }
    for (i = 0; i < 4; i++) {
        *(s16 *)(arg0 + i * 2 + 0x1B9A) = -1;
    }
}

void fn_1_2364(M577Path *arg0, s16 arg1)
{
    arg0->f00 = 0.0f;
    arg0->f04 = 0.0f;
    arg0->f08 = 0.0f;
    arg0->f0C = 0.0f;
    arg0->f10 = 0.0f;
    arg0->f14 = 3000.0f;
    arg0->v34 = arg1;
}

M577List *fn_1_26C0(M577List *arg0, s32 arg1, s32 arg2)
{
    M577List *node;

    node = arg0->next;
    if (node != 0) {
        arg0->next = node->next;
        node->argC = arg1;
        node->arg10 = arg2;
        node->next = arg0->prev;
        if (node->next != 0) {
            node->next->prev = node;
        }
        arg0->prev = node;
    }
    return node;
}

void fn_1_72A8(M577Pos *arg0)
{
    s32 v;

    v = lbl_1_bss_C->unk5C;
    arg0->prevY = arg0->y;
}
