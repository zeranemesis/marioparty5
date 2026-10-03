#include "dolphin/types.h"

// game_master.cpp holds the board-side "game master" object and the
// scene-object list helpers. The class layout is derived from the offsets used
// by every function in this unit; the dispatch tables are emitted verbatim as
// `lbl_1_data_*` arrays in binary order, so no C++ class is declared.
struct GameMaster;

typedef void (*PMF2)(void *, s32);

extern "C" {

struct GameMaster {
    GameMaster *m_f0;   // 0x00 list head / owner
    void *m_f4;         // 0x04 dispatch table or child pointer
    u8 m_f8;            // 0x08
    u8 m_pad09[0x17];
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
    u8 m_pad[0x3B];
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
void fn_1_7F1C(GameMaster *self);
GameMaster *fn_1_8090(GameMaster *self, s16 arg1);
void fn_1_80E8(void *self, s32 arg1);
GameMaster *fn_1_8798(GameMaster *self, s16 arg1);
void fn_1_8824(void *self, s32 arg1);
void fn_1_8A08(void *self, s32 arg1);
void fn_1_8DC8(void *self, s32 arg1);
void fn_1_970C(void *self);
void fn_1_9890(GameMaster *self);
void fn_1_9898(GameMaster *self);

void fn_1_187C(void *self);
void fn_1_1888(void *self, s32 arg1);
void fn_1_23F8(void *self);
void fn_1_2558(void *self);
void fn_1_26E4(void *self, s32 arg1);
void fn_1_289C(void *self, s32 arg1, s32 arg2);
void fn_1_370C(void *self, s32 arg1);
void fn_1_4DFC(void *self);
BOOL fn_1_5634(void *self, void *arg1);
void fn_1_58FC(void *self, s32 arg1);
void fn_1_5880(void *self, s32 arg1);
void fn_1_6E0(void *self);
void fn_1_7EC(char *msg, char *file, s32 line);
void *fn_1_618(u32 size);
void fn_1_F990(void *self, void *arg1);
void fn_1_123DC(void *self, s32 arg1);
void fn_1_140D8(void *self, void *arg1);

extern SceneNode *lbl_1_bss_9F4;
extern SceneNode *lbl_1_bss_9F8;
extern GameMaster *lbl_1_bss_A0C;
extern s32 lbl_1_bss_A00;
extern s32 lbl_1_bss_A04;
extern s32 lbl_1_bss_A08;

char lbl_1_data_2A8[] = "sm_ptr == NULL";
char lbl_1_data_2B7[] = "a_set.h";
char lbl_1_data_2BF[] = "p_ms! rt\0" "0 =";

// NONMATCHING: the four tables between the assert strings and the dispatch
// table are still unknown.
u32 lbl_1_data_2CC[3];
u32 lbl_1_data_2D8[3];
u32 lbl_1_data_2E4[3];
u32 lbl_1_data_2F0[3];
u32 lbl_1_data_2FC[3];
u32 lbl_1_data_308[3];
char lbl_1_data_314[0x10];
char lbl_1_data_324[0xC];

void *lbl_1_data_330[12] = {
    NULL, NULL, (void *)fn_1_8798, (void *)fn_1_8824, (void *)fn_1_8A08, (void *)fn_1_970C,
    NULL, NULL, (void *)fn_1_9898, (void *)fn_1_9890, (void *)fn_1_8DC8, NULL
};

const f32 lbl_1_rodata_110 = -39000.0f;

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
        if (lbl_1_bss_A0C != NULL) {
            fn_1_7EC(lbl_1_data_2A8, lbl_1_data_2B7, 0x1D);
        }
        lbl_1_bss_A0C = (GameMaster *)set;
        fn_1_187C((char *)set + 8);
        if (lbl_1_bss_9F8 != NULL) {
            fn_1_7EC(lbl_1_data_2A8, lbl_1_data_2B7, 0x1D);
        }
        lbl_1_bss_9F8 = (SceneNode *)((char *)set + 8);
        fn_1_187C((char *)set + 0x10);
        if (lbl_1_bss_9F4 != NULL) {
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
void fn_1_80E8(void *self, s32 arg1)
{
}

void fn_1_837C(void)
{
}

void fn_1_8380(void)
{
}

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
            flag = (self->m_f0 == self) && (self->m_f0 == self->m_f4);
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

GameMaster *fn_1_84B4(GameMaster *self, s16 arg1)
{
    SceneNode *node;
    bool flag;

    if (self) {
        do {
            node = (SceneNode *)self->m_f4;
            fn_1_123DC(node, 1);
            flag = (self->m_f0 == self) && (self->m_f0 == self->m_f4);
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
            flag = (self->m_f0 == self) && (self->m_f0 == self->m_f4);
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
void fn_1_8824(void *self, s32 arg1)
{
}

void fn_1_898C(void *self, void *arg1)
{
    if (fn_1_5634((char *)self + 0x74, (char *)arg1 + 8) != 0) {
        fn_1_140D8(self, arg1);
        fn_1_F990(arg1, self);
    }
}

#pragma noinline
void fn_1_8A08(void *self, s32 arg1)
{
}

s32 fn_1_8C24(void)
{
    SceneNode *node;
    SceneNode *end;
    s32 ret;

    if (lbl_1_bss_9F8 == NULL) {
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

s32 fn_1_8D04(void)
{
    SceneNode *node;
    SceneNode *end;

    if (lbl_1_bss_9F8 == NULL) {
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
void fn_1_8DC8(void *self, s32 arg1)
{
}

void fn_1_970C(void *self)
{
    fn_1_23F8(self);
}

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

void fn_1_9890(GameMaster *self)
{
    fn_1_8DC8((char *)self - 0x20, 0);
}

void fn_1_9898(GameMaster *self)
{
    fn_1_8798((GameMaster *)((char *)self - 0x20), 0);
}

}