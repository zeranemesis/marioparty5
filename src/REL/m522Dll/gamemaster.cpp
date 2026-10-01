#include "REL/m522Dll.h"

class CTime {
public:
    CTime(int v) : m_time(v) {}
    int m_time;
};

static CTime lbl_1_bss_A10(120);
static CTime lbl_1_bss_A14(210);
static CTime lbl_1_bss_A18(210);

extern "C" {
/* CMinigameCore, the base of the mode controller. */
void fn_1_8CE0(void *self);
void fn_1_8E6C(void *self, s32 flag);
void fn_1_B584(void *self);
/* Members of the sub object living at 0x20 of the mode controller. */
void fn_1_BDE0(M522_CORE *core, s32 arg);
void fn_1_BE5C(M522_CORE *core, s32 arg);
void fn_1_BF10(M522_CORE *core, s32 arg);
void fn_1_BFA0(M522_CORE *core, s32 arg);
/* The message queue at 0x358. */
void fn_1_C618(M522_QUEUE *queue, s32 arg);
void fn_1_C6E0(M522_QUEUE *queue, s32 arg);
void fn_1_C73C(M522_QUEUE *queue, s32 a, s32 b, s32 c);
void fn_1_C834(M522_QUEUE *queue, s32 arg);
void fn_1_C928(M522_QUEUE *queue);
u8 fn_1_C990(M522_QUEUE *queue);
/* The data selector heap at 0x35C. */
void fn_1_7B00(M522_HEAPSEL *heap, u32 size);
void fn_1_7D04(M522_HEAPSEL *heap, s32 arg);
/* Helpers. */
void *fn_1_689C(u32 size);
M522_MSG *fn_1_DEA0(M522_MSG *self, M522_HEAPSEL *heap);
M522_UNK48 *fn_1_E85C(M522_UNK48 *self);
M522_UNKAC *fn_1_D1B4(M522_UNKAC *self);
void fn_1_6A70(const char *cond, const char *file, s32 line);
u8 fn_1_8A80(void);
u8 fn_1_9614(void);
u8 fn_1_F194(s32 ptr);
s32 fn_1_F1F8(s32 ptr);
s32 fn_1_FA28(void);
void fn_1_89E8(s32 mode);
void fn_1_998C(void);
void fn_1_99E4(void);
void fn_1_94A4(void);
void fn_1_955C(void);
void fn_1_96E8(void);
void fn_1_B7A8(void);
void fn_1_F604(void);
void fn_1_F6E4(void);
void fn_1_F76C(void);
void fn_1_F800(void);
void fn_1_F894(void);
void fn_1_F904(void);
void fn_1_F974(void);
void fn_1_8B0C(void);
void fn_1_8B80(void);

void fn_1_CDE0(M522_GAMEMASTER *self);
void *fn_1_CC64(M522_GAMEMASTER *self, s16 flag);
void *fn_1_CBEC(M522_GAMEMASTER *self);
void fn_1_CCFC(M522_GAMEMASTER *self);
void fn_1_CDA8(M522_GAMEMASTER *self);
void fn_1_D094(M522_GAMEMASTER *self);
void fn_1_D0C0(void);
u8 fn_1_CF6C(M522_GAMEMASTER *self);
void fn_1_D118(M522_GAMEMASTER *self);
void fn_1_D120(M522_GAMEMASTER *self, s16 flag);
}

typedef void (*M522_VFN)(void *);

static M522_VFN lbl_1_data_3DC[] = {
    (M522_VFN)0,
    (M522_VFN)0,
    (M522_VFN)fn_1_CC64,
    (M522_VFN)fn_1_CCFC,
    (M522_VFN)fn_1_CDA8,
    (M522_VFN)fn_1_D0C0,
    (M522_VFN)0,
    (M522_VFN)0,
    (M522_VFN)fn_1_D120,
    (M522_VFN)fn_1_D118,
    (M522_VFN)fn_1_D094,
    (M522_VFN)fn_1_CDE0,
    (M522_VFN)0,
};

void *fn_1_CBEC(M522_GAMEMASTER *self)
{
    fn_1_8CE0(self);
    fn_1_BDE0(&self->m_core, 0x8000);
    self->m_vtable = (void *)lbl_1_data_3DC;
    self->m_core.m_vtable = (void *)&lbl_1_data_3DC[6];
    fn_1_C618(&self->m_queue, 0);
    fn_1_7B00(&self->m_heapSel, 0x1000);
    return self;
}

void *fn_1_CC64(M522_GAMEMASTER *self, s16 flag)
{
    if (self != (M522_GAMEMASTER *)0) {
        self->m_vtable = (void *)lbl_1_data_3DC;
        self->m_core.m_vtable = (void *)&lbl_1_data_3DC[6];
        fn_1_7D04(&self->m_heapSel, -1);
        fn_1_C6E0(&self->m_queue, -1);
        fn_1_BE5C(&self->m_core, 0);
        fn_1_8E6C(self, 0);
        if (flag > 0) {
            fn_1_B584(self);
        }
    }
    return self;
}

void fn_1_CCFC(M522_GAMEMASTER *self)
{
    M522_MSG *msg;
    M522_UNK48 *unk48;
    M522_UNKAC *unkAC;

    OSGetTick();
    fn_1_B7A8();
    msg = (M522_MSG *)fn_1_689C(0x1C);
    if (msg != (M522_MSG *)0) {
        msg = fn_1_DEA0(msg, &self->m_heapSel);
    }
    unk48 = (M522_UNK48 *)fn_1_689C(0x48);
    if (unk48 != (M522_UNK48 *)0) {
        unk48 = fn_1_E85C(unk48);
    }
    unkAC = (M522_UNKAC *)fn_1_689C(0xAC);
    if (unkAC != (M522_UNKAC *)0) {
        unkAC = fn_1_D1B4(unkAC);
    }
    fn_1_F604();
}

void fn_1_CDA8(M522_GAMEMASTER *self)
{
    fn_1_BF10(&self->m_core, 0);
    fn_1_F6E4();
    fn_1_8B0C();
}

void fn_1_CDE0(M522_GAMEMASTER *self)
{
    s32 streamNo;
    s32 i;

    fn_1_89E8(0);
    fn_1_998C();
    do {
        fn_1_BFA0(&self->m_core, 0);
    } while (WipeCheck());
    HuAudFXPlay(0x749);
    do {
        fn_1_BFA0(&self->m_core, 0);
    } while (!fn_1_8A80());
    streamNo = HuAudBGMPlay(0x3A);
    fn_1_89E8(1);
    fn_1_F76C();
    fn_1_94A4();
    do {
        fn_1_BFA0(&self->m_core, 0);
    } while (!fn_1_9614());
    fn_1_89E8(2);
    fn_1_F800();
    fn_1_C73C(&self->m_queue, 0x708, 0, 0);
    fn_1_C834(&self->m_queue, 1);
    do {
        fn_1_BFA0(&self->m_core, 0);
    } while (fn_1_CF6C(self));
    fn_1_C928(&self->m_queue);
    fn_1_89E8(3);
    fn_1_F894();
    fn_1_955C();
    HuAudSStreamFadeOut(streamNo, 0x64);
    for (i = 0; i < 0xB4; i++) {
        fn_1_BFA0(&self->m_core, 0);
    }
    fn_1_89E8(4);
    fn_1_F904();
    fn_1_96E8();
    HuAudJinglePlay(0x79);
    do {
        fn_1_BFA0(&self->m_core, 0);
    } while (!fn_1_9614());
    fn_1_99E4();
    for (;;) {
        fn_1_BFA0(&self->m_core, -1);
    }
}

u8 fn_1_CF6C(M522_GAMEMASTER *self)
{
    s32 mode;
    u8 ret;

    if (lbl_1_bss_18 == 0) {
        fn_1_6A70("ms_pointer != 0", "singleton.h", 0x27);
    }
    if (!fn_1_F194(lbl_1_bss_18)) {
        return 0;
    }
    mode = fn_1_FA28();
    if (mode < 0) {
        return 0;
    }
    if (mode == 0) {
        return 1;
    }
    ret = 1;
    if (!fn_1_C990(&self->m_queue)) {
        if (lbl_1_bss_18 == 0) {
            fn_1_6A70("ms_pointer != 0", "singleton.h", 0x27);
        }
        if (fn_1_F1F8(lbl_1_bss_18) > 0) {
            ret = 0;
        }
    }
    return ret;
}

void fn_1_D094(M522_GAMEMASTER *self)
{
    fn_1_C990(&self->m_queue);
}

void fn_1_D0C0(void)
{
    fn_1_F974();
    fn_1_8B80();
}

void fn_1_D118(M522_GAMEMASTER *self)
{
    fn_1_CDE0((M522_GAMEMASTER *)((u8 *)self - 0x20));
}

void fn_1_D120(M522_GAMEMASTER *self, s16 flag)
{
    fn_1_CC64((M522_GAMEMASTER *)((u8 *)self - 0x20), flag);
}