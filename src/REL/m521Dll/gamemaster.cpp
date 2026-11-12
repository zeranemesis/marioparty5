#include "dolphin/types.h"
#include <stddef.h>
#include "game/audio.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/wipe.h"
#include "REL/m521Dll/m521Types.h"

extern "C" {
// Opaque 4 byte object (count-down timer) the master keeps at 0x358.
typedef struct GameMapList {
    s32 unk0;
} GameMapList;

// Owner of the player list (a_set.h singleton list head).
typedef struct GameBoard {
    CListNode *owner;  // 0x00
} GameBoard;

// One panel of the "go" announcer: sprite/voice state plus its model.
typedef struct GameGadget {
    u8 active;      // 0x00
    s32 sfx;        // 0x04
    s32 sfxTimer;   // 0x08
    CModel *model;  // 0x0C
} GameGadget;  // size 0x10

typedef struct GameMaster {
    void **vtable;         // 0x00 base class vtable
    void **vtable2;        // 0x04 own vtable
    s8 unk08[0x18];
    CSubThread mapNode;    // 0x20 map thread (0x338 bytes)
    GameMapList mapList;   // 0x358
    s32 score[2];          // 0x35C
    s32 playerNo[2][2];    // 0x364
    s32 lastResult;        // 0x374
    CPlayer *player[2][2]; // 0x378
    GameBoard board;       // 0x388
    GameGadget gadget[2];  // 0x38C
} GameMaster;

#ifdef __MWERKS__
// GameCube layout checks: the embedded thread ends at 0x358, the player list at 0x378.
typedef char GameMasterCheck1[offsetof(GameMaster, mapList) == 0x358 ? 1 : -1];
typedef char GameMasterCheck2[offsetof(GameMaster, player) == 0x378 ? 1 : -1];
#endif

// Node of the intrusive list fn_1_134A8 walks: the container doubles as the
// sentinel its inner scan stops on, and m_last is the outer scan's end.
typedef struct GameNode {
    struct GameNode *next;  // 0x00
    struct GameNode *last;  // 0x04
} GameNode;

typedef void (*GameNodeCb)(GameNode *node1, GameNode *node2);


void __dl__FPv(void *ptr);
void *__construct_array(void *ptr, void (*ctor)(void *), void *dtor, u32 size, u32 n);
void __register_global_object(void *obj, void *dtor, void *reg);
CModel *__ct__6CModelFUi(CModel *self, u32 modelIdx);
void __dt__6CModelFv(CModel *self, s32 freeIt);
u32 OSGetTick(void);

void *fn_1_4A68(u32 n);
void fn_1_4C3C(const char *cond, const char *file, s32 line);
void fn_1_4C8(void);
void fn_1_5CCC(CListNode *self);
void fn_1_5CD8(void *obj, s16 flag);
void fn_1_6B34(void *obj, s16 flag);
void fn_1_69A8(void *obj);
void fn_1_6848(void);
void fn_1_66B0(s32 phase);
u8 fn_1_6748(void);
void fn_1_67D4(void);
void fn_1_6FC4(s32 playerNo);
void fn_1_716C(void);
void fn_1_7224(void);
u8 fn_1_72DC(void);
void fn_1_73B0(void);
void fn_1_7654(void);
void fn_1_76AC(void);
void fn_1_924C(void *obj);
void fn_1_9470(u32 seed);
s32 fn_1_9548(void);
void fn_1_9CD0(CSubThread *node, s32 arg);
void fn_1_9D4C(CSubThread *node, s16 flag);
void fn_1_9E00(CSubThread *node, s32 arg);
void fn_1_9E90(CSubThread *node, s32 arg);
void fn_1_A508(GameMapList *list, s32 arg);
void fn_1_A5D0(GameMapList *list, s16 flag);
void fn_1_A62C(GameMapList *list, s32 time, s32 a, s32 b);
void fn_1_A724(GameMapList *list, s32 arg);
void fn_1_A7E8(GameMapList *list);
void fn_1_A818(GameMapList *list);
u8 fn_1_A880(GameMapList *list);

// CClock / hand methods (clock unit)
u8 fn_1_0(CClock *c);
void fn_1_7A0(CClock *c);
u8 fn_1_7E8(CClock *c);
void fn_1_894(CClock *c, s32 hour, s32 minute);
void fn_1_924(CClock *c);
u8 fn_1_2998(CClock *c, f32 limit);
u8 fn_1_2C98(CClock *c, f32 limit);
void fn_1_42A0(CClock *c);
void fn_1_42B4(CClock *c);

// CModel helpers (m521 unit)
void fn_1_7F10(CModel *m, const char *name);
void fn_1_7FEC(CModel *m, const char *name);

// Constructors of the other singletons the master creates
CStage *fn_1_149C0(CStage *self);
CCamera *fn_1_1416C(CCamera *self);
CScore *fn_1_17F78(CScore *self);
void fn_1_1844C(CScore *self, s32 idx, s32 value);
void fn_1_145F8(CCamera *self, s32 flip);
CPlayer *fn_1_15664(CPlayer *self, s32 charNo, s32 slot);
void fn_1_15D94(CPlayer *a, CPlayer *b);
void fn_1_17788(CPlayer *p);
void fn_1_17794(CPlayer *p);
void fn_1_17884(CPlayer *p);
void fn_1_17920(CPlayer *p);
void fn_1_179E8(CPlayer *p);

// PTMF slots defined in driver.cpp (CPlayer / CScore handlers called through the list)
extern const M521Ptmf lbl_1_data_4CC;
extern const M521Ptmf lbl_1_data_520;
extern const M521Ptmf lbl_1_data_52C;

s32 lbl_1_bss_A18;
s32 lbl_1_bss_A1C;
s32 lbl_1_bss_A20;
void *lbl_1_bss_A24[3];
CModel *lbl_1_bss_A30;
void *lbl_1_bss_A34[3];
CModel *lbl_1_bss_A40;
s32 lbl_1_bss_A44;
s32 lbl_1_bss_A48;
u8 lbl_1_bss_A4C;
CClock *lbl_1_bss_A50;
CClock *lbl_1_bss_A54;
CClock *lbl_1_bss_A58;
s8 lbl_1_bss_A5C;
f32 lbl_1_bss_A60[2];
s32 lbl_1_bss_A68;
s8 lbl_1_bss_A6C;
CListNode *lbl_1_bss_A70;
CScore *lbl_1_bss_A74;
CStage *lbl_1_bss_A78;
CCamera *lbl_1_bss_A7C;
s32 lbl_1_bss_A80;
s8 lbl_1_bss_A84;

GameMaster *fn_1_11520(GameMaster *self);
void fn_1_11774(void *elem);
GameBoard *fn_1_11780(GameBoard *self, s16 flag);
CListNode *fn_1_117D8(CListNode *self, s16 flag);
void fn_1_1194C(CDeque16 *self);
void fn_1_11950(CDeque16 *self);
CListNode *fn_1_11954(CListNode *self, s16 flag);
GameBoard *fn_1_11AB0(GameBoard *self, s16 flag);
GameMaster *fn_1_11B08(GameMaster *self, s16 flag);
void fn_1_11BA0(GameMaster *self, s16 flag);
void fn_1_11E70(GameMaster *self);
s32 fn_1_1204C(GameMaster *self);
s32 fn_1_1214C(GameMaster *self);
void fn_1_122C4(GameMaster *self, s32 mask);
void fn_1_12448(GameMaster *self, s32 side);
void fn_1_12640(GameMaster *self, s32 *unused);
void fn_1_12CA8(GameMaster *self);
void fn_1_13488(void);
void fn_1_134A8(GameNode *self, GameNodeCb cb);
void fn_1_1353C(u32 *first, u32 *last);
void fn_1_137A0(u32 *a, u32 *b, u32 *c);
void fn_1_13F6C(u32 *first, u32 *last);
void fn_1_140C8(CSubThread *node);
void fn_1_140D0(CSubThread *node);

// Virtual tables of GameMaster: the first is stored at offset 4, the second
// (base at offset 0x20) lives right behind it.
void *lbl_1_data_538[12] = {
    0, 0, (void *)fn_1_11B08, (void *)fn_1_11BA0, (void *)fn_1_11E70, (void *)fn_1_13488,
    0, 0, (void *)fn_1_140D0, (void *)fn_1_140C8, (void *)fn_1_12CA8, 0
};

/* Comparator object of the sort partitions: the pivot value sits behind a pointer. */
typedef struct SortPivot {
    u8 pad;
    u32 *pivot;
} SortPivot;

u32 *fn_1_13898(u32 *first, u32 *last, SortPivot *cmp);
u32 *fn_1_13BA4(u32 *first, u32 *last, SortPivot *cmp);

#ifdef TARGET_PC
u32 m521GameMasterSize(void)
{
    return sizeof(GameMaster);
}
#endif

GameMaster *fn_1_11520(GameMaster *self)
{
    CListNode *list;
    CModel *modelA;
    CModel *modelB;

    fn_1_69A8(self);
    fn_1_9CD0(&self->mapNode, 0x8000);
    self->vtable2 = lbl_1_data_538;
    self->mapNode.m_vtbl = &lbl_1_data_538[6];
    fn_1_A508(&self->mapList, 0);
    list = (CListNode *)fn_1_4A68(sizeof(CListNode));
    if (list != 0) {
        fn_1_5CCC(list);
        if (lbl_1_bss_A70 != 0) {
            fn_1_4C3C("sm_ptr == NULL", "a_set.h", 0x11);
        }
        lbl_1_bss_A70 = list;
    }
    self->board.owner = list;
#ifdef TARGET_PC
    {
        s32 i;
        for (i = 0; i < 2; i++) {
            fn_1_11774(&self->gadget[i]);
        }
    }
#else
    __construct_array(self->gadget, fn_1_11774, 0, sizeof(GameGadget), 2);
#endif
    fn_1_9470(OSGetTick());
    modelA = (CModel *)fn_1_4A68(sizeof(CModel));
    if (modelA != 0) {
        modelA = __ct__6CModelFUi(modelA, 0x4F0002);
    }
    if (lbl_1_bss_A30 != modelA) {
        __dt__6CModelFv(lbl_1_bss_A30, 1);
        lbl_1_bss_A30 = modelA;
    }
    modelB = (CModel *)fn_1_4A68(sizeof(CModel));
    if (modelB != 0) {
        modelB = __ct__6CModelFUi(modelB, 0x4F0003);
    }
    if (lbl_1_bss_A40 != modelB) {
        __dt__6CModelFv(lbl_1_bss_A40, 1);
        lbl_1_bss_A40 = modelB;
    }
    fn_1_7F10(lbl_1_bss_A30, "dasu");
    fn_1_7F10(lbl_1_bss_A40, "dasu");
    lbl_1_bss_A5C = 0;
    self->gadget[0].active = 0;
    self->gadget[1].active = 0;
    self->gadget[0].sfx = -1;
    self->gadget[1].sfx = -1;
    self->gadget[0].model = lbl_1_bss_A30;
    self->gadget[1].model = lbl_1_bss_A40;
    return self;
}

void fn_1_11774(void *elem)
{
    ((GameGadget *)elem)->model = 0;
}

/* The original keeps this destructor out of line at its one call site
 * (fn_1_11B08); MWCC would otherwise inline the whole body there. */
#pragma dont_inline on
GameBoard *fn_1_11780(GameBoard *self, s16 flag)
{
    if (self != 0) {
        fn_1_117D8(self->owner, 1);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}
#pragma dont_inline reset

// Destroys every player of the list (the player destructor is inlined), then the list node.
CListNode *fn_1_117D8(CListNode *self, s16 flag)
{
    CPlayer *p;
    u8 empty;
    CDeque16 *mot;
    u16 *data;
    CCtl *ctl;

    if (self != 0) {
        if (self != 0) {
            while (1) {
                empty = 0;
                if (self->m_next == self && self->m_next == self->m_prev) {
                    empty = 1;
                }
                if (empty != 0) {
                    break;
                }
                p = (CPlayer *)self->m_prev;
                if (p != 0) {
                    if (&p->m_model != 0) {
                        __dt__6CModelFv(p->m_model, 1);
                    }
                    mot = &p->m_mot;
                    if (mot != 0 && mot->m_data != 0) {
                        fn_1_11950(mot);
                        data = mot->m_data;
                        fn_1_1194C(mot);
                        __dl__FPv(data);
                    }
                    if (&p->m_ctl != 0) {
                        ctl = p->m_ctl;
                        if (ctl != 0) {
                            ((void (*)(CCtl *, s32))ctl->m_vtable[2])(ctl, 1);
                        }
                    }
                    if (p != 0) {
                        fn_1_5CD8(p, 0);
                    }
                    __dl__FPv(p);
                }
            }
            if (self != 0) {
                fn_1_5CD8(self, 0);
            }
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void fn_1_1194C(CDeque16 *self)
{
}

void fn_1_11950(CDeque16 *self)
{
}

CListNode *fn_1_11954(CListNode *self, s16 flag)
{
    CPlayer *p;
    u8 empty;
    CDeque16 *mot;
    CCtl *ctl;

    if (self != 0) {
        while (1) {
            empty = 0;
            if (self->m_next == self && self->m_next == self->m_prev) {
                empty = 1;
            }
            if (empty != 0) {
                break;
            }
            p = (CPlayer *)self->m_prev;
            if (p != 0) {
                if (&p->m_model != 0) {
                    __dt__6CModelFv(p->m_model, 1);
                }
                mot = &p->m_mot;
                if (mot != 0 && mot->m_data != 0) {
                    __dl__FPv(mot->m_data);
                }
                if (&p->m_ctl != 0) {
                    ctl = p->m_ctl;
                    if (ctl != 0) {
                        ((void (*)(CCtl *, s32))ctl->m_vtable[2])(ctl, 1);
                    }
                }
                if (p != 0) {
                    fn_1_5CD8(p, 0);
                }
                __dl__FPv(p);
            }
        }
        if (self != 0) {
            fn_1_5CD8(self, 0);
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

GameBoard *fn_1_11AB0(GameBoard *self, s16 flag)
{
    if (self != 0) {
        fn_1_5CD8(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

GameMaster *fn_1_11B08(GameMaster *self, s16 flag)
{
    if (self != 0) {
        self->vtable2 = lbl_1_data_538;
        self->mapNode.m_vtbl = &lbl_1_data_538[6];
        fn_1_11780(&self->board, -1);
        fn_1_A5D0(&self->mapList, -1);
        fn_1_9D4C(&self->mapNode, 0);
        fn_1_6B34(self, 0);
        if (flag > 0) {
            fn_1_924C(self);
        }
    }
    return self;
}

void fn_1_11BA0(GameMaster *self, s16 flag)
{
    s32 order[4];
    s32 i;
    s32 half;
    s32 side;
    s32 charNo;
    CStage *stage;
    CCamera *camera;
    CScore *score;
    CPlayer *player;

    OSGetTick();
    fn_1_9470(OSGetTick());
    stage = (CStage *)fn_1_4A68(sizeof(CStage));
    if (stage != 0) {
        fn_1_149C0(stage);
    }
    camera = (CCamera *)fn_1_4A68(sizeof(CCamera));
    if (camera != 0) {
        fn_1_1416C(camera);
    }
    score = (CScore *)fn_1_4A68(sizeof(CScore));
    if (score != 0) {
        fn_1_17F78(score);
    }
    fn_1_894(lbl_1_bss_A58, 0xB, 0x37);
    for (i = 0; i < 4; i++) {
        order[i] = i | (GwPlayerConf[i].grp << 16);
    }
    fn_1_1353C((u32 *)&order[0], (u32 *)&order[4]);
    lbl_1_bss_A60[0] = 0.0f;
    lbl_1_bss_A60[1] = 0.0f;
    for (i = 0; i < 4; i++) {
        charNo = order[i] & 3;
        player = (CPlayer *)fn_1_4A68(sizeof(CPlayer));
        if (player != 0) {
            player = fn_1_15664(player, charNo, i);
        }
        half = i / 2;
        side = i % 2;
        self->player[half][side] = player;
        self->playerNo[half][side] = charNo;
        lbl_1_bss_A60[half] += (f32)(GwPlayerConf[charNo].dif + 1) * 0.25f;
    }
    for (i = 0; i < 2; i++) {
        self->player[i][0]->m_partner = self->player[i][1];
        self->player[i][1]->m_partner = self->player[i][0];
    }
    lbl_1_bss_A60[0] *= 0.5f;
    lbl_1_bss_A60[1] *= 0.5f;
    self->score[0] = 0;
    self->score[1] = 0;
}

void fn_1_11E70(GameMaster *self)
{
    s32 i;
    u8 expired;
    CListNode *list;
    CListNode *node;
    M521Ptmf pmf;
    s16 modelId;

    fn_1_9E00(&self->mapNode, 0);
    fn_1_67D4();
    if (lbl_1_bss_A70 == 0) {
        fn_1_4C3C("sm_ptr != 0", "a_set.h", 0x24);
    }
    list = lbl_1_bss_A70;
    pmf = lbl_1_data_4CC;
    node = list->m_next;
    while (node != list) {
        M521PtmfCall(node, &pmf);
        node = node->m_next;
    }
    fn_1_134A8((GameNode *)list, (GameNodeCb)fn_1_15D94);
    for (i = 0; i < 2; i++) {
        expired = 0;
        if (self->gadget[i].sfxTimer > 0) {
            if (--self->gadget[i].sfxTimer == 0) {
                expired = 1;
            }
        }
        if (expired != 0) {
            HuAudFXStop(self->gadget[i].sfx);
            self->gadget[i].sfx = -1;
        }
        if (self->gadget[i].active != 0) {
            modelId = self->gadget[i].model->m_id;
            if (Hu3DMotionEndCheck(modelId) != 0) {
                HuAudFXPlay(i == 0 ? 0x72D : 0x72E);
                self->gadget[i].active = 0;
            }
        }
    }
}

s32 fn_1_1204C(GameMaster *self)
{
    s32 mask;

    if (fn_1_A880(&self->mapList) != 0) {
        return 1;
    }
    if (fn_1_0(lbl_1_bss_A50) != 0 || fn_1_0(lbl_1_bss_A54) != 0) {
        fn_1_A7E8(&self->mapList);
        mask = fn_1_1214C(self);
        self->lastResult = mask;
        lbl_1_bss_A5C = 0;
        if (lbl_1_bss_A44 < 4) {
            while (self->gadget[0].active != 0 || self->gadget[1].active != 0) {
                fn_1_9E90(&self->mapNode, 0);
            }
            fn_1_122C4(self, mask);
        }
        return 1;
    }
    return 0;
}

s32 fn_1_1214C(GameMaster *self)
{
    s32 mask;
    CScore *score;

    mask = 0;
    if (fn_1_0(lbl_1_bss_A50) != 0) {
        mask = 1;
    }
    if (fn_1_0(lbl_1_bss_A54) != 0) {
        mask |= 2;
    }
    if (mask == 3) {
        if (lbl_1_bss_A6C == 0) {
            lbl_1_bss_A68 = 0;
            lbl_1_bss_A6C = 1;
        }
        lbl_1_bss_A68 ^= 1;
        mask = lbl_1_bss_A68 + 1;
    }
    if ((mask & 1) != 0) {
        self->score[0]++;
    }
    if ((mask & 2) != 0) {
        self->score[1]++;
    }
    if (lbl_1_bss_A74 == 0) {
        fn_1_4C3C("ms_pointer != 0", "singleton.h", 0x27);
    }
    score = lbl_1_bss_A74;
    fn_1_1844C(score, 0, self->score[0]);
    fn_1_1844C(score, 1, self->score[1]);
    return mask;
}

void fn_1_122C4(GameMaster *self, s32 mask)
{
    s32 i;
    s16 modelId;

    if ((mask & 4) != 0) {
        self->gadget[0].sfx = HuAudFXPlay(0x72F);
        self->gadget[0].sfxTimer = 0x14A;
    }
    for (i = 0; i < 2; i++) {
        if (self->score[i] < 3) {
            if ((mask & (1 << i)) != 0) {
                modelId = self->gadget[i].model->m_id;
                Hu3DMotionTimeSet(modelId, 0.0f);
                Hu3DMotionSpeedSet(modelId, 1.2727273f);
                HuAudFXPlay(i == 0 ? 0x72B : 0x72C);
                if ((mask & 4) == 0) {
                    self->gadget[i].sfx = HuAudFXPlay(i == 0 ? 0x730 : 0x731);
                    self->gadget[i].sfxTimer = 0x14A;
                    self->gadget[i].active = 1;
                    fn_1_17788(self->player[i][0]);
                    fn_1_17788(self->player[i][1]);
                }
            } else {
                fn_1_17794(self->player[i][0]);
                fn_1_17794(self->player[i][1]);
            }
        }
    }
}

void fn_1_12448(GameMaster *self, s32 side)
{
    if (side == 0) {
        Hu3DMotionTimeSet(lbl_1_bss_A30->m_id, 0.0f);
        HuAudFXPlay(0x72B);
        fn_1_7F10(lbl_1_bss_A30, "hazusuR");
        fn_1_7FEC(lbl_1_bss_A30, "dasu");
        Hu3DMotionForceSet(lbl_1_bss_A30->m_id, (char *)"daizaR", 0x100, 0.0f);
    }
    if (side == 1) {
        Hu3DMotionTimeSet(lbl_1_bss_A40->m_id, 0.0f);
        HuAudFXPlay(0x72C);
        fn_1_7F10(lbl_1_bss_A40, "hazusuB");
        fn_1_7FEC(lbl_1_bss_A40, "dasu");
        Hu3DMotionForceSet(lbl_1_bss_A40->m_id, (char *)"daizaB", 0x100, 0.0f);
    }
    fn_1_179E8(self->player[side][0]);
    fn_1_179E8(self->player[side][1]);
}

void fn_1_12640(GameMaster *self, s32 *unused)
{
    s32 tries;
    s32 hour;
    s32 minute;
    s32 i;
    u8 done;
    s32 mask;
    s16 modelId;

    fn_1_42A0(lbl_1_bss_A50);
    fn_1_42A0(lbl_1_bss_A54);
    tries = 0;
    do {
        hour = fn_1_9548() % 12;
        minute = fn_1_9548() % 60;
        fn_1_894(lbl_1_bss_A58, hour, minute);
        fn_1_894(lbl_1_bss_A50, hour, minute);
        fn_1_894(lbl_1_bss_A54, hour, minute);
        tries++;
    } while ((tries <= 0x3E8 || fn_1_2998(lbl_1_bss_A58, 60.0f) != 0)
             && (fn_1_2998(lbl_1_bss_A58, 60.0f) != 0 || fn_1_2C98(lbl_1_bss_A50, 20.0f) != 0
                 || fn_1_2C98(lbl_1_bss_A54, 20.0f) != 0));
    fn_1_924(lbl_1_bss_A58);
    HuAudFXPlay(0x732);
    fn_1_7A0(lbl_1_bss_A58);
    while (fn_1_7E8(lbl_1_bss_A58) == 0) {
        fn_1_9E90(&self->mapNode, 0);
    }
    HuAudFXPlay(0x733);
    fn_1_A62C(&self->mapList, 0x384, 0, 0);
    fn_1_A724(&self->mapList, 1);
    fn_1_42B4(lbl_1_bss_A50);
    fn_1_42B4(lbl_1_bss_A54);
    lbl_1_bss_A5C = 1;
    do {
        if (fn_1_A880(&self->mapList) != 0) {
            done = 1;
        } else if (fn_1_0(lbl_1_bss_A50) != 0 || fn_1_0(lbl_1_bss_A54) != 0) {
            fn_1_A7E8(&self->mapList);
            mask = 0;
            if (fn_1_0(lbl_1_bss_A50) != 0) {
                mask = 1;
            }
            if (fn_1_0(lbl_1_bss_A54) != 0) {
                mask |= 2;
            }
            if (mask == 3) {
                if (lbl_1_bss_A6C == 0) {
                    lbl_1_bss_A68 = 0;
                    lbl_1_bss_A6C = 1;
                }
                lbl_1_bss_A68 ^= 1;
                mask = lbl_1_bss_A68 + 1;
            }
            if ((mask & 1) != 0) {
                self->score[0]++;
            }
            if ((mask & 2) != 0) {
                self->score[1]++;
            }
            if (lbl_1_bss_A74 == 0) {
                fn_1_4C3C("ms_pointer != 0", "singleton.h", 0x27);
            }
            fn_1_1844C(lbl_1_bss_A74, 0, self->score[0]);
            fn_1_1844C(lbl_1_bss_A74, 1, self->score[1]);
            self->lastResult = mask;
            lbl_1_bss_A5C = 0;
            if (lbl_1_bss_A44 < 4) {
                while (self->gadget[0].active != 0 || self->gadget[1].active != 0) {
                    fn_1_9E90(&self->mapNode, 0);
                }
                if ((mask & 4) != 0) {
                    self->gadget[0].sfx = HuAudFXPlay(0x72F);
                    self->gadget[0].sfxTimer = 0x14A;
                }
                for (i = 0; i < 2; i++) {
                    if (self->score[i] < 3) {
                        if ((mask & (1 << i)) != 0) {
                            modelId = self->gadget[i].model->m_id;
                            Hu3DMotionTimeSet(modelId, 0.0f);
                            Hu3DMotionSpeedSet(modelId, 1.2727273f);
                            HuAudFXPlay(i == 0 ? 0x72B : 0x72C);
                            if ((mask & 4) == 0) {
                                self->gadget[i].sfx = HuAudFXPlay(i == 0 ? 0x730 : 0x731);
                                self->gadget[i].sfxTimer = 0x14A;
                                self->gadget[i].active = 1;
                                fn_1_17788(self->player[i][0]);
                                fn_1_17788(self->player[i][1]);
                            }
                        } else {
                            fn_1_17794(self->player[i][0]);
                            fn_1_17794(self->player[i][1]);
                        }
                    }
                }
            }
            done = 1;
        } else {
            done = 0;
        }
        if (done == 0) {
            fn_1_9E90(&self->mapNode, 0);
        }
    } while (done == 0);
    lbl_1_bss_A5C = 0;
    fn_1_42A0(lbl_1_bss_A50);
    fn_1_42A0(lbl_1_bss_A54);
    fn_1_A818(&self->mapList);
    if (lbl_1_bss_A44 < 4 && self->score[0] < 3 && self->score[1] < 3) {
        for (i = 0; i < 0xB4; i++) {
            fn_1_9E90(&self->mapNode, 0);
        }
    }
}

void fn_1_12CA8(GameMaster *self)
{
    s32 fxHandle;
    s32 streamHandle;
    s32 i;
    s32 j;
    s32 winner;
    s32 rounds;
    s32 jingle;
    s16 modelId;
    CListNode *list;
    CListNode *node;
    M521Ptmf pmf;

    fxHandle = HuAudFXPlay(0x72A);
    streamHandle = HuAudSStreamPlay(0x51);
    self->gadget[0].sfx = HuAudFXPlay(0x72F);
    self->gadget[0].sfxTimer = 0x14A;
    for (i = 0; i < 2; i++) {
        if (self->score[i] < 3) {
            if (((1 << i) & 7) != 0) {
                modelId = self->gadget[i].model->m_id;
                Hu3DMotionTimeSet(modelId, 0.0f);
                Hu3DMotionSpeedSet(modelId, 1.2727273f);
                HuAudFXPlay(i == 0 ? 0x72B : 0x72C);
            } else {
                fn_1_17794(self->player[i][0]);
                fn_1_17794(self->player[i][1]);
            }
        }
    }
    fn_1_66B0(0);
    fn_1_7654();
    while (fn_1_6748() == 0) {
        fn_1_9E90(&self->mapNode, 0);
    }
    fn_1_66B0(1);
    fn_1_716C();
    while (fn_1_72DC() == 0) {
        fn_1_9E90(&self->mapNode, 0);
    }
    fn_1_66B0(2);
    for (rounds = 0; rounds < 5;) {
        lbl_1_bss_A44 = rounds;
        lbl_1_bss_A48 = fn_1_9548();
        fn_1_12640(self, &lbl_1_bss_A48);
        if (self->score[0] < 3 && self->score[1] < 3) {
            rounds++;
        } else {
            break;
        }
    }
    fn_1_A818(&self->mapList);
    HuAudFXStop(fxHandle);
    fn_1_66B0(3);
    winner = -1;
    if (self->score[0] != self->score[1]) {
        if (self->score[0] > self->score[1]) {
            winner = 0;
        } else {
            winner = 1;
        }
    }
    lbl_1_bss_A4C = (u32)winner >> 31;
    HuAudSStreamFadeOut(streamHandle, 0x64);
    fn_1_7224();
    while (fn_1_72DC() == 0) {
        fn_1_9E90(&self->mapNode, 0);
    }
    if (lbl_1_bss_A4C == 0) {
        WipeCreate(2, 6, 0x3C);
        for (i = 0; i < 0x3C; i++) {
            fn_1_9E90(&self->mapNode, 0);
        }
        if (lbl_1_bss_A7C == 0) {
            fn_1_4C3C("ms_pointer != 0", "singleton.h", 0x27);
        }
        fn_1_145F8(lbl_1_bss_A7C, winner);
    }
    if (lbl_1_bss_A70 == 0) {
        fn_1_4C3C("sm_ptr != 0", "a_set.h", 0x24);
    }
    list = lbl_1_bss_A70;
    pmf = lbl_1_data_520;
    node = list->m_next;
    while (node != list) {
        M521PtmfCall(node, &pmf);
        node = node->m_next;
    }
    fn_1_66B0(4);
    if (lbl_1_bss_A4C == 0) {
        WipeCreate(1, 6, 0x3C);
        for (i = 0; i < 0x3C; i++) {
            fn_1_9E90(&self->mapNode, 0);
        }
        if (winner == 0) {
            Hu3DMotionTimeSet(lbl_1_bss_A30->m_id, 0.0f);
            HuAudFXPlay(0x72B);
            fn_1_7F10(lbl_1_bss_A30, "hazusuR");
            fn_1_7FEC(lbl_1_bss_A30, "dasu");
            Hu3DMotionForceSet(lbl_1_bss_A30->m_id, (char *)"daizaR", 0x100, 0.0f);
        }
        if (winner == 1) {
            Hu3DMotionTimeSet(lbl_1_bss_A40->m_id, 0.0f);
            HuAudFXPlay(0x72C);
            fn_1_7F10(lbl_1_bss_A40, "hazusuB");
            fn_1_7FEC(lbl_1_bss_A40, "dasu");
            Hu3DMotionForceSet(lbl_1_bss_A40->m_id, (char *)"daizaB", 0x100, 0.0f);
        }
        fn_1_179E8(self->player[winner][0]);
        fn_1_179E8(self->player[winner][1]);
        for (i = 0; (f32)(f64)i < 39.0f; i++) {
            fn_1_9E90(&self->mapNode, 0);
        }
        for (j = 0; j < 2; j++) {
            fn_1_6FC4(self->playerNo[winner][j]);
            fn_1_17884(self->player[winner][j]);
            fn_1_17920(self->player[winner ^ 1][j]);
        }
    } else {
        pmf = lbl_1_data_52C;
        node = list->m_next;
        while (node != list) {
            M521PtmfCall(node, &pmf);
            node = node->m_next;
        }
    }
    if (lbl_1_bss_A4C != 0) {
        jingle = 0x7B;
    } else {
        jingle = 0x79;
    }
    HuAudJinglePlay((s16)jingle);
    fn_1_73B0();
    for (i = 0; (f32)(f64)i < 210.0f; i++) {
        fn_1_9E90(&self->mapNode, 0);
    }
    fn_1_76AC();
    while (1) {
        fn_1_9E90(&self->mapNode, -1);
    }
}

void fn_1_13488(void)
{
    fn_1_6848();
}

void fn_1_134A8(GameNode *self, GameNodeCb cb)
{
    GameNode *node1;
    GameNode *node2;
    GameNode *endNode;
    GameNode *lastNode;
    GameNode *tmp0;
    GameNode *tmp1;
    GameNode *tmp2;
    GameNode *last;
    GameNode *tmp3;
    GameNode *tmp4;
    GameNode *tmp5;

    tmp0 = self->next;
    tmp1 = tmp0;
    node1 = tmp1;
    tmp2 = self;
    endNode = tmp2;
    last = endNode->last;
    lastNode = last;
    while (node1 != lastNode) {
        tmp3 = node1->next;
        node2 = tmp3;
        while (node2 != endNode) {
            cb(node1, node2);
            tmp4 = node2->next;
            node2 = tmp4;
        }
        tmp5 = node1->next;
        node1 = tmp5;
    }
}

// Quick sort on the u32 sort keys, with a pseudo random pivot offset (lbl_1_bss_A80 cycles -4..4).
void fn_1_1353C(u32 *first, u32 *last)
{
    s32 n;
    u32 *a;
    u32 *b;
    u32 *back;
    u32 *mid;
    u32 tmp;
    SortPivot cmp1;
    SortPivot cmp2;

    while (1) {
        n = last - first;
        if (n > 1) {
            if (n <= 0x14) {
                fn_1_13F6C(first, last);
                return;
            }
            if (lbl_1_bss_A84 == 0) {
                lbl_1_bss_A80 = -4;
                lbl_1_bss_A84 = 1;
            }
            a = &first[n / 4 + (lbl_1_bss_A80 % 5)];
            lbl_1_bss_A80++;
            if (lbl_1_bss_A80 >= 5) {
                lbl_1_bss_A80 = -4;
            }
            b = &first[(n * 3) / 4 + (lbl_1_bss_A80 % 5)];
            lbl_1_bss_A80++;
            if (lbl_1_bss_A80 >= 5) {
                lbl_1_bss_A80 = -4;
            }
            back = last - 1;
            fn_1_137A0(a, b, back);
            cmp1.pivot = back;
            mid = fn_1_13898(first, back, &cmp1);
            if (mid == first) {
                tmp = *mid;
                *mid = *back;
                *back = tmp;
                cmp2.pivot = first;
                first = fn_1_13BA4(mid + 1, last, &cmp2);
            } else if (mid - first < last - mid) {
                fn_1_1353C(first, mid);
                first = mid;
            } else {
                fn_1_1353C(mid, last);
                last = mid;
            }
        } else {
            return;
        }
    }
}

// Orders the three samples so the median ends up in *c.
void fn_1_137A0(u32 *a, u32 *b, u32 *c)
{
    u8 aLeC;
    u8 cLeB;
    u32 tmp;

    aLeC = *a <= *c;
    cLeB = *c <= *b;
    if (aLeC == 0 || cLeB == 0) {
        if (aLeC == 0 && cLeB == 0) {
            tmp = *a;
            *a = *b;
            *b = tmp;
            return;
        }
        if (*b < *a) {
            tmp = *a;
            *a = *b;
            *b = tmp;
        }
        if (aLeC != 0) {
            tmp = *b;
            *b = *c;
            *c = tmp;
            return;
        }
        tmp = *a;
        *a = *c;
        *c = tmp;
    }
}

// Partition on "*x < pivot".
u32 *fn_1_13898(u32 *first, u32 *last, SortPivot *cmp)
{
    u32 *l;
    u32 *r;
    u32 *back;
    u32 *end;
    u8 firstLess;
    u8 backGe;
    u32 tmp;

    if (first == last) {
        return first;
    }
    firstLess = *first < *cmp->pivot;
    back = last - 1;
    backGe = !(*back < *cmp->pivot);
    if (firstLess != 0 && backGe != 0) {
        r = back;
        l = first + 1;
        while (1) {
            while (*l < *cmp->pivot) {
                l++;
            }
            do {
                r--;
            } while (!(*r < *cmp->pivot));
            if (l < r) {
                tmp = *l;
                *l = *r;
                *r = tmp;
                l++;
                continue;
            }
            return l;
        }
    }
    if (backGe != 0) {
        if (first == back) {
            return first;
        }
        do {
            back--;
        } while (first != back && !(*back < *cmp->pivot));
        if (first == back) {
            return first;
        }
    } else if (firstLess != 0) {
        end = back + 1;
        first++;
        while (first != end && *first < *cmp->pivot) {
            first++;
        }
        if (first == end) {
            return first;
        }
        back = end - 1;
    }
    tmp = *first;
    *first = *back;
    *back = tmp;
    r = back;
    l = first + 1;
    while (1) {
        while (*l < *cmp->pivot) {
            l++;
        }
        do {
            r--;
        } while (!(*r < *cmp->pivot));
        if (l < r) {
            tmp = *l;
            *l = *r;
            *r = tmp;
            l++;
            continue;
        }
        return l;
    }
}

// Partition on "*x <= pivot".
u32 *fn_1_13BA4(u32 *first, u32 *last, SortPivot *cmp)
{
    u32 *l;
    u32 *r;
    u32 *back;
    u32 *end;
    u8 firstLe;
    u8 backGt;
    u32 tmp;

    if (first == last) {
        return first;
    }
    firstLe = !(*cmp->pivot < *first);
    back = last - 1;
    backGt = *cmp->pivot < *back;
    if (firstLe != 0 && backGt != 0) {
        r = back;
        l = first + 1;
        while (1) {
            while (!(*cmp->pivot < *l)) {
                l++;
            }
            do {
                r--;
            } while (*cmp->pivot < *r);
            if (l < r) {
                tmp = *l;
                *l = *r;
                *r = tmp;
                l++;
                continue;
            }
            return l;
        }
    }
    if (backGt != 0) {
        if (first == back) {
            return first;
        }
        do {
            back--;
        } while (first != back && *cmp->pivot < *back);
        if (first == back) {
            return first;
        }
    } else if (firstLe != 0) {
        end = back + 1;
        first++;
        while (first != end && !(*cmp->pivot < *first)) {
            first++;
        }
        if (first == end) {
            return first;
        }
        back = end - 1;
    }
    tmp = *first;
    *first = *back;
    *back = tmp;
    r = back;
    l = first + 1;
    while (1) {
        while (!(*cmp->pivot < *l)) {
            l++;
        }
        do {
            r--;
        } while (*cmp->pivot < *r);
        if (l < r) {
            tmp = *l;
            *l = *r;
            *r = tmp;
            l++;
            continue;
        }
        return l;
    }
}

// Selection sort for the short ranges.
void fn_1_13F6C(u32 *first, u32 *last)
{
    u32 *i;
    u32 *j;
    u32 *min;
    u32 tmp;

    i = first;
    if (i != last) {
        while (i < last - 1) {
            min = i;
            if (i != last) {
                j = i + 1;
                while (j != last) {
                    if (*j < *min) {
                        min = j;
                    }
                    j++;
                }
            }
            if (min != i) {
                tmp = *min;
                *min = *i;
                *i = tmp;
            }
            i++;
        }
    }
}

void fn_1_1400C(void)
{
    lbl_1_bss_A18 = 0x78;
    lbl_1_bss_A1C = 0xD2;
    lbl_1_bss_A20 = 0xD2;
    lbl_1_bss_A30 = 0;
    __register_global_object(&lbl_1_bss_A30, (void *)fn_1_4C8, lbl_1_bss_A24);
    lbl_1_bss_A40 = 0;
    __register_global_object(&lbl_1_bss_A40, (void *)fn_1_4C8, lbl_1_bss_A34);
}

void fn_1_140C8(CSubThread *node)
{
    fn_1_12CA8((GameMaster *)((u8 *)node - offsetof(GameMaster, mapNode)));
}

void fn_1_140D0(CSubThread *node)
{
    fn_1_11B08((GameMaster *)((u8 *)node - offsetof(GameMaster, mapNode)), 0);
}
}
