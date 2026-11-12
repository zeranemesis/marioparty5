#include "REL/m565dll.h"

typedef struct M565Line_s {
    Vec p[2];
} M565Line;

typedef struct M565Tri_s {
    Vec p[3];
} M565Tri;

typedef struct M565Quad_s {
    Vec p[4];
} M565Quad;

static void fn_1_1110(HU3DMODEL *modelP, Mtx *mtx);

u16 lbl_1_bss_8;
u16 lbl_1_bss_A;
u16 lbl_1_bss_C;
GXColor lbl_1_bss_E[256];
GXColor lbl_1_bss_40E[256];
GXColor lbl_1_bss_80E[256];
M565Quad lbl_1_bss_C10[256];
M565Tri lbl_1_bss_3C10[256];
M565Line lbl_1_bss_6010[256];

void fn_1_E48(M565Game *game, OMOBJMAN *man)
{
    memset(game, 0, sizeof(M565Game));
    game->man = man;
    game->timer = 3600;
    game->mesId = -1;
    game->bgm = -1;
    game->unk387C = -1;
    fn_1_25B4(&game->sys, game);
    Hu3DShadowCreate(20.0f, 20.0f, 30000.0f);
    Hu3DShadowTPLvlSet(0.5f);
    Hu3DShadowSizeSet(240);
    Hu3DShadowPosSet(&lbl_1_data_30, &lbl_1_data_3C, &lbl_1_data_48);
    Hu3DBGColorSet(0, 0, 0);
    WipeCreate(1, 5, 60);
    Hu3DNoSyncSet(1);
    Hu3DHookFuncCreate(fn_1_1110);
}

void fn_1_F48(M565Game *game)
{
    fn_1_2A94(&game->sys);
    if (game->mesId >= 0) {
        GameMesKill(game->mesId);
    }
    GameMesClose();
    memset(game, 0, sizeof(M565Game));
    fn_1_DA8();
}

s32 fn_1_FA8(M565Game *game, s32 arg1)
{
    s32 ret = 0;

    if (fn_1_2B20(&game->sys) != 0) {
        ret = 1;
    }
    if (MgSeqModeGet() == 5 || MgSeqModeGet() == 6 || MgSeqModeGet() == 7 || MgSeqModeGet() == 8) {
        MgActorExec();
    }
    if (game->timer != 0) {
        game->timer = game->timer - 1;
    }
    game->frame = game->frame + 1;
    return ret;
}

s32 fn_1_104C(M565Game *game)
{
    return fn_1_E740(game);
}

s32 fn_1_1074(M565Game *game)
{
    s32 i;
    M565Player *player;

    for (i = 0; i < 4; i++) {
        player = &game->sys.players[i];
        fn_1_8984(player, 1);
        player->enabled = 1;
    }
    return 1;
}

s32 fn_1_10E0(M565Game *game)
{
    return fn_1_EE4C(game);
}

s32 fn_1_1108(void)
{
    return 0;
}

static void fn_1_1110(HU3DMODEL *modelP, Mtx *mtx)
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
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT0, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
    GXSetAlphaUpdate(GX_TRUE);
    for (i = 0; i < lbl_1_bss_8; i++) {
        GXBegin(GX_LINESTRIP, GX_VTXFMT0, 2);
        GXPosition3f32(lbl_1_bss_6010[i].p[0].x, lbl_1_bss_6010[i].p[0].y, lbl_1_bss_6010[i].p[0].z);
        GXColor4u8(lbl_1_bss_80E[i].r, lbl_1_bss_80E[i].g, lbl_1_bss_80E[i].b, lbl_1_bss_80E[i].a);
        GXPosition3f32(lbl_1_bss_6010[i].p[1].x, lbl_1_bss_6010[i].p[1].y, lbl_1_bss_6010[i].p[1].z);
        GXColor4u8(lbl_1_bss_80E[i].r, lbl_1_bss_80E[i].g, lbl_1_bss_80E[i].b, lbl_1_bss_80E[i].a);
        GXEnd();
    }
    for (i = 0; i < lbl_1_bss_A; i++) {
        GXBegin(GX_LINESTRIP, GX_VTXFMT0, 4);
        GXPosition3f32(lbl_1_bss_3C10[i].p[0].x, lbl_1_bss_3C10[i].p[0].y, lbl_1_bss_3C10[i].p[0].z);
        GXColor4u8(lbl_1_bss_40E[i].r, lbl_1_bss_40E[i].g, lbl_1_bss_40E[i].b, lbl_1_bss_40E[i].a);
        GXPosition3f32(lbl_1_bss_3C10[i].p[1].x, lbl_1_bss_3C10[i].p[1].y, lbl_1_bss_3C10[i].p[1].z);
        GXColor4u8(lbl_1_bss_40E[i].r, lbl_1_bss_40E[i].g, lbl_1_bss_40E[i].b, lbl_1_bss_40E[i].a);
        GXPosition3f32(lbl_1_bss_3C10[i].p[2].x, lbl_1_bss_3C10[i].p[2].y, lbl_1_bss_3C10[i].p[2].z);
        GXColor4u8(lbl_1_bss_40E[i].r, lbl_1_bss_40E[i].g, lbl_1_bss_40E[i].b, lbl_1_bss_40E[i].a);
        GXPosition3f32(lbl_1_bss_3C10[i].p[0].x, lbl_1_bss_3C10[i].p[0].y, lbl_1_bss_3C10[i].p[0].z);
        GXColor4u8(lbl_1_bss_40E[i].r, lbl_1_bss_40E[i].g, lbl_1_bss_40E[i].b, lbl_1_bss_40E[i].a);
        GXEnd();
    }
    for (i = 0; i < lbl_1_bss_C; i++) {
        GXBegin(GX_LINESTRIP, GX_VTXFMT0, 5);
        GXPosition3f32(lbl_1_bss_C10[i].p[0].x, lbl_1_bss_C10[i].p[0].y, lbl_1_bss_C10[i].p[0].z);
        GXColor4u8(lbl_1_bss_E[i].r, lbl_1_bss_E[i].g, lbl_1_bss_E[i].b, lbl_1_bss_E[i].a);
        GXPosition3f32(lbl_1_bss_C10[i].p[1].x, lbl_1_bss_C10[i].p[1].y, lbl_1_bss_C10[i].p[1].z);
        GXColor4u8(lbl_1_bss_E[i].r, lbl_1_bss_E[i].g, lbl_1_bss_E[i].b, lbl_1_bss_E[i].a);
        GXPosition3f32(lbl_1_bss_C10[i].p[2].x, lbl_1_bss_C10[i].p[2].y, lbl_1_bss_C10[i].p[2].z);
        GXColor4u8(lbl_1_bss_E[i].r, lbl_1_bss_E[i].g, lbl_1_bss_E[i].b, lbl_1_bss_E[i].a);
        GXPosition3f32(lbl_1_bss_C10[i].p[3].x, lbl_1_bss_C10[i].p[3].y, lbl_1_bss_C10[i].p[3].z);
        GXColor4u8(lbl_1_bss_E[i].r, lbl_1_bss_E[i].g, lbl_1_bss_E[i].b, lbl_1_bss_E[i].a);
        GXPosition3f32(lbl_1_bss_C10[i].p[0].x, lbl_1_bss_C10[i].p[0].y, lbl_1_bss_C10[i].p[0].z);
        GXColor4u8(lbl_1_bss_E[i].r, lbl_1_bss_E[i].g, lbl_1_bss_E[i].b, lbl_1_bss_E[i].a);
        GXEnd();
    }
    lbl_1_bss_C = 0;
    lbl_1_bss_A = 0;
    lbl_1_bss_8 = 0;
}

void fn_1_1F20(Vec *a, Vec *b, GXColor *color)
{
    GXColor *dst;

    if (lbl_1_bss_8 < 256) {
        lbl_1_bss_6010[lbl_1_bss_8].p[0] = *a;
        lbl_1_bss_6010[lbl_1_bss_8].p[1] = *b;
        dst = &lbl_1_bss_80E[lbl_1_bss_8];
        dst->r = color->r;
        dst->g = color->g;
        dst->b = color->b;
        dst->a = color->a;
        lbl_1_bss_8++;
    }
}

void fn_1_1FF8(Vec *a, Vec *b, Vec *c, GXColor *color)
{
    GXColor *dst;

    if (lbl_1_bss_A < 256) {
        lbl_1_bss_3C10[lbl_1_bss_A].p[0] = *a;
        lbl_1_bss_3C10[lbl_1_bss_A].p[1] = *b;
        lbl_1_bss_3C10[lbl_1_bss_A].p[2] = *c;
        dst = &lbl_1_bss_40E[lbl_1_bss_A];
        dst->r = color->r;
        dst->g = color->g;
        dst->b = color->b;
        dst->a = color->a;
        lbl_1_bss_A++;
    }
}

void fn_1_2104(Vec *a, Vec *b, Vec *c, Vec *d, GXColor *color)
{
    GXColor *dst;

    if (lbl_1_bss_C < 256) {
        lbl_1_bss_C10[lbl_1_bss_C].p[0] = *a;
        lbl_1_bss_C10[lbl_1_bss_C].p[1] = *b;
        lbl_1_bss_C10[lbl_1_bss_C].p[2] = *c;
        lbl_1_bss_C10[lbl_1_bss_C].p[3] = *d;
        dst = &lbl_1_bss_E[lbl_1_bss_C];
        dst->r = color->r;
        dst->g = color->g;
        dst->b = color->b;
        dst->a = color->a;
        lbl_1_bss_C++;
    }
}

/* quaternion rotating direction a onto direction b, written as x,y,z,w */
void fn_1_2244(f32 *quat, Vec *a, Vec *b)
{
    Vec axis;
    f32 dot;
    f32 absDot;
    f32 s;
    f32 qx;
    f32 qy;
    f32 qz;
    f32 qw;

    PSVECCrossProduct(a, b, &axis);
    dot = PSVECDotProduct(a, b);
    if (dot >= 0.0f) {
        absDot = dot;
    } else {
        absDot = -dot;
    }
    if (absDot < 0.9999999f) {
        s = sqrtf(2.0f * (1.0f + dot));
        qx = axis.x / s;
        qy = axis.y / s;
        qz = axis.z / s;
        qw = s * 0.5f;
    } else if (dot < 0.0f) {
        qx = 0.0f;
        qy = 1.0f;
        qz = 0.0f;
        qw = 0.0f;
    } else {
        qx = 0.0f;
        qy = 0.0f;
        qz = 0.0f;
        qw = 1.0f;
    }
    quat[0] = qx;
    quat[1] = qy;
    quat[2] = qz;
    quat[3] = qw;
}
