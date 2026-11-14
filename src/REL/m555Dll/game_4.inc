#include "math.h"
#include "game/hu3d.h"

#define IT lbl_1_bss_214[i]
#undef MDL
#define MDL(n) lbl_1_bss_3030->mdl[n]
#define COPY_POS() (MDL(IT.mdlId).pos = MDL(IT.slot + 0x21).pos)
#define COPY_ROT() (MDL(IT.mdlId).rot = MDL(IT.slot + 0x21).rot)
#define SPAWN_PARTNER(kindNo, partnerMdl) \
    for (j = 0; j < 100; j++) { \
        if (lbl_1_bss_214[j].active == 0) { \
            lbl_1_bss_214[j].active = 1; \
            lbl_1_bss_214[j].type = 1; \
            lbl_1_bss_214[j].kind = kindNo; \
            lbl_1_bss_214[j].mdlId = partnerMdl; \
            lbl_1_bss_214[j].slot = 1; \
            break; \
        } \
    }

/* Per-frame driver for the 100-entry item table: each live record plays one
   of several model animations (kinds 0-4, 6, 7) as a small state machine. */
void fn_1_A758(void)
{
    s16 i;
    s16 j;
    s16 cnt;
    f32 time;

    for (i = 0; i < 100; i++) {
        if (IT.active == 0) {
            continue;
        }
        switch (IT.kind) {
        case 0:
            switch (IT.type) {
            case 0:
                IT.type = 1;
                IT.mdlId = 0x64;
                IT.slot = 0;
                SPAWN_PARTNER(0, 0x65);
                if (i > j) {
                    break;
                }
                /* fallthrough */
            case 1:
                COPY_POS();
                COPY_ROT();
                Hu3DModelAttrReset(MDL(IT.mdlId).mdlId, 1);
                Hu3DModelAttrReset(MDL(IT.mdlId).mdlId, HU3D_MOTATTR_LOOP);
                Hu3DMotionTimeSet(MDL(IT.mdlId).mdlId, 0.0f);
                Hu3DMotionSpeedSet(MDL(IT.mdlId).mdlId, 1.0f);
                IT.type++;
                break;
            case 2:
                COPY_POS();
                COPY_ROT();
                time = Hu3DMotionTimeGet(MDL(IT.mdlId).mdlId);
                if (time >= 49.0f) {
                    IT.type++;
                }
                break;
            case 3:
                COPY_POS();
                COPY_ROT();
                time = Hu3DMotionTimeGet(MDL(IT.mdlId).mdlId);
                if (time >= 97.0f) {
                    Hu3DMotionTimeSet(MDL(IT.mdlId).mdlId, 50.0f);
                }
                if (Hu3DModelAttrGet(MDL(IT.slot + 0x21).mdlId) & 1) {
                    Hu3DModelAttrSet(MDL(IT.mdlId).mdlId, 1);
                } else {
                    Hu3DModelAttrReset(MDL(IT.mdlId).mdlId, 1);
                }
                break;
            case 4:
                Hu3DModelAttrSet(MDL(IT.mdlId).mdlId, HU3D_MOTATTR_REV);
                Hu3DMotionTimeSet(MDL(IT.mdlId).mdlId, 50.0f);
                IT.type++;
                /* fallthrough */
            case 5:
                time = Hu3DMotionTimeGet(MDL(IT.mdlId).mdlId);
                if (time <= 2.0f) {
                    Hu3DModelAttrSet(MDL(IT.mdlId).mdlId, 1);
                    IT.type++;
                }
                break;
            }
            break;
        case 1:
            switch (IT.type) {
            case 0:
                IT.type = 1;
                IT.mdlId = 0x66;
                IT.slot = 0;
                SPAWN_PARTNER(1, 0x67);
                if (i > j) {
                    break;
                }
                /* fallthrough */
            case 1:
                COPY_POS();
                COPY_ROT();
                Hu3DModelAttrReset(MDL(IT.mdlId).mdlId, 1);
                Hu3DModelAttrReset(MDL(IT.mdlId).mdlId, HU3D_MOTATTR_LOOP);
                Hu3DMotionTimeSet(MDL(IT.mdlId).mdlId, 0.0f);
                Hu3DMotionSpeedSet(MDL(IT.mdlId).mdlId, 0.7f);
                IT.type++;
                break;
            case 2:
                COPY_POS();
                COPY_ROT();
                time = Hu3DMotionTimeGet(MDL(IT.mdlId).mdlId);
                if (time >= 98.0f) {
                    Hu3DModelAttrSet(MDL(IT.mdlId).mdlId, 1);
                    IT.active = 0;
                    IT.type = 0;
                }
                break;
            }
            break;
        case 2:
            switch (IT.type) {
            case 0:
                IT.type = 1;
                IT.mdlId = 0x68;
                IT.slot = 0;
                SPAWN_PARTNER(2, 0x69);
                if (i > j) {
                    break;
                }
                /* fallthrough */
            case 1:
                COPY_POS();
                COPY_ROT();
                Hu3DModelAttrReset(MDL(IT.mdlId).mdlId, 1);
                Hu3DModelAttrReset(MDL(IT.mdlId).mdlId, HU3D_MOTATTR_LOOP);
                Hu3DMotionTimeSet(MDL(IT.mdlId).mdlId, 0.0f);
                Hu3DMotionSpeedSet(MDL(IT.mdlId).mdlId, 4.0f);
                IT.type++;
                break;
            case 2:
                time = Hu3DMotionTimeGet(MDL(IT.mdlId).mdlId);
                if (time >= 97.0f) {
                    Hu3DModelAttrSet(MDL(IT.mdlId).mdlId, 1);
                    IT.active = 0;
                    IT.type = 0;
                }
                break;
            }
            break;
        case 3:
            switch (IT.type) {
            case 4:
                break;
            case 0:
                cnt = 0;
                for (j = 0; j < 100; j++) {
                    if (lbl_1_bss_214[j].active == 0) {
                        lbl_1_bss_214[j].active = 1;
                        lbl_1_bss_214[j].type = 2;
                        lbl_1_bss_214[j].cnt = 0;
                        lbl_1_bss_214[j].kind = 3;
                        lbl_1_bss_214[j].mdlId = cnt + 0x6E;
                        lbl_1_bss_214[j].slot = 0;
                        cnt++;
                    }
                    if (cnt >= 0x32) {
                        break;
                    }
                }
                IT.cnt = 0;
                IT.slot = 0;
                IT.type++;
                break;
            case 1:
                if (IT.cnt++ > 1) {
                    cnt = 0;
                    for (j = 0; j < 100; j++) {
                        if (lbl_1_bss_214[j].active != 0 && lbl_1_bss_214[j].cnt == 0 && lbl_1_bss_214[j].kind == 3
                            && lbl_1_bss_214[j].type == 2) {
                            if (IT.slot == 0) {
                                lbl_1_bss_214[j].cnt = 1;
                                lbl_1_bss_214[j].slot = cnt;
                            } else {
                                lbl_1_bss_214[j].cnt = 1;
                                lbl_1_bss_214[j].slot = (IT.slot - 1) ^ 1;
                                cnt = 1;
                            }
                            cnt++;
                        }
                        if (cnt >= 2) {
                            break;
                        }
                    }
                    IT.cnt = 0;
                }
                break;
            case 2:
                if (IT.cnt != 0) {
                    COPY_POS();
                    COPY_ROT();
                    Hu3DModelAttrReset(MDL(IT.mdlId).mdlId, 1);
                    Hu3DModelAttrReset(MDL(IT.mdlId).mdlId, HU3D_MOTATTR_LOOP);
                    Hu3DMotionTimeSet(MDL(IT.mdlId).mdlId, 0.0f);
                    Hu3DMotionSpeedSet(MDL(IT.mdlId).mdlId, 8.0f);
                    IT.fade = 0.0f;
                    IT.type++;
                }
                break;
            case 3:
                if (Hu3DModelAttrGet(MDL(IT.slot + 0x21).mdlId) & 1) {
                    Hu3DModelAttrSet(MDL(IT.mdlId).mdlId, 1);
                } else {
                    Hu3DModelAttrReset(MDL(IT.mdlId).mdlId, 1);
                }
                MDL(IT.mdlId).pos.z += 10.0f;
                IT.fade += 1.0f;
                if (IT.fade >= 12.5f) {
                    Hu3DModelAttrSet(MDL(IT.mdlId).mdlId, 1);
                    IT.cnt = 0;
                    IT.type = 2;
                }
                break;
            }
            break;
        case 4:
            switch (IT.type) {
            case 0:
                IT.type = 1;
                IT.mdlId = 0x6A;
                IT.slot = 0;
                SPAWN_PARTNER(4, 0x6B);
                if (i > j) {
                    break;
                }
                /* fallthrough */
            case 1:
                COPY_POS();
                COPY_ROT();
                Hu3DModelAttrReset(MDL(IT.mdlId).mdlId, 1);
                Hu3DModelAttrSet(MDL(IT.mdlId).mdlId, HU3D_MOTATTR_LOOP);
                Hu3DMotionTimeSet(MDL(IT.mdlId).mdlId, 0.0f);
                Hu3DMotionSpeedSet(MDL(IT.mdlId).mdlId, 1.0f);
                IT.type++;
                break;
            case 2:
                COPY_POS();
                COPY_ROT();
                if (Hu3DModelAttrGet(MDL(IT.slot + 0x21).mdlId) & 1) {
                    Hu3DModelAttrSet(MDL(IT.mdlId).mdlId, 1);
                } else {
                    Hu3DModelAttrReset(MDL(IT.mdlId).mdlId, 1);
                }
                break;
            case 3:
                COPY_POS();
                COPY_ROT();
                break;
            }
            break;
        case 6:
            switch (IT.type) {
            case 0:
                IT.mdlId = IT.slot + 0x6C;
                COPY_POS();
                MDL(IT.mdlId).pos.z += 130.0f;
                Hu3DModelAttrReset(MDL(IT.mdlId).mdlId, 1);
                Hu3DModelAttrSet(MDL(IT.mdlId).mdlId, HU3D_MOTATTR_LOOP);
                Hu3DMotionTimeSet(MDL(IT.mdlId).mdlId, 0.0f);
                Hu3DMotionSpeedSet(MDL(IT.mdlId).mdlId, 8.0f);
                IT.type++;
                /* fallthrough */
            case 1:
                MDL(IT.mdlId).pos.x = MDL(IT.slot + 0x21).pos.x + 80.0 * cos(3.141592653589793 * IT.angX / 180.0);
                MDL(IT.mdlId).pos.y = (30.0f + MDL(IT.slot + 0x21).pos.y) + 80.0 * sin(3.141592653589793 * IT.angY / 180.0);
                time = Hu3DMotionTimeGet(MDL(IT.mdlId).mdlId);
                if (time >= 90.0f) {
                    Hu3DModelAttrSet(MDL(IT.mdlId).mdlId, 1);
                    IT.active = 0;
                }
                break;
            }
            break;
        case 7:
            switch (IT.type) {
            case 0:
                Hu3DModelAttrReset(MDL(160).mdlId, 1);
                Hu3DModelAttrReset(MDL(160).mdlId, HU3D_MOTATTR_LOOP);
                Hu3DMotionTimeSet(MDL(160).mdlId, 0.0f);
                Hu3DMotionSpeedSet(MDL(160).mdlId, 1.0f);
                IT.type++;
                /* fallthrough */
            case 1:
                time = Hu3DMotionTimeGet(MDL(160).mdlId);
                if (time >= 50.0f) {
                    Hu3DMotionSpeedSet(MDL(160).mdlId, 4.0f);
                    IT.type++;
                }
                break;
            case 2:
                MDL(160).pos = lbl_1_bss_8->tgt;
                time = Hu3DMotionTimeGet(MDL(160).mdlId);
                if (time >= 94.0f) {
                    Hu3DMotionTimeSet(MDL(160).mdlId, 50.0f);
                }
                break;
            case 3:
                Hu3DMotionSpeedSet(MDL(160).mdlId, 0.5f);
                lbl_1_bss_1894.fogScale = 4.0f;
                IT.type++;
                /* fallthrough */
            case 4:
                lbl_1_bss_1894.fogScale -= 0.035f;
                if (lbl_1_bss_1894.fogScale <= 0.5f) {
                    lbl_1_bss_1894.fogScale = 0.5f;
                }
                Hu3DMotionSpeedSet(MDL(1).mdlId, lbl_1_bss_1894.fogScale);
                time = Hu3DMotionTimeGet(MDL(160).mdlId);
                if (time >= 148.0f) {
                    Hu3DModelAttrSet(MDL(160).mdlId, 1);
                    IT.type++;
                }
                break;
            }
            break;
        }
    }
}
