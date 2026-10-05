#ifndef _SD00DLL_STAGE_H
#define _SD00DLL_STAGE_H

#include "REL/sd00dll.h"

#include "dolphin/mtx.h"
#include "game/hu3d.h"
#include "game/hsfformat.h"
#include "humath.h"

/* objDataTbl entry: 11 words. */
typedef struct SdStageObjType_s {
    s32 flags;     // 0x00
    s32 unk4;
    f32 unk8;      // 0x08
    s32 unkC;
    f32 radius;    // 0x10
    f32 height;    // 0x14
    f32 height2;   // 0x18
    s32 damage;    // 0x1C
    s32 dustTime;  // 0x20
    f32 dustScale; // 0x24
    s32 hitType;   // 0x28
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
    s32 pad40[6];          // 0x40
    s32 hitF;              // 0x58
    s32 dispF;             // 0x5C
    s32 endF;              // 0x60
    SdStageObjType *typeP; // 0x64
    s32 pad68[2];
} SdStageObj; // size 0x70

/* sys->machine[2], stride 0x3F8. */
typedef struct SdStageMachine_s {
    s32 mode;          // 0x00
    s32 unk4;
    s32 timer;         // 0x08
    s32 unkC;
    s32 type;          // 0x10
    s32 pad14[5];      // 0x14
    Vec pos;           // 0x28
    f32 wheelR[6];     // 0x34
    s32 pad4C[17];     // 0x4C
    s32 hitF;          // 0x90
    s32 colF;          // 0x94
    Mtx mtx;           // 0x98
    Vec curPos;        // 0xC8
    Vec rot;           // 0xD4
    s32 padE0[2];      // 0xE0
    Vec vel;           // 0xE8
    s32 padF4;         // 0xF4
    Vec force;         // 0xF8
    s32 pad104[10];    // 0x104
    Vec wheelPos[4];   // 0x12C
    Vec rotVel;        // 0x15C
    Vec pushDir;       // 0x168
    s32 pad174[5];     // 0x174
    Vec wheelPrev[4];  // 0x188
    Vec wheelWorld[4]; // 0x1B8
    f32 wheelRot;      // 0x1E8
    Vec wallDir;       // 0x1EC
    s32 wheelFloor[4]; // 0x1F8
    s32 wheelType[4];  // 0x208
    s32 unk218;        // 0x218
    Mtx planeMtx;      // 0x21C
    f32 slope[3][4];   // 0x24C (0x10 stride)
    s32 pad27C[4];     // 0x27C
    f32 unk28C;        // 0x28C
    s32 pad290[3];     // 0x290
    f32 unk29C;
    s32 pad2A0[13];    // 0x2A0
    f32 groundY[2];    // 0x2D4
    s32 pad2DC[19];    // 0x2DC
    Vec contact;       // 0x328
    s32 contactF;      // 0x334
    s32 pad338[3];     // 0x338
    s32 playerNo;      // 0x344
    s32 pad348[3];     // 0x348
    f32 wheelBase;     // 0x354
    f32 hitRadius;     // 0x358
    f32 hitHeight;     // 0x35C
    f32 wheelY0;       // 0x360
    f32 wheelY1;       // 0x364
    s32 attr;          // 0x368
    Vec wheelWorld2[4];// 0x36C
    s32 unk39C;        // 0x39C
    s32 pad3A0[22];    // 0x3A0
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
    s32 type;  // 0x00
    Vec pos;   // 0x04
    s32 life;  // 0x10
    u8 color;  // 0x14
    u8 pad15[3];
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

typedef struct SdSys_s {
    s32 pad00[0x48 / 4];   // 0x00
    s32 gameType;          // 0x48
    s32 pad4C[(0x120 - 0x4C) / 4];
    s32 unk120;            // 0x120
    s32 pad124[3];
    s32 unk130;            // 0x130
    s32 pad134[10];
    HU3DLIGHTID lightId;   // 0x15C
    HU3DMODELID stageMdlId;// 0x15E
    HU3DMODELID backMdlId[2]; // 0x160, 0x162
    HU3DMODELID hamonMdlId[2];// 0x164, 0x166
    HU3DMODELID shadowMdlId;// 0x168
    s16 pad16A[(0x19C - 0x16A) / 2];
    HU3DMODELID smokeMdlId; // 0x19C
    s32 unk1A0;
    HU3DMODELID dustMdlId;  // 0x1A2
    HU3DMODELID flameMdlId; // 0x1A4
    s32 pad1A8[(0x1E4 - 0x1A8) / 4];
    HU3DMODELID kinokoMdlId; // 0x1E4
    s16 pad1E6;
    HU3DMODELID togezoMdlId; // 0x1E8
    s16 pad1EA;
    HU3DMODELID baseShadowMdlId; // 0x1EC
    s16 pad1EE;
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
    s32 padAA0[(0xB60 - 0xAA0) / 4];

    s32 objCreateF;    // 0xB60
    SdStageObj *obj;   // 0xB64
    s32 unkB68;
    s32 unkB6C;
    s32 unkB70;
    s32 unkB74;
    HU3DDRAWOBJ *backObj[2][20]; // 0xB78
    s32 padC18[(0xFE8 - 0xC18) / 4];

    s32 smokeSlot;    // 0xFE8
    s32 frameCount;   // 0xFEC
    s32 fxId;         // 0xFF0
} SdSys;

extern SdSys *sys;

extern s32 sdCameraBitTbl[];

void SdStageInit(s32 stageNo);
void SdStageCreate(s32 stageNo);
void SdStageBackRotYSet(s32 machineNo, f32 rotY, f32 rotX);
void SdStageClose(void);
s32 SdStageFloorCheck(Vec *pos, Vec *normal);
s32 SdStageYOfsFloorCheck(Vec *pos, Vec *normal, f32 yOfs);
s32 SdStageWallCheck(Vec *pos, Vec *normal);
s32 SdStageYOfsWallCheck(Vec *pos, Vec *normal, f32 yOfs);
void SdStageDistCalc(Vec *pos);
f32 SdStageRadiusCalc(void);
void SdStageSprPosGet(s32 *type, Vec *radius);
void SdStageRandomPosGet(Vec *pos, f32 yOfs);
f32 SdStageEdgeDistGet(Vec *pos);
f32 SdStageEdgeDistRadiusGet(Vec *pos, f32 radius);
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
void SdImpactStart(s32 snd, Vec *pos, s32 no, void *arg, f32 a, f32 b);
void SdEffectDustCreateData(Vec *pos, void *p1, void *p2, f32 scale);
void SdEffectFlameUpdate(void *data, f32 time);
void SdEffectFlameUpdateData(void *data, s32 count);
void SdRandomPosGet(Vec *pos, s32 a, Vec *b, s32 c, void *d, f32 e, f32 f);
void SdPlayerSoundPlay(SdStageMachine *machine, s32 no);
s32 SdMachineDamage(SdStageMachine *machine, Vec *pos, s32 a, f32 b);

f32 MathLerp(f32 a, f32 b, f32 t);
void MathMtxRotGet(Mtx mtx, Vec *rot);
void MathPlaneRotMtxGet(Vec *v1, Vec *v2, f32 f, Mtx mtx);

#endif