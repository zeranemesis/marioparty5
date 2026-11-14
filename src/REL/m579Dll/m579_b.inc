/* m579 library part b: CLinkNode/CLinkList, CGameObject, CMinigameCore (0x1264..0x2B14).
 * List links: offset 0 = node.next, offset 4 = node.prev (the m_prev/m_next of the m501 sources). */
#include "game/mg/actman.h"

/* CMinigameCore: the vtable pointer sits at +4 in the MWCC layout */
typedef struct M579MgCore {
    u32 unk0;
    void **vtbl;
    s32 winner[4];
    s32 gameMes;
    s32 delay;
} M579MgCore;

typedef u8 (*M579CmpFunc)(M579Node *, M579Node *);

extern "C" {

void fn_1_1D4(const char *expr, const char *file, s32 line);
void fn_1_2C18(void);
void *fn_1_47E4(void *self);

void *fn_1_1EA8(void *self, s16 flag);
void *fn_1_20CC(void *self, s16 flag);
void fn_1_1DDC(void *self);
void fn_1_246C(void *self);
void fn_1_2370(OMOBJ *obj);
void fn_1_2400(void *self);
void fn_1_2470(OMOBJ *obj);
M579Node *fn_1_14A0(M579CmpFunc cmp, M579Node *list);
M579Node *fn_1_1B48(M579Node *list);
M579Node *fn_1_1B9C(M579CmpFunc cmp, M579Node *a, M579Node *b);

char lbl_1_data_76[] = "singleton.h";
char lbl_1_data_82[] = "ms_pointer == 0";
char lbl_1_data_92[] = "ms_pointer != 0";

void *lbl_1_data_1E4[] = { 0, 0, (void *) fn_1_20CC, 0, (void *) fn_1_246C, 0 };
void *lbl_1_data_1FC[] = { 0, 0, (void *) fn_1_1EA8, 0, (void *) fn_1_1DDC };

M579Node lbl_1_bss_C;
s32 lbl_1_bss_14;
s32 lbl_1_bss_1C;
M579MgCore *lbl_1_bss_9EC;

#define M579_SINGLETON_ASSERT(cond, expr, line) \
    if (!(cond)) { \
        fn_1_1D4(expr, lbl_1_data_76, line); \
    }

/* CLinkNode::CLinkNode() */
void fn_1_1264(void *self)
{
    M579Node *n = (M579Node *) self;
    n->next = n;
    n->prev = n;
}

/* CLinkNode::~CLinkNode(), with delete flag */
void fn_1_1270(void *self, s32 arg)
{
    M579Node *n = (M579Node *) self;
    if (n != NULL) {
        n->next->prev = n->prev;
        n->prev->next = n->next;
        if ((s16) arg > 0 && n != NULL) {
            HuMemDirectFree(n);
        }
    }
}

/* CLinkNode::CLinkNode(CLinkNode &) */
void fn_1_12DC(void *self)
{
    M579Node *n = (M579Node *) self;
    n->next->prev = n->prev;
    n->prev->next = n->next;
    n->next = n;
    n->prev = n;
}

/* CLinkNode::insertFront */
void fn_1_1300(void *self, void *list)
{
    M579Node *n = (M579Node *) self;
    M579Node *l = (M579Node *) list;
    n->prev = l;
    n->next = l->next;
    l->next->prev = n;
    l->next = n;
}

/* CLinkNode::insertBack */
void fn_1_1330(void *self, void *list)
{
    M579Node *n = (M579Node *) self;
    M579Node *l = (M579Node *) list;
    n->next = l;
    n->prev = l->prev;
    l->prev->next = n;
    l->prev = n;
}

/* CLinkNode::copyLink: moves the whole ring of src in front of dst */
void fn_1_1360(void *list, void *dst)
{
    M579Node *src = (M579Node *) list;
    M579Node *d = (M579Node *) dst;
    u8 empty = 0;
    if (src->next == src && src->next == src->prev) {
        empty = 1;
    }
    if (!empty) {
        M579Node *first = src->next;
        M579Node *last = src->prev;
        src->next = src;
        src->prev = src;
        first->prev = d->prev;
        last->next = d;
        d->prev->next = first;
        d->prev = last;
    }
}

/* CLinkList::sortList(cmp): merge sort over the forward links, then rebuild the back links */
void fn_1_13E8(void *list, M579CmpFunc cmp)
{
    M579Node *l = (M579Node *) list;
    u8 empty = 0;
    if (l->next == l && l->next == l->prev) {
        empty = 1;
    }
    if (!empty) {
        M579Node *node;
        M579Node *prev;
        l->prev->next = NULL;
        node = fn_1_14A0(cmp, l->next);
        l->next = node;
        prev = l;
        while (node != NULL) {
            node->prev = prev;
            prev = node;
            node = node->next;
        }
        l->prev = prev;
        prev->next = l;
    }
}

/* recursive merge sort of a NULL terminated singly linked list */
M579Node *fn_1_14A0(M579CmpFunc cmp, M579Node *list)
{
    M579Node *half;
    M579Node *a;
    M579Node *b;
    if (list == NULL) {
        return NULL;
    }
    if (list->next == NULL) {
        return list;
    }
    half = fn_1_1B48(list);
    a = fn_1_14A0(cmp, list);
    b = fn_1_14A0(cmp, half);
    return fn_1_1B9C(cmp, a, b);
}

/* splits the list in the middle and returns the second half */
M579Node *fn_1_1B48(M579Node *list)
{
    M579Node *slow = list;
    M579Node *fast = list->next;
    M579Node *second;
    if (fast != NULL) {
        fast = fast->next;
    }
    while (fast != NULL) {
        slow = slow->next;
        fast = fast->next;
        if (fast != NULL) {
            fast = fast->next;
        }
    }
    second = slow->next;
    slow->next = NULL;
    return second;
}

/* merges two sorted lists */
M579Node *fn_1_1B9C(M579CmpFunc cmp, M579Node *a, M579Node *b)
{
    M579Node *head;
    M579Node **tail = &head;
    while (a != NULL && b != NULL) {
        if (cmp(a, b)) {
            *tail = a;
            tail = &a->next;
            a = a->next;
        } else {
            *tail = b;
            tail = &b->next;
            b = b->next;
        }
    }
    if (a == NULL) {
        *tail = b;
    } else {
        *tail = a;
    }
    return head;
}

/* CGameObject::SetObjExecMode */
void fn_1_1C48(s32 mode)
{
    M579Base *obj;
    lbl_1_bss_14 = mode;
    for (obj = (M579Base *) lbl_1_bss_C.next; obj != (M579Base *) &lbl_1_bss_C; obj = (M579Base *) obj->node.next) {
        obj->flags |= 1 << mode;
        ((void (*)(void *, s32)) obj->vtbl[3])(obj, mode);
    }
}

/* CGameObject::CheckObjExec */
u32 fn_1_1CE0(void)
{
    M579Base *obj;
    for (obj = (M579Base *) lbl_1_bss_C.next; obj != (M579Base *) &lbl_1_bss_C; obj = (M579Base *) obj->node.next) {
        if ((obj->flags & (1 << lbl_1_bss_14)) != 0) {
            return 0;
        }
    }
    return 1;
}

/* CGameObject::UpdateAll */
void fn_1_1D6C(void)
{
    M579Base *obj;
    for (obj = (M579Base *) lbl_1_bss_C.next; obj != (M579Base *) &lbl_1_bss_C; obj = (M579Base *) obj->node.next) {
        ((void (*)(void *)) obj->vtbl[4])(obj);
    }
}

/* CGameObject::on_update */
void fn_1_1DDC(void *self)
{
}

/* CGameObject::Close: deletes every object through its virtual destructor */
void fn_1_1DE0(void)
{
    M579Base *obj;
    u8 empty;
    do {
        obj = (M579Base *) lbl_1_bss_C.prev;
        if (obj != NULL) {
            ((void (*)(void *, s32)) obj->vtbl[2])(obj, 1);
        }
        empty = 0;
        if (lbl_1_bss_C.next == &lbl_1_bss_C && lbl_1_bss_C.next == lbl_1_bss_C.prev) {
            empty = 1;
        }
    } while (!empty);
}

/* CGameObject::~CGameObject (deleting) */
void *fn_1_1EA8(void *self, s16 flag)
{
    M579Base *obj = (M579Base *) self;
    if (obj != NULL) {
        obj->vtbl = lbl_1_data_1FC;
        if (obj != NULL) {
            obj->node.prev->next = obj->node.next;
            obj->node.next->prev = obj->node.prev;
        }
        if (flag > 0 && obj != NULL) {
            HuMemDirectFree(obj);
        }
    }
    return obj;
}

/* CGameObject::GetUpdateMode */
s32 fn_1_1F30(void)
{
    return lbl_1_bss_14;
}

/* CMinigameCore::CMinigameCore */
void *fn_1_1F40(void *self)
{
    M579MgCore *core = (M579MgCore *) self;
    s32 i;
    M579_SINGLETON_ASSERT(lbl_1_bss_9EC == NULL, lbl_1_data_82, 0x35);
    lbl_1_bss_9EC = core;
    core->vtbl = lbl_1_data_1E4;
    core->delay = 0;
    for (i = 0; i < 4; i++) {
        core->winner[i] = -1;
    }
    return core;
}

/* singleton base destructor with delete flag */
void *fn_1_2010(void *self, s16 flag)
{
    if (self != NULL) {
        M579_SINGLETON_ASSERT(lbl_1_bss_9EC != NULL, lbl_1_data_92, 0x3D);
        lbl_1_bss_9EC = NULL;
        if (flag > 0 && self != NULL) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

/* CMinigameCore::~CMinigameCore (deleting) */
void *fn_1_20CC(void *self, s16 flag)
{
    M579MgCore *obj = (M579MgCore *) self;
    if (obj != NULL) {
        obj->vtbl = lbl_1_data_1E4;
        if (mgDataTbl[GwSystem.mgNo].type != 3) {
            M579MgCore *core;
            s32 i;
            M579_SINGLETON_ASSERT(lbl_1_bss_9EC != NULL, lbl_1_data_92, 0x27);
            core = lbl_1_bss_9EC;
            for (i = 0; i < 4; i++) {
                s32 player = core->winner[i];
                if (player >= 0 && _CheckFlag(0x1000F) == 0) {
                    GwPlayer[player].mgCoinBonus = 10;
                }
            }
        }
        if (obj != NULL) {
            M579_SINGLETON_ASSERT(lbl_1_bss_9EC != NULL, lbl_1_data_92, 0x3D);
            lbl_1_bss_9EC = NULL;
        }
        if (flag > 0) {
            fn_1_47E4(obj);
        }
    }
    return obj;
}

/* CMinigameCore::setup(prio, stackSize) */
void fn_1_2284(void *self, s32 prio, s32 stackSize)
{
    M579MgCore *core = (M579MgCore *) self;
    OMOBJMAN *objMan = omInitObjMan((s16) prio, stackSize);
    omGameSysInit(objMan);
    ((void (*)(void *, OMOBJMAN *)) core->vtbl[3])(core, objMan);
    omAddObjEx(objMan, 0, 0, 0, -1, fn_1_2370);
}

/* CMinigameCore::actorSetup */
void fn_1_2308(void *self)
{
    M579MgCore *core = (M579MgCore *) self;
    OMOBJMAN *objMan = MgActorObjectSetup();
    ((void (*)(void *, OMOBJMAN *)) core->vtbl[3])(core, objMan);
    omAddObjEx(objMan, 0, 0, 0, -1, fn_1_2370);
}

/* CMinigameCore::ObjUpdate */
void fn_1_2370(OMOBJ *obj)
{
    if (omSysExitReq != 0 && WipeCheckEnd() == 0) {
        fn_1_2C18();
        HuAudFadeOut(1000);
        obj->objFunc = fn_1_2470;
    } else {
        fn_1_2400(lbl_1_bss_9EC);
    }
}

/* CMinigameCore::update */
void fn_1_2400(void *self)
{
    M579MgCore *core = (M579MgCore *) self;
    ((void (*)(void *)) core->vtbl[4])(core);
    if (core->delay > 0) {
        core->delay--;
    }
}

/* CMinigameCore::on_update */
void fn_1_246C(void *self)
{
}

/* CMinigameCore::MgExit */
void fn_1_2470(OMOBJ *obj)
{
    M579MgCore *core = lbl_1_bss_9EC;
    if (WipeCheckEnd() != 0) {
        ((void (*)(void *)) core->vtbl[4])(core);
        if (core->delay > 0) {
            core->delay--;
        }
    } else {
        ((void (*)(void *)) core->vtbl[5])(core);
        if (core != NULL) {
            ((void (*)(void *, s32)) core->vtbl[2])(core, 1);
        }
        CharModelKill(-1);
        omOvlReturnEx(1, 1);
    }
}

/* CMinigameCore::AddWinner */
void fn_1_255C(s32 player)
{
    M579MgCore *core;
    s32 *winner;
    s32 i;
    M579_SINGLETON_ASSERT(lbl_1_bss_9EC != NULL, lbl_1_data_92, 0x27);
    core = lbl_1_bss_9EC;
    winner = core->winner;
    for (i = 0; i < 4; i++) {
        if (winner[i] < 0) {
            winner[i] = player;
            break;
        }
    }
}

/* CMinigameCore::CheckWinner */
s32 fn_1_2630(s32 player)
{
    M579MgCore *core;
    s32 *winner;
    s32 i;
    M579_SINGLETON_ASSERT(lbl_1_bss_9EC != NULL, lbl_1_data_92, 0x27);
    core = lbl_1_bss_9EC;
    winner = core->winner;
    for (i = 0; i < 4; i++) {
        if (player == winner[i]) {
            return 1;
        }
    }
    return 0;
}

/* CMinigameCore::StartStartGameMes */
void fn_1_2704(void)
{
    M579_SINGLETON_ASSERT(lbl_1_bss_9EC != NULL, lbl_1_data_92, 0x27);
    lbl_1_bss_9EC->gameMes = GameMesCreate(2, 0);
}

/* CMinigameCore::StartFinishGameMes */
void fn_1_27BC(void)
{
    M579_SINGLETON_ASSERT(lbl_1_bss_9EC != NULL, lbl_1_data_92, 0x27);
    lbl_1_bss_9EC->gameMes = GameMesCreate(2, 1);
}

/* CMinigameCore::CheckGameMes */
u32 fn_1_2874(void)
{
    M579MgCore *core;
    u32 done = 0;
    M579_SINGLETON_ASSERT(lbl_1_bss_9EC != NULL, lbl_1_data_92, 0x27);
    core = lbl_1_bss_9EC;
    if (GameMesStatGet(-1) == 0 && core->delay <= 0) {
        done = 1;
    }
    return done;
}

/* CMinigameCore::StartWinnerGameMes */
void fn_1_2948(void)
{
    M579MgCore *core;
    s32 *winner;
    s32 chars[4];
    s32 count;
    s32 i;
    s32 j;
    M579_SINGLETON_ASSERT(lbl_1_bss_9EC != NULL, lbl_1_data_92, 0x27);
    core = lbl_1_bss_9EC;
    core->delay = lbl_1_bss_1C;
    winner = core->winner;
    for (count = 0; count < 4; count++) {
        if (winner[count] < 0) {
            break;
        }
    }
    if (count == 0) {
        GameMesCreate(2, 2);
        return;
    }
    for (i = 1; i < count; i++) {
        for (j = i; j >= 1 && winner[j - 1] > winner[j]; j--) {
            s32 tmp = winner[j - 1];
            winner[j - 1] = winner[j];
            winner[j] = tmp;
        }
    }
    for (i = 0; i < 4; i++) {
        s16 charNo;
        if (winner[i] < 0) {
            charNo = -1;
        } else {
            charNo = GwPlayerConf[winner[i]].charNo;
        }
        chars[i] = charNo;
    }
    GameMesCreate(4, 3, chars[0], chars[1], chars[2], chars[3]);
}

/* CMinigameCore::StartRecordGameMes */
void fn_1_2B14(u32 rec)
{
    if (_CheckFlag(0x1000F) == 0) {
        M579_SINGLETON_ASSERT(lbl_1_bss_9EC != NULL, lbl_1_data_92, 0x27);
        lbl_1_bss_9EC->gameMes = (s16) GameMesCreate(14, rec);
        HuAudJinglePlay(0x86);
    }
}

}
