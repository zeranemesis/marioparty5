// m517Dll papereff unit

typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

#pragma dont_inline on
extern "C" {

// Clears bit at index 'bit' in the u32 at offset 0xC of 'obj'
void fn_1_13838(void *obj, s32 bit)
{
    *(u32 *)((char *)obj + 0xC) &= ~(1u << bit);
}

// Clears bit at index 'bit' in the u32 at offset 0xC of 'obj' (duplicate of fn_1_13838)
void fn_1_14904(void *obj, s32 bit)
{
    *(u32 *)((char *)obj + 0xC) &= ~(1u << bit);
}

// Copies 3 floats from src (offsets 0,4,8) to dst (offsets 0x1C,0x20,0x24)
void fn_1_13F44(void *dst, const void *src)
{
    *(f32 *)((char *)dst + 0x1C) = *(f32 *)((char *)src + 0x0);
    *(f32 *)((char *)dst + 0x20) = *(f32 *)((char *)src + 0x4);
    *(f32 *)((char *)dst + 0x24) = *(f32 *)((char *)src + 0x8);
}

// Copies 3 floats from src (offsets 0,4,8) to dst (offsets 0x28,0x2C,0x30)
void fn_1_13F60(void *dst, const void *src)
{
    *(f32 *)((char *)dst + 0x28) = *(f32 *)((char *)src + 0x0);
    *(f32 *)((char *)dst + 0x2C) = *(f32 *)((char *)src + 0x4);
    *(f32 *)((char *)dst + 0x30) = *(f32 *)((char *)src + 0x8);
}

extern "C" void fn_1_13A70(void *, s32);

void fn_1_13590(void *arg1, void *arg2, s32 arg3)
{
    register void *saved_arg2 = arg2;
    void *val = *(void **)((char *)saved_arg2 + 0x1C);
    fn_1_13A70(val, arg3);
}

} // extern "C"
#pragma dont_inline reset