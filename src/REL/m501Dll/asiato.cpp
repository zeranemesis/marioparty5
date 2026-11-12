#include "REL/m501Dll.h"
#include "game/hu3d.h"
#include <dolphin/mtx.h>

extern "C" void __dl__FPv(void *ptr);

namespace m501 {

/* file-scope ints the original sets up in its static initialiser */
class CInt {
public:
    CInt(int v) : m_value(v) {}
    s32 m_value;
};

#ifdef TARGET_PC
static CInt s_unused30(120);
static CInt s_unused34(210);
static CInt s_unused38(210);
static CLinkList s_asiatoList;
static CInt s_asiatoLife(72);
static s32 s_unused54;
#else
/* the GC build runs the initialiser by hand: fn_1_6548 below is the .ctors entry */
static s32 s_unused30;
static s32 s_unused34;
static s32 s_unused38;
static u8 s_asiatoListChain[12];
static u8 s_asiatoListMem[8];
static s32 s_asiatoLifeValue;
static s32 s_unused54;
#define s_asiatoList (*(CLinkList *)s_asiatoListMem)
struct CLifeRef {
    s32 m_value;
};
#define s_asiatoLife (*(CLifeRef *)&s_asiatoLifeValue)
#endif

static char s_asiatoName[] = "asiato";

extern "C" void init__Q24m5017CAsiatoManFv(void)
{
    HU3DMODELID id = Hu3DHookFuncCreate(CAsiato::drawHook);
    Hu3DModelLayerSet(id, 0);
    Hu3DModelCameraSet(id, 1);
}

extern "C" void update__Q24m5017CAsiatoManFv(void)
{
    CAsiato::execAsiatoList(&s_asiatoList, CAsiato::update);
}

extern "C" void close__Q24m5017CAsiatoManFv(void)
{
    while (!s_asiatoList.isEmpty()) {
        CLinkNode *node = s_asiatoList.m_next;
        if (node != 0) {
            if (node != 0) {
                node->~CLinkNode();
            }
            __dl__FPv(node);
        }
    }
}

CAsiato::CAsiato(Vec *pos, f32 rot)
{
    m_pos = *pos;
    m_rot = rot;
    m_life = s_asiatoLife.m_value;
    insertBack(&s_asiatoList);
}

CAsiato::~CAsiato()
{
}

void CAsiato::update(CAsiato *asiato)
{
    asiato->m_life = asiato->m_life - 1;
    if (asiato->m_life < 0 && asiato != 0) {
        if (asiato != 0) {
            ((CLinkNode *)asiato)->~CLinkNode();
        }
        __dl__FPv(asiato);
    }
}

void CAsiato::drawHook(HU3DMODEL *model, Mtx *mtx)
{
    CAsiatoDrawData data;

    Hu3DModelObjDrawInit();
    PSMTXCopy(*mtx, data.m_mtx);
    CModel *inst = CParticle::getInstance();
    s16 id = inst->getId();
    s16 id2 = id;
    data.m_model = id2;
    drawAsiatoList(&s_asiatoList, &data);
}

void CAsiato::drawAsiato(CAsiato *asiato, CAsiatoDrawData *data)
{
    Mtx mtx;
    f32 scale;

    PSMTXRotRad(mtx, 'Y', 0.017453292f * asiato->m_rot);
    scale = (f32)asiato->m_life / (f32)s_asiatoLife.m_value;
    PSMTXScaleApply(mtx, mtx, 60.0f * scale, 1.0f, 60.0f * scale);
    PSMTXTransApply(mtx, mtx, asiato->m_pos.x, 5068.02f, asiato->m_pos.z);
    PSMTXConcat(data->m_mtx, mtx, mtx);
    Hu3DModelObjDraw(data->m_model, s_asiatoName, mtx);
}

void CAsiato::execAsiatoList(CLinkList *list, void (*func)(CAsiato *))
{
    CLinkList *l = list;
    CAsiato *first = (CAsiato *)l->m_prev;
    CAsiato *n = first;
    execAsiatoList(n, (CAsiato *)l, func);
}

void CAsiato::drawAsiatoList(CLinkList *list, CAsiatoDrawData *data)
{
    CLinkList *l = list;
    CAsiato *first = (CAsiato *)l->m_prev;
    CAsiato *n = first;
    n->drawAsiatoList((CAsiato *)l, data);
}

void CAsiato::drawAsiatoList(CAsiato *end, CAsiatoDrawData *data)
{
    CAsiato *node = this;
    while (node != end) {
        Mtx mtx;
        f32 scale;

        PSMTXRotRad(mtx, 'Y', 0.017453292f * node->m_rot);
        scale = (f32)node->m_life / (f32)s_asiatoLife.m_value;
        PSMTXScaleApply(mtx, mtx, 60.0f * scale, 1.0f, 60.0f * scale);
        PSMTXTransApply(mtx, mtx, node->m_pos.x, 5068.02f, node->m_pos.z);
        PSMTXConcat(data->m_mtx, mtx, mtx);
        Hu3DModelObjDraw(data->m_model, s_asiatoName, mtx);
        node = (CAsiato *)node->m_prev;
    }
}

void CAsiato::execAsiatoList(CAsiato *node, CAsiato *end, void (*func)(CAsiato *))
{
    while (node != end) {
        func(node);
        node = (CAsiato *)node->m_prev;
    }
}

#ifndef TARGET_PC
extern "C" void __register_global_object(void *obj, void *(*dtor)(void *, s16), void *reg);
extern "C" void __ct__9CLinkNodeFv(void *self);
extern "C" void *fn_1_65E0(void *self, s16 flag);

extern "C" void fn_1_6548(void)
{
    s_unused30 = 0x78;
    s_unused34 = 0xD2;
    s_unused38 = 0xD2;
    __ct__9CLinkNodeFv(s_asiatoListMem);
    __register_global_object(s_asiatoListMem, fn_1_65E0, s_asiatoListChain);
    s_asiatoLife.m_value = 0x48;
}

extern "C" void *fn_1_65E0(void *self, s16 flag)
{
    if (self != 0) {
        ((CLinkNode *)self)->~CLinkNode();
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}
#endif

} // namespace m501
