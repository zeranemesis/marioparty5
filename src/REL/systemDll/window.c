#include "types.h"

extern u8 sysWork[];
extern void HuWinAllKill(void);

void MdMessKill(void)
{
    HuWinAllKill();
}

void *fn_1_28F74(s32 arg0)
{
    // NONMATCHING: the original keeps both the table base and the computed slot in
    // saved registers (r29/r30) and needs a -0x20 frame; MWCC uses one register.
    u8 *p = *(u8 **)(sysWork + 0x194);

    return (u8 *)p + arg0 * 0x10 + 0x78;
}

void MdMessDlgInsertMesReset(void)
{
    // NONMATCHING: register allocation - the original keeps &sysWork in r31 and the
    // table pointer in r29, needing three saved registers (-0x20 frame).
    u8 *mess = *(u8 **)(sysWork + 0x194);
    u8 *field = mess + 0x14;

    *(s32 *)field = 0;
}
