#define _MATH_H
#include "REL/m522Dll.h"
#include "REL/m522Vec.h"

#include "game/audio.h"
#include "game/gxarray.h"
#include "game/hu3d.h"
#include "game/mg/actman.h"

#ifdef TARGET_PC
#include <math.h>
#else
extern "C" double sin(double x);
extern "C" double cos(double x);
#endif

#ifdef __MWERKS__
#define BUBBLE_RODATA __declspec(section ".rodata")
#else
#define BUBBLE_RODATA
#endif

typedef M522Vec<M522_VEC3> M522_POSVEC;
typedef M522Vec<s32> M522_HANDLES;

/* Spawner: bubbles come out of a ring of start positions. */
typedef struct M522BubbleMgr_s {
    /* 0x00 */ s32 spawned;
    /* 0x04 */ M522_POSVEC positions;
    /* 0x10 */ M522_HANDLES handles;
} M522_BUBBLEMGR;

typedef struct M522Bubble_s {
    /* 0x00 */ M522_EFFECT base;
    /* 0x14 */ M522_PTMF hook;
    /* 0x20 */ s16 light;
    /* 0x24 */ M522_MODEL *model;
    /* 0x28 */ MGACTOR *actor;
    /* 0x2C */ M522_VEC3 home;
    /* 0x38 */ u8 dead;
    /* 0x39 */ u8 inside;
    /* 0x3C */ f32 scale;
    /* 0x40 */ u8 alpha;
    /* 0x44 */ s32 life;
    /* 0x48 */ s32 frame;
} M522_BUBBLE;

extern "C" {
extern M522_NODE lbl_1_bss_2C;
extern M522_VTFN lbl_1_data_3A8[];

void MgActorPosGet(MGACTOR *actorP, HuVecF *pos);
void GXSetVtxAttrFmtv(GXVtxFmt fmt, const GXVtxAttrFmtList *list);
s32 __ptmf_test(M522_PTMF *ptmf);
void __ptmf_scall(void *self);
void fn_1_6A70(const char *cond, const char *file, s32 line);
u32 fn_1_B880(void);
f32 fn_1_BABC(void);
M522_MODEL *fn_1_9A34(M522_MODEL *model, s32 dataNum);
void fn_1_9BA8(M522_MODEL *model, u32 attr);
void fn_1_9DEC(void *model, s32 layer);
void fn_1_9E94(M522_MODEL *model, s16 flag);
M522_EFFECT *fn_1_E44C(M522_EFFECT *self);
void fn_1_E534(M522_EFFECT *self, s16 flag);
s32 fn_1_E5C0(M522_EFFECT *self);
u32 fn_1_E5C8(s32 handle);
M522_BURSTOBJ *fn_1_17E8(M522_BURSTOBJ *self, M522_VEC3 *pos, s32 count, f32 len, f32 time);
void fn_1_4188(void *self, s16 flag);
void fn_1_412C(M522_POSVEC *self);
u8 fn_1_F194(void *stage);
f32 fn_1_F1B8(void *stage);
s32 fn_1_F1F8(void *stage);
s16 fn_1_F564(M522_VEC3 *pos);

M522_BUBBLEMGR *fn_1_4950(M522_BUBBLEMGR *self);
void *fn_1_4AC8(void *self, s16 flag);
void *fn_1_4B5C(void *self, s16 flag);
void *fn_1_4BE8(void *self, s16 flag);
void *fn_1_4C6C(M522_BUBBLEMGR *self, s16 flag);
void *fn_1_4CF8(M522_BUBBLEMGR *self, s16 flag);
void fn_1_4E0C(M522_BUBBLEMGR *self);
M522_HANDLES *fn_1_4EFC(M522_BUBBLEMGR *self);
M522_VEC3 *fn_1_4F88(M522_BUBBLE *self, M522_VEC3 *out);
M522_BUBBLE *fn_1_4FE4(M522_BUBBLE *self, M522_VEC3 *pos, s32 id);
void *fn_1_526C(M522_BUBBLE *self, s16 flag);
void fn_1_52FC(M522_BUBBLE *self);
void fn_1_5354(M522_BUBBLE *self);
void fn_1_557C(M522_BUBBLE *self);
void fn_1_5748(M522_BUBBLE *self);
void fn_1_577C(M522_BUBBLE *self);
void fn_1_5824(M522_BUBBLE *self);
u8 fn_1_58DC(M522_BUBBLE *self);
void fn_1_58FC(M522_BUBBLE *self, s32 count);
void fn_1_5998(M522_BUBBLE *self, Mtx *camera);
void fn_1_5A04(M522_BUBBLE *self, Mtx *camera);
void fn_1_5CF8(void);
M522_VEC3 *fn_1_5D6C(M522_POSVEC *self, u32 index);
s32 *fn_1_5DE4(M522_HANDLES *self, s32 *pos);
void fn_1_5E74(M522_HANDLES *self, s32 *pos, u32 n, s32 *x);
M522_VEC3 *fn_1_622C(M522_POSVEC *self, M522_VEC3 *pos);
void fn_1_62EC(M522_POSVEC *self, M522_VEC3 *pos, u32 n, M522_VEC3 *x);
}

/* The singleton.h asserts. */
char lbl_1_data_E8[] = "ms_pointer == 0";
char lbl_1_data_F8[] = "singleton.h";
char lbl_1_data_104[] = "ms_pointer != 0";

M522_PTMF lbl_1_data_114 = { 0, -1, { (void *)fn_1_557C } };

M522_VTFN lbl_1_data_120[] = {
    (M522_VTFN)0,
    (M522_VTFN)0,
    (M522_VTFN)fn_1_526C,
    (M522_VTFN)fn_1_5354,
    (M522_VTFN)fn_1_5998,
    (M522_VTFN)fn_1_5748,
};

/* Singleton of the spawner. */
void *lbl_1_bss_18;
M522_BUBBLEMGR *lbl_1_bss_1C;

extern "C" {
BUBBLE_RODATA extern const MGACTOR_PARAM lbl_1_rodata_198;
BUBBLE_RODATA extern const GXColor lbl_1_rodata_210;
BUBBLE_RODATA extern const GXVtxDescList lbl_1_rodata_248[3];
BUBBLE_RODATA extern const GXVtxAttrFmtList lbl_1_rodata_260[4];
BUBBLE_RODATA extern const s8 lbl_1_rodata_2A0[0x40];
BUBBLE_RODATA extern const u8 lbl_1_rodata_2E0[0x20];
BUBBLE_RODATA extern const u8 lbl_1_rodata_300[0x20];

BUBBLE_RODATA const MGACTOR_PARAM lbl_1_rodata_198 = { 40.0f, 20.0f, 0, 1, 0, 0, 0, 0 };
BUBBLE_RODATA const GXColor lbl_1_rodata_210 = { 0, 0, 0, 0 };
BUBBLE_RODATA const GXVtxDescList lbl_1_rodata_248[3] = {
    { GX_VA_POS, GX_INDEX8 },
    { GX_VA_CLR0, GX_INDEX8 },
    { GX_VA_NULL, GX_NONE },
};
BUBBLE_RODATA const GXVtxAttrFmtList lbl_1_rodata_260[4] = {
    { GX_VA_POS, GX_POS_XYZ, GX_S8, 6 },
    { GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA4, 0 },
    { GX_VA_NULL, GX_POS_XY, GX_U8, 0 },
    { (GXAttr)0, GX_POS_XY, GX_U8, 0 },
};
BUBBLE_RODATA const s8 lbl_1_rodata_2A0[0x40] = {
    0, 0, 0, 0, 0, 64, 31, 0, 55, 55, 0, 32, 64, 0, 0, 55,
    0, -31, 31, 0, -55, 0, 0, -64, -31, 0, -55, -55, 0, -32, -64, 0,
    0, -55, 0, 32, -32, 0, 55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
BUBBLE_RODATA const u8 lbl_1_rodata_2E0[0x20] = {
    0xFC, 0x0F, 0xF0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
BUBBLE_RODATA const u8 lbl_1_rodata_300[0x20] = {
    0xA0, 0x00, 0x0E, 0, 0, 1, 1, 2, 1, 3, 1, 4, 1, 5, 1, 6,
    1, 7, 1, 8, 1, 9, 1, 10, 1, 11, 1, 12, 1, 1, 1, 0,
};
}

extern "C" {

M522_BUBBLEMGR *fn_1_4950(M522_BUBBLEMGR *self)
{
    M522_VEC3 pos;
    f64 angle;
    s32 i;
    M522_POSVEC *positions;
    M522_HANDLES *handles;

    if (lbl_1_bss_1C == 0) {
    } else {
        fn_1_6A70(lbl_1_data_E8, lbl_1_data_F8, 0x35);
    }
    lbl_1_bss_1C = self;
    self->spawned = 0;
    positions = &self->positions;
    self->positions.m_cap = 0;
    positions->m_size = 0;
    positions->m_data = 0;
    handles = &self->handles;
    self->handles.m_cap = 0;
    handles->m_size = 0;
    handles->m_data = 0;
    angle = 2.0943951023931953 * fn_1_BABC();
    pos.y = 2500.0f;
    for (i = 0; i < 3; i++) {
        pos.x = (f32)(200.0 * sin(angle));
        pos.z = (f32)(200.0 * cos(angle));
        fn_1_62EC(positions, positions->m_data + positions->m_size, 1, &pos);
        angle += 2.0943951023931953;
    }
    return self;
}

void *fn_1_4AC8(void *self, s16 flag)
{
    M522_HANDLES *vec = (M522_HANDLES *)self;

    if (vec != 0) {
        if (vec != 0 && vec != 0 && vec->begin() != 0) {
            fn_1_6964(vec->begin());
        }
        if (flag > 0) {
            fn_1_6964(vec);
        }
    }
    return vec;
}

void *fn_1_4B5C(void *self, s16 flag)
{
    M522_HANDLES *vec = (M522_HANDLES *)self;

    if (vec != 0) {
        if (vec != 0 && vec->begin() != 0) {
            fn_1_6964(vec->begin());
        }
        if (flag > 0) {
            fn_1_6964(vec);
        }
    }
    return vec;
}

void *fn_1_4BE8(void *self, s16 flag)
{
    M522_HANDLES *vec = (M522_HANDLES *)self;

    if (vec != 0) {
        if (vec->begin() != 0) {
            fn_1_6964(vec->begin());
        }
        if (flag > 0) {
            fn_1_6964(vec);
        }
    }
    return vec;
}

void *fn_1_4C6C(M522_BUBBLEMGR *self, s16 flag)
{
    if (self != 0) {
        if (lbl_1_bss_1C != 0) {
        } else {
            fn_1_6A70(lbl_1_data_104, lbl_1_data_F8, 0x3D);
        }
        lbl_1_bss_1C = 0;
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

void *fn_1_4CF8(M522_BUBBLEMGR *self, s16 flag)
{
    if (self != 0) {
        if (&self->handles != 0 && &self->handles != 0 && &self->handles != 0 && self->handles.begin() != 0) {
            fn_1_6964(self->handles.begin());
        }
        if (&self->positions != 0 && &self->positions != 0) {
            fn_1_4188(&self->positions, 0);
        }
        if (self != 0) {
            if (lbl_1_bss_1C != 0) {
            } else {
                fn_1_6A70(lbl_1_data_104, lbl_1_data_F8, 0x3D);
            }
            lbl_1_bss_1C = 0;
        }
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

void fn_1_4E0C(M522_BUBBLEMGR *self)
{
    M522_BUBBLE *bubble;
    s32 handle;
    u32 count;
    u32 index;

    if (self->positions.m_size != 0) {
        count = self->positions.m_size;
        index = fn_1_B880() % count;
        bubble = (M522_BUBBLE *)fn_1_689C(sizeof(M522_BUBBLE));
        if (bubble != 0) {
            bubble = fn_1_4FE4(bubble, fn_1_5D6C(&self->positions, index), self->spawned + 4);
        }
        handle = fn_1_E5C0(&bubble->base);
        fn_1_5E74(&self->handles, self->handles.m_data + self->handles.m_size, 1, &handle);
        fn_1_622C(&self->positions, fn_1_5D6C(&self->positions, index));
        self->spawned = self->spawned + 1;
    }
}

M522_HANDLES *fn_1_4EFC(M522_BUBBLEMGR *self)
{
    s32 *handle;
    s32 *end;

    handle = self->handles.m_data;
    end = self->handles.m_data + self->handles.m_size;
    while (handle != end) {
        if (fn_1_E5C8(*handle) == 0) {
            fn_1_5DE4(&self->handles, handle);
        }
        handle++;
    }
    return &self->handles;
}

M522_VEC3 *fn_1_4F88(M522_BUBBLE *self, M522_VEC3 *out)
{
    M522_VEC3 pos;

    MgActorPosGet(self->actor, (HuVecF *)&pos);
    out->x = pos.x;
    out->y = pos.y;
    out->z = pos.z;
    return out;
}

M522_BUBBLE *fn_1_4FE4(M522_BUBBLE *self, M522_VEC3 *pos, s32 id)
{
    MGACTOR_PARAM param;
    M522_MODEL *model;
    M522_MODEL *modelCopy;
    M522_MODEL *layerModel;
    M522_MODEL *attrModel;
    s32 actorId;

    actorId = id;
    fn_1_E44C(&self->base);
    self->base.vtbl = lbl_1_data_120;
    self->hook = lbl_1_data_114;
    self->light = Hu3DGLightCreate(pos->x, pos->y, pos->z, 0.0f, 0.0f, 1.0f, 0xFF, 0xA0, 0);
    model = (M522_MODEL *)fn_1_689C(sizeof(M522_MODEL));
    if (model != 0) {
        model = fn_1_9A34(model, 0x50000C);
    }
    self->model = model;
    self->dead = 0;
    self->inside = 1;
    self->scale = 1.0f;
    self->alpha = 0;
    self->life = 0;
    self->frame = 0;
    fn_1_9DEC(&self->base.hook, 6);
    Hu3DGLightStaticSet(self->light, 1);
    Hu3DGLightPointSet(self->light, 20.0f, 0.99f, GX_DA_STEEP);
    param = lbl_1_rodata_198;
    param.param = actorId;
    modelCopy = self->model;
    self->actor = MgActorCreate(&param, modelCopy->id);
    MgActorColAttrSet(self->actor, 0x100);
    self->actor->gravity = 75.0f;
    layerModel = self->model;
    fn_1_9DEC(layerModel, 6);
    attrModel = self->model;
    fn_1_9BA8(attrModel, 0x40000001);
    self->home.x = pos->x;
    self->home.y = pos->y;
    self->home.z = pos->z;
    fn_1_52FC(self);
    self->actor->rotY = 0.0f;
    return self;
}

void *fn_1_526C(M522_BUBBLE *self, s16 flag)
{
    if (self != 0) {
        self->base.vtbl = lbl_1_data_120;
        MgActorKill(self->actor);
        Hu3DGLightKill(self->light);
        if (&self->model != 0) {
            fn_1_9E94(self->model, 1);
        }
        fn_1_E534(&self->base, 0);
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

void fn_1_52FC(M522_BUBBLE *self)
{
    MgActorPosUpdate(self->actor, (HuVecF *)&self->home);
    self->life = (s32)(600.0 * fn_1_BABC());
}

void fn_1_5354(M522_BUBBLE *self)
{
    f32 fall;
    f32 clamped;
    f32 posY;
    M522_VEC3 pos;
    M522_VEC3 sound;
    u8 hit;

#ifdef TARGET_PC
    if (self->hook.f_data.f_addr != 0) {
        ((void (*)(M522_BUBBLE *))self->hook.f_data.f_addr)(self);
    }
#else
    __ptmf_scall(self);
#endif
    if (self->dead != 0) {
        if (self != 0) {
            ((void (*)(M522_BUBBLE *, s32))self->base.vtbl[2])(self, 1);
        }
        return;
    }
    fn_1_577C(self);
    posY = self->actor->pos.y;
    if (lbl_1_bss_18 != 0) {
    } else {
        fn_1_6A70(lbl_1_data_104, lbl_1_data_F8, 0x27);
    }
    fall = (posY - fn_1_F1B8(lbl_1_bss_18)) / 1000.0f;
    if (fall < 0.0f) {
        clamped = 0.0f;
    } else if (fall > 1.0f) {
        clamped = 1.0f;
    } else {
        clamped = fall;
    }
    self->alpha = (u8)(255.0 * (1.0 - (f64)clamped));
    hit = fn_1_58DC(self);
    if (self->inside == 0 && hit != 0) {
        fn_1_58FC(self, 0x40);
        self->scale = 0.6f;
        MgActorPosGet(self->actor, (HuVecF *)&pos);
        sound.x = pos.x;
        sound.y = pos.y;
        sound.z = pos.z;
        HuAudFXPlayPan(0x745, fn_1_F564(&sound));
    }
    self->inside = hit;
    self->frame = self->frame + 1;
}

void fn_1_557C(M522_BUBBLE *self)
{
    M522_VEC3 pos;

    if (lbl_1_bss_18 != 0) {
    } else {
        fn_1_6A70(lbl_1_data_104, lbl_1_data_F8, 0x27);
    }
    if (fn_1_F1F8(lbl_1_bss_18) <= 0) {
        ((void (*)(M522_BUBBLE *))self->base.vtbl[5])(self);
    }
    if (fn_1_58DC(self) != 0) {
        if (lbl_1_bss_18 != 0) {
        } else {
            fn_1_6A70(lbl_1_data_104, lbl_1_data_F8, 0x27);
        }
        if (fn_1_F194(lbl_1_bss_18) != 0) {
            MgActorColAttrReset(self->actor, 4);
        } else {
            MgActorColAttrSet(self->actor, 4);
        }
    } else {
        MgActorColAttrSet(self->actor, 4);
    }
    MgActorPosGet(self->actor, (HuVecF *)&pos);
    if (pos.x > 1000.0f) {
        MgActorPosUpdate(self->actor, (HuVecF *)&self->home);
        self->life = (s32)(600.0 * fn_1_BABC());
    }
    if (self->scale < 1.0f) {
        self->scale = (f32)((f64)self->scale + 0.02);
        Hu3DModelScaleSet(self->actor->mdlId, 1.0f, self->scale, 1.0f);
    }
}

void fn_1_5748(M522_BUBBLE *self)
{
    MgActorColAttrSet(self->actor, 4);
}

void fn_1_577C(M522_BUBBLE *self)
{
    M522_VEC3 pos;
    M522_VEC3 lightPos;
    Vec camTarget;
    Vec camPos;
    Vec camUp;
    Vec *dir;
    Vec *dirCopy;
    s16 light;

    MgActorPosGet(self->actor, (HuVecF *)&pos);
    lightPos.x = pos.x;
    lightPos.y = pos.y;
    lightPos.z = pos.z;
    Hu3DCameraPosGet(1, &camPos, &camUp, &camTarget);
    PSVECSubtract(&camPos, &camTarget, &camTarget);
    dir = &camTarget;
    dirCopy = dir;
    PSVECNormalize(dirCopy, dirCopy);
    light = self->light;
    Hu3DGLightPosSetV(light, (HuVecF *)&lightPos, (HuVecF *)&camTarget);
}

void fn_1_5824(M522_BUBBLE *self)
{
    M522_VEC3 pos;

    if (fn_1_58DC(self) != 0) {
        Hu3DModelScaleSet(self->actor->mdlId, self->scale, self->scale, self->scale);
        MgActorPosGet(self->actor, (HuVecF *)&pos);
        self->scale = (f32)((f64)self->scale - 0.016666666666666666);
        if (self->scale <= 0.0f || pos.x > 1000.0f) {
            self->dead = 1;
        }
    }
}

u8 fn_1_58DC(M522_BUBBLE *self)
{
    return (u8)((self->actor->colGroundAttr & 0x407F) != 0);
}

void fn_1_58FC(M522_BUBBLE *self, s32 count)
{
    M522_VEC3 pos;
    s32 n;
    M522_BURSTOBJ *burst;

    n = count;
    pos.x = self->actor->pos.x;
    pos.y = self->actor->pos.y;
    pos.z = self->actor->pos.z;
    burst = (M522_BURSTOBJ *)fn_1_689C(sizeof(M522_BURSTOBJ));
    if (burst != 0) {
        fn_1_17E8(burst, &pos, n, 200.0f, 2.0f);
    }
}

void fn_1_5998(M522_BUBBLE *self, Mtx *camera)
{
    Mtx *cam;

    cam = camera;
    if (self->scale <= 0.0f || self->alpha == 0) {
        return;
    }
    fn_1_5A04(self, cam);
    fn_1_5CF8();
}

void fn_1_5A04(M522_BUBBLE *self, Mtx *camera)
{
    Mtx mtx;
    M522_VEC3 pos;
    GXColor color;
    GXColor tevColor;
    f32 size;
    Mtx *cam;

    cam = camera;
    MgActorPosGet(self->actor, (HuVecF *)&pos);
    size = (f32)((f64)self->scale * (60.0 + (5.0 * sin((6.283185307179586 * (f64)self->frame) / 48.0))));
    PSMTXScale(mtx, size, 1.0f, size);
    if (lbl_1_bss_18 != 0) {
    } else {
        fn_1_6A70(lbl_1_data_104, lbl_1_data_F8, 0x27);
    }
    PSMTXTransApply(mtx, mtx, pos.x, (f32)(0.5 + fn_1_F1B8(lbl_1_bss_18)), pos.z);
    PSMTXConcat(*cam, mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(0);
    color = lbl_1_rodata_210;
    color.a = self->alpha;
    tevColor = lbl_1_rodata_210;
    tevColor.a = self->alpha;
    GXSetTevColor(GX_TEVREG0, tevColor);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetCullMode(GX_CULL_NONE);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZCompLoc(GX_TRUE);
    GXSetZMode(GX_TRUE, GX_LESS, GX_FALSE);
    GXSetAlphaCompare(GX_ALWAYS, 0xFF, GX_AOP_OR, GX_ALWAYS, 0xFF);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
}

void fn_1_5CF8(void)
{
    GXClearVtxDesc();
    GXSetVtxDescv((GXVtxDescList *)lbl_1_rodata_248);
    GXSetVtxAttrFmtv(GX_VTXFMT0, lbl_1_rodata_260);
    GXSETARRAY(GX_VA_POS, (void *)lbl_1_rodata_2A0, sizeof(lbl_1_rodata_2A0), 3, FALSE);
    GXSETARRAY(GX_VA_CLR0, (void *)lbl_1_rodata_2E0, sizeof(lbl_1_rodata_2E0), 2, FALSE);
    GXCallDisplayList((void *)lbl_1_rodata_300, 0x20);
}

M522_VEC3 *fn_1_5D6C(M522_POSVEC *self, u32 index)
{
    if (index >= self->m_size) {
        fn_1_6AD0(M522_STDERR, "vector::at index out of range");
        abort();
    }
    return self->m_data + index;
}

#define M522_ERASE_TYPE s32 *
#define M522_ERASE_RETURN return pos;
#define M522_FN fn_1_5DE4
#define M522_VEC M522_HANDLES
#define M522_T s32
#include "REL/m522vec_erase.inc"
#undef M522_FN
#undef M522_VEC
#undef M522_T
#undef M522_ERASE_TYPE
#undef M522_ERASE_RETURN

#define M522_FN fn_1_5E74
#define M522_VEC M522_HANDLES
#define M522_T s32
#include "REL/m522vec_insert_pod.inc"
#undef M522_FN
#undef M522_VEC
#undef M522_T

#define M522_ERASE_TYPE M522_VEC3 *
#define M522_ERASE_RETURN return pos;
#define M522_FN fn_1_622C
#define M522_VEC M522_POSVEC
#define M522_T M522_VEC3
#include "REL/m522vec_erase.inc"
#undef M522_FN
#undef M522_VEC
#undef M522_T
#undef M522_ERASE_TYPE
#undef M522_ERASE_RETURN

#define M522_FN fn_1_62EC
#define M522_VEC M522_POSVEC
#define M522_T M522_VEC3
#define M522_TMP_DTOR(t) fn_1_412C(&t); if (t.m_data != 0) { fn_1_6964(t.m_data); }
#include "REL/m522vec_insert.inc"
#undef M522_FN
#undef M522_VEC
#undef M522_T
#undef M522_TMP_DTOR

}
