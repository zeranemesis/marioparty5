#define _MATH_H
#define M539_USE_DOLPHIN_HEADERS
extern "C" {
#include "dolphin.h"
#include "humath.h"
#include "game/hu3d.h"
#include "game/audio.h"
#include "game/mg/timer.h"
}
#include "REL/m539Dll.h"

/*
 * m539's "score": the seven-segment board above a player.
 *
 * A score is built out of nine digit drums arranged in a three by three grid;
 * the grid cell that holds a drum is picked from a packed byte table, and the
 * same table also drives the little block model that pops out of the board
 * when the number changes. Two fades - one for the digits, one for the rolling
 * number - are advanced every frame and their results drive motion times.
 * The record itself is the M539Game of the shared header.
 */

extern "C" {

// Score bookkeeping object of the drum code (drum.cpp).
typedef struct M539Line_s {
    s32 pad0;
    s32 score;
} M539Line;

typedef struct M539LineTable_s {
    u8 cell[0x18];
    f32 pos[3][4];
} M539LineTable;

extern M539Node *lbl_1_bss_A04;

const char lbl_1_data_598[] = "sd%02d";
char lbl_1_data_59F[] = "sm_ptr != 0";
char lbl_1_data_5AB[] = "a_set.h";
char lbl_1_data_5B3[] = "012";
const char lbl_1_data_5B7[] = "block";
const char lbl_1_data_5BD[] = "head";

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


// Opaque here: the effect object lives in effect.cpp.
typedef struct M539Effect_s M539Effect;

#ifdef __MWERKS__
void *__construct_array(void *ptr, void *ctor, void *dtor, u32 size, u32 n);
#else
// Portable stand-in for the MWCC runtime helper.
static void *__construct_array(void *ptr, void *ctor, void *dtor, u32 size, u32 n)
{
    u8 *p = (u8 *)ptr;

    (void)dtor;
    while (n-- != 0) {
        ((void (*)(void *))ctor)(p);
        p += size;
    }
    return ptr;
}
#endif

void fn_1_1264(M539Node *self);
void *fn_1_1270(M539Node *self, s16 flag);
void fn_1_1330(M539Node *self, M539Node *list);
void fn_1_1D4(const char *msg, const char *file, s32 line);
void *fn_1_0(s32 size);
M539Model *fn_1_2C94(M539Model *self, s32 dataNum);
M539Model *fn_1_2DB4(M539Model *self, M539Model *parent);
void fn_1_2E40(M539Model *self, u32 flags);
void fn_1_3084(M539Model *self, u16 mask);
MtxPtr fn_1_30D8(M539Model *self);
M539Model *fn_1_30F4(M539Model *self, s16 flag);
HSFOBJECT *fn_1_3150(M539Model *self, char *name);
void fn_1_31E4(M539Model *self);
void fn_1_3660(M539Model *self, s32 arg);
M539Model *fn_1_72E8(s32 id);
M539Drum *fn_1_6704(M539Drum *self, M539Model *anchor, char *nodeName, s32 camIdx);
void fn_1_6A24(M539Drum *self);
void fn_1_6B44(M539Drum *self);
void fn_1_6B60(M539Drum *self);
void fn_1_6BEC(M539Drum *self);
void fn_1_6C64(M539Drum *self);
void fn_1_6D54(M539Drum *self);
s32 fn_1_6DBC(M539Drum *self);
void fn_1_6E3C(M539Line *self);
s32 fn_1_6E48(M539Line *self, s32 band, s32 hit, s32 want);
void *fn_1_6974(M539Model **self, s16 flag);
void *fn_1_86AC(M539Drum **self, s16 flag);
void fn_1_C750(M539Fade *self, s32 frames, f32 level);
f32 fn_1_C7C8(M539Fade *self);
M539Effect *fn_1_C920(M539Effect *self, Vec *pos);
M539ScoreSpr *fn_1_F40C(M539ScoreSpr *self, s16 arg);
int sprintf(char *str, const char *fmt, ...);
double fmod(double x, double y);

void fn_1_102F8(M539Model **arg0);
void fn_1_10304(M539Drum **arg0);
void fn_1_10504(M539Game *arg0);
void fn_1_10F14(M539Game *arg0);
void fn_1_11030(M539Drum **arg0, M539Drum *arg1);

// The GameCube stores the raw bit pattern of pi in the f32 "rot" slot.
#ifdef TARGET_PC
#define M539_ROT_SET(game, bits, f) ((game)->rot = (f))
#else
#define M539_ROT_SET(game, bits, f) (*(s32 *)&(game)->rot = (bits))
#endif

// NONMATCHING: register allocation only.
M539Model *fn_1_FAB4(s32 dataNum, M539Model **cache)
{
    s32 num = dataNum;
    M539Model *model;

    if (*cache != 0) {
        model = (M539Model *)fn_1_0(sizeof(M539Model));
        if (model != 0) {
            model = fn_1_2DB4(model, *cache);
        }
        return model;
    }
    model = (M539Model *)fn_1_0(sizeof(M539Model));
    if (model != 0) {
        model = fn_1_2C94(model, num);
    }
    *cache = model;
    return *cache;
}

// NONMATCHING: register allocation only.
M539Model *fn_1_FB54(M539Model *roller, s32 index)
{
    Mtx mtx;
    Vec pos;
    char name_buf[8];
    char *name = name_buf;
    s32 i = index;
    M539Model *model;

    model = fn_1_72E8(0x610008);
    sprintf(name, lbl_1_data_598, i);
    Hu3DModelObjMtxGet(roller->id, name, mtx);
    pos.x = mtx[0][3];
    pos.y = mtx[1][3];
    pos.z = mtx[2][3];
    fn_1_2EF4(model, &pos);
    fn_1_2E08(model, 0x40000002);
    return model;
}

// NONMATCHING: register allocation only.
void fn_1_FC08(M539Game *self, s32 index, f64 value)
{
    s32 i = index;
    M539Model *spr = self->sprs[i];
    f32 time;

    time = (f32)fmod((f64)(10.0f * -(f32)fmod(value, 10.0)), 100.0);
    if (time < 0.0f) {
        time += 100.0f;
    }
    Hu3DMotionTimeSet(spr->id, time);
}

// NONMATCHING: register allocation only.
M539Game *fn_1_FCF0(M539Game *self, s32 player)
{
    char name_buf[8];
    char digits[4];
    M539Drum *elem;
    M539Model *spr;
    s32 i;
    s32 j;
    M539Drum *model;

    fn_1_1264(&self->link);
    self->player = player;
    self->board = fn_1_72E8(0x610002);
    self->roller = fn_1_72E8(0x610003);
    __construct_array(&self->digits[0][0], (void *)fn_1_10304, (void *)fn_1_86AC, sizeof(self->digits[0][0]), 9);
    self->block = fn_1_72E8(0x610009);
    self->head = fn_1_72E8(0x610004);
    __construct_array(&self->sprs[0], (void *)fn_1_102F8, (void *)fn_1_6974, sizeof(self->sprs[0]), 3);
    fn_1_C750(&self->digitFade, 0, 0.0f);
    fn_1_C750(&self->rollFade, 0, 0.0f);
    self->effect = 0;
    self->unkB8 = 0;
    self->tick = 0;
    if (lbl_1_bss_A04 == 0) {
        fn_1_1D4(lbl_1_data_59F, lbl_1_data_5AB, 0x20);
    }
    fn_1_1330(&self->link, lbl_1_bss_A04);
    fn_1_2E08(self->board, 0x40000001);
    fn_1_2E08(self->head, 0x40000002);
    fn_1_2E40(self->block, 0x40000001);
    fn_1_2E08(self->block, 0x40000002);
    fn_1_3660(self->block, 0);
    digits[0] = lbl_1_rodata_348;
    digits[1] = lbl_1_rodata_349[0];
    digits[2] = lbl_1_rodata_349[1];
    digits[3] = lbl_1_rodata_349[2];
    for (i = 0; i < 3; i++) {
        digits[1] = lbl_1_data_5B3[i];
        for (j = 0; j < 3; j++) {
            digits[2] = lbl_1_data_5B3[j];
            model = (M539Drum *)fn_1_0(sizeof(M539Drum));
            if (model != 0) {
                model = fn_1_6704(model, self->roller, digits, player);
            }
            if (self->digits[i][j] != model) {
                elem = self->digits[i][j];
                if (elem != 0) {
                    if (&elem->mdl[2] != 0) {
                        fn_1_30F4(elem->mdl[2], 1);
                    }
                    if (&elem->mdl[1] != 0) {
                        fn_1_30F4(elem->mdl[1], 1);
                    }
                    if (&elem->mdl[0] != 0) {
                        fn_1_30F4(elem->mdl[0], 1);
                    }
                    fn_1_1270(&elem->link, 0);
                }
                self->digits[i][j] = model;
            }
        }
    }
    fn_1_6D54(self->digits[1][1]);
    fn_1_3084(self->board, (u16)(1 << player));
    fn_1_31E4(self->head);
    fn_1_3084(self->block, (u16)(1 << player));
    for (i = 0; i < 3; i++) {
        spr = fn_1_FB54(self->roller, i);
        if (self->sprs[i] != spr) {
            fn_1_30F4(self->sprs[i], 1);
            self->sprs[i] = spr;
        }
        fn_1_3084(self->sprs[i], (u16)(1 << player));
        fn_1_FC08(self, i, (f64)i);
    }
    self->step = 0;
    M539_ROT_SET(self, 0x40490FDB, 3.1415927f);
    self->unkB8 = 0;
    self->lastDigit = 0;
    return self;
}

// NONMATCHING: register allocation only.
M539ScoreSpr *fn_1_102A0(M539ScoreSpr *self, s16 count)
{
    if (self != 0) {
        fn_1_F40C(self, 1);
        if (count > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_102F8(M539Model **arg0)
{
    *arg0 = 0;
}

void fn_1_10304(M539Drum **arg0)
{
    fn_1_11030(arg0, 0);
}

// NONMATCHING: register allocation only.
M539Drum *fn_1_10330(M539Drum *self, s16 count)
{
    if (self != 0) {
        fn_1_1270(&self->link, 0);
        if (count > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_10388(M539Drum *arg0)
{
    fn_1_6A24(arg0);
}

void fn_1_103B0(M539Drum *arg0)
{
    fn_1_6B44(arg0);
}

void fn_1_103D8(M539Drum *arg0)
{
    fn_1_6BEC(arg0);
}

void fn_1_10400(M539Game *self)
{
    M539Model *head = self->head;

    fn_1_2E40(head, 0x40000002);
    HuAudFXPlay(0x7E4);
}

void fn_1_1044C(M539Game *self)
{
    fn_1_C750(&self->digitFade, 0x12, 0.0f);
    HuAudFXPlay(0x7E5);
    if (self->player == 0) {
        fn_1_10504(self);
    }
}

void fn_1_104A8(M539Game *self)
{
    fn_1_C750(&self->digitFade, 0x12, 1.0f);
    if (self->player == 0) {
        HuAudFXPlay(0x7E5);
        fn_1_10504(self);
    }
}

// NONMATCHING: register allocation only.
void fn_1_10504(M539Game *self)
{
    M539Model *blockMdl = self->block;
    HSFOBJECT *block;
    Vec pos;
    M539Effect *effect;
    f64 x;
    f64 y;
    f64 z;

    block = fn_1_3150(blockMdl, (char *)lbl_1_data_5B7);
    z = 100.0f + block->mesh.base.pos.z;
    y = block->mesh.base.pos.y;
    x = block->mesh.base.pos.x - 20.0f;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    effect = (M539Effect *)fn_1_0(M539_ALLOC_SIZE(0x18));
    if (effect != 0) {
        effect = fn_1_C920(effect, &pos);
    }
}

// The original reads every handle through by-value copies (extra stack
// temporaries around each model/drum/effect access): not reproduced.
void fn_1_105F4(M539Game *self)
{
    Mtx mtx;
    s32 i;
    s32 j;
    M539Drum *elem;
    f32 roll;
    s32 hundreds;
    s32 tens;
    s32 ones;
    s32 tickDone;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            elem = self->digits[i][j];
            if (elem != 0) {
                fn_1_10388(elem);
            }
        }
    }
    Hu3DModelObjMtxGet(self->head->id, (char *)lbl_1_data_5BD, mtx);
    self->headPos.x = mtx[0][3];
    self->headPos.y = mtx[1][3];
    self->headPos.z = mtx[2][3];
    PSVECSubtract(&self->headPos, &self->lastPos, &self->delta);
    self->lastPos = self->headPos;
    fn_1_10F14(self);
    roll = fn_1_C7C8(&self->rollFade);
    hundreds = (s32)(roll / 100.0f);
    tens = (s32)(roll / 10.0f);
    ones = (s32)fmod((f64)roll, 10.0);
    tickDone = 0;
    if (self->tick > 0) {
        self->tick = self->tick - 1;
        if (self->tick == 0) {
            tickDone = 1;
        }
    }
    if ((s32)roll != self->lastDigit) {
        self->lastDigit = (s32)roll;
        HuAudFXPlay(lbl_1_rodata_390[self->player]);
        self->tick = 3;
    }
    if (self->effect != 0) {
        self->effect->value = (s32)roll;
    }
    fn_1_FC08(self, 2, (f32)hundreds);
    fn_1_FC08(self, 1, (f32)tens);
    fn_1_FC08(self, 0, (f32)ones);
}

// NONMATCHING: register allocation only.
void fn_1_10A90(M539Game *self, s32 arg)
{
    s32 i;
    s32 j;
    M539Drum *elem;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            elem = self->digits[i][j];
            if (elem != 0) {
                fn_1_6B44(elem);
            }
        }
    }
    fn_1_3084(self->head, (u16)(1 << self->player));
    self->loopSnd = HuAudFXPlay(lbl_1_rodata_3A0[self->player]);
}

// NONMATCHING: register allocation only.
void fn_1_10B58(M539Game *self, s32 arg)
{
    s32 i;
    s32 j;
    M539Drum *elem;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            elem = self->digits[i][j];
            if (elem != 0) {
                fn_1_6BEC(elem);
            }
        }
    }
}

// NONMATCHING: register allocation only.
void fn_1_10BE8(M539Game *self, s32 arg)
{
    s32 step = self->step;

    if (step < 8) {
        HuAudFXPlay(lbl_1_rodata_3B8[self->player]);
        fn_1_6B60(self->digits[(lbl_1_rodata_3B0[step] >> 4) & 3][lbl_1_rodata_3B0[step] & 3]);
        self->step = step + 1;
        if (self->step == 8) {
            HuAudFXStop(self->loopSnd);
        }
        M539_ROT_SET(self, 0, 0.0f);
        fn_1_2E40(self->block, 0x40000002);
        fn_1_3660(self->block, 0);
        HuAudFXPlay(lbl_1_rodata_3C8[self->player]);
    }
}

// NONMATCHING: register allocation only.
s32 fn_1_10CEC(M539Game *self, s32 arg1)
{
    M539Line line;
    M539Drum *model[3];
    s32 a;
    s32 b;
    s32 c;
    s32 i = 0;

    do {
        model[i] = self->digits[(lbl_1_rodata_3D8.cell[arg1 * 3 + i] >> 4) & 3][lbl_1_rodata_3D8.cell[arg1 * 3 + i] & 3];
        i++;
    } while (i < 3);
    fn_1_6E3C(&line);
    c = fn_1_6DBC(model[2]);
    b = fn_1_6DBC(model[1]);
    a = fn_1_6DBC(model[0]);
    if (fn_1_6E48(&line, a, b, c) != 0) {
        for (i = 0; i < 3; i++) {
            fn_1_6C64(model[i]);
        }
    }
    if (line.score != 0) {
        self->rollValue = self->rollValue + line.score;
        fn_1_C750(&self->rollFade, 0x24, (f32)self->rollValue);
        HuAudFXPlay(lbl_1_rodata_420[self->player]);
    }
    return line.score;
}

// NONMATCHING: register allocation only.
s32 fn_1_10E64(s32 arg)
{
    M539Game *node;

    if (lbl_1_bss_A04 == 0) {
        fn_1_1D4(lbl_1_data_59F, lbl_1_data_5AB, 0x20);
    }
    node = (M539Game *)lbl_1_bss_A04->next;
    for (;;) {
        if ((M539Node *)node == lbl_1_bss_A04) {
            return 1;
        }
        if (node->step != 8) {
            return 0;
        }
        node = (M539Game *)node->link.next;
    }
}

// NONMATCHING: register allocation only.
void fn_1_10F14(M539Game *self)
{
    HSFOBJECT *block;
    MtxPtr mtx;
    Mtx rot;
    f32 x;
    f32 y;
    f32 z;
    f32 scale;

    block = fn_1_3150(self->block, (char *)lbl_1_data_5B7);
    mtx = fn_1_30D8(self->block);
    x = block->mesh.base.pos.x;
    y = block->mesh.base.pos.y;
    z = block->mesh.base.pos.z;
    scale = fn_1_C7C8(&self->digitFade);
    PSMTXRotRad(rot, 'z', -0.2617994f);
    PSMTXTrans(mtx, -x, -y, -z);
    PSMTXScaleApply(mtx, mtx, scale, scale, scale);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXTransApply(mtx, mtx, x, y, z);
}

void fn_1_11030(M539Drum **arg0, M539Drum *arg1)
{
    *arg0 = arg1;
}

}
