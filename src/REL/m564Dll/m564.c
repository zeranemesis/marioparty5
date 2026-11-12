#include "REL/m564Dll.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* Declared in game/object.h, but a local redeclaration must keep SHARED_SYM. */
SHARED_SYM extern s16 omSysExitReq;

char lbl_1_data_0[45] = "MgCameraMoveEntry : Camera Number Different\n";
char lbl_1_data_2D[31] = "MgFlowSet : Value is Max Over\n";
char lbl_1_data_4C[32] = "MgFlowNext : Value is Max Over\n";

M564_CAM lbl_1_data_6C = {
    1, 1, 20.0f, 20.0f, 5000.0f, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { -170.0f, 165.0f, 70.0f }, { -15.0f, 0.0f, 0.0f }, 311.0f
};

Vec lbl_1_data_B4 = { 0.0f, 4000.0f, 1.0f };
Vec lbl_1_data_C0 = { 0.0f, 1.0f, 0.0f };
Vec lbl_1_data_CC = { 0.0f, 0.0f, 0.0f };
Vec lbl_1_data_D8 = { 0.0f, 0.0f, 0.0f };

void fn_1_EC(s16 mode, s16 frameNo);
void fn_1_9FC(s16 mode, s16 frameNo);
void fn_1_C50(s16 mode, s16 frameNo);
void fn_1_C98(s16 mode, s16 frameNo);
void fn_1_D00(s16 mode, s16 frameNo);
void fn_1_F24(s16 mode, s16 frameNo);
void fn_1_FEC(s16 mode, s16 frameNo);
void fn_1_10F0(s16 mode, s16 frameNo);

MGSEQ_PARAM lbl_1_data_E4 = {
    60, 0, fn_1_EC, fn_1_9FC, fn_1_C50, fn_1_C98, fn_1_D00, fn_1_F24, fn_1_FEC, NULL, fn_1_10F0,
};

char lbl_1_data_10C[12] = "inf_light1";

M564_PLAYER *lbl_1_bss_100[4];
OMOBJMAN *lbl_1_bss_F8;
s16 lbl_1_bss_F4;
M564_CAM lbl_1_bss_AC;
s16 lbl_1_bss_8C[16];
M564_CAM lbl_1_bss_44;
M564_LIGHT lbl_1_bss_28;
s32 lbl_1_bss_24;
s32 lbl_1_bss_20;
s16 lbl_1_bss_C[10];
s32 lbl_1_bss_8;
OMOBJ *lbl_1_bss_4;
s32 lbl_1_bss_0;

static const s32 lbl_1_rodata_8C[16] = {
    0x770002, 0x770003, 0x770004, 0x770000, 0x770001, 0x770005, 0x770006, 0x770007,
    0x770008, 0x770009, 0x77000A, 0x77000B, 0x77000C, 0x77000D, 0x77000E, 0x77000F,
};

void ObjectSetup(void);
void fn_1_10F4(OMOBJ *obj);
void fn_1_16BC(M564_CAM *cam);
void fn_1_1CBC(M564_CAM *cam);
void fn_1_2380(M564_CAM *cam);
void fn_1_304C(void);
void fn_1_C3C(s16 mode, s16 frameNo);

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
    lbl_1_bss_F8 = MgActorObjectSetup();
    MgSeqCreatePrio(&lbl_1_data_E4, 2000);
    MgSeqModeHookAdd(4, fn_1_C3C);
}

#define M564_CAM_FROM_POS(cam) \
    do {                                                                                                              \
        dx = (cam)->pos.x - (cam)->target.x;                                                                          \
        dy = (cam)->pos.y - (cam)->target.y;                                                                          \
        dz = (cam)->pos.z - (cam)->target.z;                                                                          \
        dist = M564Sqrtf((dz * dz) + ((dx * dx) + (dy * dy)));                                                        \
        (cam)->dist = dist;                                                                                           \
        angY = (f32)(180.0 * (atan2(dz, dx) / M564_PI));                                                              \
        (cam)->rot.y = -(angY - 90.0f);                                                                               \
        for (i = 0; i < 4; i++) {                                                                                     \
            for (j = 0; j < 4; j++) {                                                                                 \
                mtx[i][j] = 0.0f;                                                                                     \
            }                                                                                                         \
        }                                                                                                             \
        mtx[0][0] = mtx[2][2] = (f32)cos((M564_PI * -angY) / 180.0);                                                  \
        mtx[2][0] = (f32)-sin((M564_PI * -angY) / 180.0);                                                             \
        mtx[0][2] = (f32)sin((M564_PI * -angY) / 180.0);                                                              \
        mtx[1][1] = mtx[3][3] = 1.0f;                                                                                 \
        rz = (dz * mtx[2][3]) + ((dx * mtx[0][3]) + (dy * mtx[1][3]));                                                \
        (cam)->rot.x = -(f32)(180.0 * (atan2((dz * mtx[2][1]) + ((dx * mtx[0][1]) + (dy * mtx[1][1])),                \
                                             (dz * mtx[2][0]) + ((dx * mtx[0][0]) + (dy * mtx[1][0]))) / M564_PI));   \
        (cam)->rot.z = 0.0f;                                                                                          \
    } while (0)

static inline f32 M564Fmod(f32 x, f64 y) {
    return fmod(x, y);
}

/* inline wrapper: reproduces the extra FPR copy the original emits around PSVECDotProduct */
static inline f32 M564DotA(Vec *a, Vec *b) {
    f32 r = PSVECDotProduct(a, b);
    return r;
}

static inline f32 M564Sqrtf(f32 x) {
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

void fn_1_EC(s16 mode, s16 frameNo) {
    s32 i;
    s32 j;
    HU3DMODELID mdl[3];
    f32 dz;
    f32 dx;
    f32 dy;
    f32 angY;
    f32 dist;
    f32 mtx[4][4];
    f32 rz;

    for (i = 0; i < 16; i++) {
        lbl_1_bss_8C[i] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_rodata_8C[i], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(lbl_1_bss_8C[i], 0x40000002);
        Hu3DModelAttrSet(lbl_1_bss_8C[i], 0x40000001);
        Hu3DModelLayerSet(lbl_1_bss_8C[i], 0);
        Hu3DModelPosSet(lbl_1_bss_8C[i], 0.0f, 0.0f, 0.0f);
        if (i >= 6 && i <= 15) {
            Hu3DModelRotSet(lbl_1_bss_8C[i], 0.0f, -(18.0f + (36.0f * (f32)(i - 6))), 0.0f);
            Hu3DModelAttrReset(lbl_1_bss_8C[i], 0x40000001);
        } else {
            Hu3DModelRotSet(lbl_1_bss_8C[i], 0.0f, 0.0f, 0.0f);
        }
    }
    Hu3DModelLayerSet(lbl_1_bss_8C[5], 4);
    Hu3DModelAttrReset(lbl_1_bss_8C[5], 0x40000002);
    Hu3DModelPosSet(lbl_1_bss_8C[5], 496.0f, 320.0f, 425.0f);
    for (i = 0; i < 3; i++) {
        mdl[i] = lbl_1_bss_8C[i];
        Hu3DModelAttrSet(lbl_1_bss_8C[i], 1);
    }
    MgActorColMapInit(mdl, 3, 0x13);
    MgActorColMapMaskSet(0, 1);
    MgActorColMapMaskSet(1, 2);
    MgActorColMapMaskSet(2, 4);
    Hu3DCameraCreate(lbl_1_data_6C.camNo);
    Hu3DCameraViewportSet(lbl_1_data_6C.camNo, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DModelCameraInfoSet(lbl_1_bss_8C[3], lbl_1_data_6C.camNo);
    Hu3DCameraPerspectiveGet(lbl_1_data_6C.camNo, &lbl_1_data_6C.fov, &lbl_1_data_6C.nearZ, &lbl_1_data_6C.farZ);
    lbl_1_bss_44 = lbl_1_data_6C;
    Hu3DCameraPosGet(lbl_1_bss_44.camNo, &lbl_1_bss_44.pos, &lbl_1_bss_44.up, &lbl_1_bss_44.target);
    M564_CAM_FROM_POS(&lbl_1_bss_44);
    fn_1_16BC(&lbl_1_bss_44);
    Hu3DModelLightInfoSet(lbl_1_bss_8C[3], 1);
    Hu3DGLightParamGet(Hu3DModelLightIdGet(lbl_1_bss_8C[3], lbl_1_data_10C), &lbl_1_bss_28.pos, &lbl_1_bss_28.dir,
                       &lbl_1_bss_28.color);
    Hu3DShadowCreate(lbl_1_bss_44.fov, lbl_1_bss_44.nearZ, lbl_1_bss_44.farZ);
    Hu3DShadowTPLvlSet(0.35f);
    Hu3DShadowPosSet(&lbl_1_data_B4, &lbl_1_data_C0, &lbl_1_data_CC);
    for (i = 3; i <= 15; i++) {
        Hu3DModelShadowMapSet(lbl_1_bss_8C[i]);
    }
    fn_1_3E7C();
    fn_1_60E8();
    fn_1_6858();
    omAddObjEx(lbl_1_bss_F8, 0x1300, 0, 0, -1, fn_1_10F4);
    lbl_1_bss_24 = 0;
    MgSeqModeNext();
}

void fn_1_9FC(s16 mode, s16 frameNo) {
    s32 i;

    lbl_1_bss_24 = 1;
    switch (lbl_1_bss_0) {
        case 0:
            if (frameNo == 10) {
                HuAudFXPlay(0x2D7);
            }
            if (frameNo == 60) {
                lbl_1_bss_4 = MgCameraMoveEntry(&lbl_1_data_6C, &lbl_1_bss_44, 60.0f);
                lbl_1_bss_100[0]->state = 1;
                lbl_1_bss_0++;
            }
            break;
        case 1:
            lbl_1_bss_100[0]->state = 1;
            if (((M564_CAMMOVE *)lbl_1_bss_4->data)->moveF & 2) {
                i = 1;
            } else {
                i = 0;
            }
            if (i == 0) {
                omDelObjEx(lbl_1_bss_F8, lbl_1_bss_4);
                lbl_1_bss_0++;
            }
            break;
        case 2:
            if (lbl_1_bss_1E8[0]->state == 7) {
                fn_1_43E0(lbl_1_bss_100[1]->mgPlayer, 4);
                fn_1_43E0(lbl_1_bss_100[2]->mgPlayer, 4);
                fn_1_43E0(lbl_1_bss_100[3]->mgPlayer, 4);
                for (i = 0; i < 4; i++) {
                    omVibrate(lbl_1_bss_100[i]->playerNo, 12, 12, 0);
                }
                lbl_1_bss_0++;
            }
            break;
    }
    if (frameNo == 0x96) {
        MgSeqModeNext();
    }
}

void fn_1_C3C(s16 mode, s16 frameNo) {
    lbl_1_bss_24 = 2;
}

void fn_1_C50(s16 mode, volatile s16 frameNo) {
    if (frameNo == 0) {
        lbl_1_bss_C[0] = HuAudBGMPlay(0x36);
    }
    fn_1_653C();
}

void fn_1_C98(s16 mode, s16 frameNo) {
    s32 i;
    s32 cnt;

    cnt = 0;
    for (i = 0; i < 4; i++) {
        if (fn_1_5E5C(i) != 0) {
            cnt++;
        }
    }
    if (cnt >= 3) {
        MgSeqModeNext();
    }
    fn_1_653C();
}

void fn_1_D00(s16 mode, volatile s16 frameNo) {
    s16 i;
    s32 cnt;
    s32 j;
    s16 winner[4];

    if (frameNo == 0) {
        HuAudSStreamFadeOut(lbl_1_bss_C[0], 100);
    }
    switch (lbl_1_bss_8) {
        case 0:
            lbl_1_bss_24 = 3;
            fn_1_8DEC();
            lbl_1_bss_8++;
            return;
        case 1:
            if (fn_1_8EC8() != 0) {
                lbl_1_bss_8++;
                return;
            }
            return;
        case 2:
            cnt = 0;
            for (j = 0; j < 4; j++) {
                if (lbl_1_bss_100[j]->carryF != 0) {
                    cnt++;
                    lbl_1_bss_100[j]->state = 1;
                    lbl_1_bss_1E8[lbl_1_bss_100[j]->bombNo]->comF = 1;
                }
            }
            if (cnt == 0) {
                lbl_1_bss_8++;
                return;
            }
            break;
        case 3:
            for (i = 0; i < 4; i++) {
                winner[i] = -1;
                if (fn_1_5E5C(i) == 0) {
                    fn_1_43E0(lbl_1_bss_100[i]->mgPlayer, 5);
                    winner[i] = lbl_1_bss_100[i]->charNo;
                }
            }
            MgSeqWinnerSet(winner[0], winner[1], winner[2], winner[3]);
            break;
    }
}

void fn_1_F24(s16 mode, volatile s16 frameNo) {
    s32 i;

    if (frameNo == 30) {
        lbl_1_bss_20 = 1;
        for (i = 0; i < 4; i++) {
            if (fn_1_5E5C(i) == 0) {
                fn_1_4204(lbl_1_bss_100[i]->mgPlayer, 5);
                GWMgCoinBonusSet(i, 10);
                lbl_1_bss_20 = 0;
            }
        }
        MgSeqModeNext();
    }
}

void fn_1_FEC(s16 mode, s16 frameNo) {
    s32 i;

    if (lbl_1_bss_20 == 0) {
        if (frameNo == 0) {
            fn_1_8F6C();
        }
        if (frameNo == 100) {
            HuAudFXPlay(0x8D8);
            for (i = 0; i < 15; i++) {
                if (lbl_1_bss_1E8[i]->state == 8) {
                    fn_1_7210(lbl_1_bss_1E8[i], 0);
                }
            }
            for (i = 0; i < 4; i++) {
                if (fn_1_5E5C(i) == 0) {
                    omVibrate(lbl_1_bss_100[i]->playerNo, 12, 12, 0);
                }
            }
        }
    }
}

void fn_1_10F0(s16 mode, s16 frameNo) {
}

void fn_1_10F4(OMOBJ *obj) {
    s32 i;

    MgActorExec();
    for (i = 0; i < 4; i++) {
        if (fn_1_5E5C(i) != 0) {
            Hu3DModelRotSetV((s16)lbl_1_bss_100[i]->mgPlayer->actor->mdlId, &lbl_1_bss_100[i]->rot);
        }
    }
}

void fn_1_118C(Vec *center, Vec *out, f32 min, f32 max) {
    f32 r;
    f32 a;

    r = min + (f32)frandmod((s32)(max - min));
    a = (f32)frandmod(360);
    out->x = (f32)((f64)center->x + ((f64)r * cos(M564_RAD(a))));
    out->z = (f32)((f64)center->z + ((f64)r * sin(M564_RAD(a))));
}

f32 fn_1_12D4(Vec *from, Vec *to) {
    Vec v;

    PSVECSubtract(to, from, &v);
    return (f32)(180.0 * (atan2(v.x, v.z) / M564_PI));
}

f32 fn_1_1338(f32 a, f32 b) {
    f32 d;

    d = b - a;
    if (d < -180.0f) {
        d += 360.0f;
    }
    if (d > 180.0f) {
        d -= 360.0f;
    }
    return d;
}

f32 fn_1_13A4(Vec *a, Vec *b) {
    Vec v;

    PSVECSubtract(b, a, &v);
    v.y = 0.0f;
    return PSVECMag(&v);
}

f32 fn_1_13F0(Vec *a, Vec *b, Vec *out) {
    PSVECSubtract(b, a, out);
    out->y = 0.0f;
    return PSVECMag(out);
}

s32 fn_1_1448(Vec *pos) {
    Vec screen;

    Hu3D3Dto2D(pos, (s16)lbl_1_bss_44.camNo, &screen);
    return (s32)(32.0f + 64.0f * (screen.x / 576.0f));
}

void fn_1_14D0(Vec *pos, s16 seId) {
    HuAudFXPlayPan(seId, fn_1_1448(pos));
}

void fn_1_157C(void) {
    lbl_1_bss_F8 = omInitObjMan(200, 0x1000);
    lbl_1_bss_F4 = -1;
    omGameSysInit(lbl_1_bss_F8);
}

M564_CAM *fn_1_15D0(M564_CAM *src) {
    M564_CAM *cam;

    cam = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M564_CAM), HU_MEMNUM_OVL);
    *cam = *src;
    Hu3DCameraCreate(src->camNo);
    Hu3DCameraViewportSet(src->camNo, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(src->camNo, src->fov, src->nearZ, src->farZ, 1.2f);
    fn_1_16BC(src);
    return cam;
}

void fn_1_16BC(M564_CAM *cam) {
    if (cam->state == 1) {
        fn_1_1CBC(cam);
    }
    Hu3DCameraPosSetV(cam->camNo, &cam->pos, &cam->up, &cam->target);
}

HU3DLIGHTID fn_1_170C(M564_LIGHT *light) {
    HU3DLIGHTID lightId;

    lightId = Hu3DGLightCreateV(&light->pos, &light->dir, &light->color);
    Hu3DGLightStaticSet(lightId, 1);
    Hu3DGLightInfinitytSet(lightId);
    return lightId;
}

void fn_1_1768(OMOBJ *obj) {
    M564_CAM *cam = (M564_CAM *)obj->work[0];

    if ((HuPadBtn[0] & 0x800) != 0) {
        if ((HuPadBtnDown[0] & 0x1000) != 0) {
            *cam = lbl_1_bss_AC;
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
                fn_1_2380(cam);
                break;
            case 1:
                cam->rot.x = cam->rot.x - (0.1f * (f32)HuPadStkY[0]);
                cam->rot.y = cam->rot.y + (0.1f * (f32)HuPadStkX[0]);
                cam->dist = cam->dist - (0.1f * (f32)HuPadTrigL[0]);
                cam->dist = cam->dist + (0.1f * (f32)HuPadTrigR[0]);
                fn_1_1CBC(cam);
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

void fn_1_1BE0(M564_CAM *cam) {
    OMOBJ *obj;

    obj = omAddObjEx(lbl_1_bss_F8, 0x4000, 0, 0, -1, fn_1_1768);
    obj->work[0] = (uintptr_t)cam;
    if (cam->state == 0) {
        obj->work[1] = 0;
    } else {
        obj->work[1] = 1;
    }
    lbl_1_bss_AC = *cam;
}

void fn_1_1C8C(M564_CAM *src) {
    lbl_1_bss_AC = *src;
}

void fn_1_1CBC(M564_CAM *cam) {
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
    cam->pos.x = (f32)((f64)cam->target.x + ((f64)cam->dist * (sin(M564_RAD(ry)) * cos(M564_RAD(rx)))));
    cam->pos.y = (f32)((f64)cam->target.y + ((f64)cam->dist * -sin(M564_RAD(rx))));
    cam->pos.z = (f32)((f64)cam->target.z + ((f64)cam->dist * (cos(M564_RAD(ry)) * cos(M564_RAD(rx)))));
    PSVECSubtract(&cam->pos, &cam->target, &axis);
    PSVECNormalize(&axis, &axis);
    tA = (f32)(sin(M564_RAD(ry)) * sin(M564_RAD(rx)));
    tB = (f32)cos(M564_RAD(rx));
    tC = (f32)(cos(M564_RAD(ry)) * sin(M564_RAD(rx)));
    cam->up.x = (f32)(((f64)tA * ((f64)(axis.x * axis.x) + ((f64)(1.0f - (axis.x * axis.x)) * cos(M564_RAD(rz)))))
                      + ((f64)tB * (((f64)(axis.x * axis.y) * (1.0 - cos(M564_RAD(rz)))) - ((f64)axis.z * sin(M564_RAD(rz)))))
                      + ((f64)tC * (((f64)(axis.x * axis.z) * (1.0 - cos(M564_RAD(rz)))) + ((f64)axis.y * sin(M564_RAD(rz))))));
    cam->up.y = (f32)(((f64)tB * ((f64)(axis.y * axis.y) + ((f64)(1.0f - (axis.y * axis.y)) * cos(M564_RAD(rz)))))
                      + ((f64)tA * (((f64)(axis.x * axis.y) * (1.0 - cos(M564_RAD(rz)))) + ((f64)axis.z * sin(M564_RAD(rz)))))
                      + ((f64)tC * (((f64)(axis.y * axis.z) * (1.0 - cos(M564_RAD(rz)))) - ((f64)axis.x * sin(M564_RAD(rz))))));
    cam->up.z = (f32)(((f64)tC * ((f64)(axis.z * axis.z) + ((f64)(1.0f - (axis.z * axis.z)) * cos(M564_RAD(rz)))))
                      + (((f64)tA * (((f64)(axis.x * axis.z) * (1.0 - cos(M564_RAD(rz)))) - ((f64)axis.y * sin(M564_RAD(rz)))))
                         + ((f64)tB * (((f64)(axis.y * axis.z) * (1.0 - cos(M564_RAD(rz)))) + ((f64)axis.x * sin(M564_RAD(rz)))))));
    PSVECNormalize(&cam->up, &cam->up);
}

void fn_1_2380(M564_CAM *cam) {
    s32 i;
    s32 j;
    f32 dz;
    f32 dx;
    f32 dy;
    f32 angY;
    f32 dist;
    f32 mtx[4][4];
    f32 rz;

    M564_CAM_FROM_POS(cam);
}

void fn_1_2784(OMOBJ *obj) {
    M564_CAM *start;
    M564_CAM *cam;
    M564_CAM *end;
    M564_CAMMOVE *move = obj->data;
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
        fn_1_16BC(cam);
    }
}

OMOBJ *MgCameraMoveEntry(M564_CAM *cam, M564_CAM *dest, f32 time) {
    OMOBJ *obj;
    M564_CAMMOVE *move;

    if (cam->camNo != dest->camNo) {
        OSReport(lbl_1_data_0);
#ifdef TARGET_PC
        return NULL;
#else
        return;
#endif
    }
    obj = omAddObjEx(lbl_1_bss_F8, 0x2000, 0, 0, -1, fn_1_2784);
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M564_CAMMOVE), HU_MEMNUM_OVL);
    move = obj->data;
    move->totalTime = time;
    move->time = 0.0f;
    move->cam = cam;
    move->start = *cam;
    move->end = *dest;
    move->moveF = 3;
    if (cam->state == 1) {
        fn_1_1CBC(&move->start);
        fn_1_1CBC(&move->end);
    }
    return obj;
}

s32 fn_1_2A9C(OMOBJ *obj) {
    M564_CAMMOVE *move = obj->data;

    if (move->moveF & 2) {
        return 1;
    } else {
        return 0;
    }
}

void fn_1_2AD0(OMOBJ *obj) {
    omDelObjEx(lbl_1_bss_F8, obj);
}

void fn_1_2B04(OMOBJ *obj) {
    M564_CAM *cam = obj->data;
    s32 range = obj->work[1];

    if ((u32)obj->work[0] == 0) {
        fn_1_16BC((M564_CAM *)obj->work[2]);
        omDelObjEx(lbl_1_bss_F8, obj);
        return;
    }
    cam->target.x = ((M564_CAM *)obj->work[2])->target.x + (f32)(s32)frandmod(range * 2) - (f32)range;
    cam->target.y = ((M564_CAM *)obj->work[2])->target.y + (f32)(s32)frandmod(range * 2) - (f32)range;
    fn_1_16BC(cam);
    obj->work[0]--;
}

void fn_1_2C4C(M564_CAM *src, s32 count, s32 range) {
    OMOBJ *obj;
    M564_CAM *dst;

    obj = omAddObjEx(lbl_1_bss_F8, 0x2000, 0, 0, -1, fn_1_2B04);
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M564_CAM), HU_MEMNUM_OVL);
    dst = obj->data;
    obj->work[0] = count;
    obj->work[1] = range;
    *dst = *src;
    obj->work[2] = (uintptr_t)src;
}

f32 fn_1_2D08(s16 padNo) {
    f32 len;

    len = (f32)((HuPadStkX[padNo] * HuPadStkX[padNo]) + (HuPadStkY[padNo] * HuPadStkY[padNo]));
    return M564Sqrtf(len);
}

f32 fn_1_2E8C(s16 padNo) {
    f32 ang;

    ang = (f32)((180.0 * (atan2((f32)HuPadStkY[padNo], (f32)HuPadStkX[padNo]) / M564_PI)) - 90.0);
    if (ang < 0.0f) {
        ang += 360.0f;
    }
    return ang;
}

void fn_1_2F9C(OMOBJ *obj) {
    M564_FLOW *flow = obj->data;

    if (omSysExitReq != 0) {
        if (lbl_1_bss_F4 != -1) {
            HuAudFadeOut(1000);
            lbl_1_bss_F4 = -1;
        }
        obj->objFunc = (OMOBJFUNC)fn_1_304C;
        WipeCreate(2, 6, 60);
        return;
    }
    (*(flow->index + flow->funcTbl))(obj, flow->funcTbl);
}

void fn_1_304C(void) {
    if (WipeCheckEnd() == 0) {
        omOvlReturnEx(1, 1);
    }
}

OMOBJ *fn_1_3084(M564_FLOWFUNC *funcTbl) {
    M564_FLOW *flow;
    OMOBJ *obj;
    s32 i;
    s32 j;

    i = 0;
    while (funcTbl[i] != NULL) {
        i++;
    }
    obj = omAddObjEx(lbl_1_bss_F8, 0x2000, 0, 0, -1, fn_1_2F9C);
    flow = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M564_FLOW), HU_MEMNUM_OVL);
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
    M564_FLOW *flow = obj->data;

    if (index >= flow->funcNum) {
        OSReport(lbl_1_data_2D);
        return;
    }
    flow->index = index;
}

void MgFlowNext(OMOBJ *obj) {
    M564_FLOW *flow = obj->data;

    if (flow->index == flow->funcNum - 1) {
        OSReport(lbl_1_data_4C);
        return;
    }
    flow->index++;
}

s32 fn_1_3224(OMOBJ *obj) {
    M564_FLOW *flow = obj->data;

    return flow->index;
}

s32 fn_1_3240(OMOBJ *obj, s32 index) {
    M564_FLOW *flow = obj->data;

    if (index == fn_1_3224(obj)) {
        return 1;
    }
    return 0;
}

void fn_1_328C(OMOBJ *obj, s32 type, s32 maxTime, s32 endTime, s32 recordTime) {
    M564_FLOW *flow = obj->data;

    flow->timerType = type;
    flow->maxTime = maxTime * 60;
    flow->endTime = endTime * 60;
    flow->recordTime = recordTime * 60;
}

void fn_1_32C0(OMOBJ *obj) {
    M564_FLOW *flow = obj->data;

    MgTimerDoneCheck(flow->timer);
}

void fn_1_32FC(OMOBJ *obj) {
    M564_FLOW *flow = obj->data;

    if (flow->timer->stopF == 0) {
        flow->timer->stopF = 1;
        flow->timer->mode = 1;
    }
}

void fn_1_333C(void) {
}

void fn_1_3340(OMOBJ *obj) {
    if (WipeCheckEnd() == 0) {
        MgFlowNext(obj);
    }
}

void fn_1_33B8(void) {
    if (lbl_1_bss_F4 != -1) {
        HuAudFadeOut(1000);
        lbl_1_bss_F4 = -1;
    }
    if (WipeCheckEnd() == 0) {
        omOvlReturnEx(1, 1);
    }
}

void fn_1_341C(OMOBJ *obj, s16 mesNo, s16 mesType) {
    M564_FLOW *flow = obj->data;

    if (flow->gameMesId == -1) {
        flow->gameMesId = GameMesCreate(mesNo, mesType);
        return;
    }
    if (GameMesStatGet(flow->gameMesId) == 0) {
        flow->gameMesId = -1;
        MgFlowNext(obj);
    }
}

void fn_1_34D8(OMOBJ *obj) {
    M564_FLOW *flow = obj->data;

    if (flow->gameMesId == -1) {
        flow->gameMesId = GameMesCreate(2, 0);
        return;
    }
    if (GameMesStatGet(flow->gameMesId) == 0) {
        flow->gameMesId = -1;
        MgFlowNext(obj);
    }
}

void fn_1_358C(OMOBJ *obj) {
    M564_FLOW *flow = obj->data;

    if (flow->gameMesId == -1) {
        flow->gameMesId = GameMesCreate(2, 1);
        return;
    }
    if (GameMesStatGet(flow->gameMesId) == 0) {
        flow->gameMesId = -1;
        MgFlowNext(obj);
    }
}

void fn_1_3640(OMOBJ *obj) {
    M564_FLOW *flow = obj->data;

    flow->timer = MgTimerCreate(flow->timerType);
    MgTimerParamSet(flow->timer, flow->maxTime, flow->endTime, flow->recordTime);
    MgTimerModeOnSet(flow->timer, 1);
    MgFlowNext(obj);
}

void fn_1_36E0(M564_PLAYER *work, s16 playerNo) {
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

s32 fn_1_37AC(Vec *pos, Vec *target, Vec *angle, f32 step) {
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
    angle->y = (f32)(180.0 * (atan2((f64)vec.x, (f64)vec.z) / M564_PI));
    return 0;
}

f32 fn_1_38D0(f32 cur, f32 target, f32 rate) {
    f32 diff;
    f32 result;

    diff = M564Fmod(target - cur, 360.0);
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
    result = M564Fmod(cur + diff, 360.0);
    if (result < 0.0f) {
        result += 360.0f;
    }
    return result;
}

s32 fn_1_3A40(Vec *pos, Vec *target, Vec *cur, f32 dist) {
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
        ang = (f32)(180.0 * (atan2((f64)sp14.z, (f64)sp14.x) / M564_PI));
        ang += 30.0f;
        sp20.x = (f32)((f64)radius * sin(M564_RAD(ang)));
        sp20.z = (f32)((f64)radius * cos(M564_RAD(ang)));
        PSVECAdd(pos, &sp20, target);
    }
}

void fn_1_3C78(Vec *a, Vec *b, Vec *c, Vec *out) {
    Vec sp1C;
    Vec sp10;

    PSVECSubtract(c, b, &sp1C);
    PSVECSubtract(b, a, &sp10);
    PSVECCrossProduct(&sp1C, &sp10, out);
    PSVECNormalize(out, out);
}

f32 fn_1_3CF0(Vec *normal, Vec *planePt, Vec *pos) {
    Vec sp14;

    PSVECSubtract(pos, planePt, &sp14);
    return PSVECDotProduct(normal, &sp14);
}

s32 fn_1_3D34(Vec *normal, Vec *planePt, Vec *a, Vec *b, Vec *out) {
    Vec sp28;
    Vec sp1C;
    Vec sp10;
    f32 denom;
    f32 numer;

    PSVECSubtract(b, a, &sp28);
    PSVECSubtract(a, planePt, &sp1C);
    numer = M564DotA(normal, &sp1C);
    PSVECSubtract(a, b, &sp10);
    denom = M564DotA(normal, &sp10);
    if (denom == 0.0f) {
        return 0;
    }
    PSVECScale(&sp28, &sp28, numer / denom);
    PSVECAdd(a, &sp28, out);
    return 1;
}
