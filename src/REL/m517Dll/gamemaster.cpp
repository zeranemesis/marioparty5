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
    void *m_f360;      // 0x360
    void *m_f364;      // 0x364
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

// Head of the circular sub-object list that fn_1_A748 / fn_1_A860 walk.
struct CSubHead {
    struct CSubHead *m_next;   // 0x00
    struct CSubHead *m_first;  // 0x04
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
void fn_1_10F8(void *self, s32 flag);
void fn_1_10F88(void *self, s32 flag);
void fn_1_1415C(void *obj, s32 mode);
void fn_1_15724(void *obj, s32 mode);
void fn_1_7620(void *obj, s32 mode);
void fn_1_B8A8(void *self);
void fn_1_BA58(void *self);
void fn_1_10F8(void *self, s32 flag);
void fn_1_10F88(void *self, s32 flag);
void fn_1_BA58(void *self);
void fn_1_BAFC(void *self);
void fn_1_3920(void);
void fn_1_111A8(void);
void fn_1_17614(void);
void fn_1_16B68(void);
void fn_1_16918(void);
void fn_1_13D4(void);
void fn_1_11850(void);
unsigned char fn_1_78D0(void *self);
void fn_1_A488(void *self, s32 mode);
void fn_1_629C(void *self);
void fn_1_6D9C(void *self, s32 mode);
void fn_1_3B84(void *self, s32 mode);
void fn_1_64C0(void);
void *fn_1_E3A4(void *self);
void *fn_1_DC4C(void *self);
void *fn_1_15DC8(void *self, s32 idx);
void *fn_1_10634(void *self);
void *fn_1_124B4(void *self);
void *fn_1_13F7C(void *self);
u32 OSGetTick(void);
CGameMaster *fn_1_AB90(CGameMaster *self, s16 value);
void fn_1_AC64(CGameMaster *self);
void fn_1_ADA8(void *self);
void fn_1_B058(void *self);

// smart-pointer slots owned by other units of this REL
extern void *lbl_1_bss_0;
extern void *lbl_1_bss_A08;

extern "C" void fn_1_A168(CGameMaster *self);
extern "C" CSubHead *fn_1_A748(CSubHead *self, s16 value);
extern "C" CSubHead *fn_1_A860(CSubHead *self, s16 value);
extern "C" void fn_1_A978(void *self);
extern "C" void fn_1_B2CC(CGameMaster *self);
extern "C" s32 fn_1_B310(CGameMaster *self);
extern "C" s32 fn_1_B414(void);
extern "C" void fn_1_B4C4(CGameMaster *self);
extern "C" void fn_1_B5C0(void);
extern "C" void fn_1_B660(void);
extern "C" void fn_1_B730(void *self);
extern "C" void fn_1_B8A8(void *self);
void fn_1_BA58(void *self);
void fn_1_10F8(void *self, s32 flag);
void fn_1_10F88(void *self, s32 flag);
extern "C" void fn_1_BAFC(void *self);
extern "C" void fn_1_BBA0(void *listA, void *listB, void (*fn)(void *, void *));
extern "C" void fn_1_BC3C(void *listA, void *listB, void (*fn)(void *, void *));
extern "C" void fn_1_BCD8(void *listA, void (*fn)(void *, void *));
extern "C" void fn_1_BDA0(void *self);
extern "C" void fn_1_BDA8(void *self, s16 value);
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

extern "C" CSubHead *fn_1_A748(CSubHead *self, s16 value)
{
    if (self != 0) {
while ((self->m_next == self && self->m_next == self->m_first) == 0) {
            CSubHead *first = self->m_first;
            CSubHead *sub = first;
            fn_1_10F88(sub, 1);
        }
        if (self != 0) {
            fn_1_2DB4(self, 0);
        }
        if (value > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
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

extern "C" CSubHead *fn_1_A860(CSubHead *self, s16 value)
{
    if (self != 0) {
        while ((self->m_next == self && self->m_next == self->m_first) == 0) {
            CSubHead *first = self->m_first;
            CSubHead *sub = first;
            fn_1_10F8(sub, 1);
        }
        if (self != 0) {
            fn_1_2DB4(self, 0);
        }
        if (value > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
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

extern "C" CGameMaster *fn_1_AB90(CGameMaster *self, s16 value)
{
    if (self != 0) {
        self->m_vtbl = lbl_1_data_3C0;
        self->m_vtbl2 = &lbl_1_data_3C0[6];
        if ((void *)&self->m_f364 != 0) {
            fn_1_1415C(self->m_f364, 1);
        }
        if ((void *)&self->m_f360 != 0) {
            fn_1_15724(self->m_f360, 1);
        }
        if ((void *)&self->m_f35C != 0) {
            fn_1_7620(self->m_f35C, 1);
        }
        fn_1_A430((CGameMaster *)((char *)self + 0x358), -1);
        fn_1_6D9C(&self->m_vtbl2, 0);
        fn_1_3B84(self, 0);
        if (value > 0) {
            fn_1_629C(self);
        }
    }
    return self;
}

extern "C" void fn_1_AC64(CGameMaster *self)
{
    OSGetTick();
    fn_1_64C0();

    void *stage = fn_1_1E84(0x24);
    if (stage != 0) {
        stage = fn_1_E3A4(stage);
    }

    void *camera = fn_1_1E84(0x80);
    if (camera != 0) {
        camera = fn_1_DC4C(camera);
    }

    for (s32 i = 0; i < 4; i++) {
        void *item = fn_1_1E84(0xD4);
        if (item != 0) {
            item = fn_1_15DC8(item, i);
        }
    }

    for (s32 j = 0; j < 0xC; j++) {
        void *node = fn_1_1E84(0x7C);
        if (node != 0) {
            node = fn_1_10634(node);
        }
    }

    void *board = fn_1_1E84(0xB4);
    if (board != 0) {
        board = fn_1_124B4(board);
    }

    void *scene = fn_1_1E84(0x20);
    if (scene != 0) {
        scene = fn_1_13F7C(scene);
    }

    void *built = scene;

    if (self->m_f364 != built) {
        fn_1_1415C(self->m_f364, 1);
        self->m_f364 = built;
    }
}

extern "C" void fn_1_ADA8(void *self)
{
}

#pragma dont_inline on
extern "C" void fn_1_B058(void *self)
{
}
#pragma dont_inline reset

extern "C" void fn_1_B2CC(CGameMaster *self)
{
    fn_1_B8A8(self);
    fn_1_BA58(self);
    fn_1_BAFC(self);
    fn_1_3920();
}

extern "C" s32 fn_1_B310(CGameMaster *self)
{
    struct CL {
        struct CL *m_next;    // 0x00
        char m_f4[8];
        unsigned char m_fC;   // 0x0C
        char m_fD[0x1F];
        unsigned char m_2C;   // 0x2C
    };

if (lbl_1_bss_A08 == 0) {
        fn_1_2058(lbl_1_data_398, lbl_1_data_36B, 0x2E);
    }

    void *smA08 = lbl_1_bss_A08;
    void *smRoot = smA08;
    CL *root = (CL *)smRoot;
    CL *first = root->m_next;
    CL *entry = first;
    CL *node = entry;
    CL *back = root;
    CL *stop = back;
    unsigned char ok = 1;

    do {
        if (node->m_2C == 0) {
            if (node->m_fC != 1) {
                ok = 0;
            }
        }
        CL *next = node->m_next;
        node = next;
    } while (node != stop);

    if (ok != 0) {
        self->m_f368 = 1;
        return 1;
    }

    void *sub = self->m_f35C;
    if ((unsigned char)fn_1_78D0(sub) != 0) {
        self->m_f368 = 0;
        return 1;
    }
    return 0;
}

// Node of the circular list hanging off lbl_1_bss_A08. 0x08 is read as a word
// by fn_1_B414 and 0x0C as a byte elsewhere, so only one of the two is ever
// meaningful for a given list.
struct CSmNode;
struct CSmRecNode;

struct CSmNode {
    struct CSmNode *m_next;         // 0x00
    char m_f4[4];          // 0x04
    s32 m_word;           // 0x08
    unsigned char m_fC;   // 0x0C
    char m_fD[0x1F];      // 0x0D
    unsigned char m_2C;   // 0x2C
};

// Same list, but carrying a singleton record inline at +0x10 (fn_1_B660).
struct CSmRecNode {
    struct CSmRecNode *m_next;  // 0x00
    char m_f4[0xC];
    CSingleton m_rec;           // 0x10
};

// fn_1_B730 walks the lbl_1_bss_A1C list, whose nodes hold their record at +0x70.
struct CSmRec70 {
    struct CSmRec70 *m_next;  // 0x00
    char m_f4[0x6C];
    CSingleton m_rec;         // 0x70
};

// Teardown node reached through the lbl_1_bss_A08 list (fn_1_B8A8).
struct CNodeD0 {
    struct CNodeD0 *m_next;   // 0x00
    char m_f4[0xB0];
    void *m_b4;               // 0xB4
    void *m_b8;               // 0xB8
    void **m_bc;              // 0xBC
    void *m_c0;               // 0xC0
    void *m_c4;               // 0xC4
    void *m_c8;               // 0xC8
    void *m_cc;               // 0xCC
    void *m_d0;               // 0xD0
};

// Two-step indirect call target: obj->m_link->m_fn / obj->m_f0->m_fn.
struct CInd {
    void *m_f0;
    void *m_f4;
    void *(*m_fn)(void *, s32);
};

extern "C" void fn_1_1593C(void *self);
extern "C" void fn_1_184E8(void *self, s32 flag);

extern "C" s32 fn_1_B414(void)
{
    if (lbl_1_bss_A08 == 0) {
        fn_1_2058(lbl_1_data_398, lbl_1_data_36B, 0x2E);
    }

    void *smA08 = lbl_1_bss_A08;
    void *smRoot = smA08;
    CSmNode *root = (CSmNode *)smRoot;
    CSmNode *first = root->m_next;
    CSmNode *entry = first;
    CSmNode *node = entry;
    CSmNode *back = root;
    CSmNode *stop = back;

    do {
        if (node->m_word > 0) {
            return 0;
        }
        CSmNode *next = node->m_next;
        node = next;
    } while (node != stop);
    return 1;
}

extern "C" void fn_1_B4C4(CGameMaster *self)
{
    if (lbl_1_bss_A08 == 0) {
        fn_1_2058(lbl_1_data_398, lbl_1_data_36B, 0x2E);
    }

    void *smA08 = lbl_1_bss_A08;
    void *smRoot = smA08;
    CSmNode *root = (CSmNode *)smRoot;
    CSmNode *first = root->m_next;
    CSmNode *entry = first;
    CSmNode *node = entry;
    CSmNode *back = root;
    CSmNode *stop = back;

    do {
        if (self->m_f368 != 0) {
            if (node->m_2C != 0) {
                fn_1_184E8(node, 1);
            } else {
                fn_1_184E8(node, 0);
            }
        } else {
            if (node->m_2C != 0) {
                fn_1_184E8(node, 0);
            } else {
                fn_1_184E8(node, 1);
            }
        }
        CSmNode *next = node->m_next;
        node = next;
    } while (node != stop);
}

extern "C" void fn_1_B5C0(void)
{
    if (lbl_1_bss_A08 == 0) {
        fn_1_2058(lbl_1_data_398, lbl_1_data_36B, 0x2E);
    }

    void *smA08 = lbl_1_bss_A08;
    void *smRoot = smA08;
    CSmNode *root = (CSmNode *)smRoot;
    CSmNode *first = root->m_next;
    CSmNode *entry = first;
    CSmNode *node = entry;
    CSmNode *back = root;
    CSmNode *stop = back;

    do {
        fn_1_1593C(node);
        CSmNode *next = node->m_next;
        node = next;
    } while (node != stop);
}

extern "C" void fn_1_B660(void)
{
    if (lbl_1_bss_A08 == 0) {
        fn_1_2058(lbl_1_data_398, lbl_1_data_36B, 0x2E);
    }

    void *smA08 = lbl_1_bss_A08;
    void *smRoot = smA08;
    CSmRecNode *root = (CSmRecNode *)smRoot;
    CSmRecNode *first = root->m_next;
    CSmRecNode *entry = first;
    CSmRecNode *node = entry;
    CSmRecNode *back = root;
    CSmRecNode *stop = back;

    do {
        CSingleton rec = lbl_1_data_350;
        node->m_rec = rec;
        CSmRecNode *next = node->m_next;
        node = next;
    } while (node != stop);
}

extern "C" void fn_1_B730(void *self)
{
}

extern "C" void fn_1_B8A8(void *self)
{
}

extern "C" void fn_1_BA58(void *self)
{
    if (lbl_1_bss_0 == 0) {
        fn_1_2058(lbl_1_data_398, lbl_1_data_36B, 0x2E);
    }

    void *smZero = lbl_1_bss_0;
    void *smRoot = smZero;
    CSmNode *root = (CSmNode *)smRoot;
    CSmNode *first = root->m_next;
    CSmNode *entry = first;
    CSmNode *node = entry;
    CSmNode *back = root;
    CSmNode *stop = back;

    do {
        fn_1_10F8(node, 1);
        CSmNode *next = node->m_next;
        node = next;
    } while (node != stop);
}

extern "C" void fn_1_BAFC(void *self)
{
    if (lbl_1_bss_A1C == 0) {
        fn_1_2058(lbl_1_data_398, lbl_1_data_36B, 0x2E);
    }

    void *smA1C = lbl_1_bss_A1C;
    void *smRoot = smA1C;
    CSmNode *root = (CSmNode *)smRoot;
    CSmNode *first = root->m_next;
    CSmNode *entry = first;
    CSmNode *node = entry;
    CSmNode *back = root;
    CSmNode *stop = back;

    do {
        fn_1_10F88(node, 1);
        CSmNode *next = node->m_next;
        node = next;
    } while (node != stop);
}

// Walks every entry of the circular list at +0x00 of listA, and for each of
// them every entry of the circular list at +0x00 of listB, calling fn.
extern "C" void fn_1_BBA0(void *listA, void *listB, void (*fn)(void *, void *))
{
    void *headA = *(void **)listA;
    void *outerStart = headA;
    void *outer = outerStart;
    void *outerRoot = listA;
    void *outerStop = outerRoot;

    do {
        void *headB = *(void **)listB;
        void *innerStart = headB;
        void *inner = innerStart;
        void *innerRoot = listB;
        void *innerStop = innerRoot;

        do {
            fn(outer, inner);
            void *nextB = *(void **)inner;
            inner = nextB;
        } while (inner != innerStop);

        void *nextA = *(void **)outer;
        outer = nextA;
    } while (outer != outerStop);
}

// Byte-for-byte twin of fn_1_BBA0.
extern "C" void fn_1_BC3C(void *listA, void *listB, void (*fn)(void *, void *))
{
    void *headA = *(void **)listA;
    void *outerStart = headA;
    void *outer = outerStart;
    void *outerRoot = listA;
    void *outerStop = outerRoot;

    do {
        void *headB = *(void **)listB;
        void *innerStart = headB;
        void *inner = innerStart;
        void *innerRoot = listB;
        void *innerStop = innerRoot;

        do {
            fn(outer, inner);
            void *nextB = *(void **)inner;
            inner = nextB;
        } while (inner != innerStop);

        void *nextA = *(void **)outer;
        outer = nextA;
    } while (outer != outerStop);
}

// Same as fn_1_BBA0 but the inner list ends at listA itself and the outer list
// ends at listA->m_second.
extern "C" void fn_1_BCD8(void *listA, void (*fn)(void *, void *))
{
    void *headA = *(void **)listA;
    void *outerStart = headA;
    void *outer = outerStart;
    void *outerRoot = listA;
    void *outerStop = *(void **)((char *)outerRoot + 4);

    do {
        void *innerStart = *(void **)outer;
        void *inner = innerStart;
        void *innerStop = listA;

        do {
            fn(outer, inner);
            void *nextB = *(void **)inner;
            inner = nextB;
        } while (inner != innerStop);

        void *nextA = *(void **)outer;
        outer = nextA;
    } while (outer != outerStop);
}

extern "C" void fn_1_BD6C(void)
{
    lbl_1_bss_A10 = 0x78;
    lbl_1_bss_A14 = 0xD2;
    lbl_1_bss_A18 = 0xD2;
}

#pragma dont_inline on
extern "C" void fn_1_BDA0(void *self)
{
    fn_1_B058((char *)self - 0x20);
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" void fn_1_BDA8(void *self, s16 value)
{
    fn_1_AB90((CGameMaster *)((char *)self - 0x20), value);
}
#pragma dont_inline reset