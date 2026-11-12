#include "types.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/hsfformat.h"
#include "game/memory.h"
#include "humath.h"
#ifdef __MWERKS__
/* MSL __fpclassifyf, spelled out: float.h redeclares __float_nan with a conflicting type */
inline int m580FpClassify(float value)
{
    u32 bits = *(u32 *)&value;

    switch (bits & 0x7F800000) {
        case 0x7F800000:
            if ((bits & 0x7FFFFF) != 0) {
                return 1;
            } else {
                return 2;
            }
        case 0:
            if ((bits & 0x7FFFFF) != 0) {
                return 5;
            } else {
                return 3;
            }
    }
    return 4;
}
#define M580_ISNAN(x) (m580FpClassify(x) == 1)
#else
#define M580_ISNAN(x) ((x) != (x))
#endif
#include <string.h>

#include "REL/m580Dll.h"

#define DEG2RAD_F 0.017453292f

void C_VECReflect(const Vec *src, const Vec *normal, Vec *dst);
void PSMTXRotRad(Mtx m, char axis, f32 rad);

/* 20 circle bodies followed by 20 wall sets (0xA0 bytes on GC) */
typedef struct M580ReflectData_s {
    M580_BODY *bodies[M580_BODY_MAX];
    M580_BODY *walls[M580_BODY_MAX];
} M580ReflectData;

void fn_1_1112C(OMOBJ *obj);
void fn_1_1113C(OMOBJ *obj);
s32 fn_1_11410(M580_BODY *a, M580_BODY *b);
s32 fn_1_11880(M580_BODY *body, M580_BODY *wall);

s32 lbl_1_data_8A8[4] = { 0, 1, 3, 2 };

OMOBJ *lbl_1_bss_258;

void fn_1_110A0(OMOBJMAN *objMan)
{
    OMOBJ *obj;

    obj = omAddObjEx(objMan, 0x46, 0, 0, OM_GRP_NONE, fn_1_1112C);
    lbl_1_bss_258 = obj;
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M580ReflectData), HU_MEMNUM_OVL);
    memset(obj->data, 0, sizeof(M580ReflectData));
}

void fn_1_11128(void)
{
}

void fn_1_1112C(OMOBJ *obj)
{
    obj->objFunc = fn_1_1113C;
}

void fn_1_1113C(OMOBJ *obj)
{
    M580ReflectData *data = obj->data;
    M580_BODY *body;
    M580_BODY *other;
    M580_BODY *wall;
    s32 i;
    s32 j;

    for (i = 0; i < M580_BODY_MAX; i++) {
        body = data->bodies[i];
        if (body != NULL && (body->flags & 1) != 0 && body->type == 0) {
            for (j = i + 1; j < M580_BODY_MAX; j++) {
                other = data->bodies[j];
                if (other != NULL && (other->flags & 1) != 0 && other->type == 0 && (body->mask & other->mask) != 0) {
                    fn_1_11410(body, other);
                }
            }
            if ((body->flags & 2) == 0) {
                for (j = 0; j < M580_BODY_MAX; j++) {
                    wall = data->walls[j];
                    if (wall != NULL && (wall->flags & 1) != 0 && (body->mask & wall->mask) != 0) {
                        if (wall->type != 0xA) {
                        } else {
                            fn_1_11880(body, wall);
                        }
                    }
                }
            }
        }
    }
    for (i = 0; i < M580_BODY_MAX; i++) {
        body = data->bodies[i];
        if (body != NULL) {
            if (body->type == 0) {
                f32 x = body->u.circle.pos->x;
                f32 z;

                if (M580_ISNAN(x) || (z = body->u.circle.pos->z, M580_ISNAN(z))) {
                    *body->u.circle.pos = body->u.circle.prev;
                }
            }
            if (body->update != NULL) {
                body->update(body->obj);
            }
        }
    }
}

s32 fn_1_11410(M580_BODY *a, M580_BODY *b)
{
    HuVecF *pa = a->u.circle.pos;
    HuVecF *pb = b->u.circle.pos;
    f32 dx = pa->x - pb->x;
    f32 dz = pa->z - pb->z;
    f32 dist = HuMagPoint2D(dx, dz);
    f32 sum = a->u.circle.radius + b->u.circle.radius;
    f32 nx;
    f32 nz;
    f32 midX;
    f32 midZ;
    f32 half;
    f32 k;

    if (sum > dist) {
        if (0.0f == dist) {
            nx = 0.0f;
            nz = -1.0f;
        } else {
            nx = dx / dist;
            nz = dz / dist;
        }
        midX = pa->x + 0.5 * (pb->x - pa->x);
        midZ = pa->z + 0.5 * (pb->z - pa->z);
        half = 0.5 * sum;
        pa->x = midX + nx * half;
        pa->z = midZ + nz * half;
        pb->x = midX - nx * half;
        pb->z = midZ - nz * half;
        if ((a->kind != 0 || b->kind != 1 || (a->flags & 0x10) == 0) && (b->kind != 0 || a->kind != 1 || (b->flags & 0x10) == 0)) {
            k = 0.1f;
            if ((b->flags & 4) != 0 && (a->flags & 8) == 0) {
                k = 0.5f;
            }
            a->u.circle.vel->x = a->u.circle.vel->x + a->u.circle.weight * (k * (nx * half));
            a->u.circle.vel->z = a->u.circle.vel->z + a->u.circle.weight * (k * (nz * half));
            k = 0.1f;
            if ((a->flags & 4) != 0 && (b->flags & 8) == 0) {
                k = 0.5f;
            }
            b->u.circle.vel->x = b->u.circle.vel->x - b->u.circle.weight * (k * (nx * half));
            b->u.circle.vel->z = b->u.circle.vel->z - b->u.circle.weight * (k * (nz * half));
        }
        if (a->hit != NULL) {
            a->hit(a, b);
        }
        if (b->hit != NULL) {
            b->hit(b, a);
        }
        return 1;
    }
    return 0;
}

s32 fn_1_11880(M580_BODY *body, M580_BODY *wall)
{
    s32 hit = 0;
    M580_LINE *line = wall->u.wall.lines;
    HuVecF *prev = &body->u.circle.prev;
    HuVecF *pos = body->u.circle.pos;
    f32 radius = body->u.circle.radius;
    f32 dPrev;
    f32 dFar;
    f32 dPos;
    f32 farX;
    f32 farZ;
    f32 sum;
    f32 ix;
    f32 iz;
    f32 vx;
    f32 vz;
    f32 vlen;
    f32 vscale;
    HuVecF isect;
    HuVecF refl;
    f32 ex0;
    f32 ez0;
    f32 ex1;
    f32 ez1;
    s32 i;

    for (i = 0; i < wall->u.wall.lineNum; i++, line++) {
        dPrev = line->d + (line->n.x * prev->x + line->n.z * prev->z);
        if (!(dPrev < 0.0f)) {
            farX = pos->x - line->n.x * radius;
            farZ = pos->z - line->n.z * radius;
            dFar = line->d + (line->n.x * farX + line->n.z * farZ);
            dPos = line->d + (line->n.x * pos->x + line->n.z * pos->z);
            if (!(dFar >= 0.0f && dPos >= 0.0f)) {
                dPos = -dPos;
                sum = dPrev + dPos;
                if (0.0 != sum) {
                    isect.x = pos->x + (dPos * (prev->x - pos->x)) / sum;
                    isect.z = pos->z + (dPos * (prev->z - pos->z)) / sum;
                    ex0 = isect.x - line->p[0].x;
                    ez0 = isect.z - line->p[0].z;
                    ex1 = isect.x - line->p[1].x;
                    ez1 = isect.z - line->p[1].z;
                    if (!((ex0 * ex1 + ez0 * ez1) > 0.0f)) {
                        isect.x = pos->x + 100.0 * (3.0 * line->n.x);
                        isect.z = pos->z + 100.0 * (3.0 * line->n.z);
                        dPrev = line->d + (line->n.x * isect.x + line->n.z * isect.z);
                        sum = dPrev + dPos;
                        if (0.0 != sum) {
                            pos->x = pos->x + ((dPos * (isect.x - pos->x)) / sum + line->n.x * radius);
                            pos->z = pos->z + ((dPos * (isect.z - pos->z)) / sum + line->n.z * radius);
                            hit = 1;
                            if (body->hit != NULL) {
                                body->hit(body, wall);
                            }
                            if (wall->hit == NULL || (wall->hitLine = line, wall->hit(wall, body) == 0)) {
                                vlen = PSVECMag(body->u.circle.vel);
                                if (0.0 != vlen) {
                                    if (vlen > 100.00001f) {
                                        vlen = 100.00001f;
                                    }
                                    C_VECReflect(body->u.circle.vel, &line->n, &refl);
                                    if (!M580_ISNAN(refl.x) && !M580_ISNAN(refl.z)) {
                                        vscale = 0.8 * vlen;
                                        body->u.circle.vel->x = refl.x * vscale;
                                        body->u.circle.vel->z = refl.z * vscale;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return hit;
}

s32 fn_1_11DC4(M580_BODY *body)
{
    M580ReflectData *data = lbl_1_bss_258->data;
    s32 i;

    for (i = 0; i < M580_BODY_MAX; i++) {
        if (data->bodies[i] == NULL) {
            data->bodies[i] = body;
            return i;
        }
    }
    return -1;
}

s32 fn_1_11E3C(M580_BODY *body)
{
    M580ReflectData *data = lbl_1_bss_258->data;
    s32 i;

    for (i = 0; i < M580_BODY_MAX; i++) {
        if (data->walls[i] == NULL) {
            data->walls[i] = body;
            return i + M580_BODY_MAX;
        }
    }
    return -1;
}

void fn_1_11EB4(s32 id)
{
    M580ReflectData *data = lbl_1_bss_258->data;

    if (id >= 0) {
        if (id < M580_BODY_MAX) {
            data->bodies[id] = NULL;
        }
        id -= M580_BODY_MAX;
        if (id < M580_BODY_MAX) {
            data->walls[id] = NULL;
        }
    }
}

M580_BODY *fn_1_11F18(s32 id)
{
    M580ReflectData *data = lbl_1_bss_258->data;

    if (id < 0) {
        return NULL;
    }
    if (id < M580_BODY_MAX) {
        return data->bodies[id];
    }
    id -= M580_BODY_MAX;
    if (id < M580_BODY_MAX) {
        return data->walls[id];
    }
    return NULL;
}

void fn_1_11F88(M580_BODY *body, HSFOBJECT *hsfObj, s32 objNum)
{
    s32 quadNum = 0;
    HSFOBJECT *obj = hsfObj;
    s32 n = objNum;
    HSFFACE *face;
    s32 faceNum;
    M580_SRCLINE *src;
    HSFTRANSFORM **objs;
    Mtx *mtx;
    HuVecF *vtx;
    s32 k;

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
    src = HuMemDirectMallocNum(HEAP_HEAP, quadNum * sizeof(M580_SRCLINE), HU_MEMNUM_OVL);
    body->u.wall.src = src;
    memset(src, 0, quadNum * sizeof(M580_SRCLINE));
    body->u.wall.lines = HuMemDirectMallocNum(HEAP_HEAP, quadNum * sizeof(M580_LINE), HU_MEMNUM_OVL);
    memset(body->u.wall.lines, 0, quadNum * sizeof(M580_LINE));
    body->u.wall.prevLines = HuMemDirectMallocNum(HEAP_HEAP, quadNum * sizeof(M580_LINE), HU_MEMNUM_OVL);
    memset(body->u.wall.prevLines, 0, quadNum * sizeof(M580_LINE));
    body->u.wall.objNum = objNum;
    objs = HuMemDirectMallocNum(HEAP_HEAP, body->u.wall.objNum * sizeof(HSFTRANSFORM *), HU_MEMNUM_OVL);
    body->u.wall.objs = objs;
    memset(objs, 0, body->u.wall.objNum * sizeof(HSFTRANSFORM *));
    mtx = HuMemDirectMallocNum(HEAP_HEAP, body->u.wall.objNum * sizeof(Mtx), HU_MEMNUM_OVL);
    body->u.wall.mtx = mtx;
    memset(mtx, 0, body->u.wall.objNum * sizeof(Mtx));
    obj = hsfObj;
    n = objNum;
    body->u.wall.lineNum = 0;
    while (n-- != 0) {
        if (obj->type == 2) {
            vtx = obj->mesh.vertex->data;
            face = obj->mesh.face->data;
            faceNum = obj->mesh.face->count;
            while (faceNum-- != 0) {
                if (face->type == 3) {
                    for (k = 0; k < 4;) {
                        HuVecF *a = &vtx[face->index[lbl_1_data_8A8[k]].vertex];
                        HuVecF *b = &vtx[face->index[lbl_1_data_8A8[(k + 1) & 3]].vertex];

                        if (a->y < 50.0f && b->y < 50.0f) {
                            src->p[0].x = a->x;
                            src->p[0].z = a->z;
                            src->p[1].x = b->x;
                            src->p[1].z = b->z;
                            src->mtx = mtx;
                            src++;
                            body->u.wall.lineNum++;
                            break;
                        } else {
                            k++;
                        }
                    }
                }
                face++;
            }
        }
        *objs = &obj->mesh.base;
        objs++;
        mtx++;
        obj++;
    }
}

void fn_1_12248(M580_BODY *body, s16 modelId)
{
    HSFDATA *hsf = Hu3DData[modelId].hsf;

    fn_1_11F88(body, hsf->object, hsf->objectNum);
}

void fn_1_1229C(M580_BODY *body, HSFOBJECT *obj)
{
    fn_1_11F88(body, obj, 1);
}

void fn_1_122D0(M580_BODY *body)
{
    HSFTRANSFORM **objs = body->u.wall.objs;
    Mtx *mtx = body->u.wall.mtx;
    M580_SRCLINE *src = body->u.wall.src;
    M580_LINE *line = body->u.wall.prevLines;
    M580_LINE *tmp;
    Mtx m0;
    Mtx m1;
    Mtx m2;
    f32 fx;
    f32 fz;
    f64 len;
    s32 i;

    tmp = body->u.wall.lines;
    body->u.wall.lines = body->u.wall.prevLines;
    body->u.wall.prevLines = tmp;
    line = tmp = body->u.wall.lines;
    for (i = 0; i < body->u.wall.objNum; i++, objs++, mtx++) {
        PSMTXRotRad(m1, 'z', DEG2RAD_F * (*objs)->rot.z);
        PSMTXRotRad(m2, 'y', DEG2RAD_F * (*objs)->rot.y);
        PSMTXConcat(m1, m2, m0);
        PSMTXRotRad(m1, 'x', DEG2RAD_F * (*objs)->rot.x);
        PSMTXConcat(m0, m1, m2);
        PSMTXTrans(m1, (*objs)->pos.x, (*objs)->pos.y, (*objs)->pos.z);
        PSMTXConcat(m1, m2, m0);
        PSMTXScale(m1, (*objs)->scale.x, (*objs)->scale.y, (*objs)->scale.z);
        PSMTXConcat(m0, m1, *mtx);
    }
    for (i = 0; i < body->u.wall.lineNum; i++, line++, src++) {
        PSMTXMultVec(*src->mtx, &src->p[0], &line->p[0]);
        PSMTXMultVec(*src->mtx, &src->p[1], &line->p[1]);
        fz = line->p[1].z - line->p[0].z;
        fx = line->p[1].x - line->p[0].x;
        len = sqrt((f64)fz * (f64)fz + (f64)fx * (f64)fx);
        if (0.0f != (f32)len) {
            fz = fz / len;
            fx = fx / len;
        }
        line->n.x = -fz;
        line->n.y = 0.0f;
        line->n.z = fx;
        line->d = -(-fz * line->p[0].x + fx * line->p[0].z);
    }
}

s32 fn_1_126C8(M580_BODY *body, HuVecF *pos)
{
    M580_LINE *line = body->u.wall.lines;
    s32 i;

    for (i = 0; i < body->u.wall.lineNum; i++, line++) {
        if (line->d + (line->n.x * pos->x + line->n.z * pos->z) < 0.0f) {
            return 0;
        }
    }
    return 1;
}
