#ifndef _M501DLL_H
#define _M501DLL_H

#include "dolphin/types.h"
#include <dolphin/gx/GXStruct.h>
#include <dolphin/mtx.h>

/* Local copies of the small support classes m501.cpp defines: CGameObject's
 * constructor has to be visible here so a derived constructor can inline it,
 * and CModel is only ever used through its 2-byte model handle. */

extern "C" void __assert_fail(const char *expr, const char *file, int line);

typedef s16 HU3DMODELID;
typedef s16 HU3DMOTID;

struct Hu3DMtx {
    f32 m[4][3];
};

class CLinkList;

class CRandom {
public:
    static float frandom(float min, float max);
};

class CLinkNode {
public:
    CLinkNode *m_prev;
    CLinkNode *m_next;

    CLinkNode();
    CLinkNode(CLinkNode &);
    ~CLinkNode();
    void insertBack(CLinkList *list);
};

class CLinkList : public CLinkNode {};

class CGameObject : public CLinkNode {
public:
    static CLinkList ObjectList;

    virtual void on_new_mode(int mode) = 0;
    virtual void on_update() {}
    void SetObjExecMode(s32 mode);

    virtual ~CGameObject() {}

    s32 m_execMode;

    CGameObject()
    {
        m_execMode = 0;
        insertBack(&ObjectList);
    }
};

class CModel {
public:
    CModel(unsigned int dataNum);
    ~CModel();
    Hu3DMtx *getMatrix();
    void setHookData(void *data);

    s16 getId() { return m_id; }

    s16 m_id;
};

class COMObj {
public:
    virtual ~COMObj() {}
    virtual void OmObjUpdate(struct omObj_s *obj);
    virtual void on_update(struct omObj_s *obj);
    virtual void OmObjDelete(struct omObj_s *obj);

    static void OmObjUpdateStatic(struct omObj_s *obj);

    COMObj(struct omObj_s *obj);
};

class CSubThread {
public:
    virtual ~CSubThread() {}
    virtual void *ThreadMain(void *arg);

    CSubThread(unsigned long stackSize);
    s32 GetStartPriority();
};

namespace m501 {
class CStage;
class CParticle;


class CGameMaster {
public:
    static CStage *stageInstance;
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
    static void drawHook(struct Hu3DModel_s *model, Mtx *mtx);
    static void drawAsiato(CAsiato *asiato, CAsiatoDrawData *data);
    static void execAsiatoList(CLinkList *list, void (*func)(CAsiato *));
    static void execAsiatoList(CAsiato *node, CAsiato *end, void (*func)(CAsiato *));
    static void drawAsiatoList(CLinkList *list, CAsiatoDrawData *data);
    static void drawAsiatoList(CAsiato *node, CAsiato *end, CAsiatoDrawData *data);

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
}

#endif

