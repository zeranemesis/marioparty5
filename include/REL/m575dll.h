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
// fields below. Refined when tuboeff itself is written.
typedef struct M575TuboEff_s {
    u8 unk0[0x48];
    Vec mtxRoot;
    u8 unk54[4];
    Vec dir;
    u8 unk64[0x44];
    Vec pos;
    Vec vel;
    Vec offset;
    u8 unkCC[0x20];
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

void fn_1_3A7C(M575Score *self, u16 side, s32 arg2);
void fn_1_3CD8(M575Score *self);
void fn_1_3D28(M575Score *self);
void fn_1_3D5C(M575Score *self, s32 disp);

void fn_1_4BCC(M575Ten *self, M575Main *main);
void fn_1_4E98(M575Ten *self);
void fn_1_4EF0(M575Ten *self);

#endif
