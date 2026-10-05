// m517Dll choro unit: Choro minigame objects.

typedef signed int s32; typedef struct { void *ptr; s32 adj; s32 fn; } PTMF;
typedef unsigned int u32;
typedef unsigned char u8;
typedef signed short s16;

typedef float f32;

extern "C" {
void fn_1_100E8(void *self, void *arg);
void Hu3DModelAttrReset(s16 modelId, u32 attr);
void Hu3DModelPosSet(s16 modelId, f32 x, f32 y, f32 z);
void Hu3DParticleTPLvlSet(s16 modelId, f32 level);


void *fn_1_2DB4(void *self, s32 flag); s32 __ptmf_test(const PTMF *ptmf); void __ptmf_scall(void *self, const PTMF *ptmf);
void *fn_1_1F4C(void *self);

// Clears bit `bitIndex` in the flags word at +0xC of `self`.
#pragma dont_inline on
void fn_1_FDAC(void *self, s32 bitIndex)
{
    *(u32 *)((u8 *)self + 0xC) &= ~(1u << bitIndex);
}
#pragma dont_inline reset

// Calls fn_1_100E8 with the pointer at obj+0x1C and arg2.
// NONMATCHING: register allocation differs (original uses r30 for obj, this spills to stack)
#pragma dont_inline on
void fn_1_FAB0(void *unused, void *obj, void *arg2)
{
    struct ChoroObj { char pad[0x1C]; void *ptr; };
    fn_1_100E8(((struct ChoroObj *)obj)->ptr, arg2);
}
#pragma dont_inline reset

// Resets Hu3D model attributes for the model ID at self+0x10.
// NONMATCHING: register allocation differs (original uses r30/r31, this uses r3)
#pragma dont_inline on
void fn_1_1115C(void *self)
{
    struct ChoroObj { char pad[0x10]; s16 modelId; };
    Hu3DModelAttrReset(((struct ChoroObj *)self)->modelId, 0x4002);
}
#pragma dont_inline reset

// Calls virtual function at self+0x10 if non-null.
// NONMATCHING: register allocation differs (original uses r31, this uses r30); __ptmf_scall calling convention (r12) not matched
#pragma dont_inline on
void fn_1_1293C(void *self)
{
    PTMF *ptmf = (PTMF *)((u8 *)self + 0x10);
    if (__ptmf_test(ptmf)) {
        __ptmf_scall(self, ptmf);
    }
}
#pragma dont_inline reset

// Calls fn_1_2DB4(self, 0), then fn_1_1F4C(self) if arg1 > 0.
#pragma dont_inline on
void *fn_1_10F30(void *self, s16 arg1)
{
    if (self != 0) {
        fn_1_2DB4(self, 0);
        if (arg1 > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}
#pragma dont_inline reset

// Sets model position, particle level, clears byte at +0x1C, resets attr.
#pragma dont_inline on
void fn_1_FD3C(void *self, const f32 *pos)
{
    s16 modelId = *(s16 *)((u8 *)self + 0x10);
    Hu3DModelPosSet(modelId, pos[0], pos[1], pos[2]);
    Hu3DParticleTPLvlSet(modelId, 0.0f);
    *(u8 *)((u8 *)self + 0x1C) = 0;
    Hu3DModelAttrReset(modelId, 1);
}
#pragma dont_inline reset

} // extern "C"
