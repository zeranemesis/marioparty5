// carddll/card.c - the Card Party board: random board generation, card models and the card flip animation.

#include "REL/carddll/card.h"

/* ---- .data ---- */
static char lbl_1_data_9C[8] = "S3_c029";
static char lbl_1_data_A4[8] = "S3_c028";

/* ---- .bss (non-static: referenced from the other carddll units) ---- */
s32 lbl_1_bss_320;     /* flip sound/effect started */
f32 lbl_1_bss_324;     /* card lift height */
f32 lbl_1_bss_328;     /* card flip angle */
s32 lbl_1_bss_32C;     /* flip phase */
s16 lbl_1_bss_330[50]; /* Hu3D anim ids */
s32 lbl_1_bss_394;     /* number of anims in lbl_1_bss_330 */
s32 lbl_1_bss_398;
s32 lbl_1_bss_39C[4]; /* start column of each player */
s32 lbl_1_bss_3AC[4]; /* start row of each player */
s32 lbl_1_bss_3BC[3];
s32 lbl_1_bss_3C8[4];
s32 lbl_1_bss_3D8[3];
Vec lbl_1_bss_3E4; /* world position of card [0][0] */
s32 lbl_1_bss_3F0;
s16 lbl_1_bss_3F4[30];
s16 lbl_1_bss_430[4][7];
s16 lbl_1_bss_468[4];
s16 lbl_1_bss_470[4];
s16 lbl_1_bss_478[10][10];
u8 lbl_1_bss_540[10][10][9];
s32 lbl_1_bss_8C4[10][10];
s32 lbl_1_bss_A54[10][10];

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_8EC8(CardGame *game) {
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    f32 sp8;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    s32 temp_r28;
    s32 var_r17;
    s32 var_r18;
    s32 var_r19;
    s32 var_r20;
    s32 var_r21;
    s32 var_r22;
    s32 var_r23;
    s32 var_r24;
    s32 var_r25;
    s32 var_r26;
    s32 var_r29;
    s32 var_r29_2;
    s32 var_r30;
    s32 var_r30_2;

    temp_r28 = game->boardSize;
    sp8 = 0.0f;
    spC = 0.0f;
    sp10 = 0.0f;
    var_r30 = 0;
    while (var_r30 < 4) {
        lbl_1_bss_470[var_r30] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[GwPlayerConf[var_r30].charNo + 6], 0x10000000, HEAP_MODEL));
        lbl_1_bss_468[var_r30] = Hu3DModelCreate(HuDataSelHeapReadNum(game->pl[var_r30].f28 + lbl_1_data_B0[(GwPlayerConf[var_r30].charNo * 3) + 0x24], 0x10000000, HEAP_MODEL));
        var_r29 = 0;
        while (var_r29 < 7) {
            lbl_1_bss_430[var_r30][var_r29] = Hu3DJointMotion(lbl_1_bss_468[var_r30], HuDataSelHeapReadNum(lbl_1_data_298[var_r29], 0x10000000, HEAP_MODEL));
            var_r29 += 1;
        }
        Hu3DMotionSet(lbl_1_bss_468[var_r30], lbl_1_bss_430[var_r30][6]);
        Hu3DModelCameraSet(lbl_1_bss_470[var_r30], 1U);
        Hu3DModelCameraSet(lbl_1_bss_468[var_r30], 1U);
        Hu3DModelScaleSet(lbl_1_bss_470[var_r30], 1.0f, 0.0f, 0.0f);
        Hu3DModelAttrSet(lbl_1_bss_468[var_r30], 1U);
        Hu3DModelLayerSet(lbl_1_bss_470[var_r30], 2);
        Hu3DModelLayerSet(lbl_1_bss_468[var_r30], 2);
        Hu3DModelShadowSet(lbl_1_bss_468[var_r30]);
        Hu3DModelShadowMapSet(lbl_1_bss_470[var_r30]);
        var_r30 += 1;
    }
    lbl_1_bss_3AC[0] = 0;
    lbl_1_bss_39C[0] = frandmod(temp_r28 - 2) + 1;
    lbl_1_bss_3AC[1] = temp_r28 - 1;
    lbl_1_bss_39C[1] = frandmod(temp_r28 - 2) + 1;
    lbl_1_bss_3AC[2] = frandmod(temp_r28 - 2) + 1;
    lbl_1_bss_39C[2] = 0;
    lbl_1_bss_3AC[3] = frandmod(temp_r28 - 2) + 1;
    lbl_1_bss_39C[3] = temp_r28 - 1;
    fn_1_A520(game);
    var_r30_2 = 0;
    while (var_r30_2 < temp_r28) {
        var_r29_2 = 0;
        while (var_r29_2 < temp_r28) {
            lbl_1_bss_478[var_r30_2][var_r29_2] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[lbl_1_bss_A54[var_r30_2][var_r29_2]], 0x10000000, HEAP_MODEL));
            Hu3DModelCameraSet(lbl_1_bss_478[var_r30_2][var_r29_2], 1U);
            temp_f0 = 150.0f + ((300.0f * (f32) var_r29_2) + (sp8 - (300.0f * (f32) temp_r28 * 0.5f)));
            sp14 = temp_f0;
            sp18 = spC - 10.0f;
            sp1C = 150.0f + ((300.0f * (f32) var_r30_2) + (sp10 - (300.0f * (f32) temp_r28 * 0.5f)));
            Hu3DModelPosSet(lbl_1_bss_478[var_r30_2][var_r29_2], -50.0f + temp_f0, sp18, sp1C);
            Hu3DModelRotSet(lbl_1_bss_478[var_r30_2][var_r29_2], 0.0f, 0.0f, 180.0f);
            Hu3DModelAttrSet(lbl_1_bss_478[var_r30_2][var_r29_2], 1U);
            Hu3DModelLayerSet(lbl_1_bss_478[var_r30_2][var_r29_2], 2);
            Hu3DModelShadowMapSet(lbl_1_bss_478[var_r30_2][var_r29_2]);
            if ((s32) (lbl_1_bss_8C4[var_r30_2][var_r29_2] & 0xF00) != 0) {
                if ((s32) (lbl_1_bss_8C4[var_r30_2][var_r29_2] & 0xF) == 0xF) {
                    var_r26 = 0xD;
                } else {
                    if ((s32) (lbl_1_bss_8C4[var_r30_2][var_r29_2] & 0xF) == 0xC) {
                        var_r25 = 0xE;
                    } else {
                        if ((s32) (lbl_1_bss_8C4[var_r30_2][var_r29_2] & 0xF) == 3) {
                            var_r24 = 0xF;
                        } else {
                            if ((s32) (lbl_1_bss_8C4[var_r30_2][var_r29_2] & 0xF) == 0xB) {
                                var_r23 = 0x10;
                            } else {
                                if ((s32) (lbl_1_bss_8C4[var_r30_2][var_r29_2] & 0xF) == 0xD) {
                                    var_r22 = 0x11;
                                } else {
                                    if ((s32) (lbl_1_bss_8C4[var_r30_2][var_r29_2] & 0xF) == 7) {
                                        var_r21 = 0x12;
                                    } else {
                                        if ((s32) (lbl_1_bss_8C4[var_r30_2][var_r29_2] & 0xF) == 0xE) {
                                            var_r20 = 0x13;
                                        } else {
                                            if ((s32) (lbl_1_bss_8C4[var_r30_2][var_r29_2] & 0xF) == 0xA) {
                                                var_r19 = 0x14;
                                            } else {
                                                if ((s32) (lbl_1_bss_8C4[var_r30_2][var_r29_2] & 0xF) == 9) {
                                                    var_r18 = 0x15;
                                                } else {
                                                    if ((s32) (lbl_1_bss_8C4[var_r30_2][var_r29_2] & 0xF) == 5) {
                                                        var_r17 = 0x16;
                                                    } else {
                                                        var_r17 = 0x17;
                                                    }
                                                    var_r18 = var_r17;
                                                }
                                                var_r19 = var_r18;
                                            }
                                            var_r20 = var_r19;
                                        }
                                        var_r21 = var_r20;
                                    }
                                    var_r22 = var_r21;
                                }
                                var_r23 = var_r22;
                            }
                            var_r24 = var_r23;
                        }
                        var_r25 = var_r24;
                    }
                    var_r26 = var_r25;
                }
                lbl_1_bss_3F4[lbl_1_bss_3F0] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[var_r26], 0x10000000, HEAP_MODEL));
                Hu3DModelCameraSet(lbl_1_bss_3F4[lbl_1_bss_3F0], 1U);
                Hu3DModelPosSet(lbl_1_bss_3F4[lbl_1_bss_3F0], -50.0f + sp14, sp18, sp1C);
                Hu3DModelRotSet(lbl_1_bss_3F4[lbl_1_bss_3F0], 0.0f, 0.0f, 180.0f);
                Hu3DModelAttrSet(lbl_1_bss_3F4[lbl_1_bss_3F0], 1U);
                Hu3DModelLayerSet(lbl_1_bss_3F4[lbl_1_bss_3F0], 2);
                lbl_1_bss_3F0 += 1;
            }
            if ((var_r30_2 == 0) && (var_r29_2 == 0)) {
                lbl_1_bss_3E4.x = sp14;
                lbl_1_bss_3E4.y = sp18;
                lbl_1_bss_3E4.z = sp1C;
            }
            if ((var_r30_2 == 0) && (var_r29_2 == (s32) lbl_1_bss_39C[0])) {
                sp1C -= 300.0f;
                Hu3DModelPosSet(lbl_1_bss_470[0], -50.0f + sp14, sp18, sp1C);
                Hu3DModelPosSet(lbl_1_bss_468[0], -50.0f + sp14, sp18, sp1C);
                game->pl[0].f00 = 1;
                game->pl[0].row = var_r30_2;
                game->pl[0].col = var_r29_2;
                game->pl[0].f20 = 1;
                game->pl[0].f24 = 1;
                game->pl[0].f04 = 1;
                game->pl[0].f08 = var_r30_2;
                game->pl[0].f0C = var_r29_2;
            }
            if ((var_r30_2 == (s32) (temp_r28 - 1)) && (var_r29_2 == (s32) lbl_1_bss_39C[1])) {
                sp1C += 300.0f;
                Hu3DModelPosSet(lbl_1_bss_470[1], -50.0f + sp14, sp18, sp1C);
                Hu3DModelPosSet(lbl_1_bss_468[1], -50.0f + sp14, sp18, sp1C);
                game->pl[1].f00 = 2;
                game->pl[1].row = var_r30_2;
                game->pl[1].col = var_r29_2;
                game->pl[1].f20 = 1;
                game->pl[1].f24 = 1;
                game->pl[1].f04 = 2;
                game->pl[1].f08 = var_r30_2;
                game->pl[1].f0C = var_r29_2;
            }
            if ((var_r29_2 == 0) && (var_r30_2 == (s32) lbl_1_bss_3AC[2])) {
                temp_f0_2 = sp14 - 300.0f;
                sp14 = temp_f0_2;
                Hu3DModelPosSet(lbl_1_bss_470[2], -50.0f + temp_f0_2, sp18, sp1C);
                Hu3DModelPosSet(lbl_1_bss_468[2], -50.0f + temp_f0_2, sp18, sp1C);
                game->pl[2].f00 = 3;
                game->pl[2].row = var_r30_2;
                game->pl[2].col = var_r29_2;
                game->pl[2].f20 = 1;
                game->pl[2].f24 = 1;
                game->pl[2].f04 = 3;
                game->pl[2].f08 = var_r30_2;
                game->pl[2].f0C = var_r29_2;
            }
            if ((var_r29_2 == (s32) (temp_r28 - 1)) && (var_r30_2 == (s32) lbl_1_bss_3AC[3])) {
                temp_f0_3 = sp14 + 300.0f;
                sp14 = temp_f0_3;
                Hu3DModelPosSet(lbl_1_bss_470[3], -50.0f + temp_f0_3, sp18, sp1C);
                Hu3DModelPosSet(lbl_1_bss_468[3], -50.0f + temp_f0_3, sp18, sp1C);
                game->pl[3].f00 = 4;
                game->pl[3].row = var_r30_2;
                game->pl[3].col = var_r29_2;
                game->pl[3].f20 = 1;
                game->pl[3].f24 = 1;
                game->pl[3].f04 = 4;
                game->pl[3].f08 = var_r30_2;
                game->pl[3].f0C = var_r29_2;
            }
            var_r29_2 += 1;
        }
        var_r30_2 += 1;
    }
}

/* World position of card (row, col); xSel/zSel (0, 1, 2) pick the left/centre/right slot of the card. */
// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_9C74(Vec *out, s32 row, s32 col, s32 zSel, s32 xSel)
{
    out->x = lbl_1_bss_3E4.x + (300.0f * (f32)col);
    out->y = lbl_1_bss_3E4.y;
    out->z = lbl_1_bss_3E4.z + (300.0f * (f32)row);
    if (xSel == 0) {
        out->x = out->x - 98.4375f;
    } else if (xSel == 2) {
        out->x = out->x + 98.4375f;
    }
    if (zSel == 0) {
        out->z = out->z - 98.4375f;
    } else if (zSel == 2) {
        out->z = out->z + 98.4375f;
    }
}

/* Position the current player's pawn stands on: the cell in front of it (facing f00) and the
 * matching pawn rotation. */
// NONMATCHING: the five facing cases are merged into one path here (same result)
void fn_1_9DA8(Vec *out, CardGame *game)
{
    CardPlayer *pl;
    s32 xSel;
    s32 zSel;
    s32 row;
    s32 col;
    f32 rot;

    pl = &game->pl[game->order[game->curPlayer]];
    if (pl->f00 == 0) {
        row = pl->row;
        col = pl->col;
        rot = pl->rotY;
    } else if (pl->f00 == 1) {
        row = pl->row - 1;
        col = pl->col;
        rot = 0.0f;
    } else if (pl->f00 == 2) {
        row = pl->row + 1;
        col = pl->col;
        rot = 180.0f;
    } else if (pl->f00 == 3) {
        row = pl->row;
        col = pl->col - 1;
        rot = 90.0f;
    } else if (pl->f00 == 4) {
        row = pl->row;
        col = pl->col + 1;
        rot = -90.0f;
    } else {
        return;
    }
    xSel = pl->f24;
    zSel = pl->f20;
    out->x = lbl_1_bss_3E4.x + (300.0f * (f32)col);
    out->y = lbl_1_bss_3E4.y;
    out->z = lbl_1_bss_3E4.z + (300.0f * (f32)row);
    if (xSel == 0) {
        out->x = out->x - 98.4375f;
    } else if (xSel == 2) {
        out->x = out->x + 98.4375f;
    }
    if (zSel == 0) {
        out->z = out->z - 98.4375f;
    } else if (zSel == 2) {
        out->z = out->z + 98.4375f;
    }
    if (pl->f00 != 0) {
        pl->rotY = rot;
        pl->rotY2 = rot;
    }
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_A520(CardGame *game) {
    s32 sp2C;
    s32 sp28;
    s32 sp24;
    s32 sp20;
    s32 sp1C;
    s32 sp18;
    s32 sp14;
    s32 sp10;
    s32 spC;
    CardGame *sp8;
    s32 *temp_r3;
    s32 temp_r29;
    s32 var_r17;
    s32 var_r18;
    s32 var_r19;
    s32 var_r20;
    s32 var_r21;
    s32 var_r22;
    s32 var_r23;
    s32 var_r24;
    s32 var_r25;
    s32 var_r26;
    s32 var_r27;
    s32 var_r28;
    s32 var_r30;
    s32 var_r30_2;
    s32 var_r30_3;
    s32 var_r31;
    s32 var_r31_2;
    s32 var_r31_3;

    sp8 = game;
    temp_r29 = game->boardSize;
    var_r31 = 0;
    while (var_r31 < 0xA) {
        var_r30 = 0;
        while (var_r30 < 0xA) {
            lbl_1_bss_8C4[var_r31][var_r30] = 0;
            var_r30 += 1;
        }
        var_r31 += 1;
    }
    fn_1_B104(temp_r29);
    var_r31_2 = 1;
    while (var_r31_2 < (s32) (temp_r29 - 1)) {
        var_r30_2 = 1;
        while (var_r30_2 < (s32) (temp_r29 - 1)) {
            fn_1_BB70(var_r31_2, var_r30_2, temp_r29);
            var_r30_2 += 1;
        }
        var_r31_2 += 1;
    }
    fn_1_C864(sp8);
    var_r31_3 = 0;
    while (var_r31_3 < temp_r29) {
        var_r30_3 = 0;
        while (var_r30_3 < temp_r29) {
            if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0x100) != 0) {
                if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0x1000) != 0) {
                    var_r28 = 0x18;
                } else {
                    if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0x2000) != 0) {
                        var_r27 = 0x19;
                    } else {
                        var_r27 = 0x1A;
                    }
                    var_r28 = var_r27;
                }
                var_r26 = var_r28;
            } else {
                if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0x200) != 0) {
                    if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0x1000) != 0) {
                        var_r25 = 0x1B;
                    } else {
                        if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0x2000) != 0) {
                            var_r24 = 0x1C;
                        } else {
                            if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0x4000) != 0) {
                                var_r23 = 0x1D;
                            } else {
                                var_r23 = 0x1E;
                            }
                            var_r24 = var_r23;
                        }
                        var_r25 = var_r24;
                    }
                    var_r22 = var_r25;
                } else {
                    if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0x400) != 0) {
                        if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0x1000) != 0) {
                            var_r21 = 0x21;
                        } else {
                            if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0x2000) != 0) {
                                var_r20 = 0x22;
                            } else {
                                var_r20 = 0x23;
                            }
                            var_r21 = var_r20;
                        }
                        var_r19 = var_r21;
                    } else {
                        if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0x800) != 0) {
                            var_r18 = 0x1F;
                        } else {
                            if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0xF) == 0xF) {
                                var_r17 = 0xD;
                            } else {
                                if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0xF) == 0xC) {
                                    sp2C = 0xE;
                                } else {
                                    if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0xF) == 3) {
                                        sp28 = 0xF;
                                    } else {
                                        if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0xF) == 0xB) {
                                            sp24 = 0x10;
                                        } else {
                                            if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0xF) == 0xD) {
                                                sp20 = 0x11;
                                            } else {
                                                if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0xF) == 7) {
                                                    sp1C = 0x12;
                                                } else {
                                                    if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0xF) == 0xE) {
                                                        sp18 = 0x13;
                                                    } else {
                                                        if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0xF) == 0xA) {
                                                            sp14 = 0x14;
                                                        } else {
                                                            if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0xF) == 9) {
                                                                sp10 = 0x15;
                                                            } else {
                                                                if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0xF) == 5) {
                                                                    spC = 0x16;
                                                                } else {
                                                                    spC = 0x17;
                                                                }
                                                                sp10 = spC;
                                                            }
                                                            sp14 = sp10;
                                                        }
                                                        sp18 = sp14;
                                                    }
                                                    sp1C = sp18;
                                                }
                                                sp20 = sp1C;
                                            }
                                            sp24 = sp20;
                                        }
                                        sp28 = sp24;
                                    }
                                    sp2C = sp28;
                                }
                                var_r17 = sp2C;
                            }
                            var_r18 = var_r17;
                        }
                        var_r19 = var_r18;
                    }
                    var_r22 = var_r19;
                }
                var_r26 = var_r22;
            }
            lbl_1_bss_A54[var_r31_3][var_r30_3] = var_r26;
            fn_1_AB64(var_r31_3, var_r30_3, lbl_1_bss_8C4[var_r31_3][var_r30_3]);
            if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_3] & 0xF00) != 0) {
                temp_r3 = &lbl_1_bss_8C4[var_r31_3][var_r30_3];
                *temp_r3 |= 0x80;
            }
            var_r30_3 += 1;
        }
        var_r31_3 += 1;
    }
    lbl_1_bss_398 = 0;
    lbl_1_bss_394 = 0;
    lbl_1_bss_3F0 = 0;
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_AB64(s32 row, s32 col, s32 flags) {
    s32 temp_r6;
    s32 var_r30;
    s32 var_r31;

    var_r30 = 0;
    while (var_r30 < 3) {
        var_r31 = 0;
        while (var_r31 < 3) {
            *(lbl_1_bss_540[row][col] + (var_r30 * 3) + var_r31) = 0;
            var_r31 += 1;
        }
        var_r30 += 1;
    }
    temp_r6 = flags & 0xF;
    switch (temp_r6) {
    case 15:
        lbl_1_bss_540[row][col][1] = 3;
        lbl_1_bss_540[row][col][3] = 0xC;
        lbl_1_bss_540[row][col][4] = 0xF;
        lbl_1_bss_540[row][col][5] = 0xC;
        lbl_1_bss_540[row][col][7] = 3;
        return;
    case 12:
        lbl_1_bss_540[row][col][3] = 0xC;
        lbl_1_bss_540[row][col][4] = 0xC;
        lbl_1_bss_540[row][col][5] = 0xC;
        return;
    case 3:
        lbl_1_bss_540[row][col][1] = 3;
        lbl_1_bss_540[row][col][4] = 3;
        lbl_1_bss_540[row][col][7] = 3;
        return;
    case 11:
        lbl_1_bss_540[row][col][1] = 3;
        lbl_1_bss_540[row][col][4] = 0xB;
        lbl_1_bss_540[row][col][5] = 0xC;
        lbl_1_bss_540[row][col][7] = 3;
        return;
    case 13:
        lbl_1_bss_540[row][col][1] = 3;
        lbl_1_bss_540[row][col][3] = 0xC;
        lbl_1_bss_540[row][col][4] = 0xD;
        lbl_1_bss_540[row][col][5] = 0xC;
        return;
    case 7:
        lbl_1_bss_540[row][col][1] = 3;
        lbl_1_bss_540[row][col][3] = 0xC;
        lbl_1_bss_540[row][col][4] = 7;
        lbl_1_bss_540[row][col][7] = 3;
        return;
    case 14:
        lbl_1_bss_540[row][col][3] = 0xC;
        lbl_1_bss_540[row][col][4] = 0xE;
        lbl_1_bss_540[row][col][5] = 0xC;
        lbl_1_bss_540[row][col][7] = 3;
        return;
    case 10:
        lbl_1_bss_540[row][col][4] = 0xA;
        lbl_1_bss_540[row][col][5] = 0xC;
        lbl_1_bss_540[row][col][7] = 3;
        return;
    case 9:
        lbl_1_bss_540[row][col][1] = 3;
        lbl_1_bss_540[row][col][4] = 9;
        lbl_1_bss_540[row][col][5] = 0xC;
        return;
    case 5:
        lbl_1_bss_540[row][col][1] = 3;
        lbl_1_bss_540[row][col][3] = 0xC;
        lbl_1_bss_540[row][col][4] = 5;
        return;
    case 6:
        lbl_1_bss_540[row][col][3] = 0xC;
        lbl_1_bss_540[row][col][4] = 6;
        lbl_1_bss_540[row][col][7] = 3;
        /* fallthrough */
    default:
        return;
    }
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_B104(s32 size) {
    s32 *temp_r3;
    s32 *temp_r3_10;
    s32 *temp_r3_11;
    s32 *temp_r3_12;
    s32 *temp_r3_14;
    s32 *temp_r3_15;
    s32 *temp_r3_16;
    s32 *temp_r3_17;
    s32 *temp_r3_18;
    s32 *temp_r3_20;
    s32 *temp_r3_21;
    s32 *temp_r3_22;
    s32 *temp_r3_3;
    s32 *temp_r3_4;
    s32 *temp_r3_5;
    s32 *temp_r3_6;
    s32 *temp_r3_7;
    s32 *temp_r3_9;
    s32 *temp_r4;
    s32 *temp_r4_10;
    s32 *temp_r4_11;
    s32 *temp_r4_12;
    s32 *temp_r4_13;
    s32 *temp_r4_14;
    s32 *temp_r4_2;
    s32 *temp_r4_3;
    s32 *temp_r4_4;
    s32 *temp_r4_5;
    s32 *temp_r4_6;
    s32 *temp_r4_7;
    s32 *temp_r4_8;
    s32 *temp_r4_9;
    s32 *temp_r5;
    s32 *temp_r5_2;
    s32 *temp_r5_3;
    s32 *temp_r5_4;
    s32 temp_r3_13;
    s32 temp_r3_19;
    s32 temp_r3_2;
    s32 temp_r3_8;
    s32 var_r21;
    s32 var_r22;
    s32 var_r23;
    s32 var_r24;
    s32 var_r25;
    s32 var_r26;
    s32 var_r27;
    s32 var_r28;
    s32 var_r31;
    s32 var_r31_2;
    s32 var_r31_3;
    s32 var_r31_4;

    lbl_1_bss_8C4[0][0] = 0xA;
    *lbl_1_bss_8C4[size - 1] = 9;
    (*lbl_1_bss_8C4)[size - 1] = 6;
    lbl_1_bss_8C4[size - 1][size - 1] = 5;
    var_r31 = 1;
    while (var_r31 < (s32) (size - 1)) {
        if ((s32) ((*lbl_1_bss_8C4)[var_r31 - 1] & 8) != 0) {
            var_r28 = 4;
        } else {
            var_r28 = 0;
        }
        temp_r4 = &(*lbl_1_bss_8C4)[var_r31];
        *temp_r4 |= var_r28;
        if ((s32) (*lbl_1_bss_8C4)[var_r31] == 0) {
            temp_r4_2 = &(*lbl_1_bss_8C4)[var_r31];
            *temp_r4_2 |= 2;
        } else {
            temp_r4_3 = &(*lbl_1_bss_8C4)[var_r31];
            *temp_r4_3 |= frandmod(2U) * 2;
        }
        if (var_r31 == (s32) (size - 2)) {
            if ((s32) ((*lbl_1_bss_8C4)[size - 1] & 4) != 0) {
                var_r27 = 8;
            } else {
                var_r27 = 0;
            }
            temp_r3 = &(*lbl_1_bss_8C4)[var_r31];
            *temp_r3 |= var_r27;
        } else if ((s32) (*lbl_1_bss_8C4)[var_r31] == 4) {
            temp_r3_2 = frandmod(3U);
            if (temp_r3_2 == 0) {
                temp_r3_3 = &(*lbl_1_bss_8C4)[var_r31];
                *temp_r3_3 |= 0xA;
            } else if (temp_r3_2 == 1) {
                temp_r3_4 = &(*lbl_1_bss_8C4)[var_r31];
                *temp_r3_4 |= 8;
            } else {
                temp_r4_4 = &(*lbl_1_bss_8C4)[var_r31];
                *temp_r4_4 |= 2;
            }
        } else if ((s32) (*lbl_1_bss_8C4)[var_r31] == 2) {
            temp_r4_5 = &(*lbl_1_bss_8C4)[var_r31];
            *temp_r4_5 |= 8;
        } else {
            temp_r3_5 = &(*lbl_1_bss_8C4)[var_r31];
            *temp_r3_5 |= frandmod(2U) * 8;
        }
        var_r31 += 1;
    }
    var_r31_2 = 1;
    while (var_r31_2 < (s32) (size - 1)) {
        if ((s32) (lbl_1_bss_8C4[size - 1][var_r31_2 - 1] & 8) != 0) {
            var_r26 = 4;
        } else {
            var_r26 = 0;
        }
        temp_r4_6 = &lbl_1_bss_8C4[size - 1][var_r31_2];
        *temp_r4_6 |= var_r26;
        if ((s32) lbl_1_bss_8C4[size - 1][var_r31_2] == 0) {
            temp_r3_6 = &lbl_1_bss_8C4[size - 1][var_r31_2];
            *temp_r3_6 |= 1;
        } else {
            temp_r4_7 = &lbl_1_bss_8C4[size - 1][var_r31_2];
            *temp_r4_7 |= frandmod(2U);
        }
        if (var_r31_2 == (s32) (size - 2)) {
            if ((s32) (lbl_1_bss_8C4[size - 1][size - 1] & 4) != 0) {
                var_r25 = 8;
            } else {
                var_r25 = 0;
            }
            temp_r3_7 = &lbl_1_bss_8C4[size - 1][var_r31_2];
            *temp_r3_7 |= var_r25;
        } else if ((s32) lbl_1_bss_8C4[size - 1][var_r31_2] == 4) {
            temp_r3_8 = frandmod(3U);
            if (temp_r3_8 == 0) {
                temp_r5 = &lbl_1_bss_8C4[size - 1][var_r31_2];
                *temp_r5 |= 9;
            } else if (temp_r3_8 == 1) {
                temp_r5_2 = &lbl_1_bss_8C4[size - 1][var_r31_2];
                *temp_r5_2 |= 8;
            } else {
                temp_r4_8 = &lbl_1_bss_8C4[size - 1][var_r31_2];
                *temp_r4_8 |= 1;
            }
        } else if ((s32) lbl_1_bss_8C4[size - 1][var_r31_2] == 1) {
            temp_r3_9 = &lbl_1_bss_8C4[size - 1][var_r31_2];
            *temp_r3_9 |= 8;
        } else {
            temp_r3_10 = &lbl_1_bss_8C4[size - 1][var_r31_2];
            *temp_r3_10 |= frandmod(2U) * 8;
        }
        var_r31_2 += 1;
    }
    var_r31_3 = 1;
    while (var_r31_3 < (s32) (size - 1)) {
        if ((s32) (*lbl_1_bss_8C4[var_r31_3 - 1] & 2) != 0) {
            var_r24 = 1;
        } else {
            var_r24 = 0;
        }
        temp_r4_9 = lbl_1_bss_8C4[var_r31_3];
        *temp_r4_9 |= var_r24;
        if ((s32) *lbl_1_bss_8C4[var_r31_3] == 0) {
            temp_r3_11 = lbl_1_bss_8C4[var_r31_3];
            *temp_r3_11 |= 8;
        } else {
            temp_r5_3 = lbl_1_bss_8C4[var_r31_3];
            *temp_r5_3 |= frandmod(2U) * 8;
        }
        if (var_r31_3 == (s32) (size - 2)) {
            if ((s32) (*lbl_1_bss_8C4[size - 1] & 1) != 0) {
                var_r23 = 2;
            } else {
                var_r23 = 0;
            }
            temp_r3_12 = lbl_1_bss_8C4[var_r31_3];
            *temp_r3_12 |= var_r23;
        } else if ((s32) *lbl_1_bss_8C4[var_r31_3] == 1) {
            temp_r3_13 = frandmod(3U);
            if (temp_r3_13 == 0) {
                temp_r3_14 = lbl_1_bss_8C4[var_r31_3];
                *temp_r3_14 |= 0xA;
            } else if (temp_r3_13 == 1) {
                temp_r4_10 = lbl_1_bss_8C4[var_r31_3];
                *temp_r4_10 |= 2;
            } else {
                temp_r3_15 = lbl_1_bss_8C4[var_r31_3];
                *temp_r3_15 |= 8;
            }
        } else if ((s32) *lbl_1_bss_8C4[var_r31_3] == 8) {
            temp_r3_16 = lbl_1_bss_8C4[var_r31_3];
            *temp_r3_16 |= 2;
        } else {
            temp_r5_4 = lbl_1_bss_8C4[var_r31_3];
            *temp_r5_4 |= frandmod(2U) * 2;
        }
        var_r31_3 += 1;
    }
    var_r31_4 = 1;
    while (var_r31_4 < (s32) (size - 1)) {
        if ((s32) (lbl_1_bss_8C4[var_r31_4 - 1][size - 1] & 2) != 0) {
            var_r22 = 1;
        } else {
            var_r22 = 0;
        }
        temp_r3_17 = &lbl_1_bss_8C4[var_r31_4][size - 1];
        *temp_r3_17 |= var_r22;
        if ((s32) lbl_1_bss_8C4[var_r31_4][size - 1] == 0) {
            temp_r3_18 = &lbl_1_bss_8C4[var_r31_4][size - 1];
            *temp_r3_18 |= 4;
        } else {
            temp_r4_11 = &lbl_1_bss_8C4[var_r31_4][size - 1];
            *temp_r4_11 |= frandmod(2U) * 4;
        }
        if (var_r31_4 == (s32) (size - 2)) {
            if ((s32) (lbl_1_bss_8C4[size - 1][size - 1] & 1) != 0) {
                var_r21 = 2;
            } else {
                var_r21 = 0;
            }
            temp_r4_12 = &lbl_1_bss_8C4[var_r31_4][size - 1];
            *temp_r4_12 |= var_r21;
        } else if ((s32) lbl_1_bss_8C4[var_r31_4][size - 1] == 1) {
            temp_r3_19 = frandmod(3U);
            if (temp_r3_19 == 0) {
                temp_r4_13 = &lbl_1_bss_8C4[var_r31_4][size - 1];
                *temp_r4_13 |= 6;
            } else if (temp_r3_19 == 1) {
                temp_r3_20 = &lbl_1_bss_8C4[var_r31_4][size - 1];
                *temp_r3_20 |= 2;
            } else {
                temp_r3_21 = &lbl_1_bss_8C4[var_r31_4][size - 1];
                *temp_r3_21 |= 4;
            }
        } else if ((s32) lbl_1_bss_8C4[var_r31_4][size - 1] == 4) {
            temp_r4_14 = &lbl_1_bss_8C4[var_r31_4][size - 1];
            *temp_r4_14 |= 2;
        } else {
            temp_r3_22 = &lbl_1_bss_8C4[var_r31_4][size - 1];
            *temp_r3_22 |= frandmod(2U) * 2;
        }
        var_r31_4 += 1;
    }
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_BB70(s32 row, s32 col, s32 size) {
    s32 *temp_r3;
    s32 *temp_r3_10;
    s32 *temp_r3_11;
    s32 *temp_r3_13;
    s32 *temp_r3_14;
    s32 *temp_r3_15;
    s32 *temp_r3_17;
    s32 *temp_r3_18;
    s32 *temp_r3_19;
    s32 *temp_r3_20;
    s32 *temp_r3_3;
    s32 *temp_r3_4;
    s32 *temp_r3_5;
    s32 *temp_r3_7;
    s32 *temp_r3_8;
    s32 *temp_r3_9;
    s32 *temp_r4;
    s32 *temp_r4_10;
    s32 *temp_r4_11;
    s32 *temp_r4_12;
    s32 *temp_r4_13;
    s32 *temp_r4_14;
    s32 *temp_r4_15;
    s32 *temp_r4_16;
    s32 *temp_r4_17;
    s32 *temp_r4_18;
    s32 *temp_r4_19;
    s32 *temp_r4_20;
    s32 *temp_r4_21;
    s32 *temp_r4_22;
    s32 *temp_r4_23;
    s32 *temp_r4_24;
    s32 *temp_r4_25;
    s32 *temp_r4_26;
    s32 *temp_r4_2;
    s32 *temp_r4_3;
    s32 *temp_r4_4;
    s32 *temp_r4_5;
    s32 *temp_r4_6;
    s32 *temp_r4_7;
    s32 *temp_r4_8;
    s32 *temp_r4_9;
    s32 *temp_r5;
    s32 *temp_r5_2;
    s32 *temp_r5_3;
    s32 temp_r3_12;
    s32 temp_r3_16;
    s32 temp_r3_2;
    s32 temp_r3_6;
    s32 var_r22;
    s32 var_r23;
    s32 var_r24;
    s32 var_r25;
    s32 var_r27;
    s32 var_r28;
    s32 var_r29;
    s32 var_r29_2;

    if ((s32) (lbl_1_bss_8C4[row - 1][col] & 2) != 0) {
        var_r25 = 1;
    } else {
        var_r25 = 0;
    }
    temp_r4 = &lbl_1_bss_8C4[row][col];
    *temp_r4 |= var_r25;
    if ((s32) (lbl_1_bss_8C4[row][col - 1] & 8) != 0) {
        var_r24 = 4;
    } else {
        var_r24 = 0;
    }
    temp_r4_2 = &lbl_1_bss_8C4[row][col];
    *temp_r4_2 |= var_r24;
    if (col == (s32) (size - 2)) {
        if ((s32) (lbl_1_bss_8C4[row][size - 1] & 4) != 0) {
            var_r23 = 8;
        } else {
            var_r23 = 0;
        }
        temp_r3 = &lbl_1_bss_8C4[row][col];
        *temp_r3 |= var_r23;
    } else {
        temp_r4_3 = &lbl_1_bss_8C4[row][col];
        *temp_r4_3 |= frandmod(2U) * 8;
    }
    if ((s32) lbl_1_bss_8C4[row][col] == 0) {
        temp_r3_2 = frandmod(4U);
        switch (temp_r3_2) {                        /* irregular */
        case 0:
            temp_r3_3 = &lbl_1_bss_8C4[row][col];
            *temp_r3_3 |= 0xD;
            temp_r4_4 = &lbl_1_bss_8C4[row - 1][col];
            *temp_r4_4 |= 2;
            temp_r4_5 = &lbl_1_bss_8C4[row][col - 1];
            *temp_r4_5 |= 8;
            temp_r4_6 = &lbl_1_bss_8C4[row][col + 1];
            *temp_r4_6 |= 4;
            break;
        case 1:
            temp_r5 = &lbl_1_bss_8C4[row][col];
            *temp_r5 |= 5;
            temp_r4_7 = &lbl_1_bss_8C4[row - 1][col];
            *temp_r4_7 |= 2;
            temp_r4_8 = &lbl_1_bss_8C4[row][col - 1];
            *temp_r4_8 |= 8;
            break;
        case 2:
            temp_r5_2 = &lbl_1_bss_8C4[row][col];
            *temp_r5_2 |= 9;
            temp_r4_9 = &lbl_1_bss_8C4[row - 1][col];
            *temp_r4_9 |= 2;
            temp_r3_4 = &lbl_1_bss_8C4[row][col + 1];
            *temp_r3_4 |= 4;
            break;
        default:
            temp_r3_5 = &lbl_1_bss_8C4[row][col];
            *temp_r3_5 |= 0xC;
            temp_r4_10 = &lbl_1_bss_8C4[row][col - 1];
            *temp_r4_10 |= 8;
            temp_r4_11 = &lbl_1_bss_8C4[row][col + 1];
            *temp_r4_11 |= 4;
            break;
        }
    } else if ((s32) lbl_1_bss_8C4[row][col] == 1) {
        temp_r3_6 = frandmod(3U);
        if (temp_r3_6 == 0) {
            temp_r4_12 = &lbl_1_bss_8C4[row][col];
            *temp_r4_12 |= 0xC;
            temp_r3_7 = &lbl_1_bss_8C4[row][col - 1];
            *temp_r3_7 |= 8;
            temp_r3_8 = &lbl_1_bss_8C4[row][col + 1];
            *temp_r3_8 |= 4;
        } else if (temp_r3_6 == 1) {
            temp_r3_9 = &lbl_1_bss_8C4[row][col];
            *temp_r3_9 |= 4;
            temp_r3_10 = &lbl_1_bss_8C4[row][col - 1];
            *temp_r3_10 |= 8;
        } else {
            temp_r4_13 = &lbl_1_bss_8C4[row][col];
            *temp_r4_13 |= 8;
            temp_r3_11 = &lbl_1_bss_8C4[row][col + 1];
            *temp_r3_11 |= 4;
        }
    } else if ((s32) lbl_1_bss_8C4[row][col] == 4) {
        temp_r3_12 = frandmod(3U);
        if (temp_r3_12 == 0) {
            temp_r4_14 = &lbl_1_bss_8C4[row][col];
            *temp_r4_14 |= 9;
            temp_r4_15 = &lbl_1_bss_8C4[row - 1][col];
            *temp_r4_15 |= 2;
            temp_r3_13 = &lbl_1_bss_8C4[row][col + 1];
            *temp_r3_13 |= 4;
        } else if (temp_r3_12 == 1) {
            temp_r4_16 = &lbl_1_bss_8C4[row][col];
            *temp_r4_16 |= 1;
            temp_r4_17 = &lbl_1_bss_8C4[row - 1][col];
            *temp_r4_17 |= 2;
        } else {
            temp_r3_14 = &lbl_1_bss_8C4[row][col];
            *temp_r3_14 |= 8;
            temp_r3_15 = &lbl_1_bss_8C4[row][col + 1];
            *temp_r3_15 |= 4;
        }
    } else if ((s32) lbl_1_bss_8C4[row][col] == 8) {
        temp_r3_16 = frandmod(3U);
        if (temp_r3_16 == 0) {
            temp_r4_18 = &lbl_1_bss_8C4[row][col];
            *temp_r4_18 |= 5;
            temp_r4_19 = &lbl_1_bss_8C4[row - 1][col];
            *temp_r4_19 |= 2;
            temp_r3_17 = &lbl_1_bss_8C4[row][col - 1];
            *temp_r3_17 |= 8;
        } else if (temp_r3_16 == 1) {
            temp_r4_20 = &lbl_1_bss_8C4[row][col];
            *temp_r4_20 |= 1;
            temp_r4_21 = &lbl_1_bss_8C4[row - 1][col];
            *temp_r4_21 |= 2;
        } else {
            temp_r4_22 = &lbl_1_bss_8C4[row][col];
            *temp_r4_22 |= 4;
            temp_r4_23 = &lbl_1_bss_8C4[row][col - 1];
            *temp_r4_23 |= 8;
        }
    }
    if (row == (s32) (size - 2)) {
        if ((s32) (lbl_1_bss_8C4[size - 1][col] & 1) != 0) {
            var_r22 = 2;
        } else {
            var_r22 = 0;
        }
        temp_r3_18 = &lbl_1_bss_8C4[row][col];
        *temp_r3_18 |= var_r22;
    } else if (((s32) lbl_1_bss_8C4[row][col] == 5) || ((s32) lbl_1_bss_8C4[row][col] == 9)) {
        temp_r4_24 = &lbl_1_bss_8C4[row][col];
        *temp_r4_24 |= 2;
    } else {
        temp_r4_25 = &lbl_1_bss_8C4[row][col];
        *temp_r4_25 |= frandmod(2U) * 2;
    }
    if (col == (s32) (size - 2)) {
        var_r28 = 0;
        var_r29 = 0;
        while (1) {
            if (var_r29 == 0) {
                if ((s32) (lbl_1_bss_8C4[row][var_r28] & 2) == 0) {
                    var_r28 += 1;
                    if (var_r28 == size) {
                        var_r29 = frandmod((u32) size) + 1;
                        var_r28 = 0;
                    }
                    continue;
                }
                break;
            }
            if (frandmod(5U) == 0) {
                temp_r3_19 = &lbl_1_bss_8C4[row][var_r28];
                *temp_r3_19 |= 2;
                temp_r3_20 = &lbl_1_bss_8C4[row + 1][var_r28];
                *temp_r3_20 |= 1;
                var_r28 += 1;
                var_r29 -= 1;
                if (var_r28 == size) {
                    var_r28 = 0;
                }
                if (var_r29 != 0) {
                    continue;
                }
            } else {
                continue;
            }
            break;
        }
    }
    if (row == (s32) (size - 2)) {
        var_r27 = 0;
        var_r29_2 = 0;
        while (1) {
            if (var_r29_2 == 0) {
                if ((s32) (lbl_1_bss_8C4[var_r27][col] & 8) == 0) {
                    var_r27 += 1;
                    if (var_r27 == size) {
                        var_r29_2 = frandmod((u32) size) + 1;
                        var_r27 = 0;
                    }
                    continue;
                }
                break;
            }
            if (frandmod(5U) == 0) {
                temp_r4_26 = &lbl_1_bss_8C4[var_r27][col];
                *temp_r4_26 |= 8;
                temp_r5_3 = &lbl_1_bss_8C4[var_r27][col + 1];
                *temp_r5_3 |= 4;
                var_r27 += 1;
                var_r29_2 -= 1;
                if (var_r27 == size) {
                    var_r27 = 0;
                }
                if (var_r29_2 != 0) {
                    continue;
                }
            } else {
                continue;
            }
            break;
        }
    }
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_C614(s32 size) {
    s32 temp_r3;
    s32 temp_r3_2;
    s32 temp_r3_3;
    s32 var_r31;
    s32 var_r31_2;
    s32 var_r31_3;

    lbl_1_bss_3D8[2] = 0;
    lbl_1_bss_3D8[1] = 0;
    lbl_1_bss_3D8[0] = 0;
    lbl_1_bss_3C8[3] = 0;
    lbl_1_bss_3C8[2] = 0;
    lbl_1_bss_3C8[1] = 0;
    lbl_1_bss_3C8[0] = 0;
    lbl_1_bss_3BC[2] = 0;
    lbl_1_bss_3BC[1] = 0;
    lbl_1_bss_3BC[0] = 0;
    var_r31 = 0;
    while (var_r31 < size) {
        temp_r3 = frandmod(0x64U);
        if (temp_r3 < 0x32) {
            lbl_1_bss_3D8[0] += 1;
        } else if (temp_r3 < 0x50) {
            lbl_1_bss_3D8[1] += 1;
        } else {
            lbl_1_bss_3D8[2] += 1;
        }
        var_r31 += 1;
    }
    var_r31_2 = 0;
    while (var_r31_2 < size) {
        temp_r3_2 = frandmod(0x64U);
        if (temp_r3_2 < 0xA) {
            lbl_1_bss_3C8[0] += 1;
        } else if (temp_r3_2 < 0x28) {
            lbl_1_bss_3C8[1] += 1;
        } else if (temp_r3_2 < 0x46) {
            lbl_1_bss_3C8[2] += 1;
        } else {
            lbl_1_bss_3C8[3] += 1;
        }
        var_r31_2 += 1;
    }
    var_r31_3 = 0;
    while (var_r31_3 < size) {
        temp_r3_3 = frandmod(0x64U);
        if (temp_r3_3 < 0x3C) {
            lbl_1_bss_3BC[0] += 1;
        } else if (temp_r3_3 < 0x50) {
            lbl_1_bss_3BC[1] += 1;
        } else {
            lbl_1_bss_3BC[2] += 1;
        }
        var_r31_3 += 1;
    }
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_C864(CardGame *game) {
    s32 *temp_r3;
    s32 *temp_r3_10;
    s32 *temp_r3_5;
    s32 *temp_r3_6;
    s32 *temp_r3_8;
    s32 *temp_r3_9;
    s32 *temp_r4;
    s32 *temp_r4_10;
    s32 *temp_r4_11;
    s32 *temp_r4_12;
    s32 *temp_r4_13;
    s32 *temp_r4_14;
    s32 *temp_r4_15;
    s32 *temp_r4_16;
    s32 *temp_r4_17;
    s32 *temp_r4_18;
    s32 *temp_r4_19;
    s32 *temp_r4_20;
    s32 *temp_r4_21;
    s32 *temp_r4_2;
    s32 *temp_r4_3;
    s32 *temp_r4_4;
    s32 *temp_r4_5;
    s32 *temp_r4_6;
    s32 *temp_r4_7;
    s32 *temp_r4_8;
    s32 *temp_r4_9;
    s32 *temp_r5;
    s32 *temp_r5_10;
    s32 *temp_r5_11;
    s32 *temp_r5_12;
    s32 *temp_r5_13;
    s32 *temp_r5_14;
    s32 *temp_r5_2;
    s32 *temp_r5_3;
    s32 *temp_r5_4;
    s32 *temp_r5_5;
    s32 *temp_r5_6;
    s32 *temp_r5_7;
    s32 *temp_r5_8;
    s32 *temp_r5_9;
    s32 *temp_r6;
    s32 temp_r28;
    s32 temp_r3_3;
    s32 temp_r3_4;
    s32 temp_r3_7;
    s32 var_r24;
    s32 var_r25;
    s32 var_r26;
    s32 var_r30;
    s32 var_r30_2;
    s32 var_r31;
    s32 var_r31_2;
    s32 var_r31_3;
    u32 temp_r31;
    u32 temp_r3_2;

    temp_r28 = game->boardSize;
    while (1) {
        game->cnt1C8 = 0;
        game->cnt1CC = 0;
        game->cnt1D0 = 0;
        fn_1_D7BC(game);
        fn_1_DC9C(game);
        fn_1_E17C(game);
        var_r26 = game->cnt1C8;
        var_r25 = game->cnt1CC;
        var_r24 = game->cnt1D0;
        var_r31 = 0;
        while (var_r31 < 4) {
            if ((s32) (lbl_1_bss_8C4[lbl_1_bss_3AC[var_r31]][lbl_1_bss_39C[var_r31]] & 0x100) != 0) {
                var_r26 -= 1;
            } else if ((s32) (lbl_1_bss_8C4[lbl_1_bss_3AC[var_r31]][lbl_1_bss_39C[var_r31]] & 0x200) != 0) {
                var_r25 -= 1;
            } else if ((s32) (lbl_1_bss_8C4[lbl_1_bss_3AC[var_r31]][lbl_1_bss_39C[var_r31]] & 0x400) != 0) {
                var_r24 -= 1;
            }
            var_r31 += 1;
        }
        temp_r5 = &lbl_1_bss_8C4[lbl_1_bss_3AC[0]][lbl_1_bss_39C[0]];
        *temp_r5 = (s32) (u8) *temp_r5;
        temp_r4 = &lbl_1_bss_8C4[lbl_1_bss_3AC[1]][lbl_1_bss_39C[1]];
        *temp_r4 = (s32) (u8) *temp_r4;
        temp_r4_2 = &lbl_1_bss_8C4[lbl_1_bss_3AC[2]][lbl_1_bss_39C[2]];
        *temp_r4_2 = (s32) (u8) *temp_r4_2;
        temp_r4_3 = &lbl_1_bss_8C4[lbl_1_bss_3AC[3]][lbl_1_bss_39C[3]];
        *temp_r4_3 = (s32) (u8) *temp_r4_3;
        if (var_r26 >= temp_r28 && var_r25 >= temp_r28 && var_r24 >= temp_r28) {
            break;
        }
        var_r31_2 = 0;
        while (var_r31_2 < 0xA) {
            var_r30 = 0;
            while (var_r30 < 0xA) {
                temp_r3 = &lbl_1_bss_8C4[var_r31_2][var_r30];
                *temp_r3 = (s32) (u8) *temp_r3;
                var_r30 += 1;
            }
            var_r31_2 += 1;
        }
    }
    do {
        temp_r31 = frandmod((u32) temp_r28);
        temp_r3_2 = frandmod((u32) temp_r28);
        if (((s32) (lbl_1_bss_8C4[temp_r31][temp_r3_2] & 0x100) != 0) && (var_r26 > temp_r28)) {
            temp_r5_2 = &lbl_1_bss_8C4[temp_r31][temp_r3_2];
            *temp_r5_2 = (s32) (u8) *temp_r5_2;
            var_r26 -= 1;
        }
        if (((s32) (lbl_1_bss_8C4[temp_r31][temp_r3_2] & 0x200) != 0) && (var_r25 > temp_r28)) {
            temp_r5_3 = &lbl_1_bss_8C4[temp_r31][temp_r3_2];
            *temp_r5_3 = (s32) (u8) *temp_r5_3;
            var_r25 -= 1;
        }
        if (((s32) (lbl_1_bss_8C4[temp_r31][temp_r3_2] & 0x400) != 0) && (var_r24 > temp_r28)) {
            temp_r4_4 = &lbl_1_bss_8C4[temp_r31][temp_r3_2];
            *temp_r4_4 = (s32) (u8) *temp_r4_4;
            var_r24 -= 1;
        }
    } while (var_r26 != temp_r28 || var_r25 != temp_r28 || var_r24 != temp_r28);
    game->cnt1C8 = var_r26;
    game->cnt1CC = var_r25;
    game->cnt1D0 = var_r24;
    fn_1_C614(temp_r28);
    var_r31_3 = 0;
    while (var_r31_3 < temp_r28) {
        var_r30_2 = 0;
        while (var_r30_2 < temp_r28) {
            if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_2] & 0x100) != 0) {
                temp_r3_3 = frandmod(3U);
                if (temp_r3_3 == 0) {
                    if ((s32) lbl_1_bss_3D8[0] > 0) {
                        temp_r4_5 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r4_5 |= 0x1000;
                        lbl_1_bss_3D8[0] -= 1;
                    } else if ((s32) lbl_1_bss_3D8[1] > 0) {
                        temp_r4_6 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r4_6 |= 0x2000;
                        lbl_1_bss_3D8[1] -= 1;
                    } else {
                        temp_r4_7 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r4_7 |= 0x4000;
                        lbl_1_bss_3D8[2] -= 1;
                    }
                } else if (temp_r3_3 == 1) {
                    if ((s32) lbl_1_bss_3D8[1] > 0) {
                        temp_r5_4 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r5_4 |= 0x2000;
                        lbl_1_bss_3D8[1] -= 1;
                    } else if ((s32) lbl_1_bss_3D8[0] > 0) {
                        temp_r4_8 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r4_8 |= 0x1000;
                        lbl_1_bss_3D8[0] -= 1;
                    } else {
                        temp_r4_9 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r4_9 |= 0x4000;
                        lbl_1_bss_3D8[2] -= 1;
                    }
                } else if ((s32) lbl_1_bss_3D8[2] > 0) {
                    temp_r4_10 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                    *temp_r4_10 |= 0x4000;
                    lbl_1_bss_3D8[2] -= 1;
                } else if ((s32) lbl_1_bss_3D8[0] > 0) {
                    temp_r4_11 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                    *temp_r4_11 |= 0x1000;
                    lbl_1_bss_3D8[0] -= 1;
                } else {
                    temp_r5_5 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                    *temp_r5_5 |= 0x2000;
                    lbl_1_bss_3D8[1] -= 1;
                }
            } else if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_2] & 0x200) != 0) {
                temp_r3_4 = frandmod(4U);
                switch (temp_r3_4) {                /* irregular */
                case 0:
                    if ((s32) lbl_1_bss_3C8[0] > 0) {
                        temp_r5_6 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r5_6 |= 0x1000;
                        lbl_1_bss_3C8[0] -= 1;
                    } else if ((s32) lbl_1_bss_3C8[1] > 0) {
                        temp_r5_7 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r5_7 |= 0x2000;
                        lbl_1_bss_3C8[1] -= 1;
                    } else if ((s32) lbl_1_bss_3C8[2] > 0) {
                        temp_r3_5 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r3_5 |= 0x4000;
                        lbl_1_bss_3C8[2] -= 1;
                    } else {
                        temp_r4_12 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r4_12 |= 0x8000;
                        lbl_1_bss_3C8[3] -= 1;
                    }
                    break;
                case 1:
                    if ((s32) lbl_1_bss_3C8[1] > 0) {
                        temp_r4_13 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r4_13 |= 0x2000;
                        lbl_1_bss_3C8[1] -= 1;
                    } else if ((s32) lbl_1_bss_3C8[0] > 0) {
                        temp_r4_14 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r4_14 |= 0x1000;
                        lbl_1_bss_3C8[0] -= 1;
                    } else if ((s32) lbl_1_bss_3C8[2] > 0) {
                        temp_r4_15 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r4_15 |= 0x4000;
                        lbl_1_bss_3C8[2] -= 1;
                    } else {
                        temp_r4_16 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r4_16 |= 0x8000;
                        lbl_1_bss_3C8[3] -= 1;
                    }
                    break;
                case 2:
                    if ((s32) lbl_1_bss_3C8[2] > 0) {
                        temp_r5_8 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r5_8 |= 0x4000;
                        lbl_1_bss_3C8[2] -= 1;
                    } else if ((s32) lbl_1_bss_3C8[0] > 0) {
                        temp_r5_9 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r5_9 |= 0x1000;
                        lbl_1_bss_3C8[0] -= 1;
                    } else if ((s32) lbl_1_bss_3C8[1] > 0) {
                        temp_r4_17 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r4_17 |= 0x2000;
                        lbl_1_bss_3C8[1] -= 1;
                    } else {
                        temp_r3_6 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r3_6 |= 0x8000;
                        lbl_1_bss_3C8[3] -= 1;
                    }
                    break;
                default:
                    if ((s32) lbl_1_bss_3C8[3] > 0) {
                        temp_r5_10 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r5_10 |= 0x8000;
                        lbl_1_bss_3C8[3] -= 1;
                    } else if ((s32) lbl_1_bss_3C8[0] > 0) {
                        temp_r4_18 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r4_18 |= 0x1000;
                        lbl_1_bss_3C8[0] -= 1;
                    } else if ((s32) lbl_1_bss_3C8[1] > 0) {
                        temp_r5_11 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r5_11 |= 0x2000;
                        lbl_1_bss_3C8[1] -= 1;
                    } else {
                        temp_r4_19 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r4_19 |= 0x4000;
                        lbl_1_bss_3C8[2] -= 1;
                    }
                    break;
                }
            } else if ((s32) (lbl_1_bss_8C4[var_r31_3][var_r30_2] & 0x400) != 0) {
                temp_r3_7 = frandmod(3U);
                if (temp_r3_7 == 0) {
                    if ((s32) lbl_1_bss_3BC[0] > 0) {
                        temp_r4_20 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r4_20 |= 0x1000;
                        lbl_1_bss_3BC[0] -= 1;
                    } else if ((s32) lbl_1_bss_3BC[1] > 0) {
                        temp_r3_8 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r3_8 |= 0x2000;
                        lbl_1_bss_3BC[1] -= 1;
                    } else {
                        temp_r5_12 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r5_12 |= 0x4000;
                        lbl_1_bss_3BC[2] -= 1;
                    }
                } else if (temp_r3_7 == 1) {
                    if ((s32) lbl_1_bss_3BC[1] > 0) {
                        temp_r3_9 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r3_9 |= 0x2000;
                        lbl_1_bss_3BC[1] -= 1;
                    } else if ((s32) lbl_1_bss_3BC[0] > 0) {
                        temp_r3_10 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r3_10 |= 0x1000;
                        lbl_1_bss_3BC[0] -= 1;
                    } else {
                        temp_r4_21 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                        *temp_r4_21 |= 0x4000;
                        lbl_1_bss_3BC[2] -= 1;
                    }
                } else if ((s32) lbl_1_bss_3BC[2] > 0) {
                    temp_r5_13 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                    *temp_r5_13 |= 0x4000;
                    lbl_1_bss_3BC[2] -= 1;
                } else if ((s32) lbl_1_bss_3BC[0] > 0) {
                    temp_r5_14 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                    *temp_r5_14 |= 0x1000;
                    lbl_1_bss_3BC[0] -= 1;
                } else {
                    temp_r6 = &lbl_1_bss_8C4[var_r31_3][var_r30_2];
                    *temp_r6 |= 0x2000;
                    lbl_1_bss_3BC[1] -= 1;
                }
            }
            var_r30_2 += 1;
        }
        var_r31_3 += 1;
    }
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_D7BC(CardGame *game) {
    s32 *temp_r3_10;
    s32 *temp_r3_11;
    s32 *temp_r3_12;
    s32 *temp_r3_13;
    s32 *temp_r3_14;
    s32 *temp_r3_15;
    s32 *temp_r3_16;
    s32 *temp_r3_17;
    s32 *temp_r3_2;
    s32 *temp_r3_3;
    s32 *temp_r3_4;
    s32 *temp_r3_5;
    s32 *temp_r3_6;
    s32 *temp_r3_7;
    s32 *temp_r3_8;
    s32 *temp_r3_9;
    s32 temp_r28;
    s32 temp_r3;
    s32 var_r30;
    s32 var_r30_2;

    temp_r28 = game->boardSize;
    var_r30 = 1;
    while (var_r30 < (s32) (temp_r28 - 1)) {
        temp_r3 = frandmod(3U);
        if (temp_r3 == 0) {
            if ((s32) (lbl_1_bss_8C4[0][var_r30 - 1] & 0x100) == 0) {
                temp_r3_2 = &lbl_1_bss_8C4[0][var_r30];
                *temp_r3_2 |= 0x100;
                game->cnt1C8 += 1;
            } else if (frandmod(2U) != 0) {
                if ((s32) game->cnt1CC < temp_r28) {
                    temp_r3_3 = &lbl_1_bss_8C4[0][var_r30];
                    *temp_r3_3 |= 0x200;
                    game->cnt1CC += 1;
                } else {
                    temp_r3_4 = &lbl_1_bss_8C4[0][var_r30];
                    *temp_r3_4 |= 0x400;
                    game->cnt1D0 += 1;
                }
            } else if ((s32) game->cnt1D0 < temp_r28) {
                temp_r3_5 = &lbl_1_bss_8C4[0][var_r30];
                *temp_r3_5 |= 0x400;
                game->cnt1D0 += 1;
            } else {
                temp_r3_6 = &lbl_1_bss_8C4[0][var_r30];
                *temp_r3_6 |= 0x200;
                game->cnt1CC += 1;
            }
        } else if (temp_r3 == 1) {
            if ((s32) (lbl_1_bss_8C4[0][var_r30 - 1] & 0x200) == 0) {
                temp_r3_7 = &lbl_1_bss_8C4[0][var_r30];
                *temp_r3_7 |= 0x200;
                game->cnt1CC += 1;
            } else if (frandmod(2U) != 0) {
                if ((s32) game->cnt1C8 < temp_r28) {
                    temp_r3_8 = &lbl_1_bss_8C4[0][var_r30];
                    *temp_r3_8 |= 0x100;
                    game->cnt1C8 += 1;
                } else {
                    temp_r3_9 = &lbl_1_bss_8C4[0][var_r30];
                    *temp_r3_9 |= 0x400;
                    game->cnt1D0 += 1;
                }
            } else if ((s32) game->cnt1D0 < temp_r28) {
                temp_r3_10 = &lbl_1_bss_8C4[0][var_r30];
                *temp_r3_10 |= 0x400;
                game->cnt1D0 += 1;
            } else {
                temp_r3_11 = &lbl_1_bss_8C4[0][var_r30];
                *temp_r3_11 |= 0x100;
                game->cnt1C8 += 1;
            }
        } else if ((s32) (lbl_1_bss_8C4[0][var_r30 - 1] & 0x400) == 0) {
            temp_r3_12 = &lbl_1_bss_8C4[0][var_r30];
            *temp_r3_12 |= 0x400;
            game->cnt1D0 += 1;
        } else if (frandmod(2U) != 0) {
            if ((s32) game->cnt1C8 < temp_r28) {
                temp_r3_13 = &lbl_1_bss_8C4[0][var_r30];
                *temp_r3_13 |= 0x100;
                game->cnt1C8 += 1;
            } else {
                temp_r3_14 = &lbl_1_bss_8C4[0][var_r30];
                *temp_r3_14 |= 0x200;
                game->cnt1CC += 1;
            }
        } else if ((s32) game->cnt1CC < temp_r28) {
            temp_r3_15 = &lbl_1_bss_8C4[0][var_r30];
            *temp_r3_15 |= 0x200;
            game->cnt1CC += 1;
        } else {
            temp_r3_16 = &lbl_1_bss_8C4[0][var_r30];
            *temp_r3_16 |= 0x100;
            game->cnt1C8 += 1;
        }
        var_r30 += 1;
    }
    var_r30_2 = 1;
    while (var_r30_2 < (s32) (temp_r28 - 1)) {
        if ((s32) (lbl_1_bss_8C4[0][var_r30_2] & 0x400) != 0) {
            temp_r3_17 = &lbl_1_bss_8C4[0][var_r30_2];
            *temp_r3_17 &= 0xFFFFFBFF;
        }
        var_r30_2 += 1;
    }
    game->cnt1D0 = 0;
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_DC9C(CardGame *game) {
    s32 *temp_r3_10;
    s32 *temp_r3_11;
    s32 *temp_r3_12;
    s32 *temp_r3_13;
    s32 *temp_r3_14;
    s32 *temp_r3_15;
    s32 *temp_r3_16;
    s32 *temp_r3_17;
    s32 *temp_r3_2;
    s32 *temp_r3_3;
    s32 *temp_r3_4;
    s32 *temp_r3_5;
    s32 *temp_r3_6;
    s32 *temp_r3_7;
    s32 *temp_r3_8;
    s32 *temp_r3_9;
    s32 temp_r28;
    s32 temp_r3;
    s32 var_r30;
    s32 var_r30_2;

    temp_r28 = game->boardSize;
    var_r30 = 1;
    while (var_r30 < (s32) (temp_r28 - 1)) {
        temp_r3 = frandmod(3U);
        if (temp_r3 == 0) {
            if ((s32) (*lbl_1_bss_8C4[var_r30 - 1] & 0x100) == 0) {
                temp_r3_2 = lbl_1_bss_8C4[var_r30];
                *temp_r3_2 |= 0x100;
                game->cnt1C8 += 1;
            } else if (frandmod(2U) != 0) {
                if ((s32) game->cnt1CC < temp_r28) {
                    temp_r3_3 = lbl_1_bss_8C4[var_r30];
                    *temp_r3_3 |= 0x200;
                    game->cnt1CC += 1;
                } else {
                    temp_r3_4 = lbl_1_bss_8C4[var_r30];
                    *temp_r3_4 |= 0x400;
                    game->cnt1D0 += 1;
                }
            } else if ((s32) game->cnt1D0 < temp_r28) {
                temp_r3_5 = lbl_1_bss_8C4[var_r30];
                *temp_r3_5 |= 0x400;
                game->cnt1D0 += 1;
            } else {
                temp_r3_6 = lbl_1_bss_8C4[var_r30];
                *temp_r3_6 |= 0x200;
                game->cnt1CC += 1;
            }
        } else if (temp_r3 == 1) {
            if ((s32) (*lbl_1_bss_8C4[var_r30 - 1] & 0x200) == 0) {
                temp_r3_7 = lbl_1_bss_8C4[var_r30];
                *temp_r3_7 |= 0x200;
                game->cnt1CC += 1;
            } else if (frandmod(2U) != 0) {
                if ((s32) game->cnt1C8 < temp_r28) {
                    temp_r3_8 = lbl_1_bss_8C4[var_r30];
                    *temp_r3_8 |= 0x100;
                    game->cnt1C8 += 1;
                } else {
                    temp_r3_9 = lbl_1_bss_8C4[var_r30];
                    *temp_r3_9 |= 0x400;
                    game->cnt1D0 += 1;
                }
            } else if ((s32) game->cnt1D0 < temp_r28) {
                temp_r3_10 = lbl_1_bss_8C4[var_r30];
                *temp_r3_10 |= 0x400;
                game->cnt1D0 += 1;
            } else {
                temp_r3_11 = lbl_1_bss_8C4[var_r30];
                *temp_r3_11 |= 0x100;
                game->cnt1C8 += 1;
            }
        } else if ((s32) (*lbl_1_bss_8C4[var_r30 - 1] & 0x400) == 0) {
            temp_r3_12 = lbl_1_bss_8C4[var_r30];
            *temp_r3_12 |= 0x400;
            game->cnt1D0 += 1;
        } else if (frandmod(2U) != 0) {
            if ((s32) game->cnt1C8 < temp_r28) {
                temp_r3_13 = lbl_1_bss_8C4[var_r30];
                *temp_r3_13 |= 0x100;
                game->cnt1C8 += 1;
            } else {
                temp_r3_14 = lbl_1_bss_8C4[var_r30];
                *temp_r3_14 |= 0x200;
                game->cnt1CC += 1;
            }
        } else if ((s32) game->cnt1CC < temp_r28) {
            temp_r3_15 = lbl_1_bss_8C4[var_r30];
            *temp_r3_15 |= 0x200;
            game->cnt1CC += 1;
        } else {
            temp_r3_16 = lbl_1_bss_8C4[var_r30];
            *temp_r3_16 |= 0x100;
            game->cnt1C8 += 1;
        }
        var_r30 += 1;
    }
    var_r30_2 = 1;
    while (var_r30_2 < (s32) (temp_r28 - 1)) {
        if ((s32) (*lbl_1_bss_8C4[var_r30_2] & 0x400) != 0) {
            temp_r3_17 = lbl_1_bss_8C4[var_r30_2];
            *temp_r3_17 &= 0xFFFFFBFF;
        }
        var_r30_2 += 1;
    }
    game->cnt1D0 = 0;
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_E17C(CardGame *game) {
    s32 *temp_r3_10;
    s32 *temp_r3_11;
    s32 *temp_r3_12;
    s32 *temp_r3_13;
    s32 *temp_r3_14;
    s32 *temp_r3_15;
    s32 *temp_r3_16;
    s32 *temp_r3_17;
    s32 *temp_r3_18;
    s32 *temp_r3_2;
    s32 *temp_r3_3;
    s32 *temp_r3_4;
    s32 *temp_r3_5;
    s32 *temp_r3_6;
    s32 *temp_r3_7;
    s32 *temp_r3_8;
    s32 *temp_r3_9;
    s32 *temp_r4;
    s32 *temp_r4_10;
    s32 *temp_r4_11;
    s32 *temp_r4_2;
    s32 *temp_r4_3;
    s32 *temp_r4_4;
    s32 *temp_r4_5;
    s32 *temp_r4_6;
    s32 *temp_r4_7;
    s32 *temp_r4_8;
    s32 *temp_r4_9;
    s32 temp_r23;
    s32 temp_r24;
    s32 temp_r28;
    s32 temp_r3;
    s32 var_r29;
    s32 var_r29_2;
    s32 var_r30;
    s32 var_r30_2;

    temp_r28 = game->boardSize;
    temp_r24 = temp_r28;
    temp_r23 = temp_r28;
    var_r30 = 1;
    while (var_r30 < temp_r24) {
        var_r29 = 1;
        while (var_r29 < temp_r23 && ((var_r30 != (s32) (temp_r28 - 1)) || (var_r29 != (s32) (temp_r28 - 1)))) {
            temp_r3 = frandmod(3U);
            if (temp_r3 == 0) {
                if ((s32) (lbl_1_bss_8C4[var_r30 - 1][var_r29] & 0x100) == 0) {
                    if ((s32) (lbl_1_bss_8C4[var_r30][var_r29 - 1] & 0x100) == 0) {
                        temp_r3_2 = &lbl_1_bss_8C4[var_r30][var_r29];
                        *temp_r3_2 |= 0x100;
                        game->cnt1C8 += 1;
                    } else if ((s32) (lbl_1_bss_8C4[var_r30 - 1][var_r29] & 0x200) != 0) {
                        temp_r3_3 = &lbl_1_bss_8C4[var_r30][var_r29];
                        *temp_r3_3 |= 0x400;
                        game->cnt1D0 += 1;
                    } else {
                        temp_r4 = &lbl_1_bss_8C4[var_r30][var_r29];
                        *temp_r4 |= 0x200;
                        game->cnt1CC += 1;
                    }
                } else if ((s32) (lbl_1_bss_8C4[var_r30][var_r29 - 1] & 0x100) != 0) {
                    if (frandmod(2U) != 0) {
                        if ((s32) game->cnt1CC < temp_r28) {
                            temp_r4_2 = &lbl_1_bss_8C4[var_r30][var_r29];
                            *temp_r4_2 |= 0x200;
                            game->cnt1CC += 1;
                        } else {
                            temp_r3_4 = &lbl_1_bss_8C4[var_r30][var_r29];
                            *temp_r3_4 |= 0x400;
                            game->cnt1D0 += 1;
                        }
                    } else if ((s32) game->cnt1D0 < temp_r28) {
                        temp_r3_5 = &lbl_1_bss_8C4[var_r30][var_r29];
                        *temp_r3_5 |= 0x400;
                        game->cnt1D0 += 1;
                    } else {
                        temp_r3_6 = &lbl_1_bss_8C4[var_r30][var_r29];
                        *temp_r3_6 |= 0x200;
                        game->cnt1CC += 1;
                    }
                } else if ((s32) (lbl_1_bss_8C4[var_r30][var_r29 - 1] & 0x200) != 0) {
                    temp_r4_3 = &lbl_1_bss_8C4[var_r30][var_r29];
                    *temp_r4_3 |= 0x400;
                    game->cnt1D0 += 1;
                } else {
                    temp_r4_4 = &lbl_1_bss_8C4[var_r30][var_r29];
                    *temp_r4_4 |= 0x200;
                    game->cnt1CC += 1;
                }
            } else if (temp_r3 == 1) {
                if ((s32) (lbl_1_bss_8C4[var_r30 - 1][var_r29] & 0x200) == 0) {
                    if ((s32) (lbl_1_bss_8C4[var_r30][var_r29 - 1] & 0x200) == 0) {
                        temp_r3_7 = &lbl_1_bss_8C4[var_r30][var_r29];
                        *temp_r3_7 |= 0x200;
                        game->cnt1CC += 1;
                    } else if ((s32) (lbl_1_bss_8C4[var_r30 - 1][var_r29] & 0x100) != 0) {
                        temp_r3_8 = &lbl_1_bss_8C4[var_r30][var_r29];
                        *temp_r3_8 |= 0x400;
                        game->cnt1D0 += 1;
                    } else {
                        temp_r3_9 = &lbl_1_bss_8C4[var_r30][var_r29];
                        *temp_r3_9 |= 0x100;
                        game->cnt1C8 += 1;
                    }
                } else if ((s32) (lbl_1_bss_8C4[var_r30][var_r29 - 1] & 0x200) != 0) {
                    if (frandmod(2U) != 0) {
                        if ((s32) game->cnt1C8 < temp_r28) {
                            temp_r3_10 = &lbl_1_bss_8C4[var_r30][var_r29];
                            *temp_r3_10 |= 0x100;
                            game->cnt1C8 += 1;
                        } else {
                            temp_r3_11 = &lbl_1_bss_8C4[var_r30][var_r29];
                            *temp_r3_11 |= 0x400;
                            game->cnt1D0 += 1;
                        }
                    } else if ((s32) game->cnt1D0 < temp_r28) {
                        temp_r4_5 = &lbl_1_bss_8C4[var_r30][var_r29];
                        *temp_r4_5 |= 0x400;
                        game->cnt1D0 += 1;
                    } else {
                        temp_r3_12 = &lbl_1_bss_8C4[var_r30][var_r29];
                        *temp_r3_12 |= 0x100;
                        game->cnt1C8 += 1;
                    }
                } else if ((s32) (lbl_1_bss_8C4[var_r30][var_r29 - 1] & 0x100) != 0) {
                    temp_r3_13 = &lbl_1_bss_8C4[var_r30][var_r29];
                    *temp_r3_13 |= 0x400;
                    game->cnt1D0 += 1;
                } else {
                    temp_r4_6 = &lbl_1_bss_8C4[var_r30][var_r29];
                    *temp_r4_6 |= 0x100;
                    game->cnt1C8 += 1;
                }
            } else if ((s32) (lbl_1_bss_8C4[var_r30 - 1][var_r29] & 0x400) == 0) {
                if ((s32) (lbl_1_bss_8C4[var_r30][var_r29 - 1] & 0x400) == 0) {
                    temp_r3_14 = &lbl_1_bss_8C4[var_r30][var_r29];
                    *temp_r3_14 |= 0x400;
                    game->cnt1D0 += 1;
                } else if ((s32) (lbl_1_bss_8C4[var_r30 - 1][var_r29] & 0x100) != 0) {
                    temp_r4_7 = &lbl_1_bss_8C4[var_r30][var_r29];
                    *temp_r4_7 |= 0x200;
                    game->cnt1CC += 1;
                } else {
                    temp_r4_8 = &lbl_1_bss_8C4[var_r30][var_r29];
                    *temp_r4_8 |= 0x100;
                    game->cnt1C8 += 1;
                }
            } else if ((s32) (lbl_1_bss_8C4[var_r30][var_r29 - 1] & 0x400) != 0) {
                if (frandmod(2U) != 0) {
                    if ((s32) game->cnt1C8 < temp_r28) {
                        temp_r3_15 = &lbl_1_bss_8C4[var_r30][var_r29];
                        *temp_r3_15 |= 0x100;
                        game->cnt1C8 += 1;
                    } else {
                        temp_r3_16 = &lbl_1_bss_8C4[var_r30][var_r29];
                        *temp_r3_16 |= 0x200;
                        game->cnt1CC += 1;
                    }
                } else if ((s32) game->cnt1CC < temp_r28) {
                    temp_r4_9 = &lbl_1_bss_8C4[var_r30][var_r29];
                    *temp_r4_9 |= 0x200;
                    game->cnt1CC += 1;
                } else {
                    temp_r3_17 = &lbl_1_bss_8C4[var_r30][var_r29];
                    *temp_r3_17 |= 0x100;
                    game->cnt1C8 += 1;
                }
            } else if ((s32) (lbl_1_bss_8C4[var_r30][var_r29 - 1] & 0x100) != 0) {
                temp_r3_18 = &lbl_1_bss_8C4[var_r30][var_r29];
                *temp_r3_18 |= 0x200;
                game->cnt1CC += 1;
            } else {
                temp_r4_10 = &lbl_1_bss_8C4[var_r30][var_r29];
                *temp_r4_10 |= 0x100;
                game->cnt1C8 += 1;
            }
            var_r29 += 1;
        }
        var_r30 += 1;
    }
    var_r30_2 = 1;
    while (var_r30_2 < temp_r24) {
        var_r29_2 = 1;
        while (var_r29_2 < temp_r23) {
            if (((var_r30_2 == (s32) (temp_r28 - 1)) || (var_r29_2 == (s32) (temp_r28 - 1))) && ((s32) (lbl_1_bss_8C4[var_r30_2][var_r29_2] & 0x400) != 0)) {
                temp_r4_11 = &lbl_1_bss_8C4[var_r30_2][var_r29_2];
                *temp_r4_11 &= 0xFFFFFBFF;
                game->cnt1D0 -= 1;
            }
            var_r29_2 += 1;
        }
        var_r30_2 += 1;
    }
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
s32 fn_1_EB38(s32 row, s32 col) {
    char sp14[4][15] = { "S3_c_ura_item", "S3_c_ura_event", "c_ura_star", "S3_c_ura_r" };
    s32 sp10;
    s32 spC;
    s32 sp8;
    s32 var_r17;
    s32 var_r18;
    s32 var_r19;
    s32 var_r20;
    s32 var_r21;
    s32 var_r22;
    s32 var_r23;
    s32 var_r24;
    s32 var_r25;
    s32 var_r26;
    s32 var_r27;
    s32 var_r28;
    s32 var_r29;

    var_r29 = 0;
    if ((s32) lbl_1_bss_398 == 0) {
        if ((s32) (lbl_1_bss_8C4[row][col] & 0xF) == 0xF) {
            var_r28 = 0;
        } else {
            if ((s32) (lbl_1_bss_8C4[row][col] & 0xF) == 0xC) {
                var_r27 = 1;
            } else {
                if ((s32) (lbl_1_bss_8C4[row][col] & 0xF) == 3) {
                    var_r26 = 2;
                } else {
                    if ((s32) (lbl_1_bss_8C4[row][col] & 0xF) == 0xB) {
                        var_r25 = 3;
                    } else {
                        if ((s32) (lbl_1_bss_8C4[row][col] & 0xF) == 0xD) {
                            var_r24 = 4;
                        } else {
                            if ((s32) (lbl_1_bss_8C4[row][col] & 0xF) == 7) {
                                var_r23 = 5;
                            } else {
                                if ((s32) (lbl_1_bss_8C4[row][col] & 0xF) == 0xE) {
                                    var_r22 = 6;
                                } else {
                                    if ((s32) (lbl_1_bss_8C4[row][col] & 0xF) == 0xA) {
                                        var_r21 = 7;
                                    } else {
                                        if ((s32) (lbl_1_bss_8C4[row][col] & 0xF) == 9) {
                                            var_r20 = 8;
                                        } else {
                                            if ((s32) (lbl_1_bss_8C4[row][col] & 0xF) == 5) {
                                                var_r19 = 9;
                                            } else {
                                                var_r19 = 0xA;
                                            }
                                            var_r20 = var_r19;
                                        }
                                        var_r21 = var_r20;
                                    }
                                    var_r22 = var_r21;
                                }
                                var_r23 = var_r22;
                            }
                            var_r24 = var_r23;
                        }
                        var_r25 = var_r24;
                    }
                    var_r26 = var_r25;
                }
                var_r27 = var_r26;
            }
            var_r28 = var_r27;
        }
        sp10 = var_r28;
        if ((s32) (lbl_1_bss_8C4[row][col] & 0x100) != 0) {
            var_r18 = 0;
        } else {
            if ((s32) (lbl_1_bss_8C4[row][col] & 0x200) != 0) {
                var_r17 = 1;
            } else {
                if ((s32) (lbl_1_bss_8C4[row][col] & 0x400) != 0) {
                    sp8 = 2;
                } else {
                    sp8 = 3;
                }
                var_r17 = sp8;
            }
            var_r18 = var_r17;
        }
        spC = var_r18;
        lbl_1_bss_330[lbl_1_bss_394] = Hu3DAnimCreate(HuDataReadNum(lbl_1_data_B0[sp10 + 0x5B], 0x10000000), lbl_1_bss_478[row][col], sp14[spC]);
        lbl_1_bss_394 += 1;
        lbl_1_bss_398 = 1;
        fn_1_F2A8(0, 0, 0);
    } else if (fn_1_F2A8(2, row, col) != 0) {
        lbl_1_bss_398 = 0;
        var_r29 = 1;
    }
    return var_r29;
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_F104(s32 row, s32 col) {
    s32 spC;
    s32 sp8;

    sp8 = row;
    spC = col;
    lbl_1_bss_330[lbl_1_bss_394] = Hu3DAnimCreate(HuDataReadNum(lbl_1_data_B0[0x20], 0x10000000), lbl_1_bss_478[sp8][spC], "S3_c029");
    lbl_1_bss_394 += 1;
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_F1A8(s32 row, s32 col) {
    s32 spC;
    s32 sp8;

    sp8 = row;
    spC = col;
    lbl_1_bss_330[lbl_1_bss_394] = Hu3DAnimCreate(HuDataReadNum(lbl_1_data_B0[0x20], 0x10000000), lbl_1_bss_478[sp8][spC], "S3_c028");
    lbl_1_bss_394 += 1;
}

void fn_1_F24C(void) {
    s32 var_r31;

    var_r31 = 0;
    while (var_r31 < (s32) lbl_1_bss_394) {
        Hu3DAnimKill(lbl_1_bss_330[var_r31]);
        var_r31 += 1;
    }
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
s32 fn_1_F2A8(s32 mode, s32 row, s32 col) {
    f32 sp20;
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    s16 sp8;
    f32 temp_f0;
    s32 *temp_r4;
    s32 var_r29;

    var_r29 = 0;
    if (mode == 0) {
        lbl_1_bss_32C = 1;
        lbl_1_bss_324 = 0.0f;
        lbl_1_bss_320 = 0;
        var_r29 = 1;
    } else if ((s32) lbl_1_bss_32C == 0) {
        if (mode == 1) {
            lbl_1_bss_328 -= 10.0f;
            if (lbl_1_bss_328 < 0.0f) {
                lbl_1_bss_328 = 180.0f;
                lbl_1_bss_32C = 2;
            } else {
                Hu3DModelRotSet(lbl_1_bss_478[row][col], 0.0f, 0.0f, lbl_1_bss_328);
            }
        } else {
            lbl_1_bss_328 += 10.0f;
            if (lbl_1_bss_328 > 180.0f) {
                lbl_1_bss_328 = 0.0f;
                lbl_1_bss_32C = 2;
            } else {
                Hu3DModelRotSet(lbl_1_bss_478[row][col], 0.0f, 0.0f, -lbl_1_bss_328);
            }
        }
    } else {
        temp_f0 = lbl_1_bss_3E4.x + (300.0f * (f32) col);
        sp18 = temp_f0;
        sp1C = lbl_1_bss_3E4.y;
        sp20 = lbl_1_bss_3E4.z + (300.0f * (f32) row);
        Hu3DModelPosSet(lbl_1_bss_478[row][col], -50.0f + temp_f0, sp1C + lbl_1_bss_324, sp20);
        if ((s32) lbl_1_bss_32C == 1) {
            lbl_1_bss_324 += 5.0f;
            if (lbl_1_bss_324 > 100.0f) {
                lbl_1_bss_324 = 100.0f;
                lbl_1_bss_32C = 0;
                if (mode == 1) {
                    lbl_1_bss_328 = 180.0f;
                } else {
                    lbl_1_bss_328 = 0.0f;
                }
                if (((s32) (lbl_1_bss_8C4[row][col] & 0x400) != 0) && ((s32) (lbl_1_bss_8C4[row][col] & 0x10) == 0)) {
                    HuAudSStreamVolSet(0, 0U, 0x3E8U);
                    if ((s32) (lbl_1_bss_8C4[row][col] & 0x1000) != 0) {
                        fn_1_8D04(0x402, lbl_1_bss_478[row][col], 1);
                    } else if ((s32) (lbl_1_bss_8C4[row][col] & 0x2000) != 0) {
                        fn_1_8D04(0x403, lbl_1_bss_478[row][col], 1);
                    } else if ((s32) (lbl_1_bss_8C4[row][col] & 0x4000) != 0) {
                        fn_1_8D04(0x404, lbl_1_bss_478[row][col], 1);
                    }
                } else if (((s32) (lbl_1_bss_8C4[row][col] & 0x200) != 0) && ((s32) (lbl_1_bss_8C4[row][col] & 0x10) == 0)) {
                    if ((s32) (lbl_1_bss_8C4[row][col] & 0xC000) != 0) {
                        fn_1_8D04(0x451, lbl_1_bss_478[row][col], 1);
                    } else {
                        fn_1_8D04(0x3F6, lbl_1_bss_478[row][col], 1);
                    }
                } else {
                    fn_1_8D04(0x3F6, lbl_1_bss_478[row][col], 1);
                }
            }
        } else {
            lbl_1_bss_324 -= 5.0f;
            if (lbl_1_bss_324 < 0.0f) {
                temp_r4 = &lbl_1_bss_8C4[row][col];
                *temp_r4 |= 0x10;
                lbl_1_bss_324 = 0.0f;
                var_r29 = 1;
            }
            if ((s32) lbl_1_bss_320 == 0) {
                if (lbl_1_bss_324 <= 5.0f) {
                    sp18 += -50.0f;
                    sp1C += 1.0f;
                    spC = sp18;
                    sp10 = sp1C;
                    sp14 = sp20;
                    sp8 = fn_1_3ED3C(0, (Point3d *) &spC);
                    lbl_1_bss_320 = 1;
                    fn_1_8D04(0x454, lbl_1_bss_478[row][col], 1);
                }
            }
        }
    }
    return var_r29;
}

