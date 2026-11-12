// m539Dll "drum": the drum prop (three models, a spin state machine) and the
// model cache it draws from -- a red-black tree keyed by data number, the
// same tree layout an STL map uses. Its nodes pack parent and colour into one
// 32-bit word on the GameCube; the PC build keeps them apart so the parent can
// be a real pointer.
#define _MATH_H
extern "C" {
#include "dolphin.h"
#include "humath.h"
#include "game/hu3d.h"
#include "game/memory.h"
}

extern "C" {

void abort(void);

// ---------------------------------------------------------------- types

typedef struct M539Node_s {
    struct M539Node_s *next;
    struct M539Node_s *prev;
} M539Node;

typedef struct M539Model_s {
    HU3DMODELID id;
} M539Model;

// Tree node: left, right, parent/colour, then the (key, value) pair.
typedef struct M539TNode_s {
    struct M539TNode_s *left;
    struct M539TNode_s *right;
#ifdef TARGET_PC
    struct M539TNode_s *parent;
    u32 red;
#else
    u32 parentRed;
#endif
    u32 key;
    M539Model *value;
} M539TNode;

typedef struct M539Pair_s {
    u32 key;
    M539Model *value;
} M539Pair;

// The end() node of the tree overlays the tree's own head: only left (root)
// and right (leftmost) are ever read through it.
typedef struct M539THead_s {
    M539TNode *left;
    M539TNode *right;
} M539THead;

typedef struct M539Tree_s {
    u32 size;
    M539THead head;
} M539Tree;

typedef struct M539InsertResult_s {
    M539TNode *node;
    u8 inserted;
} M539InsertResult;

typedef struct M539Drum_s {
    M539Node link;
    f32 angle;
    f32 speed;
    s32 state;
    u8 pad14[0x24 - 0x14];
    M539Model *mdl[3];
    s32 timer;
} M539Drum;

typedef struct M539Score_s {
    s32 pad0;
    s32 score;
} M539Score;

#ifdef TARGET_PC
#define M539_PARENT_RAW(n) ((n)->parent)
#define M539_SET_PARENT(n, p) ((n)->parent = (p))
#define M539_IS_RED(n) ((n)->red != 0)
#define M539_SET_RED(n) ((n)->red = 1)
#define M539_SET_BLACK(n) ((n)->red = 0)
#else
#define M539_PARENT_RAW(n) ((M539TNode *)((n)->parentRed & ~1))
#define M539_SET_PARENT(n, p) ((n)->parentRed = (u32)(p) | ((n)->parentRed & 1))
#define M539_IS_RED(n) (((n)->parentRed & 1) != 0)
#define M539_SET_RED(n) ((n)->parentRed |= 1)
#define M539_SET_BLACK(n) ((n)->parentRed &= ~1)
#endif

// Parent accessor: the original goes through iterator copies, one per access.
inline M539TNode *M539ParentGet(M539TNode *n)
{
    M539TNode *first = M539_PARENT_RAW(n);
    M539TNode *second = first;
    return second;
}

#define M539_PARENT(n) M539ParentGet(n)

// ---------------------------------------------------------------- data

char lbl_1_data_258[] = "ms_pointer == 0";
char lbl_1_data_268[] = "singleton.h";
char lbl_1_data_274[] = "ms_pointer != 0";
extern const u8 lbl_1_data_284 = 1;
extern const u8 lbl_1_data_285 = 0;
char lbl_1_data_286[] = "tree::insert length error\n";
char lbl_1_data_2A1[] = "Memory allocation failure";

M539Tree *lbl_1_bss_9F0;

// ---------------------------------------------------------------- externs (m539.cpp)

void *fn_1_0(s32 size);
void fn_1_C8(void *ptr);
void fn_1_1264(M539Node *node);
void *fn_1_1270(M539Node *node, s16 flag);
void fn_1_1D4(const char *expr, const char *file, s32 line);
int fn_1_234(void *unused, const char *fmt, ...);
M539Model *fn_1_2C94(M539Model *self, s32 dataNum);
M539Model *fn_1_2DB4(M539Model *self, M539Model *parent);
void fn_1_2E08(M539Model *self, u32 attr);
void fn_1_2E40(M539Model *self, u32 attr);
void fn_1_2EF4(M539Model *self, Vec *pos);
void fn_1_3084(M539Model *self, u16 camBit);
MtxPtr fn_1_30D8(M539Model *self);
M539Model *fn_1_30F4(M539Model *self, s16 flag);
void fn_1_3660(M539Model *self, s32 time);
f32 fn_1_4FC8(f32 from, f32 to);

#define M539_ASSERT(cond, expr, file, line) ((cond) ? (void)0 : fn_1_1D4((expr), (file), (line)))

// stderr stand-in handed to the (ignored) stream argument of fn_1_234.
#ifdef TARGET_PC
#define M539_STDERR 0
#else
extern u8 __files[];
#define M539_STDERR (__files + 0xA0)
#endif

M539Model *fn_1_72E8(u32 dataNum);
M539TNode **fn_1_745C(M539TNode **out, M539Tree *tree, u32 *key);
void fn_1_751C(M539Tree *tree, M539TNode *node);
void fn_1_797C(void *p);
void fn_1_7980(void *p);
void fn_1_7984(M539TNode *node, s32 flag);
void *fn_1_79AC(M539Tree *tree);
void fn_1_79B4(void *p);
void fn_1_79B8(void *p);
M539InsertResult *fn_1_79BC(M539InsertResult *out, M539Tree *tree, M539Pair *pair);
void fn_1_7AD0(M539Tree *tree, u8 *cmp, u8 *alloc);
M539TNode *fn_1_7B10(M539Tree *tree, M539TNode *parent, u8 goLeft, u8 setLeftmost, M539Pair *pair);
void fn_1_7C78(M539TNode *node, M539TNode *root);
void fn_1_80B8(M539TNode *node, M539TNode **root);
void fn_1_8180(M539TNode *node, M539TNode **root);

// ---------------------------------------------------------------- drum

M539Drum *fn_1_6704(M539Drum *self, M539Model *anchor, char *nodeName, s32 camIdx)
{
    Mtx m;
    Vec pos;
    M539Model *a0;
    M539Model *a1;
    M539Model *a2;
    M539Model *p0;
    M539Model *p1;
    M539Model *p2;
    M539Model *idCopy;
    HU3DMODELID id0;
    HU3DMODELID id1;
    HU3DMODELID id2;
    HU3DMODELID anchorId;
    M539Model *anchorCopy;
    M539Model *c0;
    M539Model *c1;
    M539Model *c2;
    M539Model *r1;
    M539Model *r2;
    M539Model *t0;
    M539Model *t1;
    M539Model *t2;

    fn_1_1264(&self->link);
    self->mdl[0] = fn_1_72E8(0x610005);
    self->mdl[1] = fn_1_72E8(0x610006);
    self->mdl[2] = fn_1_72E8(0x610007);
    self->timer = 0;
    c0 = self->mdl[0];
    fn_1_3084(c0, 1 << camIdx);
    c1 = self->mdl[1];
    fn_1_3084(c1, 1 << camIdx);
    c2 = self->mdl[2];
    fn_1_3084(c2, 1 << camIdx);
    r2 = self->mdl[2];
    fn_1_2E40(r2, 0x40000001);
    r1 = self->mdl[1];
    fn_1_2E40(r1, 0x40000001);
    t0 = self->mdl[0];
    fn_1_3660(t0, 10);
    t2 = self->mdl[2];
    fn_1_3660(t2, 0);
    t1 = self->mdl[1];
    fn_1_3660(t1, 0);
    a0 = self->mdl[0];
    fn_1_2E08(a0, 0x40000002);
    a2 = self->mdl[2];
    fn_1_2E08(a2, 0x40000002);
    a1 = self->mdl[1];
    fn_1_2E08(a1, 0x40000002);
    idCopy = self->mdl[0];
    id0 = idCopy->id;
    id1 = id0;
    id2 = id1;
    anchorId = anchor->id;
    anchorCopy = anchor;
    Hu3DModelObjMtxGet(anchorId, nodeName, m);
    pos.x = m[0][3];
    pos.y = m[1][3];
    pos.z = m[2][3];
    p0 = self->mdl[0];
    fn_1_2EF4(p0, &pos);
    p1 = self->mdl[1];
    fn_1_2EF4(p1, &pos);
    p2 = self->mdl[2];
    fn_1_2EF4(p2, &pos);
    self->angle = (f32)((s32)fn_1_4FC8(0.0f, 2.5f) * 0x78);
    self->state = 0;
    return self;
}

// Owner of one model.
M539Model **fn_1_6974(M539Model **self, s16 flag)
{
    if (self != 0) {
        fn_1_30F4(*self, 1);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M539Drum *fn_1_69CC(M539Drum *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(&self->link, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_6A24(M539Drum *self)
{
    s32 c;
    u8 hit;

    if (self->mdl[0] != 0) {
        switch (self->state) {
            case 0:
                break;
            case 1:
                self->angle = self->angle + self->speed;
                break;
            case 2:
                self->state = 3;
                break;
        }
        hit = 0;
        if (self->timer > 0) {
            c = self->timer - 1;
            self->timer = c;
            if (c == 0) {
                hit = 1;
            }
        }
        if (hit != 0) {
            Hu3DMotionSpeedSet(self->mdl[0]->id, 0.3f);
            fn_1_2E08(self->mdl[0], 0x40000004);
        }
        PSMTXRotRad(fn_1_30D8(self->mdl[0]), 'X', 0.017453292f * self->angle);
    }
}

void fn_1_6B44(M539Drum *self)
{
    self->speed = 48.0f;
    self->state = 1;
}

void fn_1_6C64(M539Drum *self);

void fn_1_6B60(M539Drum *self)
{
    self->state = 2;
    self->angle = (f32)((s32)fn_1_4FC8(0.0f, 2.5f) * 0x78);
    fn_1_6C64(self);
}

void fn_1_6BEC(M539Drum *self)
{
    M539Model *top;
    M539Model *m1;
    M539Model *m2;

    top = self->mdl[0];
    if (top != 0) {
        self->timer = 3;
        m1 = self->mdl[0];
        fn_1_2E40(m1, 0x40000002);
        m2 = self->mdl[0];
        fn_1_3660(m2, 0);
    }
}

void fn_1_6C64(M539Drum *self)
{
    M539Model *top;
    M539Model *a;
    M539Model *b;
    M539Model *c;
    M539Model *d;
    M539Model *e;
    M539Model *f;
    M539Model *g;
    M539Model *h;

    top = self->mdl[0];
    if (top != 0) {
        a = self->mdl[0];
        Hu3DMotionSpeedSet(a->id, 1.0f);
        b = self->mdl[1];
        fn_1_2E40(b, 0x40000002);
        c = self->mdl[1];
        fn_1_3660(c, 0);
        if (self->state == 3) {
            d = self->mdl[2];
            fn_1_2E40(d, 0x40000002);
            e = self->mdl[2];
            fn_1_3660(e, 0);
            f = self->mdl[0];
            fn_1_2E40(f, 0x40000002);
            g = self->mdl[0];
            fn_1_2E40(g, 0x40000004);
            h = self->mdl[0];
            fn_1_3660(h, 0);
        }
    }
}

void fn_1_6D54(M539Drum *self)
{
    if (self->mdl[0] != 0) {
        fn_1_30F4(self->mdl[0], 1);
        self->mdl[0] = 0;
    }
}

s32 fn_1_6DA0(M539Drum *self)
{
    return self->state >= 2;
}

// Which score band the drum is showing: 0..2 by spin angle, 3 gone, 4 not yet stopped.
s32 fn_1_6DBC(M539Drum *self)
{
    if (self->mdl[0] == 0) {
        return 3;
    }
    if (self->state < 2) {
        return 4;
    }
    if (self->angle < 120.0f) {
        return 1;
    }
    if (self->angle < 240.0f) {
        return 2;
    }
    return 0;
}

void fn_1_6E3C(M539Score *self)
{
    self->score = 0;
}

// Award points when the drum band `band` matches the hit (`hit` 3 = any).
s32 fn_1_6E48(M539Score *self, s32 band, s32 hit, s32 want)
{
    s32 pts;

    pts = 0;
    if (band != want) {
        return 0;
    }
    if (band != hit && hit != 3) {
        return 0;
    }
    switch (band) {
        case 0:
            pts = 5;
            break;
        case 1:
            pts = 10;
            break;
        case 2:
            pts = 50;
            break;
    }
    self->score = self->score + pts;
    return pts;
}

// ---------------------------------------------------------------- model cache (singleton)

M539Tree *fn_1_6ED0(M539Tree *self)
{
    M539Tree *tree;
    u8 cmp;
    u8 alloc;

    M539_ASSERT(lbl_1_bss_9F0 == 0, lbl_1_data_258, lbl_1_data_268, 0x35);
    lbl_1_bss_9F0 = self;
    tree = self;
    fn_1_7AD0(tree, &cmp, &alloc);
    return self;
}

M539Tree *fn_1_6F54(M539Tree *self, s16 flag)
{
    M539THead *h1;
    M539THead *h2;
    M539THead *h3;
    M539THead *h4;

    if (self != 0) {
        if (self != 0) {
            h1 = &self->head;
            h2 = h1;
            if (h2->left != 0) {
                h3 = &self->head;
                h4 = h3;
                fn_1_751C(self, h4->left);
            }
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M539Tree *fn_1_6FD8(M539Tree *self, s16 flag)
{
    M539THead *h1;
    M539THead *h2;
    M539THead *h3;
    M539THead *h4;

    if (self != 0) {
        h1 = &self->head;
        h2 = h1;
        if (h2->left != 0) {
            h3 = &self->head;
            h4 = h3;
            fn_1_751C(self, h4->left);
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M539Tree *fn_1_7054(M539Tree *self, s16 flag)
{
    if (self != 0) {
        M539_ASSERT(lbl_1_bss_9F0 != 0, lbl_1_data_274, lbl_1_data_268, 0x3D);
        lbl_1_bss_9F0 = 0;
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M539Tree *fn_1_70E0(M539Tree *self, s16 flag)
{
    if (self != 0) {
        if (self != 0 && self != 0 && self->head.left != 0) {
            fn_1_751C(self, self->head.left);
        }
        if (self != 0) {
            M539_ASSERT(lbl_1_bss_9F0 != 0, lbl_1_data_274, lbl_1_data_268, 0x3D);
            lbl_1_bss_9F0 = 0;
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// Model for `dataNum` from an explicit cache: a link to the cached model if
// there is one, else a fresh load that is then cached.
M539Model *fn_1_71B4(M539Tree *tree, u32 dataNum)
{
    M539TNode *found;
    M539TNode *end;
    M539Model *mdl;
    M539Model *fresh;
    M539Pair pair;
    M539InsertResult res;
    u32 key;
    u32 key2;
    M539TNode *f2;
    M539Model *fv;
    M539Model *back;
    M539Model *mdl2;
    M539TNode *it1;
    M539TNode *it2;
    M539TNode *e2;
    M539TNode *r1;
    M539TNode *r3;
    M539TNode *r4;
    u8 r2;
    u8 r5;

    key = dataNum;
    fn_1_745C(&found, tree, &key);
    it1 = found;
    it2 = found;
    end = (M539TNode *)&tree->head;
    e2 = end;
    if (it2 == e2) {
        mdl = (M539Model *)fn_1_0(sizeof(M539Model));
        if (mdl != 0) {
            mdl = fn_1_2C94(mdl, dataNum);
        }
        key2 = key;
        mdl2 = mdl;
        pair.key = key2;
        pair.value = mdl2;
        fn_1_79BC(&res, tree, &pair);
        r1 = res.node;
        r2 = res.inserted;
        r3 = res.node;
        r4 = res.node;
        r5 = res.inserted;
        return mdl;
    }
    fresh = (M539Model *)fn_1_0(sizeof(M539Model));
    if (fresh != 0) {
        f2 = found;
        fv = f2->value;
        fresh = fn_1_2DB4(fresh, fv);
    }
    back = fresh;
    return back;
}

M539Model *fn_1_72E8(u32 dataNum)
{
    M539Tree *tree;
    M539TNode *found;
    M539TNode *end;
    M539Model *mdl;
    M539Model *fresh;
    M539Model *ret;
    M539Pair pair;
    M539InsertResult res;
    u32 key;
    u32 key2;
    M539Model *mdl2;
    M539TNode *it1;
    M539TNode *it2;
    M539TNode *e2;
    M539TNode *r1;
    M539TNode *r3;
    M539TNode *r4;
    u8 r2;
    u8 r5;

    M539_ASSERT(lbl_1_bss_9F0 != 0, lbl_1_data_274, lbl_1_data_268, 0x27);
    tree = lbl_1_bss_9F0;
    key = dataNum;
    fn_1_745C(&found, tree, &key);
    it1 = found;
    it2 = found;
    end = (M539TNode *)&tree->head;
    e2 = end;
    if (it2 == e2) {
        mdl = (M539Model *)fn_1_0(sizeof(M539Model));
        if (mdl != 0) {
            mdl = fn_1_2C94(mdl, dataNum);
        }
        key2 = key;
        mdl2 = mdl;
        pair.key = key2;
        pair.value = mdl2;
        fn_1_79BC(&res, tree, &pair);
        r1 = res.node;
        r2 = res.inserted;
        r3 = res.node;
        r4 = res.node;
        r5 = res.inserted;
        ret = mdl;
    } else {
        fresh = (M539Model *)fn_1_0(sizeof(M539Model));
        if (fresh != 0) {
            fresh = fn_1_2DB4(fresh, found->value);
        }
        ret = fresh;
    }
    return ret;
}

// ---------------------------------------------------------------- tree internals

// find(): iterator to `*key`, or end().
M539TNode **fn_1_745C(M539TNode **out, M539Tree *tree, u32 *key)
{
    M539TNode **ret;
    M539TNode **retCopy;
    M539TNode *node;
    M539TNode *best;
    M539TNode *e1;
    M539TNode *e2;
    M539TNode *e3;
    M539TNode *e4;
    M539THead *h1;
    M539THead *h2;
    M539THead *h3;
    M539THead *h4;
    M539THead *h5;
    M539THead *h6;

    ret = out;
    h1 = &tree->head;
    h2 = h1;
    node = h2->left;
    h3 = &tree->head;
    best = (M539TNode *)h3;
    while (node != 0) {
        if (node->key >= *key) {
            best = node;
            node = node->left;
        } else {
            node = node->right;
        }
    }
    h4 = &tree->head;
    e1 = (M539TNode *)h4;
    if (best == e1 || *key < best->key) {
        h5 = &tree->head;
        h6 = h5;
        e2 = (M539TNode *)h6;
        *ret = e2;
        retCopy = ret;
        e3 = *retCopy;
        return ret;
    }
    *ret = best;
    retCopy = ret;
    e4 = *retCopy;
    return ret;
}

// Free every node of the subtree.
void fn_1_751C(M539Tree *tree, M539TNode *node)
{
    if (node->left != 0) {
        fn_1_751C(tree, node->left);
    }
    if (node->right != 0) {
        fn_1_751C(tree, node->right);
    }
    fn_1_79B8(tree);
    fn_1_79B4(&node->key);
    fn_1_79AC(tree);
    fn_1_7984(node, 1);
}

void fn_1_797C(void *p)
{
}

void fn_1_7980(void *p)
{
}

void fn_1_7984(M539TNode *node, s32 flag)
{
    fn_1_C8(node);
}

void *fn_1_79AC(M539Tree *tree)
{
    return &tree->head;
}

void fn_1_79B4(void *p)
{
}

void fn_1_79B8(void *p)
{
}

// insert(): unique-key insert of `*pair`.
M539InsertResult *fn_1_79BC(M539InsertResult *out, M539Tree *tree, M539Pair *pair)
{
    M539TNode *prev;
    M539TNode *parent;
    M539TNode *node;
    M539TNode *made;
    M539TNode *same;
    M539TNode *par2;
    M539TNode *end2;
    u8 left2;
    u8 lm2;
    u8 goLeft;
    u8 leftmost;

    prev = 0;
    parent = (M539TNode *)&tree->head;
    node = tree->head.left;
    goLeft = 1;
    leftmost = 1;
    while (node != 0) {
        parent = node;
        if (pair->key < node->key) {
            node = node->left;
            goLeft = 1;
        } else {
            prev = parent;
            node = node->right;
            goLeft = 0;
            leftmost = 0;
        }
    }
    if (prev == 0 || prev->key < pair->key) {
        par2 = parent;
        left2 = goLeft;
        lm2 = leftmost;
        made = fn_1_7B10(tree, par2, left2, lm2, pair);
        out->node = made;
        out->inserted = lbl_1_data_284;
        return out;
    }
    same = prev;
    out->node = same;
    out->inserted = lbl_1_data_285;
    return out;
}

// Tree constructor: empty, with the head as its own leftmost.
void fn_1_7AD0(M539Tree *tree, u8 *cmp, u8 *alloc)
{
    M539TNode *end;
    M539TNode **leftmost;

    tree->size = 0;
    tree->head.left = 0;
#ifndef TARGET_PC
    *(u8 *)&tree->head.right = *cmp;
#endif
    end = (M539TNode *)&tree->head;
    leftmost = &tree->head.right;
    *leftmost = end;
}

M539TNode *fn_1_7B10(M539Tree *tree, M539TNode *parent, u8 goLeft, u8 setLeftmost, M539Pair *pair)
{
    M539TNode *node;

    if (tree->size > 0xFFFFFFFE) {
        fn_1_234(M539_STDERR, lbl_1_data_286);
        abort();
    }
    node = (M539TNode *)fn_1_0(sizeof(M539TNode));
    if (node == 0) {
        fn_1_234(M539_STDERR, lbl_1_data_2A1);
        abort();
    }
    if (&node->key != 0) {
        node->key = pair->key;
        node->value = pair->value;
    }
    node->right = 0;
    node->left = 0;
    M539_SET_PARENT(node, parent);
    if (goLeft != 0) {
        parent->left = node;
    } else {
        parent->right = node;
    }
    tree->size = tree->size + 1;
    fn_1_7C78(node, tree->head.left);
    if (setLeftmost != 0) {
        tree->head.right = node;
    }
    return node;
}

// Red-black rebalance after inserting `node` below `root`.
void fn_1_7C78(M539TNode *node, M539TNode *root)
{
    M539TNode *x;
    M539TNode *uncle;

    x = node;
    M539_SET_RED(x);
    while (x != root && M539_IS_RED(M539_PARENT(x))) {
        if (M539_PARENT(x) == M539_PARENT(M539_PARENT(x))->left) {
            uncle = M539_PARENT(M539_PARENT(x))->right;
            if (uncle != 0 && M539_IS_RED(uncle)) {
                M539_SET_BLACK(M539_PARENT(x));
                M539_SET_BLACK(uncle);
                x = M539_PARENT(M539_PARENT(x));
                M539_SET_RED(x);
            } else {
                if (x == M539_PARENT(x)->right) {
                    x = M539_PARENT(x);
                    fn_1_8180(x, &root);
                }
                M539_SET_BLACK(M539_PARENT(x));
                M539_SET_RED(M539_PARENT(M539_PARENT(x)));
                fn_1_80B8(M539_PARENT(M539_PARENT(x)), &root);
            }
        } else {
            uncle = M539_PARENT(M539_PARENT(x))->left;
            if (uncle != 0 && M539_IS_RED(uncle)) {
                M539_SET_BLACK(M539_PARENT(x));
                M539_SET_BLACK(uncle);
                x = M539_PARENT(M539_PARENT(x));
                M539_SET_RED(x);
            } else {
                if (x == M539_PARENT(x)->left) {
                    x = M539_PARENT(x);
                    fn_1_80B8(x, &root);
                }
                M539_SET_BLACK(M539_PARENT(x));
                M539_SET_RED(M539_PARENT(M539_PARENT(x)));
                fn_1_8180(M539_PARENT(M539_PARENT(x)), &root);
            }
        }
    }
    M539_SET_BLACK(root);
}

// Rotate right around `node`.
void fn_1_80B8(M539TNode *node, M539TNode **root)
{
    M539TNode *y;

    y = node->left;
    if (*root == node) {
        *root = y;
    }
    node->left = y->right;
    if (y->right != 0) {
        M539_SET_PARENT(y->right, node);
    }
    M539_SET_PARENT(y, M539_PARENT(node));
    if (node == M539_PARENT(node)->left) {
        M539_PARENT(node)->left = y;
    } else {
        M539_PARENT(node)->right = y;
    }
    y->right = node;
    M539_SET_PARENT(node, y);
}

// Rotate left around `node`.
void fn_1_8180(M539TNode *node, M539TNode **root)
{
    M539TNode *y;

    y = node->right;
    if (*root == node) {
        *root = y;
    }
    node->right = y->left;
    if (y->left != 0) {
        M539_SET_PARENT(y->left, node);
    }
    M539_SET_PARENT(y, M539_PARENT(node));
    if (node == M539_PARENT(node)->left) {
        M539_PARENT(node)->left = y;
    } else {
        M539_PARENT(node)->right = y;
    }
    y->left = node;
    M539_SET_PARENT(node, y);
}

}
