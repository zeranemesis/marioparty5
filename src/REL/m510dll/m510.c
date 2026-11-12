#include "REL/m510dll.h"
#include "string.h"
#include "game/flag.h"
#include "game/mg/seqman.h"
#include "game/mg/actman.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

// Functions of opening.c that are not declared in the header
void fn_1_959C(u8 *openWork, M510_OBJ *obj);
void fn_1_9614(u8 *openWork);
void fn_1_9668(u8 *openWork);

// Debug drawing primitives
typedef struct DbgLine_s {
    M510_VECF a;
    M510_VECF b;
} DBG_LINE;

typedef struct DbgTri_s {
    M510_VECF a;
    M510_VECF b;
    M510_VECF c;
} DBG_TRI;

typedef struct DbgQuad_s {
    M510_VECF a;
    M510_VECF b;
    M510_VECF c;
    M510_VECF d;
} DBG_QUAD;

// .data:0x0
s32 lbl_1_data_0 = 1;

// .data:0x4 - the sequence parameters followed by one unused zero word
struct {
    MGSEQ_PARAM param;
    u32 pad;
} lbl_1_data_4 = {
    {
        0, 0,
        (MGSEQ_FUNC)fn_1_E8, (MGSEQ_FUNC)fn_1_1C0, (MGSEQ_FUNC)fn_1_21C,
        (MGSEQ_FUNC)fn_1_2BC, (MGSEQ_FUNC)fn_1_30C, (MGSEQ_FUNC)fn_1_404,
        (MGSEQ_FUNC)fn_1_494, (MGSEQ_FUNC)fn_1_568, (MGSEQ_FUNC)fn_1_164,
    },
    0,
};

// .data:0x30
HuVecF lbl_1_data_30 = { 380.0f, 80000.0f, 750.0f };
// .data:0x3C
HuVecF lbl_1_data_3C = { 0.0f, 1.0f, 0.0f };
// .data:0x48
HuVecF lbl_1_data_48 = { 380.0f, 0.0f, 950.0f };
// .data:0x54
s16 lbl_1_data_54[4] = { 2, 5, 15, 8 };

// MWCC lays out bss in reverse declaration order
HuVecF lbl_1_bss_7810;
DBG_LINE lbl_1_bss_6010[256];
DBG_TRI lbl_1_bss_3C10[256];
DBG_QUAD lbl_1_bss_C10[256];
GXColor lbl_1_bss_80E[256];
GXColor lbl_1_bss_40E[256];
GXColor lbl_1_bss_E[256];
u16 lbl_1_bss_C;
u16 lbl_1_bss_A;
u16 lbl_1_bss_8;
OMOBJMAN *lbl_1_bss_4;
M510_OBJ *lbl_1_bss_0;

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
    MgSeqCreate(&lbl_1_data_4.param);
    MgSeqModeHookAdd(4, (MGSEQ_FUNC)fn_1_260);
}

void fn_1_E8(void)
{
    M510_OBJ *obj;

    obj = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M510_OBJ), 0x10000000);
    lbl_1_bss_0 = obj;
    memset(lbl_1_bss_0, 0, sizeof(M510_OBJ));
    fn_1_7B4(obj, lbl_1_bss_4);
    obj->mode = 0;
    MgSeqModeNext();
}

void fn_1_164(void)
{
    M510_OBJ *obj = lbl_1_bss_0;

    fn_1_D84(obj);
    HuMemDirectFree(lbl_1_bss_0);
    lbl_1_bss_0 = NULL;
    MgSeqModeNext();
}

void fn_1_1C0(void)
{
    M510_OBJ *obj = lbl_1_bss_0;

    MgSeqModeChangeOff();
    if (fn_1_1274(obj) != 0) {
        MgSeqModeNext();
    }
    obj->mode = 1;
    fn_1_E54(obj, 0);
}

void fn_1_21C(void)
{
    M510_OBJ *obj = lbl_1_bss_0;

    obj->mode = 2;
    fn_1_E54(obj, 0);
}

void fn_1_260(void)
{
    M510_OBJ *obj = lbl_1_bss_0;

    fn_1_129C(obj);
    obj->mode = 3;
    fn_1_E54(obj, 0);
    lbl_1_data_0 = 1;
}

void fn_1_2BC(void)
{
    M510_OBJ *obj = lbl_1_bss_0;

    obj->mode = 4;
    if (fn_1_E54(obj, 1) != 0) {
        MgSeqModeNext();
    }
}

void fn_1_30C(void)
{
    M510_OBJ *obj = lbl_1_bss_0;
    M510_PLAYER_WORK *player;
    M510_PLAYER_MODEL_WORK *model;
    s16 mesId;
    s32 i;

    mesId = MgSeqGameMesIdGet();
    if (obj->soundId >= 0 && (GameMesStatGet(mesId) & 0x10) != 0) {
        fn_1_6E4(obj->soundId, 100);
        obj->soundId = -1;
    }
    if (lbl_1_data_0 != 0) {
        for (i = 0; i < 4; i++) {
            player = &obj->players[i];
            model = &player->model;
            model->padOn = 0;
        }
        lbl_1_data_0 = 0;
        obj->state = 0;
    }
    obj->mode = 5;
    fn_1_E54(obj, 0);
}

void fn_1_404(void)
{
    M510_OBJ *obj = lbl_1_bss_0;

    MgSeqModeChangeOff();
    MgSeqModeChangeOff();
    if (lbl_1_bss_0->practice != 0) {
        WipeCreate(2, 6, 60);
        MgSeqModeNext();
    } else {
        if (fn_1_1308(obj) != 0) {
            MgSeqModeNext();
        }
    }
    obj->mode = 6;
    fn_1_E54(obj, 0);
}

void fn_1_494(void)
{
    M510_OBJ *obj = lbl_1_bss_0;
    M510_PLAYER_WORK *player;
    s32 i;

    MgSeqModeChangeOff();
    if (lbl_1_bss_0->practice != 0) {
        if (WipeCheckEnd() == 0) {
            for (i = 0; i < 4; i++) {
                player = &obj->players[i];
                GwPlayer[player->model.playerNo].mgScore = player->score.value;
            }
            MgSeqModeSet(10);
            return;
        }
    } else {
        if (fn_1_1310(obj) != 0) {
            MgSeqModeNext();
        }
    }
    obj->mode = 7;
    fn_1_E54(obj, 0);
}

void fn_1_568(void)
{
    M510_OBJ *obj = lbl_1_bss_0;

    fn_1_1310(obj);
    obj->mode = 8;
    fn_1_E54(obj, 0);
}

s32 fn_1_5B4(s32 seId)
{
    return HuAudFXPlay(seId);
}

void fn_1_5DC(s32 seNo)
{
    HuAudFXStop(seNo);
}

void fn_1_604(s32 seNo, s32 speed)
{
    HuAudFXFadeOut(seNo, speed);
}

void fn_1_634(s32 seNo, s16 pan)
{
    HuAudFXPanning(seNo, pan);
}

void fn_1_664(Vec *pos, Vec *heading, float sndDist, float sndSpeed, float startDis)
{
    HuAudFXListnerSetEX(pos, heading, sndDist, sndSpeed, startDis, 0.0f, 0.0f);
}

void fn_1_6BC(s16 streamId)
{
    HuAudSStreamPlay(streamId);
}

void fn_1_6E4(s32 streamNo, s32 speed)
{
    HuAudSStreamFadeOut(streamNo, speed);
}

void fn_1_714(void)
{
    HuAudAllStop();
}

s32 fn_1_734(s32 arg0)
{
    return HuAudBGMPlay((s16)arg0);
}

s32 fn_1_75C(s16 streamId)
{
    return HuAudJinglePlay(streamId);
}

void fn_1_784(s16 streamId, s16 chanNo)
{
    HuAudSStreamChanPlay(streamId, chanNo);
}

void fn_1_7B4(M510_OBJ *obj, OMOBJMAN *objman)
{
    s16 i;
    s32 j;

    memset(obj, 0, sizeof(M510_OBJ));
    obj->objman = objman;
    obj->time = 1500;
    obj->mesId = -1;
    obj->soundId = -1;
    obj->unk253C = -1;
    obj->practice = _CheckFlag(0x30002);
    fn_1_959C(obj->openWork, obj);
    fn_1_89B8(&obj->cam, obj);
    obj->remain = 30;
    fn_1_295C(&obj->stage, obj);
    for (i = 0; i < 4; i++) {
        fn_1_3A64(&obj->players[i], i, obj);
    }
    obj->record = GWMgRecordGet(1);
    if (lbl_1_bss_0->practice == 0) {
        fn_1_CE04(&obj->score, 1, 0);
        obj->score.value = obj->record;
        fn_1_D354(&obj->score, 1);
    }
    Hu3DShadowCreate(2.0f, 400.0f, 100000.0f);
    Hu3DShadowTPLvlSet(0.5f);
    Hu3DShadowSizeSet(240);
    Hu3DShadowPosSet(&lbl_1_data_30, &lbl_1_data_3C, &lbl_1_data_48);
    fn_1_9B0C(obj);
    for (j = 0; j < 30; j++) {
        obj->pattern[j] = 2;
    }
    fn_1_9B8(obj, 0);
    fn_1_9B8(obj, 1);
    fn_1_9B8(obj, 3);
    obj->patIdx = 0;
    Hu3DBGColorSet(0, 0, 0);
    WipeCreate(1, 5, 60);
    Hu3DNoSyncSet(1);
}

void fn_1_9B8(M510_OBJ *obj, s16 mode)
{
    s16 count;
    s16 a;
    s16 b;
    s16 c;
    s32 r;

    count = lbl_1_data_54[mode];
    do {
        a = frand() % 10;
        b = (frand() % 10) + 10;
        c = (frand() % 10) + 20;
        r = frand() % 10000;
        switch (mode) {
        case 2:
            break;
        case 0:
            if ((count & 1) != 0) {
                if (fn_1_C6C(obj, a, 0) != 0) {
                    obj->pattern[a] = 0;
                    count--;
                }
            } else {
                if (fn_1_C6C(obj, c, 0) != 0) {
                    obj->pattern[c] = 0;
                    count--;
                }
            }
            break;
        case 1:
            if (r < 1000) {
                if (fn_1_C6C(obj, a, 1) != 0) {
                    obj->pattern[a] = 1;
                    count--;
                }
            } else if (r < 4000) {
                if (fn_1_C6C(obj, b, 1) != 0) {
                    obj->pattern[b] = 1;
                    count--;
                }
            } else {
                if (fn_1_C6C(obj, c, 1) != 0) {
                    obj->pattern[c] = 1;
                    count--;
                }
            }
            break;
        case 3:
            if (r < 1000) {
                if (fn_1_C6C(obj, a, 3) != 0) {
                    obj->pattern[a] = 3;
                    count--;
                }
            } else if (r < 4000) {
                if (fn_1_C6C(obj, b, 3) != 0) {
                    obj->pattern[b] = 3;
                    count--;
                }
            } else {
                if (fn_1_C6C(obj, c, 3) != 0) {
                    obj->pattern[c] = 3;
                    count--;
                }
            }
            break;
        }
    } while (count > 0);
}

s32 fn_1_C6C(M510_OBJ *obj, s16 idx, s16 val)
{
    if (idx <= 0) {
        if (val != 2) {
            return 0;
        }
        if (obj->pattern[idx] != 2) {
            return 0;
        }
        if (obj->pattern[idx + 1] == (u8)val) {
            return 0;
        }
    } else if (idx >= 29) {
        if (obj->pattern[idx] != 2) {
            return 0;
        }
        if (obj->pattern[idx - 1] == (u8)val) {
            return 0;
        }
    } else {
        if (obj->pattern[idx] != 2) {
            return 0;
        }
        if (obj->pattern[idx - 1] == (u8)val) {
            return 0;
        }
        if (obj->pattern[idx + 1] == (u8)val) {
            return 0;
        }
    }
    return 1;
}

void fn_1_D84(M510_OBJ *obj)
{
    s32 i;

    fn_1_A454(obj);
    if (lbl_1_bss_0->practice == 0) {
        fn_1_D1AC(&obj->score);
    }
    for (i = 0; i < 4; i++) {
        fn_1_3CCC(&obj->players[i]);
    }
    fn_1_2F8C(&obj->stage);
    fn_1_8BEC(&obj->cam);
    fn_1_9614(obj->openWork);
    if (obj->mesId >= 0) {
        GameMesKill(obj->mesId);
    }
    GameMesClose();
    memset(obj, 0, sizeof(M510_OBJ));
    HuAudAllStop();
}

s32 fn_1_E54(M510_OBJ *obj, s32 unused)
{
    s32 ret = 0;
    s32 allIdle;
    u16 i;
    M510_PITCH_WORK *pitch;
    s32 idx;

    fn_1_9668(obj->openWork);
    fn_1_8C1C(&obj->cam);
    fn_1_3020(&obj->stage);
    for (i = 0; i < 4; i++) {
        fn_1_3D34(&obj->players[i]);
    }
    if (obj->mode == 4) {
        allIdle = 1;
        for (i = 0; i < 4; i++) {
            pitch = &obj->players[i].pitch;
            if (pitch->state != 0) {
                allIdle = 0;
                break;
            }
        }
        if (obj->remain <= 0) {
            for (i = 0; i < 4; i++) {
                if (obj->players[i].ballWork.liveCount != 0) {
                    break;
                }
            }
            if (i == 4) {
                ret = 1;
            }
        } else {
            if (allIdle != 0) {
                idx = obj->patIdx++;
#ifdef TARGET_PC
                if (idx > 29) {
                    idx = 29;
                }
#endif
                for (i = 0; i < 4; i++) {
                    pitch = &obj->players[i].pitch;
                    pitch->index = obj->pattern[idx];
                    pitch->active = 1;
                }
            }
            for (i = 0; i < 4; i++) {
                pitch = &obj->players[i].pitch;
                if (pitch->state == 3) {
                    if (obj->remain != 0) {
                        obj->remain--;
                    }
                    break;
                }
            }
        }
    }
    if (lbl_1_bss_0->practice == 0) {
        fn_1_D234(&obj->score);
    }
    if (MgSeqModeGet() == 5 && obj->time != 0) {
        obj->time--;
    }
    obj->frame++;
    return ret;
}

void fn_1_1074(M510_OBJ *obj)
{
    GXColor red = { 0xFF, 0x00, 0x00, 0xFF };
    GXColor blue = { 0x00, 0x00, 0xFF, 0xFF };
    GXColor green = { 0x00, 0xFF, 0x00, 0xFF };
    M510_VECF a;
    M510_VECF b;
    M510_VECF pt;
    M510_BALL *ball;
    M510_PATH *path;
    s32 step;
    s32 i;
    s32 j;

    for (i = 0; i < 4; i++) {
        ball = &obj->players[i].ballWork.balls[0];
        if (ball->active == 0 || ball->path.count == 0) {
            ball = &obj->players[i].ballWork.balls[1];
        }
        if (ball->active != 0) {
            path = &ball->path;
            if (path->count != 0) {
                step = path->sum[path->count - 1] / 50;
                for (j = 0; j < 50; j++) {
                    fn_1_C70C(&a, path, step * j, 0.5f);
                    fn_1_C70C(&b, path, step * (j + 1), 0.5f);
                    fn_1_2148(&a, &b, &red);
                }
                for (j = 0; j < path->count; j++) {
                    pt = path->pts[j];
                    fn_1_2220(&pt, 10.0f, &blue);
                }
            }
            fn_1_2220(&ball->pos, 20.0f, &green);
        }
    }
}

s32 fn_1_1274(M510_OBJ *obj)
{
    return fn_1_966C(obj);
}

void fn_1_129C(M510_OBJ *obj)
{
    s32 i;
    M510_PLAYER_WORK *player;

    for (i = 0; i < 4; i++) {
        player = &obj->players[i];
        fn_1_D354(&player->score, 1);
        player->model.padOn = 1;
    }
}

s32 fn_1_1308(M510_OBJ *obj)
{
    return 1;
}

s32 fn_1_1310(M510_OBJ *obj)
{
    return fn_1_A4F4(obj);
}

void fn_1_1338(void *unused, Mtx mtx)
{
    s32 i;

    GXLoadPosMtxImm(mtx, 0);
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
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT0, GX_DF_CLAMP, GX_AF_SPOT);
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
    }
    for (i = 0; i < lbl_1_bss_A; i++) {
        GXBegin(GX_LINESTRIP, GX_VTXFMT0, 4);
        GXPosition3f32(lbl_1_bss_3C10[i].a.x, lbl_1_bss_3C10[i].a.y, lbl_1_bss_3C10[i].a.z);
        GXColor4u8(lbl_1_bss_40E[i].r, lbl_1_bss_40E[i].g, lbl_1_bss_40E[i].b, lbl_1_bss_40E[i].a);
        GXPosition3f32(lbl_1_bss_3C10[i].b.x, lbl_1_bss_3C10[i].b.y, lbl_1_bss_3C10[i].b.z);
        GXColor4u8(lbl_1_bss_40E[i].r, lbl_1_bss_40E[i].g, lbl_1_bss_40E[i].b, lbl_1_bss_40E[i].a);
        GXPosition3f32(lbl_1_bss_3C10[i].c.x, lbl_1_bss_3C10[i].c.y, lbl_1_bss_3C10[i].c.z);
        GXColor4u8(lbl_1_bss_40E[i].r, lbl_1_bss_40E[i].g, lbl_1_bss_40E[i].b, lbl_1_bss_40E[i].a);
        GXPosition3f32(lbl_1_bss_3C10[i].a.x, lbl_1_bss_3C10[i].a.y, lbl_1_bss_3C10[i].a.z);
        GXColor4u8(lbl_1_bss_40E[i].r, lbl_1_bss_40E[i].g, lbl_1_bss_40E[i].b, lbl_1_bss_40E[i].a);
    }
    for (i = 0; i < lbl_1_bss_C; i++) {
        GXBegin(GX_LINESTRIP, GX_VTXFMT0, 5);
        GXPosition3f32(lbl_1_bss_C10[i].a.x, lbl_1_bss_C10[i].a.y, lbl_1_bss_C10[i].a.z);
        GXColor4u8(lbl_1_bss_E[i].r, lbl_1_bss_E[i].g, lbl_1_bss_E[i].b, lbl_1_bss_E[i].a);
        GXPosition3f32(lbl_1_bss_C10[i].b.x, lbl_1_bss_C10[i].b.y, lbl_1_bss_C10[i].b.z);
        GXColor4u8(lbl_1_bss_E[i].r, lbl_1_bss_E[i].g, lbl_1_bss_E[i].b, lbl_1_bss_E[i].a);
        GXPosition3f32(lbl_1_bss_C10[i].c.x, lbl_1_bss_C10[i].c.y, lbl_1_bss_C10[i].c.z);
        GXColor4u8(lbl_1_bss_E[i].r, lbl_1_bss_E[i].g, lbl_1_bss_E[i].b, lbl_1_bss_E[i].a);
        GXPosition3f32(lbl_1_bss_C10[i].d.x, lbl_1_bss_C10[i].d.y, lbl_1_bss_C10[i].d.z);
        GXColor4u8(lbl_1_bss_E[i].r, lbl_1_bss_E[i].g, lbl_1_bss_E[i].b, lbl_1_bss_E[i].a);
        GXPosition3f32(lbl_1_bss_C10[i].a.x, lbl_1_bss_C10[i].a.y, lbl_1_bss_C10[i].a.z);
        GXColor4u8(lbl_1_bss_E[i].r, lbl_1_bss_E[i].g, lbl_1_bss_E[i].b, lbl_1_bss_E[i].a);
    }
    lbl_1_bss_C = 0;
    lbl_1_bss_A = 0;
    lbl_1_bss_8 = 0;
}

void fn_1_2148(M510_VECF *a, M510_VECF *b, GXColor *c)
{
    if (lbl_1_bss_8 < 256) {
        lbl_1_bss_6010[lbl_1_bss_8].a = *a;
        lbl_1_bss_6010[lbl_1_bss_8].b = *b;
        lbl_1_bss_80E[lbl_1_bss_8].r = c->r;
        lbl_1_bss_80E[lbl_1_bss_8].g = c->g;
        lbl_1_bss_80E[lbl_1_bss_8].b = c->b;
        lbl_1_bss_80E[lbl_1_bss_8].a = c->a;
        lbl_1_bss_8++;
    }
}

void fn_1_2220(M510_VECF *pos, f32 size, GXColor *c)
{
    M510_VECF p0;
    M510_VECF p1;
    M510_VECF p2;

    if (lbl_1_bss_A < 256) {
        p0.x = pos->x;
        p0.y = pos->y;
        p0.z = pos->z - size;
        p1.x = pos->x + size;
        p1.y = pos->y;
        p1.z = pos->z + size;
        p2.x = pos->x - size;
        p2.y = pos->y;
        p2.z = pos->z + size;
        lbl_1_bss_3C10[lbl_1_bss_A].a = p0;
        lbl_1_bss_3C10[lbl_1_bss_A].b = p1;
        lbl_1_bss_3C10[lbl_1_bss_A].c = p2;
        lbl_1_bss_40E[lbl_1_bss_A].r = c->r;
        lbl_1_bss_40E[lbl_1_bss_A].g = c->g;
        lbl_1_bss_40E[lbl_1_bss_A].b = c->b;
        lbl_1_bss_40E[lbl_1_bss_A].a = c->a;
        lbl_1_bss_A++;
    }
}

void fn_1_2390(M510_VECF *a, M510_VECF *b, M510_VECF *c, M510_VECF *d, GXColor *col)
{
    if (lbl_1_bss_C < 256) {
        lbl_1_bss_C10[lbl_1_bss_C].a = *a;
        lbl_1_bss_C10[lbl_1_bss_C].b = *b;
        lbl_1_bss_C10[lbl_1_bss_C].c = *c;
        lbl_1_bss_C10[lbl_1_bss_C].d = *d;
        lbl_1_bss_E[lbl_1_bss_C].r = col->r;
        lbl_1_bss_E[lbl_1_bss_C].g = col->g;
        lbl_1_bss_E[lbl_1_bss_C].b = col->b;
        lbl_1_bss_E[lbl_1_bss_C].a = col->a;
        lbl_1_bss_C++;
    }
}

void fn_1_24D0(float quat[4], Vec *from, Vec *to)
{
    Vec cross;
    f32 dot;
    f32 absDot;
    f32 sq;
    f32 out[4];

    PSVECCrossProduct(from, to, &cross);
    dot = PSVECDotProduct(from, to);
    absDot = (dot >= 0.0f) ? dot : -dot;
    if (absDot < 0.9999999f) {
        sq = sqrtf(2.0f * (1.0f + dot));
        out[0] = cross.x / sq;
        out[1] = cross.y / sq;
        out[2] = cross.z / sq;
        out[3] = sq * 0.5f;
    } else if (dot < 0.0f) {
        out[0] = 0.0f;
        out[1] = 1.0f;
        out[2] = 0.0f;
        out[3] = 0.0f;
    } else {
        out[0] = 0.0f;
        out[1] = 0.0f;
        out[2] = 0.0f;
        out[3] = 1.0f;
    }
    quat[0] = out[0];
    quat[1] = out[1];
    quat[2] = out[2];
    quat[3] = out[3];
}

f32 fn_1_2840(f32 a, f32 b, f32 t)
{
    f32 d;
    f32 r;

    d = (f32)fmod((f64)(b - a), 360.0);
    if (d < 0.0f) {
        d += 360.0f;
    }
    if (d > 180.0f) {
        d -= 360.0f;
    }
    r = (f32)fmod((f64)(a + (t * d)), 360.0);
    if (r < 0.0f) {
        r += 360.0f;
    }
    return r;
}
