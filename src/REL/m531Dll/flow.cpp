#include "dolphin/types.h"

extern "C" {
extern s32 lbl_1_bss_10;
extern s32 lbl_1_bss_14;
extern s32 lbl_1_bss_18;

void fn_1_56CC(void);
void fn_1_4BC0(void *base, s32 mode, s32 idx1, s32 idx2, f32 value);
f32 fn_1_4B6C(void *base, s32 mode, s32 idx1, s32 idx2);
}

void fn_1_56CC(void)
{
    lbl_1_bss_10 = 0x78;
    lbl_1_bss_14 = 0xD2;
    lbl_1_bss_18 = 0xD2;
}

#pragma opt_level 4
void fn_1_4BC0(void *base, s32 mode, s32 idx1, s32 idx2, f32 value)
{
    if (mode == 0) {
        ((f32 *)((u8 *)base + idx2 * 0x14 + idx1 * 4 + 0x54))[0] = value;
    } else {
        ((f32 *)((u8 *)base + idx2 * 0x14 + idx1 * 4 + 0x11C))[0] = value;
    }
}

f32 fn_1_4B6C(void *base, s32 mode, s32 idx1, s32 idx2)
{
    if (mode == 0) {
        return ((f32 *)((u8 *)base + idx2 * 0x14 + idx1 * 4 + 0x54))[0];
    } else {
        return ((f32 *)((u8 *)base + idx2 * 0x14 + idx1 * 4 + 0x11C))[0];
    }
}
#pragma opt_level 0

// Forward declarations for types used in thunks
struct OMOBJMAN;
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
    void (*objFunc)(omObj_s *obj);
    f32 trans[3];
    f32 rot[3];
    f32 scale[3];
    u16 mdlcnt;
    void *mdlId;
    u16 mtncnt;
    void *mtnId;
    u32 work[4];
    void *data;
};

// Object type with vptr at offset 0x4C (76 bytes of data + vptr)
struct FlowSubObj {
    u8 pad[0x4C];
    virtual void vfunc_2();
    virtual void vfunc_3();
};

extern "C" {
void fn_1_2CA0(omObj_s *obj);
void fn_1_2CE8(omObj_s *obj);
}

void fn_1_2CA0(omObj_s *obj)
{
    FlowSubObj *self = (FlowSubObj *)obj->data;
    self->vfunc_2();
}

void fn_1_2CE8(omObj_s *obj)
{
    FlowSubObj *self = (FlowSubObj *)obj->data;
    self->vfunc_3();
}

namespace m531 {
typedef struct Process_s OMOBJMAN;
class CMinigameCore {
public:
    virtual ~CMinigameCore();
    virtual void on_setup(OMOBJMAN *objMan);
    virtual void on_update();
    virtual void on_close();
};

class CGameMaster : public CMinigameCore {
public:
    virtual ~CGameMaster();
    virtual void on_setup(OMOBJMAN *objMan);
    virtual void mode0();
    virtual void mode1();
    virtual void mode2();
    virtual void mode3();
    virtual void mode4();
    virtual void mode5();
    virtual void mode6();
};
}