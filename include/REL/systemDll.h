#ifndef _REL_SYSTEMDLL_H
#define _REL_SYSTEMDLL_H

#include "types.h"
#include "humath.h"

/* Shared declarations for the systemDll overlay (nel / object / camera /
 * stage / math / window).  Only the entry points nel.c calls are declared
 * here; the implementations live in the sibling units. */

typedef struct NEL_SYS NEL_SYS;
typedef struct MD_SPRITE MD_SPRITE;

/* The model's 0x470-byte work block.  0x000 is a flat table of model ids
 * (touei / character / ring / capsule / star ...).  Three parallel 0x4D-entry
 * tables of 3D-position pointers follow at 0x0D4, 0x208 and 0x33C; all three
 * are indexed by the same model number, and 0x470 is the whole block. */
struct MD_MODEL {
    /* 0x000 */ s16 mdlId[0x6A];
    /* 0x0D4 */ Vec *mdlPos[0x4D];
    /* 0x208 */ Vec *mdlPosB[0x4D];
    /* 0x33C */ Vec *mdlPosC[0x4D];
};
typedef struct MD_MODEL MD_MODEL;

/* The sprite work block.  The group ids sit at 0xDE. */
struct MD_SPRITE {
    /* 0x000 */ s8 pad00[0xDE];
    /* 0x0DE */ s16 grpId[6];
    /* 0x0EA */ s8 pad0EA[0xA];
};
typedef struct MD_SPRITE MD_SPRITE;

/* Per-camera block: 0x84 stride, `cameraBit` at 0x04 and a 4-float `pos` at
 * 0x88 (the billboard code reads offsets 0x4 / 0x8 / 0xC of that Vec). */
typedef struct MD_CAMERA_s {
    /* 0x000 */ s32 stat;
    /* 0x004 */ s16 cameraBit;
    /* 0x006 */ s16 pad06;
    /* 0x008 */ s8 pad08[0x80];
    /* 0x088 */ float pos[4];
} MD_CAMERA;

typedef struct MD_LIGHT_s {
    s32 stat;
    s16 lightBit;
    s16 pad06;
} MD_LIGHT;

typedef struct MD_TEXBUF_s {
    s32 stat;
} MD_TEXBUF;

typedef struct MD_MESS_s {
    /* 0x00 */ s32 stat;
    /* 0x04 */ s32 winStat;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x0E */ s16 choice;
} MD_MESS;

typedef struct MD_FUKI_s {
    s32 stat;
} MD_FUKI;

typedef struct MD_PARTICLE_s {
    s32 stat;
} MD_PARTICLE;

typedef struct MD_RING_s {
    s32 stat;
} MD_RING;

/* Blend-colour debug work, 0x80 bytes at sysWork + 0x1A4. */
typedef struct NEL_BLENDDBG_s {
    /* 0x00 */ s32 stat;
    /* 0x04 */ s32 dispTimer;
    /* 0x08 */ u8 col[4];
    /* 0x0C */ s32 type;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 logic;
    /* 0x1C */ s8 pad1C[0x50];
    /* 0x6C */ u8 tpCol[4];
    /* 0x70 */ s32 tpSel;
    /* 0x74 */ s32 tp0;
    /* 0x78 */ s32 tp1;
    /* 0x7C */ s32 tp2;
} NEL_BLENDDBG;


/* Flow / process state-machine entry points.  The second argument is the
 * address of the table entry the call was made through. */
typedef s32 (*NEL_PROCFUNC)(NEL_SYS *sys, void *tbl);
typedef void (*NEL_FLOWFUNC)(NEL_SYS *sys, void *tbl);

/* The overlay's global work structure, 0x290 bytes.  Field names follow the
 * roles recovered from the disassembly; `unk` fields keep the exact offsets
 * recovered from the store/load widths. */
struct NEL_SYS {
    /* 0x000 */ s32 stat;         /* flow sub-state */
    /* 0x004 */ s32 timer;
    /* 0x008 */ s32 frameCnt;
    /* 0x00C */ float angle;
    /* 0x010 */ float angSpd;
    /* 0x014 */ s32 initF1;
    /* 0x018 */ s32 initF2;
    /* 0x01C */ s32 initF3;
    /* 0x020 */ s32 sel;
    /* 0x024 */ s32 selSub;
    /* 0x028 */ float oscSpd;
    /* 0x02C */ float oscTime1;   /* MdMathOscillate type 1/2 in */
    /* 0x030 */ float oscTime1b;
    /* 0x034 */ float oscTime1c;
    /* 0x038 */ float oscTime2;   /* MdMathOscillate type 3 in */
    /* 0x03C */ float oscTime2b;
    /* 0x040 */ float oscVal1;    /* MdMathOscillate type 1/2 out */
    /* 0x044 */ float oscVal1b;
    /* 0x048 */ float oscVal1c;
    /* 0x04C */ float oscVal2;    /* MdMathOscillate type 3 out */
    /* 0x050 */ float oscVal2b;
    /* 0x054 */ float pos0x;
    /* 0x058 */ float pos0y;
    /* 0x05C */ float pos1x;
    /* 0x060 */ float pos1y;
    /* 0x064 */ float pos2x;
    /* 0x068 */ float pos2y;
    /* 0x06C */ float pos3x;
    /* 0x070 */ float pos3y;
    /* 0x074 */ float pos4x;
    /* 0x078 */ float pos4y;
    /* 0x07C */ float pos5x;
    /* 0x080 */ float pos5y;
    /* 0x084 */ float sclPos0x;
    /* 0x088 */ float sclPos0y;
    /* 0x08C */ float sclPos1x;
    /* 0x090 */ float sclPos1y;
    /* 0x094 */ float sclPos2x;
    /* 0x098 */ float sclPos2y;
    /* 0x09C */ float sclPos3x;
    /* 0x0A0 */ float sclPos3y;
    /* 0x0A4 */ float sclVal0;
    /* 0x0A8 */ float sclVal1;
    /* 0x0AC */ float sclVal2;
    /* 0x0B0 */ float sclVal3;
    /* 0x0B4 */ float sclVal4;
    /* 0x0B8 */ float sclVal5;
    /* 0x0BC */ float sclVal6;
    /* 0x0C0 */ float sclVal7;
    /* 0x0C4 */ float sclVal8;
    /* 0x0C8 */ float sclVal9;
    /* 0x0CC */ float sclVal10;
    /* 0x0D0 */ float sclVal11;
    /* 0x0D4 */ float sclVal12;
    /* 0x0D8 */ float sclVal13;
    /* 0x0DC */ float sclVal14;
    /* 0x0E0 */ float sclVal15;
    /* 0x0E4 */ float sclVal16;
    /* 0x0E8 */ float sclVal17;
    /* 0x0EC */ float sclVal18;
    /* 0x0F0 */ float sclVal19;
    /* 0x0F4 */ float sclVal20;
    /* 0x0F8 */ float sclVal21;
    /* 0x0FC */ float sclVal22;
    /* 0x100 */ float sclVal23;
    /* 0x104 */ float sclVal24;
    /* 0x108 */ float sclVal25;
    /* 0x10C */ float sclVal26;
    /* 0x110 */ float sclVal27;
    /* 0x114 */ float sclVal28;
    /* 0x118 */ float sclVal29;
    /* 0x11C */ float sclVal30;
    /* 0x120 */ float sclVal31;
    /* 0x124 */ float sclVal32;
    /* 0x128 */ float sclVal33;
    /* 0x12C */ float sclVal34;
    /* 0x130 */ float sclVal35;
    /* 0x134 */ float sclVal36;
    /* 0x138 */ float sclVal37;
    /* 0x13C */ float unk13C;
    /* 0x140 */ s32 padStat;
    /* 0x144 */ s32 unk144;
    /* 0x148 */ s32 unk148;
    /* 0x14C */ s32 unk14C;
    /* 0x150 */ void *objMan;      /* OMOBJMAN * */
    /* 0x154 */ void *mainProc;    /* HUPROCESS * */
    /* 0x158 */ void *unk158;
    /* 0x15C */ void *messProc;    /* HUPROCESS * */
    /* 0x160 */ void *unk160;
    /* 0x164 */ void *fontObj;     /* OMOBJ * */
    /* 0x168 */ void *padObj;      /* OMOBJ * */
    /* 0x16C */ void *cameraObj;   /* OMOBJ * */
    /* 0x170 */ void *lightObj;    /* OMOBJ * */
    /* 0x174 */ void *ovlHis;      /* OMOVLHIS * */
    /* 0x178 */ s32 ovlEvtNo;
    /* 0x17C */ s32 *fontData;
    /* 0x180 */ MD_CAMERA *camera;
    /* 0x184 */ MD_LIGHT *light;
    /* 0x188 */ MD_SPRITE *sprite;
    /* 0x18C */ MD_MODEL *model;
    /* 0x190 */ MD_TEXBUF *texbuf;
    /* 0x194 */ MD_MESS *mess;
    /* 0x198 */ MD_PARTICLE *particle;
    /* 0x19C */ MD_RING *ring;
    /* 0x1A0 */ MD_FUKI *fuki;
    /* 0x1A4 */ NEL_BLENDDBG blendDbg;
    /* 0x224 */ s32 ovlMode;       /* 0 = goto, 1 = call, 2 = return */
    /* 0x228 */ s32 ovlNo;
    /* 0x22C */ s16 loadStat;
    /* 0x22E */ s16 asyncStat;
    /* 0x230 */ s32 dataDirNo;
    /* 0x234 */ s32 unk234;
    /* 0x238 */ s32 flowNo;
    /* 0x23C */ s8 exitReqF;
    /* 0x23D */ s8 unk23D;
    /* 0x23E */ s8 modeSel;
    /* 0x23F */ s8 unk23F;
    /* 0x240 */ s8 unk240;
    /* 0x241 */ s8 playerNum;
    /* 0x242 */ s8 unk242;
    /* 0x243 */ s8 pad243[4];   /* per-pad "this pad is active" flag */
    /* 0x247 */ s8 unk247[4];   /* pad index -> model slot, -1 when unused */
    /* 0x24B */ s8 mdlNo[4];
    /* 0x24F */ s8 fukiMode[4];
    /* 0x253 */ s8 mdlSelNo[8];
    /* 0x25B */ s8 ringMdlNo[3];
    /* 0x25E */ s8 selNo;
    /* 0x25F */ s8 selSubNo;
    /* 0x260 */ s8 ringMdlGrp[4];
    /* 0x264 */ s8 mdlSelNoB[4];
    /* 0x268 */ s8 mdlSelNoC2[4];
    /* 0x26C */ s8 winMes0;
    /* 0x26D */ s8 winMes1;
    /* 0x26E */ s8 mdlSelNoC[3];
    /* 0x271 */ s8 selNo3;
    /* 0x272 */ s8 selPrevNo;
    /* 0x273 */ s8 pad273;
    /* 0x274 */ float unk274;
    /* 0x278 */ s8 pad278[4];
    /* 0x27C */ float unk27C;
    /* 0x280 */ s8 menuNo;
    /* 0x281 */ s8 menuNo2;
    /* 0x282 */ s8 menuNo3;
    /* 0x283 */ s8 unk283[4];
    /* 0x287 */ s8 unk287;
    /* 0x288 */ s8 unk288;
    /* 0x289 */ s8 unk289[7];
};

typedef struct NEL_MDLINFO_s {
    const char *name;
    s32 group;
} NEL_MDLINFO;

#endif
