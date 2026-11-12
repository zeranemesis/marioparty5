#include "REL/m565dll.h"

s32 fn_1_E740(M565Game *game)
{
    HuVecF fwd;
    HuVecF toCenter;
    M565Sys *sys = &game->sys;
    M565Player *player;
    M565PukuItem *item;
    f32 maxTime;
    f32 angDiff;
    f32 target;
    f32 absDiff;
    s16 done;
    s32 ret = 0;
    s32 i;

    switch (game->step) {
        case 0:
            for (i = 0; i < 4; i++) {
                player = &sys->players[i];
                fn_1_8984(player, 0);
                Hu3DModelRotSet(player->mdl, 0.0f, 0.0f, 0.0f);
                Hu3DModelPosSet(player->mdl, player->pos->x, 0.0f, player->pos->z);
                Hu3DMotionSet(player->mdl, player->mot[0]);
                Hu3DModelAttrSet(player->mdl, 0x40000001);
            }
            game->step = game->step + 1;
            break;
        case 1:
            if (WipeCheckEnd() == 0) {
                game->step = game->step + 1;
            }
            break;
        case 2:
            item = fn_1_3284(sys, 1, 72.0f, 0.6f);
            if (item != NULL) {
                item->t30 = 10;
            }
            item = fn_1_3284(sys, 1, 144.0f, 0.6f);
            if (item != NULL) {
                item->t30 = 10;
            }
            item = fn_1_3284(sys, 1, 216.0f, 0.6f);
            if (item != NULL) {
                item->t30 = 10;
            }
            item = fn_1_3284(sys, 1, 288.0f, 0.6f);
            if (item != NULL) {
                item->t30 = 10;
            }
            item = fn_1_3284(sys, 1, 360.0f, 0.6f);
            if (item != NULL) {
                item->t30 = 10;
            }
            fn_1_C18(0x8DE);
            game->step = game->step + 1;
            break;
        case 3:
            if (sys->flag3824 == 0) {
                fn_1_C18(0x8DF);
                omVibrate(0, 30, 12, 0);
                omVibrate(1, 30, 12, 0);
                omVibrate(2, 30, 12, 0);
                omVibrate(3, 30, 12, 0);
                for (i = 0; i < 4; i++) {
                    player = &sys->players[i];
                    Hu3DMotionSet(player->mdl, player->mot[3]);
                    Hu3DModelAttrReset(player->mdl, 0x40000001);
                }
                fn_1_366C(sys, 60, 25.0f, 0.95f, 90.0f);
                game->step = game->step + 1;
            }
            break;
        case 4:
            player = &sys->players[0];
            maxTime = Hu3DMotionMaxTimeGet(player->mdl);
            if (!(Hu3DMotionTimeGet(player->mdl) < maxTime)) {
                game->counter = 0;
                game->step = game->step + 1;
            }
            break;
        case 5:
            i = game->counter;
            game->counter = i + 1;
            if (i >= 60) {
                for (i = 0; i < 4; i++) {
                    player = &sys->players[i];
                    Hu3DMotionShiftSet(player->mdl, player->mot[1], 0.0f, 4.0f, 0x40000001);
                }
                game->step = game->step + 1;
            }
            break;
        case 6:
            done = 0;
            for (i = 0; i < 4; i++) {
                player = &sys->players[i];
                fwd.x = sin(3.141592653589793 * player->facing / 180.0);
                fwd.y = 0.0f;
                fwd.z = cos(3.141592653589793 * player->facing / 180.0);
                toCenter.x = -player->pos->x;
                toCenter.y = 0.0f;
                toCenter.z = -player->pos->z;
                PSVECNormalize(&toCenter, &toCenter);
                angDiff = 180.0 * ((f32)acos(PSVECDotProduct(&fwd, &toCenter)) / 3.141592653589793);
                target = 180.0 * (atan2(toCenter.x, toCenter.z) / 3.141592653589793);
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
                    Hu3DMotionShiftSet(player->mdl, player->mot[0], 0.0f, 4.0f, 0x40000001);
                    player->mg->actor->rotY = target;
                    player->facing = target;
                    done++;
                }
                Hu3DModelRotSet(player->mdl, 0.0f, player->facing, 0.0f);
            }
            if (done >= 4) {
                game->step = game->step + 1;
            }
            break;
        case 7:
            if (sys->puku.cnt == 0 && sys->flag3828 == 0) {
                game->step = game->step + 1;
            }
            break;
        case 8:
            game->step = 0;
            ret = 1;
            break;
    }
    return ret;
}
