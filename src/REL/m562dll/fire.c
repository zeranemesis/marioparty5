#include "REL/m562dll.h"
#include "game/frand.h"
#include "game/hsfex.h"
#include "datadir_enum.h"
#include "math.h"
#include "string.h"

/* Not declared in game/hu3d.h on purpose (see the note there): the float return is used here. */
extern float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

/* 180/-180 wrap, re-evaluating the expression in every branch like the original */
#define FIRE_WRAP_D(e) (((f32)(e) > 180.0f) ? ((e) - 360.0) : (((f32)(e) <= -180.0f) ? (360.0 + (e)) : (e)))
#define FIRE_WRAP_F(e) (((e) > 180.0f) ? ((e) - 360.0f) : (((e) <= -180.0f) ? (360.0f + (e)) : (e)))
#define FIRE_ABS(x) (((x) >= 0.0f) ? (x) : -(x))
#define FIRE_RAD(a) (3.141592653589793 * (a) / 180.0)

u32 lbl_1_data_998[8] = {
    DATANUM(DATA_m562, 0x61), DATANUM(DATA_m562, 0x62), DATANUM(DATA_m562, 0x63), DATANUM(DATA_m562, 0x64),
    DATANUM(DATA_m562, 0x65), DATANUM(DATA_m562, 0x66), DATANUM(DATA_m562, 0x67), DATANUM(DATA_m562, 0x68),
};

f32 lbl_1_data_9B8[6] = { 0.0f, 7000.0f, 0.0f, 0.0f, -1.0f, 0.02f };

GXColor lbl_1_data_9D0 = { 0xA0, 0x10, 0x00, 0xFF };

void fn_1_13CFC(FireMgr *mgr, M562Work *owner) {
    s32 i;
    s16 count;
    HU3DMODELID model;

    memset(mgr, 0, sizeof(FireMgr));
    mgr->owner = owner;
    mgr->run = 0;
    mgr->enable = 0;
    mgr->anim = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m562, 0x6C), HU_MEMNUM_OVL, HEAP_MODEL));
    for (i = 0; i < FIRE_SLOT_MAX; i++) {
        fn_1_153A4(&mgr->slot[i], mgr);
    }
    count = 20; /* the original leaves this unset for an out-of-range difficulty */
    switch (owner->diff) {
    case 0:
        count = 35;
        break;
    case 1:
        count = 30;
        break;
    case 2:
        count = 25;
        break;
    case 3:
        count = 20;
        break;
    }
    mgr->count = count;
    model = Hu3DModelCreateData(DATANUM(DATA_m562, 0x14));
    mgr->deathModel = model;
    Hu3DModelAttrSet(model, 1);
    mgr->deathMotion = Hu3DJointMotionData(model, DATANUM(DATA_m562, 0x69));
    Hu3DMotionSet(model, mgr->deathMotion);
    Hu3DModelAttrReset(model, 0x40000001);
    for (i = 0; i < FIRE_MAX; i++) {
        Fire *fire = &mgr->fire[i];
        f32 len;

        fn_1_15D40(fire, i, mgr);
        if (i >= count) {
            fire->state = 7;
        } else {
            do {
                fire->pos.y = 0.0f;
                fire->pos.x = -625.0f + 1250.0f * ((f32)(frand() % 10000) / 10000.0f);
                fire->pos.z = -500.0f + 1000.0f * ((f32)(frand() % 10000) / 10000.0f);
                len = sqrtf(fire->pos.x * fire->pos.x + fire->pos.z * fire->pos.z);
            } while (!(len > 200.0f));
        }
        if (i < 5) {
            fire->aggressive = 1;
        }
    }
    fn_1_2CD6C(FIRE_FX(owner), 0x24);
    fn_1_2CE3C(FIRE_FX(owner), fn_1_14390);
}

void fn_1_140E4(FireMgr *mgr) {
    s32 i;

    fn_1_2CDE8(FIRE_FX(mgr->owner));
    for (i = 0; i < FIRE_MAX; i++) {
        fn_1_16218(&mgr->fire[i]);
    }
    Hu3DMotionKill(mgr->deathMotion);
    Hu3DModelKill(mgr->deathModel);
    for (i = 0; i < FIRE_SLOT_MAX; i++) {
        fn_1_154FC(&mgr->slot[i]);
    }
    memset(mgr, 0, sizeof(FireMgr));
}

void fn_1_14190(FireMgr *mgr) {
    M562Work *owner = mgr->owner;
    FireBoss *boss = FIRE_BOSS(owner);
    s32 i;
    Vec pos;
    f32 radius;

    mgr->count = 0;
    for (i = 0; i < FIRE_MAX; i++) {
        Fire *fire = &mgr->fire[i];

        fire->run = mgr->run;
        fn_1_16350(fire);
        if (fire->state < 6) {
            mgr->count++;
        }
        if (fire->shown != 0 && boss->held != fire && fire->state != 4 && fire->state <= 5
            && (fire->state != 5 || !(fire->vy >= 0.0f))) {
            pos = fire->pos;
            pos.y += 60.0f;
            radius = 60.0f;
            if (fire->state == 5) {
                radius *= 1.3f;
            }
            fn_1_2CE8C(FIRE_FX(owner), 1, 0, &pos, (void *)fire, radius);
        }
    }
    if (mgr->run != 0) {
        radius = 0.5f * boss->height;
        pos = boss->pos;
        pos.y += radius;
        fn_1_2CE8C(FIRE_FX(owner), 0, 0, &pos, (void *)boss, radius);
        fn_1_2CE44(FIRE_FX(owner));
    }
    for (i = 0; i < FIRE_SLOT_MAX; i++) {
        mgr->slot[i].run = mgr->run;
        fn_1_15554(&mgr->slot[i]);
    }
}

void fn_1_14390(EffectEntry *a, EffectEntry *b) {
    if (a->type == 1 && b->type == 1) {
        fn_1_14438((Fire *)a->user, (Fire *)b->user);
    } else if (a->type == 0 && b->type == 1) {
        fn_1_14B68((FireBoss *)a->user, (Fire *)b->user);
    } else if (a->type == 1 && b->type == 0) {
        fn_1_14B68((FireBoss *)b->user, (Fire *)a->user);
    }
}

void fn_1_14438(Fire *a, Fire *b) {
    Vec dir;
    f32 a0, a1, b0, b1;
    f32 mag;

    a0 = a->pos.y;
    a1 = 120.0f + a0;
    b0 = b->pos.y;
    b1 = 120.0f + b0;
    if (a->state != 4 && a->phase != 4 && a->state <= 5 && b->state <= 5) {
        if (a->state == 5) {
            fn_1_162A0(b);
        }
        if (b->state == 5) {
            fn_1_162A0(a);
        }
        if ((a0 >= b0 && a1 <= b1) || (a0 <= b0 && a1 >= b1) || (a1 >= b0 && a1 <= b1)) {
            f32 mid0, mid1, d0, d1, big, small, dd, w;

            dir.x = a->pos.x - b->pos.x;
            dir.y = 0.0f;
            dir.z = a->pos.z - b->pos.z;
            mag = PSVECMag(&dir);
            if (mag < 100.0f) {
                if (dir.x != 0.0f || dir.z != 0.0f) {
                    PSVECNormalize(&dir, &dir);
                } else {
                    dir.x = 0.0f;
                    dir.z = 1.0f;
                }
                a->targetAngle = FIRE_WRAP_D(180.0 * (atan2(dir.x, dir.z) / 3.141592653589793));
                b->targetAngle = FIRE_WRAP_D(180.0 * (atan2(-dir.x, -dir.z) / 3.141592653589793));
                PSVECScale(&dir, &dir, 100.0f - mag);
                mid0 = 0.5f * (a0 + a1);
                mid1 = 0.5f * (b0 + b1);
                d0 = FIRE_ABS(mid0 - a0);
                d1 = FIRE_ABS(mid1 - b0);
                big = (d0 > d1) ? d0 : d1;
                small = (d0 > d1) ? d1 : d0;
                dd = FIRE_ABS(mid0 - mid1);
                if (dd < big) {
                    w = 1.0f;
                } else {
                    w = 1.0f - (dd - big) / small;
                }
                a->pos.x += 0.5f * dir.x * w;
                a->pos.z += 0.5f * dir.z * w;
                b->pos.x -= 0.5f * dir.x * w;
                b->pos.z -= 0.5f * dir.z * w;
            }
        }
    }
}

void fn_1_14B68(FireBoss *boss, Fire *fire) {
    Vec dir;
    f32 b0, b1, f0, f1;
    f32 mag;

    b0 = boss->pos.y;
    b1 = b0 + boss->height;
    f0 = fire->pos.y;
    f1 = 120.0f + f0;
    if (fire->state <= 3
        && ((b1 == f1 && b0 == f0) || (b1 <= f1 && b1 >= f0) || (b0 <= f1 && b0 >= f0) || (f1 <= b1 && f1 >= b0)
            || (f0 <= b1 && f0 >= b0))) {
        f32 mid0, mid1, d0, d1, big, small, dd, w;

        dir.x = boss->pos.x - fire->pos.x;
        dir.y = 0.0f;
        dir.z = boss->pos.z - fire->pos.z;
        mag = PSVECMag(&dir);
        if (mag < 110.0f) {
            if (dir.x != 0.0f || dir.z != 0.0f) {
                PSVECNormalize(&dir, &dir);
            } else {
                dir.x = 0.0f;
                dir.z = 1.0f;
            }
            fire->targetAngle = FIRE_WRAP_D(180.0 * (atan2(-dir.x, -dir.z) / 3.141592653589793));
            PSVECScale(&dir, &dir, 110.0f - mag);
            if (fire->flameStage >= 3 && (boss->state == 1 || boss->state == 2) && boss->f10 == 0) {
                boss->fA = 0;
                boss->f6 = 0;
                boss->f8 = 0;
                boss->state = 4;
            }
            mid0 = 0.5f * (b0 + b1);
            mid1 = 0.5f * (f0 + f1);
            d0 = FIRE_ABS(mid0 - b0);
            d1 = FIRE_ABS(mid1 - f0);
            big = (d0 > d1) ? d0 : d1;
            small = (d0 > d1) ? d1 : d0;
            dd = FIRE_ABS(mid0 - mid1);
            if (dd < big) {
                w = 1.0f;
            } else {
                w = 1.0f - (dd - big) / small;
            }
            boss->pos.x += 0.5f * dir.x * w;
            boss->pos.z += 0.5f * dir.z * w;
            fire->pos.x -= 0.5f * dir.x * w;
            fire->pos.z -= 0.5f * dir.z * w;
            if (b0 > mid1 && mag < 88.0f) {
                if ((boss->state == 1 || boss->state == 2) && boss->fC4 < 0.0f) {
                    boss->fC4 = sqrtf(600.0f);
                    fn_1_AD68(boss);
                    omVibrate(boss->player, 10, 4, 2);
                }
                fn_1_162A0(fire);
            }
        }
    }
}

void fn_1_152C8(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx) {
    DCStoreRangeNoSync(particle->data, particle->maxCnt * sizeof(HU3DPARTICLEDATA));
}

FireSlot *fn_1_15308(FireMgr *mgr, Fire *fire) {
    s32 i;

    for (i = 0; i < FIRE_SLOT_MAX; i++) {
        FireSlot *slot = &mgr->slot[i];

        if (slot->playing == 0 && slot->request == 0) {
            slot->request = 1;
            slot->pos = fire->pos;
            slot->angle = fire->angle;
            return slot;
        }
    }
    return NULL;
}

void fn_1_15390(void *unused, FireSlot *slot) {
    slot->request = 0;
    slot->owner = NULL;
}

void fn_1_153A4(FireSlot *slot, FireMgr *mgr) {
    u16 camBit = FIRE_CAMBIT(mgr->owner);
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *data;
    HU3DMODELID model;
    s32 i;

    memset(slot, 0, sizeof(FireSlot));
    slot->mgr = mgr;
    slot->request = 0;
    slot->playing = 0;
    slot->run = 0;
    slot->enabled = 0;
    slot->pad48 = -1;
    slot->sound = -1;
    model = Hu3DParticleCreate(mgr->anim, FIRE_PARTICLE_MAX);
    slot->model = model;
    Hu3DModelLayerSet(model, 4);
    Hu3DModelCameraSet(model, camBit);
    Hu3DParticleHookSet(model, fn_1_152C8);
    Hu3DParticleBlendModeSet(model, 1);
    Hu3DParticleAnimModeSet(model, 0);
    particle = Hu3DData[model].hookData;
    particle->emitCnt = 0;
    particle->work = NULL;
    particle->count = 1;
    data = particle->data;
    for (i = 0; i < particle->maxCnt; i++) {
        slot->life[i] = 0;
        data->cameraBit = camBit;
        data->scale = 0.0f;
        data++;
    }
}

void fn_1_154FC(FireSlot *slot) {
    memset(slot, 0, sizeof(FireSlot));
}

void fn_1_15554(FireSlot *slot) {
    M562Work *owner = slot->mgr->owner;
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *data;
    s32 i;
    s32 alive;
    s32 spawned;
    HuVecF scr;
    Vec p;
    f32 t;
    GXColor col;

    if (slot->run != 0 && slot->enabled == 0) {
        slot->enabled = 1;
    } else if (slot->run == 0 && slot->enabled != 0) {
        particle = Hu3DData[slot->model].hookData;
        data = particle->data;
        for (i = 0; i < particle->maxCnt; i++) {
            data->scale = 0.0f;
            data++;
        }
        slot->enabled = 0;
    }
    if (slot->enabled != 0 && slot->suppress == 0) {
        if (slot->playing == 0) {
            if (slot->request != 0) {
                slot->playing = 1;
                if (slot->sound < 0) {
                    slot->sound = fn_1_814(0x8A0);
                }
            } else {
                if (slot->sound >= 0) {
                    fn_1_83C(slot->sound);
                    slot->sound = -1;
                }
                return;
            }
        }
        if (slot->playing != 0 && slot->sound >= 0) {
            Hu3D3Dto2D(&slot->pos, 2, &scr);
            fn_1_894(slot->sound, (s32)(64.0f + 32.0f * (2.0f * (scr.x / 576.0f - 0.5f))));
        }
        particle = Hu3DData[slot->model].hookData;
        data = particle->data;
        spawned = 0;
        alive = 0;
        for (i = 0; i < particle->maxCnt; i++) {
            if (slot->life[i] != 0) {
                t = (f32)slot->life[i] / 30.0f;
                p.x = slot->pos.x + (30.0f + 100.0f * t) * sin(FIRE_RAD(slot->angle));
                p.y = 40.0f + slot->pos.y + 10.0f * ((1.0f - t) * (1.0f - t));
                p.z = slot->pos.z + (30.0f + 100.0f * t) * cos(FIRE_RAD(slot->angle));
                data->cameraBit = FIRE_CAMBIT(owner);
                data->scale = 20.0f + 60.0f * (t * t) + 10.0f * ((f32)(frand() % 10000) / 10000.0f);
                data->color.r = 96.0f + 32.0f * (1.0f - t);
                data->color.g = 64.0f + 64.0f * (1.0f - t);
                data->color.b = 64.0f + 64.0f * (1.0f - t);
                data->color.a = 0xFF;
                data->pos.x = p.x;
                data->pos.y = p.y;
                data->pos.z = p.z;
                if (slot->life[i]++ > 30) {
                    slot->life[i] = 0;
                    data->scale = 0.0f;
                }
                alive++;
            } else {
                if (slot->request != 0 && spawned == 0 && slot->timer % 3 == 0) {
                    slot->life[i] = 1;
                    spawned = 1;
                }
                data->scale = 0.0f;
            }
            data++;
        }
        if (alive != 0) {
            col = lbl_1_data_9D0;
            fn_1_15B78(slot);
        }
        if (alive == 0 && slot->request == 0 && slot->playing != 0) {
            slot->playing = 0;
        }
        slot->timer++;
    }
}

void fn_1_15B78(FireSlot *slot) {
    FireBoss *boss = FIRE_BOSS(slot->mgr->owner);
    Vec diff, dir, fwd;
    f32 dot;

    if ((boss->state == 1 || boss->state == 2) && boss->f10 == 0) {
        PSVECSubtract(&boss->pos, &slot->pos, &diff);
        if (PSVECMag(&diff) < 170.0f) {
            if (diff.x == 0.0f && diff.z == 0.0f) {
                diff.z = 1.0f;
            }
            PSVECNormalize(&diff, &dir);
            fwd.x = sin(FIRE_RAD(slot->angle));
            fwd.y = 0.0f;
            fwd.z = cos(FIRE_RAD(slot->angle));
            dot = PSVECDotProduct(&dir, &fwd);
            if (dot >= cos(0.5235987755982988)) {
                boss->fA = 0;
                boss->f6 = 0;
                boss->f8 = 0;
                boss->state = 4;
            }
        }
    }
}

void fn_1_15D40(Fire *fire, s16 index, FireMgr *mgr) {
    u16 camBit = FIRE_CAMBIT(mgr->owner);
    HU3DMODELID model, shadow, death;
    f32 t;
    s32 i;

    memset(fire, 0, sizeof(Fire));
    fire->mgr = mgr;
    fire->shown = 0;
    fire->run = 0;
    fire->index = index;
    if (index == 0) {
        model = Hu3DModelCreateData(DATANUM(DATA_m562, 0x13));
        fire->model = model;
        shadow = Hu3DModelCreateData(DATANUM(DATA_m562, 0x15));
        fire->shadow = shadow;
    } else {
        model = Hu3DModelLink(mgr->fire[0].model);
        fire->model = model;
        shadow = Hu3DModelLink(mgr->fire[0].shadow);
        fire->shadow = shadow;
    }
    t = (f32)(frand() % 10000) / 10000.0f;
    fire->rot.y = fire->targetAngle = fire->angle = FIRE_WRAP_F(360.0f * (t - 0.5f));
    Hu3DModelPosSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(model, 0.0f, fire->angle, 0.0f);
    Hu3DModelCameraSet(model, camBit);
    Hu3DModelLayerSet(model, 3);
    Hu3DModelAttrSet(model, 1);
    Hu3DModelPosSet(shadow, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(shadow, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(shadow, 0.8f, 0.8f, 0.8f);
    Hu3DModelCameraSet(shadow, camBit);
    Hu3DModelLayerSet(shadow, 3);
    Hu3DModelAttrSet(shadow, 1);
    for (i = 0; i < 8; i++) {
        if (index == 0) {
            fire->motion[i] = Hu3DJointMotionData(model, lbl_1_data_998[i]);
        } else {
            fire->motion[i] = mgr->fire[0].motion[i];
        }
    }
    Hu3DMotionSet(model, fire->motion[0]);
    Hu3DModelAttrSet(model, 0x40000001);
    fire->curMotion = 0;
    fire->nextMotion = 0;
    fire->loopMotion = 1;
    death = Hu3DModelLink(mgr->deathModel);
    fire->deathModel = death;
    Hu3DModelRotSet(death, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(death, 0.5f, 0.5f, 0.5f);
    Hu3DModelCameraSet(death, camBit);
    Hu3DModelLayerSet(death, 4);
    Hu3DModelAttrSet(death, 1);
    Hu3DMotionSet(death, mgr->deathMotion);
    Hu3DModelAttrReset(death, 0x40000001);
}

void fn_1_16218(Fire *fire) {
    s32 i;

    Hu3DModelKill(fire->deathModel);
    if (fire->index == 0) {
        for (i = 0; i < 8; i++) {
            Hu3DMotionKill(fire->motion[i]);
        }
    }
    Hu3DModelKill(fire->shadow);
    Hu3DModelKill(fire->model);
    memset(fire, 0, sizeof(Fire));
}

void fn_1_162A0(Fire *fire) {
    if (fire->state < 6) {
        if (fire->flame != NULL) {
            fn_1_15390(NULL, fire->flame);
            fire->flame = NULL;
            fire->flameTimer = 0;
            fire->flameStage = 0;
        }
        if (fire->mgr->owner->step == 4) {
            fn_1_814(0x8A1);
        }
        fire->timer = 13;
        fire->state = 6;
    }
}

void fn_1_16350(Fire *fire) {
    FireMgr *mgr = fire->mgr;
    FireBoss *boss = FIRE_BOSS(mgr->owner);
    HU3DMODELID model;
    f32 maxTime;

    if (fire->run != 0 && fire->shown == 0) {
        Hu3DModelAttrReset(fire->model, 1);
        Hu3DModelAttrReset(fire->shadow, 1);
        fire->shown = 1;
    } else if (fire->run == 0 && fire->shown != 0) {
        Hu3DModelAttrSet(fire->model, 1);
        Hu3DModelAttrSet(fire->shadow, 1);
        fire->shown = 0;
    }
    if (fire->shown != 0) {
        if (mgr->enable == 0) {
            if (fire->state < 4) {
                fire->state = 1;
                fire->timer = 1;
            }
            if (fire->flame != NULL) {
                fn_1_15390(NULL, fire->flame);
                fire->flame = NULL;
                fire->flameTimer = 0;
                fire->flameStage = 0;
            }
        }
        switch (fire->state) {
        case 0:
            if (fire->run != 0) {
                Hu3DModelAttrReset(fire->model, 1);
                Hu3DModelAttrReset(fire->shadow, 1);
            }
            if (fire->aggressive != 0) {
                fire->timer = (frand() % 60) + 0x3C;
                fire->state = 3;
                if (fire->flameWait > 0xB4 && frand() % 10000 < 0x7D0) {
                    if (fire->flameStage == 0) {
                        fire->flameStage = 1;
                        fire->timer = 0xB4;
                    }
                    fire->flameWait = 0;
                }
            } else {
                fire->timer = (frand() % 90) + 0x5A;
                if ((frand() & 1) != 0) {
                    fire->state = 1;
                    if (fire->flameWait > 0xB4 && frand() % 10000 < 0x7D0) {
                        if (fire->flameStage == 0) {
                            fire->flameStage = 1;
                            fire->timer = 0xB4;
                        }
                        fire->flameWait = 0;
                    }
                } else {
                    fire->state = 2;
                }
            }
            fire->phase = 0;
            fire->timer2 = 0;
            break;
        case 1:
            fn_1_16CF4(fire);
            if (--fire->timer <= 0) {
                fire->timer = 0;
                fire->state = 0;
            }
            break;
        case 2:
            fn_1_16D08(fire);
            if (--fire->timer <= 0) {
                fire->timer = 0;
                fire->state = 0;
            }
            break;
        case 3:
            fn_1_170E0(fire);
            if (--fire->timer <= 0) {
                fire->timer = 0;
                fire->state = 0;
            }
            break;
        case 4:
            fn_1_1776C(fire);
            break;
        case 5:
            if (fn_1_174C0(fire) != 0 && fire->state < 6) {
                if (fire->flame != NULL) {
                    fn_1_15390(NULL, fire->flame);
                    fire->flame = NULL;
                    fire->flameTimer = 0;
                    fire->flameStage = 0;
                }
                if (fire->mgr->owner->step == 4) {
                    fn_1_814(0x8A1);
                }
                fire->timer = 13;
                fire->state = 6;
            }
            break;
        case 6:
            fire->nextMotion = 5;
            fire->loopMotion = 0;
            if (fire->timer-- <= 0) {
                Hu3DMotionTimeSet(fire->deathModel, 0.0f);
                Hu3DModelPosSet(fire->deathModel, fire->pos.x, fire->pos.y, fire->pos.z - 10.0f);
                Hu3DModelAttrReset(fire->deathModel, 1);
                fire->deathPlaying = 1;
                Hu3DModelAttrSet(fire->model, 1);
                Hu3DModelAttrSet(fire->shadow, 1);
                fire->timer = 0;
                fire->state = 7;
            }
            break;
        case 7:
            Hu3DModelAttrSet(fire->model, 1);
            Hu3DModelAttrSet(fire->shadow, 1);
            break;
        }
        fn_1_16AC0(fire);
        model = fire->model;
        if (fire->curMotion != fire->nextMotion && Hu3DMotionShiftIDGet(model) < 0) {
            fire->curMotion = fire->nextMotion;
            Hu3DModelAttrReset(model, 0x40000001);
            Hu3DMotionShiftSet(model, fire->motion[fire->curMotion], 0.0f, 4.0f, (fire->loopMotion != 0) ? 0x40000001 : 0);
        }
        if (fire != boss->held) {
            if (fire->state < 5) {
                fire->vy -= 1.5f;
                fire->pos.y += fire->vy;
                if (fire->pos.y <= 0.0f) {
                    fire->pos.y = 0.0f;
                    fire->vy = 0.0f;
                }
            }
            if (fire->pos.x > 625.0f) {
                fire->pos.x = 625.0f;
            } else if (fire->pos.x < -625.0f) {
                fire->pos.x = -625.0f;
            }
            if (fire->pos.z > 500.0f) {
                fire->pos.z = 500.0f;
            } else if (fire->pos.z < -500.0f) {
                fire->pos.z = -500.0f;
            }
        }
        Hu3DModelPosSet(model, fire->pos.x, fire->pos.y, fire->pos.z);
        Hu3DModelRotSet(model, fire->rot.x, fire->rot.y, fire->rot.z);
        if (fire->state == 4) {
            Hu3DModelPosSet(fire->shadow, boss->pos.x, 5.0f, boss->pos.z);
        } else {
            Hu3DModelPosSet(fire->shadow, fire->pos.x, 5.0f, fire->pos.z);
        }
        if (fire->deathPlaying != 0) {
            model = fire->deathModel;
            maxTime = Hu3DMotionMaxTimeGet(model);
            if (Hu3DMotionTimeGet(model) >= maxTime) {
                Hu3DModelAttrSet(model, 1);
                fire->deathPlaying = 0;
            }
        }
    }
}

void fn_1_16AC0(Fire *fire) {
    FireSlot *slot;

    switch (fire->flameStage) {
    case 0:
        break;
    case 1:
        slot = fn_1_15308(fire->mgr, fire);
        if (slot == NULL) {
            fire->flameTimer = 0;
            fire->flameStage = 0;
            break;
        }
        fire->flame = slot;
        fire->savedMotion = fire->curMotion;
        slot->owner = fire;
        slot->suppress = 1;
        fire->flameTimer = 60;
        fire->flameStage++;
        /* fallthrough */
    case 2:
        fire->nextMotion = (fire->state == 1) ? 3 : 6;
        fire->loopMotion = 1;
        if (--fire->flameTimer <= 0) {
            fire->flame->suppress = 0;
            fire->flameTimer = 90;
            fire->flameStage++;
        }
        break;
    case 3:
        if (fire->flame != NULL) {
            fire->flame->pos = fire->pos;
            fire->flame->angle = fire->angle;
        }
        fire->nextMotion = (fire->state == 1) ? 4 : 7;
        fire->loopMotion = 1;
        if (--fire->flameTimer <= 0) {
            fire->nextMotion = fire->savedMotion;
            fire->loopMotion = 1;
            fn_1_15390(NULL, fire->flame);
            fire->flame = NULL;
            fire->flameTimer = 0;
            fire->flameStage = 0;
        }
        break;
    }
    fire->flameWait++;
}

void fn_1_16CF4(Fire *fire) {
    fire->nextMotion = 0;
    fire->loopMotion = 1;
}

void fn_1_16D08(Fire *fire) {
    Vec step;
    f32 t;

    fire->nextMotion = 2;
    fire->loopMotion = 1;
    switch (fire->phase) {
    case 0:
        t = (f32)(frand() % 10000) / 10000.0f;
        fire->targetAngle = FIRE_WRAP_F(fire->angle + 120.0f * (t - 0.5f));
        fire->timer2 = (frand() % 90) + 0x5A;
        fire->phase = 1;
        break;
    case 1:
        if (fire->pos.z <= -500.0f) {
            fire->targetAngle = 0.0f;
        } else if (fire->pos.z >= 500.0f) {
            fire->targetAngle = 180.0f;
        }
        if (fire->pos.x <= -625.0f) {
            fire->targetAngle = 90.0f;
        } else if (fire->pos.x >= 625.0f) {
            fire->targetAngle = -90.0f;
        }
        if (fire->timer2-- <= 0) {
            fire->timer2 = 0;
            fire->phase = 0;
        }
        break;
    }
    fire->angle = fn_1_36EC(fire->targetAngle, fire->angle, 0.95f);
    step.x = 7.0 * sin(FIRE_RAD(fire->angle));
    step.z = 7.0 * cos(FIRE_RAD(fire->angle));
    fire->pos.x += step.x;
    fire->pos.z += step.z;
    fire->rot.y = fire->angle;
}

void fn_1_170E0(Fire *fire) {
    Vec dir;
    f32 mag;
    f64 speed = 7.0;

    fire->nextMotion = 2;
    fire->loopMotion = 1;
    PSVECSubtract(&FIRE_BOSS(fire->mgr->owner)->pos, &fire->pos, &dir);
    dir.y = 0.0f;
    mag = PSVECMag(&dir);
    if (dir.x != 0.0f || dir.z != 0.0f) {
        PSVECNormalize(&dir, &dir);
    }
    fire->targetAngle = FIRE_WRAP_D(180.0 * (atan2(dir.x, dir.z) / 3.141592653589793));
    if (fire->pos.z <= -500.0f) {
        fire->targetAngle = 0.0f;
    } else if (fire->pos.z >= 500.0f) {
        fire->targetAngle = 180.0f;
    }
    if (fire->pos.x <= -625.0f) {
        fire->targetAngle = 90.0f;
    } else if (fire->pos.x >= 625.0f) {
        fire->targetAngle = -90.0f;
    }
    if (mag < 300.0f) {
        fire->nextMotion = 0;
        fire->loopMotion = 1;
        speed = 0.0;
    }
    fire->angle = fn_1_36EC(fire->targetAngle, fire->angle, 0.95f);
    dir.x = speed * sin(FIRE_RAD(fire->angle));
    dir.z = speed * cos(FIRE_RAD(fire->angle));
    fire->pos.x += dir.x;
    fire->pos.z += dir.z;
    fire->rot.y = fire->angle;
}

s32 fn_1_174C0(Fire *fire) {
    Vec step;
    s32 done = 0;

    switch (fire->phase) {
    case 0:
        fire->nextMotion = 0;
        fire->loopMotion = 1;
        fire->vy = sqrtf(600.0f);
        fire->targetAngle = fire->angle = FIRE_BOSS(fire->mgr->owner)->rotY;
        fire->phase = 1;
        break;
    case 1:
        fire->vy -= 1.5f;
        fire->pos.y += fire->vy;
        if (fire->vy < 0.0f) {
            if (fire->pos.y <= 0.0f) {
                fire->rot.z = 0.0f;
                fire->rot.x = 0.0f;
                fire->pos.y = 0.0f;
                done = 1;
            }
        }
        break;
    }
    step.x = 14.0 * sin(FIRE_RAD(fire->angle));
    step.z = 14.0 * cos(FIRE_RAD(fire->angle));
    fire->pos.x += step.x;
    fire->pos.z += step.z;
    fire->rot.y = fire->angle;
    return done;
}

s32 fn_1_1776C(Fire *fire) {
    fire->nextMotion = 0;
    fire->loopMotion = 1;
    if (fire->flame != NULL) {
        fn_1_15390(NULL, fire->flame);
        fire->flame = NULL;
        fire->flameTimer = 0;
        fire->flameStage = 0;
    }
    return 0;
}
