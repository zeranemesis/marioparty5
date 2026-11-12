#include "types.h"
#include "game/hu3d.h"
#include "game/charman.h"
#include "game/esprite.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/frand.h"
#include "game/wipe.h"
#include "game/mg/seqman.h"
#include "humath.h"
#include "math.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* stagedata.c */
typedef struct M577StageVec_s {
    f32 x;
    f32 y;
} M577StageVec;

typedef struct M577StageAnim_s {
    M577StageVec *table; /* 0x00 */
    s32 count;           /* 0x04 */
    f32 value;           /* 0x08 */
} M577StageAnim;

s16 *fn_1_B430(s32 arg0, s32 arg1);
s16 *fn_1_B460(s32 arg0, s32 arg1, s32 arg2);
void fn_1_B498(M577StageAnim *arg0);
s32 fn_1_B4D0(M577StageVec *arg0, M577StageVec *arg1);

/* effect.c */
typedef struct M577Effect_s {
    HU3DPARTICLE *hook;  /* 0x00 */
    ANIMDATA *anim;      /* 0x04 */
    HuVecF *pos;         /* 0x08 */
    HU3DMODELID modelId; /* 0x0C */
} M577Effect;

void fn_1_B5D4(M577Effect *arg0, u16 arg1, HuVecF *arg2);
void fn_1_B71C(M577Effect *arg0);
void fn_1_B720(M577Effect *arg0, s32 arg1);

typedef struct M577Player_s M577Player;
typedef struct M577Sled_s M577Sled;
typedef struct M577Team_s M577Team;
typedef struct M577List_s M577List;

typedef struct M577Node_s {
    struct M577Node_s *prev; /* 0x00 */
    struct M577Node_s *next; /* 0x04 */
    M577List *list;          /* 0x08 */
    s32 (*func)(struct M577Node_s *node); /* 0x0C */
    void *arg;               /* 0x10 */
} M577Node;

struct M577List_s {
    M577Node *used; /* 0x00 */
    M577Node *free; /* 0x04 */
};

typedef struct M577Line_s {
    f32 slope;  /* 0x00 */
    f32 norm;   /* 0x04 */
    f32 offset; /* 0x08 */
    s16 vert;   /* 0x0C */
} M577Line;

typedef struct M577Cam_s {
    HuVecF target; /* 0x00 */
    HuVecF rot;    /* 0x0C: x/y angles, z distance */
    u8 pad18[0x1C];
    s16 bit;       /* 0x34 */
} M577Cam;

typedef struct M577Split_s {
    f32 x;     /* 0x00 */
    f32 unk4;  /* 0x04 */
    u8 pad8[2];
    s16 unkA;  /* 0x0A */
} M577Split;

typedef struct M577Stage_s {
    f32 rot;  /* 0x00 */
    s16 m04;
    s16 m06;
    s16 m08;
    s16 m0A;
    s16 m0C;
    s16 m0E;
    s16 m10;
    s16 m12;
    s16 m14;
    s16 m16;
    s16 m18;
    s16 m1A;
    s16 m1C;
    s16 m1E;
    s16 m20;
    s16 mark[11]; /* 0x22 */
    s16 m38;
    s16 m3A;
    s16 markDone[11]; /* 0x3C */
    s16 done52;
    s16 done54;
    s16 m56;
    s16 sfx58;
    s16 sfx5A;
    s16 sfx5C;
    s16 sfx5E;
    s16 side; /* 0x60 */
    u8 pad62[2];
} M577Stage;

typedef struct M577Pole_s {
    HuVecF pos;    /* 0x00 */
    HuVecF *rope;  /* 0x0C */
    M577Effect *eff; /* 0x10 */
    f32 prevX;     /* 0x14 */
    f32 velX;      /* 0x18 */
    f32 unk1C;     /* 0x1C */
    f32 unk20;     /* 0x20 */
    f32 rot;       /* 0x24 */
    s16 m28;       /* 0x28 */
    s16 m2A;       /* 0x2A */
    s16 m2C;       /* 0x2C */
    u8 pad2E[2];
} M577Pole;

typedef struct M577Rope_s {
    HuVecF pos[32];  /* 0x000 */
    HuVecF vel[32];  /* 0x180 */
    HuVecF prev[32]; /* 0x300 */
    HuVecF a;        /* 0x480 */
    HuVecF b;        /* 0x48C */
    f32 f498;        /* 0x498 */
    f32 f49C;        /* 0x49C */
    f32 f4A0;        /* 0x4A0 */
    f32 f4A4;        /* 0x4A4 */
    s32 count;       /* 0x4A8 */
    s16 mdl[32];     /* 0x4AC */
} M577Rope;

struct M577Sled_s {
    M577Player *player; /* 0x00 */
    M577Team *team;     /* 0x04 */
    M577Line line;      /* 0x08 */
    M577StageVec pt;    /* 0x18 */
    HuVecF pos;         /* 0x20 */
    HuVecF rot;         /* 0x2C */
    HuVecF *ropeBack;   /* 0x38 */
    HuVecF *ropeFront;  /* 0x3C */
    M577Pole *pole;     /* 0x40 */
    M577Effect *eff;    /* 0x44 */
    f32 velX;           /* 0x48 */
    f32 unk4C;          /* 0x4C */
    f32 unk50;          /* 0x50 */
    f32 baseY;          /* 0x54 */
    f32 roll;           /* 0x58 */
    s16 mSled;          /* 0x5C */
    s16 mWheel;         /* 0x5E */
    s16 mShadow;        /* 0x60 */
    s16 unk62;          /* 0x62 */
    s16 unk64;          /* 0x64 */
    u8 pad66[2];
};

struct M577Player_s {
    HuVecF pos;        /* 0x00 */
    HuVecF rot;        /* 0x0C */
    OMOBJ *obj;        /* 0x18 */
    M577Team *team;    /* 0x1C */
    M577Sled *sled;    /* 0x20 */
    void (*func)(M577Player *player); /* 0x24 */
    f32 motMax;        /* 0x28 */
    f32 motRate;       /* 0x2C */
    s16 *aiTbl;        /* 0x30 */
    s16 timer;         /* 0x34 */
    s16 mdlId;         /* 0x36 */
    s16 playerNo;      /* 0x38 */
    s16 charNo;        /* 0x3A */
    s16 grp;           /* 0x3C */
    s16 padNo;         /* 0x3E */
    s16 unk40;         /* 0x40 */
    s16 aiIdx;         /* 0x42: PC only, index into aiTbl */
    s16 type;          /* 0x44 */
    s16 dif;           /* 0x46 */
    s16 teamIdx;       /* 0x48 */
    s16 state;         /* 0x4A */
    s16 sub;           /* 0x4C */
    s16 subTimer;      /* 0x4E */
    s16 stick;         /* 0x50 */
    u8 pad52[2];
};

struct M577Team_s {
    M577Player *player[3]; /* 0x00 */
    M577Sled *sled[3];     /* 0x0C */
    M577Cam *cam;          /* 0x18 */
    M577Stage *stage;      /* 0x1C */
    M577Rope *rope[2];     /* 0x20 */
    M577StageAnim *course; /* 0x28 */
    void (*func)(M577Team *team); /* 0x2C */
    f32 speed;             /* 0x30 */
    f32 speedMax;          /* 0x34 */
    f32 unk38;             /* 0x38 */
    f32 dist;              /* 0x3C */
    s16 side;              /* 0x40 */
    s16 count;             /* 0x42 */
    s16 state;             /* 0x44 */
    s16 sub;               /* 0x46 */
    s16 timer;             /* 0x48 */
    s16 done;              /* 0x4A */
};

typedef struct M577Work_s {
    M577Player *player[4];  /* 0x000 */
    M577Team team[2];       /* 0x010 */
    M577List list;          /* 0x0A8 */
    M577Node pool[0x6C];    /* 0x0B0 */
    M577Cam cam[2];         /* 0x920 */
    M577Stage stage[2];     /* 0x990 */
    M577Split split;        /* 0xA58 */
    M577Sled sled[4];       /* 0xA64 */
    M577Rope rope[3];       /* 0xC04 */
    M577StageAnim course;   /* 0x1AC8 */
    M577Pole pole;          /* 0x1AD4 */
    M577Effect eff[5];      /* 0x1B04 */
    f32 ringAngle;          /* 0x1B54 */
    s16 esp[3];             /* 0x1B58 */
    s16 unk1B5E;            /* 0x1B5E: winner team, -1 draw */
    s16 state;              /* 0x1B60 */
    s16 timer;              /* 0x1B62 */
    s16 sub;                /* 0x1B64 */
    s16 active;             /* 0x1B66 */
    s16 mdlCache[0x19];     /* 0x1B68 */
    s16 mtnCache[4];        /* 0x1B9A */
} M577Work;

#define M577_MODEL(dst, idx)                                                                       \
    if (w->mdlCache[idx] < 0) {                                                                    \
        dst = w->mdlCache[idx] = Hu3DModelCreate(HuDataSelHeapReadNum(modelDataTbl[idx], HU_MEMNUM_OVL, HEAP_MODEL)); \
    } else {                                                                                       \
        dst = Hu3DModelLink(w->mdlCache[idx]);                                                     \
    }

#define M577_MOTION(dst, mdl, idx)                                                                            \
    if (w->mtnCache[idx] < 0) {                                                                               \
        dst = w->mtnCache[idx] = Hu3DJointMotion(mdl, HuDataSelHeapReadNum(jointMotTbl[idx], HU_MEMNUM_OVL, HEAP_MODEL)); \
    } else {                                                                                                  \
        dst = w->mtnCache[idx];                                                                               \
    }

#define M577_STAGE_MODEL(mdl, attr, rotX)  \
    Hu3DModelAttrSet(mdl, 1);              \
    Hu3DModelAttrSet(mdl, attr);           \
    Hu3DModelLayerSet(mdl, 0);             \
    Hu3DModelPosSet(mdl, 0.0f, -16000.0f, 0.0f); \
    Hu3DModelRotSet(mdl, rotX, 0.0f, 0.0f); \
    Hu3DModelCameraSet(mdl, bit)

void ObjectSetup(void);
void fn_1_1F4(s16 mode, s16 frameNo);
void fn_1_314(s16 mode, s16 frameNo);
void fn_1_798(s16 mode, s16 frameNo);
void fn_1_7EC(s16 mode, s16 frameNo);
void fn_1_B0C(s16 mode, s16 frameNo);
void fn_1_BF8(s16 mode, s16 frameNo);
void fn_1_1600(s16 mode, s16 frameNo);
void fn_1_1790(s16 mode, s16 frameNo);
void fn_1_17CC(s16 mode, s16 frameNo);
void fn_1_1870(OMOBJ *obj);
void fn_1_1B40(OMOBJ *obj);
void fn_1_1BC8(OMOBJ *obj);
void fn_1_1C4C(OMOBJ *obj);
void fn_1_1CF4(M577Work *w);
void fn_1_1D50(void);
void fn_1_1DD8(M577Work *w);
void fn_1_1FEC(M577Split *split);
void fn_1_2044(M577Split *split);
void fn_1_225C(M577Split *split);
void fn_1_2260(void);
void fn_1_2364(M577Cam *cam, s16 bit);
void fn_1_23CC(M577Cam *cam);
void fn_1_263C(M577List *list, M577Node *pool, s32 count);
M577Node *fn_1_26C0(M577List *list, s32 (*func)(M577Node *), void *arg);
void fn_1_2714(M577Node *node);
void fn_1_278C(M577List *list);
void fn_1_286C(M577Work *w);
void fn_1_2B60(M577Rope *rope, s32 count);
void fn_1_2E08(M577Rope *rope);
void fn_1_3494(M577Team *team, s16 side);
void fn_1_3AB8(M577Work *w);
void fn_1_3E10(M577Sled *sled, M577Player *player, s32 side);
void fn_1_45AC(M577Stage *stage, u16 camBit);
void fn_1_579C(M577Stage *stage);
void fn_1_5F7C(M577Player *player);
void fn_1_61A4(M577Player *player);
void fn_1_64BC(M577Player *player);
void fn_1_674C(M577Player *player);
void fn_1_6ACC(M577Player *player);
void fn_1_6D0C(M577Player *player);
void fn_1_703C(M577Team *team);
void fn_1_7040(M577Team *team);
void fn_1_72A8(M577Team *team);
void fn_1_72D0(M577Team *team);
void fn_1_7D24(M577Team *team);
void fn_1_85F4(M577Team *team);
void fn_1_8BB8(M577Team *team);
void fn_1_9380(M577Team *team);
void fn_1_9C08(M577Team *team);
void fn_1_A36C(M577Team *team);
void fn_1_A5CC(M577Team *team);
void fn_1_A754(M577Line *line, f32 *p1, f32 *p2);
f32 fn_1_A8D4(M577Line *line, M577StageVec *pt);
f32 fn_1_A9D0(M577Line *line, f32 y);
void fn_1_AA4C(M577Work *w);
s32 fn_1_AB34(M577Node *node);
s32 fn_1_AF68(M577Node *node);
void fn_1_B134(M577Work *w);
void fn_1_B26C(M577Work *w);
s32 fn_1_B318(M577Node *node);
s32 fn_1_B3CC(M577Node *node);

#ifdef __MWERKS__
#define M577_RODATA __declspec(section ".rodata")
#else
#define M577_RODATA
#endif

static MGSEQ_PARAM seqParam = {
    0x12C,
    0,
    fn_1_1F4,
    fn_1_314,
    fn_1_798,
    fn_1_7EC,
    fn_1_B0C,
    fn_1_BF8,
    fn_1_1600,
    fn_1_1790,
    fn_1_17CC,
};

static u32 charMotTbl[13][9] = {
    { 0x8C0000, 0x870076, 0x870075, 0x8C000A, 0x8C0009, 0x870074, 0x8C0030, 0x8C0031, 0x8C0001 },
    { 0x330000, 0x2E0076, 0x2E0075, 0x33000A, 0x330009, 0x2E0074, 0x330030, 0x330031, 0x330001 },
    { 0xB70000, 0xB20076, 0xB20075, 0xB7000A, 0xB70009, 0xB20074, 0xB70030, 0xB70031, 0xB70001 },
    { 0xED0000, 0xE80076, 0xE80075, 0xED000A, 0xED0009, 0xE80074, 0xED0030, 0xED0031, 0xED0001 },
    { 0xE60000, 0xE10076, 0xE10075, 0xE6000A, 0xE60009, 0xE10074, 0xE60030, 0xE60031, 0xE60001 },
    { 0x170000, 0x120076, 0x120075, 0x17000A, 0x170009, 0x120074, 0x170030, 0x170031, 0x170001 },
    { 0xE00000, 0xDB0076, 0xDB0075, 0xE0000A, 0xE00009, 0xDB0074, 0xE00030, 0xE00031, 0xE00001 },
    { 0x2D0000, 0x280076, 0x280075, 0x2D000A, 0x2D0009, 0x280074, 0x2D0030, 0x2D0031, 0x2D0001 },
    { 0xC90000, 0xC40076, 0xC40075, 0xC9000A, 0xC90009, 0xC40074, 0xC90030, 0xC90031, 0xC90001 },
    { 0xB00000, 0x9C0076, 0x9C0075, 0xB0000A, 0xB00009, 0x9C0074, 0xB00030, 0xB00031, 0xB00001 },
    { 0xB00000, 0x9C0076, 0x9C0075, 0xB0000A, 0xB00009, 0x9C0074, 0xB00030, 0xB00031, 0xB00001 },
    { 0xB00000, 0x9C0076, 0x9C0075, 0xB0000A, 0xB00009, 0x9C0074, 0xB00030, 0xB00031, 0xB00001 },
    { 0xB00000, 0x9C0076, 0x9C0075, 0xB0000A, 0xB00009, 0x9C0074, 0xB00030, 0xB00031, 0xB00001 },
};

static u32 modelDataTbl[0x19] = {
    0x840000, 0x840001, 0x840002, 0x840003, 0x840004, 0x840005, 0x840006, 0x840007, 0x840008,
    0x840009, 0x84000A, 0x84000B, 0x84000C, 0x840011, 0x84000D, 0x84000E, 0x84000F, 0x840010,
    0x840012, 0x840013, 0x840014, 0x840015, 0x840016, 0x840017, 0x840018,
};

static u32 jointMotTbl[4] = { 0x840019, 0x84001A, 0x84001B, 0x84001C };

static struct {
    HuVecF target;
    HuVecF rot;
} camInit[2] = {
    { { 0.0f, 0.0f, -974.0f }, { 90.0f, 21.0f, 7840.0f } },
    { { 0.0f, 0.0f, 1118.0f }, { 270.0f, 20.0f, 5010.0f } },
};

static s32 seActive[2] = { 0, 0 };

static s32 charSeTbl[13] = { 0x1FD, 0x1E9, 0x261, 0x2B1, 0x289, 0x1C1, 0x29D, 0x1D5, 0x275, 0x239, 0x24D, 0x225, 0x211 };

static OMOBJMAN *objman;
static OMOBJ *workObj;
static s32 bgmStream;
static s32 seHandle[2];

static const u16 camBit[4] = { 1, 2, 4, 8 };

s32 _prolog(void)
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
    M577Work *w;
    OMOBJ *obj;
    s32 i;

    objman = omInitObjMan(0xA, 0x2000);
    omGameSysInit(objman);
    obj = omAddObjEx(objman, 0x65, 0, 0, -1, fn_1_1BC8);
    workObj = obj;
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M577Work), HU_MEMNUM_OVL);
    w = obj->data;
    w->active = 0;
    fn_1_1DD8(w);
    fn_1_1D50();
    for (i = 0; i < 4; i++) {
        obj = omAddObjEx(objman, 0x64, 1, 9, 0, fn_1_1870);
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M577Player), HU_MEMNUM_OVL);
        obj->work[0] = i;
        w->player[i] = obj->data;
    }
    fn_1_1CF4(w);
    Hu3DBGColorSet(0, 0, 0);
    Hu3DNoSyncSet(1);
    MgSeqCreate(&seqParam);
}

void fn_1_1F4(s16 mode, s16 frameNo)
{
    M577Work *w = workObj->data;

    w->active = 0;
    w->state = 0;
    fn_1_286C(w);
    fn_1_3AB8(w);
    fn_1_1FEC(&w->split);
    w->cam[0].rot.x = camInit[0].rot.x;
    w->cam[0].rot.y = camInit[0].rot.y;
    w->cam[0].rot.z = camInit[0].rot.z;
    w->cam[0].target.x = camInit[0].target.x;
    w->cam[0].target.y = camInit[0].target.y;
    w->cam[0].target.z = camInit[0].target.z;
    w->cam[1].rot.x = camInit[0].rot.x;
    w->cam[1].rot.y = camInit[0].rot.y;
    w->cam[1].rot.z = camInit[0].rot.z;
    w->cam[1].target.x = camInit[0].target.x;
    w->cam[1].target.y = camInit[0].target.y;
    w->cam[1].target.z = camInit[0].target.z;
    MgSeqModeNext();
}

void fn_1_314(s16 mode, s16 frameNo)
{
    M577Work *w = workObj->data;
    M577Sled *sled;
    f32 rotX;
    f32 y;
    f32 z;
    f32 t;

    switch (w->state) {
        case 0:
            MgSeqModeChangeOff();
            w->sub = 0;
            w->timer = 0;
            sled = w->team[0].player[0]->sled;
            sled->pos.y -= -30.0f;
            sled->pole->pos.y -= 30.0f;
            Hu3DModelPosSetV(sled->mSled, &sled->pos);
            Hu3DModelPosSetV(sled->mShadow, &sled->pos);
            Hu3DModelPosSetV(sled->pole->m28, &sled->pole->pos);
            Hu3DModelPosSetV(sled->pole->m2C, &sled->pole->pos);
            HuAudFXPlay(0x940);
            w->state++;
        case 1:
            w->timer++;
            if (w->timer == 0x96) {
                w->state++;
            }
            t = (f32)w->timer / 150.0f;
            rotX = -20.0 * (1.0 - sin(M_PI * (90.0f * t) / 180.0));
            z = 16000.0 * -sin(M_PI * rotX / 180.0);
            y = 16000.0 * cos(M_PI * rotX / 180.0) - 16000.0;
            w->cam[0].target.y = y + camInit[0].target.y;
            w->cam[0].target.z = z + camInit[0].target.z;
            w->cam[0].rot.y = rotX + camInit[0].rot.y;
            if (w->timer > 0x3C && w->timer <= 0x5A) {
                sled = w->team[0].player[0]->sled;
                sled->pos.y += -1.0f;
                sled->pole->pos.y += 1.0f;
                Hu3DModelPosSetV(sled->mSled, &sled->pos);
                Hu3DModelPosSetV(sled->mShadow, &sled->pos);
                Hu3DModelPosSetV(sled->pole->m28, &sled->pole->pos);
                Hu3DModelPosSetV(sled->pole->m2C, &sled->pole->pos);
            }
            break;
        case 2:
            w->timer = 0;
            w->state++;
        case 3:
            w->timer++;
            if (w->timer == 0x1E) {
                w->state++;
            }
            t = (f32)w->timer / 30.0f;
            w->split.x = 640.0f * (1.0f - t) + 320.0f * t;
            break;
        case 4:
            espAttrReset(w->esp[0], 4);
            espAttrReset(w->esp[1], 4);
            espAttrReset(w->esp[2], 4);
            w->timer = 0;
            w->state++;
        case 5:
            w->timer++;
            if (w->timer == 0x1E) {
                w->state++;
            }
            break;
        case 6:
            bgmStream = HuAudBGMPlay(0x34);
            Hu3DModelAttrSet(w->stage[0].m0C, 1);
            Hu3DModelAttrSet(w->stage[1].m0C, 1);
            MgSeqModeNext();
            break;
    }
    fn_1_2044(&w->split);
}


void fn_1_798(s16 mode, s16 frameNo)
{
    M577Work *w = workObj->data;

    switch (w->sub) {
        case 0:
            w->state = 0;
            w->sub++;
        case 1:
            break;
    }
}

void fn_1_7EC(s16 mode, s16 frameNo)
{
    M577Work *w = workObj->data;
    M577Team *t0;
    M577Team *t1;
    f32 prog0;
    f32 prog1;

    switch (w->state) {
        case 0:
            Hu3DModelAttrReset(w->stage[0].m10, 0x40000002);
            Hu3DModelAttrReset(w->stage[1].m10, 0x40000002);
            HuAudFXPlayPan(0x941, 0x30);
            fn_1_26C0(&w->list, fn_1_B3CC, &w->stage[1].sfx58);
            w->stage[1].sfx58 = 0;
            w->team[0].func = fn_1_72D0;
            w->team[1].func = fn_1_7D24;
            w->sub = 0;
            w->active = 1;
            w->state++;
        case 1:
            t0 = &w->team[0];
            prog0 = t0->dist / t0->course->value;
            t1 = &w->team[1];
            prog1 = t1->dist / t1->course->value;
            if (prog0 >= 1.0f || prog1 >= 1.0f) {
                if (prog0 >= 1.0f && prog1 >= 1.0f) {
                    Hu3DModelAttrReset(w->stage[0].m12, 0x40000002);
                    Hu3DModelAttrReset(w->stage[1].m12, 0x40000002);
                    HuAudFXPlayPan(0x941, 0x30);
                    fn_1_26C0(&w->list, fn_1_B3CC, &w->stage[1].sfx5A);
                    w->stage[1].sfx5A = 0;
                    if (prog0 > prog1) {
                        w->unk1B5E = 0;
                        w->team[w->unk1B5E ^ 1].done = 1;
                    } else if (prog1 < prog0) {
                        w->unk1B5E = 1;
                        w->team[w->unk1B5E ^ 1].done = 1;
                    } else {
                        w->unk1B5E = -1;
                        w->team[0].func = fn_1_9380;
                        w->team[1].func = fn_1_9C08;
                        w->team[0].done = 1;
                        w->team[1].done = 1;
                    }
                } else {
                    if (prog0 >= 1.0f) {
                        w->unk1B5E = 0;
                        Hu3DModelAttrReset(w->stage[0].m12, 0x40000002);
                        Hu3DModelAttrReset(w->stage[0].m56, 0x40000002);
                        Hu3DModelAttrReset(w->stage[0].m56, 1);
                        HuAudFXPlayPan(0x941, 0x30);
                    } else {
                        w->unk1B5E = 1;
                        Hu3DModelAttrReset(w->stage[1].m12, 0x40000002);
                        Hu3DModelAttrReset(w->stage[1].m56, 0x40000002);
                        Hu3DModelAttrReset(w->stage[1].m56, 1);
                        fn_1_26C0(&w->list, fn_1_B3CC, &w->stage[1].sfx5A);
                        w->stage[1].sfx5A = 0;
                    }
                    w->team[w->unk1B5E ^ 1].done = 1;
                }
                w->active = 0;
                MgSeqModeNext();
            }
            return;
    }
}

void fn_1_B0C(s16 mode, s16 frameNo)
{
    M577Work *w = workObj->data;

    switch (w->sub) {
        case 0:
            w->active = 0;
            if (w->unk1B5E < 0) {
                MgSeqWinnerSet(-1, -1, -1, -1);
            } else if (w->unk1B5E == 0) {
                MgSeqWinnerSet(w->team[0].player[0]->charNo, -1, -1, -1);
            } else {
                MgSeqWinnerSet(w->team[1].player[0]->charNo, w->team[1].player[1]->charNo, w->team[1].player[2]->charNo, -1);
            }
            HuAudSStreamFadeOut(bgmStream, 0x3E8);
            w->state = 0;
            w->sub++;
        case 1:
            break;
    }
}

void fn_1_BF8(s16 mode, s16 frameNo)
{
    M577Work *w = workObj->data;
    M577Player *p;
    M577Team *team;
    f32 t;
    s32 i;

    switch (w->state) {
        case 0:
            MgSeqModeChangeOff();
            w->sub = 0;
            if (w->unk1B5E < 0) {
                w->state = 1;
                w->team[0].done = 1;
                w->team[1].done = 1;
                return;
            }
            w->state = 4;
            return;
        case 1:
            if (w->team[0].func == fn_1_703C && w->team[1].func == fn_1_703C) {
                for (i = 0; i < 4; i++) {
                    p = w->player[i];
                    p->timer = 1;
                    CharMotionShiftSet(p->charNo, p->obj->mtnId[8], 0.0f, 5.0f, 0x40000001);
                }
                w->timer = 0;
                w->state++;
                return;
            }
            break;
        case 2:
            w->timer++;
            if (w->timer == 0x3C) {
                w->state++;
            }
            t = (f32)w->timer / 60.0f;
            for (i = 0; i < 4; i++) {
                p = w->player[i];
                p->rot.y = 180.0f * t;
                Hu3DModelRotSetV(p->mdlId, &p->rot);
                omSetRot(p->obj, p->rot.x, p->rot.y, p->rot.z);
            }
            return;
        case 3:
            MgSeqModeNext();
            return;
        case 4:
            if (w->team[w->unk1B5E ^ 1].func == fn_1_703C) {
                if (w->unk1B5E == 0) {
                    if (w->team[0].func == fn_1_9380) {
                        w->state++;
                        return;
                    }
                } else if (w->team[1].func == fn_1_9C08) {
                    w->state++;
                    return;
                }
            }
            break;
        case 5:
            for (i = 0; i < 4; i++) {
                w->player[i]->timer = 1;
            }
            WipeColorSet(0xFF, 0xFF, 0xFF);
            WipeCreate(2, 0, 0x3C);
            w->sub = 0;
            w->state++;
        case 6:
            if (WipeCheckEnd() == 0) {
                w->state++;
                return;
            }
            break;
        case 7:
            team = &w->team[w->unk1B5E ^ 1];
            for (i = 0; i < team->count; i++) {
                Hu3DModelAttrSet(team->sled[i]->mSled, 1);
                Hu3DModelAttrSet(team->sled[i]->mShadow, 1);
            }
            if (w->unk1B5E == 1) {
                Hu3DModelAttrSet(team->sled[0]->pole->m28, 1);
                Hu3DModelAttrSet(team->sled[0]->pole->m2C, 1);
            }
            Hu3DModelAttrSet(team->stage->m04, 1);
            Hu3DModelAttrSet(team->stage->m06, 1);
            Hu3DModelAttrSet(team->stage->m08, 1);
            Hu3DModelAttrSet(team->stage->m0A, 1);
            Hu3DModelAttrSet(team->stage->m0C, 1);
            Hu3DModelAttrSet(team->stage->m0E, 1);
            Hu3DModelAttrSet(team->stage->m10, 1);
            Hu3DModelAttrSet(team->stage->m12, 1);
            Hu3DModelAttrSet(team->stage->m14, 1);
            Hu3DModelAttrSet(team->stage->m16, 1);
            Hu3DModelAttrSet(team->stage->m18, 1);
            Hu3DModelAttrSet(team->stage->m1A, 1);
            Hu3DModelAttrSet(team->stage->m1C, 1);
            Hu3DModelAttrSet(team->stage->m1E, 1);
            Hu3DModelAttrSet(team->stage->m20, 1);
            Hu3DModelAttrSet(team->stage->m38, 1);
            Hu3DModelAttrSet(team->stage->m3A, 1);
            for (i = 0; i < 0xB; i++) {
                Hu3DModelAttrSet(team->stage->mark[i], 1);
            }
            team = &w->team[w->unk1B5E];
            Hu3DModelAttrSet(team->stage->m04, 1);
            Hu3DModelAttrSet(team->stage->m06, 1);
            Hu3DModelAttrSet(team->stage->m08, 1);
            Hu3DModelAttrSet(team->stage->m0A, 1);
            Hu3DModelAttrSet(team->stage->m0C, 1);
            Hu3DModelAttrSet(team->stage->m0E, 1);
            Hu3DModelAttrSet(team->stage->m10, 1);
            Hu3DModelAttrSet(team->stage->m12, 1);
            Hu3DModelAttrSet(team->stage->m14, 1);
            Hu3DModelAttrSet(team->stage->m16, 1);
            Hu3DModelAttrSet(team->stage->m18, 1);
            Hu3DModelAttrSet(team->stage->m1A, 1);
            Hu3DModelAttrSet(team->stage->m1C, 1);
            Hu3DModelAttrSet(team->stage->m1E, 1);
            Hu3DModelAttrSet(team->stage->m20, 1);
            Hu3DModelAttrSet(team->stage->m38, 1);
            Hu3DModelAttrSet(team->stage->m3A, 1);
            for (i = 0; i < 0xB; i++) {
                Hu3DModelAttrSet(team->stage->mark[i], 1);
            }
            espAttrSet(w->esp[0], 4);
            espAttrSet(w->esp[1], 4);
            espAttrSet(w->esp[2], 4);
            Hu3DModelAttrReset(w->mdlCache[13], 1);
            Hu3DModelRotSet(w->mdlCache[11], 0.0f, 180.0f, 0.0f);
            if (w->unk1B5E == 0) {
                w->split.x = 640.0f;
                team->func = fn_1_A36C;
                team->player[0]->sled->pos.x = 0.0f;
                team->player[0]->sled->pole->pos.x = 100.0f;
                Hu3DModelPosSetV(team->sled[0]->mSled, &team->sled[0]->pos);
                Hu3DModelPosSetV(team->sled[0]->pole->m28, &team->sled[0]->pole->pos);
                fn_1_AA4C(w);
                fn_1_2E08(&w->rope[0]);
            } else {
                w->split.x = 0.0f;
                team->func = fn_1_A5CC;
                team->player[0]->sled->pos.x = 250.0f;
                team->player[1]->sled->pos.x = 0.0f;
                team->player[2]->sled->pos.x = -250.0f;
                Hu3DModelPosSetV(team->sled[0]->mSled, &team->sled[0]->pos);
                Hu3DModelPosSetV(team->sled[1]->mSled, &team->sled[1]->pos);
                Hu3DModelPosSetV(team->sled[2]->mSled, &team->sled[2]->pos);
                fn_1_AA4C(w);
                for (i = 0; i < 0x78; i++) {
                    fn_1_2E08(&w->rope[1]);
                    fn_1_2E08(&w->rope[2]);
                }
            }
            Hu3DCameraPerspectiveSet(camBit[w->unk1B5E], 15.0f, 20.0f, 40000.0f, 1.2f);
            Hu3DCameraViewportSet(camBit[w->unk1B5E], 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
            Hu3DCameraScissorSet(camBit[w->unk1B5E], 0, 0, 0x280, 0x1E0);
            fn_1_2044(&w->split);
            team->cam->rot.x = camInit[1].rot.x;
            team->cam->rot.y = camInit[1].rot.y;
            team->cam->rot.z = camInit[1].rot.z;
            team->cam->target.x = camInit[1].target.x;
            team->cam->target.y = camInit[1].target.y;
            team->cam->target.z = camInit[1].target.z;
            for (i = 0; i < team->count; i++) {
                if (team->player[i]->charNo == 3) {
                    team->player[i]->pos.z -= 100.0f;
                    Hu3DModelPosSetV(team->player[i]->mdlId, &team->player[i]->pos);
                    omSetTra(team->player[i]->obj, team->player[i]->pos.x, team->player[i]->pos.y, team->player[i]->pos.z);
                }
            }
            WipeCreate(1, 0, 0x3C);
            w->state++;
        case 8:
            fn_1_B26C(w);
            if (WipeCheckEnd() == 0) {
                MgSeqModeNext();
            }
            break;
    }

}

void fn_1_1600(s16 mode, s16 frameNo)
{
    M577Work *w = workObj->data;
    M577Player *p;
    s16 playerNo;
    s32 i;
    s32 n;

    fn_1_B26C(w);
    switch (w->sub) {
        case 0:
            if (w->unk1B5E < 0) {
                for (i = 0; i < 4; i++) {
                    p = w->player[i];
                    CharMotionShiftSet(p->charNo, p->obj->mtnId[7], 0.0f, 5.0f, 0);
                }
                CharLoseVoicePlay(w->player[0]->charNo, w->player[1]->charNo, w->player[2]->charNo, w->player[3]->charNo);
            } else {
                if (w->unk1B5E == 0) {
                    n = 1;
                } else {
                    n = 3;
                }
                for (i = 0; i < n; i++) {
                    p = w->team[w->unk1B5E].player[i];
                    playerNo = p->playerNo;
                    if (_CheckFlag(0x1000F) == 0) {
                        GwPlayer[playerNo].mgCoinBonus = 0xA;
                    }
                    CharMotionShiftSet(p->charNo, p->obj->mtnId[6], 0.0f, 5.0f, 0);
                }
            }
            w->sub++;
        case 1:
            return;
    }
}

void fn_1_1790(s16 mode, s16 frameNo)
{
    M577Work *w = workObj->data;

    fn_1_B26C(w);
}

void fn_1_17CC(s16 mode, s16 frameNo)
{
    M577Work *w = workObj->data;
    s32 i;

    fn_1_225C(&w->split);
    if (w->esp[0] >= 0) {
        espKill(w->esp[0]);
    }
    if (w->esp[1] >= 0) {
        espKill(w->esp[1]);
    }
    if (w->esp[2] >= 0) {
        espKill(w->esp[2]);
    }
    for (i = 0; i < 5; i++) {
        fn_1_B71C(&w->eff[i]);
    }
}

void fn_1_1870(OMOBJ *obj)
{
    M577Player *p = obj->data;
    M577Work *w = workObj->data;
    s32 i;

    p->playerNo = obj->work[0];
    p->charNo = GwPlayerConf[p->playerNo].charNo;
    p->grp = GwPlayerConf[p->playerNo].grp;
    p->padNo = GwPlayerConf[p->playerNo].padNo;
    p->type = GwPlayerConf[p->playerNo].type;
    p->dif = GwPlayerConf[p->playerNo].dif;
    p->obj = obj;
    w->player[p->playerNo] = p;
    obj->mdlId[0] = CharModelCreate(p->charNo, 8);
    p->mdlId = obj->mdlId[0];
    for (i = 0; i < 9; i++) {
        obj->mtnId[i] = CharMotionCreate(p->charNo, charMotTbl[p->charNo][i]);
    }
    CharMotionSet(p->charNo, obj->mtnId[2]);
    CharModelAttrSet(p->charNo, 0x40000002);
    p->motMax = CharMotionMaxTimeGet(p->charNo);
    p->motRate = 0.5f;
    CharMotionTimeSet(p->charNo, p->motMax * p->motRate);
    CharMotionUpdateSet(p->charNo, 0x8C000A, 0);
    CharMotionUpdateSet(p->charNo, 0x8C0001, 0);
    Hu3DModelLayerSet(obj->mdlId[0], 3);
    CharEffectLayerSet(4);
    CharMotionDataClose(p->charNo);
    p->func = NULL;
    p->sled = NULL;
    p->state = 0;
    p->rot.x = 0.0f;
    p->rot.y = 0.0f;
    p->rot.z = 0.0f;
    p->pos.x = 0.0f;
    p->pos.y = 0.0f;
    p->pos.z = 0.0f;
    Hu3DModelPosSetV(p->mdlId, &p->pos);
    omSetTra(p->obj, p->pos.x, p->pos.y, p->pos.z);
    Hu3DModelRotSetV(p->mdlId, &p->rot);
    omSetRot(p->obj, p->rot.x, p->rot.y, p->rot.z);
    p->stick = 0;
    p->timer = 0;
    obj->objFunc = fn_1_1B40;
}

void fn_1_1B40(OMOBJ *obj)
{
    M577Player *p = obj->data;
    M577Work *w = workObj->data;

    if (w->active != 0 && p->team->state != 2 && p->func != 0) {
        p->func(p);
    }
}

void fn_1_1BC4(void)
{
}

void fn_1_1BC8(OMOBJ *obj)
{
    M577Work *w = workObj->data;

    fn_1_263C(&w->list, w->pool, 0x6C);
    w->unk1B5E = -1;
    fn_1_B498(&w->course);
    w->team[0].func = fn_1_703C;
    w->team[1].func = fn_1_703C;
    obj->objFunc = fn_1_1C4C;
}

void fn_1_1C4C(OMOBJ *obj)
{
    M577Work *w = workObj->data;

    w->team[0].func(&w->team[0]);
    w->team[1].func(&w->team[1]);
    fn_1_579C(&w->stage[0]);
    fn_1_579C(&w->stage[1]);
    fn_1_AA4C(w);
    fn_1_2E08(&w->rope[0]);
    fn_1_2E08(&w->rope[1]);
    fn_1_2E08(&w->rope[2]);
    fn_1_23CC(&w->cam[0]);
    fn_1_23CC(&w->cam[1]);
    fn_1_B134(w);
    fn_1_278C(&w->list);
}

void fn_1_1CF0(void)
{
}

void fn_1_1CF4(M577Work *w)
{
    s32 i;

    for (i = 0; i < 0x19; i++) {
        w->mdlCache[i] = -1;
    }
    for (i = 0; i < 4; i++) {
        w->mtnCache[i] = -1;
    }
}

void fn_1_1D50(void)
{
    HU3DLIGHTID light;

    light = Hu3DGLightCreate(0.0f, 4000.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0x7F, 0x7F, 0x7F);
    Hu3DGLightInfinitytSet(light);
}

void fn_1_1DD8(M577Work *w)
{
    volatile f32 sp8;
    s32 i;

    for (i = 0; i < 2; i++) {
        Hu3DCameraCreate(camBit[i]);
        Hu3DCameraPerspectiveSet(camBit[i], 15.0f, 20.0f, 40000.0f, 0.6f);
        sp8 = 0.0f;
        Hu3DCameraPosSet(camBit[i], 0.0f, 300.0f, 1000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 150.0f, sp8);
        fn_1_2364(&w->cam[i], camBit[i]);
        w->cam[i].rot.x = camInit[i].rot.x;
        w->cam[i].rot.y = camInit[i].rot.y;
        w->cam[i].rot.z = camInit[i].rot.z;
        w->cam[i].target.x = camInit[i].target.x;
        w->cam[i].target.y = camInit[i].target.y;
        w->cam[i].target.z = camInit[i].target.z;
    }
}

void fn_1_1FEC(M577Split *split)
{
    split->x = 640.0f;
    split->unk4 = 0.0f;
    split->unkA = 0;
    fn_1_2044(split);
}

void fn_1_2044(M577Split *split)
{
    f32 aspect;
    s32 i;

    i = 0;
    Hu3DCameraViewportSet(camBit[i], 0.0f, 0.0f, split->x, 480.0f, 0.0f, 1.0f);
    i++;
    Hu3DCameraViewportSet(camBit[i], split->x, 0.0f, 640.0f - split->x, 480.0f, 0.0f, 1.0f);
    i = 0;
    Hu3DCameraScissorSet(camBit[i], 0, 0, (u32)split->x, 0x1E0);
    i++;
    Hu3DCameraScissorSet(camBit[i], (u32)split->x, 0, (u32)(640.0f - split->x), 0x1E0);
    i = 0;
    aspect = 0.9f * split->x / 480.0f;
    if (aspect < 0.0f) {
        aspect = 0.0f;
    }
    Hu3DCameraPerspectiveSet(camBit[i], 15.0f, 20.0f, 40000.0f, aspect);
}

void fn_1_225C(M577Split *split)
{
}

void fn_1_2260(void)
{
    HuVecF up = { 0.0f, 1.0f, 0.0f };
    HuVecF pos = { 0.0f, 8000.0f, 0.0f };
    HuVecF target = { 0.1f, 0.0f, 0.0f };
    s16 bits;

    bits = camBit[0] | camBit[1];
    Hu3DShadowMultiCreate(15.0f, 20.0f, 40000.0f, bits);
    Hu3DShadowMultiPosSet(&pos, &up, &target, bits);
    Hu3DShadowMultiSizeSet(0x30, bits);
    Hu3DShadowMultiColSet(0xFF, 0, 0, bits);
}

void fn_1_2364(M577Cam *cam, s16 bit)
{
    cam->target.x = 0.0f;
    cam->target.y = 0.0f;
    cam->target.z = 0.0f;
    cam->rot.x = 0.0f;
    cam->rot.y = 0.0f;
    cam->rot.z = 3000.0f;
    cam->bit = bit;
}

void fn_1_23CC(M577Cam *cam)
{
    HuVecF pos;
    volatile f32 sp8;
    f64 trig;

    while (cam->rot.x > 360.0f) {
        cam->rot.x -= 360.0f;
    }
    while (cam->rot.x < 0.0f) {
        cam->rot.x += 360.0f;
    }
    while (cam->rot.y > 360.0f) {
        cam->rot.y -= 360.0f;
    }
    while (cam->rot.y < 0.0f) {
        cam->rot.y += 360.0f;
    }
    trig = cos(M_PI * cam->rot.x / 180.0);
    pos.x = cam->target.x + cam->rot.z * cos(M_PI * cam->rot.y / 180.0) * trig;
    pos.y = cam->target.y + cam->rot.z * sin(M_PI * cam->rot.y / 180.0);
    trig = sin(M_PI * cam->rot.x / 180.0);
    pos.z = cam->target.z + cam->rot.z * cos(M_PI * cam->rot.y / 180.0) * trig;
    sp8 = cam->target.z;
    Hu3DCameraPosSet(cam->bit, pos.x, pos.y, pos.z, 0.0f, 1.0f, 0.0f, cam->target.x, cam->target.y, sp8);
}

void fn_1_263C(M577List *list, M577Node *pool, s32 count)
{
    s32 i;

    list->used = NULL;
    list->free = pool;
    for (i = 0; i < count - 1; i++, pool++) {
        pool->prev = NULL;
        pool->next = pool + 1;
        pool->func = NULL;
        pool->arg = NULL;
        pool->list = list;
    }
    pool->prev = NULL;
    pool->next = NULL;
    pool->func = NULL;
    pool->arg = NULL;
    pool->list = list;
}

M577Node *fn_1_26C0(M577List *list, s32 (*func)(M577Node *), void *arg)
{
    M577Node *node;

    node = list->free;
    if (node != 0) {
        list->free = node->next;
        node->func = func;
        node->arg = arg;
        node->next = list->used;
        if (node->next != 0) {
            node->next->prev = node;
        }
        list->used = node;
    }
    return node;
}

void fn_1_2714(M577Node *node)
{
    M577List *list;

    if (node != 0) {
        list = node->list;
        if (node->prev != 0) {
            node->prev->next = node->next;
        } else {
            list->used = node->next;
        }
        if (node->next != 0) {
            node->next->prev = node->prev;
        }
        node->prev = NULL;
        node->next = list->free;
        node->func = NULL;
        list->free = node;
    }
}

void fn_1_278C(M577List *list)
{
    M577Node *node;
    M577Node *next;

    node = list->used;
    while (node != 0) {
        next = node->next;
        if (node->func != 0 && node->func(node) != 0) {
            fn_1_2714(node);
        }
        node = next;
    }
}

void fn_1_286C(M577Work *w)
{
    s16 id;
    s16 mdl;

    w->ringAngle = 800.0f;
    id = espEntry(0x84001D, 0xB, 0);
    w->esp[0] = id;
    espDrawNoSet(id, 0);
    espPosSet(id, 288.0f, 240.0f);
    espAttrSet(id, 1);
    espAttrSet(id, 4);
    id = espEntry(0x84001E, 0xA, 0);
    w->esp[1] = id;
    espDrawNoSet(id, 0);
    espPosSet(id, 288.0f, 240.0f);
    espAttrSet(id, 1);
    espAttrSet(id, 4);
    id = espEntry(0x84001E, 0xA, 0);
    w->esp[2] = id;
    espDrawNoSet(id, 0);
    espPosSet(id, 288.0f, 240.0f);
    espAttrSet(id, 1);
    espAttrSet(id, 4);
    M577_MODEL(mdl, 0xB);
    Hu3DModelAttrSet(mdl, 1);
    Hu3DModelAttrSet(mdl, 0x40000002);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelPosSet(mdl, 0.0f, -16000.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(mdl, 1);
    M577_MODEL(mdl, 0xD);
    Hu3DModelAttrSet(mdl, 1);
    Hu3DModelAttrSet(mdl, 0x40000002);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelPosSet(mdl, 0.0f, -16000.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
}

void fn_1_2B60(M577Rope *rope, s32 count)
{
    M577Work *w = workObj->data;
    s16 mdl;
    s32 i;

    for (i = 0; i < count; i++) {
        M577_MODEL(mdl, 0);
        rope->mdl[i] = mdl;
        Hu3DModelAttrSet(mdl, 1);
        Hu3DModelAttrSet(mdl, 0x40000002);
        Hu3DModelLayerSet(mdl, 3);
        Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
        Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrReset(mdl, 1);
        rope->pos[i].x = 0.0f;
        rope->pos[i].y = 0.0f;
        rope->pos[i].z = 0.0f;
        rope->vel[i].x = 0.0f;
        rope->vel[i].y = 0.0f;
        rope->vel[i].z = 0.0f;
        rope->prev[i].x = 0.0f;
        rope->prev[i].y = 0.0f;
        rope->prev[i].z = 0.0f;
    }
    rope->count = count;
    rope->a.x = 0.0f;
    rope->a.y = 0.0f;
    rope->a.z = 0.0f;
    rope->b.x = 0.0f;
    rope->b.y = 0.0f;
    rope->b.z = 0.0f;
    rope->f498 = 0.05f;
    rope->f49C = 0.99f;
    rope->f4A0 = 400.0f;
    rope->f4A4 = 0.5f;
}

void fn_1_2E08(M577Rope *rope)
{
    HuVecF d;
    f32 len;
    f32 ratio;
    f32 k;
    s32 n;
    s32 count;
    s32 i;

    d.x = rope->a.x - rope->b.x;
    d.y = rope->a.y - rope->b.y;
    d.z = rope->a.z - rope->b.z;
    len = sqrtf(d.z * d.z + (d.x * d.x + d.y * d.y));
    ratio = len / rope->f4A0;
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    for (i = 0; i < rope->count; i++) {
        rope->pos[i].y -= 0.98f * (1.0f - ratio);
        rope->pos[i].x += rope->vel[i].x;
        rope->pos[i].y += rope->vel[i].y;
        rope->pos[i].z += rope->vel[i].z;
    }
    n = rope->count - 1;
    rope->pos[0].x = rope->a.x;
    rope->pos[0].y = rope->a.y;
    rope->pos[0].z = rope->a.z;
    rope->prev[0].x = rope->pos[0].x;
    rope->prev[0].y = rope->pos[0].y;
    rope->prev[0].z = rope->pos[0].z;
    rope->pos[n].x = rope->b.x;
    rope->pos[n].y = rope->b.y;
    rope->pos[n].z = rope->b.z;
    rope->prev[n] = rope->pos[n];
    for (i = 0; i < n; i++) {
        d.x = rope->pos[i + 1].x - rope->pos[i].x;
        d.y = rope->pos[i + 1].y - rope->pos[i].y;
        d.z = rope->pos[i + 1].z - rope->pos[i].z;
        len = sqrtf(d.z * d.z + (d.x * d.x + d.y * d.y));
        k = rope->f4A4 * (rope->f498 - len);
        rope->pos[i].x -= k * (d.x / len);
        rope->pos[i].y -= k * (d.y / len);
        rope->pos[i].z -= k * (d.z / len);
        rope->pos[i + 1].x += k * (d.x / len);
        rope->pos[i + 1].y += k * (d.y / len);
        rope->pos[i + 1].z += k * (d.z / len);
    }
    count = rope->count;
    for (i = 0; i < count; i++) {
        if (rope->pos[i].y < 40.0f) {
            rope->pos[i].y = 40.0f - 0.8f * rope->vel[i].y;
        }
        rope->vel[i].x = rope->f49C * (rope->pos[i].x - rope->prev[i].x);
        rope->vel[i].y = rope->f49C * (rope->pos[i].y - rope->prev[i].y);
        rope->vel[i].z = rope->f49C * (rope->pos[i].z - rope->prev[i].z);
        rope->prev[i] = rope->pos[i];
        Hu3DModelPosSetV(rope->mdl[i], &rope->pos[i]);
    }
}

void fn_1_3494(M577Team *team, s16 side)
{
    u16 solo[3] = { 0x19, 0x3C, 0x64 };
    u16 trio[3] = { 0x23, 0x46, 0x64 };
    M577Work *w = workObj->data;
    M577Player *p;
    M577Sled *sled;
    f32 t;
    s16 r;
    s32 i;
    s32 k;
    s32 j;
    s32 n;

    team->side = side;
    if (side != 0) {
        team->count = 3;
    } else {
        team->count = 1;
    }
    fn_1_45AC(team->stage, team->cam->bit);
    team->stage->side = side;
    team->done = 0;
    for (i = 0; i < team->count; i++) {
        p = team->player[i];
        Hu3DModelCameraSet(p->mdlId, team->cam->bit);
        if (side != 0) {
            if (p->type != 0) {
                p->func = fn_1_6D0C;
                r = frandmod(0x64);
                for (k = 0; k < 3; k++) {
                    if (r < trio[k]) {
                        break;
                    }
                }
                p->aiTbl = fn_1_B460(i, p->dif, k);
            } else {
                p->func = fn_1_674C;
                p->aiTbl = 0;
            }
        } else if (p->type != 0) {
            p->func = fn_1_6ACC;
            r = frandmod(0x64);
            for (k = 0; k < 3; k++) {
                if (r < solo[k]) {
                    break;
                }
            }
            p->aiTbl = fn_1_B430(p->dif, k);
        } else {
            p->func = fn_1_64BC;
            p->aiTbl = 0;
        }
#ifdef TARGET_PC
        p->aiIdx = 0;
#endif
        team->sled[i]->team = team;
        fn_1_3E10(team->sled[i], p, side);
        team->sled[i]->pos.z = 400.0f * (f32)i;
        team->sled[i]->pt.x = 0.0f;
        team->sled[i]->pt.y = -400.0f * (f32)i;
        t = (400.0f * (f32)i) / w->course.value;
        team->sled[i]->baseY = 30.0 + (16000.0 * cos(M_PI * (90.0f * t) / 180.0) - 16000.0);
        team->sled[i]->pos.y = team->sled[i]->baseY;
        team->sled[i]->rot.x = 90.0f * t;
        Hu3DModelPosSetV(team->sled[i]->mSled, &team->sled[i]->pos);
        Hu3DModelPosSet(team->sled[i]->mShadow, team->sled[i]->pos.x, team->sled[i]->pos.y, team->sled[i]->pos.z);
        Hu3DModelRotSetV(team->sled[i]->mSled, &team->sled[i]->rot);
        Hu3DModelCameraSet(team->sled[i]->mSled, team->cam->bit);
        Hu3DModelCameraSet(team->sled[i]->mShadow, team->cam->bit);
    }
    if (side == 0) {
        team->sled[0]->ropeFront = &team->rope[0]->a;
        team->sled[0]->pole->rope = &team->rope[0]->b;
        Hu3DModelCameraSet(team->sled[0]->pole->m28, team->cam->bit);
        Hu3DModelCameraSet(team->sled[0]->pole->m2C, team->cam->bit);
    } else {
        team->sled[0]->ropeFront = &team->rope[0]->a;
        team->sled[1]->ropeBack = &team->rope[0]->b;
        team->sled[1]->ropeFront = &team->rope[1]->a;
        team->sled[2]->ropeBack = &team->rope[1]->b;
    }
    if (side != 0) {
        n = 2;
    } else {
        n = 1;
    }
    for (j = 0; j < n; j++) {
        for (i = 0; i < team->rope[j]->count; i++) {
            Hu3DModelCameraSet(team->rope[j]->mdl[i], team->cam->bit);
        }
    }
    team->speedMax = team->course->value / 1680.0f;
    team->unk38 = 0.5f * team->speedMax;
    team->speed = 0.0f;
    team->dist = 0.0f;
    team->state = 0;
}

void fn_1_3AB8(M577Work *w)
{
    u16 unused[4] = { 3, 2, 1, 1 };
    u16 rot[3] = { 0, 1, 2 };
    s16 a;
    s16 b;
    s32 k;
    s32 i;

    for (i = 0; i < 3; i++) {
        a = frandmod(3);
        b = frandmod(3);
        rot[a] = rot[b];
        rot[b] = rot[a];
    }
    k = 0;
    for (i = 0; i < 4; i++) {
        if (w->player[i]->grp == 0) {
            w->team[0].player[0] = w->player[i];
            w->player[i]->team = &w->team[0];
        } else {
#ifdef TARGET_PC
            if (k >= 3) {
                continue;
            }
#endif
            w->team[1].player[(s16)rot[k]] = w->player[i];
            w->player[i]->team = &w->team[1];
            w->player[i]->teamIdx = rot[k];
            k++;
        }
    }
    w->team[0].sled[0] = &w->sled[0];
    w->team[1].sled[0] = &w->sled[1];
    w->team[1].sled[1] = &w->sled[2];
    w->team[1].sled[2] = &w->sled[3];
    w->team[0].rope[0] = &w->rope[0];
    w->team[1].rope[0] = &w->rope[1];
    w->team[1].rope[1] = &w->rope[2];
    w->team[0].course = &w->course;
    w->team[1].course = &w->course;
    w->team[0].stage = &w->stage[0];
    w->team[1].stage = &w->stage[1];
    w->team[0].cam = &w->cam[0];
    w->team[1].cam = &w->cam[1];
    w->team[0].stage = &w->stage[0];
    w->team[1].stage = &w->stage[1];
    w->team[0].sled[0]->pole = &w->pole;
    fn_1_2B60(w->team[0].rope[0], 8);
    w->team[0].rope[0]->f4A0 = 100.0f;
    fn_1_2B60(w->team[1].rope[0], 0x20);
    fn_1_2B60(w->team[1].rope[1], 0x20);
    fn_1_3494(&w->team[0], 0);
    fn_1_3494(&w->team[1], 1);
    w->team[0].player[0]->sled->eff = &w->eff[0];
    w->team[1].player[0]->sled->eff = &w->eff[1];
    w->team[1].player[1]->sled->eff = &w->eff[2];
    w->team[1].player[2]->sled->eff = &w->eff[3];
    w->team[0].player[0]->sled->pole->eff = &w->eff[4];
    fn_1_B5D4(&w->eff[0], camBit[0], &w->team[0].player[0]->sled->pos);
    fn_1_B5D4(&w->eff[1], camBit[1], &w->team[1].player[0]->sled->pos);
    fn_1_B5D4(&w->eff[2], camBit[1], &w->team[1].player[1]->sled->pos);
    fn_1_B5D4(&w->eff[3], camBit[1], &w->team[1].player[2]->sled->pos);
    fn_1_B5D4(&w->eff[4], camBit[0], &w->team[0].player[0]->sled->pole->pos);
}

void fn_1_3E10(M577Sled *sled, M577Player *player, s32 side)
{
    M577Work *w = workObj->data;
    M577StageVec p1;
    M577StageVec p2;
    M577StageVec *v;
    f32 t;
    s16 mdl;

    sled->player = player;
    sled->pos.x = 0.0f;
    sled->pos.y = 0.0f;
    sled->pos.z = 0.0f;
    sled->rot.x = 0.0f;
    sled->rot.y = 0.0f;
    sled->rot.z = 0.0f;
    sled->velX = 0.0f;
    sled->unk62 = 0;
    sled->unk64 = 0;
    sled->roll = 0.0f;
    M577_MODEL(mdl, 5);
    sled->mSled = mdl;
    Hu3DModelAttrSet(mdl, 1);
    Hu3DModelAttrSet(mdl, 0x40000002);
    Hu3DModelLayerSet(mdl, 3);
    Hu3DModelPosSetV(mdl, &sled->pos);
    Hu3DModelRotSetV(mdl, &sled->rot);
    Hu3DModelAttrReset(mdl, 1);
    Hu3DMotionTimeSet(mdl, 10.0f);
    M577_MODEL(mdl, 6);
    sled->mWheel = mdl;
    Hu3DModelAttrSet(mdl, 1);
    Hu3DModelAttrSet(mdl, 0x40000002);
    Hu3DModelLayerSet(mdl, 3);
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(mdl, 1);
    M577_MODEL(mdl, 8);
    sled->mShadow = mdl;
    Hu3DModelAttrSet(mdl, 1);
    Hu3DModelAttrSet(mdl, 0x40000002);
    Hu3DModelLayerSet(mdl, 1);
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(mdl, 1);
    if (side == 0) {
        sled->pole->pos.x = 0.0f;
        t = -400.0f / w->course.value;
        sled->pole->pos.y = 30.0 + (16000.0 * cos(M_PI * (90.0f * t) / 180.0) - 16000.0);
        sled->pole->pos.z = 400.0f;
        sled->pole->prevX = 0.0f;
        sled->pole->velX = 0.0f;
        sled->pole->unk20 = sled->pole->pos.y;
        M577_MODEL(mdl, 7);
        sled->pole->m28 = mdl;
        Hu3DModelAttrSet(mdl, 1);
        Hu3DModelAttrSet(mdl, 0x40000002);
        Hu3DModelLayerSet(mdl, 3);
        Hu3DModelPosSetV(mdl, &sled->pole->pos);
        Hu3DModelRotSet(mdl, 90.0f * t, 0.0f, 0.0f);
        Hu3DModelAttrReset(mdl, 1);
        M577_MODEL(mdl, 6);
        sled->pole->m2A = mdl;
        Hu3DModelAttrSet(mdl, 1);
        Hu3DModelAttrSet(mdl, 0x40000002);
        Hu3DModelLayerSet(mdl, 3);
        Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
        Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrReset(mdl, 1);
        Hu3DModelHookSet(sled->pole->m28, "mark03", sled->pole->m2A);
        M577_MODEL(mdl, 8);
        sled->pole->m2C = mdl;
        Hu3DModelAttrSet(mdl, 1);
        Hu3DModelAttrSet(mdl, 0x40000002);
        Hu3DModelLayerSet(mdl, 1);
        Hu3DModelPosSet(mdl, sled->pole->pos.x, sled->pole->pos.y, sled->pole->pos.z);
        Hu3DModelRotSet(mdl, 90.0f * t, 0.0f, 0.0f);
        Hu3DModelAttrReset(mdl, 1);
        Hu3DModelScaleSet(mdl, 0.6f, 1.0f, 0.6f);
        sled->pole->rot = 0.0f;
    }
    Hu3DModelHookSet(sled->mSled, "mark03", sled->mWheel);
    Hu3DModelHookSet(sled->mSled, "mark00", player->mdlId);
    player->sled = sled;
    v = sled->team->course->table;
    p1.x = (v[0].x + v[1].x) * 0.5f;
    p1.y = (v[0].y + v[1].y) * 0.5f;
    p2.x = (v[2].x + v[3].x) * 0.5f;
    p2.y = (v[2].y + v[3].y) * 0.5f;
    fn_1_A754(&sled->line, &p1.x, &p2.x);
}

static f32 markOfs[11][2] = {
    { -8.1268f, -449.323f },
    { -11.295f, 158.7648f },
    { -16.807f, -784.302f },
    { -21.9995f, 398.8714f },
    { -27.733f, -281.36f },
    { -36.1195f, 449.9174f },
    { -51.451f, 42.53f },
    { -58.605f, 17.1205f },
    { -67.79f, 122.0516f },
    { -71.5715f, -360.486f },
    { -79.356f, 265.9656f },
};

void fn_1_45AC(M577Stage *stage, u16 bit)
{
    M577Work *w = workObj->data;
    s16 mdl;
    s16 mot;
    s32 i;

    stage->rot = 0.0f;
    M577_MODEL(mdl, 1);
    stage->m04 = mdl;
    M577_STAGE_MODEL(mdl, 0x40000002, stage->rot);
    Hu3DModelAttrReset(mdl, 1);
    M577_MODEL(mdl, 2);
    stage->m06 = mdl;
    M577_STAGE_MODEL(mdl, 0x40000002, stage->rot);
    Hu3DModelAttrReset(mdl, 1);
    M577_MODEL(mdl, 3);
    stage->m08 = mdl;
    M577_STAGE_MODEL(mdl, 0x40000002, stage->rot);
    M577_MODEL(mdl, 4);
    stage->m0A = mdl;
    M577_STAGE_MODEL(mdl, 0x40000002, stage->rot - 22.5f);
    M577_MODEL(mdl, 9);
    stage->m0C = mdl;
    M577_STAGE_MODEL(mdl, 0x40000002, stage->rot);
    Hu3DModelAttrReset(mdl, 1);
    M577_MODEL(mdl, 0xA);
    stage->m0E = mdl;
    M577_STAGE_MODEL(mdl, 0x40000002, stage->rot);
    M577_MODEL(mdl, 0xC);
    M577_MOTION(mot, mdl, 3);
    stage->m10 = mdl;
    Hu3DMotionSet(mdl, mot);
    M577_STAGE_MODEL(mdl, 0x40000002, stage->rot);
    Hu3DModelAttrReset(mdl, 1);
    M577_MODEL(mdl, 0xC);
    M577_MOTION(mot, mdl, 3);
    stage->m12 = mdl;
    Hu3DMotionSet(mdl, mot);
    M577_STAGE_MODEL(mdl, 0x40000002, stage->rot);
    M577_MODEL(mdl, 0xE);
    stage->m14 = mdl;
    M577_STAGE_MODEL(mdl, 0x40000002, stage->rot);
    Hu3DModelAttrReset(mdl, 1);
    M577_MODEL(mdl, 0xF);
    stage->m16 = mdl;
    M577_STAGE_MODEL(mdl, 0x40000002, stage->rot);
    Hu3DModelAttrReset(mdl, 1);
    M577_MODEL(mdl, 0x10);
    stage->m18 = mdl;
    M577_STAGE_MODEL(mdl, 0x40000002, stage->rot);
    M577_MODEL(mdl, 0x11);
    stage->m1A = mdl;
    M577_STAGE_MODEL(mdl, 0x40000002, stage->rot);
    M577_MODEL(mdl, 0x12);
    stage->m1C = mdl;
    M577_STAGE_MODEL(mdl, 0x40000001, stage->rot);
    M577_MODEL(mdl, 0x13);
    stage->m1E = mdl;
    M577_STAGE_MODEL(mdl, 0x40000001, stage->rot);
    M577_MODEL(mdl, 0x14);
    stage->m20 = mdl;
    M577_STAGE_MODEL(mdl, 0x40000001, stage->rot);
    M577_MODEL(mdl, 0x16);
    M577_MOTION(mot, mdl, 1);
    stage->m38 = mdl;
    Hu3DMotionSet(mdl, mot);
    M577_STAGE_MODEL(mdl, 0x40000002, stage->rot);
    Hu3DModelAttrReset(mdl, 1);
    stage->done52 = 0;
    M577_MODEL(mdl, 0x17);
    M577_MOTION(mot, mdl, 2);
    stage->m3A = mdl;
    Hu3DMotionSet(mdl, mot);
    M577_STAGE_MODEL(mdl, 0x40000002, stage->rot);
    stage->done54 = 0;
    for (i = 0; i < 0xB; i++) {
        M577_MODEL(mdl, 0x15);
        M577_MOTION(mot, mdl, 0);
        stage->mark[i] = mdl;
        Hu3DMotionSet(mdl, mot);
        Hu3DModelAttrSet(mdl, 1);
        Hu3DModelAttrSet(mdl, 0x40000002);
        Hu3DModelLayerSet(mdl, 0);
        Hu3DModelPosSet(mdl, markOfs[i][1], -16000.0f, 0.0f);
        Hu3DModelRotSet(mdl, stage->rot + markOfs[i][0], 0.0f, 0.0f);
        Hu3DModelCameraSet(mdl, bit);
        if (i < 6) {
            Hu3DModelAttrReset(mdl, 1);
        }
        stage->markDone[i] = 0;
    }
    M577_MODEL(mdl, 0x18);
    stage->m56 = mdl;
    Hu3DModelAttrSet(mdl, 1);
    Hu3DModelAttrSet(mdl, 0x40000002);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelPosSet(mdl, 0.0f, -16000.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(mdl, bit);
}

void fn_1_579C(M577Stage *stage)
{
    M577Work *w = workObj->data;
    M577Node *node;
    s32 i;

    Hu3DModelRotSet(stage->m04, stage->rot, 0.0f, 0.0f);
    Hu3DModelRotSet(stage->m06, stage->rot, 0.0f, 0.0f);
    Hu3DModelRotSet(stage->m08, stage->rot, 0.0f, 0.0f);
    Hu3DModelRotSet(stage->m0A, stage->rot, 0.0f, 0.0f);
    Hu3DModelRotSet(stage->m0C, stage->rot, 0.0f, 0.0f);
    Hu3DModelRotSet(stage->m0E, stage->rot, 0.0f, 0.0f);
    Hu3DModelRotSet(stage->m10, stage->rot, 0.0f, 0.0f);
    Hu3DModelRotSet(stage->m12, stage->rot - 90.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(stage->m14, stage->rot, 0.0f, 0.0f);
    Hu3DModelRotSet(stage->m16, stage->rot, 0.0f, 0.0f);
    Hu3DModelRotSet(stage->m18, stage->rot, 0.0f, 0.0f);
    Hu3DModelRotSet(stage->m1A, stage->rot, 0.0f, 0.0f);
    Hu3DModelRotSet(stage->m1C, stage->rot, 0.0f, 0.0f);
    Hu3DModelRotSet(stage->m1E, stage->rot, 0.0f, 0.0f);
    Hu3DModelRotSet(stage->m20, stage->rot, 0.0f, 0.0f);
    Hu3DModelRotSet(stage->m38, stage->rot, 0.0f, 0.0f);
    Hu3DModelRotSet(stage->m3A, stage->rot, 0.0f, 0.0f);
    for (i = 0; i < 0xB; i++) {
        Hu3DModelRotSet(stage->mark[i], stage->rot + markOfs[i][0], 0.0f, 0.0f);
    }
    if (w->active != 0) {
        if (stage->rot > 10.0f && (Hu3DModelAttrGet(stage->m10) & 1) == 0) {
            Hu3DModelAttrSet(stage->m10, 1);
        }
        if (stage->rot > 55.0f && (Hu3DModelAttrGet(stage->m12) & 1) != 0) {
            Hu3DModelAttrReset(stage->m0E, 1);
            Hu3DModelAttrReset(stage->m12, 1);
        }
        if (stage->rot > 30.0f && (Hu3DModelAttrGet(stage->m14) & 1) == 0) {
            Hu3DModelAttrSet(stage->m14, 1);
            Hu3DModelAttrSet(stage->m04, 1);
            Hu3DModelAttrSet(stage->m38, 1);
            Hu3DModelAttrSet(stage->mark[0], 1);
            Hu3DModelAttrSet(stage->mark[1], 1);
            Hu3DModelAttrSet(stage->mark[2], 1);
            Hu3DModelAttrSet(stage->mark[3], 1);
        }
        if (stage->rot > 55.0f && (Hu3DModelAttrGet(stage->m16) & 1) == 0) {
            Hu3DModelAttrSet(stage->m16, 1);
            Hu3DModelAttrSet(stage->m06, 1);
            Hu3DModelAttrSet(stage->mark[4], 1);
            Hu3DModelAttrSet(stage->mark[5], 1);
        }
        if (stage->rot > 15.0f && (Hu3DModelAttrGet(stage->m18) & 1) != 0) {
            Hu3DModelAttrReset(stage->m18, 1);
            Hu3DModelAttrReset(stage->m08, 1);
            Hu3DModelAttrReset(stage->m1C, 1);
            Hu3DModelAttrReset(stage->m1E, 1);
            Hu3DModelAttrReset(stage->m3A, 1);
            Hu3DModelAttrReset(stage->mark[6], 1);
            Hu3DModelAttrReset(stage->mark[7], 1);
        }
        if (stage->rot > 80.0f && (Hu3DModelAttrGet(stage->m18) & 1) == 0) {
            Hu3DModelAttrSet(stage->m18, 1);
            Hu3DModelAttrSet(stage->m08, 1);
            Hu3DModelAttrSet(stage->m1C, 1);
            Hu3DModelAttrSet(stage->m1E, 1);
            Hu3DModelAttrSet(stage->m3A, 1);
            Hu3DModelAttrSet(stage->mark[6], 1);
            Hu3DModelAttrSet(stage->mark[7], 1);
        }
        if (stage->rot > 40.0f && (Hu3DModelAttrGet(stage->m1A) & 1) != 0) {
            Hu3DModelAttrReset(stage->m1A, 1);
            Hu3DModelAttrReset(stage->m0A, 1);
            Hu3DModelAttrReset(stage->m20, 1);
            Hu3DModelAttrReset(stage->mark[8], 1);
            Hu3DModelAttrReset(stage->mark[9], 1);
            Hu3DModelAttrReset(stage->mark[10], 1);
        }
        if (stage->done52 == 0 && stage->rot >= 16.25f) {
            stage->done52 = 1;
            Hu3DModelAttrReset(stage->m38, 0x40000002);
            if (stage->side != 0) {
                fn_1_26C0(&w->list, fn_1_B3CC, &stage->sfx5C);
                stage->sfx5C = 0;
            } else {
                HuAudFXPlayPan(0x941, 0x30);
            }
        }
        if (stage->done54 == 0 && stage->rot >= 57.38f) {
            stage->done54 = 1;
            Hu3DModelAttrReset(stage->m3A, 0x40000002);
            if (stage->side != 0) {
                fn_1_26C0(&w->list, fn_1_B3CC, &stage->sfx5E);
                stage->sfx5E = 0;
            } else {
                HuAudFXPlayPan(0x941, 0x30);
            }
        }
        for (i = 0; i < 0xB; i++) {
            if (stage->markDone[i] == 0 && stage->rot + markOfs[i][0] >= 0.0f) {
                stage->markDone[i] = 1;
                Hu3DModelAttrReset(stage->mark[i], 0x40000002);
            }
        }
    }
}

#define M577_STEER_A(p) \
    do { \
    M577Sled *sled; \
    f32 a0; \
    f32 r0; \
    f32 b0; \
\
    sled = p->sled; \
    if (p->stick < 0) { \
        sled->velX -= 0.3f; \
        if (sled->velX < -10.0f) { \
            sled->velX = -10.0f; \
        } \
        p->motRate += 0.05f; \
        if (p->motRate > 1.0f) { \
            p->motRate = 1.0f; \
        } \
    } else if (p->stick > 0) { \
        sled->velX += 0.3f; \
        if (sled->velX > 10.0f) { \
            sled->velX = 10.0f; \
        } \
        p->motRate -= 0.05f; \
        if (p->motRate < 0.0f) { \
            p->motRate = 0.0f; \
        } \
    } else { \
        if (p->motRate > 0.5f) { \
            if (p->motRate - 0.05f < 0.5f) { \
                a0 = 0.5f; \
            } else { \
                a0 = p->motRate - 0.05f; \
            } \
            r0 = a0; \
        } else { \
            if (0.05f + p->motRate > 0.5f) { \
                b0 = 0.5f; \
            } else { \
                b0 = 0.05f + p->motRate; \
            } \
            r0 = b0; \
        } \
        p->motRate = r0; \
    } \
    } while (0)

#define M577_STEER_B(p) \
    do { \
    M577Sled *sled; \
    f32 a0; \
    f32 r0; \
    f32 b0; \
    f32 a1; \
    f32 r1; \
    f32 b1; \
\
    sled = p->sled; \
    if (p->stick < 0) { \
        sled->velX -= 0.5f; \
        if (sled->velX < -9.0f) { \
            sled->velX = -8.5f; \
        } \
        p->motRate += 0.05f; \
        if (p->motRate > 1.0f) { \
            p->motRate = 1.0f; \
        } \
    } else if (p->stick > 0) { \
        sled->velX += 0.5f; \
        if (sled->velX > 9.0f) { \
            sled->velX = 8.5f; \
        } \
        p->motRate -= 0.05f; \
        if (p->motRate < 0.0f) { \
            p->motRate = 0.0f; \
        } \
    } else { \
        if (sled->velX > 0.0f) { \
            if (sled->velX - 0.5f < 0.0f) { \
                a0 = 0.0f; \
            } else { \
                a0 = sled->velX - 0.5f; \
            } \
            r0 = a0; \
        } else { \
            if (0.5f + sled->velX > 0.0f) { \
                b0 = 0.0f; \
            } else { \
                b0 = 0.5f + sled->velX; \
            } \
            r0 = b0; \
        } \
        sled->velX = r0; \
        if (p->motRate > 0.5f) { \
            if (p->motRate - 0.05f < 0.5f) { \
                a1 = 0.5f; \
            } else { \
                a1 = p->motRate - 0.05f; \
            } \
            r1 = a1; \
        } else { \
            if (0.05f + p->motRate > 0.5f) { \
                b1 = 0.5f; \
            } else { \
                b1 = 0.05f + p->motRate; \
            } \
            r1 = b1; \
        } \
        p->motRate = r1; \
    } \
    } while (0)

void fn_1_5F7C(M577Player *p)
{
    M577_STEER_A(p);
}

void fn_1_61A4(M577Player *p)
{
    M577_STEER_B(p);
}

void fn_1_64BC(M577Player *p)
{
    M577Sled *sp8 = p->sled;

    if (HuPadStkX[p->padNo] < -0x20) {
        p->stick = -1;
    } else if (HuPadStkX[p->padNo] > 0x20) {
        p->stick = 1;
    } else {
        p->stick = 0;
    }
    M577_STEER_A(p);
}

void fn_1_674C(M577Player *p)
{
    M577Sled *sp8 = p->sled;

    if (HuPadStkX[p->padNo] < -0x20) {
        p->stick = -1;
    } else if (HuPadStkX[p->padNo] > 0x20) {
        p->stick = 1;
    } else {
        p->stick = 0;
    }
    M577_STEER_B(p);
}

void fn_1_6ACC(M577Player *p)
{
#ifdef TARGET_PC
    /* the AI table has 0x691 entries per row */
    p->stick = p->aiIdx < 0x691 ? p->aiTbl[p->aiIdx] : 0;
    p->aiIdx++;
#else
    p->stick = *p->aiTbl;
    p->aiTbl++;
#endif
    M577_STEER_A(p);
}

void fn_1_6D0C(M577Player *p)
{
#ifdef TARGET_PC
    /* the AI table has 0x691 entries per row */
    p->stick = p->aiIdx < 0x691 ? p->aiTbl[p->aiIdx] : 0;
    p->aiIdx++;
#else
    p->stick = *p->aiTbl;
    p->aiTbl++;
#endif
    M577_STEER_B(p);
}

void fn_1_703C(M577Team *team)
{
}

void fn_1_7040(M577Team *team)
{
    M577Player *p;
    M577Sled *sled;
    f32 t;
    f32 a0;
    f32 r0;
    f32 b0;
    s32 n;
    s32 i;

#ifdef TARGET_PC
    sled = team->player[0]->sled;
#endif
    switch (team->sub) {
        case 0:
            if (seActive[team->side] != 0) {
                seActive[team->side] = 0;
                HuAudFXStop(seHandle[team->side]);
            }
            team->timer = 0;
            team->sub++;
        case 1:
            n = team->count;
            for (i = 0; i < n; i++) {
                p = team->player[i];
                sled = p->sled;
                if (p->motRate > 0.5f) {
                    if (p->motRate - 0.1f < 0.5f) {
                        a0 = 0.5f;
                    } else {
                        a0 = p->motRate - 0.1f;
                    }
                    r0 = a0;
                } else {
                    if (0.1f + p->motRate > 0.5f) {
                        b0 = 0.5f;
                    } else {
                        b0 = 0.1f + p->motRate;
                    }
                    r0 = b0;
                }
                p->motRate = r0;
                t = p->motMax * p->motRate;
                CharMotionTimeSet(p->charNo, t);
                Hu3DMotionTimeSet(sled->mSled, t);
                fn_1_B720(sled->eff, 0);
            }
            if (team->side == 0) {
                fn_1_B720(sled->pole->eff, 0);
            }
            team->timer++;
            if (team->timer == 5) {
                team->sub++;
            }
            return;
        case 2:
            team->func = fn_1_703C;
            break;
    }
}

void fn_1_72A8(M577Team *team)
{
    M577Work *w = workObj->data;

    team->speed = team->speedMax;
}

void fn_1_72D0(M577Team *team)
{
    M577Work *w = workObj->data;
    M577Player *p = team->player[0];
    M577Sled *sled = p->sled;
    M577Pole *pole;
    M577StageVec *v;
    M577StageVec p1;
    M577StageVec p2;
    M577StageVec q;
    f32 diff;
    f32 ad;
    f32 k;
    f32 vol;
    f32 pitch;
    f32 t;
    s32 n;
    s32 i;

    if (team->done == 0) {
        fn_1_72A8(team);
        fn_1_B720(sled->eff, 1);
        fn_1_B720(sled->pole->eff, 1);
    } else {
        team->speed -= 0.2f;
        if (team->speed < 0.0f) {
            team->speed = 0.0f;
            team->func = fn_1_7040;
            team->sub = 0;
        }
        if (team->speed < 0.25f * team->speedMax) {
            fn_1_B720(sled->eff, 0);
            fn_1_B720(sled->pole->eff, 0);
        }
    }
    sled->pos.x += sled->velX;
    if (sled->velX > -0.3f && sled->velX < 0.3f) {
        sled->velX = 0.0f;
    }
    pole = sled->pole;
    pole->pos.x += pole->velX;
    diff = pole->pos.x - sled->pos.x;
    ad = fabsf(diff);
    if (ad > 100.0f) {
        k = 0.2f * (100.0f - ad);
        sled->pos.x -= k * (diff / ad);
        pole->pos.x += k * (diff / ad);
    }
    pole->velX = 0.9f * (pole->pos.x - pole->prevX);
    pole->prevX = pole->pos.x;
    sled->pt.x = sled->pos.x;
    sled->pt.y += team->speed;
    n = team->course->count / 2 - 1;
    v = team->course->table;
    for (i = 0; i < n; i++, v += 2) {
        if (fn_1_B4D0(&sled->pt, v) != 0) {
            break;
        }
    }
    if (i == n) {
        if (p->timer == 0) {
            team->state = 2;
            team->sub = 0;
            team->func = fn_1_85F4;
            p->state = 1;
            p->timer = 1;
        } else {
            v = team->course->table;
            q.y = sled->pt.y;
            for (i = 0; i < n; i++, v += 2) {
                p1.x = (v[0].x + v[1].x) * 0.5f;
                p1.y = (v[0].y + v[1].y) * 0.5f;
                p2.x = (v[2].x + v[3].x) * 0.5f;
                p2.y = (v[2].y + v[3].y) * 0.5f;
                fn_1_A754(&sled->line, &p1.x, &p2.x);
                q.x = fn_1_A9D0(&sled->line, q.y);
                if (fn_1_B4D0(&q, v) != 0) {
                    break;
                }
            }
#ifdef TARGET_PC
            if (i == n) {
                v -= 2;
            }
#endif
            if (q.x > sled->pt.x) {
                p1 = v[0];
                p2 = v[2];
            } else {
                p1 = v[1];
                p2 = v[3];
            }
            fn_1_A754(&sled->line, &p1.x, &p2.x);
            sled->pos.x = fn_1_A9D0(&sled->line, q.y);
            if (w->unk1B5E == 0 && sled->pt.y > team->course->value) {
                team->func = fn_1_9380;
            }
        }
    } else {
        p1.x = (v[0].x + v[1].x) * 0.5f;
        p1.y = (v[0].y + v[1].y) * 0.5f;
        p2.x = (v[2].x + v[3].x) * 0.5f;
        p2.y = (v[2].y + v[3].y) * 0.5f;
        fn_1_A754(&sled->line, &p1.x, &p2.x);
        if (fn_1_A8D4(&sled->line, &sled->pt) > 75.0f) {
            team->state = 0;
        } else {
            team->state = 0;
        }
        if (w->unk1B5E == 0 && sled->pt.y > team->course->value) {
            team->func = fn_1_9380;
        }
    }
    sled->velX *= 0.97f;
    team->dist += team->speed;
    team->stage->rot = (90.0f * team->dist) / team->course->value;
    team->cam->target.x = 0.5f * sled->pos.x;
    Hu3DModelPosSetV(sled->mSled, &sled->pos);
    Hu3DModelPosSet(sled->mShadow, sled->pos.x, sled->pos.y, sled->pos.z);
    sled->roll = sled->roll - (360.0f * team->speed) / 273.94686980565;
    while (sled->roll > 180.0f) {
        sled->roll -= 360.0f;
    }
    while (sled->roll < -180.0f) {
        sled->roll += 360.0f;
    }
    Hu3DModelRotSet(sled->mWheel, sled->roll, 0.0f, 0.0f);
    Hu3DModelPosSetV(pole->m28, &pole->pos);
    Hu3DModelPosSet(pole->m2C, pole->pos.x, pole->pos.y, pole->pos.z);
    pole->rot = pole->rot - (360.0f * team->speed) / 273.94686980565;
    while (sled->pole->rot > 180.0f) {
        sled->pole->rot -= 360.0f;
    }
    while (sled->pole->rot < -180.0f) {
        sled->pole->rot += 360.0f;
    }
    Hu3DModelRotSet(sled->pole->m2A, sled->pole->rot, 0.0f, 0.0f);
    t = p->motMax * p->motRate;
    CharMotionTimeSet(p->charNo, t);
    Hu3DMotionTimeSet(sled->mSled, t);
    if (team->speed < 0.25f * team->speedMax) {
        if (seActive[0] != 0) {
            seActive[0] = 0;
            HuAudFXStop(seHandle[0]);
        }
    } else {
        vol = sled->pos.x / 700.0f;
        if (vol > 1.0f) {
            vol = 1.0f;
        }
        if (vol < -1.0f) {
            vol = -1.0f;
        }
        if (team->done != 0) {
            pitch = 8191.0f * (team->speed / team->speedMax) - 8191.0f;
        } else {
            t = fabsf(sled->velX) / 10.0f;
            if (t > 1.0f) {
                t = 1.0f;
            }
            pitch = 8191.0f * t;
        }
        if (seActive[0] == 0) {
            seHandle[0] = HuAudFXPlay(0x93C);
            seActive[0] = 1;
        }
        HuAudFXPanning(seHandle[0], (s16)(48.0f + 16.0f * vol));
        HuAudFXPitchSet(seHandle[0], (s16)pitch);
    }
}

void fn_1_7D24(M577Team *team)
{
    M577Work *w = workObj->data;
    M577Player *p;
    M577Sled *sled;
    M577StageVec *v;
    M577StageVec p1;
    M577StageVec p2;
    M577StageVec q;
    f32 sumV;
    f32 maxX;
    f32 minX;
    f32 vol;
    f32 pitch;
    f32 t;
    s32 fail;
    s32 n;
    s32 i;
    s32 k;

    if (team->done == 0) {
        fn_1_72A8(team);
        for (i = 0; i < 3; i++) {
            fn_1_B720(team->player[i]->sled->eff, 1);
        }
    } else {
        team->speed -= 0.2f;
        if (team->speed < 0.0f) {
            team->speed = 0.0f;
            team->func = fn_1_7040;
            team->sub = 0;
        }
        if (team->speed < 0.25f * team->speedMax) {
            for (i = 0; i < 3; i++) {
                fn_1_B720(team->player[i]->sled->eff, 0);
            }
        }
    }
    for (i = 0; i < 3; i++) {
        sled = team->player[i]->sled;
        sled->pos.x += sled->velX;
    }
    fail = 0;
    for (k = 0; k < 3; k++) {
        p = team->player[k];
        sled = p->sled;
        sled->pt.x = sled->pos.x;
        sled->pt.y += team->speed;
        n = team->course->count / 2 - 1;
        v = team->course->table;
        for (i = 0; i < n; i++, v += 2) {
            if (fn_1_B4D0(&sled->pt, v) != 0) {
                break;
            }
        }
        if (i == n) {
            if (p->timer == 0) {
                fail -= 3;
                p->state = 1;
            } else {
                v = team->course->table;
                q.y = sled->pt.y;
                for (i = 0; i < n; i++, v += 2) {
                    p1.x = (v[0].x + v[1].x) * 0.5f;
                    p1.y = (v[0].y + v[1].y) * 0.5f;
                    p2.x = (v[2].x + v[3].x) * 0.5f;
                    p2.y = (v[2].y + v[3].y) * 0.5f;
                    fn_1_A754(&sled->line, &p1.x, &p2.x);
                    q.x = fn_1_A9D0(&sled->line, q.y);
                    if (fn_1_B4D0(&q, v) != 0) {
                        break;
                    }
                }
#ifdef TARGET_PC
                if (i == n) {
                    v -= 2;
                }
#endif
                if (q.x > sled->pt.x) {
                    p1 = v[0];
                    p2 = v[2];
                } else {
                    p1 = v[1];
                    p2 = v[3];
                }
                fn_1_A754(&sled->line, &p1.x, &p2.x);
                sled->pos.x = fn_1_A9D0(&sled->line, q.y);
            }
        } else {
            p1.x = (v[0].x + v[1].x) * 0.5f;
            p1.y = (v[0].y + v[1].y) * 0.5f;
            p2.x = (v[2].x + v[3].x) * 0.5f;
            p2.y = (v[2].y + v[3].y) * 0.5f;
            fn_1_A754(&sled->line, &p1.x, &p2.x);
            if (fn_1_A8D4(&sled->line, &sled->pt) > 75.0f) {
                fail++;
            }
        }
    }
    if (fail < 0) {
        team->state = 2;
        team->sub = 0;
        team->func = fn_1_8BB8;
    } else {
        if (fail == 0) {
            team->state = 0;
        } else {
            team->state = 0;
        }
        if (w->unk1B5E == 1 && team->player[0]->sled->pt.y > team->course->value) {
            team->func = fn_1_9C08;
        }
    }
    team->dist += team->speed;
    team->stage->rot = (90.0f * team->dist) / team->course->value;
    sumV = 0.0f;
    maxX = -20000.0f;
    minX = 20000.0f;
    for (k = 0; k < 3; k++) {
        p = team->player[k];
        sled = p->sled;
        t = p->motMax * p->motRate;
        CharMotionTimeSet(p->charNo, t);
        Hu3DMotionTimeSet(sled->mSled, t);
        Hu3DModelPosSetV(sled->mSled, &sled->pos);
        Hu3DModelPosSet(sled->mShadow, sled->pos.x, sled->pos.y, sled->pos.z);
        sled->roll = sled->roll - (360.0f * team->speed) / 273.94686980565;
        while (sled->roll > 180.0f) {
            sled->roll -= 360.0f;
        }
        while (sled->roll < -180.0f) {
            sled->roll += 360.0f;
        }
        Hu3DModelRotSet(sled->mWheel, sled->roll, 0.0f, 0.0f);
        if (sled->pos.x > maxX) {
            maxX = sled->pos.x;
        }
        if (sled->pos.x < minX) {
            minX = sled->pos.x;
        }
        sumV += fabsf(sled->velX);
    }
    team->cam->target.x = 0.5f * (maxX + minX);
    if (team->speed < 0.25f * team->speedMax) {
        if (seActive[1] != 0) {
            seActive[1] = 0;
            HuAudFXStop(seHandle[1]);
        }
    } else {
        vol = (maxX + minX) / 700.0f;
        if (vol > 1.0f) {
            vol = 1.0f;
        }
        if (vol < -1.0f) {
            vol = -1.0f;
        }
        if (team->done != 0) {
            pitch = 8191.0f * (team->speed / team->speedMax) - 8191.0f;
        } else {
            t = sumV / 3.0f / 9.0f;
            if (t > 1.0f) {
                t = 1.0f;
            }
            pitch = 8191.0f * t;
        }
        if (seActive[1] == 0) {
            seHandle[1] = HuAudFXPlay(0x93D);
            seActive[1] = 1;
        }
        HuAudFXPanning(seHandle[1], (s16)(80.0f + 16.0f * vol));
        HuAudFXPitchSet(seHandle[1], (s16)pitch);
    }
}

void fn_1_85F4(M577Team *team)
{
    M577Work *w = workObj->data;
    M577Player *p = team->player[0];
    M577Sled *sled = p->sled;
    M577StageVec *v;
    M577StageVec p1;
    M577StageVec p2;
    M577StageVec q;
    f32 vol;
    f32 pan;
    f32 t;
    s32 n;
    s32 cnt;
    s32 i;

    switch (team->sub) {
        case 0:
            if (seActive[0] != 0) {
                seActive[0] = 0;
                HuAudFXStop(seHandle[0]);
            }
            vol = sled->pos.x / 700.0f;
            if (vol > 1.0f) {
                vol = 1.0f;
            }
            if (vol < -1.0f) {
                vol = -1.0f;
            }
            pan = 48.0f + 16.0f * vol;
            HuAudFXPlayPan(0x93E, (s16)pan);
            HuAudFXPlayPan(charSeTbl[p->charNo], (s16)pan);
            n = team->course->count / 2 - 1;
            v = team->course->table;
            q.y = sled->pt.y;
            for (i = 0; i < n; i++, v += 2) {
                p1.x = (v[0].x + v[1].x) * 0.5f;
                p1.y = (v[0].y + v[1].y) * 0.5f;
                p2.x = (v[2].x + v[3].x) * 0.5f;
                p2.y = (v[2].y + v[3].y) * 0.5f;
                fn_1_A754(&sled->line, &p1.x, &p2.x);
                q.x = fn_1_A9D0(&sled->line, q.y);
                if (fn_1_B4D0(&q, v) != 0) {
                    sled->unk4C = q.x;
                    break;
                }
            }
            sled->unk50 = sled->pt.x;
            sled->velX = 0.0f;
            sled->pole->prevX = sled->pole->pos.x;
            sled->pole->velX = 0.0f;
            sled->pole->unk1C = sled->unk4C;
            p->stick = 0;
            fn_1_B720(sled->eff, 0);
            fn_1_B720(sled->pole->eff, 0);
            fn_1_26C0(&w->list, fn_1_AB34, p);
            p->sub = 0;
            team->sub++;
            break;
        case 1:
            if (p->state != 1) {
                team->sub++;
                team->timer = 0;
            }
            break;
        case 2:
            team->timer++;
            cnt = team->rope[0]->count;
            if ((team->timer & 1) != 0) {
                Hu3DModelAttrSet(sled->mSled, 1);
                Hu3DModelAttrSet(sled->mShadow, 1);
                Hu3DModelAttrSet(sled->pole->m28, 1);
                Hu3DModelAttrSet(sled->pole->m2C, 1);
                for (i = 0; i < cnt; i++) {
                    Hu3DModelAttrSet(team->rope[0]->mdl[i], 1);
                }
            } else {
                Hu3DModelAttrReset(sled->mSled, 1);
                Hu3DModelAttrReset(sled->mShadow, 1);
                Hu3DModelAttrReset(sled->pole->m28, 1);
                Hu3DModelAttrReset(sled->pole->m2C, 1);
                for (i = 0; i < cnt; i++) {
                    Hu3DModelAttrReset(team->rope[0]->mdl[i], 1);
                }
            }
            t = (f32)team->timer / 30.0f;
            sled->pos.x = sled->unk50 * (1.0f - t) + sled->unk4C * t;
            sled->pole->pos.x = sled->pole->prevX * (1.0f - t) + sled->pole->unk1C * t;
            team->cam->target.x = 0.5f * sled->pos.x;
            if (team->timer == 0x1E) {
                Hu3DModelAttrReset(sled->mSled, 1);
                sled->pole->prevX = sled->pos.x;
                team->state = 0;
                team->func = fn_1_72D0;
                p->state = 0;
                fn_1_26C0(&w->list, fn_1_B318, p);
                p->timer = 0x5A;
            }
            break;
    }
    Hu3DModelPosSetV(sled->mSled, &sled->pos);
    Hu3DModelPosSetV(sled->pole->m28, &sled->pole->pos);
    Hu3DModelPosSetV(sled->mShadow, &sled->pos);
    Hu3DModelPosSetV(sled->pole->m2C, &sled->pole->pos);
}

void fn_1_8BB8(M577Team *team)
{
    M577Work *w = workObj->data;
    M577Player *p;
    M577Sled *sled;
    M577StageVec *v;
    M577StageVec p1;
    M577StageVec p2;
    M577StageVec q;
    f32 maxX;
    f32 minX;
    f32 vol;
    f32 pan;
    f32 t;
    s32 n;
    s32 cnt;
    s32 i;
    s32 j;
    s32 k;

    switch (team->sub) {
        case 0:
            if (seActive[1] != 0) {
                seActive[1] = 0;
                HuAudFXStop(seHandle[1]);
            }
            maxX = -10000.0f;
            minX = 10000.0f;
            n = team->course->count / 2 - 1;
            for (k = 0; k < 3; k++) {
                p = team->player[k];
                sled = p->sled;
                if (sled->pos.x > maxX) {
                    maxX = sled->pos.x;
                }
                if (sled->pos.x < minX) {
                    minX = sled->pos.x;
                }
                fn_1_B720(sled->eff, 0);
                if (p->state != 0) {
                    fn_1_26C0(&w->list, fn_1_AB34, p);
                    p->sub = 0;
                } else {
                    fn_1_26C0(&w->list, fn_1_AF68, p);
                    p->sub = 0;
                }
                if (p->state != 0) {
                    v = team->course->table;
                    q.y = sled->pt.y;
                    for (j = 0; j < n; j++, v += 2) {
                        p1.x = (v[0].x + v[1].x) * 0.5f;
                        p1.y = (v[0].y + v[1].y) * 0.5f;
                        p2.x = (v[2].x + v[3].x) * 0.5f;
                        p2.y = (v[2].y + v[3].y) * 0.5f;
                        fn_1_A754(&sled->line, &p1.x, &p2.x);
                        q.x = fn_1_A9D0(&sled->line, q.y);
                        if (fn_1_B4D0(&q, v) != 0) {
                            sled->unk4C = q.x;
                            break;
                        }
                    }
                    sled->unk50 = sled->pt.x;
                    sled->velX = 0.0f;
                    p->stick = 0;
                }
            }
            vol = (maxX + minX) / 700.0f;
            if (vol > 1.0f) {
                vol = 1.0f;
            }
            if (vol < -1.0f) {
                vol = -1.0f;
            }
            pan = 80.0f + 16.0f * vol;
            HuAudFXPlayPan(0x93F, (s16)pan);
            for (k = 0; k < 3; k++) {
                p = team->player[k];
                if (p->state != 0) {
                    HuAudFXPlayPan(charSeTbl[p->charNo], (s16)pan);
                }
            }
            team->sub++;
            break;
        case 1:
            for (k = 0; k < 3; k++) {
                if (team->player[k]->state == 1) {
                    break;
                }
            }
            if (k == 3) {
                team->sub++;
                team->timer = 0;
            }
            break;
        case 2:
            team->timer++;
            if (team->timer == 0x1E) {
                Hu3DModelAttrReset(team->sled[0]->mSled, 1);
                Hu3DModelAttrReset(team->sled[1]->mSled, 1);
                Hu3DModelAttrReset(team->sled[2]->mSled, 1);
                team->state = 0;
                team->func = fn_1_7D24;
                for (k = 0; k < 3; k++) {
                    p = team->player[k];
                    fn_1_26C0(&w->list, fn_1_B318, p);
                    p->timer = 0x5A;
                    if (p->state == 2) {
                        p->state = 0;
                    }
                }
            }
            cnt = team->rope[0]->count;
            if ((team->timer & 1) != 0) {
                for (k = 0; k < 3; k++) {
                    sled = team->sled[k];
                    Hu3DModelAttrSet(sled->mSled, 1);
                    Hu3DModelAttrSet(sled->mShadow, 1);
                }
                for (j = 0; j < cnt; j++) {
                    Hu3DModelAttrSet(team->rope[0]->mdl[j], 1);
                    Hu3DModelAttrSet(team->rope[1]->mdl[j], 1);
                }
            } else {
                for (k = 0; k < 3; k++) {
                    sled = team->sled[k];
                    Hu3DModelAttrReset(sled->mSled, 1);
                    Hu3DModelAttrReset(sled->mShadow, 1);
                }
                for (j = 0; j < cnt; j++) {
                    Hu3DModelAttrReset(team->rope[0]->mdl[j], 1);
                    Hu3DModelAttrReset(team->rope[1]->mdl[j], 1);
                }
            }
            t = (f32)team->timer / 30.0f;
            for (k = 0; k < 3; k++) {
                p = team->player[k];
                sled = p->sled;
                if (p->state == 2) {
                    sled->pos.x = sled->unk50 * (1.0f - t) + sled->unk4C * t;
                }
            }
            maxX = -20000.0f;
            minX = 20000.0f;
            for (k = 0; k < 3; k++) {
                sled = team->sled[k];
                if (sled->pos.x > maxX) {
                    maxX = sled->pos.x;
                }
                if (sled->pos.x < minX) {
                    minX = sled->pos.x;
                }
            }
            team->cam->target.x = 0.5f * (maxX + minX);
            break;
    }
    Hu3DModelPosSetV(team->sled[0]->mSled, &team->sled[0]->pos);
    Hu3DModelPosSetV(team->sled[1]->mSled, &team->sled[1]->pos);
    Hu3DModelPosSetV(team->sled[2]->mSled, &team->sled[2]->pos);
    Hu3DModelPosSetV(team->sled[0]->mShadow, &team->sled[0]->pos);
    Hu3DModelPosSetV(team->sled[1]->mShadow, &team->sled[1]->pos);
    Hu3DModelPosSetV(team->sled[2]->mShadow, &team->sled[2]->pos);
}

void fn_1_9380(M577Team *team)
{
    M577Player *p = team->player[0];
    M577Sled *sled = p->sled;
    M577Pole *pole;
    f32 pitch;
    f32 vol;
    f32 diff;
    f32 k;
    f32 t;
    f32 a0;
    f32 r0;
    f32 b0;
    f32 a1;
    f32 r1;
    f32 b1;
    f32 sp10;

    if (team->done == 0) {
        fn_1_72A8(team);
        fn_1_B720(sled->eff, 1);
        fn_1_B720(sled->pole->eff, 1);
    } else {
        team->speed -= 0.05f;
        if (team->speed < 0.0f) {
            team->speed = 0.0f;
            team->func = fn_1_7040;
            team->sub = 0;
        }
        if (team->speed < 0.25f * team->speedMax) {
            fn_1_B720(sled->eff, 0);
            fn_1_B720(sled->pole->eff, 0);
        }
    }
    if (sled->pos.x > 0.0f) {
        if (sled->pos.x - 1.5f < 0.0f) {
            a0 = 0.0f;
        } else {
            a0 = sled->pos.x - 1.5f;
        }
        r0 = a0;
    } else {
        if (1.5f + sled->pos.x > 0.0f) {
            b0 = 0.0f;
        } else {
            b0 = 1.5f + sled->pos.x;
        }
        r0 = b0;
    }
    sled->pos.x = r0;
    sled->velX = 0.0f;
    pole = sled->pole;
    pole->pos.x += pole->velX;
    diff = pole->pos.x - sled->pos.x;
    sp10 = fabsf(diff);
    if (sp10 > 100.0f) {
        k = 0.2f * (100.0f - sp10);
        sled->pos.x -= k * (diff / sp10);
        pole->pos.x += k * (diff / sp10);
    }
    pole->velX = 0.9f * (pole->pos.x - pole->prevX);
    pole->prevX = pole->pos.x;
    team->dist += team->speed;
    team->stage->rot = (90.0f * team->dist) / team->course->value;
    team->cam->target.x = 0.5f * sled->pos.x;
    Hu3DModelPosSetV(sled->mSled, &sled->pos);
    Hu3DModelPosSet(sled->mShadow, sled->pos.x, sled->pos.y, sled->pos.z);
    sled->roll = sled->roll - (360.0f * team->speed) / 273.94686980565;
    while (sled->roll > 180.0f) {
        sled->roll -= 360.0f;
    }
    while (sled->roll < -180.0f) {
        sled->roll += 360.0f;
    }
    Hu3DModelRotSet(sled->mWheel, sled->roll, 0.0f, 0.0f);
    Hu3DModelPosSetV(sled->pole->m28, &sled->pole->pos);
    Hu3DModelPosSet(sled->pole->m2C, sled->pole->pos.x, sled->pole->pos.y, sled->pole->pos.z);
    sled->pole->rot = sled->pole->rot - (360.0f * team->speed) / 273.94686980565;
    while (sled->pole->rot > 180.0f) {
        sled->pole->rot -= 360.0f;
    }
    while (sled->pole->rot < -180.0f) {
        sled->pole->rot += 360.0f;
    }
    Hu3DModelRotSet(sled->pole->m2A, sled->pole->rot, 0.0f, 0.0f);
    if (p->motRate > 0.5f) {
        if (p->motRate - 0.05f < 0.5f) {
            a1 = 0.5f;
        } else {
            a1 = p->motRate - 0.05f;
        }
        r1 = a1;
    } else {
        if (0.05f + p->motRate > 0.5f) {
            b1 = 0.5f;
        } else {
            b1 = 0.05f + p->motRate;
        }
        r1 = b1;
    }
    p->motRate = r1;
    t = p->motMax * p->motRate;
    CharMotionTimeSet(p->charNo, t);
    Hu3DMotionTimeSet(sled->mSled, t);
    if (team->speed < 0.25f * team->speedMax) {
        if (seActive[0] != 0) {
            seActive[0] = 0;
            HuAudFXStop(seHandle[0]);
        }
    } else {
        vol = sled->pos.x / 700.0f;
        if (vol > 1.0f) {
            vol = 1.0f;
        }
        if (vol < -1.0f) {
            vol = -1.0f;
        }
        vol = 48.0f + 16.0f * vol;
        if (team->done != 0) {
            pitch = 8191.0f * (team->speed / team->speedMax) - 8191.0f;
        } else {
            t = fabsf(sled->velX) / 10.0f;
            if (t > 1.0f) {
                t = 1.0f;
            }
            pitch = 8191.0f * t;
        }
        if (seActive[0] == 0) {
            seHandle[0] = HuAudFXPlay(0x93C);
            seActive[0] = 1;
        }
        HuAudFXPanning(seHandle[0], (s16)vol);
        HuAudFXPitchSet(seHandle[0], (s16)pitch);
    }
}

void fn_1_9C08(M577Team *team)
{
    M577Player *p;
    M577Sled *sled;
    f32 sp8;
    f32 sumV;
    f32 maxX;
    f32 minX;
    f32 t;
    f32 a0;
    f32 r0;
    f32 b0;
    f32 a1;
    f32 r1;
    f32 b1;
    f32 vol;
    f32 pitch;
    s32 i;

    if (team->done == 0) {
        fn_1_72A8(team);
        for (i = 0; i < 3; i++) {
            fn_1_B720(team->player[i]->sled->eff, 1);
        }
    } else {
        team->speed -= 0.05f;
        if (team->speed < 0.0f) {
            team->speed = 0.0f;
            team->func = fn_1_7040;
            team->sub = 0;
        }
        if (team->speed < 0.25f * team->speedMax) {
            for (i = 0; i < 3; i++) {
                fn_1_B720(team->player[i]->sled->eff, 0);
            }
        }
    }
    for (i = 0; i < 3; i++) {
        sled = team->player[i]->sled;
        if (sled->pos.x > 0.0f) {
            if (sled->pos.x - 1.5f < 0.0f) {
                a0 = 0.0f;
            } else {
                a0 = sled->pos.x - 1.5f;
            }
            r0 = a0;
        } else {
            if (1.5f + sled->pos.x > 0.0f) {
                b0 = 0.0f;
            } else {
                b0 = 1.5f + sled->pos.x;
            }
            r0 = b0;
        }
        sled->pos.x = r0;
        sled->velX = 0.0f;
    }
    team->dist += team->speed;
    team->stage->rot = (90.0f * team->dist) / team->course->value;
    sumV = 0.0f;
    maxX = -20000.0f;
    minX = 20000.0f;
    for (i = 0; i < 3; i++) {
        p = team->player[i];
        sled = p->sled;
        if (p->motRate > 0.5f) {
            if (p->motRate - 0.05f < 0.5f) {
                a1 = 0.5f;
            } else {
                a1 = p->motRate - 0.05f;
            }
            r1 = a1;
        } else {
            if (0.05f + p->motRate > 0.5f) {
                b1 = 0.5f;
            } else {
                b1 = 0.05f + p->motRate;
            }
            r1 = b1;
        }
        p->motRate = r1;
        t = p->motMax * p->motRate;
        CharMotionTimeSet(p->charNo, t);
        Hu3DMotionTimeSet(sled->mSled, t);
        Hu3DModelPosSetV(sled->mSled, &sled->pos);
        Hu3DModelPosSet(sled->mShadow, sled->pos.x, sled->pos.y, sled->pos.z);
        sled->roll = sled->roll - (360.0f * team->speed) / 273.94686980565;
        while (sled->roll > 180.0f) {
            sled->roll -= 360.0f;
        }
        while (sled->roll < -180.0f) {
            sled->roll += 360.0f;
        }
        Hu3DModelRotSet(sled->mWheel, sled->roll, 0.0f, 0.0f);
        if (sled->pos.x > maxX) {
            maxX = sled->pos.x;
        }
        if (sled->pos.x < minX) {
            minX = sled->pos.x;
        }
        sp8 = fabsf(sled->velX);
        sumV += sp8;
    }
    team->cam->target.x = 0.5f * (maxX + minX);
    if (team->speed < 0.25f * team->speedMax) {
        if (seActive[1] != 0) {
            seActive[1] = 0;
            HuAudFXStop(seHandle[1]);
        }
    } else {
        vol = (maxX + minX) / 700.0f;
        if (vol > 1.0f) {
            vol = 1.0f;
        }
        if (vol < -1.0f) {
            vol = -1.0f;
        }
        if (team->done != 0) {
            pitch = 8191.0f * (team->speed / team->speedMax) - 8191.0f;
        } else {
            t = sumV / 3.0f / 9.0f;
            if (t > 1.0f) {
                t = 1.0f;
            }
            pitch = 8191.0f * t;
        }
        if (seActive[1] == 0) {
            seHandle[1] = HuAudFXPlay(0x93D);
            seActive[1] = 1;
        }
        HuAudFXPanning(seHandle[1], (s16)(80.0f + 16.0f * vol));
        HuAudFXPitchSet(seHandle[1], (s16)pitch);
    }
}

void fn_1_A36C(M577Team *team)
{
    M577Sled *sled = team->player[0]->sled;

    Hu3DModelPosSetV(sled->mSled, &sled->pos);
    Hu3DModelPosSet(sled->mShadow, sled->pos.x, sled->pos.y, sled->pos.z);
    sled->roll = sled->roll - 19.7118514397737;
    while (sled->roll > 180.0f) {
        sled->roll -= 360.0f;
    }
    while (sled->roll < -180.0f) {
        sled->roll += 360.0f;
    }
    Hu3DModelRotSet(sled->mWheel, sled->roll, 0.0f, 0.0f);
    Hu3DModelPosSetV(sled->pole->m28, &sled->pole->pos);
    Hu3DModelPosSet(sled->pole->m2C, sled->pole->pos.x, sled->pole->pos.y, sled->pole->pos.z);
    sled->pole->rot = sled->pole->rot - 19.7118514397737;
    while (sled->pole->rot > 180.0f) {
        sled->pole->rot -= 360.0f;
    }
    while (sled->pole->rot < -180.0f) {
        sled->pole->rot += 360.0f;
    }
    Hu3DModelRotSet(sled->pole->m2A, sled->pole->rot, 0.0f, 0.0f);
    if (seActive[0] == 0) {
        seHandle[0] = HuAudFXPlay(0x93C);
        seActive[0] = 1;
    }
    HuAudFXPanning(seHandle[0], 0x40);
    HuAudFXPitchSet(seHandle[0], 0);
}

void fn_1_A5CC(M577Team *team)
{
    M577Sled *sled;
    s32 i;

    for (i = 0; i < 3; i++) {
        sled = team->player[i]->sled;
        Hu3DModelPosSetV(sled->mSled, &sled->pos);
        Hu3DModelPosSet(sled->mShadow, sled->pos.x, sled->pos.y, sled->pos.z);
        sled->roll = sled->roll - 19.7118514397737;
        while (sled->roll > 180.0f) {
            sled->roll -= 360.0f;
        }
        while (sled->roll < -180.0f) {
            sled->roll += 360.0f;
        }
        Hu3DModelRotSet(sled->mWheel, sled->roll, 0.0f, 0.0f);
    }
    if (seActive[1] == 0) {
        seHandle[1] = HuAudFXPlay(0x93D);
        seActive[1] = 1;
    }
    HuAudFXPanning(seHandle[1], 0x40);
    HuAudFXPitchSet(seHandle[1], 0);
}

void fn_1_A754(M577Line *line, f32 *p1, f32 *p2)
{
    f32 q;

    line->vert = (p1[0] == p2[0]);
    if (line->vert != 0) {
        line->offset = p1[0];
        return;
    }
    line->slope = (p2[1] - p1[1]) / (p2[0] - p1[0]);
    line->offset = p1[1] + (-line->slope * p1[0]);
    q = 1.0f + line->slope * line->slope;
    line->norm = sqrtf(q);
}

f32 fn_1_A8D4(M577Line *line, M577StageVec *pt)
{
    f32 r;

    if (line->vert != 0) {
        r = fabsf(line->offset - pt->x);
    } else {
        r = fabsf(line->offset + (line->slope * pt->x - pt->y)) / line->norm;
    }
    return r;
}

f32 fn_1_A9D0(M577Line *line, f32 y)
{
    f32 r;
    f32 d;

    if (line->vert != 0) {
        r = line->offset;
    } else {
        if (line->slope == 0.0f) {
            d = 0.000001f;
        } else {
            d = line->slope;
        }
        r = (y - line->offset) / d;
    }
    return r;
}

void fn_1_AA4C(M577Work *w)
{
    M577Team *team;

    team = &w->team[0];
    Hu3DModelObjPosGet(team->sled[0]->mSled, "mark02", team->sled[0]->ropeFront);
    Hu3DModelObjPosGet(team->sled[0]->pole->m28, "mark01", team->sled[0]->pole->rope);
    team = &w->team[1];
    Hu3DModelObjPosGet(team->sled[0]->mSled, "mark02", team->sled[0]->ropeFront);
    Hu3DModelObjPosGet(team->sled[1]->mSled, "mark01", team->sled[1]->ropeBack);
    Hu3DModelObjPosGet(team->sled[1]->mSled, "mark02", team->sled[1]->ropeFront);
    Hu3DModelObjPosGet(team->sled[2]->mSled, "mark01", team->sled[2]->ropeBack);
}

s32 fn_1_AB34(M577Node *node)
{
    M577Player *p;
    f32 t2;
    f32 a0;
    f32 r0;
    f32 b0;
    f32 t;
    s32 done;

    done = 0;
    p = node->arg;
    switch (p->sub) {
        case 0:
            CharMotionShiftSet(p->charNo, p->obj->mtnId[3], 0.0f, 5.0f, 0x40000001);
            omVibrate(p->playerNo, 0x3C, 0xC, 0);
            p->subTimer = 0;
            p->sub++;
        case 1:
            p->subTimer++;
            if (p->subTimer == 0x3C) {
                p->sub++;
            }
            t = (f32)p->subTimer / 60.0f;
            p->sled->pos.y = (f64)p->sled->baseY + ((f64)(15.0f * (1.0f - t)) * (1.0 - cos(M_PI * (72.0f * (f32)p->subTimer) / 180.0)));
            if (p->motRate > 0.5f) {
                if (p->motRate - 0.05f < 0.5f) {
                    a0 = 0.5f;
                } else {
                    a0 = p->motRate - 0.05f;
                }
                r0 = a0;
            } else {
                if (0.05f + p->motRate > 0.5f) {
                    b0 = 0.5f;
                } else {
                    b0 = 0.05f + p->motRate;
                }
                r0 = b0;
            }
            p->motRate = r0;
            Hu3DMotionTimeSet(p->sled->mSled, p->motMax * p->motRate);
            break;
        case 2:
            p->sled->pos.y = p->sled->baseY;
            CharMotionShiftSet(p->charNo, p->obj->mtnId[4], 0.0f, 5.0f, 0);
            p->sub++;
        case 3:
            if (p->obj->mtnId[4] == Hu3DMotionIDGet(p->mdlId)) {
                p->sub++;
            }
            break;
        case 4:
            if (Hu3DMotionEndCheck(p->mdlId) != 0) {
                CharMotionSet(p->charNo, p->obj->mtnId[5]);
                p->sub++;
            }
            break;
        case 5:
            if (Hu3DMotionEndCheck(p->mdlId) != 0) {
                CharMotionSet(p->charNo, p->obj->mtnId[2]);
                CharModelAttrSet(p->charNo, 0x40000002);
                p->motRate = 0.5f;
                t2 = p->motMax * p->motRate;
                CharMotionTimeSet(p->charNo, t2);
                Hu3DMotionTimeSet(p->sled->mSled, t2);
                p->state = 2;
                done = 1;
            }
            break;
    }
    return done;
}

s32 fn_1_AF68(M577Node *node)
{
    M577Player *p;
    f32 t;
    f32 a0;
    f32 r0;
    f32 b0;
    s32 done;

    done = 0;
    p = node->arg;
    switch (p->sub) {
        case 0:
            omVibrate(p->playerNo, 0x3C, 4, 2);
            p->subTimer = 0;
            p->sub++;
        case 1:
            p->subTimer++;
            if (p->subTimer == 0x3C) {
                done = 1;
            }
            if (p->motRate > 0.5f) {
                if (p->motRate - 0.05f < 0.5f) {
                    a0 = 0.5f;
                } else {
                    a0 = p->motRate - 0.05f;
                }
                r0 = a0;
            } else {
                if (0.05f + p->motRate > 0.5f) {
                    b0 = 0.5f;
                } else {
                    b0 = 0.05f + p->motRate;
                }
                r0 = b0;
            }
            p->motRate = r0;
            t = p->motMax * p->motRate;
            CharMotionTimeSet(p->charNo, t);
            Hu3DMotionTimeSet(p->sled->mSled, t);
            break;
    }
    return done;
}

void fn_1_B134(M577Work *w)
{
    f32 r;
    f32 r2;

    r = w->team[0].dist / w->course.value;
    if (r > 1.0f) {
        r = 1.0f;
    }
    espPosSet(w->esp[1], 276.0f, 400.0f * (1.0f - r) + 120.0f * r);
    r2 = w->team[1].dist / w->course.value;
    if (r2 > 1.0f) {
        r2 = 1.0f;
    }
    espPosSet(w->esp[2], 300.0f, 400.0f * (1.0f - r2) + 120.0f * r2);
}

void fn_1_B26C(M577Work *w)
{
    s16 mdl = w->mdlCache[13];
    f32 rot;
    f32 t;

    w->ringAngle += 10.0f;
    t = w->ringAngle / w->course.value;
    rot = 90.0f * t;
    Hu3DModelRotSet(mdl, rot, 0.0f, 0.0f);
}

s32 fn_1_B318(M577Node *node)
{
    M577Player *p;
    s32 done;

    done = 0;
    p = node->arg;
    p->timer--;
    if (p->timer == 0) {
        done = 1;
    }
    if ((p->timer & 1) != 0) {
        Hu3DModelAttrSet(p->sled->mSled, 1);
        Hu3DModelAttrSet(p->sled->mShadow, 1);
    } else {
        Hu3DModelAttrReset(p->sled->mSled, 1);
        Hu3DModelAttrReset(p->sled->mShadow, 1);
    }
    return done;
}

s32 fn_1_B3CC(M577Node *node)
{
    s16 *flag;

    flag = node->arg;
    if (*flag == 1) {
        HuAudFXPlayPan(0x942, 0x50);
        return 1;
    }
    *flag += 1;
    return 0;
}
