#include "math.h"

/* Casts rays from the player position at 8 angles, finds the nearest marker
   or arena wall hit on each, and aims at the farthest valid hit. */
void fn_1_176C4(s16 player)
{
    M555_PLAYER *pl;
    f32 wall[4][3] = {
        { 1.0f, 0.0f, -531.0f },
        { 1.0f, 0.0f, 531.0f },
        { 0.0f, 1.0f, -317.0f },
        { 0.0f, 1.0f, 317.0f },
    };
    HuVecF c;
    HuVecF end;
    HuVecF dir;
    HuVecF mk;
    HuVecF hit1;
    HuVecF hit2;
    f32 ang;
    f32 maxDist;
    f32 la, lb, lc;
    f32 wa, wb, wc;
    f32 t, num, den, len, tmp, absZ;
    s16 i, j, k, best, xs, ys;

    maxDist = 180.0f;
    hit1.z = 0.0f;
    hit2.z = 0.0f;
    for (i = 0; i < 0x24; i++) {
        lbl_1_bss_120C[player].edgeDist[i] = 2000.0f;
        lbl_1_bss_120C[player].edgePos[i].x = 0.0f;
        lbl_1_bss_120C[player].edgePos[i].y = 0.0f;
        lbl_1_bss_120C[player].edgePos[i].z = 0.0f;
        lbl_1_bss_120C[player].edgeKind[i] = -1;
    }
    c = lbl_1_bss_3030->mdl[(s16)(player + 0x21)].pos;
    c.y += 30.0f;
    ang = 0.0f;
    for (i = 0; i < 8; i++) {
        end.x = c.x + 10.0 * cos(M_PI * ang / 180.0);
        end.y = c.y + 10.0 * sin(M_PI * ang / 180.0);
        dir.x = end.x - c.x;
        dir.y = end.y - c.y;
        lc = (-(end.y - c.y) * c.x) - ((c.x - end.x) * c.y);
        for (j = 0; j < 0x32; j++) {
            mk = lbl_1_bss_3030->mdl[(s16)(j + 0x23)].pos;
            if (lbl_1_bss_B88[j].active != 0 && mk.z > -13000.0f && mk.z < 150.0f) {
                hit1.x = mk.x - c.x;
                hit1.y = mk.y - c.y;
                xs = 0;
                ys = 0;
                if ((ang >= 0.0f && ang < 90.0f) || (ang > 270.0f && ang <= 360.0f)) {
                    if (hit1.x > 0.0f) {
                        xs = 1;
                    }
                } else if (90.0f == ang || 270.0f == ang) {
                    xs = 1;
                } else if (hit1.x < 0.0f) {
                    xs = 1;
                }
                if (ang > 0.0f && ang < 180.0f) {
                    if (hit1.y > 0.0f) {
                        ys = 1;
                    }
                } else if (0.0f == ang || 180.0f == ang || 360.0f == ang) {
                    ys = 1;
                } else if (hit1.y < 0.0f) {
                    ys = 1;
                }
                len = sqrt(hit1.x * hit1.x + hit1.y * hit1.y);
                if (xs != 0 && ys != 0) {
                    den = dir.x * dir.x + dir.y * dir.y;
                    num = -1.0f * (dir.x * (end.x - mk.x) + dir.y * (end.y - mk.y));
                    if (den != 0.0f) {
                        t = num / den;
                    } else {
                        t = 0.0f;
                    }
                    hit1.x = end.x + dir.x * t;
                    hit1.y = end.y + dir.y * t;
                    hit2.x = mk.x - hit1.x;
                    hit2.y = mk.y - hit1.y;
                    len = sqrt(hit2.x * hit2.x + hit2.y * hit2.y);
                    if (hit1.x >= -530.0f && hit1.x <= 530.0f && hit1.y >= -316.0f && hit1.y <= 316.0f && len < maxDist) {
                        hit2.x = hit1.x - c.x;
                        hit2.y = hit1.y - c.y;
                        len = sqrt(hit2.x * hit2.x + hit2.y * hit2.y);
                        if ((s16)ang % 90 != 0) {
                            tmp = sqrt(200.0);
                        } else {
                            tmp = 10.0f;
                        }
                        if (mk.z / 200.0f > 0.0f) {
                            absZ = mk.z / 200.0f;
                        } else {
                            absZ = -1.0f * (mk.z / 200.0f);
                        }
                        if (!(absZ > 50.0f + (15.0f + (len / tmp)))) {
                            if (lbl_1_bss_120C[player].edgeDist[i] >= len) {
                                hit2.x = hit1.x - c.x;
                                hit2.y = hit1.y - c.y;
                                len = sqrt(hit2.x * hit2.x + hit2.y * hit2.y);
                                if (len != 0.0f) {
                                    tmp = (len - 150.0f) / len;
                                } else {
                                    tmp = 0.0f;
                                }
                                hit1.y -= 30.0f;
                                lbl_1_bss_120C[player].edgePos[i] = hit1;
                                lbl_1_bss_120C[player].edgeDist[i] = len;
                                lbl_1_bss_120C[player].edgeKind[i] = 0;
                            }
                        }
                    }
                }
            }
        }
        c = lbl_1_bss_3030->mdl[(s16)(player + 0x21)].pos;
        c.y += 30.0f;
        end.x = c.x + 10.0 * cos(M_PI * ang / 180.0);
        end.y = c.y + 10.0 * sin(M_PI * ang / 180.0);
        dir.x = end.x - c.x;
        dir.y = end.y - c.y;
        la = end.y - c.y;
        lb = c.x - end.x;
        lc = (-la * c.x) - (lb * c.y);
        for (k = 0; k < 4; k++) {
            wa = wall[k][0];
            wb = wall[k][1];
            wc = wall[k][2];
            if (sqrt(la * la + lb * lb) != 0.0 && sqrt(wa * wa + wb * wb) != 0.0) {
                num = la * wa + lb * wb;
                den = sqrt(la * la + lb * lb) * sqrt(wa * wa + wb * wb);
                t = num / den;
            } else {
                t = 0.0f;
            }
            if (t < 0.984f && t > -0.984f) {
                if (la != 0.0f && lb != 0.0f && wa != 0.0f && wb != 0.0f) {
                    hit1.x = wa * (-lb / la);
                    hit1.y = -1.0f * (wa * (-lc / la));
                    hit1.x += wb;
                    hit1.y -= wc;
                    if (hit1.x != 0.0f) {
                        hit2.y = hit1.y / hit1.x;
                    } else {
                        hit2.y = 0.0f;
                    }
                    hit2.x = ((-wb * hit2.y) - wc) / wa;
                } else if (la == 0.0f && lb != 0.0f && wa != 0.0f && wb != 0.0f) {
                    hit2.y = -lc / lb;
                    hit2.x = ((-wb * hit2.y) - wc) / wa;
                } else if (la != 0.0f && lb == 0.0f && wa != 0.0f && wb != 0.0f) {
                    hit2.x = -lc / la;
                    hit2.y = ((-wa * hit2.x) - wc) / wb;
                } else if (la != 0.0f && lb != 0.0f && wa == 0.0f && wb != 0.0f) {
                    hit2.y = -wc / wb;
                    hit2.x = ((-lb * hit2.y) - lc) / la;
                } else if (la != 0.0f && lb != 0.0f && wa != 0.0f && wb == 0.0f) {
                    hit2.x = -wc / wa;
                    hit2.y = ((-la * hit2.x) - lc) / lb;
                } else if (la == 0.0f && lb != 0.0f && wa != 0.0f && wb == 0.0f) {
                    hit2.y = -lc / lb;
                    hit2.x = -wc / wa;
                } else if (la != 0.0f && lb == 0.0f && wa == 0.0f && wb != 0.0f) {
                    hit2.x = -lc / la;
                    hit2.y = -wc / wb;
                }
                hit1.x = hit2.x - c.x;
                hit1.y = hit2.y - c.y;
                xs = 0;
                ys = 0;
                if ((ang >= 0.0f && ang < 90.0f) || (ang > 270.0f && ang <= 360.0f)) {
                    if (hit1.x > 0.0f) {
                        xs = 1;
                    }
                } else if (90.0f == ang || 270.0f == ang) {
                    xs = 1;
                } else if (hit1.x < 0.0f) {
                    xs = 1;
                }
                if (ang > 0.0f && ang < 180.0f) {
                    if (hit1.y > 0.0f) {
                        ys = 1;
                    }
                } else if (0.0f == ang || 180.0f == ang || 360.0f == ang) {
                    ys = 1;
                } else if (hit1.y < 0.0f) {
                    ys = 1;
                }
                if (hit2.x >= -531.0f && hit2.x <= 531.0f && hit2.y >= -317.0f && hit2.y <= 317.0f && xs != 0 && ys != 0) {
                    hit1.x = hit2.x - c.x;
                    hit1.y = hit2.y - c.y;
                    len = sqrt(hit1.x * hit1.x + hit1.y * hit1.y);
                    if (lbl_1_bss_120C[player].edgeDist[i] >= len) {
                        hit2.y -= 30.0f;
                        lbl_1_bss_120C[player].edgePos[i] = hit2;
                        lbl_1_bss_120C[player].edgeDist[i] = len;
                        lbl_1_bss_120C[player].edgeKind[i] = 1;
                    }
                }
            }
        }
        ang += 45.0f;
    }
    pl = &lbl_1_bss_120C[player];
    best = 0;
    len = 0.0f;
    for (i = 0; i < 8; i++) {
        if (pl->edgeDist[i] > len && pl->edgeDist[i] != 2000.0f) {
            len = pl->edgeDist[i];
            best = i;
        }
    }
    if ((s32)pl->edgePos[best].x < -0x276 || (s32)pl->edgePos[best].x > 0x276
        || (s32)pl->edgePos[best].y < -0x1A0 || (s32)pl->edgePos[best].y > 0x1A0) {
        return;
    }
    pl->aimPos = pl->edgePos[best];
    pl->targetPos = pl->edgePos[best];
    pl->carryX = lbl_1_bss_3030->mdl[(s16)(player + 0x21)].pos.x;
    pl->carryY = lbl_1_bss_3030->mdl[(s16)(player + 0x21)].pos.y;
    pl->carryZ = lbl_1_bss_3030->mdl[(s16)(player + 0x21)].pos.z;
    ang = 45.0f * best;
}
