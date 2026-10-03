#include "dolphin/types.h"
#include "dolphin/mtx.h"

class CMT19937 {
public:
    CMT19937(int seed = 5489) { setSeed(seed); }
    void setSeed(int seed);
    u32 irandom();
    double frandom();
    double randomMantissa();

private:
    u32 m_state[624];
    s32 m_index;
};

extern "C" {
void *fn_1_2B68(void *self);
void *fn_1_7C8(void *self, u32 val);
void *fn_1_4D3C(void *self, s32 val);
void setSeed__8CMT19937Fi(void *self, s32 seed);
void fn_1_4E04(void *self, s32 val);
void *fn_1_9E8(void *self, u32 val);
void *fn_1_2CF4(void *self, u32 val);
void *fn_1_2944(void *self);
void *fn_1_CED0(void *self);
void *fn_1_104(s32 size);
void fn_1_EB14(void *self);
void *fn_1_DEEC(void *self);
void *fn_1_D98C(void *self);
void *fn_1_5A7C(void *self);
void fn_1_0(const char *str1, const char *str2, s32 line);
void fn_1_3888(u32 val);
void fn_1_381C(void);
void fn_1_F1FC(void *self, s32 idx);
void fn_1_FF60(void);
void *fn_1_3334(void);
void *fn_1_34A4(void);
u32 irandom__7CRandomFv(void);
void fn_1_C224(void *self, u32 val);
void *fn_1_E1A0(void *self);
void fn_1_FA3C(void);
void fn_1_FDB0(void);
void HuAudFXPlay(s32 id);
u32 HuAudBGMPlay(s32 id);
void HuAudSStreamFadeOut(s32 id, s32 time);
void fn_1_33EC(void);
s32 fn_1_3874(void);
void fn_1_3578(void);
void HuAudJinglePlay(s32 id);
void fn_1_CE8(void *self, s32 val);
void *fn_1_C85C(void *self);
void fn_1_50B4(void *self);
void *fn_1_F864(void *self);
void fn_1_F0F4(void);
void *fn_1_3A20(void);
void fn_1_504C(void *self);
void HuAudFXPlayVol(s32 id, s32 vol);
u32 OSGetTick(void);
void setSeed__7CRandomFi(u32 seed);

void *fn_1_BEC(void *self, u32 val);
void fn_1_39AC(void);
void fn_1_F7E0(void);
void fn_1_4E60(void *self, u32 a, u32 b, u32 c);
void fn_1_4F58(void *self, u32 val);
void fn_1_21C(void *self);

extern u32 lbl_1_bss_30;
extern u32 lbl_1_bss_50[2];
u32 lbl_1_bss_38;
u32 lbl_1_bss_3C;
u32 lbl_1_bss_40;
u32 lbl_1_bss_44;
u32 lbl_1_bss_48[2];
u32 lbl_1_data_32C2[] = {
    0x6D735F70,
    0x6F696E74,
    0x65722021,
    0x3D203000,
    0x0000,
};
}

class CGameMaster {
public:
    CGameMaster() { init(); }
    void init() {
        fn_1_2B68(this);
        fn_1_7C8((void *)((u32)this + 0x20), 1);
        fn_1_4D3C(&m_rng, 0);
        setSeed__8CMT19937Fi(&m_rng, 0x1571);
    }
    virtual ~CGameMaster();
    virtual void fn_1_C3E8();
    virtual void fn_1_C4A8();
    virtual void fn_1_C4E0();
    virtual void fn_1_C7D0(s32 arg);
    virtual s32 fn_1_C85C();
    virtual void fn_1_C8B8() {}
    virtual void fn_1_C8BC();
    static void fn_1_C8E0();
    void fn_1_C914() { fn_1_C4E0(); }
    void fn_1_C91C() { this->~CGameMaster(); }

private:
    u32 m_vptr1;
    u32 m_vptr2;
    u32 m_field_0x8[0xD2]; // 0x8 - 0x354
    CMT19937 m_rng;        // 0x358
    u32 m_field_0x35C;
    u32 m_field_0x360;
};

CGameMaster::~CGameMaster()
{
    fn_1_4E04(&m_rng, -1);
    fn_1_9E8((void *)((u32)this + 0x20), 0);
    fn_1_2CF4(this, 0);
}

void CGameMaster::fn_1_C3E8()
{
    u32 tick = OSGetTick();
    setSeed__7CRandomFi(tick);
    void *r30 = fn_1_104(0x14);
    if (r30) fn_1_EB14(r30);
    void *r29 = fn_1_104(0x48);
    if (r29) fn_1_DEEC(r29);
    for (s32 i = 0; i < 4; i++) {
        void *r28 = fn_1_104(0x7C);
        if (r28) fn_1_F1FC(r28, i);
    }
    fn_1_FF60();
}

void CGameMaster::fn_1_C4A8()
{
    fn_1_BEC((void *)((u32)this + 0x20), 0);
    fn_1_39AC();
    fn_1_F7E0();
}

void CGameMaster::fn_1_C4E0()
{
    void *r30 = fn_1_104(0xCC);
    if (r30) fn_1_CED0(r30);
    void *r29 = fn_1_104(0x28);
    if (r29) fn_1_D98C(r29);
    void *r28 = fn_1_104(0x418);
    if (r28) fn_1_5A7C(r28);
    void *r22 = fn_1_104(1);
    if (r22) {
        if (!lbl_1_bss_30) {
            fn_1_0("ms_pointer == 0", "singleton.h", 0x35);
        }
        lbl_1_bss_30 = (u32)r22;
    }
    fn_1_3888(0);
    fn_1_381C();
    HuAudFXPlay(0x692);
    for (s32 i = 0; i < 0xB4; i++) {
        fn_1_CE8((void *)((u32)this + 0x20), 0);
    }
    fn_1_3888(1);
    u32 r19 = HuAudBGMPlay(0x3E);
    fn_1_3334();
    while (!fn_1_34A4()) {
        fn_1_CE8((void *)((u32)this + 0x20), 0);
    }
    fn_1_4E60(&m_rng, 0xA8C, 0, 0);
    fn_1_4F58(&m_rng, 1);
    s32 r23 = 0;
    s32 r25 = 0;
    s32 r26 = 0;
    while (true) {
        s32 r0 = r25 * 0x3C;
        if (r26 < r0) {
            Vec vec;
            if (!lbl_1_bss_30) {
                fn_1_0((const char *)lbl_1_data_32C2, "singleton.h", 0x27);
            }
            u32 r0_2 = irandom__7CRandomFv();
            fn_1_C224((void *)lbl_1_bss_30, r0_2);
            fn_1_E1A0(&vec);
            HuAudFXPlayVol(0x68F, (s32)vec.x);
            fn_1_FA3C();
            r23++;
            r25 += 2;
        } else {
            fn_1_CE8((void *)((u32)this + 0x20), 0);
            if (fn_1_C85C()) break;
        }
    }
    fn_1_504C(&m_rng);
    fn_1_3888(3);
    HuAudSStreamFadeOut(r19, 0x64);
    fn_1_33EC();
    for (s32 i = 0; i < 0xB4; i++) {
        fn_1_CE8((void *)((u32)this + 0x20), -1);
    }
    fn_1_3888(4);
    fn_1_FDB0();
    s32 r20 = fn_1_3874() ? 0x7B : 0x79;
    HuAudJinglePlay((s16)r20);
    fn_1_3578();
    while (true) {
        fn_1_CE8((void *)((u32)this + 0x20), -1);
        if (fn_1_34A4()) break;
    }
    fn_1_3874();
    while (true) {
        fn_1_CE8((void *)((u32)this + 0x20), -1);
    }
}

void CGameMaster::fn_1_C7D0(s32 arg)
{
    if (!this) return;
    if (!lbl_1_bss_30) {
        fn_1_0("ms_pointer == 0", "singleton.h", 0x3D);
    }
    lbl_1_bss_30 = 0;
    if (arg != 0) {
        fn_1_21C(this);
    }
}

s32 CGameMaster::fn_1_C85C()
{
    s32 ret = 1;
    fn_1_50B4((void *)((u32)this + 0x358));
    if (ret) {
        fn_1_F864((void *)((u32)this + 0x358));
        if (ret) {
            ret = 0;
        }
    }
    return ret;
}

void CGameMaster::fn_1_C8BC()
{
    fn_1_F0F4();
    fn_1_3A20();
}

void CGameMaster::fn_1_C8E0()
{
    *(u32 *)lbl_1_bss_38 = 0x78;
    *(u32 *)lbl_1_bss_3C = 0xD2;
    *(u32 *)lbl_1_bss_40 = 0xD2;
}

// C-style wrapper for the constructor (called from objectsetup.cpp)
extern "C" void *fn_1_C2D0(void *self)
{
    CGameMaster *obj = (CGameMaster *)self;
    obj->init();
    return self;
}