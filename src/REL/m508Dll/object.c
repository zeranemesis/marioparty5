/*
 * m508 object library: 2D collision helpers, game objects (process driven by
 * a table of mode functions), camera, wipe/message helpers and windows.
 */

#include <math.h>

#include "REL/m508Dll/object.h"

#define M508_CUROBJ ((M508_OBJ *)HuPrcCurrentGet()->property)
#define M508_CURCAMERA ((M508_CAMERA *)HuPrcCurrentGet()->property)

static void fn_1_BDF0(void);
static void fn_1_CD4C(void);
static void fn_1_E32C(void);

static M508_CIRCLE lbl_1_bss_220;
static float lbl_1_bss_22C;
static float lbl_1_bss_230;
static M508_VEC2 lbl_1_bss_234;
static M508_VEC2 lbl_1_bss_23C;
static M508_VEC2 lbl_1_bss_244;
static M508_VEC2 lbl_1_bss_24C;
static M508_VEC2 lbl_1_bss_254;
static M508_VEC2 lbl_1_bss_25C;
static float lbl_1_bss_264;
static float lbl_1_bss_268;
static float lbl_1_bss_26C;
static float lbl_1_bss_270;
static M508_POLY lbl_1_bss_274;
static M508_VEC2 lbl_1_bss_27C[5];
static float lbl_1_bss_2A4;
static float lbl_1_bss_2A8;
static M508_VEC2 lbl_1_bss_2AC;
static M508_VEC2 lbl_1_bss_2B4;
static M508_VEC2 lbl_1_bss_2BC;
static M508_VEC2 lbl_1_bss_2C4;
static M508_VEC2 lbl_1_bss_2CC;
static M508_VEC2 lbl_1_bss_2D4;

static M508_CAMERAFUNC lbl_1_data_B0[] = {
    fn_1_D164,
    fn_1_D6A0,
    fn_1_D0A4
};

/* Length of a 2D vector */
// NONMATCHING: the original inlines a different sqrtf (with NaN/inf cases)
float fn_1_A194(M508_VEC2 *vec)
{
    return sqrtf((vec->x*vec->x)+(vec->y*vec->y));
}

/* dst = src normalized */
// NONMATCHING: the original inlines a different sqrtf (with NaN/inf cases)
void fn_1_A314(M508_VEC2 *dst, M508_VEC2 *src)
{
    float len = sqrtf((src->x*src->x)+(src->y*src->y));
    float inv = 1.0f/len;
    dst->x = src->x*inv;
    dst->y = src->y*inv;
}

/* Point inside circle */
// NONMATCHING: the original materialises the result through a temp
BOOL fn_1_A4DC(M508_VEC2 *point, M508_CIRCLE *circle)
{
    float dx = point->x-circle->x;
    float dy = point->y-circle->y;
    float r = circle->r;
    if((dx*dx)+(dy*dy) > r*r) {
        return FALSE;
    } else {
        return TRUE;
    }
}

/* Circle overlaps circle */
// NONMATCHING: the original materialises the result through a temp
BOOL fn_1_A564(M508_CIRCLE *a, M508_CIRCLE *b)
{
    float dx = a->x-b->x;
    float dy = a->y-b->y;
    float r = a->r+b->r;
    if((dx*dx)+(dy*dy) < r*r) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/* Rect overlaps rect */
BOOL fn_1_A5F4(M508_RECT *a, M508_RECT *b)
{
    if(a->x1 > b->x2) {
        return FALSE;
    }
    if(a->x2 < b->x1) {
        return FALSE;
    }
    if(a->y1 > b->y2) {
        return FALSE;
    }
    if(a->y2 < b->y1) {
        return FALSE;
    }
    return TRUE;
}

/* Point inside polygon (all edge cross products of the same sign) */
BOOL fn_1_A65C(M508_VEC2 *point, M508_POLY *poly)
{
    M508_VEC2 *p0;
    M508_VEC2 *p1;
    int negCount = 0;
    int cross = 0;
    int edgeNum = poly->num-1;
    int i;
    BOOL inside;
    for(i=0; i<edgeNum; i++) {
        p0 = &poly->point[i];
        p1 = &poly->point[i+1];
        cross = ((point->x-p0->x)*(point->y-p1->y))-((point->y-p0->y)*(point->x-p1->x));
        if(cross < 0) {
            negCount++;
        }
    }
    inside = TRUE;
    if(negCount != 0 && negCount != edgeNum) {
        inside = FALSE;
    }
    return inside ? TRUE : FALSE;
}

/* Circle touches line segment */
// NONMATCHING: sqrtf/inlined point tests are scheduled differently
BOOL fn_1_A74C(M508_CIRCLE *circle, M508_RECT *line)
{
    float len;
    float inv;
    M508_VEC2 p1;
    M508_VEC2 p2;
    p1.x = line->x1;
    p1.y = line->y1;
    p2.x = line->x2;
    p2.y = line->y2;
    if(fn_1_A4DC(&p1, circle)) {
        return TRUE;
    }
    if(fn_1_A4DC(&p2, circle)) {
        return TRUE;
    }
    lbl_1_bss_2D4.x = line->x2-line->x1;
    lbl_1_bss_2D4.y = line->y2-line->y1;
    lbl_1_bss_2CC.x = circle->x-line->x1;
    lbl_1_bss_2CC.y = circle->y-line->y1;
    lbl_1_bss_2A8 = (lbl_1_bss_2D4.x*lbl_1_bss_2CC.y)-(lbl_1_bss_2D4.y*lbl_1_bss_2CC.x);
    if(lbl_1_bss_2A8 == 0.0f) {
        lbl_1_bss_2AC.x = circle->x;
        lbl_1_bss_2AC.y = circle->y;
    } else {
        lbl_1_bss_2C4.x = lbl_1_bss_2D4.y*lbl_1_bss_2A8;
        lbl_1_bss_2C4.y = -(lbl_1_bss_2D4.x*lbl_1_bss_2A8);
        len = sqrtf((lbl_1_bss_2C4.x*lbl_1_bss_2C4.x)+(lbl_1_bss_2C4.y*lbl_1_bss_2C4.y));
        inv = 1.0f/len;
        lbl_1_bss_2BC.x = lbl_1_bss_2C4.x*inv;
        lbl_1_bss_2BC.y = lbl_1_bss_2C4.y*inv;
        lbl_1_bss_2B4.x = line->x1-circle->x;
        lbl_1_bss_2B4.y = line->y1-circle->y;
        lbl_1_bss_2A4 = (lbl_1_bss_2BC.x*lbl_1_bss_2B4.x)+(lbl_1_bss_2BC.y*lbl_1_bss_2B4.y);
        if(lbl_1_bss_2A4 > circle->r) {
            return FALSE;
        }
        lbl_1_bss_2AC.x = circle->x+(lbl_1_bss_2BC.x*lbl_1_bss_2A4);
        lbl_1_bss_2AC.y = circle->y+(lbl_1_bss_2BC.y*lbl_1_bss_2A4);
    }
    if(line->x1 < lbl_1_bss_2AC.x && lbl_1_bss_2AC.x < line->x2) {
        return TRUE;
    }
    if(line->x2 < lbl_1_bss_2AC.x && lbl_1_bss_2AC.x < line->x1) {
        return TRUE;
    }
    if(line->y1 < lbl_1_bss_2AC.y && lbl_1_bss_2AC.y < line->y2) {
        return TRUE;
    }
    if(line->y2 < lbl_1_bss_2AC.y && lbl_1_bss_2AC.y < line->y1) {
        return TRUE;
    }
    return FALSE;
}

/* Circle touches polygon */
// NONMATCHING: inlined fn_1_A65C is scheduled differently
BOOL fn_1_AD98(M508_CIRCLE *circle, M508_POLY *poly)
{
    M508_RECT edge;
    M508_VEC2 center;
    int edgeNum;
    int i;
    center.x = circle->x;
    center.y = circle->y;
    if(fn_1_A65C(&center, poly)) {
        return TRUE;
    }
    edgeNum = poly->num-1;
    for(i=0; i<edgeNum; i++) {
        edge.x1 = poly->point[i].x;
        edge.y1 = poly->point[i].y;
        edge.x2 = poly->point[i+1].x;
        edge.y2 = poly->point[i+1].y;
        if(fn_1_A74C(circle, &edge)) {
            return TRUE;
        }
    }
    return FALSE;
}

/* Circle touches rect */
// NONMATCHING: inlined fn_1_A65C is scheduled differently
BOOL fn_1_AF28(M508_RECT *rect, M508_CIRCLE *circle)
{
    M508_RECT edge;
    M508_VEC2 center;
    int edgeNum;
    int i;
    lbl_1_bss_27C[0].x = rect->x1;
    lbl_1_bss_27C[0].y = rect->y1;
    lbl_1_bss_27C[1].x = rect->x2;
    lbl_1_bss_27C[1].y = rect->y1;
    lbl_1_bss_27C[2].x = rect->x2;
    lbl_1_bss_27C[2].y = rect->y2;
    lbl_1_bss_27C[3].x = rect->x1;
    lbl_1_bss_27C[3].y = rect->y2;
    lbl_1_bss_27C[4].x = rect->x1;
    lbl_1_bss_27C[4].y = rect->y1;
    lbl_1_bss_274.num = 5;
    lbl_1_bss_274.point = lbl_1_bss_27C;
    center.x = circle->x;
    center.y = circle->y;
    if(fn_1_A65C(&center, &lbl_1_bss_274)) {
        return TRUE;
    }
    edgeNum = lbl_1_bss_274.num-1;
    for(i=0; i<edgeNum; i++) {
        edge.x1 = lbl_1_bss_274.point[i].x;
        edge.y1 = lbl_1_bss_274.point[i].y;
        edge.x2 = lbl_1_bss_274.point[i+1].x;
        edge.y2 = lbl_1_bss_274.point[i+1].y;
        if(fn_1_A74C(circle, &edge)) {
            return TRUE;
        }
    }
    return FALSE;
}

/* Sphere overlaps sphere */
// NONMATCHING: the original materialises the result through a temp
BOOL fn_1_B1C0(M508_SPHERE *a, M508_SPHERE *b)
{
    float dx = a->x-b->x;
    float dy = a->y-b->y;
    float dz = a->z-b->z;
    float r = a->r+b->r;
    if((dz*dz)+((dx*dx)+(dy*dy)) < r*r) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/* Pushes circle out of a fixed circle */
BOOL fn_1_B274(M508_CIRCLE *push, M508_CIRCLE *fixed)
{
    float len2;
    lbl_1_bss_270 = push->x-fixed->x;
    lbl_1_bss_26C = push->y-fixed->y;
    lbl_1_bss_268 = push->r+fixed->r;
    len2 = (lbl_1_bss_270*lbl_1_bss_270)+(lbl_1_bss_26C*lbl_1_bss_26C);
    lbl_1_bss_264 = sqrtf(len2);
    if(lbl_1_bss_264 < lbl_1_bss_268) {
        if(lbl_1_bss_264 == 0.0f) {
            lbl_1_bss_270 = 0.0f;
            lbl_1_bss_26C = -1.0f;
        } else {
            lbl_1_bss_270 /= lbl_1_bss_264;
            lbl_1_bss_26C /= lbl_1_bss_264;
        }
        push->x = fixed->x+(lbl_1_bss_270*lbl_1_bss_268);
        push->y = fixed->y+(lbl_1_bss_26C*lbl_1_bss_268);
        return TRUE;
    }
    return FALSE;
}

/* Pushes circle out of a line segment */
// NONMATCHING: inlined sqrtf/fn_1_B274 are scheduled differently
BOOL fn_1_B514(M508_CIRCLE *push, M508_RECT *line)
{
    float len;
    float inv;
    lbl_1_bss_25C.x = line->x2-line->x1;
    lbl_1_bss_25C.y = line->y2-line->y1;
    lbl_1_bss_254.x = push->x-line->x1;
    lbl_1_bss_254.y = push->y-line->y1;
    lbl_1_bss_230 = (lbl_1_bss_25C.x*lbl_1_bss_254.y)-(lbl_1_bss_25C.y*lbl_1_bss_254.x);
    if(lbl_1_bss_230 == 0.0f) {
        lbl_1_bss_234.x = push->x;
        lbl_1_bss_234.y = push->y;
    } else {
        lbl_1_bss_24C.x = lbl_1_bss_25C.y*lbl_1_bss_230;
        lbl_1_bss_24C.y = -(lbl_1_bss_25C.x*lbl_1_bss_230);
        len = sqrtf((lbl_1_bss_24C.x*lbl_1_bss_24C.x)+(lbl_1_bss_24C.y*lbl_1_bss_24C.y));
        inv = 1.0f/len;
        lbl_1_bss_244.x = lbl_1_bss_24C.x*inv;
        lbl_1_bss_244.y = lbl_1_bss_24C.y*inv;
        lbl_1_bss_23C.x = line->x1-push->x;
        lbl_1_bss_23C.y = line->y1-push->y;
        lbl_1_bss_22C = (lbl_1_bss_244.x*lbl_1_bss_23C.x)+(lbl_1_bss_244.y*lbl_1_bss_23C.y);
        if(lbl_1_bss_22C > push->r) {
            return FALSE;
        }
        lbl_1_bss_234.x = push->x+(lbl_1_bss_244.x*lbl_1_bss_22C);
        lbl_1_bss_234.y = push->y+(lbl_1_bss_244.y*lbl_1_bss_22C);
    }
    lbl_1_bss_220.x = lbl_1_bss_234.x;
    lbl_1_bss_220.y = lbl_1_bss_234.y;
    lbl_1_bss_220.r = 1.0f;
    return fn_1_B274(push, &lbl_1_bss_220) ? TRUE : FALSE;
}

M508_OBJ *fn_1_BC7C(int prio, u32 stackSize, int funcNum, M508_OBJFUNC *funcTbl)
{
    M508_OBJ *obj;
    int i;
    obj = HuMemDirectMallocNum(HEAP_HEAP, M508_OBJ_SIZE, HU_MEMNUM_OVL);
    obj->mode = 0;
    obj->prevMode = 0;
    obj->modeNum = funcNum;
    obj->unkC = 0;
    obj->modeFunc = HuMemDirectMallocNum(HEAP_HEAP, funcNum*sizeof(M508_OBJFUNC), HU_MEMNUM_OVL);
    for(i=0; i<funcNum; i++) {
        obj->modeFunc[i] = fn_1_BE58;
    }
    obj->process = HuPrcChildCreate(fn_1_BDF0, prio, stackSize, 0, HuPrcCurrentGet());
    obj->process->property = obj;
    if(funcTbl != NULL) {
        for(i=0; i<funcNum; i++) {
            if(funcTbl[i] != NULL) {
                obj->modeFunc[i] = funcTbl[i];
            }
        }
    }
    return obj;
}

void fn_1_BDB0(M508_OBJ *obj)
{
    HuMemDirectFree(obj->modeFunc);
    HuPrcKill(obj->process);
    HuMemDirectFree(obj);
}

static void fn_1_BDF0(void)
{
    M508_OBJ *obj = M508_CUROBJ;
    while(obj->mode < obj->modeNum) {
        obj->modeFunc[obj->mode](obj);
        obj->prevMode = obj->mode;
    }
    HuPrcEnd();
}

void fn_1_BE58(M508_OBJ *obj)
{
    while(1) {
        HuPrcSleep(0);
    }
}

M508_OBJ *fn_1_BE70(int prio, u32 stackSize, u32 dataNum, int motNum, u32 *motTbl, int funcNum, M508_OBJFUNC *funcTbl)
{
    M508_OBJ *obj;
    int i;
    obj = HuMemDirectMallocNum(HEAP_HEAP, M508_MODELOBJ_SIZE, HU_MEMNUM_OVL);
    obj->mode = 0;
    obj->prevMode = 0;
    obj->modeNum = funcNum;
    obj->dispF = TRUE;
    obj->pos.x = 0;
    obj->pos.y = 0;
    obj->pos.z = 0;
    obj->rot.x = 0;
    obj->rot.y = 0;
    obj->rot.z = 0;
    obj->scale.x = 1;
    obj->scale.y = 1;
    obj->scale.z = 1;
    obj->modelId = HU3D_MODELID_NONE;
    obj->unkC = 0;
    obj->motNum = motNum;
    obj->motId = HuMemDirectMallocNum(HEAP_HEAP, motNum*sizeof(HU3DMOTID), HU_MEMNUM_OVL);
    for(i=0; i<motNum; i++) {
        obj->motId[i] = HU3D_MOTID_NONE;
    }
    obj->modeFunc = HuMemDirectMallocNum(HEAP_HEAP, funcNum*sizeof(M508_OBJFUNC), HU_MEMNUM_OVL);
    for(i=0; i<funcNum; i++) {
        obj->modeFunc[i] = fn_1_BE58;
    }
    obj->process = HuPrcChildCreate(fn_1_BDF0, prio, stackSize, 0, HuPrcCurrentGet());
    obj->process->property = obj;
    if(dataNum != HU_DATANUM_NONE) {
        obj->modelId = Hu3DModelCreateData(dataNum);
    }
    for(i=0; i<motNum; i++) {
        if(motTbl[i] != HU_DATANUM_NONE) {
            obj->motId[i] = Hu3DJointMotionData(obj->modelId, motTbl[i]);
        }
    }
    if(funcTbl != NULL) {
        for(i=0; i<funcNum; i++) {
            if(funcTbl[i] != NULL) {
                obj->modeFunc[i] = funcTbl[i];
            }
        }
    }
    return obj;
}

void fn_1_C0F4(M508_OBJ *obj)
{
    int i;
    for(i=0; i<obj->motNum; i++) {
        if(obj->motId[i] != HU3D_MOTID_NONE) {
            Hu3DMotionKill(obj->motId[i]);
        }
    }
    HuMemDirectFree(obj->motId);
    if(obj->modelId != HU3D_MODELID_NONE) {
        Hu3DModelKill(obj->modelId);
    }
    HuMemDirectFree(obj->modeFunc);
    HuPrcKill(obj->process);
    HuMemDirectFree(obj);
}

void fn_1_C194(M508_OBJ *obj, float x, float y, float z)
{
    Hu3DModelPosSet(obj->modelId, obj->pos.x = x, obj->pos.y = y, obj->pos.z = z);
}

void fn_1_C1E8(M508_OBJ *obj, float *x, float *y, float *z)
{
    if(x != NULL) {
        *x = obj->pos.x;
    }
    if(y != NULL) {
        *y = obj->pos.y;
    }
    if(z != NULL) {
        *z = obj->pos.z;
    }
}

void fn_1_C228(M508_OBJ *obj, float x, float y, float z)
{
    Hu3DModelRotSet(obj->modelId, obj->rot.x = x, obj->rot.y = y, obj->rot.z = z);
}

void fn_1_C27C(M508_OBJ *obj, float *x, float *y, float *z)
{
    if(x != NULL) {
        *x = obj->rot.x;
    }
    if(y != NULL) {
        *y = obj->rot.y;
    }
    if(z != NULL) {
        *z = obj->rot.z;
    }
}

void fn_1_C2BC(M508_OBJ *obj, float x, float y, float z)
{
    Hu3DModelScaleSet(obj->modelId, obj->scale.x = x, obj->scale.y = y, obj->scale.z = z);
}

void fn_1_C310(M508_OBJ *obj, float *x, float *y, float *z)
{
    if(x != NULL) {
        *x = obj->scale.x;
    }
    if(y != NULL) {
        *y = obj->scale.y;
    }
    if(z != NULL) {
        *z = obj->scale.z;
    }
}

void fn_1_C350(M508_OBJ *obj, float tpLvl)
{
    Hu3DModelTPLvlSet(obj->modelId, obj->tpLvl = tpLvl);
}

float fn_1_C38C(M508_OBJ *obj)
{
    return obj->tpLvl;
}

void fn_1_C394(M508_OBJ *obj)
{
    obj->dispF = TRUE;
    Hu3DModelAttrReset(obj->modelId, HU3D_ATTR_DISPOFF);
}

void fn_1_C3D0(M508_OBJ *obj)
{
    obj->dispF = FALSE;
    Hu3DModelAttrSet(obj->modelId, HU3D_ATTR_DISPOFF);
}

BOOL fn_1_C40C(M508_OBJ *obj)
{
    return obj->dispF;
}

void fn_1_C414(M508_OBJ *obj, int time)
{
    Hu3DMotionTimeSet(obj->modelId, time);
}

void fn_1_C470(M508_OBJ *obj, BOOL loopF)
{
    HU3DMODELID modelId = obj->modelId;
    Hu3DModelAttrReset(modelId, HU3D_MOTATTR_PAUSE);
    if(loopF) {
        Hu3DModelAttrSet(modelId, HU3D_MOTATTR_LOOP);
    } else {
        Hu3DModelAttrReset(modelId, HU3D_MOTATTR_LOOP);
    }
}

void fn_1_C4E8(M508_OBJ *obj)
{
    Hu3DModelAttrSet(obj->modelId, HU3D_MOTATTR_PAUSE);
}

BOOL fn_1_C520(M508_OBJ *obj)
{
    return (!Hu3DMotionEndCheck(obj->modelId)) ? TRUE : FALSE;
}

void fn_1_C570(M508_OBJ *obj, int motNo, float start, float end, u32 attr)
{
    obj->motNo = motNo;
    if(motNo == -1) {
        Hu3DMotionSet(obj->modelId, HU3D_MOTID_NONE);
    } else {
        Hu3DMotionShiftSet(obj->modelId, obj->motId[motNo], start, end, attr);
    }
}

int fn_1_C5EC(M508_OBJ *obj)
{
    return obj->motNo;
}

void fn_1_C5F4(M508_OBJ *obj, float speed)
{
    Hu3DMotionSpeedSet(obj->modelId, obj->motSpeed = speed);
}

void fn_1_C630(M508_OBJ *obj)
{
    Hu3DModelShadowSet(obj->modelId);
}

void fn_1_C660(M508_OBJ *obj)
{
    Hu3DModelShadowReset(obj->modelId);
}

void fn_1_C690(M508_OBJ *obj)
{
    Hu3DModelShadowMapSet(obj->modelId);
}

void fn_1_C6C0(M508_OBJ *obj)
{
}

void fn_1_C6C4(M508_OBJ *obj, char *objName, M508_OBJ *hookObj)
{
    Hu3DModelHookSet(obj->modelId, objName, hookObj->modelId);
}

void fn_1_C70C(M508_OBJ *obj)
{
    Hu3DModelHookReset(obj->modelId);
}

M508_OBJ *fn_1_C73C(int prio, u32 stackSize, int playerNo, int motNum, u32 *motTbl, int funcNum, M508_OBJFUNC *funcTbl, s16 modelNo)
{
    M508_OBJ *obj;
    int i;
    obj = HuMemDirectMallocNum(HEAP_HEAP, M508_CHAROBJ_SIZE, HU_MEMNUM_OVL);
    obj->mode = 0;
    obj->prevMode = 0;
    obj->modeNum = funcNum;
    obj->dispF = TRUE;
    obj->pos.x = 0;
    obj->pos.y = 0;
    obj->pos.z = 0;
    obj->rot.x = 0;
    obj->rot.y = 0;
    obj->rot.z = 0;
    obj->scale.x = 1;
    obj->scale.y = 1;
    obj->scale.z = 1;
    obj->modelId = HU3D_MODELID_NONE;
    obj->comF = (GwPlayerConf[playerNo].type != GW_TYPE_MAN) ? TRUE : FALSE;
    obj->charNo = GwPlayerConf[playerNo].charNo;
    obj->padNo = GwPlayerConf[playerNo].padNo;
    obj->dif = GwPlayerConf[playerNo].dif;
    obj->playerNo = playerNo;
    obj->unkC = 0;
    obj->motNum = motNum;
    obj->motId = HuMemDirectMallocNum(HEAP_HEAP, motNum*sizeof(HU3DMOTID), HU_MEMNUM_OVL);
    for(i=0; i<motNum; i++) {
        obj->motId[i] = HU3D_MOTID_NONE;
    }
    obj->modeFunc = HuMemDirectMallocNum(HEAP_HEAP, funcNum*sizeof(M508_OBJFUNC), HU_MEMNUM_OVL);
    for(i=0; i<funcNum; i++) {
        obj->modeFunc[i] = fn_1_BE58;
    }
    obj->process = HuPrcChildCreate(fn_1_BDF0, prio, stackSize, 0, HuPrcCurrentGet());
    obj->process->property = obj;
    obj->modelId = CharModelCreate(obj->charNo, modelNo);
    for(i=0; i<motNum; i++) {
        int motIdx = i+(obj->charNo*13);
        if(motTbl[motIdx] != HU_DATANUM_NONE) {
            obj->motId[i] = CharMotionCreate(obj->charNo, motTbl[i]);
        }
    }
    CharMotionDataClose(obj->charNo);
    for(i=0; i<funcNum; i++) {
        if(funcTbl[i] != NULL) {
            obj->modeFunc[i] = funcTbl[i];
        }
    }
    return obj;
}

void fn_1_CA2C(M508_OBJ *obj)
{
    int i;
    for(i=0; i<obj->motNum; i++) {
        if(obj->motId[i] != HU3D_MOTID_NONE) {
            Hu3DMotionKill(obj->motId[i]);
        }
    }
    HuMemDirectFree(obj->motId);
    if(obj->modelId != HU3D_MODELID_NONE) {
        Hu3DModelKill(obj->modelId);
    }
    HuMemDirectFree(obj->modeFunc);
    HuPrcKill(obj->process);
    HuMemDirectFree(obj);
}

/* The original assigns charNo = 0 instead of comparing it: always takes the else branch */
void fn_1_CACC(M508_OBJ *obj, char *objName)
{
    if(obj->charNo = 0) {
        CharModelHookDustCreate(obj->charNo, objName);
    } else {
        Hu3DModelHookReset(obj->modelId);
    }
}

M508_CAMERA *fn_1_CB24(s16 cameraBit)
{
    M508_CAMERA *camera;
    M508_CAMERAVIEW *view;
    int i;
    camera = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M508_CAMERA), HU_MEMNUM_OVL);
    camera->cameraBit = cameraBit;
    for(i=0; i<3; i++) {
        view = &camera->view[i];
        view->center.x = 0;
        view->center.y = 0;
        view->center.z = 0;
        view->rot.x = 0;
        view->rot.y = 0;
        view->rot.z = 0;
        view->zoom = 0;
    }
    camera->process = HuPrcChildCreate(fn_1_CD4C, 0x1000, 0x2000, 0, HuPrcCurrentGet());
    camera->process->property = camera;
    camera->debugF = FALSE;
    Hu3DCameraCreate(cameraBit);
    fn_1_DA0C(camera, 30, 10, 8000, 1.2f);
    fn_1_D9AC(camera, 0, 0, 640, 480, 0, 1);
    fn_1_DA5C(camera, 0, 0, 640, 480);
    camera->mode = 2;
    return camera;
}

void fn_1_CD14(M508_CAMERA *camera)
{
    HuPrcKill(camera->process);
    HuMemDirectFree(camera);
}

static void fn_1_CD4C(void)
{
    M508_CAMERA *camera = M508_CURCAMERA;
    while(camera->mode != 3) {
        lbl_1_data_B0[camera->mode](camera);
    }
    HuPrcEnd();
}

void fn_1_CDB0(M508_CAMERA *camera)
{
    M508_CAMERAVIEW *view;
    int i;
    for(i=0; i<3; i++) {
        view = &camera->view[i];
        view->pos.x = view->center.x+(view->zoom*(HuSin(view->rot.y)*HuCos(view->rot.x)));
        view->pos.y = view->center.y+(view->zoom*(-HuSin(view->rot.x)));
        view->pos.z = view->center.z+(view->zoom*(HuCos(view->rot.y)*HuCos(view->rot.x)));
        view->up.x = HuSin(view->rot.y)*HuSin(view->rot.x);
        view->up.y = HuCos(view->rot.x);
        view->up.z = HuCos(view->rot.y)*HuSin(view->rot.x);
    }
}

void fn_1_D028(M508_CAMERA *camera)
{
    M508_CAMERAVIEW *view;
    if(camera->mode != 2) {
        view = &camera->view[0];
        Hu3DCameraPosSet(camera->cameraBit,
            view->pos.x, view->pos.y, view->pos.z,
            view->up.x, view->up.y, view->up.z,
            view->center.x, view->center.y, view->center.z);
    }
}

/* Sleeps tick frames while updating the camera, returns on a mode change */
#define M508_CAMERA_SLEEP(tick) \
do { \
    int i_; \
    int mode_; \
    int num_; \
    num_ = (tick); \
    for(i_=0; i_<num_; i_++) { \
        mode_ = M508_CURCAMERA->mode; \
        fn_1_D028(M508_CURCAMERA); \
        HuPrcVSleep(); \
        if(mode_ != M508_CURCAMERA->mode) { \
            return; \
        } \
    } \
} while(0)

void fn_1_D098(M508_CAMERA *camera)
{
    camera->mode = 2;
}

void fn_1_D0A4(M508_CAMERA *camera)
{
    while(1) {
        M508_CAMERA_SLEEP(1);
    }
}

void fn_1_D158(M508_CAMERA *camera)
{
    camera->mode = 0;
}

void fn_1_D164(M508_CAMERA *camera)
{
    while(1) {
        fn_1_DAF0(camera);
        fn_1_DF6C(camera);
        M508_CAMERA_SLEEP(1);
    }
}

BOOL fn_1_D22C(M508_CAMERA *camera)
{
    return (camera->mode != 0) ? TRUE : FALSE;
}

void fn_1_D25C(M508_CAMERA *camera, HuVecF *center, HuVecF *rot, float *zoom, int time)
{
    M508_CAMERAVIEW *view = &camera->view[0];
    M508_CAMERAVIEW *start = &camera->view[1];
    M508_CAMERAVIEW *end = &camera->view[2];
    *start = *view;
    *end = *view;
    if(center != NULL) {
        end->center = *center;
    }
    if(rot != NULL) {
        end->rot = *rot;
    }
    if(zoom != NULL) {
        end->zoom = *zoom;
    }
    fn_1_CDB0(camera);
    camera->time = time;
    if(time > 0) {
        camera->mode = 1;
    } else {
        *view = *end;
        camera->mode = 0;
    }
}

void fn_1_D6A0(M508_CAMERA *camera)
{
    M508_CAMERAVIEW *view = &camera->view[0];
    M508_CAMERAVIEW *start = &camera->view[1];
    M508_CAMERAVIEW *end = &camera->view[2];
    float step = 1.0f/camera->time;
    float t = 0;
    float s;
    while(1) {
        s = HuSin(90.0f*t);
        s = s*s;
        view->center.x = start->center.x+(s*(end->center.x-start->center.x));
        view->center.y = start->center.y+(s*(end->center.y-start->center.y));
        view->center.z = start->center.z+(s*(end->center.z-start->center.z));
        view->pos.x = start->pos.x+(s*(end->pos.x-start->pos.x));
        view->pos.y = start->pos.y+(s*(end->pos.y-start->pos.y));
        view->pos.z = start->pos.z+(s*(end->pos.z-start->pos.z));
        view->up.x = start->up.x+(s*(end->up.x-start->up.x));
        view->up.y = start->up.y+(s*(end->up.y-start->up.y));
        view->up.z = start->up.z+(s*(end->up.z-start->up.z));
        fn_1_DAF0(camera);
        fn_1_DF6C(camera);
        t += step;
        if(t >= 1.0f) {
            break;
        }
        M508_CAMERA_SLEEP(1);
    }
    *view = *end;
    camera->mode = 0;
}

void fn_1_D9AC(M508_CAMERA *camera, float vpX, float vpY, float vpW, float vpH, float vpNearZ, float vpFarZ)
{
    Hu3DCameraViewportSet(camera->cameraBit, vpX, vpY, vpW, vpH, vpNearZ, vpFarZ);
}

void fn_1_DA0C(M508_CAMERA *camera, float fov, float near, float far, float aspect)
{
    Hu3DCameraPerspectiveSet(camera->cameraBit, fov, near, far, aspect);
}

void fn_1_DA5C(M508_CAMERA *camera, float scissorX, float scissorY, float scissorW, float scissorH)
{
    Hu3DCameraScissorSet(camera->cameraBit, scissorX, scissorY, scissorW, scissorH);
}

void fn_1_DAF0(M508_CAMERA *camera)
{
    M508_CAMERAVIEW *view;
    if(camera->debugF && (HuPadBtn[0] & PAD_BUTTON_Y)) {
        view = &camera->view[0];
        view->center.x += 0.1f*HuPadSubStkX[0];
        view->center.z -= 0.02f*HuPadSubStkY[0];
        view->rot.y -= 0.1f*HuPadStkX[0];
        view->rot.x += 0.1f*HuPadStkY[0];
        view->zoom += 0.1f*HuPadTrigL[0];
        view->zoom -= 0.1f*HuPadTrigR[0];
        fn_1_CDB0(camera);
    }
}

void fn_1_DF6C(M508_CAMERA *camera)
{
}

void fn_1_DF70(int time, BOOL waitF)
{
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_PREVTYPE, time);
    while(waitF && WipeCheckEnd()) {
        HuPrcVSleep();
    }
}

void fn_1_DFCC(int time, BOOL waitF)
{
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_WHITEFADE, time);
    while(waitF && WipeCheckEnd()) {
        HuPrcVSleep();
    }
}

void fn_1_E028(int time)
{
    int i;
    GAMEMESID gameMesId = GameMesMgStartCreate();
    if(time == -1) {
        while(GameMesStatGet(gameMesId)) {
            HuPrcVSleep();
        }
    } else {
        for(i=0; i<time; i++) {
            HuPrcVSleep();
        }
    }
}

void fn_1_E0B4(int time)
{
    int i;
    GAMEMESID gameMesId = GameMesMgFinishCreate();
    if(time == -1) {
        while(GameMesStatGet(gameMesId)) {
            HuPrcVSleep();
        }
    } else {
        for(i=0; i<time; i++) {
            HuPrcVSleep();
        }
    }
}

float fn_1_E140(float start, float end, float t)
{
    float diff;
    float angle;
    diff = fmod(end-start, 360.0);
    if(diff < 0.0f) {
        diff += 360.0f;
    }
    if(diff > 180.0f) {
        diff -= 360.0f;
    }
    angle = fmod(start+(diff*t), 360.0);
    if(angle < 0.0f) {
        angle += 360.0f;
    }
    return angle;
}

M508_WIN *fn_1_E244(int w, int h)
{
    M508_WIN *win = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M508_WIN), HU_MEMNUM_OVL);
    win->process = HuPrcChildCreate(fn_1_E32C, 0x1000, 0x2000, 0, HuPrcCurrentGet());
    win->process->property = win;
    win->winId = HuWinExCreateFrame(0, 0, w, h, HUWIN_SPEAKER_NONE, 1);
    return win;
}

void fn_1_E2EC(M508_WIN *win)
{
    HuWinKill(win->winId);
    HuPrcKill(win->process);
    HuMemDirectFree(win);
}

static void fn_1_E32C(void)
{
    while(1) {
        HuPrcVSleep();
    }
}

void fn_1_E340(M508_WIN *win, float x, float y)
{
    HuWinPosSet(win->winId, win->x = x, win->y = y);
}

void fn_1_E388(M508_WIN *win, float *x, float *y)
{
    *x = win->x;
    *y = win->y;
}

void fn_1_E39C(M508_WIN *win, u32 mess)
{
    HuWinMesSet(win->winId, mess);
}

void fn_1_E3D4(M508_WIN *win)
{
    HuWinDispOn(win->winId);
    win->dispF = TRUE;
}

void fn_1_E40C(M508_WIN *win)
{
    HuWinDispOff(win->winId);
    win->dispF = FALSE;
}

void fn_1_E444(M508_WIN *win, int color)
{
    HuWinMesColSet(win->winId, color);
}

void fn_1_E480(M508_WIN *win, s16 speed)
{
    HuWinMesSpeedSet(win->winId, speed);
}

void fn_1_E4B8(M508_WIN *win)
{
    HuWinMesWait(win->winId);
}

void fn_1_E4E8(M508_WIN *win)
{
    HuWinExOpen(win->winId);
}

void fn_1_E518(M508_WIN *win)
{
    HuWinExClose(win->winId);
}

void fn_1_E548(M508_WIN *win, float scaleX, float scaleY)
{
    HuWinScaleSet(win->winId, scaleX, scaleY);
}
