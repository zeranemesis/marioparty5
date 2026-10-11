#include "dolphin/types.h"

typedef s16 HU3DMODELID;
typedef s16 HU3DMOTID;

extern "C" {
typedef struct PTMF {
    long this_delta;
    long v_offset;
    union {
        void *f_addr;
        long ve_offset;
    } f_data;
} PTMF;

extern const PTMF __ptmf_null;
s32 __ptmf_test(const PTMF *ptmf);
void __ptmf_scall(void *self);
void __dl__FPv(void *ptr);

// singleton.h assertion
void fn_1_4C3C(const char *cond, const char *file, s32 line);

// The intrusive list the camera registers itself in; lbl_1_bss_1C is its head.
struct CListNode {
    CListNode *m_prev;
    CListNode *m_next;
};

extern CListNode lbl_1_bss_1C;

void fn_1_5CCC(CListNode *self);
void fn_1_5D98(CListNode *self, CListNode *head);
void fn_1_5CD8(CListNode *self, s32 freeIt);

void fn_1_51D4(void *dst, void *from, void *to, f32 t);
void fn_1_5408(void *obj, s32 flag);
void fn_1_5448(void *obj, s32 flag);

HU3DMOTID Hu3DMotionCreate(void *hsf);
HU3DMODELID Hu3DModelCameraCreate(HU3DMOTID motId, u32 cameraBit);
void Hu3DCameraMotionStart(HU3DMODELID modelId, u32 cameraBit);
void Hu3DCameraCreate(s16 camId);
void Hu3DCameraPerspectiveSet(s16 camId, f32 aspect, f32 nearZ, f32 farZ, f32 fovy);
void Hu3DCameraViewportSet(s16 camId, f32 left, f32 top, f32 wd, f32 ht, f32 nearZ, f32 farZ);
void Hu3DModelKill(HU3DMODELID modelId);
void *HuDataSelHeapReadNum(u32 fileIdx, u32 heapAddr, s32 num);
}

extern "C" {
// Virtual tables. The base one lives in m521.o; both are stored at offset 8.
extern void *lbl_1_data_21C[5];

// A point on the camera path plus a roll angle and a scale.
struct CameraPath {
    f32 v[7];
};

static const CameraPath s_cameraPath = {
    395.9320374f, 1367.4566650f, 292.0061035f, -0.3923721f, 0.0f, -0.0f, 0.9198066f
};

/* lbl_1_data_5A0 in the original: {0, -1, 0}, the empty pointer-to-member the
 * target copies into m_hook in both switch arms. Naming CCamera::step instead
 * produced an undefined symbol step__7CCameraFv and broke the REL link. */
static const PTMF s_nullPmf = {0, -1, {0}};

class CCamera {
public:
    CListNode m_node;      /* 0x00 */
    void **m_vtable;       /* 0x08 */
    s32 m_bitFlags;        /* 0x0C */
    u8 m_unk10[0x34 - 0x10];
    /* The target stores 0x34..0x6B as two consecutive 28-byte CameraPaths, so
       the path pair starts at 0x34 and the timer / PMF sit after it. */
    CameraPath m_pathFrom; /* 0x34 */
    CameraPath m_pathTo;   /* 0x50 */
    s32 m_timer;           /* 0x6C */
    PTMF m_hook;           /* 0x70 */
    s16 m_camModelId;      /* 0x7C */
};

CCamera *fn_1_1416C(CCamera *self);
CCamera *fn_1_142EC(CCamera *self, s16 freeIt);
CCamera *fn_1_14378(CCamera *self, s16 freeIt);
void fn_1_143D0();
void fn_1_143D4(CCamera *self);
void fn_1_14424(CCamera *self, s32 mode);
void fn_1_145A4(CCamera *self);
void fn_1_145F8(CCamera *self, s32 flip);
void fn_1_14744(CCamera *self);
CCamera *fn_1_14840(CCamera *self, s16 freeIt);

void *lbl_1_data_5B8[6] = {
    0, 0, (void *)fn_1_14840, (void *)fn_1_14424, (void *)fn_1_145A4, 0
};

extern CCamera *lbl_1_bss_A7C;

// singleton.h's register/unregister pair: the derived pointer is built from the
// subobject address and only then biased back, so the null check stays.
static CCamera *singletonPtr(u8 *subObj)
{
    return (subObj != 0) ? (CCamera *)(subObj - 0x10) : 0;
}

// NONMATCHING: target keeps self in r30/r31/r28 with a 3-instruction copy chain before fn_1_5CCC; ours uses r31 only, so the vtable stores and epilogue differ.
CCamera *fn_1_1416C(CCamera *self)
{
    const PTMF *nullPm = &__ptmf_null;

    fn_1_5CCC(&self->m_node);
    self->m_vtable = lbl_1_data_21C;
    self->m_bitFlags = 0;
    fn_1_5D98(&self->m_node, &lbl_1_bss_1C);
    if (lbl_1_bss_A7C != 0) {
        fn_1_4C3C("ms_pointer == 0", "singleton.h", 0x35);
    }
    lbl_1_bss_A7C = singletonPtr((u8 *)self + 0x10);
    self->m_vtable = lbl_1_data_5B8;
    ((PTMF *)&self->m_hook)->this_delta = nullPm->this_delta;
    ((PTMF *)&self->m_hook)->v_offset = nullPm->v_offset;
    ((PTMF *)&self->m_hook)->f_data = nullPm->f_data;
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 26.0f, 100.0f, 2500.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    return self;
}

// NONMATCHING: branch polarity around the ms_pointer assert differs.
CCamera *fn_1_142EC(CCamera *self, s16 freeIt)
{
    if (self != 0) {
        if (lbl_1_bss_A7C == 0) {
            fn_1_4C3C("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A7C = 0;
        if (freeIt > 0) {
            delete self;
        }
    }
    return self;
}

CCamera *fn_1_14378(CCamera *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_5CD8(&self->m_node, 0);
        if (freeIt > 0) {
            delete self;
        }
    }
    return self;
}

void fn_1_143D0()
{
}

// NONMATCHING: clears the low bit with clrrwi in both, but register choice differs.
void fn_1_143D4(CCamera *self)
{
    if (self->m_timer >= 0xF0) {
        self->m_bitFlags &= ~1;
        fn_1_5448(self->m_unk10, 1);
    }
}

// NONMATCHING: switch arms spill the PMF to the stack differently; case 1 also clears bit 1 unconditionally.
void fn_1_14424(CCamera *self, s32 mode)
{
    self->m_timer = 0;
    switch (mode) {
    case 0:
        self->m_pathTo = s_cameraPath;
        self->m_hook = s_nullPmf;
        self->m_camModelId =
            Hu3DModelCameraCreate(Hu3DMotionCreate(HuDataSelHeapReadNum(0x4F000E, 0x10000000, 2)), 1);
        Hu3DCameraMotionStart(self->m_camModelId, 1);
        break;
    case 1:
        self->m_pathTo = s_cameraPath;
        self->m_hook = s_nullPmf;
        Hu3DModelKill(self->m_camModelId);
        self->m_bitFlags &= ~(1 << mode);
        break;
    case 2:
    case 3:
    case 4:
    default:
        self->m_bitFlags &= ~(1 << mode);
        break;
    }
}

// NONMATCHING: the __ptmf_scall argument setup differs slightly.
void fn_1_145A4(CCamera *self)
{
    if (__ptmf_test((PTMF *)&self->m_hook) != 0) {
        __ptmf_scall(self);
    }
    self->m_timer++;
}

// NONMATCHING: the CameraPath temporary sits at a different stack slot, and the PMF store goes direct instead of through a stack temp.
void fn_1_145F8(CCamera *self, s32 flip)
{
    CameraPath path;

    path = s_cameraPath;
    if (flip == 0) {
        path.v[0] *= -1.0f;
    }
    self->m_pathFrom = path;
    self->m_pathTo = path;
    self->m_pathFrom.v[1] += 50.0f;
    self->m_pathFrom.v[2] += 200.0f;
    self->m_hook = s_nullPmf;
    self->m_timer = 0;
}

// NONMATCHING: the /150.0f division is constant-folded through a double literal in the target; ours folds it differently.
void fn_1_14744(CCamera *self)
{
    f32 t;

    t = (f32)self->m_timer / 150.0f;
    if (t > 1.0f) {
        t = 1.0f;
    }
    fn_1_51D4(self->m_unk10, &self->m_pathFrom, &self->m_pathTo, (3.0f - 2.0f * t) * (t * t));
    fn_1_5408(self->m_unk10, 1);
}

// NONMATCHING: the derived-to-base vtable teardown ordering differs.
CCamera *fn_1_14840(CCamera *self, s16 freeIt)
{
    if (self != 0) {
        self->m_vtable = lbl_1_data_5B8;
        if (&self->m_node != 0) {
            if (lbl_1_bss_A7C == 0) {
                fn_1_4C3C("ms_pointer != 0", "singleton.h", 0x3D);
            }
            lbl_1_bss_A7C = 0;
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
}
