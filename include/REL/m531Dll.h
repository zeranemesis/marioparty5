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
void Hu3DMotionShiftSet(HU3DMODELID modelId, HU3DMOTID motId, float start, float end, u32 attr);
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

struct CPlayer;
struct CFlowStage;

namespace m531 {

class CGameMaster : public CMinigameCore {
public:
    s32 m_mode;
    OMOBJ *m_obj;
    OMOBJMAN *m_objMan;

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

    ::CPlayer *m_player[4]; // 0x2C
    s32 m_unk3C[2];
    ::CFlowStage *m_stage; // 0x44
    s32 m_unk48[2];
    CTimer m_timer; // 0x50
    // Two 10x5 float grids (mode 0 and mode 1), indexed [row][column].
    f32 m_grid[2][10][5]; // 0x54
    u8 m_unk1E4[0x208 - 0x1E4];
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

// Stage object (CStage and its derived classes): the vtable pointer follows the data.
struct CStageObj;
struct StageObjVt {
    void *m_head[2];
    void (*m_update)(CStageObj *self);
    CStageObj *(*m_dtor)(CStageObj *self, s16 flag);
};

struct CStageObj {
    OMOBJMAN *m_objMan;
    HU3DMODELID m_model;
    OMOBJ *m_obj;
    Vec m_pos;
    Vec m_rot;
    Vec m_scale;
    const StageObjVt *m_vt;
};

// Object of the player stage ("stage" = flow field) read by the players.
struct CFlowStage {
    s32 m_unk0[18];
    f32 m_angle; // 0x48
};

struct CPlayerBase;

struct PlayerBaseVt {
    void *m_head[2];
    void (*m_update0)(CPlayerBase *self);
    void (*m_update1)(CPlayerBase *self);
    void (*m_update2)(CPlayerBase *self);
    void (*m_update3)(CPlayerBase *self);
    void (*m_input)(CPlayerBase *self);
    void (*m_cpu)(CPlayerBase *self);
    void (*m_post)(CPlayerBase *self);
    CPlayerBase *(*m_dtor)(CPlayerBase *self, s16 flag);
    void (*m_set0)(CPlayerBase *self);
    void (*m_set1)(CPlayerBase *self);
};

struct PlayerMotInfo {
    s32 dataNum;
    u32 attr;
    f32 shiftTime;
};

// MWCC puts the vtable pointer after the data of the class (offset 0xB0 on the GC).
struct CPlayerBase {
    OMOBJMAN *m_objMan;
    s16 m_playerNo;
    s16 m_charNo;
    s16 m_model;
    s16 m_motId[32];
    s16 m_type;
    s16 m_dif;
    s8 m_padNo;
    s32 m_curMot;
    u8 m_flag54;
    u8 m_flag55;
    OMOBJ *m_obj;
    PlayerMotInfo *m_motTbl;
    s32 m_motNum;
    f32 m_stickX;
    f32 m_stickY;
    u8 m_unk6C[0x78 - 0x6C];
    Vec m_home; // 0x78
    Vec m_pos;
    Vec m_rot;
    Vec m_scale;
    s32 m_nextMot;
    s32 m_dataNum;
    const PlayerBaseVt *m_vt;
};

class CEffectParticle;

// The cleaning-contest player (vtable lbl_1_data_3C4).
struct CPlayer {
    CPlayerBase m_base;
    f32 m_turn; // 0xB4
    m531::CFlow *m_flow;
    s32 m_unkBC;
    CEffectParticle *m_hit; // 0xC0
    s16 m_hatModel; // 0xC4
    Vec m_unkC8;
    f32 m_unkD4;
    u8 m_unkD8;
    f32 m_unkDC;
    s32 m_side; // 0xE0
    f32 m_unkE4;
    s32 m_unkE8;
    u8 m_unkEC;
    f32 m_unkF0;
    s32 m_team; // 0xF4
    u8 m_unkF8;
    s32 m_se; // 0xFC
    s32 m_unk100;
    s32 m_dir; // 0x104
    Vec m_cell; // 0x108
    s32 m_unk114;
    f32 m_unk118;
    Vec m_target; // 0x11C
    Vec m_unk128;
};

// The player that runs over the stage (vtable lbl_1_data_364).
struct CPlayerRunner {
    CPlayerBase m_base;
    s32 m_frame; // 0xB4
    CStageObj *m_obj[2]; // 0xB8
    f32 m_vel; // 0xC0
    u8 m_hit; // 0xC4
};

#endif
