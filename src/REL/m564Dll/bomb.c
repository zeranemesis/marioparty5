#include "types.h"

typedef struct M564Vec_s {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
} M564_VEC;

typedef struct M564Bomb_s {
    /* 0x00 */ void *unk00;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ u8 pad08[4];
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ u8 pad10[2];
    /* 0x12 */ s16 unk12;
    /* 0x14 */ u8 pad14[6];
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ u8 pad1C[4];
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ u8 pad24[2];
    /* 0x26 */ s16 unk26;
    /* 0x28 */ u8 pad28[0xC];
    /* 0x34 */ M564_VEC unk34;
    /* 0x40 */ M564_VEC unk40;
} M564_BOMB;

M564_BOMB *lbl_1_bss_1E8[15];
M564_VEC lbl_1_data_22C = {0.0f, -10.0f, 0.0f};

extern void MgActorPosGet(void *actor, M564_VEC *pos);
extern void Hu3DModelPosSet(s16 modelId, f32 x, f32 y, f32 z);
extern void PSVECAdd(M564_VEC *dst, M564_VEC *a, M564_VEC *b);
extern f32 fn_1_13A4(M564_VEC *pos, void *obj);
extern void fn_1_5E5C(s16 idx);
extern void fn_1_5EA8(s16 idx, void *pos);
extern void fn_1_6A14(M564_BOMB *bomb);
extern void fn_1_7210(M564_BOMB *bomb, s32 flag);

void fn_1_64B8(void) {
    M564_BOMB *bomb;
    s32 i;

    for (i = 0; i < 0xF; i++) {
        bomb = lbl_1_bss_1E8[i];
        if (bomb->unk20 != 0) {
            fn_1_6A14(bomb);
            Hu3DModelPosSet(bomb->unk06, bomb->unk40.x, 0.1f, bomb->unk40.z);
        }
    }
}

void fn_1_6A14(M564_BOMB *bomb) {
    if (bomb->unk20 == 5) {
        fn_1_5EA8(bomb->unk1A, &bomb->unk40);
        PSVECAdd(&bomb->unk40, &lbl_1_data_22C, &bomb->unk40);
    } else {
        MgActorPosGet(bomb->unk00, &bomb->unk40);
    }
}

void fn_1_7404(M564_BOMB *bomb) {
    fn_1_7210(bomb, 1);
}

/* s32 return type with no return statement: reserves r3 so the index
   temporaries start at r4, which is what the original does */
s32 fn_1_7674(M564_BOMB *bomb) {
    M564_BOMB *other = lbl_1_bss_1E8[bomb->unk0C - 4];

    if (other->unk20 == 6) {
        fn_1_7210(other, 1);
    }
}

s32 fn_1_84E8(void *actor, M564_VEC *pos, f32 dist) {
    f32 d = fn_1_13A4(pos, actor);

    if (d < 70.0f + dist) {
        return 1;
    }
    return 0;
}
