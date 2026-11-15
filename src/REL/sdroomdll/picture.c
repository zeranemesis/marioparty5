#include "REL/sdroomdll/picture.h"
#include "REL/sdroomdll/sdroom.h"
#include "game/window.h"
#include "game/sprite.h"
#include "game/memory.h"
#include "game/hsfex.h"
#include "game/init.h"
#include "humath.h"
#include <stdio.h>

extern void fn_1_7298(void);
extern void fn_1_7320(void);
extern void fn_1_16024(void);
extern void fn_1_16064(void);

static void fn_1_19928(void);
static void fn_1_19FC0(void);
static void fn_1_1A17C(void);
static void fn_1_1A528(s16 no);
static void fn_1_1A850(s16 layerNo);
static void fn_1_1A958(void);
static void fn_1_1B640(void);
static void fn_1_1B72C(s16 no);
static void fn_1_1BCD4(s16 no);
static void fn_1_1BDF8(f32 alpha);

/* One photo slot: preview model, its texture, and the capture buffer. */
typedef struct PicSlot_s {
    s16 mdl;
    s16 anim;
    ANIMDATA *animData;
    void *buf;
} PICSLOT;

/* picture bss (0xA90..0xC00) */
static f32 lbl_1_bss_A90; /* fade alpha */
static char lbl_1_bss_A94[10]; /* number string */
static s16 lbl_1_bss_A9E; /* capture slot, -1 none */
static s16 lbl_1_bss_AA0; /* picture count */
static SDMACHINE_DISP lbl_1_bss_AA4;
static s16 lbl_1_bss_AD0; /* hint window */
static s16 lbl_1_bss_AD2; /* message window */
static PICSLOT lbl_1_bss_AD4[11];
static s16 lbl_1_bss_BF0; /* cursor sprite group */
static s16 lbl_1_bss_BF8[3]; /* caption windows */

static OMCAMERAVIEW lbl_1_data_1860 = { { 433.0f, 200.0f, 250.0f }, { 0.0f, 240.0f, 0.0f }, 840.0f };
static OMCAMERAVIEW lbl_1_data_187C = { { 0.0f, 100.0f, 0.0f }, { 330.0f, 0.0f, 0.0f }, 5300.0f };

static Vec lbl_1_data_1898[1] = {
    { 1314.59998f, 335.0f, 759.0f },
};

static Vec lbl_1_data_18A4[2] = {
    { 1344.59998f, 335.0f, 707.0f },
    { 1287.09998f, 335.0f, 806.599976f },
};

static Vec lbl_1_data_18BC[3] = {
    { 1314.59998f, 335.0f, 759.0f },
    { 1344.59998f, 220.0f, 707.0f },
    { 1287.09998f, 220.0f, 806.599976f },
};

static Vec lbl_1_data_18E0[4] = {
    { 1344.59998f, 335.0f, 707.0f },
    { 1287.09998f, 335.0f, 806.599976f },
    { 1344.59998f, 220.0f, 707.0f },
    { 1287.09998f, 220.0f, 806.599976f },
};

static Vec lbl_1_data_1910[5] = {
    { 1344.59998f, 335.0f, 707.0f },
    { 1287.09998f, 335.0f, 806.599976f },
    { 1372.09998f, 220.0f, 659.400024f },
    { 1314.59998f, 220.0f, 759.0f },
    { 1257.09998f, 220.0f, 858.5f },
};

static Vec lbl_1_data_194C[6] = {
    { 1372.09998f, 335.0f, 659.400024f },
    { 1314.59998f, 335.0f, 759.0f },
    { 1257.09998f, 335.0f, 858.5f },
    { 1372.09998f, 220.0f, 659.400024f },
    { 1314.59998f, 220.0f, 759.0f },
    { 1257.09998f, 220.0f, 858.5f },
};

static Vec lbl_1_data_1994[7] = {
    { 1372.09998f, 335.0f, 659.400024f },
    { 1314.59998f, 335.0f, 759.0f },
    { 1257.09998f, 335.0f, 858.5f },
    { 1402.09998f, 220.0f, 607.400024f },
    { 1344.59998f, 220.0f, 707.0f },
    { 1287.09998f, 220.0f, 806.599976f },
    { 1229.59998f, 220.0f, 906.200012f },
};

static Vec lbl_1_data_19E8[8] = {
    { 1402.09998f, 335.0f, 607.400024f },
    { 1344.59998f, 335.0f, 707.0f },
    { 1287.09998f, 335.0f, 806.599976f },
    { 1229.59998f, 335.0f, 906.200012f },
    { 1402.09998f, 220.0f, 607.400024f },
    { 1344.59998f, 220.0f, 707.0f },
    { 1287.09998f, 220.0f, 806.599976f },
    { 1229.59998f, 220.0f, 906.200012f },
};

static Vec lbl_1_data_1A48[9] = {
    { 1402.09998f, 335.0f, 607.400024f },
    { 1344.59998f, 335.0f, 707.0f },
    { 1287.09998f, 335.0f, 806.599976f },
    { 1229.59998f, 335.0f, 906.200012f },
    { 1429.59998f, 220.0f, 559.400024f },
    { 1372.09998f, 220.0f, 659.400024f },
    { 1314.59998f, 220.0f, 759.0f },
    { 1257.09998f, 220.0f, 858.5f },
    { 1199.59998f, 220.0f, 958.099976f },
};

static Vec lbl_1_data_1AB4[10] = {
    { 1429.59998f, 335.0f, 559.799988f },
    { 1372.09998f, 335.0f, 659.400024f },
    { 1314.59998f, 335.0f, 759.0f },
    { 1257.09998f, 335.0f, 858.5f },
    { 1199.59998f, 335.0f, 958.099976f },
    { 1429.59998f, 220.0f, 559.400024f },
    { 1372.09998f, 220.0f, 659.400024f },
    { 1314.59998f, 220.0f, 759.0f },
    { 1257.09998f, 220.0f, 858.5f },
    { 1199.59998f, 220.0f, 958.099976f },
};


static Vec *lbl_1_data_1B2C[10] = {
    lbl_1_data_1898, lbl_1_data_18A4, lbl_1_data_18BC, lbl_1_data_18E0, lbl_1_data_1910,
    lbl_1_data_194C, lbl_1_data_1994, lbl_1_data_19E8, lbl_1_data_1A48, lbl_1_data_1AB4,
};

void fn_1_19324(void)
{
    lbl_1_data_1860.center.x = 1500.0 * -sin(-2.0943951023931953);
    lbl_1_data_1860.center.y = 200.0f;
    lbl_1_data_1860.center.z = 1500.0 * -cos(-2.0943951023931953);
}

/* Entry from the room menu: show the photo wall. */
// NONMATCHING: restructured from the decompiled flow; same behaviour, different codegen
void fn_1_193C0(void)
{
    Vec pos;
    s16 i;
    f64 t;

    fn_1_19928();
    fn_1_1A17C();
    pos.x = 1300.0 * -sin(-2.0943951023931953);
    pos.y = 70.0f;
    pos.z = 1300.0 * -cos(-2.0943951023931953);
    pos.x = pos.x + (190.0 * -sin(-0.5235987755982988));
    pos.z = pos.z + (190.0 * -cos(-0.5235987755982988));
    Hu3DModelRotSet(lbl_1_bss_58A, 0.0f, -110.0f, 0.0f);
    Hu3DModelScaleSet(lbl_1_bss_58A, 1.2f, 1.2f, 1.2f);
    Hu3DModelAttrReset(lbl_1_bss_58C[6], 0x40000002);
    Hu3DMotionTimeSet(lbl_1_bss_58C[6], 0.0f);
    HuAudFXPlay(0x50F);
    omCameraViewMoveMulti(4, &lbl_1_data_1860, 0x3C, 0);
    HuPrcSleep(0x14);
    HuAudFXPlay(0x510);
    HuPrcSleep(0x28);
    Hu3DModelAttrSet(lbl_1_bss_58C[6], 0x40000001);
    Hu3DMotionStartEndSet(lbl_1_bss_58C[6], 61.0f, 830.0f);
    for (i = 1; i <= 20; i++) {
        t = 500.0 * (1.0 - HuSin(90.0 * ((f64)i / 20.0)));
        pos.y = 70.0 + t;
        Hu3DModelPosSetV(lbl_1_bss_58A, &pos);
        HuPrcVSleep();
    }
    HuWinExOpen(lbl_1_bss_AD2);
    HuWinMesSet(lbl_1_bss_AD2, 0x19000C);
    HuAudFXPlay(0x2EF);
    HuWinMesWait(lbl_1_bss_AD2);
    if (lbl_1_bss_AA0 != 0) {
        fn_1_1A958();
    } else {
        fn_1_1B640();
    }
    HuWinExClose(lbl_1_bss_AD2);
    for (i = 1; i <= 20; i++) {
        t = 500.0 * (1.0 - HuCos(90.0 * ((f64)i / 20.0)));
        pos.y = 90.0 + t;
        Hu3DModelPosSetV(lbl_1_bss_58A, &pos);
        HuPrcVSleep();
    }
    HuAudFXPlay(0x510);
    Hu3DModelAttrReset(lbl_1_bss_58C[6], 0x40000001);
    Hu3DMotionStartEndSet(lbl_1_bss_58C[6], 61.0f, 890.0f);
    Hu3DMotionTimeSet(lbl_1_bss_58C[6], 831.0f);
    HuPrcSleep(0x28);
    HuAudFXPlay(0x50F);
    while (Hu3DMotionEndCheck(lbl_1_bss_58C[6]) == 0) {
        HuPrcVSleep();
    }
    Hu3DModelPosSet(lbl_1_bss_58A, 0.0f, 195.0f, -1155.0f);
    Hu3DModelRotSet(lbl_1_bss_58A, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(lbl_1_bss_58A, 1.8f, 1.8f, 1.8f);
    fn_1_19FC0();
    fn_1_1C13C();
    HuDataDirClose(0xBC0000);
}

/* Creates the photo models, capture textures, windows and cursor. */
// NONMATCHING: restructured from the decompiled flow; same behaviour, different codegen
static void fn_1_19928(void)
{
    s16 i;
    s16 mdl;
    s16 win;
    PICSLOT *slot;

    lbl_1_bss_AA0 = 0;
    for (i = 0; i < 10; i++) {
        if (GwSdCommon.picture[i].number != 0xFFFF) {
            lbl_1_bss_AA0++;
        }
    }
    Hu3DModelAttrSet(lbl_1_bss_58C[1], 1);
    Hu3DModelAttrSet(lbl_1_bss_58C[5], 1);
    Hu3DModelAttrSet(lbl_1_bss_58C[3], 1);
    Hu3DModelAttrSet(lbl_1_bss_58C[4], 1);
    Hu3DModelAttrSet(lbl_1_bss_58C[2], 1);
    fn_1_7320();
    fn_1_16064();
    lbl_1_bss_AD2 = HuWinExCreateFrame(156.0f, 344.0f, 0x190, 0x5E, -1, 1);
    winData[lbl_1_bss_AD2].padMask = 1;
    HuWinMesPalSet(lbl_1_bss_AD2, 7, 0, 0, 0);
    HuWinMesSpeedSet(lbl_1_bss_AD2, 0);
    HuWinPriSet(lbl_1_bss_AD2, 0x32);
    for (i = 0; i < 3; i++) {
        win = HuWinCreate(-10000.0f, (f32)(i * 0x1A + 0xA), 0x240, 0x2A, 0);
        lbl_1_bss_BF8[i] = win;
        HuWinMesSpeedSet(win, 0);
        HuWinPriSet(win, 0x28);
        HuWinDrawNoSet(win, 0x60);
        HuWinAttrSet(win, 0x800);
        HuWinBGTPLvlSet(win, 0.0f);
    }
    if (lbl_1_bss_AA0 != 0) {
        for (i = 0; i < lbl_1_bss_AA0; i++) {
            slot = &lbl_1_bss_AD4[i];
            mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0xBE0005, HU_MEMNUM_OVL, HEAP_MODEL));
            slot->mdl = mdl;
            Hu3DModelCameraSet(mdl, 4);
            Hu3DModelPosSetV(mdl, &lbl_1_data_1B2C[lbl_1_bss_AA0 - 1][i]);
            Hu3DModelRotSet(mdl, 0.0f, -120.0f, 0.0f);
            slot->buf = HuMemDirectMallocNum(HEAP_MODEL, GXGetTexBufferSize(0x80, 0x80, GX_TF_RGB5A3, 0, 0), HU_MEMNUM_OVL);
            slot->animData = HuSprAnimMake(0x80, 0x80, 2);
            slot->animData->bmp->data = slot->buf;
            slot->anim = Hu3DAnimCreate(slot->animData, mdl, "picture");
        }
        slot = &lbl_1_bss_AD4[10];
        mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0xBE0005, HU_MEMNUM_OVL, HEAP_MODEL));
        slot->mdl = mdl;
        Hu3DModelCameraSet(mdl, 4);
        Hu3DModelAttrSet(mdl, 1);
        Hu3DModelPosSetV(mdl, &lbl_1_data_1B2C[lbl_1_bss_AA0 - 1][0]);
        Hu3DModelRotSet(mdl, 0.0f, -120.0f, 0.0f);
        slot->buf = HuMemDirectMallocNum(HEAP_MODEL, GXGetTexBufferSize(0x1E0, 0x1E0, GX_TF_RGB5A3, 0, 0), HU_MEMNUM_OVL);
        slot->animData = HuSprAnimMake(0x1E0, 0x1E0, 2);
        slot->animData->bmp->data = slot->buf;
        slot->anim = Hu3DAnimCreate(slot->animData, mdl, "picture");
    }
    fn_1_480C(&lbl_1_bss_AA4);
    lbl_1_bss_BF0 = HuSprGrpCreate(1);
    HuSprGrpMemberSet(lbl_1_bss_BF0, 0, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0xE70001, HU_MEMNUM_OVL, HEAP_MODEL)), 50, 0));
    HuSprPosSet(lbl_1_bss_BF0, 0, 288.0f, -1000.0f);
    Hu3DCameraPerspectiveSet(8, 5.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(8, -3.5555496f, 72.0f, 256.0f, 192.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(8, 0x16, 0x4C, 0xCC, 0xB8);
    omCameraViewSetMulti(8, &lbl_1_data_187C);
    HuSprExecLayerCameraSet(0x60, 1, 4);
}

static void fn_1_19FC0(void)
{
    s16 i;

    if (lbl_1_bss_AA0 != 0) {
        for (i = 0; i < lbl_1_bss_AA0; i++) {
            Hu3DModelKill(lbl_1_bss_AD4[i].mdl);
            Hu3DAnimKill(lbl_1_bss_AD4[i].anim);
            HuMemDirectFree(lbl_1_bss_AD4[i].buf);
        }
        Hu3DModelKill(lbl_1_bss_AD4[10].mdl);
        Hu3DAnimKill(lbl_1_bss_AD4[10].anim);
        HuMemDirectFree(lbl_1_bss_AD4[10].buf);
    }
    Hu3DModelAttrReset(lbl_1_bss_58C[1], 1);
    Hu3DModelAttrReset(lbl_1_bss_58C[5], 1);
    Hu3DModelAttrReset(lbl_1_bss_58C[3], 1);
    Hu3DModelAttrReset(lbl_1_bss_58C[4], 1);
    Hu3DModelAttrReset(lbl_1_bss_58C[2], 1);
    fn_1_7298();
    fn_1_16024();
    HuWinKill(lbl_1_bss_AD2);
    HuSprGrpKill(lbl_1_bss_BF0);
    for (i = 0; i < 3; i++) {
        HuWinKill(lbl_1_bss_BF8[i]);
    }
    fn_1_4724(&lbl_1_bss_AA4);
    Hu3DLayerHookReset(0xC);
    Hu3DLayerHookReset(4);
}

/* Renders every stored photo into its small capture texture. */
// NONMATCHING: restructured from the decompiled flow; same behaviour, different codegen
static void fn_1_1A17C(void)
{
    GWSDPICTURE *pic;
    SDMACHINE_DISP disp;

    if (lbl_1_bss_AA0 == 0) {
        return;
    }
    lbl_1_bss_A90 = 0.0f;
    Hu3DLayerHookSet(0xC, fn_1_1A850);
    Hu3DCameraPerspectiveSet(1, 5.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 128.0f, 128.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(1, 0, 0, 0x80, 0x80);
    omCameraViewSetMulti(1, &lbl_1_data_187C);
    for (lbl_1_bss_A9E = 0; lbl_1_bss_A9E < lbl_1_bss_AA0; lbl_1_bss_A9E++) {
        pic = &GwSdCommon.picture[lbl_1_bss_A9E];
        sprintf(lbl_1_bss_A94, "%d", pic->number);
        HuWinInsertMesSet(lbl_1_bss_BF8[0], MESSNUM_PTR(lbl_1_bss_A94), 0);
        HuWinMesSet(lbl_1_bss_BF8[0], 0x190033);
        HuWinPosSet(lbl_1_bss_BF8[0], 0.0f, 0.0f);
        HuWinScaleSet(lbl_1_bss_BF8[0], 0.2f, 0.26666668f);
        HuWinInsertMesSet(lbl_1_bss_BF8[1], pic->type + 0x190035, 1);
        HuWinInsertMesSet(lbl_1_bss_BF8[1], pic->diff + 0x190038, 2);
        HuWinMesSet(lbl_1_bss_BF8[1], 0x190034);
        HuWinPosSet(lbl_1_bss_BF8[1], 0.0f, 6.933334f);
        HuWinScaleSet(lbl_1_bss_BF8[1], 0.2f, 0.26666668f);
        HuWinMesSet(lbl_1_bss_BF8[2], MESSNUM_PTR(pic->machine.name));
        HuWinPosSet(lbl_1_bss_BF8[2], 0.0f, 118.66667f);
        HuWinScaleSet(lbl_1_bss_BF8[2], 0.2f, 0.26666668f);
        fn_1_42E0(&disp, pic->machine.partNo[0], pic->machine.partNo[1], pic->machine.partNo[3], pic->machine.charNo);
        fn_1_4638(&disp, 1);
        fn_1_4590(&disp, 0.0f, 60.0f, 0.0f);
        HuPrcSleep(2);
        fn_1_4724(&disp);
    }
    lbl_1_bss_A9E = -1;
}

/* Sets up the big capture of photo `no`. */
// NONMATCHING: restructured from the decompiled flow; same behaviour, different codegen
static void fn_1_1A528(s16 no)
{
    GWSDPICTURE *pic;

    Hu3DCameraPerspectiveSet(1, 5.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 480.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(1, 0, 0, 0x1E0, 0x1E0);
    omCameraViewSetMulti(1, &lbl_1_data_187C);
    pic = &GwSdCommon.picture[no];
    sprintf(lbl_1_bss_A94, "%d", pic->number);
    HuWinInsertMesSet(lbl_1_bss_BF8[0], MESSNUM_PTR(lbl_1_bss_A94), 0);
    HuWinMesSet(lbl_1_bss_BF8[0], 0x190033);
    HuWinPosSet(lbl_1_bss_BF8[0], 0.0f, 0.0f);
    HuWinScaleSet(lbl_1_bss_BF8[0], 0.75f, 1.0f);
    HuWinInsertMesSet(lbl_1_bss_BF8[1], pic->type + 0x190035, 1);
    HuWinInsertMesSet(lbl_1_bss_BF8[1], pic->diff + 0x190038, 2);
    HuWinMesSet(lbl_1_bss_BF8[1], 0x190034);
    HuWinPosSet(lbl_1_bss_BF8[1], 0.0f, 26.0f);
    HuWinScaleSet(lbl_1_bss_BF8[1], 0.75f, 1.0f);
    HuWinMesSet(lbl_1_bss_BF8[2], MESSNUM_PTR(pic->machine.name));
    HuWinPosSet(lbl_1_bss_BF8[2], 0.0f, 445.0f);
    HuWinScaleSet(lbl_1_bss_BF8[2], 0.75f, 1.0f);
    fn_1_4724(&lbl_1_bss_AA4);
    fn_1_42E0(&lbl_1_bss_AA4, pic->machine.partNo[0], pic->machine.partNo[1], pic->machine.partNo[3], pic->machine.charNo);
    fn_1_4638(&lbl_1_bss_AA4, 1);
    fn_1_4590(&lbl_1_bss_AA4, 0.0f, 60.0f, 0.0f);
    lbl_1_bss_A9E = 0x12;
}

/* Layer hook: copies the frame buffer into the current capture texture. */
static void fn_1_1A850(s16 layerNo)
{
    if (Hu3DCameraNo == 0 && lbl_1_bss_A9E >= 0) {
        if (lbl_1_bss_A9E >= 0 && lbl_1_bss_A9E < 10) {
            Hu3DFbCopyExec(0, 0, 0x80, 0x80, GX_TF_RGB5A3, 0, lbl_1_bss_AD4[lbl_1_bss_A9E].buf);
        } else {
            if (0.0 != lbl_1_bss_A90) {
                fn_1_1BDF8(lbl_1_bss_A90);
            }
            Hu3DFbCopyExec(0, 0, 0x1E0, 0x1E0, GX_TF_RGB5A3, 0, lbl_1_bss_AD4[10].buf);
        }
        Hu3DZClear();
    }
}

/* Photo wall browser: move the cursor over the photos, A to inspect, B to leave. */
// NONMATCHING: restructured from the decompiled flow; same behaviour, different codegen
static void fn_1_1A958(void)
{
    OMCAMERAVIEW view = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 240.0f, 0.0f }, 160.0f };
    HuVecF scr;
    Vec *tbl;
    f32 dz;
    f32 s;
    s16 cur;
    s16 i;
    s16 moveX;
    s16 moveY;

    Hu3D3Dto2D(&lbl_1_data_1B2C[lbl_1_bss_AA0 - 1][0], 4, &scr);
    HuSprPosSet(lbl_1_bss_BF0, 0, scr.x - 20.0f, scr.y - 15.0f);
    for (i = 1; i <= 10; i++) {
        s = (f64)i / 10.0;
        HuSprScaleSet(lbl_1_bss_BF0, 0, s, s);
        HuPrcVSleep();
    }
    lbl_1_bss_AD0 = HuWinCreate(-10000.0f, 275.0f, 0x220, 0x1A, 0);
    HuWinMesSpeedSet(lbl_1_bss_AD0, 0);
    HuWinPriSet(lbl_1_bss_AD0, 0x28);
    HuWinAttrSet(lbl_1_bss_AD0, 0x800);
    HuWinBGTPLvlSet(lbl_1_bss_AD0, 0.0f);
    HuWinMesSet(lbl_1_bss_AD0, 0x20004);
    cur = 0;
    HuWinMesSet(lbl_1_bss_AD2, 0x19005E);
    while (1) {
        tbl = lbl_1_data_1B2C[lbl_1_bss_AA0 - 1];
        moveY = 0;
        moveX = 0;
        if (HuPadDStkRep[0] & 1) {
            for (i = lbl_1_bss_AA0 - 1; i >= 0; i--) {
                if (tbl[cur].z > tbl[i].z && tbl[cur].y == tbl[i].y) {
                    break;
                }
            }
            if (i >= 0) {
                cur = i;
                moveX = -1;
            }
        } else if (HuPadDStkRep[0] & 2) {
            for (i = 0; i < lbl_1_bss_AA0; i++) {
                if (tbl[cur].z < tbl[i].z && tbl[cur].y == tbl[i].y) {
                    break;
                }
            }
            if (i != lbl_1_bss_AA0) {
                cur = i;
                moveX = 1;
            }
        }
        if (HuPadDStkRep[0] & 8) {
            for (i = 0; i < lbl_1_bss_AA0; i++) {
                dz = tbl[cur].z - tbl[i].z;
                if (dz * dz < 8100.0f && tbl[cur].y < tbl[i].y) {
                    break;
                }
            }
            if (i != lbl_1_bss_AA0) {
                cur = i;
                moveY = -1;
            }
        } else if (HuPadDStkRep[0] & 4) {
            for (i = 0; i < lbl_1_bss_AA0; i++) {
                dz = tbl[cur].z - tbl[i].z;
                if (dz * dz < 8100.0f && tbl[cur].y > tbl[i].y) {
                    break;
                }
            }
            if (i != lbl_1_bss_AA0) {
                cur = i;
                moveY = 1;
            }
        }
        if (moveX != 0 || moveY != 0) {
            HuAudFXPlay(0);
            Hu3D3Dto2D(&tbl[cur], 4, &scr);
            HuSprPosSet(lbl_1_bss_BF0, 0, scr.x - 20.0f, scr.y - 15.0f);
        }
        if (HuPadBtnDown[0] & 0x100) {
            if (GwSdCommon.picture[cur].number != 0xFFFF) {
                HuAudFXPlay(1);
                HuWinDispOff(lbl_1_bss_AD0);
                HuWinExClose(lbl_1_bss_AD2);
                fn_1_1A528(cur);
                view.center = tbl[cur];
                omCameraViewMoveMulti(4, &view, 0x1E, 0);
                for (i = 1; i <= 10; i++) {
                    s = 1.0 - ((f64)i / 10.0);
                    HuSprScaleSet(lbl_1_bss_BF0, 0, s, s);
                    HuPrcVSleep();
                }
                Hu3DModelAttrSet(lbl_1_bss_AD4[cur].mdl, 1);
                Hu3DModelAttrReset(lbl_1_bss_AD4[10].mdl, 1);
                Hu3DModelPosSetV(lbl_1_bss_AD4[10].mdl, &tbl[cur]);
                HuPrcSleep(0x14);
                HuWinPosSet(lbl_1_bss_AD0, -10000.0f, 406.0f);
                HuWinMesSet(lbl_1_bss_AD0, 0x2001A);
                HuWinDispOn(lbl_1_bss_AD0);
                fn_1_1B72C(cur);
                HuWinMesSet(lbl_1_bss_AD0, 0x20004);
                HuWinDispOff(lbl_1_bss_AD0);
                omCameraViewMoveMulti(4, &lbl_1_data_1860, 0x1E, 0);
                for (i = 1; i <= 10; i++) {
                    s = (f64)i / 10.0;
                    HuSprScaleSet(lbl_1_bss_BF0, 0, s, s);
                    HuPrcVSleep();
                }
                Hu3DModelAttrReset(lbl_1_bss_AD4[cur].mdl, 1);
                Hu3DModelAttrSet(lbl_1_bss_AD4[10].mdl, 1);
                HuPrcSleep(0x14);
                HuWinPosSet(lbl_1_bss_AD0, -10000.0f, 275.0f);
                HuWinDispOn(lbl_1_bss_AD0);
                lbl_1_bss_A90 = 0.0f;
                HuWinExOpen(lbl_1_bss_AD2);
                HuWinMesSet(lbl_1_bss_AD2, 0x19005E);
            } else {
                HuAudFXPlay(4);
            }
        }
        if (HuPadBtnDown[0] & 0x200) {
            break;
        }
        HuPrcVSleep();
    }
    HuAudFXPlay(3);
    HuWinKill(lbl_1_bss_AD0);
    for (i = 1; i <= 10; i++) {
        s = 1.0 - ((f64)i / 10.0);
        HuSprScaleSet(lbl_1_bss_BF0, 0, s, s);
        HuPrcVSleep();
    }
}

/* No photos stored yet. */
// NONMATCHING: restructured from the decompiled flow; same behaviour, different codegen
static void fn_1_1B640(void)
{
    lbl_1_bss_AD0 = HuWinCreate(-10000.0f, 275.0f, 0x220, 0x1A, 0);
    HuWinMesSpeedSet(lbl_1_bss_AD0, 0);
    HuWinPriSet(lbl_1_bss_AD0, 0x28);
    HuWinAttrSet(lbl_1_bss_AD0, 0x800);
    HuWinBGTPLvlSet(lbl_1_bss_AD0, 0.0f);
    HuWinMesSet(lbl_1_bss_AD0, 0x2000B);
    HuWinMesSet(lbl_1_bss_AD2, 0x19005F);
    while ((HuPadBtnDown[0] & 0x200) == 0) {
        HuPrcVSleep();
    }
    HuWinKill(lbl_1_bss_AD0);
}

/* Photo viewer: orbit/zoom the camera around the shown photo, X to delete it. */
// NONMATCHING: restructured from the decompiled flow; same behaviour, different codegen
static void fn_1_1B72C(s16 no)
{
    HuVec2f size;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    f32 zoom;
    f32 t;
    f32 nx;
    f32 ny;
    s16 win;
    s16 i;

    rotX = lbl_1_data_187C.rot.x;
    rotY = lbl_1_data_187C.rot.y;
    rotZ = lbl_1_data_187C.rot.z;
    zoom = lbl_1_data_187C.zoom;
    while (1) {
        if (HuPadBtnDown[0] & 0x200) {
            HuAudFXPlay(3);
            if (rotX != lbl_1_data_187C.rot.x || rotY != lbl_1_data_187C.rot.y) {
                if (rotY > 180.0f) {
                    rotY -= 360.0f;
                }
                for (i = 1; i <= 10; i++) {
                    t = (f64)i / 10.0;
                    nx = rotX + (t * (lbl_1_data_187C.rot.x - rotX));
                    ny = rotY + (t * (lbl_1_data_187C.rot.y - rotY));
                    CRotM[0].x = nx;
                    CRotM[0].y = ny;
                    CRotM[0].z = rotZ;
                    CZoomM[0] = zoom + (t * (lbl_1_data_187C.zoom - zoom));
                    HuPrcVSleep();
                }
            }
            return;
        }
        if (HuPadBtnDown[0] & 0x400) {
            HuWinMesMaxSizeGet(1, &size, 0x190066);
            win = HuWinExCreate(-10000.0f, 200.0f, (s32)size.x, (s32)size.y, -1);
            winData[win].padMask = 1;
            HuWinPriSet(win, 0xA);
            HuWinExOpen(win);
            HuWinMesSet(win, 0x190066);
            HuWinMesWait(win);
            if (HuWinChoiceGet(win, -1) == 0) {
                HuWinExClose(win);
                HuWinKill(win);
                HuAudFXPlay(6);
                fn_1_1BCD4(no);
                return;
            }
            HuWinExClose(win);
            HuWinKill(win);
        }
        if (HuPadStkX[0] & 0xF0) {
            rotY = rotY + (0.1 * HuPadStkX[0]);
            if (rotY > 360.0f) {
                rotY -= 360.0f;
            }
            if (rotY < 0.0f) {
                rotY += 360.0f;
            }
        }
        if (HuPadStkY[0] & 0xF0) {
            rotX = rotX + (0.1 * HuPadStkY[0]);
            if (rotX > 360.0f) {
                rotX = 360.0f;
            }
            if (rotX < 270.0f) {
                rotX = 270.0f;
            }
        }
        zoom = (zoom + HuPadTrigL[0]) - HuPadTrigR[0];
        if (zoom < lbl_1_data_187C.zoom - 2000.0f) {
            zoom = lbl_1_data_187C.zoom - 2000.0f;
        }
        if (zoom > 4000.0f + lbl_1_data_187C.zoom) {
            zoom = 4000.0f + lbl_1_data_187C.zoom;
        }
        CRotM[0].x = rotX;
        CRotM[0].y = rotY;
        CRotM[0].z = rotZ;
        CZoomM[0] = zoom;
        HuPrcVSleep();
    }
}

/* Deletes photo `no`: fades, blanks its capture texture and marks the slot empty. */
// NONMATCHING: restructured from the decompiled flow; same behaviour, different codegen
static void fn_1_1BCD4(s16 no)
{
    u16 *p;
    s32 j;
    s16 i;

    for (i = 1; i <= 30; i++) {
        lbl_1_bss_A90 = (f64)i / 30.0;
        HuPrcVSleep();
    }
    p = lbl_1_bss_AD4[no].buf;
    for (j = 0; j < 0x4000; j++) {
        *p++ = 0x8000;
    }
    DCFlushRange(lbl_1_bss_AD4[no].buf, 0x8000);
    GwSdCommon.picture[no].number = 0xFFFF;
}

/* Full-screen black quad with alpha (the delete fade). */
static void fn_1_1BDF8(f32 alpha)
{
    Mtx44 proj;
    Mtx mv;
    GXColor color = { 0, 0, 0, 0 };

    color.a = 255.0f * alpha;
    MTXOrtho(proj, 0.0f, 480.0f, 0.0f, 640.0f, 0.0f, 10.0f);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);
    MTXIdentity(mv);
    GXLoadPosMtxImm(mv, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    GXSetScissor(0, 0, 0x280, 0x1E0);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_SPEC);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_SPEC);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U16, 0);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColor(GX_COLOR1, color);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2u16(0, 0);
    GXPosition2u16(640, 0);
    GXPosition2u16(640, 480);
    GXPosition2u16(0, 480);
    GXEnd();
}

/* Drops photo slots whose number is 0xFFFF (deleted) and packs the list. */
// NONMATCHING: restructured from the decompiled flow; same behaviour, different codegen
void fn_1_1C13C(void)
{
    s16 i;
    s16 j;

    for (i = 8; i >= 0; i--) {
        if (GwSdCommon.picture[i].number == 0xFFFF) {
            for (j = i; j < 9; j++) {
                GwSdCommon.picture[j] = GwSdCommon.picture[j + 1];
            }
            GwSdCommon.picture[9].number = 0xFFFF;
        }
    }
}
