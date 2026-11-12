#include "REL/systemDll/sysmd.h"
#include "game/flag.h"
#include "game/memory.h"
#include "game/wipe.h"
#include "game/data.h"
#include "game/process.h"

typedef struct MdFlowEnt_s MDFLOWENT;
struct MdFlowEnt_s { void (*func)(NEL_SYS *sys, MDFLOWENT *ent); };
typedef struct MdProcEnt_s MDPROCENT;
struct MdProcEnt_s { s32 (*func)(NEL_SYS *sys, MDPROCENT *ent); };

/* The overlay's global work block (0x290 bytes, .bss). */
NEL_SYS sysWork;

/* Runtime work struct for a single "glow" particle effect. */
typedef struct ParticleGlow_s {
    /* 0x00 */ int parManId;
} ParticleGlow;

void MdSpriteDebug(NEL_SYS *sys);
void NelGuideBillboardUpdate(NEL_SYS *sys, s32 no);
void MdRingMain(NEL_SYS *sys);
void MdFukiMain(NEL_SYS *sys);
void MdBlendDebugMain(void);
void MdSpriteDebugMain(void);
void fn_1_19CCC(void);
void MdObjCameraCreate(OMOBJ *obj);
void MdObjLightCreate(OMOBJ *obj);
void MdObjPadMain(void);
void MdObjDebugFontCreate(OMOBJ *obj);
void MdMathStdCurveInit(void);

void ObjectSetup(void);
void MdInit(NEL_SYS *sys, MDFLOWENT *tbl);
void MdClose(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_1108(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_15EC(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_1BC8(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_2390(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_2C88(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_46F4(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_5B18(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_716C(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_7C88(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_8F18(NEL_SYS *sys, MDFLOWENT *tbl);
void fn_1_974C(NEL_SYS *sys, MDFLOWENT *tbl);
s32 fn_1_A4D4(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_A554(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_A61C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_A6FC(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_A898(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_A9D8(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_ADE8(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_AF84(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_B120(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_B534(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_B6D0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_B86C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_BC80(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_BDF4(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_BF68(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_C36C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_C4E0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_C654(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_CAA0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_CF8C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_D478(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_DF0C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_E130(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_E354(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_E7C0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_E9E4(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_EC08(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_F074(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_F120(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_F19C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_F378(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_F518(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_F614(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_F710(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_F924(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_FB38(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_FC50(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_FDE0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_FF08(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_100A8(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_10220(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_104C0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_10760(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_108A0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_10B30(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_10C70(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_11188(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_11874(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_1199C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_11B4C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_11C6C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_11F70(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_12268(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_124D8(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_12704(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_129D0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_12C94(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_12F74(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_13254(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_13558(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_13880(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_13AB0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_13DAC(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_13EBC(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_1417C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_1443C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_145C8(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_1472C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_147DC(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_1488C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_14A2C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_14BBC(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_14C80(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_14D44(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_14F24(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_150F4(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_151A4(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_15254(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_153F4(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_15584(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_15CE4(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_16444(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_1664C(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_167D0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_168C8(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_169C0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_16AB8(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_16BB0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_16CB0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_16DB0(NEL_SYS *sys, MDPROCENT *tbl);
s32 fn_1_17010(NEL_SYS *sys, MDPROCENT *tbl);
void MdMain(void);
void fn_1_638(void);
void LoadProc(void);

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

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

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}


MDFLOWENT MdFlowTbl[13] = {
    MdInit,
    MdClose,
    fn_1_1108,
    fn_1_15EC,
    fn_1_1BC8,
    fn_1_2390,
    fn_1_2C88,
    fn_1_46F4,
    fn_1_5B18,
    fn_1_716C,
    fn_1_7C88,
    fn_1_8F18,
    fn_1_974C,
};

MDPROCENT MdProcTbl[0x5B] = {
    fn_1_A4D4,
    fn_1_A554,
    fn_1_A61C,
    fn_1_A6FC,
    fn_1_A898,
    fn_1_A9D8,
    fn_1_ADE8,
    fn_1_AF84,
    fn_1_B120,
    fn_1_B534,
    fn_1_B6D0,
    fn_1_B86C,
    fn_1_BC80,
    fn_1_BDF4,
    fn_1_BF68,
    fn_1_C36C,
    fn_1_C4E0,
    fn_1_C654,
    fn_1_CAA0,
    fn_1_CF8C,
    fn_1_D478,
    fn_1_DF0C,
    fn_1_E130,
    fn_1_E354,
    fn_1_E7C0,
    fn_1_E9E4,
    fn_1_EC08,
    fn_1_F074,
    fn_1_F120,
    fn_1_F19C,
    fn_1_F378,
    fn_1_F518,
    fn_1_F614,
    fn_1_F710,
    fn_1_F924,
    fn_1_FB38,
    fn_1_FC50,
    fn_1_FDE0,
    fn_1_FF08,
    fn_1_100A8,
    fn_1_10220,
    fn_1_104C0,
    fn_1_10760,
    fn_1_108A0,
    fn_1_10B30,
    fn_1_10C70,
    fn_1_11188,
    fn_1_11874,
    fn_1_1199C,
    fn_1_11B4C,
    fn_1_11C6C,
    fn_1_11F70,
    fn_1_12268,
    fn_1_124D8,
    fn_1_12704,
    fn_1_129D0,
    fn_1_12C94,
    fn_1_12F74,
    fn_1_13254,
    fn_1_13558,
    fn_1_13880,
    fn_1_13AB0,
    fn_1_13DAC,
    fn_1_13EBC,
    fn_1_1417C,
    fn_1_1443C,
    fn_1_145C8,
    fn_1_1472C,
    fn_1_147DC,
    fn_1_1488C,
    fn_1_14A2C,
    fn_1_14BBC,
    fn_1_14C80,
    fn_1_14D44,
    fn_1_14F24,
    fn_1_150F4,
    fn_1_151A4,
    fn_1_15254,
    fn_1_153F4,
    fn_1_15584,
    fn_1_15CE4,
    fn_1_16444,
    fn_1_1664C,
    fn_1_167D0,
    fn_1_168C8,
    fn_1_169C0,
    fn_1_16AB8,
    fn_1_16BB0,
    fn_1_16CB0,
    fn_1_16DB0,
    fn_1_17010,
};

void ObjectSetup(void)
{
    NEL_SYS *sys = &sysWork;

    sys->unk234 = -1;
    sys->unk23D = 0;
    sys->stat = 0;
    sys->timer = 0;
    _ClearFlag(0x1000E);
    fn_1_19CCC();
    sys->objMan = omInitObjMan(0x200, 0x2000);
    sys->ovlHis = omOvlHisGet(0);
    sys->ovlEvtNo = ((OMOVLHIS *)sys->ovlHis)->evtno;
    omGameSysInit(sys->objMan);
    sys->fontObj = omAddObjEx(sys->objMan, 0x64, 0, 0, -1, MdObjDebugFontCreate);
    ((OMOBJ *)sys->fontObj)->data = HuMemDirectMallocNum(HEAP_HEAP, 0x1C, HU_MEMNUM_OVL);
    if (((OMOBJ *)sys->fontObj)->data == 0) {
        OSReport("sys->obj[NEL_OBJ_DEBUGFONT]->data = NULL...\n");
    }
    sys->fontData = (s32 *)((OMOBJ *)sys->fontObj)->data;
    *sys->fontData = 0;
    sys->padObj = omAddObjEx(sys->objMan, 0x65, 0, 0, -1, (OMOBJFUNC)MdObjPadMain);
    sys->cameraObj = omAddObjEx(sys->objMan, 0x66, 0, 0, -1, MdObjCameraCreate);
    ((OMOBJ *)sys->cameraObj)->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(SYSCAMERA_WORK), HU_MEMNUM_OVL);
    if (((OMOBJ *)sys->cameraObj)->data == 0) {
        OSReport("sys->obj[NEL_OBJ_CAMERA]->data = NULL...\n");
    }
    sys->camera = ((OMOBJ *)sys->cameraObj)->data;
    sys->camera->stat = 0;
    sys->lightObj = omAddObjEx(sys->objMan, 0x67, 0, 0, -1, MdObjLightCreate);
    ((OMOBJ *)sys->lightObj)->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(SYSLIGHT), HU_MEMNUM_OVL);
    if (((OMOBJ *)sys->lightObj)->data == 0) {
        OSReport("sys->obj[NEL_OBJ_LIGHT]->data = NULL...\n");
    }
    sys->light = ((OMOBJ *)sys->lightObj)->data;
    sys->light->stat = 0;
    sys->sprite = HuMemDirectMallocNum(HEAP_HEAP, sizeof(SYSSPRITE), HU_MEMNUM_OVL);
    if (sys->sprite == 0) {
        OSReport("sys->sprite = NULL...\n");
    }
    sys->model = HuMemDirectMallocNum(HEAP_HEAP, sizeof(SYSMODEL), HU_MEMNUM_OVL);
    if (sys->model == 0) {
        OSReport("sys->model = NULL...\n");
    }
    sys->texbuf = HuMemDirectMallocNum(HEAP_HEAP, sizeof(SYSTEXBUF), HU_MEMNUM_OVL);
    if (sys->texbuf == 0) {
        OSReport("sys->texbuf = NULL...\n");
    }
    sys->particle = HuMemDirectMallocNum(HEAP_HEAP, sizeof(SYSPARTICLE_WORK), HU_MEMNUM_OVL);
    if (sys->particle == 0) {
        OSReport("sys->particle = NULL...\n");
    }
    sys->ring = HuMemDirectMallocNum(HEAP_HEAP, sizeof(SYSRING_WORK), HU_MEMNUM_OVL);
    if (sys->ring == 0) {
        OSReport("sys->ring = NULL...\n");
    }
    sys->fuki = HuMemDirectMallocNum(HEAP_HEAP, 0x80, HU_MEMNUM_OVL);
    if (sys->fuki == 0) {
        OSReport("sys->fuki = NULL...\n");
    }
    sys->mess = HuMemDirectMallocNum(HEAP_HEAP, sizeof(SYSMESS), HU_MEMNUM_OVL);
    if (sys->mess == 0) {
        OSReport("sys->mess = NULL...\n");
    }
    sys->mess->stat = 0;
    MdMathStdCurveInit();
    sys->messProc = HuPrcChildCreate(MdMessProc, 0x6A, 0x3000, 0, sys->objMan);
    Hu3DBGColorSet(0, 0, 0);
    Hu3DNoSyncSet(0);
    while (1) {
        if (*sys->fontData == 1 && sys->camera->stat == 1 && sys->light->stat == 1 && sys->mess->stat == 1) {
            break;
        }
        HuPrcVSleep();
    }
    sys->mainProc = HuPrcChildCreate(MdMain, 0x68, 0x3000, 0, sys->objMan);
    sys->unk234 = 0;
}

void MdMain(void)
{
    NEL_SYS *sys = &sysWork;

    sys->timer = 0;
    sys->frameCnt = 0;
    sys->stat = 0;
    sys->flowNo = 2;
    sys->exitReqF = 0;
    sys->unk23F = 1;
    sys->unk240 = 0;
    sys->unk288 = 0;
    sys->ovlMode = 0;
    sys->ovlNo = 0x70;
    if (sys->ovlEvtNo == 1) {
        omOvlReturnEx(1, 1);
        HuPrcEnd();
        while (1) {
            HuPrcVSleep();
        }
    }
    MdProcTbl->func(sys, MdProcTbl);
    while (omSysExitReq == 0 && sys->exitReqF == 0) {
        (MdFlowTbl + sys->flowNo)->func(sys, MdFlowTbl + sys->flowNo);
        MdRingMain(sys);
        MdFukiMain(sys);
        NelGuideBillboardUpdate(sys, 0x3B);
        NelGuideBillboardUpdate(sys, sys->menuNo2 + 0x3C);
        NelGuideBillboardUpdate(sys, 0x41);
        NelGuideBillboardUpdate(sys, 0x32);
        NelGuideBillboardUpdate(sys, 0x33);
        sys->frameCnt++;
        HuPrcVSleep();
    }
    MdProcTbl[1].func(sys, MdProcTbl);
}

void fn_1_638(void)
{
    NEL_SYS *sys = &sysWork;
    NEL_BLENDDBG *dbg = &sys->blendDbg;

    sys->timer = 0;
    sys->stat = 0;
    MdProcTbl->func(sys, MdProcTbl);
    WipeCreate(1, 0, 0x3C);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
    while (omSysExitReq == 0) {
        MdBlendDebugMain();
        MdSpriteDebugMain();
        HuPrcVSleep();
    }
    MdProcTbl[1].func(sys, MdProcTbl);
}

void LoadProc(void)
{
    NEL_SYS *sys = &sysWork;
    s32 tick;

    while (1) {
        switch (sys->loadStat) {
        case 0:
            break;
        case 1:
            tick = OSGetTick();
            sys->asyncStat = HuDataDirReadAsync(sys->dataDirNo);
            while (HuDataGetAsyncStat(sys->asyncStat) == 0) {
                HuPrcVSleep();
            }
            OSReport("Load Time:%d\n", OSTicksToMilliseconds((u32)(OSGetTick() - tick)));
            OSReport("Finished!\n");
            sys->loadStat = 2;
            break;
        case 2:
            HuPrcEnd();
            break;
        }
        HuPrcVSleep();
    }
}

/* fn_1_18BA0 - copy the per-slot "selected model B" table over
 * the "selected model C2" table (both 4 entries). */
void fn_1_18BA0(NEL_SYS *sys)
{
    s32 i;
    for (i = 0; i < 4; i++) {
        sys->mdlSelNoC2[i] = sys->mdlSelNoB[i];
    }
}

/* fn_1_198E0 - reached from the model-init flow with a model id
 * and a 90-degree angle.  The original resolves the shared model
 * block into a local and returns void, so the store is the only
 * observable effect; reproduced as written rather than as a return. */
void fn_1_198E0(s32 modelId, float angle)
{
    NEL_SYS *sys = &sysWork;
    MD_MODEL *model = sys->model;
}

/* fn_1_19C00 - fuki hook: mark slot idx as "mode 2". */
void fn_1_19C00(NEL_SYS *sys, s32 idx)
{
    sys->fukiMode[idx] = 2;
}

/* fn_1_19C70 - fuki hook: step the slot's mode down (2 -> 0,
 * 3 -> 1), leaving anything else alone. */
void fn_1_19C70(NEL_SYS *sys, s32 idx)
{
    if (sys->fukiMode[idx] == 2) {
        sys->fukiMode[idx] = 0;
    } else if (sys->fukiMode[idx] == 3) {
        sys->fukiMode[idx] = 1;
    }
}

/* fn_1_19CB8 - hand back the shared model block for this sys. */
MD_MODEL *fn_1_19CB8(NEL_SYS *sys)
{
    MD_MODEL *model = sys->model;
    return model;
}

/* MdSpriteDebugMain - drop into the sprite debugger when the
 * blend-debug state machine is parked on its "show" state. */
void MdSpriteDebugMain(void)
{
    if (sysWork.blendDbg.stat == 2) {
        MdSpriteDebug(&sysWork);
    }
}

/* fn_1_1A404 - read model slot 0x20's id out of the shared model
 * block.  As in the original the id lands in a local and the
 * function returns void. */
void fn_1_1A404(void)
{
    NEL_SYS *sys = &sysWork;
    MD_MODEL *model = sys->model;
    s16 mdlId = model->mdlId[0x20];
}

/* ParticleGlowKill - tear a glow particle effect down by its
 * particle-manager handle. */
void ParticleGlowKill(ParticleGlow *p)
{
    Hu3DParManKill((s16)p->parManId);
}

/* ParticleGlowPosSet - move a glow to `pos`, then restart it with a
 * two-frame life limit and attribute bit 1. */
void ParticleGlowPosSet(ParticleGlow *p, Vec *pos)
{
    Hu3DParManPosSet((s16)p->parManId, pos->x, pos->y, pos->z);
    Hu3DParManTimeLimitSet((s16)p->parManId, 2);
    Hu3DParManAttrReset((s16)p->parManId, 1);
}
