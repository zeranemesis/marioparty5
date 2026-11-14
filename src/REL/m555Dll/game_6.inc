#include "game/audio.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "string.h"

#define M555_ABS(x) (((x) > 0.0f) ? (x) : -1.0f * (x))

/* zero until the pad-check routine has placed both players once */
s32 lbl_1_data_33C[2] = { 0, 0 };

/* Drops the two players off the bottom of the stage: wobble while they fall,
   then kill them once they are far enough below. */
s32 fn_1_103D8(s16 player)
{
    HuVecF *pos = &lbl_1_bss_3030->mdl[player + 0x21].pos;
    f32 *rot = &lbl_1_bss_3030->mdl[player + 0x21].rot.x;
    s32 ret = 1;
    f32 wob[8][2] = {
        { 2.0f, 0.0f }, { 2.0f, 0.0f }, { -2.0f, 0.0f }, { -2.0f, 0.0f },
        { 0.0f, 2.0f }, { 0.0f, 2.0f }, { 0.0f, -2.0f }, { 0.0f, -2.0f },
    };
    HuVecF v;
    s16 i;

    switch (lbl_1_bss_1894.phase2) {
        case 0:
            lbl_1_bss_120C[player].velX = -0.012f;
            lbl_1_bss_120C[player].velY = 0.015f;
            lbl_1_bss_120C[player].charNo = 0; /* frame counter here */
            omVibrate(lbl_1_bss_3030->mdl0[player].playerNo, 2000, 6, 6);
            fn_1_2B10(8);
            fn_1_2D40(player + 2, 0);
            CharFXPlayVolPan(lbl_1_bss_3030->mdl0[player].charNo, 0x1F9, 0x7F,
                             (s32)(64.0f + 0.05818182f * lbl_1_bss_3030->mdl[player + 0xA9].pos.x));
            lbl_1_bss_1894.phase2++;
            /* fallthrough */
        case 1:
            lbl_1_bss_120C[player].accX += lbl_1_bss_120C[player].velX;
            lbl_1_bss_120C[player].accY += lbl_1_bss_120C[player].velY;
            pos->y += lbl_1_bss_120C[player].accX;
            pos->z += lbl_1_bss_120C[player].accY;
            if (lbl_1_bss_120C[player].charNo++ > 30) {
                for (i = 0; i < 100; i++) {
                    if (lbl_1_bss_214[i].active != 0 && lbl_1_bss_214[i].kind == 0 && player == lbl_1_bss_214[i].slot) {
                        lbl_1_bss_214[i].type = 4;
                    }
                    if (lbl_1_bss_214[i].active != 0 && lbl_1_bss_214[i].kind == 3 && lbl_1_bss_214[i].type == 1) {
                        lbl_1_bss_214[i].slot = player + 1;
                    }
                    if (lbl_1_bss_214[i].active != 0 && lbl_1_bss_214[i].kind == 4 && player == lbl_1_bss_214[i].slot) {
                        Hu3DModelAttrSet(lbl_1_bss_3030->mdl[lbl_1_bss_214[i].mdlId].mdlId, HU3D_ATTR_DISPOFF);
                        lbl_1_bss_214[i].type = 3;
                    }
                }
                lbl_1_bss_120C[player].charNo = 0;
                lbl_1_bss_1894.phase2 = 4;
            }
            break;
        case 4:
            lbl_1_bss_120C[player].accX += lbl_1_bss_120C[player].velX;
            lbl_1_bss_120C[player].accY += lbl_1_bss_120C[player].velY;
            pos->y += lbl_1_bss_120C[player].accX;
            pos->z += lbl_1_bss_120C[player].accY;
            if (lbl_1_bss_120C[player].charNo++ >= 0x30) {
                lbl_1_bss_120C[player].velX = -0.08f;
                lbl_1_bss_120C[player].velY = 0.08f;
                lbl_1_bss_120C[player].charNo = 0;
                lbl_1_bss_1894.phase2++;
            }
            break;
        case 5:
            lbl_1_bss_120C[player].accX += lbl_1_bss_120C[player].velX;
            lbl_1_bss_120C[player].accY += lbl_1_bss_120C[player].velY;
            pos->y += lbl_1_bss_120C[player].accX;
            pos->z += lbl_1_bss_120C[player].accY;
            if (pos->y <= -1000.0f) {
                omVibrate(lbl_1_bss_3030->mdl0[player].playerNo, 1, 6, 6);
                fn_1_2E60(player + 2, 1000);
                lbl_1_bss_1894.phase2++;
            }
            break;
        case 6:
            lbl_1_bss_120C[player].accX += lbl_1_bss_120C[player].velX;
            lbl_1_bss_120C[player].accY += lbl_1_bss_120C[player].velY;
            pos->y += lbl_1_bss_120C[player].accX;
            pos->z += lbl_1_bss_120C[player].accY;
            if (pos->y <= -6000.0f) {
                lbl_1_bss_1894.phase2++;
            }
            break;
        case 7:
            ret = 0;
            break;
    }
    v = *pos;
    v.y -= 20.5f;
    v.z += 112.0f;
    fn_1_25D8(&v);
    lbl_1_bss_3030->mdl[player + 0x21].pos.y += wob[lbl_1_bss_1894.unk178][0];
    lbl_1_bss_3030->mdl[player + 0x21].pos.y += wob[lbl_1_bss_1894.unk178][1];
    if (++lbl_1_bss_1894.unk178 >= 8) {
        lbl_1_bss_1894.unk178 = 0;
    }
    lbl_1_bss_3030->mdl[player + 0xA9].pos = lbl_1_bss_3030->mdl[player + 0x21].pos;
    return ret;
}

/* Places both players once, tracks idle time and checks whether both have
   reached their start spot; returns 1 when both are ready. */
s32 fn_1_10E20(void)
{
    HuVecF v;
    s32 ret = 0;
    s16 cnt = 0;
    s16 i;
    s16 r;

    for (i = 0; i < 2; i++) {
        if (lbl_1_data_33C[i] == 0) {
            lbl_1_bss_120C[i].targetPos.x = -200.0f + 400.0f * i;
            lbl_1_bss_120C[i].targetPos.y = 0.0f;
            lbl_1_bss_120C[i].flag0 = 0;
            lbl_1_bss_120C[i].flag1 = 0;
            lbl_1_bss_120C[i].camX0 = 5.0f;
            lbl_1_bss_120C[i].camY0 = 56.0f;
            lbl_1_bss_120C[i].flag2 = 0;
            lbl_1_bss_120C[i].flag3 = 0;
            lbl_1_bss_120C[i].unk0E = 1;
            lbl_1_bss_1894.itemIdx = 0;
            lbl_1_data_33C[i] = 1;
        }
        memset(&lbl_1_bss_3030->mdl0[i].btn, 0, 0x14);
        lbl_1_bss_120C[i].pad11[0] = 0;
        lbl_1_bss_120C[i].pad11[1] = 0;
        lbl_1_bss_120C[i].wheelIdx0 = 0;
        lbl_1_bss_120C[i].wheelIdx1 = 0;
        if (lbl_1_bss_120C[i].joinNo != 0) {
        } else {
            r = fn_1_16958(i);
            if (r != 0) {
                cnt++;
            }
        }
        memcpy(&lbl_1_bss_3030->mdl0[i].btn, &lbl_1_bss_120C[i].pad11[0], 4);
        memcpy(&lbl_1_bss_3030->mdl0[i].btnDown, &lbl_1_bss_120C[i].pad11[1], 4);
        lbl_1_bss_3030->mdl0[i].stickX = lbl_1_bss_120C[i].wheelIdx0;
        lbl_1_bss_3030->mdl0[i].stickY = lbl_1_bss_120C[i].wheelIdx1;
        if (lbl_1_bss_120C[i].joinNo == 0 && lbl_1_bss_3030->mdl[i + 0xA9].pos.x == lbl_1_bss_120C[i].prevPos.x
            && lbl_1_bss_3030->mdl[i + 0xA9].pos.y == lbl_1_bss_120C[i].prevPos.y) {
            lbl_1_bss_120C[i].idleCnt++;
        } else {
            lbl_1_bss_120C[i].idleCnt = 0;
        }
        if (i == 0) {
            if (lbl_1_bss_120C[i].idleCnt >= 60 && lbl_1_bss_1894.count != 0) {
                lbl_1_bss_120C[i].targetPos.x += 50.0f;
                lbl_1_bss_120C[i].targetPos.y += 50.0f;
            } else {
                lbl_1_bss_120C[i].targetPos.x = -200.0f;
                lbl_1_bss_120C[i].targetPos.y = 0.0f;
            }
        }
        lbl_1_bss_120C[i].prevPos = lbl_1_bss_3030->mdl[i + 0xA9].pos;
    }
    v.x = M555_ABS(-200.0f - lbl_1_bss_3030->mdl[0x21].pos.x);
    v.y = M555_ABS(-lbl_1_bss_3030->mdl[0x21].pos.y);
    v.z = sqrt(v.x * v.x + v.y * v.y);
    if (v.z <= 30.0f) {
        v.x = M555_ABS(200.0f - lbl_1_bss_3030->mdl[0x22].pos.x);
        v.y = M555_ABS(-lbl_1_bss_3030->mdl[0x22].pos.y);
        v.z = sqrt(v.x * v.x + v.y * v.y);
        if (v.z <= 30.0f) {
            cnt = 2;
        }
    }
    if (cnt == 2) {
        lbl_1_bss_120C[0].unk0E = 0;
        lbl_1_bss_120C[1].unk0E = 0;
        lbl_1_bss_1894.itemIdx = 1;
        ret = 1;
    } else {
        lbl_1_bss_120C[0].unk0E = 1;
        lbl_1_bss_120C[1].unk0E = 1;
        lbl_1_bss_1894.itemIdx = 0;
    }
    return ret;
}

/* Both players slide down off screen, then the state machine finishes. */
s32 fn_1_119EC(void)
{
    HuVecF *pos;
    s32 ret = 0;
    s16 i;

    switch (lbl_1_bss_1894.phase2) {
        case 0:
            for (i = 0; i < 2; i++) {
                lbl_1_bss_120C[i].velX = -0.07f;
                lbl_1_bss_120C[i].velY = 0.05f;
            }
            lbl_1_bss_1894.phase2++;
            /* fallthrough */
        case 1:
            for (i = 0; i < 2; i++) {
                pos = &lbl_1_bss_3030->mdl[i + 0x21].pos;
                lbl_1_bss_120C[i].accX += lbl_1_bss_120C[i].velX;
                lbl_1_bss_120C[i].accY += lbl_1_bss_120C[i].velY;
                pos->y += lbl_1_bss_120C[i].accX;
                pos->z += lbl_1_bss_120C[i].accY;
                lbl_1_bss_3030->mdl[i + 0xA9].pos = lbl_1_bss_3030->mdl[i + 0x21].pos;
            }
            if (pos->y <= -1000.0f) {
                fn_1_2E60(2, 1000);
                fn_1_2E60(3, 1000);
                lbl_1_bss_1894.phase2++;
            }
            break;
        case 2:
            for (i = 0; i < 2; i++) {
                pos = &lbl_1_bss_3030->mdl[i + 0x21].pos;
                lbl_1_bss_120C[i].accX += lbl_1_bss_120C[i].velX;
                lbl_1_bss_120C[i].accY += lbl_1_bss_120C[i].velY;
                pos->y += lbl_1_bss_120C[i].accX;
                pos->z += lbl_1_bss_120C[i].accY;
                lbl_1_bss_3030->mdl[i + 0xA9].pos = lbl_1_bss_3030->mdl[i + 0x21].pos;
            }
            if (pos->y <= -6000.0f) {
                lbl_1_bss_1894.phase2++;
            }
            break;
        case 3:
            ret = 1;
            break;
    }
    return ret;
}

/* Spawns and animates the four decoration models (state per work item). */
void fn_1_11DFC(s16 player)
{
    HuVecF base;
    s16 sp8 = player;
    s16 spA = 0;
    s16 spC = 0x1C;
    s16 j;
    s16 i;

    base = lbl_1_bss_3030->mdl[player + 0xA9].pos;
    for (j = 0x60, i = 0; j <= 0x63; j++, i++) {
        switch (lbl_1_bss_1894.item[i].state) {
            case 0:
                if (lbl_1_bss_1894.item[i].timer++ >= 30.0f) {
                    lbl_1_bss_1894.item[i].timer = 0;
                    lbl_1_bss_1894.item[i].state++;
                }
                break;
            case 1:
                switch (i) {
                    case 0:
                        lbl_1_bss_1894.item[i].angle = 0.0f;
                        Hu3DMotionTimeSet(lbl_1_bss_3030->mdl[j].mdlId, 60.0f);
                        lbl_1_bss_3030->mdl[j].pos.x = -900.0f;
                        lbl_1_bss_3030->mdl[j].pos.y = 0.0f;
                        lbl_1_bss_3030->mdl[j].pos.z = 400.0f;
                        lbl_1_bss_3030->mdl[j].rot.x = 15.0f;
                        lbl_1_bss_3030->mdl[j].rot.y = 135.0f;
                        lbl_1_bss_3030->mdl[j].rot.z = 0.0f;
                        lbl_1_bss_1894.item[i].posX = 2.0f;
                        lbl_1_bss_1894.item[i].posY = 0.5f;
                        lbl_1_bss_1894.item[i].posZ = 0.0f;
                        lbl_1_bss_1894.item[i].stepX = 0.0f;
                        lbl_1_bss_1894.item[i].stepY = 0.0f;
                        lbl_1_bss_1894.item[i].stepZ = 0.0f;
                        break;
                    case 1:
                        lbl_1_bss_1894.item[i].angle = 135.0f;
                        Hu3DMotionTimeSet(lbl_1_bss_3030->mdl[j].mdlId, 60.0f);
                        lbl_1_bss_3030->mdl[j].pos.x = -400.0f;
                        lbl_1_bss_3030->mdl[j].pos.y = -750.0f;
                        lbl_1_bss_3030->mdl[j].pos.z = 400.0f;
                        lbl_1_bss_3030->mdl[j].rot.x = 0.0f;
                        lbl_1_bss_3030->mdl[j].rot.y = 135.0f;
                        lbl_1_bss_3030->mdl[j].rot.z = 0.0f;
                        lbl_1_bss_1894.item[i].posX = 0.5f;
                        lbl_1_bss_1894.item[i].posY = 2.0f;
                        lbl_1_bss_1894.item[i].posZ = 0.0f;
                        lbl_1_bss_1894.item[i].stepX = 0.0f;
                        lbl_1_bss_1894.item[i].stepY = 0.0f;
                        lbl_1_bss_1894.item[i].stepZ = 0.0f;
                        break;
                    case 2:
                        lbl_1_bss_1894.item[i].angle = 45.0f;
                        Hu3DMotionTimeSet(lbl_1_bss_3030->mdl[j].mdlId, 50.0f);
                        Hu3DMotionSpeedSet(lbl_1_bss_3030->mdl[j].mdlId, 1.5f);
                        lbl_1_bss_3030->mdl[j].pos.x = 800.0f;
                        lbl_1_bss_3030->mdl[j].pos.y = 0.0f;
                        lbl_1_bss_3030->mdl[j].pos.z = 400.0f;
                        lbl_1_bss_3030->mdl[j].rot.x = 0.0f;
                        lbl_1_bss_3030->mdl[j].rot.y = 225.0f;
                        lbl_1_bss_3030->mdl[j].rot.z = 0.0f;
                        lbl_1_bss_1894.item[i].posX = -2.0f;
                        lbl_1_bss_1894.item[i].posY = 0.5f;
                        lbl_1_bss_1894.item[i].posZ = 0.0f;
                        lbl_1_bss_1894.item[i].stepX = 0.0f;
                        lbl_1_bss_1894.item[i].stepY = 0.0f;
                        lbl_1_bss_1894.item[i].stepZ = 0.0f;
                        break;
                    case 3:
                        lbl_1_bss_1894.item[i].angle = 180.0f;
                        Hu3DMotionTimeSet(lbl_1_bss_3030->mdl[j].mdlId, 50.0f);
                        Hu3DMotionSpeedSet(lbl_1_bss_3030->mdl[j].mdlId, 1.5f);
                        lbl_1_bss_3030->mdl[j].pos.x = 900.0f;
                        lbl_1_bss_3030->mdl[j].pos.y = -150.0f;
                        lbl_1_bss_3030->mdl[j].pos.z = 400.0f;
                        lbl_1_bss_3030->mdl[j].rot.x = 0.0f;
                        lbl_1_bss_3030->mdl[j].rot.y = 225.0f;
                        lbl_1_bss_3030->mdl[j].rot.z = 0.0f;
                        lbl_1_bss_1894.item[i].posX = -2.0f;
                        lbl_1_bss_1894.item[i].posY = -0.5f;
                        lbl_1_bss_1894.item[i].posZ = 0.0f;
                        lbl_1_bss_1894.item[i].stepX = 0.0f;
                        lbl_1_bss_1894.item[i].stepY = 0.0f;
                        lbl_1_bss_1894.item[i].stepZ = 0.0f;
                        break;
                }
                lbl_1_bss_3030->mdl[j].pos.x += base.x;
                lbl_1_bss_3030->mdl[j].pos.y += base.y;
                lbl_1_bss_1894.item[i].value = lbl_1_bss_3030->mdl[j].pos.y;
                Hu3DModelAttrReset(lbl_1_bss_3030->mdl[j].mdlId, HU3D_ATTR_DISPOFF);
                Hu3DModelAttrSet(lbl_1_bss_3030->mdl[j].mdlId, HU3D_MOTATTR_LOOP);
                Hu3DMotionSpeedSet(lbl_1_bss_3030->mdl[j].mdlId, 1.0f);
                lbl_1_bss_1894.item[i].state++;
                break;
            case 2:
                if (i < 4) {
                    if (i == 0) {
                    }
                }
                lbl_1_bss_1894.item[i].angle += 3.0f;
                lbl_1_bss_3030->mdl[j].pos.x += lbl_1_bss_1894.item[i].posX;
                lbl_1_bss_1894.item[i].value += lbl_1_bss_1894.item[i].posY;
                lbl_1_bss_3030->mdl[j].pos.y = lbl_1_bss_1894.item[i].value + 50.0 * HuSin(lbl_1_bss_1894.item[i].angle);
                if (lbl_1_bss_1894.item[i].timer++ >= 240.0f) {
                    lbl_1_bss_1894.item[i].state++;
                }
                break;
            case 3:
                if (i < 4) {
                    if (i == 0) {
                    }
                }
                lbl_1_bss_1894.item[i].angle += 3.0f;
                lbl_1_bss_3030->mdl[j].pos.y = lbl_1_bss_1894.item[i].value + 50.0 * HuSin(lbl_1_bss_1894.item[i].angle);
                break;
        }
    }
}
