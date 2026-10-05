// rescardDll/rescard - decompilation of the rescardDll/rescard unit (43232 B, 44 fns).
// Only _prolog, _epilog and the ParticleDotKill / ParticleGlowPosSet /
// ParticleGlowKill helpers have source so far; the rest of the unit is still
// declared-only. Declaration order follows the .text order in the original.

#include "types.h"
#include "game/hu3d.h"

/* Particle helper block, shared by ParticleDotCreate and ParticleGlowCreate:
 * both HuMemDirectMallocNum a 0xC-byte record and store HU3DPARMANIDs in it.
 * The ids are held as s32 but every use goes through the s16 HU3DPARMANID
 * parameter, which is where the extsh after each lwz comes from. */
typedef struct RescardEffect_s {
    int parManId[3];
} RESCARD_EFFECT;

typedef void (*VoidFunc)(void);

extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* The module's shared state block (sysWork in the original's .bss). Only the
 * two offsets the recovered functions touch are named; everything else is an
 * opaque pad sized so the two land on the offsets the disassembly shows. */
typedef struct RescardSys_s {
u8 pad00[0x74];
    s8 *cursorTbl; //0x74
    u8 pad78[0x140]; //0x78
    s32 resetA;     //0x1B8
    s32 resetB;     //0x1BC
    u8 padC0[8];    //0x1C0
    f32 resetF[4];  //0x1C8
} RESCARD_SYS;

extern RESCARD_SYS *sys;
extern RESCARD_SYS *sysP;

/* .rodata 0x14 - plain (non-const) f32 so it keeps the named relocation. */
f32 lbl_1_rodata_14 = 0.0f;

/* the rest of the unit is not recovered yet; it is declared here, in .text
 * order, so the calls below resolve. ObjectSetup (0xA0) has to come before
 * _prolog uses it. */
extern void ObjectSetup(void);
extern void MdMain(void);
extern void LoadProc(void);
extern void MdInit(void);
extern void MdClose(void);
extern void ResCardFlowInit(void);
extern void ResCardFlowScore(void);
extern void ResCardFlowDraw(void);
extern void ResCardFlowWin(void);
extern void ResCardProcDarken(void);
extern void ResCardProcPlayerFocus(void);
extern void ResCardProcStageFocus(void);
extern void ResCardScoreCreate(void);
extern void ResCardScoreUpdate(void);
extern void ResCardDrawStarCreate(void);
extern void ResCardDrawUpdate(void);
extern void ResCardDrawStkDirGet(void);
extern void ResCardDrawStarUpdate(void);
extern void ResCardDrawCursorModeSet(void);
extern void ResCardDrawTimerPosSet(void);
extern void ResCardDrawCursorMove(void);
extern void ResCardSpotCreate(void);
extern void ResCardSpotScaleCalc(void);
extern void ResCardSpotAngleUpdate(void);
extern void ResCardSpotUpdate(void);
extern void ResCardStarUpdate(void);
extern void ResCardPinUpdate(void);
extern void ResCardPinMotStart(void);
extern void ResCardBillboardUpdate(void);
extern void ResCardAudExec(void);
extern void ResCardFXPanGet(void);
extern void ResCardBGMReset(void);
extern void ResCardWinVoicePlay(void);
extern void MdPosAdjustMode(void);
extern void ParticleDotCreate(ANIMDATA *animP, s16 layer, s16 cameraBit);
extern void ParticleDotPosSet(RESCARD_EFFECT *effectP, HuVecF *pos);

/* 0x0 - module init: walk the static-constructor table, then bring the
 * overlay's objects up. */
int _prolog(void)
{
    const VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

/* 0x54 - module fini: walk the static-destructor table. */
void _epilog(void)
{
    const VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

/* Not yet recovered; declared so the prolog's call site resolves. */

/* 0x2018 - re-arm the flow procs: clear the two proc counters and zero the
 * four cached floats through the shared rodata zero. */
s32 ResCardProcReset(void)
{
    sys->resetA = 0;
    sys->resetB = 0;
    sys->resetF[0] = lbl_1_rodata_14;
    sys->resetF[1] = lbl_1_rodata_14;
    sys->resetF[2] = lbl_1_rodata_14;
    sys->resetF[3] = lbl_1_rodata_14;
    return 0;
}

/* 0x50EC - pick the next free cursor slot: start after the stored selection
 * (player 1 keeps its own start point), wrap around the four slots and take
 * the first one whose flag byte at cursorTbl + 0xED is set. */
void ResCardDrawCursorReset(s32 playerNo)
{
    s32 sel = 3;
    s32 *cur;
    s32 i;
    s8 *tbl;

    tbl = sys->cursorTbl;
    cur = (s32 *)(tbl + 0x60);
    if (playerNo == 1) {
        sel = cur[3];
    }
    for (i = 0; i < 3; i++) {
        sel = (sel + 1) % 4;
        if (tbl[0xED + sel] == 1) {
            cur[3] = sel;
            break;
        }
    }
}

/* 0xA718 - kill the three particles of a "dot" effect record. */
static void ParticleDotKill(RESCARD_EFFECT *effectP)
{
    Hu3DParManKill(effectP->parManId[0]);
    Hu3DParManKill(effectP->parManId[1]);
    Hu3DParManKill(effectP->parManId[2]);
}

/* 0xA764 - not yet recovered. */
static RESCARD_EFFECT *ParticleGlowCreate(ANIMDATA *animP, s16 layer, s16 cameraBit);

/* 0xA840 - move a "glow" effect's single particle and restart its lifetime. */
static void ParticleGlowPosSet(RESCARD_EFFECT *effectP, HuVecF *pos)
{
    Hu3DParManPosSet(effectP->parManId[0], pos->x, pos->y, pos->z);
    Hu3DParManTimeLimitSet(effectP->parManId[0], 2);
    Hu3DParManAttrReset(effectP->parManId[0], HU3D_PARMAN_ATTR_TIMEUP);
}

/* 0xA8AC - kill a "glow" effect's single particle. */
static void ParticleGlowKill(RESCARD_EFFECT *effectP)
{
    Hu3DParManKill(effectP->parManId[0]);
}