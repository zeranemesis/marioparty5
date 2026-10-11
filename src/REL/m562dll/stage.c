#include "types.h"

/* library */
extern void Hu3DModelKill(s16 modelId);
extern void Hu3DMotionKill(s16 motId);
extern s16 Hu3DModelLink(s16 linkMdlId);
extern void Hu3DModelRotSet(s16 modelId, f32 rotX, f32 rotY, f32 rotZ);
extern void Hu3DModelCameraSet(s16 modelId, u16 cameraBit);
extern void Hu3DModelLayerSet(s16 modelId, s16 layerNo);
extern void Hu3DModelAttrReset(s16 modelId, u32 attr);
extern void Hu3DModelAttrSet(s16 modelId, u32 attr);
extern void Hu3DModelShadowMapSet(s16 modelId);
extern void Hu3DModelPosSet(s16 modelId, f32 posX, f32 posY, f32 posZ);
extern void *memset(void *dst, s32 val, u32 len);

void fn_1_62A8(u8 *arg0);

/* three contiguous floats: HuVecF is Vec and therefore 8-byte aligned, which
   would inflate every struct that carries one of these */
typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
} Lbl1VecF;

/* this unit's .rodata, in binary order (module-relative offsets) */
static const f32 lbl_1_rodata_190 = 0.0f;
static const f64 lbl_1_rodata_1C8 = 4503601774854144.0;
static const f32 lbl_1_rodata_1D0 = -500.0f;
static const f32 lbl_1_rodata_1D4 = 250.0f;
static const f32 lbl_1_rodata_1D8 = -375.0f;

void fn_1_5BE4(u8 *arg0) {
    u8 *slot;
    s32 i;

    for (i = 0; i < 0x14; i++) {
        slot = (u8 *) ((u32) arg0 + i * 0x48 + 8);
        fn_1_62A8(slot);
    }
    Hu3DMotionKill(*(s16 *) ((u32) arg0 + 0x5c2));
    Hu3DModelKill(*(s16 *) ((u32) arg0 + 0x5c0));
    Hu3DMotionKill(*(s16 *) ((u32) arg0 + 0x5b8));
    Hu3DModelKill(*(s16 *) ((u32) arg0 + 0x5b6));
    Hu3DModelKill(*(s16 *) ((u32) arg0 + 0x5b4));
    Hu3DMotionKill(*(s16 *) ((u32) arg0 + 0x5b2));
    Hu3DModelKill(*(s16 *) ((u32) arg0 + 0x5b0));
    Hu3DModelKill(*(s16 *) ((u32) arg0 + 0x5ae));
    Hu3DModelKill(*(s16 *) ((u32) arg0 + 0x5ac));
    Hu3DModelKill(*(s16 *) ((u32) arg0 + 0x5aa));
    Hu3DModelKill(*(s16 *) ((u32) arg0 + 0x5a8));
    memset(arg0, 0, 0x5dc);
}

/* the stage's camera table slot: a word whose low half is the camera mask */
typedef struct {
    /* 0x00 */ u8 unk0[4];
    /* 0x04 */ u32 cameraBit;
} Lbl1CamSlot;

void fn_1_60F8(u8 *arg0, s16 arg1, s16 arg2, u8 *arg3) {
    Lbl1VecF pos;
    s16 modelId;
    u8 *sp2C = arg3;
    u8 *sp28 = *(u8 **) sp2C;
    Lbl1CamSlot *sp24 = (Lbl1CamSlot *) ((u32) sp28 + 0x1bc);

    memset(arg0, 0, 0x48);
    *(void **) ((u32) arg0 + 0) = (void *) ((u32) arg3);
    *(s16 *) ((u32) arg0 + 0xc) = arg1;
    *(s16 *) ((u32) arg0 + 0xe) = arg2;
    modelId = Hu3DModelLink(*(s16 *) ((u32) arg3 + 0x5a8));
    *(s16 *) ((u32) arg0 + 8) = modelId;
    Hu3DModelRotSet(modelId, lbl_1_rodata_190, lbl_1_rodata_190, lbl_1_rodata_190);
    Hu3DModelCameraSet(modelId, (u16) sp24->cameraBit);
    Hu3DModelLayerSet(modelId, 0);
    Hu3DModelAttrReset(modelId, 1);
    Hu3DModelAttrSet(modelId, 0x1000);
    Hu3DModelShadowMapSet(modelId);
    pos.x = lbl_1_rodata_1D0 + lbl_1_rodata_1D4 * (f32) arg1;
    pos.y = lbl_1_rodata_190;
    pos.z = lbl_1_rodata_1D8 + lbl_1_rodata_1D4 * (f32) arg2;
    Hu3DModelPosSet(modelId, pos.x, pos.y, pos.z);
    *(Lbl1VecF *) ((u32) arg0 + 0x10) = pos;
}

void fn_1_62A8(u8 *arg0) {
    Hu3DModelKill(*(s16 *) ((u32) arg0 + 8));
    memset(arg0, 0, 0x48);
}
