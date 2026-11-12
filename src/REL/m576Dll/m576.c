#include "REL/m576Dll.h"

typedef void (*M576VoidFunc)(void);
extern const M576VoidFunc _ctors[];
extern const M576VoidFunc _dtors[];

double fmod(double x, double y);
double fabs(double x);
double acos(double x);
float sqrtf(float x);

/* stage.c / score.c */
void fn_1_BAF4(s32 idx, s32 val);
void fn_1_C16C(s32 mdlId);
M576COM *fn_1_C1EC(MGPLAYER *player);
void fn_1_C31C(M576COM *com);
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
int fn_1_1550(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b);

typedef struct M576ColSet_s {
    GXColor start[2];
    GXColor end[2];
} M576ColSet;

/* unit-owned .bss, declared in reverse address order */
MGPLAYER *lbl_1_bss_2FC[M576_COIN_MAX];
M576COM *lbl_1_bss_2EC[M576_PLAYER_MAX];
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
    if (dest != 0) {
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
    if (pos != 0) {
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
        if (outPos != 0) {
            *outPos = lbl_1_bss_2D8[best - 8]->pos;
        }
        if (outDist != 0) {
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
        if (outPlayer != 0) {
            *outPlayer = best;
        }
        if (outPos != 0) {
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

MGACTOR *fn_1_1194(s32 playerNo, HuVecF pos)
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
    MgActorPosSet(coin, &pos);
    coin->velY = 100.0f + 100.0f * ((-1.0f + sqrtf(3201.0f)) * 0.5f); /* frame/FPR allocation differs */
    coin->forceA.x = (f32)frandmod(200) / 10.0f - 10.0f;
    coin->forceA.y = 0.0f;
    coin->forceA.z = (f32)frandmod(200) / 10.0f - 10.0f;
    fn_1_EEF8(lbl_1_bss_2FC[playerNo]->playerNo);
    HuAudFXPlayVolPan(0x939, (s16)(111.0f + 16.0f * (coin->pos.x / 700.0f)), (s16)(64.0f + 16.0f * (coin->pos.x / 700.0f)));
    return coin;
}

int fn_1_1550(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b)
{
    MGPLAYER *player;
    MGACTOR *coin;
    HuVecF pos;
    HuVecF dir;
    s32 ok;
    s32 idx;
    s32 i;
    f32 angle;

    switch (b->type) {
        case 0:
        case 1:
            player = lbl_1_bss_2FC[a->paramA];
            if (MgPlayerVibAttrCheck(player, 1)) {
                MgActorPosGet(player->actor, &pos);
                coin = fn_1_1194(a->paramA, pos);
            }
            break;
        case 2:
            player = lbl_1_bss_2FC[a->paramA];
            if (a->paramA == (u8)b->paramB) {
                return 0;
            }
            if (MgPlayerVibAttrCheck(player, 0x40) == 0) {
                return 0;
            }
            MgActorPosGet(player->actor, &pos);
            coin = fn_1_1194(a->paramA, pos);
            break;
        case 3:
            player = lbl_1_bss_2FC[a->paramA];
            if (a->paramA == (u8)b->paramB) {
                return 0;
            }
            if (MgPlayerVibAttrCheck(player, 0x80) == 0) {
                return 0;
            }
            MgActorPosGet(player->actor, &pos);
            coin = fn_1_1194(a->paramA, pos);
            break;
        case 4:
            player = lbl_1_bss_2FC[a->paramA];
            if (a->paramA == (u8)b->paramB) {
                return 0;
            }
            if (MgPlayerVibAttrCheck(player, 0x100) == 0) {
                return 0;
            }
            MgActorPosGet(player->actor, &pos);
            coin = fn_1_1194(a->paramA, pos);
            coin = fn_1_1194(a->paramA, pos);
            break;
        case 5:
            player = lbl_1_bss_2FC[a->paramA];
            if (lbl_1_bss_14 == 0) {
                idx = b->paramB - 8;
                coin = lbl_1_bss_2D8[idx];
                if (lbl_1_bss_2A0[idx] == -1) {
                    fn_1_EEC0(player->playerNo);
                    lbl_1_bss_2A0[idx] = a->paramB;
                    fn_1_BAF4(b->paramB, -1);
                    MgActorColAttrSet(coin, 6);
                    Hu3DModelAttrSet(coin->mdlId, 1);
                    MgActorPosGet(coin, &pos);
                    pos.y += 115.0f;
                    i = lbl_1_bss_8;
                    while (i--) {
                        if (a->paramB == lbl_1_bss_2EC[i]->player->actor->param) {
                            lbl_1_bss_2EC[i]->timer30 = 0;
                            break;
                        }
                    }
                    Hu3DMotionTimeSet(lbl_1_bss_0->mdlId[idx + 15], 0.0f);
                    Hu3DModelAttrReset(lbl_1_bss_0->mdlId[idx + 15], 1);
                    Hu3DModelAttrReset(lbl_1_bss_0->mdlId[idx + 15], 0x40000002);
                    Hu3DModelPosSet(lbl_1_bss_0->mdlId[idx + 15], pos.x, pos.y, pos.z);
                    HuAudFXPlayVolPan(0x938, (s16)(111.0f + 16.0f * (pos.z / 700.0f)), (s16)(64.0f + 16.0f * (pos.x / 700.0f)));
                    omVibrate(player->playerNo, 12, 4, 2);
                    return 1;
                }
            }
#ifdef TARGET_PC
            return 0;
#else
            return (s32)a;
#endif
        case 6:
            if (a->paramA >= 4) {
                return 1;
            }
            player = lbl_1_bss_2FC[a->paramA];
            PSVECSubtract(&a->point, &b->point, &dir);
            ok = fn_1_444(&dir, &dir);
            if (dir.z > 1.0f) {
                dir.z = 1.0f;
            } else if (dir.z < -1.0f) {
                dir.z = -1.0f;
            }
            angle = acos(dir.z);
            if (dir.x < 0.0f) {
                angle = -angle;
            }
            if (MgPlayerStunSet(player, 1, (f32)(180.0f * angle / 3.141592653589793), -1) == 0) {
                return 1;
            }
            MgActorPosGet(player->actor, &pos);
            coin = fn_1_1194(a->paramA, pos);
            return 1;
    }
    return 0;
}

void fn_1_32CC(OMOBJ *obj)
{
    MGACTOR_COLMAP_POLY poly;
    MGACTOR_PARAM param;
    COL_ATTRPARAM attr;
    HuVecF pos;
    HuVecF start;
    s8 order[4];
    s16 colMdl[3];
    s32 i;
    s32 j;
    s32 r;
    u32 isCom;
    s16 mdl;

    obj->mdlId[0] = Hu3DModelCreateData(0x830001);
    obj->mdlId[1] = Hu3DModelCreateData(0x830003);
    obj->mdlId[2] = colMdl[0] = Hu3DModelCreateData(0x830002);
    obj->mdlId[3] = colMdl[1] = Hu3DModelCreateData(0x830004);
    obj->mdlId[4] = colMdl[2] = Hu3DModelCreateData(0x830005);
    obj->mdlId[5] = Hu3DModelCreateData(0x830000);
    obj->mdlId[6] = Hu3DModelCreateData(0x830006);
    obj->mdlId[7] = Hu3DModelCreateData(0x830006);
    obj->mdlId[8] = Hu3DModelCreateData(0x830006);
    obj->mdlId[9] = Hu3DModelCreateData(0x830006);
    obj->mdlId[10] = Hu3DModelCreateData(0x830007);
    obj->mdlId[11] = Hu3DModelCreateData(0x830008);
    obj->mdlId[12] = Hu3DModelCreateData(0x830009);
    obj->mdlId[13] = Hu3DModelCreateData(0x83000A);
    obj->mdlId[14] = Hu3DModelCreateData(0x83000B);
    obj->mdlId[15] = Hu3DModelCreateData(0x83000C);
    obj->mdlId[16] = Hu3DModelCreateData(0x83000D);
    obj->mdlId[17] = Hu3DModelCreateData(0x83000E);
    obj->mdlId[18] = Hu3DModelCreateData(0x83000F);
    obj->mdlId[19] = Hu3DModelCreateData(0x830010);
    Hu3DModelLayerSet(obj->mdlId[0], 0);
    Hu3DModelLayerSet(obj->mdlId[1], 0);
    Hu3DModelLayerSet(obj->mdlId[3], 0);
    Hu3DModelLayerSet(obj->mdlId[4], 0);
    Hu3DModelLayerSet(obj->mdlId[5], 0);
    for (i = 6; i <= 9; i++) {
        Hu3DModelLayerSet(obj->mdlId[i], 1);
    }
    for (i = 10; i <= 14; i++) {
        Hu3DModelLayerSet(obj->mdlId[i], 2);
    }
    for (i = 15; i <= 19; i++) {
        Hu3DModelLayerSet(obj->mdlId[i], 2);
    }
    MgActorColMapInit(colMdl, 3, 40);
    MgActorColAttrParamGet(&attr, 1);
    attr.yDeviate = 1.0f;
    MgActorColAttrParamSet(&attr, 1);
    MgActorColAttrParamGet(&attr, 2);
    attr.yDeviate = 1.0f;
    MgActorColAttrParamSet(&attr, 2);
    MgActorColAttrParamGet(&attr, 0x80);
    attr.yDeviate = 1.0f;
    MgActorColAttrParamSet(&attr, 0x80);
    MgActorColAttrParamGet(&attr, 0x100);
    attr.yDeviate = 1.0f;
    MgActorColAttrParamSet(&attr, 0x100);
    Hu3DModelShadowMapSet(obj->mdlId[0]);
    Hu3DModelShadowSet(obj->mdlId[3]);
    Hu3DModelShadowSet(obj->mdlId[4]);
    Hu3DModelAttrSet(obj->mdlId[2], 1);
    Hu3DModelAttrSet(obj->mdlId[0], 0x40000001);
    Hu3DModelAttrSet(obj->mdlId[5], 0x40000002);
    Hu3DModelPosSetV(obj->mdlId[3], &lbl_1_data_C4[0]);
    Hu3DModelPosSetV(obj->mdlId[4], &lbl_1_data_C4[1]);
    lbl_1_bss_40[4] = -1;
    Hu3DModelAttrSet(obj->mdlId[15], 1);
    Hu3DModelAttrSet(obj->mdlId[16], 1);
    Hu3DModelAttrSet(obj->mdlId[17], 1);
    Hu3DModelAttrSet(obj->mdlId[18], 1);
    Hu3DModelAttrSet(obj->mdlId[19], 1);
    Hu3DModelAttrSet(obj->mdlId[15], 0x40000002);
    Hu3DModelAttrSet(obj->mdlId[16], 0x40000002);
    Hu3DModelAttrSet(obj->mdlId[17], 0x40000002);
    Hu3DModelAttrSet(obj->mdlId[18], 0x40000002);
    Hu3DModelAttrSet(obj->mdlId[19], 0x40000002);
    i = 5;
    while (i--) {
        lbl_1_bss_2A0[i] = -1;
    }
    Hu3DModelCameraInfoSet(obj->mdlId[0], 1);
    i = 9;
    while (i--) {
        start = lbl_1_data_DC[i];
        start.y -= 50.0f;
        lbl_1_data_DC[i].y += 50.0f;
        if (MgActorColMapPolyGet(&lbl_1_data_DC[i], &start, -1, &poly)) {
            if (poly.mdlNo == 0 && (poly.code & 0x407F) != 0) {
                lbl_1_bss_254[i] = poly.triNo;
                lbl_1_data_DC[i] = poly.pos;
            } else {
                OSReport("m576 : \223\301\216\352\217\210\227\235 %d \202\314\203|\203W\203V\203\207\203\223\202\314\220\335\222\350\202\252\202\250\202\251\202\265\202\242\201@(Map No : %d, Attr : %x)\n", i, poly.mdlNo, poly.code);
            }
        } else {
            OSReport("m576 : \223\301\216\352\217\210\227\235 %d \202\314\203|\203W\203V\203\207\203\223\202\252\214\251\202\302\202\251\202\347\202\310\202\242\n", i);
        }
    }
    lbl_1_bss_254[9] = -1;
    lbl_1_bss_254[10] = -1;
    lbl_1_bss_254[11] = -1;
    i = 4;
    while (i--) {
        start = lbl_1_data_184[i];
        start.y -= 50.0f;
        lbl_1_data_184[i].y += 50.0f;
        if (MgActorColMapPolyGet(&lbl_1_data_184[i], &start, -1, &poly)) {
            if (poly.mdlNo == 0 && (poly.code & 0x407F) != 0) {
                lbl_1_bss_2B4[i] = poly.triNo;
                lbl_1_data_184[i] = poly.pos;
            } else {
                OSReport("m576 : \223\301\216\352\226\332\225W\203Z\203\213 %d \202\314\203|\203\212\203S\203\223\202\252\203q\203b\203g\202\265\202\310\202\242\201@(Map No : %d, Attr : %x)\n", i, poly.mdlNo, poly.code);
            }
        } else {
            OSReport("m576 : \223\301\216\352\226\332\225W\203Z\203\213 %d \202\314\203|\203\212\203S\203\223\202\252\214\251\202\302\202\251\202\347\202\310\202\242\n", i);
        }
    }
    param.height = 150.0f;
    param.radius = 50.0f;
    param.param = 0;
    param.type = 0;
    param.narrowHook = fn_1_1550;
    param.attr = 0;
    param.correctHookParam = 0;
    param.correctHook = fn_1_FA0;
    for (i = 0; i < 4; i++) {
        if (GwPlayerConf[i].type != 0) {
            isCom = 1;
        } else {
            isCom = 0;
        }
        param.param = i;
        lbl_1_bss_2FC[i] = MgPlayerCreateJumpAlt(i, &param, 8, 1, -1, NULL, isCom);
        if (isCom == 0) {
            MgPlayerVibrateCreate(lbl_1_bss_2FC[i]);
        }
    }
    param.height = 100.0f;
    param.radius = 75.0f;
    param.param = 0;
    param.type = 6;
    param.narrowHook = NULL;
    param.attr = 0;
    param.correctHookParam = 0;
    param.correctHook = fn_1_FA0;
    i = 4;
    while (i--) {
        mdl = obj->mdlId[i + 6];
        lbl_1_bss_40[i] = -1;
        param.param = i + 4;
        lbl_1_bss_2C8[i] = MgActorCreate(&param, mdl);
        MgActorColAttrSet(lbl_1_bss_2C8[i], 0x24);
        Hu3DModelAttrSet(mdl, 0x40000002);
        lbl_1_bss_2C8[i]->gravity = 0.0f;
        MgActorPosSet(lbl_1_bss_2C8[i], &lbl_1_data_58[i]);
    }
    i = 4;
    while (i--) {
        order[i] = i;
    }
    i = 4;
    while (i--) {
        if (i != 0) {
            r = frandmod(i + 1);
        } else {
            r = 0;
        }
        lbl_1_bss_2C4[i] = order[r];
        for (j = 0; j <= i; j++) {
            if (j != r && j > r) {
                order[j - 1] = order[j];
            }
        }
    }
    param.height = 150.0f;
    param.radius = 50.0f;
    param.param = 0;
    param.type = 5;
    param.narrowHook = NULL;
    param.attr = 0;
    param.correctHookParam = 0;
    param.correctHook = fn_1_FA0;
    i = 5;
    while (i--) {
        mdl = obj->mdlId[i + 10];
        param.param = i + 8;
        lbl_1_bss_2D8[i] = MgActorCreate(&param, mdl);
        Hu3DModelAttrSet(mdl, 0x40000001);
        Hu3DModelLayerSet(mdl, 2);
        lbl_1_bss_2D8[i]->gravity = 0.0f;
        pos = lbl_1_data_88[i];
        pos.y = 1300.0f;
        MgActorPosSet(lbl_1_bss_2D8[i], &pos);
    }
    for (i = 0; i < 4; i++) {
        lbl_1_bss_2FC[i]->actor->gravity = 0.0f;
        MgActorPosSet(lbl_1_bss_2FC[i]->actor, &lbl_1_data_28[i]);
        if (GwPlayerConf[i].type == 0) {
            MgPlayerAttrSet(lbl_1_bss_2FC[i], 1);
            MgPlayerPadSet(lbl_1_bss_2FC[i], 0, 0, 0, 0);
        }
    }
    fn_1_C16C(obj->mdlId[2]);
    for (i = 0; i < 4; i++) {
        if (GwPlayerConf[i].type != 0) {
            lbl_1_bss_2EC[lbl_1_bss_8] = fn_1_C1EC(lbl_1_bss_2FC[i]);
            lbl_1_bss_8++;
        }
    }
    HuPrcChildCreate(fn_1_AC4, 0xC8, 0x1000, 0, HuPrcCurrentGet());
    obj->objFunc = NULL;
}

void fn_1_4100(void)
{
    HuVecF pos;
    HuVecF off;
    HuVecF rot;
    HuVecF diff;
    Mtx mtx;
    MGACTOR *actor;
    s32 owner;
    s32 i;

    for (i = 0; i <= 3; i++) {
        MgActorPosGet(lbl_1_bss_2FC[i]->actor, &lbl_1_bss_20C[i]);
    }
    MgActorExec();
    i = 5;
    while (i--) {
        if (lbl_1_bss_2A0[i] != -1) {
            owner = lbl_1_bss_2A0[i];
            actor = lbl_1_bss_2FC[owner]->actor;
            MgActorPosGet(actor, &pos);
            PSVECSubtract(&pos, &lbl_1_bss_20C[owner], &diff);
            if (PSVECSquareMag(&diff) < 1.0f) {
                Hu3DParManAttrSet(lbl_1_bss_28C[i], 1);
            } else {
                Hu3DParManAttrReset(lbl_1_bss_28C[i], 1);
                off.x = 0.0f;
                off.y = 80.0f;
                off.z = -15.0f;
                rot.x = 45.0f;
                rot.y = -actor->rotY;
                rot.z = 0.0f;
                PSMTXRotRad(mtx, 'Y', 0.017453292f * actor->rotY);
                PSMTXMultVec(mtx, &off, &off);
                PSMTXMultVec(mtx, &rot, &rot);
                PSVECAdd(&pos, &off, &pos);
                Hu3DParManRotSet(lbl_1_bss_28C[i], rot.x, rot.y, rot.z);
                Hu3DParManPosSet(lbl_1_bss_28C[i], pos.x, pos.y, pos.z);
            }
        } else if (lbl_1_bss_2D8[i]->velY <= 0.0f) {
            MgActorColAttrReset(lbl_1_bss_2D8[i], 4);
        }
    }
}

void fn_1_43A8(s8 *keys, s8 *vals, s8 num)
{
    s8 tmp;
    s32 i;
    s32 j;

    for (i = 0; i < num - 1; i++) {
        for (j = i + 1; j < num; j++) {
            if (keys[i] > keys[j]) {
                tmp = keys[i];
                keys[i] = keys[j];
                keys[j] = tmp;
                if (vals != NULL) {
                    tmp = vals[i];
                    vals[i] = vals[j];
                    vals[j] = tmp;
                }
            }
        }
    }
}

void fn_1_4448(void)
{
    s8 order[4];
    s8 sorted[4];
    s8 count;
    s8 maxCount;
    s8 k;
    s32 winNum;
    s32 i;
    s32 j;
    s32 run;
    s32 rank;
    s8 prev;

    maxCount = 0;
    for (i = 0; i <= 3; i++) {
        count = 0;
        j = 5;
        while (j--) {
            if (i == lbl_1_bss_2A0[j]) {
                count++;
            }
        }
        lbl_1_bss_3C[i] = count;
        if (maxCount < lbl_1_bss_3C[i]) {
            maxCount = lbl_1_bss_3C[i];
        }
    }
    winNum = 0;
    if (lbl_1_bss_3C[0] != lbl_1_bss_3C[1] || lbl_1_bss_3C[1] != lbl_1_bss_3C[2] || lbl_1_bss_3C[2] != lbl_1_bss_3C[3]) {
        for (k = 0; k < 4; k++) {
            sorted[k] = lbl_1_bss_3C[k];
            order[k] = k;
        }
        fn_1_43A8(sorted, order, 4);
        run = 0;
        rank = 0;
        prev = sorted[3];
        i = 4;
        while (i--) {
            if (prev != sorted[i]) {
                rank += run;
                run = 1;
            } else {
                run++;
            }
            if (rank == 0) {
                if (_CheckFlag(FLAG_MG_PRACTICE) == 0) {
                    GwPlayer[order[i]].mgCoinBonus = 10;
                }
            } else {
                if (_CheckFlag(FLAG_MG_PRACTICE) == 0) {
                    GwPlayer[order[i]].mgCoinBonus = 0;
                }
            }
            prev = sorted[i];
        }
        for (i = 0; i < 4; i++) {
            if (lbl_1_bss_3C[i] == maxCount) {
                lbl_1_bss_3C[i] = 1;
                order[winNum] = lbl_1_bss_2FC[i]->charNo;
                winNum++;
            } else {
                lbl_1_bss_3C[i] = 0;
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            lbl_1_bss_3C[i] = 0;
        }
        if (_CheckFlag(FLAG_MG_PRACTICE) == 0) {
            GwPlayer[0].mgCoinBonus = 0;
        }
        if (_CheckFlag(FLAG_MG_PRACTICE) == 0) {
            GwPlayer[1].mgCoinBonus = 0;
        }
        if (_CheckFlag(FLAG_MG_PRACTICE) == 0) {
            GwPlayer[2].mgCoinBonus = 0;
        }
        if (_CheckFlag(FLAG_MG_PRACTICE) == 0) {
            GwPlayer[3].mgCoinBonus = 0;
        }
    }
    for (i = winNum; i < 4; i++) {
        order[i] = -1;
    }
    MgSeqWinnerSet(order[0], order[1], order[2], order[3]);
}

void fn_1_48A0(s16 mode, s16 frameNo)
{
    s32 i;

    MgActorExec();
    for (i = 0; i < 4; i++) {
        MgPlayerPauseOn(lbl_1_bss_2FC[i]);
    }
    Hu3DCameraPosGet(1, &lbl_1_bss_58[0], &lbl_1_bss_58[1], &lbl_1_bss_58[2]);
    MgSeqModeNext();
}

void fn_1_4920(s16 mode, s16 frameNo)
{
    HuVecF cam[3];
    HuVecF pos;
    s32 i;
    s32 n;
    s32 thresh;

    MgActorExec();
    for (i = 0; i < 5; i++) {
        switch (i) {
            case 0:
                thresh = 0x2C;
                break;
            case 1:
                thresh = 0x32;
                break;
            case 2:
                thresh = 0x32;
                break;
            case 3:
                thresh = 0x36;
                break;
            case 4:
                thresh = 0x3A;
                break;
        }
        if (frameNo == thresh) {
            lbl_1_bss_2D8[i]->gravity = 100.0f;
            HuAudFXPlayVolPan(0x937, (s16)(111.0f + 16.0f * (lbl_1_bss_2D8[i]->pos.z / 700.0f)),
                (s16)(64.0f + 16.0f * (lbl_1_bss_2D8[i]->pos.x / 700.0f)));
        } else if (frameNo < thresh) {
            lbl_1_bss_2D8[i]->velY = 100.0f;
        }
    }
    if (frameNo >= 0x1E) {
        if (frameNo < 0x77) {
            n = (s32)((f64)(frameNo - 0x1D) / 1.5);
            if (n == 0) {
                n = 1;
            }
            if (n >= 40) {
                n = 40;
            }
            if (lbl_1_bss_40[4] < 0) {
                lbl_1_bss_40[4] = HuAudFXPlay(0x3A1);
            }
            HuAudFXVolSet(lbl_1_bss_40[4], (s16)(63.0f + 64.0f * ((f32)n / 40.0f)));
            if (frameNo % 10 == 0) {
                i = 4;
                while (i--) {
                    if (GwPlayerConf[i].type == 0) {
                        if (n < 15) {
                            omVibrate(lbl_1_bss_2FC[i]->playerNo, 12, 6, 6);
                        } else if (n < 30) {
                            omVibrate(lbl_1_bss_2FC[i]->playerNo, 12, 4, 2);
                        } else {
                            omVibrate(lbl_1_bss_2FC[i]->playerNo, 12, 12, 0);
                        }
                    }
                }
            }
            cam[0] = lbl_1_bss_58[0];
            cam[1] = lbl_1_bss_58[1];
            cam[2] = lbl_1_bss_58[2];
            fn_1_2AC(&cam[0], n);
            fn_1_2AC(&cam[2], n);
            Hu3DCameraPosSetV(1, &cam[0], &cam[1], &cam[2]);
        } else {
            switch (frameNo) {
                case 0x77:
                    if (lbl_1_bss_40[4] >= 0) {
                        HuAudFXStop(lbl_1_bss_40[4]);
                        lbl_1_bss_40[4] = -1;
                    }
                    Hu3DCameraPosSetV(1, &lbl_1_bss_58[0], &lbl_1_bss_58[1], &lbl_1_bss_58[2]);
                    break;
                case 0x78:
                    HuAudFXPlay(0x936);
                    Hu3DMotionTimeSet(lbl_1_bss_0->mdlId[5], 0.0f);
                    Hu3DModelAttrReset(lbl_1_bss_0->mdlId[5], 0x40000002);
                    break;
                case 0xD2:
                    lbl_1_bss_1C = 1;
                    for (i = 0; i < 4; i++) {
                        Hu3DModelShadowSet(lbl_1_bss_2FC[i]->actor->mdlId);
                        lbl_1_bss_2FC[i]->actor->gravity = 150.0f;
                        MgActorColAttrSet(lbl_1_bss_2FC[i]->actor, 2);
                        MgPlayerPauseOff(lbl_1_bss_2FC[i], &lbl_1_data_28[i]);
                    }
                    break;
            }
        }
    }
    if (lbl_1_bss_1C == 1) {
        MgActorPosGet(lbl_1_bss_2FC[0]->actor, &pos);
        if (pos.y < 1000.0f) {
            lbl_1_bss_1C = 2;
            for (i = 0; i < 4; i++) {
                MgActorColAttrReset(lbl_1_bss_2FC[i]->actor, 2);
            }
        }
    }
    if (lbl_1_bss_20 == 0 && (lbl_1_bss_2FC[0]->actor->colGroundAttr & 0x407F) != 0) {
        lbl_1_bss_20 = 1;
        i = 4;
        while (i--) {
            if (GwPlayerConf[i].type == 0) {
                omVibrate(lbl_1_bss_2FC[i]->playerNo, 12, 12, 0);
            }
        }
    }
}

void fn_1_5288(s16 mode, s16 frameNo)
{
    HuVecF pos;
    ANIMDATA *anim;
    M576ColSet *set;
    s32 i;

    if (lbl_1_bss_24 == 0) {
        MgActorPosGet(lbl_1_bss_2FC[0]->actor, &pos);
        if (pos.y < 1000.0f) {
            lbl_1_bss_24 = 1;
            for (i = 0; i < 4; i++) {
                MgActorColAttrReset(lbl_1_bss_2FC[i]->actor, 2);
            }
        }
    }
    if (lbl_1_bss_28[0] == 0 && (lbl_1_bss_2FC[0]->actor->colGroundAttr & 0x407F) != 0) {
        lbl_1_bss_28[0] = 1;
        i = 4;
        while (i--) {
            if (GwPlayerConf[i].type == 0) {
                omVibrate(lbl_1_bss_2FC[i]->playerNo, 12, 12, 0);
            }
        }
    }
    if (frameNo == 1) {
        anim = HuSprAnimRead(HuDataSelHeapReadNum(0x210001, HU_MEMNUM_OVL, HEAP_MODEL));
        lbl_1_bss_54 = HuAudBGMPlay(0x35);
        Hu3DParManInit();
        for (i = 0; i < 5; i++) {
            set = &lbl_1_data_204[i];
            lbl_1_data_1B4.colorStart[0].r = set->start[0].r;
            lbl_1_data_1B4.colorStart[0].g = set->start[0].g;
            lbl_1_data_1B4.colorStart[0].b = set->start[0].b;
            lbl_1_data_1B4.colorStart[0].a = set->start[0].a;
            set = &lbl_1_data_204[i];
            lbl_1_data_1B4.colorStart[1].r = set->start[1].r;
            lbl_1_data_1B4.colorStart[1].g = set->start[1].g;
            lbl_1_data_1B4.colorStart[1].b = set->start[1].b;
            lbl_1_data_1B4.colorStart[1].a = set->start[1].a;
            set = &lbl_1_data_204[i];
            lbl_1_data_1B4.colorEnd[0].r = set->end[0].r;
            lbl_1_data_1B4.colorEnd[0].g = set->end[0].g;
            lbl_1_data_1B4.colorEnd[0].b = set->end[0].b;
            lbl_1_data_1B4.colorEnd[0].a = set->end[0].a;
            set = &lbl_1_data_204[i];
            lbl_1_data_1B4.colorEnd[1].r = set->end[1].r;
            lbl_1_data_1B4.colorEnd[1].g = set->end[1].g;
            lbl_1_data_1B4.colorEnd[1].b = set->end[1].b;
            lbl_1_data_1B4.colorEnd[1].a = set->end[1].a;
            lbl_1_bss_7C[i] = lbl_1_data_1B4;
            lbl_1_bss_28C[i] = Hu3DParManCreate(anim, 100, &lbl_1_bss_7C[i]);
            Hu3DParManAttrSet(lbl_1_bss_28C[i], 0x65);
            Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(lbl_1_bss_28C[i]), 1);
        }
    }
    MgActorExec();
}

void fn_1_55C8(s16 mode, s16 frameNo)
{
    MGACTOR *actor;
    s32 n;
    s32 i;

    switch (frameNo) {
        case 0:
            for (i = 0; i < 4; i++) {
                if (GwPlayerConf[i].type == 0) {
                    MgPlayerAttrReset(lbl_1_bss_2FC[i], 1);
                }
            }
            for (i = 0; i <= 3; i++) {
                MgActorPosGet(lbl_1_bss_2FC[i]->actor, &lbl_1_bss_20C[i]);
            }
            fn_1_EA08(lbl_1_bss_4);
            break;
        case 0x1E0:
            n = 0;
            goto drop;
        case 0x3C0:
            n = 1;
            goto drop;
        case 0x5A0:
            n = 2;
            goto drop;
        case 0x780:
            n = 3;
        drop:
            actor = lbl_1_bss_2C8[lbl_1_bss_2C4[n]];
            OSReport("%d\n", actor->param - 4);
            lbl_1_bss_40[actor->param - 4] = HuAudFXPlayVolPan(0x93A,
                (s16)(111.0f + 16.0f * (lbl_1_data_58[actor->param - 4].z / 700.0f)),
                (s16)(64.0f + 16.0f * (lbl_1_data_58[actor->param - 4].x / 700.0f)));
            actor->gravity = 160.0f;
            MgActorColAttrReset(actor, 4);
            break;
    }
    lbl_1_bss_C++;
    lbl_1_bss_10++;
    lbl_1_bss_23C[0] = lbl_1_data_C4[0];
    if ((f32)lbl_1_bss_C < 71.42857f) {
        lbl_1_bss_23C[0].y += 7.0f * (f32)lbl_1_bss_C;
    } else if ((f32)lbl_1_bss_C < 142.85715f) {
        lbl_1_bss_23C[0].y += 500.0f - (7.0f * ((f32)lbl_1_bss_C - 71.42857f));
    } else {
        lbl_1_bss_C = 0;
    }
    Hu3DModelPosSetV(lbl_1_bss_0->mdlId[3], &lbl_1_bss_23C[0]);
    lbl_1_bss_23C[1] = lbl_1_data_C4[1];
    if ((f32)lbl_1_bss_10 < 100.0f) {
        lbl_1_bss_23C[1].x += 3.0f * (f32)lbl_1_bss_10;
    } else if ((f32)lbl_1_bss_10 < 200.0f) {
        lbl_1_bss_23C[1].x += 300.0f - (3.0f * ((f32)lbl_1_bss_10 - 100.0f));
    } else {
        lbl_1_bss_10 = 0;
    }
    Hu3DModelPosSetV(lbl_1_bss_0->mdlId[4], &lbl_1_bss_23C[1]);
    for (i = 0; i < lbl_1_bss_8; i++) {
        fn_1_C31C(lbl_1_bss_2EC[i]);
    }
    fn_1_4100();
}

void fn_1_5C04(s16 mode, s16 frameNo)
{
    COL_ATTRPARAM attr;
    s32 i;

    lbl_1_bss_18 = 1;
    if (frameNo == 0) {
        lbl_1_bss_14 = 1;
        MgActorColAttrParamGet(&attr, 1);
        attr.yDeviate = 0.0f;
        MgActorColAttrParamSet(&attr, 1);
        MgActorColAttrParamGet(&attr, 2);
        attr.yDeviate = 0.0f;
        MgActorColAttrParamSet(&attr, 2);
        MgActorColAttrParamGet(&attr, 0x80);
        attr.yDeviate = 1.0f;
        MgActorColAttrParamSet(&attr, 0x80);
        MgActorColAttrParamGet(&attr, 0x100);
        attr.yDeviate = 1.0f;
        MgActorColAttrParamSet(&attr, 0x100);
        i = 4;
        while (i--) {
            if (GwPlayerConf[i].type == 0) {
                MgPlayerAttrSet(lbl_1_bss_2FC[i], 5);
                lbl_1_bss_2FC[i]->squishTime = 0.0f;
            }
        }
        MgSeqModeDelaySet(7, 0xB4);
        HuAudSStreamStop(lbl_1_bss_54);
    }
    fn_1_4100();
}

void fn_1_5D88(MGPLAYER *player)
{
    if (lbl_1_bss_14 == 2) {
        HuAudFXPlayVolPan(lbl_1_data_254[player->charNo], 127, (s16)(64.0f + 16.0f * (player->actor->pos.x / 300.0f)));
        return;
    }
    HuAudFXPlayVolPan(lbl_1_data_254[player->charNo], 127, (s16)(64.0f + 16.0f * (player->actor->pos.x / 700.0f)));
}

void fn_1_5EAC(s16 mode, s16 frameNo)
{
    HuVecF pos;
    HuVecF up;
    HuVecF target;
    s16 mot;
    s32 win;
    s32 i;

    if (frameNo <= 0x3C) {
        fn_1_4100();
    }
    switch (frameNo) {
        case 0:
            WipeCreate(2, 6, 60);
            return;
        case 0x3C:
            fn_1_4448();
            WipeCreate(1, 6, 60);
            i = 4;
            while (i--) {
                Hu3DModelAttrReset(lbl_1_bss_2FC[i]->actor->mdlId, 1);
                Hu3DModelScaleSet(lbl_1_bss_2FC[i]->actor->mdlId, 1.0f, 1.0f, 1.0f);
                Hu3DModelRotSet(lbl_1_bss_2FC[i]->actor->mdlId, 0.0f, 0.0f, 0.0f);
                Hu3DModelPosSetV(lbl_1_bss_2FC[i]->actor->mdlId, &lbl_1_data_27C[i]);
                if (lbl_1_bss_2FC[i]->motNo != 0) {
                    CharMotionShiftSet(lbl_1_bss_2FC[i]->charNo, lbl_1_bss_2FC[i]->omObj->mtnId[0], 0.0f, 5.0f, 0x40000001);
                    lbl_1_bss_2FC[i]->motNo = 0;
                }
                lbl_1_bss_2FC[i]->actor->pos = lbl_1_data_27C[i];
            }
            i = 5;
            while (i--) {
                Hu3DModelAttrSet(lbl_1_bss_2D8[i]->mdlId, 1);
            }
            i = 4;
            while (i--) {
                Hu3DModelAttrSet(lbl_1_bss_2C8[i]->mdlId, 1);
            }
            i = 5;
            while (i--) {
                Hu3DParManAttrSet(lbl_1_bss_28C[i], 1);
            }
            Hu3DCameraPosGet(1, &pos, &up, &target);
            target.x = 0.0f;
            target.y = 200.0f;
            target.z = 0.0f;
            pos.x = 0.0f;
            pos.y = 450.0f;
            pos.z = 1400.0f;
            Hu3DCameraPerspectiveSet(1, 40.0f, 20.0f, 10000.0f, 1.2f);
            Hu3DCameraPosSetV(1, &pos, &up, &target);
            fn_1_4448();
            lbl_1_bss_14 = 2;
            return;
        case 0x78:
            win = 1;
            OSReport("\224\273\222\350\n");
            i = 4;
            while (i--) {
                if (lbl_1_bss_3C[i] != 0) {
                    mot = CharMotionCreate(lbl_1_bss_2FC[i]->charNo, 0x8C0030);
                    win = 0;
                } else {
                    mot = CharMotionCreate(lbl_1_bss_2FC[i]->charNo, 0x8C0031);
                }
                CharMotionShiftSet(lbl_1_bss_2FC[i]->charNo, mot, 0.0f, 5.0f, 0);
            }
            if (win != 0) {
                fn_1_5D88(lbl_1_bss_2FC[0]);
                fn_1_5D88(lbl_1_bss_2FC[1]);
                fn_1_5D88(lbl_1_bss_2FC[2]);
                fn_1_5D88(lbl_1_bss_2FC[3]);
            }
            MgSeqModeNext();
            return;
    }
}

void fn_1_6730(s16 mode, s16 frameNo)
{
}

void fn_1_6734(s16 mode, s16 frameNo)
{
    if (frameNo >= 60) {
        MgSeqModeNext();
    }
}

void fn_1_6764(s16 mode, s16 frameNo)
{
    MgSeqModeNext();
}
