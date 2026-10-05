#include "dolphin.h"

extern "C" {

typedef struct {
    u8 pad_00[0xC];
    u32 unk_0C;
    u8 pad_10[0x120 - 0x10];
    void *unk_120;
} WaterObj;

void fn_1_1A8C0(void *obj, s32 arg);
void fn_1_1BC84(void *obj, s32 arg);

void fn_1_19804(WaterObj *p, s32 arg)
{
    void *o = p->unk_120;

    fn_1_1A8C0(o, arg);
}

void fn_1_1A518(WaterObj *p, s32 bit)
{
    p->unk_0C &= ~((u32)1 << bit);
}

void fn_1_1B534(WaterObj *p, s32 arg)
{
    void *o = p->unk_120;

    fn_1_1BC84(o, arg);
}

void fn_1_1BB68(WaterObj *p, s32 bit)
{
    p->unk_0C &= ~((u32)1 << bit);
}

void fn_1_1C640(WaterObj *p, s32 bit)
{
    p->unk_0C &= ~((u32)1 << bit);
}

}
