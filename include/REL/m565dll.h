#ifndef _M565DLL_H
#define _M565DLL_H

#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
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

#include "math.h"
#include "string.h"

typedef struct M565Game_s M565Game;
typedef struct M565Sys_s M565Sys;
typedef struct M565Tile_s M565Tile;
typedef struct M565Stage_s M565Stage;
typedef struct M565Node_s M565Node;
typedef struct M565Puku_s M565Puku;
typedef struct M565PukuItem_s M565PukuItem;

/* intrusive doubly linked list (fn_1_0 .. fn_1_E4) */
typedef struct M565List_s {
    s32 count;
    M565Node *head;
    M565Node *tail;
} M565List;

struct M565Node_s {
    M565Tile *data;
    M565Node *next;
    M565Node *prev;
    M565List *list;
};

struct M565Tile_s {
    M565Stage *stage; // 0x00
    u16 state; // 0x04: 0 hole, 1 ?, 0x64 solid, 0x65 warning, 0x66 falling
    s16 kind; // 0x06
    u8 pad_08[4];
    s16 timer; // 0x0C
    s16 col; // 0x0E
    s16 row; // 0x10
    s16 mdl; // 0x12
    s16 mdl2; // 0x14
    u8 pad_16[2];
    HuVecF pos; // 0x18
    f32 fallY; // 0x24
    f32 fallVel; // 0x28
    s16 neighbors; // 0x2C
    u8 pad_2E[2];
    s32 visited; // 0x30
    s32 hitFlag; // 0x34
    M565PukuItem *hitItem; // 0x38
    M565Node nodeFall; // 0x3C
    M565Node nodeCol; // 0x4C
    M565Node nodeFree; // 0x5C
    M565Node nodeGroup; // 0x6C
};

typedef struct M565StageSlot_s {
    s32 active; // 0x00
    s16 mdl; // 0x04
    s16 mot; // 0x06
} M565StageSlot;

struct M565Stage_s {
    M565Sys *sys; // 0x0000
    M565Tile tiles[10][10]; // 0x0004 [row][col]
    M565StageSlot slots[10]; // 0x3074
    M565Tile *pick1; // 0x30C4
    M565Tile *pick2; // 0x30C8
    s32 pickFlag; // 0x30CC
    s32 fallFlag; // 0x30D0
    M565List fallList; // 0x30D4
    M565List colList; // 0x30E0
    M565List freeList; // 0x30EC
    M565List groups[50]; // 0x30F8
    u16 groupNum; // 0x3350
    u16 groupMax; // 0x3352
    s32 sndHandle; // 0x3354
    s32 sndWait; // 0x3358
};

typedef struct M565Com_s {
    u8 pad_00[4];
    s32 active; // 0x04
    u8 pad_08[0x3C];
    s32 btnDown; // 0x44
    s32 stkX; // 0x48
    s32 stkY; // 0x4C
    s32 btn; // 0x50
} M565Com;

typedef struct M565Player_s {
    M565Sys *sys; // 0x00
    MGPLAYER *mg; // 0x04
    s16 no; // 0x08
    u8 pad_0A[2];
    s32 charNo; // 0x0C
    s16 mdl; // 0x10
    s16 mot[7]; // 0x12
    M565Com com; // 0x20
    HuVecF *pos; // 0x74
    HuVecF *rot; // 0x78
    f32 facing; // 0x7C
    struct M565Player_s *near; // 0x80
    f32 nearDist; // 0x84
    s16 col; // 0x88
    s16 row; // 0x8A
    f32 groundDist; // 0x8C
    s32 fallen; // 0x90
    s32 enabled; // 0x94
    s32 survivor; // 0x98
    f32 fallVel; // 0x9C
    s16 fxMdl; // 0xA0
    s16 fxMot; // 0xA2
    s32 fxActive; // 0xA4
    u8 pad_A8[4];
} M565Player;

typedef struct M565Back_s {
    M565Sys *sys; // 0x00
    s32 time; // 0x04
    s16 mdl0; // 0x08
    s16 mdl1; // 0x0A
    s16 mdl2; // 0x0C
    s16 mdl3; // 0x0E
    s16 mdl4; // 0x10
    s16 mot0; // 0x12
    s16 mot1; // 0x14
    s16 mot2; // 0x16
    s16 mot3; // 0x18
    s16 mot4; // 0x1A
    s16 mdl5; // 0x1C
    s16 mdl6; // 0x1E
    s16 mdl7; // 0x20
    s16 mdl8; // 0x22
    s16 mot5; // 0x24
    s16 mot6; // 0x26
    s16 mot7; // 0x28
    u8 pad_2A[2];
} M565Back;

typedef struct M565Light_s {
    s32 unk0;
    HU3DLIGHTID lightId;
    u8 pad_6[2];
} M565Light;

struct M565PukuItem_s {
    M565Puku *puku; // 0x00
    s32 active; // 0x04
    s16 mdlA; // 0x08
    s16 mdlB; // 0x0A
    s16 mot; // 0x0C
    u8 pad_0E[2];
    HuVecF pos; // 0x10
    HuVecF rot; // 0x1C
    s32 big; // 0x28
    s32 frozen; // 0x2C
    s16 t30; // 0x30
    s16 t32; // 0x32
    M565Tile *target; // 0x34
    HuVecF from; // 0x38
    HuVecF to; // 0x44
    f32 peak; // 0x50
    s32 frame; // 0x54
    s32 blink; // 0x58
};

struct M565Puku_s {
    M565Sys *sys; // 0x00
    M565PukuItem items[5]; // 0x04
    s16 cnt; // 0x1D0
    s16 cntBig; // 0x1D2
    s16 cntSmall; // 0x1D4
    s16 cntTotal; // 0x1D6
};

struct M565Sys_s {
    M565Game *game; // 0x000
    u8 pad_04[4];
    M565Player players[4]; // 0x008
    M565Back back; // 0x2B8
    M565Stage stage; // 0x2E4
    M565Light light; // 0x3640
    M565Puku puku; // 0x3648
    s16 mdl; // 0x3820
    s16 mot; // 0x3822
    s32 flag3824; // 0x3824
    s32 flag3828; // 0x3828
    HuVecF camBase; // 0x382C
    HuVecF camOfs; // 0x3838
    s32 shakeCnt; // 0x3844
    f32 shakeAmp; // 0x3848
    f32 shakeDecay; // 0x384C
    f32 shakeSpeed; // 0x3850
    f32 shakeAngle; // 0x3854
};

struct M565Game_s {
    OMOBJMAN *man; // 0x00
    u8 pad_04[4];
    s32 frame; // 0x08
    u8 pad_0C[2];
    u16 step; // 0x0E
    s32 timer; // 0x10
    s32 counter; // 0x14
    M565Sys sys; // 0x18
    s32 enable; // 0x3870
    s32 mesId; // 0x3874
    s32 bgm; // 0x3878
    s32 unk387C; // 0x387C
};

/* m565.c */
extern M565Game *lbl_1_bss_0;
extern OMOBJMAN *lbl_1_bss_4;
extern HuVecF lbl_1_data_30;
extern HuVecF lbl_1_data_3C;
extern HuVecF lbl_1_data_48;
void fn_1_0(M565List *list);
void fn_1_30(M565List *list);
void fn_1_A0(M565List *list, M565Node *node, M565Tile *data);
void fn_1_E4(M565List *list, M565Node *node);
f32 fn_1_8F4(f32 a, f32 b, f32 t);
void fn_1_A10(HuVecF *pos, f32 *vol, f32 *pan);
int fn_1_C18(int seId);
void fn_1_C40(int seNo);
void fn_1_C68(int seNo, s32 speed);
void fn_1_C98(int seNo, s16 pan);
void fn_1_CC8(int seNo, s16 vol);
void fn_1_CF8(Vec *pos, Vec *heading, f32 a, f32 b, f32 c, f32 d, f32 e);
int fn_1_D50(s16 id);
void fn_1_D78(int streamNo, s32 speed);
void fn_1_DA8(void);
int fn_1_DC8(s16 id);
int fn_1_DF0(s16 id);
int fn_1_E18(s16 id, s16 chan);

/* main.c */
void fn_1_E48(M565Game *game, OMOBJMAN *man);
void fn_1_F48(M565Game *game);
s32 fn_1_FA8(M565Game *game, s32 arg1);
s32 fn_1_104C(M565Game *game);
s32 fn_1_1074(M565Game *game);
s32 fn_1_10E0(M565Game *game);

/* light.c (Matching): the parent slot is declared s32 there but never read */
void fn_1_FCBC(M565Light *light, M565Sys *sys);
void fn_1_FD34(M565Light *light);
void fn_1_FD88(M565Light *light);

/* object.c */
void fn_1_25B4(M565Sys *sys, M565Game *game);
void fn_1_2A94(M565Sys *sys);
s32 fn_1_2B20(M565Sys *sys);
M565PukuItem *fn_1_3284(M565Sys *sys, s32 arg1, f64 angle, f32 scale);
void fn_1_351C(M565Sys *sys);
void fn_1_366C(M565Sys *sys, s32 cnt, f32 amp, f32 decay, f32 speed);

/* back.c */
void fn_1_3690(M565Back *back, M565Sys *sys);
void fn_1_3E54(M565Back *back);
void fn_1_3F14(M565Back *back);

/* stage.c */
void fn_1_4554(M565Stage *stage, M565Sys *sys);
void fn_1_4AE0(M565Stage *stage);
void fn_1_4BCC(M565Stage *stage);
void fn_1_5EB0(M565Stage *stage, HuVecF *pos, s32 big, M565Tile **out);

/* player.c */
void fn_1_78BC(M565Player *player, s16 no, M565Sys *sys);
void fn_1_7B8C(M565Player *player);
s32 fn_1_7BDC(M565Player *player);
void fn_1_8984(M565Player *player, s32 on);
void fn_1_89D4(M565Player *player, s32 type, f32 angle);

/* puku2.c */
void fn_1_8AD0(M565Puku *puku, M565Sys *sys);
void fn_1_8B44(M565Puku *puku);
void fn_1_8BA8(M565Puku *puku);
M565PukuItem *fn_1_8C70(M565Puku *puku, HuVecF *from, HuVecF *to, s32 big, f32 peak);

/* com.c */
void fn_1_9EAC(M565Com *com, M565Player *player);
void fn_1_9FF4(M565Com *com);
void fn_1_A050(M565Com *com);

/* opening.c / ending.c */
s32 fn_1_E740(M565Game *game);
s32 fn_1_EE4C(M565Game *game);

#endif
