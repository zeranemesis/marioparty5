#include "game/hu3d.h"
#include "game/process.h"

void fn_1_BE58(void)
{
    while (TRUE) {
        HuPrcSleep(0);
    }
}

void fn_1_C630(HU3DMODEL *model)
{
    Hu3DModelShadowSet(model->unk_184);
}

void fn_1_C660(HU3DMODEL *model)
{
    Hu3DModelShadowReset(model->unk_184);
}

void fn_1_C690(HU3DMODEL *model)
{
    Hu3DModelShadowMapSet(model->unk_184);
}

void fn_1_E32C(void)
{
    while (TRUE) {
        HuPrcVSleep();
    }
}

void fn_1_E388(float *src, float *dstX, float *dstY)
{
    *dstX = src[1];
    *dstY = src[2];
}