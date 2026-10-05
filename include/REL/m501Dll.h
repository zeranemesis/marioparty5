#ifndef _M501DLL_H
#define _M501DLL_H

#include "dolphin/types.h"
#include <dolphin/gx/GXStruct.h>

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

class CLinkNode {
public:
    CLinkNode *m_prev;
    CLinkNode *m_next;

    CLinkNode();
    CLinkNode(CLinkNode &);
    ~CLinkNode();
    void insertBack(CLinkList *list);
};

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
}

#endif

