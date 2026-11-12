#include "REL/m565dll.h"

static s32 fn_1_A9D4(M565Com *com);
static s32 fn_1_AB14(M565Com *com);
static s32 fn_1_AC18(M565Com *com);
static s32 fn_1_AEF4(M565Com *com);
static s32 fn_1_B600(M565Com *com);
static s32 fn_1_B96C(M565Com *com);
static void fn_1_BD0C(M565Com *com);
static void fn_1_BE54(M565Com *com, s16 *count, M565Tile **out);
static void fn_1_C1E8(M565Com *com, s16 state);
static void fn_1_C368(M565Com *com, s16 col, s16 row, s16 state);
static void fn_1_C62C(M565Com *com, M565Tile **out);
static void fn_1_CAD8(s16 dir, s16 *dx, s16 *dy);
static void fn_1_CBB4(s16 *dir, f32 angle);
static s32 fn_1_CE68(M565Com *com);
static s32 fn_1_E534(M565Com *com, M565Tile *tile, f32 *mv, s32 reason);
static s32 fn_1_E714(M565Com *com, M565Tile *tile, s32 reason);

s32 lbl_1_data_B28[4] = { 2, 10, 15, 20 };
u32 lbl_1_data_B38[4] = { 0, 0, 500, 1000 };
u32 lbl_1_data_B48[4] = { 0, 0, 500, 1000 };
u32 lbl_1_data_B58[4] = { 6000, 5000, 3000, 2000 };
s32 lbl_1_data_B68[4] = { 1500, 2500, 3500, 4000 };
s32 lbl_1_data_B78[4] = { 1500, 2500, 3500, 4000 };

s16 lbl_1_bss_7830;
s16 lbl_1_bss_7832[9];
M565Tile *lbl_1_bss_7844[9];
M565Tile *lbl_1_bss_7868[10];

#define M565_STANDING(t) ((t)->state == 0x64 || (t)->state == 0x65)
#define M565_INR(c, r) ((c) >= 0 && (c) < 10 && (r) >= 0 && (r) < 10)
#define M565_NEWTIMER() ((s32)(60.0f + 30.0f * ((f32)(frand() % 10000) / 10000.0f)))
#define M565_ABS(v) ((v) >= 0 ? (v) : -(v))

/* angle of 180/-180 wrap, re-evaluating the expression in every branch like the original */
#define M565_WRAP_D(e) (((f32)(e) > 180.0f) ? ((e) - 360.0) : (((f32)(e) <= -180.0f) ? (360.0 + (e)) : (e)))

/* some GC paths read the stale tile pointer before it is ever assigned: start it on the player's tile */
#ifdef TARGET_PC
#define M565_STALE(t, st, pl) (t) = &(st)->tiles[(pl)->row][(pl)->col]
#else
#define M565_STALE(t, st, pl) ((void)0)
#endif

/* look for the neighbouring tile with the most standing neighbours: three probes around the heading */
#define M565_PROBE(dx, dy, mul) \
    do { \
        x = player->col + (dx) * (mul); \
        y = player->row + (dy) * (mul); \
        if (M565_INR(x, y)) { \
            t = &stage->tiles[y][x]; \
            if (best < t->neighbors) { \
                best = t->neighbors; \
                bestT = t; \
            } \
        } \
    } while (0)

#define M565_RING(dir) \
    do { \
        for (k = 0; k < 3; k++) { \
            fn_1_CAD8((s16)(((dir) + 2 - k) % 8), &dx, &dy); \
            M565_PROBE(dx, dy, 3); \
            M565_PROBE(dx, dy, 1); \
            fn_1_CAD8((s16)(((dir) + k - 2) % 8), &dx, &dy); \
            M565_PROBE(dx, dy, 3); \
            x = player->col + dx; \
            y = player->row + dy; \
            if (M565_INR(x, y) && best < t->neighbors) { \
                best = t->neighbors; \
                bestT = t; \
            } \
        } \
    } while (0)

void fn_1_9EAC(M565Com *com, M565Player *player)
{
    u16 level;
    s32 dif;

    memset(com, 0, sizeof(M565Com));
    com->player = player;
    com->active = GwPlayerConf[player->no].type;
    dif = GwPlayerConf[player->no].dif;
    if (dif == 3) {
        level = 0;
    } else if (dif == 2) {
        level = 1;
    } else if (dif == 1) {
        level = 2;
    } else {
        level = 3;
    }
    com->level = level;
    com->shootTimer = lbl_1_data_B28[com->level] + frand() % (lbl_1_data_B28[com->level] / 2);
}

void fn_1_9FF4(M565Com *com)
{
    memset(com, 0, sizeof(M565Com));
}

void fn_1_A024(M565Com *com)
{
    com->timer = 0;
    com->prio = 4;
    com->mode = 0;
    com->step = 0;
    com->phase = 0;
}

void fn_1_A050(M565Com *com)
{
    M565Player *player = com->player;
    M565Stage *stage;
    M565Tile *cur;
    s32 r;
    s32 chain;

    com->btnDown = 0;
    com->stkX = 0;
    com->stkY = 0;
    com->btn = 0;
    if (player->fallen != 0) {
        return;
    }
    stage = &player->sys->stage;
    cur = &stage->tiles[player->row][player->col];
    if (com->timer <= 0) {
        com->timer = 0;
        com->prio = 4;
        com->mode = 0;
        com->step = 0;
        com->phase = 0;
    }
    chain = 1;
    if (com->prio > 0 && (cur->state == 0x65 || cur->state == 0x66)) {
        if ((frand() % 10000) < lbl_1_data_B38[com->level]) {
            com->mode = 0;
            com->step = 0;
            com->timer = M565_NEWTIMER();
            com->prio = 3;
        } else {
            com->target = player->near;
            com->mode = 1;
            com->step = 0;
            com->timer = M565_NEWTIMER();
            com->prio = 0;
            chain = 0;
        }
    }
    if (chain != 0) {
        if (com->prio > 1) {
            if (cur->hitFlag != 0) {
                if ((frand() % 10000) < lbl_1_data_B48[com->level]) {
                    com->mode = 0;
                    com->step = 0;
                    com->timer = M565_NEWTIMER();
                    com->prio = 3;
                } else {
                    com->mode = 3;
                    com->step = 0;
                    com->timer = M565_NEWTIMER();
                    com->prio = 1;
                }
            } else if (player->mg->stunType == 5) {
                com->mode = 2;
                com->step = 0;
                com->timer = M565_NEWTIMER();
                com->prio = 1;
            } else {
                goto low;
            }
        } else {
low:
            if (com->prio > 2 && player->near != NULL && player->nearDist < 350.0f) {
                if ((frand() % 10000) < lbl_1_data_B58[com->level]) {
                    com->target = player->near;
                    com->mode = 5;
                    com->step = 0;
                    com->timer = M565_NEWTIMER();
                    com->prio = 2;
                } else {
                    com->target = player->near;
                    com->mode = 4;
                    com->step = 0;
                    com->timer = M565_NEWTIMER();
                    com->prio = 2;
                }
            } else if (com->prio > 3) {
                r = frand() % 10000;
                if (r < lbl_1_data_B68[com->level]) {
                    com->mode = 0;
                    com->step = 0;
                    com->timer = M565_NEWTIMER();
                    com->prio = 3;
                } else if (r < lbl_1_data_B68[com->level] + lbl_1_data_B78[com->level]) {
                    com->mode = 6;
                    com->step = 0;
                    com->timer = M565_NEWTIMER();
                    com->prio = 3;
                } else {
                    com->target = player->near;
                    com->mode = 5;
                    com->step = 0;
                    com->timer = M565_NEWTIMER();
                    com->prio = 3;
                }
            }
        }
    }
    switch (com->mode) {
        case 0:
            if (fn_1_A9D4(com) != 0) {
                com->timer = 0;
            }
            break;
        case 1:
            if (fn_1_AB14(com) != 0) {
                com->timer = 0;
            }
            break;
        case 2:
            if (fn_1_AEF4(com) != 0) {
                com->timer = 0;
            }
            break;
        case 3:
            if (fn_1_AC18(com) != 0) {
                com->timer = 0;
            }
            break;
        case 4:
            if (fn_1_AEF4(com) != 0) {
                com->timer = 0;
            }
            break;
        case 5:
            if (fn_1_B600(com) != 0) {
                com->timer = 0;
            }
            break;
        case 6:
            if (fn_1_B96C(com) != 0) {
                com->timer = 0;
            }
            break;
    }
    if (com->timer != 0) {
        com->timer = com->timer - 1;
    }
}

static s32 fn_1_A9D4(M565Com *com)
{
    M565Player *player = com->player;
    M565Stage *stage = &player->sys->stage;
    M565Tile *cur = &stage->tiles[player->row][player->col];
    M565Tile *t;

    if (!M565_STANDING(cur)) {
        fn_1_C62C(com, &t);
        if (t != NULL) {
            com->destCol = t->col;
            com->destRow = t->row;
            fn_1_CE68(com);
            com->mode = 6;
            com->step = 0;
            com->timer = M565_NEWTIMER();
            com->prio = 3;
        }
    }
    return 0;
}

static s32 fn_1_AB14(M565Com *com)
{
    M565Player *player = com->player;
    M565Stage *stage = &player->sys->stage;
    M565Tile *cur;
    s16 count;
    s32 done = 0;

    switch (com->step) {
        case 0:
            fn_1_BE54(com, &count, lbl_1_bss_7868);
            if (count != 0) {
                com->destCol = lbl_1_bss_7868[0]->col;
                com->destRow = lbl_1_bss_7868[0]->row;
            } else {
                fn_1_BD0C(com);
            }
            com->step = com->step + 1;
            /* fallthrough */
        case 1:
            if (fn_1_CE68(com) != 0) {
                cur = &stage->tiles[player->row][player->col];
                if (M565_STANDING(cur)) {
                    done = 1;
                }
                com->step = 0;
            }
            break;
    }
    return done;
}

static s32 fn_1_AC18(M565Com *com)
{
    HuVecF d;
    M565Player *player = com->player;
    M565Stage *stage = &player->sys->stage;
    M565Tile *cur;
    s16 dir;
    s16 dx;
    s16 dy;
    f64 ang;
    s32 found;
    s32 done = 0;

    switch (com->step) {
        case 0:
            found = 0;
            cur = &stage->tiles[player->row][player->col];
            if (cur->hitFlag != 0 && cur->hitItem != NULL && cur->hitItem->active != 0) {
                PSVECSubtract(player->pos, &cur->hitItem->pos, &d);
                d.y = 0.0f;
                PSVECNormalize(&d, &d);
                ang = M565_WRAP_D(180.0 * (atan2(d.x, d.z) / 3.141592653589793));
                fn_1_CBB4(&dir, ang);
                fn_1_CAD8(dir, &dx, &dy);
                com->destCol = player->col + dx * 3;
                com->destRow = player->row + dy * 3;
                found = 1;
            }
            if (found == 0) {
                fn_1_BD0C(com);
            }
            com->step = com->step + 1;
            /* fallthrough */
        case 1:
            if (fn_1_CE68(com) != 0) {
                if (stage->tiles[player->row][player->col].hitFlag == 0) {
                    done = 1;
                }
                com->step = 0;
            }
            break;
    }
    return done;
}

static s32 fn_1_AEF4(M565Com *com)
{
    HuVecF d;
    HuVecF fwd;
    HuVecF other;
    M565Player *player = com->player;
    M565Stage *stage = &player->sys->stage;
    M565Tile *t;
    M565Tile *bestT;
    s16 dir;
    s16 dx;
    s16 dy;
    s16 x;
    s16 y;
    s16 best;
    s32 k;
    f64 ang;
    s32 done;

    M565_STALE(t, stage, player);
    if (player->near == NULL) {
        return 1;
    }
    if (player->near->fallen != 0) {
        return 1;
    }
    done = 0;
    switch (com->step) {
        case 0:
            fn_1_BD0C(com);
            if ((frand() % 10000) < 7000) {
                PSVECSubtract(player->pos, player->near->pos, &d);
                d.y = 0.0f;
                PSVECNormalize(&d, &d);
                ang = M565_WRAP_D(180.0 * (atan2(d.x, d.z) / 3.141592653589793));
                fn_1_CBB4(&dir, ang);
                best = 0;
                bestT = NULL;
                M565_RING(dir);
                if (bestT != NULL) {
                    com->destCol = bestT->col;
                    com->destRow = bestT->row;
                } else {
                    fn_1_BD0C(com);
                }
            }
            com->step = com->step + 1;
            /* fallthrough */
        case 1:
            if (fn_1_CE68(com) != 0) {
                com->step = 0;
            }
            if (player->near != NULL && player->nearDist < 120.0f) {
                fwd.x = sin(3.141592653589793 * player->rot->y / 180.0);
                fwd.y = 0.0f;
                fwd.z = cos(3.141592653589793 * player->rot->y / 180.0);
                other.x = sin(3.141592653589793 * player->near->rot->y / 180.0);
                other.y = 0.0f;
                other.z = cos(3.141592653589793 * player->near->rot->y / 180.0);
                if (PSVECDotProduct(&fwd, &other) < 0.0f) {
                    com->target = player->near;
                    com->mode = 5;
                    com->step = 0;
                    com->timer = M565_NEWTIMER();
                    com->prio = 2;
                }
            }
            break;
    }
    if (com->timer <= 0) {
        done = 1;
    }
    return done;
}

static s32 fn_1_B600(M565Com *com)
{
    M565Player *player = com->player;
    f32 radius;
    s32 done;

    if (com->target == NULL) {
        return 1;
    }
    if (com->target->fallen != 0) {
        return 1;
    }
    done = 0;
    switch (com->step) {
        case 0:
            if ((frand() % 10000) < 7000) {
                com->destCol = com->target->col;
                com->destRow = com->target->row;
            } else {
                fn_1_BD0C(com);
            }
            com->shootTimer = (u32)(lbl_1_data_B28[com->level] + frand() % (lbl_1_data_B28[com->level] / 2)) >> 1;
            com->step = com->step + 1;
            /* fallthrough */
        case 1:
            if (fn_1_CE68(com) != 0) {
                com->step = 0;
                if (com->timer <= 0) {
                    done = 1;
                }
                switch (com->level) {
                    case 3:
                        if (player->nearDist > 150.0f) {
                            done = 1;
                        } else {
                            switch (com->level) {
                                case 2:
                                    if (player->nearDist > 200.0f) {
                                        done = 1;
                                    } else if (com->level == 1 && player->nearDist > 300.0f) {
                                        done = 1;
                                    } else if (com->level == 0 && player->nearDist > 400.0f) {
                                        done = 1;
                                    }
                                    break;
                            }
                        }
                        break;
                }
            }
            if (player->near != NULL) {
                switch (com->level) {
                    case 0:
                        radius = 190.0f;
                        break;
                    case 1:
                        radius = 170.0f;
                        break;
                    case 2:
                        radius = 150.0f;
                        break;
                    default:
                        radius = 130.0f;
                        break;
                }
                if (player->nearDist < radius) {
                    com->aim = 1;
                    com->shootTimer = com->shootTimer - 1;
                    if (com->shootTimer + 1 <= 0) {
                        com->btnDown |= 0x200;
                        com->shootTimer = lbl_1_data_B28[com->level] + frand() % (lbl_1_data_B28[com->level] / 2);
                    }
                }
            }
            break;
    }
    return done;
}

static s32 fn_1_B96C(M565Com *com)
{
    M565Player *player = com->player;
    M565Stage *stage = &player->sys->stage;
    M565Tile *t;
    M565Tile *bestT;
    s16 dir;
    s16 dx;
    s16 dy;
    s16 x;
    s16 y;
    s16 best;
    s32 k;
    s32 done;

    M565_STALE(t, stage, player);
    done = 0;
    switch (com->step) {
        case 0:
            fn_1_BD0C(com);
            com->step = com->step + 1;
            /* fallthrough */
        case 1:
            if (fn_1_CE68(com) != 0) {
                if (com->timer <= 0) {
                    done = 1;
                } else {
                    fn_1_CBB4(&dir, player->rot->y);
                    best = 0;
                    bestT = NULL;
                    M565_RING(dir);
                    if (bestT != NULL) {
                        com->destCol = bestT->col;
                        com->destRow = bestT->row;
                    } else {
                        fn_1_BD0C(com);
                    }
                    goto near;
                }
            } else {
near:
                if (player->near != NULL && player->near->fallen == 0 && player->nearDist < 150.0f) {
                    com->target = player->near;
                    com->mode = 4;
                    com->step = 0;
                    com->prio = 2;
                }
            }
            break;
    }
    return done;
}

static void fn_1_BD0C(M565Com *com)
{
    M565Player *player = com->player;
    M565Stage *stage = &player->sys->stage;
    M565Tile *t;
    M565Tile *bestT;
    s16 best;
    s16 x;
    s16 y;
    s32 i;

    bestT = NULL;
    best = 0;
    for (i = 0; i < 8; i++) {
        x = frand() % 7 + player->col - 3;
        y = frand() % 7 + player->row - 3;
        if (M565_INR(x, y)) {
            t = &stage->tiles[y][x];
            if (best < t->neighbors) {
                best = t->neighbors;
                bestT = t;
            }
        }
    }
    if (bestT != NULL) {
        com->destCol = bestT->col;
        com->destRow = bestT->row;
    }
}

static void fn_1_BE54(M565Com *com, s16 *count, M565Tile **out)
{
    M565Tile *a;
    M565Tile *b;
    s16 i;
    s16 j;

    lbl_1_bss_7830 = 0;
    fn_1_C1E8(com, 0x64);
    fn_1_C1E8(com, 0x65);
    for (i = 0; i < lbl_1_bss_7830; i++) {
        for (j = i + 1; j < lbl_1_bss_7830; j++) {
            a = lbl_1_bss_7844[i];
            b = lbl_1_bss_7844[j];
            if (a->state != 0x64 && b->state == 0x64) {
                lbl_1_bss_7844[i] = lbl_1_bss_7844[j];
                lbl_1_bss_7844[j] = lbl_1_bss_7844[i];
                lbl_1_bss_7832[i] = lbl_1_bss_7832[j];
                lbl_1_bss_7832[j] = lbl_1_bss_7832[i];
            } else if (a->state == b->state) {
                if (a->neighbors < b->neighbors) {
                    lbl_1_bss_7844[i] = lbl_1_bss_7844[j];
                    lbl_1_bss_7844[j] = lbl_1_bss_7844[i];
                    lbl_1_bss_7832[i] = lbl_1_bss_7832[j];
                    lbl_1_bss_7832[j] = lbl_1_bss_7832[i];
                } else if (a->neighbors == b->neighbors && lbl_1_bss_7832[i] > lbl_1_bss_7832[j]) {
                    lbl_1_bss_7844[i] = lbl_1_bss_7844[j];
                    lbl_1_bss_7844[j] = lbl_1_bss_7844[i];
                    lbl_1_bss_7832[i] = lbl_1_bss_7832[j];
                    lbl_1_bss_7832[j] = lbl_1_bss_7832[i];
                }
            }
        }
    }
    for (i = 0; i < lbl_1_bss_7830; i++) {
        out[i] = lbl_1_bss_7844[i];
    }
    *count = lbl_1_bss_7830;
}

static void fn_1_C1E8(M565Com *com, s16 state)
{
    M565Player *player = com->player;
    s16 x;
    s16 y;
    s32 r;
    s32 c;

#ifdef TARGET_PC
    x = player->col;
    y = player->row;
#endif
    for (r = 0; r < 5 && lbl_1_bss_7830 < 9; r++) {
        for (c = 0; c < 5 && lbl_1_bss_7830 < 9; c++) {
            if (r != 0) {
                x = player->col + c;
                y = player->row - r;
            }
            fn_1_C368(com, x, y, state);
            if (c != 0) {
                x = player->col - c;
                y = player->row - r;
                fn_1_C368(com, x, y, state);
            }
        }
        for (c = 0; c < 5 && lbl_1_bss_7830 < 9; c++) {
            if (r != 0) {
                x = player->col + c;
                y = player->row + r;
                fn_1_C368(com, x, y, state);
            }
            if (c != 0) {
                x = player->col - c;
                y = player->row + r;
                fn_1_C368(com, x, y, state);
            }
        }
    }
}

static void fn_1_C368(M565Com *com, s16 col, s16 row, s16 state)
{
    M565Player *player = com->player;
    M565Tile *t;

    if (M565_INR(col, row)) {
        t = &player->sys->stage.tiles[row][col];
        if (lbl_1_bss_7830 < 9 && t->state == state) {
            lbl_1_bss_7844[lbl_1_bss_7830] = t;
            lbl_1_bss_7832[lbl_1_bss_7830] = sqrt((f64)((player->col - col) * (player->col - col) + (player->row - row) * (player->row - row)));
            lbl_1_bss_7830 += 1;
        }
    }
}

static void fn_1_C62C(M565Com *com, M565Tile **out)
{
    M565Player *player = com->player;
    M565Stage *stage = &player->sys->stage;
    M565Tile *t;
    s16 dir;
    s16 dx;
    s16 dy;
    s16 x;
    s16 y;
    s32 i;

    fn_1_CBB4(&dir, player->rot->y);
    for (i = 0; i < 5; i++) {
        fn_1_CAD8((s16)((dir + i) % 8), &dx, &dy);
        x = player->col + dx;
        y = player->row + dy;
        if (M565_INR(x, y) && (t = &stage->tiles[y][x], M565_STANDING(t))) {
            *out = t;
            return;
        }
        fn_1_CAD8((s16)((dir - i) % 8), &dx, &dy);
        x = player->col + dx;
        y = player->row + dy;
        if (M565_INR(x, y) && (t = &stage->tiles[y][x], M565_STANDING(t))) {
            *out = t;
            return;
        }
    }
    for (i = 0; i < 5; i++) {
        fn_1_CAD8((s16)((dir + i) % 8), &dx, &dy);
        x = player->col + dx * 2;
        y = player->row + dy * 2;
        if (M565_INR(x, y) && (t = &stage->tiles[y][x], M565_STANDING(t))) {
            *out = t;
            return;
        }
        fn_1_CAD8((s16)((dir - i) % 8), &dx, &dy);
        x = player->col + dx * 2;
        y = player->row + dy * 2;
        if (M565_INR(x, y) && (t = &stage->tiles[y][x], M565_STANDING(t))) {
            *out = t;
            return;
        }
    }
    *out = NULL;
}

void fn_1_C9C8(s16 dx, s16 dy, s16 *dir)
{
    *dir = 8;
    switch (dy) {
        case 1:
            switch (dx) {
                case -1:
                    *dir = 5;
                    return;
                case 0:
                    *dir = 4;
                    return;
                case 1:
                    *dir = 3;
                    return;
            }
            break;
        case -1:
            switch (dx) {
                case -1:
                    *dir = 7;
                    return;
                case 0:
                    *dir = 0;
                    return;
                case 1:
                    *dir = 1;
                    return;
            }
            break;
        case 0:
            switch (dx) {
                case -1:
                    *dir = 6;
                    return;
                case 0:
                    *dir = 8;
                    return;
                case 1:
                    *dir = 2;
                    break;
            }
            break;
    }
}

static void fn_1_CAD8(s16 dir, s16 *dx, s16 *dy)
{
    switch (dir) {
        case 0:
            *dx = 0;
            *dy = -1;
            return;
        case -7:
        case 1:
            *dx = 1;
            *dy = -1;
            return;
        case -6:
        case 2:
            *dx = 1;
            *dy = 0;
            return;
        case -5:
        case 3:
            *dx = 1;
            *dy = 1;
            return;
        case -4:
        case 4:
            *dx = 0;
            *dy = 1;
            return;
        case -3:
        case 5:
            *dx = -1;
            *dy = 1;
            return;
        case -2:
        case 6:
            *dx = -1;
            *dy = 0;
            return;
        case -1:
        case 7:
            *dx = -1;
            *dy = -1;
            return;
        default:
            *dx = 0;
            *dy = 0;
            return;
    }
}

static void fn_1_CBB4(s16 *dir, f32 angle)
{
    f32 d = 45.0f;

    *dir = 8;
    if (angle < 0.5f * d && angle > 0.5f * -d) {
        *dir = 4;
        return;
    }
    if (angle <= 0.5f * -d && angle > 1.5f * -d) {
        *dir = 5;
        return;
    }
    if (angle <= 1.5f * -d && angle > 2.5f * -d) {
        *dir = 6;
        return;
    }
    if (angle <= 2.5f * -d && angle > 3.5f * -d) {
        *dir = 7;
        return;
    }
    if (angle < 1.5f * d && angle >= 0.5f * d) {
        *dir = 3;
        return;
    }
    if (angle < 2.5f * d && angle >= 1.5f * d) {
        *dir = 2;
        return;
    }
    if (angle < 3.5f * d && angle >= 2.5f * d) {
        *dir = 1;
        return;
    }
    *dir = 0;
}

M565Player *fn_1_CDD8(M565Com *com, M565Tile *tile)
{
    M565Player *player = com->player;
    M565Sys *sys = player->sys;
    M565Player *other;
    s32 i;

    for (i = 0; i < 4; i++) {
        other = &sys->players[i];
        if (other != player && tile->col == other->col && tile->row == other->row) {
            return other;
        }
    }
    return NULL;
}

static s32 fn_1_CE68(M565Com *com)
{
    HuVecF toTarget;
    HuVecF toDir;
    HuVecF near;
    f32 mv[3];
    M565Player *player = com->player;
    M565Stage *stage = &player->sys->stage;
    M565Tile *cur;
    M565Tile *tgt;
    M565Tile *next;
    M565Tile *a;
    M565Tile *b;
    M565Tile *t;
    s16 dx;
    s16 dy;
    s16 sx;
    s16 sy;
    s32 done;
    f32 radius;
    f32 clampV;
    f32 scale;
    f32 vlevel;
    f32 d0;
    f32 d1;
    s32 absDx;
    s32 absDy;

    if (com->destCol < 0 || com->destCol >= 10 || com->destRow < 0 || com->destRow >= 10) {
        if (com->destCol > 10) {
            com->destCol = 10;
        } else if (com->destCol < 0) {
            com->destCol = 0;
        }
        if (com->destCol > 10) {
            com->destRow = 10;
        } else if (com->destCol < 0) {
            com->destRow = 0;
        } else {
            com->destRow = com->destCol;
        }
    }
    cur = &stage->tiles[player->row][player->col];
    dx = com->destCol - player->col;
    dy = com->destRow - player->row;
    done = 0;
    sy = 0;
    sx = 0;
    mv[0] = 0.0f;
    mv[1] = 0.0f;
    mv[2] = 0.0f;
    switch (com->phase) {
        case 0:
            if (dx == 0 && dy == 0) {
                done = 1;
            } else {
                absDx = M565_ABS(dx);
                if (absDx != 0) {
                    absDy = M565_ABS(dy);
                    if (absDy != 0) {
                        sx = dx < 0 ? -1 : 1;
                        sy = dy < 0 ? -1 : 1;
                    } else {
                        goto axis;
                    }
                } else {
axis:
                    absDy = M565_ABS(dy);
                    absDx = M565_ABS(dx);
                    if (absDx > absDy) {
                        sx = dx < 0 ? -1 : 1;
                    } else {
                        sy = dy < 0 ? -1 : 1;
                    }
                }
                if (stage->tiles[com->destRow][com->destCol].hitFlag != 0 && cur->hitFlag == 0 && M565_STANDING(cur)) {
                    if (sqrtf((f32)(dx * dx + dy * dy)) == 1.0f) {
                        done = 1;
                    } else {
                        goto walk;
                    }
                } else {
walk:
                    if (!M565_STANDING(cur)) {
                        fn_1_C62C(com, &t);
                        if (t != NULL) {
                            if (player->pos->y < -3.0f) {
                                fn_1_E714(com, t, 0x463);
                            } else {
                                fn_1_E534(com, t, mv, 0x465);
                            }
                        }
                    }
                    if (M565_INR(player->col + sx, player->row + sy)) {
                        next = &stage->tiles[player->row + sy][player->col + sx];
                        if (M565_STANDING(next)) {
                            if (M565_ABS(sx) != 0 && M565_ABS(sy) != 0) {
                                a = &stage->tiles[player->row][player->col + sx];
                                b = &stage->tiles[player->row + sy][player->col];
                                if (!M565_STANDING(a) && !M565_STANDING(b)) {
                                    if (player->mg->mode == 0) {
                                        fn_1_E714(com, next, 0x479);
                                    } else {
                                        done = 1;
                                    }
                                } else if (!M565_STANDING(a) && M565_STANDING(b)) {
                                    fn_1_E534(com, b, mv, 0x481);
                                } else if (M565_STANDING(a) && !M565_STANDING(b)) {
                                    fn_1_E534(com, a, mv, 0x484);
                                } else {
                                    fn_1_E534(com, next, mv, 0x489);
                                }
                            } else {
                                fn_1_E534(com, next, mv, 0x48F);
                            }
                        } else {
                            if (M565_ABS(sx) == M565_ABS(sy) && sx != 0 && sy != 0) {
                                if (M565_INR(player->col + sx, player->row)
                                    && (t = &stage->tiles[player->row][player->col + sx], M565_STANDING(t))) {
                                    fn_1_E534(com, t, mv, 0x49C);
                                } else if (M565_INR(player->col, player->row + sy)
                                    && (t = &stage->tiles[player->row + sy][player->col], M565_STANDING(t))) {
                                    fn_1_E534(com, t, mv, 0x4A4);
                                } else {
                                    goto far;
                                }
                            } else {
far:
                                if (M565_INR(player->col + sx * 2, player->row + sy * 2) && next->state != 0
                                    && (t = &stage->tiles[player->row + sy * 2][player->col + sx * 2], M565_STANDING(t))
                                    && player->mg->mode == 0) {
                                    fn_1_E714(com, t, 0x4B0);
                                } else {
                                    if (M565_ABS(sx) == M565_ABS(sy) && sx != 0 && sy != 0) {
                                        if (M565_ABS(dx) >= 2) {
                                            if (M565_INR(player->col + sx * 2, player->row)
                                                && (t = &stage->tiles[player->row][player->col + sx * 2], M565_STANDING(t))
                                                && player->mg->mode == 0) {
                                                fn_1_E714(com, t, 0x4BC);
                                            } else if (M565_INR(player->col, player->row + sy * 2)) {
                                                if (M565_ABS(dy) >= 2
                                                    && (t = &stage->tiles[player->row + sy * 2][player->col],
                                                        M565_STANDING(t))
                                                    && player->mg->mode == 0) {
                                                    fn_1_E714(com, t, 0x4C4);
                                                } else {
                                                    goto fall;
                                                }
                                            } else {
                                                goto fall;
                                            }
                                        } else {
                                            goto fall;
                                        }
                                    } else {
fall:
                                        if (cur->state == 0x66 && next->state != 1 && next->state != 0) {
                                            if (player->pos->y < 30.0f && player->mg->mode == 0) {
                                                fn_1_E714(com, next, 0x4CD);
                                            } else {
                                                fn_1_E534(com, next, mv, 0x4CF);
                                            }
                                        } else {
                                            done = 1;
                                        }
                                    }
                                }
                            }
                        }
                    } else {
                        done = 1;
                    }
                }
            }
            break;
        case 1:
            cur = &stage->tiles[player->row][player->col];
            tgt = &stage->tiles[com->tgtRow][com->tgtCol];
            PSVECSubtract(&tgt->pos, player->pos, &toTarget);
            PSVECSubtract(&tgt->pos, &cur->pos, &toDir);
            toDir.y = 0.0f;
            toTarget.y = 0.0f;
            d0 = PSVECMag(&toTarget);
            d1 = PSVECMag(&toDir);
            if (d0 < d1 || !M565_STANDING(cur)) {
                com->btnDown |= 0x100;
                com->phase = 2;
            }
            fn_1_E534(com, &stage->tiles[com->tgtRow][com->tgtCol], mv, 0x4F1);
            break;
        case 2:
            tgt = &stage->tiles[com->tgtRow][com->tgtCol];
            if (tgt->state == 1 || tgt->state == 0 || tgt->state == 0x66) {
                if (M565_STANDING(cur)) {
                    com->tgtCol = player->col;
                    com->tgtRow = player->row;
                } else {
                    fn_1_C62C(com, &t);
                    if (t != NULL) {
                        com->tgtCol = t->col;
                        com->tgtRow = t->row;
                    }
                }
            }
            if (player->col == com->tgtCol && player->row == com->tgtRow && player->groundDist < 30.0f
                && player->mg->mode != 1) {
                com->phase = 0;
            } else {
                if (com->flag2C == 0) {
                    if (player->mg->mode == 1) {
                        com->flag2C = 1;
                        com->btnDown |= 0x100;
                    }
                }
                fn_1_E534(com, &stage->tiles[com->tgtRow][com->tgtCol], mv, 0x51E);
                if (com->tgtCol == player->col && com->tgtRow == player->row) {
                    clampV = player->groundDist / 30.0f;
                    if (clampV > 1.0f) {
                        scale = 1.0f;
                    } else if (clampV < 0.0f) {
                        scale = 0.0f;
                    } else {
                        scale = clampV;
                    }
                } else {
                    scale = 1.0f;
                }
                mv[0] *= scale;
                mv[1] *= scale;
                com->btn |= 0x100;
                if (player->near != NULL) {
                    switch (com->level) {
                        case 0:
                            radius = 150.0f;
                            break;
                        case 1:
                            radius = 140.0f;
                            break;
                        default:
                            radius = 120.0f;
                            break;
                    }
                    if (player->nearDist < radius) {
                        com->shootTimer = com->shootTimer - 1;
                        if (com->shootTimer + 1 <= 0) {
                            com->btnDown |= 0x200;
                            com->shootTimer = lbl_1_data_B28[com->level] + frand() % (lbl_1_data_B28[com->level] / 2);
                        }
                    }
                }
            }
            break;
    }
    if (dx == 0 && dy == 0 && player->groundDist < 50.0f) {
        done = 1;
    }
    if (com->aim != 0 && player->near != NULL && player->near->fallen == 0) {
        PSVECSubtract(player->near->pos, player->pos, &near);
        near.y = 0.0f;
        if (near.x != 0.0f || near.z != 0.0f) {
            PSVECNormalize(&near, &near);
        }
        mv[0] = near.x;
        mv[1] = near.z;
        clampV = player->nearDist / 30.0f;
        if (clampV > 1.0f) {
            scale = 1.0f;
        } else if (clampV < 0.0f) {
            scale = 0.0f;
        } else {
            scale = clampV;
        }
        mv[0] *= scale;
        mv[1] *= scale;
    }
    switch (com->level) {
        case 0:
            vlevel = 1.0f;
            break;
        case 1:
            vlevel = 1.0f;
            break;
        case 2:
            vlevel = 0.9f;
            break;
        default:
            vlevel = 0.8f;
            break;
    }
    if (com->phase == 0) {
        scale = vlevel;
    } else {
        scale = 1.0f;
    }
    mv[0] = mv[0] * scale;
    mv[1] = mv[1] * scale;
    com->smoothX = 0.8f * com->smoothX + 0.2f * mv[0];
    com->smoothZ = 0.8f * com->smoothZ + 0.2f * mv[1];
    com->stkX = (s8)(56.0f * com->smoothX);
    com->stkY = (s8)(56.0f * -com->smoothZ);
    com->aim = 0;
    return done;
}

static s32 fn_1_E534(M565Com *com, M565Tile *tile, f32 *mv, s32 reason)
{
    HuVecF d;
    M565Player *player = com->player;
    M565Stage *stage = &player->sys->stage;
    M565Tile *t = tile;
    M565Tile *a;
    M565Tile *b;

    com->reason1 = reason;
    a = &stage->tiles[7][4];
    b = &stage->tiles[7][5];
    if (player->row == 8 && player->col == 4 && tile->row == 8 && tile->col == 5) {
        if (M565_STANDING(a) && M565_STANDING(b)) {
            t = a;
        } else if (player->mg->mode == 0) {
            fn_1_E714(com, tile, 0x57F);
        }
    } else if (player->row == 8 && player->col == 5 && tile->row == 8 && tile->col == 4) {
        if (M565_STANDING(a) && M565_STANDING(b)) {
            t = b;
        } else if (player->mg->mode == 0) {
            fn_1_E714(com, tile, 0x588);
        }
    }
    PSVECSubtract(&t->pos, player->pos, &d);
    d.y = 0.0f;
    if (d.x != 0.0f || d.z != 0.0f) {
        PSVECNormalize(&d, &d);
    }
    mv[0] = d.x;
    mv[1] = d.z;
    return 1;
}

static s32 fn_1_E714(M565Com *com, M565Tile *tile, s32 reason)
{
    com->tgtCol = tile->col;
    com->tgtRow = tile->row;
    com->phase = 1;
    com->flag2C = 0;
    com->reason0 = reason;
    return 1;
}
