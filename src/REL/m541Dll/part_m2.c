#include "REL/m541Dll.h"
#include "game/printfunc.h"

/* debug model pose editor: pad moves the model, prints the values */
s32 fn_1_1A70(M541_STAGE *stage, s16 pad, s16 idx)
{
    s32 ret = 0;
    M541_MODEL *m = &stage->model[idx];

    if (HuPadStkX[pad] > 10) {
        m->posX += 1.0f;
    }
    if (HuPadStkX[pad] < -10) {
        m->posX -= 1.0f;
    }
    if (HuPadStkY[pad] > 10) {
        m->posY += 1.0f;
    }
    if (HuPadStkY[pad] < -10) {
        m->posY -= 1.0f;
    }
    if (HuPadTrigL[pad] > 10) {
        m->posZ += 1.0f;
    }
    if (HuPadTrigR[pad] > 10) {
        m->posZ -= 1.0f;
    }
    if (HuPadSubStkX[pad] > 10) {
        m->rotX += 1.0f;
    }
    if (HuPadSubStkX[pad] < -10) {
        m->rotX -= 1.0f;
    }
    if (HuPadSubStkY[pad] > 10) {
        m->rotY += 1.0f;
    }
    if (HuPadSubStkY[pad] < -10) {
        m->rotY -= 1.0f;
    }
    if (HuPadBtnRep[pad] & 0x100) {
        m->rotZ += 1.0f;
    }
    if (HuPadBtnRep[pad] & 0x200) {
        m->rotZ -= 1.0f;
    }
    print8(25, 136, 1.5f, "OBJ X:%.1f Y:%.1f Z:%.1f RX%.1f RY%.1f RZ%.1f", m->posX, m->posY, m->posZ, m->rotX, m->rotY, m->rotZ);
    print8(25, 154, 1.5f, "OBJW %d", idx);
    return ret;
}

void fn_1_1DC4(void)
{
}

/* debug camera editor */
#define DEADZONE(v, lim) ((((v) < 0) ? -(v) : (v)) < (lim) ? 0 : (v))

void fn_1_1DC8(s16 pad)
{
    u16 btn = HuPadBtn[pad];
    s32 btnDown = HuPadBtnDown[pad];
    s8 stkX = HuPadStkX[pad];
    s8 stkY;
    s8 subX;
    s8 subY;

    stkX = DEADZONE(stkX, 10);
    stkY = HuPadStkY[pad];
    stkY = DEADZONE(stkY, 10);
    subX = HuPadSubStkX[pad];
    subX = DEADZONE(subX, 8);
    subY = HuPadSubStkY[pad];
    subY = DEADZONE(subY, 8);

    if (btn & 0x400) {
        CZoom += 20.0f;
    }
    if (btn & 0x800) {
        CZoom -= 20.0f;
    }
    if (stkX > 0) {
        Center.x += 10.0f;
    }
    if (stkX < 0) {
        Center.x -= 10.0f;
    }
    if (stkY > 0) {
        Center.y += 10.0f;
    }
    if (stkY < 0) {
        Center.y -= 10.0f;
    }
    if (subX > 0) {
        CRot.y += 1.0f;
    }
    if (subX < 0) {
        CRot.y -= 1.0f;
    }
    if (subY > 0) {
        CRot.x += 1.0f;
    }
    if (subY < 0) {
        CRot.x -= 1.0f;
    }
    if (btn & 0x2000) {
        CRot.z += 1.0f;
    }
    if (btn & 0x4000) {
        CRot.z -= 1.0f;
    }
    if (btn & 0x8) {
        Center.z += 10.0f;
    }
    if (btn & 0x4) {
        Center.z -= 10.0f;
    }
    print8(25, 30, 1.5f, "CAMPOS X:%.1f Y:%.1f Z:%.1f ZOOM%.1f", Center.x, Center.y, Center.z, CZoom);
    print8(25, 48, 1.5f, "CAMROT X:%.1f Y:%.1f Z:%.1f", CRot.x, CRot.y, CRot.z);
}

/* save the model's material colours */
void fn_1_2280(s16 idx)
{
    HU3DMODEL *mdl = &Hu3DData[lbl_1_bss_0.player[idx]->actor->mdlId];
    HSFMATERIAL *mat;
    u8 *buf;
    s16 i;

    buf = HuMemDirectMallocNum(HEAP_HEAP, mdl->hsf->materialNum * 3, HU_MEMNUM_OVL);
    lbl_1_bss_2614[idx].colBuf = buf;
    mat = mdl->hsf->material;
    for (i = 0; i < mdl->hsf->materialNum; i++, mat++) {
        *buf++ = mat->color[0];
        *buf++ = mat->color[1];
        *buf++ = mat->color[2];
    }
}

/* fade the material colours; returns TRUE once the fade level is below 40 */
s32 fn_1_2378(s16 idx, float scale)
{
    s32 done = 0;
    HU3DMODEL *mdl = &Hu3DData[lbl_1_bss_0.player[idx]->actor->mdlId];
    HSFMATERIAL *mat;
    s16 i;

    lbl_1_bss_2614[idx].tpLvl *= scale;
    if (lbl_1_bss_2614[idx].tpLvl < 40.0f) {
        done = 1;
    }
    mat = mdl->hsf->material;
    for (i = 0; i < mdl->hsf->materialNum; i++, mat++) {
        mat->color[0] = mat->color[0] * scale;
        mat->color[1] = mat->color[1] * scale;
        mat->color[2] = mat->color[2] * scale;
    }
    return done;
}

/* restore the saved material colours */
void fn_1_24E8(s16 idx)
{
    HU3DMODEL *mdl;
    HSFMATERIAL *mat;
    u8 *buf;
    s16 i;

    lbl_1_bss_2614[idx].tpLvl = 255.0f;
    mdl = &Hu3DData[lbl_1_bss_0.player[idx]->actor->mdlId];
    buf = lbl_1_bss_2614[idx].colBuf;
    mat = mdl->hsf->material;
    for (i = 0; i < mdl->hsf->materialNum; i++, mat++) {
        mat->color[0] = *buf++;
        mat->color[1] = *buf++;
        mat->color[2] = *buf++;
    }
}

void fn_1_25E4(void)
{
    lbl_1_bss_2734.jingleId = HuAudJinglePlay(0x83);
}

void fn_1_2614(void)
{
    lbl_1_bss_2734.bgmId = HuAudBGMPlay(0x56);
}

void fn_1_2644(s16 which, s16 speed)
{
    s16 id;

    if (which != 0) {
        id = lbl_1_bss_2734.bgmId;
    } else {
        id = lbl_1_bss_2734.jingleId;
    }
    HuAudSStreamFadeOut(id, speed);
}

void fn_1_26AC(s16 which)
{
    s16 id;

    if (which != 0) {
        id = lbl_1_bss_2734.bgmId;
    } else {
        id = lbl_1_bss_2734.jingleId;
    }
    HuAudSStreamStop(id);
}

void fn_1_270C(s16 idx)
{
    if ((idx == 0 || idx == 1 || idx == 2) && lbl_1_bss_2734.playerHook[idx] != -1) {
        return;
    }
    lbl_1_bss_2734.playerHook[idx] = 1;
    lbl_1_bss_2734.fxId[idx] = HuAudFXPlay(lbl_1_data_360[idx]);
}

void fn_1_27C8(s16 idx, s16 vol, s16 pan)
{
    if ((idx == 0 || idx == 1 || idx == 2) && lbl_1_bss_2734.playerHook[idx] != -1) {
        return;
    }
    lbl_1_bss_2734.playerHook[idx] = 1;
    lbl_1_bss_2734.fxId[idx] = HuAudFXPlayVolPan(lbl_1_data_360[idx], vol, pan);
}

void fn_1_2894(s16 idx, s16 pan)
{
    if (lbl_1_bss_2734.playerHook[idx] == 1 && HuAudFXStatusGet(lbl_1_bss_2734.fxId[idx]) == 2) {
        HuAudFXPanning(lbl_1_bss_2734.fxId[idx], pan);
    }
}

void fn_1_2924(s16 idx)
{
    if (lbl_1_bss_2734.playerHook[idx] == 1
        && (HuAudFXStatusGet(lbl_1_bss_2734.fxId[idx]) == 2 || HuAudFXStatusGet(lbl_1_bss_2734.fxId[idx]) == 3)) {
        lbl_1_bss_2734.playerHook[idx] = -1;
        HuAudFXStop(lbl_1_bss_2734.fxId[idx]);
    }
}

void fn_1_29EC(s16 idx)
{
    if (lbl_1_bss_2734.playerHook[idx] == 1
        && (HuAudFXStatusGet(lbl_1_bss_2734.fxId[idx]) == 2 || HuAudFXStatusGet(lbl_1_bss_2734.fxId[idx]) == 3)) {
        lbl_1_bss_2734.playerHook[idx] = -1;
        HuAudFXStop(lbl_1_bss_2734.fxId[idx]);
    }
}

void fn_1_2AB4(void)
{
    HuAudAllStop();
}
