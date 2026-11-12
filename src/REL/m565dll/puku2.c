#include "REL/m565dll.h"

static void fn_1_8E10(M565PukuItem *item, M565Puku *puku);
static void fn_1_907C(M565PukuItem *item);
static s32 fn_1_90F0(M565PukuItem *item);
static void fn_1_9AC8(M565PukuItem *item);

/* angle wrap written as an expression macro: the original re-evaluates atan2 in every branch */
#define M565_WRAP180(fv, dv) (((fv) > 180.0f) ? ((dv) - 360.0) : (((fv) <= -180.0f) ? (360.0 + (dv)) : (dv)))

static inline f32 M565Clamp01(f32 v)
{
    if (v > 1.0f) {
        return 1.0f;
    }
    if (v < 0.0f) {
        return 0.0f;
    }
    return v;
}

void fn_1_8AD0(M565Puku *puku, M565Sys *sys)
{
    s32 i;

    memset(puku, 0, sizeof(M565Puku));
    puku->sys = sys;
    for (i = 0; i < 5; i++) {
        fn_1_8E10(&puku->items[i], puku);
    }
}

void fn_1_8B44(M565Puku *puku)
{
    s32 i;

    for (i = 0; i < 5; i++) {
        fn_1_907C(&puku->items[i]);
    }
    memset(puku, 0, sizeof(M565Puku));
}

void fn_1_8BA8(M565Puku *puku)
{
    M565PukuItem *item;
    s32 i;

    for (i = 0; i < 5; i++) {
        item = &puku->items[i];
        if (fn_1_90F0(item) != 0 && item->active != 0) {
            Hu3DModelAttrSet(item->mdlA, 1);
            Hu3DModelAttrSet(item->mdlB, 1);
            item->active = 0;
            puku->cnt = puku->cnt - 1;
            if (item->big != 0) {
                puku->cntBig = puku->cntBig - 1;
            } else {
                puku->cntSmall = puku->cntSmall - 1;
            }
        }
    }
}

M565PukuItem *fn_1_8C70(M565Puku *puku, HuVecF *from, HuVecF *to, s32 big, f32 peak)
{
    HuVecF rot;
    M565PukuItem *item;
    s32 i;

    for (i = 0; i < 5; i++) {
        item = &puku->items[i];
        if (item->active == 0) {
            item->from.x = from->x;
            item->from.y = from->y;
            item->from.z = from->z;
            item->to.x = to->x;
            item->to.y = to->y;
            item->to.z = to->z;
            item->pos.x = from->x;
            item->pos.y = from->y;
            item->pos.z = from->z;
            rot.x = 0.0f;
            rot.y = 0.0f;
            rot.z = 0.0f;
            item->rot.x = rot.x;
            item->rot.y = rot.y;
            item->rot.z = rot.z;
            item->peak = peak;
            item->frame = 0;
            item->big = big;
            item->t30 = 40;
            item->t32 = 100;
            if (big != 0) {
                Hu3DModelAttrReset(item->mdlA, 1);
                Hu3DModelShadowSet(item->mdlA);
            } else {
                Hu3DModelAttrReset(item->mdlB, 1);
                Hu3DModelShadowSet(item->mdlB);
            }
            item->active = 1;
            fn_1_90F0(item);
            puku->cnt = puku->cnt + 1;
            if (big != 0) {
                puku->cntBig = puku->cntBig + 1;
            } else {
                puku->cntSmall = puku->cntSmall + 1;
            }
            puku->cntTotal = puku->cntTotal + 1;
            return item;
        }
    }
    return NULL;
}

static void fn_1_8E10(M565PukuItem *item, M565Puku *puku)
{
    HU3DMODELID mdl;

    memset(item, 0, sizeof(M565PukuItem));
    item->puku = puku;
    item->active = 0;
    mdl = Hu3DModelCreateData(0x78006B);
    item->mdlA = mdl;
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(mdl, 0.01f, 0.01f, 0.01f);
    Hu3DModelCameraSet(mdl, 1);
    Hu3DModelLayerSet(mdl, 4);
    Hu3DModelAttrSet(mdl, 1);
    item->mot = Hu3DJointMotionData(mdl, 0x780079);
    Hu3DMotionSet(mdl, item->mot);
    mdl = Hu3DModelCreateData(0x78006C);
    item->mdlB = mdl;
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(mdl, 0.01f, 0.01f, 0.01f);
    Hu3DModelCameraSet(mdl, 1);
    Hu3DModelLayerSet(mdl, 4);
    Hu3DModelAttrSet(mdl, 1);
    item->mot = Hu3DJointMotionData(mdl, 0x780079);
    Hu3DMotionSet(mdl, item->mot);
    Hu3DModelAttrSet(mdl, 0x40000001);
}

static void fn_1_907C(M565PukuItem *item)
{
    s32 i;
    s16 *mot = &item->mot;

    for (i = 0; i < 1; i++) {
        Hu3DMotionKill(mot[i]);
    }
    Hu3DModelKill(item->mdlB);
    Hu3DModelKill(item->mdlA);
    memset(item, 0, sizeof(M565PukuItem));
}

static s32 fn_1_90F0(M565PukuItem *item)
{
    HuVecF dir;
    HuVecF npos;
    HuVecF hit;
    M565Sys *sys = item->puku->sys;
    M565Stage *stage = &sys->stage;
    f32 t;
    f32 ratio;
    f32 u;
    f32 k;
    f32 ang;
    f32 cube;
    f32 scale;
    f32 s;
    f32 num;
    f32 den;
    f32 span;

    if (item->active == 0) {
        return 0;
    }
    PSVECSubtract(&item->to, &item->from, &dir);
    dir.y = 0.0f;
    t = (f32)item->frame / (f32)(item->t32 + item->t30 * 2);
    if (item->pos.y > 0.0f) {
        ratio = item->pos.y / item->peak;
    } else {
        ratio = 0.0f;
    }
    if (item->frame < item->t30) {
        u = 0.0f;
    } else if (item->frame > item->t30 + item->t32) {
        u = 1.0f;
    } else {
        u = (f32)(item->frame - item->t30) / (f32)item->t32;
    }
    t = M565Clamp01(t);
    ratio = M565Clamp01(ratio);
    u = M565Clamp01(u);
    if (t < 0.5f) {
        npos.y = item->from.y + (f64)(item->peak - item->from.y) * sin(3.141592653589793 * (90.0f * (t / 0.5f)) / 180.0);
        k = t * ratio;
        npos.x = item->from.x + dir.x * k;
        npos.z = item->from.z + dir.z * k;
    } else {
        npos.y = item->to.y + (f64)(item->peak - item->to.y)
            * sin(3.141592653589793 * (90.0f + 90.0f * ((t - 0.5f) / 0.5f)) / 180.0);
        k = 0.5f + (0.5f - (1.0f - t) * ratio);
        npos.x = item->from.x + dir.x * k;
        npos.z = item->from.z + dir.z * k;
    }
    if (t < 0.5f) {
        if (item->from.y >= 0.0f) {
            den = item->from.y;
        } else {
            den = -item->from.y;
        }
        if (item->pos.y >= 0.0f) {
            num = item->pos.y;
        } else {
            num = -item->pos.y;
        }
        span = num / den;
    } else {
        if (item->to.y >= 0.0f) {
            den = item->to.y;
        } else {
            den = -item->to.y;
        }
        if (item->pos.y >= 0.0f) {
            num = item->pos.y;
        } else {
            num = -item->pos.y;
        }
        span = num / den;
    }
    if (item->pos.y < 0.0f) {
        s = span;
    } else {
        s = 0.0f;
    }
    cube = M565Clamp01(s * (s * s));
    s = 1.0f - cube;
    if (item->big != 0) {
        scale = 3.0f * s;
    } else {
        scale = 1.3f * s;
    }
    Hu3DModelScaleSet(item->mdlA, scale, scale, scale);
    Hu3DModelScaleSet(item->mdlB, scale, scale, scale);
    if (t >= 0.5f) {
        if (npos.y <= 0.0f && item->frozen == 0) {
            hit.x = npos.x;
            hit.y = npos.y;
            hit.z = npos.z;
            fn_1_5EB0(stage, &hit, item->big, NULL);
            if (sys->flag3824 != 0) {
                Hu3DMotionSpeedSet(sys->mdl, 1.0f);
                sys->flag3824 = 0;
            }
        }
        if (npos.y < 65.0f) {
            Hu3DModelShadowReset(item->mdlA);
            Hu3DModelShadowReset(item->mdlB);
        }
    }
    if (item->frozen == 0) {
        item->pos.x = npos.x;
        item->pos.y = npos.y;
        item->pos.z = npos.z;
        Hu3DModelPosSet(item->mdlA, item->pos.x, item->pos.y, item->pos.z);
        Hu3DModelPosSet(item->mdlB, item->pos.x, item->pos.y, item->pos.z);
    }
    PSVECNormalize(&dir, &dir);
    item->rot.y = 180.0 * (atan2(dir.x, dir.z) / 3.141592653589793);
    if (item->frozen == 0) {
        item->rot.x = -90.0f + 180.0f * u;
    }
    Hu3DModelRotSet(item->mdlA, item->rot.x, item->rot.y, item->rot.z);
    Hu3DModelRotSet(item->mdlB, item->rot.x, item->rot.y, item->rot.z);
    if (item->frozen == 0) {
        fn_1_9AC8(item);
    }
    if (item->frozen != 0) {
        Hu3DMotionSpeedSet(item->mdlA, 0.0f);
        Hu3DMotionSpeedSet(item->mdlB, 0.0f);
        if (item->big != 0) {
            if ((item->blink & 1) != 0) {
                Hu3DModelAttrSet(item->mdlA, 1);
            } else {
                Hu3DModelAttrReset(item->mdlA, 1);
            }
        } else if ((item->blink & 1) != 0) {
            Hu3DModelAttrSet(item->mdlB, 1);
        } else {
            Hu3DModelAttrReset(item->mdlB, 1);
        }
        item->blink = item->blink - 1;
        if (item->blink == 0) {
            return 1;
        }
    }
    item->frame = item->frame + 1;
    if (item->frame >= item->t32 + item->t30 * 2) {
        return 1;
    }
    return 0;
}

static void fn_1_9AC8(M565PukuItem *item)
{
    HuVecF diff;
    HuVecF mid;
    f32 vol;
    f32 pan;
    f32 dist;
    f32 reach;
    f32 ang;
    M565Sys *sys = item->puku->sys;
    M565Game *game = sys->game;
    M565Player *pl;
    s32 seNo;
    s32 i;

    for (i = 0; i < 4; i++) {
        pl = &sys->players[i];
        PSVECSubtract(pl->pos, &item->pos, &diff);
        dist = PSVECMag(&diff);
        if (pl->fallen == 0 && pl->mg->stunTime <= 0.0f) {
            if (item->big != 0) {
                reach = 150.0f;
            } else {
                reach = 65.0f;
            }
            if (dist < reach) {
                diff.y = 0.0f;
                ang = 0.0f;
                if (diff.x == 0.0f && diff.z == 0.0f) {
                    PSVECNormalize(&diff, &diff);
                    ang = M565_WRAP180((f32)(180.0 * (atan2(diff.x, diff.z) / 3.141592653589793)),
                                      180.0 * (atan2(diff.x, diff.z) / 3.141592653589793));
                }
                fn_1_89D4(pl, 1, ang);
                if (game->enable != 0 && pl->fallen == 0) {
                    omVibrate(pl->no, 30, 12, 0);
                }
                mid.x = 0.5f * (pl->pos->x + item->pos.x);
                mid.y = 0.5f * (pl->pos->y + item->pos.y);
                mid.z = 0.5f * (pl->pos->z + item->pos.z);
                Hu3DModelPosSet(pl->fxMdl, mid.x, mid.y, mid.z);
                if (MgSeqModeGet() == 5) {
                    fn_1_A10(pl->pos, &vol, &pan);
                    seNo = fn_1_C18(0x8DC);
                    fn_1_CC8(seNo, vol);
                    fn_1_C98(seNo, pan);
                }
            }
        }
    }
}
