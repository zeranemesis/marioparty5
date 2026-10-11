#include "dolphin.h"

extern "C" {

/* Splash controller: modelId at 0x0, child work at 0x1C, second child work at 0x120. */
typedef struct {
    s16 modelId;
    u8 pad_02[0x1C - 0x2];
    void *unk_1C;
    u8 pad_20[0x120 - 0x20];
    void *unk_120;
} BoatSplash;

/* Splash drop: position at 0x8, velocity at 0x14, scalar at 0x30. */
typedef struct {
    u8 pad_00[0x8];
    Vec unk_08;
    Vec unk_14;
    u8 pad_20[0x30 - 0x20];
    f32 unk_30;
} BoatSplashDrop;

void fn_1_1757C(void *arg0, s32 arg1);
void fn_1_1886C(void *arg0, s32 arg1);
void fn_1_17EC8(void *arg0, s32 arg1);
void Hu3DModelKill(s16 modelId);
void *__dl__FPv(void *self);

void fn_1_16E40(BoatSplash *arg0, BoatSplash *arg1, s32 arg2)
{
    void *o = arg1->unk_1C;

    fn_1_1757C(o, arg2);
}

BoatSplash *fn_1_170C8(BoatSplash *arg0, s16 arg1)
{
    if (arg0) {
        Hu3DModelKill(arg0->modelId);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_17544(BoatSplashDrop *arg0, Vec *arg1, Vec *arg2, f32 arg3)
{
    arg0->unk_08.x = arg1->x;
    arg0->unk_08.y = arg1->y;
    arg0->unk_08.z = arg1->z;
    arg0->unk_14.x = arg2->x;
    arg0->unk_14.y = arg2->y;
    arg0->unk_14.z = arg2->z;
    arg0->unk_30 = arg3;
}

void fn_1_17B94(BoatSplash *arg0, s32 arg2)
{
    void *o = arg0->unk_120;

    fn_1_17EC8(o, arg2);
}

void fn_1_183C0(BoatSplash *arg0, BoatSplash *arg1, s32 arg2)
{
    void *o = arg1->unk_1C;

    fn_1_1886C(o, arg2);
}

}
