// m562dll/light ' the scene's two lights (ambient fill + a spot) and the
// per-frame ambient broadcast.
#include "types.h"

typedef s16 HU3DLIGHTID;
typedef s16 HU3DLLIGHTID;

#include "game/hu3d.h"

void Hu3DAmbColorSet(f32 r, f32 g, f32 b);

/* Three floats copied as a unit ' the target moves them word by word. */
typedef struct lightVec_s {
    f32 x;
    f32 y;
    f32 z;
} lightVec;

/* The stage object the two lights belong to. Only the two offsets light.c
   reaches for are named here; the rest is opaque. */
typedef struct LightStage_s {
    u8 pad[0x2840];
} LightStage;

/* 0x18 bytes: an owner word, the ambient light id, the spot light id. */
typedef struct light_s {
    /* 0x00 */ LightStage *owner;
    /* 0x04 */ HU3DLIGHTID ambientId;
    /* 0x06 */ HU3DLIGHTID spotId;
    /* 0x08 */ s32 spotIsLocal;
} LIGHT;

void fn_1_23CB8(LightStage *self, const lightVec *col);

/* Blend helper: A at t, B at 1-t, one component a time, exactly the order
   the original evaluates them. */
#define LERP3(dst, A, B)                            \
    (dst).x = (A).x * t + (B).x * (kOne - t);       \
    (dst).y = (A).y * t + (B).y * (kOne - t);       \
    (dst).z = (A).z * t + (B).z * (kOne - t)

#define LERPC(dst, A, B)                            \
    (dst).r = (u8)((A).r * t + (B).r * (kOne - t)); \
    (dst).g = (u8)((A).g * t + (B).g * (kOne - t)); \
    (dst).b = (u8)((A).b * t + (B).b * (kOne - t)); \
    (dst).a = (u8)((A).a * t + (B).a * (kOne - t))

static const f32 kOne = 1.0f;
static const f32 kSpotAngle = 30.0f;
static const f32 kSpotBlend = 0.8f;

static HuVecF kAmb0 = {0.8f, 0.8f, 0.8f};
static HuVecF kPos1 = {2000.0f, 10000.0f, 0.0f};
static HuVecF kDir1 = {-1.0f, -1.0f, 0.0f};
static GXColor kAmbCol = {0xFF, 0xFF, 0xFF, 0xFF};
static HuVecF kAmb1 = {0.6f, 0.6f, 0.7f};
static HuVecF kPosB1 = {2000.0f, 10000.0f, 0.0f};
static HuVecF kDirB1 = {-1.0f, -1.0f, 0.0f};
static GXColor kAmbCol1 = {0xB2, 0x99, 0x7F, 0xFF};
static HuVecF kAmb2 = {0.1f, 0.1f, 0.4f};
static HuVecF kPosB2 = {2000.0f, 10000.0f, 0.0f};
static HuVecF kDirB2 = {-1.0f, -1.0f, 0.0f};
static GXColor kAmbCol2 = {0x99, 0x7F, 0x4C, 0xFF};
static HuVecF kAmb3 = {0.5f, 0.5f, 0.5f};
static HuVecF kPos3 = {0.0f, -2200.0f, 2300.0f};
static HuVecF kDir3 = {0.0f, 0.5f, -1.0f};
static GXColor kAmbCol3 = {0xE5, 0xB2, 0xB2, 0xFF};
static HuVecF kPos2 = {0.0f, 800.0f, 1000.0f};
static HuVecF kDir2 = {0.0f, -1.0f, -1.0f};
static GXColor kSpotCol = {0x60, 0x50, 0x60, 0xFF};
static HuVecF kPosL = {0.0f, 1500.0f, 0.0f};
static HuVecF kDirL = {0.0f, -1.0f, 0.02f};
static GXColor kLCol = {0xFF, 0x80, 0x60, 0x80};
static GXColor kColOff = {0, 0, 0, 0};

// NONMATCHING: MWCC evaluates Hu3DGLightSpotSet's cutoff argument before the
// spot-function constant; the original loads f1 first then li r4, 2. The
// body is otherwise instruction-for-instruction identical.
// Builds the ambient fill light and the spot light, both made made static.
void fn_1_2253C(LIGHT *self, s32 arg1)
{
    memset(self, 0, 0x18);
    self->owner = (LightStage *)arg1;
    Hu3DAmbColorSet(kOne, kOne, kOne);
    self->ambientId = Hu3DGLightCreateV(
        &kPos1,
        &kDir1,
        &kAmbCol
    );
    Hu3DGLightStaticSet(self->ambientId, 1);
    Hu3DGLightInfinitytSet(self->ambientId);
    self->spotId = Hu3DGLightCreateV(
        &kPos2,
        &kDir2,
        &kSpotCol
    );
    Hu3DGLightSpotSet(self->spotId, GX_SP_COS, kSpotAngle);
    Hu3DGLightStaticSet(self->spotId, 1);
}

// Releases both lights when their ids are non-negative, then wipes the object.
void fn_1_22620(LIGHT *self)
{
    if (0 <= self->spotId) {
        Hu3DGLightKill(self->spotId);
        self->spotId = -1;
    }
    if (self->ambientId != 0) {
        Hu3DGLightKill(self->ambientId);
        self->ambientId = -1;
    }
    memset(self, 0, 0x18);
}

void fn_1_22690(void)
{
}

// Walks the whole rig through the five lighting phases (0..3 plus "nothing to
// do"): every phase re-blends the ambient broadcast colour, the scenic light's
// position/direction and the ambient light's colour between a "from" and a
// "to" constant, and phase 2 also re-blends the spot light itself. Phase 3 is
// the odd one out: the spot light becomes a model-local light and the global
// one disappears (and vice versa on the way back).
void fn_1_22694(LIGHT *self, s16 mode, f32 t)
{
    LightStage *stage = self->owner;
    volatile u32 dummy = (u32)stage + 0x160;
    u8 *wheels = (u8 *)stage + 0x227C;
    lightVec col;
    lightVec pos;
    lightVec dir;
    GXColor lcol;

    (void)dummy;

    if (mode == 3) {
        if (self->spotIsLocal == 0) {
            Hu3DGLightKill(self->spotId);
            self->spotId = Hu3DLLightCreateV(
                *(s16 *)(wheels + 0x5AE),
                &kPosL,
                &kDirL,
                &kLCol
            );
            Hu3DLLightSpotSet(
                *(s16 *)(wheels + 0x5AE),
                self->spotId,
                GX_SP_COS,
                kSpotAngle
            );
            Hu3DLLightStaticSet(
                *(s16 *)(wheels + 0x5AE),
                self->spotId,
                1
            );
            self->spotIsLocal = 1;
        }
    } else if (self->spotIsLocal != 0) {
        Hu3DGLightKill(self->spotId);
        self->spotId = Hu3DGLightCreateV(
            &kPos2,
            &kDir2,
            &kSpotCol
        );
        Hu3DGLightSpotSet(self->spotId, GX_SP_COS, kSpotAngle);
        Hu3DGLightStaticSet(self->spotId, 1);
        self->spotIsLocal = 0;
    }

    switch (mode) {
    case 0:
        LERP3(col, kAmb0, kAmb3);
        fn_1_23CB8(stage, &col);
        LERP3(pos, kPos1, kPos3);
        LERP3(dir, kDir1, kDir3);
        Hu3DGLightPosSetV(self->ambientId, (HuVecF *)&pos, (HuVecF *)&dir);
        LERPC(lcol, kAmbCol, kAmbCol3);
        Hu3DGLightColorSet(self->ambientId, lcol.r, lcol.g, lcol.b, lcol.a);
        Hu3DGLightColorSet(self->spotId, 0, 0, 0, 0);
        break;

    case 1:
        LERP3(col, kAmb1, kAmb0);
        fn_1_23CB8(stage, &col);
        LERP3(pos, kPosB1, kPos1);
        LERP3(dir, kDirB1, kDir1);
        Hu3DGLightPosSetV(self->ambientId, (HuVecF *)&pos, (HuVecF *)&dir);
        LERPC(lcol, kAmbCol1, kAmbCol);
        Hu3DGLightColorSet(self->ambientId, lcol.r, lcol.g, lcol.b, lcol.a);
        Hu3DGLightColorSet(self->spotId, 0, 0, 0, 0);
        break;

    case 2:
        LERP3(col, kAmb2, kAmb1);
        fn_1_23CB8(stage, &col);
        LERP3(pos, kPosB2, kPosB1);
        LERP3(dir, kDirB2, kDirB1);
        Hu3DGLightPosSetV(self->ambientId, (HuVecF *)&pos, (HuVecF *)&dir);
        LERPC(lcol, kAmbCol2, kAmbCol1);
        Hu3DGLightColorSet(self->ambientId, lcol.r, lcol.g, lcol.b, lcol.a);
        LERP3(pos, kPos2, kPosL);
        LERP3(dir, kDir2, kDirL);
        Hu3DGLightPosSetV(self->spotId, (HuVecF *)&pos, (HuVecF *)&dir);
        LERPC(lcol, kSpotCol, kColOff);
        Hu3DGLightColorSet(self->spotId, lcol.r, lcol.g, lcol.b, lcol.a);
        break;

    case 3:
        LERP3(col, kAmb3, kAmb2);
        fn_1_23CB8(stage, &col);
        LERP3(pos, kPos3, kPosB2);
        LERP3(dir, kDir3, kDirB2);
        Hu3DGLightPosSetV(self->ambientId, (HuVecF *)&pos, (HuVecF *)&dir);
        LERPC(lcol, kAmbCol3, kAmbCol2);
        Hu3DGLightColorSet(self->ambientId, lcol.r, lcol.g, lcol.b, lcol.a);
        break;
    }
}

// NONMATCHING: the original keeps six precomputed base pointers in r23-r30 built by a
// flat run of addi off r27; expressing them as derived locals makes MWCC re-spill r3
// and allocate a different callee-save set (_savegpr_26 vs _savegpr_22).
// Broadcasts one ambient colour to every model in the scene: the six body
// models, the two extras, the twenty wheel entries, then the two per-type
// models, and finally records the colour on the object.
void fn_1_23CB8(LightStage *self, const lightVec *col)
{
    s32 i;
    u8 *base = (u8 *)self;
    s16 *body = (s16 *)(base + 0x16C);
    s16 *type = (s16 *)(base + 0x30C);
    u8 *wheels = base + 0x227C;

    for (i = 0; i < 6; i++) {
        Hu3DModelAmbSet(body[i], col->x, col->y, col->z);
    }
    Hu3DModelAmbSet(*(s16 *)(base + 0x3CC), col->x, col->y, col->z);
    Hu3DModelAmbSet(*(s16 *)(base + 0x2C), col->x, col->y, col->z);
    for (i = 0; i < 6; i++) {
        Hu3DModelAmbSet(*(s16 *)(wheels + 0x5A8 + i * 2), col->x, col->y, col->z);
    }
    Hu3DModelAmbSet(*(s16 *)(wheels + 0x5B6), col->x, col->y, col->z);
    Hu3DModelAmbSet(*(s16 *)(wheels + 0x5C0), col->x, col->y, col->z);
    for (i = 0; i < 20; i++) {
        Hu3DModelAmbSet(wheels[i * 0x48 + 0x10], col->x, col->y, col->z);
    }
    if (*(u16 *)(type + 2) == 3) {
        Hu3DModelAmbSet(type[0x10], col->x, col->y, col->z);
    } else {
        Hu3DModelAmbSet(type[0x10], kSpotBlend, kSpotBlend, kSpotBlend);
    }
    *(s32 *)(base + 0x2F4) = *(s32 *)&col->x;
    *(s32 *)(base + 0x2F8) = *(s32 *)&col->y;
    *(s32 *)(base + 0x2FC) = *(s32 *)&col->z;
}
