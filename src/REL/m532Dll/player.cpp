// NONMATCHING: unit is logically complete but not byte-identical (inlined vector/destructor code simplified, register allocation differs); see per-function diffs
// m532Dll "player": the two-a-side team players. Each player sits on one of
// two intrusive lists (lbl_1_bss_AA0 / lbl_1_bss_AB4), owns a character model,
// a waypoint vector, a wrapping heading and a pointer-to-member state machine
// (M532Player::state). The functions here build the teams, step the states
// and drive the motions / rumble.
#define _MATH_H
extern "C" {
#include "dolphin.h"
#include "humath.h"
#include "game/hu3d.h"
#include "game/hsfex.h"
#include "game/object.h"
#include "game/gamework.h"
#include "game/audio.h"
#include "game/pad.h"
#include "game/charman.h"
}
#include "REL/m532Dll/player.h"
#ifdef TARGET_PC
#include <stdio.h>
#include <stdlib.h>
// shortcut: effect object type lives in another unit; twice the GameCube size
// covers its pointer fields on PC, replace with sizeof once its header exists.
#define M532_EFFECT_SIZE (0x48 * 2)
#else
#define M532_EFFECT_SIZE 0x48
#endif

extern "C" {

double atan2(double y, double x);
double fmod(double x, double y);
#ifndef TARGET_PC
void abort(void);
void __register_global_object(void *obj, void *dtor, void *reg);
extern u8 __files[];
#endif

// ---------------------------------------------------------------- externs

extern void *lbl_1_bss_8; // ball manager singleton
extern void *lbl_1_bss_A28; // game master singleton
extern const Vec lbl_1_rodata_3B0; // unk.cpp: {90, 116, 300}
extern const Vec lbl_1_rodata_3BC; // unk.cpp: {320, 0, 455}

void fn_1_32B0(M532Node *node);
void fn_1_3328(M532Node *node);
void fn_1_337C(M532Node *node, M532Node *before);
void *fn_1_32BC(M532Node *node, s16 flag);
void *fn_1_1B48(s32 size);
void fn_1_1C10(void *ptr);
void fn_1_1D1C(const char *expr, const char *file, s32 line);
void fn_1_1D7C(void *stream, const char *msg);
M532Model *fn_1_4CD8(M532Model *self, s16 charNo, s32 flag);
void fn_1_500C(M532Model *self, s32 arg);
void *fn_1_50B4(M532Model *self, s16 flag);
void fn_1_51A4(M532Model *self);
void fn_1_4EB4(M532Model *self, Vec *pos);
void fn_1_4EFC(M532Model *self, Vec *rot);
void fn_1_451C(s32 plyIdx);
s32 fn_1_6AA0(void);
f32 fn_1_6F88(f32 a, f32 b);
void fn_1_8130(void *obj, Vec *pos);
s32 fn_1_A6C0(void *ballMgr, Vec *pos);
s16 *fn_1_C66C(M532Vec16 *vec, s32 idx);
void fn_1_C6E4(M532Vec16 *vec, s16 *pos, s32 n, s16 *val);
s32 fn_1_CEF0(void *gm, s32 side);
void *fn_1_EB84(s32 handle);
u8 fn_1_1498(void *ball);
Vec *fn_1_14A4(void *ball);
Vec *fn_1_14AC(void *ball, Vec *out, f32 t);
void fn_1_157C(void *ball, Vec *pos);
void fn_1_15D8(void *ball, Vec *target);

// ---------------------------------------------------------------- prototypes

s16 fn_1_EE18(Vec *pos);
void fn_1_EEB8(s32 seId, Vec *pos);
void fn_1_EF70(void);
void fn_1_F03C(M532Node *list, s32 side);
void fn_1_F1F8(void);
void fn_1_F23C(M532Player *p);
void fn_1_F27C(void);
void fn_1_F2F8(s32 arg);
s32 fn_1_F38C(f32 *outX);
s32 fn_1_F514(void);
void fn_1_F5C4(void);
void fn_1_F6E0(s32 side);
void fn_1_F9C8(void);
void fn_1_FA04(M532Node *list, s32 unused);
s32 fn_1_FA78(void);
void fn_1_FB08(s32 side);
void fn_1_FC24(M532Node *list, s32 motion);
void fn_1_FD80(void);
M532Player *fn_1_FE8C(M532Player *p, s32 idx);
void *fn_1_10174(M532VecV *self, s16 flag);
void *fn_1_101D4(M532VecV *self, s16 flag);
void *fn_1_1022C(M532Node *self, s16 flag);
void *fn_1_10284(M532Player *p, s16 flag);
void fn_1_10370(M532Player *p);
void fn_1_10374(M532Player *p, s32 arg);
void fn_1_106A0(M532Player *p);
u8 fn_1_107FC(M532Player *p);
void fn_1_10860(M532Player *p, s32 id);
void fn_1_108A8(M532Player *p);
void fn_1_10A88(M532Player *p);
void fn_1_10BD0(M532Player *p);
void fn_1_10C5C(M532Player *p);
void fn_1_10FE0(M532Player *p, s32 id);
void fn_1_11028(M532Player *p);
void fn_1_111A4(M532Player *p);
void fn_1_11340(M532Player *p, s32 id);
void fn_1_11388(M532Player *p);
void fn_1_11430(M532Player *p);
void fn_1_11518(M532Player *p);
void fn_1_1162C(M532Player *p);
void fn_1_116CC(M532Player *p);
void fn_1_11788(M532Player *p);
void fn_1_1180C(M532Player *p);
void fn_1_118FC(M532Player *p);
void fn_1_11988(M532Player *p);
M532Player *fn_1_1198C(M532Player *p);
u8 fn_1_119F4(M532Player *p);
void fn_1_11BFC(M532Player *p);
s32 fn_1_11D00(M532Player *p);
Vec *fn_1_11D20(M532Player *p, Vec *out);
Vec *fn_1_11DA0(M532Player *p, Vec *out);
Vec *fn_1_11E98(M532Player *p, Vec *out);
void fn_1_11F10(M532Player *p);
u8 fn_1_11FAC(M532Player *p, const Vec *target);
void fn_1_120C4(M532Player *p);
void fn_1_1215C(M532Player *p);
void fn_1_12254(M532Player *p, s32 id);
void fn_1_122CC(M532Player *p, s32 motion, u8 loop);
f32 fn_1_123F8(M532Player *p);
void fn_1_12464(M532Player *p, s32 kind);
u8 fn_1_1259C(s32 padNo);
u8 fn_1_1260C(void);
void fn_1_12884(M532Range *r, f32 angle);
f32 fn_1_128C0(M532Range *r, f32 target);
void fn_1_128FC(M532Range *r, f32 target, f64 rate);
f32 fn_1_12970(M532Range *r, f32 angle);
f32 fn_1_12A0C(M532Range *r);
void fn_1_12A14(M532Node *list, void (*fn)(M532Player *));
void fn_1_12A74(M532Node *first, M532Node *head, void (*fn)(M532Player *));
M532Range *fn_1_12AD4(M532Range *r, f32 a, f32 b, f32 c);
Vec *fn_1_12C38(M532VecV *v, Vec *pos);
void fn_1_12CF8(M532VecV *v, Vec *pos, u32 n, const Vec *val);
void *fn_1_132A8(M532VecV *v, s16 flag);
void fn_1_13334(M532VecV *v);
f32 fn_1_13390(M532Range *r, f32 cur, f32 target);
void fn_1_135A8(void);
void *fn_1_1368C(M532Node *self, s16 flag);

// ---------------------------------------------------------------- data

#define M532_ASSERT_PTR(p) ((p) != 0 ? (void)0 : fn_1_1D1C(lbl_1_data_6D8, lbl_1_data_6E8, 0x27))
#define M532_FIRST(list) ((M532Player *)(list).next)
#define M532_NEXT(p) ((M532Player *)(p)->link.next)
#define M532_RAD2DEG(y) ((f32)(180.0 * ((f64)(f32)(y) / 3.141592653589793)))

static const Vec lbl_1_rodata_3C8[3] = {
    { 265.0f, 98.0f, 207.0f },
    { 205.0f, 98.0f, 155.0f },
    { 145.0f, 98.0f, 102.0f },
};
static const Vec lbl_1_rodata_3EC[3] = {
    { 336.0f, 280.0f, 126.0f },
    { 275.0f, 280.0f, 75.0f },
    { 213.0f, 280.0f, 23.0f },
};
static const Vec lbl_1_rodata_410 = { 0.0f, 156.0f, 335.0f };
static const Vec lbl_1_rodata_41C = { 245.0f, 98.0f, 305.0f };
static const Vec lbl_1_rodata_428 = { 210.0f, 98.0f, 314.0f };
static const Vec lbl_1_rodata_434 = { 315.0f, 3.5f, 427.0f };
static const s32 lbl_1_rodata_474[8] = { -4, 0, -2, 0, -1, 0, 0, 0 };
static const f32 lbl_1_rodata_4D4[13] = {
    170.0f, 170.0f, 210.0f, 190.0f, 170.0f, 210.0f, 240.0f,
    170.0f, 170.0f, 170.0f, 170.0f, 170.0f, 170.0f,
};
static const u32 lbl_1_rodata_564[4] = { 0x80000000, 0x40000000, 0x20000000, 0x10000000 };

M532Ptmf lbl_1_data_6A8 = M532_PTMF_INIT(fn_1_10A88);
M532Ptmf lbl_1_data_6B4 = M532_PTMF_INIT(fn_1_10A88);
M532Ptmf lbl_1_data_6C0 = M532_PTMF_INIT(fn_1_11788);
M532Ptmf lbl_1_data_6CC = M532_PTMF_INIT(fn_1_11788);
char lbl_1_data_6D8[] = "ms_pointer != 0";
char lbl_1_data_6E8[] = "singleton.h";
M532Ptmf lbl_1_data_6F4 = M532_PTMF_INIT(fn_1_11988);
M532Ptmf lbl_1_data_700 = M532_PTMF_INIT(fn_1_11988);
M532Ptmf lbl_1_data_70C = M532_PTMF_INIT(fn_1_118FC);
M532Ptmf lbl_1_data_718 = M532_PTMF_INIT(fn_1_10370);
M532Ptmf lbl_1_data_724 = M532_PTMF_INIT(fn_1_106A0);
M532Ptmf lbl_1_data_730 = M532_PTMF_INIT(fn_1_11988);
M532Ptmf lbl_1_data_73C = M532_PTMF_INIT(fn_1_11988);
M532Ptmf lbl_1_data_748 = M532_PTMF_INIT(fn_1_108A8);
M532Ptmf lbl_1_data_754 = M532_PTMF_INIT(fn_1_10A88);
M532Ptmf lbl_1_data_760 = M532_PTMF_INIT(fn_1_10BD0);
M532Ptmf lbl_1_data_76C = M532_PTMF_INIT(fn_1_10C5C);
M532Ptmf lbl_1_data_778 = M532_PTMF_INIT(fn_1_11988);
M532Ptmf lbl_1_data_784 = M532_PTMF_INIT(fn_1_11028);
M532Ptmf lbl_1_data_790 = M532_PTMF_INIT(fn_1_111A4);
M532Ptmf lbl_1_data_79C = M532_PTMF_INIT(fn_1_11388);
M532Ptmf lbl_1_data_7A8 = M532_PTMF_INIT(fn_1_11388);
M532Ptmf lbl_1_data_7B4 = M532_PTMF_INIT(fn_1_11988);
M532Ptmf lbl_1_data_7C0 = M532_PTMF_INIT(fn_1_11518);
M532Ptmf lbl_1_data_7CC = M532_PTMF_INIT(fn_1_1180C);
M532Ptmf lbl_1_data_7D8 = M532_PTMF_INIT(fn_1_116CC);
M532Ptmf lbl_1_data_7E4 = M532_PTMF_INIT(fn_1_1180C);
M532Ptmf lbl_1_data_7F0 = M532_PTMF_INIT(fn_1_11988);
M532Ptmf lbl_1_data_7FC = M532_PTMF_INIT(fn_1_11988);
M532Ptmf lbl_1_data_808 = M532_PTMF_INIT(fn_1_11988);
char lbl_1_data_814[] = "m_motion_id.size() == NUMBEROF_MOTION";
char lbl_1_data_83A[] = "player.cpp\0vector::at index out of range";
char lbl_1_data_863[] = "vector::insert length error\n";
char lbl_1_data_880[] = "Memory allocation failure";

s32 lbl_1_bss_A80;
s32 lbl_1_bss_A84;
s32 lbl_1_bss_A88;
s32 lbl_1_bss_A8C;
s32 lbl_1_bss_A90;
u8 lbl_1_bss_A94[0xC]; // global-destructor registration records
M532Node lbl_1_bss_AA0;
u8 lbl_1_bss_AA8[0xC];
M532Node lbl_1_bss_AB4;

// ---------------------------------------------------------------- helpers

// Message to stderr then abort (MSL fputs on the GameCube).
static void M532Fatal(const char *msg)
{
#ifdef TARGET_PC
    fputs(msg, stderr);
#else
    fn_1_1D7C(__files + 0xA0, msg);
#endif
    abort();
}

// ---------------------------------------------------------------- audio pan

s16 fn_1_EE18(Vec *pos)
{
    Vec scr;
    s16 pan;
    s16 v;

    Hu3D3Dto2D(pos, 1, &scr);
    scr.x = scr.x * 0.11111111f;
    v = (s16)(32.0f + scr.x);
    if (v < 0) {
        pan = 0;
    } else if (v > 0x7F) {
        pan = 0x7F;
    } else {
        pan = v;
    }
    return pan;
}

void fn_1_EEB8(s32 seId, Vec *pos)
{
    HuAudFXPlayPan(seId, fn_1_EE18(pos));
}

// ---------------------------------------------------------------- team setup

void fn_1_EF70(void)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        M532Player *p = (M532Player *)fn_1_1B48(sizeof(M532Player));
        if (p != NULL) {
            fn_1_FE8C(p, i);
        }
    }
    fn_1_F03C(&lbl_1_bss_AA0, 0);
    fn_1_F03C(&lbl_1_bss_AB4, 1);
    fn_1_122CC(M532_FIRST(lbl_1_bss_AA0), 2, 1);
    fn_1_122CC(M532_FIRST(lbl_1_bss_AB4), 2, 1);
    CharEffectLayerSet(3);
}

void fn_1_F03C(M532Node *list, s32 side)
{
    M532Player *a;
    M532Player *b;
    s32 sgn;

    if ((fn_1_6AA0() & 1) != 0) {
        M532Node *first = list->next;
        fn_1_3328(first);
        fn_1_337C(first, list);
    }
    a = (M532Player *)list->next;
    b = M532_NEXT(a);
    sgn = (side == 0) ? -1 : 1;
    a->pos = lbl_1_rodata_3B0;
    a->pos.x = a->pos.x * (f32)sgn;
    a->tgtAng = (f32)(sgn * -0x2D);
    fn_1_12884(&a->range, a->tgtAng);
    b->pos = lbl_1_rodata_3BC;
    b->pos.x = b->pos.x * (f32)sgn;
    b->tgtAng = (f32)(sgn * -0x87);
    fn_1_12884(&b->range, b->tgtAng);
}

void fn_1_F1F8(void)
{
    fn_1_12A14(&lbl_1_bss_AA0, fn_1_F23C);
    fn_1_12A14(&lbl_1_bss_AB4, fn_1_F23C);
}

void fn_1_F23C(M532Player *p)
{
    M532PtmfCall(p, &p->state);
    fn_1_11BFC(p);
}

void fn_1_F27C(void)
{
    fn_1_122CC(M532_FIRST(lbl_1_bss_AA0), 0, 1);
    fn_1_122CC(M532_FIRST(lbl_1_bss_AB4), 0, 1);
}

void fn_1_F2F8(s32 arg)
{
    M532Player *firsts[2];
    M532Player **it;

    firsts[0] = M532_FIRST(lbl_1_bss_AA0);
    firsts[1] = M532_FIRST(lbl_1_bss_AB4);
    for (it = firsts; it != firsts + 2; it++) {
        fn_1_10374(*it, arg);
    }
}

// Decides who serves / wins the point; returns the outcome, X sign in *outX.
s32 fn_1_F38C(f32 *outX)
{
    M532Player *p1 = M532_FIRST(lbl_1_bss_AB4);
    s32 a = fn_1_11D00(M532_FIRST(lbl_1_bss_AA0));
    s32 b = fn_1_11D00(p1);

    if (a < 0 && b >= 0) {
        *outX = -lbl_1_rodata_3B0.x;
        return 1;
    }
    if (b < 0 && a >= 0) {
        *outX = lbl_1_rodata_3B0.x;
        return 0;
    }
    if (a < 0 && b < 0) {
        if (a > b) {
            *outX = lbl_1_rodata_3B0.x;
            return 0;
        }
        if (a == b && (fn_1_6AA0() & 1) != 0) {
            *outX = lbl_1_rodata_3B0.x;
            return 0;
        }
        *outX = -lbl_1_rodata_3B0.x;
        return 1;
    }
    if (a >= lbl_1_bss_A84 && b >= lbl_1_bss_A84) {
        *outX = 0.0f;
        return 3;
    }
    if (a < b || (b >= a && (fn_1_6AA0() & 1) != 0)) {
        *outX = -lbl_1_rodata_3B0.x;
        return 0;
    }
    *outX = lbl_1_rodata_3B0.x;
    return 1;
}

s32 fn_1_F514(void)
{
    M532Player *p0 = M532_FIRST(lbl_1_bss_AA0);
    M532Player *p1 = M532_FIRST(lbl_1_bss_AB4);

    if (M532PtmfIs(&p0->state, lbl_1_data_6A8) || M532PtmfIs(&p1->state, lbl_1_data_6B4)) {
        return 1;
    }
    if (p0->target == 0 && p1->target == 0) {
        return 1;
    }
    return 0;
}

void fn_1_F5C4(void)
{
    M532Player *p0 = M532_FIRST(lbl_1_bss_AA0);
    M532Player *p1 = M532_FIRST(lbl_1_bss_AB4);

    if (fn_1_11FAC(p0, &lbl_1_rodata_3B0) != 0) {
        fn_1_122CC(p0, 1, 1);
    }
    p0->state = lbl_1_data_6C0;
    if (fn_1_11FAC(p1, &lbl_1_rodata_3B0) != 0) {
        fn_1_122CC(p1, 1, 1);
    }
    p1->state = lbl_1_data_6CC;
}

void fn_1_F6E0(s32 side)
{
    M532Player *p0 = M532_FIRST(lbl_1_bss_AA0);
    M532Player *p1 = M532_FIRST(lbl_1_bss_AB4);
    u8 neg0 = (u8)((u32)fn_1_11D00(p0) >> 31);
    u8 neg1 = (u8)((u32)fn_1_11D00(p1) >> 31);
    Vec v;
    s32 id;

    switch (side) {
    case 0:
        if (neg1 != 0) {
            M532_ASSERT_PTR(lbl_1_bss_8);
            fn_1_10FE0(p1, fn_1_A6C0(lbl_1_bss_8, fn_1_11E98(p1, &v)));
            return;
        }
        M532_ASSERT_PTR(lbl_1_bss_8);
        id = fn_1_A6C0(lbl_1_bss_8, fn_1_11DA0(p0, &v));
        fn_1_10860(p0, id);
        fn_1_11340(p1, id);
        return;
    case 1:
        if (neg0 != 0) {
            M532_ASSERT_PTR(lbl_1_bss_8);
            fn_1_10FE0(p0, fn_1_A6C0(lbl_1_bss_8, fn_1_11E98(p0, &v)));
            return;
        }
        M532_ASSERT_PTR(lbl_1_bss_8);
        id = fn_1_A6C0(lbl_1_bss_8, fn_1_11DA0(p1, &v));
        fn_1_10860(p1, id);
        fn_1_11340(p0, id);
        return;
    default:
        M532_ASSERT_PTR(lbl_1_bss_8);
        id = fn_1_A6C0(lbl_1_bss_8, (Vec *)&lbl_1_rodata_410);
        fn_1_11340(p0, id);
        fn_1_11340(p1, id);
        return;
    }
}

void fn_1_F9C8(void)
{
    fn_1_FA04(&lbl_1_bss_AA0, 0);
    fn_1_FA04(&lbl_1_bss_AB4, 1);
}

// Front player is rotated to the back of the list.
void fn_1_FA04(M532Node *list, s32 unused)
{
    M532Player *first = (M532Player *)list->next;
    M532Player *second = M532_NEXT(first);

    fn_1_11430(first);
    fn_1_1162C(second);
    fn_1_12464(second, 0);
    fn_1_3328(&first->link);
    fn_1_337C(&first->link, list);
}

s32 fn_1_FA78(void)
{
    s32 r = 0;
    M532Player *p1 = M532_FIRST(lbl_1_bss_AB4);

    if (M532PtmfIs(&M532_FIRST(lbl_1_bss_AA0)->state, lbl_1_data_6F4)
        && M532PtmfIs(&p1->state, lbl_1_data_700)) {
        r = 1;
    }
    return r;
}

void fn_1_FB08(s32 side)
{
    switch (side) {
    case 0:
        fn_1_FC24(&lbl_1_bss_AA0, 7);
        fn_1_FC24(&lbl_1_bss_AB4, 8);
        return;
    case 1:
        fn_1_FC24(&lbl_1_bss_AA0, 8);
        fn_1_FC24(&lbl_1_bss_AB4, 7);
        return;
    default:
        fn_1_FC24(&lbl_1_bss_AA0, 8);
        fn_1_FC24(&lbl_1_bss_AB4, 8);
        CharLoseVoicePlay((s16)M532_FIRST(lbl_1_bss_AA0)->charNo,
                          (s16)M532_NEXT(M532_FIRST(lbl_1_bss_AA0))->charNo,
                          (s16)M532_FIRST(lbl_1_bss_AB4)->charNo,
                          (s16)M532_NEXT(M532_FIRST(lbl_1_bss_AB4))->charNo);
        return;
    }
}

// Every player of the list turns to the camera and plays the given motion.
void fn_1_FC24(M532Node *list, s32 motion)
{
    Vec cam;
    Vec up;
    M532Player *p = (M532Player *)list->next;

    Hu3DCameraPosGet(1, &cam, &up, &up);
    while ((M532Node *)p != list) {
        p->tgtAng = M532_RAD2DEG(atan2(cam.x - p->pos.x, cam.z - p->pos.z));
        p->state = lbl_1_data_70C;
        fn_1_122CC(p, motion, 0);
        if (motion == 7) {
            fn_1_451C(p->plyIdx);
        }
        p = M532_NEXT(p);
    }
}

void fn_1_FD80(void)
{
    u8 empty;

    do {
        empty = 0;
        if (lbl_1_bss_AA0.next == &lbl_1_bss_AA0 && lbl_1_bss_AA0.next == lbl_1_bss_AA0.prev) {
            empty = 1;
        }
        if (empty == 0) {
            fn_1_10284((M532Player *)lbl_1_bss_AA0.prev, 1);
        }
    } while (empty == 0);
    do {
        empty = 0;
        if (lbl_1_bss_AB4.next == &lbl_1_bss_AB4 && lbl_1_bss_AB4.next == lbl_1_bss_AB4.prev) {
            empty = 1;
        }
        if (empty == 0) {
            fn_1_10284((M532Player *)lbl_1_bss_AB4.prev, 1);
        }
    } while (empty == 0);
}

// Player constructor.
M532Player *fn_1_FE8C(M532Player *p, s32 idx)
{
    GWPLAYERCONF *conf = &GwPlayerConf[idx];
    M532Node *head;
    M532Node *n;
    s32 side;
    s32 count;

    fn_1_32B0(&p->link);
    fn_1_4CD8(&p->model, conf->charNo, 1);
    p->plyIdx = idx;
    p->charNo = conf->charNo;
    p->team = conf->grp;
    p->padNo = (conf->type != 0) ? -1 : conf->padNo;
    p->dif = conf->dif;
    p->target = 0;
    p->t28 = 0;
    p->t2C = 0;
    p->t30 = 0;
    p->path.cap = 0;
    p->path.size = 0;
    p->path.items = 0;
    fn_1_12AD4(&p->range, 0.0f, 360.0f, 0.0f);
    p->timer = 0;
    p->state = lbl_1_data_718;
    p->motions.cap = 0;
    p->motions.size = 0;
    p->motions.items = 0;
    p->curMot = -1;
    fn_1_500C(&p->model, 3);
    fn_1_51A4(&p->model);

    head = (p->team == 0) ? &lbl_1_bss_AA0 : &lbl_1_bss_AB4;
    side = p->team;
    count = 0;
    for (n = head->next; n != head; n = n->next) {
        count++;
    }
    if (count >= 2) {
        side = (p->team == 0) ? 1 : 0;
    }
    fn_1_337C(&p->link, (side == 0) ? &lbl_1_bss_AA0 : &lbl_1_bss_AB4);
    p->side = (side == 0) ? 0 : 1;
    fn_1_1215C(p);
    fn_1_122CC(p, 0, 1);
    p->pos.x = 0.0f;
    p->pos.y = 0.0f;
    p->pos.z = 0.0f;
    return p;
}

// ---------------------------------------------------------------- destructors

void *fn_1_10174(M532VecV *self, s16 flag)
{
    if (self != NULL) {
        if (self != NULL) {
            fn_1_132A8(self, 0);
        }
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

void *fn_1_101D4(M532VecV *self, s16 flag)
{
    if (self != NULL) {
        fn_1_132A8(self, 0);
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

void *fn_1_1022C(M532Node *self, s16 flag)
{
    if (self != NULL) {
        fn_1_32BC(self, 0);
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

// Player destructor.
void *fn_1_10284(M532Player *p, s16 flag)
{
    if (p != NULL) {
        if (p->motions.items != NULL) {
            fn_1_1C10(p->motions.items);
        }
        fn_1_132A8(&p->path, 0);
        fn_1_50B4(&p->model, 0);
        fn_1_32BC(&p->link, 0);
        if (flag > 0) {
            fn_1_1C10(p);
        }
    }
    return p;
}

// ---------------------------------------------------------------- states

void fn_1_10370(M532Player *p)
{
}

void fn_1_10374(M532Player *p, s32 arg)
{
    s32 tbl[8];
    s32 i;
    s32 r;

    p->t28 = lbl_1_bss_A80;
    p->t2C = arg;
    for (i = 0; i < 8; i++) {
        tbl[i] = lbl_1_rodata_474[i];
    }
    tbl[1] = (s32)((1.1 * (f64)lbl_1_bss_A84) - (f64)15);
    tbl[3] = (s32)((f64)lbl_1_bss_A84 - (f64)15);
    tbl[5] = (s32)((0.5 * (f64)lbl_1_bss_A84) - (f64)15);
    tbl[7] = (s32)((0.4 * (f64)lbl_1_bss_A84) - (f64)15);
    r = (s32)fn_1_6F88((f32)tbl[p->dif * 2], (f32)tbl[p->dif * 2 + 1]);
    if (r >= 0) {
        r += 15;
    }
    p->t30 = arg - r;
    p->state = lbl_1_data_724;
}

void fn_1_106A0(M532Player *p)
{
    if (p->t2C == p->t28) {
        fn_1_12464(p, 1);
    }
    if (fn_1_107FC(p) != 0) {
        fn_1_122CC(p, 2, 1);
    } else {
        if (fn_1_11D00(p) < lbl_1_bss_A84) {
            u8 expired = 0;
            M532Player *opp;

            if (p->t28 > 0) {
                p->t28 = p->t28 - 1;
                if (p->t28 == 0) {
                    expired = 1;
                }
            }
            if (expired == 0) {
                opp = fn_1_1198C(p);
                if (M532PtmfIs(&opp->state, lbl_1_data_730) && fn_1_11D00(opp) < 0) {
                    p->t28 = 0;
                } else {
                    return;
                }
            }
        }
        p->t28 = 0;
    }
    p->state = lbl_1_data_73C;
}

u8 fn_1_107FC(M532Player *p)
{
    if (p->padNo >= 0) {
        return (HuPadBtnDown[p->padNo] & 0x100) != 0;
    }
    return p->t30 == p->t28;
}

void fn_1_10860(M532Player *p, s32 id)
{
    p->target = id;
    p->state = lbl_1_data_748;
}

void fn_1_108A8(M532Player *p)
{
    void *ball = fn_1_EB84(p->target);
    Vec pred;
    Vec hand;

    if (ball != NULL) {
        fn_1_14AC(ball, &pred, 0.21666667f);
        fn_1_11DA0(p, &hand);
        hand.y += CharModelHeightGet((s16)p->charNo) * 0.5f;
        PSVECSubtract(&pred, &hand, &hand);
        if (PSVECMag(&hand) < 50.0) {
            fn_1_122CC(p, 3, 0);
            fn_1_122CC(fn_1_1198C(p), 0, 1);
            HuAudFXPlayPan(0x7AD, fn_1_EE18(fn_1_11D20(p, &pred)));
            p->state = lbl_1_data_754;
        }
    }
}

void fn_1_10A88(M532Player *p)
{
    s32 lv;
    s32 idx;

    if (!(fn_1_123F8(p) < 12.0f)) {
        fn_1_11F10(p);
        if (CharMotionEndCheck((s16)p->charNo) != 0) {
            M532_ASSERT_PTR(lbl_1_bss_A28);
            lv = fn_1_CEF0(lbl_1_bss_A28, p->side) - 1;
            if (lv < 0) {
                idx = 0;
            } else if (lv > 2) {
                idx = 2;
            } else {
                idx = lv;
            }
            fn_1_11FAC(p, &lbl_1_rodata_3C8[idx]);
            fn_1_122CC(p, 5, 1);
            p->state = lbl_1_data_760;
        }
    }
}

void fn_1_10BD0(M532Player *p)
{
    fn_1_11F10(p);
    if (fn_1_119F4(p) != 0) {
        fn_1_122CC(p, 6, 0);
        p->state = lbl_1_data_76C;
    }
}

void fn_1_10C5C(M532Player *p)
{
    Vec tgt;
    Vec t2;
    void *ball;
    s32 lv;
    s32 idx;
    s32 idx2;

    fn_1_11F10(p);
    M532_ASSERT_PTR(lbl_1_bss_A28);
    lv = fn_1_CEF0(lbl_1_bss_A28, p->side) - 1;
    if (lv < 0) {
        idx = 0;
    } else if (lv > 2) {
        idx = 2;
    } else {
        idx = lv;
    }
    tgt = lbl_1_rodata_3EC[idx];
    if (p->side == 0) {
        tgt.x *= -1.0f;
    }
    PSVECSubtract(&tgt, &p->pos, &tgt);
    fn_1_128FC(&p->range, M532_RAD2DEG(atan2(tgt.x, tgt.z)), 0.1);
    if (!(fn_1_123F8(p) < 15.0f)) {
        ball = fn_1_EB84(p->target);
        if (ball != NULL) {
            M532_ASSERT_PTR(lbl_1_bss_A28);
            lv = fn_1_CEF0(lbl_1_bss_A28, p->side) - 1;
            if (lv < 0) {
                idx2 = 0;
            } else if (lv > 2) {
                idx2 = 2;
            } else {
                idx2 = lv;
            }
            t2 = lbl_1_rodata_3EC[idx2];
            if (p->side == 0) {
                t2.x *= -1.0f;
            }
            fn_1_15D8(ball, &t2);
            HuAudFXPlayPan(0x7B0, fn_1_EE18(fn_1_14A4(ball)));
            p->target = 0;
        }
        if (CharMotionEndCheck((s16)p->charNo) != 0) {
            p->timer = 0x3C;
            fn_1_122CC(p, 0, 1);
            p->state = lbl_1_data_778;
        }
    }
}

void fn_1_10FE0(M532Player *p, s32 id)
{
    p->target = id;
    p->state = lbl_1_data_784;
}

void fn_1_11028(M532Player *p)
{
    void *ball = fn_1_EB84(p->target);
    f32 tbl[13];
    Vec ref;
    Vec pred;
    s32 i;

    if (ball != NULL) {
        for (i = 0; i < 13; i++) {
            tbl[i] = lbl_1_rodata_4D4[i];
        }
        ref.x = p->pos.x;
        ref.y = p->pos.y + tbl[p->charNo];
        ref.z = p->pos.z;
        fn_1_14AC(ball, &pred, 0.083333336f);
        if (!(pred.y > ref.y)) {
            fn_1_122CC(p, 4, 0);
            fn_1_12464(p, 2);
            p->state = lbl_1_data_790;
        }
    }
}

void fn_1_111A4(M532Player *p)
{
    void *ball;
    M532Player *opp;
    Vec v;

    if (!(fn_1_123F8(p) < 5.0f)) {
        ball = fn_1_EB84(p->target);
        if (ball != NULL) {
            opp = fn_1_1198C(p);
            fn_1_15D8(ball, fn_1_11DA0(opp, &v));
            HuAudFXPlayPan(0x7AF, fn_1_EE18(fn_1_14A4(ball)));
            opp->target = p->target;
            opp->state = lbl_1_data_748;
            fn_1_122CC(opp, 2, 1);
            p->target = 0;
            p->state = lbl_1_data_79C;
        }
    }
}

void fn_1_11340(M532Player *p, s32 id)
{
    p->target = id;
    p->state = lbl_1_data_7A8;
}

void fn_1_11388(M532Player *p)
{
    void *ball = fn_1_EB84(p->target);

    if (ball == NULL || fn_1_1498(ball) != 0) {
        p->target = 0;
        fn_1_122CC(p, 0, 1);
        p->state = lbl_1_data_7B4;
    }
}

void fn_1_11430(M532Player *p)
{
    fn_1_11FAC(p, &lbl_1_rodata_3C8[0]);
    fn_1_11FAC(p, &lbl_1_rodata_41C);
    fn_1_11FAC(p, &lbl_1_rodata_434);
    fn_1_11FAC(p, &lbl_1_rodata_3BC);
    fn_1_122CC(p, (p->timer <= 0) ? 1 : 0, 1);
    p->state = lbl_1_data_7C0;
}

void fn_1_11518(M532Player *p)
{
    u8 expired = 0;

    if (p->timer > 0) {
        p->timer = p->timer - 1;
        if (p->timer == 0) {
            expired = 1;
        }
    }
    if (expired != 0) {
        fn_1_122CC(p, 1, 1);
    }
    if (p->timer <= 0 && fn_1_119F4(p) != 0) {
        p->tgtAng = (f32)((p->side == 0) ? 0x87 : -0x87);
        p->state = lbl_1_data_7CC;
    }
}

void fn_1_1162C(M532Player *p)
{
    fn_1_11FAC(p, &lbl_1_rodata_434);
    fn_1_11FAC(p, &lbl_1_rodata_428);
    fn_1_11FAC(p, &lbl_1_rodata_3B0);
    fn_1_122CC(p, 1, 1);
    p->state = lbl_1_data_7D8;
}

void fn_1_116CC(M532Player *p)
{
    if (fn_1_119F4(p) != 0) {
        p->tgtAng = (f32)((p->side == 0) ? 0x2D : -0x2D);
        p->state = lbl_1_data_7E4;
    }
}

void fn_1_11788(M532Player *p)
{
    if (fn_1_119F4(p) != 0) {
        fn_1_122CC(p, 0, 1);
        p->state = lbl_1_data_7F0;
    }
}

void fn_1_1180C(M532Player *p)
{
    f32 d;

    fn_1_128FC(&p->range, p->tgtAng, 0.1);
    d = fn_1_128C0(&p->range, p->tgtAng);
    if ((d < 0.0f ? -(f64)d : (f64)d) < 1.0) {
        fn_1_12884(&p->range, p->tgtAng);
        fn_1_122CC(p, 0, 1);
        p->state = lbl_1_data_7FC;
    }
}

void fn_1_118FC(M532Player *p)
{
    fn_1_128FC(&p->range, p->tgtAng, 0.1);
    if (CharMotionEndCheck((s16)p->charNo) != 0) {
        p->state = lbl_1_data_808;
    }
}

void fn_1_11988(M532Player *p)
{
}

// ---------------------------------------------------------------- player helpers

// The player on the other team's front.
M532Player *fn_1_1198C(M532Player *p)
{
    M532Player *r;

    if (p->side == 0) {
        r = M532_FIRST(lbl_1_bss_AB4);
    } else {
        r = M532_FIRST(lbl_1_bss_AA0);
    }
    return r;
}

// Walks towards the first waypoint (max 10/3 per call); returns 1 when none left.
u8 fn_1_119F4(M532Player *p)
{
    Vec d;
    f32 mag;
    f64 step;

    if (p->path.size == 0) {
        return 1;
    }
    PSVECSubtract(p->path.items, &p->pos, &d);
    mag = PSVECMag(&d);
    if (3.3333333333333335 < (f64)mag) {
        step = 3.3333333333333335;
    } else {
        step = (f64)mag;
    }
    if (step <= 0.0) {
        fn_1_12C38(&p->path, p->path.items);
        return p->path.size == 0;
    }
    fn_1_128FC(&p->range, M532_RAD2DEG(atan2(d.x, d.z)), 0.1);
    PSVECScale(&d, &d, (f32)(step / (f64)mag));
    PSVECAdd(&d, &p->pos, &p->pos);
    return 0;
}

// Per-frame model sync: position, heading and voice pan.
void fn_1_11BFC(M532Player *p)
{
    Vec rot;

    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    rot.y = fn_1_12A0C(&p->range);
    fn_1_4EB4(&p->model, &p->pos);
    fn_1_4EFC(&p->model, &rot);
    CharModelVoicePanSet((s16)p->charNo, fn_1_EE18(&p->pos));
}

// Remaining points: limit - counter.
s32 fn_1_11D00(M532Player *p)
{
    return p->t2C - p->t28;
}

// Position of the hand hook.
Vec *fn_1_11D20(M532Player *p, Vec *out)
{
    char *hook = CharModelItemHookGet((s16)p->charNo, 2, 0);

    Hu3DModelObjPosGet(p->model.id, hook, out);
    return out;
}

// Spot in front of the player where a served ball is dropped.
Vec *fn_1_11DA0(M532Player *p, Vec *out)
{
    f32 s;

    out->x = p->pos.x;
    out->y = p->pos.y;
    out->z = p->pos.z;
    if (out->x == 0.0f) {
        s = 0.0f;
    } else {
        f32 t;
        if (out->x < 0.0f) {
            t = -1.0f;
        } else {
            t = 1.0f;
        }
        s = t;
    }
    out->x = (f32)((f64)out->x + (100.0 * (0.3 * -s)));
    out->z = (f32)((f64)out->z + 30.0);
    return out;
}

Vec *fn_1_11E98(M532Player *p, Vec *out)
{
    out->x = p->pos.x;
    out->y = p->pos.y;
    out->z = p->pos.z;
    out->y += CharModelHeightGet((s16)p->charNo) * 0.5f;
    return out;
}

void fn_1_11F10(M532Player *p)
{
    Vec v;
    void *ball = fn_1_EB84(p->target);

    if (ball != NULL) {
        char *hook = CharModelItemHookGet((s16)p->charNo, 2, 0);
        Hu3DModelObjPosGet(p->model.id, hook, &v);
        fn_1_157C(ball, &v);
    }
}

// Queue a waypoint (x mirrored for side 0); returns 1 if still far from it.
u8 fn_1_11FAC(M532Player *p, const Vec *target)
{
    Vec t;
    f32 mag;

    t.x = target->x;
    t.y = target->y;
    t.z = target->z;
    if (p->side == 0) {
        t.x *= -1.0f;
    }
    fn_1_12CF8(&p->path, p->path.items + p->path.size, 1, &t);
    PSVECSubtract(&p->pos, &t, &t);
    mag = PSVECMag(&t);
    return mag >= 3.3333333f;
}

void fn_1_120C4(M532Player *p)
{
    Vec v;
    f64 h;
    void *eff;

    v.x = p->pos.x;
    v.y = p->pos.y;
    v.z = p->pos.z;
    h = 50.0 + CharModelHeightGet((s16)p->charNo);
    v.y = (f32)((f64)v.y + h);
    eff = fn_1_1B48(M532_EFFECT_SIZE);
    if (eff != NULL) {
        fn_1_8130(eff, &v);
    }
}

// ---------------------------------------------------------------- motions

void fn_1_1215C(M532Player *p)
{
    p->motions.size = 0;
    fn_1_12254(p, 0x8C0000);
    fn_1_12254(p, 0x8C0001);
    fn_1_12254(p, 0x8700D0);
    fn_1_12254(p, 0x8700D1);
    fn_1_12254(p, 0x8700D2);
    fn_1_12254(p, 0x8C008C);
    fn_1_12254(p, 0x8C008D);
    fn_1_12254(p, 0x8C0030);
    fn_1_12254(p, 0x8C0031);
    if (p->motions.size != 9) {
        fn_1_1D1C(lbl_1_data_814, lbl_1_data_83A, 0x37E);
    }
    CharMotionDataClose((s16)p->charNo);
}

void fn_1_12254(M532Player *p, s32 id)
{
    s16 mot = CharMotionCreate((s16)p->charNo, id);

    fn_1_C6E4(&p->motions, p->motions.items + p->motions.size, 1, &mot);
}

void fn_1_122CC(M532Player *p, s32 motion, u8 loop)
{
    u8 snap;
    s32 shift;
    u32 attr;

    if (p->curMot != motion || CharMotionEndCheck((s16)p->charNo) != 0) {
        CharModelAttrSet((s16)p->charNo, 0x40000002);
        snap = 0;
        if (p->charNo == 6 && motion == 3) {
            snap = 1;
        }
        if (snap != 0) {
            shift = 0;
        } else {
            shift = 0x10;
        }
        if (loop != 0) {
            attr = 0x40000001;
        } else {
            attr = 0;
        }
        CharMotionShiftSet((s16)p->charNo, *fn_1_C66C(&p->motions, motion), 0.0f, (f32)shift, attr);
        p->curMot = motion;
    }
}

f32 fn_1_123F8(M532Player *p)
{
    if (CharMotionShiftIDGet((s16)p->charNo) >= 0) {
        return CharMotionShiftTimeGet((s16)p->charNo) - 1.0f;
    }
    return CharMotionTimeGet((s16)p->charNo);
}

// ---------------------------------------------------------------- rumble

void fn_1_12464(M532Player *p, s32 kind)
{
    Vec v;
    f64 h;
    void *eff;

    switch (kind) {
    case 0:
        omVibrate((s16)p->plyIdx, 0xC, 4, 2);
        return;
    case 1:
        if (fn_1_1260C() != 0) {
            omVibrate((s16)p->plyIdx, (s16)lbl_1_bss_A84, 0xC, 0);
            return;
        }
        v.x = p->pos.x;
        v.y = p->pos.y;
        v.z = p->pos.z;
        h = 50.0 + CharModelHeightGet((s16)p->charNo);
        v.y = (f32)((f64)v.y + h);
        eff = fn_1_1B48(M532_EFFECT_SIZE);
        if (eff != NULL) {
            fn_1_8130(eff, &v);
        }
        return;
    case 2:
        omVibrate((s16)p->plyIdx, 0xC, 0xC, 0);
        return;
    }
}

u8 fn_1_1259C(s32 padNo)
{
    u8 r;

    if (padNo < 0) {
        r = 1;
    } else {
        r = (lbl_1_rodata_564[padNo] & HuPadRumbleGet()) != 0;
    }
    return r;
}

// 1 when every human pad has rumble on (or nobody is human).
u8 fn_1_1260C(void)
{
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    u8 r1 = 0;
    u8 r2 = 0;
    u8 r3 = 0;

    if (GwCommon.rumbleF == 0) {
        return 0;
    }
    a = M532_FIRST(lbl_1_bss_AA0)->padNo;
    b = M532_NEXT(M532_FIRST(lbl_1_bss_AA0))->padNo;
    c = M532_FIRST(lbl_1_bss_AB4)->padNo;
    d = M532_NEXT(M532_FIRST(lbl_1_bss_AB4))->padNo;
    if (a < 0 && b < 0 && c < 0 && d < 0) {
        return 1;
    }
    if (fn_1_1259C(a) != 0) {
        if (fn_1_1259C(b) != 0) {
            r1 = 1;
        }
    }
    if (r1 != 0) {
        if (fn_1_1259C(c) != 0) {
            r2 = 1;
        }
    }
    if (r2 != 0) {
        if (fn_1_1259C(d) != 0) {
            r3 = 1;
        }
    }
    return r3;
}

// ---------------------------------------------------------------- wrapping angle

void fn_1_12884(M532Range *r, f32 angle)
{
    r->cur = fn_1_12970(r, angle);
}

f32 fn_1_128C0(M532Range *r, f32 target)
{
    return fn_1_13390(r, r->cur, target);
}

void fn_1_128FC(M532Range *r, f32 target, f64 rate)
{
    r->cur = fn_1_12970(r, (f32)((f64)r->cur + (rate * fn_1_13390(r, r->cur, target))));
}

f32 fn_1_12970(M532Range *r, f32 angle)
{
    if (angle < r->min) {
        return r->max - (f32)fmod(r->min - angle, r->span);
    }
    if (angle >= r->max) {
        return r->min + (f32)fmod(angle - r->max, r->span);
    }
    return angle;
}

f32 fn_1_12A0C(M532Range *r)
{
    return r->cur;
}

M532Range *fn_1_12AD4(M532Range *r, f32 a, f32 b, f32 c)
{
    if (a < b) {
        r->min = a;
    } else {
        r->min = b;
    }
    if (a > b) {
        r->max = a;
    } else {
        r->max = b;
    }
    r->span = r->max - r->min;
    r->cur = fn_1_12970(r, c);
    return r;
}

// Signed shortest difference target - cur on the circle.
f32 fn_1_13390(M532Range *r, f32 cur, f32 target)
{
    f32 t = fn_1_12970(r, target);
    f32 c = fn_1_12970(r, cur);
    f32 d = t - c;
    f32 ad;
    f32 res;
    s32 sgn;

    if (d < 0.0f) {
        ad = -d;
    } else {
        ad = d;
    }
    if (ad < (r->span * 0.5f)) {
        res = d;
    } else {
        if (d < 0.0f) {
            sgn = 1;
        } else {
            sgn = -1;
        }
        res = (r->span - ad) * (f32)sgn;
    }
    return res;
}

// ---------------------------------------------------------------- lists / vector

void fn_1_12A14(M532Node *list, void (*fn)(M532Player *))
{
    fn_1_12A74(list->next, list, fn);
}

void fn_1_12A74(M532Node *first, M532Node *head, void (*fn)(M532Player *))
{
    M532Node *n = first;

    while (n != head) {
        fn((M532Player *)n);
        n = n->next;
    }
}

// vector<Vec>::erase(pos)
Vec *fn_1_12C38(M532VecV *v, Vec *pos)
{
    Vec *end = v->items + v->size;
    Vec *dst;
    Vec *src;

    if (pos + 1 != end) {
        dst = pos;
        for (src = pos + 1; src < end; src++) {
            *dst = *src;
            dst++;
        }
    }
    v->size = v->size - 1;
    return pos;
}

// vector<Vec>::insert(pos, n, value)
void fn_1_12CF8(M532VecV *v, Vec *pos, u32 n, const Vec *val)
{
    const u32 maxN = 0x15555555;
    Vec value;
    Vec *oldItems;
    Vec *end;
    Vec *dst;
    Vec *src;
    Vec *newItems;
    u32 newCap;
    u32 want;
    u32 i;

    if (n == 0) {
        return;
    }
    if (n > maxN || v->size > maxN - n) {
        M532Fatal(lbl_1_data_863);
    }
    value = *val; // the value may live inside the vector
    oldItems = v->items;
    end = oldItems + v->size;
    if (v->size + n <= v->cap) {
        // shift the tail up by n, then fill
        for (src = end; src > pos;) {
            src--;
            src[n] = *src;
        }
        for (i = 0; i < n; i++) {
            pos[i] = value;
        }
        v->size = v->size + n;
        return;
    }
    want = v->size + n;
    newCap = (v->cap != 0) ? v->cap : 1;
    while (want > newCap) {
        if (newCap < (maxN >> 1)) {
            newCap *= 2;
        } else {
            newCap = maxN;
        }
    }
    newItems = (Vec *)fn_1_1B48(newCap * sizeof(Vec));
    if (newItems == NULL) {
        M532Fatal(lbl_1_data_880);
    }
    dst = newItems;
    for (src = oldItems; src < pos; src++) {
        *dst++ = *src;
    }
    for (i = 0; i < n; i++) {
        *dst++ = value;
    }
    for (src = pos; src < end; src++) {
        *dst++ = *src;
    }
    v->cap = newCap;
    v->items = newItems;
    v->size = want;
    if (oldItems != NULL) {
        fn_1_1C10(oldItems);
    }
}

void *fn_1_132A8(M532VecV *v, s16 flag)
{
    if (v != NULL) {
        fn_1_13334(v);
        if (v->items != NULL) {
            fn_1_1C10(v->items);
        }
        if (flag > 0) {
            fn_1_1C10(v);
        }
    }
    return v;
}

void fn_1_13334(M532VecV *v)
{
    v->size = 0;
}

// ---------------------------------------------------------------- static init

void fn_1_135A8(void)
{
    lbl_1_bss_A80 = 0x12C;
    lbl_1_bss_A84 = 0x3C;
    lbl_1_bss_A88 = 0x78;
    lbl_1_bss_A8C = 0xD2;
    lbl_1_bss_A90 = 0xD2;
    fn_1_32B0(&lbl_1_bss_AA0);
#ifdef __MWERKS__
    __register_global_object(&lbl_1_bss_AA0, (void *)fn_1_1368C, lbl_1_bss_A94);
#endif
    fn_1_32B0(&lbl_1_bss_AB4);
#ifdef __MWERKS__
    __register_global_object(&lbl_1_bss_AB4, (void *)fn_1_1368C, lbl_1_bss_AA8);
#endif
}

void *fn_1_1368C(M532Node *self, s16 flag)
{
    if (self != NULL) {
        fn_1_32BC(self, 0);
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

} // extern "C"

#ifdef __MWERKS__
__declspec(section ".ctors") void (*const lbl_ctor_135A8)(void) = fn_1_135A8;
#else
struct M532PlayerStaticInit {
    M532PlayerStaticInit() { fn_1_135A8(); }
};
static M532PlayerStaticInit lbl_player_static_init;
#endif
