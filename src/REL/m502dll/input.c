#include "REL/m502dll.h"

/* computer player: pick a column for the next block drop */
void fn_1_58E0(M502AI *ai)
{
    M502INPUT *in;
    M502PLRSUB *plr;
    M502BLOCK *block;
    s16 lvl;
    s16 row;
    s16 col;
    s16 dif;
    s32 count[3] = { 0, 0, 0 };
    s32 cand[3];
    s32 i;
    s32 minCount;
    s32 nCand;
    s32 r;

    in = ai->input;
    plr = in->plr;
    block = &plr->player->block;
    dif = GwPlayerConf[plr->playerNo].dif;
    switch (ai->sub) {
    case 0:
        if (ai->flag == 0) {
            fn_1_6250(ai, &lvl, &row, &col);
            if (lvl < 3) {
                for (i = 0; i < 3; i++) {
                    if (block->cell[lvl][i].kind == 2) {
                        count[i]++;
                    }
                    if (block->cell[lvl][i + 3].kind == 2) {
                        count[i]++;
                    }
                    if (block->cell[lvl][i + 6].kind == 2) {
                        count[i]++;
                    }
                }
                minCount = 99999;
                for (i = 0; i < 3; i++) {
                    if (minCount > count[i]) {
                        minCount = count[i];
                    }
                }
                nCand = 0;
                for (i = 0; i < 3; i++) {
                    if (minCount == count[i]) {
                        cand[nCand] = i;
                        nCand++;
                    }
                }
                r = rand8() % 200;
                ai->target = 0;
                switch (nCand) {
                case 1:
                    ai->target = cand[0];
                    break;
                case 2:
                    if (plr->playerNo == 0) {
                        if (r <= 0x14) {
                            ai->target = cand[0];
                        } else {
                            ai->target = cand[1];
                        }
                    } else if (plr->playerNo == 1) {
                        if (r <= 0x82) {
                            ai->target = cand[0];
                        } else {
                            ai->target = cand[1];
                        }
                    } else if (plr->playerNo == 2) {
                        if (r <= 0x46) {
                            ai->target = cand[0];
                        } else {
                            ai->target = cand[1];
                        }
                    } else if (r <= 0xB4) {
                        ai->target = cand[0];
                    } else {
                        ai->target = cand[1];
                    }
                    break;
                case 3:
                    if (plr->playerNo == 0) {
                        if (r <= 0x42) {
                            ai->target = cand[0];
                        } else if (r <= 0x84) {
                            ai->target = cand[1];
                        } else {
                            ai->target = cand[2];
                        }
                    } else if (plr->playerNo == 1) {
                        if (r <= 0x14) {
                            ai->target = cand[0];
                        } else if (r <= 0x28) {
                            ai->target = cand[1];
                        } else {
                            ai->target = cand[2];
                        }
                    } else if (plr->playerNo == 2) {
                        if (r <= 0x14) {
                            ai->target = cand[0];
                        } else if (r <= 0xB4) {
                            ai->target = cand[1];
                        } else {
                            ai->target = cand[2];
                        }
                    } else if (r <= 0xA0) {
                        ai->target = cand[0];
                    } else if (r <= 0xB4) {
                        ai->target = cand[1];
                    } else {
                        ai->target = cand[2];
                    }
                    break;
                }
                r = rand8() % 100;
                switch (dif) {
                case 0:
                    if (r >= 10) {
                        ai->target = r % 3;
                    }
                    break;
                case 1:
                    if (r >= 40) {
                        ai->target = r % 3;
                    }
                    break;
                case 2:
                    if (r >= 100) {
                        ai->target = r % 3;
                    }
                    break;
                case 3:
                    if (r >= 100) {
                        ai->target = r % 3;
                    }
                    break;
                }
                if (plr->lane < ai->target) {
                    in->stkX = 1;
                }
                if (plr->lane > ai->target) {
                    in->stkX = -1;
                }
                ai->flag = 1;
                ai->sub++;
            }
        } else {
            ai->sub++;
        }
        return;
    case 1:
        if (plr->lane != ai->target) {
            if (plr->lane < ai->target) {
                plr->input.stkX = 1;
            }
            if (plr->lane > ai->target) {
                plr->input.stkX = -1;
            }
        }
        if (plr->action == 0) {
            ai->sub = 0;
            ai->step++;
        }
        break;
    }
}

/* computer player: wait, then press A */
void fn_1_5EB8(M502AI *ai)
{
    M502INPUT *in;
    M502PLRSUB *plr;
    M502BLOCK *block;
    s16 lvl;
    s16 row;
    s16 col;
    s16 dif;
    u32 r;
    s32 r2;

    in = ai->input;
    plr = in->plr;
    block = &plr->player->block;
    dif = GwPlayerConf[plr->playerNo].dif;
    switch (ai->sub) {
    case 0:
        r = (rand8() % 100) / 20;
        switch (dif) {
        case 0:
            ai->wait = r + 0xF;
            break;
        case 1:
            ai->wait = r + 0xB;
            break;
        case 2:
            ai->wait = r + 8;
            break;
        case 3:
            ai->wait = r + 4;
            break;
        }
        ai->sub++;
        ai->cnt = 0;
        return;
    case 1:
        ai->cnt++;
        if (ai->cnt >= ai->wait) {
            in->btnDown |= 0x100;
            in->btn |= 0x100;
            fn_1_6250(ai, &lvl, &row, &col);
            ai->wait = 7;
            ai->sub++;
            ai->cnt = 0;
            if (lvl < 3) {
                if (row > 0) {
                    ai->flag = 0;
                }
                r2 = rand8() % 100;
                switch (dif) {
                case 0:
                    ai->wait = 7;
                    return;
                case 1:
                    if (r2 < 50) {
                        ai->wait = 7;
                        return;
                    }
                    ai->wait = 0xF;
                    return;
                case 2:
                    if (block->cell[lvl][col + row * 3].kind == 2) {
                        if (r2 < 40) {
                            ai->wait = 0xF;
                            return;
                        }
                        ai->wait = 7;
                        return;
                    }
                    if (r2 < 40) {
                        ai->wait = 0xF;
                        return;
                    }
                    ai->wait = 0x1E;
                    return;
                case 3:
                    if (block->cell[lvl][col + row * 3].kind == 2) {
                        if (r2 < 10) {
                            ai->wait = 0xF;
                            return;
                        }
                        ai->wait = 7;
                        return;
                    }
                    if (r2 < 10) {
                        ai->wait = 0xF;
                        return;
                    }
                    ai->wait = 0x1E;
                    return;
                }
            }
        }
        return;
    case 2:
        if (ai->wait != 7) {
            in->btn |= 0x100;
        }
        ai->cnt++;
        if (ai->cnt >= ai->wait) {
            in->btnDown |= 0x100;
            ai->sub++;
            ai->cnt = 0;
            return;
        }
        break;
    case 3:
        if (plr->action == 0) {
            ai->sub = 0;
            if (plr->phase == 0) {
                ai->step = 0;
            }
        }
        break;
    }
}

void fn_1_6250(M502AI *ai, s16 *lvl, s16 *row, s16 *col)
{
    Vec pos;
    M502PLRSUB *plr;

    plr = ai->input->plr;
    pos.x = plr->obj->trans.x;
    pos.y = plr->obj->trans.y;
    pos.z = plr->obj->trans.z;
    pos.y -= 150.0f;
    pos.x = 300.0f * (f32)(s16)(pos.x / 300.0f);
    pos.y = 300.0f * (f32)(s16)(pos.y / 300.0f);
    pos.z = 300.0f * (f32)(s16)(pos.z / 300.0f);
    fn_1_6E1C(&pos, lvl, row, col);
}

void fn_1_6400(M502INPUT *in, s32 playerNo, M502PLRSUB *plr)
{
    memset(in, 0, sizeof(M502INPUT));
    in->plr = plr;
    in->padNo = GwPlayerConf[playerNo].padNo;
    fn_1_57B4(&in->ai, playerNo, in);
}

void fn_1_6478(M502INPUT *in)
{
    fn_1_5818(&in->ai);
    memset(in, 0, sizeof(M502INPUT));
}

void fn_1_64B8(M502INPUT *in)
{
    s8 stk;
    s8 absStk;

    if (in->ai.enabled != 0) {
        fn_1_5848(&in->ai);
        return;
    }
    in->btn = HuPadBtn[in->padNo];
    in->btnDown = HuPadBtnDown[in->padNo];
    in->btnRep = HuPadBtnRep[in->padNo];
    in->stkX = HuPadStkX[in->padNo];
    if (in->stkX >= 0) {
        absStk = in->stkX;
    } else {
        absStk = -in->stkX;
    }
    if (absStk < 10) {
        stk = 0;
    } else {
        stk = in->stkX;
    }
    in->stkX = stk;
    in->stkY = HuPadStkY[in->padNo];
    in->subX = HuPadSubStkX[in->padNo];
    in->subY = HuPadSubStkY[in->padNo];
    in->trigL = HuPadTrigL[in->padNo];
    in->trigR = HuPadTrigR[in->padNo];
    in->dStk = HuPadDStk[in->padNo];
    in->dStkRep = HuPadDStkRep[in->padNo];
}
