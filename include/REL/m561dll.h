#ifndef _M561DLL_H
#define _M561DLL_H

#include "dolphin.h"

typedef struct M561EffectParam_s {
    s32 flags;
    GXColor colorStart;
    GXColor colorEnd;
    HuVecF vel;
    HuVecF speedDecay;
    f32 accelY;
    f32 unk28;
    f32 scaleDecay;
    f32 alphaStep;
    f32 colorStep;
} M561EffectParam;

extern s16 lbl_1_bss_2E0[16];
extern void *lbl_1_bss_300;

void fn_1_8AE8(void);
void fn_1_8B38(void);
s32 fn_1_8BCC(s16 arg0, M561EffectParam *arg1);
s32 fn_1_8F5C(s16 arg0, M561EffectParam *arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5);
void fn_1_9200(void);
void fn_1_9464(void);
void fn_1_9688(HU3DMODEL *modelP, HU3DPARTICLE *particleP, f32 mtx[3][4]);

#endif
