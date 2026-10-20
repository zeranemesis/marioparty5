// m503Dll/stage - stage setup and per-frame drive for the mini-game.
#include "types.h"
#include "game/object.h"
#include "game/sprite.h"
#include <math.h>
#include <string.h>

typedef s16 HU3DMODELID;
typedef s16 HU3DMOTID;

/* The engine indexes Hu3DData with the real 0x134 stride, which the shared
 * header model does not describe, so the m503 view of an entry is spelled out
 * here: the stage code only ever reaches hookData at 0x120. */
typedef struct {
    u8 unk000[0x120];
    void *hookData;
    u8 unk124[0x10];
} M503MODEL;

/* One grid cell of the stage field: a position plus the packed tint. */
typedef struct {
    f32 x;
    f32 y;
    f32 z;
    u8 col[4];
} M503CELL;

/* Actor descriptor handed to the attractor table; layout matches ACTOR. */
typedef struct {
    s32 flags;
    s32 unk04;
    s32 unk08;
    s32 state;
    OMOBJ *owner;
    void (*hook)(void *);
    void *tab;
    f32 scale;
    s32 mode;
} M503ACT;

/* Spawn row of the stage table: 0x14 bytes, y is what the actor copies. */
typedef struct {
    f32 x;
    s32 pad;
    f32 y;
    s32 flag;
} M503TAB;

extern const f32 lbl_1_rodata_450;
extern const f32 lbl_1_rodata_454;
extern const f64 lbl_1_rodata_458;
extern const f64 lbl_1_rodata_460;
extern const f64 lbl_1_rodata_468;
extern const f64 lbl_1_rodata_470;
extern const f64 lbl_1_rodata_478;
extern const f64 lbl_1_rodata_480;
extern const f64 lbl_1_rodata_4B8;
extern const f32 lbl_1_rodata_488;
extern const f64 lbl_1_rodata_518;
extern const f64 lbl_1_rodata_520;
extern const f64 lbl_1_rodata_550;
extern const f64 lbl_1_rodata_558;
extern const f32 lbl_1_rodata_560;
extern const f64 lbl_1_rodata_570;
extern const f64 lbl_1_rodata_578;
extern const f32 lbl_1_rodata_580;
extern const f32 lbl_1_rodata_584;
extern const f64 lbl_1_rodata_588;
extern const f32 lbl_1_rodata_590;
extern const f64 lbl_1_rodata_598;

extern const f32 lbl_1_data_584[];
extern const f32 lbl_1_data_5D8[];
extern const M503TAB lbl_1_data_520[];

extern s8 lbl_1_data_62C[];
extern OMOBJ *lbl_1_bss_B8;

void fn_1_A790(OMOBJ *obj);
void fn_1_A928(OMOBJ *obj);
void fn_1_AA0C(OMOBJ *obj);
void fn_1_AD34(OMOBJ *obj);
void fn_1_AE8C(OMOBJ *obj);
void fn_1_AF20(s8 arg0, s32 arg1);
void fn_1_B278(M503MODEL *mdl, f32 scale);
void fn_1_B888(OMOBJ *obj);
void fn_1_B9D0(HU3DMODELID id);
void fn_1_BA9C(M503MODEL *mdl, Mtx *mtx);
f32 fn_1_BFE8(f32 x, f32 z);
f32 fn_1_C18C(f32 x, f32 z);
void fn_1_C424(f32 x, f32 y, f32 z);

s32 fn_1_DFA8(void *actor);
void fn_1_CEB0(Vec *v);
void fn_1_CE1C(Vec *v);
s32 frand(void);

/* Not in the shared header; without these MWCC assumes int returns and
 * promotes the float arguments. */
BOOL Hu3DMotionEndCheck(HU3DMOTID motId);
float Hu3DMotionMaxTimeGet(HU3DMOTID motId);
void Hu3DMotionStartEndSet(HU3DMOTID motId, float start, float end);
void Hu3DMotionTimeSet(HU3DMOTID motId, float time);

// NONMATCHING: the heap id 0x1000 arrives as lis r5 rather than li r5, i.e. the
// original materialises it from something other than an inline literal.
// Spawns the stage driver object, allocates its 0xB4-byte work area and clears it.
void fn_1_A6FC(OMOBJMAN *arg0)
{
    OMOBJ *obj;
    void *work;

    obj = omAddObjEx(arg0, 0x14, 0xB, 0, -1, fn_1_A790);
    lbl_1_bss_B8 = obj;
    work = HuMemDirectMallocNum(0, 0xB4, 0x1000);
    *(s32 *)((u8 *)obj + 0x5C) = (s32)work;
    memset(work, 0, 0xB4);
    *(s32 *)((u8 *)obj + 0x4C) = 0;
}

void fn_1_A78C(void)
{
}

// NONMATCHING: three call sites materialise the heap id 0x1000 (and one file id
// 0x3D00) with lis instead of the li MWCC always picks for a literal that fits
// in a signed 16-bit immediate.
// One-time stage build: makes the three board models, the shared hook model,
// then registers the five attractor actors.
void fn_1_A790(OMOBJ *obj)
{
    s32 i;
    HU3DMODELID id;
    M503ACT *act;
    M503TAB *tab;
    u8 *work;

    work = (u8 *)obj->data;
    omSetTra(obj, lbl_1_rodata_450, lbl_1_rodata_450, lbl_1_rodata_450);
    id = Hu3DModelCreate(HuDataSelHeapReadNum(0x3D00, 0x1000, 2));
    obj->mdlId[0] = id;
    Hu3DModelLayerSet(id, 0);
    id = Hu3DModelCreate(HuDataSelHeapReadNum(0x3D01, 0x1000, 2));
    obj->mdlId[1] = id;
    Hu3DModelLayerSet(id, 5);
    id = Hu3DModelCreate(HuDataSelHeapReadNum(0x3D02, 0x1000, 2));
    obj->mdlId[2] = id;
    Hu3DModelLayerSet(id, 4);
    fn_1_AA0C(obj);
    id = Hu3DHookFuncCreate((HU3DMODELHOOK)fn_1_BA9C);
    obj->mdlId[10] = id;
    Hu3DModelLayerSet(id, 1);
    fn_1_B9D0(id);
    act = (M503ACT *)work;
    tab = (M503TAB *)lbl_1_data_520;
    for (i = 0; i < 5; i++) {
        act->flags = 1;
        act->unk04 = 2;
        act->unk08 = i;
        act->state = 0;
        act->owner = obj;
        act->hook = 0;
        act->tab = tab;
        act->scale = tab->y;
        act->mode = tab->flag;
        fn_1_DFA8(act);
        act++;
        tab++;
    }
    obj->objFunc = fn_1_A928;
}

// Per-frame stage update: moves the seven cursor models, drives the
// attractor table and keeps the looping motions running.
void fn_1_A928(OMOBJ *obj)
{
    HU3DMOTID mot;

    fn_1_AD34(obj);
    fn_1_AE8C(obj);
    fn_1_B888(obj);
    mot = obj->mdlId[0];
    if (Hu3DMotionEndCheck(mot)) {
        Hu3DModelAttrSet(mot, 0x40000001);
        Hu3DMotionStartEndSet(mot, lbl_1_rodata_454, Hu3DMotionMaxTimeGet(mot));
    }
    mot = obj->mdlId[1];
    if (Hu3DMotionEndCheck(mot)) {
        Hu3DModelAttrSet(mot, 0x40000001);
        Hu3DMotionStartEndSet(mot, lbl_1_rodata_454, Hu3DMotionMaxTimeGet(mot));
    }
    obj->work[0]++;
}

// The seven cursor slots sit at 0x40+3*2 of the model list; for the first
// 0x190 frames they interpolate between the two board tables, on the final
// frame they are simply switched on.
void fn_1_AD34(OMOBJ *obj)
{
    s32 i;
    HuVecF pos;
    f32 rate;

    if (obj->work[0] <= 0x190) {
        if (obj->work[0] == 0x190) {
            for (i = 0; i < 7; i++) {
                Hu3DModelAttrSet(obj->mdlId[i + 3], 1);
            }
        } else {
            rate = (f32)(((f64)obj->work[0] - lbl_1_rodata_480) / lbl_1_rodata_488);
            for (i = 0; i < 7; i++) {
                pos.x = lbl_1_data_584[i * 3] + rate * (lbl_1_data_5D8[i * 3] - lbl_1_data_584[i * 3]);
                pos.y = lbl_1_data_584[i * 3 + 1] + rate * (lbl_1_data_5D8[i * 3 + 1] - lbl_1_data_584[i * 3 + 1]);
                pos.z = lbl_1_data_584[i * 3 + 2] + rate * (lbl_1_data_5D8[i * 3 + 2] - lbl_1_data_584[i * 3 + 2]);
                Hu3DModelPosSetV(obj->mdlId[i + 3], &pos);
            }
        }
    }
}

// Walks the 16-entry spawn table: every entry drives its target, and the last
// one is flagged as the active row so the caller knows where the sweep stopped.
void fn_1_AE8C(OMOBJ *obj)
{
    s32 i;

    if (obj->work[0] < 2) {
        for (i = 0; i < 16; i++) {
            fn_1_AF20((s8)lbl_1_data_62C[i & 15], 0);
        }
    }
    fn_1_AF20((s8)lbl_1_data_62C[obj->work[0] & 15], 1);
}

// Bilinear relaxation of the 40x40 stage height field: every cell drifts
// towards its neighbour average and is re-tinted from the value it just lost.
void fn_1_B888(OMOBJ *obj)
{
    s32 i;
    s32 j;
    s32 v;
    u8 *work;
    M503CELL *cell;
    f32 y;
    f32 z;

    work = (u8 *)((M503MODEL *)((u8 *)Hu3DData + obj->mdlId[10] * 0x134))->hookData;
    for (i = 0; i < 40; i++) {
        for (j = 0; j < 40; j++) {
            cell = (M503CELL *)(work + (i * 0x280 + j * 0x10 + 0x2B4));
            y = cell->y;
            z = cell->z;
            cell->x = cell->x + lbl_1_rodata_550 * (y + z - cell->x);
            cell->z = lbl_1_rodata_450;
            v = (s32)((f64)z / lbl_1_rodata_558) + 0xFF;
            if (v > 0xFF) {
                v = 0xFF;
            }
            if (v < 0xC8) {
                v = 0xC8;
            }
            cell->col[0] = (u8)v;
            cell->col[1] = (u8)v;
            cell->col[2] = (u8)v;
            cell->col[3] = 0xFF;
        }
    }
}

// Allocates the 0x66B4-byte field block hung off the hook model and fills in
// its header (animation plus three zeroed floats).
void fn_1_B9D0(HU3DMODELID id)
{
    M503MODEL *mdl;
    u8 *work;

    mdl = (M503MODEL *)((u8 *)Hu3DData + id * 0x134);
    work = (u8 *)HuMemDirectMallocNum(0, 0x66B4, 0x1000);
    mdl->hookData = work;
    memset(work, 0, 0x66B4);
    *(void **)work = HuSprAnimRead(HuDataSelHeapReadNum(0x3D26, 0x1000, 2));
    *(f32 *)(work + 4) = lbl_1_rodata_450;
    *(f32 *)(work + 8) = lbl_1_rodata_450;
    *(f32 *)(work + 0xC) = lbl_1_rodata_450;
    fn_1_B278(mdl, lbl_1_rodata_560);
}

// Marches a probe around the board at one-degree steps, summing the distance
// field fn_1_C18C reports, then rescales the total into a score.
f32 fn_1_BFE8(f32 x, f32 z)
{
    f32 ang;
    f32 sum;
    f32 dx;
    f32 dz;

    ang = lbl_1_rodata_450;
    sum = lbl_1_rodata_450;
    sum = fn_1_C18C(x, z);
    ang = lbl_1_rodata_450;
    while (ang < lbl_1_rodata_584) {
        dx = (f32)(sin(lbl_1_rodata_570 * ang / lbl_1_rodata_578) * lbl_1_rodata_458 * lbl_1_rodata_4B8);
        dz = (f32)(cos(lbl_1_rodata_570 * ang / lbl_1_rodata_578) * lbl_1_rodata_458 * lbl_1_rodata_4B8);
        sum = sum + fn_1_C18C(x + dx, z + dz);
        ang = ang + lbl_1_rodata_580;
    }
    return (f32)(lbl_1_rodata_588 + (sum / lbl_1_rodata_590));
}

// Samples the smoothed height field with bilinear weights and returns the
// interpolated height; zero outside the 0..38 cell window.
f32 fn_1_C18C(f32 x, f32 z)
{
    u32 a;
    u32 b;
    f32 fa;
    f32 fb;
    u8 *work;
    f32 c00;
    f32 c01;
    f32 c10;
    f32 c11;

    fa = (f32)((f64)x - lbl_1_rodata_518);
    a = (u32)((f64)fa / lbl_1_rodata_520);
    if (a >= 0x27) {
        return lbl_1_rodata_450;
    }
    fa = (f32)((fa - lbl_1_rodata_520 * ((f64)a - lbl_1_rodata_598)) / lbl_1_rodata_520);
    fb = (f32)((f64)z - lbl_1_rodata_518);
    b = (u32)((f64)fb / lbl_1_rodata_520);
    if (b >= 0x27) {
        return lbl_1_rodata_450;
    }
    fb = (f32)((fb - lbl_1_rodata_520 * ((f64)b - lbl_1_rodata_598)) / lbl_1_rodata_520);
    work = (u8 *)((M503MODEL *)((u8 *)Hu3DData + lbl_1_bss_B8->mdlId[10] * 0x134))->hookData;
    c00 = ((M503CELL *)(work + (b * 0x280 + a * 0x10 + 0x2B4)))->x;
    c01 = ((M503CELL *)(work + (b * 0x280 + (a + 1) * 0x10 + 0x2B4)))->x;
    c10 = ((M503CELL *)(work + ((b + 1) * 0x280 + a * 0x10 + 0x2B4)))->x;
    c11 = ((M503CELL *)(work + ((b + 1) * 0x280 + (a + 1) * 0x10 + 0x2B4)))->x;
    c00 = c00 + fa * ((c01 - c00) / lbl_1_rodata_520);
    c10 = c10 + fa * ((c11 - c10) / lbl_1_rodata_520);
    return c00 + fb * ((c10 - c00) / lbl_1_rodata_520);
}

// Pushes a height-field stamp down into the stage grid, skipping out of range
// cells. x/z pick the cell, y is unused by the maths.
void fn_1_C424(f32 x, f32 y, f32 z)
{
    s32 i;
    s32 j;
    u32 a;
    u32 b;
    u32 ai;
    u32 bi;
    u8 *work;
    f32 (*height)[13];
    M503CELL (*cells)[40];

    work = (u8 *)((M503MODEL *)((u8 *)Hu3DData + lbl_1_bss_B8->mdlId[10] * 0x134))->hookData;
    height = (f32 (*)[13])(work + 0x10);
    cells = (M503CELL (*)[40])(work + 0x2B4);
    ai = (u32)(((f64)x - lbl_1_rodata_518) / lbl_1_rodata_520) - 6;
    bi = (u32)(((f64)y - lbl_1_rodata_518) / lbl_1_rodata_520) - 6;
    for (i = 0; i < 13; i++) {
        a = bi + i;
        if (a < 40) {
            for (j = 0; j < 13; j++) {
                b = ai + j;
                if (b < 40) {
                    cells[a][b].z = cells[a][b].z - z * height[i][j];
                }
            }
        }
    }
}
