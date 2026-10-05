#include "REL/sd00dll.h"
#include "game/audio.h"
#include "game/gamemes.h"
#include "game/hu3d.h"

typedef struct SdCamera_s {
    u32 bit;
    s32 focusType;
    f32 target[3];
    u8 pad14[0xC];
    f32 up[3];
    u8 pad2C[0xC];
    f32 pos[3];
    f32 fovy;
    f32 viewSpeed;
    u8 pad4C[0x3C];
} SdCamera;

typedef struct SdSys_s {
    u8 pad00[0x4C];
    SdCamera camera[2];
    u8 pad15C[0xE94];
    s32 unkFF0;
} SdSys;

extern SdSys *sys;

extern void SdFlagClose(void);
extern void SdRabbitClose(void);
extern void SdSeqUpdate(void);

extern void MathMtxRotSet(f32 *angles, f32 *mtx, s32 axis);
extern void (*_dtors[])(void);
extern void (*_ctors[])(void);
extern void ObjectSetup(void);

void SdCameraViewSpeedSet(s32 index, f32 speed)
{
    SdCamera *cam = &sys->camera[index];
    cam->viewSpeed = speed;
}

void SdSeqClose(void)
{
    HuAudAUXSet(0, 1);
    SdFlagClose();
    SdRabbitClose();
    GameMesClose();
    HuAudAllStop();
    Hu3DModelAllKill();
    Hu3DLightAllKill();
}

void SdSeqFadeOut(void)
{
    if (sys->unkFF0 != -1) {
        HuAudFXStop(sys->unkFF0);
        sys->unkFF0 = -1;
    }
    SdSeqUpdate();
}

void _epilog(void)
{
    void (**dtors)(void) = _dtors;
    while (*dtors) {
        ((void(*)(void))(*dtors))();
        dtors++;
    }
}

int _prolog(void)
{
    void (**ctors)(void) = _ctors;
    while (*ctors) {
        ((void(*)(void))(*ctors))();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

typedef struct SdMachine_s {
    u8 pad00[0xD4];
    f32 mtx[12];
    u8 pad104[0x40];
    u32 storedMtx[3];
    u8 pad150[0x258];
    u16 modelId;
} SdMachine;

// NONMATCHING: modelId load (lha vs lhz+extsh), copy uses r0 vs r3
void MachineRotAngleSet(SdMachine *machine, f32 *angles)
{
    MathMtxRotSet(angles, machine->mtx, 0);
    Hu3DModelRotSetV(machine->modelId, (HuVecF *)machine->mtx);
    machine->storedMtx[0] = ((u32 *)machine->mtx)[0];
    machine->storedMtx[1] = ((u32 *)machine->mtx)[1];
    machine->storedMtx[2] = ((u32 *)machine->mtx)[2];
}