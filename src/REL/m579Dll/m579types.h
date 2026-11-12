#ifndef M579TYPES_H
#define M579TYPES_H
/* Object types shared by the m579 units; needs Vec from the caller. */

typedef struct M579Node {
    M579Node *next;
    M579Node *prev;
} M579Node;

/* Header of every m579 object: list node, vtable and flag word. */
typedef struct M579Base {
    M579Node node;
    void **vtbl;
    u32 flags;
} M579Base;

/* Pointer to member function in the CodeWarrior layout. */
typedef struct M579Ptmf {
    s32 adj;
    s32 vtblIdx;
    void *func;
} M579Ptmf;

typedef struct M579Cam {
    Vec pos;
    Vec up;
    Vec target;
} M579Cam;

/* position + rotation quaternion, filled by fn_1_6F4 */
typedef struct M579Pose {
    Vec pos;
    f32 rot[4];
} M579Pose;


typedef struct M579Puku M579Puku;
typedef struct M579Player M579Player;

/* Fish actor (fishline.cpp); its rope simulation lives in fishman.cpp. */
typedef struct M579Fish {
    M579Node node;
    Vec pos;
    s32 index;
    f32 rotY;
    u8 caught;
    u8 held;
    u8 b1E;
    u8 b1F;
    u8 b20;
    s32 count24;
    Vec axis;
    f32 seg[20];
    Mtx mtx[20];
    u8 ready;
    u8 ropeInit;
    Vec ropePos;
    Vec ropeVel;
    f32 ropeLen;
    u8 ropeFlag[32];
    Vec ropePt[30];
    Vec ropeFrc[30];
    Vec lineOfs;
    Vec linePrev;
    s16 model;
    u32 rnd;
    s32 mode;
    s32 timer;
    s32 wait;
    f32 f780;
    f32 f784;
    f32 f788;
    f32 f78C;
    M579Player *holder;
    M579Puku *puku;
    f32 yaw;
} M579Fish;

/* Spray particle effect (water.cpp, effect.cpp). */
typedef struct M579Spray {
    M579Node node;
    s16 model;
    s32 count;
} M579Spray;

/* Expanding ripple on the water surface (water.cpp). */
typedef struct M579Ripple {
    M579Node node;
    s16 model;
    Vec pos;
    f32 scale;
    f32 maxScale;
    u8 alpha;
} M579Ripple;

typedef struct M579S16Vec {
    u32 cap;
    u32 size;
    s16 *data;
} M579S16Vec;

/* Singleton owning the motion id table (puku2.cpp reads it). */
typedef struct M579MotionMgr {
    M579Base base;
    M579S16Vec motion;
    s16 model[2];
} M579MotionMgr;

typedef struct M579Puku {
    M579Node node;
    M579Ptmf update;
    s32 anim;
    s32 u18;
    Vec pos;
    Vec prev;
    Vec vel;
    s32 frame;
    s16 model;
    u8 dead;
    u8 busy;
    s32 type;
    M579Fish *owner;
    Vec rot;
    u8 state;
} M579Puku;

#ifdef TARGET_PC
#include <stdio.h>
#define M579_STDERR ((void *) stderr)
#else
extern "C" u8 __files[];
#define M579_STDERR ((void *) (__files + 0xA0))
#endif

#ifdef __cplusplus
extern "C" {
void *fn_1_0(s32 size);
void fn_1_C8(void *self);
void fn_1_1D4(const char *expr, const char *file, s32 line);
void fn_1_1264(void *self);
void fn_1_1270(void *self, s32 arg);
void fn_1_1330(void *self, void *list);
void fn_1_12DC(void *self);
void fn_1_234(void *a, void *b);
void abort(void);
s32 fn_1_4AE0(void);
f32 fn_1_4D1C(void);
void fn_1_9668(Vec *pos, s32 seId);
void fn_1_1360(void *list, void *dst);
/* model wrappers (m579.cpp); a model is an s16 handle */
s16 *fn_1_2DB4(s16 *dst, s16 *src);
void fn_1_2E08(s16 *model, s32 attr);
void fn_1_2E40(s16 *model, s32 attr);
void fn_1_2EF4(s16 *model, Vec *pos);
void fn_1_2F3C(s16 *model, Vec *rot);
void fn_1_2FCC(s16 *model, f32 scale);
void fn_1_3014(s16 *model, f32 tpLvl);
void fn_1_304C(s16 *model, s16 layer);
void fn_1_30F4(s16 *model, s32 flag);
void fn_1_31E4(s16 *model);
}
#endif

#ifdef __MWERKS__
#define M579_RODATA __declspec(section ".rodata")
#else
#define M579_RODATA
#endif

#endif
