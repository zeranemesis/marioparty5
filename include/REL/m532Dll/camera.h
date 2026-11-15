#ifndef _M532DLL_CAMERA_H
#define _M532DLL_CAMERA_H

// camera.cpp: the module's camera singleton (a task). It owns the live camera
// pose (pos / up / target), a second "free camera" pose edited by man_map.cpp's
// demo object, and a pointer-to-member state function.
#include "dolphin/mtx.h"
#include "REL/m532Dll/obj.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct M532Camera_s {
    M532Task base;     // 0x00
    Vec pos;           // 0x10  (pos, up, target is the M532Cam layout)
    Vec up;            // 0x1C
    Vec target;        // 0x28
    M532Ptmf state;    // 0x34
    s16 camMdl;        // 0x40  camera motion model, -1 if none
    s32 frame;         // 0x44
    u8 pad48[0x80 - 0x48];
    u8 freeCam;        // 0x80
    u8 hold;           // 0x81
    Vec pos2;          // 0x84  (pos2, up2, target2 is the M532Cam layout)
    Vec up2;           // 0x90
    Vec target2;       // 0x9C
} M532Camera;

typedef M532Camera M532CamCtl;

M532Camera *fn_1_96EC(M532Camera *self);
M532Camera *fn_1_9824(M532Camera *self, s16 flag);
M532Camera *fn_1_98B0(M532Camera *self, s16 flag);
void fn_1_997C(M532Camera *self, s32 bit);
void fn_1_9A84(M532Camera *self);

#ifdef __cplusplus
}
#endif

#endif
