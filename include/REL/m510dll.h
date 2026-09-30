#ifndef _M510DLL_H
#define _M510DLL_H

#include "dolphin.h"
#include "game/hu3d.h"
#include "humath.h"

/* One of the view objects the opening drives: the camera and three model slots.
 * The opening moves each one, then raises ->showF when the sequence reaches the
 * frame that reveals it. */
typedef struct M510ViewObj_s {
    /* 0x000 */ u8 pad000[0xB8];
    /* 0x0B8 */ f32 offset;
    /* 0x0BC */ u8 pad0BC[0x44];
    /* 0x100 */ s32 showF;
} M510_VIEWOBJ;

/* A view slot: an eight byte header, then the object the opening animates. */
typedef struct M510View_s {
    /* 0x000 */ u8 pad000[0x08];
    /* 0x008 */ M510_VIEWOBJ obj;
    /* 0x10C */ u8 pad10C[0x774];
} M510_VIEW;

/* The block the opening slides across the screen. */
typedef struct M510Slide_s {
    /* 0x00 */ u8 pad00[0x0C];
    /* 0x0C */ Vec from;
    /* 0x18 */ f32 x;
    /* 0x1C */ f32 y;
    /* 0x20 */ f32 z;
    /* 0x24 */ Vec to;
    /* 0x30 */ f32 x2;
    /* 0x34 */ f32 y2;
    /* 0x38 */ f32 z2;
    /* 0x3C */ u8 pad3C[0x24];
    /* 0x60 */ f32 fade;
} M510_SLIDE;

/* A player slot, reset when the third phase ends. */
typedef struct M510Slot_s {
    /* 0x00 */ u8 pad00[0x20];
    /* 0x20 */ s16 state;
    /* 0x22 */ u8 pad22[0x02];
    /* 0x24 */ s32 activeF;
} M510_SLOT;

/* The module's work structure: the argument of the opening function. */
typedef struct M510Work_s {
    /* 0x000 */ u8 pad000[0x0E];
    /* 0x00E */ u16 phase;
    /* 0x010 */ u8 pad010[0x04];
    /* 0x014 */ s32 frame;
    /* 0x018 */ s32 time;
    /* 0x01C */ M510_VIEW view0;
    /* 0x89C */ M510_VIEW view1;
    /* 0x111C */ M510_VIEW view2;
    /* 0x199C */ M510_VIEW view3;
    /* 0x221C */ u8 pad221C[0x258];
    /* 0x2474 */ M510_SLIDE slide;
    /* 0x24D8 */ u8 pad24D8[0x60];
    /* 0x2538 */ s32 fieldId;
} M510_WORK;

s32 fn_1_734(s32 arg0);

#endif
