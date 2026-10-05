#include "dolphin/types.h"

#pragma inline_depth(0)

extern "C" {
void fn_1_73A4(void *self, u8 param);
void fn_1_AEAC(void *self, u32 param);
void GXLoadPosMtxImm(u32 mtxIdx, u32 param);
void fn_1_A100(void *self);
void Hu3DModelObjDrawInit(void);
void fn_1_785C(void *unused, void *param);
void fn_1_AEF8(void *self, u32 param);
void fn_1_78AC(void *unused, void *param);
extern void *lbl_1_bss_34;
}

void fn_1_73A4(void *self, u8 param)
{
    if (*(u8 *)((u32)self + 0x20) == 0) {
        *(u8 *)((u32)self + 0x20) = 1;
        *(u32 *)((u32)self + 0x24) = 0;
        *(u8 *)((u32)self + 0x28) = param;
    }
}

void fn_1_AEAC(void *self, u32 param)
{
    GXLoadPosMtxImm(param, 0);
    if (*(u8 *)((u32)self + 0x14) == 0) {
        fn_1_A100(self);
    }
}

void fn_1_785C(void *unused, void *param)
{
    Hu3DModelObjDrawInit();
    u32 p = (u32)param;
    void *self = lbl_1_bss_34;
    fn_1_AEAC(self, p);
}
// NONMATCHING: register allocation differs (compiler uses r31 for param, original uses r30->r31 copy)

void fn_1_78AC(void *unused, void *param)
{
    Hu3DModelObjDrawInit();
    u32 p = (u32)param;
    void *self = lbl_1_bss_34;
    fn_1_AEF8(self, p);
}
// NONMATCHING: register allocation differs (compiler uses r31 for param, original uses r30->r31 copy)