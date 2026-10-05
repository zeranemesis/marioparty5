#include "dolphin/mtx.h"

extern "C" {
typedef s16 HU3DMODELID;
typedef s16 HU3DMOTID;
typedef struct Process_s OMOBJMAN;
typedef struct omObj_s OMOBJ;
typedef void (*OMOBJFUNC)(OMOBJ *obj);
typedef struct Hu3DModel_s HU3DMODEL;
typedef struct Hu3DParticle_s HU3DPARTICLE;
typedef struct ANIMDATA ANIMDATA;

struct Hu3DModel_s {
    u8 unk000[0x120];
    void *hookData;
    u8 unk124[0x134 - 0x124];
};
extern HU3DMODEL Hu3DData[];

struct Hu3DParticle_s {
    s16 dataCnt;
    s16 emitCnt;
    f32 unk04[6];
    void *ownerP;
};

struct omObj_s {
    u16 stat;
    s16 objNext;
    s16 prio;
    s16 prev;
    s16 next;
    s16 nextNo;
    s16 grpNo;
    u16 memberNo;
    u32 mode;
    OMOBJFUNC objFunc;
    Vec trans;
    Vec rot;
    Vec scale;
    u16 mdlcnt;
    HU3DMODELID *mdlId;
    u16 mtncnt;
    HU3DMOTID *mtnId;
    u32 work[4];
    void *data;
};

OMOBJ *omAddObjEx(OMOBJMAN *objMan, s16 prio, u16 mdlcnt, u16 mtncnt, s16 grpNo, OMOBJFUNC objFunc);
void omDelObjEx(OMOBJMAN *objMan, OMOBJ *obj);
void *__dl__FPv(void *ptr);
void *HuDataSelHeapReadNum(int dataNum, s32 num, int heap);
HU3DMODELID Hu3DHookFuncCreate(void *hookFunc);
void Hu3DModelKill(HU3DMODELID modelId);
void Hu3DModelAttrSet(HU3DMODELID modelId, u32 attr);
void Hu3DModelAttrReset(HU3DMODELID modelId, u32 attr);
void Hu3DModelCameraSet(HU3DMODELID modelId, u16 cameraBit);
void Hu3DModelLayerSet(HU3DMODELID modelId, s16 layerNo);
HU3DMODELID Hu3DParticleCreate(ANIMDATA *anim, s16 maxCnt);
ANIMDATA *HuSprAnimRead(void *data);
void HuSprAnimLock(ANIMDATA *anim);
void HuSprAnimKill(ANIMDATA *anim);
s32 HuAudFXPlay(s32);
}

extern "C" {
void fn_1_EE68(void *self, f32 a2, f32 a3);

void fn_1_F6AC(void *dst, f32 *src)
{
    ((f32 *)((u8 *)dst + 0x14480))[0] = src[0];
    ((f32 *)((u8 *)dst + 0x14480))[1] = src[1];
    ((f32 *)((u8 *)dst + 0x14480))[2] = src[2];
}

void *fn_1_DAEC(void *obj, s32 idx)
{
    if (idx == 1) {
        return (u8 *)obj + 0x4;
    }
    if (idx >= 2) {
        return obj;
    }
    return obj;
}

void fn_1_E560(void *self)
{
    void *data = HuDataSelHeapReadNum(0x5905, 2, 0x10000000);
    ANIMDATA *anim = HuSprAnimRead(data);
    *(ANIMDATA **)((u8 *)self + 0x50) = anim;
}

void fn_1_F624(void *self, f32 val, f32 a3)
{
    fn_1_EE68(self, val, 32.0f);
}

void fn_1_F668(void *self, f32 val, f32 a3)
{
    fn_1_EE68(self, val, -160.0f);
}

void fn_1_10294(void *self)
{
    s32 *soundIdPtr = (s32 *)((u8 *)self + 0x144B4);
    if (*soundIdPtr < 0) {
        s32 soundId = *(s32 *)((u8 *)self + 0x144B0);
        s32 result = HuAudFXPlay(soundId);
        *soundIdPtr = result;
    }
}
}