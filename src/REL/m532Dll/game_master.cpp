// m532Dll "game_master": the top-level object of the minigame. It owns the
// sub-thread that runs the whole game flow (fn_1_8C78), the timer and the
// manager singletons (map, referee, score, ...) that it creates in setup.
#define _MATH_H
#include <stddef.h>
#include "REL/m532Dll/game_master.h"
extern "C" {
#include "game/audio.h"
}

class CFrame {
public:
    CFrame(s32 frame) { m_frame = frame; }
    s32 m_frame;
};

extern "C" {

char lbl_1_data_320[] = "ms_pointer != 0";
char lbl_1_data_330[] = "singleton.h";
char lbl_1_data_33C[] = "game_result != PLAYING_GAME";
char lbl_1_data_358[] = "game_master.cpp";

void *fn_1_8A2C(M532GM *self);
void *fn_1_8AA4(M532GM *self, s16 flag);
void fn_1_8B3C(M532GM *self, void *objman);
void fn_1_8C40(M532GM *self);
void fn_1_8C78(M532GM *self);
s32 fn_1_8F60(M532GM *self);
void fn_1_9584(M532GM *self);
void fn_1_95B0(M532GM *self);
void *fn_1_9630(M532SubThread *sub, s16 flag);
void fn_1_9628(M532SubThread *sub);

void *lbl_1_data_368[] = {
    0,
    0,
    (void *)fn_1_8AA4,
    (void *)fn_1_8B3C,
    (void *)fn_1_8C40,
    (void *)fn_1_95B0,
    0,
    0,
    (void *)fn_1_9630,
    (void *)fn_1_9628,
    (void *)fn_1_9584,
    (void *)fn_1_8C78,
};

CFrame lbl_1_bss_A10(120);
CFrame lbl_1_bss_A14(210);
CFrame lbl_1_bss_A18(210);
CFrame lbl_1_bss_A1C(300);
CFrame lbl_1_bss_A20(60);
void *lbl_1_bss_A24;
void *lbl_1_bss_A28;
void *lbl_1_bss_A2C; /* camera singleton (camera.cpp) */

extern void *lbl_1_bss_8; /* map singleton */

void fn_1_1D1C(const char *expr, const char *file, s32 line);
void *fn_1_1B48(u32 size);
void fn_1_3F00(void *self);
void fn_1_7324(void *thread, u32 stackSize);
void fn_1_73A0(void *thread, s16 flag);
void fn_1_7B5C(void *timerObj, s32 type);
void fn_1_7C24(void *timerObj, s32 flag);
void fn_1_2DAC(void *vec, u32 size);
void fn_1_2FB0(void *vec, s32 flag);
void fn_1_408C(void *master, s32 flag);
void fn_1_67A4(void *p);
void fn_1_69C8(u32 seed);
void *fn_1_E54C(void *self, void *vec);
void *fn_1_0(void *self);
void *fn_1_96EC(void *self);
void *fn_1_9D7C(void *self);
void *fn_1_CB30(void *self);
void *fn_1_AFC0(void *self);
void fn_1_EF70(void);
void fn_1_7454(void *thread, s32 arg);
void fn_1_74E4(void *thread, s32 arg);
void fn_1_F1F8(void);
void fn_1_3DB8(void);
void fn_1_3C94(s32 state);
void fn_1_4BAC(void);
u8 fn_1_B59C(void *referee);
u8 fn_1_3D2C(void);
void fn_1_46C4(void);
u8 fn_1_4834(void);
f32 fn_1_B7A8(void *referee);
void fn_1_B600(void *referee);
u8 fn_1_B670(void *referee);
void fn_1_B6CC(void *referee, s32 arg);
void fn_1_B914(void *referee);
void fn_1_7E6C(void *timerObj);
u8 fn_1_7ED4(void *timerObj);
void fn_1_7C80(void *timerObj, s32 a, s32 b, s32 c);
void fn_1_7D78(void *timerObj, s32 a);
void fn_1_F5C4(void);
void fn_1_477C(void);
void fn_1_FB08(s32 arg);
void fn_1_4908(void);
void fn_1_4C04(void);
void fn_1_A518(void *map);
u8 fn_1_A5A4(void *map, f32 val);
void fn_1_A55C(void *map);
f32 fn_1_6F88(f32 a, f32 b);
void fn_1_F2F8(s32 arg);
s32 fn_1_F38C(f32 *out);
u8 fn_1_F514(void);
void fn_1_F6E0(s32 arg);
u8 fn_1_FA78(void);
void fn_1_F9C8(void);
void fn_1_CEB4(void *score, s32 arg);
void fn_1_CF2C(void *score);
s32 fn_1_CF64(void *score, s32 arg);
void fn_1_FD80(void);
void fn_1_3E28(void);

// Singleton<T>::GetInstance(): asserts the pointer is set, then returns it.
static inline void *M532_GET(void *p)
{
    if (p == 0) {
        fn_1_1D1C(lbl_1_data_320, lbl_1_data_330, 0x27);
    }
    return p;
}

void *fn_1_8A2C(M532GM *self)
{
    fn_1_3F00(self);
    fn_1_7324(&self->thread, 0x8000);
    self->base.vtbl = (M532MasterVtbl *)lbl_1_data_368;
    self->thread.vtbl = (M532ThreadVtbl *)(lbl_1_data_368 + 6);
    fn_1_7B5C(&self->timer, 0);
    fn_1_2DAC(&self->vec, 0x1000);
    return self;
}

void *fn_1_8AA4(M532GM *self, s16 flag)
{
    if (self != 0) {
        self->base.vtbl = (M532MasterVtbl *)lbl_1_data_368;
        self->thread.vtbl = (M532ThreadVtbl *)(lbl_1_data_368 + 6);
        fn_1_2FB0(&self->vec, -1);
        fn_1_7C24(&self->timer, -1);
        fn_1_73A0(&self->thread, 0);
        fn_1_408C(self, 0);
        if (flag > 0) {
            fn_1_67A4(self);
        }
    }
    return self;
}

// NONMATCHING: the original keeps each constructed object in its own callee-saved register
void fn_1_8B3C(M532GM *self, void *objman)
{
    void *p;

    fn_1_69C8(OSGetTick());
    p = fn_1_1B48(M532_ALLOC_SIZE(0x14));
    if (p != 0) {
        p = fn_1_E54C(p, &self->vec);
    }
    p = fn_1_1B48(M532_ALLOC_SIZE(0x28));
    if (p != 0) {
        p = fn_1_0(p);
    }
    p = fn_1_1B48(M532_ALLOC_SIZE(0xA8));
    if (p != 0) {
        p = fn_1_96EC(p);
    }
    p = fn_1_1B48(M532_ALLOC_SIZE(0x48));
    if (p != 0) {
        p = fn_1_9D7C(p);
    }
    p = fn_1_1B48(M532_ALLOC_SIZE(0x1C4));
    if (p != 0) {
        p = fn_1_CB30(p);
    }
    p = fn_1_1B48(M532_ALLOC_SIZE(0x64));
    if (p != 0) {
        p = fn_1_AFC0(p);
    }
    fn_1_EF70();
}

void fn_1_8C40(M532GM *self)
{
    fn_1_7454(&self->thread, 0);
    fn_1_F1F8();
    fn_1_3DB8();
}

// NONMATCHING: the original inlines Singleton::GetInstance with extra register copies
void fn_1_8C78(M532GM *self)
{
    s32 stream;
    s32 i;
    s32 result;
    u8 win;
    s16 jingle;

    fn_1_3C94(0);
    fn_1_4BAC();
    while (fn_1_B59C(M532_GET(lbl_1_bss_A24)) == 0) {
        fn_1_74E4(&self->thread, 0);
    }
    stream = HuAudBGMPlay(0x32);
    while (fn_1_3D2C() == 0) {
        fn_1_74E4(&self->thread, 0);
    }
    fn_1_3C94(1);
    fn_1_46C4();
    i = 0;
    while (fn_1_4834() == 0) {
        if (i == lbl_1_bss_A10.m_frame - (s32)(60.0f * fn_1_B7A8(M532_GET(lbl_1_bss_A24)))) {
            fn_1_B600(M532_GET(lbl_1_bss_A24));
        }
        fn_1_74E4(&self->thread, 0);
        i++;
    }
    result = fn_1_8F60(self);
    fn_1_7E6C(&self->timer);
    fn_1_3C94(3);
    fn_1_F5C4();
    fn_1_477C();
    HuAudSStreamFadeOut(stream, 0x64);
    for (i = 0; i < 0xB4; i++) {
        fn_1_74E4(&self->thread, 0);
    }
    fn_1_3C94(4);
    fn_1_B6CC(M532_GET(lbl_1_bss_A24), result);
    fn_1_FB08(result);
    fn_1_4908();
    win = (result == 2 || result == 3);
    if (win) {
        jingle = 0x7B;
    } else {
        jingle = 0x79;
    }
    HuAudJinglePlay(jingle);
    while (fn_1_4834() == 0) {
        fn_1_74E4(&self->thread, 0);
    }
    fn_1_4C04();
    while (1) {
        fn_1_74E4(&self->thread, -1);
    }
}

// NONMATCHING: the original inlines Singleton::GetInstance with extra register copies
s32 fn_1_8F60(M532GM *self)
{
    f32 fval;
    s32 temp;
    s32 result;
    s32 round;
    s32 i;

    result = -1;
    fn_1_3C94(2);
    for (round = 1; round <= 5;) {
        if (round > 1) {
            fn_1_B600(M532_GET(lbl_1_bss_A24));
            while (fn_1_B670(M532_GET(lbl_1_bss_A24)) == 0) {
                fn_1_74E4(&self->thread, 0);
            }
        }
        fn_1_7C80(&self->timer, lbl_1_bss_A1C.m_frame, 0, 0);
        fn_1_7D78(&self->timer, 1);
        fn_1_A518(M532_GET(lbl_1_bss_8));
        temp = (s32)fn_1_6F88((f32)lbl_1_bss_A20.m_frame, (f32)lbl_1_bss_A1C.m_frame);
        fn_1_F2F8(temp);
        while (fn_1_7ED4(&self->timer) == 0 && fn_1_FA78() == 0) {
            fn_1_74E4(&self->thread, 0);
        }
        fn_1_7E6C(&self->timer);
        fval = 0.0f;
        temp = fn_1_F38C(&fval);
        while (fn_1_A5A4(M532_GET(lbl_1_bss_8), fval) == 0) {
            fn_1_74E4(&self->thread, 0);
        }
        fn_1_A55C(M532_GET(lbl_1_bss_8));
        switch (temp) {
        case 0:
            fn_1_CEB4(M532_GET(lbl_1_bss_A28), 0);
            break;
        case 1:
            fn_1_CEB4(M532_GET(lbl_1_bss_A28), 1);
            break;
        }
        fn_1_F6E0(temp);
        while (fn_1_F514() == 0) {
            fn_1_74E4(&self->thread, 0);
        }
        fn_1_B6CC(M532_GET(lbl_1_bss_A24), temp);
        fn_1_CF2C(M532_GET(lbl_1_bss_A28));
        while (fn_1_FA78() == 0 && fn_1_B670(M532_GET(lbl_1_bss_A24)) == 0) {
            fn_1_74E4(&self->thread, 0);
        }
        for (i = 0; i < 0x3C; i++) {
            fn_1_74E4(&self->thread, 0);
        }
        fn_1_B914(M532_GET(lbl_1_bss_A24));
        result = fn_1_CF64(M532_GET(lbl_1_bss_A28), round);
        if (result != -1 || round >= 5) {
            break;
        }
        fn_1_F9C8();
        while (fn_1_FA78() == 0) {
            fn_1_74E4(&self->thread, 0);
        }
        round++;
    }
    if (result == -1) {
        fn_1_1D1C(lbl_1_data_33C, lbl_1_data_358, 0xAF);
    }
    return result;
}

void fn_1_9584(M532GM *self)
{
    fn_1_7ED4(&self->timer);
}

void fn_1_95B0(M532GM *self)
{
    fn_1_FD80();
    fn_1_3E28();
}

// NONMATCHING: the original is a compiler-generated 8-byte this-adjusting thunk
void fn_1_9628(M532SubThread *sub)
{
    fn_1_8C78((M532GM *)((u8 *)sub - offsetof(M532GM, thread)));
}

// NONMATCHING: the original is a compiler-generated 8-byte this-adjusting thunk
void *fn_1_9630(M532SubThread *sub, s16 flag)
{
    return fn_1_8AA4((M532GM *)((u8 *)sub - offsetof(M532GM, thread)), flag);
}

}
