#include "types.h"

typedef struct {
    s32 unk00;
    u8 unk04[0x38];
    void *unk3C;
} MGACTOROWNER;

typedef struct {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
} MGNATIVE;

extern MGACTOROWNER *lbl_1_bss_0[3];

void MgActorKill(void *actor);
void fn_1_3AC4(void);
s32 fn_1_984C(void);
s32 fn_1_9A70(void);
void fn_1_B668(void);
void fn_1_6F30(void);

s16 fn_1_6170(void)
{
    s16 ret = 0;

    ret = fn_1_984C();
    fn_1_6F30();
    return ret;
}

s16 fn_1_6928(void)
{
    s16 ret = 0;

    ret = fn_1_9A70();
    fn_1_B668();
    fn_1_6F30();
    return ret;
}

s32 fn_1_6BDC(void)
{
    s32 ret = 1;

    fn_1_6F30();
    return ret;
}

s32 fn_1_6C0C(void)
{
    s32 ret = 1;

    fn_1_3AC4();
    MgActorKill(lbl_1_bss_0[1]->unk3C);
    MgActorKill(lbl_1_bss_0[2]->unk3C);
    return ret;
}

MGNATIVE *fn_1_B654(MGNATIVE *p)
{
    s32 tmp;

    tmp = p->unk0C;
    return p;
}
