#include "types.h"
#include "game/process.h"

typedef struct M541DllBss2734_s {
    s16 x;
    s16 y;
    s16 z;
    s16 w;
} M541DLLBSS2734;

extern void fn_1_DDCC(void);
extern void fn_1_7850(s32);
extern s32 fn_1_E0A4(void);
extern void fn_1_2AB4(void);
extern void MgActorKill(void *);
extern void fn_1_758C(void);

extern M541DLLBSS2734 lbl_1_bss_2734;
extern void *lbl_1_bss_0[8];

s32 fn_1_5DA4(void)
{
    s32 ret = 0;
    fn_1_DDCC();
    fn_1_758C();
    return ret;
}

s32 fn_1_5DD8(void)
{
    s32 ret = 0;
    lbl_1_bss_2734.x = 0;
    lbl_1_bss_2734.y = 0;
    lbl_1_bss_2734.z = 0;
    lbl_1_bss_2734.w = 0;
    fn_1_758C();
    return ret;
}

s16 fn_1_6BF0(void)
{
    s16 ret = 0;
    fn_1_7850(0);
    fn_1_7850(1);
    ret = (s16)fn_1_E0A4();
    fn_1_758C();
    return ret;
}

s32 fn_1_6E20(void)
{
    s32 ret = 1;
    fn_1_758C();
    return ret;
}

s32 fn_1_6E50(void)
{
    s32 ret = 1;
    fn_1_2AB4();
    MgActorKill(*(void **)((u8 *)lbl_1_bss_0[1] + 0x3c));
    MgActorKill(*(void **)((u8 *)lbl_1_bss_0[2] + 0x3c));
    return ret;
}

typedef struct {
    u8 pad[8];
    s32 field8;
} Struct_E900;

s32 fn_1_E900(s32 ret_val, Struct_E900 *arg)
{
    if (arg->field8 >= 6) goto ret;
    if (arg->field8 == 0) ret_val = 0;
    goto ret;
ret:
    return ret_val;
}

typedef struct {
    u8 pad[12];
    u32 field12;
} Struct_E91C;

void *fn_1_E91C(Struct_E91C *arg)
{
    u32 temp = arg->field12;
    return arg;
}
