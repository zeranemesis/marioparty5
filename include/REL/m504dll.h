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
    u16 unkE0;
} PADCTRL;

typedef struct InputWork_s {
    struct InputMain_s *main;
    u16 state;
    u16 unk6;
    void **dataTable;
    s32 timer;
    s32 type;
    u16 dif;
    u8 unk18[14];
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

int _prolog(void);
void _epilog(void);

#endif