#include "REL/m537Dll.h"

#ifdef __MWERKS__
extern "C" void __construct_array(void *ptr, void *ctor, void *dtor, u32 size, u32 n);
extern "C" void __destroy_arr(void *block, void *dtor, u32 size, u32 n);
#else
// Portable stand-ins for the MWCC runtime helpers.
static void __construct_array(void *ptr, void *ctor, void *dtor, u32 size, u32 n)
{
    u8 *p = (u8 *)ptr;

    (void)dtor;
    while (n-- != 0) {
        ((void (*)(void *))ctor)(p);
        p += size;
    }
}

static void __destroy_arr(void *block, void *dtor, u32 size, u32 n)
{
    u8 *p = (u8 *)block + size * n;

    while (n-- != 0) {
        p -= size;
        ((void (*)(void *, s16))dtor)(p, -1);
    }
}
#endif

extern "C" {

extern void *lbl_1_data_1FC[];

// The stage work object (singleton at lbl_1_bss_A14).
struct M537Stage {
    M537_NODE m_node;           /* 0x00 */
    void **m_vptr;              /* 0x08 */
    s32 m_flag;                 /* 0x0C */
    s16 m_mdl;                  /* 0x10 */
    s16 *m_sky;                 /* 0x14 */
    M537DorriePtr m_dorrie[4];  /* 0x18 */
    M537ModelPtr m_part[3];     /* 0x28 */
};

M537Dorrie *fn_1_5E4C(M537Dorrie *self, s32 index);
M537ModelPtr *fn_1_5FB0(M537ModelPtr *self, s16 freeIt);
M537Dorrie *fn_1_61AC(M537Dorrie *self, s16 freeIt);
void fn_1_66D0(M537Dorrie *self, f64 target, f64 duration);
M537Water *fn_1_13524(M537Water *self, s16 mdl);
M537Effect *fn_1_72A0(M537Effect *self);
void fn_1_1DDC(void);
f32 Hu3DMotionMaxTimeGet(s16 modelId);

void fn_1_9490(M537ModelPtr *self);
M537DorriePtr *fn_1_949C(M537DorriePtr *self, s16 freeIt);
void fn_1_94F4(M537DorriePtr *self);
M537Stage *fn_1_95AC(M537Stage *self, s16 freeIt);
void fn_1_96DC(M537Stage *self, s32 bit);
void fn_1_96F4(M537Stage *self);
void fn_1_97C8(M537Stage *self);
void fn_1_9928(M537DorriePtr *self, M537Dorrie *dorrie);

void *lbl_1_data_3FC[5] = { 0, 0, (void *)fn_1_95AC, (void *)fn_1_96DC, (void *)fn_1_1DDC };

#ifdef __MWERKS__
#define M537_RODATA __declspec(section ".rodata")
#else
#define M537_RODATA
#endif

M537_RODATA const f32 lbl_1_rodata_2A8[4] = { 45.0f, -45.0f, 135.0f, -135.0f };
M537_RODATA const f64 lbl_1_rodata_2B8 = 0.0;
M537_RODATA const s32 lbl_1_rodata_2C0[3] = { 0x5F000F, 0x5F0010, 0x5F0011 };

M537Stage *fn_1_9290(M537Stage *self)
{
    M537Stage *ms;
    s16 *sky;
    s16 *light;
    M537Water *water;
    M537Effect *effect;
    s16 id;

    fn_1_1264(self);
    self->m_vptr = lbl_1_data_1FC;
    self->m_flag = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    if (lbl_1_bss_A14 == 0) {
    } else {
        fn_1_1D4("ms_pointer == 0", "singleton.h", 0x35);
    }
    ms = (M537Stage *)((u8 *)self + 0x10);
    if (ms != 0) {
        ms = (M537Stage *)((u8 *)ms - 0x10);
    }
    lbl_1_bss_A14 = ms;
    fn_1_2C94(&self->m_mdl, 0x5F0001);
    self->m_vptr = lbl_1_data_3FC;
    sky = (s16 *)fn_1_0(2);
    if (sky != 0) {
        sky = fn_1_2C94(sky, 0x5F0002);
    }
    self->m_sky = sky;
    __construct_array(self->m_dorrie, (void *)fn_1_94F4, (void *)fn_1_949C, sizeof(M537DorriePtr), 4);
    __construct_array(self->m_part, (void *)fn_1_9490, (void *)fn_1_5FB0, sizeof(M537ModelPtr), 3);
    light = (s16 *)self;
    if (light != 0) {
        light = (s16 *)((u8 *)light + 0x10);
    }
    id = *light;
    Hu3DModelLightInfoSet(id, 1);
    fn_1_304C(&self->m_mdl, 0);
    fn_1_2E08(&self->m_mdl, 0x40000001);
    fn_1_304C(self->m_sky, 0);
    fn_1_31E4(self->m_sky);
    fn_1_96F4(self);
    fn_1_97C8(self);
#ifdef TARGET_PC
    water = (M537Water *)fn_1_0(sizeof(M537Water));
#else
    water = (M537Water *)fn_1_0(0xA08A0);
#endif
    if (water != 0) {
        fn_1_13524(water, id);
    }
#ifdef TARGET_PC
    effect = (M537Effect *)fn_1_0(sizeof(M537Effect));
#else
    effect = (M537Effect *)fn_1_0(0x20);
#endif
    if (effect != 0) {
        fn_1_72A0(effect);
    }
    return self;
}

void fn_1_9490(M537ModelPtr *self)
{
    self->p = 0;
}

M537DorriePtr *fn_1_949C(M537DorriePtr *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_61AC(self->p, 1);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_94F4(M537DorriePtr *self)
{
    fn_1_9928(self, 0);
}

M537Stage *fn_1_9520(M537Stage *self, s16 freeIt)
{
    if (self != 0) {
        if (lbl_1_bss_A14 != 0) {
        } else {
            fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A14 = 0;
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M537Stage *fn_1_95AC(M537Stage *self, s16 freeIt)
{
    if (self != 0) {
        self->m_vptr = lbl_1_data_3FC;
        __destroy_arr(self->m_part, (void *)fn_1_5FB0, sizeof(M537ModelPtr), 3);
        __destroy_arr(self->m_dorrie, (void *)fn_1_949C, sizeof(M537DorriePtr), 4);
        if ((u8 *)self + 0x14 != 0) {
            fn_1_30F4(self->m_sky, 1);
        }
        fn_1_30F4(&self->m_mdl, 0);
        if ((u8 *)self + 0x10 != 0) {
            if (lbl_1_bss_A14 != 0) {
            } else {
                fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
            }
            lbl_1_bss_A14 = 0;
        }
        if (self != 0) {
            self->m_vptr = lbl_1_data_1FC;
            if (self != 0) {
                fn_1_1270(self, 0);
            }
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M537Dorrie *fn_1_96CC(M537Stage *self, s32 index)
{
    return self->m_dorrie[index].p;
}

void fn_1_96DC(M537Stage *self, s32 bit)
{
    self->m_flag &= ~(1 << bit);
}

// Creates the four dorries around the stage and turns each to its start angle.
void fn_1_96F4(M537Stage *self)
{
    M537Dorrie *d;
    s32 i;

    for (i = 0; i < 4; i++) {
#ifdef TARGET_PC
        d = (M537Dorrie *)fn_1_0(sizeof(M537Dorrie));
#else
        d = (M537Dorrie *)fn_1_0(0x68);
#endif
        if (d != 0) {
            d = fn_1_5E4C(d, i);
        }
        if (self->m_dorrie[i].p != d) {
            fn_1_61AC(self->m_dorrie[i].p, 1);
            self->m_dorrie[i].p = d;
        }
        fn_1_66D0(self->m_dorrie[i].p, lbl_1_rodata_2A8[i], lbl_1_rodata_2B8);
    }
}

// Creates the three animated stage parts and starts them at staggered times.
void fn_1_97C8(M537Stage *self)
{
    s16 *m;
    s16 id;
    f32 tmax;
    s32 i;

    for (i = 0; i < 3; i++) {
        m = (s16 *)fn_1_0(2);
        if (m != 0) {
            m = fn_1_2C94(m, lbl_1_rodata_2C0[i]);
        }
        if (self->m_part[i].p != m) {
            fn_1_30F4(self->m_part[i].p, 1);
            self->m_part[i].p = m;
        }
        id = *self->m_part[i].p;
        tmax = Hu3DMotionMaxTimeGet(id);
        Hu3DMotionSpeedSet(id, tmax / 499.8f);
        Hu3DMotionTimeSet(id, tmax - (f32)i);
        fn_1_2E08(self->m_part[i].p, 0x40000001);
    }
}

void fn_1_9928(M537DorriePtr *self, M537Dorrie *dorrie)
{
    self->p = dorrie;
}

}
