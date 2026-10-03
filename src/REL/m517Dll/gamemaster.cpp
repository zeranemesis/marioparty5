// m517Dll gamemaster unit: CGameMaster and its singleton sub-objects.
//
// Every definition keeps C linkage: the target symbols (fn_1_A168 ...) are
// unmangled, so a C++-mangled definition would never pair up in objdiff.
//
// lbl_1_data_3C0 is one 12-word object holding both vtables; the constructor
// stores &vtable and &vtable[6] into the base slots at +0x04 and +0x20.
// lbl_1_data_338/344/350/374/380/38C are the six 12-byte singleton records
// {ptr, slot, initFn}.  lbl_1_bss_A1C/A20 are the smart-pointer slots that
// the registering helpers assert must still be NULL.

typedef signed int s32;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

struct CGameMaster {
    void *m_f0;        // 0x00 core object
    void *m_vtbl;      // 0x04
    char m_f8[0x18];   // 0x08
    void *m_vtbl2;     // 0x20
    char m_f24[0x334]; // 0x24
    void *m_f358;      // 0x358
    void *m_f35C;      // 0x35C
    s32 m_f360;        // 0x360
    s32 m_f364;        // 0x364
    unsigned char m_f368;
    unsigned char m_f369;
    char m_f36A[6];
};

// {ptr, slot, initFn}: the record each registered singleton keeps.
struct CSingleton {
    void *m_ptr;
    s32 m_slot;
    void (*m_init)(void);
};

// Declared here rather than through the game headers: those pull in MSL
// math.h, whose inline std::sqrtf emits constants into .rodata.
extern "C" {
void *fn_1_1E84(u32 size);
s32 fn_1_2058(const char *msg, const char *tag, s32 id);
void *fn_1_7558(void *p, s32 mode);
void fn_1_2DA8(void *p);
void fn_1_39F8(void *self);
void fn_1_6D20(void *self, s32 v);
void fn_1_1F4C(void *self);
void fn_1_2DB4(void *self, s32 flag);
void fn_1_1415C(void *obj, s32 mode);
void fn_1_15724(void *obj, s32 mode);
void fn_1_7620(void *obj, s32 mode);
void fn_1_B8A8(void *self);
void fn_1_BA58(void);
void fn_1_BAFC(void *self);
void fn_1_3920(void *self);
void fn_1_111A8(void);
void fn_1_17614(void);
void fn_1_16B68(void);
void fn_1_16918(void);
void fn_1_13D4(void);
void fn_1_11850(void);
void fn_1_A488(void *self, s32 mode);
void fn_1_AB90(void *self);
void fn_1_AC64(void *self);
void fn_1_ADA8(void *self);
void fn_1_B058(void *self);

// smart-pointer slots owned by other units of this REL
extern void *lbl_1_bss_0;
extern void *lbl_1_bss_A08;

extern "C" void fn_1_A168(CGameMaster *self);
extern "C" void fn_1_A748(void *self);
extern "C" void fn_1_A860(void *self);
extern "C" void fn_1_A978(void *self);
extern "C" void fn_1_B2CC(void *self);
extern "C" void fn_1_B310(void *self);
extern "C" void fn_1_B414(void *self);
extern "C" void fn_1_B4C4(void *self);
extern "C" void fn_1_B5C0(void *self);
extern "C" void fn_1_B660(void *self);
extern "C" void fn_1_B730(void *self);
extern "C" void fn_1_B8A8(void *self);
extern "C" void fn_1_BAFC(void *self);
extern "C" void fn_1_BBA0(void *self);
extern "C" void fn_1_BC3C(void *self);
extern "C" void fn_1_BCD8(void *self);
extern "C" void fn_1_BDA0(void *self);
extern "C" void fn_1_BDA8(void *self);
}

CSingleton lbl_1_data_338 = {0, -1, fn_1_111A8};
CSingleton lbl_1_data_344 = {0, -1, fn_1_17614};
CSingleton lbl_1_data_350 = {0, -1, fn_1_16B68};
char lbl_1_data_35C[] = "sm_ptr == NULL";
char lbl_1_data_36B[] = "es_a";
char lbl_1_data_370[] = "h.t";
CSingleton lbl_1_data_374 = {0, -1, fn_1_16918};
CSingleton lbl_1_data_380 = {0, -1, fn_1_13D4};
CSingleton lbl_1_data_38C = {0, -1, fn_1_11850};
char lbl_1_data_398[] = "sm_ptr != 0";
char lbl_1_data_3A4[] = "ms_pointer != 0";
char lbl_1_data_3B4[] = "singleton.h";
void *lbl_1_data_3C0[] = {
    0, 0, fn_1_AB90, fn_1_AC64, fn_1_ADA8, fn_1_B2CC,
    0, 0, fn_1_BDA8, fn_1_BDA0, fn_1_B058, 0
};

s32 lbl_1_bss_A10;
s32 lbl_1_bss_A14;
s32 lbl_1_bss_A18;
void *lbl_1_bss_A1C;
void *lbl_1_bss_A20;

extern "C" void fn_1_A168(CGameMaster *self)
{
    fn_1_39F8(self);
    fn_1_6D20(&self->m_vtbl2, 0x8000);
    self->m_vtbl = lbl_1_data_3C0;
    self->m_vtbl2 = &lbl_1_data_3C0[6];

    void *p = fn_1_1E84(0x18);
    if (p != 0) {
        void *base = p;
        void *a0 = base;
        void *t0 = a0;

        fn_1_2DA8(t0);
        if (lbl_1_bss_A08 != 0) {
            fn_1_2058(lbl_1_data_35C, lbl_1_data_36B, 0x15);
        } else {
            lbl_1_bss_A08 = a0;
        }

        void *a1 = (char *)base + 8;
        void *t1 = a1;

        fn_1_2DA8(t1);
        if (lbl_1_bss_0 != 0) {
            fn_1_2058(lbl_1_data_35C, lbl_1_data_36B, 0x15);
        } else {
            lbl_1_bss_0 = a1;
        }

        void *a2 = (char *)base + 0x10;
        void *t2 = a2;

        fn_1_2DA8(t2);
        if (lbl_1_bss_A1C != 0) {
            fn_1_2058(lbl_1_data_35C, lbl_1_data_36B, 0x15);
        } else {
            lbl_1_bss_A1C = a2;
        }

        p = base;
    }
    void *made = p;
    self->m_f358 = made;

    void *q = fn_1_1E84(0x4);
    if (q != 0) {
        q = (void *)fn_1_7558(q, 0);
    }
    void *made2 = q;
    self->m_f35C = made2;
    self->m_f360 = 0;
    self->m_f364 = 0;
    self->m_f368 = 0;
    self->m_f369 = 0;
}

extern "C" CGameMaster *fn_1_A328(CGameMaster *self, s16 value)
{

    if (self != 0) {
        fn_1_1415C(self->m_f0, 1);
        if (value > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

extern "C" CGameMaster *fn_1_A380(CGameMaster *self, s16 value)
{

    if (self != 0) {
        fn_1_15724(self->m_f0, 1);
        if (value > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

extern "C" CGameMaster *fn_1_A3D8(CGameMaster *self, s16 value)
{

    if (self != 0) {
        fn_1_7620(self->m_f0, 1);
        if (value > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

extern "C" CGameMaster *fn_1_A430(CGameMaster *self, s16 value)
{

    if (self != 0) {
        fn_1_A488(self->m_f0, 1);
        if (value > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

extern "C" void fn_1_A488(void *self, s32 mode)
{
}

extern "C" void fn_1_A740(void)
{
}

extern "C" void fn_1_A744(void)
{
}

extern "C" void fn_1_A748(void *self)
{
}

extern "C" CGameMaster *fn_1_A808(CGameMaster *self, s16 value)
{

    if (self != 0) {
        fn_1_2DB4(self, 0);
        if (value > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

extern "C" void fn_1_A860(void *self)
{
}

extern "C" CGameMaster *fn_1_A920(CGameMaster *self, s16 value)
{

    if (self != 0) {
        fn_1_2DB4(self, 0);
        if (value > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

extern "C" void fn_1_A978(void *self)
{
}

extern "C" CGameMaster *fn_1_AB38(CGameMaster *self, s16 value)
{

    if (self != 0) {
        fn_1_2DB4(self, 0);
        if (value > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

extern "C" void fn_1_AB90(void *self)
{
}

extern "C" void fn_1_AC64(void *self)
{
}

extern "C" void fn_1_ADA8(void *self)
{
}

extern "C" void fn_1_B058(void *self)
{
}

extern "C" void fn_1_B2CC(void *self)
{
    fn_1_B8A8(self);
    fn_1_BA58();
    fn_1_BAFC(self);
    fn_1_3920(self);
}

extern "C" void fn_1_B310(void *self)
{
}

extern "C" void fn_1_B414(void *self)
{
}

extern "C" void fn_1_B4C4(void *self)
{
}

extern "C" void fn_1_B5C0(void *self)
{
}

extern "C" void fn_1_B660(void *self)
{
}

extern "C" void fn_1_B730(void *self)
{
}

extern "C" void fn_1_B8A8(void *self)
{
}

extern "C" void fn_1_BA58(void)
{
}

extern "C" void fn_1_BAFC(void *self)
{
}

extern "C" void fn_1_BBA0(void *self)
{
}

extern "C" void fn_1_BC3C(void *self)
{
}

extern "C" void fn_1_BCD8(void *self)
{
}

extern "C" void fn_1_BD6C(void)
{
    lbl_1_bss_A10 = 0x78;
    lbl_1_bss_A14 = 0xD2;
    lbl_1_bss_A18 = 0xD2;
}

extern "C" void fn_1_BDA0(void *self)
{
    fn_1_B058((char *)self - 0x20);
}

extern "C" void fn_1_BDA8(void *self)
{
    fn_1_AB90((char *)self - 0x20);
}