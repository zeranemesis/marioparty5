#include "REL/m502dll.h"

void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, int wrapS, int wrapT, int filter);

void fn_1_2878(HU3DDRAWOBJ *drawObj);

static s16 lbl_1_data_B80[4] = { 0, 1, 2, 3 };
static s16 lbl_1_data_B88[8] = { 4, 5, 6, 7, 8, 9, 0xA, 0xB };
static char *lbl_1_data_C00[8] = {
    "model-hasira", "model-kabe00", "model-kabe01", "model-kabe02",
    "model-kabe04", "model-yuka00", "model-yuka01", "model-yuka02",
};
static f32 lbl_1_data_C20[2][3] = { { 0.03f, 0.0f, 0.0f }, { 0.0f, 0.04f, 0.0f } };

static f32 lbl_1_bss_5038;
static f32 lbl_1_bss_503C;
static f32 lbl_1_bss_5040;
static s32 lbl_1_bss_5044;

void fn_1_21B8(M502STAGE *st, s32 playerNo, M502PLAYER *player)
{
    M502MODELS *mdl;
    s32 i;
    s16 m;

    mdl = &player->main->models;
    memset(st, 0, sizeof(M502STAGE));
    st->player = player;
    st->rate = 0.002f;
    for (i = 0; i < 4; i++) {
        m = Hu3DModelLink(mdl->model[lbl_1_data_B80[i]]);
        st->model[i] = m;
        Hu3DModelAttrSet(m, 1);
        Hu3DModelAttrSet(m, 0x40000001);
        Hu3DModelPosSet(m, 0.0f, 0.0f, 0.0f);
        Hu3DModelRotSet(m, 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(m, (u16)lbl_1_data_B60[playerNo]);
        Hu3DModelLayerSet(m, 1);
        Hu3DModelAttrReset(m, 1);
    }
    for (i = 0; i < 8; i++) {
        st->modelNo[i] = lbl_1_data_B88[i];
        st->name[i] = lbl_1_data_C00[i];
    }
    for (i = 0; i < 5; i++) {
        st->wall[i] = Hu3DModelLink(mdl->model[0x81 + i]);
        Hu3DModelAttrSet(st->wall[i], 1);
        Hu3DModelAttrSet(st->wall[i], 0x40000001);
        Hu3DModelPosSet(st->wall[i], 300.0f * (f32)i - 622.5f, -9300.0f, -5.0f);
        Hu3DModelRotSet(st->wall[i], 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(st->wall[i], (u16)lbl_1_data_B60[playerNo + 4]);
        Hu3DModelLayerSet(st->wall[i], 4);
        Hu3DModelMatHookSet(st->wall[i], (HU3DMATHOOK)fn_1_2878);
        Hu3DModelAttrReset(st->wall[i], 1);
    }
    for (i = 0; i < 3; i++) {
        st->flame[i] = Hu3DModelLink(mdl->model[0x86 + i]);
        Hu3DModelAttrSet(st->flame[i], 1);
        Hu3DModelAttrSet(st->flame[i], 0x40000001);
        Hu3DModelPosSet(st->flame[i], 300.0f * (f32)i - 600.5f, -9300.0f, -5.0f);
        Hu3DModelScaleSet(st->flame[i], 1.3f, 0.0f, 1.0f);
        Hu3DModelRotSet(st->flame[i], 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(st->flame[i], (u16)lbl_1_data_B60[playerNo + 4]);
        Hu3DModelLayerSet(st->flame[i], 4);
    }
    st->water = mdl->model[0x89 + playerNo];
    Hu3DModelAttrSet(st->water, 1);
    Hu3DMotionShapeSpeedSet(st->water, 0.0f);
    Hu3DMotionShapeTimeSet(st->water, 0.01f);
    Hu3DModelScaleSet(st->water, 1.0f, 1.0f, 1.0f);
    Hu3DModelRotSet(st->water, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(st->water, (u16)lbl_1_data_B60[playerNo + 4]);
    Hu3DModelLayerSet(st->water, 4);
}

void fn_1_2704(M502STAGE *st)
{
    s32 i;

    for (i = 0; i < 5; i++) {
        Hu3DModelKill(st->wall[i]);
        st->wall[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        Hu3DModelKill(st->model[i]);
    }
    memset(st, 0, sizeof(M502STAGE));
}

void fn_1_27A8(M502STAGE *st)
{
}

/* layer hook: takes no argument on GC */
void fn_1_27AC(void)
{
    HU3DWATER *water;
    s32 paused;

    water = &Hu3DWaterData;
    if ((Hu3DCameraBit & 8) != 0) {
        GXSetTexCopySrc(0, 0, 640, 480);
        GXSetTexCopyDst(320, 240, GX_TF_RGB565, GX_TRUE);
        GXCopyTex(lbl_1_data_0->models.texBuf, GX_FALSE);
        paused = omUPauseFlag + omPauseChk();
        if (paused == 0) {
            lbl_1_bss_5038 += 0.002f;
        }
    }
}

void fn_1_2878(HU3DDRAWOBJ *drawObj)
{
    GXColor col = { 0x40, 0x40, 0x40, 0x18 };
    GXColor c2;
    HU3DCAMERA *cam;
    M502STAGE *st;
    s32 paused;
    GXTexObj tex;
    Mtx lightMtx;
    Mtx invMtx;
    Mtx texMtx;
    Mtx transMtx;
    Mtx scaleMtx;

    cam = &Hu3DCamera[Hu3DCameraNo];
    st = &lbl_1_data_0->player[Hu3DCameraNo - 4].stage;
    paused = omUPauseFlag + omPauseChk();
    if (paused == 0) {
        st->scroll += st->rate;
    }
    GXInitTexObj(&tex, lbl_1_data_0->models.texBuf, 320, 240, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&tex, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&tex, GX_TEXMAP1);
    HuSprTexLoad(lbl_1_data_0->models.anim1, 0, 2, 1, 1, 1);
    HuSprTexLoad(lbl_1_data_0->models.anim2, 0, 3, 1, 1, 1);
    GXSetNumTexGens(3);
    GXSetNumTevStages(3);
    C_MTXLightPerspective(lightMtx, cam->fov, 1.3333334f, 0.5f, -0.5f, (f32)(0.125 + 0.25 * (f64)(Hu3DCameraNo - 4)), 0.5f);
    PSMTXInverse(Hu3DCameraMtx, invMtx);
    PSMTXConcat(invMtx, drawObj->matrix, transMtx);
    PSMTXConcat(lightMtx, Hu3DCameraMtx, texMtx);
    PSMTXConcat(texMtx, transMtx, texMtx);
    GXLoadTexMtxImm(texMtx, 0x21, GX_MTX3x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, 0x21, GX_FALSE, GX_PTIDENTITY);
    PSMTXTrans(transMtx, lbl_1_bss_5038 * 0.5f, st->scroll * 0.25f, 0.0f);
    PSMTXScale(scaleMtx, 0.8f, 0.8f, 1.0f);
    PSMTXConcat(scaleMtx, transMtx, texMtx);
    GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    c2 = col;
    GXSetTevColor(GX_TEVREG0, c2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD2, GX_TEXMAP0, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP3, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_TEXC, GX_CC_A0, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP2);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_0);
    GXSetIndTexMtx(GX_ITM_0, lbl_1_data_C20, -1);
}

void fn_1_2E08(M502STAGE *st, f32 speed)
{
    s32 idx[4] = { 0, 1, 2, 1 };
    s32 lane;

    lane = st->player->plr.lane + 1;
    switch (st->state) {
    case 0:
        st->scaleX = 1.0f;
        st->scaleY = 1.0f;
        st->scaleY2 = 0.0f;
        st->offY = 0.0f;
        st->state++;
        st->flameIdx = 0;
        lbl_1_bss_5044 = 0;
        Hu3DModelAttrReset(st->flame[idx[st->flameIdx]], 1);
        return;
    case 1:
        lbl_1_bss_5044 = (lbl_1_bss_5044 + 1) & 1;
        st->scaleY += speed;
        if (st->scaleY > 6.0f) {
            st->scaleY = 6.0f;
            st->state++;
        }
        st->rate = 0.002f + 0.003f * (st->scaleY - 1.0f);
        if (st->scaleY < 3.0f) {
            st->scaleY2 = 1.2f * (st->scaleY - 1.0f) * 0.5f;
        } else {
            st->scaleY2 = 1.2f;
        }
        if (st->scaleY > 1.0f) {
            st->scaleX = 1.1f;
        }
        if (st->scaleY > 2.0f) {
            st->scaleX = 1.2f;
        }
        if (st->scaleY > 3.0f) {
            st->scaleX = 1.3f;
        }
        st->offY = 300.0 * ((f64)st->scaleY - 1.0);
        Hu3DModelPosSet(st->wall[lane], 300.0f * (f32)lane - 622.5f, -9300.0f + st->offY, 5.0f);
        Hu3DModelScaleSet(st->wall[lane], st->scaleX, st->scaleY, 1.0f);
        if (lbl_1_bss_5044 != 0) {
            Hu3DModelAttrSet(st->flame[idx[st->flameIdx]], 1);
            st->flameIdx = (st->flameIdx + 1) & 3;
            Hu3DModelAttrReset(st->flame[idx[st->flameIdx]], 1);
        }
        Hu3DModelPosSet(st->flame[idx[st->flameIdx]], 300.0f * (f32)lane - 600.0f, (-9300.0f + st->offY) - 2.0f, 5.0f);
        Hu3DModelScaleSet(st->flame[idx[st->flameIdx]], 1.3f, st->scaleY2, 1.0f);
        return;
    }
}

void fn_1_3298(M502STAGE *st)
{
    s32 lane;

    lane = st->player->plr.lane + 1;
    switch (st->state2) {
    case 0:
        Hu3DModelAttrReset(st->water, 1);
        Hu3DMotionShapeSpeedSet(st->water, 1.0f);
        Hu3DModelPosSet(st->water, 300.0f * (f32)lane - 600.0f, -9301.0f, 0.0f);
        st->state2++;
    case 1:
        return;
    }
}

void fn_1_3380(f32 v)
{
    lbl_1_bss_5040 = v;
}

void fn_1_3390(HU3DDRAWOBJ *drawObj)
{
    GXColor col = { 0x60, 0x60, 0x60, 9 };
    GXColor c2;
    s32 paused;
    GXTexObj tex;
    Mtx lightMtx;
    Mtx invMtx;
    Mtx texMtx;
    Mtx transMtx;
    Mtx scaleMtx;

    paused = omUPauseFlag + omPauseChk();
    if (paused == 0) {
        lbl_1_bss_503C += lbl_1_bss_5040;
    }
    GXInitTexObj(&tex, lbl_1_data_0->models.texBuf, 320, 240, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&tex, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&tex, GX_TEXMAP1);
    HuSprTexLoad(lbl_1_data_0->models.anim1, 0, 2, 1, 1, 1);
    HuSprTexLoad(lbl_1_data_0->models.anim2, 0, 3, 1, 1, 1);
    GXSetNumTexGens(3);
    GXSetNumTevStages(3);
    C_MTXLightPerspective(lightMtx, Hu3DCamera[Hu3DCameraNo].fov, 1.3333334f, 0.5f, -0.5f, (f32)(0.5 + (f64)(f32)(Hu3DCameraNo - 4)), 0.5f);
    PSMTXInverse(Hu3DCameraMtx, invMtx);
    PSMTXConcat(invMtx, drawObj->matrix, transMtx);
    PSMTXConcat(lightMtx, Hu3DCameraMtx, texMtx);
    PSMTXConcat(texMtx, transMtx, texMtx);
    GXLoadTexMtxImm(texMtx, 0x21, GX_MTX3x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, 0x21, GX_FALSE, GX_PTIDENTITY);
    PSMTXTrans(transMtx, 0.0f, lbl_1_bss_503C, 0.0f);
    PSMTXScale(scaleMtx, 0.8f, 0.8f, 1.0f);
    PSMTXConcat(scaleMtx, transMtx, texMtx);
    GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    c2 = col;
    GXSetTevColor(GX_TEVREG0, c2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD2, GX_TEXMAP0, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP3, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_TEXC, GX_CC_A0, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP2);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_0);
    GXSetIndTexMtx(GX_ITM_0, lbl_1_data_C20, -1);
}
