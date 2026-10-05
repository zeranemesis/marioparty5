#include "REL/m513Dll.h"

typedef struct M513PowerEff_s {
    /* 0x00 */ M513_OBJHEAD head;
    /* 0x08 */ void **vtable;
    /* 0x0C */ u32 flags;
    /* 0x10 */ u8 pad10[0x8];
    /* 0x18 */ HuVecF vec18;
    /* 0x24 */ HuVecF vec24;
    /* 0x30 */ u8 pad30[0xF0];
    /* 0x120 */ void *ptr120;
    /* 0x124 */ u8 pad124[0x4];
} M513PowerEff;

extern "C" {

void fn_1_8E78(void *, void *, void *);
void fn_1_929C(void *, int);
void fn_1_94E4(void *, void *);
void fn_1_9824(void *, void *);
void fn_1_9840(void *, void *);
void fn_1_985C(void *, void *);
void fn_1_A064(void *, int);
void fn_1_A528(void *, void *);

} // extern "C"

/* Clear bit in flags at offset 0xC */
extern "C" void fn_1_929C(void *obj, int bit)
{
    u32 *flags = (u32 *)((u8 *)obj + 0xC);
    *flags &= ~(1 << bit);
}

/* Clear bit in flags at offset 0xC */
extern "C" void fn_1_A064(void *obj, int bit)
{
    u32 *flags = (u32 *)((u8 *)obj + 0xC);
    *flags &= ~(1 << bit);
}

/* Copy 3 floats from src to obj+0x18 */
extern "C" void fn_1_9824(void *obj, void *src)
{
    HuVecF *dst = (HuVecF *)((u8 *)obj + 0x18);
    HuVecF *s = (HuVecF *)src;
    *dst = *s;
}

/* Copy 3 floats from src to obj+0x24 */
extern "C" void fn_1_9840(void *obj, void *src)
{
    HuVecF *dst = (HuVecF *)((u8 *)obj + 0x24);
    HuVecF *s = (HuVecF *)src;
    *dst = *s;
}

/* Wrapper: call fn_1_94E4 with *(obj2+0x1C) and arg3 */
extern "C" void fn_1_8E78(void *obj1, void *obj2, void *arg3)
{
    void *ptr = *(void **)((u8 *)obj2 + 0x1C);
    fn_1_94E4(ptr, arg3);
}

/* Wrapper: call fn_1_A528 with *(obj1+0x120) and arg2 */
extern "C" void fn_1_985C(void *obj1, void *arg2)
{
    void *ptr = *(void **)((u8 *)obj1 + 0x120);
    fn_1_A528(ptr, arg2);
}