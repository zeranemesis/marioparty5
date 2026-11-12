#include "REL/m568Dll.h"

/*
 * octtree.cpp: the octree node walk (bounding-box search over 0x40-byte
 * nodes) and the sound/model effect object that sits next to it.
 *
 * Octree nodes are loaded from a file with 32-bit child indices (-1 = none);
 * the GC build rewrites them in place into pointers. The PC build cannot do
 * that (pointers are 8 bytes), so the children stay indices and are resolved
 * against the node array base on every step.
 */

#ifdef __MWERKS__
#define OCT_RODATA __declspec(section ".rodata")
#else
#define OCT_RODATA
#endif

struct OctBox {
    f32 min[3];  // 0x00
    f32 max[3];  // 0x0C
};

struct OctNode;

// File value is an index into the node array (-1 = none). On GC it is
// rewritten in place into a pointer; PC keeps the 4-byte index.
union OctLink {
    s32 idx;
#ifndef TARGET_PC
    OctNode *ptr;
#endif
};

struct OctNode {
    s32 hdr[2];       // 0x00
    OctBox box;       // 0x08
    OctLink child[8]; // 0x20
};

typedef M568Effect OctEffect;

extern "C" {

void fn_1_1948(void *obj, M568Effect *sm);
void fn_1_32AC(M568Model *model, s32 dataNum, s32 arg);
void fn_1_3420(M568Model *model, u32 arg);
void fn_1_3458(M568Model *model, u32 arg);
void fn_1_350C(M568Model *model, Vec *pos);
void fn_1_D690(Vec *pos);
void fn_1_E7C8(Vec *pos, s32 handle);
s32 HuAudFXPlay(s32 seId);

char lbl_1_data_560[] = "ms_pointer != 0";
char lbl_1_data_570[] = "singleton.h";
char lbl_1_data_57C[] = "sm_ptr != 0";
char lbl_1_data_588[] = "a_set.h";
char lbl_1_data_590[] = "p != parent";
char lbl_1_data_59C[] = "octtree.cpp";

OCT_RODATA const s32 lbl_1_rodata_3F0[2] = { 0x007B0004, 0x007B0005 };

OctEffect *fn_1_F7A0(OctEffect *self, s32 type, Vec *pos)
{
    M568PosOwner *owner;
    Vec *src;

    fn_1_187C(self);
    self->pos = *pos;
    self->radius = 100.0f;
    fn_1_32AC(&self->model, lbl_1_rodata_3F0[type], type * 4);
    self->played = 0;
    self->seId = (type == 0) ? 0x90A : 0x90B;
    if (lbl_1_bss_9F0 != 0) {
    } else {
        fn_1_7EC(lbl_1_data_560, lbl_1_data_570, 0x27);
    }
    owner = lbl_1_bss_9F0;
    fn_1_D690(&self->pos);
    src = owner->src.GetPos();
    self->cur.x = src->x;
    self->cur.y = src->y;
    self->cur.z = src->z;
    if (lbl_1_bss_9F4 != 0) {
    } else {
        fn_1_7EC(lbl_1_data_57C, lbl_1_data_588, 0x3A);
    }
    fn_1_1948(self, lbl_1_bss_9F4);
    fn_1_350C(&self->model, pos);
    fn_1_3420(&self->model, 0x40000080);
    return self;
}

OctEffect *fn_1_F938(OctEffect *self, s16 flags)
{
    if (self != 0) {
        fn_1_1888(self, 0);
        if (flags > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

void fn_1_F990(OctEffect *self, u8 *trigger)
{
    if (self->played == 0 && trigger[8] != 0) {
        fn_1_3458(&self->model, 0x40000080);
        fn_1_E7C8(&self->pos, HuAudFXPlay(self->seId));
        self->played = 1;
    }
}

// Converts the child indices of nodes [nodes, nodes + count) into pointers.
void fn_1_FA10(OctNode *nodes, s32 count)
{
    OctNode *n;
    s32 i;

    for (n = nodes; n != nodes + count; n++) {
        for (i = 0; i < 8; i++) {
#ifdef TARGET_PC
            // Indices are kept as-is on PC; FB30 resolves them.
#else
            if (n->child[i].idx == -1) {
                n->child[i].ptr = 0;
            } else {
                n->child[i].ptr = nodes + n->child[i].idx;
            }
#endif
        }
    }
}

s32 fn_1_FAB0(OctBox *box, f32 x, f32 y, f32 z)
{
    if (box->min[0] > x) {
        return 0;
    }
    if (box->min[1] > y) {
        return 0;
    }
    if (box->min[2] > z) {
        return 0;
    }
    if (box->max[0] < x) {
        return 0;
    }
    if (box->max[1] < y) {
        return 0;
    }
    if (box->max[2] < z) {
        return 0;
    }
    return 1;
}

static u8 OctContains(OctNode *n, f32 x, f32 y, f32 z)
{
    u8 r;
    if (n->box.min[0] > x) {
        r = 0;
    } else if (n->box.min[1] > y) {
        r = 0;
    } else if (n->box.min[2] > z) {
        r = 0;
    } else if (n->box.max[0] < x) {
        r = 0;
    } else if (n->box.max[1] < y) {
        r = 0;
    } else if (n->box.max[2] < z) {
        r = 0;
    } else {
        r = 1;
    }
    return r;
}

// Descends to the deepest node containing (x, y, z); 0 if outside the root.
OctNode *fn_1_FB30(OctNode *root, f32 x, f32 y, f32 z)
{
    OctNode *p = root;
    OctNode *parent;
    OctNode *c;
    s32 i;

    if (OctContains(p, x, y, z) == 0) {
        return 0;
    }
#ifdef TARGET_PC
    while (p->child[0].idx != -1) {
#else
    while (p->child[0].ptr != 0) {
#endif
        parent = p;
        for (i = 0; i < 8; i++) {
#ifdef TARGET_PC
            c = root + p->child[i].idx;
#else
            c = p->child[i].ptr;
#endif
            if (OctContains(c, x, y, z) != 0) {
                p = c;
                break;
            }
        }
        if (p != parent) {
        } else {
            fn_1_7EC(lbl_1_data_590, lbl_1_data_59C, 0x56);
        }
    }
    return p;
}

}
