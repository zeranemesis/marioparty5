#ifndef M554DLL_TYPES_H
#define M554DLL_TYPES_H

#include "types.h"
#include "game/hu3d.h"
#include "game/object.h"

/* One model entry of the lbl_1_bss_10C table. */
typedef struct {
    s16 model;                 // 0x00
    HuVecF pos;                // 0x04
    HuVecF rot;                // 0x10
} M554ENTRY;                   // 0x1c bytes

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
} M554PLAYER;                  // 0x1c bytes

/* Per-player animation / effect state (0x54 bytes each). */
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
    s32 endF;                  // 0x24
    s16 f_28;                  // 0x28
    s16 f_2a;                  // 0x2a
    f32 f_2c;                  // 0x2c
    u32 f_30;                  // 0x30
    u32 f_34;                  // 0x34
    s16 shiftState;            // 0x38
    s16 shiftMot;              // 0x3a
    u32 shiftAttr;             // 0x3c
    f32 shiftStart;            // 0x40
    f32 shiftEnd;              // 0x44
    f32 shiftTime;             // 0x48
    f32 shiftMax;              // 0x4c
    u8 pad_50[4];
} M554SLOT;                    // 0x54 bytes

/* Stage effect object (0x3c bytes each). */
typedef struct {
    s32 state;                 // 0x00
    s32 timer[3];              // 0x04
    s32 flag;                  // 0x10
    u8 pad_14[0x18];
    f32 f_2c;                  // 0x2c
    f32 f_30;                  // 0x30
    f32 f_34;                  // 0x34
    f32 f_38;                  // 0x38
} M554OBJ;                     // 0x3c bytes

typedef struct {
    s32 state;                 // 0x00
    s32 timer;                 // 0x04
    f32 maxTime;               // 0x08
} M554ROW14;                   // 0x0c bytes

typedef struct {
    s16 kind;                  // 0x00
    s32 flip;                  // 0x04
    s32 count;                 // 0x08
} M554ROW6C;                   // 0x0c bytes

typedef struct {
    OMOBJMAN *objman;          // 0x000
    M554PLAYER player[2];      // 0x004
    M554ENTRY *ents;           // 0x03c
    s32 *motData[2];           // 0x040
    s16 *motId[2];             // 0x048
    M554SLOT slot[2];          // 0x050
    s32 *data;                 // 0x0f8
    s16 f_fc;                  // 0x0fc
    s32 mode;                  // 0x100
    s32 f_104;                 // 0x104
    s32 f_108;                 // 0x108
    s32 winner;                // 0x10c
    s32 count[4];              // 0x110
    s32 round;                 // 0x120
    s32 f_124[2][2];           // 0x124
    f32 f_134[2][4];           // 0x134
    s32 hist[2][60];           // 0x154
    u8 pad_334[0x30];
    M554OBJ obj[10];           // 0x364
    u8 pad_5bc[0x814 - 0x5bc];
} M554WORK;                    // 0x814 bytes

extern M554WORK lbl_1_bss_B1C;
typedef struct {
    M554ROW14 row[7];
    s32 pad;                   // lbl_1_bss_14 is 0x58 bytes long in the original
} M554ROW14TAB;

extern M554ROW14TAB lbl_1_bss_14;
extern M554ROW6C lbl_1_bss_6C[2];
extern OMCAMERAVIEW lbl_1_bss_98[4];

#endif
