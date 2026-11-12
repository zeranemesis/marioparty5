#ifndef _M504DLL_H
#define _M504DLL_H

#include "game/gamework.h"
#include "game/pad.h"
#include "game/object.h"
#include "game/hu3d.h"

#include "datanum/m504.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void ObjectSetup(void);

typedef struct PadOwner_s {
    struct PadOwner_s *unk0;
    s32 playerNo;
} PADOWNER;

typedef struct PadCtrl_s {
    void *unk0;
    s32 playerNo;
    u8 unk8[0xD8];
    u16 unkE0; /* 0xE0 */
    u8 padE2[0x1E];
    /* 0x100: node position the pad is measured against, written by fn_1_BEF0
       and read by fn_1_C1E8. */
    Vec unk100;
    u8 unk10C[0x28];
    /* 0x134 / 0x135: the two stick bytes fn_1_BEF0 writes. */
    s8 stkX;
    s8 stkY;
} PADCTRL;

typedef struct InputWork_s {
    struct InputMain_s *main;
    u16 state;
    u16 unk6;
    void **dataTable;
    s32 timer;
    s32 type;
    u16 dif;
    u8 pad16[2];
    f32 smoothX; /* 0x18 */
    u32 pad1C;
    f32 smoothY; /* 0x20 */
} INPUTWORK;

typedef struct InputMain_s {
    PADOWNER *playerConf;
    INPUTWORK work;
    s32 padId;
    s32 btn;
    s32 btnDown;
    s8 stkX;
    s8 stkY;
    u16 btnRep;
    s8 subStkX;
    s8 subStkY;
    u8 trigL;
    u8 trigR;
    u8 dStk;
    u8 dStkRep;
} INPUTMAIN;


struct M504Slot_s;
struct M504Work_s;

/* One player's character state (0xD4 bytes); embedded in M504SLOT at 0x8. */
typedef struct M504Player_s {
    struct M504Slot_s *slot; /* 0x00 */
    s16 unk4;
    s16 mode; /* 0x06 */
    u16 step; /* 0x08 */
    u16 timer; /* 0x0A */
    s32 playerNo; /* 0x0C */
    s16 charNo; /* 0x10 */
    s16 pad12;
    s32 grp; /* 0x14 */
    OMOBJ *obj; /* 0x18 */
    s32 loopF; /* 0x1C */
    INPUTMAIN input; /* 0x20 */
    Vec pos; /* 0x60 */
    Vec vel; /* 0x6C */
    f32 speed; /* 0x78 */
    f32 angMove; /* 0x7C */
    f32 angDisp; /* 0x80 */
    f32 angTarget; /* 0x84 */
    s16 mot; /* 0x88 */
    s16 motNext; /* 0x8A */
    s32 unk8C;
    u16 unk90;
    u16 pad92;
    s32 unk94;
    s32 unk98;
    u16 unk9C;
    u16 unk9E;
    Vec posA0;
    Vec posAC;
    Vec posB8;
    s32 active; /* 0xC4 */
    s16 mdlC8;
    s16 mtnCA;
    s16 mdlCC;
    s16 padCE;
    s32 doneD0;
} M504PLAYER;

typedef struct M504Wanwan_s {
    struct M504Slot_s *slot; /* 0x00 */
    u16 state; /* 0x04 */
    u16 timer; /* 0x06 */
    u16 step; /* 0x08 */
    u16 count; /* 0x0A */
    s16 mdl; /* 0x0C */
    s16 mtn[10]; /* 0x0E */
    s32 unk24;
    s16 mdl28;
    s16 mot2A;
    s32 on2C;
    s16 mdl30;
    s16 mot32;
    s32 on34;
    Vec pos; /* 0x38 */
    Vec rot; /* 0x44 */
    Vec vel; /* 0x50 */
    Vec off; /* 0x5C */
    f32 f68;
    f32 f6C;
    f32 f70;
    f32 f74;
    f32 face; /* 0x78 */
    s32 active; /* 0x7C */
} M504WANWAN;

/* One collision/visibility object of the stage (0x30 bytes). */
typedef struct M504StageObj_s {
    struct M504Stage_s *stage; /* 0x00 */
    s16 mdl; /* 0x04 */
    s16 pad6;
    s32 shown; /* 0x08 */
    s32 active; /* 0x0C */
    u16 type; /* 0x10 */
    u16 pad12;
    f32 x0; /* 0x14 */
    f32 y0;
    f32 z0;
    f32 x1;
    f32 y1;
    f32 z1;
    s32 hidden; /* 0x2C */
} M504STAGEOBJ;

typedef struct M504Stage_s {
    struct M504Slot_s *slot; /* 0x00 */
    s16 mdl4; /* 0x04 */
    s16 mdl6;
    s16 mdl8;
    s16 mdlA;
    s16 mdlC;
    s16 mdlE;
    s16 mdl10;
    s16 mdl12;
    s16 mdl14;
    s16 mdl16;
    s16 mdl18;
    s16 mdl1A;
    s16 mdl1C;
    s16 mdl1E;
    s16 mdl20;
    s16 mdl22;
    s16 mtn24[3];
    u16 st2A;
    s16 tm2C;
    s16 mdl2E[3];
    s16 mtn34[7];
    u16 st42[3];
    u8 pad48[2];
    s16 mdl4A;
    s16 mdl4C;
    s16 mdl4E;
    s16 mdl50;
    s16 mtn52;
    s16 mtn54;
    s16 mtn56;
    s16 mtn58;
    s16 pad5A;
    s32 unk5C;
    s16 mdl60;
    s16 mdl62;
    s16 mdl64;
    s16 mtn66;
    s16 mtn68;
    s16 mtn6A;
    u16 st6C;
    u16 st6E;
    s16 pad70;
    s16 mdl72;
    s16 mtn74[2];
    u16 st78;
    s16 pad7A;
    M504STAGEOBJ obj[10]; /* 0x7C */
    s32 pad25C;
} M504STAGE;

typedef struct M504Camera_s {
    struct M504Slot_s *slot; /* 0x00 */
    s32 camBit; /* 0x04 */
    s32 unk8;
    Vec posTgt; /* 0x0C */
    Vec lookTgt; /* 0x18 */
    Vec pos; /* 0x24 */
    Vec look; /* 0x30 */
    Vec shakePos; /* 0x3C */
    Vec shakeLook; /* 0x48 */
    Vec up; /* 0x54 */
    f32 smooth; /* 0x60 */
    u16 shakeType; /* 0x64 */
    u16 pad66;
    s32 shakeCnt; /* 0x68 */
    f32 shakeAmp; /* 0x6C */
    f32 shakeDecay; /* 0x70 */
    f32 shakeSpd; /* 0x74 */
    f32 shakePhase; /* 0x78 */
    f32 blend; /* 0x7C */
    Vec fromPos; /* 0x80 */
    Vec fromLook; /* 0x8C */
    Vec toPos; /* 0x98 */
    Vec toLook; /* 0xA4 */
} M504CAMERA;

typedef struct M504Slot_s {
    struct M504Work_s *work; /* 0x00 */
    s32 slotNo; /* 0x04 */
    M504PLAYER player; /* 0x08 */
    M504WANWAN wanwan; /* 0xDC */
    M504STAGE stage; /* 0x15C */
    M504CAMERA cam; /* 0x3BC */
    s32 unk46C;
    struct MgTimer_s *timer; /* 0x470 */
} M504SLOT;

typedef struct M504Light_s {
    s32 unk0;
    HU3DLIGHTID lightId;
} M504LIGHT;

typedef struct M504Work_s {
    HUPROCESS *proc; /* 0x00 */
    s32 unk4;
    s32 unk8;
    s32 timer; /* 0x0C */
    u16 unk10;
    s16 mdl[19]; /* 0x12 */
    M504LIGHT light; /* 0x38 */
    M504SLOT slot[4]; /* 0x40 */
    s32 unk1210;
    s32 winner; /* 0x1214 */
    s32 unk1218;
    s16 esp121C;
    s16 esp121E;
    s32 gameMes; /* 0x1220 */
    s32 unk1224;
    s32 unk1228;
    s32 unk122C;
    s32 unk1230;
    s32 unk1234;
} M504WORK;

extern M504WORK *lbl_1_bss_0;
extern s32 lbl_1_data_45E0[4];

int _prolog(void);
void _epilog(void);

#endif