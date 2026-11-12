#define _MATH_H /* the original object has no math.h sqrt literals */
#include "types.h"
#include "game/hu3d.h"
#include "datadir_enum.h"
#include "REL/m568Dll.h"
extern "C" {
#include "game/audio.h"
}

/*
 * tube.cpp: the physics body of one player (a capsule-like ball): it gathers
 * forces, integrates them, collides against the stage, the other bodies and
 * the octree effects, and keeps its model matrix in sync. A pointer-to-member
 * state hook picks the per-frame update.
 */

#ifdef __MWERKS__
#define TUBE_RODATA __declspec(section ".rodata")
#else
#define TUBE_RODATA
#endif

// Short-lived effect object (effect.cpp, 0x10 bytes).
struct TubeEffect {
    M568Link node;  // 0x00
    s16 modelId;    // 0x08
    s32 flags;      // 0x0C
};

extern "C" {

double sin(double x);
double cos(double x);
s32 __ptmf_test(void *ptmf);
void __ptmf_scall(void *obj);
extern M568Ptmf __ptmf_null;
void PSVECAdd(const Vec *a, const Vec *b, Vec *ab);
void PSVECSubtract(const Vec *a, const Vec *b, Vec *ab);
void PSVECCrossProduct(const Vec *a, const Vec *b, Vec *axb);
f32 PSVECDotProduct(const Vec *a, const Vec *b);
f32 PSVECMag(const Vec *v);
void PSVECNormalize(const Vec *src, Vec *dst);

void *fn_1_618(u32 size);
void *fn_1_1948(void *obj, void *list);
s32 fn_1_2548(void);
M568Model *fn_1_32AC(M568Model *self, s32 dataNum);
void fn_1_3420(M568Model *model, u32 attr);
void fn_1_3458(M568Model *model, u32 attr);
void fn_1_350C(M568Model *model, Vec *pos);
MtxPtr fn_1_36F0(M568Model *model);
void fn_1_370C(M568Model *self, s32 arg);
u8 fn_1_56DC(Vec *a, Vec *b, f32 s, f32 t);
void fn_1_C0A8(TubeEffect *self, Vec *pos, f32 scale);
void fn_1_C5F8(TubeEffect *self, Vec *pos);
void fn_1_CE20(Vec *dest);
void fn_1_CE54(Vec *out, f32 speed);
void fn_1_CED4(Vec *pos, f32 radius);
void fn_1_D0A0(Vec *out, Vec *pos, f32 radius);
void fn_1_E7C8(Vec *pos, s32 handle);
void fn_1_ED98(Quaternion *out, Vec *a, Vec *b);
// Stage collision interface (stage.cpp).
s32 fn_1_E304(void *stage, Vec *io, Vec *a, Vec *b);
Vec *fn_1_E2FC(void *stage);

extern M568PosOwner *lbl_1_bss_9F0;
extern SceneNode *lbl_1_bss_9F8;

char lbl_1_data_6A0[] = "invalid cid";
char lbl_1_data_6AC[] = "!\"invalid cid\"";
char lbl_1_data_6BB[] = "tube.cpp";
char lbl_1_data_704[] = "sm_ptr != 0";
char lbl_1_data_710[] = "a_set.h";
char lbl_1_data_74C[] = "ms_pointer != 0";
char lbl_1_data_75C[] = "singleton.h";

static void fn_1_12FD8(M568Tube *self);
static f32 fn_1_133FC(M568Tube *self, Vec *n, Vec *v);
static void fn_1_134D4(M568Tube *self);
static void fn_1_13634(M568Tube *self);
static s32 fn_1_1375C(M568Tube *self, Vec *p);
static void fn_1_13994(M568Tube *self);
static void fn_1_13B54(M568Tube *self);
static void fn_1_13DE4(M568Tube *self);
static void fn_1_13ECC(M568Tube *self, Vec *out, Vec *in);
static void fn_1_14238(M568Tube *self, M568Tube *other);
static void fn_1_143F4(M568Tube *self);
static void fn_1_14448(M568Tube *self);
void fn_1_12D10(M568Tube *self);
void fn_1_12D84(M568Tube *self);

M568Ptmf lbl_1_data_6F8 = { 0, -1, (void (*)(void *))fn_1_12D10 };
M568Ptmf lbl_1_data_768 = { 0, -1, (void (*)(void *))fn_1_12D84 };

TUBE_RODATA const s32 lbl_1_rodata_4D4[4] = { 0x903, 0x904, 0x905, 0x906 };
TUBE_RODATA const s32 lbl_1_rodata_518[4] = { 0, 0x907, 0x908, 0x909 };
TUBE_RODATA const s32 lbl_1_rodata_52C[5] = { 0x8FF, 0x900, 0x901, 0x902, 0 };

#define TUBE_CHECK_STAGE(line) \
    if (lbl_1_bss_9F0 != 0) { \
    } else { \
        fn_1_7EC(lbl_1_data_74C, lbl_1_data_75C, line); \
    }

#define TUBE_CHECK_SET() \
    if (lbl_1_bss_9F8 != 0) { \
    } else { \
        fn_1_7EC(lbl_1_data_704, lbl_1_data_710, 0x3A); \
    }

// Table index guard for the per-character sound tables.
#ifdef TARGET_PC
#define TUBE_SEIDX(i, n) (((u32)(i) < (n)) ? (i) : 0)
#else
#define TUBE_SEIDX(i, n) (i)
#endif

s32 fn_1_11F6C(u32 cid)
{
    switch (cid) {
    case 0:
        return DATANUM(DATA_m568, 7);
    case 1:
        return DATANUM(DATA_m568, 8);
    case 2:
        return DATANUM(DATA_m568, 9);
    case 3:
        return DATANUM(DATA_m568, 10);
    case 4:
        return DATANUM(DATA_m568, 11);
    case 5:
        return DATANUM(DATA_m568, 12);
    case 6:
        return DATANUM(DATA_m568, 13);
    case 7:
        return DATANUM(DATA_m568, 14);
    case 8:
        return DATANUM(DATA_m568, 15);
    case 9:
    case 10:
    case 11:
    case 12:
        return DATANUM(DATA_m568, 16);
    default:
        if (lbl_1_data_6A0 == 0) {
        } else {
            fn_1_7EC(lbl_1_data_6AC, lbl_1_data_6BB, 0x35);
        }
        return 0;
    }
}

M568Tube *fn_1_12058(M568Tube *self, s32 arg, u32 cid, s32 id)
{
    s32 dataNum;
    s32 mass;
    f32 radius;
    M568Ptmf tmp;

    fn_1_187C(self);
    self->ghost = (arg == 0);
    self->down = 0;
    self->event = 0;
    if (self->ghost != 0) {
        dataNum = DATANUM(DATA_m568, 6);
    } else {
        dataNum = fn_1_11F6C(cid);
    }
    fn_1_32AC(&self->model, dataNum);
    TUBE_CHECK_SET();
    fn_1_1948(self, lbl_1_bss_9F8);
    self->index = 0;
    self->seA = -1;
    if (self->ghost != 0) {
        self->seB = HuAudFXPlay(0x90C);
    } else {
        self->seB = -1;
    }
    if (self->ghost != 0) {
        radius = 135.0;
    } else {
        radius = 100.0;
    }
    self->radius = radius;
    if (self->ghost != 0) {
        fn_1_3420(&self->model, 0x40000001);
    } else {
        fn_1_3420(&self->model, 0x40000080);
        fn_1_3420(&self->model, 0x40000002);
    }
    Hu3DModelShadowSet(self->model.id);
    Hu3DModelShadowDispOn(self->model.id);
    Hu3DModelLayerSet(self->model.id, 0);
    Hu3DModelCameraSet(self->model.id, 1);
    if (arg == 0) {
        mass = 30;
    } else {
        mass = 2;
    }
    self->mass = (f32)mass;
    tmp = lbl_1_data_6F8;
    self->hook = tmp;
    return self;
}

M568Tube *fn_1_12384(M568Tube *self, s16 flag)
{
    if (self != 0) {
        fn_1_1888(self, 0);
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

M568Tube *fn_1_123DC(M568Tube *self, s16 flag)
{
    if (self != 0) {
        fn_1_370C(&self->model, -1);
        if (self != 0) {
            fn_1_1888(self, 0);
        }
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

void fn_1_12448(M568Tube *self, Vec *pos)
{
    Vec *p = &self->pos;

    p->x = pos->x;
    p->y = pos->y;
    p->z = pos->z;
    self->prev.x = pos->x;
    self->prev.y = pos->y;
    self->prev.z = pos->z;
    self->vel.x = 0.0f;
    self->vel.y = 0.0f;
    self->vel.z = 0.0f;
    self->force.x = 0.0f;
    self->force.y = 0.0f;
    self->force.z = 0.0f;
    self->accel.x = 0.0f;
    self->accel.y = 0.0f;
    self->accel.z = 0.0f;
    self->spin.x = 0.0f;
    self->spin.y = 0.0f;
    self->spin.z = 0.0f;
    self->up.x = 0.0f;
    self->up.y = 1.0f;
    self->up.z = 0.0f;
    self->upGoal.x = 0.0f;
    self->upGoal.y = 1.0f;
    self->upGoal.z = 0.0f;
    self->quat.x = 0.0f;
    self->quat.y = 0.0f;
    self->quat.z = 0.0f;
    self->quat.w = 1.0f;
    fn_1_13DE4(self);
}

void fn_1_1262C(M568Tube *self, Quaternion *out)
{
    out->x = self->quat.x;
    out->y = self->quat.y;
    out->z = self->quat.z;
    out->w = self->quat.w;
}

void fn_1_12650(M568Tube *self, Vec *v)
{
    PSVECAdd(v, &self->spin, &self->spin);
}

void fn_1_1268C(M568Tube *self)
{
    Vec up;
    Vec down;
    Vec hit;
    Vec n;
    Vec t;
    Vec *g;
    Vec *g0;
    Vec *up0;
    M568PosOwner *s0;
    M568PosOwner *s1;
    f32 d;
    f32 d2;

    up.x = 0.0f;
    up.y = 20.0f;
    up.z = 0.0f;
    fn_1_13ECC(self, &up, &up);
    down.x = 0.0f;
    down.y = -20.0f;
    down.z = 0.0f;
    up0 = &down;
    fn_1_13ECC(self, &down, up0);
    TUBE_CHECK_STAGE(0x27);
    s0 = lbl_1_bss_9F0;
    s1 = s0;
    if (fn_1_E304(s1, &down, &up, &down) != 0) {
        g0 = fn_1_E2FC(s1);
        g = g0;
        d = PSVECDotProduct(g, &self->vel);
        d2 = d;
        PSVECScale(g, &n, -d2);
        PSVECAdd(&self->vel, &n, &n);
        PSVECScale(&n, &t, -1.0f);
        PSVECAdd(&t, &self->force, &self->force);
    }
}

void fn_1_1284C(M568Tube *self, Vec *out)
{
    f32 r = self->radius;

    fn_1_D0A0(out, &self->pos, r);
}

void fn_1_1289C(M568Tube *self)
{
    M568Ptmf tmp;

    fn_1_3420(&self->model, 0x40000002);
    tmp = __ptmf_null;
    self->hook = tmp;
}

void fn_1_1290C(M568Tube *self)
{
    f32 y;
    f32 y2;
    f32 y3;
    f32 gx;
    f32 gy;
    f32 gz;
    f32 ux;
    f32 uy;
    f32 uz;
    f32 rx;
    f32 ry;
    f32 rx2;
    f32 ry2;

    if (fn_1_2548() != 2) {
        if (self->ghost != 0) {
            fn_1_1268C(self);
        } else {
            self->spin.z = -1.0f;
        }
    }
    if (__ptmf_test(&self->hook) != 0) {
        __ptmf_scall(self);
    }
    gx = self->upGoal.x;
    ux = self->up.x;
    rx = (f32)((0.9099999964237213 * (f64)ux) + (0.09000000357627869 * (f64)gx));
    rx2 = rx;
    self->up.x = rx2;
    gy = self->upGoal.y;
    uy = self->up.y;
    ry = (f32)((0.9099999964237213 * (f64)uy) + (0.09000000357627869 * (f64)gy));
    ry2 = ry;
    self->up.y = ry2;
    gz = self->upGoal.z;
    uz = self->up.z;
    y = (f32)((0.9099999964237213 * (f64)uz) + (0.09000000357627869 * (f64)gz));
    y2 = y;
    y3 = y2;
    self->up.z = y3;
    fn_1_13DE4(self);
    self->prev.x = self->pos.x;
    self->prev.y = self->pos.y;
    self->prev.z = self->pos.z;
    self->spin.x = 0.0f;
    self->spin.y = 0.0f;
    self->spin.z = 0.0f;
    fn_1_E7C8(&self->pos, self->seA);
    fn_1_E7C8(&self->pos, self->seB);
}

void fn_1_12D10(M568Tube *self)
{
    f32 r;

    fn_1_13634(self);
    fn_1_13994(self);
    if (self->down == 0) {
        r = self->radius;
        fn_1_CED4(&self->pos, r);
    }
    fn_1_12FD8(self);
    fn_1_13B54(self);
}

void fn_1_12D84(M568Tube *self)
{
    Vec g;
    M568Ptmf tmp;
    f32 r;

    fn_1_13634(self);
    fn_1_13994(self);
    if (self->down == 0) {
        r = self->radius;
        fn_1_CED4(&self->pos, r);
    }
    fn_1_12FD8(self);
    fn_1_13B54(self);
    fn_1_CE20(&g);
    if (PSVECDotProduct(&g, &self->vel) > 0.0f) {
        tmp = __ptmf_null;
        self->hook = tmp;
    }
}

void fn_1_12E80(M568Tube *self, Vec *n)
{
    f32 x;
    f32 ax;

    x = n->x;
    if (x < 0.0f) {
        ax = -x;
    } else {
        ax = x;
    }
    if (!(ax > sin(0.10471975511965977))) {
        self->upGoal.x = n->x;
        self->upGoal.y = n->y;
        self->upGoal.z = n->z;
    }
}

void fn_1_12F28(M568Tube *self, Vec *dir)
{
    Mtx m;
    Quaternion q;
    Vec a;
    Vec v;

    a.x = 0.0f;
    a.y = 1.0f;
    a.z = 0.0f;
    fn_1_ED98(&q, &a, dir);
    PSMTXQuat(m, &q);
    PSMTXMultVecSR(m, &self->spin, &v);
    PSVECScale(&v, &v, 10.0f);
    PSVECAdd(&v, &self->force, &self->force);
}

static void fn_1_12FD8(M568Tube *self)
{
    Vec a6C;
    Vec t54;
    Vec t60;
    Vec hit;
    Vec n3C;
    Vec n30;
    Vec *np;
    TubeEffect *e;
    f32 r;

    fn_1_134D4(self);
    TUBE_CHECK_STAGE(0x27);
    a6C.x = self->prev.x;
    a6C.y = self->prev.y;
    a6C.z = self->prev.z;
    a6C.y = a6C.y + 200.0f;
    if (fn_1_E304(lbl_1_bss_9F0, &self->pos, &a6C, &self->pos) == 0) {
        fn_1_14448(self);
        r = self->radius;
        t54.x = 0.0f;
        t54.y = 100.0f;
        t54.z = r;
        fn_1_13ECC(self, &t60, &t54);
        t60.x = 0.0f;
        t60.y = 0.0f;
        t60.z = r;
        fn_1_13ECC(self, &t54, &t60);
        if (fn_1_E304(lbl_1_bss_9F0, &hit, &t60, &t54) == 0) {
            np = fn_1_E2FC(lbl_1_bss_9F0);
            n3C.x = np->x;
            n3C.y = np->y;
            n3C.z = np->z;
            fn_1_12E80(self, &n3C);
        }
        return;
    }
    np = fn_1_E2FC(lbl_1_bss_9F0);
    n30.x = np->x;
    n30.y = np->y;
    n30.z = np->z;
    if (fn_1_133FC(self, &n30, &self->vel) < -8.0f) {
        e = (TubeEffect *)fn_1_618(sizeof(TubeEffect));
        if (e != 0) {
            fn_1_C5F8(e, &self->pos);
        }
        self->event = 2;
        HuAudFXPlay(lbl_1_rodata_4D4[TUBE_SEIDX(self->index, 4)]);
        fn_1_143F4(self);
    }
    e = (TubeEffect *)fn_1_618(sizeof(TubeEffect));
    if (e != 0) {
        fn_1_C0A8(e, &self->pos, 100.0f);
    }
    fn_1_12E80(self, &n30);
    fn_1_12F28(self, &n30);
    fn_1_13994(self);
}

static f32 fn_1_133FC(M568Tube *self, Vec *n, Vec *v)
{
    Vec t;
    Vec *p;
    Vec *q;
    f32 d;
    f32 s;

    d = PSVECDotProduct(v, n);
    if (d < 0.0f) {
        s = self->mass * -d;
        PSVECScale(n, &t, s);
        p = &t;
        q = p;
        PSVECAdd(q, &self->force, &self->force);
    }
    return d;
}

static void fn_1_134D4(M568Tube *self)
{
    Vec v;
    f32 r;
    f32 rad;
    f32 ang;
    f64 a;
    f64 cx;
    s32 n;
    s32 i;

    n = 4;
    for (i = 0; i < n; i++) {
        rad = self->radius;
        r = 1.2f * rad;
        a = 6.283185307179586 * (f64)i;
        ang = (f32)(a / (f64)n);
        cx = (f64)r * cos(ang);
        v.x = (f32)((f64)r * sin(ang));
        v.y = 200.0f;
        v.z = (f32)cx;
        fn_1_1375C(self, &v);
    }
    fn_1_13994(self);
}

static void fn_1_13634(M568Tube *self)
{
    Vec g;
    Vec *p;
    Vec *q;
    f32 m;

    TUBE_CHECK_STAGE(0x27);
    m = self->mass;
    fn_1_CE54(&g, m);
    p = &g;
    q = p;
    PSVECAdd(q, &self->force, &self->force);
}

void fn_1_136CC(M568Tube *self, f32 k)
{
    Vec t;
    Vec *p;
    Vec *q;
    f32 s;

    s = -0.8f * self->mass * k;
    PSVECScale(&self->vel, &t, s);
    p = &t;
    q = p;
    PSVECAdd(q, &self->force, &self->force);
}

static s32 fn_1_1375C(M568Tube *self, Vec *p)
{
    Vec a;
    Vec b;
    Vec hit;
    Vec d;
    Vec dir;
    Vec par;
    Vec *n;
    Vec *n0;
    Vec *ap;
    M568PosOwner *s0;
    M568PosOwner *s1;
    f32 py;
    f32 dt;
    f32 dt2;

    a.x = 0.0f;
    py = p->y;
    a.y = py;
    a.z = 0.0f;
    ap = &a;
    fn_1_13ECC(self, &a, ap);
    fn_1_13ECC(self, &b, p);
    TUBE_CHECK_STAGE(0x27);
    s0 = lbl_1_bss_9F0;
    s1 = s0;
    if (fn_1_E304(s1, &hit, &a, &b) == 0) {
        return 0;
    }
    PSVECSubtract(&hit, &b, &d);
    PSVECAdd(&d, &self->pos, &self->pos);
    PSVECSubtract(&b, &a, &dir);
    PSVECNormalize(&dir, &dir);
    dt = PSVECDotProduct(&dir, &self->vel);
    dt2 = dt;
    PSVECScale(&dir, &par, dt2);
    n0 = fn_1_E2FC(s1);
    n = n0;
    fn_1_133FC(self, n, &par);
    return 1;
}

static void fn_1_13994(M568Tube *self)
{
    Vec t;

    PSVECScale(&self->force, &t, 1.0f / self->mass);
    self->force.x = 0.0f;
    self->force.y = 0.0f;
    self->force.z = 0.0f;
    PSVECAdd(&t, &self->accel, &self->accel);
}

void fn_1_13A30(Vec *out, Vec *v)
{
    Vec g;
    f32 d;
    f32 d2;

    fn_1_CE20(&g);
    d = PSVECDotProduct(&g, v);
    d2 = d;
    PSVECScale(&g, out, d2);
}

void fn_1_13A90(M568Tube *self, f32 limit)
{
    Vec par;
    Vec perp;
    Vec *v;
    f32 lim;
    f32 mag;
    f32 mag2;

    lim = limit;
    v = &self->vel;
    fn_1_13A30(&par, v);
    PSVECSubtract(v, &par, &perp);
    mag = PSVECMag(&perp);
    mag2 = mag;
    if (!(mag2 < lim)) {
        PSVECNormalize(&perp, &perp);
        PSVECScale(&perp, v, lim);
        PSVECAdd(&par, v, v);
    }
}

static void fn_1_13B54(M568Tube *self)
{
    Vec old;

    old.x = self->vel.x;
    old.y = self->vel.y;
    old.z = self->vel.z;
    PSVECAdd(&self->accel, &old, &self->vel);
    fn_1_13A90(self, 25.0f);
    self->accel.x = 0.0f;
    self->accel.y = 0.0f;
    self->accel.z = 0.0f;
    PSVECAdd(&self->vel, &old, &old);
    fn_1_13A90(self, 25.0f);
    PSVECScale(&old, &old, 0.5f);
    PSVECAdd(&old, &self->pos, &self->pos);
}

static void fn_1_13DE4(M568Tube *self)
{
    Vec a;
    MtxPtr m;

    a.x = 0.0f;
    a.y = 1.0f;
    a.z = 0.0f;
    fn_1_ED98(&self->quat, &a, &self->up);
    PSMTXQuat(self->mtx, &self->quat);
    PSMTXTransApply(self->mtx, self->mtx, self->pos.x, self->pos.y, self->pos.z);
    m = fn_1_36F0(&self->model);
    PSMTXCopy(self->mtx, m);
    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
    fn_1_350C(&self->model, &self->pos);
}

static void fn_1_13ECC(M568Tube *self, Vec *out, Vec *in)
{
    PSMTXMultVec(self->mtx, in, out);
}

void fn_1_13F08(M568Tube *self, M568Tube *other)
{
    Vec d;
    Vec t;
    f32 sum;
    f32 dot;
    f32 dot2;

    sum = self->mass + other->mass;
    if (fn_1_56DC(&self->pos, &other->pos, (sum - self->mass) / sum, 1.001f) != 0) {
        if (self->ghost != 0 || other->ghost != 0) {
            fn_1_14238(self, other);
            fn_1_14238(other, self);
            return;
        }
        PSVECSubtract(&other->pos, &self->pos, &d);
        PSVECNormalize(&d, &d);
        dot = PSVECDotProduct(&self->vel, &d);
        if (dot > 0.0f) {
            PSVECScale(&d, &t, 0.8f * -dot);
            PSVECAdd(&t, &self->vel, &self->vel);
        }
        dot2 = -PSVECDotProduct(&other->vel, &d);
        if (dot2 > 0.0f) {
            PSVECScale(&d, &t, 0.8f * dot2);
            PSVECAdd(&t, &other->vel, &other->vel);
        }
    }
}

void fn_1_140D8(M568Tube *self, M568OctEffect *obj)
{
    Vec d;
    Vec *p;
    Vec *q;
    Vec *p0;
    Vec *dp;
    Vec *dq;
    f32 dot;
    f32 dot2;
    f32 sc;

    if (obj->played == 0) {
        p = &self->pos;
        p0 = &obj->pos;
        q = p0;
        fn_1_56DC(p, q, 1.0f, 1.0f);
        self->event = 8;
        if (self->ghost != 0) {
            PSVECSubtract(p, q, &d);
            PSVECNormalize(&d, &d);
            dot = PSVECDotProduct(&self->vel, &d);
            dot2 = dot;
            sc = 0.2f * (-dot2 * self->mass);
            PSVECScale(&d, &d, sc);
            dp = &d;
            dq = dp;
            PSVECAdd(dq, &self->force, &self->force);
        }
    }
}

s32 fn_1_14208(M568Tube *self)
{
    s32 r;

    r = self->event != 0;
    self->event = 0;
    return r;
}

static void fn_1_14238(M568Tube *self, M568Tube *other)
{
    Vec g;
    Vec axis;
    Vec c;
    M568Ptmf tmp;

    if (fn_1_2548() == 2 && (s32)(self->ghost | self->down) == 0) {
        fn_1_3458(&self->model, 0x40000080);
        fn_1_3458(&self->model, 0x40000002);
        self->down = 1;
        self->event = 2;
        HuAudFXPlay(lbl_1_rodata_518[TUBE_SEIDX(self->index, 4)]);
        fn_1_CE20(&g);
        self->vel.x = 0.0f;
        self->vel.y = 0.0f;
        self->vel.z = 0.0f;
        PSVECScale(&g, &self->force, -40.0f * self->mass);
        axis.x = -1.0f;
        axis.y = 0.0f;
        axis.z = 0.0f;
        PSVECCrossProduct(&g, &axis, &c);
        PSVECScale(&c, &c, -20.0f * self->mass);
        PSVECAdd(&c, &self->force, &self->force);
        tmp = lbl_1_data_768;
        self->hook = tmp;
    }
}

static void fn_1_143F4(M568Tube *self)
{
    s32 i;

    if (self->seA == -1) {
        i = self->index;
        self->seA = HuAudFXPlay(lbl_1_rodata_52C[TUBE_SEIDX(i, 5)]);
    }
}

static void fn_1_14448(M568Tube *self)
{
    if (self->seA != -1) {
        HuAudFXStop(self->seA);
        self->seA = -1;
    }
}

}
