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
char lbl_1_data_25C[] = "itemhook_c";
char lbl_1_data_284[] = "*** main mode error(%d)!!\n";
char lbl_1_data_29F[] = "******* M566 ObjectSetup *********\n";

/* Offset of the dust puff from the character's hand, rotated by the bone matrix. */
static const HuVecF lbl_1_rodata_128 = { 0.0f, 30.0f, 0.0f };

/* Axis the starfighter tumbles around: straight up. */
static const Vec lbl_1_rodata_2B0 = { 0.0f, 1.0f, 0.0f };

/* Key light direction, colour and the white it is tinted with. */
static const Vec lbl_1_rodata_7E8 = { 0.0f, 5000.0f, 0.0f };
static const Vec lbl_1_rodata_7F4 = { 0.0f, -1.0f, -0.5f };
static const GXColor lbl_1_rodata_800 = { 255, 255, 255, 255 };

/* Per-character starfighter placement: index is the clamped character number,
   two rows of five (height, then depth) read through a single pointer. */
static const s32 lbl_1_rodata_2BC[10] = { -10, 0, 30, -50, -10, -35, 60, -70, -55, -120 };
static const s32 lbl_1_rodata_2E4[10] = { 0, 0, 0, -15, -35, 0, -20, -5, 0, -30 };

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

/* Signed table lookup: the tables are plain s32 rows, one row per character. */
static const s32 PadBiasX[4] = { 20, -20, 20, -20 };
static const s32 PadBiasY[4] = { -20, -20, 20, 20 };
static const s32 LookTbl[10] = { 1, 1, 0, 0, 0, 0, 0, 1, 1, 1 };
static const s32 SearchTime[8] = { 45, 30, 15, 5, 30, 30, 10, 5 };
static const s32 UseTime[8] = { 120, 90, 60, 45, 60, 60, 60, 30 };
static const s32 UseProb[4] = { 50, 65, 80, 90 };
static const s32 BlinkTbl[8] = { 0, 3, 4, 3, 0, 1, 2, 1 };

// NONMATCHING: faithful translation; the -O0,p home-register allocation differs,
// which shifts every stack slot and most of the FPR pair loads.
/* Main per-frame update of one player object: input or computer AI, movement,
   collision against the other players and the items, item pickup and use. */
void fn_1_668(OMOBJ *obj)
{
    M566Player *work = obj->data;
    M566Player *players[4];
    M566Item *item;
    f32 stickX = 0.0f;
    f32 stickY = 0.0f;
    f32 padF = 0.0f;
    s32 anim = -1;
    s32 i;
    s32 j;
    s32 n;
    s32 no;
    s32 kind;
    s32 blink;
    s32 wrap;
    s32 step;
    s32 hold;
    s32 nearNo;
    s32 nearKind;
    s32 mode;
    f32 dx;
    f32 dz;
    f32 dist;
    f32 len;
    f32 want;
    f32 t;
    f32 angle;
    f32 diff;
    f32 f;
    Vec vec;
    Mtx mtx;

    if (work->no == 0) {
        for (i = 0; i < 29; i++) {
            lbl_1_bss_10[i] = -1;
        }
    }
    for (i = 0; i < 4; i++) {
        players[i] = lbl_1_bss_C8[i]->data;
    }
    work->velZ = 0.0f;
    work->velX = 0.0f;
    /* Normalise the facing angle into [-180, 180]. */
    wrap = 0;
    if (obj->rot.y > 180.0f) {
        wrap = -360;
    }
    else if (obj->rot.y < -180.0f) {
        wrap = 360;
    }
    obj->rot.y += wrap;

    if (work->mode == 2) {
        if (lbl_1_bss_BC < 1003) {
            if (lbl_1_bss_BC == 1000) {
                if (lbl_1_bss_0[0] <= 43.0f) {
                    if (lbl_1_bss_0[0] <= 53.0f) {
                        stickX = (PadBiasX[work->no] / 20) * 0xA;
                        stickY = (PadBiasY[work->no] / 20) * 0xA;
                    }
                }
            }
            if (lbl_1_bss_BC == 1001 && work->no == 0 && lbl_1_bss_0[0] == 168.0f) {
                padF = 1.0f;
            }
            if (lbl_1_bss_BC == 1002) {
                /* Camera look-at follows the player during the intro. */
                if (lbl_1_bss_0[0] < 110.0f) {
                    work->unk58 = (f32)(180.0 * (atan2(lbl_1_bss_1600[lbl_1_bss_1810->no].pos.y - 50.0f,
                        -lbl_1_bss_C8[0]->trans.z) / 3.141592653589793));
                }
                else {
                    work->unk58 = (f32)(0.9f * work->unk58);
                }
                work->unk58 = (work->unk58 < 0.0f) ? 0.0f : ((work->unk58 > 60.0f) ? 60.0f : work->unk58);
                if (LookTbl[work->charNoClamp] != 0) {
                    work->unk58 = (f32)(180.0f - (45.0f * (work->unk58 / 60.0f)));
                }
                else {
                    f = (work->charNoClamp != 2) ? 1.0 : 0.35;
                    work->unk58 = (f32)(90.0 - ((f32)(45.0f * (work->unk58 / 60.0f)) * f));
                }
                Hu3DMotionForceSet(obj->mdlId[0], lbl_1_data_210, 0x200U, work->unk58);
            }
            if (lbl_1_bss_BC == 1003 && work->unk58 != 0.0f) {
                work->unk58 = 0.0f;
                Hu3DMotionNoMotReset(obj->mdlId[0], lbl_1_data_210, 0x200U);
            }
        }
        else if (lbl_1_bss_BC < 1004) {
            if (work->padNo != -1 || (*HuPadBtn & 0x200) != 0) {
                stickX = (f32)(s8)(u8)HuPadStkX[work->padNo];
                stickY = (f32)(s8)(u8)HuPadStkY[work->padNo];
                padF = (f32)(HuPadBtnDown[work->padNo] & 0x100);
            }
            else {
                switch (work->comMode) {
                case 0:
                    /* Look for the closest free item once the think timer expires. */
                    for (i = 0, n = 0; i < 29; i++) {
                        if (work->comSkip[i] == 0) {
                            n++;
                        }
                    }
                    if (n == 0) {
                        for (i = 0; i < 29; i++) {
                            work->comSkip[i] = (work->comSkip[i] == 2) ? 0 : work->comSkip[i];
                        }
                    }
                    work->comTime++;
                    if (work->comTimeMax == 0) {
                        work->comTimeMax = (s32)((f32)SearchTime[work->comDif]
                            + ((f32)SearchTime[work->comDif + 4] * ((f32)rand8() / 255.0f)));
                    }
                    if (work->comTime >= work->comTimeMax) {
                        dist = 1048576.0f;
                        work->comTarget = -1;
                        for (i = 0; i < 29; i++) {
                            if (work->comSkip[i] == 0 && lbl_1_bss_1810[i].kind != 0
                                && lbl_1_bss_1810[i].state == 0) {
                                len = sqrtf(((lbl_1_bss_1810[i].pos.x - obj->trans.x)
                                        * (lbl_1_bss_1810[i].pos.x - obj->trans.x))
                                    + ((lbl_1_bss_1810[i].pos.z - obj->trans.z)
                                        * (lbl_1_bss_1810[i].pos.z - obj->trans.z)));
                                t = (10.0f * ((f32)rand8() * 0.00390625f)) + len;
                                if (dist > t) {
                                    work->comTarget = i;
                                    dist = t;
                                }
                            }
                        }
                        if (work->comTarget != -1) {
                            work->comMode = 1;
                            work->comTime = 0;
                            work->comDist = 10000.0f;
                            work->comMoveTime = 0;
                            work->comTurnTime = 0x46;
                            work->comTurnDir = 0;
                        }
                    }
                    break;
                case 1:
                    /* Walk towards the target item, turning smoothly. */
                    dx = lbl_1_bss_1810[work->comTarget].pos.x - obj->trans.x;
                    dz = lbl_1_bss_1810[work->comTarget].pos.z - obj->trans.z;
                    if (work->comAvoid != -1 && work->comTurnTime >= 0x14) {
                        want = (f32)(180.0 * (atan2(dx, dz) / 3.141592653589793));
                        angle = (f32)(180.0 * (atan2(work->avoidX - obj->trans.x,
                            work->avoidZ - obj->trans.z) / 3.141592653589793));
                        wrap = 0;
                        if ((angle - want) <= -180.0f) {
                            wrap = 0x168;
                        }
                        else if ((angle - want) > 180.0f) {
                            wrap = -0x168;
                        }
                        angle += wrap;
                        step = 1;
                        if (work->comAngle != 90.0f) {
                            if (work->comAngle != -90.0f) {
                                step = 0;
                            }
                        }
                        if (step != 0) {
                            hold = 0;
                        }
                        else {
                            hold = (angle - want) < 0.0f ? 4 : -4;
                        }
                        work->comAngle += hold;
                        if (work->comTurnDir == 0) {
                            work->comTurnDir = (hold < 0) ? -1 : ((hold > 0) ? 1 : 0);
                        }
                        else if ((hold * work->comTurnDir) < 0) {
                            work->comTurnTime -= 0x14;
                            work->comTurnDir = (hold < 0) ? -1 : ((hold > 0) ? 1 : 0);
                        }
                    }
                    else {
                        hold = ((work->comAngle < 6.0f) && (work->comAngle > -6.0f)) ? 0
                            : ((work->comAngle < 0.0f) ? 6 : -6);
                        work->comAngle += hold;
                    }
                    if (((work->comDist * work->comDist) - 100.0f) > ((dx * dx) + (dz * dz))) {
                        work->comDist = sqrtf((dx * dx) + (dz * dz));
                        work->comMoveTime = 0;
                    }
                    work->comMoveTime++;
                    work->comTime++;
                    work->comTurnTime += (work->comTurnTime < 0x46) ? 1 : 0;
                    work->comAvoid = -1;
                    vec.x = dx / 0.25f;
                    vec.y = 0.0f;
                    vec.z = -(-dz / 0.25f);
                    PSMTXRotRad(mtx, 'Z', 0.017453292f * work->comAngle);
                    PSMTXMultVec(mtx, &vec, &vec);
                    stickX = vec.x;
                    stickY = -vec.z;
                    if (lbl_1_bss_1810[work->comTarget].kind == 0
                        || lbl_1_bss_1810[work->comTarget].state != 0) {
                        work->comMode = 0;
                        work->comTime = 0;
                        work->comTimeMax = 0;
                    }
                    else if (work->nearItem == work->comTarget) {
                        angle = (f32)(180.0 * (atan2(lbl_1_bss_1810[work->comTarget].pos.x - obj->trans.x,
                            lbl_1_bss_1810[work->comTarget].pos.z - obj->trans.z) / 3.141592653589793));
                        wrap = 0;
                        if ((angle - obj->rot.y) <= -180.0f) {
                            wrap = 0x168;
                        }
                        else if ((angle - obj->rot.y) > 180.0f) {
                            wrap = 0x168;
                        }
                        angle += wrap;
                        if (fabs(angle - obj->rot.y) == 45.0f) {
                            work->comMode = 2;
                            work->comTime = 0;
                            work->comTimeMax = 0;
                        }
                    }
                    else if (work->comMoveTime > (work->no * 5) + 0x5A) {
                        work->comSkip[work->comTarget] = 2;
                        work->comMode = 0;
                        work->comTime = 0;
                        work->comTimeMax = 1;
                    }
                    break;
                case 2:
                    /* Use the item: decide whether it actually fires. */
                    work->comTime++;
                    if (work->comTimeMax == 0) {
                        work->comTimeMax = (s32)((f32)UseTime[work->comDif]
                            + ((f32)UseTime[work->comDif + 4] * ((f32)rand8() / 255.0f)));
                    }
                    if (work->comTime >= work->comTimeMax) {
                        work->comMode = 0;
                        work->comTime = 0;
                        work->comTimeMax = 0;
                        if (lbl_1_bss_1810[work->nearItem].kind == 1) {
                            padF = 1.0f;
                        }
                        padF = ((f32)rand8() < (256.0f * (f32)UseProb[work->comDif]) / 100.0f)
                            ? padF : (f32)((padF != 0.0f) == 0);
                        work->comSkip[work->comTarget] = 1;
                    }
                    else if (work->nearItem != work->comTarget) {
                        work->comMode = 1;
                        work->comTime = 0;
                    }
                    break;
                }
            }
        }
        /* Rotate the stick vector by the camera and clamp its length. */
        vec.x = 0.25f * stickX;
        vec.y = 0.0f;
        vec.z = 0.25f * -stickY;
        PSMTXRotRad(mtx, 'Z', 0.017453292f * CRotM[1].y);
        PSMTXMultVec(mtx, &vec, &vec);
        len = sqrtf((vec.x * vec.x) + (vec.z * vec.z));
        if (len < 0.5f) {
            work->velX = vec.x;
            work->velZ += vec.z;
        }
        else if (len < 10.0f) {
            work->velX = (vec.x / len) * 10.0f;
            work->velZ = (vec.z / len) * 10.0f;
        }
        else {
            work->velX = (vec.x / len) * 10.0f;
            work->velZ = (vec.z / len) * 10.0f;
        }
        work->speed = len;
        if (work->mode == 2) {
            obj->rot.y = (f32)(180.0 * (atan2(work->velX, work->velZ) / 3.141592653589793));
        }
        if (work->mode == 2) {
            anim = (work->speed <= 8.0f) ? 2 : 1;
        }
        else {
            anim = 0;
        }
    }
    else if (work->mode == 3) {
        /* Pick the item up: walk to it, then attach it to the model. */
        work->itemTime = 0U;
        if (work->modeTime < 0xA) {
            dist = sqrtf(((lbl_1_bss_1810[work->itemNo].pos.x - obj->trans.x)
                    * (lbl_1_bss_1810[work->itemNo].pos.x - obj->trans.x))
                + ((lbl_1_bss_1810[work->itemNo].pos.z - obj->trans.z)
                    * (lbl_1_bss_1810[work->itemNo].pos.z - obj->trans.z)));
            t = (f32)work->modeTime / 9.0f;
            angle = (f32)(180.0 * (atan2(lbl_1_bss_1810[work->itemNo].pos.x - obj->trans.x,
                lbl_1_bss_1810[work->itemNo].pos.z - obj->trans.z) / 3.141592653589793));
            wrap = 0;
            if ((angle - obj->rot.y) < -180.0f) {
                wrap = 0x168;
            }
            else if ((angle - obj->rot.y) > 180.0f) {
                wrap = -0x168;
            }
            angle += wrap;
            obj->rot.y = (obj->rot.y * (1.0f - t)) + (angle * t);
            if (dist == 0.0f) {
                dist = 0.000001f;
            }
            obj->trans.x = (obj->trans.x * (1.0f - t))
                + (t * (lbl_1_bss_1810[work->itemNo].pos.x
                    - (75.0f * ((lbl_1_bss_1810[work->itemNo].pos.x - obj->trans.x) / dist))));
            obj->trans.z = (obj->trans.z * (1.0f - t))
                + (t * (lbl_1_bss_1810[work->itemNo].pos.z
                    - (75.0f * ((lbl_1_bss_1810[work->itemNo].pos.z - obj->trans.z) / dist))));
            lbl_1_bss_1810[work->itemNo].state = 4;
            lbl_1_data_19C[work->itemNo] = 0;
        }
        else if (work->modeTime == 0xA) {
            if (lbl_1_bss_8 < 3) {
                lbl_1_bss_600[lbl_1_bss_8].pos.x = lbl_1_bss_1810[work->itemNo].pos.x;
                lbl_1_bss_600[lbl_1_bss_8].pos.z = lbl_1_bss_1810[work->itemNo].pos.z;
                lbl_1_bss_8++;
            }
            switch (lbl_1_bss_1810[work->itemNo].kind) {
            case 1:
                lbl_1_bss_1810[work->itemNo].state = 1;
                break;
            case 2:
                lbl_1_bss_1810[work->itemNo].state = 2;
                break;
            case 3:
                lbl_1_bss_1810[work->itemNo].state = 3;
                break;
            }
            lbl_1_bss_1810[work->itemNo].time = 0;
            lbl_1_bss_1810[work->itemNo].owner = work->no;
            {
                char *hook = CharModelItemHookGet(obj->mdlId[0], 4, 0);

                Hu3DModelHookSet(obj->mdlId[0], hook, lbl_1_bss_C4->mdlId[work->itemNo + 0x1D]);
                Hu3DModelPosSet(lbl_1_bss_C4->mdlId[work->itemNo + 0x1D], 0.0f, 0.0f, 0.0f);
            }
            if (lbl_1_bss_1810[work->itemNo].kind == 2) {
                for (i = 0; i < 4; i++) {
                    if (lbl_1_data_124[i] != 0) {
                        continue;
                    }
                }
                if (i < 4) {
                    lbl_1_bss_1810[work->itemNo].slot = i;
                    lbl_1_data_124[i] = 1;
                }
                else {
                    lbl_1_bss_1810[work->itemNo].slot = -1;
                }
            }
        }
        else {
            n = (lbl_1_bss_1810[work->itemNo].kind != 2) ? 0x14 : 0x28;
            if (work->modeTime == n) {
                if (lbl_1_bss_1810[work->itemNo].kind != 3) {
                    /* Puff of dust at the model's hand. */
                    char *hook = CharModelItemHookGet(obj->mdlId[0], 4, 0);

                    Hu3DModelObjMtxGet(obj->mdlId[0], hook, mtx);
                    PSMTXMultVec(mtx, &lbl_1_rodata_128, &vec);
                    for (i = 0, n = 0; i < 128; i++) {
                        if (lbl_1_bss_600[i].active > 0) {
                            continue;
                        }
                        lbl_1_bss_600[i].active = 1;
                        lbl_1_bss_600[i].alpha = 0x80;
                        lbl_1_bss_600[i].pos = vec;
                        lbl_1_bss_600[i].vel.x = 12.0f * ((2.0f * ((f32)rand8() / 255.0f)) - 1.0f);
                        lbl_1_bss_600[i].vel.y = 12.0f * ((2.0f * ((f32)rand8() / 255.0f)) - 1.0f);
                        lbl_1_bss_600[i].vel.z = 12.0f * ((2.0f * ((f32)rand8() / 255.0f)) - 1.0f);
                        n++;
                        if (n >= 0x20) {
                            break;
                        }
                    }
                    Hu3DModelAttrSet(lbl_1_bss_C4->mdlId[work->itemNo + 0x1D], 1U);
                }
                else {
                    /* Hand the item over with its orientation. */
                    char *hook = CharModelItemHookGet(obj->mdlId[0], 4, 0);

                    Hu3DModelObjMtxGet(obj->mdlId[0], hook, mtx);
                    C_QUATMtx(&lbl_1_bss_1810[work->itemNo].quat, mtx);
                    PSQUATInverse(&lbl_1_bss_1810[work->itemNo].quat, &lbl_1_bss_1810[work->itemNo].quat);
                    Hu3DMtxTransGet(mtx, &lbl_1_bss_1810[work->itemNo].holdPos);
                    Hu3DModelHookReset(obj->mdlId[0]);
                    Hu3DModelPosSet(lbl_1_bss_C4->mdlId[work->itemNo + 0x1D],
                        lbl_1_bss_1810[work->itemNo].holdPos.x,
                        lbl_1_bss_1810[work->itemNo].holdPos.y,
                        lbl_1_bss_1810[work->itemNo].holdPos.z);
                    PSMTXQuat(mtx, &lbl_1_bss_1810[work->itemNo].quat);
                    Hu3DMtxRotGet(mtx, &vec);
                    Hu3DModelRotSet(lbl_1_bss_C4->mdlId[work->itemNo + 0x1D], vec.x, vec.y, vec.z);
                }
            }
        }
        anim = 5;
        mode = lbl_1_bss_1810[work->itemNo].kind;
        if (mode != 1) {
            if (mode >= 1 && mode < 4) {
                n = (mode == 3) ? 0x14 : 0x28;
                if (work->modeTime >= n) {
                    work->mode = 4;
                    work->modeTime = 0;
                    if (mode == 2) {
                        omVibrate((s16)work->playerNo, 0xC, 0xC, 0);
                    }
                }
            }
        }
        else {
            if (work->modeTime >= 0x14) {
                anim = 0;
            }
            n = (lbl_1_bss_BC == 1002) ? 0xA : 0x3C;
            if (work->modeTime >= n + 0x14) {
                if (lbl_1_bss_BC == 1003) {
                    if (work->charNoClamp < 4) {
                        work->effTime[work->charNoClamp] = 0x1E;
                    }
                    if (work->charNoClamp + lbl_1_bss_1810[work->itemNo].unk10 < 4) {
                        work->effTime[work->charNoClamp + lbl_1_bss_1810[work->itemNo].unk10] = 0x1E;
                    }
                    work->charNoClamp = lbl_1_bss_1810[work->itemNo].unk10 + work->charNoClamp + 1;
                }
                work->mode = 2;
                work->modeTime = 0;
            }
        }
    }
    else if (work->mode == 4) {
        /* Play the item's animation. */
        if (lbl_1_bss_1810[work->itemNo].kind == 3) {
            anim = (work->modeTime <= 0x36) ? 4 : ((work->modeTime <= 0x72) ? 6 : 0);
            if (work->modeTime == 3) {
                CharFXPlay((s16)work->charNo, 0x1F9);
                fn_1_518(0, obj->trans.x, obj->trans.z);
            }
            if (work->modeTime >= 0x78) {
                work->mode = 2;
            }
            if (work->modeTime < 0x3C && (work->modeTime % 30) == 1) {
                omVibrate((s16)work->playerNo, 0xC, 0xC, 0);
            }
        }
        else {
            if (work->modeTime == 0x28) {
                Hu3DModelAttrSet(obj->mdlId[0], 0x40000002U);
            }
            anim = (work->modeTime <= 0x27) ? 3 : 6;
            if (work->modeTime == 1) {
                angle = (f32)(180.0 * (atan2(lbl_1_bss_1810[work->itemNo].pos.x - obj->trans.x,
                    lbl_1_bss_1810[work->itemNo].pos.z - obj->trans.z) / 3.141592653589793));
                wrap = 0;
                if (angle <= -180.0f) {
                    wrap = 360.0f + angle;
                }
                else if (angle > 180.0f) {
                    wrap = angle - 360.0f;
                }
                else {
                    wrap = angle;
                }
                obj->rot.y = angle;
                omVibrate((s16)work->playerNo, 0xC, 0xC, 0);
                CharFXPlay((s16)work->charNo, 0x202);
                fn_1_518(0, obj->trans.x, obj->trans.z);
            }
            if (work->modeTime >= 0x69) {
                work->mode = 2;
            }
            if (work->modeTime <= 0x2D) {
                work->velX = (f32)((100.0 * -sin((3.141592653589793 * (f64)obj->rot.y) / 180.0)) / 45.0);
                work->velZ = (f32)((100.0 * -cos((3.141592653589793 * (f64)obj->rot.y) / 180.0)) / 45.0);
            }
        }
    }
    work->modeTime++;
    obj->trans.x += work->velX;
    obj->trans.z += work->velZ;

    if (lbl_1_bss_BC < 1005) {
        /* Keep 100 units away from the other players. */
        for (i = 0; i < 4; i++) {
            dx = lbl_1_bss_C8[i]->trans.x - obj->trans.x;
            dz = lbl_1_bss_C8[i]->trans.z - obj->trans.z;
            if (i != work->no) {
                if (dx == 0.0f && dz == 0.0f) {
                    dz = -1.0f;
                }
                dist = sqrtf((dx * dx) + (dz * dz));
                if (dist < 100.0f) {
                    obj->trans.x = lbl_1_bss_C8[i]->trans.x - (dx * (100.0f / dist));
                    obj->trans.z = lbl_1_bss_C8[i]->trans.z - (dz * (100.0f / dist));
                    if (work->padNo == -1 && work->comMode == 1) {
                        work->comAvoid = i;
                        work->avoidX = lbl_1_bss_C8[i]->trans.x;
                        work->avoidZ = lbl_1_bss_C8[i]->trans.z;
                    }
                }
            }
        }
        /* Keep 90 (or 74) units away from the items. */
        len = (work->mode != 3) ? 90.0f : 74.0f;
        for (i = 0; i < 29; i++) {
            dx = lbl_1_bss_1810[i].pos.x - obj->trans.x;
            dz = lbl_1_bss_1810[i].pos.z - obj->trans.z;
            if (lbl_1_bss_1810[i].kind != 0
                && (work->no != lbl_1_bss_1810[i].owner
                    || (lbl_1_bss_1810[i].state != 2 && lbl_1_bss_1810[i].state != 1))) {
                if (dx == 0.0f && dz == 0.0f) {
                    dz = -1.0f;
                }
                dist = sqrtf((dx * dx) + (dz * dz));
                if (dist < len) {
                    dx = dx * (len / dist);
                    obj->trans.x = lbl_1_bss_1810[i].pos.x - dx;
                    obj->trans.z = lbl_1_bss_1810[i].pos.z - (dz * (len / dist));
                    if (work->padNo == -1 && work->comMode == 1) {
                        work->comAvoid = i + 4;
                        work->avoidX = lbl_1_bss_1810[i].pos.x;
                        work->avoidZ = lbl_1_bss_1810[i].pos.z;
                    }
                }
            }
        }
        dist = sqrtf((obj->trans.x * obj->trans.x) + (obj->trans.z * obj->trans.z));
        if (dist > 650.0f) {
            obj->trans.x *= 650.0f / dist;
            obj->trans.z *= 650.0f / dist;
        }
    }

    work->nearItem = 0;
    Hu3DModelAttrSet(lbl_1_bss_C4->mdlId[(work->no + 9) * 2], 1U);
    if (lbl_1_bss_BC >= 1000 && work->mode <= 2) {
        /* The item icon above the player blinks while it is still free. */
        nearNo = -1;
        dist = 10000.0f;
        for (i = 0; i < 29; i++) {
            if (lbl_1_bss_1810[i].kind != 0 && lbl_1_bss_1810[i].state == 0) {
                len = sqrtf(((lbl_1_bss_1810[i].pos.x - obj->trans.x)
                        * (lbl_1_bss_1810[i].pos.x - obj->trans.x))
                    + ((lbl_1_bss_1810[i].pos.z - obj->trans.z)
                        * (lbl_1_bss_1810[i].pos.z - obj->trans.z)));
                if (!(len > 120.0f)) {
                    diff = (f32)((180.0 * (atan2(lbl_1_bss_1810[i].pos.x - obj->trans.x,
                        lbl_1_bss_1810[i].pos.z - obj->trans.z) / 3.141592653589793)) - (f64)obj->rot.y);
                    wrap = 0;
                    if (diff <= -180.0f) {
                        wrap = 0x168;
                    }
                    else if (diff > 180.0f) {
                        wrap = -0x168;
                    }
                    diff += wrap;
                    if (!(fabs(diff) > 100.0) && dist > len) {
                        nearNo = i;
                        dist = len;
                    }
                }
            }
        }
        work->itemTime = (work->itemNo == nearNo) ? work->itemTime : 0;
        work->itemNo = nearNo;
        if (work->itemNo != -1) {
            blink = 0;
            if (lbl_1_bss_1810[work->itemNo].kind != 0) {
                work->nearItem = work->itemNo;
            }
            nearKind = (lbl_1_bss_1810[work->itemNo].kind == 1) ? lbl_1_data_14C[0]
                : ((lbl_1_bss_1810[work->itemNo].kind == 2) ? lbl_1_data_14C[1] : lbl_1_data_14C[2]);
            switch (nearKind) {
            case 0:
                if ((work->itemTime % 90) < 0x3C && (work->itemTime % 12) < 6) {
                    blink = 1;
                }
                break;
            case 1:
                if ((((work->itemTime % 90) < 0x1B
                        || ((work->itemTime % 90) >= 0x21 && (work->itemTime % 90) < 0x3C))
                        && (work->itemTime % 2) == 0)
                    || (work->itemTime % 90) < 3) {
                    blink = 1;
                }
                break;
            case 2:
                if ((((work->itemTime % 90) < 0x28 && ((work->itemTime % 90) % 20) < 0xA)
                        || ((work->itemTime % 90) >= 0x28 && (work->itemTime % 90) < 0x50
                            && (work->itemTime % 2) == 0))) {
                    blink = 1;
                }
                break;
            case 3:
                if ((work->itemTime % 30) < 0x14) {
                    blink = 1;
                }
                break;
            case 4:
                if ((work->itemTime % 30) < 6) {
                    blink = 1;
                }
                break;
            }
            if (lbl_1_data_158 != 0) {
                if (blink != 0) {
                    omVibrate((s16)work->playerNo, 1, 1, 0);
                }
            }
            else {
                work->rumbleTime = (blink != 0) ? work->rumbleTime + 1 : 0;
            }
            work->itemTime++;
            lbl_1_bss_1810[work->itemNo].wobble += (lbl_1_bss_1810[work->itemNo].wobble == 0) ? 8 : 0;
            lbl_1_bss_1810[work->itemNo].unk24 = 1;
            Hu3DModelAttrReset(lbl_1_bss_C4->mdlId[(work->no + 9) * 2], 1U);
            Hu3DModelPosSet(lbl_1_bss_C4->mdlId[(work->no + 9) * 2],
                lbl_1_bss_1810[work->itemNo].pos.x,
                lbl_1_bss_1810[work->itemNo].pos.x,
                lbl_1_bss_1810[work->itemNo].pos.z);
            if (padF != 0.0f && lbl_1_bss_1810[work->itemNo].state == 0
                && lbl_1_bss_1810[work->itemNo].kind != 0 && lbl_1_data_19C[work->itemNo] == 0) {
                lbl_1_data_19C[work->itemNo] = 1;
                work->mode = 3;
                work->modeTime = 0;
                if (lbl_1_bss_1810[work->itemNo].kind == 1) {
                    /* All five of that kind taken: steal one from a rival. */
                    for (i = 0, n = 0; i < 29; i++) {
                        if (i != work->itemNo) {
                            hold = 0;
                            if (lbl_1_bss_1810[i].state == 0 && lbl_1_bss_1810[i].kind == 1) {
                                hold = 1;
                            }
                            n += hold;
                        }
                    }
                    if (n == 5) {
                        do {
                            j = rand8() % 29;
                        } while (lbl_1_bss_1810[j].kind != 1 || lbl_1_bss_1810[j].state != 0
                            || j == work->itemNo);
                        for (i = 1;; i++) {
                            if (lbl_1_bss_1810[i].kind == 1 && lbl_1_bss_1810[i].state == 0
                                && i != work->itemNo) {
                                break;
                            }
                        }
                        lbl_1_bss_1810[i].unk10 = 1;
                    }
                }
            }
        }
        else {
            work->rumbleTime = 0;
        }
    }
    else {
        work->rumbleTime = 0;
        work->itemTime = 0U;
    }

    if (work->mode == 5) {
        anim = 0;
    }
    else if (work->mode == 6) {
        /* Blast off: launch the starfighters. */
        anim = 9;
        obj->rot.y *= 0.8f;
        if (work->modeTime == 0x19) {
            for (i = 0; i < 0xB; i++) {
                if (work->no == lbl_1_bss_1600[i].owner && lbl_1_bss_1600[i].mode == 4) {
                    lbl_1_bss_1600[i].mode = 6;
                    lbl_1_bss_1600[i].time = 0;
                }
            }
        }
    }
    else if (work->mode == 7) {
        /* Victory: fly the starfighters out in a circle. */
        anim = 0;
        obj->rot.y *= 0.8f;
        if (work->modeTime == 0x19) {
            n = 0;
            for (i = 0; i < 0xB; i++) {
                if (work->no == lbl_1_bss_1600[i].owner && lbl_1_bss_1600[i].mode == 4) {
                    lbl_1_bss_1600[i].mode = 5;
                    lbl_1_bss_1600[i].time = 0;
                    n++;
                }
            }
            j = 0;
            for (i = 0; i < 0xB; i++) {
                if (work->no == lbl_1_bss_1600[i].owner && lbl_1_bss_1600[i].mode == 5) {
                    lbl_1_bss_1600[i].unk1C = (f32)(70.0 * sin((3.141592653589793
                        * (f64)(((180.0f / (f32)n) * 0.5f) + (90.0f + ((180.0f / (f32)n) * (f32)j)))) / 180.0));
                    lbl_1_bss_1600[i].unk24 = (f32)((70.0 * cos((3.141592653589793
                        * (f64)(((180.0f / (f32)n) * 0.5f) + (90.0f + ((180.0f / (f32)n) * (f32)j)))) / 180.0)) - 35.0);
                    lbl_1_bss_1600[i].unk20 = CharModelHeightGet((s16)work->charNo) - 20.0f;
                    j++;
                }
            }
        }
        if (work->modeTime == 0xB5) {
            Hu3DMotionOverlaySet(obj->mdlId[0], obj->mtnId[4]);
            Hu3DMotionOverlayTimeSet(obj->mdlId[0], 0.0f);
            Hu3DModelAttrReset(obj->mdlId[0], 0x40000400U);
        }
        if (work->modeTime >= 0xB5) {
            anim = 7;
            obj->rot.y *= 0.8f;
            if (Hu3DMotionOverlayTimeGet(obj->mdlId[0]) == 3.2886717319488525) {
                Hu3DMotionOverlayTimeSet(obj->mdlId[0], 35.0f);
                Hu3DModelAttrReset(obj->mdlId[0], 0x40000400U);
            }
            n = (LookTbl[work->charNoClamp] != 0)
                ? 0xB4 - ((work->charNoClamp != 9) * 0x2D)
                : 0x5A - (((work->charNoClamp != 2 && work->charNoClamp != 5) ? 1 : 0) * 0x2D);
            Hu3DMotionForceSet(obj->mdlId[0], lbl_1_data_210, 0x200U, n);
        }
        t = 0.0f;
        if (work->modeTime >= 0xAF) {
            t = (work->modeTime < 0xCD)
                ? 6.0f * ((f32)(work->modeTime - 0xAF) / 30.0f) * ((f32)work->score / 3.0f)
                : 6.0f * ((f32)work->score / 3.0f);
        }
        obj->trans.y += (180.0f * (0.866f * t)) / 210.0f;
        f = 0.0f;
        if (work->modeTime >= 0xAF) {
            f = (work->modeTime < 0xCD)
                ? 6.0f * ((f32)(work->modeTime - 0xAF) / 30.0f) * ((f32)work->score / 3.0f)
                : 6.0f * ((f32)work->score / 3.0f);
        }
        obj->trans.z += (180.0f * (0.5f * f)) / 210.0f;
    }

    if (work->motReq >= 0) {
        anim = work->motReq;
    }
    if (anim >= 0 && anim != work->motNo) {
        s32 attr = (u8)lbl_1_data_E8[anim] ? 0 : 0x40000001;

        Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[anim], (f32)lbl_1_data_FC[anim],
            (f32)lbl_1_data_F2[anim], attr);
        work->motNo = anim;
    }

    /* Fade the three effect sprites in and out. */
    for (i = 0; i < 3; i++) {
        if (work->effTime[i] > 0) {
            f = 1.0f - ((f64)(work->effTime[i] - 1) / 30.0);
            no = i + 3;
            f = (f32)((0.5 * (f64)f) + (0.5 * (f64)(1.0f - ((1.0f - f) * (1.0f - f)))));
            f = (f32)((f < 0.75f) ? (f64)f / 0.75 : 1.0);
            t = 1.0f - ((1.0f - f) * (1.0f - f));
            f = 1.0f - ((1.0f - f) * (1.0f - t));
            if (work->effTime[i] == 0x1E) {
                espDispOn((s16)lbl_1_data_1C[work->no][no]);
                espDispOff((s16)lbl_1_data_1C[work->no][i]);
            }
            espTPLvlSet((s16)lbl_1_data_1C[work->no][no], f);
            espScaleSet((s16)lbl_1_data_1C[work->no][no], 4.0f - (3.0f * t), 4.0f - (3.0f * t));
            work->effTime[i]--;
        }
    }
}

// NONMATCHING: complete, asm-read translation. The original re-derives
// lbl_1_bss_1810[i] / lbl_1_bss_1600[no] / lbl_1_bss_600[i] at every access
// instead of caching the pointers (264/266/28 references against our 26/26/2
// before this was fixed), which is reproduced here, so `obj` lands in r30 and
// the item loop counter in r31 as in the original. What still differs is the
// local-slot layout, and it is a missing-locals problem, not an allocation
// one: the original spends 0x8..0x1ac on 81 slots (34 float, 40 int, 7
// double) where this build spends 0x8..0xdc on 34, so the frame is 0x200
// against 0x2e0 and every stack offset shifts. The int half is now nearly
// closed (30 of the original's 40 int slots, the two per-character offset
// tables included); what is left is float. The original keeps 34 f32 locals
// where this build has 3, because it gives each computed quantity its own
// variable instead of reusing dx/dy/dz/t/scale/len. Recovering those ~31
// names is what stands between this and a match.
/* Second half of the object update: the five item stands, the 29 collectibles
   (carried / dropped / thrown), the starfighters, the dust puffs, the shadow
   casters and the end-of-round camera. */
void fn_1_48F8(OMOBJ *obj)
{
    M566Player *players[4];
    /* Per-player counter of shadow casters already placed this frame. */
    s32 castCount[4] = { 0 };
    s32 i;
    s32 j;
    s32 k;
    s32 n;
    s32 m;
    s32 no;
    s32 owner;
    s32 count;
    s32 step;
    s32 wrap;
    s32 base;
    s32 off;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 len;
    f32 scale;
    f32 t;
    Vec axis;
    Quaternion quat;
    Mtx mtx;

    for (i = 0; i < 4; i++) {
        players[i] = lbl_1_bss_C8[i]->data;
    }

    /* Intro wipe: blink the eight stand models while the wipe runs. */
    if (lbl_1_bss_BC == 1000 && (lbl_1_bss_0[0] == 1.0f || lbl_1_bss_0[0] == 2.0f)) {
        for (i = 0; i < 8; i++) {
            if (lbl_1_bss_0[0] == 1.0f) {
                Hu3DModelAttrReset(obj->mdlId[i + 0x50], 1);
            }
            else {
                Hu3DModelAttrSet(obj->mdlId[i + 0x50], 1);
            }
        }
    }

    /* Item stand respawn timers; five stands, each with its own item. */
    for (i = 0; i < 5; i++) {
        if (lbl_1_data_230[i] == -1) {
            lbl_1_data_230[i] = (rand8() % 128) + 20;
        }
        if (lbl_1_data_21C[i] == 0) {
            lbl_1_data_230[i]--;
            if (lbl_1_data_230[i] == 0) {
                /* The item is about to appear: prime the spin and pick a free stand. */
                lbl_1_data_21C[i] = 1;
                lbl_1_data_230[i] = ((rand8() % 4) + 1) * 0x10;
                if (lbl_1_bss_1810[lbl_1_data_244[i]].wobble <= lbl_1_data_230[i]) {
                    lbl_1_bss_1810[lbl_1_data_244[i]].wobble =
                        lbl_1_data_230[i] + (lbl_1_bss_1810[lbl_1_data_244[i]].wobble % 8);
                    lbl_1_bss_1810[lbl_1_data_244[i]].unk24 = 0;
                }
                for (;;) {
                    lbl_1_data_244[i] = (rand8() % 28) + 1;
                    for (j = 0; j < 5; j++) {
                        if (i == j || lbl_1_data_244[i] != lbl_1_data_244[j]) {
                            continue;
                        }
                        break;
                    }
                    if (j == 5) {
                        break;
                    }
                }
            }
        }
        else if (lbl_1_data_21C[i] == 1) {
            lbl_1_data_230[i]--;
            if (lbl_1_bss_1810[lbl_1_data_244[i]].kind != 0) {
                /* The original reads the item's state word here and drops the result. */
            }
            if (lbl_1_data_230[i] == 0) {
                lbl_1_data_21C[i] = 0;
                lbl_1_data_230[i] = (rand8() % 128) + 60;
            }
        }
    }

    /* Every stand item slowly spins about Y, faster once it is being grabbed. */
    for (i = 0; i < 29; i++) {
        n = (lbl_1_bss_1810[i].unk24 != 0) ? 6 : 4;
        Hu3DModelRotSet(obj->mdlId[i + 0x1D], 0.0f, 0.0f,
            (f32)((f64)n * sin((3.141592653589793
                * (f64)(-360.0f * (f32)(lbl_1_bss_1810[i].wobble % 8) * 0.125f)) / 180.0)));
        lbl_1_bss_1810[i].wobble -= (lbl_1_bss_1810[i].wobble > 0);
    }

    for (i = 0; i < 29; i++) {
        owner = lbl_1_bss_1810[i].owner;
        switch (lbl_1_bss_1810[i].state) {
        case 0:
        case 4:
            break;
        case 1:
            /* Carried: spawn the player's escort fighters the first frame. */
            if (lbl_1_bss_1810[i].time == 0) {
                for (j = 0; j < lbl_1_bss_1810[i].unk10 + 1; j++) {
                    no = (j == 0) ? lbl_1_bss_1810[i].no : 0xA;
                    lbl_1_bss_1600[no].mode = 2;
                    lbl_1_bss_1600[no].time = 0;
                    lbl_1_bss_1600[no].owner = lbl_1_bss_1810[i].owner;
                    lbl_1_bss_1600[no].slot = j;
                    base = (lbl_1_bss_1810[i].unk10 != 0) ? ((j * 2) - 1) * 15 : 0;
                    lbl_1_bss_1600[no].pos.x = lbl_1_bss_1810[i].pos.x + (f32)base;
                    lbl_1_bss_1600[no].pos.y = lbl_1_bss_1810[i].pos.y;
                    lbl_1_bss_1600[no].pos.z = lbl_1_bss_1810[i].pos.z;
                    lbl_1_bss_1600[no].unk24 = 0.0f;
                    lbl_1_bss_1600[no].unk1C = 0.0f;
                    lbl_1_bss_1600[no].unk20 = 100.0f + CharModelHeightGet((s16)players[owner]->charNo);
                    dx = lbl_1_bss_1600[no].pos.x - lbl_1_bss_C8[lbl_1_bss_1600[no].owner]->trans.x;
                    dz = lbl_1_bss_1600[no].pos.z - lbl_1_bss_C8[lbl_1_bss_1600[no].owner]->trans.z;
                    if (i == 0) {
                        lbl_1_bss_1600[no].angle = 0.0f;
                    }
                    else {
                        lbl_1_bss_1600[no].angle = (f32)(180.0 * (atan2(dx, dz) / 3.141592653589793));
                    }
                    /* Normalise the facing angle into [-180, 180]. */
                    wrap = 0;
                    if (lbl_1_bss_1600[no].angle < -180.0f) {
                        wrap = 360;
                    }
                    else if (lbl_1_bss_1600[no].angle > 180.0f) {
                        wrap = -360;
                    }
                    lbl_1_bss_1600[no].angle += (f32)wrap;
                    lbl_1_bss_1600[no].dist = sqrtf((dx * dx) + (dz * dz));
                    Hu3DModelPosSet(obj->mdlId[no + 0x3A], lbl_1_bss_1600[no].pos.x, lbl_1_bss_1600[no].pos.y, lbl_1_bss_1600[no].pos.z);
                    Hu3DModelRotSet(obj->mdlId[no + 0x3A], 0.0f,
                        180.0f + lbl_1_bss_C8[owner]->rot.y, 0.0f);
                    Hu3DMotionTimeSet(obj->mdlId[no + 0x3A], 0.0f);
                    Hu3DModelAttrReset(obj->mdlId[no + 0x3A], 1);
                }
            }
            if (lbl_1_bss_1810[i].time == 6) {
                /* The original compares the game state here and drops the result. */
            }
            if (lbl_1_bss_1810[i].time == 0xC) {
                /* Launch: spread the live fighters of this owner over a circle. */
                count = 0;
                for (j = 0; j < 11; j++) {
                    if (lbl_1_bss_1600[j].owner == owner && lbl_1_bss_1600[j].mode >= 2) {
                        count++;
                    }
                }
                k = 0;
                for (j = 0; j < 11; j++) {
                    if (lbl_1_bss_1600[j].owner == owner
                        && lbl_1_bss_1600[j].mode != 0 && lbl_1_bss_1600[j].mode != 1) {
                        lbl_1_bss_1600[j].mode = (lbl_1_bss_BC == 1002) ? 1 : 3;
                        lbl_1_bss_1600[j].time = 0;
                        off = (count == 2) ? -90 : ((count == 4) ? -45 : 0);
                        lbl_1_bss_1600[j].unk1C = (f32)((f64)(count > 1) * (30.0
                            * sin((3.141592653589793
                                * (f64)(((f32)k * (360.0f / (f32)count)) + (f32)off)) / 180.0)));
                        lbl_1_bss_1600[j].unk24 = (f32)((f64)(count > 1) * (30.0
                            * cos((3.141592653589793
                                * (f64)(((f32)k * (360.0f / (f32)count)) + (f32)off)) / 180.0)));
                        k++;
                    }
                }
                if (lbl_1_bss_E0[7].num < 3) {
                    lbl_1_bss_E0[7].posX[lbl_1_bss_E0[7].num] = lbl_1_bss_1810[i].pos.x;
                    lbl_1_bss_E0[7].posZ[lbl_1_bss_E0[7].num] = lbl_1_bss_1810[i].pos.z;
                    lbl_1_bss_E0[7].num++;
                }
                if (lbl_1_bss_BC <= 1002) {
                    lbl_1_data_144 = HuAudFXPlay(0x8E1);
                }
                else if (lbl_1_bss_E0[lbl_1_bss_1810[i].no + 9].num < 3) {
                    lbl_1_bss_E0[lbl_1_bss_1810[i].no + 9].posX[lbl_1_bss_E0[lbl_1_bss_1810[i].no + 9].num] =
                        lbl_1_bss_1600[lbl_1_bss_1810[i].no].pos.x;
                    lbl_1_bss_E0[lbl_1_bss_1810[i].no + 9].posZ[lbl_1_bss_E0[lbl_1_bss_1810[i].no + 9].num] =
                        lbl_1_bss_1600[lbl_1_bss_1810[i].no].pos.z;
                    lbl_1_bss_E0[lbl_1_bss_1810[i].no + 9].num++;
                }
            }
            if (lbl_1_bss_1810[i].time >= 10 && players[owner]->mode != 3) {
                lbl_1_bss_1810[i].kind = 0;
                lbl_1_bss_1810[i].state = 0;
                lbl_1_bss_1810[i].time = 0;
            }
            break;
        case 2:
            /* Dropped on the ground: shrink in, then let a nearby player take it. */
            if (lbl_1_bss_1810[i].time == 0) {
                Hu3DModelPosSet(obj->mdlId[lbl_1_bss_1810[i].no + 0x45], lbl_1_bss_1810[i].pos.x, lbl_1_bss_1810[i].pos.y, lbl_1_bss_1810[i].pos.z);
                Hu3DModelRotSet(obj->mdlId[lbl_1_bss_1810[i].no + 0x45], 0.0f,
                    180.0f + lbl_1_bss_C8[owner]->rot.y, 0.0f);
                Hu3DMotionTimeSet(obj->mdlId[lbl_1_bss_1810[i].no + 0x45], 0.0f);
                Hu3DModelAttrReset(obj->mdlId[lbl_1_bss_1810[i].no + 0x45], 1);
                if (lbl_1_bss_1810[i].slot != -1) {
                    Hu3DModelHookReset(obj->mdlId[lbl_1_bss_1810[i].slot + 0x58]);
                    Hu3DModelHookSet(obj->mdlId[lbl_1_bss_1810[i].no + 0x45], lbl_1_data_25C,
                        obj->mdlId[lbl_1_bss_1810[i].slot + 0x58]);
                    Hu3DModelAttrReset(obj->mdlId[lbl_1_bss_1810[i].slot + 0x58], 1);
                    Hu3DMotionTimeSet(obj->mdlId[lbl_1_bss_1810[i].slot + 0x58], 0.0f);
                }
                if (lbl_1_bss_E0[8].num < 3) {
                    lbl_1_bss_E0[8].posX[lbl_1_bss_E0[8].num] = lbl_1_bss_1810[i].pos.x;
                    lbl_1_bss_E0[8].posZ[lbl_1_bss_E0[8].num] = lbl_1_bss_1810[i].pos.z;
                    lbl_1_bss_E0[8].num++;
                }
            }
            if (lbl_1_bss_1810[i].time >= 0 && lbl_1_bss_1810[i].time <= 3) {
                scale = (f32)(0.75 * (0.5 + (0.5 * (f64)(lbl_1_bss_1810[i].time / 3.0f))));
                Hu3DModelScaleSet(obj->mdlId[lbl_1_bss_1810[i].no + 0x45], scale, scale, scale);
                if (lbl_1_bss_1810[i].slot != -1) {
                    Hu3DModelScaleSet(obj->mdlId[lbl_1_bss_1810[i].no + 0x45], scale, scale, scale);
                }
            }
            if (lbl_1_bss_1810[i].kind != 0 && lbl_1_bss_1810[i].time >= 30 && players[owner]->mode != 3) {
                lbl_1_bss_1810[i].kind = 0;
                Hu3DModelAttrSet(obj->mdlId[lbl_1_bss_1810[i].no + 0x45], 1);
                if (lbl_1_bss_1810[i].slot != -1) {
                    Hu3DModelAttrReset(obj->mdlId[lbl_1_bss_1810[i].slot + 0x58], 1);
                    Hu3DModelAttrReset(obj->mdlId[lbl_1_bss_1810[i].slot + 0x5C], 1);
                    Hu3DModelPosSet(obj->mdlId[lbl_1_bss_1810[i].slot + 0x5C],
                        lbl_1_bss_1810[i].pos.x, lbl_1_bss_1810[i].pos.y, lbl_1_bss_1810[i].pos.z);
                    Hu3DMotionTimeSet(obj->mdlId[lbl_1_bss_1810[i].slot + 0x5C], 0.0f);
                }
                if (lbl_1_bss_E0[3].num < 3) {
                    lbl_1_bss_E0[3].posX[lbl_1_bss_E0[3].num] = lbl_1_bss_1810[i].pos.x;
                    lbl_1_bss_E0[3].posZ[lbl_1_bss_E0[3].num] = lbl_1_bss_1810[i].pos.z;
                    lbl_1_bss_E0[3].num++;
                }
            }
            if (lbl_1_bss_1810[i].time >= 30 && lbl_1_bss_1810[i].time <= 40) {
                for (j = 0; j < 4; j++) {
                    dx = lbl_1_bss_1810[i].pos.x - lbl_1_bss_C8[j]->trans.x;
                    dz = lbl_1_bss_1810[i].pos.z - lbl_1_bss_C8[j]->trans.z;
                    if (((dx * dx) + (dz * dz)) <= 14400.0f && players[j]->mode == 2) {
                        t = (f32)(180.0 * (atan2(dx, dz) / 3.141592653589793));
                        /* Normalise the angle; the original keeps the magnitude too. */
                        if (t <= -180.0f) {
                            scale = 360.0f + t;
                        }
                        else if (t > 180.0f) {
                            scale = t - 360.0f;
                        }
                        else {
                            scale = t;
                        }
                        lbl_1_bss_C8[j]->rot.y = t;
                        players[j]->mode = 4;
                        players[j]->modeTime = 0;
                        players[j]->itemNo = i;
                    }
                }
            }
            if (lbl_1_bss_1810[i].time == 0x41) {
                if (lbl_1_bss_1810[i].slot != -1) {
                    lbl_1_data_124[lbl_1_bss_1810[i].slot] = 0;
                    Hu3DModelAttrSet(obj->mdlId[lbl_1_bss_1810[i].slot + 0x5C], 1);
                }
                lbl_1_bss_1810[i].state = 0;
            }
            break;
        case 3:
            /* Thrown: the item and its shadow arc over to the target player. */
            if (lbl_1_bss_1810[i].time == 0) {
                Hu3DModelPosSet(obj->mdlId[lbl_1_bss_1810[i].no + 0x50], lbl_1_bss_1810[i].pos.x, lbl_1_bss_1810[i].pos.y, lbl_1_bss_1810[i].pos.z);
                Hu3DModelRotSet(obj->mdlId[lbl_1_bss_1810[i].no + 0x50], 0.0f,
                    180.0f + lbl_1_bss_C8[owner]->rot.y, 0.0f);
                Hu3DMotionTimeSet(obj->mdlId[lbl_1_bss_1810[i].no + 0x50], 0.0f);
                Hu3DModelAttrReset(obj->mdlId[lbl_1_bss_1810[i].no + 0x50], 1);
            }
            if (lbl_1_bss_1810[i].time == 8 && lbl_1_bss_E0[4].num < 3) {
                lbl_1_bss_E0[4].posX[lbl_1_bss_E0[4].num] = lbl_1_bss_1810[i].pos.x;
                lbl_1_bss_E0[4].posZ[lbl_1_bss_E0[4].num] = lbl_1_bss_1810[i].pos.z;
                lbl_1_bss_E0[4].num++;
            }
            if (lbl_1_bss_1810[i].time >= 0x12 || lbl_1_bss_1810[i].time <= 0x50) {
                Hu3DModelAttrReset(obj->mdlId[owner + 0x60], 1);
                Hu3DModelPosSet(obj->mdlId[owner + 0x60],
                    (f32)((0.35 * (f64)lbl_1_bss_1810[i].pos.x) + (0.65 * (f64)lbl_1_bss_C8[owner]->trans.x)),
                    70.0f + lbl_1_bss_1810[i].pos.y,
                    (f32)(60.0 + ((0.35 * (f64)lbl_1_bss_1810[i].pos.z)
                        + (0.65 * (f64)lbl_1_bss_C8[owner]->trans.z))));
                Hu3DModelScaleSet(obj->mdlId[owner + 0x60], 0.4f, 0.4f, 0.4f);
                if (lbl_1_bss_1810[i].time == 0x12 || lbl_1_bss_1810[i].time == 0x28) {
                    Hu3DMotionTimeSet(obj->mdlId[owner + 0x60], 0.0f);
                }
            }
            else {
                Hu3DModelAttrSet(obj->mdlId[owner + 0x60], 1);
            }
            if (lbl_1_bss_1810[i].time >= 10 && lbl_1_bss_1810[i].time <= 0x5A) {
                axis = lbl_1_rodata_2B0;
                t = (f32)(lbl_1_bss_1810[i].time - 10) / 80.0f;
                scale = 1.0f - t;
                C_QUATRotAxisRad(&quat, &axis, 0.0f);
                Hu3DModelPosSet(obj->mdlId[i + 0x1D],
                    (t * lbl_1_bss_1810[i].pos.x) + (scale * lbl_1_bss_1810[i].holdPos.x),
                    (400.0f * (1.0f - (((2.0f * t) - 1.0f) * ((2.0f * t) - 1.0f))))
                        + ((t * lbl_1_bss_1810[i].pos.y) + (scale * lbl_1_bss_1810[i].holdPos.y)),
                    (t * lbl_1_bss_1810[i].pos.z) + (scale * lbl_1_bss_1810[i].holdPos.z));
                C_QUATSlerp(&lbl_1_bss_1810[i].quat, &quat, &quat, t);
                PSMTXQuat(mtx, &quat);
                Hu3DMtxRotGet(mtx, &axis);
                Hu3DModelRotSet(obj->mdlId[i + 0x1D], axis.x, axis.y, axis.z);
            }
            if (lbl_1_bss_1810[i].time == 0x5A) {
                Hu3DModelAttrSet(obj->mdlId[lbl_1_bss_1810[i].no + 0x50], 1);
                lbl_1_bss_1810[i].state = 0;
                if (lbl_1_bss_E0[5].num < 3) {
                    lbl_1_bss_E0[5].posX[lbl_1_bss_E0[5].num] = lbl_1_bss_1810[i].pos.x;
                    lbl_1_bss_E0[5].posZ[lbl_1_bss_E0[5].num] = lbl_1_bss_1810[i].pos.z;
                    lbl_1_bss_E0[5].num++;
                }
            }
            break;
        }
        lbl_1_bss_1810[i].time++;
    }

    for (i = 0; i < 16; i++) {
        Hu3DModelAttrSet(lbl_1_bss_C4->mdlId[i + 0xD], 1);
    }
    Hu3DModelAttrSet(lbl_1_bss_C4->mdlId[8], 1);
    Hu3DModelRotSet(obj->mdlId[0x3A], 0.0f, 0.0f, 0.0f);

    /* Starfighters. */
    for (i = 0; i < 11; i++) {
        if (lbl_1_bss_1600[i].mode <= 6) {
            switch (lbl_1_bss_1600[i].mode) {
            case 1:
                /* Launching: shrink towards the vanishing point overhead. */
                scale = 1.6f * (1.0f - (lbl_1_bss_1600[i].pos.y / 1000.0f));
                lbl_1_bss_1600[i].pos.y += 4.0f;
                Hu3DModelPosSet(obj->mdlId[i + 0x3A], lbl_1_bss_1600[i].pos.x, lbl_1_bss_1600[i].pos.y, lbl_1_bss_1600[i].pos.z);
                Hu3DModelRotSet(obj->mdlId[i + 0x3A], 0.0f, 0.0f, 0.0f);
                Hu3DModelScaleSet(obj->mdlId[8], scale, scale, scale);
                Hu3DModelPosSet(obj->mdlId[8], lbl_1_bss_1600[i].pos.x, 1000.0f, lbl_1_bss_1600[i].pos.z);
                Hu3DModelRotSet(obj->mdlId[8], 0.0f, 0.0f, 0.0f);
                Hu3DModelAttrReset(lbl_1_bss_C4->mdlId[8], 1);
                if ((rand8() % 3) == 0) {
                    Hu3DModelAttrReset(obj->mdlId[i + 0x3A], 1);
                }
                if (lbl_1_bss_1600[i].pos.y > 1000.0f) {
                    lbl_1_bss_1600[i].mode = 0;
                    Hu3DModelAttrSet(obj->mdlId[i + 0x3A], 1);
                    HuAudFXStop(lbl_1_data_144);
                }
                break;
            case 3:
                /* Flying in: move along the orbit, shrinking the radius as it climbs. */
                dx = lbl_1_bss_1600[i].pos.x;
                dy = lbl_1_bss_1600[i].pos.y;
                dz = lbl_1_bss_1600[i].pos.z;
                lbl_1_bss_1600[i].pos.x = (f32)((f64)lbl_1_bss_1600[i].pos.x
                    - ((f64)lbl_1_bss_1600[i].dist * sin((3.141592653589793 * (f64)lbl_1_bss_1600[i].angle) / 180.0)));
                lbl_1_bss_1600[i].pos.z = (f32)((f64)lbl_1_bss_1600[i].pos.z
                    - ((f64)lbl_1_bss_1600[i].dist * cos((3.141592653589793 * (f64)lbl_1_bss_1600[i].angle) / 180.0)));
                n = (lbl_1_bss_1600[i].slot != 0) ? 2 : 1;
                lbl_1_bss_1600[i].angle = (f32)((f64)lbl_1_bss_1600[i].angle
                    + (360.0 * ((f64)(6.0f / (f32)n)
                        / (3.141592653589793 * (f64)lbl_1_bss_1600[i].dist))));
                wrap = 0;
                if (lbl_1_bss_1600[i].angle < -360.0f) {
                    wrap = 360;
                }
                else if (lbl_1_bss_1600[i].angle > 360.0f) {
                    wrap = -360;
                }
                lbl_1_bss_1600[i].angle += (f32)wrap;
                step = 0;
                if (lbl_1_bss_1600[i].pos.y > (lbl_1_bss_1600[i].unk20 - 100.0f) && lbl_1_bss_1600[i].dist > 20.0f) {
                    step = 1;
                }
                lbl_1_bss_1600[i].dist -= (f32)(step * 2);
                lbl_1_bss_1600[i].pos.x = (f32)((f64)lbl_1_bss_1600[i].pos.x
                    + ((f64)lbl_1_bss_1600[i].dist * sin((3.141592653589793 * (f64)lbl_1_bss_1600[i].angle) / 180.0)));
                lbl_1_bss_1600[i].pos.z = (f32)((f64)lbl_1_bss_1600[i].pos.z
                    + ((f64)lbl_1_bss_1600[i].dist * cos((3.141592653589793 * (f64)lbl_1_bss_1600[i].angle) / 180.0)));
                lbl_1_bss_1600[i].pos.y += 4.0f;
                dx = lbl_1_bss_1600[i].pos.x - dx;
                dy = lbl_1_bss_1600[i].pos.y - dy;
                dz = lbl_1_bss_1600[i].pos.z - dz;
                Hu3DModelPosSet(obj->mdlId[i + 0x3A], lbl_1_bss_1600[i].pos.x, lbl_1_bss_1600[i].pos.y, lbl_1_bss_1600[i].pos.z);
                if ((dx != 0.0f) || (dz != 0.0f)) {
                    Hu3DModelRotSet(obj->mdlId[i + 0x3A], 0.0f,
                        (f32)(180.0 * (atan2(dx, dz) / 3.141592653589793)), 0.0f);
                }
                if (lbl_1_bss_1600[i].pos.y > lbl_1_bss_1600[i].unk20) {
                    lbl_1_bss_1600[i].mode = 4;
                    lbl_1_bss_1600[i].time = 0;
                    if (i != 0xA && lbl_1_bss_E0[i + 9].seNo != -1) {
                        HuAudFXStop(lbl_1_bss_E0[i + 9].seNo);
                    }
                }
                break;
            case 4:
            case 6:
                /* Home in on the target point (owner position plus the orbit offset). */
                dx = (lbl_1_bss_1600[i].unk1C + lbl_1_bss_C8[lbl_1_bss_1600[i].owner]->trans.x) - lbl_1_bss_1600[i].pos.x;
                dy = lbl_1_bss_1600[i].unk20 - lbl_1_bss_1600[i].pos.y;
                dz = (lbl_1_bss_1600[i].unk24 + lbl_1_bss_C8[lbl_1_bss_1600[i].owner]->trans.z) - lbl_1_bss_1600[i].pos.z;
                len = sqrtf((dz * dz) + ((dx * dx) + (dy * dy)));
                step = (len > 1.0f) ? ((len - 1.0f) * 0.125f) : len;
                if (step > 0.0f) {
                    lbl_1_bss_1600[i].pos.x += step * (dx / len);
                    lbl_1_bss_1600[i].pos.y += step * (dy / len);
                    lbl_1_bss_1600[i].pos.z += step * (dz / len);
                }
                Hu3DModelPosSet(obj->mdlId[i + 0x3A], lbl_1_bss_1600[i].pos.x, lbl_1_bss_1600[i].pos.y, lbl_1_bss_1600[i].pos.z);
                if ((dx != 0.0f) || (dz != 0.0f)) {
                    Hu3DModelRotSet(obj->mdlId[i + 0x3A], 0.0f,
                        (f32)(180.0 * (atan2(dx, dz) / 3.141592653589793)), 0.0f);
                }
                if (lbl_1_bss_1600[i].mode == 6 && lbl_1_data_138[0] != 4
                    && lbl_1_data_138[0] != -1 && lbl_1_data_138[0] != lbl_1_bss_1600[i].owner) {
                    /* Keep a ring around the winner so the fighters do not overlap. */
                    t = lbl_1_bss_C8[lbl_1_data_138[0]]->trans.x;
                    dy = 40.0f + lbl_1_bss_C8[lbl_1_data_138[0]]->trans.y;
                    dz = lbl_1_bss_C8[lbl_1_data_138[0]]->trans.z;
                    dx = lbl_1_bss_1600[i].pos.x - t;
                    base = (lbl_1_bss_1600[i].pos.y - dy) * 0.5f;
                    len = (lbl_1_bss_1600[i].pos.z - dz) / 1.5f;
                    scale = sqrtf((len * len) + ((dx * dx) + (base * base)));
                    if (scale < 90.0f) {
                        if ((dx == 0.0f) && (len == 0.0f)) {
                            dx = (f32)(((rand8() % 2) * 2) - 1);
                            len = 0.0f;
                        }
                        lbl_1_bss_1600[i].pos.x = t + (90.0f * (dx / scale));
                        lbl_1_bss_1600[i].pos.y = dy + (2.0f * (90.0f * (base / scale)));
                        lbl_1_bss_1600[i].pos.z = dz + (1.5f * (90.0f * (len / scale)));
                    }
                }
                break;
            case 5:
                /* Circling the winner, rising with the victory animation. */
                scale = (players[lbl_1_data_138[0]]->modeTime < 0x53) ? 0.5f : 1.0f;
                dx = ((scale * lbl_1_bss_1600[i].unk1C) + lbl_1_bss_C8[lbl_1_bss_1600[i].owner]->trans.x) - lbl_1_bss_1600[i].pos.x;
                dy = (scale * lbl_1_bss_1600[i].unk20) - lbl_1_bss_1600[i].pos.y;
                dz = (lbl_1_bss_1600[i].unk24 + lbl_1_bss_C8[lbl_1_bss_1600[i].owner]->trans.z) - lbl_1_bss_1600[i].pos.z;
                len = sqrtf((dz * dz) + ((dx * dx) + (dy * dy)));
                step = (len > 32.0f) ? 4.0f : (len * 0.125f);
                if (step > 0.0f) {
                    lbl_1_bss_1600[i].pos.x += step * (dx / len);
                    lbl_1_bss_1600[i].pos.y += step * (dy / len);
                    lbl_1_bss_1600[i].pos.z += step * (dz / len);
                }
                if (players[lbl_1_data_138[0]]->modeTime < 0xAF) {
                    base = 0.0f;
                }
                else if (players[lbl_1_data_138[0]]->modeTime < 0xD7) {
                    base = 6.0f * ((f32)(players[lbl_1_data_138[0]]->modeTime - 0xAF) / 40.0f)
                        * ((f32)players[lbl_1_data_138[0]]->score / 3.0f);
                }
                else {
                    base = 6.0f * ((f32)players[lbl_1_data_138[0]]->score / 3.0f);
                }
                lbl_1_bss_1600[i].pos.y += base;
                if (players[lbl_1_data_138[0]]->modeTime < 0xAF) {
                    scale = 0.0f;
                }
                else if (players[lbl_1_data_138[0]]->modeTime < 0xD7) {
                    scale = 6.0f * ((f32)(players[lbl_1_data_138[0]]->modeTime - 0xAF) / 40.0f)
                        * ((f32)players[lbl_1_data_138[0]]->score / 3.0f);
                }
                else {
                    scale = 6.0f * ((f32)players[lbl_1_data_138[0]]->score / 3.0f);
                }
                lbl_1_bss_1600[i].unk20 += scale;
                Hu3DModelPosSet(obj->mdlId[i + 0x3A], lbl_1_bss_1600[i].pos.x, lbl_1_bss_1600[i].pos.y, lbl_1_bss_1600[i].pos.z);
                Hu3DModelRotSet(obj->mdlId[i + 0x3A], 0.0f, 0.0f, 0.0f);
                break;
            }
        }
        lbl_1_bss_1600[i].time++;
        if (castCount[lbl_1_bss_1600[i].owner] < 4 && lbl_1_bss_1600[i].mode == 5
            && players[lbl_1_data_138[0]]->modeTime >= 0x3C) {
            /* One shadow caster per fighter, round-robin over the four slots.
               The two per-character offset tables are block locals in the
               original, so each pass re-copies them out of .rodata. */
            s32 charOffY[10] = { -10, 0, 30, -50, -10, -35, 60, -70, -55, -120 };
            s32 charOffZ[10] = { 0, 0, 0, -15, -35, 0, -20, -5, 0, -30 };

            n = (lbl_1_bss_1600[i].owner * 4) + castCount[lbl_1_bss_1600[i].owner] + 0xD;
            lbl_1_bss_C++;
            lbl_1_bss_C = lbl_1_bss_C % 4;
            dx = lbl_1_bss_C8[lbl_1_bss_1600[i].owner]->trans.x;
            t = 0.0f;
            if (lbl_1_bss_BC >= 1005 && players[lbl_1_bss_1600[i].owner]->charNoClamp == 6) {
                t = 1.0f;
            }
            dy = 80.0f + lbl_1_bss_C8[lbl_1_bss_1600[i].owner]->trans.y
                + (f32)charOffY[players[lbl_1_bss_1600[i].owner]->charNoClamp] - t;
            m = 0.0f;
            if (lbl_1_bss_BC >= 1005 && players[lbl_1_bss_1600[i].owner]->charNoClamp == 7) {
                m = 1.0f;
            }
            dz = (lbl_1_bss_C8[lbl_1_bss_1600[i].owner]->trans.z - 30.0f)
                + (f32)charOffZ[players[lbl_1_bss_1600[i].owner]->charNoClamp] - m;
            t = lbl_1_bss_1600[i].pos.x - dx;
            dy = lbl_1_bss_1600[i].pos.y - dy;
            dz = lbl_1_bss_1600[i].pos.z - dz;
            Hu3DModelPosSet(lbl_1_bss_C4->mdlId[n], dx, 80.0f
                + lbl_1_bss_C8[lbl_1_bss_1600[i].owner]->trans.y
                + (f32)charOffY[players[lbl_1_bss_1600[i].owner]->charNoClamp] - t,
                lbl_1_bss_C8[lbl_1_bss_1600[i].owner]->trans.z - 30.0f
                + (f32)charOffZ[players[lbl_1_bss_1600[i].owner]->charNoClamp] - m);
            Hu3DModelRotSet(lbl_1_bss_C4->mdlId[n],
                (f32)(180.0 * (atan2(dy, sqrtf((t * t) + (dz * dz))) / 3.141592653589793)),
                (f32)(180.0 + (180.0 * (atan2(t, dz) / 3.141592653589793))), 0.0f);
            Hu3DModelScaleSet(lbl_1_bss_C4->mdlId[n], 1.0f, 1.0f,
                sqrtf((dz * dz) + ((t * t) + (dy * dy))) / 150.0f);
            Hu3DModelAttrReset(lbl_1_bss_C4->mdlId[n], 1);
            castCount[lbl_1_bss_1600[i].owner]++;
        }
    }

    for (i = 0; i < 4; i++) {
        lbl_1_data_124[i] += (lbl_1_data_124[i] > 0);
    }

    /* Dust puffs age and fade. */
    for (i = 0; i < 128; i++) {
        if (lbl_1_bss_600[i].active != 0) {
            lbl_1_bss_600[i].active++;
            lbl_1_bss_600[i].pos.x += lbl_1_bss_600[i].vel.x;
            lbl_1_bss_600[i].pos.y += lbl_1_bss_600[i].vel.y;
            lbl_1_bss_600[i].pos.z += lbl_1_bss_600[i].vel.z;
            lbl_1_bss_600[i].vel.x *= 0.85f;
            lbl_1_bss_600[i].vel.y *= 0.85f;
            lbl_1_bss_600[i].vel.z *= 0.85f;
            lbl_1_bss_600[i].alpha = (s32)((f32)lbl_1_bss_600[i].alpha * 0.95f);
            if (lbl_1_bss_600[i].alpha <= 0) {
                lbl_1_bss_600[i].active = 0;
            }
        }
    }

    /* Camera: pull back during the intro, then follow the winner. */
    if (lbl_1_bss_BC == 1002 || (lbl_1_bss_BC == 1003 && CZoomM[1] != 2472.0f)) {
        CZoomM[1] = 77.25f + (0.96875f * CZoomM[1]);
        CRotM[1].x = (f32)((0.96875f * CRotM[1].x) - 1.01875);
        CenterM[1].y = 2.625f + (0.96875f * CenterM[1].y);
    }
    else if (lbl_1_bss_BC == 1005 && lbl_1_data_138[0] != 4) {
        if (lbl_1_data_258 == -10000.0f) {
            lbl_1_data_258 = lbl_1_bss_C8[lbl_1_data_138[0]]->trans.z - 25.0f;
        }
        CZoomM[1] = 112.5f + (0.875f * CZoomM[1]);
        CenterM[1].x = (0.875f * CenterM[1].x)
            + (0.125f * lbl_1_bss_C8[lbl_1_data_138[0]]->trans.x);
        CenterM[1].y = (0.875f * CenterM[1].y)
            + (0.125f * ((866.0f * (f32)players[lbl_1_data_138[0]]->score) / 3.0f));
        CenterM[1].z = (0.875f * CenterM[1].z)
            + (0.125f * (lbl_1_data_258
                + ((500.0f * (f32)players[lbl_1_data_138[0]]->score) / 3.0f)));
        CRotM[1].x = (0.9375f * CRotM[1].x) - 3.75f;
    }

    for (i = 0; i < 29; i++) {
        if (lbl_1_bss_1810[i].kind == 0
            || (lbl_1_bss_1810[i].state != 0 && lbl_1_bss_1810[i].state != 4)) {
            Hu3DModelShadowDispOff(obj->mdlId[i + 0x1D]);
        }
        else {
            Hu3DModelShadowDispOn(obj->mdlId[i + 0x1D]);
        }
    }
}

/* Main flow of the minigame: sound queue, start/end messages, winner selection. */
// NONMATCHING: the frame, the five scalar stack slots and the home registers
// now line up, but MWCC still picks the opposite scratch pair (r4/r5) when
// evaluating the two short-circuited conditions, and folds the sign-extended
// charNo load into its destination register instead of going through r0:
// 151 of 1025 rows differ, 101 more only pick a different label for the same
// literal.
void fn_1_8F1C(OMOBJ *obj)
{
    M566Player *players[4];
    s32 i;
    s32 j;
    s32 seNo;
    s32 playerNo;

    for (i = 0; i < 4; i++) {
        players[i] = lbl_1_bss_C8[i]->data;
    }
    for (i = 0; i < 41; i++) {
        s32 seTbl[41] = { 0x8E1, 0x8E2, 0x8E4, 0x8E6, 0x2D8, 0x8E8, 0x8E9, 0x2C4, 0x2D7 };
        s32 vol;

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
            s32 vol;

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
            (void)(rand8() % 4);
            for (; i < 4; i++) {
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
            OSReport(lbl_1_data_284, lbl_1_bss_BC);
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
// NONMATCHING: the model indices were wrong by a factor of two and are fixed
// (hooks on mdlId[5]/[6]/[7], the linked shadow on mdlId[8], as the original
// does). What is left is the local-slot layout and the volatile register
// choices around the Hu3DModelCreateData loops, 611 of 1310 rows.
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
    obj->mdlId[5] = Hu3DHookFuncCreate(fn_1_9F20);
    Hu3DModelLayerSet(obj->mdlId[5], 4);
    Hu3DModelCameraSet(obj->mdlId[5], HU3D_CAM1);
    obj->mdlId[6] = Hu3DHookFuncCreate(fn_1_A654);
    Hu3DModelLayerSet(obj->mdlId[6], 2);
    Hu3DModelCameraSet(obj->mdlId[6], HU3D_CAM1);
    obj->mdlId[7] = Hu3DHookFuncCreate(fn_1_C06C);
    Hu3DModelLayerSet(obj->mdlId[7], 0);
    Hu3DModelCameraSet(obj->mdlId[7], HU3D_CAM1);
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
    obj->mdlId[8] = Hu3DModelLink(obj->mdlId[58]);
    Hu3DModelShadowSet(obj->mdlId[8]);
    Hu3DModelLayerSet(obj->mdlId[8], 1);
    Hu3DModelCameraSet(obj->mdlId[8], HU3D_CAM1);
    Hu3DModelAttrSet(obj->mdlId[8], HU3D_ATTR_DISPOFF);
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

        player->rumbleF = (GWRumbleFGet() == 0) ? 0 : ((player->padNo == -1) ? 1 : (rumbleMask[player->padNo] & HuPadRumbleGet()));
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
    Vec lightPos;
    Vec lightDir;
    Vec shadowPos;
    Vec shadowUp;
    Vec shadowTarget;
    GXColor lightColor;
    s32 i;
    OMOBJ *obj;

    OSReport(lbl_1_data_29F);
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
    lightPos = lbl_1_rodata_7E8;
    lightDir = lbl_1_rodata_7F4;
    lightColor = lbl_1_rodata_800;
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
