#include "REL/m537Dll.h"
#include <math.h>
#include "game/gxarray.h"

extern "C" {

extern void *lbl_1_data_1FC[];

f32 fn_1_4F90(f32 scale);
f32 fn_1_4FC8(f32 lo, f32 hi);
void DCFlushRange(void *addr, u32 nBytes);

void fn_1_12770(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx);
void fn_1_12C3C(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx);
void fn_1_12E7C(M537Ripple *self, Mtx mtx);
void fn_1_134D8(HU3DMODEL *model, Mtx *mtx);
void fn_1_169F8(HU3DMODEL *model, Mtx *mtx);
void fn_1_16E5C(M537Wake *wake, Mtx mtx);
void fn_1_13A44(M537Water *self);
void fn_1_13AC8(M537Water *self);
void fn_1_153D4(M537Water *self);
void fn_1_14E20(M537Water *self, Mtx mtx, HSFOBJECT *obj);
void fn_1_15A48(M537Water *self, Mtx mtx);
void fn_1_162AC(M537Water *self);
M537Wake *fn_1_16A3C(M537Wake *self, Vec *pos);
void fn_1_16BF0(M537Wake *self);
M537Spray *fn_1_126C8(M537Spray *self, s16 freeIt);
void fn_1_12758(M537Spray *self, s32 bit);
M537Ripple *fn_1_12DD4(M537Ripple *self, s16 freeIt);
void fn_1_12E64(M537Ripple *self, s32 bit);
M537Splash *fn_1_133A8(M537Splash *self, s16 freeIt);
void fn_1_13434(M537Splash *self);
void fn_1_134C0(M537Splash *self, s32 bit);
M537Water *fn_1_1393C(M537Water *self, s16 freeIt);
void fn_1_1557C(M537Water *self, s32 bit);
void fn_1_15594(M537Water *self);
M537Wake *fn_1_16B48(M537Wake *self, s16 freeIt);
void fn_1_16BD8(M537Wake *self, s32 bit);
void fn_1_1DDC(void);

#define M537_MDL(self) ((self) != 0 ? &(self)->mdl : (s16 *)0)

s32 lbl_1_bss_A98;
u8 lbl_1_bss_A9C;

#ifdef __MWERKS__
#define M537_RODATA __declspec(section ".rodata")
#else
#define M537_RODATA
#endif

M537_RODATA const Vec lbl_1_rodata_670[3] = {
    { -1600.0f, 0.0f, 650.0f },
    { 0.0f, 0.0f, -1600.0f },
    { 1400.0f, 0.0f, 0.0f },
};
M537_RODATA const Vec lbl_1_rodata_694[3] = {
    { 0.0f, 120.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 90.0f, 0.0f },
};
M537_RODATA const f32 lbl_1_rodata_718[2][3] = { { 0.2f, 0.0f, 0.0f }, { 0.0f, 0.2f, 0.0f } };
M537_RODATA const f32 lbl_1_rodata_730[2][3] = { { 0.1f, 0.0f, 0.0f }, { 0.0f, 0.1f, 0.0f } };

char lbl_1_data_6AC[] = "taki_a";
char lbl_1_data_6B3[] = "taki_b";
char lbl_1_data_6BA[] = "taki_c";
char *lbl_1_data_6C4[3] = { lbl_1_data_6AC, lbl_1_data_6B3, lbl_1_data_6BA };

void *lbl_1_data_6D0[5] = { 0, 0, (void *)fn_1_16B48, (void *)fn_1_16BD8, (void *)fn_1_16BF0 };
void *lbl_1_data_6E4[5] = { 0, 0, (void *)fn_1_1393C, (void *)fn_1_1557C, (void *)fn_1_15594 };
void *lbl_1_data_6F8[5] = { 0, 0, (void *)fn_1_133A8, (void *)fn_1_134C0, (void *)fn_1_13434 };
void *lbl_1_data_70C[5] = { 0, 0, (void *)fn_1_12DD4, (void *)fn_1_12E64, (void *)fn_1_1DDC };
void *lbl_1_data_720[5] = { 0, 0, (void *)fn_1_126C8, (void *)fn_1_12758, (void *)fn_1_1DDC };

// Spray emitter: a particle model rotated/placed at the water fall.
M537Spray *fn_1_125AC(M537Spray *self, Vec *pos, Vec *rot)
{
    s16 id;

    fn_1_1264(self);
    self->vptr = lbl_1_data_1FC;
    self->flag = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    self->vptr = lbl_1_data_720;
    self->anim = HuSprAnimRead(HuDataSelHeapReadNum(0x5F0018, 0x10000000, HEAP_MODEL));
    HuSprAnimLock(self->anim);
    id = Hu3DParticleCreate(self->anim, 0x20);
    Hu3DParticleHookSet(id, fn_1_12770);
    Hu3DParticleColSet(id, 0xFF, 0xFF, 0xFF);
    Hu3DParticleBlendModeSet(id, 0);
    Hu3DModelLayerSet(id, 4);
    Hu3DModelPosSet(id, pos->x, pos->y, pos->z);
    Hu3DModelRotSet(id, rot->x, rot->y, rot->z);
    self->mdl = id;
    return self;
}

M537Spray *fn_1_126C8(M537Spray *self, s16 freeIt)
{
    if (self != 0) {
        self->vptr = lbl_1_data_720;
        HuSprAnimKill(self->anim);
        Hu3DModelKill(self->mdl);
        if (self != 0) {
            self->vptr = lbl_1_data_1FC;
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

void fn_1_12758(M537Spray *self, s32 bit)
{
    self->flag &= ~(1 << bit);
}

// Spray particle update: launches the drops on the first frame, then moves and fades them.
void fn_1_12770(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx)
{
    HU3DPARTICLEDATA *d;
    f32 oldVy;
    f32 ang;
    f32 speed;
    s32 i;

    if (particle->count == 0) {
        d = particle->data;
        for (i = 0; i < particle->maxCnt; i++) {
            ang = 360.0f * (f32)i;
            d->vel.x = (f32)sin(0.017453292f * (ang / (f32)particle->maxCnt));
            speed = 0.27240694f * fn_1_4FC8(10.0f, 40.0f);
            d->accel.y = speed;
            d->vel.y = speed;
            ang = 360.0f * (f32)i;
            d->vel.z = (f32)cos(0.017453292f * (ang / (f32)particle->maxCnt));
            d->scale = 200.0f;
            d->pos.x = (f32)(100.0f * fn_1_4FC8(-3.2f, 3.2f));
            d->pos.y = 0.0f;
            d->pos.z = (f32)(100.0f * fn_1_4FC8(-0.32f, 0.32f));
            d->color.a = 0;
            d->time = 0;
            d->parManId = (fn_1_4AE0() & 0xF) + 0xF;
            d->zRot = (f32)(0.017453292f * fn_1_4F90(360.0f));
            d++;
        }
    }
    d = particle->data;
    for (i = 0; i < particle->maxCnt; i++) {
        if (d->time == 0) {
            d->color.a = (u8)(d->color.a + d->parManId);
            if (d->color.a >= 0xDC) {
                d->color.a = 0xDC;
                d->time = (s16)(d->time + 1);
            }
        } else if (d->color.a > 0x20) {
            d->color.a = (u8)(d->color.a - (s32)(0.5f * (f32)d->parManId));
        } else {
            d->pos.x = (f32)(100.0f * fn_1_4FC8(-3.2f, 3.2f));
            d->pos.y = 0.0f;
            d->pos.z = (f32)(100.0f * fn_1_4FC8(-0.32f, 0.32f));
            d->vel.y = d->accel.y;
            d->scale = 200.0f;
            d->color.a = 0;
            d->parManId = (fn_1_4AE0() & 0xF) + 0xF;
            d->time = 0;
        }
        d->scale = (f32)((f64)d->scale + 3.3333333333333335);
        oldVy = d->vel.y;
        d->vel.y = (f32)(d->vel.y + -0.27240694f);
        d->pos.x = (f32)((f64)d->pos.x + (0.016666666666666666 * (f64)(100.0f * (2.0f * d->vel.x))));
        d->pos.y = (f32)(d->pos.y + (0.5f * (oldVy + d->vel.y)));
        d->pos.z = (f32)((f64)d->pos.z + (0.016666666666666666 * (f64)(100.0f * (2.0f * d->vel.z))));
        d++;
    }
}

void fn_1_12C3C(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx)
{
    Mtx *m;

    m = (Mtx *)mtx;
    fn_1_12E7C((M537Ripple *)particle->work, mtx);
}

// Ripple ring: a particle model that carries a pointer back to its owner.
M537Ripple *fn_1_12C80(M537Ripple *self, Vec *pos)
{
    s16 id;

    fn_1_1264(self);
    self->vptr = lbl_1_data_1FC;
    self->flag = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    self->vptr = lbl_1_data_70C;
    self->anim = HuSprAnimRead(HuDataSelHeapReadNum(0x5F0017, 0x10000000, HEAP_MODEL));
    HuSprAnimLock(self->anim);
    id = Hu3DParticleCreate(self->anim, 0x20);
    self->particle = (HU3DPARTICLE *)Hu3DData[id].hookData;
    self->particle->work = self;
    Hu3DParticleHookSet(id, fn_1_12C3C);
    Hu3DParticleColSet(id, 0xFF, 0xFF, 0xFF);
    Hu3DParticleBlendModeSet(id, 0);
    Hu3DModelLayerSet(id, 4);
    Hu3DModelPosSet(id, pos->x, pos->y, pos->z);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    self->mdl = id;
    return self;
}

M537Ripple *fn_1_12DD4(M537Ripple *self, s16 freeIt)
{
    if (self != 0) {
        self->vptr = lbl_1_data_70C;
        HuSprAnimKill(self->anim);
        Hu3DModelKill(self->mdl);
        if (self != 0) {
            self->vptr = lbl_1_data_1FC;
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

void fn_1_12E64(M537Ripple *self, s32 bit)
{
    self->flag &= ~(1 << bit);
}

// Ripple update: expands and fades the ring; the object removes itself when it is gone.
void fn_1_12E7C(M537Ripple *self, Mtx mtx)
{
    HU3DMODEL *model;
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *d;
    f32 oldVy;
    f32 ang;
    s32 alive;
    s32 i;

    model = &Hu3DData[self->mdl];
    particle = self->particle;
    if (particle->count == 0) {
        d = particle->data;
        for (i = 0; i < particle->maxCnt; i++) {
            ang = 360.0f * (f32)i;
            d->vel.x = (f32)sin(0.017453292f * (ang / (f32)particle->maxCnt));
            d->vel.y = (f32)(0.27240694f * fn_1_4FC8(20.0f, 80.0f));
            ang = 360.0f * (f32)i;
            d->vel.z = (f32)cos(0.017453292f * (ang / (f32)particle->maxCnt));
            d->scale = 200.0f;
            d->pos.x = (f32)(0.5f * (100.0f * d->vel.x));
            d->pos.y = 0.0f;
            d->pos.z = (f32)(0.5f * (100.0f * d->vel.z));
            d->color.a = 0;
            d->time = 0;
            d->parManId = 0;
            d->zRot = (f32)(0.017453292f * fn_1_4F90(360.0f));
            d++;
        }
    }
    d = particle->data;
    alive = 0;
    for (i = 0; i < particle->maxCnt; i++) {
        if (d->time != 2) {
            if (d->time == 0) {
                d->color.a = (u8)(d->color.a + 0x10);
                if (d->color.a >= 0xC8) {
                    d->color.a = 0xC8;
                    d->time = (s16)(d->time + 1);
                }
            } else if (d->color.a != 0) {
                d->color.a = (u8)(d->color.a - 4);
            } else {
                d->time = (s16)(d->time + 1);
            }
            alive += 1;
            d->scale = (f32)((f64)d->scale + 1.6666666666666667);
            oldVy = d->vel.y;
            d->vel.y = (f32)(d->vel.y + -0.27240694f);
            d->pos.x = (f32)((f64)d->pos.x + (0.016666666666666666 * (f64)(100.0f * (2.0f * d->vel.x))));
            d->pos.y = (f32)(d->pos.y + (0.5f * (oldVy + d->vel.y)));
            d->pos.z = (f32)((f64)d->pos.z + (0.016666666666666666 * (f64)(100.0f * (2.0f * d->vel.z))));
        }
        d++;
    }
    if (alive == 0 && self != 0) {
        ((void *(*)(void *, s16))self->vptr[2])(self, 1);
    }
}

// Splash column: a model that deletes itself when its motion ends.
M537Splash *fn_1_13294(M537Splash *self, Vec *pos)
{
    s16 *m;
    s16 id;

    fn_1_1264(self);
    self->vptr = lbl_1_data_1FC;
    self->flag = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    fn_1_2C94(&self->mdl, 0x5F0013);
    self->vptr = lbl_1_data_6F8;
    m = M537_MDL(self);
    id = *m;
    Hu3DModelLayerSet(id, 4);
    Hu3DModelLightInfoSet(id, 0);
    Hu3DModelPosSet(id, pos->x, 0.0f, pos->z);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    HuAudFXPlay(0x7DB);
    return self;
}

M537Splash *fn_1_133A8(M537Splash *self, s16 freeIt)
{
    if (self != 0) {
        self->vptr = lbl_1_data_6F8;
        fn_1_30F4(&self->mdl, 0);
        if (self != 0) {
            self->vptr = lbl_1_data_1FC;
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

void fn_1_13434(M537Splash *self)
{
    s16 *m;

    m = M537_MDL(self);
    if (Hu3DMotionEndCheck(*m) != 0 && self != 0) {
        ((void *(*)(void *, s16))self->vptr[2])(self, 1);
    }
}

void fn_1_134C0(M537Splash *self, s32 bit)
{
    self->flag &= ~(1 << bit);
}

void fn_1_134D8(HU3DMODEL *model, Mtx *mtx)
{
    Mtx *m;
    M537Water *w;

    m = mtx;
    w = lbl_1_bss_9F0;
    fn_1_15A48(w, *mtx);
}

// Water constructor: textures, hook model, the three waterfall sprays and the grid.
M537Water *fn_1_13524(M537Water *self, s16 id)
{
    M537Water *ms;
    M537Spray *spray;
    s16 hook;
    s16 mid;
    s32 i;

    fn_1_1264(self);
    self->vptr = lbl_1_data_1FC;
    self->flag = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    if (lbl_1_bss_9F0 == 0) {
    } else {
        fn_1_1D4("ms_pointer == 0", "singleton.h", 0x35);
    }
    ms = (M537Water *)((u8 *)self + 0x10);
    if (ms != 0) {
        ms = (M537Water *)((u8 *)ms - 0x10);
    }
    lbl_1_bss_9F0 = ms;
    self->vptr = lbl_1_data_6E4;
    self->id = id;
    self->time = 0.0f;
    self->seId = HuAudFXPlay(0x7D8);
    hook = Hu3DHookFuncCreate((HU3DMODELHOOK)fn_1_134D8);
    Hu3DModelLayerSet(hook, 3);
    Hu3DModelCameraSet(hook, 1);
    self->anim[0] = HuSprAnimRead(HuDataSelHeapReadNum(0x5F0016, 0x10000000, HEAP_MODEL));
    HuSprAnimLock(self->anim[0]);
    self->anim[1] = HuSprAnimRead(HuDataSelHeapReadNum(0x5F0015, 0x10000000, HEAP_MODEL));
    HuSprAnimLock(self->anim[1]);
    self->anim[2] = HuSprAnimRead(HuDataSelHeapReadNum(0x5F0014, 0x10000000, HEAP_MODEL));
    HuSprAnimLock(self->anim[2]);
    mid = Hu3DModelCreate(HuDataSelHeapReadNum(0x5F0012, 0x10000000, HEAP_MODEL));
    self->mdl = mid;
    self->texOfs.x = 0.0f;
    self->texOfs.y = 0.0f;
    self->texOfs.z = 0.0f;
    fn_1_13A44(self);
    for (i = 0; i < 3; i++) {
#ifdef TARGET_PC
        spray = (M537Spray *)fn_1_0(sizeof(M537Spray));
#else
        spray = (M537Spray *)fn_1_0(0x18);
#endif
        if (spray != 0) {
            fn_1_125AC(spray, (Vec *)&lbl_1_rodata_670[i], (Vec *)&lbl_1_rodata_694[i]);
        }
    }
    fn_1_13AC8(self);
    fn_1_153D4(self);
    return self;
}

M537Water *fn_1_138B0(M537Water *self, s16 freeIt)
{
    if (self != 0) {
        if (lbl_1_bss_9F0 != 0) {
        } else {
            fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_9F0 = 0;
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M537Water *fn_1_1393C(M537Water *self, s16 freeIt)
{
    if (self != 0) {
        self->vptr = lbl_1_data_6E4;
        HuSprAnimKill(self->anim[0]);
        HuSprAnimKill(self->anim[1]);
        HuSprAnimKill(self->anim[2]);
        Hu3DModelKill(self->mdl);
        HuAudFXStop(self->seId);
        if ((u8 *)self + 0x10 != 0) {
            if (lbl_1_bss_9F0 != 0) {
            } else {
                fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
            }
            lbl_1_bss_9F0 = 0;
        }
        if (self != 0) {
            self->vptr = lbl_1_data_1FC;
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

// Looks up the three fall objects in the water model.
void fn_1_13A44(M537Water *self)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        self->obj[i] = Hu3DModelObjPtrGet(self->mdl, lbl_1_data_6C4[i]);
    }
    Hu3DModelAttrSet(self->mdl, 1);
}

// Initialises the grid positions, texture coordinates and colours.
void fn_1_13AC8(M537Water *self)
{
    s32 i;
    s32 j;
    Vec *pos;
    Vec *uv;
    u8 *col;

    for (i = 0; i < 0x20; i++) {
        for (j = 0; j < 0x20; j++) {
            pos = &self->pos[i][j];
            uv = &self->uv[i][j];
            col = self->col[i][j];
            pos->x = (200.0f * (f32)j) - 3200.0f;
            pos->y = 0.0f;
            pos->z = (200.0f * (f32)i) - 3200.0f;
            uv->x = 0.03125f * (f32)j;
            uv->z = 0.03125f * (f32)i;
            col[3] = 0;
            col[2] = 0;
            col[1] = 0;
            col[0] = 0;
            self->vel[i][j].x = 0.0f;
            self->vel[i][j].y = 0.0f;
            self->vel[i][j].z = 0.0f;
        }
    }
}

// One step of the spring simulation of the water grid.
void fn_1_13CFC(M537Water *self)
{
    Vec scaled[32][32];
    Vec acc;
    Vec d;
    Vec d2;
    Vec s;
    Vec *pos;
    s32 i;
    s32 j;
    s8 c0;
    s8 c3;
    f32 mag;
    f32 h;
    f32 dist;

    pos = &self->pos[0][0];
    for (i = 0; i < 0x20; i++) {
        for (j = 0; j < 0x20; j++) {
            scaled[i][j].x = pos->x;
            scaled[i][j].y = pos->y;
            scaled[i][j].z = pos->z;
            PSVECScale(&scaled[i][j], &scaled[i][j], 0.005f);
            pos++;
        }
    }
    for (i = 1; i < 0x1F; i++) {
        for (j = 1; j < 0x1F; j++) {
            acc.x = 0.0f;
            acc.y = 0.0f;
            acc.z = 0.0f;
            PSVECSubtract(&scaled[i][j], &scaled[i][j + 1], &d);
            mag = PSVECMag(&d);
            PSVECNormalize(&d, &d);
            PSVECScale(&d, &d2, 1.0f - mag);
            PSVECAdd(&acc, &d2, &acc);
            PSVECSubtract(&scaled[i][j], &scaled[i][j - 1], &d);
            mag = PSVECMag(&d);
            PSVECNormalize(&d, &d);
            PSVECScale(&d, &d2, 1.0f - mag);
            PSVECAdd(&acc, &d2, &acc);
            PSVECSubtract(&scaled[i][j], &scaled[i + 1][j], &d);
            mag = PSVECMag(&d);
            PSVECNormalize(&d, &d);
            PSVECScale(&d, &d2, 1.0f - mag);
            PSVECAdd(&acc, &d2, &acc);
            PSVECSubtract(&scaled[i][j], &scaled[i - 1][j], &d);
            mag = PSVECMag(&d);
            PSVECNormalize(&d, &d);
            PSVECScale(&d, &d2, 1.0f - mag);
            PSVECAdd(&acc, &d2, &acc);
            PSVECScale(&acc, &acc, 5.0f);
            PSVECAdd(&self->vel[i][j], &acc, &self->vel[i][j]);
        }
    }
    for (i = 1; i < 0x1F; i++) {
        for (j = 1; j < 0x1F; j++) {
            PSVECScale(&self->vel[i][j], &s, 5.0f);
            PSVECAdd(&self->pos[i][j], &s, &self->pos[i][j]);
            if (self->pos[i][j].y > 30.000002f) {
                self->pos[i][j].y = 30.000002f;
                self->vel[i][j].y = 0.0f;
            }
            if (self->pos[i][j].y < -150.0f) {
                self->pos[i][j].y = -150.0f;
                self->vel[i][j].y = 0.0f;
            }
            h = 150.0f + self->pos[i][j].y;
            c0 = (s8)(160.0f * (h / 180.0f));
            c3 = 0x82;
            dist = PSVECMag(&self->pos[i][j]);
            if (dist > 2400.0f) {
                c0 = 0;
            } else if (dist >= 2000.0f) {
                c0 = (s8)((f32)(u8)c0 * ((2400.0f - dist) / 400.0f));
            }
            if (dist > 3000.0f) {
                c3 = 0;
            } else if (dist >= 2000.0f) {
                c3 = (s8)((f32)0x82 * ((3000.0f - dist) / 1000.0f));
            }
            self->col[i][j][2] = c0;
            self->col[i][j][1] = c0;
            self->col[i][j][0] = c0;
            self->col[i][j][3] = c3;
        }
    }
}

// Adds an impulse to the heights of the 3x3 cells around a world position.
void fn_1_14414(M537Water *self, Vec *pos, f32 amount)
{
    s32 gx;
    s32 gz;
    s32 cx;
    s32 cz;
    s32 i;
    s32 j;

    gx = (s32)(16.0f + (f32)(s32)(pos->x / 200.0f));
    gz = (s32)(16.0f + (f32)(s32)(pos->z / 200.0f));
    if (gx < 1) {
        cx = 1;
    } else if (gx > 0x1E) {
        cx = 0x1E;
    } else {
        cx = gx;
    }
    if (gz < 1) {
        cz = 1;
    } else if (gz > 0x1E) {
        cz = 0x1E;
    } else {
        cz = gz;
    }
    for (i = -1; i <= 1; i++) {
        for (j = -1; j <= 1; j++) {
            self->vel[cz + i][cx + j].y = self->vel[cz + i][cx + j].y + amount;
        }
    }
}

// Average height of the 3x3 cells around a world position.
f32 fn_1_145A0(M537Water *self, Vec *pos)
{
    f32 sum;
    s32 gx;
    s32 gz;
    s32 cx;
    s32 cz;
    s32 i;
    s32 j;

    gx = (s32)(16.0f + (f32)(s32)(pos->x / 200.0f));
    gz = (s32)(16.0f + (f32)(s32)(pos->z / 200.0f));
    if (gx < 1) {
        cx = 1;
    } else if (gx > 0x1E) {
        cx = 0x1E;
    } else {
        cx = gx;
    }
    if (gz < 1) {
        cz = 1;
    } else if (gz > 0x1E) {
        cz = 0x1E;
    } else {
        cz = gz;
    }
    sum = 0.0f;
    for (i = -1; i <= 1; i++) {
        for (j = -1; j <= 1; j++) {
            sum += self->pos[cz + i][cx + j].y;
        }
    }
    return 15.000001f + (sum / 9.0f);
}

// Pushes the water down at a position and spawns the ripple, splash and wake objects.
void fn_1_14764(M537Water *self, Vec *pos, f32 amount)
{
    M537Ripple *ripple;
    M537Splash *splash;
    M537Wake *wake;

    fn_1_14414(self, pos, amount);
#ifdef TARGET_PC
    ripple = (M537Ripple *)fn_1_0(sizeof(M537Ripple));
#else
    ripple = (M537Ripple *)fn_1_0(0x1C);
#endif
    if (ripple != 0) {
        ripple = fn_1_12C80(ripple, pos);
    }
#ifdef TARGET_PC
    splash = (M537Splash *)fn_1_0(sizeof(M537Splash));
#else
    splash = (M537Splash *)fn_1_0(0x14);
#endif
    if (splash != 0) {
        splash = fn_1_13294(splash, pos);
    }
#ifdef TARGET_PC
    wake = (M537Wake *)fn_1_0(sizeof(M537Wake));
#else
    wake = (M537Wake *)fn_1_0(0x28);
#endif
    if (wake != 0) {
        fn_1_16A3C(wake, pos);
    }
}

// Rebuilds the 32x32 indirect (normal) texture from the grid heights.
void fn_1_14B8C(M537Water *self)
{
    s32 i;
    s32 j;
    s32 gx;
    s32 gz;
    s32 idx;
    f32 fx;
    f32 fz;

    for (i = 0; i < 0x20; i++) {
        for (j = 0; j < 0x20; j++) {
            gx = (s32)(j << 5) / 32;
            gz = (s32)(i << 5) / 32;
            fx = (200.0f * (f32)gx) - 3200.0f;
            fz = (200.0f * (f32)gz) - 3200.0f;
            idx = (((i >> 2) << 7) | (((i & 3) * 4) | ((j & 3) | ((j >> 2) * 0x10)))) * 2;
            self->ind[idx] = (s32)(127.0f * ((self->pos[gz][gx].x - fx) / 84.0f)) + 0x80;
            self->ind[idx + 1] = (u8)((s32)(127.0f * ((self->pos[gz][gx].z - fz) / 84.0f)) + 0x80);
        }
    }
    DCFlushRange(self->ind, 0x800);
}

void fn_1_14DB0(M537Water *self, Mtx mtx)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        fn_1_14E20(self, mtx, self->obj[i]);
    }
}

// Draws one indexed mesh of the water model.
void fn_1_14E20(M537Water *self, Mtx mtx, HSFOBJECT *obj)
{
    HSFBUFFER *faces;
    HSFBUFFER *verts;
    HSFBUFFER *colors;
    HSFBUFFER *sts;
    HSFFACE *face;
    HSFFACEINDEX *strip;
    u32 k;
    s32 i;

    faces = obj->mesh.face;
    verts = obj->mesh.vertex;
    colors = obj->mesh.color;
    sts = obj->mesh.st;
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSETARRAY(GX_VA_POS, verts->data, verts->count * 0xC, 0xC, TRUE);
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSETARRAY(GX_VA_CLR0, colors->data, colors->count * 4, 4, TRUE);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSETARRAY(GX_VA_TEX0, sts->data, sts->count * 8, 8, TRUE);
    face = (HSFFACE *)faces->data;
    for (i = 0; i < faces->count; i++) {
        switch (face->type & 7) {
        case 2:
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
            GXPosition1x16(face->index[0].vertex);
            GXColor1x16(face->index[0].color);
            GXTexCoord1x16(face->index[0].st);
            GXPosition1x16(face->index[2].vertex);
            GXColor1x16(face->index[2].color);
            GXTexCoord1x16(face->index[2].st);
            GXPosition1x16(face->index[1].vertex);
            GXColor1x16(face->index[1].color);
            GXTexCoord1x16(face->index[1].st);
#ifdef TARGET_PC
            GXEnd();
#endif
            break;
        case 3:
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition1x16(face->index[0].vertex);
            GXColor1x16(face->index[0].color);
            GXTexCoord1x16(face->index[0].st);
            GXPosition1x16(face->index[2].vertex);
            GXColor1x16(face->index[2].color);
            GXTexCoord1x16(face->index[2].st);
            GXPosition1x16(face->index[3].vertex);
            GXColor1x16(face->index[3].color);
            GXTexCoord1x16(face->index[3].st);
            GXPosition1x16(face->index[1].vertex);
            GXColor1x16(face->index[1].color);
            GXTexCoord1x16(face->index[1].st);
#ifdef TARGET_PC
            GXEnd();
#endif
            break;
        case 4:
            GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (u16)(face->strip.count + 3));
            GXPosition1x16(face->index[0].vertex);
            GXColor1x16(face->index[0].color);
            GXTexCoord1x16(face->index[0].st);
            GXPosition1x16(face->index[2].vertex);
            GXColor1x16(face->index[2].color);
            GXTexCoord1x16(face->index[2].st);
            GXPosition1x16(face->index[1].vertex);
            GXColor1x16(face->index[1].color);
            GXTexCoord1x16(face->index[1].st);
            strip = face->strip.data;
            for (k = 0; k < face->strip.count; k++) {
                GXPosition1x16(strip->vertex);
                GXColor1x16(strip->color);
                GXTexCoord1x16(strip->st);
                strip++;
            }
#ifdef TARGET_PC
            GXEnd();
#endif
            break;
        }
        face++;
    }
}

// Copies the frame buffer into the reflection texture and binds the water textures.
void fn_1_15300(M537Water *self)
{
    GXSetTexCopySrc(0, 0, 0x280, 0x1E0);
    GXSetTexCopyDst(0x280, 0x1E0, GX_TF_RGB565, GX_FALSE);
    GXCopyTex(self->img, GX_FALSE);
    GXLoadTexObj(&self->tex0, GX_TEXMAP0);
    GXLoadTexObj(&self->tex1, GX_TEXMAP1);
    HuSprTexLoad(self->anim[0], 0, GX_TEXMAP2, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    HuSprTexLoad(self->anim[1], 0, GX_TEXMAP3, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    HuSprTexLoad(self->anim[2], 0, GX_TEXMAP4, GX_REPEAT, GX_REPEAT, GX_LINEAR);
}

void fn_1_153D4(M537Water *self)
{
    GXInitTexObj(&self->tex0, self->img, 0x280, 0x1E0, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&self->tex0, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXInitTexObj(&self->tex1, self->ind, 0x20, 0x20, GX_TF_IA8, GX_REPEAT, GX_REPEAT, GX_FALSE);
    GXInitTexObjLOD(&self->tex1, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
}

void fn_1_154C8(Mtx mtx)
{
    Mtx m;
    Mtx r;
    Mtx *p;

    p = (Mtx *)mtx;
    GXSetNumTexGens(1);
    C_MTXLightPerspective(m, Hu3DCamera->fov, Hu3DCamera->aspect, 0.5f, -0.5f, 0.5f, 0.5f);
    PSMTXConcat(m, mtx, r);
    GXLoadTexMtxImm(r, GX_TEXMTX9, GX_MTX3x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX9, GX_FALSE, GX_PTIDENTITY);
}

void fn_1_1557C(M537Water *self, s32 bit)
{
    self->flag &= ~(1 << bit);
}

// Per-frame update: advances the simulation and refreshes the indirect texture.
void fn_1_15594(M537Water *self)
{
    s32 i;

    self->time = (f32)((f64)self->time + 0.016666666666666666);
    self->texOfs.y = 0.5f * -self->time;
    fn_1_13CFC(self);
    fn_1_14B8C(self);
    if ((s8)lbl_1_bss_A9C == 0) {
        lbl_1_bss_A98 = 0;
        lbl_1_bss_A9C = 1;
    }
    if (lbl_1_bss_A98 % 240 == 0) {
        for (i = 0; i < 3; i++) {
            fn_1_14414(self, (Vec *)&lbl_1_rodata_670[i], -0.5f);
        }
    }
    lbl_1_bss_A98 += 1;
}

// Draw hook of the water: reflection copy, indirect warp passes over the three meshes, grid.
void fn_1_15A48(M537Water *self, Mtx mtx)
{
    Mtx proj;
    Mtx tex;
    Mtx tr;
    Mtx sc;
    Mtx cc;
    f32 a;
    f32 b;
    s32 i;

    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetNumTexGens(1);
    C_MTXLightPerspective(proj, Hu3DCamera->fov, Hu3DCamera->aspect, 0.5f, -0.5f, 0.5f, 0.5f);
    PSMTXConcat(proj, mtx, tex);
    GXLoadTexMtxImm(tex, GX_TEXMTX9, GX_MTX3x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX9, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCopySrc(0, 0, 0x280, 0x1E0);
    GXSetTexCopyDst(0x280, 0x1E0, GX_TF_RGB565, GX_FALSE);
    GXCopyTex(self->img, GX_FALSE);
    GXLoadTexObj(&self->tex0, GX_TEXMAP0);
    GXLoadTexObj(&self->tex1, GX_TEXMAP1);
    HuSprTexLoad(self->anim[0], 0, GX_TEXMAP2, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    HuSprTexLoad(self->anim[1], 0, GX_TEXMAP3, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    HuSprTexLoad(self->anim[2], 0, GX_TEXMAP4, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(3);
    PSMTXTrans(tr, self->texOfs.x, 2.0f * self->texOfs.y, self->texOfs.z);
    PSMTXScale(sc, 1.0f, 1.0f, 1.0f);
    PSMTXConcat(sc, tr, cc);
    GXLoadTexMtxImm(cc, GX_TEXMTX3, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX3, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP4, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_TEXA, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_A0, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumIndStages(1);
    PSMTXTrans(tr, self->texOfs.x, self->texOfs.y, self->texOfs.z);
    PSMTXScale(sc, 1.0f, 1.0f, 1.0f);
    PSMTXConcat(sc, tr, cc);
    GXLoadTexMtxImm(cc, GX_TEXMTX0, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD2, GX_TEXMAP2);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_0);
    GXSetIndTexMtx(GX_ITM_0, (f32(*)[3])lbl_1_rodata_730, 0);
    for (i = 0; i < 3; i++) {
        fn_1_14E20(self, mtx, self->obj[i]);
    }
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP3, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_TEXA, GX_CC_ONE);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumTexGens(3);
    b = 0.017453292f * (20.0f * self->time);
    a = (f32)(0.5 * sin(0.017453292f * (10.0f * self->time)));
    PSMTXTrans(tr, 0.0f, a, (f32)(0.5 * sin(b)));
    PSMTXScale(sc, 1.0f, 1.0f, 1.0f);
    PSMTXConcat(sc, tr, cc);
    GXLoadTexMtxImm(cc, GX_TEXMTX6, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_TEXMTX6, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD2, GX_TEXMAP1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_1);
    GXSetIndTexMtx(GX_ITM_1, (f32(*)[3])lbl_1_rodata_718, 0);
    GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_ZERO, GX_LO_COPY);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    fn_1_162AC(self);
    GXSetNumIndStages(0);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetTevDirect(GX_TEVSTAGE2);
    GXSetTexCoordScaleManually(GX_TEXCOORD0, GX_FALSE, 0, 0);
    GXSetTexCoordScaleManually(GX_TEXCOORD1, GX_FALSE, 0, 0);
    GXSetTexCoordScaleManually(GX_TEXCOORD2, GX_FALSE, 0, 0);
}

// Draws the water grid as 31x31 quads with two texture coordinate sets.
void fn_1_162AC(M537Water *self)
{
    Vec *pos;
    Vec *uv;
    u8 (*col)[4];
    s32 i;
    s32 j;
    Vec *p00;
    Vec *p01;
    Vec *p11;
    Vec *p10;
    u8 *c00;
    u8 *c01;
    u8 *c11;
    u8 *c10;
    Vec *t00;
    Vec *t01;
    Vec *t11;
    Vec *t10;

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0);
    pos = &self->pos[0][0];
    uv = &self->uv[0][0];
    col = &self->col[0][0];
    GXBegin(GX_QUADS, GX_VTXFMT0, 0xF04);
    for (i = 0; i < 0x1F; i++) {
        for (j = 0; j < 0x1F; j++) {
            p00 = pos + (i * 32 + j);
            p01 = pos + (i * 32 + (j + 1));
            p11 = pos + ((i + 1) * 32 + (j + 1));
            p10 = pos + ((i + 1) * 32 + j);
            c00 = (u8 *)(col + (i * 32 + j));
            c01 = (u8 *)(col + (i * 32 + (j + 1)));
            c11 = (u8 *)(col + ((i + 1) * 32 + (j + 1)));
            c10 = (u8 *)(col + ((i + 1) * 32 + j));
            t00 = uv + (i * 32 + j);
            t01 = uv + (i * 32 + (j + 1));
            t11 = uv + ((i + 1) * 32 + (j + 1));
            t10 = uv + ((i + 1) * 32 + j);
            GXPosition3f32(p00->x, p00->y, p00->z);
            GXColor4u8(c00[0], c00[1], c00[2], c00[3]);
            GXTexCoord2f32(t00->x, t00->z);
            GXTexCoord2f32(8.0f * t00->x, 8.0f * t00->z);
            GXPosition3f32(p01->x, p01->y, p01->z);
            GXColor4u8(c01[0], c01[1], c01[2], c01[3]);
            GXTexCoord2f32(t01->x, t01->z);
            GXTexCoord2f32(8.0f * t01->x, 8.0f * t01->z);
            GXPosition3f32(p11->x, p11->y, p11->z);
            GXColor4u8(c11[0], c11[1], c11[2], c11[3]);
            GXTexCoord2f32(t11->x, t11->z);
            GXTexCoord2f32(8.0f * t11->x, 8.0f * t11->z);
            GXPosition3f32(p10->x, p10->y, p10->z);
            GXColor4u8(c10[0], c10[1], c10[2], c10[3]);
            GXTexCoord2f32(t10->x, t10->z);
            GXTexCoord2f32(8.0f * t10->x, 8.0f * t10->z);
        }
    }
#ifdef TARGET_PC
    GXEnd();
#endif
}

void fn_1_169F8(HU3DMODEL *model, Mtx *mtx)
{
    Mtx *m;

    m = mtx;
    fn_1_16E5C((M537Wake *)model->hookData, *mtx);
}

M537Wake *fn_1_16A3C(M537Wake *self, Vec *pos)
{
    s16 id;

    fn_1_1264(self);
    self->vptr = lbl_1_data_1FC;
    self->flag = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    self->vptr = lbl_1_data_6D0;
    self->pos.x = pos->x;
    self->pos.y = pos->y;
    self->pos.z = pos->z;
    self->t = 0.0f;
    id = Hu3DHookFuncCreate((HU3DMODELHOOK)fn_1_169F8);
    Hu3DData[id].hookData = self;
    Hu3DModelLayerSet(id, 4);
    Hu3DModelCameraSet(id, 1);
    self->mdl = id;
    self->anim = HuSprAnimRead(HuDataSelHeapReadNum(0x5F001D, 0x10000000, HEAP_MODEL));
    HuSprAnimLock(self->anim);
    return self;
}

M537Wake *fn_1_16B48(M537Wake *self, s16 freeIt)
{
    if (self != 0) {
        self->vptr = lbl_1_data_6D0;
        Hu3DModelKill(self->mdl);
        HuSprAnimKill(self->anim);
        if (self != 0) {
            self->vptr = lbl_1_data_1FC;
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

void fn_1_16BD8(M537Wake *self, s32 bit)
{
    self->flag &= ~(1 << bit);
}

// Wake update: grows the wake and follows the water height under it.
void fn_1_16BF0(M537Wake *self)
{
    M537Water *w;
    f32 sum;
    s32 gx;
    s32 gz;
    s32 cx;
    s32 cz;
    s32 i;
    s32 j;

    self->t = (f32)((f64)self->t + 0.016666666666666666);
    if (self->t > 1.0f) {
        self->t = 0.0f;
    }
    if (lbl_1_bss_9F0 != 0) {
    } else {
        fn_1_1D4("ms_pointer != 0", "singleton.h", 0x27);
    }
    gx = (s32)(16.0f + (f32)(s32)(self->pos.x / 200.0f));
    gz = (s32)(16.0f + (f32)(s32)(self->pos.z / 200.0f));
    if (gx < 1) {
        cx = 1;
    } else if (gx > 0x1E) {
        cx = 0x1E;
    } else {
        cx = gx;
    }
    if (gz < 1) {
        cz = 1;
    } else if (gz > 0x1E) {
        cz = 0x1E;
    } else {
        cz = gz;
    }
    w = lbl_1_bss_9F0;
    sum = 0.0f;
    for (i = -1; i <= 1; i++) {
        for (j = -1; j <= 1; j++) {
            sum += w->pos[cz + i][cx + j].y;
        }
    }
    self->pos.y = 15.000001f + (sum / 9.0f);
}

// Draws the wake as a growing textured quad on the water.
void fn_1_16E5C(M537Wake *self, Mtx mtx)
{
    Mtx trans;
    Mtx scale;
    GXColor col;
    GXColor col1;
    f32 s;

    PSMTXTrans(trans, self->pos.x, self->pos.y, self->pos.z);
    s = 1.5f * self->t;
    PSMTXScale(scale, s, 1.0f, s);
    PSMTXConcat(mtx, trans, mtx);
    PSMTXConcat(mtx, scale, scale);
    GXLoadPosMtxImm(scale, GX_PNMTX0);
    HuSprTexLoad(self->anim, 0, GX_TEXMAP0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_LEQUAL, 0xFF);
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    col.r = 0xFF;
    col.g = 0xFF;
    col.b = 0xFF;
    col.a = 0;
    col.a = (u8)(s8)(255.0f * (1.0f - self->t));
    col1.r = 0xFF;
    col1.g = 0xFF;
    col1.b = 0xFF;
    col1.a = col.a;
    GXSetTevColor(GX_TEVREG0, col1);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVREG0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVREG0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-100.0f, 0.0f, -100.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(100.0f, 0.0f, -100.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(100.0f, 0.0f, 100.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(-100.0f, 0.0f, 100.0f);
    GXTexCoord2f32(0.0f, 1.0f);
#ifdef TARGET_PC
    GXEnd();
#endif
}

}
