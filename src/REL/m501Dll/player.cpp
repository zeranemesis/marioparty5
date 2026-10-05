#include "REL/m501Dll.h"

struct Vec {
    f32 x, y, z;
};

class CLinkList {
public:
    CLinkNode *m_head;
    CLinkNode *m_tail;
    CLinkList() : m_head(0), m_tail(0) {}
};

namespace m501 {

class CPlayer : public CGameObject {
public:
    CPlayer(int playerIdx);
    virtual ~CPlayer();

    static s32 ComparePlayer(CLinkNode *a, CLinkNode *b);

    void proc_main();
    void proc_shrink_cone();
    void fn_1_B920();
    void fn_1_10158();
    static void execPlayerList(CLinkList *list, void (*func)(CPlayer *));
    static void execPlayerList(CPlayer *first, CLinkList *list, void (*func)(CPlayer *));

    void getMoveDir(Vec *dir);
    void movePlayer(Vec *dir);

private:
    char pad_10[0x14]; // 0x10-0x23
    s32 m_compareValue; // 0x24
    char pad_28[0x1C]; // 0x28-0x43
    f32 m_coneScale; // 0x44
};

#pragma global_optimizer on
#pragma optimization_level 4
#pragma function_align 16

// NONMATCHING: register allocation differs (r6/r5 vs r3/r0), logic identical
s32 CPlayer::ComparePlayer(CLinkNode *a, CLinkNode *b)
{
    CPlayer *playerA = (CPlayer *)a;
    CPlayer *playerB = (CPlayer *)b;
    s32 valA = playerA->m_compareValue;
    s32 valB = playerB->m_compareValue;

    s32 xorVal = valA ^ valB;
    s32 shrVal = xorVal >> 1;
    s32 andVal = xorVal & valA;
    s32 subVal = shrVal - andVal;

    return (s32)((u32)subVal >> 31);
}

#pragma global_optimizer reset
#pragma optimization_level reset
#pragma function_align reset

extern f32 lbl_1_bss_FC;

void CPlayer::proc_main()
{
    Vec dir;
    getMoveDir(&dir);
    movePlayer(&dir);
}

void CPlayer::proc_shrink_cone()
{
    m_coneScale -= lbl_1_bss_FC;
    if (m_coneScale <= 0.0f) {
        m_coneScale = 0.0f;
    }
}

extern "C" void __dt__9CLinkNodeFv(void *self, s32 flag);
extern "C" void __dl__FPv(void *ptr);

extern "C" CPlayer *fn_1_B920(CPlayer *player, s16 flag)
{
    if (player != 0) {
        __dt__9CLinkNodeFv(player, 0);
        if (flag > 0) {
            __dl__FPv(player);
        }
    }
    return player;
}

extern "C" CPlayer *fn_1_10158(CPlayer *player, s16 flag)
{
    if (player != 0) {
        __dt__9CLinkNodeFv(player, 0);
        if (flag > 0) {
            __dl__FPv(player);
        }
    }
    return player;
}

} // namespace m501

namespace m501 {

void CPlayer::execPlayerList(CLinkList *list, void (*func)(CPlayer *))
{
    CPlayer *first = (CPlayer *)list->m_head;
    CLinkList *listPtr = list;
    execPlayerList(first, listPtr, func);
}

void CPlayer::execPlayerList(CPlayer *first, CLinkList *list, void (*func)(CPlayer *))
{
    // Stub - will be implemented later
    (void)first;
    (void)list;
    (void)func;
}

} // namespace m501