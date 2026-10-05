// mdcardDll/mdcard - decompilation of the mdcardDll/mdcard unit (46528 B, 39 fns).
// _prolog, _epilog, MdMain, fn_1_9A0, fn_1_A204 and fn_1_942C match; the rest are absent.

#include "types.h"
#include "game/object.h"
#include "game/process.h"

typedef void (*VoidFunc)(void);
typedef void (*MD_FUNC)(void);

extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* sys + 0x98: the pad-input message state block (mode at +0, choice at +2). */
typedef struct MdcardMsg_s {
    s16 mode;   //0x00
    s16 choice; //0x02
} MdcardMsg;

typedef struct Sys_s {
    u8 pad0[0x0C];
    s32 counter;  //0x0C
    s32 flowMode; //0x10
    u8 pad14[8];
    s8 exitF;     //0x1C
    u8 pad1D[0x63];
    void *unk80;  //0x80
    u8 pad84[8];
    void *mess;   //0x8C
    u8 pad90[4];
    void *unk94;  //0x94
    MdcardMsg msg; //0x98
    u8 pad9C[0x338];
} SYS;

SYS sysWork;
SYS *sys;
SYS *sysP;

MD_FUNC MdFlowTbl[6];

f32 lbl_1_rodata_DC = 32.0f;
f32 lbl_1_rodata_104 = 0.1111111119389534f;
f64 lbl_1_rodata_98 = 3.14159265358979323846;
f64 lbl_1_rodata_A0 = 180.0;

void ObjectSetup(void);
void MdInit(void);
void MdClose(void);
void fn_1_84AC(void);
void Hu3D3Dto2D(f32 *src, u16 cameraBit, f32 *dst);
f64 atan2(f64 y, f64 x);
s16 MdMessWinChoiceGet(void *mess, int winNo);

int _prolog(void)
{
    const VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void MdMain(void)
{
    MdInit();
    while (omSysExitReq == 0 && sys->exitF == 0) {
        MdFlowTbl[sys->flowMode]();
        fn_1_84AC();
        if (++sys->counter > 36000) {
            sys->counter = 0;
        }
        HuPrcVSleep();
    }
    MdClose();
}

void fn_1_9A0(void)
{
    void *win;
    MdcardMsg *msg;

    win = sys->mess;
    msg = &sys->msg;
    while (1) {
        win = sys->mess;
        msg = &sys->msg;
        switch (msg->mode) {
        case 0:
            break;
        case 1:
            msg->choice = MdMessWinChoiceGet(win, 1);
            msg->mode = 2;
            break;
        case 2:
            break;
        }
        HuPrcVSleep();
    }
}
s32 fn_1_A204(s32 arg0, s32 arg1)
{
    f32 dst[3];
    u16 *ent;
    void *b94;
    void *b80;
    void *obj;
    s32 ret;

    b80 = sys->unk80;
    ent = (u16 *)((u8 *)b80 + arg1 * 0x88 + 8);
    b94 = sys->unk94;
    obj = *(void * const *)((u8 *)b94 + arg0 * 4 + 0xA8);
    Hu3D3Dto2D((f32 *)obj, *ent, dst);
    ret = (s32)(lbl_1_rodata_DC + lbl_1_rodata_104 * dst[0]);
    return ret;
}

void fn_1_942C(s32 arg0, s32 arg1)
{
    f32 d[3];
    u8 *ent;
    void *obj1;
    void *b94;
    void *obj2;
    void *b80;
    f32 out;

    b80 = sys->unk80;
    ent = (u8 *)b80 + arg1 * 0x88 + 8;
    b94 = sys->unk94;
    obj1 = *(void * const *)((u8 *)b94 + arg0 * 4 + 0xA8);
    obj2 = *(void * const *)((u8 *)b94 + arg0 * 4 + 0x144);
    d[0] = ((f32 *)ent)[2] - ((f32 *)obj1)[0];
    d[1] = ((f32 *)ent)[3] - ((f32 *)obj1)[1];
    d[2] = ((f32 *)ent)[4] - ((f32 *)obj1)[2];
    out = (f32)(atan2(d[0], d[2]) / lbl_1_rodata_98 * lbl_1_rodata_A0);
    ((f32 *)obj2)[1] = out;
}

