#include "types.h"
#include "string.h"
#include "REL/m552dll.h"

int rand8(void);

extern M552Field lbl_1_bss_3700;
extern M552State lbl_1_bss_4794;
extern s32 lbl_1_data_410[22];

/* live com state for one player, 0x7C bytes */
typedef struct M552Com_s {
    s32 level; /* 1-based cpu level, selects the time table row */
    s32 state; /* 0 = pick, 1 = run, 3 = wait for the timer */
    s32 sub;
    s32 pad_0C;
    s32 next; /* state entered when the timer in `timer` expires */
    s32 timer;
    u8 pad_18[0xC];
    s32 target; /* index of the falling piece being chased */
    s32 cell; /* board cell the piece is steered to */
    u8 pad_2C[0x40];
    s32 shape; /* shape of the targeted piece */
    u8 pad_70[0xC];
} M552Com;

typedef struct M552ComTick_s {
    s32 count;
    s32 pad;
} M552ComTick;

void fn_1_A7AC(s32 idx);
void fn_1_A8F4(s32 idx);
void fn_1_B2C0(s32 idx);

/* unit-owned .bss */
M552Com lbl_1_bss_4930[2];
f32 lbl_1_bss_4928[2];

/* unit-owned .data */
M552ComTick lbl_1_data_550[2] = { { 0, 0 }, { 0, 0 } };
s32 lbl_1_data_560[2] = { 0, 0 };

void fn_1_91BC(void)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        memset(&lbl_1_bss_4930[i], 0, sizeof(M552Com));
        lbl_1_bss_4930[i].level = lbl_1_bss_4794.chars[i].cpuLevel;
    }
}

void fn_1_9238(s32 idx)
{
    switch (lbl_1_bss_4930[idx].state) {
    case 0:
        fn_1_A7AC(idx);
        break;
    case 1:
        fn_1_A8F4(idx);
        break;
    case 2:
        break;
    case 3:
        fn_1_B2C0(idx);
        break;
    }
}

s32 fn_1_92BC(s32 idx, s32 type)
{
    s32 tblA[4] = { 75, 30, 10, 0 };
    s32 tblB[4] = { 75, 15, 10, 0 };
    s32 tblC[4] = { 75, 5, 1, 1 };
    s32 time = 0;

    switch (type) {
    case 0:
        time = tblA[lbl_1_bss_4930[idx].level - 1];
        time += rand8() % 10;
        break;
    case 1:
        time = tblB[lbl_1_bss_4930[idx].level - 1];
        time += rand8() % 10;
        break;
    case 2:
        time = tblC[lbl_1_bss_4930[idx].level - 1] + 2;
        break;
    }
    return time;
}

s32 fn_1_9460(s32 arg0, s32 arg1, s32 arg2)
{
    s32 col = arg1 % 5;
    s32 row = arg1 / 5;
    s32 ret = 0;
    s32 s;
    s32 o;
    s32 w;
    s32 h;
    s32 x;
    s32 y;
    s32 sum;
    s32 bit;

    if (arg2 < 0) {
        bit = 2;
        for (s = 1; s < 6; s++) {
            for (o = 0; o < 2; o++) {
                if (o == 0) {
                    w = lbl_1_data_410[s * 2];
                    h = lbl_1_data_410[s * 2 + 1];
                } else {
                    w = lbl_1_data_410[s * 2 + 1];
                    h = lbl_1_data_410[s * 2];
                }
                sum = 1;
                if (col + w <= 5 && row + h <= 7) {
                    sum = 0;
                    for (y = row; y < row + h; y++) {
                        for (x = col; x < col + w; x++) {
                            sum += lbl_1_bss_3700.plr[arg0].board[x + (y * 5)];
                        }
                    }
                    if (sum == 0) {
                        break;
                    }
                }
            }
            if (sum == 0) {
                ret += bit;
            }
            bit *= 2;
        }
        return ret;
    }
    for (o = 0; o < 2; o++) {
        if (o == 0) {
            w = lbl_1_data_410[arg2 * 2];
            h = lbl_1_data_410[arg2 * 2 + 1];
        } else {
            w = lbl_1_data_410[arg2 * 2 + 1];
            h = lbl_1_data_410[arg2 * 2];
        }
        if (col + w <= 5 && row + h <= 7) {
            sum = 0;
            for (y = row; y < row + h; y++) {
                for (x = col; x < col + w; x++) {
                    sum += lbl_1_bss_3700.plr[arg0].board[x + (y * 5)];
                }
            }
            if (sum == 0) {
                break;
            }
        }
    }
    return o;
}

s8 fn_1_9718(s32 arg0)
{
    s32 bits[7] = { 1, 2, 4, 8, 0x10, 0x20, 0x40 };
    s8 cand[8];
    f32 bestDist = -1.0f;
    f32 minY;
    f32 dx;
    f32 dy;
    f32 dist;
    s32 pieceMask;
    s32 fitMask;
    s32 cell;
    s32 best;
    s32 shape;
    s32 bit;
    s32 bestIdx;
    s32 j;
    s8 i;
    s8 n;

#ifdef TARGET_PC
    shape = 0;
#endif
    minY = lbl_1_bss_3700.plr[arg0].y - 350.0f;
    if (minY < -400.0f) {
        minY = -400.0f;
    }
    i = 0;
    pieceMask = 0;
    n = 0;
    for (; i < 0x14; i++) {
        if (lbl_1_bss_3700.item[i].alive == 0 || !(lbl_1_bss_3700.item[i].y > minY)
            || !(lbl_1_bss_3700.item[i].y < 300.0f)) {
            continue;
        }
        cand[n] = i;
        n++;
        pieceMask |= bits[lbl_1_bss_3700.item[i].shape];
        if (n >= 8) {
            break;
        }
    }
    if (n == 0) {
        return -1;
    }
    j = 0;
    best = -1;
    fitMask = 0;
    for (; j < 0x23; j++) {
        if (arg0 != 0) {
            cell = j;
        } else {
            cell = (4 - (j % 5)) + ((j / 5) * 5);
        }
        if (lbl_1_bss_3700.plr[arg0].board[cell] == 0) {
            best = cell;
            fitMask |= fn_1_9460(arg0, cell, -1);
        }
    }
    if ((fitMask & pieceMask) != 0) {
        best = -1;
    }
    if (best < 0) {
        j = 0;
        bit = 2;
        for (; j < 5; j++) {
            if ((fitMask & pieceMask & bit) != 0) {
                shape = j + 1;
            }
            bit *= 2;
        }
        i = 0;
        bestIdx = 0;
        for (; i < n; i++) {
            if (shape == lbl_1_bss_3700.item[cand[i]].shape
                && ((dx = lbl_1_bss_3700.item[cand[i]].x - lbl_1_bss_3700.plr[arg0].x,
                     dy = lbl_1_bss_3700.item[cand[i]].y - lbl_1_bss_3700.plr[arg0].y, dist = (dy * dy) + (dx * dx),
                     -1.0f == bestDist)
                    || bestDist > dist)) {
                bestDist = dist;
                bestIdx = i;
            }
        }
        for (j = 0; j < 0x23; j++) {
            if (arg0 != 0) {
                if (j < 0x15) {
                    cell = (j % 3) + ((j / 3) * 5);
                } else {
                    cell = (((j - 0x15) % 7) * 5) + ((j - 0x15) / 7) + 3;
                }
            } else if (j < 0x15) {
                cell = (((j / 3) * 5) + 4) - (j % 3);
            } else {
                cell = ((((j - 0x15) % 7) * 5) + 1) - ((j - 0x15) / 7);
            }
            if (fn_1_9460(arg0, cell, shape) != 2) {
                break;
            }
        }
        lbl_1_bss_4930[arg0].cell = cell;
        return cand[bestIdx];
    }
    for (j = 0; j < 0x23; j++) {
        if (arg0 != 0) {
            if (j < 0x15) {
                cell = (j % 3) + ((j / 3) * 5);
            } else {
                cell = (((j - 0x15) % 7) * 5) + ((j - 0x15) / 7) + 3;
            }
        } else if (j < 0x15) {
            cell = (((j / 3) * 5) + 4) - (j % 3);
        } else {
            cell = ((((j - 0x15) % 7) * 5) + 1) - ((j - 0x15) / 7);
        }
        if (lbl_1_bss_3700.plr[arg0].board[cell] == 0) {
            break;
        }
    }
    lbl_1_bss_4930[arg0].cell = cell;
    bestIdx = n;
    i = 0;
    for (; i < n; i++) {
        if (lbl_1_bss_3700.item[cand[i]].shape == 0
            && ((dx = lbl_1_bss_3700.item[cand[i]].x - lbl_1_bss_3700.plr[arg0].x,
                 dy = lbl_1_bss_3700.item[cand[i]].y - lbl_1_bss_3700.plr[arg0].y, dist = (dy * dy) + (dx * dx),
                 -1.0f == bestDist)
                || bestDist > dist)) {
            bestDist = dist;
            bestIdx = i;
        }
    }
    if (bestIdx < n) {
        return cand[bestIdx];
    }
    return -1;
}

s32 fn_1_9EE8(s32 arg0, s32 arg1)
{
    f32 dx;
    f32 dy;
    s32 w;
    s32 h;
    s32 shapeW;

    switch (arg1) {
    case 0:
        shapeW = lbl_1_data_410[lbl_1_bss_3700.plr[arg0].piece.shape * 2];
        if (arg0 != 0) {
            dx = lbl_1_bss_3700.item[lbl_1_bss_4930[arg0].target].x + (f32)(shapeW * 0x32);
        } else {
            dx = lbl_1_bss_3700.item[lbl_1_bss_4930[arg0].target].x - (f32)(shapeW * 0x32);
        }
        dx = dx - lbl_1_bss_3700.plr[arg0].x;
        dy = lbl_1_bss_3700.item[lbl_1_bss_4930[arg0].target].y - lbl_1_bss_3700.plr[arg0].y;
        if (lbl_1_bss_3700.plr[arg0].y < (50.0f + lbl_1_bss_3700.item[lbl_1_bss_4930[arg0].target].y)
            && lbl_1_bss_3700.plr[arg0].y > (lbl_1_bss_3700.item[lbl_1_bss_4930[arg0].target].y - 50.0f)) {
            dy = 0.0f;
        }
        if (lbl_1_data_550[arg0].count != 0) {
            dy = lbl_1_bss_4928[arg0];
            lbl_1_data_550[arg0].count += 1;
            if (lbl_1_data_550[arg0].count == 5) {
                lbl_1_data_550[arg0].count = 0;
            }
        } else {
            lbl_1_data_550[arg0].count += 1;
            lbl_1_bss_4928[arg0] = dy;
        }
        break;
    case 1:
        if (fn_1_9460(arg0, lbl_1_bss_4930[arg0].cell, lbl_1_bss_3700.plr[arg0].piece.shape) == 1) {
            if (lbl_1_bss_4794.stat[arg0 + 2] == 2 && lbl_1_bss_3700.plr[arg0].piece.rot == 0.0f) {
                lbl_1_bss_4794.chars[arg0].btnDown = 0x40;
            }
            h = lbl_1_data_410[(lbl_1_bss_3700.plr[arg0].piece.shape * 2) + 1] - 1;
            w = lbl_1_data_410[lbl_1_bss_3700.plr[arg0].piece.shape * 2] - 1;
        } else {
            h = lbl_1_data_410[lbl_1_bss_3700.plr[arg0].piece.shape * 2] - 1;
            w = lbl_1_data_410[(lbl_1_bss_3700.plr[arg0].piece.shape * 2) + 1] - 1;
        }
        dx = ((50.0f * (f32)h)
              + (-750.0f + (1100.0f * (f32)arg0) + (100.0f * (f32)(lbl_1_bss_4930[arg0].cell % 5))))
            - lbl_1_bss_3700.plr[arg0].x;
        dy = ((250.0f - (100.0f * (f32)(lbl_1_bss_4930[arg0].cell / 5))) - (50.0f * (f32)w))
            - lbl_1_bss_3700.plr[arg0].y;
        break;
    case 2:
        dx = (-250.0f + (500.0f * (f32)arg0)) - lbl_1_bss_3700.plr[arg0].x;
        dy = 150.0f - lbl_1_bss_3700.plr[arg0].y;
        break;
    case 3:
        dx = (f32)((arg0 * 0x4B0) - 0x258) - lbl_1_bss_3700.plr[arg0].x;
        dy = -525.0f - lbl_1_bss_3700.plr[arg0].y;
        break;
    }
    if (dx > 10.0f) {
        lbl_1_bss_4794.chars[arg0].stkX = 0xA;
    } else if (dx < -10.0f) {
        lbl_1_bss_4794.chars[arg0].stkX = -0xA;
    }
    if (dy > 10.0f) {
        lbl_1_bss_4794.chars[arg0].stkY = 0xA;
    } else if (dy < -10.0f) {
        lbl_1_bss_4794.chars[arg0].stkY = -0xA;
    }
    if (dx > -12.0f && dx < 12.0f && dy > -11.5 && dy < 11.5) {
        if (arg1 != 2) {
            if (lbl_1_bss_4794.timer[arg0 + 2] == 0) {
                lbl_1_bss_4794.chars[arg0].btnDown = 0x100;
                return 1;
            }
        } else {
            return 1;
        }
    }
    return 0;
}

void fn_1_A7AC(s32 idx)
{
    lbl_1_bss_4930[idx].state = 3;
    lbl_1_bss_4930[idx].timer = fn_1_92BC(idx, 0);
    lbl_1_bss_4930[idx].next = 1;
}

void fn_1_A8F4(s32 idx)
{
    s32 t;

    switch (lbl_1_bss_4930[idx].sub) {
    case 0:
        if (lbl_1_bss_4794.stat[idx + 2] == 4 || lbl_1_bss_4794.stat[idx + 2] == 5
            || lbl_1_bss_4794.stat[idx + 2] == 6) {
            lbl_1_bss_4930[idx].sub = 3;
            lbl_1_bss_4930[idx].target = -1;
            goto pick;
        }
        if (lbl_1_bss_4794.stat[idx + 2] == 0) {
            lbl_1_bss_4930[idx].target = fn_1_9718(idx);
        pick:
            lbl_1_data_560[idx] = 0;
            if (lbl_1_bss_4930[idx].target < 0) {
                lbl_1_bss_4930[idx].sub = 3;
                return;
            }
            lbl_1_bss_4930[idx].sub += 1;
            lbl_1_bss_4930[idx].shape = lbl_1_bss_3700.item[lbl_1_bss_4930[idx].target].shape;
            return;
        }
        if (lbl_1_bss_4794.stat[idx + 2] != 3) {
            lbl_1_bss_4930[idx].sub = 5;
            return;
        }
        return;
    case 1:
        if (lbl_1_bss_4794.stat[idx + 2] == 0
            && (lbl_1_bss_3700.item[lbl_1_bss_4930[idx].target].alive == 0
                || lbl_1_bss_3700.item[lbl_1_bss_4930[idx].target].y < -525.0f)) {
            lbl_1_bss_4930[idx].state = 3;
            lbl_1_bss_4930[idx].next = 1;
            lbl_1_bss_4930[idx].timer = fn_1_92BC(idx, 1);
            return;
        }
        if (fn_1_9EE8(idx, 0) == 0) {
            if (lbl_1_bss_4794.stat[idx + 2] == 4 || lbl_1_bss_4794.stat[idx + 2] == 5
                || lbl_1_bss_4794.stat[idx + 2] == 6) {
                lbl_1_bss_4930[idx].sub = 3;
                lbl_1_bss_4930[idx].target = -1;
                return;
            }
        } else {
            lbl_1_bss_4930[idx].sub += 1;
            return;
        }
        break;
    case 2:
        if (lbl_1_bss_4794.stat[idx + 2] == 1 || lbl_1_bss_4794.stat[idx + 2] == 2) {
            if (lbl_1_bss_3700.plr[idx].piece.shape != lbl_1_bss_4930[idx].shape) {
                lbl_1_bss_4930[idx].sub = 5;
                return;
            }
            if (fn_1_9EE8(idx, 1) != 0) {
                lbl_1_bss_4930[idx].state = 3;
                lbl_1_bss_4930[idx].next = 1;
                lbl_1_bss_4930[idx].timer = fn_1_92BC(idx, 2);
                return;
            }
        } else {
            t = lbl_1_bss_4930[idx].timer;
            lbl_1_bss_4930[idx].timer = t + 1;
            if (t > 0x14) {
                lbl_1_bss_4930[idx].state = 3;
                lbl_1_bss_4930[idx].next = 1;
                lbl_1_bss_4930[idx].timer = fn_1_92BC(idx, 1);
                return;
            }
        }
        break;
    case 3:
        if (fn_1_9EE8(idx, 2) != 0) {
            lbl_1_bss_4930[idx].sub += 1;
            lbl_1_bss_4930[idx].timer = fn_1_92BC(idx, 2);
            return;
        }
        break;
    case 4:
        lbl_1_bss_4930[idx].timer -= 1;
        if (lbl_1_bss_4930[idx].timer <= 0) {
            lbl_1_bss_4930[idx].sub = 0;
            return;
        }
        break;
    case 5:
        if (fn_1_9EE8(idx, 3) != 0) {
            lbl_1_bss_4930[idx].sub = 0;
            lbl_1_bss_4930[idx].timer = fn_1_92BC(idx, 1);
        }
        break;
    }
}

void fn_1_B2C0(s32 idx)
{
    lbl_1_bss_4930[idx].timer -= 1;
    if (lbl_1_bss_4930[idx].timer <= 0) {
        lbl_1_bss_4930[idx].state = lbl_1_bss_4930[idx].next;
        lbl_1_bss_4930[idx].sub = 0;
    }
}
