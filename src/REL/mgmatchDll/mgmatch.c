#include "game/gamework.h"
#include "game/flag.h"
#include "game/process.h"
#include "game/hu3d.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

void ObjectSetup(void);
void fn_1_BC0(void);

/* Main minigame work pointer (lbl_1_bss_0) */
typedef struct MgMatchWork_s {
    u8 pad0[0x6];
    s16 unk6;
    u8 unk8;
    u8 pad9[0xDF7];
    s32 unkE00;
} MGMATCHWORK;

/* Common/minigame-manager work pointer (lbl_1_bss_4) */
typedef struct MgMatchCom_s {
    u8 pad0[0x1B];
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
} MGMATCHCOM;

MGMATCHWORK *lbl_1_bss_0;
MGMATCHCOM *lbl_1_bss_4;
u8 lbl_1_bss_8[0xE30];

int _prolog(void)
{
    const VoidFunc *ctor = _ctors;

    while (*ctor != NULL) {
        (*ctor)();
        ctor++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtor = _dtors;

    while (*dtor != NULL) {
        (*dtor)();
        dtor++;
    }
}

void ObjectSetup(void)
{
    GwSystem.bankCoin = 0;
    _SetFlag(0xb);
    HuPrcChildCreate(fn_1_BC0, 0xc8, 0x3000, 0, HuPrcCurrentGet());
}

s16 fn_1_458(void)
{
    return lbl_1_bss_0->unk6;
}

u8 fn_1_588(void)
{
    return lbl_1_bss_0->unk8;
}

u8 fn_1_59C(void)
{
    return lbl_1_bss_4->unk1B;
}
