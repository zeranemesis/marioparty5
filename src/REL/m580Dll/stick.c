#include "types.h"

typedef s16 HU3DMODELID;
typedef s16 HU3MOTID;

typedef struct M580StickMot_s {
    /* 0x00 */ s16 id;
} M580_STICKMOT;

typedef struct M580StickData_s {
    /* 0x00 */ s32 state;
    /* 0x04 */ s32 timer;
    /* 0x08 */ s16 modelId;
    /* 0x0A */ u8 pad0A[2];
} M580_STICKDATA;

typedef struct M580Stick_s M580_STICK;

struct M580Stick_s {
    /* 0x00 */ u8 pad00[0x14];
    /* 0x14 */ void (*unk14)(M580_STICK *);
    /* 0x18 */ u8 pad18[0x28];
    /* 0x40 */ M580_STICKMOT *unk40;
    /* 0x44 */ u8 pad44[0x18];
    /* 0x5C */ M580_STICKDATA *data;
};

typedef struct M580Hu3DModel_s {
    /* 0x00 */ u8 pad00[0x1C];
    /* 0x1C */ f32 x;
    /* 0x20 */ f32 y;
    /* 0x24 */ f32 z;
    /* 0x28 */ f32 w;
    /* 0x2C */ f32 scale;
} M580_HU3DMODEL;

M580_STICK *lbl_1_bss_238[2];
s32 lbl_1_bss_1B8[32];
s32 lbl_1_bss_138[32];
s16 lbl_1_bss_F8[32];
s16 lbl_1_bss_B8[32];

s32 fn_1_D544(void);
void fn_1_8B98(M580_STICK *stick);
BOOL Hu3DMotionEndCheck(HU3DMODELID modelId);
HU3MOTID Hu3DMotionShiftIDGet(HU3DMODELID modelId);
M580_HU3DMODEL *Hu3DModelObjPtrGet(HU3DMODELID modelId, void *objName);

void fn_1_8B64(M580_STICK *stick)
{
    M580_STICKDATA *data = stick->data;

    data->state = 0;
    data->timer = 0;
    stick->unk14 = fn_1_8B98;
}

void fn_1_8B98(M580_STICK *stick)
{
    M580_STICKDATA *data = stick->data;

    switch (data->state) {
    case 0:
        if (fn_1_D544() != 0) {
            break;
        }
        data->state = data->state + 1;
    case 1:
        data->timer = fn_1_D544();
        break;
    }
}

void fn_1_8C08(void)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        lbl_1_bss_1B8[i] = -1;
        lbl_1_bss_F8[i] = -1;
    }

    for (i = 0; i < 32; i++) {
        lbl_1_bss_138[i] = -1;
        lbl_1_bss_B8[i] = -1;
    }
}

s32 fn_1_943C(M580_STICK *stick)
{
    s16 modelId = stick->unk40->id;

    if (Hu3DMotionEndCheck(modelId) != 0) {
        if (Hu3DMotionShiftIDGet(modelId) < 0) {
            return 1;
        }
    }
    return 0;
}

void fn_1_94A4(s32 arg0, f32 *pos, f32 *scale)
{
    M580_STICK *stick = lbl_1_bss_238[0];
    M580_STICKDATA *data = stick->data;
    M580_HU3DMODEL *model = Hu3DModelObjPtrGet(data->modelId, (void *)arg0);

    if (model != (M580_HU3DMODEL *)0) {
        pos[0] = model->x;
        pos[1] = model->y;
        pos[2] = model->z;
        scale[0] = model->scale;
    }
}
