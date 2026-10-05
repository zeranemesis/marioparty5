#ifndef _REL_M531DLL_H
#define _REL_M531DLL_H

// Shared declarations for the m531Dll minigame. game/object.h and game/hu3d.h
// are not included from C++: both pull in MSL math.h, whose inline std::sqrtf
// emits constants into .rodata that the REL does not have.

#include "dolphin/mtx.h"
#include "version.h"

extern "C" {
typedef s16 HU3DMODELID;
typedef s16 HU3DMOTID;
typedef struct Process_s OMOBJMAN;
typedef struct omObj_s OMOBJ;
typedef void (*OMOBJFUNC)(OMOBJ *obj);

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

typedef struct Hu3DCamera_s {
    float fov;
    float near;
    float far;
    float aspect;
    float upRot;
    Vec pos;
    Vec up;
    Vec target;
    s16 scissorX;
    s16 scissorY;
    s16 scissorW;
    s16 scissorH;
    float viewportX;
    float viewportY;
    float viewportW;
    float viewportH;
    float viewportNear;
    float viewportFar;
} HU3DCAMERA;

// The m531 code only reaches Hu3DData[id].userObj, but indexing the array
// needs the real element size (0x134), so the tail is padded.
typedef struct Hu3DModel_s {
    u8 tick;
    u8 camInfoBit;
    u8 projBit;
    u8 hiliteIdx;
    s8 reflectType;
    s16 layerNo;
    s32 motId;
    s32 motIdOvl;
    s32 motIdShift;
    s32 motIdShape;
    s32 motIdCluster[4];
    s16 clusterAttr[4];
    s32 motIdSrc;
    u16 cameraBit;
    s16 linkMdlId;
    u16 lightNum;
    u16 lightId[8];
    s16 lLightId[8];
    u32 mallocNo;
    u32 mallocNoLink;
    u32 attr;
    u32 motAttr;
    float ambR;
    float ambB;
    float ambG;
    // Pad to the real HU3DMODEL size; m531 indexes Hu3DData by it.
    u8 unk78[0x134 - 0x78];
} HU3DMODEL;

OMOBJ *omAddObjEx(OMOBJMAN *objMan, s16 prio, u16 mdlcnt, u16 mtncnt, s16 grpNo, OMOBJFUNC objFunc);
void omDelObjEx(OMOBJMAN *objMan, OMOBJ *obj);
OMOBJMAN *omInitObjMan(s16 objMax, s32 objManPrio);
void omGameSysInit(OMOBJMAN *objman);
void omOvlReturnEx(s16 hisOfs, s16 unlinkF);
SHARED_SYM extern s16 omSysExitReq;

HU3DMODELID Hu3DModelCreate(void *data);
HU3DMODELID Hu3DHookFuncCreate(void *hookFunc);
void Hu3DModelKill(HU3DMODELID modelId);
void Hu3DModelLayerSet(HU3DMODELID modelId, s16 layerNo);
void Hu3DModelCameraSet(HU3DMODELID modelId, u16 cameraBit);
void Hu3DModelShadowSet(HU3DMODELID modelId);
void Hu3DModelShadowDispOn(HU3DMODELID modelId);
void Hu3DModelAttrSet(HU3DMODELID modelId, u32 attr);
void Hu3DModelAttrReset(HU3DMODELID modelId, u32 attr);
void Hu3DModelPosSet(HU3DMODELID modelId, float x, float y, float z);
void Hu3DModelRotSet(HU3DMODELID modelId, float x, float y, float z);
void Hu3DModelScaleSet(HU3DMODELID modelId, float x, float y, float z);
HU3DMOTID Hu3DMotionCreate(void *data);
HU3DMOTID Hu3DMotionSet(HU3DMODELID modelId, HU3DMOTID motId);
void Hu3DMotionKill(HU3DMOTID motId);
void Hu3DMotionShiftSet(HU3DMOTID motId, float start, float end);
HU3DMOTID Hu3DJointMotion(HU3DMODELID modelId, void *data);
SHARED_SYM extern HU3DCAMERA Hu3DCamera[16];
SHARED_SYM extern HU3DMODEL Hu3DData[512];

void *HuDataSelHeapReadNum(int dataNum, s32 num, int heap);
void HuMemDirectFree(void *ptr);
void *HuMemDirectMallocNum(s32 heap, u32 size, int align);
void HuPrcCurrentGet(void);

HU3DMODELID CharModelCreate(s16 charNo, s16 model);
void CharModelKill(s16 charNo);
HU3DMOTID CharMotionCreate(s16 charNo, u32 dataNum);
void CharMotionDataClose(s16 charNo);
void CharMotionSet(s16 charNo, HU3DMOTID motId);
void CharMotionShiftSet(s16 charNo, HU3DMOTID motId, float start, float end, u32 attr);
void CharModelVoicePanSet(s16 charNo, s16 pan);

int HuAudFXPlay(int seId);
void HuAudFXPanning(int seNo, s16 pan);
void HuAudFadeOut(s32 speed);
int HuAudJinglePlay(s16 streamId);

extern s8 HuPadStkX[4];
extern s8 HuPadStkY[4];
extern s8 HuPadSubStkX[4];
extern s8 HuPadSubStkY[4];
extern u16 HuPadBtn[4];
extern u16 HuPadBtnDown[4];

u32 frand(void);
}

class CTime {
public:
    CTime(s32 time) { m_time = time; }
    s32 m_time;
};

class CTimer {
public:
    CTimer(int type);
    ~CTimer();
    void paramSet(s32 time, s32 scale, s32 unk);
    s32 valueGet();
    void posSet(float start, float end);

private:
    void *m_timer;
};

class CMinigameCore {
protected:
    u32 m_unk0;

public:
    void *operator new(unsigned long size);
    void operator delete(void *ptr);

    CMinigameCore();
    virtual ~CMinigameCore();
    virtual void on_setup(OMOBJMAN *objMan) = 0;
    virtual void on_update();
    virtual void on_close() = 0;

    void setup(int prio, int stackSize);

protected:
    s32 m_winner[4];
    s32 m_unk18;
    s32 m_unk1C;
};

namespace m531 {

class CGameMaster : public CMinigameCore {
protected:
    s32 m_mode;
    OMOBJ *m_obj;
    OMOBJMAN *m_objMan;

public:
    CGameMaster() { m_mode = 0; }
    virtual ~CGameMaster();
    virtual void on_setup(OMOBJMAN *objMan);
    virtual void mode0();
    virtual void mode1();
    virtual void mode2();
    virtual void mode3();
    virtual void mode4();
    virtual void mode5();
    virtual void mode6();

    void setMode(s32 mode);
    static void ObjUpdateMode0(OMOBJ *obj);
    static void ObjUpdateMode1(OMOBJ *obj);
    static void ObjUpdateMode2(OMOBJ *obj);
    static void ObjUpdateMode3(OMOBJ *obj);
    static void ObjUpdateMode4(OMOBJ *obj);
    static void ObjUpdateMode5(OMOBJ *obj);
    static void ObjUpdateMode6(OMOBJ *obj);
};

// Defined outside the class: with the destructor as the first virtual and
// inline, MWCC emits the vtable (weak) in every unit that needs it.
inline CGameMaster::~CGameMaster() {}

class CFlow : public CGameMaster {
public:
    CFlow() : m_timer(0) {}
    virtual ~CFlow();
    virtual void on_close();
    virtual void mode0();
    virtual void mode1();
    virtual void mode2();
    virtual void mode3();
    virtual void mode4();
    virtual void mode5();
    virtual void mode6();

private:
    u8 m_unk2C[0x50 - 0x2C];
    CTimer m_timer;
    u8 m_unk54[0x208 - 0x54];
};

// Base of the score display: an OMOBJ whose objFunc calls on_update().
class CScoreBase {
public:
    CScoreBase(OMOBJMAN *objMan);
    static void ObjUpdate(OMOBJ *obj);

protected:
    OMOBJMAN *m_objMan;
    OMOBJ *m_obj;

public:
    virtual void on_update();
    virtual ~CScoreBase();
};

} // namespace m531

#endif
