#ifndef M521DLL_TYPES_H
#define M521DLL_TYPES_H

/* Shared class layouts of m521Dll, used by the units finished in the PC pass
 * (gamemaster, player, score, clock, driver, m521). Pointer members are real
 * pointers so the structs scale on 64-bit hosts; offsets in comments are the
 * GameCube ones. */

#include "dolphin/types.h"
#include "dolphin/mtx.h"
#include "dolphin/os.h"

typedef s16 HU3DMODELID;

/* Pointer-to-member-function slot. The GameCube build keeps the MWCC layout
 * (this-delta, vtable index, function address); on PC the slot is just the
 * function pointer, called directly with the object. */
#ifdef TARGET_PC
typedef struct M521Ptmf_s {
    void (*fn)(void *self);
} M521Ptmf;
#define M521_PTMF_INIT(f) { (void (*)(void *))(f) }
#define M521_PTMF_NULL_INIT { 0 }
#define M521PtmfTest(p) ((p)->fn != 0)
#define M521PtmfCall(obj, p) ((p)->fn(obj))
#else
typedef struct M521Ptmf_s {
    s32 delta;
    s32 vidx;
    void *fn;
} M521Ptmf;
#define M521_PTMF_INIT(f) { 0, -1, (void *)(f) }
#define M521_PTMF_NULL_INIT { 0, -1, 0 }
extern "C" s32 __ptmf_test(const M521Ptmf *p);
extern "C" void __ptmf_scall(void *self);
#define M521PtmfTest(p) __ptmf_test(p)
#define M521PtmfCall(obj, p) __ptmf_scall(obj)
#endif

typedef Vec M521Vec;

/* Intrusive list node: every game object starts with one. */
struct CListNode {
    CListNode *m_next; /* 0x00 */
    CListNode *m_prev; /* 0x04 */
};

struct CModel {
    HU3DMODELID m_id;
    MtxPtr getMatrix();
};

/* Base of the game objects: list node, vtable (slot 2 = destructor, slot 3 = flag handler, slot 4 = update). */
struct CObject {
    CListNode m_node;  /* 0x00 */
    void **m_vtable;   /* 0x08 */
    s32 m_flags;       /* 0x0C */
};

/* Game master base class (singleton lbl_1_bss_C). */
struct CAppBase {
    void **m_vtbl0;    /* 0x00 */
    void **m_vtbl;     /* 0x04 */
    s32 m_player[4];   /* 0x08 player ids, -1 = free */
    s32 m_mes;         /* 0x18 current game message */
    s32 m_wait;        /* 0x1C frames to wait */
};

/* Sphere used for the player collision: position plus radius. */
typedef struct M521Sphere_s {
    f32 x;
    f32 y;
    f32 z;
    f32 r;
} M521Sphere;

/* Input sample a player controller fills every frame. */
struct CCtlInput {
    f32 m_scale; /* 0x00 difficulty speed factor */
    f32 m_x;     /* 0x04 stick x */
    f32 m_z;     /* 0x08 stick z */
    f32 m_mag;   /* 0x0C stick magnitude */
    u8 m_btn;    /* 0x10 button pressed */
};

/* Player controller (pad or CPU): virtual destructor in slot 2 of its vtable. */
struct CCtl {
    void **m_vtable; /* 0x00 */
    CCtlInput m_in;  /* 0x04 */
};

/* Clock hand model (class m521::CClockHand, size 0x68 on GC). */
struct CClockHand {
    CModel m_model;
    Mtx m_baseMtx;
    Mtx m_mtx;
    f32 m_angle;
    MtxPtr getMtx() { return m_mtx; }
};

/* The clock: two hands (hour = 0, minute = 1) plus their guide models. */
struct CClock {
    s32 m_kind;           /* 0x00 */
    M521Vec m_pos;        /* 0x04 */
    f32 m_time;           /* 0x10 */
    u8 m_stopped[2];      /* 0x14 */
    s32 m_cnt[2];         /* 0x18 */
    s32 m_phase;          /* 0x20 */
    f32 m_speed;          /* 0x24 */
    f32 m_ang[2];         /* 0x28 */
    f32 m_total[2];       /* 0x30 */
    f32 m_target[2];      /* 0x38 */
    f32 m_goal[2];        /* 0x40 */
    s32 m_frame;          /* 0x48 */
    s32 m_sfx[2];         /* 0x4C */
    s32 m_timer[2];       /* 0x54 */
    s32 m_last[2];        /* 0x5C */
    CClockHand *m_hand[2];/* 0x64 */
    CModel *m_model[2];   /* 0x6C */
};

/* Growable array of u16 motion ids (cdeque). */
struct CDeque16 {
    u32 m_cap;     /* 0x00 */
    u32 m_size;    /* 0x04 */
    u16 *m_data;   /* 0x08 */
};

struct CPlayer {
    CListNode m_node;    /* 0x00 */
    CCtl *m_ctl;         /* 0x08 controller object (owned, virtual dtor) */
    M521Ptmf m_hook;     /* 0x0C */
    s32 m_padNo;         /* 0x18 */
    s32 m_slot;          /* 0x1C */
    s32 m_charNo;        /* 0x20 */
    s32 m_motion;        /* 0x24 */
    M521Sphere m_pos;    /* 0x28 */
    s32 m_i38;           /* 0x38 */
    s32 m_timer;         /* 0x3C */
    s32 m_i40;           /* 0x40 */
    s32 m_i44;           /* 0x44 */
    s32 m_i48;           /* 0x48 */
    CPlayer *m_partner;  /* 0x4C */
    u8 m_u50;            /* 0x50 */
    u8 m_flag51;         /* 0x51 */
    s32 m_req;           /* 0x54 */
    s32 m_i58;           /* 0x58 */
    s32 m_i5C;           /* 0x5C */
    f32 m_f60;           /* 0x60 */
    f32 m_f64;           /* 0x64 */
    M521Vec m_vel;       /* 0x68 */
    M521Vec m_acc;       /* 0x74 */
    s32 m_state;         /* 0x80 */
    CDeque16 m_mot;      /* 0x84 */
    CModel *m_model;     /* 0x90 */
};

struct CStage {
    CListNode m_node;
    void **m_vtable;
    s32 m_bitFlags;
    void *m_unk10;
    void *m_unk14;
    void *m_unk18;
    CModel *m_modelA;
    CModel *m_modelB;
    M521Ptmf m_hook;
};

struct CCamera {
    CListNode m_node;
    void **m_vtable;
    s32 m_bitFlags;
    u8 m_unk10[0x34 - 0x10];
    f32 m_pathFrom[7];
    f32 m_pathTo[7];
    s32 m_timer;
    M521Ptmf m_hook;
    s16 m_camModelId;
};

struct ScoreEntry {
    s16 m_grp;     /* 0x00 */
    s32 m_count;   /* 0x04 */
    s32 m_fade[3]; /* 0x08 */
    f32 m_x;       /* 0x14 */
    f32 m_y;       /* 0x18 */
};

struct CScore {
    CListNode m_node;      /* 0x00 */
    void **m_vtable;       /* 0x08 */
    s32 m_bitFlags;        /* 0x0C */
    u8 m_active;           /* 0x10 */
    ScoreEntry m_entry[2]; /* 0x14 */
};

/* OS thread with its heap stack, and the thread whose body is virtual slot 3 of the
 * object (the game master embeds one at 0x20). */
typedef struct OSSemaphore {
    OSThreadQueue queue;
    s32 count;
} OSSemaphore;

typedef struct CThread {
    OSThread m_th;
    u32 *m_stack;
} CThread;

typedef struct CSubThread {
    void **m_vtbl;
    CThread m_thread;
    OSSemaphore m_sem;
    s32 m_arg;
} CSubThread;

/* ---- controllers (driver unit): pad controller, CPU controller and its task queue ---- */

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

#endif
