// m513Dll input: player input sources. A base object with a virtual update()
// that fills an input state, a pad reader and a CPU (random) player.
#include "REL/m513Dll.h"
#include "game/gamework.h"
#include "game/pad.h"
extern "C" {
#include "game/frand.h"
}
#include "math.h"

// Output of update(): stick x/y, stick magnitude and the pressed buttons.
typedef struct M513InputState_s {
    f32 x;
    f32 y;
    f32 mag;
    u32 btn;
} M513InputState;

struct M513Input_s;
typedef struct M513InputVtbl_s {
    void *rtti[2];
    struct M513Input_s *(*dtor)(struct M513Input_s *self, s16 flag);
    void (*update)(struct M513Input_s *self, M513InputState *out);
} M513InputVtbl;

typedef struct M513Input_s {
    M513InputVtbl *vtbl;
    M513InputState st;
    s32 level;     // CPU strength
    s32 playerNo;
    union {
        s32 padNo; // pad reader
        f32 base;  // CPU: base wait
    } u;
    f32 range;     // CPU: random extra wait
    s32 timer;     // CPU: frames to the next press
} M513Input;

extern "C" {

void fn_1_72AC(void *work, Mtx mtx);

M513Input *fn_1_6618(M513Input *self, s16 flag);
M513Input *fn_1_6B60(M513Input *self, s16 flag);
void fn_1_6BCC(M513Input *self, M513InputState *out);
M513Input *fn_1_6CA0(M513Input *self, s16 flag);
void fn_1_6670(M513Input *self, M513InputState *out);

f32 lbl_1_data_248[4] = { 9.5f, 5.5f, 4.5f, 2.5f };
M513InputVtbl lbl_1_data_258 = { { 0, 0 }, fn_1_6B60, fn_1_6BCC };
M513InputVtbl lbl_1_data_268 = { { 0, 0 }, fn_1_6CA0, fn_1_6670 };
M513InputVtbl lbl_1_data_278 = { { 0, 0 }, fn_1_6618, 0 };

void fn_1_6578(M513Input *self)
{
    self->st.x = 0.0f;
    self->st.y = 0.0f;
    self->st.btn = 0;
    self->vtbl->update(self, &self->st);
}

void fn_1_65E0(M513Input *self, s32 playerNo)
{
    self->vtbl = &lbl_1_data_278;
    self->vtbl = &lbl_1_data_268;
    self->playerNo = playerNo;
    self->u.padNo = GwPlayerConf[playerNo].padNo;
}

M513Input *fn_1_6618(M513Input *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_278;
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// Pad reader: map the analog stick onto a unit disc.
void fn_1_6670(M513Input *self, M513InputState *out)
{
    s32 pad = self->u.padNo;
    f32 sy = 0.017857144f * (f32)HuPadStkY[pad];
    f64 x = sin(M_PI * (f64)(0.017857144f * (f32)HuPadStkX[pad]) * 0.5);
    f64 y = sin((M_PI * (f64)sy) / -2.0);
    f64 mag = sqrt((x * x) + (y * y));
    f64 inv;

    if ((f32)mag > 1.0f) {
        inv = 1.0 / mag;
        x *= inv;
        y *= inv;
        mag = 1.0;
    }
    out->x = x;
    out->y = y;
    out->mag = mag;
    out->btn = HuPadBtnDown[pad];
}

// CPU player.
M513Input *fn_1_69EC(M513Input *self, s32 playerNo, s32 level)
{
    self->vtbl = &lbl_1_data_278;
    self->vtbl = &lbl_1_data_258;
    self->playerNo = playerNo;
    self->level = level;
    self->range = ((3.0f - (f32)level) / 3.0f) * (3.0f * (0.000015258789f * (f32)(u16)frand()));
    self->u.base = lbl_1_data_248[level];
    self->timer = self->range + (self->u.base + (2.5f * (0.000015258789f * (f32)(u16)frand())));
    return self;
}

M513Input *fn_1_6B60(M513Input *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_258;
        if (self != 0) {
            self->vtbl = &lbl_1_data_278;
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_6BCC(M513Input *self, M513InputState *out)
{
    s32 unused;
    s8 unused2;
    s32 t;

    out->btn = 0;
    unused2 = 0;
    t = self->timer;
    self->timer = t - 1;
    if (t < 0) {
        out->btn |= 0x100;
        self->timer = self->range + (self->u.base + (2.5f * (0.000015258789f * (f32)(u16)frand())));
    }
}

M513Input *fn_1_6CA0(M513Input *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_268;
        if (self != 0) {
            self->vtbl = &lbl_1_data_278;
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// Particle hook of the torch flame (the torch itself lives in taimatsu.cpp).
void fn_1_6D0C(HU3DMODEL *model, HU3DPARTICLE *particle, Mtx mtx)
{
    void *work = particle->work;

    fn_1_72AC(work, mtx);
}

}
