// REL/m528Dll/m528.c - decompilation of the m528Dll/REL/m528Dll/m528 unit.
//
// m528 is a Mario Party 5 two-versus-two minigame REL: a roulette wheel
// (M528SEG x16) spins, the four players (M528PLAYER) jump on the two teams'
// pedals (M528SLOT) to make the team's side tilt, and the sector the wheel
// stops on decides the winning team.  ObjectSetup creates one main object that
// owns M528MAIN (OMOBJ.data) plus four player objects owning M528PLAYER.
//
// Codegen notes, all measured on the first functions of this unit:
//  - the unit is compiled -O0,p, so dead stores to stack locals survive and a
//    `volatile` local is the honest way to reproduce one (fn_1_111C, fn_1_1368);
//  - objdiff prints rlwinm mask fields MSB-first, so `rlwinm r3, r0, 0, 23, 23`
//    extracts value bit 8, i.e. `x & 0x100`;
//  - `--s16field` avoids the extsh that `s16field = s16field - 1` inserts.

#include "game/charman.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/hsfformat.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mg/seqman.h"
#include "game/object.h"
#include "game/pad.h"
#include "humath.h"
#include "math.h"

#ifndef __MWERKS__
#include "game/audio.h"
#include "game/frand.h"
#else
/* The original calls frandmod with an implicit int return, which is what makes
 * MWCC convert it with the signed int -> float magic double. */
extern s32 frandmod();
#endif

#ifdef __MWERKS__
#define M528_RODATA __declspec(section ".rodata")
#else
#define M528_RODATA
#endif

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

struct m528Main_s;
struct m528Player_s;
struct m528Team_s;
struct m528Node_s;

/* Pedal of a team: the model a player stands on. */
typedef struct m528Slot_s {
    /* 0x00 */ struct m528Team_s *team;
    /* 0x04 */ HuVecF pos;
    /* 0x10 */ f32 alpha;
    /* 0x14 */ f32 active;
    /* 0x18 */ s16 waitFrames;
    /* 0x1A */ s16 mdl;
    /* 0x1C */ s16 hit;
    /* 0x1E */ s16 pad1E;
} M528SLOT;

/* One sector of the roulette wheel. */
typedef struct m528Seg_s {
    /* 0x00 */ HuVecF pos;
    /* 0x0C */ HuVecF pos2;
    /* 0x18 */ f32 progress;
    /* 0x1C */ f32 angle;
    /* 0x20 */ s16 mdl[3];
    /* 0x26 */ s16 state;
    /* 0x28 */ s16 timer;
    /* 0x2A */ s16 type;
} M528SEG;

typedef s32 (*M528SegFunc)(M528SEG *);

/* Team seesaw: two pedals, the board they stand on and its wheel sector. */
typedef struct m528Team_s {
    /* 0x00 */ M528SLOT slot[2];
    /* 0x40 */ f32 spin;
    /* 0x44 */ s16 spinMdl;
    /* 0x46 */ s16 pad46;
    /* 0x48 */ HuVecF pivot;
    /* 0x54 */ M528SEG *seg;
    /* 0x58 */ f32 tilt;
    /* 0x5C */ f32 unk5C;
    /* 0x60 */ s16 mdl;
    /* 0x62 */ s16 state;
    /* 0x64 */ s16 timer;
    /* 0x66 */ s16 sndId;
} M528TEAM;

typedef s32 (*M528TeamFunc)(M528TEAM *);

typedef struct m528Cam_s {
    /* 0x00 */ f32 aimX;
    /* 0x04 */ f32 aimY;
    /* 0x08 */ f32 aimZ;
    /* 0x0C */ f32 rotX;
    /* 0x10 */ f32 rotY;
    /* 0x14 */ f32 dist;
    /* 0x18 */ s16 state;
    /* 0x1A */ s16 timer;
} M528CAM;

typedef s32 (*M528CamFunc)(M528CAM *);

/* CPU control state of a player. */
typedef struct m528Ai_s {
    /* 0x0 */ s16 state;
    /* 0x2 */ s16 timer;
    /* 0x4 */ s16 wait;
} M528AI;

typedef struct m528Player_s {
    /* 0x00 */ HuVecF pos;
    /* 0x0C */ HuVecF rot;
    /* 0x18 */ OMOBJ *obj;
    /* 0x1C */ s32 (*hook)(struct m528Player_s *);
    /* 0x20 */ s32 state;
    /* 0x24 */ s32 timer;
    /* 0x28 */ s16 playerNo;
    /* 0x2A */ s16 charNo;
    /* 0x2C */ s16 grp;
    /* 0x2E */ s16 padNo;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s16 type;
    /* 0x36 */ s16 dif;
    /* 0x38 */ s16 unk38;
    /* 0x3A */ s16 holdFrames;
    /* 0x3C */ s16 strong;
    /* 0x3E */ s16 onSlot;
    /* 0x40 */ f32 peakY;
    /* 0x44 */ f32 baseY;
    /* 0x48 */ f32 speed;
    /* 0x4C */ struct m528Player_s *partner;
    /* 0x50 */ M528AI ai;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ M528SLOT *slot;
} M528PLAYER;

/* Debris/confetti particle driven through the M528NODE list. */
typedef struct m528Particle_s {
    /* 0x00 */ HuVecF pos;
    /* 0x0C */ HuVecF rot;
    /* 0x18 */ HuVecF vel;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ s16 state;
    /* 0x32 */ s16 timer;
    /* 0x34 */ s16 duration;
    /* 0x36 */ s16 mdl;
} M528PARTICLE;

/* Doubly linked list node: the callback is called with the node itself and a
 * non-zero result releases it. */
typedef struct m528Node_s {
    /* 0x00 */ struct m528Node_s *prev;
    /* 0x04 */ struct m528Node_s *next;
    /* 0x08 */ s32 (*func)(struct m528Node_s *);
    /* 0x0C */ M528PARTICLE *data;
} M528NODE;

/* Model that walks along the lbl_1_bss_4 path. */
typedef struct m528Walker_s {
    /* 0x00 */ HuVecF pos;
    /* 0x0C */ HuVecF rot;
    /* 0x18 */ f32 angFrom;
    /* 0x1C */ f32 angTo;
    /* 0x20 */ s16 mdl;
    /* 0x22 */ s16 state;
    /* 0x24 */ s16 timer;
    /* 0x26 */ s16 idx;
    /* 0x28 */ s16 delay;
    /* 0x2A */ s16 count;
} M528WALKER;

/* Per-sequence working state: OMOBJ.data of the main object. */
typedef struct m528Main_s {
    /* 0x0000 */ M528PLAYER *player[4];
    /* 0x0010 */ M528PLAYER *teamPlayer[2][2];
    /* 0x0020 */ M528CAM cam;
    /* 0x003C */ M528TEAM team[2];
    /* 0x010C */ M528SEG seg[16];
    /* 0x03CC */ M528CamFunc camFunc;
    /* 0x03D0 */ M528TeamFunc teamFunc[2];
    /* 0x03D8 */ M528SegFunc segFunc;
    /* 0x03DC */ M528NODE *nodeUsed;
    /* 0x03E0 */ M528NODE *nodeFree;
    /* 0x03E4 */ M528NODE node[256];
    /* 0x13E4 */ M528PARTICLE particle[256];
    /* 0x4BE4 */ M528WALKER walker[4];
    /* 0x4C94 */ f32 spinPower;
    /* 0x4C98 */ f32 wheelAngle;
    /* 0x4C9C */ f32 wheelSpeed;
    /* 0x4CA0 */ f32 wheelTarget;
    /* 0x4CA4 */ f32 unk4CA4;
    /* 0x4CA8 */ s16 sel[2];
    /* 0x4CAC */ s16 mode;
    /* 0x4CAE */ s16 timer;
    /* 0x4CB0 */ s16 winner;
    /* 0x4CB2 */ s16 inputOn;
    /* 0x4CB4 */ s16 mdlCache[0x36];
    /* 0x4D20 */ s16 mtnCache[7];
    /* 0x4D2E */ s16 segMdlCache[16];
    /* 0x4D4E */ s16 segMdlCache2[16];
    /* 0x4D6E */ s16 fadeFlag;
} M528MAIN;

/* Load model data[dataIdx] into the cache slot, or link the cached one. */
#define M528_GETMDL(var, slot, dataIdx)                                       \
    do {                                                                      \
        if ((slot) < 0) {                                                     \
            var = slot = Hu3DModelCreateData(lbl_1_data_1C8[dataIdx]);        \
        } else {                                                              \
            var = Hu3DModelLink(slot);                                        \
        }                                                                     \
    } while (0)

#define M528_GETMTN(var, slot, mdl, dataNum)                                  \
    do {                                                                      \
        if ((slot) < 0) {                                                     \
            var = slot = Hu3DJointMotionData(mdl, dataNum);                   \
        } else {                                                              \
            var = (slot);                                                     \
        }                                                                     \
    } while (0)

/* ---- function prototypes ---- */
void ObjectSetup(void);
void fn_1_1E8(s16 mode, s16 frameNo);
void fn_1_238(s16 mode, s16 frameNo);
void fn_1_8E4(s16 mode, s16 frameNo);
void fn_1_954(s16 mode, s16 frameNo);
void fn_1_9CC(s16 mode, s16 frameNo);
void fn_1_B10(s16 mode, s16 frameNo);
void fn_1_ED0(s16 mode, s16 frameNo);
void fn_1_1118(s16 mode, s16 frameNo);
void fn_1_111C(s16 mode, s16 frameNo);
void fn_1_113C(OMOBJ *obj);
void fn_1_1368(OMOBJ *obj);
void fn_1_13D0(OMOBJ *obj);
void fn_1_1458(OMOBJ *obj);
void fn_1_1538(M528MAIN *work);
void fn_1_15EC();
void fn_1_16C4();
void fn_1_17B0();
void fn_1_1874(M528MAIN *work);
void fn_1_18E0(M528MAIN *work);
void fn_1_1918(M528CAM *cam);
void fn_1_198C(M528CAM *cam, s32 idx);
void fn_1_1A20(M528CAM *cam);
void fn_1_1CAC(M528MAIN *work);
s32 fn_1_1DFC(M528PLAYER *player);
s32 fn_1_1ECC(M528PLAYER *player);
s32 fn_1_1F8C(M528PLAYER *player);
s32 fn_1_2080(M528PLAYER *player);
s32 fn_1_22B4(M528PLAYER *player);
s32 fn_1_237C(M528PLAYER *player);
s32 fn_1_2488(M528PLAYER *player);
s32 fn_1_2564(M528PLAYER *player);
s32 fn_1_25FC(M528PLAYER *player);
s32 fn_1_2680(M528PLAYER *player);
s32 fn_1_2704(M528PLAYER *player);
s32 fn_1_2758(M528PLAYER *player);
s32 fn_1_27AC(M528PLAYER *player);
s32 fn_1_2A00(M528PLAYER *player);
s16 fn_1_2C40(s16 arg0, s32 arg1);
s16 fn_1_2D7C(M528PLAYER *player);
s16 fn_1_2E3C(M528PLAYER *player);
s32 fn_1_2EC0(M528TEAM *team);
void fn_1_32D0(M528SLOT *slot);
void fn_1_3330(M528MAIN *work);
void fn_1_36DC(M528SEG *seg);
s32 fn_1_3750(M528SEG *seg, f32 farg0);
void fn_1_38F8(M528MAIN *work);
void fn_1_3A00(M528MAIN *work);
s32 fn_1_3CA0(M528CAM *cam);
s32 fn_1_3EFC(M528CAM *cam);
s32 fn_1_4158(M528CAM *cam);
s32 fn_1_43B4(M528CAM *cam);
s32 fn_1_4540(M528CAM *cam);
void fn_1_46CC(M528MAIN *work);
s32 fn_1_47F0(M528TEAM *team);
s32 fn_1_495C(M528TEAM *team);
s32 fn_1_4B04(M528SEG *seg);
s32 fn_1_4C84(M528SEG *seg);
void fn_1_4E80(M528MAIN *work);
void fn_1_4F30(M528MAIN *work);
M528NODE *fn_1_4FF4(M528MAIN *work, s32 (*func)(M528NODE *), M528PARTICLE *data);
void fn_1_5048(M528MAIN *work, M528NODE *node);
void fn_1_50B0(M528MAIN *work);
void fn_1_5194(M528PARTICLE *particle);
void fn_1_51A8(M528MAIN *work);
void fn_1_5BF8(M528MAIN *work);
void fn_1_6320(M528MAIN *work);
void fn_1_6410(M528MAIN *work);
void fn_1_654C(M528MAIN *work);
void fn_1_6688(M528MAIN *work);
void fn_1_67C4(M528MAIN *work);
void fn_1_6900(M528MAIN *work);
void fn_1_6A3C(M528MAIN *work);
void fn_1_6B78(M528MAIN *work);
void fn_1_6CB4(M528MAIN *work);
s32 fn_1_6DF0(M528NODE *node);
s32 fn_1_72E4(M528NODE *node);
s32 fn_1_7794(M528NODE *node);
s32 fn_1_7B44(M528NODE *node);
s32 fn_1_800C(M528NODE *node);
s32 fn_1_83E4(M528NODE *node);
s32 fn_1_87F4(M528NODE *node);
s32 fn_1_8CD0(M528NODE *node);
s32 fn_1_8FFC(M528NODE *node);
s32 fn_1_97C8(M528NODE *node);
s32 fn_1_9D5C(M528NODE *node);
s32 fn_1_A3A0(M528NODE *node);
s32 fn_1_ABAC(M528NODE *node);
void fn_1_B384(M528MAIN *work);
void fn_1_B528(M528WALKER *walker, s16 delay);
void fn_1_B6D4(M528WALKER *walker);
void fn_1_C314(M528MAIN *work);
void fn_1_C3DC(M528MAIN *work);
void fn_1_C530(M528MAIN *work);
void fn_1_C728(M528MAIN *work);
void fn_1_C8B4(M528MAIN *work);
void fn_1_CA34(M528MAIN *work);
void fn_1_CAEC(M528MAIN *work);
void fn_1_CC40(M528MAIN *work);

/* ---- data (m528) ---- */
static MGSEQ_PARAM lbl_1_data_0 = {
    0x12C, 0, fn_1_1E8, fn_1_238, fn_1_8E4, fn_1_954, fn_1_9CC, fn_1_B10, fn_1_ED0, fn_1_1118, fn_1_111C,
};

#define M528_MOT(b) \
    { ((b) << 16) | 0, ((b) << 16) | 3, ((b) << 16) | 4, ((b) << 16) | 8, ((b) << 16) | 0xA, ((b) << 16) | 9, ((b) << 16) | 6, ((b) << 16) | 7 }

/* Motion data numbers per character. */
static s32 lbl_1_data_28[13][8] = {
    M528_MOT(0x8C), M528_MOT(0x33), M528_MOT(0xB7), M528_MOT(0xED), M528_MOT(0xE6), M528_MOT(0x17), M528_MOT(0xE0),
    M528_MOT(0x2D), M528_MOT(0xC9), M528_MOT(0xB0), M528_MOT(0xB0), M528_MOT(0xB0), M528_MOT(0xB0),
};

/* Model data numbers. */
static s32 lbl_1_data_1C8[0x36] = {
    0x560000, 0x560001, 0x560002, 0x560003, 0x560004, 0x560005, 0x560006, 0x560007, 0x560008, 0x560009, 0x56000A,
    0x56000D, 0x56000E, 0x560011, 0x560012, 0x560013, 0x560014, 0x560015, 0x560016, 0x560017, 0x560018, 0x560019,
    0x56001A, 0x56001B, 0x56001C, 0x56001D, 0x56001E, 0x56001F, 0x560020, 0x560021, 0x560022, 0x560023, 0x560024,
    0x560025, 0x560026, 0x560027, 0x560028, 0x560029, 0x56002A, 0x56002B, 0x56002C, 0x56002D, 0x56002E, 0x56002F,
    0x560030, 0x560031, 0x560033, 0x560035, 0x560037, 0x560038, 0x560039, 0x56003A, 0x56003B, 0x56003C,
};

/* Joint motion data numbers. */
static s32 lbl_1_data_2A0[7] = { 0x56000B, 0x56000C, 0x56000F, 0x560010, 0x560032, 0x560034, 0x560036 };

/* Camera views: aim x/y/z, rot x/y, distance. */
static f32 lbl_1_data_2BC[4][6] = {
    { 0.0f, 717.0f, 0.0f, 90.0f, 37.0f, 8680.0f },
    { 0.0f, 124.0f, 0.0f, 90.0f, 26.0f, 3810.0f },
    { -300.0f, 124.0f, 0.0f, 90.0f, 21.0f, 3810.0f },
    { 300.0f, 124.0f, 0.0f, 90.0f, 21.0f, 3810.0f },
};

/* Path points the walkers follow (before rotation by the wheel angle). */
static HuVecF lbl_1_data_31C[0x37] = {
    { 76.3527f, -28.65594f, 303.8886f },
    { 89.36818f, -20.12116f, 627.3725f },
    { 132.45467f, -7.8856f, 617.07947f },
    { 175.47755f, 5.14328f, 603.90344f },
    { 213.52226f, 19.1923f, 583.2219f },
    { 258.0741f, 35.48818f, 558.334f },
    { 307.4178f, 53.50759f, 528.4491f },
    { 351.7477f, 71.07372f, 488.91357f },
    { 387.29947f, 86.45116f, 453.47565f },
    { 423.9798f, 102.31677f, 416.9127f },
    { 453.92682f, 114.74768f, 374.23383f },
    { 482.55255f, 128.153f, 327.6315f },
    { 506.63617f, 140.91176f, 280.20404f },
    { 523.21515f, 149.26572f, 234.10138f },
    { 536.4226f, 157.65977f, 186.78319f },
    { 545.4938f, 165.1289f, 144.12125f },
    { 550.5082f, 169.3213f, 99.43111f },
    { 555.52264f, 173.5137f, 54.74098f },
    { 554.31415f, 178.93967f, 10.26389f },
    { 548.245f, 182.21725f, -43.50827f },
    { 540.7429f, 184.87166f, -94.25285f },
    { 526.2494f, 189.22081f, -148.9162f },
    { 508.16705f, 190.19507f, -199.4755f },
    { 485.3958f, 191.22429f, -246.7632f },
    { 455.54944f, 191.06044f, -297.0153f },
    { 422.8942f, 189.8421f, -342.977f },
    { 387.02106f, 188.64108f, -381.5805f },
    { 347.565f, 184.92625f, -416.4199f },
    { 297.2428f, 181.25046f, -457.2818f },
    { 241.78398f, 175.96623f, -490.8356f },
    { 182.54707f, 170.04622f, -518.3706f },
    { 126.37634f, 165.11037f, -538.3222f },
    { 64.86789f, 158.29259f, -552.0461f },
    { 5.82033f, 152.42786f, -559.9274f },
    { -56.36307f, 144.40205f, -559.3981f },
    { -127.3225f, 136.82695f, -552.8494f },
    { -175.3741f, 132.81107f, -542.1f },
    { -219.185f, 128.13605f, -528.8677f },
    { -271.0152f, 123.20982f, -509.1224f },
    { -328.317f, 119.05633f, -478.0202f },
    { -383.0389f, 115.4856f, -437.4594f },
    { -429.5695f, 112.6725f, -396.8103f },
    { -477.0712f, 112.08831f, -339.5657f },
    { -511.2278f, 113.79501f, -290.2382f },
    { -544.9306f, 114.67406f, -219.9456f },
    { -565.2117f, 118.71691f, -148.9897f },
    { -576.6864f, 124.72604f, -55.24234f },
    { -571.7904f, 132.87604f, 20.97713f },
    { -559.52f, 138.74858f, 81.16179f },
    { -541.5015f, 145.87262f, 143.0994f },
    { -516.2133f, 150.99301f, 190.12534f },
    { -486.0693f, 159.3626f, 235.53607f },
    { -453.4974f, 169.35678f, 280.1392f },
    { -419.3569f, 178.96738f, 310.31488f },
    { -205.5353f, 173.01883f, 111.80605f },
};

/* ---- bss globals (m528) ---- */
static OMOBJMAN *lbl_1_bss_29C;
static OMOBJ *lbl_1_bss_298;
static HuVecF lbl_1_bss_4[0x37];
static s32 lbl_1_bss_0;

/* ---- rodata (m528) ---- */
M528_RODATA const u16 lbl_1_rodata_10[4] = { 1, 2, 4, 8 };

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
    OMOBJ *obj;
    s32 i;
    M528MAIN *work;

    lbl_1_bss_29C = omInitObjMan(10, 0x2000);
    omGameSysInit(lbl_1_bss_29C);
    obj = omAddObjEx(lbl_1_bss_29C, 0x65, 0, 0, -1, fn_1_13D0);
    lbl_1_bss_298 = obj;
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M528MAIN), HU_MEMNUM_OVL);
    work = obj->data;
    fn_1_16C4(work);
    fn_1_15EC(work);
    fn_1_17B0(work);
    for (i = 0; i < 4; i++) {
        obj = omAddObjEx(lbl_1_bss_29C, 0x64, 1, 8, 0, fn_1_113C);
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M528PLAYER), HU_MEMNUM_OVL);
        obj->work[0] = i;
    }
    fn_1_1538(work);
    Hu3DBGColorSet(0, 0, 0);
    Hu3DNoSyncSet(1);
    MgSeqCreate(&lbl_1_data_0);
}

void fn_1_1E8(s16 mode, s16 frameNo)
{
    M528MAIN *work = lbl_1_bss_298->data;

    work->mode = 0;
    work->sel[0] = -1;
    work->sel[1] = -1;
    MgSeqModeNext();
}

/* Roulette spin-up: the wheel accelerates, slows down and snaps to a sector
 * (22.5 degrees each); then the players start to jump. */
void fn_1_238(s16 mode, s16 frameNo)
{
    u16 delay[4] = { 2, 13, 25, 39 };
    M528MAIN *work;
    M528PLAYER *player;
    f32 rnd;
    s32 i;

    work = lbl_1_bss_298->data;
    switch (work->mode) {
        case 0:
            MgSeqModeChangeOff();
            work->wheelAngle = 0.0f;
            work->timer = 0;
            work->mode++;
        case 1:
            work->timer++;
            if (work->timer >= 60) {
                rnd = (f32)frandmod(100) / 100.0f;
                work->spinPower = 3.0f + rnd;
                work->timer = 0;
                HuAudFXPlay(0x786);
                HuAudFXPlay(0x78E);
                work->mode++;
            }
            break;
        case 2:
            work->wheelSpeed = 0.5 * (work->spinPower * (1.0 + HuCos(180.0f * ((f32)work->timer / 60.0f) - 180.0f)));
            work->wheelAngle -= work->wheelSpeed;
            if (work->wheelAngle < 0.0f) {
                work->wheelAngle += 360.0f;
            }
            if (work->timer == 0x32) {
                work->camFunc = fn_1_3CA0;
            }
            work->timer++;
            if (work->timer > 60) {
                work->timer = 0;
                work->mode++;
            }
            break;
        case 3:
            work->wheelSpeed = 0.2f + 0.5 * (work->spinPower * (1.0 + HuCos(180.0f * ((f32)work->timer / 120.0f))));
            work->wheelAngle -= work->wheelSpeed;
            if (work->wheelAngle < 0.0f) {
                work->wheelAngle += 360.0f;
            }
            work->timer++;
            if (work->timer >= 0x78) {
                work->mode++;
                work->wheelTarget = 22.5f * (f32)(s32)(work->wheelAngle / 22.5f);
            }
            break;
        case 4:
            work->wheelAngle -= work->wheelSpeed;
            if (work->wheelAngle <= work->wheelTarget) {
                work->wheelAngle = work->wheelTarget;
                work->mode++;
            }
            break;
        case 5:
            if (work->wheelAngle < 0.0f) {
                work->wheelAngle += 360.0f;
            }
            work->sel[0] = (0x10 - (s32)(work->wheelAngle / 22.5f)) % 16;
            work->sel[1] = (work->sel[0] + 1) % 16;
            work->mode++;
            work->timer = 0;
            fn_1_46CC(work);
            break;
        case 6:
            work->timer++;
            if (work->timer == 1) {
                work->teamFunc[0] = fn_1_47F0;
            }
            if (work->timer == 5) {
                work->teamFunc[1] = fn_1_47F0;
            }
            if (work->timer >= 8) {
                fn_1_B384(work);
                work->walker[0].state++;
                work->walker[1].state++;
                work->walker[2].state++;
                work->walker[3].state++;
                work->timer = 0;
                work->mode++;
            }
            break;
        case 7:
            work->timer++;
            for (i = 0; i < 4; i++) {
                if (work->timer == delay[i]) {
                    player = work->player[i];
                    player->state = 0;
                    player->hook = fn_1_1ECC;
                }
            }
            if (work->timer >= 0x28) {
                work->timer = 0;
                work->mode++;
            }
            break;
        case 8:
            lbl_1_bss_0 = HuAudBGMPlay(0x3C);
            MgSeqModeNext();
            break;
    }
    fn_1_3A00(work);
    if (work->camFunc != NULL && work->camFunc(&work->cam) != 0) {
        work->camFunc = NULL;
    }
    if (work->teamFunc[0] != NULL && work->teamFunc[0](&work->team[0]) != 0) {
        HuAudFXPlay(0x787);
        work->camFunc = fn_1_43B4;
        work->cam.state = 0;
        work->teamFunc[0] = NULL;
    }
    if (work->teamFunc[1] != NULL && work->teamFunc[1](&work->team[1]) != 0) {
        HuAudFXPlay(0x788);
        work->teamFunc[1] = NULL;
    }
}

void fn_1_8E4(s16 mode, s16 frameNo)
{
    M528MAIN *work = lbl_1_bss_298->data;

    if (work->camFunc != 0) {
        if (work->camFunc(&work->cam) != 0) {
            work->camFunc = 0;
        }
    }
    work->mode = 0;
    work->fadeFlag = 0;
}

void fn_1_954(s16 mode, s16 frameNo)
{
    M528MAIN *work;
    M528SEG *seg;

    work = lbl_1_bss_298->data;
    work->inputOn = 1;
    if (work->winner >= 0) {
        work->segFunc = fn_1_4C84;
        seg = work->team[work->winner].seg;
        seg->state = 0;
        work->inputOn = 0;
        MgSeqModeNext();
    }
}

void fn_1_9CC(s16 mode, s16 frameNo)
{
    M528MAIN *work = lbl_1_bss_298->data;

    work->inputOn = 0;
    if (work->fadeFlag == 0) {
        HuAudSStreamFadeOut(lbl_1_bss_0, 0x3E8);
        work->fadeFlag++;
    }
    if (work->winner < 0) {
        MgSeqWinnerSet(-1, -1, -1, -1);
    } else {
        MgSeqWinnerSet(work->teamPlayer[work->winner][0]->charNo, work->teamPlayer[work->winner][1]->charNo, -1, -1);
        fn_1_4E80(work);
        work->teamFunc[work->winner ^ 1] = fn_1_495C;
        work->team[work->winner ^ 1].state = 0;
    }
    if (work->segFunc != NULL && work->segFunc(work->team[work->winner].seg) != 0) {
        work->segFunc = NULL;
    }
}

/* Result: the winning team sector spawns its effect and the camera waits. */
void fn_1_B10(s16 mode, s16 frameNo)
{
    s32 flags[8] = { 1, 1, 1, 1, 1, 0, 1, 0 };
    void (*emit[8])(M528MAIN *) = {
        fn_1_C314, fn_1_C3DC, fn_1_C530, fn_1_C728, fn_1_C8B4, fn_1_CA34, fn_1_CAEC, fn_1_CC40,
    };
    M528MAIN *work;
    M528CamFunc camFunc;
    s32 i;
    s32 other;

    work = lbl_1_bss_298->data;
    switch (work->mode) {
        case 0:
            MgSeqModeChangeOff();
            if (work->winner >= 0) {
                if (work->winner != 0) {
                    camFunc = fn_1_4158;
                } else {
                    camFunc = fn_1_3EFC;
                }
                work->camFunc = camFunc;
            }
            work->mode++;
        case 1:
            if (work->camFunc == NULL) {
                work->mode++;
            }
            break;
        case 2:
            for (i = 0; i < 4; i++) {
                if (work->player[i]->hook != fn_1_1F8C) {
                    break;
                }
            }
            if (i == 4) {
                work->mode++;
            }
            break;
        case 3:
            if (work->winner >= 0) {
                work->segFunc = fn_1_4B04;
                work->team[work->winner].seg->state = 0;
                omVibrate(work->teamPlayer[work->winner][0]->playerNo, 0xC, 0xC, 0);
                omVibrate(work->teamPlayer[work->winner][1]->playerNo, 0xC, 0xC, 0);
            }
            work->mode++;
            break;
        case 4:
            if (work->winner >= 0) {
                emit[work->team[work->winner].seg->type](work);
                if (flags[work->team[work->winner].seg->type] != 0) {
                    HuAudFXPlay(0x78C);
                }
                if (work->team[work->winner].seg->type == 7) {
                    HuAudFXPlay(0x78D);
                }
            }
            work->timer = 0;
            work->mode++;
            break;
        case 5:
            work->timer++;
            if (work->timer > 0x1E) {
                MgSeqModeNext();
                work->mode = 0;
            }
            break;
    }
    if (work->camFunc != NULL && work->camFunc(&work->cam) != 0) {
        work->camFunc = NULL;
    }
    if (work->segFunc != NULL && work->segFunc(work->team[work->winner].seg) != 0) {
        work->segFunc = NULL;
    }
    if (work->winner >= 0) {
        other = work->winner ^ 1;
        if (work->teamFunc[other] != NULL) {
            if (work->teamFunc[other](&work->team[other]) != 0) {
                work->teamFunc[other] = NULL;
            }
        }
    }
}

void fn_1_ED0(s16 mode, s16 frameNo)
{
    M528MAIN *work;
    M528PLAYER *player;
    s16 playerNo;
    s32 i;

    work = lbl_1_bss_298->data;
    switch (work->mode) {
        case 1:
            break;
        case 0:
            if (work->winner < 0) {
                CharLoseVoicePlay(work->player[0]->charNo, work->player[1]->charNo, work->player[2]->charNo, work->player[3]->charNo);
                for (i = 0; i < 4; i++) {
                    player = work->player[i];
                    player->hook = fn_1_25FC;
                    player->state = 0;
                }
            } else {
                for (i = 0; i < 2; i++) {
                    if (i == work->winner) {
                        player = work->teamPlayer[i][0];
                        player->hook = fn_1_2680;
                        player->state = 0;
                        playerNo = player->playerNo;
                        if (_CheckFlag(0x1000F) == 0) {
                            GwPlayer[playerNo].mgCoinBonus = 10;
                        }
                        CharModelVoicePanSet(player->charNo, 0x40);
                        player = work->teamPlayer[i][1];
                        player->hook = fn_1_2680;
                        player->state = 0;
                        playerNo = player->playerNo;
                        if (_CheckFlag(0x1000F) == 0) {
                            GwPlayer[playerNo].mgCoinBonus = 10;
                        }
                        CharModelVoicePanSet(player->charNo, 0x40);
                    } else {
                        player = work->teamPlayer[i][0];
                        player->hook = fn_1_25FC;
                        player->state = 0;
                        player = work->teamPlayer[i][1];
                        player->hook = fn_1_25FC;
                        player->state = 0;
                    }
                }
            }
            work->mode++;
            break;
    }
    if (work->segFunc != NULL && work->segFunc(work->team[work->winner].seg) != 0) {
        work->segFunc = NULL;
    }
    if (work->camFunc != NULL && work->camFunc(&work->cam) != 0) {
        work->camFunc = NULL;
    }
}

void fn_1_1118(s16 mode, s16 frameNo)
{
}

/* The whole compiled body is the read of the sequence work block into a stack
 * slot that is never read back, so the local has to be memory-resident. */
void fn_1_111C(s16 mode, s16 frameNo)
{
    M528MAIN *volatile work = lbl_1_bss_298->data;
}

void fn_1_113C(OMOBJ *obj)
{
    M528PLAYER *player;
    s32 i;
    M528MAIN *work;

    player = obj->data;
    work = lbl_1_bss_298->data;
    player->playerNo = obj->work[0];
    player->charNo = GwPlayerConf[player->playerNo].charNo;
    player->grp = GwPlayerConf[player->playerNo].grp;
    player->padNo = GwPlayerConf[player->playerNo].padNo;
    player->type = GwPlayerConf[player->playerNo].type;
    player->dif = GwPlayerConf[player->playerNo].dif;
    player->obj = obj;
    work->player[player->playerNo] = player;
    i = work->teamPlayer[player->grp][0] != NULL;
    work->teamPlayer[player->grp][i] = player;
    player->slot = &work->team[player->grp].slot[i];
    *obj->mdlId = CharModelCreate(player->charNo, 2);
    for (i = 0; i < 8; i++) {
        obj->mtnId[i] = CharMotionCreate(player->charNo, lbl_1_data_28[player->charNo][i]);
    }
    CharMotionSet(player->charNo, *obj->mtnId);
    CharModelAttrSet(player->charNo, 0x40000001);
    Hu3DModelLayerSet(*obj->mdlId, 2);
    Hu3DModelShadowSet(*obj->mdlId);
    CharEffectLayerSet(3);
    CharMotionDataClose(player->charNo);
    player->hook = fn_1_1DFC;
    player->state = 0;
    player->onSlot = 0;
    player->ai.state = 0;
    obj->objFunc = fn_1_1368;
}

/* The dead read of the main object work block into a never-used slot comes
 * first, then the player own block drives the optional hook. */
void fn_1_1368(OMOBJ *obj)
{
    M528PLAYER *player = obj->data;
    M528MAIN *volatile main = lbl_1_bss_298->data;

    if (player->hook != 0) {
        player->hook(player);
    }
}

void fn_1_13CC(s16 mode, s16 frameNo)
{
}

void fn_1_13D0(OMOBJ *obj)
{
    M528MAIN *work = lbl_1_bss_298->data;

    fn_1_51A8(work);
    fn_1_1918(&work->cam);
    fn_1_198C(&work->cam, 0);
    fn_1_1CAC(work);
    fn_1_38F8(work);
    fn_1_4F30(work);
    fn_1_6320(work);
    obj->objFunc = fn_1_1458;
}

void fn_1_1458(OMOBJ *obj)
{
    M528MAIN *work;
    s32 team0;
    s32 team1;

    work = lbl_1_bss_298->data;
    team0 = fn_1_2EC0(&work->team[0]);
    team1 = fn_1_2EC0(&work->team[1]);
    if (team0 != 0 && team1 != 0) {
        work->winner = frandmod(2);
    } else if (team0 != 0) {
        work->winner = 0;
    } else if (team1 != 0) {
        work->winner = 1;
    }
    fn_1_1A20(&work->cam);
    fn_1_3330(work);
    fn_1_B6D4(&work->walker[0]);
    fn_1_B6D4(&work->walker[1]);
    fn_1_B6D4(&work->walker[2]);
    fn_1_B6D4(&work->walker[3]);
    fn_1_50B0(work);
}

void fn_1_1534(s16 mode, s16 frameNo)
{
}

void fn_1_1538(M528MAIN *work)
{
    s32 i;

    work->inputOn = 0;
    work->winner = -1;
    fn_1_1874(work);
    fn_1_18E0(work);
    for (i = 0; i < 0x10; i++) {
        fn_1_36DC(&work->seg[i]);
    }
    work->teamPlayer[0][0] = 0;
    work->teamPlayer[0][1] = 0;
    work->teamPlayer[1][0] = 0;
    work->teamPlayer[1][1] = 0;
    work->camFunc = 0;
    work->teamFunc[0] = 0;
    work->teamFunc[1] = 0;
    work->segFunc = 0;
}

void fn_1_15EC()
{
    s16 light;

    light = Hu3DGLightCreate(0.0f, 1000.0f, 1000.0f, 0.0f, -1.0f, -0.5f, 0xFF, 0xFF, 0xFF);
    Hu3DGLightPosAimSet(light, 0.0f, 100.0f, 100.0f, 0.0f, 0.0f, 0.0f);
    Hu3DGLightInfinitytSet(light);
}

void fn_1_16C4()
{
    Hu3DCameraCreate(lbl_1_rodata_10[0]);
    Hu3DCameraPerspectiveSet(lbl_1_rodata_10[0], 15.0f, 20.0f, 20000.0f, 1.2f);
    Hu3DCameraPosSet(lbl_1_rodata_10[0], 0.0f, 300.0f, 1000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 150.0f, 0.0f);
}

void fn_1_17B0()
{
    HuVecF up = { 0.0f, 1.0f, 0.0f };
    HuVecF pos = { 0.0f, 4000.0f, 1000.0f };
    HuVecF target = { 0.0f, 0.0f, 0.0f };

    Hu3DShadowCreate(15.0f, 20.0f, 20000.0f);
    Hu3DShadowTPLvlSet(0.3f);
    Hu3DShadowPosSet(&pos, &up, &target);
}

/* Clears the model cache: 0x36 entries, then the two segment model caches. */
void fn_1_1874(M528MAIN *work)
{
    s32 i;

    for (i = 0; i < 0x36; i++) {
        work->mdlCache[i] = -1;
    }
    for (i = 0; i < 0x10; i++) {
        work->segMdlCache[i] = -1;
        work->segMdlCache2[i] = -1;
    }
}

void fn_1_18E0(M528MAIN *work)
{
    s32 i;

    for (i = 0; i < 7; i++) {
        work->mtnCache[i] = -1;
    }
}

void fn_1_1918(M528CAM *cam)
{
    cam->rotX = 0.0f;
    cam->rotY = 0.0f;
    cam->dist = 1000.0f;
    cam->aimX = 0.0f;
    cam->aimY = 0.0f;
    cam->aimZ = 0.0f;
    cam->state = 0;
    cam->timer = 0;
}

void fn_1_198C(M528CAM *cam, s32 idx)
{
    cam->rotX = lbl_1_data_2BC[idx][3];
    cam->rotY = lbl_1_data_2BC[idx][4];
    cam->dist = lbl_1_data_2BC[idx][5];
    cam->aimX = lbl_1_data_2BC[idx][0];
    cam->aimY = lbl_1_data_2BC[idx][1];
    cam->aimZ = lbl_1_data_2BC[idx][2];
}

void fn_1_1A20(M528CAM *cam)
{
    f32 x;
    f32 y;
    f32 z;

    if (cam->rotX > 360.0f) {
        cam->rotX -= 360.0f;
    }
    if (cam->rotX < 0.0f) {
        cam->rotX += 360.0f;
    }
    if (cam->rotY > 360.0f) {
        cam->rotY -= 360.0f;
    }
    if (cam->rotY < 0.0f) {
        cam->rotY += 360.0f;
    }
    x = cam->aimX + cam->dist * HuCos(cam->rotY) * HuCos(cam->rotX);
    y = cam->aimY + cam->dist * HuSin(cam->rotY);
    z = cam->aimZ + cam->dist * HuCos(cam->rotY) * HuSin(cam->rotX);
    Hu3DCameraPosSet(lbl_1_rodata_10[0], x, y, z, 0.0f, 1.0f, 0.0f, cam->aimX, cam->aimY, cam->aimZ);
}

/* Place the players on their pedals and set the panning of their voices. */
void fn_1_1CAC(M528MAIN *work)
{
    s16 pan[4] = { 0x28, 0x38, 0x48, 0x58 };
    M528PLAYER *player;
    s32 i;
    s32 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            player = work->teamPlayer[i][j];
            player->pos.x = player->slot->pos.x;
            player->pos.y = 2501.0f;
            player->pos.z = 75.0f + player->slot->pos.z;
            player->rot.x = 0.0f;
            player->rot.y = 0.0f;
            player->rot.z = 0.0f;
            omSetTra(player->obj, player->pos.x, player->pos.y, player->pos.z);
            omSetRot(player->obj, player->rot.x, player->rot.y, player->rot.z);
            CharModelVoicePanSet(player->charNo, pan[i * 2 + j]);
        }
    }
}

s32 fn_1_1DFC(M528PLAYER *player)
{
    f32 time;

    switch (player->state) {
        case 0:
            CharMotionSet(player->charNo, player->obj->mtnId[1]);
            CharModelAttrReset(player->charNo, 0x40000001);
            CharModelAttrSet(player->charNo, 0x40000002);
            player->state++;
            break;
        case 1:
            time = CharMotionMaxTimeGet(player->charNo);
            CharMotionTimeSet(player->charNo, time);
            CharModelAttrReset(player->charNo, 0x40000002);
            player->state++;
            break;
    }
    return 0;
}

s32 fn_1_1ECC(M528PLAYER *player)
{
    switch (player->state) {
        case 0:
            player->pos.y = 1000.0f;
            player->baseY = 0.0f;
            player->peakY = 200.0f;
            player->timer = 0;
            player->state++;
        case 1:
            player->timer++;
            player->pos.y -= 2.4f * (f32)player->timer;
            break;
    }
    return 0;
}

s32 fn_1_1F8C(M528PLAYER *player)
{
    s32 pressed;

    switch (player->state) {
        case 0:
            player->slot->active = 1.0f;
            player->onSlot = 1;
            player->holdFrames = 0;
            player->state++;
        case 1:
            if (player->type != 0) {
                pressed = fn_1_27AC(player);
            } else {
                pressed = fn_1_2704(player);
            }
            if (pressed != 0) {
                CharMotionShiftSet(player->charNo, player->obj->mtnId[1], 0.0f, 3.0f, 0);
                player->state = 0;
                player->hook = fn_1_2080;
            }
            break;
    }
    return 0;
}

s32 fn_1_2080(M528PLAYER *player)
{
    f32 jump[2] = { 25.0f, 39.0f };
    s32 pressed;

    switch (player->state) {
        case 0:
            player->timer = 0;
            player->holdFrames = 0;
            player->strong = 0;
            player->state++;
        case 1:
            if (player->type != 0) {
                player->strong = fn_1_2E3C(player);
            } else {
                player->strong = fn_1_2D7C(player);
            }
            player->timer++;
            if (player->timer == 6) {
                player->timer = 0;
                player->state++;
            }
            break;
        case 2:
            player->onSlot = 0;
            player->slot->active = 0.0f;
            player->peakY = player->pos.y;
            player->baseY = player->pos.y;
            player->speed = 0.0f;
            player->state++;
        case 3:
            player->timer++;
            player->pos.y += jump[player->strong] - 2.4f * (f32)player->timer;
            if (player->pos.y > player->peakY) {
                player->peakY = player->pos.y;
            }
            if (player->type != 0) {
                pressed = fn_1_2A00(player);
            } else {
                pressed = fn_1_2758(player);
            }
            if (pressed != 0) {
                player->peakY = player->pos.y;
                CharMotionShiftSet(player->charNo, player->obj->mtnId[3], 0.0f, 3.0f, 0);
                player->state = 0;
                player->hook = fn_1_237C;
            }
            break;
    }
    return 0;
}

s32 fn_1_22B4(M528PLAYER *player)
{
    switch (player->state) {
        case 0:
            player->timer = 0;
            player->state++;
        case 1:
            player->timer++;
            if (player->timer == 9) {
                CharMotionShiftSet(player->charNo, player->obj->mtnId[0], 0.0f, 3.0f, 0x40000001);
            }
            if (player->timer >= 0xC) {
                player->state = 0;
                player->hook = fn_1_1F8C;
            }
            break;
    }
    return 0;
}

s32 fn_1_237C(M528PLAYER *player)
{
    switch (player->state) {
        case 0:
            player->timer = 0;
            player->state++;
        case 1:
            player->timer++;
            if (player->timer >= 3) {
                player->timer = 0;
                player->state++;
            }
            break;
        case 2:
            if (CharMotionEndCheck(player->charNo) != 0) {
                player->state++;
            }
            break;
        case 3:
            player->timer++;
            player->pos.y -= 6.3f * (f32)player->timer;
            break;
    }
    return 0;
}

s32 fn_1_2488(M528PLAYER *player)
{
    switch (player->state) {
        case 0:
            player->timer = 0;
            player->state++;
        case 1:
            player->timer++;
            if (player->timer >= 3) {
                player->state++;
            }
            break;
        case 2:
            if (CharMotionEndCheck(player->charNo) != 0) {
                CharMotionSet(player->charNo, player->obj->mtnId[5]);
                CharMotionSpeedSet(player->charNo, 1.5f);
                player->state = 0;
                player->hook = fn_1_2564;
            }
            break;
    }
    return 0;
}

s32 fn_1_2564(M528PLAYER *player)
{
    switch (player->state) {
        case 0:
            if (CharMotionEndCheck(player->charNo) != 0) {
                CharMotionSet(player->charNo, *player->obj->mtnId);
                CharMotionSpeedSet(player->charNo, 1.0f);
                CharModelAttrSet(player->charNo, 0x40000001);
                player->state = 0;
                player->hook = fn_1_1F8C;
            }
            break;
    }
    return 0;
}

s32 fn_1_25FC(M528PLAYER *player)
{
    switch (player->state) {
        case 1:
            break;
        case 0:
            CharMotionShiftSet(player->charNo, player->obj->mtnId[7], 0.0f, 3.0f, 0);
            player->state++;
            break;
    }
    return 0;
}

s32 fn_1_2680(M528PLAYER *player)
{
    switch (player->state) {
        case 1:
            break;
        case 0:
            CharMotionShiftSet(player->charNo, player->obj->mtnId[6], 0.0f, 3.0f, 0);
            player->state++;
            break;
    }
    return 0;
}

/* Pad-gated input: returns 0 unless the main object has its input block armed,
 * else the freshly-pressed bits of the pad -- 0x100 here, 0x140 for fn_1_2758.
 * Both callers only test the result against 0. */
s32 fn_1_2704(M528PLAYER *player)
{
    M528MAIN *main = lbl_1_bss_298->data;

    if (main->inputOn == 0) {
        return 0;
    }
    return HuPadBtnDown[player->padNo] & 0x100;
}

s32 fn_1_2758(M528PLAYER *player)
{
    M528MAIN *main = lbl_1_bss_298->data;

    if (main->inputOn == 0) {
        return 0;
    }
    return HuPadBtnDown[player->padNo] & 0x140;
}

/* CPU player: decide whether to press while standing on the pedal. */
s32 fn_1_27AC(M528PLAYER *player)
{
    s32 pressed = 0;
    s32 pressChance[2][4] = { { 50, 80, 95, 99 }, { 25, 15, 10, 5 } };
    s32 idleChance[2][4] = { { 10, 5, 0, 0 }, { 10, 5, 0, 0 } };
    s32 partnerBusy;
    s32 partnerAir;
    M528MAIN *main;
    M528AI *ai;

    main = lbl_1_bss_298->data;
    if (main->inputOn == 0) {
        return 0;
    }
    ai = &player->ai;
    switch (ai->state) {
        case 0:
            partnerAir = 1;
            partnerBusy = 1;
            if (player->partner->hook != fn_1_1F8C && player->partner->hook != fn_1_22B4) {
                partnerBusy = 0;
            }
            if (partnerBusy == 0 && player->partner->hook != fn_1_2488) {
                partnerAir = 0;
            }
            if (frandmod(100) < pressChance[partnerAir][player->dif]) {
                ai->timer = 0;
                if (partnerAir != 0) {
                    ai->wait = frandmod(18) + 9;
                } else {
                    ai->wait = frandmod(8) + 4;
                }
                ai->state++;
            } else {
                ai->timer = idleChance[partnerAir][player->dif] <= frandmod(100);
                pressed = 1;
            }
            break;
        case 1:
            ai->timer++;
            if (ai->timer > ai->wait) {
                ai->timer = 0;
                ai->state = 0;
            }
            break;
    }
    return pressed;
}

s32 fn_1_2A00(M528PLAYER *player)
{
    s32 pressed = 0;
    s32 chanceA[2][4] = { { 30, 60, 55, 50 }, { 30, 60, 55, 50 } };
    s32 chanceB[2][4] = { { 80, 85, 90, 95 }, { 80, 85, 90, 95 } };
    s32 partnerBusy;
    s32 partnerAir;
    M528MAIN *main;
    M528AI *ai;

    main = lbl_1_bss_298->data;
    if (main->inputOn == 0) {
        return 0;
    }
    ai = &player->ai;
    switch (ai->state) {
        case 0:
            partnerAir = 1;
            partnerBusy = 1;
            if (player->partner->hook != fn_1_1F8C && player->partner->hook != fn_1_22B4) {
                partnerBusy = 0;
            }
            if (partnerBusy == 0 && player->partner->hook != fn_1_2488) {
                partnerAir = 0;
            }
            if (player->strong != 0) {
                if (frandmod(100) < chanceB[partnerAir][player->dif]) {
                    ai->wait = fn_1_2C40(player->strong, player->dif);
                } else {
                    ai->wait = 0;
                }
            } else if (frandmod(100) < chanceA[partnerAir][player->dif]) {
                ai->wait = fn_1_2C40(player->strong, player->dif);
            } else {
                ai->wait = 0;
            }
            ai->state++;
        case 1:
            ai->timer++;
            if (ai->timer == ai->wait) {
                ai->timer = 0;
                pressed = 1;
            }
            break;
    }
    return pressed;
}

s16 fn_1_2C40(s16 strong, s32 dif)
{
    s32 d;
    s16 wait;

    d = dif;
    wait = 0;
    if (d != 0) {
        switch (strong) {
            case 0:
                wait = frandmod(9) + 0xD;
                break;
            case 1:
                wait = frandmod(8) + 0xB;
                break;
            case 2:
                wait = frandmod(7) + 9;
                break;
            case 3:
                wait = frandmod(6) + 7;
                break;
        }
    } else {
        switch (strong) {
            case 0:
                wait = frandmod(5) + 5;
                break;
            case 1:
                wait = frandmod(4) + 7;
                break;
            case 2:
                wait = frandmod(3) + 9;
                break;
            case 3:
                wait = frandmod(2) + 0xB;
                break;
        }
    }
    return wait;
}

s16 fn_1_2D7C(M528PLAYER *player)
{
    M528MAIN *main = lbl_1_bss_298->data;

    if (main->inputOn == 0) {
        return 0;
    }
    if ((HuPadBtnDown[player->padNo] & 0x100) != 0) {
        player->holdFrames = 0;
    } else if ((HuPadBtn[player->padNo] & 0x100) != 0) {
        player->holdFrames++;
    }
    return player->holdFrames > 5;
}

s16 fn_1_2E3C(M528PLAYER *player)
{
    M528MAIN *main = lbl_1_bss_298->data;
    M528AI *ai;

    if (main->inputOn == 0) {
        return 0;
    }
    ai = &player->ai;
    if (ai->timer != 0) {
        player->holdFrames++;
    }
    return player->holdFrames > 5;
}

/* Per-frame update of one team seesaw: tilt from the pedal impulses, pedal and
 * board positions, and the wheel sector advance.  Returns non-zero when the
 * sector animation reaches its end. */
s32 fn_1_2EC0(M528TEAM *team)
{
    f32 power;
    f32 c;
    f32 s;
    f32 diffActive;
    f32 tiltAdd;
    f32 diffAlpha;
    s32 result;
    M528MAIN *main;
    M528SLOT *slot0;
    M528SLOT *slot1;

    result = 0;
    main = lbl_1_bss_298->data;
    slot0 = slot0;
    slot1 = &team->slot[1];
    diffAlpha = slot0->alpha - slot1->alpha;
    diffActive = slot0->active - slot1->active;
    if (diffActive == 0.0f) {
        tiltAdd = 5.0f * (-team->tilt / 25.0f);
    } else {
        tiltAdd = 0.0f;
    }
    fn_1_32D0(slot0);
    fn_1_32D0(slot1);
    team->tilt += tiltAdd + (6.4f * diffAlpha + 0.5f * diffActive);
    if (slot0->waitFrames - slot1->waitFrames == 0) {
        power = 0.0f;
    } else {
        power = fabsf(slot0->alpha - slot1->alpha);
        if (power > 1.0f) {
            power = 1.0f;
        }
        if (slot0->hit != 0 || slot1->hit != 0) {
            HuAudFXPlay(team->sndId);
        }
    }
    slot0->hit = 0;
    slot1->hit = 0;
    if (team->tilt > 25.0f) {
        power = 0.0f;
        team->tilt = 25.0f;
    }
    if (team->tilt < -25.0f) {
        power = 0.0f;
        team->tilt = -25.0f;
    }
    if (main->inputOn != 0) {
        result = fn_1_3750(team->seg, power);
    }
    team->spin += 6.0f * power;
    if (team->spin > 360.0f) {
        team->spin -= 360.0f;
    }
    c = 150.0 * HuCos(team->tilt);
    s = 150.0 * HuSin(team->tilt);
    Hu3DModelRotSet(team->mdl, 0.0f, 0.0f, team->tilt);
    slot0->pos.x = team->pivot.x - c;
    slot0->pos.y = team->pivot.y - s;
    slot1->pos.x = team->pivot.x + c;
    slot1->pos.y = team->pivot.y + s;
    Hu3DModelPosSet(slot0->mdl, slot0->pos.x, slot0->pos.y, slot0->pos.z);
    Hu3DModelPosSet(slot1->mdl, slot1->pos.x, slot1->pos.y, slot1->pos.z);
    Hu3DModelRotSet(team->spinMdl, 0.0f, 0.0f, team->spin);
    return result;
}

/* Either decrement the wait counter or scale the alpha down by 0.95 and snap it
 * to zero once it falls under 0.01. */
void fn_1_32D0(M528SLOT *slot)
{
    if (slot->waitFrames != 0) {
        --slot->waitFrames;
        return;
    }

    slot->alpha *= 0.95f;

    if (slot->alpha < 0.01f) {
        slot->alpha = 0.0f;
    }
}

/* Players land on their pedal: pick the follow-up animation of the hook. */
void fn_1_3330(M528MAIN *work)
{
    f32 unused[2] = { 1.0f, -1.0f };
    M528PLAYER *player;
    f32 power;
    f32 power2;
    s16 wait;
    s16 wait2;
    s32 i;
    s32 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            player = work->teamPlayer[i][j];
            if (player->onSlot != 0) {
                player->pos.x = player->slot->pos.x;
                player->pos.y = player->slot->pos.y;
            } else if (player->pos.y <= player->slot->pos.y) {
                player->speed = (player->peakY - player->baseY) / 500.0f;
                player->pos.y = player->slot->pos.y;
                if (player->hook == fn_1_2080) {
                    CharMotionSet(player->charNo, player->obj->mtnId[2]);
                    player->state = 0;
                    player->onSlot = 1;
                    player->slot->active = 1.0f;
                    if (player->strong != 0) {
                        wait = 6;
                    } else {
                        wait = 3;
                    }
                    player->slot->waitFrames = wait;
                    if (player->speed > 1.0f) {
                        power = 1.0f;
                    } else {
                        power = player->speed;
                    }
                    player->slot->alpha = power;
                    player->hook = fn_1_22B4;
                    player->slot->hit = 1;
                } else if (player->hook == fn_1_237C) {
                    omVibrate(player->playerNo, 0xC, 6, 6);
                    CharMotionShiftSet(player->charNo, player->obj->mtnId[4], 0.0f, 3.0f, 0);
                    CharMotionSpeedSet(player->charNo, 1.5f);
                    player->state = 0;
                    player->speed *= 2.0f;
                    player->onSlot = 1;
                    player->slot->active = 1.0f;
                    if (player->strong != 0) {
                        wait2 = 9;
                    } else {
                        wait2 = 6;
                    }
                    player->slot->waitFrames = wait2;
                    if (player->speed > 1.0f) {
                        power2 = 1.0f;
                    } else {
                        power2 = player->speed;
                    }
                    player->slot->alpha = power2;
                    player->hook = fn_1_2488;
                    player->slot->hit = 1;
                } else if (player->hook == fn_1_1ECC) {
                    omVibrate(player->playerNo, 0xC, 6, 6);
                    CharMotionSet(player->charNo, player->obj->mtnId[2]);
                    player->state = 0;
                    player->onSlot = 1;
                    player->slot->active = 1.0f;
                    player->slot->waitFrames = 6;
                    player->slot->alpha = player->speed;
                    player->hook = fn_1_22B4;
                    player->slot->hit = 1;
                }
            }
            omSetTra(player->obj, player->pos.x, player->pos.y, player->pos.z);
            Hu3DModelPosSet(*player->obj->mdlId, player->pos.x, player->pos.y, player->pos.z);
        }
    }
}

void fn_1_36DC(M528SEG *seg)
{
    seg->pos.x = 0.0f;
    seg->pos.y = 0.0f;
    seg->pos.z = 0.0f;
    seg->progress = 0.0f;
    seg->angle = 0.0f;
    seg->state = 0;
    seg->mdl[0] = -1;
    seg->mdl[1] = -1;
    seg->mdl[2] = -1;
}

/* Turn one wheel sector while its team seesaw is pumped; returns non-zero once
 * the sector has made its full turn. */
s32 fn_1_3750(M528SEG *seg, f32 power)
{
    M528MAIN *main;
    f32 angle;
    s32 done;

    done = 0;
    seg->progress += (1.8f * power) / 360.0f;
    if (seg->progress >= 1.0f) {
        seg->progress = 1.0f;
        done = 1;
    }
    main = lbl_1_bss_298->data;
    angle = main->wheelAngle + (seg->angle - 360.0f * seg->progress);
    Hu3DModelRotSet(seg->mdl[0], 0.0f, angle, 0.0f);
    Hu3DModelRotSet(seg->mdl[1], 0.0f, angle, 0.0f);
    Hu3DModelRotSet(seg->mdl[2], 0.0f, angle, 0.0f);
    Hu3DMotionShapeTimeSet(seg->mdl[1], 1198.0f * seg->progress);
    Hu3DMotionShapeTimeSet(seg->mdl[2], 1198.0f * seg->progress);
    Hu3DMotionTimeSet(seg->mdl[2], 1198.0f * seg->progress);
    return done;
}

void fn_1_38F8(M528MAIN *work)
{
    s32 pick;

    work->teamPlayer[0][0]->partner = work->teamPlayer[0][1];
    work->teamPlayer[0][1]->partner = work->teamPlayer[0][0];
    work->teamPlayer[1][0]->partner = work->teamPlayer[1][1];
    work->teamPlayer[1][1]->partner = work->teamPlayer[1][0];
    pick = frandmod(2);
    work->teamPlayer[0][pick]->ai.timer = 0;
    work->teamPlayer[0][pick]->ai.wait = frandmod(0xF) + 0x1E;
    work->teamPlayer[0][pick]->ai.state = 1;
    pick = frandmod(2);
    work->teamPlayer[1][pick]->ai.timer = 0;
    work->teamPlayer[1][pick]->ai.wait = frandmod(0xF) + 0x1E;
    work->teamPlayer[1][pick]->ai.state = 1;
}

/* Place the wheel sector models on the circle at the current wheel angle. */
void fn_1_3A00(M528MAIN *work)
{
    f32 angle;
    s32 i;
    s16 mdl;
    M528SEG *seg;

    for (i = 0; i < 0x10; i++) {
        seg = &work->seg[i];
        angle = work->wheelAngle + seg->angle;
        work->seg[i].pos.x = 1480.0 * HuSin(angle);
        work->seg[i].pos.y = -620.0f;
        work->seg[i].pos.z = 1480.0 * HuCos(angle) - 1680.0;
        mdl = seg->mdl[0];
        Hu3DModelPosSet(mdl, work->seg[i].pos.x, work->seg[i].pos.y, work->seg[i].pos.z);
        Hu3DModelRotSet(mdl, 0.0f, angle, 0.0f);
        mdl = seg->mdl[1];
        Hu3DModelPosSet(mdl, work->seg[i].pos.x, work->seg[i].pos.y, work->seg[i].pos.z);
        Hu3DModelRotSet(mdl, 0.0f, angle, 0.0f);
        mdl = seg->mdl[2];
        Hu3DModelPosSet(mdl, work->seg[i].pos.x, work->seg[i].pos.y, work->seg[i].pos.z);
        Hu3DModelRotSet(mdl, 0.0f, angle, 0.0f);
    }
    mdl = work->mdlCache[4];
    Hu3DModelRotSet(mdl, 0.0f, work->wheelAngle, 0.0f);
    mdl = work->mdlCache[5];
    Hu3DModelRotSet(mdl, 0.0f, work->wheelAngle, 0.0f);
    mdl = work->mdlCache[7];
    Hu3DModelRotSet(mdl, 0.0f, work->wheelAngle, 0.0f);
}

/* Camera blend from view 0 to view 1 over 100 frames. */
s32 fn_1_3CA0(M528CAM *cam)
{
    f32 frac;
    f32 t;
    f32 u;
    s32 done;

    done = 0;
    switch (cam->state) {
        case 0:
            cam->timer = 0;
            cam->state++;
        case 1:
            cam->timer++;
            if (cam->timer >= 0x64) {
                cam->state = 0;
                done = 1;
            }
            frac = (f32)cam->timer / 100.0f;
            t = 1.0 - (0.5 + 0.5 * HuCos(180.0f * frac));
            u = 1.0f - t;
            cam->rotX = lbl_1_data_2BC[0][3] * u + lbl_1_data_2BC[1][3] * t;
            cam->rotY = lbl_1_data_2BC[0][4] * u + lbl_1_data_2BC[1][4] * t;
            cam->dist = lbl_1_data_2BC[0][5] * u + lbl_1_data_2BC[1][5] * t;
            cam->aimX = lbl_1_data_2BC[0][0] * u + lbl_1_data_2BC[1][0] * t;
            cam->aimY = lbl_1_data_2BC[0][1] * u + lbl_1_data_2BC[1][1] * t;
            cam->aimZ = lbl_1_data_2BC[0][2] * u + lbl_1_data_2BC[1][2] * t;
            break;
    }
    return done;
}

/* Camera blend from view 1 to view 2 over 90 frames. */
s32 fn_1_3EFC(M528CAM *cam)
{
    f32 frac;
    f32 t;
    f32 u;
    s32 done;

    done = 0;
    switch (cam->state) {
        case 0:
            cam->timer = 0;
            cam->state++;
        case 1:
            cam->timer++;
            if (cam->timer >= 0x5A) {
                cam->state = 0;
                done = 1;
            }
            frac = (f32)cam->timer / 90.0f;
            t = 1.0 - (0.5 + 0.5 * HuCos(180.0f * frac));
            u = 1.0f - t;
            cam->rotX = lbl_1_data_2BC[1][3] * u + lbl_1_data_2BC[2][3] * t;
            cam->rotY = lbl_1_data_2BC[1][4] * u + lbl_1_data_2BC[2][4] * t;
            cam->dist = lbl_1_data_2BC[1][5] * u + lbl_1_data_2BC[2][5] * t;
            cam->aimX = lbl_1_data_2BC[1][0] * u + lbl_1_data_2BC[2][0] * t;
            cam->aimY = lbl_1_data_2BC[1][1] * u + lbl_1_data_2BC[2][1] * t;
            cam->aimZ = lbl_1_data_2BC[1][2] * u + lbl_1_data_2BC[2][2] * t;
            break;
    }
    return done;
}

/* Camera blend from view 1 to view 3 over 90 frames. */
s32 fn_1_4158(M528CAM *cam)
{
    f32 frac;
    f32 t;
    f32 u;
    s32 done;

    done = 0;
    switch (cam->state) {
        case 0:
            cam->timer = 0;
            cam->state++;
        case 1:
            cam->timer++;
            if (cam->timer >= 0x5A) {
                cam->state = 0;
                done = 1;
            }
            frac = (f32)cam->timer / 90.0f;
            t = 1.0 - (0.5 + 0.5 * HuCos(180.0f * frac));
            u = 1.0f - t;
            cam->rotX = lbl_1_data_2BC[1][3] * u + lbl_1_data_2BC[3][3] * t;
            cam->rotY = lbl_1_data_2BC[1][4] * u + lbl_1_data_2BC[3][4] * t;
            cam->dist = lbl_1_data_2BC[1][5] * u + lbl_1_data_2BC[3][5] * t;
            cam->aimX = lbl_1_data_2BC[1][0] * u + lbl_1_data_2BC[3][0] * t;
            cam->aimY = lbl_1_data_2BC[1][1] * u + lbl_1_data_2BC[3][1] * t;
            cam->aimZ = lbl_1_data_2BC[1][2] * u + lbl_1_data_2BC[3][2] * t;
            break;
    }
    return done;
}

/* Camera shake (small), damped over 30 frames. */
s32 fn_1_43B4(M528CAM *cam)
{
    f32 t;
    s32 deg;
    s32 done;

    done = 0;
    switch (cam->state) {
        case 0:
            cam->timer = 0;
            cam->state++;
        case 1:
            cam->timer++;
            if (cam->timer >= 0x1E) {
                cam->state = 0;
                done = 1;
            }
            t = (f32)cam->timer / 30.0f;
            deg = (cam->timer * 0x24) % 360;
            cam->aimY = lbl_1_data_2BC[1][1] + (15.0f * (1.0f - t)) * HuSin(deg);
            break;
    }
    return done;
}

/* Camera shake (large), damped over 30 frames. */
s32 fn_1_4540(M528CAM *cam)
{
    f32 t;
    s32 deg;
    s32 done;

    done = 0;
    switch (cam->state) {
        case 0:
            cam->timer = 0;
            cam->state++;
        case 1:
            cam->timer++;
            if (cam->timer >= 0x1E) {
                cam->state = 0;
                done = 1;
            }
            t = (f32)cam->timer / 30.0f;
            deg = (cam->timer * 0x24) % 360;
            cam->aimY = lbl_1_data_2BC[1][1] + (30.0f * (1.0f - t)) * HuSin(deg);
            break;
    }
    return done;
}

/* Hand the two sectors the wheel stopped on to the teams, and hide every
 * sector decoration that is not next to them. */
void fn_1_46CC(M528MAIN *work)
{
    s32 adj[2];
    s32 i;
    s16 mdl;

    for (i = 0; i < 2; i++) {
        work->team[i].seg = &work->seg[work->sel[i]];
    }
    adj[0] = (work->sel[0] + 0xF) % 16;
    adj[1] = (work->sel[1] + 1) % 16;
    for (i = 0; i < 0x10; i++) {
        if (i != work->sel[0] && i != work->sel[1] && i != adj[0] && i != adj[1]) {
            mdl = work->seg[i].mdl[1];
            Hu3DModelAttrSet(mdl, 1);
            mdl = work->seg[i].mdl[2];
            Hu3DModelAttrSet(mdl, 1);
        }
    }
}

/* Team seesaw drops in from above. */
s32 fn_1_47F0(M528TEAM *team)
{
    s32 done;
    s16 mdl;

    done = 0;
    switch (team->state) {
        case 0:
            team->pivot.y = 1000.0f;
            team->timer = 0;
            team->state++;
        case 1:
            team->timer++;
            team->pivot.y -= 2.4f * (f32)team->timer;
            if (team->pivot.y < 0.0f) {
                team->pivot.y = 0.0f;
                done = 1;
            }
            team->slot[0].pos.y = team->pivot.y;
            team->slot[1].pos.y = team->pivot.y;
            mdl = team->mdl;
            Hu3DModelPosSet(mdl, team->pivot.x, team->pivot.y, team->pivot.z);
            mdl = team->slot[0].mdl;
            Hu3DModelPosSet(mdl, team->slot[0].pos.x, team->slot[0].pos.y, team->slot[0].pos.z);
            mdl = team->slot[1].mdl;
            Hu3DModelPosSet(mdl, team->slot[1].pos.x, team->slot[1].pos.y, team->slot[1].pos.z);
            mdl = team->spinMdl;
            Hu3DModelPosSet(mdl, team->pivot.x, team->pivot.y, team->pivot.z);
            break;
    }
    return done;
}

/* Losing team seesaw flies off the top of the screen. */
s32 fn_1_495C(M528TEAM *team)
{
    s32 done = 0;
    s16 pan[2] = { 0x30, 0x50 };
    M528MAIN *main;
    s16 mdl;

    main = lbl_1_bss_298->data;
    switch (team->state) {
        case 0:
            HuAudFXPlayPan(0x78F, pan[main->winner ^ 1]);
            team->timer = 0;
            team->state++;
        case 1:
            team->timer++;
            team->pivot.y += 20.0f - 2.4f * (f32)team->timer;
            if (team->pivot.y < -1000.0f) {
                done = 1;
            }
            team->slot[0].pos.y = team->pivot.y;
            team->slot[1].pos.y = team->pivot.y;
            mdl = team->mdl;
            Hu3DModelPosSet(mdl, team->pivot.x, team->pivot.y, team->pivot.z);
            mdl = team->slot[0].mdl;
            Hu3DModelPosSet(mdl, team->slot[0].pos.x, team->slot[0].pos.y, team->slot[0].pos.z);
            mdl = team->slot[1].mdl;
            Hu3DModelPosSet(mdl, team->slot[1].pos.x, team->slot[1].pos.y, team->slot[1].pos.z);
            mdl = team->spinMdl;
            Hu3DModelPosSet(mdl, team->pivot.x, team->pivot.y, team->pivot.z);
            break;
    }
    return done;
}

/* Winning sector pops off the wheel and flies sideways. */
s32 fn_1_4B04(M528SEG *seg)
{
    s32 done = 0;
    f32 drift[2] = { -7.0f, 7.0f };
    M528MAIN *main;
    s16 mdl;

    main = lbl_1_bss_298->data;
    switch (seg->state) {
        case 0:
            seg->pos2 = seg->pos;
            seg->timer = 0;
            seg->state++;
            HuAudFXPlay(0x78B);
        case 1:
            seg->timer++;
            if (seg->timer >= 0x78) {
                done = 1;
            }
            seg->pos2.y = seg->pos2.y + (100.0 - 2.4 * seg->timer);
            seg->pos2.x += drift[main->winner];
            seg->pos.z += 1.0f;
            mdl = seg->mdl[1];
            Hu3DModelPosSet(mdl, seg->pos2.x, seg->pos2.y, seg->pos2.z);
            break;
    }
    return done;
}

/* Winning sector sinks, then bobs. */
s32 fn_1_4C84(M528SEG *seg)
{
    s32 done = 0;
    s16 mdl;
    M528MAIN *volatile main = lbl_1_bss_298->data;

    switch (seg->state) {
        case 0:
            seg->timer = 0;
            seg->state++;
        case 1:
            seg->timer++;
            if (seg->timer == 3) {
                seg->state++;
            }
            break;
        case 2:
            seg->pos.y -= 5.0f;
            seg->timer++;
            if (seg->timer == 0x12) {
                seg->pos2 = seg->pos;
                seg->timer = 0;
                seg->state++;
            }
            mdl = seg->mdl[1];
            Hu3DModelPosSet(mdl, seg->pos.x, seg->pos.y, seg->pos.z);
            break;
        case 3:
            seg->timer++;
            if (seg->timer >= 5) {
                seg->timer = 0;
            }
            seg->pos2.y = seg->pos2.y + 2.0 * HuSin((360.0f * (f32)seg->timer) / 5.0f);
            mdl = seg->mdl[1];
            Hu3DModelPosSet(mdl, seg->pos2.x, seg->pos2.y, seg->pos2.z);
            break;
    }
    return done;
}

void fn_1_4E80(M528MAIN *work)
{
    s32 idx;
    s16 mdl;

    if (work->winner != 0) {
        idx = (work->sel[1] + 5) % 16;
    } else {
        idx = (work->sel[0] + 0xB) % 16;
    }
    mdl = work->seg[idx].mdl[1];
    Hu3DModelAttrReset(mdl, 1);
    mdl = work->seg[idx].mdl[2];
    Hu3DModelAttrReset(mdl, 1);
}

/* Chain the 256 list nodes into a free list. */
void fn_1_4F30(M528MAIN *work)
{
    s32 i;

    for (i = 0; i < 0xFF; i++) {
        work->node[i].prev = 0;
        work->node[i].next = &work->node[i + 1];
        work->node[i].func = 0;
        work->node[i].data = 0;
    }
    work->node[i].prev = 0;
    work->node[i].next = 0;
    work->node[i].func = 0;
    work->node[i].data = 0;
    work->nodeUsed = 0;
    work->nodeFree = &work->node[0];
}

/* Move the head of the free list onto the active list. */
M528NODE *fn_1_4FF4(M528MAIN *work, s32 (*func)(M528NODE *), M528PARTICLE *data)
{
    M528NODE *node = work->nodeFree;

    if (node != 0) {
        work->nodeFree = node->next;
        node->func = func;
        node->data = data;
        node->next = work->nodeUsed;

        if (node->next != 0) {
            node->next->prev = node;
        }
        work->nodeUsed = node;
    }
    return node;
}

/* Unlink a node from the active list and push it back onto the free list. */
void fn_1_5048(M528MAIN *work, M528NODE *node)
{
    if (node != 0) {
        if (node->prev != 0) {
            node->prev->next = node->next;
        } else {
            work->nodeUsed = node->next;
        }

        if (node->next != 0) {
            node->next->prev = node->prev;
        }
        node->prev = 0;
        node->next = work->nodeFree;
        node->func = 0;
        work->nodeFree = node;
    }
}

/* Run every active particle callback; a non-zero result releases the node. */
void fn_1_50B0(M528MAIN *work)
{
    M528NODE *node;
    M528NODE *next;

    node = work->nodeUsed;
    while (node != 0) {
        next = node->next;
        if (node->func != 0 && node->func(node) != 0) {
            fn_1_5048(work, node);
        }
        node = next;
    }
}

void fn_1_5194(M528PARTICLE *particle)
{
    particle->state = 0;
    particle->mdl = -1;
}

/* Create the stage: wheel, seesaw boards and pedals, then preload the models
 * of the particle effects. */
void fn_1_51A8(M528MAIN *work)
{
    s16 mdl;
    s32 i;
    s32 j;

    M528_GETMDL(mdl, work->mdlCache[4], 4);
    Hu3DModelAttrSet(mdl, 1);
    Hu3DModelAttrSet(mdl, 0x40000002);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelPosSet(mdl, 0.0f, -600.0f, -1680.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(mdl, 1);
    M528_GETMDL(mdl, work->mdlCache[8], 8);
    Hu3DModelAttrSet(mdl, 1);
    Hu3DModelAttrSet(mdl, 0x40000001);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelPosSet(mdl, 0.0f, -600.0f, -1680.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(mdl, 1);
    M528_GETMDL(mdl, work->mdlCache[5], 5);
    Hu3DModelAttrSet(mdl, 1);
    Hu3DModelAttrSet(mdl, 0x40000002);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelPosSet(mdl, 0.0f, -600.0f, -1680.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(mdl, 1);
    M528_GETMDL(mdl, work->mdlCache[6], 6);
    Hu3DModelAttrSet(mdl, 1);
    Hu3DModelAttrSet(mdl, 0x40000002);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelPosSet(mdl, 0.0f, -600.0f, -1680.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(mdl, 1);
    M528_GETMDL(mdl, work->mdlCache[7], 7);
    Hu3DModelAttrSet(mdl, 1);
    Hu3DModelAttrSet(mdl, 0x40000002);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelPosSet(mdl, 0.0f, -600.0f, -1680.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(mdl, 1);
    Hu3DReflectMapSet(HuDataSelHeapReadNum(0x56003D, HU_MEMNUM_OVL, HEAP_MODEL));
    fn_1_5BF8(work);
    for (i = 0; i < 2; i++) {
        M528_GETMDL(mdl, work->mdlCache[0], 0);
        work->team[i].mdl = mdl;
        work->team[i].tilt = 0.0f;
        work->team[i].unk5C = 0.0f;
        work->team[i].pivot.x = 600.0f * (f32)i - 300.0f;
        work->team[i].pivot.y = 2500.0f;
        work->team[i].pivot.z = 0.0f;
        work->team[i].state = 0;
        if (i == 0) {
            work->team[i].sndId = 0x789;
        } else {
            work->team[i].sndId = 0x78A;
        }
        Hu3DModelAttrSet(mdl, 1);
        Hu3DModelAttrSet(mdl, 0x40000002);
        Hu3DModelLayerSet(mdl, 2);
        Hu3DModelPosSet(mdl, work->team[i].pivot.x, work->team[i].pivot.y, work->team[i].pivot.z);
        Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrReset(mdl, 1);
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            M528_GETMDL(mdl, work->mdlCache[i + 1], i + 1);
            work->team[i].slot[j].mdl = mdl;
            work->team[i].slot[j].pos.x = (work->team[i].pivot.x + 300.0f * (f32)j) - 150.0f;
            work->team[i].slot[j].pos.y = work->team[i].pivot.y;
            work->team[i].slot[j].pos.z = work->team[i].pivot.z;
            work->team[i].slot[j].active = 0.0f;
            work->team[i].slot[j].alpha = 0.0f;
            work->team[i].slot[j].team = &work->team[i];
            work->team[i].slot[j].hit = 0;
            Hu3DModelAttrSet(mdl, 1);
            Hu3DModelAttrSet(mdl, 0x40000002);
            Hu3DModelLayerSet(mdl, 2);
            Hu3DModelPosSet(mdl, work->team[i].slot[j].pos.x, work->team[i].slot[j].pos.y, work->team[i].slot[j].pos.z);
            Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
            Hu3DModelAttrReset(mdl, 1);
            Hu3DModelShadowMapSet(mdl);
        }
    }
    for (i = 0; i < 2; i++) {
        M528_GETMDL(mdl, work->mdlCache[3], 3);
        work->team[i].spinMdl = mdl;
        work->team[i].spin = 0.0f;
        Hu3DModelAttrSet(mdl, 1);
        Hu3DModelAttrSet(mdl, 0x40000002);
        Hu3DModelLayerSet(mdl, 2);
        Hu3DModelPosSet(mdl, work->team[i].pivot.x, work->team[i].pivot.y, work->team[i].pivot.z);
        Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrReset(mdl, 1);
    }
    fn_1_6410(work);
    fn_1_654C(work);
    fn_1_6688(work);
    fn_1_67C4(work);
    fn_1_6900(work);
    fn_1_6A3C(work);
    fn_1_6B78(work);
    fn_1_6CB4(work);
    fn_1_B528(&work->walker[0], 0);
    fn_1_B528(&work->walker[1], 0x96);
    fn_1_B528(&work->walker[2], 0x14A);
    fn_1_B528(&work->walker[3], 0x384);
}

/* Build the 16 wheel sectors: a random type (weighted by prob[]) per sector,
 * plus two decoration models for each. */
void fn_1_5BF8(M528MAIN *work)
{
    s32 prob[8] = { 20, 20, 15, 15, 10, 10, 7, 3 };
    f32 angle;
    s16 mdl;
    s32 i;
    s16 mtn;
    s32 type;
    s32 acc;
    s32 rnd;

    for (i = 0; i < 0x10; i++) {
        rnd = frandmod(100);
        acc = 0;
        for (type = 0; type < 8; type++) {
            if (rnd < acc + prob[type]) {
                break;
            }
            acc += prob[type];
        }
        M528_GETMDL(mdl, work->mdlCache[type + 0xD], type + 0xD);
        work->seg[i].mdl[0] = mdl;
        angle = 22.5f * (f32)i - 11.25f;
        work->seg[i].pos.x = 1480.0 * HuSin(angle);
        work->seg[i].pos.y = -620.0f;
        work->seg[i].pos.z = 1480.0 * HuCos(angle) - 1680.0;
        work->seg[i].angle = angle;
        work->seg[i].type = type;
        Hu3DModelAttrSet(mdl, 1);
        Hu3DModelAttrSet(mdl, 0x40000002);
        Hu3DModelLayerSet(mdl, 1);
        Hu3DModelPosSet(mdl, work->seg[i].pos.x, work->seg[i].pos.y, work->seg[i].pos.z);
        Hu3DModelRotSet(mdl, 0.0f, work->seg[i].angle, 0.0f);
        Hu3DModelAttrReset(mdl, 1);
        if (type == 7) {
            M528_GETMDL(mdl, work->segMdlCache[i], 0xB);
            M528_GETMTN(mtn, work->mtnCache[2], mdl, lbl_1_data_2A0[2]);
            Hu3DMotionSet(mdl, mtn);
        } else {
            M528_GETMDL(mdl, work->segMdlCache[i], 9);
            M528_GETMTN(mtn, work->mtnCache[0], mdl, lbl_1_data_2A0[0]);
            Hu3DMotionSet(mdl, mtn);
        }
        work->seg[i].mdl[1] = mdl;
        Hu3DModelAttrSet(mdl, 1);
        Hu3DModelAttrSet(mdl, 0x40000002);
        Hu3DModelAttrSet(mdl, 0x40000080);
        Hu3DModelLayerSet(mdl, 1);
        Hu3DModelPosSet(mdl, work->seg[i].pos.x, work->seg[i].pos.y, work->seg[i].pos.z);
        Hu3DModelRotSet(mdl, 0.0f, work->seg[i].angle, 0.0f);
        Hu3DMotionSet(mdl, mtn);
        Hu3DMotionShapeSet(mdl, mtn);
        Hu3DModelAttrReset(mdl, 1);
        if (type == 7) {
            M528_GETMDL(mdl, work->segMdlCache2[i], 0xC);
            M528_GETMTN(mtn, work->mtnCache[3], mdl, lbl_1_data_2A0[3]);
            Hu3DMotionSet(mdl, mtn);
        } else {
            M528_GETMDL(mdl, work->segMdlCache2[i], 0xA);
            M528_GETMTN(mtn, work->mtnCache[1], mdl, lbl_1_data_2A0[1]);
            Hu3DMotionSet(mdl, mtn);
        }
        work->seg[i].mdl[2] = mdl;
        Hu3DModelAttrSet(mdl, 1);
        Hu3DModelAttrSet(mdl, 0x40000002);
        Hu3DModelAttrSet(mdl, 0x40000080);
        Hu3DModelLayerSet(mdl, 1);
        Hu3DModelPosSet(mdl, work->seg[i].pos.x, work->seg[i].pos.y, work->seg[i].pos.z);
        Hu3DModelRotSet(mdl, 0.0f, work->seg[i].angle, 0.0f);
        Hu3DMotionSet(mdl, mtn);
        Hu3DMotionShapeSet(mdl, mtn);
        Hu3DModelAttrReset(mdl, 1);
    }
}

void fn_1_6320(M528MAIN *work)
{
    s16 mdl;

    M528_GETMDL(mdl, work->mdlCache[0x15], 0x15);
    Hu3DModelAttrSet(mdl, 1);
    Hu3DModelAttrSet(mdl, 0x40000002);
    Hu3DModelLayerSet(mdl, 2);
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
}

/* Preload (and hide) the models of one effect group. */
#define M528_PRELOAD(name, first, count)                                      \
    void name(M528MAIN *work)                                                 \
    {                                                                         \
        s16 mdl;                                                              \
        s32 i;                                                                \
                                                                              \
        for (i = 0; i < (count); i++) {                                       \
            M528_GETMDL(mdl, work->mdlCache[i + (first)], i + (first));       \
            Hu3DModelAttrSet(mdl, 1);                                         \
            Hu3DModelAttrSet(mdl, 0x40000002);                                \
            Hu3DModelLayerSet(mdl, 2);                                        \
            Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);                           \
            Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);                           \
        }                                                                     \
    }

M528_PRELOAD(fn_1_6410, 0x16, 8)
M528_PRELOAD(fn_1_654C, 0x1E, 6)
M528_PRELOAD(fn_1_6688, 0x24, 5)
M528_PRELOAD(fn_1_67C4, 0x29, 5)
M528_PRELOAD(fn_1_6900, 0x2E, 1)
M528_PRELOAD(fn_1_6A3C, 0x2F, 1)
M528_PRELOAD(fn_1_6B78, 0x30, 4)
M528_PRELOAD(fn_1_6CB4, 0x34, 2)

#define M528_RND() ((f32)frandmod(0x2711) / 10000.0f)

/* ---- particle callbacks: run through the M528NODE list by fn_1_50B0 ---- */

s32 fn_1_6DF0(M528NODE *node)
{
    s32 done = 0;
    M528PARTICLE *p;
    M528MAIN *work;
    f32 dist;
    f32 ang;
    f32 up;
    s16 mdl;
    s32 type;

#ifdef TARGET_PC
    type = 0;
#endif
    work = lbl_1_bss_298->data;
    p = node->data;
    switch (p->state) {
        case 0:
            if (p->mdl < 0) {
                type = frandmod(8);
                M528_GETMDL(mdl, work->mdlCache[type + 0x16], type + 0x16);
                p->mdl = mdl;
            } else {
                mdl = p->mdl;
            }
            Hu3DModelAttrSet(mdl, 1);
            Hu3DModelAttrSet(mdl, 0x40000002);
            Hu3DModelLayerSet(mdl, 2);
            dist = (f32)frandmod(0x46);
            ang = (f32)(frandmod(0x55) - 0x2A);
            p->pos.x = (600.0f * (f32)work->winner + dist * HuSin(ang)) - 300.0;
            p->pos.y = -300.0f;
            p->pos.z = (dist * HuCos(ang)) - 220.0;
            p->rot.x = 0.0f;
            if (type < 5) {
                p->rot.y = (f32)frandmod(0xB4);
                p->rot.z = (f32)frandmod(0x1F) - 15.0f;
            } else {
                p->rot.y = 0.0f;
                p->rot.z = 0.0f;
            }
            dist = (f32)(frandmod(3) + 6);
            up = (f32)(frandmod(0x28) + 0x28);
            p->vel.x = dist * HuSin(ang);
            p->vel.y = up;
            p->vel.z = 0.1f + dist * HuCos(ang);
            p->timer = 0;
            p->duration = frandmod(0x1E) + 1;
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            p->timer = 0;
            p->state++;
            break;
        case 1:
            p->timer++;
            if (p->timer == p->duration) {
                Hu3DModelAttrReset(p->mdl, 1);
                p->state++;
            }
            break;
        case 2:
            p->vel.y -= 0.9f;
            p->pos.x += p->vel.x;
            p->pos.y += p->vel.y;
            p->pos.z += p->vel.z;
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            break;
    }
    return done;
}

s32 fn_1_72E4(M528NODE *node)
{
    M528PARTICLE *p;
    M528MAIN *work;
    f32 dist;
    f32 ang;
    f32 up;
    s16 mdl;
    s32 type;

    work = lbl_1_bss_298->data;
    p = node->data;
    switch (p->state) {
        case 0:
            if (p->mdl < 0) {
                type = frandmod(5);
                M528_GETMDL(mdl, work->mdlCache[type + 0x1F], type + 0x1F);
                p->mdl = mdl;
            } else {
                mdl = p->mdl;
            }
            Hu3DModelAttrSet(mdl, 1);
            Hu3DModelAttrSet(mdl, 0x40000002);
            Hu3DModelLayerSet(mdl, 2);
            dist = (f32)frandmod(0x32);
            ang = (f32)(frandmod(0x55) - 0x2A);
            p->pos.x = (600.0f * (f32)work->winner + dist * HuSin(ang)) - 300.0;
            p->pos.y = -300.0f;
            p->pos.z = (dist * HuCos(ang)) - 210.0;
            p->rot.x = 0.0f;
            p->rot.y = (f32)frandmod(0xB4);
            p->rot.z = (f32)frandmod(0x1F) - 15.0f;
            dist = (f32)(frandmod(4) + 5);
            up = (f32)(frandmod(0x28) + 0x28);
            p->vel.x = dist * HuSin(ang);
            p->vel.y = up;
            p->vel.z = dist * HuCos(ang);
            p->timer = 0;
            p->duration = frandmod(0x1E) + 1;
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            p->state++;
            break;
        case 1:
            p->timer++;
            if (p->timer == p->duration) {
                Hu3DModelAttrReset(p->mdl, 1);
                p->state++;
            }
            break;
        case 2:
            p->vel.y -= 0.9f;
            p->pos.x += p->vel.x;
            p->pos.y += p->vel.y;
            p->pos.z += p->vel.z;
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            break;
    }
    return 0;
}

s32 fn_1_7794(M528NODE *node)
{
    M528PARTICLE *p;
    M528MAIN *work;
    f32 ang;
    f32 up;
    f32 spd;
    s16 mdl;
    s32 ispd;
    s32 iang;
    s32 iup;

    work = lbl_1_bss_298->data;
    p = node->data;
    switch (p->state) {
        case 0:
            if (p->mdl < 0) {
                M528_GETMDL(mdl, work->mdlCache[0x1E], 0x1E);
                p->mdl = mdl;
            } else {
                mdl = p->mdl;
            }
            Hu3DModelAttrSet(mdl, 1);
            Hu3DModelAttrSet(mdl, 0x40000002);
            Hu3DModelLayerSet(mdl, 2);
            p->pos.x = 600.0f * (f32)work->winner - 300.0f;
            p->pos.y = -300.0f;
            p->pos.z = -200.0f;
            p->rot.x = 0.0f;
            p->rot.y = (f32)frandmod(0xB4);
            p->rot.z = (f32)frandmod(0x1F) - 15.0f;
            spd = (f32)0xA;
            iang = 0;
            ang = (f32)iang;
            p->vel.x = spd * HuSin(ang);
            iup = 0x46;
            up = (f32)iup;
            p->vel.y = up;
            p->vel.z = spd * HuCos(ang);
            p->timer = 0;
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            p->state++;
            break;
        case 1:
            p->timer++;
            if (p->timer == 0x50) {
                Hu3DModelAttrReset(p->mdl, 1);
                p->state++;
            }
            break;
        case 2:
            p->vel.y -= 2.4f;
            p->pos.x += p->vel.x;
            p->pos.y += p->vel.y;
            p->pos.z += p->vel.z;
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            break;
    }
    return 0;
}

s32 fn_1_7B44(M528NODE *node)
{
    M528PARTICLE *p;
    M528MAIN *work;
    f32 scale;
    f32 ang;
    s16 mdl;
    s16 mtn;
    s32 type;

    work = lbl_1_bss_298->data;
    p = node->data;
    switch (p->state) {
        case 0:
            if (p->mdl < 0) {
                type = frandmod(5);
                M528_GETMDL(mdl, work->mdlCache[type + 0x29], type + 0x29);
                M528_GETMTN(mtn, work->mtnCache[4], mdl, lbl_1_data_2A0[4]);
                Hu3DMotionSet(mdl, mtn);
                p->mdl = mdl;
            } else {
                mdl = p->mdl;
            }
            Hu3DModelAttrSet(mdl, 1);
            Hu3DModelAttrSet(mdl, 0x40000001);
            Hu3DModelAttrReset(mdl, 0x40000002);
            Hu3DModelLayerSet(mdl, 2);
            scale = 1.0f - 0.2f * M528_RND();
            Hu3DModelScaleSet(mdl, scale, scale, scale);
            ang = 15.0f * (f32)p->timer;
            p->pos.x = (600.0f * (f32)work->winner + 70.0 * HuSin(ang)) - 300.0;
            p->pos.y = -200.0f;
            p->pos.z = (70.0 * HuCos(ang)) - 200.0;
            p->rot.x = 0.0f;
            p->rot.y = 360.0f * M528_RND();
            p->rot.z = 0.0f;
            p->vel.x = 3.0f * HuSin(ang);
            p->vel.y = 11.0f;
            p->vel.z = 0.0f;
            p->timer = 0;
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            p->state++;
            break;
        case 1:
            p->timer++;
            if (p->timer == p->duration) {
                Hu3DModelAttrReset(p->mdl, 1);
                p->state++;
            }
            break;
        case 2:
            p->pos.x += p->vel.x;
            p->pos.y += p->vel.y;
            p->pos.z += p->vel.z;
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            break;
    }
    return 0;
}

s32 fn_1_800C(M528NODE *node)
{
    M528PARTICLE *p;
    M528MAIN *work;
    s16 mdl;
    s16 mtn;

    work = lbl_1_bss_298->data;
    p = node->data;
    switch (p->state) {
        case 0:
            if (p->mdl < 0) {
                M528_GETMDL(mdl, work->mdlCache[0x2F], 0x2F);
                M528_GETMTN(mtn, work->mtnCache[6], mdl, lbl_1_data_2A0[6]);
                Hu3DMotionSet(mdl, mtn);
                p->mdl = mdl;
            } else {
                mdl = p->mdl;
            }
            Hu3DModelAttrSet(mdl, 1);
            Hu3DModelAttrSet(mdl, 0x40000001);
            Hu3DModelAttrReset(mdl, 0x40000002);
            Hu3DModelLayerSet(mdl, 2);
            p->pos.x = (600.0f * (f32)work->winner) - 300.0f;
            p->pos.y = -400.0f;
            p->pos.z = -200.0f;
            p->rot.x = -10.0f;
            p->rot.y = 0.0f;
            p->rot.z = 0.0f;
            p->vel.x = 0.0f;
            p->vel.y = 45.0f;
            p->vel.z = 0.0f;
            p->timer = 0;
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            p->state++;
            break;
        case 1:
            Hu3DModelAttrReset(p->mdl, 1);
            p->state++;
        case 2:
            p->vel.y -= 1.5f;
            if (p->vel.y < 0.0f) {
                p->timer = 0;
                p->vel.y = 0.0f;
                p->state++;
            }
            p->pos.y += p->vel.y;
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            break;
        case 3:
            p->timer++;
            if (p->timer == 5) {
                p->state++;
            }
            break;
        case 4:
            p->vel.y -= 1.5f;
            p->pos.y += p->vel.y;
            if (p->pos.y < -400.0f) {
                HuAudFXPlay(0x2C0);
                work->camFunc = fn_1_4540;
                p->timer = 0;
                p->state++;
            }
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            break;
        case 5:
            p->timer++;
            if (p->timer == 7) {
                p->timer = 0;
                p->vel.y = 45.0f;
                p->state = 2;
            }
            break;
    }
    return 0;
}

s32 fn_1_83E4(M528NODE *node)
{
    M528PARTICLE *p;
    M528MAIN *work;
    f32 rotY;
    f32 velX;
    s16 mdl;

    work = lbl_1_bss_298->data;
    p = node->data;
    switch (p->state) {
        case 0:
            if (p->mdl < 0) {
                M528_GETMDL(mdl, work->mdlCache[0x30], 0x30);
                p->mdl = mdl;
            } else {
                mdl = p->mdl;
            }
            Hu3DModelAttrSet(mdl, 1);
            Hu3DModelAttrSet(mdl, 0x40000002);
            Hu3DModelLayerSet(mdl, 2);
            p->pos.x = (600.0f * (f32)work->winner) - 300.0f;
            p->pos.y = -200.0f;
            p->pos.z = -200.0f;
            p->rot.x = 0.0f;
            if (work->winner != 0) {
                rotY = -20.0f;
            } else {
                rotY = 20.0f;
            }
            p->rot.y = rotY;
            p->rot.z = 0.0f;
            if (work->winner != 0) {
                velX = 3.7f;
            } else {
                velX = -3.7f;
            }
            p->vel.x = velX;
            p->vel.y = 55.0f;
            p->vel.z = 1.0f;
            p->unk2C = 5.0f;
            p->timer = 0;
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            p->state++;
            break;
        case 1:
            p->timer++;
            if (p->timer == 5) {
                Hu3DModelAttrReset(p->mdl, 1);
                p->state++;
            }
            break;
        case 2:
            p->vel.y -= 1.2f;
            if (p->pos.y > 100.0f) {
                p->vel.x *= 1.5f;
                p->state++;
            }
            p->pos.x += p->vel.x;
            p->pos.y += p->vel.y;
            p->pos.z += p->vel.z;
            p->rot.z += p->unk2C;
            if (p->rot.z > 360.0f) {
                p->rot.x -= 360.0f;
            }
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            break;
        case 3:
            p->vel.y -= 1.2f;
            p->pos.x += p->vel.x;
            p->pos.y += p->vel.y;
            p->pos.z += p->vel.z;
            p->rot.z += p->unk2C;
            if (p->rot.z > 360.0f) {
                p->rot.z -= 360.0f;
            }
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            break;
    }
    return 0;
}

s32 fn_1_87F4(M528NODE *node)
{
    M528PARTICLE *p;
    M528MAIN *work;
    f32 base;
    f32 ang;
    f32 spd;
    s16 mdl;
    s32 type;

    work = lbl_1_bss_298->data;
    p = node->data;
    switch (p->state) {
        case 0:
            if (p->mdl < 0) {
                type = frandmod(3);
                M528_GETMDL(mdl, work->mdlCache[type + 0x31], type + 0x31);
                p->mdl = mdl;
            } else {
                mdl = p->mdl;
            }
            Hu3DModelAttrSet(mdl, 1);
            Hu3DModelAttrSet(mdl, 0x40000002);
            Hu3DModelLayerSet(mdl, 2);
            p->pos.x = 600.0f * (f32)work->winner - 300.0f;
            p->pos.y = -200.0f;
            p->pos.z = -200.0f;
            base = 84.0f * M528_RND();
            ang = base - 42.0f;
            p->rot.x = 0.0f;
            p->rot.y = ang;
            p->rot.z = 0.0f;
            spd = 6.1f + 3.3f * M528_RND();
            p->vel.x = spd * HuSin(ang);
            p->vel.y = 40.0f + 20.0f * M528_RND();
            p->vel.z = spd * HuCos(ang);
            base = 3.0f * M528_RND();
            p->unk2C = -5.0f - base;
            p->timer = 0;
            p->duration = (frandmod(0x3C) * 2) + 0x1E;
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            p->state++;
            break;
        case 1:
            p->timer++;
            if (p->timer == p->duration) {
                Hu3DModelAttrReset(p->mdl, 1);
                p->state++;
            }
            break;
        case 2:
            p->vel.y -= 1.2f;
            p->pos.x += p->vel.x;
            p->pos.y += p->vel.y;
            p->pos.z += p->vel.z;
            p->rot.z += p->unk2C;
            if (p->rot.z > 360.0f) {
                p->rot.x -= 360.0f;
            }
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            break;
    }
    return 0;
}

s32 fn_1_8CD0(M528NODE *node)
{
    M528PARTICLE *p;
    M528MAIN *work;
    s16 mdl;
    s16 mdl2;

    work = lbl_1_bss_298->data;
    p = node->data;
    switch (p->state) {
        case 0:
            M528_GETMDL(mdl, work->mdlCache[0x34], 0x34);
            p->mdl = mdl;
            Hu3DModelAttrSet(mdl, 1);
            Hu3DModelAttrSet(mdl, 0x40000001);
            Hu3DModelAttrReset(mdl, 0x40000002);
            Hu3DModelLayerSet(mdl, 2);
            Hu3DModelScaleSet(mdl, 2.7f, 2.7f, 2.7f);
            M528_GETMDL(mdl2, work->mdlCache[0x35], 0x35);
            p->duration = mdl2;
            Hu3DModelAttrSet(mdl2, 1);
            Hu3DModelAttrSet(mdl2, 0x40000001);
            Hu3DModelAttrReset(mdl2, 0x40000002);
            Hu3DModelLayerSet(mdl2, 2);
            Hu3DModelScaleSet(mdl2, 2.7f, 2.7f, 2.7f);
            p->pos.x = 600.0f * (f32)work->winner - 300.0f;
            p->pos.y = -400.0f;
            p->pos.z = -200.0f;
            p->rot.x = 0.0f;
            p->rot.y = 0.0f;
            p->rot.z = 0.0f;
            p->timer = 0;
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            Hu3DModelPosSetV(p->duration, &p->pos);
            Hu3DModelRotSetV(p->duration, &p->rot);
            Hu3DModelAttrReset(p->mdl, 1);
            Hu3DModelAttrReset(p->duration, 1);
            p->state++;
            break;
        case 1:
            p->timer++;
            if (p->timer == 5) {
                p->state++;
            }
            break;
        case 2:
            if (p->pos.y >= -40.0f) {
                p->state++;
            }
            p->pos.y += 5.0f;
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelPosSetV(p->duration, &p->pos);
            break;
    }
    return 0;
}

s32 fn_1_8FFC(M528NODE *node)
{
    M528PARTICLE *p;
    M528MAIN *work;
    HSFDATA *hsf;
    HSFMATERIAL *mat;
    f32 dist;
    f32 ang;
    f32 up;
    s16 mdl;
    s16 matNum;
    s32 done;
    s32 i;

    done = 0;
    work = lbl_1_bss_298->data;
    p = node->data;
    switch (p->state) {
        case 0:
            if (p->mdl < 0) {
                M528_GETMDL(mdl, work->mdlCache[0x15], 0x15);
                p->mdl = mdl;
            } else {
                mdl = p->mdl;
            }
            Hu3DModelAttrSet(mdl, 1);
            Hu3DModelAttrSet(mdl, 0x40000002);
            Hu3DModelLayerSet(mdl, 2);
            dist = 135.0f * M528_RND();
            ang = (84.0f * M528_RND()) - 42.0f;
            p->pos.x = (600.0f * (f32)work->winner + dist * HuSin(ang)) - 300.0;
            p->pos.y = -300.0f;
            p->pos.z = (dist * HuCos(ang)) - 210.0;
            p->rot.x = 0.0f;
            p->rot.y = 180.0f * M528_RND();
            p->rot.z = 0.0f;
            p->timer = 0;
            dist = 4.0f + (4.0f * M528_RND());
            up = 40.0f + 40.0f * M528_RND();
            p->vel.x = dist * HuSin(ang);
            p->vel.y = up;
            p->vel.z = 0.1f + dist * HuCos(ang);
            p->unk24 = 1.0f + (4.0f * M528_RND());
            p->unk28 = 1.0f + (4.0f * M528_RND());
            p->unk2C = 0.0f;
            hsf = Hu3DData[p->mdl].hsf;
            mat = hsf->material;
            matNum = hsf->materialNum;
            for (i = 0; i < matNum; i++) {
                mat->color[0] = frandmod(0x80) + 0x80;
                mat->color[1] = frandmod(0x80) + 0x80;
                mat->color[2] = frandmod(0x80) + 0x80;
                mat++;
            }
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            p->timer = 0;
            p->duration = frandmod(5) + 1;
            p->state++;
        case 1:
            p->timer++;
            if (p->timer == p->duration) {
                Hu3DModelAttrReset(p->mdl, 1);
                p->state++;
            }
            break;
        case 2:
            p->vel.y -= 0.9f;
            if (p->vel.y < 0.0f) {
                p->state++;
            }
            p->pos.x += p->vel.x;
            p->pos.y += p->vel.y;
            p->pos.z += p->vel.z;
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            break;
        case 3:
            p->vel.y = -3.0f - ((30.0f * M528_RND()) / 10.0f);
            p->state++;
        case 4:
            p->vel.x *= 0.9f;
            p->vel.z *= 0.9f;
            p->pos.x += p->vel.x;
            p->pos.y += p->vel.y;
            p->pos.z += p->vel.z;
            p->rot.x += p->unk24;
            p->rot.y += p->unk28;
            p->rot.z += p->unk2C;
            if (p->pos.y < -800.0f) {
                done = 1;
            }
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            break;
    }
    return done;
}

s32 fn_1_97C8(M528NODE *node)
{
    M528PARTICLE *p;
    M528MAIN *work;
    f32 dist;
    f32 ang;
    f32 up;
    s16 mdl;
    s16 mtn;

    work = lbl_1_bss_298->data;
    p = node->data;
    switch (p->state) {
        case 0:
            if (p->mdl < 0) {
                M528_GETMDL(mdl, work->mdlCache[0x2E], 0x2E);
                M528_GETMTN(mtn, work->mtnCache[5], mdl, lbl_1_data_2A0[5]);
                Hu3DMotionSet(mdl, mtn);
                p->mdl = mdl;
            } else {
                mdl = p->mdl;
            }
            Hu3DModelAttrSet(mdl, 1);
            Hu3DModelAttrSet(mdl, 0x40000001);
            Hu3DModelAttrReset(mdl, 0x40000002);
            Hu3DModelLayerSet(mdl, 2);
            ang = (f32)p->timer;
            dist = 50.0f * M528_RND();
            p->pos.x = (600.0f * (f32)work->winner + dist * HuSin(ang)) - 300.0;
            p->pos.y = -300.0f;
            p->pos.z = (dist * HuCos(ang)) - 210.0;
            p->rot.x = 0.0f;
            p->rot.y = 360.0f * M528_RND();
            p->rot.z = 0.0f;
            dist = 4.3f + 2.0f * M528_RND();
            if (ang > 90.0f && ang < 270.0f) {
                dist *= 0.7f;
            }
            up = 45.0f + 40.0f * M528_RND();
            p->vel.x = dist * HuSin(ang);
            p->vel.y = up;
            p->vel.z = dist * HuCos(ang);
            p->timer = 0;
            p->duration = (frandmod(6) * 5) + 1;
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            p->timer = 0;
            p->state++;
            break;
        case 1:
            p->timer++;
            if (p->timer == p->duration) {
                Hu3DModelAttrReset(p->mdl, 1);
                p->state++;
            }
            break;
        case 2:
            p->vel.y -= 0.9f;
            p->pos.x += p->vel.x;
            p->pos.y += p->vel.y;
            p->pos.z += p->vel.z;
            if (p->pos.y < -700.0f) {
                p->state++;
            }
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            break;
    }
    return 0;
}

s32 fn_1_9D5C(M528NODE *node)
{
    M528PARTICLE *p;
    M528MAIN *work;
    f32 scale;
    f32 dist;
    f32 ang;
    f32 up;
    s16 mdl;
    s32 type;

    work = lbl_1_bss_298->data;
    p = node->data;
    switch (p->state) {
        case 0:
            if (p->mdl < 0) {
                type = frandmod(3);
                if (type == 2) {
            type = 4;
                }
                M528_GETMDL(mdl, work->mdlCache[type + 0x24], type + 0x24);
                p->mdl = mdl;
            } else {
                mdl = p->mdl;
            }
            Hu3DModelAttrSet(mdl, 1);
            Hu3DModelAttrSet(mdl, 0x40000001);
            Hu3DModelAttrReset(mdl, 0x40000002);
            Hu3DModelLayerSet(mdl, 2);
            scale = 0.7f + M528_RND();
            Hu3DModelScaleSet(mdl, scale, scale, 1.0f);
            ang = (84.0f * M528_RND()) - 42.0f;
            dist = 40.0f * M528_RND();
            p->pos.x = (600.0f * (f32)work->winner + dist * HuSin(ang)) - 300.0;
            p->pos.y = -300.0f;
            p->pos.z = (dist * HuCos(ang)) - 210.0;
            p->rot.x = 0.0f;
            p->rot.y = 0.0f;
            p->rot.z = 0.0f;
            dist = 4.2f + (4.0f * M528_RND());
            up = 35.0f + 28.0f * M528_RND();
            p->vel.x = dist * HuSin(ang);
            p->vel.y = up;
            p->vel.z = dist * HuCos(ang);
            p->unk2C = 5.0f + (5.0f * M528_RND());
            p->timer = 0;
            p->duration = (frandmod(6) * 5) + 1;
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            p->timer = 0;
            p->state++;
            break;
        case 1:
            p->timer++;
            if (p->timer == p->duration) {
                Hu3DModelAttrReset(p->mdl, 1);
                p->state++;
            }
            break;
        case 2:
            p->vel.y -= 0.9f;
            p->pos.x += p->vel.x;
            p->pos.y += p->vel.y;
            p->pos.z += p->vel.z;
            p->rot.z += p->unk2C;
            if (p->rot.z >= 360.0f) {
                p->rot.z -= 360.0f;
            }
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            break;
    }
    return 0;
}

s32 fn_1_A3A0(M528NODE *node)
{
    u8 col[3];
    M528PARTICLE *p;
    M528MAIN *work;
    HSFDATA *hsf;
    HSFMATERIAL *mat;
    f32 scale;
    f32 dist;
    f32 ang;
    f32 up;
    f32 rnd2;
    s16 mdl;
    s16 matNum;
    s16 pick;
    s32 done;
    s32 i;

    done = 0;
    work = lbl_1_bss_298->data;
    p = node->data;
    switch (p->state) {
        case 0:
            if (p->mdl < 0) {
                M528_GETMDL(mdl, work->mdlCache[0x26], 0x26);
                p->mdl = mdl;
            } else {
                mdl = p->mdl;
            }
            Hu3DModelAttrSet(mdl, 1);
            Hu3DModelAttrSet(mdl, 0x40000002);
            Hu3DModelLayerSet(mdl, 2);
            scale = 0.1f + 0.5f * M528_RND();
            Hu3DModelScaleSet(mdl, scale, scale, scale);
            dist = 100.0f * M528_RND();
            ang = (84.0f * M528_RND()) - 42.0f;
            p->pos.x = (600.0f * (f32)work->winner + dist * HuSin(ang)) - 300.0;
            p->pos.y = -300.0f;
            p->pos.z = (dist * HuCos(ang)) - 210.0;
            rnd2 = M528_RND();
            p->rot.x = 350.0f - 60.0f * rnd2;
            p->rot.y = 180.0f * M528_RND();
            p->rot.z = 0.0f;
            p->timer = 0;
            dist = 4.0f + (4.0f * M528_RND());
            up = 30.0f + 30.0f * M528_RND();
            p->vel.x = dist * HuSin(ang);
            p->vel.y = up;
            p->vel.z = dist * HuCos(ang);
            p->unk24 = rnd2;
            p->unk28 = 4.0f + (8.0f * M528_RND());
            p->unk2C = 0.0f;
            hsf = Hu3DData[p->mdl].hsf;
            mat = hsf->material;
            matNum = hsf->materialNum;
            pick = frandmod(3);
            col[0] = 0x60;
            col[1] = 0x60;
            col[2] = 0x60;
            col[pick] = frandmod(0x80) + col[pick] + 0x20;
            for (i = 0; i < matNum; i++) {
                mat->color[0] = col[0];
                mat->color[1] = col[1];
                mat->color[2] = col[2];
                mat++;
            }
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            p->timer = 0;
            p->duration = (frandmod(3) * 3) + 1;
            p->state++;
        case 1:
            p->timer++;
            if (p->timer == p->duration) {
                Hu3DModelAttrReset(p->mdl, 1);
                p->state++;
            }
            break;
        case 2:
            p->vel.y -= 0.9f;
            if (p->vel.y < 0.0f) {
                p->state++;
            }
            p->pos.x += p->vel.x;
            p->pos.y += p->vel.y;
            p->pos.z += p->vel.z;
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            break;
        case 3:
            p->vel.y = -4.0f - ((30.0f * p->unk24) / 10.0f);
            p->state++;
        case 4:
            p->vel.x *= 0.9f;
            p->vel.z *= 0.9f;
            p->pos.x += p->vel.x;
            p->pos.y += p->vel.y;
            p->pos.z += p->vel.z;
            p->rot.y += p->unk28;
            if (p->pos.y < -800.0f) {
                done = 1;
            }
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            break;
    }
    return done;
}

s32 fn_1_ABAC(M528NODE *node)
{
    M528PARTICLE *p;
    M528MAIN *work;
    f32 scale;
    f32 dist;
    f32 ang;
    f32 up;
    f32 rnd2;
    s16 mdl;
    s32 done;

    done = 0;
    work = lbl_1_bss_298->data;
    p = node->data;
    switch (p->state) {
        case 0:
            if (p->mdl < 0) {
                M528_GETMDL(mdl, work->mdlCache[0x27], 0x27);
                p->mdl = mdl;
            } else {
                mdl = p->mdl;
            }
            Hu3DModelAttrSet(mdl, 1);
            Hu3DModelAttrSet(mdl, 0x40000002);
            Hu3DModelLayerSet(mdl, 2);
            scale = 0.1f + 0.5f * M528_RND();
            Hu3DModelScaleSet(mdl, scale, scale, scale);
            dist = 100.0f * M528_RND();
            ang = (84.0f * M528_RND()) - 42.0f;
            p->pos.x = (600.0f * (f32)work->winner + dist * HuSin(ang)) - 300.0;
            p->pos.y = -300.0f;
            p->pos.z = (dist * HuCos(ang)) - 210.0;
            rnd2 = M528_RND();
            p->rot.x = 350.0f - 60.0f * rnd2;
            p->rot.y = 180.0f * M528_RND();
            p->rot.z = 0.0f;
            p->timer = 0;
            dist = 4.0f + (4.0f * M528_RND());
            up = 30.0f + 30.0f * M528_RND();
            p->vel.x = dist * HuSin(ang);
            p->vel.y = up;
            p->vel.z = dist * HuCos(ang);
            p->unk24 = rnd2;
            p->unk28 = 4.0f + (8.0f * M528_RND());
            p->unk2C = 0.0f;
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            p->timer = 0;
            p->duration = (frandmod(3) * 3) + 1;
            p->state++;
        case 1:
            p->timer++;
            if (p->timer == p->duration) {
                Hu3DModelAttrReset(p->mdl, 1);
                p->state++;
            }
            break;
        case 2:
            p->vel.y -= 0.9f;
            if (p->vel.y < 0.0f) {
                p->state++;
            }
            p->pos.x += p->vel.x;
            p->pos.y += p->vel.y;
            p->pos.z += p->vel.z;
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            break;
        case 3:
            p->vel.y = -4.0f - ((30.0f * p->unk24) / 10.0f);
            p->state++;
        case 4:
            p->vel.x *= 0.9f;
            p->vel.z *= 0.9f;
            p->pos.x += p->vel.x;
            p->rot.y += p->unk28;
            p->pos.z = p->pos.z + (p->vel.x + HuSin(p->rot.y));
            p->pos.y = p->pos.y + (p->vel.y + HuCos(p->rot.y));
            if (p->pos.y < -800.0f) {
                done = 1;
            }
            Hu3DModelPosSetV(p->mdl, &p->pos);
            Hu3DModelRotSetV(p->mdl, &p->rot);
            break;
    }
    return done;
}

/* Rotate the path points by the wheel angle (around the y axis) and drop them
 * to the wheel plane. */
void fn_1_B384(M528MAIN *work)
{
    f32 s;
    f32 c;
    s32 i;

    s = HuSin(work->wheelAngle);
    c = HuCos(work->wheelAngle);
    for (i = 0; i < 0x37; i++) {
        lbl_1_bss_4[i].x = (c * lbl_1_data_31C[i].x) + (s * lbl_1_data_31C[i].z);
        lbl_1_bss_4[i].z = ((s * -lbl_1_data_31C[i].x) + (c * lbl_1_data_31C[i].z)) - 1680.0f;
        lbl_1_bss_4[i].y = lbl_1_data_31C[i].y - 600.0f;
    }
}

void fn_1_B528(M528WALKER *walker, s16 delay)
{
    M528MAIN *work;
    s16 mdl;
    s16 mtn;

    work = lbl_1_bss_298->data;
    M528_GETMDL(mdl, work->mdlCache[0x2E], 0x2E);
    M528_GETMTN(mtn, work->mtnCache[5], mdl, lbl_1_data_2A0[5]);
    Hu3DMotionSet(mdl, mtn);
    Hu3DModelAttrSet(mdl, 1);
    Hu3DModelAttrSet(mdl, 0x40000001);
    Hu3DModelAttrReset(mdl, 0x40000002);
    Hu3DModelLayerSet(mdl, 0);
    walker->mdl = mdl;
    walker->pos = lbl_1_bss_4[0];
    walker->rot.x = 0.0f;
    walker->rot.y = 0.0f;
    walker->rot.z = 0.0f;
    walker->state = 0;
    walker->timer = 0;
    walker->idx = 0;
    walker->delay = delay;
    walker->count = 0x37;
    Hu3DModelPosSetV(mdl, &walker->pos);
    Hu3DModelRotSetV(mdl, &walker->rot);
}

/* Heading (degrees) of the vector (dx, dz): atan2 with the x axis cases
 * handled by hand when dz is zero. */
#define M528_HEADING(out, dx, dz)                  \
    do {                                           \
        f32 _a;                                    \
        if ((dz) == 0.0f) {                        \
            if ((dx) >= 0.0f) {                    \
                _a = 1.5707964f;                   \
            } else {                               \
                _a = -1.5707964f;                  \
            }                                      \
        } else {                                   \
            _a = atan2((dx), (dz));                \
        }                                          \
        (out) = (f32)((180.0f * _a) / acos(-1.0)); \
    } while (0)

/* Walk the model along the lbl_1_bss_4 path: wait, then alternate between
 * turning on the spot and moving to the next point. */
void fn_1_B6D4(M528WALKER *w)
{
    f32 t;
    f32 head;
    f32 dx;
    f32 dz;
    HuVecF *p;

    switch (w->state) {
        case 1:
            w->timer = 0;
            w->state++;
        case 2:
            w->timer++;
            if (w->timer >= w->delay) {
                w->timer = 0;
                w->state++;
                return;
            }
            break;
        case 3:
            w->timer = 0;
            w->idx = 0;
            w->pos = lbl_1_bss_4[0];
            dz = lbl_1_bss_4[1].z - lbl_1_bss_4[0].z;
            dx = lbl_1_bss_4[1].x - lbl_1_bss_4[0].x;
            M528_HEADING(head, dx, dz);
            if (head < 0.0f) {
                head += 360.0f;
            }
            w->rot.y = head;
            w->angFrom = head;
            dz = lbl_1_bss_4[2].z - lbl_1_bss_4[1].z;
            dx = lbl_1_bss_4[2].x - lbl_1_bss_4[1].x;
            M528_HEADING(head, dx, dz);
            if (head < 0.0f) {
                head += 360.0f;
            }
            w->angTo = head;
            if (w->angTo < w->angFrom) {
                w->angTo += 360.0f;
            }
            Hu3DModelAttrReset(w->mdl, 1);
            w->state++;
        case 4:
            w->timer++;
            t = (f32)w->timer / 120.0f;
            p = &lbl_1_bss_4[w->idx];
            w->pos.x = (p[0].x * (1.0f - t)) + (p[1].x * t);
            w->pos.y = (p[0].y * (1.0f - t)) + (p[1].y * t);
            w->pos.z = (p[0].z * (1.0f - t)) + (p[1].z * t);
            Hu3DModelPosSetV(w->mdl, &w->pos);
            Hu3DModelRotSetV(w->mdl, &w->rot);
            if (w->timer == 0x78) {
                w->timer = 0;
                w->idx++;
                w->state++;
                return;
            }
            break;
        case 5:
            w->angFrom = w->rot.y;
            w->state++;
        case 6:
            w->timer++;
            t = (f32)w->timer / 60.0f;
            w->rot.y = (w->angFrom * (1.0f - t)) + (w->angTo * t);
            Hu3DModelPosSetV(w->mdl, &w->pos);
            Hu3DModelRotSetV(w->mdl, &w->rot);
            if (w->timer == 0x3C) {
                w->angFrom = w->rot.y;
                p = &lbl_1_bss_4[w->idx];
                dz = p[1].z - p[0].z;
                dx = p[1].x - p[0].x;
                M528_HEADING(head, dx, dz);
                while (head < w->angFrom) {
                    head += 360.0f;
                }
                w->angTo = head;
                w->timer = 0;
                w->state++;
                return;
            }
            break;
        case 7:
            w->angFrom = w->rot.y;
            p = &lbl_1_bss_4[w->idx];
            dz = p[1].z - p[0].z;
            dx = p[1].x - p[0].x;
            M528_HEADING(head, dx, dz);
            while (head < w->angFrom) {
                head += 360.0f;
            }
            if ((head - w->angFrom) > 180.0f) {
                head = w->angFrom;
            }
            w->angTo = head;
            w->state++;
        case 8:
            w->timer++;
            t = (f32)w->timer / 30.0f;
            p = &lbl_1_bss_4[w->idx];
            w->pos.x = (p[0].x * (1.0f - t)) + (p[1].x * t);
            w->pos.y = (p[0].y * (1.0f - t)) + (p[1].y * t);
            w->pos.z = (p[0].z * (1.0f - t)) + (p[1].z * t);
            w->rot.y = (w->angFrom * (1.0f - t)) + (w->angTo * t);
            Hu3DModelPosSetV(w->mdl, &w->pos);
            Hu3DModelRotSetV(w->mdl, &w->rot);
            if (w->timer == 0x1E) {
                w->idx++;
                w->timer = 0;
                if (w->idx == (s32)(w->count - 2)) {
                    w->state++;
                    return;
                }
                w->state--;
                return;
            }
            break;
        case 9:
            w->angFrom = w->rot.y;
            p = &lbl_1_bss_4[w->idx];
            dz = p[1].z - p[0].z;
            dx = p[1].x - p[0].x;
            M528_HEADING(head, dx, dz);
            while (head < w->angFrom) {
                head += 360.0f;
            }
            w->angTo = head;
            w->state++;
        case 10:
            w->timer++;
            t = (f32)w->timer / 60.0f;
            w->rot.y = (w->angFrom * (1.0f - t)) + (w->angTo * t);
            Hu3DModelPosSetV(w->mdl, &w->pos);
            Hu3DModelRotSetV(w->mdl, &w->rot);
            if (w->timer == 0x3C) {
                w->timer = 0;
                w->state++;
                return;
            }
            break;
        case 11:
            w->timer++;
            t = (f32)w->timer / 120.0f;
            p = &lbl_1_bss_4[w->idx];
            w->pos.x = (p[0].x * (1.0f - t)) + (p[1].x * t);
            w->pos.y = (p[0].y * (1.0f - t)) + (p[1].y * t);
            w->pos.z = (p[0].z * (1.0f - t)) + (p[1].z * t);
            Hu3DModelPosSetV(w->mdl, &w->pos);
            Hu3DModelRotSetV(w->mdl, &w->rot);
            if (w->timer == 0x78) {
                w->timer = 0;
                w->idx++;
                w->state++;
                return;
            }
            break;
        case 12:
            w->timer++;
            if (w->timer == 0x5A) {
                w->timer = 0;
                w->state = 3;
            }
            break;
    }
}

/* ---- effect emitters: one per wheel sector type (table in fn_1_B10) ---- */

void fn_1_C314(M528MAIN *work)
{
    M528NODE *node;
    s32 i;

    for (i = 0; i < 0x80; i++) {
        node = fn_1_4FF4(work, fn_1_6DF0, &work->particle[i]);
        if (node != 0) {
            fn_1_5194(&work->particle[i]);
        }
    }
}

void fn_1_C3DC(M528MAIN *work)
{
    M528NODE *node;
    s32 i;

    i = 0;
    node = fn_1_4FF4(work, fn_1_7794, &work->particle[i]);
    if (node != 0) {
        fn_1_5194(&work->particle[i]);
    }
    for (i = 1; i < 0x80; i++) {
        node = fn_1_4FF4(work, fn_1_72E4, &work->particle[i]);
        if (node != 0) {
            fn_1_5194(&work->particle[i]);
        }
    }
}

void fn_1_C530(M528MAIN *work)
{
    M528NODE *node;
    s32 i;

    for (i = 0; i < 0xA; i++) {
        node = fn_1_4FF4(work, fn_1_9D5C, &work->particle[i]);
        if (node != 0) {
            fn_1_5194(&work->particle[i]);
        }
    }
    for (; i < 0x5A; i++) {
        node = fn_1_4FF4(work, fn_1_A3A0, &work->particle[i]);
        if (node != 0) {
            fn_1_5194(&work->particle[i]);
        }
    }
    for (; i < 0x5F; i++) {
        node = fn_1_4FF4(work, fn_1_ABAC, &work->particle[i]);
        if (node != 0) {
            fn_1_5194(&work->particle[i]);
        }
    }
}

void fn_1_C728(M528MAIN *work)
{
    s16 order[0x18];
    M528NODE *node;
    s16 a;
    s16 b;
    s16 k;
    s32 i;

    for (k = 0; k < 0x18; k++) {
        order[k] = k;
    }
    for (i = 0; i < 0xC; i++) {
        a = frandmod(0x18);
        b = frandmod(0x18);
        order[a] = order[b];
        order[b] = order[a];
    }
    for (i = 0; i < 0x18; i++) {
        node = fn_1_4FF4(work, fn_1_7B44, &work->particle[i]);
        if (node != 0) {
            fn_1_5194(&work->particle[i]);
            work->particle[i].timer = order[i];
            work->particle[i].duration = (i * 0xA) + 0xA;
        }
    }
}

void fn_1_C8B4(M528MAIN *work)
{
    s16 order[0x18];
    M528NODE *node;
    s16 a;
    s16 b;
    s16 k;
    s32 i;

    for (k = 0; k < 0x18; k++) {
        order[k] = k;
    }
    for (i = 0; i < 0xC; i++) {
        a = frandmod(0x18);
        b = frandmod(0x18);
        order[a] = order[b];
        order[b] = order[a];
    }
    for (i = 0; i < 0x18; i++) {
        node = fn_1_4FF4(work, fn_1_97C8, &work->particle[i]);
        if (node != 0) {
            fn_1_5194(&work->particle[i]);
            work->particle[i].timer = (order[i] * 4) - 0x2E;
        }
    }
}

void fn_1_CA34(M528MAIN *work)
{
    M528NODE *node;
    s32 i;

    i = 0;
    node = fn_1_4FF4(work, fn_1_800C, &work->particle[i]);
    if (node != 0) {
        fn_1_5194(&work->particle[i]);
    }
}

void fn_1_CAEC(M528MAIN *work)
{
    M528NODE *node;
    s32 i;

    i = 0;
    node = fn_1_4FF4(work, fn_1_83E4, &work->particle[i]);
    if (node != 0) {
        fn_1_5194(&work->particle[i]);
    }
    for (i = 1; i < 0x23; i++) {
        node = fn_1_4FF4(work, fn_1_87F4, &work->particle[i]);
        if (node != 0) {
            fn_1_5194(&work->particle[i]);
        }
    }
}

void fn_1_CC40(M528MAIN *work)
{
    M528NODE *node;
    s32 i;

    i = 0;
    node = fn_1_4FF4(work, fn_1_8CD0, &work->particle[i]);
    if (node != 0) {
        fn_1_5194(&work->particle[i]);
    }
    for (i = 1; i < 0x80; i++) {
        node = fn_1_4FF4(work, fn_1_8FFC, &work->particle[i]);
        if (node != 0) {
            fn_1_5194(&work->particle[i]);
        }
    }
}
