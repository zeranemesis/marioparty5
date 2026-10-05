#include "types.h"

typedef s16 HU3DMODELID;
typedef s16 HU3DMOTID;

HU3DMODELID CharModelCreate(s16 charNo, s16 model);
HU3DMOTID CharMotionCreate(s16 charNo, u32 dataNum);
void Hu3DModelPosSet(HU3DMODELID modelId, f32 posX, f32 posY, f32 posZ);
void Hu3DModelAttrSet(HU3DMODELID modelId, u32 attr);
void Hu3DModelCameraSet(HU3DMODELID modelId, u16 cameraBit);
void Hu3DModelLayerSet(HU3DMODELID modelId, s16 layerNo);

void fn_1_95A0(void);
void fn_1_9BD8(void);
void fn_1_A124(void);
void fn_1_AA08(void);
void fn_1_B414(void);

/* per-character slot, 0x1C bytes, charNo lives at +0x06 */
typedef struct M547CharSlot_s {
    u8 pad_00[6];
    s16 charNo;
    u8 pad_08[0x14];
} M547CharSlot;

/* per-stage slot, 0x24 bytes, id at +0x02, scale xy at +0x04/+0x08 */
typedef struct M547StageSlot_s {
    u16 pad_00;
    s16 id;
    f32 x;
    f32 y;
    u8 pad_0C[0x18];
} M547StageSlot;

typedef struct M547Stage_s {
    M547CharSlot chars[2];
    u8 pad_38[4];
    s16 (*modelIds)[14];
    u32 *motionData[2];
    s16 *motionIds[2];
    M547StageSlot slots[2];
} M547Stage;

M547Stage *lbl_1_bss_6E8;
void (*lbl_1_data_3EC[5])(void) = { fn_1_95A0, fn_1_9BD8, fn_1_A124, fn_1_AA08, fn_1_B414 };

void fn_1_8B9C(u32 arg0, u32 arg1)
{
    lbl_1_bss_6E8->motionData[0] = (u32 *)arg0;
    lbl_1_bss_6E8->motionData[1] = (u32 *)arg1;
}

void fn_1_8BC0(u32 arg0, u32 arg1)
{
    lbl_1_bss_6E8->motionIds[0] = (s16 *)arg0;
    lbl_1_bss_6E8->motionIds[1] = (s16 *)arg1;
}

void fn_1_8BE4(void)
{
    s32 i;

    // seeds 0.0f into the rodata pool ahead of 100.0f, matching the original pool order
    (void)0.0f;
    for (i = 0; i < 2; i++) {
        lbl_1_bss_6E8->modelIds[i][182] = CharModelCreate(lbl_1_bss_6E8->chars[i].charNo, 1);
        Hu3DModelPosSet(lbl_1_bss_6E8->modelIds[i][182], (i * 200) - 100, 100.0f, 0.0f);
        Hu3DModelAttrSet(lbl_1_bss_6E8->modelIds[i][182], 0x40000001);
        Hu3DModelCameraSet(lbl_1_bss_6E8->modelIds[i][182], 1);
        Hu3DModelLayerSet(lbl_1_bss_6E8->modelIds[i][182], 0);
    }
}

void fn_1_91E8(void)
{
    s16 i;
    s16 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 7; j++) {
            lbl_1_bss_6E8->motionIds[i][j] =
                CharMotionCreate(lbl_1_bss_6E8->chars[i].charNo, lbl_1_bss_6E8->motionData[i][j]);
        }
    }
    for (i = 0; i < 2; i++) {
        lbl_1_bss_6E8->slots[i].id = -1;
        lbl_1_bss_6E8->slots[i].x = 1.0f; 
        lbl_1_bss_6E8->slots[i].y = 1.0f;
    }
}

void fn_1_954C(void)
{
    s32 i;

    for (i = 0; i < 5; i++) {
        lbl_1_data_3EC[i]();
    }
}
