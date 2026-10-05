#include "types.h"

typedef struct m530effectcell_s {
    u8 pad00[2];
    s16 f_02; //0x02
    u8 pad04[0x24];
    s32 f_28; //0x28
    u8 pad2C[8];
    void *f_34; //0x34
    s32 f_38; //0x38
    s32 f_3C; //0x3C
    s32 f_40; //0x40
    s32 f_44; //0x44
    u8 pad48[8];
} M530_EFFECT_CELL;

typedef struct m530effectmodel_s {
    s16 f_00; //0x00
} M530_EFFECT_MODEL;

struct m530effect_s;

typedef void (*M530_EFFECT_FN)(struct m530effect_s *self, M530_EFFECT_CELL *cell);

typedef struct m530effect_s {
    u8 pad00[0x14];
    M530_EFFECT_FN f_14; //0x14
    u8 pad18[0x28];
    M530_EFFECT_MODEL *f_40; //0x40
    u8 pad44[0x18];
    M530_EFFECT_CELL *f_5C; //0x5C
} M530_EFFECT;

typedef struct m530effectvec_s {
    f32 x;
    f32 y;
    f32 z;
} M530_EFFECT_VEC;

extern M530_EFFECT_CELL lbl_1_bss_DC[];
extern void *lbl_1_bss_B8;

s32 lbl_1_data_390[3] = {3, 4, 5};

s32 Hu3DMotionEndCheck(s16 motion);
void Hu3DModelKill(s16 motion);
void omDelObjEx(void *obj, void *actor);
void CharModelHookDustCreate(s16 id, void *cell);
void fn_1_10E8(M530_EFFECT *self, s32 arg1);
void fn_1_91F8(M530_EFFECT *self, M530_EFFECT_CELL *cell);
f32 PSVECMag(M530_EFFECT_VEC *v);
void PSVECNormalize(M530_EFFECT_VEC *a, M530_EFFECT_VEC *b);
void PSVECScale(M530_EFFECT_VEC *a, M530_EFFECT_VEC *b, f32 scale);

void fn_1_88FC(M530_EFFECT *self) {
    if (Hu3DMotionEndCheck(self->f_40->f_00) != 0) {
        Hu3DModelKill(self->f_40->f_00);
        omDelObjEx(lbl_1_bss_B8, self);
    }
}

s32 fn_1_8ED8(M530_EFFECT_VEC *v) {
    f32 mag = PSVECMag(v);
    if (mag <= 550.0f) {
        return 0;
    }
    PSVECNormalize(v, v);
    PSVECScale(v, v, 550.0f);
    return 1;
}

void fn_1_916C(M530_EFFECT *self) {
    M530_EFFECT_CELL *cell = self->f_5C;
    if (cell->f_38 == 0) {
        CharModelHookDustCreate(cell->f_02, cell->f_34);
        fn_1_10E8(self, 0);
    }
    cell->f_38 += 1;
    if (cell->f_38 == 0x3C) {
        cell->f_38 = 0;
        cell->f_28 = 4;
        self->f_14 = fn_1_91F8;
    }
}

void fn_1_9384(void) {
}

void fn_1_96B0(M530_EFFECT *self) {
    M530_EFFECT_CELL *cell = self->f_5C;
    if (lbl_1_bss_DC[cell->f_44].f_3C == 0) {
        cell->f_44 = -1;
        cell->f_3C = 0;
    }
}
