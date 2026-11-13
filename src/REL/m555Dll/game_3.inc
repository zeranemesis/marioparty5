#include "game/hu3d.h"
#include "game/object.h"
#include "game/frand.h"
#include "math.h"

#define MDL(i) (lbl_1_bss_3030->mdl[(s16)(i)])

/* Per-player pop-up animation: step 0 starts it, step 1 shows the models
   and holds the frame until the timer expires. */
void fn_1_8928(s16 player)
{
    s16 stage;
    f32 time;

    switch (lbl_1_bss_120C[player].step) {
    case 0:
        if (lbl_1_bss_120C[player].subStep != 0) {
            lbl_1_bss_120C[player].timer2 = 0;
            lbl_1_bss_120C[player].step++;
            Hu3DMotionSpeedSet(MDL(player + 0xA5).mdlId, 1.0f);
            omVibrate(lbl_1_bss_3030->mdl0[player].playerNo, 20, 12, 0);
            stage = 5 - lbl_1_bss_120C[player].stage;
            if (stage == 4) {
                fn_1_2BD8(player + 6, 0x7F, (s32)(64.0f + 0.05818182f * MDL(player + 0xA9).pos.x));
            }
        }
        break;
    case 1:
        if (Hu3DModelAttrGet(MDL(player + 0xA9).mdlId) & 1) {
            Hu3DModelAttrReset(MDL(player + 0x21).mdlId, 1);
            Hu3DModelAttrReset(MDL(player + 0xA9).mdlId, 1);
        } else {
            Hu3DModelAttrSet(MDL(player + 0x21).mdlId, 1);
            Hu3DModelAttrSet(MDL(player + 0xA9).mdlId, 1);
        }
        time = Hu3DMotionTimeGet(MDL(player + 0xA5).mdlId);
        stage = 5 - lbl_1_bss_120C[player].stage;
        if (stage != 5) {
            if (time >= 14.9f * stage) {
                Hu3DMotionTimeSet(MDL(player + 0xA5).mdlId, 15.0f * stage);
                Hu3DMotionSpeedSet(MDL(player + 0xA5).mdlId, 0.0f);
            }
        }
        if (lbl_1_bss_120C[player].timer2++ >= 0x78) {
            Hu3DModelAttrReset(MDL(player + 0x21).mdlId, 1);
            Hu3DModelAttrReset(MDL(player + 0xA9).mdlId, 1);
            lbl_1_bss_120C[player].subStep = 0;
            lbl_1_bss_120C[player].step = 0;
        }
        break;
    }
    stage = 5 - lbl_1_bss_120C[player].stage;
    if (stage == 4) {
        if (lbl_1_bss_1894.motion <= 3) {
            fn_1_2CB0(player + 6, (s32)(64.0f + 0.05818182f * MDL(player + 0x21).pos.x));
        }
        if (lbl_1_bss_1894.motion == 4 && lbl_1_bss_120C[player].stage == 1) {
            fn_1_2E60(player + 6, 0x64);
        }
        if (lbl_1_bss_120C[player].timer3++ >= 2) {
            if (Hu3DModelAttrGet(MDL(player + 0xA5).mdlId) & 1) {
                Hu3DModelAttrReset(MDL(player + 0xA5).mdlId, 1);
            } else {
                Hu3DModelAttrSet(MDL(player + 0xA5).mdlId, 1);
            }
            lbl_1_bss_120C[player].timer3 = 0;
        }
    }
}

s32 fn_1_8F4C(s16 player)
{
    s32 result = 0;

    if (lbl_1_bss_1894.curPlayer != 0 || lbl_1_bss_1894.curPlayer2 != 0) {
        if (lbl_1_bss_120C[player].unk0E == 0) {
            memset(&lbl_1_bss_3030->mdl0[player].posX, 0, 0x14);
        }
    }
    fn_1_7D88(player);
    fn_1_8928(player);
    if (lbl_1_bss_120C[player].stage > 0) {
        if (lbl_1_bss_1894.motion <= 3 && lbl_1_bss_1894.resultF == 0) {
            fn_1_2CB0(player + 2, (s32)(64.0f + 0.05818182f * MDL(player + 0x21).pos.x));
        }
    }
    return result;
}

/* Spawns, fades, moves and retires the flying marker models (B88 table,
   models 0x23..0x54). */
void fn_1_90C4(void)
{
    s16 limit[6] = { 3, 3, 3, 3, 3, 3 };
    f32 pos[24] = {
        -550.0f, -217.0f, -336.0f, -20.0f,
        -550.0f, -217.0f,   20.0f, 336.0f,
        -177.0f,  177.0f, -336.0f, -20.0f,
        -177.0f,  177.0f,   20.0f, 336.0f,
         217.0f,  550.0f, -336.0f, -20.0f,
         217.0f,  550.0f,   20.0f, 336.0f,
    };
    s16 i, j;
    s16 lane, mdl, full;

    lbl_1_bss_11C8.timer++;
    switch (lbl_1_bss_11C8.state) {
    case 4:
        break;
    case 0:
        lbl_1_bss_11C8.live = 0;
        lbl_1_bss_11C8.timer2 = 0;
        lbl_1_bss_11C8.limit = 0x1E;
        lbl_1_bss_11C8.limit2 = 0x28;
        lbl_1_bss_11C8.idx = 0;
        lbl_1_bss_11C8.state++;
        break;
    case 1:
        if (lbl_1_bss_1894.curPlayer != 0) {
            lbl_1_bss_11C8.state = 4;
        } else if (lbl_1_bss_1894.frameCnt >= 0x708) {
            lbl_1_bss_11C8.state = 3;
        } else {
            if (lbl_1_bss_11C8.idx++ > lbl_1_bss_11C8.limit2) {
                if (lbl_1_bss_11C8.limit-- <= 2) {
                    lbl_1_bss_11C8.limit = 2;
                }
                lbl_1_bss_11C8.idx = 0;
            }
            if (lbl_1_bss_11C8.timer2++ > lbl_1_bss_11C8.limit) {
                for (i = 0; i < 0x32; i++) {
                    if (lbl_1_bss_B88[i].active == 0) {
                        mdl = i + 0x23;
                        lbl_1_bss_B88[i].active = 1;
                        lbl_1_bss_B88[i].type = 0;
                        lbl_1_bss_B88[i].fade = 1.0f;
                        lbl_1_bss_B88[i].stepX = 200.0f;
                        do {
                            lane = frandmod(6);
                        } while (lbl_1_bss_11C8.count[lane] >= limit[lane]);
                        lbl_1_bss_11C8.count[lane]++;
                        j = 0;
                        full = 0;
                        for (; j < 6; j++) {
                            if (lbl_1_bss_11C8.count[j] == limit[j]) {
                                full++;
                            }
                        }
                        if (full >= 6) {
                            for (j = 0; j < 6; j++) {
                                lbl_1_bss_11C8.count[j] = 0;
                            }
                        }
                        MDL(mdl).pos.x = pos[lane * 4] + (f32)frandmod(0x162);
                        MDL(mdl).pos.y = pos[lane * 4 + 2] + (f32)frandmod(0x13C);
                        MDL(mdl).pos.z = -37000.0f;
                        Hu3DModelAttrReset(MDL(mdl).mdlId, 1);
                        Hu3DModelAttrSet(MDL(mdl).mdlId, 0x40000001);
                        Hu3DMotionTimeSet(MDL(mdl).mdlId, 0.0f);
                        Hu3DMotionSpeedSet(MDL(mdl).mdlId, 1.0f);
                        Hu3DModelTPLvlSet(MDL(mdl).mdlId, lbl_1_bss_B88[i].fade);
                        lbl_1_bss_11C8.timer2 = 0;
                        lbl_1_bss_11C8.live++;
                        break;
                    }
                }
            }
        }
        break;
    case 2:
        i = 0;
        j = 0;
        for (; i < 0x32; i++) {
            if (lbl_1_bss_B88[i].active != 0) {
                j++;
                lbl_1_bss_B88[i].fade -= 0.05f;
                if (lbl_1_bss_B88[i].fade <= 0.0f) {
                    lbl_1_bss_B88[i].fade = 0.0f;
                    lbl_1_bss_B88[i].active = 0;
                    Hu3DModelAttrSet(MDL(i + 0x23).mdlId, 1);
                }
                Hu3DModelTPLvlSet(MDL(i + 0x23).mdlId, lbl_1_bss_B88[i].fade);
            }
        }
        if (j == 0) {
            lbl_1_bss_11C8.state = 4;
        }
        break;
    case 3:
        if (lbl_1_bss_1894.curPlayer != 0) {
            lbl_1_bss_11C8.state = 4;
        }
        break;
    }
    for (i = 0; i < 0x32; i++) {
        if (lbl_1_bss_B88[i].active != 0) {
            mdl = i + 0x23;
            MDL(mdl).pos.z += lbl_1_bss_B88[i].stepX;
            MDL(mdl).rot.x += lbl_1_bss_B88[i].stepY;
            MDL(mdl).rot.y += lbl_1_bss_B88[i].stepZ;
            switch (lbl_1_bss_B88[i].type) {
            case 0:
                lbl_1_bss_B88[i].type++;
                break;
            case 1:
                if (MDL(mdl).pos.z >= 1500.0f) {
                    if (lbl_1_bss_1894.curPlayer != 0 || lbl_1_bss_1894.curPlayer2 != 0 || lbl_1_bss_B74[9] != 0) {
                        lbl_1_bss_B88[i].type++;
                    } else {
                        lbl_1_bss_B88[i].active = 0;
                        Hu3DModelAttrSet(MDL(mdl).mdlId, 1);
                        lbl_1_bss_11C8.live--;
                    }
                } else if (MDL(mdl).pos.z >= 100.0f) {
                    lbl_1_bss_B88[i].fade = 0.3f;
                    Hu3DModelTPLvlSet(MDL(mdl).mdlId, lbl_1_bss_B88[i].fade);
                }
                break;
            case 2:
                if (MDL(mdl).pos.z >= 1500.0f) {
                    lbl_1_bss_B88[i].active = 0;
                    Hu3DModelAttrSet(MDL(mdl).mdlId, 1);
                    lbl_1_bss_11C8.live--;
                }
                break;
            }
        }
    }
}

/* Drives the two rows of scrolling background models (0x55..0x57 and
   0x58..0x5D). */
void fn_1_9B8C(void)
{
    f32 speed;
    s16 i, j;

    if (lbl_1_bss_1894.unk26 == 0 && lbl_1_bss_1894.motion >= 2) {
        Hu3DMotionSpeedSet(lbl_1_bss_3030->mdl[1].mdlId, 4.0f);
        lbl_1_bss_1894.unk26 = 1;
    }
    if (lbl_1_bss_1894.motion >= 2) {
        speed = 300.0f;
    } else {
        speed = 100.0f;
    }
    i = 0x55;
    j = 0;
    for (; i <= 0x57; i++, j++) {
        switch (lbl_1_bss_B74[j]) {
        case 4:
            break;
        case 0:
            MDL(i).pos.x = 0.0f;
            MDL(i).pos.y = 0.0f;
            MDL(i).pos.z = -36000.0f + 12000.0f * j;
            Hu3DModelAttrReset(MDL(i).mdlId, 1);
            Hu3DModelAttrSet(MDL(i).mdlId, 0x40000001);
            Hu3DMotionTimeSet(MDL(i).mdlId, 50.0f);
            Hu3DMotionSpeedSet(MDL(i).mdlId, 1.5f);
            lbl_1_bss_B74[j] = 2;
            break;
        case 1:
            MDL(i).pos.z += speed;
            if (MDL(i).pos.z >= -36000.0f) {
                Hu3DMotionSpeedSet(MDL(i).mdlId, 1.5f);
            }
            if (Hu3DMotionTimeGet(MDL(i).mdlId) >= 50.0f) {
                lbl_1_bss_B74[j]++;
            }
            break;
        case 2:
            MDL(i).pos.z += speed;
            if (MDL(i).pos.z >= 1500.0f) {
                MDL(i).pos.z = -36000.0f;
                Hu3DMotionTimeSet(MDL(i).mdlId, 0.0f);
                lbl_1_bss_B74[j] = 1;
                if (lbl_1_bss_1894.curPlayer != 0) {
                    Hu3DMotionTimeSet(MDL(i).mdlId, 50.0f);
                    MDL(i).pos.z = 1500.0f;
                    lbl_1_bss_B74[j] = 3;
                } else if (lbl_1_bss_1894.frameCnt >= 0x6E0) {
                    Hu3DMotionTimeSet(MDL(i).mdlId, 50.0f);
                    lbl_1_bss_B74[9] = 1;
                    MDL(i).pos.z = 1500.0f;
                    lbl_1_bss_B74[j] = 3;
                }
            }
            if (Hu3DMotionTimeGet(MDL(i).mdlId) >= 96.0f) {
                Hu3DMotionTimeSet(MDL(i).mdlId, 50.0f);
            }
            break;
        case 3:
            if (Hu3DMotionTimeGet(MDL(i).mdlId) >= 96.0f) {
                Hu3DMotionTimeSet(MDL(i).mdlId, 50.0f);
            }
            MDL(i).pos.z += speed;
            if (MDL(i).pos.z >= 1500.0f) {
                Hu3DModelAttrSet(MDL(i).mdlId, 1);
                lbl_1_bss_B74[j]++;
            }
            break;
        }
    }
    if (lbl_1_bss_B74[9] != 0) {
        i = 0x58;
        j = 0;
        for (; i <= 0x5D; i++, j++) {
            switch (lbl_1_bss_B74[j + 3]) {
            case 4:
                break;
            case 0:
                MDL(i).pos.x = 0.0f;
                MDL(i).pos.y = 0.0f;
                MDL(i).pos.z = -36000.0f - 5000.0f * j;
                Hu3DModelAttrReset(MDL(i).mdlId, 1);
                Hu3DModelAttrSet(MDL(i).mdlId, 0x40000001);
                Hu3DMotionTimeSet(MDL(i).mdlId, 0.0f);
                Hu3DMotionSpeedSet(MDL(i).mdlId, 0.0f);
                lbl_1_bss_B74[j + 3]++;
                break;
            case 1:
                MDL(i).pos.z += 300.0f;
                if (MDL(i).pos.z >= -36000.0f) {
                    Hu3DMotionSpeedSet(MDL(i).mdlId, 1.5f);
                }
                if (Hu3DMotionTimeGet(MDL(i).mdlId) >= 50.0f) {
                    lbl_1_bss_B74[j + 3]++;
                }
                break;
            case 2:
                MDL(i).pos.z += 300.0f;
                if (MDL(i).pos.z >= 1500.0f) {
                    lbl_1_bss_B74[j + 3]++;
                    if (i == 0x5D) {
                        lbl_1_bss_1894.curPlayer2 = 1;
                    }
                }
                if (Hu3DMotionTimeGet(MDL(i).mdlId) >= 96.0f) {
                    Hu3DMotionTimeSet(MDL(i).mdlId, 50.0f);
                }
                break;
            case 3:
                if (Hu3DMotionTimeGet(MDL(i).mdlId) >= 96.0f) {
                    Hu3DMotionTimeSet(MDL(i).mdlId, 50.0f);
                }
                MDL(i).pos.z += 300.0f;
                if (MDL(i).pos.z >= 37000.0f) {
                    Hu3DModelAttrSet(MDL(i).mdlId, 1);
                    Hu3DMotionTimeSet(MDL(i).mdlId, 0.0f);
                    Hu3DMotionSpeedSet(MDL(i).mdlId, 0.0f);
                }
                break;
            }
        }
    }
}

s32 fn_1_FB2C(void)
{
    HuVecF *p = &lbl_1_bss_3030->mdl[95].pos;
    s32 result = 0;

    switch (lbl_1_bss_11E4.mode) {
    case 0:
        lbl_1_bss_11E4.camX = 2.5f;
        lbl_1_bss_11E4.camY = -4.0f;
        lbl_1_bss_11E4.stepX = -0.15f;
        lbl_1_bss_11E4.stepY = 0.1f;
        Hu3DMotionSpeedSet(MDL(0x21).mdlId, 3.0f);
        Hu3DMotionSpeedSet(MDL(0x22).mdlId, 3.0f);
        lbl_1_bss_11E4.mode++;
        /* fall through */
    case 1:
        p->x += lbl_1_bss_11E4.camX;
        p->y += lbl_1_bss_11E4.camY;
        lbl_1_bss_11E4.camX += lbl_1_bss_11E4.stepX;
        lbl_1_bss_11E4.camY += lbl_1_bss_11E4.stepY;
        if (p->x < -1000.0f) {
            Hu3DModelHookReset(MDL(95).mdlId);
            Hu3DModelAttrSet(MDL(94).mdlId, 1);
            Hu3DModelAttrSet(MDL(95).mdlId, 1);
            lbl_1_bss_11E4.mode++;
        }
        break;
    case 2:
        result = 1;
        break;
    }
    return result;
}

s16 fn_1_FD64(void)
{
    HuVecF *p = &lbl_1_bss_3030->mdl[95].pos;
    s32 result = 0;
    s32 sum = 0;
    s16 i;
    f32 f;

    p->y = 20.0 * sin(M_PI * lbl_1_bss_11E4.spinAngle / 180.0);
    lbl_1_bss_11E4.spinAngle += 2.5f;
    switch (lbl_1_bss_11E4.mode) {
    case 0:
        MDL(0x21).pos.x = -200.0f;
        MDL(0x21).pos.y = -100.0f;
        MDL(0x21).pos.z = 1400.0f;
        MDL(0x22).pos.x = 200.0f;
        MDL(0x22).pos.y = -100.0f;
        MDL(0x22).pos.z = 1400.0f;
        lbl_1_bss_11E4.mode++;
        /* fall through */
    case 1:
        if (lbl_1_bss_11E4.timer++ > 0x78) {
            lbl_1_bss_11E4.timer = 0;
            lbl_1_bss_11E4.mode++;
        }
        break;
    case 2:
        f = 90.0f;
        sum += fn_1_1898(&MDL(0x21).pos.z, f, 1400.0f, 0.0f);
        sum += fn_1_1898(&MDL(0x22).pos.z, f, 1400.0f, 0.0f);
        if ((s16)sum == 0) {
            Hu3DMotionSpeedSet(MDL(94).mdlId, 1.0f);
            Hu3DModelAttrReset(MDL(94).mdlId, 0x40000001);
            for (i = 0; i < 0x64; i++) {
                if (lbl_1_bss_214[i].active == 0) {
                    lbl_1_bss_214[i].active = 1;
                    lbl_1_bss_214[i].type = 0;
                    lbl_1_bss_214[i].kind = 0;
                    lbl_1_bss_214[i].mdlId = 0;
                    break;
                }
            }
            for (i = 0; i < 0x64; i++) {
                if (lbl_1_bss_214[i].active == 0) {
                    lbl_1_bss_214[i].active = 1;
                    lbl_1_bss_214[i].type = 0;
                    lbl_1_bss_214[i].kind = 1;
                    lbl_1_bss_214[i].mdlId = 0;
                    break;
                }
            }
            omVibrate(lbl_1_bss_3030->mdl0[0].playerNo, 0x8C, 6, 6);
            omVibrate(lbl_1_bss_3030->mdl0[1].playerNo, 0x8C, 6, 6);
            fn_1_2BD8(2, 0x7F, (s32)(64.0f + 0.05818182f * MDL(0x21).pos.x));
            fn_1_2BD8(3, 0x7F, (s32)(64.0f + 0.05818182f * MDL(0x22).pos.x));
            lbl_1_bss_11E4.timer = 0;
            lbl_1_bss_11E4.mode++;
        }
        break;
    case 3:
        switch (lbl_1_bss_11E4.timer) {
        case 1:
        case 0x3C:
            fn_1_2B10(0);
            break;
        case 0x78:
            fn_1_2B10(1);
            break;
        }
        if (lbl_1_bss_11E4.timer++ > 0x8C) {
            fn_1_2D40(2, 0x1F40);
            fn_1_2D40(3, 0x1F40);
            lbl_1_bss_11E4.timer = 0;
            result = 1;
        }
        break;
    }
    MDL(0xA9).pos = MDL(0x21).pos;
    MDL(0xAA).pos = MDL(0x22).pos;
    return result;
}
