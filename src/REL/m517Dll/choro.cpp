// m517Dll choro unit: the "choro" enemies that pop out of the ground, the dust
// particle effect they kick up and the manager that spawns them.
// Each choro is a node of the intrusive a_set list (lbl_1_bss_A1C), owns three
// models, a motion table and a PTMF state hook.
#include "REL/m517Dll.h"
extern "C" {
#include "game/frand.h"
#include <math.h>
}

#ifdef __MWERKS__
#define M517_RODATA __declspec(section ".rodata")
#else
#define M517_RODATA
#endif

extern "C" {

void PSVECSubtract(const Vec *a, const Vec *b, Vec *dst);
f32 PSVECMag(const Vec *v);

void fn_1_17614(void);
void fn_1_16B68(void);
void fn_1_183C4(M517Player *player);

extern M517Node *lbl_1_bss_A1C;

void fn_1_100E8(M517ChoroFx *self, Mtx mtx);
void fn_1_FDC4(M517ChoroFx *self, HU3DPARTICLE *particle);
void fn_1_1115C(M517Choro *self);
void fn_1_122AC(M517Choro *self);
void fn_1_1342C(M517ChoroMgr *self);
void fn_1_12EFC(M517ChoroMgr *self);
u8 fn_1_13214(M517ChoroMgr *self, M517Node *first, M517Node *end, Vec *out);
void fn_1_111A4(void);
void fn_1_111A8(void);
void fn_1_12EF8(void);
s32 lbl_1_bss_A38;
s8 lbl_1_bss_A3C;
s32 lbl_1_bss_A40;
s8 lbl_1_bss_A44;

void fn_1_12984(M517ChoroMgr *self);
void fn_1_12D00(M517ChoroMgr *self);
void fn_1_12E64(M517ChoroMgr *self);
M517ChoroMgr *fn_1_126B4(M517ChoroMgr *self, s16 flag);
void fn_1_12780(M517ChoroMgr *self, s32 bit);
void fn_1_1293C(M517ChoroMgr *self);
void fn_1_FDAC(M517Task *self, s32 bit);
M517ChoroFx *fn_1_FCA0(M517ChoroFx *self, s16 flag);

M517_RODATA const f32 lbl_1_rodata_380[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
M517_RODATA const s32 lbl_1_rodata_3FC[3] = { 0x4B0008, 0x4B0008, 0x4B0009 };

M517Ptmf lbl_1_data_588[3] = {
    { 0, -1, (void (*)(void *))fn_1_111A8 },
    { 0, -1, (void (*)(void *))fn_1_17614 },
    { 0, -1, (void (*)(void *))fn_1_16B68 },
};
M517Ptmf lbl_1_data_5AC = { 0, -1, (void (*)(void *))fn_1_111A4 };
char lbl_1_data_5B8[] = "sm_ptr != 0";
char lbl_1_data_5C4[] = "a_set.h";
char lbl_1_data_5E8[] = "ms_pointer != 0";
char lbl_1_data_5F8[] = "singleton.h";
M517Ptmf lbl_1_data_604 = { 0, -1, (void (*)(void *))fn_1_12EF8 };
char lbl_1_data_610[] = "ms_pointer == 0";
M517Ptmf lbl_1_data_620 = { 0, -1, (void (*)(void *))fn_1_12984 };
M517Ptmf lbl_1_data_62C = { 0, -1, (void (*)(void *))fn_1_12D00 };
M517Ptmf lbl_1_data_638 = { 0, -1, (void (*)(void *))fn_1_12E64 };
M517Ptmf lbl_1_data_644 = { 0, -1, (void (*)(void *))fn_1_12EF8 };
void *lbl_1_data_650[5] = { 0, 0, (void *)fn_1_126B4, (void *)fn_1_12780, (void *)fn_1_1293C };
void *lbl_1_data_664[5] = { 0, 0, (void *)fn_1_FCA0, (void *)fn_1_FDAC, (void *)fn_1_1E80 };

// Hook of the dust particle: advance the effect.
void fn_1_FAB0(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx)
{
    MtxPtr sp8 = mtx;

    fn_1_100E8((M517ChoroFx *)particle->work, sp8);
}

M517ChoroFx *fn_1_FAF4(M517ChoroFx *self)
{
    HU3DMODELID id;

    fn_1_2DA8(&self->link);
    self->vtbl = (void **)&lbl_1_data_68;
    self->flags = 0;
    fn_1_2E74(&self->link, &lbl_1_bss_24);
    self->vtbl = lbl_1_data_664;
    self->anim = HuSprAnimDataRead(0x4B0011);
    HuSprAnimLock(self->anim);
    id = Hu3DParticleCreate(self->anim, 6);
    self->particle = (HU3DPARTICLE *)Hu3DData[id].hookData;
    self->particle->work = self;
    Hu3DParticleHookSet(id, fn_1_FAB0);
    Hu3DParticleColSet(id, 0xF0, 0xE0, 0x80);
    Hu3DParticleTPLvlSet(id, 0.0f);
    Hu3DParticleBlendModeSet(id, 0);
    Hu3DModelLayerSet(id, 5);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    self->modelId = id;
    self->done = 1;
    fn_1_FDC4(self, self->particle);
    Hu3DModelAttrSet(id, 1);
    return self;
}

M517ChoroFx *fn_1_FCA0(M517ChoroFx *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_664;
        self->particle->work = 0;
        Hu3DModelKill(self->modelId);
        HuSprAnimKill(self->anim);
        if (self != 0) {
            self->vtbl = (void **)&lbl_1_data_68;
            if (self != 0) {
                fn_1_2DB4(&self->link, 0);
            }
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

// Put the effect at pos and let it play again.
void fn_1_FD3C(M517ChoroFx *self, Vec *pos)
{
    Hu3DModelPosSet(self->modelId, pos->x, pos->y, pos->z);
    Hu3DParticleTPLvlSet(self->modelId, 0.0f);
    self->done = 0;
    Hu3DModelAttrReset(self->modelId, 1);
}

// Virtual onFlag(): clear one flag bit.
void fn_1_FDAC(M517Task *self, s32 bit)
{
    self->flags &= ~(1 << bit);
}

// Restart every particle of the dust ring.
void fn_1_FDC4(M517ChoroFx *self, HU3DPARTICLE *particle)
{
    HU3DPARTICLEDATA *p = particle->data;
    s32 i;

    for (i = 0; i < (s16)particle->maxCnt; i++, p++) {
        p->vel.x = sin(0.017453292f * ((360.0f * (f32)i) / (f32)(s16)particle->maxCnt));
        p->accel.y = p->vel.y = 0.27240694444444447 * (f64)(10.0f + (15.0f * (0.000015258789f * (f32)(u16)frand())));
        p->vel.z = cos(0.017453292f * ((360.0f * (f32)i) / (f32)(s16)particle->maxCnt));
        p->scale = 100.0f;
        p->pos.x = 100.0f * (0.01f * ((f32)(s32)(fn_1_6598() & 0x3F) - 31.0f));
        p->pos.y = 0.0f;
        p->pos.z = 100.0f * (0.01f * ((f32)(s32)(fn_1_6598() & 0x3F) - 31.0f));
        p->color.a = 0;
        p->time = 0;
        p->parManId = (s32)(fn_1_6598() & 0xF) + 0xF;
        p->zRot = 0.017453292f * (360.0f * (0.000015258789f * (f32)(u16)frand()));
    }
}

// Particle hook body: fade the dust in and out, drift it and restart it when it is gone.
void fn_1_100E8(M517ChoroFx *self, Mtx mtx)
{
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *p;
    u8 allDone;
    s32 i;

    if (self->done == 0) {
        particle = self->particle;
        allDone = 1;
        p = particle->data;
        for (i = 0; i < (s16)particle->maxCnt; i++, p++) {
            if (p->time != 2) {
                f32 vy;

                if (p->time == 0) {
                    p->color.a = p->color.a + p->parManId;
                    if (p->color.a >= 0xDC) {
                        p->color.a = 0xDC;
                        p->time++;
                    }
                } else if (p->color.a > 0x20) {
                    p->color.a = p->color.a - (s32)(0.5f * (f32)p->parManId);
                } else {
                    p->time++;
                }
                p->scale = p->scale + 3.3333333333333335;
                vy = p->vel.y;
                p->vel.y = p->vel.y + -0.27240694444444447;
                {
                    f32 dy = 0.5f * (vy + p->vel.y);

                    p->pos.x = p->pos.x + 0.016666666666666666 * (100.0f * (2.0f * p->vel.x));
                    p->pos.y = p->pos.y + dy;
                    p->pos.z = p->pos.z + 0.016666666666666666 * (100.0f * (2.0f * p->vel.z));
                }
                allDone = 0;
            }
        }
        self->done = allDone;
        if (self->done != 0) {
            fn_1_FDC4(self, particle);
            Hu3DModelAttrSet(self->modelId, 1);
        }
    }
}

// Choro constructor: three models, the motion table, the dust effect.
M517Choro *fn_1_10634(M517Choro *self)
{
    s32 i;
    M517ChoroFx *fx;

    fn_1_2DA8(&self->link);
    fn_1_474C(&self->model[0], 0x4B0005);
    fn_1_474C(&self->model[1], 0x4B0006);
    fn_1_474C(&self->model[2], 0x4B0007);
    self->active = 0;
    self->hiding = 0;
    self->turn = 0;
    self->turnAngle = 0.0f;
    self->kind = -1;
    self->timer = 0.0f;
    self->timer2 = 0.0f;
    self->scale = 1.0f;
    self->state = 0;
    self->dir = 0;
    self->motFlag = 0;
    self->fx = 0;
    self->pos.x = 0.0f;
    self->pos.y = 0.0f;
    self->pos.z = 0.0f;
    self->rot.x = 0.0f;
    self->rot.y = 0.0f;
    self->rot.z = 0.0f;
    M517_ASSERT(lbl_1_bss_A1C != 0, lbl_1_data_5B8, lbl_1_data_5C4, 0x2E);
    fn_1_2E74(&self->link, lbl_1_bss_A1C);
    self->radius = 30.000002f;
    fn_1_49AC(&self->model[0], &self->pos);
    fn_1_49AC(&self->model[1], &self->pos);
    Hu3DModelAttrSet(self->model[0].id, 1);
    Hu3DModelAttrSet(self->model[0].id, 0x40000002);
    Hu3DModelAttrSet(self->model[1].id, 1);
    Hu3DModelAttrSet(self->model[1].id, 0x40000002);
    Hu3DModelAttrSet(self->model[2].id, 1);
    for (i = 0; i < 3; i++) {
        self->mot[i] = Hu3DJointMotion(self->model[2].id, HuDataSelHeapReadNum(lbl_1_rodata_3FC[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    Hu3DMotionSet(self->model[2].id, self->mot[0]);
    Hu3DModelAttrSet(self->model[2].id, 0x40000002);
    Hu3DModelAttrSet(self->model[2].id, 0x40000001);
    Hu3DModelLayerSet(self->model[2].id, 1);
    Hu3DModelCameraSet(self->model[2].id, 1);
    Hu3DModelLayerSet(self->model[0].id, 1);
    Hu3DModelCameraSet(self->model[0].id, 1);
    Hu3DModelLayerSet(self->model[1].id, 1);
    Hu3DModelCameraSet(self->model[1].id, 1);
    fn_1_49AC(&self->model[2], &self->pos);
    fn_1_1115C(self);
    self->active = 0;
    self->hook = lbl_1_data_5AC;
    self->wait = 0.0f;
    fx = (M517ChoroFx *)fn_1_1E84(sizeof(M517ChoroFx));
    if (fx != 0) {
        fx = fn_1_FAF4(fx);
    }
    if (self->fx != fx) {
        if (self->fx != 0) {
            ((M517TaskVtbl *)self->fx->vtbl)->dtor((M517Task *)self->fx, 1);
        }
        self->fx = fx;
    }
    return self;
}

// Smart pointer holder destructor.
M517ChoroFxPtr *fn_1_10EB4(M517ChoroFxPtr *self, s16 flag)
{
    if (self != 0) {
        if (self->p != 0) {
            ((M517TaskVtbl *)self->p->vtbl)->dtor((M517Task *)self->p, 1);
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

// a_set head destructor.
void *fn_1_10F30(M517Node *self, s16 flag)
{
    if (self != 0) {
        fn_1_2DB4(self, 0);
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

M517Choro *fn_1_10F88(M517Choro *self, s16 flag)
{
    if (self != 0) {
        if (&self->fx != 0) {
            if (self->fx != 0) {
                ((M517TaskVtbl *)self->fx->vtbl)->dtor((M517Task *)self->fx, 1);
            }
        }
        fn_1_4BAC(&self->model[2], -1);
        fn_1_4BAC(&self->model[1], -1);
        fn_1_4BAC(&self->model[0], -1);
        if (self != 0) {
            fn_1_2DB4(&self->link, 0);
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

// Move the choro (all three models) to pos, ground it at y = -120.
void fn_1_11048(M517Choro *self, Vec *pos)
{
    fn_1_49AC(&self->model[0], pos);
    fn_1_49AC(&self->model[1], pos);
    self->pos.x = pos->x;
    self->pos.y = pos->y;
    self->pos.z = pos->z;
    self->pos.y = -120.00001f;
}

void fn_1_110D4(M517Choro *self)
{
    Hu3DMotionTimeSet(self->model[0].id, 0.0f);
    Hu3DModelAttrReset(self->model[0].id, 1);
    Hu3DModelAttrReset(self->model[0].id, 0x40000002);
}

void fn_1_1115C(M517Choro *self)
{
    Hu3DModelAttrReset(self->model[1].id, 0x40000002);
}

void fn_1_111A4(void)
{
}

void fn_1_111A8(void)
{
}

// Start a choro: kind -1 is the decoy shown at the start, 0 a manager spawn.
void fn_1_111AC(M517Choro *self, s32 kind, f32 delay)
{
    self->kind = kind;
    self->delay = delay;
    self->active = 1;
    self->timer = 0.0f;
    self->timer2 = 0.0f;
    self->rot.x = 0.0f;
    self->rot.y = 0.0f;
    self->rot.z = 0.0f;
    self->dir = frand() & 1;
    if (kind == -1) {
        self->wait = 0.0f;
    } else {
        self->wait = 2.0f * (0.000015258789f * (f32)(u16)frand());
    }
    self->state = 0;
    self->turn = 0;
    self->turnAngle = 0.0f;
    self->scale = 1.0f;
    Hu3DModelScaleSet(self->model[0].id, 1.0f, 1.0f, 1.0f);
    Hu3DModelAttrSet(self->model[1].id, 1);
    Hu3DModelAttrSet(self->model[1].id, 0x40000002);
    self->motFlag = 0;
    self->hiding = 1;
}

// Stop a choro; turn != 0 makes it face turnAngle while it sinks.
void fn_1_11368(M517Choro *self, u8 turn, f32 angle)
{
    if (self->state > 2) {
        Hu3DModelAttrSet(self->model[0].id, 1);
        Hu3DModelAttrReset(self->model[1].id, 1);
        Hu3DModelAttrReset(self->model[1].id, 0x40000002);
        Hu3DMotionShiftSet(self->model[2].id, self->mot[0], 0.0f, 2.0f, 0x40000005);
        Hu3DMotionSpeedSet(self->model[2].id, 2.0f);
    } else {
        self->scale = 1.0f;
    }
    self->turn = turn;
    self->turnAngle = angle;
    self->active = 0;
}

// Idle wobble (kind 0) or bobbing (other kinds) while the choro is up.
void fn_1_11464(M517Choro *self)
{
    if (self->kind != -1 && !(self->timer < self->wait)) {
        if (self->kind == 0) {
            switch (self->dir) {
            case 0:
                self->rot.y = self->rot.y + (45.0f - self->rot.y) / 6.0f;
                if (self->timer2 < self->wait2) {
                    break;
                }
                self->wait2 = 0.000015258789f * (f32)(u16)frand();
                self->timer2 = 0.0f;
                self->dir++;
                // fall through
            case 1:
                self->rot.y = self->rot.y + (-45.0f - self->rot.y) / 6.0f;
                if (self->timer2 < self->wait2) {
                    break;
                }
                self->wait2 = 0.000015258789f * (f32)(u16)frand();
                self->timer2 = 0.0f;
                self->dir = 0;
                break;
            }
        } else {
            f32 a;

            if (self->motFlag == 0) {
                HU3DMODELID id = self->model[2].id;

                Hu3DMotionShiftSet(id, self->mot[1], 0.0f, 8.0f, 0);
                Hu3DMotionStartEndSet(id, 0.0f, 10.0f);
                self->motFlag = 1;
            } else {
                HU3DMODELID id = self->model[2].id;

                if (Hu3DMotionTimeGet(id) >= 10.0f) {
                    Hu3DModelAttrSet(id, 0x40000002);
                }
            }
            a = 3.141592653589793 * (360.0f * self->timer2 * self->freq) / 180.0;
            self->pos.y = -20.0 + 10.0 * sin(a);
        }
        self->timer2 = self->timer2 + 0.016666666666666666;
    }
}

// Interpolate angle from towards to (degrees) by t along the short way round.
f32 fn_1_1174C(f32 from, f32 to, f32 t)
{
    f32 d = fmod(to - from, 360.0);
    f32 r;

    if (d < 0.0f) {
        d += 360.0f;
    }
    if (d > 180.0f) {
        d -= 360.0f;
    }
    r = fmod(from + t * d, 360.0);
    if (r < 0.0f) {
        r += 360.0f;
    }
    return r;
}

// Choro state hook: pop up, look around, duck again.
void fn_1_11850(M517Choro *self)
{
    fn_1_122AC(self);
    if (self->active != 0) {
        switch (self->state) {
        case 0:
            self->timer = self->timer + 0.016666666666666666;
            if (self->timer < self->delay) {
                break;
            }
            self->timer = 0.0f;
            fn_1_110D4(self);
            {
                Vec v = self->pos;

                v.y = 0.0f;
                if (self->kind != -1) {
                    fn_1_1B6EC(0x6F5, &v);
                }
            }
            self->state++;
            // fall through
        case 1:
            if (Hu3DMotionEndCheck(self->model[0].id) == 0) {
                break;
            }
            self->timer = 0.0f;
            self->state++;
            // fall through
        case 2:
            self->timer = self->timer + 0.016666666666666666;
            if (self->timer < self->wait) {
                break;
            }
            self->state++;
            // fall through
        case 3: {
            HU3DMODELID id = self->model[2].id;
            Vec v;
            Vec v2;

            Hu3DModelAttrReset(id, 1);
            Hu3DMotionShiftSet(id, self->mot[0], 0.0f, 8.0f, 0);
            Hu3DModelAttrReset(id, 0x40000002);
            Hu3DMotionSpeedSet(id, 1.0f);
            Hu3DModelAttrSet(self->model[0].id, 1);
            Hu3DMotionTimeSet(self->model[1].id, 0.0f);
            Hu3DModelAttrReset(self->model[1].id, 1);
            v = self->pos;
            v.y = 0.0f;
            fn_1_FD3C(self->fx, &v);
            self->wait2 = 0.000015258789f * (f32)(u16)frand();
            self->timer = 0.0f;
            v2 = self->pos;
            fn_1_1B6EC(0x6F6, &v2);
            self->state++;
        }
            // fall through
        case 4:
            self->timer = self->timer + 0.016666666666666666;
            self->pos.y = self->pos.y + (-20.0f - self->pos.y) / 6.0f;
            if (self->timer < 0.5f) {
                break;
            }
            self->wait = 0.5f + 0.5f * (0.000015258789f * (f32)(u16)frand());
            self->timer2 = 0.0f;
            self->freq = 2.0f * (0.000015258789f * (f32)(u16)frand());
            self->duration = 3.0f + 2.0f * (0.000015258789f * (f32)(u16)frand());
            self->timer = 0.0f;
            Hu3DMotionShiftSet(self->model[2].id, self->mot[2], 0.0f, 32.0f, 0x40000001);
            self->state++;
            // fall through
        case 5:
            self->timer = self->timer + 0.016666666666666666;
            fn_1_11464(self);
            if (self->timer < self->duration) {
                break;
            }
            {
                HU3DMODELID id = self->model[2].id;

                Hu3DMotionShiftSet(id, self->mot[0], 0.0f, 2.0f, 0x40000005);
                Hu3DMotionSpeedSet(id, 2.0f);
                Hu3DModelAttrReset(id, 0x40000002);
            }
            fn_1_1115C(self);
            self->timer = 0.0f;
            self->state++;
            // fall through
        case 6:
            self->active = 0;
            break;
        }
    } else if (self->state <= 2) {
        if (self->hiding == 1) {
            self->scale = self->scale + (-self->scale) / 24.0f;
            if (self->scale < 0.2f) {
                HU3DMODELID id = self->model[2].id;

                Hu3DMotionShiftSet(id, self->mot[0], 0.0f, 2.0f, 0x40000005);
                Hu3DMotionSpeedSet(id, 2.0f);
                Hu3DModelAttrSet(id, 0x40000002);
                Hu3DModelAttrSet(id, 1);
                Hu3DModelAttrReset(id, 0x40000004);
                self->state = 0;
                self->hiding = 0;
                Hu3DModelAttrSet(self->model[0].id, 1);
                Hu3DModelScaleSet(self->model[0].id, 1.0f, 1.0f, 1.0f);
                self->scale = 1.0f;
            } else {
                Hu3DModelScaleSet(self->model[0].id, self->scale, self->scale, self->scale);
            }
        }
    } else {
        self->pos.y = self->pos.y + (-200.0f - self->pos.y) / 48.0f;
        if (self->turn != 0) {
            self->rot.y = fn_1_1174C(self->rot.y, self->turnAngle, 0.5f);
        } else {
            self->rot.y = self->rot.y + (-self->rot.y) / 12.0f;
        }
        if (self->hiding == 1 && self->pos.y < -180.0f) {
            HU3DMODELID id = self->model[2].id;

            Hu3DModelAttrSet(id, 0x40000002);
            Hu3DModelAttrSet(id, 1);
            Hu3DModelAttrReset(id, 0x40000004);
            self->state = 0;
            self->hiding = 0;
        }
    }
    fn_1_49AC(&self->model[2], &self->pos);
    fn_1_49F4(&self->model[2], &self->rot);
    if (M517_PTMF_TEST(&self->hook)) {
        M517_PTMF_CALL(&self->hook, self);
    }
}

// A player touching a standing choro gets caught by it.
void fn_1_121D8(M517Choro *self, M517Player *player)
{
    Vec d;
    f32 reach;

    if (player->cpu == 0 && self->active != 0) {
        PSVECSubtract(&self->pos, &player->sph.pos, &d);
        reach = 2.0f * self->radius;
        if (PSVECMag(&d) < reach) {
            fn_1_183C4(player);
        }
    }
}

// If the stage focus is within 200 units, make the choro duck and face it.
void fn_1_122AC(M517Choro *self)
{
    Vec a;
    Vec b;
    Vec d;
    f32 ang;

    if (self->active != 0) {
        M517_ASSERT(lbl_1_bss_10 != 0, lbl_1_data_5E8, lbl_1_data_5F8, 0x27);
        a = ((M517Stage *)lbl_1_bss_10)->offs;
        b = a;
        PSVECSubtract(&b, &self->pos, &d);
        if (!(PSVECMag(&d) > 200.0f)) {
            ang = 180.0 * (atan2(d.x, d.z) / 3.141592653589793);
            fn_1_11368(self, 1, ang);
        }
    }
}

// Manager constructor.
M517ChoroMgr *fn_1_124B4(M517ChoroMgr *self)
{
    s32 i;

    fn_1_2DA8(&self->link);
    self->vtbl = (void **)&lbl_1_data_68;
    self->flags = 0;
    fn_1_2E74(&self->link, &lbl_1_bss_24);
    M517_ASSERT(lbl_1_bss_C == 0, lbl_1_data_610, lbl_1_data_5F8, 0x35);
    M517_SINGLETON_SET(lbl_1_bss_C, self, M517ChoroMgr);
    self->vtbl = lbl_1_data_650;
    self->timer = 0.0f;
    self->count = 0;
    for (i = 0; i < 12; i++) {
        self->spots[i].x = 0.0f;
        self->spots[i].y = 0.0f;
        self->spots[i].z = 0.0f;
    }
    self->hook = lbl_1_data_604;
    return self;
}

// Singleton base destructor.
M517ChoroMgr *fn_1_12628(M517ChoroMgr *self, s16 flag)
{
    if (self != 0) {
        M517_ASSERT(lbl_1_bss_C != 0, lbl_1_data_5E8, lbl_1_data_5F8, 0x3D);
        lbl_1_bss_C = 0;
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

M517ChoroMgr *fn_1_126B4(M517ChoroMgr *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_650;
        if (M517_SUBOBJ_NONNULL(self)) {
            M517_ASSERT(lbl_1_bss_C != 0, lbl_1_data_5E8, lbl_1_data_5F8, 0x3D);
            lbl_1_bss_C = 0;
        }
        if (self != 0) {
            self->vtbl = (void **)&lbl_1_data_68;
            if (self != 0) {
                fn_1_2DB4(&self->link, 0);
            }
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

// Virtual onFlag(): a set bit n selects the manager state n.
void fn_1_12780(M517ChoroMgr *self, s32 bit)
{
    switch (bit) {
    case 0: {
        M517Ptmf p = lbl_1_data_620;

        self->hook = p;
        fn_1_FDAC((M517Task *)self, bit);
        break;
    }
    case 1: {
        M517Ptmf p = lbl_1_data_62C;

        self->hook = p;
        fn_1_FDAC((M517Task *)self, bit);
        break;
    }
    case 2: {
        M517Ptmf p = lbl_1_data_638;

        self->hook = p;
        fn_1_FDAC((M517Task *)self, bit);
        break;
    }
    case 3: {
        M517Ptmf p = lbl_1_data_644;

        self->hook = p;
        fn_1_1342C(self);
        fn_1_FDAC((M517Task *)self, bit);
        break;
    }
    case 4:
        fn_1_FDAC((M517Task *)self, bit);
        break;
    }
}

// Virtual update(): run the state hook.
void fn_1_1293C(M517ChoroMgr *self)
{
    if (M517_PTMF_TEST(&self->hook)) {
        M517_PTMF_CALL(&self->hook, self);
    }
}

// Intro state: after 72 frames pop up the three decoy choros.
void fn_1_12984(M517ChoroMgr *self)
{
    M517Choro *c;
    s32 i;

    if (lbl_1_bss_A3C == 0) {
        lbl_1_bss_A38 = 0;
        lbl_1_bss_A3C = 1;
    }
    if (lbl_1_bss_A44 == 0) {
        lbl_1_bss_A40 = 0;
        lbl_1_bss_A44 = 1;
    }
    self->timer = self->timer + 0.016666666666666666;
    if ((f32)lbl_1_bss_A38 == 72.0f && lbl_1_bss_A40 == 0) {
        M517_ASSERT(lbl_1_bss_A1C != 0, lbl_1_data_5B8, lbl_1_data_5C4, 0x2E);
        c = (M517Choro *)lbl_1_bss_A1C->next;
        for (i = -1; i < 2; i++) {
            Vec pos;

            pos.x = 150.0f * (f32)i;
            pos.y = 0.0f;
            pos.z = 0.0f;
            fn_1_111AC(c, -1, 0.0f);
            fn_1_11048(c, &pos);
            c = (M517Choro *)c->link.next;
        }
        lbl_1_bss_A40 = 1;
    }
    lbl_1_bss_A38++;
}

// Drop the first three choros.
void fn_1_12D00(M517ChoroMgr *self)
{
    M517Choro *c;
    s32 i;

    M517_ASSERT(lbl_1_bss_A1C != 0, lbl_1_data_5B8, lbl_1_data_5C4, 0x2E);
    c = (M517Choro *)lbl_1_bss_A1C->next;
    for (i = 0; i < 3; i++) {
        fn_1_11368(c, 0, 0.0f);
        c = (M517Choro *)c->link.next;
    }
}

// Spawn state: every 5 seconds raise the number of simultaneous choros.
void fn_1_12E64(M517ChoroMgr *self)
{
    self->timer = self->timer + 0.016666666666666666;
    if (self->timer > 5.0f) {
        self->count++;
        if (self->count > 12) {
            self->count = 12;
        }
        fn_1_12EFC(self);
        self->timer = 0.0f;
    }
}

void fn_1_12EF8(void)
{
}

// Raise idle choros at free spots until count are up.
void fn_1_12EFC(M517ChoroMgr *self)
{
    M517Node *end;
    M517Choro *c;
    s32 started;
    s32 idx;
    Vec pos;

    M517_ASSERT(lbl_1_bss_A1C != 0, lbl_1_data_5B8, lbl_1_data_5C4, 0x2E);
    end = lbl_1_bss_A1C;
    c = (M517Choro *)end->next;
    started = 0;
    idx = 0;
    while ((M517Node *)c != end && started < self->count) {
        if (c->active == 0 && fn_1_13214(self, end->next, end, &pos) != 0) {
            self->spots[idx] = pos;
            fn_1_111AC(c, 0, (f32)(s32)(fn_1_6598() & 3));
            fn_1_11048(c, &pos);
            started++;
        }
        c = (M517Choro *)c->link.next;
        idx++;
    }
}

// Find a spot at least 200 units from every running choro (5 tries).
u8 fn_1_13214(M517ChoroMgr *self, M517Node *first, M517Node *end, Vec *out)
{
    M517Stage *stage;
    s32 tries;

    M517_ASSERT(lbl_1_bss_10 != 0, lbl_1_data_5E8, lbl_1_data_5F8, 0x27);
    stage = (M517Stage *)lbl_1_bss_10;
    (void)stage;
    for (tries = 0; tries < 5; tries++) {
        Vec cand;
        u8 ok;
        s32 idx;
        M517Choro *c;

        cand.x = 400.0f * (0.000030517578f * ((f32)(u16)frand() - 32768.0f));
        cand.y = 0.0f;
        cand.z = 400.0f * (0.000030517578f * ((f32)(u16)frand() - 32768.0f));
        ok = 1;
        idx = 0;
        for (c = (M517Choro *)first; (M517Node *)c != end; c = (M517Choro *)c->link.next, idx++) {
            if (c->active != 0) {
                Vec d;

                PSVECSubtract(&self->spots[idx], &cand, &d);
                if (PSVECMag(&d) < 200.0f) {
                    ok = 0;
                }
            }
        }
        if (ok == 1) {
            out->x = cand.x;
            out->y = cand.y;
            out->z = cand.z;
            return 1;
        }
    }
    return 0;
}

// Stop every choro.
void fn_1_1342C(M517ChoroMgr *self)
{
    M517Node *end;
    M517Choro *c;

    M517_ASSERT(lbl_1_bss_A1C != 0, lbl_1_data_5B8, lbl_1_data_5C4, 0x2E);
    end = lbl_1_bss_A1C;
    c = (M517Choro *)end->next;
    while ((M517Node *)c != end) {
        fn_1_11368(c, 0, 0.0f);
        c = (M517Choro *)c->link.next;
    }
}

} // extern "C"
