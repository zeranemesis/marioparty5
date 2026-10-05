#include "dolphin/types.h"

struct Tube {
    u8 m_pad[0x28];
    f32 m_f28;
    f32 m_f2C;
    f32 m_f30;
    f32 m_f34;
};

extern "C" {

void fn_1_1262C(Tube *src, f32 *dst)
{
    dst[0] = src->m_f28;
    dst[1] = src->m_f2C;
    dst[2] = src->m_f30;
    dst[3] = src->m_f34;
}

}