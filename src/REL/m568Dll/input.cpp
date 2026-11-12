// m568Dll input unit: the pad-input and AI-input sub-objects.
//
// Four vtables live in .data, each {0, 0, v0, v1}; MWCC emits the two leading
// null words before the virtual entries:
//   lbl_1_data_268 -> fn_1_7DD8 / fn_1_76BC   (AI: runs at the nearest actor)
//   lbl_1_data_278 -> fn_1_7E44 / fn_1_69E0   (AI: dodges effects, then chases)
//   lbl_1_data_288 -> fn_1_7EB0 / fn_1_6384   (pad)
//   lbl_1_data_298 -> fn_1_632C / 0           (the common base)
// A derived object's constructor installs the shared base vtable first and
// then overwrites it with its own, so both stores land on offset 0x00.

#include "REL/m568Dll.h"
extern "C" {
#include "game/gamework.h"
#include "game/pad.h"
}

struct InputObj;

// What an input writes each frame: two button flags and the stick axis.
struct InputOut {
    u8 b0;     // 0x00
    u8 b1;     // 0x01
    u8 pad02[2];
    f32 axis;  // 0x04
};

struct InputVtbl {
    void *rtti[2];
    InputObj *(*dtor)(InputObj *self, s16 flag);
    void (*update)(InputObj *self, InputOut *out, SceneNode *actor);
};

struct InputObj {
    InputVtbl *vtbl;  // 0x00
    InputOut out;     // 0x04
    f32 skill;        // 0x0C
};

struct PadInput : InputObj {
    s32 playerNo;  // 0x10
    s32 padNo;     // 0x14
};

struct AiInput : InputObj {
    SceneNode *target;  // 0x10
    u32 tick;           // 0x14
    s32 timerA;         // 0x18
    s32 timerB;         // 0x1C
};

// Per-player bit masks, one byte each, read with a dynamic index.
const u8 lbl_1_rodata_C8[8] = { 0x01, 0x09, 0x08, 0x0A, 0x02, 0x06, 0x04, 0x05 };

extern "C" {

double atan2(double y, double x);
f32 fn_1_55E0(f32 from, f32 to);
void fn_1_D0A0(Vec *out, Vec *pos, f32 dirY);
void fn_1_6E0(void *self);
void fn_1_7830(AiInput *self);
InputObj *fn_1_7DD8(InputObj *self, s16 flag);
void fn_1_76BC(AiInput *self, InputOut *out, SceneNode *actor);
InputObj *fn_1_7E44(InputObj *self, s16 flag);
void fn_1_69E0(InputObj *self, InputOut *out, SceneNode *actor);
InputObj *fn_1_7EB0(InputObj *self, s16 flag);
void fn_1_6384(PadInput *self, InputOut *out, SceneNode *actor);
InputObj *fn_1_632C(InputObj *self, s16 arg);
SceneNode *fn_1_65C4(void);
s32 fn_1_6678(Vec *v);
s32 fn_1_6704(f32 dx, f32 dz);
u8 fn_1_6E94(InputObj *self, InputOut *out, SceneNode *actor);
void fn_1_7110(InputObj *self, InputOut *out, SceneNode *actor, SceneNode *target);
SceneNode *fn_1_7998(AiInput *self, Vec *pos);
SceneNode *fn_1_7C84(AiInput *self, Vec *pos);

char lbl_1_data_250[] = "sm_ptr != 0";
char lbl_1_data_25C[] = "a_set.h";
s32 lbl_1_data_264 = 800;

InputVtbl lbl_1_data_268 = { { 0, 0 }, fn_1_7DD8, (void (*)(InputObj *, InputOut *, SceneNode *))fn_1_76BC };
InputVtbl lbl_1_data_278 = { { 0, 0 }, fn_1_7E44, fn_1_69E0 };
InputVtbl lbl_1_data_288 = { { 0, 0 }, fn_1_7EB0, (void (*)(InputObj *, InputOut *, SceneNode *))fn_1_6384 };
InputVtbl lbl_1_data_298 = { { 0, 0 }, fn_1_632C, 0 };

M568PosOwner *lbl_1_bss_9F0;
M568Effect *lbl_1_bss_9F4;
SceneNode *lbl_1_bss_9F8;
s32 lbl_1_bss_9FC;

// Singleton accessor assertion shared by every list walk.
#define INPUT_ASSERT_LIST(p) \
    if ((p) != 0) { \
    } else { \
        fn_1_7EC(lbl_1_data_250, lbl_1_data_25C, 0x3A); \
    }

// Intrusive-list iteration helpers (a_set.h); the original inlines each of
// them, which is where the register-to-register copies come from.
template <class T>
inline T *ListEnd(T *head)
{
    return head;
}

template <class T>
inline T *ListBegin(T *head)
{
    return (T *)head->link.next;
}

template <class T>
inline T *ListNext(T *node)
{
    return (T *)node->link.next;
}

inline SceneNode *ActorSet()
{
    INPUT_ASSERT_LIST(lbl_1_bss_9F8)
    return lbl_1_bss_9F8;
}

inline M568Effect *EffectSet()
{
    INPUT_ASSERT_LIST(lbl_1_bss_9F4)
    return lbl_1_bss_9F4;
}

inline u8 IsInactive(SceneNode *n)
{
    return n->inactive;
}

inline u8 IsDead(SceneNode *n)
{
    return n->dead;
}

inline u8 IsPlayed(M568Effect *e)
{
    return e->played;
}

inline Vec *EffectPos(M568Effect *e)
{
    return &e->pos;
}

inline Vec *PosOf(SceneNode *n)
{
    return &n->pos;
}

inline f32 SqDist(Vec *a, Vec *b)
{
    return PSVECSquareDistance(a, b);
}

inline f32 FMul(f32 a, f32 b)
{
    return a * b;
}

// Run the input once: clear the outputs, then let the virtual fill them in.
void fn_1_628C(InputObj *self, SceneNode *actor)
{
    self->out.b0 = 0;
    self->out.b1 = 0;
    self->out.axis = 0.0f;
    self->vtbl->update(self, &self->out, actor);
}

void fn_1_62F4(PadInput *self, s32 playerNo)
{
    self->vtbl = &lbl_1_data_298;
    self->vtbl = &lbl_1_data_288;
    self->playerNo = playerNo;
    self->padNo = GwPlayerConf[playerNo].padNo;
}

InputObj *fn_1_632C(InputObj *self, s16 arg)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_298;
        if (arg > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

void fn_1_6384(PadInput *self, InputOut *out, SceneNode *actor)
{
    u32 btn;

    out->axis = 0.017857144f * HuPadStkX[self->padNo];
    btn = HuPadBtn[self->padNo];
    out->b1 = (btn & 0x100) != 0;
    out->b0 = (btn & 0x200) != 0;
}

void fn_1_6428(InputObj *self)
{
    self->vtbl = &lbl_1_data_298;
    self->vtbl = &lbl_1_data_278;
}

// Is an untriggered effect close ahead of `pos`? If so `out` is its position.
s32 fn_1_6444(Vec *out, Vec *pos)
{
    M568Effect *e;
    M568Effect *end;
    M568Effect *set;
    f32 dz;
    f32 dx;
    f32 adx;

    set = EffectSet();
    end = ListEnd(set);
    e = ListBegin(set);
    while (e != end) {
        if (IsPlayed(e) == 0) {
            *out = e->pos;
            dz = pos->z - out->z;
            if (dz < 500.0f && dz > -200.0f) {
                dx = pos->x - out->x;
                if (dx < 0.0f) {
                    adx = -dx;
                } else {
                    adx = dx;
                }
                if (adx < 200.0f) {
                    return 1;
                }
            }
        }
        e = ListNext(e);
    }
    return 0;
}

// First actor that is flagged inactive, or 0.
SceneNode *fn_1_65C4(void)
{
    SceneNode *n;
    SceneNode *end;
    SceneNode *set;

    set = ActorSet();
    end = ListEnd(set);
    n = ListBegin(set);
    while (n != end) {
        if (IsInactive(n) != 0) {
            return n;
        }
        n = ListNext(n);
    }
    return 0;
}

// Bit set of the cardinal directions a facing vector points at.
s32 fn_1_6678(Vec *v)
{
    s32 mask;

    mask = 0;
    if (v->z < -0.5f) {
        mask |= 1;
    }
    if (v->z > 0.5f) {
        mask |= 2;
    }
    if (v->x < -0.5f) {
        mask |= 4;
    }
    if (v->x > 0.5f) {
        mask |= 8;
    }
    return mask;
}

// Octant (0-7) of the bearing (dx, dz).
s32 fn_1_6704(f32 dx, f32 dz)
{
    f64 ang;

    ang = atan2(dx, -dz);
    if (ang < 0.0) {
        ang += 6.283185307179586;
    }
    ang += 0.39269908169872414;
    return (s32)((8.0 * ang) / 6.283185307179586) & 7;
}

bool fn_1_67BC(u32 mask, s32 playerNo)
{
    playerNo &= 7;
    return (lbl_1_rodata_C8[playerNo] & mask) != 0;
}

// Pick the direction bit set (from the mask table) nearest the bearing.
u8 fn_1_67E4(s32 mask, f32 dx, f32 dz)
{
    s32 oct;
    s32 i;
    s32 k;

    oct = fn_1_6704(dx, dz);
    i = oct + 4;
    if ((lbl_1_rodata_C8[i & 7] & mask) != 0) {
        k = 0;
        while (k < 3) {
            i = (oct + 4) - k;
            if ((lbl_1_rodata_C8[i & 7] & mask) != 0) {
                i = oct + k + 4;
                if ((lbl_1_rodata_C8[i & 7] & mask) != 0) {
                    k++;
                    continue;
                }
            }
            break;
        }
    }
    return lbl_1_rodata_C8[i & 7];
}

// Steer by a direction vector: button by its z sign, stick by its x.
void fn_1_6954(Vec *v, InputOut *out)
{
    s32 dir;

    if (v->z < 0.0f) {
        out->b1 = 1;
    } else {
        out->b0 = 1;
    }
    if (v->x < 0.0f) {
        dir = -1;
    } else {
        dir = 1;
    }
    out->axis = (f32)dir;
}

// AI update: dodge effects, else head for the first inactive actor.
void fn_1_69E0(InputObj *self, InputOut *out, SceneNode *actor)
{
    Vec facing;
    Vec *pos;
    SceneNode *target;
    f32 dx;
    f32 dz;
    s32 mask;
    s32 sel;
    s32 oct;
    s32 i;
    s32 k;

    pos = &actor->pos;
    if (fn_1_6E94(self, out, actor) == 0) {
        target = fn_1_65C4();
        if (target != 0) {
            fn_1_D0A0(&facing, pos, actor->radius);
            dz = target->pos.z - pos->z;
            dx = target->pos.x - pos->x;
            if ((dz * dz + dx * dx) < (f32)(lbl_1_data_264 * lbl_1_data_264)) {
                mask = fn_1_6678(&facing);
                oct = fn_1_6704(dx, dz);
                i = oct + 4;
                if ((lbl_1_rodata_C8[i & 7] & mask) != 0) {
                    k = 0;
                    while (k < 3) {
                        i = (oct + 4) - k;
                        if ((lbl_1_rodata_C8[i & 7] & mask) != 0) {
                            i = oct + k + 4;
                            if ((lbl_1_rodata_C8[i & 7] & mask) != 0) {
                                k++;
                                continue;
                            }
                        }
                        break;
                    }
                }
                sel = lbl_1_rodata_C8[i & 7];
                if ((sel & 1) != 0) {
                    out->b1 = 1;
                }
                if ((sel & 2) != 0) {
                    out->b0 = 1;
                }
                if ((sel & 4) != 0) {
                    out->axis = -1.0f;
                }
                if ((sel & 8) != 0) {
                    out->axis = 1.0f;
                }
                if (sel == 0) {
                    fn_1_7110(self, out, actor, target);
                }
            } else {
                fn_1_7110(self, out, actor, target);
            }
        }
    }
}

// Avoid an incoming effect: step aside toward the open side.
u8 fn_1_6E94(InputObj *self, InputOut *out, SceneNode *actor)
{
    M568Effect *e;
    M568Effect *end;
    M568Effect *set;
    Vec ep;
    f32 dz;
    f32 dx;
    f32 adx;
    u8 found;

    set = EffectSet();
    end = ListEnd(set);
    e = ListBegin(set);
    while (1) {
        if (e == end) {
            found = 0;
            break;
        }
        if (IsPlayed(e) == 0) {
            ep = *EffectPos(e);
            dz = PosOf(actor)->z - ep.z;
            if (dz < 500.0f && dz > -200.0f) {
                dx = PosOf(actor)->x - ep.x;
                if (dx < 0.0f) {
                    adx = -dx;
                } else {
                    adx = dx;
                }
                if (adx < 200.0f) {
                    found = 1;
                    break;
                }
            }
        }
        e = ListNext(e);
    }
    if (found == 0) {
        return 0;
    }
    if (ep.x < -400.0f) {
        out->axis = 1.0f;
        return 1;
    }
    if (ep.x > 400.0f) {
        out->axis = -1.0f;
        return 1;
    }
    if (ep.x < PosOf(actor)->x) {
        out->axis = 1.0f;
    } else {
        out->axis = -1.0f;
    }
    return 1;
}

// Chase `target`: stick toward it, buttons by facing.
void fn_1_7110(InputObj *self, InputOut *out, SceneNode *actor, SceneNode *target)
{
    Vec facing;
    Vec *pos;
    Vec *tpos;
    f32 skill;
    f32 d;
    f32 ad;
    f32 v;
    f32 dz;
    f32 adz;
    f32 dx;
    f32 adx;
    s32 dir;

    pos = &actor->pos;
    fn_1_D0A0(&facing, pos, actor->radius);
    tpos = &target->pos;
    skill = self->skill;
    d = actor->pos.x - target->pos.x;
    if (d < 0.0f) {
        ad = -d;
    } else {
        ad = d;
    }
    if (ad < (450.0f + (100.0f * skill))) {
        v = actor->pos.x - target->pos.x;
        if (v < -1.0f) {
            v = -1.0f;
        } else if (v > 1.0f) {
            v = 1.0f;
        }
        out->axis = v;
    }
    if (facing.z > -0.5f && facing.z < 0.5f) {
        if ((tpos->z - pos->z) > 0.0f) {
            out->b1 = 1;
        } else {
            out->b0 = 1;
        }
    } else {
        dz = tpos->z - pos->z;
        if (dz < 0.0f) {
            adz = -dz;
        } else {
            adz = dz;
        }
        if (adz < 200.0f) {
            if (facing.z < 0.0f) {
                out->b0 = 1;
            } else {
                out->b1 = 1;
            }
        }
    }
    if (facing.x > -0.5f && facing.x < 0.5f) {
        if ((target->pos.x - actor->pos.x) > 0.0f) {
            dir = -1;
        } else {
            dir = 1;
        }
        out->axis = (f32)dir;
        return;
    }
    dx = target->pos.x - actor->pos.x;
    if (dx < 0.0f) {
        adx = -dx;
    } else {
        adx = dx;
    }
    if (adx < 200.0f) {
        if (facing.x > 0.0f) {
            dir = -1;
        } else {
            dir = 1;
        }
        out->axis = (f32)dir;
    }
}

// Steer relative to the sideways vector between two actors.
void fn_1_7500(InputObj *self, InputOut *out, SceneNode *a, SceneNode *b)
{
    Vec rel;
    Vec vel;
    Vec up;
    Vec cross;
    s32 dir;

    PSVECSubtract(&a->pos, &b->pos, &rel);
    rel.y = 0.0f;
    vel = b->vel;
    vel.y = 0.0f;
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    out->b1 = 1;
    PSVECCrossProduct(&rel, &up, &cross);
    if (cross.z < 0.0f) {
        out->b1 = 1;
    } else {
        out->b0 = 1;
    }
    if (cross.x < 0.0f) {
        dir = -1;
    } else {
        dir = 1;
    }
    out->axis = (f32)dir;
}

AiInput *fn_1_7658(AiInput *self)
{
    self->vtbl = &lbl_1_data_298;
    self->vtbl = &lbl_1_data_268;
    self->timerA = 0;
    self->timerB = 0;
    self->target = 0;
    fn_1_7830(self);
    return self;
}

// AI update: run toward the actor it picked.
void fn_1_76BC(AiInput *self, InputOut *out, SceneNode *actor)
{
    Vec facing;
    Vec *pos;
    SceneNode *t;
    f32 dx;
    f32 v;

    pos = &actor->pos;
    t = fn_1_7998(self, pos);
    if (t != 0) {
        fn_1_D0A0(&facing, pos, actor->radius);
        dx = t->pos.x - actor->pos.x;
        if (dx < -1.0f) {
            v = -1.0f;
        } else if (dx > 1.0f) {
            v = 1.0f;
        } else {
            v = dx;
        }
        out->axis = v;
        if (facing.z > -0.8 && facing.z < 0.8) {
            if ((t->pos.z - pos->z) < 0.0f) {
                out->b1 = 1;
                return;
            }
            out->b0 = 1;
        }
    }
}

// Re-roll the two AI timers from the skill level.
void fn_1_7830(AiInput *self)
{
    self->timerA = (s32)(30.0f + FMul(60.0f, FMul(self->skill, fn_1_55E0(4.0f, 10.0f))));
    self->timerB = (s32)FMul(60.0f, FMul(1.0f - self->skill, fn_1_55E0(2.0f, 4.0f)));
}

// Pick the actor to run at: keep the current one until a timer fires.
SceneNode *fn_1_7998(AiInput *self, Vec *pos)
{
    SceneNode *n;
    SceneNode *end;
    SceneNode *set;
    s32 idx;
    u8 fired1;
    u8 fired2;

    if (self->target == 0 || (self->target != 0 && IsDead(self->target) != 0)) {
        self->target = fn_1_7C84(self, pos);
        return self->target;
    }
    fired1 = 0;
    if (self->timerA > 0) {
        self->timerA = self->timerA - 1;
        if (self->timerA == 0) {
            fired1 = 1;
        }
    }
    if (fired1 != 0) {
        fn_1_7830(self);
    }
    fired2 = 0;
    if (self->timerB > 0) {
        self->timerB = self->timerB - 1;
        if (self->timerB == 0) {
            fired2 = 1;
        }
    }
    if (fired2 != 0) {
        self->target = fn_1_7C84(self, pos);
        return self->target;
    }
    if (self->timerB > 0) {
        set = ActorSet();
        end = ListEnd(set);
        n = ListBegin(set);
        self->tick = self->tick + 1;
        idx = (self->tick >> 8) & 3;
        while (n != end) {
            idx -= 1;
            if (idx < 0 && IsInactive(n) == 0 && IsDead(n) == 0) {
                return n;
            }
            n = ListNext(n);
        }
    }
    return self->target;
}

// Nearest active actor to `pos`.
SceneNode *fn_1_7C84(AiInput *self, Vec *pos)
{
    SceneNode *n;
    SceneNode *end;
    SceneNode *best;
    SceneNode *set;
    f32 bestD;
    f32 d;

    set = ActorSet();
    end = ListEnd(set);
    n = ListBegin(set);
    best = 0;
    bestD = -1.0f;
    while (n != end) {
        if (IsInactive(n) == 0 && IsDead(n) == 0) {
            d = SqDist(PosOf(n), pos);
            if (bestD > d || bestD < 0.0f) {
                bestD = d;
                best = n;
            }
        }
        n = ListNext(n);
    }
    return best;
}

InputObj *fn_1_7DD8(InputObj *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_268;
        if (self != 0) {
            self->vtbl = &lbl_1_data_298;
        }
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

InputObj *fn_1_7E44(InputObj *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_278;
        if (self != 0) {
            self->vtbl = &lbl_1_data_298;
        }
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

InputObj *fn_1_7EB0(InputObj *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_288;
        if (self != 0) {
            self->vtbl = &lbl_1_data_298;
        }
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

}
