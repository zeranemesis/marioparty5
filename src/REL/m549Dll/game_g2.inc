/* game.c part G2: fn_1_762C .. fn_1_A18C */
#include "math.h"

/* model table entry position as a vector (x, y, z are contiguous) */
#define M549G2_MDLPOS(idx) ((HuVecF *)&lbl_1_bss_15F4->models[idx].x)

s32 fn_1_762C(s16 arg0)
{
    s32 ret;
    s16 done;
    s8 stkX;
    s8 stkY;
    f32 x;
    f32 y;

    ret = 0;
    done = 0;
    if (lbl_1_bss_3B0[arg0].s62 > 20 && (lbl_1_bss_3B0[arg0].s6A != 0 || lbl_1_bss_3B0[arg0].s6C != 0)) {
        lbl_1_bss_3B0[arg0].s28 = 1;
        if (lbl_1_bss_3B0[arg0].state != 1) {
            lbl_1_bss_3B0[arg0].state = 0;
        }
        lbl_1_bss_3B0[arg0].mode = 1;
    }
    done = lbl_1_data_49C[lbl_1_bss_3B0[arg0].mode](arg0);
    if (done != 0 && (lbl_1_bss_3B0[arg0].s6A != 0 || lbl_1_bss_3B0[arg0].s6C != 0)) {
        if (lbl_1_bss_580.s22 == 0) {
            lbl_1_bss_3B0[arg0].s28 = 0;
        }
        lbl_1_bss_3B0[arg0].s62 = 0;
        lbl_1_bss_3B0[arg0].mode = 0;
    }
    if (++lbl_1_bss_3B0[arg0].s62 <= 20 && lbl_1_bss_3B0[arg0].mode == 0) {
        if (Hu3DModelAttrGet(lbl_1_bss_15F4->models[arg0 + 0x44].id) & 1) {
            Hu3DModelAttrReset(lbl_1_bss_15F4->models[arg0 + 0x44].id, 1);
        } else {
            Hu3DModelAttrSet(lbl_1_bss_15F4->models[arg0 + 0x44].id, 1);
        }
    } else if (lbl_1_bss_3B0[arg0].mode == 0 && (lbl_1_bss_3B0[arg0].s6A != 0 || lbl_1_bss_3B0[arg0].s6C != 0)) {
        lbl_1_bss_3B0[arg0].s6A = 0;
        lbl_1_bss_3B0[arg0].s6C = 0;
        Hu3DModelAttrReset(lbl_1_bss_15F4->models[arg0 + 0x44].id, 1);
    } else {
        lbl_1_bss_3B0[arg0].s62 = 0x78;
    }
    if (lbl_1_bss_3B0[arg0].s12-- <= 0) {
        lbl_1_bss_3B0[arg0].s12 = 0;
    }
    lbl_1_bss_3B0[arg0].s60 = lbl_1_bss_3B0[arg0].s5C / 30;
    if (lbl_1_bss_3B0[arg0].s6E == 0) {
        stkX = HuPadStkX[lbl_1_bss_0.player[arg0]->padNo];
        stkY = HuPadStkY[lbl_1_bss_0.player[arg0]->padNo];
        x = stkX * lbl_1_data_484[lbl_1_bss_3B0[arg0].s60];
        y = stkY * lbl_1_data_484[lbl_1_bss_3B0[arg0].s60];
        lbl_1_bss_3B0[arg0].velX = x;
        lbl_1_bss_3B0[arg0].velZ = y;
        HuPadStkX[lbl_1_bss_0.player[arg0]->padNo] = (s8)x;
        HuPadStkY[lbl_1_bss_0.player[arg0]->padNo] = (s8)y;
    }
    if (lbl_1_bss_3B0[arg0].mode == 2) {
        fn_1_99E8(arg0);
    } else if (lbl_1_bss_580.s20 != 0) {
        fn_1_99E8(arg0);
    }
    return ret;
}

void fn_1_7CC0(s16 arg0)
{
    HuVecF vec;
    f32 scale;
    s16 i;
    s16 shadow;
    s16 body;

    for (i = 0; i < 10; i++) {
        if (lbl_1_bss_158[i].active == 0) {
            body = i + 5;
            lbl_1_bss_158[i].active = 1;
            lbl_1_bss_158[i].s04 = 0;
            lbl_1_bss_158[i].s02 = 0;
            lbl_1_bss_158[i].s06 = 0;
            lbl_1_bss_158[i].side = arg0;
            lbl_1_bss_158[i].s0A = 0;
            lbl_1_bss_158[i].angle = lbl_1_bss_0.player[arg0]->actor->rotY;
            lbl_1_bss_158[i].level = lbl_1_bss_3B0[arg0].s5C / 30;
            lbl_1_bss_158[i].s12 = lbl_1_bss_3B0[arg0].s5C;
            lbl_1_bss_158[i].radius = 50.0f + 10.0f * lbl_1_bss_158[i].level;
            lbl_1_bss_158[i].size = 12.5f;
            lbl_1_bss_158[i].f1C = 0.0f;
            lbl_1_bss_158[i].f20 = 0.05f + 0.05f * lbl_1_bss_158[i].level;
            lbl_1_bss_158[i].dir.x = (15.0f + 5.0f * lbl_1_bss_158[i].level) * sin(M_PI * lbl_1_bss_158[i].angle / 180.0);
            lbl_1_bss_158[i].dir.z = (15.0f + 5.0f * lbl_1_bss_158[i].level) * cos(M_PI * lbl_1_bss_158[i].angle / 180.0);
            Hu3DModelObjPosGet(lbl_1_bss_15F4->models[arg0 + 0x44].id, lbl_1_bss_3B0[arg0].hook, &vec);
            lbl_1_bss_158[i].pos.x = vec.x + 40.0 * sin(M_PI * lbl_1_bss_158[i].angle / 180.0);
            lbl_1_bss_158[i].pos.y = 20.0f + vec.y;
            lbl_1_bss_158[i].pos.z = vec.z + 40.0 * cos(M_PI * lbl_1_bss_158[i].angle / 180.0);
            *M549G2_MDLPOS(body) = lbl_1_bss_158[i].pos;
            lbl_1_bss_15F4->models[body].rotY = 90.0f + lbl_1_bss_158[i].angle;
            scale = lbl_1_bss_158[i].size / 50.0f;
            Hu3DModelScaleSet(lbl_1_bss_15F4->models[body].id, scale, scale, scale);
            Hu3DModelAttrReset(lbl_1_bss_15F4->models[body].id, 1);
            shadow = i + 0x1A;
            *M549G2_MDLPOS(shadow) = lbl_1_bss_158[i].pos;
            lbl_1_bss_15F4->models[shadow].y = 130.0f;
            lbl_1_bss_15F4->models[shadow].rotY = lbl_1_bss_158[i].angle;
            Hu3DMotionTimeSet(lbl_1_bss_15F4->models[shadow].id, 0.0f);
            Hu3DMotionSpeedSet(lbl_1_bss_15F4->models[shadow].id, 1.0f + 0.25f * lbl_1_bss_158[i].level);
            Hu3DModelAttrReset(lbl_1_bss_15F4->models[shadow].id, 1);
            Hu3DModelScaleSet(lbl_1_bss_15F4->models[shadow].id, 1.0f, 1.0f, lbl_1_bss_158[i].f1C);
            return;
        }
    }
}

void fn_1_8570(void)
{
    f32 dist;
    f32 scale;
    f32 time;
    s16 i;
    s16 shadow;
    s16 body;

    for (i = 0; i < 10; i++) {
        if (lbl_1_bss_158[i].active != 0) {
            shadow = i + 0x1A;
            body = i + 5;
            dist = sqrt(lbl_1_bss_158[i].pos.x * lbl_1_bss_158[i].pos.x + lbl_1_bss_158[i].pos.z * lbl_1_bss_158[i].pos.z);
            if (dist >= 645.0f && lbl_1_bss_158[i].s06 == 0) {
                lbl_1_bss_158[i].s06 = 1;
                lbl_1_bss_158[i].s04 = 2;
            }
            if (lbl_1_bss_158[i].s0A != 0 && lbl_1_bss_158[i].s06 == 0) {
                Hu3DModelAttrSet(lbl_1_bss_15F4->models[body].id, 1);
                lbl_1_bss_158[i].s06 = 1;
                lbl_1_bss_158[i].s02 = 2;
                lbl_1_bss_158[i].s04 = 4;
            }
            switch (lbl_1_bss_158[i].s02) {
                case 0:
                    lbl_1_bss_158[i].pos.x += lbl_1_bss_158[i].dir.x;
                    lbl_1_bss_158[i].pos.z += lbl_1_bss_158[i].dir.z;
                    lbl_1_bss_158[i].size += 10.0f;
                    if (lbl_1_bss_158[i].size >= lbl_1_bss_158[i].radius) {
                        lbl_1_bss_158[i].size = lbl_1_bss_158[i].radius;
                        lbl_1_bss_158[i].s02++;
                    }
                    scale = lbl_1_bss_158[i].size / 50.0f;
                    Hu3DModelScaleSet(lbl_1_bss_15F4->models[body].id, scale, scale, scale);
                    break;
                case 1:
                    lbl_1_bss_158[i].pos.x += lbl_1_bss_158[i].dir.x;
                    lbl_1_bss_158[i].pos.z += lbl_1_bss_158[i].dir.z;
                    if (dist >= 695.0f) {
                        scale = lbl_1_bss_158[i].size / 50.0f;
                        Hu3DModelScaleSet(lbl_1_bss_15F4->models[body].id, scale, scale, scale);
                        lbl_1_bss_158[i].size -= 1.5f;
                        if (lbl_1_bss_158[i].size <= 0.0f) {
                            Hu3DModelAttrSet(lbl_1_bss_15F4->models[body].id, 1);
                            lbl_1_bss_158[i].active = 0;
                            lbl_1_bss_158[i].size = 0.0f;
                        }
                    }
                    break;
            }
            *M549G2_MDLPOS(body) = lbl_1_bss_158[i].pos;
            switch (lbl_1_bss_158[i].s04) {
                case 0:
                    lbl_1_bss_15F4->models[shadow].x = lbl_1_bss_15F4->models[body].x;
                    lbl_1_bss_15F4->models[shadow].z = lbl_1_bss_15F4->models[body].z;
                    lbl_1_bss_158[i].f1C += lbl_1_bss_158[i].f20;
                    if (lbl_1_bss_158[i].f1C >= 1.0f) {
                        lbl_1_bss_158[i].f1C = 1.0f;
                    }
                    Hu3DModelScaleSet(lbl_1_bss_15F4->models[shadow].id, 1.0f, 1.0f, lbl_1_bss_158[i].f1C);
                    time = Hu3DMotionTimeGet(lbl_1_bss_15F4->models[shadow].id);
                    if (time >= 30.0f) {
                        lbl_1_bss_158[i].s04++;
                    }
                    break;
                case 1:
                    lbl_1_bss_15F4->models[shadow].x = lbl_1_bss_15F4->models[body].x;
                    lbl_1_bss_15F4->models[shadow].z = lbl_1_bss_15F4->models[body].z;
                    lbl_1_bss_158[i].f1C += lbl_1_bss_158[i].f20;
                    if (lbl_1_bss_158[i].f1C >= 1.0f) {
                        lbl_1_bss_158[i].f1C = 1.0f;
                    }
                    Hu3DModelScaleSet(lbl_1_bss_15F4->models[shadow].id, 1.0f, 1.0f, lbl_1_bss_158[i].f1C);
                    time = Hu3DMotionTimeGet(lbl_1_bss_15F4->models[shadow].id);
                    if (time >= 60.0f) {
                        Hu3DMotionTimeSet(lbl_1_bss_15F4->models[shadow].id, 30.0f);
                    }
                    break;
                case 2:
                    Hu3DMotionTimeSet(lbl_1_bss_15F4->models[shadow].id, 60.0f);
                    lbl_1_bss_158[i].s04++;
                case 3:
                    lbl_1_bss_15F4->models[shadow].x = lbl_1_bss_15F4->models[body].x;
                    lbl_1_bss_15F4->models[shadow].z = lbl_1_bss_15F4->models[body].z;
                    time = Hu3DMotionTimeGet(lbl_1_bss_15F4->models[shadow].id);
                    if (time >= 87.0f) {
                        Hu3DModelAttrSet(lbl_1_bss_15F4->models[shadow].id, 1);
                        lbl_1_bss_158[i].s04 = 6;
                    }
                    break;
                case 4:
                    Hu3DMotionTimeSet(lbl_1_bss_15F4->models[shadow].id, 60.0f);
                    lbl_1_bss_158[i].s04++;
                case 5:
                    lbl_1_bss_158[i].f1C -= lbl_1_bss_158[i].f20;
                    if (lbl_1_bss_158[i].f1C <= 0.0f) {
                        lbl_1_bss_158[i].f1C = 0.0f;
                    }
                    Hu3DModelScaleSet(lbl_1_bss_15F4->models[shadow].id, 1.0f, 1.0f, lbl_1_bss_158[i].f1C);
                    time = Hu3DMotionTimeGet(lbl_1_bss_15F4->models[shadow].id);
                    if (time >= 87.0f || lbl_1_bss_158[i].f1C == 0.0f) {
                        Hu3DModelAttrSet(lbl_1_bss_15F4->models[shadow].id, 1);
                        lbl_1_bss_158[i].active = 0;
                        lbl_1_bss_158[i].s04 = 6;
                    }
                    break;
                case 6:
                    break;
            }
        }
    }
}

void fn_1_9324(HuVecF *arg0, s16 arg1)
{
    s16 i;
    s16 j;
    s16 model;
    s16 sc;
    s16 pan;
    s32 sfx;

    sfx = 0;
    for (i = 0; i < 10; i++) {
        if (lbl_1_bss_18[i].active == 0) {
            model = i + 0xF;
            lbl_1_bss_18[i].active = 1;
            lbl_1_bss_18[i].s04 = 0;
            lbl_1_bss_18[i].s02 = 0;
            lbl_1_bss_18[i].s06 = 30;
            lbl_1_bss_18[i].s08 = 0;
            lbl_1_bss_18[i].type = arg1;
            lbl_1_bss_18[i].fx0C = 100.0f + 50.0f * lbl_1_bss_18[i].type;
            lbl_1_bss_18[i].fx10 = 100.0f + 50.0f * lbl_1_bss_18[i].type;
            lbl_1_bss_18[i].pos = *arg0;
            lbl_1_bss_18[i].pos.y = 150.0f;
            *M549G2_MDLPOS(model) = lbl_1_bss_18[i].pos;
            sc = lbl_1_bss_18[i].fx10 / 50.0f;
            Hu3DModelAttrReset(lbl_1_bss_15F4->models[model].id, 1);
            Hu3DModelScaleSet(lbl_1_bss_15F4->models[model].id, sc, sc, sc);
            Hu3DMotionTimeSet(lbl_1_bss_15F4->models[model].id, 0.0f);
            Hu3DMotionSpeedSet(lbl_1_bss_15F4->models[model].id, 1.0f);
            for (j = 0; j < 2; j++) {
                switch (lbl_1_bss_18[i].type) {
                    case 0:
                        omVibrate(lbl_1_bss_15F4->chars[j].playerNo, 20, 6, 6);
                        break;
                    case 1:
                        omVibrate(lbl_1_bss_15F4->chars[j].playerNo, 30, 6, 6);
                        break;
                    case 2:
                        omVibrate(lbl_1_bss_15F4->chars[j].playerNo, 20, 4, 2);
                        break;
                    case 3:
                        omVibrate(lbl_1_bss_15F4->chars[j].playerNo, 30, 4, 2);
                        break;
                    case 4:
                        omVibrate(lbl_1_bss_15F4->chars[j].playerNo, 30, 12, 0);
                        break;
                }
            }
            switch (arg1) {
                case 0:
                case 1:
                    sfx = 0;
                    break;
                case 2:
                case 3:
                    sfx = 1;
                    break;
                case 4:
                    sfx = 2;
                    break;
            }
            sfx += 5;
            pan = 64.0f + 0.026666667f * lbl_1_bss_18[i].pos.x;
            fn_1_3754(sfx, 0x7F, pan);
            return;
        }
    }
}

void fn_1_98F0(void)
{
    s16 i;
    s16 model;

    for (i = 0; i < 10; i++) {
        if (lbl_1_bss_18[i].active != 0) {
            model = i + 0xF;
            if (lbl_1_bss_18[i].s02 != 0) {
            } else if (Hu3DMotionEndCheck(lbl_1_bss_15F4->models[model].id) != 0) {
                lbl_1_bss_18[i].active = 0;
                Hu3DModelAttrSet(lbl_1_bss_15F4->models[model].id, 1);
            }
        }
    }
}

s32 fn_1_99E8(s16 arg0)
{
    M549StageSlot *slot;
    s32 done;

    slot = &lbl_1_bss_15F4->slots[arg0];
    done = 0;
    switch (lbl_1_bss_3B0[arg0].s06) {
        case 0:
            Hu3DModelObjPosGet(lbl_1_bss_15F4->models[arg0 + 0x44].id, lbl_1_bss_3B0[arg0].hook, M549G2_MDLPOS(arg0 + 3));
            lbl_1_bss_15F4->models[arg0 + 3].rotY = 90.0f - 180.0f * arg0;
            Hu3DModelHookObjReset(lbl_1_bss_15F4->models[arg0 + 0x44].id, lbl_1_bss_3B0[arg0].hook);
            if (lbl_1_bss_3B0[arg0].mode == 0) {
                slot->shiftState = 1;
                slot->shiftMotion = 0;
                slot->shiftStart = 0.0f;
                slot->shiftEnd = 16.0f;
                slot->shiftAttr = 0x40000001;
                slot->attrReset = 0;
            }
            lbl_1_bss_3B0[arg0].scale = 1.0f;
            lbl_1_bss_3B0[arg0].s06++;
            break;
        case 1:
            lbl_1_bss_3B0[arg0].scale -= 0.035f;
            lbl_1_bss_15F4->models[arg0 + 3].y += 10.0f;
            lbl_1_bss_15F4->models[arg0 + 3].rotX += 20.0f;
            Hu3DModelScaleSet(lbl_1_bss_15F4->models[arg0 + 3].id, lbl_1_bss_3B0[arg0].scale, lbl_1_bss_3B0[arg0].scale, lbl_1_bss_3B0[arg0].scale);
            if (lbl_1_bss_3B0[arg0].scale <= 0.1f) {
                Hu3DModelAttrSet(lbl_1_bss_15F4->models[arg0 + 3].id, 1);
                lbl_1_bss_3B0[arg0].s06++;
            }
            break;
        case 2:
            done = 1;
            break;
    }
    return done;
}

void fn_1_9DBC(void)
{
    s16 cnt;
    s16 i;

    cnt = 0;
    if (fn_1_1B8C(lbl_1_bss_580.mgTimer) != 0) {
        lbl_1_bss_580.s22 = 1;
    }
    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_3B0[i].s14 != 0) {
            lbl_1_bss_3B0[i].s6A = 0;
            lbl_1_bss_3B0[i].s6C = 0;
            lbl_1_bss_3B0[i].mode = 2;
            lbl_1_bss_580.s22 = 1;
            lbl_1_bss_580.s24 = (i ^ 1) | (i * 2);
        }
        if (lbl_1_bss_580.s22 != 0) {
            lbl_1_bss_3B0[i].s28 = 1;
            if (lbl_1_bss_3B0[i].mode != 2) {
                if (lbl_1_bss_3B0[i].mode == 0 && lbl_1_bss_3B0[i].state == 0) {
                    cnt++;
                }
            } else {
                cnt++;
            }
        }
    }
    if (cnt >= 2) {
        lbl_1_bss_580.s20 = 1;
        if ((lbl_1_bss_3B0[0].s14 != 0 && lbl_1_bss_3B0[1].s14 != 0) || (lbl_1_bss_3B0[0].s14 == 0 && lbl_1_bss_3B0[1].s14 == 0)) {
            lbl_1_bss_580.s24 = 0;
        }
    }
}

s32 fn_1_9FF4(s16 arg0, s16 arg1)
{
    s32 ret;

    ret = 0;
    switch (lbl_1_bss_3B0[arg0].sub) {
        case 0:
            lbl_1_bss_15F4->chars[arg0].btnDown |= 0x100;
            lbl_1_bss_3B0[arg0].sub++;
        case 1:
            if (lbl_1_bss_3B0[arg0].state == 1) {
                lbl_1_bss_15F4->chars[arg0].btn |= 0x100;
                if (lbl_1_bss_3B0[arg0].s5C >= arg1) {
                    lbl_1_bss_3B0[arg0].sub++;
                }
            } else {
                lbl_1_bss_3B0[arg0].sub = 0;
            }
            break;
        case 2:
            if (lbl_1_bss_3B0[arg0].state != 2) {
                lbl_1_bss_3B0[arg0].sub++;
            }
            break;
        case 3:
            ret = 1;
            break;
    }
    return ret;
}

s32 fn_1_A18C(void)
{
    s32 ret;
    s32 a;
    s32 b;
    f32 steps;

    ret = 0;
    a = 0;
    switch (lbl_1_bss_580.mode) {
        case 0:
            lbl_1_bss_15F4->chars[0].btn = 0;
            lbl_1_bss_15F4->chars[0].btnDown = 0;
            fn_1_36A4(10);
            lbl_1_bss_3B0[0].sub = 0;
            lbl_1_bss_3B0[1].sub = 0;
            lbl_1_bss_580.timer = 0;
            lbl_1_bss_580.mode++;
            break;
        case 1:
            if (lbl_1_bss_580.timer++ >= 90) {
                lbl_1_bss_580.timer = 0;
                lbl_1_bss_580.mode++;
            }
            break;
        case 2:
            switch (lbl_1_bss_3B0[0].sub) {
                case 0:
                    lbl_1_bss_15F4->chars[0].btnDown |= 0x100;
                    lbl_1_bss_3B0[0].sub++;
                case 1:
                    if (lbl_1_bss_3B0[0].state == 1) {
                        lbl_1_bss_15F4->chars[0].btn |= 0x100;
                        if (lbl_1_bss_3B0[0].s5C >= 180) {
                            lbl_1_bss_3B0[0].sub++;
                        }
                    } else {
                        lbl_1_bss_3B0[0].sub = 0;
                    }
                    break;
                case 2:
                    if (lbl_1_bss_3B0[0].state != 2) {
                        lbl_1_bss_3B0[0].sub++;
                    }
                    break;
            }
            lbl_1_bss_580.timer++;
            steps = 50.0f;
            a = fn_1_18B0(&lbl_1_bss_14[0].zoom, steps, lbl_1_data_288[0].zoom, lbl_1_data_288[2].zoom);
            a = fn_1_18B0(&lbl_1_bss_14[1].zoom, steps, lbl_1_data_288[1].zoom, lbl_1_data_288[2].zoom);
            if (a == 0) {
                lbl_1_bss_580.mode++;
            }
            break;
        case 3:
            switch (lbl_1_bss_3B0[0].sub) {
                case 0:
                    lbl_1_bss_15F4->chars[0].btnDown |= 0x100;
                    lbl_1_bss_3B0[0].sub++;
                case 1:
                    if (lbl_1_bss_3B0[0].state == 1) {
                        lbl_1_bss_15F4->chars[0].btn |= 0x100;
                        if (lbl_1_bss_3B0[0].s5C >= 180) {
                            lbl_1_bss_3B0[0].sub++;
                        }
                    } else {
                        lbl_1_bss_3B0[0].sub = 0;
                    }
                    break;
                case 2:
                    if (lbl_1_bss_3B0[0].state != 2) {
                        lbl_1_bss_3B0[0].sub++;
                    }
                    break;
            }
            if (lbl_1_bss_580.timer++ >= 120) {
                lbl_1_bss_580.timer = 0;
                lbl_1_bss_580.mode++;
            }
            break;
        case 4:
            if (lbl_1_bss_580.timer++ <= 11) {
                switch (lbl_1_bss_3B0[0].sub) {
                    case 0:
                        lbl_1_bss_15F4->chars[0].btnDown |= 0x100;
                        lbl_1_bss_3B0[0].sub++;
                    case 1:
                        if (lbl_1_bss_3B0[0].state == 1) {
                            lbl_1_bss_15F4->chars[0].btn |= 0x100;
                            if (lbl_1_bss_3B0[0].s5C >= 180) {
                                lbl_1_bss_3B0[0].sub++;
                            }
                        } else {
                            lbl_1_bss_3B0[0].sub = 0;
                        }
                        break;
                    case 2:
                        if (lbl_1_bss_3B0[0].state != 2) {
                            lbl_1_bss_3B0[0].sub++;
                        }
                        break;
                }
            } else {
                a = 1;
            }
            b = 0;
            switch (lbl_1_bss_3B0[1].sub) {
                case 0:
                    lbl_1_bss_15F4->chars[1].btnDown |= 0x100;
                    lbl_1_bss_3B0[1].sub++;
                case 1:
                    if (lbl_1_bss_3B0[1].state == 1) {
                        lbl_1_bss_15F4->chars[1].btn |= 0x100;
                        if (lbl_1_bss_3B0[1].s5C >= 0) {
                            lbl_1_bss_3B0[1].sub++;
                        }
                    } else {
                        lbl_1_bss_3B0[1].sub = 0;
                    }
                    break;
                case 2:
                    if (lbl_1_bss_3B0[1].state != 2) {
                        lbl_1_bss_3B0[1].sub++;
                    }
                    break;
                case 3:
                    b = 1;
                    break;
            }
            if ((s16)(a + b) >= 2) {
                lbl_1_bss_580.timer = 0;
                lbl_1_bss_580.mode++;
            }
            break;
        case 5:
            if (lbl_1_bss_580.timer++ >= 40) {
                lbl_1_bss_580.timer = 0;
                lbl_1_bss_580.mode++;
            }
            break;
        case 6:
            fn_1_35AC();
            ret = 1;
            break;
    }
    return ret;
}
