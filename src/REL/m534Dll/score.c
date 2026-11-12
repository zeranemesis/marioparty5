#include "REL/m534Dll.h"

static void fn_1_6C78(OMOBJ *obj);
static void fn_1_6CBC(OMOBJ *obj);
static void fn_1_6CF4(OMOBJ *obj);
static void fn_1_6DEC(OMOBJ *obj);
static void fn_1_6EE0(OMOBJ *obj);
static void fn_1_6FE8(OMOBJ *obj);

/* per-team score board (OMOBJ data of fn_1_6BDC holds two of them), 0x40 bytes */
typedef struct M534Score_s {
    s32 state;          /* 0x00 */
    f32 t;              /* 0x04 */
    f32 radius;         /* 0x08 */
    f32 angle;          /* 0x0C */
    HuVecF pos;         /* 0x10 */
    HuVecF vel;         /* 0x1C */
    HuVecF rot;         /* 0x28 */
    s32 score;          /* 0x34 */
    HU3DANIMID anim[3]; /* 0x38 */
    s16 pad3E;          /* 0x3E */
} M534SCORE;

s32 lbl_1_data_350[2] = { 0x5C0010, 0x5C0011 };

char lbl_1_data_358[3][8] = { "pos01", "pos02", "pos03" };

HuVecF lbl_1_data_370[2] = {
    { -800.0f, 800.0f, -600.0f },
    { 800.0f, 800.0f, -600.0f },
};

HuVecF lbl_1_data_388[2] = {
    { -250.0f, 280.0f, -100.0f },
    { 250.0f, 280.0f, -100.0f },
};

HuVecF lbl_1_data_3A0[2] = {
    { -360.0f, 340.0f, -270.0f },
    { 360.0f, 340.0f, -270.0f },
};

char lbl_1_data_3B8[13] = "itemhook_sao";

OMOBJ *lbl_1_bss_58;

void fn_1_6BDC(HUPROCESS *objman, OMOBJ **mainObj)
{
    OMOBJ *obj;
    void *data;

    obj = omAddObjEx(objman, 0x32, 4, 0, -1, fn_1_6C78);
    lbl_1_bss_58 = obj;
    data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M534SCORE) * 2, 0x10000000U);
    obj->data = data;
    memset(data, 0, sizeof(M534SCORE) * 2);
    obj->stat |= 0x100;
}

void fn_1_6C74(void)
{
}

static void fn_1_6C78(OMOBJ *obj)
{
    fn_1_6CF4(obj);
    fn_1_6EE0(obj);
    obj->objFunc = fn_1_6CBC;
}

static void fn_1_6CBC(OMOBJ *obj)
{
    fn_1_6DEC(obj);
    fn_1_6FE8(obj);
}

static void fn_1_6CF4(OMOBJ *obj)
{
    M534SCORE *board = obj->data;
    M534SCORE *s;
    void *anim;
    s16 model;
    s32 i;
    s32 j;

    anim = HuDataSelHeapReadNum(0x960032, 0x10000000, HEAP_MODEL);
    for (i = 0; i < 2; i++) {
        s = &board[i];
        model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_350[i], 0x10000000, HEAP_MODEL));
        obj->mdlId[i] = model;
        Hu3DModelLayerSet(model, 0);
        for (j = 0; j < 3; j++) {
            s->anim[j] = Hu3DAnimCreate(anim, model, lbl_1_data_358[j]);
            Hu3DAnimAttrSet(s->anim[j], 1);
        }
    }
}

static void fn_1_6DEC(OMOBJ *obj)
{
    M534SCORE *board = obj->data;
    M534SCORE *s;
    s32 num;
    s32 i;

    for (i = 0; i < 2; i++) {
        s = &board[i];
        num = s->score;
        if (num > 999) {
            num = 999;
        }
        Hu3DAnimBankSet(s->anim[0], (u16) (num / 100));
        num %= 100;
        Hu3DAnimBankSet(s->anim[1], (u16) (num / 10));
        Hu3DAnimBankSet(s->anim[2], (u16) (num % 10));
    }
}

static void fn_1_6EE0(OMOBJ *obj)
{
    M534SCORE *board = obj->data;
    s16 motion;
    s16 first;
    s16 model;
    s32 i;

#ifdef TARGET_PC
    motion = 0;
    first = 0;
#endif
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            first = Hu3DModelCreate(HuDataSelHeapReadNum(0x5C000E, 0x10000000, HEAP_MODEL));
            model = first;
            motion = Hu3DJointMotion(model, HuDataSelHeapReadNum(0x5C000F, 0x10000000, HEAP_MODEL));
        } else {
            model = Hu3DModelLink(first);
        }
        obj->mdlId[i + 2] = model;
        Hu3DMotionSet(model, motion);
        Hu3DModelAttrSet(model, 0x40000001);
        Hu3DModelLayerSet(model, 0);
        Hu3DModelHookSet(model, lbl_1_data_3B8, obj->mdlId[i]);
        Hu3DModelAttrSet(model, 1);
    }
}

static void fn_1_6FE8(OMOBJ *obj)
{
    M534SCORE *board = obj->data;
    M534SCORE *s;
    HuVecF v;
    f32 t;
    s16 model;
    s32 i;

    for (i = 0; i < 2; i++) {
        s = &board[i];
        model = obj->mdlId[i + 2];
        switch (s->state) {
            case 0:
                s->radius = 20.0f;
                s->angle = 90.0f * (f32) i;
                s->pos = lbl_1_data_370[i];
                s->state++;
                break;
            case 1:
                if (fn_1_179C() != 0 && fn_1_17C8() != 0) {
                    Hu3DModelAttrReset(model, 1);
                    s->t = 0.0f;
                    s->state++;
                }
                break;
            case 2:
                s->t += 0.02;
                if (s->t > 1.0) {
                    s->t = 1.0f;
                }
                t = sin(1.5707963267948966 * s->t);
                s->pos.x = lbl_1_data_370[i].x + t * (lbl_1_data_388[i].x - lbl_1_data_370[i].x);
                s->pos.y = lbl_1_data_370[i].y + t * (lbl_1_data_388[i].y - lbl_1_data_370[i].y);
                s->pos.z = lbl_1_data_370[i].z + t * (lbl_1_data_388[i].z - lbl_1_data_370[i].z);
                if (fn_1_49FC() != 0) {
                    s->t = 0.0f;
                    s->state++;
                }
                break;
            case 3:
                s->t += 0.02;
                if (s->t > 1.0) {
                    s->t = 1.0f;
                }
                t = s->t;
                s->pos.x = lbl_1_data_388[i].x + t * (lbl_1_data_3A0[i].x - lbl_1_data_388[i].x);
                s->pos.y = lbl_1_data_388[i].y + t * (lbl_1_data_3A0[i].y - lbl_1_data_388[i].y);
                s->pos.z = lbl_1_data_388[i].z + t * (lbl_1_data_3A0[i].z - lbl_1_data_388[i].z);
                break;
        }
        s->angle += 1.0;
        PSVECAdd(&s->pos, &s->vel, &s->pos);
        v.x = s->pos.x + s->radius * sin(3.141592653589793 * s->angle / 180.0);
        v.y = s->pos.y + s->radius * cos(3.141592653589793 * s->angle / 180.0);
        v.z = s->pos.z;
        Hu3DModelPosSetV(model, &v);
        Hu3DModelRotSetV(model, &s->rot);
    }
}

void fn_1_74B0(s32 team, s32 amount)
{
    M534SCORE *board = lbl_1_bss_58->data;

#ifdef TARGET_PC
    team &= 1;
#endif
    board[team].score += amount;
}

s32 fn_1_74E8(s32 team)
{
    M534SCORE *board = lbl_1_bss_58->data;

#ifdef TARGET_PC
    team &= 1;
#endif
    return board[team].score;
}
