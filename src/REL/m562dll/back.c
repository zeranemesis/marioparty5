// m562dll/back - the stage backdrop: holds one model+motion pair per board
// face plus a water-reflection copy pass.
#include "types.h"

typedef s16 HU3DMODELID;
typedef s16 HU3DMOTID;
typedef f32 Mtx[3][4];

void Hu3DModelKill(HU3DMODELID modelId);
void Hu3DMotionKill(HU3DMOTID motId);
void Hu3DModelAmbSet(HU3DMODELID id, f32 r, f32 g, f32 b);
void GXSetTexCopySrc(u16 left, u16 top, u16 wd, u16 ht);
void GXSetTexCopyDst(u16 wd, u16 ht, u32 fmt, u32 mipmap);
void GXSetCopyClear(void *color, u32 zw);
void GXCopyTex(void *dest, u8 clear);
BOOL omPauseChk(void);
s32 frand(void);

void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
void PSMTXRotRad(Mtx m, u8 axis, f32 rad);
void PSMTXConcat(Mtx a, Mtx b, Mtx out);
void PSMTXCopy(void *src, void *dst);
void Hu3DModelScaleSet(HU3DMODELID modelId, f32 x, f32 y, f32 z);
void Hu3DMotionTimeSet(HU3DMOTID motId, f32 time);
void Hu3DMotionMaxTimeGet(HU3DMOTID motId);
void Hu3DModelAttrReset(HU3DMODELID modelId, u32 attr);

void fn_1_3808(void *self);

extern void *Hu3DWaterData;
extern void *Hu3DData;
extern u16 Hu3DCameraBit;
extern u8 omUPauseFlag;
extern void *lbl_1_bss_0;

static const u8 kReflClear[4] = {0, 0, 0, 0};

/* The backdrop object: alternating model/motion id pairs from 0x0C up to 0x4A,
 * with an unhandled gap between 0x22 and 0x40. */
typedef struct back_s {
    u8 pad00[0x0C];
    /* 0x0C */ HU3DMODELID m0C;
    /* 0x0E */ HU3DMODELID m0E;
    /* 0x10 */ HU3DMODELID m10;
    /* 0x12 */ HU3DMODELID m12;
    /* 0x14 */ HU3DMODELID m14;
    /* 0x16 */ HU3DMOTID t16;
    /* 0x18 */ HU3DMOTID t18;
    /* 0x1A */ HU3DMODELID m1A;
    /* 0x1C */ HU3DMODELID m1C;
    /* 0x1E */ HU3DMOTID t1E;
    /* 0x20 */ HU3DMODELID m20;
    /* 0x22 */ HU3DMOTID t22;
    u8 pad24[0x40 - 0x24];
    /* 0x40 */ HU3DMODELID m40;
    /* 0x42 */ HU3DMOTID t42;
    /* 0x44 */ HU3DMODELID m44;
    /* 0x46 */ HU3DMODELID m46;
    /* 0x48 */ HU3DMOTID t48;
    /* 0x4A */ HU3DMOTID t4A;
    u8 pad4C[0x5C - 0x4C];
} M562BACK;

// NONMATCHING: the frame is 0xB0 with f30/f31 kept live across the PSMTXConcat
// chain and the model writes; MWCC gives 0x90 with _savegpr_25 and merges the
// two frand()/10000 int-to-float conversions. Body and call order are right.
// Steps one backdrop face forward: 0 arms a random delay, 1 ticks it down and
// then builds the spin (scale plus three rotations) and hands the matrix to the
// model, 2 holds until the motion completes and resets.
#pragma dont_inline on
void fn_1_4404(M562BACK *self, s32 modelId, s16 *timer, s16 *phase)
{
    Mtx tmp;
    Mtx rot;
    f32 r01;

    switch (*phase) {
    case 0:
        *timer = (s16)((frand() % self->m40) + self->m40 / 2);
        *phase = 1;
        break;
    case 1:
        *timer = *timer - 1;
        if ((s16)*timer <= 0) {
            break;
        }
        r01 = (f32)(frand() % 10000) / 10000.0f;
        PSMTXScale(tmp, 1.0f, 1.0f, 1.0f);
        PSMTXTrans(tmp, 0.0f, 0.0f, 0.0f);
        PSMTXRotRad(rot, 0x58, 0.7853982f);
        PSMTXConcat(rot, tmp, tmp);
        PSMTXRotRad(rot, 0x59, 0.017453292f * (360.0f * r01 - 180.0f));
        PSMTXConcat(rot, tmp, tmp);
        PSMTXRotRad(rot, 0x58, 0.6457718f);
        PSMTXConcat(rot, tmp, tmp);
        PSMTXCopy(tmp, (void *)((u8 *)Hu3DData + (s32)(s16)modelId * 0x134 + 0xF0));
        r01 = 0.5f + 0.5f * r01;
        Hu3DModelScaleSet((HU3DMODELID)modelId, r01 * (f32)self->m14, r01 * (f32)self->m14, r01 * (f32)self->m14);
        Hu3DMotionTimeSet((HU3DMOTID)modelId, 0.0f);
        Hu3DModelAttrReset((HU3DMODELID)modelId, 1);
        *timer = 0;
        *phase = 2;
        break;
    case 2:
        Hu3DMotionMaxTimeGet((HU3DMOTID)modelId);
        break;
    }
}
#pragma dont_inline reset

// Releases every model and motion the backdrop owns, then wipes the object.
void fn_1_41B4(M562BACK *self)
{
    Hu3DMotionKill(self->t4A);
    Hu3DModelKill(self->m46);
    Hu3DMotionKill(self->t48);
    Hu3DModelKill(self->m44);
    Hu3DMotionKill(self->t42);
    Hu3DModelKill(self->m40);
    Hu3DMotionKill(self->t22);
    Hu3DModelKill(self->m20);
    Hu3DMotionKill(self->t1E);
    Hu3DModelKill(self->m1C);
    Hu3DModelKill(self->m1A);
    Hu3DMotionKill(self->t18);
    Hu3DModelKill(self->m14);
    Hu3DMotionKill(self->t16);
    Hu3DModelKill(self->m12);
    Hu3DModelKill(self->m10);
    Hu3DModelKill(self->m0E);
    Hu3DModelKill(self->m0C);
    memset(self, 0, 0x5C);
}

// NONMATCHING: the original keeps &Hu3DWaterData in a stack local up front and
// copies the clear colour twice (rodata -> 0x10, then 0x10 -> 0x8) before
// passing the second copy to GXSetCopyClear; expressing that needs locals that
// MWCC immediately flattens back into a single copy.
// Water-reflection copy pass: re-renders the scene into the reflection buffer
// every frame unless the game is paused or the camera bit is off.
void fn_1_508C(void)
{
    u8 clear[4];

    clear[0] = kReflClear[0];
    clear[1] = kReflClear[1];
    clear[2] = kReflClear[2];
    clear[3] = kReflClear[3];
    if (*(void **)((u8 *)lbl_1_bss_0 + 0x2864) != 0) {
        if (*(u16 *)((u8 *)lbl_1_bss_0 + 0xE) != 4) {
            if (Hu3DCameraBit & 1) {
                GXSetTexCopySrc(0, 0, 0x280, 0x1E0);
                GXSetTexCopyDst(0x140, 0xF0, 4, 1);
                GXSetCopyClear(clear, 0x100 - 1);
                GXCopyTex(*(void **)((u8 *)lbl_1_bss_0 + 0x2864), 1);
                if (omPauseChk() + (u32)(u8)omUPauseFlag == 0) {
                }
            }
        }
    }
}