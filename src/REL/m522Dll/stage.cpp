#define _MATH_H
#include "REL/m522Dll.h"

#include "game/audio.h"
#include "game/hu3d.h"

#ifdef __MWERKS__
#define STAGE_RODATA __declspec(section ".rodata")
#else
#define STAGE_RODATA
#endif

/* Owner of a model handle (array element of the stage). */
typedef struct M522ModelPtr_s {
    M522_MODEL *model;
} M522_MODELPTR;

typedef struct M522FirePtr_s {
    M522_FIRE *fire;
} M522_FIREPTR;

typedef struct M522Owner_s {
    void *obj;
} M522_OWNER;

typedef struct M522Stage_s {
    /* 0x00 */ M522_NODE node;
    /* 0x08 */ M522_VTFN *vtbl;
    /* 0x0C */ u32 flags;
    /* 0x10 */ M522_FIRE *fire;
    /* 0x14 */ M522_MODEL *modelA;
    /* 0x18 */ M522_MODEL *modelB;
    /* 0x1C */ M522_MODEL *modelC;
    /* 0x20 */ M522_MODELPTR holes[2];
    /* 0x28 */ void *owner;
    /* 0x2C */ s32 field2C;
    /* 0x30 */ s32 sound;
    /* 0x34 */ s32 stageMax;
    /* 0x38 */ s32 stageNow;
    /* 0x3C */ s32 slot;
    /* 0x40 */ f32 drop;
    /* 0x44 */ f32 spin;
} M522_STAGE;

extern "C" {
extern void *lbl_1_bss_18;
extern void *lbl_1_bss_1C;
extern M522_CAMERA *lbl_1_bss_A1C;
extern M522_NODE lbl_1_bss_2C;
extern M522_VTFN lbl_1_data_3A8[];
extern char *lbl_1_data_540[8];

void MgActorColMapInit(HU3DMODELID *mdlId, s16 mdlNum, int bodyMax);
void *__construct_array(void *ptr, void *ctor, void *dtor, u32 size, u32 count);
void __destroy_arr(void *ptr, void *dtor, u32 size, u32 count);

M522_FIRE *fn_1_0(M522_FIRE *fire);
void *fn_1_360(M522_FIRE *fire, s16 flag);
void fn_1_428(M522_FIRE *fire);
void fn_1_460(M522_FIRE *fire);
void fn_1_474(M522_FIRE *fire);
void *fn_1_4950(void *obj);
void fn_1_4CF8(void *obj, s16 flag);
void fn_1_4E0C(void *obj);
void *fn_1_689C(u32 size);
void fn_1_6964(void *ptr);
void fn_1_6A70(const char *cond, const char *file, s32 line);
void fn_1_8004(M522_NODE *node);
void fn_1_8010(M522_NODE *node, s32 flag);
void fn_1_80D0(M522_NODE *node, M522_NODE *other);
M522_FIREEFF *fn_1_824(M522_FIREEFF *eff);
M522_MODEL *fn_1_9A34(M522_MODEL *model, s32 dataNum);
void fn_1_9B8(M522_MODELPTR *ptr, s16 flag);
void fn_1_9BA8(M522_MODEL *model, u32 attr);
void fn_1_9C94(M522_MODEL *model, M522_VEC3 *pos);
void fn_1_9CDC(M522_MODEL *model, M522_VEC3 *rot);
void fn_1_9DEC(M522_MODEL *model, s32 layer);
void *fn_1_9E94(M522_MODEL *model, s16 flag);
HSFOBJECT *fn_1_9EF0(M522_MODEL *model, char *name);
void fn_1_9FE4(M522_MODEL *model, char *name);
void fn_1_D718(M522_CAMERA *camera, f32 offset);
s16 fn_1_F564(s32 player);

M522_STAGE *fn_1_E85C(M522_STAGE *self);
void *fn_1_EB78(M522_OWNER *owner, s16 flag);
void fn_1_EBD0(M522_MODELPTR *ptr);
void *fn_1_EBDC(M522_FIREPTR *owner, s16 flag);
void *fn_1_EC34(M522_STAGE *self, s16 flag);
void *fn_1_ECC0(M522_STAGE *self, s16 flag);
void fn_1_EE24(M522_STAGE *self);
void fn_1_F200(M522_STAGE *self, s32 bit);
void fn_1_F218(M522_STAGE *self);
void fn_1_F34C(M522_STAGE *self);
void fn_1_F4E0(M522_STAGE *self);
}

char lbl_1_data_560[] = "ms_pointer == 0";
char lbl_1_data_570[] = "singleton.h";
char lbl_1_data_57C[] = "ms_pointer != 0";

M522_VTFN lbl_1_data_58C[] = {
    (M522_VTFN)0,
    (M522_VTFN)0,
    (M522_VTFN)fn_1_ECC0,
    (M522_VTFN)fn_1_F200,
    (M522_VTFN)fn_1_F218,
};

extern "C" {
STAGE_RODATA extern const s16 lbl_1_rodata_43C[2];
STAGE_RODATA extern const M522_VEC3 lbl_1_rodata_440;
STAGE_RODATA extern const s32 lbl_1_rodata_44C[8];
STAGE_RODATA extern const f32 lbl_1_rodata_478[9];
STAGE_RODATA extern const M522_VEC3 lbl_1_rodata_49C;
STAGE_RODATA extern const M522_VEC3 lbl_1_rodata_4B0;

STAGE_RODATA const s16 lbl_1_rodata_43C[2] = { 0, 0 };
STAGE_RODATA const M522_VEC3 lbl_1_rodata_440 = { 0.0f, 0.0f, 0.0f };
STAGE_RODATA const s32 lbl_1_rodata_44C[8] = { 0x73F, 0x73D, 0x73C, 0x743, 0x742, 0x741, 0x740, 0x73E };
STAGE_RODATA const f32 lbl_1_rodata_478[9] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f };
STAGE_RODATA const M522_VEC3 lbl_1_rodata_49C = { 0.0f, 0.0f, 0.0f };
STAGE_RODATA const M522_VEC3 lbl_1_rodata_4B0 = { 0.0f, 0.0f, 0.0f };
}

extern "C" {

M522_STAGE *fn_1_E85C(M522_STAGE *self)
{
    M522_NODE *node;
    M522_STAGE *singleton;
    M522_FIRE *fire;
    M522_MODEL *modelA;
    M522_MODEL *modelB;
    M522_MODEL *modelC;
    void *owner;
    M522_FIREEFF *eff;
    M522_MODEL *tmpA;
    M522_MODEL *tmpB;
    M522_MODEL *tmpC;
    M522_MODEL *tmpD;
    M522_MODEL *tmpE;
    M522_MODEL *tmpF;

    node = &self->node;
    fn_1_8004(node);
    self->vtbl = lbl_1_data_3A8;
    self->flags = 0;
    fn_1_80D0(&self->node, &lbl_1_bss_2C);
    if (lbl_1_bss_18 == 0) {
    } else {
        fn_1_6A70(lbl_1_data_560, lbl_1_data_570, 0x35);
    }
    singleton = self;
    lbl_1_bss_18 = singleton;
    self->vtbl = lbl_1_data_58C;
    fire = (M522_FIRE *)fn_1_689C(sizeof(M522_FIRE));
    if (fire != 0) {
        fire = fn_1_0(fire);
    }
    self->fire = fire;
    modelA = (M522_MODEL *)fn_1_689C(sizeof(M522_MODEL));
    if (modelA != 0) {
        modelA = fn_1_9A34(modelA, 0x500003);
    }
    self->modelA = modelA;
    modelB = (M522_MODEL *)fn_1_689C(sizeof(M522_MODEL));
    if (modelB != 0) {
        modelB = fn_1_9A34(modelB, 0x500001);
    }
    self->modelB = modelB;
    modelC = (M522_MODEL *)fn_1_689C(sizeof(M522_MODEL));
    if (modelC != 0) {
        modelC = fn_1_9A34(modelC, 0x500007);
    }
    self->modelC = modelC;
#ifdef TARGET_PC
    {
        s32 i;
        for (i = 0; i < 2; i++) {
            fn_1_EBD0(&self->holes[i]);
        }
    }
#else
    __construct_array(self->holes, (void *)fn_1_EBD0, (void *)fn_1_9B8, sizeof(M522_MODELPTR), 2);
#endif
    owner = fn_1_689C(0x1C);
    if (owner != 0) {
        owner = fn_1_4950(owner);
    }
    self->owner = owner;
    self->field2C = -1;
    self->sound = HuAudFXPlay(0x3A2);
    self->stageMax = 8;
    self->stageNow = self->stageMax;
    self->slot = 0;
    self->drop = 0.0f;
    self->spin = 0.0f;
    tmpA = self->modelA;
    fn_1_9DEC(tmpA, 3);
    tmpB = self->modelA;
    fn_1_9BA8(tmpB, 0x400000);
    tmpC = self->modelB;
    fn_1_9DEC(tmpC, 0);
    tmpD = self->modelB;
    fn_1_9BA8(tmpD, 0x40000001);
    tmpE = self->modelC;
    fn_1_9DEC(tmpE, 4);
    Hu3DShadowCreate(35.0f, 1300.0f, 2000.0f);
    Hu3DShadowSizeSet(0x1E0);
    Hu3DShadowColSet(0x20, 0x10, 0);
    Hu3DShadowTPLvlSet(0.6f);
    tmpF = self->modelC;
    fn_1_9FE4(tmpF, lbl_1_data_540[self->stageMax - 1]);
    eff = (M522_FIREEFF *)fn_1_689C(sizeof(M522_FIREEFF));
    if (eff != 0) {
        fn_1_824(eff);
    }
    fn_1_EE24(self);
    return self;
}

void *fn_1_EB78(M522_OWNER *owner, s16 flag)
{
    if (owner != 0) {
        fn_1_4CF8(owner->obj, 1);
        if (flag > 0) {
            fn_1_6964(owner);
        }
    }
    return owner;
}

void fn_1_EBD0(M522_MODELPTR *ptr)
{
    ptr->model = 0;
}

void *fn_1_EBDC(M522_FIREPTR *owner, s16 flag)
{
    if (owner != 0) {
        fn_1_360(owner->fire, 1);
        if (flag > 0) {
            fn_1_6964(owner);
        }
    }
    return owner;
}

void *fn_1_EC34(M522_STAGE *self, s16 flag)
{
    if (self != 0) {
        if (lbl_1_bss_18 != 0) {
        } else {
            fn_1_6A70(lbl_1_data_57C, lbl_1_data_570, 0x3D);
        }
        lbl_1_bss_18 = 0;
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

void *fn_1_ECC0(M522_STAGE *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_58C;
        HuAudFXStop(self->sound);
        if (&self->owner != 0) {
            fn_1_4CF8(self->owner, 1);
        }
#ifdef TARGET_PC
        {
            s32 i;
            for (i = 1; i >= 0; i--) {
                fn_1_9B8(&self->holes[i], -1);
            }
        }
#else
        __destroy_arr(self->holes, (void *)fn_1_9B8, sizeof(M522_MODELPTR), 2);
#endif
        if (&self->modelC != 0) {
            fn_1_9E94(self->modelC, 1);
        }
        if (&self->modelB != 0) {
            fn_1_9E94(self->modelB, 1);
        }
        if (&self->modelA != 0) {
            fn_1_9E94(self->modelA, 1);
        }
        if (&self->fire != 0) {
            fn_1_360(self->fire, 1);
        }
        if (&self->fire != 0) {
            if (lbl_1_bss_18 != 0) {
            } else {
                fn_1_6A70(lbl_1_data_57C, lbl_1_data_570, 0x3D);
            }
            lbl_1_bss_18 = 0;
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
    return self;
}

void fn_1_EE24(M522_STAGE *self)
{
    s16 ids[2];
    M522_MODEL *model;
    s32 i;

    for (i = 0; i < 2; i++) {
        model = (M522_MODEL *)fn_1_689C(sizeof(M522_MODEL));
        if (model != 0) {
            model = fn_1_9A34(model, 0x500008);
        }
        if (self->holes[i].model != model) {
            fn_1_9E94(self->holes[i].model, 1);
            self->holes[i].model = model;
        }
        fn_1_9BA8(self->holes[i].model, 1);
    }
    ids[0] = lbl_1_rodata_43C[0];
    ids[1] = lbl_1_rodata_43C[1];
    ids[0] = self->holes[0].model->id;
    ids[1] = self->holes[1].model->id;
    MgActorColMapInit(ids, 2, 8);
}

void fn_1_EF38(M522_STAGE *self, s32 player)
{
    M522_VEC3 pos;
    s16 pan;

    if (self->stageNow <= 0 || self->drop > 0.0f) {
        return;
    }
    self->stageNow = self->stageNow - 1;
    self->drop = 2000.0f;
    pos = lbl_1_rodata_440;
    pos.y = (f32)((self->stageNow - self->stageMax) * 2 * 100);
    fn_1_9C94(self->holes[self->slot].model, &pos);
    self->slot = self->slot ^ 1;
    if (self->stageNow > 0) {
        if (lbl_1_bss_A1C != 0) {
        } else {
            fn_1_6A70(lbl_1_data_57C, lbl_1_data_570, 0x27);
        }
        fn_1_D718(lbl_1_bss_A1C, 200.0f);
        fn_1_9FE4(self->modelC, lbl_1_data_540[self->stageNow - 1]);
    } else {
        fn_1_460(self->fire);
        fn_1_474(self->fire);
    }
    switch (self->stageNow) {
    case 2:
    case 4:
    case 6:
        if (lbl_1_bss_1C != 0) {
        } else {
            fn_1_6A70(lbl_1_data_57C, lbl_1_data_570, 0x27);
        }
        fn_1_4E0C(lbl_1_bss_1C);
        break;
    }
    pan = fn_1_F564(player);
    HuAudFXPlayPan(0x73B, pan);
    HuAudFXPlayPan(lbl_1_rodata_44C[self->stageNow], pan);
    HuAudFXPlayPan(0x744, pan);
}

u8 fn_1_F194(M522_STAGE *self)
{
    return self->drop <= 0.0f;
}

f32 fn_1_F1B8(M522_STAGE *self)
{
    return (f32)(((self->stageNow * 2) + 1) * 100);
}

s32 fn_1_F1F8(M522_STAGE *self)
{
    return self->stageNow;
}

void fn_1_F200(M522_STAGE *self, s32 bit)
{
    self->flags = self->flags & ~(1 << bit);
}

void fn_1_F218(M522_STAGE *self)
{
    f32 view[9];
    f32 height;

    if (self->drop > 0.0f) {
        fn_1_F34C(self);
    }
    height = (f32)(((self->stageNow * 2) + 1) * 100);
    view[0] = lbl_1_rodata_478[0];
    view[1] = lbl_1_rodata_478[1];
    view[2] = lbl_1_rodata_478[2];
    view[3] = lbl_1_rodata_478[3];
    view[4] = lbl_1_rodata_478[4];
    view[5] = lbl_1_rodata_478[5];
    view[6] = lbl_1_rodata_478[6];
    view[7] = lbl_1_rodata_478[7];
    view[8] = lbl_1_rodata_478[8];
    view[1] = height - 1300.0f;
    view[7] = height;
    Hu3DShadowPosSet((HuVecF *)&view[0], (HuVecF *)&view[3], (HuVecF *)&view[6]);
    fn_1_428(self->fire);
    fn_1_F4E0(self);
}

void fn_1_F34C(M522_STAGE *self)
{
    char *name;
    M522_MODEL *model;
    HSFOBJECT *obj;
    f32 *posX;
    f32 step;
    M522_VEC3 pos;
    M522_MODEL *hole;
    M522_MODEL *holeCopy;

    name = lbl_1_data_540[self->stageNow];
    model = self->modelC;
    obj = fn_1_9EF0(model, name);
    posX = &obj->mesh.base.pos.x;
    step = 41.666668f;
    *posX = *posX + step;
    self->drop = self->drop - step;
    if (self->drop <= 0.0f) {
        self->drop = 0.0f;
        obj->flags = obj->flags | 0x400;
    }
    pos = lbl_1_rodata_49C;
    pos.x = *posX;
    pos.y = (f32)(((self->stageNow + 1) - self->stageMax) * 200);
    if (self->drop <= 0.0f) {
        pos.x = 0.0f;
        pos.y = pos.y - 400.0f;
    }
    hole = self->holes[self->slot].model;
    holeCopy = hole;
    fn_1_9C94(holeCopy, &pos);
}

void fn_1_F4E0(M522_STAGE *self)
{
    M522_VEC3 rot;

    rot = lbl_1_rodata_4B0;
    rot.y = self->spin;
    fn_1_9CDC(self->modelB, &rot);
    self->spin = (f32)((f64)self->spin - 0.004166666666666667);
}

}
