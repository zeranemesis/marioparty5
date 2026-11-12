#ifndef _M575DLL_H
#define _M575DLL_H

#include "types.h"
#include "dolphin/mtx.h"
#include "game/object.h"

typedef struct M575Score_s {
    struct M575Main_s *main;
    u16 side;
    s16 winId;
    struct MgScore_s *score;
    s16 espA;
    s16 espB;
    s16 value;
} M575Score;

// Shared by REL/m575dll/tuboeff.c; ten.c only reaches into it through the
// fields below. Layout confirmed from tuboeff.c; ten.c additionally reads the
// tube axis out of mtx row 1.
typedef struct M575TuboEff_s {
    s32 unk0;
    u16 unk4;
    f32 height;
    f32 len0;
    f32 len1;
    f32 len2;
    Mtx scaleMtx;
    Mtx mtx;
    Mtx mtx2;
    Vec pos;
    Vec vel;
    Vec angVel;
    Vec angPos;
    Vec angAcc;
    f32 unkE4;
    f32 unkE8;
} M575TuboEff;

struct M575Main_s;
struct M575Game_s;
struct M575Player_s;
struct M575CoinItem_s;

// Per-player COM (computer-player) control block, REL/m575dll/com.c.
typedef struct M575Com_s {
    struct M575Player_s *work;
    u16 mode;
    s16 timer;
    s32 unk8;
    s32 unkC;
    u16 unk10;
    u8 pad12[2];
    Vec unk14;
    Vec unk20;
    s32 unk2C;
    s32 unk30;
    f32 unk34;
    f32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
} M575Com;

// REL/m575dll/player.c: one of the two players of a side (0xCC bytes).
typedef struct M575Player_s {
    struct M575Main_s *main;
    u16 state;
    struct MgPlayer_s *mgPlayer;
    s16 slot;
    s16 playerNo;
    s32 charNo;
    s16 modelId;
    s16 motion[9];
    f32 height;
    M575Com com;
    Vec *pos;
    Vec *rot;
    f32 unk80;
    f32 unk84;
    Vec vel;
    f32 speed;
    struct M575CoinItem_s *target;
    f32 targetDist;
    s32 timer;
    s32 padOn;
    s32 unkA8;
    s16 unkAC;
    s32 falling;
    s32 unkB4;
    s32 unkB8;
    f32 prevVelY;
    Vec push;
} M575Player;

// REL/m575dll/camera.c (0x68 bytes). camBit is the Hu3D camera bit.
typedef struct M575Camera_s {
    struct M575Main_s *main;
    s32 camBit;
    s32 unk8;
    Vec posTarget;
    Vec pos;
    Vec tgtTarget;
    Vec tgt;
    Vec shake;
    Vec shake2;
    Vec up;
    f32 lerp;
    s32 shakeOn;
} M575Camera;

// REL/m575dll/light.c (0x08 bytes).
typedef struct M575Light_s {
    s32 unk0;
    s16 lightId;
} M575Light;

typedef struct M575StageItem_s {
    struct M575Stage_s *stage;
    u16 state;
    s16 idx;
    s16 modelId;
    Vec target;
    Vec pos;
    f32 velY;
    f32 rotTarget;
    f32 rot;
} M575StageItem;

// REL/m575dll/stage.c (0xB68 bytes).
typedef struct M575Stage_s {
    struct M575Main_s *main;
    s32 unk4;
    s16 modelId;
    s16 modelId2;
    s16 modelId3;
    s16 modelId4;
    s16 modelId5;
    s16 modelId6;
    s16 motion1;
    s16 motion2;
    s16 motion3;
    s16 motion4;
    f32 time;
    M575StageItem item[0x3C];
    s32 itemOn;
} M575Stage;

typedef struct M575CoinItem_s {
    struct M575Coin_s *coin;
    u16 state;
    s16 timer;
    u16 kind;
    s16 modelId;
    Vec *pos;
    f32 *mtx;
    s32 active;
    Vec vel;
    Vec dest;
    Vec start;
    f32 height;
    f32 prevY;
    s32 bounced;
    Vec rel;
} M575CoinItem;

// REL/m575dll/coin.c (0x2A8 bytes).
typedef struct M575Coin_s {
    struct M575Main_s *main;
    M575CoinItem item[8];
    s16 modelId;
    s16 modelId2;
} M575Coin;

// REL/m575dll/jugem.c (0x48 bytes).
typedef struct M575Jugem_s {
    struct M575Main_s *main;
    u16 state;
    s16 timer;
    s16 modelId;
    s16 motion;
    s16 motion2;
    Vec pos;
    Vec target;
    M575Player *held;
    s32 holding;
    Vec dest;
    Vec offset;
} M575Jugem;

typedef struct M575Ten_s {
    struct M575Main_s *main;
    s16 modelId;
    s16 motionId;
    Vec *pos;
    MtxPtr mtx;
    s16 modelId2;
    Vec *pos2;
    MtxPtr mtx2;
    s16 timer;
    f32 rot;
    f32 prevRot;
    s16 counter;
    M575TuboEff eff;
} M575Ten;

// REL/m575dll/tubo.c: the ten objects that hold the tube model and its effect.
typedef struct M575Tubo_s {
    struct M575Main_s *main;
    s16 unk4;
    s16 unk6;
    s16 modelId1;
    s16 unkA;
    Vec *unkC;
    f32 *unk10;
    s16 modelId2;
    s16 motionId;
    Vec *unk18;
    f32 *unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    M575TuboEff eff;
    Mtx mtx;
    s16 unk148;
    s16 unk14A;
} M575Tubo;

// One side of the game (0x12E0 bytes): two players plus every per-side object.
typedef struct M575Main_s {
    struct M575Game_s *game;
    s16 side;
    M575Player player[2];
    M575Ten ten;
    M575Stage stage;
    M575Camera camera;
    M575Light light;
    M575Tubo tubo;
    M575Coin coin;
    M575Score score;
    M575Jugem jugem;
} M575Main;

// REL/m575dll/m575.c: the whole game (0x2614 bytes).
typedef struct M575Game_s {
    OMOBJMAN *objman;
    s32 unk4;
    s32 frame;
    u16 mode;
    u16 phase;
    s32 timer;
    s16 wait;
    M575Main side[2];
    s32 unk25D8;
    s16 mesId;
    s16 colMap[16];
    s16 colMapNum;
    s16 esp;
    s16 firstSide;
    s16 coinThresh[2];
    s32 bgm;
    s32 unk2610;
} M575Game;

// REL/m575dll/m575.c
typedef struct M575Path_s {
    s16 count;
    Vec pts[20];
    s32 len[20];
    s32 acc[20];
} M575Path;

s32 fn_1_530(s32 seId);
void fn_1_558(s32 seNo);
void fn_1_580(s32 seNo, s32 speed);
void fn_1_5B0(s32 seNo, s16 pan);
void fn_1_5E0(Vec *pos, Vec *heading, f32 sndDist, f32 sndSpeed, f32 startDis, f32 frontSurDis, f32 backSurDis);
s32 fn_1_638(s16 streamId);
void fn_1_660(s32 streamNo, s32 speed);
void fn_1_690(void);
s32 fn_1_6B0(s16 streamId);
s32 fn_1_6D8(s16 streamId);
s32 fn_1_700(s16 streamId, s16 chanNo);
f32 fn_1_30CC(f32 from, f32 to, f32 ratio);
void fn_1_1C00(s32 arg);
void fn_1_2A38(Vec *a, Vec *b, GXColor *col);
void fn_1_2B10(Vec *a, Vec *b, Vec *c, GXColor *col);
void fn_1_2C1C(Vec *a, Vec *b, Vec *c, Vec *d, GXColor *col);
void fn_1_2D5C(Quaternion *q, const Vec *v0, const Vec *v1);
void fn_1_31E8(M575Path *path);
void fn_1_3218(M575Path *path);
void fn_1_3248(M575Path *path, const Vec *p, s32 len);
void fn_1_33C4(Vec *out, M575Path *path, s32 t);
void fn_1_3880(M575Main *main, s16 side, M575Game *game);
void fn_1_396C(M575Main *main);
s32 fn_1_39E4(M575Main *main);

void fn_1_535C(M575Player *self, s16 slot, s16 playerNo, M575Main *main);
void fn_1_55EC(M575Player *self);
s32 fn_1_562C(M575Player *self);
void fn_1_5C88(M575Player *self, s32 on);
void fn_1_6630(M575Com *self, M575Player *work);
void fn_1_675C(M575Com *self);
void fn_1_678C(M575Com *self);
void fn_1_9B58(M575TuboEff *self, const Vec *arg1, const Vec *arg2);
void fn_1_CFE4(M575Jugem *self, M575Player *player);

void fn_1_535C(M575Player *self, s16 slot, s16 playerNo, M575Main *main);
void fn_1_55EC(M575Player *self);
s32 fn_1_562C(M575Player *self);
void fn_1_5C88(M575Player *self, s32 on);
void fn_1_6630(M575Com *self, M575Player *work);
void fn_1_675C(M575Com *self);
void fn_1_678C(M575Com *self);
void fn_1_9B58(M575TuboEff *self, const Vec *arg1, const Vec *arg2);
void fn_1_CFE4(M575Jugem *self, M575Player *player);

void fn_1_851C(M575Camera *self, s16 side, M575Main *main);
void fn_1_8778(M575Camera *self);
void fn_1_87A8(M575Camera *self);
void fn_1_89D8(M575Camera *a, M575Camera *b, f32 t);
void fn_1_8C68(M575Camera *self);

void fn_1_9BC8(M575Tubo *self, M575Main *main);
void fn_1_9E8C(M575Tubo *self);
void fn_1_9EE4(M575Tubo *self);
M575CoinItem *fn_1_B240(M575Coin *self, u16 kind, Vec *pos, Vec *vel, f32 time);

void fn_1_9BC8(M575Tubo *self, M575Main *main);
void fn_1_9E8C(M575Tubo *self);
void fn_1_9EE4(M575Tubo *self);
M575CoinItem *fn_1_B240(M575Coin *self, u16 kind, Vec *pos, Vec *vel, f32 time);

void fn_1_AEF0(M575Coin *self, M575Main *main);
void fn_1_B074(M575Coin *self);
void fn_1_B0E8(M575Coin *self);
void fn_1_C464(M575Jugem *self, M575Main *main);
void fn_1_C5E0(M575Jugem *self);
void fn_1_C630(M575Jugem *self);

void fn_1_3A7C(M575Score *self, u16 side, M575Main *main);
void fn_1_3CD8(M575Score *self);
void fn_1_3D28(M575Score *self);
void fn_1_3D5C(M575Score *self, s32 disp);

void fn_1_4BCC(M575Ten *self, M575Main *main);
void fn_1_4E98(M575Ten *self);
void fn_1_4EF0(M575Ten *self);

void fn_1_9320(M575TuboEff *eff, u16 arg1, const Vec *arg2, f32 height, f32 radius, f32 speed);
void fn_1_96F8(M575TuboEff *eff);
void fn_1_9728(M575TuboEff *eff);

#endif
