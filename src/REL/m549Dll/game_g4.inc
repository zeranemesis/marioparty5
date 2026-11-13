/* ---- game.c part G4: fn_1_D0C4 .. fn_1_FDB0 (COM player AI helpers) ---- */

/* chase target selection: follow the other character's predicted position */
s32 fn_1_D0C4(s16 no, s16 limit) {
    HuVecF a;
    HuVecF b;
    f32 len;
    s32 ret = 0;

    switch (lbl_1_bss_3B0[no].sA8) {
        case 0:
            lbl_1_bss_3B0[no].btnB |= 0x100;
            lbl_1_bss_3B0[no].sAA = 0;
            lbl_1_bss_3B0[no].sA8++;
        case 1:
            if (lbl_1_bss_3B0[no].state == 1) {
                lbl_1_bss_3B0[no].btnA |= 0x100;
                if (lbl_1_bss_3B0[no].s5C >= limit) {
                    a.x = (lbl_1_bss_3B0[no ^ 1].velX / 5.6f) * lbl_1_bss_3B0[no].fC0;
                    a.z = -((lbl_1_bss_3B0[no ^ 1].velZ / 5.6f) * lbl_1_bss_3B0[no].fC0);
                    a.x += lbl_1_bss_3B0[no ^ 1].pos.x;
                    a.z += lbl_1_bss_3B0[no ^ 1].pos.z;
                    b.x = a.x - lbl_1_bss_3B0[no].pos.x;
                    b.z = a.z - lbl_1_bss_3B0[no].pos.z;
                    len = M549_Mag2D(b.x, b.z);
                    if (len != 0.0f) {
                        b.x = lbl_1_bss_3B0[no].pos.x + (20.0f * (b.x / len));
                        b.z = lbl_1_bss_3B0[no].pos.z + (20.0f * (b.z / len));
                    } else {
                        b.x = lbl_1_bss_3B0[no].pos.x;
                        b.z = lbl_1_bss_3B0[no].pos.z;
                    }
                    lbl_1_bss_3B0[no].target.x = b.x;
                    lbl_1_bss_3B0[no].target.y = lbl_1_bss_3B0[no].pos.y;
                    lbl_1_bss_3B0[no].target.z = b.z;
                    lbl_1_bss_3B0[no].targetSave = lbl_1_bss_3B0[no].target;
                    lbl_1_bss_3B0[no].s82 = lbl_1_bss_3B0[no].s80;
                    lbl_1_bss_3B0[no].sA8++;
                }
            } else {
                lbl_1_bss_3B0[no].sA8 = 0;
            }
            break;
        case 2:
            lbl_1_bss_3B0[no].s80 = 0;
            if (lbl_1_bss_3B0[no].s6A == 0 && lbl_1_bss_3B0[no].s6C == 0) {
                switch (lbl_1_bss_3B0[no].s7C) {
                    case 0:
                        MgPlayerComStkOn(lbl_1_bss_0.player[no]);
                        lbl_1_bss_3B0[no].s7C++;
                        break;
                    case 1:
                        if (lbl_1_bss_3B0[no].s28 == 0) {
                            lbl_1_bss_3B0[no].chaseSpeed = lbl_1_bss_3B0[no].fC4 * lbl_1_data_484[lbl_1_bss_3B0[no].s60];
                            if (MgPlayerVecChase(lbl_1_bss_0.player[no], &lbl_1_bss_3B0[no].target,
                                                 lbl_1_bss_3B0[no].chaseSpeed, lbl_1_bss_3B0[no].chaseRadius)) {
                            }
                        }
                        break;
                    case 2:
                        MgPlayerComStkOff(lbl_1_bss_0.player[no]);
                        lbl_1_bss_3B0[no].s7C = 0;
                        break;
                }
            }
            if (lbl_1_bss_3B0[no].sAA++ >= 5) {
                lbl_1_bss_3B0[no].target = lbl_1_bss_3B0[no].targetSave;
                lbl_1_bss_3B0[no].s80 = lbl_1_bss_3B0[no].s82;
                lbl_1_bss_3B0[no].sA8++;
            } else {
                lbl_1_bss_3B0[no].btnA |= 0x100;
            }
            break;
        case 3:
            if (lbl_1_bss_3B0[no].state != 2) {
                lbl_1_bss_3B0[no].sA8 = 0;
                ret = 1;
            }
            break;
    }
    return ret;
}

s32 fn_1_D9E0(s16 no) {
    HuVecF a;
    HuVecF b;
    f32 len;
    s32 ret = 0;

    switch (lbl_1_bss_3B0[no].sA8) {
        case 0:
            lbl_1_bss_3B0[no].btnB |= 0x100;
            lbl_1_bss_3B0[no].sAA = 0;
            lbl_1_bss_3B0[no].sA8++;
        case 1:
            if (lbl_1_bss_3B0[no].state == 1) {
                lbl_1_bss_3B0[no].btnA |= 0x100;
                a.x = (lbl_1_bss_3B0[no ^ 1].velX / 5.6f) * lbl_1_bss_3B0[no].fC0;
                a.z = -((lbl_1_bss_3B0[no ^ 1].velZ / 5.6f) * lbl_1_bss_3B0[no].fC0);
                a.x += lbl_1_bss_3B0[no ^ 1].pos.x;
                a.z += lbl_1_bss_3B0[no ^ 1].pos.z;
                b.x = a.x - lbl_1_bss_3B0[no].pos.x;
                b.z = a.z - lbl_1_bss_3B0[no].pos.z;
                len = M549_Mag2D(b.x, b.z);
                if (len != 0.0f) {
                    b.x = lbl_1_bss_3B0[no].pos.x + (20.0f * (b.x / len));
                    b.z = lbl_1_bss_3B0[no].pos.z + (20.0f * (b.z / len));
                } else {
                    b.x = lbl_1_bss_3B0[no].pos.x;
                    b.z = lbl_1_bss_3B0[no].pos.z;
                }
                lbl_1_bss_3B0[no].target.x = b.x;
                lbl_1_bss_3B0[no].target.y = lbl_1_bss_3B0[no].pos.y;
                lbl_1_bss_3B0[no].target.z = b.z;
                if (lbl_1_bss_3B0[no].s6A == 0 && lbl_1_bss_3B0[no].s6C == 0) {
                    switch (lbl_1_bss_3B0[no].s7C) {
                        case 0:
                            MgPlayerComStkOn(lbl_1_bss_0.player[no]);
                            lbl_1_bss_3B0[no].s7C++;
                            break;
                        case 1:
                            if (lbl_1_bss_3B0[no].s28 == 0) {
                                lbl_1_bss_3B0[no].chaseSpeed = lbl_1_bss_3B0[no].fC4 * lbl_1_data_484[lbl_1_bss_3B0[no].s60];
                                if (MgPlayerVecChase(lbl_1_bss_0.player[no], &lbl_1_bss_3B0[no].target,
                                                     lbl_1_bss_3B0[no].chaseSpeed, lbl_1_bss_3B0[no].chaseRadius)) {
                                }
                            }
                            break;
                        case 2:
                            MgPlayerComStkOff(lbl_1_bss_0.player[no]);
                            lbl_1_bss_3B0[no].s7C = 0;
                            break;
                    }
                    lbl_1_bss_3B0[no].sA8++;
                } else {
                    lbl_1_bss_3B0[no].btnB |= 0x100;
                }
            }
            break;
        case 2:
            if (lbl_1_bss_3B0[no].state != 2) {
                lbl_1_bss_3B0[no].sA8 = 0;
                ret = 1;
            }
            break;
    }
    return ret;
}

s32 fn_1_E160(s16 no, s16 objNo) {
    HuVecF v;
    f32 len;
    s16 i;
    s32 ret = 0;

    switch (lbl_1_bss_3B0[no].sA8) {
        case 0:
            lbl_1_bss_3B0[no].btnB |= 0x100;
            lbl_1_bss_3B0[no].sAA = 0;
            lbl_1_bss_3B0[no].sA8++;
        case 1:
            if (lbl_1_bss_3B0[no].state == 1) {
                lbl_1_bss_3B0[no].btnA |= 0x100;
                v.x = lbl_1_bss_158[objNo].pos.x;
                v.z = lbl_1_bss_158[objNo].pos.z;
                for (i = 0; i < 5; i++) {
                    v.x += lbl_1_bss_158[objNo].dir.x;
                    v.z += lbl_1_bss_158[objNo].dir.z;
                }
                v.x = v.x - lbl_1_bss_3B0[no].pos.x;
                v.z = v.z - lbl_1_bss_3B0[no].pos.z;
                len = M549_Mag2D(v.x, v.z);
                if (len != 0.0f) {
                    v.x = lbl_1_bss_3B0[no].pos.x + (20.0f * (v.x / len));
                    v.z = lbl_1_bss_3B0[no].pos.z + (20.0f * (v.z / len));
                } else {
                    v.x = lbl_1_bss_3B0[no].pos.x;
                    v.z = lbl_1_bss_3B0[no].pos.z;
                }
                lbl_1_bss_3B0[no].target.x = v.x;
                lbl_1_bss_3B0[no].target.y = lbl_1_bss_3B0[no].pos.y;
                lbl_1_bss_3B0[no].target.z = v.z;
                if (lbl_1_bss_3B0[no].s6A == 0 && lbl_1_bss_3B0[no].s6C == 0) {
                    switch (lbl_1_bss_3B0[no].s7C) {
                        case 0:
                            MgPlayerComStkOn(lbl_1_bss_0.player[no]);
                            lbl_1_bss_3B0[no].s7C++;
                            break;
                        case 1:
                            if (lbl_1_bss_3B0[no].s28 == 0) {
                                lbl_1_bss_3B0[no].chaseSpeed = lbl_1_bss_3B0[no].fC4 * lbl_1_data_484[lbl_1_bss_3B0[no].s60];
                                if (MgPlayerVecChase(lbl_1_bss_0.player[no], &lbl_1_bss_3B0[no].target,
                                                     lbl_1_bss_3B0[no].chaseSpeed, lbl_1_bss_3B0[no].chaseRadius)) {
                                }
                            }
                            break;
                        case 2:
                            MgPlayerComStkOff(lbl_1_bss_0.player[no]);
                            lbl_1_bss_3B0[no].s7C = 0;
                            break;
                    }
                    lbl_1_bss_3B0[no].sA8++;
                } else {
                    lbl_1_bss_3B0[no].btnB |= 0x100;
                }
            }
            break;
        case 2:
            if (lbl_1_bss_3B0[no].state != 2) {
                lbl_1_bss_3B0[no].sA8 = 0;
                ret = 1;
            }
            break;
    }
    return ret;
}

void fn_1_E880(s16 no) {
    if (lbl_1_bss_3B0[no].sAC != 0) {
        return;
    }
    if (lbl_1_bss_3B0[no].state == 1) {
        lbl_1_bss_3B0[no].btnA |= 0x100;
    } else {
        lbl_1_bss_3B0[no].btnB |= 0x100;
    }
}

/* pick a point on the circle around the character that stays inside the arena */
s32 fn_1_E90C(s16 no, s16 objNo) {
    HuVecF v;
    HuVecF w;
    f32 lenA;
    f32 lenB;
    f32 ang;
    s32 ret;

    v.x = lbl_1_bss_158[objNo].pos.x - lbl_1_bss_3B0[no].pos.x;
    v.z = lbl_1_bss_158[objNo].pos.z - lbl_1_bss_3B0[no].pos.z;
    if (M549_Mag2D(v.x, v.z) >= 130.0f) {
        ang = lbl_1_bss_3B0[no].fBC;
        v.x = (f32)(lbl_1_bss_3B0[no].pos.x + (200.0 * HuSin(ang)));
        v.z = (f32)(lbl_1_bss_3B0[no].pos.z + (200.0 * HuCos(ang)));
        lenA = M549_Mag2D(v.x, v.z);
        ang = 180.0f + lbl_1_bss_3B0[no].fBC;
        w.x = (f32)(lbl_1_bss_3B0[no].pos.x + (200.0 * HuSin(ang)));
        w.z = (f32)(lbl_1_bss_3B0[no].pos.z + (200.0 * HuCos(ang)));
        lenB = M549_Mag2D(w.x, w.z);
        if (lenB < lbl_1_data_498) {
            lbl_1_bss_3B0[no].target.x = w.x;
            lbl_1_bss_3B0[no].target.z = w.z;
            ret = 1;
        } else if (lenA < lbl_1_data_498) {
            lbl_1_bss_3B0[no].target.x = v.x;
            lbl_1_bss_3B0[no].target.z = v.z;
            ret = 1;
        } else {
            ret = -1;
        }
    } else {
        ret = -1;
    }
    return ret;
}

/* distance to the opponent / to the arena centre */
void fn_1_F0C8(s16 no) {
    HuVecF d;
    s16 other = no ^ 1;

    d.x = lbl_1_bss_3B0[other].pos.x - lbl_1_bss_3B0[no].pos.x;
    d.z = lbl_1_bss_3B0[other].pos.z - lbl_1_bss_3B0[no].pos.z;
    lbl_1_bss_3B0[no].fB4 = M549_Mag2D(d.x, d.z);
    d.x = lbl_1_bss_3B0[no].pos.x;
    d.z = lbl_1_bss_3B0[no].pos.z;
    lbl_1_bss_3B0[no].fB8 = M549_Mag2D(d.x, d.z);
}

/* does the object's path (segment of length 1190 along dir) pass close to the character? */
s32 fn_1_F4A0(s16 no, s16 objNo) {
    HuVecF p0;
    HuVecF p1;
    f32 maxX;
    f32 minX;
    f32 maxZ;
    f32 minZ;
    f32 dx;
    f32 dz;
    f32 t;
    f32 num;
    f32 len;
    f32 den;
    f32 ang;
    f32 sinTest;
    f32 cz;
    s32 ret = 0;

    p0 = lbl_1_bss_158[objNo].pos;
    len = M549_Mag2D(lbl_1_bss_158[objNo].dir.x, lbl_1_bss_158[objNo].dir.z);
    if (len != 0.0f) {
        p1.x = p0.x + ((1190.0f / len) * lbl_1_bss_158[objNo].dir.x);
        p1.z = p0.z + ((1190.0f / len) * lbl_1_bss_158[objNo].dir.z);
    } else {
        p1.x = p0.x;
        p1.z = p0.z;
    }
    if (p0.x > p1.x) {
        maxX = p0.x;
        minX = p1.x;
    } else {
        maxX = p1.x;
        minX = p0.x;
    }
    if (p0.z > p1.z) {
        maxZ = p0.z;
        minZ = p1.z;
    } else {
        maxZ = p1.z;
        minZ = p0.z;
    }
    maxX = maxX + (45.0f + lbl_1_bss_158[objNo].radius);
    minX -= 45.0f + lbl_1_bss_158[objNo].radius;
    maxZ += 45.0f + lbl_1_bss_158[objNo].radius;
    minZ -= 45.0f + lbl_1_bss_158[objNo].radius;
    if (lbl_1_bss_3B0[no].pos.x <= maxX && lbl_1_bss_3B0[no].pos.x >= minX
        && lbl_1_bss_3B0[no].pos.z <= maxZ && lbl_1_bss_3B0[no].pos.z >= minZ) {
        dx = p0.x - p1.x;
        dz = p0.z - p1.z;
        den = (dx * dx) + (dz * dz);
        num = -1.0f * ((dx * (p0.x - lbl_1_bss_3B0[no].pos.x)) + (dz * (p0.z - lbl_1_bss_3B0[no].pos.z)));
        if (den != 0.0f) {
            t = num / den;
        } else {
            t = 0.0f;
        }
        cz = p0.z + (dz * t);
        dx = lbl_1_bss_3B0[no].pos.x - (p0.x + (dx * t));
        dz = lbl_1_bss_3B0[no].pos.z - cz;
        len = M549_Mag2D(dx, dz);
        if (len < 45.0f + lbl_1_bss_158[objNo].radius) {
            ang = (f32)(180.0 * (acos(dx / (0.1f + len)) / M_PI));
            sinTest = (f32) - (180.0 * (asin(dz / (0.1f + len)) / M_PI));
            if (sinTest <= 0.0f) {
                ang = 360.0f - ang;
            }
            lbl_1_bss_3B0[no].fBC = ang - 90.0f;
            ret = 1;
        }
    }
    return ret;
}

/* COM player AI: decide and run the current action */
s32 fn_1_FDB0(s16 no) {
    HuVecF d;
    HuVecF v;
    HuVecF w;
    f32 rot;
    f32 dist;
    s16 i;
    s16 other;
    s16 range;
    s16 rnd;
    s16 done;
    s32 ret = 0;

    other = no ^ 1;
    d.x = lbl_1_bss_3B0[other].pos.x - lbl_1_bss_3B0[no].pos.x;
    d.z = lbl_1_bss_3B0[other].pos.z - lbl_1_bss_3B0[no].pos.z;
    lbl_1_bss_3B0[no].fB4 = M549_Mag2D(d.x, d.z);
    d.x = lbl_1_bss_3B0[no].pos.x;
    d.z = lbl_1_bss_3B0[no].pos.z;
    lbl_1_bss_3B0[no].fB8 = M549_Mag2D(d.x, d.z);

    if (lbl_1_bss_3B0[no].state2 == 0) {
        for (i = 0; i < 10; i++) {
            if (lbl_1_bss_158[i].active != 0 && (no ^ 1) == lbl_1_bss_158[i].side && fn_1_F4A0(no, i) != 0
                && lbl_1_bss_3B0[no].sCC > (rnd = frandmod(100)) && fn_1_E90C(no, i) == 1) {
                lbl_1_bss_3B0[no].s80 = 1;
                lbl_1_bss_3B0[no].s84 = 0;
            }
        }
    }

    switch (lbl_1_bss_3B0[no].sB2) {
        case 0:
            if (lbl_1_bss_3B0[no].state2 == 0) {
                lbl_1_bss_3B0[no].s7A = 0;
                if (lbl_1_bss_3B0[no].fB4 <= 120.0f) {
                    if ((rnd = frandmod(2)) != 0) {
                        if (lbl_1_bss_3B0[(s16)(no ^ 1)].state2 == 1) {
                            lbl_1_bss_3B0[no].sB0 = 30;
                            lbl_1_bss_3B0[no].s80 = 0;
                            lbl_1_bss_3B0[no].sB2 = 7;
                        } else {
                            lbl_1_bss_3B0[no].s80 = 0;
                            lbl_1_bss_3B0[no].sB2 = 5;
                        }
                    }
                } else if (lbl_1_bss_3B0[no].sD2 > (rnd = frandmod(100)) && lbl_1_bss_3B0[no].fB8 >= 400.0f) {
                    lbl_1_bss_3B0[no].s80 = 1;
                    lbl_1_bss_3B0[no].s78 = 0;
                    lbl_1_bss_3B0[no].target.x = 0.0f;
                    lbl_1_bss_3B0[no].target.z = 0.0f;
                    lbl_1_bss_3B0[no].sB2 = 8;
                } else {
                    if ((rnd = frandmod(2)) != 0) {
                        if (lbl_1_bss_3B0[no].sCA > (rnd = frandmod(100))) {
                            range = 0x79 - lbl_1_bss_3B0[no].sCE;
                            if (range > 0) {
                                rnd = lbl_1_bss_3B0[no].sD0 + frandmod(range);
                            } else {
                                rnd = lbl_1_bss_3B0[no].sD0;
                            }
                            if (rnd >= 0x78) {
                                rnd = 0x78;
                            }
                            lbl_1_bss_3B0[no].sB0 = rnd;
                            lbl_1_bss_3B0[no].s80 = 0;
                            lbl_1_bss_3B0[no].sB2 = 7;
                            break;
                        }
                    } else if (lbl_1_bss_3B0[no].sCA > (rnd = frandmod(100))) {
                        range = 0x79 - lbl_1_bss_3B0[no].sCE;
                        if (range > 0) {
                            rnd = lbl_1_bss_3B0[no].sD0 + frandmod(range);
                        } else {
                            rnd = lbl_1_bss_3B0[no].sD0;
                        }
                        if (rnd >= 0x78) {
                            rnd = 0x78;
                        }
                        lbl_1_bss_3B0[no].sB0 = rnd;
                        done = 0;
                        v = lbl_1_bss_3B0[no].pos;
                        do {
                            rot = (f32)frandmod(360);
                            dist = (f32)frandmod(395);
                            v.x = (f32)(dist * HuCos(rot));
                            v.z = (f32)(dist * HuSin(rot));
                        } while (!(M549_Mag2D(v.x, v.z) < lbl_1_data_498));
                        lbl_1_bss_3B0[no].target = v;
                        done = 1;
                        lbl_1_bss_3B0[no].s80 = 1;
                        lbl_1_bss_3B0[no].s84 = 0;
                        lbl_1_bss_3B0[no].sB2 = 10;
                        break;
                    }
                    if (lbl_1_bss_3B0[no].sC8 > (rnd = frandmod(100))) {
                        done = 0;
                        w = lbl_1_bss_3B0[no].pos;
                        do {
                            rot = (f32)frandmod(360);
                            dist = (f32)frandmod(395);
                            w.x = (f32)(dist * HuCos(rot));
                            w.z = (f32)(dist * HuSin(rot));
                        } while (!(M549_Mag2D(w.x, w.z) < lbl_1_data_498));
                        lbl_1_bss_3B0[no].target = w;
                        done = 1;
                        lbl_1_bss_3B0[no].s80 = 1;
                        lbl_1_bss_3B0[no].s84 = 0;
                        lbl_1_bss_3B0[no].sB2 = 6;
                    } else if (lbl_1_bss_3B0[no].sD4 > (rnd = frandmod(100))) {
                        lbl_1_bss_3B0[no].sB2 = 8;
                    }
                }
            }
            break;
        case 5:
            lbl_1_bss_3B0[no].fC0 = 0.0f;
            if (fn_1_D9E0(no) != 0) {
                lbl_1_bss_3B0[no].sB2 = 0;
            }
            break;
        case 6:
            if (lbl_1_bss_3B0[no].s84 != 0 || lbl_1_bss_3B0[no].s6A != 0 || lbl_1_bss_3B0[no].s6C != 0
                || lbl_1_bss_3B0[no].s7A > 0x3C) {
                lbl_1_bss_3B0[no].s80 = 0;
                lbl_1_bss_3B0[no].sB2 = 0;
            }
            break;
        case 7:
            if (lbl_1_bss_3B0[no].fB4 <= 150.0f) {
                lbl_1_bss_3B0[no].fC0 = 0.0f;
                lbl_1_bss_3B0[no].sB0 = 0;
            } else if (lbl_1_bss_3B0[no].fB8 >= 500.0f) {
                lbl_1_bss_3B0[no].fC0 = 20.0f;
                lbl_1_bss_3B0[no].sB0 = 0;
            } else {
                lbl_1_bss_3B0[no].fC0 = 20.0f;
            }
            if (fn_1_D0C4(no, lbl_1_bss_3B0[no].sB0) != 0) {
                lbl_1_bss_3B0[no].sB2 = 0;
            }
            break;
        case 8:
            if (lbl_1_bss_3B0[no].s78++ > 0x28) {
                lbl_1_bss_3B0[no].s78 = 0;
                lbl_1_bss_3B0[no].sB2 = 0;
            }
            break;
        case 9:
            if (lbl_1_bss_3B0[no].s84 != 0 || lbl_1_bss_3B0[no].s6A == 0 || lbl_1_bss_3B0[no].s6C == 0) {
                lbl_1_bss_3B0[no].s78 = 0;
                lbl_1_bss_3B0[no].sB2 = 0;
            }
            break;
        case 10:
            if (lbl_1_bss_3B0[no].fB4 <= 150.0f) {
                lbl_1_bss_3B0[no].fC0 = 0.0f;
                lbl_1_bss_3B0[no].sB0 = 0;
            } else if (lbl_1_bss_3B0[no].fB8 >= 500.0f) {
                lbl_1_bss_3B0[no].fC0 = 20.0f;
                lbl_1_bss_3B0[no].sB0 = 0;
            } else {
                lbl_1_bss_3B0[no].fC0 = 20.0f;
            }
            if (fn_1_D0C4(no, lbl_1_bss_3B0[no].sB0) != 0) {
                lbl_1_bss_3B0[no].s80 = 0;
                lbl_1_bss_3B0[no].sB2 = 0;
            }
            break;
    }

    if (lbl_1_bss_3B0[no].s80 != 0) {
        done = 0;
        if (lbl_1_bss_3B0[no].s6A == 0 && lbl_1_bss_3B0[no].s6C == 0) {
            switch (lbl_1_bss_3B0[no].s7C) {
                case 0:
                    MgPlayerComStkOn(lbl_1_bss_0.player[no]);
                    lbl_1_bss_3B0[no].s7C++;
                    break;
                case 1:
                    if (lbl_1_bss_3B0[no].s28 == 0) {
                        lbl_1_bss_3B0[no].chaseSpeed = lbl_1_bss_3B0[no].fC4 * lbl_1_data_484[lbl_1_bss_3B0[no].s60];
                        if (MgPlayerVecChase(lbl_1_bss_0.player[no], &lbl_1_bss_3B0[no].target,
                                             lbl_1_bss_3B0[no].chaseSpeed, lbl_1_bss_3B0[no].chaseRadius)) {
                            done = 1;
                        }
                    }
                    break;
                case 2:
                    MgPlayerComStkOff(lbl_1_bss_0.player[no]);
                    lbl_1_bss_3B0[no].s7C = 0;
                    done = 1;
                    break;
            }
        }
        lbl_1_bss_3B0[no].s84 = done;
    }
    if (lbl_1_bss_3B0[no].sAE != 0) {
        if (lbl_1_bss_3B0[no].sAC != 0) {
        } else if (lbl_1_bss_3B0[no].state == 1) {
            lbl_1_bss_3B0[no].btnA |= 0x100;
        } else {
            lbl_1_bss_3B0[no].btnB |= 0x100;
        }
    }
    return ret;
}
