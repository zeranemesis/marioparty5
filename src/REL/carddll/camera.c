// carddll/camera.c - the Card Party camera (fn_1_30E08 .. fn_1_3224C).
// The camera glides from its current position/target to a destination with an ease curve (sin^2).

#include "REL/carddll.h"

#define CAM_EASE(t) ((f32)(sin((3.141592653589793 * (f64)(90.0f * (t))) / 180.0) * sin((3.141592653589793 * (f64)(90.0f * (t))) / 180.0)))

/* ---- .data ---- */
static Vec lbl_1_data_7B8 = { -50.0f, 3000.0f, 3000.0f }; /* camera position */
static Vec lbl_1_data_7C4 = { 0.0f, 1.0f, 0.0f };         /* up vector */
static Vec lbl_1_data_7D0 = { -50.0f, 0.0f, 0.0f };       /* camera target */

/* ---- .bss ---- */
s32 lbl_1_bss_1174;     /* used by player.c / status.c */
s32 lbl_1_bss_1168[3]; /* used by player.c / status.c */
static s32 lbl_1_bss_11B4; /* a move is in progress */
static s32 lbl_1_bss_11B0; /* phase of the two-step (axis by axis) move */
static f32 lbl_1_bss_11AC; /* elapsed frames of the current move */
static f32 lbl_1_bss_11A8; /* length in frames of the current move (mode 8) */
static Vec lbl_1_bss_119C; /* start position */
static Vec lbl_1_bss_1190; /* destination position */
static Vec lbl_1_bss_1184; /* start target */
static Vec lbl_1_bss_1178; /* destination target */

s32 fn_1_318E4(void);
s32 fn_1_319D4(f32 frames);
void fn_1_31AC0(Vec *pos, Vec *target);
s32 fn_1_31C60(Vec *up, f32 t);
s32 fn_1_31F84(void);
s32 fn_1_3224C(Vec *up, s32 axis, f32 t);
void fn_1_317F0(Vec *curPos, Vec *dstPos, Vec *curTgt, Vec *dstTgt);

void fn_1_30E08(void)
{
    Hu3DCameraCreate(1);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(1, 20.0f, 20.0f, 30000.0f, 1.2f);
    Hu3DCameraPosSetV(1, &lbl_1_data_7B8, &lbl_1_data_7C4, &lbl_1_data_7D0);
    Hu3DCameraCreate(2);
    Hu3DCameraViewportSet(2, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(2, 20.0f, 20.0f, 30000.0f, 1.2f);
    Hu3DCameraPosSetV(2, &lbl_1_data_7B8, &lbl_1_data_7C4, &lbl_1_data_7D0);
    lbl_1_bss_11B4 = 0;
    lbl_1_bss_11B0 = 0;
}

/* Start (first call) / continue (later calls) a camera move; returns 1 once the move is finished.
 * arg1 is the game state for modes < 9 and a world position for modes 9..11. */
// NONMATCHING: register allocation and uninitialised-register behaviour of the original differ
s32 fn_1_30FA4(s32 mode, void *arg1)
{
    Vec dstTgt;
    Vec curTgt;
    Vec dstPos;
    Vec curPos;
    CardGame *game;
    CardPlayer *pl;
    s32 done;

    done = 0;
    game = NULL;
    pl = NULL;
    if (mode < 9 && arg1 != NULL) {
        game = (CardGame *)arg1;
        pl = &game->pl[game->order[game->curPlayer]];
    }
    if (lbl_1_bss_11B4 == 0) {
        fn_1_31AC0(&curPos, &curTgt);
        switch (mode) {
            case 1:
                fn_1_9C74(&dstTgt, pl->row, pl->col, 1, 1);
                dstTgt.x += 150.0f;
                dstTgt.y += 100.0f;
                dstPos.x = dstTgt.x;
                dstPos.y = 1500.0f + dstTgt.y;
                dstPos.z = 3000.0f + dstTgt.z;
                break;
            case 2:
                fn_1_9DA8(&dstTgt, game);
                dstTgt.x += 100.0f;
                dstPos.x = dstTgt.x;
                dstPos.y = 1000.0f + dstTgt.y;
                dstPos.z = 2500.0f + dstTgt.z;
                break;
            case 3:
            case 8:
                fn_1_9DA8(&dstTgt, game);
                dstTgt.x += 100.0f;
                dstPos.x = 500.0f + dstTgt.x;
                dstPos.y = 1500.0f + dstTgt.y;
                dstPos.z = 2000.0f + dstTgt.z;
                break;
            case 4:
                fn_1_9DA8(&dstTgt, game);
                if (pl->rotY == 180.0f) {
                    dstTgt.z -= 100.0f;
                } else if (pl->rotY == 0.0f) {
                    dstTgt.z += 100.0f;
                } else if (pl->rotY == -90.0f) {
                    dstTgt.x -= 100.0f;
                } else {
                    dstTgt.x += 100.0f;
                }
                dstTgt.y += 200.0f;
                dstPos.x = dstTgt.x;
                dstPos.y = 500.0f + dstTgt.y;
                dstPos.z = 1800.0f + dstTgt.z;
                break;
            case 5:
                fn_1_9C74(&dstTgt, pl->row, pl->col, 1, 1);
                dstTgt.x += 20.0f;
                dstPos.x = dstTgt.x;
                dstPos.y = 2500.0f + dstTgt.y;
                dstPos.z = 100.0f + dstTgt.z;
                break;
            case 6:
                fn_1_9C74(&dstTgt, pl->row, pl->col, 1, 1);
                dstTgt.x += 80.0f;
                dstPos.x = dstTgt.x;
                dstPos.y = 2500.0f + dstTgt.y;
                dstPos.z = 100.0f + dstTgt.z;
                break;
            case 7:
                fn_1_9DA8(&dstTgt, game);
                dstTgt.x += 100.0f;
                dstTgt.y += 300.0f;
                dstPos.x = dstTgt.x;
                dstPos.y = 100.0f + dstTgt.y;
                dstPos.z = 2500.0f + dstTgt.z;
                break;
            case 9:
                dstTgt = *(Vec *)arg1;
                dstTgt.y += 300.0f;
                dstPos.x = dstTgt.x;
                dstPos.y = 50.0f + dstTgt.y;
                dstPos.z = 2500.0f + dstTgt.z;
                break;
            case 10:
                dstTgt = *(Vec *)arg1;
                dstTgt.y += 300.0f;
                dstPos.x = dstTgt.x;
                dstPos.y = 2500.0f + dstTgt.y;
                dstPos.z = 100.0f + dstTgt.z;
                break;
            case 11:
                dstTgt = *(Vec *)arg1;
                dstTgt.x += 100.0f;
                dstTgt.y += 300.0f;
                dstPos.x = dstTgt.x;
                dstPos.y = 2500.0f + dstTgt.y;
                dstPos.z = 4000.0f + dstTgt.z;
                break;
            default: /* mode 0 (and anything out of range): overview of the whole board */
                dstPos.x = 300.0f;
                #ifdef TARGET_PC
                /* fn_1_11C48 passes NULL; the original then reads GC low memory at 0x194 (assumed 0) */
                dstPos.y = 5000.0f + (f32)(((arg1 != NULL ? ((CardGame *)arg1)->boardSize : 0) - 6) * 0x320);
#else
                dstPos.y = 5000.0f + (f32)((((CardGame *)arg1)->boardSize - 6) * 0x320);
#endif
                dstPos.z = 5000.0f;
                dstTgt.x = 300.0f;
                dstTgt.y = 0.0f;
                dstTgt.z = 0.0f;
                break;
        }
        fn_1_317F0(&curPos, &dstPos, &curTgt, &dstTgt);
        lbl_1_bss_11B4 = 1;
    } else {
        switch (mode) {
            case 5:
            case 6:
                if (fn_1_31F84() != 0) {
                    lbl_1_bss_11B4 = 0;
                    done = 1;
                }
                break;
            case 8:
                if (fn_1_319D4(lbl_1_bss_11A8) != 0) {
                    lbl_1_bss_11B4 = 0;
                    done = 1;
                } else {
                    lbl_1_bss_11A8 -= 0.6f;
                    if (lbl_1_bss_11A8 < 60.0f) {
                        lbl_1_bss_11A8 = 60.0f;
                    }
                }
                break;
            default:
                if (fn_1_318E4() != 0) {
                    lbl_1_bss_11B4 = 0;
                    done = 1;
                }
                break;
        }
    }
    return done;
}

void fn_1_31710(Vec *pos)
{
    lbl_1_data_7B8.x = pos->x;
    lbl_1_data_7B8.y = pos->y;
    lbl_1_data_7B8.z = pos->z;
    Hu3DCameraPosSetV(1, &lbl_1_data_7B8, &lbl_1_data_7C4, &lbl_1_data_7D0);
}

void fn_1_31788(Vec *pos)
{
    lbl_1_data_7D0.x = pos->x;
    lbl_1_data_7D0.z = pos->z;
    Hu3DCameraPosSetV(1, &lbl_1_data_7B8, &lbl_1_data_7C4, &lbl_1_data_7D0);
}

void fn_1_317F0(Vec *curPos, Vec *dstPos, Vec *curTgt, Vec *dstTgt)
{
    lbl_1_bss_119C = *curPos;
    lbl_1_bss_1190 = *dstPos;
    lbl_1_bss_1184 = *curTgt;
    lbl_1_bss_1178 = *dstTgt;
    lbl_1_data_7B8 = *curPos;
    lbl_1_data_7D0 = *curTgt;
    lbl_1_bss_11AC = 0.0f;
    lbl_1_bss_11A8 = 120.0f;
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
s32 fn_1_318E4(void)
{
    s32 done;

    done = 0;
    lbl_1_bss_11AC += 1.0f;
    if (fn_1_31C60(&lbl_1_data_7C4, lbl_1_bss_11AC / 60.0f) != 0) {
        lbl_1_bss_11AC = 0.0f;
        lbl_1_data_7B8 = lbl_1_bss_1190;
        lbl_1_data_7D0 = lbl_1_bss_1178;
        done = 1;
    }
    return done;
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
s32 fn_1_319D4(f32 frames)
{
    s32 done;

    done = 0;
    lbl_1_bss_11AC += 1.0f;
    if (fn_1_31C60(&lbl_1_data_7C4, lbl_1_bss_11AC / frames) != 0) {
        lbl_1_bss_11AC = 0.0f;
        lbl_1_data_7B8 = lbl_1_bss_1190;
        lbl_1_data_7D0 = lbl_1_bss_1178;
        done = 1;
    }
    return done;
}

void fn_1_31AC0(Vec *pos, Vec *target)
{
    *pos = lbl_1_data_7B8;
    *target = lbl_1_data_7D0;
}

/* Ease between two vectors: out = a + ease(t) * (b - a). */
// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_31B04(Vec *a, Vec *b, Vec *out, f32 t)
{
    f32 k;

    if (t > 1.0f) {
        t = 1.0f;
    }
    k = CAM_EASE(t);
    out->x = a->x + (k * (b->x - a->x));
    out->y = a->y + (k * (b->y - a->y));
    out->z = a->z + (k * (b->z - a->z));
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
s32 fn_1_31C60(Vec *up, f32 t)
{
    Vec pos;
    Vec tgt;
    f32 kPos;
    f32 kTgt;
    f32 tp;
    f32 tt;

    tp = t;
    if (tp > 1.0f) {
        tp = 1.0f;
    }
    kPos = CAM_EASE(tp);
    pos.x = lbl_1_bss_119C.x + (kPos * (lbl_1_bss_1190.x - lbl_1_bss_119C.x));
    pos.y = lbl_1_bss_119C.y + (kPos * (lbl_1_bss_1190.y - lbl_1_bss_119C.y));
    pos.z = lbl_1_bss_119C.z + (kPos * (lbl_1_bss_1190.z - lbl_1_bss_119C.z));
    tt = t;
    if (tt > 1.0f) {
        tt = 1.0f;
    }
    kTgt = CAM_EASE(tt);
    tgt.x = lbl_1_bss_1184.x + (kTgt * (lbl_1_bss_1178.x - lbl_1_bss_1184.x));
    tgt.y = lbl_1_bss_1184.y + (kTgt * (lbl_1_bss_1178.y - lbl_1_bss_1184.y));
    tgt.z = lbl_1_bss_1184.z + (kTgt * (lbl_1_bss_1178.z - lbl_1_bss_1184.z));
    Hu3DCameraPosSetV(1, &pos, up, &tgt);
    if (1.0f <= t) {
        return 1;
    }
    return 0;
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
s32 fn_1_31F84(void)
{
    s32 done;

    done = 0;
    lbl_1_bss_11AC += 1.0f;
    if (fn_1_3224C(&lbl_1_data_7C4, lbl_1_bss_11B0, lbl_1_bss_11AC / 60.0f) != 0) {
        lbl_1_bss_11AC = 0.0f;
        lbl_1_data_7B8 = lbl_1_bss_1190;
        lbl_1_data_7D0 = lbl_1_bss_1178;
        if (lbl_1_bss_11B0 != 0) {
            lbl_1_bss_11B0 = 0;
            done = 1;
        } else {
            lbl_1_bss_11B0 += 1;
        }
    }
    return done;
}

/* Ease along one axis group: axis 0 moves x only, otherwise x is fixed and y/z move. */
// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_320C4(Vec *a, Vec *b, Vec *out, s32 axis, f32 t)
{
    f32 k;

    if (t > 1.0f) {
        t = 1.0f;
    }
    k = CAM_EASE(t);
    if (axis == 0) {
        out->x = a->x + (k * (b->x - a->x));
        out->y = a->y;
        out->z = a->z;
        return;
    }
    out->x = b->x;
    out->y = a->y + (k * (b->y - a->y));
    out->z = a->z + (k * (b->z - a->z));
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
s32 fn_1_3224C(Vec *up, s32 axis, f32 t)
{
    Vec pos;
    Vec tgt;
    f32 kPos;
    f32 kTgt;
    f32 tp;
    f32 tt;

    tp = t;
    if (tp > 1.0f) {
        tp = 1.0f;
    }
    kPos = CAM_EASE(tp);
    if (axis == 0) {
        pos.x = lbl_1_bss_119C.x + (kPos * (lbl_1_bss_1190.x - lbl_1_bss_119C.x));
        pos.y = lbl_1_bss_119C.y;
        pos.z = lbl_1_bss_119C.z;
    } else {
        pos.x = lbl_1_bss_1190.x;
        pos.y = lbl_1_bss_119C.y + (kPos * (lbl_1_bss_1190.y - lbl_1_bss_119C.y));
        pos.z = lbl_1_bss_119C.z + (kPos * (lbl_1_bss_1190.z - lbl_1_bss_119C.z));
    }
    tt = t;
    if (tt > 1.0f) {
        tt = 1.0f;
    }
    kTgt = CAM_EASE(tt);
    if (axis == 0) {
        tgt.x = lbl_1_bss_1184.x + (kTgt * (lbl_1_bss_1178.x - lbl_1_bss_1184.x));
        tgt.y = lbl_1_bss_1184.y;
        tgt.z = lbl_1_bss_1184.z;
    } else {
        tgt.x = lbl_1_bss_1178.x;
        tgt.y = lbl_1_bss_1184.y + (kTgt * (lbl_1_bss_1178.y - lbl_1_bss_1184.y));
        tgt.z = lbl_1_bss_1184.z + (kTgt * (lbl_1_bss_1178.z - lbl_1_bss_1184.z));
    }
    Hu3DCameraPosSetV(1, &pos, up, &tgt);
    if (1.0f <= t) {
        return 1;
    }
    return 0;
}
