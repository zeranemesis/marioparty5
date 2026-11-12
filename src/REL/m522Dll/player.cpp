#define _MATH_H
#include "REL/m522Dll.h"
#include "REL/m522Vec.h"

extern "C" {
#include "game/audio.h"
#include "game/data.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/mg/actman.h"
}

#ifdef TARGET_PC
#include <math.h>
#include <string.h>
#else
extern "C" double sin(double x);
extern "C" double cos(double x);
extern "C" double atan2(double y, double x);
extern "C" double fmod(double x, double y);
inline float atan2f(float y, float x)
{
    return (float)atan2(y, x);
}
extern "C" double __fabs(double x);
inline double fabs(double f)
{
    return __fabs(f);
}
extern "C" void *memcpy(void *dst, const void *src, unsigned long n);
extern "C" void *memmove(void *dst, const void *src, unsigned long n);
#endif

#ifdef __MWERKS__
#define PLAYER_RODATA __declspec(section ".rodata")
#else
#define PLAYER_RODATA
#endif

#define HV(p) ((HuVecF *)(p))

/* Vector setter taking doubles, as the original's Vec constructor does. */
static inline void M522VecSet(M522_VEC3 *v, f64 x, f64 y, f64 z)
{
    v->x = x;
    v->y = y;
    v->z = z;
}

static inline f32 M522Mag(M522_VEC3 *v)
{
    return PSVECMag(HV(v));
}

static inline f32 M522Min(f32 a, f32 b)
{
    f32 r;

    if (a < b) {
        r = a;
    } else {
        r = b;
    }
    return r;
}

static inline f64 M522MaxD(f64 a, f64 b)
{
    f64 r;

    if (a > b) {
        r = a;
    } else {
        r = b;
    }
    return r;
}

static inline f32 M522Max(f32 a, f32 b)
{
    f32 r;

    if (a > b) {
        r = a;
    } else {
        r = b;
    }
    return r;
}

#define M522_STAGE_ASSERT()                              \
    if (lbl_1_bss_18 != 0) {                             \
    } else {                                             \
        fn_1_6A70(lbl_1_data_5EB, lbl_1_data_5FB, 0x27); \
    }

/* vector<u16> of motion ids. */
typedef M522Vec<s16> M522_U16VEC;
typedef M522Vec<s32> M522_HANDLES;

/* Registration record of a static object (__register_global_object). */
typedef struct M522GlobalReg_s {
    void *next;
    void *obj;
    void *dtor;
} M522_GLOBALREG;

struct M522_PLAYERBASE;
typedef void (*M522_PFN)(M522_PLAYERBASE *);

/* CPlayerBase: list node, state (pointer to member function), vtable, the
 * entry of GwPlayerConf and the motion id table. */
struct M522_PLAYERBASE : M522_NODE {
    /* 0x08 */ M522_PTMF state;
    /* 0x14 */ M522_PFN *vtbl;
    /* 0x18 */ s32 index;
    /* 0x1C */ s32 charNo;
    /* 0x20 */ s32 grp;
    /* 0x24 */ s32 padNo;
    /* 0x28 */ s32 dif;
    /* 0x2C */ s32 vibWait;
    /* 0x30 */ s32 frame;
    /* 0x34 */ M522_U16VEC motions;
    /* 0x40 */ s32 motion;
};

/* The player with a model of its own (human or CPU controlled). */
struct M522_PLAYER : M522_PLAYERBASE {
    /* 0x44 */ M522_MODEL model;
    /* 0x48 */ s32 mode;
    /* 0x4C */ M522_VEC3 pos;
    /* 0x58 */ M522_VEC3 vel;
    /* 0x64 */ f32 baseY;
    /* 0x68 */ f32 phase;
    /* 0x6C */ f32 amp;
    /* 0x70 */ f32 ofsY;
    /* 0x74 */ f32 ofsX;
    /* 0x78 */ M522_MODEL *objA;
    /* 0x7C */ M522_MODEL *objB;
    /* 0x80 */ s16 joint;
    /* 0x84 */ f32 objBY;
    /* 0x88 */ char *hook;
    /* 0x8C */ s32 timer;
};

/* Angle in [min, max) with wrap around. */
struct M522_ANGLE {
    /* 0x0 */ f32 min;
    /* 0x4 */ f32 max;
    /* 0x8 */ f32 range;
    /* 0xC */ f32 cur;
};

/* Virtual pad of a CPU player. */
struct M522_PAD {
    /* 0x00 */ f32 angle;
    /* 0x04 */ f32 x;
    /* 0x08 */ f32 y;
    /* 0x0C */ u32 btnDown;
    /* 0x10 */ u32 btn;
};

/* The player that rides the MG actor system. */
struct M522_COM : M522_PLAYERBASE {
    /* 0x44 */ u8 out;
    /* 0x48 */ M522_VEC3 pos;
    /* 0x54 */ s32 timer;
    /* 0x58 */ M522_VEC3 cur;
    /* 0x64 */ M522_VEC3 dir;
    /* 0x70 */ f32 rotY;
    /* 0x74 */ f32 rotX;
    /* 0x78 */ MGPLAYER *mg;
    /* 0x7C */ u8 grounded;
    /* 0x7D */ u8 flying;
    /* 0x80 */ M522_PTMF think;
    /* 0x8C */ s32 jumpWait;
    /* 0x90 */ u8 steady;
    /* 0x91 */ u8 hit;
    /* 0x94 */ M522_VEC3 hitVec;
    /* 0xA0 */ M522_PAD pad;
};

extern "C" {
extern void *lbl_1_bss_1C;
extern s32 lbl_1_bss_A48;
extern s32 lbl_1_bss_A4C;
extern s32 lbl_1_bss_A50;
extern s32 lbl_1_bss_A54;
extern M522_GLOBALREG lbl_1_bss_A58;
extern M522_NODE lbl_1_bss_A64;
extern M522_PTMF lbl_1_data_5A0;
extern M522_PTMF lbl_1_data_5AC;
extern M522_PTMF lbl_1_data_5B8;
extern char lbl_1_data_5C4[];
extern char lbl_1_data_5E0[];
extern char lbl_1_data_5EB[];
extern char lbl_1_data_5FB[];
extern M522_PTMF lbl_1_data_608;
extern M522_PTMF lbl_1_data_614;
extern M522_PTMF lbl_1_data_620;
extern M522_PTMF lbl_1_data_62C;
extern M522_PTMF lbl_1_data_638;
extern M522_PTMF lbl_1_data_644;
extern M522_PTMF lbl_1_data_650;
extern M522_PTMF lbl_1_data_65C;
extern M522_PTMF lbl_1_data_668;
extern M522_PTMF lbl_1_data_674;
extern M522_PTMF lbl_1_data_680;
extern M522_PTMF lbl_1_data_68C;
extern char lbl_1_data_698[];
extern M522_PTMF lbl_1_data_6C0;
extern M522_PTMF lbl_1_data_6CC;
extern M522_PTMF lbl_1_data_6D8;
extern M522_PTMF lbl_1_data_6E4;
extern M522_PTMF lbl_1_data_6F0;
extern M522_PTMF lbl_1_data_6FC;
extern M522_PTMF lbl_1_data_708;
extern M522_PTMF lbl_1_data_714;
extern M522_PFN lbl_1_data_720[];
extern M522_PFN lbl_1_data_740[];
extern M522_PFN lbl_1_data_760[];
extern char lbl_1_data_780[];
extern char lbl_1_data_79E[];
extern char lbl_1_data_7BB[];
extern const M522_VEC3 lbl_1_rodata_4D0[3];
extern const M522_VEC3 lbl_1_rodata_528;
extern const M522_VEC3 lbl_1_rodata_534;
extern const MGACTOR_PARAM lbl_1_rodata_5E8;
extern const M522_VEC3 lbl_1_rodata_61C;
extern const M522_VEC3 lbl_1_rodata_628;
extern const M522_VEC3 lbl_1_rodata_654;
extern const M522_VEC3 lbl_1_rodata_6C0;

void __ptmf_scall(void *self, M522_PTMF *ptmf);
void __register_global_object(void *obj, void *dtor, M522_GLOBALREG *reg);
void fn_1_6A70(const char *cond, const char *file, s32 line);
void fn_1_8004(M522_NODE *node);
void fn_1_8010(void *node, s32 flag);
void fn_1_807C(M522_NODE *node);
void fn_1_80D0(M522_NODE *node, M522_NODE *other);
void fn_1_92FC(s32 index);
M522_MODEL *fn_1_9A34(M522_MODEL *model, s32 dataNum);
void fn_1_9AB8(M522_MODEL *model, s32 charNo, s32 kind);
void fn_1_9BA8(M522_MODEL *model, u32 attr);
void fn_1_9BE0(M522_MODEL *model, u32 attr);
void fn_1_9C94(M522_MODEL *model, M522_VEC3 *pos);
void fn_1_9CDC(M522_MODEL *model, M522_VEC3 *rot);
void fn_1_9DEC(M522_MODEL *model, s32 layer);
void *fn_1_9E94(M522_MODEL *model, s32 flag);
f32 fn_1_BABC(void);
f32 fn_1_BD30(f32 max);
f32 fn_1_BD68(f32 min, f32 max);
void *fn_1_E5C8(s32 handle);
void fn_1_EF38(void *stage, M522_VEC3 *pos);
u8 fn_1_F194(void *stage);
f32 fn_1_F1B8(void *stage);
M522_HANDLES *fn_1_4EFC(void *mgr);
M522_VEC3 *fn_1_4F88(void *bubble, M522_VEC3 *out);
void MgActorPosGet(MGACTOR *actorP, HuVecF *pos);

s16 fn_1_F564(M522_VEC3 *pos);
void fn_1_F604(void);
void fn_1_F6E4(void);
void fn_1_F76C(void);
void fn_1_F800(void);
void fn_1_F894(void);
void fn_1_F904(void);
void fn_1_F974(void);
s32 fn_1_FA18(void);
s32 fn_1_FA28(void);
M522_PLAYERBASE *fn_1_FA84(s32 index);
M522_PLAYERBASE *fn_1_FAF8(void);
M522_PLAYERBASE *fn_1_FB6C(M522_PLAYERBASE *self, s32 index);
M522_U16VEC *fn_1_FCAC(M522_U16VEC *vec, s16 flag);
M522_U16VEC *fn_1_FD40(M522_U16VEC *vec, s16 flag);
M522_U16VEC *fn_1_FDCC(M522_U16VEC *vec, s16 flag);
M522_PLAYERBASE *fn_1_FE50(M522_PLAYERBASE *self, s16 flag);
M522_PLAYERBASE *fn_1_FEA8(M522_PLAYERBASE *self, s16 flag);
void fn_1_FF70(M522_PLAYERBASE *self);
void fn_1_FFF8(M522_PLAYERBASE *self, s32 dataNum);
void fn_1_10070(M522_PLAYERBASE *self, u32 motion, u8 loop);
f32 fn_1_10160(M522_PLAYERBASE *self);
void fn_1_101CC(M522_PLAYERBASE *self, u8 flag);
void fn_1_10240(M522_PLAYERBASE *self, s32 kind);
M522_PLAYER *fn_1_10324(M522_PLAYER *self, s32 index);
M522_PLAYER *fn_1_107E0(M522_PLAYER *self, s16 flag);
s32 fn_1_108F8(M522_PLAYER *self);
void fn_1_109A4(M522_PLAYER *self);
void fn_1_10A00(M522_PLAYER *self);
void fn_1_10DF8(M522_PLAYER *self);
void fn_1_1103C(M522_PLAYER *self);
void fn_1_113BC(M522_PLAYER *self);
void fn_1_114CC(M522_PLAYER *self);
void fn_1_11694(M522_PLAYER *self);
void fn_1_117EC(M522_PLAYER *self);
void fn_1_11998(M522_PLAYER *self);
void fn_1_119E8(M522_PLAYER *self);
void fn_1_11A20(M522_PLAYER *self);
void fn_1_11D60(M522_PLAYER *self);
void fn_1_11E34(M522_PLAYER *self);
s32 fn_1_12028(M522_PLAYER *self);
s32 fn_1_12084(M522_PLAYER *self);
s32 fn_1_120DC(M522_PLAYER *self);
void fn_1_12368(M522_PLAYER *self);
M522_COM *fn_1_12530(M522_COM *self, s32 index, const M522_VEC3 *spawn);
M522_COM *fn_1_1289C(M522_COM *self, s16 flag);
s32 fn_1_12984(void);
void fn_1_12A1C(M522_COM *self);
void fn_1_12A44(M522_COM *self);
void fn_1_12AE4(M522_COM *self);
void fn_1_12B68(M522_COM *self);
void fn_1_12EB0(M522_COM *self);
void fn_1_130D0(M522_COM *self);
void fn_1_133F8(M522_COM *self);
void fn_1_133FC(M522_COM *self);
void fn_1_13550(M522_COM *self);
void fn_1_13698(M522_COM *self);
void fn_1_1370C(M522_COM *self);
void fn_1_137D0(M522_COM *self);
void fn_1_1384C(M522_COM *self);
u8 fn_1_13A10(M522_COM *self);
s32 fn_1_13A48(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b);
void fn_1_13BBC(M522_COM *self);
void fn_1_13CE8(M522_COM *self);
void fn_1_13DCC(M522_COM *self);
void fn_1_13E18(M522_COM *self);
u8 fn_1_144D4(M522_COM *self, M522_VEC3 *out, f32 radius);
s32 fn_1_14708(M522_COM *self, M522_VEC3 *target, f32 maxDist);
void fn_1_14868(M522_PAD *self);
void fn_1_148B4(M522_PAD *self, u8 down);
s32 fn_1_148DC(M522_PAD *self, M522_VEC3 *dir);
void fn_1_14B20(M522_PAD *self, MGPLAYER *player);
void fn_1_14CFC(M522_PLAYERBASE *self);
void fn_1_14D00(M522_PLAYERBASE *self);
M522_ANGLE *fn_1_14D04(M522_ANGLE *self, f32 a, f32 b, f32 cur);
f32 fn_1_14DF4(M522_ANGLE *self, f32 x);
f32 fn_1_14E30(M522_ANGLE *self, f32 target, f64 rate);
f32 fn_1_14EA4(M522_ANGLE *self, f32 x);
f32 fn_1_14F40(M522_ANGLE *self);
s16 *fn_1_14F48(M522_U16VEC *self, u32 index);
void fn_1_14FC0(M522_NODE *head, void (*func)(M522_PLAYERBASE *));
void fn_1_15020(M522_NODE *first, M522_NODE *end, void (*func)(M522_PLAYERBASE *));
void fn_1_15080(M522_U16VEC *self, s16 *pos, u32 n, s16 *xp);
f32 fn_1_15430(M522_ANGLE *self, f32 a, f32 b);
void fn_1_15648(void);
void *fn_1_156D0(M522_NODE *self, s16 flag);
}

/* Member function pointers travel by value: MWCC copies them to the stack. */
inline void M522SetState(M522_PLAYERBASE *self, M522_PTMF ptmf)
{
    self->state = ptmf;
}

#define M522_SETSTATE(self, ptmf) \
    {                             \
        M522_PTMF next = ptmf;    \
        M522SetState(self, next); \
    }

/* Singleton accessor: the assert of singleton.h, then the pointer. */
static inline void *M522Stage(void)
{
    void *p;

    M522_STAGE_ASSERT();
    p = lbl_1_bss_18;
    return p;
}


/* Spawn positions of the CPU players, handed out by fn_1_F604. */
PLAYER_RODATA const M522_VEC3 lbl_1_rodata_4D0[3] = {
    { 0.0f, 2500.0f, -200.0f },
    { -173.2f, 3000.0f, 100.0f },
    { 173.2f, 3500.0f, 100.0f },
};
PLAYER_RODATA const M522_VEC3 lbl_1_rodata_528 = { -400.0f, 0.0f, 0.0f };
PLAYER_RODATA const M522_VEC3 lbl_1_rodata_534 = { 0.0f, 90.0f, 0.0f };
PLAYER_RODATA const MGACTOR_PARAM lbl_1_rodata_5E8 = {
    0.0f, 40.0f, 0, 0, (COL_NARROW_HOOK)fn_1_13A48, 0, 0, 0,
};
PLAYER_RODATA const M522_VEC3 lbl_1_rodata_61C = { 0.0f, 0.0f, 0.0f };
PLAYER_RODATA const M522_VEC3 lbl_1_rodata_628 = { 0.0f, -2400.0f, 0.0f };
PLAYER_RODATA const M522_VEC3 lbl_1_rodata_654 = { 0.0f, 0.0f, 0.0f };
PLAYER_RODATA const M522_VEC3 lbl_1_rodata_6C0 = { 0.0f, 0.0f, 0.0f };

s32 lbl_1_bss_A48;
s32 lbl_1_bss_A4C;
s32 lbl_1_bss_A50;
s32 lbl_1_bss_A54;
M522_GLOBALREG lbl_1_bss_A58;
M522_NODE lbl_1_bss_A64;

/* Pointers to virtual member functions: {0, vtable offset, vptr offset}. */
M522_PTMF lbl_1_data_5A0 = { 0, 12, { (void *)20 } };
M522_PTMF lbl_1_data_5AC = { 0, 16, { (void *)20 } };
M522_PTMF lbl_1_data_5B8 = { 0, 8, { (void *)20 } };
char lbl_1_data_5C4[] = "motion < m_motion_id.size()";
char lbl_1_data_5E0[] = "player.cpp";
char lbl_1_data_5EB[] = "ms_pointer != 0";
char lbl_1_data_5FB[] = "singleton.h";
M522_PTMF lbl_1_data_608 = { 0, -1, { (void *)fn_1_10A00 } };
M522_PTMF lbl_1_data_614 = { 0, -1, { (void *)fn_1_10DF8 } };
M522_PTMF lbl_1_data_620 = { 0, -1, { (void *)fn_1_113BC } };
M522_PTMF lbl_1_data_62C = { 0, -1, { (void *)fn_1_1103C } };
M522_PTMF lbl_1_data_638 = { 0, -1, { (void *)fn_1_10A00 } };
M522_PTMF lbl_1_data_644 = { 0, -1, { (void *)fn_1_114CC } };
M522_PTMF lbl_1_data_650 = { 0, -1, { (void *)fn_1_10A00 } };
M522_PTMF lbl_1_data_65C = { 0, -1, { (void *)fn_1_10DF8 } };
M522_PTMF lbl_1_data_668 = { 0, -1, { (void *)fn_1_11998 } };
M522_PTMF lbl_1_data_674 = { 0, -1, { (void *)fn_1_117EC } };
M522_PTMF lbl_1_data_680 = { 0, -1, { (void *)fn_1_119E8 } };
M522_PTMF lbl_1_data_68C = { 0, -1, { (void *)fn_1_11694 } };
char lbl_1_data_698[] = "m_motion_id.size() == NUMBEROF_MOTION";
M522_PTMF lbl_1_data_6C0 = { 0, -1, { (void *)fn_1_13E18 } };
M522_PTMF lbl_1_data_6CC = { 0, 8, { (void *)20 } };
M522_PTMF lbl_1_data_6D8 = { 0, -1, { (void *)fn_1_12B68 } };
M522_PTMF lbl_1_data_6E4 = { 0, -1, { (void *)fn_1_12EB0 } };
M522_PTMF lbl_1_data_6F0 = { 0, -1, { (void *)fn_1_133F8 } };
M522_PTMF lbl_1_data_6FC = { 0, 8, { (void *)20 } };
M522_PTMF lbl_1_data_708 = { 0, 8, { (void *)20 } };
M522_PTMF lbl_1_data_714 = { 0, -1, { (void *)fn_1_133FC } };
M522_PFN lbl_1_data_720[8] = {
    0, 0, (M522_PFN)fn_1_12A1C, (M522_PFN)fn_1_12A44, (M522_PFN)fn_1_12AE4,
    (M522_PFN)fn_1_13550, (M522_PFN)fn_1_1370C, (M522_PFN)fn_1_1384C,
};
M522_PFN lbl_1_data_740[8] = {
    0, 0, (M522_PFN)fn_1_14CFC, (M522_PFN)fn_1_14D00, (M522_PFN)fn_1_109A4,
    (M522_PFN)fn_1_11A20, (M522_PFN)fn_1_11D60, (M522_PFN)fn_1_11E34,
};
M522_PFN lbl_1_data_760[8] = {
    0, 0, (M522_PFN)fn_1_14CFC, (M522_PFN)fn_1_14D00, 0, 0, 0, 0,
};
char lbl_1_data_780[] = "vector::at index out of range";
char lbl_1_data_79E[] = "vector::insert length error\n";
char lbl_1_data_7BB[] = "Memory allocation failure";

s16 fn_1_F564(M522_VEC3 *pos)
{
    f32 x;
    s16 pan;
    s16 result;

    Hu3D3Dto2D(HV(pos), 1, HV(&x));
    x = x * 0.11111111f;
    pan = (s16)(32.0f + x);
    if (pan < 0) {
        result = 0;
    } else if (pan > 0x7F) {
        result = 0x7F;
    } else {
        result = pan;
    }
    return result;
}

void fn_1_F604(void)
{
    s32 i;
    void *obj;
    const M522_VEC3 *spawn;

    lbl_1_bss_A54 = 0;
    spawn = lbl_1_rodata_4D0;
    for (i = 0; i < 4; i++) {
        if (GwPlayerConf[i].grp == 0) {
            obj = fn_1_689C(sizeof(M522_PLAYER));
            if (obj != 0) {
                fn_1_10324((M522_PLAYER *)obj, i);
            }
        } else {
            obj = fn_1_689C(sizeof(M522_COM));
            if (obj != 0) {
                fn_1_12530((M522_COM *)obj, i, spawn);
                spawn++;
            }
        }
    }
    CharEffectLayerSet(5);
}

void fn_1_F6E4(void)
{
    M522_NODE *first;
    M522_NODE *it;
    M522_NODE *tmp;

    fn_1_14FC0(&lbl_1_bss_A64, fn_1_FF70);
    MgActorExec();
    first = lbl_1_bss_A64.prev;
    tmp = first;
    it = tmp;
    if (it != &lbl_1_bss_A64) {
        fn_1_807C(it);
        fn_1_80D0(it, &lbl_1_bss_A64);
    }
}

void fn_1_F76C(void)
{
    M522_NODE *it;
    M522_NODE *end;
    M522_NODE *first;
    M522_NODE *tmp;
    M522_NODE *next;

    first = lbl_1_bss_A64.prev;
    tmp = first;
    it = tmp;
    end = &lbl_1_bss_A64;
    while (it != end) {
        M522SetState((M522_PLAYERBASE *)it, lbl_1_data_5A0);
        next = it->prev;
        it = next;
    }
}

void fn_1_F800(void)
{
    M522_NODE *it;
    M522_NODE *end;
    M522_NODE *first;
    M522_NODE *tmp;
    M522_NODE *next;

    first = lbl_1_bss_A64.prev;
    tmp = first;
    it = tmp;
    end = &lbl_1_bss_A64;
    while (it != end) {
        M522SetState((M522_PLAYERBASE *)it, lbl_1_data_5AC);
        next = it->prev;
        it = next;
    }
}

void fn_1_F894(void)
{
    M522_NODE *it;
    M522_NODE *end;
    M522_NODE *first;
    M522_NODE *tmp;
    M522_NODE *next;

    first = lbl_1_bss_A64.prev;
    tmp = first;
    it = tmp;
    end = &lbl_1_bss_A64;
    while (it != end) {
        ((M522_PLAYERBASE *)it)->vtbl[6]((M522_PLAYERBASE *)it);
        next = it->prev;
        it = next;
    }
}

void fn_1_F904(void)
{
    M522_NODE *it;
    M522_NODE *end;
    M522_NODE *first;
    M522_NODE *tmp;
    M522_NODE *next;

    first = lbl_1_bss_A64.prev;
    tmp = first;
    it = tmp;
    end = &lbl_1_bss_A64;
    while (it != end) {
        ((M522_PLAYERBASE *)it)->vtbl[5]((M522_PLAYERBASE *)it);
        next = it->prev;
        it = next;
    }
}

void fn_1_F974(void)
{
    M522_NODE *last;
    M522_NODE *tmp;
    u8 empty;

    goto test;
body:
    last = lbl_1_bss_A64.next;
    tmp = last;
    fn_1_FEA8((M522_PLAYERBASE *)tmp, 1);
test:
    empty = 0;
    if (lbl_1_bss_A64.prev == &lbl_1_bss_A64 && lbl_1_bss_A64.prev == lbl_1_bss_A64.next) {
        empty = 1;
    }
    if (empty == 0) {
        goto body;
    }
}

s32 fn_1_FA18(void)
{
    return lbl_1_bss_A54;
}

s32 fn_1_FA28(void)
{
    if (lbl_1_bss_A54 <= 0) {
        return 0;
    }
    if (lbl_1_bss_A54 == fn_1_12984()) {
        return 1;
    }
    return -1;
}

M522_PLAYERBASE *fn_1_FA84(s32 index)
{
    M522_NODE *it;
    M522_NODE *end;
    M522_NODE *first;
    M522_NODE *tmp;
    M522_NODE *next;

    first = lbl_1_bss_A64.prev;
    tmp = first;
    it = tmp;
    end = &lbl_1_bss_A64;
    for (; it != end; it = next) {
        if (((M522_PLAYERBASE *)it)->index == index) {
            return (M522_PLAYERBASE *)it;
        }
        next = it->prev;
    }
    return 0;
}

M522_PLAYERBASE *fn_1_FAF8(void)
{
    M522_NODE *it;
    M522_NODE *end;
    M522_NODE *first;
    M522_NODE *tmp;
    M522_NODE *next;

    first = lbl_1_bss_A64.prev;
    tmp = first;
    it = tmp;
    end = &lbl_1_bss_A64;
    for (; it != end; it = next) {
        if (((M522_PLAYERBASE *)it)->grp == 0) {
            return (M522_PLAYERBASE *)it;
        }
        next = it->prev;
    }
    return 0;
}

M522_PLAYERBASE *fn_1_FB6C(M522_PLAYERBASE *self, s32 index)
{
    s16 pad;

    fn_1_8004(self);
    self->vtbl = lbl_1_data_760;
    self->state = lbl_1_data_5B8;
    self->index = index;
    self->charNo = GwPlayerConf[index].charNo;
    self->grp = GwPlayerConf[index].grp;
    if (GwPlayerConf[index].type != 0) {
        pad = -1;
    } else {
        pad = GwPlayerConf[index].padNo;
    }
    self->padNo = pad;
    self->dif = GwPlayerConf[index].dif;
    self->vibWait = 0;
    self->frame = 0;
    self->motions.m_cap = 0;
    self->motions.m_size = 0;
    self->motions.m_data = 0;
    self->motion = -1;
    fn_1_80D0(self, &lbl_1_bss_A64);
    return self;
}

/* The vector data is released through its address, as the MSL inlines do. */
#define M522_VEC_FREE(vec)                      {                                               s16 **pb = &(vec)->m_data;                  s16 *data = *pb;                            fn_1_6964(data);                        }

M522_U16VEC *fn_1_FCAC(M522_U16VEC *vec, s16 flag)
{
    if (vec != 0) {
        if (vec != 0 && vec != 0) {
            s16 **pa = &vec->m_data;
            if (*pa != 0) {
                M522_VEC_FREE(vec);
            }
        }
        if (flag > 0) {
            fn_1_6964(vec);
        }
    }
    return vec;
}

M522_U16VEC *fn_1_FD40(M522_U16VEC *vec, s16 flag)
{
    if (vec != 0) {
        if (vec != 0) {
            s16 **pa = &vec->m_data;
            if (*pa != 0) {
                M522_VEC_FREE(vec);
            }
        }
        if (flag > 0) {
            fn_1_6964(vec);
        }
    }
    return vec;
}

M522_U16VEC *fn_1_FDCC(M522_U16VEC *vec, s16 flag)
{
    if (vec != 0) {
        s16 **pa = &vec->m_data;
        if (*pa != 0) {
            M522_VEC_FREE(vec);
        }
        if (flag > 0) {
            fn_1_6964(vec);
        }
    }
    return vec;
}

M522_PLAYERBASE *fn_1_FE50(M522_PLAYERBASE *self, s16 flag)
{
    if (self != 0) {
        fn_1_8010(self, 0);
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

M522_PLAYERBASE *fn_1_FEA8(M522_PLAYERBASE *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_760;
        if (&self->motions != 0 && &self->motions != 0 && &self->motions != 0) {
            s16 **pa = &self->motions.m_data;
            if (*pa != 0) {
                M522_VEC_FREE(&self->motions);
            }
        }
        if (self != 0) {
            fn_1_8010(self, 0);
        }
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

void fn_1_FF70(M522_PLAYERBASE *self)
{
    u8 expired = 0;

#ifdef TARGET_PC
    if (self->state.v_offset < 0) {
        ((M522_PFN)self->state.f_data.f_addr)(self);
    } else {
        self->vtbl[self->state.v_offset / 4](self);
    }
#else
    __ptmf_scall(self, &self->state);
#endif
    self->vtbl[7](self);
    if (self->vibWait > 0) {
        if (--self->vibWait == 0) {
            expired = 1;
        }
    }
    self->frame = self->frame + 1;
}

void fn_1_FFF8(M522_PLAYERBASE *self, s32 dataNum)
{
    s16 id;

    id = CharMotionCreate(self->charNo, dataNum);
    s16 *const *pp = &self->motions.m_data;
    s16 *last = *pp + self->motions.m_size;
    fn_1_15080(&self->motions, last, 1, &id);
}

void fn_1_10070(M522_PLAYERBASE *self, u32 motion, u8 loop)
{
    u32 attr;
    u32 size;
    s16 *id;

    size = self->motions.m_size;
    if (motion < size) {
    } else {
        fn_1_6A70(lbl_1_data_5C4, lbl_1_data_5E0, 0x101);
    }
    if (self->motion != (s32)motion || CharMotionEndCheck(self->charNo) != 0) {
        CharModelAttrSet(self->charNo, 0x40000002);
        if (loop != 0) {
            attr = 0x40000001;
        } else {
            attr = 0;
        }
        id = fn_1_14F48(&self->motions, motion);
        CharMotionShiftSet(self->charNo, *id, 0.0f, 16.0f, attr);
        self->motion = motion;
    }
}

f32 fn_1_10160(M522_PLAYERBASE *self)
{
    if (CharMotionShiftIDGet(self->charNo) >= 0) {
        return CharMotionShiftTimeGet(self->charNo) - 1.0f;
    }
    return CharMotionTimeGet(self->charNo);
}

void fn_1_101CC(M522_PLAYERBASE *self, u8 flag)
{
    if (flag != 0) {
        CharModelAttrReset(self->charNo, 1);
    } else {
        CharModelAttrSet(self->charNo, 1);
    }
    CharModelFxFlagSet(self->charNo, flag);
}

void fn_1_10240(M522_PLAYERBASE *self, s32 kind)
{
    switch (kind) {
    default:
        omVibrate(self->index, 12, 6, 6);
        break;
    case 2:
        omVibrate(self->index, 6, 3, 6);
        break;
    case 3:
        if (self->vibWait <= 0) {
            omVibrate(self->index, 12, 12, 0);
            self->vibWait = 18;
        }
        break;
    case 4:
        omVibrate(self->index, 24, 12, 0);
        break;
    }
}

M522_PLAYER *fn_1_10324(M522_PLAYER *self, s32 index)
{
    M522_MODEL *a;
    M522_MODEL *b;
    f32 baseY;

    fn_1_FB6C(self, index);
    fn_1_9AB8(&self->model, GwPlayerConf[index].charNo, 2);
    self->vtbl = lbl_1_data_740;
    self->mode = 2;
    self->baseY = 0.0f;
    self->phase = 0.0f;
    self->amp = 10.0f;
    self->ofsY = 90.0f;
    self->ofsX = 0.0f;
    a = (M522_MODEL *)fn_1_689C(sizeof(M522_MODEL));
    if (a != 0) {
        a = fn_1_9A34(a, 0x500009);
    }
    self->objA = a;
    b = (M522_MODEL *)fn_1_689C(sizeof(M522_MODEL));
    if (b != 0) {
        b = fn_1_9A34(b, 0x50000A);
    }
    self->objB = b;
    self->joint = Hu3DJointMotion(self->objB->id, HuDataSelHeapReadNum(0x50000B, 0x10000000, (HEAPID)2));
    self->objBY = self->ofsY;
    self->hook = CharModelItemHookGet(self->charNo, 4, 0);
    self->timer = 0;
    fn_1_9DEC(&self->model, 5);
    Hu3DModelHookSet(self->model.id, self->hook, self->objA->id);
    fn_1_9DEC(self->objB, 5);
    fn_1_9BA8(self->objB, 0x40000001);
    baseY = fn_1_F1B8(M522Stage()) - 100.0f;
    self->pos.x = -550.0f;
    self->pos.y = baseY;
    self->pos.z = 0.0f;
    self->vel.x = 0.0f;
    self->vel.y = 0.0f;
    self->vel.z = 0.0f;
    self->vtbl[7](self);
    fn_1_12368(self);
    fn_1_10070(self, 0, 1);
    return self;
}

M522_PLAYER *fn_1_107E0(M522_PLAYER *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_740;
        if (&self->objB != 0) {
            fn_1_9E94(self->objB, 1);
        }
        if (&self->objA != 0) {
            fn_1_9E94(self->objA, 1);
        }
        fn_1_9E94(&self->model, 0);
        fn_1_FEA8(self, 0);
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

s32 fn_1_108F8(M522_PLAYER *self)
{
    f32 time;
    s32 result;

    if (CharMotionShiftIDGet(self->charNo) >= 0) {
        time = CharMotionShiftTimeGet(self->charNo) - 1.0f;
    } else {
        time = CharMotionTimeGet(self->charNo);
    }
    if (time > 15.0f) {
        result = self->mode;
    } else {
        result = 2;
    }
    return result;
}

void fn_1_109A4(M522_PLAYER *self)
{
    M522_SETSTATE(self, lbl_1_data_608);
}

void fn_1_10A00(M522_PLAYER *self)
{
    self->amp = 0.99 * self->amp + 0.1;
    if (self->timer <= 0) {
            if (fn_1_F194(M522Stage()) != 0) {
            self->mode = fn_1_12028(self);
            switch (self->mode) {
            case 0:
                fn_1_10070(self, 1, 0);
                HuAudFXPlayPan(0x73A, fn_1_F564(&self->pos));
                M522_SETSTATE(self, lbl_1_data_614);
                return;
            case 1:
                fn_1_10070(self, 1, 0);
                HuAudFXPlayPan(0x73A, fn_1_F564(&self->pos));
                M522_SETSTATE(self, lbl_1_data_620);
                return;
            }
        }
    }
}

void fn_1_10DF8(M522_PLAYER *self)
{
    f32 time;

    self->amp = self->amp * 0.8;
    time = fn_1_10160(self);
    if (!(time < 25.0f)) {
        M522_VEC3 pos = lbl_1_rodata_528;
        M522_VEC3 rot;
            pos.y = 100.0f + fn_1_F1B8(M522Stage());
        rot = lbl_1_rodata_534;
            fn_1_EF38(M522Stage(), &pos);
        CharEffectHitCreate(1, HV(&pos), HV(&rot));
        omVibrate(self->index, 12, 6, 6);
        self->amp = 0.0f;
        self->baseY = self->pos.y;
        M522_SETSTATE(self, lbl_1_data_62C);
    }
}

void fn_1_1103C(M522_PLAYER *self)
{
    f32 time;
    f32 range;
    f32 floorY;
    f32 target;
    f32 t;

    time = fn_1_10160(self);
    if (!(time <= 30.0f)) {
        range = CharMotionMaxTimeGet(self->charNo) - 30.0f;
            floorY = fn_1_F1B8(M522Stage()) - 100.0f;
        if (floorY < 200.0f) {
            target = 200.0f;
        } else {
            target = floorY;
        }
        t = (time - 30.0f) / range;
        self->pos.y = (1.0 - t) * self->baseY + t * target;
        if (CharMotionEndCheck(self->charNo) != 0) {
            self->phase = 3.1415927f;
            self->amp = 80.0f;
            fn_1_10070(self, 0, 1);
            self->mode = 2;
            M522_SETSTATE(self, lbl_1_data_638);
        }
    }
}

void fn_1_113BC(M522_PLAYER *self)
{
    f32 time;

    time = fn_1_10160(self);
    if (time >= 16.0f) {
        CharModelAttrSet(self->charNo, 0x40000004);
        M522_SETSTATE(self, lbl_1_data_644);
    }
}

void fn_1_114CC(M522_PLAYER *self)
{
    if (CharMotionEndCheck(self->charNo) != 0) {
        fn_1_10070(self, 0, 1);
        self->mode = 2;
        M522_SETSTATE(self, lbl_1_data_650);
        return;
    }
    if (fn_1_12028(self) == 0) {
        self->mode = 0;
        CharModelAttrReset(self->charNo, 0x40000004);
        M522_SETSTATE(self, lbl_1_data_65C);
    }
}

void fn_1_11694(M522_PLAYER *self)
{
    f64 t;

    self->amp = self->amp * 0.9;
    self->vel.y = 0.01 * ((400.0f + fn_1_F1B8(M522Stage())) - self->pos.y);
    t = self->ofsY - 0.5172413793103449;
    self->ofsY = M522MaxD(t, 0.0);
}

void fn_1_117EC(M522_PLAYER *self)
{
    self->vel.y = self->vel.y - 0.27240694f;
    if (self->pos.y < -100.0f) {
        fn_1_10070(self, 3, 1);
        self->vel.x = -8.333333f;
        self->vel.y = 16.666666f;
        self->vel.z = -8.333333f;
        omVibrate(self->index, 24, 12, 0);
        M522_SETSTATE(self, lbl_1_data_668);
    }
}

void fn_1_11998(M522_PLAYER *self)
{
    self->vel.y = self->vel.y - 0.27240694f;
    self->ofsY = self->ofsY + 0.1 * (-135.0f - self->ofsY);
}

void fn_1_119E8(M522_PLAYER *self)
{
    self->ofsX = self->ofsX + 0.01 * (-30.0f - self->ofsX);
}

void fn_1_11A20(M522_PLAYER *self)
{
    s16 pan;

    pan = fn_1_F564(&self->pos);
    if (lbl_1_bss_A54 > 0) {
        Hu3DMotionSet(self->objB->id, self->joint);
        fn_1_9BE0(self->objB, 0x40000001);
        fn_1_10070(self, 2, 1);
        omVibrate(self->index, 6, 3, 6);
        HuAudFXPlayPan(0x748, pan);
        M522_SETSTATE(self, lbl_1_data_674);
        return;
    }
    self->vel.x = 0.0f;
    self->vel.y = 0.0f;
    self->vel.z = 0.0f;
    fn_1_10070(self, 4, 1);
    CharFXPlayPan(self->charNo, 0x1FA, pan);
    M522_SETSTATE(self, lbl_1_data_680);
}

void fn_1_11D60(M522_PLAYER *self)
{
    if (lbl_1_bss_A54 <= 0) {
        fn_1_92FC(self->index);
    }
    CharModelHookDustCreate(self->charNo, self->hook);
    self->vel.x = -0.16666667f;
    self->vel.y = 0.0f;
    self->vel.z = 0.0f;
    M522_SETSTATE(self, lbl_1_data_68C);
}

void fn_1_11E34(M522_PLAYER *self)
{
    M522_VEC3 v;
    char *hook;
    M522_MODEL *mp;
    M522_MODEL *b1;
    M522_MODEL *b2;
    s16 id;
    s16 id2;

    M522VecSet(&v, self->pos.x, self->pos.y + self->amp * sin(self->phase), self->pos.z);
    fn_1_9C94(&self->model, &v);
    PSVECAdd(HV(&self->vel), HV(&self->pos), HV(&self->pos));
    self->phase = self->phase + 0.05235987755982988;
    M522VecSet(&v, self->ofsX, self->ofsY, 0.0f);
    fn_1_9CDC(&self->model, &v);
    hook = CharModelItemHookGet(self->charNo, 4, 4);
    mp = (self != 0) ? &self->model : 0;
    id = mp->id;
    id2 = id;
    Hu3DModelObjPosGet(id2, hook, HV(&v));
    b1 = self->objB;
    fn_1_9C94(b1, &v);
    self->objBY = self->objBY + 0.01 * (self->ofsY - self->objBY);
    M522VecSet(&v, 0.0f, self->objBY, 0.0f);
    b2 = self->objB;
    fn_1_9CDC(b2, &v);
}

s32 fn_1_12028(M522_PLAYER *self)
{
    s32 result;

    if (self->padNo >= 0) {
        result = fn_1_12084(self);
    } else {
        result = fn_1_120DC(self);
    }
    return result;
}

s32 fn_1_12084(M522_PLAYER *self)
{
    u16 btn;

    btn = HuPadBtnDown[self->padNo];
    if ((btn & 0x100) != 0) {
        return 0;
    }
    if ((btn & 0x200) != 0) {
        return 1;
    }
    return 2;
}

s32 fn_1_120DC(M522_PLAYER *self)
{
    f64 interval;
    f64 weight;
    f64 chance;
    s32 alive;
    s32 total;
    f32 roll;

    if (self->mode != 2) {
        return 2;
    }
    interval = 60.0 * (0.2 * (f64)(4 - self->dif));
    if (fn_1_BABC() > 1.0 / interval) {
        return 2;
    }
    alive = fn_1_12984();
    total = lbl_1_bss_A54;
    if (alive < total) {
        chance = 1.0 - (f64)alive / (f64)total;
        weight = 0.3 * (f64)self->dif;
        chance = (1.0 - weight) * chance + weight * 1.0;
        if (fn_1_BABC() < chance) {
            return 0;
        }
    }
    roll = fn_1_BD30(3.0f);
    if (roll > 2.0) {
        return 1;
    }
    if (roll > 1.0) {
        return 0;
    }
    return 2;
}

void fn_1_12368(M522_PLAYER *self)
{
    self->motions.m_size = 0;
    fn_1_FFF8(self, 0x8C005E);
    fn_1_FFF8(self, 0x8C005F);
    fn_1_FFF8(self, 0x8C002B);
    fn_1_FFF8(self, 0x8C0015);
    fn_1_FFF8(self, 0x8C0060);
    Hu3DMotionAttrSet(*fn_1_14F48(&self->motions, 4), 1);
    if (self->motions.m_size == 5) {
    } else {
        fn_1_6A70(lbl_1_data_698, lbl_1_data_5E0, 0x29D);
    }
    CharMotionDataClose(self->charNo);
}

M522_COM *fn_1_12530(M522_COM *self, s32 index, const M522_VEC3 *spawn)
{
    s16 mdl;

    fn_1_FB6C(self, index);
    self->vtbl = lbl_1_data_720;
    self->out = 0;
    self->timer = 0;
    self->cur.x = spawn->x;
    self->cur.y = spawn->y;
    self->cur.z = spawn->z;
    self->rotY = -90.0f;
    self->rotX = 0.0f;
    self->grounded = 0;
    self->flying = 0;
    self->think = lbl_1_data_6C0;
    self->jumpWait = 0;
    self->steady = 1;
    self->hit = 0;
    self->pad.angle = -1.5707964f;
    self->pad.x = 0.0f;
    self->pad.y = 0.0f;
    self->pad.btnDown = 0;
    self->pad.btn = 0;
    MGACTOR_PARAM param = lbl_1_rodata_5E8;
    param.height = CharModelHeightGet(self->charNo);
    param.param = index;
    self->mg = MgPlayerCreate(index, &param, 4, 1, 0x13, 0);
    MgPlayerAttrSet(self->mg, 2);
    MgPlayerVibrateCreate(self->mg);
    MgPlayerPauseOn(self->mg);
    fn_1_13BBC(self);
    mdl = self->mg->actor->mdlId;
    Hu3DModelShadowSet(mdl);
    Hu3DModelLayerSet(mdl, 5);
    self->pos.x = spawn->x;
    self->pos.y = spawn->y;
    self->pos.z = spawn->z;
    self->dir.x = 0.0f;
    self->dir.y = 0.0f;
    self->dir.z = 0.0f;
    MgActorPosUpdate(self->mg->actor, HV(&self->pos));
    self->mg->actor->rotY = self->rotY;
    lbl_1_bss_A54 += 1;
    return self;
}

M522_COM *fn_1_1289C(M522_COM *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_720;
        MgActorKill(self->mg->actor);
        fn_1_FEA8(self, 0);
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}

s32 fn_1_12984(void)
{
    M522_NODE *it;
    s32 count;

    it = lbl_1_bss_A64.prev;
    count = 0;
    while (it != &lbl_1_bss_A64) {
        M522_COM *p = (M522_COM *)it;
        if (p->grp != 0 && p->out == 0 && fn_1_13A10(p) != 0) {
            count++;
        }
        it = it->prev;
    }
    return count;
}

void fn_1_12A1C(M522_COM *self)
{
    fn_1_13CE8(self);
}

void fn_1_12A44(M522_COM *self)
{
    self->flying = 0;
    MgPlayerPauseOff(self->mg, HV(&self->pos));
    MgPlayerAttrSet(self->mg, 1);
    self->grounded = 1;
    M522_SETSTATE(self, lbl_1_data_6CC);
}

void fn_1_12AE4(M522_COM *self)
{
    MgPlayerAttrReset(self->mg, 1);
    M522_SETSTATE(self, lbl_1_data_6D8);
}

void fn_1_12B68(M522_COM *self)
{
    M522_VEC3 p;
    u8 steady;

    if (self->padNo < 0) {
        fn_1_13DCC(self);
    }
    MgActorPosGet(self->mg->actor, HV(&p));
    if (p.y < (f32)(fn_1_F1B8(M522Stage()) - 50.0) || p.x > 500.0f) {
        fn_1_13698(self);
        self->flying = 1;
        MgPlayerPauseOn(self->mg);
        fn_1_10070(self, 0, 1);
        self->dir.x = p.x;
        self->dir.y = p.y;
        self->dir.z = p.z;
        PSVECSubtract(HV(&self->dir), HV(&self->cur), HV(&self->dir));
        omVibrate(self->index, 6, 3, 6);
        M522_SETSTATE(self, lbl_1_data_6E4);
    }
    self->cur.x = p.x;
    self->cur.y = p.y;
    self->cur.z = p.z;
    if (self->timer > 0) {
        if (--self->timer == 0) {
        }
    }
    if (fn_1_13A10(self) != 0) {
        if (self->timer <= 0) {
            MgPlayerAttrReset(self->mg, 1);
        } else {
            MgPlayerAttrSet(self->mg, 1);
        }
    } else {
        MgPlayerAttrReset(self->mg, 1);
        self->timer = 0x12;
    }
    steady = fn_1_F194(M522Stage());
    if (fn_1_13A10(self) != 0 && self->steady != 0 && steady == 0) {
        omVibrate(self->index, 12, 6, 6);
    }
    self->steady = steady;
}

void fn_1_12EB0(M522_COM *self)
{
    M522_VEC3 d = lbl_1_rodata_61C;
    M522_VEC3 diff;
    f64 ax;
    f64 az;
    s32 sx;
    s32 sz;

    d.x = self->cur.x;
    d.z = self->cur.z;
    if (M522Mag(&d) < 500.0f) {
        ax = fabs(self->dir.x);
        if (self->cur.x < 0.0f) {
            sx = -1;
        } else {
            sx = 1;
        }
        self->dir.x = (f64)sx * ax;
        az = fabs(self->dir.z);
        if (self->cur.z < 0.0f) {
            sz = -1;
        } else {
            sz = 1;
        }
        self->dir.z = (f64)sz * az;
    }
    M522_VEC3 target = lbl_1_rodata_628;
    PSVECSubtract(HV(&target), HV(&self->cur), HV(&diff));
    if (M522Mag(&diff) < 2400.0f) {
        fn_1_130D0(self);
    }
}

void fn_1_130D0(M522_COM *self)
{
    M522_VEC3 *apos;

    fn_1_13698(self);
    if (MgPlayerAttrCheck(self->mg, 0x400) == 0) {
        apos = (M522_VEC3 *)&self->mg->actor->pos;
        self->cur.x = apos->x;
        self->cur.y = apos->y;
        self->cur.z = apos->z;
    }
    self->dir.x = self->cur.x;
    self->dir.y = 0.0f;
    self->dir.z = self->cur.z;
    PSVECNormalize(HV(&self->dir), HV(&self->dir));
    PSVECScale(HV(&self->dir), HV(&self->dir), 10.0f);
    self->dir.y = 13.333333f;
    self->rotY = 180.0 * ((f64)atan2f(self->cur.x, self->cur.z) / 3.141592653589793);
    self->rotX = 0.0f;
    self->flying = 1;
    MgPlayerPauseOn(self->mg);
    fn_1_10070(self, 1, 1);
    omVibrate(self->index, 24, 12, 0);
    HuAudFXPlayPan(0x746, fn_1_F564(&self->cur));
    M522_SETSTATE(self, lbl_1_data_6F0);
}

void fn_1_133F8(M522_COM *self)
{
}

void fn_1_133FC(M522_COM *self)
{
    M522_ANGLE a;
    MGACTOR *actor;

    if (fn_1_13A10(self) != 0 || self->out != 0) {
        actor = self->mg->actor;
        fn_1_14D04(&a, 0.0f, 360.0f, actor->rotY);
        actor->rotY = fn_1_14E30(&a, 0.0f, 0.1);
        if (fabs(actor->rotY) < 0.1) {
            actor->rotY = 0.0f;
            M522_SETSTATE(self, lbl_1_data_6FC);
        }
    }
}

void fn_1_13550(M522_COM *self)
{
    if (self->out == 0 && lbl_1_bss_A54 > 0) {
        fn_1_10070(self, 2, 0);
        M522_SETSTATE(self, lbl_1_data_708);
    }
}

void fn_1_13698(M522_COM *self)
{
    if (self->out == 0) {
        MgActorColAttrSet(self->mg->actor, 4);
        MgPlayerAttrSet(self->mg, 1);
        self->out = 1;
        lbl_1_bss_A54 -= 1;
    }
}

void fn_1_1370C(M522_COM *self)
{
    if (lbl_1_bss_A54 > 0) {
        fn_1_92FC(self->index);
        MgActorColAttrSet(self->mg->actor, 4);
        MgPlayerAttrSet(self->mg, 1);
        if (self->out == 0) {
            MgPlayerComStkOn(self->mg);
            M522_SETSTATE(self, lbl_1_data_714);
        }
    }
}

void fn_1_137D0(M522_COM *self)
{
    self->grounded = 1;
    CharModelAttrReset(self->charNo, 1);
    CharModelFxFlagSet(self->charNo, 1);
    self->flying = 0;
    MgPlayerPauseOff(self->mg, HV(&self->pos));
    self->mg->actor->rotY = 0.0f;
}

void fn_1_1384C(M522_COM *self)
{
    M522_VEC3 p;
    s16 mdl;

    if (self->flying == 0) {
        p = *(M522_VEC3 *)&self->mg->actor->pos;
        CharModelVoicePanSet(self->charNo, fn_1_F564(&p));
        return;
    }
    mdl = self->mg->actor->mdlId;
    PSVECAdd(HV(&self->dir), HV(&self->cur), HV(&self->cur));
    self->dir.y = self->dir.y - 0.27240694f;
    Hu3DModelPosSetV(mdl, HV(&self->cur));
    CharModelVoicePanSet(self->charNo, fn_1_F564(&self->cur));
    Hu3DModelRotSet(mdl, self->rotX, self->rotY, 0.0f);
}

u8 fn_1_13A10(M522_COM *self)
{
    u32 *attr = &self->mg->actor->colGroundAttr;
    return (u8)((*attr & 0x407F) != 0);
}

s32 fn_1_13A48(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b)
{
    M522_NODE *it;
    M522_COM *found;

    it = lbl_1_bss_A64.prev;
    while (1) {
        if (it == &lbl_1_bss_A64) {
            found = 0;
            break;
        }
        if (((M522_PLAYERBASE *)it)->index == a->paramB) {
            found = (M522_COM *)it;
            break;
        }
        it = it->prev;
    }
    if (found == 0) {
        return 0;
    }
    if (b->type == 0) {
        found->hit = 1;
        found->hitVec.x = b->point.x;
        found->hitVec.y = b->point.y;
        found->hitVec.z = b->point.z;
        return 0;
    }
    if (b->type != 1) {
        return 0;
    }
    fn_1_130D0(found);
    CharFXPlayPan(found->charNo, 0x1F9, fn_1_F564(&found->cur));
    return 0;
}

void fn_1_13BBC(M522_COM *self)
{
    self->motions.m_size = 0;
    fn_1_FFF8(self, 0x8C002B);
    fn_1_FFF8(self, 0x8C0015);
    fn_1_FFF8(self, 0x8C0006);
    if (self->motions.m_size == 3) {
    } else {
        fn_1_6A70(lbl_1_data_698, lbl_1_data_5E0, 0x424);
    }
    CharMotionDataClose(self->charNo);
}

void fn_1_13CE8(M522_COM *self)
{
    M522_VEC3 p;

    if (self->grounded != 0) {
        MgActorPosGet(self->mg->actor, HV(&p));
            if (p.y <= (f32)(1.0 + fn_1_F1B8(M522Stage()))) {
            omVibrate(self->index, 12, 6, 6);
            self->grounded = 0;
        }
    }
}

void fn_1_13DCC(M522_COM *self)
{
#ifdef TARGET_PC
    ((void (*)(M522_COM *))self->think.f_data.f_addr)(self);
#else
    __ptmf_scall(self, &self->think);
#endif
    self->hit = 0;
    fn_1_14B20(&self->pad, self->mg);
}

void fn_1_13E18(M522_COM *self)
{
    M522_PLAYER *human;
    M522_VEC3 pos;
    M522_VEC3 tgt = lbl_1_rodata_654;
    M522_VEC3 avg;
    f32 limit;
    f32 mag;
    u8 wantJump;
    u8 steady;
    u8 found;
    u8 jump;
    s32 state;

    wantJump = 0;
    human = (M522_PLAYER *)fn_1_FAF8();
    if (human != 0) {
        state = fn_1_108F8(human);
        switch (state) {
        case 0:
            wantJump = fn_1_BD30((f32)(5 - self->dif)) / 5.0 < 0.047619047619047616;
            break;
        case 1:
            wantJump = fn_1_BD30((f32)(self->dif + 1)) * 0.25 < 0.08;
            break;
        }
    }
    steady = fn_1_F194(M522Stage());
    if ((self->mg->actor->colGroundAttr & 0x407F) != 0) {
        if (wantJump != 0) {
            fn_1_148B4(&self->pad, 1);
            self->jumpWait = (s32)(60.0f * fn_1_BD68(0.2f, 1.0f));
        }
    } else {
        jump = 0;
        if (self->dif == 0 && fn_1_BABC() < 0.016666666666666666) {
            jump = 1;
        }
        if (steady == 0 || self->jumpWait > 0) {
            fn_1_148B4(&self->pad, jump);
        }
    }
    if (self->jumpWait > 0) {
        if (--self->jumpWait == 0) {
        }
    }
    MgActorPosGet(self->mg->actor, HV(&pos));
    tgt.x = pos.x;
    tgt.z = pos.z;
    found = fn_1_144D4(self, &avg, 150.0f);
    if (found != 0) {
        if (self->hit != 0) {
            avg.x = 0.6 * avg.x + 0.4 * self->hitVec.x;
            avg.y = 0.6 * avg.y + 0.4 * self->hitVec.y;
            avg.z = 0.6 * avg.z + 0.4 * self->hitVec.z;
        }
        tgt.x = pos.x - avg.x;
        tgt.y = 0.0f;
        tgt.z = pos.z - avg.z;
        PSVECNormalize(HV(&tgt), HV(&tgt));
        PSVECScale(HV(&tgt), HV(&tgt), 400.0f);
        tgt.x += pos.x;
        tgt.z += pos.z;
    }
    if (found != 0) {
        limit = 400.0;
    } else {
        limit = 250.0;
    }
    mag = M522Mag(&tgt);
    if (mag > limit || found != 0) {
        PSVECScale(HV(&tgt), HV(&tgt), limit / mag);
        fn_1_14708(self, &tgt, 0.016666666666666666 * (100.0 * (2.0 + 0.3 * (f64)self->dif)));
    }
}

u8 fn_1_144D4(M522_COM *self, M522_VEC3 *out, f32 radius)
{
    M522_HANDLES *list;
    s32 *it;
    s32 *end;
    M522_VEC3 pos;
    M522_VEC3 p;
    M522_VEC3 d;
    f32 r;
    f32 dist;
    s32 count;
    u8 any;

    r = radius;
    if (lbl_1_bss_1C == 0) {
        fn_1_6A70(lbl_1_data_5EB, lbl_1_data_5FB, 0x27);
    }
    list = fn_1_4EFC(lbl_1_bss_1C);
    it = list->m_data;
    end = list->m_data + list->m_size;
    any = 0;
    count = 0;
    MgActorPosGet(self->mg->actor, HV(&pos));
    out->x = 0.0f;
    out->y = 0.0f;
    out->z = 0.0f;
    while (it != end) {
        fn_1_4F88(fn_1_E5C8(*it), &p);
        d.x = p.x - pos.x;
        d.y = 0.0f;
        d.z = p.z - pos.z;
        dist = M522Mag(&d);
        if (dist <= r) {
            r = dist;
            PSVECAdd(HV(&p), HV(out), HV(out));
            count++;
            any = 1;
        }
        it++;
    }
    if (count > 0) {
        PSVECScale(HV(out), HV(out), (f32)(1.0 / (f64)count));
    }
    return any;
}

s32 fn_1_14708(M522_COM *self, M522_VEC3 *target, f32 maxDist)
{
    M522_VEC3 cur;
    M522_VEC3 d;
    f32 limit;
    f32 mag;
    f64 dx;
    f64 dz;

    limit = M522Min(maxDist, 10.0f);
    MgActorPosGet(self->mg->actor, HV(&cur));
    dz = target->z - cur.z;
    dx = target->x - cur.x;
    d.x = dx;
    d.y = 0.0f;
    d.z = dz;
    mag = M522Mag(&d);
    if (mag < limit) {
        return 1;
    }
    PSVECScale(HV(&d), HV(&d), limit / mag);
    fn_1_148DC(&self->pad, &d);
    return 0;
}

void fn_1_14868(M522_PAD *self)
{
    self->x = self->x * 0.9;
    self->y = self->y * 0.9;
    self->btnDown = 0;
    self->btn = 0;
}

void fn_1_148B4(M522_PAD *self, u8 down)
{
    self->btn = self->btn | 0x100;
    if (down != 0) {
        self->btnDown = self->btnDown | 0x100;
    }
}

s32 fn_1_148DC(M522_PAD *self, M522_VEC3 *dir)
{
    M522_VEC3 d = lbl_1_rodata_6C0;
    M522_ANGLE a;
    f32 ang;
    f32 k;
    f32 mag;
    s32 result;

    d.x = dir->x;
    d.z = dir->z;
    fn_1_14D04(&a, -3.1415927f, 3.1415927f, self->angle);
    ang = atan2f(d.x, d.z);
    k = (f32)(1.0 - (fabs(fn_1_14DF4(&a, ang) * 0.5f) / 3.141592653589793));
    mag = k * M522Mag(&d);
    self->angle = fn_1_14E30(&a, ang, 0.05);
    self->x = (f64)mag * sin(fn_1_14F40(&a));
    self->y = -mag * cos(fn_1_14F40(&a));
    result = 0;
    if (self->x == 0.0f && self->y == 0.0f) {
        result = 1;
    }
    return result;
}

void fn_1_14B20(M522_PAD *self, MGPLAYER *player)
{
    f32 sx;
    f32 sy;
    f32 cx;
    f32 cy;

    sx = self->x / 10.0f;
    if (sx < -1.0f) {
        cx = -1.0f;
    } else if (sx > 1.0f) {
        cx = 1.0f;
    } else {
        cx = sx;
    }
    sy = self->y / 10.0f;
    if (sy < -1.0f) {
        cy = -1.0f;
    } else if (sy > 1.0f) {
        cy = 1.0f;
    } else {
        cy = sy;
    }
    MgPlayerPadSet(player, (s32)(56.0f * cx), (s32)(56.0f * cy), self->btnDown, self->btn | self->btnDown);
    self->x = self->x * 0.9;
    self->y = self->y * 0.9;
    self->btnDown = 0;
    self->btn = 0;
}

void fn_1_14CFC(M522_PLAYERBASE *self)
{
}

void fn_1_14D00(M522_PLAYERBASE *self)
{
}

M522_ANGLE *fn_1_14D04(M522_ANGLE *self, f32 a, f32 b, f32 cur)
{
    self->min = M522Min(a, b);
    self->max = M522Max(a, b);
    self->range = self->max - self->min;
    self->cur = fn_1_14EA4(self, cur);
    return self;
}

f32 fn_1_14DF4(M522_ANGLE *self, f32 x)
{
    return fn_1_15430(self, self->cur, x);
}

f32 fn_1_14E30(M522_ANGLE *self, f32 target, f64 rate)
{
    self->cur = fn_1_14EA4(self, self->cur + rate * fn_1_15430(self, self->cur, target));
    return self->cur;
}

f32 fn_1_14EA4(M522_ANGLE *self, f32 x)
{
    if (x < self->min) {
        return self->max - (f32)fmod(self->min - x, self->range);
    }
    if (x >= self->max) {
        return self->min + (f32)fmod(x - self->max, self->range);
    }
    return x;
}

f32 fn_1_14F40(M522_ANGLE *self)
{
    return self->cur;
}

s16 *fn_1_14F48(M522_U16VEC *self, u32 index)
{
    if (index >= self->m_size) {
        fn_1_6AD0(M522_STDERR, lbl_1_data_780);
        abort();
    }
    return self->m_data + index;
}

void fn_1_14FC0(M522_NODE *head, void (*func)(M522_PLAYERBASE *))
{
    M522_NODE *end = head;
    M522_NODE *first = head->prev;
    M522_NODE *tmp = first;
    fn_1_15020(tmp, end, func);
}

void fn_1_15020(M522_NODE *first, M522_NODE *end, void (*func)(M522_PLAYERBASE *))
{
    M522_NODE *it;

    it = first;
    while (it != end) {
        func((M522_PLAYERBASE *)it);
        it = it->prev;
    }
}

void fn_1_15080(M522_U16VEC *self, s16 *pos, u32 n, s16 *xp)
{
    const s16 *px;
    u32 max;
    s16 *end;
    s16 *dst;
    s16 *oldData;
    s32 after;
    s32 index;
    u32 newSize;
    u32 newCap;
    u32 k;

    px = xp;
    if (n != 0) {
        max = M522_U16VEC::maxSize();
        if (n > max || self->m_size > max - n) {
            fn_1_6AD0(M522_STDERR, lbl_1_data_79E);
            abort();
        }
        if (self->m_size + n <= self->capacity()) {
            end = self->end();
            after = end - pos;
            if (end - (after - n) <= px && px < end) {
                px += n;
            }
            if ((u32)after != 0) {
                memmove(end + n - after, pos, after * 2);
            }
            k = n;
            dst = pos;
            while (k != 0) {
                *dst = *px;
                dst++;
                k--;
            }
            self->m_size = self->m_size + n;
            return;
        }
        index = pos - self->begin();
        oldData = self->m_data;
        newSize = self->m_size + n;
        if (self->m_cap != 0) {
            newCap = self->m_cap;
        } else {
            newCap = 1;
        }
        while (newSize > newCap) {
            if (newCap < (max >> 1)) {
                newCap = newCap * 2;
            } else {
                newCap = max;
            }
        }
        self->m_data = (s16 *)fn_1_689C(newCap * 2);
        if (self->m_data == 0) {
            fn_1_6AD0(M522_STDERR, lbl_1_data_7BB);
            abort();
        }
        k = n;
        dst = self->m_data + index;
        while (k != 0) {
            *dst = *px;
            dst++;
            k--;
        }
        if (oldData != 0) {
            memcpy(self->m_data, oldData, index * 2);
            memcpy(self->m_data + index + n, oldData + index, (oldData + self->m_size - (oldData + index)) * 2);
            fn_1_6964(oldData);
        }
        self->m_size = newSize;
        self->m_cap = newCap;
    }
}

f32 fn_1_15430(M522_ANGLE *self, f32 a, f32 b)
{
    f32 wb;
    f32 wa;
    f32 diff;
    f32 absDiff;
    f32 result;
    s32 sign;

    if (b < self->min) {
        wb = self->max - (f32)fmod(self->min - b, self->range);
    } else if (b >= self->max) {
        wb = self->min + (f32)fmod(b - self->max, self->range);
    } else {
        wb = b;
    }
    if (a < self->min) {
        wa = self->max - (f32)fmod(self->min - a, self->range);
    } else if (a >= self->max) {
        wa = self->min + (f32)fmod(a - self->max, self->range);
    } else {
        wa = a;
    }
    diff = wb - wa;
    if (diff < 0.0f) {
        absDiff = -diff;
    } else {
        absDiff = diff;
    }
    if (absDiff < self->range * 0.5f) {
        result = diff;
    } else {
        if (diff < 0.0f) {
            sign = 1;
        } else {
            sign = -1;
        }
        result = (self->range - absDiff) * (f32)sign;
    }
    return result;
}

void fn_1_15648(void)
{
    lbl_1_bss_A48 = 0x78;
    lbl_1_bss_A4C = 0xD2;
    lbl_1_bss_A50 = 0xD2;
    fn_1_8004(&lbl_1_bss_A64);
    __register_global_object(&lbl_1_bss_A64, (void *)fn_1_156D0, &lbl_1_bss_A58);
}

void *fn_1_156D0(M522_NODE *self, s16 flag)
{
    if (self != 0) {
        fn_1_8010(self, 0);
        if (flag > 0) {
            fn_1_6964(self);
        }
    }
    return self;
}
