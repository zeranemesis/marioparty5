#include <dolphin.h>

#include "game/pad.h"
#include "humath.h"

typedef struct Mgr511Ptmf {
    s32 type;
    s32 param;
    void (*func)(void);
} Mgr511Ptmf;

extern s32 __ptmf_test(void *ptmf);
extern void __ptmf_scall(void *self, register const Mgr511Ptmf *ptmf);
extern void *__dl__FPv(void *self);
extern void fn_1_CF8(char *cond, char *file, s32 line);
extern void fn_1_172C(void *unk, s32 mode);
extern void fn_1_14C4(void *unk, s32 mode);
extern void fn_1_A3B4(void);
extern void fn_1_AD04(void);

typedef struct Mgr511CamNode {
    u8 unk_00[0x1C];
    HuVecF vec0;
    HuVecF vec1;
    u8 unk_34[0x10];
} Mgr511CamNode;

typedef struct Mgr511Camera {
    u8 unk_000[0x12C];
    void *unk_12C;
    u8 unk_130[0x20];
    s32 unk_150;
    Mgr511Ptmf ptmf;
    u8 unk_160[4];
    s32 unk_164;
} Mgr511Camera;

s32 *lbl_1_bss_A3C;

static char lbl_1_data_384[] = "singleton.h";
static char lbl_1_data_390[] = "ms_pointer != 0";

static Mgr511Ptmf lbl_1_data_3A0 = {0, -1, fn_1_A3B4};
static Mgr511Ptmf lbl_1_data_3B8 = {0, -1, fn_1_AD04};

const f32 lbl_1_rodata_2D4[1] = {0.1f};
const f32 lbl_1_rodata_2D8[1] = {0.05f};

// NONMATCHING: the singleton.h assert block is out-of-line in the original
// (beq into it, then a b to skip); mwcceppc 1.3.2 always branches over it.
Mgr511Camera *fn_1_9A0C(Mgr511Camera *cam, s16 arg1)
{
    if (cam != 0) {
        if (lbl_1_bss_A3C == 0) {
            fn_1_CF8(lbl_1_data_390, lbl_1_data_384, 0x3D);
        }
        lbl_1_bss_A3C = 0;
        if (arg1 > 0) {
            __dl__FPv(cam);
        }
    }
    return cam;
}

void fn_1_9F98(void *arg0, s32 arg1, HuVecF *arg2, HuVecF *arg3)
{
    HuVecF *base = &((Mgr511CamNode *)arg0)[arg1].vec0;

    PSVECSubtract(arg2, (HuVecF *)((u8 *)base + 0x18), arg2);
    PSVECScale(arg2, arg2, lbl_1_rodata_2D4[0]);
    PSVECAdd(arg2, (HuVecF *)((u8 *)base + 0x18), arg2);
    PSVECSubtract(arg3, base, arg3);
    PSVECScale(arg3, arg3, lbl_1_rodata_2D8[0]);
    PSVECAdd(arg3, base, arg3);
}

void fn_1_A4F0(Mgr511Camera *cam)
{
    Mgr511Ptmf tmp;

    fn_1_172C(&cam->unk_12C, 0);
    if (HuPadBtnDown[0] & 0x800) {
        tmp = lbl_1_data_3A0;
        cam->ptmf = tmp;
    }
    fn_1_14C4(&cam->unk_12C, 1);
}

void fn_1_B2D4(Mgr511Camera *cam, s32 arg1)
{
    Mgr511Ptmf tmp;

    cam->unk_164 = arg1;
    tmp = lbl_1_data_3B8;
    cam->ptmf = tmp;
    cam->unk_150 = 0;
}

// NONMATCHING: the original passes the PTMF in r12 (CodeWarrior builtin
// __ptmf_scall ABI) and pads a nop after the call; mwcceppc 1.3.2 uses r4.
void fn_1_B4C0(Mgr511Camera *cam)
{
    if (__ptmf_test(&cam->ptmf)) {
        __ptmf_scall(cam, &cam->ptmf);
    }
}