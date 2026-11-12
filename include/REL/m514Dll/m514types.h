#ifndef M514TYPES_H
#define M514TYPES_H

#include "types.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/mg/score.h"

/* Player work (OMOBJ.data of the four player objects). */
typedef struct M514Player_s {
    s32 playerNo;
    s32 grp;
    s32 index;
    s32 charNo;
    u32 padDown;
    u32 padBtn;
    f32 stickX;
    f32 stickY;
    s32 isCom;
    s32 comDif;
    s32 unk28;
    s32 comTimer;
    s32 state;
    s32 stateTimer;
    s32 frameNo;
    s32 motNo;
    HuVecF pos;
    HuVecF vel;
    f32 rotY;
    f32 unk5C;
    f32 unk60;
    s32 unk64;
    s32 active;
    s32 hold;
    s32 landWait;
    s32 moving;
    s32 stepSide;
    s32 runState;
    s32 runTimer;
    s32 jumpNo;
    s32 jumping;
    s32 finished;
    s32 effectId;
    s32 sfxId;
} M514PLAYER;

/* Camera work (OMOBJ.data of the camera object). */
typedef struct M514Cam_s {
    s32 active;
    s16 motId;
    s16 camMdl;
} M514CAM;


/* Scenery objects (object.c): table entry, per-object work, manager work. */
typedef struct M514ObjMot_s {
    s32 dataNum;
    f32 shift;
    u32 attr;
} M514OBJMOT;

typedef struct M514ObjDef_s {
    s32 kind;
    s32 idx;
    s32 modelNum;
    M514OBJMOT *mot;
    HuVecF *pos;
    f32 rotY;
    OMOBJFUNC func;
} M514OBJDEF;

typedef struct M514Obj_s {
    M514OBJDEF *def;
    s32 state;
    s32 timer;
    s32 unkC;
    s32 motNo;
    s32 sfx1;
    s32 sfx2;
    HuVecF pos;
    HuVecF vel;
    HuVecF rot;
    s32 unk40;
    s32 snapshot;
} M514OBJ;

typedef struct M514ObjMgr_s {
    s32 flag;
} M514OBJMGR;

/* m514.c */
void ObjectSetup(void);
void fn_1_3A0(void);
void fn_1_448(s16 mode, s16 frameNo);
void fn_1_540(s16 mode, s16 frameNo);
void fn_1_580(void);
void fn_1_600(s16 mode, s16 frameNo);
void fn_1_7B0(s16 mode, s16 frameNo);
void fn_1_8A0(s16 mode, s16 frameNo);
void fn_1_958(void);
void fn_1_95C(void);
s16 fn_1_16DC(void);
s16 fn_1_16EC(s32 idx);
s32 fn_1_1704(s32 grp);
s32 fn_1_1814(void);
void fn_1_1824(s32 playerNo);
void fn_1_1838(s32 idx, s32 grp, s32 charNo);
void fn_1_1870(s32 idx);
void fn_1_1890(s32 playerNo);
s32 fn_1_1904(f32 x);
s32 fn_1_1960(HuVecF *pos);
s32 fn_1_19F4(void);
void fn_1_1A18(s32 seId, HuVecF *pos);
void fn_1_1AF4(s32 seId, HuVecF *pos);
f32 fn_1_1BCC(f32 from, f32 to, f32 rate);
f32 fn_1_1CE8(f32 from, f32 to);

/* player.c */
void fn_1_1D9C(OMOBJMAN *objman);
void fn_1_1EA0(void);
s32 fn_1_41C4(s32 grp);
f32 fn_1_4238(s32 idx);

/* object.c */
void fn_1_4270(OMOBJMAN *objman);
void fn_1_4344(void);
s32 fn_1_50E4(void);

/* effect.c */
void fn_1_5B04(OMOBJMAN *objman);
void fn_1_5B8C(void);
s32 fn_1_79F8(HuVecF *pos, f32 size);
void fn_1_7B14(s32 id);
void fn_1_7B8C(s32 id);
void fn_1_7C10(s32 id, HuVecF *pos);
void fn_1_80C0(HuVecF *pos, f32 size);
void fn_1_8180(HuVecF *pos);
void fn_1_8228(s16 bank);

/* score.c */
void fn_1_8268(OMOBJMAN *objman);
void fn_1_8300(void);
void fn_1_9334(void);
void fn_1_9570(s32 idx, f32 val);
s32 fn_1_95BC(s32 idx);
f32 fn_1_95F4(s32 idx);
f32 fn_1_9688(void);
f32 fn_1_9794(void);
s32 fn_1_9820(void);
void fn_1_98F4(void);

#endif
