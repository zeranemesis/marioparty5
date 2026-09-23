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
s32 lbl_1_data_F0[6] = { 0, 0, 0, 0, 96, 0x3F800000 };

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
void *lbl_1_bss_11AC[4][4];
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
