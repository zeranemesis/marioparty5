#ifndef _M530DLL_H
#define _M530DLL_H

#include "game/object.h"


typedef struct m530playerwork_s {
    s16 playerNo; //0x00
    s16 charNo; //0x02
    s16 padNo; //0x04
    s16 side; //0x06
    s16 varNo; //0x08
    s16 padA; //0x0A
    s32 flag; //0x0C
    Vec pos; //0x10
    Vec rot; //0x1C
    s32 state; //0x28
    s32 slot; //0x2C
    s32 unk30; //0x30
    void *itemHook; //0x34
    s32 timer; //0x38
    s32 mode; //0x3C
    s32 timer2; //0x40
    s32 targetNo; //0x44
    Vec target; //0x48
    s16 espId; //0x54
    s16 pad56; //0x56
    Vec screen; //0x58
    Vec world; //0x64
    void *item; //0x70
    s32 unk74; //0x74
    Vec vel; //0x78
    f32 unk84; //0x84
    s32 unk88; //0x88
    Vec goal; //0x8C
    f32 goalSpeed; //0x98
} M530_PLAYER_WORK;

typedef struct m530cell_s {
    s16 no; //0x00
    s16 pad02; //0x02
    Vec start; //0x04
    Vec pos; //0x10
    s16 hookMdl; //0x1C
    s16 efMdl; //0x1E
    s16 shadowMdl; //0x20
    s16 pad22; //0x22
    Vec basePos; //0x24
    Vec vel; //0x30
    s32 state; //0x3C
    s32 kind; //0x40
    s32 variant; //0x44
    s32 wait; //0x48
    s32 timer; //0x4C
} M530_CELL;

extern M530_CELL lbl_1_bss_DC[21];
extern Vec lbl_1_bss_D0;
extern OMOBJ **lbl_1_bss_40;
extern s16 lbl_1_bss_44[4];
extern Vec lbl_1_data_348[2];
extern Vec lbl_1_data_360[2];

typedef struct m530side_s {
    s32 member[2]; //0x00
    s32 score; //0x08
    s32 unkC; //0x0C
} M530_SIDE;

extern M530_SIDE lbl_1_bss_4C[2];

extern HUPROCESS *lbl_1_bss_B8;
typedef struct m530cam_s {
    s32 camNo; //0x00
    s32 state; //0x04
    f32 fov; //0x08
    f32 nearZ; //0x0C
    f32 farZ; //0x10
    Vec pos; //0x14
    Vec up; //0x20
    Vec target; //0x2C
    Vec rot; //0x38
    f32 dist; //0x44
} M530_CAM;

extern M530_CAM lbl_1_data_6C;
extern unsigned int lbl_1_data_C0[15];

/* implemented in REL/m530Dll/m530.c */
void fn_1_439C(Vec *a, Vec *b, Vec *c, Vec *out);
void fn_1_1648(s32 side);
f32 fn_1_342C(s16 padNo);
f32 fn_1_3FF4(f32 cur, f32 target, f32 rate);
s32 fn_1_4164(Vec *pos, Vec *newPos, Vec *other, f32 radius);
void fn_1_10E8(OMOBJ *obj, s32 arg1);
void fn_1_1A50(f32 *pos, s16 *vol, s16 *pan);
void fn_1_6D7C(OMOBJ *obj);
void fn_1_8434(OMOBJ *obj);
void fn_1_1B78(f32 *pos, s16 fxId);
f32 fn_1_4414(Vec *normal, Vec *planePt, Vec *pos);
s32 fn_1_4458(Vec *normal, Vec *planePt, Vec *a, Vec *b, Vec *out);

/* implemented in REL/m530Dll/kinomi.c */
void fn_1_45A0(void);
s32 fn_1_4A18(void);
s32 fn_1_645C(Vec *pos, f32 radius);
s32 fn_1_6518(f32 *pos, f32 radius);
s32 fn_1_65D4(Vec *pos, f32 radius);
s32 fn_1_66D8(s32 cellNo);
void fn_1_6740(void);
void fn_1_6868(void);
void fn_1_694C(s32 cellNo);
void fn_1_6980(s32 cellNo);
void fn_1_69B4(void);
s32 fn_1_6A44(Vec *pos);
s32 fn_1_6B80(f32 *pos, s16 side);
s32 fn_1_6C50(s32 varNo, s32 targetNo);

/* implemented in REL/m530Dll/effect.c */
void fn_1_8848(void);
void fn_1_8958(OMOBJ *obj);

#endif