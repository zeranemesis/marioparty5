#include "dolphin/types.h"
extern void Hu3DModelAttrSet(s16 modelId, u32 attr);
typedef struct { u8 pad[0x184]; s16 modelId; } M508OBJ;
#include "game/process.h"

float fn_1_5D20(float *arg0, float *arg1)
{
    return arg0[0] * arg1[0] + arg0[1] * arg1[1];
}

float fn_1_5D40(float *arg0, float *arg1)
{
    return arg0[0] * arg1[1] - arg0[1] * arg1[0];
}

// Empty translation-unit anchors: a bare blr each.
void fn_1_9B38(void)
{
}

void fn_1_828C(void)
{
}

// Single-field readers.
s32 fn_1_81B8(void *obj)
{
    return *(s32 *)((u8 *)obj + 0x188);
}

s32 fn_1_7FD8(void *obj)
{
    return *(s32 *)((u8 *)obj + 0x158);
}

f32 fn_1_7F58(void *obj)
{
    return *(f32 *)((u8 *)obj + 0x1B0);
}

// Constant writers.
void fn_1_4150(void *obj)
{
    *(s32 *)((u8 *)obj + 0x04) = 2;
}

void fn_1_2838(void *obj)
{
    *(s32 *)((u8 *)obj + 0x04) = 2;
}

void fn_1_25A8(void *obj)
{
    *(s32 *)((u8 *)obj + 0x04) = 0;
}

void fn_1_4374(void *obj)
{
    *(s32 *)((u8 *)obj + 0x04) = 3;
}

void fn_1_8D24(void *obj)
{
    *(s32 *)((u8 *)obj + 0x00) = 0;
}

// Constant writers, second wave -- same +4 slot, distinct values.
void fn_1_3B8C(void *obj)
{
    *(s32 *)((u8 *)obj + 0x04) = 1;
}

void fn_1_3A38(void *obj)
{
    *(s32 *)((u8 *)obj + 0x04) = 0;
}

void fn_1_4790(void *obj)
{
    *(s32 *)((u8 *)obj + 0x04) = 5;
}

void fn_1_469C(void *obj)
{
    *(s32 *)((u8 *)obj + 0x04) = 4;
}

void fn_1_2F80(void *obj)
{
    *(s32 *)((u8 *)obj + 0x04) = 4;
}

void fn_1_2C7C(void *obj)
{
    *(s32 *)((u8 *)obj + 0x04) = 3;
}

void fn_1_33D8(void *obj)
{
    *(s32 *)((u8 *)obj + 0x04) = 5;
}

void fn_1_2388(void *obj)
{
    *(s32 *)((u8 *)obj + 0x04) = 1;
}

void fn_1_57D4(void *obj)
{
    *(s32 *)((u8 *)obj + 0x04) = 0;
}

void fn_1_5518(void *obj)
{
    *(s32 *)((u8 *)obj + 0x04) = 1;
}

// Constant writers, third wave.
void fn_1_49EC(void *obj)
{
    *(s32 *)((u8 *)obj + 0x04) = 7;
}

void fn_1_5078(void *obj)
{
    *(s32 *)((u8 *)obj + 0x04) = 0;
}

void fn_1_8C64(void *obj)
{
    *(s32 *)((u8 *)obj + 0x00) = 2;
}

void fn_1_5464(void *obj)
{
    *(s32 *)((u8 *)obj + 0x04) = 0;
}

void fn_1_58B8(void *obj)
{
    *(s32 *)((u8 *)obj + 0x04) = 1;
}

void fn_1_4884(void *obj)
{
    *(s32 *)((u8 *)obj + 0x04) = 6;
}

// Writes 1 into the base's +4 and the argument into a second object hanging off
// +0x18. The target computes "self + 0x18" into a register first and stores through
// that, which is why the +0x1C access has to go through a named struct rather than
// a byte cast.
typedef struct M508Obj2_s {
    s32 f00;
    s32 f04;
} M508Obj2;

typedef struct M508Sub2_s {
    s32 f00;
    s32 f04;
    u8 pad08[0x18 - 0x08];
    M508Obj2 f18;
} M508Sub2;

void fn_1_2084(M508Sub2 *self, s32 arg1)
{
    M508Obj2 *sub = &self->f18;

    sub->f04 = arg1;
    self->f04 = 1;
}

// Two-dimensional cross product: a0*b1 - a1*b0.
f32 fn_1_A174(f32 *a, f32 *b)
{
    return a[0] * b[1] - a[1] * b[0];
}

// Shadow and hook forwarders -- the model id sits at 0x184 in M508OBJ.
void fn_1_822C(M508OBJ *obj)
{
    Hu3DModelShadowReset(obj->modelId);
}

void fn_1_825C(M508OBJ *obj)
{
    Hu3DModelShadowMapSet(obj->modelId);
}

void fn_1_81FC(M508OBJ *obj)
{
    Hu3DModelShadowSet(obj->modelId);
}

void fn_1_82D8(M508OBJ *obj)
{
    Hu3DModelHookReset(obj->modelId);
}

// Window forwarders -- these read the s16 at offset 0.
// NONMATCHING: 58.33%. The target parks the object pointer in the callee-saved
// register r31 across the single call (stw r31 / mr r31,r3 / epilogue lwz r31);
// MWCC keeps it in the frame (stw r3,0x8 / lwz r3,0x8) instead. The call
// sequence itself (lha + bl HuWinEx*) is byte-exact; only the register choice
// for the surviving parameter differs, and it is not controllable from C.
void fn_1_A0B4(void *obj)
{
    HuWinExOpen(*(s16 *)obj);
}

void fn_1_A0E4(void *obj)
{
    HuWinExClose(*(s16 *)obj);
}

void fn_1_A084(void *obj)
{
    HuWinMesWait(*(s16 *)obj);
}

// Loads a transform target: flag at 0x04, then id+x+y+z packed at 0x18..0x24.
typedef struct M508Pack_s {
    s32 id;
    f32 x;
    f32 y;
    f32 z;
} M508Pack;

typedef struct M508Pose2_s {
    u8 pad00[4];
    s32 flag;
    u8 pad08[0x18 - 0x08];
    M508Pack pack;
} M508Pose2;


// Turns the window on and marks it active (state word at +0x0C).
void fn_1_9FA0(void *obj)
{
    HuWinDispOn(*(s16 *)obj);
    *(s32 *)((u8 *)obj + 0x0C) = 1;
}

// Passes the message on to the window's text printer.
void fn_1_9F68(void *obj, s32 msg)
{
    HuWinMesSet(*(s16 *)obj, msg);
}

// Increments the counter at +0x30 while it stays below 3, then raises the
// flag at +0x04.
typedef struct M508Pose3_s {
    u8 pad00[4];
    s32 flag;
    u8 pad08[0x30 - 0x08];
    s32 counter;
} M508Pose3;

void fn_1_512C(M508Pose3 *self)
{
    if (self->counter < 3) {
        self->counter++;
    }
    self->flag = 1;
}

// Sets the shadow attribute bit 0x4000 | 2 on the model.
// NONMATCHING: 85.71%. The target materialises 0x4002 as "lis r4,0x4000 /
// addi r4,r4,2"; MWCC uses a single "li r4,0x4002". Covered by the li/lis
// rule in docs/HANDOVER.md: any literal whose high 16 bits are non-zero is
// emitted with lis by the retail compiler and li by MWCC, so the two
// instruction streams cannot be reconciled from C.
void fn_1_80B4(M508OBJ *obj)
{
    Hu3DModelAttrSet(obj->modelId, 0x4002);
}

// Sets the message scroll speed.
void fn_1_A04C(void *obj, s16 speed)
{
    HuWinMesSpeedSet(*(s16 *)obj, speed);
}

// Kills the process parked at +0x0C, then frees the object and hands it back.
extern void Hu3DModelPosSet(s16 modelId, f32 x, f32 y, f32 z);
extern void HuMemDirectFree(void *ptr);
// Kills the process parked at +0x0C, then frees the object and hands it back.
// NONMATCHING: 93.33%. MWCC emits an extra "mr r3, r31" to materialise the
// return value after HuMemDirectFree; the target leaves r3 untouched and lets
// the epilogue reuse it. One instruction, not controllable from C.
extern void HuMemDirectFree(void *ptr);
void *fn_1_88E0(void *obj)
{
    HuPrcKill(*(void **)((u8 *)obj + 0x0C));
    HuMemDirectFree(obj);
    return obj;
}

// Conditionally copies three floats out of self into the caller's pointers.
// NONMATCHING: 62.50%. The target tests NULL with "li r0,0 / cmplw r4,r0";
// MWCC uses "cmplwi r4,0". The stores themselves are byte-exact.
// Each destination is written only when non-NULL.
void fn_1_7DB4(void *self, f32 *d1, f32 *d2, f32 *d3)
{
    if (d1 != 0) {
        *d1 = *(f32 *)((u8 *)self + 0x160);
    }
    if (d2 != 0) {
        *d2 = *(f32 *)((u8 *)self + 0x164);
    }
    if (d3 != 0) {
        *d3 = *(f32 *)((u8 *)self + 0x168);
    }
}

// NONMATCHING: 62.50%. Same "li r0,0 / cmplw r4,r0" vs "cmplwi r4,0" as
// fn_1_7DB4.
void fn_1_7EDC(void *self, f32 *d1, f32 *d2, f32 *d3)
{
    if (d1 != 0) {
        *d1 = *(f32 *)((u8 *)self + 0x178);
    }
    if (d2 != 0) {
        *d2 = *(f32 *)((u8 *)self + 0x17c);
    }
    if (d3 != 0) {
        *d3 = *(f32 *)((u8 *)self + 0x180);
    }
}

// NONMATCHING: 62.50%. Same "li r0,0 / cmplw r4,r0" vs "cmplwi r4,0" as
// fn_1_7DB4.
void fn_1_7E48(void *self, f32 *d1, f32 *d2, f32 *d3)
{
    if (d1 != 0) {
        *d1 = *(f32 *)((u8 *)self + 0x16c);
    }
    if (d2 != 0) {
        *d2 = *(f32 *)((u8 *)self + 0x170);
    }
    if (d3 != 0) {
        *d3 = *(f32 *)((u8 *)self + 0x174);
    }
}

// NONMATCHING: 94.12%. MWCC emits an extra "mr r3, r31" to materialise the
// return value after the final HuMemDirectFree; the target leaves r3 untouched.
// One instruction, not controllable from C.
// Frees the object at +0x10, kills its process at +0x00, frees itself, returns self.
void *fn_1_797C(void *self)
{
    HuMemDirectFree(*(void **)((u8 *)self + 0x10));
    HuPrcKill(*(void **)self);
    HuMemDirectFree(self);
    return self;
}

// NONMATCHING: 94.12%. Same extra "mr r3, r31" after HuMemDirectFree as
// fn_1_797C / fn_1_88E0.
// Frees the process at +0x10, kills its process at +0x00, frees itself, returns self.
void *fn_1_9EB8(void *self)
{
    HuWinKill(*(s16 *)self);
    HuPrcKill(*(void **)((u8 *)self + 0x10));
    HuMemDirectFree(self);
    return self;
}

// Sets the window scale; x and y are parked at +0x04 and +0x08.
typedef struct M508Win2_s {
    s16 id;
    u8 pad02[2];
    f32 x;
    f32 y;
} M508Win2;

// NONMATCHING: 37.50%. The target parks the object pointer in r31 across the
// call; MWCC keeps it in the frame. The call sequence itself is byte-exact.
void fn_1_A114(void *obj, f32 x, f32 y)
{
    HuWinScaleSet(*(s16 *)obj, x, y);
}

// NONMATCHING: 36.36%. The target parks the object pointer in r31 across the
// call; MWCC keeps it in the frame.
void fn_1_9F0C(M508Win2 *self, f32 x, f32 y)
{
    self->x = x;
    self->y = y;
    HuWinPosSet(self->id, x, y);
}

// Compares the s32 at +0x1C with the global lbl_1_bss_110.
// NONMATCHING: 38.89%. MWCC compiles the 1/0 return differently than the
// target's cmpwi/beq/li pair.
extern s32 lbl_1_bss_110;
s32 fn_1_4654(void *self)
{
    return (*(s32 *)((u8 *)self + 0x1C) == lbl_1_bss_110) ? 1 : 0;
}

// Calls fn_1_7A3C with fixed arguments and returns its result.
// NONMATCHING: 65.00%. MWCC lays out the seven-argument call to fn_1_7A3C
// with different register spills than the target; four instructions differ.
// Calls fn_1_7A3C with fixed arguments and returns its result.
extern void *lbl_1_data_98[];
extern void *fn_1_7A3C(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, void *g);
void *fn_1_5414(void)
{
    return fn_1_7A3C(0x1000, 0x2000, -1, 0, 0, 2, lbl_1_data_98);
}

// Sets the camera perspective from four floats parked at +0x08..+0x14.
// NONMATCHING: 30.00%. The target parks the object pointer in r31 across the
// call; MWCC keeps it in the frame.
extern void Hu3DCameraPerspectiveSet(s16 camId, f32 fov, f32 aspect, f32 near, f32 far);
void fn_1_95D8(void *obj, f32 fov, f32 aspect, f32 near, f32 far)
{
    Hu3DCameraPerspectiveSet(*(s16 *)((u8 *)obj + 0x08), fov, aspect, near, far);
}

// NONMATCHING: 45.00%. The cmpwi/beq/li return sequence is correct; MWCC
// materialises the 1/0 result differently (the same srawi/srwi pattern seen
// elsewhere). Not controllable from C.
// Returns 1 when the model's motion has ended, 0 otherwise.
s32 fn_1_80EC(M508OBJ *obj)
{
    return Hu3DMotionEndCheck(obj->modelId) != 0;
}

// NONMATCHING: 11.11%. MWCC stores the three float parameters into its frame
// and reloads them for each store; the target keeps them in f1/f2/f3 and stores
// straight through an r31 base. The register allocation is not controllable
// from C.
// Stores a 3-vector at +0x178..+0x180 and applies it as scale.
void fn_1_7E88(M508OBJ *obj, f32 x, f32 y, f32 z)
{
    *(f32 *)((u8 *)obj + 0x178) = x;
    *(f32 *)((u8 *)obj + 0x17c) = y;
    *(f32 *)((u8 *)obj + 0x180) = z;
    Hu3DModelScaleSet(obj->modelId, x, y, z);
}

// NONMATCHING: 11.11%. Same frame-vs-r31 register allocation as fn_1_7E88.
// Stores a 3-vector at +0x160..+0x168 and applies it as position.
void fn_1_7D60(M508OBJ *obj, f32 x, f32 y, f32 z)
{
    *(f32 *)((u8 *)obj + 0x160) = x;
    *(f32 *)((u8 *)obj + 0x164) = y;
    *(f32 *)((u8 *)obj + 0x168) = z;
    Hu3DModelPosSet(obj->modelId, x, y, z);
}

// NONMATCHING: 11.11%. Same frame-vs-r31 register allocation as fn_1_7E88.
// Stores a 3-vector at +0x16c..+0x174 and applies it as rotation.
void fn_1_7DF4(M508OBJ *obj, f32 x, f32 y, f32 z)
{
    *(f32 *)((u8 *)obj + 0x16c) = x;
    *(f32 *)((u8 *)obj + 0x170) = y;
    *(f32 *)((u8 *)obj + 0x174) = z;
    Hu3DModelRotSet(obj->modelId, x, y, z);
}

// Calls fn_1_7A3C with fixed arguments; 0x4220 is emitted as lis+addi by the
// retail build and as li by MWCC (the li/lis rule).
// NONMATCHING: 65.00%. The lis/lis materialisation of 0x4220 cannot be
// reproduced from C; see docs/HANDOVER.md.
extern void *lbl_1_data_34[];
extern void *fn_1_7A3C(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, void *g);
void *fn_1_2260(void)
{
    return fn_1_7A3C(0x1000, 0x2000, 0x4220, 0, 0, 2, lbl_1_data_34);
}

// NONMATCHING: 18.18%. Frame-vs-r31 register allocation plus the s16 load at
// +0x1bc; the target keeps self in r31, MWCK uses the frame.
// If arg1 is non-zero, creates hook dust from the s16 at +0x1bc; otherwise
// resets the model hook at +0x184.
extern void CharModelHookDustCreate(s16 modelId, s32 arg);
void fn_1_8698(void *self, s32 arg1)
{
    if (arg1 != 0) {
        CharModelHookDustCreate(*(s16 *)((u8 *)self + 0x1bc), arg1);
    } else {
        Hu3DModelHookReset(*(s16 *)((u8 *)self + 0x184));
    }
}

// NONMATCHING: 52.17%. The wipe/sleep loop and the s16 argument are correct;
// the remaining diffs are frame-vs-r31 register allocation for the self
// parameter, not controllable from C.
// Wipes with (1, 5, arg1) then sleeps until the wipe ends.
extern s32 WipeCreate(s32 type, s32 mode, s32 frame);
extern s32 WipeCheckEnd(void);
extern void HuPrcVSleep(void);
void fn_1_9B3C(void *self, s32 arg1)
{
    WipeCreate(1, 5, (s16)arg1);
    do {
        HuPrcVSleep();
    } while (WipeCheckEnd() != 0);
}

// NONMATCHING: 52.17%. Same wipe/sleep loop shape as fn_1_9B3C; frame-vs-r31
// register allocation for the remaining diffs.
// Wipes with (2, 6, arg1) then sleeps until the wipe ends.
void fn_1_9B98(void *self, s32 arg1)
{
    WipeCreate(2, 6, (s16)arg1);
    do {
        HuPrcVSleep();
    } while (WipeCheckEnd() != 0);
}

// NONMATCHING: 25.00%. Frame-vs-r31 register allocation for the self parameter
// across six float stores; not controllable from C.
// Sets the camera viewport from six floats parked at +0x08..+0x1c.
extern void Hu3DCameraViewportSet(s16 camId, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f);
void fn_1_9578(void *obj, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f)
{
    Hu3DCameraViewportSet(*(s16 *)((u8 *)obj + 0x08), a, b, c, d, e, f);
}

// Inner sprite group: id at +0x00, position at +0x10..+0x14, a second id at +0x1c.
typedef struct M508Inner2_s {
    s16 id;
    u8 pad02[0x10 - 0x02];
    f32 x;
    f32 y;
    s16 id2;
} M508Inner2;

typedef struct M508Outer2_s {
    u8 pad00[0x18];
    M508Inner2 inner;
} M508Outer2;

// NONMATCHING: 19.23%. The inner-sprite-group struct layout and both calls are
// correct; the remaining diffs are frame-vs-r31 register allocation.
// Stores the position into the inner sprite group, then sets both the sprite
// group and the score window positions.
extern void HuSprGrpPosSet(s16 id, f32 x, f32 y);
extern void MgScoreWinPosSet(s16 id, f32 x, f32 y);
void fn_1_4F48(M508Outer2 *self, f32 x, f32 y)
{
    self->inner.x = x;
    self->inner.y = y;
    HuSprGrpPosSet(self->inner.id, x, y);
    MgScoreWinPosSet(self->inner.id2, x, y);
}

// Stores the texture-palette level at +0x1B0 and applies it to the model.
void fn_1_7F1C(M508OBJ *obj, f32 lvl)
{
    *(f32 *)((u8 *)obj + 0x1B0) = lvl;
    Hu3DModelTPLvlSet(obj->modelId, lvl);
}

// Clears the word at +0x158 and sets attr bit 0 on the model.
void fn_1_7F9C(M508OBJ *obj)
{
    *(s32 *)((u8 *)obj + 0x158) = 0;
    Hu3DModelAttrSet(obj->modelId, 1);
}

// Stores the motion speed at +0x1B4 and applies it to the model's motion.
void fn_1_81C0(M508OBJ *obj, f32 lvl)
{
    *(f32 *)((u8 *)obj + 0x1B4) = lvl;
    Hu3DMotionSpeedSet(obj->modelId, lvl);
}

// Sets the message colour from the top byte of the argument.
void fn_1_A010(void *obj, s32 arg1)
{
    HuWinMesColSet(*(s16 *)obj, (u8)((u32)arg1 >> 24));
}

// Disables the window, then clears its state word at +0xC.
void fn_1_9FD8(void *obj)
{
    HuWinDispOff(*(s16 *)obj);
    *(s32 *)((u8 *)obj + 0x0C) = 0;
}

void fn_1_265C(M508Pose2 *self, s32 id, f32 x, f32 y, f32 z)
{
    M508Pack *sub = (M508Pack *)((u8 *)self + 0x18);

    sub->id = id;
    sub->x = x;
    sub->y = y;
    sub->z = z;
    self->flag = 1;
}

// Reports whether the slot is occupied. The target materialises the 0/1 result
// in r31 (a callee-saved register); naming the result nudges MWCC toward the
// same register class.
s32 fn_1_8DF8(void *slot)
{
    s32 result;

    if (*(s32 *)slot != 0) {
        result = 1;
    } else {
        result = 0;
    }
    return result;
}

// Reset a task's state word to "not running".
void fn_1_1E58(void *arg0)
{
    *(s32 *)((u8 *)arg0 + 0x04) = 0;
}

// Same reset for the second task table.
void fn_1_22B4(void *arg0)
{
    *(s32 *)((u8 *)arg0 + 0x04) = 0;
}

extern s16 omSysExitReq;
void HuPrcVSleep(void);
void HuAudFadeOut(s32 speed);
s32 WipeCreate(s32 type, s32 mode, s32 frame);
s32 WipeCheckEnd(void);
void omOvlReturnEx(s32, s32);
void HuPrcEnd(void);

// Wait for the exit request, fade audio out, then run the wipe and wait for it.
void fn_1_538(void)
{
    while (omSysExitReq == 0) {
        HuPrcVSleep();
    }
    HuAudFadeOut(0x3E8);
    WipeCreate(2, 6, 0x3C);
    while ((u8)WipeCheckEnd()) {
        HuPrcVSleep();
    }
    omOvlReturnEx(1, 1);
    HuPrcEnd();
}

// Reset a third task table's state word.
void fn_1_1FD0(void *arg0)
{
    *(s32 *)((u8 *)arg0 + 0x04) = 0;
}

extern void *lbl_1_data_2C[2];
void *fn_1_7A3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, void *arg6);
void Hu3DModelAttrReset(s16 modelId, u32 attr);
extern void Hu3DMotionStartEndSet(s16 modelId, f32 start, f32 end);
extern void Hu3DMotionTimeSet(s16 modelId, f32 time);

// NONMATCHING: the target re-reads the model id into r30 for the second reset while// MWCC keeps it in r3; body, arguments and call order are otherwise exact.
// Build the entry-effector object: allocate it, arm its state, reset the model
// attributes twice (plain then per-slot), and return the object.
// Build the entry-effector object: allocate it, arm its state, reset the model
// attributes twice (plain then per-slot), and return the object.
void *fn_1_1F34(void)
{
    M508OBJ *obj = (M508OBJ *)fn_1_7A3C(0x1000, 0x2000, 0x42000C, 0, 0, 2, lbl_1_data_2C);
    void *slot = (u8 *)obj + 0x18;
    s16 modelId;

    *(s32 *)((u8 *)obj + 0x158) = 1;
    modelId = obj->modelId;
    Hu3DModelAttrReset(modelId, 1);
    Hu3DModelAttrReset(obj->modelId, 0x40000002);
    Hu3DModelAttrSet(obj->modelId, 0x40000001);
    return obj;
}

void fn_1_7A24(void)
{
    while (1) {
        HuPrcSleep(0);
    }
}

void fn_1_9EF8(void)
{
    while (1) {
        HuPrcVSleep();
    }
}

void fn_1_9F54(float *arg0, float *arg1, float *arg2)
{
    *arg1 = arg0[1];
    *arg2 = arg0[2];
}

float fn_1_A154(float *arg0, float *arg1)
{
    return arg0[0] * arg1[0] + arg0[1] * arg1[1];
}

// Arm an effector: set its state word then clear the model's hook attribute.
void fn_1_7F60(void *arg0)
{
    *(s32 *)((u8 *)arg0 + 0x158) = 1;
    Hu3DModelAttrReset(*(s16 *)((u8 *)arg0 + 0x184), 1);
}
void Hu3DModelHookSet(s16 modelId, s32 hookId, s16 hookModelId);

// Parent one effector model onto another.
void fn_1_8290(M508OBJ *arg0, s32 arg1, M508OBJ *arg2)
{
    Hu3DModelHookSet(arg0->modelId, arg1, arg2->modelId);
}

// Park an effector's motion at a given frame, promoting the frame count to float.
void fn_1_7FE0(M508OBJ *arg0, s32 arg1)
{
    Hu3DMotionTimeSet(arg0->modelId, (f32)arg1);
}

extern void *lbl_1_data_0[];

// Drive the object parked at +0x12c of the current process: call its state
// function from the lbl_1_data_0 vtable until its state word reaches 3, then
// end the process.
void fn_1_8918(void)
{
    s32 *obj = *(s32 **)((u8 *)HuPrcCurrentGet() + 0x12c);

    while (obj[0] != 3) {
        ((void (*)(s32 *))lbl_1_data_0[obj[0]])(obj);
    }
    HuPrcEnd();
}

// Drive the object parked at +0x12c of the current process: walk its callback
// table at +0x10 from counter +4 up to limit +0x14, calling each entry with
// the object itself, then copying the counter into +8 after each call.
typedef struct M508ObjCB_s {
    u8 pad00[4];
    s32 counter;
    s32 copied;
    u8 pad0C[0x10 - 0x0C];
    void (**table)(struct M508ObjCB_s *);
    s32 limit;
} M508ObjCB;

void fn_1_79BC(void)
{
    M508ObjCB *obj = *(M508ObjCB **)((u8 *)HuPrcCurrentGet() + 0x12c);

    while (obj->counter < obj->limit) {
        obj->table[obj->counter](obj);
        obj->copied = obj->counter;
    }
    HuPrcEnd();
}

// 2D AABB overlap test: returns 1 when the boxes [min_x,min_y,max_x,max_y]
// touch or overlap, 0 otherwise. Comparisons are inclusive (>= / <=).
typedef struct M508Box_s {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
} M508Box;

s32 fn_1_61C0(M508Box *a, M508Box *b)
{
    if (a->min_x > b->max_x) {
        return 0;
    }
    if (a->max_x < b->min_x) {
        return 0;
    }
    if (a->min_y > b->max_y) {
        return 0;
    }
    if (a->max_y < b->min_y) {
        return 0;
    }
    return 1;
}

// Sets sprite attributes 0..6 to value 4 on the sprite parked at +0x18, then
// hides the score window parked at +0x34.
// NONMATCHING: 59.26%. MWCC allocates the sprite base to r29 and the counter
// to r31 (saving r29 in the frame), where the target uses r31 for the base and
// r30 for the counter. The register choice is not controllable from C.
extern void HuSprAttrSet(s16 spriteId, s32 attr, s32 value);
extern void MgScoreWinDispSet(s16 winId, s32 disp);
void fn_1_5010(void *obj)
{
    s16 *sprite = (s16 *)((u8 *)obj + 0x18);
    s32 i;

    for (i = 0; i < 7; i++) {
        HuSprAttrSet(*sprite, i, 4);
    }
    MgScoreWinDispSet(*(s16 *)((u8 *)obj + 0x34), 0);
}

// Resets sprite attributes 0..3 on the sprite parked at +0x18, then shows
// the score window parked at +0x34.
// NONMATCHING: 53.85%. Same register allocation as fn_1_5010: MWCC puts the
// sprite base in r29 and the counter in r31, the target uses r31/r30.
extern void HuSprAttrReset(s16 spriteId, s32 attr);
void fn_1_4FA8(void *obj)
{
    s16 *sprite = (s16 *)((u8 *)obj + 0x18);
    s32 i;

    for (i = 0; i < 4; i++) {
        HuSprAttrReset(*sprite, i);
    }
    MgScoreWinDispSet(*(s16 *)((u8 *)obj + 0x34), 1);
}

// If type at +0x00 is not 2, passes camId at +0x08 and eight floats at +0x10
// to Hu3DCameraPosSet. A ninth float at +0x20 is loaded but unused by the
// target (compiler artifact).
extern void Hu3DCameraPosSet(s16 camId, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h);
void fn_1_8BF4(void *obj)
{
    if (*(s32 *)obj != 2) {
        float *cam = (float *)((u8 *)obj + 0x10);
        Hu3DCameraPosSet(*(s16 *)((u8 *)obj + 8), cam[0], cam[1], cam[2], cam[3],
                         cam[4], cam[5], cam[6], cam[7]);
    }
}

// Reads modelId at +0x184, always resets attr 0x4002, then sets or resets
// attr 0x4001 depending on arg1.
// NONMATCHING: 50.00%. The target materialises 0x4002 and 0x4001 as
// lis+addi pairs; MWCC uses single li instructions (the li/lis rule in
// docs/HANDOVER.md). MWCC also keeps obj in r3 instead of spilling it to r30,
// so frame offsets differ.
void fn_1_803C(void *obj, s32 arg1)
{
    s16 modelId = *(s16 *)((u8 *)obj + 0x184);

    Hu3DModelAttrReset(modelId, 0x4002);
    if (arg1 != 0) {
        Hu3DModelAttrSet(modelId, 0x4001);
    } else {
        Hu3DModelAttrReset(modelId, 0x4001);
    }
}

// Stores arg1 at +0x188, then either calls Hu3DMotionSet(modelId, -1) when
// arg1 == -1, or reads a s16 from the table at +0x190 indexed by arg1*2 and
// calls Hu3DMotionShiftSet(modelId, val, arg2, arg3, arg4).
// NONMATCHING: 33.33%. Target keeps obj in r31 (callee-saved) and reloads
// modelId from it in each branch; MWCC keeps obj in r28/arg1 in r29 and keeps
// modelId in a local. Register allocation not controllable from C.
extern void Hu3DMotionSet(s16 modelId, s32 arg);
extern void Hu3DMotionShiftSet(s16 modelId, s16 val, f32 a, f32 b, s32 c);
void fn_1_813C(void *obj, s32 arg1, f32 arg2, f32 arg3, s32 arg4)
{
    s16 modelId = *(s16 *)((u8 *)obj + 0x184);

    *(s32 *)((u8 *)obj + 0x188) = arg1;
    if (arg1 == -1) {
        Hu3DMotionSet(modelId, -1);
    } else {
        s16 *table = (s16 *)*(s32 *)((u8 *)obj + 0x190);
        Hu3DMotionShiftSet(modelId, table[arg1], arg2, arg3, arg4);
    }
}

// Allocates a new object via fn_1_7A3C with entry lbl_1_rodata_754[obj],
// stores obj at +0x18 of the new object, and returns it.
// NONMATCHING: 45.16%. Target computes the table index in r6 and loads the
// entry into r5; MWCC uses r4/r3 and moves the loaded value to r5 before the
// call. Register allocation not controllable from C.
extern s32 lbl_1_rodata_754[];
extern void *lbl_1_data_A4[];
void *fn_1_5758(void *obj)
{
    s32 entry = lbl_1_rodata_754[(s32)obj];
    void *new_obj = fn_1_7A3C(0x1000, 0x2000, entry, 0, 0, 2, lbl_1_data_A4);

    *(void **)((u8 *)new_obj + 0x18) = obj;
    return new_obj;
}

// Allocates a new object via fn_1_7A3C with entry lbl_1_rodata_738[obj],
// zeroes +0x158, sets model attr 1 on the model at +0x184, and returns it.
// NONMATCHING: 78.12%. The target materialises "new_obj + 0x18" into the
// frame at +0xc(r1) even though the value is never read afterwards; MWCC
// eliminates the dead store entirely. Register/liveness choice not controllable
// from C.
extern s32 lbl_1_rodata_738[];
void *fn_1_5394(void *obj)
{
    void *new_obj = fn_1_7A3C(0x1000, 0x2000, lbl_1_rodata_738[(s32)obj], 0, 0, 1, 0);

    *(s32 *)((u8 *)new_obj + 0x158) = 0;
    Hu3DModelAttrSet(*(s16 *)((u8 *)new_obj + 0x184), 1);
    return new_obj;
}

// Creates a type-2 message, then either waits for it to finish or sleeps
// arg1 frames, then ends the process.
// NONMATCHING: 88.57%. MWCC parks arg1 in r29 across the GameMesCreate call,
// the target parks self in r29. Register allocation not controllable from C.
extern s32 GameMesCreate(s32 type, s32 arg);
extern s32 GameMesStatGet(s32 msg);
void fn_1_9BF4(void *self, s32 arg1)
{
    s32 msg = GameMesCreate(2, 0);

    if (arg1 == -1) {
        while ((u8)GameMesStatGet(msg) != 0) {
            HuPrcVSleep();
        }
    } else {
        s32 i;

        for (i = 0; i < arg1; i++) {
            HuPrcVSleep();
        }
    }
    HuPrcEnd();
}

// Same as fn_1_9BF4 but creates the message with argument 1.
// NONMATCHING: 88.57%. MWCC parks arg1 in r29 across the GameMesCreate call,
// the target parks self in r29. Register allocation not controllable from C.
void fn_1_9C80(void *self, s32 arg1)
{
    s32 msg = GameMesCreate(2, 1);

    if (arg1 == -1) {
        while ((u8)GameMesStatGet(msg) != 0) {
            HuPrcVSleep();
        }
    } else {
        s32 i;

        for (i = 0; i < arg1; i++) {
            HuPrcVSleep();
        }
    }
    HuPrcEnd();
}

// 3D cone test: returns 1 when (dx*dx + dy*dy) < (a.z + b.z)^2, 0 otherwise.
// Note: bge -> return 0, so the strict inequality is on the dyadic side.
typedef struct M508Vec3_s {
    f32 x;
    f32 y;
    f32 z;
} M508Vec3;
s32 fn_1_6130(M508Vec3 *a, M508Vec3 *b)
{
    f32 dx = a->x - b->x;
    f32 dy = a->y - b->y;
    f32 dz = a->z + b->z;

    return (dx * dx + dy * dy) < (dz * dz) ? 1 : 0;
}

// Sets the camera scissor rectangle from four floats parked in the frame;
// camId is the s16 at +0x8 of obj.
// NONMATCHING: 45.95%. MWCC stores the four floats at different frame offsets
// and reloads camId from the frame instead of keeping obj in a register;
// register/frame allocation not controllable from C.
extern void Hu3DCameraScissorSet(s16 camId, u32 left, u32 top, u32 right, u32 bottom);
void fn_1_9628(void *obj, f32 x, f32 y, f32 z, f32 w)
{
    Hu3DCameraScissorSet(*(s16 *)((u8 *)obj + 0x8), (u32)x, (u32)y, (u32)z, (u32)w);
}

// Kills all motions in the table at +0x190 (count at +0x18c), kills the model
// at +0x184 if not -1, frees the pointer at +0x10, returns the first word.
// NONMATCHING: 10.00%. MWCC allocates obj/count/table across r28-r31 and the
// frame very differently from the target; the loop and kill calls are
// otherwise identical.
extern void Hu3DMotionKill(s16 motId);
extern void Hu3DModelKill(s16 modelId);
void *fn_1_7CC0(void *obj)
{
    s16 *table = (s16 *)*(s32 *)((u8 *)obj + 0x190);
    s32 count = *(s32 *)((u8 *)obj + 0x18c);
    s32 i;

    for (i = 0; i < count; i++) {
        if (table[i] != -1) {
            Hu3DMotionKill(table[i]);
        }
    }
    if (*(s16 *)((u8 *)obj + 0x184) != -1) {
        Hu3DModelKill(*(s16 *)((u8 *)obj + 0x184));
    }
    HuMemDirectFree(*(void **)((u8 *)obj + 0x10));
    return *(void **)obj;
}

// Same as fn_1_7CC0 but also frees the motion table at +0x190 itself.
void *fn_1_85F8(void *obj)
{
    s16 *table = (s16 *)*(s32 *)((u8 *)obj + 0x190);
    s32 count = *(s32 *)((u8 *)obj + 0x18c);
    s32 i;

    for (i = 0; i < count; i++) {
        if (table[i] != -1) {
            Hu3DMotionKill(table[i]);
        }
    }
    HuMemDirectFree(table);
    if (*(s16 *)((u8 *)obj + 0x184) != -1) {
        Hu3DModelKill(*(s16 *)((u8 *)obj + 0x184));
    }
    HuMemDirectFree(*(void **)((u8 *)obj + 0x10));
    return *(void **)obj;
}

// Resets attr 0x4002 and 0x4001 on the model at +0x184, sets motion start/end
// from rodata_3F0 indexed by arg1*2, then sets motion time from rodata_2C[0].
extern f32 lbl_1_rodata_3F0[];
extern f32 lbl_1_rodata_2C[];
// NONMATCHING: 9.76%. Target keeps obj in r30 and modelId in r31; MWCC keeps
// obj in r4/r3 and reloads modelId. Register allocation not controllable from C.
void fn_1_21C8(void *obj, s32 arg1)
{
    s16 modelId = *(s16 *)((u8 *)obj + 0x184);

    Hu3DModelAttrReset(modelId, 0x4002);
    Hu3DModelAttrReset(modelId, 0x4001);
    Hu3DMotionStartEndSet(modelId, lbl_1_rodata_3F0[arg1 * 2], lbl_1_rodata_3F0[arg1 * 2 + 1]);
    Hu3DMotionTimeSet(modelId, lbl_1_rodata_2C[0]);
}

// Positions the sprite at +0x18 (s16 id), stores (x,y,z) at +0x160..+0x168,
// then repositions the model at +0x184.
// NONMATCHING: 68.57%. The target materialises "obj + 0x18" into a separate
// register r30 before loading the s16 id at 0x0(r30); MWCC uses a direct
// "lha r3, 0x18(r31)" instead. Register allocation not controllable from C.
void fn_1_38CC(void *obj, f32 x, f32 y, f32 z)
{
    Hu3DModelPosSet(*(s16 *)((u8 *)obj + 0x18), x, y, z);
    *(f32 *)((u8 *)obj + 0x160) = x;
    *(f32 *)((u8 *)obj + 0x164) = y;
    *(f32 *)((u8 *)obj + 0x168) = z;
    Hu3DModelPosSet(*(s16 *)((u8 *)obj + 0x184), x, y, z);
}

// 2D cone test: returns 1 when (dx*dx + dy*dy) <= dz*dz, where dz is b->z.
// NONMATCHING: 88.57%. MWCC lowers the fcmpo+ble into cror+bne with inverted
// branch polarity; MWCC's codegen for this float compare pattern is not
// controllable from C.

s32 fn_1_60A8(M508Vec3 *a, M508Vec3 *b)
{
    f32 dx = a->x - b->x;
    f32 dy = a->y - b->y;
    f32 dz = b->z;

    return (dx * dx + dy * dy) <= (dz * dz) ? 1 : 0;
}

// Allocates an object, spawns a child process running fn_1_9EF8, cross-links
// them at +0x10/+0x12c, creates a window frame at +0x0, and returns the object.
// The target also emits dead calls to HuPrcCurrentGet and lbl_1_rodata_2C[0]
// whose results are never used; MWCC eliminates them, so this cannot match.
//
// NONMATCHING: expected ~75-85%. Target keeps dead HuPrcCurrentGet() and two
// lbl_1_rodata_2C[0] loads whose results are never read; MWCC drops dead code.
// That register/frame difference is not controllable from C.
extern void *HuMemDirectMallocNum(s32 heap, s32 size, s32 align);
extern s16 HuWinExCreateFrame(s16 x, s16 y, s16 w, s16 h);
void *fn_1_9E10(s16 arg1, s16 arg2)
{
    extern s32 HuPrcChildCreate(void *, s32, s32, s32);
    void *obj;
    void *fnp;
    s32 proc;
    f32 dummy1;
    f32 dummy2;

    obj = HuMemDirectMallocNum(0, 0x14, 0x1000);
    fnp = (void *)fn_1_9EF8;
    proc = HuPrcChildCreate(fnp, 0x1000, 0x2000, 0);

    (void)HuPrcCurrentGet();

    // dead-code placeholders
    dummy1 = lbl_1_rodata_2C[0];
    dummy2 = lbl_1_rodata_2C[0];
    (void)dummy1;
    (void)dummy2;

    *(void **)((u8 *)obj + 0x10) = (void *)proc;
    *(void **)((u8 *)proc + 0x12c) = obj;
    *(s16 *)obj = HuWinExCreateFrame(arg1, arg2, -1, 1);
    return obj;
}