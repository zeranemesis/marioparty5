#include "REL/m565dll.h"

s32 lbl_1_bss_7890;
f32 lbl_1_bss_7894;
f32 lbl_1_bss_7898;
HuVecF lbl_1_bss_789C;

/* angle wrap on an expression (re-evaluated in every branch, as in the original) */
#define M565_WRAP_F(e) (((e) > 180.0f) ? ((e) - 360.0f) : (((e) <= -180.0f) ? (360.0f + (e)) : (e)))

s32 fn_1_EE4C(M565Game *game)
{
    HuVecF d;
    HuVecF fwd;
    HuVecF axis;
    HuVecF ofs;
    HuVecF zero;
    s16 win[4];
    M565Sys *sys = &game->sys;
    M565Back *back = &sys->back;
    M565Player *player;
    M565Player *pa;
    M565Player *pb;
    f32 angDiff;
    f32 absDiff;
    f32 target;
    f32 len;
    s16 done;
    s32 ret = 0;
    s32 moving;
    s32 count;
    s32 i;
    s32 j;
    s32 tmp;

    switch (game->step) {
        case 0:
            if (sys->stage.fallFlag == 0 && sys->puku.cnt == 0) {
                moving = 0;
                for (i = 0; i < 4; i++) {
                    player = &sys->players[i];
                    if (player->pos->y < -300.0f) {
                        if (player->pos->y > -9499.99f) {
                            moving = 1;
                        }
                    } else {
                        if (player->mg->actor->velY >= 0.0f) {
                            absDiff = player->mg->actor->velY;
                        } else {
                            absDiff = -player->mg->actor->velY;
                        }
                        if (absDiff > 0.0f) {
                            moving = 1;
                        }
                    }
                    MgPlayerVibrateKill(player->mg);
                }
                if (moving == 0) {
                    game->counter = 0;
                    game->step = game->step + 1;
                }
            }
            break;
        case 1:
            count = 0;
            zero.x = 0.0f;
            zero.y = 0.0f;
            zero.z = 0.0f;
            lbl_1_bss_789C = zero;
            for (i = 0; i < 4; i++) {
                player = &sys->players[i];
                fn_1_8984(player, 0);
                Hu3DMotionSet(player->mdl, player->mot[0]);
                Hu3DModelAttrSet(player->mdl, 0x40000001);
                Hu3DModelAttrReset(player->mdl, 1);
                Hu3DModelScaleSet(player->mdl, 1.0f, 1.0f, 1.0f);
                if (player->pos->y > -300.0f && player->fallen == 0) {
                    player->survivor = 1;
                    PSVECAdd(&lbl_1_bss_789C, player->pos, &lbl_1_bss_789C);
                    count++;
                }
                CharModelVoiceVolSet(player->charNo, 127);
            }
            if (count == 0) {
                game->step = 10;
            } else {
                lbl_1_bss_789C.x /= (f32)count;
                lbl_1_bss_789C.y = 150.0f;
                lbl_1_bss_789C.z /= (f32)count;
                lbl_1_bss_7894 = 0.0f;
                for (i = 0; i < 4; i++) {
                    player = &sys->players[i];
                    fwd.x = sin(3.141592653589793 * player->rot->y / 180.0);
                    fwd.y = 0.0f;
                    fwd.z = cos(3.141592653589793 * player->rot->y / 180.0);
                    axis.x = 0.0f;
                    axis.y = 0.0f;
                    axis.z = 1.0f;
                    PSVECNormalize(&axis, &axis);
                    angDiff = 180.0 * ((f32)acos(PSVECDotProduct(&fwd, &axis)) / 3.141592653589793);
                    if (angDiff >= 0.0f) {
                        absDiff = angDiff;
                    } else {
                        absDiff = -angDiff;
                    }
                    if (absDiff > 5.0f && player->survivor != 0) {
                        Hu3DMotionShiftSet(player->mdl, player->mot[1], 0.0f, 4.0f, 0x40000001);
                    }
                    player->facing = M565_WRAP_F(Hu3DData[player->mdl].rot.y);
                    if (player->survivor != 0) {
                        PSVECSubtract(player->pos, &lbl_1_bss_789C, &d);
                        d.y = 0.0f;
                        len = PSVECMag(&d);
                        if (lbl_1_bss_7894 < len) {
                            lbl_1_bss_7894 = len;
                        }
                    }
                }
                Hu3DModelAttrReset(back->mdl5, 1);
                Hu3DModelAttrReset(back->mdl6, 1);
                Hu3DModelAttrReset(back->mdl8, 1);
                ofs.x = 0.0f;
                ofs.y = 0.0f;
                ofs.z = 0.0f;
                sys->camOfs = ofs;
                lbl_1_bss_7898 = CZoom;
                lbl_1_bss_7890 = 0;
                game->counter = 0;
                game->step = game->step + 1;
            }
            break;
        case 2:
            if (lbl_1_bss_7890 == 0) {
                done = 0;
                for (i = 0; i < 4; i++) {
                    player = &sys->players[i];
                    fwd.x = sin(3.141592653589793 * player->facing / 180.0);
                    fwd.y = 0.0f;
                    fwd.z = cos(3.141592653589793 * player->facing / 180.0);
                    axis.x = 0.0f;
                    axis.y = 0.0f;
                    axis.z = 1.0f;
                    PSVECNormalize(&axis, &axis);
                    angDiff = 180.0 * ((f32)acos(PSVECDotProduct(&fwd, &axis)) / 3.141592653589793);
                    target = 180.0 * (atan2(axis.x, axis.z) / 3.141592653589793);
                    if (target > 180.0f) {
                        target = target - 360.0f;
                    } else if (target <= -180.0f) {
                        target = 360.0f + target;
                    }
                    if (angDiff >= 0.0f) {
                        absDiff = angDiff;
                    } else {
                        absDiff = -angDiff;
                    }
                    if (absDiff > 3.0f) {
                        player->facing = fn_1_8F4(player->facing, target, 0.9f);
                    } else {
                        Hu3DMotionShiftSet(player->mdl, player->mot[0], 0.0f, 8.0f, 0x40000001);
                        player->mg->actor->rotY = target;
                        player->facing = target;
                        done++;
                    }
                    Hu3DModelRotSet(player->mdl, 0.0f, player->facing, 0.0f);
                }
                if (done >= 4) {
                    lbl_1_bss_7890 = 1;
                }
            }
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    pa = &sys->players[i];
                    pb = &sys->players[j];
                    if (i != j && pa->survivor != 0 && pb->survivor != 0) {
                        PSVECSubtract(pa->pos, pb->pos, &d);
                        d.y = 0.0f;
                        if (PSVECMag(&d) < 100.0f) {
                            PSVECScale(&d, &d, 0.1f);
                            PSVECAdd(pa->pos, &d, pa->pos);
                            PSVECScale(&d, &d, -1.0f);
                            PSVECAdd(pb->pos, &d, pb->pos);
                        }
                    }
                }
            }
            PSVECSubtract(&lbl_1_bss_789C, &sys->camBase, &d);
            sys->camBase.x = sys->camBase.x + 0.04f * d.x;
            sys->camBase.y = sys->camBase.y + 0.04f * d.y;
            sys->camBase.z = sys->camBase.z + 0.04f * d.z;
            CZoom += 0.04f * ((lbl_1_bss_7898 - (1000.0f - lbl_1_bss_7894)) - CZoom);
            CRot.x += 0.04f * -CRot.x;
            tmp = game->counter;
            game->counter = tmp + 1;
            if (tmp >= 60) {
                Hu3DModelAttrReset(back->mdl7, 1);
                Hu3DMotionShapeTimeSet(back->mdl7, 0.0f);
                Hu3DModelPosSet(back->mdl7, sys->camBase.x, 0.0f, sys->camBase.z - 900.0f);
                game->step = game->step + 1;
            }
            break;
        case 3:
            for (i = 0; i < 4; i++) {
                win[i] = -1;
                player = &sys->players[i];
                if (player->survivor != 0) {
                    win[i] = player->charNo;
                    Hu3DMotionShiftSet(player->mdl, player->mot[5], 0.0f, 8.0f, 0);
                    Hu3DModelAttrReset(player->mdl, 0x40000001);
                    tmp = player->no;
                    if (_CheckFlag(0x1000F) == 0) {
                        GwPlayer[tmp].mgCoinBonus = 10;
                    }
                }
            }
            game->mesId = GameMesCreate(4, 3, win[0], win[1], win[2], win[3]);
            game->counter = 0;
            game->step = game->step + 1;
            break;
        case 4:
            if (game->bgm < 0) {
                game->bgm = fn_1_DF0(0x79);
            }
            tmp = game->counter + 1;
            game->counter = tmp;
            if (tmp >= 209) {
                game->mesId = -1;
                game->counter = 0;
                game->step = 12;
            }
            break;
        case 10:
            game->mesId = GameMesCreate(2, 2);
            game->counter = 0;
            game->step = game->step + 1;
            break;
        case 11:
            if (game->bgm < 0) {
                game->bgm = fn_1_DF0(0x7B);
            }
            if (game->counter == 0) {
                CharLoseVoicePlay(-1, -1, -1, -1);
            }
            tmp = game->counter + 1;
            game->counter = tmp;
            if (tmp >= 209) {
                game->mesId = -1;
                game->counter = 0;
                game->step = game->step + 1;
            }
            break;
        case 12:
            game->counter = 0;
            ret = 1;
            break;
    }
    return ret;
}
