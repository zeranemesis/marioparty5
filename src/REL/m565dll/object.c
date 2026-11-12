#include "REL/m565dll.h"

s32 lbl_1_bss_7810;

static M565PukuItem *fn_1_3020(M565Sys *sys, M565Tile *tile, s32 big);

/* put a player on the centre of tile (col,row), standing on the ground */
#define M565_PLACE(player, c, r) \
    do { \
        M565Player *pl = (player); \
        HuVecF tilePos; \
        pl->col = (c); \
        pl->row = (r); \
        tilePos = sys->stage.tiles[pl->row][pl->col].pos; \
        tilePos.y = 0.0f; \
        pl->pos->x = tilePos.x; \
        pl->mg->actor->pos.x = tilePos.x; \
        pl->pos->y = 0.0f; \
        pl->mg->actor->pos.y = 0.0f; \
        pl->pos->z = tilePos.z; \
        pl->mg->actor->pos.z = tilePos.z; \
    } while (0)

void fn_1_25B4(M565Sys *sys, M565Game *game)
{
    HU3DMODELID colMdl[100];
    HuVecF zero;
    HU3DMODELID mdl;
    M565Node *node;
    s16 colNum;
    s32 i;
    M565Stage *stage = &sys->stage;

    memset(sys, 0, sizeof(M565Sys));
    sys->game = game;
    fn_1_FCBC(&sys->light, sys);
    fn_1_3690(&sys->back, sys);
    fn_1_4554(stage, sys);
    fn_1_8AD0(&sys->puku, sys);
    mdl = Hu3DModelCreateData(0x78006A);
    sys->mdl = mdl;
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(mdl, 1);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelShadowMapSet(mdl);
    sys->mot = Hu3DJointMotionData(mdl, 0x780078);
    Hu3DModelAttrReset(mdl, 0x40000001);
    Hu3DModelAttrReset(mdl, 1);
    Hu3DMotionSpeedSet(mdl, 0.0f);
    Hu3DMotionSet(mdl, sys->mot);
    colNum = 0;
    for (node = stage->colList.head; node != NULL; node = node->next) {
        colMdl[colNum] = node->data->mdl2;
        colNum++;
    }
    ColKill();
    MgActorColMapInit(colMdl, colNum, 40);
    for (i = 0; i < 4; i++) {
        fn_1_78BC(&sys->players[i], i, sys);
    }
    M565_PLACE(&sys->players[0], 3, 4);
    M565_PLACE(&sys->players[1], 6, 4);
    M565_PLACE(&sys->players[2], 3, 7);
    M565_PLACE(&sys->players[3], 6, 7);
    MgActorExec();
    sys->flag3824 = 1;
    sys->flag3828 = 1;
    CharEffectLayerSet(4);
    zero.x = 0.0f;
    zero.y = 0.0f;
    zero.z = 0.0f;
    sys->camOfs = zero;
    sys->camBase.x = Center.x;
    sys->camBase.y = Center.y;
    sys->camBase.z = Center.z;
}

void fn_1_2A94(M565Sys *sys)
{
    s32 i;

    Hu3DMotionKill(sys->mot);
    Hu3DModelKill(sys->mdl);
    fn_1_8B44(&sys->puku);
    for (i = 0; i < 4; i++) {
        fn_1_7B8C(&sys->players[i]);
    }
    fn_1_4AE0(&sys->stage);
    fn_1_3E54(&sys->back);
    memset(sys, 0, sizeof(M565Sys));
}

s32 fn_1_2B20(M565Sys *sys)
{
    f32 vol1, pan1;
    f32 vol2, pan2;
    f32 vol3, pan3;
    f32 vol4, pan4;
    M565Game *game = sys->game;
    M565Stage *stage = &sys->stage;
    M565Player *player;
    M565PukuItem *item;
    f32 maxTime;
    f32 shakeVal;
    s32 ret = 0;
    s32 alive;
    s32 big;
    s32 seNo;
    s32 i;
    s32 tmp;

    fn_1_8BA8(&sys->puku);
    alive = 0;
    for (i = 0; i < 4; i++) {
        player = &sys->players[i];
        fn_1_7BDC(player);
        if (player->fallen == 0) {
            alive++;
        }
    }
    if (alive <= 1) {
        ret = 1;
    }
    fn_1_4BCC(stage);
    fn_1_3F14(&sys->back);
    tmp = lbl_1_bss_7810;
    lbl_1_bss_7810 = tmp + 1;
    if (tmp >= (s16)(game->timer / 60 + 30)) {
        if (MgSeqModeGet() == 5) {
            if (stage->pick1 != NULL && stage->pick2 != NULL) {
                big = 1;
                if (sys->puku.cntBig >= 2) {
                    big = 0;
                }
                item = fn_1_3020(sys, stage->pick1, big);
                if (item != NULL) {
                    fn_1_A10(&item->pos, &vol1, &pan1);
                    seNo = fn_1_C18(0x8DE);
                    fn_1_CC8(seNo, vol1);
                    fn_1_C98(seNo, pan1);
                }
                item = fn_1_3020(sys, stage->pick2, big);
                if (item != NULL) {
                    fn_1_A10(&item->pos, &vol2, &pan2);
                    seNo = fn_1_C18(0x8DE);
                    fn_1_CC8(seNo, vol2);
                    fn_1_C98(seNo, pan2);
                }
                stage->pick1 = NULL;
                stage->pick2 = NULL;
            } else if (stage->pick1 != NULL) {
                big = (frand() % 10000) / 5.0f < 50.0f / (0.0f + (game->timer / 60));
                if (sys->puku.cntBig >= 2) {
                    big = 0;
                }
                item = fn_1_3020(sys, stage->pick1, big);
                stage->pick1 = NULL;
                if (big != 0) {
                    if (item != NULL) {
                        fn_1_A10(&item->pos, &vol3, &pan3);
                        seNo = fn_1_C18(0x8DE);
                        fn_1_CC8(seNo, vol3);
                        fn_1_C98(seNo, pan3);
                    }
                } else if (item != NULL) {
                    fn_1_A10(&item->pos, &vol4, &pan4);
                    seNo = fn_1_C18(0x8DD);
                    fn_1_CC8(seNo, vol4);
                    fn_1_C98(seNo, pan4);
                }
            }
        }
        lbl_1_bss_7810 = 0;
    }
    if (sys->flag3828 != 0) {
        maxTime = Hu3DMotionMaxTimeGet(sys->mdl);
        if (Hu3DMotionTimeGet(sys->mdl) >= maxTime) {
            Hu3DModelAttrSet(sys->mdl, 1);
            sys->flag3828 = 0;
        }
    }
    if (sys->shakeCnt != 0) {
        shakeVal = sys->shakeAmp * sin(3.141592653589793 * sys->shakeAngle / 180.0);
        sys->shakeAngle = sys->shakeAngle + sys->shakeSpeed;
        sys->shakeAmp = sys->shakeAmp * sys->shakeDecay;
        sys->shakeCnt = sys->shakeCnt - 1;
        if (sys->shakeCnt == 0) {
            sys->shakeAngle = 0.0f;
            sys->shakeSpeed = 0.0f;
            sys->shakeDecay = 0.0f;
            sys->shakeAmp = 0.0f;
        }
        sys->camOfs.x = shakeVal;
    }
    Center.x = sys->camBase.x + sys->camOfs.x;
    Center.y = sys->camBase.y + sys->camOfs.y;
    Center.z = sys->camBase.z + sys->camOfs.z;
    return ret;
}

static M565PukuItem *fn_1_3020(M565Sys *sys, M565Tile *tile, s32 big)
{
    M565Tile *out[4];
    HuVecF dir;
    HuVecF scaled;
    HuVecF from;
    HuVecF to;
    f32 reach;
    f32 mag;
    M565PukuItem *item;
    s32 i;

    if (big != 0) {
        reach = 1300.0f;
    } else {
        reach = 1200.0f;
    }
    dir.x = tile->pos.x;
    dir.y = 0.0f;
    dir.z = tile->pos.z;
    mag = PSVECMag(&dir);
    PSVECNormalize(&dir, &dir);
    if (reach < 200.0f + mag) {
        reach = 200.0f + mag;
    }
    PSVECScale(&dir, &scaled, reach);
    from = scaled;
    from.y = -800.0f;
    to.x = tile->pos.x;
    to.y = tile->pos.y;
    to.z = tile->pos.z;
    to.y = -1000.0f;
    item = fn_1_8C70(&sys->puku, &from, &to, big, 500.0f);
    if (item == NULL) {
        return NULL;
    }
    if (big != 0) {
        item->t30 = 60;
    }
    item->target = tile;
    fn_1_5EB0(&sys->stage, &to, big, out);
    if (big != 0) {
        for (i = 0; i < 4; i++) {
            if (out[i] != NULL) {
                out[i]->hitFlag = 1;
                out[i]->hitItem = item;
            }
        }
    } else if (out[0] != NULL) {
        out[0]->hitFlag = 1;
        out[0]->hitItem = item;
    }
    return item;
}

M565PukuItem *fn_1_3284(M565Sys *sys, s32 big, f64 angle, f32 scale)
{
    M565Tile *out[4];
    HuVecF dir;
    HuVecF scaled;
    HuVecF from;
    HuVecF to;
    f32 reach;
    M565PukuItem *item;
    s32 i;

    if (big != 0) {
        reach = 1300.0f;
    } else {
        reach = 1200.0f;
    }
    dir.x = sin(3.141592653589793 * angle / 180.0);
    dir.y = 0.0f;
    dir.z = cos(3.141592653589793 * angle / 180.0);
    PSVECScale(&dir, &scaled, reach);
    from = scaled;
    from.y = -800.0f;
    PSVECScale(&dir, &scaled, reach * scale);
    to = scaled;
    to.y = -1000.0f;
    item = fn_1_8C70(&sys->puku, &from, &to, big, 500.0f);
    if (item == NULL) {
        return NULL;
    }
    if (big != 0) {
        item->t30 = 60;
    }
    item->target = NULL;
    fn_1_5EB0(&sys->stage, &to, big, out);
    if (big != 0) {
        for (i = 0; i < 4; i++) {
            if (out[i] != NULL) {
                out[i]->hitFlag = 1;
                out[i]->hitItem = item;
            }
        }
    } else if (out[0] != NULL) {
        out[0]->hitFlag = 1;
        out[0]->hitItem = item;
    }
    return item;
}

void fn_1_351C(M565Sys *sys)
{
    M565Stage *stage = &sys->stage;
    M565Tile *tile;
    s32 row;
    s32 col;

    Hu3DModelAttrReset(sys->mdl, 1);
    Hu3DMotionTimeSet(sys->mdl, 0.0f);
    Hu3DMotionSpeedSet(sys->mdl, 0.0f);
    sys->flag3824 = 1;
    for (row = 0; row < 10; row++) {
        for (col = 0; col < 10; col++) {
            tile = &stage->tiles[row][col];
            if (tile->state != 0x64 && tile->state != 0) {
                Hu3DModelPosSet(tile->mdl, 0.0f, 0.0f, 0.0f);
                Hu3DModelAttrReset(tile->mdl, 1);
                if (tile->state == 1) {
                    fn_1_A0(&stage->freeList, &tile->nodeFree, tile);
                }
                tile->state = 0x64;
                Hu3DModelPosSet(tile->mdl2, 0.0f, 0.0f, 0.0f);
            }
        }
    }
}

void fn_1_366C(M565Sys *sys, s32 cnt, f32 amp, f32 decay, f32 speed)
{
    sys->shakeCnt = cnt;
    sys->shakeAmp = amp;
    sys->shakeDecay = decay;
    sys->shakeSpeed = speed;
    sys->shakeAngle = 90.0f;
}
