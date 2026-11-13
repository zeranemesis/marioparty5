#include "string.h"
#include "game/data.h"
#include "game/hu3d.h"

#define MDL(i) (lbl_1_bss_3030->mdl[i])
#define DATA(i) (lbl_1_bss_3030->dataList[i])
#define MDL_SETPOS(i, vx, vy, vz, rx, ry, rz) \
    do {                                   \
        MDL(i).pos.x = (vx);                \
        MDL(i).pos.y = (vy);                \
        MDL(i).pos.z = (vz);                \
        MDL(i).rot.x = (rx);                \
        MDL(i).rot.y = (ry);                \
        MDL(i).rot.z = (rz);                \
    } while (0)
#define MDL_ZERO(i) MDL_SETPOS(i, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)
/* first model of a group is created from data, the rest link to it */
#define MDL_NEW(i, first, d)                                        \
    if ((i) == (first)) {                                           \
        MDL(i).mdlId = Hu3DModelCreateData(DATA(d));                \
    } else {                                                        \
        MDL(i).mdlId = Hu3DModelLink(MDL(first).mdlId);             \
    }                                                               \
    Hu3DModelAttrSet(MDL(i).mdlId, 1);                              \
    Hu3DModelCameraSet(MDL(i).mdlId, 1)
#define MDL_MOTION(i, spd)                  \
    Hu3DMotionTimeSet(MDL(i).mdlId, 0.0f);  \
    Hu3DMotionSpeedSet(MDL(i).mdlId, (spd))

/* attaches the item to the rod (lbl_1_data_2D0) */
static char m555_hookName[] = "itemhook_sao";

/* Allocates every model the minigame draws (indexes into lbl_1_bss_3030->mdl)
   and resets the shared work areas. */
void fn_1_3010(M555_CTRL *ctrl)
{
    s16 mot;
    s16 ids[10];
    f32 posX[8];
    s16 i;
    s16 j;
    s16 k;
    M555_CTRLWIN *win;

    memset(&lbl_1_bss_1894, 0, sizeof(M555_WORK));
    memset(lbl_1_bss_120C, 0, sizeof(lbl_1_bss_120C));
    memset(&lbl_1_bss_11E4, 0, sizeof(M555_SPIN));
    memset(&lbl_1_bss_11C8, 0, sizeof(M555_HOOK));
    memset(lbl_1_bss_B88, 0, sizeof(lbl_1_bss_B88));
    memset(&lbl_1_bss_B74, 0, sizeof(lbl_1_bss_B74));
    memset(lbl_1_bss_214, 0, sizeof(lbl_1_bss_214));
    memset(&lbl_1_bss_C, 0, sizeof(lbl_1_bss_C));
    lbl_1_bss_3030 = ctrl;
    for (i = 0; i < 12; i++) {
        lbl_1_bss_1894.hookIdx[i] = -1;
    }
    lbl_1_bss_120C[0].hookX = 1;
    lbl_1_bss_120C[1].hookX = 1;
    lbl_1_bss_120C[0].stage = 5;
    lbl_1_bss_120C[1].stage = 5;

    for (i = 0, j = 0; i <= 1; i++, j++) {
        MDL(i).mdlId = Hu3DModelCreateData(DATA(j));
        Hu3DModelAttrSet(MDL(i).mdlId, 1);
        Hu3DModelCameraSet(MDL(i).mdlId, 1);
        Hu3DModelLayerSet(MDL(i).mdlId, 0);
        MDL_ZERO(i);
        Hu3DModelAttrReset(MDL(i).mdlId, 1);
        if (i == 1) {
            Hu3DModelAttrSet(MDL(i).mdlId, HU3D_MOTATTR_LOOP);
            MDL_MOTION(i, 1.0f);
        }
    }

    for (i = 2; i <= 5; i++) {
        MDL_NEW(i, 2, 2);
        Hu3DModelLayerSet(MDL(i).mdlId, 7);
        MDL_ZERO(i);
        MDL_MOTION(i, 0.0f);
    }
    for (i = 6; i <= 9; i++) {
        MDL_NEW(i, 6, 3);
        Hu3DModelLayerSet(MDL(i).mdlId, 7);
        MDL_ZERO(i);
        MDL_MOTION(i, 0.0f);
    }
    for (i = 10; i <= 13; i++) {
        MDL_NEW(i, 10, 4);
        Hu3DModelLayerSet(MDL(i).mdlId, 7);
        MDL_ZERO(i);
        MDL_MOTION(i, 0.0f);
    }
    for (i = 14; i <= 17; i++) {
        MDL_NEW(i, 14, 6);
        Hu3DModelLayerSet(MDL(i).mdlId, 7);
        MDL_ZERO(i);
        MDL_MOTION(i, 0.0f);
    }
    for (i = 18; i <= 21; i++) {
        MDL_NEW(i, 18, 8);
        Hu3DModelLayerSet(MDL(i).mdlId, 7);
        MDL_ZERO(i);
        MDL_MOTION(i, 0.0f);
    }
    for (i = 22; i <= 25; i++) {
        MDL_NEW(i, 22, 9);
        Hu3DModelLayerSet(MDL(i).mdlId, 7);
        MDL_ZERO(i);
        MDL_MOTION(i, 0.0f);
    }

    MDL(28).mdlId = Hu3DModelCreateData(DATA(10));
    Hu3DModelAttrSet(MDL(28).mdlId, 1);
    Hu3DModelCameraSet(MDL(28).mdlId, 1);
    Hu3DModelLayerSet(MDL(28).mdlId, 2);
    MDL_ZERO(28);
    MDL_MOTION(28, 0.0f);

    for (i = 29; i <= 32; i++) {
        MDL_NEW(i, 29, 11);
        Hu3DModelLayerSet(MDL(i).mdlId, 7);
        MDL_ZERO(i);
        MDL_MOTION(i, 0.0f);
    }

    for (i = 96; i <= 97; i++) {
        MDL_NEW(i, 96, 46);
        Hu3DModelLayerSet(MDL(i).mdlId, 0);
        MDL_ZERO(i);
        mot = Hu3DJointMotionData(MDL(i).mdlId, 0x710031);
        Hu3DMotionSet(MDL(i).mdlId, mot);
        Hu3DModelAttrSet(MDL(i).mdlId, HU3D_MOTATTR_LOOP);
        MDL_MOTION(i, 0.0f);
    }
    for (i = 98; i <= 99; i++) {
        MDL_NEW(i, 98, 47);
        Hu3DModelLayerSet(MDL(i).mdlId, 0);
        MDL_ZERO(i);
        mot = Hu3DJointMotionData(MDL(i).mdlId, 0x710032);
        Hu3DMotionSet(MDL(i).mdlId, mot);
        Hu3DModelAttrSet(MDL(i).mdlId, HU3D_MOTATTR_LOOP);
        MDL_MOTION(i, 0.0f);
    }
    lbl_1_bss_1894.posIdx = Hu3DJointMotionData(MDL(98).mdlId, 0x710033);
    lbl_1_bss_1894.posIdx2 = Hu3DJointMotionData(MDL(99).mdlId, 0x710033);

    for (i = 33, j = 12, k = 0; i <= 34; i++, j++, k++) {
        MDL(i).mdlId = Hu3DModelCreateData(DATA(j));
        Hu3DModelAttrSet(MDL(i).mdlId, 1);
        Hu3DModelCameraSet(MDL(i).mdlId, 1);
        Hu3DModelLayerSet(MDL(i).mdlId, 0);
        Hu3DModelAttrReset(MDL(i).mdlId, 1);
        MDL_SETPOS(i, -200.0f + 400.0f * k, -100.0f, 0.0f, 0.0f, 180.0f, 0.0f);
        Hu3DModelAttrSet(MDL(i).mdlId, HU3D_MOTATTR_LOOP);
        MDL_MOTION(i, 0.5f);
    }

    for (i = 35; i <= 84; i++) {
        MDL_NEW(i, 35, 14);
        Hu3DModelLayerSet(MDL(i).mdlId, 2);
        MDL_SETPOS(i, 0.0f, 0.0f, -10000.0f, 0.0f, 0.0f, 0.0f);
    }
    for (i = 85; i <= 87; i++) {
        MDL_NEW(i, 85, 15);
        Hu3DModelLayerSet(MDL(i).mdlId, 2);
        MDL_ZERO(i);
        Hu3DModelAttrSet(MDL(i).mdlId, HU3D_MOTATTR_LOOP);
        MDL_MOTION(i, 1.0f);
    }
    for (i = 88; i <= 92; i++) {
        MDL_NEW(i, 88, 16);
        Hu3DModelLayerSet(MDL(i).mdlId, 2);
        MDL_ZERO(i);
        Hu3DModelAttrReset(MDL(i).mdlId, HU3D_MOTATTR_LOOP);
        MDL_MOTION(i, 0.0f);
    }

    MDL(93).mdlId = Hu3DModelCreateData(DATA(17));
    Hu3DModelAttrSet(MDL(93).mdlId, 1);
    Hu3DModelCameraSet(MDL(93).mdlId, 1);
    Hu3DModelLayerSet(MDL(93).mdlId, 2);
    MDL_ZERO(93);
    Hu3DModelAttrReset(MDL(93).mdlId, HU3D_MOTATTR_LOOP);
    MDL_MOTION(93, 0.0f);

    MDL(94).mdlId = Hu3DModelCreateData(DATA(18));
    Hu3DModelAttrSet(MDL(94).mdlId, 1);
    Hu3DModelCameraSet(MDL(94).mdlId, 1);
    Hu3DModelLayerSet(MDL(94).mdlId, 0);
    MDL_ZERO(94);
    Hu3DModelAttrReset(MDL(94).mdlId, 1);
    MDL_MOTION(94, 0.0f);

    MDL(95).mdlId = Hu3DModelCreateData(DATA(45));
    Hu3DModelAttrSet(MDL(95).mdlId, 1);
    Hu3DModelCameraSet(MDL(95).mdlId, 1);
    Hu3DModelLayerSet(MDL(95).mdlId, 0);
    MDL_SETPOS(95, 0.0f, 30.0f, -200.0f, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(MDL(95).mdlId, 1);
    lbl_1_bss_11E4.pad1[1] = Hu3DJointMotionData(MDL(95).mdlId, 0x710030);
    Hu3DMotionSet(MDL(95).mdlId, lbl_1_bss_11E4.pad1[1]);
    Hu3DModelAttrSet(MDL(95).mdlId, HU3D_MOTATTR_LOOP);
    MDL_MOTION(95, 1.0f);
    Hu3DModelHookSet(MDL(95).mdlId, m555_hookName, MDL(94).mdlId);

    ids[0] = 0x13;
    ids[1] = 0x13;
    ids[2] = 0x14;
    ids[3] = 0x14;
    ids[4] = 0x15;
    ids[5] = 0x15;
    ids[6] = 0x17;
    ids[7] = 0x17;
    ids[8] = 0x18;
    ids[9] = 0x18;
    for (i = 100, j = 0; i <= 109; i++, j++) {
        MDL(i).mdlId = Hu3DModelCreateData(DATA(ids[j]));
        Hu3DModelAttrSet(MDL(i).mdlId, 1);
        Hu3DModelCameraSet(MDL(i).mdlId, 1);
        Hu3DModelLayerSet(MDL(i).mdlId, 3);
        MDL_SETPOS(i, 0.0f, 0.0f, 0.0f, 180.0f, 0.0f, 0.0f);
        Hu3DModelAttrReset(MDL(i).mdlId, HU3D_MOTATTR_LOOP);
        MDL_MOTION(i, 0.0f);
    }

    for (i = 110; i <= 159; i++) {
        MDL_NEW(i, 110, 22);
        Hu3DModelLayerSet(MDL(i).mdlId, 3);
        MDL_ZERO(i);
        Hu3DModelAttrReset(MDL(i).mdlId, HU3D_MOTATTR_LOOP);
        MDL_MOTION(i, 0.0f);
    }

    ids[0] = lbl_1_bss_3030->mdl0[0].charNo + 0x20;
    ids[1] = lbl_1_bss_3030->mdl0[1].charNo + 0x20;
    ids[2] = 0x1D;
    ids[3] = 0x1E;
    ids[4] = 0x1B;
    ids[5] = 0x1C;
    ids[6] = 0x19;
    ids[7] = 0x1A;
    posX[0] = -568.0f;
    posX[1] = 568.0f;
    posX[2] = -520.0f;
    posX[3] = 520.0f;
    posX[4] = -520.0f;
    posX[5] = 520.0f;
    posX[6] = -520.0f;
    posX[7] = 520.0f;
    for (i = 161, j = 0; i <= 168; i++, j++) {
        MDL(i).mdlId = Hu3DModelCreateData(DATA(ids[j]));
        Hu3DModelAttrSet(MDL(i).mdlId, 1);
        Hu3DModelCameraSet(MDL(i).mdlId, 1);
        if (i == 167 || i == 168) {
            Hu3DModelLayerSet(MDL(i).mdlId, 7);
        } else {
            Hu3DModelLayerSet(MDL(i).mdlId, 6);
        }
        MDL_SETPOS(i, posX[j], -364.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrReset(MDL(i).mdlId, 1);
        Hu3DModelAttrReset(MDL(i).mdlId, HU3D_MOTATTR_LOOP);
        Hu3DModelAttrSet(MDL(i).mdlId, HU3D_ATTR_ZCMP_OFF);
        if (i == 165 || i == 166) {
            Hu3DMotionSpeedSet(MDL(i).mdlId, 0.0f);
            Hu3DMotionTimeSet(MDL(i).mdlId, 0.0f);
        }
    }

    MDL(160).mdlId = Hu3DModelCreateData(DATA(31));
    Hu3DModelAttrSet(MDL(160).mdlId, 1);
    Hu3DModelCameraSet(MDL(160).mdlId, 1);
    Hu3DModelLayerSet(MDL(160).mdlId, 6);
    MDL_ZERO(160);
    MDL_MOTION(160, 0.0f);

    win = &lbl_1_bss_3030->win[0];
    win->on = 0;
    win->scale = 1.0f;
    win->value = 0.0f;
    win->alpha = HU3D_MOTATTR_LOOP;
    win->flags = 0;
    win = &lbl_1_bss_3030->win[1];
    win->on = 0;
    win->scale = 1.0f;
    win->value = 0.0f;
    win->alpha = HU3D_MOTATTR_LOOP;
    win->flags = 0;
    fn_1_152D8(0);
    fn_1_152D8(1);
    lbl_1_bss_1894.fogSet = 1;
    lbl_1_bss_1894.fogOn = 0;
    lbl_1_bss_1894.fogStart = 10000.0f;
    lbl_1_bss_1894.fogEnd = 23000.0f;
    lbl_1_bss_1894.fogR = 0xFF;
    lbl_1_bss_1894.fogG = 0xFF;
    lbl_1_bss_1894.fogB = 0xFF;
    fn_1_247C();
    fn_1_7840();
    fn_1_78DC();
}

s32 fn_1_69C4(M555_CTRL *ctrl)
{
    s32 ret = 1;

    fn_1_3010(ctrl);
    lbl_1_bss_1894.motion = 0;
    return ret;
}

s16 fn_1_6A0C(void)
{
    s16 result = 0;

    result = fn_1_130D8();
    fn_1_A758();
    fn_1_9B8C();
    fn_1_CFB8();
    fn_1_78DC();
    lbl_1_bss_1894.motion = 1;
    return result;
}

s16 fn_1_6A60(void)
{
    s16 result = 0;

    fn_1_FB2C();
    lbl_1_bss_1894.phase = 0;
    lbl_1_bss_1894.phase2 = 0;
    lbl_1_bss_1894.subPhase = 0;
    lbl_1_bss_1894.subPhase2 = 0;
    fn_1_A758();
    fn_1_9B8C();
    fn_1_CFB8();
    fn_1_78DC();
    lbl_1_bss_1894.motion = 2;
    return result;
}
