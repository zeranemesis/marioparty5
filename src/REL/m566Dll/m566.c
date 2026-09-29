#include "REL/m566Dll.h"

#include "math.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

int rand8(void);

#define M566_DATA(no) DATANUM(DATA_m566, (no))

typedef struct M566Esprite_s {
    s16 memberNo;
    s16 animNo;
} M566ESPRITE;

typedef struct M566EspAnim_s {
    unsigned int dataNum;
    u16 useCnt;
    ANIMDATA *anim;
} M566ESPANIM;

/* esprite.c keeps these private; the minigame reads the sprite animation directly. */
extern M566ESPRITE esprite[];
extern M566ESPANIM espanim[];
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);

/* Player work (obj->data of the 4 player objects), size = 0x108. */
typedef struct M566Player_s {
    /* 0x00 */ s32 no;
    /* 0x04 */ s32 playerNo;
    /* 0x08 */ s32 charNoClamp;
    /* 0x0C */ s32 charNo;
    /* 0x10 */ s32 padNo;
    /* 0x14 */ s32 comDif;
    /* 0x18 */ s32 score;
    /* 0x1C */ s32 motNo;
    /* 0x20 */ s32 motReq;
    /* 0x24 */ s32 mode;
    /* 0x28 */ s32 modeTime;
    /* 0x2C */ s32 effTime[3];
    /* 0x38 */ s32 unk38;
    /* 0x3C */ float speed;
    /* 0x40 */ float velX;
    /* 0x44 */ float unk44;
    /* 0x48 */ float velZ;
    /* 0x4C */ s32 itemNo;
    /* 0x50 */ s32 itemTime;
    /* 0x54 */ s32 rumbleTime;
    /* 0x58 */ float unk58;
    /* 0x5C */ s32 rumbleF;
    /* 0x60 */ s32 comMode;
    /* 0x64 */ s32 comTime;
    /* 0x68 */ s32 comTimeMax;
    /* 0x6C */ s32 comTarget;
    /* 0x70 */ s32 comSkip[29];
    /* 0xE4 */ s32 comAvoid;
    /* 0xE8 */ float avoidX;
    /* 0xEC */ float avoidZ;
    /* 0xF0 */ float comAngle;
    /* 0xF4 */ s32 nearItem;
    /* 0xF8 */ float comDist;
    /* 0xFC */ s32 comMoveTime;
    /* 0x100 */ s32 comTurnTime;
    /* 0x104 */ s32 comTurnDir;
} M566Player;

/* Sound effect request queue entry (one per sound), size = 0x20. */
typedef struct M566SeReq_s {
    /* 0x00 */ s32 num;
    /* 0x04 */ float posX[3];
    /* 0x10 */ float posZ[3];
    /* 0x1C */ s32 seNo;
} M566SeReq;

/* Particle used by fn_1_9F20 (128 of them), size = 0x20. */
typedef struct M566Particle_s {
    /* 0x00 */ s32 active;
    /* 0x04 */ s32 alpha;
    /* 0x08 */ Vec pos;
    /* 0x14 */ Vec vel;
} M566Particle;

/* Item stand / carried item slot (11 of them), size = 0x30. */
typedef struct M566Group_s {
    /* 0x00 */ s32 mode;
    /* 0x04 */ s32 time;
    /* 0x08 */ s32 owner;
    /* 0x0C */ s32 slot;
    /* 0x10 */ Vec pos;
    /* 0x1C */ float unk1C;
    /* 0x20 */ float unk20;
    /* 0x24 */ float unk24;
    /* 0x28 */ float angle;
    /* 0x2C */ float dist;
} M566Group;

/* Collectible item (29 of them), size = 0x60. */
typedef struct M566Item_s {
    /* 0x00 */ s32 kind;
    /* 0x04 */ s32 no;
    /* 0x08 */ s32 model;
    /* 0x0C */ s32 slot;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 state;
    /* 0x18 */ s32 time;
    /* 0x1C */ s32 owner;
    /* 0x20 */ s32 wobble;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ Vec pos;
    /* 0x34 */ Vec holdPos;
    /* 0x40 */ s32 unk40[3];
    /* 0x4C */ Quaternion quat;
    /* 0x5C */ s32 unk5C;
} M566Item;

void ObjectSetup(void);
void fn_1_518(s32 seNo, float x, float z);
void fn_1_668(OMOBJ *obj);
void fn_1_48F8(OMOBJ *obj);
void fn_1_8F1C(OMOBJ *obj);
void fn_1_9F20(HU3DMODEL *model, Mtx *mtx);
void fn_1_A654(HU3DMODEL *model, Mtx *mtx);
void fn_1_C06C(HU3DMODEL *model, Mtx *mtx);
void fn_1_C708(OMOBJ *obj);
void fn_1_DACC(OMOBJ *obj);
void fn_1_DEAC(OMOBJ *obj);
void fn_1_E4EC(OMOBJ *obj);
void fn_1_E544(OMOBJ *obj);

s32 lbl_1_data_0[7] = {
    M566_DATA(0x1C), M566_DATA(0x1C), M566_DATA(0x1C),
    M566_DATA(0x1D), M566_DATA(0x1D), M566_DATA(0x1D),
    M566_DATA(0x0F)
};
s32 lbl_1_data_1C[4][7] = { 0 };
s32 lbl_1_data_8C[13] = {
    0x00890000, 0x00300000, 0x00B40000, 0x00EA0000, 0x00E30000,
    0x00140000, 0x00DD0000, 0x002A0000, 0x00C60000, 0x00AD0000,
    0, 0, 0
};
s32 lbl_1_data_C0[10] = {
    0x008C0000, 0x008C0001, 0x008C0002, 0x008C000F, 0x008C004B,
    0x00870069, 0x008C0016, 0x008C004F, 0x008C001F, 0x008C0031
};
u8 lbl_1_data_E8[10] = { 0, 0, 0, 1, 0, 1, 0, 0, 0, 1 };
u8 lbl_1_data_F2[10] = { 8, 8, 8, 0, 8, 8, 8, 8, 8, 8 };
s32 lbl_1_data_FC[10] = { 0, 0, 0, 0, 0x1C, 0, 0, 0, 0, 0 };
s32 lbl_1_data_124[4] = { 0 };
s16 lbl_1_data_134 = -1;
s32 lbl_1_data_138[2] = { -1, 0x708 };
s32 lbl_1_data_140 = -1;
s32 lbl_1_data_144 = -1;
s32 lbl_1_data_148 = 1;
s32 lbl_1_data_14C[3] = { 0, 4, 3 };
s32 lbl_1_data_158 = 1;
float lbl_1_data_15C[4][4] = {
    { -100.0f, 0.0f, -100.0f, 0.0f },
    { 100.0f, 0.0f, -100.0f, 0.0f },
    { -100.0f, 0.0f, 100.0f, 0.0f },
    { 100.0f, 0.0f, 100.0f, 0.0f },
};
s32 lbl_1_data_19C[29] = { 0 };
char lbl_1_data_210[] = "ske_head";
s32 lbl_1_data_21C[5] = { 0 };
s32 lbl_1_data_230[5] = { -1, -1, -1, -1, -1 };
s32 lbl_1_data_244[5] = { 0 };
float lbl_1_data_258 = -10000.0f;
char lbl_1_data_25C[] = "metehook_c";

float lbl_1_bss_0[2];
s32 lbl_1_bss_8;
s32 lbl_1_bss_C;
s32 lbl_1_bss_10[29];
s16 lbl_1_bss_84[4];
s32 lbl_1_bss_8C;
s32 lbl_1_bss_90[2];
s32 lbl_1_bss_98[5];
s32 lbl_1_bss_AC;
s32 lbl_1_bss_B0;
s16 lbl_1_bss_B4;
OMOBJMAN *lbl_1_bss_B8;
s32 lbl_1_bss_BC;
OMOBJ *lbl_1_bss_C0;
OMOBJ *lbl_1_bss_C4;
OMOBJ *lbl_1_bss_C8[4];
OMOBJ *lbl_1_bss_D8;
OMOBJ *lbl_1_bss_DC;
M566SeReq lbl_1_bss_E0[41];
M566Particle lbl_1_bss_600[128];
M566Group lbl_1_bss_1600[11];
M566Item lbl_1_bss_1810[29];

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

void fn_1_A0(void)
{
    Vec pos;
    Vec target;
    Vec up;
    float rotX;
    float rotY;
    float rotZ;

    CenterM[1].y += 20.0 * ((HuPadTrigL[0] - HuPadTrigR[0]) / 128.0);
    CZoomM[1] += 25.0 * (((HuPadBtn[0] & PAD_BUTTON_X) != 0) - ((HuPadBtn[0] & PAD_BUTTON_Y) != 0));
    CRotM[1].y += HuPadSubStkX[0] / 32.0;
    CRotM[1].x += HuPadSubStkY[0] / -32.0;
    rotX = CRotM[1].x;
    rotY = CRotM[1].y;
    rotZ = CRotM[1].z;
    pos.x = CenterM[1].x + (CZoomM[1] * (sin((M_PI * rotY) / 180.0) * cos((M_PI * rotX) / 180.0)));
    pos.y = CenterM[1].y + (CZoomM[1] * -sin((M_PI * rotX) / 180.0));
    pos.z = CenterM[1].z + (CZoomM[1] * (cos((M_PI * rotY) / 180.0) * cos((M_PI * rotX) / 180.0)));
    target.x = CenterM[1].x;
    target.y = CenterM[1].y;
    target.z = CenterM[1].z;
    up.x = sin((M_PI * rotY) / 180.0) * sin((M_PI * rotX) / 180.0);
    up.y = cos((M_PI * rotX) / 180.0);
    up.z = cos((M_PI * rotY) / 180.0) * sin((M_PI * rotX) / 180.0);
}

/* Play/adjust an effect: pan and volume from the screen position. */
void fn_1_518(s32 seNo, float x, float z)
{
    s16 vol;

    HuAudFXPanning(seNo, 0.5 + (64.0f + (63.0f * (0.25f * (x / 650.0f)))));
    vol = 0.5 + (104.0f + (23.0f * (1.0f + (z / 550.0f))));
    vol = (vol < 0x68) ? 0x68 : ((vol > 0x7F) ? 0x7F : vol);
    HuAudFXVolSet(seNo, vol);
}

/* Main flow of the minigame: sound queue, start/end messages, winner selection. */
void fn_1_8F1C(OMOBJ *obj)
{
    M566Player *players[4];
    s32 i;
    s32 j;
    s32 rnd;
    s32 seNo;
    s32 vol;
    s32 playerNo;

    for (i = 0; i < 4; i++) {
        players[i] = lbl_1_bss_C8[i]->data;
    }
    for (i = 0; i < 41; i++) {
        s32 seTbl[41] = { 0x8E1, 0x8E2, 0x8E4, 0x8E6, 0x2D8, 0x8E8, 0x8E9, 0x2C4, 0x2D7 };

        if (lbl_1_bss_E0[i].num > 3) {
            lbl_1_bss_E0[i].num = 3;
        }
        if (lbl_1_bss_E0[i].num > 0) {
            if (i >= 9 || seTbl[i] != -1) {
                seNo = HuAudFXPlay(seTbl[(i >= 9) ? 2 : i]);
                lbl_1_bss_E0[i].seNo = seNo;
                HuAudFXPanning(seNo, 0.5 + (64.0f + (63.0f * (0.25f * (lbl_1_bss_E0[i].posX[lbl_1_bss_E0[i].num - 1] / 650.0f)))));
                vol = 0.5 + (104.0f + (23.0f * (1.0f + (lbl_1_bss_E0[i].posZ[lbl_1_bss_E0[i].num - 1] / 550.0f))));
                vol = (vol < 0) ? 0 : ((vol >= 127) ? 127 : vol);
                HuAudFXVolSet(seNo, vol);
            }
            lbl_1_bss_E0[i].num--;
        }
    }
    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_BC == 1005 && lbl_1_data_138[0] != 4) {
            CharModelVoicePanSet(players[i]->charNo, 64);
            CharModelVoiceVolSet(players[i]->charNo, 127);
        }
        else {
            CharModelVoicePanSet(players[i]->charNo, 0.5 + (64.0f + (63.0f * (0.25f * (lbl_1_bss_C8[i]->trans.x / 650.0f)))));
            vol = 0.5 + (104.0f + (23.0f * (1.0f + (lbl_1_bss_C8[i]->trans.z / 550.0f))));
            vol = (vol < 0) ? 0 : ((vol >= 127) ? 127 : vol);
            CharModelVoiceVolSet(players[i]->charNo, vol);
        }
    }
    if (omSysExitReq != 0 && WipeCheckEnd() == 0 && lbl_1_bss_8 == 0) {
        HuAudFadeOut(1000);
        lbl_1_data_148 = 1;
        lbl_1_bss_8 = 1;
    }
    switch (lbl_1_bss_BC) {
        case 1000:
            lbl_1_bss_0[0] += 1.0f;
            if (lbl_1_bss_0[0] == 5.0f) {
                HuAudFXPlay(0x8EA);
            }
            if (WipeCheckEnd() == 0) {
                lbl_1_bss_BC++;
                lbl_1_bss_0[0] = 0.0f;
            }
            break;
        case 1001:
            lbl_1_bss_0[0] += 1.0f;
            if (lbl_1_bss_0[0] >= 180.0f) {
                lbl_1_bss_BC++;
                lbl_1_bss_0[0] = 0.0f;
            }
            break;
        case 1002:
            lbl_1_bss_0[0] += 1.0f;
            if (lbl_1_bss_0[0] == 5.0f && lbl_1_bss_B4 < 0) {
                lbl_1_bss_B4 = GameMesMgStartCreate();
            }
            if (lbl_1_bss_0[0] >= 5.0f && lbl_1_data_140 == -1) {
                lbl_1_data_140 = HuAudSStreamPlay(0x34);
            }
            if (lbl_1_bss_0[0] >= 5.0f && GameMesStatGet(lbl_1_bss_B4) == 0 && lbl_1_bss_B4 >= 0) {
                lbl_1_bss_BC++;
                lbl_1_bss_B0 = 3600;
                lbl_1_bss_B4 = -1;
                lbl_1_bss_0[0] = 0.0f;
                for (i = 0; i < 4; i++) {
                    players[i]->mode = 2;
                    for (j = 0; j < 7; j++) {
                        if (j < 3 || j > 5) {
                            espDispOn(lbl_1_data_1C[i][j]);
                        }
                    }
                    MgScoreWinDispSet(lbl_1_bss_84[i], TRUE);
                }
            }
            break;
        case 1003:
            lbl_1_bss_0[0] += 1.0f;
            if (lbl_1_data_134 == -1 && lbl_1_bss_B0 == 3600) {
                lbl_1_data_134 = GameMesTimerCreate(lbl_1_bss_B0 / 60);
                GameMesPosSet(lbl_1_data_134, 288.0f, 64.0f);
            }
            if (lbl_1_data_134 != -1 && lbl_1_bss_B0 <= 3600) {
                GameMesTimerValueSet(lbl_1_data_134, (lbl_1_bss_B0 + 58) / 60);
            }
            lbl_1_bss_B0--;
            i = 0;
            rnd = rand8() % 4;
            for (i = 0; i < 4; i++) {
                if (players[i]->score >= 3) {
                    if (lbl_1_data_138[0] == -1) {
                        lbl_1_data_138[0] = i;
                    }
                    else if (players[i]->score > lbl_1_data_138[0]) {
                        lbl_1_data_138[0] = i;
                    }
                }
            }
            if (lbl_1_bss_B0 > 0 && lbl_1_data_138[0] == -1) {
                break;
            }
            lbl_1_bss_BC = 1004;
            lbl_1_bss_0[0] = 0.0f;
            lbl_1_data_138[0] = (lbl_1_data_138[0] != -1) ? lbl_1_data_138[0] : 4;
            break;
        case 1004:
            if (lbl_1_bss_B4 < 0 && lbl_1_bss_0[0] == 0.0f) {
                GameMesTimerEnd(lbl_1_data_134);
                lbl_1_bss_B4 = GameMesMgFinishCreate();
                if (lbl_1_data_140 != -1) {
                    HuAudSStreamFadeOut(lbl_1_data_140, 100);
                }
            }
            else if (GameMesStatGet(lbl_1_bss_B4) == 0 && lbl_1_bss_0[0] >= 180.0f) {
                if (lbl_1_data_138[0] == 4 || (players[lbl_1_data_138[0]]->mode == 7 && players[lbl_1_data_138[0]]->modeTime >= 60)) {
                    lbl_1_bss_BC++;
                    lbl_1_bss_0[0] = -1.0f;
                    if (lbl_1_data_138[0] != 4) {
                        playerNo = players[lbl_1_data_138[0]]->playerNo;
                        if (!_CheckFlag(FLAG_MG_PRACTICE)) {
                            GwPlayer[playerNo].mgCoinBonus = 10;
                        }
                        lbl_1_bss_B4 = GameMesMgWinnerCreate(players[lbl_1_data_138[0]]->charNo, -1, -1, -1);
                        seNo = HuAudSStreamPlay(0x79);
                        lbl_1_data_144 = HuAudFXPlay(0x8E9);
                        CharFXPlay(players[lbl_1_data_138[0]]->charNo, 0x1F8);
                    }
                    else {
                        lbl_1_bss_B4 = GameMesMgDrawCreate();
                        HuAudSStreamPlay(0x7B);
                        for (i = 0; i < 4; i++) {
                            CharFXPlay(players[i]->charNo, 0x200);
                        }
                    }
                }
            }
            for (i = 0; i < 4; i++) {
                s32 prevMode = players[i]->mode;

                players[i]->mode = (players[i]->mode == 2) ? ((lbl_1_data_138[0] == i) ? 7 : 5) : players[i]->mode;
                players[i]->modeTime = (prevMode != players[i]->mode) ? 0 : players[i]->modeTime;
            }
            lbl_1_bss_0[0] += 1.0f;
            break;
        case 1005:
            lbl_1_bss_0[0] += 1.0f;
            for (i = 0; i < 4; i++) {
                s32 prevMode = players[i]->mode;

                players[i]->mode = (players[i]->mode == 5) ? ((lbl_1_data_138[0] == i) ? 7 : 6) : players[i]->mode;
                players[i]->modeTime = (prevMode != players[i]->mode) ? 0 : players[i]->modeTime;
            }
            if (210.0f < lbl_1_bss_0[0] && lbl_1_bss_8 == 0) {
                if (lbl_1_data_138[0] != 4) {
                    HuAudFXFadeOut(lbl_1_data_144, 1000);
                }
                lbl_1_bss_B4 = -1;
                lbl_1_bss_8 = 1;
            }
            break;
        default:
            OSReport("*** main mode error(%d)!!\n", lbl_1_bss_BC);
            break;
    }
}

/* Draws the puffs of dust behind the players (particles of lbl_1_bss_600). */
void fn_1_9F20(HU3DMODEL *model, Mtx *mtx)
{
    Mtx lookAt;
    Mtx lookAtT;
    Vec in[4] = {
        { -40.0f, 40.0f, 0.0f },
        { 40.0f, 40.0f, 0.0f },
        { 40.0f, -40.0f, 0.0f },
        { -40.0f, -40.0f, 0.0f },
    };
    Vec out[4];
    HU3DCAMERA *camera;
    s32 i;

    camera = &Hu3DCamera[1];
    MTXLookAt(lookAt, &camera->pos, &camera->up, &camera->target);
    MTXTranspose(lookAt, lookAtT);
    MTXMultVecArray(lookAtT, in, out, 4);
    GXLoadPosMtxImm(*mtx, GX_PNMTX0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZCompLoc(GX_FALSE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    HuSprTexLoad(espanim[esprite[lbl_1_bss_98[0]].animNo].anim, 0, GX_TEXMAP0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    for (i = 0; i < 128; i++) {
        if (lbl_1_bss_600[i].active != 0) {
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(out[0].x + lbl_1_bss_600[i].pos.x, out[0].y + lbl_1_bss_600[i].pos.y, out[0].z + lbl_1_bss_600[i].pos.z);
            GXColor4u8(240, 240, 240, (u8)lbl_1_bss_600[i].alpha);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(out[1].x + lbl_1_bss_600[i].pos.x, out[1].y + lbl_1_bss_600[i].pos.y, out[1].z + lbl_1_bss_600[i].pos.z);
            GXColor4u8(240, 240, 240, (u8)lbl_1_bss_600[i].alpha);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(out[2].x + lbl_1_bss_600[i].pos.x, out[2].y + lbl_1_bss_600[i].pos.y, out[2].z + lbl_1_bss_600[i].pos.z);
            GXColor4u8(240, 240, 240, (u8)lbl_1_bss_600[i].alpha);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(out[3].x + lbl_1_bss_600[i].pos.x, out[3].y + lbl_1_bss_600[i].pos.y, out[3].z + lbl_1_bss_600[i].pos.z);
            GXColor4u8(192, 192, 192, (u8)lbl_1_bss_600[i].alpha);
            GXTexCoord2f32(0.0f, 1.0f);
        }
    }
    GXPixModeSync();
}

/* Draws the shaking icon above the players (used when the pad has no rumble). */
void fn_1_A654(HU3DMODEL *model, Mtx *mtx)
{
    M566Player *players[4];
    Mtx lookAt;
    Mtx lookAtT;
    Vec in[4] = {
        { -45.0f, 54.0f, 0.0f },
        { 45.0f, 54.0f, 0.0f },
        { 45.0f, -54.0f, 0.0f },
        { -45.0f, -54.0f, 0.0f },
    };
    Vec out[4];
    HU3DCAMERA *camera;
    s32 pass;
    s32 i;
    s32 j;
    s32 k;
    s32 kind;
    float x;
    float y;
    float z;

    for (i = 0; i < 4; i++) {
        players[i] = lbl_1_bss_C8[i]->data;
    }
    camera = &Hu3DCamera[1];
    MTXLookAt(lookAt, &camera->pos, &camera->up, &camera->target);
    MTXTranspose(lookAt, lookAtT);
    MTXMultVecArray(lookAtT, in, out, 4);
    GXLoadPosMtxImm(*mtx, GX_PNMTX0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetAlphaCompare(GX_GEQUAL, 127, GX_AOP_AND, GX_GEQUAL, 127);
    GXSetZCompLoc(GX_FALSE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    for (pass = 0; pass < 2; pass++) {
        HuSprTexLoad(espanim[esprite[lbl_1_bss_90[pass]].animNo].anim, 0, GX_TEXMAP0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
        for (i = 0; i < 4; i++) {
            s32 xOfs[4] = { 0, -7, 0, 7 };
            float scale = 1.0f;
            float uv[4][2] = {
                { 0.0f, 0.0f },
                { 1.0f, 0.0f },
                { 1.0f, 1.0f },
                { 0.0f, 1.0f },
            };
            s32 yOfs[10] = { 0, 0, 0, 0, 0, 0, 30, 0, 0, 0 };
            s32 zOfs[10] = { 0, 0, 0, 0, 0, 0, 30, 0, 0, 0 };

            if (lbl_1_data_158 != 0) {
                continue;
            }
            if (pass == 0 && players[i]->itemTime == 0) {
                continue;
            }
            if (pass == 1 && players[i]->itemTime == 0) {
                continue;
            }
            if (pass == 1) {
                kind = (lbl_1_bss_1810[players[i]->itemNo].kind == 1) ? lbl_1_data_14C[0]
                    : ((lbl_1_bss_1810[players[i]->itemNo].kind == 2) ? lbl_1_data_14C[1] : lbl_1_data_14C[2]);
                switch (kind) {
                    case 0:
                        scale = (players[i]->itemTime % 90 < 56)
                            ? ((players[i]->itemTime % 90 % 12 < 8)
                                ? (0.8 + 0.6 * ((players[i]->itemTime % 90 % 12) / 8.0f))
                                : (0.8 + 0.6 * ((12 - players[i]->itemTime % 90 % 12) / 4.0f)))
                            : (0.8 + 0.6 * ((90 - players[i]->itemTime % 90) / 34.0f));
                        break;
                    case 1:
                        scale = (players[i]->itemTime % 90 < 27)
                            ? (0.6 + 0.5 * (players[i]->itemTime % 90 / 27.0f)
                                + 0.1 * ((players[i]->itemTime % 4 == 3) ? 1 : players[i]->itemTime % 4))
                            : ((players[i]->itemTime % 90 < 33)
                                ? (1.1 - 0.4 * ((players[i]->itemTime % 90 - 27) / 6.0f))
                                : ((players[i]->itemTime % 90 < 60)
                                    ? (0.7 + 0.5 * ((players[i]->itemTime % 90 - 33) / 27.0f)
                                        + 0.1 * ((players[i]->itemTime % 4 == 3) ? 1 : players[i]->itemTime % 4))
                                    : ((players[i]->itemTime % 90 < 90)
                                        ? (1.2 - 0.6 * ((players[i]->itemTime % 90 - 60) / 30.0f))
                                        : 0.0)));
                        break;
                    case 2:
                        scale = (players[i]->itemTime % 90 < 7)
                            ? (0.7 + 0.6 * (players[i]->itemTime % 90 / 7.0f))
                            : ((players[i]->itemTime % 90 < 20)
                                ? (1.3 - 0.6 * ((players[i]->itemTime % 90 - 7) / 13.0f))
                                : ((players[i]->itemTime % 90 < 27)
                                    ? (0.7 + 0.6 * ((players[i]->itemTime % 90 - 20) / 7.0f))
                                    : ((players[i]->itemTime % 90 < 40)
                                        ? (1.3 - 0.6 * ((players[i]->itemTime % 90 - 27) / 13.0f))
                                        : ((players[i]->itemTime % 90 < 80)
                                            ? (0.7 + 0.7 * ((players[i]->itemTime % 90 - 40) / 40.0f)
                                                + 0.1 * ((players[i]->itemTime % 4 == 3) ? 1 : players[i]->itemTime % 4))
                                            : ((players[i]->itemTime % 90 < 90)
                                                ? (1.4 - 0.7 * ((players[i]->itemTime % 90 - 80) / 10.0f))
                                                : 0.0)))));
                        break;
                    case 3:
                        scale = (players[i]->itemTime % 30 < 20)
                            ? (1.3 + 0.3 * ((players[i]->itemTime % 4 == 3) ? 1 : players[i]->itemTime % 4))
                            : 1.3;
                        break;
                    case 4:
                        scale = (players[i]->itemTime % 30 < 8)
                            ? (0.6 + 0.4 * ((players[i]->itemTime % 30) / 8.0f))
                            : (0.6 + 0.4 * ((30 - players[i]->itemTime % 30) / 22.0f));
                        break;
                }
            }
            for (j = 0; j < 4; j++) {
                for (k = 0; k < 2; k++) {
                    uv[j][k] = 0.5f + ((uv[j][k] - 0.5f) / scale);
                }
            }
            x = lbl_1_bss_C8[i]->trans.x;
            y = yOfs[players[i]->charNoClamp] + (22.0f + (lbl_1_bss_C8[i]->trans.y + CharModelHeightGet(players[i]->charNo)));
            z = 20.0f + lbl_1_bss_C8[i]->trans.z + zOfs[players[i]->charNoClamp];
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(x + out[0].x * scale, y + out[0].y * scale, z + out[0].z * scale + pass * 2);
            GXColor4u8(255, 255, 255, 255);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(x + out[1].x * scale, y + out[1].y * scale, z + out[1].z * scale + pass * 2);
            GXColor4u8(255, 255, 255, 255);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(x + out[2].x * scale, y + out[2].y * scale, z + out[2].z * scale + pass * 2);
            GXColor4u8(255, 255, 255, 255);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(x + out[3].x * scale, y + out[3].y * scale, z + out[3].z * scale + pass * 2);
            GXColor4u8(255, 255, 255, 255);
            GXTexCoord2f32(0.0f, 1.0f);
        }
        GXPixModeSync();
    }
}

/* Draws the shadow of each item lying on the ground. */
void fn_1_C06C(HU3DMODEL *model, Mtx *mtx)
{
    s32 i;

    GXLoadPosMtxImm(*mtx, GX_PNMTX0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZCompLoc(GX_FALSE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    HuSprTexLoad(espanim[esprite[lbl_1_bss_8C].animNo].anim, 0, GX_TEXMAP0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    for (i = 0; i < 29; i++) {
        if (lbl_1_bss_1810[i].kind != 0 && (lbl_1_bss_1810[i].state == 0 || lbl_1_bss_1810[i].state == 4)) {
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(1.0f + (lbl_1_bss_1810[i].pos.x - 66.0f), 1.0f, 4.0f + (lbl_1_bss_1810[i].pos.z - 66.0f));
            GXColor4u8(0, 0, 0, 127);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(1.0f + (66.0f + lbl_1_bss_1810[i].pos.x), 1.0f, 4.0f + (lbl_1_bss_1810[i].pos.z - 66.0f));
            GXColor4u8(0, 0, 0, 127);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(1.0f + (66.0f + lbl_1_bss_1810[i].pos.x), 1.0f, 4.0f + (66.0f + lbl_1_bss_1810[i].pos.z));
            GXColor4u8(0, 0, 0, 127);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(1.0f + (lbl_1_bss_1810[i].pos.x - 66.0f), 1.0f, 4.0f + (66.0f + lbl_1_bss_1810[i].pos.z));
            GXColor4u8(0, 0, 0, 127);
            GXTexCoord2f32(0.0f, 1.0f);
        }
    }
    GXPixModeSync();
}

/* Creates the stage models, hooks, items, sprites and the score windows. */
void fn_1_C708(OMOBJ *obj)
{
    M566Player *players[4];
    s32 i;
    s32 mdlNo;
    s32 j;
    s32 itemCnt;

    for (i = 0; i < 4; i++) {
        players[i] = lbl_1_bss_C8[i]->data;
    }
    obj->mdlId[10] = Hu3DHookFuncCreate(fn_1_9F20);
    Hu3DModelLayerSet(obj->mdlId[10], 4);
    Hu3DModelCameraSet(obj->mdlId[10], HU3D_CAM1);
    obj->mdlId[12] = Hu3DHookFuncCreate(fn_1_A654);
    Hu3DModelLayerSet(obj->mdlId[12], 2);
    Hu3DModelCameraSet(obj->mdlId[12], HU3D_CAM1);
    obj->mdlId[14] = Hu3DHookFuncCreate(fn_1_C06C);
    Hu3DModelLayerSet(obj->mdlId[14], 0);
    Hu3DModelCameraSet(obj->mdlId[14], HU3D_CAM1);
    for (i = 0; i < 5; i++) {
        s32 dataNum[5] = { M566_DATA(0), M566_DATA(1), M566_DATA(2), M566_DATA(3), M566_DATA(4) };

        mdlNo = i;
        obj->mdlId[mdlNo] = Hu3DModelCreateData(dataNum[i]);
        Hu3DModelLayerSet(obj->mdlId[mdlNo], (i <= 1) ? 0 : 3);
        Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM1);
        Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_MOTATTR_LOOP);
        if (i == 0) {
            Hu3DModelShadowMapSet(obj->mdlId[mdlNo]);
        }
    }
    for (i = 0; i < 16; i++) {
        mdlNo = i + 13;
        if (i == 0) {
            obj->mdlId[mdlNo] = Hu3DModelCreateData(M566_DATA(5));
        }
        else {
            obj->mdlId[mdlNo] = Hu3DModelLink(obj->mdlId[13]);
        }
        Hu3DModelLayerSet(obj->mdlId[mdlNo], 0);
        Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM1);
        Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_ATTR_DISPOFF);
        Hu3DModelShadowSet(obj->mdlId[mdlNo]);
    }
    for (i = 0; i < 4; i++) {
        mdlNo = i + 9;
        if (i == 0) {
            obj->mdlId[mdlNo] = Hu3DModelCreateData(M566_DATA(14));
        }
        else {
            obj->mdlId[mdlNo] = Hu3DModelLink(obj->mdlId[9]);
        }
        Hu3DModelLayerSet(obj->mdlId[mdlNo], 2);
        Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM1);
        Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_ATTR_DISPOFF);
    }
    for (i = 0; i < 6; i++) {
        s32 mdlNum[6] = { 11, 11, 8, 4, 4, 4 };
        s32 mdlBase[6] = { 58, 69, 80, 88, 92, 96 };
        s32 motNo[3] = { 0, 1, 2 };
        s32 mdlData[6] = { M566_DATA(0x21), M566_DATA(0x23), M566_DATA(0x27), M566_DATA(0x26), M566_DATA(0x25), M566_DATA(0x0D) };
        s32 motData[3] = { M566_DATA(0x22), M566_DATA(0x24), M566_DATA(0x28) };

        for (j = 0; j < mdlNum[i]; j++) {
            mdlNo = j + mdlBase[i];
            if (j == 0 || i < 3) {
                obj->mdlId[mdlNo] = Hu3DModelCreateData(mdlData[i]);
            }
            else {
                obj->mdlId[mdlNo] = Hu3DModelLink(obj->mdlId[mdlBase[i]]);
            }
            if (j == 0 && i < 3) {
                obj->mtnId[motNo[i]] = Hu3DJointMotionData(obj->mdlId[mdlNo], motData[i]);
            }
            if (i < 3) {
                Hu3DMotionSet(obj->mdlId[mdlNo], obj->mtnId[motNo[i]]);
                Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_MOTATTR_LOOP);
                if (i != 0 || j != 0) {
                    Hu3DModelShadowSet(obj->mdlId[mdlNo]);
                }
            }
            Hu3DMotionTimeSet(obj->mdlId[mdlNo], 0.0f);
            Hu3DModelLayerSet(obj->mdlId[mdlNo], (i < 3) ? 1 : 3);
            Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM1);
            Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_ATTR_DISPOFF);
            if (i == 0) {
                Hu3DModelScaleSet(obj->mdlId[mdlNo], 1.6f, 1.6f, 1.6f);
            }
            else if (i == 1 || i == 3) {
                Hu3DModelScaleSet(obj->mdlId[mdlNo], 0.75f, 0.75f, 0.75f);
            }
            Hu3DModelPosSet(obj->mdlId[mdlNo], 0.0f, 14000.0f, 0.0f);
        }
    }
    obj->mdlId[16] = Hu3DModelLink(obj->mdlId[58]);
    Hu3DModelShadowSet(obj->mdlId[16]);
    Hu3DModelLayerSet(obj->mdlId[16], 1);
    Hu3DModelCameraSet(obj->mdlId[16], HU3D_CAM1);
    Hu3DModelAttrSet(obj->mdlId[16], HU3D_ATTR_DISPOFF);
    memset(lbl_1_bss_600, 0, sizeof(lbl_1_bss_600));
    memset(lbl_1_bss_1600, 0, sizeof(lbl_1_bss_1600));
    memset(lbl_1_bss_1810, 0, sizeof(lbl_1_bss_1810));
    itemCnt = 0;
    {
        s32 kindCnt[7] = { 0, 0, 0, 0, 0, 0, 0 };
        s32 kindMdl[7] = { -1, -1, -1, -1, -1, -1, -1 };
        s32 kindData[7] = { M566_DATA(6), M566_DATA(7), M566_DATA(8), M566_DATA(9), M566_DATA(10), M566_DATA(11), M566_DATA(12) };
        s32 itemKind[29] = { 1, 1, 2, 3, 2, 1, 1, 2, 3, 2, 1, 1, 2, 3, 2, 1, 1, 2, 3, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 };
        s32 itemUsed[29] = { 0 };
        s32 kindNo[3] = { 0, 0, 0 };

        for (i = 0; i < 29; i++) {
            do {
                j = rand8() % 7;
            } while (itemCnt / 7 != kindCnt[j]);
            mdlNo = i + 29;
            if (kindMdl[j] == -1) {
                obj->mdlId[mdlNo] = Hu3DModelCreateData(kindData[j]);
                kindMdl[j] = obj->mdlId[mdlNo];
            }
            else {
                obj->mdlId[mdlNo] = Hu3DModelLink(kindMdl[j]);
            }
            lbl_1_bss_1810[i].model = j;
            Hu3DModelLayerSet(obj->mdlId[mdlNo], 1);
            Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM1);
            Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_MOTATTR_LOOP);
            Hu3DModelShadowSet(obj->mdlId[mdlNo]);
            itemCnt++;
            kindCnt[j]++;
            if (i == 0) {
                lbl_1_bss_1810[i].pos.z = 0.0f;
                lbl_1_bss_1810[i].pos.x = 0.0f;
            }
            else if (i <= 11) {
                lbl_1_bss_1810[i].pos.x = 355.0 * sin(M_PI * (360.0f * (i - 1) / 11.0f) / 180.0);
                lbl_1_bss_1810[i].pos.z = 355.0 * cos(M_PI * (360.0f * (i - 1) / 11.0f) / 180.0);
            }
            else if (i <= 28) {
                lbl_1_bss_1810[i].pos.x = 555.0 * sin(M_PI * (180.0f + 360.0f * (i - 12) / 17.0f) / 180.0);
                lbl_1_bss_1810[i].pos.z = 555.0 * cos(M_PI * (180.0f + 360.0f * (i - 12) / 17.0f) / 180.0);
            }
            Hu3DModelPosSet(obj->mdlId[mdlNo], lbl_1_bss_1810[i].pos.x, lbl_1_bss_1810[i].pos.y, lbl_1_bss_1810[i].pos.z);
            lbl_1_bss_1810[i].state = 0;
        }
        for (i = 0; i < 29; i++) {
            if (i == 0) {
                j = 0;
            }
            else {
                do {
                    j = rand8() % 29;
                } while (itemUsed[j] != 0);
            }
            itemUsed[j] = 1;
            lbl_1_bss_1810[i * 4 % 29].kind = itemKind[j];
            lbl_1_bss_1810[i * 4 % 29].unk10 = 0;
            lbl_1_bss_1810[i * 4 % 29].no = kindNo[itemKind[j] - 1];
            kindNo[itemKind[j] - 1]++;
        }
    }
    for (j = 0; j < 4; j++) {
        s32 scoreX[4] = { 70, 190, 388, 508 };

        for (i = 0; i < 7; i++) {
            s32 espOfs[7] = { -8, 14, 36, -36, 0, 0, 0 };

            lbl_1_data_1C[j][i] = espEntry(lbl_1_data_0[i] + ((i == 6) ? players[j]->charNo : 0), 0, 0);
            espDispOff(lbl_1_data_1C[j][i]);
            espAttrSet(lbl_1_data_1C[j][i], HUSPR_ATTR_NOANIM);
            espBankSet(lbl_1_data_1C[j][i], 0);
            espPosSet(lbl_1_data_1C[j][i], scoreX[j] + espOfs[(i == 6) ? 3 : i % 3] - 2, 56.0f);
        }
        lbl_1_bss_84[j] = MgScoreWinCreate(112, 36);
        MgScoreWinPosSet(lbl_1_bss_84[j], scoreX[j], 56.0f);
        MgScoreWinDispSet(lbl_1_bss_84[j], FALSE);
    }
    lbl_1_bss_98[0] = espEntry(0x210002, 0, 0);
    espDispOff(lbl_1_bss_98[0]);
    lbl_1_bss_90[0] = espEntry(M566_DATA(0x1E), 0, 0);
    espDispOff(lbl_1_bss_90[0]);
    lbl_1_bss_90[1] = espEntry(M566_DATA(0x1F), 0, 0);
    espDispOff(lbl_1_bss_90[1]);
    lbl_1_bss_8C = espEntry(M566_DATA(0x20), 0, 0);
    espDispOff(lbl_1_bss_8C);
    lbl_1_data_14C[0] = rand8() % 4;
    do {
        lbl_1_data_14C[1] = rand8() % 4;
    } while (lbl_1_data_14C[0] == lbl_1_data_14C[1]);
    do {
        lbl_1_data_14C[2] = rand8() % 4;
    } while (lbl_1_data_14C[0] == lbl_1_data_14C[2] || lbl_1_data_14C[1] == lbl_1_data_14C[2]);
    lbl_1_data_14C[0] = (lbl_1_data_14C[0] == 3) ? 4 : lbl_1_data_14C[0];
    lbl_1_data_14C[1] = (lbl_1_data_14C[1] == 3) ? 4 : lbl_1_data_14C[1];
    lbl_1_data_14C[2] = (lbl_1_data_14C[2] == 3) ? 4 : lbl_1_data_14C[2];
    obj->objFunc = fn_1_48F8;
}

/* Player object creation: picks the character, pad and starting position. */
void fn_1_DACC(OMOBJ *obj)
{
    s32 order[4] = { 0, 1, 2, 3 };
    M566Player *player;
    s32 i;
    s32 j;
    s32 count;

    count = 0;
    player = obj->data;
    memset(player, 0, sizeof(M566Player));
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (GwPlayerConf[j].grp == i) {
                order[count] = j;
                count++;
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
    {
        s32 rumbleMask[4] = { 0x80000000, 0x40000000, 0x20000000, 0x10000000 };

        player->rumbleF = GWRumbleFGet() ? ((player->padNo == -1) ? 1 : (rumbleMask[player->padNo] & HuPadRumbleGet())) : 0;
    }
    if (player->rumbleF == 0) {
        lbl_1_data_158 = 0;
    }
    player->comDif = (player->padNo != -1) ? 0 : GwPlayerConf[player->playerNo].dif;
    player->mode = 2;
    player->itemNo = -1;
    obj->mdlId[0] = CharModelCreate(player->charNo, 4);
    Hu3DModelLayerSet(obj->mdlId[0], 1);
    Hu3DModelCameraSet(obj->mdlId[0], HU3D_CAM1);
    CharEffectLayerSet(3);
    Hu3DModelAttrReset(obj->mdlId[0], HU3D_MOTATTR_PAUSE);
    Hu3DModelShadowSet(obj->mdlId[0]);
    player->motNo = 0;
    player->motReq = -1;
    for (i = 0; i < 10; i++) {
        obj->mtnId[i] = CharMotionCreate(player->charNo, lbl_1_data_C0[i]);
    }
    Hu3DMotionSet(obj->mdlId[0], obj->mtnId[0]);
    Hu3DModelAttrSet(obj->mdlId[0], HU3D_MOTATTR_LOOP);
    CharMotionVoiceOnSet(player->charNo, obj->mtnId[9], FALSE);
    obj->trans.x = lbl_1_data_15C[player->no][0];
    obj->trans.y = lbl_1_data_15C[player->no][1];
    obj->trans.z = lbl_1_data_15C[player->no][2];
    obj->rot.y = lbl_1_data_15C[player->no][3];
    obj->scale.x = obj->scale.y = obj->scale.z = 1.0f;
    obj->rot.x = obj->rot.z = 0.0f;
    obj->objFunc = fn_1_668;
}

void fn_1_DEAC(OMOBJ *obj)
{
    lbl_1_bss_BC = 1000;
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_PREVTYPE, 60);
    obj->objFunc = fn_1_8F1C;
}

void ObjectSetup(void)
{
    Vec lightPos = { 0.0f, 5000.0f, 0.0f };
    Vec lightDir = { 0.0f, -1.0f, -0.5f };
    Vec shadowPos;
    Vec shadowUp;
    Vec shadowTarget;
    GXColor lightColor = { 255, 255, 255, 255 };
    s32 i;
    OMOBJ *obj;

    OSReport("******* M566 ObjectSetup *********\n");
    lbl_1_bss_B8 = omInitObjMan(600, 0x2000);
    omGameSysInit(lbl_1_bss_B8);
    Hu3DCameraCreate(HU3D_CAM0 | HU3D_CAM1);
    Hu3DCameraPerspectiveSet(HU3D_CAM0, 30.0f, 20.0f, 25000.0f, 1.2f);
    Hu3DCameraViewportSet(HU3D_CAM0, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(HU3D_CAM0, 0, 0, 640, 480);
    Hu3DCameraPerspectiveSet(HU3D_CAM1, 30.0f, 20.0f, 25000.0f, 1.2f);
    Hu3DCameraViewportSet(HU3D_CAM1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(HU3D_CAM1, 0, 0, 640, 480);
    CZoomM[0] = 2472.0f;
    CenterM[0].x = 0.0f;
    CenterM[0].y = 84.0f;
    CenterM[0].z = 0.0f;
    CRotM[0].x = -32.6f;
    CRotM[0].y = 0.0f;
    CRotM[0].z = 0.0f;
    CZoomM[1] = 1000.0f;
    CenterM[1].x = 0.0f;
    CenterM[1].y = 30.0f;
    CenterM[1].z = 0.0f;
    CRotM[1].x = -32.6f;
    CRotM[1].y = 0.0f;
    CRotM[1].z = 0.0f;
    Hu3DLighInit();
    lbl_1_bss_AC = Hu3DGLightCreateV(&lightPos, &lightDir, &lightColor);
    Hu3DGLightStaticSet(lbl_1_bss_AC, TRUE);
    Hu3DGLightInfinitytSet(lbl_1_bss_AC);
    shadowPos.x = 0.0f;
    shadowPos.y = 10000.0f;
    shadowPos.z = 0.0f;
    shadowUp.x = 0.0f;
    shadowUp.y = 1.0f;
    shadowUp.z = 0.0f;
    shadowTarget.x = 0.0f;
    shadowTarget.y = 0.0f;
    shadowTarget.z = -0.1f;
    Hu3DShadowCreate(8.5f, 5000.0f, 13000.0f);
    Hu3DShadowPosSet(&shadowPos, &shadowUp, &shadowTarget);
    lbl_1_bss_B4 = -1;
    lbl_1_data_138[0] = -1;
    lbl_1_bss_C4 = omAddObjEx(lbl_1_bss_B8, 101, 100, 3, 1, fn_1_C708);
    for (i = 0; i < 4; i++) {
        obj = lbl_1_bss_C8[i] = omAddObjEx(lbl_1_bss_B8, 100, 1, 10, 0, fn_1_DACC);
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M566Player), HU_MEMNUM_OVL);
        obj->work[0] = i;
    }
    lbl_1_bss_D8 = omAddObjEx(lbl_1_bss_B8, 32730, 0, 0, -1, omOutViewMulti);
    lbl_1_bss_D8->work[0] = 2;
    lbl_1_bss_DC = omAddObjEx(lbl_1_bss_B8, 102, 0, 0, -1, fn_1_DEAC);
    lbl_1_bss_C0 = omAddObjEx(lbl_1_bss_B8, 103, 1, 0, -1, fn_1_E4EC);
}

void fn_1_E4EC(OMOBJ *obj)
{
    if (lbl_1_bss_8 != 0) {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_WHITEFADE, 60);
        obj->objFunc = fn_1_E544;
    }
}

void fn_1_E544(OMOBJ *obj)
{
    s32 i;
    M566Player *player;

    if (WipeCheckEnd() == 0) {
        for (i = 0; i < 4; i++) {
            player = lbl_1_bss_C8[i]->data;
            CharModelKill(player->charNo);
        }
        if (lbl_1_bss_B4 >= 0) {
            GameMesKill(lbl_1_bss_B4);
        }
        GameMesClose();
        HuAudAllStop();
        omOvlReturnEx(lbl_1_data_148, 1);
    }
}
