// m531Dll "m531": the module's runtime layer -- camera pose maths, the
// minigame core singleton, thin MgTimer wrappers and the module entry points.
#define _MATH_H
extern "C" {
#include "stdio.h"
#include "stdarg.h"
#include "dolphin.h"
#include "humath.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/memory.h"
#include "game/gamemes.h"
#include "game/wipe.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/audio.h"
#include "game/pad.h"
#include "game/mgdata.h"
#include "game/charman.h"
#include "game/process.h"
#include "game/mg/timer.h"
#include "game/mg/actman.h"
}

extern "C" {
void ObjectSetup(void);
extern void (*_ctors[])(void);
extern void (*_dtors[])(void);

void fn_1_1D4(const char *expr, const char *file, s32 line);
int fn_1_234(void *unused, const char *fmt, ...);
void fn_1_408(Vec *cam, Vec *x, Vec *y, Vec *z);
void fn_1_56C(Vec *cam, Vec *delta);
}


// The inlined form of the assertion handler (singleton.h, om.cpp, timer.cpp).
#define ASSERT_FAIL(expr, file, line) { char assertBuf[0x400]; sprintf(assertBuf, "\nASSERTION FAILED at %s:%d\n%s", (file), (line), (expr)); OSPanic("assert.cpp", 0x12, assertBuf); }

// Camera pose: eye position, up vector, look-at target.
struct CCam {
    Vec pos;
    Vec up;
    Vec target;
};

// Stored pose: position plus orientation quaternion.
struct CPose {
    Vec pos;
    Quaternion rot;
};

// A pose viewed as the seven raw words the pose-table dump prints.
union CPoseBits {
    CPose pose;
    u32 w[7];
};

// Timer wrapper: owns an MgTimer handle.
class CTimer {
public:
    CTimer(int type);
    MGTIMER *m_timer;
};

// Singleton base of the minigame master: data, then the vtable pointer.
class CMinigameCore {
public:
    u32 m_unk0;

    void *operator new(unsigned long size);
    void operator delete(void *ptr);

    CMinigameCore();
    virtual ~CMinigameCore();
    virtual void on_setup(OMOBJMAN *objMan) = 0;
    virtual void on_update();
    virtual void on_close() = 0;

    void setup(int prio, int stackSize);

    s32 m_winner[4];
    s32 m_unk18;
    s32 m_unk1C;
};

// Object thunk: a lone vtable pointer, slot 0 the deleting destructor and slot 1 run(obj).
struct CThunk;
struct ThunkVt {
    void *m_head[2];
    CThunk *(*m_dtor)(CThunk *self, s16 flag);
    void (*m_run)(CThunk *self, OMOBJ *obj);
};

struct CThunk {
    const ThunkVt *m_vt;
};

extern "C" {
void *fn_1_0(s32 size);
void *fn_1_64(s32 size);
void fn_1_100(void *ptr);
void _prolog(void);
void _epilog(void);
void fn_1_2D8(CCam *self, f32 pitch, f32 yaw);
void fn_1_39C(CCam *self, f32 angle);
void fn_1_4B0(CCam *self, Mtx out);
void fn_1_504(CCam *self, Vec *local);
void fn_1_5C0(CCam *self);
void fn_1_654(CCam *self, CPose *pose);
void fn_1_6F4(CCam *self, CPose *out);
void fn_1_76C(CCam *self, CPose *a, CPose *b, f32 t);
void fn_1_87C(CCam *self, CCam *src);
void fn_1_8E8(CCam *self, Vec *a, Vec *b, f32 t);
void fn_1_9A0(CCam *self, u16 camBit);
void fn_1_9E0(CCam *self, u16 camBit);
void fn_1_A20(CCam *self);
f32 fn_1_AB8(s32 pad);
f32 fn_1_B0C(s32 pad);
f32 fn_1_B60(s32 pad);
f32 fn_1_BB4(s32 pad);
void fn_1_C08(CCam *self, s32 pad);
void **fn_1_1334(void **self, s16 flag);
void fn_1_162C(CMinigameCore *self);
void fn_1_1694(OMOBJ *obj);
void fn_1_1724(CMinigameCore *core);
void fn_1_1794(OMOBJ *obj);
void fn_1_1880(s32 player);
s32 fn_1_1954(s32 player);
void fn_1_1A28(void);
void fn_1_1AE0(void);
s32 fn_1_1B98(void);
void fn_1_1C6C(void);
void fn_1_1E38(s32 mes);
void fn_1_1F10(void);
void fn_1_1F3C(void);
void fn_1_1F68(void);
void *fn_1_2008(s32 size);
void fn_1_20CC(void *ptr);
void fn_1_2104(CThunk *self, OMOBJ *obj);
void fn_1_2124(OMOBJ *obj);
void fn_1_2170(CThunk *self, OMOBJ *obj);
void fn_1_2174(OMOBJ *obj);
CThunk *fn_1_21E4(CThunk *self, s16 flag);
void fn_1_2244(OMOBJMAN *objMan, OMOBJ *obj);
CTimer *fn_1_2380(CTimer *self, s16 flag);
void fn_1_23DC(CTimer *self, s32 a, s32 b, s32 c);
s32 fn_1_2424(CTimer *self);
void fn_1_2454(CTimer *self, f32 x, f32 y);
void fn_1_2494(CTimer *self, f32 *x, f32 *y);
void fn_1_24D4(CTimer *self, s32 mode);
void fn_1_2598(CTimer *self);
void fn_1_25C8(CTimer *self);
void fn_1_25F4(CTimer *self);
u8 fn_1_2630(CTimer *self);
void fn_1_2670(CTimer *self, u8 r, u8 g, u8 b);
void fn_1_26B8(void);

extern s32 lbl_1_bss_0;
extern s32 lbl_1_bss_4;
extern s32 lbl_1_bss_8;
extern CMinigameCore *lbl_1_bss_C;
extern ThunkVt lbl_1_data_BC;
}

s32 lbl_1_bss_0;
s32 lbl_1_bss_4;
s32 lbl_1_bss_8;
CMinigameCore *lbl_1_bss_C;

void *fn_1_0(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    if (p != NULL) {
    } else {
        ASSERT_FAIL("ptr != NULL", "new.cpp", 0x17)
    }
    return p;
}

void *fn_1_64(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    if (p != NULL) {
    } else {
        ASSERT_FAIL("ptr != NULL", "new.cpp", 0x17)
    }
    return p;
}

void operator delete(void *ptr)
{
    if (ptr != NULL) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_100(void *ptr)
{
    if (ptr != NULL) {
        HuMemDirectFree(ptr);
    }
}

#ifdef TARGET_PC
// The GameCube symbol of operator delete, reached by the other units of the module.
extern "C" void *__dl__FPv(void *ptr)
{
    operator delete(ptr);
    return ptr;
}
#endif

void _prolog(void)
{
    void (**ctor)(void) = _ctors;

    while (*ctor != NULL) {
        (*ctor)();
        ctor++;
    }
    ObjectSetup();
}

void _epilog(void)
{
    void (**dtor)(void) = _dtors;

    while (*dtor != NULL) {
        (*dtor)();
        dtor++;
    }
}

void fn_1_1D4(const char *expr, const char *file, s32 line)
{
    char buf[0x40C];

    sprintf(buf, "\nASSERTION FAILED at %s:%d\n%s", file, line, expr);
    OSPanic("assert.cpp", 0x12, buf);
}

int fn_1_234(void *unused, const char *fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = vprintf(fmt, ap);
    va_end(ap);
    return ret;
}

// Orbit: rotate about the camera X axis (pitch) then world Y (yaw).
void fn_1_2D8(CCam *self, f32 pitch, f32 yaw)
{
    Vec ax, ay, az;
    Mtx rotA, rotB, rot;

    fn_1_408(&self->pos, &ax, &ay, &az);
    PSMTXRotAxisRad(rotA, &ax, 0.017453292f * pitch);
    PSMTXRotRad(rotB, 'Y', 0.017453292f * yaw);
    PSMTXConcat(rotA, rotB, rot);
    PSMTXMultVecSR(rot, &az, &az);
    PSMTXMultVecSR(rot, &ay, &self->up);
    PSVECSubtract(&self->pos, &az, &self->target);
}

void fn_1_39C(CCam *self, f32 angle)
{
    Mtx m;
    Vec axis;

    PSVECSubtract(&self->target, &self->pos, &axis);
    PSMTXRotAxisRad(m, &axis, 0.017453292f * angle);
    PSMTXMultVec(m, &self->up, &self->up);
}

// Camera basis: the three rows of the look-at matrix.
void fn_1_408(Vec *cam, Vec *x, Vec *y, Vec *z)
{
    Mtx m;

    C_MTXLookAt(m, &cam[0], &cam[1], &cam[2]);
    x->x = m[0][0];
    x->y = m[0][1];
    x->z = m[0][2];
    y->x = m[1][0];
    y->y = m[1][1];
    y->z = m[1][2];
    z->x = m[2][0];
    z->y = m[2][1];
    z->z = m[2][2];
}

void fn_1_4B0(CCam *self, Mtx out)
{
    C_MTXLookAt(out, &self->pos, &self->up, &self->target);
    PSMTXInverse(out, out);
}

void fn_1_504(CCam *self, Vec *local)
{
    Mtx m;
    Vec world;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    PSMTXMultVecSR(m, local, &world);
    fn_1_56C(&self->pos, &world);
}

void fn_1_56C(Vec *cam, Vec *delta)
{
    CCam *self = (CCam *)cam;

    PSVECAdd(delta, &self->target, &self->target);
    PSVECAdd(delta, &self->pos, &self->pos);
}

void fn_1_5C0(CCam *self)
{
    self->up.x = 0.0f;
    self->up.y = 1.0f;
    self->up.z = 0.0f;
    self->pos.x = 0.0f;
    self->pos.y = 0.0f;
    self->pos.z = 5000.0f;
    self->target.x = 0.0f;
    self->target.y = 0.0f;
    self->target.z = 0.0f;
}

void fn_1_654(CCam *self, CPose *pose)
{
    Mtx m;

    PSMTXQuat(m, &pose->rot);
    self->up.x = m[0][1];
    self->up.y = m[1][1];
    self->up.z = m[2][1];
    self->pos.x = pose->pos.x;
    self->pos.y = pose->pos.y;
    self->pos.z = pose->pos.z;
    self->target.x = pose->pos.x - m[0][2];
    self->target.y = pose->pos.y - m[1][2];
    self->target.z = pose->pos.z - m[2][2];
}

void fn_1_6F4(CCam *self, CPose *out)
{
    Mtx m;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    out->pos.x = self->pos.x;
    out->pos.y = self->pos.y;
    out->pos.z = self->pos.z;
    C_QUATMtx(&out->rot, m);
}

void fn_1_76C(CCam *self, CPose *a, CPose *b, f32 t)
{
    Mtx m;
    Quaternion q;
    Vec p, pa, pb;

    C_QUATSlerp(&a->rot, &b->rot, &q, t);
    PSVECScale(&a->pos, &pa, 1.0f - t);
    PSVECScale(&b->pos, &pb, t);
    PSVECAdd(&pa, &pb, &p);
    PSMTXQuat(m, &q);
    self->up.x = m[0][1];
    self->up.y = m[1][1];
    self->up.z = m[2][1];
    self->pos.x = p.x;
    self->pos.y = p.y;
    self->pos.z = p.z;
    self->target.x = p.x - m[0][2];
    self->target.y = p.y - m[1][2];
    self->target.z = p.z - m[2][2];
}

void fn_1_87C(CCam *self, CCam *src)
{
    Vec delta;

    PSVECSubtract(&src->pos, &self->pos, &delta);
    self->pos.x = src->pos.x;
    self->pos.y = src->pos.y;
    self->pos.z = src->pos.z;
    PSVECAdd(&self->target, &delta, &self->target);
}

void fn_1_8E8(CCam *self, Vec *a, Vec *b, f32 t)
{
    Vec pa, pb, delta;

    PSVECScale(a, &pa, 1.0f - t);
    PSVECScale(b, &pb, t);
    PSVECAdd(&pa, &pb, &pb);
    PSVECSubtract(&pb, &self->pos, &delta);
    self->pos.x = pb.x;
    self->pos.y = pb.y;
    self->pos.z = pb.z;
    PSVECAdd(&self->target, &delta, &self->target);
}

void fn_1_9A0(CCam *self, u16 camBit)
{
    Hu3DCameraPosSetV(camBit, &self->pos, &self->up, &self->target);
}

void fn_1_9E0(CCam *self, u16 camBit)
{
    Hu3DCameraPosGet(camBit, &self->pos, &self->up, &self->target);
}

// Dump the current camera as a pose table row (floats printed as raw words).
void fn_1_A20(CCam *self)
{
    Mtx m;
    CPoseBits out;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    out.pose.pos.x = self->pos.x;
    out.pose.pos.y = self->pos.y;
    out.pose.pos.z = self->pos.z;
    C_QUATMtx(&out.pose.rot, m);
    printf("{{0x%08x,0x%08x,0x%08x},{0x%08x,0x%08x,0x%08x,0x%08x}},\n", out.w[0], out.w[1], out.w[2], out.w[3], out.w[4],
           out.w[5], out.w[6]);
}

f32 fn_1_AB8(s32 pad)
{
    return 0.017857144f * HuPadStkX[pad];
}

f32 fn_1_B0C(s32 pad)
{
    return 0.017857144f * HuPadStkY[pad];
}

f32 fn_1_B60(s32 pad)
{
    return 0.016949153f * HuPadSubStkX[pad];
}

f32 fn_1_BB4(s32 pad)
{
    return 0.016949153f * HuPadSubStkY[pad];
}

// Free-fly debug camera driven by pad `pad`.
void fn_1_C08(CCam *self, s32 pad)
{
    Mtx inv, rot, rotB, rotA, m, dump;
    CPoseBits out;
    Vec mv, fwd, z, y, x;
    f32 yaw, pitch, speed, len;
    u32 btn;
    u8 fast;

    btn = HuPadBtn[pad];
    if (btn & 0x800) {
        self->up.x = 0.0f;
        self->up.y = 1.0f;
        self->up.z = 0.0f;
    }
    yaw = -4.0f * fn_1_AB8(pad);
    pitch = -4.0f * fn_1_B0C(pad);
    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    x.x = m[0][0];
    x.y = m[0][1];
    x.z = m[0][2];
    y.x = m[1][0];
    y.y = m[1][1];
    y.z = m[1][2];
    z.x = m[2][0];
    z.y = m[2][1];
    z.z = m[2][2];
    PSMTXRotAxisRad(rotA, &x, 0.017453292f * pitch);
    PSMTXRotRad(rotB, 'Y', 0.017453292f * yaw);
    PSMTXConcat(rotA, rotB, rot);
    PSMTXMultVecSR(rot, &z, &z);
    PSMTXMultVecSR(rot, &y, &self->up);
    PSVECSubtract(&self->pos, &z, &self->target);
    fast = (btn & 0x200) != 0;
    speed = fast ? 70.0f : 20.0f;
    C_MTXLookAt(inv, &self->pos, &self->up, &self->target);
    PSMTXInverse(inv, inv);
    mv.x = (speed * (u8)((btn & 2) != 0)) - (speed * (u8)((btn & 1) != 0));
    mv.y = (speed * (u8)((btn & 8) != 0)) - (speed * (u8)((btn & 4) != 0));
    mv.x = mv.x + (speed * fn_1_B60(pad));
    mv.z = 0.0f;
    PSMTXMultVecSR(inv, &mv, &mv);
    mv.y = mv.y + (speed * fn_1_BB4(pad));
    PSVECAdd(&mv, &self->target, &self->target);
    PSVECAdd(&mv, &self->pos, &self->pos);
    fwd.x = 0.0f;
    fwd.y = 0.0f;
    fwd.z = (f32)((u8)((btn & 0x400) != 0) - (u8)((btn & 0x100) != 0));
    PSMTXMultVecSR(inv, &fwd, &fwd);
    fwd.y = 0.0f;
    len = PSVECMag(&fwd);
    if (len > 1e-10f) {
        PSVECScale(&fwd, &fwd, speed / len);
        PSVECAdd(&fwd, &self->target, &self->target);
        PSVECAdd(&fwd, &self->pos, &self->pos);
    }
    if ((HuPadBtnDown[pad] & 0x200) != 0) {
        C_MTXLookAt(dump, &self->pos, &self->up, &self->target);
        PSMTXInverse(dump, dump);
        out.pose.pos.x = self->pos.x;
        out.pose.pos.y = self->pos.y;
        out.pose.pos.z = self->pos.z;
        C_QUATMtx(&out.pose.rot, dump);
        printf("{{0x%08x,0x%08x,0x%08x},{0x%08x,0x%08x,0x%08x,0x%08x}},\n", out.w[0], out.w[1], out.w[2], out.w[3], out.w[4],
               out.w[5], out.w[6]);
    }
}

CMinigameCore::CMinigameCore()
{
    s32 i;

    if (lbl_1_bss_C == NULL) {
    } else {
        ASSERT_FAIL("ms_pointer == 0", "singleton.h", 0x35)
    }
    lbl_1_bss_C = this;
    m_unk1C = 0;
    for (i = 0; i < 4; i++) {
        m_winner[i] = -1;
    }
}

void **fn_1_1334(void **self, s16 flag)
{
    if (self != NULL) {
        if (lbl_1_bss_C != NULL) {
        } else {
            ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x3D)
        }
        lbl_1_bss_C = (CMinigameCore *)0;
        if (flag > 0 && self != NULL) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

CMinigameCore::~CMinigameCore()
{
    s32 i;

    if (mgDataTbl[GwSystem.mgNo].type != 3) {
        CMinigameCore *core;
        if (lbl_1_bss_C != NULL) {
        } else {
            ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x27)
        }
        core = lbl_1_bss_C;
        for (i = 0; i < 4; i++) {
            s32 player = core->m_winner[i];
            if (player >= 0 && _CheckFlag(0x1000F) == 0) {
                GwPlayer[player].mgCoinBonus = 10;
            }
        }
    }
    if (this != NULL) {
        if (lbl_1_bss_C != NULL) {
        } else {
            ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x3D)
        }
        lbl_1_bss_C = (CMinigameCore *)0;
    }
}

void CMinigameCore::setup(int objMax, int prio)
{
    OMOBJMAN *objMan;

    objMan = omInitObjMan(objMax, prio);
    omGameSysInit(objMan);
    on_setup(objMan);
    omAddObjEx(objMan, 0, 0, 0, -1, fn_1_1694);
}

void fn_1_162C(CMinigameCore *self)
{
    OMOBJMAN *objMan;

    objMan = MgActorObjectSetup();
    self->on_setup(objMan);
    omAddObjEx(objMan, 0, 0, 0, -1, fn_1_1694);
}

void fn_1_1694(OMOBJ *obj)
{
    if (omSysExitReq != 0 && WipeCheckEnd() == 0) {
        fn_1_1F3C();
        HuAudFadeOut(0x3E8);
        obj->objFunc = fn_1_1794;
        return;
    }
    CMinigameCore *a = lbl_1_bss_C;
    CMinigameCore *b = a;
    fn_1_1724(b);
}

void fn_1_1724(CMinigameCore *core)
{
    core->on_update();
    if (core->m_unk1C > 0) {
        s32 t = core->m_unk1C - 1;
        core->m_unk1C = t;
        if (t == 0) {
        }
    }
}

void CMinigameCore::on_update()
{
}

void fn_1_1794(OMOBJ *obj)
{
    CMinigameCore *core;

    core = lbl_1_bss_C;
    if (WipeCheckEnd() != 0) {
        fn_1_1724(core);
        return;
    }
    core->on_close();
    delete core;
    CharModelKill(-1);
    omOvlReturnEx(1, 1);
}

// Appends a player to the winner list.
void fn_1_1880(s32 player)
{
    s32 *slot;
    s32 i;

    if (lbl_1_bss_C != NULL) {
    } else {
        ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x27)
    }
    slot = lbl_1_bss_C->m_winner;
    for (i = 0; i < 4; i++) {
        if (slot[i] < 0) {
            slot[i] = player;
            return;
        }
    }
}

s32 fn_1_1954(s32 player)
{
    s32 i;

    if (lbl_1_bss_C != NULL) {
    } else {
        ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x27)
    }
    for (i = 0; i < 4; i++) {
        if (player == lbl_1_bss_C->m_winner[i]) {
            return 1;
        }
    }
    return 0;
}

void fn_1_1A28(void)
{
    CMinigameCore *core;

    if (lbl_1_bss_C != NULL) {
    } else {
        ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x27)
    }
    core = lbl_1_bss_C;
    core->m_unk18 = GameMesCreate(2, 0);
}

void fn_1_1AE0(void)
{
    CMinigameCore *core;

    if (lbl_1_bss_C != NULL) {
    } else {
        ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x27)
    }
    core = lbl_1_bss_C;
    core->m_unk18 = GameMesCreate(2, 1);
}

s32 fn_1_1B98(void)
{
    CMinigameCore *core;
    s32 ret;

    if (lbl_1_bss_C != NULL) {
    } else {
        ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x27)
    }
    core = lbl_1_bss_C;
    ret = 0;
    if (GameMesStatGet(-1) == 0 && core->m_unk1C <= 0) {
        ret = 1;
    }
    return ret;
}

// Orders the winners, then announces them (or the draw when there is none).
void fn_1_1C6C(void)
{
    CMinigameCore *core;
    s32 *w;
    s32 count;
    s32 i;
    s32 j;
    s32 k;
    s32 args[4];

    if (lbl_1_bss_C != NULL) {
    } else {
        ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x27)
    }
    lbl_1_bss_C->m_unk1C = lbl_1_bss_4;
    w = lbl_1_bss_C->m_winner;
    count = 0;
    while (count < 4 && w[count] >= 0) {
        count++;
    }
    if (count == 0) {
        GameMesCreate(2, 2);
        return;
    }
    for (i = 1; i < count; i++) {
        for (j = i; j >= 1 && w[j - 1] > w[j]; j--) {
            s32 tmp = w[j - 1];
            w[j - 1] = w[j];
            w[j] = tmp;
        }
    }
    for (k = 0; k < 4; k++) {
        s16 charNo;
        if (w[k] < 0) {
            charNo = -1;
        } else {
            charNo = GwPlayerConf[w[k]].charNo;
        }
        args[k] = charNo;
    }
    GameMesCreate(4, 3, args[0], args[1], args[2], args[3]);
}

void fn_1_1E38(s32 mes)
{
    CMinigameCore *core;

    if (_CheckFlag(0x1000F) == 0) {
        if (lbl_1_bss_C != NULL) {
        } else {
            ASSERT_FAIL("ms_pointer != 0", "singleton.h", 0x27)
        }
        core = lbl_1_bss_C;
        core->m_unk18 = GameMesCreate(0xE, mes);
        HuAudJinglePlay(0x86);
    }
}

void fn_1_1F10(void)
{
    WipeCreate(1, 5, 0x3C);
}

void fn_1_1F3C(void)
{
    WipeCreate(2, 6, 0x3C);
}

void fn_1_1F68(void)
{
    omSysExitReq = 1;
}

void *CMinigameCore::operator new(unsigned long size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    if (p != NULL) {
    } else {
        ASSERT_FAIL("ptr != NULL", "om.cpp", 0x18)
    }
    return p;
}

void *fn_1_2008(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    if (p != NULL) {
    } else {
        ASSERT_FAIL("ptr != NULL", "om.cpp", 0x1F)
    }
    return p;
}

void CMinigameCore::operator delete(void *ptr)
{
    if (ptr != NULL) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_20CC(void *ptr)
{
    if (ptr != NULL) {
        HuMemDirectFree(ptr);
    }
}

void fn_1_2104(CThunk *self, OMOBJ *obj)
{
    self->m_vt = &lbl_1_data_BC;
    obj->data = self;
    obj->objFunc = fn_1_2124;
}

void fn_1_2124(OMOBJ *obj)
{
    CThunk *thunk = (CThunk *)obj->data;

    thunk->m_vt->m_run(thunk, obj);
}

void fn_1_2170(CThunk *self, OMOBJ *obj)
{
}

void fn_1_2174(OMOBJ *obj)
{
    CThunk *thunk = (CThunk *)obj->data;

    if (thunk != NULL) {
        thunk->m_vt->m_dtor(thunk, 1);
    }
    omDelObjEx((OMOBJMAN *)HuPrcCurrentGet(), obj);
}

CThunk *fn_1_21E4(CThunk *self, s16 flag)
{
    if (self != NULL) {
        self->m_vt = &lbl_1_data_BC;
        if (flag > 0 && self != NULL) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_2244(OMOBJMAN *objMan, OMOBJ *obj)
{
    CThunk *thunk = (CThunk *)obj->data;

    if (thunk != NULL) {
        thunk->m_vt->m_dtor(thunk, 1);
    }
    omDelObjEx(objMan, obj);
}

CTimer::CTimer(int type)
{
    switch (type) {
        case 0:
            m_timer = MgTimerCreate(0);
            break;
        case 1:
            m_timer = MgTimerCreate(1);
            break;
        case 2:
            m_timer = MgTimerCreate(2);
            break;
        default:
            ASSERT_FAIL("false", "timer.cpp", 0x1E)
            break;
    }
}

CTimer *fn_1_2380(CTimer *self, s16 flag)
{
    if (self != NULL) {
        MgTimerKill(self->m_timer);
        if (flag > 0 && self != NULL) {
            HuMemDirectFree(self);
        }
    }
    return self;
}

void fn_1_23DC(CTimer *self, s32 a, s32 b, s32 c)
{
    MgTimerParamSet(self->m_timer, a, b, c);
}

s32 fn_1_2424(CTimer *self)
{
    return MgTimerValueGet(self->m_timer);
}

void fn_1_2454(CTimer *self, f32 x, f32 y)
{
    MgTimerPosSet(self->m_timer, x, y);
}

void fn_1_2494(CTimer *self, f32 *x, f32 *y)
{
    MgTimerPosGet(self->m_timer, x, y);
}

void fn_1_24D4(CTimer *self, s32 mode)
{
    switch (mode) {
        case 0:
            MgTimerModeOnSet(self->m_timer, 0);
            return;
        case 1:
            MgTimerModeOnSet(self->m_timer, 1);
            return;
        case 2:
            MgTimerModeOnSet(self->m_timer, 2);
            return;
        default:
            ASSERT_FAIL("false", "timer.cpp", 0x48)
            return;
    }
}

void fn_1_2598(CTimer *self)
{
    MgTimerModeOffSet(self->m_timer);
}

void fn_1_25C8(CTimer *self)
{
    if (self->m_timer->stopF == 0) {
        self->m_timer->stopF = 1;
        self->m_timer->mode = 1;
    }
}

void fn_1_25F4(CTimer *self)
{
    MgTimerKill(self->m_timer);
    self->m_timer = MgTimerCreate(0);
}

u8 fn_1_2630(CTimer *self)
{
    return MgTimerDoneCheck(self->m_timer) != 0;
}

void fn_1_2670(CTimer *self, u8 r, u8 g, u8 b)
{
    MgTimerWinColorSet(self->m_timer, r, g, b);
}

void fn_1_26B8(void)
{
    lbl_1_bss_0 = 0x78;
    lbl_1_bss_4 = 0xD2;
    lbl_1_bss_8 = 0xD2;
}

// Vtable of the object thunk: deleting destructor, then run().
ThunkVt lbl_1_data_BC = { { NULL, NULL }, fn_1_21E4, fn_1_2170 };
