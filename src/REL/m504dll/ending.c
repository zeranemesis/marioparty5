#define _MATH_H

#include "REL/m504dll.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/gamemes.h"
#include "game/wipe.h"

extern s32 rand8(void);
extern double sin();

void PSVECAdd(const Vec *a, const Vec *b, Vec *ab);
void PSVECScale(const Vec *src, Vec *dst, f32 scale);

extern void fn_1_10CEC(f32 x, f32 y);
extern void fn_1_63C(s32 seNo, s32 speed);
extern s32 fn_1_794(s16 streamId);

HuVecF lbl_1_data_48C8 = { 0.0f, 0.0f, 3750.0f };
HuVecF lbl_1_data_48D4 = { 0.0f, 0.0f, 3600.0f };
HuVecF lbl_1_data_48E0 = { 0.0f, 850.0f, 5300.0f };
HuVecF lbl_1_data_48EC = { 0.0f, 150.0f, 3800.0f };
HuVecF lbl_1_data_48F8 = { 0.0f, 8000.0f, 1500.0f };
HuVecF lbl_1_data_4904 = { 0.0f, 1.0f, 0.0f };
HuVecF lbl_1_data_4910 = { 0.0f, 0.0f, 0.0f };
HuVecF lbl_1_data_491C[10] = {
    { -150.0f, 50.0f, 3500.0f },
    { -250.0f, 200.0f, 3700.0f },
    { 250.0f, 150.0f, 3500.0f },
    { -250.0f, 100.0f, 3400.0f },
    { -100.0f, 50.0f, 4000.0f },
    { -150.0f, 100.0f, 4100.0f },
    { 100.0f, 100.0f, 4100.0f },
    { 350.0f, 50.0f, 3900.0f },
    { 450.0f, 150.0f, 3500.0f },
    { 100.0f, 150.0f, 3300.0f },
};
HuVecF lbl_1_data_4994[10] = {
    { 30.0f, 10.0f, 0.0f },
    { 30.0f, 5.0f, 0.0f },
    { 30.0f, -10.0f, 0.0f },
    { 30.0f, 5.0f, 0.0f },
    { 30.0f, 10.0f, 0.0f },
    { 30.0f, 10.0f, 0.0f },
    { 30.0f, -10.0f, 0.0f },
    { 30.0f, -10.0f, 0.0f },
    { 30.0f, -5.0f, 0.0f },
    { 30.0f, -5.0f, 0.0f },
};
HuVecF lbl_1_data_4A0C[10] = {
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

s16 lbl_1_bss_7A08[10];
f32 lbl_1_bss_7A1C;
f32 lbl_1_bss_7A20;

s32 fn_1_118E8(M504WORK *work)
{
    M504PLAYER *player;
    M504WANWAN *wanwan;
    M504CAMERA *cam;
    OMOBJ *obj;
    s32 ret;
    s32 i;
    f32 maxTime;
    f32 time;
    f32 bob;
    s16 *mdl;

    player = &work->slot[work->winner].player;
    wanwan = &work->slot[work->winner].wanwan;
    cam = &work->slot[work->winner].cam;
    mdl = work->mdl;
    ret = 0;
    switch (work->step) {
        case 0:
            lbl_1_bss_7A20 = 320.0f;
            lbl_1_bss_7A1C = 240.0f;
            fn_1_10CEC(lbl_1_bss_7A20, lbl_1_bss_7A1C);
            WipeCreate(2, 0, 60);
            WipeColorSet(255, 255, 255);
            for (i = 0; i < 4; i++) {
                if (work->slot[i].stage.unk5C >= 0) {
                    fn_1_63C(work->slot[i].stage.unk5C, 1000);
                    work->slot[i].stage.unk5C = -1;
                }
            }
            work->unk10 = 0;
            work->step++;
            break;
        case 1:
            work->unk10++;
            if (work->unk10 >= 60) {
                work->unk10 = 60;
            }
            switch (work->winner) {
                case 0:
                    lbl_1_bss_7A20 = 320.0f + ((325.0f * work->unk10) / 60.0f);
                    lbl_1_bss_7A1C = 240.0f + ((245.0f * work->unk10) / 60.0f);
                    break;
                case 1:
                    lbl_1_bss_7A20 = 320.0f - ((325.0f * work->unk10) / 60.0f);
                    lbl_1_bss_7A1C = 240.0f + ((245.0f * work->unk10) / 60.0f);
                    break;
                case 2:
                    lbl_1_bss_7A20 = 320.0f + ((325.0f * work->unk10) / 60.0f);
                    lbl_1_bss_7A1C = 240.0f - ((245.0f * work->unk10) / 60.0f);
                    break;
                default:
                    lbl_1_bss_7A20 = 320.0f - ((325.0f * work->unk10) / 60.0f);
                    lbl_1_bss_7A1C = 240.0f - ((245.0f * work->unk10) / 60.0f);
                    break;
            }
            fn_1_10CEC(lbl_1_bss_7A20, lbl_1_bss_7A1C);
            if (WipeCheckEnd() != 0 || work->unk10 < 60) {
                break;
            }
            work->unk10 = 0;
            work->step++;
            /* fallthrough */
        case 2:
            Hu3DModelAttrSet(player->mdlCC, 1);
            Hu3DModelAttrSet(work->slot[0].player.obj->mdlId[0], 1);
            Hu3DModelAttrSet(work->slot[1].player.obj->mdlId[0], 1);
            Hu3DModelAttrSet(work->slot[2].player.obj->mdlId[0], 1);
            Hu3DModelAttrSet(work->slot[3].player.obj->mdlId[0], 1);
            Hu3DModelAttrReset(work->slot[work->winner].player.obj->mdlId[0], 1);
            Hu3DModelAttrReset(work->slot[work->winner].stage.mdl6, 1);
            Hu3DModelAttrReset(work->slot[work->winner].stage.mdl1C, 1);
            Hu3DModelAttrReset(work->slot[work->winner].stage.mdl1E, 1);
            Hu3DModelAttrReset(work->slot[work->winner].stage.mdl20, 1);
            work->unk1218 = 1;
            CharModelVoicePanSet(player->charNo, 64);
            cam->look = *(Vec *)&lbl_1_data_48EC;
            cam->lookTgt = cam->look;
            cam->pos = *(Vec *)&lbl_1_data_48E0;
            cam->posTgt = cam->pos;
            obj = player->obj;
            obj->trans.x = lbl_1_data_48C8.x;
            obj->trans.y = lbl_1_data_48C8.y;
            obj->trans.z = lbl_1_data_48C8.z;
            obj->rot.y = 0.0f;
            Hu3DModelPosSetV(wanwan->mdl, &lbl_1_data_48D4);
            Hu3DModelRotSet(wanwan->mdl, 0.0f, 0.0f, 0.0f);
            Hu3DMotionSet(wanwan->mdl, wanwan->mtn[0]);
            Hu3DMotionSpeedSet(wanwan->mdl, 1.0f);
            Hu3DMotionTimeSet(wanwan->mdl, 0.0f);
            Hu3DModelAttrSet(wanwan->mdl, 0x40000001);
            for (i = 0; i < 4; i++) {
                Hu3DMotionSet(work->slot[i].player.obj->mdlId[0], work->slot[i].player.obj->mtnId[0]);
            }
            Hu3DMotionSpeedSet(player->obj->mdlId[0], 1.0f);
            Hu3DMotionTimeSet(player->obj->mdlId[0], 0.0f);
            Hu3DModelAttrSet(player->obj->mdlId[0], 0x40000001);
            lbl_1_data_48F8.z = 1200.0f + player->obj->trans.z;
            lbl_1_data_4910.z = player->obj->trans.z;
            Hu3DShadowMultiPosSet(&lbl_1_data_48F8, &lbl_1_data_4904, &lbl_1_data_4910, (s16)lbl_1_data_45E0[work->winner]);
            for (i = 0; i < 10; i++) {
                if ((i % 3) == 0) {
                    lbl_1_bss_7A08[i] = Hu3DModelCreate(HuDataSelHeapReadNum(0x3E001F, 0x10000000, HEAP_MODEL));
                } else if ((i % 3) == 1) {
                    lbl_1_bss_7A08[i] = Hu3DModelCreate(HuDataSelHeapReadNum(0x3E0020, 0x10000000, HEAP_MODEL));
                } else {
                    lbl_1_bss_7A08[i] = Hu3DModelCreate(HuDataSelHeapReadNum(0x3E0021, 0x10000000, HEAP_MODEL));
                }
                Hu3DModelAttrReset(lbl_1_bss_7A08[i], 1);
                Hu3DModelAttrSet(lbl_1_bss_7A08[i], 0x40000001);
                Hu3DModelPosSetV(lbl_1_bss_7A08[i], &lbl_1_data_491C[i]);
                Hu3DModelRotSetV(lbl_1_bss_7A08[i], &lbl_1_data_4994[i]);
                Hu3DModelScaleSet(lbl_1_bss_7A08[i], 0.3f, 0.3f, 0.3f);
                Hu3DModelCameraSet(lbl_1_bss_7A08[i], (u16)lbl_1_data_45E0[work->winner]);
                Hu3DMotionSpeedSet(lbl_1_bss_7A08[i], 4.0f);
                Hu3DMotionTimeSet(lbl_1_bss_7A08[i], 10.0f * i);
                Hu3DModelShadowSet(lbl_1_bss_7A08[i]);
            }
            WipeCreate(1, 0, 60);
            work->step++;
            break;
        case 3:
            if (WipeCheckEnd() == 0) {
                work->gameMes = GameMesCreate(4, 3, player->charNo, -1, -1, -1);
                Hu3DMotionShiftSet(player->obj->mdlId[0], player->obj->mtnId[9], 0.0f, 10.0f, 0);
                Hu3DMotionShiftSet(wanwan->mdl, wanwan->mtn[8], 0.0f, 10.0f, 0x40000001);
                work->unk10 = 0;
                work->step++;
            }
            break;
        case 4:
            if (work->unk1230 < 0) {
                work->unk1230 = fn_1_794(0x79);
            }
            work->unk10++;
            if (work->unk10 >= 209) {
                work->gameMes = -1;
                work->unk10 = 0;
                work->step++;
            }
            break;
        default:
            work->unk10 = 0;
            ret = 1;
            break;
    }
    maxTime = Hu3DMotionMaxTimeGet(lbl_1_bss_7A08[0]);
    for (i = 0; i < 10; i++) {
        time = Hu3DMotionTimeGet(lbl_1_bss_7A08[i]);
        if (time < 1.0f) {
            if (lbl_1_data_491C[i].x < 0.0f) {
                lbl_1_data_4A0C[i].x = 4.0f * ((rand8() % 6) - 3);
            } else {
                lbl_1_data_4A0C[i].x = 4.0f * ((rand8() % 6) - 4);
            }
            if (lbl_1_data_491C[i].y < 150.0f) {
                lbl_1_data_4A0C[i].y = 2.0f * ((rand8() % 6) - 3);
            } else {
                lbl_1_data_4A0C[i].y = 2.0f * ((rand8() % 6) - 4);
            }
            PSVECScale((Vec *)&lbl_1_data_4A0C[i], (Vec *)&lbl_1_data_4A0C[i], 2.0f / maxTime);
        }
        PSVECAdd((Vec *)&lbl_1_data_491C[i], (Vec *)&lbl_1_data_4A0C[i], (Vec *)&lbl_1_data_491C[i]);
        bob = 10.0 * sin(3.141592653589793 * (180.0f + ((180.0f * time) / maxTime)) / 180.0);
        if (lbl_1_data_491C[i].y < 20.0f) {
            lbl_1_data_491C[i].y = 20.0f;
        }
        Hu3DModelPosSet(lbl_1_bss_7A08[i], lbl_1_data_491C[i].x, bob + lbl_1_data_491C[i].y, lbl_1_data_491C[i].z);
    }
    return ret;
}

void fn_1_128E0(M504WORK *work)
{
    s32 i;

    for (i = 0; i < 10; i++) {
        Hu3DModelAttrSet(lbl_1_bss_7A08[i], 0x40000002);
        Hu3DModelAttrSet(lbl_1_bss_7A08[i], 1);
        Hu3DModelKill(lbl_1_bss_7A08[i]);
    }
}
