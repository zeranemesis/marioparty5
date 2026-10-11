// m503Dll/stage - stage setup and per-frame drive for the mini-game.
#include "types.h"
#include "game/object.h"

typedef s16 HU3DMODELID;
typedef s16 HU3DMOTID;

void fn_1_A790(OMOBJ *obj);
extern void fn_1_AF20(s8 arg0, s32 arg1);
extern s8 lbl_1_data_62C[];

extern OMOBJ *lbl_1_bss_B8;

// NONMATCHING: the byte read from the table keeps its raw u8 in the original and// is passed to fn_1_AF20 without an extsb; MWCC sign-extends because the prototype// takes s8. Two branch displacements also differ.
// Walks the 16-entry spawn table: every entry drives its target, and the last
// one is flagged as the active row so the caller knows where the sweep stopped.
void fn_1_AE8C(OMOBJ *obj)
{
    s32 i;

    if (*(u32 *)((u8 *)obj + 0x4C) < 2) {
        for (i = 0; i < 16; i++) {
            fn_1_AF20(lbl_1_data_62C[i & 15], 0);
        }
    }
    fn_1_AF20(lbl_1_data_62C[*(s32 *)((u8 *)obj + 0x4C) & 15], 1);
}
void fn_1_A78C(void)
{
}

// NONMATCHING: the heap id 0x1000 arrives as lis r5 rather than li r5, i.e. the
// original materialises it from something other than an inline literal. Everything// else matches.
// Spawns the stage driver object, allocates its 0xB4-byte work area and clears it.
void fn_1_A6FC(OMOBJMAN *arg0)
{
    OMOBJ *obj;
    void *work;

    obj = omAddObjEx(arg0, 0x14, 0xB, 0, -1, fn_1_A790);
    lbl_1_bss_B8 = obj;
    work = HuMemDirectMallocNum(0, 0xB4, 0x1000);
    *(s32 *)((u8 *)obj + 0x5C) = (s32)work;
    memset(work, 0, 0xB4);
    *(s32 *)((u8 *)obj + 0x4C) = 0;
}