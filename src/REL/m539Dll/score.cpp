#include "REL/m539Dll.h"

/*
 * m539's "score": the seven-segment board above a player.
 *
 * A score is built out of nine digit models arranged in a three by three grid;
 * the grid cell that holds a digit is picked from a packed byte table, and the
 * same table also drives the little block model that pops out of the board
 * when the number changes. Two fades - one for the digits, one for the rolling
 * number - are advanced every frame and their results drive motion times.
 */

extern "C" {

typedef struct M539Fade_s {
    s32 frames;
    s32 cur;
    f32 from;
    f32 hold;
    f32 to;
} M539Fade;

typedef struct M539Score_s {
    s32 unk0;
    s32 unk4;
    s32 player;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 board;
    s32 roller;
    void *digits[3][3];
    s32 block;
    s32 head;
    s32 sprs[3];
    Vec headPos;
    Vec lastPos;
    Vec delta;
    s32 rot;
    M539Fade digitFade;
    M539Fade rollFade;
    s32 step;
    s32 rollValue;
    s32 effect;
    s32 unkB8;
    s32 loopSnd;
    s32 lastDigit;
    s32 tick;
} M539Score;

typedef struct M539Line_s {
    s32 tag;
    s32 res;
    s32 v[3];
} M539Line;

typedef struct M539Block_s {
    s32 unk0;
    s32 unk1C;
    f32 x;
    f32 y;
    f32 z;
} M539Block;

typedef struct M539LineTable_s {
    u8 cell[0x18];
    f32 pos[3][4];
} M539LineTable;

extern void **lbl_1_bss_A04;
extern s32 fn_1_6974;
extern s32 fn_1_86AC;

const s8 lbl_1_data_598[] = "sd%02d";
char lbl_1_data_59F[] = "sm_ptr != 0";
char lbl_1_data_5AB[] = "a_set.h";
char lbl_1_data_5B3[] = "012";
const s8 lbl_1_data_5B7[] = "block";
const s8 lbl_1_data_5BD[] = "head";

u8 lbl_1_rodata_348 = 0x64;
char lbl_1_rodata_349[] = "??";
s32 lbl_1_rodata_390[4] = {0x7F6, 0x7F8, 0x7F7, 0x7F9};
s32 lbl_1_rodata_3A0[4] = {0x7E6, 0x7E8, 0x7E7, 0x7E9};
u8 lbl_1_rodata_3B0[8] = {0x10, 0x02, 0x01, 0x00, 0x22, 0x21, 0x20, 0x12};
s32 lbl_1_rodata_3B8[4] = {0x7EA, 0x7EC, 0x7EB, 0x7ED};
s32 lbl_1_rodata_3C8[4] = {0x7EE, 0x7F0, 0x7EF, 0x7F1};
M539LineTable lbl_1_rodata_3D8 = {
    {0x10, 0x02, 0x01, 0x00, 0x21, 0x20, 0x11, 0x12,
     0x20, 0x10, 0x22, 0x00, 0x02, 0x11, 0x21, 0x01,
     0x11, 0x22, 0x20, 0x12, 0x20, 0x11, 0x02, 0x22},
    {{64.0f, 140.0f, 0.0f, 345.0f},
     {64.0f, 140.0f, 0.0f, 300.0f},
     {345.0f, 140.0f, 0.0f, 300.0f}}
};
s32 lbl_1_rodata_420[4] = {0x7F2, 0x7F4, 0x7F3, 0x7F5};

void *__construct_array(void *ptr, void *ctor, void *dtor, u32 size, u32 n);

void fn_1_1264(void *self);
void fn_1_1270(void *self, s32 arg);
void fn_1_1330(void *self, void **list);
void fn_1_1D4(const char *msg, const char *file, s32 line);
u32 fn_1_0(s32 arg, ...);
u32 fn_1_2C94(u32 self, s32 dataNum);
u32 fn_1_2DB4(u32 self, u32 arg);
void fn_1_2E40(s16 *self, u32 flags);
void fn_1_3084(s16 *self, u16 mask);
f32 (*fn_1_30D8(s32 self))[4];
void fn_1_30F4(s32 ptr, s32 flag);
void *fn_1_3150(s32 self, const s8 *name);
void fn_1_31E4(s16 *self);
void fn_1_3660(s16 *self, s32 arg);
u32 fn_1_6704(u32 self, s16 *spr, u8 *digits, s32 player);
void fn_1_6A24(u32 self);
void fn_1_6B44(u32 self);
void fn_1_6B60(s32 self);
void fn_1_6BEC(u32 self);
void fn_1_6C64(s32 self);
void fn_1_6D54(s32 self);
s32 fn_1_6DBC(s32 self);
void fn_1_6E3C(M539Line *line);
s32 fn_1_6E48(M539Line *line, s32 a, s32 b, s32 c);
void fn_1_C750(M539Fade *self, s32 frames, f32 level);
f32 fn_1_C7C8(M539Fade *self);
void *fn_1_C920(u32 self, Vec *pos);
void *fn_1_F40C(s32 *self, s16 arg);
void Hu3DModelObjMtxGet(s32 modelId, const s8 *name, f32 mtx[4][4]);
void Hu3DMotionTimeSet(s16 motionId, f32 time);
void PSMTXRotRad(f32 mtx[4][4], s16 axis, f32 rad);
void PSMTXTrans(f32 mtx[4][4], f32 x, f32 y, f32 z);
void PSMTXScaleApply(f32 mtx[4][4], f32 mtx2[4][4], f32 x, f32 y, f32 z);
void PSMTXConcat(f32 mtx[4][4], f32 a[4][4], f32 b[4][4]);
void PSMTXTransApply(f32 mtx[4][4], f32 mtx2[4][4], f32 x, f32 y, f32 z);
s32 HuAudFXPlay(s32 id);
void HuAudFXStop(s32 id);
s32 sprintf(s8 *str, const s8 *fmt, ...);

void fn_1_102F8(s32 *arg0);
void fn_1_10304(s32 *arg0);
void fn_1_10504(void *arg0);
void fn_1_10F14(void *arg0);
void fn_1_11030(s32 *arg0, s32 arg1);

// NONMATCHING: register allocation only.
u32 fn_1_FAB4(s32 dataNum, u32 *cache)
{
    s32 num = dataNum;
    u32 model;

    if (*cache != 0U) {
        model = fn_1_0(2, 0.0f);
        if (model != 0U) {
            model = fn_1_2DB4(model, *cache);
        }
        return model;
    }
    model = fn_1_0(2, 0.0f);
    if (model != 0U) {
        model = fn_1_2C94(model, num);
    }
    *cache = model;
    return *cache;
}

// NONMATCHING: register allocation only.
s16 *fn_1_FB54(s16 *arg0, s32 index)
{
    f32 mtx[4][4];
    Vec pos;
    s8 name_buf[8];
    s8 *name = name_buf;
    s32 i = index;
    s16 *model;

    model = (s16 *)fn_1_72E8(0x610008);
    sprintf(name, lbl_1_data_598, i);
    Hu3DModelObjMtxGet(*arg0, name, mtx);
    pos.x = mtx[3][0];
    pos.y = mtx[3][1];
    pos.z = mtx[3][2];
    fn_1_2EF4(model, &pos);
    fn_1_2E08(model, 0x40000002);
    return model;
}

// NONMATCHING: register allocation only.
void fn_1_FC08(void *arg0, s32 index, f64 value)
{
    s32 i = index;
    s32 *spr = *(s32 **)((char *)arg0 + (i * 4) + 0x50);
    f32 time;

    time = (f32)fmod((f64)(10.0f * -(f32)fmod(value, 10.0)), 100.0);
    if (time < 0.0f) {
        time += 100.0f;
    }
    Hu3DMotionTimeSet((s16)*spr, time);
}

// NONMATCHING: register allocation only.
void *fn_1_FCF0(void *arg0, s32 arg1)
{
    M539Score *self = (M539Score *)arg0;
    s32 player = arg1;
    f32 mtx[4][4];
    Vec pos;
    s8 name_buf[8];
    s8 *name = name_buf;
    u8 digits[4];
    s32 *elem;
    s32 *spr;
    s32 i;
    s32 j;
    u32 model;

    fn_1_1264(self);
    self->player = player;
    self->board = (s32)fn_1_72E8(0x610002);
    self->roller = (s32)fn_1_72E8(0x610003);
    __construct_array(&self->digits[0][0], (void *)fn_1_10304, (void *)&fn_1_86AC, 4, 9);
    self->block = (s32)fn_1_72E8(0x610009);
    self->head = (s32)fn_1_72E8(0x610004);
    __construct_array(&self->sprs[0], (void *)fn_1_102F8, (void *)&fn_1_6974, 4, 3);
    fn_1_C750(&self->digitFade, 0, 0.0f);
    fn_1_C750(&self->rollFade, 0, 0.0f);
    self->effect = 0;
    self->unkB8 = 0;
    self->tick = 0;
    if (lbl_1_bss_A04 == NULL) {
        fn_1_1D4(lbl_1_data_59F, lbl_1_data_5AB, 0x20);
    }
    fn_1_1330(self, lbl_1_bss_A04);
    fn_1_2E08((s16 *)self->board, 0x40000001);
    fn_1_2E08((s16 *)self->head, 0x40000002);
    fn_1_2E40((s16 *)self->block, 0x40000001);
    fn_1_2E08((s16 *)self->block, 0x40000002);
    fn_1_3660((s16 *)self->block, 0);
    digits[0] = lbl_1_rodata_348;
    digits[1] = lbl_1_rodata_349[0];
    digits[2] = lbl_1_rodata_349[1];
    digits[3] = lbl_1_rodata_349[2];
    for (i = 0; i < 3; i++) {
        digits[1] = lbl_1_data_5B3[i];
        for (j = 0; j < 3; j++) {
            digits[2] = lbl_1_data_5B3[j];
            model = fn_1_0(0x34, 0.0f);
            if (model != 0U) {
                model = fn_1_6704(model, (s16 *)self->roller, digits, player);
            }
            if (*(u32 *)((char *)self + (i * 0xC) + (j * 4) + 0x24) != model) {
                elem = *(s32 **)((char *)self + (i * 0xC) + (j * 4) + 0x24);
                if (elem != NULL) {
                    if ((u32)(elem + 0x2C) != 0U) {
                        fn_1_30F4(*(s32 *)((char *)elem + 0x2C), 1);
                    }
                    if (*(void **)((char *)elem + 0x28) != NULL) {
                        fn_1_30F4(*(s32 *)((char *)elem + 0x28), 1);
                    }
                    if (*(void **)((char *)elem + 0x24) != NULL) {
                        fn_1_30F4(*(s32 *)((char *)elem + 0x24), 1);
                    }
                    fn_1_1270(elem, 0);
                }
                *(s32 **)((char *)self + (i * 0xC) + (j * 4) + 0x24) = (s32 *)model;
            }
        }
    }
    fn_1_6D54(*(s32 *)((char *)self + 0x34));
    fn_1_3084((s16 *)self->board, (u16)(1 << player));
    fn_1_31E4((s16 *)self->head);
    fn_1_3084((s16 *)self->block, (u16)(1 << player));
    for (i = 0; i < 3; i++) {
        model = (s32)fn_1_FB54((s16 *)self->roller, i);
        sprintf(name, lbl_1_data_598, i);
        Hu3DModelObjMtxGet(*(s32 *)self->roller, name, mtx);
        pos.x = mtx[3][0];
        pos.y = mtx[3][1];
        pos.z = mtx[3][2];
        fn_1_2EF4((s16 *)model, &pos);
        fn_1_2E08((s16 *)model, 0x40000002);
        if (*(s32 *)((char *)self + (i * 4) + 0x50) != (s32)model) {
            fn_1_30F4(*(s32 *)((char *)self + (i * 4) + 0x50), 1);
            *(s32 *)((char *)self + (i * 4) + 0x50) = (s32)model;
        }
        fn_1_3084((s16 *)(*(s32 *)((char *)self + (i * 4) + 0x50)), (u16)(1 << player));
        fn_1_FC08(arg0, i, (f64)i);
    }
    self->step = 0;
    self->rot = 0x40490FDB;
    self->unkB8 = 0;
    self->lastDigit = 0;
    return self;
}

// NONMATCHING: register allocation only.
s32 *fn_1_102A0(s32 *arg0, s16 arg1)
{
    s16 count = arg1;

    if (arg0 != NULL) {
        fn_1_F40C(arg0, 1);
        if (count > 0) {
            fn_1_C8(arg0);
        }
    }
    return arg0;
}

void fn_1_102F8(s32 *arg0)
{
    *arg0 = 0;
}

void fn_1_10304(s32 *arg0)
{
    fn_1_11030(arg0, 0);
}

// NONMATCHING: register allocation only.
s32 *fn_1_10330(s32 *arg0, s16 arg1)
{
    s16 count = arg1;

    if (arg0 != NULL) {
        fn_1_1270(arg0, 0);
        if (count > 0) {
            fn_1_C8(arg0);
        }
    }
    return arg0;
}

void fn_1_10388(u32 arg0)
{
    fn_1_6A24(arg0);
}

void fn_1_103B0(u32 arg0)
{
    fn_1_6B44(arg0);
}

void fn_1_103D8(u32 arg0)
{
    fn_1_6BEC(arg0);
}

void fn_1_10400(void *arg0)
{
    M539Score *self = (M539Score *)arg0;

    fn_1_2E40((s16 *)self->head, 0x40000002);
    HuAudFXPlay(0x7E4);
}

void fn_1_1044C(void *arg0)
{
    M539Score *self = (M539Score *)arg0;

    fn_1_C750(&self->digitFade, 0x12, 0.0f);
    HuAudFXPlay(0x7E5);
    if (self->player == 0) {
        fn_1_10504(self);
    }
}

void fn_1_104A8(void *arg0)
{
    M539Score *self = (M539Score *)arg0;

    fn_1_C750(&self->digitFade, 0x12, 1.0f);
    if (self->player == 0) {
        HuAudFXPlay(0x7E5);
        fn_1_10504(self);
    }
}

// NONMATCHING: register allocation only.
void fn_1_10504(void *arg0)
{
    M539Score *self = (M539Score *)arg0;
    M539Block *block;
    Vec pos;
    u32 effect;

    block = (M539Block *)fn_1_3150(self->block, lbl_1_data_5B7);
    pos.x = block->x - 20.0f;
    pos.y = block->y;
    pos.z = 100.0f + block->z;
    effect = fn_1_0(0x18, block->x);
    if (effect != 0U) {
        fn_1_C920(effect, &pos);
    }
}

// NONMATCHING: register allocation only.
void fn_1_105F4(void *arg0)
{
    M539Score *self = (M539Score *)arg0;
    f32 mtx[4][4];
    s32 i;
    s32 j;
    u32 elem;
    f32 roll;
    s32 ones;
    f32 time;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            elem = *(u32 *)((char *)self + (i * 0xC) + (j * 4) + 0x24);
            if (elem != 0U) {
                fn_1_6A24(elem);
            }
        }
    }
    Hu3DModelObjMtxGet(*(s32 *)self->head, lbl_1_data_5BD, mtx);
    self->headPos.x = mtx[3][0];
    self->headPos.y = mtx[3][1];
    self->headPos.z = mtx[3][2];
    PSVECSubtract(&self->headPos, &self->lastPos, &self->delta);
    self->lastPos = self->headPos;
    fn_1_10F14(self);
    roll = fn_1_C7C8(&self->rollFade);
    ones = (s32)fmod((f64)roll, 10.0);
    if (self->tick > 0) {
        self->tick = self->tick - 1;
        if (self->tick == 0) {
        }
    }
    if (ones != self->lastDigit) {
        self->lastDigit = ones;
        HuAudFXPlay(lbl_1_rodata_390[self->player]);
        self->tick = 3;
    }
    if (self->effect != 0) {
        *(s32 *)self->effect = (s32)roll;
    }
    time = (f32)fmod((f64)(10.0f * -(f32)fmod((f64)(f32)(s32)(roll / 100.0f), 10.0)), 100.0);
    if (time < 0.0f) {
        time += 100.0f;
    }
    Hu3DMotionTimeSet((s16)self->sprs[1], time);
    time = (f32)fmod((f64)(10.0f * -(f32)fmod((f64)(f32)(s32)(roll / 10.0f), 10.0)), 100.0);
    if (time < 0.0f) {
        time += 100.0f;
    }
    Hu3DMotionTimeSet((s16)self->sprs[2], time);
    time = (f32)fmod((f64)(10.0f * -(f32)fmod((f64)(f32)ones, 10.0)), 100.0);
    if (time < 0.0f) {
        time += 100.0f;
    }
    Hu3DMotionTimeSet((s16)self->sprs[0], time);
}

// NONMATCHING: register allocation only.
void fn_1_10A90(void *arg0, s32 arg)
{
    M539Score *self = (M539Score *)arg0;
    s32 i;
    s32 j;
    u32 elem;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            elem = *(u32 *)((char *)self + (i * 0xC) + (j * 4) + 0x24);
            if (elem != 0U) {
                fn_1_6B44(elem);
            }
        }
    }
    fn_1_3084((s16 *)self->head, (u16)(1 << self->player));
    self->loopSnd = HuAudFXPlay(lbl_1_rodata_3A0[self->player]);
}

// NONMATCHING: register allocation only.
void fn_1_10B58(s32 arg0, s32 arg)
{
    s32 *self = (s32 *)arg0;
    s32 i;
    s32 j;
    u32 elem;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            elem = *(u32 *)((char *)self + (i * 0xC) + (j * 4) + 0x24);
            if (elem != 0U) {
                fn_1_6BEC(elem);
            }
        }
    }
}

// NONMATCHING: register allocation only.
void fn_1_10BE8(void *arg0, s32 arg)
{
    M539Score *self = (M539Score *)arg0;
    s32 step = self->step;

    if (step < 8) {
        HuAudFXPlay(lbl_1_rodata_3B8[self->player]);
        fn_1_6B60(*(s32 *)((char *)self + (((lbl_1_rodata_3B0[step] >> 4) & 3) * 0xC) + ((lbl_1_rodata_3B0[step] & 3) * 4) + 0x24));
        self->step = step + 1;
        if (self->step == 8) {
            HuAudFXStop(self->loopSnd);
        }
        self->rot = 0;
        fn_1_2E40((s16 *)self->block, 0x40000002);
        fn_1_3660((s16 *)self->block, 0);
        HuAudFXPlay(lbl_1_rodata_3C8[self->player]);
    }
}

// NONMATCHING: register allocation only.
s32 fn_1_10CEC(void *arg0, s32 arg1, s32 arg)
{
    M539Score *self = (M539Score *)arg0;
    M539Line line;
    s32 model[3];
    s32 a;
    s32 b;
    s32 c;
    s32 i = 0;

    do {
        model[i] = *(s32 *)((char *)self + ((lbl_1_rodata_3D8.cell[arg1 * 3 + i] >> 4 & 3) * 0xC) + ((lbl_1_rodata_3D8.cell[arg1 * 3 + i] & 3) * 4) + 0x24);
        i++;
    } while (i < 3);
    fn_1_6E3C(&line);
    c = fn_1_6DBC(line.v[2]);
    b = fn_1_6DBC(line.v[1]);
    a = fn_1_6DBC(line.v[0]);
    if (fn_1_6E48(&line, a, b, c) != 0) {
        for (i = 0; i < 3; i++) {
            fn_1_6C64(model[i]);
        }
    }
    if (line.res != 0) {
        self->rollValue = self->rollValue + line.res;
        fn_1_C750(&self->rollFade, 0x24, (f32)self->rollValue);
        HuAudFXPlay(lbl_1_rodata_420[self->player]);
    }
    return line.res;
}

// NONMATCHING: register allocation only.
s32 fn_1_10E64(s32 arg)
{
    M539Score *node;

    if (lbl_1_bss_A04 == NULL) {
        fn_1_1D4(lbl_1_data_59F, lbl_1_data_5AB, 0x20);
    }
    node = (M539Score *)(*lbl_1_bss_A04);
    for (;;) {
        if ((void *)node == lbl_1_bss_A04) {
            return 1;
        }
        if (node->step != 8) {
            return 0;
        }
        node = (M539Score *)node->unk0;
    }
}

// NONMATCHING: register allocation only.
void fn_1_10F14(void *arg0)
{
    M539Score *self = (M539Score *)arg0;
    M539Block *block;
    f32 (*mtx)[4];
    f32 rot[4][4];
    f32 x;
    f32 y;
    f32 z;
    f32 scale;

    block = (M539Block *)fn_1_3150(self->block, lbl_1_data_5B7);
    mtx = fn_1_30D8(self->block);
    x = block->x;
    y = block->y;
    z = block->z;
    scale = fn_1_C7C8(&self->digitFade);
    PSMTXRotRad(rot, 0x59, -0.2617994f);
    PSMTXTrans(mtx, -x, -y, -z);
    PSMTXScaleApply(mtx, mtx, scale, scale, scale);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXTransApply(mtx, mtx, x, y, z);
}

void fn_1_11030(s32 *arg0, s32 arg1)
{
    *arg0 = arg1;
}

}
