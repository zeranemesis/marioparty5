#include "REL/m501Dll.h"
#include "datanum/m501.h"

struct Vec3 {
    f32 x, y, z;
};

extern "C" {
void PSVECNormalize(Vec3 *src, Vec3 *dst);
void PSVECScale(Vec3 *src, Vec3 *dst, f32 scale);
void PSVECAdd(Vec3 *a, Vec3 *b, Vec3 *dst);

void Hu3DLighInit(void);
s32 Hu3DGLightCreateV(Vec3 *pos, Vec3 *dir, GXColor *color);
void Hu3DGLightInfinitytSet(s16 lightId);
void Hu3DShadowCreate(f32 fov, f32 near, f32 far);
void Hu3DShadowTPLvlSet(f32 tpLvl);
void Hu3DShadowPosSet(Vec3 *camPos, Vec3 *camUp, Vec3 *camTarget);
void Hu3DShadowColSet(u8 r, u8 g, u8 b);
void Hu3DModelShadowMapSet(s16 modelId);
s16 Hu3DMotionShapeIDGet(s16 modelId);
void Hu3DModelAttrSet(s16 modelId, u32 attr);
void Hu3DMotionShapeSet(s16 modelId, s16 motId);
/* these two take the computed time unconverted, so their prototype is double */
void Hu3DMotionShapeTimeSet(s16 modelId, f64 time);
void Hu3DMotionShapeStartEndSet(s16 modelId, f64 start, f64 end);
void Hu3DCameraMotionOff(s16 modelId);
void __dl__FPv(void *ptr);
double cos(f64 x);
}

namespace m501 {

static Vec3 s_lightPos = {9654.0f, 8151.0f, 2774.0f};
static GXColor s_lightColor = {0xFF, 0xFF, 0xFF, 0xFF};

class CStage : public CGameObject, public CSingleton {
public:
    CStage();
    virtual ~CStage();
    virtual void on_new_mode(s32 mode);
    virtual void on_update();
    void setAisuMotion();

private:
    CModel m_stage;
    CModel m_border;
    CModel m_camera;
    CModel m_aisu;
    s32 m_timer;
    f32 m_angle;
};

CStage::CStage() :
    m_stage((u32)M501_HSF_stage),
    m_border((u32)M501_HSF_smoke),
    m_camera((u32)M501_HSF_stage_border),
    m_aisu((u32)M501_HSF_stage_shadow)
{
    Vec3 lightDir;
    Vec3 shadowPos;
    Vec3 up;
    Vec3 pos;

    m_timer = 0;

    Hu3DLighInit();
    PSVECNormalize(&s_lightPos, &lightDir);
    PSVECScale(&lightDir, &lightDir, -1.0f);
    Hu3DGLightInfinitytSet(Hu3DGLightCreateV(&s_lightPos, &lightDir, &s_lightColor));
    Hu3DShadowCreate(30.0f, 20.0f, 25000.0f);
    Hu3DShadowTPLvlSet(0.4f);

    pos.x = 1578.489f;
    pos.y = 5060.02f;
    pos.z = 414.244f;
    PSVECScale(&lightDir, &shadowPos, -3000.0f);
    PSVECAdd(&shadowPos, &pos, &shadowPos);
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    Hu3DShadowPosSet(&shadowPos, &up, &pos);
    Hu3DShadowColSet(0, 0, 0x40);

    Hu3DModelShadowMapSet(m_border.m_id);
    Hu3DModelAttrSet(m_stage.m_id, 0x40000040);
    Hu3DMotionShapeSet(m_stage.m_id, Hu3DMotionShapeIDGet(m_stage.m_id));
    Hu3DMotionShapeTimeSet(m_stage.m_id, 1 - 0x7FFF8000);
    Hu3DMotionShapeStartEndSet(m_stage.m_id, 0xF0 - 0x7FFF8000, 1 - 0x7FFF8000);

    m_angle = 0.0f;
}

extern "C" void *fn_1_AFD8(void *self, s16 flag)
{
    if (self != (void *)0) {
        if (CGameMaster::stageInstance == (CStage *)0) {
            __assert_fail("ms_pointer != 0", "singleton.h", 0x3D);
        }
        CGameMaster::stageInstance = (CStage *)0;
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void CStage::setAisuMotion()
{
    if (m_timer <= 0) {
        m_timer = 0x25;
        Hu3DMotionShapeSet(m_aisu.m_id, Hu3DMotionShapeIDGet(m_aisu.m_id));
    }
}

void CStage::on_update()
{
    s32 expired = 0;

    if (m_timer > 0) {
        m_timer = m_timer - 1;
        if (m_timer == 0) {
            expired = 1;
        }
    }

    Hu3DMtx *mtx = m_aisu.getMatrix();
    mtx->m[2][1] = (f32)(0.1 * (100.0 * cos(m_angle)));
    m_angle = (f32)((f64)m_angle + 0.017453292519943295);
    if (m_angle > 3.141592653589793) {
        m_angle = (f32)((f64)m_angle - 6.283185307179586);
    }
}

void CStage::on_new_mode(s32 mode)
{
    m_execMode &= ~(1 << mode);

    switch (mode) {
    case 0:
        break;
    case 2:
        Hu3DCameraMotionOff(m_camera.m_id);
        break;
    case 5:
        break;
    }
}

CStage::~CStage() {}
}