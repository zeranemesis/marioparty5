#ifndef _M523DLL_H
#define _M523DLL_H

#include "game/object.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/hsfex.h"
#include "game/gamemes.h"
#include "game/wipe.h"
#include "game/audio.h"
#include "game/memory.h"
#include "game/pad.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"

#include "humath.h"
#include "msm.h"

#include "datanum/m523.h"

typedef struct M523Camera_s {
    s32 camNo; // 0x00
    s32 mode; // 0x04
    f32 nearZ; // 0x08
    f32 fov; // 0x0C
    f32 farZ; // 0x10
    Vec pos; // 0x14
    Vec up; // 0x20
    Vec target; // 0x2C
    f32 angX; // 0x38
    f32 angY; // 0x3C
    f32 angZ; // 0x40
    f32 dist; // 0x44
} M523Camera;

typedef struct M523CamMove_s {
    M523Camera *cam; // 0x00
    M523Camera start; // 0x04
    M523Camera end; // 0x4C
    f32 totalTime; // 0x94
    f32 time; // 0x98
    s32 moveF; // 0x9C
} M523CamMove;

typedef void (*M523FlowFunc)(OMOBJ *obj, void *flow);

typedef struct M523Flow_s {
    s32 index; // 0x00
    M523FlowFunc *funcTbl; // 0x04
    s32 funcNum; // 0x08
    s16 gameMesId; // 0x0C
    s16 pad0E; // 0x0E
    MGTIMER *timer; // 0x10
    s32 timerType; // 0x14
    s32 maxTime; // 0x18
    s32 endTime; // 0x1C
    s32 recordTime; // 0x20
    s32 pad24; // 0x24
    s32 work[16]; // 0x28
} M523Flow;

typedef struct M523Light_s {
    Point3d pos;
    Point3d target;
    GXColor color;
} M523Light;

typedef struct M523Cyl_s {
    struct M523Cyl_s *next; // 0x00
    struct M523Cyl_s *prev; // 0x04
    s16 model; // 0x08
    s16 pad0A; // 0x0A
    Vec pos; // 0x0C
    s16 index; // 0x18
    s16 timer; // 0x1A
    s16 state; // 0x1C
    s16 pad1E; // 0x1E
    s32 killF; // 0x20
} M523Cyl;

typedef struct M523Player_s {
    s16 index; // 0x00
    s16 charNo; // 0x02
    s16 padNo; // 0x04
    s16 grp; // 0x06
    s16 dif; // 0x08
    s16 pad0A; // 0x0A
    s32 type; // 0x0C
    Vec pos; // 0x10
    Vec rot; // 0x1C
    s32 state; // 0x28
    OMOBJ *obj; // 0x2C
    s16 teamNo; // 0x30
    s16 motNo; // 0x32
    f32 radius; // 0x34
    s8 stkX; // 0x38
    s8 stkY; // 0x39
    Vec vel; // 0x3C
    Vec dest; // 0x48
    s32 doneF; // 0x54
    s16 hitNo; // 0x58
    s16 pad5A; // 0x5A
    Vec prevPos; // 0x5C
    Vec spawnPos; // 0x68
    Vec velSave; // 0x74
    s16 hitTimer; // 0x80
    s16 shockTimer; // 0x82
    s32 shockF; // 0x84
    s32 shockSe; // 0x88
    M523Cyl *cyl; // 0x8C
    s16 fuseModel; // 0x90
    s16 fuseMot; // 0x92
    s16 fuseMotWin; // 0x94
    s16 fuseMotLose; // 0x96
    s16 fuseMotHit; // 0x98
    s16 pad9A; // 0x9A
    s32 aiTimer; // 0x9C
    s16 padA0; // 0xA0
    s16 aiState; // 0xA2
    Vec aiTarget; // 0xA4
} M523Player;

extern M523Player *lbl_1_bss_13C;
extern M523Player *lbl_1_bss_140[4];

void fn_1_39DC(void);
void fn_1_405C(void);
void fn_1_4858(void);
void fn_1_7C84(OMOBJ *obj);


#endif
