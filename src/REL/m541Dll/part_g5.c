#include "REL/m541Dll.h"

#include <math.h>

/* model table entry of leaf k (first model of its triangle) */
#define MDL(k) (lbl_1_bss_3730->model[lbl_1_bss_36A4[k].a])
/* model table entry of the k-th leaf of the board path */
#define MDLN(k) (lbl_1_bss_3730->model[lbl_1_bss_36A4[lbl_1_bss_2F0.nextLeaf[k]].a])

s32 fn_1_DDCC(void)
{
    M541_MODELWORK *workA = &lbl_1_bss_3730->work[0];
    M541_MODELWORK *workB = &lbl_1_bss_3730->work[1];
    s32 ret = 0;
    s16 flag = 1;
    s16 i;
    float tbl[32] = {
        0.0f, -200.0f, 200.0f, -120.0f, -200.0f, 100.0f, 0.0f, 220.0f,
        300.0f, 0.0f, -300.0f, 0.0f, 0.0f, -300.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
    };

    switch (lbl_1_bss_2734.mode) {
        case 0:
            fn_1_2614();
            fn_1_270C(1);
            lbl_1_bss_2734.mode++;
            break;
        case 1:
            fn_1_92FC(0);
            if (lbl_1_bss_2F0.mode == 5) {
                workA->motNo = 4;
                workA->speed = 1.0f;
                workA->time = 0.0f;
                workA->attrReset = 0;
                workA->attr = 0x40000001;
                workB->motNo = 4;
                workB->speed = 1.0f;
                workB->time = 0.0f;
                workB->attrReset = 0;
                workB->attr = 0x40000001;
                workA->nextF = 1;
                workA->nextMotNo = 0;
                workA->nextTime = 1.0f;
                workA->nextAttr = 0x40000001;
                workA->nextAttrReset = 0;
                workB->nextF = 1;
                workB->nextMotNo = 0;
                workB->nextTime = 1.0f;
                workB->nextAttr = 0x40000001;
                workB->nextAttrReset = 0;
                lbl_1_bss_2734.mode++;
            }
            break;
        case 2:
            if ((s16)fn_1_92FC(0) != 0) {
                lbl_1_bss_2F0.count = 0;
                lbl_1_bss_2734.mode++;
            }
            break;
        case 3:
            for (i = 0; i < 7; i++) {
                flag = (s16)(flag * fn_1_A9B8(lbl_1_bss_2F0.spawnCount + i, 2, 0.0f, 0.0f));
            }
            if (flag != 0) {
                lbl_1_bss_2F0.spawnCount += 7;
                lbl_1_bss_2734.mode++;
            }
            break;
        case 4:
        default:
            break;
    }
    return ret;
}

s32 fn_1_E0A4(void)
{
    HU3DMODEL *mdl = &Hu3DData[lbl_1_bss_3730->model[(lbl_1_bss_2734.cpuPlayer & 1) + 0x6A].modelId];
    M541_MODELWORK *work = &lbl_1_bss_3730->work[lbl_1_bss_2734.cpuPlayer & 1];
    s32 ret = 0;
    s32 done = 0;
    s16 count;
    s16 i;
    M541_MODEL *m0;
    M541_MODEL *m1;
    M541_MODEL *m2;
    float rate;

    switch (lbl_1_bss_2734.mode) {
        case 0:
            for (i = 6; i <= 0x23; i++) {
                Hu3DModelAttrSet(lbl_1_bss_3730->model[i].modelId, HU3D_ATTR_DISPOFF);
            }
            for (i = 0xF; i < 0x17; i++) {
                lbl_1_bss_20[i].tpLvl = 1.0f;
                lbl_1_bss_20[i].exist = 1;
            }
            for (i = 0; i < 0x17; i++) {
                if (lbl_1_bss_20[i].exist != 0) {
                    m0 = &MDL(i);
                    lbl_1_bss_20[i].vel.y = 200.0f;
                    lbl_1_bss_20[i].tpLvl = 1.0f;
                }
            }
            fn_1_29EC(0);
            fn_1_29EC(1);
            fn_1_29EC(2);
            lbl_1_bss_2734.mode++;
            break;
        case 1:
            count = 0;
            for (i = 0; i < 0x17; i++) {
                if (lbl_1_bss_20[i].exist != 0) {
                    count++;
                    m0 = &MDL(i);
                    m1 = &lbl_1_bss_3730->model[lbl_1_bss_36A4[i].b];
                    m2 = &lbl_1_bss_3730->model[lbl_1_bss_36A4[i].c];
                    rate = 60.0f;
                    done += fn_1_1850(&m0->posY, rate, lbl_1_bss_20[i].vel.y, 0.0f);
                    done += fn_1_1850(&m1->posY, rate, lbl_1_bss_20[i].vel.y, 0.0f);
                    done += fn_1_1850(&m2->posY, rate, lbl_1_bss_20[i].vel.y, 0.0f);
                    if ((lbl_1_bss_20[i].tpLvl -= 0.02f) <= 0.0f) {
                        lbl_1_bss_20[i].tpLvl = 0.0f;
                    } else {
                        done = 1;
                    }
                    Hu3DModelTPLvlSet(m0->modelId, lbl_1_bss_20[i].tpLvl);
                    Hu3DModelTPLvlSet(m1->modelId, lbl_1_bss_20[i].tpLvl);
                    Hu3DModelTPLvlSet(m2->modelId, lbl_1_bss_20[i].tpLvl);
                    if ((s16)done == 0) {
                        lbl_1_bss_20[i].unk0 = 0;
                        lbl_1_bss_20[i].exist = 0;
                        Hu3DModelAttrSet(m0->modelId, HU3D_ATTR_DISPOFF);
                        Hu3DModelAttrSet(m1->modelId, HU3D_ATTR_DISPOFF);
                        Hu3DModelAttrSet(m2->modelId, HU3D_ATTR_DISPOFF);
                    }
                }
            }
            if (count != 0) {
                if (lbl_1_bss_36A0++ <= 0x50) {
                    break;
                }
            }
            for (i = 6; i <= 0x68; i++) {
                Hu3DModelAttrSet(lbl_1_bss_3730->model[i].modelId, HU3D_ATTR_DISPOFF);
            }
            lbl_1_bss_2734.mode++;
            break;
        case 2:
            if (lbl_1_bss_2734.cpuPlayer != 0) {
                if (mdl->rot.y <= 1.0f && mdl->rot.y >= -1.0f) {
                    lbl_1_bss_2734.mode = 4;
                } else {
                    lbl_1_bss_2734.unk8 = mdl->rot.y;
                    work->shiftMode = 1;
                    work->shiftMotNo = 1;
                    work->shiftSpeed = 0.0f;
                    work->shiftTime = 4.0f;
                    work->shiftAttr = 0;
                    work->attr = 0x40000001;
                    lbl_1_bss_2734.mode = 3;
                }
            } else {
                lbl_1_bss_2734.mode = 5;
            }
            break;
        case 3:
            rate = 30.0f;
            done += fn_1_1850(&mdl->rot.y, rate, lbl_1_bss_2734.unk8, 0.0f);
            if ((s16)done == 0) {
                work->shiftMode = 1;
                work->shiftMotNo = 0;
                work->shiftSpeed = 0.0f;
                work->shiftTime = 4.0f;
                work->shiftAttr = 0;
                work->attr = 0x40000001;
                lbl_1_bss_2734.mode++;
            }
            break;
        case 4:
            rate = 30.0f;
            done += fn_1_1850(&lbl_1_bss_1C->center.x, rate, lbl_1_data_1F8[1].center.x, mdl->pos.x);
            done += fn_1_1850(&lbl_1_bss_1C->center.y, rate, lbl_1_data_1F8[1].center.y, 70.0f + mdl->pos.y);
            done += fn_1_1850(&lbl_1_bss_1C->center.z, rate, lbl_1_data_1F8[1].center.z, mdl->pos.z);
            done += fn_1_1850(&lbl_1_bss_1C->rot.x, rate, lbl_1_data_1F8[1].rot.x, -18.0f);
            done += fn_1_1850(&lbl_1_bss_1C->zoom, rate, lbl_1_data_1F8[1].zoom, 500.0f);
            if ((s16)done == 0) {
                lbl_1_bss_2734.mode++;
            }
            break;
        case 5:
            ret = 1;
            break;
        default:
            break;
    }
    return ret;
}

int fn_1_E900(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b)
{
    switch (b->type) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
    }
    return 0;
}

void fn_1_E91C(COLBODY *body, void *user)
{
    s32 temp = body->param.paramB;
}

void fn_1_E930(void)
{
    s16 unused0 = 0;
    s16 unused1 = 0;
    s16 unused2 = 0;
    s16 i;
    s16 j;
    float a = 30.0f;
    float b = 30.000002f;
    float c = 10.0f;
    float tbl[8] = { 90.0f, 180.0f, -90.0f, 0.0f, 90.0f, -180.0f, -90.0f, 0.0f };
    float dx;
    float dz;
    float dist;
    float len2;
    float t0;
    float cx;
    float scale;
    float cz;
    float inside;
    float t;

    for (i = 0; i < 15; i++) {
        if (lbl_1_bss_20[i].unk0 == 0 || lbl_1_bss_20[i].hidden != 0) {
            continue;
        }
        for (j = 0; j < 15; j++) {
            if (lbl_1_bss_20[j].unk0 == 0 || i == j) {
                continue;
            }
            dx = MDL(j).posX - MDL(i).posX;
            dz = MDL(j).posZ - MDL(i).posZ;
            dist = sqrt(dx * dx + dz * dz);
            if (dist < a + b) {
                if (dist <= 1.0f) {
                    dist = 1.0f;
                }
                scale = (10.0f + (a + b)) / dist;
                dx *= scale;
                dz *= scale;
                MDL(i).posX = MDL(j).posX - dx;
                MDL(i).posZ = MDL(j).posZ - dz;
                dist = sqrt((MDL(j).posX - MDL(i).posX) * (MDL(j).posX - MDL(i).posX)
                             + (MDL(j).posZ - MDL(i).posZ) * (MDL(j).posZ - MDL(i).posZ));
                if (dist <= 1.0f) {
                    dist = 1.0f;
                }
                t0 = 0.0f;
                len2 = t0;
                len2 = (MDL(j).posZ - MDL(i).posZ) / dist;
                t0 = (MDL(j).posX - MDL(i).posX) / dist;
                if (0.0f == len2 && 0.0f == t0) {
                    len2 = 1.0f;
                    t0 = 0.0f;
                }
                dx = lbl_1_bss_20[j].vel.x * t0 + lbl_1_bss_20[j].vel.z * len2;
                dz = -lbl_1_bss_20[i].vel.x * len2 + lbl_1_bss_20[i].vel.z * t0;
                cx = lbl_1_bss_20[i].vel.x * t0 + lbl_1_bss_20[i].vel.z * len2;
                cz = -lbl_1_bss_20[j].vel.x * len2 + lbl_1_bss_20[j].vel.z * t0;
                lbl_1_bss_20[i].vel.x = dx * t0 - dz * len2;
                lbl_1_bss_20[i].vel.z = dz * t0 + dx * len2;
                lbl_1_bss_20[j].vel.x = cx * t0 - cz * len2;
                lbl_1_bss_20[j].vel.z = cz * t0 + cx * len2;
                dx = sqrt(lbl_1_bss_20[i].vel.x * lbl_1_bss_20[i].vel.x + lbl_1_bss_20[i].vel.z * lbl_1_bss_20[i].vel.z);
                dz = sqrt(lbl_1_bss_20[j].vel.x * lbl_1_bss_20[j].vel.x + lbl_1_bss_20[j].vel.z * lbl_1_bss_20[j].vel.z);
                if (dx != 0.0f) {
                    dx = lbl_1_bss_2F0.length / dx;
                } else {
                    dx = 0.0f;
                }
                if (dz != 0.0f) {
                    dz = lbl_1_bss_2F0.length / dz;
                } else {
                    dz = 0.0f;
                }
                lbl_1_bss_20[i].vel.x *= dx;
                lbl_1_bss_20[i].vel.z *= dx;
                lbl_1_bss_20[j].vel.x *= dz;
                lbl_1_bss_20[j].vel.z *= dz;

                dx = 180.0 * (acos(lbl_1_bss_20[i].vel.x / (0.1f + lbl_1_bss_2F0.length)) / 3.141592653589793);
                dz = 180.0 * (asin(lbl_1_bss_20[i].vel.z / (0.1f + lbl_1_bss_2F0.length)) / 3.141592653589793);
                if (dz < 0.0f) {
                    dx = 360.0f - dx;
                }
                lbl_1_bss_20[i].angle = dx;
                lbl_1_bss_20[i].vel.x = lbl_1_bss_2F0.length * cos(3.141592653589793 * lbl_1_bss_20[i].angle / 180.0);
                lbl_1_bss_20[i].vel.y = 0.0f;
                lbl_1_bss_20[i].vel.z = lbl_1_bss_2F0.length * sin(3.141592653589793 * lbl_1_bss_20[i].angle / 180.0);
                if (lbl_1_bss_20[j].hidden == 0) {
                    dx = 180.0 * (acos(lbl_1_bss_20[j].vel.x / (0.1f + lbl_1_bss_2F0.length)) / 3.141592653589793);
                    dz = 180.0 * (asin(lbl_1_bss_20[j].vel.z / (0.1f + lbl_1_bss_2F0.length)) / 3.141592653589793);
                    if (dz < 0.0f) {
                        dx = 360.0f - dx;
                    }
                    lbl_1_bss_20[j].angle = dx;
                    lbl_1_bss_20[j].vel.x = lbl_1_bss_2F0.length * cos(3.141592653589793 * lbl_1_bss_20[j].angle / 180.0);
                    lbl_1_bss_20[j].vel.y = 0.0f;
                    lbl_1_bss_20[j].vel.z = lbl_1_bss_2F0.length * sin(3.141592653589793 * lbl_1_bss_20[j].angle / 180.0);
                }
            }
        }

        dx = MDL(i).posX;
        dz = MDL(i).posZ;
        for (j = 0; j < 8; j++) {
            dx = lbl_1_data_368[j][0] - lbl_1_data_368[j][2];
            dz = lbl_1_data_368[j][1] - lbl_1_data_368[j][3];
            len2 = dx * dx + dz * dz;
            t0 = -1.0f * (dx * (lbl_1_data_368[j][0] - MDL(i).posX) + dz * (lbl_1_data_368[j][1] - MDL(i).posZ));
            if (len2 != 0.0f) {
                t = t0 / len2;
            } else {
                t = 0.0f;
            }
            cx = dx * t + lbl_1_data_368[j][0];
            cz = dz * scale + lbl_1_data_368[j][1];
            dx = MDL(i).posX - cx;
            dz = MDL(i).posZ - cz;
            dist = sqrt(dx * dx + dz * dz);
            if (dist < b) {
                if (dist <= 1.0f) {
                    dist = 1.0f;
                }
                scale = b / dist;
                dx *= scale;
                dz *= scale;
                MDL(i).posX = cx + dx;
                MDL(i).posZ = cz + dz;
                lbl_1_bss_20[i].angle = 360.0f - lbl_1_bss_20[i].angle;
                lbl_1_bss_20[i].angle += tbl[j];
                if (lbl_1_bss_20[i].angle >= 360.0f) {
                    lbl_1_bss_20[i].angle -= 360.0f;
                }
                lbl_1_bss_20[i].vel.x = lbl_1_bss_2F0.length * cos(3.141592653589793 * lbl_1_bss_20[i].angle / 180.0);
                lbl_1_bss_20[i].vel.y = 0.0f;
                lbl_1_bss_20[i].vel.z = lbl_1_bss_2F0.length * sin(3.141592653589793 * lbl_1_bss_20[i].angle / 180.0);
            }
        }
    }

    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_2614[i].stunMode == 2) {
            continue;
        }
        for (j = 0; j < 15; j++) {
            if (lbl_1_bss_20[j].unk0 == 0) {
                continue;
            }
            dx = MDL(j).posX - lbl_1_bss_2614[i].pos.x;
            dz = MDL(j).posZ - lbl_1_bss_2614[i].pos.z;
            dist = sqrt(dx * dx + dz * dz);
            if (dist < a + b) {
                if (dist <= 1.0f) {
                    dist = 1.0f;
                }
                scale = (a + b) / dist;
                dx *= scale;
                dz *= scale;
                lbl_1_bss_2614[i].stunMode = 2;
            }
        }
        if (lbl_1_bss_2F0.ready != 0) {
            for (j = 0; j < lbl_1_bss_2F0.count + 1; j++) {
                if (MDLN(j).posX < MDLN(j + 1).posX) {
                    dx = MDLN(j).posX;
                    cx = MDLN(j + 1).posX;
                } else {
                    dx = MDLN(j + 1).posX;
                    cx = MDLN(j).posX;
                }
                if (MDLN(j).posZ < MDLN(j + 1).posZ) {
                    dz = MDLN(j).posZ;
                    cz = MDLN(j + 1).posZ;
                } else {
                    dz = MDLN(j + 1).posZ;
                    cz = MDLN(j).posZ;
                }
                if (a + lbl_1_bss_2614[i].pos.x >= dx
                    && lbl_1_bss_2614[i].pos.x - a <= cx
                    && a + lbl_1_bss_2614[i].pos.z >= dz
                    && lbl_1_bss_2614[i].pos.z - a <= cz) {
                    dx = MDLN(j).posX - MDLN(j + 1).posX;
                    dz = MDLN(j).posZ - MDLN(j + 1).posZ;
                    len2 = dx * dx + dz * dz;
                    t0 = -1.0f * (dx * (MDLN(j).posX - lbl_1_bss_2614[i].pos.x)
                                 + dz * (MDLN(j).posZ - lbl_1_bss_2614[i].pos.z));
                    if (len2 != 0.0f) {
                        t = t0 / len2;
                    } else {
                        t = 0.0f;
                    }
                    cx = dx * t + MDLN(j).posX;
                    cz = dz * t + MDLN(j).posZ;
                    dx = lbl_1_bss_2614[i].pos.x - cx;
                    dz = lbl_1_bss_2614[i].pos.z - cz;
                    dist = sqrt(dx * dx + dz * dz);
                    if (dist < a + c) {
                        lbl_1_bss_2614[i].stunMode = 2;
                    }
                }
            }
        }
        dx = lbl_1_bss_2614[i].pos.x;
        dz = lbl_1_bss_2614[i].pos.z;
        inside = -1.0f;
        if (-dx + dz > -784.0f && dx < 554.0f && dx + dz < 784.0f && dz < 554.0f
            && -dx + dz < 784.0f && dx > -554.0f && dx + dz > -784.0f && dz > -554.0f) {
            inside = 1.0f;
        }
        for (j = 0; j < 8; j++) {
            dx = lbl_1_data_368[j][0] - lbl_1_data_368[j][2];
            dz = lbl_1_data_368[j][1] - lbl_1_data_368[j][3];
            len2 = dx * dx + dz * dz;
            t0 = -1.0f * (dx * (lbl_1_data_368[j][0] - lbl_1_bss_2614[i].pos.x)
                         + dz * (lbl_1_data_368[j][1] - lbl_1_bss_2614[i].pos.z));
            if (len2 != 0.0f) {
                t = t0 / len2;
            } else {
                t = 0.0f;
            }
            cx = dx * t + lbl_1_data_368[j][0];
            cz = dz * scale + lbl_1_data_368[j][1];
            dx = lbl_1_bss_2614[i].pos.x - cx;
            dz = lbl_1_bss_2614[i].pos.z - cz;
            dist = sqrt(dx * dx + dz * dz);
            if (dist < a) {
                if (dist <= 1.0f) {
                    dist = 1.0f;
                }
                scale = (20.0f + a) / dist;
                dx = inside * (dx * scale);
                dz = inside * (dz * scale);
                lbl_1_bss_2614[i].pos.x = cx + dx;
                lbl_1_bss_2614[i].pos.z = cz + dz;
                lbl_1_bss_2614[i].stunMode = 2;
            }
        }
    }

    if (lbl_1_bss_2734.cpuPlayer == 0 && lbl_1_bss_2614[0].stunMode == 2 && lbl_1_bss_2614[1].stunMode == 2) {
        i = frandmod(2);
        lbl_1_bss_2614[i].stunMode = 2;
        lbl_1_bss_2614[i ^ 1].stunMode = 0;
    }
}
