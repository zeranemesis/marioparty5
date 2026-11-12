#ifndef _M564DLL_H
#define _M564DLL_H

#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/init.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/wipe.h"

#include "game/mg/actman.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"

#include "math.h"
#include "string.h"

/* camera state shared by the minigame camera helpers (size 0x48) */
typedef struct m564cam_s {
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
} M564_CAM;

/* player work, obj->data of the 4 player objects */
typedef struct m564player_s {
    s16 playerNo; //0x00
    s16 charNo; //0x02
    s16 padNo; //0x04
    s16 side; //0x06
    s16 varNo; //0x08
    s16 pad0A; //0x0A
    s32 flag; //0x0C
    Vec pos; //0x10
    Vec rot; //0x1C
    s32 state; //0x28
    MGPLAYER *mgPlayer; //0x2C
    char *itemHook; //0x30
    s32 carryF; //0x34
    s16 bombNo; //0x38
    s16 carryTime; //0x3A
    s32 unk3C; //0x3C
    Vec comTarget; //0x40
    s32 pad4C[3]; //0x4C
    s16 comMode; //0x58
    s16 pad5A; //0x5A
    s16 comBomb; //0x5C
    s16 comPlayer; //0x5E
    s16 comTimer; //0x60
    s16 pad62; //0x62
    s32 stkX; //0x64
    s32 stkY; //0x68
    s32 btn; //0x6C
    s32 unk70; //0x70
} M564_PLAYER;

/* bomb work (size 0x5C) */
typedef struct m564bomb_s {
    MGACTOR *actor; //0x00
    s16 mdlId; //0x04
    s16 shadowMdl; //0x06
    s16 mot[3]; //0x08
    s16 curMot; //0x0E
    s16 no; //0x10
    s16 fxMdl; //0x12
    s16 kind; //0x14
    s16 target; //0x16
    s16 spot; //0x18
    s16 holder; //0x1A
    f32 flip; //0x1C
    s16 state; //0x20
    s16 timer; //0x22
    s16 wait; //0x24
    s16 explTime; //0x26
    f32 speed; //0x28
    s16 moveTime; //0x2C
    s16 pad2E; //0x2E
    f32 holdRot; //0x30
    Vec explPos; //0x34
    Vec pos; //0x40
    Vec dest; //0x4C
    s32 comF; //0x58
} M564_BOMB;

typedef struct m564spot_s {
    s16 mdlId; //0x00
    s16 state; //0x02
    Vec pos; //0x04
    s16 bombNo; //0x10
    s16 timer; //0x12
} M564_SPOT;

typedef struct m564light_s {
    Vec pos; //0x00
    Vec dir; //0x0C
    GXColor color; //0x18
} M564_LIGHT;

typedef struct m564cammove_s {
    M564_CAM *cam; //0x00
    M564_CAM start; //0x04
    M564_CAM end; //0x4C
    f32 totalTime; //0x94
    f32 time; //0x98
    s32 moveF; //0x9C
} M564_CAMMOVE;

typedef void (*M564_FLOWFUNC)(OMOBJ *obj, void *tbl);

typedef struct m564flow_s {
    s32 index; //0x00
    M564_FLOWFUNC *funcTbl; //0x04
    s32 funcNum; //0x08
    s16 gameMesId; //0x0C
    s16 pad0E; //0x0E
    MGTIMER *timer; //0x10
    s32 timerType; //0x14
    s32 maxTime; //0x18
    s32 endTime; //0x1C
    s32 recordTime; //0x20
    s32 pad24; //0x24
    s32 work[16]; //0x28
} M564_FLOW;

#define M564_PI 3.141592653589793
#define M564_RAD(x) ((M564_PI * (f64)(x)) / 180.0)

/* not declared by the shared headers */
void MgActorColMaskSet(MGACTOR *actorP, u32 mask);
void MgActorPosGet(MGACTOR *actorP, HuVecF *pos);
float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

/* m564.c */
extern s32 lbl_1_bss_24;
extern M564_CAM lbl_1_bss_44;
extern s16 lbl_1_bss_8C[16];
extern OMOBJMAN *lbl_1_bss_F8;
extern M564_PLAYER *lbl_1_bss_100[4];
extern Vec lbl_1_data_D8;
void fn_1_118C(Vec *center, Vec *out, f32 min, f32 max);
f32 fn_1_12D4(Vec *from, Vec *to);
f32 fn_1_1338(f32 a, f32 b);
f32 fn_1_13A4(Vec *a, Vec *b);
f32 fn_1_13F0(Vec *a, Vec *b, Vec *out);
s32 fn_1_1448(Vec *pos);
void fn_1_14D0(Vec *pos, s16 seId);
void fn_1_36E0(M564_PLAYER *work, s16 playerNo);
s32 fn_1_37AC(Vec *pos, Vec *target, Vec *angle, f32 step);
f32 fn_1_38D0(f32 cur, f32 target, f32 rate);
OMOBJ *MgCameraMoveEntry(M564_CAM *cam, M564_CAM *dest, f32 time);

/* player.c */
extern OMOBJ **lbl_1_bss_110;
void fn_1_3E7C(void);
void fn_1_43E0(MGPLAYER *player, s16 mode);
s32 fn_1_5E5C(s16 playerNo);
void fn_1_5EA8(s16 playerNo, Vec *pos);
void fn_1_5F08(s16 playerNo);
s16 fn_1_5FDC(Vec *pos);
void fn_1_4204(MGPLAYER *player, s16 motNo);

/* bomb.c */
extern M564_BOMB *lbl_1_bss_1E8[15];
void fn_1_60E8(void);
void fn_1_653C(void);
void fn_1_6858(void);
void fn_1_7210(M564_BOMB *bomb, s32 sound);
s32 fn_1_6404(void);
s16 fn_1_8340(Vec *pos, f32 rotY);
s32 fn_1_8558(Vec *pos, f32 a, f32 b);
void fn_1_86D8(s16 bombNo, s16 playerNo);
void fn_1_87B4(s16 bombNo, s16 playerNo);
void fn_1_89EC(s16 bombNo);
s32 fn_1_8D60(s16 bombNo);
void fn_1_8DEC(void);
s32 fn_1_8EC8(void);
void fn_1_8F6C(void);
s32 fn_1_902C(Vec *pos, Vec *out);
void fn_1_7404(M564_BOMB *bomb);

/* com.c */
void fn_1_9AC0(M564_PLAYER *work);
void fn_1_9ADC(M564_PLAYER *work);

#endif
