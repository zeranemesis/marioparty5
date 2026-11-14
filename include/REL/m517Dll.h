#ifndef _M517DLL_H
#define _M517DLL_H

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
#include "datanum/m517.h"

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* m517Dll - the module's runtime layer (m517.cpp) shared by every unit. */


typedef struct M517Node_s {
    struct M517Node_s *next;
    struct M517Node_s *prev;
} M517Node;

// Camera pose: eye position, up vector, look-at target.
typedef struct M517Cam_s {
    Vec pos;
    Vec up;
    Vec target;
} M517Cam;

// Stored pose: position plus orientation quaternion.
typedef struct M517Pose_s {
    Vec pos;
    Quaternion rot;
} M517Pose;

// A pose viewed as the seven raw words the pose-table dump prints.
typedef union M517PoseBits_u {
    M517Pose pose;
    u32 w[7];
} M517PoseBits;

// Task registered on the module-wide list; its vtable lives at +8.
struct M517Task_s;
typedef struct M517TaskVtbl_s {
    void *rtti[2];
    struct M517Task_s *(*dtor)(struct M517Task_s *self, s16 flag);
    void (*onFlag)(struct M517Task_s *self, s32 bit);
    void (*update)(struct M517Task_s *self);
    void *unk;
} M517TaskVtbl;

// Pointer-to-member-function triple (MWCC layout) following the task vtable.
typedef struct M517TaskPtmf_s {
    long delta;
    long vOffset;
    void (*func)(void);
} M517TaskPtmf;

typedef struct M517TaskVtblEx_s {
    M517TaskVtbl vtbl;
    M517TaskPtmf ptmf[2];
} M517TaskVtblEx;

typedef struct M517Task_s {
    M517Node link;
    M517TaskVtbl *vtbl;
    u32 flags;
} M517Task;

// Singleton game master (only the base part that m517 touches).
struct M517Master_s;
typedef struct M517MasterVtbl_s {
    void *rtti[2];
    struct M517Master_s *(*dtor)(struct M517Master_s *self, s16 flag);
    void (*setup)(struct M517Master_s *self, OMOBJMAN *objman);
    void (*update)(struct M517Master_s *self);
    void (*finish)(struct M517Master_s *self);
    void *slot6; // unused trailing vtable word
} M517MasterVtbl;

typedef struct M517Master_s {
    s32 pad0;
    M517MasterVtbl *vtbl;
    s32 slot[4];
    s32 mesId;
    s32 counter;
} M517Master;

// Sub-thread: vtable, OSThread, stack pointer, semaphore, argument.
struct M517SubThread_s;
typedef struct M517ThreadVtbl_s {
    void *rtti[2];
    struct M517SubThread_s *(*dtor)(struct M517SubThread_s *self, s16 flag);
    s32 (*run)(struct M517SubThread_s *self, s32 arg);
} M517ThreadVtbl;

typedef struct M517Thread_s {
    OSThread thread;
    u32 *stack;
} M517Thread;

#ifdef TARGET_PC
typedef OSSemaphore M517Sem; /* Aurora's semaphore is larger than the GC one */
#else
typedef struct M517Sem_s {
    s32 count;
    OSThreadQueue queue;
} M517Sem;
#endif

typedef struct M517SubThread_s {
    M517ThreadVtbl *vtbl;
    M517Thread th;
    M517Sem sem;
    s32 arg;
} M517SubThread;

// Mersenne Twister state.
typedef struct M517Rand_s {
    u32 mt[0x270];
    u32 idx;
} M517Rand;

// Functor kept in an object's data slot: slot +0xC runs it.
struct M517Functor_s;
typedef struct M517FunctorVtbl_s {
    void *rtti[2];
    struct M517Functor_s *(*dtor)(struct M517Functor_s *self, s16 flag);
    void (*run)(struct M517Functor_s *self, OMOBJ *obj);
} M517FunctorVtbl;

typedef struct M517Functor_s {
    M517FunctorVtbl *vtbl;
} M517Functor;

// A wrapper that owns a Hu3D model / MgTimer handle.
typedef struct M517Model_s {
    HU3DMODELID id;
} M517Model;

typedef struct M517TimerObj_s {
    MGTIMER *timer;
} M517TimerObj;

typedef u8 (*M517Cmp)(M517Node *a, M517Node *b);

// Slot of the handle table: free/used list links, payload and the handle id
// (20 bit slot index, 12 bit use counter).
typedef struct M517HandleEnt_s {
    struct M517HandleEnt_s *next;
    struct M517HandleEnt_s *prev;
    void *obj;
    union {
        u32 id;
        struct {
            u32 index : 20;
            u32 count : 12;
        } b;
    } h;
} M517HandleEnt;

typedef struct M517HandleTbl_s {
    M517HandleEnt *ents; // slot 0 heads the free list
    M517HandleEnt *tail; // sentinel of the used list
    u32 max;
} M517HandleTbl;

typedef struct M517Sphere_s {
    Vec pos;
    f32 radius;
} M517Sphere;

/* MWCC pointer-to-member-function triple: this adjust, vtable index, function. */
typedef struct M517Ptmf_s {
    long adj;
    long idx;
    void (*fn)(void *);
} M517Ptmf;

#ifdef TARGET_PC
#define M517_PTMF_TEST(p) ((p)->adj != 0 || (p)->idx != 0 || (p)->fn != 0)
#define M517_PTMF_CALL(p, self) ((p)->fn(self))
#else
long __ptmf_test(const void *ptmf);
void __ptmf_scall(void *self);
#define M517_PTMF_TEST(p) __ptmf_test(p)
#define M517_PTMF_CALL(p, self) __ptmf_scall(self)
#endif

/* std::vector<s16> as built by this module. */
typedef struct M517Vec16_s {
    u32 cap;
    u32 size;
    s16 *data;
} M517Vec16;

/* The "stderr" argument the library's fprintf wrapper ignores. */
#ifdef TARGET_PC
#define M517_STDERR ((void *)0)
#else
extern u8 __files[];
#define M517_STDERR ((void *)(__files + 0xA0))
#endif

typedef struct M517ModelPtr_s {
    M517Model *p;
} M517ModelPtr;

/* Singleton registration: the original stores the pointer to the singleton<T>
 * subobject (this + 0x10) rebased by -0x10, i.e. the object itself. */
#ifdef TARGET_PC
#define M517_SINGLETON_SET(var, self, T) ((var) = (self))
#define M517_SUBOBJ_NONNULL(self) ((self) != 0)
#else
#define M517_SINGLETON_SET(var, self, T)                                                               {                                                                                                      T *ms_ = (T *)((char *)(self) + 0x10);                                                             if (ms_ != 0) {                                                                                        ms_ = (T *)((char *)ms_ - 0x10);                                                               }                                                                                                  (var) = ms_;                                                                                   }
#define M517_SUBOBJ_NONNULL(self) ((void *)((char *)(self) + 0x10) != 0)
#endif

#define M517_ASSERT(cond, expr, file, line) ((cond) ? (void)0 : fn_1_2058((expr), (file), (line)))

extern M517Node lbl_1_bss_24;
extern u8 lbl_1_bss_A00;
extern M517Master *lbl_1_bss_A04;
extern M517TaskVtblEx lbl_1_data_68;
extern M517MasterVtbl lbl_1_data_244;

void *fn_1_1EE8(s32 size);
void fn_1_1F4C(void *ptr);
void fn_1_1F84(void *ptr);
void _prolog(void);
void _epilog(void);
void fn_1_2058(const char *expr, const char *file, s32 line);
int fn_1_20B8(void *unused, const char *fmt, ...);
void fn_1_215C(M517Cam *self, f32 pitch, f32 yaw);
void fn_1_2220(M517Cam *self, f32 angle);
void fn_1_228C(M517Cam *self, Vec *x, Vec *y, Vec *z);
void fn_1_2334(M517Cam *self, Mtx out);
void fn_1_2388(M517Cam *self, Vec *local);
void fn_1_23F0(M517Cam *self, Vec *delta);
void fn_1_2444(M517Cam *self);
void fn_1_24D8(M517Cam *self, M517Pose *pose);
void fn_1_2578(M517Cam *self, M517Pose *out);
void fn_1_25F0(M517Cam *self, M517Pose *a, M517Pose *b, f32 t);
void fn_1_2700(M517Cam *self, M517Cam *src);
void fn_1_276C(M517Cam *self, Vec *a, Vec *b, f32 t);
void fn_1_2824(M517Cam *self, u16 camBit);
void fn_1_2864(M517Cam *self, u16 camBit);
M517HandleTbl *fn_1_28A4(M517HandleTbl *self, u32 n);
M517HandleTbl *fn_1_2AA8(M517HandleTbl *self, s16 flag);
u32 fn_1_2B18(M517HandleTbl *self, void *obj);
void *fn_1_2BA8(M517HandleTbl *self, u32 id);
void *fn_1_2C6C(M517HandleTbl *self, u32 id);
s32 fn_1_2CDC(M517Node *self);
s32 fn_1_2D18(M517HandleTbl *self);
s32 fn_1_2D60(M517HandleTbl *self);
void fn_1_2DA8(M517Node *self);
void *fn_1_2DB4(M517Node *self, s16 flag);
void fn_1_2E20(M517Node *self);
void fn_1_2E44(M517Node *self, M517Node *other);
void fn_1_2E74(M517Node *self, M517Node *other);
void fn_1_2EA4(M517Node *self, M517Node *other);
void fn_1_2F2C(M517Node *self, M517Cmp cmp);
M517Node *fn_1_2FE4(M517Cmp cmp, M517Node *list);
M517Node *fn_1_368C(M517Node *list);
M517Node *fn_1_36E0(M517Cmp cmp, M517Node *a, M517Node *b);
void fn_1_378C(s32 bit);
s32 fn_1_3824(void);
void fn_1_38B0(void);
void fn_1_3920(void);
s32 fn_1_39E8(void);
M517Master *fn_1_39F8(M517Master *self);
M517Master *fn_1_3AC8(M517Master *self, s16 flag);
M517Master *fn_1_3B84(M517Master *self, s16 flag);
void fn_1_3D3C(M517Master *self, s32 objMax, s32 prio);
void fn_1_3DC0(M517Master *self);
void fn_1_3E28(OMOBJ *obj);
void fn_1_3EB8(M517Master *self);
void fn_1_3F24(M517Master *self);
void fn_1_3F28(void);
void fn_1_4014(s32 player);
s32 fn_1_40E8(s32 player);
void fn_1_41BC(void);
void fn_1_4274(void);
s32 fn_1_432C(void);
void fn_1_4400(void);
void fn_1_45CC(s32 mes);
void fn_1_46A4(void);
void fn_1_46D0(void);
void fn_1_46FC(void);
M517Model *fn_1_4710(M517Model *self, void *data);
M517Model *fn_1_474C(M517Model *self, s32 dataNum);
M517Model *fn_1_4794(M517Model *self, HU3DMODELHOOK hook);
M517Model *fn_1_47D0(M517Model *self, s32 charNo, s32 type);
M517Model *fn_1_486C(M517Model *self, M517Model *parent);
void fn_1_48C0(M517Model *self, u32 attr);
void fn_1_48F8(M517Model *self, u32 attr);
void fn_1_4930(M517Model *self, OMOBJ *obj);
void fn_1_49AC(M517Model *self, Vec *pos);
void fn_1_49F4(M517Model *self, Vec *rot);
void fn_1_4A3C(M517Model *self, Vec *scale);
void fn_1_4A84(M517Model *self, f32 scale);
void fn_1_4ACC(M517Model *self, f32 tpLvl);
void fn_1_4B04(M517Model *self, s16 layer);
void fn_1_4B3C(M517Model *self, u16 camBit);
void fn_1_4B74(M517Model *self, void *data);
MtxPtr fn_1_4B90(M517Model *self);
M517Model *fn_1_4BAC(M517Model *self, s16 flag);
HSFOBJECT *fn_1_4C08(M517Model *self, char *name);
void fn_1_4C9C(M517Model *self);
void fn_1_4CCC(M517Model *self);
void fn_1_4CFC(M517Model *self, char *name);
void fn_1_4D34(M517Model *self);
void fn_1_4D64(M517Model *self, char *name);
void fn_1_4E10(M517Model *self, char *name);
void fn_1_4EB8(M517Model *self, char *name);
void fn_1_4F60(M517Model *self, char *name);
void fn_1_503C(M517Model *self, char *name);
void fn_1_5118(M517Model *self, s32 time);
void fn_1_5184(M517Rand *r, u32 seed);
void fn_1_51F8(M517Rand *r, u32 *key, u32 len);
u32 fn_1_53BC(M517Rand *r);
u32 fn_1_5548(M517Rand *r);
f32 fn_1_56E0(M517Rand *r);
f32 fn_1_58A4(M517Rand *r);
f32 fn_1_5A68(M517Rand *r);
f64 fn_1_5C3C(M517Rand *r);
f32 fn_1_5FB4(M517Rand *r);
void *fn_1_6184(s32 size);
void *fn_1_6210(s32 size);
void fn_1_629C(void *ptr);
void fn_1_62D4(void *ptr);
void fn_1_630C(M517Functor *self, OMOBJ *obj);
void fn_1_632C(OMOBJ *obj);
void fn_1_6378(M517Functor *self, OMOBJ *obj);
void fn_1_637C(OMOBJ *obj);
M517Functor *fn_1_63EC(M517Functor *self, s16 flag);
void fn_1_644C(OMOBJMAN *man, OMOBJ *obj);
void fn_1_64C0(u32 seed);
u32 fn_1_6598(void);
f32 fn_1_67D4(void);
f32 fn_1_6A48(f32 scale);
f32 fn_1_6A80(f32 from, f32 to);
s32 fn_1_6AD4(M517Sphere *a, M517Sphere *b);
s32 fn_1_6B7C(M517Sphere *a, M517Sphere *b, f32 ka, f32 kb);
s32 fn_1_6CFC(void);
M517SubThread *fn_1_6D20(M517SubThread *self, u32 stackSize);
M517SubThread *fn_1_6D9C(M517SubThread *self, s16 flag);
u8 fn_1_6E08(M517SubThread *self);
s32 fn_1_6E50(M517SubThread *self, s32 arg);
s32 fn_1_6EE0(M517SubThread *self, s32 arg);
void *fn_1_6FE4(void *arg);
M517Thread *fn_1_7040(M517Thread *th, void *(*func)(void *), void *arg, u32 size, s32 prio, u16 attr);
M517Thread *fn_1_7124(M517Thread *th, void *(*func)(void *), void *arg, u32 *stack, u32 size, s32 prio, u16 attr);
void fn_1_71A0(M517Thread *th, void *(*func)(void *), void *arg, u32 *stack, u32 words, s32 prio, u16 attr);
M517Thread *fn_1_72C4(M517Thread *th, s16 flag);
u32 fn_1_73F8(M517Thread *th);
s32 fn_1_7498(u32 *p, s32 mode);
void fn_1_750C(u32 *p, u32 words);
M517TimerObj *fn_1_7558(M517TimerObj *self, s32 type);
M517TimerObj *fn_1_7620(M517TimerObj *self, s16 flag);
void fn_1_767C(M517TimerObj *self, s32 maxTime, s32 endTime, s32 recordTime);
void fn_1_76C4(M517TimerObj *self);
void fn_1_76F4(M517TimerObj *self, f32 x, f32 y);
void fn_1_7734(M517TimerObj *self, f32 *x, f32 *y);
void fn_1_7774(M517TimerObj *self, s32 offType);
void fn_1_7838(M517TimerObj *self);
void fn_1_7868(M517TimerObj *self);
void fn_1_7894(M517TimerObj *self);
u8 fn_1_78D0(M517TimerObj *self);
void fn_1_7910(M517TimerObj *self, u8 r, u8 g, u8 b);
void fn_1_7958(void);
void *fn_1_7AB8(M517Node *self, s16 flag);

/* ---- input unit (input.cpp): player input sources (pad reader / two CPU brains) ---- */

typedef struct M517InputState_s {
    f32 x;
    f32 y;
    f32 mag;
} M517InputState;

struct M517Input_s;
typedef struct M517InputVtbl_s {
    void *rtti[2];
    struct M517Input_s *(*dtor)(struct M517Input_s *self, s16 flag);
    void (*update)(struct M517Input_s *self, M517InputState *out);
} M517InputVtbl;

typedef struct M517Input_s {
    M517InputVtbl *vtbl;
    M517InputState st;
    s32 level; // CPU strength (0 for the pad reader)
} M517Input;

#ifdef TARGET_PC
typedef M517Input *M517InputCtorRet; /* callers use the result: GC returns self in r3 */
#else
typedef void M517InputCtorRet;
#endif

void fn_1_7B2C(M517Input *self);
void fn_1_7B74(M517Input *self);
M517InputCtorRet fn_1_7BD4(M517Input *self, s32 playerNo);
M517Input *fn_1_7C44(M517Input *self, s16 flag);
void fn_1_7C9C(M517Input *self, M517InputState *out);
M517InputCtorRet fn_1_8000(M517Input *self, s32 playerNo, s32 level);
M517InputCtorRet fn_1_9930(M517Input *self, s32 level);
M517Input *fn_1_8134(M517Input *self, s16 flag);
void fn_1_81A0(M517Input *self, M517InputState *out);
void fn_1_99EC(M517Input *self, M517InputState *out);
M517Input *fn_1_A090(M517Input *self, s16 flag);
M517Input *fn_1_A0FC(M517Input *self, s16 flag);

/* ---- kuribo unit (kuribo.cpp) ---- */
struct M517Kuribo_s;
struct M517Kuribo_s *fn_1_C994(struct M517Kuribo_s *self, Vec *pos, Vec *rot, s32 type);


/* ---- camera / stage units (camera.cpp, stage.cpp): the two singleton task objects ---- */

/* Camera singleton: task base, camera pose and a state hook. */
typedef struct M517Camera_s {
    M517Task task;
    M517Cam cam;
    u8 unk34[0x3C];
    M517Ptmf state;
    u8 farView; // set by the game master once the game starts
    s16 motionId;
} M517Camera;

/* Stage singleton: three models loaded from DATA_m517. */
typedef struct M517Stage_s {
    M517Task task;
    M517Model model[3];
    Vec offs;
} M517Stage;

M517Camera *fn_1_DC4C(M517Camera *self);
M517Camera *fn_1_DDDC(M517Camera *self, s16 flag);
M517Stage *fn_1_E3A4(M517Stage *self);
M517Stage *fn_1_E790(M517Stage *self, s16 flag);

/* ---- audio unit (audio.cpp) ---- */
void fn_1_1B48C(Point3d *pos, s16 cameraBit, Point3d *out);
s32 fn_1_1B6EC(s32 seId, Point3d *pos);
void fn_1_1B9B8(s16 charNo, Point3d *pos);
void fn_1_1BC6C(s32 seNo, Point3d *pos);

#ifdef __cplusplus
}
#endif

#endif
