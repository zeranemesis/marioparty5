#include <math.h>

#include "game/hu3d.h"

void *__dl__FPv(void *self);
void fn_1_1D88(void *self);
void fn_1_1D94(void *self, s32 arg1);
void fn_1_1E54(void *self, void **arg1);
u16 fn_1_5578(s32 arg0, f32 arg1);
void fn_1_CF8(char *cond, char *file, s32 line);
extern "C" s32 frand(void);

struct Mgr511Smoke;
struct Mgr511Spark;

void fn_1_894C(Mgr511Smoke *arg0, struct ParticleNode *arg1);
void fn_1_8B8C(Mgr511Smoke *arg0, Mtx mtx);
void fn_1_917C(Mgr511Spark *arg0, struct ParticleNode *arg1);
void fn_1_93BC(Mgr511Spark *arg0, Mtx mtx);

/* One entry of the particle pool: 0x44 bytes. */
struct ParticleNode {
    s16 state;
    s16 rate;
    f32 unk4;
    f32 unk8;
    f32 vel;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 size;
    f32 angle;
    f32 posX;
    f32 posY;
    f32 posZ;
    u8 cr;
    u8 cg;
    u8 cb;
    u8 alpha;
};

/* The shared particle pool that both managers allocate out of. */
struct ParticlePool {
    u8 pad00[0x1C];
    void *owner;
    u8 pad20[0x10];
    s16 maxCnt;
    u8 pad32[0x16];
    struct ParticleNode *data;
};

struct Mgr511Smoke {
    void *unk0;
    void **vtable;
    s32 unkC;
    s16 modelId;
    struct ParticlePool *pool;
    f32 posX;
    f32 posY;
    f32 posZ;
    f32 unk24;
    f32 speed;
    f32 unk2C;
    f32 timer;
    u8 pad38;
    s32 tick;
    s32 count;
};

struct Mgr511Spark {
    s16 modelId;
    struct ParticlePool *pool;
    f32 posX;
    f32 posY;
    f32 posZ;
    f32 velX;
    f32 velY;
    f32 velZ;
    s32 unk20;
    s32 unk24;
};

struct Mgr511ParticleRoot {
    u8 pad10[0x10];
    s32 unk10;
};

extern "C" {

extern void *lbl_1_bss_2C;

extern void *lbl_1_data_40[6];
extern void *lbl_1_data_354[6];

extern Mgr511ParticleRoot *lbl_1_bss_A38;

void fn_1_8488(HU3DMODEL *arg1, HU3DPARTICLE *arg2, Mtx mtx)
{
    fn_1_8B8C((Mgr511Smoke *)arg2->work, mtx);
}

// NONMATCHING: the pool walk and the field offsets are not reproduced.
Mgr511Smoke *fn_1_84CC(Mgr511Smoke *arg0, Mgr511Smoke *arg1)
{
    HU3DMODELID pid;
    Mgr511Smoke *src;
    struct ParticleNode *node;
    s32 i;

    fn_1_1D88(arg0);
    arg0->vtable = &lbl_1_data_40[2];
    arg0->unkC = 0;
    fn_1_1E54(arg0, (void **)lbl_1_bss_2C);
    arg0->vtable = lbl_1_data_354;
    src = arg1;
    arg0->timer = 0.0f;
    arg0->pad38 = 0;
    arg0->tick = 0;
    arg0->count = 1;
    if (lbl_1_bss_A38 == NULL) {
        fn_1_CF8("ms_pointer != 0", "singleton.h", 0x27);
    }
    pid = Hu3DParticleCreate((ANIMDATA *)lbl_1_bss_A38->unk10, 8);
    arg0->pool = (struct ParticlePool *)Hu3DData[pid].hookData;
    arg0->pool->owner = arg0;
    Hu3DParticleHookSet(pid, fn_1_8488);
    Hu3DParticleColSet(pid, 0xFF, 0xFF, 0xFF);
    Hu3DParticleTPLvlSet(pid, 0.0f);
    Hu3DParticleBlendModeSet(pid, 1);
    Hu3DModelLayerSet(pid, 6);
    Hu3DModelPosSet(pid, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(pid, 0.0f, 0.0f, 0.0f);
    arg0->posX = (f32)*(f32 *)((u8 *)src + 0x88);
    arg0->posY = (f32)*(f32 *)((u8 *)src + 0x8C);
    arg0->posZ = (f32)*(f32 *)((u8 *)src + 0x90);
    arg0->unk24 = 0.0f;
    arg0->speed = (f32)*(s32 *)((u8 *)src + 0x70);
    arg0->unk2C = 0.0f;
    node = arg0->pool->data;
    for (i = 0; i < arg0->pool->maxCnt; i++) {
        fn_1_894C(arg0, node);
        node++;
    }
    arg0->modelId = (s16)pid;
    return arg0;
}

void *fn_1_8770(Mgr511Smoke *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        arg0->vtable = lbl_1_data_354;
        Hu3DModelKill(arg0->modelId);
        arg0->vtable = &lbl_1_data_40[2];
        fn_1_1D94(arg0, 0);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_87F8(void *arg0, s32 arg1)
{
    s32 *flag = (s32 *)((u8 *)arg0 + 0xC);

    *flag &= ~(1 << arg1);
}

void fn_1_8810(Mgr511Smoke *arg0)
{
    Mgr511Smoke *src = arg0;
    s32 tick;

    arg0->posX = (f32)*(f32 *)((u8 *)src + 0x88);
    arg0->posY = (f32)*(f32 *)((u8 *)src + 0x8C);
    arg0->posZ = (f32)*(f32 *)((u8 *)src + 0x90);
    arg0->unk24 = 0.0f;
    arg0->speed = (f32)*(s32 *)((u8 *)src + 0x70);
    arg0->unk2C = 0.0f;
    if (arg0->pad38 != 1) {
        tick = arg0->tick;
        arg0->tick = tick + 1;
        if ((tick & 1) == 0) {
            arg0->count = arg0->count + 1;
            if (arg0->count >= 8) {
                arg0->count = 8;
            }
        }
        arg0->timer = (f32)((f64)arg0->timer + 0.016666666666666666);
        if (arg0->timer > 3.0f) {
            arg0->pad38 = 1;
        }
    }
}

void fn_1_894C(Mgr511Smoke *arg0, struct ParticleNode *arg1)
{
    struct ParticleNode *node = arg1;
    s32 seed;
    u32 color;
    f32 temp_f1;
    f32 temp_f1_2;

    node->state = 0;
    seed = frand() & 0xF;
    node->rate = (s16)(seed + 7);
    node->posX = (f32)(arg0->posX + (50.0f * (0.000030517578f * ((f32)fn_1_5578(seed, 0.0f) - 32768.0f))));
    node->posY = (f32)(100.0f + arg0->posY);
    temp_f1 = 50.0f * (0.000030517578f * ((f32)fn_1_5578(0, 100.0f) - 32768.0f));
    node->posZ = (f32)(arg0->posZ + temp_f1);
    temp_f1_2 = 0.2f + (0.1f * (0.000015258789f * (f32)fn_1_5578(0, temp_f1)));
    node->size = (f32)(100.0f * temp_f1_2);
    node->angle = (f32)(0.017453292f * (360.0f * (0.000015258789f * (f32)fn_1_5578(0, temp_f1_2))));
    color = frand();
    node->cr = (u8)color;
    node->cg = (u8)(color >> 8);
    node->cb = (u8)(color >> 16);
    node->alpha = 0;
}

// NONMATCHING: the alpha ramp and the vtable call are not reproduced.
void fn_1_8B8C(Mgr511Smoke *arg0, Mtx mtx)
{
    struct ParticleNode *node;
    s32 i;
    u8 var_r25;

    var_r25 = 1;
    node = arg0->pool->data;
    for (i = 0; i < arg0->count; i++) {
        if (node->state <= 1) {
            if (node->state == 0) {
                node->alpha = (u8)(node->alpha + node->rate);
                if (node->alpha > 0xE6) {
                    node->alpha = 0xFF;
                    node->state = node->state + 1;
                }
            } else {
                node->alpha = (u8)(node->alpha - node->rate);
                if (node->alpha < 0x19) {
                    if (arg0->pad38 == 1) {
                        node->state = node->state + 1;
                    } else {
                        node->state = 0;
                        fn_1_894C(arg0, node);
                    }
                }
            }
            node->angle = (f32)(node->angle + 0.08726646f);
            var_r25 = 0;
        }
        node++;
    }
    if (var_r25 != 0 && arg0 != NULL) {
        ((void (*)(Mgr511Smoke *, s32))arg0->vtable[1])(arg0, 1);
    }
}

void fn_1_8EF4(HU3DMODEL *arg1, HU3DPARTICLE *arg2, Mtx mtx)
{
    fn_1_93BC((Mgr511Spark *)arg2->work, mtx);
}

Mgr511Spark *fn_1_8F38(Mgr511Spark *arg0, Point3d *arg1, s32 arg_sp0)
{
    HU3DMODELID pid;
    struct ParticleNode *node;
    s32 i;

    arg0->unk20 = 0;
    arg0->unk24 = 1;
    arg0->posX = arg1->x;
    arg0->posY = arg1->y;
    arg0->posZ = arg1->z;
    arg0->velX = 0.0f;
    arg0->velY = 0.0f;
    arg0->velZ = 0.0f;
    if (lbl_1_bss_A38 == NULL) {
        fn_1_CF8("ms_pointer != 0", "singleton.h", 0x27);
    }
    pid = Hu3DParticleCreate((ANIMDATA *)lbl_1_bss_A38->unk10, 0x28);
    arg0->pool = (struct ParticlePool *)Hu3DData[pid].hookData;
    arg0->pool->owner = arg0;
    Hu3DParticleHookSet(pid, fn_1_8EF4);
    Hu3DParticleColSet(pid, 0xFF, 0xFF, 0xFF);
    Hu3DParticleTPLvlSet(pid, 0.0f);
    Hu3DParticleBlendModeSet(pid, 1);
    Hu3DModelLayerSet(pid, 7);
    Hu3DModelPosSet(pid, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(pid, 0.0f, 0.0f, 0.0f);
    node = arg0->pool->data;
    for (i = 0; i < arg0->pool->maxCnt; i++) {
        fn_1_917C(arg0, node);
        node++;
    }
    arg0->modelId = (s16)pid;
    return arg0;
}

void *fn_1_9128(Mgr511Spark *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        Hu3DModelKill(arg0->modelId);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_917C(Mgr511Spark *arg0, struct ParticleNode *arg1)
{
    struct ParticleNode *node = arg1;
    s32 seed;
    u32 color;
    f32 temp_f1;
    f32 temp_f1_2;
    f32 temp_f1_3;

    node->state = 0;
    seed = frand() & 0xF;
    node->rate = (s16)(seed + 7);
    temp_f1 = 70.0f * (0.000030517578f * ((f32)fn_1_5578(seed, 0.0f) - 32768.0f));
    node->posX = (f32)(arg0->posX + temp_f1);
    node->posY = arg0->posY;
    temp_f1_2 = 70.0f * (0.000030517578f * ((f32)fn_1_5578(0, temp_f1) - 32768.0f));
    node->posZ = (f32)(arg0->posZ + temp_f1_2);
    temp_f1_3 = 0.2f + (0.3f * (0.000015258789f * (f32)fn_1_5578(0, temp_f1_2)));
    node->size = (f32)(100.0f * temp_f1_3);
    node->angle = (f32)(0.017453292f * (360.0f * (0.000015258789f * (f32)fn_1_5578(0, temp_f1_3))));
    color = frand();
    node->vel = 0.0f;
    node->cr = (u8)color;
    node->cg = (u8)(color >> 8);
    node->cb = (u8)(color >> 16);
    node->alpha = 0;
}

// NONMATCHING: the alpha ramp and the gravity integration are not reproduced.
void fn_1_93BC(Mgr511Spark *arg0, Mtx mtx)
{
    struct ParticlePool *pool;
    struct ParticleNode *node;
    s32 i;
    s32 seed;
    u32 color;
    f32 temp_f1;
    f32 temp_f1_2;
    f32 temp_f1_3;
    f32 temp_f31;

    pool = arg0->pool;
    node = pool->data;
    for (i = 0; i < pool->maxCnt; i++) {
        if (node->state != 0xFF) {
            if (node->state == 0) {
                node->alpha = (u8)(node->alpha + 4);
                if (node->alpha > 0xFA) {
                    node->alpha = 0xFF;
                    node->state = node->state + 1;
                }
            } else {
                node->alpha = (u8)(node->alpha - node->rate);
                if (node->alpha < 0x19) {
                    node->state = 0;
                    seed = frand() & 0xF;
                    node->rate = (s16)(seed + 7);
                    temp_f1 = 70.0f * (0.000030517578f * ((f32)fn_1_5578(seed, 0.0f) - 32768.0f));
                    node->posX = (f32)(arg0->posX + temp_f1);
                    node->posY = arg0->posY;
                    temp_f1_2 = 70.0f * (0.000030517578f * ((f32)fn_1_5578(0, temp_f1) - 32768.0f));
                    node->posZ = (f32)(arg0->posZ + temp_f1_2);
                    temp_f1_3 = 0.2f + (0.3f * (0.000015258789f * (f32)fn_1_5578(0, temp_f1_2)));
                    node->size = (f32)(100.0f * temp_f1_3);
                    node->angle = (f32)(0.017453292f * (360.0f * (0.000015258789f * (f32)fn_1_5578(0, temp_f1_3))));
                    color = frand();
                    node->vel = 0.0f;
                    node->cr = (u8)color;
                    node->cg = (u8)(color >> 8);
                    node->cb = (u8)(color >> 16);
                    node->alpha = 0;
                }
            }
            node->angle = (f32)(node->angle + 0.017453292f);
            temp_f31 = node->vel;
            node->vel = (f32)((f64)node->vel + -0.08172208658067717);
            node->posY = (f32)(node->posY + (0.5f * (temp_f31 + node->vel)));
        }
        node++;
    }
}

void fn_1_9700(Mgr511Spark *arg0, Point3d *arg1, Point3d *arg2)
{
    arg0->posX = arg1->x;
    arg0->posY = arg1->y;
    arg0->posZ = arg1->z;
    arg0->velX = arg2->x;
    arg0->velY = arg2->y;
    arg0->velZ = arg2->z;
}

}