#include "REL/m534Dll.h"

static void fn_1_1A74(OMOBJ *obj);
static void fn_1_1F1C(OMOBJ *obj);
static void fn_1_2130(s32 from, s32 to, f32 t);

typedef struct M534CamKey_s {
    HuVecF center; /* 0x00 */
    HuVecF rot;    /* 0x0C: x/y rotation, z = zoom */
} M534CAMKEY;

M534CAMKEY lbl_1_data_50[6] = {
    { { 0.0f, 0.0f, -800.0f }, { -60.0f, 0.0f, 3200.0f } },
    { { 0.0f, 0.0f, -170.0f }, { -25.0f, 0.0f, 1000.0f } },
    { { 0.0f, 0.0f, -520.0f }, { -45.0f, 0.0f, 800.0f } },
    { { 0.0f, 200.0f, 0.0f }, { -2.5f, 0.0f, 810.0f } },
    { { -170.0f, 100.0f, 0.0f }, { -2.5f, 0.0f, 450.0f } },
    { { 170.0f, 100.0f, 0.0f }, { -2.5f, 0.0f, 450.0f } },
};

s32 lbl_1_bss_28;
s32 lbl_1_bss_2C;

s32 fn_1_1840(f32 x)
{
    s32 vol;

    x *= 0.11111111f;
    vol = 32.0f + x;

    if (vol < 32) {
        vol = 32;
    }
    if (vol > 96) {
        vol = 96;
    }
    return vol;
}

s32 fn_1_189C(HuVecF *pos)
{
    HuVecF scr;

    Hu3D3Dto2D(pos, 1, &scr);
    return fn_1_1840(scr.x);
}

void fn_1_1930(s32 seId, HuVecF *pos)
{
    HuAudFXPlayPan(seId, fn_1_189C(pos));
}

void (*fn_1_19E8(HUPROCESS *proc))(OMOBJ *)
{
    M534CAMKEY *k = lbl_1_data_50;

    Center = k->center;
    CRot.x = k->rot.x;
    CRot.y = k->rot.y;
    CRot.z = 0.0f;
    CZoom = k->rot.z;
    return fn_1_1A74;
}

static void fn_1_1A74(OMOBJ *obj)
{
    M534MAIN *m = obj->data;
    u32 delay;
    u32 next;
    s32 to;
    s32 from;
    f32 t;
    f32 amp;
    f32 dur;

    from = -1;
    to = -1;
    delay = 0;
    dur = 1.0f;
    next = 7;
    amp = 0.0f;
    fn_1_444(obj);
    switch (m->camPhase) {
        case 0:
            lbl_1_bss_28 = -1;
        case 1:
            from = 1;
            to = 1;
            delay = 60;
            next = 2;
            break;
        case 2:
            next = 3;
            break;
        case 3:
            from = 1;
            to = 2;
            delay = 30;
            dur = 24.0f;
            next = 4;
            break;
        case 4:
            to = 2;
            from = to;
            amp = (f32) (f32) m->camPhase;
            if (amp > 10.0) {
                amp = 10.0f;
            }
            delay = 42;
            next = 5;
            if (lbl_1_bss_28 == -1) {
                lbl_1_bss_28 = HuAudFXPlay(0x7BB);
            }
            break;
        case 5:
            to = 2;
            from = to;
            amp = 20.0f;
            delay = 12;
            next = 6;
            if (lbl_1_bss_28 != -1) {
                HuAudFXFadeOut(lbl_1_bss_28, 30);
                lbl_1_bss_28 = -1;
            }
            break;
        case 6:
            from = 2;
            to = 3;
            delay = 12;
            dur = 60.0f;
            next = 7;
            break;
        case 7:
            fn_1_518(obj);
        default:
            return;
    }
    if (m->camTimer < delay) {
        t = 0.0f;
    } else {
        t = (1.0f / dur) * (f32) (m->camTimer - delay);
        if (t >= 1.0) {
            t = 1.0f;
        } else {
            t = sin(1.5707963267948966 * t);
        }
    }
    if (t >= 1.0) {
        m->camPhase = next;
        m->camTimer = 0;
    }
    if (from >= 0) {
        fn_1_2130(from, to, t);
    }
    if (amp > 0.0f) {
        t = 1.5707963267948966 * (f32) (m->camTimer & 3);
        Center.x = Center.x + amp * sin(t);
        Center.z = Center.z + amp * cos(t);
    }
}

void (*fn_1_1DF4(HUPROCESS *proc, OMOBJ *obj))(OMOBJ *)
{
    M534MAIN *m = obj->data;
    s32 i;

    switch (fn_1_4624()) {
        case 1:
            m->mode = 1;
            break;
        case -1:
            m->mode = 2;
            break;
        default:
        case 0:
            m->mode = 0;
            break;
    }
    for (i = 0; i < 4; i++) {
        m->unk44[i] = -1;
        if ((m->mode == 1 && m->unk24[i] == 0) || (m->mode == 2 && m->unk24[i] == 1)) {
            m->unk44[i] = m->unk34[i];
        }
    }
    m->endPhase = 1;
    m->endTimer = 0;
    lbl_1_bss_2C = -1;
    return fn_1_1F1C;
}

static void fn_1_1F1C(OMOBJ *obj)
{
    M534MAIN *m = obj->data;
    s32 i;

    fn_1_444(obj);
    switch (m->endPhase) {
        case 1:
            if (fn_1_49BC() != 0) {
                m->endTimer = 0;
                if (m->mode == 0) {
                    lbl_1_bss_2C = GameMesCreate(2, 2);
                    m->endPhase = 4;
                    HuAudJinglePlay(0x7B);
                    return;
                }
                m->endPhase = 2;
            }
            break;
        case 2:
            m->endPhase = 3;
            m->endTimer = 0;
            break;
        case 3:
            if (!((f64) m->endTimer < 12.0)) {
                lbl_1_bss_2C = GameMesCreate(4, 3, m->unk44[0], m->unk44[1], m->unk44[2], m->unk44[3]);
                m->endPhase = 4;
                m->endTimer = 0;
                for (i = 0; i < 4; i++) {
                    if (m->unk44[i] >= 0) {
                        GWMgCoinBonusSet(i, GWMgCoinBonusGet(i) + 10);
                    }
                }
                HuAudJinglePlay(0x79);
            }
            break;
        case 4:
            if (!((f64) m->endTimer < 210.0)) {
                m->endPhase = 5;
                fn_1_518(obj);
            }
            break;
    }
}

static void fn_1_2130(s32 from, s32 to, f32 t)
{
    HuVecF tmp;
    M534CAMKEY *a;
    M534CAMKEY *b;

    if (from < 0) {
        if (to < 0) {
            return;
        }
        from = to;
    }
    if (to < 0) {
        to = from;
    }
#ifdef TARGET_PC
    if (from > 5) {
        from = 5;
    }
    if (to > 5) {
        to = 5;
    }
#endif
    a = &lbl_1_data_50[from];
    b = &lbl_1_data_50[to];
    PSVECSubtract(&b->center, &a->center, &tmp);
    PSVECScale(&tmp, &tmp, t);
    PSVECAdd(&a->center, &tmp, &tmp);
    Center = tmp;
    PSVECSubtract(&b->rot, &a->rot, &tmp);
    PSVECScale(&tmp, &tmp, t);
    PSVECAdd(&a->rot, &tmp, &tmp);
    CRot.x = tmp.x;
    CRot.y = tmp.y;
    CRot.z = 0.0f;
    CZoom = tmp.z;
}
