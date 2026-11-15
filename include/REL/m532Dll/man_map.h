#ifndef _M532DLL_MAN_MAP_H
#define _M532DLL_MAN_MAP_H

extern "C" {
#include "dolphin.h"
}

// Layout of the map manager. Offsets in the comments are the GameCube ones.
typedef struct M532MapVec_s {
    u32 cap;
    u32 size;
    void **items; // handles of the 16 balls (ball + 0x10)
} M532MapVec;

typedef struct M532MapFog_s {
    s32 type;
    f32 start;
    f32 end;
    GXColor color;
} M532MapFog;

typedef struct M532ManMap_s {
    void *next; // 0x00 list node
    void *prev; // 0x04
    void **vtable; // 0x08
    s32 flags; // 0x0C
    s16 mdl[4]; // 0x10 stage, sun a, sun b, stairs (Hu3D model ids)
    M532MapVec balls; // 0x18
    f32 *sunRotA; // 0x24 rot.y of "tenmen_sun"
    f32 *sunRotB; // 0x28 rot.y of "tenmen_sun_b"
    f32 angle; // 0x2C
    f32 speed; // 0x30
    s32 se; // 0x34
    M532MapFog fog; // 0x38
} M532ManMap;

// Camera demo object: only the fields the three man_map.cpp methods touch.
typedef struct M532CamCtl_s {
    u8 pad0[0x10];
    Vec pos; // 0x10
    Vec target; // 0x1C
    Vec up; // 0x28
    u8 pad34[0x40 - 0x34];
    s16 camMdl; // 0x40
    u8 pad42[0x80 - 0x42];
    u8 freeCam; // 0x80
    u8 hold; // 0x81
    u8 pad82[2];
    Vec pos2; // 0x84
    Vec target2; // 0x90
    Vec up2; // 0x9C
} M532CamCtl;

extern "C" {
void fn_1_9B9C(M532CamCtl *self);
void fn_1_9C00(M532CamCtl *self);
void fn_1_9C38(M532CamCtl *self);
M532ManMap *fn_1_9D7C(M532ManMap *self);
void *fn_1_A190(void *self, s16 flag);
void *fn_1_A224(void *self, s16 flag);
void *fn_1_A2B0(void *self, s16 flag);
void *fn_1_A334(void *self, s16 flag);
M532ManMap *fn_1_A3C0(M532ManMap *self, s16 flag);
void fn_1_A518(M532ManMap *self);
void fn_1_A55C(M532ManMap *self);
f32 fn_1_A59C(M532ManMap *self);
s32 fn_1_A5A4(M532ManMap *self, f32 x);
void *fn_1_A6C0(M532ManMap *self, Vec *pt);
void fn_1_A7FC(M532ManMap *self, s32 bit);
void fn_1_A814(M532ManMap *self);
void fn_1_A874(M532ManMap *self);
void fn_1_A9E0(M532ManMap *self);
void fn_1_AA64(s16 layer);
void fn_1_AB38(s16 layer);
void fn_1_ABE4(M532MapVec *self, void **pos, u32 n, void **val);
}

#endif
