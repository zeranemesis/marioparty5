// m517Dll input unit: the per-minigame "input" sub-objects.
//
// Three vtables live in .data, each {RTTI, RTTI, v0, v1}; MWCC emits two
// leading null words before the virtual entries:
//   lbl_1_data_2F8 -> fn_1_A090 / fn_1_99EC
//   lbl_1_data_308 -> fn_1_8134 / fn_1_81A0
//   lbl_1_data_328 -> fn_1_7C44 / 0        (the common base)
// The two derived destructors store their own vtable, then the inlined base
// destructor body stores lbl_1_data_328 back.

typedef signed int s32;
typedef signed short s16;
typedef float f32;

struct InputVtbl {
    void *m_unk0;
    void *m_unk4;
    void *m_dtor;
    void *m_fn3;
};

struct InputObj {
    InputVtbl *m_vtable; // 0x00
    f32 m_f4;           // 0x04
    f32 m_f8;           // 0x08
    f32 m_fC;           // 0x0C
    s32 m_f10;          // 0x10
};

// Declared only so that the call in fn_1_7B74 uses MWCC's virtual-call
// sequence (lwz r12,0(r3) / lwz r12,0xc(r12) / mtctr / bctrl).  No
// constructor is emitted here, so no __vt_ symbol is generated.
class InputVObj {
public:
    virtual void m_v0(s16 arg);
    virtual void m_v1(f32 *out);
    f32 m_f4;
    f32 m_f8;
    f32 m_fC;
    s32 m_f10;
};

extern "C" {
void fn_1_7B2C(InputObj *self);
void fn_1_7B74(InputVObj *self);
void *fn_1_7C44(void *self, s16 arg);
void *fn_1_8134(void *self, s16 arg);
void *fn_1_A090(void *self, s16 arg);

void fn_1_1F4C(void *self);
void fn_1_81A0(void *self, s16 arg);
void fn_1_99EC(void *self, s16 arg);
}

InputVtbl lbl_1_data_2F8 = {0, 0, fn_1_A090, fn_1_99EC};
InputVtbl lbl_1_data_308 = {0, 0, fn_1_8134, fn_1_81A0};
InputVtbl lbl_1_data_328 = {0, 0, fn_1_7C44, 0};

// Reading through a volatile lvalue is what keeps the named .rodata
// relocation; a plain read of this const is folded into an anonymous @N.
extern const f32 lbl_1_rodata_108 = 0.0f;

extern "C" void fn_1_7B2C(InputObj *self)
{
    self->m_vtable = &lbl_1_data_328;
    self->m_f4 = *(volatile f32 *)&lbl_1_rodata_108;
    self->m_f8 = *(volatile f32 *)&lbl_1_rodata_108;
    self->m_fC = *(volatile f32 *)&lbl_1_rodata_108;
    self->m_f10 = 0;
}

extern "C" void fn_1_7B74(InputVObj *self)
{
    self->m_f4 = *(volatile f32 *)&lbl_1_rodata_108;
    self->m_f8 = *(volatile f32 *)&lbl_1_rodata_108;
    self->m_v1(&self->m_f4);
}

extern "C" void *fn_1_7C44(void *self, s16 arg)
{
    if (self != 0) {
        ((InputObj *)self)->m_vtable = &lbl_1_data_328;
        if (arg > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

extern "C" void *fn_1_8134(void *self, s16 arg)
{
    if (self != 0) {
        ((InputObj *)self)->m_vtable = &lbl_1_data_308;
        if (self != 0) {
            ((InputObj *)self)->m_vtable = &lbl_1_data_328;
        }
        if (arg > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

extern "C" void *fn_1_A090(void *self, s16 arg)
{
    if (self != 0) {
        ((InputObj *)self)->m_vtable = &lbl_1_data_2F8;
        if (self != 0) {
            ((InputObj *)self)->m_vtable = &lbl_1_data_328;
        }
        if (arg > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}
