#ifndef _M513DLL_H
#define _M513DLL_H

#include "types.h"
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "dolphin.h"
#include "humath.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/memory.h"
#include "game/data.h"
#include "game/mg/timer.h"
#include "game/sprite.h"
#include "datanum/m513.h"

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* m513Dll - the module's runtime layer (m513.cpp) shared by every unit. */


typedef struct M513Node_s {
    struct M513Node_s *next;
    struct M513Node_s *prev;
} M513Node;

// Camera pose: eye position, up vector, look-at target.
typedef struct M513Cam_s {
    Vec pos;
    Vec up;
    Vec target;
} M513Cam;

// Stored pose: position plus orientation quaternion.
typedef struct M513Pose_s {
    Vec pos;
    Quaternion rot;
} M513Pose;

// A pose viewed as the seven raw words the pose-table dump prints.
typedef union M513PoseBits_u {
    M513Pose pose;
    u32 w[7];
} M513PoseBits;

// Task registered on the module-wide list; its vtable lives at +8.
struct M513Task_s;
typedef struct M513TaskVtbl_s {
    void *rtti[2];
    struct M513Task_s *(*dtor)(struct M513Task_s *self, s16 flag);
    void (*onFlag)(struct M513Task_s *self, s32 bit);
    void (*update)(struct M513Task_s *self);
    void *unk;
} M513TaskVtbl;

// Pointer-to-member-function triple (MWCC layout) following the task vtable.
typedef struct M513TaskPtmf_s {
    long delta;
    long vOffset;
    void (*func)(void);
} M513TaskPtmf;

typedef struct M513TaskVtblEx_s {
    M513TaskVtbl vtbl;
    M513TaskPtmf ptmf[2];
} M513TaskVtblEx;

typedef struct M513Task_s {
    M513Node link;
    M513TaskVtbl *vtbl;
    u32 flags;
} M513Task;

// Singleton game master (only the base part that m513 touches).
struct M513Master_s;
typedef struct M513MasterVtbl_s {
    void *rtti[2];
    struct M513Master_s *(*dtor)(struct M513Master_s *self, s16 flag);
    void (*setup)(struct M513Master_s *self, OMOBJMAN *objman);
    void (*update)(struct M513Master_s *self);
    void (*finish)(struct M513Master_s *self);
} M513MasterVtbl;

typedef struct M513Master_s {
    s32 pad0;
    M513MasterVtbl *vtbl;
    s32 slot[4];
    s32 mesId;
    s32 counter;
} M513Master;

// Sub-thread: vtable, OSThread, stack pointer, semaphore, argument.
struct M513SubThread_s;
typedef struct M513ThreadVtbl_s {
    void *rtti[2];
    struct M513SubThread_s *(*dtor)(struct M513SubThread_s *self, s16 flag);
    s32 (*run)(struct M513SubThread_s *self, s32 arg);
} M513ThreadVtbl;

typedef struct M513Thread_s {
    OSThread thread;
    u32 *stack;
} M513Thread;

typedef struct M513Sem_s {
    s32 count;
    OSThreadQueue queue;
} M513Sem;

typedef struct M513SubThread_s {
    M513ThreadVtbl *vtbl;
    M513Thread th;
    M513Sem sem;
    s32 arg;
} M513SubThread;

// Mersenne Twister state.
typedef struct M513Rand_s {
    u32 mt[0x270];
    u32 idx;
} M513Rand;

// Functor kept in an object's data slot: slot +0xC runs it.
struct M513Functor_s;
typedef struct M513FunctorVtbl_s {
    void *rtti[2];
    struct M513Functor_s *(*dtor)(struct M513Functor_s *self, s16 flag);
    void (*run)(struct M513Functor_s *self, OMOBJ *obj);
} M513FunctorVtbl;

typedef struct M513Functor_s {
    M513FunctorVtbl *vtbl;
} M513Functor;

// A wrapper that owns a Hu3D model / MgTimer handle.
typedef struct M513Model_s {
    HU3DMODELID id;
} M513Model;

typedef struct M513TimerObj_s {
    MGTIMER *timer;
} M513TimerObj;

typedef u8 (*M513Cmp)(M513Node *a, M513Node *b);

// Slot of the handle table: free/used list links, payload and the handle id
// (20 bit slot index, 12 bit use counter).
typedef struct M513HandleEnt_s {
    struct M513HandleEnt_s *next;
    struct M513HandleEnt_s *prev;
    void *obj;
    union {
        u32 id;
        struct {
            u32 index : 20;
            u32 count : 12;
        } b;
    } h;
} M513HandleEnt;

typedef struct M513HandleTbl_s {
    M513HandleEnt *ents; // slot 0 heads the free list
    M513HandleEnt *tail; // sentinel of the used list
    u32 max;
} M513HandleTbl;

typedef struct M513Sphere_s {
    Vec pos;
    f32 radius;
} M513Sphere;

/* "Indicator" singleton (powerind.cpp). It owns a 640x480 RGB565 copy of the
 * frame buffer, so the whole texture is part of the object; the sub animations
 * are used as particle textures by the other units. */
typedef struct M513IndHook_s {
    M513Node link;
    void **vtbl;
    u32 flags;
    u8 pad10[0x20];
    GXTexObj texObj;
    u8 pad50[0x10];
    u16 frameBuf[0x280 * 0x1E0];
    f32 fade[3];
    ANIMDATA *sprAnim[32];
    ANIMDATA *subAnim[4];
} M513IndHook;

extern M513IndHook *lbl_1_bss_9F0;

/* MWCC pointer-to-member-function triple: this adjust, vtable index, function. */
typedef struct M513Ptmf_s {
    long adj;
    long idx;
    void (*fn)(void *);
} M513Ptmf;

#ifdef TARGET_PC
#define M513_PTMF_TEST(p) ((p)->adj != 0 || (p)->idx != 0 || (p)->fn != 0)
#define M513_PTMF_CALL(p, self) ((p)->fn(self))
#else
long __ptmf_test(const void *ptmf);
void __ptmf_scall(void *self);
#define M513_PTMF_TEST(p) __ptmf_test(p)
#define M513_PTMF_CALL(p, self) __ptmf_scall(self)
#endif

/* std::vector<s16> as built by this module. */
typedef struct M513Vec16_s {
    u32 cap;
    u32 size;
    s16 *data;
} M513Vec16;

/* The "stderr" argument the library's fprintf wrapper ignores. */
#ifdef TARGET_PC
#define M513_STDERR ((void *)0)
#else
extern u8 __files[];
#define M513_STDERR ((void *)(__files + 0xA0))
#endif

// Output of update(): stick x/y, stick magnitude and the pressed buttons.
typedef struct M513InputState_s {
    f32 x;
    f32 y;
    f32 mag;
    u32 btn;
} M513InputState;

struct M513Input_s;
typedef struct M513InputVtbl_s {
    void *rtti[2];
    struct M513Input_s *(*dtor)(struct M513Input_s *self, s16 flag);
    void (*update)(struct M513Input_s *self, M513InputState *out);
} M513InputVtbl;

typedef struct M513Input_s {
    M513InputVtbl *vtbl;
    M513InputState st;
    s32 level;     // CPU strength
    s32 playerNo;
    union {
        s32 padNo; // pad reader
        f32 base;  // CPU: base wait
    } u;
    f32 range;     // CPU: random extra wait
    s32 timer;     // CPU: frames to the next press
} M513Input;

typedef struct M513ModelPtr_s {
    M513Model *p;
} M513ModelPtr;

// Leaf-burst particle effect (0x28 bytes).
typedef struct M513LeafFx_s {
    M513Node link;
    void **vtbl;
    u32 flags;
    s16 modelId;
    HU3DPARTICLE *particle;
    u8 done;
    Vec pos;
} M513LeafFx;

// A thrown leaf (0x58 bytes), member of the leaf set.
typedef struct M513Leaf_s {
    M513Node link;
    M513Model model;
    M513ModelPtr sub[2];
    f32 time;
    s32 unk18;
    Vec pos;
    Vec rot;
    u8 flag;
    s32 idx;
    s32 step;
    f32 frac;
    HSFOBJECT *obj[2];
    M513Ptmf hook;
} M513Leaf;

// Smoke particle object (0x34 bytes).
typedef struct M513Smoke_s {
    M513Node link;
    void **vtbl;
    u32 flags;
    s16 modelId;
    HU3DPARTICLE *particle;
    Vec pos;
    Vec vec;
    u8 active;
} M513Smoke;

// Controller (0x48 bytes): owns the smoke object and a draw hook.
typedef struct M513PowerEff_s {
    M513Node link;
    void **vtbl;
    u32 flags;
    Vec pos;
    Vec target;
    s16 hookModel;
    u8 col[4];
    f32 timer;
    s32 state;
    f32 limit;
    M513Smoke *smoke;
    s32 frame;
    f32 scale;
} M513PowerEff;

typedef struct M513Score_s {
    M513Node link;
    void **vtbl;
    u32 flags;
    MGTIMER *timer;
    MGTIMER *timers[4];
    s32 recordTime;
    u8 created;
    u8 multi;
    s16 esp[4];
    s16 win[4];
} M513Score;

typedef struct M513Heiho_s {
    M513Node link;
    M513Model model;
    s32 motion;
    M513Vec16 mot;
    Vec pos;
    f32 unk28;
    Vec rot;
    s32 idx;
    s32 state;
    f32 t;
    M513Model item;
    f32 t2;
    s32 cnt;
    M513Ptmf hook;
} M513Heiho;

/* Size of the pad input object: it ends before the CPU-only fields. */
#define M513_PADINPUT_SIZE offsetof(M513Input, range)

/* The camera singleton (camera.cpp): task base, camera pose, state hook. */
typedef struct M513Camera_s {
    M513Node link;
    void **vtbl;
    u32 flags;
    M513Cam cam;
    M513Ptmf pmf;
    s16 motionId;
    f32 angle;
    Vec destPos;
    Vec destTarget;
} M513Camera;

typedef struct M513InputPtr_s {
    M513Input *p;
} M513InputPtr;

typedef struct M513LeafPtr_s {
    M513Leaf *p;
} M513LeafPtr;

typedef struct M513Player_s {
    M513Node link;
    Vec pos;
    f32 unk14;
    Vec rot;
    s32 active;
    M513Ptmf hook;
    s32 charNo;
    s32 motion;
    s32 playerNo;
    s32 state;
    f32 t;
    s32 score;
    f32 cryT;
    u8 done;
    u8 type;
    M513PowerEff *eff[16];
    s32 effIdx;
    M513Vec16 mot;
    M513Model *model;
    M513Input *input;
    M513Leaf *leaf;
} M513Player;

typedef struct M513Kuribo_s {
    M513Node link;
    M513Model model;
    Vec pos;
    f32 unk18;
    Vec rot;
    s32 motion;
    s32 target;
    M513Vec16 mot;
    s32 state;
    f32 t;
    s32 dir;
    f32 wait;
    s32 mode;
    M513Ptmf hook;
} M513Kuribo;

typedef struct M513Torch_s {
    M513Node link;
    void **vtbl;
    s32 flags;
    s16 modelId;
    HU3DPARTICLE *particle;
    f32 time;
    s32 count;
    s32 emit;
} M513Torch;

/* The stage object: three models loaded from DATA_m513. */
typedef struct M513Stage_s {
    M513Node link;
    void **vtbl;
    u32 flags;
    M513Model model[3];
} M513Stage;

/* Singleton pointers (defined in kuribo.cpp / leaf.cpp). */
extern M513Node *lbl_1_bss_A10;
extern M513Node *lbl_1_bss_A14;
extern M513Node *lbl_1_bss_A0C;
extern M513Node *lbl_1_bss_9F8;
extern M513ModelPtr *lbl_1_bss_9FC;
extern M513Camera *lbl_1_bss_A18;
extern M513Stage *lbl_1_bss_A1C;
extern M513Score *lbl_1_bss_A20;

// The four intrusive sets the singletons point into.
typedef struct M513Sets_s {
    M513Node players;
    M513Node leaves;
    M513Node heihos;
    M513Node kuribos;
} M513Sets;

typedef struct M513SetsPtr_s {
    M513Sets *p;
} M513SetsPtr;

typedef struct M513GameMaster_s {
    M513Master base;
    M513SubThread thread;
    M513SetsPtr sets;
    M513ModelPtr *mp;
    s32 record;
    u8 multi;
    s32 winner;
    M513TimerObj timer;
    u8 recordChanged;
} M513GameMaster;


M513GameMaster *fn_1_B570(M513GameMaster *self);

/* Singleton registration: the original stores the pointer to the singleton<T>
 * subobject (this + 0x10) rebased by -0x10, i.e. the object itself. */
#ifdef TARGET_PC
#define M513_SINGLETON_SET(var, self, T) ((var) = (self))
#define M513_SUBOBJ_NONNULL(self) ((self) != 0)
#else
#define M513_SINGLETON_SET(var, self, T)                                                               {                                                                                                      T *ms_ = (T *)((char *)(self) + 0x10);                                                             if (ms_ != 0) {                                                                                        ms_ = (T *)((char *)ms_ - 0x10);                                                               }                                                                                                  (var) = ms_;                                                                                   }
#define M513_SUBOBJ_NONNULL(self) ((void *)((char *)(self) + 0x10) != 0)
#endif

#define M513_ASSERT(cond, expr, file, line) ((cond) ? (void)0 : fn_1_1D4((expr), (file), (line)))

extern M513Node lbl_1_bss_C;
extern M513TaskVtblEx lbl_1_data_218;
extern M513MasterVtbl lbl_1_data_200;

void *fn_1_0(s32 size);
void *fn_1_64(s32 size);
void fn_1_C8(void *ptr);
void fn_1_100(void *ptr);
void _prolog(void);
void _epilog(void);
void fn_1_1D4(const char *expr, const char *file, s32 line);
int fn_1_234(void *unused, const char *fmt, ...);
void fn_1_2D8(M513Cam *self, f32 pitch, f32 yaw);
void fn_1_39C(M513Cam *self, f32 angle);
void fn_1_408(M513Cam *self, Vec *x, Vec *y, Vec *z);
void fn_1_4B0(M513Cam *self, Mtx out);
void fn_1_504(M513Cam *self, Vec *local);
void fn_1_56C(M513Cam *self, Vec *delta);
void fn_1_5C0(M513Cam *self);
void fn_1_654(M513Cam *self, M513Pose *pose);
void fn_1_6F4(M513Cam *self, M513Pose *out);
void fn_1_76C(M513Cam *self, M513Pose *a, M513Pose *b, f32 t);
void fn_1_87C(M513Cam *self, M513Cam *src);
void fn_1_8E8(M513Cam *self, Vec *a, Vec *b, f32 t);
void fn_1_9A0(M513Cam *self, u16 camBit);
void fn_1_9E0(M513Cam *self, u16 camBit);
void fn_1_A20(M513Cam *self);
f32 fn_1_AB8(s32 pad);
f32 fn_1_B0C(s32 pad);
f32 fn_1_B60(s32 pad);
f32 fn_1_BB4(s32 pad);
void fn_1_C08(M513Cam *self, s32 pad);
M513HandleTbl *fn_1_1264(M513HandleTbl *self, u32 n);
M513HandleTbl *fn_1_1468(M513HandleTbl *self, s16 flag);
u32 fn_1_14D8(M513HandleTbl *self, void *obj);
void *fn_1_1568(M513HandleTbl *self, u32 id);
void *fn_1_162C(M513HandleTbl *self, u32 id);
s32 fn_1_169C(M513Node *self);
s32 fn_1_16D8(M513HandleTbl *self);
s32 fn_1_1720(M513HandleTbl *self);
void fn_1_1768(M513Node *self);
void *fn_1_1774(M513Node *self, s16 flag);
void fn_1_17E0(M513Node *self);
void fn_1_1804(M513Node *self, M513Node *other);
void fn_1_1834(M513Node *self, M513Node *other);
void fn_1_1864(M513Node *self, M513Node *other);
void fn_1_18EC(M513Node *self, M513Cmp cmp);
M513Node *fn_1_19A4(M513Cmp cmp, M513Node *list);
M513Node *fn_1_204C(M513Node *list);
M513Node *fn_1_20A0(M513Cmp cmp, M513Node *a, M513Node *b);
void fn_1_214C(s32 bit);
s32 fn_1_21E4(void);
void fn_1_2270(void);
void fn_1_22E0(M513Task *self);
void fn_1_22E4(void);
M513Task *fn_1_23AC(M513Task *self, s16 flag);
s32 fn_1_2434(void);
M513Master *fn_1_2444(M513Master *self);
M513Master *fn_1_2514(M513Master *self, s16 flag);
M513Master *fn_1_25D0(M513Master *self, s16 flag);
void fn_1_2788(M513Master *self, s32 objMax, s32 prio);
void fn_1_280C(M513Master *self);
void fn_1_2874(OMOBJ *obj);
void fn_1_2904(M513Master *self);
void fn_1_2970(M513Master *self);
void fn_1_2974(void);
void fn_1_2A60(s32 player);
s32 fn_1_2B34(s32 player);
void fn_1_2C08(void);
void fn_1_2CC0(void);
s32 fn_1_2D78(void);
void fn_1_2E4C(void);
void fn_1_3018(s32 mes);
void fn_1_30F0(void);
void fn_1_311C(void);
void fn_1_3148(void);
M513Model *fn_1_315C(M513Model *self, void *data);
M513Model *fn_1_3198(M513Model *self, s32 dataNum);
M513Model *fn_1_31E0(M513Model *self, HU3DMODELHOOK hook);
M513Model *fn_1_321C(M513Model *self, s32 charNo, s32 type);
M513Model *fn_1_32B8(M513Model *self, M513Model *parent);
void fn_1_330C(M513Model *self, u32 attr);
void fn_1_3344(M513Model *self, u32 attr);
void fn_1_337C(M513Model *self, OMOBJ *obj);
void fn_1_33F8(M513Model *self, Vec *pos);
void fn_1_3440(M513Model *self, Vec *rot);
void fn_1_3488(M513Model *self, Vec *scale);
void fn_1_34D0(M513Model *self, f32 scale);
void fn_1_3518(M513Model *self, f32 tpLvl);
void fn_1_3550(M513Model *self, s16 layer);
void fn_1_3588(M513Model *self, u16 camBit);
void fn_1_35C0(M513Model *self, void *data);
MtxPtr fn_1_35DC(M513Model *self);
M513Model *fn_1_35F8(M513Model *self, s16 flag);
HSFOBJECT *fn_1_3654(M513Model *self, char *name);
void fn_1_36E8(M513Model *self);
void fn_1_3718(M513Model *self);
void fn_1_3748(M513Model *self, char *name);
void fn_1_3780(M513Model *self);
void fn_1_37B0(M513Model *self, char *name);
void fn_1_385C(M513Model *self, char *name);
void fn_1_3904(M513Model *self, char *name);
void fn_1_39AC(M513Model *self, char *name);
void fn_1_3A88(M513Model *self, char *name);
void fn_1_3B64(M513Model *self, s32 time);
void fn_1_3BD0(M513Rand *r, u32 seed);
void fn_1_3C44(M513Rand *r, u32 *key, u32 len);
u32 fn_1_3E08(M513Rand *r);
u32 fn_1_3F94(M513Rand *r);
f32 fn_1_412C(M513Rand *r);
f32 fn_1_42F0(M513Rand *r);
f32 fn_1_44B4(M513Rand *r);
f64 fn_1_4688(M513Rand *r);
f32 fn_1_4A00(M513Rand *r);
void *fn_1_4BD0(s32 size);
void *fn_1_4C5C(s32 size);
void fn_1_4CE8(void *ptr);
void fn_1_4D20(void *ptr);
void fn_1_4D58(M513Functor *self, OMOBJ *obj);
void fn_1_4D78(OMOBJ *obj);
void fn_1_4DC4(M513Functor *self, OMOBJ *obj);
void fn_1_4DC8(OMOBJ *obj);
M513Functor *fn_1_4E38(M513Functor *self, s16 flag);
void fn_1_4E98(OMOBJMAN *man, OMOBJ *obj);
void fn_1_4F0C(u32 seed);
u32 fn_1_4FE4(void);
f32 fn_1_5220(void);
f32 fn_1_5494(f32 scale);
f32 fn_1_54CC(f32 from, f32 to);
f32 fn_1_5520(Vec *out, Vec *dir, Vec *target, f32 size);
s32 fn_1_55C8(M513Sphere *a, M513Sphere *b, f32 ka, f32 kb);
s32 fn_1_5748(void);
M513SubThread *fn_1_576C(M513SubThread *self, u32 stackSize);
M513SubThread *fn_1_57E8(M513SubThread *self, s16 flag);
u8 fn_1_5854(M513SubThread *self);
s32 fn_1_589C(M513SubThread *self, s32 arg);
s32 fn_1_592C(M513SubThread *self, s32 arg);
void *fn_1_5A30(void *arg);
M513Thread *fn_1_5A8C(M513Thread *th, void *(*func)(void *), void *arg, u32 size, s32 prio, u16 attr);
M513Thread *fn_1_5B70(M513Thread *th, void *(*func)(void *), void *arg, u32 *stack, u32 size, s32 prio, u16 attr);
void fn_1_5BEC(M513Thread *th, void *(*func)(void *), void *arg, u32 *stack, u32 words, s32 prio, u16 attr);
M513Thread *fn_1_5D10(M513Thread *th, s16 flag);
u32 fn_1_5E44(M513Thread *th);
s32 fn_1_5EE4(u32 *p, s32 mode);
void fn_1_5F58(u32 *p, u32 words);
M513TimerObj *fn_1_5FA4(M513TimerObj *self, s32 type);
M513TimerObj *fn_1_606C(M513TimerObj *self, s16 flag);
void fn_1_60C8(M513TimerObj *self, s32 maxTime, s32 endTime, s32 recordTime);
void fn_1_6110(M513TimerObj *self);
void fn_1_6140(M513TimerObj *self, f32 x, f32 y);
void fn_1_6180(M513TimerObj *self, f32 *x, f32 *y);
void fn_1_61C0(M513TimerObj *self, s32 offType);
void fn_1_6284(M513TimerObj *self);
void fn_1_62B4(M513TimerObj *self);
void fn_1_62E0(M513TimerObj *self);
u8 fn_1_631C(M513TimerObj *self);
void fn_1_635C(M513TimerObj *self, u8 r, u8 g, u8 b);
void fn_1_63A4(void);
void *fn_1_6504(M513Node *self, s16 flag);

#ifdef __cplusplus
}
#endif

#endif
