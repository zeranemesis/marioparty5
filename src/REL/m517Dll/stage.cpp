// m517Dll stage: the stage singleton. It loads the three stage models, sets up
// light, shadow and fog and spawns the two kuribos.
#define _MATH_H
#include "REL/m517Dll.h"

#ifdef __MWERKS__
#define M517_RODATA __declspec(section ".rodata")
#else
#define M517_RODATA
#endif

extern "C" {

void fn_1_E81C(M517Stage *self);
void fn_1_E820(s16 layerNo);
void fn_1_E96C(s16 layerNo);
void fn_1_EAB8(M517Stage *self, s32 bit);
M517Stage *fn_1_EAE0(M517Stage *self, s16 flag);

// Position, rotation and type of the two kuribos (four entries are stored).
M517_RODATA const Vec lbl_1_rodata_2A0[4] = {
    { 600.0f, 245.0f, -1085.0f }, { -950.0f, 147.0f, -587.0f }, { 545.0f, 257.0f, 498.0f }, { -200.0f, 61.0f, 752.0f },
};
M517_RODATA const Vec lbl_1_rodata_2D0[4] = {
    { 0.0f, -8.0f, 0.0f }, { 0.0f, 65.0f, 0.0f }, { 0.0f, -130.0f, 0.0f }, { 0.0f, 170.0f, 0.0f },
};
M517_RODATA const s32 lbl_1_rodata_300[4] = { 2, 0, 0, 1 };

GXColor lbl_1_data_4F0 = { 0xF0, 0xFA, 0xA0, 0x00 };
Vec lbl_1_data_4F4 = { 1.0f, 4179.73388671875f, 3252.0009765625f };
GXColor lbl_1_data_500 = { 0xFF, 0xFF, 0xFF, 0xFF };
char lbl_1_data_504[] = "ms_pointer == 0";
char lbl_1_data_514[] = "singleton.h";
char lbl_1_data_520[] = "ms_pointer != 0";
void *lbl_1_data_530[6] = { 0, 0, (void *)fn_1_EAE0, (void *)fn_1_EAB8, (void *)fn_1_E81C, 0 };

M517Stage *fn_1_E3A4(M517Stage *self)
{
    Vec lightDir;
    Vec camTarget;
    Vec camUp;
    Vec camPos;
    Vec zero;
    HU3DLIGHTID light;
    s32 i;

    fn_1_2DA8(&self->task.link);
    self->task.vtbl = &lbl_1_data_68.vtbl;
    self->task.flags = 0;
    fn_1_2E74(&self->task.link, &lbl_1_bss_24);
    M517_ASSERT(lbl_1_bss_10 == 0, lbl_1_data_504, lbl_1_data_514, 0x35);
    M517_SINGLETON_SET(lbl_1_bss_10, self, M517Stage);
    self->task.vtbl = (M517TaskVtbl *)lbl_1_data_530;

    fn_1_474C(&self->model[0], M517_HSF_stage1);
    fn_1_474C(&self->model[1], M517_HSF_stage2);
    fn_1_474C(&self->model[2], M517_HSF_stage3);

    Hu3DBGColorSet(lbl_1_data_4F0.r, lbl_1_data_4F0.g, lbl_1_data_4F0.b);
    Hu3DLighInit();
    PSVECNormalize(&lbl_1_data_4F4, &lightDir);
    PSVECScale(&lightDir, &lightDir, -1.0f);
    light = Hu3DGLightCreateV(&lbl_1_data_4F4, &lightDir, &lbl_1_data_500);
    Hu3DGLightInfinitytSet(light);
    Hu3DGLightStaticSet(light, 1);
    Hu3DShadowCreate(20.0f, 100.0f, 50000.0f);
    Hu3DShadowTPLvlSet(0.4f);

    camTarget.x = 0.0f;
    camTarget.y = 0.0f;
    camTarget.z = 0.0f;
    PSVECScale(&lightDir, &camPos, -3000.0f);
    PSVECAdd(&camPos, &camTarget, &camPos);
    camUp.x = 0.0f;
    camUp.y = 1.0f;
    camUp.z = 0.0f;
    Hu3DShadowPosSet(&camPos, &camUp, &camTarget);
    Hu3DShadowColSet(0, 0, 0x40);

    Hu3DModelShadowMapSet(self->model[1].id);
    Hu3DModelLayerSet(self->model[1].id, 1);
    Hu3DModelLayerSet(self->model[0].id, 1);
    Hu3DModelLayerSet(self->model[2].id, 1);
    Hu3DModelAttrSet(self->model[2].id, 0x40000001);
    zero.x = 0.0f;
    zero.y = 0.0f;
    zero.z = 0.0f;
    fn_1_49AC(&self->model[1], &zero);
    self->offs.x = 0.0f;
    self->offs.y = 0.0f;
    self->offs.z = 0.0f;
    Hu3DFogSet(1000.0f, 1500.0f, lbl_1_data_4F0.r, lbl_1_data_4F0.g, lbl_1_data_4F0.b);
    Hu3DLayerHookSet(0, fn_1_E820);
    Hu3DLayerHookSet(1, fn_1_E96C);

    for (i = 0; i < 2; i++) {
        struct M517Kuribo_s *kuribo = (struct M517Kuribo_s *)fn_1_1E84(M517_KURIBO_SIZE);

        if (kuribo != 0) {
            fn_1_C994(kuribo, (Vec *)&lbl_1_rodata_2A0[i], (Vec *)&lbl_1_rodata_2D0[i], lbl_1_rodata_300[i]);
        }
    }
    return self;
}

M517Stage *fn_1_E790(M517Stage *self, s16 flag)
{
    if (self != 0) {
        M517_ASSERT(lbl_1_bss_10 != 0, lbl_1_data_520, lbl_1_data_514, 0x3D);
        lbl_1_bss_10 = 0;
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

void fn_1_E81C(M517Stage *self)
{
}

// Layer 0 hook: linear fog that starts at the camera-to-focus distance.
void fn_1_E820(s16 layerNo)
{
    HU3DCAMERA *cam = &Hu3DCamera[0];
    Vec camPos = cam->pos;
    Vec camTarget = cam->target;
    Vec dir;
    Vec diff;
    f32 dist;

    camPos.y = 0.0f;
    camTarget.y = 0.0f;
    PSVECSubtract(&camTarget, &camPos, &dir);
    PSVECNormalize(&dir, &dir);
    PSVECScale(&dir, &dir, 100.0f);
    PSVECSubtract(&dir, &camPos, &diff);
    dist = PSVECMag(&diff);
    GXSetFog(GX_FOG_PERSP_LIN, dist, 2000.0f + dist, cam->near, cam->far, lbl_1_data_4F0);
}

// Layer 1 hook: exponential fog ending at the camera-to-focus distance.
void fn_1_E96C(s16 layerNo)
{
    HU3DCAMERA *cam = &Hu3DCamera[0];
    Vec camPos = cam->pos;
    Vec camTarget = cam->target;
    Vec dir;
    Vec diff;
    f32 dist;

    camPos.y = 0.0f;
    camTarget.y = 0.0f;
    PSVECSubtract(&camTarget, &camPos, &dir);
    PSVECNormalize(&dir, &dir);
    PSVECScale(&dir, &dir, 2200.0f);
    PSVECSubtract(&dir, &camPos, &diff);
    dist = PSVECMag(&diff);
    GXSetFog(GX_FOG_PERSP_EXP, dist - 200.0f, dist, cam->near, cam->far, lbl_1_data_4F0);
}

// Virtual onFlag(): clear the flag bit.
void fn_1_EAB8(M517Stage *self, s32 bit)
{
    self->task.flags &= ~(1 << bit);
    if (bit < 5) {
    }
}

M517Stage *fn_1_EAE0(M517Stage *self, s16 flag)
{
    if (self != 0) {
        self->task.vtbl = (M517TaskVtbl *)lbl_1_data_530;
        fn_1_4BAC(&self->model[2], -1);
        fn_1_4BAC(&self->model[1], -1);
        fn_1_4BAC(&self->model[0], -1);
        if (M517_SUBOBJ_NONNULL(self)) {
            M517_ASSERT(lbl_1_bss_10 != 0, lbl_1_data_520, lbl_1_data_514, 0x3D);
            lbl_1_bss_10 = 0;
        }
        if (self != 0) {
            self->task.vtbl = &lbl_1_data_68.vtbl;
            if (self != 0) {
                fn_1_2DB4(&self->task.link, 0);
            }
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

}
