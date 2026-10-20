#include "REL/m539Dll.h"

extern s16 omSysExitReq;

extern "C" {

typedef struct M539Hu3DData_s {
    u8 unk_000[0x120];
    void *hookData;
    u8 unk_124[0x134 - 0x124];
} M539Hu3DData;

extern M539Hu3DData Hu3DData[];

void fn_1_2C44(void)
{
    omSysExitReq = 1;
}

void HuMemDirectFree(void *ptr);

// Release an optional owned block; a NULL owner is a no-op.
void fn_1_C8(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

// Second owner type, same lifetime rule.
void fn_1_100(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

// NONMATCHING: register allocation - uses r31/stack frame instead of r0
void fn_1_30BC(void *arg0, void *arg1)
{
    s16 index = *(s16 *)arg0;
    Hu3DData[index].hookData = arg1;
}

// NONMATCHING: register allocation - uses r31/stack frame instead of r0
void *fn_1_30D8(void *arg0)
{
    s16 index = *(s16 *)arg0;
    return (u8 *)&Hu3DData[index] + 0xF0;
}

typedef struct M539UnkStructA_s {
    void *vtable;
    u8 unk_04[0x10];
    void (*func_14)(void *, void *);
    u8 unk_18[0x44];
    void *unk_5C;
} M539UnkStructA;

typedef struct M539UnkStructB_s {
    u8 unk_00[0x14];
    void (*func_14)(void *, void *);
    u8 unk_18[0x44];
    M539UnkStructA *unk_5C;
} M539UnkStructB;

extern void *lbl_1_data_1D4[];
extern void fn_1_4874(void *, void *);
extern void fn_1_48C0(void);
extern void *fn_1_4934(void *, s16);

void fn_1_4854(M539UnkStructA *arg0, M539UnkStructB *arg1)
{
    arg0->vtable = lbl_1_data_1D4;
    arg1->unk_5C = arg0;
    arg1->func_14 = fn_1_4874;
}

typedef struct M539ListNode_s {
    struct M539ListNode_s *prev;
    struct M539ListNode_s *next;
} M539ListNode;

// NONMATCHING: register allocation - uses stack frame and r31/r30 instead of r0/r4
void fn_1_12DC(M539ListNode *node)
{
    M539ListNode *prev = node->prev;
    M539ListNode *next = node->next;

    prev->next = next;
    next->prev = prev;
    node->prev = node;
    node->next = node;
}

void *OSGetCurrentThread(void);
s32 OSGetThreadPriority(void *thread);

// NONMATCHING: register allocation - saves r31 unnecessarily
s32 fn_1_5568(void)
{
    void *thread = OSGetCurrentThread();
    return OSGetThreadPriority(thread);
}

// An 8-byte pair that is handed a back-pointer to itself in both halves.
typedef struct M539Pair_s {
    void *a;
    void *b;
} M539Pair;

extern void *lbl_1_bss_14;
extern u8 lbl_1_bss_24;

void fn_1_1264(M539Pair *self)
{
    self->a = self;
    self->b = self;
}

// Reads the single word parked at lbl_1_bss_14 and hands it straight back.
void *fn_1_1F30(void)
{
    return lbl_1_bss_14;
}

// Doubly-linked node: the two list helpers below splice a node onto the front
// of a chain, which is why both reload the neighbour's head between stores.
typedef struct M539Node_s {
    struct M539Node_s *f00;
    struct M539Node_s *f04;
} M539Node;

// The wrapper that owns a payload block; only two of its fields are touched.
typedef struct M539Inner_s {
    u32 f00;
    u8 pad04[0x14 - 0x04];
    s32 f14;
} M539Inner;

typedef struct M539Outer_s {
    M539Inner *inner;
} M539Outer;

void WipeCreate(s32 wipeId, s32 arg1, s32 arg2);

// NONMATCHING: the store sequence is byte-for-byte the same, but the target
// parks the second parameter in r31 and spills it (frame 0x10) where MWCC
// keeps it in the incoming r4 and needs no frame at all.
// Splice self onto the front of the chain reached through the *other* slot.
void fn_1_1300(M539Node *self, M539Node *other)
{
    self->f04 = other;
    self->f00 = other->f00;
    other->f00->f04 = self;
    other->f00 = self;
}

// NONMATCHING: same r31-vs-r4 allocation difference as fn_1_1300.
// Mirror image of fn_1_1300, walking the pair of offsets swapped.
void fn_1_1330(M539Node *self, M539Node *other)
{
    self->f00 = other;
    self->f04 = other->f04;
    other->f04->f00 = self;
    other->f04 = self;
}

// Latches the payload the first time it is asked to, and only then.
void fn_1_60D4(M539Outer *self)
{
    if (self->inner->f14 == 0) {
        self->inner->f14 = 1;
        self->inner->f00 = 1;
    }
}

// The two wipe descriptors the module installs at boot.
void fn_1_2BEC(void)
{
    WipeCreate(1, 5, 0x3C);
}

void fn_1_2C18(void)
{
    WipeCreate(2, 6, 0x3C);
}

// Two single-field owners: one keyed by a timer handle, one by a model id.
typedef struct M539Timer_s {
    s32 f00;
} M539Timer;

typedef struct M539Model_s {
    s16 f00;
} M539Model;

extern s32 MgTimerValueGet(s32 handle);
extern void MgTimerModeOffSet(s32 handle);
extern void Hu3DModelShadowSet(s16 modelId);
extern void Hu3DModelShadowMapSet(s16 modelId);
extern void Hu3DModelShadowMapObjReset(s16 modelId);

// Thin forwarders. The handle is reloaded through a spilled r31 because the
// callee clobbers every volatile register.
void fn_1_5F30(M539Timer *self)
{
    MgTimerValueGet(self->f00);
}

void fn_1_60A4(M539Timer *self)
{
    MgTimerModeOffSet(self->f00);
}

void fn_1_31E4(M539Model *self)
{
    Hu3DModelShadowSet(self->f00);
}

void fn_1_3214(M539Model *self)
{
    Hu3DModelShadowMapSet(self->f00);
}

void fn_1_327C(M539Model *self)
{
    Hu3DModelShadowMapObjReset(self->f00);
}

extern void Hu3DModelAttrSet(s16 modelId, s32 attr);
extern void Hu3DModelAttrReset(s16 modelId, s32 attr);
extern void Hu3DModelTPLvlSet(s16 modelId, f32 lvl);
extern void Hu3DModelLayerSet(s16 modelId, s16 layer);
extern void Hu3DModelCameraSet(s16 modelId, u16 camBit);
extern double fn_1_4184(void *self);

// NONMATCHING: include/REL/m539Dll.h pins this one to (void *, u32) and seven
// other translation units call through that declaration, so it cannot take a
// named struct type here -- and a void * parameter is exactly what makes MWCC
// spill (stw r3/lwz r3) where the target keeps it in r31.
// The second argument arrives in a volatile register but has to survive the
// call, so each of these stores it to the frame at 0x8 and reloads it.
void fn_1_2E08(void *actor, u32 flags)
{
    Hu3DModelAttrSet(*(s16 *)actor, flags);
}

void fn_1_2E40(M539Model *self, s32 attr)
{
    Hu3DModelAttrReset(self->f00, attr);
}

void fn_1_3014(M539Model *self, f32 lvl)
{
    Hu3DModelTPLvlSet(self->f00, lvl);
}

void fn_1_304C(M539Model *self, s16 layer)
{
    Hu3DModelLayerSet(self->f00, layer);
}

void fn_1_3084(M539Model *self, u16 camBit)
{
    Hu3DModelCameraSet(self->f00, camBit);
}

// The helper hands back a double, so the product only gets rounded on the way
// out -- that frsp is the whole reason the function exists in this shape.
f32 fn_1_4F90(f32 scale)
{
    return scale * fn_1_4184(&lbl_1_bss_24);
}

void fn_1_47E4(void *self)
{
    if (self != 0) {
        HuMemDirectFree(self);
    }
}

// Camera record: the three blocks the Hu3D camera helpers read are at 0x00,
// 0x0C and 0x18, so f0C is a Vec3 and f18 the one right behind it.
typedef struct M539Vec3_s {
    f32 x;
    f32 y;
    f32 z;
} M539Vec3;

typedef struct M539Cam_s {
    u16 f00;
    u8 pad02[0x0C - 0x02];
    M539Vec3 f0C;
    M539Vec3 f18;
} M539Cam;

extern void Hu3DModelShadowMapObjSet(s16 modelId, s32 arg);
extern s16 Hu3DModelCreate(s32 arg);
extern s16 Hu3DHookFuncCreate(s32 arg);
extern void MgTimerKill(s32 handle);
extern s32 MgTimerCreate(s32 mode);
extern void MgTimerPosSet(s32 handle, f32 x, f32 y);
extern void MgTimerPosGet(s32 handle, s32 *x, s32 *y);
extern s32 MgTimerDoneCheck(s32 handle);
extern void Hu3DCameraPosSetV(u16 camId, void *obj, M539Vec3 *a, M539Vec3 *b);
extern void Hu3DCameraPosGet(u16 camId, void *obj, M539Vec3 *a, M539Vec3 *b);

void fn_1_3244(M539Model *self, s32 arg)
{
    Hu3DModelShadowMapObjSet(self->f00, arg);
}

// Hands the freshly built id straight back to the caller.
M539Model *fn_1_2C58(M539Model *self, s32 arg)
{
    self->f00 = Hu3DModelCreate(arg);
    return self;
}

M539Model *fn_1_2CDC(M539Model *self, s32 arg)
{
    self->f00 = Hu3DHookFuncCreate(arg);
    return self;
}

void fn_1_9A0(M539Cam *self, u16 camId)
{
    Hu3DCameraPosSetV(camId, self, &self->f0C, &self->f18);
}

void fn_1_9E0(M539Cam *self, u16 camId)
{
    Hu3DCameraPosGet(camId, self, &self->f0C, &self->f18);
}

void fn_1_5F60(M539Timer *self, f32 x, f32 y)
{
    MgTimerPosSet(self->f00, x, y);
}

void fn_1_5FA0(M539Timer *self, s32 *x, s32 *y)
{
    MgTimerPosGet(self->f00, x, y);
}

// Kills the old timer and parks a fresh one in the same slot.
void fn_1_6100(M539Timer *self)
{
    MgTimerKill(self->f00);
    self->f00 = MgTimerCreate(0);
}

// The neg/or/srwi/clrlwi run is MWCC collapsing "!= 0" into a byte.
u8 fn_1_613C(M539Timer *self)
{
    return MgTimerDoneCheck(self->f00) != 0;
}

// The retail vtables in this module are read at 0x00, 0x08, 0x0C and 0x10 by
// the functions below, so spell all five slots out rather than padding.
typedef s32 (*M539VFn)(void *self, ...);

typedef struct M539Vtbl_s {
    M539VFn v01;
    M539VFn v02;
    M539VFn v03;
    M539VFn v04;
    M539VFn v05;
} M539Vtbl;

typedef struct M539Virtual_s {
    u8 pad00[0x5C];
    M539Vtbl *f5C;
} M539Virtual;

extern void Hu3DModelScaleSet(s16 modelId, f32 x, f32 y, f32 z);
extern void Hu3DModelPosSet(s16 modelId, f32 x, f32 y, f32 z);
extern void Hu3DModelRotSet(s16 modelId, f32 x, f32 y, f32 z);
extern void MgTimerWinColorSet(s32 handle, u8 r, u8 g, u8 b);
extern void MgTimerParamSet(s32 handle, s32 a, s32 b, s32 c);
extern s32 OSIsThreadTerminated(void *thread);
extern void *HuDataSelHeapReadNum(s32 group, s32 first, s32 count);

void fn_1_481C(void *self)
{
    if (self != 0) {
        HuMemDirectFree(self);
    }
}

void fn_1_2F84(M539Model *self, M539Vec3 *v)
{
    Hu3DModelScaleSet(self->f00, v->x, v->y, v->z);
}

// NONMATCHING fn_1_2EF4 / fn_1_2F3C: include/REL/m539Dll.h fixes the first
// argument at void *, and casting it to a struct here still leaves MWCC
// spilling the parameter (stw r3/lwz r3) instead of parking it in r30.
// The call sequence itself is correct.
void fn_1_2EF4(void *actor, Vec *pos)
{
    Hu3DModelPosSet(((M539Model *)actor)->f00, pos->x, pos->y, pos->z);
}

void fn_1_2F3C(void *actor, Vec *rot)
{
    Hu3DModelRotSet(((M539Model *)actor)->f00, rot->x, rot->y, rot->z);
}

// f1 is parked in f31 across the model-id load; stfd is just how MWCC saves a
// float that happens to sit in the double-width half of f31.
void fn_1_2FCC(M539Model *self, f32 scale)
{
    Hu3DModelScaleSet(self->f00, scale, scale, scale);
}

// NONMATCHING: the spill is right but the addend is not -- the target forms
// self+8 in r31 and hands r31 over, MWCC folds the addend into the call setup.
// Here the void * is deliberate: the target spills the argument to 0x8(r1),
// which is exactly what MWCC does for an untyped pointer.
u8 fn_1_5674(void *self)
{
    return OSIsThreadTerminated((u8 *)self + 8) != 0;
}

void fn_1_617C(M539Timer *self, u8 r, u8 g, u8 b)
{
    MgTimerWinColorSet(self->f00, r, g, b);
}

void fn_1_5EE8(M539Timer *self, s32 a, s32 b, s32 c)
{
    MgTimerParamSet(self->f00, a, b, c);
}

// NONMATCHING: the virtual call is right, the prologue is not -- this file
// already declares fn_1_4874 as (void *, void *) so the struct-typed self
// cannot be used and the vtable load ends up through a spill.
// Virtual dispatch through slot 0x0C, loaded via mtctr/bctrl. The second
// argument is part of the published signature but the target never reads it.
void fn_1_4874(void *actor, void *arg1)
{
    M539Virtual *self = (M539Virtual *)actor;

    self->f5C->v03(self);
}

// NONMATCHING: the 0x1000 group number is materialised as lis r4,0x1000 in
// the target and as a single li r4,0x1000 by MWCC -- see the li/lis note in
// docs/HANDOVER.md. The body itself is a straight model build from data.
M539Model *fn_1_2C94(M539Model *self, s32 group)
{
    self->f00 = Hu3DModelCreate((s32)HuDataSelHeapReadNum(group, 0x1000, 2));
    return self;
}

// Stick lookups. The tables hold signed chars (lbz + extsb) and the per-axis
// bias is a double in rodata_28, which is what forces the magic-base int->double
// conversion the target shows before its fmuls.
extern const s8 HuPadStkX[];
extern const s8 HuPadStkY[];
extern const s8 HuPadSubStkX[];
extern const s8 HuPadSubStkY[];
extern const f32 lbl_1_rodata_20;
extern const f32 lbl_1_rodata_30;
extern const f64 lbl_1_rodata_28;

// NONMATCHING fn_1_AB8 / fn_1_B0C / fn_1_BB4 / fn_1_B60: every instruction
// agrees except the final multiply. The target emits "fsubs f0,f0,f1" then
// "fmuls f1,f2,f0" -- a *single*-precision multiply that reads the low half of
// the double result with no frsp between them. MWCC insists on promoting the
// float operand to double and emits fmul + frsp instead, and asking for it
// explicitly with a (f32) cast only makes it reload the bias a second time.
// Bodies below are semantically correct; only that one instruction differs.
f32 fn_1_AB8(s32 idx)
{
    return lbl_1_rodata_20 * (HuPadStkX[idx] - lbl_1_rodata_28);
}

f32 fn_1_B0C(s32 idx)
{
    return (HuPadStkY[idx] - lbl_1_rodata_28) * lbl_1_rodata_20;
}

f32 fn_1_BB4(s32 idx)
{
    return (HuPadSubStkY[idx] - lbl_1_rodata_28) * lbl_1_rodata_30;
}

f32 fn_1_B60(s32 idx)
{
    return (HuPadSubStkX[idx] - lbl_1_rodata_28) * lbl_1_rodata_30;
}

// A look record: two Vec3s at 0x0C and 0x18, matching the two helpers above.
typedef struct M539Look_s {
    u8 pad00[0x0C];
    M539Vec3 f0C;
    M539Vec3 f18;
} M539Look;

typedef struct M539Vec3Tail_s {
    M539Vec3 f00;
    u8 pad0C[0x18 - 0x0C];
    M539Vec3 f18;
} M539Vec3Tail;

extern void PSVECAdd(const Vec *a, const Vec *b, Vec *dst);
extern void C_MTXLookAt(void *out, M539Look *look, M539Vec3 *eye, M539Vec3 *up);
extern void PSMTXInverse(void *mtx, void *out);

// The accumulator is the SECOND parameter: the target moves r3 into r31 and
// r4 into r30, then calls PSVECAdd(acc, &v->f18, &v->f18) and PSVECAdd(acc, v, v).
void fn_1_56C(M539Vec3Tail *v, Vec *acc)
{
    PSVECAdd(acc, (Vec *)&v->f18, (Vec *)&v->f18);
    PSVECAdd(acc, (Vec *)&v->f00, (Vec *)&v->f00);
}

void fn_1_4B0(M539Look *look, void *out)
{
    C_MTXLookAt(out, look, &look->f0C, &look->f18);
    PSMTXInverse(out, out);
}

// fn_1_4184 returns a double, so the lerp is computed in double and only
// rounded on the way out -- that frsp is the last instruction before blr.
f32 fn_1_4FC8(f32 from, f32 to)
{
    return from + (to - from) * fn_1_4184(&lbl_1_bss_24);
}

// Walks a chain of next-pointers and returns the first live node, or NULL.
// The target runs this as a do/while so the test is "still walking" rather
// than "has a node", which is why the head is dereferenced before the check.
typedef struct M539Chain_s {
    struct M539Chain_s *next;
} M539Chain;

M539Chain *fn_1_1B48(M539Chain *head)
{
    M539Chain *node = head;

    while (node != 0) {
        node = node->next;
    }
    return node;
}

// NONMATCHING: 17 of 19 instructions agree. The target lays the do-while out
// as "b <test>; body; test; bne body" -- it jumps over its own loop body on
// entry -- while MWCC falls straight into the body, leaving one branch out and
// shortening the beq by 4.
// Fills a table with the retail "unused" marker, but only once the module's
// init flag has been set. 0xDEADBABE does not fit a 16-bit immediate, so MWCC
// reaches for lis/subi -- and happens to pick the same pair as the target.
extern const u8 lbl_1_bss_9E8;

void fn_1_5D78(s32 *table, s32 count)
{
    s32 *end;

    if (lbl_1_bss_9E8 != 0) {
        end = (s32 *)((u8 *)table + count * 4);
        do {
            *table = 0xDEADBABE;
            table++;
        } while (table != end);
    }
}

// Owner records for the three conditional-release helpers below. The flag is
// an s16 (sth/lha round-trip) and the three differ only in what they release:
// a Hu3D model, an MgTimer handle, or a bare pointer block.
typedef struct M539Link_s {
    s16 f00;
} M539Link;

typedef struct M539Owner_s {
    void *p;
} M539Owner;

// Big record: vtable at 0x000, semaphore at 0x328, and a pointer handed to the
// virtual call at 0x334.
typedef struct M539Big_s {
    M539Vtbl *vtable;
    u8 pad004[4];
    u8 f08[0x328 - 0x08];
    u32 f328;
    u8 pad32C[0x334 - 0x32C];
    void *f334;
} M539Big;

extern s32 Hu3DModelLink(s32 modelId);
extern s32 Hu3DModelKill(s32 modelId);
extern void OSSignalSemaphore(void *sem);

// NONMATCHING fn_1_2DB4: 66.67 %. The target loads the s16 into r0 and then
// sign-extends into r30 with a separate extsh; MWCC folds the two into a single
// "lha r30, 0(r29)". The named s32 below already recovers most of the gap
// (19.05 % -> 66.67 %) by forcing the value into its own register.
// The named s32 is not decoration: without it MWCC passes the lha result
// straight through and drops the extsh the target emits into a second register.
M539Model *fn_1_2DB4(M539Model *self, M539Link *parent)
{
    s32 id = parent->f00;

    self->f00 = Hu3DModelLink(id);
    return self;
}

M539Model *fn_1_30F4(M539Model *self, s16 flag)
{
    if (self != 0) {
        Hu3DModelKill(self->f00);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

M539Timer *fn_1_5E8C(M539Timer *self, s16 flag)
{
    if (self != 0) {
        MgTimerKill(self->f00);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

// Returns self: the target ends with "mr r3, r31" immediately before blr, so
// the pointer is handed back even though every path through it can free it.
void *fn_1_4934(void *self, s16 flag)
{
    if (self != 0) {
        ((M539Owner *)self)->p = lbl_1_data_1D4;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

// Virtual call with (self, self->f334), then post the semaphore and report 0.
// NONMATCHING fn_1_5850: the call sequence is right but the target reserves a
// 0x20 frame against MWCC's 0x10, so every displacement after the prologue is
// 4 bytes short and the body never lines up.
s32 fn_1_5850(M539Big *self)
{
    self->vtable->v03(self, self->f334);
    OSSignalSemaphore(&self->f328);
    return 0;
}

// NONMATCHING fn_1_1D4: 79.17 %, 23 of 24 instructions identical including the
// 0x420 frame and the vararg order. The target emits "crclr cr1eq" before the
// sprintf to declare that no argument went in an FPR; MWCC omits it here.
// Fatal-argument formatter: sprintf into a 0x40C scratch buffer (the frame is
// 0x420 once the three spilled parameters at 0x08..0x10 are accounted for),
// then hand it to OSPanic. The varargs order is param2, param3, param1 --
// r5, r6, r7 in that order, not the source order.
extern const char lbl_1_data_14[];
extern const char lbl_1_data_32[];
extern int sprintf(char *dst, const char *fmt, ...);
extern void OSPanic(const char *file, s32 line, const char *msg);

void fn_1_1D4(s32 arg1, s32 arg2, s32 arg3)
{
    char buf[0x40C];

    sprintf(buf, lbl_1_data_14, arg2, arg3, arg1);
    OSPanic(lbl_1_data_32, 0x12, buf);
}

// Three-float records: f00/f0C/f18 for the axis maths, f00/f18 for the
// direction-copy helper.
typedef struct M539Axis_s {
    M539Vec3 f00;
    M539Vec3 f0C;
    M539Vec3 f18;
} M539Axis;

typedef struct M539Dir_s {
    M539Vec3 f00;
    u8 pad0C[0x18 - 0x0C];
    M539Vec3 f18;
} M539Dir;

extern void PSVECSubtract(const Vec *a, const Vec *b, Vec *dst);
extern void PSMTXRotAxisRad(Vec *dst, Vec *axis, f32 angle);
extern void PSMTXMultVec(Vec *mtx, const Vec *src, Vec *dst);
extern void Hu3DMotionTimeSet(s32 motionId, double time);
extern void fn_1_5B30(void *self, s32 arg);
extern const f32 lbl_1_rodata_10;
extern M539VFn lbl_1_data_1C4[];

// Copies src over dst, then folds the delta it just clobbered into dst->f18.
void fn_1_87C(M539Dir *dst, M539Dir *src)
{
    Vec delta;

    PSVECSubtract((Vec *)src, (Vec *)dst, &delta);
    dst->f00 = src->f00;
    PSVECAdd((Vec *)&dst->f18, &delta, (Vec *)&dst->f18);
}

// NONMATCHING fn_1_3660: 66.67 %. Same extsh fusion as fn_1_2DB4 (MWCC emits
// "lha r31" where the target wants "lha r0" + "extsh r31,r0"), plus MWCC reloads
// lbl_1_rodata_28 after the magic-base pair instead of keeping it in f1.
// magic-base int->double again, but here the result is handed straight to a
// double parameter, so there is no fmuls to disagree about.
void fn_1_3660(M539Model *self, s32 time)
{
    s32 motionId = self->f00;

    Hu3DMotionTimeSet(motionId, time - lbl_1_rodata_28);
}

// NONMATCHING fn_1_39C: call sequence correct, frame wrong. The target reserves
// 0x50 (locals through 0x4F) against MWCC's 0x30 -- the block at 0x18 is 0x38
// bytes in the original, not the 0x0C a Vec3 needs, and I cannot tell from the
// disassembly what the extra 0x2C is holding.
// Rotates around the axis spanned by f18 - f00, then applies the matrix to f0C.
void fn_1_39C(M539Axis *self, f32 angle)
{
    Vec axis;
    Vec delta;

    PSVECSubtract((Vec *)&self->f18, (Vec *)&self->f00, &delta);
    PSMTXRotAxisRad(&axis, &delta, lbl_1_rodata_10 * angle);
    PSMTXMultVec(&axis, (Vec *)&self->f0C, (Vec *)&self->f0C);
}

// Same shape as fn_1_4934, plus an init call on the block at +8.
void *fn_1_5608(void *self, s16 flag)
{
    if (self != 0) {
        ((M539Owner *)self)->p = lbl_1_data_1C4;
        fn_1_5B30((u8 *)self + 8, -1);
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

// Chain node whose child vtable lives at +8, used by the module-wide walk.
typedef struct M539Chain2_s {
    struct M539Chain2_s *next;
    u8 pad04[0x08 - 0x04];
    M539Vtbl *child;
} M539Chain2;

// Record whose vtable pointer sits at 0x5C.
typedef struct M539Del_s {
    u8 pad00[0x5C];
    M539Vtbl *f5C;
} M539Del;

extern void *HuMemDirectMallocNum(s32 heap, s32 size, s32 align);
extern s32 HuPrcCurrentGet(void);
extern void omDelObjEx(s32 treeId, void *obj);
extern u8 lbl_1_bss_C[];
extern u8 lbl_1_data_0[];
extern u8 lbl_1_data_C[];

// Walks the whole chain rooted at lbl_1_bss_C, calling slot 0x10 on each
// child's vtable, and stops when the link comes back to the root.
// NONMATCHING fn_1_1D6C: the walk itself is right, but the target opens a
// 0x20 frame and spills r27-r31 through _savegpr_27 while MWCC keeps the node,
// the sentinel and the child in one register and never spills at all. Tried
// both do/while and for(;;)+break; same result, so it is the loop's register
// demand, not its shape.
void fn_1_1D6C(void)
{
    M539Chain2 *root = (M539Chain2 *)lbl_1_bss_C;
    M539Chain2 *node = root;

    do {
        node->child->v05(node->child);
        node = node->next;
    } while (node != root);
}

// NONMATCHING fn_1_48C4: 82.14 %, 5 of 28 differ, all of them register choice
// -- the target loads the vtable into r29 and reuses r31 for both the call
// argument copy and the return value, MWCC puts the vtable in r31 and the
// result in r29. Naming the vtable pointer in a local is what recovered the
// function from 28.57 %.
// Posts a virtual notification if there is a vtable, then unlinks the object
// from the tree HuPrcCurrentGet() names.
void fn_1_48C4(M539Del *self)
{
    // rc has to stay initialised: without the "rc = 0" MWCC treats it as read
// before set and inserts a crclr, which costs more than the one extra
// instruction the initialiser costs (82.14 % vs 67.86 %).
M539Vtbl *vt = self->f5C;
    s32 rc = 0;

    if (vt != 0) {
        rc = vt->v03(vt, 1);
    }
    omDelObjEx(HuPrcCurrentGet(), self);
}

// NONMATCHING fn_1_0 and fn_1_64: 72 %, 3 of 25 instructions differ. One is
// the li/lis wall -- 0x1000 is "lis r5,0x1000" in the retail build and a single
// "li r5,0x1000" here, which alone makes them unreachable. The other two are
// the knock-on branch layout: the target branches "beq" into the panic call and
// "b" past it, MWCC inverts to a single "bne" over the block. See the li/lis
// rule in docs/HANDOVER.md.
void *fn_1_64(s32 size)
{
    void *block = HuMemDirectMallocNum(0, size, 0x1000);

    if (block == 0) {
        fn_1_1D4((s32)lbl_1_data_0, (s32)lbl_1_data_C, 0x17);
    }
    return block;
}

void *fn_1_0(s32 size)
{
    void *block = HuMemDirectMallocNum(0, size, 0x1000);

    if (block == 0) {
        fn_1_1D4((s32)lbl_1_data_0, (s32)lbl_1_data_C, 0x17);
    }
    return block;
}

// The two words are POINTERS to another node of the same shape, not scalars:
// the target does "stw r0, 0x4(r3)" where r3 is the *value* of f00, which only
// makes sense as node->next = other.
typedef struct M539Swap_s {
    struct M539Swap_s *f00;
    struct M539Swap_s *f04;
} M539Swap;

// The 0x270-entry LCG table; the counter lives immediately after the array.
typedef struct M539Rand_s {
    s32 v[0x270];
    s32 count;
} M539Rand;

extern void PSMTXMultVecSR(void *mtx, void *vec, Vec *dst);

// NONMATCHING fn_1_4994: same virtual-notify-then-unlink as fn_1_48C4 but the
// tree id arrives as a plain argument instead of coming from HuPrcCurrentGet.
// fn_1_4994(s32 treeId, M539Del *self)
void fn_1_4994(s32 treeId, M539Del *self)
{
    M539Vtbl *vt = self->f5C;
    s32 rc = 0;

    if (vt != 0) {
        rc = vt->v03(vt, 1);
    }
    omDelObjEx(treeId, self);
}

// The shuffle is written the way the target has it -- f04 = f00, then f00 =
// f04 -- which is why the binary reloads both words between the two stores
// instead of keeping the old f04 in a register.
// fn_1_6324 is fn_1_1270 plus one extra redundant NULL test on the way in.
// Same node-pointer shuffle, same types.
void *fn_1_6324(void *self, s16 flag)
{
    if (self != 0) {
        if (self != 0) {
            ((M539Swap *)self)->f00->f04 = ((M539Swap *)self)->f04;
            ((M539Swap *)self)->f00 = ((M539Swap *)self)->f04;
        }
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

// NONMATCHING fn_1_1270 (88.89 %) and fn_1_6324 (89.66 %): 4-5 instructions
// differ, all reloads. The target reloads BOTH self->f00 and self->f04 after
// the "f00->f04 = f04" store; MWCC proves the store cannot alias self and
// keeps one in a register. Typing the two words as node POINTERS rather than
// scalars is what took fn_1_1270 from 77.78 % to 88.89 % -- the target's
// "stw r0, 0x4(r3)" stores through the value of f00, not into self.
void *fn_1_1270(void *self, s16 flag)
{
    if (self != 0) {
        ((M539Swap *)self)->f00->f04 = ((M539Swap *)self)->f04;
        ((M539Swap *)self)->f00 = ((M539Swap *)self)->f04;
        if (flag > 0 && self != 0) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

// Numerical Recipes LCG, 0x6C078965. Both constants are too wide for li, so
// MWCC reaches for lis/subi exactly as the target does. The u32 casts are load
// bearing: the target uses srwi and cmplwi, i.e. the shift and the bound test
// are LOGICAL, not arithmetic.
void fn_1_36CC(M539Rand *r, s32 seed)
{
    r->v[0] = seed;
    r->count = 1;
    while ((u32)r->count < 0x270) {
        r->v[r->count] = r->count +
            (((u32)r->v[r->count - 1] ^ ((u32)r->v[r->count - 1] >> 30)) * 0x6C078965);
        r->count++;
    }
}

// A 0x30-byte transform matrix, per the frame layouts below.
typedef u8 Mat[0x30];

// Pose record: three Vec3s at 0x18, 0x24 and 0x30, which is exactly the
// triplet the three Hu3D setters read.
typedef struct M539Pose_s {
    u8 pad00[0x18];
    M539Vec3 f18;
    M539Vec3 f24;
    M539Vec3 f30;
} M539Pose;

// Record with a semaphore at 0x328 and a vtable pointer at 0x334.
typedef struct M539Big2_s {
    M539VFn f00;
    u8 pad04[0x318 - 0x04];
    u32 f318;
    u8 pad31C[0x334 - 0x31C];
    M539VFn f334;
} M539Big2;

extern void C_QUATMtx(void *dst, void *mtx);
extern s32 OSGetThreadPriority(void *thread);
extern void OSInitSemaphore(void *sem, s32 count);
extern void fn_1_5A0C(void *self, s32 a, s32 b, s32 c, s32 d, s32 e, u16 f);
extern void fn_1_58AC(void *dst, s32 (*fn)(M539Big *), void *self, s32 a, s32 b, s32 c);

// Builds the view matrix from the two Vec3s, copies the position, then turns
// the inverted matrix into the quaternion stored at out+0xC.
void fn_1_6F4(M539Axis *self, M539Axis *out)
{
    Mat m;

    C_MTXLookAt(m, (M539Look *)self, (M539Vec3 *)&self->f0C, (M539Vec3 *)&self->f18);
    PSMTXInverse(m, m);
    out->f00 = self->f00;
    C_QUATMtx(&out->f0C, m);
}

// Loads position, rotation and scale straight out of the pose record. The model
// id is hoisted into a local so the three calls share one lha.
void fn_1_2E78(M539Model *self, M539Pose *pose)
{
    s16 id = self->f00;

    Hu3DModelPosSet(id, pose->f18.x, pose->f18.y, pose->f18.z);
    Hu3DModelRotSet(id, pose->f24.x, pose->f24.y, pose->f24.z);
    Hu3DModelScaleSet(id, pose->f30.x, pose->f30.y, pose->f30.z);
}

// Submits a process and posts its semaphore. The current thread handle is
// fetched inline so its priority can be forwarded to fn_1_58AC.
void *fn_1_558C(void *self, s32 arg1)
{
    s32 prio;

    ((M539Big *)self)->vtable = (M539Vtbl *)lbl_1_data_1C4;
    prio = OSGetThreadPriority(OSGetCurrentThread());
    fn_1_58AC(((M539Big *)self)->f08, &fn_1_5850, self, arg1, prio, 1);
    OSInitSemaphore(&((M539Big *)self)->f328, 0);
    return self;
}

// Zeroes the field at 0x318 and forwards everything, with the fourth argument
// right-shifted by two on the way through. The sixth argument is a u16 (the
// prologue spills it with sth and reloads it with lhz); the fourth is u32 so
// the shift comes out as srwi.
void *fn_1_5990(void *self, s32 a, s32 b, s32 c, u32 d, s32 e, u16 f)
{
    u32 t;

    ((M539Big2 *)self)->f318 = 0;
    t = d >> 2;
    fn_1_5A0C(self, a, b, c, t, e, f);
    return self;
}

// Empty translation-unit anchors: a bare blr each.
void fn_1_1DDC(void)
{
}

void fn_1_246C(void)
{
}

void fn_1_48C0(void)
{
}

}