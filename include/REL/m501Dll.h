#ifndef _M501DLL_H
#define _M501DLL_H

#include "dolphin/types.h"
#include <dolphin/gx/GXStruct.h>
#include <dolphin/mtx.h>
#include <dolphin/os/OSThread.h>

struct OSSemaphore {
    OSThreadQueue queue;
    s32 count;
};

/* Local copies of the small support classes m501.cpp defines: CGameObject's
 * constructor has to be visible here so a derived constructor can inline it,
 * and CModel is only ever used through its 2-byte model handle. */

extern "C" void __assert_fail(const char *expr, const char *file, int line);

typedef s16 HU3DMODELID;
typedef s16 HU3DMOTID;

struct Hu3DMtx {
    f32 m[4][3];
};

struct Hu3DModel_s;
class CLinkList;

class CRandom {
public:
    static float frandom(float min, float max);
    static u32 irandom();
    static void setSeed(int seed);
};

class CLinkNode {
public:
    CLinkNode *m_prev;
    CLinkNode *m_next;

    CLinkNode();
    CLinkNode(CLinkNode &);
    ~CLinkNode();
    void insertFront(CLinkList *list);
    void insertBack(CLinkList *list);
    void copyLink(CLinkList *list);
};

class CLinkList : public CLinkNode {
public:
    bool isEmpty()
    {
        bool empty = false;
        if (m_prev == this && m_prev == m_next) {
            empty = true;
        }
        return empty;
    }
};

/* the original symbol carries a comparator typed "void", keep it as plain C */
extern "C" void sortList__9CLinkListFPFP9CLinkNodeP9CLinkNode_v(CLinkList *list, bool (*cmp)(CLinkNode *, CLinkNode *));

struct CHandleNode {
    CHandleNode *m_next;
    CHandleNode *m_prev;
    void *m_ptr;
    u32 m_index : 20;
    u32 m_gen : 12;
};

class CHandleMan {
public:
    CHandleMan(unsigned int n);
    ~CHandleMan();
    int insertHandle(void *ptr);
    void *removeHandle(int handle);
    void *getHandlePtr(int handle);
    int getHandleNum();
    int getEndHandleNum();
    int getBeginHandleNum();

    CHandleNode *m_nodes;
    CHandleNode *m_used;
    u32 m_count;
};

struct CCameraView {
    Vec m_pos;
    Quaternion m_rot;
};

class CCamera {
public:
    void rotateCamera(f32 yaw, f32 pitch);
    void rotateCameraUp(f32 angle);
    void getLookAtVec(Vec *a, Vec *b, Vec *c);
    void getLookAtMtxInverse(Mtx *mtx);
    void applyOffsetLocal(Vec *offset);
    void applyOffset(Vec *offset);
    void resetLookAt();
    void setView(CCameraView *view);
    void getView(CCameraView *view);
    void lerpView(CCameraView *a, CCameraView *b, f32 t);
    void getOffset(Vec *out);
    void setPos(u16 camNo);
    void getPos(u16 camNo);
    void printView();
    static f32 GetStickX(int pad);
    static f32 GetStickY(int pad);
    static f32 GetSubStickX(int pad);
    static f32 GetSubStickY(int pad);
    void debugView(int pad);

    Vec m_pos;    // 0x00
    Vec m_up;     // 0x0C
    Vec m_target; // 0x18
};

/* the original symbol has no float in its signature, keep it as plain C */
extern "C" void getOffsetTo__7CCameraFP3VecP3Vec(CCamera *self, Vec *a, Vec *b, f32 t);

class CGameObject : public CLinkNode {
public:
    static CLinkList ObjectList;
    static s32 s_updateMode;

    virtual void on_new_mode(int mode) = 0;
    virtual void on_update();
    static void SetObjExecMode(int mode);
    static s32 CheckObjExec();
    static void UpdateAll();
    static void Close();
    static s32 GetUpdateMode();

    virtual ~CGameObject();

    s32 m_execMode;

    CGameObject()
    {
        m_execMode = 0;
        insertBack(&ObjectList);
    }
};

class CMinigameCore {
public:
    static CMinigameCore *instance;

    void *operator new(unsigned long size);
    void *operator new[](unsigned long size);
    void operator delete(void *ptr);
    void operator delete[](void *ptr);

    CMinigameCore();
    virtual ~CMinigameCore();
    virtual void on_setup(struct Process_s *objMan) = 0;
    virtual void on_update();
    virtual void on_close() = 0;

    void setup(int prio, int stackSize);
    void actorSetup(int, int);
    static void ObjUpdate(struct omObj_s *obj);
    void update();
    static void MgExit(struct omObj_s *obj);
    static void AddWinner(int player);
    static bool CheckWinner(int player);
    static void StartStartGameMes();
    static void StartFinishGameMes();
    static bool CheckGameMes();
    static void StartWinnerGameMes();
    static void StartRecordGameMes(unsigned int rec);
    static void StartFadeIn();
    static void StartFadeOut();
    static void ExitReq();

    u32 m_unk0;      // 0x00
    s32 m_winner[4]; // 0x08
    s32 m_gameMes;   // 0x18
    s32 m_delay;     // 0x1C
};

typedef void (*HU3DMODELHOOK)(Hu3DModel_s *, Mtx *);

class CModel {
public:
    CModel(void *data);
    CModel(unsigned int dataNum);
    CModel(int charNo, int mode);
    CModel(CModel &src);
    ~CModel();
    void setAttr(unsigned int attr);
    void resetAttr(unsigned int attr);
    void setObjXform(struct omObj_s *obj);
    void setPos(Vec *pos);
    void setRot(Vec *rot);
    void setScale(Vec *scale);
    void setScale(f32 scale);
    void setTPLvl(f32 lvl);
    void setLayer(s16 layer);
    void setCamera(unsigned short camBit);
    Hu3DMtx *getMatrix();
    void setHookData(void *data);
    void *getObjPtr(const char *name);
    void set_shadow();
    void setShadowMap();
    void setShadowMapObj(const char *name);
    void resetShadowMapObj();
    void shadowMapOffObj(char *name);
    void shadowOnObj(char *name);
    void shadowOffObj(char *name);
    void shadowOnlyObjOn(char *name);
    void shadowOnlyObjOff(char *name);
    void setMotTime(int time);

    s16 getId() { return m_id; }

    s16 m_id;
};

/* the original symbol mangles the hook typedef by name, keep it as plain C */
extern "C" CModel *__ct__6CModelF13HU3DMODELHOOK(CModel *self, HU3DMODELHOOK hook);

class CMT19937 {
public:
    CMT19937(int seed = 5489) { setSeed(seed); }
    void setSeed(int seed);
    u32 random();
    u32 irandom();
    f32 nrandom();
    f32 nrandom2();
    f32 nrandomMid();
    double frandom();
    f32 randomMantissa();
    u32 gen();

    u32 m_state[624];
    u32 m_index;
};

/* the original symbol has no parameters in its signature, keep it as plain C */
extern "C" void setSeedDefault__8CMT19937Fv(CMT19937 *self, u32 *key, u32 keyLen);

class COMObj {
public:
    virtual ~COMObj();
    virtual void on_update(struct omObj_s *obj);

    static void OmObjUpdate(struct omObj_s *obj);
    static void OmObjDelete(struct omObj_s *obj);
    static void OmObjDelete(struct Process_s *proc, struct omObj_s *obj);

    COMObj(struct omObj_s *obj);
};

class CThread {
public:
    CThread(void *(*func)(void *), void *arg, u32 stackSize, s32 priority, u16 attr);
    CThread(void *(*func)(void *), void *arg, u32 *stack, u32 stackSize, s32 priority, u16 attr);
    void createThread(void *(*func)(void *), void *arg, u32 *stack, u32 stackWords, s32 priority, u16 attr);
    ~CThread();
    u32 getStackRemain();
    static s32 checkStackOverflow(u32 *stack, int mode);
    static void initStack(u32 *stack, u32 words);

    OSThread m_thread;
    u32 *m_stack;
};

class CSubThread {
public:
    virtual ~CSubThread();
    virtual void thread_main(int arg) = 0;

    CSubThread(unsigned long stackSize);
    static s32 GetStartPriority();
    bool isTerminated();
    s32 wakeupThread(int arg);
    s32 sleepThread(int arg);
    static void *ThreadMain(void *self);

    CThread m_thread;
    OSSemaphore m_sema;
    s32 m_arg;
};

class CTimer {
public:
    CTimer(int type);
    ~CTimer();
    void setParam(int a, int b, int c);
    s32 getValue();
    void setPos(f32 x, f32 y);
    void getPos(f32 *x, f32 *y);
    void setModeOn(int mode);
    void setModeOff();
    void stopTimer();
    void resetTimer();
    bool checkDone();
    void setWinColor(u8 r, u8 g, u8 b);

    struct MgTimer_s *m_timer;
};

namespace m501 {
class CStage;
class CParticle;
class CCameraObj;


class CGameMaster {
public:
    static CStage *stageInstance;
    static CCameraObj *cameraInstance;
};

/* singleton.h: the instance is published as the address of the CSingleton
 * subobject (offset 0x10), walked back to the start of the object. */
#define M501_MEMBER_OFFSET 0x10

/* non-polymorphic, so it contributes no vtable of its own */
class CSingleton {
public:
    CSingleton()
    {
        if (CGameMaster::stageInstance != 0) {
            __assert_fail("ms_pointer == 0", "singleton.h", 0x35);
        } else {
            char *ms_ptr = (char *)this;
            if (ms_ptr != 0) {
                ms_ptr -= M501_MEMBER_OFFSET;
            }
            CGameMaster::stageInstance = (CStage *)ms_ptr;
        }
    }

    ~CSingleton()
    {
        if (CGameMaster::stageInstance == 0) {
            __assert_fail("ms_pointer != 0", "singleton.h", 0x3D);
        } else {
        }

        CGameMaster::stageInstance = (CStage *)0;
    }
};
/* same pattern as CSingleton, but with this class's own instance pointer */
class CParticleSingleton {
public:
    static CParticle *ms_pointer;
    CParticleSingleton()
    {
        if (ms_pointer != 0) {
            __assert_fail("ms_pointer == 0", "singleton.h", 0x35);
        } else {
#ifndef TARGET_PC /* on PC the CParticle constructor publishes itself */
            char *ms_ptr = (char *)this;
            if (ms_ptr != 0) {
                ms_ptr -= M501_MEMBER_OFFSET;
            }
            ms_pointer = (CParticle *)ms_ptr;
#endif
        }
    }

    ~CParticleSingleton()
    {
        if (ms_pointer == 0) {
            __assert_fail("ms_pointer != 0", "singleton.h", 0x3D);
        } else {
        }

        ms_pointer = (CParticle *)0;
    }
};

class CParticle : public CGameObject, public CParticleSingleton {
public:
    CParticle();
    virtual ~CParticle();
    virtual void on_new_mode(int mode);
    virtual void on_update();
    static CModel *getInstance();

    CModel m_model;
};

struct CAsiatoDrawData {
    Mtx m_mtx;
    s16 m_model;
};

class CAsiato : public CLinkNode {
public:
    CAsiato(Vec *pos, f32 rot);
    ~CAsiato();
    static void update(CAsiato *asiato);
    static void drawHook(Hu3DModel_s *model, Mtx *mtx);
    static void drawAsiato(CAsiato *asiato, CAsiatoDrawData *data);
    static void execAsiatoList(CLinkList *list, void (*func)(CAsiato *));
    static void execAsiatoList(CAsiato *node, CAsiato *end, void (*func)(CAsiato *));
    static void drawAsiatoList(CLinkList *list, CAsiatoDrawData *data);
    void drawAsiatoList(CAsiato *end, CAsiatoDrawData *data);

    Vec m_pos;  // 0x08
    f32 m_rot;  // 0x14
    s32 m_life; // 0x18
};

/* the original symbols carry a bogus class name, keep them as plain C */
extern "C" void init__Q24m5017CAsiatoManFv(void);
extern "C" void update__Q24m5017CAsiatoManFv(void);
extern "C" void close__Q24m5017CAsiatoManFv(void);
extern "C" void fn_1_A6C0(Vec *pos, s32 colorIdx);
extern "C" void fn_1_A768(Vec *pos, s32 colorIdx);

/* singleton base of CCameraObj: publishes the instance, no assertions */
class CCameraSingleton {
public:
    CCameraSingleton()
    {
#ifndef TARGET_PC /* on PC the CCameraObj constructor publishes itself */
        char *ms_ptr = (char *)this;
        if (ms_ptr != 0) {
            ms_ptr -= M501_MEMBER_OFFSET;
        }
        CGameMaster::cameraInstance = (CCameraObj *)ms_ptr;
#endif
    }

    ~CCameraSingleton()
    {
        CGameMaster::cameraInstance = (CCameraObj *)0;
    }
};

class CCameraObj : public CGameObject, public CCameraSingleton, public CCamera {
public:
    typedef void (CCameraObj::*PROC)();

    CCameraObj();
    virtual ~CCameraObj();
    virtual void on_new_mode(int mode);
    virtual void on_update();
    void set_focus_player(Vec *pos);
    void proc_idle();
    void proc_null();
    void proc_focus_player();
    void proc_move_up();

    CCameraView m_viewFrom; // 0x34
    CCameraView m_viewTo;   // 0x50
    s32 m_timer;            // 0x6C
    PROC m_proc;            // 0x70
    HU3DMODELID m_model;    // 0x7C
};

}

#endif

