#include "dolphin/types.h"
typedef struct { u8 pad[0x184]; s16 modelId; } M508OBJ;
#include "game/process.h"

float fn_1_5D20(float *arg0, float *arg1)
{
    return arg0[0] * arg1[0] + arg0[1] * arg1[1];
}

float fn_1_5D40(float *arg0, float *arg1)
{
    return arg0[0] * arg1[1] - arg0[1] * arg1[0];
}

// Reset a task's state word to "not running".
void fn_1_1E58(void *arg0)
{
    *(s32 *)((u8 *)arg0 + 0x04) = 0;
}

// Same reset for the second task table.
void fn_1_22B4(void *arg0)
{
    *(s32 *)((u8 *)arg0 + 0x04) = 0;
}

extern s16 omSysExitReq;
void HuPrcVSleep(void);
void HuAudFadeOut(s32 speed);
s32 WipeCreate(s32 type, s32 mode, s32 frame);
s32 WipeCheckEnd(void);
void omOvlReturnEx(s32, s32);
void HuPrcEnd(void);

// Wait for the exit request, fade audio out, then run the wipe and wait for it.
void fn_1_538(void)
{
    while (omSysExitReq == 0) {
        HuPrcVSleep();
    }
    HuAudFadeOut(0x3E8);
    WipeCreate(2, 6, 0x3C);
    while ((u8)WipeCheckEnd()) {
        HuPrcVSleep();
    }
    omOvlReturnEx(1, 1);
    HuPrcEnd();
}

// Reset a third task table's state word.
void fn_1_1FD0(void *arg0)
{
    *(s32 *)((u8 *)arg0 + 0x04) = 0;
}

extern void *lbl_1_data_2C[2];
void *fn_1_7A3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, void *arg6);
void Hu3DModelAttrReset(s16 modelId, u32 attr);
void Hu3DModelAttrSet(s16 modelId, u32 attr);

// NONMATCHING: the target re-reads the model id into r30 for the second reset while// MWCC keeps it in r3; body, arguments and call order are otherwise exact.
// Build the entry-effector object: allocate it, arm its state, reset the model
// attributes twice (plain then per-slot), and return the object.
// Build the entry-effector object: allocate it, arm its state, reset the model
// attributes twice (plain then per-slot), and return the object.
void *fn_1_1F34(void)
{
    M508OBJ *obj = (M508OBJ *)fn_1_7A3C(0x1000, 0x2000, 0x42000C, 0, 0, 2, lbl_1_data_2C);
    void *slot = (u8 *)obj + 0x18;
    s16 modelId;

    *(s32 *)((u8 *)obj + 0x158) = 1;
    modelId = obj->modelId;
    Hu3DModelAttrReset(modelId, 1);
    Hu3DModelAttrReset(obj->modelId, 0x40000002);
    Hu3DModelAttrSet(obj->modelId, 0x40000001);
    return obj;
}

void fn_1_7A24(void)
{
    while (1) {
        HuPrcSleep(0);
    }
}

void fn_1_9EF8(void)
{
    while (1) {
        HuPrcVSleep();
    }
}

void fn_1_9F54(float *arg0, float *arg1, float *arg2)
{
    *arg1 = arg0[1];
    *arg2 = arg0[2];
}

float fn_1_A154(float *arg0, float *arg1)
{
    return arg0[0] * arg1[0] + arg0[1] * arg1[1];
}

// Arm an effector: set its state word then clear the model's hook attribute.
void fn_1_7F60(void *arg0)
{
    *(s32 *)((u8 *)arg0 + 0x158) = 1;
    Hu3DModelAttrReset(*(s16 *)((u8 *)arg0 + 0x184), 1);
}

void Hu3DModelHookSet(s16 modelId, s32 hookId, s16 hookModelId);

// Parent one effector model onto another.
void fn_1_8290(M508OBJ *arg0, s32 arg1, M508OBJ *arg2)
{
    Hu3DModelHookSet(arg0->modelId, arg1, arg2->modelId);
}

// Park an effector's motion at a given frame, promoting the frame count to float.
void fn_1_7FE0(M508OBJ *arg0, s32 arg1)
{
    Hu3DMotionTimeSet(arg0->modelId, (f32)arg1);
}