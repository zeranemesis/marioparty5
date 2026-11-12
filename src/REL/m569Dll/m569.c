#include "REL/m569Dll.h"

#include "game/mg/seqman.h"
#include "game/mg/timer.h"
#include "game/hu3d.h"
#include "game/hsfex.h"
#include "game/sprite.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/wipe.h"
#include "game/pad.h"
#include "game/memory.h"

#include "dolphin/mtx.h"

#include "math.h"

#define M569_RAD(x) (3.141592653589793 * (f64)(x) / 180.0)

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

typedef void (*M569FLOWFUNC)(OMOBJ *obj);

/* Flow controller (0x68 bytes), OMOBJ::data of the flow object. */
typedef struct m569Flow_s {
    /* 0x00 */ s32 idx;
    /* 0x04 */ M569FLOWFUNC *tbl;
    /* 0x08 */ s32 count;
    /* 0x0C */ s16 mesId;
    /* 0x10 */ MGTIMER *timer;
    /* 0x14 */ s32 timerType;
    /* 0x18 */ s32 timerMax;
    /* 0x1C */ s32 timerEnd;
    /* 0x20 */ s32 timerSpeed;
    /* 0x24 */ s32 pad24;
    /* 0x28 */ s32 work[16];
} M569FLOW;

/* Camera move object data (0xA0 bytes). */
typedef struct m569Move_s {
    /* 0x00 */ M569CAM *src;
    /* 0x04 */ M569CAM from_cam;
    /* 0x4C */ M569CAM to;
    /* 0x94 */ f32 progress;
    /* 0x98 */ f32 accum;
    /* 0x9C */ s32 flags;
} M569MOVE;

void ObjectSetup(void);
void fn_1_110(s16 mode, s16 frame);
void fn_1_820(s16 mode, s16 frame);
void fn_1_94C(s16 mode, s16 frame);
void fn_1_9C4(s16 mode, s16 frame);
void fn_1_DD0(s16 mode, s16 frame);
void fn_1_E20(s16 mode, s16 frame);
void fn_1_FA8(s16 mode, s16 frame);
void fn_1_1010(s16 mode, s16 frame);
void fn_1_1888(M569CAM *cam);
void fn_1_1F4C(M569CAM *cam);
OMOBJ *MgCameraMoveEntry(M569CAM *from, M569CAM *to, f32 frames);

/* ---------------------------------------------------------------- data */

static const s32 lbl_1_rodata_8C[5] = { 0x7C0000, 0x7C0001, 0x7C0002, 0x7C0003, 0x7C0004 };

static M569CAM lbl_1_data_6C = {
    1, 1, 0.0f, 0.0f, 0.0f,
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f },
    { 0.0f, 100.0f, -800.0f },
    { -10.0f, 0.0f, 0.0f },
    900.0f
};
static HuVecF lbl_1_data_B4 = { 0.0f, 2500.0f, 100.0f };
static HuVecF lbl_1_data_C0 = { 0.0f, 1.0f, 0.0f };
static HuVecF lbl_1_data_CC = { 0.0f, 0.0f, 0.0f };
HuVecF lbl_1_data_D8[5] = {
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { -350.0f, 450.0f, -200.0f },
    { 350.0f, 450.0f, -200.0f },
    { 0.0f, 0.0f, 0.0f }
};
static MGSEQ_PARAM lbl_1_data_114 = {
    0,
    0,
    fn_1_110,
    fn_1_820,
    NULL,
    fn_1_9C4,
    fn_1_DD0,
    fn_1_E20,
    fn_1_FA8,
    NULL,
    fn_1_1010
};
static char lbl_1_data_13C[] = "inf_light1";

static OMOBJ *lbl_1_bss_0;
static s32 lbl_1_bss_4;
static s16 lbl_1_bss_8;
static M569KURIBO *lbl_1_bss_C;
static MGTIMER *lbl_1_bss_10;
s32 lbl_1_bss_14;
static M569LIGHT lbl_1_bss_18;
M569CAM lbl_1_bss_34;
static s16 lbl_1_bss_7C[6];
static M569CAM lbl_1_bss_88;
static s16 lbl_1_bss_D0;
OMOBJMAN *lbl_1_bss_D4;
M569TEAM lbl_1_bss_D8[2];
OMOBJ **lbl_1_bss_F8;

/* ------------------------------------------------------------- functions */

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
    lbl_1_bss_D4 = omInitObjMan(200, 0x1000);
    lbl_1_bss_D0 = -1;
    omGameSysInit(lbl_1_bss_D4);
    MgSeqCreate(&lbl_1_data_114);
    MgSeqModeHookAdd(MGSEQ_MODE_PREMAIN, fn_1_94C);
}

/* fn_1_1F4C's body, shared with fn_1_110 where the original compiler inlined it. */
#define M569_CAM_ROT_CALC(cam)     do {         f32 m[4][4];         f32 dx;         f32 dy;         f32 dz;         f32 ang;         f32 w;         f32 vx;         f32 vy;         s32 i;         s32 j;         dx = (cam)->pos.x - (cam)->target.x;         dy = (cam)->pos.y - (cam)->target.y;         dz = (cam)->pos.z - (cam)->target.z;         (cam)->dist = sqrtf(dz * dz + (dx * dx + dy * dy));         ang = (f32)(180.0 * (atan2(dz, dx) / 3.141592653589793));         (cam)->rot.y = -(ang - 90.0f);         for (i = 0; i < 4; i++) {             for (j = 0; j < 4; j++) {                 m[i][j] = 0.0f;             }         }         m[0][0] = m[2][2] = (f32)cos(M569_RAD(-ang));         m[2][0] = (f32)-sin(M569_RAD(-ang));         m[0][2] = (f32)sin(M569_RAD(-ang));         m[1][1] = m[3][3] = 1.0f;         vx = dz * m[2][0] + (dx * m[0][0] + dy * m[1][0]);         vy = dz * m[2][1] + (dx * m[0][1] + dy * m[1][1]);         w = dz * m[2][3] + (dx * m[0][3] + dy * m[1][3]);         (cam)->rot.x = -(180.0 * (atan2(vy, vx) / 3.141592653589793));         (cam)->rot.z = 0.0f;     } while (0)

void fn_1_110(s16 mode, s16 frame)
{
    s32 i;
    s16 lightId;

    for (i = 0; i < 5; i++) {
        lbl_1_bss_7C[i] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_rodata_8C[i], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelLayerSet(lbl_1_bss_7C[i], 0);
        Hu3DModelPosSetV(lbl_1_bss_7C[i], &lbl_1_data_D8[i]);
        Hu3DModelRotSet(lbl_1_bss_7C[i], 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrSet(lbl_1_bss_7C[i], 0x40000001);
    }
    Hu3DCameraCreate(lbl_1_data_6C.camNo);
    Hu3DCameraViewportSet(lbl_1_data_6C.camNo, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DModelCameraInfoSet(lbl_1_bss_7C[0], (u16)lbl_1_data_6C.camNo);
    Hu3DCameraPerspectiveGet(lbl_1_data_6C.camNo, &lbl_1_data_6C.fov, &lbl_1_data_6C.nearZ, &lbl_1_data_6C.farZ);
    lbl_1_bss_34 = lbl_1_data_6C;
    Hu3DCameraPosGet(lbl_1_bss_34.camNo, &lbl_1_bss_34.pos, &lbl_1_bss_34.up, &lbl_1_bss_34.target);
    M569_CAM_ROT_CALC(&lbl_1_bss_34);
    fn_1_1288(&lbl_1_data_6C);
    Hu3DModelLightInfoSet(lbl_1_bss_7C[0], 1);
    lightId = Hu3DModelLightIdGet(lbl_1_bss_7C[0], lbl_1_data_13C);
    Hu3DGLightParamGet(lightId, &lbl_1_bss_18.pos, &lbl_1_bss_18.dir, &lbl_1_bss_18.color);
    Hu3DShadowCreate(lbl_1_data_6C.fov, lbl_1_data_6C.nearZ, lbl_1_data_6C.farZ);
    Hu3DShadowTPLvlSet(0.35f);
    Hu3DShadowPosSet(&lbl_1_data_B4, &lbl_1_data_C0, &lbl_1_data_CC);
    Hu3DModelShadowMapSet(lbl_1_bss_7C[0]);
    fn_1_3A48();
    lbl_1_bss_C = fn_1_582C();
    fn_1_6DB4();
    fn_1_80AC();
    lbl_1_bss_14 = 0;
    lbl_1_bss_8 = -1;
    fn_1_9B2C();
    MgSeqModeNext();
}

/* fn_1_269C is inlined into fn_1_820 by the original compiler. */
static inline void M569ObjDel(OMOBJ *obj)
{
    omDelObjEx(lbl_1_bss_D4, obj);
}

void fn_1_820(s16 mode, s16 frame)
{
    s32 i;

    if (frame == 0) {
        fn_1_74B0();
        lbl_1_bss_8 = HuAudJinglePlay(0x88);
    }
    switch (lbl_1_bss_14) {
    case 1:
        if (lbl_1_bss_0 == NULL) {
            lbl_1_bss_0 = MgCameraMoveEntry(&lbl_1_data_6C, &lbl_1_bss_34, 90.0f);
        }
        break;
    case 2:
        if (lbl_1_bss_0 != NULL) {
            M569ObjDel(lbl_1_bss_0);
            for (i = 0; i < 4; i++) {
                fn_1_4EB4(i, 1);
            }
            lbl_1_bss_0 = NULL;
        }
        break;
    }
}

void fn_1_94C(s16 mode, s16 frame)
{
    fn_1_87D4();
    lbl_1_bss_10 = MgTimerCreate(0);
    MgTimerParamSet(lbl_1_bss_10, 0x258, 0, 0);
    MgTimerModeOnSet(lbl_1_bss_10, 1);
    fn_1_80FC();
    lbl_1_bss_14 = 3;
}

void fn_1_9C4(s16 mode, s16 frame)
{
    s32 i;
    s32 team;
    s32 ok;

    if (lbl_1_bss_14 >= 3 && lbl_1_bss_14 <= 6) {
        team = 0;
    } else {
        team = 1;
    }
    switch (lbl_1_bss_14) {
    case 3:
    case 8:
        if (fn_1_8544() != 0) {
            for (i = 0; i < 3; i++) {
                if (lbl_1_bss_D8[team].member[i] != -1) {
                    fn_1_4EB4(lbl_1_bss_D8[team].member[i], 2);
                }
            }
            lbl_1_bss_14++;
        }
        break;
    case 4:
    case 9:
        if (MgTimerDoneCheck(lbl_1_bss_10) != 0) {
            fn_1_8614();
            lbl_1_bss_14++;
            return;
        }
        if (fn_1_85C0() != 0) {
            if (lbl_1_bss_10->stopF == 0) {
                lbl_1_bss_10->stopF = 1;
                lbl_1_bss_10->mode = 1;
            }
            lbl_1_bss_14++;
            return;
        }
        break;
    case 5:
    case 10:
        lbl_1_bss_4++;
        if (lbl_1_bss_4 == 0x5A) {
            lbl_1_bss_D8[team].score = fn_1_86C4();
            fn_1_8A74(team, fn_1_86C4());
            return;
        }
        if (lbl_1_bss_4 > 0x5A && fn_1_8AAC(team) != 0) {
            fn_1_833C();
            lbl_1_bss_4 = 0;
            lbl_1_bss_14++;
            fn_1_54A8(team);
            if (lbl_1_bss_D8[team].score > 0) {
                fn_1_6D9C(lbl_1_bss_C);
                fn_1_74B0();
                fn_1_735C();
                return;
            }
        }
        break;
    case 6:
    case 11:
        lbl_1_bss_4++;
        if (lbl_1_bss_4 == 0x78) {
            lbl_1_bss_4 = 0;
            lbl_1_bss_14++;
            return;
        }
        break;
    case 7:
        if (lbl_1_bss_4++ == 0) {
            fn_1_4F14(lbl_1_bss_D8[0].member[0]);
            for (i = 0; i < 3; i++) {
                fn_1_5138(lbl_1_bss_D8[1].member[i]);
            }
            return;
        }
        ok = 1;
        for (i = 0; i < 4; i++) {
            if (fn_1_4EE4(i) != 0) {
                ok = 0;
            }
        }
        if (ok != 0) {
            MgTimerParamSet(lbl_1_bss_10, 0x258, 0, 0);
            MgTimerModeOnSet(lbl_1_bss_10, 1);
            fn_1_80FC();
            lbl_1_bss_4 = 0;
            lbl_1_bss_14 = 8;
            return;
        }
        break;
    case 12:
        MgSeqModeNext();
        break;
    }
}

void fn_1_DD0(s16 mode, s16 frame)
{
    s32 i;

    if (frame == 0x1E) {
        for (i = 0; i < 4; i++) {
            fn_1_52F8(i);
        }
    }
}

void fn_1_E20(s16 mode, s16 frame)
{
    s32 i;
    s32 flag;
    M569PLAYER *player;

    if (frame == 0) {
        fn_1_8ADC();
    }
    if (frame == 0x46) {
        if (lbl_1_bss_D8[0].score != 0 || lbl_1_bss_D8[1].score != 0) {
            flag = 0;
            HuAudJinglePlay(0x79);
        } else {
            flag = 1;
            HuAudJinglePlay(0x7B);
        }
        for (i = 0; i < 4; i++) {
            player = (M569PLAYER *)lbl_1_bss_F8[i]->data;
            if (player->coin != 0) {
                fn_1_4B8C(lbl_1_bss_F8[i], 4);
            } else {
                if (flag != 0) {
                    player->voiceF = 1;
                } else {
                    player->voiceF = 0;
                }
                fn_1_4B8C(lbl_1_bss_F8[i], 5);
            }
        }
    }
    if (frame >= 0x3C) {
        if (frame == 0x3C) {
            fn_1_74B0();
            fn_1_741C();
        }
        if (lbl_1_bss_D8[0].score != 0 || lbl_1_bss_D8[1].score != 0) {
            fn_1_6D9C(lbl_1_bss_C);
        }
    }
}

void fn_1_FA8(s16 mode, s16 frame)
{
    if (lbl_1_bss_D8[0].score != 0 || lbl_1_bss_D8[1].score != 0) {
        fn_1_6D9C(lbl_1_bss_C);
    }
    if (frame == 0x28) {
        MgSeqModeNext();
    }
}

void fn_1_1010(s16 mode, s16 frame)
{
}

s32 fn_1_1014(HuVecF *pos)
{
    HuVecF screen;
    s32 pan;

    Hu3D3Dto2D(pos, (s16)lbl_1_bss_34.camNo, &screen);
    pan = (s32)(32.0f + 64.0f * (screen.x / 576.0f));
    return pan;
}

void fn_1_109C(HuVecF *pos, s16 seId)
{
    HuAudFXPlayPan(seId, fn_1_1014(pos));
}

void fn_1_1148(void)
{
    lbl_1_bss_D4 = omInitObjMan(200, 0x1000);
    lbl_1_bss_D0 = -1;
    omGameSysInit(lbl_1_bss_D4);
}

M569CAM *fn_1_119C(M569CAM *src)
{
    M569CAM *cam;

    cam = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M569CAM), HU_MEMNUM_OVL);
    *cam = *src;
    Hu3DCameraCreate(src->camNo);
    Hu3DCameraViewportSet(src->camNo, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(src->camNo, src->fov, src->nearZ, src->farZ, 1.2f);
    fn_1_1288(src);
    return cam;
}

void fn_1_1288(M569CAM *cam)
{
    if (cam->state == 1) {
        fn_1_1888(cam);
    }
    Hu3DCameraPosSetV(cam->camNo, &cam->pos, &cam->up, &cam->target);
}

s16 fn_1_12D8(M569LIGHT *light)
{
    HU3DLIGHTID lightId;

    lightId = Hu3DGLightCreateV(&light->pos, &light->dir, &light->color);
    Hu3DGLightStaticSet(lightId, TRUE);
    Hu3DGLightInfinitytSet(lightId);
    return lightId;
}

static void fn_1_1334(OMOBJ *obj)
{
    M569CAM *cam = (M569CAM *)obj->work[0];

    if (HuPadBtn[0] & 0x800) {
        if (HuPadBtnDown[0] & 0x1000) {
            *cam = lbl_1_bss_88;
        }
        if (HuPadBtnDown[0] & 0x200) {
            obj->work[1]++;
            if (obj->work[1] == 2) {
                obj->work[1] = 0;
            }
        }
        switch (obj->work[1]) {
        case 0:
            cam->pos.z = cam->pos.z - 0.1f * (f32)HuPadTrigL[0];
            cam->pos.z = cam->pos.z + 0.1f * (f32)HuPadTrigR[0];
            cam->pos.x = cam->pos.x + 0.1f * (f32)HuPadStkX[0];
            cam->pos.y = cam->pos.y + 0.1f * (f32)HuPadStkY[0];
            fn_1_1F4C(cam);
            break;
        case 1:
            cam->rot.x = cam->rot.x - 0.1f * (f32)HuPadStkY[0];
            cam->rot.y = cam->rot.y + 0.1f * (f32)HuPadStkX[0];
            cam->dist = cam->dist - 0.1f * (f32)HuPadTrigL[0];
            cam->dist = cam->dist + 0.1f * (f32)HuPadTrigR[0];
            fn_1_1888(cam);
            break;
        }
        if (HuPadBtn[0] & 0x400) {
            cam->target.z = cam->target.z - 0.1f * (f32)HuPadSubStkY[0];
        } else {
            cam->target.x = cam->target.x + 0.1f * (f32)HuPadSubStkX[0];
            cam->target.y = cam->target.y + 0.1f * (f32)HuPadSubStkY[0];
        }
        Hu3DCameraPosSetV(cam->camNo, &cam->pos, &cam->up, &cam->target);
    }
}

void fn_1_17AC(M569CAM *cam)
{
    OMOBJ *obj;

    obj = omAddObjEx(lbl_1_bss_D4, 0x4000, 0, 0, OM_OBJ_NONE, fn_1_1334);
    obj->work[0] = (uintptr_t)cam;
    if (cam->state == 0) {
        obj->work[1] = 0;
    } else {
        obj->work[1] = 1;
    }
    lbl_1_bss_88 = *cam;
}

void fn_1_1858(M569CAM *src)
{
    lbl_1_bss_88 = *src;
}

void fn_1_1888(M569CAM *cam)
{
    HuVecF axis;
    f32 rx;
    f32 ry;
    f32 rz;
    f32 ux;
    f32 uy;
    f32 uz;

    rx = cam->rot.x;
    ry = cam->rot.y;
    rz = cam->rot.z;
    cam->pos.x = cam->target.x + cam->dist * (sin(M569_RAD(ry)) * cos(M569_RAD(rx)));
    cam->pos.y = cam->target.y + cam->dist * -sin(M569_RAD(rx));
    cam->pos.z = cam->target.z + cam->dist * (cos(M569_RAD(ry)) * cos(M569_RAD(rx)));
    PSVECSubtract(&cam->pos, &cam->target, &axis);
    PSVECNormalize(&axis, &axis);
    ux = sin(M569_RAD(ry)) * sin(M569_RAD(rx));
    uy = cos(M569_RAD(rx));
    uz = cos(M569_RAD(ry)) * sin(M569_RAD(rx));
    cam->up.x = ux * ((axis.x * axis.x) + (1.0f - axis.x * axis.x) * cos(M569_RAD(rz)))
                + uy * ((axis.x * axis.y) * (1.0 - cos(M569_RAD(rz))) - axis.z * sin(M569_RAD(rz)))
                + uz * ((axis.x * axis.z) * (1.0 - cos(M569_RAD(rz))) + axis.y * sin(M569_RAD(rz)));
    cam->up.y = uy * ((axis.y * axis.y) + (1.0f - axis.y * axis.y) * cos(M569_RAD(rz)))
                + ux * ((axis.x * axis.y) * (1.0 - cos(M569_RAD(rz))) + axis.z * sin(M569_RAD(rz)))
                + uz * ((axis.y * axis.z) * (1.0 - cos(M569_RAD(rz))) - axis.x * sin(M569_RAD(rz)));
    cam->up.z = uz * ((axis.z * axis.z) + (1.0f - axis.z * axis.z) * cos(M569_RAD(rz)))
                + (ux * ((axis.x * axis.z) * (1.0 - cos(M569_RAD(rz))) - axis.y * sin(M569_RAD(rz)))
                   + uy * ((axis.y * axis.z) * (1.0 - cos(M569_RAD(rz))) + axis.x * sin(M569_RAD(rz))));
    PSVECNormalize(&cam->up, &cam->up);
}

void fn_1_1F4C(M569CAM *cam)
{
    M569_CAM_ROT_CALC(cam);
}

static void fn_1_2350(OMOBJ *obj)
{
    M569CAM *from;
    M569CAM *cam;
    M569CAM *to;
    M569MOVE *mv;
    f32 t;

    mv = (M569MOVE *)obj->data;
    if ((mv->flags & 2) != 0) {
        cam = mv->src;
        from = &mv->from_cam;
        to = &mv->to;
        mv->accum += 1.0f;
        t = mv->accum / mv->progress;
        if (cam->state == 0) {
            cam->pos.x = from->pos.x + t * (to->pos.x - from->pos.x);
            cam->pos.y = from->pos.y + t * (to->pos.y - from->pos.y);
            cam->pos.z = from->pos.z + t * (to->pos.z - from->pos.z);
        } else {
            cam->dist = from->dist + t * (to->dist - from->dist);
            cam->rot.x = from->rot.x + t * (to->rot.x - from->rot.x);
            cam->rot.y = from->rot.y + t * (to->rot.y - from->rot.y);
        }
        cam->target.x = from->target.x + t * (to->target.x - from->target.x);
        cam->target.y = from->target.y + t * (to->target.y - from->target.y);
        cam->target.z = from->target.z + t * (to->target.z - from->target.z);
        if (mv->accum >= mv->progress) {
            *cam = *to;
            mv->flags &= ~2;
        }
        fn_1_1288(cam);
    }
}

OMOBJ *MgCameraMoveEntry(M569CAM *from, M569CAM *to, f32 frames)
{
    OMOBJ *obj;
    M569MOVE *mv;

    if (from->camNo != to->camNo) {
        OSReport("MgCameraMoveEntry : Camera Number Different\n");
#ifdef TARGET_PC
        return NULL;
#else
        return;
#endif
    }
    obj = omAddObjEx(lbl_1_bss_D4, 0x2000, 0, 0, -1, fn_1_2350);
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M569MOVE), HU_MEMNUM_OVL);
    mv = (M569MOVE *)obj->data;
    mv->progress = frames;
    mv->accum = 0.0f;
    mv->src = from;
    mv->from_cam = *from;
    mv->to = *to;
    mv->flags = 3;
    if (from->state == 1) {
        fn_1_1888(&mv->from_cam);
        fn_1_1888(&mv->to);
    }
    return obj;
}

s32 fn_1_2668(OMOBJ *obj)
{
    M569MOVE *mv = (M569MOVE *)obj->data;

    if ((mv->flags & 2) != 0) {
        return 1;
    }
    return 0;
}

void fn_1_269C(OMOBJ *obj)
{
    M569ObjDel(obj);
}

static void fn_1_26D0(OMOBJ *obj)
{
    M569CAM *cam;
    s32 range;

    cam = (M569CAM *)obj->data;
    range = obj->work[1];
    if (obj->work[0] == 0) {
        fn_1_1288((M569CAM *)obj->work[2]);
        omDelObjEx(lbl_1_bss_D4, obj);
        return;
    }
    cam->target.x = ((M569CAM *)obj->work[2])->target.x + (f32)(s32)frandmod(range * 2) - (f32)range;
    cam->target.y = ((M569CAM *)obj->work[2])->target.y + (f32)(s32)frandmod(range * 2) - (f32)range;
    fn_1_1288(cam);
    obj->work[0]--;
}

void fn_1_2818(M569CAM *cam, s32 frames, s32 range)
{
    OMOBJ *obj;
    M569CAM *dst;

    obj = omAddObjEx(lbl_1_bss_D4, 0x2000, 0, 0, OM_OBJ_NONE, fn_1_26D0);
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M569CAM), HU_MEMNUM_OVL);
    dst = (M569CAM *)obj->data;
    obj->work[0] = frames;
    obj->work[1] = range;
    *dst = *cam;
    obj->work[2] = (uintptr_t)cam;
}

f32 fn_1_28D4(s16 pad)
{
    f32 mag;

    mag = (f32)(HuPadStkX[pad] * HuPadStkX[pad] + HuPadStkY[pad] * HuPadStkY[pad]);
    return sqrtf(mag);
}

f32 fn_1_2A58(s16 pad)
{
    f32 ang;

    ang = (f32)(180.0 * (atan2((f64)(f32)HuPadStkY[pad], (f64)(f32)HuPadStkX[pad]) / 3.141592653589793) - 90.0);
    if (ang < 0.0f) {
        ang += 360.0f;
    }
    return ang;
}

static void fn_1_2C18(OMOBJ *obj);

static void fn_1_2B68(OMOBJ *obj)
{
    M569FLOW *flow;

    flow = (M569FLOW *)obj->data;
    if (omSysExitReq != 0) {
        if (lbl_1_bss_D0 != -1) {
            HuAudFadeOut(1000);
            lbl_1_bss_D0 = -1;
        }
        obj->objFunc = fn_1_2C18;
        WipeCreate(2, 6, 60);
        return;
    }
    flow->tbl[flow->idx](obj);
}

static void fn_1_2C18(OMOBJ *obj)
{
    if (WipeCheckEnd() == 0) {
        omOvlReturnEx(1, 1);
    }
}

OMOBJ *fn_1_2C50(M569FLOWFUNC *tbl)
{
    M569FLOW *flow;
    OMOBJ *obj;
    s32 i;
    s32 count;

    count = 0;
    while (tbl[count] != NULL) {
        count++;
    }
    obj = omAddObjEx(lbl_1_bss_D4, 0x2000, 0, 0, OM_OBJ_NONE, fn_1_2B68);
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M569FLOW), HU_MEMNUM_OVL);
    flow = (M569FLOW *)obj->data;
    flow->count = count;
    flow->tbl = tbl;
    flow->idx = 0;
    flow->mesId = -1;
    for (i = 0; i < 16; i++) {
        flow->work[i] = 0;
    }
    return obj;
}

void MgFlowSet(OMOBJ *obj, s32 idx)
{
    M569FLOW *flow;

    flow = (M569FLOW *)obj->data;
    if (idx >= flow->count) {
        OSReport("MgFlowSet : Value is Max Over\n");
        return;
    }
    flow->idx = idx;
}

void MgFlowNext(OMOBJ *obj)
{
    M569FLOW *flow;

    flow = (M569FLOW *)obj->data;
    if (flow->idx == flow->count - 1) {
        OSReport("MgFlowNext : Value is Max Over\n");
        return;
    }
    flow->idx++;
}

s32 fn_1_2DF0(OMOBJ *obj)
{
    M569FLOW *flow;

    flow = (M569FLOW *)obj->data;
    return flow->idx;
}

s32 fn_1_2E0C(OMOBJ *obj, s32 idx)
{
    M569FLOW *flow;

    flow = (M569FLOW *)obj->data;
    if (idx == fn_1_2DF0(obj)) {
        return 1;
    }
    return 0;
}

void fn_1_2E58(OMOBJ *obj, s32 type, s32 maxTime, s32 endTime, s32 speed)
{
    M569FLOW *flow;

    flow = (M569FLOW *)obj->data;
    flow->timerType = type;
    flow->timerMax = maxTime * 60;
    flow->timerEnd = endTime * 60;
    flow->timerSpeed = speed * 60;
}

BOOL fn_1_2E8C(OMOBJ *obj)
{
    M569FLOW *flow;

    flow = (M569FLOW *)obj->data;
    return MgTimerDoneCheck(flow->timer);
}

void fn_1_2EC8(OMOBJ *obj)
{
    M569FLOW *flow;

    flow = (M569FLOW *)obj->data;
    if (flow->timer->stopF == 0) {
        flow->timer->stopF = 1;
        flow->timer->mode = 1;
    }
}

void fn_1_2F08(void)
{
}

void fn_1_2F0C(OMOBJ *obj)
{
    if (WipeCheckEnd() == 0) {
        MgFlowNext(obj);
    }
}

void fn_1_2F84(void)
{
    if (lbl_1_bss_D0 != -1) {
        HuAudFadeOut(1000);
        lbl_1_bss_D0 = -1;
    }
    if (WipeCheckEnd() == 0) {
        omOvlReturnEx(1, 1);
    }
}

void fn_1_2FE8(OMOBJ *obj, s16 mesNo, s16 type)
{
    M569FLOW *flow;

    flow = (M569FLOW *)obj->data;
    if (flow->mesId == -1) {
        flow->mesId = GameMesCreate(mesNo, type);
        return;
    }
    if (GameMesStatGet(flow->mesId) == 0) {
        flow->mesId = -1;
        MgFlowNext(obj);
    }
}

void fn_1_30A4(OMOBJ *obj)
{
    M569FLOW *flow;

    flow = (M569FLOW *)obj->data;
    if (flow->mesId == -1) {
        flow->mesId = GameMesCreate(2, 0);
        return;
    }
    if (GameMesStatGet(flow->mesId) == 0) {
        flow->mesId = -1;
        MgFlowNext(obj);
    }
}

void fn_1_3158(OMOBJ *obj)
{
    M569FLOW *flow;

    flow = (M569FLOW *)obj->data;
    if (flow->mesId == -1) {
        flow->mesId = GameMesCreate(2, 1);
        return;
    }
    if (GameMesStatGet(flow->mesId) == 0) {
        flow->mesId = -1;
        MgFlowNext(obj);
    }
}

void fn_1_320C(OMOBJ *obj)
{
    M569FLOW *flow;

    flow = (M569FLOW *)obj->data;
    flow->timer = MgTimerCreate(flow->timerType);
    MgTimerParamSet(flow->timer, flow->timerMax, flow->timerEnd, flow->timerSpeed);
    MgTimerModeOnSet(flow->timer, 1);
    MgFlowNext(obj);
}

void fn_1_32AC(M569PLAYER *player, s16 idx)
{
    player->objId = idx;
    player->charNo = GwPlayerConf[idx].charNo;
    player->padNo = GwPlayerConf[idx].padNo;
    player->grp = GwPlayerConf[idx].grp;
    player->type = GwPlayerConf[idx].type;
    player->dif = GwPlayerConf[idx].dif;
    player->state = 0;
    player->pos.x = player->pos.y = player->pos.z = 0.0f;
    player->rot.x = player->rot.y = player->rot.z = 0.0f;
}

s32 fn_1_3378(HuVecF *pos, HuVecF *goal, HuVecF *angle, f32 step)
{
    HuVecF dir;
    f32 mag;

    PSVECSubtract(goal, pos, &dir);
    mag = PSVECMag(&dir);
    if (mag == 0.0f) {
        return 1;
    }
    PSVECNormalize(&dir, &dir);
    PSVECScale(&dir, &dir, step);
    if (PSVECMag(&dir) >= mag) {
        *pos = *goal;
        return 1;
    }
    PSVECAdd(pos, &dir, pos);
    angle->y = (f32)(180.0 * (atan2(dir.x, dir.z) / 3.141592653589793));
    return 0;
}

f32 fn_1_349C(f32 val, f32 target, f32 max)
{
    f32 diff;
    f32 ret;

    diff = (f32)fmod(target - val, 360.0);
    if (diff < 0.0f) {
        diff += 360.0f;
    }
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    if (fabs(diff) > max) {
        if (diff < 0.0f) {
            diff = -max;
        } else {
            diff = max;
        }
    }
    ret = (f32)fmod(val + diff, 360.0);
    if (ret < 0.0f) {
        ret += 360.0f;
    }
    return ret;
}

s32 fn_1_360C(HuVecF *pos, HuVecF *goal, HuVecF *from, f32 dist)
{
    HuVecF dir;
    HuVecF step;
    HuVecF saved;
    f32 len;
    f32 ang;

    saved = *goal;
    for (;;) {
        PSVECSubtract(goal, pos, &dir);
        if (PSVECMag(&dir) == 0.0f) {
            return 0;
        }
        PSVECSubtract(goal, from, &step);
        len = PSVECMag(&step);
        if (len < dist) {
            if (len != 0.0f) {
                PSVECNormalize(&step, &step);
                PSVECScale(&step, &step, dist);
                PSVECAdd(from, &step, goal);
            } else {
                *goal = *pos;
            }
            PSVECSubtract(goal, pos, &step);
            if (PSVECMag(&step) < 0.001f) {
                len = PSVECMag(&dir);
                ang = (f32)(180.0 * (atan2(step.z, step.x) / 3.141592653589793));
                ang += 30.0f;
                dir.x = len * sin(M569_RAD(ang));
                dir.z = len * cos(M569_RAD(ang));
                PSVECAdd(pos, &dir, goal);
                continue;
            }
            return 1;
        }
        return 0;
    }
}

void fn_1_3844(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *out)
{
    HuVecF v1;
    HuVecF v2;

    PSVECSubtract(c, b, &v1);
    PSVECSubtract(b, a, &v2);
    PSVECCrossProduct(&v1, &v2, out);
    PSVECNormalize(out, out);
}

f32 fn_1_38BC(HuVecF *n, HuVecF *p0, HuVecF *p)
{
    HuVecF d;

    PSVECSubtract(p, p0, &d);
    return PSVECDotProduct(n, &d);
}

s32 fn_1_3900(HuVecF *n, HuVecF *p0, HuVecF *a, HuVecF *b, HuVecF *out)
{
    HuVecF d;
    f32 num;
    f32 den;

    PSVECSubtract(b, a, &d);
    num = fn_1_38BC(n, p0, a);
    den = fn_1_38BC(n, b, a);
    if (den == 0.0f) {
        return 0;
    }
    PSVECScale(&d, &d, num / den);
    PSVECAdd(a, &d, out);
    return 1;
}
