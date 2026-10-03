#include "dolphin/types.h"

typedef s16 HU3DMODELID;

extern "C" {
typedef struct PTMF {
    long this_delta;
    long v_offset;
    union {
        void *f_addr;
        long ve_offset;
    } f_data;
} PTMF;

s32 __ptmf_test(const PTMF *ptmf);
void __ptmf_scall(void *self);
void __dl__FPv(void *ptr);

// singleton.h assertion
void fn_1_4C3C(const char *cond, const char *file, s32 line);

struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};

// The intrusive list the stage registers itself in; lbl_1_bss_1C is its head.
struct CListNode {
    CListNode *m_prev;
    CListNode *m_next;
};

extern CListNode lbl_1_bss_1C;

void fn_1_5CCC(CListNode *self);
void fn_1_5D98(CListNode *self, CListNode *head);
void fn_1_5CD8(CListNode *self, s32 freeIt);
void fn_1_5DC(void *obj, s32 flag);

// Class allocator: HuMemDirectMallocNum(0, n, 0x1000) plus a null assert.
void *fn_1_4A68(u32 n);
void *fn_1_38(void *obj, s32 kind, const Vec3 *pos);
void fn_1_C4C(void *obj);
void fn_1_7870(void *model, u32 flags);
void fn_1_7D14(void *model, const char *attrName);

struct CModel {
    s16 m_id;
};

CModel *__ct__6CModelFUi(CModel *self, u32 modelIdx);
void __dt__6CModelFv(CModel *self, s32 freeIt);

void PSVECNormalize(const Vec3 *src, Vec3 *dst);
void PSVECScale(const Vec3 *src, Vec3 *dst, f32 scale);
void PSVECAdd(Vec3 *dst, const Vec3 *a, const Vec3 *b);

void Hu3DLighInit(void);
s32 Hu3DGLightCreateV(const Vec3 *pos, const Vec3 *dir, const void *color);
void Hu3DGLightInfinitytSet(s16 lightId);
void Hu3DShadowCreate(f32 near, f32 far, f32 depthRange);
void Hu3DShadowTPLvlSet(f32 level);
void Hu3DShadowPosSet(const Vec3 *pos, const Vec3 *target, const Vec3 *up);
void Hu3DShadowColSet(s32 r, s32 g, s32 b);
void Hu3DModelShadowMapSet(s16 modelId);
void Hu3DModelShadowDispOn(s16 modelId);
void Hu3DModelShadowSet(s16 modelId);
void Hu3DModelLayerSet(s16 modelId, s16 layerNo);
}

extern "C" {
// Virtual tables. The base one lives in m521.o; both are stored at offset 8.
extern void *lbl_1_data_21C[5];
extern void *lbl_1_data_66C[5];

// The three effect anchors the stage spawns effect objects at.
static const Vec3 s_effectPos[3] = {
    {-450.0f, 90.0f, 0.0f},
    {450.0f, 90.0f, 0.0f},
    {0.0f, 342.59368896484375f, -386.13800048828125f},
};

class CStage {
public:
    CListNode m_node;
    void **m_vtable;
    s32 m_bitFlags;
    void *m_unk10;
    void *m_unk14;
    void *m_unk18;
    CModel *m_modelA;
    CModel *m_modelB;
    PTMF m_hook;
};

CStage *fn_1_1490C();
void fn_1_1497C(s16 modelId);
CStage *fn_1_149C0(CStage *self);
CStage *fn_1_14FF4(CStage *self, s16 freeIt);
void fn_1_15080();
void fn_1_15084(CStage *self);
void fn_1_15108(CStage *self, s32 bit);
CStage *fn_1_15120(CStage *self, s16 freeIt);
void fn_1_15264();

extern CStage *lbl_1_bss_A78;
extern CModel *lbl_1_bss_A30;
extern CModel *lbl_1_bss_A40;
extern void *lbl_1_bss_A50;
extern void *lbl_1_bss_A54;
extern void *lbl_1_bss_A58;
extern s32 lbl_1_bss_A98;
extern s32 lbl_1_bss_A9C;
extern s32 lbl_1_bss_AA0[2];

// singleton.h's register/unregister pair: the derived pointer is built from the
// subobject address and only then biased back, so the null check survives.
static CStage *singletonPtr(u8 *subObj)
{
    return (subObj != 0) ? (CStage *)(subObj - 0x10) : 0;
}

// NONMATCHING: the target keeps the loaded pointer in r30/r31 and spills it to the caller arg slot at 0x8(r1); ours returns it in r3 with a 0x10 frame.
CStage *fn_1_1490C()
{
    CStage *p;

    if (lbl_1_bss_A78 == 0) {
        fn_1_4C3C("ms_pointer != 0", "singleton.h", 0x27);
    }
    p = lbl_1_bss_A78;
    return p;
}

void fn_1_1497C(s16 modelId)
{
    Hu3DModelShadowMapSet(modelId);
    Hu3DModelShadowDispOn(modelId);
    Hu3DModelLayerSet(modelId, 0);
}

static const u32 s_lightColor = 0xFFFFFFFF;
static const Vec3 s_lightPos = {0.0f, 100.0f, 1000.0f};
static const Vec3 s_shadowDir = {0.0f, 1000.0f, 400.0f};
static const PTMF s_stageHook = {0, -1, {(void *)fn_1_15080}};

// NONMATCHING: register allocation and the stack-temporary round trips around the model/effect stores differ; the effect order is unk10/unk18/unk14.
CStage *fn_1_149C0(CStage *self)
{
    Vec3 lightDir;
    Vec3 shadowDir;
    Vec3 shadowPos;
    Vec3 shadowTarget;
    Vec3 shadowUp;
    CModel *model;
    s16 modelId;
    void *effect;

    fn_1_5CCC(&self->m_node);
    self->m_vtable = lbl_1_data_21C;
    self->m_bitFlags = 0;
    fn_1_5D98(&self->m_node, &lbl_1_bss_1C);
    if (lbl_1_bss_A78 != 0) {
        fn_1_4C3C("ms_pointer == 0", "singleton.h", 0x35);
    }
    lbl_1_bss_A78 = singletonPtr((u8 *)self + 0x10);
    self->m_vtable = lbl_1_data_66C;
    self->m_unk10 = 0;
    self->m_unk14 = 0;
    self->m_unk18 = 0;
    model = (CModel *)fn_1_4A68(2);
    if (model != 0) {
        model = __ct__6CModelFUi(model, 0x4F0000);
    }
    self->m_modelA = model;
    model = (CModel *)fn_1_4A68(2);
    if (model != 0) {
        model = __ct__6CModelFUi(model, 0x4F0001);
    }
    self->m_modelB = model;
    Hu3DLighInit();
    PSVECNormalize(&s_lightPos, &lightDir);
    PSVECScale(&lightDir, &lightDir, -1.0f);
    modelId = (s16)Hu3DGLightCreateV(&s_lightPos, &lightDir, &s_lightColor);
    Hu3DGLightInfinitytSet(modelId);
    Hu3DShadowCreate(40.0f, 20.0f, 25000.0f);
    modelId = self->m_modelA->m_id;
    Hu3DModelShadowMapSet(modelId);
    Hu3DModelShadowDispOn(modelId);
    Hu3DModelLayerSet(modelId, 0);
    fn_1_7870(self->m_modelA, 0x40000001);
    PSVECNormalize(&s_shadowDir, &shadowDir);
    Hu3DShadowTPLvlSet(0.5f);
    shadowTarget.x = 0.0f;
    shadowTarget.y = 20.0f;
    shadowTarget.z = -500.0f;
    PSVECScale(&shadowDir, &shadowPos, 3000.0f);
    PSVECAdd(&shadowPos, &shadowTarget, &shadowPos);
    shadowUp.x = 0.0f;
    shadowUp.y = 1.0f;
    shadowUp.z = 0.0f;
    Hu3DShadowPosSet(&shadowPos, &shadowUp, &shadowTarget);
    Hu3DShadowColSet(0x20, 0x20, 0x20);
    fn_1_7D14(self->m_modelA, "sphere6");
    fn_1_7D14(self->m_modelA, "sphere12");
    fn_1_7D14(self->m_modelA, "ie_ALL");
    fn_1_7D14(self->m_modelA, "cyl9");
    fn_1_7D14(self->m_modelA, "cyl30");
    modelId = self->m_modelB->m_id;
    Hu3DModelShadowSet(modelId);
    fn_1_7870(self->m_modelB, 8);
    modelId = lbl_1_bss_A30->m_id;
    Hu3DModelShadowMapSet(modelId);
    modelId = lbl_1_bss_A40->m_id;
    Hu3DModelShadowMapSet(modelId);
    fn_1_7D14(lbl_1_bss_A30, "tobiraRR");
    fn_1_7D14(lbl_1_bss_A30, "tobiraRL");
    fn_1_7D14(lbl_1_bss_A40, "tobiraBR");
    fn_1_7D14(lbl_1_bss_A40, "tobiraBL");
    effect = fn_1_4A68(0x74);
    if (effect != 0) {
        effect = fn_1_38(effect, 0, &s_effectPos[0]);
    }
    if (self->m_unk10 != effect) {
        fn_1_5DC(self->m_unk10, 1);
        self->m_unk10 = effect;
    }
    effect = fn_1_4A68(0x74);
    if (effect != 0) {
        effect = fn_1_38(effect, 1, &s_effectPos[1]);
    }
    if (self->m_unk18 != effect) {
        fn_1_5DC(self->m_unk18, 1);
        self->m_unk18 = effect;
    }
    effect = fn_1_4A68(0x74);
    if (effect != 0) {
        effect = fn_1_38(effect, 2, &s_effectPos[2]);
    }
    if (self->m_unk14 != effect) {
        fn_1_5DC(self->m_unk14, 1);
        self->m_unk14 = effect;
    }
    lbl_1_bss_A50 = self->m_unk10;
    lbl_1_bss_A54 = self->m_unk18;
    lbl_1_bss_A58 = self->m_unk14;
    {
        const PTMF *hook = &s_stageHook;
        self->m_hook.this_delta = hook->this_delta;
        self->m_hook.v_offset = hook->v_offset;
        self->m_hook.f_data = hook->f_data;
    }
    return self;
}

struct CSingletonBase {
    void *m_sub;
};

CStage *fn_1_14F9C(CSingletonBase *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_5DC(self->m_sub, 1);
        if (freeIt > 0) {
            delete self;
        }
    }
    return (CStage *)self;
}

// NONMATCHING: the ms_pointer assert branch polarity differs slightly.
CStage *fn_1_14FF4(CStage *self, s16 freeIt)
{
    if (self != 0) {
        if (lbl_1_bss_A78 == 0) {
            fn_1_4C3C("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A78 = 0;
        if (freeIt > 0) {
            delete self;
        }
    }
    return self;
}

void fn_1_15080()
{
}

// NONMATCHING: the target sets r12 before __ptmf_scall and emits a nop after it.
void fn_1_15084(CStage *self)
{
    void *a;
    void *b;
    void *c;

    if (__ptmf_test(&self->m_hook) != 0) {
        __ptmf_scall(self);
    }
    a = self->m_unk10;
    fn_1_C4C(a);
    b = self->m_unk18;
    fn_1_C4C(b);
    c = self->m_unk14;
    fn_1_C4C(c);
}

// NONMATCHING: target uses andc r0, r5, r0 where ours emits nor+and.
void fn_1_15108(CStage *self, s32 bit)
{
    self->m_bitFlags &= ~(1 << bit);
}

// NONMATCHING: the derived-to-base vtable teardown ordering differs.
CStage *fn_1_15120(CStage *self, s16 freeIt)
{
    if (self != 0) {
        self->m_vtable = lbl_1_data_66C;
        if (&self->m_modelB != 0) {
            __dt__6CModelFv(self->m_modelB, 1);
        }
        if (&self->m_modelA != 0) {
            __dt__6CModelFv(self->m_modelA, 1);
        }
        if (&self->m_unk18 != 0) {
            fn_1_5DC(self->m_unk18, 1);
        }
        if (&self->m_unk14 != 0) {
            fn_1_5DC(self->m_unk14, 1);
        }
        if (&self->m_unk10 != 0) {
            fn_1_5DC(self->m_unk10, 1);
        }
        if (&self->m_unk10 != 0) {
            if (lbl_1_bss_A78 == 0) {
                fn_1_4C3C("ms_pointer != 0", "singleton.h", 0x3D);
            }
            lbl_1_bss_A78 = 0;
        }
        if (self != 0) {
            self->m_vtable = lbl_1_data_21C;
            if (self != 0) {
                fn_1_5CD8(&self->m_node, 0);
            }
        }
        if (freeIt > 0) {
            delete self;
        }
    }
    return self;
}

void fn_1_15264()
{
    lbl_1_bss_A98 = 0x78;
    lbl_1_bss_A9C = 0xD2;
    lbl_1_bss_AA0[0] = 0xD2;
}

void *lbl_1_data_66C[5] = {
    0, 0, (void *)fn_1_15120, (void *)fn_1_15108, (void *)fn_1_15084
};
}
