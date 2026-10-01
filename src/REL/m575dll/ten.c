#include "REL/m575dll.h"
#include "game/hu3d.h"
#include "string.h"

// acos lives in math.h, whose unused extern inline sqrtf would add
// _half/_three to this unit's .rodata; the original object has neither.
extern double acos(double x);

// REL/m575dll/m575.c and REL/m575dll/tuboeff.c, whose headers still pull in
// math.h the same way.
extern void fn_1_2D5C(Quaternion *q, const Vec *v0, const Vec *v1);
extern void fn_1_530(u16 dataNum);
extern void fn_1_9320(M575TuboEff *eff, s32 arg1, const Vec *arg2, f32 height, f32 radius, f32 speed);
extern void fn_1_96F8(M575TuboEff *eff);
extern void fn_1_9728(M575TuboEff *eff);

static const f32 lbl_1_rodata_160[1] = { 0.0f };
static const f32 lbl_1_rodata_164[1] = { 750.0f };
static const f32 lbl_1_rodata_168[1] = { 4.0f };
static const f32 lbl_1_rodata_16C[1] = { 1.3f };
const Vec lbl_1_rodata_170 = { 0.0f, 0.0f, 0.0f };
const Vec lbl_1_rodata_17C = { 0.0f, 1.0f, 0.0f };
static const f32 lbl_1_rodata_188[1] = { -65.0f };
static const f32 lbl_1_rodata_18C[1] = { 0.9f };
static const f64 lbl_1_rodata_190[1] = { 180.0 };
static const f64 lbl_1_rodata_198[1] = { 3.141592653589793 };
static const f32 lbl_1_rodata_1A0[1] = { 30.0f };
static const f32 lbl_1_rodata_1A4[1] = { 0.5235988f };
static const f32 lbl_1_rodata_1A8[1] = { 0.98f };
static const f32 lbl_1_rodata_1AC[1] = { 0.5f };
static const f32 lbl_1_rodata_1B0[1] = { 90.0f };
static const f32 lbl_1_rodata_1B4[1] = { 0.017453292f };
static const f32 lbl_1_rodata_1B8[2] = { 25.0f, 0.0f };

// NONMATCHING: register allocation - the original keeps the main pointer in r27
// with a copy in r28 and `&copy->tenConf` in r29, then reuses the model id's
// r30 slot for the second model. With that shape here the allocator swaps r30
// and r31 between `self` and `modelId` instead.
void fn_1_4BCC(M575Ten *self, M575Main *main)
{
    M575TenConf *conf = &main->tenConf;
    HU3DMODELID modelId;
    HU3DMODELID modelId2;

    memset(self, 0, sizeof(M575Ten));
    self->main = main;
    if (main->unk4 == 0) {
        modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x820007, HU_MEMNUM_OVL, HEAP_MODEL));
        self->modelId = modelId;
        self->motionId = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x820016, HU_MEMNUM_OVL, HEAP_MODEL));
    }
    else {
        modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x820006, HU_MEMNUM_OVL, HEAP_MODEL));
        self->modelId = modelId;
        self->motionId = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(0x820015, HU_MEMNUM_OVL, HEAP_MODEL));
    }
    Hu3DModelPosSet(modelId, lbl_1_rodata_160[0], lbl_1_rodata_160[0], lbl_1_rodata_160[0]);
    Hu3DModelRotSet(modelId, lbl_1_rodata_160[0], lbl_1_rodata_160[0], lbl_1_rodata_160[0]);
    Hu3DModelCameraSet(modelId, conf->camBit);
    Hu3DModelLayerSet(modelId, 0);
    Hu3DModelAttrReset(modelId, 1);
    Hu3DModelShadowMapSet(modelId);
    self->pos = &Hu3DData[modelId].pos;
    self->mtx = (MtxPtr)&Hu3DData[modelId].mtx;
    Hu3DMotionSet(modelId, self->motionId);
    Hu3DModelAttrSet(modelId, HU3D_MOTATTR_LOOP);
    modelId2 = Hu3DModelCreate(HuDataSelHeapReadNum(0x82000E, HU_MEMNUM_OVL, HEAP_MODEL));
    self->modelId2 = modelId2;
    Hu3DModelPosSet(modelId2, lbl_1_rodata_160[0], lbl_1_rodata_160[0], lbl_1_rodata_160[0]);
    Hu3DModelRotSet(modelId2, lbl_1_rodata_160[0], lbl_1_rodata_160[0], lbl_1_rodata_160[0]);
    Hu3DModelCameraSet(modelId2, conf->camBit);
    Hu3DModelLayerSet(modelId2, 0);
    Hu3DModelAttrSet(modelId2, 1);
    self->pos2 = &Hu3DData[modelId2].pos;
    self->mtx2 = (MtxPtr)&Hu3DData[modelId2].mtx;
    fn_1_9320(&self->eff, 0, (const Vec *)&lbl_1_rodata_170, lbl_1_rodata_164[0], lbl_1_rodata_168[0], lbl_1_rodata_16C[0]);
}

void fn_1_4E98(M575Ten *self)
{
    fn_1_96F8(&self->eff);
    Hu3DMotionKill(self->motionId);
    Hu3DModelKill(self->modelId2);
    Hu3DModelKill(self->modelId);
    memset(self, 0, sizeof(M575Ten));
}

void fn_1_4EF0(M575Ten *self)
{
    M575Main *main = self->main;
    M575MainConf *conf = main->conf;
    M575TuboEff *eff = &self->eff;
    Mtx mtx;
    Vec v;
    Vec dir;
    Vec axis;
    Vec up;
    Quaternion q0, q1, q2, q3, q4, q5;
    Vec tmpA;
    Vec tmpB;
    Vec *pA;
    Vec *pB;
    f32 angle;
    f32 dot;
    f32 rad;
    f32 clamped;

    fn_1_9728(eff);
    PSMTXCopy((const f32 (*)[4])&eff->mtxRoot, self->mtx);
    PSMTXCopy((const f32 (*)[4])&eff->mtxRoot, self->mtx2);
    self->pos->y = lbl_1_rodata_188[0];
    self->pos2->y = lbl_1_rodata_188[0];
    tmpA = lbl_1_rodata_170;
    pA = &tmpA;
    eff->offset = *pA;
    PSVECScale(&eff->pos, &eff->pos, lbl_1_rodata_18C[0]);
    PSVECScale(&eff->vel, &eff->vel, lbl_1_rodata_18C[0]);
    tmpB = lbl_1_rodata_17C;
    pB = &tmpB;
    up = *pB;
    dir.x = eff->dir.x;
    dir.y = eff->dir.y;
    dir.z = eff->dir.z;
    PSVECNormalize(&dir, &dir);
    dot = PSVECDotProduct(&dir, &up);
    rad = (f32)acos(dot);
    angle = lbl_1_rodata_190[0] * (rad / lbl_1_rodata_198[0]);

    if (angle > lbl_1_rodata_1A0[0]) {
        v.x = dir.x;
        v.y = lbl_1_rodata_160[0];
        v.z = dir.z;
        PSVECNormalize(&v, &v);
        PSVECCrossProduct(&up, &v, &axis);
        PSMTXRotAxisRad(mtx, &axis, lbl_1_rodata_1A4[0]);
        PSMTXMultVec(mtx, &up, &v);
        fn_1_2D5C(&q0, &dir, &v);
        C_QUATMtx(&q1, (const f32 (*)[4])&eff->mtxRoot);
        PSQUATMultiply(&q0, &q1, &q2);
        PSMTXQuat((f32 (*)[4])&eff->mtxRoot, &q2);
        angle = lbl_1_rodata_1A0[0];
    }

    if (conf->unkC >= 5) {
        v.x = dir.x;
        v.y = lbl_1_rodata_160[0];
        v.z = dir.z;
        angle = lbl_1_rodata_1A8[0] * (self->rot - lbl_1_rodata_1AC[0]);

        if (angle > lbl_1_rodata_1B0[0]) {
            clamped = lbl_1_rodata_1B0[0];
        }
        else {
            clamped = (angle < lbl_1_rodata_160[0]) ? lbl_1_rodata_160[0] : angle;
        }
        angle = clamped;

        PSVECNormalize(&v, &v);
        PSVECCrossProduct(&up, &v, &axis);
        PSMTXRotAxisRad(mtx, &axis, lbl_1_rodata_1B4[0] * angle);
        PSMTXMultVec(mtx, &up, &v);
        fn_1_2D5C(&q3, &dir, &v);
        C_QUATMtx(&q4, (const f32 (*)[4])&eff->mtxRoot);
        PSQUATMultiply(&q3, &q4, &q5);
        PSMTXQuat((f32 (*)[4])&eff->mtxRoot, &q5);
    }
    self->prevRot = self->rot;
    self->rot = angle;

    if (self->prevRot < lbl_1_rodata_1B8[0] && self->rot > lbl_1_rodata_1B8[0] && self->counter == 0) {
        if (main->unk4 == 0) {
            fn_1_530(0x930);
        }
        else if (main->unk4 == 1) {
            fn_1_530(0x931);
        }
        self->counter = 0x1E;
    }

    if (self->counter != 0) {
        self->counter--;
    }

    if (self->timer != 0) {
        self->timer--;
    }
}
