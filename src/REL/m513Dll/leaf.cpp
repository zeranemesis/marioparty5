// m513Dll leaf: the leaf objects the heiho characters throw, a leaf-burst
// particle effect and the model pointer helpers.
#define _MATH_H
#include "REL/m513Dll.h"
extern "C" {
#include "game/frand.h"
}

#ifdef __MWERKS__
#define M513_RODATA __declspec(section ".rodata")
#else
#define M513_RODATA
#endif







inline HU3DMODELID M513LeafSubId(M513Leaf *self, s32 i)
{
    M513Model *m = self->sub[i].p;

    return m->id;
}

extern "C" {

void abort(void);
void __construct_array(void *ptr, void (*ctor)(void *), void *dtor, u32 size, u32 count);
void __destroy_arr(void *ptr, void *dtor, u32 size, u32 count);
void fn_1_145A8(s32 seId, Vec *pos);
void PSVECAdd(const Vec *a, const Vec *b, Vec *ab);

void fn_1_8E70(M513ModelPtr *self, M513Model *p);
void fn_1_8E6C(void);
void fn_1_77A4(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx);
M513LeafFx *fn_1_79EC(M513LeafFx *self, s16 flag);
void fn_1_7A80(M513LeafFx *self, s32 bit);
void fn_1_7B00(M513LeafFx *self, HU3DPARTICLEDATA *p);
void fn_1_7D8C(M513LeafFx *self, Mtx mtx);
M513ModelPtr *fn_1_8234(M513ModelPtr *self, s16 flag);
void fn_1_828C(M513ModelPtr *self);
void fn_1_8490(M513Leaf *self, s32 idx);
void fn_1_8578(M513Leaf *self);

M513_RODATA const s32 lbl_1_rodata_184[3] = { 0x470005, 0x470006, 0x470007 };
M513_RODATA const s32 lbl_1_rodata_194[4] = { 0x6D5, 0x6D6, 0x6D7, 0x6D8 };

char lbl_1_data_2B8[] = "ms_pointer != 0";
char lbl_1_data_2C8[] = "singleton.h";
M513Ptmf lbl_1_data_2D4 = { 0, -1, (void (*)(void *))fn_1_8E6C };
char lbl_1_data_2E0[] = "sm_ptr != 0";
char lbl_1_data_2EC[] = "a_set.h";
char lbl_1_data_2F4[] = "null1";
char lbl_1_data_2FA[] = "leaf01";
char lbl_1_data_301[] = "leaf02";
char lbl_1_data_308[8] = "leaf03";
char *lbl_1_data_310[3] = { lbl_1_data_2FA, lbl_1_data_301, lbl_1_data_308 };
void *lbl_1_data_31C[5] = { 0, 0, (void *)fn_1_79EC, (void *)fn_1_7A80, (void *)fn_1_22E0 };

M513Node *lbl_1_bss_9F8;
M513ModelPtr *lbl_1_bss_9FC;

void fn_1_77A4(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx)
{
    MtxPtr sp8 = mtx;

    fn_1_7D8C((M513LeafFx *)particle->work, sp8);
}

M513LeafFx *fn_1_77E8(M513LeafFx *self, Vec *pos)
{
    HU3DMODELID id;
    HU3DPARTICLEDATA *p;
    s32 i;

    fn_1_1768(&self->link);
    self->vtbl = (void **)&lbl_1_data_218;
    self->flags = 0;
    fn_1_1834(&self->link, &lbl_1_bss_C);
    self->vtbl = lbl_1_data_31C;
    self->pos.x = pos->x;
    self->pos.y = pos->y;
    self->pos.z = pos->z;
    M513_ASSERT(lbl_1_bss_9F0 != 0, lbl_1_data_2B8, lbl_1_data_2C8, 0x27);
    id = Hu3DParticleCreate(lbl_1_bss_9F0->subAnim[3], 0x40);
    self->particle = (HU3DPARTICLE *)Hu3DData[id].hookData;
    self->particle->work = self;
    Hu3DParticleHookSet(id, fn_1_77A4);
    Hu3DParticleColSet(id, 0xFF, 0xFF, 0x80);
    Hu3DParticleTPLvlSet(id, 0.0f);
    Hu3DParticleBlendModeSet(id, 0);
    Hu3DModelLayerSet(id, 5);
    Hu3DModelCameraSet(id, 1);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    self->modelId = id;
    p = self->particle->data;
    for (i = 0; i < (s16)self->particle->maxCnt; i++, p++) {
        fn_1_7B00(self, p);
    }
    return self;
}

M513LeafFx *fn_1_79EC(M513LeafFx *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_31C;
        self->particle->work = 0;
        Hu3DModelKill(self->modelId);
        if (self != 0) {
            self->vtbl = (void **)&lbl_1_data_218;
            if (self != 0) {
                fn_1_1774(&self->link, 0);
            }
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// Virtual update(): bit 0 .. clear the flag; delete the effect once it is done.
void fn_1_7A80(M513LeafFx *self, s32 bit)
{
    self->flags &= ~(1 << bit);
    if ((self->done != 0) && (self != 0)) {
        ((M513TaskVtbl *)self->vtbl)->dtor((M513Task *)self, 1);
    }
}

// Restart one leaf particle.
#define M513_LEAF_INIT(self, p)                                                                    \
    (p)->time = 0;                                                                                 \
    (p)->parManId = (frand() & 0xF) + 4;                                                           \
    (p)->pos.x = (self)->pos.x;                                                                    \
    (p)->pos.y = (self)->pos.y;                                                                    \
    (p)->pos.z = (self)->pos.z;                                                                    \
    (p)->scale = 0.5f * (100.0f * (0.1f + (0.2f * (0.000015258789f * (f32)(u16)frand()))));        \
    (p)->zRot = 0.017453292f * (360.0f * (0.000015258789f * (f32)(u16)frand()));                   \
    (p)->vel.x = 0.27240694444444447 * (f64)(14.0f * (0.000030517578f * ((f32)(u16)frand() - 32768.0f))); \
    (p)->vel.y = 0.27240694444444447 * (f64)(14.0f * (0.000030517578f * ((f32)(u16)frand() - 32768.0f))); \
    (p)->vel.z = 0.27240694444444447 * -(14.0f * (0.000015258789f * (f32)(u16)frand()));           \
    (p)->color.a = 0

void fn_1_7B00(M513LeafFx *self, HU3DPARTICLEDATA *p)
{
    M513_LEAF_INIT(self, p);
}

// Particle hook: advance the leaf burst.
void fn_1_7D8C(M513LeafFx *self, Mtx mtx)
{
    HU3DMODEL *model = &Hu3DData[self->modelId];
    HU3DPARTICLE *particle = self->particle;
    HU3DPARTICLEDATA *p;
    s32 i;

    self->done = 1;
    p = particle->data;
    for (i = 0; i < (s16)particle->maxCnt; i++, p++) {
        if ((s16)p->time != 0xFF) {
            if ((s16)p->time == 0) {
                p->color.a = p->color.a + 0x10;
                if (p->color.a > 0xEB) {
                    p->color.a = 0xFF;
                    p->time++;
                }
            } else {
                p->color.a = p->color.a - p->parManId;
                if (p->color.a < 0x14) {
                    p->color.a = 0;
                    p->time = 0xFF;
                }
            }
            p->zRot = p->zRot + 0.017453292f;
            p->vel.x = p->vel.x * 0.99f;
            p->vel.y = p->vel.y * 0.99f;
            p->vel.z = p->vel.z * 0.99f;
            p->pos.x = p->pos.x + p->vel.x;
            p->pos.y = p->pos.y + p->vel.y;
            p->pos.z = p->pos.z + p->vel.z;
            self->done = 0;
        }
    }
}

M513Leaf *fn_1_7F04(M513Leaf *self, s32 idx, Vec *pos, Vec *rot)
{
    HU3DMODELID id;
    s32 i;

    fn_1_1768(&self->link);
    fn_1_3198(&self->model, 0x470004);
#ifdef TARGET_PC
    for (i = 0; i < 2; i++) {
        fn_1_8E70(&self->sub[i], 0);
    }
#else
    __construct_array(self->sub, (void (*)(void *))fn_1_828C, (void *)fn_1_8234, 4, 2);
#endif
    self->time = 0.0f;
    self->pos.x = pos->x;
    self->pos.y = pos->y;
    self->pos.z = pos->z;
    self->rot.x = rot->x;
    self->rot.y = rot->y;
    self->rot.z = rot->z;
    self->flag = 0;
    self->idx = idx;
    self->step = 0;
    self->frac = 0.0f;
    M513_ASSERT(lbl_1_bss_9F8 != 0, lbl_1_data_2E0, lbl_1_data_2EC, 0x2E);
    fn_1_1834(&self->link, lbl_1_bss_9F8);
    id = self->model.id;
    Hu3DModelLayerSet(id, 3);
    Hu3DModelCameraSet(id, 1);
    Hu3DModelAttrSet(id, 1);
    Hu3DModelShadowSet(id);
    Hu3DModelShadowDispOn(id);
    fn_1_33F8(&self->model, &self->pos);
    fn_1_3440(&self->model, &self->rot);
    Hu3DMotionShapeSet(id, Hu3DMotionIDGet(id));
    Hu3DMotionShapeStartEndSet(id, 0.0f, self->time);
    Hu3DMotionShapeSpeedSet(id, 0.2f);
    for (i = 0; i < 2; i++) {
        M513Model *m = (M513Model *)fn_1_0(sizeof(M513Model));
        HU3DMODELID sid;

        if (m != 0) {
            m = fn_1_3198(m, lbl_1_rodata_184[i]);
        }
        if (self->sub[i].p != m) {
            fn_1_35F8(self->sub[i].p, 1);
            self->sub[i].p = m;
        }
        sid = self->sub[i].p->id;
        Hu3DMotionShapeSet(sid, Hu3DMotionIDGet(sid));
        Hu3DMotionShapeTimeSet(sid, 0.0f);
        Hu3DModelAttrSet(sid, 0x40000080);
        Hu3DModelAttrSet(sid, 1);
        Hu3DModelLayerSet(sid, 3);
        Hu3DModelCameraSet(sid, 1);
        Hu3DModelShadowSet(sid);
        Hu3DModelShadowDispOn(sid);
    }
    fn_1_8578(self);
    self->hook = lbl_1_data_2D4;
    return self;
}

M513ModelPtr *fn_1_8234(M513ModelPtr *self, s16 flag)
{
    if (self != 0) {
        fn_1_35F8(self->p, 1);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_828C(M513ModelPtr *self)
{
    fn_1_8E70(self, 0);
}

M513Leaf *fn_1_82B8(M513Leaf *self, s16 flag)
{
    if (self != 0) {
        fn_1_1774(&self->link, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M513Leaf *fn_1_8310(M513Leaf *self, s16 flag)
{
    if (self != 0) {
#ifdef TARGET_PC
        fn_1_8234(&self->sub[1], 0);
        fn_1_8234(&self->sub[0], 0);
#else
        __destroy_arr(self->sub, (void *)fn_1_8234, 4, 2);
#endif
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

// Hide the leaf and its two sub models.
void fn_1_8394(M513Leaf *self)
{
    s32 i;
    s16 mid = self->model.id;

    Hu3DModelAttrReset(mid, 1);
    for (i = 0; i < 2; i++) {
        Hu3DModelAttrReset(M513LeafSubId(self, i), 1);
        fn_1_8490(self, i);
    }
}

void fn_1_8428(M513Leaf *self, s32 idx)
{
    Hu3DModelAttrReset(M513LeafSubId(self, idx), 0x40000080);
}

// Place sub model `idx` on its hook object.
void fn_1_8490(M513Leaf *self, s32 idx)
{
    Mtx mtx;
    Vec pos;
    Vec rot;
    HSFOBJECT *obj = self->obj[idx];

    Hu3DModelObjMtxGet(self->model.id, lbl_1_data_2F4, mtx);
    pos.x = obj->mesh.curr.pos.x;
    pos.y = obj->mesh.curr.pos.y;
    pos.z = obj->mesh.curr.pos.z;
    rot.x = obj->mesh.curr.rot.x;
    rot.y = obj->mesh.curr.rot.y;
    rot.z = obj->mesh.curr.rot.z;
    pos.z = -pos.z;
    PSVECAdd(&self->pos, &pos, &pos);
    rot.y = 180.0f;
    fn_1_33F8(self->sub[idx].p, &pos);
    fn_1_3440(self->sub[idx].p, &rot);
}

void fn_1_8578(M513Leaf *self)
{
    HU3DMODELID id = self->model.id;
    s32 i;

    for (i = 0; i < 2; i++) {
        self->obj[i] = Hu3DModelObjPtrGet(id, lbl_1_data_310[i]);
    }
}

// Advance the leaf animation by `speed` frames.
void fn_1_85F4(M513Leaf *self, f32 speed)
{
    HU3DMODELID id;
    f32 next;
    f32 cur;
    f32 diff;
    s32 i;

    if (self->flag != 1) {
        id = self->model.id;
        next = self->time + speed;
        cur = Hu3DData[id].motShapeWork.time;
        if ((next - cur) < 0.0f) {
            diff = -next - cur;
        } else {
            diff = next - cur;
        }
        if (!(diff > 0.6f)) {
            self->time = next;
            if (self->time >= 60.0f) {
                self->time = 60.0f;
            }
            Hu3DMotionShapeStartEndSet(id, 0.0f, self->time);
            self->frac = cur / 60.0f;
            if (self->step < 2) {
                if (cur >= (20.0f * (f32)(self->step + 1))) {
                    s32 other = 2 - (self->step + 1);
                    Vec p;

                    Hu3DModelAttrReset(self->sub[other].p->id, 0x40000080);
                    p.x = self->pos.x;
                    p.y = self->pos.y;
                    p.z = self->pos.z;
                    fn_1_145A8(lbl_1_rodata_194[self->step], &p);
                    self->step++;
                }
            }
            for (i = 0; i < 2; i++) {
                fn_1_8490(self, i);
            }
            if ((cur >= 59.0f) && (self->flag == 0)) {
                Vec p;
                Vec at;
                M513LeafFx *fx;

                self->flag = 1;
                p.x = self->pos.x;
                p.y = self->pos.y;
                p.z = self->pos.z;
                fn_1_145A8(0x6D9, &p);
                at.x = self->pos.x;
                at.y = self->pos.y;
                at.z = self->pos.z;
                at.y += 380.0f;
                at.z = at.z + 100.0f;
                fx = (M513LeafFx *)fn_1_0(sizeof(M513LeafFx));
                if (fx != 0) {
                    fx = fn_1_77E8(fx, &at);
                }
                Hu3DMotionShapeSpeedSet(id, 1.0f);
                Hu3DMotionShapeStartEndSet(id, 0.0f, 100.0f);
            }
        }
    }
}

void fn_1_8E24(M513Leaf *self)
{
    if (M513_PTMF_TEST(&self->hook)) {
        M513_PTMF_CALL(&self->hook, self);
    }
}

void fn_1_8E6C(void)
{
}

void fn_1_8E70(M513ModelPtr *self, M513Model *p)
{
    self->p = p;
}

}
