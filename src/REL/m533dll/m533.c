#include "game/process.h"
#include "game/object.h"
#include "game/memory.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/audio.h"
#include "game/mg/seqman.h"

extern void (*_ctors[])(void);
extern void (*_dtors[])(void);

extern s32 lbl_1_bss_0;
extern s32 lbl_1_bss_4;
extern s32 lbl_1_bss_8;
extern s32 lbl_1_bss_C;
extern s16 lbl_1_bss_10[10];
extern s32 lbl_1_bss_2B8;
extern s32 lbl_1_bss_2BC[3];
extern s32 lbl_1_bss_42;
extern s32 lbl_1_bss_98;
extern s32 lbl_1_bss_A0;
extern s32 lbl_1_bss_A4;
extern s32 lbl_1_bss_A8;
extern s32 lbl_1_bss_AC;
extern s32 lbl_1_bss_B0;
extern s32 lbl_1_bss_B4[0x40];
extern s32 lbl_1_bss_C;
extern s32 lbl_1_bss_9C;

extern void fn_1_6A8(void);
extern void fn_1_165C(void);
extern void fn_1_1F98(void);
extern void fn_1_2D4C(void);
extern void fn_1_3424(void);
extern void fn_1_3890(void);
extern void fn_1_3950(void);
extern void fn_1_3D50(void);
extern void fn_1_4080(void);
extern void fn_1_4A44(void);
extern void fn_1_4F84(void);
extern void fn_1_6528(void);
extern void fn_1_6590(void);
extern void ObjectSetup(void);

static s32 lbl_1_bss_0_val;
static s32 lbl_1_bss_4_val;
static s32 lbl_1_bss_8_val;
static s32 lbl_1_bss_C_val;
static s16 lbl_1_bss_10_val[10];
static s32 lbl_1_bss_2B8_val;
static s32 lbl_1_bss_2BC_val[3];
static s32 lbl_1_bss_42_val;
static s32 lbl_1_bss_98_val;
static s32 lbl_1_bss_A0_val;
static s32 lbl_1_bss_A4_val;
static s32 lbl_1_bss_A8_val;
static s32 lbl_1_bss_AC_val;
static s32 lbl_1_bss_B0_val;
static s32 lbl_1_bss_B4_val[0x40];
static s32 lbl_1_bss_9C_val;

static void fn_1_5580(void)
{
    lbl_1_bss_2BC_val[1] = 0;
    lbl_1_bss_2BC_val[2] = 0;
}

static void fn_1_5544(void)
{
    Hu3DModelKill(lbl_1_bss_10_val[6]);
    Hu3DModelKill(lbl_1_bss_10_val[7]);
}

static s32 fn_1_6304(s32 arg0, s32 arg1)
{
    return arg0 + frandmod(arg1 - arg0);
}

static void fn_1_56D0(void)
{
    s32 i;
    for (i = 0; i < 0x20; i++) {
        lbl_1_bss_B4_val[i * 2 + 0] = 0;
        lbl_1_bss_B4_val[i * 2 + 1] = -1;
    }
}

s32 _prolog(void)
{
    void (**ctor)(void) = (void (**)(void)) &_ctors;

    while (*ctor != NULL) {
        (*ctor)();
        ctor++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    void (**dtor)(void) = (void (**)(void)) &_dtors;

    while (*dtor != NULL) {
        (*dtor)();
        dtor++;
    }
}