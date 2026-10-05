#include "types.h"

typedef struct {
    u8 pad_00[0x7A];
    s16 unk_7A;
    u8 pad_7C[0x6C];
} GameEntry;

typedef struct ActorRef ActorRef;

struct ActorRef {
    u8 pad_00[0x3C];
    ActorRef *unk_3C;
};

typedef struct {
    u8 pad_00[0x04];
    s32 unk_04;
    s32 unk_08;
} GameParam;

extern GameEntry lbl_1_bss_3B0[];
extern ActorRef *lbl_1_bss_0[];

void fn_1_3B54(void);
void MgActorKill(ActorRef *ref);

s32 fn_1_762C(s32 idx);
void fn_1_8570(void);
void fn_1_98F0(void);
void fn_1_5CD4(void);
s16 fn_1_A898(void);

s32 fn_1_5788(void) {
    s32 ret = 0;
    ret = fn_1_A898();
    fn_1_762C(0);
    fn_1_762C(1);
    fn_1_8570();
    fn_1_98F0();
    fn_1_5CD4();
    return ret;
}

s32 fn_1_5A00(void) {
    s32 ret = 1;
    fn_1_762C(0);
    fn_1_762C(1);
    fn_1_5CD4();
    return ret;
}

s32 fn_1_5A40(void) {
    s32 ret = 1;
    fn_1_3B54();
    MgActorKill(lbl_1_bss_0[1]->unk_3C);
    MgActorKill(lbl_1_bss_0[2]->unk_3C);
    return ret;
}

s32 fn_1_AD78(void *unused, GameParam *param) {
    s16 tmp = 0;
    /* MWCC roots this 2-case switch at case 6 (cmpwi/beq/bge) where CodeWarrior roots
   it at case 0 (cmpwi/beq/blt); body below is the faithful semantics. */
    switch (param->unk_08) {
    case 0:
        lbl_1_bss_3B0[param->unk_04].unk_7A++;
        break;
    case 6:
        break;
    }
    return 0;
}

void fn_1_ADC8(GameParam *param) {
    s32 val = *(s32 *)((u8 *)param + 0xC);
}