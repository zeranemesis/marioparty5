#include "REL/m565dll.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

void ObjectSetup(void);
static void fn_1_3C8(s16 mode, s16 frameNo);
static void fn_1_43C(s16 mode, s16 frameNo);
static void fn_1_498(s16 mode, s16 frameNo);
static void fn_1_4EC(s16 mode, s16 frameNo);
static void fn_1_548(s16 mode, s16 frameNo);
static void fn_1_59C(s16 mode, s16 frameNo);
static void fn_1_5EC(s16 mode, s16 frameNo);
static void fn_1_820(s16 mode, s16 frameNo);
static void fn_1_864(s16 mode, s16 frameNo);
static void fn_1_8B8(s16 mode, s16 frameNo);

s32 lbl_1_data_0 = 1;

MGSEQ_PARAM lbl_1_data_4 = {
    60, 0, fn_1_3C8, fn_1_498, fn_1_4EC, fn_1_59C, fn_1_5EC, fn_1_820, fn_1_864, fn_1_8B8, fn_1_43C,
};

HuVecF lbl_1_data_30 = { 1000.0f, 7000.0f, 1000.0f };
HuVecF lbl_1_data_3C = { 0.0f, 0.0f, -1.0f };
HuVecF lbl_1_data_48 = { 0.0f, 0.0f, 0.0f };

M565Game *lbl_1_bss_0;
OMOBJMAN *lbl_1_bss_4;

void fn_1_0(M565List *list)
{
    memset(list, 0, sizeof(M565List));
}

void fn_1_30(M565List *list)
{
    M565Node *node;
    M565Node *next;

    node = list->head;
    while (node) {
        next = node->next;
        memset(node, 0, sizeof(M565Node));
        node = next;
    }
    memset(list, 0, sizeof(M565List));
}

void fn_1_A0(M565List *list, M565Node *node, M565Tile *data)
{
    node->data = data;
    if (!list->head) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }
    node->list = list;
    list->count = list->count + 1;
}

void fn_1_E4(M565List *list, M565Node *node)
{
    if (node == list->head) {
        list->head = node->next;
        if (list->head) {
            list->head->prev = NULL;
        }
    } else if (node == list->tail) {
        list->tail = node->prev;
        if (list->tail) {
            list->tail->next = NULL;
        }
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    node->list = NULL;
    node->prev = NULL;
    node->next = NULL;
    list->count = list->count - 1;
}

int _prolog(void)
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
    CRot.x = -55.0f;
    CRot.y = 0.0f;
    CRot.z = 0.0f;
    Center.x = 0.0f;
    Center.y = 0.0f;
    Center.z = 20.0f;
    CZoom = 1800.0f;
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 45.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    omAddObjEx(lbl_1_bss_4, 0x7FDA, 0, 0, -1, omOutView);
    MgSeqCreate(&lbl_1_data_4);
    MgSeqModeHookAdd(4, fn_1_548);
}

static void fn_1_3C8(s16 mode, s16 frameNo)
{
    M565Game *game = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M565Game), HU_MEMNUM_OVL);

    lbl_1_bss_0 = game;
    memset(lbl_1_bss_0, 0, sizeof(M565Game));
    fn_1_E48(game, lbl_1_bss_4);
    MgSeqModeNext();
}

static void fn_1_43C(s16 mode, s16 frameNo)
{
    M565Game *game = lbl_1_bss_0;

    fn_1_F48(game);
    HuMemDirectFree(lbl_1_bss_0);
    lbl_1_bss_0 = NULL;
    MgSeqModeNext();
}

static void fn_1_498(s16 mode, s16 frameNo)
{
    M565Game *game = lbl_1_bss_0;

    MgSeqModeChangeOff();
    if (fn_1_104C(game) != 0) {
        MgSeqModeNext();
    }
    fn_1_FA8(game, 0);
}

static void fn_1_4EC(s16 mode, s16 frameNo)
{
    M565Game *game = lbl_1_bss_0;
    s16 mesId;

    mesId = MgSeqGameMesIdGet();
    if (game->bgm < 0) {
        game->bgm = fn_1_DC8(0x37);
    }
    fn_1_FA8(game, 0);
}

static void fn_1_548(s16 mode, s16 frameNo)
{
    M565Game *game = lbl_1_bss_0;

    fn_1_1074(game);
    fn_1_FA8(game, 0);
    lbl_1_data_0 = 1;
}

static void fn_1_59C(s16 mode, s16 frameNo)
{
    M565Game *game = lbl_1_bss_0;

    game->enable = 1;
    if (fn_1_FA8(game, 1) != 0) {
        MgSeqModeNext();
    }
}

static void fn_1_5EC(s16 mode, s16 frameNo)
{
    M565Game *game = lbl_1_bss_0;
    M565Sys *sys = &game->sys;
    M565Stage *stage = &sys->stage;
    M565Player *player;
    M565PukuItem *item;
    M565Tile *tile;
    s16 mesId;
    s32 i;

    if (lbl_1_data_0 != 0) {
        for (i = 0; i < 4; i++) {
            player = &sys->players[i];
            player->enabled = 0;
            if (player->fallen == 0) {
                tile = &stage->tiles[player->row][player->col];
                if ((tile->state == 0x64 || tile->state == 0x65) && player->mg->mode == 0
                    && player->pos->y >= -2.0f && player->pos->y < 5.0f) {
                    fn_1_8984(player, 0);
                    Hu3DMotionSet(player->mdl, player->mot[0]);
                    Hu3DModelAttrSet(player->mdl, 0x40000001);
                    Hu3DModelAttrReset(player->mdl, 1);
                    Hu3DModelScaleSet(player->mdl, 1.0f, 1.0f, 1.0f);
                }
            }
        }
        for (i = 0; i < 5; i++) {
            item = &sys->puku.items[i];
            if (item->active != 0) {
                item->frozen = 1;
                item->blink = 60;
            }
        }
        MgPlayerVibrateKill(sys->players[3].mg);
        lbl_1_data_0 = 0;
        game->step = 0;
    }
    mesId = MgSeqGameMesIdGet();
    if (game->bgm >= 0 && (GameMesStatGet(mesId) & 0x10) != 0) {
        fn_1_D78(game->bgm, 100);
        game->bgm = -1;
        if (stage->sndHandle >= 0) {
            fn_1_C40(stage->sndHandle);
            stage->sndHandle = -1;
        }
    }
    game->enable = 0;
    fn_1_FA8(game, 0);
}

static void fn_1_820(s16 mode, s16 frameNo)
{
    M565Game *game = lbl_1_bss_0;

    MgSeqModeChangeOff();
    MgSeqModeNext();
    fn_1_FA8(game, 0);
}

static void fn_1_864(s16 mode, s16 frameNo)
{
    M565Game *game = lbl_1_bss_0;

    MgSeqModeChangeOff();
    if (fn_1_10E0(game) != 0) {
        MgSeqModeNext();
    }
    fn_1_FA8(game, 0);
}

static void fn_1_8B8(s16 mode, s16 frameNo)
{
    M565Game *game = lbl_1_bss_0;

    fn_1_FA8(game, 0);
}

f32 fn_1_8F4(f32 a, f32 b, f32 t)
{
    f32 diff;
    f32 res;

    diff = fmod(b - a, 360.0);
    if (diff < 0.0f) {
        diff += 360.0f;
    }
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    res = fmod(a + t * diff, 360.0);
    if (res < 0.0f) {
        res += 360.0f;
    }
    return res;
}

static inline f32 M565Clamp(f32 v, f32 lo, f32 hi)
{
    if (v > hi) {
        return hi;
    }
    if (v < lo) {
        return lo;
    }
    return v;
}

void fn_1_A10(HuVecF *pos, f32 *vol, f32 *pan)
{
    HuVecF scr;
    f32 x;
    f32 z;

    Hu3D3Dto2D(pos, 1, &scr);
    x = M565Clamp(2.0f * (scr.x / 576.0f - 0.5f), -1.0f, 1.0f);
    z = M565Clamp((pos->z - -550.0f) / 1100.0f, 0.0f, 1.0f);
    *vol = 96.0f + 31.0f * z;
    *pan = 64.0f + 16.0f * x;
}

int fn_1_C18(int seId)
{
    return HuAudFXPlay(seId);
}

void fn_1_C40(int seNo)
{
    HuAudFXStop(seNo);
}

void fn_1_C68(int seNo, s32 speed)
{
    HuAudFXFadeOut(seNo, speed);
}

void fn_1_C98(int seNo, s16 pan)
{
    HuAudFXPanning(seNo, pan);
}

void fn_1_CC8(int seNo, s16 vol)
{
    HuAudFXVolSet(seNo, vol);
}

void fn_1_CF8(Vec *pos, Vec *heading, f32 a, f32 b, f32 c, f32 d, f32 e)
{
    HuAudFXListnerSetEX(pos, heading, a, b, c, d, e);
}

void fn_1_D50(s16 id)
{
    HuAudSStreamPlay(id);
}

void fn_1_D78(int streamNo, s32 speed)
{
    HuAudSStreamFadeOut(streamNo, speed);
}

void fn_1_DA8(void)
{
    HuAudAllStop();
}

int fn_1_DC8(s16 id)
{
    return HuAudBGMPlay(id);
}

void fn_1_DF0(s16 id)
{
    HuAudJinglePlay(id);
}

void fn_1_E18(s16 id, s16 chan)
{
    HuAudSStreamChanPlay(id, chan);
}
