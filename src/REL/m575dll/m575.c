// REL/m575dll/m575.c - the main file: sequence hooks, per-frame update, audio
// forwarders, the debug-line draw hook and the small math helpers.
#include "REL/m575dll.h"
#include "game/hu3d.h"
#include "game/frand.h"
#include "game/memory.h"
#include "game/gamework.h"
#include "game/mg/seqman.h"
#include "game/mg/actman.h"
#include "game/mg/colman.h"
#include "string.h"
#include "math.h"
#include "humath.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

// Declared locally (see the note in score.c about math.h-dragging headers).
int HuAudFXPlay(int seId);
void HuAudFXStop(int seNo);
void HuAudFXFadeOut(int seNo, s32 speed);
void HuAudFXPanning(int seNo, s16 pan);
void HuAudFXListnerSetEX(Vec *pos, Vec *heading, float sndDist, float sndSpeed, float startDis,
                         float frontSurDis, float backSurDis);
int HuAudSStreamPlay(s16 streamId);
void HuAudSStreamFadeOut(int streamNo, s32 speed);
void HuAudAllStop(void);
int HuAudBGMPlay(s16 streamId);
int HuAudJinglePlay(s16 streamId);
int HuAudSStreamChanPlay(s16 streamId, s16 chanNo);
void CharLoseVoicePlay(s16 charNo1, s16 charNo2, s16 charNo3, s16 charNo4);
void CharEffectLayerSet(s16 layerNo);
void ColKill(void);
void MgActorColMaskSet(MGACTOR *actorP, u32 mask);
void WipeCreate(s16 mode, s16 type, s16 maxTime);
void GameMesKill(s16 id);
void GameMesClose(void);
u8 GameMesStatGet(s16 id);
s16 espEntry(unsigned int dataNum, s16 prio, s16 bank);
void espKill(s16 espId);
void espPriSet(s16 espId, s16 pri);
void espDispOff(s16 espId);

// Other units of this module.
void fn_1_9250(M575Light *light, M575Main *main);
void fn_1_92C8(M575Light *light);
void fn_1_931C(M575Light *light);
void fn_1_9BC8(M575Tubo *self, M575Main *main);
void fn_1_9E8C(M575Tubo *self);
void fn_1_9EE4(M575Tubo *self);
void fn_1_851C(M575Camera *self, s16 side, M575Main *main);
void fn_1_8778(M575Camera *self);
void fn_1_87A8(M575Camera *self);
void fn_1_AEF0(M575Coin *self, M575Main *main);
void fn_1_B074(M575Coin *self);
void fn_1_B0E8(M575Coin *self);
void fn_1_C464(M575Jugem *self, M575Main *main);
void fn_1_C5E0(M575Jugem *self);
void fn_1_C630(M575Jugem *self);
void fn_1_3DE0(M575Stage *self, M575Main *main);
void fn_1_4360(M575Stage *self);
void fn_1_442C(M575Stage *self);
s32 fn_1_D00C(M575Game *game);
void fn_1_E478(void);
void fn_1_E4C8(void);
void fn_1_EBEC(void);

void ObjectSetup(void);
void fn_1_280(s16 mode, s16 frameNo);
s32 fn_1_D08(M575Game *game, s32 arg);
s32 fn_1_F14(M575Game *game);
s32 fn_1_F3C(M575Game *game);
s32 fn_1_FEC(M575Game *game);
s32 fn_1_14F0(M575Player *player);
void fn_1_730(M575Game *game, OMOBJMAN *objman);
void fn_1_C5C(M575Game *game);
void fn_1_E8(s16 mode, s16 frameNo);
void fn_1_164(s16 mode, s16 frameNo);
void fn_1_1C0(s16 mode, s16 frameNo);
void fn_1_21C(s16 mode, s16 frameNo);
void fn_1_2DC(s16 mode, s16 frameNo);
void fn_1_334(s16 mode, s16 frameNo);
void fn_1_444(s16 mode, s16 frameNo);
void fn_1_4A0(s16 mode, s16 frameNo);
void fn_1_4EC(s16 mode, s16 frameNo);
void fn_1_36E4(Vec *out, Vec *p0, Vec *p1, Vec *p2, Vec *p3, f32 t);
f32 fn_1_37A4(f32 t, f32 p0, f32 p1, f32 p2, f32 p3);

s32 lbl_1_data_0 = 1;

// MGSEQ_PARAM plus the four bytes of padding the original object carries.
typedef struct M575SeqParam_s {
    MGSEQ_PARAM param;
    u32 pad;
} M575SeqParam;

M575SeqParam lbl_1_data_4 = {
    {
        0x1E,
        0,
        fn_1_E8,
        fn_1_1C0,
        fn_1_21C,
        fn_1_2DC,
        fn_1_334,
        fn_1_444,
        fn_1_4A0,
        fn_1_4EC,
        fn_1_164,
    },
    0,
};

Vec lbl_1_data_30 = { 0.0f, 5000.0f, 10.0f };
Vec lbl_1_data_3C = { 0.0f, 1.0f, 0.0f };
f32 lbl_1_data_48[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

M575Game *lbl_1_bss_0;
OMOBJMAN *lbl_1_bss_4;
u16 lbl_1_bss_8;
u16 lbl_1_bss_A;
u16 lbl_1_bss_C;
GXColor lbl_1_bss_E[0x100];
GXColor lbl_1_bss_40E[0x100];
GXColor lbl_1_bss_80E[0x100];
struct M575Line4 {
    Vec a;
    Vec b;
    Vec c;
    Vec d;
} lbl_1_bss_C10[0x100];
struct M575Line3 {
    Vec a;
    Vec b;
    Vec c;
} lbl_1_bss_3C10[0x100];
struct M575Line2 {
    Vec a;
    Vec b;
} lbl_1_bss_6010[0x100];

s32 _prolog(void)
{
    const VoidFunc *ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    lbl_1_bss_4 = MgActorObjectSetup();
    MgSeqCreate(&lbl_1_data_4.param);
    MgSeqModeHookAdd(4, fn_1_280);
}

void fn_1_E8(s16 mode, s16 frameNo)
{
    M575Game *game;

    lbl_1_bss_0 = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M575Game), HU_MEMNUM_OVL);
    game = lbl_1_bss_0;
    memset(lbl_1_bss_0, 0, sizeof(M575Game));
    fn_1_730(game, lbl_1_bss_4);
    game->mode = 0;
    MgSeqModeNext();
}

void fn_1_164(s16 mode, s16 frameNo)
{
    fn_1_C5C(lbl_1_bss_0);
    HuMemDirectFree(lbl_1_bss_0);
    lbl_1_bss_0 = NULL;
    MgSeqModeNext();
}

void fn_1_1C0(s16 mode, s16 frameNo)
{
    M575Game *game = lbl_1_bss_0;

    MgSeqModeChangeOff();
    if (fn_1_F14(game) != 0) {
        MgSeqModeNext();
    }
    game->mode = 1;
    fn_1_D08(game, 0);
}

void fn_1_21C(s16 mode, s16 frameNo)
{
    M575Game *game = lbl_1_bss_0;
    s16 mesId;

    mesId = MgSeqGameMesIdGet();
    if (game->bgm < 0) {
        game->bgm = fn_1_6B0(0x35);
    }
    game->mode = 2;
    fn_1_D08(game, 0);
}

void fn_1_280(s16 mode, s16 frameNo)
{
    M575Game *game = lbl_1_bss_0;

    fn_1_F3C(game);
    game->mode = 3;
    fn_1_D08(game, 0);
    lbl_1_data_0 = 1;
}

void fn_1_2DC(s16 mode, s16 frameNo)
{
    lbl_1_bss_0->unk25D8 = 1;
    lbl_1_bss_0->mode = 4;
    if (fn_1_D08(lbl_1_bss_0, 1) != 0) {
        MgSeqModeNext();
    }
}

void fn_1_334(s16 mode, s16 frameNo)
{
    M575Game *game = lbl_1_bss_0;
    s16 mesId;
    s32 i;
    s32 j;

    if (lbl_1_data_0 != 0) {
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 2; j++) {
                M575Player *player = &game->side[i].player[j];
                player->padOn = 0;
                player->com.unkC = 0;
            }
        }
        lbl_1_data_0 = 0;
        game->phase = 0;
    }
    mesId = MgSeqGameMesIdGet();
    if (game->bgm >= 0 && (GameMesStatGet(mesId) & 0x10) != 0) {
        fn_1_660(game->bgm, 100);
        game->bgm = -1;
    }
    game->unk25D8 = 0;
    game->mode = 5;
    fn_1_D08(game, 0);
}

void fn_1_444(s16 mode, s16 frameNo)
{
    M575Game *game = lbl_1_bss_0;

    MgSeqModeChangeOff();
    if (fn_1_FEC(game) != 0) {
        MgSeqModeNext();
    }
    game->mode = 6;
    fn_1_D08(game, 0);
}

void fn_1_4A0(s16 mode, s16 frameNo)
{
    M575Game *game = lbl_1_bss_0;

    MgSeqModeChangeOff();
    MgSeqModeNext();
    game->mode = 7;
    fn_1_D08(game, 0);
}

void fn_1_4EC(s16 mode, s16 frameNo)
{
    lbl_1_bss_0->mode = 8;
    fn_1_D08(lbl_1_bss_0, 0);
}

s32 fn_1_530(s32 seId)
{
    return HuAudFXPlay(seId);
}

void fn_1_558(s32 seNo)
{
    HuAudFXStop(seNo);
}

void fn_1_580(s32 seNo, s32 speed)
{
    HuAudFXFadeOut(seNo, speed);
}

void fn_1_5B0(s32 seNo, s16 pan)
{
    HuAudFXPanning(seNo, pan);
}

void fn_1_5E0(Vec *pos, Vec *heading, f32 sndDist, f32 sndSpeed, f32 startDis, f32 frontSurDis, f32 backSurDis)
{
    HuAudFXListnerSetEX(pos, heading, sndDist, sndSpeed, startDis, frontSurDis, backSurDis);
}

s32 fn_1_638(s16 streamId)
{
    return HuAudSStreamPlay(streamId);
}

void fn_1_660(s32 streamNo, s32 speed)
{
    HuAudSStreamFadeOut(streamNo, speed);
}

void fn_1_690(void)
{
    HuAudAllStop();
}

s32 fn_1_6B0(s16 streamId)
{
    return HuAudBGMPlay(streamId);
}

s32 fn_1_6D8(s16 streamId)
{
    return HuAudJinglePlay(streamId);
}

s32 fn_1_700(s16 streamId, s16 chanNo)
{
    return HuAudSStreamChanPlay(streamId, chanNo);
}

void fn_1_730(M575Game *game, OMOBJMAN *objman)
{
    COL_ATTRPARAM attr;
    s16 order[4];
    s16 count;
    f32 ang;
    s32 i;
    M575Main *side;

    memset(game, 0, sizeof(M575Game));
    game->objman = objman;
    game->timer = 0x708;
    game->mesId = -1;
    game->bgm = -1;
    game->unk2610 = -1;
    fn_1_3880(&game->side[0], 0, game);
    fn_1_3880(&game->side[1], 1, game);
    game->colMapNum = 0;
    game->colMap[game->colMapNum++] = game->side[0].ten.modelId2;
    game->colMap[game->colMapNum++] = game->side[1].ten.modelId2;
    ColKill();
    MgActorColMapInit(game->colMap, game->colMapNum, 0x28);

    count = 0;
    order[0] = 1;
    order[1] = 2;
    order[2] = 3;
    order[3] = 4;
    side = &game->side[0];
    for (i = 0; i < 4; i++) {
        if (GwPlayerConf[i].grp == 0) {
            fn_1_535C(&side->player[count % 2], count % 2, i, side);
            order[i] = 0;
            count++;
            if (count >= 2) {
                break;
            }
        }
    }
    if (count < 2) {
        for (i = 0; i < 4; i++) {
            if (order[i] != 0) {
                fn_1_535C(&side->player[count % 2], count % 2, order[i] - 1, side);
                order[i] = 0;
                count++;
                if (count >= 2) {
                    break;
                }
            }
        }
    }

    count = 0;
    side = &game->side[1];
    for (i = 0; i < 4; i++) {
        if (order[i] != 0) {
            fn_1_535C(&side->player[count % 2], count % 2, order[i] - 1, side);
            count++;
        }
    }

    MgActorColMapMaskSet(0, 1);
    MgActorColMapMaskSet(1, 2);
    MgActorColMaskSet(game->side[0].player[0].mgPlayer->actor, 1);
    MgActorColMaskSet(game->side[0].player[1].mgPlayer->actor, 1);
    MgActorColMaskSet(game->side[1].player[0].mgPlayer->actor, 2);
    MgActorColMaskSet(game->side[1].player[1].mgPlayer->actor, 2);

    ang = 10.0f;
    MgActorColAttrParamGet(&attr, 1);
    attr.maxDot = (f32)cos(M_PI * ang / 180.0);
    MgActorColAttrParamSet(&attr, 1);
    MgActorColAttrParamGet(&attr, 2);
    attr.maxDot = (f32)cos(M_PI * ang / 180.0);
    MgActorColAttrParamSet(&attr, 2);

    Hu3DShadowMultiCreate(45.0f, 20.0f, 80000.0f, 0x11);
    Hu3DShadowMultiTPLvlSet(0.5f, -1);
    Hu3DShadowMultiSizeSet(0xF0, -1);
    Hu3DShadowMultiPosSet(&lbl_1_data_30, &lbl_1_data_3C, (Vec *)lbl_1_data_48, 0x11);
    game->esp = espEntry(0x820018, 0, 0);
    espPriSet(game->esp, 0x100);
    espDispOff(game->esp);
    if ((frand() & 1) != 0) {
        game->firstSide = 1;
    }
    else {
        game->firstSide = 2;
    }
    game->unk2606 = frand() % 20 + 5;
    if (game->firstSide == 2) {
        game->unk2608 = frand() % 10 + game->unk2606 + 10;
    }
    fn_1_E478();
    fn_1_EBEC();
    Hu3DBGColorSet(0, 0, 0);
    WipeCreate(1, 5, 0x3C);
    Hu3DNoSyncSet(1);
}

void fn_1_C5C(M575Game *game)
{
    s32 i;

    fn_1_E4C8();
    espKill(game->esp);
    for (i = 0; i < 2; i++) {
        M575Main *side = &game->side[i];
        fn_1_55EC(&side->player[1]);
        fn_1_55EC(&side->player[0]);
    }
    fn_1_396C(&game->side[0]);
    if (game->mesId >= 0) {
        GameMesKill(game->mesId);
    }
    GameMesClose();
    memset(game, 0, sizeof(M575Game));
    fn_1_690();
}

s32 fn_1_D08(M575Game *game, s32 arg)
{
    Vec a;
    Vec b;
    Vec c;
    Vec off;
    Vec *pos;

    fn_1_39E4(&game->side[0]);
    fn_1_39E4(&game->side[1]);
    if (MgSeqModeGet() == 2 || MgSeqModeGet() == 3 || MgSeqModeGet() == 4 || MgSeqModeGet() == 5
        || MgSeqModeGet() == 6 || MgSeqModeGet() == 7 || MgSeqModeGet() == 8) {
        MgActorExec();
    }
    if (MgSeqModeGet() < 4) {
        pos = game->side[0].player[0].pos;
        off = *pos;
        off.y = 0.0f;
        off.x = 0.0f;
        PSVECAdd(&lbl_1_data_30, &off, &a);
        PSVECAdd(&lbl_1_data_3C, &off, &b);
        PSVECAdd((Vec *)lbl_1_data_48, &off, &c);
        Hu3DShadowMultiPosSet(&a, &b, &c, 1);
        pos = game->side[1].player[0].pos;
        off = *pos;
        off.y = 0.0f;
        off.x = 0.0f;
        PSVECAdd(&lbl_1_data_30, &off, &a);
        PSVECAdd(&lbl_1_data_3C, &off, &b);
        PSVECAdd((Vec *)lbl_1_data_48, &off, &c);
        Hu3DShadowMultiPosSet(&a, &b, &c, 0x10);
    }
    else if (MgSeqModeGet() == 4) {
        Hu3DShadowMultiPosSet(&lbl_1_data_30, &lbl_1_data_3C, (Vec *)lbl_1_data_48, 0x11);
    }
    if (MgSeqModeGet() == 5 && game->timer != 0) {
        game->timer--;
    }
    game->frame++;
    return 0;
}

s32 fn_1_F14(M575Game *game)
{
    return fn_1_D00C(game);
}

s32 fn_1_F3C(M575Game *game)
{
    s32 i;
    s32 j;

    for (i = 0; i < 2; i++) {
        M575Main *side = &game->side[i];
        for (j = 0; j < 2; j++) {
            M575Player *player = &side->player[j];
            fn_1_5C88(player, 1);
            player->padOn = 1;
            player->com.unkC = 1;
        }
        fn_1_3D5C(&side->score, 1);
    }
    return 1;
}

s32 fn_1_FEC(M575Game *game)
{
    M575Main *side0 = &game->side[0];
    M575Main *side1 = &game->side[1];
    M575Score *score0 = &side0->score;
    M575Score *score1 = &side1->score;
    M575Player *p0 = &side0->player[0];
    M575Player *p1 = &side0->player[1];
    M575Player *p2 = &side1->player[0];
    M575Player *p3 = &side1->player[1];
    s32 ret = 0;

    switch (game->phase) {
    case 0:
        if (side0->ten.rot < 3.0f && side1->ten.rot < 3.0f) {
            p3->state = 0;
            p2->state = 0;
            p1->state = 0;
            p0->state = 0;
            game->phase++;
        }
        break;
    case 1: {
        s32 d0 = fn_1_14F0(p0);
        s32 d1 = fn_1_14F0(p1);
        s32 d2 = fn_1_14F0(p2);
        s32 d3 = fn_1_14F0(p3);
        if (d0 != 0 && d1 != 0 && d2 != 0 && d3 != 0) {
            game->wait = 0;
            game->phase++;
        }
        break;
    }
    case 2:
        if (game->wait++ > 0x1E) {
            game->wait = 0;
            game->phase++;
        }
        break;
    case 3:
        if (score0->value != 0) {
            Hu3DMotionShiftSet(p0->modelId, p0->motion[7], 0.0f, 4.0f, 0);
            Hu3DMotionShiftSet(p1->modelId, p1->motion[7], 0.0f, 4.0f, 0);
            GWMgCoinSet(p0->playerNo, GWMgCoinGet(p0->playerNo) + score0->value);
            GWMgCoinSet(p1->playerNo, GWMgCoinGet(p1->playerNo) + score0->value);
        }
        else {
            Hu3DMotionShiftSet(p0->modelId, p0->motion[8], 0.0f, 4.0f, 0);
            Hu3DMotionShiftSet(p1->modelId, p1->motion[8], 0.0f, 4.0f, 0);
        }
        if (score1->value != 0) {
            Hu3DMotionShiftSet(p2->modelId, p2->motion[7], 0.0f, 4.0f, 0);
            Hu3DMotionShiftSet(p3->modelId, p3->motion[7], 0.0f, 4.0f, 0);
            GWMgCoinSet(p2->playerNo, GWMgCoinGet(p2->playerNo) + score1->value);
            GWMgCoinSet(p3->playerNo, GWMgCoinGet(p3->playerNo) + score1->value);
        }
        else {
            Hu3DMotionShiftSet(p2->modelId, p2->motion[8], 0.0f, 4.0f, 0);
            Hu3DMotionShiftSet(p3->modelId, p3->motion[8], 0.0f, 4.0f, 0);
        }
        if (score0->value + score1->value != 0) {
            fn_1_6D8(0x79);
        }
        else {
            fn_1_6D8(0x7B);
            CharLoseVoicePlay(p0->charNo, p1->charNo, p2->charNo, p3->charNo);
        }
        game->wait = 0;
        game->phase++;
        break;
    case 4:
        if (++game->wait > 0xD0) {
            game->wait = 0;
            game->phase++;
            ret = 1;
        }
        break;
    }
    return ret;
}

s32 fn_1_14F0(M575Player *player)
{
    M575Main *main = player->main;
    M575Player *other;
    Vec unusedA = { 0.0f, 1.0f, 0.0f };
    Vec unusedB;
    Vec goal;
    Vec diff;
    Vec tmp;
    s32 ret;
    f32 v;
    f32 w;
    f32 m;

    if (&main->player[0] == player) {
        other = &main->player[1];
    }
    else {
        other = &main->player[0];
    }
    unusedB = unusedA;
    ret = 0;
    switch (player->state) {
    case 0:
        if (player->falling == 0 && other->falling == 0 && player->mgPlayer->mode == 0) {
            fn_1_5C88(player, 0);
            if (&main->player[0] == player) {
                tmp.x = -150.0f;
                tmp.y = 0.0f;
                tmp.z = 0.0f;
            }
            else {
                tmp.x = 150.0f;
                tmp.y = 0.0f;
                tmp.z = 0.0f;
            }
            goal = tmp;
            PSVECSubtract(&goal, player->pos, &diff);
            diff.y = 0.0f;
            if (PSVECMag(&diff) > 16.0f) {
                Hu3DMotionShiftSet(player->modelId, player->motion[1], 0.0f, 4.0f, 0x40000001);
                player->state = 1;
            }
            else {
                v = (player->rot->y >= 0.0f) ? player->rot->y : -player->rot->y;
                if (v > 3.0f) {
                    Hu3DMotionShiftSet(player->modelId, player->motion[1], 0.0f, 4.0f, 0x40000001);
                }
                player->state = 2;
            }
        }
        break;
    case 1:
        if (&main->player[0] == player) {
            tmp.x = -150.0f;
            tmp.y = 0.0f;
            tmp.z = 0.0f;
        }
        else {
            tmp.x = 150.0f;
            tmp.y = 0.0f;
            tmp.z = 0.0f;
        }
        goal = tmp;
        PSVECSubtract(&goal, player->pos, &diff);
        diff.y = 0.0f;
        if (PSVECMag(&diff) > 16.0f) {
            PSVECNormalize(&diff, &diff);
            player->rot->y = fn_1_30CC((f32)HuAtan(diff.x, diff.z), player->rot->y, 0.5f);
            if (player->rot->y > 180.0f) {
                w = player->rot->y - 360.0f;
            }
            else {
                if (player->rot->y <= -180.0f) {
                    v = 360.0f + player->rot->y;
                }
                else {
                    v = player->rot->y;
                }
                w = v;
            }
            player->rot->y = w;
            PSVECScale(&diff, &diff, 8.0f);
            PSVECAdd(player->pos, &diff, player->pos);
            if (&main->player[1] == player) {
                PSVECSubtract(player->pos, other->pos, &diff);
                diff.y = 0.0f;
                m = PSVECMag(&diff);
                if (m < 100.0f) {
                    if (diff.x != 0.0f || diff.z != 0.0f) {
                        PSVECNormalize(&diff, &diff);
                    }
                    PSVECScale(&diff, &diff, 100.0f - m);
                    PSVECAdd(player->pos, &diff, player->pos);
                }
            }
        }
        else {
            *player->pos = goal;
            player->state++;
        }
        break;
    case 2:
        v = (player->rot->y >= 0.0f) ? player->rot->y : -player->rot->y;
        if (v > 3.0f) {
            player->rot->y = fn_1_30CC(0.0f, player->rot->y, 0.9f);
            if (player->rot->y > 180.0f) {
                w = player->rot->y - 360.0f;
            }
            else {
                if (player->rot->y <= -180.0f) {
                    v = 360.0f + player->rot->y;
                }
                else {
                    v = player->rot->y;
                }
                w = v;
            }
            player->rot->y = w;
        }
        else {
            player->rot->y = 0.0f;
            Hu3DMotionShiftSet(player->modelId, player->motion[0], 0.0f, 4.0f, 0x40000001);
            player->state++;
            ret = 1;
        }
        break;
    case 3:
        ret = 1;
        break;
    }
    return ret;
}

void fn_1_1C00(s32 arg)
{
    fn_1_E424(arg);
}

#define M575_VTX(p, col)     {         Vec v_ = (p);         GXColor c_ = (col);         GXPosition3f32(v_.x, v_.y, v_.z);         GXColor4u8(c_.r, c_.g, c_.b, c_.a);     }

// Debug-line draw hook: draws the three queues of line strips (segments,
// triangles, quads) filled by fn_1_2A38 / fn_1_2B10 / fn_1_2C1C, then empties them.
void fn_1_1C28(HU3DMODEL *model, Mtx *mtx)
{
    s32 i;

    GXLoadPosMtxImm(*mtx, GX_PNMTX0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT0, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
    GXSetAlphaUpdate(GX_TRUE);

    for (i = 0; i < lbl_1_bss_8; i++) {
        GXBegin(GX_LINESTRIP, GX_VTXFMT0, 2);
        GXPosition3f32(lbl_1_bss_6010[i].a.x, lbl_1_bss_6010[i].a.y, lbl_1_bss_6010[i].a.z);
        GXColor4u8(lbl_1_bss_80E[i].r, lbl_1_bss_80E[i].g, lbl_1_bss_80E[i].b, lbl_1_bss_80E[i].a);
        GXPosition3f32(lbl_1_bss_6010[i].b.x, lbl_1_bss_6010[i].b.y, lbl_1_bss_6010[i].b.z);
        GXColor4u8(lbl_1_bss_80E[i].r, lbl_1_bss_80E[i].g, lbl_1_bss_80E[i].b, lbl_1_bss_80E[i].a);
#ifdef TARGET_PC
        GXEnd();
#endif
    }
    for (i = 0; i < lbl_1_bss_A; i++) {
        GXBegin(GX_LINESTRIP, GX_VTXFMT0, 4);
        GXPosition3f32(lbl_1_bss_3C10[i].a.x, lbl_1_bss_3C10[i].a.y, lbl_1_bss_3C10[i].a.z);
        GXColor4u8(lbl_1_bss_40E[i].r, lbl_1_bss_40E[i].g, lbl_1_bss_40E[i].b, lbl_1_bss_40E[i].a);
        M575_VTX(lbl_1_bss_3C10[i].b, lbl_1_bss_40E[i]);
        M575_VTX(lbl_1_bss_3C10[i].c, lbl_1_bss_40E[i]);
        M575_VTX(lbl_1_bss_3C10[i].a, lbl_1_bss_40E[i]);
#ifdef TARGET_PC
        GXEnd();
#endif
    }
    for (i = 0; i < lbl_1_bss_C; i++) {
        GXBegin(GX_LINESTRIP, GX_VTXFMT0, 5);
        GXPosition3f32(lbl_1_bss_C10[i].a.x, lbl_1_bss_C10[i].a.y, lbl_1_bss_C10[i].a.z);
        GXColor4u8(lbl_1_bss_E[i].r, lbl_1_bss_E[i].g, lbl_1_bss_E[i].b, lbl_1_bss_E[i].a);
        M575_VTX(lbl_1_bss_C10[i].b, lbl_1_bss_E[i]);
        M575_VTX(lbl_1_bss_C10[i].c, lbl_1_bss_E[i]);
        M575_VTX(lbl_1_bss_C10[i].d, lbl_1_bss_E[i]);
        M575_VTX(lbl_1_bss_C10[i].a, lbl_1_bss_E[i]);
#ifdef TARGET_PC
        GXEnd();
#endif
    }
    lbl_1_bss_C = 0;
    lbl_1_bss_A = 0;
    lbl_1_bss_8 = 0;
}

void fn_1_2A38(Vec *a, Vec *b, GXColor *col)
{
    if (lbl_1_bss_8 < 0x100) {
        lbl_1_bss_6010[lbl_1_bss_8].a = *a;
        lbl_1_bss_6010[lbl_1_bss_8].b = *b;
        lbl_1_bss_80E[lbl_1_bss_8] = *col;
        lbl_1_bss_8++;
    }
}

void fn_1_2B10(Vec *a, Vec *b, Vec *c, GXColor *col)
{
    if (lbl_1_bss_A < 0x100) {
        lbl_1_bss_3C10[lbl_1_bss_A].a = *a;
        lbl_1_bss_3C10[lbl_1_bss_A].b = *b;
        lbl_1_bss_3C10[lbl_1_bss_A].c = *c;
        lbl_1_bss_40E[lbl_1_bss_A] = *col;
        lbl_1_bss_A++;
    }
}

void fn_1_2C1C(Vec *a, Vec *b, Vec *c, Vec *d, GXColor *col)
{
    if (lbl_1_bss_C < 0x100) {
        lbl_1_bss_C10[lbl_1_bss_C].a = *a;
        lbl_1_bss_C10[lbl_1_bss_C].b = *b;
        lbl_1_bss_C10[lbl_1_bss_C].c = *c;
        lbl_1_bss_C10[lbl_1_bss_C].d = *d;
        lbl_1_bss_E[lbl_1_bss_C] = *col;
        lbl_1_bss_C++;
    }
}

// Quaternion rotating v0 onto v1.
void fn_1_2D5C(Quaternion *q, const Vec *v0, const Vec *v1)
{
    Vec cross;
    Quaternion r;
    f32 dot;
    f32 absDot;
    f32 s;

    PSVECCrossProduct(v0, v1, &cross);
    dot = PSVECDotProduct(v0, v1);
    absDot = (dot >= 0.0f) ? dot : -dot;
    if (absDot < 0.9999999f) {
        s = sqrtf(2.0f * (1.0f + dot));
        r.x = cross.x / s;
        r.y = cross.y / s;
        r.z = cross.z / s;
        r.w = s * 0.5f;
    }
    else if (dot < 0.0f) {
        r.x = 0.0f;
        r.y = 1.0f;
        r.z = 0.0f;
        r.w = 0.0f;
    }
    else {
        r.x = 0.0f;
        r.y = 0.0f;
        r.z = 0.0f;
        r.w = 1.0f;
    }
    q->x = r.x;
    q->y = r.y;
    q->z = r.z;
    q->w = r.w;
}

// Moves angle `from` towards `to` (degrees) by `ratio` along the shorter arc.
f32 fn_1_30CC(f32 from, f32 to, f32 ratio)
{
    f32 diff;
    f32 res;

    diff = (f32)fmod(to - from, 360.0);
    if (diff < 0.0f) {
        diff += 360.0f;
    }
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    res = (f32)fmod(from + ratio * diff, 360.0);
    if (res < 0.0f) {
        res += 360.0f;
    }
    return res;
}

void fn_1_31E8(M575Path *path)
{
    memset(path, 0, sizeof(M575Path));
}

void fn_1_3218(M575Path *path)
{
    memset(path, 0, sizeof(M575Path));
}

void fn_1_3248(M575Path *path, const Vec *p, s32 len)
{
    if (path->count < 20) {
        if (path->count % 4 == 3) {
            path->pts[path->count] = *p;
            path->len[path->count] = 0;
            if (path->count != 0) {
                path->acc[path->count] = path->acc[path->count - 1] + path->len[path->count - 1];
            }
            else {
                path->acc[path->count] = 0;
            }
            path->count++;
        }
#ifdef TARGET_PC
        if (path->count >= 20) {
            return;
        }
#endif
        path->pts[path->count] = *p;
        path->len[path->count] = len;
        if (path->count != 0) {
            path->acc[path->count] = path->acc[path->count - 1] + path->len[path->count - 1];
        }
        else {
            path->acc[path->count] = 0;
        }
        path->count++;
    }
}

void fn_1_33C4(Vec *out, M575Path *path, s32 t)
{
    Vec res;
    Vec p3;
    Vec p2;
    Vec p1;
    Vec p0;
    f32 ratio;
    f32 clamped;
    f32 lo;
    s32 i;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s16 n;

#ifdef TARGET_PC
    a = 0;
    b = 1;
    c = 2;
    d = 3;
#endif
    if (path->count % 4 != 0) {
        n = path->count;
        for (i = path->count % 4; i < 4; i++) {
            path->pts[path->count] = path->pts[n - 1];
            path->acc[path->count] = path->acc[n - 1];
            path->len[path->count] = path->len[n - 1];
            path->count++;
        }
    }
    for (i = path->count - 1; i >= 0; i--) {
        if (path->acc[i] <= t) {
            a = (i / 4) * 4;
            b = a + 1;
            c = b + 1;
            d = c + 1;
            break;
        }
    }
    ratio = (f32)(t - path->acc[a]) / (f32)(path->acc[d] - path->acc[a]);
    if (ratio > 1.0f) {
        clamped = 1.0f;
    }
    else {
        if (ratio < 0.0f) {
            lo = 0.0f;
        }
        else {
            lo = ratio;
        }
        clamped = lo;
    }
    p0 = path->pts[d];
    p1 = path->pts[c];
    p2 = path->pts[b];
    p3 = path->pts[a];
    fn_1_36E4(&res, &p3, &p2, &p1, &p0, clamped);
    out->x = res.x;
    out->y = res.y;
    out->z = res.z;
}

void fn_1_36E4(Vec *out, Vec *p0, Vec *p1, Vec *p2, Vec *p3, f32 t)
{
    Vec res;

    res.x = fn_1_37A4(t, p0->x, p1->x, p2->x, p3->x);
    res.y = fn_1_37A4(t, p0->y, p1->y, p2->y, p3->y);
    res.z = fn_1_37A4(t, p0->z, p1->z, p2->z, p3->z);
    out->x = res.x;
    out->y = res.y;
    out->z = res.z;
}

f32 fn_1_37A4(f32 t, f32 p0, f32 p1, f32 p2, f32 p3)
{
    return p3 * (t * (t * t))
           + (p2 * ((1.0f - t) * (3.0f * t * t))
              + (p0 * ((1.0f - t) * ((1.0f - t) * (1.0f - t))) + p1 * ((1.0f - t) * (3.0f * t * (1.0f - t)))));
}

void fn_1_3880(M575Main *main, s16 side, M575Game *game)
{
    M575Ten *ten = &main->ten;
    u16 scoreSide;

    memset(main, 0, sizeof(M575Main));
    main->game = game;
    main->side = side;
    fn_1_9250(&main->light, main);
    fn_1_851C(&main->camera, side, main);
    fn_1_3DE0(&main->stage, main);
    if (side != 0) {
        scoreSide = 1;
    }
    else {
        scoreSide = 0;
    }
    fn_1_3A7C(&main->score, scoreSide, main);
    fn_1_4BCC(ten, main);
    fn_1_9BC8(&main->tubo, main);
    fn_1_C464(&main->jugem, main);
    fn_1_AEF0(&main->coin, main);
    CharEffectLayerSet(4);
}

void fn_1_396C(M575Main *main)
{
    fn_1_B074(&main->coin);
    fn_1_C5E0(&main->jugem);
    fn_1_9E8C(&main->tubo);
    fn_1_4E98(&main->ten);
    fn_1_3CD8(&main->score);
    fn_1_4360(&main->stage);
    fn_1_8778(&main->camera);
    fn_1_92C8(&main->light);
    memset(main, 0, sizeof(M575Main));
}

s32 fn_1_39E4(M575Main *main)
{
    M575Game *game = main->game;
    M575Ten *ten = &main->ten;

    fn_1_4EF0(ten);
    fn_1_562C(&main->player[0]);
    fn_1_562C(&main->player[1]);
    fn_1_3D28(&main->score);
    fn_1_9EE4(&main->tubo);
    fn_1_C630(&main->jugem);
    fn_1_B0E8(&main->coin);
    fn_1_442C(&main->stage);
    fn_1_87A8(&main->camera);
    fn_1_931C(&main->light);
    return 0;
}
