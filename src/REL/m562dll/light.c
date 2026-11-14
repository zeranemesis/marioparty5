// m562dll/light ' the scene's two lights (ambient fill + a spot) and the
// per-frame ambient broadcast.
#include "REL/m562dll.h"

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
static GXColor kColOff[2] = {{0, 0, 0, 0}, {0, 0, 0, 0}};

// NONMATCHING: MWCC evaluates Hu3DGLightSpotSet's cutoff argument before the
// spot-function constant; the original loads f1 first then li r4, 2. The
// body is otherwise instruction-for-instruction identical.
// Builds the ambient fill light and the spot light, both made made static.
void fn_1_2253C(LIGHT *self, LightStage *stage)
{
    memset(self, 0, sizeof(LIGHT));
    self->owner = stage;
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
    memset(self, 0, sizeof(LIGHT));
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
    lightVec col;
    lightVec pos;
    lightVec dir;
    GXColor lcol;

    if (mode == 3) {
        if (self->spotIsLocal == 0) {
            Hu3DGLightKill(self->spotId);
            self->spotId = Hu3DLLightCreateV(
                stage->wheelModel[3],
                &kPosL,
                &kDirL,
                &kLCol
            );
            Hu3DLLightSpotSet(
                stage->wheelModel[3],
                self->spotId,
                GX_SP_COS,
                kSpotAngle
            );
            Hu3DLLightStaticSet(
                stage->wheelModel[3],
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
        LERPC(lcol, kSpotCol, kColOff[0]);
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

// Broadcasts one ambient colour to every model in the scene and records it
// in the light block.
void fn_1_23CB8(LightStage *self, const lightVec *col)
{
    LIGHT *light = (LIGHT *)self->lightBlock;
    s32 i;

#define AMB(id) Hu3DModelAmbSet((id), col->x, col->y, col->z)
    AMB(self->bodyModel[0]);
    AMB(self->bodyModel[1]);
    AMB(self->bodyModelC);
    AMB(self->bodyModelD[0]);
    AMB(self->bodyModelD[1]);
    AMB(self->bodyModelE);
    AMB(self->ambModelB);
    AMB(self->ambModelA);
    AMB(self->wheelModel[0]);
    AMB(self->wheelModel[1]);
    AMB(self->wheelModel[2]);
    AMB(self->wheelModel[3]);
    AMB(self->wheelModel[4]);
    AMB(self->wheelModelA);
    AMB(self->wheelModelB);
    for (i = 0; i < 20; i++) {
        AMB(self->wheel[i].model);
    }
    if (self->kind == 3) {
        AMB(self->kindModel);
    } else {
        Hu3DModelAmbSet(self->kindModel, kSpotBlend, kSpotBlend, kSpotBlend);
    }
#undef AMB
    light->amb = *col;
}
