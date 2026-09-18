#ifndef _MSM_MSM_DATA_H
#define _MSM_MSM_DATA_H

#include "dolphin.h"
#include "musyx/musyx.h"

/* Layout of the sound files read from the disc (big-endian, 32-bit offsets). */

#define MSM_STREAM_FLAG_STEREO 0x1
#define MSM_STREAM_FLAG_LOOP 0x2

#define MSM_FILE_VERSION 2
#define MSM_PDT_FILE_VERSION 1

typedef struct MSMHeader_s {
    /* 0x00 */ s32 magic;
    /* 0x04 */ s32 version;
    /* 0x08 */ s32 endOfs;
    /* 0x0C */ s32 endSize;
    /* 0x10 */ s32 infoOfs;
    /* 0x14 */ u32 infoSize;
    /* 0x18 */ s32 auxParamOfs;
    /* 0x1C */ u32 auxParamSize;
    /* 0x20 */ s32 grpInfoOfs;
    /* 0x24 */ s32 grpInfoSize;
    /* 0x28 */ s32 musOfs;
    /* 0x2C */ s32 musSize;
    /* 0x30 */ s32 seOfs;
    /* 0x34 */ s32 seSize;
    /* 0x38 */ s32 grpDataOfs;
    /* 0x3C */ s32 grpDataSize;
    /* 0x40 */ s32 sampOfs;
    /* 0x44 */ s32 sampSize;
    /* 0x48 */ s32 dummyMusOfs;
    /* 0x4C */ s32 dummyMusSize;
    /* 0x50 */ s32 grpSetOfs;
    /* 0x54 */ s32 grpSetSize;
    /* 0x58 */ s32 pad[2];
} MSM_HEADER; // Size 0x60

typedef struct MSMInfo_s {
    /* 0x00 */ s8 voices;
    /* 0x01 */ s8 music;
    /* 0x02 */ s8 sfx;
    /* 0x03 */ s8 grpMax;
    /* 0x04 */ s16 musMax;
    /* 0x06 */ s16 seMax;
    /* 0x08 */ s8 musChanMax;
    /* 0x09 */ s8 stackDepthA;
    /* 0x0A */ s8 stackDepthB;
    /* 0x0B */ s8 surroundF;
    /* 0x0C */ s8 unkC[2];
    /* 0x0E */ s8 auxParamA;
    /* 0x0F */ s8 auxParamB;
    /* 0x10 */ s32 minMem;
    /* 0x14 */ s32 aramSize;
    /* 0x18 */ s32 grpBufSizeA;
    /* 0x1C */ s32 grpBufSizeB;
    /* 0x20 */ s32 dummyMusSize;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s8 baseGrpNum;
    /* 0x29 */ s8 baseGrp[23];
} MSM_INFO; // Size 0x40

typedef struct MSMAuxParamReverbHi_s {
    /* 0x00 */ u32 tempDisableFX;
    /* 0x04 */ f32 coloration;
    /* 0x08 */ f32 mix;
    /* 0x0C */ f32 time;
    /* 0x10 */ f32 damping;
    /* 0x14 */ f32 preDelay;
    /* 0x18 */ f32 crosstalk;
} MSM_AUXPARAM_REVERBHI;

typedef struct MSMAuxParamReverbStd_s {
    /* 0x00 */ u32 tempDisableFX;
    /* 0x04 */ f32 coloration;
    /* 0x08 */ f32 mix;
    /* 0x0C */ f32 time;
    /* 0x10 */ f32 damping;
    /* 0x14 */ f32 preDelay;
    /* 0x18 */ f32 crosstalk;
} MSM_AUXPARAM_REVERBSTD;

typedef struct MSMAuxParamChorus_s {
    /* 0x00 */ u32 baseDelay;
    /* 0x04 */ u32 variation;
    /* 0x08 */ u32 period;
} MSM_AUXPARAM_CHORUS;

typedef struct MSMAuxParamDelay_s {
    /* 0x00 */ u32 delay[3];
    /* 0x0C */ u32 feedback[3];
    /* 0x18 */ u32 output[3];
} MSM_AUXPARAM_DELAY;

typedef struct MSMAuxParam_s {
    /* 0x00 */ s8 type;
    /* 0x01 */ u8 pad[3];
    /* 0x04 */ union {
        MSM_AUXPARAM_REVERBHI revHi;
        MSM_AUXPARAM_REVERBSTD revStd;
        MSM_AUXPARAM_CHORUS chorus;
        MSM_AUXPARAM_DELAY delay;
    };
} MSM_AUXPARAM; // Size 0x28

typedef struct MSMMus_s {
    /* 0x00 */ SND_GROUPID sgid;
    /* 0x02 */ SND_SONGID sid;
    /* 0x04 */ s32 songOfs;
    /* 0x08 */ s32 songSize;
    /* 0x0C */ s8 songGrp;
    /* 0x0D */ s8 vol;
    /* 0x0E */ u8 pad[2];
} MSM_MUS; // Size 0x10

typedef struct MSMGrpHead_s {
    /* 0x00 */ s32 poolOfs;
    /* 0x04 */ s32 projOfs;
    /* 0x08 */ s32 sdirOfs;
    /* 0x0C */ s32 sngOfs;
} MSM_GRP_HEAD; // Size 0x10

typedef struct MSMGrpInfo_s {
    /* 0x00 */ SND_GROUPID gid;
    /* 0x02 */ s8 stackNo;
    /* 0x03 */ s8 subGrpId;
    /* 0x04 */ s32 dataOfs;
    /* 0x08 */ s32 dataSize;
    /* 0x0C */ s32 sampOfs;
    /* 0x10 */ s32 sampSize;
    /* 0x14 */ u8 pad[12];
} MSM_GRP_INFO; // Size 0x20

typedef struct MSMGrpSet_s {
    /* 0x00 */ s8 numGrpSet;
    /* 0x01 */ s8 grpSetW;
    /* 0x02 */ s8 data[1];
} MSM_GRP_SET;

typedef struct MSMStreamHeader_s {
    /* 0x00 */ s16 version;
    /* 0x02 */ s16 streamMax;
    /* 0x04 */ s32 chanMax;
    /* 0x08 */ s32 sampleFrq;
    /* 0x0C */ s32 maxBufs;
    /* 0x10 */ u32 streamPackListOfs;
    /* 0x14 */ u32 adpcmParamOfs;
    /* 0x18 */ u32 streamPackOfs;
    /* 0x1C */ u32 sampleOfs;
} MSM_STREAM_HEADER; // Size 0x20

typedef struct MSMStream_s {
    /* 0x00 */ s32 sampleOfs;
    /* 0x04 */ s16 adpcmParamIdx;
    /* 0x06 */ u16 pad;
} MSM_STREAM; // Size 0x8

typedef struct MSMStreamPack_s {
    /* 0x00 */ s8 flag;
    /* 0x01 */ s8 vol;
    /* 0x02 */ s8 pan;
    /* 0x03 */ s8 span;
    /* 0x04 */ s8 auxA;
    /* 0x05 */ s8 auxB;
    /* 0x06 */ u16 frq;
    /* 0x08 */ u32 loopOfsEnd;
    /* 0x0C */ u32 loopOfsStart;
    /* 0x10 */ MSM_STREAM stream[2];
} MSM_STREAM_PACK;

#endif
