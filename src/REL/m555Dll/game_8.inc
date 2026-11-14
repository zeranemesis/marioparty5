#include "math.h"
#include "string.h"
#include "game/hu3d.h"
#include "game/frand.h"

#define P555(p) lbl_1_bss_120C[p]
#define M555_ABS(x) ((x) > 0.0f ? (x) : -1.0f * (x))

/* Pick the per-character camera/AI parameters from the model table. */
void fn_1_152D8(s16 player)
{
    f32 tblX[4] = { 10.0f, 25.0f, 40.0f, 56.0f };
    f32 tblY[4] = { 0.0f, 20.0f, 40.0f, 56.0f };
    f32 tblZ[4] = { -30000.0f, -30000.0f, -30000.0f, -30000.0f };
    f32 tblA[4] = { 25.0f, 25.0f, 2.0f, 1.0f };
    f32 tblB[4] = { 30.0f, 20.0f, 0.0f, 0.0f };
    f32 tblC[4] = { 30.0f, 5.0f, 0.0f, 0.0f };

    if (lbl_1_bss_3030->mdl0[player].charSlot != 0) {
        P555(player).wheel = 1;
        P555(player).camMode = lbl_1_bss_3030->mdl0[player].charSlot - 1;
        P555(player).camX0 = tblX[P555(player).camMode];
        P555(player).camY0 = tblY[P555(player).camMode];
        P555(player).camZ0 = tblZ[P555(player).camMode];
        P555(player).camIdx0 = tblA[P555(player).camMode];
        P555(player).camIdx1 = tblB[P555(player).camMode];
        P555(player).camIdx2 = tblC[P555(player).camMode];
    }
}

s32 fn_1_155DC(s16 player)
{
    HuVecF pos;
    s32 ret = 0;

    P555(player).pad11[0] = 0;
    P555(player).pad11[1] = 0;
    P555(player).wheelIdx0 = 0;
    P555(player).wheelIdx1 = 0;
    switch (P555(player).wheelState) {
        case 0:
            P555(player).wheelState++;
        case 1:
            pos = lbl_1_bss_3030->mdl[(s16)(player + 0x21)].pos;
            P555(player).anim2 = fn_1_17400(&pos, 130.0f, -10000.0f);
            if (fn_1_1A880(player, P555(player).anim2) != 0) {
                if (frandmod(100) < P555(player).camIdx1) {
                    P555(player).anim = 0;
                    P555(player).wheelState = 4;
                } else {
                    fn_1_176C4(player);
                    P555(player).wheelState = 2;
                }
            } else {
                if (P555(player).camMode == 3 && lbl_1_bss_1894.frameCnt > 600) {
                    P555(player).camIdx0 = 0;
                }
                if (frandmod(100) < P555(player).camIdx0) {
                    if (fn_1_1A4B0(player) != 0) {
                        P555(player).flag2 = 1;
                        P555(player).flag3 = 1;
                        P555(player).wheelState = 3;
                    }
                } else if (frandmod(100) < P555(player).camIdx2 && fn_1_1A6F4(player) != 0) {
                    P555(player).wheelState = 5;
                }
            }
            break;
        case 2:
            if (fn_1_171C8(player, 50.0f) != 0) {
                pos = lbl_1_bss_3030->mdl[(s16)(player + 0x21)].pos;
                if (fn_1_17400(&pos, 230.0f, -10000.0f) == 0) {
                    P555(player).flag0 = 1;
                    P555(player).flag1 = 1;
                } else if (fn_1_19C0C(player) != 0) {
                    P555(player).flag0 = 1;
                    P555(player).flag1 = 1;
                }
            }
            if (fn_1_16104(player) != 0) {
                P555(player).wheelState = 1;
            }
            break;
        case 3:
            if (fn_1_16104(player) != 0) {
                P555(player).flag2 = 0;
                P555(player).flag3 = 0;
                P555(player).wheelState = 1;
            }
            break;
        case 4:
            if (P555(player).anim++ > 60) {
                P555(player).anim = 0;
                P555(player).wheelState = 1;
            }
            break;
        case 5:
            if (fn_1_16104(player) != 0) {
                P555(player).wheelState = 6;
            }
            break;
        case 6:
            if (P555(player).anim++ > 60) {
                P555(player).anim = 0;
                P555(player).wheelState = 1;
            }
            break;
    }
    memcpy(&lbl_1_bss_3030->mdl0[player].posX, &P555(player).pad11[0], 4);
    memcpy(&lbl_1_bss_3030->mdl0[player].posY, &P555(player).pad11[1], 4);
    lbl_1_bss_3030->mdl0[player].stickX = (s8)P555(player).wheelIdx0;
    lbl_1_bss_3030->mdl0[player].stickY = (s8)P555(player).wheelIdx1;
    return ret;
}

/* Walk toward aimPos; returns 1 when both axes are done. */
s32 fn_1_15CF4(s16 player)
{
    f32 dx, dy, adx, ady;
    s32 done = 0;
    s16 n = 0;

    dx = P555(player).aimPos.x - lbl_1_bss_3030->mdl[(s16)(player + 0xA9)].pos.x;
    dy = P555(player).aimPos.y - lbl_1_bss_3030->mdl[(s16)(player + 0xA9)].pos.y;
    adx = M555_ABS(dx);
    ady = M555_ABS(dy);
    switch (P555(player).tallyA) {
        case 0:
            if (ady >= 20.0f) {
                P555(player).wheelIdx1 = (s32)((P555(player).hookX != 0 ? -1.0f : 1.0f) * ((dy > 0.0f ? 1.0f : -1.0f) * P555(player).camX0));
            } else {
                P555(player).tallyA++;
            }
            break;
        case 1:
            n = 1;
            break;
    }
    switch (P555(player).tallyB) {
        case 0:
            if (adx >= 20.0f) {
                P555(player).wheelIdx0 = (s32)((dx > 0.0f ? 1.0f : -1.0f) * P555(player).camX0);
            } else {
                P555(player).tallyB++;
            }
            break;
        case 1:
            n++;
            break;
    }
    if (n >= 2) {
        P555(player).tallyA = 0;
        P555(player).tallyB = 0;
        done = 1;
    }
    return done;
}

/* Walk toward targetPos; returns 1 when finished (or timed out). */
s32 fn_1_16104(s16 player)
{
    f32 dx, dy, adx, ady;
    s32 done = 0;
    s16 n = 0;

    dx = P555(player).targetPos.x - lbl_1_bss_3030->mdl[(s16)(player + 0xA9)].pos.x;
    dy = P555(player).targetPos.y - lbl_1_bss_3030->mdl[(s16)(player + 0xA9)].pos.y;
    adx = M555_ABS(dx);
    ady = M555_ABS(dy);
    switch (P555(player).flag0) {
        case 0:
            if (ady >= 20.0f) {
                P555(player).wheelIdx1 = (s32)((P555(player).hookX != 0 ? -1.0f : 1.0f) * ((dy > 0.0f ? 1.0f : -1.0f) * P555(player).camX0));
            } else {
                P555(player).flag0++;
            }
            break;
        case 1:
            if ((P555(player).stickY > 0.0f || -1.0f * P555(player).stickY >= 6.0f) && P555(player).flag3 == 0) {
                P555(player).wheelIdx1 = (s32)((P555(player).hookX != 0 ? -1.0f : 1.0f) * ((P555(player).stickY > 0.0f ? -1.0f : 1.0f) * P555(player).camY0));
            } else {
                P555(player).flag0++;
                n = 1;
            }
            break;
        case 2:
            n = 1;
            break;
    }
    switch (P555(player).flag1) {
        case 0:
            if (adx >= 20.0f) {
                P555(player).wheelIdx0 = (s32)((dx > 0.0f ? 1.0f : -1.0f) * P555(player).camX0);
            } else {
                P555(player).flag1++;
            }
            break;
        case 1:
            if ((P555(player).stickX > 0.0f || -1.0f * P555(player).stickX >= 6.0f) && P555(player).flag2 == 0) {
                P555(player).wheelIdx0 = (s32)((P555(player).stickX > 0.0f ? -1.0f : 1.0f) * P555(player).camY0);
            } else {
                P555(player).flag1++;
                n++;
            }
            break;
        case 2:
            n++;
            break;
    }
    if (n >= 2) {
        P555(player).flag0 = 0;
        P555(player).flag1 = 0;
        done = 1;
    } else if (lbl_1_bss_1894.hitCnt >= 0x78) {
        P555(player).flag0 = 0;
        P555(player).flag1 = 0;
        done = 1;
    }
    return done;
}

/* Same walk as fn_1_16104 but toward the model slot player+0x21, with a settle phase. */
s16 fn_1_16958(s16 player)
{
    f32 dx, dy, adx, ady;
    s32 done = 0;
    s16 n = 0;

    dx = P555(player).targetPos.x - lbl_1_bss_3030->mdl[(s16)(player + 0x21)].pos.x;
    dy = P555(player).targetPos.y - lbl_1_bss_3030->mdl[(s16)(player + 0x21)].pos.y;
    adx = M555_ABS(dx);
    ady = M555_ABS(dy);
    switch (P555(player).flag0) {
        case 0:
            if (ady >= 20.0f) {
                P555(player).wheelIdx1 = (s32)((P555(player).hookX != 0 ? -1.0f : 1.0f) * ((dy > 0.0f ? 1.0f : -1.0f) * P555(player).camX0));
            } else {
                P555(player).flag0++;
            }
            break;
        case 1:
            if ((P555(player).stickY > 0.0f || -1.0f * P555(player).stickY >= 6.0f) && P555(player).flag3 == 0) {
                P555(player).wheelIdx1 = (s32)((P555(player).hookX != 0 ? -1.0f : 1.0f) * ((P555(player).stickY > 0.0f ? -1.0f : 1.0f) * P555(player).camY0));
            } else {
                P555(player).flag0++;
            }
            break;
        case 2:
            if (ady >= 20.0f) {
                P555(player).flag0 = 0;
            } else {
                n = 1;
            }
            break;
    }
    switch (P555(player).flag1) {
        case 0:
            if (adx >= 20.0f) {
                P555(player).wheelIdx0 = (s32)((dx > 0.0f ? 1.0f : -1.0f) * P555(player).camX0);
            } else {
                P555(player).flag1++;
            }
            break;
        case 1:
            if ((P555(player).stickX > 0.0f || -1.0f * P555(player).stickX >= 6.0f) && P555(player).flag2 == 0) {
                P555(player).wheelIdx0 = (s32)((P555(player).stickX > 0.0f ? -1.0f : 1.0f) * P555(player).camY0);
            } else {
                P555(player).flag1++;
            }
            break;
        case 2:
            if (adx >= 20.0f) {
                P555(player).flag1 = 0;
            } else {
                n++;
            }
            break;
    }
    if (n >= 2) {
        P555(player).flag0 = 0;
        P555(player).flag1 = 0;
        done = 1;
    }
    return done;
}

/* Returns 1 when the carry point is at least `radius` away from the model slot player+0x21. */
s32 fn_1_171C8(s16 player, f32 radius)
{
    HuVecF d;
    s32 ret = 0;
    f32 len;

    d.x = P555(player).carryX - lbl_1_bss_3030->mdl[(s16)(player + 0x21)].pos.x;
    d.y = P555(player).carryY - lbl_1_bss_3030->mdl[(s16)(player + 0x21)].pos.y;
    len = sqrt(d.x * d.x + d.y * d.y);
    if (len >= radius) {
        ret = 1;
    }
    return ret;
}

/* Returns 1 when a live marker (height in (minZ, 121]) is within `radius` of pos. */
s32 fn_1_17400(HuVecF *pos, f32 radius, f32 minZ)
{
    s32 ret = 0;
    s16 i;
    f32 dx, dy;

    for (i = 0; i < 0x32; i++) {
        s16 m = i + 0x23;
        if (lbl_1_bss_B88[i].active != 0 && lbl_1_bss_3030->mdl[m].pos.z > minZ
            && lbl_1_bss_3030->mdl[m].pos.z <= 121.0f) {
            dx = lbl_1_bss_3030->mdl[m].pos.x - pos->x;
            dy = lbl_1_bss_3030->mdl[m].pos.y - (30.0f + pos->y);
            if (sqrt(dx * dx + dy * dy) < radius) {
                ret = 1;
            }
        }
    }
    return ret;
}
