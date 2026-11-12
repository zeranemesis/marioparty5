// m513Dll stage: the stage singleton. It loads the stage model and the two
// leaf models, sets up the light and the shadow camera.
#define _MATH_H
#include "REL/m513Dll.h"

extern "C" {

void fn_1_10848(M513Stage *self);
void fn_1_1084C(M513Stage *self, s32 bit);
M513Stage *fn_1_10874(M513Stage *self, s16 flag);

char lbl_1_data_760[] = "ms_pointer == 0";
char lbl_1_data_770[] = "singleton.h";
char lbl_1_data_77C[] = "ms_pointer != 0";
void *lbl_1_data_78C[5] = { 0, 0, (void *)fn_1_10874, (void *)fn_1_1084C, (void *)fn_1_10848 };

M513Stage *fn_1_104B0(M513Stage *self)
{
    M513Stage *ms;
    GXColor lightColor;
    Vec lightPos;
    Vec lightDir;
    Vec camTarget;
    Vec camPos;
    Vec camUp;

    fn_1_1768(&self->link);
    self->vtbl = (void **)&lbl_1_data_218;
    self->flags = 0;
    fn_1_1834(&self->link, &lbl_1_bss_C);
    M513_ASSERT(lbl_1_bss_A1C == 0, lbl_1_data_760, lbl_1_data_770, 0x35);
    ms = (M513Stage *)((char *)self + 0x10);
    if (ms != 0) {
        ms = (M513Stage *)((char *)ms - 0x10);
    }
    lbl_1_bss_A1C = ms;
    self->vtbl = lbl_1_data_78C;

    fn_1_3198(&self->model[0], M513_HSF_stage);
    fn_1_3198(&self->model[1], M513_HSF_mame);
    fn_1_3198(&self->model[2], M513_HSF_mame_small);

    Hu3DBGColorSet(0x32, 0x32, 0x9c);
    {
        char name[6] = "light";

        Hu3DModelLightInfoSet(self->model[0].id, 1);
        Hu3DGLightParamGet(Hu3DModelLightIdGet(self->model[0].id, name), &lightPos, &lightDir, &lightColor);
    }
    Hu3DShadowMultiCreate(20.0f, 100.0f, 50000.0f, 1);
    Hu3DShadowTPLvlSet(0.4f);

    camTarget.x = 0.0f;
    camTarget.y = 0.0f;
    camTarget.z = 0.0f;
    PSVECScale(&lightDir, &camPos, -3000.0f);
    PSVECAdd(&camPos, &camTarget, &camPos);

    camUp.x = 0.0f;
    camUp.y = 1.0f;
    camUp.z = 0.0f;
    Hu3DShadowMultiPosSet(&camPos, &camUp, &camTarget, 1);

    Hu3DShadowMultiColSet(0, 0, 0x40, 1);
    Hu3DShadowMultiSizeSet(0x60, 1);
    Hu3DModelShadowMapSet(self->model[0].id);
    Hu3DModelLayerSet(self->model[0].id, 1);
    Hu3DModelCameraSet(self->model[0].id, 1);
    Hu3DModelLayerSet(self->model[1].id, 1);
    Hu3DModelCameraSet(self->model[1].id, 1);
    Hu3DModelLayerSet(self->model[2].id, 3);
    Hu3DModelCameraSet(self->model[2].id, 1);
    Hu3DModelAttrSet(self->model[2].id, 0x40000001);
    return self;
}

M513Stage *fn_1_107BC(M513Stage *self, s16 flag)
{
    if (self != 0) {
        M513_ASSERT(lbl_1_bss_A1C != 0, lbl_1_data_77C, lbl_1_data_770, 0x3D);
        lbl_1_bss_A1C = 0;
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_10848(M513Stage *self)
{
}

// Virtual update(): clear the flag bit.
void fn_1_1084C(M513Stage *self, s32 bit)
{
    self->flags &= ~(1 << bit);
    if (bit < 5) {
    }
}

M513Stage *fn_1_10874(M513Stage *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_78C;
        fn_1_35F8(&self->model[2], -1);
        fn_1_35F8(&self->model[1], -1);
        fn_1_35F8(&self->model[0], -1);
        if ((void *)((char *)self + 0x10) != 0) {
            M513_ASSERT(lbl_1_bss_A1C != 0, lbl_1_data_77C, lbl_1_data_770, 0x3D);
            lbl_1_bss_A1C = 0;
        }
        if (self != 0) {
            self->vtbl = (void **)&lbl_1_data_218;
            if (self != 0) {
                fn_1_1774(&self->link, 0);
            }
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

}
