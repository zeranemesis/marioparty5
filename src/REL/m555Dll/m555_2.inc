#include "game/audio.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/printfunc.h"
#include "game/sprite.h"
#include "dolphin/mtx.h"
#include "dolphin/os.h"

void fn_1_1FC0(void)
{
}

/* Debug camera editor: pad buttons move the camera centre, rotation and zoom. */
void fn_1_1FC4(s16 padNo)
{
    u16 btn = HuPadBtn[padNo];
    s8 sx, sy, cx, cy;
    s8 ax, ay, acx, acy;
    s8 lx, ly, lcx, lcy;

    sx = HuPadStkX[padNo];
    ax = (sx >= 0) ? sx : -sx;
    lx = (ax < 10) ? 0 : sx;
    sy = HuPadStkY[padNo];
    ay = (sy >= 0) ? sy : -sy;
    ly = (ay < 10) ? 0 : sy;
    cx = HuPadSubStkX[padNo];
    acx = (cx >= 0) ? cx : -cx;
    lcx = (acx < 8) ? 0 : cx;
    cy = HuPadSubStkY[padNo];
    acy = (cy >= 0) ? cy : -cy;
    lcy = (acy < 8) ? 0 : cy;
    if (btn & 0x400) {
        CZoom += 20.0f;
    }
    if (btn & 0x800) {
        CZoom -= 20.0f;
    }
    if (lx > 0) {
        Center.x += 10.0f;
    }
    if (lx < 0) {
        Center.x -= 10.0f;
    }
    if (ly > 0) {
        Center.y += 10.0f;
    }
    if (ly < 0) {
        Center.y -= 10.0f;
    }
    if (lcx > 0) {
        CRot.y += 1.0f;
    }
    if (lcx < 0) {
        CRot.y -= 1.0f;
    }
    if (lcy > 0) {
        CRot.x += 1.0f;
    }
    if (lcy < 0) {
        CRot.x -= 1.0f;
    }
    if (btn & 0x2000) {
        CRot.z += 1.0f;
    }
    if (btn & 0x4000) {
        CRot.z -= 1.0f;
    }
    if (btn & 8) {
        Center.z += 10.0f;
    }
    if (btn & 4) {
        Center.z -= 10.0f;
    }
    print8(0x19, 0x1E, 1.5f, "CAMPOS X:%.1f Y:%.1f Z:%.1f ZOOM%.1f", Center.x, Center.y, Center.z, CZoom);
    print8(0x19, 0x30, 1.5f, lbl_1_data_29B, CRot.x, CRot.y, CRot.z);
}

/* Create the dust/spark particle model and clear its slots. */
void fn_1_247C(void)
{
    s16 i;
    HU3DPARTICLE *p;
    HU3DPARTICLEDATA *d;

    lbl_1_bss_3028 = Hu3DParticleCreate(HuSprAnimRead(HuDataSelHeapReadNum(0x710035, 0x10000000, 2)), 0x3C);
    Hu3DParticleBlendModeSet(lbl_1_bss_3028, 0);
    Hu3DParticleColSet(lbl_1_bss_3028, 0xFF, 0xFF, 0xFF);
    Hu3DParticleScaleSet(lbl_1_bss_3028, 0.0f);
    Hu3DParticleHookSet(lbl_1_bss_3028, fn_1_28F0);
    Hu3DModelLayerSet(lbl_1_bss_3028, 4);
    Hu3DModelAttrSet(lbl_1_bss_3028, 1);
    Hu3DModelAttrReset(lbl_1_bss_3028, 1);
    p = Hu3DData[lbl_1_bss_3028].hookData;
    d = p->data;
    for (i = 0; i < p->maxCnt; i++, d++) {
        d->time = 0;
    }
}

/* Spawn one jittered particle at *pos. */
void fn_1_25D8(HuVecF *pos)
{
    HuVecF p, v;
    f32 scale;
    f32 scaleBase;
    s16 r;

    p = *pos;
    r = frandmod(0x14);
    p.x = pos->x + (r - 10);
    r = frandmod(0x14);
    p.y = pos->y + (r - 10);
    r = frandmod(0x14);
    p.z = pos->z + (r - 10);
    v.x = 0.0f;
    v.y = 0.0f;
    r = frandmod(2);
    v.z = r + 15;
    r = frandmod(9);
    scale = r + 34;
    scaleBase = 2.0f;
    fn_1_2800(&p, &v, scale, scaleBase);
}

s32 fn_1_2800(HuVecF *pos, HuVecF *vel, f32 scale, f32 scaleBase)
{
    s16 i;
    HU3DPARTICLE *p = Hu3DData[lbl_1_bss_3028].hookData;
    HU3DPARTICLEDATA *d = p->data;

    for (i = 0; i < p->maxCnt; i++, d++) {
        if (d->time == 0) {
            d->pos = *pos;
            d->vel = *vel;
            d->color.r = 0xFF;
            d->color.g = 0xFF;
            d->color.b = 0xFF;
            d->color.a = 0xFF;
            d->scaleBase = scaleBase;
            d->scale = scale;
            d->time = 1;
            d->parManId = 0x1B;
            break;
        }
    }
    return 0;
}

void fn_1_28F0(HU3DMODEL *model, HU3DPARTICLE *p, Mtx mtx)
{
    s16 i;
    s16 t;
    HU3DPARTICLEDATA *d = p->data;

    for (i = 0; i < p->maxCnt; i++, d++) {
        if (d->time != 0) {
            PSVECAdd(&d->vel, &d->pos, &d->pos);
            d->color.r = d->parManId * 7;
            d->color.g = d->parManId * 7;
            d->color.b = d->parManId * 7;
            d->color.a = d->parManId * 7;
            d->scale += d->scaleBase;
            t = d->parManId;
            d->parManId = t - 1;
            if (t <= 0) {
                d->time = 0;
            }
        }
    }
    DCStoreRangeNoSync(p->data, p->maxCnt * sizeof(HU3DPARTICLEDATA));
}

void fn_1_29E8(void)
{
    lbl_1_bss_1894.jingleHandle = HuAudJinglePlay(0x83);
}

void fn_1_2A18(void)
{
    lbl_1_bss_1894.bgmHandle = HuAudBGMPlay(0x35);
}

void fn_1_2A48(s16 bgm, s16 speed)
{
    s16 h;

    if (bgm != 0) {
        h = lbl_1_bss_1894.bgmHandle;
    } else {
        h = lbl_1_bss_1894.jingleHandle;
    }
    HuAudSStreamFadeOut(h, speed);
}

void fn_1_2AB0(s16 bgm)
{
    s16 h;

    if (bgm != 0) {
        h = lbl_1_bss_1894.bgmHandle;
    } else {
        h = lbl_1_bss_1894.jingleHandle;
    }
    HuAudSStreamStop(h);
}

#define M555_FX_FREE(id) (((id) != 2 && (id) != 3 && (id) != 6 && (id) != 7) || lbl_1_bss_1894.hookIdx[id] == -1)

void fn_1_2B10(s16 id)
{
    if (M555_FX_FREE(id)) {
        lbl_1_bss_1894.hookIdx[id] = 1;
        lbl_1_bss_1894.fxHandle[id] = HuAudFXPlay(lbl_1_data_2B8[id]);
    }
}

void fn_1_2BD8(s16 id, s16 vol, s16 pan)
{
    if (M555_FX_FREE(id)) {
        lbl_1_bss_1894.hookIdx[id] = 1;
        lbl_1_bss_1894.fxHandle[id] = HuAudFXPlayVolPan(lbl_1_data_2B8[id], vol, pan);
    }
}

void fn_1_2CB0(s16 id, s16 pan)
{
    if (lbl_1_bss_1894.hookIdx[id] == 1 && HuAudFXStatusGet(lbl_1_bss_1894.fxHandle[id]) == 2) {
        HuAudFXPanning(lbl_1_bss_1894.fxHandle[id], pan);
    }
}

void fn_1_2D40(s16 id, s16 pitch)
{
    if (lbl_1_bss_1894.hookIdx[id] == 1 && HuAudFXStatusGet(lbl_1_bss_1894.fxHandle[id]) == 2) {
        HuAudFXPitchSet(lbl_1_bss_1894.fxHandle[id], pitch);
    }
}

void fn_1_2DD0(s16 id, s16 vol)
{
    if (lbl_1_bss_1894.hookIdx[id] == 1 && HuAudFXStatusGet(lbl_1_bss_1894.fxHandle[id]) == 2) {
        HuAudFXVolSet(lbl_1_bss_1894.fxHandle[id], vol);
    }
}

void fn_1_2E60(s16 id, s16 fade)
{
    if (lbl_1_bss_1894.hookIdx[id] == 1
        && (HuAudFXStatusGet(lbl_1_bss_1894.fxHandle[id]) == 2 || HuAudFXStatusGet(lbl_1_bss_1894.fxHandle[id]) == 3)) {
        lbl_1_bss_1894.hookIdx[id] = -1;
        HuAudFXStop(lbl_1_bss_1894.fxHandle[id]);
    }
}

void fn_1_2F28(s16 id)
{
    if (lbl_1_bss_1894.hookIdx[id] == 1
        && (HuAudFXStatusGet(lbl_1_bss_1894.fxHandle[id]) == 2 || HuAudFXStatusGet(lbl_1_bss_1894.fxHandle[id]) == 3)) {
        lbl_1_bss_1894.hookIdx[id] = -1;
        HuAudFXStop(lbl_1_bss_1894.fxHandle[id]);
    }
}

void fn_1_2FF0(void)
{
    HuAudAllStop();
}
