#include "dolphin/types.h"
#include "dolphin/mtx.h"
#include "dolphin/gx.h"
#include "game/gamework.h"
#include "game/pad.h"
#include "REL/m521Dll/m521Types.h"

/* Controllers of m521Dll: the pad controller (CDriverAsPad), the CPU controller
 * (CDriverAsAI) and the small deque template the CPU controller queues its
 * behaviours in (cdeque = circular buffer of block pointers, deque = blocks). */

#ifdef __MWERKS__
#define DRV_RODATA __declspec(section ".rodata")
#else
#define DRV_RODATA
#endif

/* A behaviour of the CPU controller: void (CDriverAsAI::*)(CDriver::INFO &).
 * The GameCube slot is the MWCC pointer-to-member (this-delta, vtable index,
 * function); on PC it is the function pointer. */
#ifdef TARGET_PC
typedef struct DrvPtmf_s {
    void (*fn)(void *self, void *info);
} DrvPtmf;
#define DRV_PTMF_INIT(f) { (void (*)(void *, void *))(f) }
#define DrvPtmfCall(obj, p, info) ((p)->fn((obj), (info)))
#else
typedef M521Ptmf DrvPtmf;
#define DRV_PTMF_INIT(f) M521_PTMF_INIT(f)
/* the MWCC call sequence also loads r12 with the PTMF address */
#define DrvPtmfCall(obj, p, info) ((void (*)(void *, void *))__ptmf_scall)((obj), (info))
#endif

struct CDriverAsAI;

/* One queued behaviour: object plus pointer-to-member (16 bytes on GC). */
struct DrvTask {
    CDriverAsAI *m_obj;
    DrvPtmf m_pmf;
};

/* cdeque<DrvTask *>: circular buffer of block pointers. */
struct CDequeMap {
    u32 m_cap;        /* 0x00 */
    u32 m_size;       /* 0x04 */
    u32 m_start;      /* 0x08 */
    DrvTask **m_data; /* 0x0C */
};

/* deque<DrvTask>: m_blk tasks per block, blocks addressed through the map. */
struct CDeque {
    u32 m_blk;       /* 0x00 */
    CDequeMap m_map; /* 0x04 */
    u32 m_off;       /* 0x14 index of the first task in the first block */
    u32 m_cnt;       /* 0x18 */
};

/* Iterator of the map (3 words) and of the deque (6 words). */
struct CMapIt {
    CDequeMap *m_map;
    u32 m_idx;
    DrvTask **m_slot;
    u8 m_tag;
};

struct CDequeIt {
    CDequeMap *m_map;
    u32 m_idx;
    DrvTask **m_slot;
    DrvTask *m_cur;
    DrvTask *m_end;
    u32 m_pos;
};

/* Pad controller (0x20 bytes): input comes from the pad of the character. */
struct CDriverAsPad : CCtl {
    s32 m_charIdx; /* 0x18 */
    s32 m_padNo;   /* 0x1C */
};

/* CPU controller (0x94 bytes). */
struct CDriverAsAI : CCtl {
    CModel m_model;   /* 0x18 debug-draw hook model */
    CDeque m_queue;   /* 0x1C queued behaviours */
    CPlayer *m_owner; /* 0x38 */
    s32 m_side;       /* 0x3C clock hand side */
    s32 m_hit;        /* 0x40 hand hit by the owner's front probe */
    M521Vec m_nrm;    /* 0x44 normal of that hit */
    f32 m_pen;        /* 0x50 */
    M521Vec m_dir;    /* 0x54 */
    s32 m_timer;      /* 0x60 */
    s32 m_cool;       /* 0x64 */
    s32 m_wait;       /* 0x68 */
    s32 m_tick;       /* 0x6C */
    f32 m_f70;        /* 0x70 */
    u8 m_flag74;      /* 0x74 */
    M521Vec m_tgt;    /* 0x78 */
    f32 m_f84[3];     /* 0x84 */
    s32 m_state;      /* 0x90 */
};

/* The INFO a controller fills every frame. */
typedef CCtlInput DrvInfo;

extern "C" {
double sin(double x);
void abort(void);
void __dl__FPv(void *ptr);
void __dt__6CModelFv(CModel *self, s32 freeIt);

/* MSL sqrt(double) as the original inlines it (no pool of its own) */
double __frsqrte(double x);
extern int __float_nan[];
extern int __float_huge[];

static inline double drv_sqrt(double x)
{
    if (x > 0.0) {
        double guess = __frsqrte(x);
        guess = .5 * guess * (3.0 - guess * guess * x);
        guess = .5 * guess * (3.0 - guess * guess * x);
        guess = .5 * guess * (3.0 - guess * guess * x);
        guess = .5 * guess * (3.0 - guess * guess * x);
        return x * guess;
    } else if (x == 0)
        return 0;
    else if (x)
        return *(float *)__float_nan;

    return *(float *)__float_huge;
}

// stderr of the C library (element 2 of the FILE table)
extern u8 __files[];
#define DRV_STDERR ((void *)(__files + 0xA0))
int fn_1_4C9C(void *file, const char *msg, ...);

// singleton.h assertion
void fn_1_4C3C(const char *cond, const char *file, s32 line);

void *fn_1_4A68(u32 n);

// CModel (m521 unit)
CModel *fn_1_7744(CModel *self, void (*hook)(void *, void *));
void fn_1_7AB4(CModel *self, s16 layer);
void fn_1_7AEC(CModel *self, u16 cameraBit);
void fn_1_7B24(const HU3DMODELID *modelIdPtr, void *data);
f32 fn_1_9A30(f32 min, f32 max);

// clock (clock unit)
void fn_1_658(CClock *self, M521Vec *out, s32 idx);
void fn_1_6B8(CClock *self, M521Vec *out, s32 idx, f32 along, f32 side);
u8 fn_1_1370(CClock *self, s32 idx);
s32 fn_1_1CA0(CClock *self, const M521Vec *pt, M521Vec *outNormal, f32 *outPen);
s32 fn_1_2480(CClock *self, s32 idx);
s32 fn_1_24D0(CClock *self, f32 angle);
s32 fn_1_2EFC(CClock *self, s32 idx);
s32 fn_1_42C8(CClock *self, const M521Vec *pt, s32 id);

// player (player unit)
s32 fn_1_15D68(CPlayer *self);
CClock *fn_1_1650C(CPlayer *self);
void fn_1_17578(CPlayer *self, M521Vec *out);
void fn_1_15F04(CPlayer *self);
void fn_1_17840(CPlayer *self);
void fn_1_17920(CPlayer *self);

extern s32 lbl_1_bss_A48;
extern s8 lbl_1_bss_A5C;

// controllers
void fn_1_B768(CDriverAsAI *self, DrvInfo *info);
void fn_1_B040(CDriverAsPad *self, DrvInfo *info);
CDriverAsAI *fn_1_D9D4(CDriverAsAI *self, s16 freeIt);
CCtl *fn_1_DAA4(CCtl *self, s16 freeIt);
CCtl *fn_1_AFE8(CCtl *self, s16 freeIt);

void fn_1_C060(CDriverAsAI *self);
void fn_1_CDDC(CDriverAsAI *self, DrvInfo *info, s32 y, f32 limit);
void fn_1_D0F0(CDriverAsAI *self, const DrvPtmf *p);
void fn_1_D1F8(CDriverAsAI *self, const DrvPtmf *p);
void fn_1_D26C(CDriverAsAI *self);
void fn_1_D298(CDriverAsAI *self, s32 state, const char *func);
void fn_1_D2AC(void);
void proc00__Q24m52111CDriverAsAIFRQ34m5217CDriver4INFO(CDriverAsAI *self, DrvInfo *info);
void proc01__Q24m52111CDriverAsAIFRQ34m5217CDriver4INFO(CDriverAsAI *self, DrvInfo *info);
void proc02__Q24m52111CDriverAsAIFRQ34m5217CDriver4INFO(CDriverAsAI *self, DrvInfo *info);
void proc0e__Q24m52111CDriverAsAIFRQ34m5217CDriver4INFO(CDriverAsAI *self, DrvInfo *info);
void proc0f__Q24m52111CDriverAsAIFRQ34m5217CDriver4INFO(CDriverAsAI *self, DrvInfo *info);
void fn_1_C790(CDriverAsAI *self, DrvInfo *info);
void fn_1_C9C8(CDriverAsAI *self, DrvInfo *info);

// the deque template (cdeque<DrvTask *> and deque<DrvTask>)
CDequeMap *fn_1_E2A8(CDequeMap *self, s16 freeIt);
CDequeMap *fn_1_E31C(CDequeMap *self, s16 freeIt);
void fn_1_DB3C(CDeque *self);
void fn_1_DDEC(CDeque *self);
void fn_1_DFE0(CDeque *self, const DrvTask *item);
CDeque *fn_1_E388(CDeque *self, const CDeque *src);
void fn_1_E6F0(CDeque *self, const void *unused);
void fn_1_E748(CDequeMap *self);
void fn_1_E784(CDequeMap *self, DrvTask *const *val);
void fn_1_EA80(CDequeMap *self, u32 n);
void fn_1_ECA4(CDequeMap *self, u32 n, DrvTask *val);
u8 fn_1_EF30(CDeque *self, s32 front, CDequeMap *spare, u32 *count, s32 n);
void fn_1_F59C(CDequeMap *self, DrvTask *const *val);
void fn_1_F5CC(CDequeIt *out, const CDeque *self, u32 idx);
void fn_1_F724(CDequeIt *out, CDeque *self, u32 idx);
void fn_1_F87C(CDeque *self, CDequeIt first, CDequeIt last);
void fn_1_FBA8(CDequeMap *self, CMapIt first, CMapIt last, const u8 *tag);
void fn_1_FF40(CDequeMap *self, CDequeMap *other);
void fn_1_FFB4(CMapIt *out, CDequeMap *self, CMapIt first, CMapIt last);
void fn_1_104A8(CDequeMap *self, CMapIt pos, u32 n, DrvTask *const *val);
u32 fn_1_1147C(CDequeMap *self);
DrvTask **fn_1_DB10(CDequeMap *self, u32 idx);

// strings of the template
const char lbl_1_data_40B[] = "Memory allocation failure";

#define DRV_ABORT_ALLOC()                      \
    do {                                       \
        fn_1_4C9C(DRV_STDERR, lbl_1_data_40B); \
        abort();                               \
    } while (0)

extern const u32 lbl_1_rodata_238;
u32 lbl_1_data_428 = 1;
u32 lbl_1_data_46C = 1;

s32 lbl_1_bss_A00;
s32 lbl_1_bss_A04;
s32 lbl_1_bss_A08;
s32 lbl_1_bss_A0C;
u8 lbl_1_bss_A10;

// {0, -1, proc}: behaviours the CPU controller queues
const DrvPtmf lbl_1_data_250 = DRV_PTMF_INIT(proc00__Q24m52111CDriverAsAIFRQ34m5217CDriver4INFO);
const DrvPtmf lbl_1_data_25C = DRV_PTMF_INIT(fn_1_C790);
const DrvPtmf lbl_1_data_268 = DRV_PTMF_INIT(proc01__Q24m52111CDriverAsAIFRQ34m5217CDriver4INFO);
f32 lbl_1_data_2A8 = 50.0f;
const DrvPtmf lbl_1_data_2AC = DRV_PTMF_INIT(proc0f__Q24m52111CDriverAsAIFRQ34m5217CDriver4INFO);
const DrvPtmf lbl_1_data_2EC = DRV_PTMF_INIT(proc02__Q24m52111CDriverAsAIFRQ34m5217CDriver4INFO);
const DrvPtmf lbl_1_data_2F8 = DRV_PTMF_INIT(proc0e__Q24m52111CDriverAsAIFRQ34m5217CDriver4INFO);
const DrvPtmf lbl_1_data_304 = DRV_PTMF_INIT(proc01__Q24m52111CDriverAsAIFRQ34m5217CDriver4INFO);
const DrvPtmf lbl_1_data_310 = DRV_PTMF_INIT(proc0e__Q24m52111CDriverAsAIFRQ34m5217CDriver4INFO);
const DrvPtmf lbl_1_data_31C = DRV_PTMF_INIT(fn_1_C9C8);

// vtables: {0, 0, destructor, update}
void *lbl_1_data_3BC[4] = { 0, 0, (void *)fn_1_D9D4, (void *)fn_1_B768 };
void *lbl_1_data_3CC[4] = { 0, 0, (void *)fn_1_DAA4, (void *)fn_1_B040 };
void *lbl_1_data_3DC[4] = { 0, 0, (void *)fn_1_AFE8, 0 };

// slots of the player / score handlers, called through the object list (gamemaster)
const M521Ptmf lbl_1_data_4CC = M521_PTMF_INIT(fn_1_15F04);
const M521Ptmf lbl_1_data_520 = M521_PTMF_INIT(fn_1_17840);
const M521Ptmf lbl_1_data_52C = M521_PTMF_INIT(fn_1_17920);

void fn_1_AF00(CCtl *self)
{
    self->m_in.m_x = 0.0f;
    self->m_in.m_z = 0.0f;
    self->m_in.m_mag = 0.0f;
    self->m_in.m_btn = 0;
    ((void (*)(CCtl *, CCtlInput *))self->m_vtable[3])(self, &self->m_in);
}

CCtl *fn_1_AF78(CDriverAsPad *self, s32 charIdx)
{
    self->m_vtable = lbl_1_data_3DC;
    self->m_in.m_x = 0.0f;
    self->m_in.m_z = 0.0f;
    self->m_in.m_mag = 0.0f;
    self->m_in.m_btn = 0;
    self->m_vtable = lbl_1_data_3CC;
    self->m_charIdx = charIdx;
    self->m_padNo = GwPlayerConf[charIdx].padNo;
    return self;
}

CCtl *fn_1_AFE8(CCtl *self, s16 freeIt)
{
    if (self != 0) {
        self->m_vtable = lbl_1_data_3DC;
        if (freeIt > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void fn_1_B040(CDriverAsPad *self, DrvInfo *info)
{
    s32 pad = self->m_padNo;
    f32 sx = 0.017857144f * (f32)HuPadStkX[pad];
    f32 sy = 0.017857144f * (f32)HuPadStkY[pad];
    double x = sin(3.141592653589793 * (f64)sx * 0.5);
    double y = sin((3.141592653589793 * (f64)sy) / -2.0);
    double len = drv_sqrt(x * x + y * y);
    if (len > 1.0) {
        double inv = 1.0 / len;
        x *= inv;
        y *= inv;
        len = 1.0;
    }
    info->m_x = (f32)x;
    info->m_z = (f32)y;
    info->m_mag = (f32)len;
    info->m_btn = (HuPadBtnDown[pad] & 0x100) != 0;
}

CDriverAsAI *fn_1_B3CC(CDriverAsAI *self, CPlayer *owner)
{
    CDeque tmp;
    s32 dummy;

    self->m_vtable = lbl_1_data_3DC;
    self->m_in.m_x = 0.0f;
    self->m_in.m_z = 0.0f;
    self->m_in.m_mag = 0.0f;
    self->m_in.m_btn = 0;
    fn_1_7744(&self->m_model, (void (*)(void *, void *))fn_1_D2AC);
    self->m_vtable = lbl_1_data_3BC;
    fn_1_E6F0(&tmp, &dummy);
    fn_1_E388(&self->m_queue, &tmp);
    fn_1_DB3C(&tmp);
    if (tmp.m_map.m_data != 0) {
        __dl__FPv(tmp.m_map.m_data);
    }
    self->m_owner = owner;
    self->m_timer = 0;
    self->m_cool = 0;
    self->m_wait = 0;
    self->m_state = -1;
    fn_1_7B24(&self->m_model.m_id, self);
    fn_1_7AB4(&self->m_model, 5);
    fn_1_7AEC(&self->m_model, 1);
    self->m_flag74 = 0;
    return self;
}

CDeque *fn_1_B4FC(CDeque *self, s16 freeIt)
{
    if (self != 0) {
        if (self != 0) {
            fn_1_DB3C(self);
            if (&self->m_map != 0 && &self->m_map != 0 && self->m_map.m_data != 0) {
                __dl__FPv(self->m_map.m_data);
            }
        }
        if (freeIt > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CDeque *fn_1_B590(CDeque *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_DB3C(self);
        if (&self->m_map != 0 && &self->m_map != 0 && self->m_map.m_data != 0) {
            __dl__FPv(self->m_map.m_data);
        }
        if (freeIt > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

/* Number of frames a CPU of difficulty lv (0..1) waits before reacting. */
s32 fn_1_B61C(f32 lv)
{
    u8 ok = 0;
    f32 inv;
    f32 a;
    f32 b;

    if (lv >= 0.0f && lv <= 1.0f) {
        ok = 1;
    }
    if (ok) {
    } else {
        fn_1_4C3C("lv >= 0 && lv <= 1", "driver.cpp", 0x4B);
    }
    inv = 1.0f - lv;
    a = fn_1_9A30(1.0f, 6.0f);
    b = fn_1_9A30(0.5f, 1.0f);
    return (s32)(60.0f * (inv + (b + a) * inv));
}

void fn_1_B768(CDriverAsAI *self, DrvInfo *info)
{
    CPlayer *owner;
    CPlayer *partner;
    CClock *clock;
    M521Vec ahead;
    M521Vec probe;
    DrvPtmf pmf;
    DrvTask *task;
    u8 done;
    u32 at;

    self->m_tick++;
    if (lbl_1_bss_A5C == 0) {
        self->m_cool = lbl_1_bss_A0C;
        return;
    }
    owner = self->m_owner;
    partner = owner->m_partner;
    self->m_side = ((lbl_1_bss_A48 >> ((owner->m_slot >> 1) & 1)) & 1) ^ (owner->m_slot & 1);
    clock = fn_1_1650C(owner);
    if (fn_1_15D68(partner) == 0) {
        if (self->m_flag74 != 0) {
            self->m_flag74 = fn_1_24D0(clock, 80.0f);
        } else {
            self->m_flag74 = fn_1_24D0(clock, 40.0f);
        }
    }
    if (self->m_flag74 == 0) {
        if (fn_1_1370(clock, self->m_side) != 0) {
            if (fn_1_15D68(partner) == 0 && partner->m_timer <= 0) {
                if (owner->m_slot < 2) {
                    lbl_1_bss_A48 ^= 1;
                } else {
                    lbl_1_bss_A48 ^= 2;
                }
            }
            return;
        }
        if (fn_1_15D68(partner) != 0) {
            if (fn_1_2480(clock, self->m_side) != 0) {
                return;
            }
        }
    } else {
        self->m_side = 1;
    }
    fn_1_17578(owner, &ahead);
    PSVECScale(&ahead, &ahead, 20.0f);
    PSVECAdd((Vec *)&owner->m_pos, &ahead, &probe);
    self->m_hit = fn_1_1CA0(clock, &probe, &self->m_nrm, &self->m_pen);
    if (self->m_queue.m_cnt == 0) {
        self->m_cool = fn_1_B61C(info->m_scale);
        self->m_f70 = owner->m_f60;
        pmf = lbl_1_data_250;
        fn_1_D1F8(self, &pmf);
    }
    at = self->m_queue.m_off + (self->m_queue.m_cnt - 1);
    task = *fn_1_DB10(&self->m_queue.m_map, at / self->m_queue.m_blk) + (at % self->m_queue.m_blk);
    DrvPtmfCall(task->m_obj, &task->m_pmf, info);
    if (info->m_mag != 0.0f || info->m_btn == 0) {
        self->m_cool = lbl_1_bss_A0C;
    }
    done = 0;
    if (self->m_cool > 0) {
        if (--self->m_cool == 0) {
            done = 1;
        }
    }
    if (done) {
        self->m_cool = lbl_1_bss_A0C;
        pmf = lbl_1_data_25C;
        fn_1_D1F8(self, &pmf);
    }
}

void proc00__Q24m52111CDriverAsAIFRQ34m5217CDriver4INFO(CDriverAsAI *self, DrvInfo *info)
{
    u8 done;
    DrvPtmf pmf;
    f32 ang;

    fn_1_D298(self, 0xA2, "m521::CDriverAsAI::proc00(m521::CDriver::INFO &)");
    done = 0;
    if (self->m_timer > 0) {
        if (--self->m_timer == 0) {
            done = 1;
        }
    }
    if (done || self->m_timer <= 0) {
        pmf = lbl_1_data_268;
        fn_1_D0F0(self, &pmf);
        return;
    }
    ang = (f32)((3.141592653589793 * (f64)((self->m_timer % 360) * 2)) / 180.0);
    self->m_owner->m_f64 = (f32)(45.0 * sin(ang)) + self->m_f70;
}

void proc01__Q24m52111CDriverAsAIFRQ34m5217CDriver4INFO(CDriverAsAI *self, DrvInfo *info)
{
    CPlayer *owner;
    CClock *clock;
    f32 along;
    f32 hands;
    M521Vec goal;
    M521Vec d;
    DrvPtmf pmf;
    f32 sq;
    f32 lim;

    fn_1_D298(self, 0xB2, "m521::CDriverAsAI::proc01(m521::CDriver::INFO &)");
    owner = self->m_owner;
    clock = fn_1_1650C(owner);
    hands = (f32)fn_1_2EFC(clock, self->m_side);
    if (self->m_flag74 == 0) {
        along = 0.5f;
    } else {
        along = 0.8f;
    }
    fn_1_6B8(clock, &goal, self->m_side, along, 0.5f * (100.0f * hands));
    self->m_tgt.x = goal.x;
    self->m_tgt.y = goal.y;
    self->m_tgt.z = goal.z;
    PSVECSubtract(&goal, (Vec *)&owner->m_pos, &d);
    d.y = 0.0f;
    sq = PSVECSquareMag(&d);
    lim = lbl_1_data_2A8 * lbl_1_data_2A8;
    if (sq < lim && fn_1_42C8(clock, (M521Vec *)&owner->m_pos, self->m_side) != 0) {
        fn_1_C060(self);
        return;
    }
    PSVECNormalize(&d, &d);
    if (self->m_hit >= 0 && self->m_flag74 == 0) {
        if (fn_1_42C8(clock, (M521Vec *)&owner->m_pos, self->m_side) != 0 || self->m_flag74 != 0) {
            fn_1_C060(self);
            return;
        }
        if (self->m_pen > 0.0f) {
            PSVECScale(&self->m_nrm, &self->m_nrm, -1.0f);
        }
        self->m_dir.x = self->m_nrm.x;
        self->m_dir.y = self->m_nrm.y;
        self->m_dir.z = self->m_nrm.z;
        self->m_timer = 6;
        pmf = lbl_1_data_2AC;
        fn_1_D1F8(self, &pmf);
        return;
    }
    info->m_x = d.x;
    info->m_z = d.z;
    info->m_mag = 1.0f;
}

void fn_1_C060(CDriverAsAI *self)
{
    CClock *clock;
    DrvPtmf pmf;
    DrvPtmf pmf2;
    f32 scale;

    clock = fn_1_1650C(self->m_owner);
    pmf = lbl_1_data_2EC;
    fn_1_D0F0(self, &pmf);
    fn_1_658(clock, &self->m_dir, self->m_side);
    scale = (f32)(-fn_1_2EFC(clock, self->m_side));
    PSVECScale(&self->m_dir, &self->m_dir, scale);
    pmf2 = lbl_1_data_2F8;
    fn_1_D1F8(self, &pmf2);
}

u8 fn_1_C164(CDriverAsAI *self)
{
    CPlayer *owner;
    CClock *clock;
    M521Vec ahead;
    M521Vec probe;
    f32 pen;
    s32 hit;
    u8 res;

    owner = self->m_owner;
    clock = fn_1_1650C(owner);
    fn_1_17578(owner, &ahead);
    PSVECScale(&ahead, &ahead, 20.0f);
    PSVECAdd((Vec *)&owner->m_pos, &ahead, &probe);
    hit = fn_1_1CA0(clock, &probe, &self->m_nrm, &pen);
    res = 0;
    if (hit >= 0 && hit != self->m_side) {
        res = 1;
    }
    return res;
}

void proc02__Q24m52111CDriverAsAIFRQ34m5217CDriver4INFO(CDriverAsAI *self, DrvInfo *info)
{
    CPlayer *owner;
    CClock *clock;
    M521Vec goal;
    DrvPtmf pmf;
    DrvPtmf pmf2;
    DrvPtmf pmf3;
    f32 hands;
    f32 dist;

    fn_1_D298(self, 0x103, "m521::CDriverAsAI::proc02(m521::CDriver::INFO &)");
    owner = self->m_owner;
    clock = fn_1_1650C(owner);
    if (fn_1_42C8(clock, (M521Vec *)&owner->m_pos, self->m_side) == 0) {
        while (self->m_queue.m_cnt != 0) {
            fn_1_DDEC(&self->m_queue);
        }
        return;
    }
    if (fn_1_C164(self) != 0 && self->m_flag74 == 0) {
        pmf = lbl_1_data_304;
        fn_1_D0F0(self, &pmf);
        fn_1_6B8(clock, &goal, self->m_side, 1.0f, 0.0f);
        PSVECSubtract(&goal, (Vec *)&owner->m_pos, &self->m_dir);
        self->m_dir.y = 0.0f;
        PSVECNormalize(&self->m_dir, &self->m_dir);
        pmf2 = lbl_1_data_310;
        fn_1_D1F8(self, &pmf2);
        pmf3 = lbl_1_data_31C;
        fn_1_D1F8(self, &pmf3);
        return;
    }
    hands = (f32)fn_1_2EFC(clock, self->m_side);
    if (self->m_flag74 == 0) {
        if (self->m_side == 0) {
            dist = 200.0f;
        } else {
            dist = 250.0f;
        }
        fn_1_CDDC(self, info, (s32)-hands, dist);
        if (fn_1_1370(clock, self->m_side) != 0) {
            while (self->m_queue.m_cnt != 0) {
                fn_1_DDEC(&self->m_queue);
            }
        }
    } else {
        fn_1_CDDC(self, info, (s32)-hands, 280.0f);
    }
}

void fn_1_C51C(CDriverAsAI *self, DrvInfo *info)
{
    CPlayer *owner;
    M521Vec d;
    u8 done;
    f32 dist;

    done = 0;
    if (self->m_wait > 0) {
        if (--self->m_wait == 0) {
            done = 1;
        }
    }
    if (done || self->m_wait <= 0) {
        fn_1_DDEC(&self->m_queue);
    }
    owner = self->m_owner;
    PSVECSubtract((Vec *)&owner->m_pos, &fn_1_1650C(owner)->m_pos, &d);
    d.y = 0.0f;
    dist = (f32)drv_sqrt(PSVECSquareMag(&d));
    if (dist <= 120.0) {
    } else {
        PSVECNormalize(&d, &d);
        info->m_x = -d.x;
        info->m_z = -d.z;
        info->m_mag = 1.0f;
    }
}

void fn_1_C790(CDriverAsAI *self, DrvInfo *info)
{
    CPlayer *owner;
    M521Vec d;
    f32 dist;

    owner = self->m_owner;
    PSVECSubtract((Vec *)&owner->m_pos, &fn_1_1650C(owner)->m_pos, &d);
    d.y = 0.0f;
    dist = (f32)drv_sqrt(PSVECSquareMag(&d));
    PSVECNormalize(&d, &d);
    if (dist <= 120.0) {
        fn_1_DDEC(&self->m_queue);
        return;
    }
    info->m_x = -d.x;
    info->m_z = -d.z;
    info->m_mag = 1.0f;
}

void fn_1_C9C8(CDriverAsAI *self, DrvInfo *info)
{
    CPlayer *owner;
    M521Vec d;
    f32 dist;

    owner = self->m_owner;
    PSVECSubtract((Vec *)&owner->m_pos, &fn_1_1650C(owner)->m_pos, &d);
    d.y = 0.0f;
    dist = (f32)drv_sqrt(PSVECSquareMag(&d));
    PSVECNormalize(&d, &d);
    if (dist >= 300.0f) {
        fn_1_DDEC(&self->m_queue);
        return;
    }
    info->m_x = d.x;
    info->m_z = d.z;
    info->m_mag = 1.0f;
}

void proc0e__Q24m52111CDriverAsAIFRQ34m5217CDriver4INFO(CDriverAsAI *self, DrvInfo *info)
{
    CPlayer *owner;
    M521Vec fwd;
    f32 dot;

    fn_1_D298(self, 0x173, "m521::CDriverAsAI::proc0e(m521::CDriver::INFO &)");
    owner = self->m_owner;
    fn_1_17578(owner, &fwd);
    dot = PSVECDotProduct(&self->m_dir, &fwd);
    if (dot >= 0.9f) {
        fn_1_DDEC(&self->m_queue);
        self->m_tick = 0;
        return;
    }
    info->m_x = self->m_dir.x;
    info->m_z = self->m_dir.z;
    info->m_mag = 0.8f;
}

void proc0f__Q24m52111CDriverAsAIFRQ34m5217CDriver4INFO(CDriverAsAI *self, DrvInfo *info)
{
    u8 done;

    fn_1_D298(self, 0x184, "m521::CDriverAsAI::proc0f(m521::CDriver::INFO &)");
    self->m_dir.y = 0.0f;
    if (0.0f == PSVECMag(&self->m_dir)) {
        self->m_dir.x = 1.0f;
    } else {
        PSVECNormalize(&self->m_dir, &self->m_dir);
    }
    info->m_x = self->m_dir.x;
    info->m_z = self->m_dir.z;
    info->m_mag = 1.0f;
    done = 0;
    if (self->m_timer > 0) {
        if (--self->m_timer == 0) {
            done = 1;
        }
    }
    if (done) {
        info->m_btn = 1;
        fn_1_D26C(self);
    }
}

void fn_1_CDDC(CDriverAsAI *self, DrvInfo *info, s32 y, f32 limit)
{
    CPlayer *owner;
    M521Vec dir;
    M521Vec up;
    M521Vec side;
    f32 dist;
    f32 diff;
    f32 clamped;

    owner = self->m_owner;
    PSVECSubtract((Vec *)&owner->m_pos, &fn_1_1650C(owner)->m_pos, &dir);
    dist = (f32)drv_sqrt(PSVECSquareMag(&dir));
    PSVECNormalize(&dir, &dir);
    dir.y = 0.0f;
    up.x = 0.0f;
    up.y = (f32)y;
    up.z = 0.0f;
    PSVECCrossProduct(&dir, &up, &side);
    diff = limit - dist;
    if (diff < -0.1f) {
        clamped = -0.1f;
    } else if (diff > 0.1f) {
        clamped = 0.1f;
    } else {
        clamped = diff;
    }
    diff = clamped;
    PSVECScale(&dir, &dir, diff);
    PSVECAdd(&dir, &side, &side);
    PSVECNormalize(&side, &side);
    info->m_x = side.x;
    info->m_z = side.z;
    info->m_mag = 1.0f;
}

void fn_1_D0F0(CDriverAsAI *self, const DrvPtmf *p)
{
    DrvPtmf pmf;
    DrvPtmf stored;
    DrvTask *task;
    u32 at;

    if (self->m_queue.m_cnt == 0) {
        pmf = *p;
        fn_1_D1F8(self, &pmf);
        return;
    }
    pmf = *p;
    at = self->m_queue.m_off + (self->m_queue.m_cnt - 1);
    task = *fn_1_DB10(&self->m_queue.m_map, at / self->m_queue.m_blk) + (at % self->m_queue.m_blk);
    task->m_pmf = pmf;
    stored = task->m_pmf;
}

void fn_1_D1F8(CDriverAsAI *self, const DrvPtmf *p)
{
    DrvPtmf pmf;
    DrvTask task;

    pmf = *p;
    task.m_obj = self;
    task.m_pmf = pmf;
    fn_1_DFE0(&self->m_queue, &task);
}

void fn_1_D26C(CDriverAsAI *self)
{
    CDriverAsAI *obj = self;
    fn_1_DDEC(&obj->m_queue);
}

void fn_1_D298(CDriverAsAI *self, s32 state, const char *func)
{
    if (self->m_state != state) {
        self->m_state = state;
    }
}

void fn_1_D2AC(void)
{
}

/* The debug draw writes the colour through a GXColor, bytes in a, b, g, r order. */
#define DRV_COLOR(r_, g_, b_, a_)                     do {                                                  GXColor c_;                                       c_.r = (r_);                                      c_.g = (g_);                                      c_.b = (b_);                                      c_.a = (a_);                                      GXColor4u8(c_.a, c_.b, c_.g, c_.r);           } while (0)

/* Debug draw of the CPU controller: target, hit normal and stick direction. */
void fn_1_D2B0(CDriverAsAI *self, Mtx mtx)
{
    CPlayer *owner;
    M521Vec *pos;
    M521Vec v;
    M521Vec w;
    CCtl *ctl;

    GXLoadPosMtxImm(mtx, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_RGBA6, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 1, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetLineWidth(0x1E, GX_TO_ZERO);
    owner = self->m_owner;
    pos = (M521Vec *)&owner->m_pos;

    GXBegin(GX_LINESTRIP, GX_VTXFMT0, 2);
    GXPosition3f32(self->m_tgt.x, self->m_tgt.y, self->m_tgt.z);
    DRV_COLOR(0xFF, 0xFF, 0xFF, 0xFF);
    GXPosition3f32(pos->x, pos->y, pos->z);
    DRV_COLOR(0xFF, 0xFF, 0xFF, 0xFF);

    GXBegin(GX_LINESTRIP, GX_VTXFMT0, 2);
    PSVECScale(&self->m_nrm, &v, 300.0f);
    PSVECAdd(pos, &v, &v);
    GXPosition3f32(v.x, v.y, v.z);
    DRV_COLOR(0xFF, 0x00, 0x00, 0xFF);
    GXPosition3f32(pos->x, pos->y, pos->z);
    DRV_COLOR(0xFF, 0x00, 0x00, 0xFF);

    GXBegin(GX_LINESTRIP, GX_VTXFMT0, 2);
    ctl = owner->m_ctl;
    w.x = ctl->m_in.m_x;
    w.y = 0.0f;
    w.z = ctl->m_in.m_z;
    PSVECScale(&w, &w, 300.0f * ctl->m_in.m_mag);
    PSVECAdd(pos, &w, &w);
    GXPosition3f32(w.x, w.y, w.z);
    DRV_COLOR(0xFF, 0x00, 0xFF, 0xC0);
    GXPosition3f32(pos->x, pos->y, pos->z);
    DRV_COLOR(0xFF, 0x00, 0xFF, 0xC0);
}

CDriverAsAI *fn_1_D9D4(CDriverAsAI *self, s16 freeIt)
{
    if (self != 0) {
        self->m_vtable = lbl_1_data_3BC;
        if (&self->m_queue != 0 && &self->m_queue != 0) {
            fn_1_DB3C(&self->m_queue);
            if (&self->m_queue.m_map != 0 && &self->m_queue.m_map != 0 && self->m_queue.m_map.m_data != 0) {
                __dl__FPv(self->m_queue.m_map.m_data);
            }
        }
        __dt__6CModelFv(&self->m_model, 0);
        if (self != 0) {
            self->m_vtable = lbl_1_data_3DC;
        }
        if (freeIt > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CCtl *fn_1_DAA4(CCtl *self, s16 freeIt)
{
    if (self != 0) {
        self->m_vtable = lbl_1_data_3CC;
        if (self != 0) {
            self->m_vtable = lbl_1_data_3DC;
        }
        if (freeIt > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

DrvTask **fn_1_DB10(CDequeMap *self, u32 idx)
{
    idx += self->m_start;
    if (idx >= self->m_cap) {
        idx -= self->m_cap;
    }
    return &self->m_data[idx];
}

/* ---- helpers of the template: inline iterator operations ---- */

static inline const u32 &drvMinRef(const u32 &a, const u32 &b)
{
    return (b < a) ? b : a;
}

static inline const u32 &drvMin(const u32 &a, const u32 &b)
{
    const u32 &r = drvMinRef(a, b);
    return r;
}

/* number of slots between two map iterators (distance) */
static inline u32 mapDistImpl(CMapIt first, CMapIt last)
{
    u32 n = last.m_idx - first.m_idx;
    return n;
}

static inline u32 mapDist(CMapIt first, CMapIt last)
{
    u32 n = mapDistImpl(first, last);
    return n;
}

/* deque helpers: length limit, capacity in tasks, in-place construction */
static inline u32 dqMaxLen()
{
    u32 m = 0x0FFFFFFF;
    return m;
}

static inline u32 dqTaskCap(const CDeque *d)
{
    u32 blocks = d->m_map.m_size;
    return d->m_blk * (blocks - 1);
}

static inline DrvTask *dqPlace(DrvTask *p)
{
    return p;
}

static inline DrvTask *dqConstruct(DrvTask *p, const DrvTask *src)
{
    DrvTask *q = dqPlace(p);
    if (q != 0) {
        DrvTask *r = q;
        *r = *src;
        q = r;
    }
    return q;
}

/* first / one-past-last slot of the map */
static inline CMapIt mapBegin(CDequeMap *m)
{
    CMapIt it;
    it.m_map = m;
    it.m_idx = 0;
    it.m_slot = m->m_data + m->m_start;
    return it;
}

static inline CMapIt mapEnd(CDequeMap *m)
{
    u32 t = m->m_start + m->m_size;
    if (t < m->m_cap) {
        CMapIt it;
        it.m_map = m;
        it.m_idx = m->m_size;
        it.m_slot = m->m_data + t;
        return it;
    } else {
        CMapIt it;
        it.m_map = m;
        it.m_idx = m->m_size;
        it.m_slot = m->m_data + (t - m->m_cap);
        return it;
    }
}

static inline CMapIt mapAt(CDequeMap *m, u32 idx)
{
    CMapIt it = mapBegin(m);
    u32 cap;
    u32 t;
    it.m_idx += idx;
    cap = it.m_map->m_cap;
    t = it.m_map->m_start + it.m_idx;
    if (t >= cap) {
        t -= cap;
    }
    it.m_slot = it.m_map->m_data + t;
    return it;
}

static inline void mapNext(CMapIt *it)
{
    it->m_idx++;
    it->m_slot++;
    if (it->m_slot == it->m_map->m_data + it->m_map->m_cap) {
        it->m_slot = it->m_map->m_data;
    }
}

static inline void mapPrev(CMapIt *it)
{
    it->m_idx--;
    if (it->m_slot == it->m_map->m_data) {
        it->m_slot = it->m_map->m_data + it->m_map->m_cap;
    }
    it->m_slot--;
}

/* copy [first, last) to dst */
static inline DrvTask **mapCopy(CMapIt first, CMapIt last, DrvTask **dst)
{
    while (first.m_idx != last.m_idx) {
        *dst = *first.m_slot;
        mapNext(&first);
        dst++;
    }
    return dst;
}

/* step of the deque iterator, moving to the next block when the current one ends */
static inline void dqNext(CDequeIt *it)
{
    it->m_pos++;
    it->m_cur++;
    if (it->m_cur == it->m_end) {
        u32 n = it->m_end - *it->m_slot;
        it->m_idx++;
        it->m_slot++;
        if (it->m_slot == it->m_map->m_data + it->m_map->m_cap) {
            it->m_slot = it->m_map->m_data;
        }
        it->m_cur = *it->m_slot;
        it->m_end = it->m_cur + n;
    }
}

/* cdeque::reserve, inlined where the template uses it */
static inline void mapReserve(CDequeMap *self, u32 n)
{
    DrvTask **nd;
    CMapIt it;
    CMapIt end;

    if (n > fn_1_1147C(self)) {
        fn_1_4C9C(DRV_STDERR, "cdeque::reserve length error\n");
        abort();
    }
    if (n > self->m_cap) {
        nd = (DrvTask **)fn_1_4A68(n * sizeof(DrvTask *));
        if (nd == 0) {
            DRV_ABORT_ALLOC();
        }
        if (self->m_data != 0) {
            end = mapEnd(self);
            it = mapBegin(self);
            mapCopy(it, end, nd);
            if (self->m_data != 0) {
                __dl__FPv(self->m_data);
            }
        }
        self->m_cap = n;
        self->m_data = nd;
        self->m_start = 0;
    }
}

static inline DrvTask *dqCopyTasks(CDequeIt first, CDequeIt last, DrvTask *dst)
{
    while (first.m_cur != last.m_cur) {
        dqConstruct(dst, first.m_cur);
        dst++;
        dqNext(&first);
    }
    return dst;
}

/* iterator -> const_iterator conversion */
static inline CMapIt mapToConst(CMapIt it)
{
    CMapIt c;
    c.m_map = it.m_map;
    c.m_idx = it.m_idx;
    c.m_slot = it.m_slot;
    return c;
}

/* slot of the map by raw index, and release of the block it points to */
static inline DrvTask **mapSlot(CDequeMap *m, u32 idx)
{
    return m->m_data + idx;
}

static inline void dqFreeBlock(DrvTask **slot)
{
    DrvTask *blk = *slot;
    __dl__FPv(blk);
    *slot = 0;
}

static inline u32 mapSize(const CDequeMap *m)
{
    return m->m_size;
}

static inline CDequeIt dqEndConst(const CDeque *d)
{
    CDequeIt it;
    fn_1_F5CC(&it, d, d->m_off + d->m_cnt);
    return it;
}

static inline CDequeIt dqBeginConst(const CDeque *d)
{
    CDequeIt it;
    fn_1_F5CC(&it, d, d->m_off);
    return it;
}

CDequeMap *fn_1_E2A8(CDequeMap *self, s16 freeIt)
{
    if (self != 0) {
        if (self != 0 && self->m_data != 0) {
            __dl__FPv(self->m_data);
        }
        if (freeIt > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CDequeMap *fn_1_E31C(CDequeMap *self, s16 freeIt)
{
    if (self != 0) {
        if (self->m_data != 0) {
            __dl__FPv(self->m_data);
        }
        if (freeIt > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

/* ~deque body: destroy the tasks (trivial), free the blocks. */
void fn_1_DB3C(CDeque *self)
{
    CDequeIt last;
    CDequeIt first;
    CMapIt it;
    CMapIt stop;

    if (self->m_map.m_size != 0) {
        fn_1_F724(&last, self, self->m_off + self->m_cnt);
        fn_1_F724(&first, self, self->m_off);
        fn_1_F87C(self, first, last);
        stop = mapEnd(&self->m_map);
        stop.m_idx--;
        if (stop.m_slot == stop.m_map->m_data) {
            stop.m_slot = stop.m_map->m_data + stop.m_map->m_cap;
        }
        stop.m_slot--;
        it = mapBegin(&self->m_map);
        while (it.m_idx != stop.m_idx) {
            __dl__FPv(*it.m_slot);
            mapNext(&it);
        }
    }
}

/* pop_back */
void fn_1_DDEC(CDeque *self)
{
    DrvTask **slot;
    DrvTask *elem;
    u32 at;
    u32 q;
    u32 n;

    at = self->m_off + (self->m_cnt - 1);
    q = at / self->m_blk + self->m_map.m_start;
    if (q >= self->m_map.m_cap) {
        q -= self->m_map.m_cap;
    }
    slot = &self->m_map.m_data[q];
    elem = *slot + (at % self->m_blk);
    self->m_cnt--;
    while (dqTaskCap(self) - (self->m_off + self->m_cnt) >= (self->m_blk * 3) >> 1) {
        self->m_map.m_size--;
        q = self->m_map.m_size - 1 + self->m_map.m_start;
        if (q >= self->m_map.m_cap) {
            q -= self->m_map.m_cap;
        }
        slot = mapSlot(&self->m_map, q);
        dqFreeBlock(slot);
    }
    if (self->m_cnt == 0) {
        n = mapSize(&self->m_map) - 2;
        while (n != 0) {
            self->m_map.m_size--;
            q = self->m_map.m_size - 1 + self->m_map.m_start;
            if (q >= self->m_map.m_cap) {
                q -= self->m_map.m_cap;
            }
            slot = mapSlot(&self->m_map, q);
            dqFreeBlock(slot);
            n--;
        }
        self->m_off = self->m_blk >> 1;
    }
}

/* push_back */
void fn_1_DFE0(CDeque *self, const DrvTask *item)
{
    u32 added;
    CDequeMap spare;
    u8 grew;
    DrvTask *p;
    u32 at;
    u32 q;

    if (self->m_cnt > dqMaxLen() - 1) {
        fn_1_4C9C(DRV_STDERR, "deque::push_back length error\n");
        abort();
    }
    if (self->m_map.m_size == 0) {
        fn_1_EA80(&self->m_map, 2);
        fn_1_ECA4(&self->m_map, 1, 0);
    }
    added = 0;
    spare.m_cap = 0;
    spare.m_size = 0;
    spare.m_start = 0;
    spare.m_data = 0;
    grew = 0;
    if (self->m_off + self->m_cnt == dqTaskCap(self)) {
        grew = fn_1_EF30(self, 0, &spare, &added, 1);
    }
    at = self->m_off + self->m_cnt;
    if (at < self->m_blk) {
        DrvTask **slot = self->m_map.m_data + self->m_map.m_start;
        p = dqConstruct(*slot + at, item);
    } else {
        DrvTask **slot;
        q = at / self->m_blk + self->m_map.m_start;
        if (q >= self->m_map.m_cap) {
            q -= self->m_map.m_cap;
        }
        slot = self->m_map.m_data + q;
        p = dqConstruct(*slot + (at % self->m_blk), item);
    }
    self->m_cnt++;
    if (spare.m_data != 0) {
        __dl__FPv(spare.m_data);
    }
    while (self->m_off >= (self->m_blk * 3) >> 1) {
        DrvTask **first = self->m_map.m_data + self->m_map.m_start;
        __dl__FPv(*first);
        fn_1_E748(&self->m_map);
        self->m_off -= self->m_blk;
    }
}

/* deque copy */
CDeque *fn_1_E388(CDeque *self, const CDeque *src)
{
    DrvTask *blk;
    DrvTask *nul;
    DrvTask *dst;
    CDequeIt it;
    CDequeIt end;

    CDequeMap *map;

    self->m_blk = lbl_1_rodata_238;
    map = &self->m_map;
    map->m_cap = 0;
    map->m_size = 0;
    map->m_start = 0;
    map->m_data = 0;
    self->m_off = 0;
    self->m_cnt = src->m_cnt;
    if (self->m_cnt != 0) {
        fn_1_EA80(&self->m_map, 2);
        fn_1_ECA4(&self->m_map, 1, 0);
        if (self->m_blk < self->m_cnt) {
            self->m_blk = self->m_cnt;
        }
        blk = (DrvTask *)fn_1_4A68(self->m_blk * sizeof(DrvTask));
        if (blk == 0) {
            DRV_ABORT_ALLOC();
        }
        *(self->m_map.m_data + self->m_map.m_start) = blk;
        nul = 0;
        fn_1_E784(&self->m_map, &nul);
        self->m_off = (self->m_blk - self->m_cnt) >> 1;
        dst = blk + self->m_off;
        end = dqEndConst(src);
        it = dqBeginConst(src);
        dqCopyTasks(it, end, dst);
    }
    return self;
}

/* default deque */
void fn_1_E6F0(CDeque *self, const void *unused)
{
    CDequeMap *map;

    self->m_blk = lbl_1_rodata_238;
    map = &self->m_map;
    map->m_cap = 0;
    map->m_size = 0;
    map->m_start = 0;
    map->m_data = 0;
    self->m_off = 0;
    self->m_cnt = 0;
}

/* cdeque::pop_front */
void fn_1_E748(CDequeMap *self)
{
    self->m_size--;
    self->m_start++;
    if (self->m_start >= self->m_cap) {
        self->m_start -= self->m_cap;
    }
}

/* cdeque::push_back */
void fn_1_E784(CDequeMap *self, DrvTask *const *val)
{
    const u32 *keep;
    u32 newCap;
    u32 t;
    u32 cap;
    DrvTask **nd;
    CMapIt it;
    CMapIt end;

    if (self->m_size == fn_1_1147C(self)) {
        fn_1_4C9C(DRV_STDERR, "cdeque::push_back length error\n");
        abort();
    }
    cap = self->m_cap;
    if (self->m_size < cap) {
        t = self->m_size + self->m_start;
        if (t >= self->m_cap) {
            t -= self->m_cap;
        }
        self->m_data[t] = *val;
        self->m_size++;
        return;
    }
    if (self->m_cap < lbl_1_data_428) {
        keep = &lbl_1_data_428;
    } else {
        keep = &self->m_cap;
    }
    newCap = *keep;
    if (self->m_size + 1 > newCap) {
        newCap *= 2;
    }
    nd = (DrvTask **)fn_1_4A68(newCap * sizeof(DrvTask *));
    if (nd == 0) {
        DRV_ABORT_ALLOC();
    }
    nd[self->m_size] = *val;
    if (self->m_size != 0) {
        end = mapEnd(self);
        it = mapBegin(self);
        mapCopy(it, end, nd);
    }
    if (self->m_data != 0) {
        __dl__FPv(self->m_data);
    }
    self->m_data = nd;
    self->m_size++;
    self->m_cap = newCap;
    self->m_start = 0;
}

/* cdeque::reserve */
void fn_1_EA80(CDequeMap *self, u32 n)
{
    mapReserve(self, n);
}

/* cdeque::resize */
void fn_1_ECA4(CDequeMap *self, u32 n, DrvTask *val)
{
    DrvTask *v;
    CMapIt end;
    CMapIt res;

    v = val;
    if (n > self->m_size) {
        end = mapEnd(self);
        fn_1_104A8(self, end, n - self->m_size, &v);
        return;
    }
    if (n < self->m_size) {
        fn_1_FFB4(&res, self, mapAt(self, n), mapEnd(self));
    }
}

/* deque: append one block of tasks at the back of the map */
u8 fn_1_EF30(CDeque *self, s32 front, CDequeMap *spare, u32 *count, s32 n)
{
    DrvTask *blk;
    DrvTask *nul;
    u32 cap2;
    u32 t;
    u32 left;
    u8 grew;
    u8 tag;

    grew = 0;
    left = n - front;
    do {
        blk = (DrvTask *)fn_1_4A68(self->m_blk * sizeof(DrvTask));
        if (blk == 0) {
            DRV_ABORT_ALLOC();
        }
        if (grew == 0) {
            if (self->m_map.m_size == self->m_map.m_cap) {
                fn_1_FF40(&self->m_map, spare);
                grew = 1;
                cap2 = spare->m_cap * 2;
                mapReserve(&self->m_map, cap2);
                tag = lbl_1_bss_A10;
                fn_1_FBA8(&self->m_map, mapToConst(mapBegin(spare)), mapToConst(mapEnd(spare)), &tag);
            }
        }
        t = self->m_map.m_size - 1 + self->m_map.m_start;
        if (t >= self->m_map.m_cap) {
            t -= self->m_map.m_cap;
        }
        self->m_map.m_data[t] = blk;
        nul = 0;
        fn_1_F59C(&self->m_map, &nul);
        *count = *count + 1;
        if (left > self->m_blk) {
            left -= self->m_blk;
            continue;
        }
        break;
    } while (1);
    return grew;
}

void fn_1_F59C(CDequeMap *self, DrvTask *const *val)
{
    CDequeMap *m = self;
    DrvTask *const *v = val;
    fn_1_E784(m, v);
}

/* deque iterator at index idx (const / mutable) */
void fn_1_F5CC(CDequeIt *out, const CDeque *self, u32 idx)
{
    CMapIt it;
    DrvTask *base;
    DrvTask *cur;
    u32 t;

    it = mapBegin((CDequeMap *)&self->m_map);
    if (idx < self->m_blk) {
        if (self->m_map.m_size != 0) {
            base = *it.m_slot;
        } else {
            base = 0;
        }
        cur = base + idx;
    } else {
        it.m_idx += idx / self->m_blk;
        t = it.m_map->m_start + it.m_idx;
        if (t >= it.m_map->m_cap) {
            t -= it.m_map->m_cap;
        }
        it.m_slot = it.m_map->m_data + t;
        base = *it.m_slot;
        cur = base + (idx % self->m_blk);
    }
    out->m_map = it.m_map;
    out->m_idx = it.m_idx;
    out->m_slot = it.m_slot;
    out->m_cur = cur;
    out->m_end = base + self->m_blk;
    out->m_pos = idx;
}

void fn_1_F724(CDequeIt *out, CDeque *self, u32 idx)
{
    CMapIt it;
    DrvTask *base;
    DrvTask *cur;
    u32 t;

    it = mapBegin(&self->m_map);
    if (idx < self->m_blk) {
        if (self->m_map.m_size != 0) {
            base = *it.m_slot;
        } else {
            base = 0;
        }
        cur = base + idx;
    } else {
        it.m_idx += idx / self->m_blk;
        t = it.m_map->m_start + it.m_idx;
        if (t >= it.m_map->m_cap) {
            t -= it.m_map->m_cap;
        }
        it.m_slot = it.m_map->m_data + t;
        base = *it.m_slot;
        cur = base + (idx % self->m_blk);
    }
    out->m_map = it.m_map;
    out->m_idx = it.m_idx;
    out->m_slot = it.m_slot;
    out->m_cur = cur;
    out->m_end = base + self->m_blk;
    out->m_pos = idx;
}

/* destroy the tasks of [first, last): they are trivial, only the walk remains */
void fn_1_F87C(CDeque *self, CDequeIt first, CDequeIt last)
{
    u32 n;
    u32 q;
    u32 r;
    u32 e;
    u32 take;
    u32 left;
    CMapIt it;
    DrvTask *base;
    DrvTask *p;

    if (first.m_cur != last.m_cur) {
        n = last.m_pos - first.m_pos;
        q = first.m_pos / self->m_blk;
        r = first.m_pos % self->m_blk;
        e = r + n;
        take = drvMin(e, self->m_blk);
        it = mapAt(&self->m_map, q);
        base = *it.m_slot;
        for (p = base + r; p < base + take; p++) {
        }
        left = n - (take - r);
        if (left != 0) {
            mapNext(&it);
            while (left >= self->m_blk) {
                base = *it.m_slot;
                for (p = base; p < base + self->m_blk; p++) {
                }
                mapNext(&it);
                left -= self->m_blk;
            }
            if (left != 0) {
                base = *it.m_slot;
                for (p = base; p < base + left; p++) {
                }
            }
        }
    }
}

/* cdeque::assign(first, last) */
void fn_1_FBA8(CDequeMap *self, CMapIt first, CMapIt last, const u8 *tag)
{
    u32 n;
    u32 lim;
    u32 a;
    u32 b;
    DrvTask **old;
    DrvTask **dst;

    n = mapDist(first, last);
    a = 0x3FFFFFFF;
    b = 0x7FFFFFFF;
    lim = drvMin(a, b);
    if (n > lim) {
        fn_1_4C9C(DRV_STDERR, "cdeque::assign length error\n");
        abort();
    }
    if (n <= self->m_cap) {
        mapCopy(first, last, self->m_data);
        self->m_start = 0;
        self->m_size = n;
        return;
    }
    old = self->m_data;
    self->m_data = (DrvTask **)fn_1_4A68(n * sizeof(DrvTask *));
    if (self->m_data == 0) {
        DRV_ABORT_ALLOC();
    }
    dst = self->m_data;
    mapCopy(first, last, dst);
    if (old != 0) {
        __dl__FPv(old);
    }
    self->m_cap = n;
    self->m_size = n;
    self->m_start = 0;
}

/* cdeque::swap */
void fn_1_FF40(CDequeMap *self, CDequeMap *other)
{
    u32 t0;
    u32 t1;
    u32 t2;
    DrvTask **t3;

    if (self != other) {
        t0 = self->m_cap;
        self->m_cap = other->m_cap;
        other->m_cap = t0;
        t1 = self->m_size;
        self->m_size = other->m_size;
        other->m_size = t1;
        t2 = self->m_start;
        self->m_start = other->m_start;
        other->m_start = t2;
        t3 = self->m_data;
        self->m_data = other->m_data;
        other->m_data = t3;
    }
}

/* cdeque::erase(first, last) */
void fn_1_FFB4(CMapIt *out, CDequeMap *self, CMapIt first, CMapIt last)
{
    CMapIt b;
    CMapIt e;
    CMapIt d;
    CMapIt s;
    CMapIt stop;
    u32 before;
    u32 after;
    u32 n;

    if (first.m_idx == last.m_idx) {
        out->m_map = first.m_map;
        out->m_idx = first.m_idx;
        out->m_slot = first.m_slot;
        return;
    }
    b = mapBegin(self);
    before = mapDist(b, first);
    e = mapEnd(self);
    after = mapDist(last, e);
    n = mapDist(first, last);
    if (before < after) {
        if (before != 0) {
            d = last;
            s = first;
            stop = mapBegin(self);
            while (s.m_idx != stop.m_idx) {
                mapPrev(&d);
                mapPrev(&s);
                *d.m_slot = *s.m_slot;
            }
        }
        self->m_start += n;
        if (self->m_start >= self->m_cap) {
            self->m_start -= self->m_cap;
        }
    } else if (after != 0) {
        d = first;
        e = mapEnd(self);
        s = last;
        while (s.m_idx != e.m_idx) {
            *d.m_slot = *s.m_slot;
            mapNext(&s);
            mapNext(&d);
        }
    }
    self->m_size -= n;
    out->m_map = first.m_map;
    out->m_idx = first.m_idx;
    out->m_slot = first.m_slot;
}

/* fill n slots from it with *val */
static inline void mapFill(CMapIt it, u32 n, DrvTask *const *val)
{
    while (n != 0) {
        *it.m_slot = *val;
        mapNext(&it);
        n--;
    }
}

/* cdeque::insert(pos, n, val) */
void fn_1_104A8(CDequeMap *self, CMapIt pos, u32 n, DrvTask *const *val)
{
    u32 a;
    u32 b;
    u32 lim;
    u32 before;
    u32 after;
    u32 newSize;
    u32 newCap;
    const u32 *keep;
    CMapIt b0;
    CMapIt e0;
    CMapIt d;
    CMapIt s;
    CMapIt se;
    CMapIt src;
    DrvTask **nd;
    DrvTask **p;

    if (n == 0) {
        return;
    }
    a = 0x3FFFFFFF;
    b = 0x7FFFFFFF;
    lim = drvMin(a, b);
    if (n > lim || self->m_size > lim - n) {
        fn_1_4C9C(DRV_STDERR, "cdeque::insert length error\n");
        abort();
    }
    if (self->m_size + n <= self->m_cap) {
        b0 = mapBegin(self);
        before = mapDist(b0, pos);
        e0 = mapEnd(self);
        after = mapDist(pos, e0);
        if (before < after) {
            if (self->m_start >= n) {
                self->m_start -= n;
            } else {
                self->m_start = self->m_cap - (n - self->m_start);
            }
            if (before == 0) {
                mapFill(mapBegin(self), n, val);
            } else {
                /* move the first `before` tasks n slots down, then fill the gap */
                d = mapBegin(self);
                s = mapAt(self, n);
                se = mapAt(self, n + before);
                while (s.m_idx != se.m_idx) {
                    *d.m_slot = *s.m_slot;
                    mapNext(&s);
                    mapNext(&d);
                }
                mapFill(mapAt(self, before), n, val);
            }
        } else if (after == 0) {
            mapFill(mapEnd(self), n, val);
        } else {
            /* move the tail n slots up, then fill the gap */
            d = mapAt(self, self->m_size + n);
            s = mapEnd(self);
            while (s.m_idx != pos.m_idx) {
                mapPrev(&d);
                mapPrev(&s);
                *d.m_slot = *s.m_slot;
            }
            mapFill(pos, n, val);
        }
        self->m_size += n;
        return;
    }
    newSize = self->m_size + n;
    if (self->m_cap < lbl_1_data_46C) {
        keep = &lbl_1_data_46C;
    } else {
        keep = &self->m_cap;
    }
    newCap = *keep;
    while (newSize > newCap) {
        newCap *= 2;
    }
    b0 = mapBegin(self);
    before = mapDist(b0, pos);
    nd = (DrvTask **)fn_1_4A68(newCap * sizeof(DrvTask *));
    if (nd == 0) {
        DRV_ABORT_ALLOC();
    }
    if (before != 0) {
        mapCopy(mapBegin(self), pos, nd);
    }
    p = nd + before;
    for (a = n; a != 0; a--) {
        *p = *val;
        p++;
    }
    if (before < self->m_size) {
        e0 = mapEnd(self);
        mapCopy(pos, e0, nd + (before + n));
    }
    if (self->m_data != 0) {
        __dl__FPv(self->m_data);
    }
    self->m_data = nd;
    self->m_size = newSize;
    self->m_cap = newCap;
    self->m_start = 0;
}

/* cdeque::max_size */
u32 fn_1_1147C(CDequeMap *self)
{
    u32 a = 0x3FFFFFFF;
    u32 b = 0x7FFFFFFF;

    return drvMin(a, b);
}

void fn_1_114DC()
{
    lbl_1_bss_A00 = 0x78;
    lbl_1_bss_A04 = 0xD2;
    lbl_1_bss_A08 = 0xD2;
    lbl_1_bss_A0C = 0x96;
}

DRV_RODATA const u32 lbl_1_rodata_238 = 0x20;
}
