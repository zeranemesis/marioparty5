#include "REL/m574Dll.h"
#include "game/hsfformat.h"

#include "dolphin/mtx.h"

/* Not byte-identical: FPR/GPR allocation only (7AE4, 7C88, 7FD8, 82AC, 87B8, 8C94); logic matches the asm. */
#define M574_COL_DYN_MAX 0x52
#define M574_COL_STA_MAX 0x14

#define M574_FP_NAN 1

typedef struct M574ColTbl_s {
    M574_COLLIDER *dyn[M574_COL_DYN_MAX];
    M574_COLHEAD *sta[M574_COL_STA_MAX];
} M574_COL_TBL;

static void fn_1_7AD4(OMOBJ *obj);
static void fn_1_7AE4(OMOBJ *obj);
static s32 fn_1_7C88(M574_COLLIDER *a, M574_COLLIDER *b);
static s32 fn_1_7FD8(M574_COLLIDER *col, M574_COL_CIRCLE *circle);
static s32 fn_1_82AC(M574_COLLIDER *col, M574_COL_WALL *wall, M574_PLANE *plane, HuVecF *pt);
static s32 fn_1_87B8(M574_COLLIDER *col, M574_COL_WALL *wall);

static s32 quadOrder[4] = { 0, 1, 3, 2 };

static OMOBJ *colObj;

static inline s32 M574FpClassF(f32 x)
{
    switch (*(s32 *)&x & 0x7F800000) {
        case 0x7F800000:
            if ((*(s32 *)&x & 0x007FFFFF) != 0) {
                return 1;
            }
            return 2;
        case 0:
            if ((*(s32 *)&x & 0x007FFFFF) != 0) {
                return 5;
            }
            return 3;
        default:
            return 4;
    }
}

void fn_1_7A48(OMOBJMAN *om)
{
    OMOBJ *obj;
    void *data;

    obj = omAddObjEx(om, 0x32, 0, 0, -1, fn_1_7AD4);
    colObj = obj;
    data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M574_COL_TBL), HU_MEMNUM_OVL);
    obj->data = data;
    memset(data, 0, sizeof(M574_COL_TBL));
}

void fn_1_7AD0(void)
{
}

static void fn_1_7AD4(OMOBJ *obj)
{
    obj->objFunc = fn_1_7AE4;
}

static void fn_1_7AE4(OMOBJ *obj)
{
    M574_COL_TBL *tbl;
    M574_COLLIDER *a;
    M574_COLLIDER *b;
    M574_COLHEAD *sta;
    s32 i;
    s32 j;

    tbl = obj->data;
    for (i = 0; i < M574_COL_DYN_MAX; i++) {
        a = tbl->dyn[i];
        if (a != NULL && (a->flag & 1) != 0 && a->kind == 0) {
            for (j = i + 1; j < M574_COL_DYN_MAX; j++) {
                b = tbl->dyn[j];
                if (b != NULL && (b->flag & 1) != 0 && b->kind == 0) {
                    fn_1_7C88(a, b);
                }
            }
            if ((a->flag & 2) == 0) {
                for (j = 0; j < M574_COL_STA_MAX; j++) {
                    sta = tbl->sta[j];
                    if (sta != NULL && (sta->flag & 1) != 0) {
                        switch (sta->kind) {
                            case 0:
                                fn_1_7FD8(a, (M574_COL_CIRCLE *)sta);
                                break;
                            case 1:
                                fn_1_87B8(a, (M574_COL_WALL *)sta);
                                break;
                        }
                    }
                }
            }
        }
    }
    for (i = 0; i < M574_COL_DYN_MAX; i++) {
        a = tbl->dyn[i];
        if (a != NULL && a->hook != NULL) {
            a->hook(a->user);
        }
    }
}

static s32 fn_1_7C88(M574_COLLIDER *a, M574_COLLIDER *b)
{
    HuVecF *pa;
    HuVecF *pb;
    f32 dx;
    f32 dy;
    f32 dist;
    f32 sum;
    f32 mx;
    f32 my;
    f32 half;

    pa = a->pos;
    pb = b->pos;
    dx = pa->x - pb->x;
    dy = pa->y - pb->y;
    dist = sqrtf(dx * dx + dy * dy);
    sum = a->radius + b->radius;
    if (sum > dist) {
        if (0.0f == dist) {
            dx = 0.0f;
            dy = -1.0f;
        }
        else {
            dx = dx / dist;
            dy = dy / dist;
        }
        mx = pa->x + 0.5 * (pb->x - pa->x);
        my = pa->y + 0.5 * (pb->y - pa->y);
        half = 0.5 * sum;
        pa->x = mx + dx * half;
        pa->y = my + dy * half;
        pb->x = mx - dx * half;
        pb->y = my - dy * half;
        a->vel->x = a->vel->x + 0.3 * (dx * half);
        a->vel->y = a->vel->y + 0.3 * (dy * half);
        b->vel->x = b->vel->x + -0.3 * (dx * half);
        b->vel->y = b->vel->y + -0.3 * (dy * half);
        return 1;
    }
    return 0;
}

static s32 fn_1_7FD8(M574_COLLIDER *col, M574_COL_CIRCLE *circle)
{
    HuVecF *pos;
    HuVecF *cpos;
    f32 dx;
    f32 dy;
    f32 dist;
    f32 sum;

    pos = col->pos;
    cpos = circle->pos;
    dx = pos->x - cpos->x;
    dy = pos->y - cpos->y;
    dist = sqrtf(dx * dx + dy * dy);
    sum = col->radius + circle->radius;
    if (sum > dist) {
        if (circle->hook != NULL) {
            circle->hook(circle, col);
        }
        if (0.0f == dist) {
            dx = 0.0f;
            dy = -1.0f;
        }
        else {
            dx = dx / dist;
            dy = dy / dist;
        }
        pos->x = cpos->x + dx * sum;
        pos->y = cpos->y + dy * sum;
        col->vel->x = col->vel->x + 100.0 * (0.5 * dx);
        col->vel->y = col->vel->y + 100.0 * (0.5 * dy);
        return 1;
    }
    return 0;
}

static s32 fn_1_82AC(M574_COLLIDER *col, M574_COL_WALL *wall, M574_PLANE *plane, HuVecF *pt)
{
    HuVecF *pos;
    f32 radius;
    f32 prevSide;
    f32 sideA;
    f32 sideB;
    f32 px;
    f32 py;
    f32 ex0;
    f32 ey0;
    f32 ex1;
    f32 ey1;
    f32 speed;
    f32 scale;
    s32 hit;
    s32 i;
    HuVecF cross;

    hit = 0;
    pos = col->pos;
    radius = col->radius;
    for (i = 0; i < wall->edgeNum; i++, plane++) {
        prevSide = plane->d + (plane->n.x * pt->x + plane->n.y * pt->y);
        if (prevSide < 0.0f) {
            continue;
        }
        px = pos->x - plane->n.x * radius;
        py = pos->y - plane->n.y * radius;
        sideA = plane->d + (plane->n.x * px + plane->n.y * py);
        sideB = plane->d + (plane->n.x * pos->x + plane->n.y * pos->y);
        if (!(sideA >= 0.0f) || !(sideB >= 0.0f)) {
            sideB = -sideB;
            sideA = prevSide + sideB;
            if (0.0f == sideA) {
                continue;
            }
            cross.x = pos->x + (sideB * (pt->x - pos->x)) / sideA;
            cross.y = pos->y + (sideB * (pt->y - pos->y)) / sideA;
            px = cross.x - plane->p0.x;
            py = cross.y - plane->p0.y;
            ex0 = cross.x - plane->p1.x;
            ey0 = cross.y - plane->p1.y;
            if (px * ex0 + py * ey0 > 0.0f) {
                continue;
            }
            cross.x = pos->x + 100.0 * (3.0 * plane->n.x);
            cross.y = pos->y + 100.0 * (3.0 * plane->n.y);
            prevSide = plane->d + (plane->n.x * cross.x + plane->n.y * cross.y);
            sideA = prevSide + sideB;
            if (0.0f == sideA) {
                continue;
            }
            pos->x = pos->x + ((sideB * (cross.x - pos->x)) / sideA + plane->n.x * radius);
            pos->y = pos->y + ((sideB * (cross.y - pos->y)) / sideA + plane->n.y * radius);
            hit = 1;
            if (wall->hook != NULL) {
                wall->lastPlane = plane;
                if (wall->hook(wall, col) != 0) {
                    continue;
                }
            }
            speed = PSVECMag(col->vel);
            if (speed > 100.00000521540642) {
                speed = 100.00001f;
            }
            C_VECReflect(col->vel, &plane->n, &cross);
            if (M574FpClassF(cross.x) == M574_FP_NAN) {
                continue;
            }
            if (M574FpClassF(cross.y) == M574_FP_NAN) {
                continue;
            }
            scale = 0.8 * speed;
            col->vel->x = cross.x * scale;
            col->vel->y = cross.y * scale;
        }
    }
    return hit;
}

static s32 fn_1_87B8(M574_COLLIDER *col, M574_COL_WALL *wall)
{
    M574_PLANE *plane;
    HuVecF pt;
    f32 dx;
    f32 dy;
    f32 dist;
    s32 side;
    s32 i;

    if ((u32)wall->shape == 6) {
        if (col->pos->x < 0.0f) {
            side = 0;
        }
        else {
            side = 1;
        }
        if ((wall->id >> 1) == side) {
            plane = wall->planeA;
            pt.x = col->prevPos.x;
            pt.y = col->prevPos.y;
            pt.z = 0.0f;
            for (i = 0; i < wall->edgeNum; i++, plane++) {
                if (plane->d + (plane->n.x * pt.x + plane->n.y * pt.y) < 0.0f) {
                    break;
                }
            }
            if (i < wall->edgeNum) {
                dx = (plane->p0.x + plane->p1.x) * 0.5f - pt.x;
                dy = (plane->p0.y + plane->p1.y) * 0.5f - pt.y;
                dist = sqrtf(dx * dx + dy * dy);
                if (dist < 300.0) {
                    pt.y = pt.y + 40.0;
                    if (fn_1_82AC(col, wall, wall->planeA, &pt) == 0) {
                        return fn_1_82AC(col, wall, wall->planeB, &pt);
                    }
                    return 1;
                }
            }
        }
    }
    return fn_1_82AC(col, wall, wall->planeA, &col->prevPos);
}

s32 fn_1_8AD0(M574_COLLIDER *col)
{
    OMOBJ *obj;
    M574_COL_TBL *tbl;
    M574_COLLIDER **p;
    s32 i;

    obj = colObj;
    tbl = obj->data;
    p = tbl->dyn;
    for (i = 0; i < M574_COL_DYN_MAX; i++, p++) {
        if (*p == NULL) {
            *p = col;
            return i;
        }
    }
    return -1;
}

s32 fn_1_8B48(M574_COLHEAD *col)
{
    OMOBJ *obj;
    M574_COL_TBL *tbl;
    M574_COLHEAD **p;
    s32 i;

    obj = colObj;
    tbl = obj->data;
    p = tbl->sta;
    for (i = 0; i < M574_COL_STA_MAX; i++, p++) {
        if (*p == NULL) {
            *p = col;
            return i + M574_COL_DYN_MAX;
        }
    }
    return -1;
}

void fn_1_8BC0(s32 handle)
{
    OMOBJ *obj;
    M574_COL_TBL *tbl;

    obj = colObj;
    tbl = obj->data;
    if (handle >= 0) {
        if (handle < M574_COL_DYN_MAX) {
            tbl->dyn[handle] = NULL;
        }
        handle -= M574_COL_DYN_MAX;
#ifdef TARGET_PC
        if (handle >= 0 && handle < M574_COL_STA_MAX) {
#else
        if (handle < M574_COL_STA_MAX) {
#endif
            tbl->sta[handle] = NULL;
        }
    }
}

void *fn_1_8C24(s32 handle)
{
    OMOBJ *obj;
    M574_COL_TBL *tbl;

    obj = colObj;
    tbl = obj->data;
    if (handle < 0) {
        return NULL;
    }
    if (handle < M574_COL_DYN_MAX) {
        return tbl->dyn[handle];
    }
    handle -= M574_COL_DYN_MAX;
    if (handle < M574_COL_STA_MAX) {
        return tbl->sta[handle];
    }
    return NULL;
}

void fn_1_8C94(M574_COL_WALL *wall, HSFOBJECT *objs, s32 objNum)
{
    HSFOBJECT *obj;
    HSFFACE *face;
    HSFTRANSFORM **objTbl;
    HuVecF *vtx;
    HuVecF *va;
    HuVecF *vb;
    M574_EDGE *edge;
    Mtx *mtx;
    s32 size;
    s32 quadNum;
    s32 n;
    s32 faceNum;
    s32 k;

    quadNum = 0;
    obj = objs;
    n = objNum;
    while (n-- != 0) {
        if (obj->type == 2) {
            face = obj->mesh.face->data;
            faceNum = obj->mesh.face->count;
            while (faceNum-- != 0) {
                if (face->type == 3) {
                    quadNum++;
                }
                face++;
            }
        }
        obj++;
    }
    size = quadNum * sizeof(M574_EDGE);
    edge = HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
    wall->edge = edge;
    memset(edge, 0, size);
    size = quadNum * sizeof(M574_PLANE);
    wall->planeA = HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
    memset(wall->planeA, 0, size);
    wall->planeB = HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
    memset(wall->planeB, 0, size);
    wall->objNum = objNum;
    size = wall->objNum * sizeof(HSFTRANSFORM *);
    objTbl = HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
    wall->objTbl = objTbl;
    memset(objTbl, 0, size);
    size = wall->objNum * sizeof(Mtx);
    mtx = HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
    wall->mtxTbl = mtx;
    memset(mtx, 0, size);
    obj = objs;
    n = objNum;
    wall->edgeNum = 0;
    while (n-- != 0) {
        if (obj->type == 2) {
            vtx = obj->mesh.vertex->data;
            face = obj->mesh.face->data;
            faceNum = obj->mesh.face->count;
            while (faceNum-- != 0) {
                if (face->type == 3) {
                    for (k = 0; k < 4; k++) {
                        va = &vtx[face->index[quadOrder[k]].vertex];
                        vb = &vtx[face->index[quadOrder[(k + 1) & 3]].vertex];
                        if (va->z > -50.0 && vb->z > -50.0) {
                            edge->p0.x = va->x;
                            edge->p0.y = va->y;
                            edge->p1.x = vb->x;
                            edge->p1.y = vb->y;
                            edge->mtx = mtx;
                            edge++;
                            wall->edgeNum++;
                            break;
                        }
                    }
                }
                face++;
            }
        }
        *objTbl = &obj->mesh.base;
        objTbl++;
        mtx++;
        obj++;
    }
}

void fn_1_8F54(M574_COL_WALL *wall, s16 model)
{
    HSFDATA *hsf;

    hsf = Hu3DData[model].hsf;
    fn_1_8C94(wall, hsf->object, hsf->objectNum);
}

void fn_1_8FA8(M574_COL_WALL *wall, HSFOBJECT *obj)
{
    fn_1_8C94(wall, obj, 1);
}

void fn_1_8FDC(M574_COL_WALL *wall)
{
    Mtx mtxC;
    Mtx mtxB;
    Mtx mtxA;
    HSFTRANSFORM **objTbl;
    Mtx *mtxTbl;
    M574_EDGE *edge;
    M574_PLANE *plane;
    f64 ny;
    f64 nx;
    f64 len;
    s32 i;

    objTbl = wall->objTbl;
    mtxTbl = wall->mtxTbl;
    edge = wall->edge;
    plane = wall->planeB;
    wall->planeB = wall->planeA;
    wall->planeA = plane;
    for (i = 0; i < wall->objNum; i++) {
        PSMTXRotRad(mtxB, 'z', 0.017453292f * (*objTbl)->rot.z);
        PSMTXRotRad(mtxC, 'y', 0.017453292f * (*objTbl)->rot.y);
        PSMTXConcat(mtxB, mtxC, mtxA);
        PSMTXRotRad(mtxB, 'x', 0.017453292f * (*objTbl)->rot.x);
        PSMTXConcat(mtxA, mtxB, mtxC);
        PSMTXTrans(mtxB, (*objTbl)->pos.x, (*objTbl)->pos.y, (*objTbl)->pos.z);
        PSMTXConcat(mtxB, mtxC, mtxA);
        PSMTXScale(mtxB, (*objTbl)->scale.x, (*objTbl)->scale.y, (*objTbl)->scale.z);
        PSMTXConcat(mtxA, mtxB, *mtxTbl);
        objTbl++;
        mtxTbl++;
    }
    for (i = 0; i < wall->edgeNum; i++) {
        PSMTXMultVec(*edge->mtx, &edge->p0, &plane->p0);
        PSMTXMultVec(*edge->mtx, &edge->p1, &plane->p1);
        ny = plane->p1.y - plane->p0.y;
        nx = plane->p1.x - plane->p0.x;
        len = sqrt(ny * ny + nx * nx);
        if (0.0 != len) {
            ny = ny / len;
            nx = nx / len;
        }
        plane->n.x = -ny;
        plane->n.y = nx;
        plane->n.z = 0.0f;
        plane->d = -(-ny * plane->p0.x + nx * plane->p0.y);
        plane++;
        edge++;
    }
}
