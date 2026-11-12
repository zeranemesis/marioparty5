#ifndef _REL_M569DLL_H
#define _REL_M569DLL_H

#include "types.h"
#include "game/object.h"
#include "game/process.h"
#include "game/hu3d.h"
#include "game/mg/timer.h"
#include "game/mg/score.h"

/* Camera descriptor (0x48 bytes), shared by the camera helpers in m569.c. */
typedef struct m569Cam_s {
    /* 0x00 */ s32 camNo;
    /* 0x04 */ s32 state;
    /* 0x08 */ f32 fov;
    /* 0x0C */ f32 nearZ;
    /* 0x10 */ f32 farZ;
    /* 0x14 */ HuVecF pos;
    /* 0x20 */ HuVecF up;
    /* 0x2C */ HuVecF target;
    /* 0x38 */ HuVecF rot;
    /* 0x44 */ f32 dist;
} M569CAM;

/* Light descriptor passed to Hu3DGLightCreateV. */
typedef struct m569Light_s {
    /* 0x00 */ HuVecF pos;
    /* 0x0C */ HuVecF dir;
    /* 0x18 */ GXColor color;
} M569LIGHT;

/* Per-player state (0x70 bytes), OMOBJ::data of the player objects. */
typedef struct m569Player_s {
    /* 0x00 */ s16 objId;
    /* 0x02 */ s16 charNo;
    /* 0x04 */ s16 padNo;
    /* 0x06 */ s16 grp;
    /* 0x08 */ s16 dif;
    /* 0x0A */ s16 padA;
    /* 0x0C */ s32 type;
    /* 0x10 */ HuVecF pos;
    /* 0x1C */ HuVecF rot;
    /* 0x28 */ s32 state;
    /* 0x2C */ s32 slot;
    /* 0x30 */ s32 motNo;
    /* 0x34 */ s16 pad34;
    /* 0x36 */ s16 jointMot[8];
    /* 0x46 */ s16 row;
    /* 0x48 */ s32 action;
    /* 0x4C */ s32 coin;
    /* 0x50 */ HuVecF goal;
    /* 0x5C */ HuVecF ang;
    /* 0x68 */ s16 aiRow;
    /* 0x6A */ s16 aiWait;
    /* 0x6C */ s32 voiceF;
} M569PLAYER;

/* Shared state of one team table entry (0x10 bytes). */
typedef struct m569Team_s {
    /* 0x00 */ s32 score;
    /* 0x04 */ s32 member[3];
} M569TEAM;

/* Big kuribo (0x40 bytes). */
typedef struct m569Kuribo_s {
    /* 0x00 */ s32 flag;
    /* 0x04 */ HuVecF pos;
    /* 0x10 */ HuVecF rot;
    /* 0x1C */ f32 unk1C[4];
    /* 0x2C */ s32 motNo;
    /* 0x30 */ f32 prevTime;
    /* 0x34 */ s32 jointMot[3];
} M569KURIBO;

/* Audience kuribo (0x40 bytes). */
typedef struct m569Spect_s {
    /* 0x00 */ s32 state;
    /* 0x04 */ s32 angle;
    /* 0x08 */ s32 dir;
    /* 0x0C */ HuVecF pos;
    /* 0x18 */ HuVecF ofs;
    /* 0x24 */ s32 pad24[7];
} M569SPECT;

/* Block (0x30 bytes). */
typedef struct m569Block_s {
    /* 0x00 */ u8 type;
    /* 0x04 */ s32 state;
    /* 0x08 */ HuVecF pos;
    /* 0x14 */ s16 rotY;
    /* 0x18 */ f32 scale;
    /* 0x1C */ f32 bob;
    /* 0x20 */ u8 mot[6];
    /* 0x26 */ u8 motIdx;
    /* 0x27 */ u8 motCnt;
    /* 0x28 */ s32 timer;
    /* 0x2C */ s16 parId;
} M569BLOCK;

/* m569.c */
extern s32 lbl_1_bss_14;
extern OMOBJMAN *lbl_1_bss_D4;
extern M569CAM lbl_1_bss_34;
extern OMOBJ **lbl_1_bss_F8;
extern M569TEAM lbl_1_bss_D8[2];
extern HuVecF lbl_1_data_D8[5];

s32 fn_1_1014(HuVecF *pos);
void fn_1_109C(HuVecF *pos, s16 seId);
void fn_1_1288(M569CAM *cam);
void fn_1_32AC(M569PLAYER *player, s16 idx);
s32 fn_1_3378(HuVecF *pos, HuVecF *goal, HuVecF *angle, f32 step);
f32 fn_1_349C(f32 val, f32 target, f32 max);
s32 fn_1_360C(HuVecF *pos, HuVecF *goal, HuVecF *from, f32 dist);

/* player.c */
void fn_1_3A48(void);
void fn_1_454C(OMOBJ *obj);
void fn_1_4D74(void);
void fn_1_4B8C(OMOBJ *obj, s32 motNo);
void fn_1_4EB4(s32 idx, s32 state);
s32 fn_1_4EE4(s32 idx);
void fn_1_4F14(s32 idx);
void fn_1_5138(s32 idx);
void fn_1_52F8(s32 idx);
void fn_1_54A8(s32 team);

/* kuribo.c */
M569KURIBO *fn_1_582C(void);
void fn_1_6D9C(M569KURIBO *kuribo);
void fn_1_6DB4(void);
void fn_1_735C(void);
void fn_1_741C(void);
void fn_1_74B0(void);

/* block.c */
extern HuVecF lbl_1_data_32C[3];
void fn_1_80AC(void);
void fn_1_80FC(void);
void fn_1_833C(void);
void fn_1_839C(s32 idx);
s32 fn_1_848C(s32 idx);
s32 fn_1_84A8(void);
s32 fn_1_8544(void);
s32 fn_1_85C0(void);
void fn_1_8614(void);
s32 fn_1_86C4(void);

/* score.c */
void fn_1_87D4(void);
void fn_1_8A74(s32 idx, s32 value);
s32 fn_1_8AAC(s32 idx);
void fn_1_8ADC(void);
void fn_1_9B2C(void);

#endif
