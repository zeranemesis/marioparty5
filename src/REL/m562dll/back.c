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
double sin(double x);
void *memset(void *dst, s32 val, u32 len);

void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
void PSMTXRotRad(Mtx m, u8 axis, f32 rad);
void PSMTXConcat(Mtx a, Mtx b, Mtx out);
void PSMTXCopy(void *src, void *dst);
void Hu3DModelScaleSet(HU3DMODELID modelId, f32 x, f32 y, f32 z);
f32 Hu3DMotionTimeGet(HU3DMOTID motId);
f32 Hu3DMotionMaxTimeGet(HU3DMOTID motId);
void Hu3DMotionTimeSet(HU3DMOTID motId, f32 time);
void Hu3DModelAttrReset(HU3DMODELID modelId, u32 attr);
void Hu3DModelAttrSet(HU3DMODELID modelId, u32 attr);
void Hu3DModelPosSet(HU3DMODELID modelId, f32 posX, f32 posY, f32 posZ);

extern u8 Hu3DData[];
extern u8 Hu3DWaterData[];
extern s16 Hu3DCameraBit;
extern u8 omUPauseFlag;
extern void *lbl_1_bss_0;

/* this unit's .rodata, in binary order (module-relative offsets) */
static const f32 lbl_1_rodata_D4 = 0.0f;
static const f32 lbl_1_rodata_F8 = 400.0f;
static const f64 lbl_1_rodata_100 = 30.0;
static const f64 lbl_1_rodata_108 = 3.141592653589793;
static const f32 lbl_1_rodata_110 = 360.0f;
static const f64 lbl_1_rodata_118 = 180.0;
static const f64 lbl_1_rodata_120 = 4503601774854144.0;
static const f32 lbl_1_rodata_128 = 10000.0f;
static const f32 lbl_1_rodata_12C = 1.0f;
static const f32 lbl_1_rodata_130 = 0.7853982f;
static const f32 lbl_1_rodata_134 = 0.017453292f;
static const f32 lbl_1_rodata_138 = 180.0f;
static const f32 lbl_1_rodata_13C = 0.6457718f;
static const f32 lbl_1_rodata_140 = 0.5f;
static const f64 lbl_1_rodata_148 = 4503599627370496.0;
static const u8 lbl_1_rodata_16C = 0;

/* The backdrop object: alternating model/motion id pairs from 0x0C up to 0x4A,
 * with an unhandled gap between 0x22 and 0x40. */
typedef struct back_s {
    /* 0x00 */ void *f00;
    u8 pad04[0x0C - 0x04];
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
    u8 pad24[0x2C - 0x24];
    /* 0x2C */ s32 cnt;
    /* 0x30 */ s32 f30;
    /* 0x34 */ f32 f34;
    /* 0x38 */ f32 f38;
    /* 0x3C */ f32 f3C;
    /* 0x40 */ HU3DMODELID m40;
    /* 0x42 */ HU3DMOTID t42;
    /* 0x44 */ HU3DMODELID m44;
    /* 0x46 */ HU3DMODELID m46;
    /* 0x48 */ HU3DMOTID t48;
    /* 0x4A */ HU3DMOTID t4A;
    /* 0x4C */ s16 p4C;
    /* 0x4E */ s16 p4E;
    /* 0x50 */ s16 t50;
    /* 0x52 */ s16 t52;
    /* 0x54 */ f32 f54;
    /* 0x58 */ s16 m58;
    u8 pad5A[0x5C - 0x5A];
} M562BACK;

/* the object hung off back->f00: only the word at 0x0E is inspected here */
typedef struct backpeer_s {
    u8 pad00[0x0E];
    u16 field0E;
} M562PEER;

/* three contiguous floats: HuVecF is 8-byte aligned, which would inflate
 * every struct that carries one of these */
typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
} M562VEC3;

void fn_1_4404(M562BACK *self, s16 modelId, s16 *timer, s16 *phase);

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

#pragma dont_inline on
// Steps one backdrop face forward: 0 arms a random delay, 1 ticks it down and
// then builds the spin (scale plus three rotations) and hands the matrix to the
// model, 2 holds until the motion completes and resets.
void fn_1_4404(M562BACK *self, s16 modelId, s16 *timer, s16 *phase)
{
    Mtx rot;
    Mtx tmp;
    f32 r01;
    f32 rmax;

    switch (*timer) {
    case 0:
        *phase = (s16)((frand() % (u32)self->m58) + self->m58 / 2);
        *timer = 1;
        break;
    case 1:
        *phase = (s16)(*phase - 1);
        if (*phase > 0) {
            break;
        }
        r01 = (f32)(frand() % (u32)10000) / lbl_1_rodata_128;
        PSMTXScale(tmp, lbl_1_rodata_12C, lbl_1_rodata_12C, lbl_1_rodata_12C);
        PSMTXTrans(tmp, lbl_1_rodata_D4, lbl_1_rodata_D4, lbl_1_rodata_D4);
        PSMTXRotRad(rot, 0x58, lbl_1_rodata_130);
        PSMTXConcat(rot, tmp, tmp);
        PSMTXRotRad(rot, 0x59, lbl_1_rodata_134 * (lbl_1_rodata_110 * r01 - lbl_1_rodata_138));
        PSMTXConcat(rot, tmp, tmp);
        PSMTXRotRad(rot, 0x58, lbl_1_rodata_13C);
        PSMTXConcat(rot, tmp, tmp);
        PSMTXCopy(tmp, (void *)(Hu3DData + modelId * 0x134 + 0xF0));
        r01 = (f32)(frand() % (u32)10000) / lbl_1_rodata_128;
        r01 = lbl_1_rodata_140 + lbl_1_rodata_140 * r01;
        Hu3DModelScaleSet(modelId, r01 * self->f54, r01 * self->f54, r01 * self->f54);
        Hu3DMotionTimeSet(modelId, lbl_1_rodata_D4);
        Hu3DModelAttrReset(modelId, 1);
        *phase = 0;
        *timer = 2;
        break;
    case 2:
        rmax = Hu3DMotionMaxTimeGet(modelId);
        if (Hu3DMotionTimeGet(modelId) >= rmax) {
            Hu3DModelAttrSet(modelId, 1);
            *phase = 0;
            *timer = 0;
        }
        break;
    }
}

// Two backdrop faces advance, then all three get bobbed by a shared sine of
// the frame counter.
void fn_1_427C(M562BACK *self)
{
    M562PEER *peer;
    M562VEC3 pos;
    f32 fv;

    peer = (M562PEER *)self->f00;
    fn_1_4404(self, self->m44, &self->p4C, &self->t50);
    fn_1_4404(self, self->m46, &self->p4E, &self->t52);

    if (peer->field0E != 4) {
        fv = (f32)(self->cnt % 400) / lbl_1_rodata_F8;
        pos.x = self->f34;
        pos.y = (f32)(sin(lbl_1_rodata_108 * (lbl_1_rodata_110 * fv) / lbl_1_rodata_118) * lbl_1_rodata_100 + self->f38);
        pos.z = self->f3C;
        Hu3DModelPosSet(self->m1A, pos.x, pos.y, pos.z);
        Hu3DModelPosSet(self->m1C, pos.x, pos.y, pos.z);
        Hu3DModelPosSet(self->m20, pos.x, pos.y, pos.z);
        if (self->f30 != 0) {
            self->cnt++;
        }
    }
}
#pragma dont_inline reset

// Water-reflection copy pass: re-renders the scene into the reflection buffer
// every frame unless the game is paused or the camera bit is off.
void fn_1_508C(void)
{
    u8 *water;
    u8 clearA[4];
    u8 clearB[4];
    s32 pause;

    water = Hu3DWaterData;
    clearA[0] = ((const u8 *)&lbl_1_rodata_16C)[0];
    clearA[1] = ((const u8 *)&lbl_1_rodata_16C)[1];
    clearA[2] = ((const u8 *)&lbl_1_rodata_16C)[2];
    clearA[3] = ((const u8 *)&lbl_1_rodata_16C)[3];
    if (*(void **)((u8 *)*(void **)lbl_1_bss_0 + 0x2864) != 0) {
        if (*(u16 *)((u8 *)*(void **)lbl_1_bss_0 + 0xE) != 4) {
            if (Hu3DCameraBit & 1) {
                GXSetTexCopySrc(0, 0, 0x280, 0x1E0);
                GXSetTexCopyDst(0x140, 0xF0, 4, 1);
                clearB[0] = clearA[0];
                clearB[1] = clearA[1];
                clearB[2] = clearA[2];
                clearB[3] = clearA[3];
                GXSetCopyClear(clearB, 0x100 - 1);
                GXCopyTex(*(void **)((u8 *)*(void **)lbl_1_bss_0 + 0x2864), 1);
                pause = omPauseChk() + omUPauseFlag;
                if (pause == 0) {
                }
            }
        }
    }
}