#include "REL/m564Dll.h"

void fn_1_9C00(M564_PLAYER *work, s16 mode);
void fn_1_9C20(M564_PLAYER *work);
void fn_1_9D28(M564_PLAYER *work);
void fn_1_9D90(M564_PLAYER *work);
void fn_1_9E30(M564_PLAYER *work);
void fn_1_9EB8(M564_PLAYER *work);
void fn_1_A008(M564_PLAYER *work);
s16 fn_1_A38C(M564_PLAYER *work);
s16 fn_1_A594(M564_PLAYER *work);
s32 fn_1_A678(M564_PLAYER *work);
f32 fn_1_A6C0(M564_PLAYER *work);
s32 fn_1_A8D8(M564_PLAYER *work, s32 chase);
s32 fn_1_AD38(M564_PLAYER *work);
s32 fn_1_B00C(M564_PLAYER *work);
s32 fn_1_B2DC(M564_PLAYER *work);
f32 fn_1_B380(Vec *p, Vec *a, Vec *b, Vec *out);

void fn_1_9AC0(M564_PLAYER *work) {
    work->comMode = 0;
    work->comBomb = -1;
    work->comPlayer = -1;
}

void fn_1_9ADC(M564_PLAYER *work) {
    s16 tbl[4] = { 100, 80, 60, 40 };

    if (work->mgPlayer->mode == 0) {
        if (work->carryF != 0 && lbl_1_bss_1E8[work->bombNo]->timer < tbl[work->varNo]) {
            work->btn = 0x100;
            fn_1_9C00(work, 0);
            return;
        }
        switch (work->comMode) {
            case 0:
                fn_1_9C20(work);
                return;
            case 1:
                fn_1_9D28(work);
                return;
            case 2:
                fn_1_9D90(work);
                return;
            case 3:
                fn_1_9E30(work);
                return;
            case 4:
                fn_1_9EB8(work);
                break;
        }
    }
}

void fn_1_9C00(M564_PLAYER *work, s16 mode) {
    if (mode != work->comMode) {
        work->comMode = mode;
        work->comTimer = 0;
    }
}

void fn_1_9C20(M564_PLAYER *work) {
    if (fn_1_AD38(work) != 0) {
        fn_1_9C00(work, 3);
        return;
    }
    if (fn_1_B00C(work) != 0) {
        fn_1_9C00(work, 2);
        return;
    }
    if (fn_1_B2DC(work) != 0) {
        fn_1_9C00(work, 1);
        return;
    }
    if (work->carryF != 0) {
        fn_1_A008(work);
        return;
    }
    work->comBomb = fn_1_A38C(work);
    if (work->comBomb != -1) {
        fn_1_9C00(work, 4);
    }
}

void fn_1_9D28(M564_PLAYER *work) {
    work->comTimer++;
    fn_1_A8D8(work, 1);
    if (work->comTimer >= 30) {
        fn_1_9C00(work, 0);
    }
}

void fn_1_9D90(M564_PLAYER *work) {
    Vec v;

    work->comTimer++;
    if (work->comTimer >= 10) {
        if (fn_1_902C(&work->pos, &v) != 0) {
            work->comTarget = v;
            work->comTimer = 0;
        } else {
            fn_1_9C00(work, 0);
        }
    }
    fn_1_A8D8(work, 1);
}

void fn_1_9E30(M564_PLAYER *work) {
    if (fn_1_AD38(work) == 0) {
        fn_1_9C00(work, 0);
        return;
    }
    if (fn_1_A8D8(work, 1) != 0) {
        fn_1_9C00(work, 0);
    }
}

void fn_1_9EB8(M564_PLAYER *work) {
    s16 found;

    if (work->varNo >= 2) {
        if (fn_1_AD38(work) != 0) {
            fn_1_9C00(work, 3);
            return;
        }
        if (fn_1_B00C(work) != 0) {
            fn_1_9C00(work, 2);
            return;
        }
    }
    if (fn_1_8D60(work->comBomb) == 0) {
        fn_1_9C00(work, 0);
        return;
    }
    found = fn_1_8340(&work->pos, work->mgPlayer->actor->rotY);
    MgActorPosGet(lbl_1_bss_1E8[work->comBomb]->actor, &work->comTarget);
    if (found != -1) {
        work->comBomb = -1;
        work->btn = 0x100;
        fn_1_9C00(work, 0);
        return;
    }
    fn_1_A8D8(work, 0);
}

void fn_1_A008(M564_PLAYER *work) {
    s32 tbl[4] = { 80, 85, 90, 100 };
    Vec v;
    f32 d;

    if (work->comPlayer == -1) {
        work->comPlayer = fn_1_A594(work);
    }
    if (work->comPlayer != -1) {
        work->comTarget = lbl_1_bss_100[work->comPlayer]->pos;
        d = fn_1_13F0(&work->pos, &work->comTarget, &v);
        if (d >= 170.0f && d <= 450.0f) {
            if ((f32)fabs(fn_1_1338(work->mgPlayer->actor->rotY, (f32)(180.0 * (atan2(v.x, v.z) / M564_PI)))) < 20.0f
                && fn_1_A678(work) != 0 && frandmod(100) < tbl[work->varNo]) {
                work->btn = 0x100;
                work->comPlayer = -1;
                return;
            }
            fn_1_A8D8(work, 0);
        }
        if (d > 450.0f) {
            fn_1_A8D8(work, 0);
        }
    }
}

s32 fn_1_A218(M564_PLAYER *work, s16 bombNo) {
    s16 tbl[4] = { 120, 110, 100, 90 };
    f32 d;
    s32 i;

    if (fn_1_8D60(bombNo) == 0) {
        return 0;
    }
    if (lbl_1_bss_1E8[bombNo]->timer < tbl[work->varNo]) {
        return 0;
    }
    d = fn_1_13A4(&work->pos, &lbl_1_bss_1E8[bombNo]->pos);
    for (i = 0; i < 4; i++) {
        if (work->playerNo != i && fn_1_5E5C(i) == 0) {
            if (fn_1_13A4(&lbl_1_bss_100[i]->pos, &lbl_1_bss_1E8[bombNo]->pos) < d) {
                return 0;
            }
        }
    }
    return 1;
}

s16 fn_1_A38C(M564_PLAYER *work) {
    Vec p;
    f32 d;
    f32 best;
    s16 found;
    s32 i;

    found = -1;
    best = 9999.9f;
    for (i = 0; i < 15; i++) {
        if (fn_1_A218(work, i) != 0) {
            MgActorPosGet(lbl_1_bss_1E8[i]->actor, &p);
            d = fn_1_13A4(&work->pos, &p);
            if (d < best) {
                best = d;
                found = i;
            }
        }
    }
    return found;
}

s16 fn_1_A594(M564_PLAYER *work) {
    f32 d;
    f32 best;
    s16 found;
    s32 i;

    found = -1;
    best = 9999.9f;
    for (i = 0; i < 4; i++) {
        if (work->playerNo != i && fn_1_5E5C(i) == 0) {
            d = fn_1_13A4(&lbl_1_bss_100[i]->pos, &work->pos);
            if (d < best && d > 170.0f) {
                best = d;
                found = i;
            }
        }
    }
    return found;
}

s32 fn_1_A678(M564_PLAYER *work) {
    if (fn_1_A6C0(work) < 120.0f) {
        return 0;
    }
    return 1;
}

f32 fn_1_A6C0(M564_PLAYER *work) {
    f32 d;
    f32 best;
    s32 i;

    best = 9999.9f;
    for (i = 0; i < 4; i++) {
        if (i != work->playerNo && fn_1_5E5C(i) == 0) {
            d = fn_1_13A4(&work->pos, &lbl_1_bss_100[i]->pos);
            if (d < best) {
                best = d;
            }
        }
    }
    return best;
}

void fn_1_A780(M564_PLAYER *work, s16 playerNo) {
    Vec v;

    fn_1_13F0(&work->pos, &lbl_1_bss_100[playerNo]->pos, &v);
    work->mgPlayer->actor->rotY = (f32)(180.0 * (atan2(v.x, v.z) / M564_PI)) + 100.0f;
}

void fn_1_A830(M564_PLAYER *work, s16 playerNo) {
    Vec v;

    fn_1_13F0(&work->pos, &lbl_1_bss_100[playerNo]->pos, &v);
    PSVECNormalize(&v, &v);
    PSVECScale(&v, &v, 101.0f);
    PSVECAdd(&lbl_1_bss_100[playerNo]->pos, &v, &work->comTarget);
}

s32 fn_1_A8D8(M564_PLAYER *work, s32 chase) {
    f32 scale[4] = { 0.8f, 0.9f, 1.0f, 1.0f };
    Vec v;
    f32 cur;
    f32 diff;
    f32 rot;
    f32 dist;
    f32 ang;

    dist = fn_1_13F0(&work->pos, &work->comTarget, &v);
    if (dist < 10.0f) {
        return 1;
    }
    if (chase != 0) {
        cur = work->mgPlayer->actor->rotY;
        ang = fn_1_12D4(&work->pos, &work->comTarget);
        diff = fn_1_1338(cur, ang);
        if (fabs(diff) < 15.0) {
            rot = cur + diff;
        } else if (diff < 0.0f) {
            rot = cur - 15.0f;
        } else {
            rot = cur + 15.0f;
        }
        v.x = (f32)sin(M564_RAD(rot));
        v.z = (f32)cos(M564_RAD(rot));
        v.y = 0.0f;
    } else {
        PSVECNormalize(&v, &v);
    }
    work->stkX = (s32)(56.0f * v.x * scale[work->varNo]);
    work->stkY = (s32)(56.0f * -v.z * scale[work->varNo]);
    return 0;
}

s32 fn_1_AB2C(M564_PLAYER *work, M564_PLAYER *other) {
    Vec end;
    Vec closest;
    Vec dir;
    f32 ang;
    f32 d;

    if (other->carryF == 0) {
        return 0;
    }
    ang = other->mgPlayer->actor->rotY;
    end.x = (f32)((f64)other->pos.x + (550.0 * sin(M564_RAD(ang))));
    end.z = (f32)((f64)other->pos.z + (550.0 * cos(M564_RAD(ang))));
    end.y = other->pos.y;
    d = fn_1_B380(&work->pos, &other->pos, &end, &closest);
    if (d > 150.0f || d < 0.0f) {
        return 0;
    }
    PSVECSubtract(&work->pos, &closest, &dir);
    if (PSVECMag(&dir) == 0.0f) {
        return 0;
    }
    PSVECNormalize(&dir, &dir);
    PSVECScale(&dir, &dir, 150.0f);
    PSVECAdd(&work->pos, &dir, &work->comTarget);
    work->comTarget.y = 0.0f;
    if (PSVECMag(&work->comTarget) > 425.0f) {
        PSVECScale(&dir, &dir, 10.0f);
        PSVECSubtract(&closest, &dir, &work->comTarget);
    }
    return 1;
}

s32 fn_1_AD38(M564_PLAYER *work) {
    s32 tbl[4] = { 0, 30, 60, 100 };
    s32 i;

    if (frandmod(100) < tbl[work->varNo]) {
        for (i = 0; i < 4; i++) {
            if (work->playerNo != i && fn_1_5E5C(i) == 0) {
                if (fn_1_AB2C(work, lbl_1_bss_100[i]) != 0) {
                    fn_1_9C00(work, 3);
                    return 1;
                }
            }
        }
    }
    return 0;
}

s32 fn_1_B00C(M564_PLAYER *work) {
    s32 tbl[4] = { 60, 80, 90, 100 };
    Vec v;

    if (frandmod(100) < tbl[work->varNo] && fn_1_902C(&work->pos, &v) != 0) {
        work->comTarget = v;
        fn_1_A8D8(work, 1);
        return 1;
    }
    return 0;
}

s32 fn_1_B2DC(M564_PLAYER *work) {
    s32 tbl[4] = { 45, 30, 10, 0 };

    if (frandmod(100) < tbl[work->varNo]) {
        fn_1_118C(&lbl_1_data_D8, &work->comTarget, 0.0f, 400.0f);
        return 1;
    }
    return 0;
}

f32 fn_1_B380(Vec *p, Vec *a, Vec *b, Vec *out) {
    Vec ab;
    Vec ap;
    Vec proj;
    Vec perp;
    f32 len2;
    f32 t;

    PSVECSubtract(b, a, &ab);
    PSVECSubtract(p, a, &ap);
    len2 = PSVECSquareMag(&ab);
    t = PSVECDotProduct(&ab, &ap) / len2;
    if (t < 0.0f || t > 1.0f) {
        return -1.0f;
    }
    PSVECScale(&ab, &proj, t);
    PSVECAdd(a, &proj, out);
    PSVECSubtract(&proj, &ap, &perp);
    return PSVECMag(&perp);
}
