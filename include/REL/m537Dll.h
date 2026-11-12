#ifndef _M537DLL_H
#define _M537DLL_H

#include "dolphin.h"
#include "humath.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "game/hu3d.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "game/data.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "game/memory.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "game/audio.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "game/object.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "game/pad.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "game/process.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "game/gamework.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "game/hsfex.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "game/charman.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "game/sprite.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "game/mg/timer.h"
#ifdef __cplusplus
}
#endif
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __MWERKS__
#define M537_RODATA __declspec(section ".rodata")
#else
#define M537_RODATA
#endif

/* Intrusive list node every m537 work object starts with (see fn_1_1264/1330). */
typedef struct M537Node_s {
    struct M537Node_s *next;
    struct M537Node_s *prev;
} M537_NODE;

/* MWCC pointer-to-member-function triple. */
typedef struct M537PTMF_s {
    s32 thisDelta;
    s32 vOffset;
    void *fn;
} M537_PTMF;

s32 __ptmf_test(M537_PTMF *ptmf);
void __ptmf_scall(void *self);

void fn_1_1D4(const char *cond, const char *file, s32 line);
void fn_1_C8(void *ptr);
void fn_1_1264(void *node);
void *fn_1_1270(void *node, s32 flag);
void fn_1_1330(void *node, void *head);
void fn_1_C08(Vec *pos, s32 mode);
void *fn_1_0(u32 size);
s16 *fn_1_2C94(s16 *out, int dataNum);
s16 *fn_1_2DB4(s16 *out, s16 *modelId);
s16 *fn_1_30F4(s16 *modelId, s16 freeIt);
/* Effect object shared by killereff.cpp and killer.cpp (0x10 bytes). */
typedef struct M537EffObj_s {
    M537_NODE node;
    s16 mdl;
    s32 unkC;
} M537_EFFOBJ;

f32 fn_1_4D1C(void);
void fn_1_12DC(void *node);
M537_EFFOBJ *fn_1_7B3C(M537_EFFOBJ *self, Vec *pos, f32 rotY);
M537_EFFOBJ *fn_1_8098(M537_EFFOBJ *self, Vec *pos);
s16 *fn_1_2D18(s16 *out, s16 charNo, s16 flag);
void fn_1_2EF4(s16 *modelId, Vec *pos);
void fn_1_2F3C(s16 *modelId, Vec *rot);
int fn_1_234(void *file, const char *fmt, ...);
s32 fn_1_4AE0(void);
s32 fn_1_1F30(void);
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT,
                  GXTexFilter filter);
void fn_1_2E08(s16 *modelId, u32 attr);
void fn_1_2E40(s16 *modelId, u32 attr);
void fn_1_304C(s16 *modelId, s16 layerNo);
void fn_1_31E4(s16 *modelId);
void fn_1_3214(s16 *modelId);
void *__register_global_object(void *object, void *destructor, void *regmem);

extern M537_NODE lbl_1_bss_C;

/* Circular value range used by the dorrie rotation (fn_1_6C20 and friends). */
typedef struct M537Cycle_s {
    f64 lo;     /* 0x00 */
    f64 hi;     /* 0x08 */
    f64 range;  /* 0x10 */
    f64 value;  /* 0x18 */
} M537_CYCLE;

/* std::vector<s16>: capacity, size, data (fn_1_6D0C inserts). */
typedef struct M537VecS16_s {
    u32 cap;
    u32 size;
    s16 *data;
} M537_VECS16;

/* One dorrie: model id (CModel at 0), rotation cycle, tween state, motion list. */
typedef struct M537Dorrie {
    s16 mdl;            /* 0x00 */
    M537_CYCLE cyc;     /* 0x08 */
    f64 from;           /* 0x28 */
    f64 to;             /* 0x30 */
    f64 t;              /* 0x38 */
    f64 dur;            /* 0x40 */
    u8 turn;            /* 0x48 */
    s32 count;          /* 0x4C */
    M537_VECS16 mot;    /* 0x50 */
    s32 timer;          /* 0x5C */
    s16 *shadow;        /* 0x60 */
} M537Dorrie;

/* Lens flare object: list node, vtable, flags, then the four flare sprites. */
typedef struct M537Effect {
    M537_NODE node;     /* 0x00 */
    void **vptr;        /* 0x08 */
    s32 flag;           /* 0x0C */
    ANIMDATA *anim[4];  /* 0x10 */
} M537Effect;

/* Holder of a dorrie owned by the stage (4 bytes on GameCube). */
typedef struct M537DorriePtr {
    M537Dorrie *p;
} M537DorriePtr;

/* Holder of a model owned by the stage. */
typedef struct M537ModelPtr {
    s16 *p;
} M537ModelPtr;

/* Water splash object (0x14 on GameCube). */
typedef struct M537Splash {
    M537_NODE node;     /* 0x00 */
    void **vptr;        /* 0x08 */
    s32 flag;           /* 0x0C */
    s16 mdl;            /* 0x10 */
} M537Splash;

/* Ripple particle object (0x1C on GameCube). */
typedef struct M537Ripple {
    M537_NODE node;         /* 0x00 */
    void **vptr;            /* 0x08 */
    s32 flag;               /* 0x0C */
    s16 mdl;                /* 0x10 */
    ANIMDATA *anim;         /* 0x14 */
    HU3DPARTICLE *particle; /* 0x18 */
} M537Ripple;

// The module main object (singleton at lbl_1_bss_9EC).
struct M537Main {
    s32 m_unk0;         /* 0x00 */
    void **m_vptr;      /* 0x04 */
    s32 m_player[4];    /* 0x08 */
    s32 m_msg;          /* 0x18 */
    s32 m_timer;        /* 0x1C */
};

// Look-at camera: position, up vector and target.
struct M537CamLook {
    Vec m_pos;      /* 0x00 */
    Vec m_up;       /* 0x0C */
    Vec m_target;   /* 0x18 */
};

// Camera as position plus orientation quaternion.
struct M537CamQuat {
    Vec m_pos;          /* 0x00 */
    Quaternion m_rot;   /* 0x0C */
};

struct M537Semaphore {
    s32 m_count;
    OSThreadQueue m_queue;
};

// OS thread plus the heap block holding its stack.
struct M537Thread {
    OSThread m_os;      /* 0x000 */
    u32 *m_stack;       /* 0x318 */
};

// Thread object: vtable (slot 3 is the thread body), the thread and a semaphore
// the creator waits on.
struct M537CThread {
    void **m_vptr;          /* 0x000 */
    M537Thread m_th;        /* 0x008 */
    M537Semaphore m_sem;    /* 0x328 */
    s32 m_value;            /* 0x334 */
};

// The camera work object (singleton at lbl_1_bss_A10).
struct M537Camera {
    M537_NODE m_node;       /* 0x00 */
    void **m_vptr;          /* 0x08 */
    s32 m_flag;             /* 0x0C */
    M537CamLook m_look;     /* 0x10 */
    M537_PTMF m_hook;       /* 0x34 */
    s16 m_camMdl;           /* 0x40 */
    f64 m_duration;         /* 0x48 */
    s32 m_timer;            /* 0x50 */
    M537CamQuat m_from;     /* 0x54 */
    M537CamQuat m_to;       /* 0x70 */
    u8 m_debug;             /* 0x8C */
    u8 m_debugLock;         /* 0x8D */
    M537CamLook m_dbg;      /* 0x90 */
};

// The stage work object (singleton at lbl_1_bss_A14).
struct M537Stage {
    M537_NODE m_node;           /* 0x00 */
    void **m_vptr;              /* 0x08 */
    s32 m_flag;                 /* 0x0C */
    s16 m_mdl;                  /* 0x10 */
    s16 *m_sky;                 /* 0x14 */
    M537DorriePtr m_dorrie[4];  /* 0x18 */
    M537ModelPtr m_part[3];     /* 0x28 */
};

// One rope point: position, previous position and velocity.
struct M537Pt {
    Vec m_pos;      /* 0x00 */
    Vec m_prev;     /* 0x0C */
    Vec m_vel;      /* 0x18 */
};

// std::vector<M537Pt>: capacity, size, data.
struct M537VecPt {
    u32 m_cap;
    u32 m_size;
    M537Pt *m_data;
};

// std::vector<Vec>.
struct M537VecV3 {
    u32 m_cap;
    u32 m_size;
    Vec *m_data;
};

// One rope: list node, drawing hook model, point count, segment length and points.
struct M537Arm {
    M537_NODE m_node;       /* 0x00 */
    s16 m_hook;             /* 0x08 */
    u32 m_count;            /* 0x0C */
    f32 m_len;              /* 0x10 */
    M537VecPt m_pts;        /* 0x14 */
    Vec m_dir;              /* 0x20 */
    u8 m_flag;              /* 0x2C */
    ANIMDATA *m_anim;       /* 0x30 */
};

// Holder of a rope owned by the centre object.
struct M537ArmPtr {
    M537Arm *m_p;
};

// The centre object (singleton at lbl_1_bss_A0C): model, target and smoothed position.
struct M537Center {
    s16 m_mdl;              /* 0x00 */
    u8 m_flag;              /* 0x02 */
    Vec m_target;           /* 0x04 */
    Vec m_pos;              /* 0x10 */
    Vec m_vel;              /* 0x1C */
    M537ArmPtr m_arm[4];    /* 0x28 */
};

// The minigame master object: core, thread (second base at 0x20), timer and frame counter.
struct M537GameMaster {
    M537Main m_core;        /* 0x00 */
    M537CThread m_thread;   /* 0x20 */
    MGTIMER *m_timer;       /* 0x358 */
    s32 m_counter;          /* 0x35C */
};

/* Particle spray emitter (0x18 on GameCube). */
typedef struct M537Spray {
    M537_NODE node;     /* 0x00 */
    void **vptr;        /* 0x08 */
    s32 flag;           /* 0x0C */
    s16 mdl;            /* 0x10 */
    ANIMDATA *anim;     /* 0x14 */
} M537Spray;

/* Water wake quad (0x28 on GameCube). */
typedef struct M537Wake {
    M537_NODE node;     /* 0x00 */
    void **vptr;        /* 0x08 */
    s32 flag;           /* 0x0C */
    s16 mdl;            /* 0x10 */
    ANIMDATA *anim;     /* 0x14 */
    Vec pos;            /* 0x18 */
    f32 t;              /* 0x24 */
} M537Wake;

/* The water surface (singleton at lbl_1_bss_9F0): 32x32 spring grid, screen copy and indirect map. */
struct M537Water {
    M537_NODE node;             /* 0x00 */
    void **vptr;                /* 0x08 */
    s32 flag;                   /* 0x0C */
    GXTexObj tex0;              /* 0x10 */
    GXTexObj tex1;              /* 0x30 */
    u8 pad50[0x10];             /* 0x50 */
#ifdef TARGET_PC
    alignas(32)
#endif
    u8 img[0x96000];            /* 0x60 */
    ANIMDATA *anim[3];          /* 0x96060 */
    Vec texOfs;                 /* 0x9606C */
    HSFOBJECT *obj[3];          /* 0x96078 */
    s16 id;                     /* 0x96084 */
    s16 mdl;                    /* 0x96086 */
    f32 time;                   /* 0x96088 */
    s32 seId;                   /* 0x9608C */
    u8 pad90[0x10];             /* 0x96090 */
    u8 ind[0x800];              /* 0x960A0 */
    Vec pos[32][32];            /* 0x968A0 */
    Vec vel[32][32];            /* 0x998A0 */
    Vec uv[32][32];             /* 0x9C8A0 */
    u8 col[32][32][4];          /* 0x9F8A0 */
};

extern struct M537Water *lbl_1_bss_9F0;

#ifndef TARGET_PC
typedef char M537Water_size_check[(sizeof(struct M537Water) == 0xA08A0) ? 1 : -1];
#endif
extern struct M537Center *lbl_1_bss_A0C;
extern struct M537Camera *lbl_1_bss_A10;
extern struct M537Stage *lbl_1_bss_A14;

/* Inline list-empty test the original compiled into the destroy-all loops. */
static inline u8 M537ListEmpty(M537_NODE *list)
{
    u8 r = 0;

    if (list->next == list && list->next == list->prev) {
        r = 1;
    }
    return r;
}

#ifdef __cplusplus
}
#endif

#endif
