// m515Dll / m515
//
#include "types.h"
#include "game/hu3d.h"
#include "game/esprite.h"
#include "game/object.h"
#include "game/memory.h"
#include "game/audio.h"
#include "game/charman.h"
#include "game/mg/seqman.h"
#include "game/pad.h"
#include "game/data.h"
#include "humath.h"
#include "dolphin/mtx.h"

typedef struct M515Node2_s M515_NODE2;
typedef s32 (*M515_NODEFN)(M515_NODE2 *node);
typedef struct M515Slot_s M515_SLOT;
typedef struct M515Cell_s M515_CELL;
typedef struct M515Node2_s M515_NODE2;
typedef struct M515Sub_s M515_SUB;
typedef struct M515Ply_s M515_PLY;

// Global per-player board slot, stride 0x30, at sub+0x234.
struct M515Slot_s {
    f32 f00;
    f32 f04;
    f32 f08;
    HuVecF look;
    HuVecF vec;
    s16 state;
    s16 timer;
    s16 idx;
    char pad_2a[2];
    s32 f2c;
};

// Board item attached to a cell.
typedef struct M515Item_s {
    s16 mdl;
    s16 mot;
    s16 mdl2;
    char pad_06[2];
    HuVecF a;
    HuVecF b;
    s16 f20;
    s16 f22;
    s16 f24;
    char pad_26[2];
    f32 f28;
} M515_ITEM;

typedef struct M515Place_s {
    s16 id;
    s16 pos;
    f32 rot;
} M515_PLACE;

// Board cell, stride 0x28. nbr: right, left, down, up.
struct M515Cell_s {
    M515_CELL *nbr[4];
    char pad_10[4];
    M515_ITEM *item;
    HuVecF vec;
    u16 type;
    char pad_26[2];
};

typedef struct M515Entry_s {
    s16 e00;
    s16 e02;
    M515_SLOT *slot;
    s32 e08;
    s32 e0c;
} M515_ENTRY;

typedef struct M515EspPair_s {
    s16 id0;
    s16 id1;
    f32 f4;
    f32 f8;
    f32 fc;
} M515_ESPPAIR;

typedef struct M515Id4_s {
    s16 a;
    s16 b;
    s16 c;
    s16 d;
} M515_ID4;

// Per-player group of the global state (stride 0x28).
typedef struct M515Grp_s {
    s32 f00;
    char pad_04[8];
    s16 f0c;
    char pad_0e[0x28 - 0x0e];
} M515_GRP;

// Intrusive doubly-linked node (stride 0x14) behind the head/cur lists of M515_SUB.
struct M515Node2_s {
    s16 f00;
    s16 f02;
    s16 f04;
    char pad_06[2];
    M515_NODEFN func;
    M515_NODE2 *prev;
    M515_NODE2 *next;
};

// Per-player continuation hook, stored at ply+0x688.
typedef s32 (*M515_HOOKFN)(M515_ENTRY *entry, M515_PLY *self);
s32 fn_1_B0B8(M515_ENTRY *entry, M515_PLY *self);
s32 fn_1_9DF8(M515_ENTRY *entry, M515_PLY *self);
s32 fn_1_D928(M515_ENTRY *entry, M515_PLY *self);
s32 fn_1_B5D8(M515_ENTRY *entry, M515_PLY *self);

// Global state block (0xcb0 bytes) hung off the first object's data.
struct M515Sub_s {
    s16 v00;
    s16 v02[5];
    s16 modelId;
    char pad_00E[0x16 - 0xE];
    s16 id16[4];
    s16 id1e[4];
    s16 id26[4];
    char pad_02E[0x76 - 0x2E];
    M515_ID4 id76[4];
    s16 id96[4][2];
    s16 ida6[4];
    char pad_0AE[0xB6 - 0xAE];
    s16 idb6[4][3];
    char pad_0CE[0xDC - 0xCE];
    s16 mdl;
    s16 mdl2;
    s16 mot[8];
    char pad_0F0[0x10C - 0xF0];
    s16 cache[18];
    M515_GRP grp[4];
    s16 v1d0;
    s16 v1d2;
    s16 v1d4;
    s16 v1d6;
    s16 v1d8;
    s16 v1da;
    s16 v1dc[4][3];
    s32 v1f4;
    char pad_1F8[0x224 - 0x1F8];
    M515_PLY *ply[4];
    M515_SLOT slot[4];
    M515_ENTRY entry[4];
    M515_ESPPAIR esp;
    M515_NODE2 *head;
    M515_NODE2 *cur;
    M515_NODE2 node[0x78];
    s16 v0cac;
    char pad_CAE[0xCB0 - 0xCAE];
};

typedef struct M515Fx_s {
    char pad_00[0x10];
} M515_FX;

// Per-player block (0x6a0 bytes).
struct M515Ply_s {
    s16 idx;
    s16 chr;
    s16 pad;
    char pad_006[4];
    s16 type;
    s16 dif;
    s16 v0e;
    M515_CELL cell[5][5];
    M515_CELL *p3f8;
    M515_CELL ext[2];
    HuVecF pos;
    HuVecF rot;
    HuVecF from;
    HuVecF vec;
    s16 v47c;
    s16 v47e;
    s16 v480;
    s16 v482;
    s16 v484;
    s16 v486;
    M515_ITEM item[8];
    s16 v5e8;
    s16 mdlIds[3][25];
        u16 v680;
    s16 v682;
    M515_ENTRY *p684;
    M515_HOOKFN hook;
    OMOBJ *obj;
    M515_FX fx;
};

float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);
s32 rand8(void);

typedef void (*M515_VOIDFN)(void);
extern const M515_VOIDFN _ctors[];
extern const M515_VOIDFN _dtors[];
void ObjectSetup(void);

// .bss:0x0 / 0x4 / 0x8. bss_4 is the one live object pointer.
s32 lbl_1_bss_0;
OMOBJ *lbl_1_bss_4;
OMOBJMAN *lbl_1_bss_8;

#define M515_SUBOF(o) ((M515_SUB *)(o)->data)

// .rodata:0x10, camera bit per player.
#ifdef __MWERKS__
#define M515_RODATA __declspec(section ".rodata")
#else
#define M515_RODATA
#endif
M515_RODATA const u16 lbl_1_rodata_10[4] = { 1, 2, 4, 8 };

void fn_1_32C4(M515_SUB *sub);
void fn_1_CE98(M515_SUB *sub);
void fn_1_9920(M515_ESPPAIR *esp);
void fn_1_9828(M515_ESPPAIR *esp);
void fn_1_9958(M515_ESPPAIR *esp);
void fn_1_CC20(s16 layerNo);
void fn_1_7E5C(M515_SLOT *slot, s16 idx);
void fn_1_7EE0(M515_SLOT *slot);
void fn_1_C958(s32 idx, HuVecF *pos, s32 arg);
void fn_1_1FC(s16 mode, s16 frameNo);
void fn_1_26C(s16 mode, s16 frameNo);
void fn_1_928(s16 mode, s16 frameNo);
void fn_1_968(s16 mode, s16 frameNo);
void fn_1_B08(s16 mode, s16 frameNo);
void fn_1_D30(s16 mode, s16 frameNo);
void fn_1_1B58(s16 mode, s16 frameNo);
void fn_1_1D20(s16 mode, s16 frameNo);
void fn_1_1D24(s16 mode, s16 frameNo);
void fn_1_1D60(OMOBJ *obj);
void fn_1_29D0(OMOBJ *obj);
void fn_1_2AC0(OMOBJ *obj);
void fn_1_2B6C(M515_SUB *sub);
void fn_1_2EB4(M515_SUB *sub);
void fn_1_2F3C(M515_SUB *sub);
void fn_1_5C40(M515_SUB *sub);
void fn_1_5DB0(M515_SUB *sub);
void fn_1_C83C(void);
void fn_1_D0D8(M515_SUB *sub);
s32 fn_1_83E8(M515_SLOT *slot);
void fn_1_BE78(M515_SUB *sub, s16 mdl, s16 mode);
void fn_1_BF24(M515_SUB *sub, s16 mdl, s16 mode);
extern f32 lbl_1_data_28[60];
s32 fn_1_BFD0(M515_NODE2 *node);
s32 fn_1_C0D0(M515_NODE2 *node);
s32 fn_1_C150(M515_NODE2 *node);
s32 fn_1_C26C(M515_NODE2 *node);
M515_NODE2 *fn_1_5CE4(M515_SUB *sub);
void fn_1_73B0(M515_PLY *ply, s32 row);
void fn_1_7768(M515_PLY *ply, s32 row);
void fn_1_77F0(M515_PLY *ply, s32 row);
void fn_1_69B8(M515_PLY *ply, M515_SUB *sub);
void fn_1_6FE8(M515_PLY *ply);
void fn_1_E274(M515_FX *fx, s16 a, s16 b);
void fn_1_E548(M515_PLY *ply);
void fn_1_2684(OMOBJ *obj);
s32 fn_1_6324(M515_PLY *ply, M515_SUB *sub);
s16 fn_1_66D0(M515_PLY *ply);
u16 fn_1_7924(M515_PLY *ply);
u16 fn_1_DD18(M515_PLY *ply);
void fn_1_7BE8(M515_ITEM *item, M515_SUB *sub);
void fn_1_81A8(M515_SLOT *slot, M515_PLY *ply);
s32 fn_1_7878(M515_PLY *ply);
void fn_1_6980(M515_PLY *ply);
void fn_1_6318(M515_PLY *ply);
s32 fn_1_611C(M515_PLY *ply);
s32 fn_1_5FD0(M515_PLY *ply);
void fn_1_9804(M515_ENTRY *entry, M515_SLOT *slot, s32 index);

// DATA BEGIN
f32 lbl_1_data_28[60] = {
    270.0f, 10.0f, 4240.0f, 0.0f, 108.0f, 204.0f, 420.0f, 40.0f, 6000.0f, 25.0f, 25.0f, 75.0f,
    420.0f, 40.0f, 6000.0f, 25.0f, 25.0f, -900.0f, 240.0f, 40.0f, 6000.0f, -25.0f, 425.0f,
    -675.0f, 240.0f, 40.0f, 6000.0f, -25.0f, 425.0f, -75.0f, 240.0f, 40.0f, 6000.0f, -25.0f,
    425.0f, 900.0f, 60.0f, 40.0f, 6000.0f, 25.0f, 825.0f, 675.0f, 60.0f, 40.0f, 6000.0f, 25.0f,
    825.0f, 75.0f, 90.0f, 15.0f, 1900.0f, 0.0f, 1000.0f, -225.0f, 90.0f, 4.0f, 1970.0f, 0.0f,
    150.0f, -225.0f
};

s32 lbl_1_data_118[13][8] = {
    { 0x8C0000, 0x8C0001, 0x8C0002, 0x8C0005, 0x8C0006, 0x8C0007, 0x8C000D, 0x8C000E },
    { 0x330000, 0x330001, 0x330002, 0x330005, 0x330006, 0x330007, 0x33000D, 0x33000E },
    { 0xB70000, 0xB70001, 0xB70002, 0xB70005, 0xB70006, 0xB70007, 0xB7000D, 0xB7000E },
    { 0xED0000, 0xED0001, 0xED0002, 0xED0005, 0xED0006, 0xED0007, 0xED000D, 0xED000E },
    { 0xE60000, 0xE60001, 0xE60002, 0xE60005, 0xE60006, 0xE60031, 0xE6000D, 0xE6000E },
    { 0x170000, 0x170001, 0x170002, 0x170005, 0x170006, 0x170007, 0x17000D, 0x17000E },
    { 0xE00000, 0xE00001, 0xE00002, 0xE00005, 0xE00006, 0xE00007, 0xE0000D, 0xE0000E },
    { 0x2D0000, 0x2D0001, 0x2D0002, 0x2D0005, 0x2D0006, 0x2D0007, 0x2D000D, 0x2D000E },
    { 0xC90000, 0xC90001, 0xC90002, 0xC90005, 0xC90006, 0xC90007, 0xC9000D, 0xC9000E },
    { 0xB00000, 0xB00001, 0xB00002, 0xB00005, 0xB00006, 0xB00007, 0xB0000D, 0xB0000E },
    { 0xB00000, 0xB00001, 0xB00002, 0xB00005, 0xB00006, 0xB00007, 0xB0000D, 0xB0000E },
    { 0xB00000, 0xB00001, 0xB00002, 0xB00005, 0xB00006, 0xB00007, 0xB0000D, 0xB0000E },
    { 0xB00000, 0xB00001, 0xB00002, 0xB00005, 0xB00006, 0xB00007, 0xB0000D, 0xB0000E }
};

s32 lbl_1_data_320[17] = {
    0x490028, 0x490029, 0x49002A, 0x49002B, 0x49002C, 0x49002D, 0x49002E, 0x49002F, 0x490030,
    0x490031, 0x490032, 0x490033, 0x490034, 0x490035, 0x490036, 0x490037, 0x490038
};

s32 lbl_1_data_364[17] = {
    0x490039, 0x49003A, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

s32 lbl_1_data_3A8[8] = {
    0x49003B, 0x49003C, 0x49003D, 0x49003E, 0x49003F, 0x490040, 0x490041, 0x490042
};

u16 lbl_1_data_530[25] = {
    1, 1, 0, 0, 1, 1, 1, 0, 2, 0, 1, 1, 1, 0, 0, 1, 1, 0, 2, 0, 1, 1, 0, 1, 0
};

u16 lbl_1_data_562[25] = {
    1, 1, 0, 0, 0, 1, 1, 0, 2, 0, 1, 1, 1, 0, 2, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0
};

u16 lbl_1_data_594[25] = {
    1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 2, 2, 0, 1, 1, 0, 1, 1
};

u16 lbl_1_data_5C6[25] = {
    0, 0, 0, 1, 1, 2, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 2, 0, 1, 1
};

u16 lbl_1_data_5F8[25] = {
    1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 2, 1, 1, 1, 1, 0, 0, 1, 1
};

u16 lbl_1_data_62A[25] = {
    1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0
};

u16 lbl_1_data_65C[25] = {
    1, 1, 0, 0, 1, 1, 1, 0, 2, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 2, 0
};

u16 lbl_1_data_68E[25] = {
    0, 0, 0, 1, 1, 2, 2, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1
};

u16 lbl_1_data_6C0[25] = {
    1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 2, 2, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1
};

u16 lbl_1_data_6F2[25] = {
    1, 1, 0, 0, 0, 1, 1, 1, 0, 2, 1, 1, 0, 1, 0, 1, 1, 2, 0, 0, 1, 1, 0, 1, 1
};

u16 lbl_1_data_724[25] = {
    0, 0, 0, 2, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 1, 0, 0, 0, 0, 0
};

u16 lbl_1_data_756[25] = {
    0, 2, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 2, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0
};

u16 lbl_1_data_788[25] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 2, 1, 1, 2, 1, 0, 1, 1, 0, 0, 0
};

u16 lbl_1_data_7BA[25] = {
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 2, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0
};

u16 lbl_1_data_7EC[25] = {
    0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 2, 1, 2, 1, 1, 0, 0, 0, 0
};

u16 lbl_1_data_81E[25] = {
    1, 0, 0, 0, 1, 1, 0, 2, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1
};

u16 lbl_1_data_850[25] = {
    1, 0, 0, 1, 1, 1, 2, 2, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1
};

u16 lbl_1_data_882[25] = {
    1, 0, 2, 0, 0, 1, 1, 0, 2, 2, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1
};

u16 lbl_1_data_8B4[25] = {
    1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 2, 0, 2, 0, 1, 0, 2, 0, 1
};

u16 lbl_1_data_8E6[25] = {
    1, 0, 2, 0, 1, 0, 2, 0, 1, 1, 1, 0, 2, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1
};

u16 lbl_1_data_918[25] = {
    0, 2, 0, 2, 0, 1, 0, 2, 0, 1, 0, 2, 0, 2, 0, 1, 0, 2, 0, 1, 0, 2, 0, 2, 0
};

u16 lbl_1_data_94A[25] = {
    1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0
};

u16 lbl_1_data_97C[25] = {
    0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 1, 0, 0, 0, 1, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0
};

u16 lbl_1_data_9AE[25] = {
    0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0
};

u16 lbl_1_data_9E0[25] = {
    1, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 2, 0, 2, 1, 0, 2, 2, 2, 0, 0, 1, 0, 0, 0
};

u16 lbl_1_data_A12[25] = {
    0, 0, 2, 0, 0, 0, 1, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1, 0, 0, 0, 2, 0, 0
};

u16 lbl_1_data_A44[25] = {
    1, 0, 2, 0, 0, 0, 1, 0, 2, 0, 2, 0, 2, 0, 0, 0, 1, 1, 1, 2, 0, 0, 0, 0, 0
};

u16 lbl_1_data_A76[25] = {
    1, 0, 2, 0, 1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 2, 2, 2, 0, 1, 0, 0, 0, 1
};

u16 lbl_1_data_AA8[25] = {
    1, 0, 2, 0, 1, 1, 2, 0, 2, 0, 0, 2, 0, 0, 0, 0, 1, 1, 1, 2, 0, 0, 0, 0, 0
};

u16 lbl_1_data_ADA[25] = {
    0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 1, 0, 1, 1, 0, 1, 2, 1, 1, 0, 0, 0, 1
};

M515_PLACE lbl_1_data_B0C[5] = {
    { 16, 0, 0.0f },
    { 6, 12, 0.0f },
    { 1, 23, 0.0f },
    { 4, 4, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_B34[4] = {
    { 16, 0, 0.0f },
    { 6, 12, 180.0f },
    { 4, 18, 180.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_B54[7] = {
    { 16, 0, 0.0f },
    { 8, 4, 0.0f },
    { 7, 7, 0.0f },
    { 3, 9, 270.0f },
    { 6, 23, 0.0f },
    { 1, 24, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_B8C[6] = {
    { 6, 7, 180.0f },
    { 4, 11, 0.0f },
    { 7, 12, 0.0f },
    { 6, 17, 0.0f },
    { 16, 24, 180.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_BBC[8] = {
    { 8, 0, 0.0f },
    { 4, 1, 0.0f },
    { 3, 10, 90.0f },
    { 6, 15, 0.0f },
    { 7, 17, 0.0f },
    { 1, 20, 0.0f },
    { 16, 24, 180.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_BFC[7] = {
    { 16, 0, 0.0f },
    { 8, 3, 0.0f },
    { 2, 4, 0.0f },
    { 6, 8, 0.0f },
    { 7, 17, 0.0f },
    { 4, 18, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_C34[6] = {
    { 16, 0, 0.0f },
    { 2, 4, 0.0f },
    { 7, 12, 0.0f },
    { 4, 13, 0.0f },
    { 6, 17, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_C64[6] = {
    { 4, 11, 0.0f },
    { 6, 12, 180.0f },
    { 7, 17, 0.0f },
    { 1, 20, 0.0f },
    { 16, 24, 180.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_C94[6] = {
    { 8, 0, 0.0f },
    { 2, 1, 0.0f },
    { 6, 16, 0.0f },
    { 7, 17, 0.0f },
    { 16, 24, 180.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_CC4[6] = {
    { 16, 0, 0.0f },
    { 7, 7, 0.0f },
    { 4, 13, 0.0f },
    { 6, 23, 0.0f },
    { 1, 24, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_CF4[4] = {
    { 11, 6, 0.0f },
    { 12, 16, 0.0f },
    { 10, 19, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_D14[6] = {
    { 11, 6, 0.0f },
    { 12, 8, 270.0f },
    { 12, 16, 90.0f },
    { 9, 12, 0.0f },
    { 11, 18, 180.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_D44[6] = {
    { 10, 10, 0.0f },
    { 11, 11, 270.0f },
    { 12, 13, 270.0f },
    { 12, 20, 90.0f },
    { 9, 21, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_D74[5] = {
    { 2, 4, 0.0f },
    { 10, 10, 0.0f },
    { 11, 12, 270.0f },
    { 9, 13, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_D9C[5] = {
    { 10, 7, 0.0f },
    { 12, 12, 270.0f },
    { 9, 19, 0.0f },
    { 9, 20, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_DC4[9] = {
    { 12, 0, 270.0f },
    { 11, 4, 270.0f },
    { 11, 10, 0.0f },
    { 12, 13, 270.0f },
    { 10, 14, 0.0f },
    { 11, 19, 270.0f },
    { 14, 21, 90.0f },
    { 9, 23, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_E0C[9] = {
    { 11, 0, 270.0f },
    { 12, 3, 0.0f },
    { 11, 9, 270.0f },
    { 2, 10, 90.0f },
    { 9, 15, 0.0f },
    { 12, 16, 270.0f },
    { 15, 18, 270.0f },
    { 10, 20, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_E54[9] = {
    { 9, 0, 0.0f },
    { 9, 5, 0.0f },
    { 11, 6, 270.0f },
    { 10, 10, 0.0f },
    { 12, 18, 270.0f },
    { 9, 19, 0.0f },
    { 14, 21, 90.0f },
    { 10, 24, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_E9C[8] = {
    { 12, 3, 270.0f },
    { 10, 4, 0.0f },
    { 14, 6, 90.0f },
    { 11, 9, 270.0f },
    { 10, 10, 0.0f },
    { 9, 20, 0.0f },
    { 10, 24, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_EDC[8] = {
    { 10, 0, 0.0f },
    { 10, 4, 0.0f },
    { 12, 9, 180.0f },
    { 11, 10, 270.0f },
    { 9, 14, 0.0f },
    { 15, 18, 270.0f },
    { 12, 21, 180.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_F1C[5] = {
    { 13, 5, 90.0f },
    { 4, 9, 0.0f },
    { 4, 15, 0.0f },
    { 17, 19, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_F44[7] = {
    { 17, 0, 0.0f },
    { 13, 1, 0.0f },
    { 13, 3, 0.0f },
    { 17, 4, 0.0f },
    { 4, 16, 0.0f },
    { 4, 18, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_F7C[3] = {
    { 13, 10, 90.0f },
    { 17, 14, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_F94[3] = {
    { 13, 5, 90.0f },
    { 17, 9, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_FAC[5] = {
    { 17, 0, 0.0f },
    { 17, 14, 0.0f },
    { 13, 21, 90.0f },
    { 13, 3, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_FD4[3] = {
    { 13, 6, 90.0f },
    { 4, 18, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_FEC[5] = {
    { 13, 0, 90.0f },
    { 17, 6, 0.0f },
    { 4, 16, 0.0f },
    { 5, 17, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_1014[5] = {
    { 17, 0, 0.0f },
    { 13, 4, 90.0f },
    { 13, 20, 90.0f },
    { 17, 24, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_103C[5] = {
    { 13, 4, 0.0f },
    { 3, 5, 90.0f },
    { 4, 16, 0.0f },
    { 5, 17, 0.0f },
    { -1, 0, 0.0f }
};

M515_PLACE lbl_1_data_1064[6] = {
    { 13, 12, 0.0f },
    { 17, 14, 0.0f },
    { 4, 17, 0.0f },
    { 5, 19, 270.0f },
    { 3, 20, 90.0f },
    { -1, 0, 0.0f }
};

u16 * lbl_1_data_1094[10] = {
    lbl_1_data_530, lbl_1_data_562, lbl_1_data_594, lbl_1_data_5C6, lbl_1_data_5F8,
    lbl_1_data_62A, lbl_1_data_65C, lbl_1_data_68E, lbl_1_data_6C0, lbl_1_data_6F2
};

u16 * lbl_1_data_10BC[10] = {
    lbl_1_data_724, lbl_1_data_756, lbl_1_data_788, lbl_1_data_7BA, lbl_1_data_7EC,
    lbl_1_data_81E, lbl_1_data_850, lbl_1_data_882, lbl_1_data_8B4, lbl_1_data_8E6
};

u16 * lbl_1_data_10E4[10] = {
    lbl_1_data_918, lbl_1_data_94A, lbl_1_data_97C, lbl_1_data_9AE, lbl_1_data_9E0,
    lbl_1_data_A12, lbl_1_data_A44, lbl_1_data_A76, lbl_1_data_AA8, lbl_1_data_ADA
};

M515_PLACE * lbl_1_data_110C[10] = {
    lbl_1_data_B0C, lbl_1_data_B34, lbl_1_data_B54, lbl_1_data_B8C, lbl_1_data_BBC,
    lbl_1_data_BFC, lbl_1_data_C34, lbl_1_data_C64, lbl_1_data_C94, lbl_1_data_CC4
};

M515_PLACE * lbl_1_data_1134[10] = {
    lbl_1_data_CF4, lbl_1_data_D14, lbl_1_data_D44, lbl_1_data_D74, lbl_1_data_D9C,
    lbl_1_data_DC4, lbl_1_data_E0C, lbl_1_data_E54, lbl_1_data_E9C, lbl_1_data_EDC
};

M515_PLACE * lbl_1_data_115C[10] = {
    lbl_1_data_F1C, lbl_1_data_F44, lbl_1_data_F7C, lbl_1_data_F94, lbl_1_data_FAC,
    lbl_1_data_FD4, lbl_1_data_FEC, lbl_1_data_1014, lbl_1_data_103C, lbl_1_data_1064
};
// DATA END

MGSEQ_PARAM lbl_1_data_0 = { 300, 0, fn_1_1FC, fn_1_26C, fn_1_928, fn_1_968, fn_1_B08, fn_1_D30,
    fn_1_1B58, fn_1_1D20, fn_1_1D24 };


s32 _prolog(void)
{
    const M515_VOIDFN *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const M515_VOIDFN *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    OMOBJ *obj;
    M515_SUB *sub;
    s32 i;

    lbl_1_bss_8 = omInitObjMan(500, 0x2000);
    omGameSysInit(lbl_1_bss_8);
    obj = omAddObjEx(lbl_1_bss_8, 0x65, 0, 0, -1, fn_1_29D0);
    lbl_1_bss_4 = obj;
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M515_SUB), HU_MEMNUM_OVL);
    sub = obj->data;
    fn_1_2B6C(sub);
    fn_1_2EB4(sub);
    fn_1_C83C();
    for (i = 0; i < 4; i++) {
        obj = omAddObjEx(lbl_1_bss_8, 0x64, 1, 8, 0, fn_1_1D60);
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M515_PLY), HU_MEMNUM_OVL);
        obj->work[0] = i;
        sub->ply[i] = obj->data;
    }
    fn_1_2F3C(sub);
    Hu3DBGColorSet(0, 0, 0);
    Hu3DNoSyncSet(1);
    MgSeqCreate(&lbl_1_data_0);
    sub->v0cac = 0;
}

void fn_1_1FC(s16 mode, s16 frameNo)
{
    M515_SUB *sub = M515_SUBOF(lbl_1_bss_4);

    sub->v1d0 = 0;
    sub->v1da = 0;
    sub->v1d2 = 0;
    sub->v1d6 = -1;
    sub->v1f4 = 0;
    fn_1_32C4(sub);
    fn_1_CE98(sub);
    MgSeqModeNext();
}

void fn_1_26C(s16 mode, s16 frameNo)
{
    M515_SUB *sub;
    M515_PLY *ply;
    s32 i;
    f32 t;
    f32 u;
    HuVecF p[3] = { { -150.0f, 25.0f, 1000.0f }, { 0.0f, 25.0f, 1000.0f }, { 0.0f, 25.0f, 450.0f } };
    f32 unused[4] = { 160.0f, 180.0f, 195.0f, 200.0f };

    sub = M515_SUBOF(lbl_1_bss_4);
    ply = sub->ply[0];
    switch (sub->v1da) {
    case 0:
        HuAudFXPlay(0x6E8);
        MgSeqModeChangeOff();
        sub->v1d2 = 0;
        sub->v1da++;
    case 1:
        fn_1_D0D8(sub);
        sub->v1d2++;
        t = (f32)sub->v1d2 / 40.0f;
        ply->pos.x = p[0].x * (1.0f - t) + p[1].x * t;
        omSetTra(ply->obj, ply->pos.x, ply->pos.y, ply->pos.z);
        if (sub->v1d2 >= 0x28) {
            Hu3DMotionShiftSet(ply->obj->mdlId[0], ply->obj->mtnId[0], 0.0f, 5.0f, 0);
            sub->v1d2 = 0;
            sub->v1da++;
            return;
        }
        break;
    case 2:
        fn_1_D0D8(sub);
        sub->v1d2++;
        if (sub->v1d2 >= 0x1E) {
            Hu3DMotionShiftSet(ply->obj->mdlId[0], ply->obj->mtnId[6], 60.0f, 5.0f, 0);
            sub->v1d2 = 0;
            sub->v1da++;
            return;
        }
        break;
    case 3:
        fn_1_D0D8(sub);
        sub->v1d2++;
        if (sub->v1d2 == 0x37) {
            Hu3DMotionShiftSet(ply->obj->mdlId[0], ply->obj->mtnId[0], 0.0f, 5.0f, 0x40000001);
        }
        t = (f32)sub->v1d2 / 60.0f;
        ply->rot.y = 90.0f * (1.0f - t) + 180.0f * t;
        omSetRot(ply->obj, ply->rot.x, ply->rot.y, ply->rot.z);
        if (sub->v1d2 >= 0x3C) {
            HuAudFXPlay(0x6DF);
            Hu3DModelAttrReset(sub->ida6[0], 0x40000002);
            sub->v1d2 = 0;
            sub->v1da++;
            sub->grp[0].f00++;
            sub->grp[1].f00++;
            sub->grp[2].f00++;
            sub->grp[3].f00++;
            return;
        }
        break;
    case 4:
        fn_1_D0D8(sub);
        sub->v1d2++;
        if (sub->v1d2 >= 0x50) {
            Hu3DMotionShiftSet(ply->obj->mdlId[0], ply->obj->mtnId[1], 0.0f, 5.0f, 0x40000001);
            sub->v1d2 = 0;
            sub->v1da++;
            return;
        }
        break;
    case 5:
        fn_1_D0D8(sub);
        sub->v1d2++;
        t = (f32)sub->v1d2 / 120.0f;
        if (t > 1.0f) {
            t = 1.0f;
            if (ply->obj->mtnId[0] != Hu3DMotionIDGet(ply->obj->mdlId[0])
                && ply->obj->mtnId[0] != Hu3DMotionShiftIDGet(ply->obj->mdlId[0])) {
                Hu3DMotionShiftSet(ply->obj->mdlId[0], ply->obj->mtnId[0], 0.0f, 5.0f, 0x40000001);
            }
        }
        ply->pos.z = p[1].z * (1.0f - t) + p[2].z * t;
        omSetTra(ply->obj, ply->pos.x, ply->pos.y, ply->pos.z);
        if (sub->v1d2 == 0x3C) {
            Hu3DModelAttrSet(sub->ida6[0], 0x40000004);
        }
        if (sub->v1d2 == 0x2D) {
            Hu3DModelAttrReset(sub->id1e[0], HU3D_ATTR_DISPOFF);
        }
        if (sub->v1d2 > 0x14) {
            if (fn_1_83E8(&sub->slot[0]) != 0) {
                for (i = 0; i < 4; i++) {
                    Hu3DModelAttrSet(sub->grp[i].f0c, HU3D_ATTR_DISPOFF);
                }
                sub->v1da++;
            }
            u = (f32)sub->slot[0].timer / 120.0f;
            sub->esp.f4 = 640.0f * (1.0f - u) + 320.0f * u;
            sub->esp.f8 = 480.0f * (1.0f - u) + 240.0f * u;
            fn_1_9958(&sub->esp);
            return;
        }
        break;
    case 6:
        lbl_1_bss_0 = HuAudBGMPlay(0x37);
        MgSeqModeNext();
        break;
    }
}

void fn_1_928(s16 mode, s16 frameNo)
{
    M515_SUB *sub = M515_SUBOF(lbl_1_bss_4);

    Hu3DModelAttrSet(sub->modelId, HU3D_ATTR_DISPOFF);
}

void fn_1_968(s16 mode, s16 frameNo)
{
    M515_SUB *sub;
    M515_PLY *ply;
    s32 i;
    s32 n;
    s32 list[4];

    sub = M515_SUBOF(lbl_1_bss_4);
    n = 0;
    for (i = 0; i < 4; i++) {
        ply = sub->ply[i];
        list[n] = -1;
        if (ply->v482 == 5) {
            list[n] = i;
            n++;
        }
    }
    if (n != 0) {
        sub->v0cac = 1;
        if (n == 1) {
            sub->v1d6 = list[0];
        } else {
            sub->v1d6 = list[rand8() / (0xFF / n + 1)];
        }
        MgSeqModeNext();
        for (i = 0; i < 4; i++) {
            ply = sub->ply[i];
            if (ply->hook != fn_1_9DF8) {
                if (i == sub->v1d6) {
                    ply->hook = fn_1_B5D8;
                } else {
                    ply->hook = fn_1_B0B8;
                    if (ply->v482 == 5) {
                        ply->vec = ply->from;
                        ply->v484 = 0;
                    }
                }
                ply->p684->e00 = 0;
            }
            ply->v680 = 0;
            ply->v682 = 0;
        }
        return;
    }
    sub->v1d4 = 1;
}

void fn_1_B08(s16 mode, s16 frameNo)
{
    M515_SUB *sub;
    M515_PLY *ply;
    s32 i;

    sub = M515_SUBOF(lbl_1_bss_4);
    sub->v1d4 = 0;
    switch (sub->v1d0) {
    case 1:
        break;
    case 0:
        HuAudSStreamFadeOut(lbl_1_bss_0, 1000);
        sub->v0cac = 1;
        if (sub->v1d6 >= 0) {
            ply = sub->ply[sub->v1d6];
            MgSeqWinnerSet(ply->chr, -1, -1, -1);
        } else {
            for (i = 0; i < 4; i++) {
                ply = sub->ply[i];
                if (ply->hook != fn_1_9DF8) {
                    ply->hook = fn_1_B0B8;
                    ply->p684->e00 = 0;
                }
                ply->v680 = 0;
                ply->v682 = 0;
            }
            MgSeqWinnerSet(-1, -1, -1, -1);
        }
        sub->v1da = 0;
        sub->v1d0++;
        break;
    }
    for (i = 0; i < 4; i++) {
        ply = sub->ply[i];
        if (ply->hook != 0) {
            if (ply->hook(ply->p684, ply) != 0 && ply->hook == fn_1_9DF8) {
                ply->hook = fn_1_B0B8;
                ply->p684->e00 = 0;
            }
            if (ply->hook == fn_1_9DF8) {
                fn_1_5FD0(ply);
                fn_1_611C(ply);
                omSetTra(ply->obj, ply->pos.x, ply->pos.y, ply->pos.z);
                omSetRot(ply->obj, ply->rot.x, ply->rot.y, ply->rot.z);
            }
            if (ply->hook == fn_1_D928) {
                ply->hook = fn_1_B0B8;
                ply->p684->e00 = 0;
            }
        }
    }
}

void fn_1_1D20(s16 mode, s16 frameNo)
{
}

void fn_1_1D24(s16 mode, s16 frameNo)
{
    M515_SUB *sub = M515_SUBOF(lbl_1_bss_4);

    fn_1_9920(&sub->esp);
}

void fn_1_1D60(OMOBJ *obj)
{
    M515_PLY *ply;
    s32 i;
    s32 j;
    s32 unused;
    M515_SUB *sub;
    u16 *tbl;
    HuVecF start = { -150.0f, 25.0f, 1000.0f };
    f32 rotY[4] = { 180.0f, 90.0f, 270.0f, 0.0f };
    s16 pan[4] = { 0x30, 0x50, 0x30, 0x50 };

    ply = obj->data;
    sub = M515_SUBOF(lbl_1_bss_4);
    ply->v0e = 0;
    ply->idx = obj->work[0];
    ply->chr = GwPlayerConf[ply->idx].charNo;
    ply->pad = GwPlayerConf[ply->idx].padNo;
    ply->type = GwPlayerConf[ply->idx].type;
    ply->dif = GwPlayerConf[ply->idx].dif;
    obj->mdlId[0] = CharModelCreate(ply->chr, 8);
    for (i = 0; i < 8; i++) {
        obj->mtnId[i] = CharMotionCreate(ply->chr, lbl_1_data_118[ply->chr][i]);
    }
    CharMotionSet(ply->chr, obj->mtnId[0]);
    Hu3DModelAttrSet(obj->mdlId[0], 0x40000001);
    Hu3DModelLayerSet(obj->mdlId[0], 3);
    Hu3DModelCameraSet(obj->mdlId[0], lbl_1_rodata_10[ply->idx]);
    Hu3DModelShadowSet(obj->mdlId[0]);
    CharEffectLayerSet(4);
    CharModelStepFxSet(ply->chr, 1);
    CharMotionDataClose(ply->chr);
    tbl = lbl_1_data_1094[sub->v02[ply->v0e]];
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            ply->cell[i][j].type = *tbl++;
            ply->cell[i][j].vec.x = 150.0f * j - 300.0f;
            ply->cell[i][j].vec.y = 0.0f;
            ply->cell[i][j].vec.z = 150.0f * i - 300.0f;
        }
    }
    for (i = 0; i < 5; i++) {
        ply->cell[0][i].nbr[3] = 0;
        ply->cell[0][i].nbr[2] = &ply->cell[1][i];
    }
    for (i = 0; i < 5; i++) {
        ply->cell[4][i].nbr[2] = 0;
        ply->cell[4][i].nbr[3] = &ply->cell[3][i];
    }
    for (i = 0; i < 5; i++) {
        ply->cell[i][0].nbr[1] = 0;
        ply->cell[i][0].nbr[0] = &ply->cell[i][1];
    }
    for (i = 0; i < 5; i++) {
        ply->cell[i][4].nbr[0] = 0;
        ply->cell[i][4].nbr[1] = &ply->cell[i][3];
    }
    for (i = 1; i < 4; i++) {
        for (j = 0; j < 5; j++) {
            ply->cell[i][j].nbr[3] = &ply->cell[i - 1][j];
            ply->cell[i][j].nbr[2] = &ply->cell[i + 1][j];
        }
    }
    for (i = 0; i < 5; i++) {
        for (j = 1; j < 4; j++) {
            ply->cell[i][j].nbr[1] = &ply->cell[i][j - 1];
            ply->cell[i][j].nbr[0] = &ply->cell[i][j + 1];
        }
    }
    ply->cell[0][2].nbr[3] = &ply->ext[1];
    ply->cell[4][2].nbr[2] = &ply->ext[0];
    ply->ext[0].nbr[3] = &ply->cell[4][2];
    ply->ext[0].nbr[0] = 0;
    ply->ext[0].nbr[1] = 0;
    ply->ext[0].nbr[2] = 0;
    ply->ext[0].type = 3;
    ply->ext[0].vec.x = 0.0f;
    ply->ext[0].vec.y = 25.0f;
    ply->ext[0].vec.z = 450.0f;
    ply->ext[1].nbr[3] = 0;
    ply->ext[1].nbr[0] = 0;
    ply->ext[1].nbr[1] = 0;
    ply->ext[1].nbr[2] = &ply->cell[0][2];
    ply->ext[1].type = 4;
    ply->ext[1].vec.x = 0.0f;
    ply->ext[1].vec.y = 25.0f;
    ply->ext[1].vec.z = -450.0f;
    ply->p3f8 = &ply->ext[0];
    if (ply->idx == 0) {
        ply->pos = start;
        ply->rot.x = 0.0f;
        ply->rot.y = 90.0f;
        ply->rot.z = 0.0f;
        CharMotionSet(ply->chr, obj->mtnId[1]);
    } else {
        ply->pos = ply->p3f8->vec;
        ply->rot.x = 0.0f;
        ply->rot.y = 180.0f;
        ply->rot.z = 0.0f;
    }
    omSetTra(obj, ply->pos.x, ply->pos.y, ply->pos.z);
    omSetRot(obj, ply->rot.x, ply->rot.y, ply->rot.z);
    ply->v482 = 0;
    ply->v484 = 15;
    ply->v486 = 5;
    ply->v480 = 0;
    ply->v47c = 0;
    ply->v47e = 0;
    ply->from = ply->p3f8->vec;
    ply->vec = ply->p3f8->vec;
    ply->v680 = 0;
    ply->v682 = 0;
    ply->p684 = &sub->entry[ply->idx];
    ply->hook = 0;
    fn_1_73B0(ply, 0);
    fn_1_73B0(ply, 1);
    fn_1_73B0(ply, 2);
    fn_1_7768(ply, 0);
    fn_1_77F0(ply, 1);
    fn_1_77F0(ply, 2);
    fn_1_69B8(ply, sub);
    fn_1_6FE8(ply);
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            if (ply->cell[i][j].type == 2) {
                ply->cell[i][j].item->f24 = rand8() / 86 + 1;
                Hu3DModelRotSet(ply->cell[i][j].item->mdl, 0.0f, 0.0f, 0.0f);
                Hu3DModelRotSet(ply->cell[i][j].item->mdl, 0.0f, rotY[ply->cell[i][j].item->f24], 0.0f);
            }
        }
    }
    ply->obj = obj;
    fn_1_E274(&ply->fx, 0, ply->dif);
    CharModelVoicePanSet(ply->chr, pan[ply->idx]);
    obj->objFunc = fn_1_2684;
}

void fn_1_2684(OMOBJ *obj)
{
    M515_PLY *ply;
    M515_SUB *sub;
    s32 i;
    s32 r27;
    s32 r26;

    sub = M515_SUBOF(lbl_1_bss_4);
    ply = obj->data;
    fn_1_E548(ply);
    if (sub->v1d4 != 0) {
        if (ply->v482 == 4) {
            ply->v682 = 0;
            if ((s16)fn_1_7878(ply) != 0) {
                ply->v482 = 0;
            }
            fn_1_5FD0(ply);
            fn_1_611C(ply);
        } else if (ply->v482 == 3) {
            if (fn_1_6324(ply, sub) != 0) {
                ply->v482 = 0;
            }
        } else {
            if (ply->type != 0) {
                ply->v680 = fn_1_DD18(ply);
            } else {
                ply->v680 = fn_1_7924(ply);
            }
            if (ply->v680 == 0x20) {
                ply->v682 = 0;
                ply->v482 = 4;
                ply->hook = fn_1_D928;
                return;
            }
            r27 = fn_1_5FD0(ply);
            r26 = fn_1_611C(ply);
            fn_1_6980(ply);
            if (r27 != 0 && r26 != 0) {
                ply->v482 = fn_1_66D0(ply);
                if ((ply->v482 == 0 || ply->v482 == 2) && ply->v682 > 0) {
                    ply->v482 = 3;
                    fn_1_6318(ply);
                    ply->v682 = 0;
                }
            }
            switch (ply->v482) {
            case 0:
                if (obj->mtnId[0] != Hu3DMotionIDGet(obj->mdlId[0])
                    && obj->mtnId[0] != Hu3DMotionShiftIDGet(obj->mdlId[0])) {
                    Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[0], 0.0f, 5.0f, 0x40000001);
                }
                break;
            case 1:
            case 2:
                if (obj->mtnId[2] != Hu3DMotionIDGet(obj->mdlId[0])
                    && obj->mtnId[2] != Hu3DMotionShiftIDGet(obj->mdlId[0])) {
                    Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[2], 0.0f, 5.0f, 0x40000001);
                }
                break;
            case 3:
                Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[3], 0.0f, 5.0f, 0x40000001);
                break;
            }
            fn_1_81A8(ply->p684->slot, ply);
        }
        if (ply->v482 != 5) {
            omSetTra(obj, ply->pos.x, ply->pos.y, ply->pos.z);
            omSetRot(obj, ply->rot.x, ply->rot.y, ply->rot.z);
        }
        for (i = 0; i < ply->v5e8; i++) {
            fn_1_7BE8(&ply->item[i], sub);
        }
    }
}

void fn_1_29CC(void)
{
}

void fn_1_29D0(OMOBJ *obj)
{
    s32 i;
    M515_SUB *sub;
    s32 k;

    sub = obj->data;
    for (i = 0; i < 4; i++) {
        fn_1_7E5C(&sub->slot[i], i);
        fn_1_9804(&sub->entry[i], &sub->slot[i], i);
    }
    fn_1_5C40(sub);
    for (i = 0; i < 4; i++) {
        for (k = 0; k < 3; k++) {
            sub->v1dc[i][k] = 0;
        }
    }
    Hu3DLayerHookSet(1, fn_1_CC20);
    obj->objFunc = fn_1_2AC0;
}

void fn_1_2AC0(OMOBJ *obj)
{
    M515_SUB *sub;
    s32 i;
    HuVecF *pp;
    HuVecF pos;

    sub = obj->data;
    fn_1_5DB0(sub);
    for (i = 0; i < 4; i++) {
        fn_1_7EE0(&sub->slot[i]);
        pos = sub->ply[i]->pos;
        pp = &pos;
        fn_1_C958(i, pp, sub->v1f4);
    }
}

void fn_1_2B68(void)
{
}

void fn_1_2B6C(M515_SUB *sub)
{
    s32 i;
    s32 row;
    s32 col;

    sub->v00 = 0;
    for (i = 0; i < 4; i++) {
        row = i / 2;
        col = i % 2;
        Hu3DCameraCreate(lbl_1_rodata_10[i]);
        Hu3DCameraPerspectiveSet(lbl_1_rodata_10[i], 15.0f, 20.0f, 12000.0f, 1.17f);
        Hu3DCameraPosSet(lbl_1_rodata_10[i], 0.0f, 300.0f, 1000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 150.0f,
            0.0f);
    }
    Hu3DCameraViewportSet(lbl_1_rodata_10[0], 8.0f, 20.0f, 320.0f, 240.0f, 0.0f, 1.0f);
    Hu3DCameraViewportSet(lbl_1_rodata_10[1], 312.0f, 20.0f, 320.0f, 240.0f, 0.0f, 1.0f);
    Hu3DCameraViewportSet(lbl_1_rodata_10[2], 8.0f, 220.0f, 320.0f, 240.0f, 0.0f, 1.0f);
    Hu3DCameraViewportSet(lbl_1_rodata_10[3], 312.0f, 220.0f, 320.0f, 240.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(lbl_1_rodata_10[0], 0, 0, 319, 239);
    Hu3DCameraScissorSet(lbl_1_rodata_10[1], 321, 0, 319, 239);
    Hu3DCameraScissorSet(lbl_1_rodata_10[2], 0, 241, 319, 239);
    Hu3DCameraScissorSet(lbl_1_rodata_10[3], 321, 241, 319, 239);
    fn_1_9828(&sub->esp);
    fn_1_9958(&sub->esp);
}

void fn_1_2EB4(M515_SUB *sub)
{
    HU3DLIGHTID lightId;

    lightId = Hu3DGLightCreate(0.0f, 4800.0f, 3600.0f, 0.0f, -1.0f, -1.0f, 0xFF, 0xFF, 0xFF);
    Hu3DGLightInfinitytSet(lightId);
}

void fn_1_5C40(M515_SUB *sub)
{
    s32 i;

    for (i = 0; i < 0x77; i++) {
        sub->node[i].prev = 0;
        sub->node[i].next = &sub->node[i + 1];
        sub->node[i].func = 0;
    }
    sub->node[i].prev = 0;
    sub->node[i].next = 0;
    sub->node[i].func = 0;
    sub->head = 0;
    sub->cur = &sub->node[0];
}

M515_NODE2 *fn_1_5CE4(M515_SUB *sub)
{
    M515_NODE2 *node = sub->cur;

    if (node != 0) {
        sub->cur = node->next;
        node->f00 = -1;
        node->f02 = -1;
        node->f04 = 0;
        node->next = sub->head;
        if (node->next != 0) {
            node->next->prev = node;
        }
        sub->head = node;
    }

    return node;
}

// The dead `node = 0` store is what emits the lone li before the blr.
void fn_1_5D48(M515_SUB *sub, M515_NODE2 *node)
{
    if (node != 0) {
        if (node->prev != 0) {
            node->prev->next = node->next;
        } else {
            sub->head = node->next;
        }

        if (node->next != 0) {
            node->next->prev = node->prev;
        }

        node->prev = 0;
        node->next = sub->cur;
        node->func = 0;
        sub->cur = node;
        node = 0;
    }
}

void fn_1_5DB0(M515_SUB *sub)
{
    M515_NODE2 *node;
    M515_NODE2 *next;

    node = sub->head;
    while (node != 0) {
        next = node->next;
        if (node->func(node) != 0) {
            fn_1_5D48(sub, node);
        }
        node = next;
    }
}

void fn_1_5E88(HU3DMODELID modelId, HuVecF *pos, HuVecF *rot, s16 layerNo, u16 cameraBit)
{
    Hu3DModelAttrSet(modelId, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(modelId, 0x40000002);
    Hu3DModelLayerSet(modelId, layerNo);
    Hu3DModelCameraSet(modelId, cameraBit);
    Hu3DModelPosSet(modelId, pos->x, pos->y, pos->z);
    Hu3DModelRotSet(modelId, rot->x, rot->y, rot->z);
}

void fn_1_5F2C(HU3DMODELID modelId, HuVecF *pos, HuVecF *rot, s16 layerNo, u16 cameraBit)
{
    Hu3DModelAttrSet(modelId, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(modelId, 0x40000080);
    Hu3DModelLayerSet(modelId, layerNo);
    Hu3DModelCameraSet(modelId, cameraBit);
    Hu3DModelPosSet(modelId, pos->x, pos->y, pos->z);
    Hu3DModelRotSet(modelId, rot->x, rot->y, rot->z);
}

s32 fn_1_5FD0(M515_PLY *ply)
{
    f32 t;
    s32 ret;

    ret = 0;
    ply->v484++;
    if (ply->v484 >= 15) {
        ply->v484 = 15;
        ply->from = ply->vec;
        ret = 1;
    }
    t = (f32)ply->v484 / 15.0f;
    ply->pos.x = (1.0f - t) * ply->from.x + t * ply->vec.x;
    ply->pos.y = (1.0f - t) * ply->from.y + t * ply->vec.y;
    ply->pos.z = (1.0f - t) * ply->from.z + t * ply->vec.z;
    return ret;
}

void fn_1_60E8(M515_PLY *ply, M515_CELL *slot)
{
    if (ply->v484 == 15) {
        ply->vec = slot->vec;
        ply->p3f8 = slot;
        ply->v484 = 0;
    }
}

s32 fn_1_611C(M515_PLY *ply)
{
    s32 ret = 0;
    f32 t;
    f32 ang[4] = { 180.0f, 90.0f, 270.0f, 0.0f };

    ply->v486++;
    if (ply->v486 >= 5) {
        ply->v486 = 5;
        ply->v47c = ply->v47e;
        ret = 1;
    }
    if ((ply->v47c == 2 && ply->v47e == 3) || (ply->v47c == 3 && ply->v47e == 2)) {
        ang[3] += 360.0f;
    }
    if (ply->v0e == 1) {
        if ((ply->v47c == 0 && ply->v47e == 3) || (ply->v47c == 3 && ply->v47e == 0)) {
            ang[3] += 360.0f;
        }
    } else if ((ply->v47c == 2 && ply->v47e == 1) || (ply->v47c == 1 && ply->v47e == 2)) {
        ang[1] += 360.0f;
    }
    t = (f32)ply->v486 / 5.0f;
    ply->rot.y = (1.0f - t) * ang[ply->v47c] + t * ang[ply->v47e];
    return ret;
}

void fn_1_62FC(M515_PLY *ply, s16 value)
{
    if (ply->v486 == 5) {
        ply->v47e = value;
        ply->v486 = 0;
    }
}

void fn_1_6318(M515_PLY *ply)
{
    ply->v480 = 0;
}

s32 fn_1_6324(M515_PLY *ply, M515_SUB *sub)
{
    s32 ret = 0;
    M515_CELL *cell;
    M515_CELL *next;
    M515_ITEM *item;
    s16 a[4] = { 3, 2, 1, 0 };
    s16 b[4][4] = { { 0, 2, 1, 3 }, { 1, 0, 3, 2 }, { 2, 3, 0, 1 }, { 3, 1, 2, 0 } };
    s16 pan[4] = { 0x30, 0x50, 0x30, 0x50 };

    cell = 0;
    next = 0;
    ply->v480++;
    if (ply->v480 == 10) {
        switch (ply->v47c) {
        case 0:
            cell = ply->p3f8->nbr[3];
            if (cell != 0 && cell->nbr[3] != 0) {
                next = cell->nbr[3];
            }
            break;
        case 1:
            cell = ply->p3f8->nbr[0];
            if (cell != 0 && cell->nbr[0] != 0) {
                next = cell->nbr[0];
            }
            break;
        case 2:
            cell = ply->p3f8->nbr[1];
            if (cell != 0 && cell->nbr[1] != 0) {
                next = cell->nbr[1];
            }
            break;
        case 3:
            cell = ply->p3f8->nbr[2];
            if (cell != 0 && cell->nbr[2] != 0) {
                next = cell->nbr[2];
            }
            break;
        }
        if (cell != 0 && cell->type == 2) {
            if ((ply->idx & 1) != 0) {
                HuAudFXPlayPan(0x6E2, pan[ply->idx]);
            } else {
                HuAudFXPlayPan(0x6E1, pan[ply->idx]);
            }
            omVibrate(ply->idx, 12, 4, 2);
            CharModelHitCreate(ply->chr);
            item = cell->item;
            if (next != 0 && next->type == 0) {
                if (item != 0) {
                    item->a = cell->vec;
                    item->b = next->vec;
                }
                next->type = cell->type;
                cell->type = 0;
                next->item = cell->item;
                cell->item = 0;
            } else if (item != 0) {
                item->a = cell->vec;
                item->b = cell->vec;
            }
            if (item != 0) {
                item->f20 = 1;
                item->mot = sub->mot[a[b[ply->v47c][item->f24]]];
            }
        }
    }
    if (ply->v480 == 0x19) {
        Hu3DMotionShiftSet(ply->obj->mdlId[0], ply->obj->mtnId[0], 0.0f, 5.0f, 0x40000001);
    }
    if (ply->v480 > 0x1D) {
        ret = 1;
    }
    return ret;
}

s16 fn_1_66D0(M515_PLY *ply)
{
    s32 ret = 0;
    M515_CELL *cell;

    switch (ply->v680 & 0xF) {
    case 0:
    case 3:
    case 5:
    case 6:
    case 7:
        break;
    case 1:
        ret = 2;
        cell = ply->p3f8->nbr[3];
        if (cell != 0 && (cell->type == 0 || cell->type == 4)) {
            if (ply->v484 == 15) {
                ply->vec = cell->vec;
                ply->p3f8 = cell;
                ply->v484 = 0;
            }
            if (cell->type == 4) {
                if (ply->v0e == 2) {
                    ret = 5;
                } else {
                    ply->hook = fn_1_9DF8;
                    ret = 4;
                }
            } else {
                ret = 1;
            }
        }
        if (ply->v486 == 5) {
            ply->v47e = 0;
            ply->v486 = 0;
        }
        break;
    case 2:
        ret = 2;
        cell = ply->p3f8->nbr[0];
        if (cell != 0 && (cell->type == 0 || cell->type == 4)) {
            if (ply->v484 == 15) {
                ply->vec = cell->vec;
                ply->p3f8 = cell;
                ply->v484 = 0;
            }
            ret = 1;
        }
        if (ply->v486 == 5) {
            ply->v47e = 1;
            ply->v486 = 0;
        }
        break;
    case 4:
        ret = 2;
        cell = ply->p3f8->nbr[1];
        if (cell != 0 && (cell->type == 0 || cell->type == 4)) {
            if (ply->v484 == 15) {
                ply->vec = cell->vec;
                ply->p3f8 = cell;
                ply->v484 = 0;
            }
            ret = 1;
        }
        if (ply->v486 == 5) {
            ply->v47e = 2;
            ply->v486 = 0;
        }
        break;
    case 8:
        ret = 2;
        cell = ply->p3f8->nbr[2];
        if (cell != 0 && (cell->type == 0 || cell->type == 4)) {
            if (ply->v484 == 15) {
                ply->vec = cell->vec;
                ply->p3f8 = cell;
                ply->v484 = 0;
            }
            if (cell->type == 4) {
                if (ply->v0e == 2) {
                    ret = 5;
                } else {
                    ply->hook = fn_1_9DF8;
                    ret = 4;
                }
            } else {
                ret = 1;
            }
        }
        if (ply->v486 == 5) {
            ply->v47e = 3;
            ply->v486 = 0;
        }
        break;
    }
    return ret;
}

void fn_1_6980(M515_PLY *ply)
{
    if (ply->v682 > 0) {
        ply->v682--;
    }

    switch (ply->v680 & 0x10) {
    case 0x10:
        ply->v682 = 5;
        break;
    }
}

void fn_1_69B8(M515_PLY *ply, M515_SUB *sub)
{
    s32 i;
    f32 sc;

    sc = 1.1f;
    if (sub->mdl < 0) {
        sub->mdl = Hu3DModelCreateData(lbl_1_data_364[1]);
        ply->item[0].mdl = sub->mdl;
        for (i = 0; i < 8; i++) {
            sub->mot[i] = Hu3DJointMotionData(sub->mdl, lbl_1_data_3A8[i]);
        }
        Hu3DMotionSet(sub->mdl, sub->mot[4]);
    } else {
        ply->item[0].mdl = Hu3DModelLink(sub->mdl);
    }
    Hu3DModelAttrSet(ply->item[0].mdl, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(ply->item[0].mdl, 0x40000002);
    Hu3DModelLayerSet(ply->item[0].mdl, 3);
    Hu3DModelPosSet(ply->item[0].mdl, 0.0f, 1.0f, 0.0f);
    Hu3DModelRotSet(ply->item[0].mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(ply->item[0].mdl, lbl_1_rodata_10[ply->idx]);
    Hu3DModelScaleSet(ply->item[0].mdl, 1.1f, 1.1f, 1.1f);
    ply->item[0].f20 = 0;
    ply->item[0].f28 = 45.0f * (rand8() / 32);
    for (i = 1; i < 8; i++) {
        ply->item[i].mdl = Hu3DModelLink(sub->mdl);
        Hu3DModelAttrSet(ply->item[i].mdl, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(ply->item[i].mdl, 0x40000002);
        Hu3DModelLayerSet(ply->item[i].mdl, 3);
        Hu3DModelPosSet(ply->item[i].mdl, 0.0f, 1.0f, 0.0f);
        Hu3DModelRotSet(ply->item[i].mdl, 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(ply->item[i].mdl, lbl_1_rodata_10[ply->idx]);
        Hu3DModelScaleSet(ply->item[i].mdl, 1.1f, 1.1f, 1.1f);
        ply->item[i].f20 = 0;
        ply->item[i].f28 = 45.0f * (rand8() / 32);
    }
    if (sub->mdl2 < 0) {
        sub->mdl2 = Hu3DModelCreateData(lbl_1_data_364[0]);
        ply->item[0].mdl2 = sub->mdl2;
    } else {
        ply->item[0].mdl2 = Hu3DModelLink(sub->mdl2);
    }
    Hu3DModelAttrSet(ply->item[0].mdl2, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(ply->item[0].mdl2, 0x40000002);
    Hu3DModelLayerSet(ply->item[0].mdl2, 2);
    Hu3DModelPosSet(ply->item[0].mdl2, 0.0f, 0.5f, 0.0f);
    Hu3DModelRotSet(ply->item[0].mdl2, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(ply->item[0].mdl2, lbl_1_rodata_10[ply->idx]);
    Hu3DModelScaleSet(ply->item[0].mdl, 1.1f, 1.1f, 1.1f);
    for (i = 1; i < 8; i++) {
        ply->item[i].mdl2 = Hu3DModelLink(sub->mdl2);
        Hu3DModelAttrSet(ply->item[i].mdl2, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(ply->item[i].mdl2, 0x40000002);
        Hu3DModelLayerSet(ply->item[i].mdl2, 2);
        Hu3DModelPosSet(ply->item[i].mdl2, 0.0f, 0.5f, 0.0f);
        Hu3DModelRotSet(ply->item[i].mdl2, 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(ply->item[i].mdl2, lbl_1_rodata_10[ply->idx]);
        Hu3DModelScaleSet(ply->item[i].mdl2, 1.1f, 1.1f, 1.1f);
    }
}

void fn_1_6FE8(M515_PLY *ply)
{
    f32 rotY[4] = { 180.0f, 90.0f, 270.0f, 0.0f };
    s32 i;
    s32 j;
    s32 n;

    ply->v5e8 = 0;
    for (i = 0, n = 0; i < 5; i++) {
        for (j = 0; j < 5; j++, n++) {
            if (ply->cell[i][j].type == 2) {
                ply->cell[i][j].item = &ply->item[ply->v5e8];
                ply->v5e8++;
                ply->cell[i][j].item->a = ply->cell[i][j].vec;
                ply->cell[i][j].item->b = ply->cell[i][j].vec;
                ply->cell[i][j].item->f24 = rand8() / 64;
                Hu3DModelAttrReset(ply->cell[i][j].item->mdl, HU3D_ATTR_DISPOFF);
                Hu3DModelPosSet(ply->cell[i][j].item->mdl, ply->cell[i][j].vec.x,
                    1.0f + ply->cell[i][j].vec.y, ply->cell[i][j].vec.z);
                Hu3DModelRotSet(ply->cell[i][j].item->mdl, 0.0f, rotY[ply->cell[i][j].item->f24], 0.0f);
                Hu3DModelAttrReset(ply->cell[i][j].item->mdl2, HU3D_ATTR_DISPOFF);
                Hu3DModelPosSet(ply->cell[i][j].item->mdl2, ply->cell[i][j].vec.x,
                    0.5f + ply->cell[i][j].vec.y, ply->cell[i][j].vec.z);
            }
        }
    }
}

void fn_1_72E4(M515_PLY *ply)
{
    s32 i;
    M515_SUB *sub;

    sub = M515_SUBOF(lbl_1_bss_4);
    for (i = 0; i < 8; i++) {
        Hu3DModelAttrSet(ply->item[i].mdl, HU3D_ATTR_DISPOFF);
        Hu3DMotionSet(ply->item[i].mdl, sub->mot[4]);
        Hu3DMotionTimeSet(ply->item[i].mdl, 0.0f);
        Hu3DModelAttrSet(ply->item[i].mdl, 0x40000002);
        Hu3DModelAttrSet(ply->item[i].mdl2, HU3D_ATTR_DISPOFF);
    }
}

void fn_1_73B0(M515_PLY *ply, s32 row)
{
    M515_SUB *sub;
    M515_PLACE *tbl;
    f32 rot0;
    s32 pos;
    s32 id;
    s32 i;
    s32 py;
    s32 px;

    sub = M515_SUBOF(lbl_1_bss_4);
    if (row == 0) {
        tbl = lbl_1_data_110C[sub->v02[row]];
        rot0 = 0.0f;
    } else if (row == 1) {
        tbl = lbl_1_data_1134[sub->v02[row]];
        rot0 = 180.0f;
    } else {
        tbl = lbl_1_data_115C[sub->v02[row]];
        rot0 = 0.0f;
    }
    i = 0;
    while (i < 25) {
        if (tbl->id < 0) {
            break;
        }
        id = tbl->id - 1;
        if (sub->cache[id] < 0) {
            sub->cache[id] = Hu3DModelCreateData(lbl_1_data_320[id]);
            ply->mdlIds[row][i] = sub->cache[id];
        } else {
            ply->mdlIds[row][i] = Hu3DModelLink(sub->cache[id]);
        }
        if (row == 1) {
            pos = 24 - tbl->pos;
        } else {
            pos = tbl->pos;
        }
        py = pos / 5;
        px = pos % 5;
        Hu3DModelAttrSet(ply->mdlIds[row][i], HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(ply->mdlIds[row][i], 0x40000002);
        Hu3DModelLayerSet(ply->mdlIds[row][i], 1);
        Hu3DModelPosSet(ply->mdlIds[row][i], 150.0f * px - 300.0f, 400.0f * row,
            150.0f * py - 300.0f);
        Hu3DModelRotSet(ply->mdlIds[row][i], 0.0f, tbl->rot + rot0, 0.0f);
        Hu3DModelCameraSet(ply->mdlIds[row][i], lbl_1_rodata_10[ply->idx]);
        i++;
        tbl++;
    }
    for (; i < 25; i++) {
        ply->mdlIds[row][i] = -1;
    }
}

void fn_1_7768(M515_PLY *ply, s32 r)
{
    s32 i;

    for (i = 0; i < 25; i++) {
        if (ply->mdlIds[r][i] >= 0) {
            Hu3DModelAttrReset(ply->mdlIds[r][i], HU3D_ATTR_DISPOFF);
        }
    }
}

void fn_1_77F0(M515_PLY *ply, s32 r)
{
    s32 i;

    for (i = 0; i < 25; i++) {
        if (ply->mdlIds[r][i] >= 0) {
            Hu3DModelAttrSet(ply->mdlIds[r][i], HU3D_ATTR_DISPOFF);
        }
    }
}

s32 fn_1_7878(M515_PLY *self)
{
    s32 ret = 0;
    M515_SUB *global = M515_SUBOF(lbl_1_bss_4);

    if (self->hook != 0) {
        if (self->hook(self->p684, self) != 0) {
            if (global->v0cac == 0) {
                self->hook = 0;
                ret = 1;
            } else {
                self->hook = fn_1_B0B8;
                self->p684->e00 = 0;
            }
        }
    }

    return ret;
}

u16 fn_1_7924(M515_PLY *ply)
{
    u16 ret;
    f32 sx;
    f32 sy;
    f32 ang;
    f32 ay;
    f32 ax;
    f32 sum;

    ret = 0;
    sx = HuPadStkX[ply->pad];
    sy = HuPadStkY[ply->pad];
    if (ply->v0e == 1) {
        sx *= -1.0f;
        sy *= -1.0f;
    }
    ang = HuAtan(sx, sy);
    if (sy < 0.0f) {
        ay = -sy;
    } else {
        ay = sy;
    }
    if (sx < 0.0f) {
        ax = -sx;
    } else {
        ax = sx;
    }
    sum = ax + ay;
    if (sum > 32.0) {
        if (ang >= -30.0f && ang < 60.0f) {
            ret = 1;
        } else if (ang >= 60.0f && ang < 150.0f) {
            ret = 2;
        } else if (ang >= -120.0f && ang < -30.0f) {
            ret = 4;
        } else {
            ret = 8;
        }
    }
    if ((HuPadBtnDown[ply->pad] & 0x800) != 0) {
        ply->v682 = 0;
        ret = 0x20;
    } else if ((HuPadBtnDown[ply->pad] & 0x200) != 0) {
        ret |= 0x10;
    }
    return ret;
}

void fn_1_7BE8(M515_ITEM *item, M515_SUB *sub)
{
    HuVecF p;
    f32 t;

    switch (item->f20) {
    case 0:
        break;
    case 1:
        item->f22 = 0;
        Hu3DMotionSet(item->mdl, item->mot);
        Hu3DModelAttrSet(item->mdl, 0x40000001);
        Hu3DModelAttrReset(item->mdl, 0x40000002);
        item->f20++;
    case 2:
        item->f22++;
        if (item->f22 >= 2) {
            item->f20++;
            return;
        }
        break;
    case 3:
        item->f22++;
        if (item->f22 >= 10) {
            item->f20++;
        }
        t = (f32)item->f22 - 2.0f;
        t *= 0.125f;
        p.x = (1.0f - t) * item->a.x + t * item->b.x;
        p.y = 1.0f + item->b.y;
        p.z = (1.0f - t) * item->a.z + t * item->b.z;
        Hu3DModelPosSet(item->mdl, p.x, p.y, p.z);
        p.y = 0.5f + item->b.y;
        Hu3DModelPosSet(item->mdl2, p.x, p.y, p.z);
        return;
    case 4:
        item->f22++;
        if (item->f22 >= 20) {
            Hu3DMotionShiftSet(item->mdl, sub->mot[7], 0.0f, 3.0f, 0x40000001);
            item->f20++;
            return;
        }
        break;
    case 5:
        item->f22++;
        if (item->f22 >= 100) {
            Hu3DModelAttrSet(item->mdl, 0x40000002);
        }
        break;
    }
}

void fn_1_7E5C(M515_SLOT *slot, s16 idx)
{
    f32 *src;

    if (idx == 0) {
        src = lbl_1_data_28;
    } else {
        src = &lbl_1_data_28[6];
    }
    slot->f00 = src[0];
    slot->f04 = src[1];
    slot->f08 = src[2];
    slot->look.x = src[3];
    slot->look.y = src[4];
    slot->look.z = src[5];
    slot->state = 0;
    slot->timer = 0;
    slot->idx = idx;
    slot->f2c = 0;
}

void fn_1_7EE0(M515_SLOT *slot)
{
    f32 x;
    f32 y;
    f32 z;

    if (slot->f00 > 360.0f) {
        slot->f00 = slot->f00 - 360.0f;
    }
    if (slot->f00 < 0.0f) {
        slot->f00 += 360.0f;
    }
    if (slot->f04 > 360.0f) {
        slot->f04 = slot->f04 - 360.0f;
    }
    if (slot->f04 < 0.0f) {
        slot->f04 += 360.0f;
    }
    x = slot->look.x + (0.5f * slot->f08) * HuCos(slot->f04) * HuCos(slot->f00);
    y = slot->look.y + (0.5f * slot->f08) * HuSin(slot->f04);
    z = slot->look.z + (0.5f * slot->f08) * HuCos(slot->f04) * HuSin(slot->f00);
    Hu3DCameraPosSet(lbl_1_rodata_10[slot->idx], x, y, z, 0.0f, 1.0f, 0.0f, slot->look.x,
        slot->look.y, slot->look.z);
}

void fn_1_81A8(M515_SLOT *slot, M515_PLY *ply)
{
    f32 a;

    if (ply->v0e == 1) {
        if (ply->pos.x > 150.0f) {
            a = ply->pos.x - 150.0f;
        } else if (ply->pos.x < -150.0f) {
            a = 0.5f * (150.0f + ply->pos.x);
        } else {
            a = 0.0f;
        }
    } else if (ply->pos.x > 150.0f) {
        a = 0.5f * (ply->pos.x - 150.0f);
    } else if (ply->pos.x < -150.0f) {
        a = 150.0f + ply->pos.x;
    } else {
        a = 0.0f;
    }
    if (ply->v0e == 1) {
        slot->look.x = a - 25.0f;
    } else {
        slot->look.x = 25.0f + a;
    }
    if (ply->pos.z > 150.0f) {
        a = ply->pos.z - 150.0f;
    } else if (ply->pos.z < -150.0f) {
        a = 150.0f + ply->pos.z;
    } else {
        a = 0.0f;
    }
    if (a > 150.0f) {
        a = 150.0f;
    }
    if (a < -150.0f) {
        a = -150.0f;
    }
    if (ply->v0e == 1) {
        slot->look.z = 75.0f + a;
        return;
    }
    slot->look.z = a - 75.0f;
}

s32 fn_1_83E8(M515_SLOT *slot)
{
    M515_SUB *sub;
    s32 ret;
    f32 k;
    f32 inv;
    f32 t;

    ret = 0;
    sub = M515_SUBOF(lbl_1_bss_4);
    switch (slot->state) {
    case 0:
        slot->timer = 0;
        slot->state++;
    case 1:
        slot->timer++;
        if (slot->timer == 0x37) {
            fn_1_BE78(sub, sub->id16[slot->idx], 0);
            fn_1_BF24(sub, sub->id26[slot->idx], 1);
        }
        if (slot->timer >= 0x78) {
            fn_1_BE78(sub, sub->id96[slot->idx][0], 0);
            fn_1_BE78(sub, sub->id76[slot->idx].a, 0);
            fn_1_BE78(sub, sub->id76[slot->idx].a, 0);
            slot->state = 0;
            ret = 1;
        }
        t = (f32)slot->timer / 120.0f;
        k = 1.0 - (0.5 + 0.5 * HuCos(180.0f * t));
        inv = 1.0f - k;
        slot->f00 = lbl_1_data_28[0] * inv + lbl_1_data_28[6] * k;
        slot->f04 = lbl_1_data_28[1] * inv + lbl_1_data_28[7] * k;
        slot->f08 = lbl_1_data_28[2] * inv + lbl_1_data_28[8] * k;
        slot->look.x = lbl_1_data_28[3] * inv + lbl_1_data_28[9] * k;
        slot->look.y = lbl_1_data_28[4] * inv + lbl_1_data_28[10] * k;
        slot->look.z = lbl_1_data_28[5] * inv + lbl_1_data_28[11] * k;
        break;
    }
    return ret;
}

s32 fn_1_86F4(M515_SLOT *slot)
{
    M515_SUB *sub;
    f32 k;
    f32 inv;
    f32 t;
    s32 ret;

    ret = 0;
    sub = M515_SUBOF(lbl_1_bss_4);
    switch (slot->state) {
    case 0:
        slot->timer = 0;
        slot->vec = slot->look;
        slot->state++;
    case 1:
        slot->timer++;
        if (slot->timer >= 70) {
            slot->state = 0;
            ret = 1;
        }
        t = (f32)slot->timer / 70.0f;
        k = 1.0 - (0.5 + 0.5 * HuCos(180.0f * t));
        inv = 1.0f - k;
        slot->f00 = lbl_1_data_28[6] * inv + lbl_1_data_28[12] * k;
        slot->f04 = lbl_1_data_28[7] * inv + lbl_1_data_28[13] * k;
        slot->f08 = lbl_1_data_28[8] * inv + lbl_1_data_28[14] * k;
        slot->look.x = slot->vec.x * inv + lbl_1_data_28[15] * k;
        slot->look.y = slot->vec.y * inv + lbl_1_data_28[16] * k;
        slot->look.z = slot->vec.z * inv + lbl_1_data_28[17] * k;
        break;
    }
    return ret;
}

s32 fn_1_8964(M515_SLOT *slot)
{
    M515_SUB *sub;
    f32 k;
    f32 inv;
    f32 t;
    s32 ret;

    ret = 0;
    sub = M515_SUBOF(lbl_1_bss_4);
    switch (slot->state) {
    case 0:
        slot->timer = 0;
        slot->state++;
    case 1:
        slot->timer++;
        if (slot->timer >= 100) {
            slot->state = 0;
            ret = 1;
        }
        t = (f32)slot->timer / 100.0f;
        k = 1.0 - (0.5 + 0.5 * HuCos(180.0f * t));
        inv = 1.0f - k;
        slot->f00 = lbl_1_data_28[12] * inv + lbl_1_data_28[18] * k;
        slot->f04 = lbl_1_data_28[13] * inv + lbl_1_data_28[19] * k;
        slot->f08 = lbl_1_data_28[14] * inv + lbl_1_data_28[20] * k;
        slot->look.x = lbl_1_data_28[15] * inv + lbl_1_data_28[21] * k;
        slot->look.y = lbl_1_data_28[16] * inv + lbl_1_data_28[22] * k;
        slot->look.z = lbl_1_data_28[17] * inv + lbl_1_data_28[23] * k;
        break;
    }
    return ret;
}

s32 fn_1_8BD4(M515_SLOT *slot)
{
    M515_SUB *sub;
    f32 k;
    f32 inv;
    f32 t;
    s32 ret;

    ret = 0;
    sub = M515_SUBOF(lbl_1_bss_4);
    switch (slot->state) {
    case 0:
        slot->timer = 0;
        slot->state++;
    case 1:
        slot->timer++;
        if (slot->timer >= 45) {
            slot->state = 0;
            ret = 1;
        }
        t = (f32)slot->timer / 45.0f;
        k = 1.0 - (0.5 + 0.5 * HuCos(180.0f * t));
        inv = 1.0f - k;
        slot->f00 = lbl_1_data_28[18] * inv + lbl_1_data_28[24] * k;
        slot->f04 = lbl_1_data_28[19] * inv + lbl_1_data_28[25] * k;
        slot->f08 = lbl_1_data_28[20] * inv + lbl_1_data_28[26] * k;
        slot->look.x = lbl_1_data_28[21] * inv + lbl_1_data_28[27] * k;
        slot->look.y = lbl_1_data_28[22] * inv + lbl_1_data_28[28] * k;
        slot->look.z = lbl_1_data_28[23] * inv + lbl_1_data_28[29] * k;
        break;
    }
    return ret;
}

s32 fn_1_8E44(M515_SLOT *slot)
{
    M515_SUB *sub;
    f32 k;
    f32 inv;
    f32 t;
    s32 ret;

    ret = 0;
    sub = M515_SUBOF(lbl_1_bss_4);
    switch (slot->state) {
    case 0:
        slot->timer = 0;
        slot->vec = slot->look;
        slot->state++;
    case 1:
        slot->timer++;
        if (slot->timer >= 70) {
            slot->state = 0;
            ret = 1;
        }
        t = (f32)slot->timer / 70.0f;
        k = 1.0 - (0.5 + 0.5 * HuCos(180.0f * t));
        inv = 1.0f - k;
        slot->f00 = lbl_1_data_28[24] * inv + lbl_1_data_28[30] * k;
        slot->f04 = lbl_1_data_28[25] * inv + lbl_1_data_28[31] * k;
        slot->f08 = lbl_1_data_28[26] * inv + lbl_1_data_28[32] * k;
        slot->look.x = slot->vec.x * inv + lbl_1_data_28[33] * k;
        slot->look.y = slot->vec.y * inv + lbl_1_data_28[34] * k;
        slot->look.z = slot->vec.z * inv + lbl_1_data_28[35] * k;
        break;
    }
    return ret;
}

s32 fn_1_90B4(M515_SLOT *slot)
{
    M515_SUB *sub;
    f32 k;
    f32 inv;
    f32 t;
    s32 ret;

    ret = 0;
    sub = M515_SUBOF(lbl_1_bss_4);
    switch (slot->state) {
    case 0:
        slot->timer = 0;
        slot->state++;
    case 1:
        slot->timer++;
        if (slot->timer >= 100) {
            slot->state = 0;
            ret = 1;
        }
        t = (f32)slot->timer / 100.0f;
        k = 1.0 - (0.5 + 0.5 * HuCos(180.0f * t));
        inv = 1.0f - k;
        slot->f00 = lbl_1_data_28[30] * inv + lbl_1_data_28[36] * k;
        slot->f04 = lbl_1_data_28[31] * inv + lbl_1_data_28[37] * k;
        slot->f08 = lbl_1_data_28[32] * inv + lbl_1_data_28[38] * k;
        slot->look.x = lbl_1_data_28[33] * inv + lbl_1_data_28[39] * k;
        slot->look.y = lbl_1_data_28[34] * inv + lbl_1_data_28[40] * k;
        slot->look.z = lbl_1_data_28[35] * inv + lbl_1_data_28[41] * k;
        break;
    }
    return ret;
}

s32 fn_1_9324(M515_SLOT *slot)
{
    M515_SUB *sub;
    f32 k;
    f32 inv;
    f32 t;
    s32 ret;

    ret = 0;
    sub = M515_SUBOF(lbl_1_bss_4);
    switch (slot->state) {
    case 0:
        slot->timer = 0;
        slot->state++;
    case 1:
        slot->timer++;
        if (slot->timer >= 45) {
            slot->state = 0;
            ret = 1;
        }
        t = (f32)slot->timer / 45.0f;
        k = 1.0 - (0.5 + 0.5 * HuCos(180.0f * t));
        inv = 1.0f - k;
        slot->f00 = lbl_1_data_28[36] * inv + lbl_1_data_28[42] * k;
        slot->f04 = lbl_1_data_28[37] * inv + lbl_1_data_28[43] * k;
        slot->f08 = lbl_1_data_28[38] * inv + lbl_1_data_28[44] * k;
        slot->look.x = lbl_1_data_28[39] * inv + lbl_1_data_28[45] * k;
        slot->look.y = lbl_1_data_28[40] * inv + lbl_1_data_28[46] * k;
        slot->look.z = lbl_1_data_28[41] * inv + lbl_1_data_28[47] * k;
        break;
    }
    return ret;
}

s32 fn_1_9594(M515_SLOT *slot)
{
    M515_SUB *sub;
    f32 k;
    f32 inv;
    f32 t;
    s32 ret;

    ret = 0;
    sub = M515_SUBOF(lbl_1_bss_4);
    switch (slot->state) {
    case 0:
        slot->timer = 0;
        slot->vec = slot->look;
        slot->state++;
    case 1:
        slot->timer++;
        if (slot->timer >= 80) {
            slot->state = 0;
            ret = 1;
        }
        t = (f32)slot->timer / 80.0f;
        k = 1.0 - (0.5 + 0.5 * HuCos(180.0f * t));
        inv = 1.0f - k;
        slot->f00 = lbl_1_data_28[42] * inv + lbl_1_data_28[48] * k;
        slot->f04 = lbl_1_data_28[43] * inv + lbl_1_data_28[49] * k;
        slot->f08 = lbl_1_data_28[44] * inv + lbl_1_data_28[50] * k;
        slot->look.x = slot->vec.x * inv + lbl_1_data_28[51] * k;
        slot->look.y = slot->vec.y * inv + lbl_1_data_28[52] * k;
        slot->look.z = slot->vec.z * inv + lbl_1_data_28[53] * k;
        break;
    }
    return ret;
}

void fn_1_9804(M515_ENTRY *entry, M515_SLOT *slot, s32 index)
{
    entry->e02 = 0;
    entry->e00 = 0;
    entry->slot = slot;
    entry->e0c = index;
    entry->e08 = 0;
}

void fn_1_9828(M515_ESPPAIR *esp)
{
    esp->id0 = espEntry(0x490043, 10, 0);
    espDrawNoSet(esp->id0, 0);
    espPosSet(esp->id0, 576.0f, 240.0f);
    espAttrSet(esp->id0, 1);
    esp->id1 = espEntry(0x490044, 10, 0);
    espDrawNoSet(esp->id1, 0);
    espPosSet(esp->id1, 576.0f, 240.0f);
    espAttrSet(esp->id1, 1);
    esp->f4 = 640.0f;
    esp->f8 = 480.0f;
    esp->fc = 0.0f;
}

void fn_1_9920(M515_ESPPAIR *esp)
{
    espKill(esp->id0);
    espKill(esp->id1);
}

f32 fn_1_BD10(s16 mdl, s32 idx, f32 t)
{
    f32 lo[2] = { 0.0f, 400.0f };
    f32 hi[2] = { 400.0f, 800.0f };
    f32 k;
    f32 y;

    k = 1.0 - (0.5 + 0.5 * HuCos(180.0f * t));
    if (k > 1.0f) {
        k = 1.0f;
    }
    y = (1.0f - k) * lo[idx] + k * hi[idx];
    Hu3DModelPosSet(mdl, 0.0f, y, 0.0f);
    return y;
}

void fn_1_BE78(M515_SUB *sub, s16 mdl, s16 mode)
{
    M515_NODE2 *node;

    node = fn_1_5CE4(sub);
    if (node != 0) {
        if (mode != 0) {
            node->func = fn_1_BFD0;
        } else {
            node->func = fn_1_C0D0;
        }
        node->f00 = mdl;
    }
}

void fn_1_BF24(M515_SUB *sub, s16 mdl, s16 mode)
{
    M515_NODE2 *node;

    node = fn_1_5CE4(sub);
    if (node != 0) {
        if (mode != 0) {
            node->func = fn_1_C150;
        } else {
            node->func = fn_1_C26C;
        }
        node->f00 = mdl;
    }
}

s32 fn_1_BFD0(M515_NODE2 *node)
{
    s32 ret = 0;

    switch (node->f04) {
    case 0:
        if (node->f00 >= 0) {
            Hu3DModelAttrReset(node->f00, HU3D_ATTR_DISPOFF);
            node->f02 = Hu3DMotionIDGet(node->f00);
            if (node->f02 >= 0) {
                Hu3DMotionTimeSet(node->f00, Hu3DMotionMaxTimeGet(node->f00) - 1.0f);
                Hu3DModelAttrSet(node->f00, 0x40000004);
                Hu3DModelAttrReset(node->f00, 0x40000002);
                node->f04++;
            } else {
                ret = 1;
            }
        } else {
            ret = 1;
        }
        break;
    case 1:
        if (Hu3DMotionEndCheck(node->f00) != 0) {
            ret = fn_1_C0D0(node);
        }
        break;
    }
    return ret;
}

s32 fn_1_C0D0(M515_NODE2 *node)
{
    if (node->f00 >= 0) {
        Hu3DModelAttrReset(node->f00, HU3D_ATTR_DISPOFF);
        node->f02 = Hu3DMotionIDGet(node->f00);
        if (node->f02 >= 0) {
            Hu3DMotionTimeSet(node->f00, 0.0f);
            Hu3DModelAttrSet(node->f00, 0x40000002);
        }
    }

    return 1;
}

s32 fn_1_C150(M515_NODE2 *node)
{
    s32 ret = 0;

    switch (node->f04) {
    case 0:
        if (node->f00 >= 0) {
            node->f02 = Hu3DMotionIDGet(node->f00);
            if (node->f02 >= 0) {
                Hu3DMotionTimeSet(node->f00, 0.0f);
                Hu3DModelAttrReset(node->f00, 0x40000004);
                Hu3DModelAttrReset(node->f00, 0x40000002);
                node->f04++;
            } else {
                Hu3DModelAttrSet(node->f00, HU3D_ATTR_DISPOFF);
                ret = 1;
            }
        } else {
            ret = 1;
        }
        break;
    case 1:
        if (Hu3DMotionEndCheck(node->f00) != 0) {
            node->f04++;
        }
        break;
    case 2:
        Hu3DModelAttrSet(node->f00, HU3D_ATTR_DISPOFF);
        ret = fn_1_C26C(node);
        break;
    }
    return ret;
}

s32 fn_1_C26C(M515_NODE2 *node)
{
    if (node->f00 >= 0) {
        Hu3DModelAttrSet(node->f00, HU3D_ATTR_DISPOFF);
    }

    return 1;
}

void fn_1_C83C(void)
{
    Hu3DShadowMultiCreate(15.0f, 20.0f, 12000.0f,
        lbl_1_rodata_10[0] | lbl_1_rodata_10[1] | lbl_1_rodata_10[2] | lbl_1_rodata_10[3]);
    Hu3DShadowMultiTPLvlSet(0.3f,
        lbl_1_rodata_10[0] | lbl_1_rodata_10[1] | lbl_1_rodata_10[2] | lbl_1_rodata_10[3]);
    Hu3DShadowMultiSizeSet(24,
        lbl_1_rodata_10[0] | lbl_1_rodata_10[1] | lbl_1_rodata_10[2] | lbl_1_rodata_10[3]);
}

void fn_1_C958(s32 idx, HuVecF *pos, s32 mode)
{
    HuVecF up = { 0.0f, 1.0f, 0.0f };
    HuVecF tgt;
    HuVecF eye;
    Vec side;
    Vec upv;
    Vec dir;

    tgt = *pos;
    if (mode != 0) {
        eye.x = pos->x;
        eye.y = 800.0f + pos->y;
        eye.z = pos->z - 2400.0f;
    } else {
        eye.x = pos->x;
        eye.y = 1200.0f + pos->y;
        eye.z = pos->z;
    }
    PSVECSubtract((Vec *)&tgt, (Vec *)&eye, &dir);
    PSVECNormalize(&dir, &dir);
    if (dir.x == 0.0f && dir.z == 0.0f) {
        dir.z = 0.0001f;
    }
    PSVECCrossProduct((Vec *)&up, &dir, &side);
    PSVECNormalize(&side, &side);
    PSVECCrossProduct(&dir, &side, &upv);
    PSVECNormalize(&upv, &upv);
    Hu3DShadowMultiPosSet(&eye, (HuVecF *)&upv, &tgt, lbl_1_rodata_10[idx]);
}

void fn_1_CAF0(M515_SUB *sub, s32 i)
{
    Hu3DModelAttrReset(sub->idb6[i][0], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(sub->id26[i], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(sub->ida6[i], HU3D_ATTR_DISPOFF);
}

void fn_1_CB60(M515_SUB *sub, s32 i)
{
    Hu3DModelAttrReset(sub->idb6[i][0], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(sub->id16[i], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(sub->id1e[i], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(sub->id96[i][0], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(sub->id76[i].a, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(sub->id76[i].b, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(sub->ida6[i], HU3D_ATTR_DISPOFF);
}

void fn_1_CC20(s16 layerNo)
{
    M515_SUB *sub;
    M515_PLY *ply;
    s32 i;
    s32 j;
    s32 k;

    sub = M515_SUBOF(lbl_1_bss_4);
    if (omUPauseFlag + omPauseChk() != 0) {
        if (sub->v1d8 == 0) {
            sub->v1d8 = 1;
            for (i = 0; i < 4; i++) {
                ply = sub->ply[i];
                for (j = 0; j < 3; j++) {
                    if ((Hu3DModelAttrGet(sub->idb6[i][j]) & 1) == 0) {
                        sub->v1dc[i][j] = 1;
                        for (k = 0; k < 25; k++) {
                            if (ply->mdlIds[j][k] >= 0) {
                                Hu3DModelAttrSet(ply->mdlIds[j][k], HU3D_ATTR_DISPOFF);
                            }
                        }
                    }
                }
                for (j = 0; j < ply->v5e8; j++) {
                    Hu3DModelAttrSet(ply->item[j].mdl, HU3D_ATTR_DISPOFF);
                    Hu3DModelAttrSet(ply->item[j].mdl2, HU3D_ATTR_DISPOFF);
                }
            }
        }
    } else if (sub->v1d8 != 0) {
        sub->v1d8 = 0;
        for (i = 0; i < 4; i++) {
            ply = sub->ply[i];
            for (j = 0; j < 3; j++) {
                if (sub->v1dc[i][j] != 0) {
                    sub->v1dc[i][j] = 0;
                    for (k = 0; k < 25; k++) {
                        if (ply->mdlIds[j][k] >= 0) {
                            Hu3DModelAttrReset(ply->mdlIds[j][k], HU3D_ATTR_DISPOFF);
                        }
                    }
                }
            }
            for (j = 0; j < ply->v5e8; j++) {
                Hu3DModelAttrReset(ply->item[j].mdl, HU3D_ATTR_DISPOFF);
                Hu3DModelAttrReset(ply->item[j].mdl2, HU3D_ATTR_DISPOFF);
            }
        }
    }
}

void fn_1_E548(M515_PLY *ply)
{
    s32 i;
    f32 t;

    for (i = 0; i < ply->v5e8; i++) {
        ply->item[i].f28 += 2.0f;
        if (ply->item[i].f28 > 360.0f) {
            ply->item[i].f28 -= 360.0f;
        }
        t = 0.05f * HuSin(ply->item[i].f28);
        Hu3DModelScaleSet(ply->item[i].mdl, 1.1f, 1.05f + t, 1.1f);
        Hu3DModelScaleSet(ply->item[i].mdl2, 1.1f - t, 1.1f, 1.1f - t);
    }
}
