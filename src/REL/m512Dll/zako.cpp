// m512Dll/REL/m512Dll/zako

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

// -------------------------------------------------------------------- extern

extern "C" {

void *__dl__FPv(void *);

void fn_1_1270(void *self, s32 arg);
void fn_1_12DC(void *self);
void fn_1_1330(void *self, void *arg);
void fn_1_10610(void *self, void *arg, void (*callback)(void *));
void fn_1_F070(void);
void fn_1_F534(void);

void fn_1_EAD4(void);
void fn_1_EAF4(void *self, s32 bit);
void fn_1_EF44(void *self, s16 flag);
void fn_1_F4F0(void *self);
void fn_1_105B0(void *self, void *arg);
void fn_1_106F8(void *self, s16 flag);

}

// ------------------------------------------------------------------ forward

extern "C" void fn_1_EAD4(void);
extern "C" void fn_1_EAF4(void *self, s32 bit);
extern "C" void fn_1_EF44(void *self, s16 flag);
extern "C" void fn_1_F4F0(void *self);
extern "C" void fn_1_105B0(void *self, void *arg);
extern "C" void fn_1_106F8(void *self, s16 flag);

// ---------------------------------------------------------------- data

void *lbl_1_data_550[] = {
    0, (void *)0xFFFFFFFF, (void *)fn_1_F534,
};

s32 lbl_1_bss_AA0;

// ---------------------------------------------------------------- functions

// Simple wrapper that calls fn_1_F070
extern "C" void fn_1_EAD4(void)
{
    fn_1_F070();
}

// NONMATCHING: branch type differs (b vs blt) after dead compare
// Clears a bit in self->unk0C if bit is not in range [0, 5]
extern "C" void fn_1_EAF4(void *self, s32 bit)
{
    *(u32 *)((char *)self + 0xC) &= ~(1u << bit);
    if (bit >= 5) {
        return;
    }
    if (bit < 0) {
        return;
    }
}

// Calls destructor-like function and deletes if flag > 0
extern "C" void fn_1_EF44(void *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    __asm { mr r3, r31 }
}

// NONMATCHING: compiler optimizes away intermediate stack stores
// Copies 3 words from lbl_1_data_550 to self+0xC
extern "C" void fn_1_F4F0(void *self)
{
    u32 *src = (u32 *)lbl_1_data_550;
    u32 *dst = (u32 *)((char *)self + 0xC);
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

// NONMATCHING: prologue stack layout differs (self spilled to 0x8 vs kept in r31)
// Calls fn_1_10610 on first node of list
extern "C" void fn_1_105B0(void *self, void *arg)
{
    void *head = self;
    void *head2 = head;
    void *first = *(void **)head;
    void *current = first;
    fn_1_10610(current, head2, (void (*)(void *))arg);
}

// Same as fn_1_EF44 - calls destructor-like function and deletes if flag > 0
extern "C" void fn_1_106F8(void *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    __asm { mr r3, r31 }
}