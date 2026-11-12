#include "REL/m522Dll.h"

#include "game/data.h"
#include "game/hu3d.h"
#include "game/sprite.h"

#ifdef __MWERKS__
#define SUNFIRE_RODATA __declspec(section ".rodata")
#else
#define SUNFIRE_RODATA
#endif

/* The sprite bank manager (singleton).  Its list node and vtable come first,
 * the two sprite sheets are read in the constructor. */
typedef struct M522SprMgr_s {
    /* 0x00 */ M522_NODE node;
    /* 0x08 */ M522_VTFN *vtbl;
    /* 0x0C */ u32 flags;
    /* 0x10 */ ANIMDATA *sprite[2];
    /* 0x18 */ s32 handleGrp;
} M522_SPRMGR;

extern "C" {
extern M522_SPRMGR *lbl_1_bss_14;
extern M522_NODE lbl_1_bss_2C;
extern M522_VTFN lbl_1_data_3A8[];

void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, s32 wrapS, s32 wrapT, s32 filter);
void *__register_global_object(void *object, void *destructor, void *regmem);
void fn_1_6964(void *ptr);
void fn_1_6A70(const char *cond, const char *file, s32 line);
s32 fn_1_7D74(s32 grp, void *obj);
void fn_1_7E04(s32 grp, s32 handle);
void fn_1_7EC8(s32 grp, s32 handle);
void fn_1_8004(M522_NODE *node);
void fn_1_8010(M522_NODE *node, s32 flag);
void fn_1_807C(M522_NODE *node);
void fn_1_80D0(M522_NODE *node, M522_NODE *other);
void fn_1_8100(M522_NODE *dst, M522_NODE *src);
void fn_1_9A7C(s16 *hook, HU3DMODELHOOK func);
void fn_1_9E5C(s16 *hook, void *data);
void fn_1_9E94(s16 *hook, s16 flag);
void fn_1_26A4(void);

void fn_1_E04C(M522_SPRMGR *self, s16 flag);
void fn_1_E1D4(M522_SPRMGR *self, s32 bit);
void fn_1_E250(void);
void fn_1_E270(void);
void fn_1_E2A0(M522_EFFECT *node);
void fn_1_E2D4(void);
void fn_1_E31C(void);
void fn_1_E320(void);
void fn_1_E350(M522_EFFECT *node);
void fn_1_E384(void);
M522_EFFECT *fn_1_E534(M522_EFFECT *self, s16 flag);
void fn_1_E5FC(M522_NODE *head, void (*func)(M522_EFFECT *));
void fn_1_E65C(M522_NODE *head, void (*func)(M522_EFFECT *));
void fn_1_E7A4(M522_NODE *first, M522_NODE *stop, void (*func)(M522_EFFECT *));
M522_NODE *fn_1_E74C(M522_NODE *node, s16 flag);
}

/* The singleton.h asserts. */
char lbl_1_data_4A8[] = "ms_pointer == 0";
char lbl_1_data_4B8[] = "singleton.h";
char lbl_1_data_4C4[] = "ms_pointer != 0";

M522_VTFN lbl_1_data_4D4[] = {
    (M522_VTFN)0,
    (M522_VTFN)0,
    (M522_VTFN)fn_1_E534,
    (M522_VTFN)fn_1_E2D4,
    (M522_VTFN)fn_1_E31C,
    (M522_VTFN)fn_1_26A4,
};

M522_VTFN lbl_1_data_4EC[] = {
    (M522_VTFN)0,
    (M522_VTFN)0,
    (M522_VTFN)fn_1_E04C,
    (M522_VTFN)fn_1_E1D4,
    (M522_VTFN)fn_1_E250,
};

char lbl_1_data_500[] = "brock_a";
char lbl_1_data_508[] = "brock_b";
char lbl_1_data_510[] = "brock_c";
char lbl_1_data_518[] = "brock_d";
char lbl_1_data_520[] = "brock_e";
char lbl_1_data_528[] = "brock_f";
char lbl_1_data_530[] = "brock_g";
char lbl_1_data_538[] = "brock_h";

char *lbl_1_data_540[8] = {
    lbl_1_data_500, lbl_1_data_508, lbl_1_data_510, lbl_1_data_518,
    lbl_1_data_520, lbl_1_data_528, lbl_1_data_530, lbl_1_data_538,
};


/* Registration record for the global list head, then the head itself and the
 * handle group of the effects. */
u8 lbl_1_bss_A30[0xC];
M522_NODE lbl_1_bss_A3C;
s32 lbl_1_bss_A44;

extern "C" {
SUNFIRE_RODATA extern const s32 lbl_1_rodata_420[2];
SUNFIRE_RODATA const s32 lbl_1_rodata_420[2] = { 0x500005, 0x500006 };
}

extern "C" {

M522_SPRMGR *fn_1_DEA0(M522_SPRMGR *self, s32 handleGrp)
{
    s32 i;
    M522_SPRMGR *singleton;

    fn_1_8004(&self->node);
    self->vtbl = lbl_1_data_3A8;
    self->flags = 0;
    fn_1_80D0(&self->node, &lbl_1_bss_2C);
    if (lbl_1_bss_14 == 0) {
    } else {
        fn_1_6A70(lbl_1_data_4A8, lbl_1_data_4B8, 0x35);
    }
    singleton = self;
    lbl_1_bss_14 = singleton;
    self->vtbl = lbl_1_data_4EC;
    self->handleGrp = handleGrp;
    for (i = 0; i < 2; i++) {
        self->sprite[i] = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_rodata_420[i], 0x10000000, HEAP_MODEL));
    }
    lbl_1_bss_A44 = self->handleGrp;
    return self;
}

void *fn_1_DFC0(void *self, s16 flag)
{
    if (self != 0) {
        if (lbl_1_bss_14 != 0) {
        } else {
            fn_1_6A70(lbl_1_data_4C4, lbl_1_data_4B8, 0x3D);
        }
        lbl_1_bss_14 = 0;
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

void fn_1_E04C(M522_SPRMGR *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_4EC;
        fn_1_E384();
        if (self != 0) {
            if (lbl_1_bss_14 != 0) {
            } else {
                fn_1_6A70(lbl_1_data_4C4, lbl_1_data_4B8, 0x3D);
            }
            lbl_1_bss_14 = 0;
        }
        if (self != 0) {
            self->vtbl = lbl_1_data_3A8;
            if (self != 0) {
                fn_1_8010(&self->node, 0);
            }
        }
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
}

void fn_1_E11C(s32 index, s32 bmpNo, s32 texMapId, s32 wrapS, s32 wrapT)
{
    M522_SPRMGR *mgr;
    M522_SPRMGR *inst;
    M522_SPRMGR *self;

    if (lbl_1_bss_14 != 0) {
    } else {
        fn_1_6A70(lbl_1_data_4C4, lbl_1_data_4B8, 0x27);
    }
    mgr = lbl_1_bss_14;
    inst = mgr;
    self = inst;
    HuSprTexLoad(self->sprite[index], 0, (s16)bmpNo, texMapId, wrapS, wrapT);
}

void fn_1_E1D4(M522_SPRMGR *self, s32 bit)
{
    self->flags = self->flags & ~(1 << bit);
    switch (bit) {
    case 3:
        fn_1_E320();
        self->flags = self->flags & ~(1 << bit);
        break;
    }
}

void fn_1_E250(void)
{
    fn_1_E270();
}

void fn_1_E270(void)
{
    fn_1_E65C(&lbl_1_bss_A3C, fn_1_E2A0);
}

void fn_1_E2A0(M522_EFFECT *node)
{
    ((void (*)(M522_EFFECT *))node->vtbl[3])(node);
}

void fn_1_E2D4(void)
{
}

void fn_1_E2D8(HU3DMODEL *model, Mtx *mtx)
{
    M522_EFFECT *effect = (M522_EFFECT *)model->hookData;
    ((void (*)(M522_EFFECT *, Mtx *))effect->vtbl[4])(effect, mtx);
}

void fn_1_E31C(void)
{
}

void fn_1_E320(void)
{
    fn_1_E5FC(&lbl_1_bss_A3C, fn_1_E350);
}

void fn_1_E350(M522_EFFECT *node)
{
    ((void (*)(M522_EFFECT *))node->vtbl[5])(node);
}

void fn_1_E384(void)
{
    u8 empty;

    for (;;) {
        empty = 0;
        if (lbl_1_bss_A3C.prev == &lbl_1_bss_A3C && lbl_1_bss_A3C.prev == lbl_1_bss_A3C.next) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        if (lbl_1_bss_A3C.next != 0) {
            M522_EFFECT *effect = (M522_EFFECT *)lbl_1_bss_A3C.next;
            ((void (*)(M522_EFFECT *, s32))effect->vtbl[2])(effect, 1);
        }
    }
}

M522_EFFECT *fn_1_E44C(M522_EFFECT *self)
{
    fn_1_8004(&self->node);
    fn_1_9A7C(&self->hook, fn_1_E2D8);
    self->vtbl = lbl_1_data_4D4;
    self->handle = fn_1_7D74(lbl_1_bss_A44, self);
    fn_1_80D0(&self->node, &lbl_1_bss_A3C);
    fn_1_9E5C(&self->hook, self);
    return self;
}

M522_NODE *fn_1_E4DC(M522_NODE *node, s16 flag)
{
    if (node != 0) {
        fn_1_8010(node, 0);
        if (flag > 0) {
            fn_1_6964(node);
        }
    }
    return node;
}

M522_EFFECT *fn_1_E534(M522_EFFECT *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_4D4;
        fn_1_7E04(lbl_1_bss_A44, self->handle);
        fn_1_9E94(&self->hook, 0);
        if (self != 0) {
            fn_1_8010(&self->node, 0);
        }
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

s32 fn_1_E5C0(M522_EFFECT *self)
{
    return self->handle;
}

void fn_1_E5C8(s32 handle)
{
    fn_1_7EC8(lbl_1_bss_A44, handle);
}

void fn_1_E5FC(M522_NODE *head, void (*func)(M522_EFFECT *))
{
    M522_NODE *list = head;
    M522_NODE *first = head->prev;
    M522_NODE *begin = first;

    fn_1_E7A4(begin, list, func);
}

void fn_1_E65C(M522_NODE *head, void (*func)(M522_EFFECT *))
{
    M522_NODE tmp;
    u8 empty;
    u8 done;

    empty = 0;
    if (head->prev == head && head->prev == head->next) {
        empty = 1;
    }
    if (empty == 0) {
        fn_1_8004(&tmp);
        fn_1_8100(head, &tmp);
        for (;;) {
            done = 0;
            if (tmp.prev == &tmp && tmp.prev == tmp.next) {
                done = 1;
            }
            if (done != 0) {
                break;
            }
            fn_1_807C(tmp.prev);
            fn_1_80D0(tmp.prev, head);
            func((M522_EFFECT *)tmp.prev);
        }
        fn_1_8010(&tmp, 0);
    }
}

M522_NODE *fn_1_E74C(M522_NODE *node, s16 flag)
{
    if (node != 0) {
        fn_1_8010(node, 0);
        if (flag > 0) {
            fn_1_6964(node);
        }
    }
    return node;
}

void fn_1_E7A4(M522_NODE *first, M522_NODE *stop, void (*func)(M522_EFFECT *))
{
    M522_NODE *node = first;

    while (node != stop) {
        func((M522_EFFECT *)node);
        node = node->prev;
    }
}

void fn_1_E804(void)
{
    M522_NODE *list = &lbl_1_bss_A3C;
    M522_NODE *obj;

    fn_1_8004(list);
    obj = list;
    __register_global_object(obj, (void *)fn_1_E74C, lbl_1_bss_A30);
}

}
