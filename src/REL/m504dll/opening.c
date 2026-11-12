#define _MATH_H

#include "REL/m504dll.h"
#include "game/data.h"
#include "string.h"

#ifdef __MWERKS__
#define OP_RODATA __declspec(section ".rodata")
#else
#define OP_RODATA
#endif

typedef struct FPair_s {
    f32 a;
    f32 b;
} FPAIR;

typedef struct OpeningUnit_s {
    s32 frame; /* 0x00 */
    s32 phase; /* 0x04 */
    s32 flagA; /* 0x08 */
    s32 flagB; /* 0x0C */
    s16 confetti[10]; /* 0x10 */
    s16 modelId[20]; /* 0x24 */
    s32 live[20]; /* 0x4C */
    HuVecF vec[20]; /* 0x9C */
} OPENINGUNIT;

extern s32 rand8(void);
extern double atan2();
extern double fmod();
extern double sqrt();
extern double sin();
extern double cos();
extern float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

extern void fn_1_5EC(s32 seId);
extern s32 fn_1_76C(s16 streamId);
extern void fn_1_C99C(M504CAMERA *cam, s16 type, s32 cnt, f32 amp, f32 decay, f32 speed);
extern void fn_1_CC3C(M504CAMERA *cam, Vec *pos, Vec *up, Vec *look);
extern void fn_1_CC88(M504CAMERA *cam, Vec *pos, Vec *up, Vec *look);
extern s32 fn_1_CCEC(M504CAMERA *cam, Vec *up, f32 duration);

void fn_1_10CEC(f32 x, f32 y);
void fn_1_11088(void);
void fn_1_11198(Vec *pos, Vec *vel);
void fn_1_11360(Vec *pos, Vec *vel);
void fn_1_11548(Mtx m, Vec *rot);

Vec lbl_1_data_4648 = { 0.0f, 0.0f, 100.0f };
Vec lbl_1_data_4654 = { 0.0f, 180.0f, 0.0f };
Vec lbl_1_data_4660 = { 220.0f, 0.0f, 250.0f };
Vec lbl_1_data_466C = { 0.0f, -120.0f, 0.0f };
Vec lbl_1_data_4678 = { 0.0f, 0.0f, -600.0f };
Vec lbl_1_data_4684 = { 0.0f, 0.0f, -100.0f };
Vec lbl_1_data_4690 = { 0.0f, 0.0f, 100.0f };
Vec lbl_1_data_469C = { 0.0f, 0.0f, 0.0f };
Vec lbl_1_data_46A8 = { 0.0f, 0.0f, 0.0f };
Vec lbl_1_data_46B4 = { 0.0f, 18.0f, 200.0f };
Vec lbl_1_data_46C0 = { 90.0f, 0.0f, 0.0f };
Vec lbl_1_data_46CC[10] = {
    { -150.0f, 50.0f, -500.0f },
    { 50.0f, 200.0f, -700.0f },
    { 230.0f, 150.0f, -500.0f },
    { -400.0f, 100.0f, -600.0f },
    { -250.0f, 200.0f, -700.0f },
    { -200.0f, 150.0f, -500.0f },
    { 250.0f, 50.0f, -500.0f },
    { 300.0f, 200.0f, -700.0f },
    { 350.0f, 150.0f, -500.0f },
    { 200.0f, 300.0f, -700.0f },
};
Vec lbl_1_data_4744[10] = {
    { 30.0f, 15.0f, 0.0f },
    { 30.0f, -5.0f, 0.0f },
    { 30.0f, -15.0f, 0.0f },
    { 30.0f, 15.0f, 0.0f },
    { 30.0f, 10.0f, 0.0f },
    { 30.0f, 5.0f, 0.0f },
    { 30.0f, -5.0f, 0.0f },
    { 30.0f, -10.0f, 0.0f },
    { 30.0f, -15.0f, 0.0f },
    { 30.0f, -10.0f, 0.0f },
};
Vec lbl_1_data_47BC[10] = {
    { 1.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f },
    { -1.0f, 0.0f, 0.0f },
    { 1.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f },
    { -1.0f, 0.0f, 0.0f },
    { 1.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f },
    { -1.0f, 0.0f, 0.0f },
    { 1.0f, 0.0f, 0.0f },
};
Vec lbl_1_data_4834 = { 0.0f, 432.0f, 522.0f };
Vec lbl_1_data_4840 = { 0.0f, 332.0f, 1481.0f };
Vec lbl_1_data_484C = { 0.0f, 1800.0f, 1250.0f };
Vec lbl_1_data_4858[2] = {
    { 0.0f, 100.0f, -600.0f },
    { 0.0f, 0.0f, 300.0f },
};
Vec lbl_1_data_4870 = { 0.0f, 200.0f, 250.0f };
HuVecF lbl_1_data_487C = { 0.0f, 8000.0f, 1500.0f };
HuVecF lbl_1_data_4888 = { 0.0f, 1.0f, 0.0f };
HuVecF lbl_1_data_4894 = { 0.0f, 0.0f, 0.0f };

f32 lbl_1_bss_7848[10];
OPENINGUNIT lbl_1_bss_7870;
f32 lbl_1_bss_79FC;
f32 lbl_1_bss_7A00;

OP_RODATA const Vec lbl_1_rodata_3F8 = { 0.0f, 1.0f, 0.0f };
OP_RODATA const Vec lbl_1_rodata_404 = { 0.0f, 45.0f, 0.0f };
OP_RODATA const Vec lbl_1_rodata_410 = { 0.0f, 0.0f, 0.0f };
OP_RODATA const Vec lbl_1_rodata_41C = { 0.0f, 0.0f, 0.0f };
OP_RODATA const Vec lbl_1_rodata_428 = { 0.0f, 0.0f, 0.0f };
OP_RODATA const Vec lbl_1_rodata_434 = { 0.0f, 0.0f, 0.0f };
OP_RODATA const Vec lbl_1_rodata_440 = { 0.0f, 0.0f, 0.0f };
OP_RODATA const Vec lbl_1_rodata_44C = { 0.0f, 0.0f, 0.0f };
OP_RODATA const Vec lbl_1_rodata_458 = { 0.0f, 0.0f, 0.0f };
OP_RODATA const Vec lbl_1_rodata_464 = { 0.0f, 0.0f, 6.0f };
OP_RODATA const Vec lbl_1_rodata_470 = { 0.0f, 0.0f, 0.0f };
OP_RODATA const Vec lbl_1_rodata_47C = { 0.0f, 0.0f, 6.0f };
OP_RODATA const Vec lbl_1_rodata_488 = { 0.0f, 0.0f, -1.0f };
OP_RODATA const Vec lbl_1_rodata_494 = { 0.0f, 0.0f, 0.0f };
OP_RODATA const Vec lbl_1_rodata_4A0 = { 0.0f, 0.0f, 0.0f };
OP_RODATA const Vec lbl_1_rodata_4AC = { 0.0f, 0.0f, 0.0f };
OP_RODATA const Vec lbl_1_rodata_4B8 = { 0.0f, 0.0f, 0.0f };

s32 fn_1_E14C(M504WORK *work)
{
    M504PLAYER *player;
    M504STAGE *stage;
    M504WANWAN *wanwan;
    M504CAMERA *cam;
    M504WANWAN *ww;
    OPENINGUNIT clearUnit;
    void *clearPtr;
    s16 *mdlTbl;
    Vec velV;
    Vec posV;
    Vec tmpV;
    Mtx mtx;
    f32 maxTime;
    f32 time;
    f32 bob;
    f32 c;
    s32 ret;
    s32 i;
    s32 v;
    s32 v2;
    s32 v3;
    s32 v4;
    s32 v5;

    player = &work->slot[0].player;
    stage = &work->slot[0].stage;
    wanwan = &work->slot[0].wanwan;
    cam = &work->slot[0].cam;
    mdlTbl = work->mdl;
    ret = 0;
    fn_1_11088();
    switch (work->step) {
        case 0:
            clearUnit = lbl_1_bss_7870;
            clearPtr = &clearUnit;
            memset(clearPtr, 0, sizeof(OPENINGUNIT));
            work->unk1230 = fn_1_76C(0x4D);
            wanwan->off.z = 0.0f;
            work->unk1218 = 1;
            for (i = 0; i < 10; i++) {
                if ((i % 3) == 0) {
                    lbl_1_bss_7870.confetti[i] = Hu3DModelCreate(HuDataSelHeapReadNum(0x3E001F, 0x10000000, HEAP_MODEL));
                } else if ((i % 3) == 1) {
                    lbl_1_bss_7870.confetti[i] = Hu3DModelCreate(HuDataSelHeapReadNum(0x3E0020, 0x10000000, HEAP_MODEL));
                } else {
                    lbl_1_bss_7870.confetti[i] = Hu3DModelCreate(HuDataSelHeapReadNum(0x3E0021, 0x10000000, HEAP_MODEL));
                }
                Hu3DModelAttrReset(lbl_1_bss_7870.confetti[i], 1);
                Hu3DModelAttrSet(lbl_1_bss_7870.confetti[i], 0x40000001);
                Hu3DModelPosSetV(lbl_1_bss_7870.confetti[i], (HuVecF *)&lbl_1_data_46CC[i]);
                Hu3DModelRotSetV(lbl_1_bss_7870.confetti[i], (HuVecF *)&lbl_1_data_4744[i]);
                Hu3DModelScaleSet(lbl_1_bss_7870.confetti[i], 0.3f, 0.3f, 0.3f);
                Hu3DModelCameraSet(lbl_1_bss_7870.confetti[i], (u16)lbl_1_data_45E0[0]);
                Hu3DMotionSpeedSet(lbl_1_bss_7870.confetti[i], 4.0f);
                Hu3DMotionTimeSet(lbl_1_bss_7870.confetti[i], 10.0f * i);
                Hu3DModelShadowSet(lbl_1_bss_7870.confetti[i]);
            }
            for (i = 0; i < 20; i++) {
                lbl_1_bss_7870.live[i / 2] = 0;
                if (i < 10) {
                    lbl_1_bss_7870.modelId[i] = Hu3DModelCreate(HuDataSelHeapReadNum(0x3E0022, 0x10000000, HEAP_MODEL));
                } else {
                    lbl_1_bss_7870.modelId[i] = Hu3DModelCreate(HuDataSelHeapReadNum(0x3E0023, 0x10000000, HEAP_MODEL));
                }
                Hu3DModelAttrSet(lbl_1_bss_7870.modelId[i], 1);
                Hu3DModelAttrSet(lbl_1_bss_7870.modelId[i], 0x40000002);
                Hu3DModelPosSet(lbl_1_bss_7870.modelId[i], 0.0f, 0.0f, 0.0f);
                Hu3DModelCameraSet(lbl_1_bss_7870.modelId[i], (u16)lbl_1_data_45E0[0]);
                Hu3DMotionSpeedSet(lbl_1_bss_7870.modelId[i], 1.0f);
                Hu3DMotionTimeSet(lbl_1_bss_7870.modelId[i], 0.0f);
            }
            for (i = 0; i < 4; i++) {
                ww = &work->slot[i].wanwan;
                omSetTra(work->slot[i].player.obj, lbl_1_data_4660.x, lbl_1_data_4660.y, lbl_1_data_4660.z);
                omSetRot(work->slot[i].player.obj, lbl_1_data_466C.x, lbl_1_data_466C.y, lbl_1_data_466C.z);
                Hu3DModelPosSetV(ww->mdl, (HuVecF *)&lbl_1_data_4690);
                work->slot[i].cam.look = lbl_1_data_4870;
                work->slot[i].cam.lookTgt = work->slot[i].cam.look;
                work->slot[i].cam.pos = lbl_1_data_484C;
                work->slot[i].cam.posTgt = work->slot[i].cam.pos;
                lbl_1_data_487C.z = 700.0f + work->slot[i].player.obj->trans.z;
                lbl_1_data_4894.z = work->slot[i].player.obj->trans.z;
                Hu3DModelAttrSet(work->slot[i].player.obj->mdlId[0], 1);
                Hu3DShadowMultiPosSet(&lbl_1_data_487C, &lbl_1_data_4888, &lbl_1_data_4894, (s16)lbl_1_data_45E0[i]);
                Hu3DModelAttrSet(work->slot[i].stage.mdl14, 1);
                Hu3DModelAttrSet(work->slot[i].stage.mdl14, 0x40000001);
                Hu3DModelPosSetV(work->slot[i].stage.mdl14, (HuVecF *)&lbl_1_data_46B4);
                Hu3DModelRotSetV(work->slot[i].stage.mdl14, (HuVecF *)&lbl_1_data_46C0);
            }
            Hu3DModelAttrReset(player->obj->mdlId[0], 1);
            tmpV = lbl_1_rodata_3F8;
            cam->up = tmpV;
            cam->look = lbl_1_data_4858[0];
            cam->lookTgt = cam->look;
            cam->pos = lbl_1_data_4834;
            cam->posTgt = cam->pos;
            wanwan->pos = lbl_1_data_4678;
            tmpV = lbl_1_rodata_404;
            wanwan->rot = tmpV;
            omSetTra(player->obj, lbl_1_data_4648.x, lbl_1_data_4648.y, lbl_1_data_4648.z);
            omSetRot(player->obj, lbl_1_data_4654.x, lbl_1_data_4654.y, lbl_1_data_4654.z);
            Hu3DMotionSet(wanwan->mdl, wanwan->mtn[8]);
            Hu3DModelAttrSet(wanwan->mdl, 0x40000001);
            Hu3DMotionSpeedSet(wanwan->mdl, 1.0f);
            Hu3DMotionTimeSet(wanwan->mdl, 0.0f);
            Hu3DModelAttrSet(wanwan->mdl, 0x40000001);
            Hu3DModelPosSetV(stage->mdl14, (HuVecF *)&lbl_1_data_469C);
            Hu3DModelRotSetV(stage->mdl14, (HuVecF *)&lbl_1_data_46A8);
            Hu3DModelAttrReset(stage->mdl14, 1);
            Hu3DModelShadowSet(stage->mdl14);
            lbl_1_bss_7A00 = 640.0f;
            lbl_1_bss_79FC = 480.0f;
            fn_1_10CEC(lbl_1_bss_7A00, lbl_1_bss_79FC);
            fn_1_CC3C(cam, &cam->pos, &cam->up, &cam->look);
            fn_1_CC88(cam, &lbl_1_data_4840, &cam->up, &cam->look);
            Hu3DMotionShiftSet(player->obj->mdlId[0], player->obj->mtnId[7], 0.0f, 10.0f, 0);
            work->unk10 = 0;
            work->step++;
            break;
        case 1:
            lbl_1_bss_7870.frame++;
            if (Hu3DMotionEndCheck(player->obj->mdlId[0]) != 0) {
                Hu3DMotionShiftSet(player->obj->mdlId[0], player->obj->mtnId[8], 0.0f, 10.0f, 0);
            }
            if (fn_1_CCEC(cam, &cam->up, 60.0f) != 0) {
                Hu3DModelPosSet(player->mdlC8, 0.0f, 80.0f, 0.0f);
                Hu3DModelRotSet(player->mdlC8, 20.0f, 0.0f, 0.0f);
                Hu3DMotionTimeSet(player->mdlC8, 0.0f);
                Hu3DMotionSpeedSet(player->mdlC8, 2.0f);
                Hu3DModelAttrReset(player->mdlC8, 1);
                fn_1_5EC(0x65B);
                lbl_1_bss_7870.phase = 0;
                lbl_1_bss_7870.phase = 0;
                work->step++;
            }
            break;
        case 2:
            lbl_1_bss_7870.frame++;
            lbl_1_bss_7870.phase++;
            if (lbl_1_bss_7870.phase >= 10) {
                Hu3DMotionShiftSet(wanwan->mdl, wanwan->mtn[9], 0.0f, 10.0f, 0);
                PSVECSubtract(&cam->look, &cam->pos, &tmpV);
                PSVECScale(&tmpV, &tmpV, 0.6f);
                PSVECAdd(&cam->pos, &tmpV, &cam->look);
                cam->lookTgt = cam->look;
                wanwan->pos.y = 0.0f;
                wanwan->pos.x = 0.0f;
                lbl_1_bss_7870.phase = 0;
                lbl_1_bss_7870.frame = 0;
                velV = lbl_1_rodata_410;
                posV = wanwan->pos;
                fn_1_11198(&posV, &velV);
                velV = lbl_1_rodata_41C;
                posV = wanwan->pos;
                fn_1_11360(&posV, &velV);
                for (i = 0; i < 10; i++) {
                    lbl_1_bss_7848[i] = 0.01f * lbl_1_data_46CC[i].x;
                }
                work->step++;
            }
            if (lbl_1_bss_7870.phase == 4) {
                Hu3DModelAttrReset(player->mdlCC, 1);
                omVibrate(0, 15, 6, 6);
                omVibrate(1, 15, 6, 6);
                omVibrate(2, 15, 6, 6);
                omVibrate(3, 15, 6, 6);
            }
            break;
        case 3:
            if (Hu3DMotionEndCheck(player->mdlC8) != 0) {
                Hu3DModelAttrSet(player->mdlC8, 1);
                work->step++;
            }
            break;
        case 4:
            lbl_1_bss_7870.frame++;
            if (lbl_1_bss_7870.frame >= 15) {
                lbl_1_bss_7870.frame = 0;
                work->step++;
            }
            break;
        case 5:
            wanwan->rot.y += 10.0f;
            if (wanwan->rot.y > 0.0f) {
                Hu3DMotionShiftSet(wanwan->mdl, wanwan->mtn[1], 0.0f, 10.0f, 0x40000001);
                fn_1_CC3C(cam, &cam->pos, &cam->up, &cam->look);
                fn_1_CC88(cam, &lbl_1_data_484C, &cam->up, &lbl_1_data_4870);
                wanwan->f68 = 20.0f;
                wanwan->f6C = 0.0f;
                wanwan->timer = 30;
                wanwan->rot.y = 0.0f;
                lbl_1_bss_7870.frame = 0;
                fn_1_5EC(0x2D8);
                work->step++;
            }
            break;
        case 6:
            wanwan->f6C += 10.0f;
            if (wanwan->f6C > 90.0f) {
                wanwan->f6C = 0.0f;
            }
            c = cos(3.141592653589793 * wanwan->f6C / 180.0);
            wanwan->rot.x = wanwan->f68 * c;
            wanwan->off.z = (50.0f * c) * cos(3.141592653589793 * wanwan->rot.y / 180.0);
            if (wanwan->timer != 0) {
                wanwan->timer--;
            }
            if (wanwan->timer == 0) {
                wanwan->off.z = 0.0f;
                wanwan->rot.x = 0.0f;
                wanwan->timer = 0;
                lbl_1_bss_7870.frame = 0;
                work->step++;
            }
            break;
        case 7:
            lbl_1_bss_7870.frame++;
            v = lbl_1_bss_7870.frame - 25;
            if (v < 0) {
                v = 0;
            }
            if (v > 30) {
                v = 30;
            }
            if (lbl_1_bss_7870.flagA == 0) {
                if (v == 1) {
                    Hu3DMotionShiftSet(player->obj->mdlId[0], player->obj->mtnId[11], 0.0f, 10.0f, 0x40000001);
                    Hu3DModelAttrSet(player->mdlCC, 1);
                }
                if (v != 0) {
                    player->obj->rot.y -= 15.0f;
                    if (player->obj->rot.y < 60.0f) {
                        player->obj->rot.y = 60.0f;
                    }
                }
                if (v == 30) {
                    for (i = 0; i < 4; i++) {
                        Hu3DMotionShiftSet(work->slot[i].player.obj->mdlId[0], work->slot[i].player.obj->mtnId[0], 0.0f, 10.0f, 0x40000001);
                    }
                    lbl_1_bss_7870.flagA = 1;
                }
                player->obj->trans.x = lbl_1_data_4648.x + (((f32)v * (lbl_1_data_4660.x - lbl_1_data_4648.x)) / 30.0f);
                player->obj->trans.z = lbl_1_data_4648.z + (((f32)v * (lbl_1_data_4660.z - lbl_1_data_4648.z)) / 30.0f);
            } else {
                player->obj->rot.y += 10.0f;
                if (player->obj->rot.y > 240.0f) {
                    player->obj->rot.y = 240.0f;
                }
            }
            v2 = lbl_1_bss_7870.frame;
            if (v2 > 35) {
                v2 = 35;
            }
            if (lbl_1_bss_7870.flagB == 0) {
                if (v2 >= 5 && ((v2 - 5) % 3) == 0) {
                    velV = lbl_1_rodata_434;
                    velV.z = 0.2f * (v2 - 5);
                    posV = lbl_1_rodata_428;
                    posV.x = 70.0f + wanwan->pos.x;
                    posV.y = wanwan->pos.y;
                    posV.z = wanwan->pos.z;
                    fn_1_11198(&posV, &velV);
                    velV = lbl_1_rodata_44C;
                    velV.z = 0.2f * (v2 - 5);
                    posV = lbl_1_rodata_440;
                    posV.x = wanwan->pos.x - 70.0f;
                    posV.y = wanwan->pos.y;
                    posV.z = wanwan->pos.z;
                    fn_1_11360(&posV, &velV);
                }
                if (v2 == 1) {
                    fn_1_C99C(cam, 0, 60, 1.0f, 1.08f, 35.0f);
                }
                if (v2 == 20) {
                    Hu3DMotionShiftSet(wanwan->mdl, wanwan->mtn[2], 0.0f, 10.0f, 0);
                }
                if (v2 == 35) {
                    omVibrate(0, 30, 6, 6);
                    omVibrate(1, 30, 6, 6);
                    omVibrate(2, 30, 6, 6);
                    omVibrate(3, 30, 6, 6);
                    fn_1_C99C(cam, 0, 60, 25.0f, 0.95f, 90.0f);
                    Hu3DMotionShiftSet(wanwan->mdl, wanwan->mtn[7], 0.0f, 10.0f, 0);
                    fn_1_5EC(0x648);
                    Hu3DModelPosSet(wanwan->mdl28, wanwan->pos.x, 150.0f + wanwan->pos.y, 150.0f + wanwan->pos.z);
                    Hu3DMotionTimeSet(wanwan->mdl28, 0.0f);
                    Hu3DModelAttrReset(wanwan->mdl28, 1);
                    lbl_1_bss_7870.flagB = 1;
                }
            } else if (lbl_1_bss_7870.flagB == 1 && Hu3DMotionEndCheck(wanwan->mdl) != 0) {
                for (i = 0; i < 4; i++) {
                    Hu3DMotionShiftSet(work->slot[i].wanwan.mdl, work->slot[i].wanwan.mtn[6], 0.0f, 10.0f, 0x40000001);
                }
                lbl_1_bss_7870.flagB = 2;
            }
            if (lbl_1_bss_7870.flagB == 0) {
                wanwan->pos.z = lbl_1_data_4678.z + ((lbl_1_data_4684.z - lbl_1_data_4678.z) * (1.0 - cos(3.141592653589793 * (90.0f * ((f32)v2 / 35.0f)) / 180.0)));
            } else {
                v3 = lbl_1_bss_7870.frame - 35;
                if (v3 > 30) {
                    v3 = 30;
                }
                if (v3 < 25 && (v3 % 3) == 0) {
                    velV = lbl_1_rodata_464;
                    posV = lbl_1_rodata_458;
                    posV.x = 70.0f + wanwan->pos.x;
                    posV.y = wanwan->pos.y;
                    posV.z = wanwan->pos.z;
                    fn_1_11198(&posV, &velV);
                    velV = lbl_1_rodata_47C;
                    posV = lbl_1_rodata_470;
                    posV.x = wanwan->pos.x - 70.0f;
                    posV.y = wanwan->pos.y;
                    posV.z = wanwan->pos.z;
                    fn_1_11360(&posV, &velV);
                }
                Hu3DModelPosSet(wanwan->mdl28, wanwan->pos.x, 200.0f + wanwan->pos.y, (150.0f + wanwan->pos.z) - (5.0f * v3));
                if (Hu3DMotionEndCheck(wanwan->mdl28) != 0) {
                    Hu3DModelAttrSet(wanwan->mdl28, 1);
                }
                wanwan->pos.z = lbl_1_data_4684.z + ((lbl_1_data_4690.z - lbl_1_data_4684.z) * sin(3.141592653589793 * (90.0f * ((f32)v3 / 30.0f)) / 180.0));
            }
            if (lbl_1_bss_7870.frame >= 35) {
                v4 = lbl_1_bss_7870.frame - 35;
                if (v4 > 30) {
                    v4 = 30;
                }
                Hu3DData[stage->mdl14].pos.z = 200.0 * sin(3.141592653589793 * (90.0f * ((f32)v4 / 30.0f)) / 180.0);
                if (v4 > 15) {
                    v4 = 15;
                }
                if (v4 == 14) {
                    Hu3DModelShadowDispOff(stage->mdl14);
                }
                Hu3DData[stage->mdl14].pos.y = (1.2f * v4) + (80.0 * sin(3.141592653589793 * (180.0f * ((f32)v4 / 15.0f)) / 180.0));
                Hu3DData[stage->mdl14].rot.x += 7.5f;
                if (Hu3DData[stage->mdl14].rot.x > 90.0f) {
                    Hu3DData[stage->mdl14].rot.x = 90.0f;
                }
            }
            if (lbl_1_bss_7870.frame >= 65) {
                v5 = lbl_1_bss_7870.frame - 65;
                if ((v5 % 2) != 0) {
                    Hu3DModelAttrSet(stage->mdl14, 1);
                } else {
                    Hu3DModelAttrReset(stage->mdl14, 1);
                }
                if (v5 >= 30) {
                    Hu3DModelAttrSet(stage->mdl14, 1);
                }
            }
            if (fn_1_CCEC(cam, &cam->up, 90.0f) != 0) {
                for (i = 0; i < 10; i++) {
                    Hu3DModelAttrSet(lbl_1_bss_7870.confetti[i], 0x40000002);
                    Hu3DModelAttrSet(lbl_1_bss_7870.confetti[i], 1);
                    Hu3DModelKill(lbl_1_bss_7870.confetti[i]);
                }
                Hu3DModelAttrReset(work->slot[0].player.obj->mdlId[0], 1);
                Hu3DModelAttrReset(work->slot[1].player.obj->mdlId[0], 1);
                Hu3DModelAttrReset(work->slot[2].player.obj->mdlId[0], 1);
                Hu3DModelAttrReset(work->slot[3].player.obj->mdlId[0], 1);
                work->unk10 = 0;
                lbl_1_bss_7870.frame = 0;
                for (i = 1; i < 4; i++) {
                    work->slot[i].wanwan.pos = work->slot[0].wanwan.pos;
                    work->slot[i].wanwan.rot = work->slot[0].wanwan.rot;
                    work->slot[i].wanwan.f68 = work->slot[0].wanwan.f68;
                    work->slot[i].wanwan.f6C = work->slot[0].wanwan.f6C;
                    work->slot[i].wanwan.f70 = work->slot[0].wanwan.f70;
                }
                work->step++;
            }
            if (lbl_1_bss_7870.frame == 45) {
                tmpV = lbl_1_rodata_488;
                cam->up = tmpV;
            }
            break;
        case 8:
            work->unk10++;
            if (work->unk10 >= 45) {
                for (i = 0; i < 20; i++) {
                    Hu3DModelAttrSet(lbl_1_bss_7870.modelId[i], 1);
                }
                work->step++;
                work->unk10 = 45;
            }
            lbl_1_bss_7A00 = 640.0f - ((320.0f * work->unk10) / 45.0f);
            lbl_1_bss_79FC = 480.0f - ((240.0f * work->unk10) / 45.0f);
            fn_1_10CEC(lbl_1_bss_7A00, lbl_1_bss_79FC);
            break;
        case 9:
            work->gameMes = -1;
            work->unk1218 = 0;
            work->step++;
            break;
        default:
            work->unk10 = 0;
            work->step = 0;
            ret = 1;
            break;
    }
    if (work->step < 8 && ret == 0) {
        maxTime = Hu3DMotionMaxTimeGet(lbl_1_bss_7870.confetti[0]);
        for (i = 0; i < 10; i++) {
            time = Hu3DMotionTimeGet(lbl_1_bss_7870.confetti[i]);
            if (time < 1.0f) {
                if (lbl_1_data_46CC[i].x < 0.0f) {
                    lbl_1_data_47BC[i].x = 4.0f * ((rand8() % 6) - 3);
                } else {
                    lbl_1_data_47BC[i].x = 4.0f * ((rand8() % 6) - 4);
                }
                if (lbl_1_data_46CC[i].y < 150.0f) {
                    lbl_1_data_47BC[i].y = 2.0f * ((rand8() % 6) - 3);
                } else {
                    lbl_1_data_47BC[i].y = 2.0f * ((rand8() % 6) - 4);
                }
                PSVECScale(&lbl_1_data_47BC[i], &lbl_1_data_47BC[i], 2.0f / maxTime);
            }
            PSVECAdd(&lbl_1_data_46CC[i], &lbl_1_data_47BC[i], &lbl_1_data_46CC[i]);
            if (work->step >= 3) {
                lbl_1_data_46CC[i].x += lbl_1_bss_7848[i];
                lbl_1_data_46CC[i].z -= 1.0f;
                lbl_1_data_46CC[i].y += 1.0f;
            }
            bob = 10.0 * sin(3.141592653589793 * (180.0f + ((180.0f * time) / maxTime)) / 180.0);
            if (lbl_1_data_46CC[i].y < 20.0f) {
                lbl_1_data_46CC[i].y = 20.0f;
            }
            Hu3DModelPosSet(lbl_1_bss_7870.confetti[i], lbl_1_data_46CC[i].x, bob + lbl_1_data_46CC[i].y, lbl_1_data_46CC[i].z);
        }
    }
    if ((work->step == 7 && lbl_1_bss_7870.flagB >= 2) || work->step >= 8) {
        for (i = 0; i < 4; i++) {
            ww = &work->slot[i].wanwan;
            Hu3DModelAttrReset(ww->mdl, 0x40000001);
            Hu3DModelAttrSet(ww->mdl, 0x40000004);
            ww->f6C += 8.0f;
            ww->f68 = 10.0 * sin(3.141592653589793 * ww->f6C / 180.0);
            ww->f70 = 10.0 * cos(3.141592653589793 * ww->f6C / 180.0);
            mtxRot(mtx, 0.0f, 360.0f - ww->face, ww->f70);
            mtxRotCat(mtx, ww->f68, 0.0f, 0.0f);
            fn_1_11548(mtx, &ww->rot);
            ww->pos.y = 0.0f;
        }
    }
    if (work->step >= 1 && work->step <= 2) {
        wanwan->rot.y = 100.0 * sin(3.141592653589793 * (360.0f * ((f32)((lbl_1_bss_7870.frame + 36) % 144) / 144.0f)) / 180.0);
        wanwan->pos.x = 100.0 * sin(3.141592653589793 * (360.0f * ((f32)(lbl_1_bss_7870.frame % 144) / 144.0f)) / 180.0);
        if ((lbl_1_bss_7870.frame % 10) == 0) {
            if ((lbl_1_bss_7870.frame % 144) >= 36 && (lbl_1_bss_7870.frame % 144) < 108) {
                velV = lbl_1_rodata_4A0;
                posV = lbl_1_rodata_494;
                posV.x = wanwan->pos.x - 15.0f;
                posV.y = wanwan->pos.y;
                posV.z = wanwan->pos.z;
                fn_1_11198(&posV, &velV);
            } else {
                velV = lbl_1_rodata_4B8;
                posV = lbl_1_rodata_4AC;
                posV.x = 15.0f + wanwan->pos.x;
                posV.y = wanwan->pos.y;
                posV.z = wanwan->pos.z;
                fn_1_11360(&posV, &velV);
            }
        }
    }
    for (i = 0; i < 4; i++) {
        ww = &work->slot[i].wanwan;
        Hu3DModelPosSet(ww->mdl, ww->pos.x, ww->pos.y, ww->pos.z + ww->off.z);
        Hu3DModelRotSetV(ww->mdl, (HuVecF *)&ww->rot);
    }
    return ret;
}

void fn_1_10CEC(f32 x, f32 y)
{
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, x - 1.0f, 20.0f + (y - 1.0f), 0.0f, 1.0f);
    Hu3DCameraScissorSet(1, 0, 0, (u32)(x - 1.0f), (u32)(y - 1.0f));
    Hu3DCameraViewportSet(2, 2.0f + x, 0.0f, 640.0f - x, 20.0f + (y - 1.0f), 0.0f, 1.0f);
    Hu3DCameraScissorSet(2, (u32)(2.0f + x), 0, (u32)(640.0f - x), (u32)(y - 1.0f));
    Hu3DCameraViewportSet(4, 0.0f, -20.0f + (2.0f + y), x - 1.0f, 20.0f + (480.0f - y), 0.0f, 1.0f);
    Hu3DCameraScissorSet(4, 0, (u32)(2.0f + y), (u32)(x - 1.0f), (u32)(480.0f - y));
    Hu3DCameraViewportSet(8, 2.0f + x, -20.0f + (2.0f + y), 640.0f - x, 20.0f + (480.0f - y), 0.0f, 1.0f);
    Hu3DCameraScissorSet(8, (u32)(2.0f + x), (u32)(2.0f + y), (u32)(640.0f - x), (u32)(480.0f - y));
}

void fn_1_11088(void)
{
    s32 i;

    for (i = 0; i < 20; i++) {
        if (lbl_1_bss_7870.live[i] == 0) {
            continue;
        }
        PSVECAdd(&Hu3DData[lbl_1_bss_7870.modelId[i]].pos, &lbl_1_bss_7870.vec[i],
                 &Hu3DData[lbl_1_bss_7870.modelId[i]].pos);
        if (Hu3DMotionEndCheck(lbl_1_bss_7870.modelId[i]) == 0) {
            continue;
        }
        lbl_1_bss_7870.live[i] = 0;
        Hu3DModelAttrSet(lbl_1_bss_7870.modelId[i], 1);
    }
}

void fn_1_11198(Vec *pos, Vec *vel)
{
    s32 i;
    s32 rnd;

    for (i = 0; i < 10; i++) {
        if (lbl_1_bss_7870.live[i] != 0) {
            continue;
        }
        lbl_1_bss_7870.live[i] = 1;
        lbl_1_bss_7870.vec[i] = *(HuVecF *)vel;
        rnd = rand8() % 30;
        Hu3DModelAttrReset(lbl_1_bss_7870.modelId[i], 1);
        /* NONMATCHING: CodeWarrior materialises the 0x4002 attr mask as lis+addi, MWCC folds
           it into one li, which also shifts the 3 branch offsets below. 8 spellings probed. */
        Hu3DModelAttrReset(lbl_1_bss_7870.modelId[i], 0x4000 + HU3D_ATTR_ZWRITE_OFF);
        Hu3DModelPosSet(lbl_1_bss_7870.modelId[i], pos->x + 10.0f + (f32)rnd, 0.0f, pos->z);
        Hu3DMotionSpeedSet(lbl_1_bss_7870.modelId[i], 1.0f);
        Hu3DMotionTimeSet(lbl_1_bss_7870.modelId[i], 0.0f);
        break;
    }
}

void fn_1_11360(Vec *pos, Vec *vel)
{
    s32 i;
    s32 rnd;

    for (i = 0; i < 10; i++) {
        if (lbl_1_bss_7870.live[i + 10] != 0) {
            continue;
        }
        lbl_1_bss_7870.live[i + 10] = 1;
        lbl_1_bss_7870.vec[i + 10] = *(HuVecF *)vel;
        rnd = rand8() % 30;
        Hu3DModelAttrReset(lbl_1_bss_7870.modelId[i + 10], 1);
        /* NONMATCHING: same 0x4002 attr-mask lis+addi vs li difference as fn_1_11198. */
        Hu3DModelAttrReset(lbl_1_bss_7870.modelId[i + 10], 0x4000 + HU3D_ATTR_ZWRITE_OFF);
        Hu3DModelPosSet(lbl_1_bss_7870.modelId[i + 10], pos->x - 10.0f - (f32)rnd, 0.0f, pos->z);
        Hu3DMotionTimeSet(lbl_1_bss_7870.modelId[i + 10], 0.0f);
        Hu3DMotionSpeedSet(lbl_1_bss_7870.modelId[i + 10], 1.0f);
        break;
    }
}

void fn_1_11548(Mtx m, Vec *rot)
{
    f64 y;
    f64 t;
    f32 z;
    f32 g;
    f32 s;
    FPAIR tmp;

    rot->x = (f32)(180.0 * (atan2(m[1][2], m[2][2]) / 3.141592653589793));
    if (rot->x < 0.0f) {
        rot->x += 360.0f;
    }
    rot->z = (f32)(180.0 * (atan2(m[0][1], m[0][0]) / 3.141592653589793));
    if (rot->z < 0.0f) {
        rot->z += 360.0f;
    }
    z = -m[0][2];
    /* NONMATCHING (2 rows here, 13 in the sqrt block below): CodeWarrior tests "1.0 - z*z" with
       fcmpo + cror eq,gt,eq + bne; MWCC 1.3.2 lowers the same float truthiness test to
       fcmpu + beq. Semantics are identical, the encodings are not. */
    if (1.0 - z * z) {
        t = 1.0 - z * z;
    } else {
        t = -(1.0 - z * z);
    }
    if ((f32)t > 0.0f) {
        /* NONMATCHING: CodeWarrior expands sqrt() inline as frsqrte + two Newton steps of
           y = 0.5*y*(3 - x*y*y); MWCC 1.3.2 at -O0,p never emits frsqrte (probed: sqrt,
           1.0/sqrt(x), sqrtf, float and double spellings all become bl sqrt), so the seed
           is a call here and a frsqrte there. The two Newton steps and the final multiply
           match byte for byte. */
        y = 1.0 / sqrt((f64)(f32)t);
        y = (0.5 * y) * (3.0 - (f64)(f32)t * (y * y));
        y = (0.5 * y) * (3.0 - (f64)(f32)t * (y * y));
        tmp.a = (f32)((f64)(f32)t * y); /* struct member forces the stfs/lfs round trip */
        s = tmp.a;
    } else {
        s = (f32)t;
    }
    g = s;
    if (rot->x > 90.0f && rot->x < 270.0f && rot->z > 90.0f && rot->z < 270.0f) {
        rot->x = (f32)fmod(180.0f + rot->x, 360.0);
        rot->z = (f32)fmod(180.0f + rot->z, 360.0);
        g = -g;
    }
    rot->y = (f32)(180.0 * (atan2(z, g) / 3.141592653589793));
    if (rot->y < 0.0f) {
        rot->y += 360.0f;
    }
}
