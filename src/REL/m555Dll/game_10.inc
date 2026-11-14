#include "math.h"
#include "game/frand.h"

/* Tests whether the line from the lure position (model 0x21 + player, raised
   by 30) towards the player's target point passes within 130 of a live marker;
   returns 1 on a hit. */
s32 fn_1_19C0C(s16 player)
{
    f32 margin = 130.0f;
    f32 unused = 0.0f;
    s32 result;
    HuVecF pos;
    HuVecF tgt;
    HuVecF d;
    HuVecF cur;
    HuVecF foot;
    HuVecF e;
    f32 maxX;
    f32 minX;
    f32 maxY;
    f32 minY;
    f32 den;
    f32 t;
    f32 u;
    f32 len;
    f32 dist;
    s16 i;

    result = 0;
    unused = 0.0f;
    pos = lbl_1_bss_3030->mdl[player + 0x21].pos;
    pos.y += 30.0f;
    tgt = lbl_1_bss_120C[player].targetPos;
    d.x = tgt.x - pos.x;
    d.y = tgt.y - pos.y;
    d.z = 0.0f; /* the original reads an uninitialised stack slot here */
    len = sqrt(d.x * d.x + d.z * d.z);
    len /= 300.0f;
    tgt.x = pos.x + d.x * len;
    tgt.y = pos.y + d.y * len;
    d.x = tgt.x - pos.x;
    d.y = tgt.y - pos.y;
    if (pos.x > tgt.x) {
        maxX = pos.x;
        minX = tgt.x;
    } else {
        maxX = tgt.x;
        minX = pos.x;
    }
    if (pos.y > tgt.y) {
        maxY = pos.y;
        minY = tgt.y;
    } else {
        maxY = tgt.y;
        minY = pos.y;
    }
    maxX += margin;
    minX -= margin;
    maxY += margin;
    minY -= margin;
    for (i = 0; i < 0x32; i++) {
        cur = lbl_1_bss_3030->mdl[(s16)(i + 0x23)].pos;
        if (lbl_1_bss_B88[i].active != 0 && cur.z >= -12000.0f && cur.x <= maxX && cur.x >= minX
            && cur.y <= maxY && cur.y >= minY) {
            den = d.x * d.x + d.y * d.y;
            t = -1.0f * (d.x * (tgt.x - cur.x) + d.y * (tgt.y - cur.y));
            if (den != 0.0f) {
                u = t / den;
            } else {
                u = 0.0f;
            }
            foot.x = tgt.x + d.x * u;
            foot.y = tgt.y + d.y * u;
            e.x = cur.x - foot.x;
            e.y = cur.y - foot.y;
            e.z = 0.0f;
            dist = sqrt(e.x * e.x + e.y * e.y);
            if (dist < margin) {
                e.x = foot.x - pos.x;
                e.y = foot.y - pos.y;
                dist = sqrt(e.x * e.x + e.y * e.y);
                if (dist <= 300.0f) {
                    result = 1;
                }
            }
        }
    }
    return result;
}

/* Picks a random point within 50..250 of the player's marker model that stays
   inside the play area; up to 21 tries. */
s32 fn_1_1A4B0(s16 player)
{
    s32 result;
    s16 tries;
    f32 ang;
    f32 dist;
    HuVecF pos;
    HuVecF cand;

    result = 0;
    tries = 0;
    pos = lbl_1_bss_3030->mdl[player + 0xA9].pos;
    do {
        ang = (f32)frandmod(0x168);
        dist = 50.0f + (f32)frandmod(0xC8);
        cand.x = pos.x + dist * cos(3.141592653589793 * ang / 180.0);
        cand.y = pos.y + dist * sin(3.141592653589793 * ang / 180.0);
        cand.z = 0.0f; /* the original copies an uninitialised stack slot */
        if (cand.x <= 530.0f && cand.x >= -530.0f && 30.0f + cand.y <= 316.0f && 30.0f + cand.y >= -316.0f) {
            lbl_1_bss_120C[player].targetPos = cand;
            result = 1;
            break;
        }
    } while (tries++ < 0x14);
    return result;
}

/* Finds a live marker (model 0x23 + i) inside the play area and far enough
   back in z; stores it as the player's target. */
s32 fn_1_1A6F4(s16 player)
{
    s32 result;
    s16 i;
    HuVecF p;

    result = 0;
    for (i = 0; i < 0x32; i++) {
        p = lbl_1_bss_3030->mdl[i + 0x23].pos;
        if (lbl_1_bss_B88[i].active != 0 && p.z < -10000.0f && p.z > -16000.0f && p.x <= 530.0f && p.x >= -530.0f
            && 30.0f + p.y <= 316.0f && 30.0f + p.y >= -316.0f) {
            lbl_1_bss_120C[player].targetPos = p;
            result = 1;
            break;
        }
    }
    return result;
}

/* Returns 1 if the marker model is inside the ring (always when flag != 0). */
s32 fn_1_1A880(s32 player, s16 flag)
{
    s32 result;
    s16 idx;

    result = 0;
    idx = player + 0xA9;
    if (flag == 0) {
        if (lbl_1_bss_3030->mdl[idx].pos.x <= 100.0f && lbl_1_bss_3030->mdl[idx].pos.x >= -100.0f
            && lbl_1_bss_3030->mdl[idx].pos.y <= 100.0f && lbl_1_bss_3030->mdl[idx].pos.y >= -150.0f) {
            result = 1;
        }
    } else {
        result = 1;
    }
    return result;
}
