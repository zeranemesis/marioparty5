// m512Dll/REL/m512Dll/heiho
// CHeiho: a shy-guy-style item carrier with two models (body + held item),
// a two-entry motion table (std::vector<s16>) and a PTMF state hook.
#define _MATH_H
#include "types.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/audio.h"

#ifdef __MWERKS__
#define HEIHO_RODATA __declspec(section ".rodata")
#else
#define HEIHO_RODATA
#endif

struct HeihoNode {
    HeihoNode *next; // 0x00
    HeihoNode *prev; // 0x04
};

// GC pointer-to-member-function layout (this-adjust, vtable index, function)
struct HeihoPtmf {
    s32 adj;
    s32 idx;
    void (*fn)(void *);
};

// std::vector<s16> as built by this module: 0x24..0x30
struct HeihoVec {
    u32 cap;     // 0x24
    u32 size;    // 0x28
    s16 *data;   // 0x2C
};

struct HeihoModelId {
    s16 id;
};

struct Heiho {
    HeihoNode node;       // 0x00
    void **vtbl;          // 0x08
    s32 unkC;             // 0x0C
    HeihoPtmf hook;       // 0x10
    HeihoModelId body;    // 0x1C
    HeihoModelId item;    // 0x1E
    s32 motion;           // 0x20
    HeihoVec motIds;      // 0x24
    u8 sndPlayed;         // 0x30
    s32 timer;            // 0x34
};

extern "C" {
void *__dl__FPv(void *);
void __register_global_object(void *obj, void *dtor, void *reg);
s32 __ptmf_test(void *ptmf);
void __ptmf_scall(void *obj);
void *fn_1_1264(void *self);
void fn_1_1270(void *self, int flag);
void fn_1_1330(void *self, void *arg);
void fn_1_1D4(const char *msg, const char *file, int line);
void fn_1_2C94(void *self, s32 dataNum);
void fn_1_2EF4(void *self, void *vec);
void fn_1_2F3C(void *self, void *vec);
void fn_1_30F4(void *self, s32 id);
s16 *fn_1_CA88(void *vec, s32 idx);
void fn_1_CBC0(void *vec, s16 *pos, s32 count, s16 *val);
void fn_1_CF70(void *vec, s32 count);
void fn_1_9818(void);

extern HeihoNode lbl_1_bss_C;
extern void *lbl_1_data_1FC[4];
extern Heiho *lbl_1_bss_A04;
}

extern "C" void fn_1_10A88(Heiho *self);
extern "C" void fn_1_10AD0(Heiho *self);
extern "C" void fn_1_10B6C(Heiho *self);
extern "C" void fn_1_10CA0(Heiho *self, s32 bit);
extern "C" void *fn_1_10D58(Heiho *self, s16 flag);
extern "C" void fn_1_10E98(void);
extern "C" s16 fn_1_109F0(Heiho *self, s32 dataNum);
extern "C" void fn_1_10B9C(Heiho *self, s32 motion, u8 loop);

// -------------------------------------------------------------------- data

extern "C" {
HeihoPtmf lbl_1_data_608 = { 0, -1, (void (*)(void *))fn_1_10B6C };
char lbl_1_data_614[] = "ms_pointer == 0";
char lbl_1_data_624[] = "singleton.h";
char lbl_1_data_630[] = "m_mot_id.size() == NUMBEROF_MOTION";
char lbl_1_data_653[] = "zako2.cpp";
char lbl_1_data_65D[] = "itemhook_R";
char lbl_1_data_668[] = "ms_pointer != 0";
HeihoPtmf lbl_1_data_678 = { 0, -1, (void (*)(void *))fn_1_10AD0 };
HeihoPtmf lbl_1_data_684 = { 0, -1, (void (*)(void *))fn_1_10B6C };
void *lbl_1_data_690[5] = { 0, 0, (void *)fn_1_10D58, (void *)fn_1_10CA0, (void *)fn_1_10A88 };
// vector<s16> error strings (used by the vector code in player.cpp)
char lbl_1_data_6A4[] = "vector::at index out of range\0vector::insert length error\n\0Memory allocation failure\0vector::reserve length error\n";

HEIHO_RODATA const f32 lbl_1_rodata_558[3] = { -104.0f, 200.0f, 550.0f };
HEIHO_RODATA const f32 lbl_1_rodata_564[3] = { 0.0f, 0.0f, 0.0f };
}

extern "C" {
f64 lbl_1_bss_AA8;
char lbl_1_bss_AB0[8];
}
s32 lbl_1_bss_AB8;
s32 lbl_1_bss_ABC;
s32 lbl_1_bss_AC0;
char lbl_1_bss_AC4[12];
struct HeihoSlot {
    s32 a, b, c, d;
} lbl_1_bss_AD0;

// ------------------------------------------------------------------ functions

extern "C" Heiho *fn_1_10750(Heiho *self)
{
    s16 body;
    HeihoVec *vec;

    fn_1_1264(self);
    self->vtbl = lbl_1_data_1FC;
    self->unkC = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    if (lbl_1_bss_A04 == 0) {
        } else {
        fn_1_1D4(lbl_1_data_614, lbl_1_data_624, 0x35);
    }
    {
        Heiho *s = (Heiho *)((char *)self + 0x10);
        if (s != 0) {
            s = (Heiho *)((char *)s - 0x10);
        }
        lbl_1_bss_A04 = s;
    }
    self->vtbl = lbl_1_data_690;
    self->hook = lbl_1_data_608;
    fn_1_2C94(&self->body, 0x46000A);
    fn_1_2C94(&self->item, 0x46000B);
    vec = &self->motIds;
    vec->cap = 0;
    vec->size = 0;
    vec->data = 0;
    self->sndPlayed = 0;
    self->timer = 0;
    body = self->body.id;
    self->motIds.size = 0;
    fn_1_CF70(&self->motIds, 2);
    fn_1_109F0(self, 0x460011);
    fn_1_109F0(self, 0x460010);
    if (self->motIds.size != 2) {
        fn_1_1D4(lbl_1_data_630, lbl_1_data_653, 0x3A);
    }
    fn_1_2EF4(&self->body, (void *)lbl_1_rodata_558);
    fn_1_2F3C(&self->body, (void *)lbl_1_rodata_564);
    Hu3DModelHookSet(self->body.id, lbl_1_data_65D, self->item.id);
    Hu3DMotionSet(body, Hu3DMotionIDGet(body));
    Hu3DModelAttrSet(body, 0x40000002);
    Hu3DModelAttrReset(body, 0x40000001);
    return self;
}

extern "C" Heiho *fn_1_10964(Heiho *self, s16 flag)
{
    if (self != 0) {
        if (lbl_1_bss_A04 != 0) {
            } else {
            fn_1_1D4(lbl_1_data_668, lbl_1_data_624, 0x3D);
        }
        lbl_1_bss_A04 = 0;
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

extern "C" s16 fn_1_109F0(Heiho *self, s32 dataNum)
{
    s16 mot;
    s16 body;
    s16 **dp;
    s16 *end;

    body = self->body.id;
    mot = Hu3DJointMotion(body, HuDataSelHeapReadNum(dataNum, 0x10000000, HEAP_MODEL));
    dp = &self->motIds.data;
    end = *dp + self->motIds.size;
    fn_1_CBC0(&self->motIds, end, 1, &mot);
    return mot;
}

extern "C" void fn_1_10A88(Heiho *self)
{
#ifdef TARGET_PC
    if (self->hook.fn != 0) {
        self->hook.fn(self);
    }
#else
    if (__ptmf_test(&self->hook) != 0) {
        __ptmf_scall(self);
    }
#endif
}

extern "C" void fn_1_10AD0(Heiho *self)
{
    fn_1_10B9C(self, 1, 1);
    if (self->sndPlayed == 0 && (f32)self->timer > 60.0f) {
        self->sndPlayed = 1;
        HuAudFXPlay(0x6BA);
    }
    self->timer = self->timer + 1;
}

extern "C" void fn_1_10B6C(Heiho *self)
{
    fn_1_10B9C(self, 0, 0);
}

extern "C" void fn_1_10B9C(Heiho *self, s32 motion, u8 loop)
{
    s16 body = self->body.id;
    s16 mot = *fn_1_CA88(&self->motIds, motion);
    s16 item;
    u32 attr;

    if (self->motion != motion) {
        Hu3DModelAttrReset(body, 0x40000002);
        if (loop == 0) {
            attr = 0x40000001;
        } else {
            attr = 0;
        }
        Hu3DMotionShiftSet(body, mot, 0.0f, 8.0f, attr);
        self->motion = motion;
        if (motion == 1) {
            item = self->item.id;
            Hu3DModelAttrReset(item, 0x40000002);
            Hu3DModelAttrReset(item, 0x40000001);
            Hu3DMotionTimeSet(item, 0.0f);
        }
    }
}

extern "C" void fn_1_10CA0(Heiho *self, s32 bit)
{
    self->unkC &= ~(1 << bit);
    switch (bit) {
    case 1:
        self->hook = lbl_1_data_678;
        return;
    case 2:
        self->hook = lbl_1_data_684;
    case 0:
        return;
    }
}

extern "C" void *fn_1_10D58(Heiho *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_690;
        if ((HeihoVec *)((char *)self + 0x24) != 0) {
            if ((HeihoVec *)((char *)self + 0x24) != 0) {
                if ((HeihoVec *)((char *)self + 0x24) != 0) {
                    s16 **dp = &self->motIds.data;
                    if (*dp != 0) {
                        s16 **dp2 = &self->motIds.data;
                        s16 *d = *dp2;
                        __dl__FPv(d);
                    }
                }
            }
        }
        fn_1_30F4(&self->item, -1);
        fn_1_30F4(&self->body, -1);
        if ((void *)((char *)self + 0x10) != 0) {
            if (lbl_1_bss_A04 != 0) {
                } else {
                fn_1_1D4(lbl_1_data_668, lbl_1_data_624, 0x3D);
            }
            lbl_1_bss_A04 = 0;
        }
        if (self != 0) {
            self->vtbl = lbl_1_data_1FC;
            if (self != 0) {
                fn_1_1270(self, 0);
            }
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

extern "C" void fn_1_10E98(void)
{
    lbl_1_bss_AB8 = 120;
    lbl_1_bss_ABC = 210;
    lbl_1_bss_AC0 = 210;
    lbl_1_bss_AD0.a = 0;
    lbl_1_bss_AD0.b = 0;
    lbl_1_bss_AD0.c = 0;
    __register_global_object(&lbl_1_bss_AD0, (void *)fn_1_9818, lbl_1_bss_AC4);
}

#ifdef __MWERKS__
__declspec(section ".ctors") void (*const heiho_ctor)(void) = fn_1_10E98;
#else
static struct HeihoSinit { HeihoSinit() { fn_1_10E98(); } } heiho_sinit;
#endif
