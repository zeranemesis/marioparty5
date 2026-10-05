#include "REL/systemDll.h"
#include "game/hu3d.h"

/* The overlay's global work block (0x290 bytes, .bss).  Every
 * flow helper in this unit reaches it through sysWork; the
 * sibling units (object / camera / ...) reference it extern. */
NEL_SYS sysWork;

/* Runtime work struct for a single "glow" particle effect.
 * ParticleGlowCreate mallocs 0xC bytes; the particle-manager
 * id landed in the first word, stored as a full int (the
 * create path sign-extends the HU3DPARMANID with extsh before
 * stw), so the field is int rather than HU3DPARMANID. */
typedef struct ParticleGlow_s {
    /* 0x00 */ int parManId;
} ParticleGlow;

/* Defined in a sibling unit of the same REL. */
void MdSpriteDebug(NEL_SYS *sys);

/* The overlay's object-manager bring-up, run once from _prolog. */
void ObjectSetup(void);

/* Overlay entry / teardown pair: the _ctors / _dtors tables are the
 * module-level constructor and destructor lists the linker emits. */
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
