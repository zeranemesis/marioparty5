#include "game/hu3d.h"

typedef struct M551_Mode_s {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 mode;
} M551_Mode;

s32 fn_1_EA8C(void);
void fn_1_9360(void);

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

s32 fn_1_117B0(s32 arg0, M551_Mode *param)
{
    if (param->mode >= 6 || param->mode <= 0) {
        return 0;
    }
    return 0;
}