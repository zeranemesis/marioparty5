#include "game/hu3d.h"

typedef struct M551_Mode_s {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 mode;
} M551_Mode;

s32 fn_1_EA8C(void);
void fn_1_9360(void);
extern void fn_1_43CC(void);
extern void MgActorKill(void *arg0);
extern u8 lbl_1_bss_0[];
extern u8 lbl_1_bss_16CB4[];
extern s16 lbl_1_bss_1679C;
extern void fn_1_5FD4(void);
extern void fn_1_A758(s32 arg0);
extern s32 fn_1_F814(void);

s32 fn_1_81F4(void)
{
    s32 ret = 0;
    ret = (s16)fn_1_EA8C();
    fn_1_9360();
    return ret;
}

s32 fn_1_8FF4(void)
{
    s32 ret = 1;
    fn_1_9360();
    return ret;
}

s32 fn_1_822C(void)
{
    s32 ret = 0;

    ((s16 *)lbl_1_bss_16CB4)[0] = 0;
    ((s16 *)lbl_1_bss_16CB4)[1] = 0;
    ((s16 *)lbl_1_bss_16CB4)[2] = 0;
    fn_1_9360();
    return ret;
}

s32 fn_1_8CBC(void)
{
    // NONMATCHING: the original clrlwi's the masked value into r0 and extsh's it,
    // while MWCC routes the 0/1 flag through r3 (3 differing rows, no logic impact).
    s32 ret = 0;

    if (((s16 *)lbl_1_bss_16CB4)[0xc] != 0) {
        fn_1_A758(((s16 *)lbl_1_bss_16CB4)[0xc] & 1);
    }
    ret = (s16)fn_1_F814();
    fn_1_9360();
    return ret;
}

s32 fn_1_9024(void)
{
    s32 ret = 1;

    fn_1_5FD4();
    MgActorKill(*(void **)((u8 *)*(void **)((u8 *)&lbl_1_bss_0 + 0x28) + 0x3c));
    MgActorKill(*(void **)((u8 *)*(void **)((u8 *)&lbl_1_bss_0 + 0x2c) + 0x3c));
    if (lbl_1_bss_1679C != 0) {
        fn_1_43CC();
    }
    return ret;
}

s32 fn_1_117B0(s32 arg0, M551_Mode *param)
{
    if (param->mode >= 6 || param->mode <= 0) {
        return 0;
    }
    return 0;
}