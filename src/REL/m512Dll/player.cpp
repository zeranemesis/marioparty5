// m512Dll/REL/m512Dll/player

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef double f64;

extern "C" {

void CharFXPlay(s16 id, s16 unk);

void *lbl_1_bss_A4C;

void fn_1_1270(void *self, int flag);
void __dl__FPv(void *obj);

void fn_1_A1F0(void)
{
    if (lbl_1_bss_A4C != 0) {
        CharFXPlay((s16)*(u32 *)((u8 *)lbl_1_bss_A4C + 0x1C), 0x1FA);
    }
}

f32 fn_1_A4BC(f32 *dst)
{
    dst[0] = *(f32 *)((u8 *)lbl_1_bss_A4C + 0x40);
    dst[1] = *(f32 *)((u8 *)lbl_1_bss_A4C + 0x44);
    dst[2] = *(f32 *)((u8 *)lbl_1_bss_A4C + 0x48);
    return *(f32 *)((u8 *)lbl_1_bss_A4C + 0x2C);
}

void *fn_1_99BC(void *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void *fn_1_D120(void *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void fn_1_CB60(void *node, void *end, void (*func)(void *))
{
    void *next;
    while (node != end) {
        func(node);
        next = *(void **)node;
        node = next;
    }
}

// NONMATCHING: register allocation differs (r31/r30/r29/r28 vs r28/r31/r30/r29)
void fn_1_CB00(void *self, s32 arg)
{
    void *self_copy = self;
    void *vtable = *(void **)self;
    void *vtable_copy = vtable;
    fn_1_CB60(vtable_copy, self_copy, (void (*)(void *))arg);
}

}