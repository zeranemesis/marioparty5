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
static M562EvtCell *lbl_1_bss_7960;
static s16 lbl_1_bss_7964[20];
static s16 lbl_1_bss_798C;

/* Final phase of the battle: Bowser's stage collapses tile by tile (random
   order), Bowser falls out of view and the wipe ends the scene. */
s32 fn_1_27950(M562EvtWork *w)
{
    M562EvtPlayer *player = &w->player;
    M562EvtStage *stage = &w->stage;
    M562EvtCamera *camera = &w->camera;
    M562EvtCells *cells = &w->cells;
    s32 done = 0;
    f32 t;
    f32 clamped;
    s16 i;
    s16 a;
    s16 b;
    s16 tmp;
    M562EvtCell *cell;

    switch (w->state) {
    case 0:
        stage->unk24 = 0;
        stage->unk28 = 0;
        if (GwSystem.subGameNo >= 3) {
            w->timer = 0;
            w->state = 4;
            break;
        }
        lbl_1_bss_7960 = &cells->cell[(s16)(0.99f * (125.0f + (player->pos.x - -500.0f)) / 250.0f)
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
        camera->unkA = 1;
        camera->pos.x = lbl_1_data_D98[0];
        camera->pos.y = lbl_1_data_D98[1];
        camera->pos.z = lbl_1_data_D98[2];
        camera->target.x = lbl_1_data_DA4[0];
        camera->target.y = lbl_1_data_DA4[1];
        camera->target.z = lbl_1_data_DA4[2];
        camera->unkC = 0;
        camera->posSpeed = 0.04f;
        camera->targetSpeed = 0.04f;
        player->unk4 = 0;
        player->animNo = 0x1A;
        player->animLoop = 1;
        Hu3DMotionSpeedSet(player->motion, 3.0f);
        lbl_1_bss_798C = 0;
        w->timer = 0;
        w->state++;
        /* fallthrough */
    case 1:
        if (w->sndB < 0) {
            w->sndB = fn_1_814(0x8B5);
        }
        t = (f32)w->timer / 120.0f;
        if (t > 1.0f) {
            clamped = 1.0f;
        } else if (t < 0.0f) {
            clamped = 0.0f;
        } else {
            clamped = t;
        }
        Hu3DMotionSpeedSet(stage->motion40, 0.25 + 2.0 * sin(M_PI * (f64)(180.0f * clamped) / 180.0));
        Hu3DMotionSpeedSet(player->motion, 3.0f);
        w->timer++;
        if (w->timer % 10 == 0) {
            fn_1_21B68(camera, 1, 0x32, 10.0f, 0.95f, 90.0f);
            omVibrate(player->charIdx, 0x14, 0xC, 0);
        }
        if (w->timer % 5 == 0) {
            if (lbl_1_bss_798C < 20) {
                cell = &cells->cell[lbl_1_bss_7964[lbl_1_bss_798C]];
                if (cell != lbl_1_bss_7960 && cell != w->boss.curCell) {
                    fn_1_814(0x8C3);
                    cell->state = 0xA;
                }
                lbl_1_bss_798C++;
            } else {
                Hu3DMotionSpeedSet(player->motion, 1.0f);
                w->timer = 0;
                w->state++;
            }
        }
        break;
    case 2:
        fn_1_814(0x8C3);
        lbl_1_bss_7960->state = 0xA;
        player->velY = 0.0f;
        player->unk4 = 0;
        player->animNo = 0x17;
        player->animLoop = 0;
        Hu3DMotionSpeedSet(player->motion, 1.0f);
        w->timer = 0;
        w->state++;
        break;
    case 3:
        w->timer++;
        if (w->timer == 0x1C) {
            player->animNo = 0;
            player->animLoop = 0;
        }
        if (w->timer >= 0x1E) {
            player->velY -= 1.5f;
            player->pos.y += player->velY;
            if (player->pos.y < -2000.0f) {
                w->timer = 0;
                w->state++;
            }
        }
        break;
    case 4:
        w->timer++;
        if (w->timer >= 0x1E) {
            WipeCreate(2, 0, 0x3C);
            WipeColorSet(0, 0, 0);
            player->unk6 = 0;
            player->unk4 = 0;
            w->timer = 0;
            w->state++;
        }
        break;
    case 5:
        w->timer++;
        if (WipeCheckEnd() == 0) {
            w->timer = 0;
            w->state = 0;
            done = 1;
        }
        break;
    }
    return done;
}
