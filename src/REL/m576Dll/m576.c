#include "REL/m576Dll.h"

typedef void (*M576VoidFunc)(void);
extern const M576VoidFunc _ctors[];
extern const M576VoidFunc _dtors[];

double fmod(double x, double y);
double fabs(double x);
float sqrtf(float x);

/* stage.c / score.c */
void fn_1_BAF4(s32 idx, s32 val);
void fn_1_C16C(s16 mdlId);
void **fn_1_C1EC(MGPLAYER *player, s32 idx);
void fn_1_C31C(void **work, s32 idx);
void fn_1_EA08(OMOBJMAN *om);
void fn_1_EEC0(s32 idx);
void fn_1_EEF8(s32 idx);

void ObjectSetup(void);
void fn_1_32CC(OMOBJ *obj);
void fn_1_48A0(s16 mode, s16 frameNo);
void fn_1_4920(s16 mode, s16 frameNo);
void fn_1_5288(s16 mode, s16 frameNo);
void fn_1_55C8(s16 mode, s16 frameNo);
void fn_1_5C04(s16 mode, s16 frameNo);
void fn_1_5EAC(s16 mode, s16 frameNo);
void fn_1_6730(s16 mode, s16 frameNo);
void fn_1_6734(s16 mode, s16 frameNo);
void fn_1_6764(s16 mode, s16 frameNo);
void fn_1_FA0(MGACTOR *actor, int param);
s32 fn_1_1550(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b);

typedef struct M576ColSet_s {
    GXColor start[2];
    GXColor end[2];
} M576ColSet;

/* unit-owned .bss, declared in reverse address order */
MGPLAYER *lbl_1_bss_2FC[M576_COIN_MAX];
void **lbl_1_bss_2EC[M576_PLAYER_MAX];
MGACTOR *lbl_1_bss_2D8[M576_COIN_MAX];
MGACTOR *lbl_1_bss_2C8[M576_PLAYER_MAX];
s8 lbl_1_bss_2C4[4];
s32 lbl_1_bss_2B4[4];
s32 lbl_1_bss_2A0[M576_COIN_MAX];
s32 lbl_1_bss_28C[M576_COIN_MAX];
s32 lbl_1_bss_254[14];
HuVecF lbl_1_bss_23C[2];
HuVecF lbl_1_bss_20C[M576_PLAYER_MAX];
HU3DPARMANPARAM lbl_1_bss_7C[M576_COIN_MAX];
HuVecF lbl_1_bss_58[3];
s32 lbl_1_bss_54;
s32 lbl_1_bss_40[M576_COIN_MAX];
s8 lbl_1_bss_3C[4];
s32 lbl_1_bss_28[M576_COIN_MAX];
s32 lbl_1_bss_24;
s32 lbl_1_bss_20;
s32 lbl_1_bss_1C;
s32 lbl_1_bss_18;
s32 lbl_1_bss_14;
s32 lbl_1_bss_10;
s32 lbl_1_bss_C;
s32 lbl_1_bss_8;
OMOBJMAN *lbl_1_bss_4;
OMOBJ *lbl_1_bss_0;

MGSEQ_PARAM lbl_1_data_0 = {
    0x2D,
    0,
    fn_1_48A0,
    fn_1_4920,
    fn_1_5288,
    fn_1_55C8,
    fn_1_5C04,
    fn_1_5EAC,
    fn_1_6730,
    fn_1_6734,
    fn_1_6764,
};

HuVecF lbl_1_data_28[4] = {
    { -300.0f, 1400.0f, 400.0f },
    { 300.0f, 1400.0f, 400.0f },
    { -300.0f, 1400.0f, 600.0f },
    { 300.0f, 1400.0f, 600.0f },
};

HuVecF lbl_1_data_58[4] = {
    { -350.0f, 1400.0f, -280.0f },
    { 450.0f, 1400.0f, -400.0f },
    { 320.0f, 1400.0f, 400.0f },
    { -500.0f, 1400.0f, 500.0f },
};

HuVecF lbl_1_data_88[5] = {
    { 0.0f, 620.0f, -700.0f },
    { -700.0f, 820.0f, -700.0f },
    { 700.0f, 720.0f, -100.0f },
    { -700.0f, 420.0f, 700.0f },
    { 700.0f, 420.0f, 700.0f },
};

HuVecF lbl_1_data_C4[2] = {
    { -700.0f, 300.0f, 450.0f },
    { 300.0f, 500.0f, -650.0f },
};

HuVecF lbl_1_data_DC[14] = {
    { 0.0f, 520.0f, -650.0f },
    { -500.0f, 0.0f, 550.0f },
    { 250.0f, 0.0f, 700.0f },
    { 580.0f, 200.0f, -580.0f },
    { -500.0f, 300.0f, -400.0f },
    { -730.0f, 300.0f, 300.0f },
    { -670.0f, 300.0f, 700.0f },
    { 140.0f, 500.0f, -650.0f },
    { 700.0f, 550.0f, -100.0f },
    { -700.0f, 300.0f, 450.0f },
    { 480.0f, 500.0f, -650.0f },
    { 600.7f, 500.0f, -470.0f },
    { -300.0f, 300.0f, 750.0f },
    { 200.0f, 500.0f, -100.0f },
};

HuVecF lbl_1_data_184[4] = {
    { -625.0f, 300.0f, 675.0f },
    { -710.0f, 300.0f, 740.0f },
    { 725.0f, 600.0f, -175.0f },
    { 625.0f, 600.0f, -25.0f },
};

HU3DPARMANPARAM lbl_1_data_1B4 = {
    40,
    0,
    0.3f,
    30.0f,
    10.0f,
    { 0.0f, -0.2f, 0.0f },
    2.0f,
    1.0f,
    70.0f,
    0.98f,
    2,
    { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 255, 255, 255, 0 }, { 255, 255, 255, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};

M576ColSet lbl_1_data_204[5] = {
    { { { 0xFF, 0x80, 0x80, 0xFF }, { 0xFF, 0x40, 0x40, 0xFF } }, { { 0xFF, 0x00, 0x00, 0x00 }, { 0x80, 0x40, 0x40, 0x00 } } },
    { { { 0xA0, 0xA0, 0xFF, 0xFF }, { 0x80, 0x80, 0xFF, 0xFF } }, { { 0x40, 0x40, 0xFF, 0x00 }, { 0x80, 0x80, 0xFF, 0x00 } } },
    { { { 0x80, 0xFF, 0x80, 0xFF }, { 0x40, 0xFF, 0x40, 0xFF } }, { { 0x00, 0xFF, 0x00, 0x00 }, { 0x40, 0xFF, 0x40, 0x00 } } },
    { { { 0xFF, 0xFF, 0x80, 0xFF }, { 0xFF, 0xFF, 0x40, 0xFF } }, { { 0xA0, 0xA0, 0x00, 0x00 }, { 0xA0, 0xA0, 0x40, 0x00 } } },
    { { { 0xC0, 0xC0, 0xFF, 0xFF }, { 0xA0, 0xA0, 0xFF, 0xFF } }, { { 0x40, 0x40, 0x80, 0x00 }, { 0xA0, 0xA0, 0xFF, 0x00 } } },
};

s32 lbl_1_data_254[10] = { 0x200, 0x1EC, 0x264, 0x2B4, 0x28C, 0x1C4, 0x2A0, 0x1D8, 0x278, 0x23C };

HuVecF lbl_1_data_27C[4] = {
    { -300.0f, 0.0f, 400.0f },
    { -100.0f, 0.0f, 400.0f },
    { 100.0f, 0.0f, 400.0f },
    { 300.0f, 0.0f, 400.0f },
};

int _prolog(void)
{
    const M576VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const M576VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

f32 fn_1_A0(f32 angle, f32 dead)
{
    f32 mod = fmod(angle, 180.0);
    f32 ret;

    if (fabs(mod) < dead) {
        return 0.0f;
    }
    if (mod > 0.0f) {
        ret = mod - dead;
    } else {
        ret = mod + dead;
    }
    return ret;
}

s32 fn_1_174(s32 idx, HuVecF *dest)
{
    if (dest != NULL) {
        *dest = lbl_1_data_DC[idx];
    }
    return lbl_1_bss_254[idx];
}

s32 fn_1_1BC(HuVecF *dest)
{
    *dest = lbl_1_bss_23C[0];
    return (f32)lbl_1_bss_C <= 71.42857f;
}

s32 fn_1_234(HuVecF *dest)
{
    *dest = lbl_1_bss_23C[1];
    return (f32)lbl_1_bss_10 <= 100.0f;
}

void fn_1_2AC(HuVecF *pos, s32 range)
{
    pos->x += (f32)frandmod(range * 20) / 10.0f - (f32)range;
    pos->y += (f32)frandmod(range * 20) / 10.0f - (f32)range;
    pos->z += (f32)frandmod(range * 20) / 10.0f - (f32)range;
}

s8 fn_1_424(s32 idx)
{
    return lbl_1_bss_2FC[idx]->stunType;
}

s32 fn_1_444(HuVecF *dir, HuVecF *out)
{
    s32 ret;

    if (PSVECSquareMag(dir) < 1e-06) {
        out->x = 0.01f * ((f32)frandmod(20) - 10.0f);
        out->z = 0.01f * ((f32)frandmod(20) - 10.0f);
        if (frandmod(2) != 0) {
            out->y = 0.01f;
        } else {
            out->y = -0.01f;
        }
        PSVECNormalize(out, out);
        ret = 0;
    } else {
        PSVECNormalize(dir, out);
        ret = 1;
    }
    return ret;
}

s32 fn_1_59C(void)
{
    s32 count = 0;
    s32 i = 5;

    while (i--) {
        if (lbl_1_bss_2A0[i] == -1) {
            count++;
        }
    }
    return count;
}

s32 fn_1_5F4(s32 idx, HuVecF *pos)
{
    if (pos != NULL) {
        MgActorPosGet(lbl_1_bss_2D8[idx - 8], pos);
    }
    return lbl_1_bss_2A0[idx - 8];
}

s32 fn_1_668(MGPLAYER **player, HuVecF *outPos, f32 *outDist)
{
    HuVecF diff;
    f32 minDist = 1e10f;
    s32 best = -1;
    s32 i = 5;

    while (i--) {
        if (lbl_1_bss_2A0[i] == -1) {
            f32 dist;
            PSVECSubtract(&(*player)->actor->pos, &lbl_1_bss_2D8[i]->pos, &diff);
            dist = PSVECSquareMag(&diff);
            if (minDist > dist) {
                best = i + 8;
                minDist = dist;
            }
        }
    }
    if (best >= 0) {
        if (outPos != NULL) {
            *outPos = lbl_1_bss_2D8[best - 8]->pos;
        }
        if (outDist != NULL) {
            *outDist = minDist;
        }
    }
    return best;
}

s32 fn_1_798(MGPLAYER **player)
{
    s32 count = 0;
    s32 i = 5;

    while (i--) {
        if ((*player)->actor->param == lbl_1_bss_2A0[i]) {
            count++;
        }
    }
    return count;
}

s32 fn_1_7FC(s32 playerNo)
{
    s32 count = 0;
    s32 i = 5;

    while (i--) {
        if (playerNo == lbl_1_bss_2A0[i]) {
            count++;
        }
    }
    return count;
}

s32 fn_1_854(s32 playerNo)
{
    s32 count = 0;
    s32 i = 5;

    while (i--) {
        if (playerNo == lbl_1_bss_2A0[i]) {
            count++;
        }
    }
    return count;
}

s32 fn_1_8AC(s32 *outPlayer, HuVecF *outPos)
{
    s32 count;
    s32 maxCount;
    s32 i;
    s32 j;
#ifdef TARGET_PC
    s32 best = 0;
#else
    s32 best;
#endif

    maxCount = 0;
    for (i = 0; i < 4; i++) {
        count = 0;
        for (j = 0; j < 5; j++) {
            if (i == lbl_1_bss_2A0[j]) {
                count++;
            }
        }
        if (frandmod(2) != 0) {
            if (maxCount <= count) {
                best = i;
                maxCount = count;
            }
        } else if (maxCount < count) {
            best = i;
            maxCount = count;
        }
    }
    if (maxCount != 0) {
        if (outPlayer != NULL) {
            *outPlayer = best;
        }
        if (outPos != NULL) {
            MgActorPosGet(lbl_1_bss_2FC[best]->actor, outPos);
        }
    }
    return maxCount;
}

void fn_1_9A4(s32 idx, HuVecF *pos)
{
    MgActorPosGet(lbl_1_bss_2FC[idx]->actor, pos);
}

s32 fn_1_9EC(s32 idx)
{
    if (lbl_1_bss_2B4[0] == idx || lbl_1_bss_2B4[1] == idx) {
        return 1;
    }
    if (lbl_1_bss_2B4[2] == idx || lbl_1_bss_2B4[3] == idx) {
        return 2;
    }
    return 0;
}

u32 fn_1_A54(s32 idx)
{
    if (idx < 8) {
        if (idx < 4) {
            if (idx >= 0) {
                return lbl_1_bss_2FC[idx]->actor->colGroundAttr;
            }
        }
    } else if (idx < 13) {
        return lbl_1_bss_2D8[idx - 8]->colGroundAttr;
    }
    return 0;
}

void fn_1_AC4(void)
{
    MGPLAYER *player;
    s32 i;

    while (1) {
        player = lbl_1_bss_2FC[0];
        if (lbl_1_bss_14 == 2) {
            i = 4;
            while (i--) {
                CharModelVoicePanSet(player->charNo, (s16)(64.0f + 16.0f * (player->actor->pos.x / 300.0f)));
                CharModelVoiceVolSet(player->charNo, 127);
                player++;
            }
        } else {
            i = 4;
            while (i--) {
                CharModelVoicePanSet(player->charNo, (s16)(64.0f + 16.0f * (player->actor->pos.x / 700.0f)));
                CharModelVoiceVolSet(player->charNo, (s16)(111.0f + 16.0f * (player->actor->pos.z / 700.0f)));
                player++;
            }
        }
        HuPrcVSleep();
    }
}

void fn_1_C24(void)
{
    HuPrcChildCreate(fn_1_AC4, 0xC8, 0x1000, 0, HuPrcCurrentGet());
}

void ObjectSetup(void)
{
    HuVecF lightPos = { 0.0f, 8000.0f, 0.0f };
    HuVecF shadowUp = { 0.0f, 0.0f, 1.0f };
    HuVecF shadowTarget = { 0.0f, 0.0f, 0.0f };
    HuVecF lightDir = { 0.0f, -1.0f, 0.0f };
    s16 light;

    lbl_1_bss_4 = MgActorObjectSetup();
    CRot.x = 270.0f;
    CRot.y = 0.0f;
    CRot.z = 0.0f;
    Center.x = 0.0f;
    Center.y = 300.0f;
    Center.z = -50.0f;
    CZoom = 5000.0f;
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 20.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    light = Hu3DGLightCreate(0.0f, 1000.0f, 1000.0f, 0.0f, -1.0f, -1.0f, 255, 255, 255);
    Hu3DGLightStaticSet(light, 1);
    Hu3DGLightPosSetV(light, &lightPos, &lightDir);
    Hu3DShadowCreate(15.0f, 20.0f, 10000.0f);
    Hu3DShadowTPLvlSet(0.5f);
    Hu3DShadowPosSet(&lightPos, &shadowUp, &shadowTarget);
    Hu3DShadowSizeSet(240);
    lbl_1_bss_0 = omAddObjEx(lbl_1_bss_4, 16, 20, 0, -1, fn_1_32CC);
    lbl_1_bss_8 = 0;
    lbl_1_bss_C = 0;
    lbl_1_bss_10 = 0;
    lbl_1_bss_14 = 0;
    MgSeqCreate(&lbl_1_data_0);
    MgSeqModeDelaySet(7, -1);
}

void fn_1_FA0(MGACTOR *actor, int param)
{
    HuVecF up;
    s32 id;
    s32 face;

    id = actor->param;
    if (id < 13 && (actor->colGroundAttr & 0x407F) != 0) {
        if (actor->colMesh == 0) {
            face = actor->colFace;
        } else {
            face = -1;
        }
        fn_1_BAF4(id, face);
        if (id < 8) {
            if (id < 4) {
                return;
            }
            up.x = up.z = 0.0f;
            up.y = 1.0f;
            if (!(PSVECDotProduct(&actor->colNorm, &up) < 0.1)) {
                MgActorColAttrSet(actor, 2);
                actor->gravity = 0.0f;
                memset(&actor->forceA, 0, sizeof(HuVecF));
                actor->velY = 0.0f;
                Hu3DModelAttrReset(actor->mdlId, 0x40000002);
                HuAudFXStop(lbl_1_bss_40[id - 4]);
                HuAudFXPlayVolPan(0x93B, (s16)(111.0f + 16.0f * (lbl_1_data_58[id - 4].z / 700.0f)),
                    (s16)(64.0f + 16.0f * (lbl_1_data_58[id - 4].x / 700.0f)));
            }
        } else if (id < 13) {
        }
    }
}

MGACTOR *fn_1_1194(s32 playerNo, HuVecF *pos)
{
    MGACTOR *coin = NULL;
    s32 i;

    if (lbl_1_bss_18 != 0) {
        return NULL;
    }
    i = 5;
    while (i--) {
        if (playerNo == lbl_1_bss_2A0[i]) {
            Hu3DParManAttrSet(lbl_1_bss_28C[i], 1);
            lbl_1_bss_2A0[i] = -1;
            coin = lbl_1_bss_2D8[i];
            break;
        }
    }
    if (coin == NULL) {
        return NULL;
    }
    Hu3DModelAttrReset(coin->mdlId, 1);
    MgActorColAttrReset(coin, 2);
    MgActorColAttrSet(coin, 0x01000004);
    coin->gravity = 100.0f;
    MgActorPosSet(coin, pos);
    coin->velY = 100.0f + 100.0f * ((-1.0f + sqrtf(3201.0f)) * 0.5f); /* frame/FPR allocation differs */
    coin->forceA.x = (f32)frandmod(200) / 10.0f - 10.0f;
    coin->forceA.y = 0.0f;
    coin->forceA.z = (f32)frandmod(200) / 10.0f - 10.0f;
    fn_1_EEF8(lbl_1_bss_2FC[playerNo]->playerNo);
    HuAudFXPlayVolPan(0x939, (s16)(111.0f + 16.0f * (coin->pos.x / 700.0f)), (s16)(64.0f + 16.0f * (coin->pos.x / 700.0f)));
    return coin;
}
