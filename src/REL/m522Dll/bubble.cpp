#include "REL/m522Dll.h"

typedef struct {
    u8 pad[0x84];
    u32 value;
} Inner;

typedef struct MGACTOR_s MGACTOR;
typedef struct { f32 x, y, z; } HuVecF;

#define GX_VTXFMT0 0
#define GX_VA_POS 9
#define GX_VA_CLR0 11

extern "C" {
void MgActorColAttrSet(MGACTOR *actorP, u32 mask);
void MgActorPosUpdate(MGACTOR *actorP, HuVecF *pos);
void MgActorPosGet(MGACTOR *actorP, HuVecF *pos);
double fn_1_BABC(void);
void fn_1_5A04(void *arg0, void *arg1);
void fn_1_5CF8(void *arg0);
u8 fn_1_58DC(void *arg0);
void fn_1_5748(void *arg0);
void fn_1_52FC(void *arg0);
void *fn_1_4F88(void *arg0, void *arg1);
void fn_1_5998(void *arg0, void *arg1);
void GXClearVtxDesc(void);
void GXSetVtxDescv(void *list);
void GXSetVtxAttrFmtv(int fmt, void *list);
void GXSetArray(int attr, void *base, int stride);
void GXCallDisplayList(void *list, u32 size);
}

extern u32 lbl_1_rodata_248[];
extern u32 lbl_1_rodata_260[];
extern u32 lbl_1_rodata_2A0[];
extern u32 lbl_1_rodata_2E0[];
extern u32 lbl_1_rodata_300[];

u8 fn_1_58DC(void *arg0)
{
    return (*(u32 *)((u8 *)*(Inner **)((u8 *)arg0 + 0x28) + 0x84) & 0x407F) != 0;
}

void fn_1_5748(void *arg0)
{
    void *base = arg0;
    MgActorColAttrSet(*(MGACTOR **)((u8 *)base + 0x28), 4);
}
// NONMATCHING: stack frame 32 vs 16 bytes, saves r3 to stack

void fn_1_52FC(void *arg0)
{
    u8 *base = (u8 *)arg0;
    MGACTOR *actor = *(MGACTOR **)(base + 0x28);
    HuVecF *pos = (HuVecF *)(base + 0x2C);
    MgActorPosUpdate(actor, pos);
    double val = fn_1_BABC();
    *(s32 *)(base + 0x44) = (s32)(600.0 * val);
}
// NONMATCHING: stack frame 48 vs 32, extra regs saved, const loading differs

void *fn_1_4F88(void *arg0, void *arg1)
{
    MGACTOR *actor = *(MGACTOR **)((u8 *)arg0 + 0x28);
    HuVecF pos;
    MgActorPosGet(actor, &pos);
    *(HuVecF *)arg1 = pos;
    return arg1;
}
// NONMATCHING: extra r3 stack save, reg allocation differs

void fn_1_5998(void *arg0, void *arg1)
{
    f32 val = *(f32 *)((u8 *)arg0 + 0x3C);
    if (val > 0.0f && *(u8 *)((u8 *)arg0 + 0x40)) {
        fn_1_5A04(arg0, arg1);
        fn_1_5CF8(arg0);
    }
}
// NONMATCHING: stack frame 48 vs 32, saves f31, reg allocation differs

void fn_1_5CF8(void *arg0)
{
    (void)arg0;
    GXClearVtxDesc();
    GXSetVtxDescv(lbl_1_rodata_248);
    GXSetVtxAttrFmtv(GX_VTXFMT0, lbl_1_rodata_260);
    GXSetArray(GX_VA_POS, lbl_1_rodata_2A0, 3);
    GXSetArray(GX_VA_CLR0, lbl_1_rodata_2E0, 2);
    GXCallDisplayList(lbl_1_rodata_300, 0x20);
}
// NONMATCHING: extra r3 stack save