#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/init.h"
#include "game/memory.h"
#include "game/mg/seqman.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/wipe.h"

#include "math.h"

/*************************************************************
 * m567.c
 *
 * "567" overlay: minigame with a 1P vs 3P team layout. Most helpers
 * carry objdiff's placeholder names (fn_1_XXXX) keyed to their .text
 * offset.
 *************************************************************/

#ifdef __MWERKS__
#define M567_RODATA __declspec(section ".rodata")
#else
#define M567_RODATA
#endif

#define M567_MDL_NONE (-1)

/* one 8-byte record: material to blink + state machine of fn_1_98F0 */
typedef struct M567FxRec_s {
    /* 0x00 */ HSFMATERIAL *mat;
    /* 0x04 */ s16 state;
    /* 0x06 */ s16 timer;
} M567FXREC;

typedef struct M567Fx M567FX;

/* effect node pool entry (intrusive doubly linked list) */
typedef struct M567FxNode_s M567FXNODE;
typedef struct M567FxList_s M567FXLIST;

struct M567FxNode_s {
    /* 0x00 */ M567FXNODE *prev;
    /* 0x04 */ M567FXNODE *next;
    /* 0x08 */ M567FXLIST *list;
    /* 0x0C */ s32 (*func)(M567FXNODE *node);
    /* 0x10 */ void *arg;
};

struct M567FxList_s {
    /* 0x00 */ M567FXNODE *head;
    /* 0x04 */ M567FXNODE *free;
};

/* camera pose table entry */
typedef struct M567CamPose_s {
    Vec target;
    Vec rot;
} M567CAMPOSE;

/* camera orbit parameters (size 0x38) */
typedef struct M567Cam_s {
    /* 0x00 */ Vec target;
    /* 0x0C */ Vec rot; /* x/y angle, z = distance */
    /* 0x18 */ u8 pad18[0x1C];
    /* 0x34 */ s16 camBit;
    /* 0x36 */ s16 pad36;
} M567CAM;

/* horizontal screen split position */
typedef struct M567Split_s {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ s16 pad08;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ u8 pad0C[0x84 - 0xC];
} M567SPLIT;

typedef struct M567Group_s M567GROUP;

/* player work (obj->data of the 4 player objects), size 0x44 */
typedef struct M567Slot_s M567SLOT;
typedef struct M567Player_s {
    /* 0x00 */ Vec pos;
    /* 0x0C */ Vec rot;
    /* 0x18 */ OMOBJ *obj;
    /* 0x1C */ M567SLOT *slot;
    /* 0x20 */ M567GROUP *grp;
    /* 0x24 */ s16 mdl;
    /* 0x26 */ s16 no;
    /* 0x28 */ s16 charNo;
    /* 0x2A */ s16 grpNo;
    /* 0x2C */ s16 padNo;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 type;
    /* 0x34 */ s16 dif;
    /* 0x36 */ s16 idx;
    /* 0x38 */ s16 state;
    /* 0x3A */ s16 hookMdl;
    /* 0x3C */ s16 comState;
    /* 0x3E */ s16 comTimer;
    /* 0x40 */ s16 comWait;
    /* 0x42 */ s16 comChoice;
} M567PLAYER;

/* slot machine reel (size 0x20) */
struct M567Slot_s {
    /* 0x00 */ HSFMATERIAL *mat;
    /* 0x04 */ s16 state;
    /* 0x06 */ s16 timer;
    /* 0x08 */ s16 mdl;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 fxMdl;
    /* 0x10 */ s16 miniMdl;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 pos;
    /* 0x16 */ s16 maxPos;
    /* 0x18 */ s16 spin;
    /* 0x1A */ s16 target;
    /* 0x1C */ s16 unit;
    /* 0x1E */ s16 slotNo;
};

/* reel display of one group (size 0xD8) */
typedef struct M567Mover_s {
    /* 0x00 */ M567CAM *cam;
    /* 0x04 */ M567FXREC fx[3];
    /* 0x1C */ Vec scr;
    /* 0x28 */ Vec objPos[3];
    /* 0x4C */ Vec rot;
    /* 0x58 */ M567SLOT slot[3];
    /* 0xB8 */ s16 type;
    /* 0xBA */ s16 flag;
    /* 0xBC */ s16 mdl;
    /* 0xBE */ s16 mdlA[3];
    /* 0xC4 */ s16 mdlB[3];
    /* 0xCA */ s16 mdlC[3];
    /* 0xD0 */ s16 cur;
    /* 0xD2 */ s16 timer;
    /* 0xD4 */ s16 state;
} M567MOVER;

/* movable part of a group: position, models and camera (size 0x4C) */
typedef struct M567Body_s {
    /* 0x00 */ Vec pos;
    /* 0x0C */ Vec rot;
    /* 0x18 */ f32 f18;
    /* 0x1C */ f32 f1C;
    /* 0x20 */ f32 f20;
    /* 0x24 */ f32 f24;
    /* 0x28 */ f32 f28;
    /* 0x2C */ f32 f2C;
    /* 0x30 */ s16 mdl;
    /* 0x32 */ s16 mtn[4];
    /* 0x3A */ s16 miniMdl[3];
    /* 0x40 */ s16 pumMdl[3];
    /* 0x46 */ s16 fxMdl;
    /* 0x48 */ M567CAM *cam;
} M567BODY;

/* team layout (size 0x1B4): group 0 = 1P, group 1 = 3P */
struct M567Group_s {
    /* 0x000 */ M567PLAYER *player[3];
    /* 0x00C */ s16 map[11][5];
    /* 0x07A */ s16 dir[3];
    /* 0x080 */ M567BODY body;
    /* 0x0CC */ M567MOVER mover;
    /* 0x1A4 */ s16 kind;
    /* 0x1A6 */ s16 row;
    /* 0x1A8 */ s16 col;
    /* 0x1AA */ s16 nextRow;
    /* 0x1AC */ s16 nextCol;
    /* 0x1AE */ s16 state;
    /* 0x1B0 */ s16 timer;
    /* 0x1B2 */ s16 limit;
};

/* work block of the main object (size 0xD8C) */
typedef struct M567Work_s {
    /* 0x000 */ M567PLAYER *player[4];
    /* 0x010 */ M567GROUP grp[2];
    /* 0x378 */ M567FXLIST fx;
    /* 0x380 */ M567FXNODE fxNode[100];
    /* 0xB50 */ M567CAM cam[2];
    /* 0xBC0 */ M567SPLIT split;
    /* 0xC44 */ s16 mdlList[0x20];
    /* 0xC84 */ s16 mdlRowA[8][3];
    /* 0xCB4 */ s16 mdlRowB[8][3];
    /* 0xCE4 */ s16 mdlCE4[3];
    /* 0xCEA */ s16 winGrp;
    /* 0xCEC */ s16 step;
    /* 0xCEE */ s16 timer;
    /* 0xCF0 */ s16 subStep;
    /* 0xCF2 */ s16 frame;
    /* 0xCF4 */ s16 inputOn;
    /* 0xCF6 */ s16 moveOn;
    /* 0xCF8 */ s16 mdlCache[0x41];
    /* 0xD7A */ s16 mtnCache[9];
} M567WORK;

#ifdef __MWERKS__
typedef char M567WorkSizeCheck[(sizeof(M567WORK) == 0xD8C) ? 1 : -1];
#endif

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void ObjectSetup(void);

extern OMOBJ *lbl_1_bss_4;
s32 lbl_1_bss_0;
OMOBJ *lbl_1_bss_4;
OMOBJMAN *lbl_1_bss_8;

void fn_1_1FC(void);
void fn_1_32C(void);
void fn_1_FB8(void);
void fn_1_108C(void);
void fn_1_1160(void);
void fn_1_1474(void);
void fn_1_32CC(void);
void fn_1_362C(void);
void fn_1_371C(void);

MGSEQ_PARAM lbl_1_data_0 = {
    300, 0,
    (MGSEQ_FUNC)fn_1_1FC, (MGSEQ_FUNC)fn_1_32C, (MGSEQ_FUNC)fn_1_FB8,
    (MGSEQ_FUNC)fn_1_108C, (MGSEQ_FUNC)fn_1_1160, (MGSEQ_FUNC)fn_1_1474,
    (MGSEQ_FUNC)fn_1_32CC, (MGSEQ_FUNC)fn_1_362C, (MGSEQ_FUNC)fn_1_371C
};

u32 lbl_1_data_28[13][5] = {
    { 0x008C0000, 0x008C006A, 0x008C003F, 0x008C0006, 0x008C0017 },
    { 0x00330000, 0x0033006A, 0x0033003F, 0x00330006, 0x00330017 },
    { 0x00B70000, 0x00B7006A, 0x00B7003F, 0x00B70006, 0x00B70017 },
    { 0x00ED0000, 0x00ED006A, 0x00ED003F, 0x00ED0006, 0x00ED0017 },
    { 0x00E60000, 0x00E6006A, 0x00E6003F, 0x00E60006, 0x00E60017 },
    { 0x00170000, 0x0017006A, 0x0017003F, 0x00170006, 0x00170017 },
    { 0x00E00000, 0x00E0006A, 0x00E0003F, 0x00E00006, 0x00E00017 },
    { 0x002D0000, 0x002D006A, 0x002D003F, 0x002D0006, 0x002D0017 },
    { 0x00C90000, 0x00C9006A, 0x00C9003F, 0x00C90006, 0x00C90017 },
    { 0x00B00000, 0x00B0006A, 0x00B0003F, 0x00B00006, 0x00B00017 },
    { 0x00B00000, 0x00B0006A, 0x00B0003F, 0x00B00006, 0x00B00017 },
    { 0x00B00000, 0x00B0006A, 0x00B0003F, 0x00B00006, 0x00B00017 },
    { 0x00B00000, 0x00B0006A, 0x00B0003F, 0x00B00006, 0x00B00017 },
};
u32 lbl_1_data_12C[65] = {
    0x007A000B, 0x007A000C, 0x007A000D, 0x007A000E,
    0x007A000F, 0x007A0010, 0x007A0011, 0x007A0012,
    0x007A0013, 0x007A0014, 0x007A0015, 0x007A0016,
    0x007A0017, 0x007A0018, 0x007A0019, 0x007A001A,
    0x007A001B, 0x007A001C, 0x007A0009, 0x007A000A,
    0x007A001D, 0x007A001E, 0x007A001F, 0x007A0020,
    0x007A0021, 0x007A0022, 0x007A0023, 0x007A0024,
    0x007A0025, 0x007A0026, 0x007A0027, 0x007A0028,
    0x007A0029, 0x007A002A, 0x007A002B, 0x007A002C,
    0x007A002D, 0x007A002E, 0x007A002F, 0x007A0030,
    0x007A0031, 0x007A0032, 0x007A0033, 0x007A0034,
    0x007A0035, 0x007A0036, 0x007A0037, 0x007A0038,
    0x007A0039, 0x007A003A, 0x007A003B, 0x007A003C,
    0x007A003D, 0x007A003E, 0x007A003F, 0x007A0040,
    0x007A0041, 0x007A0042, 0x007A0043, 0x007A0044,
    0x007A0045, 0x007A0046, 0x007A0047, 0x007A0048,
    0x007A0049,
};
u32 lbl_1_data_230[9] = {
    0x007A0000, 0x007A0001, 0x007A0002, 0x007A0003, 0x007A0004, 0x007A0005, 0x007A0006, 0x007A0007, 0x007A0008,
};
M567CAMPOSE lbl_1_data_254[10] = {
    { { 0.0f, 0.0f, 2600.0f }, { 90.0f, 41.0f, 1890.0f } },
    { { 0.0f, 0.0f, -2150.0f }, { 270.0f, 41.0f, 1890.0f } },
    { { 0.0f, 0.0f, -2400.0f }, { 90.0f, 41.0f, 1400.0f } },
    { { 0.0f, 500.0f, 2150.0f }, { 90.0f, 50.0f, 1700.0f } },
    { { 0.0f, 0.0f, 2150.0f }, { 90.0f, 41.0f, 1890.0f } },
    { { 0.0f, 500.0f, -2150.0f }, { 270.0f, 50.0f, 1700.0f } },
    { { 0.0f, 0.0f, -2150.0f }, { 270.0f, 41.0f, 1890.0f } },
    { { 0.0f, 0.0f, 2000.0f }, { 90.0f, 31.0f, 1730.0f } },
    { { 0.0f, 0.0f, -2000.0f }, { 270.0f, 31.0f, 1730.0f } },
    { { 0.0f, 0.0f, -781.0f }, { 90.0f, 21.0f, 2120.0f } },
};
s16 lbl_1_data_344[5][24] = {
    { 2, 6, 7, 0, 5, 2, 3, 6, 0, 2, 0, 3, 1, 5, 7, 0, 2, 6, 1, 4, 5, 2, 1, 2 },
    { 0, 2, 7, 7, 6, 1, 5, 4, 2, 0, 7, 6, 5, 1, 4, 7, 5, 7, 3, 0, 2, 0, 7, 5 },
    { 7, 1, 4, 6, 5, 6, 0, 7, 0, 4, 6, 2, 3, 2, 7, 2, 4, 6, 1, 0, 2, 6, 3, 0 },
    { 1, 0, 5, 3, 7, 3, 0, 2, 6, 5, 1, 2, 7, 3, 6, 6, 4, 5, 7, 2, 1, 3, 5, 3 },
    { 4, 6, 4, 5, 3, 7, 0, 1, 5, 7, 4, 0, 6, 5, 6, 4, 2, 4, 1, 0, 2, 0, 6, 3 },
};
s32 lbl_1_data_434[2] = { -1, -1 };
s32 lbl_1_data_43C[2] = { -1, -1 };

M567_RODATA const u16 lbl_1_rodata_10[4] = { 1, 2, 4, 8 };

void fn_1_3758(OMOBJ *obj);
void fn_1_3908(OMOBJ *obj);
void fn_1_3994(OMOBJ *obj);
void fn_1_39F8(OMOBJ *obj);
void fn_1_3B78(M567WORK *work);
void fn_1_3BF8(void);
void fn_1_3C80(M567WORK *work);
void fn_1_3E94(M567SPLIT *split);
void fn_1_3EEC(M567SPLIT *split);
void fn_1_4164(M567SPLIT *split);
void fn_1_4168(M567WORK *work);
void fn_1_42A8(M567WORK *work, s32 grpNo);
void fn_1_439C(M567CAM *cam, s16 camBit);
void fn_1_4404(M567CAM *cam, Vec *eye);
void fn_1_4680(M567FXLIST *list, M567FXNODE *nodes, s32 count);
M567FXNODE *fn_1_4704(M567FXLIST *list, s32 (*func)(M567FXNODE *), void *arg);
void fn_1_4758(M567FXNODE *node);
void fn_1_47D0(M567FXLIST *list);
void fn_1_48B0(M567WORK *work);
s32 fn_1_5F04(s32 prev);
s32 fn_1_5FC0(s32 prev);
void fn_1_5BD4(s16 map[11][5]);
void fn_1_6094(s16 dst[12][5], s16 src[12][5]);
void fn_1_60F8(M567WORK *work);
void fn_1_69B4(M567BODY *body, s32 grpNo);
s32 fn_1_7084(M567GROUP *grp);
void fn_1_7DB4(M567MOVER *mover, M567CAM *cam, s16 grpNo);
void fn_1_8330(M567MOVER *mover, Vec *eye);
void fn_1_8794(M567SLOT *slot, s32 grpNo);
void fn_1_89F4(M567SLOT *slot);
void fn_1_8DE0(M567SLOT *slot);
void fn_1_8EC8(M567PLAYER *player);
void fn_1_8F1C(M567PLAYER *player);
void fn_1_92B0(M567PLAYER *player);
s32 fn_1_9470(M567FXNODE *node);
void fn_1_959C(s16 map[][5], s16 row, s16 col);
s32 fn_1_9694(s16 map[][5], s16 row, s16 col);
s32 fn_1_979C(M567FXNODE *node);
s32 fn_1_9828(M567FXNODE *node);
s32 fn_1_988C(M567FXNODE *node);
s32 fn_1_98F0(M567FXNODE *node);

#define M567_EASE(t, r, timer, n) \
    do { \
        (r) = (f32)(timer) / (n); \
        (t) = 1.0 - (0.5 + 0.5 * cos(3.141592653589793 * (180.0f * (r)) / 180.0)); \
    } while (0)

int _prolog(void) {
    const VoidFunc *ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void) {
    const VoidFunc *dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void) {
    OMOBJ *obj;
    M567WORK *work;
    s32 i;

    lbl_1_bss_8 = omInitObjMan(10, 0x2000);
    omGameSysInit(lbl_1_bss_8);
    obj = omAddObjEx(lbl_1_bss_8, 0x65, 0, 0, -1, fn_1_3994);
    lbl_1_bss_4 = obj;
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M567WORK), HU_MEMNUM_OVL);
    work = obj->data;
    work->inputOn = 0;
    work->moveOn = 0;
    fn_1_3C80(work);
    fn_1_3BF8();
    for (i = 0; i < 4; i++) {
        obj = omAddObjEx(lbl_1_bss_8, 0x64, 1, 5, 0, fn_1_3758);
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M567PLAYER), HU_MEMNUM_OVL);
        obj->work[0] = i;
        work->player[i] = obj->data;
    }
    fn_1_3B78(work);
    Hu3DBGColorSet(0, 0, 0);
    Hu3DNoSyncSet(1);
    MgSeqCreate(&lbl_1_data_0);
}

void fn_1_1FC(void) {
    M567WORK *work = lbl_1_bss_4->data;

    work->inputOn = 0;
    work->moveOn = 0;
    work->step = 0;
    work->frame = 0;
    fn_1_60F8(work);
    fn_1_48B0(work);
    fn_1_3E94(&work->split);
    work->cam[0].rot.x = lbl_1_data_254[2].rot.x;
    work->cam[0].rot.y = lbl_1_data_254[2].rot.y;
    work->cam[0].rot.z = lbl_1_data_254[2].rot.z;
    work->cam[0].target.x = lbl_1_data_254[2].target.x;
    work->cam[0].target.y = lbl_1_data_254[2].target.y;
    work->cam[0].target.z = lbl_1_data_254[2].target.z;
    work->cam[1].rot.x = lbl_1_data_254[5].rot.x;
    work->cam[1].rot.y = lbl_1_data_254[5].rot.y;
    work->cam[1].rot.z = lbl_1_data_254[5].rot.z;
    work->cam[1].target.x = lbl_1_data_254[5].target.x;
    work->cam[1].target.y = lbl_1_data_254[5].target.y;
    work->cam[1].target.z = lbl_1_data_254[5].target.z;
    MgSeqModeNext();
}

void fn_1_32C(void) {
    M567WORK *work = lbl_1_bss_4->data;
    f32 t;
    f32 r;
    f32 inv1;
    f32 inv2;
    f32 inv3;
    f32 inv4;
    f32 inv5;

    switch (work->step) {
        case 0:
            MgSeqModeChangeOff();
            HuAudFXPlay(0x8FD);
            work->subStep = 0;
            work->timer = 0;
            work->step++;
        case 1:
            work->timer++;
            if (work->timer == 150) {
                work->step++;
            }
            M567_EASE(t, r, work->timer, 150.0f);
            inv1 = 1.0f - t;
            work->cam[0].rot.x = lbl_1_data_254[2].rot.x * inv1 + lbl_1_data_254[3].rot.x * t;
            work->cam[0].rot.y = lbl_1_data_254[2].rot.y * inv1 + lbl_1_data_254[3].rot.y * t;
            work->cam[0].rot.z = lbl_1_data_254[2].rot.z * inv1 + lbl_1_data_254[3].rot.z * t;
            work->cam[0].target.x = lbl_1_data_254[2].target.x * inv1 + lbl_1_data_254[3].target.x * t;
            work->cam[0].target.y = lbl_1_data_254[2].target.y * inv1 + lbl_1_data_254[3].target.y * t;
            work->cam[0].target.z = lbl_1_data_254[2].target.z * inv1 + lbl_1_data_254[3].target.z * t;
            break;
        case 2:
            work->timer = 0;
            work->step++;
        case 3:
            work->timer++;
            if (work->timer == 30) {
                work->step++;
            }
            t = (f32)work->timer / 30.0f;
            work->split.x = 640.0f * (1.0f - t) + 320.0f * t;
            break;
        case 4:
            HuAudFXPlay(0x8EC);
            Hu3DModelAttrSet(work->mdlCache[0xC], 1);
            work->timer = 0;
            work->step++;
        case 5:
            work->timer++;
            if (work->timer == 15) {
                HuAudFXPlay(0x8FE);
                Hu3DModelAttrReset(work->grp[0].mover.mdlC[0], 0x40000002);
                Hu3DModelAttrReset(work->grp[1].mover.mdlC[0], 0x40000002);
                Hu3DModelAttrReset(work->grp[1].mover.mdlC[1], 0x40000002);
                Hu3DModelAttrReset(work->grp[1].mover.mdlC[2], 0x40000002);
                work->step++;
            }
            M567_EASE(t, r, work->timer, 15.0f);
            work->grp[0].mover.scr.y = 500.0f * (1.0f - t) + 401.0f * t;
            work->grp[1].mover.scr.y = 500.0f * (1.0f - t) + 401.0f * t;
            M567_EASE(t, r, work->timer, 240.0f);
            inv2 = 1.0f - t;
            work->cam[0].rot.x = lbl_1_data_254[3].rot.x * inv2 + lbl_1_data_254[4].rot.x * t;
            work->cam[0].rot.y = lbl_1_data_254[3].rot.y * inv2 + lbl_1_data_254[4].rot.y * t;
            work->cam[0].rot.z = lbl_1_data_254[3].rot.z * inv2 + lbl_1_data_254[4].rot.z * t;
            work->cam[0].target.x = lbl_1_data_254[3].target.x * inv2 + lbl_1_data_254[4].target.x * t;
            work->cam[0].target.y = lbl_1_data_254[3].target.y * inv2 + lbl_1_data_254[4].target.y * t;
            work->cam[0].target.z = lbl_1_data_254[3].target.z * inv2 + lbl_1_data_254[4].target.z * t;
            inv3 = 1.0f - t;
            work->cam[1].rot.x = lbl_1_data_254[5].rot.x * inv3 + lbl_1_data_254[6].rot.x * t;
            work->cam[1].rot.y = lbl_1_data_254[5].rot.y * inv3 + lbl_1_data_254[6].rot.y * t;
            work->cam[1].rot.z = lbl_1_data_254[5].rot.z * inv3 + lbl_1_data_254[6].rot.z * t;
            work->cam[1].target.x = lbl_1_data_254[5].target.x * inv3 + lbl_1_data_254[6].target.x * t;
            work->cam[1].target.y = lbl_1_data_254[5].target.y * inv3 + lbl_1_data_254[6].target.y * t;
            work->cam[1].target.z = lbl_1_data_254[5].target.z * inv3 + lbl_1_data_254[6].target.z * t;
            break;
        case 6:
            work->timer++;
            if (work->timer == 0x23) {
                Hu3DModelAttrSet(work->grp[0].mover.mdlC[0], 0x40000002);
                Hu3DModelAttrSet(work->grp[1].mover.mdlC[0], 0x40000002);
                Hu3DModelAttrSet(work->grp[1].mover.mdlC[1], 0x40000002);
                Hu3DModelAttrSet(work->grp[1].mover.mdlC[2], 0x40000002);
            }
            if (work->timer == 0xAF) {
                Hu3DModelAttrReset(work->grp[0].mover.mdlC[0], 0x40000002);
                Hu3DModelAttrReset(work->grp[1].mover.mdlC[0], 0x40000002);
            }
            if (work->timer == 0xB9) {
                Hu3DModelAttrReset(work->grp[1].mover.mdlC[1], 0x40000002);
            }
            if (work->timer == 0xC3) {
                Hu3DModelAttrReset(work->grp[1].mover.mdlC[2], 0x40000002);
            }
            if (work->timer == 0xBE) {
                HuAudFXPlayPan(0x8ED, 0x30);
                HuAudFXPlayPan(0x8EE, 0x48);
                Hu3DModelAttrReset(work->grp[0].mover.mdlA[0], 0x40000002);
                Hu3DModelAttrReset(work->grp[1].mover.mdlA[0], 0x40000002);
                fn_1_4704(&work->fx, fn_1_98F0, &work->grp[0].mover.fx[0]);
                work->grp[0].mover.fx[0].state = 2;
                work->grp[0].mover.fx[0].timer = 0xF;
                fn_1_4704(&work->fx, fn_1_98F0, &work->grp[0].mover.fx[1]);
                work->grp[0].mover.fx[1].state = 2;
                work->grp[0].mover.fx[1].timer = 0xF;
                fn_1_4704(&work->fx, fn_1_98F0, &work->grp[1].mover.fx[0]);
                work->grp[1].mover.fx[0].state = 2;
                work->grp[1].mover.fx[0].timer = 0xF;
            }
            if (work->timer == 0xC8) {
                HuAudFXPlayPan(0x8EF, 0x50);
                Hu3DModelAttrReset(work->grp[1].mover.mdlA[1], 0x40000002);
                fn_1_4704(&work->fx, fn_1_98F0, &work->grp[1].mover.fx[1]);
                work->grp[1].mover.fx[1].state = 2;
                work->grp[1].mover.fx[1].timer = 0xF;
            }
            if (work->timer == 0xD2) {
                HuAudFXPlayPan(0x8F0, 0x58);
                Hu3DModelAttrReset(work->grp[1].mover.mdlA[2], 0x40000002);
                fn_1_4704(&work->fx, fn_1_98F0, &work->grp[1].mover.fx[2]);
                work->grp[1].mover.fx[2].state = 2;
                work->grp[1].mover.fx[2].timer = 0xF;
            }
            if (work->timer == 0xF0) {
                work->step++;
            }
            M567_EASE(t, r, work->timer, 240.0f);
            inv4 = 1.0f - t;
            work->cam[0].rot.x = lbl_1_data_254[3].rot.x * inv4 + lbl_1_data_254[4].rot.x * t;
            work->cam[0].rot.y = lbl_1_data_254[3].rot.y * inv4 + lbl_1_data_254[4].rot.y * t;
            work->cam[0].rot.z = lbl_1_data_254[3].rot.z * inv4 + lbl_1_data_254[4].rot.z * t;
            work->cam[0].target.x = lbl_1_data_254[3].target.x * inv4 + lbl_1_data_254[4].target.x * t;
            work->cam[0].target.y = lbl_1_data_254[3].target.y * inv4 + lbl_1_data_254[4].target.y * t;
            work->cam[0].target.z = lbl_1_data_254[3].target.z * inv4 + lbl_1_data_254[4].target.z * t;
            inv5 = 1.0f - t;
            work->cam[1].rot.x = lbl_1_data_254[5].rot.x * inv5 + lbl_1_data_254[6].rot.x * t;
            work->cam[1].rot.y = lbl_1_data_254[5].rot.y * inv5 + lbl_1_data_254[6].rot.y * t;
            work->cam[1].rot.z = lbl_1_data_254[5].rot.z * inv5 + lbl_1_data_254[6].rot.z * t;
            work->cam[1].target.x = lbl_1_data_254[5].target.x * inv5 + lbl_1_data_254[6].target.x * t;
            work->cam[1].target.y = lbl_1_data_254[5].target.y * inv5 + lbl_1_data_254[6].target.y * t;
            work->cam[1].target.z = lbl_1_data_254[5].target.z * inv5 + lbl_1_data_254[6].target.z * t;
            break;
        case 7:
            lbl_1_bss_0 = HuAudBGMPlay(0x34);
            MgSeqModeNext();
            break;
    }
}

void fn_1_FB8(void) {
    M567WORK *work = lbl_1_bss_4->data;

    switch (work->subStep) {
        case 0:
            Hu3DModelAttrSet(work->grp[0].mover.mdlA[0], 1);
            Hu3DModelAttrSet(work->grp[1].mover.mdlA[0], 1);
            Hu3DModelAttrSet(work->grp[1].mover.mdlA[1], 1);
            Hu3DModelAttrSet(work->grp[1].mover.mdlA[2], 1);
            Hu3DModelAttrSet(work->grp[0].mover.mdlC[0], 1);
            Hu3DModelAttrSet(work->grp[1].mover.mdlC[0], 1);
            Hu3DModelAttrSet(work->grp[1].mover.mdlC[1], 1);
            Hu3DModelAttrSet(work->grp[1].mover.mdlC[2], 1);
            work->grp[0].mover.flag = 1;
            work->grp[1].mover.flag = 1;
            work->step = 0;
            work->subStep++;
        case 1:
            break;
    }
}

void fn_1_108C(void) {
    M567WORK *work = lbl_1_bss_4->data;

    switch (work->step) {
        case 0:
            work->grp[0].state = 1;
            work->grp[1].state = 1;
            work->subStep = 0;
            work->inputOn = 1;
            work->moveOn = 1;
            work->step++;
        case 1:
            if (work->winGrp >= 0) {
                work->inputOn = 0;
                MgSeqModeChangeOff();
                work->step++;
            }
            break;
        case 2:
            if (work->grp[work->winGrp].state == 7) {
                MgSeqModeNext();
            }
            break;
    }
}

void fn_1_1160(void) {
    M567WORK *work = lbl_1_bss_4->data;
    f32 t;
    f32 r;

    switch (work->subStep) {
        case 0:
            if (work->grp[0].state != 4 && work->grp[0].state != 7 && work->grp[0].state != 8) {
                work->grp[0].state = 7;
            }
            if (work->grp[1].state != 4 && work->grp[1].state != 7 && work->grp[1].state != 8) {
                work->grp[1].state = 7;
            }
            if (work->winGrp < 0) {
                if (work->grp[0].nextRow == 9) {
                    work->grp[0].nextRow = work->grp[0].row;
                    work->grp[0].body.f2C = work->grp[0].body.f20;
                }
                if (work->grp[1].nextRow == 9) {
                    work->grp[1].nextRow = work->grp[1].row;
                    work->grp[1].body.f2C = work->grp[1].body.f20;
                }
                MgSeqWinnerSet(-1, -1, -1, -1);
            } else if (work->winGrp == 0) {
                MgSeqWinnerSet(work->grp[0].player[0]->charNo, -1, -1, -1);
            } else {
                MgSeqWinnerSet(work->grp[1].player[0]->charNo, work->grp[1].player[1]->charNo, work->grp[1].player[2]->charNo, -1);
            }
            work->step = 0;
            work->inputOn = 0;
            work->moveOn = 0;
            work->timer = 0;
            HuAudSStreamFadeOut(lbl_1_bss_0, 1000);
            work->subStep++;
        case 1:
            work->timer++;
            if (work->timer == 15) {
                work->subStep++;
            }
            M567_EASE(t, r, work->timer, 15.0f);
            work->grp[0].mover.scr.y = 401.0f * (1.0f - t) + 500.0f * t;
            work->grp[1].mover.scr.y = 401.0f * (1.0f - t) + 500.0f * t;
            break;
        case 2:
            Hu3DModelAttrSet(work->grp[0].mover.mdl, 1);
            Hu3DModelAttrSet(work->grp[1].mover.mdl, 1);
            work->subStep++;
            break;
        case 3:
            break;
    }
}

#define M567_PLAYER_POS(p)                                      \
    do {                                                        \
        Hu3DModelPosSetV((p)->mdl, &(p)->pos);                  \
        omSetTra((p)->obj, (p)->pos.x, (p)->pos.y, (p)->pos.z); \
    } while (0)

#define M567_PLAYER_ROT(p)                                      \
    do {                                                        \
        Hu3DModelRotSetV((p)->mdl, &(p)->rot);                  \
        omSetRot((p)->obj, (p)->rot.x, (p)->rot.y, (p)->rot.z); \
    } while (0)

#define M567_PLAYER_SET(p, px, py, pz, rx, ry, rz) \
    do {                                           \
        (p)->pos.x = (px);                         \
        (p)->pos.y = (py);                         \
        (p)->pos.z = (pz);                         \
        (p)->rot.x = (rx);                         \
        (p)->rot.y = (ry);                         \
        (p)->rot.z = (rz);                         \
    } while (0)

#define M567_HOOK_GET(g, p, name)                  \
    do {                                           \
        Hu3DModelObjMtxGet((g)->body.mdl, (name), mtx); \
        Hu3DMtxTransGet(mtx, &(p)->pos);           \
        Hu3DMtxRotGet(mtx, &(p)->rot);             \
        M567_PLAYER_POS(p);                        \
        M567_PLAYER_ROT(p);                        \
        Hu3DModelHookObjReset((g)->body.mdl, (name));   \
    } while (0)

#define M567_FLY(w)                                 \
    do {                                            \
        if ((w)->winGrp != 0) {                     \
            (w)->grp[0].player[0]->pos.y += 50.0f;  \
            M567_PLAYER_POS((w)->grp[0].player[0]); \
        }                                           \
        if ((w)->winGrp != 1) {                     \
            (w)->grp[1].player[0]->pos.y += 50.0f;  \
            (w)->grp[1].player[0]->pos.x += 5.0f;   \
            M567_PLAYER_POS((w)->grp[1].player[0]); \
            (w)->grp[1].player[1]->pos.y += 50.0f;  \
            M567_PLAYER_POS((w)->grp[1].player[1]); \
            (w)->grp[1].player[2]->pos.y += 50.0f;  \
            (w)->grp[1].player[2]->pos.x -= 5.0f;   \
            M567_PLAYER_POS((w)->grp[1].player[2]); \
        }                                           \
    } while (0)

#define M567_FIRE_FX(w, b)                                              do {                                                                    fn_1_4704(&(w)->fx, fn_1_988C, (b));                                Hu3DModelAttrReset((b)->fxMdl, 1);                                  Hu3DModelAttrReset((b)->fxMdl, 0x40000002);                         Hu3DMotionTimeSet((b)->fxMdl, 0.0f);                                Hu3DModelPosSet((b)->fxMdl, (b)->pos.x, (b)->pos.y, (b)->pos.z);         if ((w)->winGrp == 0) {                                                 lbl_1_data_43C[0] = HuAudFXPlay(0x8F6);                         }                                                                   if ((w)->winGrp == 1) {                                                 lbl_1_data_43C[1] = HuAudFXPlay(0x8F8);                         }                                                               } while (0)

void fn_1_1474(void) {
    s16 pan[3] = { 0x20, 0x40, 0x60 };
    Mtx mtx;
    M567WORK *work = lbl_1_bss_4->data;
    M567BODY *body;
    f32 time;
    s32 i;
    s32 j;

    switch (work->step) {
        case 0:
            MgSeqModeChangeOff();
            work->subStep = 0;
            work->timer = 0;
            work->step++;
            break;
        case 1:
            if (work->winGrp < 0) {
                Hu3DMotionShiftSet(work->grp[0].body.mdl, work->grp[0].body.mtn[2], 0.0f, 5.0f, 0);
                Hu3DMotionShiftSet(work->grp[1].body.mdl, work->grp[1].body.mtn[2], 0.0f, 5.0f, 0);
            } else {
                Hu3DMotionShiftSet(work->grp[work->winGrp ^ 1].body.mdl, work->grp[work->winGrp ^ 1].body.mtn[2], 0.0f, 5.0f, 0);
            }
            work->step++;
            break;
        case 2:
            work->timer++;
            if (work->timer == 0x28) {
                if (work->winGrp != 0) {
                    M567_HOOK_GET(&work->grp[0], work->grp[0].player[0], "m567-play0");
                    omVibrate(work->grp[0].player[0]->no, 0x19, 0xC, 0);
                    HuAudFXPlayPan(0x8FB, pan[work->grp[0].nextCol - 1]);
                }
                if (work->winGrp != 1) {
                    M567_HOOK_GET(&work->grp[1], work->grp[1].player[0], "m567-play3");
                    M567_HOOK_GET(&work->grp[1], work->grp[1].player[1], "m567-play1");
                    M567_HOOK_GET(&work->grp[1], work->grp[1].player[2], "m567-play2");
                    omVibrate(work->grp[1].player[0]->no, 0x23, 0xC, 0);
                    omVibrate(work->grp[1].player[1]->no, 0x23, 0xC, 0);
                    omVibrate(work->grp[1].player[2]->no, 0x23, 0xC, 0);
                    HuAudFXPlayPan(0x8FC, pan[work->grp[1].nextCol - 1]);
                }
                work->step++;
            }
            break;
        case 3:
            work->timer++;
            if (work->timer == 0x32) {
                work->step++;
            }
            M567_FLY(work);
            break;
        case 4:
            WipeColorSet(0xFF, 0xFF, 0xFF);
            WipeCreate(2, 0, 0x3C);
            work->step++;
            M567_FLY(work);
            break;
        case 5:
            M567_FLY(work);
            if (WipeCheckEnd() == 0) {
                work->step++;
            }
            break;
        case 6:
            for (i = 0; i < 0x20; i++) {
                if (work->mdlList[i] >= 0) {
                    Hu3DModelAttrSet(work->mdlList[i], 1);
                }
            }
            if (work->winGrp != 0) {
                Hu3DModelAttrSet(work->grp[0].body.mdl, 1);
                Hu3DModelAttrReset(work->mdlCache[0x3F], 1);
                CharMotionSet(work->grp[0].player[0]->charNo, work->grp[0].player[0]->obj->mtnId[4]);
                CharModelAttrReset(work->grp[0].player[0]->charNo, 0x40000002);
                CharModelAttrSet(work->grp[0].player[0]->charNo, 0x40000001);
            }
            if (work->winGrp != 1) {
                Hu3DModelAttrSet(work->grp[1].body.mdl, 1);
                Hu3DModelAttrReset(work->mdlCache[0x40], 1);
                CharMotionSet(work->grp[1].player[0]->charNo, work->grp[1].player[0]->obj->mtnId[4]);
                CharMotionSet(work->grp[1].player[1]->charNo, work->grp[1].player[1]->obj->mtnId[4]);
                CharMotionSet(work->grp[1].player[2]->charNo, work->grp[1].player[2]->obj->mtnId[4]);
                CharModelAttrReset(work->grp[1].player[0]->charNo, 0x40000002);
                CharModelAttrReset(work->grp[1].player[1]->charNo, 0x40000002);
                CharModelAttrReset(work->grp[1].player[2]->charNo, 0x40000002);
                CharModelAttrSet(work->grp[1].player[0]->charNo, 0x40000001);
                CharModelAttrSet(work->grp[1].player[1]->charNo, 0x40000001);
                CharModelAttrSet(work->grp[1].player[2]->charNo, 0x40000001);
            }
            if (work->winGrp >= 0) {
                work->split.x = 640.0f;
                fn_1_42A8(work, work->winGrp);
                work->cam[0].rot.x = lbl_1_data_254[9].rot.x;
                work->cam[0].rot.y = lbl_1_data_254[9].rot.y;
                work->cam[0].rot.z = lbl_1_data_254[9].rot.z;
                work->cam[0].target.x = lbl_1_data_254[9].target.x;
                work->cam[0].target.y = lbl_1_data_254[9].target.y;
                work->cam[0].target.z = lbl_1_data_254[9].target.z;
                Hu3DModelAttrReset(work->mdlCache[0xB], 1);
                Hu3DModelCameraSet(work->grp[0].player[0]->mdl, lbl_1_rodata_10[0]);
                Hu3DModelCameraSet(work->grp[1].player[0]->mdl, lbl_1_rodata_10[0]);
                Hu3DModelCameraSet(work->grp[1].player[1]->mdl, lbl_1_rodata_10[0]);
                Hu3DModelCameraSet(work->grp[1].player[2]->mdl, lbl_1_rodata_10[0]);
                for (i = 0; i < 7; i++) {
                    for (j = 0; j < 3; j++) {
                        Hu3DModelAttrSet(work->mdlRowA[i][j], 1);
                        Hu3DModelCameraSet(work->mdlRowB[i][j], lbl_1_rodata_10[0]);
                    }
                }
                Hu3DModelCameraSet(work->mdlRowA[7][0], lbl_1_rodata_10[0]);
                Hu3DModelCameraSet(work->mdlRowA[7][1], lbl_1_rodata_10[0]);
                Hu3DModelCameraSet(work->mdlRowA[7][2], lbl_1_rodata_10[0]);
                Hu3DModelAttrReset(work->mdlCache[8], 1);
                Hu3DModelAttrSet(work->mdlCache[9], 1);
                work->grp[work->winGrp].body.pos.x = 0.0f;
                work->grp[work->winGrp].body.pos.y = 0.0f;
                work->grp[work->winGrp].body.pos.z = -300.0f;
                work->grp[work->winGrp].body.rot.x = 0.0f;
                work->grp[work->winGrp].body.rot.y = 0.0f;
                work->grp[work->winGrp].body.rot.z = 0.0f;
                work->grp[work->winGrp ^ 1].body.pos.x = 150.0f;
                work->grp[work->winGrp ^ 1].body.pos.y = 0.0f;
                work->grp[work->winGrp ^ 1].body.pos.z = -900.0f;
                work->grp[work->winGrp ^ 1].body.rot.x = 0.0f;
                work->grp[work->winGrp ^ 1].body.rot.y = 0.0f;
                work->grp[work->winGrp ^ 1].body.rot.z = 0.0f;
                Hu3DModelPosSetV(work->grp[work->winGrp].body.mdl, &work->grp[work->winGrp].body.pos);
                Hu3DModelRotSetV(work->grp[work->winGrp].body.mdl, &work->grp[work->winGrp].body.rot);
                for (i = 0; i < 3; i++) {
                    Hu3DModelShadowMapSet(work->mdlRowB[7][i]);
                    Hu3DModelShadowMapSet(work->mdlCE4[i]);
                }
                work->grp[work->winGrp].body.fxMdl = work->mdlCache[0x1D];
                if (work->winGrp == 0) {
                    Hu3DModelHookObjReset(work->grp[0].body.mdl, "m567-play0");
                    M567_PLAYER_SET(work->grp[0].player[0], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                    M567_PLAYER_SET(work->grp[1].player[0], -300.0f, 0.0f, -200.0f, 0.0f, 0.0f, 0.0f);
                    M567_PLAYER_SET(work->grp[1].player[1], 225.0f, 0.0f, -200.0f, 0.0f, 0.0f, 0.0f);
                    M567_PLAYER_SET(work->grp[1].player[2], 375.0f, 0.0f, -200.0f, 0.0f, 0.0f, 0.0f);
                    Hu3DModelPosSetV(work->mdlCache[0x40], &work->grp[1].body.pos);
                    Hu3DModelRotSetV(work->mdlCache[0x40], &work->grp[1].body.rot);
                    CharMotionSet(work->grp[0].player[0]->charNo, work->grp[0].player[0]->obj->mtnId[3]);
                    Hu3DMotionSet(work->grp[0].body.mdl, work->grp[0].body.mtn[3]);
                    Hu3DModelAttrSet(work->grp[0].body.mdl, 0x40000001);
                    Hu3DModelCameraSet(work->grp[0].body.mdl, lbl_1_rodata_10[0]);
                } else {
                    Hu3DModelHookObjReset(work->grp[1].body.mdl, "m567-play3");
                    Hu3DModelHookObjReset(work->grp[1].body.mdl, "m567-play1");
                    Hu3DModelHookObjReset(work->grp[1].body.mdl, "m567-play2");
                    M567_PLAYER_SET(work->grp[0].player[0], -300.0f, 0.0f, -200.0f, 0.0f, 0.0f, 0.0f);
                    M567_PLAYER_SET(work->grp[1].player[0], -200.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                    M567_PLAYER_SET(work->grp[1].player[1], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                    M567_PLAYER_SET(work->grp[1].player[2], 200.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                    Hu3DModelPosSetV(work->mdlCache[0x3F], &work->grp[0].body.pos);
                    Hu3DModelRotSetV(work->mdlCache[0x3F], &work->grp[0].body.rot);
                    CharMotionSet(work->grp[1].player[0]->charNo, work->grp[1].player[0]->obj->mtnId[3]);
                    CharMotionSet(work->grp[1].player[1]->charNo, work->grp[1].player[1]->obj->mtnId[3]);
                    CharMotionSet(work->grp[1].player[2]->charNo, work->grp[1].player[2]->obj->mtnId[3]);
                    Hu3DModelRotSet(work->mdlCache[0x17], 0.0f, 180.0f, 0.0f);
                    Hu3DModelRotSet(work->mdlCE4[0], 0.0f, 180.0f, 0.0f);
                    Hu3DModelRotSet(work->mdlCE4[1], 0.0f, 180.0f, 0.0f);
                    Hu3DModelRotSet(work->mdlCE4[2], 0.0f, 180.0f, 0.0f);
                    Hu3DMotionSet(work->grp[1].body.mdl, work->grp[1].body.mtn[3]);
                    Hu3DModelAttrSet(work->grp[1].body.mdl, 0x40000001);
                    Hu3DModelCameraSet(work->grp[1].body.mdl, lbl_1_rodata_10[0]);
                }
            } else {
                fn_1_4168(work);
                work->grp[0].body.pos.x = 0.0f;
                work->grp[0].body.pos.y = 0.0f;
                work->grp[0].body.pos.z = 2100.0f;
                work->grp[0].body.rot.x = 0.0f;
                work->grp[0].body.rot.y = 0.0f;
                work->grp[0].body.rot.z = 0.0f;
                M567_PLAYER_SET(work->grp[0].player[0], 0.0f, 0.0f, 2400.0f, 0.0f, 0.0f, 0.0f);
                work->grp[1].body.pos.x = 0.0f;
                work->grp[1].body.pos.y = 0.0f;
                work->grp[1].body.pos.z = -2100.0f;
                work->grp[1].body.rot.x = 0.0f;
                work->grp[1].body.rot.y = 180.0f;
                work->grp[1].body.rot.z = 0.0f;
                M567_PLAYER_SET(work->grp[1].player[0], 200.0f, 0.0f, -2400.0f, 0.0f, 180.0f, 0.0f);
                M567_PLAYER_SET(work->grp[1].player[1], 0.0f, 0.0f, -2400.0f, 0.0f, 180.0f, 0.0f);
                M567_PLAYER_SET(work->grp[1].player[2], -200.0f, 0.0f, -2400.0f, 0.0f, 180.0f, 0.0f);
                work->cam[0].rot.x = lbl_1_data_254[7].rot.x;
                work->cam[0].rot.y = lbl_1_data_254[7].rot.y;
                work->cam[0].rot.z = lbl_1_data_254[7].rot.z;
                work->cam[0].target.x = lbl_1_data_254[7].target.x;
                work->cam[0].target.y = lbl_1_data_254[7].target.y;
                work->cam[0].target.z = lbl_1_data_254[7].target.z;
                work->cam[1].rot.x = lbl_1_data_254[8].rot.x;
                work->cam[1].rot.y = lbl_1_data_254[8].rot.y;
                work->cam[1].rot.z = lbl_1_data_254[8].rot.z;
                work->cam[1].target.x = lbl_1_data_254[8].target.x;
                work->cam[1].target.y = lbl_1_data_254[8].target.y;
                work->cam[1].target.z = lbl_1_data_254[8].target.z;
                Hu3DModelCameraSet(work->grp[0].player[0]->mdl, lbl_1_rodata_10[0]);
                Hu3DModelCameraSet(work->grp[1].player[0]->mdl, lbl_1_rodata_10[1]);
                Hu3DModelCameraSet(work->grp[1].player[1]->mdl, lbl_1_rodata_10[1]);
                Hu3DModelCameraSet(work->grp[1].player[2]->mdl, lbl_1_rodata_10[1]);
                Hu3DModelPosSetV(work->mdlCache[0x3F], &work->grp[0].body.pos);
                Hu3DModelPosSetV(work->mdlCache[0x40], &work->grp[1].body.pos);
                Hu3DModelRotSetV(work->mdlCache[0x3F], &work->grp[0].body.rot);
                Hu3DModelRotSetV(work->mdlCache[0x40], &work->grp[1].body.rot);
                for (i = 0; i < 3; i++) {
                    Hu3DModelCameraSet(work->mdlRowA[0][i], lbl_1_rodata_10[0]);
                    Hu3DModelShadowMapSet(work->mdlRowA[0][i]);
                    Hu3DModelCameraSet(work->mdlRowB[0][i], lbl_1_rodata_10[1]);
                    Hu3DModelShadowMapSet(work->mdlRowB[0][i]);
                }
                for (i = 0; i < 8; i++) {
                    for (j = 0; j < 3; j++) {
                        Hu3DModelAttrReset(work->mdlRowA[i][j], 1);
                        Hu3DModelAttrReset(work->mdlRowB[i][j], 1);
                    }
                }
            }
            M567_PLAYER_POS(work->grp[0].player[0]);
            M567_PLAYER_ROT(work->grp[0].player[0]);
            M567_PLAYER_POS(work->grp[1].player[0]);
            M567_PLAYER_ROT(work->grp[1].player[0]);
            M567_PLAYER_POS(work->grp[1].player[1]);
            M567_PLAYER_ROT(work->grp[1].player[1]);
            M567_PLAYER_POS(work->grp[1].player[2]);
            M567_PLAYER_ROT(work->grp[1].player[2]);
            Hu3DModelShadowSet(work->grp[0].player[0]->mdl);
            Hu3DModelShadowSet(work->grp[1].player[0]->mdl);
            Hu3DModelShadowSet(work->grp[1].player[1]->mdl);
            Hu3DModelShadowSet(work->grp[1].player[2]->mdl);
            WipeCreate(1, 0, 0x3C);
            work->step++;
            break;
        case 7:
            if (WipeCheckEnd() == 0) {
                work->step++;
            }
            if (work->winGrp >= 0) {
                body = &work->grp[work->winGrp].body;
                time = Hu3DMotionTimeGet(body->mdl);
                if (time >= 40.0f && time < 41.0f) {
                    M567_FIRE_FX(work, body);
                }
            }
            break;
        case 8:
            if (work->winGrp >= 0) {
                body = &work->grp[work->winGrp].body;
                time = Hu3DMotionTimeGet(body->mdl);
                if (time >= 40.0f && time < 41.0f) {
                    M567_FIRE_FX(work, body);
                }
            }
            MgSeqModeNext();
            break;
    }
}

void fn_1_32CC(void) {
    M567WORK *work = lbl_1_bss_4->data;
    M567BODY *body;
    f32 time;
    s32 p0;
    s32 p1;
    s32 p2;
    s32 p3;

    switch (work->subStep) {
        case 0:
            if (work->winGrp == 0) {
                p0 = work->grp[0].player[0]->no;
                if (_CheckFlag(0x1000F) == 0) {
                    GwPlayer[p0].mgCoinBonus = 10;
                }
                CharModelAttrReset(work->grp[0].player[0]->charNo, 0x40000002);
            }
            if (work->winGrp == 1) {
                p1 = work->grp[1].player[0]->no;
                if (_CheckFlag(0x1000F) == 0) {
                    GwPlayer[p1].mgCoinBonus = 10;
                }
                p2 = work->grp[1].player[1]->no;
                if (_CheckFlag(0x1000F) == 0) {
                    GwPlayer[p2].mgCoinBonus = 10;
                }
                p3 = work->grp[1].player[2]->no;
                if (_CheckFlag(0x1000F) == 0) {
                    GwPlayer[p3].mgCoinBonus = 10;
                }
                CharModelAttrReset(work->grp[1].player[0]->charNo, 0x40000002);
                CharModelAttrReset(work->grp[1].player[1]->charNo, 0x40000002);
                CharModelAttrReset(work->grp[1].player[2]->charNo, 0x40000002);
            }
            if (work->winGrp < 0) {
                CharModelVoicePanSet(work->grp[0].player[0]->charNo, 0x30);
                CharModelVoicePanSet(work->grp[1].player[0]->charNo, 0x50);
                CharModelVoicePanSet(work->grp[1].player[1]->charNo, 0x50);
                CharModelVoicePanSet(work->grp[1].player[2]->charNo, 0x50);
                CharLoseVoicePlay(work->player[0]->charNo, work->player[1]->charNo, work->player[2]->charNo, work->player[3]->charNo);
            }
            work->subStep++;
        case 1:
            if (work->winGrp >= 0) {
                work->frame++;
                body = &work->grp[work->winGrp].body;
                time = Hu3DMotionTimeGet(body->mdl);
                if (time >= 40.0f && time < 41.0f) {
                    fn_1_4704(&work->fx, fn_1_988C, body);
                    Hu3DModelAttrReset(body->fxMdl, 1);
                    Hu3DModelAttrReset(body->fxMdl, 0x40000002);
                    Hu3DMotionTimeSet(body->fxMdl, 0.0f);
                    Hu3DModelPosSetV(body->fxMdl, &body->pos);
                    if (work->frame < 0xB4) {
                        if (work->winGrp == 0) {
                            lbl_1_data_43C[0] = HuAudFXPlay(0x8F6);
                        }
                        if (work->winGrp == 1) {
                            lbl_1_data_43C[1] = HuAudFXPlay(0x8F8);
                        }
                    } else {
                        if (HuAudFXStatusGet(lbl_1_data_43C[work->winGrp]) == 2) {
                            HuAudFXFadeOut(lbl_1_data_43C[work->winGrp], 1000);
                        }
                    }
                }
            }
            break;
    }
}

void fn_1_362C(void) {
    M567WORK *work = lbl_1_bss_4->data;
    M567BODY *body;
    f32 time;

    if (work->winGrp >= 0) {
        body = &work->grp[work->winGrp].body;
        time = Hu3DMotionTimeGet(body->mdl);
        if (time >= 40.0f && time < 41.0f) {
            fn_1_4704(&work->fx, fn_1_988C, body);
            Hu3DModelAttrReset(body->fxMdl, 1);
            Hu3DModelAttrReset(body->fxMdl, 0x40000002);
            Hu3DMotionTimeSet(body->fxMdl, 0.0f);
            Hu3DModelPosSetV(body->fxMdl, &body->pos);
        }
    }
}

void fn_1_371C(void) {
    M567WORK *work = lbl_1_bss_4->data;

    fn_1_4164(&work->split);
}

void fn_1_3758(OMOBJ *obj) {
    M567PLAYER *player = obj->data;
    s32 i;
    M567WORK *work = lbl_1_bss_4->data;

    player->no = obj->work[0];
    player->charNo = GwPlayerConf[player->no].charNo;
    player->grpNo = GwPlayerConf[player->no].grp;
    player->padNo = GwPlayerConf[player->no].padNo;
    player->type = GwPlayerConf[player->no].type;
    player->dif = GwPlayerConf[player->no].dif;
    player->obj = obj;
    work->player[player->no] = player;
    *obj->mdlId = CharModelCreate(player->charNo, 8);
    player->mdl = *obj->mdlId;
    for (i = 0; i < 5; i++) {
        obj->mtnId[i] = CharMotionCreate(player->charNo, lbl_1_data_28[player->charNo][i]);
    }
    CharMotionSet(player->charNo, obj->mtnId[1]);
    CharModelAttrSet(player->charNo, 0x40000002);
    Hu3DModelLayerSet(*obj->mdlId, 2);
    CharEffectLayerSet(4);
    CharMotionDataClose(player->charNo);
    obj->objFunc = fn_1_3908;
}

void fn_1_3908(OMOBJ *obj) {
    M567PLAYER *player = obj->data;
    M567WORK *work = lbl_1_bss_4->data;

    if (work->inputOn != 0 && player->slot->state == 0xF) {
        if (player->type != 0) {
            fn_1_8F1C(player);
            return;
        }
        fn_1_8EC8(player);
    }
}

void fn_1_3990(void) {
}

void fn_1_3994(OMOBJ *obj) {
    M567WORK *work = lbl_1_bss_4->data;

    fn_1_4680(&work->fx, work->fxNode, 100);
    work->winGrp = -1;
    obj->objFunc = fn_1_39F8;
}

void fn_1_39F8(OMOBJ *obj) {
    M567WORK *work = lbl_1_bss_4->data;
    M567GROUP *g;
    Vec eye;
    s32 a;
    s32 b;

    a = fn_1_7084(&work->grp[0]);
    b = fn_1_7084(&work->grp[1]);
    if (a != 0 || b != 0) {
        if (a != 0) {
            if (b != 0) {
                work->winGrp = frandmod(2);
            } else {
                work->winGrp = 0;
            }
        } else if (b != 0) {
            work->winGrp = 1;
        }
        if (work->grp[0].state != 4 && work->grp[0].state != 7 && work->grp[0].state != 8) {
            work->grp[0].state = 7;
        }
        if (work->grp[1].state != 4 && work->grp[1].state != 7 && work->grp[1].state != 8) {
            work->grp[1].state = 7;
        }
        g = &work->grp[work->winGrp ^ 1];
        if (g->nextRow == 9) {
            g->nextRow = g->row;
            g->body.f2C = g->body.f20;
        }
    }
    fn_1_4404(&work->cam[0], &eye);
    fn_1_8330(&work->grp[0].mover, &eye);
    fn_1_4404(&work->cam[1], &eye);
    fn_1_8330(&work->grp[1].mover, &eye);
    fn_1_3EEC(&work->split);
    fn_1_47D0(&work->fx);
}

void fn_1_3B74(void) {
}

void fn_1_3B78(M567WORK *work) {
    s32 i;

    for (i = 0; i < 0x41; i++) {
        work->mdlCache[i] = -1;
    }
    for (i = 0; i < 9; i++) {
        work->mtnCache[i] = -1;
    }
    for (i = 0; i < 0x20; i++) {
        work->mdlList[i] = -1;
    }
}

void fn_1_3BF8(void) {
    HU3DLIGHTID light;

    light = Hu3DGLightCreate(0.0f, 4000.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0x7F, 0x7F, 0x7F);
    Hu3DGLightInfinitytSet(light);
}

void fn_1_3C80(M567WORK *work) {
    s32 i;

    for (i = 0; i < 2; i++) {
        Hu3DCameraCreate(lbl_1_rodata_10[i]);
        Hu3DCameraPerspectiveSet(lbl_1_rodata_10[i], 45.0f, 20.0f, 20000.0f, 0.6f);
        Hu3DCameraPosSet(lbl_1_rodata_10[i], 0.0f, 300.0f, 1000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 150.0f, 0.0f);
        fn_1_439C(&work->cam[i], lbl_1_rodata_10[i]);
        work->cam[i].rot.x = lbl_1_data_254[i].rot.x;
        work->cam[i].rot.y = lbl_1_data_254[i].rot.y;
        work->cam[i].rot.z = lbl_1_data_254[i].rot.z;
        work->cam[i].target.x = lbl_1_data_254[i].target.x;
        work->cam[i].target.y = lbl_1_data_254[i].target.y;
        work->cam[i].target.z = lbl_1_data_254[i].target.z;
    }
}

void fn_1_3E94(M567SPLIT *split) {
    split->x = 640.0f;
    split->y = 0.0f;
    split->unkA = 0;
    fn_1_3EEC(split);
}

void fn_1_3EEC(M567SPLIT *split) {
    s32 i;
    f32 aspect;

    i = 0;
    Hu3DCameraViewportSet(lbl_1_rodata_10[i], 0.0f, 0.0f, 16.0f + split->x, 480.0f, 0.0f, 1.0f);
    i++;
    Hu3DCameraViewportSet(lbl_1_rodata_10[i], split->x - 16.0f, 0.0f, 16.0f + (640.0f - split->x), 480.0f, 0.0f, 1.0f);
    i = 0;
    Hu3DCameraScissorSet(lbl_1_rodata_10[i], 0, 0, (u32)(split->x - 1.0f), 0x1E0);
    i++;
    Hu3DCameraScissorSet(lbl_1_rodata_10[i], (u32)(1.0f + split->x), 0, (u32)((640.0f - split->x) - 1.0f), 0x1E0);
    i = 0;
    aspect = (0.9f * split->x) / 480.0f;
    if (aspect < 0.0f) {
        aspect = 0.0f;
    }
    Hu3DCameraPerspectiveSet(lbl_1_rodata_10[i], 45.0f, 20.0f, 20000.0f, aspect);
    i++;
}

void fn_1_4164(M567SPLIT *split) {
}

void fn_1_4168(M567WORK *work) {
    Vec up = { 0.0f, 1.0f, 0.0f };
    Vec pos = { 0.0f, 2000.0f, 2000.0f };
    Vec target = { 0.1f, 0.0f, 2000.0f };
    s16 mask;

    mask = lbl_1_rodata_10[0] | lbl_1_rodata_10[1];
    Hu3DShadowMultiCreate(45.0f, 20.0f, 20000.0f, mask);
    Hu3DShadowMultiPosSet(&pos, &up, &target, lbl_1_rodata_10[0]);
    pos.z *= -1.0f;
    target.z *= -1.0f;
    Hu3DShadowMultiPosSet(&pos, &up, &target, lbl_1_rodata_10[1]);
}

void fn_1_42A8(M567WORK *work, s32 grpNo) {
    Vec up = { 0.0f, 1.0f, 0.0f };
    Vec pos = { 0.0f, 2000.0f, -300.0f };
    Vec target = { 0.1f, 0.0f, -300.0f };

    Hu3DShadowCreate(45.0f, 20.0f, 20000.0f);
    if (grpNo != 0) {
        pos.z *= -1.0f;
        target.z *= -1.0f;
    }
    Hu3DShadowPosSet(&pos, &up, &target);
}

void fn_1_439C(M567CAM *cam, s16 camBit) {
    cam->target.x = 0.0f;
    cam->target.y = 0.0f;
    cam->target.z = 0.0f;
    cam->rot.x = 0.0f;
    cam->rot.y = 0.0f;
    cam->rot.z = 3000.0f;
    cam->camBit = camBit;
}

void fn_1_4404(M567CAM *cam, Vec *eye) {
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
    eye->x = cam->target.x + cam->rot.z * cos(3.141592653589793 * cam->rot.y / 180.0) * cos(3.141592653589793 * cam->rot.x / 180.0);
    eye->y = cam->target.y + cam->rot.z * sin(3.141592653589793 * cam->rot.y / 180.0);
    eye->z = cam->target.z + cam->rot.z * cos(3.141592653589793 * cam->rot.y / 180.0) * sin(3.141592653589793 * cam->rot.x / 180.0);
    Hu3DCameraPosSet(cam->camBit, eye->x, eye->y, eye->z, 0.0f, 1.0f, 0.0f, cam->target.x, cam->target.y, cam->target.z);
}

void fn_1_4680(M567FXLIST *list, M567FXNODE *nodes, s32 count) {
    s32 i;

    list->head = NULL;
    list->free = nodes;
    for (i = 0; i < count - 1; i++, nodes++) {
        nodes->prev = NULL;
        nodes->next = nodes + 1;
        nodes->func = NULL;
        nodes->arg = NULL;
        nodes->list = list;
    }
    nodes->prev = NULL;
    nodes->next = NULL;
    nodes->func = NULL;
    nodes->arg = NULL;
    nodes->list = list;
}

M567FXNODE *fn_1_4704(M567FXLIST *list, s32 (*func)(M567FXNODE *), void *arg) {
    M567FXNODE *node = list->free;

    if (node != 0) {
        list->free = node->next;
        node->func = func;
        node->arg = arg;
        node->next = list->head;
        if (node->next != 0) {
            node->next->prev = node;
        }
        list->head = node;
    }
    return node;
}

void fn_1_4758(M567FXNODE *node) {
    M567FXLIST *list;

    if (node != 0) {
        list = node->list;
        if (node->prev != 0) {
            node->prev->next = node->next;
        } else {
            list->head = node->next;
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

void fn_1_47D0(M567FXLIST *list) {
    M567FXNODE *node = list->head;
    M567FXNODE *next;

    while (node != 0) {
        next = node->next;
        if (node->func != 0 && node->func(node) != 0) {
            fn_1_4758(node);
        }
        node = next;
    }
}

/* fetch (create once, then link) a model of the shared model cache */
#define M567_MDL_GET(dst, idx)                                         \
    do {                                                               \
        if (work->mdlCache[(idx)] < 0) {                               \
            (dst) = work->mdlCache[(idx)] = Hu3DModelCreateData(lbl_1_data_12C[(idx)]); \
        } else {                                                       \
            (dst) = Hu3DModelLink(work->mdlCache[(idx)]);              \
        }                                                              \
    } while (0)

void fn_1_48B0(M567WORK *work) {
    s16 id;

    M567_MDL_GET(id, 8);
    Hu3DModelAttrSet(id, 1);
    Hu3DModelAttrSet(id, 0x40000002);
    Hu3DModelLayerSet(id, 0);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(id, lbl_1_rodata_10[0]);
    M567_MDL_GET(id, 9);
    Hu3DModelAttrSet(id, 1);
    Hu3DModelAttrSet(id, 0x40000002);
    Hu3DModelLayerSet(id, 0);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(id, 1);
    M567_MDL_GET(id, 0xA);
    Hu3DModelAttrSet(id, 1);
    Hu3DModelAttrSet(id, 0x40000002);
    Hu3DModelLayerSet(id, 1);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(id, 1);
    M567_MDL_GET(id, 0xB);
    Hu3DModelAttrSet(id, 1);
    Hu3DModelAttrSet(id, 0x40000001);
    Hu3DModelLayerSet(id, 1);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(id, lbl_1_rodata_10[0]);
    M567_MDL_GET(id, 0xC);
    Hu3DModelAttrSet(id, 1);
    Hu3DModelAttrSet(id, 0x40000002);
    Hu3DModelLayerSet(id, 1);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(id, lbl_1_rodata_10[0]);
    Hu3DModelAttrReset(id, 1);
    M567_MDL_GET(id, 0x17);
    Hu3DModelAttrSet(id, 1);
    Hu3DModelAttrSet(id, 0x40000002);
    Hu3DModelLayerSet(id, 1);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(id, 1);
    M567_MDL_GET(id, 0x19);
    Hu3DModelAttrSet(id, 1);
    Hu3DModelAttrSet(id, 0x40000002);
    Hu3DModelLayerSet(id, 1);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(id, 1);
    M567_MDL_GET(id, 0x1A);
    Hu3DModelAttrSet(id, 1);
    Hu3DModelAttrSet(id, 0x40000002);
    Hu3DModelLayerSet(id, 1);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 180.0f, 0.0f);
    Hu3DModelAttrReset(id, 1);
    M567_MDL_GET(id, 0x3F);
    Hu3DModelAttrSet(id, 1);
    Hu3DModelAttrSet(id, 0x40000002);
    Hu3DModelLayerSet(id, 2);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    M567_MDL_GET(id, 0x40);
    Hu3DModelAttrSet(id, 1);
    Hu3DModelAttrSet(id, 0x40000002);
    Hu3DModelLayerSet(id, 2);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    M567_MDL_GET(id, 0x1D);
    Hu3DModelAttrSet(id, 1);
    Hu3DModelAttrSet(id, 0x40000002);
    Hu3DModelLayerSet(id, 4);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(id, lbl_1_rodata_10[0]);
    {
        s32 col;
        s32 row;
        s32 k;
        s16 tblNo;
        s32 count;
        s16 (*map)[5];
        s32 unused;
        s16 rnd[8][3];

        map = work->grp[0].map;
        count = 0;
        tblNo = frandmod(5);
        k = 0;
        unused = 0;
        for (row = 1; row < 9; row++) {
            for (col = 1; col < 4; col++) {
                if (map[row][col] == -1) {
                    rnd[row - 1][col - 1] = frandmod(4);
                    M567_MDL_GET(id, rnd[row - 1][col - 1] + 0xD);
                    work->mdlList[count] = id;
                    count++;
                    Hu3DModelAttrSet(id, 1);
                    Hu3DModelAttrSet(id, 0x40000002);
                    Hu3DModelLayerSet(id, 0);
                    Hu3DModelPosSet(id, 300.0f * (f32)(col - 2), 0.0f, 2700.0f - 300.0f * (f32)row);
                    Hu3DModelRotSet(id, 0.0f, 180.0f, 0.0f);
                    Hu3DModelAttrReset(id, 1);
                }
                M567_MDL_GET(id, lbl_1_data_344[tblNo][k]);
                k++;
                Hu3DModelAttrSet(id, 1);
                Hu3DModelAttrSet(id, 0x40000002);
                Hu3DModelLayerSet(id, 0);
                Hu3DModelPosSet(id, 300.0f * (f32)(col - 2), 0.0f, 2700.0f - 300.0f * (f32)row);
                Hu3DModelRotSet(id, 0.0f, 180.0f, 0.0f);
                Hu3DModelAttrReset(id, 1);
                work->mdlRowA[row - 1][col - 1] = id;
            }
        }
        for (col = 1; col < 4; col++) {
            M567_MDL_GET(id, 0x11);
            Hu3DModelAttrSet(id, 1);
            Hu3DModelAttrSet(id, 0x40000002);
            Hu3DModelLayerSet(id, 0);
            Hu3DModelPosSet(id, 300.0f * (f32)(col - 2), 0.0f, 0.0f);
            Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
            Hu3DModelAttrReset(id, 1);
            work->mdlCE4[col - 1] = id;
        }
        k = 0;
        for (row = 1; row < 9; row++) {
            for (col = 1; col < 4; col++) {
                if (map[row][col] == -1) {
                    M567_MDL_GET(id, rnd[row - 1][col - 1] + 0xD);
                    work->mdlList[count] = id;
                    count++;
                    Hu3DModelAttrSet(id, 1);
                    Hu3DModelAttrSet(id, 0x40000002);
                    Hu3DModelLayerSet(id, 0);
                    Hu3DModelPosSet(id, 300.0f * (f32) - (col - 2), 0.0f, -2700.0f + 300.0f * (f32)row);
                    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
                    Hu3DModelAttrReset(id, 1);
                }
                M567_MDL_GET(id, lbl_1_data_344[tblNo][k]);
                k++;
                Hu3DModelAttrSet(id, 1);
                Hu3DModelAttrSet(id, 0x40000002);
                Hu3DModelLayerSet(id, 0);
                Hu3DModelPosSet(id, 300.0f * (f32) - (col - 2), 0.0f, -2700.0f + 300.0f * (f32)row);
                Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
                Hu3DModelAttrReset(id, 1);
                work->mdlRowB[row - 1][col - 1] = id;
            }
        }
    }
}

void fn_1_5BD4(s16 map[11][5]) {
    s16 tbl[9][5] = {
        { -1, 0, 0, 0, -1 },
        { -1, -1, 0, 0, -1 },
        { -1, 0, -1, 0, -1 },
        { -1, -1, -1, 0, -1 },
        { -1, 0, 0, -1, -1 },
        { -1, -1, 0, -1, -1 },
        { -1, 0, -1, -1, -1 },
        { -1, 0, 0, 0, -1 },
        { -1, 2, 2, 2, -1 },
    };
    s16 cnt[3];
    s16 rows[3] = { 1, 2, 4 };
    s32 j;
    s32 i;
    s32 prev;
    s32 mask;

    for (i = 0; i < 5; i++) {
        map[0][i] = -1;
        map[10][i] = -1;
    }
    for (i = 0; i < 5; i++) {
        map[1][i] = tbl[7][i];
        map[9][i] = tbl[8][i];
    }
    prev = rows[frandmod(3)];
    for (j = 0; j < 5; j++) {
        map[2][j] = tbl[prev][j];
    }
    for (i = 3; i < 9; i++) {
        cnt[0] = 0;
        cnt[1] = 0;
        cnt[2] = 0;
        if (i >= 5) {
            for (j = i - 4; j < i; j++) {
                cnt[0] += map[j][1] == 0;
                cnt[1] += map[j][2] == 0;
                cnt[2] += map[j][3] == 0;
            }
        }
        mask = 0;
        if (cnt[0] == 4) {
            mask += 1;
        }
        if (cnt[1] == 4) {
            mask += 2;
        }
        if (cnt[2] == 4) {
            mask += 4;
        }
        if (mask != 0 && i < 8) {
            if (prev != 0) {
                prev = 0;
                for (j = 0; j < 5; j++) {
                    map[i][j] = tbl[prev][j];
                }
                i++;
            }
            prev = mask;
            for (j = 0; j < 5; j++) {
                map[i][j] = tbl[prev][j];
            }
        } else {
            if (i == 8) {
                prev = fn_1_5FC0(prev);
            } else {
                prev = fn_1_5F04(prev);
            }
            for (j = 0; j < 5; j++) {
                map[i][j] = tbl[prev][j];
            }
        }
    }
}

s32 fn_1_5F04(s32 prev) {
    s32 opts[3] = { 0, 2, 4 };
#ifdef TARGET_PC
    s32 ret = 0;
#else
    s32 ret;
#endif

    switch (prev) {
        case 0:
            ret = frandmod(6) + 1;
            break;
        case 1:
            ret = opts[frandmod(3)];
            break;
        case 4:
            ret = frandmod(3);
            break;
        case 2:
        case 3:
        case 5:
        case 6:
            ret = 0;
            break;
    }
    return ret;
}

s32 fn_1_5FC0(s32 prev) {
    s32 optsA[2] = { 0, 4 };
    s32 optsB[2] = { 0, 1 };
#ifdef TARGET_PC
    s32 ret = 0;
#else
    s32 ret;
#endif

    switch (prev) {
        case 0:
            ret = frandmod(6) + 1;
            break;
        case 1:
            ret = optsA[frandmod(2)];
            break;
        case 4:
            ret = optsB[frandmod(2)];
            break;
        case 2:
        case 3:
        case 5:
        case 6:
            ret = 0;
            break;
    }
    return ret;
}

void fn_1_6094(s16 dst[12][5], s16 src[12][5]) {
    s32 i;
    s32 j;

    for (i = 0; i < 12; i++) {
        for (j = 0; j < 5; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

void fn_1_60F8(M567WORK *work) {
    M567PLAYER *player;
    M567BODY *body;
    s32 i;
    s16 id;
    s32 col;
    s32 row;
    s32 n;
    M567PLAYER *others[3];

    work->grp[0].kind = 0;
    work->grp[1].kind = 1;
    work->grp[0].row = 1;
    work->grp[0].col = 2;
    work->grp[1].row = 1;
    work->grp[1].col = 2;
    n = 0;
    for (i = 0; i < 4; i++) {
        player = work->player[i];
        if (player->grpNo == 0) {
            work->grp[0].player[0] = player;
        } else {
            others[n] = player;
            n++;
        }
    }
    for (i = 0; i < 3; i++) {
        s16 a;
        s16 b;
        M567PLAYER *tmp;

        a = frandmod(3);
        b = frandmod(3);
        tmp = others[a];
        others[a] = others[b];
        others[b] = tmp;
    }
    for (i = 0; i < 3; i++) {
        work->grp[1].player[i] = others[i];
    }
    work->grp[0].player[0]->idx = 0;
    work->grp[1].player[0]->idx = 0;
    work->grp[1].player[1]->idx = 1;
    work->grp[1].player[2]->idx = 2;
    work->grp[0].player[0]->grp = &work->grp[0];
    work->grp[1].player[0]->grp = &work->grp[1];
    work->grp[1].player[1]->grp = &work->grp[1];
    work->grp[1].player[2]->grp = &work->grp[1];
    work->grp[0].body.cam = &work->cam[0];
    work->grp[1].body.cam = &work->cam[1];
    fn_1_5BD4(work->grp[0].map);
    /* the original copies 12 rows of 5 although the map only has 11 (row 11 is dir + pos.x) */
    for (row = 0; row < 12; row++) {
        for (col = 0; col < 5; col++) {
            work->grp[1].map[row][col] = work->grp[0].map[row][col];
        }
    }
    player = work->grp[0].player[0];
    M567_PLAYER_SET(player, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    M567_PLAYER_POS(player);
    M567_PLAYER_ROT(player);
    for (i = 0; i < 3; i++) {
        player = work->grp[1].player[i];
        M567_PLAYER_SET(player, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        M567_PLAYER_POS(player);
        M567_PLAYER_ROT(player);
    }
    body = &work->grp[0].body;
    fn_1_69B4(body, 0);
    body->pos.x = 0.0f;
    body->pos.y = 0.0f;
    body->pos.z = 2400.0f;
    body->rot.x = 0.0f;
    body->rot.y = 180.0f;
    body->rot.z = 0.0f;
    Hu3DModelPosSetV(body->mdl, &body->pos);
    Hu3DModelRotSetV(body->mdl, &body->rot);
    body = &work->grp[1].body;
    fn_1_69B4(body, 1);
    body->pos.x = 0.0f;
    body->pos.y = 0.0f;
    body->pos.z = -2400.0f;
    body->rot.x = 0.0f;
    body->rot.y = 0.0f;
    body->rot.z = 0.0f;
    Hu3DModelPosSetV(body->mdl, &body->pos);
    Hu3DModelRotSetV(body->mdl, &body->rot);
    work->grp[0].mover.slot[0].miniMdl = work->grp[0].body.miniMdl[0];
    work->grp[1].mover.slot[0].miniMdl = work->grp[1].body.miniMdl[0];
    work->grp[1].mover.slot[1].miniMdl = work->grp[1].body.miniMdl[1];
    work->grp[1].mover.slot[2].miniMdl = work->grp[1].body.miniMdl[2];
    fn_1_7DB4(&work->grp[0].mover, work->grp[0].body.cam, 0);
    fn_1_7DB4(&work->grp[1].mover, work->grp[1].body.cam, 1);
    Hu3DModelHookSet(work->grp[0].body.mdl, "m567-play0", work->grp[0].player[0]->mdl);
    Hu3DModelHookSet(work->grp[1].body.mdl, "m567-play3", work->grp[1].player[0]->mdl);
    Hu3DModelHookSet(work->grp[1].body.mdl, "m567-play1", work->grp[1].player[1]->mdl);
    Hu3DModelHookSet(work->grp[1].body.mdl, "m567-play2", work->grp[1].player[2]->mdl);
    Hu3DModelHookSet(work->grp[0].body.mdl, "m567-1pa_mini", work->grp[0].body.miniMdl[0]);
    Hu3DModelHookSet(work->grp[1].body.mdl, "m567-3pa_mini", work->grp[1].body.miniMdl[0]);
    Hu3DModelHookSet(work->grp[1].body.mdl, "m567-3pb_mini", work->grp[1].body.miniMdl[1]);
    Hu3DModelHookSet(work->grp[1].body.mdl, "m567-3pc_mini", work->grp[1].body.miniMdl[2]);
    Hu3DModelHookSet(work->grp[0].body.mdl, "m567-pum0", work->grp[0].body.pumMdl[0]);
    Hu3DModelHookSet(work->grp[1].body.mdl, "m567-pum3", work->grp[1].body.pumMdl[0]);
    Hu3DModelHookSet(work->grp[1].body.mdl, "m567-pum1", work->grp[1].body.pumMdl[1]);
    Hu3DModelHookSet(work->grp[1].body.mdl, "m567-pum2", work->grp[1].body.pumMdl[2]);
    work->grp[0].player[0]->slot = &work->grp[0].mover.slot[0];
    work->grp[1].player[0]->slot = &work->grp[1].mover.slot[0];
    work->grp[1].player[1]->slot = &work->grp[1].mover.slot[1];
    work->grp[1].player[2]->slot = &work->grp[1].mover.slot[2];
    work->grp[0].player[0]->hookMdl = work->grp[0].body.pumMdl[0];
    work->grp[1].player[0]->hookMdl = work->grp[1].body.pumMdl[0];
    work->grp[1].player[1]->hookMdl = work->grp[1].body.pumMdl[1];
    work->grp[1].player[2]->hookMdl = work->grp[1].body.pumMdl[2];
    work->grp[0].state = 0;
    work->grp[1].state = 0;
    work->grp[0].limit = 0x14;
    work->grp[1].limit = 0x1E;
    M567_MDL_GET(id, work->grp[0].player[0]->charNo + 0x1E);
    work->grp[0].mover.mdlA[0] = id;
    Hu3DModelAttrSet(id, 1);
    Hu3DModelAttrSet(id, 0x40000002);
    Hu3DModelLayerSet(id, 6);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(id, 1);
    for (i = 0; i < 3; i++) {
        M567_MDL_GET(id, work->grp[1].player[i]->charNo + 0x2B);
        work->grp[1].mover.mdlA[i] = id;
        Hu3DModelAttrSet(id, 1);
        Hu3DModelAttrSet(id, 0x40000002);
        Hu3DModelLayerSet(id, 6);
        Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrReset(id, 1);
    }
}

/* fetch (create once) a joint motion of the shared motion cache */
#define M567_MTN_GET(dst, mdl, idx)                                                              \
    do {                                                                                         \
        if (work->mtnCache[(idx)] < 0) {                                                         \
            (dst) = work->mtnCache[(idx)] = Hu3DJointMotionData((mdl), lbl_1_data_230[(idx)]);   \
        } else {                                                                                 \
            (dst) = work->mtnCache[(idx)];                                                       \
        }                                                                                        \
    } while (0)

void fn_1_69B4(M567BODY *body, s32 grpNo) {
    M567WORK *work = lbl_1_bss_4->data;
    s16 id;
    s16 mtn;
    s32 cnt;
    s32 n;
    s32 i;

    if (grpNo == 0) {
        M567_MDL_GET(id, 0x12);
        M567_MTN_GET(mtn, id, 1);
        body->mtn[0] = mtn;
        Hu3DMotionSet(id, mtn);
        M567_MTN_GET(mtn, id, 2);
        body->mtn[1] = mtn;
        M567_MTN_GET(mtn, id, 3);
        body->mtn[2] = mtn;
        M567_MTN_GET(mtn, id, 4);
        body->mtn[3] = mtn;
    } else {
        M567_MDL_GET(id, 0x13);
        M567_MTN_GET(mtn, id, 5);
        body->mtn[0] = mtn;
        Hu3DMotionSet(id, mtn);
        M567_MTN_GET(mtn, id, 6);
        body->mtn[1] = mtn;
        M567_MTN_GET(mtn, id, 7);
        body->mtn[2] = mtn;
        M567_MTN_GET(mtn, id, 8);
        body->mtn[3] = mtn;
    }
    body->mdl = id;
    Hu3DModelAttrSet(id, 1);
    Hu3DModelAttrSet(id, 0x40000001);
    Hu3DModelLayerSet(id, 2);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(id, 1);
    if (grpNo != 0) {
        n = 3;
    } else {
        n = 1;
    }
    cnt = n;
    for (i = 0; i < cnt; i++) {
        M567_MDL_GET(id, 0x38);
        body->miniMdl[i] = id;
        Hu3DModelAttrSet(id, 1);
        Hu3DModelAttrSet(id, 0x40000002);
        Hu3DModelLayerSet(id, 2);
        Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrReset(id, 1);
        if (grpNo != 0) {
            M567_MDL_GET(id, 0x3E);
        } else {
            M567_MDL_GET(id, 0x3D);
        }
        body->pumMdl[i] = id;
        M567_MTN_GET(mtn, id, 0);
        Hu3DMotionSet(id, mtn);
        Hu3DModelAttrSet(id, 1);
        Hu3DModelAttrSet(id, 0x40000002);
        Hu3DModelLayerSet(id, 2);
        Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrReset(id, 1);
    }
    M567_MDL_GET(id, 0x1C);
    body->fxMdl = id;
    Hu3DModelAttrSet(id, 1);
    Hu3DModelAttrSet(id, 0x40000002);
    Hu3DModelLayerSet(id, 4);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
}

/* queue an effect node that follows `arg` */
#define M567_FX_ADD(work, func, arg) fn_1_4704(&(work)->fx, (func), (arg))

#define M567_FX_STOP(idx)                       \
    do {                                        \
        if (lbl_1_data_434[(idx)] >= 0) {       \
            HuAudFXStop(lbl_1_data_434[(idx)]); \
            lbl_1_data_434[(idx)] = -1;         \
        }                                       \
    } while (0)

s32 fn_1_7084(M567GROUP *grp) {
    s32 ret = 0;
    s16 pan[3] = { 0x20, 0x40, 0x60 };
    M567WORK *work;
    s32 i;
    s32 cnt;
    s32 n;
    s32 n2;
    s32 n3;
    s32 n4;
    f32 t;

    work = lbl_1_bss_4->data;
    if (work->moveOn != 0) {
        if (grp->kind != 0) {
            grp->body.cam->target.x = 0.5f * grp->body.pos.x;
            grp->body.cam->target.z = 250.0f + grp->body.pos.z;
        } else {
            grp->body.cam->target.x = 0.5f * grp->body.pos.x;
            grp->body.cam->target.z = grp->body.pos.z - 250.0f;
        }
    }
    switch (grp->state) {
        case 1:
            grp->timer = 0;
            if (grp->kind != 0) {
                n = 3;
            } else {
                n = 1;
            }
            cnt = n;
            for (i = 0; i < cnt; i++) {
                grp->player[i]->slot->state = 0xA;
                grp->player[i]->comState = 0;
            }
            if (work->inputOn != 0) {
                if (grp->kind == 0) {
                    if (lbl_1_data_434[0] < 0) {
                        lbl_1_data_434[0] = HuAudFXPlayPan(0x8F1, 0x30);
                    }
                } else if (lbl_1_data_434[1] < 0) {
                    lbl_1_data_434[1] = HuAudFXPlayPan(0x8F2, 0x50);
                }
            }
            grp->mover.state = 2;
            fn_1_959C(grp->map, grp->row, grp->col);
            grp->state++;
        case 2:
            if (grp->kind != 0) {
                n2 = 3;
            } else {
                n2 = 1;
            }
            cnt = n2;
            i = 0;
            while (i < cnt) {
                if (grp->player[i]->slot->state == 0x3C) {
                    i++;
                    continue;
                }
                break;
            }
            if (i == cnt) {
                if (grp->kind == 0) {
                    M567_FX_STOP(0);
                } else {
                    M567_FX_STOP(1);
                }
                grp->state++;
                grp->mover.cur = 0;
            }
            break;
        case 3:
            grp->nextRow = grp->row + grp->player[grp->mover.cur]->slot->unkC;
            grp->nextCol = grp->col + grp->player[grp->mover.cur]->slot->unkA;
            if (grp->map[grp->nextRow][grp->nextCol] != 0 && grp->map[grp->nextRow][grp->nextCol] != 2) {
                grp->nextRow = grp->row;
                grp->nextCol = grp->col;
                if (grp->kind == 0) {
                    omVibrate(grp->player[0]->no, 0x19, 4, 2);
                } else {
                    omVibrate(grp->player[0]->no, 0x19, 4, 2);
                    omVibrate(grp->player[1]->no, 0x19, 4, 2);
                    omVibrate(grp->player[2]->no, 0x19, 4, 2);
                }
                i = grp->mover.cur;
                Hu3DModelAttrReset(grp->mover.mdlB[i], 1);
                Hu3DModelPosSetV(grp->mover.mdlB[i], &grp->mover.objPos[i]);
                Hu3DModelRotSetV(grp->mover.mdlB[i], &grp->mover.rot);
                if (grp->kind == 0) {
                    HuAudFXPlayPan(0x8F9, 0x30);
                } else {
                    HuAudFXPlayPan(0x8FA, 0x50);
                }
            }
            if (grp->kind == 0) {
                grp->body.f18 = 300.0f * (f32)grp->col - 600.0f;
                grp->body.f20 = 2700.0f + 300.0f * (f32) - grp->row;
                grp->body.f24 = 300.0f * (f32)grp->nextCol - 600.0f;
                grp->body.f2C = 2700.0f + 300.0f * (f32) - grp->nextRow;
                HuAudFXPlayPan(0x8F5, pan[grp->nextCol - 1]);
            } else {
                grp->body.f18 = 600.0f + 300.0f * (f32) - grp->col;
                grp->body.f20 = 300.0f * (f32)grp->row - 2700.0f;
                grp->body.f24 = 600.0 + 300.0 * (f64) - grp->nextCol;
                grp->body.f2C = 300.0f * (f32)grp->nextRow - 2700.0f;
                HuAudFXPlayPan(0x8F7, pan[grp->nextCol - 1]);
            }
            if (grp->nextRow == 9) {
                ret = 1;
            }
            grp->state++;
            grp->timer = 0;
            Hu3DMotionShiftSet(grp->body.mdl, grp->body.mtn[1], 0.0f, 5.0f, 0);
            M567_FX_ADD(work, fn_1_979C, &grp->body);
            break;
        case 4:
            grp->timer++;
            if (grp->timer == grp->limit) {
                grp->row = grp->nextRow;
                grp->col = grp->nextCol;
                if (work->inputOn != 0) {
                    grp->state++;
                } else {
                    grp->state = 7;
                }
                Hu3DModelAttrSet(grp->mover.mdlB[grp->mover.cur], 1);
            }
            if (grp->kind != 0) {
                if (grp->timer < 0x11) {
                    t = 0.0f;
                } else if (grp->timer < 0x1C) {
                    t = ((f32)grp->timer - 16.0f) / 11.0f;
                } else {
                    t = 1.0f;
                }
                if (grp->timer == 0x1C) {
                    M567_FX_ADD(work, fn_1_988C, &grp->body);
                    Hu3DModelAttrReset(grp->body.fxMdl, 1);
                    Hu3DModelAttrReset(grp->body.fxMdl, 0x40000002);
                    Hu3DMotionTimeSet(grp->body.fxMdl, 0.0f);
                    Hu3DModelPosSetV(grp->body.fxMdl, &grp->body.pos);
                    HuAudFXPlayPan(0x8F8, pan[grp->nextCol - 1]);
                }
                if (grp->nextRow == 9 && grp->timer == 0x16) {
                    Hu3DModelAttrReset(work->mdlCache[0x19], 0x40000002);
                    Hu3DModelAttrReset(work->mdlCache[0x1A], 0x40000002);
                }
            } else {
                if (grp->timer < 0xB) {
                    t = 0.0f;
                } else if (grp->timer < 0x13) {
                    t = ((f32)grp->timer - 10.0f) * 0.125f;
                } else {
                    t = 1.0f;
                }
                if (grp->timer == 0x13) {
                    M567_FX_ADD(work, fn_1_988C, &grp->body);
                    Hu3DModelAttrReset(grp->body.fxMdl, 1);
                    Hu3DModelAttrReset(grp->body.fxMdl, 0x40000002);
                    Hu3DMotionTimeSet(grp->body.fxMdl, 0.0f);
                    Hu3DModelPosSetV(grp->body.fxMdl, &grp->body.pos);
                    HuAudFXPlayPan(0x8F6, pan[grp->nextCol - 1]);
                }
                if (grp->nextRow == 9 && grp->timer == 0xF) {
                    Hu3DModelAttrReset(work->mdlCache[0x19], 0x40000002);
                    Hu3DModelAttrReset(work->mdlCache[0x1A], 0x40000002);
                }
            }
            grp->body.pos.x = grp->body.f18 * (1.0f - t) + grp->body.f24 * t;
            grp->body.pos.z = grp->body.f20 * (1.0f - t) + grp->body.f2C * t;
            Hu3DModelPosSetV(grp->body.mdl, &grp->body.pos);
            break;
        case 5:
            grp->player[grp->mover.cur]->slot->state = 0x46;
            grp->state++;
        case 6:
            if (grp->player[grp->mover.cur]->slot->state == 1) {
                grp->mover.cur++;
                if (grp->kind != 0) {
                    n3 = 3;
                } else {
                    n3 = 1;
                }
                cnt = n3;
                if (grp->mover.cur == cnt) {
                    grp->state = 1;
                } else {
                    grp->state = 9;
                }
            }
            break;
        case 7:
            if (grp->kind != 0) {
                n4 = 3;
            } else {
                n4 = 1;
            }
            cnt = n4;
            for (i = 0; i < cnt; i++) {
                grp->player[i]->slot->state = 0x46;
            }
            Hu3DMotionShiftSet(grp->body.mdl, grp->body.mtn[0], 0.0f, 5.0f, 0x40000001);
            if (grp->kind == 0) {
                M567_FX_STOP(0);
            } else {
                M567_FX_STOP(1);
            }
            grp->state++;
            break;
        case 9:
            grp->timer = 0;
            grp->state++;
        case 10:
            grp->timer++;
            if (grp->timer == 0x1E) {
                grp->state = 3;
            }
            break;
    }
    return ret;
}

char lbl_1_data_55D[0xB] = "m567-3pc";

void fn_1_7DB4(M567MOVER *mover, M567CAM *cam, s16 grpNo) {
    M567WORK *work;
    s16 id;
    s32 cnt;
    s32 n;
    s32 i;

    work = lbl_1_bss_4->data;
    mover->cam = cam;
    mover->type = grpNo;
    if (grpNo == 0) {
        M567_MDL_GET(id, 0x14);
    } else {
        M567_MDL_GET(id, 0x15);
    }
    mover->mdl = id;
    Hu3DModelAttrSet(id, 1);
    Hu3DModelAttrSet(id, 0x40000002);
    Hu3DModelLayerSet(id, 6);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(id, mover->cam->camBit);
    Hu3DModelAttrReset(id, 1);
    if (grpNo == 0) {
        mover->fx[0].mat = &Hu3DData[id].hsf->material[3];
        mover->fx[1].mat = &Hu3DData[id].hsf->material[4];
    } else {
        mover->fx[0].mat = &Hu3DData[id].hsf->material[6];
        mover->fx[1].mat = &Hu3DData[id].hsf->material[5];
        mover->fx[2].mat = &Hu3DData[id].hsf->material[7];
    }
    if (grpNo == 0) {
        n = 1;
    } else {
        n = 3;
    }
    cnt = n;
    for (i = 0; i < cnt; i++) {
        M567_MDL_GET(id, 0x18);
        mover->mdlB[i] = id;
        Hu3DModelAttrSet(id, 1);
        Hu3DModelAttrSet(id, 0x40000001);
        Hu3DModelLayerSet(id, 6);
        Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(id, mover->cam->camBit);
        if (grpNo == 0) {
            M567_MDL_GET(id, 0x39);
        } else {
            M567_MDL_GET(id, i + 0x3A);
        }
        mover->mdlC[i] = id;
        Hu3DModelAttrSet(id, 1);
        Hu3DModelAttrSet(id, 0x40000002);
        Hu3DModelLayerSet(id, 6);
        Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(id, mover->cam->camBit);
        Hu3DModelAttrReset(id, 1);
        fn_1_8794(&mover->slot[i], grpNo);
    }
    if (grpNo == 0) {
        Hu3DModelHookSet(mover->mdl, "slot04-1pa", mover->slot[0].mdl);
    } else {
        Hu3DModelHookSet(mover->mdl, "m567-3pa", mover->slot[0].mdl);
        Hu3DModelHookSet(mover->mdl, "m567-3pb", mover->slot[1].mdl);
        Hu3DModelHookSet(mover->mdl, lbl_1_data_55D, mover->slot[2].mdl);
    }
    mover->scr.x = 288.0f;
    mover->scr.y = 500.0f;
    mover->scr.z = 494.0f;
    mover->state = 0;
    mover->flag = 0;
}

void fn_1_8330(M567MOVER *mover, Vec *unused) {
    Vec pos;
    M567WORK *work;
    M567CAM *cam;
    s32 cnt;
    s32 n;
    s32 i;

    work = lbl_1_bss_4->data;
    cam = mover->cam;
    Hu3D2Dto3D(&mover->scr, cam->camBit, &pos);
    mover->rot.x = -cam->rot.y;
    mover->rot.y = 90.0f - cam->rot.x;
    mover->rot.z = 0.0f;
    Hu3DModelPosSetV(mover->mdl, &pos);
    Hu3DModelRotSetV(mover->mdl, &mover->rot);
    if (mover->type != 0) {
        Hu3DModelObjPosGet(mover->mdl, "m567-3pa", &mover->objPos[0]);
        Hu3DModelObjPosGet(mover->mdl, "m567-3pb", &mover->objPos[1]);
        Hu3DModelObjPosGet(mover->mdl, lbl_1_data_55D, &mover->objPos[2]);
    } else {
        Hu3DModelObjPosGet(mover->mdl, "slot04-1pa", &mover->objPos[0]);
    }
    if (mover->type != 0) {
        n = 3;
    } else {
        n = 1;
    }
    cnt = n;
    for (i = 0; i < cnt; i++) {
        fn_1_89F4(&mover->slot[i]);
    }
    if (mover->flag == 0) {
        for (i = 0; i < cnt; i++) {
            Hu3DModelPosSetV(mover->mdlA[i], &mover->objPos[i]);
            Hu3DModelRotSetV(mover->mdlA[i], &mover->rot);
            Hu3DModelPosSetV(mover->mdlC[i], &mover->objPos[i]);
            Hu3DModelRotSetV(mover->mdlC[i], &mover->rot);
        }
    }
    if (mover->type == 0) {
        switch (mover->state) {
            case 0:
                mover->timer = 0;
                mover->state++;
                return;
            case 2:
                M567_FX_ADD(work, fn_1_98F0, &mover->fx[0]);
                mover->fx[0].state = 0;
                mover->state++;
            case 3:
                mover->timer++;
                if (mover->timer == 0xF) {
                    M567_FX_ADD(work, fn_1_98F0, &mover->fx[1]);
                    mover->fx[1].state = 0;
                    mover->state = 0;
                    return;
                }
        }
    } else {
        switch (mover->state) {
            case 0:
                mover->timer = 0;
                mover->state++;
                return;
            case 2:
                M567_FX_ADD(work, fn_1_98F0, &mover->fx[0]);
                mover->fx[0].state = 0;
                mover->state++;
            case 3:
                mover->timer++;
                if (mover->timer == 5) {
                    M567_FX_ADD(work, fn_1_98F0, &mover->fx[1]);
                    mover->fx[1].state = 0;
                    mover->state++;
                    return;
                }
                break;
            case 4:
                mover->timer++;
                if (mover->timer == 0xA) {
                    M567_FX_ADD(work, fn_1_98F0, &mover->fx[2]);
                    mover->fx[2].state = 0;
                    mover->state = 0;
                }
                break;
        }
    }
}

void fn_1_8794(M567SLOT *slot, s32 grpNo) {
    M567WORK *work;
    s16 id;

    work = lbl_1_bss_4->data;
    M567_MDL_GET(id, 0x16);
    slot->mdl = id;
    Hu3DModelAttrSet(id, 1);
    Hu3DModelAttrSet(id, 0x40000001);
    Hu3DModelAttrSet(id, 0x40000002);
    Hu3DModelLayerSet(id, 6);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(id, 1);
    M567_MDL_GET(id, 0x1B);
    slot->fxMdl = id;
    Hu3DModelAttrSet(id, 1);
    Hu3DModelLayerSet(id, 6);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    slot->state = 0;
    slot->timer = 0;
    slot->mat = Hu3DData[slot->mdl].hsf->material;
    slot->mat->color[0] = 0x7F;
    slot->mat->color[1] = 0x7F;
    slot->mat->color[2] = 0x7F;
    slot->spin = 0;
    slot->pos = 0;
    slot->maxPos = 0x3C;
    slot->unit = 0x14;
    slot->slotNo = grpNo;
}

void fn_1_89F4(M567SLOT *slot) {
    s32 se[2] = { 0x8F3, 0x8F4 };
    s16 pan[2] = { 0x30, 0x50 };
    s32 level;

    {
        switch (slot->state) {
            case 0x0:
                slot->spin = 0;
                slot->state++;
                break;
            case 0xA:
                slot->mat->color[0] = 0x7F;
                slot->mat->color[1] = 0x7F;
                slot->mat->color[2] = 0x7F;
                slot->timer = 0;
                slot->state++;
            case 0xB:
                slot->timer++;
                if (slot->timer == 5) {
                    slot->state++;
                }
                break;
            case 0xC:
                slot->spin = 1;
                slot->timer = 0;
                slot->state++;
            case 0xD:
                slot->timer++;
                if (slot->timer == 0x19) {
                    slot->state++;
                }
                break;
            case 0xE:
                slot->mat->color[0] = 0xFF;
                slot->mat->color[1] = 0xFF;
                slot->mat->color[2] = 0xFF;
                slot->state++;
                break;
            case 0x14:
                if (slot->pos < slot->unit) {
                    slot->target = slot->unit;
                    slot->state = 0x1E;
                    slot->unkA = 1;
                    slot->unkC = 0;
                } else if (slot->pos < slot->unit * 2) {
                    slot->target = slot->unit * 2;
                    slot->state = 0x28;
                    slot->unkA = -1;
                    slot->unkC = 0;
                } else {
                    slot->target = slot->unit * 2;
                    slot->state = 0x32;
                    slot->unkA = 0;
                    slot->unkC = 1;
                }
                break;
            case 0x1E:
            case 0x28:
                if (slot->pos >= slot->target) {
                    slot->pos = slot->target;
                    slot->spin = 0;
                    slot->state = 0x3C;
                    HuAudFXPlayPan(se[slot->slotNo], pan[slot->slotNo]);
                }
                break;
            case 0x32:
                if (slot->pos < slot->target) {
                    slot->pos = 0;
                    slot->spin = 0;
                    slot->state = 0x3C;
                    HuAudFXPlayPan(se[slot->slotNo], pan[slot->slotNo]);
                }
                break;
            case 0x46:
                slot->timer = 0;
                slot->state++;
            case 0x47:
                slot->timer++;
                if (slot->timer == 5) {
                    slot->state = 0;
                }
                level = (s32)(128.0f * (1.0f - ((f32)slot->timer / 5.0f)));
                slot->mat->color[0] = level + 0x7F;
                slot->mat->color[1] = level + 0x7F;
                slot->mat->color[2] = level + 0x7F;
                break;
            case 0x50:
                slot->mat->color[0] = 0x7F;
                slot->mat->color[1] = 0x7F;
                slot->mat->color[2] = 0x7F;
                slot->state++;
                break;
        }
    }
    fn_1_8DE0(slot);
    Hu3DMotionTimeSet(slot->miniMdl, Hu3DMotionTimeGet(slot->mdl));
}

void fn_1_8DE0(M567SLOT *slot) {
    f32 t;

    switch (slot->spin) {
        case 0:
            break;
        case 1:
            slot->pos++;
            slot->pos = slot->pos % slot->maxPos;
            break;
    }
    t = (60.0f * (f32)slot->pos) / (f32)slot->maxPos;
    Hu3DMotionTimeSet(slot->mdl, t);
}

void fn_1_8EC8(M567PLAYER *player) {
    if ((HuPadBtnDown[player->padNo] & 0x100) != 0) {
        fn_1_92B0(player);
    }
}

void fn_1_8F1C(M567PLAYER *player) {
    s32 tblA[2][4] = { { 50, 30, 20, 10 }, { 60, 40, 25, 15 } };
    s32 tblB[2][4] = { { 30, 30, 20, 10 }, { 30, 40, 25, 15 } };
    s32 tblC[4][4] = { { 45, 65, 75, 85 }, { 45, 65, 75, 85 }, { 45, 60, 75, 80 }, { 45, 55, 75, 75 } };
    s32 idx;

    switch (player->comState) {
        case 0:
            player->comWait = tblB[player->grpNo][player->dif] + frandmod(tblA[player->grpNo][player->dif]);
            player->comTimer = 0;
            player->comState++;
        case 1:
            player->comTimer++;
            if (player->comTimer == player->comWait) {
                player->comState++;
                return;
            }
            return;
        case 2:
            idx = player->grpNo + player->idx;
            if ((s32)frandmod(100) < tblC[idx][player->dif]) {
                player->comChoice = player->grp->dir[player->idx];
            } else {
                player->comChoice = (player->grp->dir[player->idx] + ((s32)frandmod(2) + 1)) % 3;
            }
            player->comState++;
            return;
        case 3:
            switch (player->comChoice) {
                case 0:
                    if (player->slot->pos > player->slot->unit * 2) {
                        fn_1_92B0(player);
                        player->comState++;
                        return;
                    }
                    break;
                case 1:
                    if (player->slot->pos <= player->slot->unit && player->slot->pos > 0) {
                        fn_1_92B0(player);
                        player->comState++;
                        return;
                    }
                    break;
                case 2:
                    if (player->slot->pos <= player->slot->unit * 2 && player->slot->pos > player->slot->unit) {
                        fn_1_92B0(player);
                        player->comState++;
                    }
            }
            break;
    }
}

void fn_1_92B0(M567PLAYER *player) {
    M567WORK *work;
    M567SLOT *slot;
    M567MOVER *mover;
    s32 idx;

    work = lbl_1_bss_4->data;
    slot = player->slot;
    mover = &player->grp->mover;
    idx = player->idx;
    if (M567_FX_ADD(work, fn_1_9828, slot) != 0) {
        Hu3DModelPosSet(slot->fxMdl, mover->objPos[idx].x, mover->objPos[idx].y, mover->objPos[idx].z - 2.0f);
        Hu3DModelRotSetV(slot->fxMdl, &mover->rot);
        Hu3DModelAttrReset(slot->fxMdl, 1);
        Hu3DModelAttrReset(slot->fxMdl, 0x40000002);
        Hu3DMotionTimeSet(slot->fxMdl, 0.0f);
    }
    if (M567_FX_ADD(work, fn_1_9470, player) != 0) {
        player->state = 0;
    }
    if (player->grpNo == 0) {
        mover->fx[0].state++;
        mover->fx[1].state++;
    } else {
        mover->fx[idx].state++;
    }
    omVibrate(player->no, 0xC, 6, 6);
    slot->state = 0x14;
}

s32 fn_1_9470(M567FXNODE *node) {
    s32 ret = 0;
    M567PLAYER *player = node->arg;

    switch (player->state) {
        case 0:
            CharModelAttrReset(player->charNo, 0x40000002);
            Hu3DModelAttrReset(player->hookMdl, 0x40000002);
            player->state++;
        case 1:
            if (CharMotionEndCheck(player->charNo) != 0) {
                CharModelAttrSet(player->charNo, 0x40000004);
                Hu3DModelAttrSet(player->hookMdl, 0x40000004);
                player->state++;
            }
            break;
        case 2:
            if (CharMotionEndCheck(player->charNo) != 0) {
                CharModelAttrReset(player->charNo, 0x40000004);
                Hu3DModelAttrReset(player->hookMdl, 0x40000004);
                CharModelAttrSet(player->charNo, 0x40000002);
                Hu3DModelAttrSet(player->hookMdl, 0x40000002);
                ret = 1;
            }
            break;
    }
    return ret;
}

void fn_1_959C(s16 map[][5], s16 row, s16 col) {
    s32 d;
    s32 t;
    s32 i;

    for (i = 0; i < 3; i++) {
        if (map[row + 1][col] == 0 || map[row + 1][col] == 2) {
            map[11][i] = 0;
            if (row < 8) {
                row++;
            }
        } else {
            d = fn_1_9694(map, row, col);
            if (d < 0) {
                t = 2;
            } else {
                t = 1;
            }
            map[11][i] = (s16)t;
            col += d;
        }
    }
}

s32 fn_1_9694(s16 map[][5], s16 row, s16 col) {
    s32 right = col + 1;
    s32 left = col - 1;
    s32 r;
    s32 ret;

    if (map[row][left] == -1) {
        return 1;
    }
    if (map[row][right] == -1) {
        return -1;
    }
    for (r = row + 1; r < 9; r++) {
        if (map[r][left] == -1) {
            return 1;
        }
        if (map[r][right] == -1) {
            return -1;
        }
    }
    if ((s32)frandmod(2) != 0) {
        ret = 1;
    } else {
        ret = -1;
    }
    return ret;
}

s32 fn_1_979C(M567FXNODE *node) {
    s32 ret = 0;
    M567BODY *body = node->arg;

    if (body->mtn[1] == Hu3DMotionIDGet(body->mdl) && Hu3DMotionEndCheck(body->mdl) != 0) {
        Hu3DMotionSet(body->mdl, body->mtn[0]);
        Hu3DModelAttrSet(body->mdl, 0x40000001);
        ret = 1;
    }
    return ret;
}

s32 fn_1_9828(M567FXNODE *node) {
    s32 ret = 0;
    M567SLOT *slot = node->arg;

    if (Hu3DMotionEndCheck(slot->fxMdl) != 0) {
        Hu3DModelAttrSet(slot->fxMdl, 1);
        ret = 1;
    }
    return ret;
}

s32 fn_1_988C(M567FXNODE *node) {
    s32 ret = 0;
    M567BODY *body = node->arg;

    if (Hu3DMotionEndCheck(body->fxMdl) != 0) {
        Hu3DModelAttrSet(body->fxMdl, 1);
        ret = 1;
    }
    return ret;
}

s32 fn_1_98F0(M567FXNODE *node) {
    M567FXREC *rec;
    s32 c0;
    s32 c1;
    s32 ret;
    f32 t;
    f32 r;

    ret = 0;
    rec = node->arg;
    switch (rec->state) {
        case 0:
            rec->timer = 0;
            rec->state++;
        case 1:
            rec->timer++;
            rec->timer = rec->timer % 30;
            r = (f32)rec->timer / 30.0f;
            t = sin(3.141592653589793 * (180.0f * r) / 180.0);
            c0 = (s32)(150.0f + 94.0f * t);
            c1 = (s32)(100.0f + 100.0f * t);
            rec->mat->color[0] = c0;
            rec->mat->color[1] = c1;
            rec->mat->color[2] = 0;
            break;
        case 2:
            rec->timer++;
            if (rec->timer == 0x1E) {
                ret = 1;
            }
            r = (f32)rec->timer / 30.0f;
            t = sin(3.141592653589793 * (180.0f * r) / 180.0);
            c0 = (s32)(150.0f + 94.0f * t);
            c1 = (s32)(100.0f + 100.0f * t);
            rec->mat->color[0] = c0;
            rec->mat->color[1] = c1;
            rec->mat->color[2] = 0;
            break;
    }
    return ret;
}
