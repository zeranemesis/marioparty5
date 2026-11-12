// m513Dll heiho: the shy-guy "heiho" characters that hold a torch-like item.
// Each one is a node of an intrusive set, owns a model, a motion table
// (std::vector<s16>) and a PTMF state hook.
#define _MATH_H
#include "REL/m513Dll.h"
#include "game/audio.h"

extern "C" {
void abort(void);
void *memmove(void *dst, const void *src, u32 n);
void *memcpy(void *dst, const void *src, u32 n);
int fn_1_234(void *unused, const char *fmt, ...);
}

#ifdef __MWERKS__
#define M513_RODATA __declspec(section ".rodata")
#else
#define M513_RODATA
#endif

typedef struct M513ModelPtr_s {
    M513Model *p;
} M513ModelPtr;

typedef struct M513Heiho_s {
    M513Node link;
    M513Model model;
    s32 motion;
    M513Vec16 mot;
    Vec pos;
    f32 unk28;
    Vec rot;
    s32 idx;
    s32 state;
    f32 t;
    M513Model item;
    f32 t2;
    s32 cnt;
    M513Ptmf hook;
} M513Heiho;

extern "C" {

extern M513Node *lbl_1_bss_9F8;
extern M513ModelPtr *lbl_1_bss_9FC;
extern M513Node *lbl_1_bss_A14;

void fn_1_8394(M513Node *leaf);
void fn_1_145A8(s32 seId, Vec *pos);

void fn_1_E13C(M513Heiho *self);
void fn_1_E138(void);
void fn_1_DF5C(M513Heiho *self);
void fn_1_E650(M513Heiho *self, s32 dataNum);
void fn_1_E6D8(M513Heiho *self, s32 motion, u8 loop, f32 speed);
s16 *fn_1_E890(M513Vec16 *v, u32 idx);
void fn_1_E908(M513Vec16 *v, s16 *pos, u32 n, s16 *val);
void fn_1_ECB8(M513Vec16 *v, u32 cap);

char lbl_1_data_488[] = "ms_pointer == 0";
char lbl_1_data_498[] = "singleton.h";
char lbl_1_data_4A4[] = "ms_pointer != 0";
char lbl_1_data_4B4[] = "p01";
char lbl_1_data_4B8[] = "p02";
char lbl_1_data_4BC[] = "p03";
char lbl_1_data_4C0[] = "p04";
char *lbl_1_data_4C4[4] = { lbl_1_data_4B4, lbl_1_data_4B8, lbl_1_data_4BC, lbl_1_data_4C0 };
M513Ptmf lbl_1_data_4D4 = { 0, -1, (void (*)(void *))fn_1_E13C };
char lbl_1_data_4E0[] = "sm_ptr != 0";
char lbl_1_data_4EC[] = "a_set.h";
char lbl_1_data_4F4[] = "m_mot_id.size() == NUMBEROF_MOTION";
char lbl_1_data_517[] = "heiho.cpp";
char lbl_1_data_521[] = "itemhook_L";
f32 lbl_1_data_52C[4] = { 4.0f, 3.5f, 3.0f, 2.5f };
M513Ptmf lbl_1_data_53C = { 0, -1, (void (*)(void *))fn_1_E138 };
M513Ptmf lbl_1_data_548 = { 0, -1, (void (*)(void *))fn_1_E138 };
char lbl_1_data_554[] = "vector::at index out of range";
char lbl_1_data_572[] = "vector::insert length error\n";
char lbl_1_data_58F[] = "Memory allocation failure";
char lbl_1_data_5A9[] = "vector::reserve length error\n";

void fn_1_1220C(void);
void fn_1_12208(void);
void fn_1_F698(void *self);
void fn_1_F3EC(void *self);

typedef struct M513PtmfPair_s {
    M513Ptmf a;
    M513Ptmf b;
} M513PtmfPair;

M513PtmfPair lbl_1_data_5C8 = { { 0, -1, (void (*)(void *))fn_1_1220C }, { 0, -1, (void (*)(void *))fn_1_12208 } };
M513Ptmf lbl_1_data_5E0 = { 0, -1, (void (*)(void *))fn_1_F698 };
M513Ptmf lbl_1_data_5EC = { 0, -1, (void (*)(void *))fn_1_F3EC };

M513_RODATA const s32 lbl_1_rodata_374[5] = { 0x6D1, 0x6D2, 0x6D3, 0x6D4, 0 };

M513ModelPtr *fn_1_D738(M513ModelPtr *self)
{
    M513Model *model;

    M513_ASSERT(lbl_1_bss_9FC == 0, lbl_1_data_488, lbl_1_data_498, 0x35);
    lbl_1_bss_9FC = self;
    self->p = 0;
    model = (M513Model *)fn_1_0(sizeof(M513Model));
    if (model != 0) {
        model = fn_1_3198(model, 0x47000D);
    }
    if (self->p != model) {
        fn_1_35F8(self->p, 1);
        self->p = model;
    }
    return self;
}

M513ModelPtr *fn_1_D800(M513ModelPtr *self, s16 flag)
{
    if (self != 0) {
        M513_ASSERT(lbl_1_bss_9FC != 0, lbl_1_data_4A4, lbl_1_data_498, 0x3D);
        lbl_1_bss_9FC = 0;
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M513ModelPtr *fn_1_D88C(M513ModelPtr *self, s16 flag)
{
    if (self != 0) {
        if (self != 0) {
            fn_1_35F8(self->p, 1);
        }
        if (self != 0) {
            M513_ASSERT(lbl_1_bss_9FC != 0, lbl_1_data_4A4, lbl_1_data_498, 0x3D);
            lbl_1_bss_9FC = 0;
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// Position / rotation of a hook object of the shared model; true while the motion plays.
s32 fn_1_D934(M513ModelPtr *self, s32 idx, Vec *pos, Vec *rot)
{
    HU3DMODELID id = self->p->id;
    HSFOBJECT *obj = Hu3DModelObjPtrGet(id, lbl_1_data_4C4[idx]);

    if (obj != 0) {
        pos->x = obj->mesh.curr.pos.x;
        pos->y = obj->mesh.curr.pos.y;
        pos->z = obj->mesh.curr.pos.z;
        rot->x = obj->mesh.curr.rot.x;
        rot->y = obj->mesh.curr.rot.y;
        rot->z = obj->mesh.curr.rot.z;
        rot->y = rot->y + 90.0f;
    }
    return Hu3DMotionEndCheck(id) == 0;
}

M513Heiho *fn_1_DA0C(M513Heiho *self, s32 idx)
{
    HU3DMODELID id;

    fn_1_1768(&self->link);
    fn_1_3198(&self->model, 0x470008);
    self->motion = -1;
    self->mot.cap = 0;
    self->mot.size = 0;
    self->mot.data = 0;
    self->idx = idx;
    self->state = 0;
    self->t = 0.0f;
    self->t2 = 0.0f;
    self->cnt = 0;
    M513_ASSERT(lbl_1_bss_A14 != 0, lbl_1_data_4E0, lbl_1_data_4EC, 0x2E);
    fn_1_1834(&self->link, lbl_1_bss_A14);
    self->unk28 = 60.000004f;
    id = self->model.id;
    Hu3DModelCameraSet(id, 1);
    Hu3DModelLayerSet(id, 3);
    Hu3DModelShadowSet(id);
    Hu3DModelShadowDispOn(id);
    self->mot.size = 0;
    fn_1_ECB8(&self->mot, 4);
    fn_1_E650(self, 0x470009);
    fn_1_E650(self, 0x47000B);
    fn_1_E650(self, 0x47000C);
    fn_1_E650(self, 0x47000A);
    M513_ASSERT(self->mot.size == 4, lbl_1_data_4F4, lbl_1_data_517, 0x58);
    self->pos.x = 0.0f;
    self->pos.y = 0.0f;
    self->pos.z = 0.0f;
    self->rot.x = 0.0f;
    self->rot.y = 0.0f;
    self->rot.z = 0.0f;
    fn_1_33F8(&self->model, &self->pos);
    fn_1_DF5C(self);
    fn_1_E6D8(self, 0, 0, 8.0f);
    self->hook = lbl_1_data_4D4;
    return self;
}

M513Vec16 *fn_1_DC98(M513Vec16 *v, s16 flag)
{
    if (v != 0) {
        if ((v != 0) && (v != 0) && (*(s16 **)&v->data != 0)) {
            s16 **q = &v->data;
            s16 *d = *q;
            fn_1_C8(d);
        }
        if (flag > 0) {
            fn_1_C8(v);
        }
    }
    return v;
}

M513Vec16 *fn_1_DD2C(M513Vec16 *v, s16 flag)
{
    if (v != 0) {
        if ((v != 0) && (*(s16 **)&v->data != 0)) {
            s16 **q = &v->data;
            s16 *d = *q;
            fn_1_C8(d);
        }
        if (flag > 0) {
            fn_1_C8(v);
        }
    }
    return v;
}

M513Vec16 *fn_1_DDB8(M513Vec16 *v, s16 flag)
{
    if (v != 0) {
        if (*(s16 **)&v->data != 0) {
            s16 **q = &v->data;
            s16 *d = *q;
            fn_1_C8(d);
        }
        if (flag > 0) {
            fn_1_C8(v);
        }
    }
    return v;
}

M513Heiho *fn_1_DE3C(M513Heiho *self, s16 flag)
{
    if (self != 0) {
        fn_1_1774(&self->link, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M513Heiho *fn_1_DE94(M513Heiho *self, s16 flag)
{
    if (self != 0) {
        if ((&self->mot != 0) && (&self->mot != 0) && (&self->mot != 0) && (*(s16 **)&self->mot.data != 0)) {
            s16 **q = &self->mot.data;
            s16 *d = *q;
            fn_1_C8(d);
        }
        fn_1_35F8(&self->model, -1);
        if (self != 0) {
            fn_1_1774(&self->link, 0);
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// Attach the held item model to the left hand.
void fn_1_DF5C(M513Heiho *self)
{
    HU3DMODELID id = self->model.id;
    HU3DMODELID item = Hu3DModelCreate(HuDataSelHeapReadNum(0x47000E, 0x10000000, HEAP_MODEL));

    Hu3DModelHookSet(id, lbl_1_data_521, item);
    Hu3DModelRotSet(item, 0.0f, 270.0f, 0.0f);
    Hu3DModelLayerSet(item, 3);
    Hu3DModelCameraSet(item, 1);
    Hu3DModelShadowSet(item);
    Hu3DModelShadowDispOn(item);
    self->item.id = item;
}

void fn_1_E030(M513Heiho *self)
{
    HU3DMODELID a = self->model.id;
    HU3DMODELID b = a;

    Hu3DModelHookReset(b);
    Hu3DModelKill(self->item.id);
}

// Remove this heiho's leaf from the set.
void fn_1_E084(M513Heiho *self)
{
    M513Node *it;

    M513_ASSERT(lbl_1_bss_9F8 != 0, lbl_1_data_4E0, lbl_1_data_4EC, 0x2E);
    it = lbl_1_bss_9F8->next;
    while (it != lbl_1_bss_9F8) {
        if (((M513Heiho *)it)->idx != self->idx) {
            it = it->next;
            continue;
        }
        break;
    }
    fn_1_8394(it);
}

void fn_1_E138(void)
{
}

// State hook: throw the item, then the heiho sinks away.
void fn_1_E13C(M513Heiho *self)
{
    switch (self->state) {
        case 0: {
            M513ModelPtr *mp;
            M513ModelPtr *mp2;
            M513Model *mdl;
            HU3DMODELID id0;
            HU3DMODELID id;
            HSFOBJECT *obj;

            M513_ASSERT(lbl_1_bss_9FC != 0, lbl_1_data_4A4, lbl_1_data_498, 0x27);
            mp = lbl_1_bss_9FC;
            mp2 = lbl_1_bss_9FC;
            fn_1_E6D8(self, 1, 0, 8.0f);
            mdl = mp2->p;
            id0 = mdl->id;
            id = id0;
            obj = Hu3DModelObjPtrGet(id, lbl_1_data_4C4[self->idx]);
            if (obj != 0) {
                self->pos.x = obj->mesh.curr.pos.x;
                self->pos.y = obj->mesh.curr.pos.y;
                self->pos.z = obj->mesh.curr.pos.z;
                self->rot.x = obj->mesh.curr.rot.x;
                self->rot.y = obj->mesh.curr.rot.y;
                self->rot.z = obj->mesh.curr.rot.z;
                self->rot.y = self->rot.y + 90.0f;
            }
            Hu3DMotionEndCheck(id);
            self->t = self->t + 0.016666666666666666;
            self->t2 = self->t2 + 0.016666666666666666;
            if (self->t2 > 0.25f) {
                Vec p;

                p.x = self->pos.x;
                p.y = self->pos.y;
                p.z = self->pos.z;
                fn_1_145A8(0x2C2, &p);
                self->t2 = 0.0f;
            }
            if (self->t < lbl_1_data_52C[self->idx]) {
                break;
            }
            self->t = 0.0f;
            self->state++;
        }
        // fall through
        case 1:
            self->rot.y = self->rot.y + (-self->rot.y / 12.0f);
            self->pos.z = self->pos.z + ((-340.0f - self->pos.z) / 30.0f);
            self->t = self->t + 0.016666666666666666;
            if (self->t < 1.0f) {
                break;
            }
            fn_1_E6D8(self, 2, 1, 8.0f);
            self->state++;
            // fall through
        case 2:
            if (self->cnt != 0xFF) {
                self->cnt++;
                if (self->cnt == 0xF) {
                    M513Node *it;
                    M513Node *set;
                    M513Node *set2;
                    M513Node *first;
                    M513Node *first2;
                    M513Node *first3;
                    M513Node *nx;
                    M513Node *nx2;
                    Vec p;
                    HU3DMODELID hid = self->model.id;

                    Hu3DModelHookReset(hid);
                    Hu3DModelKill(self->item.id);
                    M513_ASSERT(lbl_1_bss_9F8 != 0, lbl_1_data_4E0, lbl_1_data_4EC, 0x2E);
                    set = lbl_1_bss_9F8;
                    set2 = lbl_1_bss_9F8;
                    first = set2->next;
                    first2 = first;
                    first3 = first2;
                    it = first3;
                    while (it != set2) {
                        if (((M513Heiho *)it)->idx != self->idx) {
                            nx = it->next;
                            nx2 = nx;
                            it = nx2;
                            continue;
                        }
                        break;
                    }
                    (void)set;
                    fn_1_8394(it);
                    self->cnt = 0xFF;
                    p.x = self->pos.x;
                    p.y = self->pos.y;
                    p.z = self->pos.z;
                    fn_1_145A8(lbl_1_rodata_374[self->idx], &p);
                }
            }
            if (Hu3DMotionEndCheck(self->model.id) == 0) {
                break;
            }
            self->state++;
            // fall through
        case 3:
            fn_1_E6D8(self, 0, 0, 8.0f);
            self->hook = lbl_1_data_53C;
            break;
    }
}

void fn_1_E5F0(M513Heiho *self)
{
    if (M513_PTMF_TEST(&self->hook)) {
        M513_PTMF_CALL(&self->hook, self);
    }
    fn_1_33F8(&self->model, &self->pos);
    fn_1_3440(&self->model, &self->rot);
}

void fn_1_E650(M513Heiho *self, s32 dataNum)
{
    HU3DMODELID id = self->model.id;
    s16 mot = Hu3DJointMotion(id, HuDataSelHeapReadNum(dataNum, 0x10000000, HEAP_MODEL));

    fn_1_E908(&self->mot, self->mot.data + self->mot.size, 1, &mot);
}

// Switch to motion `motion` (looping unless `loop`).
void fn_1_E6D8(M513Heiho *self, s32 motion, u8 loop, f32 speed)
{
    HU3DMODELID id = self->model.id;
    s16 mot = *fn_1_E890(&self->mot, motion);

    if (self->motion != motion) {
        u32 attr;

        Hu3DModelAttrReset(id, 0x40000002);
        if (loop == 0) {
            attr = 0x40000001;
        } else {
            attr = 0;
        }
        Hu3DMotionShiftSet(id, mot, 0.0f, speed, attr);
        self->motion = motion;
    }
}

void fn_1_E790(M513Heiho *self)
{
    Vec p;
    HU3DMODELID id;
    s16 mot;

    p.x = self->pos.x;
    p.y = self->pos.y;
    p.z = self->pos.z;
    fn_1_145A8(0x2C5, &p);
    id = self->model.id;
    mot = *fn_1_E890(&self->mot, 3);
    if (self->motion != 3) {
        Hu3DModelAttrReset(id, 0x40000002);
        Hu3DMotionShiftSet(id, mot, 0.0f, 8.0f, 0x40000001);
        self->motion = 3;
    }
    self->hook = lbl_1_data_548;
}

s16 *fn_1_E890(M513Vec16 *v, u32 idx)
{
    if (idx >= v->size) {
        fn_1_234(M513_STDERR, lbl_1_data_554);
        abort();
    }
    {
        s16 **dp = &v->data;
        return *dp + idx;
    }
}

#pragma dont_inline on
// vector<s16>::insert(pos, n, value)
void fn_1_E908(M513Vec16 *v, s16 *pos, u32 n, s16 *val)
{
    s16 *srcP = val;
    if (n != 0) {
        M513Vec16 *t54 = v;
        volatile u32 max0 = 0x7FFFFFFF;
        volatile u32 max1 = max0;
        volatile u32 max = max1;
        M513Vec16 *t44;
        u32 capNow;
        volatile u32 capCopy;
        if (n > max || v->size > max - n) {
            fn_1_234(M513_STDERR, lbl_1_data_572);
            abort();
        }
        t44 = v;
        capNow = t44->cap;
        capCopy = capNow;
        if (v->size + n <= capCopy) {
            s16 **itp = &v->data;
            s16 *end = *itp + v->size;
            volatile s16 *endCopy = end;
            s32 after = end - pos;
            s16 *src = srcP;
            u32 k;
            s16 *p;
            if (end - (after - n) <= src && src < end) {
                src += n;
            }
            if (after != 0) {
                s32 cnt = end - pos;
                volatile s32 cnt2 = cnt;
                memmove(end + n - cnt, pos, cnt * 2);
            }
            k = n;
            p = pos;
            while (k != 0) {
                *p = *src;
                p++;
                k--;
            }
            v->size = v->size + n;
            (void)endCopy;
        } else {
            s16 **itp = &v->data;
            s16 *old = *itp;
            volatile s16 *old2 = old;
            s32 idx = pos - old;
            s16 **itp2 = &v->data;
            u32 newSize = v->size + n;
            M513Vec16 *t28 = v;
            u32 cap;
            s16 *mem;
            u32 k;
            s16 *p;
            if (t28->cap != 0) {
                M513Vec16 *t24 = v;
                volatile u32 c20 = t24->cap;
                cap = c20;
            } else {
                volatile u32 c20 = 1;
                cap = c20;
            }
            while (newSize > cap) {
                if (cap < (max >> 1)) {
                    cap *= 2;
                } else {
                    cap = max;
                }
            }
            {
                s16 **itp3 = &v->data;
                M513Vec16 *t1c = v;
                M513Vec16 *t78 = t1c;
                mem = (s16 *)fn_1_0(cap * 2);
                (void)itp3;
                (void)t78;
            }
            if (mem == 0) {
                fn_1_234(M513_STDERR, lbl_1_data_58F);
                abort();
            }
            {
                volatile s16 *m74 = mem;
                *itp2 = mem;
                (void)m74;
            }
            k = n;
            {
                s16 **itp4 = &v->data;
                p = *itp4 + idx;
            }
            while (k != 0) {
                *p = *srcP;
                p++;
                k--;
            }
            if (old != 0) {
                s16 **itp5 = &v->data;
                s16 *nw = *itp5;
                volatile s16 *nw2 = nw;
                s32 cnt = (old + idx) - old;
                volatile s32 cnt2 = cnt;
                s16 **itp6;
                s16 *dst;
                memcpy(nw, old, cnt * 2);
                itp6 = &v->data;
                dst = *itp6 + (idx + n);
                {
                    s16 *e = old + v->size;
                    s32 cnt3 = e - (old + idx);
                    volatile s32 cnt4 = cnt3;
                    memcpy(dst, old + idx, cnt3 * 2);
                    (void)cnt4;
                }
                fn_1_C8(old);
                (void)nw2;
                (void)cnt2;
            }
            v->size = newSize;
            {
                M513Vec16 *t0c = v;
                t0c->cap = cap;
            }
            (void)old2;
        }
        (void)t54;
    }
}

// vector<s16>::reserve(cap)
void fn_1_ECB8(M513Vec16 *v, u32 cap)
{
    u32 *cp = &v->cap;
    if (cap > *cp) {
        M513Vec16 *self2 = v;
        volatile u32 max0 = 0x7FFFFFFF;
        volatile u32 max = max0;
        s16 **ip = &v->data;
        s16 *old;
        s16 **ip2;
        volatile M513Vec16 *sv;
        volatile M513Vec16 *sv2;
        s16 *mem;
        if (cap > max) {
            fn_1_234(M513_STDERR, lbl_1_data_5A9);
            abort();
        }
        old = *ip;
        ip2 = &v->data;
        sv = v;
        sv2 = sv;
        mem = (s16 *)fn_1_0(cap * 2);
        if (mem == 0) {
            fn_1_234(M513_STDERR, lbl_1_data_58F);
            abort();
        }
        *ip2 = mem;
        if (old != 0) {
            s16 **ip3 = &v->data;
            s16 *nw = *ip3;
            s16 *e = old + v->size;
            s32 cnt = e - old;
            memcpy(nw, old, cnt * 2);
            fn_1_C8(old);
        }
        {
            M513Vec16 *s3 = v;
            s3->cap = cap;
        }
        (void)self2;
        (void)sv2;
    }
}

#pragma dont_inline reset
}
