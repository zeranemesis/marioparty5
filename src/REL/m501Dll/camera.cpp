#include "REL/m501Dll.h"
#include "datanum/m501.h"
#include "game/hu3d.h"
#include "game/data.h"

extern "C" {
void __dl__FPv(void *ptr);
void PlayerShow__Q24m5017CPlayerFv(void);
}

namespace m501 {

static inline void setVec(Vec *v, f64 x, f64 y, f64 z)
{
    v->x = x;
    v->y = y;
    v->z = z;
}

CCameraObj::CCameraObj()
{
#ifdef TARGET_PC
    CGameMaster::cameraInstance = this;
#endif
    m_proc = &CCameraObj::proc_idle;
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 20.0f, 100.0f, 50000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
}

/* out-of-line destructor of the singleton base, with the delete flag */
extern "C" void *fn_1_9C8C(void *self, s16 flag)
{
    if (self != 0) {
        CGameMaster::cameraInstance = (CCameraObj *)0;
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

extern "C" void *fn_1_9CE8(void *self, s16 flag)
{
    if (self != 0) {
        ((CLinkNode *)self)->~CLinkNode();
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void CCameraObj::set_focus_player(Vec *pos)
{
    CCamera cam;
    Vec offset;

    getView(&m_viewFrom);
    struct CamRaw {
        u32 w[9];
    };
    *(CamRaw *)&cam = *(CamRaw *)(CCamera *)this;
    setVec(&offset, pos->x, 120.00001f + pos->y, pos->z);
    cam.getOffset(&offset);
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = 1400.0f;
    cam.applyOffsetLocal(&offset);
    cam.getView(&m_viewTo);
    m_timer = 0;
    m_proc = &CCameraObj::proc_focus_player;
}

void CCameraObj::proc_focus_player()
{
    int frames = 30;
    f32 t = (f32)m_timer / (f32)frames;
    if (t > 1.0f) {
        t = 1.0f;
    }
    t = (3.0f - 2.0f * t) * (t * t);
    lerpView(&m_viewFrom, &m_viewTo, t);
    setPos(1);
    m_timer++;
}

void CCameraObj::proc_null()
{
}

void CCameraObj::proc_move_up()
{
    f32 moveT;
    f32 showT;

    if (m_timer >= 0x7F && m_timer <= 0xB9) {
        moveT = (f32)(m_timer - 0x7F) / 58.0f;
    }
    if (m_timer == 0xD0) {
        PlayerShow__Q24m5017CPlayerFv();
    }
    if (m_timer >= 0xD0 && m_timer <= 0xF0) {
        showT = (f32)(m_timer - 0xD0) * 0.03125f;
    }
    if (m_timer > 0xF0) {
        m_proc = &CCameraObj::proc_idle;
        m_execMode &= ~1;
        getPos(1);
    }
    m_timer++;
}

void CCameraObj::proc_idle()
{
    Vec *pos = &m_pos;
    Vec *up = &m_up;
    Vec *target = &m_target;
    Hu3DCameraPosSetV(1, pos, up, target);
}

void CCameraObj::on_new_mode(int mode)
{
    switch (mode) {
    case 0:
        m_timer = 0;
        m_proc = &CCameraObj::proc_move_up;
        m_model = Hu3DModelCameraCreate(Hu3DMotionCreate(HuDataSelHeapReadNum(DATANUM(DATA_m501, 3), 0x10000000, HEAP_MODEL)), 1);
        Hu3DCameraMotionStart(m_model, 1);
        return;
    case 1:
        Hu3DModelKill(m_model);
        m_execMode &= ~(1 << mode);
        return;
    case 2:
        m_execMode &= ~(1 << mode);
        return;
    case 3:
        m_execMode &= ~(1 << mode);
        return;
    case 4:
        m_execMode &= ~(1 << mode);
        return;
    }
}

void CCameraObj::on_update()
{
    if (m_proc != 0) {
        (this->*m_proc)();
    }
}

CCameraObj::~CCameraObj()
{
}

} // namespace m501
