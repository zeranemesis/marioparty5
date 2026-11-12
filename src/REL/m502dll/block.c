#include "REL/m502dll.h"

void fn_1_8904(HU3DMODEL *model, Mtx *mtx);
void fn_1_8940(HU3DMODEL *model, Mtx *mtx);
void fn_1_897C(HU3DMODEL *model, Mtx *mtx);
void fn_1_89B8(HU3DMODEL *model, Mtx *mtx);
void fn_1_89F4(M502BLOCK *b, HU3DMODEL *model, Mtx *mtx);
void fn_1_7548(M502BLOCK *b, s16 lvl, s16 col, s16 pattern);
void fn_1_7BFC(M502BLOCK *b);
void fn_1_7ECC(M502BLOCK *b);
void fn_1_8324(M502CELL *c);
void fn_1_83AC(M502BLOCK *b, s16 kind, s16 lvl, Vec *pos, u16 n);
void fn_1_8438(M502LEVELFX *fx, M502BLOCK *b);
void fn_1_850C(M502LEVELFX *fx);
void fn_1_8594(M502LEVELFX *fx);
void fn_1_876C(M502LEVELFX *fx, s16 kind, s16 lvl, Vec *pos, u16 n);
void fn_1_8DE4(M502BLOCK *b, f32 *out);
void fn_1_8FDC(M502BLOCKFX *fx, M502BLOCK *b);
void fn_1_9028(M502BLOCKFX *fx);
void fn_1_9058(M502BLOCKFX *fx);
void fn_1_912C(M502BLOCKFX *fx, s32 idx, Vec *pos);

static HU3DMODELHOOK lbl_1_data_E98[4] = { fn_1_8904, fn_1_8940, fn_1_897C, fn_1_89B8 };
static M502BLOCK *lbl_1_data_EA8[4];
static s32 lbl_1_data_EB8 = 1;
static Vec lbl_1_data_EBC[8] = {
    { -0.8f, 1.6f, 0.0f },
    { -0.2f, 1.6f, 0.0f },
    { 0.8f, 1.6f, 0.0f },
    { -0.8f, 1.6f, 0.0f },
    { -0.4f, 1.6f, 0.0f },
    { -0.2f, 1.6f, 0.0f },
    { 0.4f, 1.6f, 0.0f },
    { 0.8f, 1.6f, 0.0f },
};
static Vec lbl_1_data_F1C[8] = {
    { -1.6f, 2.4f, 0.0f },
    { -0.1f, 2.8f, 0.0f },
    { 1.6f, 2.4f, 0.0f },
    { -1.6f, -2.8f, 0.0f },
    { -0.8f, 2.4f, 0.0f },
    { -1.2f, -2.8f, 0.0f },
    { 0.8f, 2.4f, 0.0f },
    { 1.2f, 2.8f, 0.0f },
};
static Vec lbl_1_data_F7C[8] = {
    { -1.6f, 3.2f, 0.0f },
    { -0.1f, 3.6f, 0.0f },
    { 1.6f, 3.2f, 0.0f },
    { -1.6f, 3.6f, 0.0f },
    { -1.2f, -3.2f, 0.0f },
    { -1.8f, 3.6f, 0.0f },
    { 1.2f, -3.2f, 0.0f },
    { 1.8f, 3.6f, 0.0f },
};

static f32 lbl_1_bss_5048;
static f32 lbl_1_bss_504C;

void fn_1_6664(M502BLOCK *b, s32 no, M502PLAYER *player)
{
    s32 i;
    s16 hook;

    memset(b, 0, sizeof(M502BLOCK));
    b->player = player;
    lbl_1_data_EA8[no] = b;
    for (i = 0; i < 3; i++) {
        fn_1_8438(&b->levelFx[i], b);
    }
    for (i = 0; i < 2; i++) {
        fn_1_8FDC(&b->fx[i], b);
    }
    hook = Hu3DHookFuncCreate(lbl_1_data_E98[no]);
    b->hook = hook;
    Hu3DModelCameraSet(hook, (u16)lbl_1_data_B60[no]);
    lbl_1_data_EB8 = 1;
}

void fn_1_6778(M502BLOCK *b)
{
    s32 i;

    Hu3DModelKill(b->hook);
    for (i = 0; i < 3; i++) {
        fn_1_850C(&b->levelFx[i]);
    }
    for (i = 0; i < 2; i++) {
        fn_1_9028(&b->fx[i]);
    }
    memset(b, 0, sizeof(M502BLOCK));
}

void fn_1_6808(M502BLOCK *b)
{
    M502PLAYER *player = b->player;
    M502PLRSUB *plr = &player->plr;
    s16 lvl;
    s32 idx;
    s32 i;
    s16 pan;
    s16 d;
    M502CELL *c;
    Vec a;
    Vec bpos;
    Vec scrA;
    Vec scrB;

    for (lvl = 0; lvl < 3; lvl++) {
        for (idx = 0; idx < 27; idx++) {
            if (idx % 3 == 0) {
                fn_1_6CFC(lvl, (s16)(idx / 3), 1, &a);
                bpos.x = a.x;
                bpos.y = a.y;
                bpos.z = a.z;
                a.y += 300.0f;
                bpos.y = bpos.y - 300.0f;
                Hu3D3Dto2D(&a, player->camera.camBit1, &scrA);
                Hu3D3Dto2D(&bpos, player->camera.camBit1, &scrB);
                if (fn_1_6CB8(&scrA) != 0 || fn_1_6CB8(&scrB) != 0) {
                    b->cell[lvl][idx].vis = 1;
                    b->cell[lvl][idx + 1].vis = 1;
                    b->cell[lvl][idx + 2].vis = 1;
                }
                else {
                    b->cell[lvl][idx].vis = 0;
                    b->cell[lvl][idx + 1].vis = 0;
                    b->cell[lvl][idx + 2].vis = 0;
                }
            }
            c = &b->cell[lvl][idx];
            if (c->timerE < 10000) {
                c->timerE = c->timerE - 1;
                if (c->timerE <= 0) {
                    if (c->unk1E == 2) {
                        c->unk1C = 0x1A;
                    }
                    else {
                        c->unk1C = 0x1C;
                    }
                    c->unk1C = (s16)((idx / 3) % 2 + c->unk1C - 12);
                    if (lvl == 0) {
                        c->unk1C = (s16)(c->unk1C + 12);
                    }
                    else if (lvl == 1) {
                        c->unk1C = (s16)(c->unk1C + 0x1E);
                    }
                    else {
                        c->unk1C = (s16)(c->unk1C + 0x30);
                    }
                    c->timerE = 10000;
                    for (i = 0; i < 2; i++) {
                        if (b->fx[i].state == 0) {
                            fn_1_912C(&b->fx[i], i, &c->pos);
                            break;
                        }
                    }
                }
            }
            if (c->timerC < 10000) {
                c->timerC = c->timerC - 1;
                if (c->timerC <= 0) {
                    d = plr->unk78 - c->unk20;
                    if (d > 3) {
                        d = 3;
                    }
                    else if (d < 1) {
                        d = 1;
                    }
                    fn_1_83AC(b, c->kind, lvl, &c->pos, (u16)d);
                    if (plr->playerNo == 0) {
                        pan = 0x28;
                    }
                    else if (plr->playerNo == 1) {
                        pan = 0x38;
                    }
                    else if (plr->playerNo == 2) {
                        pan = 0x48;
                    }
                    else {
                        pan = 0x58;
                    }
                    if (c->kind != 2) {
                        fn_1_218(fn_1_198(0x62D), pan);
                    }
                    else {
                        fn_1_218(fn_1_198(0x62E), pan);
                    }
                    fn_1_8324(c);
                    plr->unk70 = 2;
                }
            }
        }
    }
    for (i = 0; i < 3; i++) {
        fn_1_8594(&b->levelFx[i]);
    }
    for (i = 0; i < 2; i++) {
        fn_1_9058(&b->fx[i]);
    }
}

s32 fn_1_6CB8(Vec *v)
{
    s32 result;

    if (v->y < 0.0f) {
        result = 0;
    }
    else if (v->y >= 480.0f) {
        result = 0;
    }
    else {
        result = 1;
    }
    return result;
}

void fn_1_6CFC(s16 lvl, s16 row, s16 col, Vec *out)
{
    out->x = -300.0f + 300.0f * (f32)col;
    out->y = (f32) - ((300.0f * (f32)row) + ((2700.0f * (f32)lvl) + (600.0f * (f32)lvl)));
    out->z = 0.0f;
}

void fn_1_6E1C(Vec *pos, s16 *lvlOut, s16 *rowOut, s16 *colOut)
{
    Vec p = *pos;
    s16 lvl;
    s16 row = 0;

    lvl = (p.y > 0.0f) ? 6 :
          (p.y > -2700.0f) ? 0 :
          (p.y > -3300.0f) ? 3 :
          (p.y > -6000.0f) ? 1 :
          (p.y > -6600.0f) ? 4 :
          (p.y > -9300.0f) ? 2 :
          (p.y > -9900.0f) ? 5 : 7;
    if (lvl < 3) {
        row = (s16)((p.y + 3300.0f * (f32)lvl) / -300.0f);
        if (row >= 9) {
            row = 0;
            lvl = (lvl == 0) ? 3 :
                  (lvl == 3) ? 1 :
                  (lvl == 1) ? 4 :
                  (lvl == 4) ? 2 :
                  (lvl == 2) ? 5 : 6;
        }
    }
    *lvlOut = lvl;
    *rowOut = row;
    *colOut = (s16)((300.0f + p.x) / 300.0f);
}

s32 fn_1_70B0(M502BLOCK *b, s16 lvl, s16 row, s16 col, s16 n)
{
    s32 ret;
    s16 cnt;
    s16 i;
    s16 idx;
    M502CELL *c;

    if (lvl < 0 || lvl > 2) {
        return 0;
    }
    ret = 0;
    cnt = 0;
    for (i = 0; i < n; i++) {
        idx = col + (row + i) * 3;
        if (idx >= 27) {
            break;
        }
        c = &b->cell[lvl][idx];
        if (c->kind != 0) {
            c->unk1E = c->unk1E - 1;
            if (c->unk1E <= 0) {
                c->timerC = (s16)(i * 11 + 3);
                c->unk20 = (u16)i;
                cnt++;
                ret = 1;
            }
            if (c->kind == 2) {
                if (c->unk1E > 0) {
                    c->timerE = (s16)(i * 11 + 3);
                    omVibrate((s16)b->player->no, 0x14, 6, 6);
                }
                break;
            }
        }
    }
    switch (cnt) {
    case 0:
        break;
    case 1:
        omVibrate((s16)b->player->no, 0x14, 6, 6);
        break;
    case 2:
        omVibrate((s16)b->player->no, 0x19, 4, 2);
        break;
    case 3:
        omVibrate((s16)b->player->no, 0x1E, 0xC, 0);
        break;
    }
    return ret;
}

void fn_1_7290(M502BLOCK *dst, M502BLOCK *src)
{
    s32 lvl;
    s32 idx;
    M502CELL *d;
    M502CELL *s;

    for (lvl = 0; lvl < 3; lvl++) {
        for (idx = 0; idx < 27; idx++) {
            d = &dst->cell[lvl][idx];
            s = &src->cell[lvl][idx];
            d->kind = s->kind;
            d->type = s->type;
            d->vis = s->vis;
            d->pos = s->pos;
            d->unk1C = s->unk1C;
            d->unk1E = s->unk1E;
            d->timerC = s->timerC;
            d->timerE = s->timerE;
        }
    }
}

void fn_1_735C(M502BLOCK *b)
{
    u16 tbl[18] = { 0, 1, 2, 0, 2, 1, 1, 0, 2, 1, 2, 0, 2, 0, 1, 2, 1, 0 };
    s16 pick[3];
    s16 lvl;
    s16 col;
    s16 v;

    for (lvl = 0; lvl < 3; lvl++) {
        v = rand8() % 6;
        pick[lvl] = v;
        if (lvl != 0 && pick[lvl] == pick[lvl - 1]) {
            v = (v + 3) % 6;
            pick[lvl] = v;
        }
        for (col = 0; col < 3; col++) {
            fn_1_7548(b, lvl, col, tbl[v * 3 + col]);
        }
    }
    fn_1_7BFC(b);
    fn_1_7ECC(b);
}

void fn_1_7548(M502BLOCK *b, s16 lvl, s16 col, s16 pattern)
{
    u16 sel[4] = { 0, 0, 0, 0 };
    u8 unused[3] = { 0x40, 0x38, 0x71 };
    u8 cnt[9] = { 1, 2, 3, 1, 2, 4, 1, 2, 4 };
    u8 dead;
    s16 i;
    s16 n;
    s16 k;
    s16 same;
    u8 t;
    u8 e;

    dead = unused[pattern];
    for (i = 0; i < 9; i++) {
        if (i >= 7 && lvl == 2) {
            b->cell[lvl][col + i * 3].kind = 2;
            b->cell[lvl][col + i * 3].unk1E = 3;
        }
        else {
            b->cell[lvl][col + i * 3].kind = 1;
            b->cell[lvl][col + i * 3].unk1E = 1;
        }
    }
    switch (lvl) {
    case 0:
        switch (pattern) {
        case 0:
            sel[0] = (u8)(rand8() % 8) + 1;
            break;
        case 1:
            sel[0] = (u8)(rand8() % 4) + 1;
            sel[1] = (u8)(rand8() % 4) + 5;
            break;
        case 2:
            sel[0] = 1;
            sel[1] = (u8)(rand8() % 3) + 2;
            sel[2] = (u8)(rand8() % 4) + 5;
            break;
        }
        break;
    case 1:
        switch (pattern) {
        case 0:
            sel[0] = (u8)(rand8() % 9);
            break;
        case 1:
            sel[0] = rand8() % 9;
            do {
                t = rand8() % 9;
            } while (sel[0] == t);
            sel[1] = t;
            break;
        case 2:
            sel[0] = rand8() % 9;
            n = 1;
            do {
                t = rand8() % 9;
                same = 0;
                for (k = 0; k < n; k++) {
                    if (sel[k] != t) {
                        same++;
                    }
                }
                if (same == n) {
                    sel[n] = t;
                    n++;
                }
            } while (n != 4);
            break;
        }
        break;
    case 2:
        switch (pattern) {
        case 0:
            if ((u8)(rand8() % 10) > 1) {
                e = rand8() % 3;
            }
            else {
                e = rand8() % 4 + 3;
            }
            sel[0] = e;
            break;
        case 1:
            sel[0] = (u8)(rand8() % 3 + 2);
            sel[1] = (u8)(rand8() % 2 + 5);
            break;
        case 2:
            sel[0] = (u8)(rand8() % 3);
            e = rand8() % 4;
            n = 0;
            for (k = 0; k < 4; k++) {
                if (e != k) {
                    sel[n + 1] = (s16)(k + 3);
                    n++;
                }
            }
            break;
        }
        break;
    }
    for (i = 0; i < cnt[lvl * 3 + pattern]; i++) {
        b->cell[lvl][col + sel[i] * 3].kind = 2;
        b->cell[lvl][col + sel[i] * 3].unk1E = 3;
    }
}

void fn_1_7BFC(M502BLOCK *b)
{
    struct {
        s32 lvl;
        s32 row;
    } mark[3];
    u16 a[6] = { 0, 1, 2, 1, 0, 2 };
    u16 unusedB[9] = { 0xA, 9, 1, 2, 3, 4, 5, 0xA, 0xA };
    u8 dead;
    u8 flip;
    s32 lvl;
    s32 row;
    s32 col;
    s32 cnt;

    dead = rand8() % 2;
    flip = rand8() % 2;
    for (lvl = 0; lvl < 3; lvl++) {
        mark[lvl].row = 0;
        mark[lvl].lvl = 0;
    }
    for (lvl = 0; lvl < 3; lvl++) {
        for (row = 0; row < 9; row++) {
            cnt = 0;
            for (col = 0; col < 3; col++) {
                if (b->cell[lvl][col + row * 3].kind != 2) {
                    b->cell[lvl][col + row * 3].type = a[flip * 3 + row % 3];
                    if (row != 0) {
                        cnt++;
                    }
                }
                else {
                    b->cell[lvl][col + row * 3].type = 4;
                }
            }
            if (cnt == 3 && mark[lvl].lvl == 0) {
                mark[lvl].lvl = lvl + 1;
                mark[lvl].row = row;
            }
        }
        flip ^= 1;
    }
    for (lvl = 0; lvl < 3; lvl++) {
        if (mark[lvl].lvl != 0) {
            for (col = 0; col < 3; col++) {
                b->cell[mark[lvl].lvl - 1][col + mark[lvl].row * 3].type = 3;
            }
        }
    }
}

void fn_1_7ECC(M502BLOCK *b)
{
    Vec p;
    s16 lvl;
    s32 idx;
    s32 add;
    u32 v;
    M502CELL *c;

    for (lvl = 0; lvl < 3; lvl++) {
        switch (lvl) {
        case 0:
            add = 0xC;
            break;
        case 1:
            add = 0x1E;
            break;
        default:
            add = 0x30;
            break;
        }
        for (idx = 0; idx < 27; idx++) {
            c = &b->cell[lvl][idx];
            v = 0;
            switch (c->type) {
            case 0:
                v = (idx % 6) / 3;
                break;
            case 1:
                v = (idx % 6) / 3 + 2;
                break;
            case 2:
                v = (idx % 6) / 3 + 4;
                break;
            case 3:
                v = (idx % 3) + ((idx % 6) / 3) * 3 + 6;
                break;
            case 4:
                v = (idx % 6) / 3 + 0xC;
                break;
            }
            c->unk1C = (s16)(v + add);
            p.x = -300.0f + 300.0f * (f32)(s16)(idx % 3);
            p.y = (f32) - ((300.0f * (f32)(s16)(idx / 3)) + ((2700.0f * (f32)lvl) + (600.0f * (f32)lvl)));
            p.z = 0.0f;
            c->pos = p;
            c->vis = 1;
            c->timerC = 10000;
            c->timerE = 10000;
        }
    }
}

void fn_1_8290(M502BLOCK *b)
{
    s32 lvl;
    s32 idx;

    for (lvl = 0; lvl < 3; lvl++) {
        for (idx = 0; idx < 27; idx++) {
            if (b->cell[lvl][idx].unk1C != 0) {
                fn_1_8324(&b->cell[lvl][idx]);
            }
        }
    }
}

void fn_1_8324(M502CELL *c)
{
    Vec zero = { 0.0f, 0.0f, 0.0f };

    c->kind = 0;
    c->type = 0;
    c->vis = 0;
    c->pos = zero;
    c->unk1C = 0;
    c->unk1E = 0;
    c->timerC = 10000;
    c->timerE = 10000;
}

void fn_1_83AC(M502BLOCK *b, s16 kind, s16 lvl, Vec *pos, u16 n)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        if (b->levelFx[i].state == 0) {
            fn_1_876C(&b->levelFx[i], kind, lvl, pos, n);
            return;
        }
    }
}

void fn_1_8438(M502LEVELFX *fx, M502BLOCK *b)
{
    Vec zero;
    s32 i;

    memset(fx, 0, sizeof(M502LEVELFX));
    fx->block = b;
    for (i = 0; i < 8; i++) {
        fx->model[i] = 0;
        zero.x = 0.0f;
        zero.y = 0.0f;
        zero.z = 0.0f;
        fx->pos[i] = zero;
    }
}

void fn_1_850C(M502LEVELFX *fx)
{
    s32 i;

    for (i = 0; i < 8; i++) {
        if (fx->model[i] != 0) {
            Hu3DModelKill(fx->model[i]);
            fx->model[i] = 0;
        }
    }
    memset(fx, 0, sizeof(M502LEVELFX));
}

void fn_1_8594(M502LEVELFX *fx)
{
    Vec zeroA;
    Vec zeroB;
    s32 i;

    switch (fx->state) {
    case 1:
        for (i = 0; i < 8; i++) {
            fx->acc[i].y -= 0.4f;
            PSVECAdd(&fx->vel[i], &fx->acc[i], &fx->vel[i]);
            PSVECAdd(&fx->vel[i], &fx->pos[i], &fx->pos[i]);
        }
        if (fx->t++ > 0x3C) {
            fx->state = 2;
        }
        return;
    case 2:
        for (i = 0; i < 8; i++) {
            zeroA.x = 0.0f;
            zeroA.y = 0.0f;
            zeroA.z = 0.0f;
            fx->pos[i] = zeroA;
            zeroB.x = 0.0f;
            zeroB.y = 0.0f;
            zeroB.z = 0.0f;
            fx->vel[i] = zeroB;
            fx->model[i] = 0;
        }
        fx->state = 0;
        fx->t = 0;
        fx->active = 0;
        break;
    }
}

void fn_1_876C(M502LEVELFX *fx, s16 kind, s16 lvl, Vec *pos, u16 n)
{
    s32 base;
    s32 i;
    Vec *src;

    if (kind == 2) {
        if (lvl == 0) {
            base = 0x5A;
        }
        else if (lvl == 1) {
            base = 0x62;
        }
        else {
            base = 0x6A;
        }
    }
    else {
        if (lvl == 0) {
            base = 0x42;
        }
        else if (lvl == 1) {
            base = 0x4A;
        }
        else {
            base = 0x52;
        }
    }
    for (i = 0; i < 8; i++) {
        fx->model[i] = (s16)(base + i);
        fx->pos[i] = *pos;
        if (n == 3) {
            src = &lbl_1_data_F7C[i];
        }
        else if (n == 2) {
            src = &lbl_1_data_F1C[i];
        }
        else {
            src = &lbl_1_data_EBC[i];
        }
        fx->acc[i] = *src;
        fx->active = 1;
    }
    fx->state = 1;
    fx->t = 0;
}

void fn_1_8904(HU3DMODEL *model, Mtx *mtx)
{
    fn_1_89F4(lbl_1_data_EA8[0], model, mtx);
}

void fn_1_8940(HU3DMODEL *model, Mtx *mtx)
{
    fn_1_89F4(lbl_1_data_EA8[1], model, mtx);
}

void fn_1_897C(HU3DMODEL *model, Mtx *mtx)
{
    fn_1_89F4(lbl_1_data_EA8[2], model, mtx);
}

void fn_1_89B8(HU3DMODEL *model, Mtx *mtx)
{
    fn_1_89F4(lbl_1_data_EA8[3], model, mtx);
}

void fn_1_89F4(M502BLOCK *b, HU3DMODEL *model, Mtx *mtx)
{
    M502PLAYER *player = b->player;
    M502MAIN *m = player->main;
    M502MODELS *mdl = &m->models;
    M502LIGHT *light = &m->light;
    M502STAGE *stage = &player->stage;
    M502LIGHT *light2;
    s32 playerNo;
    f32 scale;
    f32 f30;
    f32 f31;
    s32 col;
    s32 lvl;
    s32 idx;
    s32 k;
    s32 j;
    s16 id;
    M502CELL *c;
    M502LEVELFX *fx;
    Mtx mtxL;

    if (b != NULL) {
        playerNo = player->no;
        if (m->state != 6) {
            Hu3DModelObjDrawInit();
            light2 = &m->light;
            scale = 1.0f;
            if (lbl_1_data_EB8 != 0) {
                fn_1_8DE4(b, &scale);
            }
            f30 = 0.3f + 0.3f * scale;
            f31 = 0.7f + 0.3f * scale;
            col = (s32)(255.0f * (0.2f + 0.6f * scale));
            if (lbl_1_data_EB8 != 0) {
                Hu3DGLightColorSet(light2->id, col, col, col, col);
            }
            for (k = 0; k < 8; k++) {
                id = mdl->model[stage->modelNo[k]];
                if (lbl_1_data_EB8 != 0) {
                    Hu3DModelAmbSet(id, f30, f30, f30);
                }
                PSMTXTrans(mtxL, 0.0f, 0.0f, 0.0f);
                PSMTXConcat(*mtx, mtxL, mtxL);
                Hu3DModelObjDraw(id, stage->name[k], mtxL);
            }
            for (lvl = 0; lvl < 3; lvl++) {
                for (idx = 0; idx < 27; idx++) {
                    c = &b->cell[lvl][idx];
                    if (c->vis != 0 && c->kind != 0) {
                        PSMTXTrans(mtxL, c->pos.x, c->pos.y, c->pos.z);
                        PSMTXConcat(*mtx, mtxL, mtxL);
                        id = mdl->model[c->unk1C];
                        if (lbl_1_data_EB8 != 0) {
                            Hu3DModelAmbSet(id, f31, f31, f31);
                        }
                        Hu3DModelObjDraw(id, mdl->name[c->unk1C], mtxL);
                    }
                }
            }
            for (k = 0; k < 3; k++) {
                fx = &b->levelFx[k];
                if (fx->active != 0) {
                    for (j = 0; j < 8; j++) {
                        PSMTXTrans(mtxL, fx->pos[j].x, fx->pos[j].y, fx->pos[j].z);
                        PSMTXConcat(*mtx, mtxL, mtxL);
                        id = mdl->model[fx->model[j]];
                        if (lbl_1_data_EB8 != 0) {
                            Hu3DModelAmbSet(id, f31, f31, f31);
                        }
                        Hu3DModelObjDraw(id, mdl->name[fx->model[j]], mtxL);
                    }
                }
            }
            if (lbl_1_data_EB8 != 0) {
                Hu3DGLightColorSet(light->id, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }
    }
}

void fn_1_8DE4(M502BLOCK *b, f32 *out)
{
    M502PLRSUB *plr = &b->player->plr;
    f32 t;
    f32 r;
    f32 c;

    if (plr->obj->trans.y < lbl_1_bss_504C) {
        t = (plr->obj->trans.y - lbl_1_bss_504C) / (-9360.0f - lbl_1_bss_504C);
        if (t > 1.0f) {
            c = 1.0f;
        }
        else if (t < 0.0f) {
            c = 0.0f;
        }
        else {
            c = t;
        }
        r = 1.0f - c;
    }
    else {
        r = 1.0f;
    }
    *out = r;
}

void fn_1_8F04(M502BLOCK *b)
{
    M502PLAYER *player = b->player;
    M502MAIN *m = player->main;
    s32 i;

    for (i = 0; i < 8; i++) {
        if (lbl_1_data_EB8 != 0) {
            Hu3DModelAmbSet(m->models.model[player->stage.modelNo[i]], 1.0f, 1.0f, 1.0f);
        }
        Hu3DGLightColorSet(m->light.id, 0xFF, 0xFF, 0xFF, 0xFF);
    }
    lbl_1_data_EB8 = 0;
}

void fn_1_8FDC(M502BLOCKFX *fx, M502BLOCK *b)
{
    memset(fx, 0, sizeof(M502BLOCKFX));
    fx->block = b;
    fx->idx = 0;
}

void fn_1_9028(M502BLOCKFX *fx)
{
    memset(fx, 0, sizeof(M502BLOCKFX));
}

void fn_1_9058(M502BLOCKFX *fx)
{
    M502MODELS *mdl = &fx->block->player->main->models;
    f32 time;
    f32 maxTime;

    switch (fx->state) {
    case 0:
        break;
    case 1:
        time = Hu3DData[mdl->model[fx->idx]].motShapeWork.time;
        maxTime = Hu3DMotionShapeMaxTimeGet(mdl->model[fx->idx]);
        if (time >= maxTime) {
            Hu3DModelAttrSet(mdl->model[fx->idx], 1);
            fx->state = 0;
        }
        break;
    }
}

void fn_1_912C(M502BLOCKFX *fx, s32 idx, Vec *pos)
{
    M502PLAYER *player = fx->block->player;
    M502MODELS *mdl = &player->main->models;
    s32 playerNo = player->no;
    Vec v;
    s32 yi;
    s32 row;

    v = *pos;
    yi = (s32)pos->y;
    fx->idx = (s16)(playerNo * 2 + idx + 0x79);
    Hu3DModelAttrReset(mdl->model[fx->idx], 1);
    Hu3DMotionShapeSpeedSet(mdl->model[fx->idx], 1.0f);
    Hu3DMotionShapeTimeSet(mdl->model[fx->idx], 0.0f);
    Hu3DModelCameraSet(mdl->model[fx->idx], (u16)lbl_1_data_B60[playerNo]);
    if (pos->y > -2700.0f) {
        fx->amb[0] = 0.5f;
        fx->amb[1] = 0.5f;
        fx->amb[2] = 0.0f;
    }
    else if (pos->y > -6000.0f) {
        yi = (s32)((f32)yi - 3300.0f);
        fx->amb[0] = 0.25f;
        fx->amb[1] = 0.15f;
        fx->amb[2] = 0.15f;
    }
    else {
        yi = (s32)((f32)yi - 6600.0f);
        fx->amb[0] = 0.2f;
        fx->amb[1] = 0.0f;
        fx->amb[2] = 0.1f;
    }
    row = (s32)((f32)yi / 300.0f);
    if (row % 2 != 0) {
        v.x -= 30.000002f;
    }
    else {
        v.x += 15.000001f;
    }
    v.y += 25.0f;
    Hu3DModelPosSet(mdl->model[fx->idx], v.x, v.y, v.z);
    Hu3DModelAmbSet(mdl->model[fx->idx], fx->amb[0], fx->amb[1], fx->amb[2]);
    fx->state = 1;
    fx->pad6 = 0;
}
