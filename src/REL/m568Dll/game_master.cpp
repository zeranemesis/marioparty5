#include "dolphin/types.h"

// game_master.cpp holds the board-side "game master" object and the
// scene-object list helpers. The class layout is derived from the offsets used
// by every function in this unit; the dispatch tables are emitted verbatim as
// `lbl_1_data_*` arrays in binary order, so no C++ class is declared.
struct GameMaster;

typedef void (*PMF2)(void *, s32);

extern "C" {

struct GameMaster {
    GameMaster *m_f0;   // 0x00 list head sentinel
    void *m_f4;         // 0x04 list head next / dispatch table
    union {
        u8 m_f8;
        void *m_f8p;
    };                 // 0x08
    void *m_fC;         // 0x0C second list next
    void *m_f10;        // 0x10 third list head sentinel
    void *m_f14;        // 0x14 third list next
    u8 m_pad18[8];
    void *m_f20;        // 0x20 second dispatch table
    u8 m_pad24[0x20];
    void *m_f44;        // 0x44
    u8 m_pad48[4];
    void *m_f4C;        // 0x4C
    void *m_f50;        // 0x50
    void *m_f54;        // 0x54
    u8 m_pad58[0x1C];
    void *m_f74;        // 0x74
    u8 m_pad78[0x40];
    u8 m_fB8;           // 0xB8
    u8 m_padB9[0x29F];
    void *m_f358;       // 0x358
};

struct SceneNode {
    SceneNode *m_next;  // 0x00
    void *m_f4;         // 0x04
    union {
        u8 m_f8;        // 0x08 flag
        void *m_f8p;    // 0x08 polymorphic sub-object
    };
    u8 m_pad[0x34];
    s32 m_fC;         // 0x0C
    u8 m_pad2[0x34];
    void *m_f44;        // 0x44
    u8 m_pad48[4];
    void *m_f4C;        // 0x4C
    void *m_f50;        // 0x50
    void *m_f54;        // 0x54
    u8 m_pad58[0x1C];
    void *m_f74;        // 0x74
    u8 m_pad78[0x40];
    u8 m_fB8;           // 0xB8
};

// scene / object helpers living in m568.cpp, input.cpp, octtree.cpp, tube.cpp
void fn_1_10E9C(void *self);
void fn_1_1290C(void *self);
void fn_1_7F1C(GameMaster *self);
void fn_1_837C(void *self);
void fn_1_8380(void *self);
GameMaster *fn_1_8090(GameMaster *self, s16 arg1);
GameMaster *fn_1_80E8(GameMaster *self, s16 arg1);
GameMaster *fn_1_8798(GameMaster *self, s16 arg1);
void fn_1_8824(void);
void fn_1_8A08(GameMaster *self);
void fn_1_8DC8(GameMaster *self);
void fn_1_970C();
void fn_1_9890(GameMaster *self);
void fn_1_9898(GameMaster *self);

void fn_1_187C(void *self);
void fn_1_1888(void *self, s32 arg1);
void fn_1_23F8();
void fn_1_2558(void *self);
void fn_1_26E4(void *self, s32 arg1);
void fn_1_289C(void *self, s32 arg1, s32 arg2);
void fn_1_370C(void *self, s32 arg1);
void fn_1_4DFC(void *self);
BOOL fn_1_5634(void *self, void *arg1);
void fn_1_58FC(void *self, s32 arg1);
void fn_1_5880(void *self, s32 arg1);
void fn_1_6E0(void *self);
void fn_1_5020(u64 tick);
u64 OSGetTick(void);
void *fn_1_9D60(void *self);
void *fn_1_B668(void *self);
void *fn_1_D260(void *self);
void *fn_1_104FC(void *self, s32 arg1);
void fn_1_7EC(char *msg, char *file, s32 line);
void *fn_1_618(u32 size);
void fn_1_F990(void *self, void *arg1);
void fn_1_123DC(void *self, s32 arg1);
void fn_1_140D8(void *self, void *arg1);
s32 HuAudSStreamPlay(u32 id);
void fn_1_2260(s32 phase);
BOOL fn_1_22F8(void);
void fn_1_2D1C(void);
BOOL fn_1_2E8C(void);
void fn_1_3204(void);
void fn_1_5A40(void *self, s32 arg1);
void fn_1_1290C(void *self);
void fn_1_13F08(void *self, void *arg1);
void fn_1_2384(void);
s32 fn_1_59B0(void *self, s32 arg1);
void fn_1_972C(SceneNode *self, void (*func)(void *, void *));
void fn_1_97C0(SceneNode *self, SceneNode *arg1, void (*func)(void *, void *));

extern SceneNode *lbl_1_bss_9F4;
extern SceneNode *lbl_1_bss_9F8;
extern SceneNode *lbl_1_bss_A0C;
extern s32 lbl_1_bss_A00;
extern s32 lbl_1_bss_A04;
extern s32 lbl_1_bss_A08;

char lbl_1_data_2A8[] = "sm_ptr == NULL";
char lbl_1_data_2B7[] = "a_set.h";
char lbl_1_data_2BF[] = "p_ms! rt\0" "0 =";

// NONMATCHING: the four tables between the assert strings and the dispatch
// table are pointer-to-member-function records; three are still unknown.
typedef struct PTMF {
    s32 this_delta;
    s32 v_offset;
    void *f_addr;
} PTMF;

PTMF lbl_1_data_2CC = { 0, -1, (void *)fn_1_10E9C };
PTMF lbl_1_data_2D8 = { 0, -1, (void *)fn_1_1290C };
PTMF lbl_1_data_2E4 = { 0, 0, 0 };
PTMF lbl_1_data_2F0 = { 0, 0, 0 };
PTMF lbl_1_data_2FC = { 0, 0, 0 };
PTMF lbl_1_data_308 = { 0, 0, 0 };
char lbl_1_data_314[0x10];
char lbl_1_data_324[0xC];

void *lbl_1_data_330[12] = {
    NULL, NULL, (void *)fn_1_8798, (void *)fn_1_8824, (void *)fn_1_8A08, (void *)fn_1_970C,
    NULL, NULL, (void *)fn_1_9898, (void *)fn_1_9890, (void *)fn_1_8DC8, NULL
};

const f32 lbl_1_rodata_110 = -39000.0f;

// NONMATCHING: MWCC keeps the three singleton pointers in more temporaries than this.

void fn_1_7F1C(GameMaster *self)
{
    void *set;

    fn_1_2558(self);
    fn_1_5880(self->m_f20, 0x8000);
    self->m_f4 = (void *)&lbl_1_data_330[0];
    self->m_f20 = (void *)&lbl_1_data_330[6];

    set = fn_1_618(0x18);
    if (set != NULL) {
        fn_1_187C(set);
        if (lbl_1_bss_A0C != 0) {
            fn_1_7EC(lbl_1_data_2A8, lbl_1_data_2B7, 0x1D);
        }
        lbl_1_bss_A0C = (SceneNode *)set;
        fn_1_187C((char *)set + 8);
        if (lbl_1_bss_9F8 != 0) {
            fn_1_7EC(lbl_1_data_2A8, lbl_1_data_2B7, 0x1D);
        }
        lbl_1_bss_9F8 = (SceneNode *)((char *)set + 8);
        fn_1_187C((char *)set + 0x10);
        if (lbl_1_bss_9F4 != 0) {
            fn_1_7EC(lbl_1_data_2A8, lbl_1_data_2B7, 0x1D);
        }
        lbl_1_bss_9F4 = (SceneNode *)((char *)set + 0x10);
        self->m_f358 = set;
    } else {
        self->m_f358 = NULL;
    }
}

GameMaster *fn_1_8090(GameMaster *self, s16 arg1)
{
    if (self) {
        fn_1_80E8(self->m_f0, 1);
        if (arg1 > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

#pragma noinline
// NONMATCHING: the three list walks use different callee-saved registers here.

GameMaster *fn_1_80E8(GameMaster *self, s16 arg1)
{
    SceneNode *node;
    SceneNode *node2;
    SceneNode *node3;
    bool flag;

    if (self) {
        do {
            node3 = (SceneNode *)self->m_f14;
            if (node3) {
                fn_1_370C((char *)node3 + 0x18, -1);
                if (node3) {
                    fn_1_1888(node3, 0);
                }
                fn_1_6E0(node3);
            }
            flag = (self->m_f10 == (void *)&self->m_f10) && (self->m_f10 == self->m_f14);
        } while (!flag);
        if (&self->m_f10) {
            fn_1_1888(self->m_f10, 0);
        }

        do {
            node2 = (SceneNode *)self->m_fC;
            fn_1_123DC(node2, 1);
            flag = (self->m_f8p == (void *)&self->m_f8p) && (self->m_f8p == self->m_fC);
        } while (!flag);
        if (&self->m_f8p) {
            fn_1_1888(self->m_f8p, 0);
        }

        if (self) {
            do {
                node = (SceneNode *)self->m_f4;
                if (node) {
                    if (&node->m_f54) {
                        fn_1_123DC(node->m_f54, 1);
                    }
                    if (&node->m_f50) {
                        fn_1_370C(node->m_f50, 1);
                    }
                    if (&node->m_f44) {
                        if (&node->m_f44) {
                            if (&node->m_f44) {
                                if (node->m_f4C) {
                                    fn_1_8380(&node->m_f44);
                                    fn_1_837C(&node->m_f44);
                                    fn_1_6E0(node->m_f4C);
                                }
                            }
                        }
                    }
                    if (&node->m_f8p) {
                        if (node->m_f8p) {
                            void **vtbl = *(void ***)node->m_f8p;
                            ((PMF2)vtbl[2])(node->m_f8p, 1);
                        }
                    }
                    if (node) {
                        fn_1_1888(node, 0);
                    }
                    fn_1_6E0(node);
                }
                flag = (self->m_f0 == (GameMaster *)&self->m_f0) && (self->m_f0 == self->m_f4);
            } while (!flag);
            if (self) {
                fn_1_1888(self, 0);
            }
        }
        if (arg1 > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

void fn_1_837C(void *self)
{
}

void fn_1_8380(void *self)
{
}

// NONMATCHING: the list-node pointer is held in r24 here, not r31.

GameMaster *fn_1_8384(GameMaster *self, s16 arg1)
{
    SceneNode *node;
    bool flag;

    if (self) {
        do {
            node = (SceneNode *)self->m_f4;
            if (node) {
                fn_1_370C((char *)node + 0x18, -1);
                if (node) {
                    fn_1_1888(node, 0);
                }
                fn_1_6E0(node);
            }
            flag = (self->m_f0 == (GameMaster *)&self->m_f0) && (self->m_f0 == self->m_f4);
        } while (!flag);

        if (self) {
            fn_1_1888(self, 0);
        }
        if (arg1 > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

GameMaster *fn_1_845C(GameMaster *self, s16 arg1)
{
    if (self) {
        fn_1_1888(self, 0);
        if (arg1 > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

// NONMATCHING: two rows still differ in the sentinel compare.

GameMaster *fn_1_84B4(GameMaster *self, s16 arg1)
{
    SceneNode *node;
    bool flag;

    if (self) {
        do {
            node = (SceneNode *)self->m_f4;
            fn_1_123DC(node, 1);
            flag = (self->m_f0 == (GameMaster *)&self->m_f0) && (self->m_f0 == self->m_f4);
        } while (!flag);

        if (self) {
            fn_1_1888(self, 0);
        }
        if (arg1 > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

GameMaster *fn_1_8574(GameMaster *self, s16 arg1)
{
    if (self) {
        fn_1_1888(self, 0);
        if (arg1 > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

// NONMATCHING: the null-check temporaries for 0x44/0x50/0x54 are not reproduced.

GameMaster *fn_1_85CC(GameMaster *self, s16 arg1)
{
    SceneNode *node;
    bool flag;

    if (self) {
        do {
            node = (SceneNode *)self->m_f4;
            if (node) {
                if (node->m_f54 != NULL) {
                    fn_1_123DC(node->m_f54, 1);
                }
                if (node->m_f50 != NULL) {
                    fn_1_370C(node->m_f50, 1);
                }
                if (node->m_f44 != NULL) {
                    if (node->m_f44 != NULL) {
                        if (node->m_f44 != NULL) {
                            if (node->m_f4C != NULL) {
                                fn_1_6E0(node->m_f4C);
                            }
                        }
                    }
                }
                if (node->m_f8p != NULL) {
                    if (node->m_f8p != NULL) {
                        void **vtbl = *(void ***)node->m_f8p;
                        ((PMF2)vtbl[2])(node->m_f8p, 1);
                    }
                }
                if (node) {
                    fn_1_1888(node, 0);
                }
                fn_1_6E0(node);
            }
            flag = (self->m_f0 == (GameMaster *)&self->m_f0) && (self->m_f0 == self->m_f4);
        } while (!flag);

        if (self) {
            fn_1_1888(self, 0);
        }
        if (arg1 > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

GameMaster *fn_1_8740(GameMaster *self, s16 arg1)
{
    if (self) {
        fn_1_1888(self, 0);
        if (arg1 > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

// NONMATCHING: dispatch-table stores land in a different order.

GameMaster *fn_1_8798(GameMaster *self, s16 arg1)
{
    if (self) {
        self->m_f4 = (void *)&lbl_1_data_330[0];
        self->m_f20 = (void *)&lbl_1_data_330[6];
        fn_1_8090((GameMaster *)self->m_f358, -1);
        fn_1_58FC(self->m_f20, 0);
        fn_1_26E4(self, 0);
        if (arg1 > 0) {
            fn_1_4DFC(self);
        }
    }
    return self;
}

#pragma noinline
// NONMATCHING: needs two more live locals to reach the 0x50 frame.

void fn_1_8824(void)
{
    SceneNode *node;
    SceneNode *end;
    s32 num;
    void *obj1;
    void *obj2;
    void *obj3;
    void *obj4;
    s32 i;

    fn_1_5020(OSGetTick());

    obj1 = fn_1_618(0x60);
    if (obj1) {
        obj1 = fn_1_D260(obj1);
    }
    obj2 = fn_1_618(0x8C);
    if (obj2) {
        obj2 = fn_1_9D60(obj2);
    }
    obj3 = fn_1_618(0x18);
    if (obj3) {
        obj3 = fn_1_B668(obj3);
    }
    for (i = 0; i < 4; i++) {
        obj4 = fn_1_618(0x58);
        if (obj4) {
            obj4 = fn_1_104FC(obj4, i);
        }
    }

    if (lbl_1_bss_9F8 == 0) {
        fn_1_7EC(lbl_1_data_2BF, lbl_1_data_2B7, 0x3A);
    }
    end = lbl_1_bss_9F8;
    node = end->m_next;
    num = 1;
    while (node != end) {
        if (node->m_f8 != 0) {
            node->m_fC = 0;
        } else {
            node->m_fC = num;
            num++;
        }
        node = node->m_next;
    }
}

void fn_1_898C(void *self, void *arg1)
{
    if (fn_1_5634((char *)self + 0x74, (char *)arg1 + 8) != 0) {
        fn_1_140D8(self, arg1);
        fn_1_F990(arg1, self);
    }
}

#pragma noinline
// The original dispatches these records through __ptmf_scall, which adds
// this_delta to the object and then either tail-calls f_addr directly
// (v_offset < 0) or indexes the object vtable. Every record used here has
// this_delta 0 and v_offset -1, so the direct call is equivalent.
// NONMATCHING: emits a direct call instead of bl __ptmf_scall with r12.
static void pmf_call(const PTMF *pmf, void *obj)
{
    ((void (*)(void *))pmf->f_addr)(obj);
}

// NONMATCHING: emits direct calls where the original uses bl __ptmf_scall with r12.

void fn_1_8A08(GameMaster *self)
{
    SceneNode *master;
    SceneNode *listA;
    SceneNode *listB;
    SceneNode *node;
    PTMF pmf1;
    PTMF pmf2;

    if (fn_1_59B0(self->m_f20, 0) < 0) {
        return;
    }
    fn_1_2384();
    if (lbl_1_bss_A0C == 0) {
        fn_1_7EC(lbl_1_data_2BF, lbl_1_data_2B7, 0x3A);
    }
    master = lbl_1_bss_A0C;
    if (lbl_1_bss_9F8 == 0) {
        fn_1_7EC(lbl_1_data_2BF, lbl_1_data_2B7, 0x3A);
    }
    listA = lbl_1_bss_9F8;
    if (lbl_1_bss_9F4 == 0) {
        fn_1_7EC(lbl_1_data_2BF, lbl_1_data_2B7, 0x3A);
    }
    listB = lbl_1_bss_9F4;

    pmf1 = lbl_1_data_2CC;
    node = master->m_next;
    while (node != master) {
        pmf_call(&pmf1, node);
        node = node->m_next;
    }

    fn_1_972C(listA, fn_1_13F08);
    fn_1_97C0(listA, listB, fn_1_898C);

    pmf2 = lbl_1_data_2D8;
    node = listA->m_next;
    while (node != listA) {
        pmf_call(&pmf2, node);
        node = node->m_next;
    }
}

// NONMATCHING: the list head is re-read through four stack slots here.

s32 fn_1_8C24(void)
{
    SceneNode *node;
    SceneNode *end;
    s32 ret;

    if (lbl_1_bss_9F8 == 0) {
        fn_1_7EC(lbl_1_data_2BF, lbl_1_data_2B7, 0x3A);
    }
    end = lbl_1_bss_9F8;
    node = end->m_next;
    ret = 0;
    do {
        ret = 0;
        if (node->m_f8 == 0 && node->m_fB8 == 0
            && *(f32 *)((char *)node->m_f74 + 8) < lbl_1_rodata_110) {
            ret = 1;
        }
        node = node->m_next;
    } while (node != end);
    return ret;
}

// NONMATCHING: register allocation only.

s32 fn_1_8D04(void)
{
    SceneNode *node;
    SceneNode *end;

    if (lbl_1_bss_9F8 == 0) {
        fn_1_7EC(lbl_1_data_2BF, lbl_1_data_2B7, 0x3A);
    }
    end = lbl_1_bss_9F8;
    node = end->m_next;
    do {
        if (node->m_f8 == 0 && node->m_fB8 == 0) {
            return 0;
        }
        node = node->m_next;
    } while (node != end);
    return 1;
}

#pragma noinline
// The two phase gates at the top of this routine are reconstructed; the long
// scene update that follows them (the list walk over lbl_1_bss_9F8 and the
// per-object updates) is not yet written.
// NONMATCHING: the update body after the fn_1_2260(2) phase switch is missing.
void fn_1_8DC8(GameMaster *self)
{
    fn_1_2260(0);
    fn_1_3204();
    while (!fn_1_22F8()) {
        fn_1_5A40(self->m_f20, 0);
    }

    fn_1_2260(1);
    fn_1_2D1C();
    while (!fn_1_2E8C()) {
        fn_1_5A40(self->m_f20, 0);
    }

    fn_1_2260(2);
}

void fn_1_970C()
{
    fn_1_23F8();
}

// NONMATCHING: loop heads read the list pointers through extra stack slots.

void fn_1_972C(SceneNode *self, void (*func)(void *, void *))
{
    SceneNode *end = self;
    SceneNode *stop = (SceneNode *)self->m_f4;
    SceneNode *outer = (SceneNode *)self->m_next;

    do {
        SceneNode *inner = outer->m_next;
        while (inner != end) {
            func(outer, inner);
            inner = inner->m_next;
        }
        outer = outer->m_next;
    } while (outer != stop);
}

void fn_1_97C0(SceneNode *self, SceneNode *arg1, void (*func)(void *, void *))
{
    SceneNode *end = self;
    SceneNode *outer = self->m_next;

    do {
        SceneNode *stop = arg1;
        SceneNode *inner = arg1->m_next;
        while (inner != stop) {
            func(outer, inner);
            inner = inner->m_next;
        }
        outer = outer->m_next;
    } while (outer != end);
}

void fn_1_985C(void)
{
    lbl_1_bss_A00 = 0x78;
    lbl_1_bss_A04 = 0xD2;
    lbl_1_bss_A08 = 0xD2;
}

// NONMATCHING: tail call into fn_1_8DC8 is not reproduced.

void fn_1_9890(GameMaster *self)
{
    fn_1_8DC8((GameMaster *)((char *)self - 0x20));
}

// NONMATCHING: tail call into fn_1_8798 is not reproduced.

void fn_1_9898(GameMaster *self)
{
    fn_1_8798((GameMaster *)((char *)self - 0x20), 0);
}

}