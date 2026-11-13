#include "REL/m541Dll.h"

#include "math.h"

s32 fn_1_7DE8(s16 arg0)
{
    M541_MODELWORK *work = &lbl_1_bss_3730->work[arg0];
    float offZ = -30.0f;
    s32 ret = 0;
    HuVecF pos;

    switch (lbl_1_bss_2614[arg0].sub) {
        case 0:
            if (lbl_1_bss_2734.cpuPlayer == 0) {
                omVibrate(lbl_1_bss_3730->player[arg0].playerNo, 30, 6, 6);
            }
            work->motNo = 5;
            work->speed = 1.0f;
            work->time = 0.0f;
            work->attrReset = 0;
            work->attr = 0x40000001;
            lbl_1_bss_2614[arg0].sub++;
            break;
        case 1:
            if (work->time > 10.0f) {
                Hu3DModelAttrReset(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 1);
                Hu3DModelAttrSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 0x40000001);
                Hu3DMotionTimeSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 0.0f);
                Hu3DMotionSpeedSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 4.0f);
                Hu3DModelObjPosGet(lbl_1_bss_3730->model[arg0 + 0x6A].modelId, lbl_1_bss_2614[arg0].hook, &pos);
                lbl_1_bss_3730->model[arg0 + 0x1A].posX = pos.x;
                lbl_1_bss_3730->model[arg0 + 0x1A].posY = pos.y;
                lbl_1_bss_3730->model[arg0 + 0x1A].posZ = pos.z + offZ;
                lbl_1_bss_2614[arg0].sub++;
            }
            break;
        case 2:
            Hu3DModelObjPosGet(lbl_1_bss_3730->model[arg0 + 0x6A].modelId, lbl_1_bss_2614[arg0].hook, &pos);
            lbl_1_bss_3730->model[arg0 + 0x1A].posX = pos.x;
            lbl_1_bss_3730->model[arg0 + 0x1A].posY = pos.y;
            lbl_1_bss_3730->model[arg0 + 0x1A].posZ = pos.z + offZ;
            if (work->time >= 80.0f) {
                Hu3DModelAttrReset(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 0x40000001);
                Hu3DModelAttrSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 1);
                Hu3DMotionTimeSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 0.0f);
                Hu3DMotionSpeedSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 0.0f);
                lbl_1_bss_2614[arg0].sub++;
            }
            break;
        case 3:
            if (work->endCheck != 0) {
                lbl_1_bss_2614[arg0].sub++;
            }
            break;
        case 4:
            work->motNo = 0;
            work->speed = 1.0f;
            work->time = 0.0f;
            work->attrReset = 0x40000001;
            work->attr = 0;
            lbl_1_bss_2614[arg0].sub = 0;
            ret = 1;
            break;
    }
    return ret;
}

s32 fn_1_832C(s16 arg0)
{
    HU3DMODEL *mdl = &Hu3DData[lbl_1_bss_3730->model[arg0 + 0x6A].modelId];
    M541_MODELWORK *work = &lbl_1_bss_3730->work[arg0];
    float offZ = -30.0f;
    s32 ret = 0;
    s16 tmp = 0;
    HuVecF pos;
    HuVecF vec;

    switch (lbl_1_bss_2614[arg0].motion) {
        case 0: {
            s32 y, x;
            float fy, fx;
            omVibrate(lbl_1_bss_3730->player[arg0].playerNo, 30, 12, 0);
            fy = 0.026956521f * mdl->pos.z;
            fx = 0.055652175f * mdl->pos.x;
            y = 111.0f + fy;
            x = 64.0f + fx;
            fn_1_2644(1, 100);
            fn_1_27C8(3, y, x);
            work->motNo = 5;
            work->speed = 1.0f;
            work->time = 0.0f;
            work->attrReset = 0;
            work->attr = 0x40000001;
            lbl_1_bss_2614[arg0].motion++;
            break;
        }
        case 1:
            if (work->time > 10.0f) {
                Hu3DModelAttrReset(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 1);
                Hu3DModelAttrSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 0x40000001);
                Hu3DMotionTimeSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 0.0f);
                Hu3DMotionSpeedSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 4.0f);
                Hu3DModelObjPosGet(lbl_1_bss_3730->model[arg0 + 0x6A].modelId, lbl_1_bss_2614[arg0].hook, &pos);
                lbl_1_bss_3730->model[arg0 + 0x1A].posX = pos.x;
                lbl_1_bss_3730->model[arg0 + 0x1A].posY = pos.y;
                lbl_1_bss_3730->model[arg0 + 0x1A].posZ = pos.z + offZ;
                lbl_1_bss_2614[arg0].tick = 0;
                lbl_1_bss_2614[arg0].anim = 0;
                lbl_1_bss_2614[arg0].motion++;
            }
            break;
        case 2:
            Hu3DModelObjPosGet(lbl_1_bss_3730->model[arg0 + 0x6A].modelId, lbl_1_bss_2614[arg0].hook, &pos);
            lbl_1_bss_3730->model[arg0 + 0x1A].posX = pos.x;
            lbl_1_bss_3730->model[arg0 + 0x1A].posY = pos.y;
            lbl_1_bss_3730->model[arg0 + 0x1A].posZ = pos.z + offZ;
            if (lbl_1_bss_2614[arg0].tick++ == 0) {
                fn_1_2378(arg0, 0.25f);
            } else if (lbl_1_bss_2614[arg0].tick > 3) {
                if (lbl_1_bss_2614[arg0].tick <= 6) {
                    fn_1_24E8(arg0);
                } else {
                    lbl_1_bss_2614[arg0].anim = 0;
                    lbl_1_bss_2614[arg0].tick = 0;
                }
            }
            if (work->time >= 86.0f) {
                fn_1_24E8(arg0);
                fn_1_2378(arg0, 0.25f);
                work->shiftMode = 1;
                work->shiftMotNo = 2;
                work->shiftSpeed = 0.0f;
                work->shiftTime = 10.0f;
                work->shiftAttr = 0;
                work->attr = 0x40000001;
                Hu3DModelAttrReset(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 0x40000001);
                Hu3DModelAttrSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 1);
                Hu3DMotionTimeSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 0.0f);
                Hu3DMotionSpeedSet(lbl_1_bss_3730->model[arg0 + 0x1A].modelId, 0.0f);
                lbl_1_bss_2614[arg0].motion++;
            }
            break;
        case 3:
            if (work->shiftMode == 3) {
                tmp = fn_1_B4E0(arg0);
                if (tmp == 0) {
                    work->shiftMode = 1;
                    work->shiftMotNo = 3;
                    work->shiftSpeed = 0.0f;
                    work->shiftTime = 8.0f;
                    work->shiftAttr = 0x40000001;
                    work->attr = 0;
                    lbl_1_bss_2614[arg0].motion = 9;
                } else {
                    vec.x = lbl_1_bss_2734.moveX - lbl_1_bss_2614[arg0].pos.x;
                    vec.z = lbl_1_bss_2734.moveZ - lbl_1_bss_2614[arg0].pos.z;
                    lbl_1_bss_2734.velX = vec.x / 20.0f;
                    lbl_1_bss_2734.velY = 10.0f;
                    lbl_1_bss_2734.velZ = vec.z / 20.0f;
                    lbl_1_bss_2734.angAcc = 1.0f;
                    vec.x = lbl_1_bss_2734.goal.x - lbl_1_bss_2734.moveX;
                    vec.z = lbl_1_bss_2734.goal.z - lbl_1_bss_2734.moveZ;
                    lbl_1_bss_2734.angVelX = vec.x * 0.0625f;
                    lbl_1_bss_2734.angVelY = 8.0f;
                    lbl_1_bss_2734.angVelZ = vec.z * 0.0625f;
                    lbl_1_bss_2734.angCount = 0;
                    lbl_1_bss_2614[arg0].motion++;
                }
            }
            break;
        case 4:
            lbl_1_bss_2614[arg0].pos.x += lbl_1_bss_2734.velX;
            lbl_1_bss_2614[arg0].pos.y += lbl_1_bss_2734.velY;
            lbl_1_bss_2614[arg0].pos.z += lbl_1_bss_2734.velZ;
            lbl_1_bss_2734.velY -= lbl_1_bss_2734.angAcc;
            mdl->pos = lbl_1_bss_2614[arg0].pos;
            MgActorPosUpdate(lbl_1_bss_0.player[arg0 + 1]->actor, &lbl_1_bss_2614[arg0].pos);
            if (++lbl_1_bss_2734.angCount >= 10) {
                lbl_1_bss_2734.velY = 0.0f;
                lbl_1_bss_2734.angCount = 0;
                lbl_1_bss_2614[arg0].motion++;
            }
            break;
        case 5:
            lbl_1_bss_2614[arg0].pos.x += lbl_1_bss_2734.velX;
            lbl_1_bss_2614[arg0].pos.y += lbl_1_bss_2734.velY;
            lbl_1_bss_2614[arg0].pos.z += lbl_1_bss_2734.velZ;
            lbl_1_bss_2734.velY -= lbl_1_bss_2734.angAcc;
            mdl->pos = lbl_1_bss_2614[arg0].pos;
            MgActorPosUpdate(lbl_1_bss_0.player[arg0 + 1]->actor, &lbl_1_bss_2614[arg0].pos);
            if (++lbl_1_bss_2734.angCount >= 10) {
                work->shiftMode = 1;
                work->shiftMotNo = 2;
                work->shiftSpeed = 0.0f;
                work->shiftTime = 10.0f;
                work->shiftAttr = 0;
                work->attr = 0x40000001;
                lbl_1_bss_2734.angCount = 0;
                lbl_1_bss_2614[arg0].motion++;
            }
            break;
        case 6:
            lbl_1_bss_2614[arg0].pos.x += lbl_1_bss_2734.angVelX;
            lbl_1_bss_2614[arg0].pos.y += lbl_1_bss_2734.angVelY;
            lbl_1_bss_2614[arg0].pos.z += lbl_1_bss_2734.angVelZ;
            lbl_1_bss_2734.angVelY -= lbl_1_bss_2734.angAcc;
            mdl->pos = lbl_1_bss_2614[arg0].pos;
            MgActorPosUpdate(lbl_1_bss_0.player[arg0 + 1]->actor, &lbl_1_bss_2614[arg0].pos);
            if (++lbl_1_bss_2734.angCount >= 8) {
                lbl_1_bss_2734.angVelY = 0.0f;
                lbl_1_bss_2734.angCount = 0;
                lbl_1_bss_2614[arg0].motion++;
            }
            break;
        case 7:
            lbl_1_bss_2614[arg0].pos.x += lbl_1_bss_2734.angVelX;
            lbl_1_bss_2614[arg0].pos.y += lbl_1_bss_2734.angVelY;
            lbl_1_bss_2614[arg0].pos.z += lbl_1_bss_2734.angVelZ;
            lbl_1_bss_2734.angVelY -= lbl_1_bss_2734.angAcc;
            mdl->pos = lbl_1_bss_2614[arg0].pos;
            MgActorPosUpdate(lbl_1_bss_0.player[arg0 + 1]->actor, &lbl_1_bss_2614[arg0].pos);
            if (++lbl_1_bss_2734.angCount >= 8) {
                work->shiftMode = 1;
                work->shiftMotNo = 2;
                work->shiftSpeed = 0.0f;
                work->shiftTime = 10.0f;
                work->shiftAttr = 0;
                work->attr = 0x40000001;
                lbl_1_bss_2614[arg0].pos.y = 100.0f;
                mdl->pos.y = 100.0f;
                lbl_1_bss_2734.angCount = 0;
                lbl_1_bss_2614[arg0].motion++;
            }
            break;
        case 8:
            if (work->shiftMode == 3) {
                work->shiftMode = 1;
                work->shiftMotNo = 3;
                work->shiftSpeed = 0.0f;
                work->shiftTime = 8.0f;
                work->shiftAttr = 0x40000001;
                work->attr = 0;
                lbl_1_bss_2614[arg0].motion++;
            }
            break;
        case 9:
            lbl_1_bss_2614[arg0].motion = 0;
            ret = 1;
            break;
    }
    return ret;
}

s32 fn_1_92FC(s16 arg0)
{
    s32 ret = 0;
    float range[11][2] = {
        { 0.0f, 0.0f },     { 400.0f, 650.0f }, { 350.0f, 600.0f }, { 300.0f, 550.0f },
        { 250.0f, 500.0f }, { 200.0f, 450.0f }, { 150.0f, 400.0f }, { 100.0f, 350.0f },
        { 50.0f, 300.0f },  { 0.0f, 250.0f },   { 0.0f, 200.0f },
    };
    s16 found;
    s16 i, j, k;
    s16 rnd;
    float dx, dz, d, ang, angSin;

    switch (lbl_1_bss_2F0.mode) {
        case 0:
            if (arg0 != 0) {
                lbl_1_bss_2F0.timer = lbl_1_bss_45C.aiTimer[lbl_1_bss_2F0.count];
            } else {
                lbl_1_bss_2F0.timer = 120;
            }
            lbl_1_bss_2F0.count++;
            lbl_1_bss_2F0.mode++;
            break;
        case 1: {
            s16 old = lbl_1_bss_2F0.timer;
            lbl_1_bss_2F0.timer--;
            if (old < 0) {
                lbl_1_bss_2F0.timer = 0;
                lbl_1_bss_2F0.mode++;
            }
            if (arg0 != 0) {
                lbl_1_bss_45C.cnt217E++;
            }
            break;
        }
        case 2:
            if (arg0 != 0) {
                lbl_1_bss_45C.cnt217C++;
            }
            for (i = 0; i < 10; i++) {
                lbl_1_bss_2F0.nextLeaf[i] = -1;
            }
            if (arg0 != 0) {
                rnd = lbl_1_bss_45C.aiMode[lbl_1_bss_2F0.count];
            } else {
                rnd = frandmod(8);
            }
            lbl_1_bss_2F0.nextLeaf[0] = rnd + 15;
            for (i = 1; i <= lbl_1_bss_2F0.count; i++) {
                for (j = 0; j < 15; j++) {
                    found = 0;
                    for (k = 0; k < 10; k++) {
                        if (lbl_1_bss_2F0.nextLeaf[k] == j) {
                            found = 1;
                        }
                    }
                    if (found == 0 && lbl_1_bss_20[j].unk0 != 0) {
                        dx = lbl_1_bss_3730->model[lbl_1_bss_36A4[j].a].posX
                            - lbl_1_bss_3730->model[lbl_1_bss_36A4[lbl_1_bss_2F0.nextLeaf[i - 1]].a].posX;
                        dz = lbl_1_bss_3730->model[lbl_1_bss_36A4[j].a].posZ
                            - lbl_1_bss_3730->model[lbl_1_bss_36A4[lbl_1_bss_2F0.nextLeaf[i - 1]].a].posZ;
                        d = sqrt(dx * dx + dz * dz);
                        if (range[lbl_1_bss_2F0.count][0] < d && range[lbl_1_bss_2F0.count][1] > d) {
                            lbl_1_bss_2F0.nextLeaf[i] = j;
                            lbl_1_bss_2F0.dist[i - 1] = d;
                            ang = 180.0 * (acos(dx / (0.1f + d)) / 3.141592653589793);
                            angSin = 180.0 * (asin(dz / (0.1f + d)) / 3.141592653589793);
                            if (angSin < 0.0f) {
                                ang = 360.0f - ang;
                            }
                            lbl_1_bss_2F0.angle[i - 1] = ang;
                            break;
                        }
                    }
                }
                if (lbl_1_bss_2F0.nextLeaf[i] == -1) {
                    found = 1;
                    break;
                }
            }
            if (found == 0) {
                for (j = 15; j < 23; j++) {
                    if (arg0 == 0) {
                        j = (lbl_1_bss_2F0.nextLeaf[0] - 11) % 8 + 15;
                    }
                    if (lbl_1_bss_2F0.nextLeaf[0] != j) {
                        dx = lbl_1_bss_3730->model[lbl_1_bss_36A4[j].a].posX
                            - lbl_1_bss_3730->model[lbl_1_bss_36A4[lbl_1_bss_2F0.nextLeaf[lbl_1_bss_2F0.count]].a].posX;
                        dz = lbl_1_bss_3730->model[lbl_1_bss_36A4[j].a].posZ
                            - lbl_1_bss_3730->model[lbl_1_bss_36A4[lbl_1_bss_2F0.nextLeaf[lbl_1_bss_2F0.count]].a].posZ;
                        d = sqrt(dx * dx + dz * dz);
                        if (range[lbl_1_bss_2F0.count][0] < d && range[lbl_1_bss_2F0.count][1] > d) {
                            lbl_1_bss_2F0.nextLeaf[lbl_1_bss_2F0.count + 1] = j;
                            lbl_1_bss_2F0.dist[lbl_1_bss_2F0.count] = d;
                            ang = 180.0 * (acos(dx / (0.1f + d)) / 3.141592653589793);
                            angSin = 180.0 * (asin(dz / (0.1f + d)) / 3.141592653589793);
                            if (angSin < 0.0f) {
                                ang = 360.0f - ang;
                            }
                            lbl_1_bss_2F0.angle[lbl_1_bss_2F0.count] = ang;
                            break;
                        }
                    }
                }
                for (i = 0; i <= lbl_1_bss_2F0.count + 1; i++) {
                    if (lbl_1_bss_2F0.nextLeaf[i] == -1) {
                        i = 99;
                        for (j = 0; j < 10; j++) {
                            lbl_1_bss_2F0.nextLeaf[j] = -1;
                        }
                    }
                }
                if (lbl_1_bss_2F0.nextLeaf[0] != -1) {
                    lbl_1_bss_2F0.mode++;
                }
            }
            break;
        case 3:
            for (i = 0; i < lbl_1_bss_2F0.count + 2; i++) {
                M541_MODEL *dst = &lbl_1_bss_3730->model[i + 6];
                M541_MODEL *src = &lbl_1_bss_3730->model[lbl_1_bss_36A4[lbl_1_bss_2F0.nextLeaf[i]].a];
                dst->posX = src->posX;
                dst->posY = src->posY;
                dst->posZ = src->posZ;
                lbl_1_bss_3730->model[i + 6].posZ -= 1.0f;
                Hu3DModelAttrReset(lbl_1_bss_3730->model[i + 6].modelId, 1);
                Hu3DModelAttrSet(lbl_1_bss_3730->model[i + 6].modelId, 0x40000001);
                Hu3DMotionTimeSet(lbl_1_bss_3730->model[i + 6].modelId, 0.0f);
                Hu3DMotionSpeedSet(lbl_1_bss_3730->model[i + 6].modelId, 4.0f);
            }
            for (i = 1; i < lbl_1_bss_2F0.count + 1; i++) {
                lbl_1_bss_20[lbl_1_bss_2F0.nextLeaf[i]].hidden = 1;
                lbl_1_bss_20[lbl_1_bss_2F0.nextLeaf[i]].velSave = lbl_1_bss_20[lbl_1_bss_2F0.nextLeaf[i]].vel;
                lbl_1_bss_20[lbl_1_bss_2F0.nextLeaf[i]].vel.x = 0.0f;
                lbl_1_bss_20[lbl_1_bss_2F0.nextLeaf[i]].vel.y = 0.0f;
                lbl_1_bss_20[lbl_1_bss_2F0.nextLeaf[i]].vel.z = 0.0f;
            }
            if (arg0 != 0) {
                lbl_1_bss_2F0.timer = 60;
            } else {
                lbl_1_bss_2F0.timer = 120;
            }
            if (lbl_1_bss_2734.cpuPlayer == 0) {
                omVibrate(lbl_1_bss_3730->player[0].playerNo, lbl_1_bss_2F0.timer / 2, 6, 6);
                omVibrate(lbl_1_bss_3730->player[1].playerNo, lbl_1_bss_2F0.timer / 2, 6, 6);
            }
            fn_1_270C(0);
            lbl_1_bss_2F0.mode++;
            break;
        case 4: {
            s16 old = lbl_1_bss_2F0.timer;
            lbl_1_bss_2F0.timer--;
            if (old < 0) {
                for (i = 0; i < lbl_1_bss_2F0.count + 1; i++) {
                    M541_MODEL *dst = &lbl_1_bss_3730->model[i + 0x10];
                    M541_MODEL *src = &lbl_1_bss_3730->model[lbl_1_bss_36A4[lbl_1_bss_2F0.nextLeaf[i]].a];
                    dst->posX = src->posX;
                    dst->posY = src->posY;
                    dst->posZ = src->posZ;
                    lbl_1_bss_3730->model[i + 0x10].posY -= 1.0f;
                    lbl_1_bss_3730->model[i + 0x10].posZ -= 1.0f;
                    Hu3DModelAttrReset(lbl_1_bss_3730->model[i + 0x10].modelId, 1);
                    Hu3DModelAttrSet(lbl_1_bss_3730->model[i + 0x10].modelId, 0x40000001);
                    Hu3DMotionTimeSet(lbl_1_bss_3730->model[i + 0x10].modelId, 0.0f);
                    Hu3DMotionSpeedSet(lbl_1_bss_3730->model[i + 0x10].modelId, 3.0f);
                    lbl_1_bss_3730->model[i + 0x10].rotY = 90.0f + -lbl_1_bss_2F0.angle[i];
                    lbl_1_bss_2F0.unk20[i].x = 0.7f;
                    lbl_1_bss_2F0.unk20[i].z = lbl_1_bss_2F0.dist[i] / 1000.0f;
                }
                lbl_1_bss_2F0.ready = 1;
                if (arg0 != 0) {
                    lbl_1_bss_2F0.timer = 120;
                } else {
                    lbl_1_bss_2F0.timer = 120;
                }
                if (lbl_1_bss_2734.cpuPlayer == 0) {
                    omVibrate(lbl_1_bss_3730->player[0].playerNo, lbl_1_bss_2F0.timer / 2, 12, 0);
                    omVibrate(lbl_1_bss_3730->player[1].playerNo, lbl_1_bss_2F0.timer / 2, 12, 0);
                }
                fn_1_29EC(0);
                fn_1_270C(2);
                lbl_1_bss_2F0.mode++;
            }
            break;
        }
        case 5: {
            s16 old = lbl_1_bss_2F0.timer;
            lbl_1_bss_2F0.timer--;
            if (old < 0) {
                for (i = 6; i <= 0x19; i++) {
                    Hu3DModelAttrSet(lbl_1_bss_3730->model[i].modelId, 1);
                    Hu3DMotionTimeSet(lbl_1_bss_3730->model[i].modelId, 0.0f);
                    Hu3DMotionSpeedSet(lbl_1_bss_3730->model[i].modelId, 0.0f);
                }
                for (i = 1; i < lbl_1_bss_2F0.count + 1; i++) {
                    lbl_1_bss_20[lbl_1_bss_2F0.nextLeaf[i]].hidden = 0;
                    lbl_1_bss_20[lbl_1_bss_2F0.nextLeaf[i]].vel = lbl_1_bss_20[lbl_1_bss_2F0.nextLeaf[i]].velSave;
                }
                fn_1_29EC(2);
                lbl_1_bss_2F0.ready = 0;
                lbl_1_bss_2F0.mode = 0;
                ret = 1;
            }
            break;
        }
    }
    return ret;
}
