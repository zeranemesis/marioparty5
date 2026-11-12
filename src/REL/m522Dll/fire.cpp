#define _MATH_H
#include "REL/m522Dll.h"
#include "REL/m522Vec.h"

#include "game/gxarray.h"
#include "game/hu3d.h"

#ifdef __MWERKS__
#define FIRE_RODATA __declspec(section ".rodata")
#else
#define FIRE_RODATA
#endif

extern "C" {
void fn_1_3498(void);
void fn_1_4390(M522Vec<M522_VEC3> *self, const M522_VEC3 *first, const M522_VEC3 *last);
void fn_1_4504(M522Vec<M522_VEC3> *self, const M522_VEC3 *first, const M522_VEC3 *last);
}

/* One position of a trail, and the trail itself: a ring of 8 points plus the
 * velocity it is drifting with. */
class M522Trail {
public:
    M522Vec<M522_VEC3> pts;
    M522_VEC3 vel;
    f32 total;
    f32 remain;
    u32 index;

    M522Trail() {}
    M522Trail(const M522Trail &o)
    {
        pts.m_cap = 0;
        pts.m_size = 0;
        pts.m_data = 0;
        fn_1_4390(&pts, o.pts.m_data, o.pts.m_data + o.pts.m_size);
        vel = o.vel;
        total = o.total;
        remain = o.remain;
        index = o.index;
    }
    M522Trail &operator=(const M522Trail &o)
    {
        if (&pts != &o.pts) {
            fn_1_4504(&pts, o.pts.m_data, o.pts.m_data + o.pts.m_size);
        }
        vel = o.vel;
        total = o.total;
        remain = o.remain;
        index = o.index;
        return *this;
    }
};

inline void m522Destroy(M522Trail *t)
{
    M522_VEC3 *first = t->pts.begin();
    M522_VEC3 *last = first + t->pts.m_size;

    while (last > first) {
        last--;
        fn_1_3498();
    }
    t->pts.m_size = 0;
    if (t->pts.m_data != 0) {
        M522_VEC3 *data = t->pts.begin();
        fn_1_6964(data);
    }
}

typedef M522Vec<M522_FIREWAVE> M522_WAVES;
typedef M522Vec<M522_SPARK> M522_SPARKS;
typedef M522Vec<M522_VEC3> M522_POINTS;
typedef M522Vec<M522Trail> M522_TRAILS;

/* The burst effect: effect base followed by its trails. */
typedef struct M522Burst_s {
    /* 0x00 */ M522_EFFECT base;
    /* 0x14 */ M522_TRAILS trails;
} M522_BURST;

extern "C" {
extern M522_FIRE *lbl_1_bss_10;
extern M522_NODE lbl_1_bss_2C;

void GXSetVtxAttrFmtv(GXVtxFmt fmt, const GXVtxAttrFmtList *list);
double sin(double x);
double cos(double x);
void DCFlushRangeNoSync(void *addr, u32 size);
void fn_1_6A70(const char *cond, const char *file, s32 line);
M522_MODEL *fn_1_9A34(M522_MODEL *model, s32 dataNum);
void fn_1_9BA8(M522_MODEL *model, u32 attr);
void fn_1_9CDC(M522_MODEL *model, M522_VEC3 *rot);
void fn_1_9DEC(M522_MODEL *model, s32 layer);
void fn_1_9E94(M522_MODEL *model, s16 flag);
void fn_1_9FE4(M522_MODEL *model, char *name);
f32 fn_1_BABC(void);
f32 fn_1_BD68(f32 min, f32 max);
void fn_1_E11C(s32 index, s32 bmpNo, s32 texMapId, s32 wrapS, s32 wrapT);
M522_EFFECT *fn_1_E44C(M522_EFFECT *self);
void fn_1_E534(M522_EFFECT *self, s16 flag);

M522_FIRE *fn_1_0(M522_FIRE *self);
void *fn_1_21C(M522_FIRE *self, s16 flag);
void *fn_1_27C(M522_FIRE *self, s16 flag);
void *fn_1_2D4(M522_FIRE *self, s16 flag);
void *fn_1_360(M522_FIRE *self, s16 flag);
void fn_1_428(M522_FIRE *self);
void fn_1_460(M522_FIRE *self);
void fn_1_474(M522_FIRE *self);
void fn_1_4A4(M522_FIRE *self);
void fn_1_510(M522_FIRE *self);
f32 fn_1_614(M522_FIREWAVE *w);
void fn_1_628(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
M522_FIREEFF *fn_1_824(M522_FIREEFF *self);
void *fn_1_900(M522_FIREEFF *self, s16 flag);
void *fn_1_960(M522_FIREEFF *self, s16 flag);
void *fn_1_9B8(M522_MODEL **ptr, s16 flag);
void *fn_1_A10(M522_FIREEFF *self, s16 flag);
void fn_1_AB0(M522_FIREEFF *self, s32 kind);
M522_SPARK *fn_1_B88(M522_SPARK *spark, s32 kind);
void fn_1_C2C(M522_SPARK *dst, const M522_SPARK *src);
void fn_1_C50(M522_SPARK *spark);
void fn_1_D50(M522_SPARK *spark, f32 *u0, f32 *v0, f32 *u1, f32 *v1);
void fn_1_F84(M522_FIREEFF *self);
void fn_1_10C8(M522_FIREEFF *self, Mtx *camera);
void fn_1_12C0(M522_FIREEFF *self, s32 kind, Mtx camera, Mtx rot, M522_SPARK **cur);
M522_BURST *fn_1_17E8(M522_BURST *self, M522_VEC3 *pos, s32 count, f32 len, f32 time);
void *fn_1_1A18(M522Trail *self, s16 flag);
void *fn_1_1A80(M522_TRAILS *self, s16 flag);
void *fn_1_1AE0(M522_TRAILS *self, s16 flag);
void *fn_1_1B38(M522_BURST *self, s16 flag);
M522Trail *fn_1_1BC0(M522Trail *self, M522_VEC3 *pos, M522_VEC3 *vel, f32 len);
void *fn_1_1C78(M522Trail *self, s16 flag);
void *fn_1_1CD8(M522Trail *self, s16 flag);
s32 fn_1_1D30(M522Trail *self);
void fn_1_1FE4(M522_BURST *self);
void fn_1_232C(M522_BURST *self, Mtx *camera);
void fn_1_24FC(M522Trail *trail);
void fn_1_26A4(void);
void fn_1_26A8(M522_WAVES *self, u32 n, M522_FIREWAVE *x);
void fn_1_27E8(M522_SPARKS *self, M522_SPARK *pos, u32 n, M522_SPARK *x);
void fn_1_2F88(M522_POINTS *self, u32 n, M522_VEC3 *x);
void fn_1_32D8(M522_TRAILS *self, M522Trail *pos);
void fn_1_3498(void);
void fn_1_349C(M522_TRAILS *self, M522Trail *pos, u32 n, M522Trail *x);
void *fn_1_4014(M522_WAVES *self, s16 flag);
void *fn_1_40A0(M522_SPARKS *self, s16 flag);
void fn_1_412C(M522_POINTS *self);
void *fn_1_4188(M522_POINTS *self, s16 flag);
void fn_1_422C(M522_TRAILS *self);
void *fn_1_4304(M522_TRAILS *self, s16 flag);
void fn_1_4390(M522_POINTS *self, const M522_VEC3 *first, const M522_VEC3 *last);
void fn_1_4504(M522_POINTS *self, const M522_VEC3 *first, const M522_VEC3 *last);
void fn_1_487C(M522_WAVES *self);
void fn_1_48D8(M522_SPARKS *self);
void fn_1_4934(void);
}

/* The singleton.h asserts and the model names. */
char lbl_1_data_0[] = "ms_pointer == 0";
char lbl_1_data_10[] = "singleton.h";
char lbl_1_data_1C[] = "ms_pointer != 0";
char lbl_1_data_2C[] = "sun01";
char lbl_1_data_32[] = "fire01";
char lbl_1_data_39[] = "prominence";
char *lbl_1_data_44[2] = { lbl_1_data_32, lbl_1_data_39 };

M522_VTFN lbl_1_data_4C[] = {
    (M522_VTFN)0,
    (M522_VTFN)0,
    (M522_VTFN)fn_1_1B38,
    (M522_VTFN)fn_1_1FE4,
    (M522_VTFN)fn_1_232C,
    (M522_VTFN)fn_1_26A4,
};

M522_VTFN lbl_1_data_64[] = {
    (M522_VTFN)0,
    (M522_VTFN)0,
    (M522_VTFN)fn_1_A10,
    (M522_VTFN)fn_1_F84,
    (M522_VTFN)fn_1_10C8,
    (M522_VTFN)fn_1_26A4,
};

/* Static state: the spin step, the default wave, and the singleton. */
f32 lbl_1_bss_0;
M522_FIREWAVE lbl_1_bss_4;
M522_FIRE *lbl_1_bss_10;
void *lbl_1_bss_14;

extern "C" {
FIRE_RODATA extern const M522_VEC3 lbl_1_rodata_38;
FIRE_RODATA extern const f32 lbl_1_rodata_50[6];
FIRE_RODATA extern const s32 lbl_1_rodata_78[2];
FIRE_RODATA extern const GXVtxDescList lbl_1_rodata_98[3];
FIRE_RODATA extern const GXVtxAttrFmtList lbl_1_rodata_B0[3];
FIRE_RODATA extern const M522_VEC3 lbl_1_rodata_F8;
FIRE_RODATA extern const GXVtxDescList lbl_1_rodata_120[3];
FIRE_RODATA extern const GXVtxAttrFmtList lbl_1_rodata_138[3];

FIRE_RODATA const M522_VEC3 lbl_1_rodata_38 = { 0.0f, 0.0f, 0.0f };
FIRE_RODATA const f32 lbl_1_rodata_50[6] = { 0.08f, 0.0f, 0.0f, 0.0f, 0.08f, 0.0f };
FIRE_RODATA const s32 lbl_1_rodata_78[2] = { 4, 9 };
FIRE_RODATA const GXVtxDescList lbl_1_rodata_98[3] = {
    { GX_VA_POS, GX_INDEX16 },
    { GX_VA_TEX0, GX_DIRECT },
    { GX_VA_NULL, GX_NONE },
};
FIRE_RODATA const GXVtxAttrFmtList lbl_1_rodata_B0[3] = {
    { GX_VA_POS, GX_POS_XYZ, GX_U16, 0 },
    { GX_VA_TEX0, GX_TEX_ST, GX_F32, 0 },
    { GX_VA_NULL, GX_POS_XYZ, GX_U8, 0 },
};
FIRE_RODATA const M522_VEC3 lbl_1_rodata_F8 = { 0.0f, 0.0f, 0.0f };
FIRE_RODATA const GXVtxDescList lbl_1_rodata_120[3] = {
    { GX_VA_POS, GX_DIRECT },
    { GX_VA_CLR0, GX_DIRECT },
    { GX_VA_NULL, GX_NONE },
};
FIRE_RODATA const GXVtxAttrFmtList lbl_1_rodata_138[3] = {
    { GX_VA_POS, GX_POS_XYZ, GX_F32, 0 },
    { GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0 },
    { GX_VA_NULL, GX_POS_XYZ, GX_U8, 0 },
};
}

extern "C" {

M522_FIRE *fn_1_0(M522_FIRE *self)
{
    M522_FIREWAVE wave;
    M522_FIREWAVE *pWave;
    M522_WAVES *waves;
    M522_FIREWAVE *w;
    M522_FIREWAVE *end;
    M522_FIRE *singleton;
    s32 lightInfo;
    s16 model;

    fn_1_9A34((M522_MODEL *)self, 0x500002);
    if (lbl_1_bss_10 == 0) {
    } else {
        fn_1_6A70(lbl_1_data_0, lbl_1_data_10, 0x35);
    }
    singleton = self;
    lbl_1_bss_10 = singleton;
    self->field4 = 0.0f;
    self->field8 = lbl_1_bss_0;
    waves = (M522_WAVES *)&self->waves;
    waves->m_cap = 0;
    waves->m_size = 0;
    waves->m_data = 0;
    wave = lbl_1_bss_4;
    pWave = &wave;
    fn_1_26A8(waves, 0x400, pWave);
    model = self->model;
    fn_1_9DEC((M522_MODEL *)self, 2);
    fn_1_9BA8((M522_MODEL *)self, 0x40000001);
    Hu3DModelMatHookSet(model, fn_1_628);
    lightInfo = Hu3DModelLightInfoSet(model, 1);
    w = self->waves.data;
    end = self->waves.data + self->waves.size;
    while (w != end) {
        w->phase = 0.0f;
        w->delta = (f32)((3.141592653589793 * fn_1_BD68(-2.0f, 2.0f)) / 108.0);
        w->amp = fn_1_BD68(1.0f, 127.0f);
        w++;
    }
    GXInitTexObj(&self->texObj, self->tex, 0x20, 0x20, GX_TF_IA8, GX_REPEAT, GX_REPEAT, GX_FALSE);
    return self;
}

void *fn_1_21C(M522_FIRE *self, s16 flag)
{
    if (self != 0) {
        if (self != 0) {
            fn_1_4014((M522_WAVES *)&self->waves, 0);
        }
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

void *fn_1_27C(M522_FIRE *self, s16 flag)
{
    if (self != 0) {
        fn_1_4014((M522_WAVES *)&self->waves, 0);
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

void *fn_1_2D4(M522_FIRE *self, s16 flag)
{
    if (self != 0) {
        if (lbl_1_bss_10 != 0) {
        } else {
            fn_1_6A70(lbl_1_data_1C, lbl_1_data_10, 0x3D);
        }
        lbl_1_bss_10 = 0;
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

void *fn_1_360(M522_FIRE *self, s16 flag)
{
    if (self != 0) {
        if (&self->waves != 0 && &self->waves != 0) {
            fn_1_4014((M522_WAVES *)&self->waves, 0);
        }
        if (self != 0) {
            if (lbl_1_bss_10 != 0) {
            } else {
                fn_1_6A70(lbl_1_data_1C, lbl_1_data_10, 0x3D);
            }
            lbl_1_bss_10 = 0;
        }
        fn_1_9E94((M522_MODEL *)self, 0);
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

void fn_1_428(M522_FIRE *self)
{
    fn_1_4A4(self);
    fn_1_510(self);
}

void fn_1_460(M522_FIRE *self)
{
    self->field8 = 0.0f;
}

void fn_1_474(M522_FIRE *self)
{
    fn_1_9FE4((M522_MODEL *)self, lbl_1_data_2C);
}

void fn_1_4A4(M522_FIRE *self)
{
    M522_VEC3 rot;

    rot = lbl_1_rodata_38;
    rot.y = self->field4;
    fn_1_9CDC((M522_MODEL *)self, &rot);
    self->field4 = self->field4 + self->field8;
}

void fn_1_510(M522_FIRE *self)
{
    M522_FIREWAVE *w;
    M522_FIREWAVE *end;
    u8 *px;
    f32 phase;

    w = self->waves.data;
    end = self->waves.data + self->waves.size;
    px = self->tex;
    while (px != self->tex + sizeof(self->tex) && w != end) {
        phase = fn_1_614(w);
        px[0] = (u8)(128.0 + (f64)w->amp * sin(phase));
        px[1] = (u8)(128.0 + (f64)w->amp * cos(phase));
        px += 2;
        w++;
    }
    DCFlushRangeNoSync(self->tex, 0x800);
}

f32 fn_1_614(M522_FIREWAVE *w)
{
    return w->phase = w->phase + w->delta;
}

}

extern "C" {

void fn_1_628(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    if (lbl_1_bss_10 != 0) {
    } else {
        fn_1_6A70(lbl_1_data_1C, lbl_1_data_10, 0x27);
    }
    GXLoadTexObj(&lbl_1_bss_10->texObj, GX_TEXMAP1);
    GXSetNumIndStages(1);
    GXSetIndTexMtx(GX_ITM_0, (f32 (*)[3])lbl_1_rodata_50, 0);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD0, GX_TEXMAP1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_4, GX_ITS_4);
    GXSetNumTevStages(2);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVREG0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVREG0);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_C0, GX_CC_TEXC, GX_CC_HALF, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
}

M522_FIREEFF *fn_1_824(M522_FIREEFF *self)
{
    M522_MODEL *model;
    M522_SPARKVEC *sparks;
    M522_MODEL *copy;

    fn_1_E44C(&self->base);
    self->base.vtbl = lbl_1_data_64;
    model = (M522_MODEL *)fn_1_689C(sizeof(M522_MODEL));
    if (model != 0) {
        model = fn_1_9A34(model, 0x500004);
    }
    self->model = model;
    self->anim = 0.0f;
    sparks = &self->sparks;
    sparks->cap = 0;
    sparks->size = 0;
    sparks->data = 0;
    fn_1_9DEC((M522_MODEL *)&self->base.hook, 1);
    copy = self->model;
    fn_1_9BA8(copy, 1);
    fn_1_AB0(self, 0);
    fn_1_AB0(self, 1);
    return self;
}

void *fn_1_900(M522_FIREEFF *self, s16 flag)
{
    if (self != 0) {
        if (self != 0) {
            fn_1_40A0((M522_SPARKS *)self, 0);
        }
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

void *fn_1_960(M522_FIREEFF *self, s16 flag)
{
    if (self != 0) {
        fn_1_40A0((M522_SPARKS *)self, 0);
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

void *fn_1_9B8(M522_MODEL **ptr, s16 flag)
{
    if (ptr != 0) {
        fn_1_9E94(*ptr, 1);
        if (flag > 0) {
            fn_1_6964(ptr);
        }
    }
    return ptr;
}

void *fn_1_A10(M522_FIREEFF *self, s16 flag)
{
    if (self != 0) {
        self->base.vtbl = lbl_1_data_64;
        if (&self->sparks != 0 && &self->sparks != 0) {
            fn_1_40A0((M522_SPARKS *)&self->sparks, 0);
        }
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

void fn_1_AB0(M522_FIREEFF *self, s32 kind)
{
    M522_SPARK spark;
    HSFOBJECT *obj;
    HSFBUFFER *faces;
    HSFFACE *face;
    s32 i;

    obj = Hu3DModelObjPtrGet(self->model->id, lbl_1_data_44[kind]);
    if (obj != 0) {
        faces = obj->mesh.face;
        face = (HSFFACE *)faces->data;
        i = 0;
        while (i < faces->count) {
            if ((face->type & 7) == 3) {
                fn_1_B88(&spark, kind);
                fn_1_27E8((M522_SPARKS *)&self->sparks, self->sparks.data + self->sparks.size, 1, &spark);
            }
            i++;
            face++;
        }
    }
}

M522_SPARK *fn_1_B88(M522_SPARK *spark, s32 kind)
{
    s32 frames;

    spark->kind = kind;
    if (kind == 0) {
        frames = 0x3F;
    } else {
        frames = 0x8F;
    }
    spark->frames = frames;
    spark->frame = (s32)((f32)spark->frames * fn_1_BABC());
    spark->wait = 0;
    return spark;
}

void fn_1_C2C(M522_SPARK *dst, const M522_SPARK *src)
{
    dst->kind = src->kind;
    dst->frames = src->frames;
    dst->frame = src->frame;
    dst->wait = src->wait;
}

void fn_1_C50(M522_SPARK *spark)
{
    if (spark->wait > 0) {
        spark->wait = spark->wait - 1;
        if (spark->wait == 0) {
        }
    }
    if (spark->wait <= 0) {
        spark->frame = spark->frame + 1;
        if (spark->frame > spark->frames) {
            spark->frame = 0;
            if (spark->kind == 1) {
                spark->wait = (s32)(60.0f * (10.0f * fn_1_BABC()));
            }
        }
    }
}

void fn_1_D50(M522_SPARK *spark, f32 *u0, f32 *v0, f32 *u1, f32 *v1)
{
    s32 rows[2];
    s32 *pRows;

    rows[0] = lbl_1_rodata_78[0];
    rows[1] = lbl_1_rodata_78[1];
    pRows = &rows[spark->kind];
    *u0 = (f32)(0.0009765625 + (f64)(spark->frame & 0xF) * 0.0625);
    *u1 = (f32)(((f64)((spark->frame & 0xF) + 1) * 0.0625) - 0.0009765625);
    *v0 = (f32)(((f64)(spark->frame / 16) / (f64)*pRows) + (1.0 / (f64)(*pRows << 6)));
    *v1 = (f32)(((f64)((spark->frame / 16) + 1) / (f64)*pRows) - (1.0 / (f64)(*pRows << 6)));
}

void fn_1_F84(M522_FIREEFF *self)
{
    M522_SPARK *spark;
    M522_SPARK *end;

    spark = self->sparks.data;
    end = self->sparks.data + self->sparks.size;
    while (spark != end) {
        if (spark->wait > 0) {
            spark->wait = spark->wait - 1;
            if (spark->wait == 0) {
            }
        }
        if (spark->wait <= 0) {
            spark->frame = spark->frame + 1;
            if (spark->frame > spark->frames) {
                spark->frame = 0;
                if (spark->kind == 1) {
                    spark->wait = (s32)(60.0f * (10.0f * fn_1_BABC()));
                }
            }
        }
        spark++;
    }
    self->anim = self->anim + lbl_1_bss_0;
}

void fn_1_10C8(M522_FIREEFF *self, Mtx *camera)
{
    Mtx inv;
    Mtx rot;
    M522_SPARK *cursor;

    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetCullMode(GX_CULL_NONE);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetAlphaCompare(GX_ALWAYS, 0xFF, GX_AOP_OR, GX_ALWAYS, 0xFF);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
    GXClearVtxDesc();
    GXSetVtxDescv((GXVtxDescList *)lbl_1_rodata_98);
    GXSetVtxAttrFmtv(GX_VTXFMT0, (GXVtxAttrFmtList *)lbl_1_rodata_B0);
    PSMTXInverse(*camera, inv);
    inv[2][3] = 0.0f;
    inv[1][3] = 0.0f;
    inv[0][3] = 0.0f;
    PSMTXRotRad(rot, 'z', 0.017453292f * self->anim);
    PSMTXConcat(inv, rot, inv);
    cursor = self->sparks.data;
    fn_1_12C0(self, 0, *camera, inv, &cursor);
    fn_1_12C0(self, 1, *camera, inv, &cursor);
}

void fn_1_12C0(M522_FIREEFF *self, s32 kind, Mtx camera, Mtx rot, M522_SPARK **cur)
{
    Mtx objMtx;
    HSFOBJECT *obj;
    HSFBUFFER *faces;
    HSFFACE *face;
    char *name;
    s32 i;
    s32 rows[2];
    s32 *pRows;
    f32 u0;
    f32 u1;
    f32 v0;
    f32 v1;
    M522_SPARK *spark;

    name = lbl_1_data_44[kind];
    Hu3DModelObjMtxGet(self->model->id, name, objMtx);
    PSMTXConcat(camera, objMtx, objMtx);
    PSMTXConcat(objMtx, rot, objMtx);
    GXLoadPosMtxImm(objMtx, 0);
    fn_1_E11C(kind, 0, 0, 0, 1);
    obj = Hu3DModelObjPtrGet(self->model->id, name);
    faces = obj->mesh.face;
    face = (HSFFACE *)faces->data;
    GXSETARRAY(GX_VA_POS, obj->mesh.vertex->data, obj->mesh.vertex->count * sizeof(HuVecF), sizeof(HuVecF), TRUE);
    i = 0;
    while (i < faces->count) {
        if (*cur == self->sparks.data + self->sparks.size) {
            break;
        }
        if ((face->type & 7) == 3) {
            if ((*cur)->wait <= 0) {
                spark = *cur;
                rows[0] = lbl_1_rodata_78[0];
                rows[1] = lbl_1_rodata_78[1];
                pRows = &rows[spark->kind];
                u0 = (f32)(0.0009765625 + (f64)(spark->frame & 0xF) * 0.0625);
                u1 = (f32)(((f64)((spark->frame & 0xF) + 1) * 0.0625) - 0.0009765625);
                v0 = (f32)(((f64)(spark->frame / 16) / (f64)*pRows) + (1.0 / (f64)(*pRows << 6)));
                v1 = (f32)(((f64)((spark->frame / 16) + 1) / (f64)*pRows) - (1.0 / (f64)(*pRows << 6)));
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                GXPosition1x16(face->index[0].vertex);
                GXTexCoord2f32(u0, v0);
                GXPosition1x16(face->index[2].vertex);
                GXTexCoord2f32(u1, v0);
                GXPosition1x16(face->index[3].vertex);
                GXTexCoord2f32(u1, v1);
                GXPosition1x16(face->index[1].vertex);
                GXTexCoord2f32(u0, v1);
                GXEnd();
            }
            *cur = *cur + 1;
        }
        i++;
        face++;
    }
}

M522_BURST *fn_1_17E8(M522_BURST *self, M522_VEC3 *pos, s32 count, f32 len, f32 time)
{
    M522_VEC3 dir;
    M522_VEC3 start;
    M522Trail trail;
    M522_TRAILS *trails;
    s32 i;
    f32 x;
    f32 y;
    f32 z;

    fn_1_E44C(&self->base);
    self->base.vtbl = lbl_1_data_4C;
    trails = &self->trails;
    trails->m_cap = 0;
    trails->m_size = 0;
    trails->m_data = 0;
    fn_1_9DEC((M522_MODEL *)&self->base.hook, 6);
    fn_1_422C(trails);
    for (i = 0; i < count; i++) {
        z = fn_1_BD68(-1.0f, 1.0f);
        y = fn_1_BD68(-0.01f, -1.0f);
        x = fn_1_BD68(-1.0f, 1.0f);
        dir.x = x;
        dir.y = y;
        dir.z = z;
        PSVECNormalize((Vec *)&dir, (Vec *)&dir);
        PSVECScale((Vec *)&dir, (Vec *)&start, (f32)(0.1 * len));
        PSVECAdd((Vec *)pos, (Vec *)&start, (Vec *)&start);
        PSVECScale((Vec *)&dir, (Vec *)&dir, len / (60.0f * time));
        fn_1_1BC0(&trail, &start, &dir, len);
        fn_1_349C(trails, trails->m_data + trails->m_size, 1, &trail);
        fn_1_4188((M522_POINTS *)&trail, 0);
    }
    return self;
}

void *fn_1_1A18(M522Trail *self, s16 flag)
{
    if (self != 0) {
        if (self != 0 && self != 0) {
            fn_1_4188((M522_POINTS *)self, 0);
        }
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

void *fn_1_1A80(M522_TRAILS *self, s16 flag)
{
    if (self != 0) {
        if (self != 0) {
            fn_1_4304(self, 0);
        }
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

void *fn_1_1AE0(M522_TRAILS *self, s16 flag)
{
    if (self != 0) {
        fn_1_4304(self, 0);
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

void *fn_1_1B38(M522_BURST *self, s16 flag)
{
    if (self != 0) {
        self->base.vtbl = lbl_1_data_4C;
        if (&self->trails != 0 && &self->trails != 0) {
            fn_1_4304(&self->trails, 0);
        }
        fn_1_E534(&self->base, 0);
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

M522Trail *fn_1_1BC0(M522Trail *self, M522_VEC3 *pos, M522_VEC3 *vel, f32 len)
{
    M522_VEC3 *start;

    start = pos;
    self->pts.m_cap = 0;
    self->pts.m_size = 0;
    self->pts.m_data = 0;
    self->vel = *vel;
    self->total = len;
    self->remain = len;
    self->index = 0;
    fn_1_412C(&self->pts);
    fn_1_2F88(&self->pts, 8, start);
    return self;
}

void *fn_1_1C78(M522Trail *self, s16 flag)
{
    if (self != 0) {
        if (self != 0) {
            fn_1_4188((M522_POINTS *)self, 0);
        }
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

void *fn_1_1CD8(M522Trail *self, s16 flag)
{
    if (self != 0) {
        fn_1_4188((M522_POINTS *)self, 0);
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

s32 fn_1_1D30(M522Trail *self)
{
    M522_VEC3 rnd;
    M522_VEC3 *cur;
    M522_VEC3 *next;
    f32 mag;

    cur = self->pts.m_data + self->index;
    self->index = self->index + 1;
    if (self->index >= self->pts.m_size) {
        self->index = 0;
    }
    next = self->pts.m_data + self->index;
    PSVECAdd((Vec *)cur, (Vec *)&self->vel, (Vec *)next);
    mag = PSVECMag((Vec *)&self->vel);
    self->remain = self->remain - mag;
    rnd = lbl_1_rodata_F8;
    rnd.x = fn_1_BD68(-4.0f, 4.0f);
    rnd.y = fn_1_BD68(0.0f, 4.0f);
    rnd.z = fn_1_BD68(-4.0f, 4.0f);
    self->vel.x = (f32)((0.8 * (f64)self->vel.x) + (0.2 * (f64)rnd.x));
    self->vel.y = (f32)((0.8 * (f64)self->vel.y) + (0.2 * (f64)rnd.y));
    self->vel.z = (f32)((0.8 * (f64)self->vel.z) + (0.2 * (f64)rnd.z));
    return self->remain > 0.0f;
}

void fn_1_1FE4(M522_BURST *self)
{
    M522_VEC3 rnd;
    M522Trail *trail;
    M522Trail *end;
    M522_VEC3 *cur;
    M522_VEC3 *next;
    u32 size;

    trail = self->trails.m_data;
    end = self->trails.m_data + self->trails.m_size;
    while (trail != end) {
        cur = trail->pts.m_data + trail->index;
        size = trail->pts.m_size;
        trail->index = trail->index + 1;
        if (trail->index >= size) {
            trail->index = 0;
        }
        next = trail->pts.m_data + trail->index;
        PSVECAdd((Vec *)cur, (Vec *)&trail->vel, (Vec *)next);
        trail->remain = trail->remain - PSVECMag((Vec *)&trail->vel);
        rnd = lbl_1_rodata_F8;
        rnd.x = fn_1_BD68(-4.0f, 4.0f);
        rnd.y = fn_1_BD68(0.0f, 4.0f);
        rnd.z = fn_1_BD68(-4.0f, 4.0f);
        trail->vel.x = (f32)((0.8 * (f64)trail->vel.x) + (0.2 * (f64)rnd.x));
        trail->vel.y = (f32)((0.8 * (f64)trail->vel.y) + (0.2 * (f64)rnd.y));
        trail->vel.z = (f32)((0.8 * (f64)trail->vel.z) + (0.2 * (f64)rnd.z));
        if (!(trail->remain > 0.0f)) {
            fn_1_32D8(&self->trails, trail);
        }
        trail++;
    }
    if (self->trails.m_size == 0 && self != 0) {
        ((void (*)(M522_BURST *, s32))self->base.vtbl[2])(self, 1);
    }
}

void fn_1_232C(M522_BURST *self, Mtx *camera)
{
    M522Trail *trail;
    M522Trail *end;
    Mtx *cam;

    cam = camera;
    GXLoadPosMtxImm(*cam, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetCullMode(GX_CULL_NONE);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZCompLoc(GX_TRUE);
    GXSetZMode(GX_TRUE, GX_LESS, GX_FALSE);
    GXSetAlphaCompare(GX_ALWAYS, 0xFF, GX_AOP_OR, GX_ALWAYS, 0xFF);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
    GXClearVtxDesc();
    GXSetVtxDescv((GXVtxDescList *)lbl_1_rodata_120);
    GXSetVtxAttrFmtv(GX_VTXFMT0, (GXVtxAttrFmtList *)lbl_1_rodata_138);
    GXSetLineWidth(6, GX_TO_ZERO);
    trail = self->trails.m_data;
    end = self->trails.m_data + self->trails.m_size;
    GXBegin(GX_LINES, GX_VTXFMT0, (u16)(self->trails.m_size * 2));
    while (trail != end) {
        fn_1_24FC(trail);
        trail++;
    }
    GXEnd();
}

void fn_1_24FC(M522Trail *trail)
{
    M522_VEC3 *a;
    M522_VEC3 *b;

    a = trail->pts.m_data + trail->index;
    b = trail->pts.m_data + ((trail->index + 1) % trail->pts.m_size);
    GXPosition3f32(a->x, a->y, a->z);
    GXColor4u8(0xFF, 0xFF, 0, (u8)(s32)((255.0f * trail->remain) / trail->total));
    GXPosition3f32(b->x, b->y, b->z);
    GXColor4u8(0xFF, 0, 0, 0);
}

void fn_1_26A4(void)
{
}

}

extern "C" {

#define M522_FN fn_1_26A8
#define M522_VEC M522_WAVES
#define M522_T M522_FIREWAVE
#include "REL/m522vec_init_fill.inc"
#undef M522_FN
#undef M522_VEC
#undef M522_T

#define M522_TMP_DTOR(t) fn_1_48D8(&t); if (t.m_data != 0) { fn_1_6964(t.m_data); }
#define M522_FN fn_1_27E8
#define M522_VEC M522_SPARKS
#define M522_T M522_SPARK
#include "REL/m522vec_insert.inc"
#undef M522_FN
#undef M522_VEC
#undef M522_T
#undef M522_TMP_DTOR

#define M522_FN fn_1_2F88
#define M522_VEC M522_POINTS
#define M522_T M522_VEC3
#include "REL/m522vec_assign_fill.inc"
#undef M522_FN
#undef M522_VEC
#undef M522_T

#define M522_FN fn_1_32D8
#define M522_VEC M522_TRAILS
#define M522_T M522Trail
#include "REL/m522vec_erase.inc"
#undef M522_FN
#undef M522_VEC
#undef M522_T

void fn_1_3498(void)
{
}

#define M522_TMP_DTOR(t) fn_1_4304((M522_TRAILS *)&t, -1)
#define M522_FN fn_1_349C
#define M522_VEC M522_TRAILS
#define M522_T M522Trail
#include "REL/m522vec_insert.inc"
#undef M522_FN
#undef M522_VEC
#undef M522_T
#undef M522_TMP_DTOR

void *fn_1_4014(M522_WAVES *self, s16 flag)
{
    if (self != 0) {
        fn_1_487C(self);
        if (self->begin() != 0) {
            fn_1_6964(self->begin());
        }
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

void *fn_1_40A0(M522_SPARKS *self, s16 flag)
{
    if (self != 0) {
        fn_1_48D8(self);
        if (self->begin() != 0) {
            fn_1_6964(self->begin());
        }
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

#define M522_FN fn_1_412C
#define M522_VEC M522_POINTS
#define M522_T M522_VEC3
#include "REL/m522vec_clear.inc"
#undef M522_FN
#undef M522_VEC
#undef M522_T

void *fn_1_4188(M522_POINTS *self, s16 flag)
{
    if (self != 0) {
        M522_VEC_CLEAR(self, M522_VEC3);
        if (self->begin() != 0) {
            fn_1_6964(self->begin());
        }
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

#define M522_FN fn_1_422C
#define M522_VEC M522_TRAILS
#define M522_T M522Trail
#include "REL/m522vec_clear.inc"
#undef M522_FN
#undef M522_VEC
#undef M522_T

void *fn_1_4304(M522_TRAILS *self, s16 flag)
{
    if (self != 0) {
        fn_1_422C(self);
        if (self->begin() != 0) {
            fn_1_6964(self->begin());
        }
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

#define M522_FN fn_1_4390
#define M522_VEC M522_POINTS
#define M522_T M522_VEC3
#include "REL/m522vec_init_range.inc"
#undef M522_FN
#undef M522_VEC
#undef M522_T

#define M522_FN fn_1_4504
#define M522_VEC M522_POINTS
#define M522_T M522_VEC3
#include "REL/m522vec_assign_range.inc"
#undef M522_FN
#undef M522_VEC
#undef M522_T

#define M522_FN fn_1_487C
#define M522_VEC M522_WAVES
#define M522_T M522_FIREWAVE
#include "REL/m522vec_clear.inc"
#undef M522_FN
#undef M522_VEC
#undef M522_T

#define M522_FN fn_1_48D8
#define M522_VEC M522_SPARKS
#define M522_T M522_SPARK
#include "REL/m522vec_clear.inc"
#undef M522_FN
#undef M522_VEC
#undef M522_T

void fn_1_4934(void)
{
    lbl_1_bss_0 = 0.011805556f;
}

}
