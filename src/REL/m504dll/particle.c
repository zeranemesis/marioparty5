#define _MATH_H

#include "REL/m504dll.h"
#include "game/data.h"
#include "game/memory.h"
#include "game/sprite.h"

extern double sin();
extern double cos();

void PSVECAdd(const Vec *a, const Vec *b, Vec *ab);

/* One particle template, copied per emitted particle into the work array. */
typedef struct M504ParTmpl_s {
    u32 flags; /* 0x00 */
    GXColor col; /* 0x04 */
    GXColor colEnd; /* 0x08 */
    Vec vel; /* 0x0C */
    f32 mulX; /* 0x18 */
    f32 mulY;
    f32 mulZ;
    f32 grav; /* 0x24 */
    f32 f28;
    f32 scaleStep; /* 0x2C */
    f32 alphaStep; /* 0x30 */
    f32 colorStep; /* 0x34 */
} M504PARTMPL;

s16 fn_1_D560(s16 camMask, M504PARTMPL *tmpl, f32 x, f32 y, f32 z, f32 life);
void fn_1_D804(void);
void fn_1_DC8C(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);

M504PARTMPL lbl_1_data_4610 = {
    0,
    { 0xA0, 0xA0, 0xA0, 0xFF },
    { 0x00, 0x00, 0x00, 0xFF },
    { 0.0f, 2.0f, 1.0f },
    0.95f,
    0.99f,
    0.95f,
    0.0f,
    0.0f,
    1.0f,
    -4.0f,
    0.0f,
};

s16 lbl_1_bss_7820[16];
M504PARTMPL *lbl_1_bss_7840;

void fn_1_D10C(void)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        lbl_1_bss_7820[i] = -1;
    }
    lbl_1_bss_7840 = NULL;
}

void fn_1_D15C(void)
{
    s32 i;

    if (lbl_1_bss_7840 != NULL) {
        HuMemDirectFree(lbl_1_bss_7840);
        lbl_1_bss_7840 = NULL;
    }
    for (i = 0; i < 16; i++) {
        lbl_1_bss_7820[i] = -1;
    }
    lbl_1_bss_7840 = NULL;
}

void fn_1_D1F0(s16 camMask, Vec *pos)
{
    s32 i;

    for (i = 0; i < 8; i++) {
        lbl_1_data_4610.vel.x = 4.0 * sin(3.141592653589793 * (270.0f + 45.0f * i) / 180.0);
        lbl_1_data_4610.vel.y = 5.0f;
        lbl_1_data_4610.vel.z = 4.0 * cos(3.141592653589793 * (270.0f + 45.0f * i) / 180.0);
        fn_1_D560(camMask, &lbl_1_data_4610, pos->x, pos->y * 1.5f, pos->z, 150.0f);
    }
    for (i = 0; i < 8; i++) {
        lbl_1_data_4610.vel.x = 2.0 * sin(3.141592653589793 * (270.0 + (22.5 + (45.0f * i))) / 180.0);
        lbl_1_data_4610.vel.y = 5.0f;
        lbl_1_data_4610.vel.z = 2.0 * cos(3.141592653589793 * (270.0 + (22.5 + (45.0f * i))) / 180.0);
        fn_1_D560(camMask, &lbl_1_data_4610, pos->x, pos->y * 1.5f, pos->z, 150.0f);
    }
}

s16 fn_1_D560(s16 camMask, M504PARTMPL *tmpl, f32 x, f32 y, f32 z, f32 life)
{
    HU3DPARTICLE *par;
    HU3DPARTICLEDATA *data;
    s16 ret;
    s16 idx;
    s32 i;
    s32 bit;

    fn_1_D804();
    ret = -1;
    bit = 1;
    for (i = 0; i < 16; i++, bit *= 2) {
        if ((bit & camMask) != 0 && Hu3DCamera[i].fov != -1.0f) {
            if (lbl_1_bss_7820[i] == -1) {
                return -1;
            }
            par = Hu3DData[lbl_1_bss_7820[i]].hookData;
            data = &par->data[par->emitCnt];
            idx = par->emitCnt;
            while (idx < par->maxCnt) {
                if (data->scale != 0.0f) {
                    idx++;
                    data++;
                } else {
                    break;
                }
            }
            if (idx >= par->maxCnt) {
                data = par->data;
                idx = 0;
                while (idx < par->maxCnt) {
                    if (data->scale != 0.0f) {
                        idx++;
                        data++;
                    } else {
                        break;
                    }
                }
            }
            if (idx != par->maxCnt) {
                ((M504PARTMPL *)par->work)[idx] = *tmpl;
                data->cameraBit = camMask;
                data->pos.x = x;
                data->pos.y = y;
                data->pos.z = z;
                data->vel = *(HuVecF *)&tmpl->vel;
                data->color = tmpl->col;
                data->scaleBase = life;
                data->scale = life;
                data->time = 0;
                data->parManId = -1;
                par->emitCnt = idx;
                ret = idx;
            }
        }
    }
    return ret;
}

void fn_1_D804(void)
{
    ANIMDATA *anim;
    HU3DPARTICLE *par;
    HU3DPARTICLEDATA *data;
    s32 i;
    s32 j;

    anim = NULL;
    for (i = 0; i < 16; i++) {
        if (Hu3DCamera[i].fov != -1.0f) {
            if (lbl_1_bss_7820[i] != -1) {
                par = Hu3DData[lbl_1_bss_7820[i]].hookData;
                if (par->anim != NULL) {
                    anim = par->anim;
                }
            } else {
                if (anim == NULL) {
                    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x3E0049, 0x10000000, HEAP_MODEL));
                }
                lbl_1_bss_7820[i] = Hu3DParticleCreate(anim, 150);
                Hu3DModelLayerSet(lbl_1_bss_7820[i], 4);
                Hu3DParticleHookSet(lbl_1_bss_7820[i], fn_1_DC8C);
                Hu3DModelCameraSet(lbl_1_bss_7820[i], (u16)(1 << i));
                if (lbl_1_bss_7840 == NULL) {
                    lbl_1_bss_7840 = HuMemDirectMallocNum(HEAP_HEAP, 150 * sizeof(M504PARTMPL), 0x10000000);
                }
                Hu3DParticleBlendModeSet(lbl_1_bss_7820[i], 0);
                par = Hu3DData[lbl_1_bss_7820[i]].hookData;
                par->emitCnt = 0;
                par->work = lbl_1_bss_7840;
                par->count = 1;
                data = par->data;
                for (j = 0; j < par->maxCnt; j++, data++) {
                    data->scale = 0.0f;
                }
            }
        }
    }
}

void fn_1_DA68(void)
{
    ANIMDATA *anim;
    HU3DPARTICLE *par;
    HU3DPARTICLEDATA *data;
    s32 i;
    s32 j;

    anim = NULL;
    for (i = 0; i < 16; i++) {
        if (Hu3DCamera[i].fov != -1.0f && lbl_1_bss_7820[i] == -1) {
            if (anim == NULL) {
                anim = HuSprAnimRead(HuDataSelHeapReadNum(0x3E0049, 0x10000000, HEAP_MODEL));
            }
            lbl_1_bss_7820[i] = Hu3DParticleCreate(anim, 150);
            Hu3DModelLayerSet(lbl_1_bss_7820[i], 4);
            Hu3DParticleHookSet(lbl_1_bss_7820[i], fn_1_DC8C);
            Hu3DModelCameraSet(lbl_1_bss_7820[i], (u16)(1 << i));
            if (lbl_1_bss_7840 == NULL) {
                lbl_1_bss_7840 = HuMemDirectMallocNum(HEAP_HEAP, 150 * sizeof(M504PARTMPL), 0x10000000);
            }
            Hu3DParticleBlendModeSet(lbl_1_bss_7820[i], 0);
            par = Hu3DData[lbl_1_bss_7820[i]].hookData;
            par->emitCnt = 0;
            par->work = lbl_1_bss_7840;
            par->count = 1;
            data = par->data;
            for (j = 0; j < par->maxCnt; j++, data++) {
                data->scale = 0.0f;
            }
        }
    }
}

void fn_1_DC8C(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    M504PARTMPL *work;
    HU3DPARTICLEDATA *data;
    s16 i;
    s16 c;
    s16 a;
    f64 flick;

    work = particleP->work;
    if (particleP->count == 0) {
        data = particleP->data;
        for (i = 0; i < particleP->maxCnt; i++, data++) {
            data->scale = 0.0f;
        }
    }
    data = particleP->data;
    for (i = 0; i < particleP->maxCnt; i++, data++) {
        if (data->scale != 0.0f) {
            data->vel.x *= work[i].mulX;
            data->vel.y *= work[i].mulY;
            data->vel.z *= work[i].mulZ;
            PSVECAdd((Vec *)&data->vel, (Vec *)&data->pos, (Vec *)&data->pos);
            data->vel.y += work[i].grav;
            c = (s16)((f32)data->color.r + (work[i].colorStep * (f32)(work[i].colEnd.r - work[i].col.r)));
            if (c < 0) {
                c = 0;
            } else if (c > 255) {
                c = 255;
            }
            data->color.r = c;
            c = (s16)((f32)data->color.g + (work[i].colorStep * (f32)(work[i].colEnd.g - work[i].col.g)));
            if (c < 0) {
                c = 0;
            } else if (c > 255) {
                c = 255;
            }
            data->color.g = c;
            c = (s16)((f32)data->color.b + (work[i].colorStep * (f32)(work[i].colEnd.b - work[i].col.b)));
            if (c < 0) {
                c = 0;
            } else if (c > 255) {
                c = 255;
            }
            data->color.b = c;
            a = (s16)((f32)data->color.a + work[i].alphaStep);
            if (a < 1) {
                data->scale = 0.0f;
            }
            data->color.a = a;
            if (data->scale != 0.0f) {
                if ((work[i].flags & 1) != 0) {
                    if (((data->time + i) & 1) != 0) {
                        flick = 1.0;
                    } else {
                        flick = 0.5;
                    }
                    data->scale = data->scaleBase * flick;
                } else {
                    data->scale = data->scaleBase;
                }
                data->scaleBase += work[i].scaleStep;
                if (data->scaleBase <= 0.01f) {
                    data->scale = 0.0f;
                }
            }
            data->time++;
        }
    }
    DCStoreRangeNoSync(particleP->data, particleP->maxCnt * sizeof(HU3DPARTICLEDATA));
}
