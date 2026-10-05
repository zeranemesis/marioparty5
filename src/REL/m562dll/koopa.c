#include "types.h"

/* library */
extern void Hu3DModelKill(s16 modelId);
extern void Hu3DMotionKill(s16 motionId);
extern void *memset(void *dst, s32 val, u32 len);
extern s32 HuDataSelHeapReadNum(u32 id, u32 heap, s32 count);
extern s16 Hu3DModelCreate(void);
extern void Hu3DModelScaleSet(s16 model, f32 x, f32 y, f32 z);
extern void Hu3DModelCameraSet(s16 model, u32 cam);
extern void Hu3DModelLayerSet(s16 model, s32 layer);
extern void Hu3DModelAttrReset(s16 model, s32 attr);
extern void Hu3DModelShadowSet(s16 model);
extern void Hu3DMotionSpeedSet(s16 model, f32 speed);
extern void Hu3DModelPosSet(s16 model, f32 x, f32 y, f32 z);
extern void Hu3DModelRotSet(s16 model, f32 x, f32 y, f32 z);
extern s16 Hu3DJointMotion(s16 model, s32 motion);
extern void Hu3DMotionSet(s16 model, s16 motion);
extern void Hu3DModelAttrSet(s16 model, u32 attr);
extern void Hu3DMotionShiftSet(s16 model, s16 motion, f32 a, f32 b, u32 flags);

/* sibling module functions */
extern void fn_1_2CDE8(void *arg0);
extern void fn_1_2CD6C(void *arg0, u32 arg1);

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

typedef struct {
    /* 0x0000 */ void *unk0;
    /* 0x0004 */ u8 pad4[0x1C];
    /* 0x0020 */ s16 modelId;
    /* 0x0022 */ s16 motionId[14];
    /* 0x003E */ u8 pad3E[0x0A];
    /* 0x0048 */ s16 f48;
    /* 0x004A */ s16 f4A;
    /* 0x004C */ u32 f4C;
    /* 0x0050 */ u8 pad50[4];
    /* 0x0054 */ Vec3f pos;
    /* 0x0060 */ u8 pad60[0x54];
    /* 0x00B4 */ f32 fB4;
} KoopaObj;

typedef struct {
    /* 0x0000 */ u8 pad0[0x1C0];
    /* 0x01C0 */ u32 camA;
    /* 0x01C4 */ u8 pad1C4[0x98];
    /* 0x025C */ u32 camB;
} KoopaOwner;

typedef struct {
    /* 0x00 */ u16 kind;
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ u16 state;
    /* 0x06 */ s16 f6;
    /* 0x08 */ s16 f8;
    /* 0x0A */ s16 fA;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ s16 f10;
} KoopaNode;

typedef struct {
    /* 0x00 */ u16 kind;
    /* 0x02 */ u8 pad2[6];
    /* 0x08 */ KoopaNode *node;
} KoopaRef;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ Vec3f min;
    /* 0x1C */ u8 pad1C[0x53C];
} KoopaBound;

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ KoopaBound b[2];
} KoopaLimit;

/* the 14 joint-motion ids this koopa cycles through */
u32 lbl_1_data_7F8[14] = {
    0x0075002D, 0x0075002E, 0x0075002F, 0x00750030,
    0x00750031, 0x00750032, 0x00750033, 0x00750034,
    0x00750035, 0x00750036, 0x00750037, 0x00750038,
    0x00750039, 0x0075003A
};

/* spawn position handed to Hu3DModelPosSet */
Vec3f lbl_1_data_830 = { 0.0f, 2000.0f, -1176.0f };

void fn_1_EEC4(KoopaRef *arg0, KoopaRef *arg1);
void fn_1_F014(KoopaNode *arg0, KoopaNode *arg1);

extern void fn_1_2CE3C(void *arg0, void (*arg1)(KoopaRef *a, KoopaRef *b));

void fn_1_B2E0(KoopaObj *arg0, KoopaOwner *arg1) {
    KoopaOwner *owner = arg1;
    s16 model;
    s32 i;

    memset(arg0, 0, 0xB8);
    arg0->unk0 = arg1;
    arg0->fB4 = 0.0f;
    HuDataSelHeapReadNum(0x00750017, 0x10000000, 2);
    model = Hu3DModelCreate();
    arg0->modelId = model;
    Hu3DModelScaleSet(model, 1.5f, 1.5f, 1.5f);
    Hu3DModelCameraSet(model, (u16)(owner->camA | owner->camB));
    Hu3DModelLayerSet(model, 4);
    Hu3DModelAttrReset(model, 1);
    Hu3DModelShadowSet(model);
    Hu3DMotionSpeedSet(model, 1.0f);
    arg0->pos = lbl_1_data_830;
    Hu3DModelPosSet(model, arg0->pos.x, arg0->pos.y, arg0->pos.z);
    Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
    for (i = 0; i < 14; i++) {
        arg0->motionId[i] = Hu3DJointMotion(model, HuDataSelHeapReadNum(lbl_1_data_7F8[i], 0x10000000, 2));
    }
    Hu3DMotionSet(model, arg0->motionId[0]);
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DMotionShiftSet(model, arg0->motionId[0], 0.0f, 4.0f, 0x40000001);
    arg0->f48 = arg0->f4A = 0;
    arg0->f4C = 1;
    fn_1_2CD6C((u8 *)owner + 0x287C, 0x24);
    fn_1_2CE3C((u8 *)owner + 0x287C, fn_1_EEC4);
}

void fn_1_B4F8(KoopaObj *arg0) {
    void *owner = arg0->unk0;
    s32 i;

    fn_1_2CDE8((u8 *)owner + 0x287C);
    for (i = 0; i < 14; i++) {
        Hu3DMotionKill(arg0->motionId[i]);
    }
    Hu3DModelKill(arg0->modelId);
    memset(arg0, 0, 0xB8);
}

void fn_1_CE10(KoopaObj *arg0) {
    arg0->f4A = 0;
    arg0->f4C = 1;
}

void fn_1_ED90(KoopaObj *arg0, Vec3f *in, Vec3f *out) {
    void *owner = arg0->unk0;
    KoopaLimit *lim = (KoopaLimit *)((u8 *)owner + 0x227C);
    KoopaBound *bnd = &lim->b[0];
    f32 minX;
    f32 maxX;
    f32 minZ;
    f32 maxZ;

    minX = bnd->min.x;
    minZ = bnd->min.z;
    bnd = &lim->b[1];
    maxX = bnd->min.x;
    maxZ = bnd->min.z;
    out->x = (in->x > maxX) ? maxX : ((in->x < minX) ? minX : in->x);
    out->y = in->y;
    out->z = (in->z > maxZ) ? maxZ : ((in->z < minZ) ? minZ : in->z);
}

void fn_1_EEC4(KoopaRef *arg0, KoopaRef *arg1) {
    KoopaNode *p;

    if (arg0->kind == 0) {
        if (arg1->kind == 3) {
            fn_1_F014(arg0->node, arg1->node);
            return;
        }
    }
    if (arg0->kind == 3) {
        if (arg1->kind == 0) {
            fn_1_F014(arg1->node, arg0->node);
            return;
        }
    }
    if (arg0->kind == 4) {
        if (arg1->kind == 0) {
            p = arg1->node;
            if (p->state == 1) {
                goto checkF10;
            }
            if (p->state != 2) {
                return;
            }
        checkF10:
            if (p->f10 != 0) {
                return;
            }
            p->fA = 0;
            p->f6 = 0;
            p->f8 = 0;
            p->state = 4;
            return;
        }
    }
    if (arg0->kind == 0) {
        if (arg1->kind == 4) {
            p = arg0->node;
            if (p->state == 1 || p->state == 2) {
                if (p->f10 == 0) {
                    p->fA = 0;
                    p->f6 = 0;
                    p->f8 = 0;
                    p->state = 4;
                }
            }
        }
    }
}