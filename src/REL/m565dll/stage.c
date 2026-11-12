#include "REL/m565dll.h"

typedef struct M565TileInit_s {
    u16 state;
    s32 resA;
    s32 resB;
    s16 kind;
    f32 ox;
    f32 oz;
} M565TileInit;

static void fn_1_5284(M565Stage *stage, M565Tile **pick);
static void fn_1_5700(M565Stage *stage, M565Tile *tile, M565List *group);
static void fn_1_57E4(M565Stage *stage, M565Tile *from, s16 col, s16 row, M565List *group);
static void fn_1_5998(M565Stage *stage);
static void fn_1_5F24(M565Stage *stage, HuVecF *pos, M565Tile **out);
static void fn_1_6140(M565Stage *stage, HuVecF *pos, M565Tile **out);
static void fn_1_6474(M565Stage *stage, M565Tile *tile);
static void fn_1_6540(M565Stage *stage, M565Tile *tile);
static void fn_1_66D4(M565Stage *stage, M565Tile *tile);
static void fn_1_677C(M565Stage *stage, M565Tile *tile);
static void fn_1_68A0(M565Stage *stage, M565Tile *tile);
static void fn_1_7578(M565Stage *stage, HuVecF *pos);
static void fn_1_7624(M565Stage *stage);
static void fn_1_76C0(M565Tile *tile);

/* tile layout: state, model data numbers, kind, centre offset */
M565TileInit lbl_1_data_58[10][10] = {
    {
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x64, 0x780003, 0x780036, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 40.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
    },
    {
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x64, 0x780004, 0x780037, 0, -10.0f, 0.0f },
        { 0x64, 0x780005, 0x780038, 0, 0.0f, -10.0f },
        { 0x0, 0x0, 0x0, 0, -20.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
    },
    {
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, -40.0f, 0.0f },
        { 0x64, 0x780006, 0x780039, 0, 0.0f, 0.0f },
        { 0x64, 0x780007, 0x78003A, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 40.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
    },
    {
        { 0x64, 0x780008, 0x78003B, 0, 0.0f, 0.0f },
        { 0x64, 0x780009, 0x78003C, 0, 20.0f, 0.0f },
        { 0x64, 0x78000A, 0x78003D, 0, 0.0f, 0.0f },
        { 0x64, 0x78000B, 0x78003E, 1, 0.0f, 15.0f },
        { 0x64, 0x78000C, 0x78003F, 1, 0.0f, 0.0f },
        { 0x64, 0x78000D, 0x780040, 1, -20.0f, 15.0f },
        { 0x64, 0x78000E, 0x780041, 1, 0.0f, 0.0f },
        { 0x64, 0x78000F, 0x780042, 0, 0.0f, 0.0f },
        { 0x64, 0x780010, 0x780043, 0, 0.0f, 0.0f },
        { 0x64, 0x780011, 0x780044, 0, 0.0f, -5.0f },
    },
    {
        { 0x0, 0x0, 0x0, 0, -40.0f, 20.0f },
        { 0x64, 0x780012, 0x780045, 0, 0.0f, -10.0f },
        { 0x64, 0x780013, 0x780046, 0, -10.0f, 5.0f },
        { 0x64, 0x780014, 0x780047, 1, 10.0f, 10.0f },
        { 0x64, 0x780015, 0x780048, 0, 0.0f, 0.0f },
        { 0x64, 0x780016, 0x780049, 0, 0.0f, 0.0f },
        { 0x64, 0x780017, 0x78004A, 1, 0.0f, 0.0f },
        { 0x64, 0x780018, 0x78004B, 0, 0.0f, 0.0f },
        { 0x64, 0x780019, 0x78004C, 0, -30.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 40.0f, 20.0f },
    },
    {
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x64, 0x78001A, 0x78004D, 0, 25.0f, 0.0f },
        { 0x64, 0x78001B, 0x78004E, 1, 0.0f, 0.0f },
        { 0x64, 0x78001C, 0x78004F, 0, 0.0f, 0.0f },
        { 0x64, 0x78001D, 0x780050, 0, 0.0f, 0.0f },
        { 0x64, 0x78001E, 0x780051, 0, -10.0f, -10.0f },
        { 0x64, 0x78001F, 0x780052, 0, 0.0f, -15.0f },
        { 0x64, 0x780020, 0x780053, 1, -10.0f, 0.0f },
        { 0x64, 0x780021, 0x780054, 0, -10.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
    },
    {
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x64, 0x780022, 0x780055, 1, 20.0f, 0.0f },
        { 0x64, 0x780023, 0x780056, 1, -10.0f, -20.0f },
        { 0x64, 0x780024, 0x780057, 0, 10.0f, -15.0f },
        { 0x64, 0x780025, 0x780058, 0, -10.0f, -10.0f },
        { 0x64, 0x780026, 0x780059, 1, 0.0f, 0.0f },
        { 0x64, 0x780027, 0x78005A, 1, -20.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
    },
    {
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x64, 0x780028, 0x78005B, 0, 0.0f, -25.0f },
        { 0x64, 0x780029, 0x78005C, 0, 0.0f, -10.0f },
        { 0x64, 0x78002A, 0x78005D, 1, 0.0f, 0.0f },
        { 0x64, 0x78002B, 0x78005E, 1, 0.0f, -15.0f },
        { 0x64, 0x78002C, 0x78005F, 0, -10.0f, 0.0f },
        { 0x64, 0x78002D, 0x780060, 0, -10.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
    },
    {
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x64, 0x78002E, 0x780061, 0, 10.0f, -15.0f },
        { 0x64, 0x78002F, 0x780062, 0, 5.0f, -20.0f },
        { 0x64, 0x780030, 0x780063, 1, 25.0f, -25.0f },
        { 0x64, 0x780031, 0x780064, 1, -10.0f, -10.0f },
        { 0x64, 0x780032, 0x780065, 0, 0.0f, -15.0f },
        { 0x64, 0x780033, 0x780066, 0, 0.0f, -35.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
    },
    {
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x64, 0x780034, 0x780067, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x64, 0x780035, 0x780068, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
        { 0x0, 0x0, 0x0, 0, 0.0f, 0.0f },
    },
};

s32 lbl_1_bss_7818;

void fn_1_4554(M565Stage *stage, M565Sys *sys)
{
    f32 sumX[10];
    f32 sumZ[10];
    s16 cntCol[10];
    s16 cntRow[10];
    M565Tile *tile;
    M565StageSlot *slot;
    HU3DMODELID mdl;
    s16 row;
    s16 col;
    s32 i;

    memset(stage, 0, sizeof(M565Stage));
    stage->sys = sys;
    stage->sndHandle = -1;
    fn_1_0(&stage->colList);
    fn_1_0(&stage->freeList);
    memset(sumX, 0, sizeof(sumX));
    memset(sumZ, 0, sizeof(sumZ));
    memset(cntCol, 0, sizeof(cntCol));
    memset(cntRow, 0, sizeof(cntRow));
    for (row = 0; row < 10; row++) {
        for (col = 0; col < 10; col++) {
            tile = &stage->tiles[row][col];
            memset(tile, 0, sizeof(M565Tile));
            tile->stage = stage;
            tile->state = lbl_1_data_58[row][col].state;
            tile->kind = lbl_1_data_58[row][col].kind;
            tile->col = col;
            tile->row = row;
            if (tile->state == 0x64) {
                mdl = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_58[row][col].resA, HU_MEMNUM_OVL, HEAP_MODEL));
                tile->mdl = mdl;
                Hu3DModelShadowMapSet(mdl);
                Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
                Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
                Hu3DModelCameraSet(mdl, 1);
                Hu3DModelLayerSet(mdl, 0);
                Hu3DModelAttrReset(mdl, 1);
                mdl = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_58[row][col].resB, HU_MEMNUM_OVL, HEAP_MODEL));
                tile->mdl2 = mdl;
                Hu3DModelAttrSet(mdl, 1);
                fn_1_76C0(tile);
                sumX[tile->col] += tile->pos.x;
                sumZ[tile->row] += tile->pos.z;
                cntCol[tile->col] = cntCol[tile->col] + 1;
                cntRow[tile->row] = cntRow[tile->row] + 1;
                fn_1_A0(&stage->colList, &tile->nodeCol, tile);
                fn_1_A0(&stage->freeList, &tile->nodeFree, tile);
            } else {
                tile->mdl = 0;
            }
        }
    }
    for (row = 0; row < 10; row++) {
        for (col = 0; col < 10; col++) {
            tile = &stage->tiles[row][col];
            if (tile->state == 0) {
                tile->pos.x = sumX[tile->col] / (f32)cntCol[tile->col];
                tile->pos.y = 0.0f;
                tile->pos.z = sumZ[tile->row] / (f32)cntRow[tile->row];
                tile->pos.x = tile->pos.x + lbl_1_data_58[tile->row][tile->col].ox;
                tile->pos.z = tile->pos.z + lbl_1_data_58[tile->row][tile->col].oz;
            }
        }
    }
    for (i = 0; i < 10; i++) {
        slot = &stage->slots[i];
        memset(slot, 0, sizeof(M565StageSlot));
        mdl = Hu3DModelCreateData(0x780069);
        slot->mdl = mdl;
        Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
        Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(mdl, 10.0f, 10.0f, 10.0f);
        Hu3DModelCameraSet(mdl, 1);
        Hu3DModelLayerSet(mdl, 4);
        slot->mot = Hu3DJointMotionData(mdl, 0x780077);
        Hu3DMotionSpeedSet(mdl, 2.0f);
        Hu3DMotionSet(mdl, slot->mot);
        Hu3DModelAttrReset(mdl, 0x40000001);
        Hu3DModelAttrSet(mdl, 1);
    }
    fn_1_5998(stage);
}

void fn_1_4AE0(M565Stage *stage)
{
    M565StageSlot *slot;
    M565Tile *tile;
    s32 i;
    s32 row;
    s32 col;

    for (i = 0; i < 10; i++) {
        slot = &stage->slots[i];
        Hu3DMotionKill(slot->mot);
        Hu3DModelKill(slot->mdl);
        memset(slot, 0, sizeof(M565StageSlot));
    }
    fn_1_30(&stage->freeList);
    fn_1_30(&stage->colList);
    for (row = 0; row < 10; row++) {
        for (col = 0; col < 10; col++) {
            tile = &stage->tiles[row][col];
            Hu3DModelKill(tile->mdl2);
            Hu3DModelKill(tile->mdl);
            memset(tile, 0, sizeof(M565Tile));
        }
    }
    memset(stage, 0, sizeof(M565Stage));
}

void fn_1_4BCC(M565Stage *stage)
{
    HuVecF snd;
    f32 vol;
    f32 pan;
    f32 wob;
    f32 add;
    M565Game *game = stage->sys->game;
    M565Node *node;
    M565Node *next;
    M565Tile *tile;
    M565List *grp;
    s32 dcol;
    s32 drow;
    s32 seNo;
    s32 tries;
    s32 g;

    stage->fallFlag = 0;
    for (node = stage->freeList.head; node != NULL; node = node->next) {
        tile = node->data;
        if (tile->state == 0x65) {
            if (game->enable != 0) {
                if (tile->timer <= 0) {
                    fn_1_6474(stage, tile);
                    if (stage->sndHandle >= 0) {
                        fn_1_C40(stage->sndHandle);
                        stage->sndHandle = -1;
                    }
                    if (MgSeqModeGet() == 5 && stage->sndWait <= 0) {
                        fn_1_A10(&tile->pos, &vol, &pan);
                        seNo = fn_1_C18(0x8DB);
                        fn_1_CC8(seNo, vol);
                        fn_1_C98(seNo, pan);
                        stage->sndWait = 1;
                    }
                } else if (tile->timer <= 60) {
                    if (((lbl_1_bss_0->frame / 3) & 1) != 0) {
                        wob = 1.0f;
                    } else {
                        wob = -1.0f;
                    }
                    Hu3DModelPosSet(tile->mdl, 0.0f, 3.0f * wob, 0.0f);
                    if (stage->sndHandle < 0 && MgSeqModeGet() == 5) {
                        fn_1_A10(&tile->pos, &vol, &pan);
                        stage->sndHandle = fn_1_C18(0x8E0);
                        fn_1_CC8(stage->sndHandle, vol);
                        fn_1_C98(stage->sndHandle, pan);
                    }
                }
            } else {
                Hu3DModelPosSet(tile->mdl, 0.0f, 0.0f, 0.0f);
                tile->state = 0x64;
            }
            if (tile->timer != 0) {
                tile->timer = tile->timer - 1;
            }
            if (stage->freeList.count <= 1) {
                tile->state = 0x64;
            }
        } else if (tile->state == 0x66) {
            fn_1_6540(stage, tile);
            stage->fallFlag = 1;
        }
        memset(&tile->nodeGroup, 0, sizeof(M565Node));
        tile->visited = 0;
    }
    for (node = stage->fallList.head; node != NULL; node = next) {
        tile = node->data;
        next = node->next;
        if (tile->fallVel < 200.0f) {
            add = 0.1f + 0.2f * tile->fallVel;
        } else {
            add = 0.0f;
        }
        tile->fallVel = tile->fallVel + add;
        tile->fallY = tile->fallY - tile->fallVel;
        if (tile->fallY <= -10000.0f) {
            tile->fallY = -10000.0f;
            fn_1_E4(&stage->fallList, &tile->nodeFall);
        }
        Hu3DModelPosSet(tile->mdl2, 0.0f, tile->fallY, 0.0f);
    }
    if (stage->pickFlag != 0 && game->enable != 0) {
        stage->groupMax = 0;
        stage->groupNum = 0;
        memset(stage->groups, 0, sizeof(stage->groups));
        for (node = stage->freeList.head; node != NULL; node = node->next) {
            tile = node->data;
            if (tile->visited == 0) {
                grp = &stage->groups[stage->groupNum];
                fn_1_5700(stage, tile, grp);
                if (stage->groupMax < grp->count) {
                    stage->groupMax = grp->count;
                }
                stage->groupNum = stage->groupNum + 1;
            }
        }
        for (g = 0; g < stage->groupNum; g++) {
            grp = &stage->groups[g];
            if (grp->count < 10 && grp->count < stage->groupMax && stage->freeList.count > 9
                && (frand() % 100) < (u32)(100 - grp->count * 10)) {
                for (node = grp->head; node != NULL; node = node->next) {
                    tile = node->data;
                    if (tile->state == 0x64) {
                        tile->state = 0x65;
                        tile->timer = 60;
                    } else if (tile->state == 0x65 && tile->timer > 60) {
                        tile->timer = 60;
                    }
                }
            }
        }
        fn_1_5998(stage);
    }
    if (stage->pick1 == NULL) {
        if (game->timer < 2400 && game->timer > 1200 && (frand() % 10000) < 2000) {
            fn_1_5284(stage, &stage->pick1);
            if (stage->pick1 != NULL) {
                tries = 0;
                for (;;) {
                    stage->pick2 = NULL;
                    fn_1_5284(stage, &stage->pick2);
                    if (stage->pick1 != NULL && stage->pick2 != NULL) {
                        dcol = stage->pick1->col - stage->pick2->col;
                        if (dcol < 0) {
                            dcol = -dcol;
                        }
                        if (dcol > 3) {
                            drow = stage->pick1->row - stage->pick2->row;
                            if (drow < 0) {
                                drow = -drow;
                            }
                            if (drow > 4) {
                                break;
                            }
                        }
                    }
                    tries++;
                    if (tries > 21) {
                        stage->pick2 = NULL;
                        break;
                    }
                }
            }
        } else {
            fn_1_5284(stage, &stage->pick1);
        }
    }
    fn_1_7624(stage);
    if (stage->sndWait != 0) {
        stage->sndWait = stage->sndWait - 1;
    }
    stage->pickFlag = 0;
}

static void fn_1_5284(M565Stage *stage, M565Tile **pick)
{
    M565Sys *sys = stage->sys;
    M565Puku *puku = &sys->puku;
    M565Game *game = sys->game;
    M565Tile *tile;
    f32 val;
    s16 col;
    s16 row;
    u16 lo;
    s16 mix;
    s32 special;
    s32 corner;
    s32 edge;
    s32 tries;
    s32 tries2;
    s32 over;

    if (*pick != NULL) {
        return;
    }
    special = 0;
    if (game->timer > 3000) {
        corner = (frand() % 10000) < 9000;
        edge = 0;
    } else if (game->timer > 2700) {
        corner = (frand() % 10000) < 8000;
        edge = 0;
    } else if (game->timer > 2400) {
        corner = (frand() % 10000) < 6000;
        edge = 0;
    } else if (game->timer > 1800) {
        corner = (frand() % 10000) < 2000;
        edge = 0;
    } else if (game->timer > 600) {
        corner = 0;
        edge = 0;
    } else {
        corner = (frand() % 10000) < 9000;
        edge = 1;
    }
    switch (puku->cntTotal) {
        case 8:
            edge = 0;
            corner = 0;
            special = 1;
            break;
        case 10:
            edge = 0;
            corner = 0;
            special = 1;
            break;
        case 12:
            edge = 0;
            corner = 0;
            special = 1;
            break;
    }
    if (stage->freeList.count <= 9) {
        edge = 1;
        corner = 1;
    }
    tries = 0;
    while (*pick == NULL) {
        if (special != 0) {
            tries2 = 0;
            for (;;) {
                col = frand() % 10;
                tile = &stage->tiles[(s16)(frand() % 10)][col];
                if (tile->state != 0x64 && tile->state != 0x65) {
                    over = tries2 > 20;
                    tries2++;
                    if (!over) {
                        continue;
                    }
                }
                break;
            }
        } else {
            if (edge != 0) {
                if ((frand() & 1) != 0) {
                    col = frand() & 1;
                } else {
                    col = 9 - (frand() & 1);
                }
                if ((frand() & 1) != 0) {
                    row = frand() & 1;
                } else {
                    row = 9 - (frand() & 1);
                }
            } else {
                col = frand() % 10;
                row = frand() % 10;
            }
            tile = &stage->tiles[row][col];
        }
        if (corner != 0) {
            if (tile->state <= 1) {
                *pick = tile;
                return;
            }
        } else {
            if (edge != 0 || (edge == 0 && tile->state != 0)) {
                lo = frand();
                mix = frand() ^ lo;
                val = 100.0f * ((f32)mix / 65535.0f);
                if (tile->kind != 1) {
                    if (val < 1.5f) {
                        *pick = tile;
                    }
                } else if (val < 6.4941406f) {
                    *pick = tile;
                }
            }
        }
        over = tries > 20;
        tries++;
        if (over) {
            break;
        }
    }
}

static void fn_1_5700(M565Stage *stage, M565Tile *tile, M565List *group)
{
    if (tile->visited == 0) {
        fn_1_A0(group, &tile->nodeGroup, tile);
        tile->visited = 1;
        fn_1_57E4(stage, tile, tile->col, tile->row - 1, group);
        fn_1_57E4(stage, tile, tile->col, tile->row + 1, group);
        fn_1_57E4(stage, tile, tile->col - 1, tile->row, group);
        fn_1_57E4(stage, tile, tile->col + 1, tile->row, group);
    }
}

static void fn_1_57E4(M565Stage *stage, M565Tile *from, s16 col, s16 row, M565List *group)
{
    M565Tile *tile;

    if (col >= 0 && col < 10 && row >= 0 && row < 10) {
        tile = &stage->tiles[row][col];
        if ((tile->state == 0x64 || tile->state == 0x65) && tile->visited == 0
            && (from->row != 8 || from->col != 4 || row != 8 || col != 5)
            && (from->row != 8 || from->col != 5 || row != 8 || col != 4) && tile->visited == 0) {
            fn_1_A0(group, &tile->nodeGroup, tile);
            tile->visited = 1;
            fn_1_57E4(stage, tile, tile->col, tile->row - 1, group);
            fn_1_57E4(stage, tile, tile->col, tile->row + 1, group);
            fn_1_57E4(stage, tile, tile->col - 1, tile->row, group);
            fn_1_57E4(stage, tile, tile->col + 1, tile->row, group);
        }
    }
}

/* count one more standing neighbour for the tile at (c, r) if it is standing */
#define M565_NB(c, r) \
    do { \
        if ((c) >= 0 && (c) < 10 && (r) >= 0 && (r) < 10) { \
            nb = &stage->tiles[(r)][(c)]; \
            if (nb->state == 0x64 || nb->state == 0x65) { \
                nb->neighbors = nb->neighbors + 1; \
            } \
        } \
    } while (0)

static void fn_1_5998(M565Stage *stage)
{
    M565Tile *tile;
    M565Tile *nb;
    s16 row;
    s16 col;

    for (row = 0; row < 10; row++) {
        for (col = 0; col < 10; col++) {
            tile = &stage->tiles[row][col];
            tile->neighbors = 0;
            if (tile->state == 0x64 || tile->state == 0x65) {
                M565_NB(col, row);
                M565_NB(col - 1, row);
                M565_NB(col + 1, row);
                M565_NB(col - 1, row - 1);
                M565_NB(col + 1, row - 1);
                M565_NB(col - 1, row + 1);
                M565_NB(col + 1, row + 1);
                M565_NB(col, row - 1);
                M565_NB(col, row + 1);
            }
        }
    }
}

void fn_1_5EB0(M565Stage *stage, HuVecF *pos, s32 big, M565Tile **out)
{
    if (big == 0) {
        fn_1_5F24(stage, pos, out);
    } else {
        fn_1_6140(stage, pos, out);
    }
}

static void fn_1_5F24(M565Stage *stage, HuVecF *pos, M565Tile **out)
{
    HuVecF diff;
    HuVecF hit;
    f32 vol;
    f32 pan;
    f32 best;
    f32 dist;
    M565Tile *found;
    M565Tile *tile;
    s16 row;
    s16 col;
    s32 seNo;
    s32 done;

    found = NULL;
    best = 50000.0f;
    for (row = 0; row < 10; row++) {
        for (col = 0; col < 10; col++) {
            tile = &stage->tiles[row][col];
            PSVECSubtract(pos, &tile->pos, &diff);
            diff.y = 0.0f;
            dist = PSVECMag(&diff);
            if (dist < 200.0f && dist < best) {
                best = dist;
                found = tile;
            }
        }
    }
    done = 0;
    if (out != NULL) {
        out[0] = found;
        out[1] = NULL;
        out[2] = NULL;
        out[3] = NULL;
        return;
    }
    if (found != NULL) {
        switch (found->state) {
            case 0x64:
            case 0x65:
                fn_1_677C(stage, found);
                done = 1;
                break;
            case 0x66:
                if (pos->y < found->fallY) {
                    hit.x = found->pos.x;
                    hit.y = pos->y;
                    hit.z = found->pos.z;
                    fn_1_7578(stage, &hit);
                    fn_1_66D4(stage, found);
                    done = 1;
                }
                break;
        }
        if (done != 0 && MgSeqModeGet() == 5) {
            fn_1_A10(&found->pos, &vol, &pan);
            seNo = fn_1_C18(0x8DA);
            fn_1_CC8(seNo, vol);
            fn_1_C98(seNo, pan);
        }
    }
}

static void fn_1_6140(M565Stage *stage, HuVecF *pos, M565Tile **out)
{
    M565Tile *found[4];
    f32 dists[4];
    HuVecF diff;
    HuVecF hit;
    f32 vol;
    f32 pan;
    f32 dist;
    M565Tile *tile;
    s16 row;
    s16 col;
    s32 k;
    s32 idx;
    s32 seNo;
    s32 done;
    s32 played;

    found[3] = NULL;
    found[2] = NULL;
    found[1] = NULL;
    found[0] = NULL;
    dists[3] = 50000.0f;
    dists[2] = 50000.0f;
    dists[1] = 50000.0f;
    dists[0] = 50000.0f;
    for (row = 0; row < 10; row++) {
        for (col = 0; col < 10; col++) {
            tile = &stage->tiles[row][col];
            PSVECSubtract(pos, &tile->pos, &diff);
            diff.y = 0.0f;
            dist = PSVECMag(&diff);
            for (k = 0; k < 4;) {
                idx = (k + lbl_1_bss_7818) % 4;
                if (dist < 200.0f && dist < dists[idx]) {
                    dists[idx] = dist;
                    found[idx] = tile;
                    lbl_1_bss_7818 += 1;
                    break;
                }
                k++;
            }
        }
    }
    played = 0;
    done = 0;
    if (out != NULL) {
        out[0] = found[0];
        out[1] = found[1];
        out[2] = found[2];
        out[3] = found[3];
        return;
    }
    for (k = 0; k < 4; k++) {
        if (found[k] != NULL) {
            if (found[k]->state == 0x64 || found[k]->state == 0x65) {
                fn_1_677C(stage, found[k]);
                done = 1;
            } else if (found[k]->state == 0x66 && pos->y < found[k]->fallY) {
                hit.x = found[k]->pos.x;
                hit.y = pos->y;
                hit.z = found[k]->pos.z;
                fn_1_7578(stage, &hit);
                fn_1_66D4(stage, found[k]);
                done = 1;
            }
            if (done != 0 && MgSeqModeGet() == 5 && played == 0) {
                fn_1_A10(&found[k]->pos, &vol, &pan);
                seNo = fn_1_C18(0x8DA);
                fn_1_CC8(seNo, vol);
                fn_1_C98(seNo, pan);
                played++;
            }
        }
    }
}

static void fn_1_6474(M565Stage *stage, M565Tile *tile)
{
    Hu3DModelPosSet(tile->mdl, 0.0f, 0.0f, 0.0f);
    tile->fallY = 0.0f;
    tile->fallVel = 0.1f * ((f32)(frand() % 10000) / 10000.0f);
    tile->state = 0x66;
}

static void fn_1_6540(M565Stage *stage, M565Tile *tile)
{
    f32 wob;
    f32 add;

    if (((lbl_1_bss_0->frame / 3) & 1) != 0) {
        wob = 1.0f;
    } else {
        wob = -1.0f;
    }
    if (tile->fallVel < 100.0f) {
        add = 0.4f + 0.02f * tile->fallVel;
    } else {
        add = 0.0f;
    }
    tile->fallVel = tile->fallVel + add;
    tile->fallY = tile->fallY - tile->fallVel;
    Hu3DModelPosSet(tile->mdl, 0.0f, tile->fallY + 3.0f * wob, 0.0f);
    Hu3DModelPosSet(tile->mdl2, 0.0f, tile->fallY, 0.0f);
    if (tile->fallY <= -10000.0f) {
        fn_1_66D4(stage, tile);
    }
}

static void fn_1_66D4(M565Stage *stage, M565Tile *tile)
{
    Hu3DModelAttrSet(tile->mdl, 1);
    Hu3DModelPosSet(tile->mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelPosSet(tile->mdl2, 0.0f, -10000.0f, 0.0f);
    fn_1_E4(&stage->freeList, &tile->nodeFree);
    tile->state = 1;
}

static void fn_1_677C(M565Stage *stage, M565Tile *tile)
{
    HuVecF p;
    f32 vel;

    if (stage->freeList.count > 1) {
        if (stage->sndHandle >= 0) {
            fn_1_C40(stage->sndHandle);
            stage->sndHandle = -1;
        }
        Hu3DModelAttrSet(tile->mdl, 1);
        tile->state = 1;
        tile->fallY = -50.0f;
        if (tile->fallVel < 5.0f) {
            vel = 5.0f;
        } else {
            vel = tile->fallVel;
        }
        tile->fallVel = vel;
        fn_1_A0(&stage->fallList, &tile->nodeFall, tile);
        fn_1_E4(&stage->freeList, &tile->nodeFree);
        p.x = tile->pos.x;
        p.y = tile->pos.y;
        p.z = tile->pos.z;
        p.y = 0.0f;
        fn_1_7578(stage, &p);
        fn_1_68A0(stage, tile);
        stage->pickFlag = 1;
    }
}

/* tile at (c, r) is standing and not yet warned: start its collapse countdown */
#define M565_WARN(c, r) \
    do { \
        victim = &stage->tiles[(r)][(c)]; \
        if (victim->state == 0x64) { \
            victim->state = 0x65; \
            victim->timer = (s16)(60.0f + 540.0f * ((f32)(rand8() % 256) / 255.0f)); \
        } \
    } while (0)

#define M565_INR(c, r) ((c) >= 0 && (c) < 10 && (r) >= 0 && (r) < 10)

static void fn_1_68A0(M565Stage *stage, M565Tile *tile)
{
    M565Tile *victim;
    s16 col = tile->col;
    s16 row = tile->row;
    s32 roll;

    if (M565_INR(col, row - 2) && stage->tiles[row - 2][col].state == 1) {
        M565_WARN(col, row - 1);
        return;
    }
    if (M565_INR(col + 2, row) && stage->tiles[row][col + 2].state == 1) {
        M565_WARN(col + 1, row);
        return;
    }
    if (M565_INR(col, row + 2) && stage->tiles[row + 2][col].state == 1) {
        M565_WARN(col, row + 1);
        return;
    }
    if (M565_INR(col - 2, row) && stage->tiles[row][col - 2].state == 1) {
        M565_WARN(col - 1, row);
        return;
    }
    if (M565_INR(col + 1, row - 1) && stage->tiles[row - 1][col + 1].state == 1) {
        roll = rand8() % 100;
        if (roll < 30 || roll >= 60) {
            victim = &stage->tiles[row - 1][col];
            if (victim->state == 0x64) {
                victim->state = 0x65;
                victim->timer = (s16)(60.0f + 540.0f * ((f32)(rand8() % 256) / 255.0f));
            } else {
                roll = 60;
            }
        }
        if (roll >= 30) {
            M565_WARN(col + 1, row);
        }
        return;
    }
    if (M565_INR(col + 1, row + 1) && stage->tiles[row + 1][col + 1].state == 1) {
        roll = rand8() % 100;
        if (roll < 30 || roll >= 60) {
            victim = &stage->tiles[row][col + 1];
            if (victim->state == 0x64) {
                victim->state = 0x65;
                victim->timer = (s16)(60.0f + 540.0f * ((f32)(rand8() % 256) / 255.0f));
            } else {
                roll = 60;
            }
        }
        if (roll >= 30) {
            M565_WARN(col, row + 1);
        }
        return;
    }
    if (M565_INR(col - 1, row + 1) && stage->tiles[row + 1][col - 1].state == 1) {
        roll = rand8() % 100;
        if (roll < 30 || roll >= 60) {
            victim = &stage->tiles[row + 1][col];
            if (victim->state == 0x64) {
                victim->state = 0x65;
                victim->timer = (s16)(60.0f + 540.0f * ((f32)(rand8() % 256) / 255.0f));
            } else {
                roll = 60;
            }
        }
        if (roll >= 30) {
            M565_WARN(col - 1, row);
        }
        return;
    }
    if (M565_INR(col - 1, row) && stage->tiles[row][col - 1].state == 1) {
        roll = rand8() % 100;
        if (roll < 30 || roll >= 60) {
            victim = &stage->tiles[row][col - 1];
            if (victim->state == 0x64) {
                victim->state = 0x65;
                victim->timer = (s16)(60.0f + 540.0f * ((f32)(rand8() % 256) / 255.0f));
            } else {
                roll = 60;
            }
        }
        if (roll >= 30) {
            M565_WARN(col, row - 1);
        }
    }
}

static void fn_1_7578(M565Stage *stage, HuVecF *pos)
{
    M565StageSlot *slot;
    s32 i;

    for (i = 0; i < 10; i++) {
        slot = &stage->slots[i];
        if (slot->active == 0) {
            Hu3DModelPosSet(slot->mdl, pos->x, pos->y, pos->z);
            Hu3DMotionTimeSet(slot->mdl, 0.0f);
            Hu3DModelAttrReset(slot->mdl, 1);
            slot->active = 1;
            return;
        }
    }
}

static void fn_1_7624(M565Stage *stage)
{
    M565StageSlot *slot;
    f32 maxTime;
    s32 i;

    for (i = 0; i < 10; i++) {
        slot = &stage->slots[i];
        if (slot->active != 0) {
            maxTime = Hu3DMotionMaxTimeGet(slot->mdl);
            if (Hu3DMotionTimeGet(slot->mdl) >= maxTime) {
                Hu3DModelAttrSet(slot->mdl, 1);
                slot->active = 0;
            }
        }
    }
}

static void fn_1_76C0(M565Tile *tile)
{
    HSFBUFFER *vtx = Hu3DData[tile->mdl2].hsf->vertex;
    Vec *v = vtx->data;
    HuVecF sum;
    s32 n;
    s32 i;

    n = 0;
    sum.x = 0.0f;
    sum.y = 0.0f;
    sum.z = 0.0f;
    for (i = 0; i < vtx->count; i++) {
        if (v[i].y > -20.0f) {
            sum.x += v[i].x;
            sum.y += v[i].y;
            sum.z += v[i].z;
            n++;
        }
    }
    tile->pos.x = sum.x / (f32)n;
    tile->pos.y = sum.y / (f32)n;
    tile->pos.z = sum.z / (f32)n;
    tile->pos.x += lbl_1_data_58[tile->row][tile->col].ox;
    tile->pos.z += lbl_1_data_58[tile->row][tile->col].oz;
}
