// m512Dll/REL/m512Dll/gamemaster
// CGameMaster: the per-minigame driver object. It owns the sub-object at
// +0x20, the timer at +0x358, the record/score pointers and the frame tick,
// and runs the whole minigame as one long function, fn_1_635C.
//
// The layout and the 12-word vtable match m517Dll's gamemaster unit: the
// constructor stores &vtable at +0x04 and &vtable[6] at +0x20.
//
// Declared locally rather than through the game headers: those reach MSL
// math.h, whose `extern inline sqrtf` makes MWCC emit _half$localstatic3 and
// _three$localstatic4, 16 bytes of .rodata the target object does not have.

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

// Head of M512Score (see REL/m512Dll/score.cpp); only ->timer at 0x10 is read.
struct M512ScoreHead {
    s32 unk00;
    s32 unk04;
    void *vtable;
    s32 bitmask;
    void *timer;     // 0x10
};

struct CGameMaster {
    void *m_f0;        // 0x00
    void *m_vtbl;      // 0x04
    char m_f8[0x18];   // 0x08
    void *m_vtbl2;     // 0x20
    char m_f24[0x334]; // 0x24
    void *m_f358;      // 0x358
    void *m_f35C;        // 0x35C score window
    s32 m_f360;        // 0x360 frame tick
    u32 m_f364;        // 0x364 current record
    u8 m_f368;         // 0x368 wide-record flag
    u8 m_f369;
    char m_f36A[6];
};

// -------------------------------------------------------------------- extern

extern "C" {

void *__nw__FUl(u32 size);

CGameMaster *__ct__Q24m51211CGameMasterFv(CGameMaster *self);
CGameMaster *__dt__Q24m51211CGameMasterFv(CGameMaster *self, s16 flag);

void fn_1_1C48(s32 mode);
bool fn_1_1CE0(void);
void fn_1_1D6C(void);
void fn_1_1DE0(void);
void fn_1_1F40(void *self);
void fn_1_20CC(void *self, s32 flag);
void fn_1_2704(void);
void fn_1_27BC(void);
bool fn_1_2874(void);
void fn_1_2948(void);
void fn_1_2B14(s32 rec);
void fn_1_2BEC(void);
void fn_1_2C44(void);
void fn_1_47E4(void *self);
void fn_1_4A08(void);

void fn_1_5268(void *self, u32 mode);
void fn_1_52E4(void *self, s32 mode);
void fn_1_5398(void *self, s32 mode);
void fn_1_5428(void *self, s32 mode);

void fn_1_5AA0(void *self, s32 mode);
void fn_1_5B68(void *self, s32 mode);
void fn_1_5BC4(void *self, s32 a, s32 b, s32 c);
void fn_1_5CBC(void *self, s32 mode);
void fn_1_5DB0(void *self);
bool fn_1_5E18(void *self);

void *fn_1_67FC(void *self);
void *fn_1_78A4(void *self);
void *fn_1_8144(void *self);
void fn_1_9104(void);
void fn_1_91AC(void);
void *fn_1_9250(void *self, s32 idx);
void fn_1_9B84(void);
bool fn_1_9EAC(u8 flag);
bool fn_1_A040(u8 flag);
void fn_1_A160(void);
void fn_1_A1F0(void);
u32 fn_1_A23C(void);

void *fn_1_D178(void *self, s32 rec, u8 wideF);
bool fn_1_D60C(void *self);
void *fn_1_D804(void *self, u8 recordF);
void *fn_1_E698(void *self);
void *fn_1_10750(void *self);

void MgTimerRecordSet(void *timer, s32 rec);

void Hu3DBGColorSet(s32 prio, s32 r, s32 b);
void *HuAudBGMPlay(s32 id);
void HuAudSStreamFadeOut(void *stream, s32 fade);
void HuAudJinglePlay(s16 id);

s32 GWMgRecordGet(s32 idx);
void GWMgRecordSet(s32 idx, s32 rec);

s32 _CheckFlag(u32 flag);
s32 OSGetTick(void);

int printf(const char *fmt, ...);

}

// ------------------------------------------------------------------ forward

// Constructor for the three file-scope CTimes; run from .ctors.  MWCC folds
// it into __sinit, which is what the original's static-initialiser does.
class CTime {
public:
    CTime(int v) : m_time(v) {}
    s32 m_time;
};

extern "C" void fn_1_61B0(CGameMaster *self);
extern "C" void fn_1_6324(CGameMaster *self);
extern "C" void fn_1_635C(CGameMaster *self);
extern "C" void fn_1_650C(CGameMaster *self);
extern "C" bool fn_1_65E4(CGameMaster *self);
extern "C" bool fn_1_66A0(CGameMaster *self);
extern "C" void fn_1_66FC(CGameMaster *self);
extern "C" void fn_1_6700(CGameMaster *self);
extern "C" void fn_1_6758(CGameMaster *self);
extern "C" void fn_1_6760(CGameMaster *self);

// ---------------------------------------------------------------------- data

char lbl_1_data_210[] = "m512::CGameMaster::~CGameMaster()";
char lbl_1_data_232[] = "\0%s\n\0";

// One 12-word object: vtable[0..5] for CGameMaster, vtable[6..9] for the
// sub-object at +0x20, then a two-entry table.
void *lbl_1_data_238[] = {
    0, 0,
    (void *)__dt__Q24m51211CGameMasterFv, (void *)fn_1_61B0, (void *)fn_1_6324, (void *)fn_1_6700,
    0, 0,
    (void *)fn_1_6760, (void *)fn_1_6758,
    (void *)fn_1_66FC, (void *)fn_1_635C
};

// The three CTimes and the seven spare words are the module's own globals.
// __sinit (the .ctors entry) stores 120/210/210 into the first three and is
// the target's fn_1_6724 byte for byte, but MWCC always emits it after every
// other function, so it lands at 0x6C0 where the original has it at 0x6B0 --
// the two 8-byte adjustor thunks that follow it there are compiler-generated
// in the original.  objdiff therefore cannot pair the two.
CTime lbl_1_bss_9F0(120);
CTime lbl_1_bss_9F4(210);
CTime lbl_1_bss_9F8(210);
s32 lbl_1_bss_9FC;
s32 lbl_1_bss_A00;
s32 lbl_1_bss_A04;
s32 lbl_1_bss_A08;
s32 lbl_1_bss_A0C;
s32 lbl_1_bss_A10;
s32 lbl_1_bss_A14;

// ---------------------------------------------------------------- functions

// MWCC inlines a definition into its callers inside the same translation
// unit; every function here is referenced through the vtable array or is a
// loop-call target, and all of them survive out of line without the pragma.

extern "C" CGameMaster *__ct__Q24m51211CGameMasterFv(CGameMaster *self)
{
    fn_1_1F40(self);
    fn_1_5268(&self->m_vtbl2, 0x8000);
    self->m_vtbl = lbl_1_data_238;
    self->m_vtbl2 = &lbl_1_data_238[6];
    fn_1_5AA0(&self->m_f358, 0);
    self->m_f364 = GWMgRecordGet(3);
    self->m_f368 = (_CheckFlag(0x30002) != 0);
    return self;
}

extern "C" CGameMaster *__dt__Q24m51211CGameMasterFv(CGameMaster *self, s16 flag)
{
    if (self != 0) {
        self->m_vtbl = lbl_1_data_238;
        self->m_vtbl2 = &lbl_1_data_238[6];
        printf(lbl_1_data_232, lbl_1_data_210);
        fn_1_5B68(&self->m_f358, -1);
        fn_1_52E4(&self->m_vtbl2, 0);
        fn_1_20CC(self, 0);
        if (flag > 0) {
            fn_1_47E4(self);
        }
    }
    return self;
}

// NONMATCHING: only the register numbers differ.  The four score objects
// land in r28..r26 where the original uses r29..r27: MWCC ranks locals by
// live-range length, and holding the fourth one across the store to
// self->m_f35C pushes it above the three shorter-lived ones.  Every opcode,
// the frame and the size are the original's.
extern "C" void fn_1_61B0(CGameMaster *self)
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
    Hu3DBGColorSet(0x1D, 0x80, 0xFD);

    a = __nw__FUl(0x20);
    if (a) {
        a = fn_1_8144(a);
    }
    b = __nw__FUl(0x48);
    if (b) {
        b = fn_1_67FC(b);
    }
    c = __nw__FUl(0xF4);
    if (c) {
        c = fn_1_D804(c, self->m_f368);
    }
    d = __nw__FUl(0x1C);
    if (d) {
        d = fn_1_D178(d, self->m_f364, self->m_f368);
    }
    self->m_f35C = d;

    fn_1_9104();
    for (i = 0; i < 4; i++) {
        e = __nw__FUl(0xB0);
        if (e) {
            e = fn_1_9250(e, i);
        }
    }
    f = __nw__FUl(0x10);
    if (f) {
        f = fn_1_78A4(f);
    }
    g = __nw__FUl(0x38);
    if (g) {
        g = fn_1_10750(g);
    }
    h = __nw__FUl(0x20);
    if (h) {
        h = fn_1_E698(h);
    }
}

extern "C" void fn_1_6324(CGameMaster *self)
{
    fn_1_5398(&self->m_vtbl2, 0);
    fn_1_1D6C();
    fn_1_9B84();
}

extern "C" void fn_1_635C(CGameMaster *self)
{
    bool mode;
    bool flag;
    s32 jingle;
    void *bgm;

    fn_1_1C48(0);
    fn_1_2BEC();
    bgm = HuAudBGMPlay(0x55);
    while (!fn_1_1CE0()) {
        fn_1_5428(&self->m_vtbl2, 0);
    }
    fn_1_1C48(1);
    fn_1_2704();
    while (!fn_1_2874()) {
        fn_1_5428(&self->m_vtbl2, 0);
    }
    mode = 0;
    fn_1_1C48(2);
    self->m_f360 = 0;
    fn_1_5BC4(&self->m_f358, 0x708, 0, 0);
    while (!fn_1_65E4(self)) {
        if (!mode && self->m_f360 > 0x3F48) {
            fn_1_5CBC(&self->m_f358, 1);
            mode = 1;
        }
        fn_1_5428(&self->m_vtbl2, 0);
    }
    flag = fn_1_A040(self->m_f368);
    if (mode) {
        fn_1_5DB0(&self->m_f358);
    }
    fn_1_1C48(3);
    HuAudSStreamFadeOut(bgm, 0x64);
    fn_1_650C(self);
    fn_1_1C48(4);
    if (self->m_f368 == 0) {
        if (flag) {
            jingle = 0x7B;
        } else {
            jingle = 0x79;
        }
        HuAudJinglePlay(jingle);
        if (flag) {
            fn_1_A160();
        } else {
            fn_1_A1F0();
        }
        fn_1_2948();
        while (!fn_1_2874()) {
            fn_1_5428(&self->m_vtbl2, 0);
        }
    }
    fn_1_2C44();
    for (;;) {
        fn_1_5428(&self->m_vtbl2, -1);
    }
}

extern "C" void fn_1_650C(CGameMaster *self)
{
    M512ScoreHead *score;
    s32 rec;

    fn_1_27BC();
    while (!fn_1_2874()) {
        fn_1_5428(&self->m_vtbl2, 0);
    }
    if (fn_1_66A0(self)) {
        if (self->m_f368 == 0) {
            rec = self->m_f364;
            score = (M512ScoreHead *)self->m_f35C;
            MgTimerRecordSet(score->timer, rec);
            if (_CheckFlag(0x1000F) == 0) {
                GWMgRecordSet(3, self->m_f364);
                fn_1_2B14(self->m_f364);
                while (!fn_1_2874()) {
                    fn_1_5428(&self->m_vtbl2, 0);
                }
            }
        }
    }
}

extern "C" bool fn_1_65E4(CGameMaster *self)
{
    bool wide;
    bool narrow;

    self->m_f360 += 1;
    if (self->m_f368 != 0) {
        wide = 1;
        if (!fn_1_D60C(self->m_f35C) && !fn_1_9EAC(self->m_f368)) {
            wide = 0;
        }
        return wide;
    }
    narrow = 1;
    if (!fn_1_5E18(&self->m_f358) && !fn_1_9EAC(self->m_f368)) {
        narrow = 0;
    }
    return narrow;
}

extern "C" bool fn_1_66A0(CGameMaster *self)
{
    bool ret;
    u32 rec;

    ret = 0;
    rec = fn_1_A23C();
    if (self->m_f364 > rec) {
        self->m_f364 = rec;
        ret = 1;
    }
    return ret;
}

extern "C" void fn_1_66FC(CGameMaster *self)
{
}

extern "C" void fn_1_6700(CGameMaster *self)
{
    fn_1_91AC();
    fn_1_1DE0();
}

extern "C" void fn_1_6758(CGameMaster *self)
{
    __asm {
        subi r3, r3, 0x20
        b      fn_1_635C
    }
}

extern "C" void fn_1_6760(CGameMaster *self)
{
    __asm {
        subi r3, r3, 0x20
        b      __dt__Q24m51211CGameMasterFv
    }
}
