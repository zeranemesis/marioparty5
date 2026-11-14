#ifndef _SD00DLL_SYS_H
#define _SD00DLL_SYS_H

#include "REL/sd00dll.h"

#include "dolphin/mtx.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/mg/timer.h"
#include "game/hsfformat.h"
#include "humath.h"

/* objDataTbl entry: 11 words. */
typedef struct SdStageObjType_s {
    s32 flags;     // 0x00
    union {
        Vec offset;    // 0x04 (aim offset from the object pos)
        struct {
            s32 unk4;
            f32 unk8;  // 0x08
            s32 unkC;
        };
    };
    f32 radius;    // 0x10
    f32 height;    // 0x14
    f32 height2;   // 0x18
    s32 waitTime;  // 0x1C (bomhei: frames idle before the fuse starts)
    s32 hideTime;  // 0x20 (bomhei: frames hidden after the blast)
    f32 bounceF;   // 0x24 (0: no push-back while the object is active)
    s32 damage;    // 0x28
} SdStageObjType; // size 0x2C

/* stageColMesh / stageColWall entry. */
typedef struct SdStageColTri_s {
    Vec pos;     // 0x00
    Vec unkC;    // 0x0C
    Vec normal;  // 0x18
    Mtx mtx;     // 0x24
    Mtx invMtx;  // 0x54
    Vec vtx[3];  // 0x84
    Vec dir[3];  // 0xA8
    f32 dot[3];  // 0xCC
    Vec vtx2[3]; // 0xD8
    Vec edge[3]; // 0xFC
    f32 dot2[3]; // 0x120
    f32 radius;  // 0x12C
    s32 attr;    // 0x130
} SdStageColTri; // size 0x134

/* sys->obj[8], stride 0x70. */
typedef struct SdStageObj_s {
    s32 mode;              // 0x00
    s32 unk4;
    s32 timer;             // 0x08
    s32 unkC;
    s32 type;              // 0x10
    HU3DMODELID mdlId;     // 0x14
    HU3DMODELID fireMdlId; // 0x16
    HU3DMODELID shadowMdlId; // 0x18
    HU3DMOTID motId1;      // 0x1A
    HU3DMOTID motId2;      // 0x1C
    s32 fireIdx;           // 0x20
    s32 flameIdx;          // 0x24
    Vec pos;               // 0x28
    f32 radius;            // 0x34
    f32 height;            // 0x38
    f32 height2;           // 0x3C
    Vec basePos;           // 0x40 (spawn pos, StageKinokoUpdate)
    s32 pad4C[3];          // 0x4C
    s32 hitF;              // 0x58
    s32 dispF;             // 0x5C
    s32 endF;              // 0x60
    SdStageObjType *typeP; // 0x64
    s32 pad68[2];
} SdStageObj; // size 0x70

/* machine->corner[cornerNum] */
typedef struct SdMachineCorner_s {
    s32 effIdx; // 0x00 (particle slot base)
    Vec pos;    // 0x04 (hook pos relative to machine)
} SdMachineCorner; // size 0x10

/* machine->track: per-wheel/exhaust effect slots (size 0x110) */
typedef struct SdMachineTrack_s {
    s32 effIdx[2][4]; // 0x00 (per wheel [2]: model[2], [9], [10]? slot bases)
    HU3DPARTICLEDATA *eff[2][30]; // 0x20 (live track particles per wheel)
} SdMachineTrack; // size 0x110 on GC (use sizeof)

/* sys->machine[2], stride 0x3F8. */
typedef struct SdStageMachine_s {
    s32 mode;          // 0x00
    s32 unk4;
    s32 timer;         // 0x08
    s32 unkC;
    s32 type;          // 0x10
    s32 pad14;         // 0x14
    s32 modeTimer;     // 0x18
    s32 pad1C[3];      // 0x1C
    union {
        Vec pos;       // 0x28
        s32 boostType; // 0x28 (compared with 1/2/5)
    };
    f32 wheelR[6];     // 0x34
    s32 pad4C[17];     // 0x4C
    s32 hitF;          // 0x90
    s32 colF;          // 0x94
    Mtx mtx;           // 0x98
    Vec curPos;        // 0xC8
    Vec rot;           // 0xD4
    f32 speed;         // 0xE0
    s32 hp;            // 0xE4
    Vec vel;           // 0xE8
    f32 yawVel;        // 0xF4
    Vec force;         // 0xF8
    f32 friction;      // 0x104
    Mtx storedMtx;     // 0x108
    Vec basePos;       // 0x138
    Vec storedRot;     // 0x144
    Vec smoothRot;     // 0x150 (SdStageMachineCol smoothed pitch/yaw/roll)
    f32 unk15C;        // 0x15C
    Vec rotVel;        // 0x160
    s32 launchFlag;    // 0x16C
    f32 tireScale[4];  // 0x170
    Vec tireRot[4];    // 0x180
    Vec wheelPos[4];   // 0x1B0 (world pos of each wheel contact)
    s32 wheelOn[4];    // 0x1E0 (wheel touching ground)
    s32 wheelAttr[4];  // 0x1F0 (surface type under wheel: 0 dirt,1 grass,2 sand,3..6 ...)
    s32 gunNum;        // 0x200
    s32 gunBulletNum;  // 0x204
    s32 cornerNum;     // 0x208
    s32 gunTimerInit;  // 0x20C
    s32 unk210;        // 0x210
    s32 unk214;        // 0x214
    s32 unk218;        // 0x218
    Mtx planeMtx;      // 0x21C
    union {
        f32 slope[3][4]; // 0x24C (0x10 stride)
        Mtx savedMtx;    // 0x24C
    };
    f32 dmgOfsA;       // 0x27C
    f32 dmgOfsB;       // 0x280
    s32 pad284;        // 0x284
    s32 turnLv;        // 0x288
    s32 speedLv;       // 0x28C
    s32 accelLv;       // 0x290
    HSFOBJECT *tireObj[4]; // 0x294
    struct SdStageMachine_s *target; // 0x2A4 (opponent)
    struct SdGun_s *gun; // 0x2A8
    s32 comSteerState; // 0x2AC (SdComMain: 0 straight, 1 right, 2 left)
    s32 comSteerTimer; // 0x2B0
    s32 comSteerTime;  // 0x2B4
    f32 comSteerStick; // 0x2B8
    s32 pad2BC[6];     // 0x2BC
    f32 groundY[2];    // 0x2D4
    s32 comStickDir;   // 0x2DC (SdComMain: 0 none, 1 right, 2 left)
    s32 comTimer;      // 0x2E0 (frames since COM start)
    s32 comMode;       // 0x2E4 (SdComMain action, 0 = idle)
    s32 comPreset;     // 0x2E8 (COM preset index, -1: none; SdMachineConfigSet)
    s32 comLevel;      // 0x2EC (difficulty from comDataTbl)
    f32 comStickX;     // 0x2F0
    s32 pad2F4;        // 0x2F4
    f32 comStickY;     // 0x2F8
    Vec comObjPos;     // 0x2FC (ComStageObjPosGet result, x == -10000: none)
    Vec comRandPos;    // 0x308 (SdStageRandomPosGet result, x == -10000: none)
    s16 fireMdlId[2];  // 0x314
    s32 fireCnt;       // 0x318
    Vec firePos;       // 0x31C
    s32 effType;       // 0x328
    s32 effTimer;      // 0x32C
    s32 camSingleF;    // 0x330 (1: machine visible to own camera only)
    s32 contactF;      // 0x334
    s32 damage;        // 0x338
    struct SdMachineCorner_s *corner; // 0x33C (cornerNum entries)
    struct SdMachineTrack_s *track;   // 0x340
    s32 playerNo;      // 0x344
    s32 padNo;         // 0x348
    s32 charNo;        // 0x34C
    s32 cameraNo;      // 0x350
    f32 wheelBase;     // 0x354
    f32 hitRadius;     // 0x358
    f32 hitHeight;     // 0x35C
    f32 wheelY0;       // 0x360
    f32 wheelY1;       // 0x364
    s32 attr;          // 0x368
    Vec wheelWorld2[4];// 0x36C
    s32 unk39C;        // 0x39C
    s16 hookMdlId[4];  // 0x3A0
    s16 mdlId;         // 0x3A8
    s16 gunMdlId;      // 0x3AA
    HU3DMODELID gunMdlId2; // 0x3AC
    s16 mdl3AE;        // 0x3AE
    s16 charMdlId;     // 0x3B0
    s16 charMotId;     // 0x3B2
    s16 mdl3B4;        // 0x3B4
    s16 pad3B6;        // 0x3B6
    s32 kind;          // 0x3B8
    s32 bodyType;      // 0x3BC
    s32 engineType;    // 0x3C0
    s32 bulletType;    // 0x3C4
    s32 stat[5];       // 0x3C8 (clamped 1..8)
    s32 engineSeId;    // 0x3DC
    s32 skidSeId;      // 0x3E0
    s32 pad3E4[3];     // 0x3E4
    s32 hitSeId;       // 0x3F0
    s32 gunSeId;       // 0x3F4
} SdStageMachine; // size 0x3F8

typedef struct SdStageData_s {
    s32 groundFile; // 0x00
    s32 objFile;    // 0x04
    s32 type;       // 0x08
    f32 radius;     // 0x0C
    f32 radius2;    // 0x10
} SdStageData; // size 0x14

/* stageTrialMapHeight / stageTrialMapNum: 0x31 x 0x31 cells of 0x24 bytes. */
typedef struct SdStageTrialCell_s {
    s32 count;     // 0x00
    void *data[8]; // 0x04
} SdStageTrialCell; // size 0x24

typedef struct SdStageSmoke_s {
    s32 velY;  // 0x00
    s32 x;     // 0x04
    s32 y;     // 0x08
    s32 z;     // 0x0C
    s32 scale; // 0x10
    s32 color; // 0x14
} SdStageSmoke; // size 0x18

/* Hu3DData[sys->smokeMdlId].hookData + smokeSlot * 0x44 */
typedef struct SdStageParticle_s {
    s16 type;      // 0x00
    s16 unk2;
    s16 cameraBit; // 0x06
    f32 vel[3];    // 0x08
    f32 life;      // 0x14
    Vec pos;       // 0x18
    s32 unk24;
    Mtx mtx;       // 0x28
    GXColor color; // 0x40
} SdStageParticle; // size 0x44

/* ===== gun / bullet (owner: gun part A) ===== */
struct SdBullet_s;
struct SdStageMachine_s;
/* machine->gun[i] (stride 0x44 on GC) */
typedef struct SdGun_s {
    s32 timer;                 // 0x00
    Mtx mtx;                   // 0x04
    struct SdBullet_s *lastBullet; // 0x34
    struct SdBullet_s *bullet; // 0x38 (array, machine gunBulletNum entries)
    GXColor color;             // 0x3C
    s32 pad40;                 // 0x40
} SdGun;

/* sdBulletParam[bulletType], stride 0x54 */
typedef struct SdBulletParam_s {
    s32 unk0;      // 0x00 (==2: homing)
    s32 unk4;
    s32 unk8;      // 0x08
    f32 range;     // 0x0C
    f32 unk10;
    f32 unk14;     // 0x14
    f32 unk18;
    f32 unk1C;     // 0x1C
    f32 unk20;     // 0x20
    s32 partNum[12]; // 0x24 particles per sys->partMdlId bank
} SdBulletParam;

typedef struct SdBullet_s {
    s32 mode;       // 0x00
    s32 unk4;       // 0x04
    s32 pad08[3];
    s32 unk14;      // 0x14
    s32 unk18;      // 0x18
    s32 pad1C[3];
    s16 mdlId;      // 0x28
    s16 mdlId2;     // 0x2A
    s16 mdlId3;     // 0x2C
    s16 mdlId4;     // 0x2E
    s16 mdlId5;     // 0x30
    s16 pad32;
    s32 partIdx[12]; // 0x34 particle slot base per sys->partMdlId bank
                     // ([5] = BulletExplodeUpdate effect idx, [10] = LaserEffectUpdate effect idx)
    f32 unk64;      // 0x64
    SdBulletParam *param; // 0x68
    Mtx mtx;        // 0x6C
    f32 pad9C[12];  // 0x9C
    Vec pos;        // 0xCC
    Vec prevPos;    // 0xD8
    Vec rot;        // 0xE4
    s32 padF0[3];
    f32 unkFC;      // 0xFC
    s32 pad100;
    Vec startPos;   // 0x104
    Vec pos110;     // 0x110
    f32 speed;      // 0x11C
    f32 unk120;     // 0x120
    f32 unk124;     // 0x124
    struct SdStageMachine_s *machine; // 0x128
    SdGun *gun;     // 0x12C
    s32 pad130[4];
} SdBullet; // 0x140 on GC

/* sys->camera[2], stride 0x88. */
typedef struct SdCamera_s {
    s32 bit;           // 0x00 camera bit (sdCameraBitTbl)
    s32 active;        // 0x04 follow-camera flag
    Vec goalPos;       // 0x08
    Vec goalTarget;    // 0x14
    Vec pos;           // 0x20
    Vec target;        // 0x2C
    Vec up;            // 0x38
    f32 follow;        // 0x44 lerp factor to goal
    f32 viewTime;      // 0x48 view-lerp progress 0..1
    Vec beginPos;      // 0x4C
    Vec beginTarget;   // 0x58
    Vec endPos;        // 0x64
    Vec endTarget;     // 0x70
    s32 offsetOn;      // 0x7C
    f32 offsetDist;    // 0x80
    f32 shake;         // 0x84
} SdCamera; // size 0x88

/* sys->spr (0xAA0, size 0xC0 on GC): HUD sprites */
typedef struct SdSprWork_s {
    s32 timer;             // 0x00
    s16 hpIcon[2][8];      // 0x04 (per playerNo: damage pips)
    s16 mapSpr;            // 0x24
    s16 playerSpr[2];      // 0x26 (minimap marker per playerNo)
    s16 pad2A[9];          // 0x2A
    s16 *bulletSpr[2];     // 0x3C (per playerNo: gunNum*gunBulletNum markers)
    s16 pad44;
    s16 zoomSpr[2][3];     // 0x46 (big marker, per playerNo)
    s16 zoomHpIcon[2][8][3]; // 0x52
    s16 centerSpr;         // 0xB2
    s16 flagSpr;           // 0xB4
    s16 rabbitSpr;         // 0xB6
    s32 hideF;             // 0xB8
    s32 winnerNo;          // 0xBC (sys+0xB5C: winner machine no, -1 none)
} SdSprWork; // size 0xC0

/* sys->impact[30] (0xC18, stride 0x20) */
typedef struct SdImpact_s {
    s16 active;    // 0x00
    s16 timer;     // 0x02
    s32 snd[2];    // 0x04 (per camera)
    s16 delay[2];  // 0x0C
    s16 pan[2];    // 0x10
    s16 vol[2];    // 0x14
    s32 vibrate;   // 0x18
    s32 pad1C;
} SdImpact; // size 0x20

typedef struct SdSys_s {
    OMOBJMAN *objman;      // 0x00
    OMOBJ *cameraObj;      // 0x04
    s32 seqStep;           // 0x08 (state inside a Seq* mode)
    s32 seqStepA;          // 0x0C (SdSeqInit: stageNo temp; FadeIn: ready sprite step)
    s32 seqStepB;          // 0x10 (FadeIn: HP bar step)
    s32 seqStepC;          // 0x14 (FadeIn: split screen step)
    s32 unk18;             // 0x18
    s32 seqTimer;          // 0x1C (frame counter of current mode)
    s32 seqTimerA;         // 0x20
    s32 seqTimerB;         // 0x24
    s32 seqTimerC;         // 0x28
    s32 unk2C;             // 0x2C
    MGTIMER *timer;        // 0x30
    s32 timeSec;           // 0x34 (seconds elapsed)
    s32 noTimerF;          // 0x38
    s32 stageType;         // 0x3C
    s32 padNo[2];          // 0x40
    s32 gameType;          // 0x48
    SdCamera camera[2];    // 0x4C
    HU3DLIGHTID lightId;   // 0x15C
    HU3DMODELID stageMdlId;// 0x15E
    HU3DMODELID backMdlId[2]; // 0x160, 0x162
    HU3DMODELID hamonMdlId[2];// 0x164, 0x166
    HU3DMODELID shadowMdlId;// 0x168
    s16 pad16A;             // 0x16A
    ANIMDATA *partAnim[12]; // 0x16C (particle anim banks; [3],[5] = machine fire)
    HU3DMODELID partMdlId[12]; // 0x19C (smoke=[0], dust=[3], flame=[4])
    s32 partCnt[12];        // 0x1B4 (particles allocated per bank)
    HU3DMODELID kinokoMdlId; // 0x1E4
    HU3DMODELID kinokoMdlId2; // 0x1E6
    HU3DMODELID togezoMdlId; // 0x1E8
    HU3DMODELID togezoMdlId2; // 0x1EA
    HU3DMODELID baseShadowMdlId; // 0x1EC
    HU3DMODELID bulletHookMdlId; // 0x1EE (SdBulletDraw hook model)
    s32 stageNo;   // 0x1F0
    s32 pad1F4[(0x244 - 0x1F4) / 4];

    SdStageMachine machine[2]; // 0x244

    Vec trialPos[5];  // 0xA34
    void *hamonTex;    // 0xA70
    ANIMDATA *hamonAnim[2]; // 0xA74, 0xA78
    f32 hamonTime;    // 0xA7C
    f32 hamonSpeed;   // 0xA80
    Vec hamonPos[2];  // 0xA84
    GXColor hamonColor; // 0xA9C
    SdSprWork spr;         // 0xAA0 (cameraSprId = spr.centerSpr, 0xB52)

    s32 objCreateF;    // 0xB60
    SdStageObj *obj;   // 0xB64
    s32 timeUpF;       // 0xB68
    s32 allDoneF;      // 0xB6C (both machines finished)
    s32 stageClosedF;  // 0xB70
    s32 bgmStream;     // 0xB74
    HSFOBJECT *backObj[2][20]; // 0xB78 (Hu3DModelObjPtrGet results; ->mesh.base.rot.y animated)
    SdImpact impact[30];   // 0xC18
    s32 vibrate[2][2];     // 0xFD8 {level, timer}

    s32 smokeSlot;    // 0xFE8
    s32 frameCount;   // 0xFEC
    s32 fxId;         // 0xFF0
} SdSys;

extern SdSys *sys;

/* aliases for fields that turned out to be array elements */
#define smokeMdlId partMdlId[0]
#define dustMdlId partMdlId[3]
#define flameMdlId partMdlId[4]
#define cameraSprId spr.centerSpr

typedef SdStageMachine SdMachine;

/* ===== Cross-unit prototypes: one section per owning unit. Add yours with a
   small Edit; grep first so nothing is declared twice. ===== */
/* proto: sd00 */
/* sd00 part C */
void MachineDispSet(SdMachine *machine, s32 disp);
void MachineScaleSet(SdMachine *machine, Vec *scale);
void MachineCameraBitSet(SdMachine *machine, u16 cameraBit);
void MachineCameraInit(SdMachine *machine, s32 mode);
void MachineVelocityApply(SdMachine *machine, Vec *eye, Vec *look);
void MachineViewUpdate(s32 cameraNo, s32 flip, Vec *from, Vec *to, Vec *out, f32 t);
void MachinePosSet(SdMachine *machine, Vec *pos);
void MachinePosInit(SdMachine *machine);
void MachineRotSet(SdMachine *machine, Vec *rot);
void MachineRotApply(SdMachine *machine, Vec *rot);
void MachineRotAngleSet(SdMachine *machine, Vec *rot);
void MachineRotAngleApply(SdMachine *machine, Vec *rot);
void SdCharInit(SdMachine *machine1, SdMachine *machine2);
void SdCameraCreate(void);
void SdCameraSplitSet(f32 split);
void SdCameraUpdate(void);
void SdSinLerp(Vec *out, Vec *a, Vec *b, f32 t);
void CameraSetViewPos(s32 cameraNo, Vec *pos, Vec *up, Vec *target);
void SdCameraViewSet(s32 cameraNo, Vec *pos, Vec *up, Vec *target, f32 follow);
void SdCameraViewBeginSet(s32 cameraNo, Vec *pos, Vec *up, Vec *target);
void SdCameraViewEndSet(s32 cameraNo, Vec *pos, Vec *up, Vec *target);
void SdCameraViewSpeedSet(s32 cameraNo, f32 time);
s32 SdCameraViewUpdate(s32 cameraNo, Vec *up, f32 duration);
void SdVibrateUpdate(void);
/* proto: machine */
void SdMachineParamSet(SdMachine *machine, s32 tireType, s32 bodyType, s32 engineType, s32 gunType);
void SdMachineCreate(SdMachine *machine);
void SdMachineKill(SdMachine *machine);
void SdMachineDestroy(SdMachine *machine);
void SdMachineConfigSet(s32 playerNo);
void SdMachineEffectCreate(void);
void SdSprCreate(void);
void SdHpSprCreate(void);
void SdSprKill(void);
void SdHpSprKill(void);
void SdSprScissorUpdate(s16 sprId, s32 cameraNo);
void SdSprUpdate(void);
void SdSprHideAll(void);
void SdMachineFxStart(SdMachine *machine);
void SdMachineFxStop(SdMachine *machine);
void SdMachineFxUpdate(SdMachine *machine);
void SdMachineFxGunPlay(struct SdBullet_s *bullet);
void SdMachineFxImpactPlay(struct SdBullet_s *bullet);
void SdImpactSoundStart(s32 snd, Vec *pos, f32 vol);
void SdImpactUpdate(void);
/* proto: com */
/* proto: gun */
void SdBulletUpdate(SdStageMachine *machine);
void SdBulletDraw(HU3DMODEL *modelP, Mtx *mtx);
/* proto: stage */
/* stage part E (stage_e.inc) asm-local, forward-declare in stage.c before MakeTrialStage:
   static void StageTrialMakeGround(void); static void MakeTrialStageMesh(void);
   (defined in stage_e.inc; it needs these statics defined earlier in stage.c: u8 *stageTrialMap,
   u8 *stageTrialMapColType, f32 stageTrialHeight, s32 stageTrialSmooth, stageTrialLavaFloorOnF,
   stageTrialClearOff, u16 *texturePtr, s16 trialModelId[4]; it defines stageTrialNoise{Dec,X,Y,Num}). */
s32 SdStageObjHitCheck(Vec *pos, f32 radius);
s32 SdStageBaseObjHitCheck(Vec *pos, f32 radius);
s32 SdStageObjHitCheck2(Vec *pos, f32 radius, s32 type);
s32 SdStageBaseObjHitCheck2(Vec *pos, f32 radius, s32 type);
/* proto: math */
void LightColorSet(s16 modelIdx, s32 lightIdx, u8 r, u8 g, u8 b);
/* proto: util */
void SdScreenPosGet(Vec *pos, s16 cameraBit, Vec *out);
/* proto: flag */
s32 SdFlagCheck(void);
void SdFlagPosGet(Vec *pos);
void SdFlagSprDispOff(void);
/* flag work block: used by the flag (1 instance) and the rabbit (6 instances) */
typedef struct SdFlagWork_s {
    HU3DMODELID mdlId; // 0x00
    Vec pos;           // 0x04
    s32 hitF;          // 0x10 (flag: unused; rabbit: caught flag)
    s32 timer[2];      // 0x14 (pickup/catch count per machine)
    s16 spr[2][3];     // 0x1C (HUD markers per machine)
} SdFlagWork; // size 0x28
s32 SdFlagPosSet(SdFlagWork *work);
void SdFlagSprUpdate(SdFlagWork *work);
/* proto: rabbit */
void SdRabbitPlayerCheck(s32 playerNo);
void SdRabbitZAxisGet(Vec *zAxis);
void SdRabbitSprDispOff(void);

extern s32 sdCameraBitTbl[];

void SdStageInit(s32 stageNo);
void SdStageCreate(s32 stageNo);
void SdStageBackRotYSet(s32 machineNo, f32 rotY, f32 rotX);
void SdStageClose(void);
s32 SdStageFloorCheck(Vec *pos, Vec *normal);
s32 SdStageYOfsFloorCheck(Vec *pos, Vec *normal, f32 yOfs);
s32 SdStageWallCheck(Vec *pos, Vec *normal);
s32 SdStageYOfsWallCheck(Vec *pos, Vec *normal, f32 yOfs);
f32 SdStageDistCalc(Vec *pos);
f32 SdStageRadiusCalc(void);
void SdStageSprPosGet(s32 *type, Vec *radius);
void SdStageRandomPosGet(Vec *pos, f32 radius);
f32 SdStageEdgeDistGet(Vec *pos);
f32 SdStageEdgeDistRadiusGet(Vec *pos, f32 radius);
s32 SdStageBaseObjHitCheck(Vec *pos, f32 radius);
void SdStageUpdate(void);
void SdStageObjCreate(void);
void SdStageMachineCol(SdStageMachine *machine);
void SdParticleCameraSet(s32 cameraBit);
void SdAuxSet(void);

s32 SdRabbitCheck(void);
s32 SdRabbitDoneCheck(void);
s32 SdRabbitDispTimerGet(s32 no);
void SdRabbitPosGet(Vec *pos);
s32 SdFlagDispTimerGet(s32 no);
void SdMachineParticleCreate(void);
void SdMachineParticleKill(void);
void SdModelGlowSet(HU3DMODELID mdlId);
void SdVibrate(s32 playerNo, s32 a, s32 b);
void SdImpactStart(s32 snd, Vec *pos, s32 vibrate, f32 vol, f32 range);
void SdEffectDustCreateData(Vec *pos, void *p1, void *p2, f32 scale);
void SdEffectFlameUpdate(void *data, s32 count);
void SdEffectFlameUpdateData(void *data, s32 count);
void SdRandomPosGet(Vec *out, s32 mode, Vec *center, Vec *axis, f32 maxR, f32 minR);
void SdPlayerSoundPlay(SdStageMachine *machine, s32 no);
s32 SdMachineDamage(SdStageMachine *machine, Vec *pos, s32 damage);
void SdComMain(SdStageMachine *machine);
void SdGunUpdate(SdStageMachine *machine);
void MachineScaleSet(SdStageMachine *machine, Vec *scale);

f32 MathLerp(s32 mode, f32 t);
void MathVecInterpolate(Vec *dst, Vec *a, Vec *b, f32 t, f32 s);
void MathMtxRotSet(Vec *rot, Vec *out, Mtx *mtx);
void MathMtxRotGet(Mtx mtx, Vec *rot);
void MathPlaneRotMtxGet(Vec *v1, Vec *v2, f32 f, Mtx mtx);

#endif