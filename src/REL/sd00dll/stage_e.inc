/* stage part e: trial stage (random bumpy ground) generation.
   Needs from part A (defined earlier in stage.c): stageTrialMap (u8[16*16]), stageTrialMapColType
   (u8[128*128]), stageTrialHeight, stageTrialSmooth, stageTrialLavaFloorOnF, stageTrialClearOff,
   texturePtr (u16 *, 512x512 RGB565), trialModelId[4], sys. */

extern s32 rand8(void);

/* defined here, only used by the noise random walk */
static s32 stageTrialNoiseDec;
static s32 stageTrialNoiseY;
static s32 stageTrialNoiseX;
static s32 stageTrialNoiseNum;

static void StageTrialAddNoise(s32 *x, s32 *y, s32 val);
static void StageTrialAddSpecial(s32 x, s32 y, s32 count, s32 val);
static void StageTrialAddBoundary(void);
static void StageTrialClearDist(Vec *pos, f32 dist);
static void StageTrialAddSlope(Vec *pos, f32 radius);
static void StageTrialAddSlope2(Vec *from, Vec *to, f32 radius);

static void StageTrialMakeGround(void)
{
    s32 kindTbl[8] = { 16, 32, 48, 64, 32, 32, 48, 64 };
    Vec pos;
    Vec tmp;
    s32 lava;
    s32 idx;
    s32 a, b, c;

    if (stageTrialHeight > 0.0f) {
        if (rand8() % 6 == 0) {
            if (rand8() % 2 != 0) {
                pos.x = pos.y = pos.z = 0.0f;
                pos.x = 300.0f * (f32)(rand8() % 6);
                pos.z = 300.0f * (f32)(rand8() % 6 + 10);
                sys->trialPos[0] = pos;
                tmp = sys->trialPos[0];
                StageTrialClearDist(&tmp, 800.0f);
            }
            if (rand8() % 2 != 0) {
                pos.x = pos.y = pos.z = 0.0f;
                pos.x = 300.0f * (f32)(rand8() % 6 + 10);
                pos.z = 300.0f * (f32)(rand8() % 6);
                sys->trialPos[1] = pos;
                tmp = sys->trialPos[1];
                StageTrialClearDist(&tmp, 800.0f);
            }
        }
        pos.x = pos.y = pos.z = 0.0f;
        pos.x = 300.0f * (f32)(rand8() % 16);
        pos.y = 2.6f * stageTrialHeight;
        pos.z = 300.0f * (f32)(rand8() % 16);
        tmp = pos;
        StageTrialAddSlope(&tmp, 1000.0f);

        pos.x = pos.y = pos.z = 0.0f;
        pos.x = 300.0f * (f32)(rand8() % 16);
        pos.y = 2.6f * stageTrialHeight;
        pos.z = 300.0f * (f32)(rand8() % 16);
        tmp = pos;
        StageTrialAddSlope(&tmp, 1000.0f);

        pos.x = 300.0f;
        pos.y = 2.6f * stageTrialHeight;
        pos.z = 300.0f;
        tmp = pos;
        StageTrialAddSlope(&tmp, 1000.0f);

        pos.x = 4200.0f;
        pos.y = 2.6f * stageTrialHeight;
        pos.z = 4200.0f;
        tmp = pos;
        StageTrialAddSlope(&tmp, 1000.0f);

        {
            Vec from, to;

            from.x = 900.0f;
            from.y = 0.6f * stageTrialHeight;
            from.z = 900.0f;
            to.x = 3600.0f;
            to.y = 0.6f * stageTrialHeight;
            to.z = 3600.0f;
            StageTrialAddSlope2(&from, &to, 600.0f);

            if (rand8() % 2 != 0) {
                from.x = 900.0f;
                from.y = 1.6f * stageTrialHeight;
                from.z = 3600.0f;
                to.x = 3600.0f;
                to.y = 1.6f * stageTrialHeight;
                to.z = 900.0f;
                StageTrialAddSlope2(&from, &to, 300.0f);
            }
        }
    }
    lava = (stageTrialLavaFloorOnF != 0) ? 1 : 0;
    if (rand8() % 2 != 0) {
        /* arguments are evaluated last to first */
        idx = lava * 4 + rand8() % 4;
        c = rand8() % 15;
        b = rand8() % 3;
        a = rand8() % 3;
        StageTrialAddSpecial(a, b, c + 10, kindTbl[idx]);

        idx = lava * 4 + rand8() % 4;
        c = rand8() % 15;
        b = rand8() % 5;
        a = rand8() % 5;
        StageTrialAddSpecial(a + 10, b + 10, c + 10, kindTbl[idx]);

        idx = lava * 4 + rand8() % 4;
        c = rand8() % 15;
        b = rand8() % 5;
        a = rand8() % 5;
        StageTrialAddSpecial(a, b + 10, c + 10, kindTbl[idx]);

        idx = lava * 4 + rand8() % 4;
        c = rand8() % 15;
        b = rand8() % 5;
        a = rand8() % 5;
        StageTrialAddSpecial(a + 10, b, c + 10, kindTbl[idx]);

        idx = lava * 4 + rand8() % 4;
        c = rand8() % 15;
        StageTrialAddSpecial(8, 8, c + 10, kindTbl[idx]);
    }
    StageTrialAddBoundary();
}

/* random walk painting `val` (type<<4) into the 16x16 type map, stageTrialNoiseNum steps */
static void StageTrialAddNoise(s32 *x, s32 *y, s32 val)
{
    stageTrialMap[*y * 16 + *x] &= 0x8F;
    stageTrialMap[*y * 16 + *x] |= val;
    if (--stageTrialNoiseNum > 0) {
        stageTrialNoiseDec = rand8() % 4;
        if (stageTrialNoiseDec == 0) {
            stageTrialNoiseX = *x;
            stageTrialNoiseY = *y - 1;
        } else if (stageTrialNoiseDec == 1) {
            stageTrialNoiseX = *x;
            stageTrialNoiseY = *y + 1;
        } else if (stageTrialNoiseDec == 2) {
            stageTrialNoiseX = *x - 1;
            stageTrialNoiseY = *y;
        } else {
            stageTrialNoiseX = *x + 1;
            stageTrialNoiseY = *y;
        }
        if (stageTrialNoiseX > 15) {
            stageTrialNoiseX = 15;
        } else if (stageTrialNoiseX < 0) {
            stageTrialNoiseX = 0;
        }
        if (stageTrialNoiseY > 15) {
            stageTrialNoiseY = 15;
        } else if (stageTrialNoiseY < 0) {
            stageTrialNoiseY = 0;
        }
        StageTrialAddNoise(&stageTrialNoiseX, &stageTrialNoiseY, val);
    }
}

/* paints a 2x2 block of `val` at (x,y) (mirrored toward the map centre), then random walks */
static void StageTrialAddSpecial(s32 x, s32 y, s32 count, s32 val)
{
    s32 x2;
    s32 y2;

    if (x < 8) {
        x2 = x + 1;
    } else {
        x2 = x - 1;
    }
    if (y < 8) {
        y2 = y + 1;
    } else {
        y2 = y - 1;
    }
    stageTrialMap[y * 16 + x] &= 0x8F;
    stageTrialMap[y * 16 + x] |= val;
    stageTrialMap[y * 16 + x2] &= 0x8F;
    stageTrialMap[y * 16 + x2] |= val;
    stageTrialMap[y2 * 16 + x] &= 0x8F;
    stageTrialMap[y2 * 16 + x] |= val;
    stageTrialMap[y2 * 16 + x2] &= 0x8F;
    stageTrialMap[y2 * 16 + x2] |= val;
    stageTrialNoiseNum = count;
    StageTrialAddNoise(&x2, &y2, val);
}

/* builds the 128x128 per-texel type map from the 16x16 map and, for smooth>=3, blends borders */
static void StageTrialAddBoundary(void)
{
    s32 y, x, sy, sx, k, j;
    s32 flags;
    s32 type, nType;
    s32 ny, nx;
    s32 nextA, nextB;
    s32 amp;
    f32 ang;
    s32 n;

    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            for (sy = 0; sy < 8; sy++) {
                for (sx = 0; sx < 8; sx++) {
                    stageTrialMapColType[(y * 8 + sy) * 128 + x * 8 + sx] = (stageTrialMap[y * 16 + x] >> 4) & 7;
                }
            }
        }
    }
    if (stageTrialSmooth >= 3) {
        nextA = nextB = 0;
        amp = 0;
        ang = 0.0f;
        for (y = 0; y < 16; y++) {
            for (x = 0; x < 16; x++) {
                flags = 0;
                type = (stageTrialMap[y * 16 + x] >> 4) & 7;
                for (k = 0; k < 9; k++) {
                    ny = y + k / 3 - 1;
                    if (ny > 15) {
                        ny = 15;
                    } else if (ny < 0) {
                        ny = 0;
                    }
                    nx = x + k % 3 - 1;
                    if (nx > 15) {
                        nx = 15;
                    } else if (nx < 0) {
                        nx = 0;
                    }
                    nType = (stageTrialMap[ny * 16 + nx] >> 4) & 7;
                    if (type != nType) {
                        stageTrialMap[y * 16 + x] |= 0x80;
                        if (k == 5) {
                            flags |= 1;
                            nextA = nType;
                        }
                        if (k == 7) {
                            flags |= 2;
                            nextB = nType;
                        }
                    }
                }
                if (flags & 1) {
                    for (sx = 0; sx < 8; sx++) {
                        if (sx % 4 == 0) {
                            amp = rand8() % 2 + 1;
                            if (amp == 0) {
                                ang = 45.0f;
                            } else {
                                ang = 22.5f;
                            }
                        }
                        n = (s32)((f64)amp + (f64)amp * sin(3.141592653589793 * (f64)((f32)sx * ang) / 180.0));
                        for (j = 0; j < n; j++) {
                            stageTrialMapColType[(y * 8 + sx) * 128 + x * 8 + (7 - j)] = nextA;
                        }
                    }
                }
                if (flags & 2) {
                    for (sy = 0; sy < 8; sy++) {
                        if (sy % 4 == 0) {
                            amp = rand8() % 2 + 1;
                            if (amp == 0) {
                                ang = 45.0f;
                            } else {
                                ang = 22.5f;
                            }
                        }
                        n = (s32)((f64)amp + (f64)amp * sin(3.141592653589793 * (f64)((f32)sy * ang) / 180.0));
                        for (j = 0; j < n; j++) {
                            stageTrialMapColType[(y * 8 + (7 - j)) * 128 + x * 8 + sy] = nextB;
                        }
                    }
                }
            }
        }
    }
}

/* clears the height of every map cell within `dist` of pos (flat area), marks lava floor */
static void StageTrialClearDist(Vec *pos, f32 dist)
{
    s32 y, x;
    Vec v;
    f32 mag;

    if (stageTrialClearOff == 0) {
        stageTrialLavaFloorOnF = 1;
        for (y = 0; y < 16; y++) {
            for (x = 0; x < 16; x++) {
                v.x = pos->x - 3.0f * (100.0f * (f32)x);
                v.y = 0.0f;
                v.z = pos->z - 3.0f * (100.0f * (f32)y);
                mag = PSVECMag(&v);
                if (!(dist < mag)) {
                    stageTrialMap[y * 16 + x] &= 0xF0;
                }
            }
        }
    }
}

/* raises the height nibble of the cells around pos (cone, height pos->y) */
static void StageTrialAddSlope(Vec *pos, f32 radius)
{
    s32 y, x;
    s32 h;
    Vec v;
    f32 mag;

    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            v.x = pos->x - 3.0f * (100.0f * (f32)x);
            v.y = 0.0f;
            v.z = pos->z - 3.0f * (100.0f * (f32)y);
            mag = PSVECMag(&v);
            if (!(radius < mag)) {
                h = (s32)((pos->y * MathLerp(1, (radius - mag) / radius)) / stageTrialHeight) + 2;
                if (h >= (stageTrialMap[y * 16 + x] & 0xF)) {
                    stageTrialMap[y * 16 + x] &= 0xF0;
                    stageTrialMap[y * 16 + x] |= h;
                }
            }
        }
    }
}

/* raises the height nibble of the cells along a ramp from `from` to `to` (half width radius) */
static void StageTrialAddSlope2(Vec *from, Vec *to, f32 radius)
{
    s32 y, x;
    s32 h;
    Vec dir;
    Vec cell;
    Mtx rot;
    f32 len;
    f32 deg;
    f32 ax;

    PSVECSubtract(to, from, &dir);
    dir.y = 0.0f;
    len = PSVECMag(&dir);
    PSVECNormalize(&dir, &dir);
    deg = (f32)-(180.0 * (atan2(dir.x, dir.z) / 3.141592653589793));
    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            cell.x = 3.0f * (100.0f * (f32)x);
            cell.y = 0.0f;
            cell.z = 3.0f * (100.0f * (f32)y);
            PSVECSubtract(&cell, from, &dir);
            dir.y = 0.0f;
            PSMTXRotRad(rot, 'y', 0.017453292f * deg);
            PSMTXMultVec(rot, &dir, &dir);
            ax = (dir.x < 0.0f) ? -dir.x : dir.x;
            if (!(ax > radius) && !(dir.z < 0.0f) && !(dir.z > len)) {
                h = (s32)((from->y + (to->y - from->y) * (PSVECMag(&dir) / len)) / stageTrialHeight) + 2;
                if (h >= (stageTrialMap[y * 16 + x] & 0xF)) {
                    stageTrialMap[y * 16 + x] &= 0xF0;
                    stageTrialMap[y * 16 + x] |= h;
                }
            }
        }
    }
}

/* paints the 512x512 ground texture (4x4-tiled RGB565) from the 16x16 type map; border cells
   (flag 0x80) are blended over a 5x5 neighbourhood of the 128x128 type map */
static void MakeTrialStageMesh(void)
{
    u16 *tex[5];
    u8 *colType;
    s32 my, mx, y, x;
    s32 type;
    s32 dx, dy, sx, sy;
    s32 dIdx, sIdx;
    s32 baseX, baseY, subX, subY, gridY, gridX;
    s32 rowTbl[5], colTbl[5];
    s32 i, j, k;
    s32 b[8], g[8], r[8];
    u16 *p;
    u16 pix;
    s32 sumB, sumG, sumR;

    tex[0] = texturePtr;
    for (i = 0; i < 4; i++) {
        tex[i + 1] = (u16 *)Hu3DData[trialModelId[i]].hsf->attribute->bitmap->data;
    }
    colType = stageTrialMapColType;
    for (my = 0; my < 16; my++) {
        for (mx = 0; mx < 16; mx++) {
            baseX = mx * 32;
            baseY = my * 32;
            subX = (mx % 4) * 32;
            subY = (my % 4) * 32;
            gridY = my * 8;
            gridX = mx * 8;
            if ((stageTrialMap[my * 16 + mx] & 0x80) == 0) {
                type = (stageTrialMap[my * 16 + mx] >> 4) & 7;
                if (type != 0) {
                    for (y = 0; y < 32; y++) {
                        for (x = 0; x < 32; x++) {
                            dx = baseX + x % 512;
                            dy = baseY + y % 512;
                            dIdx = dx % 4 + (dy % 4) * 4 + (dy / 4) * 0x800 + (dx / 4) * 16;
                            sx = subX + x % 128;
                            sy = subY + y % 128;
                            sIdx = sx % 4 + (sy % 4) * 4 + (sy / 4) * 0x200 + (sx / 4) * 16;
                            memcpy(&tex[0][dIdx], &tex[type][sIdx], 2);
                        }
                    }
                }
            } else {
                for (y = 0; y < 32; y++) {
                    for (x = 0; x < 32; x++) {
                        dx = baseX + x % 512;
                        dy = baseY + y % 512;
                        dIdx = dx % 4 + (dy % 4) * 4 + (dy / 4) * 0x800 + (dx / 4) * 16;
                        sx = subX + x % 128;
                        sy = subY + y % 128;
                        sIdx = sx % 4 + (sy % 4) * 4 + (sy / 4) * 0x200 + (sx / 4) * 16;
                        for (k = 0; k < 5; k++) {
                            p = (k == 0) ? &tex[0][dIdx] : &tex[k][sIdx];
                            r[k] = (*p >> 8) & 0xF8;
                            g[k] = (*p >> 3) & 0xFC;
                            b[k] = (*p * 8) & 0xF8;
                        }
                        for (k = 0; k < 5; k++) {
                            i = gridY + (y + k - 2) / 4;
                            rowTbl[k] = (i > 0x7F) ? 0x7F : ((i < 0) ? 0 : i);
                            i = gridX + (x + k - 2) / 4;
                            colTbl[k] = (i > 0x7F) ? 0x7F : ((i < 0) ? 0 : i);
                        }
                        sumB = sumG = sumR = 0;
                        for (i = 0; i < 5; i++) {
                            for (j = 0; j < 5; j++) {
                                k = colType[(rowTbl[i] << 7) + colTbl[j]];
                                sumB += b[k];
                                sumG += g[k];
                                sumR += r[k];
                            }
                        }
                        pix = (u16)((((sumB / 25) & 0xF8) >> 3) | ((((sumR / 25) & 0xF8) * 0x100) | (((sumG / 25) & 0xFC) * 8)));
                        memcpy(&tex[0][dIdx], &pix, 2);
                    }
                }
            }
        }
    }
    DCStoreRangeNoSync(tex[0], 0x80000);
}
