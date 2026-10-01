#include "game/audio.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/gamemes.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/process.h"

#include "game/mg/actman.h"
#include "game/mg/score.h"
#include "game/mg/seqman.h"

#include "humath.h"

typedef struct M572ModelPair_s {
    s16 unk0;
    s16 unk2;
} M572ModelPair;

typedef struct M572Vec3_s {
    f32 x;
    f32 y;
    f32 z;
} M572Vec3;

typedef struct M572Vec3Pair_s {
    M572Vec3 unk0;
    M572Vec3 unk4;
} M572Vec3Pair;

typedef struct M572ScoreEntry_s {
    s32 unk0;
    s16 unk4;
    s16 unk6;
} M572ScoreEntry;

typedef struct M572ProcList_s {
    s32 unk0;
    struct M572ProcList_s *unk4;
    struct M572ProcList_s *unk8;
} M572ProcList;

/* Heap object owned by the fn_1_85E4 / fn_1_619C pair (0xB0 bytes). */
typedef struct M572Item_s {
    /* 0x00 */ s16 model;
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ f32 posX;
    /* 0x08 */ f32 posY;
    /* 0x0C */ f32 posZ;
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ s32 playerIdx;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ f32 rotX;
    /* 0x20 */ f32 rotY;
    /* 0x24 */ f32 rotZ;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s32 state;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ M572Vec3 home;   /* 0x34 */
    /* 0x40 */ M572Vec3 vel;    /* 0x40 */
    /* 0x4C */ u8 pad4C[8];
    /* 0x54 */ s16 itemModel;
    /* 0x56 */ s16 hookL;
    /* 0x58 */ s16 hookR;
    /* 0x5A */ u8 pad5A[2];
    /* 0x5C */ s32 ownerIdx;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ u8 pad64[0x10];
    /* 0x74 */ union {
        f32 f;
        struct {
            u16 lo;
            u16 hi;
        } h;
    } unk74;
    /* 0x78 */ u8 pad78[4];
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ f32 unk80;
    /* 0x84 */ u8 pad84[0x14];
    /* 0x98 */ f32 unk98;
    /* 0x9C */ f32 unk9C;
    /* 0xA0 */ f32 unkA0;
    /* 0xA4 */ s16 unkA4;
    /* 0xA6 */ u8 padA6[2];
    /* 0xA8 */ s16 subModel0;
    /* 0xAA */ s16 subModel1;
    /* 0xAC */ s16 unkAC;
    /* 0xAE */ u8 padAE[2];
} M572Item; /* size = 0xB0 */

/* Owner passed to the minigame hooks: holds a M572Item * at 0x5C. */
typedef struct M572HookOwner_s {
    u8 pad0[0x5C];
    /* 0x5C */ M572Item *item;
} M572HookOwner;

/* Heap object owned by fn_1_99CC / fn_1_98BC / fn_1_95C8 (0x48 bytes). */
typedef struct M572Score_s {
    /* 0x00 */ s32 count;
    /* 0x04 */ s32 frame;
    /* 0x08 */ f32 posX;
    /* 0x0C */ f32 posY;
    /* 0x10 */ f32 posZ;
    /* 0x14 */ f32 rotX;
    /* 0x18 */ f32 rotY;
    /* 0x1C */ f32 rotZ;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 speedX;
    /* 0x3C */ f32 speedY;
    /* 0x40 */ f32 speedZ;
    /* 0x44 */ s32 slot;
} M572Score; /* size = 0x48 */

/* Prefix-compatible spawner object created by fn_1_9CC8 (0x18 bytes). */
typedef struct M572Spawner_s {
    /* 0x00 */ s32 count;
    /* 0x04 */ s32 frame;
    /* 0x08 */ s32 slot;
    /* 0x0C */ f32 posX;
    /* 0x10 */ f32 posY;
    /* 0x14 */ f32 posZ;
} M572Spawner; /* size = 0x18 */

/* Two-plane overlap test input (x used, z at +0x08). */
typedef struct M572PlanePt_s {
    f32 x;
    f32 pad4;
    f32 z;
} M572PlanePt;

struct _struct_lbl_1_bss_4D0_0x8 {
    /* 0x0 */ u32 unk0;                             /* inferred */
    /* 0x4 */ s16 unk4;                             /* inferred */
    /* 0x6 */ char pad6[2];
};                                                  /* size = 0x8 */

struct _struct_lbl_1_bss_5E4_0xC {
    /* 0x0 */ f32 unk0;                             /* inferred */
    /* 0x4 */ f32 unk4;                             /* inferred */
    /* 0x8 */ f32 unk8;                             /* inferred */
};                                                  /* size = 0xC */

struct _struct_lbl_1_bss_614_0xC {
    /* 0x0 */ f32 unk0;                             /* inferred */
    /* 0x4 */ f32 unk4;                             /* inferred */
    /* 0x8 */ f32 unk8;                             /* inferred */
};                                                  /* size = 0xC */

struct _struct_lbl_1_bss_644_0xC {
    /* 0x0 */ f32 unk0;                             /* inferred */
    /* 0x4 */ f32 unk4;                             /* inferred */
    /* 0x8 */ f32 unk8;                             /* inferred */
};                                                  /* size = 0xC */

struct _struct_lbl_1_bss_674_0xC {
    /* 0x0 */ f32 unk0;                             /* inferred */
    /* 0x4 */ f32 unk4;                             /* inferred */
    /* 0x8 */ f32 unk8;                             /* inferred */
};                                                  /* size = 0xC */

struct _struct_lbl_1_data_23C_0xC {
    /* 0x0 */ f32 unk0;                             /* inferred */
    /* 0x4 */ f32 unk4;                             /* inferred */
    /* 0x8 */ f32 unk8;                             /* inferred */
};                                                  /* size = 0xC */

struct _struct_lbl_1_data_2A0_0xC {
    /* 0x0 */ f32 unk0;                             /* inferred */
    /* 0x4 */ f32 unk4;                             /* inferred */
    /* 0x8 */ f32 unk8;                             /* inferred */
};                                                  /* size = 0xC */

struct _struct_lbl_1_data_2DC_0xC {
    /* 0x0 */ f32 unk0;                             /* inferred */
    /* 0x4 */ f32 unk4;                             /* inferred */
    /* 0x8 */ f32 unk8;                             /* inferred */
};                                                  /* size = 0xC */

struct _struct_lbl_1_data_2F4_0xC {
    /* 0x0 */ f32 unk0;                             /* inferred */
    /* 0x4 */ f32 unk4;                             /* inferred */
    /* 0x8 */ f32 unk8;                             /* inferred */
};                                                  /* size = 0xC */

struct _struct_lbl_1_data_30C_0xC {
    /* 0x0 */ f32 unk0;                             /* inferred */
    /* 0x4 */ f32 unk4;                             /* inferred */
    /* 0x8 */ f32 unk8;                             /* inferred */
};                                                  /* size = 0xC */

struct _struct_lbl_1_data_324_0xC {
    /* 0x0 */ f32 unk0;                             /* inferred */
    /* 0x4 */ f32 unk4;                             /* inferred */
    /* 0x8 */ f32 unk8;                             /* inferred */
};                                                  /* size = 0xC */

struct _struct_lbl_1_data_7C_0x8 {
    /* 0x0 */ f32 unk0;                             /* inferred */
    /* 0x4 */ f32 unk4;                             /* inferred */
};                                                  /* size = 0x8 */

struct _struct_lbl_1_data_9C_0xC {
    /* 0x0 */ f32 unk0;                             /* inferred */
    /* 0x4 */ f32 unk4;                             /* inferred */
    /* 0x8 */ f32 unk8;                             /* inferred */
};                                                  /* size = 0xC */

struct _struct_lbl_1_data_B4_0xC {
    /* 0x0 */ f32 unk0;                             /* inferred */
    /* 0x4 */ f32 unk4;                             /* inferred */
    /* 0x8 */ f32 unk8;                             /* inferred */
};                                                  /* size = 0xC */

f32 Hu3DMotionMaxTimeGet(s16);                      /* extern */
void ObjectSetup();                                 /* static */
void fn_1_1050(OMOBJ *arg0);                        /* static */
void fn_1_1500(OMOBJ *arg0);                        /* static */
s32 fn_1_1F94(OMOBJ *arg0, s32 arg1);               /* static */
void fn_1_20A8(void *arg0);                         /* static */
void fn_1_214C(OMOBJ *arg0);                        /* static */
void fn_1_2404(OMOBJ *arg0);             /* static */
void fn_1_2574(OMOBJ *arg0);             /* static */
void fn_1_26EC(OMOBJ *arg0);             /* static */
void fn_1_27FC(OMOBJ *arg0);             /* static */
void fn_1_2974(s16 arg_sp0, s16 frameNo);           /* static */
void fn_1_2AD8(s16 arg_sp0, s16 frameNo);           /* static */
void fn_1_2C9C(s16 mode, s16 frameNo);              /* static */
void fn_1_2CFC(s16 mode, s16 frameNo);              /* static */
void fn_1_2DF0(s16 arg_sp0, s16 frameNo);           /* static */
void fn_1_30FC(s16 arg_sp0, s16 frameNo);           /* static */
void fn_1_32A8(OMOBJ *arg0, s16 arg1, u32 arg2, f32 farg0); /* static */
void fn_1_3344(OMOBJ *arg0, s32 arg1);              /* static */
void fn_1_33A4();                                   /* static */
void fn_1_33A8(s32 *);                              /* static */
void fn_1_3454(u32 arg0, s32 arg1, f32 farg0, f32 farg1, f32 farg2); /* static */
void fn_1_3634(void);                              /* static */
void fn_1_370C(OMOBJ *arg0);                        /* static */
void fn_1_3954(OMOBJ *arg0);                        /* static */
void fn_1_3C9C(OMOBJ *arg0, s16 arg1, u32 arg2, f32 farg0); /* static */
s32 fn_1_3D28(OMOBJ *arg0, s32 arg1);               /* static */
s32 fn_1_45A4(s32 arg0, s32 arg1);                  /* static */
void fn_1_45E0();                                   /* static */
void fn_1_4C04(f32 *arg0, s32 arg1);                /* static */
void fn_1_710(OMOBJ *arg0);                         /* static */
void fn_1_85E4(OMOBJ *arg0);                        /* static */
s32 fn_1_89EC();                                    /* static */
void fn_1_8D6C(OMOBJ *arg0);                        /* static */
void fn_1_9B1C(OMOBJ *arg0);                        /* static */
void fn_1_9E4(OMOBJ *arg0);                         /* static */
void fn_1_C54(OMOBJ *arg0);                         /* static */
void fn_1_D94(OMOBJ *arg0);                         /* static */
typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];
static u32 lbl_1_data_0[0xD] = {
    0x960020,
    0x960021,
    0x960022,
    0x960023,
    0x960024,
    0x960026,
    0x960027,
    0x960028,
    0x960029,
    0x96002A,
    0x96002B,
    0x96002C,
    0x96002D,
};
static s32 lbl_1_data_34[0xF] = {
    0x7E0008,
    0x7E0009,
    0x7E000A,
    0x7E000D,
    0x7E000E,
    0x7E000F,
    0x7E0010,
    0x7E0011,
    0x7E0012,
    0x7E0013,
    0x7E0014,
    0x7E0015,
    0x7E0016,
    0x7E000B,
    0x7E000C,
};
static M572Vec3 lbl_1_data_70 = { 0.0f, 100.0f, -750.0f };
static struct _struct_lbl_1_data_7C_0x8 lbl_1_data_7C[4] = { { 71.0f, 60.0f }, { 191.0f, 60.0f }, { 385.0f, 60.0f }, { 505.0f, 60.0f } };
static struct _struct_lbl_1_data_9C_0xC lbl_1_data_9C[2] = { { -880.0f, 0.0f, -140.0f }, { 880.0f, 0.0f, -140.0f } };
static struct _struct_lbl_1_data_B4_0xC lbl_1_data_B4[2] = { { 0.0f, 90.0f, 0.0f }, { 0.0f, 270.0f, 0.0f } };
static f32 lbl_1_data_CC[0xD] = {
    -53.0f,
    0.0f,
    -140.0f,
    719.0f,
    0.0f,
    -140.0f,
    0.0f,
    10000.0f,
    10000.0f,
    0.0f,
    0.0f,
    0.0f,
    -1.0f,
};
static s8 lbl_1_data_100[0xD] = "a-itemhook-l";
static s8 lbl_1_data_10D[0xD] = "a-itemhook-r";
static s8 lbl_1_data_11A[0xA] = "itemhook1";
static s8 lbl_1_data_124[0xC] = { 0x69, 0x74, 0x65, 0x6D, 0x68, 0x6F, 0x6F, 0x6B, 0x32, 0, 0, 0 };
static u32 lbl_1_data_1A4[8] = {
    0x8C0000,
    0x8C0006,
    0x8C0031,
    0x8C0001,
    0x870099,
    0x87009A,
    0x8C0078,
    0x8C006E,
};
static s32 lbl_1_data_1C4[9] = {
    0x7F0003,
    0x7F0004,
    0x7F0005,
    0x7F0006,
    0x7F0007,
    0x7F0008,
    0x7F0009,
    0x7F000A,
    0x7F000B,
};
static OMCAMERAVIEW lbl_1_data_1E8 = { { 0.0f, 221.5f, -500.0f }, { 4.4f, 0.0f, 0.0f }, 526.0f };
static OMCAMERAVIEW lbl_1_data_204 = { { 0.0f, 350.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 1729.0f };
static OMCAMERAVIEW lbl_1_data_220 = { { 0.0f, 221.5f, -500.0f }, { 4.4f, 0.0f, 0.0f }, 1200.0f };
static struct _struct_lbl_1_data_23C_0xC lbl_1_data_23C[4] = {
    { -360.0f, 0.0f, 0.0f },
    { -120.0f, 0.0f, 0.0f },
    { 120.0f, 0.0f, 0.0f },
    { 360.0f, 0.0f, 0.0f },
};
static s32 lbl_1_data_26C[4] = { 1, 3, 5, 7 };
static f32 lbl_1_data_27C[9] = { -480.0f, -360.0f, -240.0f, -120.0f, 0.0f, 120.0f, 240.0f, 360.0f, 480.0f };
static struct _struct_lbl_1_data_2A0_0xC lbl_1_data_2A0[5] = {
    { -240.0f, 0.0f, -450.0f },
    { -80.36f, 0.0f, -450.0f },
    { 80.36f, 0.0f, -450.0f },
    { 240.66f, 0.0f, -450.0f },
    { 0.0f, 100.0f, -800.0f },
};
static struct _struct_lbl_1_data_2DC_0xC lbl_1_data_2DC[2] = { { 1000.0f, 1700.0f, -900.0f }, { 1100.0f, 1800.0f, -800.0f } };
static struct _struct_lbl_1_data_2F4_0xC lbl_1_data_2F4[2] = { { -1100.0f, 2700.0f, -500.0f }, { -1000.0f, 2900.0f, -400.0f } };
static struct _struct_lbl_1_data_30C_0xC lbl_1_data_30C[2] = { { -10.0f, 30.0f, 0.0f }, { -10.0f, 30.0f, 0.0f } };
static struct _struct_lbl_1_data_324_0xC lbl_1_data_324[2] = { { 60.0f, 30.0f, 0.0f }, { 60.0f, 30.0f, 0.0f } };
static M572Vec3 lbl_1_data_33C = { 0.0f, 10000.0f, 10000.0f };
static M572Vec3 lbl_1_data_348 = { 0.0f, 0.0f, 0.0f };
static s8 lbl_1_data_354[4] = { -1, -1, -1, -1 };
static M572Vec3 lbl_1_data_358 = { -100.0f, 8000.0f, 0.0f };
static M572Vec3 lbl_1_data_364 = { 0.0f, 1.0f, 0.0f };
static M572Vec3 lbl_1_data_370 = { 0.0f, 0.0f, 0.0f };
static MGSEQ_PARAM lbl_1_data_37C = {
    0xA,
    0,
    fn_1_2974,
    fn_1_2AD8,
    NULL,
    fn_1_2C9C,
    fn_1_2CFC,
    fn_1_2DF0,
    fn_1_30FC,
    NULL,
    NULL,
};
static s32 lbl_1_data_3A4[3][0xE1] = {
    {
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000001,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0xFFFFFFFF,
        0x00000001,
        0x00000000,
        0x00000001,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000005,
        0x00000001,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000005,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000005,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000005,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000001,
        0xFFFFFFFF,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
    },
    {
        0x00000001,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000005,
        0xFFFFFFFF,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000005,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000001,
        0x00000001,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000001,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000005,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000005,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000001,
        0x00000000,
        0x00000001,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000001,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
    },
    {
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0xFFFFFFFF,
        0x00000001,
        0xFFFFFFFF,
        0x00000000,
        0xFFFFFFFF,
        0x00000001,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000005,
        0x00000000,
        0x00000005,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000005,
        0x00000000,
        0x00000005,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000001,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000001,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0xFFFFFFFF,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0x00000001,
        0xFFFFFFFF,
        0x00000000,
        0xFFFFFFFF,
        0x00000001,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000001,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
    },
};
static s16 lbl_1_data_E30[9] = { 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60 };
static s8 lbl_1_data_E42[0x26] = {
    0x2A,
    0x2A,
    0x2A,
    0x2A,
    0x2A,
    0x2A,
    0x2A,
    0x20,
    0x4D,
    0x35,
    0x37,
    0x32,
    0x20,
    0x4F,
    0x62,
    0x6A,
    0x65,
    0x63,
    0x74,
    0x53,
    0x65,
    0x74,
    0x75,
    0x70,
    0x20,
    0x2A,
    0x2A,
    0x2A,
    0x2A,
    0x2A,
    0x2A,
    0x2A,
    0x2A,
    0x2A,
    0xA,
    0,
    0,
    0,
};
static s8 lbl_1_data_E8C[0x1E] = "%d COM HIT ppos.z:%f kind:%d\n";
static s8 lbl_1_data_EAA[0x21] = "%d BANANA HIT ppos.z:%f kind:%d\n";
static s8 lbl_1_data_ECB[0x1F] = "Center X:%.02f Y:%.02f Z:%.02f";
static s8 lbl_1_data_EEA[0x1D] = "CRot X:%.02f Y:%.02f Z:%.02f";
static s8 lbl_1_data_F07[0xC] = "CZoom:%.02f";
static s16 lbl_1_bss_4;
static s16 lbl_1_bss_6;
static s16 lbl_1_bss_8;
static s32 lbl_1_bss_C;
static M572ModelPair lbl_1_bss_10;
static s16 lbl_1_bss_14;
static OMOBJ *lbl_1_bss_18;
static OMOBJ *lbl_1_bss_1C[0xE1];
static OMOBJ **lbl_1_bss_3A0;
static OMOBJ **lbl_1_bss_3A4;
static OMOBJ **lbl_1_bss_3A8;
static s32 lbl_1_bss_3B0;
static s32 lbl_1_bss_3B4;
static s16 lbl_1_bss_3B8;
static OMOBJ *lbl_1_bss_3BC[0x14];
static MGSCORE *lbl_1_bss_40C[4];
static s16 lbl_1_bss_41C[4];
static s16 lbl_1_bss_424[4];
static s16 lbl_1_bss_42C[4];
static s16 lbl_1_bss_434[4];
static f32 lbl_1_bss_43C[4];
static s32 lbl_1_bss_44C[4];
static s32 lbl_1_bss_45C[4];
static s32 lbl_1_bss_46C[4];
static s32 lbl_1_bss_47C[4];
static s32 lbl_1_bss_48C;
static s32 lbl_1_bss_490[4];
static s16 lbl_1_bss_4A0[0x10];
static s32 lbl_1_bss_4C0;
static OMOBJ *lbl_1_bss_4C4;
static OMOBJ **lbl_1_bss_4CC;
static struct _struct_lbl_1_bss_4D0_0x8 lbl_1_bss_4D0[0x20];
static s32 lbl_1_bss_5D4;
static s32 lbl_1_bss_5D8;
static struct _struct_lbl_1_bss_5E4_0xC lbl_1_bss_5E4[4];
static struct _struct_lbl_1_bss_614_0xC lbl_1_bss_614[4];
static struct _struct_lbl_1_bss_644_0xC lbl_1_bss_644[4];
static struct _struct_lbl_1_bss_674_0xC lbl_1_bss_674[4];
static M572Vec3 lbl_1_bss_6A4;
static M572Vec3 lbl_1_bss_6B0;
static M572Vec3 lbl_1_bss_6BC;
static M572Vec3 lbl_1_bss_6C8;
static HUPROCESS *lbl_1_bss_6D4;
static M572ProcList lbl_1_bss_6D8;

s32 _prolog(void) {
    VoidFunc (*var_r31)();

    var_r31 = &_ctors;
loop_2:
    if (((VoidFunc) *var_r31) != NULL) {
        (*var_r31)();
        var_r31 += 4;
        goto loop_2;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void) {
    VoidFunc (*var_r31)();

    var_r31 = &_dtors;
loop_2:
    if (((VoidFunc) *var_r31) != NULL) {
        (*var_r31)();
        var_r31 += 4;
        goto loop_2;
    }
}

void ObjectSetup(void) {
    OMOBJ *temp_r3;
    OMOBJ *temp_r3_2;
    OMOBJ *temp_r3_3;
    OMOBJ *temp_r3_4;
    s32 var_r29;
    s32 var_r30;
    s32 var_r30_2;
    s32 var_r30_3;

    OSReport(lbl_1_data_E42);
    lbl_1_bss_6D4 = omInitObjMan(0xC8, 0x2000);
    omGameSysInit(lbl_1_bss_6D4);
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 30.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    omCameraViewSet(&lbl_1_data_1E8);
    lbl_1_bss_18 = omAddObjEx(lbl_1_bss_6D4, 0x7FDA, 0U, 0U, -1, omOutView);
    lbl_1_bss_18->work[0] = 2;
    lbl_1_bss_14 = Hu3DGLightCreate(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0U, 0U, 0U);
    Hu3DGLightPosAimSetV(lbl_1_bss_14, &lbl_1_data_33C, &lbl_1_data_348);
    Hu3DGLightStaticSet(lbl_1_bss_14, 1);
    Hu3DGLightInfinitytSet(lbl_1_bss_14);
    Hu3DGLightColorSet(lbl_1_bss_14, lbl_1_data_354[0], lbl_1_data_354[1], lbl_1_data_354[2], lbl_1_data_354[3]);
    Hu3DShadowCreate(30.0f, 10.0f, 10000.0f);
    Hu3DShadowColSet(0x10U, 0x10U, 0x10U);
    Hu3DShadowPosSet(&lbl_1_data_358, &lbl_1_data_364, &lbl_1_data_370);
    Hu3DShadowTPLvlSet(0.6f);
    omMakeGroupEx(lbl_1_bss_6D4, 0U, 4U);
    lbl_1_bss_3A8 = omGetGroupMemberListEx(lbl_1_bss_6D4, 0);
    var_r30 = 0;
loop_2:
    if (var_r30 < 4) {
        temp_r3 = omAddObjEx(lbl_1_bss_6D4, 0x64, 1U, 8U, 0, fn_1_D94);
        lbl_1_bss_3A8[var_r30] = temp_r3;
        temp_r3->data = HuMemDirectMallocNum(HEAP_HEAP, 0x80, 0x10000000U);
        temp_r3->work[0] = (u32) var_r30;
        var_r30 += 1;
        goto loop_2;
    }
    omMakeGroupEx(lbl_1_bss_6D4, 3U, 1U);
    lbl_1_bss_4CC = omGetGroupMemberListEx(lbl_1_bss_6D4, 3);
    temp_r3_2 = omAddObjEx(lbl_1_bss_6D4, 0x68, 0U, 0U, 3, fn_1_85E4);
    *lbl_1_bss_4CC = temp_r3_2;
    temp_r3_2->data = HuMemDirectMallocNum(HEAP_HEAP, 0xB0, 0x10000000U);
    temp_r3_2->work[0] = 1;
    omMakeGroupEx(lbl_1_bss_6D4, 2U, 9U);
    lbl_1_bss_3A0 = omGetGroupMemberListEx(lbl_1_bss_6D4, 2);
    var_r30_2 = 0;
loop_5:
    if (var_r30_2 < 9) {
        temp_r3_3 = omAddObjEx(lbl_1_bss_6D4, 0x66, 2U, 0U, 2, fn_1_C54);
        lbl_1_bss_3A0[var_r30_2] = temp_r3_3;
        temp_r3_3->data = HuMemDirectMallocNum(HEAP_HEAP, 0x2C, 0x10000000U);
        temp_r3_3->work[0] = (u32) var_r30_2;
        var_r30_2 += 1;
        goto loop_5;
    }
    lbl_1_bss_C = 0;
    omMakeGroupEx(lbl_1_bss_6D4, 1U, 0xE1U);
    lbl_1_bss_8 = Hu3DModelCreate(HuDataSelHeapReadNum(0x7F0010, 0x10000000, HEAP_MODEL));
    lbl_1_bss_6 = Hu3DModelCreate(HuDataSelHeapReadNum(0x7F0012, 0x10000000, HEAP_MODEL));
    lbl_1_bss_4 = Hu3DModelCreate(HuDataSelHeapReadNum(0x7F000D, 0x10000000, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_8, 1U);
    Hu3DModelAttrSet(lbl_1_bss_6, 1U);
    Hu3DModelAttrSet(lbl_1_bss_4, 1U);
    omMakeGroupEx(lbl_1_bss_6D4, 4U, 2U);
    lbl_1_bss_3A4 = omGetGroupMemberListEx(lbl_1_bss_6D4, 4);
    var_r30_3 = 0;
loop_8:
    if (var_r30_3 < 2) {
        temp_r3_4 = omAddObjEx(lbl_1_bss_6D4, 0x69, 1U, 1U, 4, fn_1_710);
        lbl_1_bss_3A4[var_r30_3] = temp_r3_4;
        temp_r3_4->data = HuMemDirectMallocNum(HEAP_HEAP, 0x34, 0x10000000U);
        temp_r3_4->work[0] = (u32) var_r30_3;
        var_r30_3 += 1;
        goto loop_8;
    }
    lbl_1_bss_3B8 = Hu3DModelCreate(HuDataSelHeapReadNum(0x7F0011, 0x10000000, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_3B8, 1U);
    var_r29 = 0;
loop_11:
    if (var_r29 < 0x14) {
        lbl_1_bss_3BC[var_r29] = NULL;
        var_r29 += 1;
        goto loop_11;
    }
    lbl_1_bss_3B0 = -1;
    fn_1_33A8(&lbl_1_bss_3B0);
    MgSeqCreatePrio(&lbl_1_data_37C, 0x3E8);
}

void fn_1_710(OMOBJ *arg0) {
    void *temp_r31;

    temp_r31 = arg0->data;
    temp_r31->unk0 = (u32) arg0->work[0];
    *arg0->mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(0x7F0013, 0x10000000, HEAP_MODEL));
    Hu3DModelAttrSet(*arg0->mdlId, 0x40000001U);
    temp_r31->unk4 = (f32) lbl_1_data_2DC[temp_r31->unk0].unk0;
    temp_r31->unk8 = (f32) lbl_1_data_2DC[temp_r31->unk0].unk4;
    temp_r31->unkC = (f32) lbl_1_data_2DC[temp_r31->unk0].unk8;
    temp_r31->unk1C = (f32) lbl_1_data_30C[temp_r31->unk0].unk0;
    temp_r31->unk20 = (f32) lbl_1_data_30C[temp_r31->unk0].unk4;
    temp_r31->unk24 = (f32) lbl_1_data_30C[temp_r31->unk0].unk8;
    temp_r31->unk10 = (f32) ((lbl_1_data_2F4[temp_r31->unk0].unk0 - lbl_1_data_2DC[temp_r31->unk0].unk0) / 200.0f);
    temp_r31->unk14 = (f32) ((lbl_1_data_2F4[temp_r31->unk0].unk4 - lbl_1_data_2DC[temp_r31->unk0].unk4) / 200.0f);
    temp_r31->unk18 = (f32) ((lbl_1_data_2F4[temp_r31->unk0].unk8 - lbl_1_data_2DC[temp_r31->unk0].unk8) / 200.0f);
    temp_r31->unk28 = (f32) ((lbl_1_data_324[temp_r31->unk0].unk0 - lbl_1_data_30C[temp_r31->unk0].unk0) / 200.0f);
    temp_r31->unk2C = (f32) ((lbl_1_data_324[temp_r31->unk0].unk4 - lbl_1_data_30C[temp_r31->unk0].unk4) / 200.0f);
    temp_r31->unk30 = (f32) ((lbl_1_data_324[temp_r31->unk0].unk8 - lbl_1_data_30C[temp_r31->unk0].unk8) / 200.0f);
    arg0->objFunc = fn_1_9E4;
}

void fn_1_9E4(OMOBJ *arg0) {
    void *temp_r31;

    temp_r31 = arg0->data;
    if ((MgSeqModeGet() == 5) && (MgSeqFrameNoGet() >= 0x190U) && (MgSeqFrameNoGet() <= 0x258U)) {
        temp_r31->unk4 = (f32) (temp_r31->unk4 + temp_r31->unk10);
        temp_r31->unk8 = (f32) (temp_r31->unk8 + temp_r31->unk14);
        temp_r31->unkC = (f32) (temp_r31->unkC + temp_r31->unk18);
        temp_r31->unk1C = (f32) (temp_r31->unk1C + temp_r31->unk28);
        temp_r31->unk20 = (f32) (temp_r31->unk20 + temp_r31->unk2C);
        temp_r31->unk24 = (f32) (temp_r31->unk24 + temp_r31->unk30);
    }
    omSetTra(arg0, temp_r31->unk4, temp_r31->unk8, temp_r31->unkC);
    omSetRot(arg0, temp_r31->unk1C, temp_r31->unk20, temp_r31->unk24);
}

void fn_1_AC4(OMOBJ *arg0) {
    s32 temp_r0;
    void *temp_r31;

    temp_r31 = arg0->data;
    temp_r0 = temp_r31->unk1C;
    if (temp_r0 != 1) {
        if (temp_r0 < 1) {

        }
    } else if (temp_r31->unk20 > 45.0f) {
        temp_r31->unk1C = 0;
        temp_r31->unk20 = 0;
        temp_r31->unk8 = 0.0f;
    } else {
        temp_r31->unk20 = (s32) (temp_r31->unk20 + 1);
        temp_r31->unk24 = (f32) (temp_r31->unk24 - 2.0f);
        if (temp_r31->unk24 < 0.0f) {
            temp_r31->unk1C = 0;
            temp_r31->unk20 = 0;
            temp_r31->unk8 = 0.0f;
        }
        temp_r31->unk8 = (f32) (45.0 + (700.0 * sin((3.141592653589793 * (f64) temp_r31->unk24) / 180.0)));
    }
    Hu3DModelPosSet(temp_r31->unk0, temp_r31->unk4, temp_r31->unk8, temp_r31->unkC);
    Hu3DModelRotSet(temp_r31->unk0, temp_r31->unk10, temp_r31->unk14, temp_r31->unk18);
}

void fn_1_C54(OMOBJ *arg0) {
    void *temp_r31;

    temp_r31 = arg0->data;
    temp_r31->unk28 = (u32) arg0->work[0];
    temp_r31->unk0 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_1C4[temp_r31->unk28], 0x10000000, HEAP_MODEL));
    temp_r31->unk2 = Hu3DModelCreate(HuDataSelHeapReadNum(0x7F000C, 0x10000000, HEAP_MODEL));
    Hu3DModelPosSet(temp_r31->unk2, lbl_1_data_27C[temp_r31->unk28], 0.0f, 0.0f);
    Hu3DModelAttrSet(temp_r31->unk2, 0x40000002U);
    temp_r31->unk4 = 0.0f;
    temp_r31->unk8 = 745.0f;
    temp_r31->unkC = 0.0f;
    temp_r31->unk18 = 0.0f;
    temp_r31->unk14 = 0.0f;
    temp_r31->unk10 = 0.0f;
    temp_r31->unk1C = 0;
    temp_r31->unk20 = 0;
    temp_r31->unk24 = 90.0f;
    arg0->objFunc = fn_1_AC4;
}

void fn_1_D94(OMOBJ *arg0) {
    s32 var_r29;
    void *temp_r31;

    temp_r31 = arg0->data;
    temp_r31->unk14 = (u32) arg0->work[0];
    temp_r31->unk18 = (s32) GwPlayerConf[temp_r31->unk14].charNo;
    temp_r31->unk1C = (s32) GwPlayerConf[temp_r31->unk14].padNo;
    temp_r31->unk20 = 0;
    temp_r31->unk24 = 0;
    temp_r31->unk28 = 0;
    temp_r31->unk76 = 0;
    temp_r31->unk74 = 0;
    temp_r31->unk64 = 0;
    temp_r31->unk68 = 0;
    temp_r31->unk6C = 0;
    temp_r31->unk74 = 0;
    temp_r31->unk7C = -1;
    *arg0->mdlId = CharModelCreate((s16) temp_r31->unk18, 2);
    var_r29 = 0;
loop_2:
    if (var_r29 < 8) {
        arg0->mtnId[var_r29] = CharMotionCreate((s16) temp_r31->unk18, lbl_1_data_1A4[var_r29]);
        CharMotionSet((s16) temp_r31->unk18, arg0->mtnId[var_r29]);
        var_r29 += 1;
        goto loop_2;
    }
    temp_r31->unk0 = 0;
    Hu3DModelAttrSet(*arg0->mdlId, 1U);
    Hu3DModelAttrSet(*arg0->mdlId, 0x40000001U);
    Hu3DMotionSet(*arg0->mdlId, *arg0->mtnId);
    Hu3DModelShadowSet(*arg0->mdlId);
    temp_r31->unk12 = (s16) *arg0->mdlId;
    temp_r31->unk78 = (s32) GwPlayerConf[temp_r31->unk14].dif;
    temp_r31->unk58 = 0.0f;
    temp_r31->unk54 = 0.0f;
    temp_r31->unk50 = 0.0f;
    temp_r31->unk34 = (f32) lbl_1_data_23C[temp_r31->unk14].unk0;
    temp_r31->unk38 = (f32) lbl_1_data_23C[temp_r31->unk14].unk4;
    temp_r31->unk3C = (f32) lbl_1_data_23C[temp_r31->unk14].unk8;
    temp_r31->unk5C = (s32) lbl_1_data_26C[temp_r31->unk14];
    temp_r31->unk60 = -1;
    omSetTra(arg0, temp_r31->unk34, temp_r31->unk38, temp_r31->unk3C);
    omSetRot(arg0, temp_r31->unk50, temp_r31->unk54, temp_r31->unk58);
    Hu3DModelAttrReset(*arg0->mdlId, 1U);
    CharMotionDataClose((s16) temp_r31->unk18);
    arg0->objFunc = fn_1_1050;
}

void fn_1_1050(OMOBJ *arg0) {
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r3;
    void *temp_r31;

    temp_r31 = arg0->data;
    temp_r0 = temp_r31->unk24;
    switch (temp_r0) {                              /* switch 1; irregular */
    case 0:                                         /* switch 1 */
        break;
    case 1:                                         /* switch 1 */
        fn_1_32A8(lbl_1_bss_3A8[temp_r31->unk14], 3, 0x40000001U, 8.0f);
        temp_r31->unk54 = (f32) (temp_r31->unk54 + 6.0f);
        if (temp_r31->unk54 > 180.0f) {
            temp_r31->unk54 = 180.0f;
            temp_r31->unk24 = 2;
            fn_1_32A8(lbl_1_bss_3A8[temp_r31->unk14], 4, 0x40000001U, 8.0f);
        }
        break;
    case 2:                                         /* switch 1 */
        if (temp_r31->unk28 > 8.0f) {
            temp_r31->unk24 = 3;
            Hu3DMotionSpeedSet(temp_r31->unk12, 0.0f);
            temp_r31->unk28 = 0;
        } else {
            temp_r31->unk28 = (s32) (temp_r31->unk28 + 1);
            temp_r31->unk3C = (f32) (temp_r31->unk3C - 3.0f);
        }
        break;
    case 3:                                         /* switch 1 */
        if ((s16) GwPlayerConf[temp_r31->unk14].type != 0) {
            temp_r0_2 = temp_r31->unk78;
            switch (temp_r0_2) {                    /* switch 2; irregular */
            case 0:                                 /* switch 2 */
                fn_1_2404(arg0);
                break;
            case 1:                                 /* switch 2 */
                fn_1_2574(arg0);
                break;
            case 2:                                 /* switch 2 */
                fn_1_26EC(arg0);
                break;
            case 3:                                 /* switch 2 */
                fn_1_27FC(arg0);
                break;
            }
        } else {
            temp_r31->unk74 = (u16) HuPadBtnDown[temp_r31->unk1C];
            temp_r31->unk76 = (u16) HuPadBtn[temp_r31->unk1C];
        }
        fn_1_1500(arg0);
        temp_r3 = fn_1_3D28(arg0, 0);
        if (temp_r3 != -2) {
            if (fn_1_45A4(temp_r3, temp_r31->unk14) == -1) {
                temp_r31->unk24 = 4;
                omVibrate((s16) temp_r31->unk1C, 0xF, 0xC, 0);
                temp_r31->unk28 = 0;
                temp_r31->unk2C = 0.0f;
                temp_r31->unk4C = temp_r31->unk38;
                HuAudFXPlayPan(0x977, lbl_1_data_E30[temp_r31->unk5C]);
            } else {
                HuAudFXPlayPan(0x96C, lbl_1_data_E30[temp_r31->unk5C]);
            }
        }
        break;
    case 4:                                         /* switch 1 */
        if ((s32) temp_r31->unk28 > 0x19) {
            temp_r31->unk28 = 0;
            temp_r31->unk24 = 3;
            temp_r31->unk2C = 0.0f;
            Hu3DModelAttrReset(*arg0->mdlId, 1U);
            fn_1_32A8(lbl_1_bss_3A8[temp_r31->unk14], 4, 0x40000001U, 8.0f);
        } else {
            fn_1_32A8(lbl_1_bss_3A8[temp_r31->unk14], 7, 0x40000001U, 8.0f);
            temp_r31->unk28 = (s32) (temp_r31->unk28 + 1);
            temp_r31->unk38 = (f32) (temp_r31->unk38 - 3.4041665f);
            if ((s32) (temp_r31->unk28 & 1) != 0) {
                Hu3DModelAttrSet(*arg0->mdlId, 1U);
            } else {
                Hu3DModelAttrReset(*arg0->mdlId, 1U);
            }
        }
        break;
    }
    if ((MgSeqModeGet() == 6) && ((s32) temp_r31->unk64 != 3) && ((s32) temp_r31->unk64 != 4)) {
        Hu3DMotionSpeedSet(temp_r31->unk12, 0.0f);
    }
    fn_1_20A8(temp_r31 + 0x34);
    fn_1_214C(arg0);
    temp_r31->unk40 = temp_r31->unk34;
    temp_r31->unk44 = temp_r31->unk38;
    temp_r31->unk48 = temp_r31->unk3C;
    omSetTra(arg0, temp_r31->unk34, temp_r31->unk38, temp_r31->unk3C);
    omSetRot(arg0, temp_r31->unk50, temp_r31->unk54, temp_r31->unk58);
}

void fn_1_1500(OMOBJ *arg0) {
    u32 temp_r4;
    void *temp_r31;

    temp_r31 = arg0->data;
    temp_r4 = temp_r31->unk64;
    switch (temp_r4) {
    case 0:
        if (MgSeqModeGet() == 5) {
            if ((s32) (temp_r31->unk76 & 0x100) != 0) {
                temp_r31->unk38 = (f32) (temp_r31->unk38 + 6.45f);
                Hu3DModelAttrReset(temp_r31->unk12, 0x40000004U);
                Hu3DMotionSpeedSet(temp_r31->unk12, 3.0f);
                if ((s32) temp_r31->unk7C == -1) {
                    temp_r31->unk7C = HuAudFXPlayPan(temp_r31->unk14 + 0x96F, lbl_1_data_E30[temp_r31->unk5C]);
                } else if (HuAudFXStatusGet(temp_r31->unk7C) == 0) {
                    temp_r31->unk7C = -1;
                }
            }
            if ((s32) (temp_r31->unk76 & 0x200) != 0) {
                temp_r31->unk38 = (f32) (temp_r31->unk38 - 6.808333f);
                Hu3DModelAttrSet(temp_r31->unk12, 0x40000004U);
                Hu3DMotionSpeedSet(temp_r31->unk12, 3.0f);
            }
            if (((s32) (temp_r31->unk76 & 0x100) == 0) && ((s32) (temp_r31->unk76 & 0x200) == 0)) {
                Hu3DMotionSpeedSet(temp_r31->unk12, 0.0f);
            }
            if ((s32) (temp_r31->unk74 & 0x40) != 0) {
                temp_r31->unk68 = 0;
                if (fn_1_1F94(arg0, temp_r31->unk5C - 1) != 0) {
                    temp_r31->unk64 = 3U;
                    temp_r31->unk60 = (s32) (temp_r31->unk5C - 1);
                    fn_1_32A8(lbl_1_bss_3A8[temp_r31->unk14], 5, 0U, 8.0f);
                    HuAudFXPlayPan(temp_r31->unk14 + 0x973, lbl_1_data_E30[temp_r31->unk5C]);
                } else {
                    temp_r31->unk64 = 5U;
                }
            }
            if ((s32) (temp_r31->unk74 & 0x20) != 0) {
                temp_r31->unk68 = 0;
                if (fn_1_1F94(arg0, temp_r31->unk5C + 1) != 0) {
                    temp_r31->unk64 = 4U;
                    temp_r31->unk60 = (s32) (temp_r31->unk5C + 1);
                    fn_1_32A8(lbl_1_bss_3A8[temp_r31->unk14], 6, 0U, 8.0f);
                    HuAudFXPlayPan(temp_r31->unk14 + 0x973, lbl_1_data_E30[temp_r31->unk5C]);
                    return;
                }
                temp_r31->unk64 = 6U;
                return;
            }
        } else {
        default:
            return;
        }
        break;
    case 5:
        temp_r31->unk68 = (s32) (temp_r31->unk68 + 1);
        if (temp_r31->unk68 > 20.0f) {
            fn_1_3344(lbl_1_bss_3A8[temp_r31->unk14], 4);
            Hu3DModelAttrSet(temp_r31->unk12, 0x40000001U);
            Hu3DMotionSpeedSet(temp_r31->unk12, 0.0f);
            temp_r31->unk68 = 0;
            temp_r31->unk64 = 0U;
            temp_r31->unk34 = (f32) lbl_1_data_27C[temp_r31->unk5C];
            return;
        }
        if (temp_r31->unk68 < 10.0f) {
            fn_1_32A8(lbl_1_bss_3A8[temp_r31->unk14], 5, 0U, 8.0f);
            temp_r31->unk34 = (f32) (temp_r31->unk34 - 3.0f);
            return;
        }
        Hu3DModelAttrSet(temp_r31->unk12, 0x40000004U);
        temp_r31->unk34 = (f32) (temp_r31->unk34 + 3.0f);
        return;
    case 6:
        temp_r31->unk68 = (s32) (temp_r31->unk68 + 1);
        if (temp_r31->unk68 > 20.0f) {
            fn_1_3344(lbl_1_bss_3A8[temp_r31->unk14], 4);
            Hu3DModelAttrSet(temp_r31->unk12, 0x40000001U);
            Hu3DMotionSpeedSet(temp_r31->unk12, 0.0f);
            temp_r31->unk68 = 0;
            temp_r31->unk64 = 0U;
            temp_r31->unk34 = (f32) lbl_1_data_27C[temp_r31->unk5C];
            return;
        }
        if (temp_r31->unk68 < 10.0f) {
            fn_1_32A8(lbl_1_bss_3A8[temp_r31->unk14], 6, 0U, 8.0f);
            temp_r31->unk34 = (f32) (temp_r31->unk34 + 3.0f);
            return;
        }
        Hu3DModelAttrSet(temp_r31->unk12, 0x40000004U);
        temp_r31->unk34 = (f32) (temp_r31->unk34 - 3.0f);
        return;
    case 7:
        if ((s32) temp_r31->unk68 > (s32) temp_r31->unk6C) {
            fn_1_3344(lbl_1_bss_3A8[temp_r31->unk14], 4);
            Hu3DModelAttrSet(temp_r31->unk12, 0x40000001U);
            Hu3DMotionSpeedSet(temp_r31->unk12, 0.0f);
            temp_r31->unk68 = 0;
            temp_r31->unk64 = 0U;
            temp_r31->unk34 = (f32) lbl_1_data_27C[temp_r31->unk5C];
            return;
        }
        Hu3DModelAttrSet(temp_r31->unk12, 0x40000004U);
        temp_r31->unk34 = (f32) (temp_r31->unk34 + temp_r31->unk70);
        temp_r31->unk68 = (s32) (temp_r31->unk68 + 1);
        return;
    case 8:
        if ((s32) temp_r31->unk68 > (s32) temp_r31->unk6C) {
            fn_1_3344(lbl_1_bss_3A8[temp_r31->unk14], 4);
            Hu3DModelAttrSet(temp_r31->unk12, 0x40000001U);
            Hu3DMotionSpeedSet(temp_r31->unk12, 0.0f);
            temp_r31->unk68 = 0;
            temp_r31->unk64 = 0U;
            temp_r31->unk34 = (f32) lbl_1_data_27C[temp_r31->unk5C];
            return;
        }
        Hu3DModelAttrSet(temp_r31->unk12, 0x40000004U);
        temp_r31->unk34 = (f32) (temp_r31->unk34 - temp_r31->unk70);
        temp_r31->unk68 = (s32) (temp_r31->unk68 + 1);
        return;
    case 3:
        temp_r31->unk68 = (s32) (temp_r31->unk68 + 1);
        if (temp_r31->unk68 > 20.0f) {
            temp_r31->unk68 = 0;
            temp_r31->unk64 = 0U;
            temp_r31->unk5C = (s32) (temp_r31->unk5C - 1);
            temp_r31->unk60 = -1;
            fn_1_3344(lbl_1_bss_3A8[temp_r31->unk14], 4);
            Hu3DModelAttrSet(temp_r31->unk12, 0x40000001U);
            Hu3DMotionSpeedSet(temp_r31->unk12, 0.0f);
            if ((s32) temp_r31->unk5C < 0) {
                temp_r31->unk5C = 0;
            }
            temp_r31->unk34 = (f32) lbl_1_data_27C[temp_r31->unk5C];
            return;
        }
        if (fn_1_1F94(arg0, temp_r31->unk5C - 1) == 0) {
            temp_r31->unk6C = (s32) temp_r31->unk68;
            temp_r31->unk70 = (f32) (fabs(lbl_1_data_27C[temp_r31->unk5C] - temp_r31->unk34) / (f64) temp_r31->unk6C);
            temp_r31->unk68 = 0;
            temp_r31->unk64 = 7U;
            return;
        }
        temp_r31->unk34 = (f32) (temp_r31->unk34 - 6.0f);
        return;
    case 4:
        temp_r31->unk68 = (s32) (temp_r31->unk68 + 1);
        if (temp_r31->unk68 > 20.0f) {
            temp_r31->unk68 = 0;
            temp_r31->unk64 = 0U;
            temp_r31->unk5C = (s32) (temp_r31->unk5C + 1);
            temp_r31->unk60 = -1;
            fn_1_3344(lbl_1_bss_3A8[temp_r31->unk14], 4);
            Hu3DModelAttrSet(temp_r31->unk12, 0x40000001U);
            Hu3DMotionSpeedSet(temp_r31->unk12, 0.0f);
            if ((s32) temp_r31->unk5C >= 9) {
                temp_r31->unk5C = 8;
            }
            temp_r31->unk34 = (f32) lbl_1_data_27C[temp_r31->unk5C];
            return;
        }
        if (fn_1_1F94(arg0, temp_r31->unk5C + 1) == 0) {
            temp_r31->unk6C = (s32) temp_r31->unk68;
            temp_r31->unk70 = (f32) (fabs(temp_r31->unk34 - lbl_1_data_27C[temp_r31->unk5C]) / (f64) temp_r31->unk6C);
            temp_r31->unk68 = 0;
            temp_r31->unk64 = 8U;
            return;
        }
        temp_r31->unk34 = (f32) (temp_r31->unk34 + 6.0f);
        break;
    }
}

s32 fn_1_1F94(OMOBJ *arg0, s32 arg1) {
    s32 var_r30;
    void *temp_r29;
    void *temp_r31;

    temp_r29 = arg0->data;
    if (arg1 < 0) {
        return 0;
    }
    if (arg1 >= 9) {
        return 0;
    }
    var_r30 = 0;
loop_11:
    if (var_r30 >= 4) {
        return 1;
    }
    if ((var_r30 != (s32) temp_r29->unk14) && ((temp_r31 = lbl_1_bss_3A8[var_r30]->data, ((arg1 == (s32) temp_r31->unk5C) != 0)) || (arg1 == (s32) temp_r31->unk60))) {
        if (fabsf(temp_r29->unk38 - temp_r31->unk38) <= 150.0f) {
            return 0;
        }
    }
    var_r30 += 1;
    goto loop_11;
}

void fn_1_20A8(void *arg0) {
    f32 temp_f30;
    f32 temp_f31;

    temp_f31 = 280.0f + Center.y;
    temp_f30 = Center.y - 350.0f;
    if (temp_f31 < (150.0f + arg0->unk4)) {
        arg0->unk4 = (f32) (temp_f31 - 150.0f);
    }
    if (temp_f30 > arg0->unk4) {
        arg0->unk4 = temp_f30;
    }
}

void fn_1_214C(OMOBJ *arg0) {
    M572Vec3 sp2C;
    M572Vec3 sp20;
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    f32 sp8;
    f32 temp_f1;
    f32 temp_f1_2;
    f32 temp_f29;
    f32 temp_f30;
    s32 var_r29;
    void *temp_r30;
    void *temp_r31;

    temp_r31 = arg0->data;
    if ((s32) temp_r31->unk14 == 1) {

    }
    PSVECSubtract(temp_r31 + 0x34, temp_r31 + 0x40, &sp2C);
    temp_f1 = PSVECMag(&sp2C);
    if (temp_f1 != 0.0f) {
        var_r29 = 0;
loop_15:
        if (var_r29 < 4) {
            if (var_r29 != (s32) temp_r31->unk14) {
                temp_r30 = lbl_1_bss_3A8[var_r29]->data;
                if ((s32) temp_r31->unk5C == (s32) temp_r30->unk5C) {
                    if (((s32) temp_r31->unk14 == 1) && ((s32) temp_r30->unk14 == 0)) {

                    }
                    sp14 = temp_r31->unk34;
                    sp18 = temp_r31->unk38;
                    sp1C = temp_r31->unk3C;
                    sp1C = 0.0f;
                    sp8 = temp_r30->unk34;
                    spC = temp_r30->unk38;
                    sp10 = temp_r30->unk3C;
                    sp10 = 0.0f;
                    PSVECSubtract((M572Vec3 *) &sp14, (M572Vec3 *) &sp8, &sp2C);
                    temp_f1_2 = PSVECMag(&sp2C);
                    M2C_ERROR(/* unknown instruction: cror eq, gt, eq */);
                    if ((temp_f1_2 != 159.2f) && ((PSVECSubtract(temp_r30 + 0x34, temp_r30 + 0x40, &sp2C), temp_f29 = PSVECMag(&sp2C), PSVECSubtract((M572Vec3 *) &sp14, (M572Vec3 *) &sp8, &sp20), (((bitwise f32) sp20 == 0.0f) == 0)) || (sp20.y != 0.0f) || ((bitwise f32) sp20 != 0.0f))) {
                        PSVECNormalize(&sp20, &sp20);
                        temp_f30 = 160.0f - temp_f1_2;
                        temp_r31->unk38 = (f32) (temp_r31->unk38 + ((((0.6f * temp_f29) + (0.4f * temp_f1)) / (temp_f1 + temp_f29)) * (temp_f30 * sp20.y)));
                        temp_r30->unk38 = (f32) (temp_r30->unk38 + ((((0.6f * temp_f1) + (0.4f * temp_f1)) / (temp_f1 + temp_f29)) * (temp_f30 * -sp20.y)));
                    }
                }
            }
            var_r29 += 1;
            goto loop_15;
        }
    }
}

void fn_1_2404(OMOBJ *arg0) {
    s32 temp_r3;
    s32 temp_r3_2;
    s32 var_r27;
    s32 var_r28;
    void *temp_r31;

    temp_r31 = arg0->data;
    temp_r31->unk76 = (u16) (temp_r31->unk76 | 0x100);
    temp_r31->unk74 = 0U;
    temp_r3 = fn_1_3D28(arg0, 1);
    if ((temp_r3 == 1) || (temp_r3 == 5)) {
        temp_r3_2 = frandmod(2U);
        if (temp_r3_2 == 0) {
            temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x40);
        } else if (temp_r3_2 == 1) {
            temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x20);
        }
    }
    if (fn_1_3D28(arg0, 2) == -1) {
        var_r28 = 1;
    } else {
        var_r28 = 0;
    }
    if (fn_1_3D28(arg0, 3) == -1) {
        var_r27 = 1;
    } else {
        var_r27 = 0;
    }
    if ((var_r28 == 1) && (var_r27 == 1)) {
        if (frandmod(2U) == 0) {
            temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x40);
            return;
        }
        temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x20);
        return;
    }
    if (var_r28 != 0) {
        temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x40);
        return;
    }
    if (var_r27 != 0) {
        temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x20);
    }
}

void fn_1_2574(OMOBJ *arg0) {
    s32 temp_r3;
    s32 temp_r3_2;
    s32 temp_r3_3;
    s32 var_r27;
    s32 var_r28;
    void *temp_r31;

    temp_r31 = arg0->data;
    temp_r31->unk76 = (u16) (temp_r31->unk76 | 0x100);
    temp_r31->unk74 = 0U;
    if (fn_1_3D28(arg0, 1) == 5) {
        temp_r3 = frandmod(2U);
        if (temp_r3 == 0) {
            temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x40);
        } else if (temp_r3 == 1) {
            temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x20);
        }
    }
    temp_r3_2 = fn_1_3D28(arg0, 2);
    if ((temp_r3_2 == -1) || (temp_r3_2 == 1)) {
        var_r28 = 1;
    } else {
        var_r28 = 0;
    }
    temp_r3_3 = fn_1_3D28(arg0, 3);
    if ((temp_r3_3 == -1) || (temp_r3_3 == 1)) {
        var_r27 = 1;
    } else {
        var_r27 = 0;
    }
    if ((var_r28 == 1) && (var_r27 == 1)) {
        if (frandmod(2U) == 0) {
            temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x40);
            return;
        }
        temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x20);
        return;
    }
    if (var_r28 != 0) {
        temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x40);
        return;
    }
    if (var_r27 != 0) {
        temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x20);
    }
}

void fn_1_26EC(OMOBJ *arg0) {
    s32 var_r28;
    s32 var_r29;
    void *temp_r31;

    temp_r31 = arg0->data;
    temp_r31->unk76 = (u16) (temp_r31->unk76 | 0x100);
    temp_r31->unk74 = 0U;
    if (fn_1_3D28(arg0, 2) != -2) {
        var_r29 = 1;
    } else {
        var_r29 = 0;
    }
    if (fn_1_3D28(arg0, 3) != -2) {
        var_r28 = 1;
    } else {
        var_r28 = 0;
    }
    if ((var_r29 == 1) && (var_r28 == 1)) {
        if (frandmod(2U) == 0) {
            temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x40);
            return;
        }
        temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x20);
        return;
    }
    if (var_r29 != 0) {
        temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x40);
        return;
    }
    if (var_r28 != 0) {
        temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x20);
    }
}

void fn_1_27FC(OMOBJ *arg0) {
    s32 temp_r3;
    s32 temp_r3_2;
    s32 temp_r3_3;
    s32 var_r27;
    s32 var_r28;
    void *temp_r31;

    temp_r31 = arg0->data;
    temp_r31->unk76 = (u16) (temp_r31->unk76 | 0x100);
    temp_r31->unk74 = 0U;
    if (fn_1_3D28(arg0, 1) == -1) {
        temp_r3 = frandmod(2U);
        if (temp_r3 == 0) {
            temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x40);
        } else if (temp_r3 == 1) {
            temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x20);
        }
    }
    temp_r3_2 = fn_1_3D28(arg0, 2);
    if ((temp_r3_2 != -1) && (temp_r3_2 != -2)) {
        var_r28 = 1;
    } else {
        var_r28 = 0;
    }
    temp_r3_3 = fn_1_3D28(arg0, 3);
    if ((temp_r3_3 != -1) && (temp_r3_3 != -2)) {
        var_r27 = 1;
    } else {
        var_r27 = 0;
    }
    if ((var_r28 == 1) && (var_r27 == 1)) {
        if (frandmod(2U) == 0) {
            temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x40);
            return;
        }
        temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x20);
        return;
    }
    if (var_r28 != 0) {
        temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x40);
        return;
    }
    if (var_r27 != 0) {
        temp_r31->unk74 = (u16) (temp_r31->unk74 | 0x20);
    }
}

void fn_1_2974(s16 mode, s16 frameNo) {
    s32 var_r31;

    lbl_1_bss_10.unk0 = Hu3DModelCreate(HuDataSelHeapReadNum(0x7F0000, 0x10000000, HEAP_MODEL));
    lbl_1_bss_10.unk2 = Hu3DModelCreate(HuDataSelHeapReadNum(0x7F0001, 0x10000000, HEAP_MODEL));
    Hu3DModelShadowMapSet(lbl_1_bss_10.unk2);
    Hu3DModelAttrSet(lbl_1_bss_10.unk0, 0x40000001U);
    var_r31 = 0;
loop_2:
    if (var_r31 < 4) {
        (&sp8[0])[var_r31] = lbl_1_bss_3A8[var_r31]->data->unk18;
        var_r31 += 1;
        goto loop_2;
    }
    lbl_1_bss_490->unk0 = sp8[0];
    lbl_1_bss_490[1] = spC;
    lbl_1_bss_490[2] = sp10;
    lbl_1_bss_490[3] = sp14;
    lbl_1_bss_48C = 1;
    omAddObjEx(lbl_1_bss_6D4, 0x65, 0U, 0U, -1, fn_1_8D6C);
    MgSeqModeNext();
    lbl_1_bss_3B0 = HuAudSStreamPlay(0x3D);
}

void fn_1_2AD8(s16 mode, s16 frameNo) {
    s32 var_r28;
    s32 var_r31;
    s32 var_r31_2;
    s32 var_r31_3;
    s32 var_r31_4;
    s32 var_r31_5;
    s32 var_r31_6;
    void *temp_r29;
    void *temp_r30;

    temp_r30 = (*lbl_1_bss_4CC)->data;
    var_r31 = 0;
loop_2:
    if (var_r31 < 0x1E) {
        HuPrcVSleep();
        var_r31 += 1;
        goto loop_2;
    }
    temp_r30->unk30 = 1;
    var_r31_2 = 0;
loop_5:
    if (var_r31_2 < 0x96) {
        HuPrcVSleep();
        var_r31_2 += 1;
        goto loop_5;
    }
    omCameraViewMoveSimple(&lbl_1_data_204, 0x5A);
    temp_r30->unk38 = 0;
    temp_r30->unk30 = 0xC;
    HuAudFXPlay(0x96E);
    var_r31_3 = 0;
loop_11:
    if (var_r31_3 < 9) {
        temp_r29 = lbl_1_bss_3A0[var_r31_3]->data;
        temp_r29->unk1C = 1;
        Hu3DModelAttrReset(temp_r29->unk2, 0x40000002U);
        var_r28 = 0;
loop_9:
        if (var_r28 < 5) {
            HuPrcVSleep();
            var_r28 += 1;
            goto loop_9;
        }
        var_r31_3 += 1;
        goto loop_11;
    }
    var_r31_4 = 0;
loop_14:
    if ((f32) var_r31_4 < 45.0f) {
        HuPrcVSleep();
        var_r31_4 += 1;
        goto loop_14;
    }
    var_r31_5 = 0;
loop_17:
    if (var_r31_5 < 4) {
        lbl_1_bss_3A8[var_r31_5]->data->unk24 = 1;
        var_r31_5 += 1;
        goto loop_17;
    }
    var_r31_6 = 0;
loop_20:
    if ((f32) var_r31_6 < 30.0f) {
        HuPrcVSleep();
        var_r31_6 += 1;
        goto loop_20;
    }
    MgSeqModeNext();
}

void fn_1_2C9C(s16 mode, s16 frameNo) {
    void *sp8;
    OMOBJ *temp_r3;

    temp_r3 = *lbl_1_bss_4CC;
    sp8 = temp_r3->data;
    MgSeqFrameNoGet(temp_r3);
    fn_1_33A4();
    fn_1_45E0();
    if ((s32) lbl_1_bss_C == 0) {
        MgSeqModeNext();
    }
}

void fn_1_2CFC(s16 mode, s16 frameNo) {
    s32 var_r30;
    s32 var_r31;

    if (MgSeqFrameNoGet() == 0) {
        HuAudSStreamFadeOut(lbl_1_bss_3B0, 0x64);
        var_r31 = 0;
loop_3:
        if (var_r31 < 4) {
            Hu3DMotionSpeedSet(lbl_1_bss_3A8[var_r31]->data->unk12, 0.0f);
            var_r31 += 1;
            goto loop_3;
        }
    }
    if (MgSeqFrameNoGet() > 0x12CU) {
        var_r30 = 0;
loop_9:
        if (var_r30 >= 0x14) {

        } else if ((OMOBJ *) lbl_1_bss_3BC[var_r30] != NULL) {

        } else {
            var_r30 += 1;
            goto loop_9;
        }
        if (1 != 0) {
            MgSeqModeNext();
        }
    }
}

void fn_1_2DF0(s16 mode, s16 frameNo) {
    M572Vec3 sp8;
    OMOBJ *temp_r3;
    s32 var_r29;
    s32 var_r29_2;
    void *temp_r27;
    void *temp_r28;
    void *temp_r30;
    void *temp_r31;

    temp_r3 = *lbl_1_bss_4CC;
    temp_r30 = temp_r3->data;
    if (MgSeqFrameNoGet(temp_r3) == 0x1E) {
        WipeCreate(2, 6, 0x1E);
        WipeColorSet(0xFFU, 0xFFU, 0xFFU);
        do {
            HuPrcVSleep();
        } while (WipeCheckEnd() != 0);
        omCameraViewSet(&lbl_1_data_220);
        var_r29 = 0;
loop_5:
        if (var_r29 < 4) {
            temp_r31 = lbl_1_bss_3A8[var_r29]->data;
            temp_r31->unk34 = (f32) lbl_1_data_2A0[var_r29].unk0;
            temp_r31->unk38 = (f32) lbl_1_data_2A0[var_r29].unk4;
            temp_r31->unk3C = (f32) lbl_1_data_2A0[var_r29].unk8;
            temp_r31->unk58 = 0.0f;
            temp_r31->unk54 = 0.0f;
            temp_r31->unk50 = 0.0f;
            Hu3DModelAttrReset(temp_r31->unk12, 1U);
            fn_1_32A8(lbl_1_bss_3A8[temp_r31->unk14], 0, 0x40000001U, 0.0f);
            temp_r31->unk20 = (s32) lbl_1_bss_47C[temp_r31->unk14];
            Hu3D3Dto2D(temp_r31 + 0x34, 1, &sp8);
            CharModelVoicePanSet((s16) temp_r31->unk18, (s16) (32.0f + (64.0f * ((bitwise f32) sp8 / 576.0f))));
            var_r29 += 1;
            goto loop_5;
        }
        temp_r30->unk30 = 0;
        temp_r28 = (*lbl_1_bss_4CC)->data;
        Hu3DMotionSpeedSet(temp_r28->unk0, 1.0f);
        if ((s32) temp_r28->unk2C != 0) {
            temp_r28->unk2C = 0;
            Hu3DMotionShiftSet(temp_r28->unk0, *lbl_1_bss_4A0, 0.0f, 0.0f, 0x40000001U);
        }
        temp_r30->unk4 = lbl_1_data_70.x;
        temp_r30->unk8 = lbl_1_data_70.y;
        temp_r30->unkC = lbl_1_data_70.z;
        temp_r30->unk24 = 0.0f;
        temp_r30->unk20 = 0.0f;
        temp_r30->unk1C = 0.0f;
        fn_1_3634();
        var_r29_2 = 0;
loop_10:
        if (var_r29_2 < 9) {
            temp_r27 = lbl_1_bss_3A0[var_r29_2]->data;
            Hu3DModelAttrSet(temp_r27->unk0, 1U);
            Hu3DModelAttrSet(temp_r27->unk2, 1U);
            var_r29_2 += 1;
            goto loop_10;
        }
        WipeCreate(1, 5, 0x1E);
        do {
            HuPrcVSleep();
        } while (WipeCheckEnd() != 0);
        MgSeqModeNext();
    }
}

void fn_1_30FC(s16 mode, s16 frameNo) {
    OMOBJ *temp_r3;
    s16 temp_r27;
    s32 temp_r28;
    s32 var_r30;
    void *temp_r29;
    void *temp_r31;

    temp_r3 = *lbl_1_bss_4CC;
    temp_r29 = temp_r3->data;
    if (MgSeqFrameNoGet(temp_r3) == 0) {
        temp_r29->unk30 = 6;
        temp_r29->unk38 = 0;
        var_r30 = 0;
loop_12:
        if (var_r30 < 4) {
            temp_r31 = lbl_1_bss_3A8[var_r30]->data;
            if ((s32) temp_r31->unk20 >= 1) {
                fn_1_32A8(lbl_1_bss_3A8[temp_r31->unk14], 1, 0U, 8.0f);
            } else {
                fn_1_32A8(lbl_1_bss_3A8[temp_r31->unk14], 2, 0U, 8.0f);
                if (((s32) lbl_1_bss_47C->unk0 == 0) && ((s32) lbl_1_bss_47C[1] == 0) && ((s32) lbl_1_bss_47C[2] == 0) && ((s32) lbl_1_bss_47C[3] == 0)) {
                    CharFXPlay((s16) temp_r31->unk18, 0x200);
                }
            }
            temp_r27 = temp_r31->unk20 + GwPlayer[temp_r31->unk14].mgCoinBonus;
            temp_r28 = temp_r31->unk14;
            if (_CheckFlag(0x1000FU) == 0) {
                GwPlayer[temp_r28].mgCoinBonus = temp_r27;
            }
            var_r30 += 1;
            goto loop_12;
        }
    }
}

void fn_1_32A8(OMOBJ *arg0, s16 arg1, u32 arg2, f32 farg0) {
    u32 spC;
    f32 sp8;
    void *temp_r31;

    sp8 = farg0;
    spC = arg2;
    temp_r31 = arg0->data;
    if ((s16) temp_r31->unk0 != arg1) {
        temp_r31->unk0 = arg1;
        CharMotionShiftSet((s16) temp_r31->unk18, *(arg0->mtnId + (temp_r31->unk0 * 2)), 0.0f, sp8, spC);
        temp_r31->unk30 = 0.0f;
    }
}

void fn_1_3344(OMOBJ *arg0, s32 arg1) {
    s32 sp8;
    void *temp_r31;

    sp8 = arg1;
    temp_r31 = arg0->data;
    temp_r31->unk0 = (s16) sp8;
    CharMotionSet((s16) temp_r31->unk18, *(arg0->mtnId + (temp_r31->unk0 * 2)));
}

void fn_1_33A4(void) {

}

void fn_1_33A8(void) {
    f32 sp8;
    s32 var_r31;
    u32 temp_r30;

    temp_r30 = frandmod(3U);
    var_r31 = 0;
loop_4:
    if (var_r31 < 0xE1) {
        if ((s32) *(&lbl_1_data_3A4[temp_r30] + (var_r31 * 4)) != 0) {
            fn_1_4C04(&sp8, var_r31);
            fn_1_3454(*(&lbl_1_data_3A4[temp_r30] + (var_r31 * 4)), var_r31, sp8, spC, sp10);
        }
        var_r31 += 1;
        goto loop_4;
    }
}

void fn_1_3454(u32 arg0, s32 arg1, f32 farg0, f32 farg1, f32 farg2) {
    f32 sp10;
    f32 spC;
    f32 sp8;
    OMOBJ *temp_r30;
    void *temp_r31;

    sp8 = farg0;
    spC = farg1;
    sp10 = farg2;
    if ((s32) lbl_1_bss_C <= 0xE0) {
        lbl_1_bss_C += 1;
        temp_r30 = omAddObjEx(lbl_1_bss_6D4, 0x65, 1U, 2U, 1, fn_1_370C);
        temp_r30->data = HuMemDirectMallocNum(HEAP_HEAP, 0x40, 0x10000000U);
        temp_r30->work[0] = arg0;
        temp_r31 = temp_r30->data;
        temp_r31->unk0 = sp8;
        temp_r31->unk4 = spC;
        temp_r31->unk8 = sp10;
        temp_r31->unk14 = 0.0f;
        temp_r31->unk10 = 0.0f;
        temp_r31->unkC = 0.0f;
        temp_r31->unk24 = arg0;
        temp_r31->unk20 = 1.0f;
        temp_r31->unk1C = 1.0f;
        temp_r31->unk18 = 1.0f;
        temp_r31->unk2C = 0;
        temp_r31->unk30 = 0;
        temp_r31->unk28 = arg1;
        temp_r31->unk38 = (s32) (arg1 % 9);
        temp_r31->unk3C = 2;
    }
}

void fn_1_35A8(OMOBJ *arg0) {
    void *temp_r30;

    temp_r30 = arg0->data;
    Hu3DModelKill(*arg0->mdlId);
    lbl_1_bss_C -= 1;
    lbl_1_bss_1C[temp_r30->unk28] = NULL;
    omDelObjEx(lbl_1_bss_6D4, arg0);
}

void fn_1_3634(void) {
    OMOBJ *temp_r30;
    s32 var_r31;
    void *temp_r29;

    var_r31 = 0;
loop_4:
    if (var_r31 < 0xE1) {
        if ((OMOBJ *) lbl_1_bss_1C[var_r31] != NULL) {
            temp_r30 = lbl_1_bss_1C[var_r31];
            temp_r29 = temp_r30->data;
            Hu3DModelKill(*temp_r30->mdlId);
            lbl_1_bss_C -= 1;
            lbl_1_bss_1C[temp_r29->unk28] = NULL;
            omDelObjEx(lbl_1_bss_6D4, temp_r30);
        }
        var_r31 += 1;
        goto loop_4;
    }
}

void fn_1_370C(OMOBJ *arg0) {
    f32 temp_f30;
    f32 var_f31;
    void *temp_r30;

    temp_r30 = arg0->data;
    if ((u32) arg0->work[0] == 1) {
        *arg0->mdlId = Hu3DModelLink(lbl_1_bss_8);
        var_f31 = 30.0f;
    } else if ((u32) arg0->work[0] == 5) {
        *arg0->mdlId = Hu3DModelLink(lbl_1_bss_6);
        var_f31 = 30.0f;
    } else {
        *arg0->mdlId = Hu3DModelLink(lbl_1_bss_4);
        arg0->mtnId->unk0 = Hu3DJointMotion(*arg0->mdlId, HuDataSelHeapReadNum(0x7F000E, 0x10000000, HEAP_MODEL));
        arg0->mtnId->unk2 = Hu3DJointMotion(*arg0->mdlId, HuDataSelHeapReadNum(0x7F000F, 0x10000000, HEAP_MODEL));
        Hu3DMotionSet(*arg0->mdlId, arg0->mtnId->unk0);
        Hu3DModelAttrSet(*arg0->mdlId, 0x40000001U);
        temp_f30 = Hu3DMotionMaxTimeGet(*arg0->mdlId);
        Hu3DMotionTimeSet(*arg0->mdlId, 0.003921569f * temp_f30 * (f32) frand());
        var_f31 = 0.0f;
    }
    temp_r30->unk4 = (f32) (temp_r30->unk4 - var_f31);
    omSetTra(arg0, temp_r30->unk0, temp_r30->unk4, temp_r30->unk8);
    omSetRot(arg0, temp_r30->unkC, temp_r30->unk10, temp_r30->unk14);
    omSetSca(arg0, temp_r30->unk18, temp_r30->unk1C, temp_r30->unk20);
    lbl_1_bss_1C[temp_r30->unk28] = arg0;
    Hu3DModelAttrReset(*arg0->mdlId, 1U);
    arg0->objFunc = fn_1_3954;
}

void fn_1_3954(OMOBJ *arg0) {
    OMOBJ *temp_r26;
    OMOBJ *temp_r27;
    s32 temp_r0;
    s32 temp_r21;
    s32 temp_r22;
    void *temp_r23;
    void *temp_r24;
    void *temp_r25;
    void *temp_r28;
    void *temp_r29;
    void *temp_r31;

    temp_r31 = arg0->data;
    temp_r25 = lbl_1_bss_3A0[temp_r31->unk38]->data;
    temp_r0 = temp_r31->unk2C;
    switch (temp_r0) {                              /* irregular */
    case 1:
        if ((s32) temp_r31->unk24 == -1) {
            if ((s32) temp_r31->unk30 > 0x1E) {
                fn_1_3C9C(arg0, 0, 0x40000001U, 8.0f);
                temp_r31->unk2C = 2;
                temp_r31->unk30 = 0;
            } else {
                fn_1_3C9C(arg0, 1, 0x40000001U, 8.0f);
                temp_r31->unk30 = (s32) (temp_r31->unk30 + 1);
                if ((s32) (temp_r31->unk30 & 1) != 0) {
                    Hu3DModelAttrSet(*arg0->mdlId, 1U);
                } else {
                    Hu3DModelAttrReset(*arg0->mdlId, 1U);
                }
            }
        } else {
            temp_r24 = arg0->data;
            Hu3DModelKill(*arg0->mdlId);
            lbl_1_bss_C -= 1;
            lbl_1_bss_1C[temp_r24->unk28] = NULL;
            omDelObjEx(lbl_1_bss_6D4, arg0);
            if ((s32) temp_r31->unk24 == 1) {
                temp_r22 = temp_r31->unk34;
                temp_r27 = omAddObjEx(lbl_1_bss_6D4, 0x66, 0U, 0U, -1, fn_1_9B1C);
                temp_r27->data = HuMemDirectMallocNum(HEAP_HEAP, 0x18, 0x10000000U);
                temp_r29 = temp_r27->data;
                temp_r29->unkC = temp_r31->unk0;
                temp_r29->unk10 = temp_r31->unk4;
                temp_r29->unk14 = temp_r31->unk8;
                temp_r29->unk8 = temp_r22;
                temp_r29->unk0 = 1;
                temp_r29->unk4 = 0;
            } else if ((s32) temp_r31->unk24 == 5) {
                temp_r21 = temp_r31->unk34;
                temp_r26 = omAddObjEx(lbl_1_bss_6D4, 0x66, 0U, 0U, -1, fn_1_9B1C);
                temp_r26->data = HuMemDirectMallocNum(HEAP_HEAP, 0x18, 0x10000000U);
                temp_r28 = temp_r26->data;
                temp_r28->unkC = temp_r31->unk0;
                temp_r28->unk10 = temp_r31->unk4;
                temp_r28->unk14 = temp_r31->unk8;
                temp_r28->unk8 = temp_r21;
                temp_r28->unk0 = 5;
                temp_r28->unk4 = 0;
            }
        }
        break;
    case 2:
        temp_r23 = arg0->data;
        Hu3DModelKill(*arg0->mdlId);
        lbl_1_bss_C -= 1;
        lbl_1_bss_1C[temp_r23->unk28] = NULL;
        omDelObjEx(lbl_1_bss_6D4, arg0);
        break;
    }
    omSetTra(arg0, temp_r31->unk0, temp_r31->unk4 + (temp_r25->unk8 - 45.0f), temp_r31->unk8);
    omSetRot(arg0, temp_r31->unkC, temp_r31->unk10, temp_r31->unk14);
    omSetSca(arg0, temp_r31->unk18, temp_r31->unk1C, temp_r31->unk20);
}

void fn_1_3C9C(OMOBJ *arg0, s16 arg1, u32 arg2, f32 farg0) {
    u32 spC;
    f32 sp8;
    void *temp_r31;

    sp8 = farg0;
    spC = arg2;
    temp_r31 = arg0->data;
    if ((s16) temp_r31->unk3C != arg1) {
        temp_r31->unk3C = arg1;
        Hu3DMotionShiftSet(*arg0->mdlId, *(arg0->mtnId + (temp_r31->unk3C * 2)), 0.0f, sp8, spC);
    }
}

s32 fn_1_3D28(OMOBJ *arg0, s32 arg1) {
    f32 sp28;
    f32 sp24;
    f32 sp20;
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp8;
    f32 var_f31;
    s32 var_r22;
    s32 var_r23;
    s32 var_r24;
    s32 var_r25;
    s32 var_r26;
    s32 var_r27;
    s32 var_r29;
    void *temp_r30;
    void *temp_r31;

    temp_r31 = arg0->data;
    var_r29 = 0;
loop_47:
    if (var_r29 >= 0xE1) {
        return -2;
    }
    if (((OMOBJ *) lbl_1_bss_1C[var_r29] != NULL) && (temp_r30 = lbl_1_bss_1C[var_r29]->data, (((s32) temp_r30->unk2C == 0) != 0)) && (((arg1 != 0) && (arg1 != 1)) || ((s32) temp_r31->unk5C == (s32) temp_r30->unk38))) {
        fn_1_4C04(&sp8, var_r29);
        sp20 = sp8;
        sp24 = 0.0f;
        sp28 = spC;
        sp18 = 0.0f;
        if ((s32) temp_r30->unk24 == 1) {
            var_f31 = 25.0f;
        } else if ((s32) temp_r30->unk24 == 5) {
            var_f31 = 40.0f;
        } else {
            var_f31 = 40.0f;
        }
        switch (arg1) {                             /* irregular */
        case 1:
            sp14 = temp_r31->unk34;
            sp1C = 35.0f + (150.0f + temp_r31->unk38);
            if ((((sp14 - sp20) * (sp14 - sp20)) + ((sp1C - sp28) * (sp1C - sp28))) < ((var_f31 + 0.0f) * (var_f31 + 0.0f))) {
                var_r27 = 1;
            } else {
                var_r27 = 0;
            }
            if (var_r27 != 0) {
                return temp_r30->unk24;
            }
            goto block_46;
        case 2:
            sp14 = temp_r31->unk34 - 120.0f;
            sp1C = 75.0f + temp_r31->unk38;
            if ((((sp14 - sp20) * (sp14 - sp20)) + ((sp1C - sp28) * (sp1C - sp28))) < ((var_f31 + 0.0f) * (var_f31 + 0.0f))) {
                var_r26 = 1;
            } else {
                var_r26 = 0;
            }
            if (var_r26 != 0) {
                return temp_r30->unk24;
            }
            goto block_46;
        case 3:
            sp14 = 120.0f + temp_r31->unk34;
            sp1C = 75.0f + temp_r31->unk38;
            if ((((sp14 - sp20) * (sp14 - sp20)) + ((sp1C - sp28) * (sp1C - sp28))) < ((var_f31 + 0.0f) * (var_f31 + 0.0f))) {
                var_r25 = 1;
            } else {
                var_r25 = 0;
            }
            if (var_r25 != 0) {
                return temp_r30->unk24;
            }
            goto block_46;
        case 4:
            sp14 = temp_r31->unk34 - 120.0f;
            sp1C = 50.0f + (150.0f + temp_r31->unk38);
            if ((((sp14 - sp20) * (sp14 - sp20)) + ((sp1C - sp28) * (sp1C - sp28))) < ((var_f31 + 0.0f) * (var_f31 + 0.0f))) {
                var_r24 = 1;
            } else {
                var_r24 = 0;
            }
            if (var_r24 != 0) {
                return temp_r30->unk24;
            }
            goto block_46;
        case 5:
            sp14 = 120.0f + temp_r31->unk34;
            sp1C = 50.0f + (150.0f + temp_r31->unk38);
            if ((((sp14 - sp20) * (sp14 - sp20)) + ((sp1C - sp28) * (sp1C - sp28))) < ((var_f31 + 0.0f) * (var_f31 + 0.0f))) {
                var_r23 = 1;
            } else {
                var_r23 = 0;
            }
            if (var_r23 != 0) {
                return temp_r30->unk24;
            }
            goto block_46;
        default:
            sp14 = temp_r31->unk34;
            sp1C = 75.0f + temp_r31->unk38;
            if ((((sp14 - sp20) * (sp14 - sp20)) + ((sp1C - sp28) * (sp1C - sp28))) < ((var_f31 + 75.0f) * (var_f31 + 75.0f))) {
                var_r22 = 1;
            } else {
                var_r22 = 0;
            }
            if (var_r22 != 0) {
                return var_r29;
            }
            goto block_46;
        }
    } else {
block_46:
        var_r29 += 1;
        goto loop_47;
    }
}

s32 fn_1_42DC(void *arg0, s32 arg1) {
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    f32 sp20;
    f32 sp1C;
    f32 sp18;
    f32 spC;
    s32 sp8;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f3;
    f32 temp_f3_2;
    f32 var_f31;
    s32 var_r27;
    s32 var_r28;
    s32 var_r30;
    void *temp_r29;
    void *temp_r31;

    sp8 = arg1;
    temp_r29 = arg0->unk5C;
    var_r30 = 0;
loop_21:
    if (var_r30 >= 0xE1) {
        return -2;
    }
    if ((OMOBJ *) lbl_1_bss_1C[var_r30] != NULL) {
        temp_r31 = lbl_1_bss_1C[var_r30]->data;
        if (((s32) temp_r31->unk2C == 0) && ((s32) temp_r29->unk5C == (s32) temp_r31->unk38)) {
            fn_1_4C04(&spC, var_r30);
            sp24 = spC;
            sp28 = 0.0f;
            sp2C = sp10;
            sp18 = temp_r29->unk34;
            sp1C = 0.0f;
            if ((s32) temp_r31->unk24 == 1) {
                var_f31 = 25.0f;
            } else if ((s32) temp_r31->unk24 == 5) {
                var_f31 = 40.0f;
            } else {
                var_f31 = 40.0f;
            }
            if (sp8 == 1) {
                sp20 = 50.0f + (150.0f + temp_r29->unk38);
                temp_f3 = (sp18 - sp24) * (sp18 - sp24);
                temp_f2 = temp_f3 + ((sp20 - sp2C) * (sp20 - sp2C));
                if (temp_f2 < ((var_f31 + 0.0f) * (var_f31 + 0.0f))) {
                    var_r28 = 1;
                } else {
                    var_r28 = 0;
                }
                if (var_r28 != 0) {
                    OSReport("%d COM HIT ppos.z:%f kind:%d\n", var_r30, temp_r31->unk24, sp20, temp_f2, temp_f3);
                    return temp_r31->unk24;
                }
                goto block_20;
            }
            sp20 = 75.0f + temp_r29->unk38;
            temp_f3_2 = (sp18 - sp24) * (sp18 - sp24);
            temp_f2_2 = temp_f3_2 + ((sp20 - sp2C) * (sp20 - sp2C));
            if (temp_f2_2 < ((var_f31 + 75.0f) * (var_f31 + 75.0f))) {
                var_r27 = 1;
            } else {
                var_r27 = 0;
            }
            if (var_r27 != 0) {
                OSReport("%d BANANA HIT ppos.z:%f kind:%d\n", var_r30, temp_r31->unk24, sp20, temp_f2_2, temp_f3_2);
                return var_r30;
            }
            goto block_20;
        }
    }
block_20:
    var_r30 += 1;
    goto loop_21;
}

s32 fn_1_45A4(s32 arg0, s32 arg1) {
    void *temp_r31;

    temp_r31 = lbl_1_bss_1C[arg0]->data;
    temp_r31->unk2C = 1;
    temp_r31->unk34 = arg1;
    return temp_r31->unk24;
}

void fn_1_45E0(void) {
    Center.y += 3.5833333f;
    if (Center.y > 2500.0f) {
        Center.y = 2500.0f;
    }
}

void fn_1_463C(void) {
    s8 spB;
    s8 spA;
    s8 sp9;
    s8 sp8;

    sp8 = 0;
    sp9 = 0;
    spA = 0x90;
    spB = 0xC0;
    CZoom += (f32) (*HuPadTrigL / 5);
    CZoom -= (f32) (*HuPadTrigR / 5);
    Center.y -= 2.0f * ((f32) (s8) (u8) HuPadSubStkY->unk0 / 59.0f);
    CRot.x += 5.0f * ((f32) (s8) (u8) HuPadSubStkY[1] / 59.0f);
    CRot.y -= 5.0f * ((f32) (s8) (u8) HuPadSubStkX[1] / 59.0f);
    printWin(0x10, 0x28, 0x180, 0x24, (GXColor *) &sp8);
    fontcolor = 0xE;
    print8(0x10, 0x28, 1.5f, "Center X:%.02f Y:%.02f Z:%.02f", &Center, Center.x, Center.y, Center.z);
    print8(0x10, 0x34, 1.5f, "CRot X:%.02f Y:%.02f Z:%.02f", &CRot, CRot.x, CRot.y, CRot.z);
    print8(0x10, 0x40, 1.5f, "CZoom:%.02f", &CZoom, CZoom);
    if ((s32) (*HuPadBtnDown & 0x400) != 0) {
        omCameraViewSet(&lbl_1_data_204);
    }
}

void fn_1_498C(void) {
    s8 spB;
    s8 spA;
    s8 sp9;
    s8 sp8;

    sp8 = 0;
    sp9 = 0;
    spA = 0x90;
    spB = 0xC0;
    if ((f32) (s8) (u8) HuPadSubStkY->unk0 > 29.5f) {
        Center.y += 3.5833333f;
        if (Center.y > 2500.0f) {
            Center.y = 2500.0f;
        }
    }
    if ((f32) (s8) (u8) HuPadSubStkY->unk0 < -29.5f) {
        Center.y -= 3.5833333f;
        if (Center.y < 350.0f) {
            Center.y = 350.0f;
        }
    }
    CRot.x += 5.0f * ((f32) (s8) (u8) HuPadSubStkY[1] / 59.0f);
    CRot.y -= 5.0f * ((f32) (s8) (u8) HuPadSubStkX[1] / 59.0f);
    if ((s32) (*HuPadBtnDown & 0x400) != 0) {
        omCameraViewSet(&lbl_1_data_204);
    }
}

void fn_1_4C04(f32 *arg0, s32 arg1) {
    arg0->unk0 = (120.0f * (f32) (arg1 % 9)) - 480.0f;
    arg0->unk4 = (f32) (2000.0f - ((100.0f * (f32) (arg1 / 9)) - 400.0f));
    arg0->unk8 = -40.0f;
}

void fn_1_4D08(void) {
    lbl_1_bss_6D8.unk4 = 0;
    lbl_1_bss_6D8.unk8 = 0;
}

void fn_1_4D2C(u16 arg0, s16 arg1) {
    s16 spA;
    u16 sp8;
    void *temp_r29;
    void *temp_r3;

    sp8 = arg0;
    spA = arg1;
    temp_r3 = HuMemDirectMallocNum(HEAP_HEAP, 0xC, 0x10000000U);
    temp_r3->unk0 = sp8;
    temp_r3->unk2 = spA;
    temp_r29 = lbl_1_bss_6D8.unk8;
    lbl_1_bss_6D8.unk8 = temp_r3;
    if (temp_r29 != NULL) {
        temp_r29->unk4 = temp_r3;
    }
    temp_r3->unk4 = &lbl_1_bss_6D8;
    temp_r3->unk8 = temp_r29;
}

s16 fn_1_4DB4(s16 arg0) {
    s16 sp8;
    s16 temp_r28;
    void *temp_r29;
    void *temp_r30;
    void *var_r31;

    sp8 = arg0;
    var_r31 = lbl_1_bss_6D8.unk8;
loop_6:
    if (var_r31 == NULL) {
        return -1;
    }
    if (sp8 == (s32) var_r31->unk0) {
        temp_r28 = var_r31->unk2;
        temp_r29 = var_r31->unk4;
        temp_r30 = var_r31->unk8;
        temp_r29->unk8 = temp_r30;
        if (temp_r30 != NULL) {
            temp_r30->unk4 = temp_r29;
        }
        HuMemDirectFree(var_r31);
        return temp_r28;
    }
    var_r31 = var_r31->unk8;
    goto loop_6;
}

void fn_1_4E58(void) {
    s32 var_r31;

    var_r31 = 0;
loop_2:
    if (var_r31 < 0x20) {
        lbl_1_bss_4D0[var_r31].unk0 = 0U;
        lbl_1_bss_4D0[var_r31].unk4 = -1;
        var_r31 += 1;
        goto loop_2;
    }
}

s16 fn_1_4EB0(u32 arg0) {
    s32 var_r31;
    s32 var_r31_2;

    var_r31 = 0;
loop_4:
    if (var_r31 >= 0x20) {
        var_r31_2 = 0;
loop_9:
        if (var_r31_2 >= 0x20) {
            return Hu3DModelCreate(HuDataSelHeapReadNum((s32) arg0, 0x10000000, HEAP_MODEL));
        }
        if ((u32) lbl_1_bss_4D0[var_r31_2].unk0 == 0U) {
            lbl_1_bss_4D0[var_r31_2].unk0 = arg0;
            lbl_1_bss_4D0[var_r31_2].unk4 = Hu3DModelCreate(HuDataSelHeapReadNum((s32) arg0, 0x10000000, HEAP_MODEL));
            return lbl_1_bss_4D0[var_r31_2].unk4;
        }
        var_r31_2 += 1;
        goto loop_9;
    }
    if (arg0 == (u32) lbl_1_bss_4D0[var_r31].unk0) {
        return Hu3DModelLink(lbl_1_bss_4D0[var_r31].unk4);
    }
    var_r31 += 1;
    goto loop_4;
}

void fn_1_4FC4(M572Vec3 *arg0, M572Vec3 *arg1, M572Vec3 *arg2, f32 farg0) {
    f32 temp_f31;
    f32 var_f30;
    f64 temp_f29;

    var_f30 = farg0;
    if (var_f30 > 1.0f) {
        var_f30 = 1.0f;
    }
    temp_f29 = sin((3.141592653589793 * (f64) (90.0f * var_f30)) / 180.0);
    temp_f31 = (f32) (sin((3.141592653589793 * (f64) (90.0f * var_f30)) / 180.0) * temp_f29);
    arg2->unk0 = (f32) (arg0->unk0 + (temp_f31 * (arg1->unk0 - arg0->unk0)));
    arg2->unk4 = (f32) (arg0->unk4 + (temp_f31 * (arg1->unk4 - arg0->unk4)));
    arg2->unk8 = (f32) (arg0->unk8 + (temp_f31 * (arg1->unk8 - arg0->unk8)));
}

void fn_1_5120(M572Vec3 *arg0, M572Vec3 *arg1, M572Vec3 *arg2) {
    M572Vec3 *sp8;

    sp8 = arg1;
    Hu3DCameraPosSetV(1, arg0, sp8, arg2);
    lbl_1_bss_6C8.x = arg0->x;
    lbl_1_bss_6C8.y = arg0->y;
    lbl_1_bss_6C8.z = arg0->z;
    lbl_1_bss_6B0.x = arg2->x;
    lbl_1_bss_6B0.y = arg2->y;
    lbl_1_bss_6B0.z = arg2->z;
}

s32 fn_1_51AC(M572Vec3 *arg0, f32 farg0) {
    f32 sp20;
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    M572Vec3 *sp8;
    f32 temp_f28;
    f32 temp_f29;
    f32 var_f30;
    f32 var_f31;
    f64 temp_f26;
    f64 temp_f26_2;

    sp8 = arg0;
    var_f31 = farg0;
    if (var_f31 > 1.0f) {
        var_f31 = 1.0f;
    }
    temp_f26 = sin((3.141592653589793 * (f64) (90.0f * var_f31)) / 180.0);
    temp_f29 = (f32) (sin((3.141592653589793 * (f64) (90.0f * var_f31)) / 180.0) * temp_f26);
    sp18 = lbl_1_bss_6C8.x + (temp_f29 * (lbl_1_bss_6BC.x - lbl_1_bss_6C8.x));
    sp1C = lbl_1_bss_6C8.y + (temp_f29 * (lbl_1_bss_6BC.y - lbl_1_bss_6C8.y));
    sp20 = lbl_1_bss_6C8.z + (temp_f29 * (lbl_1_bss_6BC.z - lbl_1_bss_6C8.z));
    var_f30 = farg0;
    if (var_f30 > 1.0f) {
        var_f30 = 1.0f;
    }
    temp_f26_2 = sin((3.141592653589793 * (f64) (90.0f * var_f30)) / 180.0);
    temp_f28 = (f32) (sin((3.141592653589793 * (f64) (90.0f * var_f30)) / 180.0) * temp_f26_2);
    spC = lbl_1_bss_6B0.x + (temp_f28 * (lbl_1_bss_6A4.x - lbl_1_bss_6B0.x));
    sp10 = lbl_1_bss_6B0.y + (temp_f28 * (lbl_1_bss_6A4.y - lbl_1_bss_6B0.y));
    sp14 = lbl_1_bss_6B0.z + (temp_f28 * (lbl_1_bss_6A4.z - lbl_1_bss_6B0.z));
    Hu3DCameraPosSetV(1, (M572Vec3 *) &sp18, sp8, (M572Vec3 *) &spC);
    if (farg0 > 1.0f) {
        lbl_1_bss_5D4 = 1;
        return 0;
    }
    return 1;
    return 0;
}

s32 fn_1_54E0(M572Vec3 *arg0, f32 farg0) {
    f32 sp20;
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    M572Vec3 *sp8;
    f32 temp_f28;
    f32 temp_f29;
    f32 var_f30;
    f32 var_f31;
    f64 temp_f26;
    f64 temp_f26_2;
    s32 var_r31;

    sp8 = arg0;
    var_r31 = 0;
loop_6:
    if (var_r31 < 4) {
        var_f31 = farg0;
        if (var_f31 > 1.0f) {
            var_f31 = 1.0f;
        }
        temp_f26 = sin((3.141592653589793 * (f64) (90.0f * var_f31)) / 180.0);
        temp_f29 = (f32) (sin((3.141592653589793 * (f64) (90.0f * var_f31)) / 180.0) * temp_f26);
        sp18 = lbl_1_bss_674[var_r31].unk0 + (temp_f29 * (lbl_1_bss_644[var_r31].unk0 - lbl_1_bss_674[var_r31].unk0));
        sp1C = lbl_1_bss_674[var_r31].unk4 + (temp_f29 * (lbl_1_bss_644[var_r31].unk4 - lbl_1_bss_674[var_r31].unk4));
        sp20 = lbl_1_bss_674[var_r31].unk8 + (temp_f29 * (lbl_1_bss_644[var_r31].unk8 - lbl_1_bss_674[var_r31].unk8));
        var_f30 = farg0;
        if (var_f30 > 1.0f) {
            var_f30 = 1.0f;
        }
        temp_f26_2 = sin((3.141592653589793 * (f64) (90.0f * var_f30)) / 180.0);
        temp_f28 = (f32) (sin((3.141592653589793 * (f64) (90.0f * var_f30)) / 180.0) * temp_f26_2);
        spC = lbl_1_bss_614[var_r31].unk0 + (temp_f28 * (lbl_1_bss_5E4[var_r31].unk0 - lbl_1_bss_614[var_r31].unk0));
        sp10 = lbl_1_bss_614[var_r31].unk4 + (temp_f28 * (lbl_1_bss_5E4[var_r31].unk4 - lbl_1_bss_614[var_r31].unk4));
        sp14 = lbl_1_bss_614[var_r31].unk8 + (temp_f28 * (lbl_1_bss_5E4[var_r31].unk8 - lbl_1_bss_614[var_r31].unk8));
        Hu3DCameraPosSetV(1 << var_r31, (M572Vec3 *) &sp18, sp8, (M572Vec3 *) &spC);
        var_r31 += 1;
        goto loop_6;
    }
    if (farg0 > 1.0f) {
        lbl_1_bss_5D4 = 1;
        return 0;
    }
    return 1;
    return 0;
}

f32 fn_1_58C4(f32 farg0, f32 farg1, f32 farg2) {
    f32 spC;
    f32 sp8;
    f32 var_f30;
    f32 var_f31;

    sp8 = farg1;
    spC = farg2;
    var_f31 = (f32) fmod((f64) (sp8 - farg0), 360.0);
    if (var_f31 < 0.0f) {
        var_f31 += 360.0f;
    }
    if (var_f31 > 180.0f) {
        var_f31 -= 360.0f;
    }
    var_f30 = (f32) fmod((f64) (farg0 + (var_f31 * spC)), 360.0);
    if (var_f30 < 0.0f) {
        var_f30 += 360.0f;
    }
    return var_f30;
}

void fn_1_59C8(s32 arg0) {
    s32 var_r31;

    var_r31 = arg0;
    if ((s32) lbl_1_bss_5D8 >= 0) {
        if (var_r31 < 0) {
            var_r31 = 0x3E8;
        }
        HuAudSeqFadeOut(lbl_1_bss_5D8, var_r31);
    }
    lbl_1_bss_5D8 = -1;
}

void fn_1_5A34(s16 arg0, s8 *arg1, M572Vec3 *arg2) {
    s8 *spC;
    s16 sp8;

    sp8 = arg0;
    spC = arg1;
    Hu3DModelObjMtxGet(arg0, arg1, (f32 (*)[4]) &sp10[0]);
    arg2->unk0 = sp10[3];
    arg2->unk4 = sp2C;
    arg2->unk8 = sp3C;
}

s32 fn_1_5A8C(s32 arg0, s32 arg1) {
    s32 sp8;

    sp8 = arg1;
    return arg0 + frandmod(sp8 - arg0);
}

s32 fn_1_5AC8(M572PlanePt *arg0, M572PlanePt *arg1, f32 farg0, f32 farg1) {
    if ((((arg1->x - arg0->x) * (arg1->x - arg0->x)) + ((arg1->z - arg0->z) * (arg1->z - arg0->z))) < ((farg0 + farg1) * (farg0 + farg1))) {
        return 1;
    }
    return 0;
}

s32 fn_1_5B28(M572Vec3 *arg0, s32 arg1, s32 arg2) {
    M572Vec3 sp10;
    s32 spC;
    M572Vec3 *sp8;

    sp8 = arg0;
    spC = arg2;
    Hu3D3Dto2D(arg0, 1, &sp10);
    return (s32) ((f32) arg1 + (((bitwise f32) sp10 / 576.0f) * (f32) (spC - arg1)));
}

s32 fn_1_5BEC(M572Vec3 *arg0, s32 arg1, s32 arg2) {
    M572Vec3 sp10;
    s32 spC;
    M572Vec3 *sp8;

    sp8 = arg0;
    spC = arg2;
    Hu3D3Dto2D(arg0, 1, &sp10);
    return (s32) ((f32) arg1 + ((sp10.y / 480.0f) * (f32) (spC - arg1)));
}

void fn_1_5CB0(M572HookOwner *arg0, s32 arg1, u32 arg2, f32 farg0) {
    u32 spC;
    f32 sp8;
    M572Item *temp_r31;

    sp8 = farg0;
    spC = arg2;
    temp_r31 = arg0->unk5C;
    if (arg1 == 8) {
        Hu3DMotionSpeedSet(temp_r31->unk0, 2.0f);
    } else {
        Hu3DMotionSpeedSet(temp_r31->unk0, 1.0f);
    }
    if ((s32) temp_r31->unk2C != arg1) {
        switch (arg1) {                             /* irregular */
        case 2:
        case 13:
            HuAudFXPlay(0x2DB);
            break;
        case 14:
            HuAudFXPlay(0x2DD);
            break;
        }
        temp_r31->unk2C = arg1;
        Hu3DMotionShiftSet(temp_r31->unk0, lbl_1_bss_4A0[arg1], 0.0f, sp8, spC);
        if (arg1 == 0xE) {
            HuAudJinglePlay(0x7A);
        }
        if (arg1 == 0xD) {
            HuAudJinglePlay(0x79);
        }
    }
}

void fn_1_5DB8(M572HookOwner *arg0, s32 arg1, u32 arg2, f32 farg0) {
    u32 spC;
    f32 sp8;
    M572Item *temp_r31;

    sp8 = farg0;
    spC = arg2;
    temp_r31 = arg0->unk5C;
    temp_r31->unk2C = arg1;
    Hu3DMotionShiftSet(temp_r31->unk0, lbl_1_bss_4A0[arg1], 0.0f, sp8, arg2);
}

void fn_1_5E34(M572HookOwner *arg0, s32 arg1) {
    M572Item *temp_r31;

    temp_r31 = arg0->unk5C;
    temp_r31->unk2C = arg1;
    Hu3DMotionSet(temp_r31->unk0, lbl_1_bss_4A0[arg1]);
}

void fn_1_5E94(M572HookOwner *arg0) {
    M572Item *temp_r31;

    temp_r31 = arg0->unk5C;
    if ((s32) (temp_r31->unk7C % 3) == 0) {
        Hu3DModelHookObjReset(temp_r31->unk0, "a-itemhook-l");
        Hu3DModelHookSet(temp_r31->unk0, "a-itemhook-r", temp_r31->unk56);
        Hu3DModelAttrReset(temp_r31->unk56, 1U);
        Hu3DModelAttrSet(temp_r31->unk58, 1U);
        return;
    }
    if ((s32) (temp_r31->unk7C % 3) == 1) {
        Hu3DModelHookObjReset(temp_r31->unk0, "a-itemhook-r");
        Hu3DModelHookSet(temp_r31->unk0, "a-itemhook-l", temp_r31->unk58);
        Hu3DModelAttrReset(temp_r31->unk58, 1U);
        Hu3DModelAttrSet(temp_r31->unk58, 0x40000002U);
        Hu3DMotionTimeSet(temp_r31->unk58, 0.0f);
        Hu3DModelAttrSet(temp_r31->unk56, 1U);
        return;
    }
    if ((s32) (temp_r31->unk7C % 3) == 2) {
        Hu3DModelHookObjReset(temp_r31->unk0, "a-itemhook-r");
        Hu3DModelHookObjReset(temp_r31->unk0, "a-itemhook-l");
        Hu3DModelAttrSet(temp_r31->unk56, 1U);
        Hu3DModelAttrSet(temp_r31->unk58, 1U);
    }
}

void fn_1_601C(M572HookOwner *arg0) {
    M572Item *temp_r31;

    temp_r31 = arg0->unk5C;
    if ((s32) (temp_r31->unk7C % 3) == 0) {
        temp_r31->unk20 = 90.0f;
        Hu3DModelHookObjReset(temp_r31->unk54, "itemhook1");
        Hu3DModelHookSet(temp_r31->unk54, lbl_1_data_124, temp_r31->unk0);
        temp_r31->unk74 = 180.0f;
        return;
    }
    if ((s32) (temp_r31->unk7C % 3) == 1) {
        temp_r31->unk20 = 270.0f;
        Hu3DModelHookObjReset(temp_r31->unk54, lbl_1_data_124);
        Hu3DModelHookSet(temp_r31->unk54, "itemhook1", temp_r31->unk0);
        temp_r31->unk74 = 0.0f;
        return;
    }
    if ((s32) (temp_r31->unk7C % 3) == 2) {
        temp_r31->unk20 = 90.0f;
        Hu3DModelHookObjReset(temp_r31->unk54, "itemhook1");
        Hu3DModelHookSet(temp_r31->unk54, lbl_1_data_124, temp_r31->unk0);
        temp_r31->unk74 = 180.0f;
    }
}

void fn_1_619C(OMOBJ *arg0) {
    s32 sp44;
    s32 sp40;
    M572Item *sp3C;
    M572Item *sp38;
    M572Item *sp34;
    M572Item *sp30;
    M572Item *sp2C;
    M572Item *sp28;
    M572Item *sp24;
    M572Item *sp20;
    M572Item *sp1C;
    M572Item *sp18;
    M572Item *sp14;
    M572Item *sp10;
    M572Item *spC;
    M572Item *sp8;
    f32 temp_f23;
    f32 temp_f24;
    f32 temp_f25;
    f32 var_f26;
    f32 var_f27;
    f32 var_f28;
    f32 var_f29;
    f32 var_f30;
    f32 var_f31;
    s32 temp_r3;
    s32 temp_r3_4;
    s32 temp_r4_3;
    s32 temp_r5;
    s32 temp_r5_3;
    u32 temp_r6;
    u32 temp_r6_2;
    u32 temp_r6_3;
    u32 temp_r6_4;
    M572Item *temp_r17;
    M572Item *temp_r18;
    M572Item *temp_r19;
    M572Item *temp_r20;
    M572Item *temp_r21;
    M572Item *temp_r22;
    M572Item *temp_r23;
    M572Item *temp_r24;
    M572Item *temp_r25;
    M572Item *temp_r26;
    M572Item *temp_r27;
    M572Item *temp_r28;
    M572Item *temp_r29;
    M572Item *temp_r31;
    M572Item *temp_r3_2;
    M572Item *temp_r3_3;
    M572Item *temp_r4;
    M572Item *temp_r4_2;
    M572Item *temp_r5_2;

    temp_r31 = arg0->data;
    temp_r6 = temp_r31->unk30;
    switch (temp_r6) {                              /* switch 1 */
    case 1:                                         /* switch 1 */
        temp_r25 = arg0->data;
        Hu3DMotionSpeedSet(temp_r25->unk0, 1.0f);
        if ((s32) temp_r25->unk2C != 2) {
            HuAudFXPlay(0x2DB);
            temp_r25->unk2C = 2;
            Hu3DMotionShiftSet(temp_r25->unk0, lbl_1_bss_4A0[2], 0.0f, 8.0f, 0U);
        }
        break;
    case 2:                                         /* switch 1 */
        if ((s32) temp_r31->unk38 == 0) {
            temp_r24 = arg0->data;
            Hu3DMotionSpeedSet(temp_r24->unk0, 1.0f);
            if ((s32) temp_r24->unk2C != 3) {
                temp_r24->unk2C = 3;
                Hu3DMotionShiftSet(temp_r24->unk0, lbl_1_bss_4A0[3], 0.0f, 8.0f, 0U);
            }
            HuAudFXPlay(0x969);
        } else if ((s32) temp_r31->unk38 > 5) {
            if ((s32) temp_r31->unk38 > 0x1E) {
                temp_r31->unk30 = 0U;
                temp_r23 = arg0->data;
                Hu3DMotionSpeedSet(temp_r23->unk0, 1.0f);
                if ((s32) temp_r23->unk2C != 0) {
                    temp_r23->unk2C = 0;
                    Hu3DMotionShiftSet(temp_r23->unk0, lbl_1_bss_4A0->unk0, 0.0f, 8.0f, 0x40000001U);
                }
                Hu3DModelAttrSet(temp_r31->unk0, 1U);
            } else {
                temp_r31->unk8 = (f32) ((f64) temp_r31->unk40 + (300.0 * sin((3.141592653589793 * (f64) temp_r31->unk3C) / 180.0)));
                temp_r31->unk3C = (f32) (temp_r31->unk3C + 3.6f);
            }
        }
        temp_r31->unk38 = (s32) (temp_r31->unk38 + 1);
        break;
    case 3:                                         /* switch 1 */
        temp_r31->unk20 = 90.0f;
        temp_r31->unkC = 0.0f;
        temp_r31->unk8 = 0.0f;
        temp_r31->unk4 = 0.0f;
        temp_r31->unk7C = 0;
        temp_r27 = arg0->data;
        if ((s32) (temp_r27->unk7C % 3) == 0) {
            temp_r27->unk20 = 90.0f;
            Hu3DModelHookObjReset(temp_r27->unk54, "itemhook1");
            Hu3DModelHookSet(temp_r27->unk54, lbl_1_data_124, temp_r27->unk0);
            temp_r27->unk74 = 180.0f;
        } else if ((s32) (temp_r27->unk7C % 3) == 1) {
            temp_r27->unk20 = 270.0f;
            Hu3DModelHookObjReset(temp_r27->unk54, lbl_1_data_124);
            Hu3DModelHookSet(temp_r27->unk54, "itemhook1", temp_r27->unk0);
            temp_r27->unk74 = 0.0f;
        } else if ((s32) (temp_r27->unk7C % 3) == 2) {
            temp_r27->unk20 = 90.0f;
            Hu3DModelHookObjReset(temp_r27->unk54, "itemhook1");
            Hu3DModelHookSet(temp_r27->unk54, lbl_1_data_124, temp_r27->unk0);
            temp_r27->unk74 = 180.0f;
        }
        temp_r29 = arg0->data;
        if ((s32) (temp_r29->unk7C % 3) == 0) {
            Hu3DModelHookObjReset(temp_r29->unk0, "a-itemhook-l");
            Hu3DModelHookSet(temp_r29->unk0, "a-itemhook-r", temp_r29->unk56);
            Hu3DModelAttrReset(temp_r29->unk56, 1U);
            Hu3DModelAttrSet(temp_r29->unk58, 1U);
        } else if ((s32) (temp_r29->unk7C % 3) == 1) {
            Hu3DModelHookObjReset(temp_r29->unk0, "a-itemhook-r");
            Hu3DModelHookSet(temp_r29->unk0, "a-itemhook-l", temp_r29->unk58);
            Hu3DModelAttrReset(temp_r29->unk58, 1U);
            Hu3DModelAttrSet(temp_r29->unk58, 0x40000002U);
            Hu3DMotionTimeSet(temp_r29->unk58, 0.0f);
            Hu3DModelAttrSet(temp_r29->unk56, 1U);
        } else if ((s32) (temp_r29->unk7C % 3) == 2) {
            Hu3DModelHookObjReset(temp_r29->unk0, "a-itemhook-r");
            Hu3DModelHookObjReset(temp_r29->unk0, "a-itemhook-l");
            Hu3DModelAttrSet(temp_r29->unk56, 1U);
            Hu3DModelAttrSet(temp_r29->unk58, 1U);
        }
        temp_r5 = (temp_r31->unk7C % 3) + 5;
        sp44 = temp_r5;
        temp_r22 = arg0->data;
        if (temp_r5 == 8) {
            Hu3DMotionSpeedSet(temp_r22->unk0, 2.0f);
        } else {
            Hu3DMotionSpeedSet(temp_r22->unk0, 1.0f);
        }
        if ((s32) temp_r22->unk2C != sp44) {
            switch (sp44) {                         /* switch 2; irregular */
            case 2:                                 /* switch 2 */
            case 13:                                /* switch 2 */
                HuAudFXPlay(0x2DB);
                break;
            case 14:                                /* switch 2 */
                HuAudFXPlay(0x2DD);
                break;
            }
            temp_r22->unk2C = sp44;
            Hu3DMotionShiftSet(temp_r22->unk0, lbl_1_bss_4A0[sp44], 0.0f, 0.0f, 0x40000002U);
            if (sp44 == 0xE) {
                HuAudJinglePlay(0x7A);
            }
            if (sp44 == 0xD) {
                HuAudJinglePlay(0x79);
            }
        }
        Hu3DMotionSpeedSet(temp_r31->unk0, 0.0f);
        temp_r31->unk68 = -60.0f;
        temp_r31->unk64 = 0.0f;
        temp_r31->unk6C = -90.0f;
        temp_r31->unk70 = 0.0f;
        temp_r31->unk74 = 180.0f;
        Hu3DModelRotSet(temp_r31->unk54, temp_r31->unk70, temp_r31->unk74, temp_r31->unk64 + temp_r31->unk68);
        Hu3DModelAttrReset(temp_r31->unk0, 1U);
        Hu3DModelAttrReset(temp_r31->unk54, 1U);
        temp_r31->unk30 = 4U;
        break;
    case 4:                                         /* switch 1 */
        temp_r31->unk68 = (f32) ((90.0 * sin((3.141592653589793 * (f64) temp_r31->unk6C) / 180.0)) / 1.5);
        temp_r31->unk6C = (f32) (temp_r31->unk6C + 1.0f);
        temp_r31->unk60 = (s32) (temp_r31->unk60 + 1);
        Hu3DModelRotSet(temp_r31->unk54, temp_r31->unk70, temp_r31->unk74, temp_r31->unk64 + temp_r31->unk68);
        if ((s32) temp_r31->unk60 > 0xB4) {
            temp_r31->unk30 = 5U;
            temp_r31->unk38 = 0;
        } else {
            if ((s32) temp_r31->unk60 == 0x1E) {
                Hu3DModelAttrReset(temp_r31->unk0, 0x40000002U);
                Hu3DMotionSpeedSet(temp_r31->unk0, 1.0f);
                if ((s32) (temp_r31->unk7C % 3) == 1) {
                    Hu3DModelAttrReset(temp_r31->unk58, 0x40000002U);
                    Hu3DModelAttrSet(temp_r31->unk58, 0U);
                }
            }
            if (((s32) temp_r31->unk7C == 1) && ((s32) temp_r31->unk60 == 0x66)) {
                HuAudFXPlay(0x96A);
            }
        }
        break;
    case 5:                                         /* switch 1 */
        if ((s32) temp_r31->unk38 > 0x1E) {
            temp_r31->unk30 = 4U;
            temp_r31->unk38 = 0;
            temp_r31->unk60 = 0;
            temp_r31->unk7C = (s32) (temp_r31->unk7C + 1);
            temp_r26 = arg0->data;
            if ((s32) (temp_r26->unk7C % 3) == 0) {
                temp_r26->unk20 = 90.0f;
                Hu3DModelHookObjReset(temp_r26->unk54, "itemhook1");
                Hu3DModelHookSet(temp_r26->unk54, lbl_1_data_124, temp_r26->unk0);
                temp_r26->unk74 = 180.0f;
            } else if ((s32) (temp_r26->unk7C % 3) == 1) {
                temp_r26->unk20 = 270.0f;
                Hu3DModelHookObjReset(temp_r26->unk54, lbl_1_data_124);
                Hu3DModelHookSet(temp_r26->unk54, "itemhook1", temp_r26->unk0);
                temp_r26->unk74 = 0.0f;
            } else if ((s32) (temp_r26->unk7C % 3) == 2) {
                temp_r26->unk20 = 90.0f;
                Hu3DModelHookObjReset(temp_r26->unk54, "itemhook1");
                Hu3DModelHookSet(temp_r26->unk54, lbl_1_data_124, temp_r26->unk0);
                temp_r26->unk74 = 180.0f;
            }
            temp_r28 = arg0->data;
            if ((s32) (temp_r28->unk7C % 3) == 0) {
                Hu3DModelHookObjReset(temp_r28->unk0, "a-itemhook-l");
                Hu3DModelHookSet(temp_r28->unk0, "a-itemhook-r", temp_r28->unk56);
                Hu3DModelAttrReset(temp_r28->unk56, 1U);
                Hu3DModelAttrSet(temp_r28->unk58, 1U);
            } else if ((s32) (temp_r28->unk7C % 3) == 1) {
                Hu3DModelHookObjReset(temp_r28->unk0, "a-itemhook-r");
                Hu3DModelHookSet(temp_r28->unk0, "a-itemhook-l", temp_r28->unk58);
                Hu3DModelAttrReset(temp_r28->unk58, 1U);
                Hu3DModelAttrSet(temp_r28->unk58, 0x40000002U);
                Hu3DMotionTimeSet(temp_r28->unk58, 0.0f);
                Hu3DModelAttrSet(temp_r28->unk56, 1U);
            } else if ((s32) (temp_r28->unk7C % 3) == 2) {
                Hu3DModelHookObjReset(temp_r28->unk0, "a-itemhook-r");
                Hu3DModelHookObjReset(temp_r28->unk0, "a-itemhook-l");
                Hu3DModelAttrSet(temp_r28->unk56, 1U);
                Hu3DModelAttrSet(temp_r28->unk58, 1U);
            }
            temp_r3 = (temp_r31->unk7C % 3) + 5;
            sp40 = temp_r3;
            temp_r21 = arg0->data;
            if (temp_r3 == 8) {
                Hu3DMotionSpeedSet(temp_r21->unk0, 2.0f);
            } else {
                Hu3DMotionSpeedSet(temp_r21->unk0, 1.0f);
            }
            if ((s32) temp_r21->unk2C != sp40) {
                switch (sp40) {                     /* switch 3; irregular */
                case 2:                             /* switch 3 */
                case 13:                            /* switch 3 */
                    HuAudFXPlay(0x2DB);
                    break;
                case 14:                            /* switch 3 */
                    HuAudFXPlay(0x2DD);
                    break;
                }
                temp_r21->unk2C = sp40;
                Hu3DMotionShiftSet(temp_r21->unk0, lbl_1_bss_4A0[sp40], 0.0f, 0.0f, 0x40000002U);
                if (sp40 == 0xE) {
                    HuAudJinglePlay(0x7A);
                }
                if (sp40 == 0xD) {
                    HuAudJinglePlay(0x79);
                }
            }
            Hu3DMotionSpeedSet(temp_r31->unk0, 0.0f);
            if ((s32) temp_r31->unk7C >= 3) {
                temp_r31->unk30 = 0U;
            }
        } else {
            temp_r31->unk38 = (s32) (temp_r31->unk38 + 1);
        }
        break;
    case 6:                                         /* switch 1 */
        if ((s32) temp_r31->unk38 == 0) {
            if (fn_1_89EC() == 0) {
                temp_r20 = arg0->data;
                Hu3DMotionSpeedSet(temp_r20->unk0, 1.0f);
                if ((s32) temp_r20->unk2C != 0xE) {
                    HuAudFXPlay(0x2DD);
                    temp_r20->unk2C = 0xE;
                    Hu3DMotionShiftSet(temp_r20->unk0, lbl_1_bss_4A0[0xE], 0.0f, 8.0f, 0U);
                    HuAudJinglePlay(0x7A);
                }
            } else {
                temp_r19 = arg0->data;
                Hu3DMotionSpeedSet(temp_r19->unk0, 1.0f);
                if ((s32) temp_r19->unk2C != 0xD) {
                    HuAudFXPlay(0x2DB);
                    temp_r19->unk2C = 0xD;
                    Hu3DMotionShiftSet(temp_r19->unk0, lbl_1_bss_4A0[0xD], 0.0f, 8.0f, 0U);
                    HuAudJinglePlay(0x79);
                }
            }
        }
        break;
    case 7:                                         /* switch 1 */
        temp_r31->unk8C = temp_r31->unk4;
        temp_r31->unk90 = temp_r31->unk8;
        temp_r31->unk94 = temp_r31->unkC;
        temp_r31->unk98 = (f32) ((temp_r31->unk80 - temp_r31->unk8C) / temp_r31->unk44);
        temp_r31->unk9C = (f32) ((temp_r31->unk84 - temp_r31->unk90) / temp_r31->unk44);
        temp_r31->unkA0 = (f32) ((temp_r31->unk88 - temp_r31->unk94) / temp_r31->unk44);
        temp_r31->unk20 = (f32) (180.0 * (atan2((f64) (temp_r31->unk80 - temp_r31->unk4), (f64) (temp_r31->unk88 - temp_r31->unkC)) / 3.141592653589793));
        temp_r31->unk30 = 8U;
        temp_r18 = arg0->data;
        Hu3DMotionSpeedSet(temp_r18->unk0, 1.0f);
        if ((s32) temp_r18->unk2C != 1) {
            temp_r18->unk2C = 1;
            Hu3DMotionShiftSet(temp_r18->unk0, lbl_1_bss_4A0[1], 0.0f, 8.0f, 0x40000001U);
        }
        break;
    case 8:                                         /* switch 1 */
        if ((s32) temp_r31->unk38 > (s32) temp_r31->unk44) {
            if ((s32) temp_r31->unk34 != -1) {
                temp_r31->unk30 = (u32) temp_r31->unk34;
                temp_r31->unk34 = -1;
                temp_r31->unk38 = 0;
            } else {
                temp_r31->unk30 = 0U;
            }
        } else {
            temp_r31->unk38 = (s32) (temp_r31->unk38 + 1);
            temp_r31->unk4 = (f32) (temp_r31->unk4 + temp_r31->unk98);
            temp_r31->unk8 = (f32) (temp_r31->unk8 + temp_r31->unk9C);
            temp_r31->unkC = (f32) (temp_r31->unkC + temp_r31->unkA0);
        }
        break;
    case 9:                                         /* switch 1 */
        temp_r31->unk3C = 0.0f;
        temp_r31->unk40 = temp_r31->unk8;
        temp_r31->unk30 = 0xAU;
        break;
    case 10:                                        /* switch 1 */
        if ((s32) temp_r31->unk38 > (s32) temp_r31->unk48) {
            if ((s32) temp_r31->unk4C > 0) {
                temp_r31->unk30 = 0xBU;
                temp_r31->unk38 = 0;
            } else if ((s32) temp_r31->unk34 != -1) {
                temp_r31->unk30 = (u32) temp_r31->unk34;
                temp_r31->unk34 = -1;
                temp_r31->unk38 = 0;
            } else {
                temp_r31->unk30 = 0U;
            }
            temp_r17 = arg0->data;
            Hu3DMotionSpeedSet(temp_r17->unk0, 1.0f);
            if ((s32) temp_r17->unk2C != 0) {
                temp_r17->unk2C = 0;
                Hu3DMotionShiftSet(temp_r17->unk0, lbl_1_bss_4A0->unk0, 0.0f, 8.0f, 0x40000001U);
            }
        } else {
            sp3C = arg0->data;
            Hu3DMotionSpeedSet(sp3C->unk0, 1.0f);
            if ((s32) sp3C->unk2C != 3) {
                sp3C->unk2C = 3;
                Hu3DMotionShiftSet(sp3C->unk0, lbl_1_bss_4A0[3], 0.0f, 8.0f, 0U);
            }
            temp_r31->unk8 = (f32) ((f64) temp_r31->unk40 + (100.0 * sin((3.141592653589793 * (f64) temp_r31->unk3C) / 180.0)));
            temp_r31->unk4 = (f32) ((f64) temp_r31->unk4 + (((f64) temp_r31->unk50 * sin((3.141592653589793 * (f64) temp_r31->unk20) / 180.0)) / (f64) temp_r31->unk48));
            temp_r31->unkC = (f32) ((f64) temp_r31->unkC + (((f64) temp_r31->unk50 * cos((3.141592653589793 * (f64) temp_r31->unk20) / 180.0)) / (f64) temp_r31->unk48));
            temp_r31->unk38 = (s32) (temp_r31->unk38 + 1);
            temp_r31->unk3C = (f32) (temp_r31->unk3C + (180.0f / temp_r31->unk48));
        }
        break;
    case 11:                                        /* switch 1 */
        if ((s32) temp_r31->unk38 > (s32) temp_r31->unk4C) {
            if ((s32) temp_r31->unk34 != -1) {
                temp_r31->unk30 = (u32) temp_r31->unk34;
                temp_r31->unk34 = -1;
                temp_r31->unk38 = 0;
            } else {
                temp_r31->unk30 = 0U;
            }
        } else {
            temp_r31->unk38 = (s32) (temp_r31->unk38 + 1);
            temp_r31->unk8 = (f32) ((f64) temp_r31->unk40 + (100.0 * sin((3.141592653589793 * (f64) (temp_r31->unk3C + temp_r31->unk38)) / 180.0)));
        }
        break;
    case 12:                                        /* switch 1 */
        temp_r5_2 = arg0->data;
        sp38 = temp_r5_2;
        Hu3DMotionSpeedSet(temp_r5_2->unk0, 1.0f);
        if ((s32) temp_r5_2->unk2C != 1) {
            sp38->unk2C = 1;
            Hu3DMotionShiftSet(sp38->unk0, lbl_1_bss_4A0[1], 0.0f, 8.0f, 0x40000001U);
        }
        if ((s32) temp_r31->unk38 > 0x1E) {
            temp_r31->unk30 = 7U;
            temp_r31->unk80 = 180.0f;
            temp_r31->unk84 = temp_r31->unk8;
            temp_r31->unk88 = -800.0f;
            temp_r31->unk44 = 0x1E;
            temp_r31->unk34 = 0xD;
            temp_r31->unk38 = 0;
        } else {
            temp_r31->unk38 = (s32) (temp_r31->unk38 + 1);
            temp_r31->unk20 = (f32) (temp_r31->unk20 + 3.6666667f);
        }
        break;
    case 13:                                        /* switch 1 */
        temp_r31->unk30 = 9U;
        temp_r31->unk34 = 0xF;
        temp_r31->unk48 = 0x28;
        temp_r31->unk4C = 0;
        temp_r31->unk50 = 200.0f;
        temp_r31->unk38 = 0;
        break;
    case 14:                                        /* switch 1 */
        temp_r31->unk30 = 7U;
        temp_r31->unk80 = 410.0f;
        temp_r31->unk84 = 0.0f;
        temp_r31->unk88 = -880.0f;
        temp_r31->unk44 = 0x1E;
        temp_r31->unk34 = 0xF;
        temp_r31->unk38 = 0;
        break;
    case 15:                                        /* switch 1 */
        sp34 = arg0->data;
        Hu3DMotionSpeedSet(sp34->unk0, 2.0f);
        if ((s32) sp34->unk2C != 8) {
            sp34->unk2C = 8;
            Hu3DMotionShiftSet(sp34->unk0, lbl_1_bss_4A0[8], 0.0f, 8.0f, 0x40000001U);
        }
        if ((s32) temp_r31->unk38 > 0xB4) {
            temp_r31->unk1C = 0.0f;
            temp_r31->unk30 = 9U;
            temp_r31->unk34 = 0x10;
            temp_r31->unk48 = 0x2D;
            temp_r31->unk4C = 0xF;
            temp_r31->unk50 = 100.0f;
            temp_r31->unk38 = 0;
        } else {
            temp_r31->unk38 = (s32) (temp_r31->unk38 + 1);
            temp_r31->unk4 = (f32) (temp_r31->unk4 + -0.075f);
            temp_r31->unk8 = (f32) (temp_r31->unk8 + 4.3333335f);
            temp_r31->unkC = (f32) (temp_r31->unkC + -0.575f);
            temp_r31->unk20 = 215.0f;
            temp_r31->unk1C = 10.0f;
        }
        break;
    case 16:                                        /* switch 1 */
        temp_r31->unk30 = 7U;
        temp_r31->unk80 = 220.0f;
        temp_r31->unk84 = 860.0f;
        temp_r31->unk88 = -1050.0f;
        temp_r31->unk44 = 0x1E;
        temp_r31->unk34 = 0x11;
        temp_r31->unk38 = 0;
        break;
    case 17:                                        /* switch 1 */
        temp_r4 = arg0->data;
        sp30 = temp_r4;
        Hu3DMotionSpeedSet(temp_r4->unk0, 1.0f);
        if ((s32) sp30->unk2C != 0xA) {
            sp30->unk2C = 0xA;
            Hu3DMotionShiftSet(sp30->unk0, lbl_1_bss_4A0[0xA], 0.0f, 8.0f, 0x40000001U);
        }
        if ((s32) temp_r31->unk38 > 0x78) {
            temp_r31->unk30 = 7U;
            temp_r31->unk80 = 10.0f;
            temp_r31->unk84 = 860.0f;
            temp_r31->unk88 = -1070.0f;
            temp_r31->unk44 = 0x1E;
            temp_r31->unk34 = 0x12;
            temp_r31->unk38 = 0;
        } else {
            temp_f25 = temp_r31->unk20;
            var_f31 = (f32) fmod((f64) (0.0f - temp_f25), 360.0);
            if (var_f31 < 0.0f) {
                var_f31 += 360.0f;
            }
            if (var_f31 > 180.0f) {
                var_f31 -= 360.0f;
            }
            var_f28 = (f32) fmod((f64) (temp_f25 + (var_f31 * 0.5f)), 360.0);
            if (var_f28 < 0.0f) {
                var_f28 += 360.0f;
            }
            temp_r31->unk20 = var_f28;
            temp_r31->unk38 = (s32) (temp_r31->unk38 + 1);
        }
        break;
    case 18:                                        /* switch 1 */
        sp2C = arg0->data;
        Hu3DMotionSpeedSet(sp2C->unk0, 2.0f);
        if ((s32) sp2C->unk2C != 8) {
            sp2C->unk2C = 8;
            Hu3DMotionShiftSet(sp2C->unk0, lbl_1_bss_4A0[8], 0.0f, 8.0f, 0x40000001U);
        }
        if ((s32) temp_r31->unk38 > 0xB4) {
            temp_r31->unk1C = 0.0f;
            temp_r31->unk30 = 9U;
            temp_r31->unk34 = 0x13;
            temp_r31->unk48 = 0x2D;
            temp_r31->unk4C = 0x14;
            temp_r31->unk50 = 100.0f;
            temp_r31->unk38 = 0;
        } else {
            temp_r31->unk38 = (s32) (temp_r31->unk38 + 1);
            temp_r31->unk8 = (f32) (temp_r31->unk8 + 3.2777777f);
            temp_r31->unk20 = -161.0f;
            temp_r31->unk1C = 10.0f;
        }
        break;
    case 19:                                        /* switch 1 */
        temp_r31->unk30 = 7U;
        temp_r31->unk80 = -180.0f;
        temp_r31->unk84 = 1450.0f;
        temp_r31->unk88 = -1150.0f;
        temp_r31->unk44 = 0x1E;
        temp_r31->unk34 = 0x15;
        temp_r31->unk40 = temp_r31->unk8;
        temp_r31->unk3C = 0.0f;
        temp_r31->unk38 = 0;
        break;
    case 20:                                        /* switch 1 */
        temp_r4_2 = arg0->data;
        sp28 = temp_r4_2;
        Hu3DMotionSpeedSet(temp_r4_2->unk0, 1.0f);
        if ((s32) sp28->unk2C != 9) {
            sp28->unk2C = 9;
            Hu3DMotionShiftSet(sp28->unk0, lbl_1_bss_4A0[9], 0.0f, 8.0f, 0x40000001U);
        }
        if ((s32) temp_r31->unk38 > 0x78) {
            temp_r31->unk30 = 0U;
            temp_r31->unk38 = 0;
        } else {
            temp_f24 = temp_r31->unk20;
            var_f30 = (f32) fmod((f64) (0.0f - temp_f24), 360.0);
            if (var_f30 < 0.0f) {
                var_f30 += 360.0f;
            }
            if (var_f30 > 180.0f) {
                var_f30 -= 360.0f;
            }
            var_f27 = (f32) fmod((f64) (temp_f24 + (var_f30 * 0.5f)), 360.0);
            if (var_f27 < 0.0f) {
                var_f27 += 360.0f;
            }
            temp_r31->unk20 = var_f27;
            temp_r31->unk38 = (s32) (temp_r31->unk38 + 1);
        }
        break;
    case 21:                                        /* switch 1 */
        if (temp_r31->unk38 > 60.0f) {
            temp_r31->unk30 = 0x16U;
            sp24 = arg0->data;
            Hu3DMotionSpeedSet(sp24->unk0, 1.0f);
            if ((s32) sp24->unk2C != 4) {
                sp24->unk2C = 4;
                Hu3DMotionShiftSet(sp24->unk0, lbl_1_bss_4A0[4], 0.0f, 8.0f, 0U);
            }
            temp_r31->unk38 = 0;
        } else {
            sp20 = arg0->data;
            Hu3DMotionSpeedSet(sp20->unk0, 1.0f);
            if ((s32) sp20->unk2C != 3) {
                sp20->unk2C = 3;
                Hu3DMotionShiftSet(sp20->unk0, lbl_1_bss_4A0[3], 0.0f, 8.0f, 0U);
            }
            temp_r31->unk8 = (f32) ((f64) temp_r31->unk40 + (450.0 * sin((3.141592653589793 * (f64) temp_r31->unk3C) / 180.0)));
            temp_r31->unk4 = (f32) ((f64) temp_r31->unk4 + ((200.0 * sin((3.141592653589793 * (f64) temp_r31->unk20) / 180.0)) / 90.0));
            temp_r31->unkC = (f32) ((f64) temp_r31->unkC + ((200.0 * cos((3.141592653589793 * (f64) temp_r31->unk20) / 180.0)) / 90.0));
            temp_r31->unk38 = (s32) (temp_r31->unk38 + 1);
            temp_r31->unk3C = (f32) (temp_r31->unk3C + 2.0f);
        }
        break;
    case 22:                                        /* switch 1 */
        temp_r31->unk38 = (s32) (temp_r31->unk38 + 1);
        if ((s32) temp_r31->unk38 > 0x1E) {
            sp1C = arg0->data;
            Hu3DMotionSpeedSet(sp1C->unk0, 1.0f);
            if ((s32) sp1C->unk2C != 9) {
                sp1C->unk2C = 9;
                Hu3DMotionShiftSet(sp1C->unk0, lbl_1_bss_4A0[9], 0.0f, 8.0f, 0x40000001U);
            }
        }
        temp_f23 = temp_r31->unk20;
        var_f29 = (f32) fmod((f64) (0.0f - temp_f23), 360.0);
        if (var_f29 < 0.0f) {
            var_f29 += 360.0f;
        }
        if (var_f29 > 180.0f) {
            var_f29 -= 360.0f;
        }
        var_f26 = (f32) fmod((f64) (temp_f23 + (var_f29 * 0.5f)), 360.0);
        if (var_f26 < 0.0f) {
            var_f26 += 360.0f;
        }
        temp_r31->unk20 = var_f26;
        break;
    case 24:                                        /* switch 1 */
        if ((s32) temp_r31->unk38 > 0x28) {
            temp_r31->unk38 = 0;
            temp_r31->unk30 = 0x19U;
        } else {
            temp_r31->unk38 = (s32) (temp_r31->unk38 + 1);
        }
        break;
    case 25:                                        /* switch 1 */
        temp_r31->unk4 = (f32) lbl_1_data_9C[temp_r31->unkA4].unk0;
        temp_r31->unk8 = (f32) lbl_1_data_9C[temp_r31->unkA4].unk4;
        temp_r31->unkC = (f32) lbl_1_data_9C[temp_r31->unkA4].unk8;
        temp_r31->unk1C = (f32) lbl_1_data_B4[temp_r31->unkA4].unk0;
        temp_r31->unk20 = (f32) lbl_1_data_B4[temp_r31->unkA4].unk4;
        temp_r31->unk24 = (f32) lbl_1_data_B4[temp_r31->unkA4].unk8;
        Hu3DModelAttrReset(temp_r31->unk0, 1U);
        temp_r31->unk30 = 0x1AU;
        temp_r3_2 = arg0->data;
        sp18 = temp_r3_2;
        temp_r3_2->unk2C = 1;
        Hu3DMotionShiftSet(temp_r3_2->unk0, lbl_1_bss_4A0[1], 0.0f, 8.0f, 0x40000001U);
        Hu3DModelAttrReset(*(temp_r31 + ((temp_r31->unkA4 * 2) + 0xA8)), 0x40000002U);
        Hu3DMotionTimeSet(*(temp_r31 + ((temp_r31->unkA4 * 2) + 0xA8)), 0.0f);
        if ((s32) temp_r31->unkA4 == 0) {
            HuAudFXPlay(0x978);
        } else if ((s32) temp_r31->unkA4 == 1) {
            HuAudFXPlay(0x979);
        }
        break;
    case 26:                                        /* switch 1 */
        temp_r5_3 = temp_r31->unkA4;
        temp_r6_2 = (u32) temp_r5_3 >> 0x1FU;
        if ((s32) (((temp_r5_3 & 1) ^ temp_r6_2) - temp_r6_2) == 0) {
            temp_r31->unk4 = (f32) (temp_r31->unk4 + 2.0f);
        } else {
            temp_r31->unk4 = (f32) (temp_r31->unk4 - 2.0f);
        }
        if ((s32) temp_r31->unk38 > 0x64) {
            temp_r31->unk38 = 0;
            temp_r31->unk30 = 0x1CU;
            temp_r4_3 = temp_r31->unkA4;
            temp_r6_3 = (u32) temp_r4_3 >> 0x1FU;
            if ((s32) (((temp_r4_3 & 1) ^ temp_r6_3) - temp_r6_3) == 0) {
                temp_r3_3 = arg0->data;
                sp14 = temp_r3_3;
                Hu3DMotionSpeedSet(temp_r3_3->unk0, 1.0f);
                if ((s32) temp_r3_3->unk2C != 0xB) {
                    sp14->unk2C = 0xB;
                    Hu3DMotionShiftSet(sp14->unk0, lbl_1_bss_4A0[0xB], 0.0f, 8.0f, 0U);
                }
                Hu3DModelHookSet(temp_r31->unk0, "a-itemhook-l", temp_r31->unkAC);
                Hu3DModelAttrReset(temp_r31->unkAC, 1U);
                Hu3DModelAttrReset(temp_r31->unkAC, 0x40000002U);
            } else {
                sp10 = arg0->data;
                Hu3DMotionSpeedSet(sp10->unk0, 1.0f);
                if ((s32) sp10->unk2C != 0xC) {
                    sp10->unk2C = 0xC;
                    Hu3DMotionShiftSet(sp10->unk0, lbl_1_bss_4A0[0xC], 0.0f, 8.0f, 0U);
                }
            }
        } else {
            temp_r31->unk38 = (s32) (temp_r31->unk38 + 1);
        }
        break;
    case 27:                                        /* switch 1 */
        temp_r3_4 = temp_r31->unkA4;
        temp_r6_4 = (u32) temp_r3_4 >> 0x1FU;
        if ((s32) (((temp_r3_4 & 1) ^ temp_r6_4) - temp_r6_4) == 0) {
            temp_r31->unk4 = (f32) (temp_r31->unk4 - 3.3333333f);
        } else {
            temp_r31->unk4 = (f32) (temp_r31->unk4 + 3.3333333f);
        }
        if ((s32) temp_r31->unk38 > 0x3C) {
            temp_r31->unk38 = 0;
            temp_r31->unk30 = 0x18U;
            temp_r31->unkA4 = (s32) (temp_r31->unkA4 + 1);
            if ((s32) temp_r31->unkA4 >= 2) {
                temp_r31->unkA4 = 0;
                spC = arg0->data;
                spC->unk2C = 0;
                Hu3DMotionShiftSet(spC->unk0, lbl_1_bss_4A0->unk0, 0.0f, 0.0f, 0U);
                temp_r31->unk30 = 0x17U;
            }
        } else {
            temp_r31->unk38 = (s32) (temp_r31->unk38 + 1);
        }
        break;
    case 28:                                        /* switch 1 */
        if ((s32) temp_r31->unk38 > 0x70) {
            temp_r31->unk38 = 0;
            temp_r31->unk30 = 0x1BU;
            sp8 = arg0->data;
            sp8->unk2C = 1;
            Hu3DMotionShiftSet(sp8->unk0, lbl_1_bss_4A0[1], 0.0f, 8.0f, 0U);
            Hu3DModelHookObjReset(temp_r31->unk0, "a-itemhook-l");
            Hu3DModelAttrSet(temp_r31->unkAC, 1U);
        } else {
            temp_r31->unk38 = (s32) (temp_r31->unk38 + 1);
        }
        if (((s32) temp_r31->unkA4 == 0) && ((s32) temp_r31->unk38 == 0x34)) {
            HuAudFXPlayPan(0x96A, 0x20);
        }
        break;
    }
    Hu3DModelPosSet(temp_r31->unk0, temp_r31->unk4, temp_r31->unk8, temp_r31->unkC);
    Hu3DModelRotSet(temp_r31->unk0, temp_r31->unk1C, temp_r31->unk20, temp_r31->unk24);
}

void fn_1_85E4(OMOBJ *arg0) {
    s32 var_r30;
    s32 var_r30_2;
    void *temp_r31;

    temp_r31 = arg0->data;
    temp_r31->unk0 = Hu3DModelCreate(HuDataSelHeapReadNum(0x7E0007, 0x10000000, HEAP_MODEL));
    switch ((u32) arg0->work[0]) {                  /* irregular */
    case 1:
        break;
    case 0:
        temp_r31->unk54 = Hu3DModelCreate(HuDataSelHeapReadNum(0x7E0017, 0x10000000, HEAP_MODEL));
        Hu3DModelAttrSet(temp_r31->unk54, 1U);
        Hu3DModelPosSet(temp_r31->unk54, 0.0f, 1050.0f, 300.0f);
        Hu3DModelRotSet(temp_r31->unk54, 0.0f, 180.0f, 0.0f);
        Hu3DMotionClusterSpeedSet(temp_r31->unk54, 0, 1.0f);
        Hu3DModelClusterAttrSet(temp_r31->unk54, 0, -0x3FFFFFFF);
        temp_r31->unk56 = Hu3DModelCreate(HuDataSelHeapReadNum(0x7E001D, 0x10000000, HEAP_MODEL));
        Hu3DModelAttrSet(temp_r31->unk56, 1U);
        temp_r31->unk58 = Hu3DModelCreate(HuDataSelHeapReadNum(0x7E001B, 0x10000000, HEAP_MODEL));
        Hu3DModelAttrSet(temp_r31->unk58, 0x40000002U);
        Hu3DModelAttrSet(temp_r31->unk58, 1U);
        break;
    case 2:
        var_r30 = 0;
loop_9:
        if (var_r30 < 2) {
            *(temp_r31 + ((var_r30 * 2) + 0xA8)) = Hu3DModelCreate(HuDataSelHeapReadNum(0x800002, 0x10000000, HEAP_MODEL));
            Hu3DModelPosSet(*(temp_r31 + ((var_r30 * 2) + 0xA8)), *(&lbl_1_data_CC + (var_r30 * 0xC)), (&lbl_1_data_CC + (var_r30 * 0xC))->unk4, (&lbl_1_data_CC + (var_r30 * 0xC))->unk8);
            Hu3DModelAttrSet(*(temp_r31 + ((var_r30 * 2) + 0xA8)), 0x40000002U);
            if (var_r30 == 0) {
                Hu3DModelScaleSet(*(temp_r31 + ((var_r30 * 2) + 0xA8)), 1.0f, 1.0f, 1.0f);
            } else {
                Hu3DModelScaleSet(*(temp_r31 + ((var_r30 * 2) + 0xA8)), -1.0f, 1.0f, 1.0f);
            }
            var_r30 += 1;
            goto loop_9;
        }
        temp_r31->unkAC = Hu3DModelCreate(HuDataSelHeapReadNum(0x7E001C, 0x10000000, HEAP_MODEL));
        Hu3DModelAttrSet(temp_r31->unkAC, 0x40000002U);
        Hu3DModelAttrSet(temp_r31->unkAC, 1U);
        break;
    }
    var_r30_2 = 0;
loop_13:
    if (var_r30_2 < 0xF) {
        lbl_1_bss_4A0[var_r30_2] = Hu3DJointMotion(temp_r31->unk0, HuDataSelHeapReadNum(lbl_1_data_34[var_r30_2], 0x10000000, HEAP_MODEL));
        var_r30_2 += 1;
        goto loop_13;
    }
    temp_r31->unk2C = 0;
    Hu3DMotionSet(temp_r31->unk0, *lbl_1_bss_4A0);
    Hu3DModelAttrSet(temp_r31->unk0, 0x40000001U);
    Hu3DModelShadowSet(temp_r31->unk0);
    temp_r31->unk4 = lbl_1_data_70.x;
    temp_r31->unk8 = lbl_1_data_70.y;
    temp_r31->unkC = lbl_1_data_70.z;
    temp_r31->unk24 = 0.0f;
    temp_r31->unk20 = 0.0f;
    temp_r31->unk1C = 0.0f;
    temp_r31->unk30 = 0;
    temp_r31->unk34 = -1;
    temp_r31->unk38 = 0;
    temp_r31->unk28 = 0.0f;
    temp_r31->unk60 = 0;
    temp_r31->unk7C = 0;
    temp_r31->unk40 = temp_r31->unk8;
    temp_r31->unk3C = 0.0f;
    temp_r31->unkA4 = 0;
    arg0->objFunc = fn_1_619C;
}

s32 fn_1_89EC(void) {
    s32 var_r30;
    s32 var_r31;

    var_r30 = 0;
    var_r31 = 0;
loop_2:
    if (var_r31 < 4) {
        var_r30 += lbl_1_bss_47C[var_r31];
        var_r31 += 1;
        goto loop_2;
    }
    return var_r30;
}

void fn_1_8A3C(OMOBJ *arg0) {
    f32 *temp_r4;
    f32 *temp_r4_2;
    f32 temp_f1;
    f32 temp_f1_2;
    s32 *temp_r4_3;
    s32 var_r31;

    if (MgSeqModeGet() >= 5U) {
        var_r31 = 0;
loop_15:
        if (var_r31 < 4) {
            MgScoreWinDispSet(lbl_1_bss_434[var_r31], 1);
            espDispOn(lbl_1_bss_42C[var_r31]);
            espDispOn(lbl_1_bss_424[var_r31]);
            espDispOn(lbl_1_bss_41C[var_r31]);
            MgScoreDispOn(lbl_1_bss_40C[var_r31]);
            if ((s32) lbl_1_bss_47C[var_r31] > 0x63) {
                MgScoreValueSet(lbl_1_bss_40C[var_r31], 0x63);
            } else {
                MgScoreValueSet(lbl_1_bss_40C[var_r31], lbl_1_bss_47C[var_r31]);
            }
            if ((s32) lbl_1_bss_45C[var_r31] != 0) {
                if ((s32) lbl_1_bss_46C[var_r31] > 0xC) {
                    lbl_1_bss_45C[var_r31] = 0;
                    lbl_1_bss_46C[var_r31] = 0;
                    lbl_1_bss_43C[var_r31] = 1.0f;
                    MgScoreDigitScaleSet(lbl_1_bss_40C[var_r31], lbl_1_bss_43C[var_r31], lbl_1_bss_43C[var_r31]);
                } else {
                    if ((s32) lbl_1_bss_46C[var_r31] < 4) {
                        temp_r4 = &lbl_1_bss_43C[var_r31];
                        temp_f1 = *temp_r4 + 0.2f;
                        *temp_r4 = temp_f1;
                        if (temp_f1 >= 1.8f) {
                            lbl_1_bss_43C[var_r31] = 1.8f;
                        }
                    } else {
                        temp_r4_2 = &lbl_1_bss_43C[var_r31];
                        temp_f1_2 = *temp_r4_2 - 0.1f;
                        *temp_r4_2 = temp_f1_2;
                        if (temp_f1_2 <= 1.0f) {
                            lbl_1_bss_43C[var_r31] = 1.0f;
                        }
                    }
                    temp_r4_3 = &lbl_1_bss_46C[var_r31];
                    *temp_r4_3 += 1;
                    MgScoreDigitScaleSet(lbl_1_bss_40C[var_r31], lbl_1_bss_43C[var_r31], lbl_1_bss_43C[var_r31]);
                }
            }
            var_r31 += 1;
            goto loop_15;
        }
    }
}

void fn_1_8D6C(OMOBJ *arg0) {
    s32 var_r31;

    var_r31 = 0;
loop_7:
    if (var_r31 < 4) {
        lbl_1_bss_434[var_r31] = MgScoreWinCreate(0x70, 0x28);
        MgScoreWinTPLvlSet(lbl_1_bss_434[var_r31], 0.85f);
        MgScoreWinPosSet(lbl_1_bss_434[var_r31], lbl_1_data_7C[var_r31].unk0, lbl_1_data_7C[var_r31].unk4);
        MgScoreWinDispSet(lbl_1_bss_434[var_r31], 0);
        lbl_1_bss_42C[var_r31] = espEntry(lbl_1_data_0[lbl_1_bss_490[var_r31]], 0x63, 0);
        espDispOff(lbl_1_bss_42C[var_r31]);
        espDrawNoSet(lbl_1_bss_42C[var_r31], 0);
        espPosSet(lbl_1_bss_42C[var_r31], lbl_1_data_7C[var_r31].unk0 - 38.0f, lbl_1_data_7C[var_r31].unk4);
        espAttrSet(lbl_1_bss_42C[var_r31], 1U);
        if ((s32) lbl_1_bss_48C == 0) {
            lbl_1_bss_424[var_r31] = espEntry(0x7E001EU, 0x63, 0);
        } else if ((s32) lbl_1_bss_48C == 1) {
            lbl_1_bss_424[var_r31] = espEntry(0x7F0014U, 0x63, 0);
        } else {
            lbl_1_bss_424[var_r31] = espEntry(0x800006U, 0x63, 0);
        }
        espDispOff(lbl_1_bss_424[var_r31]);
        espDrawNoSet(lbl_1_bss_424[var_r31], 0);
        espPosSet(lbl_1_bss_424[var_r31], lbl_1_data_7C[var_r31].unk0 - 15.0f, lbl_1_data_7C[var_r31].unk4);
        espAttrSet(lbl_1_bss_424[var_r31], 1U);
        lbl_1_bss_41C[var_r31] = espEntry(0x960030U, 0x63, 0xF);
        espDispOff(lbl_1_bss_41C[var_r31]);
        espDrawNoSet(lbl_1_bss_41C[var_r31], 0);
        espPosSet(lbl_1_bss_41C[var_r31], 4.0f + lbl_1_data_7C[var_r31].unk0, lbl_1_data_7C[var_r31].unk4);
        espAttrSet(lbl_1_bss_41C[var_r31], 1U);
        lbl_1_bss_40C[var_r31] = MgScoreCreate(0x960031, -1, 1);
        MgScoreValueSet(lbl_1_bss_40C[var_r31], 0);
        MgScorePosSet(lbl_1_bss_40C[var_r31], 22.0f + lbl_1_data_7C[var_r31].unk0, lbl_1_data_7C[var_r31].unk4);
        MgScoreMaxDigitSet(lbl_1_bss_40C[var_r31], 2);
        MgScoreDispOff(lbl_1_bss_40C[var_r31]);
        MgScorePriSet(lbl_1_bss_40C[var_r31], 0x63);
        lbl_1_bss_45C[var_r31] = 0;
        lbl_1_bss_46C[var_r31] = 0;
        lbl_1_bss_44C[var_r31] = 0;
        lbl_1_bss_43C[var_r31] = 1.0f;
        var_r31 += 1;
        goto loop_7;
    }
    arg0->objFunc = fn_1_8A3C;
}

void fn_1_92A0(OMOBJ *arg0) {
    if ((s32) lbl_1_bss_4C0 != 0) {
        MgActorExec();
    }
}

void fn_1_92D4(void) {
    lbl_1_bss_4C0 = 1;
    lbl_1_bss_4C4 = omAddObjEx(lbl_1_bss_6D4, 0x64, 0U, 0U, -1, fn_1_92A0);
}

void fn_1_9334(s32 arg0) {
    lbl_1_bss_4C0 = arg0;
}

void fn_1_9344(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sp18;
    s32 sp14;
    s32 sp10;
    s32 spC;
    s32 sp8;

    sp8 = arg0;
    spC = arg1;
    sp10 = arg2;
    sp14 = arg3;
    sp18 = arg4;
    lbl_1_bss_490->unk0 = sp8;
    lbl_1_bss_490[1] = spC;
    lbl_1_bss_490[2] = sp10;
    lbl_1_bss_490[3] = sp14;
    lbl_1_bss_48C = sp18;
    omAddObjEx(lbl_1_bss_6D4, 0x65, 0U, 0U, -1, fn_1_8D6C);
}

void fn_1_93EC(s32 arg0, s32 arg1) {
    lbl_1_bss_47C[arg0] = arg1;
}

void fn_1_9404(s32 arg0) {
    s32 var_r31;

    if (arg0 == 0) {
        lbl_1_bss_3B8 = Hu3DModelCreate(HuDataSelHeapReadNum(0x7E001A, 0x10000000, HEAP_MODEL));
    } else if (arg0 == 1) {
        lbl_1_bss_3B8 = Hu3DModelCreate(HuDataSelHeapReadNum(0x7F0011, 0x10000000, HEAP_MODEL));
    } else {
        lbl_1_bss_3B8 = Hu3DModelCreate(HuDataSelHeapReadNum(0x800003, 0x10000000, HEAP_MODEL));
    }
    Hu3DModelAttrSet(lbl_1_bss_3B8, 1U);
    var_r31 = 0;
loop_7:
    if (var_r31 < 0x14) {
        lbl_1_bss_3BC[var_r31] = NULL;
        var_r31 += 1;
        goto loop_7;
    }
}

void fn_1_94F8(OMOBJ *arg0) {
    void *sp8;
    s32 var_r30;

    sp8 = arg0->data;
    Hu3DModelAttrSet(*arg0->mdlId, 1U);
    Hu3DModelKill(*arg0->mdlId);
    lbl_1_bss_3B4 -= 1;
    var_r30 = 0;
loop_4:
    if (var_r30 < 0x14) {
        if (arg0 == (OMOBJ *) lbl_1_bss_3BC[var_r30]) {
            lbl_1_bss_3BC[var_r30] = NULL;
        } else {
            var_r30 += 1;
            goto loop_4;
        }
    }
    omDelObjEx(lbl_1_bss_6D4, arg0);
}

void fn_1_95C8(OMOBJ *arg0) {
    void *sp8;
    s32 *temp_r4;
    s32 var_r29;
    M572Score *temp_r31;

    temp_r31 = arg0->data;
    if ((s32) temp_r31->unk0 != 0) {

    } else {
        Hu3D2Dto3D(temp_r31 + 0x38, 1, temp_r31 + 0x20);
        temp_r31->unk2C = (f32) ((temp_r31->unk20 - temp_r31->unk8) / (30.0f - temp_r31->unk4));
        temp_r31->unk30 = (f32) ((temp_r31->unk24 - temp_r31->unkC) / (30.0f - temp_r31->unk4));
        temp_r31->unk34 = (f32) ((temp_r31->unk28 - temp_r31->unk10) / (30.0f - temp_r31->unk4));
        temp_r31->unk8 = (f32) (temp_r31->unk8 + temp_r31->unk2C);
        temp_r31->unkC = (f32) (temp_r31->unkC + temp_r31->unk30);
        temp_r31->unk10 = (f32) (temp_r31->unk10 + temp_r31->unk34);
        temp_r31->unk18 = (f32) (temp_r31->unk18 + 10.0f);
        temp_r31->unk4 = (s32) (temp_r31->unk4 + 1);
        if (temp_r31->unk4 > 30.0f) {
            temp_r31->unkC = temp_r31->unk24;
            temp_r31->unkC = temp_r31->unk24;
            temp_r31->unkC = temp_r31->unk24;
            sp8 = arg0->data;
            Hu3DModelAttrSet(*arg0->mdlId, 1U);
            Hu3DModelKill(*arg0->mdlId);
            lbl_1_bss_3B4 -= 1;
            var_r29 = 0;
loop_7:
            if (var_r29 < 0x14) {
                if (arg0 == (OMOBJ *) lbl_1_bss_3BC[var_r29]) {
                    lbl_1_bss_3BC[var_r29] = NULL;
                } else {
                    var_r29 += 1;
                    goto loop_7;
                }
            }
            omDelObjEx(lbl_1_bss_6D4, arg0);
            temp_r4 = &lbl_1_bss_47C[temp_r31->unk44];
            *temp_r4 += 1;
            lbl_1_bss_45C[temp_r31->unk44] = 1;
            HuAudFXPlay(0x96D);
        }
    }
    omSetTra(arg0, temp_r31->unk8, temp_r31->unkC, temp_r31->unk10);
    omSetRot(arg0, temp_r31->unk14, temp_r31->unk18, temp_r31->unk1C);
}

void fn_1_98BC(OMOBJ *arg0) {
    s32 var_r29;
    M572Score *temp_r31;

    temp_r31 = arg0->data;
    *arg0->mdlId = Hu3DModelLink(lbl_1_bss_3B8);
    omSetTra(arg0, temp_r31->unk8, temp_r31->unkC, temp_r31->unk10);
    omSetRot(arg0, temp_r31->unk14, temp_r31->unk18, temp_r31->unk1C);
    omSetSca(arg0, 1.0f, 1.0f, 1.0f);
    var_r29 = 0;
loop_4:
    if (var_r29 < 0x14) {
        if ((OMOBJ *) lbl_1_bss_3BC[var_r29] == NULL) {
            lbl_1_bss_3BC[var_r29] = arg0;
        } else {
            var_r29 += 1;
            goto loop_4;
        }
    }
    Hu3DModelAttrReset(*arg0->mdlId, 1U);
    arg0->objFunc = fn_1_95C8;
}

void fn_1_99CC(M572Vec3 *arg0, s32 arg1) {
    OMOBJ *temp_r29;
    M572Score *temp_r31;

    lbl_1_bss_3B4 += 1;
    temp_r29 = omAddObjEx(lbl_1_bss_6D4, 0x66, 1U, 0U, -1, fn_1_98BC);
    temp_r29->data = HuMemDirectMallocNum(HEAP_HEAP, 0x48, 0x10000000U);
    temp_r31 = temp_r29->data;
    temp_r31->unk0 = 0;
    temp_r31->unk4 = 0;
    temp_r31->unk38 = (f32) lbl_1_data_7C[arg1].unk0;
    temp_r31->unk3C = (f32) (20.0f + lbl_1_data_7C[arg1].unk4);
    temp_r31->unk40 = 1500.0f;
    temp_r31->unk1C = 0.0f;
    temp_r31->unk18 = 0.0f;
    temp_r31->unk14 = 0.0f;
    temp_r31->unk8 = arg0->unk0;
    temp_r31->unkC = arg0->unk4;
    temp_r31->unk10 = arg0->unk8;
    temp_r31->unk34 = 0.0f;
    temp_r31->unk24 = 0.0f;
    temp_r31->unk2C = 0.0f;
    temp_r31->unk44 = arg1;
}

void fn_1_9B1C(OMOBJ *arg0, s16 arg_sp0) {
    OMOBJ *temp_r29;
    s32 temp_r28;
    void *temp_r30;
    void *temp_r31;

    temp_r30 = arg0->data;
    if ((s32) (temp_r30->unk4 % 15) == 0) {
        temp_r28 = temp_r30->unk8;
        lbl_1_bss_3B4 += 1;
        temp_r29 = omAddObjEx(lbl_1_bss_6D4, 0x66, 1U, 0U, -1, fn_1_98BC);
        temp_r29->data = HuMemDirectMallocNum(HEAP_HEAP, 0x48, 0x10000000U);
        temp_r31 = temp_r29->data;
        temp_r31->unk0 = 0;
        temp_r31->unk4 = 0;
        temp_r31->unk38 = (f32) lbl_1_data_7C[temp_r28].unk0;
        temp_r31->unk3C = (f32) (20.0f + lbl_1_data_7C[temp_r28].unk4);
        temp_r31->unk40 = 1500.0f;
        temp_r31->unk1C = 0.0f;
        temp_r31->unk18 = 0.0f;
        temp_r31->unk14 = 0.0f;
        temp_r31->unk8 = temp_r30->unkC;
        temp_r31->unkC = temp_r30->unk10;
        temp_r31->unk10 = temp_r30->unk14;
        temp_r31->unk34 = 0.0f;
        temp_r31->unk24 = 0.0f;
        temp_r31->unk2C = 0.0f;
        temp_r31->unk44 = temp_r28;
        temp_r30->unk0 = (s32) (temp_r30->unk0 - 1);
        if ((s32) temp_r30->unk0 <= 0) {
            omDelObjEx(lbl_1_bss_6D4, arg0);
        }
    }
    temp_r30->unk4 = (s32) (temp_r30->unk4 + 1);
}

void fn_1_9CC8(M572Vec3 *arg0, s32 arg1, s32 arg2) {
    s32 spC;
    s32 sp8;
    OMOBJ *temp_r29;
    M572Spawner *temp_r31;

    sp8 = arg1;
    spC = arg2;
    temp_r29 = omAddObjEx(lbl_1_bss_6D4, 0x66, 0U, 0U, -1, fn_1_9B1C);
    temp_r29->data = HuMemDirectMallocNum(HEAP_HEAP, 0x18, 0x10000000U);
    temp_r31 = temp_r29->data;
    temp_r31->unkC = arg0->unk0;
    temp_r31->unk10 = arg0->unk4;
    temp_r31->unk14 = arg0->unk8;
    temp_r31->unk8 = sp8;
    temp_r31->unk0 = spC;
    temp_r31->unk4 = 0;
}

s32 fn_1_9D7C(void) {
    s32 var_r31;

    var_r31 = 0;
loop_4:
    if (var_r31 >= 0x14) {
        return 1;
    }
    if ((OMOBJ *) lbl_1_bss_3BC[var_r31] != NULL) {
        return 1;
    }
    var_r31 += 1;
    goto loop_4;
}

void fn_1_9DD0(void) {
    lbl_1_bss_3B0 = -1;
}

void fn_1_9DE4(void) {
    lbl_1_bss_3B0 = HuAudSStreamPlay(0x3D);
}

void fn_1_9E14(void) {
    HuAudSStreamFadeOut(lbl_1_bss_3B0, 0x64);
}

s32 fn_1_9E44(void *arg0, void *arg1, f32 farg0, f32 farg1) {
    f32 sp8;
    f32 temp_f29;
    f32 temp_f30;
    f32 temp_f31;
    f32 var_f27;
    f64 temp_f28;
    f64 temp_f28_2;
    f64 temp_f28_3;

    temp_f31 = arg0->unk0 - arg1->unk0;
    temp_f30 = arg0->unk4 - arg1->unk4;
    temp_f29 = arg0->unk8 - arg1->unk8;
    if (((temp_f29 * temp_f29) + ((temp_f31 * temp_f31) + (temp_f30 * temp_f30))) > 0.0f) {
        temp_f28 = __frsqrte((temp_f29 * temp_f29) + ((temp_f31 * temp_f31) + (temp_f30 * temp_f30)));
        temp_f28_2 = 0.5 * temp_f28 * (3.0 - ((f64) ((temp_f29 * temp_f29) + ((temp_f31 * temp_f31) + (temp_f30 * temp_f30))) * (temp_f28 * temp_f28)));
        temp_f28_3 = 0.5 * temp_f28_2 * (3.0 - ((f64) ((temp_f29 * temp_f29) + ((temp_f31 * temp_f31) + (temp_f30 * temp_f30))) * (temp_f28_2 * temp_f28_2)));
        sp8 = (f32) ((f64) ((temp_f29 * temp_f29) + ((temp_f31 * temp_f31) + (temp_f30 * temp_f30))) * (0.5 * temp_f28_3 * (3.0 - ((f64) ((temp_f29 * temp_f29) + ((temp_f31 * temp_f31) + (temp_f30 * temp_f30))) * (temp_f28_3 * temp_f28_3)))));
        var_f27 = sp8;
    } else {
        var_f27 = (temp_f29 * temp_f29) + ((temp_f31 * temp_f31) + (temp_f30 * temp_f30));
    }
    if ((farg0 + farg1) > var_f27) {
        return 1;
    }
    return 0;
}

s32 fn_1_A03C(M572PlanePt *arg0, M572PlanePt *arg1, f32 farg0, f32 farg1) {
    if ((((arg1->x - arg0->x) * (arg1->x - arg0->x)) + ((arg1->z - arg0->z) * (arg1->z - arg0->z))) < ((farg0 + farg1) * (farg0 + farg1))) {
        return 1;
    }
    return 0;
}

f32 fn_1_A09C(f32 farg0, f32 farg1, f32 farg2, f32 farg3) {
    f32 sp8;
    f32 temp_f30;
    f32 temp_f31;
    f32 var_f28;
    f64 temp_f29;
    f64 temp_f29_2;
    f64 temp_f29_3;

    temp_f31 = farg0 - farg2;
    temp_f30 = farg1 - farg3;
    if (((temp_f31 * temp_f31) + (temp_f30 * temp_f30)) > 0.0f) {
        temp_f29 = __frsqrte((temp_f31 * temp_f31) + (temp_f30 * temp_f30));
        temp_f29_2 = 0.5 * temp_f29 * (3.0 - ((f64) ((temp_f31 * temp_f31) + (temp_f30 * temp_f30)) * (temp_f29 * temp_f29)));
        temp_f29_3 = 0.5 * temp_f29_2 * (3.0 - ((f64) ((temp_f31 * temp_f31) + (temp_f30 * temp_f30)) * (temp_f29_2 * temp_f29_2)));
        sp8 = (f32) ((f64) ((temp_f31 * temp_f31) + (temp_f30 * temp_f30)) * (0.5 * temp_f29_3 * (3.0 - ((f64) ((temp_f31 * temp_f31) + (temp_f30 * temp_f30)) * (temp_f29_3 * temp_f29_3)))));
        var_f28 = sp8;
    } else {
        var_f28 = (temp_f31 * temp_f31) + (temp_f30 * temp_f30);
    }
    return var_f28;
}

s32 fn_1_A20C(s32 arg0, s32 arg1) {
    s32 sp8;

    sp8 = arg1;
    return arg0 + frandmod(sp8 - arg0);
}

s32 fn_1_A248(M572Vec3 *arg0) {
    M572Vec3 spC;
    M572Vec3 *sp8;

    sp8 = arg0;
    Hu3D3Dto2D(arg0, 1, &spC);
    return (s32) (32.0f + (64.0f * ((bitwise f32) spC / 576.0f)));
}

s32 fn_1_A2C4(M572Vec3 *arg0) {
    M572Vec3 spC;
    M572Vec3 *sp8;

    sp8 = arg0;
    Hu3D3Dto2D(arg0, 1, &spC);
    return (s32) (96.0f + (31.0f * (spC.y / 480.0f)));
}
