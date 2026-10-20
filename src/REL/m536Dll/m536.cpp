#include "types.h"

// m536 top-level unit. It owns the 16-byte vtable lbl_1_data_1D4 (two null
// slots, then fn_1_4934 and fn_1_48C0), which fn_1_4854 installs into the
// freshly built object. fn_1_30BC and fn_1_30D8 are the two accessors for the
// 0x134-byte Hu3D per-model record: one writes the field at 0x120, the other
// hands back the sub-record based at 0xF0. fn_1_12DC splices a list node into
// itself (both links end up pointing at the node itself).
extern "C" {

typedef void (*M536VtableFn)(void);

void fn_1_4874(void);
void fn_1_48C0(void);
void fn_1_4934(void);

// 16 bytes at .data+0x1D4: two null slots then two handler pointers.
struct M536Vtable {
    void *m_unk0;
    void *m_unk4;
    M536VtableFn m_fn8;
    M536VtableFn m_fnC;
};

M536Vtable lbl_1_data_1D4 = {0, 0, fn_1_4934, fn_1_48C0};

// Doubly-linked list node; fn_1_12DC is the self-splice.
struct M536Node {
    M536Node *m_next;
    M536Node *m_prev;
};

// The 0x30-byte tail block of a Hu3D record, based at offset 0xF0.
struct M536Hu3DPart {
    s32 m_unk00[0x30 / 4];
};

// One per-model Hu3D record, 0x134 bytes.
struct M536Hu3DSlot {
    s32 m_unk00[0xF0 / 4];
    M536Hu3DPart m_part;
    s32 m_value;
    s32 m_unk124[4];
};

extern M536Hu3DSlot Hu3DData[];
extern s16 omSysExitReq;

// .bss layout, reproduced in declaration order so the labels land at the
// same offsets as the original object (0x9F0 bytes total).
u8 lbl_1_bss_0[0xC];
u8 lbl_1_bss_C[0x8];
void *lbl_1_bss_14;
u8 lbl_1_bss_18[0x4];
u8 lbl_1_bss_1C[0x4];
u8 lbl_1_bss_20[0x4];
u8 lbl_1_bss_24[0x9C4];
u8 lbl_1_bss_9E8[0x1];
u8 gap_05_000009E9_bss[0x3];
u8 lbl_1_bss_9EC[0x4];

typedef s16 HU3DMODELID;

// Vector triple used by the position/rotation forwarders.
typedef struct M536Vec {
    f32 x;
    f32 y;
    f32 z;
} M536Vec;

// Transform block: position, rotation and scale triples back to back.
typedef struct M536Xform {
    u8 m_unk00[0x18];
    M536Vec pos;
    M536Vec rot;
    M536Vec scale;
} M536Xform;

// Tail of the object pointed at by the 4-byte reference fn_1_5DB0 walks.
typedef struct M536TimerObj {
    s32 m_unk00;
    s32 m_unk04[4];
    s32 m_unk14;
} M536TimerObj;

typedef struct M536TimerRef {
    M536TimerObj *m_unk0;
} M536TimerRef;

void WipeCreate(s16 mode, s16 type, s16 maxTime);
HU3DMODELID Hu3DModelCreate(void *data);
HU3DMODELID Hu3DHookFuncCreate(void *hookFunc);
void Hu3DModelAttrSet(HU3DMODELID modelId, u32 attr);
void Hu3DModelAttrReset(HU3DMODELID modelId, u32 attr);
void Hu3DModelPosSet(HU3DMODELID modelId, f32 x, f32 y, f32 z);
void Hu3DModelRotSet(HU3DMODELID modelId, f32 x, f32 y, f32 z);
void Hu3DModelScaleSet(HU3DMODELID modelId, f32 x, f32 y, f32 z);
HU3DMODELID Hu3DModelLink(HU3DMODELID linkMdlId);
void Hu3DModelTPLvlSet(HU3DMODELID modelId, f32 lvl);
void Hu3DModelLayerSet(HU3DMODELID modelId, s16 layerNo);
void Hu3DModelCameraSet(HU3DMODELID modelId, u16 cameraBit);
void Hu3DModelShadowSet(HU3DMODELID modelId);
void Hu3DModelShadowMapSet(HU3DMODELID modelId);
void Hu3DModelShadowMapObjSet(HU3DMODELID modelId, char *objName);
void Hu3DModelShadowMapObjReset(HU3DMODELID modelId);
void *HuDataSelHeapReadNum(int dataNum, s32 num, int heap);
void Hu3DCameraPosSetV(int cameraBit, M536Vec *pos, M536Vec *up, M536Vec *target);
void Hu3DCameraPosGet(int cameraBit, M536Vec *pos, M536Vec *up, M536Vec *target);
void PSVECSubtract(const M536Vec *a, const M536Vec *b, M536Vec *out);
void PSMTXRotAxisRad(f32 (*m)[4], const M536Vec *axis, f32 rad);
void PSMTXMultVec(const f32 (*m)[4], const M536Vec *src, M536Vec *dst);
void *MgTimerCreate(int type);
void MgTimerKill(void *timer);
int MgTimerDoneCheck(void *timer);
void MgTimerWinColorSet(void *timer, u8 r, u8 g, u8 b);
void HuMemDirectFree(void *ptr);
int MgTimerValueGet(void *timer);
void MgTimerModeOffSet(void *timer);
typedef struct OSThread OSThread;
OSThread *OSGetCurrentThread(void);
s32 OSGetThreadPriority(OSThread *thread);

#define M536_MEMNUM_OVL 0x10000000
#define M536_HEAP_MODEL 2

void fn_1_12DC(M536Node *node)
{
    node->m_next->m_prev = node->m_prev;
    node->m_prev->m_next = node->m_next;
    node->m_next = node;
    node->m_prev = node;
}

void fn_1_2C44(void)
{
    omSysExitReq = 1;
}

void fn_1_30BC(s16 *index, s32 value)
{
    Hu3DData[*index].m_value = value;
}

void *fn_1_30D8(s16 *index)
{
    return &Hu3DData[*index].m_part;
}

// Constructed object: the vtable goes into the object, the object is handed to
// its container at 0x5C, and the container's entry point at 0x14 is set.
struct M536Object {
    void *m_vtable;
};

struct M536Container {
    s32 m_unk00[0x14 / 4];
    M536VtableFn m_fn14;
    s32 m_unk18[(0x5C - 0x18) / 4];
    void *m_unk5C;
};

void fn_1_4854(M536Object *object, M536Container *container)
{
    object->m_vtable = &lbl_1_data_1D4;
    container->m_unk5C = object;
    container->m_fn14 = fn_1_4874;
}

// --- bodies with a bare `blr` in the original object ---

void fn_1_1DDC(void)
{
}

void fn_1_246C(void)
{
}

void fn_1_48C0(void)
{
}

// --- circular-list splicing ---

void fn_1_1264(M536Node *node)
{
    node->m_next = node;
    node->m_prev = node;
}

// Insert `node` after `at`.
void fn_1_1300(M536Node *node, M536Node *at)
{
    M536Node *anchor = at;

    node->m_prev = anchor;
    node->m_next = anchor->m_next;
    anchor->m_next->m_prev = node;
    anchor->m_next = node;
}

// Insert `node` before `at`.
void fn_1_1330(M536Node *node, M536Node *at)
{
    M536Node *anchor = at;

    node->m_next = anchor;
    node->m_prev = anchor->m_prev;
    anchor->m_prev->m_next = node;
    anchor->m_prev = node;
}

void *fn_1_1F30(void)
{
    return lbl_1_bss_14;
}

void fn_1_2BEC(void)
{
    WipeCreate(1, 5, 0x3C);
}

void fn_1_2C18(void)
{
    WipeCreate(2, 6, 0x3C);
}

s16 *fn_1_2C58(s16 *out, void *data)
{
    *out = Hu3DModelCreate(data);
    return out;
}

s16 *fn_1_2C94(s16 *out, int dataNum)
{
    *out = Hu3DModelCreate(HuDataSelHeapReadNum(dataNum, M536_MEMNUM_OVL, M536_HEAP_MODEL));
    return out;
}

s16 *fn_1_2CDC(s16 *out, void *hookFunc)
{
    *out = Hu3DHookFuncCreate(hookFunc);
    return out;
}

s16 *fn_1_2DB4(s16 *out, s16 *modelId)
{
    s16 id = (s16)*modelId;

    *out = Hu3DModelLink(id);
    return out;
}

void fn_1_2E78(s16 *modelId, M536Xform *xform)
{
    s16 id = *modelId;

    Hu3DModelPosSet(id, xform->pos.x, xform->pos.y, xform->pos.z);
    Hu3DModelRotSet(id, xform->rot.x, xform->rot.y, xform->rot.z);
    Hu3DModelScaleSet(id, xform->scale.x, xform->scale.y, xform->scale.z);
}

void fn_1_2E08(s16 *modelId, u32 attr)
{
    Hu3DModelAttrSet(*modelId, attr);
}

void fn_1_2E40(s16 *modelId, u32 attr)
{
    Hu3DModelAttrReset(*modelId, attr);
}

void fn_1_2EF4(s16 *modelId, M536Vec *pos)
{
    Hu3DModelPosSet(*modelId, pos->x, pos->y, pos->z);
}

void fn_1_2F3C(s16 *modelId, M536Vec *rot)
{
    Hu3DModelRotSet(*modelId, rot->x, rot->y, rot->z);
}

void fn_1_2F84(s16 *modelId, M536Vec *scale)
{
    Hu3DModelScaleSet(*modelId, scale->x, scale->y, scale->z);
}

void fn_1_2FCC(s16 *modelId, f32 scale)
{
    Hu3DModelScaleSet(*modelId, scale, scale, scale);
}

void fn_1_3014(s16 *modelId, f32 lvl)
{
    Hu3DModelTPLvlSet(*modelId, lvl);
}

void fn_1_304C(s16 *modelId, s16 layerNo)
{
    Hu3DModelLayerSet(*modelId, layerNo);
}

void fn_1_3084(s16 *modelId, u16 cameraBit)
{
    Hu3DModelCameraSet(*modelId, cameraBit);
}

void fn_1_3244(s16 *modelId, char *objName)
{
    Hu3DModelShadowMapObjSet(*modelId, objName);
}

void fn_1_31E4(s16 *modelId)
{
    Hu3DModelShadowSet(*modelId);
}

void fn_1_3214(s16 *modelId)
{
    Hu3DModelShadowMapSet(*modelId);
}

void fn_1_327C(s16 *modelId)
{
    Hu3DModelShadowMapObjReset(*modelId);
}

void fn_1_5C0C(M536TimerRef *ref)
{
    MgTimerValueGet(ref->m_unk0);
}

void fn_1_5D80(M536TimerRef *ref)
{
    MgTimerModeOffSet(ref->m_unk0);
}

void fn_1_5DB0(M536TimerRef *ref)
{
    if (ref->m_unk0->m_unk14 == 0) {
        ref->m_unk0->m_unk14 = 1;
        ref->m_unk0->m_unk00 = 1;
    }
}

// --- null-guarded frees ---

void fn_1_C8(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_100(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_47E4(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_481C(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

s32 fn_1_5244(void)
{
    return OSGetThreadPriority(OSGetCurrentThread());
}

}
