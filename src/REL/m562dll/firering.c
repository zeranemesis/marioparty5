#include "REL/m562dll.h"
#include "game/frand.h"
#include "datadir_enum.h"
#include "game/mg/timer.h"
#include "math.h"
#include "string.h"

#define FR_RAD(a) (3.141592653589793 * (a) / 180.0)

/* put the three rings on a circle (120 degrees apart) of radius rad */
#define FR_CIRCLES(mgr, rad) \
    do { \
        s32 i_; \
        for (i_ = 0; i_ < 3; i_++) { \
            FireRing *r_ = &(mgr)->ring[i_]; \
            f32 a_ = 60.0f + 120.0f * (f32)i_; \
            Vec v_; \
            v_.x = (rad) * sin(FR_RAD(a_)); \
            v_.y = 40.0f; \
            v_.z = (rad) * cos(FR_RAD(a_)); \
            r_->pos = v_; \
        } \
    } while (0)

/* same, but also reset the radius and the start of the phase */
#define FR_CIRCLES_INIT(mgr) \
    do { \
        s32 i_; \
        for (i_ = 0; i_ < 3; i_++) { \
            FireRing *r_ = &(mgr)->ring[i_]; \
            f32 a_ = 60.0f + 120.0f * (f32)i_; \
            Vec v_; \
            v_.x = 380.0f * sin(FR_RAD(a_)); \
            v_.y = 40.0f; \
            v_.z = 380.0f * cos(FR_RAD(a_)); \
            r_->pos = v_; \
            r_->radius = 380.0f; \
        } \
    } while (0)

/* remember where the rings start */
#define FR_SAVE_POS(mgr) \
    do { \
        lbl_1_bss_7860 = (mgr)->ring[0].pos; \
        lbl_1_bss_7854 = (mgr)->ring[1].pos; \
        lbl_1_bss_7848 = (mgr)->ring[2].pos; \
    } while (0)

Vec lbl_1_data_A8C = { 0.0f, 7000.0f, 0.0f };
Vec lbl_1_data_A98 = { 0.0f, -1.0f, 0.02f };
GXColor lbl_1_data_AA4 = { 0, 0, 0, 0 };
GXColor lbl_1_data_AA8[2] = { { 0xA0, 0x20, 0x00, 0xFF }, { 0, 0, 0, 0 } };

Vec lbl_1_bss_7848;
Vec lbl_1_bss_7854;
Vec lbl_1_bss_7860;
FireRingPath lbl_1_bss_786C;

void fn_1_18E88(FireRingMgr *mgr, M562Work *owner) {
    s16 i;

    memset(mgr, 0, sizeof(FireRingMgr));
    mgr->owner = owner;
    mgr->active = 0;
    mgr->sound = -1;
    mgr->anim = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m562, 0x6A), HU_MEMNUM_OVL, HEAP_MODEL));
    mgr->anim2 = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m562, 0x6B), HU_MEMNUM_OVL, HEAP_MODEL));
    for (i = 0; i < 3; i++) {
        fn_1_19790(&mgr->ring[i], i, mgr);
    }
}

void fn_1_18F5C(FireRingMgr *mgr) {
    M562Work *owner = mgr->owner;
    s32 i;

    for (i = 0; i < 3; i++) {
        fn_1_19A3C(&mgr->ring[i]);
    }
    memset(mgr, 0, sizeof(FireRingMgr));
}

void fn_1_18FC8(FireRingMgr *mgr) {
    M562Work *owner = mgr->owner;
    M562Player *boss = FIRE_BOSS(owner);
    s32 i;
    f32 rad;

    if (mgr->moveOn == 0) {
        if (mgr->state == 4) {
            mgr->state = 4;
        } else if (mgr->state >= 1) {
            mgr->state = 1;
        }
    }
    switch (mgr->state) {
    case 4:
        break;
    case 0:
        FR_CIRCLES(mgr, 450.0f);
        for (i = 0; i < 3; i++) {
            mgr->ring[i].radius = 380.0f;
        }
        fn_1_19518(mgr);
        mgr->patIdx = 0;
        mgr->timer = 0;
        mgr->state = 1;
        /* fallthrough */
    case 1:
        if (mgr->moveOn != 0) {
            if (mgr->sound >= 0) {
                fn_1_83C(mgr->sound);
                mgr->sound = -1;
            }
            mgr->timer = 0;
            mgr->state = 2;
        }
        break;
    case 2:
        if (mgr->sound < 0) {
            mgr->sound = fn_1_814(0x8A2);
        }
        rad = 450.0f + 200.0f * (1.0f - (f32)mgr->timer / 60.0f);
        FR_CIRCLES(mgr, rad);
        for (i = 0; i < 3; i++) {
            mgr->ring[i].radius = 380.0f;
        }
        if (++mgr->timer >= 0x3C) {
            MgTimerModeOnSet(owner->timer, 1);
            mgr->timer = 0;
            mgr->state = 3;
            mgr->subTimer = 0;
            mgr->sub = 0;
        }
        break;
    case 3:
        if (mgr->sound < 0) {
            mgr->sound = fn_1_814(0x8A2);
        }
        if (fn_1_19694(mgr, mgr->order[mgr->patIdx]) != 0) {
            mgr->subTimer = 0;
            mgr->sub = 0;
            mgr->patIdx++;
            if (mgr->patIdx >= 6) {
                mgr->patIdx = 0;
                mgr->timer = 0;
                mgr->state = 4;
            } else if (mgr->sound >= 0) {
                fn_1_83C(mgr->sound);
                mgr->sound = -1;
            }
        }
        break;
    }
    for (i = 0; i < 3; i++) {
        FireRing *ring = &mgr->ring[i];

        if (mgr->active != 0) {
            ring->enable = 1;
        } else {
            ring->enable = 0;
        }
        fn_1_19A98(ring);
        if (ring->active != 0 && mgr->state != 4 && owner->step == 4 && fn_1_1A728(ring) != 0
            && (boss->state == 1 || boss->state == 2) && boss->invuln == 0) {
            boss->timerA = 0;
            boss->sub = 0;
            boss->subTimer = 0;
            boss->state = 4;
        }
    }
}

void fn_1_19518(FireRingMgr *mgr) {
    M562Work *owner = mgr->owner;
    s32 i;
    s32 j;
    s32 hard;
    s32 dup;
    s16 pick;

    for (i = 0; i < 6; i++) {
        do {
            hard = 0;
            switch (owner->diff) {
            case 0:
                if (i == 1 || i == 3 || i == 4 || i == 5) {
                    hard = 1;
                }
                break;
            case 1:
                if (i == 3 || i == 4 || i == 5) {
                    hard = 1;
                }
                break;
            case 2:
                if (i == 4 || i == 5) {
                    hard = 1;
                }
                break;
            default:
                if (i == 5) {
                    hard = 1;
                }
                break;
            }
            if (hard != 0) {
                pick = (frand() % 5) + 5;
            } else {
                pick = frand() % 5;
            }
            dup = 0;
            for (j = 0; j < i; j++) {
                if (mgr->order[j] == pick) {
                    dup = 1;
                    break;
                }
            }
        } while (dup != 0);
        mgr->order[i] = pick;
    }
}

s32 fn_1_19694(FireRingMgr *mgr, s16 pattern) {
    s32 done = 0;

    if ((u32)pattern <= 9) {
        switch (pattern) {
        case 0:
            done = fn_1_1A804(mgr);
            break;
        case 1:
            done = fn_1_1AC94(mgr);
            break;
        case 2:
            done = fn_1_1AFB4(mgr);
            break;
        case 3:
            done = fn_1_1B4FC(mgr);
            break;
        case 4:
            done = fn_1_1B9D4(mgr);
            break;
        case 5:
            done = fn_1_1BD6C(mgr);
            break;
        case 6:
            done = fn_1_1C418(mgr);
            break;
        case 7:
            done = fn_1_1CCC4(mgr);
            break;
        case 8:
            done = fn_1_1D320(mgr);
            break;
        case 9:
            done = fn_1_1D9F8(mgr);
            break;
        }
    }
    return done;
}

void fn_1_19790(FireRing *ring, s16 index, FireRingMgr *mgr) {
    M562Work *owner = mgr->owner;
    u16 camBit = FIRE_CAMBIT(owner);
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *data;
    HU3DMODELID model;
    Vec zero = { 0.0f, 0.0f, 0.0f };
    s32 i;

    memset(ring, 0, sizeof(FireRing));
    ring->mgr = mgr;
    ring->pos = zero;
    ring->radius = 800.0f;
    ring->count = 0x50;
    ring->light = -1;
    ring->active = 0;
    ring->enable = 0;
    ring->index = index;
    ring->angle = ((360.0f / (f32)ring->count) / 3.0f) * (f32)index;
    model = Hu3DParticleCreate(mgr->anim, 0x50);
    ring->model = model;
    Hu3DModelLayerSet(model, 4);
    Hu3DModelCameraSet(model, camBit);
    Hu3DParticleHookSet(model, fn_1_1A6E8);
    Hu3DParticleBlendModeSet(model, 0);
    Hu3DParticleAnimModeSet(model, 0);
    particle = Hu3DData[model].hookData;
    particle->emitCnt = 0;
    particle->work = NULL;
    particle->count = 1;
    data = particle->data;
    for (i = 0; i < particle->maxCnt; i++) {
        data->scale = 0.0f;
        data++;
    }
    model = Hu3DParticleCreate(mgr->anim2, 0xA0);
    ring->model2 = model;
    Hu3DModelLayerSet(model, 4);
    Hu3DModelCameraSet(model, camBit);
    Hu3DParticleHookSet(model, fn_1_1A6E8);
    Hu3DParticleBlendModeSet(model, 0);
    Hu3DParticleAnimModeSet(model, 0);
    particle = Hu3DData[model].hookData;
    particle->emitCnt = 0;
    particle->work = NULL;
    particle->count = 1;
    data = particle->data;
    for (i = 0; i < particle->maxCnt; i++) {
        data->scale = 0.0f;
        data++;
    }
}

void fn_1_19A3C(FireRing *ring) {
    if (ring->light >= 0) {
        Hu3DGLightKill(ring->light);
    }
    Hu3DModelKill(ring->model2);
    Hu3DModelKill(ring->model);
    memset(ring, 0, sizeof(FireRing));
}

void fn_1_19A98(FireRing *ring) {
    M562Work *owner = ring->mgr->owner;
    CAMERA *cam = &owner->camera[0];
    GXColor col1 = { 0xFF, 0xE0, 0xE0, 0xE0 };
    GXColor col2 = { 0xFF, 0xE0, 0xD0, 0xFF };
    GXColor base;
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *d1;
    HU3DPARTICLEDATA *d2;
    HuVecF lightPos;
    s32 i;
    f32 t;
    f32 a;

    if (ring->enable != 0 && ring->active == 0) {
        ring->light = Hu3DGLightCreateV(&lbl_1_data_A8C, &lbl_1_data_A98, &lbl_1_data_AA4);
        Hu3DGLightStaticSet(ring->light, 1);
        Hu3DGLightSpotSet(ring->light, GX_SP_COS, 36.0f);
        ring->active = 1;
        ring->fade = 0;
    } else if (ring->enable == 0 && ring->active != 0) {
        particle = Hu3DData[ring->model].hookData;
        d1 = particle->data;
        for (i = 0; i < particle->maxCnt; i++) {
            d1->scale = 0.0f;
            d1++;
        }
        particle = Hu3DData[ring->model2].hookData;
        d1 = particle->data;
        for (i = 0; i < particle->maxCnt; i++) {
            d1->scale = 0.0f;
            d1++;
        }
        ring->active = 0;
    }
    if (ring->active == 0) {
        if (ring->light >= 0) {
            if (ring->fade > 0) {
                t = (f32)ring->fade / 120.0f;
                Hu3DGLightColorSet(ring->light, (u8)(s32)((f32)lbl_1_data_AA8[0].r * t),
                                   (u8)(s32)((f32)lbl_1_data_AA8[0].g * t), (u8)(s32)((f32)lbl_1_data_AA8[0].b * t),
                                   (u8)(s32)((f32)lbl_1_data_AA8[0].a * t));
                ring->fade--;
            } else if (ring->fade == 0) {
                Hu3DGLightKill(ring->light);
                ring->light = -1;
            }
        }
        return;
    }
    if (ring->fade < 0x78 && ring->light >= 0) {
        t = (f32)ring->fade / 120.0f;
        Hu3DGLightColorSet(ring->light, (u8)(s32)((f32)lbl_1_data_AA8[0].r * t),
                           (u8)(s32)((f32)lbl_1_data_AA8[0].g * t), (u8)(s32)((f32)lbl_1_data_AA8[0].b * t),
                           (u8)(s32)((f32)lbl_1_data_AA8[0].a * t));
        ring->fade++;
    } else if (ring->fade >= 0x78) {
        base = lbl_1_data_AA8[0];
        base.r = (f32)lbl_1_data_AA8[0].r
                 * (0.699999988079071 + 0.30000001192092896 * sin(FR_RAD(180.0f * ((f32)(owner->frame % 4) * 0.25f))));
        Hu3DGLightColorSet(ring->light, base.r, base.g, base.b, base.a);
    }
    t = ring->radius / 800.0f;
    if (t > 1.0f) {
        t = 1.0f;
    } else if (t < 0.0f) {
        t = 0.0f;
    }
    ring->count = 80.0f * (0.25f + 0.75f * t);
    particle = Hu3DData[ring->model].hookData;
    d1 = particle->data;
    d2 = ((HU3DPARTICLE *)Hu3DData[ring->model2].hookData)->data;
    for (i = 0; i < particle->maxCnt; i++) {
        if (i >= ring->count) {
            d1->scale = 0.0f;
            d1++;
            d2->scale = 0.0f;
            d2++;
            d2->scale = 0.0f;
            d2++;
        } else {
            a = ring->angle + (f32)i * (360.0f / (f32)ring->count);
            d1->cameraBit = (u16)cam->camMask;
            d1->pos.x = ring->pos.x + ring->radius * sin(FR_RAD(a));
            d1->pos.y = 40.0f + ring->pos.y;
            d1->pos.z = ring->pos.z + ring->radius * cos(FR_RAD(a));
            d1->scale = ((i & 1) != 0) ? 120.0f : 140.0f;
            d1->color = col1;
            d1++;
            a = ring->angle + 2.0f * (f32)i * (360.0f / (f32)(ring->count * 2));
            d2->cameraBit = (u16)cam->camMask;
            d2->pos.x = ring->pos.x + ring->radius * sin(FR_RAD(a));
            d2->pos.y = 40.0f + ring->pos.y;
            d2->pos.z = ring->pos.z + ring->radius * cos(FR_RAD(a));
            d2->scale = 50.0f;
            d2->color = col2;
            d2++;
            a = ring->angle + (1.0f + 2.0f * (f32)i) * (360.0f / (f32)(ring->count * 2));
            d2->cameraBit = (u16)cam->camMask;
            d2->pos.x = ring->pos.x + ring->radius * sin(FR_RAD(a));
            d2->pos.y = 40.0f + ring->pos.y;
            d2->pos.z = ring->pos.z + ring->radius * cos(FR_RAD(a));
            d2->scale = 80.0f;
            d2->color = col2;
            d2++;
        }
    }
    t = (ring->radius - 190.0f) / 610.0f;
    if (t > 1.0f) {
        t = 1.0f;
    } else if (t < 0.0f) {
        t = 0.0f;
    }
    lightPos = ring->pos;
    lightPos.y += 1000.0f * t;
    Hu3DGLightPosSetV(ring->light, &lightPos, &lbl_1_data_A98);
    ring->angle -= 0.5f;
    ring->tick++;
}

void fn_1_1A6E8(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx) {
    HU3DPARTICLEDATA *data = particle->data;

    DCStoreRangeNoSync(particle->data, particle->maxCnt * sizeof(HU3DPARTICLEDATA));
}

s32 fn_1_1A728(FireRing *ring) {
    M562Player *boss = FIRE_BOSS(ring->mgr->owner);
    Vec d;
    f32 mag;
    s32 hit = 0;

    PSVECSubtract(&boss->pos, &ring->pos, &d);
    d.y = 0.0f;
    mag = PSVECMag(&d);
    if (mag >= ring->radius - 50.0f) {
        if (mag <= 50.0f + ring->radius && boss->pos.y < 50.0f) {
            hit = 1;
        }
    }
    return hit;
}

s32 fn_1_1A804(FireRingMgr *mgr) {
    s32 done = 0;
    f32 rad;

    switch (mgr->sub) {
    case 0:
        FR_CIRCLES_INIT(mgr);
        mgr->subTimer = 0;
        mgr->sub++;
        /* fallthrough */
    case 1:
        rad = 450.0f * (1.0f - (f32)mgr->subTimer / 150.0f);
        FR_CIRCLES(mgr, rad);
        if (mgr->subTimer++ >= 0x96) {
            mgr->subTimer = 0;
            mgr->sub++;
        }
        break;
    case 2:
        rad = 450.0f * ((f32)mgr->subTimer / 150.0f);
        FR_CIRCLES(mgr, rad);
        if (++mgr->subTimer >= 0x96) {
            mgr->subTimer = 0;
            mgr->sub = 0;
            done = 1;
        }
        break;
    }
    return done;
}

s32 fn_1_1AC94(FireRingMgr *mgr) {
    s32 done = 0;
    s32 i;
    f32 t;

    switch (mgr->sub) {
    case 0:
        FR_CIRCLES_INIT(mgr);
        mgr->subTimer = 0;
        mgr->sub++;
        /* fallthrough */
    case 1:
        t = (f32)mgr->subTimer / 300.0f;
        for (i = 0; i < 3; i++) {
            FireRing *r = &mgr->ring[i];
            f32 a = (60.0f + 120.0f * (f32)i) - 360.0f * t;
            Vec v;

            v.x = 380.0f * sin(FR_RAD(a));
            v.y = 40.0f;
            v.z = 380.0f * cos(FR_RAD(a));
            r->pos = v;
        }
        if (++mgr->subTimer >= 0x12C) {
            mgr->subTimer = 0;
            mgr->sub = 0;
            done = 1;
        }
        break;
    }
    return done;
}

s32 fn_1_1AFB4(FireRingMgr *mgr) {
    s32 done = 0;
    s32 i;
    f32 t;
    f32 rad;

    switch (mgr->sub) {
    case 0:
        FR_CIRCLES_INIT(mgr);
        mgr->subTimer = 0;
        mgr->sub++;
        /* fallthrough */
    case 1:
        t = (f32)mgr->subTimer / 150.0f;
        rad = 450.0f * (0.4f + 0.6f * (1.0f - t));
        for (i = 0; i < 3; i++) {
            FireRing *r = &mgr->ring[i];
            f32 a = 60.0f + 120.0f * (f32)i;
            Vec v;

            v.x = rad * sin(FR_RAD(a));
            v.y = 40.0f;
            v.z = rad * cos(FR_RAD(a));
            r->pos = v;
            r->radius = 380.0f * (0.5f + 0.5f * (1.0f - t));
        }
        if (mgr->subTimer++ >= 0x96) {
            mgr->subTimer = 0;
            mgr->sub++;
        }
        break;
    case 2:
        t = (f32)mgr->subTimer / 150.0f;
        rad = 450.0f * (0.4f + 0.6f * t);
        for (i = 0; i < 3; i++) {
            FireRing *r = &mgr->ring[i];
            f32 a = 60.0f + 120.0f * (f32)i;
            Vec v;

            v.x = rad * sin(FR_RAD(a));
            v.y = 40.0f;
            v.z = rad * cos(FR_RAD(a));
            r->pos = v;
            r->radius = 380.0f * (0.5f + 0.5f * t);
        }
        if (++mgr->subTimer >= 0x96) {
            mgr->subTimer = 0;
            mgr->sub = 0;
            done = 1;
        }
        break;
    }
    return done;
}

s32 fn_1_1B4FC(FireRingMgr *mgr) {
    s32 done = 0;
    s32 i;
    f32 t;
    f32 rad;

    switch (mgr->sub) {
    case 0:
        FR_CIRCLES_INIT(mgr);
        mgr->subTimer = 0;
        mgr->sub++;
        /* fallthrough */
    case 1:
        t = (f32)mgr->subTimer / 150.0f;
        rad = 450.0f * (1.0f - t);
        for (i = 0; i < 3; i++) {
            FireRing *r = &mgr->ring[i];
            f32 a = 60.0f + 120.0f * (f32)i;
            Vec v;

            v.x = rad * sin(FR_RAD(a));
            v.y = 40.0f;
            v.z = rad * cos(FR_RAD(a));
            r->pos = v;
            r->radius = 380.0f + 420.0f * t;
        }
        if (mgr->subTimer++ >= 0x96) {
            mgr->subTimer = 0;
            mgr->sub++;
        }
        break;
    case 2:
        t = (f32)mgr->subTimer / 150.0f;
        rad = 450.0f * t;
        for (i = 0; i < 3; i++) {
            FireRing *r = &mgr->ring[i];
            f32 a = 60.0f + 120.0f * (f32)i;
            Vec v;

            v.x = rad * sin(FR_RAD(a));
            v.y = 40.0f;
            v.z = rad * cos(FR_RAD(a));
            r->pos = v;
            r->radius = 800.0f + -420.0f * t;
        }
        if (++mgr->subTimer >= 0x96) {
            mgr->subTimer = 0;
            mgr->sub = 0;
            done = 1;
        }
        break;
    }
    return done;
}

s32 fn_1_1B9D4(FireRingMgr *mgr) {
    FireRing *r0 = &mgr->ring[0];
    FireRing *r1 = &mgr->ring[1];
    FireRing *r2 = &mgr->ring[2];
    s32 done = 0;
    f32 t;

    switch (mgr->sub) {
    case 0:
        FR_CIRCLES_INIT(mgr);
        FR_SAVE_POS(mgr);
        mgr->subTimer = 0;
        mgr->sub++;
        /* fallthrough */
    case 1:
        t = (f32)mgr->subTimer / 150.0f;
        r0->pos.z = lbl_1_bss_7860.z + -lbl_1_bss_7860.z * t;
        r1->pos.z = lbl_1_bss_7854.z + -lbl_1_bss_7854.z * t;
        r2->pos.z = lbl_1_bss_7848.z + -lbl_1_bss_7848.z * t;
        if (mgr->subTimer++ >= 0x96) {
            mgr->subTimer = 0;
            mgr->sub++;
        }
        break;
    case 2:
        t = (f32)mgr->subTimer / 150.0f;
        r0->pos.z = lbl_1_bss_7860.z * t;
        r1->pos.z = lbl_1_bss_7854.z * t;
        r2->pos.z = lbl_1_bss_7848.z * t;
        if (++mgr->subTimer >= 0x96) {
            mgr->subTimer = 0;
            mgr->sub = 0;
            done = 1;
        }
        break;
    }
    return done;
}

s32 fn_1_1BD6C(FireRingMgr *mgr) {
    FireRing *r0 = &mgr->ring[0];
    FireRing *r1 = &mgr->ring[1];
    FireRing *r2 = &mgr->ring[2];
    s32 done = 0;
    f32 t;

    switch (mgr->sub) {
    case 0:
        FR_CIRCLES_INIT(mgr);
        FR_SAVE_POS(mgr);
        mgr->subTimer = 0;
        mgr->sub++;
        /* fallthrough */
    case 1:
        t = (f32)mgr->subTimer / 75.0f;
        r0->radius = 380.0f + 420.0f * t;
        r1->radius = 380.0f + -190.0f * t;
        r2->radius = 380.0f + -190.0f * t;
        r0->pos.x = lbl_1_bss_7860.x + -lbl_1_bss_7860.x * t;
        r0->pos.z = lbl_1_bss_7860.z + -lbl_1_bss_7860.z * t;
        if (mgr->subTimer++ >= 0x4B) {
            mgr->subTimer = 0;
            mgr->sub++;
        }
        break;
    case 2:
        t = (f32)mgr->subTimer / 75.0f;
        r0->radius = 800.0f + -610.0f * t;
        r2->radius = 190.0f + 610.0f * t;
        r1->radius = 190.0f;
        r0->pos.x = lbl_1_bss_7860.x * t;
        r0->pos.z = lbl_1_bss_7860.z * t;
        r2->pos.x = lbl_1_bss_7848.x + -lbl_1_bss_7848.x * t;
        r2->pos.z = lbl_1_bss_7848.z + -lbl_1_bss_7848.z * t;
        if (mgr->subTimer++ >= 0x4B) {
            mgr->subTimer = 0;
            mgr->sub++;
        }
        break;
    case 3:
        t = (f32)mgr->subTimer / 75.0f;
        r2->radius = 800.0f + -610.0f * t;
        r1->radius = 190.0f + 610.0f * t;
        r0->radius = 190.0f;
        r2->pos.x = lbl_1_bss_7848.x * t;
        r2->pos.z = lbl_1_bss_7848.z * t;
        r1->pos.x = lbl_1_bss_7854.x + -lbl_1_bss_7854.x * t;
        r1->pos.z = lbl_1_bss_7854.z + -lbl_1_bss_7854.z * t;
        if (mgr->subTimer++ >= 0x4B) {
            mgr->subTimer = 0;
            mgr->sub++;
        }
        break;
    case 4:
        t = (f32)mgr->subTimer / 75.0f;
        r2->radius = 190.0f + 190.0f * t;
        r1->radius = 800.0f + -420.0f * t;
        r0->radius = 190.0f + 190.0f * t;
        r1->pos.x = lbl_1_bss_7854.x * t;
        r1->pos.z = lbl_1_bss_7854.z * t;
        if (mgr->subTimer++ >= 0x4B) {
            mgr->subTimer = 0;
            mgr->sub = 0;
            done = 1;
        }
        break;
    }
    return done;
}

s32 fn_1_1C418(FireRingMgr *mgr) {
    FireRing *r0 = &mgr->ring[0];
    FireRing *r1 = &mgr->ring[1];
    FireRing *r2 = &mgr->ring[2];
    s32 done = 0;
    f32 t;
    Vec v;
    Vec p;

    switch (mgr->sub) {
    case 0:
        FR_CIRCLES_INIT(mgr);
        fn_1_1E9C8(&lbl_1_bss_786C);
        p.x = r1->pos.x;
        p.y = 0.0f;
        p.z = r1->pos.z;
        fn_1_1E9F8(&lbl_1_bss_786C, &p);
        p.x = r2->pos.x;
        p.y = 0.0f;
        p.z = r1->pos.z;
        fn_1_1E9F8(&lbl_1_bss_786C, &p);
        p.x = r2->pos.x;
        p.y = 0.0f;
        p.z = r2->pos.z;
        fn_1_1E9F8(&lbl_1_bss_786C, &p);
        p.x = r1->pos.x;
        p.y = 0.0f;
        p.z = r2->pos.z;
        fn_1_1E9F8(&lbl_1_bss_786C, &p);
        p.x = r0->pos.x;
        p.y = 0.0f;
        p.z = r0->pos.z;
        fn_1_1E9F8(&lbl_1_bss_786C, &p);
        p.x = r0->pos.x;
        p.y = 0.0f;
        p.z = r1->pos.z;
        fn_1_1E9F8(&lbl_1_bss_786C, &p);
        p.x = r1->pos.x;
        p.y = 0.0f;
        p.z = r1->pos.z;
        fn_1_1E9F8(&lbl_1_bss_786C, &p);
        mgr->subTimer = 0;
        mgr->sub++;
        /* fallthrough */
    case 1:
        t = (f32)mgr->subTimer / 60.0f;
        r1->pos = *fn_1_1EBBC(&v, &lbl_1_bss_786C, 0, 2, t);
        r2->pos = *fn_1_1EBBC(&v, &lbl_1_bss_786C, 2, 3, t);
        r0->pos = *fn_1_1EBBC(&v, &lbl_1_bss_786C, 4, 4, t);
        r2->pos.y = 40.0f;
        r1->pos.y = 40.0f;
        r0->pos.y = 40.0f;
        if (++mgr->subTimer >= 0x3C) {
            mgr->subTimer = 0;
            mgr->sub++;
        }
        break;
    case 2:
        t = (f32)mgr->subTimer / 60.0f;
        r1->pos = *fn_1_1EBBC(&v, &lbl_1_bss_786C, 2, 2, t);
        r2->pos = *fn_1_1EBBC(&v, &lbl_1_bss_786C, 3, 4, t);
        r0->pos = *fn_1_1EBBC(&v, &lbl_1_bss_786C, 4, 5, t);
        r2->pos.y = 40.0f;
        r1->pos.y = 40.0f;
        r0->pos.y = 40.0f;
        if (++mgr->subTimer >= 0x3C) {
            mgr->subTimer = 0;
            mgr->sub++;
        }
        break;
    case 3:
        t = (f32)mgr->subTimer / 60.0f;
        r1->pos = *fn_1_1EBBC(&v, &lbl_1_bss_786C, 2, 2, t);
        r2->pos = *fn_1_1EBBC(&v, &lbl_1_bss_786C, 4, 4, t);
        r0->pos = *fn_1_1EBBC(&v, &lbl_1_bss_786C, 5, 7, t);
        r2->pos.y = 40.0f;
        r1->pos.y = 40.0f;
        r0->pos.y = 40.0f;
        if (++mgr->subTimer >= 0x3C) {
            mgr->subTimer = 0;
            mgr->sub++;
        }
        break;
    case 4:
        t = (f32)mgr->subTimer / 60.0f;
        r1->pos = *fn_1_1EBBC(&v, &lbl_1_bss_786C, 2, 4, t);
        r2->pos = *fn_1_1EBBC(&v, &lbl_1_bss_786C, 4, 7, t);
        r0->pos = *fn_1_1EBBC(&v, &lbl_1_bss_786C, 1, 2, t);
        r2->pos.y = 40.0f;
        r1->pos.y = 40.0f;
        r0->pos.y = 40.0f;
        if (++mgr->subTimer >= 0x3C) {
            mgr->subTimer = 0;
            mgr->sub++;
        }
        break;
    case 5:
        t = (f32)mgr->subTimer / 60.0f;
        r1->pos = *fn_1_1EBBC(&v, &lbl_1_bss_786C, 4, 6, t);
        r2->pos = *fn_1_1EBBC(&v, &lbl_1_bss_786C, 1, 2, t);
        r0->pos = *fn_1_1EBBC(&v, &lbl_1_bss_786C, 2, 4, t);
        r2->pos.y = 40.0f;
        r1->pos.y = 40.0f;
        r0->pos.y = 40.0f;
        if (++mgr->subTimer >= 0x3C) {
            mgr->subTimer = 0;
            mgr->sub = 0;
            done = 1;
        }
        break;
    }
    return done;
}

s32 fn_1_1CCC4(FireRingMgr *mgr) {
    FireRing *r0 = &mgr->ring[0];
    FireRing *r1 = &mgr->ring[1];
    FireRing *r2 = &mgr->ring[2];
    s32 done = 0;
    f32 t;

    switch (mgr->sub) {
    case 0:
        FR_CIRCLES_INIT(mgr);
        FR_SAVE_POS(mgr);
        mgr->subTimer = 0;
        mgr->sub++;
        /* fallthrough */
    case 1:
        t = (f32)mgr->subTimer / 75.0f;
        r0->radius = 380.0f;
        r1->radius = 380.0f + 420.0f * t;
        r2->radius = 380.0f;
        r1->pos.x = lbl_1_bss_7854.x + -lbl_1_bss_7854.x * t;
        r1->pos.z = lbl_1_bss_7854.z + -lbl_1_bss_7854.z * t;
        if (mgr->subTimer++ >= 0x4B) {
            mgr->subTimer = 0;
            mgr->sub++;
        }
        break;
    case 2:
        t = (f32)mgr->subTimer / 75.0f;
        r0->radius = 380.0f + 420.0f * t;
        r1->radius = 800.0f + -420.0f * t;
        r2->radius = 380.0f;
        r0->pos.x = lbl_1_bss_7860.x + -lbl_1_bss_7860.x * t;
        r0->pos.z = lbl_1_bss_7860.z + -lbl_1_bss_7860.z * t;
        r1->pos.x = lbl_1_bss_7854.x * t;
        r1->pos.z = lbl_1_bss_7854.z * t;
        if (mgr->subTimer++ >= 0x4B) {
            mgr->subTimer = 0;
            mgr->sub++;
        }
        break;
    case 3:
        t = (f32)mgr->subTimer / 75.0f;
        r0->radius = 800.0f + -420.0f * t;
        r1->radius = 380.0f;
        r2->radius = 380.0f + 420.0f * t;
        r0->pos.x = lbl_1_bss_7860.x * t;
        r0->pos.z = lbl_1_bss_7860.z * t;
        r2->pos.x = lbl_1_bss_7848.x + -lbl_1_bss_7848.x * t;
        r2->pos.z = lbl_1_bss_7848.z + -lbl_1_bss_7848.z * t;
        if (mgr->subTimer++ >= 0x4B) {
            mgr->subTimer = 0;
            mgr->sub++;
        }
        break;
    case 4:
        t = (f32)mgr->subTimer / 75.0f;
        r0->radius = 380.0f;
        r1->radius = 380.0f;
        r2->radius = 800.0f + -420.0f * t;
        r2->pos.x = lbl_1_bss_7848.x * t;
        r2->pos.z = lbl_1_bss_7848.z * t;
        if (mgr->subTimer++ >= 0x4B) {
            mgr->subTimer = 0;
            mgr->sub = 0;
            done = 1;
        }
        break;
    }
    return done;
}

s32 fn_1_1D320(FireRingMgr *mgr) {
    FireRing *r0 = &mgr->ring[0];
    FireRing *r1 = &mgr->ring[1];
    FireRing *r2 = &mgr->ring[2];
    s32 done = 0;
    f32 t;

    switch (mgr->sub) {
    case 0:
        FR_CIRCLES_INIT(mgr);
        FR_SAVE_POS(mgr);
        mgr->subTimer = 0;
        mgr->sub++;
        /* fallthrough */
    case 1:
        t = (f32)mgr->subTimer / 60.0f;
        r0->radius = 380.0f + 420.0f * t;
        r1->radius = 380.0f + 420.0f * t;
        r2->radius = 380.0f + 420.0f * t;
        r0->pos.x = lbl_1_bss_7860.x + -lbl_1_bss_7860.x * t;
        r0->pos.z = lbl_1_bss_7860.z + -lbl_1_bss_7860.z * t;
        r1->pos.x = lbl_1_bss_7854.x + -lbl_1_bss_7854.x * t;
        r1->pos.z = lbl_1_bss_7854.z + -lbl_1_bss_7854.z * t;
        r2->pos.x = lbl_1_bss_7848.x + -lbl_1_bss_7848.x * t;
        r2->pos.z = lbl_1_bss_7848.z + -lbl_1_bss_7848.z * t;
        if (mgr->subTimer++ >= 0x3C) {
            mgr->subTimer = 0;
            mgr->sub++;
        }
        break;
    case 2:
        t = (f32)mgr->subTimer / 60.0f;
        r1->radius = 800.0f + -610.0f * t;
        r1->pos.x = lbl_1_bss_7854.x * t;
        r1->pos.z = lbl_1_bss_7854.z * t;
        if (mgr->subTimer++ >= 0x3C) {
            mgr->subTimer = 0;
            mgr->sub++;
        }
        break;
    case 3:
        t = (f32)mgr->subTimer / 60.0f;
        r0->radius = 800.0f + -610.0f * t;
        r0->pos.x = lbl_1_bss_7860.x * t;
        r0->pos.z = lbl_1_bss_7860.z * t;
        r1->radius = 190.0f + 190.0f * t;
        if (mgr->subTimer++ >= 0x3C) {
            mgr->subTimer = 0;
            mgr->sub++;
        }
        break;
    case 4:
        t = (f32)mgr->subTimer / 60.0f;
        r2->radius = 800.0f + -610.0f * t;
        r2->pos.x = lbl_1_bss_7848.x * t;
        r2->pos.z = lbl_1_bss_7848.z * t;
        r0->radius = 190.0f + 190.0f * t;
        if (mgr->subTimer++ >= 0x3C) {
            mgr->subTimer = 0;
            mgr->sub++;
        }
        break;
    case 5:
        r2->radius = 190.0f + 190.0f * ((f32)mgr->subTimer / 60.0f);
        if (mgr->subTimer++ >= 0x3C) {
            mgr->subTimer = 0;
            mgr->sub = 0;
            done = 1;
        }
        break;
    }
    return done;
}

/* walk ring 2 along the path and let the other two follow one step behind */
#define FR_FOLLOW(mgr, from, to) \
    do { \
        Vec v_; \
        f32 t_ = (f32)(mgr)->subTimer / 20.0f; \
        FireRing *a_ = &(mgr)->ring[2]; \
        FireRing *b_ = &(mgr)->ring[1]; \
        FireRing *c_ = &(mgr)->ring[0]; \
        a_->pos = *fn_1_1EBBC(&v_, &lbl_1_bss_786C, from, to, t_); \
        b_->pos = a_->pos; \
        c_->pos = b_->pos; \
        a_->pos.y = 40.0f; \
        b_->pos.y = 40.0f; \
        c_->pos.y = 40.0f; \
        if (++(mgr)->subTimer >= 0x14) { \
            (mgr)->subTimer = 0; \
            (mgr)->sub++; \
        } \
    } while (0)

s32 fn_1_1D9F8(FireRingMgr *mgr) {
    FireRing *r0 = &mgr->ring[0];
    FireRing *r1 = &mgr->ring[1];
    FireRing *r2 = &mgr->ring[2];
    s32 done = 0;
    f32 t;
    f32 rad;
    Vec p;

    switch (mgr->sub) {
    case 0:
        FR_CIRCLES_INIT(mgr);
        fn_1_1E9C8(&lbl_1_bss_786C);
        p.x = 0.0f;
        p.y = 0.0f;
        p.z = 0.0f;
        fn_1_1E9F8(&lbl_1_bss_786C, &p);
        p.x = r2->pos.x;
        p.y = 0.0f;
        p.z = 0.0f;
        fn_1_1E9F8(&lbl_1_bss_786C, &p);
        p.x = r2->pos.x;
        p.y = 0.0f;
        p.z = r1->pos.z;
        fn_1_1E9F8(&lbl_1_bss_786C, &p);
        p.x = r1->pos.x;
        p.y = 0.0f;
        p.z = r1->pos.z;
        fn_1_1E9F8(&lbl_1_bss_786C, &p);
        p.x = r0->pos.x;
        p.y = 0.0f;
        p.z = r1->pos.z;
        fn_1_1E9F8(&lbl_1_bss_786C, &p);
        p.x = r0->pos.x;
        p.y = 0.0f;
        p.z = 0.0f;
        fn_1_1E9F8(&lbl_1_bss_786C, &p);
        p.x = r0->pos.x;
        p.y = 0.0f;
        p.z = r0->pos.z;
        fn_1_1E9F8(&lbl_1_bss_786C, &p);
        p.x = 0.0f;
        p.y = 0.0f;
        p.z = r0->pos.z;
        fn_1_1E9F8(&lbl_1_bss_786C, &p);
        p.x = r2->pos.x;
        p.y = 0.0f;
        p.z = r2->pos.z;
        fn_1_1E9F8(&lbl_1_bss_786C, &p);
        p.x = r2->pos.x;
        p.y = 0.0f;
        p.z = 0.0f;
        fn_1_1E9F8(&lbl_1_bss_786C, &p);
        p.x = 0.0f;
        p.y = 0.0f;
        p.z = 0.0f;
        fn_1_1E9F8(&lbl_1_bss_786C, &p);
        mgr->subTimer = 0;
        mgr->sub++;
        /* fallthrough */
    case 1:
        rad = 450.0f * (1.0f - (f32)mgr->subTimer / 50.0f);
        FR_CIRCLES(mgr, rad);
        if (mgr->subTimer++ >= 0x32) {
            mgr->subTimer = 0;
            mgr->sub++;
        }
        break;
    case 2:
        FR_FOLLOW(mgr, 0, 1);
        break;
    case 3:
        FR_FOLLOW(mgr, 1, 2);
        break;
    case 4:
        FR_FOLLOW(mgr, 2, 3);
        break;
    case 5:
        FR_FOLLOW(mgr, 3, 4);
        break;
    case 6:
        FR_FOLLOW(mgr, 4, 5);
        break;
    case 7:
        FR_FOLLOW(mgr, 5, 6);
        break;
    case 8:
        FR_FOLLOW(mgr, 6, 7);
        break;
    case 9:
        FR_FOLLOW(mgr, 7, 8);
        break;
    case 10:
        FR_FOLLOW(mgr, 8, 9);
        break;
    case 11:
        FR_FOLLOW(mgr, 9, 10);
        break;
    case 12:
        rad = 450.0f * ((f32)mgr->subTimer / 50.0f);
        FR_CIRCLES(mgr, rad);
        if (++mgr->subTimer >= 0x32) {
            mgr->subTimer = 0;
            mgr->sub = 0;
            done = 1;
        }
        break;
    }
    return done;
}

void fn_1_1E9C8(FireRingPath *path) {
    memset(path, 0, sizeof(FireRingPath));
}

void fn_1_1E9F8(FireRingPath *path, Vec *p) {
    if (path->count < 0xC) {
        path->pt[path->count] = *p;
        path->count++;
    }
}

void fn_1_1EA38(Vec *out, FireRingPath *path, f32 t) {
    Vec diff;
    Vec res;
    f32 step;
    s16 idx;

    if (t >= 1.0f) {
        t = fmod(t, 1.0);
    }
    step = 1.0f / (f32)(path->count - 1);
    idx = t / step;
    PSVECSubtract(&path->pt[idx + 1], &path->pt[idx], &diff);
    PSVECScale(&diff, &diff, (t - step * (f32)idx) / step);
    PSVECAdd(&path->pt[idx], &diff, &res);
    *out = res;
}

Vec *fn_1_1EBBC(Vec *out, FireRingPath *path, s16 from, s16 to, f32 t) {
    Vec diff;
    Vec res;
    f32 step;
    f32 start;
    f32 pos;
    s16 idx;

    step = 1.0f / (f32)(path->count - 1);
    start = (f32)from * step;
    pos = start + t * ((f32)to * step - start);
    if (pos >= 1.0f) {
        pos = fmod(pos, 1.0);
    }
    step = 1.0f / (f32)(path->count - 1);
    idx = pos / step;
    PSVECSubtract(&path->pt[idx + 1], &path->pt[idx], &diff);
    PSVECScale(&diff, &diff, (pos - step * (f32)idx) / step);
    PSVECAdd(&path->pt[idx], &diff, &res);
    *out = res;
    return out;
}
