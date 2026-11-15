#ifndef _SDROOMDLL_H
#define _SDROOMDLL_H

#include "game/hu3d.h"
#include "game/object.h"
#include "game/process.h"
#include "game/pad.h"
#include "game/audio.h"
#include "game/wipe.h"
#include "game/gamework.h"
#include "game/data.h"
#include "datadir_enum.h"
#include <math.h>

/* machine part table entry: 3 model files, price and 3 flag bytes */
typedef struct SdMachinePart_s {
    unsigned int file[3]; //0x00
    u8 cost; //0x0C
    s8 unkD; //0x0D
    s8 unkE; //0x0E
    s8 unkF; //0x0F
} SDMACHINE_PART;

typedef struct SdMachineBody_s {
    unsigned int file[4]; //0x00
    s32 unk10[3]; //0x10
    float yOfs; //0x1C
    float yOfs2; //0x20
    float unk24; //0x24
    float unk28; //0x28
    float unk2C; //0x2C
    float unk30; //0x30
    s32 flags; //0x34
} SDMACHINE_BODY;

typedef struct SdMachineHead_s {
    unsigned int file; //0x00
    s32 unk4[3]; //0x04
    float unk10; //0x10
    float unk14; //0x14
    s32 unk18; //0x18
} SDMACHINE_HEAD;

typedef struct SdMachineTail_s {
    unsigned int file; //0x00
    unsigned int file2; //0x04
    s32 unk8[5]; //0x08
} SDMACHINE_TAIL;

/* partNo[0..3] = head, body, tail, item (-1 = none) */
typedef struct SdMachineDisp_s {
    u8 unk0[0x12];
    s16 partNo[4]; //0x12
    s16 headMdl; //0x1A
    s16 bodyMdl[4]; //0x1C
    s16 tailMdl; //0x24
    s16 itemMdl; //0x26
    float yOfs; //0x28
} SDMACHINE_DISP;

extern SDMACHINE_PART *SdMachinePartTbl[5];
extern s16 SdMachinePartNum[6];

/* defined in garage.c */
extern float SdMachineFadeAlpha; //lbl_1_bss_5B8
extern s16 SdMachinePadBtn[4]; //lbl_1_bss_5BC
extern s16 SdMachineFadeLayer; //lbl_1_bss_5C4
extern s16 SdMachineFadeCount; //lbl_1_bss_5C6
extern s16 SdMachineMask[4]; //lbl_1_bss_5C8

void fn_1_3E8C(s16 charNo, s16 partNo);
BOOL fn_1_3EC0(s16 charNo, s16 partNo);
void fn_1_3F00(void);
int fn_1_414C(s16 charNo, s16 partNo);
void fn_1_4238(s16 stat);
void fn_1_42E0(SDMACHINE_DISP *disp, s16 headNo, s16 bodyNo, s16 tailNo, s16 itemNo);
void fn_1_44E4(SDMACHINE_DISP *disp, s16 headNo, s16 bodyNo, s16 tailNo, s16 itemNo);
void fn_1_4534(SDMACHINE_DISP *disp, float x, float y, float z);
void fn_1_4590(SDMACHINE_DISP *disp, float x, float y, float z);
void fn_1_45E4(SDMACHINE_DISP *disp, float x, float y, float z);
void fn_1_4638(SDMACHINE_DISP *disp, s16 cameraBit);
void fn_1_4680(SDMACHINE_DISP *disp, s16 layerNo);
void fn_1_46C4(SDMACHINE_DISP *disp, BOOL dispF);
void fn_1_4724(SDMACHINE_DISP *disp);
void fn_1_480C(SDMACHINE_DISP *disp);
void fn_1_484C(s16 layerNo);
void fn_1_4D0C(s16 layerNo);
void fn_1_4E14(void);
u16 fn_1_4F24(u16 padMask);
u16 fn_1_4FD0(u16 padMask);
u16 fn_1_507C(u16 padMask);
u16 fn_1_5118(u16 padMask);
void fn_1_51C4(int headNo, s16 *mdlId);
void fn_1_521C(int bodyNo, s16 *mdlId);
void fn_1_52E0(int tailNo, s16 *mdlId);
void fn_1_5338(int itemNo, s16 *mdlId);
float fn_1_5390(int bodyNo, s16 headMdl, s16 *bodyMdl, s16 tailMdl, s16 itemMdl);

#endif
