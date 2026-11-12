#include "REL/m537Dll.h"

// m537::CGameMaster is written as plain C inside extern "C" with an explicit
// 12-word vtable. The object carries two vtable slots, so the last two vtable
// entries are this-adjusting thunks; on GameCube they are inline __asm blocks,
// on PC they are plain C functions that step back to the master object.

extern "C" {


void fn_1_1C48(s32 arg);
u8 fn_1_1CE0(void);
void fn_1_1D6C(void);
void fn_1_1DE0(void);
M537Main *fn_1_1F40(M537Main *self);
M537Main *fn_1_20CC(M537Main *self, s16 freeIt);
void fn_1_2704(void);
void fn_1_27BC(void);
u8 fn_1_2874(void);
void fn_1_2BEC(void);
void fn_1_2C44(void);
void fn_1_47E4(void *self);
void fn_1_4A08(u32 seed);
M537CThread *fn_1_5040(M537CThread *self, u32 stackSize);
void *fn_1_50BC(void *self, s16 freeIt);
s32 fn_1_5170(M537CThread *self, s32 value);
s32 fn_1_5200(M537CThread *self, s32 value);
MGTIMER **fn_1_5878(MGTIMER **out, s32 type);
MGTIMER **fn_1_5940(MGTIMER **timer, s16 freeIt);
void fn_1_599C(MGTIMER **timer, s32 maxTime, s32 endTime, s32 recordTime);
void fn_1_5A94(MGTIMER **timer, s32 offType);
void fn_1_5B88(MGTIMER **timer);
u8 fn_1_5BF0(MGTIMER **timer);
M537Camera *fn_1_8964(M537Camera *self);
M537Stage *fn_1_9290(M537Stage *self);
void fn_1_99D0(void);
void fn_1_9ACC(void);
void fn_1_9B64(void);
void fn_1_9BFC(void);
void fn_1_9D74(void);
void fn_1_9EE4(void);
s32 fn_1_A0C4(void);
M537Center *fn_1_FF4C(M537Center *self);
M537Center *fn_1_10230(M537Center *self, s16 freeIt);
void fn_1_10590(void);

s32 lbl_1_bss_A00;
s32 lbl_1_bss_A04;
s32 lbl_1_bss_A08;
M537Center *lbl_1_bss_A0C;
M537Camera *lbl_1_bss_A10;
M537Stage *lbl_1_bss_A14;

// Shadow light placement: position, up vector and target.
struct M537Shadow {
    Vec v[3];
};

struct M537ShadowFov {
    f32 v[2];
};

extern const M537ShadowFov lbl_1_rodata_1F8 = {{288.0f, 0.0f}};
extern const M537Shadow lbl_1_rodata_200 = {
    {{0.0f, 8000.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
};

/* Preserve the original constant pool and load order instead of folding literals. */
extern const f32 lbl_1_rodata_224 = 30.0f;
extern const f32 lbl_1_rodata_228 = 6800.0f;
extern const f32 lbl_1_rodata_22C = 7200.0f;
extern const f32 lbl_1_rodata_230[2] = {0.4f, 0.0f};

void *fn_1_83CC(M537GameMaster *self);
void *fn_1_8438(M537GameMaster *self, s16 arg);
void fn_1_84C4(void);
void fn_1_8604(M537GameMaster *self);
void fn_1_8640(M537GameMaster *self);
u8 fn_1_87C4(M537GameMaster *self);
void fn_1_881C(M537GameMaster *self);
void fn_1_8848(void);
void fn_1_888C(void);
#ifdef TARGET_PC
void fn_1_88C0(M537CThread *thread);
void *fn_1_88C8(M537CThread *thread, s16 flag);
#else
void fn_1_88C0(void);
void fn_1_88C8(void);
#endif

void *lbl_1_data_320[] = {
    0,
    0,
    (void *)fn_1_8438,
    (void *)fn_1_84C4,
    (void *)fn_1_8604,
    (void *)fn_1_8848,
    0,
    0,
    (void *)fn_1_88C8,
    (void *)fn_1_88C0,
    (void *)fn_1_881C,
    (void *)fn_1_8640,
};

void *fn_1_83CC(M537GameMaster *self)
{
    fn_1_1F40(&self->m_core);
    fn_1_5040(&self->m_thread, 0x8000);
    self->m_core.m_vptr = lbl_1_data_320;
    self->m_thread.m_vptr = &lbl_1_data_320[6];
    fn_1_5878(&self->m_timer, 0);
    return self;
}

void *fn_1_8438(M537GameMaster *self, s16 arg)
{
    if (self != 0) {
        self->m_core.m_vptr = lbl_1_data_320;
        self->m_thread.m_vptr = &lbl_1_data_320[6];
        fn_1_5940(&self->m_timer, -1);
        fn_1_50BC(&self->m_thread, 0);
        fn_1_20CC(&self->m_core, 0);
        if (arg > 0) {
            fn_1_47E4(self);
        }
    }
    return self;
}

void fn_1_84C4(void)
{
    M537Shadow sp;
    M537Stage *stage;
    M537Camera *camera;
    M537Center *center;

    fn_1_4A08(OSGetTick());
    sp = lbl_1_rodata_200;
    Hu3DShadowCreate(*(volatile const f32 *)&lbl_1_rodata_224, *(volatile const f32 *)&lbl_1_rodata_228, *(volatile const f32 *)&lbl_1_rodata_22C);
    Hu3DShadowPosSet(&sp.v[0], &sp.v[1], &sp.v[2]);
    Hu3DShadowSizeSet(0xF0);
    Hu3DShadowTPLvlSet(*(volatile const f32 *)&lbl_1_rodata_230);
#ifdef TARGET_PC
    stage = (M537Stage *)fn_1_0(sizeof(M537Stage));
#else
    stage = (M537Stage *)fn_1_0(0x34);
#endif
    if (stage != 0) {
        stage = fn_1_9290(stage);
    }
#ifdef TARGET_PC
    camera = (M537Camera *)fn_1_0(sizeof(M537Camera));
#else
    camera = (M537Camera *)fn_1_0(0xB8);
#endif
    if (camera != 0) {
        camera = fn_1_8964(camera);
    }
    fn_1_99D0();
#ifdef TARGET_PC
    center = (M537Center *)fn_1_0(sizeof(M537Center));
#else
    center = (M537Center *)fn_1_0(0x38);
#endif
    if (center != 0) {
        center = fn_1_FF4C(center);
    }
}

void fn_1_8604(M537GameMaster *self)
{
    fn_1_5170(&self->m_thread, 0);
    fn_1_1D6C();
    fn_1_9ACC();
    fn_1_10590();
}

#pragma dont_inline on
void fn_1_8640(M537GameMaster *self)
{
    s32 var1;
    s32 var2;
    s16 var3;
    s32 var4;

    fn_1_1C48(0);
    fn_1_2BEC();
    HuAudFXPlay(0x7DC);
    while (fn_1_1CE0() == 0) {
        fn_1_5200(&self->m_thread, 0);
    }
    var4 = HuAudBGMPlay(0x36);
    fn_1_1C48(1);
    fn_1_2704();
    while (fn_1_2874() == 0) {
        fn_1_5200(&self->m_thread, 0);
    }
    fn_1_1C48(2);
    fn_1_9B64();
    fn_1_599C(&self->m_timer, 0xE10, 0, 0);
    fn_1_5A94(&self->m_timer, 1);
    while (fn_1_87C4(self) == 0) {
        fn_1_5200(&self->m_thread, 0);
    }
    fn_1_5B88(&self->m_timer);
    fn_1_1C48(3);
    fn_1_9BFC();
    fn_1_27BC();
    HuAudSStreamFadeOut(var4, 0x64);
    for (var1 = 0; var1 < 0xB4; var1++) {
        fn_1_5200(&self->m_thread, 0);
    }
    fn_1_1C48(4);
    fn_1_9D74();
    if (fn_1_A0C4() >= 4) {
        var3 = 0x7B;
    } else {
        var3 = 0x79;
    }
    HuAudJinglePlay((s32)var3);
    for (var2 = 0; var2 < lbl_1_bss_A08; var2++) {
        fn_1_5200(&self->m_thread, 0);
    }
    fn_1_2C44();
    while (1) {
        fn_1_5200(&self->m_thread, -1);
    }
}

u8 fn_1_87C4(M537GameMaster *self)
{
    u8 res;

    res = 1;
    if ((fn_1_5BF0(&self->m_timer) == 0) && (fn_1_A0C4() > 1)) {
        res = 0;
    }
    return res;
}

void fn_1_881C(M537GameMaster *self)
{
    fn_1_5BF0(&self->m_timer);
}

void fn_1_8848(void)
{
    M537Center *var;

    fn_1_9EE4();
    fn_1_1DE0();
    var = lbl_1_bss_A0C;
    fn_1_10230(var, 1);
}

void fn_1_888C(void)
{
    lbl_1_bss_A00 = 0x78;
    lbl_1_bss_A04 = 0xD2;
    lbl_1_bss_A08 = 0xD2;
}

#ifdef TARGET_PC
void fn_1_88C0(M537CThread *thread)
{
    fn_1_8640((M537GameMaster *)((u8 *)thread - offsetof(M537GameMaster, m_thread)));
}

void *fn_1_88C8(M537CThread *thread, s16 flag)
{
    return fn_1_8438((M537GameMaster *)((u8 *)thread - offsetof(M537GameMaster, m_thread)), flag);
}
#else
#pragma dont_inline on
void fn_1_88C0(void)
{
    __asm {
        subi r3, r3, 0x20
        b      fn_1_8640
    }
}
#pragma dont_inline reset

#pragma dont_inline on
void fn_1_88C8(void)
{
    __asm {
        subi r3, r3, 0x20
        b      fn_1_8438
    }
}
#pragma dont_inline reset
#endif
}

#ifdef TARGET_PC
// The GC build links fn_1_888C through the .ctors section; PC runs it at load time.
static struct M537GameMasterInit {
    M537GameMasterInit() { fn_1_888C(); }
} lbl_1_gameMasterInit;
#else
__declspec(section ".ctors")
static void (*const gameMasterInit)(void) = fn_1_888C;
#endif
