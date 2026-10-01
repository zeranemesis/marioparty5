#include "REL/m575dll.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/memory.h"
#include "string.h"

static const f32 lbl_1_rodata_450[1] = { 0.0f };
static const f32 lbl_1_rodata_454[1] = { 320.0f };
static const f32 lbl_1_rodata_458[1] = { 1.0f };
static const f32 lbl_1_rodata_45C[1] = { 500.0f };

// Hu3DData's element type is opaque in hu3d.h, so the two hooks the tube keeps
// per model are reached through the raw byte offset the original object uses.
#define TUBO_DATA(n) ((0x82 << 16) + (n))
#define TUBO_HOOK(modelId, offset) ((void *)((u8 *)&Hu3DData[modelId] + (offset)))

// NONMATCHING: 83.71%. Every instruction is the one the original object has,
// but the original keeps one more value alive across the prologue (a copy of
// conf, read once for the unk4 test) and so allocates r27..r31 where this
// version allocates a different permutation of the same five registers.
void fn_1_9BC8(M575Tubo *self, M575Main *conf)
{
    M575TenConf *tenConf = &conf->tenConf;
    s16 modelId1;
    s16 modelId2;

    memset(self, 0, sizeof(*self));
    self->main = conf;

    if (conf->unk4 == 0) {
        modelId1 = Hu3DModelCreateData(TUBO_DATA(9));
        self->modelId1 = modelId1;
    }
    else {
        modelId1 = Hu3DModelCreateData(TUBO_DATA(8));
        self->modelId1 = modelId1;
    }

    Hu3DModelPosSet(modelId1, lbl_1_rodata_450[0], lbl_1_rodata_454[0], lbl_1_rodata_450[0]);
    Hu3DModelRotSet(modelId1, lbl_1_rodata_450[0], lbl_1_rodata_450[0], lbl_1_rodata_450[0]);
    Hu3DModelCameraSet(modelId1, tenConf->camBit);
    Hu3DModelLayerSet(modelId1, 0);
    Hu3DModelAttrReset(modelId1, 1);

    self->unkC = TUBO_HOOK(modelId1, 0xCC);
    self->unk10 = TUBO_HOOK(modelId1, 0xF0);
    Hu3DModelShadowSet(modelId1);

    modelId2 = Hu3DModelCreateData(TUBO_DATA(0xD));
    self->modelId2 = modelId2;

    Hu3DModelPosSet(modelId2, lbl_1_rodata_450[0], lbl_1_rodata_454[0], lbl_1_rodata_450[0]);
    Hu3DModelRotSet(modelId2, lbl_1_rodata_450[0], lbl_1_rodata_450[0], lbl_1_rodata_450[0]);
    Hu3DModelCameraSet(modelId2, tenConf->camBit);
    Hu3DModelLayerSet(modelId2, 0);
    Hu3DModelAttrSet(modelId2, 1);

    self->unk18 = TUBO_HOOK(modelId2, 0xCC);
    self->unk1C = TUBO_HOOK(modelId2, 0xF0);

    self->motionId = Hu3DJointMotionData(modelId2, TUBO_DATA(0x17));
    Hu3DMotionSet(modelId2, self->motionId);
    Hu3DMotionSpeedSet(modelId2, lbl_1_rodata_458[0]);
    Hu3DModelAttrReset(modelId2, 0x40000001);

    fn_1_9320(&self->eff, 1, (const Vec *)&lbl_1_rodata_45C,
              lbl_1_rodata_45C[0], lbl_1_rodata_458[0], lbl_1_rodata_450[0]);
    PSMTXIdentity((f32 (*)[4])&self->mtx);
}

void fn_1_9E8C(M575Tubo *self)
{
    fn_1_96F8(&self->eff);
    Hu3DMotionKill(self->motionId);
    Hu3DModelKill(self->modelId2);
    Hu3DModelKill(self->modelId1);
    memset(self, 0, sizeof(*self));
}