// m562dll/light ' the scene's two lights (ambient fill + a spot) and the
// per-frame ambient broadcast.
#include "types.h"

typedef s16 HU3DMODELID;
typedef s16 HU3DLIGHTID;

#include "game/hu3d.h"

void Hu3DAmbColorSet(f32 r, f32 g, f32 b);

/* Three floats copied as a unit ' the target moves them word by word. */
typedef struct lightVec_s {
    f32 x;
    f32 y;
    f32 z;
} lightVec;



/* 0x18 bytes: an owner word, the ambient light id, the spot light id. */
typedef struct light_s {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ HU3DLIGHTID ambientId;
    /* 0x06 */ HU3DLIGHTID spotId;
} LIGHT;

static const f32 kOne = 1.0f;
static const f32 kSpotAngle = 30.0f;
static const f32 kSpotBlend = 0.8f;
static HuVecF kPos1 = {2000.0f, 10000.0f, 0.0f};
static HuVecF kDir1 = {-1.0f, -1.0f, 0.0f};
static HuVecF kPos2 = {0.0f, 800.0f, 1000.0f};
static HuVecF kDir2 = {0.0f, -1.0f, -1.0f};
static GXColor kColFF = {0xFF, 0, 0, 0};
static GXColor kCol60 = {0x60, 0, 0, 0};

// NONMATCHING: MWCC evaluates Hu3DGLightSpotSet's cutoff argument before the
// spot-function constant; the original loads f1 first then li r4, 2. The
// body is otherwise instruction-for-instruction identical.
// Builds the ambient fill light and the spot light, both made made static.
void fn_1_2253C(LIGHT *self, s32 arg1)
{
    memset(self, 0, 0x18);
    self->unk00 = arg1;
    Hu3DAmbColorSet(kOne, kOne, kOne);
    self->ambientId = Hu3DGLightCreateV(
        &kPos1,
        &kDir1,
        &kColFF
    );
    Hu3DGLightStaticSet(self->ambientId, 1);
    Hu3DGLightInfinitytSet(self->ambientId);
    self->spotId = Hu3DGLightCreateV(
        &kPos2,
        &kDir2,
        &kCol60
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
// NONMATCHING: the original keeps six precomputed base pointers in r23-r30 built by a\n// flat run of addi off r27; expressing them as derived locals makes MWCC re-spill r3\n// and allocate a different callee-save set (_savegpr_26 vs _savegpr_22).\n// Broadcasts one ambient colour to every model in the scene: the six body
// models, the two extras, the twenty wheel entries, then the two per-type
// models, and finally records the colour on the object.
void fn_1_23CB8(void *self, const lightVec *col)
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