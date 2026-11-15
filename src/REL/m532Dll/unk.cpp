// m532Dll "unk": singleton that updates / draws the list of unk objects, which
// are registered in the game master's handle table and drawn by a Hu3D hook.
#define _MATH_H
extern "C" {
#include "dolphin.h"
#include "game/hu3d.h"
}
#include "REL/m532Dll/unk.h"

#ifdef __MWERKS__
#define M532_CTORS __declspec(section ".ctors")
#endif

extern "C" {

void __register_global_object(void *obj, void *dtor, void *reg);
void fn_1_1AF8(void);
void fn_1_1AFC(void);
// Task base vtable (ball.cpp).
extern M532TaskVtbl lbl_1_data_B4;

char lbl_1_data_650[] = "ms_pointer == 0";
char lbl_1_data_660[] = "singleton.h";
char lbl_1_data_66C[] = "ms_pointer != 0";

M532UnkObj *fn_1_EAF0(M532UnkObj *self, s16 flag);
void fn_1_E894(M532UnkObj *self);
M532Task *fn_1_E6C4(M532Task *self, s16 flag);
void fn_1_E794(M532Task *self, s32 bit);
void fn_1_E810(M532Task *self);
void fn_1_E898(HU3DMODEL *model, Mtx *mtx);

M532UnkObjVtbl lbl_1_data_67C = { { 0, 0 },
                                  fn_1_EAF0,
                                  fn_1_E894,
                                  (void (*)(M532UnkObj *, Mtx *))fn_1_1AF8,
                                  (void (*)(M532UnkObj *))fn_1_1AFC };
M532TaskVtbl lbl_1_data_694 = { { 0, 0 }, fn_1_E6C4, fn_1_E794, fn_1_E810 };

// ms_pointer of the singleton (lives in m532.cpp's .bss range in the original).
M532Unk *lbl_1_bss_10;
u8 lbl_1_bss_A68[0xC];
M532Node lbl_1_bss_A74;
M532HandleMan *lbl_1_bss_A7C;

M532Unk *fn_1_E54C(M532Unk *self, M532HandleMan *handleMan)
{
    fn_1_32B0(&self->base.link);
    self->base.vtbl = &lbl_1_data_B4;
    self->base.flags = 0;
    fn_1_337C(&self->base.link, &lbl_1_bss_24);
    if (lbl_1_bss_10 != 0) {
        fn_1_1D1C(lbl_1_data_650, lbl_1_data_660, 0x35);
    }
    lbl_1_bss_10 = self;
    self->base.vtbl = &lbl_1_data_694;
    self->handleMan = handleMan;
    lbl_1_bss_A7C = self->handleMan;
    return self;
}

M532Task *fn_1_E638(M532Task *self, s16 flag)
{
    if (self != 0) {
        if (lbl_1_bss_10 == 0) {
            fn_1_1D1C(lbl_1_data_66C, lbl_1_data_660, 0x3D);
        }
        lbl_1_bss_10 = 0;
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

void fn_1_E940(void);

M532Task *fn_1_E6C4(M532Task *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_694;
        fn_1_E940();
        if (self != 0) {
            if (lbl_1_bss_10 == 0) {
                fn_1_1D1C(lbl_1_data_66C, lbl_1_data_660, 0x3D);
            }
            lbl_1_bss_10 = 0;
        }
        if (self != 0) {
            self->vtbl = &lbl_1_data_B4;
            if (self != 0) {
                fn_1_32BC(&self->link, 0);
            }
        }
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

void fn_1_E8DC(void);

void fn_1_E794(M532Task *self, s32 bit)
{
    self->flags = self->flags & ~(1 << bit);
    switch (bit) {
    case 3:
        fn_1_E8DC();
        self->flags = self->flags & ~(1 << bit);
        break;
    }
}

void fn_1_E830(void);

void fn_1_E810(M532Task *self)
{
    fn_1_E830();
}

void fn_1_EC18(M532Node *list, void (*func)(M532Node *));
void fn_1_E860(M532UnkObj *obj);

void fn_1_E830(void)
{
    fn_1_EC18(&lbl_1_bss_A74, (void (*)(M532Node *))fn_1_E860);
}

void fn_1_E860(M532UnkObj *obj)
{
    obj->vtbl->update(obj);
}

void fn_1_E894(M532UnkObj *self)
{
}

// Model hook: draw the object stored in the model's hook data.
void fn_1_E898(HU3DMODEL *model, Mtx *mtx)
{
    M532UnkObj *obj = (M532UnkObj *)model->hookData;

    obj->vtbl->draw(obj, mtx);
}

void fn_1_EBB8(M532Node *list, void (*func)(M532Node *));
void fn_1_E90C(M532UnkObj *obj);

void fn_1_E8DC(void)
{
    fn_1_EBB8(&lbl_1_bss_A74, (void (*)(M532Node *))fn_1_E90C);
}

void fn_1_E90C(M532UnkObj *obj)
{
    obj->vtbl->unused(obj);
}

// Destroy every unk object, last one first.
void fn_1_E940(void)
{
    M532UnkObj *obj;
    u8 empty;

    while (1) {
        empty = 0;
        if (lbl_1_bss_A74.next == &lbl_1_bss_A74 && lbl_1_bss_A74.next == lbl_1_bss_A74.prev) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        obj = (M532UnkObj *)lbl_1_bss_A74.prev;
        if (obj != 0) {
            obj->vtbl->dtor(obj, 1);
        }
    }
}

M532UnkObj *fn_1_EA08(M532UnkObj *self)
{
    fn_1_32B0(&self->link);
    fn_1_4C9C(&self->model, fn_1_E898);
    self->vtbl = &lbl_1_data_67C;
    self->handle = fn_1_3020(lbl_1_bss_A7C, self);
    fn_1_337C(&self->link, &lbl_1_bss_A74);
    fn_1_507C(&self->model, self);
    return self;
}

M532Node *fn_1_EA98(M532Node *self, s16 flag)
{
    if (self != 0) {
        fn_1_32BC(self, 0);
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

M532UnkObj *fn_1_EAF0(M532UnkObj *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_67C;
        fn_1_30B0(lbl_1_bss_A7C, self->handle);
        fn_1_50B4(&self->model, 0);
        if (self != 0) {
            fn_1_32BC(&self->link, 0);
        }
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

s32 fn_1_EB7C(M532UnkObj *self)
{
    return self->handle;
}

void *fn_1_EB84(s32 handle)
{
    return fn_1_3174(lbl_1_bss_A7C, handle);
}

void fn_1_ED60(M532Node *node, M532Node *end, void (*func)(M532Node *));

// Call func on every node of the list.
void fn_1_EBB8(M532Node *list, void (*func)(M532Node *))
{
    fn_1_ED60(list->next, list, func);
}

// Call func on every node; nodes are moved to a scratch list first so that
// func may destroy the node it is handed.
void fn_1_EC18(M532Node *list, void (*func)(M532Node *))
{
    M532Node tmp;
    M532Node *node;
    u8 empty;

    empty = 0;
    if (list->next == list && list->next == list->prev) {
        empty = 1;
    }
    if (empty == 0) {
        fn_1_32B0(&tmp);
        fn_1_33AC(list, &tmp);
        while (1) {
            empty = 0;
            if (tmp.next == &tmp && tmp.next == tmp.prev) {
                empty = 1;
            }
            if (empty != 0) {
                break;
            }
            node = tmp.next;
            fn_1_3328(node);
            fn_1_337C(node, list);
            func(node);
        }
        fn_1_32BC(&tmp, 0);
    }
}

void fn_1_ED08(M532Node *self, s16 flag)
{
    if (self != 0) {
        fn_1_32BC(self, 0);
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
}

void fn_1_ED60(M532Node *node, M532Node *end, void (*func)(M532Node *))
{
    while (node != end) {
        func(node);
        node = node->next;
    }
}

void fn_1_EDC0(void)
{
    fn_1_32B0(&lbl_1_bss_A74);
#ifdef __MWERKS__
    __register_global_object(&lbl_1_bss_A74, (void *)fn_1_ED08, lbl_1_bss_A68);
#endif
}

// The unit's one static initialiser, hooked into the constructor table.
#ifdef __MWERKS__
M532_CTORS void (*const lbl_ctor_EDC0)(void) = fn_1_EDC0;
#else
struct M532UnkStaticInit {
    M532UnkStaticInit() { fn_1_EDC0(); }
};
static M532UnkStaticInit lbl_static_init;
#endif
}
