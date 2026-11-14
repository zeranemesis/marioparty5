// m562dll/mini_mechakoopa - the swarm of mini mecha-koopas (34 slots) that
// rushes out of Bowser's head or hand, with the shared glow light and sound.
#include <humath.h>
#include <math.h>
#include <string.h>

#include "REL/m562dll.h"
#include "datadir_enum.h"
#include "game/hsfex.h"

/* the head node of each boss body, passed to Hu3DModelObjMtxGet */
char lbl_1_data_A14[14] = "g000m0-ns_hed";
char lbl_1_data_A22[14] = "g000mSP-head";

/* the glow light's position and direction, and its base colour */
HuVecF lbl_1_data_9F8 = {0.0f, 1200.0f, 0.0f};
HuVecF lbl_1_data_A04 = {0.0f, -1.0f, 0.01f};
GXColor lbl_1_data_A10 = {0xFF, 0x30, 0x00, 0xFF};

/* spawn counter: even / odd decides which model of the pair shows */
s16 lbl_1_bss_7840;

static const Vec kZeroSound = {0.0f, 0.0f, 0.0f};
static const Vec kZeroSum = {0.0f, 0.0f, 0.0f};

#define CLAMP01(v) ((v) > 1.0f ? 1.0f : ((v) < 0.0f ? 0.0f : (v)))
#define CLAMP_RANGE(v, lo, hi) ((v) > (hi) ? (hi) : ((v) < (lo) ? (lo) : (v)))

void fn_1_182B8(KbMiniEntry *entry, KbMiniMgr *mgr);
void fn_1_1848C(KbMiniEntry *entry);
s32 fn_1_184D4(KbMiniEntry *entry);
void fn_1_18D28(KbMiniEntry *entry, Vec *pos, s32 flip, f32 angle);

// Creates the two shared models (body and shadow) with their motions, builds
// all 34 entries and marks the light and sound handle as unused.
void fn_1_177EC(KbMiniMgr *mgr, M562Work *host)
{
    s32 i;
    HU3DMODELID model;

    memset(mgr, 0, sizeof(KbMiniMgr));
    mgr->host = host;

    model = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m562, 0x18), HU_MEMNUM_OVL, HEAP_MODEL));
    mgr->modelA = model;
    Hu3DModelAttrSet(model, 1);
    mgr->motionA = Hu3DJointMotion(model, HuDataSelHeapReadNum(DATANUM(DATA_m562, 0x3B), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DMotionSet(model, mgr->motionA);
    Hu3DMotionSpeedSet(model, 1.0f);
    Hu3DModelAttrReset(model, HU3D_MOTATTR_LOOP);

    model = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m562, 0x19), HU_MEMNUM_OVL, HEAP_MODEL));
    mgr->modelB = model;
    Hu3DModelAttrSet(model, 1);
    mgr->motionB = Hu3DJointMotion(model, HuDataSelHeapReadNum(DATANUM(DATA_m562, 0x3C), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DMotionSet(model, mgr->motionB);
    Hu3DMotionSpeedSet(model, 1.0f);
    Hu3DModelAttrReset(model, HU3D_MOTATTR_LOOP);

    for (i = 0; i < 34; i++) {
        fn_1_182B8(&mgr->entry[i], mgr);
    }
    mgr->lightId = -1;
    mgr->sndId = -1;
}

// Tears down the light, every entry, the shared models and motions.
void fn_1_17984(KbMiniMgr *mgr)
{
    s32 i;

    if (mgr->lightId >= 0) {
        Hu3DGLightKill(mgr->lightId);
    }
    for (i = 0; i < 34; i++) {
        fn_1_1848C(&mgr->entry[i]);
    }
    Hu3DMotionKill(mgr->motionB);
    Hu3DMotionKill(mgr->motionA);
    Hu3DModelKill(mgr->modelB);
    Hu3DModelKill(mgr->modelA);
    memset(mgr, 0, sizeof(KbMiniMgr));
}

// Per-frame update: start / stop the swarm, track the head angle, advance
// every live entry while summing their positions (the loudest, i.e. oldest,
// one places the sound), then drive the sound and the pulsing glow light.
void fn_1_17A28(KbMiniMgr *mgr)
{
    M562Work *host = mgr->host;
    Koopa *koopa = &host->koopa;
    KoopaBoss *boss = &host->boss;
    Mtx mtx;
    Vec dir;
    Vec first;
    Vec sound;
    Vec sum;
    Vec lightPos;
    GXColor color;
    f32 angle;
    f32 level;
    f64 deg;
    f64 wrapped;
    s16 oldest;
    s32 i;
    KbMiniEntry *entry;

    angle = 0.0f;
    if (mgr->count == 0 && mgr->sndId >= 0) {
        fn_1_83C(mgr->sndId);
        mgr->sndId = -1;
    }
    if (mgr->active != 0 && mgr->started == 0) {
        mgr->started = 1;
    } else if (mgr->active == 0 && mgr->started != 0) {
        if (mgr->count != 0) {
            for (i = 0; i < 34; i++) {
                entry = &mgr->entry[i];
                if (entry->active != 0) {
                    Hu3DModelAttrSet(entry->modelA, 1);
                    Hu3DModelAttrSet(entry->modelB, 1);
                    entry->active = 0;
                }
            }
            mgr->count = 0;
        }
        if (mgr->lightId >= 0) {
            Hu3DGLightKill(mgr->lightId);
            mgr->lightId = -1;
        }
        mgr->started = 0;
    }
    if (mgr->started == 0) {
        return;
    }

    memset(mtx, 0, sizeof(mtx));
    if (host->mode == 3) {
        Hu3DModelObjMtxGet(koopa->modelId, lbl_1_data_A14, mtx);
        angle = koopa->rotY;
    } else if (boss->host != NULL) {
        Hu3DModelObjMtxGet(boss->modelId, lbl_1_data_A22, mtx);
        angle = *(f32 *)&boss->motionId[15];
    }
    dir.x = mtx[0][0];
    dir.y = 0.0f;
    dir.z = mtx[2][0];
    if (dir.x != 0.0f || dir.z != 0.0f) {
        PSVECNormalize(&dir, &dir);
        deg = HuAtan(dir.x, dir.z);
        if (deg > 180.0) {
            wrapped = deg - 360.0;
        } else if (deg <= -180.0) {
            wrapped = 360.0 + deg;
        } else {
            wrapped = deg;
        }
        angle = wrapped;
    }
    mgr->headAngle = angle;

    first = kZeroSound;
    mgr->soundPos = first;
    oldest = 0;
    sum = kZeroSum;
    for (i = 0; i < 34; i++) {
        entry = &mgr->entry[i];
        if (entry->active != 0) {
            fn_1_184D4(entry);
            sum.x += entry->pos.x;
            sum.y += entry->pos.y;
            sum.z += entry->pos.z;
            if (oldest < entry->timer) {
                oldest = entry->timer;
                mgr->soundPos = entry->pos;
            }
        }
    }

    if (mgr->count != 0) {
        if (mgr->sndId < 0) {
            if (host->mode == 3) {
                mgr->sndId = fn_1_934(0x8A5, &mgr->soundPos);
            } else {
                mgr->sndId = fn_1_934(0x8AD, &mgr->soundPos);
            }
        } else {
            fn_1_964(mgr->sndId, &mgr->soundPos);
        }
    }

    if (mgr->count != 0) {
        color = lbl_1_data_A10;
        if (mgr->lightId < 0) {
            mgr->lightId = Hu3DGLightCreateV(&lbl_1_data_9F8, &lbl_1_data_A04, &lbl_1_data_A10);
            Hu3DGLightStaticSet(mgr->lightId, 1);
            Hu3DGLightSpotSet(mgr->lightId, GX_SP_COS, 30.0f);
            Hu3DGLightInfinitytSet(mgr->lightId);
        }
        if (mgr->lightId >= 0) {
            lightPos.x = sum.x / mgr->count;
            lightPos.y = 1000.0f;
            lightPos.z = sum.z / mgr->count;
            level = (0.699999988079071 + 0.30000001192092896 * HuSin(180.0f * ((host->frame % 4) * 0.25f)));
            level *= (f32)mgr->count / 34.0f;
            color.r = (u8)(lbl_1_data_A10.r * level);
            color.g = (u8)(lbl_1_data_A10.g * level);
            color.b = (u8)(lbl_1_data_A10.b * level);
            color.a = 0xFF;
            Hu3DGLightPosSetV(mgr->lightId, (HuVecF *)&lightPos, &lbl_1_data_A04);
            Hu3DGLightColorSet(mgr->lightId, color.r, color.g, color.b, color.a);
        }
    } else if (mgr->lightId >= 0) {
        Hu3DGLightColorSet(mgr->lightId, 0, 0, 0, 0);
        Hu3DGLightKill(mgr->lightId);
        mgr->lightId = -1;
    }
}

// Starts the first free entry at pos (alternating between the two model
// variants) and returns it, or NULL when all 34 are in flight.
KbMiniEntry *fn_1_181FC(KbMiniMgr *mgr, Vec *pos, f32 angle)
{
    s32 i;
    KbMiniEntry *entry;
    KbMiniEntry *found;

    found = NULL;
    for (i = 0; i < 34; i++) {
        entry = &mgr->entry[i];
        if (entry->active != 0) {
            continue;
        }
        fn_1_18D28(entry, pos, lbl_1_bss_7840 & 1, angle);
        lbl_1_bss_7840++;
        found = entry;
        break;
    }
    return (KbMiniEntry *)found;
}

// Links the two shared models into one entry: hidden, at the origin, on the
// stage camera, layer 4, playing the shared motion once.
void fn_1_182B8(KbMiniEntry *entry, KbMiniMgr *mgr)
{
    CAMERA *cam = &mgr->host->camera[0];
    HU3DMODELID model;

    memset(entry, 0, sizeof(KbMiniEntry));
    entry->mgr = mgr;

    model = Hu3DModelLink(mgr->modelA);
    entry->modelA = model;
    Hu3DModelPosSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(model, cam->camMask);
    Hu3DModelLayerSet(model, 4);
    Hu3DModelAttrSet(model, 1);
    Hu3DMotionSet(model, mgr->motionA);
    Hu3DMotionSpeedSet(model, 1.0f);
    Hu3DModelAttrReset(model, HU3D_MOTATTR_LOOP);

    model = Hu3DModelLink(mgr->modelB);
    entry->modelB = model;
    Hu3DModelPosSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(model, cam->camMask);
    Hu3DModelLayerSet(model, 4);
    Hu3DModelAttrSet(model, 1);
    Hu3DMotionSet(model, mgr->motionB);
    Hu3DMotionSpeedSet(model, 1.0f);
    Hu3DModelAttrReset(model, HU3D_MOTATTR_LOOP);
}

void fn_1_1848C(KbMiniEntry *entry)
{
    Hu3DModelKill(entry->modelB);
    Hu3DModelKill(entry->modelA);
    memset(entry, 0, sizeof(KbMiniEntry));
}

// One entry's flight: it grows over 32 frames while sweeping from the origin
// along its heading (stage mode 3 blends towards the head's heading and
// arcs up from the head), registers a hit sphere, and frees itself at the end.
// Returns 1 when the entry just finished (or was already inactive).
s32 fn_1_184D4(KbMiniEntry *entry)
{
    KbMiniMgr *mgr = entry->mgr;
    M562Work *host = mgr->host;
    Vec dirA;
    Vec posA;
    Vec posB;
    Vec pos;
    f32 reach;
    f32 baseY;
    f32 t;
    f32 tt;
    f32 dist;
    f32 v;
    f32 scale;
    s32 done;

    done = 0;
    if (entry->active == 0) {
        Hu3DModelAttrSet(entry->modelA, 1);
        Hu3DModelAttrSet(entry->modelB, 1);
        return 1;
    }
    if (host->mode == 3) {
        reach = 500.0f;
        baseY = 130.0f;
    } else {
        reach = 1800.0f;
        baseY = 150.0f;
    }
    t = (f32)entry->timer / (f32)32;
    t = CLAMP01(t);

    if (host->mode == 3) {
        dist = reach * t;
        dirA.x = sin((M_PI * entry->angle) / 180.0);
        dirA.y = 0.0f;
        dirA.z = cos((M_PI * entry->angle) / 180.0);
        PSVECScale(&dirA, &dirA, dist);
        PSVECAdd(&entry->origin, &dirA, &posA);
        dirA.x = sin((M_PI * mgr->headAngle) / 180.0);
        dirA.y = 0.0f;
        dirA.z = cos((M_PI * mgr->headAngle) / 180.0);
        PSVECScale(&dirA, &dirA, dist);
        PSVECAdd(&entry->origin, &dirA, &posB);
        tt = t * t;
        pos.x = posA.x * tt + posB.x * (1.0f - tt);
        pos.z = posA.z * tt + posB.z * (1.0f - tt);
        v = 90.0f + 270.0f * t;
        v = CLAMP_RANGE(v, 90.0f, 270.0f);
        pos.y = 0.5 * (1.0 + HuSin(v));
        pos.y = baseY + pos.y * (entry->origin.y - baseY);
        scale = 6.0f * (0.05f + 0.95f * (t * t));
    } else {
        dirA.x = sin((M_PI * entry->angle) / 180.0);
        dirA.y = 0.0f;
        dirA.z = cos((M_PI * entry->angle) / 180.0);
        PSVECScale(&dirA, &dirA, reach * t);
        PSVECAdd(&entry->origin, &dirA, &pos);
        v = 90.0f + 380.0f * t;
        v = CLAMP_RANGE(v, 90.0f, 270.0f);
        pos.y = 0.5 * (1.0 + HuSin(v));
        pos.y = baseY + pos.y * ((entry->origin.y - 50.0f) - baseY);
        scale = 8.0f * (0.15f + 0.85f * t);
    }
    entry->pos.x = pos.x;
    entry->pos.y = pos.y;
    entry->pos.z = pos.z;
    Hu3DModelPosSet(entry->modelA, pos.x, pos.y, pos.z);
    Hu3DModelPosSet(entry->modelB, pos.x, pos.y, pos.z);
    Hu3DModelScaleSet(entry->modelA, scale, scale, scale);
    Hu3DModelScaleSet(entry->modelB, scale, scale, scale);
    if (entry->flip != 0) {
        Hu3DModelAttrReset(entry->modelA, 1);
        Hu3DModelAttrSet(entry->modelB, 1);
    } else {
        Hu3DModelAttrSet(entry->modelA, 1);
        Hu3DModelAttrReset(entry->modelB, 1);
    }
    if (host->mode == 3) {
        fn_1_2CE8C(KP_HITA(host), 4, 0, &pos, NULL, 50.0f * (0.2f + 0.8f * t));
    } else if (host->mode == 4) {
        fn_1_2CE8C(KP_HITB(host), 4, 0, &pos, NULL, 250.0f * (0.1f + 0.9f * t));
        pos.y = 50.0f;
        fn_1_2CE8C(KP_HITB(host), 4, 0, &pos, NULL, 250.0f * (0.1f + 0.9f * t));
    }
    entry->timer++;
    if (entry->timer >= 32) {
        Hu3DModelAttrSet(entry->modelA, 1);
        Hu3DModelAttrSet(entry->modelB, 1);
        entry->active = 0;
        mgr->count--;
        done = 1;
    }
    return done;
}

// Arms an inactive entry at pos: origin and position, heading, which variant
// shows, models parked there at scale 0 with their motions rewound.
void fn_1_18D28(KbMiniEntry *entry, Vec *pos, s32 flip, f32 angle)
{
    KbMiniMgr *mgr = entry->mgr;

    if (entry->active == 0) {
        entry->pos.x = pos->x;
        entry->pos.y = pos->y;
        entry->pos.z = pos->z;
        entry->origin.x = entry->pos.x;
        entry->origin.y = entry->pos.y;
        entry->origin.z = entry->pos.z;
        entry->angle = angle;
        entry->active = 1;
        entry->flip = flip;
        Hu3DModelPosSet(entry->modelA, pos->x, pos->y, pos->z);
        Hu3DModelPosSet(entry->modelB, pos->x, pos->y, pos->z);
        Hu3DModelScaleSet(entry->modelA, 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(entry->modelB, 0.0f, 0.0f, 0.0f);
        Hu3DMotionTimeSet(entry->modelA, 0.0f);
        Hu3DMotionTimeSet(entry->modelB, 0.0f);
        entry->timer = 0;
        mgr->count++;
    }
}
