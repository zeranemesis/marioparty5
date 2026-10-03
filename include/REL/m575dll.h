#ifndef _M575DLL_H
#define _M575DLL_H

#include "types.h"
#include "dolphin/mtx.h"

typedef struct M575Score_s {
    s32 unk0;
    u16 side;
    s16 winId;
    struct MgScore_s *score;
    s16 espA;
    s16 espB;
    s16 value;
} M575Score;

// Shared by REL/m575dll/tuboeff.c; ten.c only reaches into it through the
// fields below. Layout confirmed from tuboeff.c; ten.c additionally reads the
// tube axis out of mtx row 1.
typedef struct M575TuboEff_s {
    s32 unk0;
    u16 unk4;
    f32 height;
    f32 len0;
    f32 len1;
    f32 len2;
    Mtx scaleMtx;
    Mtx mtx;
    Mtx mtx2;
    Vec pos;
    Vec vel;
    Vec angVel;
    Vec angPos;
    Vec angAcc;
    f32 unkE4;
    f32 unkE8;
} M575TuboEff;

// REL/m575dll/m575.c, still provisional: only the fields ten.c reads are named.
typedef struct M575TenConf_s {
    s32 unk0;
    s32 camBit;
} M575TenConf;

typedef struct M575Main_s {
    struct M575MainConf_s *conf;
    s16 unk4;
    u8 pad6[6];
    u16 unkC;
    u8 padE[0xE20 - 0xE];
    M575TenConf tenConf;
} M575Main;

typedef struct M575MainConf_s {
    s32 unk0;
    u8 pad4[8];
    u16 unkC;
} M575MainConf;

typedef struct M575Ten_s {
    M575Main *main;
    s16 modelId;
    s16 motionId;
    Vec *pos;
    MtxPtr mtx;
    s16 modelId2;
    Vec *pos2;
    MtxPtr mtx2;
    s16 timer;
    f32 rot;
    f32 prevRot;
    s16 counter;
    M575TuboEff eff;
} M575Ten;

// REL/m575dll/tubo.c: the ten objects that hold the tube model and its effect.
typedef struct M575Tubo_s {
    M575Main *main;
    s16 unk4;
    s16 unk6;
    s16 modelId1;
    s16 unkA;
    void *unkC;
    void *unk10;
    s16 modelId2;
    s16 motionId;
    void *unk18;
    void *unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    M575TuboEff eff;
    Mtx mtx;
    s16 unk148;
    s16 unk14A;
} M575Tubo;

void fn_1_3A7C(M575Score *self, u16 side, s32 arg2);
void fn_1_3CD8(M575Score *self);
void fn_1_3D28(M575Score *self);
void fn_1_3D5C(M575Score *self, s32 disp);

void fn_1_4BCC(M575Ten *self, M575Main *main);
void fn_1_4E98(M575Ten *self);
void fn_1_4EF0(M575Ten *self);

void fn_1_9320(M575TuboEff *eff, u16 arg1, const Vec *arg2, f32 height, f32 radius, f32 speed);
void fn_1_96F8(M575TuboEff *eff);
void fn_1_9728(M575TuboEff *eff);

#endif
