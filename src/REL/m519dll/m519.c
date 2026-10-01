#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/main.h"
#include "game/memory.h"
#include "game/mg/seqman.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/sprite.h"
#include "game/wipe.h"

#include "math.h"
#include "humath.h"

#include "datanum/charmot.h"

#define M519_DATANUM(no) DATANUM(DATA_m519, (no))

/* Hu3DMotionMaxTimeGet is not declared in game/hu3d.h (the original m518.c saw
 * an implicit int return); this unit uses the float return, so declare it here. */
float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

/* This unit calls the random helpers without their prototypes: frand() is
   treated as signed and frandmod() as returning s16, and one call passes a
   float in f1 where frandmod expects an integer. The native build uses the
   real prototypes and passes the intended maximum. */
#ifdef TARGET_PC
#include "game/frand.h"

#define M519_FRANDMOD_F(x) ((s16)frandmod((u32)(x)))
#define M519_FRAND() ((s32)frand())
#define M519_FRANDMOD(x) ((s16)frandmod(x))
#else
extern s32 frand();
extern s32 frandmod();
#define M519_FRANDMOD_F(x) frandmod(x)
#define M519_FRAND() frand()
#define M519_FRANDMOD(x) frandmod(x)
#endif

typedef struct M519Light_s {
    HuVecF pos;
    HuVecF target;
    GXColor color;
} M519_LIGHT;

typedef struct M519Shadow_s {
    HuVecF pos;
    HuVecF up;
    HuVecF target;
} M519_SHADOW;

typedef struct M519StartPos_s {
    float dist;
    float angle;
    float rotY;
} M519_STARTPOS;

typedef struct M519CameraSet_s {
    float fov;
    float near;
    float far;
    float aspect;
    float vpX;
    float vpY;
    float vpW;
    float vpH;
    float vpNear;
    float vpFar;
    float scissorX;
    float scissorY;
    float scissorW;
    float scissorH;
} M519_CAMERASET;

typedef struct M519HudPos_s {
    s16 type;
    HuVec2f pos;
} M519_HUDPOS;

typedef struct M519ObjPos_s {
    s16 type;
    HuVecF pos;
    HuVecF rot;
} M519_OBJPOS;

typedef struct M519MechParam_s {
    float accel;
    float rotAccel;
    float radius;
    s16 hp;
} M519_MECHPARAM;

typedef struct M519MotParam_s {
    float start;
    float end;
    u32 attr;
} M519_MOTPARAM;

typedef struct M519ShotParam_s {
    float speed;
    float radiusEnd;
    float radiusStart;
    s16 life;
    s16 growTime;
    s16 damage;
    s16 knockback;
    BOOL pierceF;
} M519_SHOTPARAM;

typedef struct M519Particle_s {
    HU3DMODELID modelId;
    HuVecF pos;
    ANIMDATA *anim;
} M519_PARTICLE;

struct M519Obj_s;

typedef struct M519Stage_s {
    /* 0x00 */ s16 mode;
    /* 0x02 */ s16 prevMode;
    /* 0x04 */ HU3DMODELID modelId[7];
    /* 0x14 */ struct M519Obj_s *obj[12];
    /* 0x44 */ HUPROCESS *process;
} M519_STAGE;

typedef struct M519Mech_s {
    /* 0x00 */ s16 mode;
    /* 0x02 */ s16 prevMode;
    /* 0x04 */ s16 type;
    /* 0x06 */ s16 no;
    /* 0x08 */ s16 pilotNo;
    /* 0x0A */ HU3DMODELID modelId[9];
    /* 0x1C */ HU3DMOTID motId[7];
    /* 0x2A */ s16 motNo;
    /* 0x2C */ s16 motNoPrev;
    /* 0x2E */ s16 shotWait;
    /* 0x30 */ s16 hp;
    /* 0x32 */ s16 hpDisp;
    /* 0x34 */ s16 hpMax;
    /* 0x36 */ s16 chargeA;
    /* 0x38 */ s16 chargeB;
    /* 0x3A */ s16 damageNo;
    /* 0x3C */ s16 hitId;
    /* 0x3E */ s16 comAction;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ BOOL inputF;
    /* 0x48 */ BOOL boostF;
    /* 0x4C */ BOOL cameraF;
    /* 0x50 */ float speed;
    /* 0x54 */ float rotSpeed;
    /* 0x58 */ float speedTarget;
    /* 0x5C */ float rotSpeedTarget;
    /* 0x60 */ float rotAccel;
    /* 0x64 */ float accel;
    /* 0x68 */ float knockback;
    /* 0x6C */ float knockbackAngle;
    /* 0x70 */ float radius;
    /* 0x74 */ float turretRot;
    /* 0x78 */ HuVecF unk78;
    /* 0x84 */ HuVecF pos;
    /* 0x90 */ HuVecF rot;
    /* 0x9C */ HuVecF prevPos;
    /* 0xA8 */ HuVecF prevRot;
    /* 0xB4 */ M519_PARTICLE *effect[2];
    /* 0xBC */ HUPROCESS *process;
    /* 0xC0 */ BOOL collideF;
    /* 0xC4 */ BOOL comChargeF;
    /* 0xC8 */ s16 btnDown;
    /* 0xCA */ s16 btn;
    /* 0xCC */ s16 stkX;
    /* 0xCE */ s16 stkY;
    /* 0xD0 */ s16 trigL;
    /* 0xD2 */ s16 trigR;
    /* 0xD4 */ BOOL comTargetF;
    /* 0xD8 */ HuVecF comTarget;
    /* 0xE4 */ s16 comTargetNo;
    /* 0xE6 */ s16 comShotWait;
    /* 0xE8 */ s16 comTimer;
    /* 0xEA */ s16 comTimer2;
    /* 0xEC */ s16 comSpeed;
    /* 0xEE */ s16 comMode;
    /* 0xF0 */ int seNo;
    /* 0xF4 */ s32 unkF4;
    /* 0xF8 */ float stepTime;
} M519_MECH;

typedef struct M519Pilot_s {
    /* 0x00 */ s16 mode;
    /* 0x02 */ s16 prevMode;
    /* 0x04 */ s16 grp;
    /* 0x06 */ s16 playerNo;
    /* 0x08 */ s16 mechNo;
    /* 0x0A */ s16 charNo;
    /* 0x0C */ s32 comF;
    /* 0x10 */ s16 dif;
    /* 0x12 */ s16 padNo;
    /* 0x14 */ HU3DMODELID modelId[2];
    /* 0x18 */ HU3DMOTID motId[6];
    /* 0x24 */ HuVecF pos;
    /* 0x30 */ HuVecF rot;
    /* 0x3C */ HUPROCESS *process;
} M519_PILOT;

typedef struct M519Shot_s {
    /* 0x00 */ s16 mode;
    /* 0x02 */ s16 prevMode;
    /* 0x04 */ BOOL activeF;
    /* 0x08 */ BOOL pierceF;
    /* 0x0C */ s16 id;
    /* 0x0E */ s16 type;
    /* 0x10 */ s16 damage;
    /* 0x12 */ s16 knockback;
    /* 0x14 */ HU3DMODELID modelId[2];
    /* 0x18 */ s16 ownerNo;
    /* 0x1C */ float speed;
    /* 0x20 */ float curSpeed;
    /* 0x24 */ HuVecF pos;
    /* 0x30 */ HuVecF rot;
    /* 0x3C */ float radiusStart;
    /* 0x40 */ float radiusEnd;
    /* 0x44 */ float radius;
    /* 0x48 */ s16 life;
    /* 0x4A */ s16 growTime;
    /* 0x4C */ HUPROCESS *process;
} M519_SHOT;

typedef struct M519Obj_s {
    /* 0x00 */ s16 mode;
    /* 0x02 */ s16 prevMode;
    /* 0x04 */ s16 type;
    /* 0x06 */ HU3DMODELID modelId[3];
    /* 0x0C */ s16 hp;
    /* 0x0E */ s16 hitId;
    /* 0x10 */ s16 hitOwnerNo;
    /* 0x14 */ HuVecF pos;
    /* 0x20 */ HuVecF rot;
    /* 0x2C */ float radius;
    /* 0x30 */ BOOL respawnF;
    /* 0x34 */ HUPROCESS *process;
} M519_OBJ;

typedef struct M519Hud_s {
    /* 0x00 */ s16 mode;
    /* 0x02 */ s16 prevMode;
    /* 0x04 */ s16 type;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 mechNo;
    /* 0x0A */ s16 sprId[18];
    /* 0x30 */ HuVec2f pos;
    /* 0x38 */ HuVec2f ofs[18];
    /* 0xC8 */ HUPROCESS *process;
} M519_HUD;

typedef void (*M519_STAGEFUNC)(M519_STAGE *stage);
typedef void (*M519_MECHFUNC)(M519_MECH *mech);
typedef void (*M519_PILOTFUNC)(M519_PILOT *pilot);
typedef void (*M519_SHOTFUNC)(M519_SHOT *shot);
typedef void (*M519_OBJFUNC)(M519_OBJ *obj);
typedef void (*M519_PARTICLEFUNC)(M519_PARTICLE *particle, HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
typedef void (*M519_HUDFUNC)(M519_HUD *hud);

#define M519_SLEEP_MODECHG(type, tick) \
    do { \
        int i_; \
        s16 mode_; \
        s16 prevMode_; \
        int num_; \
        num_ = (tick); \
        for (i_ = 0; i_ < num_; i_++) { \
            HuPrcVSleep(); \
            mode_ = ((type *)HuPrcCurrentGet()->property)->mode; \
            prevMode_ = ((type *)HuPrcCurrentGet()->property)->prevMode; \
            ((type *)HuPrcCurrentGet()->property)->prevMode = mode_; \
            if (mode_ != prevMode_) { \
                return; \
            } \
        } \
    } while (0)

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

void ObjectSetup(void);
void fn_1_21C(void);
void fn_1_28C(s16 mode, s16 frameNo);
void fn_1_9F4(s16 mode, s16 frameNo);
void fn_1_D64(void);
void fn_1_143C(s16 mode, s16 frameNo);
void fn_1_14E8(s16 mode, s16 frameNo);
void fn_1_1628(s16 mode, s16 frameNo);
void fn_1_19C4(s16 mode, s16 frameNo);
void fn_1_1FC0(s16 mode, s16 frameNo);
void fn_1_2270(void);
M519_STAGE *fn_1_2298(void);
void fn_1_2654(void);
void fn_1_2694(M519_STAGE *stage);
void fn_1_26A0(M519_STAGE *stage);
void fn_1_2730(M519_STAGE *stage);
void fn_1_2790(M519_STAGE *stage);
void fn_1_27FC(M519_STAGE *stage);
void fn_1_2890(M519_STAGE *stage);
void fn_1_2900(M519_STAGE *stage);
M519_MECH *fn_1_297C(s16 type, s16 no);
void fn_1_2F54(void);
void fn_1_2FA0(M519_MECH *mech, s16 motNo, float speed);
void fn_1_3098(M519_MECH *mech);
void fn_1_3138(M519_MECH *mech, HuVecF pos);
HuVecF fn_1_3274(M519_MECH *mech);
void fn_1_3290(M519_MECH *mech, HuVecF rot);
HuVecF fn_1_34FC(M519_MECH *mech);
void fn_1_3518(M519_MECH *mech, M519_PILOT *pilot);
void fn_1_3580(M519_MECH *mech);
void fn_1_35B0(M519_MECH *mech);
void fn_1_35BC(M519_MECH *mech);
void fn_1_3658(M519_MECH *mech);
void fn_1_3664(M519_MECH *mech);
void fn_1_4D40(M519_MECH *mech);
void fn_1_4E18(M519_MECH *mech);
void fn_1_5EDC(M519_MECH *mech);
void fn_1_68E4(M519_MECH *mech);
void fn_1_69BC(M519_MECH *mech);
void fn_1_7998(M519_MECH *mech);
float fn_1_7EB8(M519_MECH *mech, HuVecF *pos);
float fn_1_7FFC(M519_MECH *mech, HuVecF *pos);
void fn_1_817C(M519_MECH *mech);
void fn_1_8188(M519_MECH *mech);
void fn_1_82E4(M519_MECH *mech);
void fn_1_8450(M519_MECH *mech);
void fn_1_8D40(M519_MECH *mech);
s16 fn_1_8DD0(M519_MECH *mech);
void fn_1_8E40(M519_MECH *mech);
void fn_1_95B0(M519_MECH *mech);
void fn_1_9700(M519_MECH *mech);
void fn_1_98A0(M519_MECH *mech);
void fn_1_98AC(M519_MECH *mech);
void fn_1_9B9C(M519_MECH *mech);
void fn_1_9BA8(M519_MECH *mech);
void fn_1_A068(M519_MECH *mech);
BOOL fn_1_A1D4(M519_MECH *mech);
M519_PILOT *fn_1_A204(s16 grp, s16 playerNo);
void fn_1_A40C(void);
void fn_1_A44C(M519_PILOT *pilot, HuVecF pos);
HuVecF fn_1_A4D0(M519_PILOT *pilot);
void fn_1_A4EC(M519_PILOT *pilot, HuVecF rot);
HuVecF fn_1_A570(M519_PILOT *pilot);
void fn_1_A58C(M519_PILOT *pilot);
void fn_1_A598(M519_PILOT *pilot);
void fn_1_A628(M519_PILOT *pilot);
void fn_1_A634(M519_PILOT *pilot);
void fn_1_A6B8(M519_PILOT *pilot);
void fn_1_A6C4(M519_PILOT *pilot);
void fn_1_A770(M519_PILOT *pilot);
void fn_1_A77C(M519_PILOT *pilot);
void fn_1_A828(M519_PILOT *pilot);
void fn_1_A834(M519_PILOT *pilot);
BOOL fn_1_A9F0(s16 type, HuVecF pos, HuVecF rot, s16 ownerNo);
M519_SHOT *fn_1_AB40(s16 type);
void fn_1_ADE4(void);
void fn_1_AE24(M519_SHOT *shot, HuVecF pos);
HuVecF fn_1_AEA8(M519_SHOT *shot);
void fn_1_AEC4(M519_SHOT *shot, HuVecF rot);
HuVecF fn_1_AF48(M519_SHOT *shot);
void fn_1_AF64(M519_SHOT *shot);
void fn_1_B004(M519_SHOT *shot);
void fn_1_B0B0(M519_SHOT *shot);
void fn_1_B0BC(M519_SHOT *shot);
BOOL fn_1_B70C(M519_SHOT *shot);
BOOL fn_1_B9BC(M519_SHOT *shot);
BOOL fn_1_BAC8(M519_SHOT *shot);
BOOL fn_1_BECC(M519_SHOT *shot);
M519_OBJ *fn_1_C058(s16 type);
void fn_1_C248(void);
void fn_1_C288(M519_OBJ *obj, HuVecF pos);
HuVecF fn_1_C30C(M519_OBJ *obj);
void fn_1_C328(M519_OBJ *obj, HuVecF rot);
HuVecF fn_1_C3AC(M519_OBJ *obj);
void fn_1_C3C8(M519_OBJ *obj);
void fn_1_C3D4(M519_OBJ *obj);
void fn_1_C464(M519_OBJ *obj);
void fn_1_C470(M519_OBJ *obj);
void fn_1_C544(M519_OBJ *obj);
void fn_1_C570(M519_OBJ *obj);
void fn_1_C788(M519_OBJ *obj, BOOL respawnF);
void fn_1_C798(M519_OBJ *obj);
void fn_1_CBEC(M519_OBJ *obj);
M519_PARTICLE *fn_1_CC44(void);
void fn_1_CCF4(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
void fn_1_CD54(M519_PARTICLE *particle, HuVecF pos);
HuVecF fn_1_CD70(M519_PARTICLE *particle);
void fn_1_CD8C(M519_PARTICLE *particle);
void fn_1_CDF0(M519_PARTICLE *particle, HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
void fn_1_CDF4(M519_PARTICLE *particle);
void fn_1_CEE4(M519_PARTICLE *particle, HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
BOOL fn_1_CFE8(M519_PARTICLE *particle);
M519_HUD *fn_1_D0BC(s16 type, s16 mechNo);
void fn_1_D610(void);
void fn_1_D65C(M519_HUD *hud, HuVec2f pos);
HuVec2f fn_1_D708(M519_HUD *hud);
void fn_1_D714(M519_HUD *hud);
void fn_1_D720(M519_HUD *hud);
void fn_1_D7B0(M519_HUD *hud);
void fn_1_D7BC(M519_HUD *hud);
void fn_1_DD08(M519_HUD *hud);
void fn_1_E168(M519_HUD *hud);
void fn_1_E174(M519_HUD *hud);
void fn_1_E298(M519_HUD *hud);
void fn_1_E2D4(M519_HUD *hud);
void fn_1_E714(M519_HUD *hud);
void fn_1_EA10(M519_HUD *hud);
void fn_1_ED78(M519_HUD *hud);
void fn_1_EE8C(M519_HUD *hud);
void fn_1_F104(M519_HUD *hud);
void fn_1_F2DC(M519_HUD *hud);
void fn_1_F2E8(M519_HUD *hud);
void fn_1_F568(M519_HUD *hud);
void fn_1_F7F0(M519_HUD *hud);
BOOL fn_1_F860(HuVecF *a, HuVecF *b, BOOL pushF);
BOOL fn_1_FA6C(HuVecF *a, HuVecF *b, BOOL pushF);
float fn_1_FC78(float start, float end, float t);
void fn_1_FD7C(s16 playerNo, s16 type, s16 time);
s16 fn_1_FE20(u16 cameraBit, HuVecF pos);
s16 fn_1_10244(u16 cameraBit, HuVecF pos);

const u16 lbl_1_rodata_10[4] = { HU3D_CAM0, HU3D_CAM1, HU3D_CAM2, HU3D_CAM3 };
const s16 lbl_1_rodata_18[2] = { 32, 96 };
const s16 lbl_1_rodata_1C[2] = { 48, 80 };
const M519_STARTPOS lbl_1_rodata_20[4] = {
    { 0.0f, 0.0f, 0.0f },
    { 1800.0f, 180.0f, 180.0f },
    { 1800.0f, 45.0f, 45.0f },
    { 1800.0f, 315.0f, 315.0f },
};
const M519_LIGHT lbl_1_rodata_50[3] = {
    { { 0.0f, 1200.0f, 5.0f }, { 0.0f, 0.0f, -5.0f }, { 255, 255, 255, 255 } },
    { { 0.0f, 1200.0f, 5.0f }, { 0.0f, 0.0f, -5.0f }, { 255, 255, 255, 255 } },
    { { 0.0f, 1200.0f, 5.0f }, { 0.0f, 0.0f, -5.0f }, { 255, 255, 255, 255 } },
};
const M519_LIGHT lbl_1_rodata_A4[3] = {
    { { 0.0f, 1500.0f, 1500.0f }, { 0.0f, 0.0f, -1500.0f }, { 255, 255, 255, 255 } },
    { { 0.0f, 1500.0f, 1500.0f }, { 0.0f, 0.0f, -1500.0f }, { 255, 255, 255, 255 } },
    { { 0.0f, 1500.0f, 1500.0f }, { 0.0f, 0.0f, -1500.0f }, { 255, 255, 255, 255 } },
};
const M519_SHADOW lbl_1_rodata_F8[3] = {
    { { 0.0f, 5000.0f, 100.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
    { { 0.0f, 5000.0f, 100.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
    { { 0.0f, 5000.0f, 100.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
};
const int lbl_1_rodata_164[CHARNO_MAX] = { 505, 485, 605, 685, 645, 445, 665, 465, 625, 565, 585, 545, 525 };
const M519_CAMERASET lbl_1_rodata_198[4] = {
    { 50.0f, 10.0f, 24000.0f, 1.2f, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f, 0.0f, 0.0f, 640.0f, 480.0f },
    { 50.0f, 10.0f, 24000.0f, 1.7625275f, 596.0f, 0.0f, 364.0f, 213.33f, 0.0f, 1.0f, 640.0f, 0.0f, 320.0f, 173.33f },
    { 50.0f, 10.0f, 24000.0f, 2.1692724f, 596.0f, 153.33f, 364.0f, 153.33f, 0.0f, 1.0f, 640.0f, 173.33f, 320.0f, 133.33f },
    { 50.0f, 10.0f, 24000.0f, 1.7625275f, 596.0f, 286.66f, 276.0f, 193.33f, 0.0f, 1.0f, 640.0f, 306.66f, 320.0f, 173.33f },
};
const int lbl_1_rodata_278[3] = { M519_DATANUM(71), M519_DATANUM(72), M519_DATANUM(72) };
const M519_HUDPOS lbl_1_rodata_284[4] = {
    { 0, { 128.0f, 520.0f } },
    { 1, { 640.0f, 87.0f } },
    { 2, { 640.0f, 240.5f } },
    { 3, { 640.0f, 394.0f } },
};

char *lbl_1_data_14[2] = { "itemhook_c", "itemfook" };

M519_STAGEFUNC lbl_1_data_1C[1] = { fn_1_26A0 };

M519_MECHFUNC lbl_1_data_20[2][5] = {
    { fn_1_35BC, fn_1_3664, fn_1_8188, fn_1_98AC, fn_1_9BA8 },
    { fn_1_35BC, fn_1_5EDC, fn_1_8188, fn_1_98AC, fn_1_A068 },
};

M519_PILOTFUNC lbl_1_data_48[5] = { fn_1_A598, fn_1_A634, fn_1_A6C4, fn_1_A77C, fn_1_A834 };

M519_SHOTFUNC lbl_1_data_5C[2] = { fn_1_B004, fn_1_B0BC };

M519_OBJFUNC lbl_1_data_64[4] = { fn_1_C3D4, fn_1_C570, fn_1_C798, fn_1_C470 };

M519_PARTICLEFUNC lbl_1_data_74[2] = { fn_1_CDF0, fn_1_CEE4 };

M519_HUDFUNC lbl_1_data_7C[4][4] = {
    { fn_1_D720, fn_1_D7BC, fn_1_E174, fn_1_F2E8 },
    { fn_1_D720, fn_1_DD08, fn_1_ED78, fn_1_F568 },
    { fn_1_D720, fn_1_DD08, fn_1_ED78, fn_1_F568 },
    { fn_1_D720, fn_1_DD08, fn_1_ED78, fn_1_F568 },
};

OMOBJ *lbl_1_bss_17C;
M519_STAGE *lbl_1_bss_178;
OMCAMERAVIEW lbl_1_bss_108[4];
M519_MECH *lbl_1_bss_F8[4];
M519_PILOT *lbl_1_bss_E8[4];
M519_SHOT *lbl_1_bss_38[44];
s16 lbl_1_bss_32;
s16 lbl_1_bss_30;
M519_HUD *lbl_1_bss_20[4];
s16 lbl_1_bss_18[3];
int lbl_1_bss_14;
int lbl_1_bss_10;
s16 lbl_1_bss_C;
BOOL lbl_1_bss_8;
BOOL lbl_1_bss_4;
BOOL lbl_1_bss_0;

int _prolog(void)
{
    const VoidFunc *ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    OMOBJMAN *objman;
    MGSEQ_PARAM seqParam;

    objman = omInitObjMan(1, 8192);
    lbl_1_bss_17C = omAddObjEx(objman, 4096, 0, 0, 0, omOutViewMulti);
    lbl_1_bss_17C->work[0] = 4;
    omGameSysInit(objman);
    Hu3DNoSyncSet(TRUE);
    seqParam.maxTime = 60;
    seqParam.timerPos = MGSEQ_TIMER_TOP;
    seqParam.initHook = fn_1_28C;
    seqParam.fadeInHook = fn_1_9F4;
    seqParam.startHook = fn_1_143C;
    seqParam.mainHook = fn_1_14E8;
    seqParam.finishHook = fn_1_1628;
    seqParam.preWinnerHook = fn_1_19C4;
    seqParam.winnerHook = fn_1_1FC0;
    seqParam.fadeOutHook = NULL;
    seqParam.closeHook = NULL;
    MgSeqCreate(&seqParam);
    if (GWSubGameNoGet() == -1) {
        lbl_1_bss_30 = (s32)GlobalCounter % 3;
    }
    else {
        lbl_1_bss_30 = GWSubGameNoGet();
    }
}

void fn_1_21C(void)
{
    while (!omSysExitReq) {
        HuPrcVSleep();
    }
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
    omOvlReturnEx(1, 1);
    HuPrcEnd();
}

void fn_1_28C(s16 mode, s16 frameNo)
{
    int i;
    int num;
    int shotNo;
    s16 grp[4];
    HuVec2f pos;

    switch (frameNo) {
        case 0:
    lbl_1_bss_178 = fn_1_2298();
    for (i = 0; i < 4; i++) {
        switch (i) {
            case 0:
                lbl_1_bss_F8[i] = fn_1_297C(0, i);
                break;
            case 1:
            case 2:
            case 3:
                lbl_1_bss_F8[i] = fn_1_297C(1, i);
                break;
        }
    }
    for (i = 0; i < 4; i++) {
        grp[i] = GwPlayerConf[i].grp;
    }
    num = 0;
    for (i = 0; i < 4; i++) {
        if (grp[i] == 0) {
            num++;
        }
    }
    if (num == 1) {
        for (i = 0; i < 4; i++) {
            if (grp[i] != 0) {
                grp[i] = 1;
            }
        }
    }
    else {
        grp[0] = 0;
        grp[1] = 1;
        grp[2] = 1;
        grp[3] = 1;
    }
    for (i = 0; i < 4; i++) {
        lbl_1_bss_E8[i] = fn_1_A204(grp[i], i);
    }
    for (i = 0; i < 4; i++) {
        if (grp[i] == 0) {
            fn_1_3518(lbl_1_bss_F8[0], lbl_1_bss_E8[i]);
            break;
        }
    }
    num = 1;
    for (i = 0; i < 4; i++) {
        if (grp[i] != 0) {
            fn_1_3518(lbl_1_bss_F8[num++], lbl_1_bss_E8[i]);
        }
    }
    shotNo = 0;
    for (i = 0; i < 1; i++, shotNo++) {
        lbl_1_bss_38[shotNo] = fn_1_AB40(3);
    }
    for (i = 0; i < 2; i++, shotNo++) {
        lbl_1_bss_38[shotNo] = fn_1_AB40(2);
    }
    for (i = 0; i < 1; i++, shotNo++) {
        lbl_1_bss_38[shotNo] = fn_1_AB40(1);
    }
    for (i = 0; i < 8; i++, shotNo++) {
        lbl_1_bss_38[shotNo] = fn_1_AB40(0);
    }
    for (i = 0; i < 32; i++, shotNo++) {
        lbl_1_bss_38[shotNo] = fn_1_AB40(4);
    }
    for (i = 0; i < 4; i++) {
        lbl_1_bss_20[i] = fn_1_D0BC(lbl_1_rodata_284[i].type, i);
        fn_1_D65C(lbl_1_bss_20[i], lbl_1_rodata_284[i].pos);
    }
    for (i = 0; i < 3; i++) {
        lbl_1_bss_18[i] = espEntry(lbl_1_rodata_278[i], 0, 0);
        espPriSet(lbl_1_bss_18[i], 500);
    }
    lbl_1_bss_32 = Hu3DGLightCreate(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    Hu3DGLightPosAimSetV(lbl_1_bss_32, (HuVecF *)&lbl_1_rodata_A4[lbl_1_bss_30].pos, (HuVecF *)&lbl_1_rodata_A4[lbl_1_bss_30].target);
    Hu3DGLightColorSet(lbl_1_bss_32, lbl_1_rodata_A4[lbl_1_bss_30].color.r, lbl_1_rodata_A4[lbl_1_bss_30].color.g, lbl_1_rodata_A4[lbl_1_bss_30].color.b,
        lbl_1_rodata_A4[lbl_1_bss_30].color.a);
    Hu3DGLightStaticSet(lbl_1_bss_32, FALSE);
    Hu3DCameraCreate(HU3D_CAM0 | HU3D_CAM1 | HU3D_CAM2 | HU3D_CAM3);
    for (i = 0; i < 4; i++) {
        s16 cameraBit = lbl_1_rodata_10[i];
        Hu3DCameraPerspectiveSet(cameraBit, lbl_1_rodata_198[i].fov, lbl_1_rodata_198[i].near, lbl_1_rodata_198[i].far, lbl_1_rodata_198[i].aspect);
        Hu3DCameraViewportSet(cameraBit, lbl_1_rodata_198[i].vpX, lbl_1_rodata_198[i].vpY, lbl_1_rodata_198[i].vpW, lbl_1_rodata_198[i].vpH,
            lbl_1_rodata_198[i].vpNear, lbl_1_rodata_198[i].vpFar);
        Hu3DCameraScissorSet(cameraBit, lbl_1_rodata_198[i].scissorX, lbl_1_rodata_198[i].scissorY, lbl_1_rodata_198[i].scissorW,
            lbl_1_rodata_198[i].scissorH);
    }
            break;
    }
    MgSeqModeNext();
}

void fn_1_9F4(s16 mode, s16 frameNo)
{
    int i;
    float dist;
    float angle;
    float rotY;
    HuVecF pos;
    HuVecF rot;

    switch (frameNo) {
        case 0:
            lbl_1_bss_10 = HuAudFXPlay(1800);
            for (i = 0; i < 4; i++) {
                dist = lbl_1_rodata_20[i].dist;
                angle = lbl_1_rodata_20[i].angle;
                rotY = lbl_1_rodata_20[i].rotY;
                pos.x = dist * HuSin(angle);
                pos.z = dist * HuCos(angle);
                pos.y = 0.0f;
                rot.x = 0.0f;
                rot.z = 0.0f;
                rot.y = rotY;
                fn_1_3138(lbl_1_bss_F8[i], pos);
                fn_1_3290(lbl_1_bss_F8[i], rot);
                fn_1_9B9C(lbl_1_bss_F8[i]);
            }
            for (i = 0; i < 4; i++) {
                lbl_1_bss_108[i].center.x = 0.0f;
                lbl_1_bss_108[i].center.y = 0.0f;
                lbl_1_bss_108[i].center.z = 0.0f;
                lbl_1_bss_108[i].rot.x = -10.0f;
                lbl_1_bss_108[i].rot.y = 0.0f;
                lbl_1_bss_108[i].rot.z = 0.0f;
                lbl_1_bss_108[i].zoom = 1000.0f;
                omCameraViewSetMulti(lbl_1_rodata_10[i], &lbl_1_bss_108[i]);
            }
            break;
        case 120:
            HuAudFXStop(lbl_1_bss_10);
            break;
        case 180:
            fn_1_2730(lbl_1_bss_178);
            fn_1_D64();
            fn_1_2790(lbl_1_bss_178);
            MgSeqModeNext();
            break;
    }
}

void fn_1_D64(void)
{
    static const M519_CAMERASET cameraTbl[2][4] = {
        {
            { 50.0f, 100.0f, 24000.0f, 1.2f, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f, 0.0f, 0.0f, 640.0f, 480.0f },
            { 50.0f, 100.0f, 24000.0f, 1.7625275f, 596.0f, 0.0f, 364.0f, 213.33f, 0.0f, 1.0f, 640.0f, 0.0f, 320.0f, 173.33f },
            { 50.0f, 100.0f, 24000.0f, 2.1692724f, 596.0f, 153.33f, 364.0f, 153.33f, 0.0f, 1.0f, 640.0f, 173.33f, 320.0f, 133.33f },
            { 50.0f, 100.0f, 24000.0f, 1.7625275f, 596.0f, 286.66f, 276.0f, 193.33f, 0.0f, 1.0f, 640.0f, 306.66f, 320.0f, 173.33f },
        },
        {
            { 50.0f, 100.0f, 24000.0f, 0.6f, 0.0f, 0.0f, 320.0f, 480.0f, 0.0f, 1.0f, 0.0f, 0.0f, 320.0f, 480.0f },
            { 50.0f, 100.0f, 24000.0f, 1.7625275f, 276.0f, 0.0f, 364.0f, 213.33f, 0.0f, 1.0f, 320.0f, 0.0f, 320.0f, 173.33f },
            { 50.0f, 100.0f, 24000.0f, 1.7625275f, 276.0f, 153.33f, 364.0f, 193.33f, 0.0f, 1.0f, 320.0f, 173.33f, 320.0f, 133.33f },
            { 50.0f, 100.0f, 24000.0f, 1.7625275f, 276.0f, 286.66f, 364.0f, 193.33f, 0.0f, 1.0f, 320.0f, 306.66f, 320.0f, 173.33f },
        },
    };
    int i;
    int time;
    s16 cameraBit;
    float t;
    float h;
    float x;
    float y;
    float w;
    float near;
    float far;
    float fov;
    float aspect;

    for (time = 1; time <= 60; time++) {
        for (i = 0; i < 4; i++) {
            cameraBit = lbl_1_rodata_10[i];
            t = HuSin(90.0f * ((1.0f / 60.0f) * time));
            fov = cameraTbl[0][i].fov + (t * (cameraTbl[1][i].fov - cameraTbl[0][i].fov));
            near = cameraTbl[0][i].near + (t * (cameraTbl[1][i].near - cameraTbl[0][i].near));
            far = cameraTbl[0][i].far + (t * (cameraTbl[1][i].far - cameraTbl[0][i].far));
            aspect = cameraTbl[0][i].aspect + (t * (cameraTbl[1][i].aspect - cameraTbl[0][i].aspect));
            Hu3DCameraPerspectiveSet(cameraBit, fov, near, far, aspect);
            x = cameraTbl[0][i].vpX + (t * (cameraTbl[1][i].vpX - cameraTbl[0][i].vpX));
            y = cameraTbl[0][i].vpY + (t * (cameraTbl[1][i].vpY - cameraTbl[0][i].vpY));
            w = cameraTbl[0][i].vpW + (t * (cameraTbl[1][i].vpW - cameraTbl[0][i].vpW));
            h = cameraTbl[0][i].vpH + (t * (cameraTbl[1][i].vpH - cameraTbl[0][i].vpH));
            near = cameraTbl[0][i].vpNear + (t * (cameraTbl[1][i].vpNear - cameraTbl[0][i].vpNear));
            far = cameraTbl[0][i].vpFar + (t * (cameraTbl[1][i].vpFar - cameraTbl[0][i].vpFar));
            Hu3DCameraViewportSet(cameraBit, x, y, w, h, near, far);
            x = cameraTbl[0][i].scissorX + (t * (cameraTbl[1][i].scissorX - cameraTbl[0][i].scissorX));
            y = cameraTbl[0][i].scissorY + (t * (cameraTbl[1][i].scissorY - cameraTbl[0][i].scissorY));
            w = cameraTbl[0][i].scissorW + (t * (cameraTbl[1][i].scissorW - cameraTbl[0][i].scissorW));
            h = cameraTbl[0][i].scissorH + (t * (cameraTbl[1][i].scissorH - cameraTbl[0][i].scissorH));
            Hu3DCameraScissorSet(cameraBit, x, y, w, h);
            switch (i) {
                case 0:
                    espPosSet(lbl_1_bss_18[0], 0.9f * w, 239.0f);
                    break;
                case 1:
                    espPosSet(lbl_1_bss_18[1], 16.0f + (0.9f * (160.0f + x)), (y + h) - 1.0f);
                    break;
                case 2:
                    espPosSet(lbl_1_bss_18[2], 16.0f + (0.9f * (160.0f + x)), (y + h) - 1.0f);
                    break;
            }
        }
        HuPrcVSleep();
    }
}

void fn_1_143C(s16 mode, s16 frameNo)
{
    int i;
    int j;

    switch (frameNo) {
        case 0:
            lbl_1_bss_14 = HuAudSStreamPlay(62);
            for (i = 0; i < 4; i++) {
                fn_1_D7B0(lbl_1_bss_20[i]);
                for (j = 0; j < 12; j++) {
                    HuPrcVSleep();
                }
            }
            break;
        case 120:
            MgSeqModeNext();
            break;
    }
}

void fn_1_14E8(s16 mode, s16 frameNo)
{
    int i;

    switch (frameNo) {
        case 0:
            for (i = 0; i < 4; i++) {
                fn_1_3658(lbl_1_bss_F8[i]);
                lbl_1_bss_F8[i]->inputF = TRUE;
            }
            break;
        default:
            if (fn_1_A1D4(lbl_1_bss_F8[0])) {
                lbl_1_bss_0 = TRUE;
            }
            if (fn_1_A1D4(lbl_1_bss_F8[1]) && fn_1_A1D4(lbl_1_bss_F8[2]) && fn_1_A1D4(lbl_1_bss_F8[3])) {
                lbl_1_bss_4 = TRUE;
            }
            if (lbl_1_bss_0 || lbl_1_bss_4) {
                MgSeqModeNext();
            }
            if ((u32)MgSeqTimerValueGet() == 0) {
                MgSeqModeNext();
            }
            break;
    }
}

void fn_1_1628(s16 mode, s16 frameNo)
{
    int i;
    M519_MECH *mech;

    switch (frameNo) {
        case 0:
            lbl_1_bss_8 = TRUE;
            HuAudSStreamFadeOut(lbl_1_bss_14, 1000);
            for (i = 0; i < 4; i++) {
                mech = lbl_1_bss_F8[i];
                mech->inputF = FALSE;
                mech->chargeA = 0;
                mech->chargeB = 0;
                fn_1_8E40(mech);
            }
            for (i = 0; i < 4; i++) {
                mech = lbl_1_bss_F8[i];
                if (!fn_1_A1D4(mech)) {
                    fn_1_F2DC(lbl_1_bss_20[mech->no]);
                }
            }
            break;
        case 30:
            if ((!lbl_1_bss_0 && !lbl_1_bss_4) || (lbl_1_bss_0 && lbl_1_bss_4)) {
                MgSeqDrawSet();
            }
            else if (lbl_1_bss_0) {
                MgSeqWinnerSet(lbl_1_bss_E8[lbl_1_bss_F8[1]->pilotNo]->charNo, lbl_1_bss_E8[lbl_1_bss_F8[2]->pilotNo]->charNo,
                    lbl_1_bss_E8[lbl_1_bss_F8[3]->pilotNo]->charNo, CHARNO_NONE);
                GWMgCoinBonusSet(lbl_1_bss_E8[lbl_1_bss_F8[1]->pilotNo]->playerNo, 10);
                GWMgCoinBonusSet(lbl_1_bss_E8[lbl_1_bss_F8[2]->pilotNo]->playerNo, 10);
                GWMgCoinBonusSet(lbl_1_bss_E8[lbl_1_bss_F8[3]->pilotNo]->playerNo, 10);
            }
            else if (lbl_1_bss_4) {
                MgSeqWinnerSet(lbl_1_bss_E8[lbl_1_bss_F8[0]->pilotNo]->charNo, CHARNO_NONE, CHARNO_NONE, CHARNO_NONE);
                GWMgCoinBonusSet(lbl_1_bss_E8[lbl_1_bss_F8[0]->pilotNo]->playerNo, 10);
            }
            break;
    }
}

void fn_1_19C4(s16 mode, s16 frameNo)
{
    static const float posXTbl[4] = { -320.0f, -60.0f, 130.0f, 320.0f };
    static const s16 panTbl[4] = { 40, 56, 72, 88 };
    int i;
    M519_MECH *mech;
    s16 cameraBit;
    OMCAMERAVIEW *view;
    HuVecF pos;
    HuVecF rot;

    switch (frameNo) {
        case 60:
            WipeColorSet(255, 255, 255);
            WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
            HuAudFadeOut(30);
            break;
        case 90:
            for (i = 0; i < 4; i++) {
                fn_1_F7F0(lbl_1_bss_20[i]);
            }
            fn_1_2900(lbl_1_bss_178);
            pos.x = 0.0f;
            pos.y = -5000.0f;
            pos.z = 0.0f;
            for (i = 0; i < 4; i++) {
                mech = lbl_1_bss_F8[i];
                fn_1_A628(lbl_1_bss_E8[mech->pilotNo]);
                fn_1_35B0(mech);
                fn_1_3138(mech, pos);
                mech->cameraF = FALSE;
            }
            for (i = 0; i < 4; i++) {
                cameraBit = lbl_1_rodata_10[i];
                mech = lbl_1_bss_F8[i];
                switch (i) {
                    case 0:
                        Hu3DCameraPerspectiveSet(cameraBit, 50.0f, 10.0f, 24000.0f, 1.2f);
                        Hu3DCameraViewportSet(cameraBit, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
                        Hu3DCameraScissorSet(cameraBit, 0, 0, 640, 480);
                        view = &lbl_1_bss_108[mech->no];
                        view->center.x = 0.0 * HuSin(0.0);
                        view->center.z = 0.0 * HuCos(0.0);
                        view->center.y = 150.0f;
                        view->zoom = 600.0f;
                        view->rot.y = 0.0f;
                        view->rot.x = -15.0f;
                        view->rot.z = 0.0f;
                        omCameraViewMoveMulti(lbl_1_rodata_10[0], view, 1, 1);
                        break;
                    case 1:
                    case 2:
                    case 3:
                        break;
                }
            }
            Hu3DCameraKill(HU3D_CAM1 | HU3D_CAM2 | HU3D_CAM3);
            lbl_1_bss_17C->work[0] = 1;
            for (i = 0; i < 3; i++) {
                espDispOff(lbl_1_bss_18[i]);
            }
            for (i = 0; i < 4; i++) {
                mech = lbl_1_bss_F8[i];
                pos.x = posXTbl[i];
                pos.y = 0.0f;
                pos.z = 0.0f;
                fn_1_A44C(lbl_1_bss_E8[mech->pilotNo], pos);
                rot.x = 0.0f;
                rot.y = 0.0f;
                rot.z = 0.0f;
                fn_1_A4EC(lbl_1_bss_E8[mech->pilotNo], rot);
                CharModelVoicePanSet(lbl_1_bss_E8[mech->pilotNo]->charNo, panTbl[i]);
            }
            break;
        case 120:
            WipeColorSet(255, 255, 255);
            WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
            mech = lbl_1_bss_F8[0];
            view = &lbl_1_bss_108[mech->no];
            view->center.x = 0.0 * HuSin(0.0);
            view->center.z = 0.0 * HuCos(0.0);
            view->center.y = 150.0f;
            view->zoom = 1500.0f;
            view->rot.y = 0.0f;
            view->rot.x = -15.0f;
            view->rot.z = 0.0f;
            omCameraViewMoveMulti(lbl_1_rodata_10[0], view, 400, 2);
            break;
        case 180:
            MgSeqModeNext();
            break;
    }
}

void fn_1_1FC0(s16 mode, s16 frameNo)
{
    int i;
    M519_MECH *mech;

    switch (frameNo) {
        case 0:
            if ((!lbl_1_bss_0 && !lbl_1_bss_4) || (lbl_1_bss_0 && lbl_1_bss_4)) {
                CharLoseVoicePlay(lbl_1_bss_E8[lbl_1_bss_F8[0]->pilotNo]->charNo, lbl_1_bss_E8[lbl_1_bss_F8[1]->pilotNo]->charNo,
                    lbl_1_bss_E8[lbl_1_bss_F8[2]->pilotNo]->charNo, lbl_1_bss_E8[lbl_1_bss_F8[3]->pilotNo]->charNo);
                for (i = 0; i < 4; i++) {
                    mech = lbl_1_bss_F8[i];
                    fn_1_A770(lbl_1_bss_E8[mech->pilotNo]);
                }
            }
            else if (lbl_1_bss_0) {
                for (i = 0; i < 4; i++) {
                    mech = lbl_1_bss_F8[i];
                    if (i == 0) {
                        fn_1_A770(lbl_1_bss_E8[mech->pilotNo]);
                    }
                    else {
                        fn_1_A6B8(lbl_1_bss_E8[mech->pilotNo]);
                    }
                }
            }
            else if (lbl_1_bss_4) {
                for (i = 0; i < 4; i++) {
                    mech = lbl_1_bss_F8[i];
                    if (i == 0) {
                        fn_1_A6B8(lbl_1_bss_E8[mech->pilotNo]);
                    }
                    else {
                        fn_1_A770(lbl_1_bss_E8[mech->pilotNo]);
                    }
                }
            }
            fn_1_27FC(lbl_1_bss_178);
            break;
        case 179:
            HuAudFXStop(lbl_1_bss_10);
            break;
    }
}

void fn_1_2270(void)
{
    omOvlReturnEx(0, 1);
}

M519_STAGE *fn_1_2298(void)
{
    static const int modelTbl[3][7] = {
        { M519_DATANUM(0), M519_DATANUM(3), M519_DATANUM(1), M519_DATANUM(2), M519_DATANUM(4), M519_DATANUM(5), M519_DATANUM(6) },
        { M519_DATANUM(0), M519_DATANUM(13), M519_DATANUM(1), M519_DATANUM(2), -1, -1, -1 },
        { M519_DATANUM(0), M519_DATANUM(23), M519_DATANUM(1), M519_DATANUM(2), M519_DATANUM(24), M519_DATANUM(25), -1 },
    };
    static const GXColor bgColorTbl[3] = {
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 100, 110, 120, 0 },
    };
    static const M519_OBJPOS objPosTbl[3][12] = {
        {
            { 0, { 800.0f, 0.0f, -800.0f }, { 0.0f, 0.0f, 0.0f } },
            { 0, { 800.0f, 0.0f, 800.0f }, { 0.0f, 0.0f, 0.0f } },
            { 0, { -800.0f, 0.0f, -800.0f }, { 0.0f, 0.0f, 0.0f } },
            { 0, { -800.0f, 0.0f, 800.0f }, { 0.0f, 0.0f, 0.0f } },
            { 1, { -1500.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { 1, { 1500.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { 1, { -600.0f, 0.0f, -1500.0f }, { 0.0f, 0.0f, 0.0f } },
            { 1, { -600.0f, 0.0f, 1500.0f }, { 0.0f, 0.0f, 0.0f } },
            { 1, { 600.0f, 0.0f, -1500.0f }, { 0.0f, 0.0f, 0.0f } },
            { 1, { 600.0f, 0.0f, 1500.0f }, { 0.0f, 0.0f, 0.0f } },
            { 2, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { 2, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
        },
        {
            { 0, { 0.0f, 0.0f, 1000.0f }, { 0.0f, 0.0f, 0.0f } },
            { 0, { 600.0f, 0.0f, -1000.0f }, { 0.0f, 0.0f, 0.0f } },
            { 0, { -600.0f, 0.0f, -1000.0f }, { 0.0f, 0.0f, 0.0f } },
            { 1, { -600.0f, 0.0f, 500.0f }, { 0.0f, 0.0f, 0.0f } },
            { 2, { -850.0f, 0.0f, 500.0f }, { 0.0f, 0.0f, 0.0f } },
            { 1, { -1100.0f, 0.0f, 500.0f }, { 0.0f, 0.0f, 0.0f } },
            { 1, { 600.0f, 0.0f, 500.0f }, { 0.0f, 0.0f, 0.0f } },
            { 2, { 850.0f, 0.0f, 500.0f }, { 0.0f, 0.0f, 0.0f } },
            { 1, { 1100.0f, 0.0f, 500.0f }, { 0.0f, 0.0f, 0.0f } },
            { 1, { 0.0f, 0.0f, -1000.0f }, { 0.0f, 0.0f, 0.0f } },
            { 2, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { 2, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
        },
        {
            { 1, { -450.0f, 0.0f, -450.0f }, { 0.0f, 0.0f, 0.0f } },
            { 1, { -450.0f, 0.0f, 450.0f }, { 0.0f, 0.0f, 0.0f } },
            { 1, { 450.0f, 0.0f, -450.0f }, { 0.0f, 0.0f, 0.0f } },
            { 1, { 450.0f, 0.0f, 450.0f }, { 0.0f, 0.0f, 0.0f } },
            { 0, { -1350.0f, 0.0f, -450.0f }, { 0.0f, 0.0f, 0.0f } },
            { 0, { -1350.0f, 0.0f, 450.0f }, { 0.0f, 0.0f, 0.0f } },
            { 0, { 1350.0f, 0.0f, -450.0f }, { 0.0f, 0.0f, 0.0f } },
            { 0, { 1350.0f, 0.0f, 450.0f }, { 0.0f, 0.0f, 0.0f } },
            { 0, { -450.0f, 0.0f, -1350.0f }, { 0.0f, 0.0f, 0.0f } },
            { 0, { -450.0f, 0.0f, 1350.0f }, { 0.0f, 0.0f, 0.0f } },
            { 2, { 450.0f, 0.0f, -1350.0f }, { 0.0f, 0.0f, 0.0f } },
            { 2, { 450.0f, 0.0f, 1350.0f }, { 0.0f, 0.0f, 0.0f } },
        },
    };
    M519_STAGE *stage;
    int i;
    s16 type;

    stage = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M519_STAGE), HU_MEMNUM_OVL);
    stage->mode = 0;
    stage->prevMode = 0;
    for (i = 0; i < 7; i++) {
        if (modelTbl[lbl_1_bss_30][i] != -1) {
            stage->modelId[i] = Hu3DModelCreateData(modelTbl[lbl_1_bss_30][i]);
            Hu3DModelAttrSet(stage->modelId[i], HU3D_ATTR_NOCULL);
        }
        else {
            stage->modelId[i] = -1;
        }
    }
    Hu3DModelCameraSet(stage->modelId[2], HU3D_CAM0);
    Hu3DModelCameraSet(stage->modelId[3], HU3D_CAM0);
    Hu3DModelAttrSet(stage->modelId[1], HU3D_MOTATTR_LOOP);
    if (stage->modelId[4] != -1) {
        Hu3DModelAttrSet(stage->modelId[4], HU3D_MOTATTR_LOOP);
    }
    if (stage->modelId[5] != -1) {
        Hu3DModelAttrSet(stage->modelId[5], HU3D_MOTATTR_LOOP);
    }
    if (stage->modelId[6] != -1) {
        Hu3DModelAttrSet(stage->modelId[6], HU3D_MOTATTR_LOOP);
    }
    Hu3DModelLayerSet(stage->modelId[0], 1);
    Hu3DModelLayerSet(stage->modelId[1], 0);
    Hu3DModelLayerSet(stage->modelId[2], 1);
    Hu3DModelLayerSet(stage->modelId[3], 2);
    if (stage->modelId[4] != -1) {
        Hu3DModelLayerSet(stage->modelId[4], 0);
    }
    if (stage->modelId[5] != -1) {
        Hu3DModelLayerSet(stage->modelId[5], 0);
    }
    if (stage->modelId[6] != -1) {
        Hu3DModelLayerSet(stage->modelId[6], 0);
    }
    Hu3DBGColorSet(bgColorTbl[lbl_1_bss_30].r, bgColorTbl[lbl_1_bss_30].g, bgColorTbl[lbl_1_bss_30].b);
    stage->process = HuPrcChildCreate(fn_1_2654, 4096, 4096, 0, HuPrcCurrentGet());
    stage->process->property = stage;
    for (i = 0; i < 12; i++) {
        type = objPosTbl[lbl_1_bss_30][i].type;
        if (type != 2) {
            stage->obj[i] = fn_1_C058(type);
            fn_1_C288(stage->obj[i], objPosTbl[lbl_1_bss_30][i].pos);
            fn_1_C328(stage->obj[i], objPosTbl[lbl_1_bss_30][i].rot);
            fn_1_C3C8(stage->obj[i]);
        }
        else {
            stage->obj[i] = NULL;
        }
    }
    return stage;
}

void fn_1_2654(void)
{
    M519_STAGE *stage = HuPrcCurrentGet()->property;

    while (TRUE) {
        lbl_1_data_1C[stage->mode](stage);
    }
}

void fn_1_2694(M519_STAGE *stage)
{
    stage->mode = 0;
}

void fn_1_26A0(M519_STAGE *stage)
{
    while (TRUE) {
        M519_SLEEP_MODECHG(M519_STAGE, 1);
    }
}

void fn_1_2730(M519_STAGE *stage)
{
    Hu3DModelAttrSet(stage->modelId[2], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(stage->modelId[3], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(stage->modelId[2], HU3D_MOTATTR_PAUSE);
    Hu3DModelAttrSet(stage->modelId[3], HU3D_MOTATTR_PAUSE);
}

void fn_1_2790(M519_STAGE *stage)
{
    int i;

    for (i = 0; i < 12; i++) {
        if (stage->obj[i] != NULL) {
            fn_1_C544(stage->obj[i]);
        }
    }
}

void fn_1_27FC(M519_STAGE *stage)
{
    Hu3DModelAttrReset(stage->modelId[2], HU3D_MOTATTR_PAUSE);
    Hu3DModelAttrReset(stage->modelId[2], HU3D_ATTR_DISPOFF);
    Hu3DMotionStartEndSet(stage->modelId[2], 0.0f, Hu3DMotionMaxTimeGet(stage->modelId[2]));
    Hu3DMotionTimeSet(stage->modelId[2], Hu3DMotionMaxTimeGet(stage->modelId[2]));
    Hu3DModelAttrSet(stage->modelId[2], HU3D_MOTATTR_REV);
    Hu3DModelAttrReset(stage->modelId[2], HU3D_MOTATTR_LOOP);
}

void fn_1_2890(M519_STAGE *stage)
{
    int i;

    for (i = 0; i < 12; i++) {
        if (stage->obj[i] != NULL) {
            fn_1_C788(stage->obj[i], FALSE);
        }
    }
}

void fn_1_2900(M519_STAGE *stage)
{
    int i;

    for (i = 0; i < 12; i++) {
        if (stage->obj[i] != NULL) {
            fn_1_C3C8(stage->obj[i]);
            fn_1_CBEC(stage->obj[i]);
        }
    }
}

M519_MECH *fn_1_297C(s16 type, s16 no)
{
    static const int modelTbl[2][9] = {
        { M519_DATANUM(33), M519_DATANUM(52), M519_DATANUM(55), M519_DATANUM(50), M519_DATANUM(51), M519_DATANUM(63), M519_DATANUM(34), M519_DATANUM(35),
            M519_DATANUM(64) },
        { M519_DATANUM(43), M519_DATANUM(58), M519_DATANUM(65), M519_DATANUM(49), M519_DATANUM(66), M519_DATANUM(67), M519_DATANUM(68), -1, -1 },
    };
    static const int motTbl[2][7] = {
        { M519_DATANUM(36), M519_DATANUM(37), M519_DATANUM(38), M519_DATANUM(40), M519_DATANUM(39), M519_DATANUM(41), M519_DATANUM(42) },
        { M519_DATANUM(44), M519_DATANUM(45), M519_DATANUM(46), M519_DATANUM(45), M519_DATANUM(46), M519_DATANUM(47), M519_DATANUM(47) },
    };
    static const M519_MECHPARAM paramTbl[2] = {
        { 0.17857143f, 0.029761905f, 140.0f, 80 },
        { 0.5952381f, 0.05952381f, 80.0f, 40 },
    };
    M519_MECH *mech;
    int i;

    mech = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M519_MECH), HU_MEMNUM_OVL);
    mech->type = type;
    mech->hitId = -1;
    mech->pilotNo = 0;
    mech->no = no;
    mech->shotWait = 0;
    mech->damageNo = 0;
    mech->speed = 0.0f;
    mech->rotSpeed = 0.0f;
    mech->speedTarget = 0.0f;
    mech->rotSpeedTarget = 0.0f;
    mech->accel = paramTbl[type].accel;
    mech->rotAccel = paramTbl[type].rotAccel;
    mech->motNo = 1;
    mech->motNoPrev = 0;
    mech->pos.x = 0.0f;
    mech->pos.y = 0.0f;
    mech->pos.z = 0.0f;
    mech->rot.x = 0.0f;
    mech->rot.y = 0.0f;
    mech->rot.z = 0.0f;
    mech->chargeA = 0;
    mech->chargeB = 0;
    mech->inputF = FALSE;
    mech->boostF = FALSE;
    mech->cameraF = TRUE;
    mech->turretRot = 0.0f;
    mech->radius = paramTbl[type].radius;
    mech->hpMax = paramTbl[type].hp;
    mech->hp = mech->hpMax;
    mech->hpDisp = mech->hpMax;
    mech->effect[0] = NULL;
    mech->effect[1] = NULL;
    mech->knockback = 0.0f;
    mech->knockbackAngle = 0.0f;
    mech->comTargetF = FALSE;
    mech->comMode = 0;
    mech->comAction = 0;
    mech->comShotWait = 0;
    mech->comChargeF = FALSE;
    mech->seNo = -1;
    mech->collideF = FALSE;
    mech->mode = 0;
    mech->prevMode = 0;
    for (i = 0; i < 9; i++) {
        if (modelTbl[type][i] != -1) {
            mech->modelId[i] = Hu3DModelCreateData(modelTbl[type][i]);
            Hu3DModelAttrSet(mech->modelId[i], HU3D_ATTR_NOCULL);
        }
        else {
            mech->modelId[i] = -1;
        }
    }
    switch (type) {
        case 0:
            Hu3DModelAttrSet(mech->modelId[6], HU3D_ATTR_ZWRITE_OFF);
            Hu3DModelAttrSet(mech->modelId[6], HU3D_MOTATTR_PAUSE);
            Hu3DModelAttrSet(mech->modelId[7], HU3D_MOTATTR_PAUSE);
            Hu3DModelAttrSet(mech->modelId[8], HU3D_MOTATTR_PAUSE);
            Hu3DModelAttrSet(mech->modelId[0], HU3D_ATTR_ZWRITE_OFF);
            Hu3DModelAttrSet(mech->modelId[1], HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(mech->modelId[2], HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(mech->modelId[3], HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(mech->modelId[4], HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(mech->modelId[7], HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(mech->modelId[8], HU3D_ATTR_DISPOFF);
            Hu3DModelLayerSet(mech->modelId[0], 2);
            Hu3DModelLayerSet(mech->modelId[1], 2);
            Hu3DModelLayerSet(mech->modelId[2], 2);
            Hu3DModelLayerSet(mech->modelId[3], 2);
            Hu3DModelLayerSet(mech->modelId[4], 2);
            Hu3DModelLayerSet(mech->modelId[5], 2);
            Hu3DModelLayerSet(mech->modelId[6], 2);
            Hu3DModelLayerSet(mech->modelId[7], 2);
            Hu3DModelLayerSet(mech->modelId[8], 2);
            break;
        case 1:
            Hu3DModelAttrSet(mech->modelId[2], HU3D_MOTATTR_LOOP);
            Hu3DModelAttrSet(mech->modelId[0], HU3D_ATTR_ZWRITE_OFF);
            Hu3DModelAttrSet(mech->modelId[3], HU3D_ATTR_ZCMP_OFF);
            Hu3DModelAttrSet(mech->modelId[1], HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(mech->modelId[4], HU3D_MOTATTR_PAUSE);
            Hu3DModelAttrSet(mech->modelId[4], HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(mech->modelId[5], HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(mech->modelId[6], HU3D_ATTR_DISPOFF);
            Hu3DModelLayerSet(mech->modelId[0], 2);
            Hu3DModelLayerSet(mech->modelId[1], 2);
            Hu3DModelLayerSet(mech->modelId[2], 2);
            Hu3DModelLayerSet(mech->modelId[4], 2);
            Hu3DModelLayerSet(mech->modelId[5], 2);
            Hu3DModelLayerSet(mech->modelId[6], 2);
            Hu3DModelLayerSet(mech->modelId[3], 7);
            Hu3DModelCameraSet(mech->modelId[3], lbl_1_rodata_10[no]);
            break;
    }
    for (i = 0; i < 7; i++) {
        mech->motId[i] = Hu3DJointMotionData(mech->modelId[0], motTbl[type][i]);
    }
    fn_1_2FA0(mech, 0, 1.0f);
    switch (type) {
        case 0:
            break;
        case 1:
            mech->effect[0] = fn_1_CC44();
            fn_1_CDF4(mech->effect[0]);
            mech->effect[1] = fn_1_CC44();
            fn_1_CDF4(mech->effect[1]);
            break;
    }
    mech->process = HuPrcChildCreate(fn_1_2F54, 2048, 10240, 0, HuPrcCurrentGet());
    mech->process->property = mech;
    return mech;
}

void fn_1_2F54(void)
{
    M519_MECH *mech = HuPrcCurrentGet()->property;

    while (TRUE) {
        lbl_1_data_20[mech->type][mech->mode](mech);
    }
}

const M519_MOTPARAM lbl_1_rodata_9F8[2][7] = {
    {
        { 0.0f, 16.0f, HU3D_MOTATTR_LOOP },
        { 0.0f, 16.0f, HU3D_MOTATTR_LOOP },
        { 0.0f, 16.0f, HU3D_MOTATTR_LOOP },
        { 0.0f, 16.0f, HU3D_MOTATTR_LOOP },
        { 0.0f, 16.0f, HU3D_MOTATTR_LOOP },
        { 0.0f, 16.0f, HU3D_MOTATTR_NONE },
        { 0.0f, 0.0f, HU3D_MOTATTR_NONE },
    },
    {
        { 0.0f, 16.0f, HU3D_MOTATTR_LOOP },
        { 0.0f, 16.0f, HU3D_MOTATTR_NONE },
        { 0.0f, 16.0f, HU3D_MOTATTR_NONE },
        { 0.0f, 16.0f, HU3D_MOTATTR_NONE },
        { 0.0f, 16.0f, HU3D_MOTATTR_NONE },
        { 0.0f, 16.0f, HU3D_MOTATTR_NONE },
        { 0.0f, 16.0f, HU3D_MOTATTR_NONE },
    },
};

void fn_1_2FA0(M519_MECH *mech, s16 motNo, float speed)
{
    if (mech->motNoPrev != mech->motNo) {
        Hu3DMotionShiftSet(mech->modelId[0], mech->motId[motNo], lbl_1_rodata_9F8[mech->type][motNo].start, lbl_1_rodata_9F8[mech->type][motNo].end,
            lbl_1_rodata_9F8[mech->type][motNo].attr);
        mech->motNoPrev = mech->motNo;
        mech->stepTime = 0.0f;
    }
    mech->motNo = motNo;
    Hu3DMotionSpeedSet(mech->modelId[0], speed);
}

void fn_1_3098(M519_MECH *mech)
{
    int i;

    for (i = 0; i < 4; i++) {
        if (mech->modelId[i] != -1) {
            Hu3DModelTPLvlSet(mech->modelId[i], 0.0f);
            Hu3DModelAttrSet(mech->modelId[i], HU3D_MOTATTR_PAUSE);
            Hu3DModelAttrSet(mech->modelId[i], HU3D_ATTR_DISPOFF);
        }
    }
}

void fn_1_3138(M519_MECH *mech, HuVecF pos)
{
    int i;

    mech->prevPos = mech->pos;
    mech->pos = pos;
    for (i = 0; i < 9; i++) {
        if (mech->modelId[i] != -1) {
            Hu3DModelPosSet(mech->modelId[i], pos.x, pos.y, pos.z);
        }
    }
    switch (mech->type) {
        case 0:
            Hu3DModelPosSet(mech->modelId[5], pos.x, 8.0f + pos.y, pos.z);
            break;
        case 1:
            Hu3DModelPosSet(mech->modelId[2], pos.x, 8.0f + pos.y, pos.z);
            Hu3DModelPosSet(mech->modelId[3], pos.x, 180.0f + pos.y, pos.z);
            break;
    }
}

HuVecF fn_1_3274(M519_MECH *mech)
{
    return mech->pos;
}

void fn_1_3290(M519_MECH *mech, HuVecF rot)
{
    int i;
    MtxPtr mtx;
    float diff;

    mech->prevRot = mech->rot;
    mech->rot = rot;
    for (i = 0; i < 9; i++) {
        if ((mech->type != 1 || i != 2) && (mech->type != 0 || i != 5) && mech->modelId[i] != -1) {
            mtx = Hu3DData[mech->modelId[i]].mtx;
            PSMTXIdentity(mtx);
            PSMTXRotRad(mtx, 'z', 0.017453292f * rot.z);
            Hu3DModelRotSet(mech->modelId[i], rot.x, rot.y, 0.0f);
        }
    }
    switch (mech->type) {
        case 0:
            break;
        case 1:
            mtx = Hu3DData[mech->modelId[3]].mtx;
            PSMTXIdentity(mtx);
            diff = HuAbs(fmod(mech->rot.y - mech->turretRot, 360.0));
            if (diff > 180.0f) {
                diff = 180.0f - (diff - 180.0f);
            }
            Hu3DModelRotSet(mech->modelId[3], -45.0f + (0.5f * diff), mech->turretRot, 0.0f);
            break;
    }
}

HuVecF fn_1_34FC(M519_MECH *mech)
{
    return mech->rot;
}

void fn_1_3518(M519_MECH *mech, M519_PILOT *pilot)
{
    Hu3DModelHookSet(mech->modelId[0], lbl_1_data_14[mech->type], pilot->modelId[1]);
    mech->pilotNo = pilot->playerNo;
    pilot->mechNo = mech->no;
}

void fn_1_3580(M519_MECH *mech)
{
    Hu3DModelHookReset(mech->modelId[0]);
}

void fn_1_35B0(M519_MECH *mech)
{
    mech->mode = 0;
}

void fn_1_35BC(M519_MECH *mech)
{
    while (TRUE) {
        fn_1_95B0(mech);
        fn_1_9700(mech);
        fn_1_8450(mech);
        M519_SLEEP_MODECHG(M519_MECH, 1);
    }
}

void fn_1_3658(M519_MECH *mech)
{
    mech->mode = 1;
}

void fn_1_3664(M519_MECH *mech)
{
    BOOL deadF = FALSE;
    float rotStep = 0.2f;
    float speedStep = 0.2f;
    float stkX;
    float stkY;
    float speed;
    float rotSpeed;
    float dx;
    float dz;
    float angle;

    while (TRUE) {
        mech->stkX = 0;
        mech->stkY = 0;
        mech->trigL = 0;
        mech->trigR = 0;
        mech->btn = 0;
        mech->btnDown = 0;
        if (mech->hp > 0) {
            if (mech->inputF) {
                if (lbl_1_bss_E8[mech->pilotNo]->comF) {
                    fn_1_4E18(mech);
                }
                else {
                    fn_1_4D40(mech);
                }
            }
        }
        else if (!deadF) {
            HuAudFXPlayPan(lbl_1_rodata_164[lbl_1_bss_E8[mech->pilotNo]->charNo], lbl_1_rodata_1C[mech->type]);
            HuAudFXPlay(1822);
            Hu3DModelAttrReset(mech->modelId[8], HU3D_ATTR_DISPOFF);
            Hu3DModelAttrReset(mech->modelId[8], HU3D_MOTATTR_PAUSE);
            Hu3DModelAttrSet(mech->modelId[8], HU3D_MOTATTR_LOOP);
            Hu3DMotionTimeSet(mech->modelId[8], 0.0f);
            fn_1_FD7C(mech->pilotNo, 2, 60);
            deadF = TRUE;
        }
        else if (mech->speed <= 0.0f) {
            fn_1_817C(mech);
            return;
        }
        if (mech->trigR >= 150 || mech->trigL >= 150) {
            if (!mech->boostF) {
                mech->boostF = TRUE;
                mech->speed = 0.0f;
                mech->rotSpeed = 0.0f;
            }
            speed = mech->speed;
            stkX = mech->stkX * -mech->accel;
            stkY = mech->stkY * mech->accel;
            angle = HuAtan(stkX, stkY);
            speed = HuMagPoint2D(stkX, stkY);
            dx = speed * HuSin(angle + mech->rot.y);
            dz = speed * HuCos(angle + mech->rot.y);
            mech->speed = speed;
            rotSpeed = 0.0f;
            if (stkY > 0.0f && HuAbs(stkY) > HuAbs(stkX)) {
                fn_1_2FA0(mech, 1, 1.0f + HuAbs(0.5f * stkY));
            }
            else if (stkY < 0.0f && HuAbs(stkY) > HuAbs(stkX)) {
                fn_1_2FA0(mech, 2, 1.0f + HuAbs(0.5f * stkY));
            }
            else if (stkX < 0.0f) {
                fn_1_2FA0(mech, 3, 1.0f + HuAbs(0.5f * stkX));
            }
            else if (stkX > 0.0f) {
                fn_1_2FA0(mech, 4, 1.0f + HuAbs(0.5f * stkX));
            }
            else {
                fn_1_2FA0(mech, 0, 1.0f);
            }
            if (stkY != 0.0f && HuAbs(stkY) > HuAbs(stkX)) {
                if (mech->stepTime >= 0.0f && mech->stepTime < HuAbs(0.5f * stkY)) {
                    HuAudFXPlayPan(1803, lbl_1_rodata_18[mech->type]);
                }
                mech->stepTime += 1.0f + HuAbs(0.5f * stkY);
            }
            else if (stkX != 0.0f) {
                if (mech->stepTime >= 0.0f && mech->stepTime < HuAbs(0.5f * stkX)) {
                    HuAudFXPlayPan(1803, lbl_1_rodata_18[mech->type]);
                }
                mech->stepTime += 1.0f + HuAbs(0.5f * stkX);
            }
            if (mech->stepTime >= 80.0f) {
                mech->stepTime = 0.0f;
            }
        }
        else {
            if (mech->boostF) {
                mech->boostF = FALSE;
                mech->speed = 0.0f;
            }
            rotSpeed = mech->rotSpeed;
            speed = mech->speed;
            mech->rotSpeedTarget = mech->stkX * -mech->rotAccel;
            mech->speedTarget = mech->stkY * mech->accel;
            if (rotSpeed < mech->rotSpeedTarget) {
                rotSpeed += rotStep;
                if (rotSpeed > mech->rotSpeedTarget) {
                    rotSpeed = mech->rotSpeedTarget;
                }
            }
            if (rotSpeed > mech->rotSpeedTarget) {
                rotSpeed -= rotStep;
                if (rotSpeed < mech->rotSpeedTarget) {
                    rotSpeed = mech->rotSpeedTarget;
                }
            }
            if (speed < mech->speedTarget) {
                speed += speedStep;
                if (speed > mech->speedTarget) {
                    speed = mech->speedTarget;
                }
            }
            if (speed > mech->speedTarget) {
                speed -= speedStep;
                if (speed < mech->speedTarget) {
                    speed = mech->speedTarget;
                }
            }
            dx = speed * HuSin(mech->rot.y);
            dz = speed * HuCos(mech->rot.y);
            mech->rotSpeed = rotSpeed;
            mech->speed = speed;
            if (speed > 0.0f) {
                fn_1_2FA0(mech, 1, 1.0f + HuAbs(0.5f * speed));
            }
            else if (speed < 0.0f) {
                fn_1_2FA0(mech, 2, 1.0f + HuAbs(0.5f * speed));
            }
            else if (rotSpeed != 0.0f) {
                fn_1_2FA0(mech, 1, 1.0f + HuAbs(0.5f * rotSpeed));
            }
            else {
                fn_1_2FA0(mech, 0, 1.0f);
            }
            if (speed != 0.0f) {
                if ((mech->stepTime >= 0.0f && mech->stepTime < HuAbs(0.5f * speed))
                    || (mech->stepTime >= 39.0f && mech->stepTime < 39.0f + HuAbs(0.5f * speed))) {
                    HuAudFXPlayPan(1802, lbl_1_rodata_18[mech->type]);
                }
                mech->stepTime += 1.0f + HuAbs(0.5f * speed);
            }
            if (rotSpeed != 0.0f) {
                if ((mech->stepTime >= 0.0f && mech->stepTime < HuAbs(0.5f * rotSpeed))
                    || (mech->stepTime >= 39.0f && mech->stepTime < 39.0f + HuAbs(0.5f * rotSpeed))) {
                    HuAudFXPlayPan(1802, lbl_1_rodata_18[mech->type]);
                }
                mech->stepTime += 1.0f + HuAbs(0.5f * rotSpeed);
            }
            if (mech->stepTime >= 80.0f) {
                mech->stepTime = 0.0f;
            }
        }
        mech->pos.x += dx;
        mech->pos.z += dz;
        mech->rot.y += rotSpeed;
        fn_1_82E4(mech);
        fn_1_95B0(mech);
        fn_1_9700(mech);
        fn_1_8450(mech);
        fn_1_8E40(mech);
        M519_SLEEP_MODECHG(M519_MECH, 1);
    }
}

void fn_1_4D40(M519_MECH *mech)
{
    s16 padNo = lbl_1_bss_E8[mech->pilotNo]->padNo;

    mech->stkX = HuPadStkX[padNo];
    mech->stkY = HuPadStkY[padNo];
    mech->trigL = HuPadTrigL[padNo];
    mech->trigR = HuPadTrigR[padNo];
    mech->btn = HuPadBtn[padNo];
    mech->btnDown = HuPadBtnDown[padNo];
}

void fn_1_4E18(M519_MECH *mech)
{
    static const int timeTbl[4] = { 180, 170, 150, 160 };
    static const float aimTbl[4] = { 15.0f, 16.0f, 17.0f, 18.0f };
    static const s16 targetTimeTbl[4] = { 30, 25, 20, 20 };
    static const int nearTbl[4] = { 640, 660, 680, 700 };
    static const int farTbl[4] = { 1300, 1200, 1100, 1000 };
    static const s16 waitTbl[4] = { 35, 25, 20, 15 };
    static const int escapeTbl[4] = { 600, 610, 620, 630 };
    static const s16 shotWaitTbl[4] = { 6, 4, 2, 0 };
    static const float turnTbl[4] = { 35.0f, 34.0f, 33.0f, 32.0f };
    M519_MECH *target;
    s16 dif;
    s16 targetNo;
    BOOL aimF;
    s16 count;
    int i;
    float dx;
    float dz;
    float dist;
    float angle;
    float minDist;
    float minAngle;
    float distSum;
    float diff;
    float tC;
    HuVecF circle;

    dif = lbl_1_bss_E8[mech->pilotNo]->dif;
    circle.x = mech->pos.x;
    circle.y = mech->pos.z;
    circle.z = mech->radius;
    target = lbl_1_bss_F8[mech->comTargetNo];
    switch (mech->comMode) {
        case 0:
            if (!mech->comTargetF) {
                dist = M519_FRANDMOD_F(2390.0f - mech->radius);
                angle = M519_FRAND() % 360;
                mech->comTarget.x = dist * HuSin(angle);
                mech->comTarget.z = dist * HuCos(angle);
                mech->comTarget.y = 0.0f;
                mech->comTimer = 0;
                mech->comSpeed = (M519_FRAND() % 20) + 50;
                mech->comTargetF = TRUE;
            }
            dx = mech->comTarget.x - mech->pos.x;
            dz = mech->comTarget.z - mech->pos.z;
            tC = HuMagPoint2D(dx, dz);
            dist = tC;
            if (dist < 300.0f || mech->comTimer++ > timeTbl[dif]) {
                mech->comSpeed = 0;
                mech->comMode = 1;
                mech->comTimer = 0;
                mech->comTargetF = FALSE;
                mech->comTargetNo = M519_FRANDMOD(3) + 1;
            }
            else {
                mech->btnDown |= PAD_BUTTON_A;
            }
            break;
        case 1:
            targetNo = 0;
            minDist = 4780.0f;
            minAngle = 360.0f;
            aimF = FALSE;
            count = 0;
            distSum = 0.0f;
            if (mech->comTimer > 0) {
                mech->comTimer--;
            }
            for (i = 0; i < 3; i++) {
                target = lbl_1_bss_F8[i + 1];
                if (!fn_1_A1D4(target)) {
                    count++;
                    dx = target->pos.x - mech->pos.x;
                    dz = target->pos.z - mech->pos.z;
                    tC = HuMagPoint2D(dx, dz);
                    distSum += tC;
                    angle = (180.0 + HuAtan(mech->pos.x - target->pos.x, mech->pos.z - target->pos.z)) - mech->rot.y;
                    if (angle < 0.0f) {
                        angle = 360.0f + angle;
                    }
                    if (angle >= 360.0f) {
                        angle -= 360.0f;
                    }
                    if (angle >= 180.0f) {
                        angle -= 360.0f;
                    }
                    angle = -angle;
                    if (minAngle > HuAbs(angle)) {
                        minAngle = HuAbs(angle);
                        if (dist < 2000.0f) {
                            targetNo = i + 1;
                        }
                    }
                    if (minDist > dist) {
                        minDist = dist;
                    }
                    if (HuAbs(angle) < aimTbl[dif]) {
                        aimF = TRUE;
                    }
                }
            }
            if (count != 0) {
                if (mech->comTimer == 0) {
                    if (targetNo > 0) {
                        mech->comTargetNo = targetNo;
                    }
                    if (!fn_1_A1D4(lbl_1_bss_F8[mech->comTargetNo])) {
                        mech->comTimer = targetTimeTbl[dif];
                    }
                    else {
                        mech->comTargetNo = M519_FRANDMOD(3) + 1;
                    }
                }
                target = lbl_1_bss_F8[mech->comTargetNo];
                mech->comTarget = target->pos;
                dx = mech->comTarget.x - mech->pos.x;
                dz = mech->comTarget.z - mech->pos.z;
                dist = HuMagPoint2D(dx, dz);
                if (dist > 500.0f) {
                    mech->comSpeed = -(s16)(0.015 * dist);
                }
                if (dist < 2000.0f) {
                    mech->comSpeed = 0;
                }
                else {
                    mech->comSpeed = 0.015 * dist;
                }
                switch (mech->comAction) {
                    case 0:
                        if (aimF) {
                            mech->btnDown |= PAD_BUTTON_A;
                            mech->comTimer2 = 0;
                            if (minDist < nearTbl[dif]) {
                                mech->comAction = 2;
                            }
                        }
                        else if (mech->comTimer2++ > waitTbl[dif]) {
                            if (distSum / count > farTbl[dif]) {
                                mech->comAction = 1;
                            }
                            else {
                                mech->comAction = 3;
                            }
                        }
                        break;
                    case 1:
                        if (aimF && mech->chargeA > 90) {
                            mech->comAction = 0;
                        }
                        else {
                            mech->btn |= PAD_BUTTON_A;
                        }
                        break;
                    case 2:
                        mech->btnDown |= PAD_BUTTON_B;
                        if (minDist > escapeTbl[dif]) {
                            mech->comAction = 0;
                        }
                        break;
                    case 3:
                        if (mech->chargeB > 90) {
                            mech->comAction = 0;
                        }
                        else {
                            mech->btn |= PAD_BUTTON_B;
                        }
                        break;
                }
            }
            break;
        case 2:
            break;
    }
    if (mech->comShotWait > 0) {
        mech->comShotWait--;
        mech->btnDown &= ~PAD_BUTTON_A;
    }
    else if (mech->btnDown & PAD_BUTTON_A) {
        mech->comShotWait = shotWaitTbl[dif] + 8;
    }
    diff = 0.0f;
    if (mech->comTarget.x != mech->pos.x || mech->comTarget.z != mech->pos.z) {
        diff = (180.0 + HuAtan(mech->pos.x - mech->comTarget.x, mech->pos.z - mech->comTarget.z)) - mech->rot.y;
        if (diff < 0.0f) {
            diff = 360.0f + diff;
        }
        if (diff >= 360.0f) {
            diff -= 360.0f;
        }
        if (diff >= 180.0f) {
            diff -= 360.0f;
        }
        diff = -diff;
    }
    angle = diff;
    angle = (angle > 60.0f) ? 60.0f : angle;
    angle = (angle < -60.0f) ? -60.0f : angle;
    dx = angle * (s16)(68.0f / turnTbl[dif]);
    dz = mech->comSpeed;
    dist = sqrt(dx * dx + dz * dz);
    angle = HuAtan(dx, dz);
    dx = dist * HuSin(angle);
    dz = dist * HuCos(angle);
    mech->stkX = dx;
    mech->stkY = dz;
}

void fn_1_5EDC(M519_MECH *mech)
{
    BOOL deadF = FALSE;
    float rotStep = 0.18f;
    float speedStep = 0.36f;
    s16 padNo = lbl_1_bss_E8[mech->pilotNo]->padNo;
    s16 playerNo = lbl_1_bss_E8[mech->pilotNo]->playerNo;
    float rotSpeed;
    float speed;
    float dx;
    float dz;

    while (TRUE) {
        rotSpeed = mech->rotSpeed;
        speed = mech->speed;
        mech->stkX = 0;
        mech->stkY = 0;
        mech->trigL = 0;
        mech->trigR = 0;
        mech->btn = 0;
        mech->btnDown = 0;
        if (mech->hp > 0) {
            if (mech->inputF) {
                if (lbl_1_bss_E8[mech->pilotNo]->comF) {
                    fn_1_69BC(mech);
                }
                else {
                    fn_1_68E4(mech);
                }
            }
        }
        else if (!deadF) {
            HuAudFXPlay(lbl_1_rodata_164[lbl_1_bss_E8[mech->pilotNo]->charNo]);
            HuAudFXPlay(1823);
            Hu3DModelAttrReset(mech->modelId[4], HU3D_ATTR_DISPOFF);
            Hu3DModelAttrReset(mech->modelId[4], HU3D_MOTATTR_PAUSE);
            Hu3DModelAttrSet(mech->modelId[4], HU3D_MOTATTR_LOOP);
            Hu3DMotionTimeSet(mech->modelId[4], 0.0f);
            fn_1_FD7C(mech->pilotNo, 2, 60);
            deadF = TRUE;
        }
        else if (mech->speed <= 0.0f) {
            fn_1_817C(mech);
            return;
        }
        mech->rotSpeedTarget = mech->stkX * -mech->rotAccel;
        mech->speedTarget = mech->stkY * mech->accel;
        if (mech->btn & PAD_BUTTON_B) {
            if (mech->chargeB++ == 0) {
                if (mech->seNo != -1) {
                    HuAudFXStop(mech->seNo);
                }
                mech->seNo = HuAudFXPlay(1825);
                Hu3DModelAttrReset(mech->modelId[5], HU3D_ATTR_DISPOFF);
                Hu3DMotionStartEndSet(mech->modelId[5], 0.0f, 60.0f);
                Hu3DMotionTimeSet(mech->modelId[5], 0.0f);
                Hu3DModelAttrSet(mech->modelId[5], HU3D_MOTATTR_LOOP);
            }
            else if (mech->chargeB == 40) {
                Hu3DMotionStartEndSet(mech->modelId[5], 40.0f, 60.0f);
                Hu3DMotionTimeSet(mech->modelId[5], 0.0f);
                Hu3DModelAttrSet(mech->modelId[5], HU3D_MOTATTR_LOOP);
            }
            if (mech->chargeB >= 40 && mech->chargeB % 5 == 0) {
                fn_1_FD7C(mech->pilotNo, 1, 3);
            }
        }
        else if (mech->chargeB > 0) {
            Hu3DModelAttrSet(mech->modelId[5], HU3D_ATTR_DISPOFF);
            if (mech->seNo != -1) {
                HuAudFXStop(mech->seNo);
                mech->seNo = -1;
            }
            if (mech->chargeB >= 90) {
                HuAudFXPlayPan(1821, lbl_1_rodata_18[mech->type]);
                Hu3DMotionStartEndSet(mech->modelId[6], 0.0f, 60.0f);
                Hu3DMotionTimeSet(mech->modelId[6], 0.0f);
                Hu3DModelAttrReset(mech->modelId[6], HU3D_ATTR_DISPOFF);
                mech->speedTarget = 100.0f * mech->accel;
                speed = 100.0f * mech->accel;
            }
            mech->chargeB = 0;
        }
        if (rotSpeed < mech->rotSpeedTarget) {
            rotSpeed += rotStep;
            if (rotSpeed > mech->rotSpeedTarget) {
                rotSpeed = mech->rotSpeedTarget;
            }
        }
        if (rotSpeed > mech->rotSpeedTarget) {
            rotSpeed -= rotStep;
            if (rotSpeed < mech->rotSpeedTarget) {
                rotSpeed = mech->rotSpeedTarget;
            }
        }
        if (speed < mech->speedTarget) {
            speed += speedStep;
            if (speed > mech->speedTarget) {
                speed = mech->speedTarget;
            }
        }
        if (speed > mech->speedTarget) {
            speed -= speedStep;
            if (speed < mech->speedTarget) {
                speed = mech->speedTarget;
            }
        }
        mech->rot.y += rotSpeed;
        mech->rot.z = rotSpeed * -HuAbs(0.2f * speed);
        dx = speed * HuSin(mech->rot.y);
        dz = speed * HuCos(mech->rot.y);
        mech->pos.x += dx;
        mech->pos.z += dz;
        mech->rotSpeed = rotSpeed;
        mech->speed = speed;
        if (speed > 0.0f) {
            fn_1_2FA0(mech, 1, 1.0f);
        }
        else if (speed < 0.0f) {
            fn_1_2FA0(mech, 2, 1.0f);
        }
        else if (rotSpeed != 0.0f) {
            fn_1_2FA0(mech, 1, 1.0f);
        }
        else if (Hu3DMotionEndCheck(mech->modelId[0])) {
            fn_1_2FA0(mech, 0, 1.0f);
        }
        if (HuAbs(speed) > 16.0f) {
            fn_1_7998(mech);
        }
        fn_1_82E4(mech);
        fn_1_95B0(mech);
        fn_1_9700(mech);
        fn_1_8450(mech);
        fn_1_8D40(mech);
        if (HuAbs(speed) < 72.0f) {
            fn_1_8E40(mech);
        }
        M519_SLEEP_MODECHG(M519_MECH, 1);
    }
}

void fn_1_68E4(M519_MECH *mech)
{
    s16 padNo = lbl_1_bss_E8[mech->pilotNo]->padNo;

    mech->stkX = HuPadStkX[padNo];
    mech->stkY = HuPadStkY[padNo];
    mech->trigL = HuPadTrigL[padNo];
    mech->trigR = HuPadTrigR[padNo];
    mech->btn = HuPadBtn[padNo];
    mech->btnDown = HuPadBtnDown[padNo];
}

void fn_1_69BC(M519_MECH *mech)
{
    static const int farTbl[4] = { 2000, 2100, 2200, 2300 };
    static const int chargeTbl[4] = { 40, 30, 20, 10 };
    static const int timeTbl[4] = { 180, 170, 150, 160 };
    static const int nearTbl[4] = { 550, 540, 530, 520 };
    static const int aimTbl[4] = { 10, 15, 20, 25 };
    static const s16 shotWaitTbl[4] = { 4, 3, 2, 0 };
    M519_MECH *target;
    s16 dif;
    float dx;
    float dz;
    float dist;
    float angle;
    float step;
    float diff;
    float tA;
    float tB;
    HuVecF circle;
    HuVecF goal;
    HuVecF area;

    dif = lbl_1_bss_E8[mech->pilotNo]->dif;
    target = lbl_1_bss_F8[0];
    circle.x = mech->pos.x;
    circle.y = mech->pos.z;
    circle.z = mech->radius;
    switch (mech->comMode) {
        case 0:
            if (!mech->comTargetF) {
                dist = M519_FRAND() % (int)(2390.0f - 2.0f * mech->radius);
                angle = M519_FRAND() % 360;
                mech->comTarget.x = dist * HuSin(angle);
                mech->comTarget.z = dist * HuCos(angle);
                mech->comTarget.y = 0.0f;
                mech->comTimer = 0;
                mech->comSpeed = (M519_FRAND() % 20) + 50;
                mech->comTargetF = TRUE;
            }
            dx = mech->comTarget.x - mech->pos.x;
            dz = mech->comTarget.z - mech->pos.z;
            dist = HuMagPoint2D(dx, dz);
            if (dist > 300.0f && dist < farTbl[dif]) {
                mech->comMode = 1;
                mech->comTimer = 0;
                if (!mech->comChargeF && (M519_FRAND() % chargeTbl[dif]) == 0) {
                    mech->comChargeF = TRUE;
                }
            }
            else if (dist < 250.0f || mech->comTimer++ > timeTbl[dif]) {
                mech->comTargetF = FALSE;
                mech->comTimer = 0;
            }
            break;
        case 1:
            mech->comTarget.x = target->pos.x;
            mech->comTarget.z = target->pos.z;
            mech->comTarget.y = 0.0f;
            dx = mech->comTarget.x - mech->pos.x;
            dz = mech->comTarget.z - mech->pos.z;
            dist = HuMagPoint2D(dx, dz);
            if (dist < nearTbl[dif] || mech->knockback > 0.0f) {
                mech->comMode = 2;
                mech->comTargetF = FALSE;
                mech->comTimer = 0;
            }
            else {
                angle = (180.0 + HuAtan(mech->pos.x - mech->comTarget.x, mech->pos.z - mech->comTarget.z)) - mech->rot.y;
                if (angle < 0.0f) {
                    angle = 360.0f + angle;
                }
                if (angle >= 360.0f) {
                    angle -= 360.0f;
                }
                if (angle >= 180.0f) {
                    angle -= 360.0f;
                }
                angle = -angle;
                mech->comSpeed = 20.0f + dist / 120.0f;
                tA = HuAbs(angle);
                if (tA < aimTbl[dif]) {
                    mech->btnDown = PAD_BUTTON_A;
                }
            }
            break;
        case 2:
            if (!mech->comTargetF) {
                dist = 2300.0f + (M519_FRAND() % 1700);
                angle = M519_FRAND() % 360;
                area.x = 0.0f;
                area.y = 0.0f;
                area.z = 2390.0f;
                step = 0.0f;
                while (TRUE) {
                    goal.x = dist * HuSin(angle + step) - mech->comTarget.x;
                    goal.y = dist * HuCos(angle + step) - mech->comTarget.z;
                    goal.z = mech->radius;
                    if (fn_1_FA6C(&area, &goal, FALSE)) {
                        diff = (180.0 + HuAtan(mech->pos.x - goal.x, mech->pos.z - goal.y)) - mech->rot.y;
                        if (diff < 0.0f) {
                            diff = 360.0f + diff;
                        }
                        if (diff >= 360.0f) {
                            diff -= 360.0f;
                        }
                        if (diff >= 180.0f) {
                            diff -= 360.0f;
                        }
                        diff = -diff;
                        tB = HuAbs(diff);
                        if (tB > 30.0f) {
                            break;
                        }
                    }
                    step += 30.0f;
                    if (step >= 360.0f) {
                        step = 0.0f;
                        dist -= 250.0f;
                    }
                }
                mech->comTarget.x = goal.x;
                mech->comTarget.z = goal.y;
                mech->comTarget.y = 0.0f;
                mech->comTargetF = TRUE;
                mech->comTimer = 0;
                mech->comSpeed = (M519_FRAND() % 10) + 60;
            }
            if (mech->chargeB >= 90) {
                mech->comChargeF = FALSE;
            }
            goal.x = mech->comTarget.x;
            goal.y = mech->comTarget.z;
            goal.z = 300.0f;
            if (fn_1_F860(&circle, &goal, FALSE) || mech->comTimer++ > 120) {
                mech->comMode = 0;
                mech->comTargetF = FALSE;
            }
            break;
    }
    if (mech->comChargeF) {
        mech->btn |= PAD_BUTTON_B;
    }
    if (mech->comShotWait > 0) {
        mech->comShotWait--;
        if (mech->btnDown & PAD_BUTTON_A) {
            mech->btnDown &= ~PAD_BUTTON_A;
        }
    }
    else if (mech->btnDown & PAD_BUTTON_A) {
        mech->comShotWait = shotWaitTbl[dif] + 8;
    }
    if (mech->collideF) {
        mech->comTarget.x = mech->pos.x;
        mech->comTarget.z = mech->pos.z;
    }
    diff = 0.0f;
    if (mech->comTarget.x != mech->pos.x || mech->comTarget.z != mech->pos.z) {
        diff = (180.0 + HuAtan(mech->pos.x - mech->comTarget.x, mech->pos.z - mech->comTarget.z)) - mech->rot.y;
        if (diff < 0.0f) {
            diff = 360.0f + diff;
        }
        if (diff >= 360.0f) {
            diff -= 360.0f;
        }
        if (diff >= 180.0f) {
            diff -= 360.0f;
        }
        diff = -diff;
    }
    angle = diff;
    angle = (angle > 60.0f) ? 60.0f : angle;
    angle = (angle < -60.0f) ? -60.0f : angle;
    dx = angle;
    dz = mech->comSpeed;
    dist = HuMagPoint2D(dx, dz);
    angle = HuAtan(dx, dz);
    dx = dist * HuSin(angle);
    dz = dist * HuCos(angle);
    mech->stkX = dx;
    mech->stkY = dz;
}

void fn_1_7998(M519_MECH *mech)
{
    int i;
    float dist;
    float t;
    HuVecF prev;
    HuVecF cur;
    HuVecF pos;

    dist = 80.0f - (2.0f * mech->rot.z);
    prev.x = mech->prevPos.x + (dist * HuSin(120.0f + mech->prevRot.y));
    prev.z = mech->prevPos.z + (dist * HuCos(120.0f + mech->prevRot.y));
    prev.y = (100.0f + mech->prevPos.y) + mech->prevRot.z;
    cur.x = mech->pos.x + (dist * HuSin(120.0f + mech->rot.y));
    cur.z = mech->pos.z + (dist * HuCos(120.0f + mech->rot.y));
    cur.y = (100.0f + mech->pos.y) + mech->rot.z;
    for (i = 0; i < 3; i++) {
        t = (1.0f / 3.0f) * i;
        pos.x = prev.x + (t * (cur.x - prev.x));
        pos.y = prev.y + (t * (cur.y - prev.y));
        pos.z = prev.z + (t * (cur.z - prev.z));
        fn_1_CD54(mech->effect[0], pos);
        fn_1_CFE8(mech->effect[0]);
    }
    dist = 80.0f + (2.0f * mech->rot.z);
    prev.x = mech->prevPos.x + (dist * HuSin(240.0f + mech->prevRot.y));
    prev.z = mech->prevPos.z + (dist * HuCos(240.0f + mech->prevRot.y));
    prev.y = (100.0f + mech->prevPos.y) - mech->prevRot.z;
    cur.x = mech->pos.x + (dist * HuSin(240.0f + mech->rot.y));
    cur.z = mech->pos.z + (dist * HuCos(240.0f + mech->rot.y));
    cur.y = (100.0f + mech->pos.y) - mech->rot.z;
    for (i = 0; i < 3; i++) {
        t = (1.0f / 3.0f) * i;
        pos.x = prev.x + (t * (cur.x - prev.x));
        pos.y = prev.y + (t * (cur.y - prev.y));
        pos.z = prev.z + (t * (cur.z - prev.z));
        fn_1_CD54(mech->effect[1], pos);
        fn_1_CFE8(mech->effect[1]);
    }
}

float fn_1_7EB8(M519_MECH *mech, HuVecF *pos)
{
    float angle = 0.0f;

    if (pos->x != mech->pos.x || pos->z != mech->pos.z) {
        angle = (180.0 + HuAtan(mech->pos.x - pos->x, mech->pos.z - pos->z)) - mech->rot.y;
        if (angle < 0.0f) {
            angle = 360.0f + angle;
        }
        if (angle >= 360.0f) {
            angle -= 360.0f;
        }
        if (angle >= 180.0f) {
            angle -= 360.0f;
        }
        angle = -angle;
    }
    return angle;
}

float fn_1_7FFC(M519_MECH *mech, HuVecF *pos)
{
    float dx = pos->x - mech->pos.x;
    float dz = pos->z - mech->pos.z;
    float dist = HuMagPoint2D(dx, dz);

    return dist;
}

void fn_1_817C(M519_MECH *mech)
{
    mech->mode = 2;
}

void fn_1_8188(M519_MECH *mech)
{
    fn_1_2FA0(mech, 5, 1.0f);
    M519_SLEEP_MODECHG(M519_MECH, 50);
    fn_1_A828(lbl_1_bss_E8[mech->pilotNo]);
    fn_1_F2DC(lbl_1_bss_20[mech->no]);
    fn_1_98A0(mech);
}

void fn_1_82E4(M519_MECH *mech)
{
    float dx;
    float dz;
    s16 step;

    if (mech->knockback <= 0.0f) {
        return;
    }
    {
        fn_1_FD7C(mech->pilotNo, 2, 2);
        dx = mech->knockback * HuSin(mech->knockbackAngle);
        dz = mech->knockback * HuCos(mech->knockbackAngle);
        mech->pos.x += dx;
        mech->pos.z += dz;
        step = mech->knockback / 4.0f;
        mech->knockback -= step;
        if (step == 0) {
            mech->knockback = 0.0f;
        }
    }
}

void fn_1_8450(M519_MECH *mech)
{
    s16 no;
    OMCAMERAVIEW *view;
    Mtx mtx;
    HuVecF pos;

    no = mech->no;
    if (mech->cameraF) {
        view = &lbl_1_bss_108[no];
        switch (no) {
            case 0:
                view->center.x = mech->pos.x + (740.0 * HuSin(mech->rot.y));
                view->center.z = mech->pos.z + (740.0 * HuCos(mech->rot.y));
                view->center.y = 0.0f;
                view->zoom = 1600.0f;
                view->rot.y = 180.0f + mech->rot.y;
                view->rot.x = -26.0f;
                view->rot.z = 0.0f;
                break;
            case 1:
            case 2:
            case 3:
                view->center.x = mech->pos.x + (500.0 * HuSin(mech->rot.y));
                view->center.z = mech->pos.z + (500.0 * HuCos(mech->rot.y));
                view->center.y = 0.0f;
                view->zoom = 1200.0f;
                view->rot.y = 180.0f + mech->rot.y;
                view->rot.x = -22.5f;
                view->rot.z = 0.0f;
                break;
        }
        omCameraViewSetMulti(lbl_1_rodata_10[no], view);
    }
    fn_1_3290(mech, mech->rot);
    fn_1_3138(mech, mech->pos);
    switch (mech->type) {
        case 0:
            Hu3DModelObjMtxGet(mech->modelId[0], lbl_1_data_14[0], mtx);
            pos.x = mtx[0][3];
            pos.y = mtx[1][3];
            pos.z = mtx[2][3];
            Hu3DModelPosSet(mech->modelId[6], pos.x, pos.y, pos.z);
            Hu3DModelPosSet(mech->modelId[7], pos.x, pos.y, pos.z);
            Hu3DModelPosSet(mech->modelId[8], pos.x, pos.y, pos.z);
            if ((mech->hpMax / 5) * (4 - mech->damageNo) > mech->hp) {
                int seNo = HuAudFXPlayPan(1808, lbl_1_rodata_18[mech->type]);
                Hu3DMotionStartEndSet(mech->modelId[6], 40.0f * mech->damageNo, 40.0f + (40.0f * mech->damageNo));
                Hu3DMotionStartEndSet(mech->modelId[7], 40.0f * mech->damageNo, 40.0f + (40.0f * mech->damageNo));
                Hu3DMotionTimeSet(mech->modelId[6], 40.0f * mech->damageNo);
                Hu3DModelAttrReset(mech->modelId[6], HU3D_MOTATTR_LOOP);
                Hu3DModelAttrReset(mech->modelId[6], HU3D_MOTATTR_PAUSE);
                Hu3DMotionTimeSet(mech->modelId[7], 40.0f * mech->damageNo);
                Hu3DModelAttrReset(mech->modelId[7], HU3D_MOTATTR_LOOP);
                Hu3DModelAttrReset(mech->modelId[7], HU3D_MOTATTR_PAUSE);
                mech->damageNo++;
            }
            break;
        case 1:
            Hu3DModelObjMtxGet(mech->modelId[0], lbl_1_data_14[1], mtx);
            pos.x = mtx[0][3];
            pos.y = mtx[1][3];
            pos.z = mtx[2][3];
            Hu3DModelPosSet(mech->modelId[4], pos.x, pos.y, pos.z);
            if (mech->mode == 1) {
                Hu3DModelAttrReset(mech->modelId[3], HU3D_ATTR_DISPOFF);
            }
            else {
                Hu3DModelAttrSet(mech->modelId[3], HU3D_ATTR_DISPOFF);
            }
            break;
    }
}

void fn_1_8D40(M519_MECH *mech)
{
    M519_MECH *target = lbl_1_bss_F8[0];

    mech->turretRot = 180.0 + HuAtan(mech->pos.x - target->pos.x, mech->pos.z - target->pos.z);
}

s16 fn_1_8DD0(M519_MECH *mech)
{
    if (mech->chargeB > 0) {
        if (mech->chargeB >= 90) {
            return 2;
        }
        if (mech->chargeB > 40) {
            return 1;
        }
    }
    else if (mech->chargeA > 0) {
        if (mech->chargeA >= 90) {
            return 2;
        }
        if (mech->chargeA > 40) {
            return 1;
        }
    }
    return 0;
}

void fn_1_8E40(M519_MECH *mech)
{
    HuVecF pos;
    HuVecF rot;
    s16 charge;

    if (mech->shotWait > 0 && --mech->shotWait > 0) {
        return;
    }
    switch (mech->type) {
        case 0:
            pos = mech->pos;
            rot = mech->rot;
            if ((mech->btnDown & PAD_BUTTON_A) && !(mech->btn & PAD_BUTTON_B)) {
                Hu3DModelAttrReset(mech->modelId[1], HU3D_ATTR_DISPOFF);
                Hu3DMotionTimeSet(mech->modelId[1], 0.0f);
                pos.y += 170.0f;
                fn_1_A9F0(0, pos, rot, mech->no);
                mech->shotWait = 8;
                mech->chargeA = 0;
                mech->chargeB = 0;
                fn_1_FD7C(mech->pilotNo, 0, 3);
            }
            else if ((mech->btn & PAD_BUTTON_A) && !(mech->btn & PAD_BUTTON_B)) {
                mech->chargeB = 0;
                if (mech->chargeA++ == 0) {
                    if (mech->seNo != -1) {
                        HuAudFXStop(mech->seNo);
                    }
                    mech->seNo = HuAudFXPlayPan(1805, lbl_1_rodata_18[mech->type]);
                    Hu3DModelAttrReset(mech->modelId[3], HU3D_ATTR_DISPOFF);
                    Hu3DMotionShapeStartEndSet(mech->modelId[3], 0.0f, 60.0f);
                    Hu3DMotionTimeSet(mech->modelId[3], 0.0f);
                    Hu3DModelAttrSet(mech->modelId[3], HU3D_MOTATTR_LOOP);
                }
                else if (mech->chargeA == 40) {
                    Hu3DMotionShapeStartEndSet(mech->modelId[3], 40.0f, 60.0f);
                    Hu3DMotionTimeSet(mech->modelId[3], 0.0f);
                    Hu3DModelAttrSet(mech->modelId[3], HU3D_MOTATTR_LOOP);
                }
                if (mech->chargeA >= 40 && mech->chargeA % 5 == 0) {
                    fn_1_FD7C(mech->pilotNo, 1, 3);
                }
            }
            else if (mech->chargeA > 0) {
                HuAudFXStop(mech->seNo);
                mech->seNo = -1;
                Hu3DModelAttrSet(mech->modelId[3], HU3D_ATTR_DISPOFF);
                charge = mech->chargeA;
                mech->chargeA = 0;
                if (charge < 90) {
                    break;
                }
                Hu3DModelAttrReset(mech->modelId[2], HU3D_ATTR_DISPOFF);
                Hu3DMotionTimeSet(mech->modelId[2], 0.0f);
                pos.y += 170.0f;
                fn_1_A9F0(1, pos, rot, mech->no);
                mech->shotWait = 8;
                fn_1_FD7C(mech->pilotNo, 2, 30);
            }
            pos = mech->pos;
            rot = mech->rot;
            if ((mech->btnDown & PAD_BUTTON_B) && !(mech->btn & PAD_BUTTON_A)) {
                fn_1_A9F0(2, pos, rot, mech->no);
                mech->shotWait = 32;
                mech->chargeB = 0;
                mech->chargeA = 0;
                fn_1_FD7C(mech->pilotNo, 0, 3);
            }
            else if ((mech->btn & PAD_BUTTON_B) && !(mech->btn & PAD_BUTTON_A)) {
                mech->chargeA = 0;
                if (mech->chargeB++ == 0) {
                    if (mech->seNo != -1) {
                        HuAudFXStop(mech->seNo);
                    }
                    mech->seNo = HuAudFXPlayPan(1805, lbl_1_rodata_18[mech->type]);
                    Hu3DModelAttrReset(mech->modelId[4], HU3D_ATTR_DISPOFF);
                    Hu3DMotionStartEndSet(mech->modelId[4], 0.0f, 60.0f);
                    Hu3DMotionTimeSet(mech->modelId[4], 0.0f);
                    Hu3DModelAttrSet(mech->modelId[4], HU3D_MOTATTR_LOOP);
                }
                else if (mech->chargeB == 30) {
                    Hu3DMotionStartEndSet(mech->modelId[4], 30.0f, 60.0f);
                    Hu3DMotionTimeSet(mech->modelId[4], 0.0f);
                    Hu3DModelAttrSet(mech->modelId[4], HU3D_MOTATTR_LOOP);
                }
                if (mech->chargeB >= 30 && mech->chargeB % 5 == 0) {
                    fn_1_FD7C(mech->pilotNo, 1, 3);
                }
            }
            else if (mech->chargeB > 0) {
                HuAudFXStop(mech->seNo);
                mech->seNo = -1;
                Hu3DModelAttrSet(mech->modelId[4], HU3D_ATTR_DISPOFF);
                charge = mech->chargeB;
                mech->chargeB = 0;
                if (charge < 90) {
                    break;
                }
                fn_1_A9F0(3, pos, rot, mech->no);
                mech->shotWait = 8;
                fn_1_FD7C(mech->pilotNo, 2, 30);
            }
            break;
        case 1:
            pos = mech->pos;
            rot = mech->rot;
            if (mech->btnDown & PAD_BUTTON_A) {
                Hu3DModelAttrReset(mech->modelId[1], HU3D_ATTR_DISPOFF);
                Hu3DMotionTimeSet(mech->modelId[1], 0.0f);
                pos.y += 170.0f;
                fn_1_A9F0(4, pos, rot, mech->no);
                mech->shotWait = 8;
                fn_1_FD7C(mech->pilotNo, 0, 3);
            }
            break;
    }
}

void fn_1_95B0(M519_MECH *mech)
{
    int i;
    M519_MECH *other;
    HuVecF selfCircle;
    HuVecF otherCircle;

    selfCircle.x = mech->pos.x;
    selfCircle.y = mech->pos.z;
    selfCircle.z = mech->radius;
    mech->collideF = FALSE;
    for (i = 0; i < 4; i++) {
        other = lbl_1_bss_F8[i];
        if (other != mech && other->mode != 3) {
            otherCircle.x = other->pos.x;
            otherCircle.y = other->pos.z;
            otherCircle.z = other->radius;
            if (fn_1_F860(&selfCircle, &otherCircle, TRUE)) {
                mech->rot.y = fn_1_FC78(mech->rot.y, HuAtan(selfCircle.x - mech->pos.x, selfCircle.y - mech->pos.z), 0.0009f * mech->speed);
                mech->pos.x = selfCircle.x;
                mech->pos.z = selfCircle.y;
                if (mech->type == other->type) {
                    mech->collideF = TRUE;
                }
            }
        }
    }
}

void fn_1_9700(M519_MECH *mech)
{
    int i;
    M519_OBJ *obj;
    HuVecF selfCircle;
    HuVecF objCircle;
    float rotY;

    selfCircle.x = mech->pos.x;
    selfCircle.y = mech->pos.z;
    selfCircle.z = mech->radius;
    for (i = 0; i < 12; i++) {
        obj = lbl_1_bss_178->obj[i];
        if (obj != NULL && (obj->mode == 3 || obj->mode == 1)) {
            objCircle.x = obj->pos.x;
            objCircle.y = obj->pos.z;
            objCircle.z = obj->radius;
            if (fn_1_F860(&selfCircle, &objCircle, TRUE)) {
                rotY = mech->rot.y;
                mech->rot.y = fn_1_FC78(mech->rot.y, HuAtan(selfCircle.x - mech->pos.x, selfCircle.y - mech->pos.z), 0.0009f * mech->speed);
                mech->pos.x = selfCircle.x;
                mech->pos.z = selfCircle.y;
            }
        }
    }
    objCircle.x = 0.0f;
    objCircle.y = 0.0f;
    objCircle.z = 2390.0f;
    if (fn_1_FA6C(&selfCircle, &objCircle, TRUE)) {
        mech->pos.x = selfCircle.x;
        mech->pos.z = selfCircle.y;
    }
}

void fn_1_98A0(M519_MECH *mech)
{
    mech->mode = 3;
}

void fn_1_98AC(M519_MECH *mech)
{
    static const int seTbl[2] = { 1809, 1811 };
    int time;
    float t;

    HuAudFXPlayPan(seTbl[mech->type], lbl_1_rodata_18[mech->type]);
    fn_1_FD7C(mech->pilotNo, 1, 90);
    switch (mech->type) {
        case 0:
            Hu3DModelAttrSet(mech->modelId[1], HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(mech->modelId[2], HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(mech->modelId[3], HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(mech->modelId[4], HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(mech->modelId[6], HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(mech->modelId[7], HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(mech->modelId[8], HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(mech->modelId[5], HU3D_ATTR_DISPOFF);
            break;
        case 1:
            Hu3DModelAttrSet(mech->modelId[1], HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(mech->modelId[2], HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(mech->modelId[3], HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(mech->modelId[4], HU3D_ATTR_DISPOFF);
            break;
    }
    Hu3DModelAttrReset(mech->modelId[0], HU3D_ATTR_NOCULL);
    Hu3DModelAttrSet(mech->modelId[0], HU3D_ATTR_ZWRITE_OFF);
    for (time = 1; time <= 60; time++) {
        t = (1.0f / 60.0f) * time;
        Hu3DModelTPLvlSet(mech->modelId[0], 1.0f - t);
        if (mech->type == 0) {
            Hu3DModelTPLvlSet(mech->modelId[5], 1.0f - t);
        }
        fn_1_8450(mech);
        M519_SLEEP_MODECHG(M519_MECH, 1);
    }
    fn_1_3098(mech);
    while (TRUE) {
        M519_SLEEP_MODECHG(M519_MECH, 1);
    }
}

void fn_1_9B9C(M519_MECH *mech)
{
    mech->mode = 4;
}

void fn_1_9BA8(M519_MECH *mech)
{
    int time;
    float t;
    float s;
    HuVecF pos;

    pos = mech->pos;
    Hu3DModelAttrReset(mech->modelId[0], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(mech->modelId[5], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(mech->modelId[6], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(mech->modelId[7], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(mech->modelId[0], HU3D_ATTR_NOCULL);
    Hu3DModelAttrSet(mech->modelId[0], HU3D_ATTR_ZWRITE_OFF);
    HuAudFXPlay(1801);
    for (time = 1; time <= 100; time++) {
        t = 0.01f * (time + 1);
        s = HuCos(90.0f * t);
        Hu3DModelTPLvlSet(mech->modelId[0], t);
        Hu3DModelTPLvlSet(mech->modelId[5], t);
        pos.y = 1000.0f * s;
        fn_1_3138(mech, pos);
        fn_1_8450(mech);
        M519_SLEEP_MODECHG(M519_MECH, 1);
    }
    Hu3DModelTPLvlSet(mech->modelId[0], 1.0f);
    Hu3DModelTPLvlSet(mech->modelId[5], 1.0f);
    Hu3DModelAttrSet(mech->modelId[0], HU3D_ATTR_NOCULL);
    Hu3DModelAttrReset(mech->modelId[0], HU3D_ATTR_ZWRITE_OFF);
    HuAudFXPlay(1824);
    fn_1_2FA0(mech, 6, 1.0f);
    fn_1_FD7C(mech->pilotNo, 2, 30);
    M519_SLEEP_MODECHG(M519_MECH, 90);
    fn_1_2FA0(mech, 0, 1.0f);
    fn_1_35B0(mech);
}

void fn_1_A068(M519_MECH *mech)
{
    int time;
    float t;

    Hu3DModelAttrReset(mech->modelId[0], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(mech->modelId[2], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(mech->modelId[0], HU3D_ATTR_NOCULL);
    Hu3DModelAttrSet(mech->modelId[0], HU3D_ATTR_ZWRITE_OFF);
    for (time = 0; time < 90; time++) {
        t = (1.0 / 90.0) * time;
        Hu3DModelTPLvlSet(mech->modelId[0], t);
        Hu3DModelTPLvlSet(mech->modelId[2], t);
        M519_SLEEP_MODECHG(M519_MECH, 1);
    }
    Hu3DModelTPLvlSet(mech->modelId[0], 1.0f);
    Hu3DModelTPLvlSet(mech->modelId[2], 1.0f);
    Hu3DModelAttrSet(mech->modelId[0], HU3D_ATTR_NOCULL);
    Hu3DModelAttrReset(mech->modelId[0], HU3D_ATTR_ZWRITE_OFF);
    fn_1_35B0(mech);
}

BOOL fn_1_A1D4(M519_MECH *mech)
{
    return (mech->hp == 0) ? TRUE : FALSE;
}

M519_PILOT *fn_1_A204(s16 grp, s16 playerNo)
{
    static const int motTbl[CHARNO_MAX][6] = {
        { CHARMOT_DATANUM(CHARMOT_HSF_c000m1_300, mario), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_303, mario), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_304, mario),
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_301, mario), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_306, mario), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_307, mario) },
        { CHARMOT_DATANUM(CHARMOT_HSF_c000m1_300, luigi), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_303, luigi), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_304, luigi),
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_301, luigi), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_306, luigi), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_307, luigi) },
        { CHARMOT_DATANUM(CHARMOT_HSF_c000m1_300, peach), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_303, peach), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_304, peach),
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_301, peach), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_306, peach), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_307, peach) },
        { CHARMOT_DATANUM(CHARMOT_HSF_c000m1_300, yoshi), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_303, yoshi), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_304, yoshi),
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_301, yoshi), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_306, yoshi), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_307, yoshi) },
        { CHARMOT_DATANUM(CHARMOT_HSF_c000m1_300, wario), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_303, wario), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_304, wario),
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_301, wario), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_306, wario), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_307, wario) },
        { CHARMOT_DATANUM(CHARMOT_HSF_c000m1_300, daisy), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_303, daisy), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_304, daisy),
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_301, daisy), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_306, daisy), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_307, daisy) },
        { CHARMOT_DATANUM(CHARMOT_HSF_c000m1_300, waluigi), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_303, waluigi),
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_304, waluigi), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_301, waluigi),
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_306, waluigi), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_307, waluigi) },
        { CHARMOT_DATANUM(CHARMOT_HSF_c000m1_300, kinopio), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_303, kinopio),
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_304, kinopio), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_301, kinopio),
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_306, kinopio), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_307, kinopio) },
        { CHARMOT_DATANUM(CHARMOT_HSF_c000m1_300, teresa), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_303, teresa),
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_304, teresa), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_301, teresa),
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_306, teresa), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_307, teresa) },
        { CHARMOT_DATANUM(CHARMOT_HSF_c000m1_300, minikoopa), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_303, minikoopa),
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_304, minikoopa), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_301, minikoopa),
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_306, minikoopa), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_307, minikoopa) },
        { CHARMOT_DATANUM(CHARMOT_HSF_c000m1_300, minikoopa), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_303, minikoopa),
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_304, minikoopa), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_301, minikoopa),
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_306, minikoopa), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_307, minikoopa) },
        { CHARMOT_DATANUM(CHARMOT_HSF_c000m1_300, minikoopa), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_303, minikoopa),
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_304, minikoopa), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_301, minikoopa),
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_306, minikoopa), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_307, minikoopa) },
        { CHARMOT_DATANUM(CHARMOT_HSF_c000m1_300, minikoopa), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_303, minikoopa),
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_304, minikoopa), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_301, minikoopa),
            CHARMOT_DATANUM(CHARMOT_HSF_c000m1_306, minikoopa), CHARMOT_DATANUM(CHARMOT_HSF_c000m1_307, minikoopa) },
    };
    static const int modelTbl[CHARNO_MAX] = {
        M519_DATANUM(93),
        M519_DATANUM(94),
        M519_DATANUM(95),
        M519_DATANUM(96),
        M519_DATANUM(97),
        M519_DATANUM(98),
        M519_DATANUM(99),
        M519_DATANUM(100),
        M519_DATANUM(101),
        M519_DATANUM(102),
        M519_DATANUM(103),
        M519_DATANUM(104),
        M519_DATANUM(105),
    };
    M519_PILOT *pilot;
    int i;

    pilot = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M519_PILOT), HU_MEMNUM_OVL);
    pilot->grp = grp;
    pilot->playerNo = playerNo;
    pilot->comF = GwPlayerConf[playerNo].type;
    pilot->charNo = GwPlayerConf[playerNo].charNo;
    pilot->dif = GwPlayerConf[playerNo].dif;
    pilot->padNo = GwPlayerConf[playerNo].padNo;
    pilot->mode = 0;
    pilot->prevMode = 0;
    pilot->modelId[0] = CharModelCreate(pilot->charNo, CHAR_MODEL1);
    pilot->modelId[1] = Hu3DModelCreateData(modelTbl[pilot->charNo]);
    Hu3DModelAttrSet(pilot->modelId[1], HU3D_ATTR_NOCULL);
    Hu3DModelAttrSet(pilot->modelId[0], HU3D_ATTR_NOCULL);
    Hu3DModelAttrSet(pilot->modelId[1], HU3D_ATTR_ZWRITE_OFF);
    Hu3DModelAttrSet(pilot->modelId[0], HU3D_ATTR_ZWRITE_OFF);
    Hu3DModelAttrSet(pilot->modelId[0], HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(pilot->modelId[0], 2);
    Hu3DModelLayerSet(pilot->modelId[1], 2);
    for (i = 0; i < 6; i++) {
        pilot->motId[i] = CharMotionCreate(pilot->charNo, motTbl[pilot->charNo][i]);
    }
    CharMotionDataClose(pilot->charNo);
    Hu3DMotionSet(pilot->modelId[0], pilot->motId[0]);
    Hu3DModelAttrSet(pilot->modelId[0], HU3D_MOTATTR_PAUSE);
    pilot->process = HuPrcChildCreate(fn_1_A40C, 4096, 4096, 0, HuPrcCurrentGet());
    pilot->process->property = pilot;
    return pilot;
}

void fn_1_A40C(void)
{
    M519_PILOT *pilot = HuPrcCurrentGet()->property;

    while (TRUE) {
        lbl_1_data_48[pilot->mode](pilot);
    }
}

void fn_1_A44C(M519_PILOT *pilot, HuVecF pos)
{
    int i;

    pilot->pos = pos;
    for (i = 0; i < 2; i++) {
        Hu3DModelPosSet(pilot->modelId[i], pos.x, pos.y, pos.z);
    }
}

HuVecF fn_1_A4D0(M519_PILOT *pilot)
{
    return pilot->pos;
}

void fn_1_A4EC(M519_PILOT *pilot, HuVecF rot)
{
    int i;

    pilot->rot = rot;
    for (i = 0; i < 2; i++) {
        Hu3DModelRotSet(pilot->modelId[i], rot.x, rot.y, rot.z);
    }
}

HuVecF fn_1_A570(M519_PILOT *pilot)
{
    return pilot->rot;
}

void fn_1_A58C(M519_PILOT *pilot)
{
    pilot->mode = 0;
}

void fn_1_A598(M519_PILOT *pilot)
{
    while (TRUE) {
        M519_SLEEP_MODECHG(M519_PILOT, 1);
    }
}

void fn_1_A628(M519_PILOT *pilot)
{
    pilot->mode = 1;
}

void fn_1_A634(M519_PILOT *pilot)
{
    Hu3DModelAttrSet(pilot->modelId[1], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(pilot->modelId[0], HU3D_ATTR_DISPOFF);
    Hu3DModelTPLvlSet(pilot->modelId[0], 1.0f);
    fn_1_3580(lbl_1_bss_F8[pilot->mechNo]);
    fn_1_A58C(pilot);
}

void fn_1_A6B8(M519_PILOT *pilot)
{
    pilot->mode = 2;
}

void fn_1_A6C4(M519_PILOT *pilot)
{
    Hu3DMotionShiftSet(pilot->modelId[0], pilot->motId[4], 0.0f, 8.0f, HU3D_MOTATTR_NONE);
    while (TRUE) {
        M519_SLEEP_MODECHG(M519_PILOT, 1);
    }
}

void fn_1_A770(M519_PILOT *pilot)
{
    pilot->mode = 3;
}

void fn_1_A77C(M519_PILOT *pilot)
{
    Hu3DMotionShiftSet(pilot->modelId[0], pilot->motId[5], 0.0f, 8.0f, HU3D_MOTATTR_NONE);
    while (TRUE) {
        M519_SLEEP_MODECHG(M519_PILOT, 1);
    }
}

void fn_1_A828(M519_PILOT *pilot)
{
    pilot->mode = 4;
}

void fn_1_A834(M519_PILOT *pilot)
{
    int i;
    int time;
    float t;

    Hu3DModelAttrReset(pilot->modelId[1], HU3D_ATTR_NOCULL);
    Hu3DModelAttrReset(pilot->modelId[0], HU3D_ATTR_NOCULL);
    Hu3DModelAttrSet(pilot->modelId[1], HU3D_ATTR_ZWRITE_OFF);
    Hu3DModelAttrSet(pilot->modelId[0], HU3D_ATTR_ZWRITE_OFF);
    for (time = 0; time < 90; time++) {
        t = (1.0 / 90.0) * time;
        for (i = 0; i < 2; i++) {
            Hu3DModelTPLvlSet(pilot->modelId[i], 1.0f - t);
        }
        M519_SLEEP_MODECHG(M519_PILOT, 1);
    }
    for (i = 0; i < 2; i++) {
        Hu3DModelAttrSet(pilot->modelId[i], HU3D_ATTR_DISPOFF);
    }
    while (TRUE) {
        M519_SLEEP_MODECHG(M519_PILOT, 1);
    }
}

BOOL fn_1_A9F0(s16 type, HuVecF pos, HuVecF rot, s16 ownerNo)
{
    int i;
    M519_SHOT *shot;

    for (i = 0; i < 44; i++) {
        shot = lbl_1_bss_38[i];
        if (shot != NULL && shot->mode == 0 && shot->prevMode == 0 && shot->type == type) {
            shot->ownerNo = ownerNo;
            shot->id = lbl_1_bss_C++ % 32000;
            fn_1_AE24(shot, pos);
            fn_1_AEC4(shot, rot);
            fn_1_B0B0(shot);
            return TRUE;
        }
    }
    return FALSE;
}

M519_SHOT *fn_1_AB40(s16 type)
{
    static const int modelTbl[5][2] = {
        { M519_DATANUM(53), M519_DATANUM(54) },
        { M519_DATANUM(56), M519_DATANUM(57) },
        { M519_DATANUM(61), M519_DATANUM(57) },
        { M519_DATANUM(62), M519_DATANUM(57) },
        { M519_DATANUM(59), M519_DATANUM(60) },
    };
    static const M519_SHOTPARAM paramTbl[5] = {
        { 73.333336f, 12.0f, 12.0f, 40, 1, 3, 30, TRUE },
        { 76.666664f, 80.0f, 80.0f, 120, 1, 15, 200, FALSE },
        { 0.0f, 500.0f, 200.0f, 15, 15, 1, 200, FALSE },
        { 0.0f, 1000.0f, 200.0f, 30, 30, 3, 200, FALSE },
        { 73.333336f, 12.0f, 12.0f, 25, 1, 1, 10, TRUE },
    };
    M519_SHOT *shot;
    int i;

    shot = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M519_SHOT), HU_MEMNUM_OVL);
    shot->id = 0;
    shot->type = type;
    shot->activeF = FALSE;
    shot->ownerNo = -1;
    shot->radius = 1.0f;
    shot->damage = paramTbl[type].damage;
    shot->knockback = paramTbl[type].knockback;
    shot->radiusEnd = paramTbl[type].radiusEnd;
    shot->radiusStart = paramTbl[type].radiusStart;
    shot->life = paramTbl[type].life;
    shot->growTime = paramTbl[type].growTime;
    shot->speed = paramTbl[type].speed;
    shot->pierceF = paramTbl[type].pierceF;
    shot->curSpeed = 0.0f;
    shot->pos.x = 0.0f;
    shot->pos.y = 0.0f;
    shot->pos.z = 0.0f;
    shot->rot.x = 0.0f;
    shot->rot.y = 0.0f;
    shot->rot.z = 0.0f;
    shot->mode = 0;
    shot->prevMode = 0;
    for (i = 0; i < 2; i++) {
        shot->modelId[i] = Hu3DModelCreateData(modelTbl[type][i]);
        Hu3DModelAttrSet(shot->modelId[i], HU3D_ATTR_NOCULL);
        Hu3DModelAttrSet(shot->modelId[i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(shot->modelId[i], 2);
    }
    shot->process = HuPrcChildCreate(fn_1_ADE4, 2304, 8192, 0, HuPrcCurrentGet());
    shot->process->property = shot;
    return shot;
}

void fn_1_ADE4(void)
{
    M519_SHOT *shot = HuPrcCurrentGet()->property;

    while (TRUE) {
        lbl_1_data_5C[shot->mode](shot);
    }
}

void fn_1_AE24(M519_SHOT *shot, HuVecF pos)
{
    int i;

    i = i;
    shot->pos = pos;
    for (i = 0; i < 2; i++) {
        Hu3DModelPosSet(shot->modelId[i], pos.x, pos.y, pos.z);
    }
}

HuVecF fn_1_AEA8(M519_SHOT *shot)
{
    return shot->pos;
}

void fn_1_AEC4(M519_SHOT *shot, HuVecF rot)
{
    int i;

    shot->rot = rot;
    for (i = 0; i < 2; i++) {
        Hu3DModelRotSet(shot->modelId[i], rot.x, rot.y, rot.z);
    }
}

HuVecF fn_1_AF48(M519_SHOT *shot)
{
    return shot->rot;
}

void fn_1_AF64(M519_SHOT *shot)
{
    s32 i;
    HuVecF pos = { 0.0f, 0.0f, 0.0f };

    i = i;
    shot->pos = pos;
    for (i = 0; i < 2; i++) {
        Hu3DModelPosSet(shot->modelId[i], pos.x, pos.y, pos.z);
    }
    shot->mode = 0;
}

void fn_1_B004(M519_SHOT *shot)
{
    int i;

    for (i = 0; i < 2; i++) {
        Hu3DModelAttrSet(shot->modelId[i], HU3D_ATTR_DISPOFF);
    }
    while (TRUE) {
        M519_SLEEP_MODECHG(M519_SHOT, 1);
    }
}

void fn_1_B0B0(M519_SHOT *shot)
{
    shot->mode = 1;
}

void fn_1_B0BC(M519_SHOT *shot)
{
    static const int seTbl[5] = { 1804, 1806, 1807, 1807, 1810 };
    BOOL hitF;
    float scale;
    int time;
    float dx;
    float dz;
    HuVecF pos;
    s16 growTime;

    HuAudFXPlayPan(seTbl[shot->type], lbl_1_rodata_18[lbl_1_bss_F8[shot->ownerNo]->type]);
    Hu3DModelAttrReset(shot->modelId[0], HU3D_ATTR_DISPOFF);
    switch (shot->type) {
        case 1:
            Hu3DModelAttrSet(shot->modelId[0], HU3D_MOTATTR_LOOP);
            break;
        case 0:
        case 2:
        case 4:
            Hu3DModelAttrReset(shot->modelId[0], HU3D_MOTATTR_LOOP);
            break;
    }
    Hu3DMotionShapeStartEndSet(shot->modelId[0], 0.0f, Hu3DMotionMaxTimeGet(shot->modelId[0]));
    Hu3DMotionTimeSet(shot->modelId[0], 0.0f);
    shot->activeF = TRUE;
    hitF = FALSE;
    scale = 0.0f;
    for (time = 0; time < shot->life; time++) {
        if (time < 10) {
            scale += 0.1f;
        }
        if (time > shot->life - 10) {
            scale -= 0.1f;
        }
        shot->curSpeed = shot->speed;
        dx = shot->curSpeed * HuSin(shot->rot.y);
        dz = shot->curSpeed * HuCos(shot->rot.y);
        if (shot->type != 3 && shot->type != 2) {
            Hu3DModelScaleSet(shot->modelId[0], 1.0f, 1.0f, scale);
        }
        pos = shot->pos;
        pos.x += dx;
        pos.z += dz;
        fn_1_AE24(shot, pos);
        growTime = (time >= shot->growTime) ? shot->growTime : time;
        shot->radius = shot->radiusStart + ((shot->radiusEnd - shot->radiusStart) * HuSin(90.0f * (1.0f / shot->growTime) * growTime));
        if (shot->type == 0 || shot->type == 1 || shot->type == 4) {
            if (fn_1_BECC(shot)) {
                hitF = TRUE;
            }
            if (fn_1_B9BC(shot)) {
                hitF = TRUE;
            }
            if (fn_1_B70C(shot) && shot->type != 1) {
                hitF = TRUE;
            }
            if (fn_1_BAC8(shot) && shot->type != 1) {
                hitF = TRUE;
            }
        }
        else {
            fn_1_BECC(shot);
            fn_1_B70C(shot);
            fn_1_BAC8(shot);
        }
        if (hitF) {
            break;
        }
        M519_SLEEP_MODECHG(M519_SHOT, 1);
    }
    shot->activeF = FALSE;
    if (shot->type == 3 || shot->type == 2) {
        M519_SLEEP_MODECHG(M519_SHOT, 20);
    }
    if (hitF) {
        Hu3DModelAttrSet(shot->modelId[0], HU3D_ATTR_DISPOFF);
        Hu3DModelAttrReset(shot->modelId[1], HU3D_ATTR_DISPOFF);
        Hu3DMotionTimeSet(shot->modelId[1], 0.0f);
        M519_SLEEP_MODECHG(M519_SHOT, Hu3DMotionMaxTimeGet(shot->modelId[1]));
        Hu3DModelAttrReset(shot->modelId[1], HU3D_ATTR_DISPOFF);
    }
    fn_1_AF64(shot);
}

BOOL fn_1_B70C(M519_SHOT *shot)
{
    static const int seTbl[2] = { 1812, 1813 };
    M519_MECH *owner;
    int i;
    M519_MECH *mech;
    float dx;
    float dz;
    HuVecF shotCircle;
    HuVecF mechCircle;

    if (shot->ownerNo == -1) {
        return FALSE;
    }
    owner = lbl_1_bss_F8[shot->ownerNo];
    shotCircle.x = shot->pos.x;
    shotCircle.y = shot->pos.z;
    shotCircle.z = shot->radius;
    i = i;
    for (i = 0; i < 4; i++) {
        mech = lbl_1_bss_F8[i];
        if (mech != owner && mech->mode != 3) {
            mechCircle.x = mech->pos.x;
            mechCircle.y = mech->pos.z;
            mechCircle.z = mech->radius;
            if (fn_1_F860(&shotCircle, &mechCircle, TRUE)) {
                HuAudFXPlay(seTbl[mech->type]);
                if (mech->type != owner->type && mech->hitId != shot->id && !lbl_1_bss_8) {
                    if ((mech->hp -= shot->damage) < 0) {
                        mech->hp = 0;
                    }
                }
                mech->hitId = shot->id;
                if (shot->pierceF) {
                    shot->pos.x = shotCircle.x;
                    shot->pos.z = shotCircle.y;
                    fn_1_AE24(shot, shot->pos);
                }
                if (mech->mode == 1) {
                    if (shot->curSpeed == 0.0f) {
                        dx = mech->pos.x - shot->pos.x;
                        dz = mech->pos.z - shot->pos.z;
                        shot->rot.y = HuAtan(dx, dz);
                    }
                    mech->knockback = shot->knockback;
                    mech->knockbackAngle = shot->rot.y;
                }
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL fn_1_B9BC(M519_SHOT *shot)
{
    HuVecF shotCircle;
    HuVecF stageCircle;

    shotCircle.x = shot->pos.x;
    shotCircle.y = shot->pos.z;
    shotCircle.z = shot->radius;
    stageCircle.x = 0.0f;
    stageCircle.y = 0.0f;
    stageCircle.z = 2390.0f;
    if (fn_1_FA6C(&shotCircle, &stageCircle, TRUE)) {
        shot->pos.x = shotCircle.x;
        shot->pos.z = shotCircle.y;
        fn_1_AE24(shot, shot->pos);
        return TRUE;
    }
    return FALSE;
}

BOOL fn_1_BAC8(M519_SHOT *shot)
{
    static const BOOL damageTbl[5][2] = {
        { TRUE, TRUE },
        { TRUE, TRUE },
        { TRUE, TRUE },
        { TRUE, TRUE },
        { FALSE, TRUE },
    };
    static const int seTbl[2] = { 1819, 1820 };
    int i;
    M519_OBJ *obj;
    M519_MECH *owner;
    HuVecF shotCircle;
    HuVecF objCircle;
    HuVecF objPos;
    float angle;

    shotCircle.x = shot->pos.x;
    shotCircle.y = shot->pos.z;
    shotCircle.z = shot->radius;
    i = i;
    i = 0;
    while (i < 12) {
        obj = lbl_1_bss_178->obj[i];
        if (obj != NULL && obj->mode == 3 && obj->hp > 0 && obj->hitId != shot->id) {
            objCircle.x = obj->pos.x;
            objCircle.y = obj->pos.z;
            objCircle.z = obj->radius;
            if (fn_1_F860(&shotCircle, &objCircle, TRUE)) {
                owner = lbl_1_bss_F8[shot->ownerNo];
                objPos = obj->pos;
                angle = fn_1_7EB8(owner, &objPos);
                HuAudFXPlayVolPan(seTbl[0], fn_1_FE20(HU3D_CAM0, shot->pos), fn_1_10244(HU3D_CAM0, shot->pos));
                HuAudFXPlayVolPan(seTbl[1], fn_1_FE20(HU3D_CAM1 | HU3D_CAM2 | HU3D_CAM3, shot->pos),
                    fn_1_10244(HU3D_CAM1 | HU3D_CAM2 | HU3D_CAM3, shot->pos));
                if (damageTbl[shot->type][obj->type] && obj->hitId != shot->id) {
                    if ((obj->hp -= shot->damage) < 0) {
                        obj->hp = 0;
                    }
                }
                obj->hitId = shot->id;
                obj->hitOwnerNo = shot->ownerNo;
                if (shot->pierceF) {
                    shot->pos.x = shotCircle.x;
                    shot->pos.z = shotCircle.y;
                    fn_1_AE24(shot, shot->pos);
                }
                return TRUE;
            }
        }
        i++;
    }
    return FALSE;
}

BOOL fn_1_BECC(M519_SHOT *shot)
{
    int i;
    M519_SHOT *other;
    HuVecF shotCircle;
    HuVecF otherCircle;

    if (shot->type == 1) {
        return FALSE;
    }
    shotCircle.x = shot->pos.x;
    shotCircle.y = shot->pos.z;
    shotCircle.z = shot->radius;
    i = i;
    for (i = 0; i < 44; i++) {
        other = lbl_1_bss_38[i];
        if (other != NULL && shot->mode == 1 && other->activeF && other->ownerNo != shot->ownerNo && other != shot) {
            otherCircle.x = other->pos.x;
            otherCircle.y = other->pos.z;
            otherCircle.z = other->radius;
            if (fn_1_F860(&shotCircle, &otherCircle, TRUE)) {
                if (shot->pierceF) {
                    shot->pos.x = shotCircle.x;
                    shot->pos.z = shotCircle.y;
                    fn_1_AE24(shot, shot->pos);
                }
                return TRUE;
            }
        }
    }
    return FALSE;
}

M519_OBJ *fn_1_C058(s16 type)
{
    static const int modelTbl[3][6] = {
        { M519_DATANUM(7), M519_DATANUM(8), M519_DATANUM(9), M519_DATANUM(10), M519_DATANUM(11), M519_DATANUM(12) },
        { M519_DATANUM(17), M519_DATANUM(18), M519_DATANUM(19), M519_DATANUM(20), M519_DATANUM(21), M519_DATANUM(22) },
        { M519_DATANUM(27), M519_DATANUM(28), M519_DATANUM(29), M519_DATANUM(30), M519_DATANUM(31), M519_DATANUM(32) },
    };
    static const s16 radiusTbl[2] = { 140, 100 };
    M519_OBJ *obj;
    int i;

    obj = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M519_OBJ), HU_MEMNUM_OVL);
    obj->type = type;
    obj->hitId = -1;
    obj->radius = radiusTbl[type];
    obj->hp = 1;
    obj->pos.x = 0.0f;
    obj->pos.y = 0.0f;
    obj->pos.z = 0.0f;
    obj->rot.x = 0.0f;
    obj->rot.y = 0.0f;
    obj->rot.z = 0.0f;
    obj->respawnF = FALSE;
    obj->hitOwnerNo = -1;
    obj->mode = 0;
    obj->prevMode = 0;
    for (i = 0; i < 3; i++) {
        obj->modelId[i] = Hu3DModelCreateData(modelTbl[lbl_1_bss_30][type * 3 + i]);
        Hu3DModelAttrSet(obj->modelId[i], HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(obj->modelId[i], HU3D_ATTR_NOCULL);
        Hu3DModelLayerSet(obj->modelId[i], 2);
    }
    obj->process = HuPrcChildCreate(fn_1_C248, 2304, 8192, 0, HuPrcCurrentGet());
    obj->process->property = obj;
    return obj;
}

void fn_1_C248(void)
{
    M519_OBJ *obj = HuPrcCurrentGet()->property;

    while (TRUE) {
        lbl_1_data_64[obj->mode](obj);
    }
}

void fn_1_C288(M519_OBJ *obj, HuVecF pos)
{
    int i;

    obj->pos = pos;
    for (i = 0; i < 3; i++) {
        Hu3DModelPosSet(obj->modelId[i], pos.x, pos.y, pos.z);
    }
}

HuVecF fn_1_C30C(M519_OBJ *obj)
{
    return obj->pos;
}

void fn_1_C328(M519_OBJ *obj, HuVecF rot)
{
    int i;

    obj->rot = rot;
    for (i = 0; i < 3; i++) {
        Hu3DModelRotSet(obj->modelId[i], rot.x, rot.y, rot.z);
    }
}

HuVecF fn_1_C3AC(M519_OBJ *obj)
{
    return obj->rot;
}

void fn_1_C3C8(M519_OBJ *obj)
{
    obj->mode = 0;
}

void fn_1_C3D4(M519_OBJ *obj)
{
    while (TRUE) {
        M519_SLEEP_MODECHG(M519_OBJ, 1);
    }
}

void fn_1_C464(M519_OBJ *obj)
{
    obj->mode = 3;
}

void fn_1_C470(M519_OBJ *obj)
{
    Hu3DModelAttrReset(obj->modelId[0], HU3D_ATTR_DISPOFF);
    Hu3DMotionTimeSet(obj->modelId[0], 0.0f);
    Hu3DModelAttrSet(obj->modelId[1], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(obj->modelId[2], HU3D_ATTR_DISPOFF);
    while (TRUE) {
        if (obj->hp <= 0) {
            fn_1_C788(obj, TRUE);
        }
        M519_SLEEP_MODECHG(M519_OBJ, 1);
    }
}

void fn_1_C544(M519_OBJ *obj)
{
    static const s16 hpTbl[2] = { 15, 10 };

    obj->hp = hpTbl[obj->type];
    obj->mode = 1;
}

void fn_1_C570(M519_OBJ *obj)
{
    static const int seTbl[2] = { 1817, 1818 };
    M519_MECH *mech;

    if (obj->hitOwnerNo != -1) {
        mech = lbl_1_bss_F8[obj->hitOwnerNo];
        HuAudFXPlayVolPan(seTbl[0], fn_1_FE20(HU3D_CAM0, obj->pos), fn_1_10244(HU3D_CAM0, obj->pos));
        HuAudFXPlayVolPan(seTbl[1], fn_1_FE20(HU3D_CAM1 | HU3D_CAM2 | HU3D_CAM3, obj->pos),
            fn_1_10244(HU3D_CAM1 | HU3D_CAM2 | HU3D_CAM3, obj->pos));
    }
    obj->hitOwnerNo = -1;
    Hu3DModelAttrReset(obj->modelId[1], HU3D_MOTATTR_PAUSE);
    Hu3DModelAttrReset(obj->modelId[1], HU3D_ATTR_DISPOFF);
    Hu3DMotionTimeSet(obj->modelId[1], 0.0f);
    Hu3DModelAttrSet(obj->modelId[0], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(obj->modelId[2], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(obj->modelId[0], HU3D_MOTATTR_PAUSE);
    Hu3DModelAttrSet(obj->modelId[2], HU3D_MOTATTR_PAUSE);
    M519_SLEEP_MODECHG(M519_OBJ, Hu3DMotionMaxTimeGet(obj->modelId[1]));
    Hu3DModelAttrReset(obj->modelId[1], HU3D_MOTATTR_PAUSE);
    fn_1_C464(obj);
}

void fn_1_C788(M519_OBJ *obj, BOOL respawnF)
{
    obj->respawnF = respawnF;
    obj->mode = 2;
}

void fn_1_C798(M519_OBJ *obj)
{
    static const int seTbl[2] = { 1815, 1816 };
    static const int respawnTbl[2] = { 300, 300 };
    M519_MECH *mech;
    int i;
    int respawnTime;
    HuVecF objCircle;
    HuVecF mechCircle;

    if (obj->hitOwnerNo != -1) {
        mech = lbl_1_bss_F8[obj->hitOwnerNo];
        HuAudFXPlayVolPan(seTbl[0], fn_1_FE20(HU3D_CAM0, obj->pos), fn_1_10244(HU3D_CAM0, obj->pos));
        HuAudFXPlayVolPan(seTbl[1], fn_1_FE20(HU3D_CAM1 | HU3D_CAM2 | HU3D_CAM3, obj->pos),
            fn_1_10244(HU3D_CAM1 | HU3D_CAM2 | HU3D_CAM3, obj->pos));
    }
    Hu3DModelAttrReset(obj->modelId[2], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(obj->modelId[2], HU3D_MOTATTR_PAUSE);
    Hu3DMotionTimeSet(obj->modelId[2], 0.0f);
    Hu3DModelAttrSet(obj->modelId[0], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(obj->modelId[1], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(obj->modelId[0], HU3D_MOTATTR_PAUSE);
    Hu3DModelAttrSet(obj->modelId[1], HU3D_MOTATTR_PAUSE);
    M519_SLEEP_MODECHG(M519_OBJ, Hu3DMotionMaxTimeGet(obj->modelId[1]));
    Hu3DModelAttrSet(obj->modelId[2], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(obj->modelId[2], HU3D_MOTATTR_PAUSE);
    respawnTime = respawnTbl[obj->type];
    for (i = 0; i < respawnTime; i++) {
        M519_SLEEP_MODECHG(M519_OBJ, 1);
    }
    objCircle.x = obj->pos.x;
    objCircle.y = obj->pos.z;
    objCircle.z = 100.0f + obj->radius;
    if (obj->respawnF) {
        while (TRUE) {
            for (i = 0; i < 4; i++) {
                mech = lbl_1_bss_F8[i];
                if (mech != NULL) {
                    mechCircle.x = mech->pos.x;
                    mechCircle.y = mech->pos.z;
                    mechCircle.z = mech->radius;
                    if (fn_1_F860(&objCircle, &mechCircle, FALSE) && !lbl_1_bss_0 && !lbl_1_bss_4) {
                        break;
                    }
                }
            }
            if (i == 4) {
                break;
            }
            M519_SLEEP_MODECHG(M519_OBJ, 1);
        }
    }
    else {
        while (TRUE) {
            M519_SLEEP_MODECHG(M519_OBJ, 1);
        }
    }
    fn_1_C544(obj);
}

void fn_1_CBEC(M519_OBJ *obj)
{
    int i;

    for (i = 0; i < 3; i++) {
        Hu3DModelAttrSet(obj->modelId[i], HU3D_ATTR_DISPOFF);
    }
}

M519_PARTICLE *fn_1_CC44(void)
{
    M519_PARTICLE *particle;
    HU3DMODEL *modelP;
    HU3DPARTICLE *particleP;

    particle = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M519_PARTICLE), HU_MEMNUM_OVL);
    particle->anim = HuSprAnimDataRead(M519_DATANUM(69));
    particle->modelId = Hu3DParticleCreate(particle->anim, 42);
    Hu3DParticleBlendModeSet(particle->modelId, 1);
    modelP = &Hu3DData[particle->modelId];
    particleP = modelP->hookData;
    particleP->work = particle;
    Hu3DParticleHookSet(particle->modelId, fn_1_CCF4);
    return particle;
}

void fn_1_CCF4(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    lbl_1_data_74[particleP->dataCnt](particleP->work, modelP, particleP, mtx);
}

void fn_1_CD54(M519_PARTICLE *particle, HuVecF pos)
{
    particle->pos = pos;
}

HuVecF fn_1_CD70(M519_PARTICLE *particle)
{
    return particle->pos;
}

void fn_1_CD8C(M519_PARTICLE *particle)
{
    HU3DMODEL *modelP;
    HU3DPARTICLE *particleP;

    Hu3DModelAttrSet(particle->modelId, HU3D_ATTR_DISPOFF);
    modelP = &Hu3DData[particle->modelId];
    particleP = modelP->hookData;
    particleP->dataCnt = 0;
}

void fn_1_CDF0(M519_PARTICLE *particle, HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx) { }

void fn_1_CDF4(M519_PARTICLE *particle)
{
    HU3DMODEL *modelP;
    HU3DPARTICLE *particleP;
    HU3DPARTICLEDATA *data;
    int i;

    Hu3DModelAttrReset(particle->modelId, HU3D_ATTR_DISPOFF);
    modelP = &Hu3DData[particle->modelId];
    particleP = modelP->hookData;
    data = particleP->data;
    for (i = 0, data = particleP->data; i < particleP->maxCnt; i++, data++) {
        data->time = 0;
        data->parManId = 0;
        data->pos.x = 0.0f;
        data->pos.y = -100.0f;
        data->pos.z = 0.0f;
        data->color.r = 255;
        data->color.g = 255;
        data->color.b = 255;
        data->color.a = 255;
        data->scale = 1.0f;
    }
    particleP->dataCnt = 1;
}

void fn_1_CEE4(M519_PARTICLE *particle, HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *data;
    int i;

    for (i = 0, data = particleP->data; i < particleP->maxCnt; i++, data++) {
        switch (data->time) {
            case 0:
                data->pos.x = 0.0f;
                data->pos.y = -100.0f;
                data->pos.z = 0.0f;
                break;
            case 1:
                data->color.a = 64 - (s32)(4.0f * data->parManId);
                if (data->parManId++ > 16) {
                    data->color.a = 0;
                    data->time = 0;
                }
                break;
        }
    }
}

BOOL fn_1_CFE8(M519_PARTICLE *particle)
{
    HU3DMODEL *modelP;
    HU3DPARTICLE *particleP;
    HU3DPARTICLEDATA *data;
    int i;

    modelP = &Hu3DData[particle->modelId];
    particleP = modelP->hookData;
    for (data = particleP->data, i = 0; i < particleP->maxCnt; i++, data++) {
        if (data->time == 0) {
            data->time = 1;
            data->parManId = 0;
            data->pos.x = particle->pos.x;
            data->pos.y = particle->pos.y;
            data->pos.z = particle->pos.z;
            data->color.r = 255;
            data->color.g = 255;
            data->color.b = 255;
            data->color.a = 255;
            data->scale = 5.0f;
            return TRUE;
        }
    }
    return FALSE;
}

M519_HUD *fn_1_D0BC(s16 type, s16 mechNo)
{
    static const int sprTbl[4][18] = {
        { M519_DATANUM(76), M519_DATANUM(73), M519_DATANUM(75), M519_DATANUM(74), M519_DATANUM(74), M519_DATANUM(74), M519_DATANUM(74),
            M519_DATANUM(80), M519_DATANUM(81), M519_DATANUM(82), M519_DATANUM(83), M519_DATANUM(84), M519_DATANUM(85), M519_DATANUM(86),
            M519_DATANUM(87), M519_DATANUM(88), M519_DATANUM(89), M519_DATANUM(90) },
        { M519_DATANUM(77), M519_DATANUM(73), M519_DATANUM(75), M519_DATANUM(74), M519_DATANUM(74), M519_DATANUM(74), M519_DATANUM(74),
            M519_DATANUM(80), M519_DATANUM(81), M519_DATANUM(82), M519_DATANUM(83), M519_DATANUM(84), M519_DATANUM(85), M519_DATANUM(86),
            M519_DATANUM(87), M519_DATANUM(88), M519_DATANUM(91), M519_DATANUM(92) },
        { M519_DATANUM(78), M519_DATANUM(73), M519_DATANUM(75), M519_DATANUM(74), M519_DATANUM(74), M519_DATANUM(74), M519_DATANUM(74),
            M519_DATANUM(80), M519_DATANUM(81), M519_DATANUM(82), M519_DATANUM(83), M519_DATANUM(84), M519_DATANUM(85), M519_DATANUM(86),
            M519_DATANUM(87), M519_DATANUM(88), M519_DATANUM(91), M519_DATANUM(92) },
        { M519_DATANUM(79), M519_DATANUM(73), M519_DATANUM(75), M519_DATANUM(74), M519_DATANUM(74), M519_DATANUM(74), M519_DATANUM(74),
            M519_DATANUM(80), M519_DATANUM(81), M519_DATANUM(82), M519_DATANUM(83), M519_DATANUM(84), M519_DATANUM(85), M519_DATANUM(86),
            M519_DATANUM(87), M519_DATANUM(88), M519_DATANUM(91), M519_DATANUM(92) },
    };
    static const HuVec2f ofsTbl[4][18] = {
        {
            { 0.0f, 0.0f }, { -84.0f, -16.0f }, { -32.0f, 0.0f }, { -84.0f, -16.0f }, { -84.0f, -16.0f }, { -84.0f, -16.0f }, { -84.0f, -16.0f },
            { 111.0f, -3.0f }, { 111.0f, -3.0f }, { 111.0f, -3.0f }, { 129.0f, -22.0f }, { 129.0f, -22.0f }, { 129.0f, -22.0f },
            { 148.5f, -41.5f }, { 148.5f, -41.5f }, { 148.5f, -41.5f }, { 24.0f, -28.0f }, { 24.0f, -28.0f },
        },
        {
            { 0.0f, 0.0f }, { -88.0f, -40.0f }, { -14.0f, 78.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f },
            { -24.0f, -15.0f }, { -24.0f, -15.0f }, { -24.0f, -15.0f }, { -4.0f, -35.0f }, { -4.0f, -35.0f }, { -4.0f, -35.0f },
            { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 8.0f, -31.0f }, { 8.0f, -31.0f },
        },
        {
            { 0.0f, 0.0f }, { -88.0f, -40.0f }, { -14.0f, 61.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f },
            { -24.0f, -33.0f }, { -24.0f, -33.0f }, { -24.0f, -33.0f }, { -4.0f, -52.0f }, { -4.0f, -52.0f }, { -4.0f, -52.0f },
            { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 8.0f, -48.0f }, { 8.0f, -48.0f },
        },
        {
            { 0.0f, 0.0f }, { -88.0f, -40.0f }, { -14.0f, 42.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f },
            { -24.0f, -49.5f }, { -24.0f, -49.5f }, { -24.0f, -49.5f }, { -4.0f, -69.0f }, { -4.0f, -69.0f }, { -4.0f, -69.0f },
            { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 8.0f, -64.0f }, { 8.0f, -64.0f },
        },
    };
    M519_HUD *hud;
    int i;

    hud = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M519_HUD), HU_MEMNUM_OVL);
    hud->type = type;
    hud->unk6 = 0;
    hud->mechNo = mechNo;
    hud->mode = 0;
    hud->prevMode = 0;
    for (i = 0; i < 18; i++) {
        if (sprTbl[type][i] != -1) {
            hud->sprId[i] = espEntry(sprTbl[type][i], 0, 0);
            espDispOff(hud->sprId[i]);
        }
        else {
            hud->sprId[i] = -1;
        }
    }
    switch (hud->type) {
        case 0:
            espPriSet(hud->sprId[0], 1000);
            espPriSet(hud->sprId[1], 900);
            espPriSet(hud->sprId[2], 900);
            espPriSet(hud->sprId[3], 950);
            espPriSet(hud->sprId[4], 960);
            espPriSet(hud->sprId[5], 970);
            espPriSet(hud->sprId[6], 980);
            espPriSet(hud->sprId[7], 900);
            espPriSet(hud->sprId[8], 910);
            espPriSet(hud->sprId[9], 920);
            espPriSet(hud->sprId[10], 900);
            espPriSet(hud->sprId[11], 910);
            espPriSet(hud->sprId[12], 920);
            espPriSet(hud->sprId[13], 900);
            espPriSet(hud->sprId[14], 910);
            espPriSet(hud->sprId[15], 920);
            espPriSet(hud->sprId[16], 900);
            espPriSet(hud->sprId[17], 900);
            espScaleSet(hud->sprId[7], 0.6f, 0.6f);
            espScaleSet(hud->sprId[8], 0.6f, 0.6f);
            espScaleSet(hud->sprId[9], 0.6f, 0.6f);
            espScaleSet(hud->sprId[10], 0.6f, 0.6f);
            espScaleSet(hud->sprId[11], 0.6f, 0.6f);
            espScaleSet(hud->sprId[12], 0.6f, 0.6f);
            espScaleSet(hud->sprId[13], 0.6f, 0.6f);
            espScaleSet(hud->sprId[14], 0.6f, 0.6f);
            espScaleSet(hud->sprId[15], 0.6f, 0.6f);
            break;
        case 1:
        case 2:
        case 3:
            espPriSet(hud->sprId[0], 1000);
            espPriSet(hud->sprId[1], 900);
            espPriSet(hud->sprId[2], 900);
            espPriSet(hud->sprId[3], 950);
            espPriSet(hud->sprId[4], 960);
            espPriSet(hud->sprId[5], 970);
            espPriSet(hud->sprId[6], 980);
            espPriSet(hud->sprId[7], 900);
            espPriSet(hud->sprId[8], 910);
            espPriSet(hud->sprId[9], 920);
            espPriSet(hud->sprId[10], 900);
            espPriSet(hud->sprId[11], 910);
            espPriSet(hud->sprId[12], 920);
            espPriSet(hud->sprId[16], 900);
            espPriSet(hud->sprId[17], 900);
            espScaleSet(hud->sprId[7], 0.6f, 0.6f);
            espScaleSet(hud->sprId[8], 0.6f, 0.6f);
            espScaleSet(hud->sprId[9], 0.6f, 0.6f);
            espScaleSet(hud->sprId[10], 0.35f, 0.35f);
            espScaleSet(hud->sprId[11], 0.35f, 0.35f);
            espScaleSet(hud->sprId[12], 0.35f, 0.35f);
            espScaleSet(hud->sprId[16], 0.58f, 0.58f);
            espScaleSet(hud->sprId[17], 0.68f, 0.68f);
            break;
    }
    for (i = 0; i < 18; i++) {
        hud->ofs[i] = ofsTbl[type][i];
    }
    hud->process = HuPrcChildCreate(fn_1_D610, 4096, 6144, 0, HuPrcCurrentGet());
    hud->process->property = hud;
    return hud;
}

void fn_1_D610(void)
{
    M519_HUD *hud = HuPrcCurrentGet()->property;

    while (TRUE) {
        lbl_1_data_7C[hud->type][hud->mode](hud);
    }
}

void fn_1_D65C(M519_HUD *hud, HuVec2f pos)
{
    int i;

    hud->pos = pos;
    for (i = 0; i < 18; i++) {
        if (hud->sprId[i] != -1) {
            espPosSet(hud->sprId[i], pos.x + hud->ofs[i].x, pos.y + hud->ofs[i].y);
        }
    }
}

HuVec2f fn_1_D708(M519_HUD *hud)
{
    return hud->pos;
}

void fn_1_D714(M519_HUD *hud)
{
    hud->mode = 0;
}

void fn_1_D720(M519_HUD *hud)
{
    while (TRUE) {
        M519_SLEEP_MODECHG(M519_HUD, 1);
    }
}

void fn_1_D7B0(M519_HUD *hud)
{
    hud->mode = 1;
}

void fn_1_D7BC(M519_HUD *hud)
{
    M519_MECH *mech;
    int i;
    int j;
    float t;
    float s;
    HuVec2f pos;
    HuVec2f newPos;

    mech = lbl_1_bss_F8[hud->mechNo];
    pos = hud->pos;
    espDispOn(hud->sprId[0]);
    for (i = 1; i <= 60; i++) {
        t = (1.0f / 60.0f) * i;
        newPos.x = pos.x;
        newPos.y = pos.y - (100.0 * HuSin(90.0f * t));
        fn_1_D65C(hud, newPos);
        M519_SLEEP_MODECHG(M519_HUD, 1);
    }
    pos.y -= 100.0f;
    fn_1_D65C(hud, pos);
    espDispOn(hud->sprId[2]);
    espDispOn(hud->sprId[1]);
    espDispOn(hud->sprId[3]);
    espDispOn(hud->sprId[4]);
    espDispOn(hud->sprId[5]);
    espDispOn(hud->sprId[6]);
    espDispOn(hud->sprId[7]);
    espDispOn(hud->sprId[8]);
    espDispOn(hud->sprId[9]);
    espDispOn(hud->sprId[10]);
    espDispOn(hud->sprId[11]);
    espDispOn(hud->sprId[12]);
    espDispOn(hud->sprId[13]);
    espDispOn(hud->sprId[14]);
    espDispOn(hud->sprId[15]);
    espDispOn(hud->sprId[16]);
    fn_1_E298(hud);
    fn_1_E2D4(hud);
    fn_1_E714(hud);
    for (j = 1; j <= 100; j++) {
        t = 0.01f * j;
        s = HuSin(90.0f * t);
        espScaleSet(hud->sprId[2], 108.0f * s, 0.8f);
        espTPLvlSet(hud->sprId[2], 0.8f * s);
        espTPLvlSet(hud->sprId[7], s);
        espTPLvlSet(hud->sprId[8], 0.0f);
        espTPLvlSet(hud->sprId[9], 0.0f);
        espTPLvlSet(hud->sprId[10], s);
        espTPLvlSet(hud->sprId[11], 0.0f);
        espTPLvlSet(hud->sprId[12], 0.0f);
        espTPLvlSet(hud->sprId[13], s);
        espTPLvlSet(hud->sprId[14], 0.0f);
        espTPLvlSet(hud->sprId[15], 0.0f);
        espTPLvlSet(hud->sprId[16], 0.2f * s);
        espZRotSet(hud->sprId[1], 720.0f * s);
        espTPLvlSet(hud->sprId[1], 0.5f * s);
        for (i = 0; i < 4; i++) {
            espTPLvlSet(hud->sprId[i + 3], s);
        }
        M519_SLEEP_MODECHG(M519_HUD, 1);
    }
    fn_1_E168(hud);
}

void fn_1_DD08(M519_HUD *hud)
{
    M519_MECH *mech;
    int i;
    float t;
    float s;
    HuVec2f pos;
    HuVec2f newPos;

    mech = lbl_1_bss_F8[hud->mechNo];
    pos = hud->pos;
    espDispOn(hud->sprId[0]);
    for (i = 1; i <= 60; i++) {
        t = (1.0f / 60.0f) * i;
        newPos.x = pos.x - (96.0 * HuSin(90.0f * t));
        newPos.y = pos.y;
        fn_1_D65C(hud, newPos);
        M519_SLEEP_MODECHG(M519_HUD, 1);
    }
    pos.x -= 96.0f;
    fn_1_D65C(hud, pos);
    espDispOn(hud->sprId[7]);
    espDispOn(hud->sprId[8]);
    espDispOn(hud->sprId[9]);
    espDispOn(hud->sprId[2]);
    espDispOn(hud->sprId[16]);
    for (i = 1; i <= 100; i++) {
        t = 0.01f * i;
        s = HuSin(90.0f * t);
        espTPLvlSet(hud->sprId[7], s);
        espTPLvlSet(hud->sprId[8], 0.0f);
        espTPLvlSet(hud->sprId[9], 0.0f);
        espTPLvlSet(hud->sprId[10], s);
        espTPLvlSet(hud->sprId[11], 0.0f);
        espTPLvlSet(hud->sprId[12], 0.0f);
        espTPLvlSet(hud->sprId[16], 0.2f * s);
        espZRotSet(hud->sprId[2], -90.0f);
        espScaleSet(hud->sprId[2], 72.0f * s, 0.8f);
        espTPLvlSet(hud->sprId[2], 0.8f * s);
        M519_SLEEP_MODECHG(M519_HUD, 1);
    }
    fn_1_E168(hud);
}

void fn_1_E168(M519_HUD *hud)
{
    hud->mode = 2;
}

void fn_1_E174(M519_HUD *hud)
{
    while (TRUE) {
        fn_1_E298(hud);
        fn_1_E2D4(hud);
        fn_1_E714(hud);
        fn_1_EA10(hud);
        fn_1_D65C(hud, hud->pos);
        M519_SLEEP_MODECHG(M519_HUD, 1);
    }
}

void fn_1_E298(M519_HUD *hud)
{
    espZRotSet(hud->sprId[1], 0.0f);
}

void fn_1_E2D4(M519_HUD *hud)
{
    HuVecF radarCircle;
    HuVecF blipCircle;
    HuVecF selfRot;
    HuVecF otherRot;
    HuVecF selfPos;
    HuVecF otherPos;
    HuVec2f blip;
    HuVec2f dir;
    int i;
    s16 sprId;
    s32 no;
    float angle;
    float dist;

    radarCircle.x = 0.0f;
    radarCircle.y = 0.0f;
    radarCircle.z = 30.0f;
    selfPos = fn_1_3274(lbl_1_bss_F8[hud->mechNo]);
    selfRot = fn_1_34FC(lbl_1_bss_F8[hud->mechNo]);
    for (i = 0; i < 4; i++) {
        if (i != hud->mechNo) {
            otherPos = fn_1_3274(lbl_1_bss_F8[i]);
            otherRot = fn_1_34FC(lbl_1_bss_F8[i]);
            dir.x = otherPos.x - selfPos.x;
            dir.y = otherPos.z - selfPos.z;
            angle = 180.0 + HuAtan(dir.x, dir.y);
            dist = HuMagVec2F(&dir);
            blip.x = (0.015f * dist) * HuSin(angle - selfRot.y);
            blip.y = (0.015f * dist) * HuCos(angle - selfRot.y);
            blipCircle.x = blip.x;
            blipCircle.y = -blip.y;
            blipCircle.z = 4.0f;
            blip.x -= 84.0f;
            blip.y -= 16.0f;
            no = i + 3;
            hud->ofs[no] = blip;
            sprId = hud->sprId[no];
            if (!fn_1_A1D4(lbl_1_bss_F8[i]) && fn_1_F860(&radarCircle, &blipCircle, FALSE)) {
                espZRotSet(sprId, selfRot.y - otherRot.y);
                espDispOn(sprId);
            }
            else {
                espDispOff(sprId);
            }
        }
    }
}

void fn_1_E714(M519_HUD *hud)
{
    M519_MECH *mech;
    float speedMax;
    s16 knockback;

    mech = lbl_1_bss_F8[hud->mechNo];
    espScaleSet(hud->sprId[2], 108.0f * ((1.0f / mech->hpMax) * mech->hpDisp), 0.8f);
    if (mech->hpDisp > mech->hp) {
        mech->hpDisp--;
    }
    else if (mech->hpDisp < mech->hp) {
        mech->hpDisp++;
    }
    speedMax = 56.0f * HuAbs(mech->accel);
    espTPLvlSet(hud->sprId[14], (1.0f / speedMax) * mech->speed);
    espTPLvlSet(hud->sprId[15], (1.0f / speedMax) * mech->speed);
    espTPLvlSet(hud->sprId[8], 0.125f * mech->shotWait);
    espTPLvlSet(hud->sprId[9], 0.125f * mech->shotWait);
    knockback = (mech->knockback > 10.0f) ? 10.0f : mech->knockback;
    espTPLvlSet(hud->sprId[11], 0.1f * knockback);
    espTPLvlSet(hud->sprId[12], 0.1f * knockback);
}

void fn_1_EA10(M519_HUD *hud)
{
    M519_MECH *mech;

    mech = lbl_1_bss_F8[hud->mechNo];
    espDispOff(hud->sprId[16]);
    espDispOff(hud->sprId[17]);
    if (mech->chargeB > 0) {
        switch (fn_1_8DD0(mech)) {
            case 0:
                espDispOn(hud->sprId[17]);
                espTPLvlSet(hud->sprId[17], 0.2f + (0.008888889f * mech->chargeB));
                break;
            case 1:
                if ((mech->chargeB / 4) % 2 == 0) {
                    espDispOn(hud->sprId[17]);
                    espTPLvlSet(hud->sprId[17], 1.0f);
                }
                break;
            case 2:
                if ((mech->chargeB / 2) % 2 == 0) {
                    espDispOn(hud->sprId[17]);
                    espTPLvlSet(hud->sprId[17], 1.0f);
                }
                break;
        }
    }
    else {
        switch (fn_1_8DD0(mech)) {
            case 0:
                espDispOn(hud->sprId[16]);
                espTPLvlSet(hud->sprId[16], 0.2f + (0.008888889f * mech->chargeA));
                break;
            case 1:
                if ((mech->chargeA / 4) % 2 == 0) {
                    espDispOn(hud->sprId[16]);
                    espTPLvlSet(hud->sprId[16], 1.0f);
                }
                break;
            case 2:
                if ((mech->chargeA / 2) % 2 == 0) {
                    espDispOn(hud->sprId[16]);
                    espTPLvlSet(hud->sprId[16], 1.0f);
                }
                break;
        }
    }
}

void fn_1_ED78(M519_HUD *hud)
{
    while (TRUE) {
        fn_1_EE8C(hud);
        fn_1_F104(hud);
        fn_1_D65C(hud, hud->pos);
        M519_SLEEP_MODECHG(M519_HUD, 1);
    }
}

void fn_1_EE8C(M519_HUD *hud)
{
    M519_MECH *mech;
    s16 knockback;

    mech = lbl_1_bss_F8[hud->mechNo];
    espZRotSet(hud->sprId[2], -90.0f);
    espScaleSet(hud->sprId[2], 72.0f * ((1.0f / mech->hpMax) * mech->hpDisp), 0.8f);
    if (mech->hpDisp > mech->hp) {
        mech->hpDisp--;
    }
    else if (mech->hpDisp < mech->hp) {
        mech->hpDisp++;
    }
    espTPLvlSet(hud->sprId[8], 0.125f * mech->shotWait);
    espTPLvlSet(hud->sprId[9], 0.125f * mech->shotWait);
    knockback = (mech->knockback > 10.0f) ? 10.0f : mech->knockback;
    espTPLvlSet(hud->sprId[11], 0.1f * knockback);
    espTPLvlSet(hud->sprId[12], 0.1f * knockback);
}

void fn_1_F104(M519_HUD *hud)
{
    M519_MECH *mech;

    mech = lbl_1_bss_F8[hud->mechNo];
    espDispOff(hud->sprId[16]);
    espDispOff(hud->sprId[17]);
    switch (fn_1_8DD0(mech)) {
        case 0:
            espDispOn(hud->sprId[16]);
            espTPLvlSet(hud->sprId[16], 0.2f + (0.008888889f * mech->chargeB));
            break;
        case 1:
            if ((mech->chargeB / 4) % 2 == 0) {
                espDispOn(hud->sprId[16]);
                espTPLvlSet(hud->sprId[16], 1.0f);
            }
            break;
        case 2:
            if ((mech->chargeB / 2) % 2 == 0) {
                espDispOn(hud->sprId[17]);
                espTPLvlSet(hud->sprId[17], 1.0f);
            }
            break;
    }
}

void fn_1_F2DC(M519_HUD *hud)
{
    hud->mode = 3;
}

void fn_1_F2E8(M519_HUD *hud)
{
    M519_MECH *mech;
    int i;
    float t;
    HuVec2f pos;
    HuVec2f newPos;

    mech = lbl_1_bss_F8[hud->mechNo];
    pos = hud->pos;
    for (i = 1; i <= 120; i++) {
        t = (1.0f / 120.0f) * i;
        newPos.x = pos.x;
        newPos.y = pos.y + (100.0 * HuSin(90.0f * t));
        fn_1_D65C(hud, newPos);
        M519_SLEEP_MODECHG(M519_HUD, 1);
    }
    pos.y += 104.0f;
    fn_1_D65C(hud, pos);
    fn_1_D714(hud);
}

void fn_1_F568(M519_HUD *hud)
{
    M519_MECH *mech;
    int i;
    float t;
    HuVec2f pos;
    HuVec2f newPos;

    mech = lbl_1_bss_F8[hud->mechNo];
    pos = hud->pos;
    espDispOn(hud->sprId[0]);
    for (i = 1; i <= 120; i++) {
        t = (1.0f / 120.0f) * i;
        newPos.x = pos.x + (96.0 * HuSin(90.0f * t));
        newPos.y = pos.y;
        fn_1_D65C(hud, newPos);
        M519_SLEEP_MODECHG(M519_HUD, 1);
    }
    pos.x += 96.0f;
    fn_1_D65C(hud, pos);
    fn_1_D714(hud);
}

void fn_1_F7F0(M519_HUD *hud)
{
    int i;

    for (i = 0; i < 18; i++) {
        if (hud->sprId[i] != -1) {
            espDispOff(hud->sprId[i]);
        }
    }
    hud->mode = 0;
}

BOOL fn_1_F860(HuVecF *a, HuVecF *b, BOOL pushF)
{
    float dx = a->x - b->x;
    float dy = a->y - b->y;
    float radius = a->z + b->z;
    float dist = HuMagPoint2D(dx, dy);

    if (dist < radius) {
        if (dist == 0.0f) {
            dx = 0.0f;
            dy = -1.0f;
        }
        else {
            dx /= dist;
            dy /= dist;
        }
        if (pushF) {
            a->x = b->x + (dx * radius);
            a->y = b->y + (dy * radius);
        }
        return TRUE;
    }
    return FALSE;
}

BOOL fn_1_FA6C(HuVecF *a, HuVecF *b, BOOL pushF)
{
    float dx = a->x - b->x;
    float dy = a->y - b->y;
    float radius = b->z - a->z;
    float dist = HuMagPoint2D(dx, dy);

    if (dist > radius) {
        if (dist == 0.0f) {
            dx = 0.0f;
            dy = -1.0f;
        }
        else {
            dx /= dist;
            dy /= dist;
        }
        if (pushF) {
            a->x = b->x + (dx * radius);
            a->y = b->y + (dy * radius);
        }
        return TRUE;
    }
    return FALSE;
}

float fn_1_FC78(float start, float end, float t)
{
    float diff;
    float result;

    diff = fmod(end - start, 360.0);
    if (diff < 0.0f) {
        diff += 360.0f;
    }
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    result = fmod(start + (diff * t), 360.0);
    if (result < 0.0f) {
        result += 360.0f;
    }
    return result;
}

void fn_1_FD7C(s16 playerNo, s16 type, s16 time)
{
    switch (type) {
        case 0:
            omVibrate(playerNo, time, 6, 6);
            break;
        case 1:
            omVibrate(playerNo, time, 4, 2);
            break;
        case 2:
            omVibrate(playerNo, time, 12, 0);
            break;
    }
}

s16 fn_1_FE20(u16 cameraBit, HuVecF pos)
{
    HU3DCAMERA *camera;
    s16 vol;
    int i;
    OMCAMERAVIEW *view;
    s16 camVol;
    float angle;
    float dx;
    float dz;

    vol = 0;
    for (i = 0; i < 4; i++) {
        if (cameraBit & lbl_1_rodata_10[i]) {
            view = &lbl_1_bss_108[i];
            camera = &Hu3DCamera[i];
            angle = (180.0 + HuAtan(camera->pos.x - pos.x, camera->pos.z - pos.z)) - (180.0f + view->rot.y);
            if (angle < 0.0f) {
                angle = 360.0f + angle;
            }
            if (angle >= 360.0f) {
                angle -= 360.0f;
            }
            if (angle >= 180.0f) {
                angle -= 360.0f;
            }
            angle = -angle;
            if (HuAbs(angle) < 30.0f) {
                dx = HuAbs(camera->pos.x - pos.x);
                dz = HuAbs(camera->pos.z - pos.z);
                camVol = 127.0f - ((1.0f / 60.0f) * (4800.0f - HuMagPoint2D(dx, dz)));
                if (vol < camVol) {
                    vol = camVol;
                }
            }
        }
    }
    if (vol < 0) {
        vol = 0;
    }
    if (vol > 127) {
        vol = 127;
    }
    return vol;
}

#define M519_ABS(x) (((x) < 0.0f) ? -(x) : (x))

s16 fn_1_10244(u16 cameraBit, HuVecF pos)
{
    HU3DCAMERA *camera;
    s16 pan;
    int i;
    int num;
    OMCAMERAVIEW *view;
    float angle;
    float dx;
    float dz;

    pan = 0;
    num = 0;
    for (i = 0; i < 4; i++) {
        if (cameraBit & lbl_1_rodata_10[i]) {
            view = &lbl_1_bss_108[i];
            camera = &Hu3DCamera[i];
            angle = (180.0 + HuAtan(camera->pos.x - pos.x, camera->pos.z - pos.z)) - (180.0f + view->rot.y);
            if (angle < 0.0f) {
                angle = 360.0f + angle;
            }
            if (angle >= 360.0f) {
                angle -= 360.0f;
            }
            if (angle >= 180.0f) {
                angle -= 360.0f;
            }
            angle = -angle;
            if (M519_ABS(angle < 30.0f)) {
                dx = HuAbs(camera->pos.x - pos.x);
                dz = HuAbs(camera->pos.z - pos.z);
                if (i == 0) {
                    pan += 32.0f + (0.53333336f * (30.0f + angle));
                }
                else {
                    pan += 96.0f - (0.53333336f * (30.0f + angle));
                }
                num++;
            }
        }
    }
    pan = (num > 0) ? pan / num : 64;
    if (pan < 32) {
        pan = 32;
    }
    if (pan > 96) {
        pan = 96;
    }
    return pan;
}
