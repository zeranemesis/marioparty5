#include "REL/m531Dll.h"

extern "C" {
void PSMTXMultVec(const Mtx m, const Vec *src, Vec *dst);

void *__dl__FPv(void *ptr);
void *__ct__Q24m5316CStageFP9Process_sissUs(void *self, OMOBJMAN *objMan, int dataNum, s16 layer, s16 prio, u16 camera);
void *__dt__Q24m5316CStageFv(void *self, s16 flag);
void setShadow__Q24m5316CStageFv(void *self);
void on_update__Q24m5316CStageFv(CStageObj *self);

void *fn_1_0(s32 size);
void fn_1_1D4(const char *expr, const char *file, int line);
void *fn_1_5D28(void *self, s32 dataNum, s32 maxCnt, Vec *pos, Vec *rot);
void *fn_1_DAEC(void *self, s32 arg);
void fn_1_F6AC(void *self, Vec *pos);
void fn_1_F6D4(void *self, f32 value);
extern void *lbl_1_bss_1C;

HU3DMODELID Hu3DModelCreate(void *data);
void Hu3DModelHookSet(HU3DMODELID modelId, const char *name, s16 hookModel);
void Hu3DModelShadowMapSet(HU3DMODELID modelId);
void Hu3DModelObjMtxGet(HU3DMODELID modelId, const char *name, Mtx mtx);
void CharEffectHitCreate(s32 type, Vec *pos, Vec *rot);
}

class CEffectParticle {
public:
    void *m_anim;
    s16 m_model;
    void *m_work;
    virtual ~CEffectParticle();
};

// The stage set: the two stage models, the dust effect, eight clouds and five pots.
struct CStageSet {
    CStageObj *m_a;
    CStageObj *m_b;
    CEffectParticle *m_fx;
    CStageObj *m_cloud[8]; // 0x0C
    CStageObj *m_pot[5]; // 0x2C
};

// Pot (vtable lbl_1_data_324): a state machine driven by the dust cloud values.
struct CObjAnim {
    CStageObj m_base;
    s32 m_state; // 0x34
    Vec m_anim; // 0x38, scale animation
    Vec m_hold; // 0x44
    s32 m_frame; // 0x50
    s32 m_index; // 0x54
    s32 m_phase; // 0x58
    u8 m_flag; // 0x5C
};

// Cloud (vtable lbl_1_data_314).
struct CObjCloud {
    CStageObj m_base;
};

// Moving pot (vtable lbl_1_data_334): the base stage object plus its hook model.
struct CObjPot {
    CStageObj m_base;
    s16 m_hook; // 0x34
};

extern "C" {
CStageSet *fn_1_7BB0(CStageSet *self, OMOBJMAN *objMan);
CStageSet *fn_1_7E40(CStageSet *self, s16 flag);
CObjCloud *fn_1_7F94(CObjCloud *self, s16 flag);
CObjPot *fn_1_7FF8(CObjPot *self, s16 flag);
CObjAnim *fn_1_805C(CObjAnim *self, OMOBJMAN *objMan, s32 dataNum, s32 index);
void fn_1_8124(CObjAnim *self);
void fn_1_8190(CObjAnim *self);
void fn_1_81F4(CObjAnim *self);
void fn_1_8410(CObjAnim *self);
void fn_1_8750(CObjAnim *self);
CObjPot *fn_1_8CD4(CObjPot *self, OMOBJMAN *objMan, s32 idx, s32 modelSel, Vec *pos, Vec *rot, u8 flag);
CObjCloud *fn_1_8E88(CObjCloud *self, OMOBJMAN *objMan, Vec *pos);
void fn_1_8FA0(CObjCloud *self);
CObjAnim *fn_1_9064(CObjAnim *self, s16 flag);

extern StageObjVt lbl_1_data_314;
extern StageObjVt lbl_1_data_324;
extern StageObjVt lbl_1_data_334;
}

namespace m531 {
class CStage {
public:
    void on_update();
};

void CStage::on_update()
{
}
} // namespace m531

static Vec lbl_1_data_1D0[8] = {
    { -800.0f, -1200.0f, -300.0f },
    { -400.0f, -1200.0f, 0.0f },
    { 0.0f, -1200.0f, 200.0f },
    { 400.0f, -1200.0f, 400.0f },
    { 800.0f, -1200.0f, 600.0f },
    { -300.0f, -1200.0f, 800.0f },
    { -200.0f, -1200.0f, 200.0f },
    { -400.0f, -1200.0f, 300.0f },
};

struct PotPlace {
    Vec m_pos;
    Vec m_rot;
};

static PotPlace lbl_1_data_230[5] = {
    { { -722.0f, -389.0f, -53.0f }, { 0.0f, 59.4f, 0.0f } },
    { { 687.0f, -406.0f, 145.0f }, { 0.0f, -69.5f, 0.0f } },
    { { -13.0f, -582.0f, 433.0f }, { 0.0f, -120.0f, 0.0f } },
    { { -711.0f, -582.0f, 405.0f }, { 0.0f, -226.0f, 0.0f } },
    { { 877.0f, -582.0f, -205.0f }, { 0.0f, -22.4f, 0.0f } },
};

static s32 lbl_1_data_2A8[5] = { 1, 0, 0, 0, 1 };
static u8 lbl_1_data_2BC[5] = { 0, 0, 1, 0, 0 };

static const s32 kPotMotion[2] = { 0x590012, 0x590013 };

CStageSet *fn_1_7BB0(CStageSet *self, OMOBJMAN *objMan)
{
    s32 i;
    void *obj;
    Vec fxPos = { -732.0f, -222.0f, -185.0f };
    Vec fxRot = { 0.0f, 45.0f, 0.0f };
    void *fx;

    for (i = 0; i < 8; i++) {
        obj = fn_1_0(0x34);
        if (obj != NULL) {
            obj = fn_1_8E88((CObjCloud *)obj, objMan, &lbl_1_data_1D0[i]);
        }
        self->m_cloud[i] = (CStageObj *)obj;
    }
    obj = fn_1_0(0x34);
    if (obj != NULL) {
        obj = __ct__Q24m5316CStageFP9Process_sissUs(obj, objMan, 0x590000, 1, 4, 1);
    }
    self->m_a = (CStageObj *)obj;
    Hu3DModelShadowMapSet(self->m_a->m_model);
    obj = fn_1_0(0x34);
    if (obj != NULL) {
        obj = __ct__Q24m5316CStageFP9Process_sissUs(obj, objMan, 0x590001, 1, 4, 1);
    }
    self->m_b = (CStageObj *)obj;
    for (i = 0; i < 5; i++) {
        obj = fn_1_0(0x38);
        if (obj != NULL) {
            Vec rot = lbl_1_data_230[i].m_rot;
            Vec pos = lbl_1_data_230[i].m_pos;
            obj = fn_1_8CD4((CObjPot *)obj, objMan, i, lbl_1_data_2A8[i], &pos, &rot, lbl_1_data_2BC[i]);
        }
        self->m_pot[i] = (CStageObj *)obj;
    }
    fx = fn_1_0(0x10);
    if (fx != NULL) {
        Vec r = fxRot;
        Vec p = fxPos;
        fx = fn_1_5D28(fx, 0x590003, 0xC, &p, &r);
    }
    self->m_fx = (CEffectParticle *)fx;
    return self;
}

CStageSet *fn_1_7E40(CStageSet *self, s16 flag)
{
    s32 i;

    if (self != NULL) {
        for (i = 0; i < 5; i++) {
            CStageObj *obj = self->m_pot[i];
            if (obj != NULL) {
                obj->m_vt->m_dtor(obj, 1);
            }
        }
        {
            CStageObj *obj = self->m_a;
            if (obj != NULL) {
                obj->m_vt->m_dtor(obj, 1);
            }
        }
        {
            CStageObj *obj = self->m_b;
            if (obj != NULL) {
                obj->m_vt->m_dtor(obj, 1);
            }
        }
        delete self->m_fx;
        for (i = 0; i < 8; i++) {
            CStageObj *obj = self->m_cloud[i];
            if (obj != NULL) {
                obj->m_vt->m_dtor(obj, 1);
            }
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CObjCloud *fn_1_7F94(CObjCloud *self, s16 flag)
{
    if (self != NULL) {
        self->m_base.m_vt = &lbl_1_data_314;
        __dt__Q24m5316CStageFv(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CObjPot *fn_1_7FF8(CObjPot *self, s16 flag)
{
    if (self != NULL) {
        self->m_base.m_vt = &lbl_1_data_334;
        __dt__Q24m5316CStageFv(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CObjAnim *fn_1_805C(CObjAnim *self, OMOBJMAN *objMan, s32 dataNum, s32 index)
{
    __ct__Q24m5316CStageFP9Process_sissUs(self, objMan, dataNum, 3, 4, 1);
    self->m_base.m_vt = &lbl_1_data_324;
    self->m_state = 0;
    self->m_frame = 0;
    self->m_index = index;
    self->m_phase = 0;
    self->m_flag = 0;
    setShadow__Q24m5316CStageFv(self);
    Hu3DModelAttrSet(self->m_base.m_model, 0x40000080);
    self->m_anim.z = 1.0f;
    self->m_anim.y = 1.0f;
    self->m_anim.x = 1.0f;
    return self;
}

void fn_1_8124(CObjAnim *self)
{
    s16 model = self->m_base.m_model;
    Hu3DModelPosSet(model, self->m_base.m_pos.x, self->m_base.m_pos.y, self->m_base.m_pos.z);
    Hu3DModelRotSet(model, self->m_base.m_rot.x, self->m_base.m_rot.y, self->m_base.m_rot.z);
}

static inline s16 StageGetModel(CObjAnim *self)
{
    return self->m_base.m_model;
}

void fn_1_8190(CObjAnim *self)
{
    if (self->m_state == 0) {
        HuAudFXPlay(0x7A3);
        Hu3DModelAttrReset(StageGetModel(self), 0x40000080);
        self->m_state = 1;
    }
}

static inline void *CleanGet(void)
{
    if (lbl_1_bss_1C != 0) {
    } else {
        fn_1_1D4("ms_pointer != 0", "singleton.h", 0x27);
    }
    return lbl_1_bss_1C;
}

static inline f32 MinF(f32 a, f32 b)
{
    if (a < b) {
        return a;
    }
    return b;
}

static inline void Obj81F4(CObjAnim *self)
{
    Mtx mtx;
    Vec v = { 0.0f, 100.0f, 45.0f };
    f32 scale;
    s16 model;
    void *clean;

    model = StageGetModel(self);
    clean = fn_1_DAEC(CleanGet(), 0);
    Hu3DModelObjMtxGet(model, "guruguru6-cyl1", mtx);
    PSMTXMultVec(mtx, &v, &v);
    Hu3DModelObjMtxGet(model, "guruguru6-scale_dmy", mtx);
    scale = MinF(mtx[1][1], 1.0f);
    if (self->m_index == 0) {
        fn_1_F6AC(clean, &v);
        fn_1_F6D4(clean, scale);
        if (self->m_flag == 0 && v.y < 28.0f) {
            HuAudFXPlay(0x7A4);
            HuAudFXPlay(0x7A5);
            self->m_flag = 1;
        }
    }
    if (scale >= 1.0f) {
        self->m_state = 2;
        self->m_hold.x = self->m_base.m_pos.x;
        self->m_hold.y = self->m_base.m_pos.y;
        self->m_hold.z = self->m_base.m_pos.z;
        Hu3DModelAttrSet(self->m_base.m_model, 0x40000080);
    }
}

void fn_1_81F4(CObjAnim *self)
{
    Obj81F4(self);
}

static inline void Obj8410(CObjAnim *self)
{
    Vec hit;
    s16 model;

    model = self->m_base.m_model;
    switch (self->m_phase) {
        case 0:
            self->m_anim.x = self->m_anim.x + ((0.2f - self->m_anim.x) / 6.0f);
            self->m_anim.y = self->m_anim.y + ((0.2f - self->m_anim.y) / 6.0f);
            self->m_anim.z = self->m_anim.z + ((0.2f - self->m_anim.z) / 6.0f);
            if (self->m_anim.x > 0.3f) {
                break;
            }
            self->m_phase = self->m_phase + 1;
        case 1:
            self->m_anim.x = self->m_anim.x + ((1.5f - self->m_anim.x) * 0.5f);
            self->m_anim.y = self->m_anim.y + ((1.0f - self->m_anim.y) * 0.5f);
            self->m_anim.z = self->m_anim.z + ((0.5f - self->m_anim.z) * 0.5f);
            if (self->m_anim.x < 1.3) {
                break;
            }
            self->m_phase = self->m_phase + 1;
        case 2:
            self->m_anim.x = self->m_anim.x + (-self->m_anim.x / 5.0f);
            self->m_anim.y = self->m_anim.y + (-self->m_anim.y / 5.0f);
            self->m_anim.z = self->m_anim.z + (-self->m_anim.z / 5.0f);
            if (self->m_anim.x > 0.1f) {
                break;
            }
            self->m_anim.z = 0.0f;
            self->m_anim.y = 0.0f;
            self->m_anim.x = 0.0f;
            self->m_state = 3;
            hit.x = self->m_base.m_pos.x;
            hit.y = 22.5f + self->m_base.m_pos.y;
            hit.z = 50.0f + self->m_base.m_pos.z;
            CharEffectHitCreate(1, &hit, &self->m_base.m_rot);
            self->m_phase = self->m_phase + 1;
            break;
    }
    self->m_base.m_pos.y = self->m_hold.y + (22.5f * (1.0f - self->m_anim.y));
    self->m_base.m_pos.z = self->m_hold.z + (50.0f * (1.0f - self->m_anim.z));
    Hu3DModelScaleSet(model, self->m_anim.x, self->m_anim.y, self->m_anim.z);
}

void fn_1_8410(CObjAnim *self)
{
    Obj8410(self);
}

void fn_1_8750(CObjAnim *self)
{
    s16 model = self->m_base.m_model;
    Hu3DModelPosSet(model, self->m_base.m_pos.x, self->m_base.m_pos.y, self->m_base.m_pos.z);
    Hu3DModelRotSet(model, self->m_base.m_rot.x, self->m_base.m_rot.y, self->m_base.m_rot.z);
    switch (self->m_state) {
        case 1:
            Obj81F4(self);
            break;
        case 2:
            Obj8410(self);
            break;
    }
    self->m_frame = self->m_frame + 1;
}

CObjPot *fn_1_8CD4(CObjPot *self, OMOBJMAN *objMan, s32 idx, s32 modelSel, Vec *pos, Vec *rot, u8 flag)
{
    s16 model;
    HU3DMODEL *data;

    __ct__Q24m5316CStageFP9Process_sissUs(self, objMan, 0x590011, 3, 4, 1);
    self->m_base.m_vt = &lbl_1_data_334;
    model = self->m_base.m_model;
    Hu3DMotionSet(model, Hu3DJointMotion(model, HuDataSelHeapReadNum(kPotMotion[modelSel], 0x10000000, 2)));
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelPosSet(model, pos->x, pos->y, pos->z);
    Hu3DModelRotSet(model, rot->x, rot->y, rot->z);
    data = &Hu3DData[model];
    if (flag != 0) {
        data->ambR = 0.5f;
        data->ambG = 0.2f;
        data->ambB = 0.7f;
    } else {
        data->ambR = 0.7f;
        data->ambG = 0.7f;
        data->ambB = 0.65f;
    }
    self->m_hook = Hu3DModelCreate(HuDataSelHeapReadNum(0x590010, 0x10000000, 2));
    Hu3DModelHookSet(self->m_base.m_model, "g003m2-itemhook_R", self->m_hook);
    return self;
}

CObjCloud *fn_1_8E88(CObjCloud *self, OMOBJMAN *objMan, Vec *pos)
{
    s16 model;
    f32 scale;

    __ct__Q24m5316CStageFP9Process_sissUs(self, objMan, 0x59000A, 0, 4, 1);
    self->m_base.m_vt = &lbl_1_data_314;
    model = self->m_base.m_model;
    scale = 0.5f + (0.5f * (0.000015258789f * (f32)frand()));
    Hu3DModelScaleSet(model, scale, scale, scale);
    self->m_base.m_pos.x = pos->x;
    self->m_base.m_pos.y = pos->y;
    self->m_base.m_pos.z = pos->z;
    return self;
}

void fn_1_8FA0(CObjCloud *self)
{
    s16 model;

    self->m_base.m_pos.x = self->m_base.m_pos.x + 0.33333334f;
    if (self->m_base.m_pos.x > 1200.0f) {
        self->m_base.m_pos.x = -1200.0f;
    }
    model = self->m_base.m_model;
    Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelPosSet(model, self->m_base.m_pos.x, self->m_base.m_pos.y, self->m_base.m_pos.z);
}

CObjAnim *fn_1_9064(CObjAnim *self, s16 flag)
{
    if (self != NULL) {
        self->m_base.m_vt = &lbl_1_data_324;
        __dt__Q24m5316CStageFv(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

#define SVT(fn) ((void (*)(CStageObj *))fn)
#define SDT(fn) ((CStageObj * (*)(CStageObj *, s16)) fn)

StageObjVt lbl_1_data_314 = { { NULL, NULL }, SVT(fn_1_8FA0), SDT(fn_1_7F94) };
StageObjVt lbl_1_data_324 = { { NULL, NULL }, SVT(fn_1_8750), SDT(fn_1_9064) };
StageObjVt lbl_1_data_334 = { { NULL, NULL }, SVT(on_update__Q24m5316CStageFv), SDT(fn_1_7FF8) };
