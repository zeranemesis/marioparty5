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

/* Real Hu3DData[] entry layout. The shared hu3d.h Hu3DModel_s stops at 0x93,
   but the engine indexes Hu3DData with a 0x134 stride and these functions read
   particle state past that, so the offsets are spelled out locally. */

/* Particle data record reached through the 0x48 pointer, stride 0x44. */
typedef struct M561ParticleData_s {
    u8 unk00[0x2C];
    /* 0x2C */ f32 unk2C;
} M561ParticleData;

typedef struct M561Hu3DParm_s M561Hu3DParm;

typedef struct M561Hu3DModel_s {
    /* 0x00 */ u8 unk00[2];
    /* 0x02 */ s16 unk02;
    /* 0x04 */ u8 unk04[0x18];
    /* 0x1C */ void *unk1C;
    /* 0x20 */ u8 unk20[0x10];
    /* 0x30 */ s16 unk30;
    /* 0x32 */ u8 unk32[2];
    /* 0x34 */ s32 unk34;
    /* 0x38 */ u8 unk38[0xC];
    /* 0x44 */ void *unk44;
    /* 0x48 */ M561ParticleData *unk48;
    /* 0x4C */ u8 unk4C[0xD4];
    /* 0x120 */ M561Hu3DParm *parm;
} M561Hu3DModel;

/* Per-model particle parameter block reached through the 0x120 pointer. */
typedef struct M561Hu3DParm_s {
    /* 0x00 */ u8 unk00[2];
    /* 0x02 */ s16 unk02;
    /* 0x04 */ u8 unk04[0x18];
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ u8 unk20[0x10];
    /* 0x30 */ s16 dataCnt;
    /* 0x32 */ u8 unk32[2];
    /* 0x34 */ s32 unk34;
    /* 0x38 */ u8 unk38[0xC];
    /* 0x44 */ void *unk44;
    /* 0x48 */ M561ParticleData *data;
} M561Hu3DParm;

extern s16 lbl_1_bss_2E0[16];
extern void *lbl_1_bss_300;

void fn_1_8AE8(void);
void fn_1_8B38(void);
void fn_1_8BCC(s16 arg0, Vec *arg1);
void fn_1_8F5C(s16 arg0, M561EffectParam *arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5);
void fn_1_9200(void);
void fn_1_9464(void);
void fn_1_9688(HU3DMODEL *modelP, HU3DPARTICLE *particleP, f32 mtx[3][4]);

#endif
