#ifndef M521DLL_TYPES_H
#define M521DLL_TYPES_H

/* Shared class layouts of m521Dll, used by the units finished in the PC pass
 * (gamemaster, player, score, clock, driver, m521). Pointer members are real
 * pointers so the structs scale on 64-bit hosts; offsets in comments are the
 * GameCube ones. */

#include "dolphin/types.h"

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

typedef struct M521Vec_s {
    f32 x;
    f32 y;
    f32 z;
} M521Vec;

/* Intrusive list node: every game object starts with one. */
struct CListNode {
    CListNode *m_prev;
    CListNode *m_next;
};

struct CModel {
    HU3DMODELID m_id;
};

/* Object with a virtual destructor in slot 2 of its vtable. */
struct CVirtual {
    void **m_vtable;
};

/* Growable array of u16 motion ids (cdeque). */
struct CDeque16 {
    s32 m_unk0;    /* 0x00 */
    u32 m_size;    /* 0x04 */
    u16 *m_data;   /* 0x08 */
};

struct CPlayer {
    CListNode m_node;    /* 0x00 */
    CVirtual *m_ctl;     /* 0x08 controller object (virtual dtor in slot 2) */
    M521Ptmf m_hook;     /* 0x0C */
    s32 m_padNo;         /* 0x18 */
    s32 m_slot;          /* 0x1C */
    s32 m_charNo;        /* 0x20 */
    s32 m_motion;        /* 0x24 */
    M521Vec m_pos;       /* 0x28 */
    f32 m_f34;           /* 0x34 */
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

#endif
