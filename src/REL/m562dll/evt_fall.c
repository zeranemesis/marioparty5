#include "REL/m562dll.h"
#include "game/gamework.h"
#include "game/wipe.h"
#include "game/frand.h"
#include "game/window.h"
#include <math.h>

/* data */
static f32 lbl_1_data_D98[3] = { 0.0f, 1000.0f, 1400.0f };
static f32 lbl_1_data_DA4[3] = { 0.0f, 100.0f, -200.0f };

/* bss */
static M562StageTile *lbl_1_bss_7960;
static s16 lbl_1_bss_7964[20];
static s16 lbl_1_bss_798C;

/* Final phase of the battle: Bowser's stage collapses tile by tile (random
   order), Bowser falls out of view and the wipe ends the scene. */
s32 fn_1_27950(M562Work *w)
{
    M562Player *player = &w->player;
    M562Back *back = &w->back;
    CAMERA *camera = (CAMERA *)&w->camera[0];
    Koopa *koopa = (Koopa *)&w->koopa;
    M562StageField *field = &w->stage;
    s32 done = 0;
    f32 t;
    f32 clamped;
    s16 i;
    s16 a;
    s16 b;
    s16 tmp;
    M562StageTile *cell;

    switch (w->state) {
    case 0:
        back->reflectOn = 0;
        back->rippleOn = 0;
        if (GwSystem.subGameNo >= 3) {
            w->evtTimer = 0;
            w->state = 4;
            break;
        }
        lbl_1_bss_7960 = &field->tile[(s16)(0.99f * (125.0f + (player->pos.x - -500.0f)) / 250.0f)
            + (s16)(0.99f * (125.0f + (player->pos.z - -375.0f)) / 250.0f) * 5];
        for (i = 0; i < 20; i++) {
            lbl_1_bss_7964[i] = i;
        }
        for (i = 0; i < 20; i++) {
            a = frand() % 20;
            b = frand() % 20;
            tmp = lbl_1_bss_7964[a];
            lbl_1_bss_7964[a] = lbl_1_bss_7964[b];
            lbl_1_bss_7964[b] = tmp;
        }
        camera->mode = 1;
        camera->posGoal.x = lbl_1_data_D98[0];
        camera->posGoal.y = lbl_1_data_D98[1];
        camera->posGoal.z = lbl_1_data_D98[2];
        camera->tgtGoal.x = lbl_1_data_DA4[0];
        camera->tgtGoal.y = lbl_1_data_DA4[1];
        camera->tgtGoal.z = lbl_1_data_DA4[2];
        camera->prevMode = 0;
        camera->speed = 0.04f;
        camera->slerpT = 0.04f;
        player->state = 0;
        player->reqMotion = 0x1A;
        player->loopMotion = 1;
        Hu3DMotionSpeedSet(player->modelId, 3.0f);
        lbl_1_bss_798C = 0;
        w->evtTimer = 0;
        w->state++;
        /* fallthrough */
    case 1:
        if (w->seId < 0) {
            w->seId = fn_1_814(0x8B5);
        }
        t = (f32)w->evtTimer / 120.0f;
        if (t > 1.0f) {
            clamped = 1.0f;
        } else if (t < 0.0f) {
            clamped = 0.0f;
        } else {
            clamped = t;
        }
        Hu3DMotionSpeedSet(back->model40, 0.25 + 2.0 * sin(M_PI * (f64)(180.0f * clamped) / 180.0));
        Hu3DMotionSpeedSet(player->modelId, 3.0f);
        w->evtTimer++;
        if (w->evtTimer % 10 == 0) {
            fn_1_21B68(camera, 1, 0x32, 10.0f, 0.95f, 90.0f);
            omVibrate(player->playerNo, 0x14, 0xC, 0);
        }
        if (w->evtTimer % 5 == 0) {
            if (lbl_1_bss_798C < 20) {
                cell = &field->tile[lbl_1_bss_7964[lbl_1_bss_798C]];
                if (cell != lbl_1_bss_7960 && cell != (M562StageTile *)koopa->cell) {
                    fn_1_814(0x8C3);
                    cell->state = 0xA;
                }
                lbl_1_bss_798C++;
            } else {
                Hu3DMotionSpeedSet(player->modelId, 1.0f);
                w->evtTimer = 0;
                w->state++;
            }
        }
        break;
    case 2:
        fn_1_814(0x8C3);
        lbl_1_bss_7960->state = 0xA;
        player->vy = 0.0f;
        player->state = 0;
        player->reqMotion = 0x17;
        player->loopMotion = 0;
        Hu3DMotionSpeedSet(player->modelId, 1.0f);
        w->evtTimer = 0;
        w->state++;
        break;
    case 3:
        w->evtTimer++;
        if (w->evtTimer == 0x1C) {
            player->reqMotion = 0;
            player->loopMotion = 0;
        }
        if (w->evtTimer >= 0x1E) {
            player->vy -= 1.5f;
            player->pos.y += player->vy;
            if (player->pos.y < -2000.0f) {
                w->evtTimer = 0;
                w->state++;
            }
        }
        break;
    case 4:
        w->evtTimer++;
        if (w->evtTimer >= 0x1E) {
            WipeCreate(2, 0, 0x3C);
            WipeColorSet(0, 0, 0);
            player->sub = 0;
            player->state = 0;
            w->evtTimer = 0;
            w->state++;
        }
        break;
    case 5:
        w->evtTimer++;
        if (WipeCheckEnd() == 0) {
            w->evtTimer = 0;
            w->state = 0;
            done = 1;
        }
        break;
    }
    return done;
}
