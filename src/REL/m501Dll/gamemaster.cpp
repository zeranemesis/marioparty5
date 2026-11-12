#include "REL/m501Dll.h"
#include "datanum/m501.h"
#include "game/hu3d.h"
#include "game/data.h"
extern "C" {
#include "game/audio.h"
}
#include "game/gamework.h"
#include "game/object.h"

extern "C" {
void __dl__FPv(void *ptr);
}

namespace m501 {

/* three file-scope delays the original sets up in its static initialiser */
class CInt {
public:
    CInt(int v) : m_value(v) {}
    s32 m_value;
};

#ifdef TARGET_PC
static CInt s_delay58(120);
static CInt s_delay5C(210);
static CInt s_delay60(210);
#else
/* the GC build runs the initialiser by hand: fn_1_7384 below is the .ctors entry */
static s32 s_delay58;
static s32 s_delay5C;
static s32 s_delay60;
#endif

/* minimum distance between two aisu spawns */
static s32 s_minDist = 300;

CStage *CGameMaster::stageInstance;
CCameraObj *CGameMaster::cameraInstance;

CGameMaster::CGameMaster() :
    CSubThread(0x8000),
    m_timer(0),
    m_cone((u32)DATANUM(DATA_m501, 6)),
    m_aisu((u32)DATANUM(DATA_m501, 5)),
    m_handles(0x1000)
{
    Hu3DModelShadowSet(m_cone.getId());
    Hu3DModelShadowDispOn(m_cone.getId());
    Hu3DModelLayerSet(m_cone.getId(), 0);
}

CGameMaster::~CGameMaster()
{
}

void CGameMaster::on_setup(Process_s *objMan)
{
    Hu3DModelAttrSet(m_aisu.m_id, 1);
    CAisu::init(&m_handles);
    CRandom::setSeed(OSGetTick());
    new CStage();
    new CParticle();
    new CCameraObj();
    for (int i = 0; i < 4; i++) {
        new CPlayer(i);
    }
}

void CGameMaster::on_update()
{
    wakeupThread(0);
    CGameObject::UpdateAll();
    CPlayer::UpdateAll();
    CAisu::UpdateAll();
}

void CGameMaster::thread_main()
{
    thread_exec();
}

void CGameMaster::thread_exec()
{
    s32 seId;
    s32 streamId;
    s32 spawnWait;
    s32 i;
    s32 n;
    s16 jingle;
    bool win;

    CGameObject::SetObjExecMode(0);
    CMinigameCore::StartFadeIn();
    HuAudFXPlay(0x628);
    seId = HuAudFXPlay(0x396);
    while (CGameObject::CheckObjExec() == 0) {
        sleepThread(0);
    }
    HuAudFXStop(seId);
    CGameObject::SetObjExecMode(1);
    CMinigameCore::StartStartGameMes();
    streamId = HuAudSStreamPlay(0x35);
    while (CMinigameCore::CheckGameMes() == 0) {
        sleepThread(0);
    }
    CGameObject::SetObjExecMode(2);
    m_timer.setParam(0x708, 0, 0);
    m_timer.setModeOn(1);
    spawnWait = 0;
    while (m_timer.checkDone() == 0) {
        if (!((f32)m_timer.getValue() < 150.0f)) {
            spawnWait -= 1;
            if (spawnWait < 0) {
                for (n = 3; n > 0; n--) {
                    newAisu();
                    for (i = 0; (f32)i < 12.0f; i++) {
                        sleepThread(0);
                    }
                }
                spawnWait = (s32)(90.0f + (f32)(CRandom::irandom() % 108));
            }
        }
        sleepThread(0);
    }
    CGameObject::SetObjExecMode(3);
    HuAudSStreamFadeOut(streamId, 100);
    CPlayer::startWinnerMove();
    CMinigameCore::StartFinishGameMes();
    for (i = 0; i < 0x78; i++) {
        sleepThread(0);
    }
    while (CPlayer::CheckWinIdle() == 0) {
        sleepThread(0);
    }
    CGameObject::SetObjExecMode(4);
    win = CPlayer::StartPlayerWin();
    if (win) {
        jingle = 0x7B;
    } else {
        jingle = 0x79;
    }
    HuAudJinglePlay(jingle);
    if (win) {
        for (i = 0; i < 4; i++) {
            CharFXPlay(GwPlayerConf[i].charNo, 0x200);
        }
    } else {
        for (i = 0; i < 4; i++) {
            if (CMinigameCore::CheckWinner(i) != 0) {
                CharFXPlay(GwPlayerConf[i].charNo, 0x1FA);
            }
        }
    }
    CMinigameCore::StartWinnerGameMes();
    while (CMinigameCore::CheckGameMes() == 0) {
        sleepThread(0);
    }
    CMinigameCore::ExitReq();
    for (;;) {
        sleepThread(-1);
    }
}

static inline f32 Sqr(f32 v)
{
    return v * v;
}

/* true when (x, z) is outside the arena or too close to a listed point */
static inline s32 CheckBad(std::vector<HuVec2f_s> *points, f32 x, f32 z)
{
    f32 cx;
    f32 cz;
    HuVec2f_s *it;
    HuVec2f_s *end;

    if (Sqr(x) + Sqr(z) > 206297.66f) {
        return 1;
    }
    cx = 1578.489f + x;
    cz = 414.244f + z;
    it = points->begin();
    end = points->end();
    while (it != end) {
        if (Sqr(it->x - cx) + Sqr(it->y - cz) < (f32)(s_minDist * s_minDist)) {
            return 1;
        }
        it++;
    }
    return 0;
}

extern "C" s32 fn_1_6C44(std::vector<HuVec2f_s> *points, f32 x, f32 z)
{
    return CheckBad(points, x, z);
}

void CGameMaster::newAisu()
{
    std::vector<HuVec2f_s> points;
    f32 x;
    f32 z;
    f32 y;
    s32 tries;
    Vec pos;

    CPlayer::GetPlayerDist(&points);
    fn_1_8510(&points);
    tries = 200;
    do {
        x = CRandom::frandom(-454.2f, 454.2f);
        z = CRandom::frandom(-454.2f, 454.2f);
        tries -= 1;
        if (tries == 0) {
            return;
        }
    } while (CheckBad(&points, x, z));
    y = 100.0f * CRandom::frandom(8.0f, 40.0f);
    pos.x = 1578.489f + x;
    pos.y = 5060.02f + y;
    pos.z = 414.244f + z;
    new CAisu((s32)CRandom::irandom() % 8, &pos);
}

/* vector destructors: the three flavours the original emits */
extern "C" void *fn_1_729C(void *self, s16 flag);
extern "C" void fn_1_7328(void *self);

extern "C" void *fn_1_715C(void *self, s16 flag)
{
    if (self != 0) {
        if (self != 0) {
            fn_1_729C(self, 0);
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

extern "C" void *fn_1_71BC(void *self, s16 flag)
{
    if (self != 0) {
        fn_1_729C(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void CGameMaster::on_close()
{
    CPlayer::Close();
    CAisu::Close();
    CGameObject::Close();
}

CModel *CGameMaster::GetConeModel()
{
    if (CMinigameCore::instance != 0) {
    } else {
        __assert_fail("ms_pointer != 0", "singleton.h", 0x27);
    }
    CGameMaster *master = (CGameMaster *)CMinigameCore::instance;
    return &master->m_cone;
}

CModel *CGameMaster::GetAisuModel()
{
    if (CMinigameCore::instance != 0) {
    } else {
        __assert_fail("ms_pointer != 0", "singleton.h", 0x27);
    }
    CGameMaster *master = (CGameMaster *)CMinigameCore::instance;
    return &master->m_aisu;
}

extern "C" void *fn_1_729C(void *self, s16 flag)
{
    std::vector<HuVec2f_s> *vec = (std::vector<HuVec2f_s> *)self;
    if (vec != 0) {
        fn_1_7328(vec);
        HuVec2f_s *data = vec->m_data;
        if (data != 0) {
            __dl__FPv(data);
        }
        if (flag > 0) {
            __dl__FPv(vec);
        }
    }
    return vec;
}

extern "C" void fn_1_7328(void *self)
{
    std::vector<HuVec2f_s> *vec = (std::vector<HuVec2f_s> *)self;
    HuVec2f_s *begin = vec->m_data;
    HuVec2f_s *end = vec->m_data + vec->m_size;
    while (end > begin) {
        end--;
    }
    vec->m_size = 0;
}

#ifndef TARGET_PC
extern "C" void fn_1_7384(void)
{
    s_delay58 = 0x78;
    s_delay5C = 0xD2;
    s_delay60 = 0xD2;
}
#endif

} // namespace m501

namespace std {
template <>
vector<HuVec2f_s, allocator<HuVec2f_s> >::~vector()
{
    m501::fn_1_729C(this, 0);
}
}
