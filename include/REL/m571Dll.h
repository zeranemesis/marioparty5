#ifndef _REL_M571DLL_H
#define _REL_M571DLL_H

#include "types.h"
#include "game/charman.h"
#include "game/hu3d.h"
#include "game/mg/actman.h"
#include "game/mg/seqman.h"
#include "game/object.h"
#include "game/process.h"

#ifndef __MWERKS__
#include "game/mg/score.h"
#else
typedef struct MGSCORE_s MGSCORE;

MGSCORE *MgScoreCreate(int digitFile, int unitFile, BOOL dispLeadZeroF);
void MgScoreValueSet(MGSCORE *score, int value);
void MgScorePosSet(MGSCORE *score, float posX, float posY);
void MgScoreMaxDigitSet(MGSCORE *score, int maxDigit);
void MgScoreDispOn(MGSCORE *score);
void MgScoreDispOff(MGSCORE *score);
void MgScorePriSet(MGSCORE *score, s16 arg1);
void MgScoreDigitScaleSet(MGSCORE *score, float scaleX, float scaleY);
s16 MgScoreWinCreate(s16 sizeX, s16 sizeY);
void MgScoreWinTPLvlSet(s16 grpId, float tpLvl);
void MgScoreWinPosSet(s16 grpId, float posX, float posY);
void MgScoreWinDispSet(s16 grpId, BOOL disp);
#endif

typedef struct M571Pos_s {
    f32 x; //0x0
    f32 y; //0x4
    f32 z; //0x8
} M571Pos;

typedef struct M571Cell_s {
    s32 x; //0x0
    s32 y; //0x4
    s32 z; //0x8
} M571Cell;

typedef struct M571Grid_s {
    M571Pos pos[49]; //0x0 (7x7 board lattice, -360..360 step 120)
    M571Cell cell[49]; //0x24C (per-cell tile table)
} M571Grid;

typedef struct M571Player_s {
    union {
        s32 playerNo; //0x0 player number (mirror of obj->work[0])
        s16 modelId; //0x0 character model read by the hook helpers
    } unk0;
    s32 unk4; //0x4
    s32 unk8; //0x8
    s32 unkC; //0xC
    s32 unk10; //0x10
    s16 unk14; //0x14
    s16 unk16; //0x16
    Vec unk18; //0x18 mirror of the MGACTOR position
    s32 unk24; //0x24 player number again
    union {
        f32 rotY; //0x28 rotation copied from the MGACTOR, and 1300.0 on the hook reset
        s32 raw; //0x28 read back as a sign-extended half by the voice bank pick
    } unk28;
    s32 unk2C; //0x2C
    s32 unk30; //0x30
    s32 unk34; //0x34
    s32 unk38; //0x38
    s16 unk3C; //0x3C
    u8 pad3E[10]; //0x3E
    s32 unk48; //0x48
    s32 unk4C; //0x4C
    s32 unk50; //0x50
    s16 hookL; //0x54 left hand item model
    s16 hookR; //0x56 right hand item model
    s16 hookSwap; //0x58 swapped hand item model
    s32 unk5C; //0x5C character type taken from GwPlayerConf
    s32 unk60; //0x60
    Vec unk64; //0x64
    s32 unk70; //0x70
    union {
        s32 padY; //0x74 stack Y handed to MgPlayerPadSet
        f32 hookAngle; //0x74 degrees written by the hook swap
    } unk74;
    s32 unk78; //0x78
    s32 hookCycle; //0x7C counter cycling 0..2 over the hook swap
    s32 unk80; //0x80
    s32 unk84; //0x84
    s32 unk88; //0x88
} M571Player; //0x8C

typedef struct M571Tile_s {
    Vec pos; //0x0 board lattice cell
    s32 state; //0xC
    s32 timer; //0x10
} M571Tile; //0x14

typedef struct M571SpawnPt_s {
    f32 x; //0x0
    f32 y; //0x4
} M571SpawnPt; //0x8

typedef struct M571Quad_s {
    s32 v[4]; //0x0
} M571Quad; //0x10

typedef struct M571Slot_s {
    u32 val; //0x0
    s16 flag; //0x4 set to -1 by fn_1_3EE4
    u8 pad06[2]; //0x6
} M571Slot; //0x8

typedef struct M571Item_s {
    Vec pos; //0x00
    Vec rot; //0x0C
    Vec sca; //0x18
    s32 src; //0x24
    s32 slot; //0x28 index into lbl_1_bss_28C
    s32 state; //0x2C
    s32 timer; //0x30
    f32 phase; //0x34
    s32 arg; //0x38
} M571Item; //0x3C

typedef struct M571Spin_s {
    s16 curMot; //0x00
    s16 motId[3]; //0x02
    s16 modelId; //0x08
    u8 pad0A[2]; //0x0A
    Vec pos; //0x0C slot start from lbl_1_data_23C
    Vec rot; //0x18 always zero
    s32 playerNo; //0x24
    s32 charNo; //0x28
    s32 pad2C; //0x2C
    s32 score; //0x30
} M571Spin; //0x34

typedef struct M571Cursor_s {
    s32 unk0; //0x0
    s32 timer; //0x4
    Vec pos; //0x8
    Vec rot; //0x14
    Vec tgt; //0x20 3D point the 2D spawn position is projected to
    Vec step; //0x2C per-frame delta toward tgt
    f32 spawnX; //0x38 2D screen X handed to Hu3D2Dto3D
    f32 spawnY; //0x3C
    f32 spawnZ; //0x40 depth handed to Hu3D2Dto3D
    s32 variant; //0x44 index into the spawn table
} M571Cursor; //0x48

typedef struct M571ListNode_s {
    s16 id; //0x00
    s16 pad02; //0x02
    struct M571ListNode_s *prev; //0x04
    struct M571ListNode_s *next; //0x08
} M571ListNode;

// NONMATCHING: register allocation and expression form differ from the target build
typedef struct M571PopupGroup_s {
    s32 count; //0x00 popups still to spawn
    s32 timer; //0x04 frame counter, one popup every 15 frames
    s32 variant; //0x08 index into lbl_1_data_7C
    Vec pos; //0x0C world position the popups start from
} M571PopupGroup; //0x18

// The item-hook machine drives its own OMOBJ data block: the shared slots
// (0x00 model, 0x2C motion, 0x30 state, 0x38 timer, 0x54-0x58 hooks, 0x7C cycle)
// line up with M571Player, but 0x04-0x24 hold the model position and rotation
// and the block runs to 0xAC, so it is declared on its own.
typedef struct M571Hook_s {
    s16 modelId; //0x00 character model driven by the machine
    u8 pad02[2]; //0x02
    f32 posX; //0x04
    f32 posY; //0x08
    f32 posZ; //0x0C
    u8 pad10[0xC]; //0x10
    f32 rotX; //0x1C
    f32 rotY; //0x20
    f32 rotZ; //0x24
    f32 unk28; //0x28
    s32 motionNo; //0x2C motion currently requested from lbl_1_bss_44C
    s32 state; //0x30 machine state, the switch below
    s32 returnState; //0x34 state the running motion returns to, -1 for none
    s32 timer; //0x38
    f32 angle; //0x3C swinging angle in degrees
    f32 baseY; //0x40 Y the arc hangs from
    s32 duration; //0x44
    s32 duration2; //0x48
    s32 holdTime; //0x4C
    f32 speed; //0x50
    s16 hookL; //0x54 left hand model
    s16 hookR; //0x56 right hand model
    s16 hookSwap; //0x58 swapped hand model
    u8 pad5A[2]; //0x5A
    s32 unk60; //0x60
    f32 hookRotZ; //0x64
    f32 hookRotZOff; //0x68
    f32 hookPhase; //0x6C
    f32 hookRotX; //0x70
    f32 hookRotY; //0x74
    u8 pad78[4]; //0x78
    s32 cycle; //0x7C hook swap cycle
    f32 tgtX; //0x80
    f32 tgtY; //0x84
    f32 tgtZ; //0x88
    f32 startX; //0x8C
    f32 startY; //0x90
    f32 startZ; //0x94
    f32 stepX; //0x98
    f32 stepY; //0x9C
    f32 stepZ; //0xA0
    s32 idx; //0xA4 winner side being presented
    s16 hookMdl[2]; //0xA8 per-side hook models
    s16 itemMdl; //0xAC item model handed over
} M571Hook; //0xB0

/* globals, defined in m571.c */
extern u32 lbl_1_data_0[13];
extern u32 lbl_1_data_34[15];
extern Vec lbl_1_data_70;
extern M571SpawnPt lbl_1_data_7C[4];
extern Vec lbl_1_data_9C[2];
extern Vec lbl_1_data_B4[2];
extern Vec lbl_1_data_CC[2];
extern Vec lbl_1_data_E4;
extern Vec lbl_1_data_F0;
extern u8 lbl_1_data_FC[4];
extern char lbl_1_data_100[];
extern char lbl_1_data_10D[];
extern char lbl_1_data_11A[];
extern char lbl_1_data_124[];
extern u32 lbl_1_data_1A4[5];
extern OMCAMERAVIEW lbl_1_data_1B8;
extern OMCAMERAVIEW lbl_1_data_1D4;
extern OMCAMERAVIEW lbl_1_data_1F0;
extern float lbl_1_data_20C[12];
extern Vec lbl_1_data_23C[4];
extern M571Grid lbl_1_data_26C;
extern s32 lbl_1_data_704[8][49];
extern Vec lbl_1_data_D24;
extern Vec lbl_1_data_D30;
extern Vec lbl_1_data_D3C;
extern MGSEQ_PARAM lbl_1_data_D48;
extern char lbl_1_data_D70[];
extern char lbl_1_data_D94[];
extern char lbl_1_data_DA2[];
extern char lbl_1_data_DB1[];
extern char lbl_1_data_DC0[];
extern char lbl_1_data_DCF[];
extern char lbl_1_data_DDC[];
extern char lbl_1_data_DE9[];

extern OMOBJ *lbl_1_bss_0;
extern s32 lbl_1_bss_4;
extern s32 lbl_1_bss_8;
extern s16 lbl_1_bss_C;
extern s16 lbl_1_bss_E;
extern s16 lbl_1_bss_10;
extern s16 lbl_1_bss_12;
extern s16 lbl_1_bss_14[0x64];
extern MGPLAYER *lbl_1_bss_DC[4];
extern s32 lbl_1_bss_EC;
extern s32 lbl_1_bss_F0[25];
extern s16 lbl_1_bss_F0_pad;
extern s16 lbl_1_bss_156[0x35];
extern OMOBJ *lbl_1_bss_1C0;
extern OMOBJ *lbl_1_bss_1C4;
extern OMOBJ *lbl_1_bss_1C8[49];
extern OMOBJ *lbl_1_bss_28C[49];
extern OMOBJ **lbl_1_bss_350;
extern OMOBJ **lbl_1_bss_354;
extern s16 lbl_1_bss_358;
extern s16 lbl_1_bss_35A;
extern s32 lbl_1_bss_35C;
extern s32 lbl_1_bss_360;
extern s16 lbl_1_bss_364;
extern s16 lbl_1_bss_366;
extern OMOBJ *lbl_1_bss_368[20];
extern MGSCORE *lbl_1_bss_3B8[4];
extern s16 lbl_1_bss_3C8[4];
extern s16 lbl_1_bss_3D0[4];
extern s16 lbl_1_bss_3D8[4];
extern s16 lbl_1_bss_3E0[4];
extern f32 lbl_1_bss_3E8[4];
extern s32 lbl_1_bss_3F8[4];
extern s32 lbl_1_bss_408[4];
extern s32 lbl_1_bss_418[4];
extern s32 lbl_1_bss_428[4];
extern s32 lbl_1_bss_438;
extern s32 lbl_1_bss_43C[4];
extern s16 lbl_1_bss_44C[16];
extern s32 lbl_1_bss_46C;
extern OMOBJ *lbl_1_bss_470[2];
extern OMOBJ **lbl_1_bss_478;
extern M571Slot lbl_1_bss_47C[0x20];
extern u8 lbl_1_bss_57C[4];
extern s32 lbl_1_bss_580;
extern s32 lbl_1_bss_584[3];
extern Vec lbl_1_bss_590[4];
extern Vec lbl_1_bss_5C0[4];
extern Vec lbl_1_bss_5F0[4];
extern Vec lbl_1_bss_620[4];
extern Vec lbl_1_bss_650;
extern Vec lbl_1_bss_65C;
extern Vec lbl_1_bss_668;
extern Vec lbl_1_bss_674;
extern OMOBJMAN *lbl_1_bss_680;
extern s32 lbl_1_bss_684[3];

/* functions */
void ObjectSetup(void);
void fn_1_68C(OMOBJ *obj);
void fn_1_BF4(OMOBJ *obj);
void fn_1_D58(OMOBJ *obj);
void fn_1_1078(OMOBJ *obj);
s32 fn_1_1168(OMOBJ *obj);
s32 fn_1_1264(Vec *pos, Vec *out);
void fn_1_1518(void);
void fn_1_1630(void);
void fn_1_16F4(void);
void fn_1_1760(void);
void fn_1_1898(OMOBJ *obj);
void fn_1_18B8(void);
void fn_1_1BA0(void);
void fn_1_1D44(s32 tileIdx);
void fn_1_1F00(OMOBJ *obj);
void fn_1_1F74(OMOBJ *obj);
void fn_1_2088(OMOBJ *obj);
void fn_1_21DC(s32 src, f32 x, f32 y, f32 z, s32 slot, s32 arg);
void fn_1_231C(OMOBJ *obj);
void fn_1_2388(OMOBJ *obj);
void fn_1_2468(OMOBJ *obj);
void fn_1_27E8(OMOBJ *obj, s32 motNo, f32 speed, s32 attr);
void fn_1_2870(s32 flag);
s32 fn_1_28F4(HuVecF *pos);
s32 fn_1_2A14(OMOBJ *obj, s32 useAltOffset);
void fn_1_2D54(void);
void fn_1_2D58(void);
void fn_1_2E28(void);
void fn_1_2F04(s32 type, f32 x, f32 y, f32 z, s32 idx);
s32 fn_1_2FE0(OMOBJ *obj, s32 arg);
void fn_1_3520(void);
void fn_1_3594(OMOBJ *obj);
void fn_1_36B8(OMOBJ *obj);
void fn_1_3700(OMOBJ *obj);
void fn_1_38E8(OMOBJ *obj);
void fn_1_3944(void);
void fn_1_3D94(void);
void fn_1_3DB8(u16 arg0, s16 arg1);
s32 fn_1_3E40(s16 id);
void fn_1_3EE4(void);
HU3DMODELID fn_1_3F3C(u32 playerNo);
void fn_1_4050(f32 t, HuVecF *to, HuVecF *from, HuVecF *out);
void fn_1_41AC(Vec *pos, Vec *up, Vec *target);
s32 fn_1_4238(f32 t, Vec *up);
s32 fn_1_456C(f32 t, Vec *up);
s32 fn_1_4B18(s32 base, s32 range);
f32 fn_1_4950(f32 arg1, f32 arg2, f32 arg3);
void fn_1_4A54(s32 fadeSpeed);
void fn_1_4AC0(s16 modelId, s32 arg1, Vec *out);
s32 fn_1_4B54(Vec *arg0, Vec *arg1, f32 arg2, f32 arg3);
s32 fn_1_4BB4(HuVecF *pos, s32 t0, s32 t1);
s32 fn_1_4C78(HuVecF *pos, s32 t0, s32 t1);
void fn_1_4D3C(OMOBJ *obj, s32 motNo, f32 start, u32 attr);
void fn_1_4E44(OMOBJ *obj, s32 motNo, f32 start, u32 attr);
void fn_1_4EC0(OMOBJ *obj, s32 motNo);
void fn_1_4F20(OMOBJ *obj);
void fn_1_50A8(OMOBJ *obj);
void fn_1_5228(OMOBJ *obj);
void fn_1_7670(OMOBJ *obj);
s32 fn_1_7A78(void);
void fn_1_7AC8(OMOBJ *obj);
void fn_1_7DF8(OMOBJ *obj);
void fn_1_832C(void);
void fn_1_8360(void);
void fn_1_83C0(s32 arg0);
void fn_1_83D0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void fn_1_8478(s32 index, s32 value);
void fn_1_8490(s32 type);
void fn_1_8584(OMOBJ *obj);
void fn_1_8654(OMOBJ *obj);
void fn_1_8A58(Vec *pos, s32 variant);
void fn_1_8BA8(OMOBJ *obj);
void fn_1_8D54(Vec *pos, s32 variant, s32 count);
s32 fn_1_8E08(void);
void fn_1_8E5C(void);
void fn_1_8E70(void);
void fn_1_8EA0(void);
void fn_1_8948(OMOBJ *obj);
s32 fn_1_8ED0(Vec *arg0, Vec *arg1, f32 arg2, f32 arg3);
f32 fn_1_9128(f32 arg0, f32 arg1, f32 arg2, f32 arg3);
s32 fn_1_90C8(Vec *arg0, Vec *arg1, f32 arg2, f32 arg3);
s32 fn_1_9298(s32 base, s32 range);
s16 fn_1_92D4(HuVecF *pos);
s16 fn_1_9350(HuVecF *pos);

#endif
