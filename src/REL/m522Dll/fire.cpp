#include "REL/m522Dll.h"

#include "game/hu3d.h"
#include "dolphin/os.h"

extern "C" {
void fn_1_9FE4(void *model, const char *name);
void fn_1_4A4(void *model);
void fn_1_510(void *model);
void fn_1_9CDC(void *model, const void *rot);
void DCFlushRangeNoSync(void *addr, u32 size);
f32 sin(f32);
f32 cos(f32);
}

typedef struct FireParticle_s {
    f32 m_accum;   // 0x0
    f32 m_delta;   // 0x4
    f32 m_field8;  // 0x8
    f32 m_fieldC;  // 0xC
} FireParticle;

static f32 lbl_1_bss_0;

void fn_1_460(FireParticle *particle)
{
    particle->m_field8 = 0.0f;
}

void fn_1_614(FireParticle *particle)
{
    particle->m_accum += particle->m_delta;
}

void fn_1_4934(void)
{
    lbl_1_bss_0 = 0.011805556f;
}

void fn_1_C2C(FireParticle *dst, const FireParticle *src)
{
    dst->m_accum = src->m_accum;
    dst->m_delta = src->m_delta;
    dst->m_field8 = src->m_field8;
    dst->m_fieldC = src->m_fieldC;
}

void fn_1_474(void *model)
{
    fn_1_9FE4(model, "sun01");
}

void fn_1_428(void *model)
{
    fn_1_4A4(model);
    fn_1_510(model);
}