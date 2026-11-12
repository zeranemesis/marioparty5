#include "REL/m501Dll.h"
#include "game/hu3d.h"
#include "game/gamework.h"
#include "game/pad.h"
extern "C" {
#include "game/charman.h"
#include "game/object.h"
}
#include <dolphin/mtx.h>
#include <math.h>
#include <string.h>

#ifdef TARGET_PC
#include <stdio.h>
#include <stdlib.h>
#define M501_STDERR stderr
#else
extern "C" {
extern char __files[];
int fprintf(void *file, const char *fmt, ...);
void abort(void);
double sin(double x);
double cos(double x);
double atan2(double y, double x);
}
#define M501_STDERR ((void *)(__files + 0xA0))
#endif

extern "C" {
void __dl__FPv(void *ptr);
void *__nw__FUl(unsigned long size);
void __dt__9CLinkNodeFv(void *self, s32 flag);
void Hu3D3Dto2D(Vec *src, s16 camBit, Vec *dst);
}

namespace std {
template <>
vector<int, allocator<int> >::~vector()
{
    if (this != 0 && this != 0 && this != 0 && m_data != 0) {
        __dl__FPv(m_data);
    }
}
}

namespace m501 {

extern "C" void __ct__9CLinkNodeFR9CLinkNode(CLinkNode *self);
extern "C" s32 fn_1_4FB4(CCircle *a, CCircle *b, f32 wa, f32 wb);

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

extern "C" void UpdatePlayerEnding__Q24m5017CPlayerFPQ24m5017CPlayer(CPlayer *a, CPlayer *b);
extern "C" void execPlayerListReverse__Q24m5017CPlayerFP9CLinkListPFPQ24m5017CPlayer_v(CLinkList *list, void (*func)(CPlayer *, CPlayer *));

/* file-scope ints the original sets up in its static initialiser */
class CInt {
public:
    CInt(int v) : m_value(v) {}
    s32 m_value;
};

#ifdef TARGET_PC
static CInt s_unusedD8(120);
static CInt s_unusedDC(210);
static CInt s_unusedE0(210);
static CLinkList s_playerList;
static CPlayer *s_winner;
static f32 s_shrinkRate = 0.033333335f;
#define playerList s_playerList
#else
/* the GC build runs the initialiser by hand: fn_1_100B8 below is the .ctors entry */
static s32 s_unusedD8;
static s32 s_unusedDC;
static s32 s_unusedE0;
static u8 s_playerChain[12];
extern "C" {
u8 playerList__Q24m5017CPlayer[8];
}
static CPlayer *s_winner;
static f32 s_shrinkRate;
#define playerList (*(CLinkList *)playerList__Q24m5017CPlayer)
#endif

static const Vec s_stageCenter = { 1578.489f, 5060.02f, 414.244f };

static f32 s_runDist = 60.000004f;
static f32 s_walkDist = 30.000002f;

s16 CPlayer::getVoicePan(Vec *pos)
{
    Vec out;
    s16 pan;
    s32 v;

    Hu3D3Dto2D(pos, 1, &out);
    out.x = out.x * 0.11111111f;
    v = (s32)(32.0f + out.x);
    if (v < 0) {
        pan = 0;
    } else if (v > 0x7F) {
        pan = 0x7F;
    } else {
        pan = v;
    }
    return pan;
}

CPlayer::CPlayer(int playerNo) :
    m_waitTimer(0),
    m_timer34(0),
    m_model(GwPlayerConf[playerNo].charNo, 2),
    m_cone(*CGameMaster::GetConeModel()),
    m_joint(),
    m_motIds(),
    m_comTimer(0),
    m_pad12C(0)
{
    insertBack(&playerList);
    m_circle.m_radius = 80.0f;
    m_playerNo = playerNo;
    m_charNo = GwPlayerConf[playerNo].charNo;
    m_speed = (f32)(GwPlayerConf[playerNo].dif + 1) * 0.25f;
    if (GwPlayerConf[playerNo].type != 0) {
        m_padNo = -1;
    } else {
        m_padNo = GwPlayerConf[playerNo].padNo;
    }
    m_targetAngle = 53.26265f;
    m_angle = 53.26265f;
    m_coneJoint = &m_joint;
    m_score = 0;
    m_isWinner = 0;
    m_motion = -1;
    m_coneScale = 1.0f;
    PSMTXIdentity(m_mtx);
    Hu3DModelShadowSet(m_model.getId());
    Hu3DModelShadowDispOn(m_model.getId());
    Hu3DModelLayerSet(m_model.getId(), 0);
    Hu3DModelCameraSet(m_model.getId(), 1);
    m_model.setAttr(1);
    m_motIds.m_size = 0;
    m_motIds.reserve(6);
    for (int i = 0; i < 4; i++) {
        createMotion(0x8C0040 + i);
    }
    createMotion(0x8C0007);
    createMotion(0x8C0021);
    if (m_motIds.m_size == 6) {
    } else {
        __assert_fail("m_mot_id.size() == NUMBEROF_MOTION", "player.cpp", 0x61);
    }
    CharMotionVoiceOnSet(m_charNo, 0x43, 0);
    CharMotionVoiceOnSet(m_charNo, 7, 0);
    Hu3DModelHookSet(m_model.getId(), CharModelItemHookGet(m_charNo, 4, 0), m_cone.getId());
    CharMotionDataClose(m_charNo);
    CharModelFxFlagSet(m_charNo, 0);
    GetStartPos(&m_circle.m_pos, playerNo);
    m_lastPos.x = m_circle.m_pos.x;
    m_lastPos.y = m_circle.m_pos.y;
    m_lastPos.z = m_circle.m_pos.z;
    m_stepCount = 0;
    {
        CPlayer::PROC tmp = &CPlayer::proc_hold_cone;
        m_proc = tmp;
    }
    memset(&m_comTimer, 0, 0x24); /* comTimer .. comLocal are contiguous 32-bit fields */
    m_coneHandle = 0;
}

/* vector<int> destructors, in the three inlining depths the original emits */
extern "C" void *fn_1_B77C(std::vector<int> *vec, s16 flag)
{
    if (vec != 0) {
        if (vec != 0 && vec != 0 && vec->data_ref() != 0) {
            int *data = vec->begin();
            __dl__FPv(data);
        }
        if (flag > 0) {
            __dl__FPv(vec);
        }
    }
    return vec;
}

extern "C" void *fn_1_B810(std::vector<int> *vec, s16 flag)
{
    if (vec != 0) {
        if (vec != 0 && vec->data_ref() != 0) {
            int *data = vec->begin();
            __dl__FPv(data);
        }
        if (flag > 0) {
            __dl__FPv(vec);
        }
    }
    return vec;
}

extern "C" void *fn_1_B89C(std::vector<int> *vec, s16 flag)
{
    if (vec != 0) {
        if (vec->data_ref() != 0) {
            int *data = vec->begin();
            __dl__FPv(data);
        }
        if (flag > 0) {
            __dl__FPv(vec);
        }
    }
    return vec;
}

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

void CPlayer::createMotion(int dataNum)
{
    int mot = CharMotionCreate(m_charNo, dataNum);
    m_motIds.insert(m_motIds.end(), 1, mot);
}

void CPlayer::GetStartPos(Vec *out, int playerNo)
{
    f32 rad = -0.9296086f;
    f32 k = 1.8f;
    f32 ang = playerNo * k - 1.5 * k;
    f32 dx = 100.0 * (ang * cos(rad));
    f32 dz = 100.0 * (ang * sin(rad));
    f32 vx = 1578.489f + dx;
    f32 vz = 414.244f + dz;
    out->x = vx;
    out->y = 5060.02f;
    out->z = vz;
}

void CPlayer::PlayerShow()
{
    CPlayer *it;
    CPlayer *end;
    CPlayer *first;
    CPlayer *copy;
    CPlayer *next;

    first = (CPlayer *)playerList.m_prev;
    copy = first;
    it = copy;
    end = (CPlayer *)&playerList;
    while (it != end) {
        it->m_model.resetAttr(1);
        next = (CPlayer *)it->m_prev;
        it = next;
    }
}

void CPlayer::Close()
{
    while (!playerList.isEmpty()) {
        CPlayer *p = (CPlayer *)playerList.m_next;
        delete p;
    }
}

void CPlayer::UpdateAll()
{
    execPlayerList(&playerList, UpdatePlayer);
    execPlayerListReverse__Q24m5017CPlayerFP9CLinkListPFPQ24m5017CPlayer_v(&playerList, UpdatePlayerEnding__Q24m5017CPlayerFPQ24m5017CPlayer);
    execPlayerList(&playerList, CheckAisu);
    CPlayer *first = (CPlayer *)playerList.m_prev;
    if (first != (CPlayer *)&playerList) {
        unlinkNode(first);
        first->insertBack(&playerList);
    }
}

void CPlayer::UpdatePlayer(CPlayer *p)
{
    Vec rot;
    CharModelVoicePanSet(p->m_charNo, getVoicePan(&p->m_circle.m_pos));
    (p->*(p->m_proc))();
    p->doStageCollide();
    PSMTXRotRad(p->m_mtx, 'Y', 0.017453292f * p->m_angle);
    PSMTXTransApply(p->m_mtx, p->m_mtx, p->m_circle.m_pos.x, p->m_circle.m_pos.y, p->m_circle.m_pos.z);
    rot.x = 0.0f;
    rot.y = p->m_angle;
    rot.z = 0.0f;
    p->m_model.setRot(&rot);
    p->m_model.setPos(&p->m_circle.m_pos);
    p->shrinkCone();
    if (p->m_waitTimer > 0) {
        s32 t = p->m_waitTimer - 1;
        p->m_waitTimer = t;
        if (t == 0) {
        }
    }
    if (p->m_waitTimer <= 0) {
        p->updateComTargetPos();
    }
}

void CPlayer::doStageCollide()
{
    Vec d;
    PSVECSubtract(&m_circle.m_pos, &s_stageCenter, &d);
    f32 mag = PSVECMag(&d);
    if (!(mag < 474.2f)) {
        PSVECScale(&d, &d, 1.0f + ((474.2f - mag) / mag));
        PSVECAdd(&s_stageCenter, &d, &m_circle.m_pos);
    }
}

void CPlayer::getJointPos(Vec *out)
{
    Mtx *mtx = m_coneJoint->getMatrix();
    f32 x;
    f32 y;
    f32 z;
    z = (*mtx)[2][3];
    y = (*mtx)[1][3];
    x = (*mtx)[0][3];
    out->x = x;
    out->y = y;
    out->z = z;
}

void CPlayer::CheckAisu(CPlayer *p)
{
    Vec pos;

    p->centerAisu();
    p->getJointPos(&pos);
    CAisu *aisu = CAisu::checkTake(&pos, 20.0f);
    if (aisu == 0) {
        pos.y += 200.0f;
        CAisu *high = CAisu::checkTake(&pos, 20.0f);
        if (high != 0) {
            p->startConeDrop(high->m_handle);
        }
    } else {
        s32 n = p->m_score;
        p->m_score = n + 1;
        p->m_coneJoint = aisu->doTake(p->m_coneJoint, n);
        omVibrate(p->m_playerNo, 12, 6, 6);
        p->m_coneHandle = 0;
    }
}

void CPlayer::startConeDrop(int handle)
{
    CPlayer *it;
    CPlayer *end;
    CPlayer *first;
    CPlayer *copy;
    CPlayer *next;

    first = (CPlayer *)playerList.m_prev;
    copy = first;
    it = copy;
    end = (CPlayer *)&playerList;
    while (it != end) {
        if (it->m_coneHandle == handle) {
            return;
        }
        next = (CPlayer *)it->m_prev;
        it = next;
    }
    m_coneHandle = handle;
    if (CGameObject::GetUpdateMode() == 2) {
        {
            CPlayer::PROC tmp = &CPlayer::proc_drop_cone_start;
            m_proc = tmp;
        }
    }
}

s32 CPlayer::centerAisu()
{
    Vec pos;
    getJointPos(&pos);
    CAisu *aisu = (CAisu *)CAisu::getHandle(m_coneHandle);
    if (aisu == 0) {
        return 0;
    }
    aisu->setPos(&pos, 0.6f);
    return 1;
}

void CPlayer::updateComTargetPos()
{
    Mtx inv;
    Vec pos;

    PSMTXInverse(m_mtx, inv);
    getJointPos(&pos);
    PSMTXMultVec(inv, &pos, &m_comLocal);
}

/* distance helpers the original keeps out of line; UpdatePlayerEnding inlines them */
static inline void StartDelta(CPlayer *p, Vec *out)
{
    CPlayer::GetStartPos(out, p->m_playerNo);
    PSVECSubtract(out, &p->m_circle.m_pos, out);
}

extern "C" Vec *fn_1_C400(CPlayer *self, Vec *out)
{
    StartDelta(self, out);
    return out;
}

extern "C" f32 fn_1_C59C(CPlayer *self)
{
    Vec v;
    StartDelta(self, &v);
    return PSVECMag(&v);
}

extern "C" void UpdatePlayerEnding__Q24m5017CPlayerFPQ24m5017CPlayer(CPlayer *a, CPlayer *b)
{
    f32 ratio = 0.5f;
    if (CGameObject::GetUpdateMode() == 3) {
        Vec va;
        Vec vb;
        StartDelta(a, &va);
        f32 da = PSVECMag(&va);
        StartDelta(b, &vb);
        f32 db = PSVECMag(&vb);
        f32 sum = da + db;
        if (sum != 0.0f) {
            ratio = db / sum;
        }
    } else {
        f32 w = a->m_moveMag + b->m_moveMag;
        if (w != 0.0f) {
            ratio = a->m_moveMag / w;
        }
    }
    if (fn_1_4FB4(&a->m_circle, &b->m_circle, ratio, 1.05f) != 0 && CGameObject::GetUpdateMode() == 3) {
        Vec vb;
        Vec va;
        StartDelta(b, &vb);
        StartDelta(a, &va);
        f32 dot = PSVECDotProduct(&va, &vb);
        f32 mb = PSVECMag(&vb);
        f32 ma = PSVECMag(&va);
        if ((ma + mb) != 0.0f && !(dot > -0.95f)) {
            Vec dir;
            Vec up;
            PSVECSubtract(&a->m_circle.m_pos, &b->m_circle.m_pos, &dir);
            dir.y = 0.0f;
            PSVECNormalize(&dir, &dir);
            up.x = 0.0f;
            up.y = 1.0f;
            up.z = 0.0f;
            PSVECCrossProduct(&dir, &up, &dir);
            PSVECScale(&dir, &dir, 0.0f);
            a->m_circle.m_pos.x = a->m_circle.m_pos.x + dir.x;
            a->m_circle.m_pos.z = a->m_circle.m_pos.z + dir.z;
            b->m_circle.m_pos.x = b->m_circle.m_pos.x - dir.x;
            b->m_circle.m_pos.z = b->m_circle.m_pos.z - dir.z;
        }
    }
}

void CPlayer::shrinkCone()
{
    char *hook = CharModelItemHookGet(m_charNo, 4, 0);
    m_joint.m_locked = 1;
    CJoint *joint = &m_joint;
    Hu3DModelObjMtxGet(m_model.getId(), hook, joint->m_world);
    if (m_coneScale < 1.0f) {
        Hu3DMtx *coneMtx = m_cone.getMatrix();
        Mtx scale;
        PSMTXScale(scale, m_coneScale, m_coneScale, m_coneScale);
        PSMTXConcat(scale, (f32(*)[4])coneMtx, (f32(*)[4])coneMtx);
        PSMTXConcat(joint->m_world, scale, joint->m_world);
    }
    m_joint.lockMatrixChain();
}

void CPlayer::setMotion(int idx, bool once)
{
    s16 mdl = m_model.m_id;
    s16 mot = *m_motIds.at(idx);
    if (m_motion != idx) {
        Hu3DModelAttrReset(mdl, 0x40000002);
        u32 attr;
        if (once == 0) {
            attr = 0x40000001;
        } else {
            attr = 0;
        }
        CharMotionShiftSet(m_charNo, mot, 0.0f, 8.0f, attr);
        m_motion = idx;
    }
}

static inline s32 DegToBin(f32 deg)
{
    return (s32)(182.04444444444445 * deg);
}

static inline f32 BinToDeg(s32 bin)
{
    return 0.0054931640625 * bin;
}

f32 CPlayer::SubAngle(f32 a, f32 b)
{
    s32 binA = DegToBin(a);
    s32 binB = DegToBin(b);
    s32 d = (u16)((binA - binB) - 0x8000) - 0x8000;
    return BinToDeg(d);
}

f32 CPlayer::SubAngleMax(f32 a, f32 b, f32 max)
{
    f32 diff = SubAngle(a, b);
    f32 ret;
    if (diff < -max) {
        ret = -max;
    } else if (diff > max) {
        ret = max;
    } else {
        ret = diff;
    }
    return ret;
}

void CPlayer::proc_hold_cone()
{
    setMotion(0, false);
    {
        CPlayer::PROC tmp = &CPlayer::proc_main;
        m_proc = tmp;
    }
}

void CPlayer::proc_main()
{
    Vec dir;
    getMoveDir(&dir);
    movePlayer(&dir);
}

void CPlayer::proc_drop_cone_start()
{
    setMotion(2, true);
    Hu3DMotionShiftStartEndSet(m_model.getId(), 0.0f, 24.0f);
    {
        CPlayer::PROC tmp = &CPlayer::proc_drop_cone;
        m_proc = tmp;
    }
}

void CPlayer::proc_drop_cone()
{
    if (CharMotionEndCheck(m_charNo) != 0) {
        {
            CPlayer::PROC tmp = &CPlayer::proc_main;
            m_proc = tmp;
        }
    }
}

void CPlayer::startWinnerMove()
{
    CPlayer *it;
    CPlayer *end;
    CPlayer *first;
    CPlayer *copy;
    CPlayer *next;

    first = (CPlayer *)playerList.m_prev;
    copy = first;
    it = copy;
    end = (CPlayer *)&playerList;
    while (it != end) {
        {
            CPlayer::PROC tmp = &CPlayer::proc_winner_move;
            it->m_proc = tmp;
        }
        it->m_waitTimer = 1;
        next = (CPlayer *)it->m_prev;
        it = next;
    }
}

void CPlayer::proc_winner_move()
{
    Vec toStart;
    StartDelta(this, &toStart);
    f32 dist = PSVECMag(&toStart);
    m_moveMag = 1.0f;
    if (dist > 50.0f) {
        PSVECScale(&toStart, &toStart, 1.0f / dist);
        movePlayer(&toStart);
        return;
    }
    GetStartPos(&m_circle.m_pos, m_playerNo);
    f32 speed = getWinAngleSpeed(13.666667f, 1.3333334f);
    f32 turn = SubAngleMax(53.26265f, m_angle, speed);
    m_angle = m_angle + turn;
    f32 absTurn;
    if (turn < 0.0f) {
        absTurn = -turn;
    } else {
        absTurn = turn;
    }
    if (absTurn < 0.01f) {
        setMotion(0, false);
    } else {
        m_waitTimer = 0x12;
    }
    m_moveMag = 0.0f;
}

void CPlayer::SortPlayerList()
{
    sortList__9CLinkListFPFP9CLinkNodeP9CLinkNode_v(&playerList, (bool (*)(CLinkNode *, CLinkNode *))ComparePlayer);
    CPlayer *first = (CPlayer *)playerList.m_prev;
    s32 top = first->m_score;
    s_winner = 0;
    if (top != 0) {
        s_winner = first;
        s32 count = 0;
        CPlayer *p = first;
        while (p != (CPlayer *)&playerList) {
            if (p->m_score == top) {
                CMinigameCore::AddWinner(p->m_playerNo);
                p->m_isWinner = 1;
                count += 1;
            }
            p = (CPlayer *)p->m_prev;
        }
        if (count != 1) {
            s_winner = 0;
        }
    }
}

#pragma global_optimizer on
#pragma optimization_level 4
#pragma function_align 16

// NONMATCHING: register allocation differs (r6/r5 vs r3/r0), logic identical
s32 CPlayer::ComparePlayer(CLinkNode *a, CLinkNode *b)
{
    CPlayer *playerA = (CPlayer *)a;
    CPlayer *playerB = (CPlayer *)b;
    s32 valA = playerA->m_score;
    s32 valB = playerB->m_score;

    s32 xorVal = valA ^ valB;
    s32 shrVal = xorVal >> 1;
    s32 andVal = xorVal & valA;
    s32 subVal = shrVal - andVal;

    return (s32)((u32)subVal >> 31);
}

#pragma global_optimizer reset
#pragma optimization_level reset
#pragma function_align reset

bool CPlayer::StartPlayerWin()
{
    CPlayer *p;
    s32 winners;

    sortList__9CLinkListFPFP9CLinkNodeP9CLinkNode_v(&playerList, (bool (*)(CLinkNode *, CLinkNode *))ComparePlayer);
    CPlayer *top = (CPlayer *)playerList.m_prev;
    s32 topScore = top->m_score;
    s_winner = 0;
    if (topScore != 0) {
        s_winner = top;
        s32 count = 0;
        CPlayer *q = top;
        while (q != (CPlayer *)&playerList) {
            if (q->m_score == topScore) {
                CMinigameCore::AddWinner(q->m_playerNo);
                q->m_isWinner = 1;
                count += 1;
            }
            q = (CPlayer *)q->m_prev;
        }
        if (count != 1) {
            s_winner = 0;
        }
    }
    p = (CPlayer *)playerList.m_prev;
    winners = 0;
    while (p != (CPlayer *)&playerList) {
        if (p->m_isWinner != 0) {
            {
                CPlayer::PROC tmp = &CPlayer::proc_lose;
                p->m_proc = tmp;
            }
            p->m_timer34 = 0x1E;
            CharWinLoseVoicePlay(p->m_charNo, 0x8C0030, 0x1FA);
            winners += 1;
        } else {
            {
                CPlayer::PROC tmp = &CPlayer::proc_win;
                p->m_proc = tmp;
            }
        }
        if (p == s_winner) {
            if (CGameMaster::cameraInstance != 0) {
            } else {
                __assert_fail("ms_pointer != 0", "singleton.h", 0x27);
            }
            CCameraObj *cam = CGameMaster::cameraInstance;
            cam->set_focus_player(&p->m_circle.m_pos);
        }
        CPlayer *next = (CPlayer *)p->m_prev;
        p = next;
    }
    if (winners == 0) {
        CPlayer *r = (CPlayer *)playerList.m_prev;
        while (r != (CPlayer *)&playerList) {
            CharWinLoseVoicePlay(r->m_charNo, 0x8C0030, 0x200);
            CPlayer *next = (CPlayer *)r->m_prev;
            r = next;
        }
    }
    return winners == 0;
}

void CPlayer::proc_lose()
{
    Hu3DMotionSpeedSet(m_model.getId(), 1.0f);
    setMotion(3, true);
}

void CPlayer::proc_win()
{
    Hu3DMotionSpeedSet(m_model.getId(), 1.0f);
    setMotion(4, true);
    {
        CPlayer::PROC tmp = &CPlayer::proc_shrink_cone;
        m_proc = tmp;
    }
}

void CPlayer::proc_shrink_cone()
{
    m_coneScale -= s_shrinkRate;
    if (m_coneScale <= 0.0f) {
        m_coneScale = 0.0f;
    }
}

f32 CPlayer::getWinAngleSpeed(f32 a, f32 b)
{
    f32 t = (f32)m_score * 0.125f;
    if (t > 1.0f) {
        t = 1.0f;
    }
    return (a * (1.0f - t)) + (b * t);
}

s32 CPlayer::movePlayer(Vec *dir)
{
    Vec fwd;
    Vec step;
    Vec foot;

    if (m_moveMag < 0.3f) {
        Hu3DMotionSpeedSet(m_model.getId(), 1.0f);
        setMotion(0, false);
        return 0;
    }
    m_waitTimer = 0x30;
    m_targetAngle = 180.0 * (atan2(dir->x, dir->z) / 3.141592653589793);
    f32 turn = getWinAngleSpeed(13.666667f, 1.3333334f);
    f32 d = SubAngleMax(m_targetAngle, m_angle, turn);
    m_angle = m_angle + d;
    f32 rad = (3.141592653589793 * m_angle) / 180.0;
    fwd.z = cos(rad);
    fwd.x = sin(rad);
    fwd.y = 0.0f;
    f32 dot = PSVECDotProduct(&fwd, dir);
    if (dot < 0.0f) {
        return 1;
    }
    f32 base = getWinAngleSpeed(9.0f, 3.0f);
    f32 spd = base * (m_moveMag * (1.6666666666666667 * dot));
    if (CAisu::checkSlowMove(&m_circle.m_pos) != 0) {
        spd *= 0.3f;
    }
    PSVECScale(&fwd, &step, spd);
    PSVECAdd(&step, &m_circle.m_pos, &m_circle.m_pos);
    m_running = spd > 4.5f;
    if (m_running != 0) {
        setMotion(1, false);
    } else {
        setMotion(5, false);
    }
    if (m_charNo != 8) {
        f32 thresh;
        if (m_running != 0) {
            thresh = s_runDist * s_runDist;
        } else {
            thresh = s_walkDist * s_walkDist;
        }
        if (PSVECSquareDistance(&m_circle.m_pos, &m_lastPos) >= thresh) {
            m_lastPos.x = m_circle.m_pos.x;
            m_lastPos.y = m_circle.m_pos.y;
            m_lastPos.z = m_circle.m_pos.z;
            s32 n = m_stepCount;
            m_stepCount = n + 1;
            if ((n & 1) == 0) {
                foot.x = -20.0f;
            } else {
                foot.x = 20.0f;
            }
            foot.y = 0.0f;
            foot.z = 0.0f;
            PSMTXMultVec(m_mtx, &foot, &foot);
            new CAsiato(&foot, m_angle);
        }
    }
    return 1;
}

void CPlayer::GetStickData(char *x, char *y, char min, char max)
{
    f32 fx = (f32)(s8)*x;
    f32 fy = (f32)(s8)*y;
    f32 len = sqrtf(fx * fx + fy * fy);
    if (len < (f32)min) {
        *x = 0;
        *y = 0;
        return;
    }
    if (len > (f32)max) {
        *x = (u8)(s32)((f32)((s8)*x * max) / len);
        *y = (u8)(s32)((f32)((s8)*y * max) / len);
        f32 nx = (f32)(s8)*x;
        f32 ny = (f32)(s8)*y;
        len = sqrtf(nx * nx + ny * ny);
    }
    if (len > 0.000001f) {
        *x = (u8)(s32)((f32)(s8)*x - ((f32)((s8)*x * min) / len));
        *y = (u8)(s32)((f32)(s8)*y - ((f32)((s8)*y * min) / len));
    }
}

void CPlayer::getMoveDir(Vec *dir)
{
    m_moveMag = 0.0f;
    dir->x = 0.0f;
    dir->y = 0.0f;
    dir->z = 0.0f;
    if (CGameObject::GetUpdateMode() == 2) {
        if (m_padNo < 0) {
            getComMoveDir(dir);
            clampMag();
            return;
        }
        u8 stickX = (u8)HuPadStkX[m_padNo];
        u8 stickY = (u8)HuPadStkY[m_padNo];
        GetStickData((char *)&stickX, (char *)&stickY, 0, 28);
        dir->x = (f32)(s8)stickX / 28.0f;
        dir->y = 0.0f;
        dir->z = -((f32)(s8)stickY / 28.0f);
        Mtx rot;
        PSMTXRotRad(rot, 'Y', 0.9296086f);
        PSMTXMultVecSR(rot, dir, dir);
        m_moveMag = PSVECMag(dir);
        clampMag();
    }
}

void CPlayer::clampMag()
{
    f32 m = m_moveMag;
    if (m >= 0.8f) {
        m = 1.0f;
    } else if (m >= 0.3f) {
        m = 0.5f;
    } else {
        m = 0.0f;
    }
    m_moveMag = m;
}

static inline f32 Sqr(f32 v)
{
    return v * v;
}

/* nearest aisu handle to a position (an out-of-line copy of code getComMoveDir inlines) */
static inline s32 NearestAisu(std::vector<int> *ids, Vec *pos)
{
    f64 best = 2e+120;
    int *it = ids->begin();
    int *end = ids->end();
    s32 bestHandle = 0;
    while (it != end) {
        CAisu *aisu = (CAisu *)CAisu::getHandle(*it);
        if (aisu != 0) {
            f32 d = Sqr(aisu->m_pos.x - pos->x) + Sqr(aisu->m_pos.z - pos->z);
            if (!(best < d)) {
                best = d;
                bestHandle = *it;
            }
        }
        it++;
    }
    return bestHandle;
}

extern "C" s32 fn_1_F588(std::vector<int> *ids, Vec *pos)
{
    return NearestAisu(ids, pos);
}

extern "C" void fn_1_FBAC(CPlayer *self, std::vector<int> *ids);

void CPlayer::getComMoveDir(Vec *dir)
{
    Vec cur;

    PSMTXMultVec(m_mtx, &m_comLocal, &cur);
    if (CAisu::getHandle(m_comAisu) == 0) {
        std::vector<int> handles;
        handles.m_cap = 0;
        handles.m_size = 0;
        handles.m_data = 0;
        CAisu::GetHandleList(handles);
        fn_1_FBAC(this, &handles);
        m_comAisu = NearestAisu(&handles, &cur);
        f32 k = 0.25f + 0.75f * (1.0f - m_speed);
        m_comTimer = (s32)(k * (60.0f * CRandom::frandom(1.5f, 2.0f)));
        if (handles.m_data != 0) {
            __dl__FPv(handles.m_data);
        }
    }
    if (m_comTimer > 0) {
        s32 t = m_comTimer - 1;
        m_comTimer = t;
        if (t == 0) {
        }
    }
    if (m_comTimer <= 0) {
        Vec *target = &m_comTarget;
        CAisu *aisu = (CAisu *)CAisu::getHandle(m_comAisu);
        if (aisu != 0) {
            target->x = aisu->m_pos.x;
            target->y = aisu->m_pos.y;
            target->z = aisu->m_pos.z;
            PSVECSubtract(target, &cur, dir);
            dir->y = 0.0f;
            f32 len = PSVECMag(dir);
            if (!(len < 10.0f)) {
                if (len > 1.0f) {
                    PSVECScale(dir, dir, 1.0f / len);
                    len = 1.0f;
                }
                m_moveMag = len;
            }
        }
    }
}

extern "C" void *fn_1_FA8C(std::vector<int> *vec, s16 flag)
{
    if (vec != 0) {
        if (vec != 0 && vec != 0 && vec->data_ref() != 0) {
            int *data = vec->begin();
            __dl__FPv(data);
        }
        if (flag > 0) {
            __dl__FPv(vec);
        }
    }
    return vec;
}

extern "C" void *fn_1_FB20(std::vector<int> *vec, s16 flag)
{
    if (vec != 0) {
        if (vec != 0 && vec->data_ref() != 0) {
            int *data = vec->begin();
            __dl__FPv(data);
        }
        if (flag > 0) {
            __dl__FPv(vec);
        }
    }
    return vec;
}

/* drop from the candidate list every aisu another player is already chasing */
extern "C" void fn_1_FBAC(CPlayer *self, std::vector<int> *ids)
{
    CPlayer *p;
    CPlayer *end2;
    CPlayer *first2;
    CPlayer *copy2;
    CPlayer *next;

    first2 = (CPlayer *)playerList.m_prev;
    copy2 = first2;
    p = copy2;
    end2 = (CPlayer *)&playerList;
    while (p != end2) {
        if (p != self) {
            int *it = ids->begin();
            int *end = ids->end();
            while (it != end) {
                int *found = it;
                while (found != end && *found != p->m_comAisu) {
                    found++;
                }
                it = found;
                if (it != end) {
                    *it = 0;
                    it++;
                }
            }
        }
        next = (CPlayer *)p->m_prev;
        p = next;
    }
}

void CPlayer::GetPlayerDist(std::vector<HuVec2f_s> *out)
{
    HuVec2f_s v;
    CPlayer *it;
    CPlayer *end;
    CPlayer *first;
    CPlayer *copy;
    CPlayer *next;

    first = (CPlayer *)playerList.m_prev;
    copy = first;
    it = copy;
    end = (CPlayer *)&playerList;
    while (it != end) {
        v = (HuVec2f_s){ 0.0f, 0.0f };
        v.x = it->m_circle.m_pos.x;
        v.y = it->m_circle.m_pos.z;
        out->insert(out->m_data + out->m_size, 1, v);
        next = (CPlayer *)it->m_prev;
        it = next;
    }
}

bool CPlayer::CheckWinIdle()
{
    CPlayer *it;
    CPlayer *end;
    CPlayer *first;
    CPlayer *copy;
    CPlayer *next;

    first = (CPlayer *)playerList.m_prev;
    copy = first;
    it = copy;
    end = (CPlayer *)&playerList;
    while (it != end) {
        if (it->m_waitTimer > 0) {
            return false;
        }
        next = (CPlayer *)it->m_prev;
        it = next;
    }
    return true;
}

} // namespace m501

namespace std {
template <>
int *__vector_pod<int, allocator<int> >::at(unsigned long idx)
{
    if (idx >= m_size) {
        fprintf(M501_STDERR, "vector::at index out of range");
        abort();
    }
    return m_data + idx;
}
}

namespace m501 {

void CPlayer::execPlayerList(CLinkList *list, void (*func)(CPlayer *))
{
    CPlayer *first = (CPlayer *)list->m_prev;
    CPlayer *listPtr = (CPlayer *)list;
    execPlayerList(first, listPtr, func);
}

void CPlayer::execPlayerList(CPlayer *first, CPlayer *list, void (*func)(CPlayer *))
{
    while (first != list) {
        func(first);
        first = (CPlayer *)first->m_prev;
    }
}

extern "C" void execPlayerListReverse__Q24m5017CPlayerFP9CLinkListPFPQ24m5017CPlayer_v(CLinkList *list, void (*func)(CPlayer *, CPlayer *))
{
    CPlayer *a = (CPlayer *)list->m_prev;
    CLinkNode *last = list->m_next;
    while (a != (CPlayer *)last) {
        CPlayer *b = (CPlayer *)a->m_prev;
        while (b != (CPlayer *)list) {
            func(a, b);
            b = (CPlayer *)b->m_prev;
        }
        a = (CPlayer *)a->m_prev;
    }
}

} // namespace m501

namespace std {
template <>
void __vector_pod<int, allocator<int> >::reserve(unsigned long n)
{
    if (n > capacity()) {
        unsigned long maxSize = max_size();
        if (n > maxSize) {
            fprintf(M501_STDERR, "vector::reserve length error\n");
            abort();
        }
        int *oldData = begin();
        int *buf = (int *)__nw__FUl(n * 4);
        if (buf == 0) {
            fprintf(M501_STDERR, "Memory allocation failure");
            abort();
        }
        data_ref() = buf;
        if (oldData != 0) {
            int *dst = begin();
            int *src = oldData;
            while (src < oldData + size_ref()) {
                *dst = *src;
                src++;
                dst++;
            }
            __dl__FPv(oldData);
        }
        cap_ref() = n;
    }
}
}

namespace m501 {

#ifndef TARGET_PC
extern "C" void __register_global_object(void *obj, void *(*dtor)(void *, s16), void *reg);
extern "C" void __ct__9CLinkNodeFv(void *self);
extern "C" CPlayer *fn_1_10158(CPlayer *player, s16 flag);

extern "C" void fn_1_100B8(void)
{
    s_unusedD8 = 0x78;
    s_unusedDC = 0xD2;
    s_unusedE0 = 0xD2;
    __ct__9CLinkNodeFv(playerList__Q24m5017CPlayer);
    __register_global_object(playerList__Q24m5017CPlayer, (void *(*)(void *, s16))fn_1_10158, s_playerChain);
    s_shrinkRate = 0.033333335f;
}
#endif

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
