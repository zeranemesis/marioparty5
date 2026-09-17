#ifndef _MSM_MSMSYS_H
#define _MSM_MSMSYS_H

#include "msm.h"
#include "msm/msm_data.h"

#define MSM_AUX_REVERBHI 0
#define MSM_AUX_REVERBSTD 1
#define MSM_AUX_CHORUS 2
#define MSM_AUX_DELAY 3

#define MSM_AUXNO_NULL -2
#define MSM_AUXNO_UNSET -1

#define MSM_GRP_BASE_MAX 15

typedef union MSMAux_u {
    SND_AUX_REVERBHI revHi;
    SND_AUX_REVERBSTD revStd;
    SND_AUX_CHORUS chorus;
    SND_AUX_DELAY delay;
} MSM_AUX; // Size 0x1E0

typedef struct MSMGrpStack_s {
    /* 0x0 */ s8 grpId;
    /* 0x1 */ s8 baseGrpF;
    /* 0x4 */ u32 num;
    /* 0x8 */ void *buf;
} MSM_GRP_STACK; // Size 0xC

typedef struct MSMSys_s {
    /* 0x000 */ s32 msmEntryNum;
    /* 0x004 */ s8 outputMode;
    /* 0x005 */ s8 unk5;
    /* 0x006 */ s8 unk6;
    /* 0x007 */ s8 arInitF;
    /* 0x008 */ s32 aramP;
    /* 0x00C */ MSM_HEADER *header;
    /* 0x010 */ MSM_INFO *info;
    /* 0x014 */ MSM_AUXPARAM *auxParam;
    /* 0x018 */ s8 auxParamNo[2];
    /* 0x01C */ MSM_AUX aux[2]; // 0: userA, 1: userB
    /* 0x3DC */ s8 grpMax;
    /* 0x3DD */ s8 unk3DD;
    /* 0x3DE */ s8 unk3DE;
    /* 0x3DF */ s8 grpLoadMode;
    /* 0x3E0 */ s32 grpNum;
    /* 0x3E4 */ u32 sampSize;
    /* 0x3E8 */ u32 sampSizeBase;
    /* 0x3EC */ MSM_GRP_INFO *grpInfo;
    /* 0x3F0 */ MSM_GRP_SET *grpSet;
    /* 0x3F4 */ s8 baseGrpNum;
    /* 0x3F8 */ MSM_GRP_HEAD *grpData[MSM_GRP_BASE_MAX];
    /* 0x434 */ s8 grpStackAMax;
    /* 0x435 */ u8 grpStackADepth;
    /* 0x436 */ s8 grpStackAOfs;
    /* 0x438 */ void *grpBufA;
    /* 0x43C */ MSM_GRP_STACK grpStackA[4];
    /* 0x46C */ s8 grpStackBMax;
    /* 0x46D */ u8 grpStackBDepth;
    /* 0x46E */ s8 grpStackBOfs;
    /* 0x470 */ void *grpBufB;
    /* 0x474 */ MSM_GRP_STACK grpStackB[10];
    /* 0x4EC */ s8 grpPushNum;
    /* 0x4ED */ s8 grpPushId[MSM_GRP_BASE_MAX];
    /* 0x4FC */ s32 irqDepth;
    /* 0x500 */ BOOL irqState;
    /* 0x504 */ AIDCallback oldAIDCallback;
    /* 0x508 */ s32 timer;
} MSM_SYS; // Size 0x50C

void msmSysIrqDisable(void);
void msmSysIrqEnable(void);
BOOL msmSysCheckBaseGroup(s32 grpId);
void *msmSysGetGroupDataPtr(s32 grpId);
BOOL msmSysCheckLoadGroupID(s32 grpId);
s32 msmSysGetOutputMode(void);

#endif
