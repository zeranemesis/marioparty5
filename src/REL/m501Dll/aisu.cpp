#include "REL/m501Dll.h"
#include "game/hu3d.h"
extern "C" {
#include "game/audio.h"
}
#include <dolphin/mtx.h>

#ifdef TARGET_PC
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define M501_STDERR stderr
#else
extern "C" {
extern char __files[];
int fprintf(void *file, const char *fmt, ...);
void abort(void);
double sin(double x);
double cos(double x);
}
#define M501_STDERR ((void *)(__files + 0xA0))
#endif

extern "C" {
int sprintf(char *buf, const char *fmt, ...);
void __dl__FPv(void *ptr);
void *__nw__FUl(unsigned long size);
void __dt__9CLinkNodeFv(void *self, s32 flag);
void *fn_1_729C(void *self, s16 flag);
}

namespace m501 {

extern "C" void __ct__9CLinkNodeFR9CLinkNode(CLinkNode *self);

/* the "copy constructor" of CLinkNode only unlinks the node and turns it back into a ring of one */
static inline void unlinkNode(CLinkNode *node)
{
#ifdef TARGET_PC
    node->m_prev->m_next = node->m_next;
    node->m_next->m_prev = node->m_prev;
    node->m_prev = node;
    node->m_next = node;
#else
    __ct__9CLinkNodeFR9CLinkNode(node);
#endif
}

/* file-scope ints the original sets up in its static initialiser */
class CInt {
public:
    CInt(int v) : m_value(v) {}
    s32 m_value;
};

#ifdef TARGET_PC
static CInt s_unused70(120);
static CInt s_unused74(210);
static CInt s_unused78(210);
static CLinkList s_aisuConeList;
static CLinkList s_aisuAllList;
static CLinkList s_aisuFallList;
#define aisuConeList s_aisuConeList
#define aisuAllList s_aisuAllList
#define aisuFallList s_aisuFallList
#else
/* the GC build runs the initialiser by hand: fn_1_922C below is the .ctors entry */
static s32 s_unused70;
static s32 s_unused74;
static s32 s_unused78;
static u8 s_aisuConeChain[12];
extern "C" u8 aisuConeList__Q24m5015CAisu[8];
static u8 s_aisuAllChain[12];
extern "C" u8 aisuAllList__Q24m5015CAisu[8];
static u8 s_aisuFallChain[12];
extern "C" u8 aisuFallList__Q24m5015CAisu[8];
#define aisuConeList (*(CLinkList *)aisuConeList__Q24m5015CAisu)
#define aisuAllList (*(CLinkList *)aisuAllList__Q24m5015CAisu)
#define aisuFallList (*(CLinkList *)aisuFallList__Q24m5015CAisu)
#endif

CHandleMan *CAisu::aisuHandleMan = 0;
static s32 s_unused4C;

extern "C" {
void fn_1_7988(CAisu *aisu);
void fn_1_79FC(CAisu *aisu);
void fn_1_7BF0(CAisu *aisu);
void fn_1_7D94(CAisu *aisu);
void fn_1_7E3C(CAisu *aisu);
u8 fn_1_8500(CAisu *aisu);
}

static const char *s_aisuNames[] = { "str", "mav", "mint", "choco", "melon", "lemon", "ore", "bani" };
static const char *s_aisuSuffix[] = { "_A", "_B", "_C" };

/* state procs: non-virtual member-function pointers, built by hand */
static CAisuProc s_procWait = { 0, -1, fn_1_7988 };
static CAisuProc s_procFall = { 0, -1, fn_1_79FC };
static CAisuProc s_procShrink = { 0, -1, fn_1_7BF0 };
static CAisuProc s_procGone = { 0, -1, fn_1_7D94 };
static CAisuProc s_procHeld = { 0, -1, fn_1_7E3C };

void CAisu::init(CHandleMan *handleMan)
{
    aisuHandleMan = handleMan;
    HU3DMODELID hookMdl = Hu3DHookFuncCreate((HU3DMODELHOOK)CAisu::drawHook);
    Hu3DModelLayerSet(hookMdl, 0);
    Hu3DModelCameraSet(hookMdl, 1);
}

void *CAisu::getHandle(int idx)
{
    return aisuHandleMan->getHandlePtr(idx);
}

void CAisu::Close()
{
    while (!aisuConeList.isEmpty()) {
        CAisu *aisu = (CAisu *)aisuConeList.m_next;
        delete aisu;
    }
    while (!aisuAllList.isEmpty()) {
        CAisu *aisu = (CAisu *)aisuAllList.m_next;
        delete aisu;
    }
    while (!aisuFallList.isEmpty()) {
        CAisu *aisu = (CAisu *)aisuFallList.m_next;
        delete aisu;
    }
}

CAisu::CAisu(int type, Vec *pos)
{
    m_type = type;
    m_pos = *pos;
    insertBack(&aisuAllList);
    m_proc = s_procWait;
    m_vel.x = 0.0f;
    m_vel.y = 0.0f;
    m_vel.z = 0.0f;
    m_scale = 1.0f;
    setObjName(0);
    m_handle = aisuHandleMan->insertHandle(this);
    m_sound = HuAudFXPlay(0x624);
}

extern "C" CAisu *fn_1_7708(CAisu *this_, s16 flag)
{
    if (this_ != 0) {
        __dt__9CLinkNodeFv(this_, 0);
        if (flag > 0) {
            __dl__FPv(this_);
        }
    }
    return this_;
}

CAisu::~CAisu()
{
    aisuHandleMan->removeHandle(m_handle);
}

void CAisu::UpdateAll()
{
    execAisuList(&aisuAllList, AisuUpdate);
    execAisuList(&aisuFallList, AisuUpdate);
    execAisuList(&aisuConeList, AisuUpdate);
}

void CAisu::AisuUpdate(CAisu *aisu)
{
#ifdef TARGET_PC
    aisu->m_proc.m_fn(aisu);
#else
    typedef void (CAisu::*PROC)();
    (aisu->*(*(PROC *)&aisu->m_proc))();
#endif
    if (fn_1_8500(aisu) == 0) {
        CJoint *joint = &aisu->m_joint;
        joint->m_locked = 1;
        PSMTXTrans(joint->m_world, aisu->m_pos.x, aisu->m_pos.y, aisu->m_pos.z);
        f32 scale = aisu->m_scale;
        joint->m_world[1][1] = scale;
        f32 s2 = scale + 2.0f * (1.0f - scale);
        joint->m_world[0][0] = s2;
        joint->m_world[2][2] = s2;
    }
}

void CAisu::setObjName(int variant)
{
    sprintf(m_name, "%s%s", s_aisuNames[m_type], s_aisuSuffix[variant]);
}

/* waiting in the air: show the shadow and start to fall */
extern "C" void fn_1_7988(CAisu *aisu)
{
    aisu->m_shadow = 1;
    aisu->m_taken = 0;
    aisu->m_proc = s_procFall;
    aisu->m_shadowScale = 0.0f;
}

/* falling */
extern "C" void fn_1_79FC(CAisu *aisu)
{
    Vec step;

    step = aisu->m_vel;
    aisu->m_vel.y = aisu->m_vel.y - 0.27240695529513886;
    PSVECAdd(&aisu->m_vel, &step, &step);
    PSVECScale(&step, &step, 0.5f);
    PSVECAdd(&aisu->m_pos, &step, &aisu->m_pos);
    f32 ratio = 5060.02f / aisu->m_pos.y;
    aisu->m_shadowScale = ratio * ratio;
    if (aisu->m_pos.y <= 5060.02f) {
        aisu->playSound(0x626);
        aisu->m_pos.y = 5060.02f;
        sprintf(aisu->m_name, "%s%s", s_aisuNames[aisu->m_type], s_aisuSuffix[1]);
        aisu->m_proc = s_procShrink;
        aisu->m_shadow = 0;
        unlinkNode(aisu);
        aisu->insertBack(&aisuFallList);
        CGameMaster::stageInstance->setAisuMotion();
        fn_1_A6C0(&aisu->m_pos, aisu->m_type);
        CAisu::aisuHandleMan->removeHandle(aisu->m_handle);
        aisu->m_handle = CAisu::aisuHandleMan->insertHandle(aisu);
    }
}

/* landed: shrink the shadow away */
extern "C" void fn_1_7BF0(CAisu *aisu)
{
    aisu->m_scale = aisu->m_scale - 0.004166666666666667;
    if (aisu->m_scale < 0.05f) {
        aisu->m_scale = 0.05f;
        aisu->m_proc = s_procGone;
    }
}

void CAisu::playSound(int se)
{
    HuAudFXStop(m_sound);
    m_sound = HuAudFXPlay(se);
    f32 dx = m_pos.x - 1578.489f;
    f32 dz = m_pos.z - 414.244f;
    f64 s = dz * sin(0.9296085834503174);
    f32 v = dx * cos(0.9296085834503174) - s;
    v = v / 454.20001220703125;
    HuAudFXPanning(m_sound, (s16)(64.0f + 16.0f * v));
}

/* sunk: count down, then delete */
extern "C" void fn_1_7D94(CAisu *aisu)
{
    aisu->m_pos.y = aisu->m_pos.y - 0.03333333333333333;
    if (aisu->m_pos.y <= 5058.02f) {
        if (aisu != 0) {
            delete aisu;
        }
    }
}

extern "C" void fn_1_7E3C(CAisu *aisu)
{
}

void CAisu::drawHook(Hu3DModel_s *model, Mtx *mtx)
{
    CAisuDrawData data;

    Hu3DModelObjDrawInit();
    PSMTXCopy(*mtx, data.m_mtx);
    CModel *inst = CGameMaster::GetAisuModel();
    s16 id = inst->getId();
    s16 id2 = id;
    data.m_model = id2;
    drawAisuList(&aisuAllList, &data);
    drawAisuList(&aisuFallList, &data);
    drawAisuList(&aisuConeList, &data);
}

void CAisu::drawAisu(CAisu *aisu, CAisuDrawData *data)
{
    Mtx mtx;

    if (aisu->m_shadow) {
        PSMTXTrans(mtx, aisu->m_pos.x, 5068.02f, aisu->m_pos.z);
        mtx[0][0] = 15.0f * aisu->m_shadowScale;
        mtx[2][2] = 15.0f * aisu->m_shadowScale;
        PSMTXConcat(data->m_mtx, mtx, mtx);
        Hu3DModelObjDraw(data->m_model, (char *)"kage", mtx);
    }
    Mtx *joint = aisu->m_joint.getMatrix();
    PSMTXConcat(data->m_mtx, *joint, mtx);
    Hu3DModelObjDraw(data->m_model, aisu->m_name, mtx);
}

CJoint *CAisu::doTake(CJoint *parent, int randomize)
{
    Mtx rot;

    HuAudFXStop(m_sound);
    m_sound = HuAudFXPlay(0x625);
    f32 dx = m_pos.x - 1578.489f;
    f32 dz = m_pos.z - 414.244f;
    f64 s = dz * sin(0.9296085834503174);
    f32 v = dx * cos(0.9296085834503174) - s;
    v = v / 454.20001220703125;
    HuAudFXPanning(m_sound, (s16)(64.0f + 16.0f * v));
    sprintf(m_name, "%s%s", s_aisuNames[m_type], s_aisuSuffix[2]);
    m_proc = s_procHeld;
    m_shadow = 0;
    m_taken = 1;
    m_joint.setSuper(parent);
    m_joint.m_locked = 1;
    CJoint *joint = &m_joint;
    if (randomize != 0) {
        joint->m_world[2][3] = 60.0f;
        joint->m_world[0][3] = CRandom::frandom(-10.0f, 10.0f);
        joint->m_world[1][3] = CRandom::frandom(-10.0f, 10.0f);
    } else {
        joint->m_world[2][3] = 0.0f;
        joint->m_world[0][3] = 0.0f;
        joint->m_world[1][3] = 0.0f;
    }
    PSMTXRotRad(rot, 'X', -1.5707964f);
    PSMTXConcat(joint->m_world, rot, joint->m_world);
    unlinkNode(this);
    insertBack(&aisuConeList);
    aisuHandleMan->removeHandle(m_handle);
    m_handle = aisuHandleMan->insertHandle(this);
    Mtx *mtx = m_joint.getMatrix();
    m_pos.x = 0.0f;
    m_pos.y = 0.0f;
    m_pos.z = 0.0f;
    PSMTXMultVec(*mtx, &m_pos, &m_pos);
    fn_1_A768(&m_pos, m_type);
    return &m_joint;
}

void CAisu::GetHandleList(std::vector<int> &out)
{
    out.m_size = 0;
    CAisu *aisu = (CAisu *)aisuAllList.m_prev;
    while (aisu != (CAisu *)&aisuAllList) {
        out.insert(out.m_data + out.m_size, 1, aisu->m_handle);
        aisu = (CAisu *)aisu->m_prev;
    }
}

bool CAisu::checkSlowMove(Vec *pos)
{
    CAisu *it;
    CAisu *end;
    CAisu *first;
    CAisu *copy;
    CAisu *next;

    first = (CAisu *)aisuFallList.m_prev;
    copy = first;
    it = copy;
    end = (CAisu *)&aisuFallList;
    while (it != end) {
        f32 dx = pos->x - it->m_pos.x;
        f32 dz = pos->z - it->m_pos.z;
        f64 distSq = (f64)dx * dx + (f64)dz * dz;
        if (distSq < 1600.0) {
            return true;
        }
        next = (CAisu *)it->m_prev;
        it = next;
    }
    return false;
}

CAisu *CAisu::checkTake(Vec *pos, f32 radius)
{
    Vec diff;
    CAisu *it;
    CAisu *end;
    CAisu *first;
    CAisu *copy;
    CAisu *next;

    first = (CAisu *)aisuAllList.m_prev;
    copy = first;
    it = copy;
    end = (CAisu *)&aisuAllList;
    while (it != end) {
        PSVECSubtract(pos, &it->m_pos, &diff);
        f32 mag = PSVECMag(&diff);
        if (mag <= 40.0f + radius) {
            return it;
        }
        next = (CAisu *)it->m_prev;
        it = next;
    }
    return 0;
}

extern "C" u8 fn_1_8500(CAisu *aisu)
{
    return (u8)aisu->m_taken;
}

/* append the xz position of every aisu in flight or landed */
extern "C" void fn_1_8510(std::vector<HuVec2f_s> *out)
{
    HuVec2f_s v;

    CAisu *aisu = (CAisu *)aisuAllList.m_prev;
    while (aisu != (CAisu *)&aisuAllList) {
        v.x = 0.0f;
        v.y = 0.0f;
        v.x = aisu->m_pos.x;
        v.y = aisu->m_pos.z;
        out->insert(out->m_data + out->m_size, 1, v);
        aisu = (CAisu *)aisu->m_prev;
    }
    aisu = (CAisu *)aisuFallList.m_prev;
    while (aisu != (CAisu *)&aisuFallList) {
        v.x = 0.0f;
        v.y = 0.0f;
        v.x = aisu->m_pos.x;
        v.y = aisu->m_pos.z;
        out->insert(out->m_data + out->m_size, 1, v);
        aisu = (CAisu *)aisu->m_prev;
    }
}

void CAisu::setPos(Vec *pos, f32 t)
{
    m_pos.x = m_pos.x + t * (pos->x - m_pos.x);
    m_pos.z = m_pos.z + t * (pos->z - m_pos.z);
}

void CAisu::drawAisuList(CLinkList *list, CAisuDrawData *data)
{
    CAisu *end = (CAisu *)list;
    CAisu *first = (CAisu *)list->m_prev;
    CAisu *it = first;
    drawAisuList(it, end, data);
}

void CAisu::drawAisuList(CAisu *first, CAisu *end, CAisuDrawData *data)
{
    CAisu *aisu = first;
    while (aisu != end) {
        Mtx mtx;

        if (aisu->m_shadow) {
            PSMTXTrans(mtx, aisu->m_pos.x, 5068.02f, aisu->m_pos.z);
            mtx[0][0] = 15.0f * aisu->m_shadowScale;
            mtx[2][2] = 15.0f * aisu->m_shadowScale;
            PSMTXConcat(data->m_mtx, mtx, mtx);
            Hu3DModelObjDraw(data->m_model, (char *)"kage", mtx);
        }
        Mtx *joint = aisu->m_joint.getMatrix();
        PSMTXConcat(data->m_mtx, *joint, mtx);
        Hu3DModelObjDraw(data->m_model, aisu->m_name, mtx);
        aisu = (CAisu *)aisu->m_prev;
    }
}

void CAisu::execAisuList(CLinkList *list, void (*func)(CAisu *))
{
    if (!list->isEmpty()) {
        CLinkList tmp;

        list->copyLink(&tmp);
        while (!tmp.isEmpty()) {
            CAisu *aisu = (CAisu *)tmp.m_prev;
            unlinkNode(aisu);
            aisu->insertBack(list);
            func(aisu);
        }
    }
}

extern "C" CAisu *fn_1_88DC(CAisu *this_, s16 flag)
{
    if (this_ != 0) {
        __dt__9CLinkNodeFv(this_, 0);
        if (flag > 0) {
            __dl__FPv(this_);
        }
    }
    return this_;
}

} // namespace m501

namespace std {

template <>
void __vector_pod<int, allocator<int> >::insert(int *pos, unsigned long n, const int &value)
{
    if (n == 0) {
        return;
    }
    unsigned long maxSize = 0x3FFFFFFF;
    if (n > maxSize || m_size > maxSize - n) {
        fprintf(M501_STDERR, "vector::insert length error\n");
        abort();
    }
    if (m_size + n <= m_cap) {
        int *end = m_data + m_size;
        int after = end - pos;
        const int *p = &value;
        if (end - (after - n) <= p && p < end) {
            p += n;
        }
        if (after != 0) {
            int *src = end;
            int *dst = end + n;
            while (src > pos) {
                src--;
                dst--;
                *dst = *src;
            }
        }
        unsigned long cnt = n;
        int *d = pos;
        while (cnt != 0) {
            *d = *p;
            d++;
            cnt--;
        }
        m_size += n;
        return;
    }
    int *oldData = m_data;
    int idx = pos - m_data;
    unsigned long newSize = m_size + n;
    unsigned long cap = m_cap != 0 ? m_cap : 1;
    while (newSize > cap) {
        if (cap < (maxSize >> 1)) {
            cap *= 2;
        } else {
            cap = maxSize;
        }
    }
    int *buf = (int *)__nw__FUl(cap * 4);
    if (buf == 0) {
        fprintf(M501_STDERR, "Memory allocation failure");
        abort();
    }
    m_data = buf;
    unsigned long cnt = n;
    int *d = m_data + idx;
    while (cnt != 0) {
        *d = value;
        d++;
        cnt--;
    }
    if (oldData != 0) {
        int *dst = m_data;
        int *src = oldData;
        while (src < oldData + idx) {
            *dst = *src;
            src++;
            dst++;
        }
        dst = m_data + idx + n;
        src = oldData + idx;
        while (src < oldData + m_size) {
            *dst = *src;
            src++;
            dst++;
        }
        __dl__FPv(oldData);
    }
    m_size = newSize;
    m_cap = cap;
}

template <>
void __vector_pod<HuVec2f_s, allocator<HuVec2f_s> >::insert(HuVec2f_s *pos, unsigned long n, const HuVec2f_s &value)
{
    if (n == 0) {
        return;
    }
    unsigned long maxSize = 0x1FFFFFFF;
    if (n > maxSize || m_size > maxSize - n) {
        fprintf(M501_STDERR, "vector::insert length error\n");
        abort();
    }
    if (m_size + n <= m_cap) {
        HuVec2f_s *end = m_data + m_size;
        int after = end - pos;
        const HuVec2f_s *p = &value;
        unsigned long cnt = n;
        if (cnt > (unsigned long)after) {
            HuVec2f_s *dst = end;
            while (cnt > (unsigned long)after) {
                if (dst != 0) {
                    *dst = value;
                }
                dst++;
                cnt--;
                m_size++;
            }
            HuVec2f_s *src = pos;
            while (src < end) {
                if (dst != 0) {
                    *dst = *src;
                }
                src++;
                dst++;
                m_size++;
            }
        } else {
            HuVec2f_s *src = end - cnt;
            HuVec2f_s *dst = end;
            while (src < end) {
                if (dst != 0) {
                    *dst = *src;
                }
                src++;
                dst++;
                m_size++;
            }
            if (end - (after - cnt) <= p && p < end) {
                p += cnt;
            }
            HuVec2f_s *a = end;
            HuVec2f_s *b = pos + (after - cnt);
            while (b > pos) {
                b--;
                a--;
                a->x = b->x;
                a->y = b->y;
            }
        }
        unsigned long fill = cnt;
        HuVec2f_s *d = pos;
        while (fill != 0) {
            d->x = p->x;
            d->y = p->y;
            d++;
            fill--;
        }
        return;
    }
    __vector_pod<HuVec2f_s, allocator<HuVec2f_s> > tmp;
    tmp.m_cap = 0;
    tmp.m_size = 0;
    tmp.m_data = 0;
    unsigned long newSize = m_size + n;
    unsigned long cap = m_cap != 0 ? m_cap : 1;
    while (newSize > cap) {
        if (cap < (maxSize >> 1)) {
            cap *= 2;
        } else {
            cap = maxSize;
        }
    }
    HuVec2f_s *buf = (HuVec2f_s *)__nw__FUl(cap * 8);
    if (buf == 0) {
        fprintf(M501_STDERR, "Memory allocation failure");
        abort();
    }
    tmp.m_data = buf;
    tmp.m_cap = cap;
    HuVec2f_s *dst = tmp.m_data;
    HuVec2f_s *src = m_data;
    HuVec2f_s *oldEnd = m_data + m_size;
    while (src < pos) {
        if (dst != 0) {
            *dst = *src;
        }
        src++;
        dst++;
        tmp.m_size++;
    }
    unsigned long cnt = n;
    while (cnt != 0) {
        if (dst != 0) {
            *dst = value;
        }
        dst++;
        cnt--;
        tmp.m_size++;
    }
    while (src < oldEnd) {
        if (dst != 0) {
            *dst = *src;
        }
        src++;
        dst++;
        tmp.m_size++;
    }
    if (&tmp != this) {
        unsigned long t = tmp.m_cap;
        tmp.m_cap = m_cap;
        m_cap = t;
        HuVec2f_s *td = tmp.m_data;
        tmp.m_data = m_data;
        m_data = td;
        t = tmp.m_size;
        tmp.m_size = m_size;
        m_size = t;
    }
    fn_1_729C(&tmp, -1);
}

} // namespace std

namespace m501 {

#ifndef TARGET_PC
extern "C" void __register_global_object(void *obj, void *(*dtor)(void *, s16), void *reg);
extern "C" void __ct__9CLinkNodeFv(void *self);

extern "C" void fn_1_922C(void)
{
    s_unused70 = 0x78;
    s_unused74 = 0xD2;
    s_unused78 = 0xD2;
    __ct__9CLinkNodeFv(aisuConeList__Q24m5015CAisu);
    __register_global_object(aisuConeList__Q24m5015CAisu, (void *(*)(void *, s16))fn_1_88DC, s_aisuConeChain);
    __ct__9CLinkNodeFv(aisuAllList__Q24m5015CAisu);
    __register_global_object(aisuAllList__Q24m5015CAisu, (void *(*)(void *, s16))fn_1_88DC, s_aisuAllChain);
    __ct__9CLinkNodeFv(aisuFallList__Q24m5015CAisu);
    __register_global_object(aisuFallList__Q24m5015CAisu, (void *(*)(void *, s16))fn_1_88DC, s_aisuFallChain);
}
#endif

} // namespace m501
