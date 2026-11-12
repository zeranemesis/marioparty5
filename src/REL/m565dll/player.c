#include "REL/m565dll.h"

/* motion data numbers of each character: 7 entries per character */
#define M565_MOTS(hi) \
    ((hi) << 16) | 0x00, ((hi) << 16) | 0x01, ((hi) << 16) | 0x02, ((hi) << 16) | 0x1A, ((hi) << 16) | 0x2B, \
        ((hi) << 16) | 0x30, ((hi) << 16) | 0x31

u32 lbl_1_data_9B8[92] = {
    M565_MOTS(0x8C), M565_MOTS(0x33), M565_MOTS(0xB7), M565_MOTS(0xED), M565_MOTS(0xE6), M565_MOTS(0x17),
    M565_MOTS(0xE0), M565_MOTS(0x2D), M565_MOTS(0xC9), M565_MOTS(0xB0), M565_MOTS(0x8C), M565_MOTS(0x8C),
    M565_MOTS(0x8C), 0,
};

MGPLAYER *lbl_1_bss_7820[4];

static void fn_1_8024(M565Player *player);

/* one of the nine tiles around the player: remember its coordinates and squared distance */
#define M565_CAND(idx, dc, dr) \
    do { \
        if (player->col + (dc) >= 0 && player->col + (dc) < 10 && player->row + (dr) >= 0 && player->row + (dr) < 10) { \
            colArr[idx] = player->col + (dc); \
            rowArr[idx] = player->row + (dr); \
            c = stage->tiles[player->row + (dr)][player->col + (dc)].pos; \
            dist[idx] = (p.x - c.x) * (p.x - c.x) + (p.z - c.z) * (p.z - c.z); \
        } \
    } while (0)

void fn_1_78BC(M565Player *player, s16 no, M565Sys *sys)
{
    MGACTOR_PARAM param;
    HU3DMODELID fx;
    s32 i;

    memset(player, 0, sizeof(M565Player));
    player->sys = sys;
    player->no = no;
    player->charNo = GwPlayerConf[player->no].charNo;
    player->fallen = 0;
    player->enabled = 0;
    fn_1_9EAC(&player->com, player);
    param.height = 150.0f;
    param.radius = 40.0f;
    param.param = no;
    param.type = 0;
    param.narrowHook = NULL;
    param.attr = 0;
    param.correctHookParam = 0;
    param.correctHook = NULL;
    lbl_1_bss_7820[no] = MgPlayerCreate(no, &param, 8, 1, -0x11, NULL);
    player->mg = lbl_1_bss_7820[no];
    player->mdl = player->mg->actor->mdlId;
    player->pos = &Hu3DData[player->mdl].pos;
    player->rot = &Hu3DData[player->mdl].rot;
    for (i = 0; i < 7; i++) {
        player->mot[i] = CharMotionCreate(player->charNo, lbl_1_data_9B8[player->charNo * 7 + i]);
    }
    Hu3DModelShadowSet(player->mdl);
    fx = Hu3DModelCreateData(0x780073);
    player->fxMdl = fx;
    Hu3DModelPosSet(fx, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(fx, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(fx, 2.0f, 2.0f, 2.0f);
    Hu3DModelCameraSet(fx, 1);
    Hu3DModelLayerSet(fx, 5);
    Hu3DModelAttrSet(fx, 1);
    player->fxMot = Hu3DJointMotionData(fx, 0x78007D);
    Hu3DMotionSet(fx, player->fxMot);
    Hu3DModelAttrReset(fx, 0x40000001);
    MgPlayerVibrateCreate(player->mg);
}

void fn_1_7B8C(M565Player *player)
{
    Hu3DMotionKill(player->fxMot);
    Hu3DModelKill(player->fxMdl);
    fn_1_9FF4(&player->com);
    memset(player, 0, sizeof(M565Player));
}

s32 fn_1_7BDC(M565Player *player)
{
    HuVecF diff;
    f32 vol;
    f32 pan;
    f32 maxTime;
    f32 len;
    f32 best;
    f32 t;
    M565Sys *sys = player->sys;
    M565Stage *stage = &sys->stage;
    M565Game *game = sys->game;
    M565Com *com = &player->com;
    M565Tile *tile;
    M565Player *other;
    M565Player *nearest;
    s32 i;

    fn_1_8024(player);
    if (player->pos->y < -300.0f && player->fallen == 0) {
        fn_1_8984(player, 0);
        Hu3DMotionShiftSet(player->mdl, player->mot[4], 0.0f, 4.0f, 0x40000001);
        player->fallVel = player->mg->actor->velY / 100.0f;
        if (player->fallVel > 0.0f) {
            t = 0.0f;
        } else {
            t = player->fallVel;
        }
        player->fallVel = t;
        Hu3DModelShadowReset(player->mdl);
        player->fallen = 1;
    }
    if (player->fallen != 0) {
        player->fallVel = player->fallVel - 1.5f;
        if (player->fallVel < -225.0f) {
            player->fallVel = -225.0f;
        }
        player->pos->y += player->fallVel;
        tile = &stage->tiles[player->row][player->col];
        if (tile->state == 0x66 && tile->fallY > player->pos->y) {
            tile->fallY = tile->fallY + 0.2f * (player->pos->y - tile->fallY);
        }
        if (player->pos->y < -9999.99f) {
            player->pos->y = -9999.99f;
            player->mg->actor->pos.y = -9999.99f;
        }
    }
    if (com->active != 0 && player->fallen == 0) {
        fn_1_A050(com);
        MgPlayerPadSet(player->mg, com->stkX, com->stkY, com->btnDown, com->btn);
    }
    if (player->enabled == 0 || player->fallen != 0) {
        MgPlayerPadSet(player->mg, 0, 0, 0, 0);
    }
    if (player->fxActive != 0) {
        maxTime = Hu3DMotionMaxTimeGet(player->fxMdl);
        if (Hu3DMotionTimeGet(player->fxMdl) >= maxTime) {
            Hu3DModelAttrSet(player->fxMdl, 1);
            player->fxActive = 0;
        }
    }
    nearest = NULL;
    best = 1e9f;
    for (i = 0; i < 4; i++) {
        other = &sys->players[i];
        if (other != player && other->fallen == 0 && player->fallen == 0) {
            PSVECSubtract(other->pos, player->pos, &diff);
            diff.y = 0.0f;
            len = PSVECMag(&diff);
            if (len < 0.0f) {
                t = 0.0f;
            } else {
                t = len;
            }
            if (t < best) {
                best = t;
                nearest = other;
            }
        }
    }
    player->near = nearest;
    if (nearest != NULL) {
        t = best;
    } else {
        t = 0.0f;
    }
    player->nearDist = t;
    fn_1_A10(player->pos, &vol, &pan);
    if (game->enable != 0) {
        CharModelVoiceVolSet(player->charNo, vol);
    }
    CharModelVoicePanSet(player->charNo, pan);
    return 0;
}

static void fn_1_8024(M565Player *player)
{
    M565Stage *stage = &player->sys->stage;
    f32 dist[9];
    s16 colArr[9];
    s16 rowArr[9];
    HuVecF p;
    HuVecF c;
    f32 bestD;
    f32 d;
    s32 best;
    s32 i;

    p.x = player->pos->x;
    p.y = 0.0f;
    p.z = player->pos->z;
    dist[8] = -9999.99f;
    dist[7] = -9999.99f;
    dist[6] = -9999.99f;
    dist[5] = -9999.99f;
    dist[4] = -9999.99f;
    dist[3] = -9999.99f;
    dist[2] = -9999.99f;
    dist[1] = -9999.99f;
    dist[0] = -9999.99f;
    M565_CAND(0, 0, 0);
    M565_CAND(1, 0, -1);
    M565_CAND(2, 1, -1);
    M565_CAND(3, 1, 0);
    M565_CAND(4, 1, 1);
    M565_CAND(5, 0, 1);
    M565_CAND(6, -1, 1);
    M565_CAND(7, -1, 0);
    M565_CAND(8, -1, -1);
    best = 0;
    bestD = 1e10f;
    for (i = 0; i < 9; i++) {
        if (dist[i] >= 0.0f && dist[i] < bestD) {
            best = i;
            bestD = dist[i];
        }
    }
    player->col = colArr[best];
    player->row = rowArr[best];
    d = dist[best];
    if (d > 0.0f) {
        player->groundDist = sqrtf(d);
    } else {
        player->groundDist = d;
    }
}

void fn_1_8984(M565Player *player, s32 on)
{
    if (on != 0) {
        MgPlayerPauseOff(player->mg, player->pos);
        return;
    }
    MgPlayerPauseOn(player->mg);
}

void fn_1_89D4(M565Player *player, s32 type, f32 angle)
{
    HU3DMODELID mdl;

    if (type != 0) {
        MgPlayerStunSet(player->mg, 2, angle, -1);
    } else {
        MgPlayerStunSet(player->mg, 1, angle, -1);
    }
    if (player->fxActive == 0) {
        mdl = player->fxMdl;
        Hu3DModelPosSet(mdl, player->pos->x, player->pos->y, player->pos->z);
        Hu3DModelRotSet(mdl, 45.0f, angle, 0.0f);
        Hu3DMotionTimeSet(mdl, 0.0f);
        Hu3DModelAttrReset(mdl, 1);
        player->fxActive = 1;
    }
}
