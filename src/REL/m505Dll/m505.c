#include "REL/m505Dll.h"

#include "math.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

int rand8(void);

void ObjectSetup(void);
void fn_1_0(HU3DMODEL *model, Mtx *mtx);
void fn_1_A90(HU3DMODEL *model, Mtx *mtx);
void fn_1_1284(void);
void fn_1_1828(OMOBJ *obj);
void fn_1_4FB4(OMOBJ *obj);
void fn_1_B694(HU3DMODEL *model, Mtx *mtx);
void fn_1_BD58(HU3DMODEL *model, Mtx *mtx);
void fn_1_C0BC(HU3DMODEL *model, Mtx *mtx);
void fn_1_D9E0(HU3DMODEL *model, Mtx *mtx);
void fn_1_EF6C(OMOBJ *obj);
void fn_1_FADC(OMOBJ *obj);
void fn_1_11650(OMOBJ *obj);
void fn_1_119A0(OMOBJ *obj);
void fn_1_12078(OMOBJ *obj);
void fn_1_120D0(OMOBJ *obj);

typedef struct M505Player_s {
    /* 0x00 */ s32 no;
    /* 0x04 */ s32 playerNo;
    /* 0x08 */ s32 charNoClamp;
    /* 0x0C */ s32 charNo;
    /* 0x10 */ s32 padNo;
    /* 0x14 */ s32 dif;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 mode;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ float unk2C;
    /* 0x30 */ float unk30;
    /* 0x34 */ float unk34;
    /* 0x38 */ float unk38;
    /* 0x3C */ float unk3C;
    /* 0x40 */ float unk40;
    /* 0x44 */ float unk44;
    /* 0x48 */ float unk48;
    /* 0x4C */ float unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 unk64;
} M505Player; /* size = 0x68 */

/* Wake particle left behind a moving obstacle. */
typedef struct M505Wake_s {
    /* 0x00 */ float unk0;
    /* 0x04 */ float unk4;
    /* 0x08 */ float unk8;
    /* 0x0C */ float unkC;
    /* 0x10 */ float unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
} M505Wake; /* size = 0x18 */

/* Obstacle drifting down the river (64 of them, type 0 small / 1 big). */
typedef struct M505Obj_s {
    /* 0x000 */ Vec pos;
    /* 0x00C */ Vec vel;
    /* 0x018 */ Vec prevPos;
    /* 0x024 */ float rotY;
    /* 0x028 */ s32 state;
    /* 0x02C */ u8 type;
    /* 0x02D */ u8 rnd;
    /* 0x02E */ u8 animNo;
    /* 0x02F */ u8 pad2F;
    /* 0x030 */ s16 time;
    /* 0x032 */ s16 speed;
    /* 0x034 */ s16 time0;
    /* 0x036 */ s8 lane;
    /* 0x037 */ u8 unk37;
    /* 0x038 */ u8 unk38;
    /* 0x039 */ u8 pad39[3];
    /* 0x03C */ M505Wake wake[96];
    /* 0x93C */ u8 unk93C;
    /* 0x93D */ u8 pad93D[3];
} M505Obj; /* size = 0x940 */

/* Background actor (4 used, 96 slots), drawn by fn_1_D9E0 from copied frames. */
typedef struct M505BgActor_s {
    /* 0x00 */ Vec pos;
    /* 0x0C */ float unkC;
    /* 0x10 */ s8 dir;
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ float velY;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 frame;
    /* 0x1A */ u8 type;
    /* 0x1B */ u8 pad1B;
    /* 0x1C */ s32 state;
    /* 0x20 */ s8 no;
    /* 0x21 */ u8 pad21[3];
} M505BgActor; /* size = 0x24 */

/* Ripple quad on the water surface. */
typedef struct M505Ripple_s {
    /* 0x00 */ float unk0;
    /* 0x04 */ float unk4;
    /* 0x08 */ float unk8;
    /* 0x0C */ float unkC;
    /* 0x10 */ float unk10;
    /* 0x14 */ float unk14;
    /* 0x18 */ float unk18;
    /* 0x1C */ float unk1C;
    /* 0x20 */ float unk20;
    /* 0x24 */ float unk24;
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 unk29;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ s32 time;
} M505Ripple; /* size = 0x30 */

/* Splash effect. */
typedef struct M505Splash_s {
    /* 0x00 */ float unk0;
    /* 0x04 */ float unk4;
    /* 0x08 */ float unk8;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ s32 time;
} M505Splash; /* size = 0x14 */

/* Sound requests queued by the game logic and played once per frame by fn_1_EF6C. */
typedef struct M505SeReq_s {
    /* 0x00 */ s32 num;
    /* 0x04 */ float posX[3];
    /* 0x10 */ s32 panF[3];
    /* 0x1C */ s32 seNo;
} M505SeReq; /* size = 0x20 */

s32 lbl_1_data_0[2] = { 0, 0 };
u32 lbl_1_data_8[13] = { DATA_mariomdl1, DATA_luigimdl1, DATA_peachmdl1, DATA_yoshimdl1, DATA_wariomdl1, DATA_daisymdl1, DATA_waluigimdl1,
    DATA_kinopiomdl1, DATA_teresamdl1, DATA_minikoopamdl1, 0, 0, 0 };
u32 lbl_1_data_3C[12] = { CHARMOT_HSF_c000m1_300, CHARMOT_HSF_c000m1_301, CHARMOT_HSF_c000m1_302, CHARMOT_HSF_c000m1_303, CHARMOT_HSF_c000m1_332,
    CHARMOT_HSF_c000m1_306, 0, 0, 0, 0, 0, 0 };
u8 lbl_1_data_6C[6] = { 0, 0, 0, 1, 0, 1 };
u8 lbl_1_data_72[6] = { 8, 8, 8, 16, 6, 8 };
s32 lbl_1_data_78 = -1;
s32 lbl_1_data_7C = 1800;
s32 lbl_1_data_80 = -1;
s32 lbl_1_data_84 = -1;
s32 lbl_1_data_88 = -1;
s32 lbl_1_data_8C = 1;
float lbl_1_data_90[4][4] = {
    { -240.0f, 0.0f, -250.0f, 0.0f },
    { -80.0f, 0.0f, -250.0f, 0.0f },
    { 80.0f, 0.0f, -250.0f, 0.0f },
    { 240.0f, 0.0f, -250.0f, 0.0f },
};
s32 lbl_1_data_D0[8] = { 0 };
u8 lbl_1_data_F0[6][4] = { { 0 }, { 0 }, { 0 }, { 0 }, { 0, 0, 0, 0x60 }, { 0x3F, 0x80, 0, 0 } };
char lbl_1_data_108[] = "ske_head";
s8 lbl_1_data_111[4] = { 4, -1, 4, 4 };
s8 lbl_1_data_115[4] = { 0 };
s8 lbl_1_data_119 = 1;
s32 lbl_1_data_11C[2] = { -1, -1 };
s32 lbl_1_data_124 = 60;
s32 lbl_1_data_128 = 120;
s32 lbl_1_data_12C[2] = { 0, 0 };

typedef struct M505Esprite_s {
    s16 memberNo;
    s16 animNo;
} M505ESPRITE;

typedef struct M505EspAnim_s {
    unsigned int dataNum;
    u16 useCnt;
    ANIMDATA *anim;
} M505ESPANIM;

/* esprite.c keeps these private; the minigame reads the sprite animation directly. */
extern M505ESPRITE esprite[];
extern M505ESPANIM espanim[];
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);

M505SeReq lbl_1_bss_2ED50[11];
OMOBJ *lbl_1_bss_2ED4C;
OMOBJ *lbl_1_bss_2ED48;
OMOBJ *lbl_1_bss_2ED38[4];
OMOBJ *lbl_1_bss_2ED34;
OMOBJ *lbl_1_bss_2ED30;
s32 lbl_1_bss_2ED2C;
OMOBJMAN *lbl_1_bss_2ED28;
s16 lbl_1_bss_2ED26;
s16 lbl_1_bss_2ED24;
s32 lbl_1_bss_2ED20;
s32 lbl_1_bss_2ED1C;
M505Obj lbl_1_bss_9D1C[64];
M505Splash lbl_1_bss_977C[72];
M505Ripple lbl_1_bss_1F7C[640];
M505BgActor lbl_1_bss_11FC[96];
u32 lbl_1_bss_11EC[4];
void *lbl_1_bss_11AC[4][4];
void *lbl_1_bss_119C[4];
u32 lbl_1_bss_1198;
void *lbl_1_bss_1158[4][4];
float lbl_1_bss_1154;
float lbl_1_bss_1150;
GXTexObj lbl_1_bss_918[64];
GXColor lbl_1_bss_111C;
GXColor lbl_1_bss_1118;
s32 lbl_1_bss_8;
float lbl_1_bss_0;

void fn_1_0(HU3DMODEL *model, Mtx *mtx)
{
    Mtx modelview;
#ifdef TARGET_PC
    Mtx44 proj;
#else
    Mtx proj;
#endif
    void *buf = lbl_1_bss_11AC;
    GXColor colorBlack = { 0, 0, 0, 255 };
    GXColor colorWhite = { 255, 255, 255, 255 };
    float z;

    z = 0.0f;
    lbl_1_bss_111C.r = (model->rot.x > 255.0f) ? 255.0f : ((model->rot.x < 0.0f) ? 0.0f : model->rot.x);
    lbl_1_bss_111C.g = (model->rot.y > 255.0f) ? 255.0f : ((model->rot.y < 0.0f) ? 0.0f : model->rot.y);
    lbl_1_bss_111C.b = (model->rot.z > 255.0f) ? 255.0f : ((model->rot.z < 0.0f) ? 0.0f : model->rot.z);
    lbl_1_bss_111C.a = 255;
    C_MTXOrtho(proj, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);
    PSMTXIdentity(modelview);
    GXLoadPosMtxImm(modelview, GX_PNMTX0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
    GXSetTevColor(GX_TEVREG0, lbl_1_bss_111C);
    if (model->pos.z == 0.0f) {
        GXSetColorUpdate(GX_TRUE);
        GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    }
    else if (model->pos.z == -1.0f) {
        GXSetColorUpdate(GX_TRUE);
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
        GXSetTevColor(GX_TEVREG0, colorBlack);
        z = 0.999969f;
    }
    else {
        GXSetColorUpdate(GX_FALSE);
        GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
    }
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(0.0f, 0.0f, z);
    GXColor4u8(255, 255, 255, 255);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(1.0f, 0.0f, z);
    GXColor4u8(255, 255, 255, 255);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(1.0f, 1.0f, z);
    GXColor4u8(255, 255, 255, 255);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(0.0f, 1.0f, z);
    GXColor4u8(255, 255, 255, 255);
    GXTexCoord2f32(0.0f, 1.0f);
    GXEnd();
    if (model->pos.z == -1.0f) {
        GXSetZMode(GX_TRUE, GX_GREATER, GX_TRUE);
        GXSetTevColor(GX_TEVREG0, colorWhite);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(0.0f, 0.0f, z);
        GXColor4u8(255, 255, 255, 255);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(1.0f, 0.0f, z);
        GXColor4u8(255, 255, 255, 255);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(1.0f, 1.0f, z);
        GXColor4u8(255, 255, 255, 255);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(0.0f, 1.0f, z);
        GXColor4u8(255, 255, 255, 255);
        GXTexCoord2f32(0.0f, 1.0f);
        GXEnd();
    }
    GXSetColorUpdate(GX_TRUE);
}

void fn_1_A90(HU3DMODEL *model, Mtx *mtx)
{
    Mtx modelview;
#ifdef TARGET_PC
    Mtx44 proj;
#else
    Mtx proj;
#endif
    void *buf = lbl_1_bss_11AC;
    s32 i;

    lbl_1_bss_1118.r = (model->rot.x > 255.0f) ? 255.0f : ((model->rot.x < 0.0f) ? 0.0f : model->rot.x);
    lbl_1_bss_1118.g = (model->rot.y > 255.0f) ? 255.0f : ((model->rot.y < 0.0f) ? 0.0f : model->rot.y);
    lbl_1_bss_1118.b = (model->rot.z > 255.0f) ? 255.0f : ((model->rot.z < 0.0f) ? 0.0f : model->rot.z);
    lbl_1_bss_1118.a = 255;
    C_MTXOrtho(proj, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);
    PSMTXIdentity(modelview);
    GXLoadPosMtxImm(modelview, GX_PNMTX0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTCLR, GX_BL_ONE, GX_LO_NOOP);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ONE);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTCLR, GX_BL_ONE, GX_LO_NOOP);
    GXSetTevColor(GX_TEVREG0, lbl_1_bss_1118);
    GXSetColorUpdate(GX_TRUE);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    for (i = 0; i < model->rot.x; i++) {
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(0.0f, 0.0f, 0.0f);
        GXColor4u8(255, 255, 255, 255);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(1.0f, 0.0f, 0.0f);
        GXColor4u8(255, 255, 255, 255);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(1.0f, 1.0f, 0.0f);
        GXColor4u8(255, 255, 255, 255);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(0.0f, 1.0f, 0.0f);
        GXColor4u8(255, 255, 255, 255);
        GXTexCoord2f32(0.0f, 1.0f);
        GXEnd();
    }
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

void fn_1_1284(void)
{
    Vec pos;
    Vec target;
    Vec up;
    float rotX;
    float rotY;
    float rotZ;
    s32 cameraNo;

    cameraNo = 1;
    CenterM[cameraNo].x += 5.0 * (((HuPadBtn[0] & PAD_BUTTON_RIGHT) != 0) - ((HuPadBtn[0] & PAD_BUTTON_LEFT) != 0));
    CenterM[cameraNo].y += 20.0 * ((HuPadTrigL[0] - HuPadTrigR[0]) / 128.0);
    CZoomM[cameraNo] += 25.0 * (((HuPadBtn[0] & PAD_BUTTON_X) != 0) - ((HuPadBtn[0] & PAD_BUTTON_Y) != 0));
    CRotM[cameraNo].y += HuPadSubStkX[0] / 32.0;
    CRotM[cameraNo].x += HuPadSubStkY[0] / -32.0;
    fontcolor = 0;
    rotX = CRotM[cameraNo].x;
    rotY = CRotM[cameraNo].y;
    rotZ = CRotM[cameraNo].z;
    pos.x = CenterM[cameraNo].x + (CZoomM[cameraNo] * (sin((M_PI * rotY) / 180.0) * cos((M_PI * rotX) / 180.0)));
    pos.y = CenterM[cameraNo].y + (CZoomM[cameraNo] * -sin((M_PI * rotX) / 180.0));
    pos.z = CenterM[cameraNo].z + (CZoomM[cameraNo] * (cos((M_PI * rotY) / 180.0) * cos((M_PI * rotX) / 180.0)));
    target.x = CenterM[cameraNo].x;
    target.y = CenterM[cameraNo].y;
    target.z = CenterM[cameraNo].z;
    up.x = sin((M_PI * rotY) / 180.0) * sin((M_PI * rotX) / 180.0);
    up.y = cos((M_PI * rotX) / 180.0);
    up.z = cos((M_PI * rotY) / 180.0) * sin((M_PI * rotX) / 180.0);
}

/* Draws the ripples, the obstacle shadows and the obstacles themselves (billboards cut from the frame buffer). */
void fn_1_C0BC(HU3DMODEL *model, Mtx *mtx)
{
    Mtx lookAt;
    Mtx camMtx;
    Vec corners[4] = { { -1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { 1.0f, -1.0f, 0.0f }, { -1.0f, -1.0f, 0.0f } };
    Vec billboard[4];
    Mtx rippleMtx;
    Mtx tempMtx;
    HU3DCAMERA *camera;
    s32 i;
    s32 pass;
    s32 quadNo;
    s32 side;
    s32 color;
    s32 alpha;
    s32 frame;

    for (i = 0; i < 16; i++) {
        if (model->cameraBit & (1 << i)) {
            break;
        }
    }
    camera = &Hu3DCamera[i];
    C_MTXLookAt(lookAt, &camera->pos, &camera->up, &camera->target);
    PSMTXTranspose(lookAt, camMtx);
    PSMTXMultVecArray(camMtx, corners, billboard, 4);
    GXLoadPosMtxImm(*mtx, GX_PNMTX0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_AND, GX_GEQUAL, 0x80);
    GXSetZCompLoc(GX_FALSE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
    GXSetZMode(GX_TRUE, GX_GREATER, GX_FALSE);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZCompLoc(GX_TRUE);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASA, GX_CC_RASC);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    for (pass = 0; pass < 2; pass++) {
        HuSprTexLoad(espanim[esprite[lbl_1_data_0[(pass == 0) ? 0 : 1]].animNo].anim, 0, GX_TEXMAP0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
        for (i = 0; i < 640; i++) {
            Vec quad[4][4] = {
                { { -1.0f, 0.0f, 2.0f }, { 1.0f, 0.0f, 2.0f }, { 1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f } },
                { { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, -2.0f }, { -1.0f, 0.0f, -2.0f } },
            };
            float uv[2][4] = { { 0.5f, 0.0f, 1.0f, 0.5f }, { 0.0f, 0.5f, 0.5f, 1.0f } };
            float size = 20.0f;
            float y = -3.0f;
            Vec pos = { 0.0f, 55.0f, 0.0f };

            if (lbl_1_bss_1F7C[i].time < 0) {
                continue;
            }
            if (pass == 0 && lbl_1_bss_1F7C[i].unk29 >= 2) {
                continue;
            }
            if (pass == 1 && lbl_1_bss_1F7C[i].unk29 < 2) {
                continue;
            }
            pos.x = lbl_1_bss_1F7C[i].unk0;
            pos.y = y;
            pos.z = lbl_1_bss_1F7C[i].unk8;
            size = (lbl_1_bss_1F7C[i].unk29 != 1) ? 20 : 65;
            alpha = 255 - (lbl_1_bss_1F7C[i].time * 255) / 192;
            color = 0;
            side = quadNo = lbl_1_bss_1F7C[i].unk28;
            if (lbl_1_bss_1F7C[i].unkC != 0.0f || lbl_1_bss_1F7C[i].unk29 >= 2) {
                Vec axis = { 0.0f, 1.0f, 0.0f };

                PSMTXScale(rippleMtx, lbl_1_bss_1F7C[i].unk24, 1.0f, lbl_1_bss_1F7C[i].unk20);
                PSMTXTrans(tempMtx, -lbl_1_bss_1F7C[i].unk1C, 0.0f, -lbl_1_bss_1F7C[i].unk18);
                PSMTXConcat(tempMtx, rippleMtx, rippleMtx);
                PSMTXTrans(tempMtx, 0.0f, 0.0f, -lbl_1_bss_1F7C[i].unk14 * ((side * 2) - 1));
                PSMTXConcat(tempMtx, rippleMtx, rippleMtx);
                PSMTXRotAxisRad(tempMtx, &axis, MTXDegToRad(lbl_1_bss_1F7C[i].unkC));
                PSMTXConcat(tempMtx, rippleMtx, rippleMtx);
                PSMTXRotAxisRad(tempMtx, &axis, MTXDegToRad(-lbl_1_bss_1F7C[i].unk10 * ((side * 2) - 1)));
                PSMTXConcat(tempMtx, rippleMtx, rippleMtx);
                PSMTXMultVecArray(rippleMtx, quad[quadNo], quad[quadNo + 2], 4);
                quadNo += 2;
            }
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(pos.x + size * quad[quadNo][0].x, y, pos.z - size * quad[quadNo][0].z);
            GXColor4u8(color, color, 0, alpha);
            GXTexCoord2f32(uv[side][0], uv[side][1]);
            GXPosition3f32(pos.x + size * quad[quadNo][1].x, y, pos.z - size * quad[quadNo][1].z);
            GXColor4u8(color, color, 0, alpha);
            GXTexCoord2f32(uv[side][2], uv[side][1]);
            GXPosition3f32(pos.x + size * quad[quadNo][2].x, y, pos.z - size * quad[quadNo][2].z);
            GXColor4u8(color, color, 0, alpha);
            GXTexCoord2f32(uv[side][2], uv[side][3]);
            GXPosition3f32(pos.x + size * quad[quadNo][3].x, y, pos.z - size * quad[quadNo][3].z);
            GXColor4u8(color, color, 0, alpha);
            GXTexCoord2f32(uv[side][0], uv[side][3]);
            GXEnd();
        }
        GXPixModeSync();
    }
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZMode(GX_TRUE, GX_GREATER, GX_FALSE);
    GXSetAlphaCompare(GX_GREATER, 1, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZCompLoc(GX_TRUE);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    for (i = 0; i < 64; i++) {
#ifdef TARGET_PC
        HU3DMODEL *objModel = NULL; /* unused; the original reads past the end of the model list for i >= 13 */
#else
        HU3DMODEL *objModel = &Hu3DData[lbl_1_bss_2ED34->mdlId[i + 13]];
#endif
        float scale = 26.0f * lbl_1_bss_1154;
        float shadowY = -3.0f;
        s32 shadowAlpha = 127;
        Vec pos = { 0.0f, 55.0f, 0.0f };

        pos.x += lbl_1_bss_9D1C[i].pos.x;
        pos.z += lbl_1_bss_9D1C[i].pos.z;
        if (lbl_1_bss_9D1C[i].state >= 0 && lbl_1_bss_9D1C[i].lane < 0) {
            frame = lbl_1_bss_9D1C[i].time / 16;
            frame = (lbl_1_bss_9D1C[i].animNo != 2 || lbl_1_data_119 == 3)
                ? frame
                : ((lbl_1_data_119 == 2 && lbl_1_data_111[0] == 2) ? ((lbl_1_data_115[0] > frame) ? frame : lbl_1_data_115[0]) : 0);
            GXInitTexObj(&lbl_1_bss_918[i], (u8 *)lbl_1_bss_1158[lbl_1_bss_9D1C[i].type][lbl_1_bss_9D1C[i].animNo] + lbl_1_bss_1198 * frame, 26,
                22, GX_TF_I8, GX_CLAMP, GX_CLAMP, GX_FALSE);
            GXLoadTexObj(&lbl_1_bss_918[i], GX_TEXMAP0);
            GXSetAlphaCompare(GX_GEQUAL, (12.5f * lbl_1_bss_9D1C[i].unk93C) / 255.0f, GX_AOP_AND, GX_ALWAYS, 0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(pos.x + corners[0].x * scale, shadowY, pos.z - corners[0].y * scale);
            GXColor4u8(60, 90, 150, shadowAlpha);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(pos.x + corners[1].x * scale, shadowY, pos.z - corners[1].y * scale);
            GXColor4u8(60, 90, 150, shadowAlpha);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(pos.x + corners[2].x * scale, shadowY, pos.z - corners[2].y * scale);
            GXColor4u8(60, 90, 150, shadowAlpha);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(pos.x + corners[3].x * scale, shadowY, pos.z - corners[3].y * scale);
            GXColor4u8(60, 90, 150, shadowAlpha);
            GXTexCoord2f32(0.0f, 1.0f);
            GXEnd();
            GXPixModeSync();
        }
    }
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetAlphaCompare(GX_GEQUAL, 0xC0, GX_AOP_AND, GX_GEQUAL, 0xC0);
    GXSetZCompLoc(GX_FALSE);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    for (i = 0; i < 64; i++) {
        HU3DMODEL *objModel = &Hu3DData[lbl_1_bss_2ED34->mdlId[13]];
        Vec pos = { 0.0f, 0.0f, 0.0f };

        if (lbl_1_bss_9D1C[i].state >= 0 && lbl_1_bss_9D1C[i].lane < 0 && lbl_1_bss_9D1C[i].unk93C >= 255) {
            pos.x = lbl_1_bss_9D1C[i].pos.x;
            pos.y = 55.0f + lbl_1_bss_9D1C[i].pos.y;
            pos.z = lbl_1_bss_9D1C[i].pos.z;
            GXInitTexObj(&lbl_1_bss_918[i],
                (void *)((uintptr_t)lbl_1_bss_11AC[lbl_1_bss_9D1C[i].type][lbl_1_bss_9D1C[i].animNo]
                    + (lbl_1_bss_9D1C[i].time / 16) * lbl_1_bss_11EC[lbl_1_bss_9D1C[i].type]),
                56, 60, GX_TF_RGB5A3, GX_CLAMP, GX_CLAMP, GX_FALSE);
            GXLoadTexObj(&lbl_1_bss_918[i], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(pos.x + 32.0f * billboard[0].x * lbl_1_bss_1154, pos.y + 28.0f * billboard[0].y * lbl_1_bss_1150,
                pos.z + 28.0f * billboard[0].z * lbl_1_bss_1150);
            GXColor4u8(255, 255, 255, 255);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(pos.x + 24.0f * billboard[1].x * lbl_1_bss_1154, pos.y + 28.0f * billboard[1].y * lbl_1_bss_1150,
                pos.z + 28.0f * billboard[1].z * lbl_1_bss_1150);
            GXColor4u8(255, 255, 255, 255);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(pos.x + 24.0f * billboard[2].x * lbl_1_bss_1154, pos.y + 32.0f * billboard[2].y * lbl_1_bss_1150,
                pos.z + 32.0f * billboard[2].z * lbl_1_bss_1150);
            GXColor4u8(255, 255, 255, 0);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(pos.x + 32.0f * billboard[3].x * lbl_1_bss_1154, pos.y + 32.0f * billboard[3].y * lbl_1_bss_1150,
                pos.z + 32.0f * billboard[3].z * lbl_1_bss_1150);
            GXColor4u8(255, 255, 255, 0);
            GXTexCoord2f32(0.0f, 1.0f);
            GXEnd();
            GXPixModeSync();
        }
    }
    GXSetZCompLoc(GX_TRUE);
}

/* Copies each character from the frame buffer into its animation strip. */
void fn_1_B694(HU3DMODEL *model, Mtx *mtx)
{
    GXColor clearColor = { 64, 64, 64, 64 };
    Vec pos2d;
    Vec pos;
    s32 i;
    HU3DMODEL *charModel;
    s32 x;
    s32 y;
    void *buf;
    s32 texW;
    s32 texH;
    s32 w;
    s32 h;

    if (WipeCheckEnd() == 0) {
        if (lbl_1_data_119 == 2) {
            fn_1_BD58(model, mtx);
            return;
        }
        GXSetCopyFilter(GX_FALSE, RenderMode->sample_pattern, GX_FALSE, RenderMode->vfilter);
        GXPixModeSync();
        GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
        for (i = 0; i < ((lbl_1_data_119 == 4) ? 1 : 4); i++) {
            if (lbl_1_data_111[i] == -1) {
                continue;
            }
            if (lbl_1_data_111[i] >= ((i == 0) ? 4 : 1)) {
                continue;
            }
            if (!((lbl_1_data_119 == 0) ? 1 : ((lbl_1_data_119 == 1) ? (i == 1) : 0))) {
                continue;
            }
            charModel = &Hu3DData[lbl_1_bss_2ED34->mdlId[i + 13]];
            buf = (i != 1 || lbl_1_data_119 == 1) ? (void *)((uintptr_t)lbl_1_bss_11AC[i][lbl_1_data_111[i]] + lbl_1_data_115[i] * lbl_1_bss_11EC[i])
                                                  : lbl_1_bss_119C[i];
            w = (i == 0) ? 32 : (i == 1) ? 40 : (i == 2) ? 20 : 12;
            h = (i == 0) ? 28 : (i == 1) ? 34 : (i == 2) ? 20 : 12;
            texW = (i == 0) ? 56 : (i == 1) ? 80 : (i == 2) ? 40 : 24;
            texH = (i == 0) ? 60 : (i == 1) ? 68 : (i == 2) ? 40 : 24;
            pos.x = charModel->pos.x;
            pos.y = charModel->pos.y + ((i == 0) ? 55.0f : (i == 1) ? -90.0f : 0.0f);
            pos.z = charModel->pos.z;
            Hu3D3Dto2D(&pos, HU3D_CAM0, &pos2d);
            pos2d.x = 320.0f + 640.0f * ((pos2d.x - 288.0f) / 576.0f);
            pos2d.y = 240.0f + 480.0f * ((pos2d.y - 240.0f) / 480.0f);
            x = pos2d.x - w;
            y = pos2d.y - h;
            x = ((x + 1) / 2) * 2;
            y = ((y + 1) / 2) * 2;
            GXSetTexCopySrc(x, y, texW, texH);
            GXSetTexCopyDst(texW, texH, GX_TF_RGB5A3, GX_FALSE);
            GXSetCopyClear(clearColor, 0xFFFFFF);
            GXCopyTex(buf, GX_TRUE);
            DCFlushRange(buf, lbl_1_bss_11EC[i]);
            GXPixModeSync();
        }
        GXSetCopyFilter(RenderMode->aa, RenderMode->sample_pattern, GX_TRUE, RenderMode->vfilter);
        {
            GXColor color = { 0, 0, 0, 0 };

            GXSetCopyClear(color, 0xFFFFFF);
        }
    }
}

/* Copy mode 2: only grabs the head of the first character, as an intensity texture. */
void fn_1_BD58(HU3DMODEL *model, Mtx *mtx)
{
    GXColor clearColor = { 64, 64, 64, 64 };
    Vec pos2d;
    s32 i;
    HU3DMODEL *charModel;
    s32 x;
    s32 y;
    void *buf;

    GXPixModeSync();
    for (i = 0; i < 1; i++) {
        if (lbl_1_data_111[i] != -1 && lbl_1_data_111[i] < 4) {
            charModel = &Hu3DData[lbl_1_bss_2ED34->mdlId[i + 13]];
            {
                Vec pos = { 0.0f, 55.0f, 0.0f };

                buf = (void *)((uintptr_t)lbl_1_bss_1158[i][lbl_1_data_111[i]] + lbl_1_bss_1198 * lbl_1_data_115[i]);
                pos.x += charModel->pos.x;
                pos.y += charModel->pos.y;
                pos.z += charModel->pos.z;
                Hu3D3Dto2D(&pos, HU3D_CAM0, &pos2d);
                pos2d.x = 320.0f + 640.0f * ((pos2d.x - 288.0f) / 576.0f);
                pos2d.y = 240.0f + 480.0f * ((pos2d.y - 240.0f) / 480.0f);
                x = pos2d.x - 26.0f;
                y = pos2d.y - 22.0f;
                x = ((x + 1) / 2) * 2;
                y = ((y + 1) / 2) * 2;
                GXSetTexCopySrc(x, y, 52, 44);
                GXSetTexCopyDst(26, 22, GX_TF_I8, GX_TRUE);
                GXSetCopyClear(clearColor, 0xFFFFFF);
                GXCopyTex(buf, GX_TRUE);
                DCFlushRange(buf, lbl_1_bss_11EC[i]);
                GXPixModeSync();
                }
        }
    }
    {
        GXColor color = { 0, 0, 0, 0 };

        GXSetCopyClear(color, 0xFFFFFF);
    }
}

void fn_1_EF6C(OMOBJ *obj)
{
    M505Player *players[4];
    s32 i;
    s32 aliveNum;
    s32 playerNo;
    s32 pan;
    s32 seNo;

    for (i = 0; i < 4; i++) {
        players[i] = lbl_1_bss_2ED38[i]->data;
    }
    for (i = 0; i < 11; i++) {
        s32 seTbl[11] = { 0x660, 0x661, 0x662, 0x663, 0x664, 0x665, 0x663, 0x667, 0x668, 0x669, 0x66A };

        if (lbl_1_bss_2ED50[i].num > 3) {
            lbl_1_bss_2ED50[i].num = 3;
        }
        if (lbl_1_bss_2ED50[i].num > 0) {
            if (seTbl[i] != -1) {
                seNo = HuAudFXPlay(seTbl[i]);
                lbl_1_bss_2ED50[i].seNo = seNo;
                if (lbl_1_bss_2ED50[i].panF[lbl_1_bss_2ED50[i].num - 1] != 0) {
                    pan = (s16)(0.5 + (64.0f + (32.0f * ((lbl_1_bss_2ED50[i].posX[lbl_1_bss_2ED50[i].num - 1] - CenterM[1].x) / 700.0f))));
                    pan = (pan >= 96) ? 96 : pan;
                    HuAudFXPanning(seNo, pan);
                }
                lbl_1_bss_2ED50[i].panF[lbl_1_bss_2ED50[i].num - 1] = 0;
            }
            lbl_1_bss_2ED50[i].num--;
        }
    }
    if (omSysExitReq != 0 && WipeCheckEnd() == 0 && lbl_1_bss_8 == 0) {
        lbl_1_bss_8 = 1;
        HuAudFadeOut(1000);
    }
    switch (lbl_1_bss_2ED2C) {
        case 1000:
            lbl_1_bss_0 += 1.0f;
            if (lbl_1_bss_0 == 2.0f) {
                lbl_1_data_84 = HuAudFXPlay(0x39F);
            }
            if (WipeCheckEnd() == 0) {
                lbl_1_bss_2ED2C++;
                lbl_1_bss_0 = 0.0f;
            }
            break;
        case 1001:
            lbl_1_bss_0 += (lbl_1_bss_9D1C[0].state == -1);
            if (lbl_1_bss_0 >= 50.0f) {
                lbl_1_bss_2ED2C++;
                lbl_1_bss_0 = 0.0f;
            }
            break;
        case 1002:
            lbl_1_bss_0 += 1.0f;
            if (lbl_1_bss_0 == 5.0f && lbl_1_bss_2ED26 < 0) {
                lbl_1_bss_2ED26 = GameMesMgStartCreate();
            }
            if (lbl_1_bss_0 >= 5.0f && lbl_1_data_80 == -1) {
                lbl_1_data_80 = HuAudSStreamPlay(0x37);
            }
            if (lbl_1_bss_0 >= 5.0f && GameMesStatGet(lbl_1_bss_2ED26) == 0 && lbl_1_bss_2ED26 >= 0) {
                lbl_1_bss_2ED2C++;
                lbl_1_bss_2ED20 = 1800;
                lbl_1_bss_2ED24 = GameMesTimerCreate(lbl_1_bss_2ED20 / 60);
                GameMesPosSet(lbl_1_bss_2ED24, 525.0f, 64.0f);
                lbl_1_bss_2ED26 = -1;
                lbl_1_bss_0 = 0.0f;
                for (i = 0; i < 4; i++) {
                    players[i]->mode = 1;
                }
            }
            break;
        case 1003:
            lbl_1_bss_0 += 1.0f;
            GameMesTimerValueSet(lbl_1_bss_2ED24, (lbl_1_bss_2ED20 + 58) / 60);
            lbl_1_bss_2ED20--;
            aliveNum = 0;
            for (i = 0; i < 4; i++) {
                aliveNum += players[i]->mode != 2;
            }
            if (lbl_1_bss_2ED20 <= 0 || aliveNum <= 1) {
                lbl_1_bss_2ED2C = 1004;
                lbl_1_bss_0 = 0.0f;
            }
            break;
        case 1004:
            if (lbl_1_bss_0 == 10.0f) {
                lbl_1_data_78 = 4;
                for (i = 0; i < 4; i++) {
                    lbl_1_data_78 = (players[i]->mode != 2) ? i : lbl_1_data_78;
                    players[i]->mode = (players[i]->mode == 1) ? 3 : players[i]->mode;
                }
            }
            if (lbl_1_bss_2ED26 < 0 && lbl_1_bss_0 == 0.0f) {
                GameMesTimerEnd(lbl_1_bss_2ED24);
                lbl_1_bss_2ED26 = GameMesMgFinishCreate();
                if (lbl_1_data_80 != -1) {
                    HuAudSStreamFadeOut(lbl_1_data_80, 100);
                }
            }
            else if (GameMesStatGet(lbl_1_bss_2ED26) == 0 && lbl_1_bss_0 >= 90.0f) {
                s32 winNum = 0;
                s32 winner[4] = { -1, -1, -1, -1 };

                lbl_1_bss_2ED2C++;
                lbl_1_bss_0 = 0.0f;
                for (i = 0; i < 4; i++) {
                    if (players[i]->mode != 2) {
                        winner[winNum] = players[i]->charNo;
                        playerNo = players[i]->playerNo;
                        if (!_CheckFlag(FLAG_MG_PRACTICE)) {
                            GwPlayer[playerNo].mgCoinBonus = 10;
                        }
                        winNum++;
                    }
                }
                if (winNum > 0) {
                    lbl_1_bss_2ED26 = GameMesMgWinnerCreate(winner[0], winner[1], winner[2], winner[3]);
                    HuAudSStreamPlay(0x79);
                }
                else {
                    lbl_1_bss_2ED26 = GameMesMgDrawCreate();
                    HuAudSStreamPlay(0x7B);
                }
            }
            lbl_1_bss_0 += 1.0f;
            break;
        case 1005:
            lbl_1_bss_0 += 1.0f;
            if (210.0f < lbl_1_bss_0 && lbl_1_bss_8 == 0) {
                lbl_1_bss_8 = 1;
                HuAudFXFadeOut(lbl_1_data_84, 1000);
                lbl_1_bss_2ED26 = -1;
            }
            break;
        default:
            OSReport("*** main mode error(%d)!!\n", lbl_1_bss_2ED2C);
            break;
    }
}

void fn_1_FADC(OMOBJ *obj)
{
    Mtx lookAt;
    Mtx camMtx;
    Mtx scale;
    Vec pos[2];
    Vec waterMin = { -1250.0f, -200.0f, -1550.0f };
    Vec waterMax = { 1900.0f, -200.0f, 1000.0f };
    s32 i;
    s32 j;
    s32 mdlNo;
    s32 camNo;
    HU3DCAMERA *camera;
    s32 w;
    s32 h;
    float ofsX;
    s32 posNo;

    Hu3DWaterCreate(1, HU3D_WATER_ANIM_NONE, HU3D_WATER_ANIM_NONE, HU3D_WATER_ANIM_NONE, FALSE, &waterMin, &waterMax);
    Hu3DWaterCameraSet(HU3D_CAM1);
    Hu3DWaterTexScaleSet(2.4f, 0.9f, 1.0f);
    obj->mdlId[0] = Hu3DModelCreateData(DATANUM(DATA_m505, 0));
    Hu3DModelLayerSet(obj->mdlId[0], 0);
    Hu3DModelCameraSet(obj->mdlId[0], HU3D_CAM1);
    omSetTra(obj, 0.0f, 0.0f, 0.0f);
    Hu3DModelShadowMapSet(obj->mdlId[0]);
    obj->mdlId[1] = Hu3DModelCreateData(DATANUM(DATA_m505, 1));
    Hu3DModelLayerSet(obj->mdlId[1], 0);
    Hu3DModelCameraSet(obj->mdlId[1], HU3D_CAM1);
    Hu3DModelShadowMapSet(obj->mdlId[1]);
    obj->mdlId[2] = Hu3DModelCreateData(DATANUM(DATA_m505, 2));
    Hu3DModelLayerSet(obj->mdlId[2], 2);
    Hu3DModelCameraSet(obj->mdlId[2], HU3D_CAM1);
    Hu3DModelAttrSet(obj->mdlId[2], HU3D_MOTATTR_PAUSE);
    obj->mdlId[3] = Hu3DHookFuncCreate(fn_1_0);
    Hu3DModelLayerSet(obj->mdlId[3], 0);
    Hu3DModelCameraSet(obj->mdlId[3], HU3D_CAM0);
    Hu3DModelRotSet(obj->mdlId[3], 255.0f, 255.0f, 255.0f);
    obj->mdlId[4] = Hu3DHookFuncCreate(fn_1_B694);
    Hu3DModelLayerSet(obj->mdlId[4], 4);
    Hu3DModelCameraSet(obj->mdlId[4], HU3D_CAM0);
    obj->mdlId[5] = Hu3DHookFuncCreate(fn_1_C0BC);
    Hu3DModelLayerSet(obj->mdlId[5], 0);
    Hu3DModelCameraSet(obj->mdlId[5], HU3D_CAM1);
    Hu3DModelAttrSet(obj->mdlId[5], HU3D_ATTR_DISPOFF);
    obj->mdlId[7] = Hu3DHookFuncCreate(fn_1_D9E0);
    Hu3DModelLayerSet(obj->mdlId[7], 3);
    Hu3DModelCameraSet(obj->mdlId[7], HU3D_CAM1);
    Hu3DModelAttrSet(obj->mdlId[7], HU3D_ATTR_DISPOFF);
    obj->mdlId[14] = Hu3DModelCreateData(DATANUM(DATA_m505, 7));
    Hu3DModelLayerSet(obj->mdlId[14], 0);
    Hu3DModelCameraSet(obj->mdlId[14], HU3D_CAM0);
    Hu3DModelAttrSet(obj->mdlId[14], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(obj->mdlId[14], HU3D_MOTATTR_LOOP);
    Hu3DModelPosSet(obj->mdlId[14], -600.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(obj->mdlId[14], 0.5f, 0.5f, 0.5f);
    for (i = 0; i < 2; i++) {
        obj->mdlId[i + 15] = Hu3DModelCreateData((i == 0) ? DATANUM(DATA_m505, 3) : DATANUM(DATA_m505, 4));
        Hu3DModelLayerSet(obj->mdlId[i + 15], 0);
        Hu3DModelCameraSet(obj->mdlId[i + 15], HU3D_CAM0);
        Hu3DModelAttrSet(obj->mdlId[i + 15], HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(obj->mdlId[i + 15], HU3D_MOTATTR_LOOP);
        Hu3DModelPosSet(obj->mdlId[i + 15], 100.0f, ((i == 0) ? -1 : 1) * -100, ((i == 0) ? -1 : 1) * 100);
        Hu3DModelRotSet(obj->mdlId[i + 15], -63.0f, 0.0f, 0.0f);
    }
    for (i = 0; i < 4; i++) {
        obj->mdlId[i + 8] = Hu3DModelCreateData((i < 3) ? DATANUM(DATA_m505, 3) : DATANUM(DATA_m505, 4));
        Hu3DModelLayerSet(obj->mdlId[i + 8], 3);
        Hu3DModelCameraSet(obj->mdlId[i + 8], HU3D_CAM1);
        Hu3DModelAttrSet(obj->mdlId[i + 8], HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(obj->mdlId[i + 8], HU3D_MOTATTR_LOOP);
    }
    obj->mdlId[12] = Hu3DModelCreateData(DATANUM(DATA_m505, 14));
    Hu3DModelLayerSet(obj->mdlId[12], 1);
    Hu3DModelCameraSet(obj->mdlId[12], HU3D_CAM1);
    Hu3DModelAttrSet(obj->mdlId[12], HU3D_MOTATTR_PAUSE);
    Hu3DModelPosSet(obj->mdlId[12], 1300.0f, 0.0f, -400.0f);
    Hu3DModelShadowSet(obj->mdlId[12]);
    for (i = 0; i < 2; i++) {
        float sign[2] = { -1.0f, 1.0f };
        float zero[2] = { 0.0f, 0.0f };
        u32 mdlData[2] = { DATANUM(DATA_m505, 14), DATANUM(DATA_m505, 8) };
        u32 mot0Data[2] = { DATANUM(DATA_m505, 16), DATANUM(DATA_m505, 10) };
        u32 mot1Data[2] = { DATANUM(DATA_m505, 18), DATANUM(DATA_m505, 12) };
        u32 mot2Data[2] = { DATANUM(DATA_m505, 15), DATANUM(DATA_m505, 9) };
        u32 mot3Data[2] = { DATANUM(DATA_m505, 17), DATANUM(DATA_m505, 11) };

        mdlNo = (i == 0) ? 13 : 17;
        obj->mdlId[mdlNo] = Hu3DModelCreateData(mdlData[i]);
        obj->mtnId[i] = Hu3DJointMotionData(obj->mdlId[mdlNo], mot0Data[i]);
        obj->mtnId[i + 2] = Hu3DJointMotionData(obj->mdlId[mdlNo], mot1Data[i]);
        obj->mtnId[i + 4] = Hu3DJointMotionData(obj->mdlId[mdlNo], mot2Data[i]);
        obj->mtnId[i + 6] = Hu3DJointMotionData(obj->mdlId[mdlNo], mot3Data[i]);
        {
            Vec ofs[2] = { { -1.0f, 0.0f, 0.0f }, { 3.0f, 0.0f, 0.0f } };

            for (camNo = 0; camNo < 16; camNo++) {
                if ((1 << camNo) & HU3D_CAM0) {
                    break;
                }
            }
            camera = &Hu3DCamera[camNo];
            C_MTXLookAt(lookAt, &camera->pos, &camera->up, &camera->target);
            PSMTXTranspose(lookAt, camMtx);
            PSMTXScale(scale, 150.0f, 150.0f, 150.0f);
            PSMTXConcat(scale, camMtx, camMtx);
#ifdef TARGET_PC
            PSMTXMultVecArray(camMtx, ofs, pos, 2);
#else
            /* only two vectors are declared: the last two results land in ofs */
            PSMTXMultVecArray(camMtx, ofs, pos, 4);
#endif
        }
        Hu3DModelPosSet(obj->mdlId[mdlNo], pos[i].x, pos[i].y, pos[i].z);
        Hu3DModelRotSet(obj->mdlId[mdlNo], 0.0f, 270.0f, 0.0f);
        Hu3DMotionSet(obj->mdlId[mdlNo], obj->mtnId[i]);
        Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_MOTATTR_LOOP);
        Hu3DModelLayerSet(obj->mdlId[mdlNo], 1);
        Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM0);
        Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_ATTR_DISPOFF);
        if (i == 1) {
            for (j = 0; j < 8; j++) {
                obj->mdlId[j + 18] = Hu3DModelCreateData(mdlData[i]);
                Hu3DModelRotSet(obj->mdlId[j + 18], 0.0f, 270.0f, 0.0f);
                Hu3DMotionSet(obj->mdlId[j + 18], obj->mtnId[i]);
                Hu3DModelAttrSet(obj->mdlId[j + 18], HU3D_MOTATTR_LOOP);
                Hu3DModelLayerSet(obj->mdlId[j + 18], 1);
                Hu3DModelCameraSet(obj->mdlId[j + 18], HU3D_CAM0);
                Hu3DModelShadowSet(obj->mdlId[j + 18]);
                Hu3DModelAttrSet(obj->mdlId[j + 18], HU3D_ATTR_DISPOFF);
            }
        }
    }
    for (i = 0; i < 4; i++) {
        w = (i == 0) ? 56 : (i == 1) ? 80 : (i == 2) ? 40 : 24;
        h = (i == 0) ? 60 : (i == 1) ? 68 : (i == 2) ? 40 : 24;
        lbl_1_bss_11EC[i] = GXGetTexBufferSize(w, h, GX_TF_RGB5A3, GX_FALSE, 0);
        if (i == 0) {
            lbl_1_bss_1198 = GXGetTexBufferSize(26, 22, GX_TF_I8, GX_FALSE, 0);
        }
        for (j = 0; j < ((i == 0) ? 4 : 1); j++) {
            s32 motOfs[4] = { 0, 2, 4, 6 };

            if (i == 0) {
                lbl_1_data_F0[i][j] = Hu3DMotionMotionMaxTimeGet(obj->mtnId[i + motOfs[j]]) + 1;
                if (j == 3) {
                    lbl_1_data_F0[i][j] *= 2;
                }
            }
            else {
                lbl_1_data_F0[i][j] = Hu3DMotionMaxTimeGet(obj->mdlId[i + 13]) + 1;
            }
            if (i < 4) {
                lbl_1_bss_11AC[i][j] = HuMemDirectMallocNum(HEAP_MODEL, lbl_1_bss_11EC[i] * lbl_1_data_F0[i][j], HU_MEMNUM_OVL);
                if (i == 0) {
                    lbl_1_bss_1158[i][j] = HuMemDirectMallocNum(HEAP_MODEL, lbl_1_bss_1198 * lbl_1_data_F0[i][j], HU_MEMNUM_OVL);
                }
            }
        }
        if (i == 1) {
            lbl_1_bss_119C[i] = HuMemDirectMallocNum(HEAP_MODEL, lbl_1_bss_11EC[i], HU_MEMNUM_OVL);
        }
    }
    for (i = 0; i < 2; i++) {
        u32 espData[2] = { DATANUM(DATA_m505, 5), DATANUM(DATA_m505, 6) };

        lbl_1_data_0[i] = espEntry(espData[i], 0, 0);
        espDispOff(lbl_1_data_0[i]);
    }
    for (i = 0; i < 64; i++) {
        lbl_1_bss_9D1C[i].state = -1;
        lbl_1_bss_9D1C[i].rnd = rand8();
        lbl_1_bss_9D1C[i].type = 0;
        lbl_1_bss_9D1C[i].pos.x = 1000.0 + 1600.0 * (rand8() / 255.0);
        lbl_1_bss_9D1C[i].pos.y = 0.0f;
        lbl_1_bss_9D1C[i].pos.z = ((rand8() % 12) * 100) - 550;
        lbl_1_bss_9D1C[i].rotY = 0.0f;
        lbl_1_bss_9D1C[i].vel.x = (lbl_1_bss_9D1C[i].type == 0) ? (-400.0 + 200.0 * ((rand8() / 255.0) - 0.75)) / 60.0
                                                                 : (-600.0 + 200.0 * ((rand8() / 255.0) - 0.5)) / 60.0;
        lbl_1_bss_9D1C[i].vel.y = 0.0f;
        lbl_1_bss_9D1C[i].vel.z = 0.0f;
        lbl_1_bss_9D1C[i].lane = -1;
        lbl_1_bss_9D1C[i].animNo = 0;
#ifdef TARGET_PC
        /* The original indexes the frame table with the object number (i up to 63), reading far past its six rows;
         * the result only randomizes the starting animation frame, so the port uses the object's own row. */
        lbl_1_bss_9D1C[i].time0 = lbl_1_bss_9D1C[i].time
            = ((i == 1) ? 2 : 1) * ((lbl_1_data_F0[lbl_1_bss_9D1C[i].type][0] * 16 * rand8()) / 255);
#else
        lbl_1_bss_9D1C[i].time0 = lbl_1_bss_9D1C[i].time = ((i == 1) ? 2 : 1) * ((lbl_1_data_F0[i][0] * 16 * rand8()) / 255);
#endif
        lbl_1_bss_9D1C[i].speed = (lbl_1_bss_9D1C[i].type == 0) ? 256.0f * (2.0f * ((60.0f * -lbl_1_bss_9D1C[i].vel.x) / 350.0f))
                                                                 : 256.0f * ((60.0f * -lbl_1_bss_9D1C[i].vel.x) / 500.0f);
        lbl_1_bss_9D1C[i].unk37 = 0;
        if (i == 0) {
            lbl_1_bss_9D1C[i].state = 0;
            lbl_1_bss_9D1C[i].pos.x = 1300.0f;
            lbl_1_bss_9D1C[i].pos.z = -370.0f;
            lbl_1_bss_9D1C[i].rotY = -90.0f;
            lbl_1_bss_9D1C[i].vel.x = -9.166667f;
            lbl_1_bss_9D1C[i].lane = 0;
            lbl_1_bss_9D1C[i].time = 240;
            lbl_1_bss_9D1C[i].speed = 0;
        }
        lbl_1_bss_9D1C[i].prevPos.x = lbl_1_bss_9D1C[i].pos.x;
        lbl_1_bss_9D1C[i].prevPos.y = lbl_1_bss_9D1C[i].pos.y;
        lbl_1_bss_9D1C[i].prevPos.z = lbl_1_bss_9D1C[i].pos.z;
        lbl_1_bss_9D1C[i].unk38 = 0;
        for (j = 0; j < 96; j++) {
            lbl_1_bss_9D1C[i].wake[j].unk15 = 0;
        }
        lbl_1_bss_9D1C[i].unk93C = 255;
    }
    memset(lbl_1_bss_977C, 0, sizeof(lbl_1_bss_977C));
    for (i = 0; i < 72; i++) {
        lbl_1_bss_977C[i].time = -1;
    }
    memset(lbl_1_bss_1F7C, 0, sizeof(lbl_1_bss_1F7C));
    for (i = 0; i < 640; i++) {
        lbl_1_bss_1F7C[i].time = -1;
    }
    memset(lbl_1_bss_11FC, 0, sizeof(lbl_1_bss_11FC));
    for (i = 0; i < 96; i++) {
        lbl_1_bss_11FC[i].state = -1;
    }
    posNo = rand8() % 5;
    for (i = 0; i < 4; i++) {
        lbl_1_bss_11FC[i].state = 0;
        ofsX = (450.0f * (rand8() / 255.0f), 0.0f);
        lbl_1_bss_11FC[i].pos.x = (posNo * 450 + ofsX) - 900.0f;
        lbl_1_bss_11FC[i].pos.y = ((rand8() * 3) / 2, 0) + 1400;
        if (posNo == 4) {
            lbl_1_bss_11FC[i].pos.x += rand8() + 350;
            lbl_1_bss_11FC[i].pos.y -= 500.0f;
        }
        lbl_1_bss_11FC[i].pos.z = 1800.0f * (rand8() / 255.0f) - 900.0f;
        lbl_1_bss_11FC[i].pos.z = (lbl_1_bss_11FC[i].pos.z < 0.0f) ? -lbl_1_bss_11FC[i].pos.z : lbl_1_bss_11FC[i].pos.z;
        lbl_1_bss_11FC[i].unkC = 0.0f;
        lbl_1_bss_11FC[i].dir = ((rand8() % 2) * 2) - 1;
        lbl_1_bss_11FC[i].velY = -3.5f;
        lbl_1_bss_11FC[i].pos.y -= 30.0f * lbl_1_bss_11FC[i].velY;
        lbl_1_bss_11FC[i].unk18 = 0;
        lbl_1_bss_11FC[i].type = (i < 3) ? 0 : 1;
        lbl_1_bss_11FC[i].frame = lbl_1_data_F0[lbl_1_bss_11FC[i].type + 2][0] * (rand8() / 255.0f);
        lbl_1_bss_11FC[i].no = i;
        posNo = (posNo + 1) % 5;
    }
    obj->objFunc = fn_1_4FB4;
}

void fn_1_11650(OMOBJ *obj)
{
    s32 order[4] = { 0, 1, 2, 3 };
    s32 num;
    M505Player *player;
    s32 i;
    s32 j;

    num = 0;
    player = obj->data;
    memset(player, 0, sizeof(M505Player));
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (i == GwPlayerConf[j].grp) {
                order[num] = j;
                num++;
            }
        }
    }
    player->no = obj->work[0];
    player->playerNo = order[player->no];
    if (player->playerNo == 4) {
        player->playerNo = player->no;
    }
    player->charNo = GwPlayerConf[player->playerNo].charNo;
    player->charNoClamp = (player->charNo >= 9) ? 9 : player->charNo;
    player->padNo = (GwPlayerConf[player->playerNo].type != 0) ? -1 : GwPlayerConf[player->playerNo].padNo;
    player->dif = (player->padNo != -1) ? 0 : GwPlayerConf[player->playerNo].dif;
    player->mode = 0;
    player->unk50 = player->unk54 = 0;
    player->unk58 = 0;
    player->unk5C = rand8() % 64;
    obj->mdlId[0] = CharModelCreate(player->charNo, 8);
    Hu3DModelLayerSet(obj->mdlId[0], 1);
    Hu3DModelCameraSet(obj->mdlId[0], HU3D_CAM1);
    Hu3DModelAttrReset(obj->mdlId[0], HU3D_MOTATTR_PAUSE);
    Hu3DModelShadowSet(obj->mdlId[0]);
    player->unk1C = 0;
    player->unk20 = -1;
    for (i = 0; i < 6; i++) {
        obj->mtnId[i] = CharMotionCreate(player->charNo, lbl_1_data_3C[i]);
    }
    Hu3DMotionSet(obj->mdlId[0], obj->mtnId[0]);
    Hu3DModelAttrSet(obj->mdlId[0], HU3D_MOTATTR_LOOP);
    obj->trans.x = lbl_1_data_90[player->no][0];
    obj->trans.y = lbl_1_data_90[player->no][1];
    obj->trans.z = lbl_1_data_90[player->no][2];
    obj->rot.y = lbl_1_data_90[player->no][3];
    obj->scale.x = obj->scale.y = obj->scale.z = 1.0f;
    obj->rot.x = obj->rot.z = 0.0f;
    obj->objFunc = fn_1_1828;
}

void fn_1_119A0(OMOBJ *obj)
{
    lbl_1_bss_2ED2C = 1000;
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_PREVTYPE, 60);
    obj->objFunc = fn_1_EF6C;
}

void ObjectSetup(void)
{
    s32 i;
    OMOBJ *obj;

    OSReport("******* M505 ObjectSetup *********\n");
    lbl_1_bss_2ED28 = omInitObjMan(600, 0x2000);
    omGameSysInit(lbl_1_bss_2ED28);
    Hu3DCameraCreate(HU3D_CAM0 | HU3D_CAM1);
    Hu3DCameraPerspectiveSet(HU3D_CAM0, 20.0f, 20.0f, 25000.0f, 1.2f);
    Hu3DCameraViewportSet(HU3D_CAM0, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(HU3D_CAM0, 0, 0, 640, 480);
    Hu3DCameraPerspectiveSet(HU3D_CAM1, 20.0f, 20.0f, 25000.0f, 1.2f);
    Hu3DCameraViewportSet(HU3D_CAM1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(HU3D_CAM1, 0, 0, 640, 480);
    for (i = 0; i < 2; i++) {
        CZoomM[i] = 4300.0f;
        CenterM[i].x = -50.0f;
        CenterM[i].y = -150.0f;
        CenterM[i].z = 0.0f;
        CRotM[i].x = -63.0f;
        CRotM[i].y = 0.0f;
        CRotM[i].z = 0.0f;
    }
    CZoomM[1] = 4000.0f;
    CenterM[1].x = 1800.0f;
    CenterM[1].y = -150.0f;
    CenterM[1].z = 0.0f;
    CRotM[1].x = -40.0f;
    CRotM[1].y = 0.0f;
    CRotM[1].z = 0.0f;
    {
        Vec lightPos = { 0.0f, 5000.0f, 0.0f };
        Vec lightDir = { 0.0f, -1.0f, -0.5f };
        GXColor lightColor = { 255, 255, 255, 255 };

        lbl_1_bss_2ED1C = Hu3DGLightCreateV(&lightPos, &lightDir, &lightColor);
        Hu3DGLightStaticSet(lbl_1_bss_2ED1C, TRUE);
        Hu3DGLightInfinitytSet(lbl_1_bss_2ED1C);
    }
    {
        Vec shadowPos;
        Vec shadowUp;
        Vec shadowTarget;

        shadowPos.x = 345.0f;
        shadowPos.y = 10000.0 * sin(1.5707963267948966);
        shadowPos.z = 10000.0 * cos(1.5707963267948966);
        shadowUp.x = 0.0f;
        shadowUp.y = 1.0f;
        shadowUp.z = 0.0f;
        shadowTarget.x = 346.0f;
        shadowTarget.y = 0.0f;
        shadowTarget.z = 0.0f;
        Hu3DShadowCreate(12.0f, 7000.0f, 12000.0f);
        Hu3DShadowTPLvlSet(0.5f);
        Hu3DShadowPosSet(&shadowPos, &shadowUp, &shadowTarget);
        Hu3DShadowColSet(60, 90, 150);
    }
    lbl_1_bss_2ED26 = -1;
    lbl_1_data_78 = -1;
    lbl_1_bss_2ED34 = omAddObjEx(lbl_1_bss_2ED28, 101, 26, 8, 1, fn_1_FADC);
    for (i = 0; i < 4; i++) {
        obj = lbl_1_bss_2ED38[i] = omAddObjEx(lbl_1_bss_2ED28, 100, 2, 6, 0, fn_1_11650);
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M505Player), HU_MEMNUM_OVL);
        obj->work[0] = i;
    }
    lbl_1_bss_2ED48 = omAddObjEx(lbl_1_bss_2ED28, 32730, 0, 0, -1, omOutViewMulti);
    lbl_1_bss_2ED48->work[0] = 2;
    lbl_1_bss_2ED4C = omAddObjEx(lbl_1_bss_2ED28, 102, 0, 0, -1, fn_1_119A0);
    lbl_1_bss_2ED30 = omAddObjEx(lbl_1_bss_2ED28, 103, 1, 0, -1, fn_1_12078);
}

void fn_1_12078(OMOBJ *obj)
{
    if (lbl_1_bss_8 != 0) {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_WHITEFADE, 60);
        obj->objFunc = fn_1_120D0;
    }
}

void fn_1_120D0(OMOBJ *obj)
{
    s32 i;
    M505Player *player;

    if (WipeCheckEnd() == 0) {
        for (i = 0; i < 4; i++) {
            player = lbl_1_bss_2ED38[i]->data;
            CharModelKill(player->charNo);
        }
        if (lbl_1_bss_2ED26 >= 0) {
            GameMesKill(lbl_1_bss_2ED26);
        }
        GameMesClose();
        HuAudAllStop();
        omOvlReturnEx(lbl_1_data_8C, 1);
    }
}
