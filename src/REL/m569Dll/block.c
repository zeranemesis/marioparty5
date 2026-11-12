#include "REL/m569Dll.h"

#include "game/audio.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/memory.h"
#include "game/sprite.h"

#include "math.h"

#define M569_RAD(x) (3.141592653589793 * (f64)(x) / 180.0)

/* ---------------------------------------------------------------- data */

static const s32 lbl_1_rodata_198[2] = { 0x7C000F, 0x7C0010 };

static u8 lbl_1_data_320[2][6] = {
    { 0, 1, 2, 3, 4, 5 },
    { 0, 2, 0, 1, 0, 2 }
};
HuVecF lbl_1_data_32C[3] = {
    { -200.0f, 270.0f, -50.0f },
    { 0.0f, 270.0f, -50.0f },
    { 200.0f, 270.0f, -50.0f }
};
static u8 lbl_1_data_350[4] = { 0, 1, 0, 0 };
static HU3DPARMANPARAM lbl_1_data_354 = {
    0x32,
    0,
    50.0f,
    0.0f,
    65.0f,
    { 0.0f, -0.195f, 0.0f },
    15.0f,
    0.95f,
    35.0f,
    0.999f,
    4,
    { { 0xFF, 0xFF, 0x20, 0xFF }, { 0xFF, 0xFF, 0x00, 0xFF }, { 0xFF, 0xFF, 0x80, 0xFF }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 0xFF, 0xFF, 0xFF, 0x00 }, { 0xFF, 0xFF, 0xFF, 0x00 }, { 0xFF, 0xFF, 0xFF, 0x00 }, { 0xFF, 0xFF, 0xFF, 0x00 } }
};

static s32 lbl_1_bss_150;
static s16 lbl_1_bss_154;
static M569BLOCK *lbl_1_bss_158[3];

/* ------------------------------------------------------------- functions */

static void fn_1_7F94(OMOBJ *obj);

s16 fn_1_74E0(HuVecF *pos)
{
    HU3DPARMANID parId;
    HU3DMODELID mdl;
    ANIMDATA *anim;
    void *data;

    data = HuDataSelHeapReadNum(0x210001, HU_MEMNUM_OVL, HEAP_MODEL);
    anim = HuSprAnimRead(data);
    parId = Hu3DParManCreate(anim, 0x32, &lbl_1_data_354);
    Hu3DParManAttrSet(parId, 0x108);
    Hu3DParManPosSet(parId, pos->x, pos->y, pos->z);
    Hu3DParManRotSet(parId, 0.0f, 0.0f, 0.0f);
    Hu3DParManVecSet(parId, 0.0f, 0.4f, 0.2f);
    Hu3DParManTimeLimitSet(parId, 0x32);
    mdl = Hu3DParManModelIDGet(parId);
    Hu3DModelLayerSet(mdl, 2);
    Hu3DParticleBlendModeSet(mdl, 0);
    return parId;
}

void fn_1_75F8(OMOBJ *obj)
{
    M569BLOCK *block;

    block = (M569BLOCK *)obj->data;
    block->rotY += 0x19;
    if (block->rotY >= 0x438) {
        block->state = 1;
        block->scale = 1.0f;
        block->rotY = 0;
        block->timer = 0;
        return;
    }
    if (block->timer < 0xB4) {
        block->scale = sin(M569_RAD((f32)(block->timer % 180)));
    } else {
        block->scale = 1.0 + 0.5 * sin(M569_RAD((f32)(block->timer % 180)));
    }
    if (block->timer < 0x168) {
        if (block->timer < 0xB4) {
            block->timer += 6;
        } else {
            block->timer += 0x12;
        }
        if (block->timer > 0x168) {
            block->timer = 0x168;
        }
    }
}

static void fn_1_77D8(OMOBJ *obj)
{
    M569BLOCK *block;

    block = (M569BLOCK *)obj->data;
    switch (block->state) {
    case 0:
        fn_1_75F8(obj);
        break;
    case 1:
        block->bob += 4.0f;
        if (block->bob > 360.0f) {
            block->bob -= 360.0f;
        }
        block->pos.y = 270.0 + 5.0 * sin(M569_RAD(block->bob));
        break;
    case 2:
        block->state = 3;
        break;
    case 3:
        block->timer++;
        Hu3DModelTPLvlSet(obj->mdlId[0], 1.0f - 0.2f * (f32)block->timer);
        if (block->timer > 4) {
            block->timer = 0;
            fn_1_7F94(obj);
            Hu3DModelAttrSet(obj->mdlId[0], 1);
            block->parId = fn_1_74E0(&block->pos);
            block->state = 4;
        }
        break;
    case 5:
        block->timer++;
        Hu3DModelTPLvlSet(obj->mdlId[1], 1.0f - 0.1f * (f32)block->timer);
        if (block->timer == 10) {
            block->state = 6;
        }
        return;
    case 6:
        Hu3DModelKill(obj->mdlId[0]);
        Hu3DModelKill(obj->mdlId[1]);
        if (block->parId != -1) {
            Hu3DParManKill(block->parId);
        }
        omDelObjEx(lbl_1_bss_D4, obj);
#ifdef TARGET_PC
        return;
#endif
        break;
    }
    if (block->state <= 1) {
        block->motCnt++;
        if (block->motCnt == 3) {
            block->motCnt = 0;
            block->motIdx++;
            if (block->motIdx == 6) {
                block->motIdx = 0;
            }
            Hu3DMotionTimeSet(obj->mdlId[0], (f32)block->mot[block->motIdx]);
        }
    }
    omSetRot(obj, 0.0f, (f32)block->rotY, 0.0f);
    omSetTra(obj, block->pos.x, block->pos.y, block->pos.z);
    omSetSca(obj, block->scale, block->scale, block->scale);
}

OMOBJ *fn_1_7DC0(u8 type, HuVecF *pos)
{
    OMOBJ *obj;
    M569BLOCK *block;
    s16 mdl;
    s32 i;

    obj = omAddObjEx(lbl_1_bss_D4, 0x2000, 2, 0, -1, fn_1_77D8);
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M569BLOCK), HU_MEMNUM_OVL);
    block = (M569BLOCK *)obj->data;
    block->pos = *pos;
    block->type = type;
    block->scale = 0.0f;
    block->timer = 0;
    block->rotY = 0;
    block->parId = -1;
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_rodata_198[block->type], HU_MEMNUM_OVL, HEAP_MODEL));
    obj->mdlId[0] = mdl;
    Hu3DModelPosSetV(mdl, &block->pos);
    Hu3DModelScaleSet(mdl, block->scale, block->scale, block->scale);
    Hu3DModelAttrSet(mdl, 0x40000002);
    for (i = 0; i < 6; i++) {
        block->mot[i] = lbl_1_data_320[type][i];
    }
    block->motIdx = frandmod(6);
    block->motCnt = 0;
    block->state = 0;
    block->bob = (f32)(s32)frandmod(0x168);
    omSetTra(obj, block->pos.x, block->pos.y, block->pos.z);
    omSetSca(obj, block->scale, block->scale, block->scale);
    return obj;
}

static void fn_1_7F94(OMOBJ *obj)
{
    M569BLOCK *block;
    s16 mdl;

    block = (M569BLOCK *)obj->data;
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x7C0011, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(mdl, 0x40000002);
    Hu3DModelPosSet(mdl, block->pos.x, 270.0f, block->pos.z);
    Hu3DModelLayerSet(mdl, 1);
    if (block->type == 0) {
        Hu3DMotionTimeSet(mdl, (f32)block->mot[block->motIdx]);
    } else {
        Hu3DMotionTimeSet(mdl, (f32)(block->mot[block->motIdx] + 6));
    }
    obj->mdlId[1] = mdl;
}

void fn_1_80AC(void)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        lbl_1_bss_158[i] = NULL;
    }
    lbl_1_bss_150 = -1;
}

void fn_1_80FC(void)
{
    s32 i;
    OMOBJ *obj;

    for (i = 0; i < 3; i++) {
        obj = fn_1_7DC0(lbl_1_data_350[i], &lbl_1_data_32C[i]);
        lbl_1_bss_158[i] = (M569BLOCK *)obj->data;
    }
    lbl_1_bss_154 = 3;
    lbl_1_bss_150 = HuAudFXPlay(0x1D);
}

void fn_1_833C(void)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        lbl_1_bss_158[i]->state = 5;
        lbl_1_bss_158[i]->timer = 0;
    }
}

void fn_1_839C(s32 idx)
{
    M569BLOCK **slot;

    slot = &lbl_1_bss_158[idx];
    if (lbl_1_bss_158[idx]->state == 1) {
        lbl_1_bss_158[idx]->state = 2;
        lbl_1_bss_154--;
        if (lbl_1_bss_154 == 0) {
            if (lbl_1_bss_150 != -1) {
                HuAudFXStop(lbl_1_bss_150);
                lbl_1_bss_150 = -1;
            }
            HuAudFXPlay(0x1E);
            return;
        }
        HuAudFXPlay(0x21);
    }
}

s32 fn_1_848C(s32 idx)
{
    return lbl_1_bss_158[idx]->state;
}

s32 fn_1_84A8(void)
{
    s32 list[3];
    s32 cnt;
    s32 i;

    cnt = 0;
    for (i = 0; i < 3; i++) {
        if (lbl_1_bss_158[i]->state == 1) {
            list[cnt++] = i;
        }
    }
    if (cnt != 0) {
        return list[frandmod(cnt)];
    }
    return -1;
}

s32 fn_1_8544(void)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        if (lbl_1_bss_158[i] == NULL) {
            return 0;
        }
        if (lbl_1_bss_158[i]->state != 1) {
            return 0;
        }
    }
    return 1;
}

s32 fn_1_85C0(void)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        if (lbl_1_bss_158[i]->state != 4) {
            return 0;
        }
    }
    return 1;
}

void fn_1_8614(void)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        if (lbl_1_bss_158[i]->state == 1) {
            lbl_1_bss_158[i]->state = 2;
        }
    }
    if (lbl_1_bss_150 != -1) {
        HuAudFXStop(lbl_1_bss_150);
        HuAudFXPlay(0x1E);
        lbl_1_bss_150 = -1;
    }
}

s32 fn_1_86C4(void)
{
    s32 ret;
    u8 a;
    u8 b;
    u8 op;

    ret = 0;
    a = lbl_1_bss_158[0]->mot[lbl_1_bss_158[0]->motIdx] + 1;
    b = lbl_1_bss_158[2]->mot[lbl_1_bss_158[2]->motIdx] + 1;
    op = lbl_1_bss_158[1]->mot[lbl_1_bss_158[1]->motIdx];
    switch (op) {
    case 1:
        ret = a - b;
        if (ret < 0) {
            ret = 0;
        }
        break;
    case 0:
        ret = a + b;
        break;
    case 2:
        ret = a * b;
        break;
    }
    return ret;
}
