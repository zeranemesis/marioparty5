/* ---- game.c part G3: fn_1_A898 .. fn_1_CDD8 ---- */

s32 fn_1_A898(void)
{
    HU3DMODEL *mdl;
    M549StageSlot *slot;
    s32 done;
    s32 ret;

    mdl = &Hu3DData[lbl_1_bss_15F4->models[(lbl_1_bss_580.s24 & 1) + 0x44].id];
    slot = &lbl_1_bss_15F4->slots[lbl_1_bss_580.s24 & 1];
    done = 0;
    ret = 0;
    switch (lbl_1_bss_580.mode) {
        case 0:
            if (lbl_1_bss_580.timer++ >= 30) {
                lbl_1_bss_580.timer = 0;
                if (lbl_1_bss_580.s24 != 0) {
                    lbl_1_bss_580.f08 = lbl_1_bss_0.player[lbl_1_bss_580.s24 & 1]->actor->rotY;
                    if (lbl_1_bss_580.f08 <= 1.0f && lbl_1_bss_580.f08 >= -1.0f) {
                        lbl_1_bss_580.mode = 2;
                        break;
                    }
                    slot->shiftState = 1;
                    slot->shiftMotion = 1;
                    slot->shiftStart = 0.0f;
                    slot->shiftEnd = 4.0f;
                    slot->shiftAttr = 0x40000001;
                    slot->attrReset = 0;
                }
                lbl_1_bss_580.mode++;
            }
            break;
        case 1:
            if (lbl_1_bss_580.s24 != 0) {
                ret = fn_1_18B0(&mdl->rot.y, 30.0f, lbl_1_bss_580.f08, 0.0f);
                if ((s16)ret == 0) {
                    slot->shiftState = 1;
                    slot->shiftMotion = 0;
                    slot->shiftStart = 0.0f;
                    slot->shiftEnd = 8.0f;
                    slot->shiftAttr = 0x40000001;
                    slot->attrReset = 0;
                    lbl_1_bss_580.mode++;
                }
            } else {
                lbl_1_bss_580.mode++;
            }
            break;
        case 2:
            if (lbl_1_bss_580.s24 != 0) {
                f32 time = 30.0f;

                ret += fn_1_18B0(&lbl_1_bss_14[0].center.x, time, lbl_1_data_288[2].center.x, mdl->pos.x);
                ret += fn_1_18B0(&lbl_1_bss_14[0].center.y, time, lbl_1_data_288[2].center.y, 70.0f + mdl->pos.y);
                ret += fn_1_18B0(&lbl_1_bss_14[0].center.z, time, lbl_1_data_288[2].center.z, mdl->pos.z);
                ret += fn_1_18B0(&lbl_1_bss_14[0].zoom, time, lbl_1_data_288[2].zoom, 500.0f);
                ret += fn_1_18B0(&lbl_1_bss_14[1].center.x, time, lbl_1_data_288[2].center.x, mdl->pos.x);
                ret += fn_1_18B0(&lbl_1_bss_14[1].center.y, time, lbl_1_data_288[2].center.y, 70.0f + mdl->pos.y);
                ret += fn_1_18B0(&lbl_1_bss_14[1].center.z, time, lbl_1_data_288[2].center.z, mdl->pos.z);
                ret += fn_1_18B0(&lbl_1_bss_14[1].zoom, time, lbl_1_data_288[2].zoom, 500.0f);
            }
            if ((s16)ret == 0) {
                lbl_1_bss_580.mode++;
            }
            break;
        case 3:
            done = 1;
            break;
    }
    return done;
}

int fn_1_AD78(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b)
{
    s16 unused = 0;

    switch (b->type) {
        case 0:
            lbl_1_bss_3B0[b->paramB].s7A++;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            break;
    }
    return 0;
}

void fn_1_ADC8(MGACTOR *actor, int param)
{
    int unused = actor->param;
}

void fn_1_ADDC(void)
{
    f32 dx;
    f32 dz;
    f32 dist;
    f32 unusedF = 45.0f;
    s16 unused0 = 0;
    s16 unused1 = 0;
    s16 unused2 = 0;
    s16 unused3 = 0;
    s16 unused4 = 0;
    s16 i;
    s16 j;
    s16 diff;
    s32 count;
    HuVecF *hitP;
    HuVecF pos;
    HuVecF hitPos;

    for (i = 0; i < 10; i++) {
        if (lbl_1_bss_158[i].active != 0 && lbl_1_bss_158[i].s0A == 0) {
            for (j = 0; j < 10; j++) {
                if (lbl_1_bss_158[j].active != 0 && lbl_1_bss_158[j].s0A == 0 && i != j) {
                    dx = lbl_1_bss_158[j].pos.x - lbl_1_bss_158[i].pos.x;
                    dz = lbl_1_bss_158[j].pos.z - lbl_1_bss_158[i].pos.z;
                    dist = sqrt((dx * dx) + (dz * dz));
                    if (dist <= lbl_1_bss_158[i].radius + lbl_1_bss_158[j].radius) {
                        lbl_1_bss_158[i].s0A = 1;
                        lbl_1_bss_158[j].s0A = 1;
                        dx *= 0.5f;
                        dz *= 0.5f;
                        pos.x = dx + lbl_1_bss_158[i].pos.x;
                        pos.y = 0.0f;
                        pos.z = dz + lbl_1_bss_158[i].pos.z;
                        diff = lbl_1_bss_158[j].level - lbl_1_bss_158[i].level;
                        if (diff > 0) {
                            count = diff;
                        } else {
                            count = diff * -1;
                        }
                        diff = count;
                        hitPos = pos;
                        hitP = &hitPos;
                        fn_1_9324(hitP, diff);
                    }
                }
            }
        }
    }
    for (i = 0; i < 2; i++) {
        dx = -lbl_1_bss_3B0[i].pos.x;
        dz = -lbl_1_bss_3B0[i].pos.z;
        dist = sqrt((dx * dx) + (dz * dz));
        if (dist > lbl_1_data_498 && lbl_1_bss_3B0[i].s14 == 0) {
            lbl_1_bss_3B0[i].f44 = lbl_1_bss_3B0[i].pos.x;
            lbl_1_bss_3B0[i].f4C = lbl_1_bss_3B0[i].pos.z;
            dist = sqrt((lbl_1_bss_3B0[i].f44 * lbl_1_bss_3B0[i].f44) + (lbl_1_bss_3B0[i].f4C * lbl_1_bss_3B0[i].f4C));
            lbl_1_bss_3B0[i].f44 = 10.0f * (lbl_1_bss_3B0[i].f44 / dist);
            lbl_1_bss_3B0[i].f4C = 10.0f * (lbl_1_bss_3B0[i].f4C / dist);
            lbl_1_bss_3B0[i].s14 = 1;
        }
        if (lbl_1_bss_580.s22 == 0) {
            for (j = 0; j < 10; j++) {
                if (lbl_1_bss_158[j].active != 0 && lbl_1_bss_158[j].s0A == 0 && i != lbl_1_bss_158[j].side) {
                    dx = lbl_1_bss_158[j].pos.x - lbl_1_bss_3B0[i].pos.x;
                    dz = lbl_1_bss_158[j].pos.z - lbl_1_bss_3B0[i].pos.z;
                    dist = sqrt((dx * dx) + (dz * dz));
                    if (dist <= 45.0f + lbl_1_bss_158[j].radius) {
                        lbl_1_bss_158[j].s0A = 1;
                        if (lbl_1_bss_3B0[i].s6A == 0 && lbl_1_bss_3B0[i].s6C == 0) {
                            lbl_1_bss_3B0[i].f64 = lbl_1_bss_158[j].angle;
                            lbl_1_bss_3B0[i].s68 = lbl_1_bss_158[j].level;
                            lbl_1_bss_3B0[i].s6A = 1;
                        }
                    }
                }
            }
            if (lbl_1_bss_3B0[i].s6A == 0 && lbl_1_bss_3B0[i].s6C == 0) {
                for (j = 0; j < 10; j++) {
                    if (lbl_1_bss_18[j].active != 0) {
                        dx = lbl_1_bss_18[j].pos.x - lbl_1_bss_3B0[i].pos.x;
                        dz = lbl_1_bss_18[j].pos.z - lbl_1_bss_3B0[i].pos.z;
                        dist = sqrt((dx * dx) + (dz * dz));
                        if (dist <= lbl_1_bss_18[j].fx0C) {
                            dx = 180.0 * (acos(dx / (0.1f + dist)) / 3.141592653589793);
                            dz = -(180.0 * (asin(dz / (0.1f + dist)) / 3.141592653589793));
                            if (dz < 0.0f) {
                                dx = 360.0f - dx;
                            }
                            lbl_1_bss_3B0[i].f64 = dx - 90.0f;
                            lbl_1_bss_3B0[i].s68 = lbl_1_bss_18[j].type;
                            lbl_1_bss_3B0[i].s6C = 1;
                        }
                    }
                }
            }
        }
    }
}

void fn_1_BF34(s16 player)
{
    f32 speedTbl[4] = { 6.2222223f, 7.0f, 8.0f, 10.0f };
    s16 tblA[4] = { 10, 8, 5, 1 };
    s16 tblB[4] = { 5, 7, 8, 10 };
    s16 tblC[4] = { 20, 30, 70, 100 };
    s16 tblD[4] = { 20, 10, 3, 0 };
    s16 tblE[4] = { 0, 10, 40, 60 };
    s16 tblF[4] = { 20, 50, 80, 100 };
    s16 tblG[4] = { 20, 5, 0, 0 };

    if (lbl_1_bss_15F4->chars[player].cpuLevel != 0) {
        lbl_1_bss_3B0[player].s6E = 1;
        lbl_1_bss_3B0[player].s70 = lbl_1_bss_15F4->chars[player].cpuLevel - 1;
        lbl_1_bss_3B0[player].fC4 = speedTbl[lbl_1_bss_3B0[player].s70];
        lbl_1_bss_3B0[player].chaseSpeed = speedTbl[lbl_1_bss_3B0[player].s70];
        lbl_1_bss_3B0[player].chaseRadius = 10.0f;
        lbl_1_bss_3B0[player].sC8 = tblA[lbl_1_bss_3B0[player].s70];
        lbl_1_bss_3B0[player].sCA = tblB[lbl_1_bss_3B0[player].s70];
        lbl_1_bss_3B0[player].sCC = tblC[lbl_1_bss_3B0[player].s70];
        lbl_1_bss_3B0[player].sCE = tblD[lbl_1_bss_3B0[player].s70];
        lbl_1_bss_3B0[player].sD0 = tblE[lbl_1_bss_3B0[player].s70];
        lbl_1_bss_3B0[player].sD2 = tblF[lbl_1_bss_3B0[player].s70];
        lbl_1_bss_3B0[player].sD4 = tblG[lbl_1_bss_3B0[player].s70];
        fn_1_C418(player);
    }
}

s32 fn_1_C360(s16 player)
{
    s32 ret = 0;
    s16 unused = 0;

    lbl_1_bss_3B0[player].btnC = 0;
    lbl_1_bss_3B0[player].btnD = 0;
    lbl_1_bss_3B0[player].btnB = 0;
    lbl_1_bss_3B0[player].btnA = 0;
    fn_1_FDB0(player);
    return ret;
}

s32 fn_1_C418(s16 player)
{
    s32 ret = 0;
    s16 chased = 0;

    if (lbl_1_bss_3B0[player].s6A == 0 && lbl_1_bss_3B0[player].s6C == 0) {
        switch (lbl_1_bss_3B0[player].s7C) {
            case 0:
                MgPlayerComStkOn(lbl_1_bss_0.player[player]);
                lbl_1_bss_3B0[player].s7C++;
                break;
            case 1:
                if (lbl_1_bss_3B0[player].s28 == 0) {
                    lbl_1_bss_3B0[player].chaseSpeed = lbl_1_bss_3B0[player].fC4 * lbl_1_data_484[lbl_1_bss_3B0[player].s60];
                    chased = MgPlayerVecChase(lbl_1_bss_0.player[player], &lbl_1_bss_3B0[player].target, lbl_1_bss_3B0[player].chaseSpeed, lbl_1_bss_3B0[player].chaseRadius);
                    if (chased != 0) {
                        ret = 1;
                    }
                }
                break;
            case 2:
                MgPlayerComStkOff(lbl_1_bss_0.player[player]);
                lbl_1_bss_3B0[player].s7C = 0;
                ret = 1;
                break;
        }
    }
    return ret;
}

void fn_1_C64C(s16 player)
{
    f32 dist;
    HuVecF dir;
    HuVecF aim;

    aim.x = (lbl_1_bss_3B0[player ^ 1].velX / 5.6f) * lbl_1_bss_3B0[player].fC0;
    aim.z = -((lbl_1_bss_3B0[player ^ 1].velZ / 5.6f) * lbl_1_bss_3B0[player].fC0);
    aim.x += lbl_1_bss_3B0[player ^ 1].pos.x;
    aim.z += lbl_1_bss_3B0[player ^ 1].pos.z;
    dir.x = aim.x - lbl_1_bss_3B0[player].pos.x;
    dir.z = aim.z - lbl_1_bss_3B0[player].pos.z;
    dist = sqrt((dir.x * dir.x) + (dir.z * dir.z));
    if (dist != 0.0f) {
        dir.x = lbl_1_bss_3B0[player].pos.x + (20.0f * (dir.x / dist));
        dir.z = lbl_1_bss_3B0[player].pos.z + (20.0f * (dir.z / dist));
    } else {
        dir.x = lbl_1_bss_3B0[player].pos.x;
        dir.z = lbl_1_bss_3B0[player].pos.z;
    }
    lbl_1_bss_3B0[player].target.x = dir.x;
    lbl_1_bss_3B0[player].target.y = lbl_1_bss_3B0[player].pos.y;
    lbl_1_bss_3B0[player].target.z = dir.z;
}

void fn_1_CA38(s16 player, s16 objNo)
{
    f32 dist;
    HuVecF dir;
    s16 i;

    dir.x = lbl_1_bss_158[objNo].pos.x;
    dir.z = lbl_1_bss_158[objNo].pos.z;
    for (i = 0; i < 5; i++) {
        dir.x += lbl_1_bss_158[objNo].dir.x;
        dir.z += lbl_1_bss_158[objNo].dir.z;
    }
    dir.x -= lbl_1_bss_3B0[player].pos.x;
    dir.z -= lbl_1_bss_3B0[player].pos.z;
    dist = sqrt((dir.x * dir.x) + (dir.z * dir.z));
    if (dist != 0.0f) {
        dir.x = lbl_1_bss_3B0[player].pos.x + (20.0f * (dir.x / dist));
        dir.z = lbl_1_bss_3B0[player].pos.z + (20.0f * (dir.z / dist));
    } else {
        dir.x = lbl_1_bss_3B0[player].pos.x;
        dir.z = lbl_1_bss_3B0[player].pos.z;
    }
    lbl_1_bss_3B0[player].target.x = dir.x;
    lbl_1_bss_3B0[player].target.y = lbl_1_bss_3B0[player].pos.y;
    lbl_1_bss_3B0[player].target.z = dir.z;
}

s32 fn_1_CDD8(s16 player)
{
    f32 ang;
    f32 radius;
    s32 ret = 0;
    HuVecF pos;

    pos = lbl_1_bss_3B0[player].pos;
    do {
        ang = frandmod(360);
        radius = frandmod(395);
        pos.x = radius * cos((3.141592653589793 * ang) / 180.0);
        pos.z = radius * sin((3.141592653589793 * ang) / 180.0);
    } while (!((f32)sqrt((pos.x * pos.x) + (pos.z * pos.z)) < lbl_1_data_498));
    lbl_1_bss_3B0[player].target = pos;
    ret = 1;
    return ret;
}
