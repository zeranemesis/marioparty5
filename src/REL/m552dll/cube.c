#include "types.h"
#include "string.h"
#include "dolphin.h"
#include "game/hu3d.h"
#include "REL/m552dll.h"

int rand8(void);
void PSMTXRotRad(Mtx m, char axis, f32 rad);

s32 fn_1_8C88(s32 arg0);
s32 fn_1_8D4C(s8 color, s32 mode, f32 time);
void fn_1_8F2C(s32 arg0, s32 arg1);

extern M552Field lbl_1_bss_3700;
extern M552Obj lbl_1_bss_1340[0xD0];
extern M552Obj lbl_1_bss_490[4];
extern M552Obj lbl_1_bss_530[12];

/* unit-owned .bss */
s32 lbl_1_bss_4920;

/* unit-owned .data: piece dimensions (w, h) per shape */
s32 lbl_1_data_410[22] = {
    1, 1, 1, 2, 1, 3, 1, 4, 2, 2, 2, 3, 2, 4, 3, 3, 3, 4, 4, 4, 5, 5,
};
f32 lbl_1_data_468[12] = { 0.0f, 0.0f, 0.0f, 50.0f, 0.0f, 0.0f, 0.0f, 50.0f, 50.0f, 50.0f, 50.0f, 0.0f };
f32 lbl_1_data_498[6] = { 100.0f, 200.0f, 300.0f, 400.0f, 200.0f, 300.0f };
s8 lbl_1_data_4B0[6][25] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 },
};
/* last shape picked / how many times in a row */
s32 lbl_1_data_548[2] = { -1, 0 };

void fn_1_7888(void)
{
    s32 i;

    memset(&lbl_1_bss_3700, 0, sizeof(M552Field));
    for (i = 0; i < 20; i++) {
        lbl_1_bss_3700.item[i].shape = -1;
    }
}

u8 fn_1_78EC(void)
{
    u8 rnd = rand8() % 128;
    u8 weight[6] = { 0x25, 0x1E, 0x1E, 0x0A, 0x0B, 0x0A };
    u8 shape;
    u8 sum;

    shape = 0;
    sum = 0;
    while (shape < 6) {
        sum += weight[shape];
        if (sum > rnd) {
            break;
        }
        shape++;
    }
    if (shape == lbl_1_data_548[0]) {
        lbl_1_data_548[1]++;
        if (lbl_1_data_548[1] == 3) {
            shape = (shape + 1) % 3;
        }
    } else {
        lbl_1_data_548[0] = shape;
        lbl_1_data_548[1] = 0;
    }
    return shape;
}

s32 fn_1_7A40(void)
{
    s32 ret = 0;
    M552Piece *next = &lbl_1_bss_3700.item[lbl_1_bss_3700.head];
    s32 prev;

    if (next->shape < 0) {
        next->shape = fn_1_78EC();
    }
    prev = lbl_1_bss_3700.head - 1;
    if (prev < 0) {
        prev = 19;
    }
    if (lbl_1_bss_3700.item[prev].alive == 0) {
        lbl_1_bss_3700.item[prev].y -= 8.5f;
    }
    if (lbl_1_bss_3700.item[prev].y
        < 1050.0f - (155.0f + (lbl_1_data_498[lbl_1_bss_3700.item[prev].shape] + lbl_1_data_498[next->shape]) * 0.5f)) {
        ret = next->shape + 1;
    }
    return ret;
}

s32 fn_1_7CBC(s16 arg0, s8 arg1)
{
    s32 cost[6] = { 1, 2, 3, 4, 4, 6 };
    s32 free = 0;
    s32 i;

    for (i = 0; i < 0x28; i++) {
        if (lbl_1_bss_1340[i + arg1 * 0x28].state == 0) {
            free++;
        }
    }
    for (i = 0; i < 2; i++) {
        M552Piece *piece = &lbl_1_bss_3700.plr[i].piece;

        if (piece->alive != 0 && piece->grid[12] == arg1 + 1) {
            free -= cost[piece->shape];
        }
    }
    if (free - cost[arg0] >= 0) {
        return 1;
    }
    return 0;
}

void fn_1_7DE4(s16 arg0)
{
    s32 count = 1;
    s32 i;
    s32 k;
    s8 color;
    M552Piece *piece;

    color = rand8() % 4;
    while (fn_1_7CBC(arg0, color) == 0) {
        color = (color + 1) % 4;
    }
    if (arg0 == 0) {
        if (rand8() < 0x80) {
            count = 2;
        }
    }
    if (arg0 == 1) {
        if (rand8() < 0x60) {
            count += 1;
        }
    }
    if (arg0 == 2) {
        if (rand8() < 0x40) {
            count += 1;
        }
    }
    if (arg0 == 3) {
        if (rand8() < 0x20) {
            count += 1;
        }
    }
    if (count == 2) {
        lbl_1_bss_4920 += 1;
    } else {
        lbl_1_bss_4920 = 0;
    }
    for (i = 0; i < count; i++) {
        piece = &lbl_1_bss_3700.item[lbl_1_bss_3700.head];
        piece->alive = 1;
        piece->shape = arg0;
        for (k = 0; k < 25; k++) {
            piece->grid[k] = lbl_1_data_4B0[piece->shape][k];
            if (piece->grid[k] != 0) {
                piece->grid[k] = color + 1;
            }
        }
        piece->y = 1050.0f;
        if (count > 1) {
            piece->x = i * 200 - 100;
            color = (color + 1) % 4;
            while (fn_1_7CBC(arg0, color) == 0) {
                color = (color + 1) % 4;
            }
        } else {
            piece->x = 0.0f;
        }
        fn_1_8C88(lbl_1_bss_3700.head);
        fn_1_8F2C(lbl_1_bss_3700.head, 0);
        lbl_1_bss_3700.head = (lbl_1_bss_3700.head + 1) % 20;
    }
}

void fn_1_82BC(s32 arg0)
{
    f32 time = -1.0f;
    s32 color;
    s32 i;
    s32 count;
    M552Obj *obj;

    i = 0;
    count = 0;
    for (; i < 25; i++) {
        if (lbl_1_bss_3700.plr[arg0].piece.grid[i] != 0) {
            obj = &lbl_1_bss_1340[lbl_1_bss_3700.plr[arg0].piece.cube[i]];
            if (count == 0) {
                time = Hu3DMotionTimeGet(obj->model);
                color = lbl_1_bss_3700.plr[arg0].piece.grid[i] - 1;
            }
            if (lbl_1_bss_530[count + arg0 * 6].state != 0) {
                Hu3DModelKill(lbl_1_bss_530[count + arg0 * 6].model);
            }
            lbl_1_bss_530[count + arg0 * 6].state = 1;
            lbl_1_bss_530[count + arg0 * 6].model = Hu3DModelLink(lbl_1_bss_490[color].model);
            Hu3DModelAttrSet(lbl_1_bss_530[count + arg0 * 6].model, 1);
            Hu3DModelPosSetV(lbl_1_bss_530[count + arg0 * 6].model,
                             &lbl_1_bss_1340[lbl_1_bss_3700.plr[arg0].piece.cube[i]].pos);
            lbl_1_bss_530[count + arg0 * 6].pos.x = lbl_1_bss_1340[lbl_1_bss_3700.plr[arg0].piece.cube[i]].pos.x;
            lbl_1_bss_530[count + arg0 * 6].pos.y = lbl_1_bss_1340[lbl_1_bss_3700.plr[arg0].piece.cube[i]].pos.y;
            lbl_1_bss_530[count + arg0 * 6].pos.z = lbl_1_bss_1340[lbl_1_bss_3700.plr[arg0].piece.cube[i]].pos.z;
            Hu3DMotionTimeSet(lbl_1_bss_530[count + arg0 * 6].model, time);
            Hu3DMotionSpeedSet(lbl_1_bss_530[count + arg0 * 6].model, 2.0f);
            Hu3DModelAttrReset(lbl_1_bss_530[count + arg0 * 6].model, 0x40000001);
            obj->count = 0;
            Hu3DModelCameraSet(lbl_1_bss_530[count + arg0 * 6].model, 2);
            count++;
        }
        if (count >= 6) {
            break;
        }
    }
}

void fn_1_8644(s32 arg0, s32 arg1)
{
    s32 i;
    M552Obj *obj;

    for (i = 0; i < 25; i++) {
        if (lbl_1_bss_3700.plr[arg0].piece.grid[i] != 0) {
            obj = &lbl_1_bss_1340[lbl_1_bss_3700.plr[arg0].piece.cube[i]];
            if (arg1 != 0) {
                Hu3DModelAttrReset(obj->model, 1);
            } else {
                Hu3DModelAttrSet(obj->model, 1);
            }
        }
    }
    for (i = 0; i < 6; i++) {
        if (lbl_1_bss_530[i + arg0 * 6].state != 0) {
            if (arg1 != 0) {
                Hu3DModelAttrSet(lbl_1_bss_530[i + arg0 * 6].model, 1);
            } else {
                Hu3DModelAttrReset(lbl_1_bss_530[i + arg0 * 6].model, 1);
            }
        }
    }
}

void fn_1_879C(s32 arg0, s32 arg1)
{
    M552Piece *piece;
    M552Obj *obj;
    s32 i;

    if (arg1 == 0) {
        piece = &lbl_1_bss_3700.item[arg0];
        piece->shape = -1;
    } else {
        piece = &lbl_1_bss_3700.plr[arg0].piece;
    }
    piece->alive = 0;
    for (i = 0; i < 25; i++) {
        if (piece->grid[i] != 0) {
            obj = &lbl_1_bss_1340[piece->cube[i]];
            obj->state = 0;
            Hu3DModelAttrSet(obj->model, 1);
        }
    }
    if (arg1 != 0) {
        for (i = 0; i < 6; i++) {
            if (lbl_1_bss_530[i + arg0 * 6].state != 0) {
                Hu3DModelKill(lbl_1_bss_530[i + arg0 * 6].model);
                lbl_1_bss_530[i + arg0 * 6].state = 0;
            }
        }
    }
}

void fn_1_88EC(void)
{
    s32 spawn;
    s32 i;
    s32 k;
    M552Piece *piece;
    M552Obj *obj;

    for (i = 0; i < 20; i++) {
        if (lbl_1_bss_3700.item[i].alive != 0) {
            lbl_1_bss_3700.item[i].y -= 8.5f;
            fn_1_8F2C(i, 0);
            if (lbl_1_bss_3700.item[i].y < -1050.0f) {
                piece = &lbl_1_bss_3700.item[i];
                piece->shape = -1;
                piece->alive = 0;
                for (k = 0; k < 25; k++) {
                    if (piece->grid[k] != 0) {
                        obj = &lbl_1_bss_1340[piece->cube[k]];
                        obj->state = 0;
                        Hu3DModelAttrSet(obj->model, 1);
                    }
                }
            }
        }
    }
    spawn = fn_1_7A40();
    if (spawn != 0) {
        fn_1_7DE4(spawn - 1);
    }
}

s32 fn_1_8C88(s32 arg0)
{
    s32 failed = 0;
    M552Piece *piece = &lbl_1_bss_3700.item[arg0];
    s32 i;

    for (i = 0; i < 25; i++) {
        if (piece->grid[i] != 0) {
            piece->cube[i] = fn_1_8D4C(piece->grid[i], 0, -1.0f);
            if (piece->cube[i] < 0) {
                failed = 1;
            }
        }
    }
    return failed;
}

s32 fn_1_8D4C(s8 color, s32 mode, f32 time)
{
    M552Obj *obj;
    HU3DMODEL *model;
    HSFMATERIAL *material;
    s32 limit;
    s32 i;
    s32 n;

    if (mode != 0) {
        limit = 0xC;
        obj = &lbl_1_bss_1340[(color - 1) * 0xC + 0xA0];
    } else {
        limit = 0x28;
        obj = &lbl_1_bss_1340[(color - 1) * 0x28];
    }
    for (i = 0; i < limit; i++, obj++) {
        if (obj->state == 0) {
            break;
        }
    }
    if (i == limit) {
        return -1;
    }
    obj->state = 1;
    Hu3DModelPosSet(obj->model, 0.0f, 0.0f, -500.0f);
    Hu3DModelAttrReset(obj->model, 1);
    if (time < 0.0f) {
        Hu3DMotionTimeSet(obj->model, 0.0f);
    } else {
        Hu3DMotionTimeSet(obj->model, time);
    }
    Hu3DMotionSpeedSet(obj->model, 2.0f);
    Hu3DModelAttrReset(obj->model, 0x40000001);
    obj->count = 0;
    model = &Hu3DData[obj->model];
    material = model->hsf->material;
    for (n = 0; n < model->hsf->materialNum; n++) {
        material->invAlpha = 0.0f;
        material++;
    }
    if (mode != 0) {
        return i + ((color - 1) * 0xC + 0xA0);
    }
    return i + (color - 1) * 0x28;
}

void fn_1_8F2C(s32 arg0, s32 arg1)
{
    Mtx transM;
    Mtx rotM;
    M552Piece *piece;
    M552Obj *obj;
    s32 count;
    s32 y;
    s32 x;

    if (arg1 == 0) {
        piece = &lbl_1_bss_3700.item[arg0];
    } else {
        piece = &lbl_1_bss_3700.plr[arg0].piece;
    }
    PSMTXRotRad(rotM, 'z', 0.017453292f * piece->rot);
    count = 0;
    for (y = 0; y < 5; y++) {
        for (x = 0; x < 5; x++) {
            if (piece->grid[y * 5 + x] != 0) {
                obj = &lbl_1_bss_1340[piece->cube[x + y * 5]];
                obj->pos.x = (-200.0f + 100.0f * x) - lbl_1_data_468[piece->shape * 2];
                obj->pos.y = -200.0f + 100.0f * y + lbl_1_data_468[piece->shape * 2 + 1];
                PSMTXTrans(transM, obj->pos.x, obj->pos.y, piece->z);
                PSMTXConcat(rotM, transM, transM);
                obj->pos.x = transM[0][3];
                obj->pos.y = transM[1][3];
                obj->pos.z = transM[2][3];
                obj->pos.x += piece->x;
                obj->pos.y += piece->y;
                if (arg1 != 0) {
                    lbl_1_bss_530[count + arg0 * 6].pos.x = obj->pos.x;
                    lbl_1_bss_530[count + arg0 * 6].pos.y = obj->pos.y;
                    lbl_1_bss_530[count + arg0 * 6].pos.z = obj->pos.z;
                    count++;
                }
            }
        }
    }
}
