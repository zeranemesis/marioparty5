#include "REL/m564Dll.h"

typedef struct m564motent_s {
    u16 mot; //0x00
    s16 overlay; //0x02
    u32 attr; //0x04
} M564_MOTENT;

static inline f32 M564Fmod(f32 x, f64 y) {
    return fmod(x, y);
}

void fn_1_411C(OMOBJ *obj);
void fn_1_44A0(MGPLAYER *player);
void fn_1_4D2C(MGPLAYER *player);
void fn_1_4E84(MGPLAYER *player);
void fn_1_4FA4(MGPLAYER *player);
void fn_1_50CC(MGPLAYER *player);
void fn_1_52AC(MGPLAYER *player);
s32 fn_1_5600(M564_PLAYER *work);
void fn_1_57C4(M564_PLAYER *work);
s32 fn_1_5930(M564_PLAYER *work);
void fn_1_5D5C(MGPLAYER *player, s16 bombNo);
void fn_1_5DF4(MGPLAYER *player);

unsigned int lbl_1_data_118[11] = {
    0x8C0000, 0x8C0001, 0x8C0002, 0x8C0028, 0x8C0059, 0x8C002F, 0x8C003F, 0x8C005B, 0x8C0026, 0x8C0027, 0,
};

M564_MOTENT lbl_1_data_144[11] = {
    { 0, -1, 0x40000001 }, { 1, -1, 0x40000001 }, { 2, -1, 0x40000001 }, { 3, -1, 0 },
    { 4, -1, 0 },          { 5, -1, 0 },          { 6, -1, 0x40000001 }, { 7, -1, 0 },
    { 0, 8, 0x40000001 },  { 1, 8, 0x40000001 },  { 2, 9, 0x40000001 },
};

MGPLAYER_MODE_FUNC lbl_1_data_19C[6] = {
    fn_1_44A0, fn_1_4D2C, fn_1_4E84, fn_1_4FA4, fn_1_50CC, fn_1_52AC,
};

Vec lbl_1_data_1B4[4] = {
    { -250.0f, 0.0f, -250.0f },
    { 250.0f, 0.0f, -250.0f },
    { -250.0f, 0.0f, 250.0f },
    { 250.0f, 0.0f, 250.0f },
};

f32 lbl_1_data_1E4[4] = { 45.0f, -45.0f, 135.0f, -135.0f };

char lbl_1_data_1F4[12] = "root_motion";

OMOBJ **lbl_1_bss_110;

void fn_1_3E7C(void) {
    MGACTOR_PARAM param = { 150.0f, 50.0f, 0, 0, NULL, 0, 0, NULL };
    OMOBJ *obj;
    M564_PLAYER *work;
    s16 i;
    s32 mdl;

    omMakeGroupEx(lbl_1_bss_F8, 0, 4);
    lbl_1_bss_110 = omGetGroupMemberListEx(lbl_1_bss_F8, 0);
    for (i = 0; i < 4; i++) {
        obj = omAddObjEx(lbl_1_bss_F8, (s16)(i + 0x1000), 0, 0, 0, fn_1_411C);
        lbl_1_bss_110[i] = obj;
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M564_PLAYER), HU_MEMNUM_OVL);
        work = obj->data;
        lbl_1_bss_100[i] = work;
        fn_1_36E0(work, i);
        work->mgPlayer = MgPlayerCreate(i, &param, 2, (s16)lbl_1_bss_44.camNo, -1, lbl_1_data_118);
        MgPlayerModeFuncSet(work->mgPlayer, lbl_1_data_19C);
        MgActorColMaskSet(work->mgPlayer->actor, 3);
        param.param++;
        mdl = work->mgPlayer->actor->mdlId;
        Hu3DModelShadowSet((s16)mdl);
        Hu3DModelLayerSet((s16)mdl, 2);
        MgPlayerPadSet(work->mgPlayer, 0, 0, 0, 0);
        work->state = 0;
        work->carryF = 0;
        work->bombNo = -1;
        work->unk70 = 0;
        work->unk3C = 0;
        fn_1_9AC0(work);
        work->pos = lbl_1_data_1B4[work->playerNo];
        work->mgPlayer->actor->rotY = lbl_1_data_1E4[work->playerNo];
        MgActorPosUpdate(work->mgPlayer->actor, &lbl_1_data_1B4[work->playerNo]);
        work->itemHook = CharModelItemHookGet(work->charNo, 2, 0);
    }
    CharEffectLayerSet(3);
}

void fn_1_411C(OMOBJ *obj) {
    M564_PLAYER *work;

    work = obj->data;
    MgActorPosGet(work->mgPlayer->actor, &work->pos);
    CharModelVoicePanSet(work->charNo, fn_1_1448(&work->pos));
    if (lbl_1_bss_24 != 2) {
        work->btn = 0;
        if (work->state == 1) {
            work->state = 0;
            work->btn = 0x100;
        }
        MgPlayerPadSet(work->mgPlayer, 0, 0, work->btn, 0);
        return;
    }
    if (work->flag != 0) {
        work->btn = 0;
        work->stkY = 0;
        work->stkX = 0;
        fn_1_9ADC(work);
        MgPlayerPadSet(work->mgPlayer, work->stkX, work->stkY, work->btn, 0);
    }
}

void fn_1_4204(MGPLAYER *player, s16 motNo) {
    s16 mdl;
    s16 overlay;
    u32 attr;
    u32 charAttr;

    if (player->motNo != motNo) {
        player->motNo = motNo;
        mdl = player->actor->mdlId;
        attr = lbl_1_data_144[motNo].attr;
        Hu3DModelAttrReset(mdl, 0x40000201);
        overlay = lbl_1_data_144[motNo].overlay;
        if (overlay >= 0) {
            CharMotionShiftSet(player->charNo, player->omObj->mtnId[lbl_1_data_144[motNo].mot], 0.0f, 12.0f, attr);
            Hu3DMotionOverlaySet(mdl, player->omObj->mtnId[overlay]);
            charAttr = 0;
            if ((attr & 0x40000001) != 0) {
                charAttr |= 0x40000200;
            }
            CharModelAttrSet(player->charNo, charAttr);
            return;
        }
        Hu3DMotionOverlayReset(mdl);
        if (motNo == 4 || motNo == 6) {
            CharMotionSet(player->charNo, player->omObj->mtnId[lbl_1_data_144[motNo].mot]);
            return;
        }
        CharMotionShiftSet(player->charNo, player->omObj->mtnId[lbl_1_data_144[motNo].mot], 0.0f, 12.0f, attr);
    }
}

void fn_1_43E0(MGPLAYER *player, s16 mode) {
    player->work[0] = 0;
    player->work[1] = 0;
    MgPlayerModeSet(player, mode);
}

s32 fn_1_4428(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b) {
    M564_PLAYER *work;
    s16 idx;

    work = lbl_1_bss_100[a->paramB];
    idx = (s16)(b->paramB - 4);
    if (idx >= 0 && a->paramB == lbl_1_bss_1E8[idx]->holder) {
        return 0;
    }
    return 1;
}

void fn_1_44A0(MGPLAYER *player) {
    M564_PLAYER *work;
    f32 speed2;

    work = lbl_1_bss_100[player->playerNo];
    if (fn_1_5930(work) == 0) {
        if ((HuPadBtnDown[player->padNo] & 0x100) != 0) {
            if (work->carryF != 0) {
                fn_1_57C4(work);
                return;
            }
            if (fn_1_5600(work) != 0) {
                return;
            }
        }
        speed2 = (player->actor->vel.x * player->actor->vel.x) + (player->actor->vel.z * player->actor->vel.z);
        if (work->carryF != 0) {
            if (speed2 > 25.0f) {
                fn_1_4204(player, 10);
            } else if (speed2 > 0.001f) {
                fn_1_4204(player, 9);
            } else {
                fn_1_4204(player, 8);
            }
        } else if (speed2 > 25.0f) {
            fn_1_4204(player, 2);
        } else if (speed2 > 0.001f) {
            fn_1_4204(player, 1);
        } else {
            fn_1_4204(player, 0);
        }
    }
}

void fn_1_4D2C(MGPLAYER *player) {
    M564_PLAYER *work;
    s16 charNo;
    s16 t;

    work = lbl_1_bss_100[player->playerNo];
    if (fn_1_5930(work) == 0) {
        if (work->bombNo == -1) {
            fn_1_43E0(player, 0);
            MgPlayerAttrReset(player, 0x300);
            return;
        }
        charNo = player->charNo;
        switch (player->work[0]) {
            case 0:
                t = player->work[1];
                player->work[1] = t + 1;
                if (t == 0xF) {
                    fn_1_14D0(&lbl_1_bss_1E8[work->bombNo]->pos, 0x8D4);
                    fn_1_5D5C(player, work->bombNo);
                    player->work[0] = 1;
                    return;
                }
                break;
            case 1:
                if (CharMotionEndCheck(charNo) != 0 && CharMotionShiftIDGet(charNo) < 0) {
                    fn_1_43E0(player, 0);
                    MgPlayerAttrReset(player, 0x300);
                }
                break;
        }
    }
}

void fn_1_4E84(MGPLAYER *player) {
    M564_PLAYER *work;

    work = lbl_1_bss_100[player->playerNo];
    if (fn_1_5930(work) == 0) {
        switch (player->work[0]) {
            case 0:
                if (Hu3DMotionTimeGet(player->actor->mdlId) >= 19.0f) {
                    fn_1_14D0(&lbl_1_bss_1E8[work->bombNo]->pos, 0x8D5);
                    fn_1_89EC(work->bombNo);
                    fn_1_5DF4(player);
                    player->work[0] = 1;
                    return;
                }
                break;
            case 1:
                if (CharMotionEndCheck(player->charNo) != 0) {
                    fn_1_43E0(player, 0);
                    MgPlayerAttrReset(player, 0x300);
                }
                break;
        }
    }
}

void fn_1_4FA4(MGPLAYER *player) {
    M564_PLAYER *work;

    work = lbl_1_bss_100[player->playerNo];
    work->carryTime++;
    if (work->carryTime >= 0x46) {
        MgActorPauseOn(player->actor);
        return;
    }
    work->rot.x = -M564Fmod(5.0f * (f32)work->carryTime, 360.0);
    work->rot.y = player->actor->rotY;
    work->rot.z = 0.0f;
    player->actor->vel.z = -20.0f;
}

void fn_1_50CC(MGPLAYER *player) {
    s16 charNo;

    charNo = player->charNo;
    switch (player->work[0]) {
        case 0:
            fn_1_4204(lbl_1_bss_100[player->playerNo]->mgPlayer, 7);
            player->work[0] = 1;
            return;
        case 1:
            if (CharMotionEndCheck(charNo) != 0 && CharMotionShiftIDGet(charNo) < 0) {
                fn_1_43E0(player, 0);
            }
            return;
    }
}

void fn_1_52AC(MGPLAYER *player) {
    if (fabs(player->actor->rotY) > 0.1f) {
        fn_1_4204(player, 1);
        player->actor->rotY = fn_1_38D0(player->actor->rotY, 0.0f, 10.0f);
        if (fabs(player->actor->rotY) < 0.1f) {
            fn_1_4204(player, 0);
        }
    }
}

s32 fn_1_5600(M564_PLAYER *work) {
    work->bombNo = fn_1_8340(&work->pos, work->mgPlayer->actor->rotY);
    if (work->bombNo == -1) {
        return 0;
    }
    fn_1_86D8(work->bombNo, work->playerNo);
    fn_1_43E0(work->mgPlayer, 1);
    MgPlayerAttrSet(work->mgPlayer, 0x300);
    fn_1_4204(work->mgPlayer, 3);
    return 1;
}

void fn_1_57C4(M564_PLAYER *work) {
    fn_1_43E0(work->mgPlayer, 2);
    MgPlayerAttrSet(work->mgPlayer, 0x300);
    fn_1_4204(work->mgPlayer, 4);
}

s32 fn_1_5930(M564_PLAYER *work) {
    Vec root;
    Vec diff;
    s32 mdl;
    f32 s;

    if (fn_1_5E5C(work->playerNo) != 0) {
        return 1;
    }
    if (fn_1_8558(&work->pos, 50.0f, 150.0f) == 0 && work->unk3C == 0) {
        return 0;
    }
    if (work->carryF != 0) {
        fn_1_7404(lbl_1_bss_1E8[work->bombNo]);
    }
    work->bombNo = -1;
    omVibrate(work->playerNo, 60, 12, 0);
    work->rot.x = 0.0f;
    work->rot.y = work->mgPlayer->actor->rotY;
    work->rot.z = 0.0f;
    work->carryTime = 0;
    fn_1_43E0(work->mgPlayer, 3);
    fn_1_4204(work->mgPlayer, 6);
    s = sqrtf(12001.0f);
    work->mgPlayer->actor->velY = 100.0f + (100.0f * ((-1.0f + s) * 0.5f));
    MgActorColMaskSet(work->mgPlayer->actor, 0);
    MgPlayerAttrSet(work->mgPlayer, 0x300);
    mdl = work->mgPlayer->actor->mdlId;
    Hu3DModelObjPosGet((s16)mdl, lbl_1_data_1F4, &root);
    PSVECSubtract(&work->pos, &root, &diff);
    PSMTXTrans(Hu3DData[(s16)mdl].mtx, 0.0f, diff.y, 0.0f);
    work->pos.y = root.y;
    MgActorPosSet(work->mgPlayer->actor, &work->pos);
    fn_1_14D0(&work->pos, 0x8D9);
    work->unk70++;
    return 1;
}

void fn_1_5D5C(MGPLAYER *player, s16 bombNo) {
    M564_PLAYER *work;

    work = lbl_1_bss_100[player->playerNo];
    Hu3DModelHookSet((s16)player->actor->mdlId, work->itemHook, lbl_1_bss_1E8[bombNo]->mdlId);
    work->carryF = 1;
    fn_1_87B4(bombNo, work->playerNo);
}

void fn_1_5DF4(MGPLAYER *player) {
    M564_PLAYER *work;

    work = lbl_1_bss_100[player->playerNo];
    Hu3DModelHookReset((s16)player->actor->mdlId);
    work->carryF = 0;
    work->bombNo = -1;
}

s32 fn_1_5E5C(s16 playerNo) {
    if (playerNo == -1) {
        return 1;
    }
    if (lbl_1_bss_100[playerNo]->mgPlayer->mode == 3) {
        return 1;
    }
    return 0;
}

void fn_1_5EA8(s16 playerNo, Vec *pos) {
    M564_PLAYER *work;

    work = lbl_1_bss_100[playerNo];
    Hu3DModelObjPosGet((s16)work->mgPlayer->actor->mdlId, work->itemHook, pos);
}

void fn_1_5F08(s16 playerNo) {
    M564_PLAYER *work;

    work = lbl_1_bss_100[playerNo];
    if (work->carryF != 0) {
        fn_1_5DF4(work->mgPlayer);
        fn_1_43E0(work->mgPlayer, 0);
        MgPlayerAttrSet(work->mgPlayer, 0x300);
        work->unk3C = 1;
    }
}

s16 fn_1_5FDC(Vec *pos) {
    f32 d;
    f32 best;
    s16 found;
    s16 i;

    best = 9999.9f;
    found = -1;
    for (i = 0; i < 4; i++) {
        if (fn_1_5E5C(i) == 0) {
            d = fn_1_13A4(pos, &lbl_1_bss_100[i]->pos);
            if (d < best) {
                best = d;
                found = i;
            }
        }
    }
    return found;
}
