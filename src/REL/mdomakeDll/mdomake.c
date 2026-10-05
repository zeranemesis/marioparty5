#include "game/hu3d.h"
#include "game/process.h"
#include "dolphin/types.h"

typedef struct MDMESS_WORK MDMESS_WORK;

typedef struct SYS {
    u8 pad[0x78];
    MDMESS_WORK *mess;
} SYS;

extern SYS *sys;
extern void MdMessMain(MDMESS_WORK *mess, int winNo);
extern void ObjectSetup(void);

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

void fn_1_1314(void)
{
    while (1) {
        MDMESS_WORK *mess = sys->mess;
        MdMessMain(mess, 0);
        HuPrcVSleep();
    }
    // NONMATCHING: redundant load sequence (original -O4,p vs build -O0,p)
}

void fn_1_1358(void)
{
    while (1) {
        MDMESS_WORK *mess = sys->mess;
        MdMessMain(mess, 1);
        HuPrcVSleep();
    }
    // NONMATCHING: redundant load sequence (original -O4,p vs build -O0,p)
}

typedef struct ParticleDot_s {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} ParticleDot;

void ParticleDotKill(ParticleDot *particle)
{
    Hu3DParManKill((s16)particle->unk0);
    Hu3DParManKill((s16)particle->unk4);
    Hu3DParManKill((s16)particle->unk8);
}

typedef struct ParticleGlow_s {
    u32 unk0;
} ParticleGlow;

void ParticleGlowKill(ParticleGlow *glow)
{
    Hu3DParManKill((s16)glow->unk0);
}