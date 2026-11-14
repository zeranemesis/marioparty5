#include "REL/m562dll.h"
#include "game/sprite.h"
#include <math.h>
#include <string.h>

/* one 0x38 byte particle parameter block (copied into the particle work area) */
typedef struct EffectParam {
    u32 flags;
    u8 colStart[4];
    u8 colEnd[4];
    Vec vel;
    Vec damp;
    f32 gravity;
    f32 unk28;
    f32 scaleStep;
    f32 alphaStep;
    f32 colorBlend;
} EffectParam;

/* sweep-and-prune node */
typedef struct EffectNode {
    u16 type; /* 0 = begin, 1 = end */
    f32 val;
    EffectEntry *entry;
} EffectNode;

typedef struct EffectNodeList {
    u16 count;
    EffectNode *nodes;
} EffectNodeList;

extern void fn_1_2D234(u16 axis, EffectNodeList *out, EffectList *in);
extern void fn_1_2D3F0(EffectNodeList *list);
extern void fn_1_2D434(EffectNode *nodes, s16 left, s16 right);
extern s16 fn_1_2D8FC(EffectNode *nodes, s16 left, s16 right);
extern void fn_1_2ED2C(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);

/* .data */
static EffectParam lbl_1_data_FF8 = {
    0,
    { 0xA0, 0xA0, 0xA0, 0xFF },
    { 0x00, 0x00, 0x00, 0xFF },
    { 0.0f, 2.0f, 1.0f },
    { 0.95f, 0.99f, 0.95f },
    0.0f,
    0.0f,
    1.0f,
    -4.0f,
    0.0f,
};

/* .bss */
s16 lbl_1_bss_79F8[0x10];
static EffectParam *lbl_1_bss_7A18;

void fn_1_2CD6C(EffectList *list, s16 max) {
    memset(list, 0, sizeof(*list));
    list->count = 0;
    list->max = max;
    list->entries = HuMemDirectMallocNum(HEAP_HEAP, max * sizeof(EffectEntry), 0x10000000);
    memset(list->entries, 0, max * sizeof(EffectEntry));
}

void fn_1_2CDE8(EffectList *list) {
    if (list->entries != NULL) {
        HuMemDirectFree(list->entries);
        list->count = 0;
    }
    memset(list, 0, sizeof(*list));
}

void fn_1_2CE3C(EffectList *list, EffectHitFunc func) {
    list->hitFunc = func;
}

void fn_1_2CE44(EffectList *list) {
    fn_1_2CEF4(list, list, 2, 1, 0);
    list->count = 0;
}

void fn_1_2CE8C(EffectList *list, s16 type, u32 flag, Vec *pos, void *user, f32 radius) {
    EffectEntry *e;

    if (list->count < list->max) {
        e = &list->entries[list->count];
        e->type = type;
        e->user = user;
        e->pos = *pos;
        e->radius = radius;
        e->flag = flag;
        list->count++;
    }
}

void fn_1_2CEF4(EffectList *list, EffectList *arr, u16 axis, u16 a, u16 b) {
    EffectNodeList sweep;
    EffectList sub;
    Vec diff;
    EffectEntry *e1, *e2;
    s32 i, j;
    u16 active = 0;
    u16 movable = 0;
    u16 nextA = a;
    u16 nextB = b;
    s32 more = 0;
    s32 fixed = 0;
    EffectNode *n;

    if (arr->count < 10 || axis == 3) {
        if (list->hitFunc != NULL) {
            for (i = 0; i < arr->count; i++) {
                for (j = i + 1; j < arr->count; j++) {
                    e1 = &arr->entries[i];
                    e2 = &arr->entries[j];
                    PSVECSubtract(&e1->pos, &e2->pos, &diff);
                    if (PSVECMag(&diff) <= e1->radius + e2->radius) {
                        list->hitFunc(e1, e2);
                    }
                }
            }
        }
        return;
    }
    fn_1_2D234(axis, &sweep, arr);
    fn_1_2D3F0(&sweep);
    memset(&sub, 0, sizeof(sub));
    sub.entries = HuMemDirectMallocNum(HEAP_HEAP, arr->count * sizeof(EffectEntry), 0x10000000);
    memset(sub.entries, 0, arr->count * sizeof(EffectEntry));
    for (i = 0; i < sweep.count; i++) {
        n = &sweep.nodes[i];
        if (n->type == 0) {
            active++;
            if (sub.count < arr->count) {
                sub.entries[sub.count] = *n->entry;
                sub.count++;
            }
            if (n->entry->flag != 0) {
                fixed++;
            } else {
                movable++;
            }
        } else {
            active--;
            if (movable == 0) {
                sub.count--;
            }
            if (n->entry->flag != 0) {
                fixed--;
            } else {
                movable--;
            }
            if (active == 0) {
                if (i < sweep.count - 1) {
                    more = 1;
                }
                if (more != 0) {
                    switch (axis) {
                    case 2:
                        nextA = 1;
                        nextB = 0;
                        break;
                    case 1:
                        nextA = 2;
                        nextB = 0;
                        break;
                    case 0:
                        nextA = 2;
                        nextB = 1;
                        break;
                    }
                }
                if (sub.count != 0) {
                    fn_1_2CEF4(list, &sub, nextA, nextB, 3);
                }
                sub.count = 0;
            }
        }
    }
    if (sub.entries != NULL) {
        HuMemDirectFree(sub.entries);
        sub.count = 0;
    }
    if (sweep.nodes != NULL) {
        HuMemDirectFree(sweep.nodes);
        sweep.count = 0;
    }
}

void fn_1_2D234(u16 axis, EffectNodeList *out, EffectList *in) {
    u16 i;
    EffectEntry *e;
    f32 radius;
    f32 val;

    out->count = in->count * 2;
    out->nodes = HuMemDirectMallocNum(HEAP_HEAP, out->count * sizeof(EffectNode), 0x10000000);
    memset(out->nodes, 0, out->count * sizeof(EffectNode));
    for (i = 0; i < in->count; i++) {
        e = &in->entries[i];
        radius = e->radius;
        if (axis == 0) {
            val = e->pos.x;
        } else if (axis == 1) {
            val = e->pos.y;
        } else {
            val = e->pos.z;
        }
        out->nodes[i * 2].type = 0;
        out->nodes[i * 2].val = val - radius;
        out->nodes[i * 2].entry = e;
        out->nodes[i * 2 + 1].type = 1;
        out->nodes[i * 2 + 1].val = val + radius;
        out->nodes[i * 2 + 1].entry = e;
    }
}

void fn_1_2D3F0(EffectNodeList *list) {
    fn_1_2D434(list->nodes, 0, (s16) (list->count - 1));
}

/* quicksort of the sweep nodes by val */
void fn_1_2D434(EffectNode *nodes, s16 left, s16 right) {
    s16 p;

    if (left < right) {
        p = fn_1_2D8FC(nodes, left, right);
        if (left < (s16) (p - 1)) {
            fn_1_2D434(nodes, left, (s16) (p - 1));
        }
        if ((s16) (p + 1) < right) {
            fn_1_2D434(nodes, (s16) (p + 1), right);
        }
    }
}

s16 fn_1_2D8FC(EffectNode *nodes, s16 left, s16 right) {
    s16 i = left - 1;
    s16 j = right;
    f32 pivot = nodes[right].val;
    EffectNode tmp;

    for (;;) {
        do {
            i++;
        } while (nodes[i].val < pivot);
        do {
            j--;
        } while (i < j && pivot < nodes[j].val);
        if (i < j) {
            tmp = nodes[i];
            nodes[i] = nodes[j];
            nodes[j] = tmp;
        } else {
            break;
        }
    }
    tmp = nodes[i];
    nodes[i] = nodes[right];
    nodes[right] = tmp;
    return i;
}

void fn_1_2DB24(void) {
    s32 i;

    for (i = 0; i < 0x10; i++) {
        lbl_1_bss_79F8[i] = -1;
    }
    lbl_1_bss_7A18 = NULL;
}

void fn_1_2DB74(void) {
    s32 i;

    if (lbl_1_bss_7A18 != NULL) {
        HuMemDirectFree(lbl_1_bss_7A18);
        lbl_1_bss_7A18 = NULL;
    }
    for (i = 0; i < 0x10; i++) {
        lbl_1_bss_79F8[i] = -1;
    }
    lbl_1_bss_7A18 = NULL;
}

void fn_1_2DC08(s16 camMask, Vec *pos, s16 kind) {
    s16 i;
    f32 scale = 1.5f;

    switch (kind) {
    case 2:
        for (i = 0; i < 0x10; i++) {
            lbl_1_data_FF8.vel.x = scale * (28.0 * sin(M_PI * (f64) (270.0f + 22.5f * (f32) i) / 180.0));
            lbl_1_data_FF8.vel.y = 3.0f;
            lbl_1_data_FF8.vel.z = scale * (28.0 * cos(M_PI * (f64) (270.0f + 22.5f * (f32) i) / 180.0));
            fn_1_2E600(camMask, &lbl_1_data_FF8, pos->x, pos->y * 1.5f, pos->z, 240.0f);
        }
        for (i = 0; i < 0x10; i++) {
            lbl_1_data_FF8.vel.x = scale * (22.0 * sin(M_PI * (270.0 + (22.5 + (f64) (22.5f * (f32) i))) / 180.0));
            lbl_1_data_FF8.vel.y = 3.0f;
            lbl_1_data_FF8.vel.z = scale * (22.0 * cos(M_PI * (270.0 + (22.5 + (f64) (22.5f * (f32) i))) / 180.0));
            fn_1_2E600(camMask, &lbl_1_data_FF8, pos->x, pos->y * 1.5f, pos->z, 240.0f);
        }
        break;
    case 0:
        for (i = 0; i < 8; i++) {
            lbl_1_data_FF8.vel.x = scale * (8.0 * sin(M_PI * (f64) (270.0f + 45.0f * (f32) i) / 180.0));
            lbl_1_data_FF8.vel.y = 2.0f;
            lbl_1_data_FF8.vel.z = scale * (8.0 * cos(M_PI * (f64) (270.0f + 45.0f * (f32) i) / 180.0));
            fn_1_2E600(camMask, &lbl_1_data_FF8, pos->x, pos->y * 1.5f, pos->z, 100.0f);
        }
        for (i = 0; i < 8; i++) {
            lbl_1_data_FF8.vel.x = scale * (6.0 * sin(M_PI * (270.0 + (22.5 + (f64) (45.0f * (f32) i))) / 180.0));
            lbl_1_data_FF8.vel.y = 2.0f;
            lbl_1_data_FF8.vel.z = scale * (6.0 * cos(M_PI * (270.0 + (22.5 + (f64) (45.0f * (f32) i))) / 180.0));
            fn_1_2E600(camMask, &lbl_1_data_FF8, pos->x, pos->y * 1.5f, pos->z, 100.0f);
        }
        break;
    case 1:
        lbl_1_data_FF8.colStart[3] = 0x80;
        for (i = 0; i < 8; i++) {
            lbl_1_data_FF8.vel.x = scale * (3.5 * sin(M_PI * (f64) (270.0f + 45.0f * (f32) i) / 180.0));
            lbl_1_data_FF8.vel.y = 1.0f;
            lbl_1_data_FF8.vel.z = scale * (3.5 * cos(M_PI * (f64) (270.0f + 45.0f * (f32) i) / 180.0));
            fn_1_2E600(camMask, &lbl_1_data_FF8, pos->x, pos->y * 1.5f, pos->z, 50.0f);
        }
        for (i = 0; i < 5; i++) {
            lbl_1_data_FF8.vel.x = scale * (1.5 * sin(M_PI * (270.0 + (22.5 + (f64) (72.0f * (f32) i))) / 180.0));
            lbl_1_data_FF8.vel.y = 1.0f;
            lbl_1_data_FF8.vel.z = scale * (1.5 * cos(M_PI * (270.0 + (22.5 + (f64) (72.0f * (f32) i))) / 180.0));
            fn_1_2E600(camMask, &lbl_1_data_FF8, pos->x, pos->y * 1.5f, pos->z, 30.0f);
        }
        lbl_1_data_FF8.colStart[3] = 0xFF;
        break;
    }
}

s16 fn_1_2E600(s16 camMask, void *param, f32 x, f32 y, f32 z, f32 scale) {
    s16 ret = -1;
    s16 cam;
    s16 bit = 1;
    s16 idx;
    HU3DPARTICLE *p;
    HU3DPARTICLEDATA *d;
    EffectParam *work;
    EffectParam *src = param;

    fn_1_2E8A4();
    for (cam = 0; cam < 0x10; cam++, bit *= 2) {
        if ((bit & camMask) == 0 || Hu3DCamera[cam].fov == -1.0f) {
            continue;
        }
        if (lbl_1_bss_79F8[cam] == -1) {
            return -1;
        }
        p = Hu3DData[lbl_1_bss_79F8[cam]].hookData;
        d = &p->data[p->emitCnt];
        idx = p->emitCnt;
        while (idx < p->maxCnt && d->scale != 0.0f) {
            idx++;
            d++;
        }
        if (idx >= p->maxCnt) {
            d = p->data;
            idx = 0;
            while (idx < p->maxCnt && d->scale != 0.0f) {
                idx++;
                d++;
            }
        }
        if (idx != p->maxCnt) {
            work = p->work;
            work[idx] = *src;
            d->unk04 = 0;
            d->cameraBit = camMask;
            d->pos.x = x;
            d->pos.y = y;
            d->pos.z = z;
            d->vel.x = src->vel.x;
            d->vel.y = src->vel.y;
            d->vel.z = src->vel.z;
            d->color.r = src->colStart[0];
            d->color.g = src->colStart[1];
            d->color.b = src->colStart[2];
            d->color.a = src->colStart[3];
            d->scaleBase = scale;
            d->scale = scale;
            d->time = 0;
            d->parManId = -1;
            p->emitCnt = idx;
            ret = idx;
        }
    }
    return ret;
}

static void effectParticleInit(s16 cam, ANIMDATA *anim) {
    HU3DPARTICLE *p;
    HU3DPARTICLEDATA *d;
    s16 i;

    lbl_1_bss_79F8[cam] = Hu3DParticleCreate(anim, 0x96);
    Hu3DModelLayerSet(lbl_1_bss_79F8[cam], 4);
    Hu3DParticleHookSet(lbl_1_bss_79F8[cam], fn_1_2ED2C);
    Hu3DModelCameraSet(lbl_1_bss_79F8[cam], (u16) (1 << cam));
    if (lbl_1_bss_7A18 == NULL) {
        lbl_1_bss_7A18 = HuMemDirectMallocNum(HEAP_HEAP, 150 * sizeof(EffectParam), 0x10000000);
    }
    Hu3DParticleBlendModeSet(lbl_1_bss_79F8[cam], 0);
    p = Hu3DData[lbl_1_bss_79F8[cam]].hookData;
    p->emitCnt = 0;
    p->work = lbl_1_bss_7A18;
    p->count = 1;
    d = p->data;
    for (i = 0; i < p->maxCnt; i++, d++) {
        d->scale = 0.0f;
    }
}

void fn_1_2E8A4(void) {
    ANIMDATA *anim = NULL;
    HU3DPARTICLE *p;
    s16 cam;

    for (cam = 0; cam < 0x10; cam++) {
        if (Hu3DCamera[cam].fov == -1.0f) {
            continue;
        }
        if (lbl_1_bss_79F8[cam] != -1) {
            p = Hu3DData[lbl_1_bss_79F8[cam]].hookData;
            if (p->anim != NULL) {
                anim = p->anim;
            }
        } else {
            if (anim == NULL) {
                anim = HuSprAnimDataRead(0x750070);
            }
            effectParticleInit(cam, anim);
        }
    }
}

void fn_1_2EB08(void) {
    ANIMDATA *anim = NULL;
    s16 cam;

    for (cam = 0; cam < 0x10; cam++) {
        if (Hu3DCamera[cam].fov != -1.0f && lbl_1_bss_79F8[cam] == -1) {
            if (anim == NULL) {
                anim = HuSprAnimDataRead(0x750070);
            }
            effectParticleInit(cam, anim);
        }
    }
}

void fn_1_2ED2C(HU3DMODEL *modelP, HU3DPARTICLE *p, Mtx mtx) {
    EffectParam *work = p->work;
    HU3DPARTICLEDATA *d;
    s16 i;
    s16 v;
    f64 mul;

    if (p->count == 0) {
        d = p->data;
        for (i = 0; i < p->maxCnt; i++, d++) {
            d->scale = 0.0f;
        }
    }
    d = p->data;
    for (i = 0; i < p->maxCnt; i++, d++) {
        EffectParam *w = &work[i];

        if (d->scale == 0.0f) {
            continue;
        }
        d->vel.x *= w->damp.x;
        d->vel.y *= w->damp.y;
        d->vel.z *= w->damp.z;
        PSVECAdd((Vec *) &d->vel, (Vec *) &d->pos, (Vec *) &d->pos);
        d->vel.y += w->gravity;
        v = (s16) ((f32) d->color.r + w->colorBlend * (f32) (w->colEnd[0] - w->colStart[0]));
        d->color.r = v < 0 ? 0 : (v > 0xFF ? 0xFF : v);
        v = (s16) ((f32) d->color.g + w->colorBlend * (f32) (w->colEnd[1] - w->colStart[1]));
        d->color.g = v < 0 ? 0 : (v > 0xFF ? 0xFF : v);
        v = (s16) ((f32) d->color.b + w->colorBlend * (f32) (w->colEnd[2] - w->colStart[2]));
        d->color.b = v < 0 ? 0 : (v > 0xFF ? 0xFF : v);
        v = (s16) ((f32) d->color.a + w->alphaStep);
        if (v < 1) {
            d->scale = 0.0f;
        }
        d->color.a = v;
        if (d->scale != 0.0f) {
            if (w->flags & 1) {
                mul = ((d->time + i) & 1) ? 1.0 : 0.5;
                d->scale = d->scaleBase * mul;
            } else {
                d->scale = d->scaleBase;
            }
            d->scaleBase += w->scaleStep;
            if (d->scaleBase <= 0.01f) {
                d->scale = 0.0f;
            }
        }
        d->time++;
    }
    DCStoreRangeNoSync(p->data, p->maxCnt * sizeof(HU3DPARTICLEDATA));
}
