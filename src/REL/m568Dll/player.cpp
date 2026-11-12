#define _MATH_H /* the original object has no math.h sqrt literals */
#include "types.h"
#include "game/hu3d.h"
#include "game/gamework.h"
extern "C" {
#include "game/charman.h"
}
#include "game/object.h"
#include "datadir_enum.h"
#include "REL/m568Dll.h"
#ifdef TARGET_PC
#include <stdio.h>
#include <stdlib.h>
#endif

/*
 * player.cpp: one player of the minigame. It owns an input source (human pad
 * or CPU), a character model, the physics body (M568Tube, tube.cpp) and a
 * vector of motion ids; a pointer-to-member state hook picks the behaviour.
 */

#ifdef __MWERKS__
#define PLR_RODATA __declspec(section ".rodata")
#else
#define PLR_RODATA
#endif

// The input classes (input.cpp) differ in size; the PC build over-allocates
// because their extra members hold pointers.
#ifdef TARGET_PC
#define PLR_ALLOC(n) ((n) * 2)
#else
#define PLR_ALLOC(n) (n)
#endif

// Input source base (input.cpp); slot 8 of its vtable is the deleting destructor.
struct PlrInput {
    void **vtbl;  // 0x00
    u8 f4;        // 0x04
    u8 f5;        // 0x05
    f32 f8;       // 0x08 stick x
    f32 fC;       // 0x0C
};

// vector<s32>: capacity first (MSL-style layout of this build).
struct PlrVec {
    u32 cap;    // 0x00
    u32 size;   // 0x04
    s32 *data;  // 0x08
};

struct Player568 {
    M568Link node;     // 0x00
    PlrInput *input;   // 0x08
    M568Ptmf hook;     // 0x0C
    s32 padNo;         // 0x18
    s32 charNo;        // 0x1C
    s32 motion;        // 0x20
    f32 spin;          // 0x24
    u8 first;          // 0x28
    Vec pos;           // 0x2C
    f32 f38;           // 0x38
    s32 f3C;           // 0x3C
    u8 f40;            // 0x40
    PlrVec motions;    // 0x44
    M568Model *model;  // 0x50
    M568Tube *tube;    // 0x54
};

extern "C" {

s32 __ptmf_test(void *ptmf);
void __ptmf_scall(void *obj);
#ifndef TARGET_PC
void abort(void);
extern u8 __files[];
#endif

void *fn_1_618(u32 size);
void *fn_1_1948(void *obj, void *list);
s32 fn_1_2548(void);
void fn_1_2B74(s32 padNo);
M568Model *fn_1_3330(M568Model *self, s32 charNo, s32 arg);
void fn_1_3420(M568Model *model, u32 attr);
MtxPtr fn_1_36F0(M568Model *model);
BOOL fn_1_5634(Vec *a, Vec *b);
void fn_1_628C(PlrInput *input, M568Tube *tube);
PlrInput *fn_1_62F4(PlrInput *self, s32 padNo);
PlrInput *fn_1_6428(PlrInput *self);
PlrInput *fn_1_7658(PlrInput *self);
void fn_1_84C(void *file, const char *msg);
u8 fn_1_D690(Vec *pos);
M568Tube *fn_1_12058(M568Tube *self, s32 arg, s32 charNo, s32 id);
void fn_1_123DC(M568Tube *self, s16 flag);
void fn_1_12448(M568Tube *self, Vec *pos);
void fn_1_1262C(M568Tube *self, Quaternion *out);
void fn_1_12650(M568Tube *self, Vec *v);
void fn_1_1268C(M568Tube *self);
void fn_1_1289C(M568Tube *self);
u8 fn_1_14208(M568Tube *self);

extern SceneNode *lbl_1_bss_A0C;

s32 lbl_1_bss_A80;
s32 lbl_1_bss_A84;
s32 lbl_1_bss_A88;

#ifdef TARGET_PC
#define PLR_STDERR ((void *)stderr)
#else
#define PLR_STDERR ((void *)(__files + 0xA0))
#endif

static void fn_1_1019C(Player568 *self, f32 a, f32 b);
static void fn_1_10E14(Player568 *self, s32 dataNum);
static void fn_1_10F54(Player568 *self, s32 motion, u8 arg);
static void fn_1_11268(Player568 *self);
static void fn_1_11380(Player568 *self);
static void fn_1_11630(Player568 *self, f32 t);
static void fn_1_11784(Player568 *self);
static void fn_1_1184C(Player568 *self);
static s32 *fn_1_11914(PlrVec *v, u32 idx);
static void fn_1_1198C(M568Link *list, s32 *count);
static void fn_1_11A4C(PlrVec *v, s32 *pos, u32 n, s32 *val);
static void fn_1_11E04(PlrVec *v, u32 n);
void fn_1_FFE4(Player568 *self);
void fn_1_100F8(Player568 *self);
void fn_1_11098(Player568 *self);
void fn_1_11230(Player568 *self);

M568Ptmf lbl_1_data_5A8 = { 0, -1, (void (*)(void *))fn_1_100F8 };
M568Ptmf lbl_1_data_5B4 = { 0, -1, (void (*)(void *))fn_1_100F8 };
char lbl_1_data_5C0[] = "sm_ptr != 0";
char lbl_1_data_5CC[] = "a_set.h";
char lbl_1_data_5D4[] = "DIE!";
char lbl_1_data_5D9[] = "!\"DIE!\"";
char lbl_1_data_5E1[] = "player.cpp";
M568Ptmf lbl_1_data_5EC = { 0, -1, (void (*)(void *))fn_1_11098 };
char lbl_1_data_5F8[] = "m_mot_id.size() == NUMBEROF_MOTION";
M568Ptmf lbl_1_data_61C = { 0, -1, (void (*)(void *))fn_1_11230 };
char lbl_1_data_628[] = "vector::at index out of range";
char lbl_1_data_646[] = "vector::insert length error\n";
char lbl_1_data_663[] = "Memory allocation failure";
char lbl_1_data_67D[] = "vector::reserve length error\n";

PLR_RODATA const f32 lbl_1_rodata_414[4] = { -300.0f, 0.0f, 300.0f, 600.0f };
PLR_RODATA const f32 lbl_1_rodata_450[3] = { 0.0f, 0.0f, 0.0f };

#define PLR_VEC_FREE(v)     {         s32 **a_ = &(v)->data;         if (*a_ != 0) {             s32 **b_ = &(v)->data;             s32 *c_ = *b_;             fn_1_6E0(c_);         }     }

#define PLR_CHECK_SET() \
    if (lbl_1_bss_A0C != 0) { \
    } else { \
        fn_1_7EC(lbl_1_data_5C0, lbl_1_data_5CC, 0x3A); \
    }

BOOL fn_1_FD14(s32 playerNo)
{
    return GwPlayerConf[playerNo].grp == 0;
}

void fn_1_FD38(s32 *count, Vec *a, Vec *b)
{
    if (fn_1_5634(a, b) != 0) {
        *count += 1;
    }
}

void fn_1_FD8C(Player568 *self)
{
    fn_1_1019C(self, -4000.0f, 16000.0f);
    fn_1_12448(self->tube, &self->pos);
}

void fn_1_FDEC(Player568 *self)
{
    M568Ptmf tmp;

    fn_1_1019C(self, -500.0f, 0.0f);
    fn_1_10F54(self, 5, 0);
    tmp = lbl_1_data_5A8;
    self->hook = tmp;
    self->f40 = self->first;
    fn_1_FFE4(self);
}

void fn_1_FE8C(Player568 *self)
{
    M568Ptmf tmp;

    fn_1_1019C(self, 0.0f, -500.0f);
    fn_1_10F54(self, 5, 0);
    tmp = lbl_1_data_5B4;
    self->hook = tmp;
    self->f40 = (self->first == 0);
    fn_1_FFE4(self);
}

void fn_1_FF34(Player568 *self)
{
    s32 m;

    if (self->f40 != 0) {
        m = 6;
    } else {
        m = 7;
    }
    fn_1_10F54(self, m, 1);
}

void fn_1_FF8C(Player568 *self)
{
    s32 m;

    if (self->f40 != 0) {
        m = 6;
    } else {
        m = 7;
    }
    fn_1_10F54(self, m, 1);
}

void fn_1_FFE4(Player568 *self)
{
    Vec p;
    PlrInput *in;

    if (self->f40 != 0) {
        fn_1_2B74(self->padNo);
    }
    if (self->input != 0) {
        in = self->input;
        if (in != 0) {
            ((void (*)(void *, s32))in->vtbl[2])(in, 1);
        }
        self->input = 0;
    }
    if (self->f40 == 0) {
        if (self->tube != 0) {
            fn_1_123DC(self->tube, 1);
            self->tube = 0;
        }
    } else {
        fn_1_1289C(self->tube);
        p = self->pos;
        p.z = p.z - 250.0f;
        fn_1_12448(self->tube, &p);
    }
}

void fn_1_100F8(Player568 *self)
{
    Mtx m;
    f32 z = self->pos.z;
    f32 y = self->pos.y;
    f32 x = self->pos.x;

    PSMTXIdentity(m);
    PSMTXTransApply(m, fn_1_36F0(self->model), x, y, z);
}

static void fn_1_1019C(Player568 *self, f32 a, f32 b)
{
    s32 i;
    s32 count;
    u8 hit;

    self->pos.y = 0.0f;
    if (self->first != 0) {
        self->pos.z = b;
    } else {
        self->pos.z = a;
    }
    if (self->first != 0) {
        if (fn_1_2548() == 3) {
            self->pos.x = -150.0f;
        } else {
            self->pos.x = 0.0f;
        }
        return;
    }
    i = 0;
    while (1) {
        if (i >= 4) {
            if (lbl_1_data_5D4 == 0) {
                return;
            }
            fn_1_7EC(lbl_1_data_5D9, lbl_1_data_5E1, 0x8C);
            return;
        }
        self->pos.x = lbl_1_rodata_414[i];
        hit = fn_1_D690(&self->pos);
        count = 0;
        PLR_CHECK_SET();
        fn_1_1198C((M568Link *)lbl_1_bss_A0C, &count);
        if (count == 0) {
            return;
        }
        i++;
    }
}

M568Tube *fn_1_10320(s32 padNo)
{
    M568Tube *t;
    s32 arg;

    t = (M568Tube *)fn_1_618(sizeof(M568Tube));
    if (t != 0) {
        if (fn_1_FD14(padNo) != 0) {
            arg = 0;
        } else {
            arg = 1;
        }
        t = fn_1_12058(t, arg, GwPlayerConf[padNo].charNo, padNo * 10);
    }
    return t;
}

PlrInput *fn_1_103D4(s32 padNo)
{
    PlrInput *p;

    if (GwPlayerConf[padNo].grp == 0) {
        if (GwPlayerConf[padNo].type != 0) {
            p = (PlrInput *)fn_1_618(PLR_ALLOC(0x20));
            if (p != 0) {
                p = fn_1_7658(p);
            }
            return p;
        }
        p = (PlrInput *)fn_1_618(PLR_ALLOC(0x18));
        if (p != 0) {
            p = fn_1_62F4(p, padNo);
        }
        return p;
    }
    if (GwPlayerConf[padNo].type != 0) {
        p = (PlrInput *)fn_1_618(sizeof(PlrInput));
        if (p != 0) {
            p = fn_1_6428(p);
        }
        return p;
    }
    p = (PlrInput *)fn_1_618(PLR_ALLOC(0x18));
    if (p != 0) {
        p = fn_1_62F4(p, padNo);
    }
    return p;
}

Player568 *fn_1_104FC(Player568 *self, s32 padNo)
{
    M568Model *model;
    M568Ptmf tmp;
    f32 skill;

    fn_1_187C(self);
    self->input = fn_1_103D4(padNo);
    self->f3C = 0;
    self->motions.cap = 0;
    self->motions.size = 0;
    self->motions.data = 0;
    model = (M568Model *)fn_1_618(sizeof(M568Model));
    if (model != 0) {
        model = fn_1_3330(model, GwPlayerConf[padNo].charNo, 2);
    }
    self->model = model;
    self->tube = fn_1_10320(padNo);
    self->first = (GwPlayerConf[padNo].grp == 0);
    PLR_CHECK_SET();
    fn_1_1948(self, lbl_1_bss_A0C);
    self->f38 = 150.0f;
    skill = (f32)(GwPlayerConf[padNo].dif + 1);
    self->input->fC = skill * 0.25f;
    self->padNo = padNo;
    self->charNo = GwPlayerConf[padNo].charNo;
    self->motion = -1;
    self->motions.size = 0;
    fn_1_11E04(&self->motions, 8);
    fn_1_10E14(self, DATANUM(DATA_mariomot, 0x53));
    fn_1_10E14(self, DATANUM(DATA_mariomot, 0x54));
    fn_1_10E14(self, DATANUM(DATA_mariomot, 0x55));
    fn_1_10E14(self, DATANUM(DATA_mariomot, 0x56));
    fn_1_10E14(self, DATANUM(DATA_mariomot, 0x3F));
    fn_1_10E14(self, DATANUM(DATA_mariomot, 0x00));
    fn_1_10E14(self, DATANUM(DATA_mariomot, 0x06));
    fn_1_10E14(self, DATANUM(DATA_mariomot, 0x07));
    if (self->motions.size == 8) {
    } else {
        fn_1_7EC(lbl_1_data_5F8, lbl_1_data_5E1, 0xC1);
    }
    CharMotionDataClose(self->charNo);
    Hu3DModelShadowSet(self->model->id);
    Hu3DModelShadowDispOn(self->model->id);
    Hu3DModelLayerSet(self->model->id, 0);
    Hu3DModelCameraSet(self->model->id, 1);
    fn_1_1019C(self, -4000.0f, 16000.0f);
    fn_1_12448(self->tube, &self->pos);
    tmp = lbl_1_data_5EC;
    self->hook = tmp;
    self->spin = 0.0f;
    return self;
}

M568Tube **fn_1_10B44(M568Tube **self, s16 flag)
{
    if (self != 0) {
        fn_1_123DC(*self, 1);
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

PlrVec *fn_1_10B9C(PlrVec *self, s16 flag)
{
    if (self != 0) {
        if (self != 0 && self != 0) {
            PLR_VEC_FREE(self);
        }
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

PlrVec *fn_1_10C30(PlrVec *self, s16 flag)
{
    if (self != 0) {
        if (self != 0) {
            PLR_VEC_FREE(self);
        }
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

PlrVec *fn_1_10CBC(PlrVec *self, s16 flag)
{
    if (self != 0) {
        PLR_VEC_FREE(self);
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

PlrInput **fn_1_10D40(PlrInput **self, s16 flag)
{
    PlrInput *in;

    if (self != 0) {
        in = *self;
        if (in != 0) {
            ((void (*)(void *, s32))in->vtbl[2])(in, 1);
        }
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

void *fn_1_10DBC(void *self, s16 flag)
{
    if (self != 0) {
        fn_1_1888(self, 0);
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

static void fn_1_10E14(Player568 *self, s32 dataNum)
{
    s32 id;

    s32 **pd;
    s32 *pos;

    id = CharMotionCreate(self->charNo, dataNum);
    pd = &self->motions.data;
    pos = *pd + self->motions.size;
    fn_1_11A4C(&self->motions, pos, 1, &id);
}

void fn_1_10E9C(Player568 *self)
{
    s32 padNo;
    M568Tube *tube;

    if (__ptmf_test(&self->hook) != 0) {
        __ptmf_scall(self);
    }
    padNo = self->padNo;
    if (GwPlayerConf[padNo].type == 0) {
        tube = self->tube;
        if (tube != 0 && fn_1_14208(tube) != 0) {
            omVibrate(padNo, 30, 6, 6);
        }
    }
}

static void fn_1_10F54(Player568 *self, s32 motion, u8 arg)
{
    s16 modelId;
    s16 mot;
    u32 attr;
    f32 end;

    modelId = self->model->id;
    mot = *fn_1_11914(&self->motions, motion);
    if (self->motion != motion) {
        if (self->motion == 3) {
            Hu3DModelAttrSet(modelId, 0x40000002);
        }
        if (motion != 3) {
            if (arg == 0) {
                attr = 0x40000001;
            } else {
                attr = 0;
            }
            if (motion == 5) {
                end = 0.0f;
            } else {
                end = 18.0f;
            }
            CharMotionShiftSet(self->charNo, mot, 0.0f, end, attr);
        } else {
            CharMotionShiftSet(self->charNo, mot, 9.0f, 18.0f, 0x40000002);
        }
        self->motion = motion;
    }
}

void fn_1_11098(Player568 *self)
{
    s16 modelId;
    s16 mot;
    M568Ptmf tmp;

    if (self->first != 0) {
        modelId = self->model->id;
        mot = *fn_1_11914(&self->motions, 0);
        if (self->motion != 0) {
            if (self->motion == 3) {
                Hu3DModelAttrSet(modelId, 0x40000002);
            }
            CharMotionShiftSet(self->charNo, mot, 0.0f, 18.0f, 0x40000001);
            self->motion = 0;
        }
    } else {
        modelId = self->model->id;
        mot = *fn_1_11914(&self->motions, 3);
        if (self->motion != 3) {
            if (self->motion == 3) {
                Hu3DModelAttrSet(modelId, 0x40000002);
            }
            CharMotionShiftSet(self->charNo, mot, 9.0f, 18.0f, 0x40000002);
            self->motion = 3;
        }
    }
    tmp = lbl_1_data_61C;
    self->hook = tmp;
}

void fn_1_11230(Player568 *self)
{
    fn_1_11268(self);
    fn_1_11380(self);
}

static void fn_1_11268(Player568 *self)
{
    Vec v;
    PlrInput *in;

    if (fn_1_2548() != 2) {
        fn_1_11630(self, 0.0f);
        return;
    }
    if (self->input == 0 || self->tube == 0) {
        return;
    }
    in = self->input;
    fn_1_628C(in, self->tube);
    if (in->f4 != 0) {
        fn_1_1268C(self->tube);
        fn_1_11784(self);
        return;
    }
    v.x = lbl_1_rodata_450[0];
    v.y = lbl_1_rodata_450[1];
    v.z = lbl_1_rodata_450[2];
    v.x = in->f8;
    if (in->f5 != 0) {
        v.z = -1.0f;
        fn_1_1184C(self);
    } else {
        fn_1_11630(self, in->f8);
    }
    fn_1_12650(self->tube, &v);
}

static void fn_1_11380(Player568 *self)
{
    Mtx m;
    Mtx r;
    Quaternion q;
    M568Tube *tube;
    s16 modelId;
    s16 mot;

    PSMTXRotRad(m, 'Y', 3.1415927f);
    if (self->tube->down != 0) {
        self->spin = (f32)((f64)self->spin + 6.0);
        if (self->spin > 360.0f) {
            self->spin = (f32)(self->spin - 360.0f);
        }
        PSMTXTransApply(m, m, 0.0f, -50.0f, 0.0f);
        PSMTXRotRad(r, 'X', 0.017453292f * self->spin);
        PSMTXConcat(r, m, m);
        PSMTXTransApply(m, m, 0.0f, 50.0f, 0.0f);
    }
    fn_1_1262C(self->tube, &q);
    PSMTXQuat(r, &q);
    PSMTXConcat(r, m, m);
    tube = self->tube;
    PSMTXTransApply(m, fn_1_36F0(self->model), tube->pos.x, tube->pos.y, tube->pos.z);
    if (self->tube->down != 0) {
        modelId = self->model->id;
        mot = *fn_1_11914(&self->motions, 4);
        if (self->motion != 4) {
            if (self->motion == 3) {
                Hu3DModelAttrSet(modelId, 0x40000002);
            }
            CharMotionShiftSet(self->charNo, mot, 0.0f, 18.0f, 0x40000001);
            self->motion = 4;
        }
    }
}

static void fn_1_11630(Player568 *self, f32 t)
{
    s16 modelId;
    s16 mot;

    if (self->first == 0) {
        modelId = self->model->id;
        mot = *fn_1_11914(&self->motions, 3);
        if (self->motion != 3) {
            if (self->motion == 3) {
                Hu3DModelAttrSet(modelId, 0x40000002);
            }
            CharMotionShiftSet(self->charNo, mot, 9.0f, 18.0f, 0x40000002);
            self->motion = 3;
        }
        if (self->motion == 3) {
            fn_1_3420(self->model, 0x40000002);
            modelId = self->model->id;
            Hu3DMotionTimeSet(modelId, 9.0f + (-9.0f * t));
            Hu3DModelAttrSet(modelId, 0x40000002);
        }
    }
}

static void fn_1_11784(Player568 *self)
{
    s16 modelId;
    s16 mot;

    if (self->first == 0) {
        modelId = self->model->id;
        mot = *fn_1_11914(&self->motions, 1);
        if (self->motion != 1) {
            if (self->motion == 3) {
                Hu3DModelAttrSet(modelId, 0x40000002);
            }
            CharMotionShiftSet(self->charNo, mot, 0.0f, 18.0f, 0);
            self->motion = 1;
        }
    }
}

static void fn_1_1184C(Player568 *self)
{
    s16 modelId;
    s16 mot;

    if (self->first == 0) {
        modelId = self->model->id;
        mot = *fn_1_11914(&self->motions, 2);
        if (self->motion != 2) {
            if (self->motion == 3) {
                Hu3DModelAttrSet(modelId, 0x40000002);
            }
            CharMotionShiftSet(self->charNo, mot, 0.0f, 18.0f, 0);
            self->motion = 2;
        }
    }
}

static s32 *fn_1_11914(PlrVec *v, u32 idx)
{
    if (idx >= v->size) {
        fn_1_84C(PLR_STDERR, lbl_1_data_628);
        abort();
    }
    return v->data + idx;
}

// Counts the pairs of players that overlap; the list is circular and the
// first word of each node is the forward link.
static void fn_1_1198C(M568Link *list, s32 *count)
{
    M568Link *a;
    M568Link *b;
    M568Link *last;

    a = list->prev;
    last = list->next;
    while (a != last) {
        b = a->prev;
        while (b != list) {
            fn_1_FD38(count, &((Player568 *)a)->pos, &((Player568 *)b)->pos);
            b = b->prev;
        }
        a = a->prev;
    }
}

// vector<s32>::insert(pos, n, value)
static void fn_1_11A4C(PlrVec *v, s32 *pos, u32 n, s32 *val)
{
    const u32 max = 0x3FFFFFFF;
    s32 *end;
    s32 *s;
    s32 *d;
    s32 *p;
    s32 *old;
    s32 *src;
    s32 *dst;
    s32 move;
    s32 idx;
    u32 newSize;
    u32 cap;
    u32 cnt;

    if (n != 0) {
        if (n > max || v->size > max - n) {
            fn_1_84C(PLR_STDERR, lbl_1_data_646);
            abort();
        }
        if (v->size + n <= v->cap) {
            end = v->data + v->size;
            move = (s32)(end - pos);
            if (end - (move - (s32)n) <= val && val < end) {
                val = val + n;
            }
            if ((u32)move != 0) {
                s = end;
                d = end + n;
                while (s > pos) {
                    s--;
                    d--;
                    *d = *s;
                }
            }
            cnt = n;
            dst = pos;
            while (cnt != 0) {
                *dst = *val;
                dst++;
                cnt--;
            }
            v->size = v->size + n;
            return;
        }
        idx = (s32)(pos - v->data);
        old = v->data;
        newSize = v->size + n;
        if (v->cap != 0) {
            cap = v->cap;
        } else {
            cap = 1;
        }
        while (newSize > cap) {
            if (cap < (max >> 1)) {
                cap *= 2;
            } else {
                cap = max;
            }
        }
        p = (s32 *)fn_1_618(cap * sizeof(s32));
        if (p == 0) {
            fn_1_84C(PLR_STDERR, lbl_1_data_663);
            abort();
        }
        v->data = p;
        cnt = n;
        dst = v->data + idx;
        while (cnt != 0) {
            *dst = *val;
            dst++;
            cnt--;
        }
        if (old != 0) {
            dst = v->data;
            src = old;
            while (src < old + idx) {
                *dst = *src;
                src++;
                dst++;
            }
            dst = v->data + idx + n;
            src = old + idx;
            while (src < old + v->size) {
                *dst = *src;
                src++;
                dst++;
            }
            fn_1_6E0(old);
        }
        v->size = newSize;
        v->cap = cap;
    }
}

// vector<s32>::reserve(n)
static void fn_1_11E04(PlrVec *v, u32 n)
{
    const u32 max = 0x3FFFFFFF;
    s32 *old;
    s32 *p;
    s32 *src;
    s32 *dst;

    if (n > v->cap) {
        if (n > max) {
            fn_1_84C(PLR_STDERR, lbl_1_data_67D);
            abort();
        }
        old = v->data;
        p = (s32 *)fn_1_618(n * sizeof(s32));
        if (p == 0) {
            fn_1_84C(PLR_STDERR, lbl_1_data_663);
            abort();
        }
        v->data = p;
        if (old != 0) {
            dst = v->data;
            src = old;
            while (src < old + v->size) {
                *dst = *src;
                src++;
                dst++;
            }
            fn_1_6E0(old);
        }
        v->cap = n;
    }
}

void fn_1_11F38(void)
{
    lbl_1_bss_A80 = 0x78;
    lbl_1_bss_A84 = 0xD2;
    lbl_1_bss_A88 = 0xD2;
}

}
