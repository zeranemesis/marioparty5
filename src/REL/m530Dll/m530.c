#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/sprite.h"
#include "game/wipe.h"
#include "math.h"

#include "game/mg/score.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"

#include "REL/m530Dll.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* Declared in game/object.h, but a local redeclaration must keep SHARED_SYM. */
SHARED_SYM extern s16 omSysExitReq;

typedef struct m530light_s {
    Vec pos; //0x00
    Vec dir; //0x0C
    GXColor color; //0x18
} M530_LIGHT;

typedef struct m530cammove_s {
    M530_CAM *cam; //0x00
    M530_CAM start; //0x04
    M530_CAM end; //0x4C
    f32 totalTime; //0x94
    f32 time; //0x98
    s32 moveF; //0x9C
} M530_CAMMOVE;

typedef void (*M530_FLOWFUNC)(OMOBJ *obj, void *tbl);

typedef struct m530flow_s {
    s32 index; //0x00
    M530_FLOWFUNC *funcTbl; //0x04
    s32 funcNum; //0x08
    s16 gameMesId; //0x0C
    s16 pad0E; //0x0E
    MGTIMER *timer; //0x10
    s32 timerType; //0x14
    s32 maxTime; //0x18
    s32 endTime; //0x1C
    s32 recordTime; //0x20
    s32 pad24; //0x24
    s32 work[16]; //0x28
} M530_FLOW;

#define M530_PI 3.141592653589793
#define M530_RAD(x) ((M530_PI * (f64)(x)) / 180.0)

#ifdef __MWERKS__
#define M530_RODATA __declspec(section ".rodata")
#else
#define M530_RODATA
#endif

char lbl_1_data_0[45] = "MgCameraMoveEntry : Camera Number Different\n";
char lbl_1_data_2D[31] = "MgFlowSet : Value is Max Over\n";
char lbl_1_data_4C[32] = "MgFlowNext : Value is Max Over\n";

M530_CAM lbl_1_data_6C = {
    1, 1, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 0.0f
};

Vec lbl_1_data_B4 = { 0.0f, 0.0f, 0.0f };

unsigned int lbl_1_data_C0[15] = {
    0x8C0000, 0x8C0001, 0x8C0002, 0x8C0040, 0x8C0021, 0x8C0041, 0x870058, 0x8C004A,
    0x8C004B, 0x870056, 0x870057, 0x8C000E, 0x8C0006, 0x8C0007, 0,
};

s32 lbl_1_data_FC[4] = { 0x46, 0x3C, 0x1F6, 0x3C };

void fn_1_1DC(s16 mode, s16 frameNo);
void fn_1_C00(s16 mode, s16 frameNo);
void fn_1_C58(s16 mode, s16 frameNo);
void fn_1_CE0(s16 mode, s16 frameNo);
void fn_1_D00(s16 mode, s16 frameNo);
void fn_1_DAC(s16 mode, s16 frameNo);
void fn_1_DDC(s16 mode, s16 frameNo);
void fn_1_F48(s16 mode, s16 frameNo);

MGSEQ_PARAM lbl_1_data_10C = {
    30, 0, fn_1_1DC, fn_1_C00, NULL, fn_1_CE0, fn_1_D00, fn_1_DAC, fn_1_DDC, NULL, fn_1_F48,
};

char lbl_1_data_134[7] = "light1";
char lbl_1_data_13B[7] = "planet";
char lbl_1_data_142[9] = "%d : %f\n";

OMOBJMAN *lbl_1_bss_B8;
s16 lbl_1_bss_B4;
M530_CAM lbl_1_bss_6C;
M530_SIDE lbl_1_bss_4C[2];
s16 lbl_1_bss_44[4];
OMOBJ **lbl_1_bss_40;
s16 lbl_1_bss_3C;
f32 lbl_1_bss_34[2];
s16 lbl_1_bss_30[2];
s16 lbl_1_bss_2C[2];
MGSCORE *lbl_1_bss_24[2];
s16 lbl_1_bss_20[2];
M530_LIGHT lbl_1_bss_4;
s32 lbl_1_bss_0;

static const s32 lbl_1_rodata_8C[3] = { 0x580000, 0x580003, 0x580004 };

void ObjectSetup(void);
void fn_1_F4C(OMOBJ *obj);
void fn_1_FB0(OMOBJ *obj);
void fn_1_16D8(OMOBJ *obj);
void fn_1_1768(void);
void fn_1_11DC(void);
void fn_1_1524(void);
void fn_1_1DE0(M530_CAM *cam);
void fn_1_23E0(M530_CAM *cam);
void fn_1_2AA4(M530_CAM *cam);
void fn_1_1E8C(OMOBJ *obj);
void fn_1_2EA8(OMOBJ *obj);
void fn_1_3228(OMOBJ *obj);
void fn_1_3770(void);
void fn_1_36C0(OMOBJ *obj);

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
    lbl_1_bss_B8 = omInitObjMan(200, 0x1000);
    lbl_1_bss_B4 = -1;
    omGameSysInit(lbl_1_bss_B8);
    MgSeqCreate(&lbl_1_data_10C);
    MgSeqModeHookAdd(4, fn_1_C58);
    lbl_1_bss_3C = -1;
}

void fn_1_120(void) {
    s32 i;
    M530_PLAYER_WORK *work;

    for (i = 0; i < 4; i++) {
        work = lbl_1_bss_40[i]->data;
        work->slot = work->side & 1;
        lbl_1_bss_4C[work->side / 2].member[work->slot] = i;
        work->side = work->side / 2;
    }
    lbl_1_bss_4C[0].score = 0;
    lbl_1_bss_4C[1].score = 0;
}

#define M530_CAM_FROM_POS(cam) \
    do {                                                                                                              \
        dx = (cam)->pos.x - (cam)->target.x;                                                                          \
        dy = (cam)->pos.y - (cam)->target.y;                                                                          \
        dz = (cam)->pos.z - (cam)->target.z;                                                                          \
        dist = M530Sqrtf((dz * dz) + ((dx * dx) + (dy * dy)));                                                        \
        (cam)->dist = dist;                                                                                           \
        angY = (f32)(180.0 * (atan2(dz, dx) / M530_PI));                                                              \
        (cam)->rot.y = -(angY - 90.0f);                                                                               \
        for (i = 0; i < 4; i++) {                                                                                     \
            for (j = 0; j < 4; j++) {                                                                                 \
                mtx[i][j] = 0.0f;                                                                                     \
            }                                                                                                         \
        }                                                                                                             \
        mtx[0][0] = mtx[2][2] = (f32)cos((M530_PI * -angY) / 180.0);                                                  \
        mtx[2][0] = (f32)-sin((M530_PI * -angY) / 180.0);                                                             \
        mtx[0][2] = (f32)sin((M530_PI * -angY) / 180.0);                                                              \
        mtx[1][1] = mtx[3][3] = 1.0f;                                                                                 \
        rz = (dz * mtx[2][3]) + ((dx * mtx[0][3]) + (dy * mtx[1][3]));                                                \
        (cam)->rot.x = -(f32)(180.0 * (atan2((dz * mtx[2][1]) + ((dx * mtx[0][1]) + (dy * mtx[1][1])),                \
                                             (dz * mtx[2][0]) + ((dx * mtx[0][0]) + (dy * mtx[1][0]))) / M530_PI));   \
        (cam)->rot.z = 0.0f;                                                                                          \
    } while (0)

static inline f32 M530Fmod(f32 x, f64 y) {
    return fmod(x, y);
}

/* inline wrapper: reproduces the extra FPR copy the original emits around PSVECDotProduct */
static inline f32 M530DotA(Vec *a, Vec *b) {
    f32 r = PSVECDotProduct(a, b);
    return r;
}

static inline f32 M530Sqrtf(f32 x) {
    const double _half = .5;
    const double _three = 3.0;
    volatile float y;
    if (x > 0.0f) {
        double guess = __frsqrte((double)x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        y = (float)(x * guess);
        return y;
    }
    return x;
}

void fn_1_1DC(s16 mode, s16 frameNo) {
    s32 i;
    s32 j;
    M530_PLAYER_WORK *work;
    OMOBJ *obj;
    f32 dz;
    f32 dx;
    f32 dy;
    f32 angY;
    f32 dist;
    f32 mtx[4][4];
    f32 rz;

    for (i = 0; i < 3; i++) {
        lbl_1_bss_44[i] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_rodata_8C[i], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelLayerSet(lbl_1_bss_44[i], 0);
        Hu3DModelAttrSet(lbl_1_bss_44[i], 1);
        Hu3DModelPosSet(lbl_1_bss_44[i], 0.0f, 0.0f, 0.0f);
        Hu3DModelRotSet(lbl_1_bss_44[i], 0.0f, 0.0f, 0.0f);
        if (i == 0) {
            Hu3DModelAttrSet(lbl_1_bss_44[0], 0x40000002);
        } else {
            Hu3DModelAttrSet(lbl_1_bss_44[i], 0x40000001);
        }
        Hu3DModelAttrReset(lbl_1_bss_44[i], 1);
    }
    Hu3DMotionCalc(lbl_1_bss_44[0]);
    PSMTXTrans(Hu3DData[lbl_1_bss_44[1]].mtx, 0.0f, 2000.0f, 0.0f);
    Hu3DModelPosSet(lbl_1_bss_44[1], 0.0f, -2000.0f, 0.0f);
    Hu3DCameraCreate(lbl_1_data_6C.camNo);
    Hu3DCameraViewportSet(lbl_1_data_6C.camNo, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DModelCameraInfoSet(lbl_1_bss_44[0], lbl_1_data_6C.camNo);
    Hu3DCameraPosGet(lbl_1_data_6C.camNo, &lbl_1_data_6C.pos, &lbl_1_data_6C.up, &lbl_1_data_6C.target);
    Hu3DCameraPerspectiveGet(lbl_1_data_6C.camNo, &lbl_1_data_6C.fov, &lbl_1_data_6C.nearZ, &lbl_1_data_6C.farZ);
    M530_CAM_FROM_POS(&lbl_1_data_6C);
    fn_1_1DE0(&lbl_1_data_6C);
    Hu3DModelLightInfoSet(lbl_1_bss_44[0], 1);
    Hu3DGLightParamGet(Hu3DModelLightIdGet(lbl_1_bss_44[0], lbl_1_data_134), &lbl_1_bss_4.pos, &lbl_1_bss_4.dir,
                       &lbl_1_bss_4.color);
    Hu3DShadowCreate(lbl_1_data_6C.fov, lbl_1_data_6C.nearZ, lbl_1_data_6C.farZ);
    Hu3DShadowTPLvlSet(0.35f);
    Hu3DShadowPosSet(&lbl_1_bss_4.pos, &lbl_1_data_6C.up, &lbl_1_data_B4);
    Hu3DModelShadowMapObjSet(lbl_1_bss_44[0], lbl_1_data_13B);
    Hu3DModelShadowMapSet(lbl_1_bss_44[2]);
    omMakeGroupEx(lbl_1_bss_B8, 0, 4);
    lbl_1_bss_40 = omGetGroupMemberListEx(lbl_1_bss_B8, 0);
    for (i = 0; i < 4; i++) {
        obj = omAddObjEx(lbl_1_bss_B8, 0x2000, 3, 14, 0, fn_1_F4C);
        lbl_1_bss_40[i] = obj;
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M530_PLAYER_WORK), HU_MEMNUM_OVL);
        work = obj->data;
        work->playerNo = i;
        work->charNo = GwPlayerConf[i].charNo;
        work->padNo = GwPlayerConf[i].padNo;
        work->side = GwPlayerConf[i].grp;
        work->flag = GwPlayerConf[i].type;
        work->varNo = GwPlayerConf[i].dif;
        work->state = 0;
        work->pos.z = 0.0f;
        work->pos.y = 0.0f;
        work->pos.x = 0.0f;
        work->rot.z = 0.0f;
        work->rot.y = 0.0f;
        work->rot.x = 0.0f;
        work->unk30 = 0;
        work->espId = -1;
    }
    CharEffectLayerSet(1);
    fn_1_120();
    fn_1_45A0();
    lbl_1_bss_34[0] = 0.0f;
    omAddObjEx(lbl_1_bss_B8, 0x2000, 0, 0, -1, fn_1_16D8)->work[0] = 0;
    fn_1_11DC();
    MgSeqModeNext();
}

void fn_1_C00(s16 mode, s16 frameNo) {
    if (WipeCheckEnd() == 0 && lbl_1_bss_3C == -1) {
        lbl_1_bss_3C = HuAudSStreamPlay(0x4D);
    }
}

void fn_1_C58(s16 mode, s16 frameNo) {
    s32 i;
    M530_PLAYER_WORK *work;

    Hu3DModelAttrReset(lbl_1_bss_44[0], 0x40000002);
    fn_1_1524();
    for (i = 0; i < 4; i++) {
        work = lbl_1_bss_40[i]->data;
        if (work->espId != -1) {
            espDispOn(work->espId);
        }
    }
}

void fn_1_CE0(s16 mode, s16 frameNo) {
    fn_1_6740();
}

void fn_1_D00(s16 mode, volatile s16 frameNo) {
    s32 i;
    M530_PLAYER_WORK *work;

    if (frameNo == 0) {
        if (lbl_1_bss_3C != -1) {
            HuAudSStreamFadeOut(lbl_1_bss_3C, 100);
        }
        fn_1_69B4();
        for (i = 0; i < 4; i++) {
            work = lbl_1_bss_40[i]->data;
            if (work->espId != -1) {
                espKill(work->espId);
            }
        }
        fn_1_1768();
    }
}

void fn_1_DAC(s16 mode, volatile s16 frameNo) {
    if (frameNo == 0x78) {
        MgSeqModeNext();
    }
}

void fn_1_DDC(s16 mode, s16 frameNo) {
    s32 i;
    M530_PLAYER_WORK *work;
    OMOBJ *obj;

    if (frameNo == 0) {
        for (i = 0; i < 4; i++) {
            work = lbl_1_bss_40[i]->data;
            CharModelAttrReset(work->charNo, 0x40000001);
            CharModelAttrReset(work->charNo, 0x40000002);
            if (work->unk30 != 0) {
                CharMotionSet(work->charNo, lbl_1_bss_40[i]->mtnId[12]);
            } else {
                CharMotionSet(work->charNo, lbl_1_bss_40[i]->mtnId[13]);
                if (lbl_1_bss_0 == 0) {
                    obj = omAddObjEx(lbl_1_bss_B8, 0x2000, 0, 0, -1, fn_1_FB0);
                    obj->work[0] = work->playerNo;
                    obj->work[1] = 13;
                }
            }
        }
    }
    if (lbl_1_bss_0 != 0) {
        if (frameNo == 0) {
            HuAudFXPlay(0x7A1);
        }
        fn_1_6868();
    }
}

void fn_1_F48(s16 mode, s16 frameNo) {
}

void fn_1_F4C(OMOBJ *obj) {
    M530_PLAYER_WORK *work = obj->data;

    switch (work->slot) {
        case 0:
            fn_1_6D7C(obj);
            return;
        case 1:
            fn_1_8434(obj);
    }
}

void fn_1_FB0(OMOBJ *obj) {
    M530_PLAYER_WORK *work;
    HU3DMODELID modelId;
    s16 extra;
    s16 vol;
    s16 pan;
    s32 fxNo;

    work = lbl_1_bss_40[obj->work[0]]->data;
    fxNo = 0x200;
    extra = CharMotionExtraTimeGet(work->charNo, lbl_1_data_C0[obj->work[1]]);
    modelId = lbl_1_bss_40[obj->work[0]]->mdlId[0];
    OSReport(lbl_1_data_142, work->playerNo, Hu3DMotionTimeGet(modelId));
    if (Hu3DMotionTimeGet(modelId) >= (f32)extra && Hu3DMotionShiftIDGet(modelId) < 0) {
        fn_1_1A50(&work->pos.x, &vol, &pan);
        CharFXPlayVolPan(work->charNo, fxNo, vol, pan);
        omDelObjEx(lbl_1_bss_B8, obj);
    }
}

void fn_1_10E8(OMOBJ *obj, s32 anim) {
    M530_PLAYER_WORK *work = obj->data;
    s16 charNo = work->charNo;
    f32 start = 0.0f;
    f32 end = 10.0f;
    u32 attr;

    attr = 0x40000001;
    switch (anim) {
        case 6:
        case 8:
            attr = 0;
            break;
        case 3:
            work->unk74 = 0;
            break;
        case 4:
            work->unk74 = 1;
            break;
        case 5:
            work->unk74 = 2;
            break;
    }
    CharMotionShiftSet(charNo, obj->mtnId[anim], start, end, attr);
}

void fn_1_11DC(void) {
    s32 i;
    MGSCORE *score;
    s16 id;
    ANIMDATA *anim;
    f32 x;
    f32 y;

    for (i = 0; i < 2; i++) {
        x = lbl_1_data_FC[i * 2];
        y = lbl_1_data_FC[i * 2 + 1];
        lbl_1_bss_30[i] = MgScoreWinCreate(0x70, 0x28);
        MgScoreWinPosSet(lbl_1_bss_30[i], x, y);
        MgScoreWinDispSet(lbl_1_bss_30[i], 0);
        score = MgScoreCreate(0x960031, -1, 1);
        MgScoreColorSet(score, 0xFF, 0xD8, 0x15);
        MgScoreMaxDigitSet(score, 2);
        MgScoreValueSet(score, 0);
        MgScorePosSet(score, 14.0f + x, y);
        MgScoreDispOff(score);
        lbl_1_bss_24[i] = score;
        anim = score->digitAnim;
        score = MgScoreCreate(0x960030, -1, 1);
        anim = score->digitAnim;
        MgScoreDispOff(score);
        id = HuSprCreate(anim, 0x10, 0xF);
        lbl_1_bss_20[i] = HuSprGrpCreate(1);
        HuSprGrpMemberSet(lbl_1_bss_20[i], 0, id);
        HuSprAttrSet(lbl_1_bss_20[i], 0, 4);
        HuSprColorSet(lbl_1_bss_20[i], 0, 0xFF, 0xFF, 0xFF);
        HuSprPosSet(lbl_1_bss_20[i], 0, -8.0f + x, y);
        id = espEntry(0x58000F, 0x10, 0);
        espDrawNoSet(id, 0);
        espPosSet(id, -32.0f + x, y);
        espDispOff(id);
        lbl_1_bss_2C[i] = id;
    }
    MgScoreWinColorSet(lbl_1_bss_30[0], 0xFA, 0, 0x1E);
    MgScoreWinColorSet(lbl_1_bss_30[1], 0, 0x32, 0xFA);
}

void fn_1_1524(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        MgScoreWinDispSet(lbl_1_bss_30[i], 1);
        espDispOn(lbl_1_bss_2C[i]);
        HuSprAttrReset(lbl_1_bss_20[i], 0, 4);
        MgScoreDispOn(lbl_1_bss_24[i]);
    }
}

void fn_1_15C8(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        espKill(lbl_1_bss_2C[i]);
        HuSprGrpKill(lbl_1_bss_20[i]);
        MgScoreKill(lbl_1_bss_24[i]);
    }
}

void fn_1_1648(s32 side) {
    if (lbl_1_bss_4C[side].score <= 99) {
        MgScoreValueSet(lbl_1_bss_24[side], lbl_1_bss_4C[side].score);
        return;
    }
    MgScoreValueSet(lbl_1_bss_24[side], 99);
}

void fn_1_16D8(OMOBJ *obj) {
    obj->work[0]++;
    lbl_1_bss_34[0] -= 0.09615385f;
    Hu3DModelRotSet(lbl_1_bss_44[1], 0.0f, 0.0f, lbl_1_bss_34[0]);
}

void fn_1_1768(void) {
    s16 winners[4];
    s32 i;
    s16 idx;
    M530_PLAYER_WORK *work;

    for (i = 0; i < 4; i++) {
        winners[i] = -1;
    }
    if (lbl_1_bss_4C[0].score > lbl_1_bss_4C[1].score) {
        idx = lbl_1_bss_4C[0].member[0];
        work = lbl_1_bss_40[idx]->data;
        work->unk30 = 1;
        winners[idx] = work->charNo;
        if (_CheckFlag(0x1000F) == 0) {
            GwPlayer[idx].mgCoinBonus = 10;
        }
        idx = lbl_1_bss_4C[0].member[1];
        work = lbl_1_bss_40[idx]->data;
        work->unk30 = 1;
        winners[idx] = work->charNo;
        if (_CheckFlag(0x1000F) == 0) {
            GwPlayer[idx].mgCoinBonus = 10;
        }
    } else if (lbl_1_bss_4C[0].score < lbl_1_bss_4C[1].score) {
        idx = lbl_1_bss_4C[1].member[0];
        work = lbl_1_bss_40[idx]->data;
        work->unk30 = 1;
        winners[idx] = work->charNo;
        if (_CheckFlag(0x1000F) == 0) {
            GwPlayer[idx].mgCoinBonus = 10;
        }
        idx = lbl_1_bss_4C[1].member[1];
        work = lbl_1_bss_40[idx]->data;
        work->unk30 = 1;
        winners[idx] = work->charNo;
        if (_CheckFlag(0x1000F) == 0) {
            GwPlayer[idx].mgCoinBonus = 10;
        }
    }
    MgSeqWinnerSet(winners[0], winners[1], winners[2], winners[3]);
    lbl_1_bss_0 = 0;
    for (i = 0; i < 4; i++) {
        work = lbl_1_bss_40[i]->data;
        if (work->unk30 != 0) {
            lbl_1_bss_0 = 1;
        }
    }
}

void fn_1_1A50(f32 *pos, s16 *vol, s16 *pan) {
    Vec screen;
    f32 depth;

    Hu3D3Dto2D((Vec *)pos, (s16)lbl_1_data_6C.camNo, &screen);
    *pan = (s16)(32.0f + 64.0f * (screen.x / 576.0f));
    depth = 550.0f + pos[2];
    *vol = (s16)(96.0f + 31.0f * (2.0f * (depth / 550.0f)));
    if (*vol > 0x7F) {
        *vol = 0x7F;
    }
}

void fn_1_1B78(f32 *pos, s16 fxId) {
    Vec screen;
    s16 vol;
    s32 pan;
    f32 depth;

    Hu3D3Dto2D((Vec *)pos, (s16)lbl_1_data_6C.camNo, &screen);
    pan = (s32)(32.0f + 64.0f * (screen.x / 576.0f));
    depth = 550.0f + pos[2];
    vol = (s16)(96.0f + 31.0f * (2.0f * (depth / 550.0f)));
    if (vol > 0x7F) {
        vol = 0x7F;
    }
    HuAudFXPlayVolPan(fxId, vol, pan);
}

void fn_1_1CA0(void) {
    lbl_1_bss_B8 = omInitObjMan(200, 0x1000);
    lbl_1_bss_B4 = -1;
    omGameSysInit(lbl_1_bss_B8);
}

M530_CAM *fn_1_1CF4(M530_CAM *src) {
    M530_CAM *cam;

    cam = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M530_CAM), HU_MEMNUM_OVL);
    *cam = *src;
    Hu3DCameraCreate(src->camNo);
    Hu3DCameraViewportSet(src->camNo, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(src->camNo, src->fov, src->nearZ, src->farZ, 1.2f);
    fn_1_1DE0(src);
    return cam;
}

void fn_1_1DE0(M530_CAM *cam) {
    if (cam->state == 1) {
        fn_1_23E0(cam);
    }
    Hu3DCameraPosSetV(cam->camNo, &cam->pos, &cam->up, &cam->target);
}

HU3DLIGHTID fn_1_1E30(M530_LIGHT *light) {
    HU3DLIGHTID lightId;

    lightId = Hu3DGLightCreateV(&light->pos, &light->dir, &light->color);
    Hu3DGLightStaticSet(lightId, 1);
    Hu3DGLightInfinitytSet(lightId);
    return lightId;
}

void fn_1_1E8C(OMOBJ *obj) {
    M530_CAM *cam = (M530_CAM *)obj->work[0];

    if ((HuPadBtn[0] & 0x800) != 0) {
        if ((HuPadBtnDown[0] & 0x1000) != 0) {
            *cam = lbl_1_bss_6C;
        }
        if ((HuPadBtnDown[0] & 0x200) != 0) {
            obj->work[1]++;
            if (obj->work[1] == 2) {
                obj->work[1] = 0;
            }
        }
        switch (obj->work[1]) {
            case 0:
                cam->pos.z = cam->pos.z - (0.1f * (f32)HuPadTrigL[0]);
                cam->pos.z = cam->pos.z + (0.1f * (f32)HuPadTrigR[0]);
                cam->pos.x = cam->pos.x + (0.1f * (f32)HuPadStkX[0]);
                cam->pos.y = cam->pos.y + (0.1f * (f32)HuPadStkY[0]);
                fn_1_2AA4(cam);
                break;
            case 1:
                cam->rot.x = cam->rot.x - (0.1f * (f32)HuPadStkY[0]);
                cam->rot.y = cam->rot.y + (0.1f * (f32)HuPadStkX[0]);
                cam->dist = cam->dist - (0.1f * (f32)HuPadTrigL[0]);
                cam->dist = cam->dist + (0.1f * (f32)HuPadTrigR[0]);
                fn_1_23E0(cam);
                break;
        }
        if ((HuPadBtn[0] & 0x400) != 0) {
            cam->target.z = cam->target.z - (0.1f * (f32)HuPadSubStkY[0]);
        } else {
            cam->target.x = cam->target.x + (0.1f * (f32)HuPadSubStkX[0]);
            cam->target.y = cam->target.y + (0.1f * (f32)HuPadSubStkY[0]);
        }
        Hu3DCameraPosSetV(cam->camNo, &cam->pos, &cam->up, &cam->target);
    }
}

void fn_1_2304(M530_CAM *cam) {
    OMOBJ *obj;

    obj = omAddObjEx(lbl_1_bss_B8, 0x4000, 0, 0, -1, fn_1_1E8C);
    obj->work[0] = (uintptr_t)cam;
    if (cam->state == 0) {
        obj->work[1] = 0;
    } else {
        obj->work[1] = 1;
    }
    lbl_1_bss_6C = *cam;
}

void fn_1_23B0(M530_CAM *src) {
    lbl_1_bss_6C = *src;
}

void fn_1_23E0(M530_CAM *cam) {
    Vec axis;
    f32 rx;
    f32 ry;
    f32 rz;
    f32 tA;
    f32 tB;
    f32 tC;

    rx = cam->rot.x;
    ry = cam->rot.y;
    rz = cam->rot.z;
    cam->pos.x = (f32)((f64)cam->target.x + ((f64)cam->dist * (sin(M530_RAD(ry)) * cos(M530_RAD(rx)))));
    cam->pos.y = (f32)((f64)cam->target.y + ((f64)cam->dist * -sin(M530_RAD(rx))));
    cam->pos.z = (f32)((f64)cam->target.z + ((f64)cam->dist * (cos(M530_RAD(ry)) * cos(M530_RAD(rx)))));
    PSVECSubtract(&cam->pos, &cam->target, &axis);
    PSVECNormalize(&axis, &axis);
    tA = (f32)(sin(M530_RAD(ry)) * sin(M530_RAD(rx)));
    tB = (f32)cos(M530_RAD(rx));
    tC = (f32)(cos(M530_RAD(ry)) * sin(M530_RAD(rx)));
    cam->up.x = (f32)(((f64)tA * ((f64)(axis.x * axis.x) + ((f64)(1.0f - (axis.x * axis.x)) * cos(M530_RAD(rz)))))
                      + ((f64)tB * (((f64)(axis.x * axis.y) * (1.0 - cos(M530_RAD(rz)))) - ((f64)axis.z * sin(M530_RAD(rz)))))
                      + ((f64)tC * (((f64)(axis.x * axis.z) * (1.0 - cos(M530_RAD(rz)))) + ((f64)axis.y * sin(M530_RAD(rz))))));
    cam->up.y = (f32)(((f64)tB * ((f64)(axis.y * axis.y) + ((f64)(1.0f - (axis.y * axis.y)) * cos(M530_RAD(rz)))))
                      + ((f64)tA * (((f64)(axis.x * axis.y) * (1.0 - cos(M530_RAD(rz)))) + ((f64)axis.z * sin(M530_RAD(rz)))))
                      + ((f64)tC * (((f64)(axis.y * axis.z) * (1.0 - cos(M530_RAD(rz)))) - ((f64)axis.x * sin(M530_RAD(rz))))));
    cam->up.z = (f32)(((f64)tC * ((f64)(axis.z * axis.z) + ((f64)(1.0f - (axis.z * axis.z)) * cos(M530_RAD(rz)))))
                      + (((f64)tA * (((f64)(axis.x * axis.z) * (1.0 - cos(M530_RAD(rz)))) - ((f64)axis.y * sin(M530_RAD(rz)))))
                         + ((f64)tB * (((f64)(axis.y * axis.z) * (1.0 - cos(M530_RAD(rz)))) + ((f64)axis.x * sin(M530_RAD(rz)))))));
    PSVECNormalize(&cam->up, &cam->up);
}

void fn_1_2AA4(M530_CAM *cam) {
    s32 i;
    s32 j;
    f32 dz;
    f32 dx;
    f32 dy;
    f32 angY;
    f32 dist;
    f32 mtx[4][4];
    f32 rz;

    M530_CAM_FROM_POS(cam);
}

void fn_1_2EA8(OMOBJ *obj) {
    M530_CAM *start;
    M530_CAM *cam;
    M530_CAM *end;
    M530_CAMMOVE *move = obj->data;
    f32 ratio;

    if ((move->moveF & 2) != 0) {
        cam = move->cam;
        start = &move->start;
        end = &move->end;
        move->time += 1.0f;
        ratio = move->time / move->totalTime;
        if (cam->state == 0) {
            cam->pos.x = start->pos.x + (ratio * (end->pos.x - start->pos.x));
            cam->pos.y = start->pos.y + (ratio * (end->pos.y - start->pos.y));
            cam->pos.z = start->pos.z + (ratio * (end->pos.z - start->pos.z));
        } else {
            cam->dist = start->dist + (ratio * (end->dist - start->dist));
            cam->rot.x = start->rot.x + (ratio * (end->rot.x - start->rot.x));
            cam->rot.y = start->rot.y + (ratio * (end->rot.y - start->rot.y));
        }
        cam->target.x = start->target.x + (ratio * (end->target.x - start->target.x));
        cam->target.y = start->target.y + (ratio * (end->target.y - start->target.y));
        cam->target.z = start->target.z + (ratio * (end->target.z - start->target.z));
        if (move->time >= move->totalTime) {
            *cam = *end;
            move->moveF = move->moveF & ~2;
        }
        fn_1_1DE0(cam);
    }
}

OMOBJ *MgCameraMoveEntry(M530_CAM *cam, M530_CAM *dest, f32 time) {
    OMOBJ *obj;
    M530_CAMMOVE *move;

    if (cam->camNo != dest->camNo) {
        OSReport(lbl_1_data_0);
#ifdef TARGET_PC
        return NULL;
#else
        return;
#endif
    }
    obj = omAddObjEx(lbl_1_bss_B8, 0x2000, 0, 0, -1, fn_1_2EA8);
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M530_CAMMOVE), HU_MEMNUM_OVL);
    move = obj->data;
    move->totalTime = time;
    move->time = 0.0f;
    move->cam = cam;
    move->start = *cam;
    move->end = *dest;
    move->moveF = 3;
    if (cam->state == 1) {
        fn_1_23E0(&move->start);
        fn_1_23E0(&move->end);
    }
    return obj;
}

s32 fn_1_31C0(OMOBJ *obj) {
    M530_CAMMOVE *move = obj->data;

    if (move->moveF & 2) {
        return 1;
    } else {
        return 0;
    }
}

void fn_1_31F4(OMOBJ *obj) {
    omDelObjEx(lbl_1_bss_B8, obj);
}

void fn_1_3228(OMOBJ *obj) {
    M530_CAM *cam = obj->data;
    s32 range = obj->work[1];

    if ((u32)obj->work[0] == 0) {
        fn_1_1DE0((M530_CAM *)obj->work[2]);
        omDelObjEx(lbl_1_bss_B8, obj);
        return;
    }
    cam->target.x = ((M530_CAM *)obj->work[2])->target.x + (f32)(s32)frandmod(range * 2) - (f32)range;
    cam->target.y = ((M530_CAM *)obj->work[2])->target.y + (f32)(s32)frandmod(range * 2) - (f32)range;
    fn_1_1DE0(cam);
    obj->work[0]--;
}

void fn_1_3370(M530_CAM *src, s32 count, s32 range) {
    OMOBJ *obj;
    M530_CAM *dst;

    obj = omAddObjEx(lbl_1_bss_B8, 0x2000, 0, 0, -1, fn_1_3228);
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M530_CAM), HU_MEMNUM_OVL);
    dst = obj->data;
    obj->work[0] = count;
    obj->work[1] = range;
    *dst = *src;
    obj->work[2] = (uintptr_t)src;
}

f32 fn_1_342C(s16 padNo) {
    f32 len;

    len = (f32)((HuPadStkX[padNo] * HuPadStkX[padNo]) + (HuPadStkY[padNo] * HuPadStkY[padNo]));
    return M530Sqrtf(len);
}

f32 fn_1_35B0(s16 padNo) {
    f32 ang;

    ang = (f32)((180.0 * (atan2((f32)HuPadStkY[padNo], (f32)HuPadStkX[padNo]) / M530_PI)) - 90.0);
    if (ang < 0.0f) {
        ang += 360.0f;
    }
    return ang;
}

void fn_1_36C0(OMOBJ *obj) {
    M530_FLOW *flow = obj->data;

    if (omSysExitReq != 0) {
        if (lbl_1_bss_B4 != -1) {
            HuAudFadeOut(1000);
            lbl_1_bss_B4 = -1;
        }
        obj->objFunc = (OMOBJFUNC)fn_1_3770;
        WipeCreate(2, 6, 60);
        return;
    }
    (*(flow->index + flow->funcTbl))(obj, flow->funcTbl);
}

void fn_1_3770(void) {
    if (WipeCheckEnd() == 0) {
        omOvlReturnEx(1, 1);
    }
}

OMOBJ *fn_1_37A8(M530_FLOWFUNC *funcTbl) {
    M530_FLOW *flow;
    OMOBJ *obj;
    s32 i;
    s32 j;

    i = 0;
    while (funcTbl[i] != NULL) {
        i++;
    }
    obj = omAddObjEx(lbl_1_bss_B8, 0x2000, 0, 0, -1, fn_1_36C0);
    flow = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M530_FLOW), HU_MEMNUM_OVL);
    obj->data = flow;
    flow->funcNum = i;
    flow->funcTbl = funcTbl;
    flow->index = 0;
    flow->gameMesId = -1;
    for (j = 0; j < 16; j++) {
        flow->work[j] = 0;
    }
    return obj;
}

void MgFlowSet(OMOBJ *obj, s32 index) {
    M530_FLOW *flow = obj->data;

    if (index >= flow->funcNum) {
        OSReport(lbl_1_data_2D);
        return;
    }
    flow->index = index;
}

void MgFlowNext(OMOBJ *obj) {
    M530_FLOW *flow = obj->data;

    if (flow->index == flow->funcNum - 1) {
        OSReport(lbl_1_data_4C);
        return;
    }
    flow->index++;
}

s32 fn_1_3948(OMOBJ *obj) {
    M530_FLOW *flow = obj->data;

    return flow->index;
}

s32 fn_1_3964(OMOBJ *obj, s32 index) {
    M530_FLOW *flow = obj->data;

    if (index == fn_1_3948(obj)) {
        return 1;
    }
    return 0;
}

void fn_1_39B0(OMOBJ *obj, s32 type, s32 maxTime, s32 endTime, s32 recordTime) {
    M530_FLOW *flow = obj->data;

    flow->timerType = type;
    flow->maxTime = maxTime * 60;
    flow->endTime = endTime * 60;
    flow->recordTime = recordTime * 60;
}

void fn_1_39E4(OMOBJ *obj) {
    M530_FLOW *flow = obj->data;

    MgTimerDoneCheck(flow->timer);
}

void fn_1_3A20(OMOBJ *obj) {
    M530_FLOW *flow = obj->data;

    if (flow->timer->stopF == 0) {
        flow->timer->stopF = 1;
        flow->timer->mode = 1;
    }
}

void fn_1_3A60(void) {
}

void fn_1_3A64(OMOBJ *obj) {
    if (WipeCheckEnd() == 0) {
        MgFlowNext(obj);
    }
}

void fn_1_3ADC(void) {
    if (lbl_1_bss_B4 != -1) {
        HuAudFadeOut(1000);
        lbl_1_bss_B4 = -1;
    }
    if (WipeCheckEnd() == 0) {
        omOvlReturnEx(1, 1);
    }
}

void fn_1_3B40(OMOBJ *obj, s16 mesNo, s16 mesType) {
    M530_FLOW *flow = obj->data;

    if (flow->gameMesId == -1) {
        flow->gameMesId = GameMesCreate(mesNo, mesType);
        return;
    }
    if (GameMesStatGet(flow->gameMesId) == 0) {
        flow->gameMesId = -1;
        MgFlowNext(obj);
    }
}

void fn_1_3BFC(OMOBJ *obj) {
    M530_FLOW *flow = obj->data;

    if (flow->gameMesId == -1) {
        flow->gameMesId = GameMesCreate(2, 0);
        return;
    }
    if (GameMesStatGet(flow->gameMesId) == 0) {
        flow->gameMesId = -1;
        MgFlowNext(obj);
    }
}

void fn_1_3CB0(OMOBJ *obj) {
    M530_FLOW *flow = obj->data;

    if (flow->gameMesId == -1) {
        flow->gameMesId = GameMesCreate(2, 1);
        return;
    }
    if (GameMesStatGet(flow->gameMesId) == 0) {
        flow->gameMesId = -1;
        MgFlowNext(obj);
    }
}

void fn_1_3D64(OMOBJ *obj) {
    M530_FLOW *flow = obj->data;

    flow->timer = MgTimerCreate(flow->timerType);
    MgTimerParamSet(flow->timer, flow->maxTime, flow->endTime, flow->recordTime);
    MgTimerModeOnSet(flow->timer, 1);
    MgFlowNext(obj);
}

void fn_1_3E04(M530_PLAYER_WORK *work, s16 playerNo) {
    work->playerNo = playerNo;
    work->charNo = GwPlayerConf[playerNo].charNo;
    work->padNo = GwPlayerConf[playerNo].padNo;
    work->side = GwPlayerConf[playerNo].grp;
    work->flag = GwPlayerConf[playerNo].type;
    work->varNo = GwPlayerConf[playerNo].dif;
    work->state = 0;
    work->pos.x = work->pos.y = work->pos.z = 0.0f;
    work->rot.x = work->rot.y = work->rot.z = 0.0f;
}

s32 fn_1_3ED0(Vec *pos, Vec *target, Vec *angle, f32 step) {
    Vec vec;
    f32 mag;

    PSVECSubtract(target, pos, &vec);
    mag = PSVECMag(&vec);
    if (mag == 0.0f) {
        return 1;
    }
    PSVECNormalize(&vec, &vec);
    PSVECScale(&vec, &vec, step);
    if (PSVECMag(&vec) >= mag) {
        *pos = *target;
        return 1;
    }
    PSVECAdd(pos, &vec, pos);
    angle->y = (f32)(180.0 * (atan2((f64)vec.x, (f64)vec.z) / M530_PI));
    return 0;
}

f32 fn_1_3FF4(f32 cur, f32 target, f32 rate) {
    f32 diff;
    f32 result;

    diff = M530Fmod(target - cur, 360.0);
    if (diff < 0.0f) {
        diff += 360.0f;
    }
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    if (fabs(diff) > rate) {
        if (diff < 0.0f) {
            diff = -rate;
        } else {
            diff = rate;
        }
    }
    result = M530Fmod(cur + diff, 360.0);
    if (result < 0.0f) {
        result += 360.0f;
    }
    return result;
}

s32 fn_1_4164(Vec *pos, Vec *target, Vec *cur, f32 dist) {
    Vec sp20;
    Vec sp14;
    Vec sp8;
    f32 mag;
    f32 radius;
    f32 ang;

    sp8 = *target;
    while (TRUE) {
        PSVECSubtract(target, pos, &sp20);
        if (PSVECMag(&sp20) == 0.0f) {
            return 0;
        }
        PSVECSubtract(target, cur, &sp14);
        mag = PSVECMag(&sp14);
        if (mag >= dist) {
            return 0;
        }
        if (mag == 0.0f) {
            *target = *pos;
        } else {
            PSVECNormalize(&sp14, &sp14);
            PSVECScale(&sp14, &sp14, dist);
            PSVECAdd(cur, &sp14, target);
        }
        PSVECSubtract(target, pos, &sp14);
        if (PSVECMag(&sp14) >= 0.001f) {
            return 1;
        }
        radius = PSVECMag(&sp20);
        ang = (f32)(180.0 * (atan2((f64)sp14.z, (f64)sp14.x) / M530_PI));
        ang += 30.0f;
        sp20.x = (f32)((f64)radius * sin(M530_RAD(ang)));
        sp20.z = (f32)((f64)radius * cos(M530_RAD(ang)));
        PSVECAdd(pos, &sp20, target);
    }
}

void fn_1_439C(Vec *a, Vec *b, Vec *c, Vec *out) {
    Vec sp1C;
    Vec sp10;

    PSVECSubtract(c, b, &sp1C);
    PSVECSubtract(b, a, &sp10);
    PSVECCrossProduct(&sp1C, &sp10, out);
    PSVECNormalize(out, out);
}

f32 fn_1_4414(Vec *normal, Vec *planePt, Vec *pos) {
    Vec sp14;

    PSVECSubtract(pos, planePt, &sp14);
    return PSVECDotProduct(normal, &sp14);
}

s32 fn_1_4458(Vec *normal, Vec *planePt, Vec *a, Vec *b, Vec *out) {
    Vec sp28;
    Vec sp1C;
    Vec sp10;
    f32 denom;
    f32 numer;

    PSVECSubtract(b, a, &sp28);
    PSVECSubtract(a, planePt, &sp1C);
    numer = M530DotA(normal, &sp1C);
    PSVECSubtract(a, b, &sp10);
    denom = M530DotA(normal, &sp10);
    if (denom == 0.0f) {
        return 0;
    }
    PSVECScale(&sp28, &sp28, numer / denom);
    PSVECAdd(a, &sp28, out);
    return 1;
}
