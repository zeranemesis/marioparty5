#include "game/object.h"
#include "game/hu3d.h"
#include "dolphin/os/OSThread.h"

extern s16 omSysExitReq;

extern u32 lbl_1_data_1F0[4];

extern "C" void fn_1_29D4(void);
extern "C" void fn_1_2A94(void);
extern "C" void fn_1_2A20(void);

typedef struct {
    u8 pad[0xF0];
    f32 scale[3];
    u8 pad2[0x24];
    u32 unk_120;
    u8 pad3[0x10];
} M509_HU3DMODEL;

extern "C" void fn_1_3874(void) {
    omSysExitReq = 1;
}

extern "C" void fn_1_3FE4(s16 *arg0, u32 arg1) {
    *(u32 *)((u8 *)Hu3DData + *arg0 * 0x134 + 0x120) = arg1;
}

extern "C" s32 fn_1_4000(s16 *arg0) {
    return (s32)((u8 *)Hu3DData + *arg0 * 0x134 + 0xF0);
}

extern "C" void fn_1_29B4(u32 *arg0, u32 *arg1) {
    *arg0 = (u32)lbl_1_data_1F0;
    arg1[0x5C/4] = (u32)arg0;
    arg1[0x14/4] = (u32)fn_1_29D4;
}

extern "C" void fn_1_7A4(void) {
    OSThread *thread = OSGetCurrentThread();
    OSGetThreadPriority(thread);
}

extern "C" void fn_1_EC0(u32 *arg0) {
    u32 next = arg0[1];
    u32 prev = arg0[0];
    ((u32 *)prev)[1] = next;
    ((u32 *)next)[0] = prev;
    arg0[0] = (u32)arg0;
    arg0[1] = (u32)arg0;
}