#include "math.h"
#include "game/hu3d.h"
#include "game/frand.h"

#define M555_PI 3.141592653589793

#define SC_MDL(i) (lbl_1_bss_3030->mdl[i])
#define SC_MODEL(i) (lbl_1_bss_3030->mdl[i].mdlId)

void fn_1_CFB8(void)
{
    s16 i;
    s16 k;
    s16 j;
    s16 r;
    s16 r2;
    s16 m;
    s16 side;
    f32 speed;
    f32 t;
    s16 idx[6] = { 2, 0, 6, 4, 10, 8 };
    s16 rng[16] = { 0x0BB8, 0x03E8, 0x07D0, 0xFC18, 0x0BB8, 0xF060, 0x07D0, 0xFC18,
                    0x03E8, 0xFE0C, 0x01F4, 0x03E8, 0x03E8, 0xFE0C, 0x0064, 0xF830 };

    if (lbl_1_bss_1894.motion >= 2) {
        speed = 200.0f;
    } else {
        speed = 66.666664f;
    }

    switch (lbl_1_bss_C.sw1) {
        case 0:
            for (i = 0x12, k = 0; i <= 0x15; i++, k++) {
                SC_MDL(i).pos.x = 0.0f;
                SC_MDL(i).pos.y = 0.0f;
                SC_MDL(i).pos.z = 0.0f;
                SC_MDL(i).rot.x = 0.0f;
                SC_MDL(i).rot.y = 90.0f * (f32)k;
                SC_MDL(i).rot.z = 0.0f;
                Hu3DModelLayerSet(SC_MODEL(i), 1);
                Hu3DModelAttrReset(SC_MODEL(i), 1);
                Hu3DModelAttrSet(SC_MODEL(i), 0x40000001);
                Hu3DMotionTimeSet(SC_MODEL(i), 25.0f * (f32)k);
                Hu3DMotionSpeedSet(SC_MODEL(i), 0.5f);
            }
            for (i = 0x16, k = 0; i <= 0x19; i++, k++) {
                SC_MDL(i).pos.x = 0.0f;
                SC_MDL(i).pos.y = 0.0f;
                SC_MDL(i).pos.z = 0.0f;
                SC_MDL(i).rot.x = 0.0f;
                SC_MDL(i).rot.y = 45.0f + (90.0f * (f32)k);
                SC_MDL(i).rot.z = 0.0f;
                Hu3DModelLayerSet(SC_MODEL(i), 1);
                Hu3DModelAttrReset(SC_MODEL(i), 1);
                Hu3DModelAttrSet(SC_MODEL(i), 0x40000001);
                Hu3DMotionTimeSet(SC_MODEL(i), 25.0f * (f32)k);
                Hu3DMotionSpeedSet(SC_MODEL(i), 0.5f);
            }
            lbl_1_bss_C.sw1++;
        case 1:
            for (i = 0x12; i <= 0x19; i++) {
                SC_MDL(i).rot.y -= 0.01f;
            }
            break;
    }

    switch (lbl_1_bss_C.sw2) {
        case 0:
            for (i = 0xE, k = 0; i <= 0x11; i++, k++) {
                SC_MDL(i).pos.x = (f32)frandmod(0x5DC) - 750.0f;
                SC_MDL(i).pos.y = (f32)frandmod(0x5DC) - 750.0f;
                SC_MDL(i).pos.z = -37000.0f + (9000.0f * (f32)k);
                Hu3DModelLayerSet(SC_MODEL(i), 2);
                Hu3DModelAttrReset(SC_MODEL(i), 1);
                Hu3DModelAttrSet(SC_MODEL(i), 0x40000001);
                Hu3DMotionTimeSet(SC_MODEL(i), 50.0f);
                Hu3DMotionSpeedSet(SC_MODEL(i), 1.0f);
            }
            lbl_1_bss_C.sw2++;
        case 1:
            for (i = 0xE; i <= 0x11; i++) {
                if (Hu3DMotionTimeGet(SC_MODEL(i)) >= 97.0f) {
                    Hu3DMotionTimeSet(SC_MODEL(i), 50.0f);
                }
                SC_MDL(i).pos.z += speed;
                if (SC_MDL(i).pos.z >= 1500.0f) {
                    SC_MDL(i).pos.x = (f32)frandmod(0x5DC) - 750.0f;
                    SC_MDL(i).pos.y = (f32)frandmod(0x5DC) - 750.0f;
                    SC_MDL(i).pos.z = -37000.0f;
                    Hu3DMotionTimeSet(SC_MODEL(i), 0.0f);
                    Hu3DMotionSpeedSet(SC_MODEL(i), 1.0f);
                    if (lbl_1_bss_1894.curPlayer != 0 || lbl_1_bss_1894.frameCnt >= 0x708) {
                        Hu3DModelAttrSet(SC_MODEL(i), 1);
                    }
                }
            }
            break;
    }

    switch (lbl_1_bss_C.sw3) {
        case 0:
            for (i = 2; i <= 0xD; i++) {
                Hu3DModelLayerSet(SC_MODEL(i), 2);
                Hu3DModelAttrSet(SC_MODEL(i), 0x40000001);
                Hu3DMotionTimeSet(SC_MODEL(i), 0.0f);
                Hu3DMotionSpeedSet(SC_MODEL(i), 0.0f);
            }
            for (j = 0; j < 10; j++) {
                r = frandmod(3);
                i = idx[r * 2];
                for (k = idx[r * 2 + 1]; k <= idx[r * 2 + 1] + 3; k++, i++) {
                    if (lbl_1_bss_C.rec[k].active == 0) {
                        lbl_1_bss_C.rec[k].active = 1;
                        lbl_1_bss_C.rec[k].state = 0;
                        lbl_1_bss_C.rec[k].f24 = (f32)frandmod(0x168);
                        r2 = frandmod(4);
                        SC_MDL(i).pos.x = (f32)(s16)(rng[r2 * 4 + 1] + frandmod(rng[r2 * 4]));
                        SC_MDL(i).pos.y = (f32)(s16)(rng[r2 * 4 + 3] + frandmod(rng[r2 * 4 + 2]));
                        SC_MDL(i).pos.z = -37000.0f + (4000.0f * (f32)j);
                        lbl_1_bss_C.rec[k].f20 = SC_MDL(i).pos.y;
                        Hu3DModelAttrReset(SC_MODEL(i), 1);
                        Hu3DMotionTimeSet(SC_MODEL(i), 50.0f);
                        Hu3DMotionSpeedSet(SC_MODEL(i), 1.0f);
                        break;
                    }
                }
            }
            lbl_1_bss_C.sw3++;
        case 1:
            if (lbl_1_bss_1894.motion >= 2) {
                lbl_1_bss_C.timer3 = 10;
            } else {
                lbl_1_bss_C.timer3 = frandmod(10) + 0x3C;
            }
            lbl_1_bss_C.sw3++;
        case 2:
            if (lbl_1_bss_1894.curPlayer != 0 || lbl_1_bss_1894.frameCnt >= 0x708) {
                lbl_1_bss_C.sw3++;
            } else {
                s16 tm = lbl_1_bss_C.timer3;
                lbl_1_bss_C.timer3--;
                if (tm <= 0) {
                    do {
                        r = frandmod(3);
                    } while (lbl_1_bss_C.used3[r] != 0);
                    lbl_1_bss_C.used3[r] = 1;
                    if (lbl_1_bss_C.used3[0] != 0 && lbl_1_bss_C.used3[1] != 0 && lbl_1_bss_C.used3[2] != 0) {
                        lbl_1_bss_C.used3[2] = 0;
                        lbl_1_bss_C.used3[1] = 0;
                        lbl_1_bss_C.used3[0] = 0;
                    }
                    i = idx[r * 2];
                    for (k = idx[r * 2 + 1]; k <= idx[r * 2 + 1] + 3; k++, i++) {
                        if (lbl_1_bss_C.rec[k].active == 0) {
                            lbl_1_bss_C.rec[k].active = 1;
                            lbl_1_bss_C.rec[k].state = 0;
                            lbl_1_bss_C.rec[k].f24 = (f32)frandmod(0x168);
                            do {
                                r2 = frandmod(4);
                            } while (lbl_1_bss_C.used4[r2] != 0);
                            lbl_1_bss_C.used4[r2] = 1;
                            if (lbl_1_bss_C.used4[0] != 0 && lbl_1_bss_C.used4[1] != 0 && lbl_1_bss_C.used4[2] != 0
                                && lbl_1_bss_C.used4[3] != 0) {
                                lbl_1_bss_C.used4[3] = 0;
                                lbl_1_bss_C.used4[2] = 0;
                                lbl_1_bss_C.used4[1] = 0;
                                lbl_1_bss_C.used4[0] = 0;
                            }
                            SC_MDL(i).pos.x = (f32)(s16)(rng[r2 * 4 + 1] + frandmod(rng[r2 * 4]));
                            SC_MDL(i).pos.y = (f32)(s16)(rng[r2 * 4 + 3] + frandmod(rng[r2 * 4 + 2]));
                            SC_MDL(i).pos.z = -37000.0f;
                            lbl_1_bss_C.rec[k].f20 = SC_MDL(i).pos.y;
                            Hu3DModelAttrReset(SC_MODEL(i), 1);
                            Hu3DMotionTimeSet(SC_MODEL(i), 0.0f);
                            Hu3DMotionSpeedSet(SC_MODEL(i), 1.0f);
                            break;
                        }
                    }
                    lbl_1_bss_C.sw3 = 1;
                }
            }
            break;
    }

    for (i = 2, k = 0; i <= 0xD; i++, k++) {
        if (lbl_1_bss_C.rec[k].active != 0) {
            switch (lbl_1_bss_C.rec[k].state) {
                case 0:
                    if (Hu3DMotionTimeGet(SC_MODEL(i)) >= 97.0f) {
                        Hu3DMotionTimeSet(SC_MODEL(i), 50.0f);
                    }
                    lbl_1_bss_C.rec[k].f24 += 0.5f;
                    SC_MDL(i).pos.z += speed;
                    if (SC_MDL(i).pos.z >= 1500.0f) {
                        if (lbl_1_bss_1894.curPlayer != 0 || lbl_1_bss_1894.frameCnt >= 0x708) {
                            lbl_1_bss_C.rec[k].state++;
                        } else {
                            lbl_1_bss_C.rec[k].active = 0;
                            Hu3DModelAttrSet(SC_MODEL(i), 1);
                            Hu3DMotionTimeSet(SC_MODEL(i), 0.0f);
                            Hu3DMotionSpeedSet(SC_MODEL(i), 0.0f);
                        }
                    }
                    break;
                case 1:
                    if (Hu3DMotionTimeGet(SC_MODEL(i)) >= 97.0f) {
                        Hu3DMotionTimeSet(SC_MODEL(i), 50.0f);
                    }
                    SC_MDL(i).pos.z += speed;
                    if (SC_MDL(i).pos.z >= 1500.0f) {
                        lbl_1_bss_C.rec[k].active = 0;
                        Hu3DModelAttrSet(SC_MODEL(i), 1);
                    }
                    break;
            }
        }
    }

    switch (lbl_1_bss_C.sw5) {
        case 2:
            break;
        case 0:
            SC_MDL(28).pos.x = 16000.0f;
            SC_MDL(28).pos.y = 1000.0f;
            SC_MDL(28).pos.z = -30000.0f;
            SC_MDL(28).rot.x = 0.0f;
            SC_MDL(28).rot.y = -90.0f;
            SC_MDL(28).rot.z = 0.0f;
            Hu3DModelAttrReset(SC_MODEL(28), 1);
            Hu3DModelAttrSet(SC_MODEL(28), 0x40000001);
            Hu3DModelLayerSet(SC_MODEL(28), 2);
            Hu3DMotionTimeSet(SC_MODEL(28), 0.0f);
            Hu3DMotionSpeedSet(SC_MODEL(28), 2.0f);
            for (i = 0x1D; i <= 0x20; i++) {
                Hu3DModelLayerSet(SC_MODEL(i), 2);
            }
            lbl_1_bss_C.sw5++;
        case 1:
            if (lbl_1_bss_1894.motion >= 2) {
                t = 50.0f;
            } else {
                t = 10.0f;
            }
            SC_MDL(28).pos.x -= 19.0f;
            SC_MDL(28).pos.z += t;
            {
                s16 c = lbl_1_bss_C.cnt5;
                lbl_1_bss_C.cnt5++;
                if ((f32)c >= 30.0f) {
                    for (i = 0x1D, k = 0; i <= 0x20; i++, k++) {
                        if (lbl_1_bss_C.ring[k].on == 0) {
                            lbl_1_bss_C.ring[k].on = 1;
                            lbl_1_bss_C.ring[k].hit = 0;
                            SC_MDL(i).pos.x = 150.0f + SC_MDL(28).pos.x;
                            SC_MDL(i).pos.y = SC_MDL(28).pos.y;
                            SC_MDL(i).pos.z = SC_MDL(28).pos.z;
                            Hu3DModelAttrReset(SC_MODEL(i), 1);
                            Hu3DMotionTimeSet(SC_MODEL(i), 0.0f);
                            Hu3DMotionSpeedSet(SC_MODEL(i), 1.0f);
                            break;
                        }
                    }
                    lbl_1_bss_C.cnt5 = 0;
                }
            }
            if (SC_MDL(28).pos.z >= 36000.0f) {
                Hu3DModelAttrSet(SC_MODEL(28), 1);
                lbl_1_bss_C.sw5++;
            }
            break;
        case 3:
            SC_MDL(28).pos.x -= 10.0f;
            {
                s16 c = lbl_1_bss_C.cnt5;
                lbl_1_bss_C.cnt5++;
                if ((f32)c >= 30.0f) {
                    for (i = 0x1D, k = 0; i <= 0x20; i++, k++) {
                        if (lbl_1_bss_C.ring[k].on == 0) {
                            lbl_1_bss_C.ring[k].on = 1;
                            lbl_1_bss_C.ring[k].hit = 0;
                            SC_MDL(i).pos.x = 150.0f + SC_MDL(28).pos.x;
                            SC_MDL(i).pos.y = SC_MDL(28).pos.y;
                            SC_MDL(i).pos.z = SC_MDL(28).pos.z;
                            Hu3DModelAttrReset(SC_MODEL(i), 1);
                            Hu3DMotionTimeSet(SC_MODEL(i), 0.0f);
                            Hu3DMotionSpeedSet(SC_MODEL(i), 1.0f);
                            break;
                        }
                    }
                    lbl_1_bss_C.cnt5 = 0;
                }
            }
            break;
    }

    for (i = 0x1D, k = 0; i <= 0x20; i++, k++) {
        if (lbl_1_bss_C.ring[k].on != 0) {
            if (lbl_1_bss_C.ring[k].hit != 0) {
            } else {
                if (lbl_1_bss_1894.motion >= 2) {
                    t = 50.0f;
                } else {
                    t = 10.0f;
                }
                SC_MDL(i).pos.z += t;
                if (Hu3DMotionTimeGet(SC_MODEL(i)) >= 97.0f) {
                    lbl_1_bss_C.ring[k].on = 0;
                    Hu3DModelAttrSet(SC_MODEL(i), 1);
                }
            }
        }
    }

    switch (lbl_1_bss_C.sw6) {
        case 4:
            break;
        case 0:
            for (i = 0x60; i <= 0x63; i++) {
                Hu3DModelAttrSet(SC_MODEL(i), 0x40000001);
                Hu3DMotionTimeSet(SC_MODEL(i), 0.0f);
                Hu3DMotionSpeedSet(SC_MODEL(i), 1.0f);
            }
            lbl_1_bss_C.sw6++;
        case 1:
            if (lbl_1_bss_1894.motion >= 1) {
                lbl_1_bss_C.sw6++;
            }
            break;
        case 2:
            lbl_1_bss_C.timer6 = 0xC8;
            lbl_1_bss_C.sw6++;
            break;
        case 3:
            if (lbl_1_bss_1894.curPlayer != 0 || lbl_1_bss_1894.frameCnt >= 0x708) {
                lbl_1_bss_C.sw6 = 3;
            } else {
                s16 tm = lbl_1_bss_C.timer6;
                lbl_1_bss_C.timer6--;
                if (tm <= 0) {
                    do {
                        r = frandmod(4);
                        m = r + 0x60;
                    } while (lbl_1_bss_C.used6[r] != 0);
                    lbl_1_bss_C.used6[r] = 1;
                    if (lbl_1_bss_C.used6[0] != 0 && lbl_1_bss_C.used6[1] != 0 && lbl_1_bss_C.used6[2] != 0
                        && lbl_1_bss_C.used6[3] != 0) {
                        lbl_1_bss_C.used6[3] = 0;
                        lbl_1_bss_C.used6[2] = 0;
                        lbl_1_bss_C.used6[1] = 0;
                        lbl_1_bss_C.used6[0] = 0;
                    }
                    if (lbl_1_bss_C.bird[r].on == 0) {
                        side = frandmod(2);
                        lbl_1_bss_C.bird[r].on = 1;
                        lbl_1_bss_C.bird[r].state = 0;
                        if (side != 0) {
                            SC_MDL(m).pos.x = 600.0f + (f32)frandmod(0x64);
                        } else {
                            SC_MDL(m).pos.x = (f32)frandmod(0x64) - 700.0f;
                        }
                        SC_MDL(m).pos.y = (f32)frandmod(0x3E8) - 500.0f;
                        SC_MDL(m).pos.z = -30000.0f;
                        lbl_1_bss_C.bird[r].f14C = SC_MDL(m).pos.y;
                        lbl_1_bss_C.bird[r].f150 = 0.0f;
                        if (side != 0) {
                            SC_MDL(m).rot.y = 225.0f;
                        } else {
                            SC_MDL(m).rot.y = 135.0f;
                        }
                        Hu3DModelAttrReset(SC_MODEL(m), 1);
                    }
                    lbl_1_bss_C.sw6 = 1;
                }
            }
            break;
    }

    for (i = 0x60, k = 0; i <= 0x63; i++, k++) {
        if (lbl_1_bss_C.bird[k].on != 0) {
            switch (lbl_1_bss_C.bird[k].state) {
                case 0:
                    lbl_1_bss_C.bird[k].f150 += 3.0f;
                    SC_MDL(i).pos.y = lbl_1_bss_C.bird[k].f14C + 50.0 * sin(M555_PI * lbl_1_bss_C.bird[k].f150 / 180.0);
                    SC_MDL(i).pos.z += 50.0f;
                    if (SC_MDL(i).pos.z >= 1500.0f) {
                        if (frandmod(2) != 0) {
                            SC_MDL(i).rot.y = 225.0f;
                            SC_MDL(i).pos.x = 600.0f + (f32)frandmod(0x64);
                        } else {
                            SC_MDL(i).rot.y = 135.0f;
                            SC_MDL(i).pos.x = (f32)frandmod(0x64) - 700.0f;
                        }
                        SC_MDL(i).pos.y = (f32)frandmod(0x3E8) - 500.0f;
                        SC_MDL(i).pos.z = -30000.0f;
                        lbl_1_bss_C.bird[k].f14C = SC_MDL(i).pos.y;
                        lbl_1_bss_C.bird[k].f150 = 0.0f;
                        if (lbl_1_bss_1894.curPlayer != 0 || lbl_1_bss_1894.frameCnt >= 0x4B0) {
                            Hu3DModelAttrSet(SC_MODEL(i), 1);
                            SC_MDL(i).pos.z = 1500.0f;
                            lbl_1_bss_C.bird[k].state = 2;
                        }
                    }
                    if (lbl_1_bss_1894.curPlayer != 0 || lbl_1_bss_1894.frameCnt >= 0x7D0) {
                        lbl_1_bss_C.bird[k].f154 = SC_MDL(i).rot.y;
                        lbl_1_bss_C.bird[k].state++;
                    }
                    break;
                case 1:
                    lbl_1_bss_C.bird[k].f150 += 3.0f;
                    SC_MDL(i).pos.y = lbl_1_bss_C.bird[k].f14C + 50.0 * sin(M555_PI * lbl_1_bss_C.bird[k].f150 / 180.0);
                    SC_MDL(i).pos.z += 50.0f;
                    if (SC_MDL(i).pos.z >= 1500.0f) {
                        lbl_1_bss_C.bird[k].f158 += 0.3f;
                        SC_MDL(i).pos.z += lbl_1_bss_C.bird[k].f158;
                        if (SC_MDL(i).pos.z >= 50000.0f) {
                            Hu3DModelAttrSet(SC_MODEL(i), 1);
                            lbl_1_bss_C.bird[k].state++;
                        }
                    }
                    break;
            }
        }
    }
}
