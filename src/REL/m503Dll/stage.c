// m503Dll/stage - stage setup and per-frame drive for the mini-game.
#include "types.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/memory.h"
#include "game/sprite.h"
#include "humath.h"
#include "REL/m503Dll.h"
#include <string.h>

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

double atan2(double, double);

/* Not in the shared headers; without these MWCC assumes int returns and
 * promotes the float arguments. */
float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);

/* Attractor actor: same layout as the entries of the actor table. */
typedef struct {
    s32 flags;
    s32 unk04;
    s32 unk08;
    s32 state;
    OMOBJ *owner;
    void (*hook)(OMOBJ *owner);
    HuVecF *pos;
    f32 rad;
    s32 mode;
} M503StageActor;

/* Attractor spawn table row: position, radius and mode. */
typedef struct {
    HuVecF pos;
    f32 rad;
    s32 mode;
} M503AttractInfo;

typedef struct {
    f32 height;
    f32 base;
    f32 delta;
    GXColor col;
} M503Cell;

/* Hook data of the water-surface model (0x66B4 bytes on GC). */
typedef struct {
    ANIMDATA *anim;
    HuVecF pos;
    f32 kernel[13][13];
    M503Cell cells[40][40];
} M503Field;

extern s32 fn_1_DFA8(void *actor);
extern void fn_1_CEB0(HuVecF *pos);
extern void fn_1_CE1C(HuVecF *pos);

void fn_1_A6FC(OMOBJMAN *objMan);
void fn_1_A78C(void);
void fn_1_A790(OMOBJ *obj);
void fn_1_A928(OMOBJ *obj);
void fn_1_AA0C(OMOBJ *obj);
void fn_1_AD34(OMOBJ *obj);
void fn_1_AE8C(OMOBJ *obj);
void fn_1_AF20(s8 type, s32 mode);
void fn_1_B278(HU3DMODEL *model, f32 scale);
void fn_1_B888(OMOBJ *obj);
void fn_1_B9D0(HU3DMODELID id);
void fn_1_BA9C(HU3DMODEL *model, Mtx *mtx);
f32 fn_1_BFE8(f32 x, f32 z);
f32 fn_1_C18C(f32 x, f32 z);
void fn_1_C424(f32 x, f32 z, f32 scale);

M503AttractInfo lbl_1_data_520[5] = {
    { { -470.0f, 0.0f, -425.0f }, 250.0f, 0 },
    { { 470.0f, 0.0f, -425.0f }, 250.0f, 0 },
    { { 470.0f, 0.0f, 500.0f }, 300.0f, 0 },
    { { -470.0f, 0.0f, 500.0f }, 300.0f, 0 },
    { { 0.0f, 0.0f, 0.0f }, 450.0f, 1 },
};

HuVecF lbl_1_data_584[7] = {
    { 3047.0f, -13048.0f, -6562.0f },
    { 3277.0f, -13048.0f, -6402.0f },
    { 3578.0f, -13048.0f, -6650.0f },
    { 3748.0f, -13048.0f, -6505.0f },
    { 4066.0f, -13048.0f, -6593.0f },
    { 2849.0f, -13048.0f, -6445.0f },
    { 2660.0f, -13048.0f, -6633.0f },
};

HuVecF lbl_1_data_5D8[7] = {
    { -3731.0f, -13048.0f, -6562.0f },
    { -3465.0f, -13048.0f, -6402.0f },
    { -3130.0f, -13048.0f, -6650.0f },
    { -2923.0f, -13048.0f, -6505.0f },
    { -2651.0f, -13048.0f, -6593.0f },
    { -3946.0f, -13048.0f, -6445.0f },
    { -4129.0f, -13048.0f, -6633.0f },
};

s8 lbl_1_data_62C[16] = { 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 4, 0, 1, 0, 1 };

OMOBJ *lbl_1_bss_B8;

void fn_1_A6FC(OMOBJMAN *objMan)
{
    OMOBJ *obj;
    void *work;

    obj = omAddObjEx(objMan, 0x14, 0xB, 0, OM_GRP_NONE, fn_1_A790);
    lbl_1_bss_B8 = obj;
    work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M503StageActor) * 5, HU_MEMNUM_OVL);
    obj->data = work;
    memset(work, 0, sizeof(M503StageActor) * 5);
    obj->work[0] = 0;
}

void fn_1_A78C(void)
{
}

void fn_1_A790(OMOBJ *obj)
{
    M503StageActor *act = obj->data;
    M503AttractInfo *info;
    HU3DMODELID id;
    s32 i;

    omSetTra(obj, 0.0f, 0.0f, 0.0f);
    id = Hu3DModelCreate(HuDataSelHeapReadNum(0x3D0000, HU_MEMNUM_OVL, HEAP_MODEL));
    obj->mdlId[0] = id;
    Hu3DModelLayerSet(id, 0);
    id = Hu3DModelCreate(HuDataSelHeapReadNum(0x3D0001, HU_MEMNUM_OVL, HEAP_MODEL));
    obj->mdlId[1] = id;
    Hu3DModelLayerSet(id, 5);
    id = Hu3DModelCreate(HuDataSelHeapReadNum(0x3D0002, HU_MEMNUM_OVL, HEAP_MODEL));
    obj->mdlId[2] = id;
    Hu3DModelLayerSet(id, 4);
    fn_1_AA0C(obj);
    id = Hu3DHookFuncCreate(fn_1_BA9C);
    obj->mdlId[10] = id;
    Hu3DModelLayerSet(id, 1);
    fn_1_B9D0(id);
    info = lbl_1_data_520;
    for (i = 0; i < 5; i++, act++, info++) {
        act->flags = 1;
        act->unk04 = 2;
        act->unk08 = i;
        act->state = 0;
        act->owner = obj;
        act->hook = NULL;
        act->pos = &info->pos;
        act->rad = info->rad;
        act->mode = info->mode;
        fn_1_DFA8(act);
    }
    obj->objFunc = fn_1_A928;
}

void fn_1_A928(OMOBJ *obj)
{
    HU3DMODELID id;

    fn_1_AD34(obj);
    fn_1_AE8C(obj);
    fn_1_B888(obj);
    id = obj->mdlId[0];
    if (Hu3DMotionEndCheck(id) != 0) {
        Hu3DModelAttrSet(id, 0x40000001);
        Hu3DMotionStartEndSet(id, 241.0f, Hu3DMotionMaxTimeGet(id));
    }
    id = obj->mdlId[1];
    if (Hu3DMotionEndCheck(id) != 0) {
        Hu3DModelAttrSet(id, 0x40000001);
        Hu3DMotionStartEndSet(id, 241.0f, Hu3DMotionMaxTimeGet(id));
    }
    obj->work[0]++;
}

void fn_1_AA0C(OMOBJ *obj)
{
    HU3DMODELID first;
    HU3DMODELID id;
    HuVecF dir;
    HuVecF rot;
    f32 maxTime;
    s32 i;

    for (i = 0; i < 7; i++) {
        if (i == 0) {
            first = Hu3DModelCreate(HuDataSelHeapReadNum(0x3D0003, HU_MEMNUM_OVL, HEAP_MODEL));
            id = first;
        } else {
            id = Hu3DModelLink(first);
        }
        obj->mdlId[i + 3] = id;
        Hu3DModelAttrSet(id, 0x40000001);
        Hu3DModelLayerSet(id, 2);
        dir.x = lbl_1_data_5D8[i].x - lbl_1_data_584[i].x;
        dir.y = lbl_1_data_5D8[i].y - lbl_1_data_584[i].y;
        dir.z = lbl_1_data_5D8[i].z - lbl_1_data_584[i].z;
        rot.x = 57.29578f * atan2(-dir.y, HuMagXZVecF(&dir));
        rot.y = 90.0 + 57.29578f * atan2(dir.x, dir.z);
        rot.z = 0.0f;
        Hu3DModelRotSetV(id, &rot);
        maxTime = Hu3DMotionMaxTimeGet(id);
        Hu3DMotionTimeSet(id, 1.52587890625e-05 * (f64)(f32)(s32)frand() * maxTime);
    }
}

void fn_1_AD34(OMOBJ *obj)
{
    HuVecF pos;
    f32 rate;
    s32 i;
    s32 time = obj->work[0];

    if (time <= 0x190) {
        if (time == 0x190) {
            for (i = 0; i < 7; i++) {
                Hu3DModelAttrSet(obj->mdlId[i + 3], 1);
            }
            return;
        }
        rate = (f32)time / 400.0f;
        for (i = 0; i < 7; i++) {
            pos.x = lbl_1_data_584[i].x + rate * (lbl_1_data_5D8[i].x - lbl_1_data_584[i].x);
            pos.y = lbl_1_data_584[i].y + rate * (lbl_1_data_5D8[i].y - lbl_1_data_584[i].y);
            pos.z = lbl_1_data_584[i].z + rate * (lbl_1_data_5D8[i].z - lbl_1_data_584[i].z);
            Hu3DModelPosSetV(obj->mdlId[i + 3], &pos);
        }
    }
}

void fn_1_AE8C(OMOBJ *obj)
{
    s32 i;

    if ((u32)obj->work[0] < 2U) {
        for (i = 0; i < 0x10; i++) {
            fn_1_AF20(lbl_1_data_62C[i & 0xF], 0);
        }
    }
    fn_1_AF20(lbl_1_data_62C[obj->work[0] & 0xF], 1);
}

void fn_1_AF20(s8 type, s32 mode)
{
    HuVecF pos;
    f32 off;

    switch (type) {
        case 0:
            pos.x = -2010.0000000000002 + 3410.0 * (1.52587890625e-05 * (f64)(f32)(s32)frand());
            pos.z = -7800.0f;
            break;
        case 1:
            off = 1131.0 * (1.52587890625e-05 * (f64)(f32)(s32)frand());
            pos.x = 4213.0 + (f64)off;
            if (off != 0.0f) {
                off = 100.0 * (2.0 * ((f64)off / 1131.0));
            }
            pos.z = -7990.000000000001 + (f64)off;
            break;
        case 2:
            pos.x = 1630.0f;
            pos.z = -7500.0 + 50.0 * (3.0517578125e-05 * (f64)(f32)((s32)frand() - 0x8000));
            break;
        case 3:
            pos.x = -3105.0 + 50.0 * (3.0517578125e-05 * (f64)(f32)((s32)frand() - 0x8000));
            pos.z = -7160.0f;
            break;
        case 4:
            pos.x = -4530.0f;
            pos.z = -7045.0 + 50.0 * (3.0517578125e-05 * (f64)(f32)((s32)frand() - 0x8000));
            break;
#ifdef TARGET_PC
        default:
            pos.x = 0.0f;
            pos.z = 0.0f;
            break;
#endif
    }
    if (mode == 0) {
        pos.y = -13800.0f;
        fn_1_CEB0(&pos);
        return;
    }
    pos.y = -14000.0f;
    fn_1_CE1C(&pos);
}

void fn_1_B278(HU3DMODEL *model, f32 scale)
{
    M503Field *work = model->hookData;
    M503Cell *cell;
    f32 maxDist;
    f32 x;
    f32 z;
    f32 ratio;
    f32 height;
    s32 i;
    s32 j;

    maxDist = sqrtf(720000.0f);
    for (i = 0; i < 40; i++) {
        for (j = 0; j < 40; j++) {
            cell = &work->cells[i][j];
            x = -585.0 + 30.0 * (f64)j;
            z = -585.0 + 30.0 * (f64)i;
            ratio = HuMagPoint2D(x, z) / maxDist;
            if (ratio > 1.0f) {
                ratio = 1.0f;
            }
            height = (f64)scale * -cos(1.5707963267948966 * (f64)ratio);
            cell->base = height;
            cell->height = height;
            cell->delta = 0.0f;
            cell->col.r = 0xFF;
            cell->col.g = 0xFF;
            cell->col.b = 0xFF;
            cell->col.a = 0xFF;
        }
    }
    for (i = 0; i < 13; i++) {
        for (j = 0; j < 13; j++) {
            x = -185.0 + 30.0 * (f64)j;
            z = -185.0 + 30.0 * (f64)i;
            ratio = HuMagPoint2D(x, z) / 200.0f;
            if (ratio > 1.0f) {
                ratio = 1.0f;
            }
            work->kernel[i][j] = cos(1.5707963267948966 * (f64)ratio);
        }
    }
}

void fn_1_B888(OMOBJ *obj)
{
    M503Field *work;
    M503Cell *cell;
    f32 delta;
    s32 val;
    s32 i;
    s32 j;

    for (i = 0; i < 40; i++) {
        for (j = 0; j < 40; j++) {
            work = Hu3DData[obj->mdlId[10]].hookData;
            cell = &work->cells[i][j];
            delta = cell->delta;
            cell->height = cell->height + 0.1 * ((cell->base + delta) - cell->height);
            cell->delta = 0.0f;
            val = (s32)((f64)delta / 0.3) + 0xFF;
            if (val > 0xFF) {
                val = 0xFF;
            }
            if (val < 0xC8) {
                val = 0xC8;
            }
            cell->col.r = val;
            cell->col.g = val;
            cell->col.b = val;
            cell->col.a = 0xFF;
        }
    }
}

void fn_1_B9D0(HU3DMODELID id)
{
    HU3DMODEL *model = &Hu3DData[id];
    M503Field *work;

    work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M503Field), HU_MEMNUM_OVL);
    model->hookData = work;
    memset(work, 0, sizeof(M503Field));
    work->anim = HuSprAnimRead(HuDataSelHeapReadNum(0x3D0026, HU_MEMNUM_OVL, HEAP_MODEL));
    work->pos.x = 0.0f;
    work->pos.y = 0.0f;
    work->pos.z = 0.0f;
    fn_1_B278(model, 100.0f);
}

#ifdef TARGET_PC
#define M503_FIFO_COLOR(c) \
    GXWGFifo.u8 = (c).r; \
    GXWGFifo.u8 = (c).g; \
    GXWGFifo.u8 = (c).b; \
    GXWGFifo.u8 = (c).a
#else
#define M503_FIFO_COLOR(c) GXWGFifo.u32 = *(u32 *)&(c)
#endif

void fn_1_BA9C(HU3DMODEL *model, Mtx *mtx)
{
    M503Field *work = model->hookData;
    M503Cell *cell;
    Mtx mat;
    f32 x;
    f32 z;
    f32 s;
    f32 t;
    s32 i;
    s32 j;

    HuSprTexLoad(work->anim, 0, GX_TEXMAP0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, 0x7D);
    PSMTXIdentity(mat);
    PSMTXTrans(mat, work->pos.x, work->pos.y, work->pos.z);
    PSMTXConcat(*mtx, mat, mat);
    GXLoadPosMtxImm(mat, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_TRUE, GX_LESS, GX_TRUE);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_LEQUAL, 0xFF);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_COPY);
    GXSetNumChans(1);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
#ifdef TARGET_PC
#define M503_VTX(px, py, pz, c, ss, tt)     GXPosition3f32(px, py, pz); GXColor4u8((c).r, (c).g, (c).b, (c).a); GXTexCoord2f32(ss, tt)
    for (i = 0; i < 0x27; i++) {
        for (j = 0; j < 0x27; j++) {
            x = -585.0 + 30.0 * (f64)j;
            z = -585.0 + 30.0 * (f64)i;
            s = 0.02564102564102564 * (f64)j;
            t = 0.02564102564102564 * (f64)i;
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            cell = &work->cells[i][j];
            M503_VTX(x, cell->height, z, cell->col, s, t);
            cell = &work->cells[i][j + 1];
            M503_VTX(30.0f + x, cell->height, z, cell->col, 0.02564102564102564f + s, t);
            cell = &work->cells[i + 1][j + 1];
            M503_VTX(30.0f + x, cell->height, 30.0f + z, cell->col, 0.02564102564102564f + s, 0.02564102564102564f + t);
            cell = &work->cells[i + 1][j];
            M503_VTX(x, cell->height, 30.0f + z, cell->col, s, 0.02564102564102564f + t);
            GXEnd();
        }
    }
#else
    for (i = 0; i < 0x27; i++) {
        for (j = 0; j < 0x27; j++) {
            x = -585.0 + 30.0 * (f64)j;
            z = -585.0 + 30.0 * (f64)i;
            s = 0.02564102564102564 * (f64)j;
            t = 0.02564102564102564 * (f64)i;
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            cell = &work->cells[i][j];
            GXWGFifo.f32 = x;
            GXWGFifo.f32 = cell->height;
            GXWGFifo.f32 = z;
            M503_FIFO_COLOR(cell->col);
            GXWGFifo.f32 = s;
            GXWGFifo.f32 = t;
            cell = &work->cells[i][j + 1];
            GXWGFifo.f32 = 30.0 + (f64)x;
            GXWGFifo.f32 = cell->height;
            GXWGFifo.f32 = z;
            M503_FIFO_COLOR(cell->col);
            GXWGFifo.f32 = 0.02564102564102564 + (f64)s;
            GXWGFifo.f32 = t;
            cell = &work->cells[i + 1][j + 1];
            GXWGFifo.f32 = 30.0 + (f64)x;
            GXWGFifo.f32 = cell->height;
            GXWGFifo.f32 = 30.0 + (f64)z;
            M503_FIFO_COLOR(cell->col);
            GXWGFifo.f32 = 0.02564102564102564 + (f64)s;
            GXWGFifo.f32 = 0.02564102564102564 + (f64)t;
            cell = &work->cells[i + 1][j];
            GXWGFifo.f32 = x;
            GXWGFifo.f32 = cell->height;
            GXWGFifo.f32 = 30.0 + (f64)z;
            M503_FIFO_COLOR(cell->col);
            GXWGFifo.f32 = s;
            GXWGFifo.f32 = 0.02564102564102564 + (f64)t;
        }
    }
#endif
}

f32 fn_1_BFE8(f32 x, f32 z)
{
    f32 dx;
    f32 sum;
    f32 ang;

    sum = fn_1_C18C(x, z);
    ang = 0.0f;
    while (ang < 359.0f) {
        dx = 100.0 * (0.5 * HuSin(ang));
        sum += fn_1_C18C(x + dx, z + (f32)(100.0 * (0.5 * HuCos(ang))));
        ang += 90.0f;
    }
    return 20.0 + (f64)(sum / 5.0f);
}

f32 fn_1_C18C(f32 x, f32 z)
{
    M503Field *work = Hu3DData[lbl_1_bss_B8->mdlId[10]].hookData;
    f32 offX;
    f32 offZ;
    f32 fracX;
    f32 fracZ;
    f32 c00;
    f32 c10;
    f32 c11;
    f32 row0;
    u32 ix;
    u32 iz;

    offX = x - -585.0;
    ix = (u32)((f64)offX / 30.0);
    if (ix >= 0x27U) {
        return 0.0f;
    }
    offZ = z - -585.0;
    iz = (u32)((f64)offZ / 30.0);
    if (iz >= 0x27U) {
        return 0.0f;
    }
    fracX = ((f64)offX - 30.0 * (f64)(f32)ix) / 30.0;
    c00 = work->cells[iz][ix].height;
    c10 = work->cells[iz + 1][ix].height;
    row0 = (f64)c00 + (f64)fracX * ((f64)(work->cells[iz][ix + 1].height - c00) / 30.0);
    c11 = work->cells[iz + 1][ix + 1].height;
    fracZ = ((f64)offZ - 30.0 * (f64)(f32)iz) / 30.0;
    return (f64)row0 + (f64)fracZ * ((f64)((f32)((f64)c10 + (f64)fracX * ((f64)(c11 - c10) / 30.0)) - row0) / 30.0);
}

void fn_1_C424(f32 x, f32 z, f32 scale)
{
    M503Field *work = Hu3DData[lbl_1_bss_B8->mdlId[10]].hookData;
    u32 row;
    u32 col;
    u32 i;
    u32 j;

    for (i = 0; i < 13U; i++) {
        row = (u32)(((f64)z - -585.0) / 30.0) - 6 + i;
        if (row < 0x28U) {
            for (j = 0; j < 13U; j++) {
                col = (u32)(((f64)x - -585.0) / 30.0) - 6 + j;
                if (col < 0x28U) {
                    work->cells[row][col].delta = work->cells[row][col].delta - scale * work->kernel[i][j];
                }
            }
        }
    }
}
