#ifndef M542DLL_TYPES_H
#define M542DLL_TYPES_H

#include "types.h"
#include "game/hu3d.h"
#include "game/object.h"

/* One model entry of the lbl_1_bss_1588 table. */
typedef struct {
    s16 model;                 // 0x00
    HuVecF pos;                // 0x04
    HuVecF rot;                // 0x10
} M542ENTRY;                   // 0x1c bytes

typedef struct {
    s16 playerNo;              // 0x00
    s16 charNo;                // 0x02
    s16 dif;                   // 0x04
    s16 padNo;                 // 0x06
    s32 btn;                   // 0x08
    s32 btnDown;               // 0x0c
    s8 stkX;                   // 0x10
    s8 stkY;                   // 0x11
    u16 btnRep;                // 0x12
    s8 subStkX;                // 0x14
    s8 subStkY;                // 0x15
    u8 trigL;                  // 0x16
    u8 trigR;                  // 0x17
    u8 dStk;                   // 0x18
    u8 dStkRep;                // 0x19
    u8 pad_1a[2];
} M542PLAYER;                  // 0x1c bytes

/* Per-player character animation state (0x24 bytes each). */
typedef struct {
    s16 mot;                   // 0x00
    s16 motCur;                // 0x02
    f32 speed;                 // 0x04
    f32 speedCur;              // 0x08
    f32 time;                  // 0x0c
    f32 timeCur;               // 0x10
    f32 maxTime;               // 0x14
    u32 attrSet;               // 0x18
    u32 attrCur;               // 0x1c
    u32 attrReset;             // 0x20
} M542SLOT;                    // 0x24 bytes

typedef struct {
    s32 state;                 // 0x00
    s32 cnt;                   // 0x04
    u8 pad_8[8];
} M542JUMP;                    // 0x10 bytes

typedef struct {
    OMOBJMAN *objman;          // 0x00
    M542PLAYER player[2];      // 0x04
    M542ENTRY *ents;           // 0x3c
    s32 *motData[2];           // 0x40
    s16 *motId[2];             // 0x48
    M542SLOT slot[2];          // 0x50
    s32 *data;                 // 0x98
    s16 f_9c;                  // 0x9c
    union {
        s32 st[4];             // 0xa0 (the original indexes fall as st[i + 2])
        struct {
            s32 mode;          // 0xa0
            s32 timer;         // 0xa4
            s32 fall[2];       // 0xa8
        };
    };
    s32 phase;                 // 0xb0
    s32 power[2];              // 0xb4
    s32 f_bc;                  // 0xbc
    s32 f_c0;                  // 0xc0
    M542JUMP jump[2];          // 0xc4
    s32 wait[2];               // 0xe4
    s32 anim[2];               // 0xec
    f32 gauge[2];              // 0xf4
    s32 ang[2][2];             // 0xfc
    u8 pad_10c[0x1c];          // 0x10c
    f32 vel[2][3];             // 0x128
    u8 pad_140[0x14];          // 0x140
} M542WORK;                    // 0x154 bytes

/* Dust/spark particle (0x2c bytes each, 4 groups of 30). */
typedef struct {
    s32 active;                // 0x00
    s32 cnt;                   // 0x04
    s32 f_08;                  // 0x08
    f32 ang;                   // 0x0c
    s16 model;                 // 0x10
    HuVecF pos;                // 0x14
    HuVecF rot;                // 0x20
} M542PART;                    // 0x2c bytes

typedef struct {
    s32 state;                 // 0x00
    s32 step;                  // 0x04
    s32 idx;                   // 0x08
    s32 cnt;                   // 0x0c
    f32 ang;                   // 0x10
    HuVecF pos;                // 0x14
} M542EMIT;                    // 0x20 bytes

/* CPU player decision state (0x1c bytes each). */
typedef struct {
    s16 kind;                  // 0x00
    s32 step;                  // 0x04
    s32 timer;                 // 0x08
    s32 holdMax;               // 0x0c
    s32 holdTime;              // 0x10
    s32 target;                // 0x14
    s32 wait;                  // 0x18
} M542AI;                      // 0x1c bytes

extern M542WORK lbl_1_bss_1954;
extern M542ENTRY lbl_1_bss_1588[0x22];
extern M542PART lbl_1_bss_E8[4][30];
extern s16 lbl_1_bss_1940[2][5];
extern M542EMIT lbl_1_bss_1AA8[4];
extern s16 lbl_1_bss_1B28;
extern HuVecF lbl_1_data_3B0[6];
extern s32 lbl_1_data_0[0x20];
extern s32 lbl_1_data_80[13][9];
extern s16 lbl_1_bss_0;
extern s32 lbl_1_bss_4;
extern s32 lbl_1_bss_8;
extern s32 lbl_1_bss_C;
extern u8 lbl_1_bss_10;
/* lbl_1_bss_14 is 0x40 bytes long in the original (two entries plus tail padding). */
typedef struct {
    M542AI ai[2];
    s32 pad[2];
} M542AITAB;

extern M542AITAB lbl_1_bss_14;
extern s16 lbl_1_bss_54[2][9];
extern OMCAMERAVIEW lbl_1_bss_78[4];

#endif
