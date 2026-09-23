#include "game/hu3d.h"
#include "string.h"

typedef struct MgLight_s {
    s32 unk0;
    HU3DLIGHTID lightId;
} MgLight;

HuVecF lbl_1_data_C40 = { 7000.0f, 7000.0f, 7000.0f };
HuVecF lbl_1_data_C4C = { -1.0f, -1.0f, -1.0f };
GXColor lbl_1_data_C58 = { 255, 255, 255, 255 };

void fn_1_FCBC(MgLight *light, s32 arg1)
{
    memset(light, 0, sizeof(MgLight));
    light->unk0 = arg1;
    light->lightId = Hu3DGLightCreateV(&lbl_1_data_C40, &lbl_1_data_C4C, &lbl_1_data_C58);
    Hu3DGLightStaticSet(light->lightId, TRUE);
    Hu3DGLightInfinitytSet(light->lightId);
}

void fn_1_FD34(MgLight *light)
{
    if (light->lightId != 0) {
        Hu3DGLightKill(light->lightId);
        light->lightId = 0;
    }
    memset(light, 0, sizeof(MgLight));
}

void fn_1_FD88(MgLight *light)
{
}
