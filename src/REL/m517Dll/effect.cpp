// m517Dll effect unit

typedef signed int s32;
typedef signed short s16;
typedef unsigned int u32;
typedef float f32;

#pragma dont_inline on
extern "C" {

// Clears bit at index 'bit' in the u32 at offset 0xC of 'obj'
void fn_1_39C(void *obj, s32 bit)
{
    *(u32 *)((char *)obj + 0xC) &= ~(1u << bit);
}

// Copies 3 floats from src (offsets 0,4,8) to dst (offsets 0x28,0x2C,0x30)
void fn_1_9D0(void *dst, const void *src)
{
    *(f32 *)((char *)dst + 0x28) = *(f32 *)((char *)src + 0x0);
    *(f32 *)((char *)dst + 0x2C) = *(f32 *)((char *)src + 0x4);
    *(f32 *)((char *)dst + 0x30) = *(f32 *)((char *)src + 0x8);
}

// Copies 4 floats from self (offsets 8,C,10,14) to dst (offsets 0,4,8,C)
void fn_1_1360(void *self, void *dst)
{
    *(f32 *)((char *)dst + 0x0) = *(f32 *)((char *)self + 0x8);
    *(f32 *)((char *)dst + 0x4) = *(f32 *)((char *)self + 0xC);
    *(f32 *)((char *)dst + 0x8) = *(f32 *)((char *)self + 0x10);
    *(f32 *)((char *)dst + 0xC) = *(f32 *)((char *)self + 0x14);
}

// Copies 3 floats from src (offsets 0,4,8) to dst (offsets 0xA0,0xA4,0xA8)
void fn_1_1384(void *dst, const void *src)
{
    *(f32 *)((char *)dst + 0xA0) = *(f32 *)((char *)src + 0x0);
    *(f32 *)((char *)dst + 0xA4) = *(f32 *)((char *)src + 0x4);
    *(f32 *)((char *)dst + 0xA8) = *(f32 *)((char *)src + 0x8);
}

void fn_1_1E80()
{
}

} // extern "C"
#pragma dont_inline reset
