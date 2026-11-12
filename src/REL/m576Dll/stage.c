#include "REL/m576Dll.h"
#include <math.h>
#include <stdlib.h>

typedef struct M576Rec_s {
    /* 0x00 */ u8 flags;
    /* 0x04 */ f32 base;
    /* 0x08 */ s8 dir;
    /* 0x0C */ f32 g;
    /* 0x10 */ f32 h;
} M576REC;

typedef struct M576Node_s M576NODE;
struct M576Node_s {
    /* 0x00 */ HSFFACE *face;
    /* 0x04 */ s32 id;
    /* 0x08 */ HuVecF pos;
    /* 0x14 */ HuVecF mid[3];
    /* 0x38 */ f32 len[3];
    /* 0x44 */ M576NODE *adj[3];
    /* 0x50 */ M576REC rec[4];
};

typedef struct M576Open_s M576OPEN;
struct M576Open_s {
    /* 0x00 */ M576OPEN *prev;
    /* 0x04 */ M576OPEN *next;
    /* 0x08 */ s8 used;
    /* 0x0A */ s16 idx;
    /* 0x0C */ f32 cost;
    /* 0x10 */ M576NODE *node;
};

typedef struct M576Level_s {
    /* 0x00 */ u8 a[10];
    /* 0x0A */ u8 b[10];
    /* 0x14 */ u8 c[10];
    /* 0x1E */ u8 flags;
    /* 0x20 */ f32 f20;
    /* 0x24 */ f32 f24;
    /* 0x28 */ f32 f28;
    /* 0x2C */ f32 f2C;
} M576LEVEL;

typedef void (*M576StateFunc)(M576COM *com);

void fn_1_ABEC(M576COM *com, M576NODE *goal, HuVecF *goalPos);
s32 fn_1_B328(M576COM *com, M576NODE **cand, HuVecF *pos, s32 num);
void fn_1_C6E4(M576COM *com);
void fn_1_C6E8(M576COM *com);
void fn_1_CB9C(M576COM *com);
void fn_1_D1D8(M576COM *com);
void fn_1_D54C(M576COM *com);
void fn_1_D8E0(M576COM *com);
void fn_1_DBE0(M576COM *com);
void fn_1_DF40(M576COM *com);
void fn_1_E494(M576COM *com);
void _NaviMapInit(s16 mdlId);

/* unit-owned .bss, declared in reverse address order */
M576COM lbl_1_bss_35C[M576_PLAYER_MAX];
s8 lbl_1_bss_358;
s32 lbl_1_bss_324[13];
M576NODE *lbl_1_bss_320;
s32 lbl_1_bss_31C;
M576OPEN *lbl_1_bss_318;
s32 lbl_1_bss_314;
s32 lbl_1_bss_310;

M576StateFunc lbl_1_data_3B0[9] = {
    fn_1_C6E4,
    fn_1_C6E8,
    fn_1_CB9C,
    fn_1_D1D8,
    fn_1_D54C,
    fn_1_D8E0,
    fn_1_DBE0,
    fn_1_DF40,
    fn_1_E494,
};

M576LEVEL lbl_1_data_3D4[4] = {
    { { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 }, { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0x00, 10.0f, 60.0f, 30.0f, 80.0f },
    { { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 }, { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0x03, 5.0f, 40.0f, 25.0f, 65.0f },
    { { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }, { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 }, 0x0F, 1.5f, 25.0f, 20.0f, 50.0f },
    { { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 }, 0x0F, 1.0f, 0.0f, 0.0f, 30.0f },
};

/* does the node's face use a material flagged as walkable */
s32 fn_1_6784(HSFOBJECT *obj, HSFFACE *face)
{
    return obj->mesh.material[face->mat & 0xFFF].flags & 0x70000;
}

void fn_1_67A4(s32 *out, HSFFACE *face, s32 tri)
{
    switch (face->type) {
        case 2:
            out[0] = face->index[0].vertex;
            out[1] = face->index[1].vertex;
            out[2] = face->index[2].vertex;
            return;
        case 3:
            if ((tri & 1) != 0) {
                out[0] = face->index[0].vertex;
                out[1] = face->index[1].vertex;
                out[2] = face->index[3].vertex;
                return;
            }
            out[0] = face->index[0].vertex;
            out[1] = face->index[3].vertex;
            out[2] = face->index[2].vertex;
            return;
        case 4:
            out[0] = face->strip.data[tri].vertex;
            out[1] = face->strip.data[tri + 1].vertex;
            out[2] = face->strip.data[tri + 2].vertex;
            return;
    }
}

u32 fn_1_6868(HuVecF a, HuVecF b)
{
    HuVecF diff;

    PSVECSubtract(&a, &b, &diff);
    if (fabsf(diff.y) < 280.0f) {
        diff.y = 0.0f;
        return PSVECSquareMag(&diff) < 0.001;
    }
    return 0;
}

s32 fn_1_693C(M576NODE *node, M576NODE *other, HuVecF *verts, s32 v0, s32 v1)
{
    HSFFACE *face;
    s32 idx[3];
    HuVecF p0;
    HuVecF p1;
    HuVecF a0;
    HuVecF a1;
    HuVecF a2;

    face = node->face;
    switch (face->type) {
        case 2:
            idx[0] = face->index[0].vertex;
            idx[1] = face->index[1].vertex;
            idx[2] = face->index[2].vertex;
            break;
        case 3:
            idx[0] = face->index[0].vertex;
            idx[1] = face->index[3].vertex;
            idx[2] = face->index[2].vertex;
            break;
        case 4:
            idx[0] = face->strip.data[0].vertex;
            idx[1] = face->strip.data[1].vertex;
            idx[2] = face->strip.data[2].vertex;
            break;
    }
    p0 = verts[v0];
    p1 = verts[v1];
    a0 = verts[idx[0]];
    a1 = verts[idx[1]];
    a2 = verts[idx[2]];
    if (fn_1_6868(a0, p0)) {
        if (fn_1_6868(a1, p1)) {
            node->adj[0] = other;
            return 1;
        }
        if (fn_1_6868(a2, p1)) {
            node->adj[2] = other;
            return 1;
        }
        return 0;
    }
    if (fn_1_6868(a1, p0)) {
        if (fn_1_6868(a0, p1)) {
            node->adj[0] = other;
            return 1;
        }
        if (fn_1_6868(a2, p1)) {
            node->adj[1] = other;
            return 1;
        }
        return 0;
    }
    if (fn_1_6868(a2, p0)) {
        if (fn_1_6868(a0, p1)) {
            node->adj[2] = other;
            return 1;
        }
        if (fn_1_6868(a1, p1)) {
            node->adj[1] = other;
            return 1;
        }
    }
    return 0;
}

void fn_1_70AC(M576REC *rec)
{
    rec->flags &= ~3;
    rec->dir = 0;
    rec->h = 0.0f;
    rec->g = 0.0f;
}

void fn_1_70E8(M576COM *com)
{
    M576NODE *node = lbl_1_bss_320;
    M576OPEN *open = lbl_1_bss_318;
    M576REC *rec;
    s32 i;

    i = lbl_1_bss_31C;
    while (i--) {
        rec = &node->rec[com->no];
        rec->flags &= ~3;
        rec->dir = 0;
        rec->h = 0.0f;
        rec->g = 0.0f;
        node++;
    }
    lbl_1_bss_310 = -1;
    i = lbl_1_bss_314;
    while (i--) {
        open->used = 0;
        open++;
    }
}

void fn_1_71D0(M576COM *com)
{
    M576NODE *node = lbl_1_bss_320;
    s32 i;

    i = lbl_1_bss_31C;
    while (i--) {
        node->rec[com->no].base = 0.0f;
        node++;
    }
}

M576NODE *fn_1_7238(s32 id)
{
    M576NODE *node = lbl_1_bss_320;
    s32 i;

    i = lbl_1_bss_31C;
    while (1) {
        if (i-- == 0) {
            return NULL;
        }
        if (node->id == id) {
            return node;
        }
        node++;
    }
}

s32 fn_1_7298(void)
{
    return (lbl_1_bss_310 >= 0);
}

f32 fn_1_72B0(M576REC *rec)
{
    return rec->g + rec->h;
}

void fn_1_72C0(M576OPEN *entry)
{
    M576OPEN *cur = &lbl_1_bss_318[lbl_1_bss_310];
    M576OPEN *prev;

    while (1) {
        if (entry->cost < cur->cost) {
            prev = cur->prev;
            if (prev != NULL) {
                prev->next = entry;
            } else {
                lbl_1_bss_310 = entry->idx;
            }
            entry->prev = prev;
            entry->next = cur;
            cur->prev = entry;
            return;
        }
        if (cur->next == NULL) {
            cur->next = entry;
            entry->prev = cur;
            entry->next = NULL;
            return;
        }
        cur = cur->next;
    }
}

s32 fn_1_7368(M576NODE *node, s32 comNo)
{
    M576REC *rec = &node->rec[comNo];
    M576OPEN *entry;

    if (lbl_1_bss_310 < 0) {
        return 0;
    }
    entry = &lbl_1_bss_318[lbl_1_bss_310];
    while (entry != NULL && entry->node != node) {
        entry = entry->next;
    }
    if (entry == NULL) {
        return 0;
    }
    entry->cost = rec->g + rec->h;
    if (entry->next == NULL && entry->prev == NULL) {
        return 1;
    }
    if (entry->next != NULL) {
        entry->next->prev = entry->prev;
    }
    if (entry->prev != NULL) {
        entry->prev->next = entry->next;
    } else {
        lbl_1_bss_310 = entry->next->idx;
    }
    fn_1_72C0(entry);
    return 1;
}

void fn_1_751C(M576NODE *node, s32 comNo)
{
    M576REC *rec = &node->rec[comNo];
    M576OPEN *entry = lbl_1_bss_318;
    M576OPEN *list;
    s32 i;
    s32 n = 0;

    while (n < lbl_1_bss_314 && entry->used != 0) {
        n++;
        entry++;
    }
    if (n == lbl_1_bss_314) {
        lbl_1_bss_314 += lbl_1_bss_31C / 4;
        list = HuMemDirectMallocNum(HEAP_MODEL, lbl_1_bss_314 * sizeof(M576OPEN), HU_MEMNUM_OVL);
        memset(list, 0, lbl_1_bss_314 * sizeof(M576OPEN));
        memcpy(list, lbl_1_bss_318, n * sizeof(M576OPEN));
        HuMemDirectFree(lbl_1_bss_318);
        lbl_1_bss_318 = list;
        entry = lbl_1_bss_318 + n;
        i = lbl_1_bss_31C;
        while (i--) {
            lbl_1_bss_318[i].idx = i;
        }
    }
    entry->cost = rec->g + rec->h;
    entry->node = node;
    entry->used = 1;
    if (lbl_1_bss_310 < 0) {
        lbl_1_bss_310 = n;
        entry->prev = NULL;
        entry->next = NULL;
        return;
    }
    fn_1_72C0(entry);
}

M576NODE *fn_1_778C(void)
{
    M576OPEN *entry;
    M576OPEN *next;

    if (lbl_1_bss_310 < 0) {
        return NULL;
    }
    entry = &lbl_1_bss_318[lbl_1_bss_310];
    next = entry->next;
    if (next != NULL) {
        next->prev = NULL;
        lbl_1_bss_310 = next->idx;
    } else {
        lbl_1_bss_310 = -1;
    }
    entry->used = 0;
    return entry->node;
}

void fn_1_7828(M576COM *com, s32 *ids, s32 num, f32 weight, f32 radius)
{
    M576NODE *src;
    M576NODE *node;
    HuVecF center;
    HuVecF diff;
    f32 dist;
    s32 i;
    s32 j;

    i = num;
    while (i--) {
        src = fn_1_7238(ids[i]);
        if (src != NULL) {
            center = src->pos;
            node = lbl_1_bss_320;
            j = lbl_1_bss_31C;
            while (j--) {
                PSVECSubtract(&center, &node->pos, &diff);
                dist = PSVECMag(&diff);
                if (dist < radius) {
                    node->rec[com->no].base += weight * (1.0f - dist / radius);
                }
                node++;
            }
        }
    }
}

f32 fn_1_7994(M576NODE *node, HuVecF *pos)
{
    HuVecF diff;

    PSVECSubtract(&node->pos, pos, &diff);
    return PSVECMag(&diff);
}

s32 fn_1_79D4(M576NODE *node, M576NODE *from, HuVecF *goal, s32 comNo, f32 g)
{
    M576REC *rec = &node->rec[comNo];
    HuVecF diff;
    s32 changed = 0;

    if ((rec->flags & 1) == 0) {
        changed = 1;
        rec->flags |= 3;
        if (goal != NULL) {
            PSVECSubtract(&node->pos, goal, &diff);
            rec->h = rec->base + PSVECMag(&diff);
        } else {
            rec->h = rec->base;
        }
        rec->g = g;
        fn_1_751C(node, comNo);
    } else if ((rec->flags & 2) != 0 && (g + rec->h) < (rec->g + rec->h)) {
        changed = 1;
        rec->g = g;
        fn_1_7368(node, comNo);
    }
    if (changed != 0) {
        if (from == node->adj[0]) {
            rec->dir = 0;
        } else if (from == node->adj[1]) {
            rec->dir = 1;
        } else if (from == node->adj[2]) {
            rec->dir = 2;
        }
    }
    return changed;
}

void fn_1_7F44(M576NODE *node, s32 comNo, HuVecF *goal)
{
    M576REC *rec = &node->rec[comNo];
    s32 k;

    if ((rec->flags & 1) != 0) {
        rec->flags &= ~2;
        for (k = 0; k < 3; k++) {
            if (node->adj[k] != NULL) {
                fn_1_79D4(node->adj[k], node, goal, comNo, rec->g + node->len[abs(k - rec->dir)]);
            }
        }
    }
}

void _NaviMapInit(s16 mdlId)
{
    HSFDATA *hsf;
    HSFOBJECT *obj;
    HSFFACE *face;
    HuVecF *verts;
    M576NODE *node;
    M576NODE *other;
    HuVecF up;
    HuVecF tmp;
    s32 idx[3];
    s32 count;
    s32 i;
    s32 j;
    s32 k;
    s32 swapIdx[3];

    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    hsf = Hu3DData[mdlId].hsf;
    if (hsf->objectNum != 1) {
        OSReport("ERROR!! | _NaviMapInit : \203I\203u\203W\203F\203N\203g\202\314\220\224\202\315\225K\202\270\210\352\202\302\202\311\202\265\202\304\202\255\202\276\202\263\202\242\201I\201I\n");
    }
    obj = hsf->object;
    count = 0;
    face = obj->mesh.face->data;
    i = obj->mesh.face->count;
    while (i--) {
        if (fn_1_6784(obj, face) != 0 && !(PSVECDotProduct((HuVecF *)face->nbt, &up) < 0.0f)) {
            if (face->type != 2) {
                OSReport("ERROR!! | _NaviMapInit : \202`\202h\227p\202\240\202\275\202\350\224\273\222\350\203}\203b\203v\202\315\221S\202\304\202R\212p\214`\203|\203\212\203S\203\223\202\305\223\235\210\352\202\265\202\304\202\255\202\276\202\263\202\242\n");
                abort();
            }
            count++;
        }
        face++;
    }
    lbl_1_bss_31C = count;
    lbl_1_bss_320 = HuMemDirectMallocNum(HEAP_MODEL, lbl_1_bss_31C * sizeof(M576NODE), HU_MEMNUM_OVL);
    lbl_1_bss_314 = lbl_1_bss_31C;
    lbl_1_bss_318 = HuMemDirectMallocNum(HEAP_MODEL, lbl_1_bss_314 * sizeof(M576OPEN), HU_MEMNUM_OVL);
    i = lbl_1_bss_31C;
    while (i--) {
        lbl_1_bss_318[i].idx = i;
    }
    node = lbl_1_bss_320;
    face = obj->mesh.face->data;
    verts = obj->mesh.vertex->data;
    for (i = 0; i < obj->mesh.face->count; i++, face++) {
        if (fn_1_6784(obj, face) != 0 && !(PSVECDotProduct((HuVecF *)face->nbt, &up) < 0.0f)) {
            switch (face->type) {
                case 2:
                    idx[0] = face->index[0].vertex;
                    idx[1] = face->index[1].vertex;
                    idx[2] = face->index[2].vertex;
                    break;
                case 3:
                    idx[0] = face->index[0].vertex;
                    idx[1] = face->index[3].vertex;
                    idx[2] = face->index[2].vertex;
                    break;
                case 4:
                    idx[0] = face->strip.data[0].vertex;
                    idx[1] = face->strip.data[1].vertex;
                    idx[2] = face->strip.data[2].vertex;
                    break;
            }
            node->face = face;
            node->id = i;
            node->pos.x = (verts[idx[2]].x + (verts[idx[0]].x + verts[idx[1]].x)) / 3.0f;
            node->pos.y = (verts[idx[2]].y + (verts[idx[0]].y + verts[idx[1]].y)) / 3.0f;
            node->pos.z = (verts[idx[2]].z + (verts[idx[0]].z + verts[idx[1]].z)) / 3.0f;
            PSVECAdd(&verts[idx[0]], &verts[idx[1]], &node->mid[0]);
            PSVECScale(&node->mid[0], &node->mid[0], 0.5f);
            PSVECAdd(&verts[idx[1]], &verts[idx[2]], &node->mid[1]);
            PSVECScale(&node->mid[1], &node->mid[1], 0.5f);
            PSVECAdd(&verts[idx[2]], &verts[idx[0]], &node->mid[2]);
            PSVECScale(&node->mid[2], &node->mid[2], 0.5f);
            PSVECSubtract(&node->mid[0], &node->mid[1], &tmp);
            node->len[0] = PSVECMag(&tmp);
            PSVECSubtract(&node->mid[1], &node->mid[2], &tmp);
            node->len[1] = PSVECMag(&tmp);
            PSVECSubtract(&node->mid[2], &node->mid[0], &tmp);
            node->len[2] = PSVECMag(&tmp);
            node->adj[2] = NULL;
            node->adj[1] = NULL;
            node->adj[0] = NULL;
            node++;
        }
    }
    node = lbl_1_bss_320;
    verts = obj->mesh.vertex->data;
    i = lbl_1_bss_31C;
    while (i--) {
        face = node->face;
        switch (face->type) {
            case 2:
                swapIdx[0] = face->index[0].vertex;
                swapIdx[1] = face->index[1].vertex;
                swapIdx[2] = face->index[2].vertex;
                break;
            case 3:
                swapIdx[0] = face->index[0].vertex;
                swapIdx[1] = face->index[3].vertex;
                swapIdx[2] = face->index[2].vertex;
                break;
            case 4:
                swapIdx[0] = face->strip.data[0].vertex;
                swapIdx[1] = face->strip.data[1].vertex;
                swapIdx[2] = face->strip.data[2].vertex;
                break;
        }
        other = lbl_1_bss_320;
        j = lbl_1_bss_31C;
        while (j--) {
            if (node != other) {
                if (node->adj[0] == NULL && fn_1_693C(other, node, verts, swapIdx[0], swapIdx[1]) != 0) {
                    node->adj[0] = other;
                } else if (node->adj[1] == NULL && fn_1_693C(other, node, verts, swapIdx[1], swapIdx[2]) != 0) {
                    node->adj[1] = other;
                } else if (node->adj[2] == NULL && fn_1_693C(other, node, verts, swapIdx[2], swapIdx[0]) != 0) {
                    node->adj[2] = other;
                }
                if (node->adj[0] == NULL || node->adj[1] == NULL || node->adj[2] == NULL) {
                    other++;
                    continue;
                }
                break;
            }
            other++;
        }
        node++;
    }
}

s32 fn_1_869C(M576COM *com)
{
    M576NODE *node;
    M576NODE *best;
    M576NODE *nodes[5];
    HuVecF vecs[5];
    HuVecF spot;
    s32 ids[5];
    s32 types[5];
    s32 pids[5];
    s32 list[4];
    s32 timer;
    s32 type;
    s32 mode;
    s32 coinsFree;
    s32 attr;
    s32 cnt;
    s32 n;
    s32 i;
    s32 sel;
    s32 who;
    s32 dest;
    f32 dist;

    timer = 1;
    type = fn_1_9EC(lbl_1_bss_324[com->player->actor->param]);
    if (type == 0) {
        attr = fn_1_A54(com->player->actor->param) & 0x407F;
        if ((attr & 4) != 0) {
            type = 3;
        } else if ((attr & 8) != 0) {
            type = 4;
        }
    }
    if (type != 0) {
        return type;
    }
    mode = 0;
    coinsFree = fn_1_59C();
    if (fn_1_798(&com->player) >= 3) {
        mode = 4;
        if (coinsFree != 0) {
            fn_1_668(&com->player, NULL, &dist);
            if ((lbl_1_data_3D4[com->level].flags & 8) == 0 || dist < 62500.0f) {
                mode = 1;
            }
        }
    } else if (coinsFree != 0) {
        mode = 1;
    } else {
        n = fn_1_8AC(NULL, NULL);
        if (n >= 3) {
            mode = 2;
        } else {
            mode = 3;
        }
    }
    fn_1_71D0(com);
    com->mode = mode;
    switch (com->mode) {
        case 4:
            cnt = 0;
            for (i = 0; i <= 3; i++) {
                if (i != com->player->actor->param) {
                    list[cnt] = lbl_1_bss_324[i];
                    cnt++;
                }
            }
            fn_1_7828(com, list, cnt, 10000.0f, 800.0f);
            if ((lbl_1_data_3D4[com->level].flags & 1) != 0) {
                fn_1_7828(com, &lbl_1_bss_324[4], 4, 10000000.0f, 100.0f);
            }
            dest = fn_1_174(0, &com->targetPos);
            node = fn_1_7238(dest);
            for (i = 1; i <= 4; i++) {
                dest = fn_1_174(i, &spot);
                best = fn_1_7238(dest);
                if (best->rec[com->no].base < node->rec[com->no].base) {
                    node = best;
                    com->targetPos = spot;
                }
            }
            fn_1_ABEC(com, node, &com->targetPos);
            com->reach = 250.0f;
            com->targetType = 0;
            com->target = -1;
            com->coinTarget = -1;
            timer = 15;
            break;
        case 3:
            cnt = 0;
            for (i = 0; i <= 3; i++) {
                if (i != com->player->actor->param) {
                    n = fn_1_854(i);
                    types[cnt] = fn_1_9EC(lbl_1_bss_324[i]);
                    switch (types[cnt]) {
                        case 1:
                            dest = fn_1_174(5, NULL);
                            break;
                        case 2:
                            dest = fn_1_174(7, NULL);
                            break;
                        case 0:
                            attr = fn_1_A54(i) & 0x407F;
                            if ((attr & 4) != 0) {
                                dest = fn_1_174(5, NULL);
                                types[cnt] = 3;
                            } else if ((attr & 8) != 0) {
                                dest = fn_1_174(7, NULL);
                                types[cnt] = 4;
                            } else {
                                dest = lbl_1_bss_324[i];
                            }
                            break;
                    }
                    if ((lbl_1_data_3D4[com->level].flags & 8) != 0) {
                        fn_1_7828(com, &dest, 1, 8000.0f / (f32)(n + 1), 400.0f / (f32)(n + 1));
                    } else {
                        fn_1_7828(com, &dest, 1, 1333.3334f, 66.666664f);
                    }
                    if (n != 0) {
                        fn_1_9A4(i, &vecs[cnt]);
                        nodes[cnt] = fn_1_7238(dest);
                        if (nodes[cnt] != NULL) {
                            pids[cnt] = i;
                            cnt++;
                        }
                    }
                }
            }
            if ((lbl_1_data_3D4[com->level].flags & 1) != 0) {
                fn_1_7828(com, &lbl_1_bss_324[4], 4, 10000000.0f, 100.0f);
            }
            if (cnt == 0) {
                cnt = 0;
                for (i = 0; i <= 3; i++) {
                    if (i != com->player->actor->param) {
                        types[cnt] = fn_1_9EC(lbl_1_bss_324[i]);
                        fn_1_9A4(i, &vecs[cnt]);
                        switch (types[cnt]) {
                            case 1:
                                nodes[cnt] = fn_1_7238(fn_1_174(5, NULL));
                                break;
                            case 2:
                                nodes[cnt] = fn_1_7238(fn_1_174(7, NULL));
                                break;
                            case 0:
                                attr = fn_1_A54(i) & 0x407F;
                                if ((attr & 4) != 0) {
                                    nodes[cnt] = fn_1_7238(fn_1_174(5, NULL));
                                    types[cnt] = 3;
                                } else if ((attr & 8) != 0) {
                                    nodes[cnt] = fn_1_7238(fn_1_174(7, NULL));
                                    types[cnt] = 4;
                                } else {
                                    nodes[cnt] = fn_1_7238(lbl_1_bss_324[i]);
                                }
                                break;
                        }
                        if (nodes[cnt] != NULL) {
                            pids[cnt] = i;
                            cnt++;
                        }
                    }
                }
            }
            sel = fn_1_B328(com, nodes, vecs, cnt);
            com->reach = 200.0f;
            com->coinTarget = -1;
            if (sel >= 0) {
                com->target = pids[sel];
                com->targetType = types[sel];
            } else {
                com->target = -1;
                com->targetType = 0;
            }
            timer = 90;
            break;
        case 1:
            cnt = 0;
            if ((lbl_1_data_3D4[com->level].flags & 8) != 0) {
                for (i = 0; i <= 3; i++) {
                    if (i != com->player->actor->param) {
                        list[cnt] = lbl_1_bss_324[i];
                        cnt++;
                    }
                }
                fn_1_7828(com, list, cnt, 5000.0f, 400.0f);
            }
            if ((lbl_1_data_3D4[com->level].flags & 1) != 0) {
                fn_1_7828(com, &lbl_1_bss_324[4], 4, 10000000.0f, 100.0f);
            }
            cnt = 0;
            for (i = 8; i <= 12; i++) {
                if (fn_1_5F4(i, NULL) < 0) {
                    types[cnt] = fn_1_9EC(lbl_1_bss_324[i]);
                    switch (types[cnt]) {
                        case 1:
                            nodes[cnt] = fn_1_7238(fn_1_174(5, &vecs[cnt]));
                            break;
                        case 2:
                            nodes[cnt] = fn_1_7238(fn_1_174(7, &vecs[cnt]));
                            break;
                        case 0:
                            attr = fn_1_A54(i) & 0x407F;
                            if ((attr & 4) != 0) {
                                nodes[cnt] = fn_1_7238(fn_1_174(5, &vecs[cnt]));
                                types[cnt] = 3;
                            } else if ((attr & 8) != 0) {
                                nodes[cnt] = fn_1_7238(fn_1_174(7, &vecs[cnt]));
                                types[cnt] = 4;
                            } else {
                                nodes[cnt] = fn_1_7238(lbl_1_bss_324[i]);
                                fn_1_5F4(i, &vecs[cnt]);
                            }
                            break;
                    }
                    ids[cnt] = i;
                    if (nodes[cnt] != NULL) {
                        cnt++;
                    }
                }
            }
            sel = fn_1_B328(com, nodes, vecs, cnt);
            com->reach = 100.0f;
            com->target = -1;
            if (sel >= 0) {
                com->targetType = types[sel];
                com->coinTarget = ids[sel];
            } else {
                com->targetType = 0;
                com->coinTarget = -1;
            }
            timer = 30;
            break;
        case 2:
            fn_1_8AC(&who, &com->targetPos);
            type = fn_1_9EC(lbl_1_bss_324[who]);
            switch (type) {
                case 1:
                    best = fn_1_7238(fn_1_174(5, &com->targetPos));
                    break;
                case 2:
                    best = fn_1_7238(fn_1_174(7, &com->targetPos));
                    break;
                case 0:
                    attr = fn_1_A54(who) & 0x407F;
                    if ((attr & 4) != 0) {
                        best = fn_1_7238(fn_1_174(5, &com->targetPos));
                        type = 3;
                    } else if ((attr & 8) != 0) {
                        best = fn_1_7238(fn_1_174(7, &com->targetPos));
                        type = 4;
                    } else {
                        best = fn_1_7238(lbl_1_bss_324[who]);
                    }
                    break;
            }
            cnt = 0;
            for (i = 0; i <= 3; i++) {
                if (i != com->player->actor->param && i != who) {
                    list[cnt] = lbl_1_bss_324[i];
                    cnt++;
                }
            }
            if (cnt != 0 && (lbl_1_data_3D4[com->level].flags & 8) != 0) {
                fn_1_7828(com, list, cnt, 2000.0f, 300.0f);
            }
            if ((lbl_1_data_3D4[com->level].flags & 1) != 0) {
                fn_1_7828(com, &lbl_1_bss_324[4], 4, 10000000.0f, 100.0f);
            }
            fn_1_ABEC(com, best, &com->targetPos);
            com->reach = 300.0f;
            com->target = who;
            com->coinTarget = -1;
            com->targetType = type;
            timer = 15;
            break;
    }
    com->timer30 = timer;
    com->timer32 = timer;
    return 0;
}

void fn_1_AB24(M576COM *com, HuVecF *pos)
{
    HuVecF *buf;

    com->trail[com->trailPos] = *pos;
    com->trailPos++;
    if (com->trailMax == com->trailPos) {
        com->trailMax += 10;
        buf = HuMemDirectMallocNum(HEAP_MODEL, com->trailMax * sizeof(HuVecF), HU_MEMNUM_OVL);
        memcpy(buf, com->trail, com->trailPos * sizeof(HuVecF));
        HuMemDirectFree(com->trail);
        com->trail = buf;
    }
}

void fn_1_ABEC(M576COM *com, M576NODE *goal, HuVecF *goalPos)
{
    M576NODE *start;
    M576NODE *cur;
    HuVecF pos;
    s32 found;
    s32 dir;
    s32 k;
#ifdef TARGET_PC
    f32 best = 0.0f;
#else
    f32 best;
#endif
    f32 dist;

    fn_1_70E8(com);
    start = fn_1_7238(lbl_1_bss_324[com->player->actor->param]);
    if (start != NULL) {
        start->rec[com->no].flags |= 3;
        start->rec[com->no].h = 0.0f;
        start->rec[com->no].g = 0.0f;
        dir = -1;
        for (k = 0; k < 3; k++) {
            if (start->adj[k] == NULL) {
                dir = k;
                break;
            }
            MgActorPosGet(com->player->actor, &pos);
            PSVECSubtract(&start->mid[k], &pos, &pos);
            dist = PSVECSquareMag(&pos);
            if (dir < 0 || best > dist) {
                best = dist;
                dir = k;
            }
        }
        if (dir < 0) {
            dir = 0;
        }
        start->rec[com->no].dir = dir;
        fn_1_751C(start, com->no);
        found = 0;
        while (lbl_1_bss_310 >= 0 && found == 0) {
            cur = fn_1_778C();
            if (cur == goal) {
                found = 1;
            } else {
                fn_1_7F44(cur, com->no, goalPos);
            }
        }
        if (found != 0) {
            com->trailPos = 0;
            cur = goal;
            while (start != cur) {
                dir = cur->rec[com->no].dir;
                fn_1_AB24(com, &cur->mid[dir]);
                cur = cur->adj[dir];
            }
            com->trailPos--;
        }
    }
}

s32 fn_1_B328(M576COM *com, M576NODE **cand, HuVecF *pos, s32 num)
{
    M576NODE *start;
    M576NODE *cur;
    M576NODE *goal;
    HuVecF self;
    s32 found;
    s32 idx;
    s32 dir;
    s32 k;
    s32 i;
#ifdef TARGET_PC
    f32 best = 0.0f;
#else
    f32 best;
#endif
    f32 dist;

    fn_1_70E8(com);
    start = fn_1_7238(lbl_1_bss_324[com->player->actor->param]);
    if (start == NULL) {
        return -1;
    }
    start->rec[com->no].flags |= 3;
    start->rec[com->no].h = 0.0f;
    start->rec[com->no].g = 0.0f;
    dir = -1;
    for (k = 0; k < 3; k++) {
        if (start->adj[k] == NULL) {
            dir = k;
            break;
        }
        MgActorPosGet(com->player->actor, &self);
        PSVECSubtract(&start->mid[k], &self, &self);
        dist = PSVECSquareMag(&self);
        if (dir < 0 || best > dist) {
            best = dist;
            dir = k;
        }
    }
    if (dir < 0) {
        dir = 0;
    }
    start->rec[com->no].dir = dir;
    fn_1_751C(start, com->no);
    found = 0;
    idx = -1;
    while (lbl_1_bss_310 >= 0 && found == 0) {
        cur = fn_1_778C();
        i = num;
        while (i--) {
            if (cur == cand[i]) {
                com->targetPos = pos[i];
                goal = cand[i];
                found = 1;
                idx = i;
            }
        }
        if (found == 0) {
            fn_1_7F44(cur, com->no, NULL);
        }
    }
    if (found == 0) {
        return -1;
    }
    com->trailPos = 0;
    cur = goal;
    while (start != cur) {
        dir = cur->rec[com->no].dir;
        fn_1_AB24(com, &cur->mid[dir]);
        cur = cur->adj[dir];
    }
    com->trailPos--;
    return idx;
}

void fn_1_BAD8(M576COM *com, s16 mode)
{
    com->player->work[0] = mode;
    com->player->work[1] = 0;
}

void fn_1_BAF4(s32 idx, s32 val)
{
    lbl_1_bss_324[idx] = val;
}

s32 fn_1_BB0C(M576COM *com, s32 idx, f32 range)
{
    HuVecF self;
    HuVecF other;
    HuVecF diff;

    MgActorPosGet(com->player->actor, &self);
    fn_1_9A4(idx, &other);
    PSVECSubtract(&other, &self, &diff);
    if (fabsf(diff.y) > 230.0f) {
        return 0;
    }
    diff.y = 0.0f;
    if (PSVECSquareMag(&diff) < range) {
        com->player->work[0] = 1;
        com->player->work[1] = 0;
        fn_1_9A4(idx, &other);
        com->targetPos = other;
        com->target = idx;
        com->timer30 = 15;
        com->timer32 = com->timer30;
        return 1;
    }
    return 0;
}

s32 fn_1_BC74(M576COM *com, HuVecF *pos, f32 radius)
{
    HuVecF diff;
    f32 sq;

    if (MgPlayerVecChase(com->player, pos, 10.0f, radius) != 0) {
        return 1;
    }
    if (com->timer32 - com->timer30 > 5) {
        PSVECSubtract(&com->pos, &com->player->actor->pos, &diff);
        diff.y = 0.0f;
        sq = PSVECSquareMag(&diff);
        if ((com->player->actor->colGroundAttr & 0x407F) != 0 && MgPlayerModeAttrCheck(com->player, 0x10) == 0
            && ((com->player->actor->colGroundAttr & 0xBF80) != 0 || sq < 7.1111116f)) {
            MgPlayerModeLandSet(com->player, 1);
        }
    }
    return 0;
}

s32 fn_1_BD94(M576COM *com)
{
    HuVecF spot;
    s32 ret;
    s32 stay;
    s32 r;

#ifdef TARGET_PC
    ret = 0;
#endif
    if (lbl_1_data_3D4[com->level].b[frandmod(10)] != 0) {
        ret = 5;
        com->timer30 = (s16)lbl_1_data_3D4[com->level].f28;
    } else {
        r = fn_1_869C(com);
        switch (r) {
            case 0:
                com->trailPos--;
                ret = 0;
                break;
            case 1:
                fn_1_174(12, &spot);
                goto handle;
            case 2:
                fn_1_174(13, &spot);
                goto handle;
            case 3:
                fn_1_174(12, &spot);
                goto handle;
            case 4:
                fn_1_174(13, &spot);
            handle:
                stay = 1;
                if (com->mode != 1) {
                    if (com->mode >= 1 && com->mode < 4) {
                        if (com->target >= 0) {
                            if (fn_1_BB0C(com, com->target, 90000.0f) != 0) {
                                stay = 0;
                            }
                        }
                    }
                } else if (MgPlayerVecChase(com->player, &com->targetPos, 10.0f, 10.0f) == 0) {
                    if (com->coinTarget >= 0) {
                        if (lbl_1_bss_324[com->coinTarget] >= 0) {
                            fn_1_5F4(com->coinTarget, &com->targetPos);
                            stay = 0;
                        }
                    } else if (fn_1_668(&com->player, &com->targetPos, NULL) != 0) {
                        stay = 0;
                    }
                    ret = 0;
                    com->targetType = 0;
                    com->trailPos = -1;
                    com->timer30 = 20;
                }
                if (stay != 0) {
                    ret = 0;
                    com->targetType = 0;
                    com->trailPos = -1;
                    com->targetPos = spot;
                    com->timer30 = 10;
                }
                break;
        }
        com->timer32 = (s16)((f32)com->timer32 * lbl_1_data_3D4[com->level].f20);
    }
    com->timer32 = com->timer30;
    return ret;
}

void fn_1_C16C(s16 mdlId)
{
    s32 i;

    _NaviMapInit(mdlId);
    lbl_1_bss_358 = 0;
    memset(lbl_1_bss_324, 0, sizeof(lbl_1_bss_324));
    i = 13;
    while (i--) {
        lbl_1_bss_324[i] = -1;
    }
}

M576COM *fn_1_C1EC(MGPLAYER *player)
{
    M576COM *com;

    if (lbl_1_bss_358 >= 4) {
        return NULL;
    }
    com = &lbl_1_bss_35C[lbl_1_bss_358];
    com->player = player;
    MgPlayerAttrSet(com->player, 1);
    com->no = lbl_1_bss_358;
    com->trailPos = 0;
    com->trailMax = 30;
    com->trail = HuMemDirectMallocNum(HEAP_MODEL, com->trailMax * sizeof(HuVecF), HU_MEMNUM_OVL);
    com->mode = 1;
    com->coinTarget = -1;
    com->target = -1;
    com->timer30 = 0;
    com->timer32 = 0;
    com->timer34 = 0;
    com->cooldown = 0;
    com->level = GwPlayerConf[com->player->playerNo].dif;
    lbl_1_bss_358++;
    return com;
}

void fn_1_C31C(M576COM *com)
{
    HuVecF self;
    HuVecF other;
    HuVecF diff;
    f32 range;
    s32 cnt;
    s32 i;
    s32 r;

    if (com->timer34 > 0) {
        com->timer34--;
    } else {
        for (i = 0; i <= 3; i++) {
            if (i != com->player->actor->param && fn_1_424(i) == 0) {
                cnt = fn_1_854(i);
                if (com->mode == 4) {
                    range = 160.0f;
                } else if (cnt >= 3) {
                    range = 280.0f;
                } else {
                    range = 100.0f + 60.0f * (f32)cnt;
                }
                if (fn_1_BB0C(com, i, range * range) != 0) {
                    break;
                }
            }
        }
        com->timer34 = (s16)((lbl_1_data_3D4[com->level].f2C + (f32)frandmod(20)) - 10.0f);
    }
    if (com->timer30 > 0) {
        com->timer30--;
    } else {
        r = fn_1_BD94(com);
        switch (r) {
            case 0:
                com->player->work[0] = 2;
                com->player->work[1] = 0;
                break;
            case 3:
                com->player->work[0] = 5;
                com->player->work[1] = 0;
                break;
            case 4:
                com->player->work[0] = 6;
                com->player->work[1] = 0;
                break;
            case 5:
                com->player->work[0] = 0;
                com->player->work[1] = 0;
                break;
        }
    }
    lbl_1_data_3B0[com->player->work[0]](com);
    com->pos = com->player->actor->pos;
}

void fn_1_C6E4(M576COM *com)
{
}

#define M576_LOST_A(com, lost) \
    do { \
        (lost) = 0; \
        if ((com)->mode != 1) { \
            if ((com)->mode >= 1 && (com)->mode < 4) { \
                if ((com)->target >= 0) { \
                    if (fn_1_BB0C((com), (com)->target, 90000.0f) != 0) { \
                        (lost) = 1; \
                    } \
                } \
            } \
        } else if ((com)->coinTarget >= 0 && fn_1_5F4((com)->coinTarget, NULL) >= 0) { \
            (lost) = 1; \
        } \
    } while (0)

#define M576_LOST_B(com, lost) \
    do { \
        (lost) = 0; \
        if ((com)->mode != 1) { \
            if ((com)->mode >= 1 && (com)->mode < 4) { \
                if ((com)->target >= 0) { \
                    if (lbl_1_bss_324[(com)->target] == -1) { \
                        if (fn_1_BB0C((com), (com)->target, 90000.0f) != 0) { \
                            (lost) = 1; \
                        } \
                    } else { \
                        (lost) = 1; \
                    } \
                } \
            } \
        } else if ((com)->coinTarget >= 0 && fn_1_5F4((com)->coinTarget, NULL) >= 0) { \
            (lost) = 1; \
        } \
    } while (0)

void fn_1_C6E8(M576COM *com)
{
    HuVecF target;
    HuVecF self;
    HuVecF diff;
    f32 sq;
    s32 air;

    fn_1_9A4(com->target, &target);
    fn_1_BC74(com, &target, 150.0f);
    if (com->cooldown > 0) {
        com->cooldown--;
        return;
    }
    if (com->mode != 2 && com->mode != 3 && fn_1_424(com->target) != 0) {
        com->timer30 = 0;
        return;
    }
    MgActorPosGet(com->player->actor, &self);
    PSVECSubtract(&target, &self, &diff);
    if (fabsf(diff.y) > 230.0f) {
        com->timer30 = 0;
        return;
    }
    air = 0;
    if ((com->player->actor->colGroundAttr & 0x407F) != 0 && fabsf(diff.y) > 150.0f) {
        air = 1;
    }
    diff.y = 0.0f;
    sq = PSVECSquareMag(&diff);
    if (air == 0 && sq < 28900.0f) {
        if (lbl_1_data_3D4[com->level].a[frandmod(10)] != 0) {
            MgPlayerVecChase(com->player, &target, 0.0f, 0.01f);
            if ((com->player->actor->colGroundAttr & 0x407F) != 0 && MgPlayerModeAttrCheck(com->player, 0x10) == 0) {
                MgPlayerModeLandSet(com->player, 3);
            } else if (com->player->actor->velY < com->player->actor->gravity) {
                if ((lbl_1_data_3D4[com->level].flags & 4) != 0 && lbl_1_data_3D4[com->level].c[frandmod(10)] != 0 && sq < 14400.0f) {
                    MgPlayerModeLandSet(com->player, 5);
                }
                if ((lbl_1_data_3D4[com->level].flags & 2) != 0) {
                    MgPlayerModeLandSet(com->player, 4);
                }
            }
        }
        com->cooldown = (s16)lbl_1_data_3D4[com->level].f24;
    }
}

void fn_1_CB9C(M576COM *com)
{
    HuVecF dest;
    HuVecF diff;
    s16 mode;
    f32 radius;

    if (com->trailPos < 0) {
        dest = com->targetPos;
    } else {
        dest = com->trail[com->trailPos];
        PSVECSubtract(&dest, &com->player->actor->pos, &diff);
        diff.y = 0.0f;
        if (PSVECSquareMag(&diff) < com->reach) {
            if (com->trailPos < 0) {
                dest = com->targetPos;
            } else {
                com->trailPos--;
                dest = com->trail[com->trailPos];
            }
        }
    }
    switch (com->targetType) {
        case 0:
            radius = 10.0f;
            if (com->trailPos < 2) {
                if (com->mode < 4) {
                    if (com->mode < 2) {
                    } else {
                        radius = 150.0f;
                    }
                }
            }
            if (fn_1_BC74(com, &dest, radius) != 0) {
                com->trailPos--;
            }
            break;
        case 1:
            mode = 3;
            goto chase;
        case 2:
            mode = 4;
            goto chase;
        case 3:
            mode = 7;
            goto chase;
        case 4:
            mode = 8;
        chase:
            if (com->trailPos >= 0) {
                if (fn_1_BC74(com, &dest, 10.0f) != 0) {
                    com->trailPos--;
                }
            } else if (fn_1_BC74(com, &dest, 10.0f) != 0) {
                com->player->work[0] = mode;
                com->player->work[1] = 0;
                com->state = 0;
                return;
            }
            break;
    }
    if (com->target >= 0 && com->trailPos < 2) {
        fn_1_BB0C(com, com->target, 90000.0f);
    }
}

void fn_1_D1D8(M576COM *com)
{
    HuVecF v;
    HuVecF diff;
    s32 lost;

    M576_LOST_A(com, lost);
    if (lost != 0) {
        com->timer30 = 0;
        com->state = 0;
        return;
    }
    switch (com->state) {
        case 0:
            fn_1_174(9, &com->targetPos);
            com->state = 1;
            PSVECSubtract(&com->targetPos, &com->player->actor->pos, &diff);
            diff.y = 0.0f;
            if (PSVECSquareMag(&diff) < 100.0f) {
                com->state = 3;
                return;
            }
        case 1:
            com->timer30 = 1;
            fn_1_1BC(&v);
            if (!(v.y > 500.0f) && (com->player->mode == 1 || MgPlayerModeLandSet(com->player, 1) != 0)) {
                com->state = 2;
                return;
            }
            return;
        case 2:
            com->timer30 = 2;
            if (MgPlayerVecChase(com->player, &com->targetPos, 10.0f, 10.0f) != 0) {
                com->state = 3;
                return;
            }
            break;
        case 3:
            com->timer30 = 2;
            if ((com->player->actor->colGroundAttr & 0x407F) != 0 && com->player->mode == 0) {
                com->player->work[0] = 5;
                com->player->work[1] = 0;
                com->state = 0;
            }
            break;
    }
}

void fn_1_D54C(M576COM *com)
{
    HuVecF v;
    HuVecF diff;
    s32 lost;

    M576_LOST_A(com, lost);
    if (lost != 0) {
        com->timer30 = 0;
        com->state = 0;
        return;
    }
    switch (com->state) {
        case 0:
            fn_1_174(10, &com->targetPos);
            com->state = 1;
            PSVECSubtract(&com->targetPos, &com->player->actor->pos, &diff);
            diff.y = 0.0f;
            if (PSVECSquareMag(&diff) < 100.0f) {
                com->state = 3;
                return;
            }
        case 1:
            com->timer30 = 1;
            if (fn_1_234(&v) != 0 && !(v.x > 450.0f) && !(v.x < 380.0f) && (com->player->mode == 1 || MgPlayerModeLandSet(com->player, 1) != 0)) {
                com->state = 2;
                return;
            }
            return;
        case 2:
            com->timer30 = 1;
            if (MgPlayerVecChase(com->player, &com->targetPos, 10.0f, 10.0f) != 0) {
                com->state = 3;
                return;
            }
            break;
        case 3:
            com->timer30 = 1;
            if ((com->player->actor->colGroundAttr & 0x407F) != 0 && com->player->mode == 0) {
                com->player->work[0] = 6;
                com->player->work[1] = 0;
                com->state = 0;
            }
            break;
    }
}

void fn_1_D8E0(M576COM *com)
{
    s32 lost;

    M576_LOST_A(com, lost);
    if (lost != 0) {
        com->timer30 = 0;
        com->state = 0;
        return;
    }
    switch (com->state) {
        case 0:
            if ((com->player->actor->colGroundAttr & 4) == 0) {
                return;
            }
            fn_1_174(6, &com->targetPos);
            com->state = 1;
        case 1:
            if ((com->player->actor->colGroundAttr & 4) != 0) {
                if (com->player->mode == 1 || MgPlayerModeLandSet(com->player, 1) != 0) {
                    com->state = 2;
                }
            }
        default:
            com->timer30 = 1;
            return;
        case 2:
            if (MgPlayerVecChase(com->player, &com->targetPos, 10.0f, 100.0f) != 0) {
                com->state = 3;
            }
            com->timer30 = 1;
            return;
        case 3:
            if ((com->player->actor->colGroundAttr & 0x407F) != 0) {
                com->timer30 = 0;
                return;
            }
            com->timer30 = 1;
            return;
    }
}

void fn_1_DBE0(M576COM *com)
{
    s32 lost;

    M576_LOST_A(com, lost);
    if (lost != 0) {
        com->timer30 = 0;
        com->state = 0;
        return;
    }
    switch (com->state) {
        case 0:
            if ((com->player->actor->colGroundAttr & 8) == 0) {
                return;
            }
            fn_1_174(11, &com->targetPos);
            com->state = 1;
        case 1:
            if ((com->player->actor->colGroundAttr & 8) != 0) {
                if (MgPlayerVecChase(com->player, &com->targetPos, 10.0f, 10.0f) != 0) {
                    fn_1_174(8, &com->targetPos);
                    com->state = 2;
                }
            }
        default:
            com->timer30 = 1;
            return;
        case 2:
            if ((com->player->actor->colGroundAttr & 8) != 0) {
                if (com->player->mode == 1 || MgPlayerModeLandSet(com->player, 1) != 0) {
                    com->state = 3;
                }
                com->timer30 = 1;
                return;
            }
            break;
        case 3:
            if (MgPlayerVecChase(com->player, &com->targetPos, 10.0f, 100.0f) != 0) {
                com->state = 4;
            }
            com->timer30 = 1;
            return;
        case 4:
            if ((com->player->actor->colGroundAttr & 0x407F) != 0) {
                com->timer30 = 0;
                return;
            }
            com->timer30 = 1;
            return;
    }
}

void fn_1_DF40(M576COM *com)
{
    HuVecF v;
    HuVecF diff;
    s32 lost;

    M576_LOST_B(com, lost);
    if (lost != 0) {
        com->timer30 = 0;
        com->state = 0;
        return;
    }
    switch (com->state) {
        case 0:
            fn_1_174(9, &com->targetPos);
            com->state = 1;
            PSVECSubtract(&com->targetPos, &com->player->actor->pos, &diff);
            diff.y = 0.0f;
            if (PSVECSquareMag(&diff) < 100.0f) {
                com->state = 3;
                return;
            }
        case 1:
            com->timer30 = 1;
            fn_1_1BC(&v);
            if (!(v.y > 500.0f) && (com->player->mode == 1 || MgPlayerModeLandSet(com->player, 1) != 0)) {
                com->state = 2;
                return;
            }
            return;
        case 2:
            com->timer30 = 2;
            if (MgPlayerVecChase(com->player, &com->targetPos, 10.0f, 10.0f) != 0) {
                com->state = 3;
                return;
            }
            break;
        case 3:
            com->timer30 = 2;
            if ((com->player->actor->colGroundAttr & 0x407F) != 0 && com->player->mode == 0) {
                com->state = 4;
                return;
            }
            break;
        case 4:
            com->timer30 = 2;
            if (com->mode != 1) {
                if (com->mode >= 1 && com->mode < 4) {
                    if (com->target >= 0) {
                        fn_1_9A4(com->target, &com->targetPos);
                        fn_1_BB0C(com, com->target, 90000.0f);
                    }
                }
            } else if (com->coinTarget >= 0) {
                fn_1_5F4(com->coinTarget, &com->targetPos);
            }
            if (MgPlayerVecChase(com->player, &com->targetPos, 10.0f, 10.0f) != 0) {
                com->player->work[0] = 5;
                com->player->work[1] = 0;
                com->state = 0;
            }
            break;
    }
}

void fn_1_E494(M576COM *com)
{
    HuVecF v;
    HuVecF diff;
    s32 lost;

    M576_LOST_B(com, lost);
    if (lost != 0) {
        com->timer30 = 0;
        com->state = 0;
        return;
    }
    switch (com->state) {
        case 0:
            fn_1_174(10, &com->targetPos);
            com->state = 1;
            PSVECSubtract(&com->targetPos, &com->player->actor->pos, &diff);
            diff.y = 0.0f;
            if (PSVECSquareMag(&diff) < 100.0f) {
                com->state = 3;
                return;
            }
        case 1:
            com->timer30 = 1;
            if (fn_1_234(&v) != 0 && !(v.x > 450.0f) && !(v.x < 380.0f) && (com->player->mode == 1 || MgPlayerModeLandSet(com->player, 1) != 0)) {
                com->state = 2;
                return;
            }
            return;
        case 2:
            com->timer30 = 1;
            if (MgPlayerVecChase(com->player, &com->targetPos, 10.0f, 10.0f) != 0) {
                com->state = 3;
                return;
            }
            break;
        case 3:
            com->timer30 = 1;
            if ((com->player->actor->colGroundAttr & 0x407F) != 0 && com->player->mode == 0) {
                com->state = 4;
                return;
            }
            break;
        case 4:
            com->timer30 = 2;
            if (com->mode != 1) {
                if (com->mode >= 1 && com->mode < 4) {
                    if (com->target >= 0) {
                        fn_1_9A4(com->target, &com->targetPos);
                        fn_1_BB0C(com, com->target, 90000.0f);
                    }
                }
            } else if (com->coinTarget >= 0) {
                fn_1_5F4(com->coinTarget, &com->targetPos);
            }
            if (MgPlayerVecChase(com->player, &com->targetPos, 10.0f, 10.0f) != 0) {
                com->player->work[0] = 6;
                com->player->work[1] = 0;
                com->state = 0;
            }
            break;
    }
}
