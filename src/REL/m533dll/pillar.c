#include "game/hu3d.h"
#include "game/memory.h"

void fn_1_7390(void);
void fn_1_74E0(s32 arg0, s32 arg1);
void fn_1_80BC(s16 arg0, s32 arg1, Vec *arg2);
s32 fn_1_8114(s32 arg0, s32 arg1);
u32 fn_1_8150(Vec *arg0, Vec *arg1, float arg2, float arg3);

typedef struct M533ListNode_s {
    u16 unk0;
    s16 unk2;
    struct M533ListNode_s *prev;
    struct M533ListNode_s *next;
} M533LISTNODE;

typedef struct M533ListHead_s {
    s32 unk0;
    struct M533ListNode_s *unk4;
    struct M533ListNode_s *unk8;
} M533LISTHEAD;

typedef struct M533CacheEntry_s {
    s32 unk0;
    s16 unk4;
    s16 unk6;
} M533CACHEENTRY;

typedef struct M533Cache_s {
    M533CACHEENTRY entry[32];
    s32 unk100;
} M533CACHE;

M533LISTHEAD lbl_1_bss_4E0;

M533CACHE lbl_1_bss_2D8;

void fn_1_7390(void)
{
    lbl_1_bss_4E0.unk4 = 0;
    lbl_1_bss_4E0.unk8 = 0;
}

void fn_1_74E0(s32 arg0, s32 arg1)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        lbl_1_bss_2D8.entry[i].unk0 = 0;
        lbl_1_bss_2D8.entry[i].unk4 = -1;
    }
}

void fn_1_80BC(s16 arg0, s32 arg1, Vec *arg2)
{
    Mtx sp8;

    Hu3DModelObjMtxGet(arg0, (char *)arg1, sp8);
    arg2->x = sp8[0][3];
    arg2->y = sp8[1][3];
    arg2->z = sp8[2][3];
}

s32 fn_1_8114(s32 arg0, s32 arg1)
{
    return arg0 + frandmod(arg1 - arg0);
}

u32 fn_1_8150(Vec *arg0, Vec *arg1, float arg2, float arg3)
{
    if (((arg1->x - arg0->x) * (arg1->x - arg0->x)) + ((arg1->z - arg0->z) * (arg1->z - arg0->z))
        < (arg2 + arg3) * (arg2 + arg3)) {
        return 1;
    } else {
        return 0;
    }
}
