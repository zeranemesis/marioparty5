#include "REL/m502dll.h"

s32 fn_1_DE94(Vec *v);
void fn_1_CB10(M502BATITEM *it, u16 idx, M502BAT *bat);
void fn_1_CE54(M502BATITEM *it);
void fn_1_CEBC(M502BATITEM *it);

static s16 lbl_1_bss_5090;

void fn_1_C468(M502BAT *bat, M502PLAYER *player)
{
    s32 i;

    memset(bat, 0, sizeof(M502BAT));
    bat->player = player;
    for (i = 0; i < 8; i++) {
        fn_1_CB10(&bat->item[i], (u16)i, bat);
    }
    lbl_1_bss_5090 = 0;
}

void fn_1_C4F0(M502BAT *bat)
{
    s32 i;

    for (i = 0; i < 8; i++) {
        fn_1_CE54(&bat->item[i]);
    }
    memset(bat, 0, sizeof(M502BAT));
}

/* spawn the bats when the first player drops below the ceiling, then update them all */
void fn_1_C554(M502BAT *bat)
{
    Vec v;
    Vec zero = { 0.0f, 0.0f, 0.0f };
    Vec zero2 = { 0.0f, 0.0f, 0.0f };
    M502PLRSUB *plr;
    M502BATITEM *it;
    f32 spread = 0.0f;
    s32 i;

    plr = &bat->player->plr;
    switch (bat->phase) {
    case 0:
        fn_1_6CFC(0, 4, 1, &v);
        if (plr->obj->trans.y < v.y) {
            for (i = 0; i < 8; i++) {
                it = &bat->item[i];
                if ((i % 4) == 0) {
                    spread = -300.0f + 600.0f * ((f32)(rand8() % 256) / 255.0f);
                }
                if (i < 5) {
                    f32 jitter;

                    fn_1_6CFC(0, 8, 1, &v);
                    jitter = 20.0f * ((f32)(rand8() % 256) / 255.0f);
                    v.x = (spread + 30.0f * (f32)(i % 4) + jitter) - 30.0f;
                    v.y -= 305.0f;
                    v.z = -240.0f;
                    it->pos = v;
                    it->rot = zero;
                    it->state = 0;
                    it->offY = 0.0f;
                    Hu3DMotionSet(it->model, it->mot0);
                    Hu3DModelAttrReset(it->model, 0x40000001);
                } else {
                    it->state = 3;
                }
            }
            bat->phase++;
        }
        break;
    case 1:
        fn_1_6CFC(1, 4, 1, &v);
        if (plr->obj->trans.y < v.y) {
            for (i = 0; i < 8; i++) {
                f32 jitter;

                it = &bat->item[i];
                if ((i % 4) == 0) {
                    spread = -300.0f + 600.0f * ((f32)(rand8() % 256) / 255.0f);
                }
                fn_1_6CFC(1, 8, 1, &v);
                jitter = 20.0f * ((f32)(rand8() % 256) / 255.0f);
                v.x = (spread + 30.0f * (f32)(i % 4) + jitter) - 30.0f;
                v.y -= 305.0f;
                v.z = -240.0f;
                it->pos = v;
                it->rot = zero2;
                it->state = 0;
                it->offY = 0.0f;
                Hu3DMotionSet(it->model, it->mot0);
                Hu3DModelAttrReset(it->model, 0x40000001);
            }
            bat->phase++;
        }
        break;
    }
    for (i = 0; i < 8; i++) {
        fn_1_CEBC(&bat->item[i]);
    }
}

void fn_1_CAC8(M502BAT *bat)
{
    s32 i;

    for (i = 0; i < 8; i++) {
        bat->item[i].visible = 0;
        bat->item[i].state = 3;
    }
}

void fn_1_CB10(M502BATITEM *it, u16 idx, M502BAT *bat)
{
    Vec v;
    Vec zero = { 0.0f, 0.0f, 0.0f };
    Vec rot = { 0.0f, 0.0f, 0.0f };
    s16 model;

    memset(it, 0, sizeof(M502BATITEM));
    it->bat = bat;
    model = Hu3DModelCreateData(0x3C0085);
    it->model = model;
    it->mot0 = Hu3DJointMotionData(model, 0x3C0086);
    it->mot1 = Hu3DJointMotionData(model, 0x3C0087);
    it->mot2 = Hu3DJointMotionData(model, 0x3C0088);
    Hu3DModelCameraSet(model, (u16)lbl_1_data_B60[bat->player->no]);
    Hu3DModelLayerSet(model, 0);
    Hu3DModelAttrSet(model, 1);
    it->visible = 0;
    Hu3DModelScaleSet(model, 1.3f, 1.3f, 1.3f);
    v = zero;
    if (idx < 8) {
        fn_1_6CFC(0, 8, 1, &v);
        v.x = -390.0f + 780.0f * ((f32)(rand8() % 256) / 255.0f);
    } else {
        fn_1_6CFC(1, 8, 1, &v);
        v.x = -390.0f + 780.0f * ((f32)(rand8() % 256) / 255.0f);
    }
    v.y -= 300.0f;
    v.z = -300.0f;
    it->pos = v;
    it->rot = rot;
    Hu3DModelPosSet(model, it->pos.x, it->pos.y, it->pos.z);
    Hu3DModelRotSet(model, it->rot.x, it->rot.y, it->rot.z);
}

void fn_1_CE54(M502BATITEM *it)
{
    Hu3DModelAttrSet(it->model, 0x40000002);
    Hu3DMotionKill(it->mot2);
    Hu3DMotionKill(it->mot1);
    Hu3DMotionKill(it->mot0);
    Hu3DModelKill(it->model);
    memset(it, 0, sizeof(M502BATITEM));
}

void fn_1_CEBC(M502BATITEM *it)
{
    Vec out1;
    Vec out2;
    Vec a;
    Vec b;
    Vec p;
    Vec zero = { 0.0f, 0.0f, 0.0f };
    M502PLAYER *player;
    M502PLRSUB *plr;
    M502BLOCK *block;
    s16 lvl;
    s16 row;
    s16 col;
    s32 vis;
    s32 pan;
    s32 first;
    f32 c;
    f32 c2;
    s32 ang;

    player = it->bat->player;
    plr = &player->plr;
    block = &player->block;
    out1 = zero;
    out2 = zero;
    if (it->state >= 3) {
        vis = 0;
    } else {
        a = it->pos;
        b = it->pos;
        b.y += 300.0f;
        a.y -= 300.0f;
        Hu3D3Dto2D(&b, player->camera.camBit1, &out2);
        Hu3D3Dto2D(&a, player->camera.camBit1, &out1);
        first = 1;
        if (fn_1_DE94(&out2) == 0 && fn_1_DE94(&out1) == 0) {
            first = 0;
        }
        vis = first;
    }
    if (it->visible != 0 && vis == 0) {
        Hu3DModelAttrSet(it->model, 1);
        it->visible = 0;
    } else if (it->visible == 0 && vis != 0) {
        Hu3DModelAttrReset(it->model, 1);
        it->visible = 1;
    }
    switch (it->state) {
    case 0:
        p = it->pos;
        p.x += 150.0f;
        p.y += 300.0f;
        fn_1_6E1C(&p, &lvl, &row, &col);
#ifdef TARGET_PC
        if (lvl < 3 && block->cell[lvl][col + row * 3].kind == 0) {
#else
        if (lvl < 6 && block->cell[lvl][col + row * 3].kind == 0) {
#endif
            it->state++;
        } else if (plr->obj->trans.y < it->pos.y && plr->obj->trans.x > (it->pos.x - 150.0f) && plr->obj->trans.x < (150.0f + it->pos.x)) {
            it->state++;
        }
        break;
    case 1:
        if (lbl_1_bss_5090++ < 4) {
            if (plr->playerNo == 0) {
                pan = 0x28;
            } else if (plr->playerNo == 1) {
                pan = 0x38;
            } else if (plr->playerNo == 2) {
                pan = 0x48;
            } else {
                pan = 0x58;
            }
            fn_1_218(fn_1_198(0x62F), pan);
        }
        if ((rand8() % 100) < 50) {
            it->dir = 1;
        } else {
            it->dir = 0;
        }
        it->power = (f32)(rand8() % 100);
        it->t = 0;
        it->t2 = (rand8() % 180) + 180;
        it->k = ((rand8() % 100) / 10) + 10;
        it->state++;
        break;
    case 2:
        if (it->t == 3) {
            Hu3DMotionSet(it->model, it->mot1);
            Hu3DMotionSpeedSet(it->model, 10.0f);
            Hu3DModelAttrReset(it->model, 0x40000001);
        } else if (it->t == 8) {
            Hu3DMotionSet(it->model, it->mot2);
            Hu3DMotionSpeedSet(it->model, 6.0f);
            Hu3DModelAttrSet(it->model, 0x40000001);
        }
        if (it->t >= 12) {
            c = (f32)(it->t - 16) / 12.0f;
            if (c > 1.0f) {
                c = 1.0f;
            } else if (c < 0.0f) {
                c = 0.0f;
            }
            if (it->t < 24) {
                it->pos.y += (1.0f - c) * (5.0f + 0.05f * it->power);
            }
            if (it->dir != 0) {
                it->pos.x -= ((100.0f + it->power) / 10.0f) * (0.5f + 0.5f * c);
                it->pos.z -= 0.05f * it->power * (0.5f + 0.5f * c);
            } else {
                it->pos.x += ((100.0f + it->power) / 10.0f) * (0.5f + 0.5f * c);
                it->pos.z -= 0.05f * it->power * (0.5f + 0.5f * c);
            }
        } else {
            c2 = (f32)it->t / 12.0f;
            if (c2 > 1.0f) {
                c2 = 1.0f;
            } else if (c2 < 0.0f) {
                c2 = 0.0f;
            }
            c2 = 1.0f - c2;
            it->pos.y -= c2 * (10.0f + 0.3f * it->power);
            if (it->dir != 0) {
                it->pos.x -= ((100.0f + it->power) / 10.0f) * (0.1f + 0.1f * c2);
            } else {
                it->pos.x += ((100.0f + it->power) / 10.0f) * (0.1f + 0.1f * c2);
            }
        }
        if (it->dir != 0) {
            if ((it->rot.z - (3.0f + 0.03f * it->power)) > 180.0f) {
                it->rot.z = 180.0f;
            } else if ((it->rot.z - (3.0f + 0.03f * it->power)) < (75.0f - 0.2f * it->power)) {
                it->rot.z = 75.0f - 0.2f * it->power;
            } else {
                it->rot.z = it->rot.z - (3.0f + 0.03f * it->power);
            }
            if ((it->rot.y - (2.0f + 0.03f * it->power)) > 0.0f) {
                it->rot.y = 0.0f;
            } else if ((it->rot.y - (2.0f + 0.03f * it->power)) < -45.0f) {
                it->rot.y = -45.0f;
            } else {
                it->rot.y = it->rot.y - (2.0f + 0.03f * it->power);
            }
        } else {
            if ((it->rot.z + (3.0f + 0.03f * it->power)) > (285.0f + 0.2f * it->power)) {
                it->rot.z = 285.0f + 0.2f * it->power;
            } else if ((it->rot.z + (3.0f + 0.03f * it->power)) < 180.0f) {
                it->rot.z = 180.0f;
            } else {
                it->rot.z = it->rot.z + (3.0f + 0.03f * it->power);
            }
            if ((it->rot.y + (2.0f + 0.03f * it->power)) > 45.0f) {
                it->rot.y = 45.0f;
            } else if ((it->rot.y + (2.0f + 0.03f * it->power)) < 0.0f) {
                it->rot.y = 0.0f;
            } else {
                it->rot.y = it->rot.y + (2.0f + 0.03f * it->power);
            }
        }
        if (((f32)it->t2 / 180.0f) > 1.0f) {
            c = 1.0f;
        } else if (((f32)it->t2 / 180.0f) < 0.0f) {
            c = 0.0f;
        } else {
            c = (f32)it->t2 / 180.0f;
        }
        ang = ((it->t2 * it->k) % 360) + 180;
        it->offY = (f32)((f64)it->offY + (f64)(12.0f + 18.0f * (1.0f - c)) * HuSin((f32)ang));
        if (it->pos.x < -750.0f || it->pos.x > 750.0f) {
            if (lbl_1_bss_5090 != 0) {
                lbl_1_bss_5090--;
            }
            it->state++;
        }
        it->t++;
        it->t2++;
        break;
    }
    Hu3DModelPosSet(it->model, it->pos.x + it->offX, it->pos.y + it->offY, it->pos.z + it->offZ);
    Hu3DModelRotSet(it->model, it->rot.x, it->rot.y, it->rot.z);
}

s32 fn_1_DE94(Vec *v)
{
    if (v->y < 0.0f || v->y >= 480.0f) {
        return 0;
    }
    return 1;
}
