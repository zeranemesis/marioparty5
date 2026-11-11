#include <humath.h>
#include <math.h>

#include "game/audio.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/sprite.h"

typedef struct M533ListNode_s {
    u16 unk0;
    s16 unk2;
    struct M533ListNode_s *prev;
    struct M533ListNode_s *next;
} M533LISTNODE;

typedef struct M533CacheEntry_s {
    u32 unk0;
    s16 unk4;
    s16 unk6;
} M533CACHEENTRY;

/* 0x10 bytes in the original: a Vec plus trailing padding */
typedef struct M533VecPad_s {
    Vec v;
    u32 pad;
} M533VECPAD;

void fn_1_6984(OMOBJ *arg0, Vec *arg1, s32 arg2, float arg4, u16 arg3);
void fn_1_6B18(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
void fn_1_6F54(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);

Vec lbl_1_data_670[13] = {
    { -800.0f, -700.0f, 600.0f },
    { 1100.0f, -700.0f, 420.0f },
    { 2000.0f, -600.0f, -700.0f },
    { 2900.0f, -700.0f, 220.0f },
    { 3900.0f, -600.0f, 350.0f },
    { 5600.0f, -700.0f, -750.0f },
    { 6800.0f, -500.0f, 220.0f },
    { 7600.0f, -700.0f, -700.0f },
    { 8600.0f, -700.0f, 700.0f },
    { 10300.0f, -700.0f, -700.0f },
    { 12400.0f, -700.0f, -700.0f },
    { 14000.0f, -700.0f, 350.0f },
    { 0.0f, 0.0f, 0.0f },
};

char lbl_1_data_70C[0x28] = "******* No entry PillarPoint *********\n";

/* .bss is emitted in reverse declaration order by MWCC, so the original's
   address order is declared bottom-up; pads precede the variable they follow. */
M533LISTNODE lbl_1_bss_4E0;
M533VECPAD lbl_1_bss_4D0;
Vec lbl_1_bss_4C4;
Vec lbl_1_bss_4B8;
Vec lbl_1_bss_4AC;
Vec lbl_1_bss_47C[4];
Vec lbl_1_bss_44C[4];
Vec lbl_1_bss_41C[4];
Vec lbl_1_bss_3EC[4];
u32 lbl_1_bss_3E0_pad[2];
s32 lbl_1_bss_3E0;
s32 lbl_1_bss_3DC;
u32 lbl_1_bss_2D8_pad;
M533CACHEENTRY lbl_1_bss_2D8[32];
s32 lbl_1_bss_2D4;
u32 lbl_1_bss_2CC_pad;
void *lbl_1_bss_2CC;
ANIMDATA *lbl_1_bss_2C8;

void fn_1_6528(void)
{
    lbl_1_bss_2CC = HuDataSelHeapReadNum(0x5B000F, 0x10000000, HEAP_MODEL);
    lbl_1_bss_2C8 = HuSprAnimRead(lbl_1_bss_2CC);
    lbl_1_bss_2D4 = 0;
}

void fn_1_6590(OMOBJ *arg0)
{
    Vec sp8;
    float var_f31;
    s32 i;

    for (i = 0; i < 12; i++) {
        if (lbl_1_data_670[lbl_1_bss_2D4].x == 0.0f && lbl_1_data_670[lbl_1_bss_2D4].y == 0.0f
            && lbl_1_data_670[lbl_1_bss_2D4].z == 0.0f) {
            OSReport(lbl_1_data_70C);
            return;
        }
        sp8.x = lbl_1_data_670[lbl_1_bss_2D4].x;
        sp8.y = 300.0f + lbl_1_data_670[lbl_1_bss_2D4].y;
        sp8.z = lbl_1_data_670[lbl_1_bss_2D4].z;
        if (sp8.z < 0.0f) {
            var_f31 = 15.0f;
        } else {
            var_f31 = 25.0f;
        }
        fn_1_6984(arg0, &sp8, 0, var_f31, 3);
        lbl_1_bss_2D4++;
    }
}

void fn_1_6770(OMOBJ *arg0, s32 arg1)
{
    Vec sp8;

    sp8.x = 11400.0f;
    sp8.y = -400.0f;
    sp8.z = -600.0f;
    fn_1_6984(arg0, &sp8, 1, 50.0f, arg1);
    sp8.x = 11500.0f;
    sp8.y = -400.0f;
    sp8.z = -600.0f;
    fn_1_6984(arg0, &sp8, 1, 50.0f, arg1);
    sp8.x = 11600.0f;
    sp8.y = -400.0f;
    sp8.z = -600.0f;
    fn_1_6984(arg0, &sp8, 1, 50.0f, arg1);
    sp8.x = 11400.0f;
    sp8.y = -400.0f;
    sp8.z = 1000.0f;
    fn_1_6984(arg0, &sp8, 1, 50.0f, arg1);
    sp8.x = 11500.0f;
    sp8.y = -400.0f;
    sp8.z = 1000.0f;
    fn_1_6984(arg0, &sp8, 1, 50.0f, arg1);
    sp8.x = 11600.0f;
    sp8.y = -400.0f;
    sp8.z = 1000.0f;
    fn_1_6984(arg0, &sp8, 1, 50.0f, arg1);
}

void fn_1_6984(OMOBJ *arg0, Vec *arg1, s32 arg2, float arg4, u16 arg3)
{
    if (arg2 == 0) {
        arg0->mdlId[0] = Hu3DParticleCreate(lbl_1_bss_2C8, 50);
    } else {
        arg0->mdlId[0] = Hu3DParticleCreate(lbl_1_bss_2C8, 100);
    }
    Hu3DModelCameraSet(arg0->mdlId[0], arg3);
    Hu3DModelAttrSet(arg0->mdlId[0], 1);
    Hu3DModelPosSet(arg0->mdlId[0], arg1->x, arg1->y, arg1->z);
    Hu3DModelLayerSet(arg0->mdlId[0], 5);
    if (arg2 == 0) {
        Hu3DParticleHookSet(arg0->mdlId[0], fn_1_6B18);
    } else {
        Hu3DParticleHookSet(arg0->mdlId[0], fn_1_6F54);
    }
    Hu3DParticleBlendModeSet(arg0->mdlId[0], 0);
    Hu3DParticleColSet(arg0->mdlId[0], 255, 255, 255);
    arg0->trans.x = arg1->x;
    arg0->trans.y = arg1->y;
    arg0->trans.z = arg1->z;
    omSetTra(arg0, arg1->x, arg1->y, arg1->z);
    omSetRot(arg0, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(arg0->mdlId[0], 1);
}

void fn_1_6B18(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *data;
    s32 i;
    s32 j;

    if (particleP->count == 0) {
        data = particleP->data;
        for (i = 0; i < particleP->maxCnt; i++, data++) {
            data->color.a = 0;
            data->scale = 0.0f;
        }
        particleP->dataCnt = 0;
    }
    if (particleP->dataCnt == 0) {
        for (i = 0; i < 2; i++) {
            data = particleP->data;
            for (j = 0; j < particleP->maxCnt; j++, data++) {
                if (data->color.a == 0) {
                    break;
                }
            }
            if (j != particleP->maxCnt) {
                data->vel.x = -50.0f + 0.3921569f * (frand() & 0xFF);
                data->vel.y = 0.058823533f * (frand() & 0xFF);
                data->accel.x = 0.3529412f * (frand() & 0xFF);
                data->accel.y = 5.0f + 0.098039225f * (frand() & 0xFF);
                data->accel.z = 8.0f + frand() % 10;
                data->pos.z = -5.0f + 0.043137256f * (frand() & 0xFF);
                data->pos.y = -5.0f + frand() % 31;
                data->scale = 35.0f + data->vel.y;
                data->color.a = 255;
                data->vel.y = 5.0 + 0.2 * data->vel.y;
                particleP->dataCnt = frand() % 10 + 10;
            }
        }
    } else {
        particleP->dataCnt--;
    }
    data = particleP->data;
    for (i = 0; i < particleP->maxCnt; i++, data++) {
        if (data->color.a != 0) {
            data->pos.x = data->vel.x + data->accel.z * sin(3.141592653589793 * data->accel.x / 180.0);
            data->accel.x += data->accel.y;
            if (data->accel.x >= 360.0f) {
                data->accel.x -= 360.0f;
            }
            data->pos.y += data->vel.y;
            data->color.a--;
        }
    }
}

void fn_1_6F54(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *data;
    s32 i;
    s32 j;

    if (particleP->count == 0) {
        data = particleP->data;
        for (i = 0; i < particleP->maxCnt; i++, data++) {
            data->color.a = 0;
            data->scale = 0.0f;
        }
        particleP->dataCnt = 0;
    }
    if (particleP->dataCnt == 0) {
        for (i = 0; i < 5; i++) {
            data = particleP->data;
            for (j = 0; j < particleP->maxCnt; j++, data++) {
                if (data->color.a == 0) {
                    break;
                }
            }
            if (j != particleP->maxCnt) {
                data->vel.x = -50.0f + 0.3921569f * (frand() & 0xFF);
                data->vel.y = 0.058823533f * (frand() & 0xFF);
                data->accel.x = 0.3529412f * (frand() & 0xFF);
                data->accel.y = 10.0f + 0.098039225f * (frand() & 0xFF);
                data->accel.z = 8.0f + frand() % 10;
                data->pos.z = -5.0f + 0.043137256f * (frand() & 0xFF);
                data->pos.y = -5.0f + frand() % 31;
                data->scale = 45.0f + data->vel.y;
                data->color.a = 255;
                data->vel.y = 15.0 + 0.2 * data->vel.y;
                particleP->dataCnt = frand() % 10 + 10;
            }
        }
    } else {
        particleP->dataCnt--;
    }
    data = particleP->data;
    for (i = 0; i < particleP->maxCnt; i++, data++) {
        if (data->color.a != 0) {
            data->pos.x = data->vel.x + data->accel.z * sin(3.141592653589793 * data->accel.x / 180.0);
            data->accel.x += data->accel.y;
            if (data->accel.x >= 360.0f) {
                data->accel.x -= 360.0f;
            }
            data->pos.y += data->vel.y;
            data->color.a--;
        }
    }
}

void fn_1_7390(void)
{
    lbl_1_bss_4E0.prev = NULL;
    lbl_1_bss_4E0.next = NULL;
}

void fn_1_73B4(u16 arg0, s16 arg1)
{
    M533LISTNODE *head;
    M533LISTNODE *temp_r3;
    M533LISTNODE *temp_r29;

    temp_r3 = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M533LISTNODE), 0x10000000U);
    temp_r3->unk0 = arg0;
    temp_r3->unk2 = arg1;
    head = &lbl_1_bss_4E0;
    temp_r29 = head->next;
    head->next = temp_r3;
    if (NULL != temp_r29) {
        temp_r29->prev = temp_r3;
    }
    temp_r3->prev = head;
    temp_r3->next = temp_r29;
}

s16 fn_1_743C(s16 arg0)
{
    M533LISTNODE *var_r31;
    M533LISTNODE *temp_r29;
    M533LISTNODE *temp_r30;
    s16 temp_r28;

    var_r31 = lbl_1_bss_4E0.next;
    while (NULL != var_r31) {
        if (arg0 == var_r31->unk0) {
            temp_r28 = var_r31->unk2;
            temp_r29 = var_r31->prev;
            temp_r30 = var_r31->next;
            temp_r29->next = temp_r30;
            if (NULL != temp_r30) {
                temp_r30->prev = temp_r29;
            }
            HuMemDirectFree(var_r31);
            return temp_r28;
        }
        var_r31 = var_r31->next;
    }
    return -1;
}

void fn_1_74E0(void)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        lbl_1_bss_2D8[i].unk0 = 0;
        lbl_1_bss_2D8[i].unk4 = -1;
    }
}

s16 fn_1_7538(u32 arg0)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        if (arg0 == lbl_1_bss_2D8[i].unk0) {
            return Hu3DModelLink(lbl_1_bss_2D8[i].unk4);
        }
    }
    for (i = 0; i < 32; i++) {
        if (lbl_1_bss_2D8[i].unk0 == 0U) {
            lbl_1_bss_2D8[i].unk0 = arg0;
            lbl_1_bss_2D8[i].unk4 = Hu3DModelCreate(HuDataSelHeapReadNum(arg0, 0x10000000, HEAP_MODEL));
            return lbl_1_bss_2D8[i].unk4;
        }
    }
    return Hu3DModelCreate(HuDataSelHeapReadNum(arg0, 0x10000000, HEAP_MODEL));
}

void fn_1_764C(float var_f30, Vec *arg0, Vec *arg1, Vec *arg2)
{
    float var_f31;

    if (1.0f < var_f30) {
        var_f30 = 1.0f;
    }
    var_f31 = HuSin(90.0f * var_f30) * HuSin(90.0f * var_f30);
    arg2->x = arg0->x + (var_f31 * (arg1->x - arg0->x));
    arg2->y = arg0->y + (var_f31 * (arg1->y - arg0->y));
    arg2->z = arg0->z + (var_f31 * (arg1->z - arg0->z));
}

void fn_1_77A8(Vec *arg0, Vec *arg1, Vec *arg2)
{
    Hu3DCameraPosSetV(1, arg0, arg1, arg2);
    lbl_1_bss_4D0.v = *arg0;
    lbl_1_bss_4B8 = *arg2;
}

s32 fn_1_7834(float var_f27, Vec *arg0)
{
    Vec sp18;
    Vec spC;
    float var_f31;
    float var_f30;
    float var_f29;
    float var_f28;

    var_f31 = var_f27;
    if (1.0f < var_f31) {
        var_f31 = 1.0f;
    }
    var_f29 = HuSin(90.0f * var_f31) * HuSin(90.0f * var_f31);
    sp18.x = lbl_1_bss_4D0.v.x + (var_f29 * (lbl_1_bss_4C4.x - lbl_1_bss_4D0.v.x));
    sp18.y = lbl_1_bss_4D0.v.y + (var_f29 * (lbl_1_bss_4C4.y - lbl_1_bss_4D0.v.y));
    sp18.z = lbl_1_bss_4D0.v.z + (var_f29 * (lbl_1_bss_4C4.z - lbl_1_bss_4D0.v.z));
    var_f30 = var_f27;
    if (1.0f < var_f30) {
        var_f30 = 1.0f;
    }
    var_f28 = HuSin(90.0f * var_f30) * HuSin(90.0f * var_f30);
    spC.x = lbl_1_bss_4B8.x + (var_f28 * (lbl_1_bss_4AC.x - lbl_1_bss_4B8.x));
    spC.y = lbl_1_bss_4B8.y + (var_f28 * (lbl_1_bss_4AC.y - lbl_1_bss_4B8.y));
    spC.z = lbl_1_bss_4B8.z + (var_f28 * (lbl_1_bss_4AC.z - lbl_1_bss_4B8.z));
    Hu3DCameraPosSetV(1, &sp18, arg0, &spC);
    if (1.0f <= var_f27) {
        return 1;
    }
    lbl_1_bss_3DC = 1;
    return 0;
}

s32 fn_1_7B68(float var_f27, Vec *arg0)
{
    Vec sp18;
    Vec spC;
    s32 i;
    float var_f31;
    float var_f30;
    float var_f29;
    float var_f28;

    for (i = 0; i < 4; i++) {
        var_f31 = var_f27;
        if (1.0f < var_f31) {
            var_f31 = 1.0f;
        }
        var_f29 = HuSin(90.0f * var_f31) * HuSin(90.0f * var_f31);
        sp18.x = lbl_1_bss_47C[i].x + (var_f29 * (lbl_1_bss_44C[i].x - lbl_1_bss_47C[i].x));
        sp18.y = lbl_1_bss_47C[i].y + (var_f29 * (lbl_1_bss_44C[i].y - lbl_1_bss_47C[i].y));
        sp18.z = lbl_1_bss_47C[i].z + (var_f29 * (lbl_1_bss_44C[i].z - lbl_1_bss_47C[i].z));
        var_f30 = var_f27;
        if (1.0f < var_f30) {
            var_f30 = 1.0f;
        }
        var_f28 = HuSin(90.0f * var_f30) * HuSin(90.0f * var_f30);
        spC.x = lbl_1_bss_41C[i].x + (var_f28 * (lbl_1_bss_3EC[i].x - lbl_1_bss_41C[i].x));
        spC.y = lbl_1_bss_41C[i].y + (var_f28 * (lbl_1_bss_3EC[i].y - lbl_1_bss_41C[i].y));
        spC.z = lbl_1_bss_41C[i].z + (var_f28 * (lbl_1_bss_3EC[i].z - lbl_1_bss_41C[i].z));
        Hu3DCameraPosSetV(1 << i, &sp18, arg0, &spC);
    }
    if (1.0f <= var_f27) {
        return 1;
    }
    lbl_1_bss_3DC = 1;
    return 0;
}

float fn_1_7F4C(float var_f29, float arg1, float arg2)
{
    float var_f31;
    float var_f30;

    var_f31 = fmod(arg1 - var_f29, 360.0);
    if (0.0f > var_f31) {
        var_f31 += 360.0f;
    }
    if (180.0f < var_f31) {
        var_f31 -= 360.0f;
    }
    var_f30 = fmod(var_f29 + (var_f31 * arg2), 360.0);
    if (0.0f > var_f30) {
        var_f30 += 360.0f;
    }
    return var_f30;
}

void fn_1_8050(s32 arg0)
{
    if (lbl_1_bss_3E0 >= 0) {
        if (arg0 < 0) {
            arg0 = 1000;
        }
        HuAudSeqFadeOut(lbl_1_bss_3E0, arg0);
    }
    lbl_1_bss_3E0 = -1;
}

void fn_1_80BC(s16 arg0, char *arg1, Vec *arg2)
{
    Mtx sp10;

    Hu3DModelObjMtxGet(arg0, arg1, sp10);
    arg2->x = sp10[0][3];
    arg2->y = sp10[1][3];
    arg2->z = sp10[2][3];
}

u32 fn_1_8114(u32 arg0, u32 arg1)
{
    return arg0 + frandmod(arg1 - arg0);
}

s32 fn_1_8150(Vec *arg0, Vec *arg1, float arg2, float arg3)
{
    if ((((arg1->x - arg0->x) * (arg1->x - arg0->x)) + ((arg1->z - arg0->z) * (arg1->z - arg0->z)))
        < ((arg2 + arg3) * (arg2 + arg3))) {
        return 1;
    }
    return 0;
}

s32 fn_1_81B0(Vec *arg0, s32 arg1, s32 arg2)
{
    Vec sp10;
    s32 var_r31;

    Hu3D3Dto2D(arg0, 1, &sp10);
    var_r31 = arg1 + ((sp10.x / 576.0f) * (arg2 - arg1));
    return var_r31;
}

s32 fn_1_8274(Vec *arg0, s32 arg1, s32 arg2)
{
    Vec sp10;
    s32 var_r31;

    Hu3D3Dto2D(arg0, 1, &sp10);
    var_r31 = arg1 + ((sp10.y / 480.0f) * (arg2 - arg1));
    return var_r31;
}
