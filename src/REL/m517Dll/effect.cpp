// m517Dll effect: the ball the players push around (M517Ball), the spark trail it
// leaves (M517Spark, a particle task) and the allocation helper the other units use.
#define _MATH_H
#include "REL/m517Dll.h"
extern "C" {
#include "game/frand.h"
#include "game/audio.h"
}

/* Spark particle task: link/vtbl/flags, the particle model and a trail anchor. */
typedef struct M517Spark_s {
    M517Task task;
    HU3DMODELID modelId;    // 0x10
    ANIMDATA *anim;         // 0x14
    HU3DPARTICLE *particle; // 0x18
    Vec pos;                // 0x1C
    Vec vec;                // 0x28
} M517Spark;

extern "C" {

extern M517Ptmf __ptmf_null;
extern char lbl_1_data_80[];
extern char lbl_1_data_8C[];

M517Spark *fn_1_44(M517Spark *self);
M517Task *fn_1_23C(M517Task *self, s16 flag);
M517Spark *fn_1_300(M517Spark *self, s16 flag);
void fn_1_39C(M517Spark *self, s32 bit);
void fn_1_5AC(M517Spark *self, Mtx mtx);
void fn_1_8A4(M517Spark *self, Vec *target);
void fn_1_1A44(M517Ball *self);
void fn_1_1904(M517Ball *self, Vec *oldPos, Vec *newPos);
void fn_1_16F8(M517Ball *self);

char lbl_1_data_24[] = "sm_ptr != 0";
char lbl_1_data_30[] = "a_set.h";
char lbl_1_data_38[] = "ms_pointer != 0";
char lbl_1_data_48[] = "singleton.h";
void *lbl_1_data_54[5] = { 0, 0, (void *)fn_1_300, (void *)fn_1_39C, (void *)fn_1_1E80 };
M517TaskVtblEx lbl_1_data_68 = { { { 0, 0 }, fn_1_23C, 0, fn_1_1E80, 0 }, { { 0, 0, 0 }, { 0, 0, 0 } } };

void *lbl_1_bss_0;
void *lbl_1_bss_4;
void *lbl_1_bss_8;
void *lbl_1_bss_C;
void *lbl_1_bss_10;

// Restart one spark particle around the spark task.
static inline void M517SparkInit(M517Spark *self, HU3DPARTICLEDATA *p)
{
    p->time = 0;
    p->parManId = (frand() & 0xF) + 4;
    p->pos.x = self->pos.x + (50.0f * (0.000030517578f * ((f32)(u16)frand() - 32768.0f)));
    p->pos.z = self->pos.z + (50.0f * (0.000030517578f * ((f32)(u16)frand() - 32768.0f)));
    p->scale = 100.0f * (0.5f + (0.000015258789f * (f32)(u16)frand()));
    p->pos.y = self->pos.y + (0.5f * p->scale);
    p->zRot = 0.017453292f * (360.0f * (0.000015258789f * (f32)(u16)frand()));
    p->color.a = 0;
}

// Particle hook: run the spark update on the owning task.
void fn_1_0(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx)
{
    MtxPtr sp8 = mtx;

    fn_1_5AC((M517Spark *)particle->work, sp8);
}

M517Spark *fn_1_44(M517Spark *self)
{
    HU3DMODELID id;
    HU3DPARTICLEDATA *p;
    s32 i;

    fn_1_2DA8(&self->task.link);
    self->task.vtbl = &lbl_1_data_68.vtbl;
    self->task.flags = 0;
    fn_1_2E74(&self->task.link, &lbl_1_bss_24);
    self->task.vtbl = (M517TaskVtbl *)lbl_1_data_54;
    self->anim = HuSprAnimRead(HuDataSelHeapReadNum(0x4B0011, HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(self->anim);
    id = Hu3DParticleCreate(self->anim, 0x10);
    self->particle = (HU3DPARTICLE *)Hu3DData[id].hookData;
    self->particle->work = self;
    Hu3DParticleHookSet(id, fn_1_0);
    Hu3DParticleColSet(id, 0xF0, 0xE0, 0x80);
    Hu3DParticleTPLvlSet(id, 0.0f);
    Hu3DParticleBlendModeSet(id, 0);
    Hu3DModelLayerSet(id, 2);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    self->modelId = id;
    self->pos.x = 0.0f;
    self->pos.y = 0.0f;
    self->pos.z = 0.0f;
    if (self->particle->count == 0) {
        p = self->particle->data;
        for (i = 0; i < (s16)self->particle->maxCnt; i++, p++) {
            p->time = 3;
        }
        self->particle->dataCnt = 0;
    }
    return self;
}

M517Task *fn_1_23C(M517Task *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_68.vtbl;
        if (self != 0) {
            fn_1_2DB4(&self->link, 0);
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

M517Node *fn_1_2A8(M517Node *self, s16 flag)
{
    if (self != 0) {
        fn_1_2DB4(self, 0);
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

M517Spark *fn_1_300(M517Spark *self, s16 flag)
{
    if (self != 0) {
        self->task.vtbl = (M517TaskVtbl *)lbl_1_data_54;
        self->particle->work = 0;
        Hu3DModelKill(self->modelId);
        HuSprAnimKill(self->anim);
        if (self != 0) {
            self->task.vtbl = &lbl_1_data_68.vtbl;
            if (self != 0) {
                fn_1_2DB4(&self->task.link, 0);
            }
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

void fn_1_39C(M517Spark *self, s32 bit)
{
    self->task.flags &= ~(1 << bit);
}

// Restart spark particle p around the task.
void fn_1_3B4(M517Spark *self, HU3DPARTICLEDATA *p)
{
    M517SparkInit(self, p);
}

// Particle update: fade each spark in, then out, spinning it.
void fn_1_5AC(M517Spark *self, Mtx mtx)
{
    HU3DMODEL *model;
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *p;
    s32 i;

    model = &Hu3DData[self->modelId];
    particle = self->particle;
    p = particle->data;
    for (i = 0; i < (s16)particle->maxCnt; i++, p++) {
        if ((s16)p->time > 2) {
            continue;
        }
        switch ((s16)p->time) {
            case 0:
                M517SparkInit(self, p);
                p->time++;
                // fall through
            case 1:
                p->color.a = p->color.a + 8;
                if (p->color.a < 0xF0) {
                    break;
                }
                p->color.a = 0xFF;
                p->time++;
                // fall through
            case 2:
                p->color.a = p->color.a - p->parManId;
                if (p->color.a > 0x14) {
                    break;
                }
                p->color.a = 0;
                p->time++;
                break;
            default:
                break;
        }
        p->zRot = p->zRot + 0.017453292f;
    }
}

// Pull the spark trail towards target (flattened to the ground) and emit the next particle.
void fn_1_8A4(M517Spark *self, Vec *target)
{
    Vec to;
    Vec d;
    HU3DPARTICLEDATA *p;
    f32 mag;

    to = *target;
    to.y = 0.0f;
    PSVECSubtract(&to, &self->pos, &d);
    mag = PSVECMag(&d);
    if (mag >= 10.0f) {
        self->pos = to;
        p = self->particle->data + self->particle->dataCnt;
        if ((s16)p->time > 2) {
            p->time = 0;
        }
        self->particle->dataCnt = self->particle->dataCnt + 1;
        if (self->particle->dataCnt >= 0x10) {
            self->particle->dataCnt = 0;
        }
    }
}

void fn_1_9D0(M517Spark *self, Vec *src)
{
    self->vec.x = src->x;
    self->vec.y = src->y;
    self->vec.z = src->z;
}

// Push the two balls apart: a moves by ka of the overlap, b by the rest. 1 if they overlapped.
s32 fn_1_9EC(Vec *a, M517Sphere *b, f32 ka)
{
    Vec d;
    Vec t;
    f32 rad;
    f32 dist;
    f32 kb;

    rad = 130.0f + b->radius;
    PSVECSubtract(&b->pos, a, &d);
    dist = PSVECMag(&d);
    if (dist >= rad) {
        return 0;
    }
    kb = 1.0f - ka;
    PSVECScale(&d, &d, (rad - dist) / dist);
    PSVECScale(&d, &t, -ka);
    PSVECAdd(&t, a, a);
    PSVECScale(&d, &t, kb);
    PSVECAdd(&t, &b->pos, &b->pos);
    return 1;
}

M517Ball *fn_1_B58(M517Ball *self, s32 arg)
{
    M517Spark *spark;

    fn_1_2DA8(&self->link);
    self->arg = arg;
    self->hit = 0;
    fn_1_474C(&self->model, 0x4B0004);
    self->spark = 0;
    M517_ASSERT(lbl_1_bss_0 != 0, lbl_1_data_24, lbl_1_data_30, 0x2E);
    fn_1_2E74(&self->link, (M517Node *)lbl_1_bss_0);
    self->sph.radius = 100.0f;
    Hu3DModelShadowSet(self->model.id);
    Hu3DModelShadowDispOn(self->model.id);
    Hu3DModelLayerSet(self->model.id, 1);
    Hu3DModelCameraSet(self->model.id, 1);
    self->vel.x = 0.0f;
    self->vel.y = 0.0f;
    self->vel.z = 0.0f;
    self->v94.x = 0.0f;
    self->v94.y = 0.0f;
    self->v94.z = 0.0f;
    self->v88.x = 0.0f;
    self->v88.y = 0.0f;
    self->v88.z = 0.0f;
    self->accel.x = 0.0f;
    self->accel.y = 0.0f;
    self->accel.z = 0.0f;
    self->delta.x = 0.0f;
    self->delta.y = 0.0f;
    self->delta.z = 0.0f;
    self->rot.x = 0.0f;
    self->rot.y = 0.0f;
    self->rot.z = 0.0f;
    self->rot.w = 1.0f;
    PSMTXIdentity(self->mtx);
    spark = (M517Spark *)fn_1_1E84(sizeof(M517Spark));
    if (spark != 0) {
        fn_1_44(spark);
    }
    if (self->spark != spark) {
        M517Spark *old = self->spark;

        if (old != 0) {
            old->task.vtbl->dtor(&old->task, 1);
        }
        self->spark = spark;
    }
    self->snd = HuAudFXPlay(0x6F2);
    HuAudFXVolSet(self->snd, 0);
    self->hook = __ptmf_null;
    return self;
}

// Owner of a spark pointer: destroy the spark.
M517Spark **fn_1_1024(M517Spark **self, s16 flag)
{
    if (self != 0) {
        M517Spark *obj = *self;

        if (obj != 0) {
            obj->task.vtbl->dtor(&obj->task, 1);
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

M517Node *fn_1_10A0(M517Node *self, s16 flag)
{
    if (self != 0) {
        fn_1_2DB4(self, 0);
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

M517Ball *fn_1_10F8(M517Ball *self, s16 flag)
{
    if (self != 0) {
        HuAudFXStop(self->snd);
        if ((void *)&self->spark != 0) {
            M517Spark *old = self->spark;

            if (old != 0) {
                old->task.vtbl->dtor(&old->task, 1);
            }
        }
        fn_1_4BAC(&self->model, -1);
        if (self != 0) {
            fn_1_2DB4(&self->link, 0);
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

// Put the ball at pos and stop it.
void fn_1_11A8(M517Ball *self, Vec *pos)
{
    void *ms;

    M517_ASSERT(lbl_1_bss_10 != 0, lbl_1_data_38, lbl_1_data_48, 0x27);
    ms = lbl_1_bss_10;
    self->sph.pos.x = pos->x;
    self->sph.pos.y = pos->y;
    self->sph.pos.z = pos->z;
    self->sph.pos.y = self->sph.pos.y + 100.0f;
    self->vel.x = 0.0f;
    self->vel.y = 0.0f;
    self->vel.z = 0.0f;
    self->v94.x = 0.0f;
    self->v94.y = 0.0f;
    self->v94.z = 0.0f;
    self->accel.x = 0.0f;
    self->accel.y = 0.0f;
    self->accel.z = 0.0f;
}

// Speed of the ball in units of 11.67.
f32 fn_1_12E8(M517Ball *self)
{
    Vec v = self->vel;
    f32 mag = PSVECMag(&v);

    return mag / 11.666666666666666;
}

void fn_1_1360(M517Ball *self, Quaternion *dst)
{
    dst->x = self->rot.x;
    dst->y = self->rot.y;
    dst->z = self->rot.z;
    dst->w = self->rot.w;
}

void fn_1_1384(M517Ball *self, Vec *accel)
{
    self->accel.x = accel->x;
    self->accel.y = accel->y;
    self->accel.z = accel->z;
}

void fn_1_13A0(M517Ball *self)
{
    self->vel.x = 0.0f;
    self->vel.y = 0.0f;
    self->vel.z = 0.0f;
}

// Per-frame update: move, drag the spark trail along and fit the rolling sound to the speed.
void fn_1_13D4(M517Ball *self)
{
    Vec snd;
    f32 vol;

    if (M517_PTMF_TEST(&self->hook)) {
        M517_PTMF_CALL(&self->hook, self);
    }
    fn_1_16F8(self);
    PSVECSubtract(&self->sph.pos, &self->prev, &self->delta);
    self->prev.x = self->sph.pos.x;
    self->prev.y = self->sph.pos.y;
    self->prev.z = self->sph.pos.z;
    fn_1_8A4(self->spark, &self->sph.pos);
    self->accel.x = 0.0f;
    self->accel.y = 0.0f;
    self->accel.z = 0.0f;
    snd = self->sph.pos;
    fn_1_1BC6C(self->snd, &snd);
    vol = PSVECMag(&self->delta) / 11.666666666666666;
    if (vol < 0.0f) {
        vol = 0.0f;
    }
    if (vol > 1.0f) {
        vol = 1.0f;
    }
    HuAudFXVolSet(self->snd, (s32)(127.0f * vol));
}

// Limit the horizontal speed to +-limit.
void fn_1_164C(M517Ball *self, f32 limit)
{
    f32 x = self->vel.x;
    f32 z = self->vel.z;

    if (x < -limit) {
        x = -limit;
    } else if (x > limit) {
        x = limit;
    }
    self->vel.x = x;
    if (z < -limit) {
        z = -limit;
    } else if (z > limit) {
        z = limit;
    }
    self->vel.z = z;
}

// Integrate velocity and gravity, then keep the ball inside the arena and spin it.
void fn_1_16F8(M517Ball *self)
{
    Vec scaled;
    Vec v;
    Vec prev;
    f32 x;
    f32 z;

    PSVECScale(&self->accel, &scaled, 0.3f);
    PSVECAdd(&scaled, &self->vel, &self->vel);
    x = self->vel.x;
    if (x < -11.666667f) {
        x = -11.666667f;
    } else if (x > 11.666667f) {
        x = 11.666667f;
    }
    self->vel.x = x;
    z = self->vel.z;
    if (z < -11.666667f) {
        z = -11.666667f;
    } else if (z > 11.666667f) {
        z = 11.666667f;
    }
    self->vel.z = z;
    v.x = self->vel.x;
    v.y = self->vel.y;
    v.z = self->vel.z;
    self->vel.y = self->vel.y + 0.27240694444444447;
    v.y = -(0.5f * (v.y + self->vel.y));
    prev.x = self->sph.pos.x;
    prev.y = self->sph.pos.y;
    prev.z = self->sph.pos.z;
    PSVECAdd(&v, &self->sph.pos, &self->sph.pos);
    fn_1_1A44(self);
    PSVECScale(&self->vel, &self->vel, 0.99f);
    fn_1_1904(self, &prev, &self->sph.pos);
    fn_1_49AC(&self->model, &self->sph.pos);
}

// Roll the ball's orientation by the distance it moved.
void fn_1_1904(M517Ball *self, Vec *oldPos, Vec *newPos)
{
    Quaternion q;
    Vec from;
    Vec to;
    f32 r = self->sph.radius;
    f32 dz;
    f32 dx;

    dz = oldPos->z - newPos->z;
    dx = oldPos->x - newPos->x;
    to.x = dx;
    to.y = -r;
    to.z = dz;
    from.x = 0.0f;
    from.y = -r;
    from.z = 0.0f;
    fn_1_ED24(&q, &from, &to);
    PSQUATMultiply(&q, &self->rot, &self->rot);
    PSQUATNormalize(&self->rot, &self->rot);
    PSMTXQuat(self->mtx, &self->rot);
    PSMTXCopy(self->mtx, fn_1_4B90(&self->model));
}

// Keep the ball inside the round arena (radius 600 minus its own radius) and floor it.
void fn_1_1A44(M517Ball *self)
{
    Vec t;
    Vec s;
    Vec p;
    void *ms;
    f32 arena;
    f32 lim;
    f32 mag;
    f32 x;
    f32 z;

    M517_ASSERT(lbl_1_bss_10 != 0, lbl_1_data_38, lbl_1_data_48, 0x27);
    ms = lbl_1_bss_10;
    arena = 600.0f;
    z = self->sph.pos.z;
    x = self->sph.pos.x;
    t.x = x;
    t.y = 0.0f;
    t.z = z;
    mag = PSVECMag(&t);
    lim = arena - 100.0f;
    if (mag > lim) {
        PSVECScale(&t, &s, (lim - mag) / mag);
        PSVECAdd(&s, &self->sph.pos, &self->sph.pos);
        PSVECAdd(&s, &self->vel, &self->vel);
        if (self->hit == 0) {
            if (PSVECMag(&self->vel) > 1.6666666666666667) {
                p = self->sph.pos;
                fn_1_1B6EC(0x6F4, &p);
                self->hit = 1;
            }
        }
    } else {
        self->hit = 0;
    }
    if (self->sph.pos.y < 100.0f) {
        self->sph.pos.y = 100.0f;
        self->vel.y = 0.0f;
    }
}

// Ball against ball collision (result unused by the original).
void fn_1_1C78(M517Ball *self, M517Ball *other)
{
    M517Sphere *a = &self->sph;
    M517Sphere *b = &other->sph;

    fn_1_6B7C(a, b, 0.5f, 1.0f);
}

// Push the ball away from a player's sphere and tell the player.
void fn_1_1CEC(M517Ball *self, M517PlayerHead *other)
{
    Vec *a;
    M517Sphere *b;
    s32 hit;

    if (other->cpu == 0 && other->busy == 0) {
        a = &self->sph.pos;
        b = &other->sph;
        hit = fn_1_9EC(a, b, 0.0f);
        if ((u8)hit != 0) {
            fn_1_17FCC(other, self);
        }
    }
}

void fn_1_1E80(M517Task *self)
{
}

void *fn_1_1E84(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);

    M517_ASSERT(p != 0, lbl_1_data_80, lbl_1_data_8C, 0x17);
    return p;
}

} // extern "C"
