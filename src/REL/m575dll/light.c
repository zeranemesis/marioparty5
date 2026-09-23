#include "game/hu3d.h"
#include "string.h"

typedef struct MgLight_s {
    s32 unk0;
    HU3DLIGHTID lightId;
} MgLight;

HuVecF lbl_1_data_2C0 = { 7000.0f, 7000.0f, 7000.0f };
HuVecF lbl_1_data_2CC = { -1.0f, -1.0f, -1.0f };
GXColor lbl_1_data_2D8 = { 255, 255, 255, 255 };

void fn_1_9250(MgLight *light, s32 arg1)
{
    memset(light, 0, sizeof(MgLight));
    light->unk0 = arg1;
    light->lightId = Hu3DGLightCreateV(&lbl_1_data_2C0, &lbl_1_data_2CC, &lbl_1_data_2D8);
    Hu3DGLightStaticSet(light->lightId, TRUE);
    Hu3DGLightInfinitytSet(light->lightId);
}

void fn_1_92C8(MgLight *light)
{
    if (light->lightId != 0) {
        Hu3DGLightKill(light->lightId);
        light->lightId = 0;
    }
    memset(light, 0, sizeof(MgLight));
}

void fn_1_931C(MgLight *light)
{
}
