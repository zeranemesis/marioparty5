#include "types.h"
#include "dol2asm.h"

// m536::CGameMaster is written as plain C with an explicit vtable: in C++ the
// real class shape emits a vtable whose second group is unreachable from
// source, and the two 8-byte thunks at the end cannot be expressed as C.
extern "C" {

struct M536GameMaster;

void *__ct__Q24m53611CGameMasterFv(M536GameMaster *self);
void *__dt__Q24m53611CGameMasterFv(M536GameMaster *self, s16 flag);
void fn_1_6184(void);
void fn_1_62D0(M536GameMaster *self);
void fn_1_6308(M536GameMaster *self);
u8 fn_1_64B8(M536GameMaster *self);
void fn_1_6524(void);
void fn_1_6528(void);
void fn_1_6550(void);
void fn_1_6584(M536GameMaster *self);
void fn_1_658C(M536GameMaster *self);

// The primary base occupies 0x00..0x1F and the second base 0x20..; the only
// members this unit touches are the two vtable slots, the object handle at
// 0x358 and the frame counter at 0x35C.
struct M536GameMaster {
    s32 m_unk0;
    void **m_vptr;
    s8 m_unk8[0x18];
    void **m_vptr2;
    s8 m_unk24[0x334];
    s32 m_obj;
    s32 m_counter;
};

int printf(const char *fmt, ...);
u32 OSGetTick(void);

void *fn_1_0(u32 size);
void fn_1_1C48(s32 state);
u8 fn_1_1CE0(void);
void fn_1_1D6C(void);
void fn_1_1DE0(void);
void fn_1_1F40(M536GameMaster *self);
void fn_1_20CC(M536GameMaster *self, s32 flag);
void fn_1_2704(void);
void fn_1_27BC(void);
u8 fn_1_2874(void);
void fn_1_2BEC(void);
void fn_1_2C44(void);
void fn_1_47E4(M536GameMaster *self);
void fn_1_4A08(void);
void fn_1_5268(void *self, s16 flag);
void fn_1_52E4(void *self, s32 flag);
void fn_1_5398(void *self, s32 flag);
void fn_1_5428(void *self, s32 flag);
void fn_1_5AA0(s32 *obj, s32 value);
void fn_1_5B68(s32 *obj, s32 value);
void fn_1_5BC4(s32 *obj, s32 a, s32 b, s32 c);
void fn_1_5CBC(s32 *obj, s32 flag);
void fn_1_5DB0(s32 *obj);
u8 fn_1_5E18(s32 *obj);
void *fn_1_6628(void *self);
void *fn_1_73BC(void *self);
void *fn_1_8B94(void *self);
void *fn_1_906C(void *self);
void fn_1_A3A0(void);
void *fn_1_A4A8(void *self, s32 index);
void fn_1_AB28(void);
u8 fn_1_AC40(void);
void fn_1_ACD4(void);
u8 fn_1_AFF0(void);
void *fn_1_D6FC(void *self);
void *fn_1_E24C(void *self);
void fn_1_E8D4(void);
void *fn_1_F670(void *self);

s32 HuAudBGMPlay(s32 seq);
void HuAudSStreamFadeOut(s32 stream, s32 fade);
void HuAudJinglePlay(s16 jingle);

// ---------------------------------------------------------------- data

char lbl_1_data_210[] = "m536::CGameMaster::~CGameMaster()";
char lbl_1_data_232[] = "%s\n";
void *lbl_1_data_238[12] = {
    NULL, NULL, (void *)__dt__Q24m53611CGameMasterFv, (void *)fn_1_6184,
    (void *)fn_1_62D0, (void *)fn_1_6528,
    NULL, NULL, (void *)fn_1_658C, (void *)fn_1_6584,
    (void *)fn_1_6524, (void *)fn_1_6308,
};

s32 lbl_1_bss_9F0;
s32 lbl_1_bss_9F4;
s32 lbl_1_bss_9F8;
s32 lbl_1_bss_9FC;
s32 lbl_1_bss_A00;
s32 lbl_1_bss_A04;
s32 lbl_1_bss_A08;
s32 lbl_1_bss_A0C;
s32 lbl_1_bss_A10;
s32 lbl_1_bss_A14;

// ---------------------------------------------------------------- unit

void *__ct__Q24m53611CGameMasterFv(M536GameMaster *self)
{
    fn_1_1F40(self);
    fn_1_5268(&self->m_vptr2, 0x8000);
    self->m_vptr = lbl_1_data_238;
    self->m_vptr2 = &lbl_1_data_238[6];
    fn_1_5AA0(&self->m_obj, 0);
    return self;
}

void *__dt__Q24m53611CGameMasterFv(M536GameMaster *self, s16 flag)
{
    if (self) {
        self->m_vptr = lbl_1_data_238;
        self->m_vptr2 = &lbl_1_data_238[6];
        printf(lbl_1_data_232, lbl_1_data_210);
        fn_1_5B68(&self->m_obj, -1);
        fn_1_52E4(&self->m_vptr2, 0);
        fn_1_20CC(self, 0);

        if (flag > 0) {
            fn_1_47E4(self);
        }
    }
    return self;
}

void fn_1_6184(void)
{
    s32 i;
    void *a;
    void *b;
    void *c;
    void *d;
    void *e;
    void *f;
    void *g;
    void *h;

    OSGetTick();
    fn_1_4A08();

    a = fn_1_0(0x10);
    if (a) {
        a = fn_1_73BC(a);
    }

    b = fn_1_0(0x2C);
    if (b) {
        b = fn_1_8B94(b);
    }

    c = fn_1_0(0x4C);
    if (c) {
        c = fn_1_906C(c);
    }

    d = fn_1_0(0x4C);
    if (d) {
        d = fn_1_6628(d);
    }

    e = fn_1_0(0x94);
    if (e) {
        e = fn_1_D6FC(e);
    }

    f = fn_1_0(0x34);
    if (f) {
        f = fn_1_E24C(f);
    }

    for (i = 0; i < 4; i++) {
        g = fn_1_0(0x98);
        if (g) {
            g = fn_1_A4A8(g, i);
        }
    }

    h = fn_1_0(0x58);
    if (h) {
        h = fn_1_F670(h);
    }
}

void fn_1_62D0(M536GameMaster *self)
{
    fn_1_5398(&self->m_vptr2, 0);
    fn_1_1D6C();
    fn_1_AB28();
}

void fn_1_6308(M536GameMaster *self)
{
    u8 fastForward;
    s32 wait;
    s16 jingle;
    s32 stream;

    fn_1_1C48(0);
    fn_1_2BEC();
    stream = HuAudBGMPlay(0x54);

    while (fn_1_1CE0() == 0) {
        fn_1_5428(&self->m_vptr2, 0);
    }

    fn_1_1C48(1);
    fn_1_2704();

    while (fn_1_2874() == 0) {
        fn_1_5428(&self->m_vptr2, 0);
    }

    fastForward = 0;
    fn_1_1C48(2);
    self->m_counter = 0;
    fn_1_5BC4(&self->m_obj, 0x708, 0, 0);

    while (fn_1_64B8(self) == 0) {
        if (fastForward == 0) {
            if (self->m_counter > 0x3F48) {
                fn_1_5CBC(&self->m_obj, 1);
                fastForward = 1;
            }
        }

        fn_1_5428(&self->m_vptr2, 0);
    }

    if (fastForward != 0) {
        fn_1_5DB0(&self->m_obj);
    }

    fn_1_ACD4();
    fn_1_1C48(3);
    HuAudSStreamFadeOut(stream, 0x64);
    fn_1_27BC();

    for (wait = 0; wait < 0xB4; wait++) {
        fn_1_5428(&self->m_vptr2, 0);
    }

    fn_1_1C48(4);

    if (fn_1_AFF0() != 0) {
        jingle = 0x7B;
    } else {
        jingle = 0x79;
    }

    HuAudJinglePlay(jingle);

    for (wait = 0; wait < lbl_1_bss_9F8; wait++) {
        fn_1_5428(&self->m_vptr2, 0);
    }

    fn_1_2C44();

    for (;;) {
        fn_1_5428(&self->m_vptr2, -1);
    }
}

u8 fn_1_64B8(M536GameMaster *self)
{
    u8 ret;

    self->m_counter = self->m_counter + 1;
    ret = 1;

    if (fn_1_5E18(&self->m_obj) == 0) {
        if (fn_1_AC40() == 0) {
            ret = 0;
        }
    }
    return ret;
}

void fn_1_6524(void)
{
}

void fn_1_6528(void)
{
    fn_1_E8D4();
    fn_1_A3A0();
    fn_1_1DE0();
}

void fn_1_6550(void)
{
    lbl_1_bss_9F0 = 0x78;
    lbl_1_bss_9F4 = 0xD2;
    lbl_1_bss_9F8 = 0xD2;
}

#pragma noinline
void fn_1_6584(M536GameMaster *self)
{
    __asm {
        subi r3, r3, 0x20
        b fn_1_6308
    }
}

#pragma noinline
void fn_1_658C(M536GameMaster *self)
{
    __asm {
        subi r3, r3, 0x20
        b __dt__Q24m53611CGameMasterFv
    }
}

}

SECTION_CTORS void *const _ctors_game_master = (void *)fn_1_6550;