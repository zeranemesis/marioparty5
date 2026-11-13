#include "REL/m525Dll.h"

#include "datadir_enum.h"

#include "math.h"
#include "humath.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

int rand8(void);

void ObjectSetup(void);

/*
 * M525: one player swings on a giant pendulum (player no 0, "swinger") and
 * throws items at the three others ("runners", no 1..3) who dodge and jump
 * on the platforms.  Work structures of this unit:
 *
 *  M525Player  (0xC4)    OMOBJ::data of the four player objects (bss 8C[4])
 *  M525Swing   (0x14)    pendulum state (angle, angular speed/accel, pivot Y)
 *  M525Item    (0x1C x 20)  items thrown by the swinger (bss 316C)
 *  M525Fx      (0x30 x 256) confetti particles drawn by fn_1_7C60 (bss 16C)
 *  M525Se      (0x14 x 10)  sound requests queued by the game logic, played
 *                           once per frame by fn_1_889C (bss A4)
 *
 * Game mode (lbl_1_bss_80): 0x3E8 wipe in, 0x3E9 / 0x3EA / 0x3EB intro,
 * 0x3EC "start" message, 0x3ED play, 0x3EE finish, 0x3EF result.
 */
typedef struct M525Player_s {
    /* 0x00 */ s32 no;
    /* 0x04 */ s32 playerNo;
    /* 0x08 */ s32 charNoClamp;
    /* 0x0C */ s32 charNo;
    /* 0x10 */ s32 padNo;
    /* 0x14 */ s32 dif;
    /* 0x18 */ s32 motNo;
    /* 0x1C */ s32 nextMotNo;
    /* 0x20 */ s32 jumpHold;
    /* 0x24 */ s32 grounded;
    /* 0x28 */ s32 mode;
    /* 0x2C */ s32 modeTime;
    /* 0x30 */ s32 down;
    /* 0x34 */ s32 swingTime;
    /* 0x38 */ s32 itemNo;
    /* 0x3C */ s32 itemBank;
    /* 0x40 */ s32 throwTime;
    /* 0x44 */ s32 throwState;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ float startX;
    /* 0x50 */ float startY;
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ float footX[3];
    /* 0x70 */ float footY[3];
    /* 0x7C */ s32 footNo;
    /* 0x80 */ s32 dir;
    /* 0x84 */ s32 prevDir;
    /* 0x88 */ s32 turnTime;
    /* 0x8C */ float rankOfs;
    /* 0x90 */ s32 shake;
    /* 0x94 */ s32 unk94;
    /* 0x98 */ s32 unk98;
    /* 0x9C */ float speed;
    /* 0xA0 */ float velX;
    /* 0xA4 */ float velY;
    /* 0xA8 */ float velZ;
    /* 0xAC */ s32 aiState;
    /* 0xB0 */ s32 aiTarget;
    /* 0xB4 */ s32 aiOfs;
    /* 0xB8 */ s32 aiWait;
    /* 0xBC */ float aiX;
    /* 0xC0 */ s16 lightId;
} M525Player; /* size = 0xC4 */

typedef struct M525Swing_s {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ float angle;
    /* 0x08 */ float speed;
    /* 0x0C */ float accel;
    /* 0x10 */ float pivotY;
} M525Swing; /* size = 0x14 */

typedef struct M525Item_s {
    /* 0x00 */ s32 time;
    /* 0x04 */ float x;
    /* 0x08 */ float y;
    /* 0x0C */ float velX;
    /* 0x10 */ float velY;
    /* 0x14 */ s32 hit;
    /* 0x18 */ s16 lightId;
} M525Item; /* size = 0x1C */

typedef struct M525Fx_s {
    /* 0x00 */ s32 time;
    /* 0x04 */ s32 kind;
    /* 0x08 */ float x;
    /* 0x0C */ float y;
    /* 0x10 */ float z;
    /* 0x14 */ float velX;
    /* 0x18 */ float velY;
    /* 0x1C */ float velZ;
    /* 0x20 */ Vec axis;
    /* 0x2C */ float rotSpeed;
} M525Fx; /* size = 0x30 */

typedef struct M525Se_s {
    /* 0x00 */ s32 num;
    /* 0x04 */ float pan[3];
    /* 0x10 */ s32 seNo;
} M525Se; /* size = 0x14 */

typedef struct M525Esprite_s {
    s16 memberNo;
    s16 animNo;
} M525ESPRITE;

typedef struct M525EspAnim_s {
    unsigned int dataNum;
    u16 useCnt;
    ANIMDATA *anim;
} M525ESPANIM;

/* esprite.c keeps these private; the minigame reads the sprite animation directly. */
SHARED_SYM extern M525ESPRITE esprite[];
SHARED_SYM extern M525ESPANIM espanim[];
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);

static float fn_1_5930(float t, float a, float b, float c);
static void fn_1_7698(HU3DMODEL *model, Mtx *mtx);
static void fn_1_7C60(HU3DMODEL *model, Mtx *mtx);
static void fn_1_8394(HU3DMODEL *model, Mtx *mtx);
static void fn_1_654(OMOBJ *obj);
static void fn_1_5AF4(OMOBJ *obj);
static void fn_1_889C(OMOBJ *obj);
static void fn_1_A690(OMOBJ *obj);
static void fn_1_B930(OMOBJ *obj);
static void fn_1_BDB4(OMOBJ *obj);
static void fn_1_C51C(OMOBJ *obj);
static void fn_1_C574(OMOBJ *obj);

static s32 lbl_1_data_0[13] = { 0x00890000, 0x00300000, 0x00B40000, 0x00EA0000, 0x00E30000, 0x00140000, 0x00DD0000, 0x002A0000, 0x00C60000, 0x00AD0000, 0x00000000, 0x00000000, 0x00000000 };

/* Character motion ids: [0] swinger, [1] runners. */
static u32 lbl_1_data_34[2][10] = {
    { 0x008C0000, 0x00870095, 0x008C0003, 0x008C0004, 0x00870096, 0x008C0006, 0, 0, 0, 0 },
    { 0x008C0000, 0x00870093, 0x008C0003, 0x008C0004, 0x008C002B, 0x008C0006, 0x008C0031, 0x00870094, 0x008C0019, 0x008C000E },
};

/* Per motion: 0 = looped, 1 = played once (attr), shift time. */
static u8 lbl_1_data_84[2][10] = {
    { 0, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
    { 0, 0, 1, 1, 0, 1, 1, 0, 0, 1 },
};

static u8 lbl_1_data_98[2][10] = {
    { 8, 15, 0, 3, 2, 8, 0, 0, 0, 0 },
    { 8, 0, 0, 8, 0, 8, 8, 0, 15, 4 },
};

static s32 lbl_1_data_AC[2][10] = {
    { 0, 0, 0, 8, 2, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 5, 0, 0, 0, 0, 0, 60 },
};

/* Winner: -1 none, 0 swinger, 1 runners, 4 draw. */
static s32 lbl_1_data_FC = -1;
static s32 lbl_1_data_100 = -1;
static s32 lbl_1_data_104 = 1;

/* Ground profile (x / height) built every frame from the runners' feet. */
static float lbl_1_data_108[11] = { -500.0f, 500.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
static float lbl_1_data_134[11] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

/* Start position (x, y, z, rotY) of each player. */
static float lbl_1_data_160[4][4] = {
    { 0.0f, 700.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f },
    { -310.0f, 0.0f, 0.0f, 0.0f },
    { 310.0f, 0.0f, 0.0f, 0.0f },
};
static float lbl_1_data_1A0 = 1.0f;

static s32 lbl_1_data_1A4[5] = { 0, 0, 0, 0, 0 };

static float lbl_1_bss_0;
static s32 lbl_1_bss_4[2];
static s32 lbl_1_bss_C;
static s32 lbl_1_bss_10[2];
static s32 lbl_1_bss_18[2];
static float lbl_1_bss_20;
static float lbl_1_bss_24;
static float lbl_1_bss_28;
static float lbl_1_bss_2C;
static float lbl_1_bss_30;
static Vec lbl_1_bss_34;
static Vec lbl_1_bss_40;
static Vec lbl_1_bss_4C;
static s32 lbl_1_bss_58;
static float lbl_1_bss_5C[6];
static s32 lbl_1_bss_74;
static s16 lbl_1_bss_78;
static s16 lbl_1_bss_7A;
static OMOBJMAN *lbl_1_bss_7C;
static s32 lbl_1_bss_80;
static OMOBJ *lbl_1_bss_84;
static OMOBJ *lbl_1_bss_88;
static OMOBJ *lbl_1_bss_8C[4];
static OMOBJ *lbl_1_bss_9C;
static OMOBJ *lbl_1_bss_A0;
static M525Se lbl_1_bss_A4[10];
static M525Fx lbl_1_bss_16C[256];
static M525Item lbl_1_bss_316C[20];
static M525Swing lbl_1_bss_339C;
static float lbl_1_bss_33A0[4];

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

/* Debug camera: moves the two cameras with the first pad. */
void fn_1_A0(void)
{
    Vec pos;
    Vec target;
    Vec up;
    float rotX;
    float rotY;
    float rotZ;
    s32 i;

    for (i = 0; i < 2; i++) {
        CenterM[i].x += 25.0 * (((HuPadBtn[0] & PAD_BUTTON_RIGHT) != 0) - ((HuPadBtn[0] & PAD_BUTTON_LEFT) != 0));
        CenterM[i].y += 25.0 * (((HuPadBtn[0] & PAD_BUTTON_UP) != 0) - ((HuPadBtn[0] & PAD_BUTTON_DOWN) != 0));
        CZoomM[i] += 25.0 * (((HuPadBtn[0] & PAD_BUTTON_X) != 0) - ((HuPadBtn[0] & PAD_BUTTON_Y) != 0));
        CRotM[i].y += HuPadSubStkX[0] / 32.0;
        CRotM[i].x += HuPadSubStkY[0] / -32.0;
        rotX = CRotM[i].x;
        rotY = CRotM[i].y;
        rotZ = CRotM[i].z;
        pos.x = CenterM[i].x + (CZoomM[i] * (sin((M_PI * rotY) / 180.0) * cos((M_PI * rotX) / 180.0)));
        pos.y = CenterM[i].y + (CZoomM[i] * -sin((M_PI * rotX) / 180.0));
        pos.z = CenterM[i].z + (CZoomM[i] * (cos((M_PI * rotY) / 180.0) * cos((M_PI * rotX) / 180.0)));
        target.x = CenterM[i].x;
        target.y = CenterM[i].y;
        target.z = CenterM[i].z;
        up.x = sin((M_PI * rotY) / 180.0) * sin((M_PI * rotX) / 180.0);
        up.y = cos((M_PI * rotX) / 180.0);
        up.z = cos((M_PI * rotY) / 180.0) * sin((M_PI * rotX) / 180.0);
    }
}

/* Per-player update (player 0: the swinger on the pendulum, 1..3: runners). Reads the pad or the CPU AI,
   moves the object, throws/dodges items and picks the motion. */
static void fn_1_654(OMOBJ *obj)
{
    M525Player *data;
    M525Player *player[4];
    float stkX;
    float stkY;
    float btnHold;
    float jump;
    float throwB;
    float groundY;
    s16 mot;
    s32 row;
    s32 i;
    s32 j;
    s32 k;

    data = obj->data;
    stkX = 0.0f;
    stkY = 0.0f;
    btnHold = 0.0f;
    jump = 0.0f;
    throwB = 0.0f;
    mot = -1;
    groundY = 0.0f;
    row = data->no != 0;
    for (i = 0; i < 4; i++) {
        player[i] = lbl_1_bss_8C[i]->data;
    }
    lbl_1_data_AC[1][8] = (data->no * 60) % 120;
    if (data->motNo != 2) {
        data->velZ = 0.0f;
        data->velX = 0.0f;
    }
    data->modeTime++;
    data->swingTime++;
    if (data->no == 0) {
        float fade;
        float prevSpeed;
        s32 sgn;
        s32 sgnNew;
        s32 sgnOld;

        if (data->unk58 <= 0) {
            fade = 1.0f;
        } else {
            float f;

            if (data->unk58 <= 0x1C2) {
                f = 1.0f - ((f32)data->unk58 / 450.0f);
            } else {
                f = 0.0f;
            }
            fade = f;
        }
        data->unk60++;
        prevSpeed = lbl_1_bss_339C.speed;
        lbl_1_bss_339C.angle = lbl_1_bss_339C.angle + lbl_1_bss_339C.speed;
        if (lbl_1_bss_339C.angle < 0.0f) {
            sgn = 1;
        } else {
            sgn = -1;
        }
        lbl_1_bss_339C.speed = lbl_1_bss_339C.speed + ((f32)(lbl_1_bss_80 >= 0x3EB) * (lbl_1_bss_339C.accel * (f32)sgn));
        if (lbl_1_bss_339C.speed < 0.0f) {
            sgnNew = -1;
        } else {
            sgnNew = (lbl_1_bss_339C.speed > 0.0f) ? 1 : 0;
        }
        if (prevSpeed < 0.0f) {
            sgnOld = -1;
        } else {
            sgnOld = (prevSpeed > 0.0f) ? 1 : 0;
        }
        if (sgnOld != sgnNew) {
            data->swingTime = 0;
            data->unk60 = 0;
            data->unk5C = (rand8() % ((data->dif == 2) ? 8 : 4)) == 0;
            if (data->padNo == -1 && data->mode == 2) {
                float bestWidth = 10000.0f;
                float bestNo = 0.0f;
                float bestTarget = 0.0f;
                float lo;
                float hi;

                for (i = 0; i < 3; i++) {
                    lo = -500.0f;
                    hi = 500.0f;
                    for (j = 1; j < 3; j++) {
                        s32 c;
                        float v;

                        c = 0;
                        if ((lbl_1_bss_8C[i + 1]->trans.x > lbl_1_bss_8C[((i + j) % 3) + 1]->trans.x) && (lo < lbl_1_bss_8C[((i + j) % 3) + 1]->trans.x)) {
                            c = 1;
                        }
                        if (c != 0) {
                            v = lbl_1_bss_8C[((i + j) % 3) + 1]->trans.x;
                        } else {
                            v = lo;
                        }
                        lo = v;
                        c = 0;
                        if ((lbl_1_bss_8C[i + 1]->trans.x < lbl_1_bss_8C[((i + j) % 3) + 1]->trans.x) && (hi > lbl_1_bss_8C[((i + j) % 3) + 1]->trans.x)) {
                            c = 1;
                        }
                        if (c != 0) {
                            v = lbl_1_bss_8C[((i + j) % 3) + 1]->trans.x;
                        } else {
                            v = hi;
                        }
                        hi = v;
                    }
                    if (((hi - lo) < bestWidth) && (player[i + 1]->down == 0)) {
                        f64 d;
                        s32 t;

                        bestWidth = hi - lo;
                        bestNo = (f32)(i + 1);
                        d = fabs((f64)(lbl_1_bss_8C[i + 1]->trans.x - lbl_1_bss_8C[((i + 2) % 3) + 1]->trans.x));
                        if (fabs((f64)(lbl_1_bss_8C[i + 1]->trans.x - lbl_1_bss_8C[((i + 1) % 3) + 1]->trans.x)) < d) {
                            t = i + 1;
                        } else {
                            t = i + 2;
                        }
                        bestTarget = (f32)t;
                    }
                }
                data->aiWait = -1;
                if (data->dif == 4) {
                    data->aiWait = (rand8() % 12) + 15;
                }
                data->aiTarget = (s32)bestNo;
                data->aiOfs = (s32)((150.0f * ((f32)rand8() / 255.0f)) - 75.0f);
            }
        }
        Hu3DModelRotSet(lbl_1_bss_88->mdlId[2], 0.0f, 0.0f, lbl_1_bss_339C.angle * fade);
        Hu3DModelRotSet(lbl_1_bss_88->mdlId[3], 0.0f, 0.0f, lbl_1_bss_339C.angle * fade);
        data->throwTime++;
        switch (data->mode) {
        case 1: {
            s32 c = 1;

            if (data->throwTime != 0 && data->throwTime < 0x1E) {
                c = 0;
            }
            mot = (c != 0) ? 1 : 4;
            break;
        }
        case 2:
            if (lbl_1_bss_80 == 0x3EB) {
                if ((lbl_1_bss_0 < 120.0f) && (lbl_1_bss_339C.speed > 0.0f) && (data->throwTime >= 0xB) && (obj->trans.x > 120.0f) && (obj->trans.x < 220.0f)) {
                    jump = 1.0f;
                }
                if ((lbl_1_bss_0 < 120.0f) && (lbl_1_bss_339C.speed > 0.0f) && (data->throwTime >= 0xB) && (obj->trans.x > -180.0f) && (obj->trans.x < -80.0f)) {
                    jump = 1.0f;
                }
            } else if (lbl_1_bss_80 == 0x3ED) {
                if (data->padNo != -1) {
                    jump = (f32)((HuPadBtnDown[data->padNo] & 0x100) > 0);
                    throwB = (f32)((HuPadBtnDown[data->padNo] & 0x200) > 0);
                } else {
                    s32 tblA[4] = { 17, 14, 12, 12 };
                    s32 tblB[4] = { 4, 4, 2, 2 };
                    s32 dir;

                    if (lbl_1_bss_339C.speed < 0.0f) {
                        dir = -1;
                    } else {
                        dir = 1;
                    }
                    if (data->aiWait == -1 && data->dif >= 4) {
                        data->aiWait = 0;
                    } else if (data->aiWait == -1) {
                        float ang = lbl_1_bss_339C.angle;
                        float spd = lbl_1_bss_339C.speed;
                        s32 s;

                        for (i = 0; (f32)i < ((f32)((tblA[data->dif] + (tblB[data->dif] / 2)) * 4) * 0.5f); i++) {
                            ang += spd;
                            if (ang < 0.0f) {
                                s = 1;
                            } else {
                                s = -1;
                            }
                            spd += lbl_1_bss_339C.accel * (f32)s;
                        }
                        if (((f32)dir * ((f32)data->aiOfs + lbl_1_bss_8C[data->aiTarget]->trans.x)) < ((f64)dir * (800.0 * sin((M_PI * (f64)ang) / 180.0)))) {
                            data->aiWait = 0;
                        }
                    }
                    for (i = 0; i < 5; i++) {
                        if (lbl_1_data_1A4[i] == 1) {
                            jump = 1.0f;
                        }
                        lbl_1_data_1A4[i] -= lbl_1_data_1A4[i] > 0;
                    }
                    if (data->aiWait == 0) {
                        if (data->dif >= 4) {
                            if ((lbl_1_bss_339C.speed < 0.0f) && (data->itemNo < 5)) {
                                throwB = 1.0f;
                                lbl_1_data_1A4[data->itemNo] = ((0x50 - data->unk60) * 2) + (rand8() % 7) + 7;
                            }
                        } else if (data->dif >= 2) {
                            throwB = (f32)data->unk5C;
                            jump = (f32)((throwB != 0.0f) == 0);
                        } else {
                            jump = 1.0f;
                        }
                        data->aiWait = tblA[data->dif] + (rand8() % tblB[data->dif]);
                    }
                    data->aiWait = data->aiWait - (data->aiWait > 0);
                }
            }
            if ((data->throwState == 0) && ((lbl_1_bss_339C.speed * lbl_1_bss_339C.angle) > 0.0f) && (fabs((f64)lbl_1_bss_339C.speed) < (10.0f * lbl_1_bss_339C.accel)) && (lbl_1_data_FC != 1) && (lbl_1_bss_80 == 0x3EE)) {
                if ((lbl_1_bss_0 >= 60.0f) && (lbl_1_data_FC == 0)) {
                    s32 a;
                    s32 b;

                    data->throwState = 1;
                    a = 0;
                    b = 0;
                    if (lbl_1_bss_80 == 0x3EE) {
                        if (lbl_1_bss_0 >= 60.0f) {
                            b = 1;
                        }
                    }
                    if ((b != 0) && (lbl_1_data_FC == 0)) {
                        a = 1;
                    }
                    data->unk48 = (a != 0) ? 1 : 0;
                    data->startX = obj->trans.x;
                    data->startY = obj->trans.y - 75.0f;
                    Hu3DModelHookReset(obj->mdlId[0]);
                    if (data->itemNo < 5) {
                        Hu3DModelAttrSet(lbl_1_bss_88->mdlId[data->itemNo + (data->itemBank * 5) + 0x17], 1);
                    }
                }
            }
            if (data->throwState == -1) {
                s32 idx = data->itemNo + (data->itemBank * 5) + 0x17;

                if (data->itemNo < 5) {
                    Hu3DModelAttrReset(lbl_1_bss_88->mdlId[idx], 1);
                }
                data->throwState = 0;
            }
            if (data->throwState > 0) {
                float p;
                s32 sg;

                if (data->throwState <= 0x17) {
                    p = (f32)data->throwState / 23.0f;
                } else {
                    float q;

                    if (data->throwState <= 0x1B) {
                        q = 1.0f;
                    } else {
                        q = 1.0f - ((f32)(data->throwState - 0x1B) / 23.0f);
                    }
                    p = q;
                }
                if (lbl_1_bss_339C.angle < 0.0f) {
                    sg = -1;
                } else {
                    sg = 1;
                }
                obj->trans.x = (f32)(((1.0f - p) * data->startX) + (p * (f32)(sg * ((data->unk48 * 0x4B) + 0x28A))));
                obj->trans.y = (f32)(125.0 * (1.0 - ((((f64)p - 0.5) * ((f64)p - 0.5)) / 0.25)));
                obj->trans.y = (f32)(obj->trans.y + ((800.0f * p) + ((1.0f - p) * data->startY)));
                if (lbl_1_bss_339C.angle < 0.0f) {
                    sg = -1;
                } else {
                    sg = 1;
                }
                obj->rot.y = (f32)(((data->throwState <= 0x1B) ? 1 : -1) * (sg * 0x5A));
                obj->rot.z = 0.0f;
                mot = (p != 1.0f) ? 2 : 3;
                data->throwState++;
                if (data->throwState == 0x32) {
                    s32 idx = data->itemNo + (data->itemBank * 5) + 0x17;

                    if (data->itemNo < 5) {
                        Hu3DModelPosSet(lbl_1_bss_88->mdlId[idx], 0.0f, 0.0f, 0.0f);
                        Hu3DModelScaleSet(lbl_1_bss_88->mdlId[idx], 1.0833334f, 1.0833334f, 1.0833334f);
                        Hu3DModelHookSet(obj->mdlId[0], CharModelItemHookGet((s16)data->charNo, 2, 0), lbl_1_bss_88->mdlId[idx]);
                        Hu3DModelAttrSet(lbl_1_bss_88->mdlId[idx], 1);
                    }
                }
                if ((data->throwState == 0x1A) && (data->unk48 != 0) && (lbl_1_bss_80 == 0x3EE)) {
                    data->mode = 4;
                    data->modeTime = 0;
                    data->unk58 = 1;
                } else if (data->throwState >= 0x32) {
                    data->throwState = -1;
                }
            } else {
                s32 c;

                data->unk54 = 1;
                obj->trans.x = (f32)(800.0 * sin((M_PI * (f64)(lbl_1_bss_339C.angle * fade)) / 180.0));
                obj->trans.y = (f32)((f64)lbl_1_bss_339C.pivotY - (800.0 * cos((M_PI * (f64)(lbl_1_bss_339C.angle * fade)) / 180.0)));
                if (data->unk54 != 0) {
                    c = 0;
                } else {
                    c = 0xB4;
                }
                obj->rot.y = (f32)c;
                obj->rot.z = (f32)(lbl_1_bss_339C.angle * fade);
                if ((data->throwTime >= 0xB) && ((jump != 0.0f) || (throwB != 0.0f)) && (data->itemNo < 5)) {
                    s32 it;
                    s32 se;
                    f64 vy;
                    Mtx mtx;

                    it = data->itemNo + (data->itemBank * 5);
                    Hu3DModelObjMtxGet(obj->mdlId[0], CharModelItemHookGet((s16)data->charNo, 2, 0), mtx);
                    data->throwTime = 0;
                    lbl_1_bss_316C[it].x = mtx[0][3];
                    lbl_1_bss_316C[it].y = mtx[1][3];
                    lbl_1_bss_316C[it].x = 0.9375f * obj->trans.x;
                    lbl_1_bss_316C[it].y = (0.9375f * obj->trans.y) + (lbl_1_bss_339C.pivotY * 0.0625f);
                    lbl_1_bss_316C[it].velX = 0.0f;
                    if (jump != 0.0f) {
                        vy = 0.0;
                    } else {
                        vy = 5.860805860805861;
                    }
                    lbl_1_bss_316C[it].velY = (f32)vy;
                    lbl_1_bss_316C[it].time = 1;
                    lbl_1_bss_316C[it].hit = 0;
                    Hu3DModelHookReset(obj->mdlId[0]);
                    data->itemNo = data->itemNo + 1;
                    data->itemNo = data->itemNo % 5;
                    data->itemBank = data->itemBank + (data->itemNo == 0);
                    data->itemBank = data->itemBank % 4;
                    se = (jump != 0.0f) ? 5 : 4;
                    if (lbl_1_bss_A4[se].num < 3) {
                        lbl_1_bss_A4[se].pan[lbl_1_bss_A4[se].num] = (f32)(0.25f * (obj->trans.x / 500.0f));
                        lbl_1_bss_A4[se].num++;
                    }
                }
                c = 1;
                if (data->throwTime != 0 && data->throwTime < 0x1E) {
                    c = 0;
                }
                mot = (c != 0) ? 1 : 4;
                if (data->throwTime == 0x14) {
                    s32 idx = data->itemNo + (data->itemBank * 5) + 0x17;

                    if (data->itemNo < 5) {
                        Hu3DModelAttrSet(lbl_1_bss_88->mdlId[idx], 1);
                        Hu3DModelPosSet(lbl_1_bss_88->mdlId[idx], 0.0f, 0.0f, 0.0f);
                        Hu3DModelScaleSet(lbl_1_bss_88->mdlId[idx], 1.0833334f, 1.0833334f, 1.0833334f);
                        Hu3DModelHookSet(obj->mdlId[0], CharModelItemHookGet((s16)data->charNo, 2, 0), lbl_1_bss_88->mdlId[idx]);
                    }
                }
            }
            break;
        case 4:
            data->unk58++;
            if (data->modeTime >= 3) {
                obj->rot.y = obj->rot.y * 0.7f;
                mot = 0;
            }
            break;
        case 6: {
            s32 s;

            data->unk58++;
            if (lbl_1_bss_8C[0]->trans.x < 0.0f) {
                s = -1;
            } else {
                s = 1;
            }
            obj->rot.y = (f32)((0.3 * (f64)obj->rot.y) + (0.7 * (f64)(16.0f * (f32)s)));
            if (lbl_1_bss_80 == 0x3EF) {
                data->nextMotNo = 5;
            }
            break;
        }
        }
        for (i = 0; i < 0x14; i++) {
            s32 idx = i + 0x17;

            Hu3DLLightColorSet(lbl_1_bss_88->mdlId[idx], lbl_1_bss_316C[i].lightId, 0xFF, 0xFF, 0xFF, 0xFF);
            Hu3DModelAmbSet(lbl_1_bss_88->mdlId[idx], 1.0f, 1.0f, 1.0f);
            Hu3DModelAttrSet(lbl_1_bss_88->mdlId[idx + 0x14], 1);
            if (lbl_1_bss_316C[i].time == 0) {
                if (lbl_1_bss_80 == 0x3EB) {
                    HU3DMODEL *mdl = &Hu3DData[lbl_1_bss_88->mdlId[idx]];

                    if ((u32)(mdl->attr & 0x8000) != 0) {
                        Mtx mtx;

                        Hu3DModelObjMtxGet(obj->mdlId[0], CharModelItemHookGet((s16)data->charNo, 2, 0), mtx);
                        Hu3DModelAttrSet(lbl_1_bss_88->mdlId[idx + 0x14], 1);
                        Hu3DModelPosSet(lbl_1_bss_88->mdlId[idx + 0x14], mtx[0][3], mtx[1][3], mtx[2][3]);
                        Hu3DModelScaleSet(lbl_1_bss_88->mdlId[idx + 0x14], 1.0833334f, 1.0833334f, 1.0833334f);
                    }
                }
            } else {
                lbl_1_bss_316C[i].velY = (f32)((f64)lbl_1_bss_316C[i].velY + -0.1188118811881188);
                lbl_1_bss_316C[i].x += lbl_1_bss_316C[i].velX;
                lbl_1_bss_316C[i].y += lbl_1_bss_316C[i].velY;
                lbl_1_bss_316C[i].time += 1;
                Hu3DModelAttrReset(lbl_1_bss_88->mdlId[idx], 1);
                Hu3DModelPosSet(lbl_1_bss_88->mdlId[idx], lbl_1_bss_316C[i].x, lbl_1_bss_316C[i].y, 0.0f);
                Hu3DModelScaleSet(lbl_1_bss_88->mdlId[idx], 1.0833334f, 1.0833334f, 1.0833334f);
                if (lbl_1_bss_80 == 0x3EB) {
                    Hu3DModelAttrReset(lbl_1_bss_88->mdlId[idx + 0x14], 1);
                    Hu3DModelPosSet(lbl_1_bss_88->mdlId[idx + 0x14], lbl_1_bss_316C[i].x, lbl_1_bss_316C[i].y, 0.0f);
                    Hu3DModelScaleSet(lbl_1_bss_88->mdlId[idx + 0x14], 1.0833334f, 1.0833334f, 1.0833334f);
                }
                if (lbl_1_bss_316C[i].y <= -500.0f) {
                    lbl_1_bss_316C[i].time = 0;
                    Hu3DModelAttrSet(lbl_1_bss_88->mdlId[idx], 1);
                } else if (lbl_1_bss_316C[i].hit != 0) {
                    if ((lbl_1_bss_316C[i].time % 2) != 0) {
                        Hu3DModelAttrReset(lbl_1_bss_88->mdlId[idx], 1);
                    } else {
                        Hu3DModelAttrSet(lbl_1_bss_88->mdlId[idx], 1);
                    }
                }
                if (lbl_1_bss_80 == 0x3EB) {
                    float dx;
                    float dy;
                    float dist;
                    float fac;
                    float amb;
                    s32 col;

                    dx = lbl_1_bss_316C[i].x - obj->trans.x;
                    dy = lbl_1_bss_316C[i].y - obj->trans.y;
                    dist = sqrtf((dx * dx) + (dy * dy));
                    if (dist < 175.0f) {
                        fac = 1.0f;
                    } else {
                        float f;

                        if (dist < 225.0f) {
                            f = 1.0f - ((dist - 175.0f) / 50.0f);
                        } else {
                            f = 0.0f;
                        }
                        fac = f;
                    }
                    amb = (f32)(0.44999999999999996 + (0.55 * (f64)fac));
                    col = (s32)(255.0f * amb);
                    if (sqrtf((dx * dx) + (dy * dy)) >= 100.0f) {
                        Hu3DLLightColorSet(lbl_1_bss_88->mdlId[idx], lbl_1_bss_316C[i].lightId, (u8)col, (u8)col, (u8)col, 0xFF);
                        Hu3DModelAmbSet(lbl_1_bss_88->mdlId[idx], amb, amb, amb);
                    }
                }
            }
        }
    } else {
        switch (data->mode) {
        case 3:
            if (obj->trans.y > -3000.0f) {
                obj->trans.y = obj->trans.y + data->velY;
                obj->trans.z = obj->trans.z - 4.0f;
                obj->rot.x = 45.0f;
                obj->rot.y = 0.0f;
                data->velY = data->velY + -0.60833335f;
            }
            mot = 4;
            break;
        case 2:
            switch (lbl_1_bss_80) {
            case 0x3EE:
                break;
            case 0x3ED:
                if (data->padNo != -1) {
                    s32 mask;

                    stkX = (f32)HuPadStkX[data->padNo];
                    stkY = 0.0f;
                    btnHold = (f32)((HuPadBtn[data->padNo] & 0x100) > 0);
                    if (data->motNo == 2) {
                        mask = 0x140;
                    } else {
                        mask = 0x100;
                    }
                    jump = (f32)((HuPadBtnDown[data->padNo] & mask) > 0);
                } else if (data->aiState < 2) {
                    if (data->aiState >= 0) {
                        s32 link[30][2] = { { -1, -1 } };
                        float range[30][2] = { { -442.0f, 442.0f } };
                        s32 tblA[4] = { 30, 30, 10, 2 };
                        s32 tblB[4] = { 450, 450, 650, 1500 };
                        s32 head;
                        s32 cnt;
                        s32 cur;
                        float x1;
                        float x2;

                        head = 0;
                        cnt = 1;
                        for (i = 0; i < 0x16; i++) {
                            if (i < 0x14) {
                                float py;
                                float pv;

                                if ((lbl_1_bss_316C[i].time != 0) && (lbl_1_bss_316C[i].hit == 0) && (lbl_1_bss_316C[i].time >= tblA[data->dif]) && !(lbl_1_bss_316C[i].y > (obj->trans.y + (f32)tblB[data->dif])) && !(lbl_1_bss_316C[i].velY > 0.0f) && (lbl_1_bss_316C[i].y > (obj->trans.y - 10.0f))) {
                                    x1 = (lbl_1_bss_316C[i].x - 70.0f) - 5.0f;
                                    x2 = 5.0f + (70.0f + lbl_1_bss_316C[i].x);
                                    py = lbl_1_bss_316C[i].y;
                                    pv = lbl_1_bss_316C[i].velY;
                                    for (k = 0; (f32)k < 60.0f; k++) {
                                        pv = (f32)((f64)pv + -0.1188118811881188);
                                        py += pv;
                                    }
                                    if ((155.0f + obj->trans.y) >= py) {
                                        if (lbl_1_bss_316C[i].x < obj->trans.x) {
                                            x1 = -492.0f;
                                        } else {
                                            x2 = 492.0f;
                                        }
                                    }
                                } else {
                                    continue;
                                }
                            } else {
                                s32 t = (((i + data->no) - 0x14) % 3) + 1;

                                if (player[t]->grounded != 0) {
                                    if (lbl_1_bss_8C[t]->trans.x < obj->trans.x) {
                                        x1 = -482.0f;
                                        x2 = 40.0f + (40.0f + lbl_1_bss_8C[t]->trans.x);
                                    } else {
                                        x1 = (lbl_1_bss_8C[t]->trans.x - 40.0f) - 40.0f;
                                        x2 = 482.0f;
                                    }
                                } else {
                                    continue;
                                }
                            }
                            cur = head;
                            while (cur != -1) {
                                if ((range[cur][0] < x1) && (x1 < range[cur][1]) && (range[cur][0] < x2) && (x2 < range[cur][1])) {
                                    link[cnt][1] = link[cur][1];
                                    link[cur][1] = cnt;
                                    link[cnt][0] = cur;
                                    range[cnt][1] = range[cur][1];
                                    range[cnt][0] = x2;
                                    range[cur][1] = x1;
                                    cnt++;
                                    break;
                                }
                                if ((x1 <= range[cur][0]) && (range[cur][0] <= x2) && (x1 <= range[cur][1]) && (range[cur][1] <= x2)) {
                                    s32 h;

                                    if (head == cur) {
                                        h = link[cur][1];
                                    } else {
                                        h = head;
                                    }
                                    head = h;
                                    if (link[cur][0] != -1) {
                                        link[link[cur][0]][1] = link[cur][1];
                                    }
                                    if (link[cur][1] != -1) {
                                        link[link[cur][1]][0] = link[cur][0];
                                    }
                                } else if ((range[cur][0] < x1) && (x1 < range[cur][1])) {
                                    range[cur][1] = x1;
                                } else if ((range[cur][0] < x2) && (x2 < range[cur][1])) {
                                    range[cur][0] = x2;
                                }
                                cur = link[cur][1];
                            }
                        }
                        cur = head;
                        data->aiX = 10000.0f;
                        while (cur != -1) {
                            f64 d;

                            if ((cur == head) || (d = fabs((f64)(range[cur][0] - obj->trans.x)), (fabs((f64)(data->aiX - obj->trans.x)) > d)) || (d = fabs((f64)(range[cur][1] - obj->trans.x)), (fabs((f64)(data->aiX - obj->trans.x)) > d))) {
                                float v;

                                d = fabs((f64)(range[cur][1] - obj->trans.x));
                                if (fabs((f64)(range[cur][0] - obj->trans.x)) < d) {
                                    v = range[cur][0];
                                } else {
                                    v = range[cur][1];
                                }
                                data->aiX = v;
                            }
                            if ((range[cur][0] <= obj->trans.x) && (obj->trans.x <= range[cur][1])) {
                                data->aiX = obj->trans.x;
                                break;
                            }
                            cur = link[cur][1];
                        }
                        if (data->aiX == 10000.0f) {
                            data->aiX = obj->trans.x;
                        }
                        stkX = (data->aiX - obj->trans.x) / 0.25f;
                        if (data->dif == 0) {
                            if ((stkX != 0.0f) && (data->unk98 == 0) && (data->unk94 == 0)) {
                                if ((rand8() % 2) == 0) {
                                    data->unk98 = (rand8() % 4) + 0xD;
                                }
                                data->unk94 = (rand8() % 25) + 0x28;
                            }
                            {
                                float v;

                                if (data->unk98 != 0) {
                                    v = -stkX;
                                } else {
                                    v = stkX;
                                }
                                stkX = v;
                            }
                            data->unk94 = data->unk94 - (data->unk94 > 0);
                            data->unk98 = data->unk98 - (data->unk98 > 0);
                        }
                    }
                }
                break;
            }
            break;
        case 4: {
            float t;
            s32 side;
            s32 isSide;
            float r;

            if (data->throwState <= 0x2D) {
                t = (f32)data->throwState / 45.0f;
            } else {
                t = 1.0f;
            }
            obj->trans.x = (f32)((200.0f * data->rankOfs * t) + ((1.0f - t) * data->startX));
            obj->trans.z = (f32)((f64)t * (50.0 + (-1500.0 + (-100.0 * fabs((f64)data->rankOfs)))));
            obj->trans.y = (f32)(400.0 * (1.0 - ((((f64)t - 0.5) * ((f64)t - 0.5)) / 0.25)));
            obj->trans.y = (f32)(obj->trans.y + ((400.0f * t) + ((1.0f - t) * data->startY)));
            if (data->throwState <= 0x37) {
                r = 180.0f;
            } else {
                float q;

                if (data->throwState <= 0x73) {
                    isSide = 1;
                    if ((0.5f != data->rankOfs) && (data->rankOfs != 0.0f)) {
                        isSide = 0;
                    }
                    if (isSide != 0) {
                        side = -1;
                    } else {
                        side = 1;
                    }
                    q = 180.0f + (180.0f * ((f32)(data->throwState - 0x37) / 60.0f) * (f32)side);
                } else {
                    q = 0.0f;
                }
                r = q;
            }
            obj->rot.y = r;
            isSide = 1;
            if ((0.5f != data->rankOfs) && (data->rankOfs != 0.0f)) {
                isSide = 0;
            }
            if (isSide != 0) {
                k = 0;
            } else {
                k = 0x3C;
            }
            lbl_1_data_AC[1][9] = k;
            obj->rot.z = 0.0f;
            if (data->throwState <= 0x2D) {
                k = 2;
            } else {
                s32 q;

                if (data->throwState <= 0x37) {
                    q = 3;
                } else {
                    s32 q2;

                    if (data->throwState <= 0x6D) {
                        q2 = 9;
                    } else {
                        q2 = 0;
                    }
                    q = q2;
                }
                k = q;
            }
            mot = (s16)k;
            data->throwState++;
            if (lbl_1_bss_80 == 0x3EF) {
                data->mode = 6;
            }
            break;
        }
        case 6:
            obj->rot.y = (f32)((f64)obj->rot.y * 0.7);
            if (lbl_1_bss_80 == 0x3EF) {
                data->nextMotNo = 5;
            }
            break;
        }
        if (data->mode == 2) {
            switch (data->motNo) {
            case 0:
            case 1: {
                float nx;
                float nz;
                float len;

                nx = (f32)(0.25 * (f64)stkX);
                nz = (f32)(0.25 * -stkY);
                len = sqrtf((nx * nx) + (nz * nz));
                if (len >= 0.5f) {
                    if (len < 2.5f) {
                        data->velX = nx;
                        data->velZ = data->velZ + nz;
                    } else {
                        float ux;
                        float uz;

                        ux = nx / len;
                        uz = nz / len;
                        len = 2.5f;
                        if (data->motNo != 2) {
                            data->velX = (f32)(ux * 2.5f);
                            data->velZ = (f32)(uz * 2.5f);
                        } else {
                            data->velX = (f32)((0.875 * (f64)data->velX) + (0.125 * (f64)(ux * 2.5f)));
                            data->velZ = (f32)((0.875 * (f64)data->velZ) + (0.125 * (f64)(uz * 2.5f)));
                        }
                    }
                    data->speed = len;
                    mot = 1;
                } else if (data->motNo != 3) {
                    mot = 0;
                }
                data->velY = 0.0f;
                if ((lbl_1_bss_80 == 0x3EE) && (lbl_1_data_FC == (1 - (data->no / 2))) && (lbl_1_bss_0 == 70.0f)) {
                    data->mode = 5;
                } else if (jump != 0.0f) {
                    data->grounded = 0;
                    data->jumpHold = 1;
                    data->velY = 15.816668f;
                    mot = 2;
                }
                break;
            }
            case 3:
                if (Hu3DMotionTimeGet(obj->mdlId[0]) >= 0.0f) {
                    mot = 7;
                    HuAudFXPanning(CharFXPlay((s16)data->charNo, 0x1FF), (s16)(0.5 + (f64)(64.0f + (63.0f * (0.35f * (obj->trans.x / 500.0f))))));
                }
                break;
            case 2: {
                float nx;
                float nz;
                float len;

                nx = (f32)(0.25 * (f64)stkX);
                nz = (f32)(0.25 * -stkY);
                len = sqrtf((nx * nx) + (nz * nz));
                if (len >= 0.5f) {
                    if (len < 3.7f) {
                        data->velX = nx;
                        data->velZ = nz;
                    } else {
                        float ux;
                        float uz;

                        ux = nx / len;
                        uz = nz / len;
                        len = 3.7f;
                        if (data->motNo != 2) {
                            data->velX = (f32)(ux * 3.7f);
                            data->velZ = (f32)(uz * 3.7f);
                        } else {
                            data->velX = (f32)((0.875 * (f64)data->velX) + (0.125 * (f64)(ux * 3.7f)));
                            data->velZ = (f32)((0.875 * (f64)data->velZ) + (0.125 * (f64)(uz * 3.7f)));
                        }
                    }
                    data->speed = len;
                    obj->rot.y = (f32)(180.0 * (atan2((f64)data->velX, (f64)data->velZ) / 3.141592653589793));
                }
                if (data->jumpHold != 0) {
                    if (btnHold != 0.0f) {
                        s32 n = data->jumpHold + 1;

                        data->jumpHold = n;
                        if (n > 4) {
                            data->jumpHold = 0;
                        } else {
                            data->velY = data->velY + 2.4333334f;
                        }
                    } else {
                        data->jumpHold = 0;
                    }
                }
                data->velY = data->velY + -1.2166667f;
                if (data->grounded != 0) {
                    data->velY = 0.0f;
                    mot = 3;
                    if (lbl_1_bss_A4[9].num < 3) {
                        lbl_1_bss_A4[9].pan[lbl_1_bss_A4[9].num] = (f32)(0.25f * (obj->trans.x / 500.0f));
                        lbl_1_bss_A4[9].num++;
                    }
                }
                break;
            }
            case 5:
            case 6:
                obj->rot.y = (f32)(2.4f + (0.8f * obj->rot.y));
                break;
            case 7:
                data->shake = 6;
                if (Hu3DMotionTimeGet(obj->mdlId[0]) >= 28.0f) {
                    mot = 0;
                }
                break;
            }
        }
        if (data->mode == 2) {
            s32 turn;

            if (data->velX != 0.0f) {
                if (data->velX < 0.0f) {
                    turn = -1;
                } else {
                    s32 t2;

                    if (data->velX > 0.0f) {
                        t2 = 1;
                    } else {
                        t2 = 0;
                    }
                    turn = t2;
                }
                if (data->dir == turn) {
                    data->turnTime = data->turnTime + 1;
                } else {
                    data->prevDir = data->dir;
                    if (data->velX < 0.0f) {
                        turn = -1;
                    } else {
                        s32 t2;

                        if (data->velX > 0.0f) {
                            t2 = 1;
                        } else {
                            t2 = 0;
                        }
                        turn = t2;
                    }
                    data->dir = turn;
                    data->turnTime = 0;
                }
            } else {
                data->turnTime = data->turnTime + 1;
            }
            if (data->turnTime < 0xA) {
                float f = (f32)data->turnTime / 10.0f;

                obj->rot.y = (f32)(((1.0f - f) * (f32)(data->prevDir * 0x5A)) + (90.0f * (f * (f32)data->dir)));
            } else {
                obj->trans.x = obj->trans.x + data->velX;
                obj->rot.y = (f32)(data->dir * 0x5A);
            }
            if (data->grounded == 0) {
                obj->trans.y = obj->trans.y + data->velY;
            }
            obj->trans.z = obj->trans.z + data->velZ;
            {
                float v;

                if (obj->trans.x < -442.0f) {
                    v = -442.0f;
                } else {
                    float w;

                    if (obj->trans.x > 442.0f) {
                        w = 442.0f;
                    } else {
                        w = obj->trans.x;
                    }
                    v = w;
                }
                obj->trans.x = v;
            }
            for (i = 0; i < 2; i++) {
                s32 o = ((data->no + i) % 3) + 1;
                float dx = lbl_1_bss_8C[o]->trans.x - obj->trans.x;
                float dy = (f32)fabs((f64)(lbl_1_bss_8C[o]->trans.y - obj->trans.y));

                if (player[o]->down == 0) {
                    float ov;

                    if (dy > 130.0f) {
                        ov = 0.0f;
                    } else {
                        float w;

                        if (dy > 60.0f) {
                            w = 50.0f * (1.0f - ((dy - 60.0f) / 70.0f));
                        } else {
                            w = 50.0f;
                        }
                        ov = w;
                    }
                    if ((ov != 0.0f) && (fabs((f64)dx) < (50.0f + ov))) {
                        s32 sg;

                        if (lbl_1_bss_8C[o]->trans.x >= 482.0f) {
                            sg = -1;
                        } else {
                            s32 sg2;

                            if (lbl_1_bss_8C[o]->trans.x <= -482.0f) {
                                sg2 = 1;
                            } else {
                                s32 sg3;

                                if (dx < 0.0f) {
                                    sg3 = -1;
                                } else {
                                    sg3 = 1;
                                }
                                sg2 = sg3;
                            }
                            sg = sg2;
                        }
                        obj->trans.x = lbl_1_bss_8C[o]->trans.x - ((50.0f + ov) * (f32)sg);
                    }
                }
            }
            if (data->mode != 5) {
                for (i = 0; i < 2; i++) {
                    s32 o = (((data->no + i) - 1) % 3) + 1;

                    if (player[o]->down == 0) {
                        float v;

                        if (lbl_1_bss_8C[o]->trans.x < -442.0f) {
                            v = -442.0f;
                        } else {
                            float w;

                            if (lbl_1_bss_8C[o]->trans.x > 442.0f) {
                                w = 442.0f;
                            } else {
                                w = lbl_1_bss_8C[o]->trans.x;
                            }
                            v = w;
                        }
                        lbl_1_bss_8C[o]->trans.x = v;
                    }
                }
                for (i = 0; i < 2; i++) {
                    s32 o = ((data->no + i) % 3) + 1;
                    float dx = lbl_1_bss_8C[o]->trans.x - obj->trans.x;
                    float dy = (f32)fabs((f64)(lbl_1_bss_8C[o]->trans.y - obj->trans.y));

                    if (player[o]->down == 0) {
                        float ov;

                        if (dy > 130.0f) {
                            ov = 0.0f;
                        } else {
                            float w;

                            if (dy > 60.0f) {
                                w = 50.0f * (1.0f - ((dy - 60.0f) / 70.0f));
                            } else {
                                w = 50.0f;
                            }
                            ov = w;
                        }
                        if ((ov != 0.0f) && (fabs((f64)dx) < (50.0f + ov))) {
                            s32 sg;

                            if (obj->trans.x >= 482.0f) {
                                sg = -1;
                            } else {
                                s32 sg2;

                                if (obj->trans.x <= -482.0f) {
                                    sg2 = 1;
                                } else {
                                    s32 sg3;

                                    if (dx < 0.0f) {
                                        sg3 = -1;
                                    } else {
                                        sg3 = 1;
                                    }
                                    sg2 = sg3;
                                }
                                sg = sg2;
                            }
                            lbl_1_bss_8C[o]->trans.x = obj->trans.x + ((50.0f + ov) * (f32)sg);
                        }
                    }
                }
            }
            for (j = 0; lbl_1_data_108[j] < 500.0f;) {
                if ((lbl_1_data_108[j] <= obj->trans.x) && (obj->trans.x <= lbl_1_data_108[j + 1])) {
                    float t = (obj->trans.x - lbl_1_data_108[j]) / (lbl_1_data_108[j + 1] - lbl_1_data_108[j]);
                    float h = ((1.0f - t) * lbl_1_data_134[j]) + (t * lbl_1_data_134[j + 1]);

                    if (groundY > h) {
                        groundY = h;
                    }
                    break;
                }
                j++;
            }
            if (obj->trans.y < groundY) {
                obj->trans.y = groundY;
                if (data->velY < 0.0f) {
                    if (data->grounded == 0) {
                        data->footNo = data->footNo + 1;
                        data->footNo = data->footNo % 3;
                    }
                    data->grounded = 1;
                }
            }
            if ((lbl_1_bss_80 == 0x3EB) && (lbl_1_bss_0 == 125.0f)) {
                omVibrate((s16)data->playerNo, 0xC, 4, 2);
            }
            if (lbl_1_bss_80 == 0x3EB) {
                if ((lbl_1_bss_0 >= 125.0f) && (lbl_1_bss_0 < 185.0f)) {
                    mot = 7;
                }
            }
            if ((lbl_1_bss_80 == 0x3EB) && (lbl_1_bss_0 == 125.0f)) {
                HuAudFXPanning(CharFXPlay((s16)data->charNo, 0x1FF), (s16)(0.5 + (f64)(64.0f + (63.0f * (0.35f * (obj->trans.x / 500.0f))))));
            }
            if ((lbl_1_bss_80 == 0x3EB) && (lbl_1_bss_0 < 110.0f)) {
                mot = 8;
            }
            if ((lbl_1_bss_80 == 0x3EA) && (lbl_1_bss_0 > 30.0f)) {
                mot = 8;
            }
            for (i = 0; i < 0x14; i++) {
                if ((lbl_1_bss_316C[i].time != 0) && (lbl_1_bss_316C[i].hit == 0) && ((obj->trans.x - 70.0f) < lbl_1_bss_316C[i].x) && (lbl_1_bss_316C[i].x < (70.0f + obj->trans.x)) && ((obj->trans.y - 10.0f) < lbl_1_bss_316C[i].y) && (lbl_1_bss_316C[i].y < (155.0f + obj->trans.y))) {
                    float ang;

                    lbl_1_bss_316C[i].hit = 1;
                    ang = (f32)(180.0 * (atan2((f64)(lbl_1_bss_316C[i].x - obj->trans.x), (f64)(lbl_1_bss_316C[i].y - (70.0f + obj->trans.y))) / 3.141592653589793));
                    lbl_1_bss_316C[i].velX = (f32)(9.0 * sin((3.141592653589793 * (f64)ang) / 180.0));
                    lbl_1_bss_316C[i].velY = (f32)(9.0 * -cos((3.141592653589793 * (f64)ang) / 180.0));
                    data->down = 1;
                    data->mode = 3;
                    data->velY = 10.0f;
                    omVibrate((s16)data->playerNo, 0xC, 0xC, 0);
                    if ((lbl_1_bss_80 == 0x3ED) && (player[1]->down != 0) && (player[2]->down != 0) && (player[3]->down != 0)) {
                        lbl_1_data_FC = 0;
                    }
                    if (lbl_1_bss_A4[6].num < 3) {
                        lbl_1_bss_A4[6].pan[lbl_1_bss_A4[6].num] = (f32)(0.25f * (obj->trans.x / 500.0f));
                        lbl_1_bss_A4[6].num++;
                    }
                    if (lbl_1_bss_A4[7].num < 3) {
                        lbl_1_bss_A4[7].pan[lbl_1_bss_A4[7].num] = (f32)(0.25f * (obj->trans.x / 500.0f));
                        lbl_1_bss_A4[7].num++;
                    }
                    lbl_1_bss_C = 1;
                    lbl_1_bss_5C[0] = 0.25f * (obj->trans.x / 500.0f);
                }
            }
        }
    }
    if (data->no != 0) {
        for (i = 0; i < 3; i++) {
            if ((data->grounded != 0) && (data->down == 0) && (data->mode < 4) && (i == data->footNo)) {
                float v;

                data->footX[i] = obj->trans.x;
                data->footY[i] = ((0.6f * (1.0f + data->footY[i])) - 0.1f) - 1.0f;
                if (data->footY[i] > 0.0f) {
                    v = 0.0f;
                } else {
                    float w;

                    if (data->footY[i] < -1.0f) {
                        w = -1.0f;
                    } else {
                        w = data->footY[i];
                    }
                    v = w;
                }
                data->footY[i] = v;
            } else {
                float v;

                data->footY[i] = 0.1f + (0.4f * data->footY[i]);
                if (data->footY[i] > 0.0f) {
                    v = 0.0f;
                } else {
                    float w;

                    if (data->footY[i] < -1.0f) {
                        w = -1.0f;
                    } else {
                        w = data->footY[i];
                    }
                    v = w;
                }
                data->footY[i] = v;
            }
        }
    }
    if (data->nextMotNo >= 0) {
        mot = (s16)data->nextMotNo;
    }
    if ((lbl_1_bss_80 == 0x3EE) && (lbl_1_bss_0 == 2.0f) && (data->motNo == 0)) {
        mot = 0;
        data->motNo = 8;
    }
    if ((mot >= 0) && (mot != data->motNo)) {
        s32 m = (s32)mot;
        u32 attr;

        if ((m == 0) && ((lbl_1_bss_80 == 0x3EC) || (lbl_1_bss_80 == 0x3ED))) {
            m = 8;
        }
        if (lbl_1_data_84[row][m] != 0) {
            attr = 0;
        } else {
            attr = 0x40000001;
        }
        Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[m], (f32)lbl_1_data_AC[row][m], (f32)lbl_1_data_98[row][m], attr);
        data->motNo = mot;
    }
    if (data->no == 0) {
        HU3DMODEL *mdl = &Hu3DData[obj->mdlId[0]];
        float t;
        float c;
        float w;

        t = (f32)data->swingTime / 80.0f;
        if (t < 0.0f) {
            c = 0.0f;
        } else {
            float u;

            if (t > 1.0f) {
                u = 1.0f;
            } else {
                u = t;
            }
            c = u;
        }
        if (lbl_1_bss_339C.speed < 0.0f) {
            w = 1.0f - c;
        } else {
            w = c;
        }
        if ((s16)mdl->motIdShift == (s16)obj->mtnId[1]) {
            mdl->motShiftWork.time = w * Hu3DMotionMotionMaxTimeGet(obj->mtnId[1]);
        }
        if ((s16)mdl->motId == (s16)obj->mtnId[1]) {
            mdl->motWork.time = w * Hu3DMotionMotionMaxTimeGet(obj->mtnId[1]);
        }
    }
    obj->rot.y = obj->rot.y * (1.0f - ((f32)data->shake / 6.0f));
    data->shake = data->shake - (data->shake > 0);
    if ((lbl_1_bss_80 == 0x3EF) && (lbl_1_data_FC != 1)) {
        CharModelVoicePanSet((s16)data->charNo, 0x40);
        return;
    }
    CharModelVoicePanSet((s16)data->charNo, (s16)(0.5 + (f64)(64.0f + (63.0f * (0.35f * (obj->trans.x / 500.0f))))));
}

/* Trapezoid speed profile: rises over a, stays for b, falls over c. Returns the travelled fraction at t. */
static float fn_1_5930(float t, float a, float b, float c)
{
    float total = (c / 2) + ((a / 2) + b);

    return (t < a) ? ((((1.0f + t) / 2 * t) / a) / total)
                   : ((t < (a + b)) ? (((((1.0f + a) / 2 * a) / a) / total) + ((t - a) / total))
                                    : ((t < ((c + (a + b)) - 1.0f))
                                           ? (1.0f - ((((1.0f + (((c + (a + b)) - t) - 1.0f)) / 2 * ((((c + (a + b)) - 1.0f) - t) - 1.0f)) / (c - 1.0f)) / total))
                                           : 1.0f));
}

/* Per-frame minigame logic: rebuilds the ground profile from the feet of the
   runners, drives the scripted camera moves for the current timer state, and
   spawns/advances the confetti (M525Fx) particles. */
static void fn_1_5AF4(OMOBJ *obj)
{
    M525Player *player[4];
    f32 minX;
    f32 maxX;
    f32 prevH;
    f32 bestH;
    f32 x;
    f32 fall;
    f32 h;
    f32 t;
    f32 lin;
    f32 rot;
    f32 endY;
    f32 blend;
    f32 inv;
    f32 rate;
    s32 i;
    s32 j;
    s32 n;
    s32 dir;
    s32 leader;
    M525Fx *fx;

    for (i = 0; i < 4; i++) {
        player[i] = lbl_1_bss_8C[i]->data;
    }
    maxX = 0.0f;
    minX = -500.0f;
    prevH = 0.0f;
    n = 0;
    lbl_1_data_108[0] = -500.0f;
    lbl_1_data_134[0] = 0.0f;
    while (maxX < 500.0f) {
        maxX = 500.0f;
        bestH = 0.0f;
        for (j = 0; j < 9; j++) {
            x = player[(j / 3) + 1]->footX[j % 3];
            if (minX < x) {
                fall = fabs(x) / 1000.0;
                h = (f64)(33.0f * player[(j / 3) + 1]->footY[j % 3]) * (1.0 - ((0.875 * (f64)fall) + (0.125 * (f64)(fall * fall))));
                t = (x - minX) / (maxX - minX);
                lin = (prevH * (1.0f - t)) + (bestH * t);
                if (lin > h) {
                    maxX = x;
                    bestH = h;
                }
            }
        }
        n++;
        lbl_1_data_108[n] = maxX;
        lbl_1_data_134[n] = bestH;
        minX = maxX;
        prevH = bestH;
    }
    for (i = 0; i < 3; i++) {
        if (player[i + 1]->down == 0 && player[i + 1]->mode < 4 && player[i + 1]->grounded != 0) {
            f32 y;

            x = player[i + 1]->footX[player[i + 1]->footNo];
            y = 33.0f * player[i + 1]->footY[player[i + 1]->footNo];
            fall = fabs(x) / 1000.0;
            y = (f64)y * (1.0 - ((0.875 * (f64)fall) + (0.125 * (f64)(fall * fall))));
            lbl_1_bss_8C[i + 1]->trans.y = y;
        }
    }
    if (lbl_1_bss_80 <= 0x3E9) {
        for (i = 0; i < 2; i++) {
            CZoomM[i] = 1700.0f;
            CenterM[i].y = 115.0f;
            CRotM[i].x = 5.0f;
        }
    }
    if (lbl_1_bss_80 == 0x3EB) {
        for (i = 0; i < 2; i++) {
            rate = 1.0f - 0.96f;
            CZoomM[i] = (2400.0f * rate) + (0.96f * CZoomM[i]);
            CenterM[i].y = (390.0f * rate) + (0.96f * CenterM[i].y);
            CRotM[i].x = (18.0f * rate) + (0.96f * CRotM[i].x);
        }
    }
    if (lbl_1_bss_80 == 0x3EC) {
        for (i = 0; i < 2; i++) {
            rate = 1.0f - 0.96f;
            CZoomM[i] = (2900.0f * rate) + (0.96f * CZoomM[i]);
            CenterM[i].y = (390.0f * rate) + (0.96f * CenterM[i].y);
            CRotM[i].x = 0.96f * CRotM[i].x;
        }
    }
    if (lbl_1_bss_80 == 0x3EE && lbl_1_data_FC == 1) {
        if (lbl_1_bss_0 >= 61.0f) {
            rot = lbl_1_bss_0 - 61.0f;
            endY = 50.0f;
            if (rot < 10.0f) {
                blend = (((1.0f + rot) * 0.5f * rot) / 10.0f) / endY;
            } else {
                f32 b;

                if (rot < 40.0f) {
                    b = (5.5f / endY) + ((rot - 10.0f) / endY);
                } else {
                    f32 z;

                    if (rot < 69.0f) {
                        z = 1.0f - ((((1.0f + ((70.0f - rot) - 1.0f)) * 0.5f * ((69.0f - rot) - 1.0f)) / 29.0f) / endY);
                    } else {
                        z = 1.0f;
                    }
                    b = z;
                }
                blend = b;
            }
            inv = 1.0f - blend;
            if (lbl_1_bss_0 == 61.0f) {
                lbl_1_bss_30 = CZoomM[0];
                lbl_1_bss_2C = CenterM[0].x;
                lbl_1_bss_28 = CenterM[0].y;
                lbl_1_bss_24 = CenterM[0].z;
                lbl_1_bss_20 = CRotM[0].x;
            }
            if (lbl_1_bss_0 == 81.0f) {
                Hu3DModelShadowDispOff(lbl_1_bss_8C[0]->mdlId[0]);
                Hu3DModelShadowSet(lbl_1_bss_8C[1]->mdlId[0]);
                Hu3DModelShadowSet(lbl_1_bss_8C[2]->mdlId[0]);
                Hu3DModelShadowSet(lbl_1_bss_8C[3]->mdlId[0]);
                lbl_1_bss_4C.x = 0.0f;
                lbl_1_bss_4C.y = 2000.0f;
                lbl_1_bss_4C.z = -1500.0f;
                lbl_1_bss_40.x = 0.0f;
                lbl_1_bss_40.y = 0.0f;
                lbl_1_bss_40.z = -1.0f;
                lbl_1_bss_34.x = 0.0f;
                lbl_1_bss_34.y = 0.0f;
                lbl_1_bss_34.z = -1500.0f;
                Hu3DShadowMultiPosSet(&lbl_1_bss_4C, &lbl_1_bss_40, &lbl_1_bss_34, -1);
            }
            for (i = 0; i < 2; i++) {
                CZoomM[i] = (1300.0f * blend) + (lbl_1_bss_30 * inv);
                CenterM[i].x = lbl_1_bss_2C * inv;
                CenterM[i].y = (461.0f * blend) + (lbl_1_bss_28 * inv);
                CenterM[i].z = (-1500.0f * blend) + (lbl_1_bss_24 * inv);
                CRotM[i].x = (-4.0f * blend) + (lbl_1_bss_20 * inv);
            }
        }
    }
    if (lbl_1_bss_80 == 0x3EF) {
        if (lbl_1_data_FC == 0) {
            Hu3DModelPosSet(obj->mdlId[0x40], lbl_1_bss_8C[0]->trans.x, 100.0f + lbl_1_bss_8C[0]->trans.y, lbl_1_bss_8C[0]->trans.z);
            Hu3DModelAttrReset(obj->mdlId[0x14], 1);
            Hu3DModelPosSet(obj->mdlId[0x14], lbl_1_bss_8C[0]->trans.x, lbl_1_bss_8C[0]->trans.y, lbl_1_bss_8C[0]->trans.z);
            for (i = 0; i < 2; i++) {
                rate = 1.0f - 0.96f;
                CZoomM[i] = (1000.0f * rate) + (0.96f * CZoomM[i]);
                CenterM[i].x = (0.96f * CenterM[i].x) + (rate * lbl_1_bss_8C[0]->trans.x);
                CenterM[i].y = (0.96f * CenterM[i].y) + (rate * (100.0f + lbl_1_bss_8C[0]->trans.y));
                CenterM[i].z = (0.96f * CenterM[i].z) + (rate * lbl_1_bss_8C[0]->trans.z);
                CRotM[i].x = (-8.0f * rate) + (0.96f * CRotM[i].x);
                if (lbl_1_bss_8C[0]->trans.x < 0.0f) {
                    dir = -1;
                } else {
                    dir = 1;
                }
                CRotM[i].y = (0.96f * CRotM[i].y) + (16.0f * rate * (f32)dir);
            }
            lbl_1_bss_4C.x = lbl_1_bss_8C[0]->trans.x;
            lbl_1_bss_4C.y = 2000.0f + lbl_1_bss_8C[0]->trans.y;
            lbl_1_bss_4C.z = lbl_1_bss_8C[0]->trans.z;
            lbl_1_bss_40.x = 0.0f;
            lbl_1_bss_40.y = 0.0f;
            lbl_1_bss_40.z = -1.0f;
            lbl_1_bss_34.x = lbl_1_bss_8C[0]->trans.x;
            lbl_1_bss_34.y = lbl_1_bss_8C[0]->trans.y;
            lbl_1_bss_34.z = lbl_1_bss_8C[0]->trans.z;
            Hu3DShadowMultiPosSet(&lbl_1_bss_4C, &lbl_1_bss_40, &lbl_1_bss_34, -1);
        } else {
            Hu3DModelPosSet(obj->mdlId[0x40], 0.0f, 461.0f, -1500.0f);
            for (i = 0; i < 3; i++) {
                Hu3DModelAttrReset(obj->mdlId[i + 0x14], 1);
                Hu3DModelPosSet(obj->mdlId[i + 0x14], lbl_1_bss_8C[i + 1]->trans.x, lbl_1_bss_8C[i + 1]->trans.y, lbl_1_bss_8C[i + 1]->trans.z);
            }
            /* the original loop bound is -2: this camera move never runs */
            for (i = 0; i < -2; i++) {
                rate = 1.0f - 0.9f;
                if (player[1]->down == 0) {
                    leader = 1;
                } else {
                    if (player[2]->down == 0) {
                        j = 2;
                    } else {
                        j = 3;
                    }
                    leader = j;
                }
                CZoomM[i] = (1300.0f * rate) + (0.9f * CZoomM[i]);
                CenterM[i].x = 0.9f * CenterM[i].x;
                CenterM[i].y = (461.0f * rate) + (0.9f * CenterM[i].y);
                CenterM[i].z = (-1500.0f * rate) + (0.9f * CenterM[i].z);
                CRotM[i].x = (-4.0f * rate) + (0.9f * CRotM[i].x);
            }
        }
    }
    if (lbl_1_data_FC != -1 && lbl_1_bss_80 >= 0x3EF) {
        if (lbl_1_data_FC == 1 || (lbl_1_bss_80 == 0x3EF && lbl_1_bss_0 > 7.0f)) {
            Hu3DModelAttrReset(lbl_1_bss_88->mdlId[0x40], 1);
        }
        for (i = 0; i < 0x100; i++) {
            if (lbl_1_bss_16C[i].time >= 0x100) {
                f32 r;

                fx = &lbl_1_bss_16C[i];
                r = (f32)rand8() / 255.0f;
                fx->x = 900.0 * ((f64)r - 0.5);
                fx->y = 320.0f;
                fx->z = 0.0f;
                fx->velX = 400.0 * ((f64)((f32)rand8() / 255.0f) - 0.5) * 0.00390625;
                fx->velY = (-900.0f + (-100.0f * ((f32)rand8() / 255.0f))) * 0.00390625f;
                fx->velZ = 0.0f;
                fx->axis.x = 4.0 * ((f64)((f32)rand8() / 255.0f) - 0.5);
                fx->axis.y = (f64)((f32)rand8() / 255.0f) - 0.5;
                fx->axis.z = 0.5 * ((f64)((f32)rand8() / 255.0f) - 0.5);
                fx->rotSpeed = 1.0f / (45.0f + (30.0f * ((f32)rand8() / 255.0f)));
                if (fx->axis.x == 0.0f && fx->axis.y == 0.0f && fx->axis.z == 0.0f) {
                    fx->axis.x = 1.0f;
                }
                fx->time = 0;
                fx->kind = rand8() % 4;
            }
            lbl_1_bss_16C[i].x += lbl_1_bss_16C[i].velX;
            lbl_1_bss_16C[i].y += lbl_1_bss_16C[i].velY;
            lbl_1_bss_16C[i].z += lbl_1_bss_16C[i].velZ;
            lbl_1_bss_16C[i].time += 1;
        }
    }
    if (lbl_1_bss_C != 0) {
        if (lbl_1_bss_18[0] > 1 && lbl_1_bss_A4[1].seNo != -1) {
            HuAudFXStop(lbl_1_bss_A4[1].seNo);
            lbl_1_bss_A4[1].seNo = -1;
        }
        if (lbl_1_bss_A4[1].num < 3) {
            lbl_1_bss_A4[1].pan[lbl_1_bss_A4[1].num] = lbl_1_bss_5C[0];
            lbl_1_bss_A4[1].num += 1;
        }
        lbl_1_bss_18[0] = 0x78;
        lbl_1_bss_C = 0;
    }
    lbl_1_bss_18[0] -= lbl_1_bss_18[0] > 0;
    if (lbl_1_bss_18[0] == 1 && lbl_1_bss_A4[1].seNo != -1) {
        HuAudFXStop(lbl_1_bss_A4[1].seNo);
        lbl_1_bss_A4[1].seNo = -1;
    }
    lbl_1_bss_4C.x = 0.0f;
    sin(1.5707963267948966);
    lbl_1_bss_4C.y = 0.0;
    lbl_1_bss_4C.z = 10000.0f;
    lbl_1_bss_40.x = 0.0f;
    lbl_1_bss_40.y = 1.0f;
    lbl_1_bss_40.z = 0.0f;
    lbl_1_bss_34.x = lbl_1_bss_8C[0]->trans.x;
    lbl_1_bss_34.y = lbl_1_bss_8C[0]->trans.y;
    lbl_1_bss_34.z = lbl_1_bss_8C[0]->trans.z;
    Hu3DModelPosSet(obj->mdlId[13], lbl_1_bss_8C[0]->trans.x, lbl_1_bss_8C[0]->trans.y, lbl_1_bss_8C[0]->trans.z);
}

static void fn_1_7698(HU3DMODEL *model, Mtx *mtx)
{
    M525Player *player[4];
    float sp18;
    float sp14;
    float sp10;
    float spC;
    s32 i;
    s32 j;
    s32 alpha;
    s32 flag;

    for (i = 0; i < 4; i++) {
        player[i] = lbl_1_bss_8C[i]->data;
    }
    GXLoadPosMtxImm(*mtx, GX_PNMTX0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZCompLoc(GX_TRUE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    HuSprTexLoad(espanim[esprite[lbl_1_bss_58].animNo].anim, 0, GX_TEXMAP0, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    sp18 = 0.0f;
    sp14 = 0.0f;
    sp10 = -500.0f;
    spC = 0.0f;
    flag = 1;
    if (lbl_1_bss_80 != 0x3EA && lbl_1_bss_80 != 0x3EB) {
        flag = 0;
    }
    alpha = flag ? 95.625 : 255.0;
    for (i = 0; lbl_1_data_108[i] < 500.0f; i++) {
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 8);
        for (j = 0; j < 4; j++) {
            float ofs[4] = { 2.0f, -2.0f, -8.0f, -12.0f };
            s32 col[4] = { 0, 255, 255, 0 };

            GXPosition3f32(lbl_1_data_108[i], lbl_1_data_134[i] + ofs[j], 0.0f);
            GXColor4u8(alpha, alpha, alpha, col[j]);
            GXTexCoord2f32(50.0f * (lbl_1_data_108[i] / 500.0f), ofs[j] / -10.0f);
            GXPosition3f32(lbl_1_data_108[i + 1], lbl_1_data_134[i + 1] + ofs[j], 0.0f);
            GXColor4u8(alpha, alpha, alpha, col[j]);
            GXTexCoord2f32(50.0f * (lbl_1_data_108[i + 1] / 500.0f), ofs[j] / -10.0f);
        }
    }
    GXPixModeSync();
}

static void fn_1_7C60(HU3DMODEL *model, Mtx *mtx)
{
    Vec out[5];
    Vec quad[5] = { { -1.0f, -1.0f, 0.0f }, { 1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { -1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } };
    M525Player *player[4];
    Vec light = { 0.0f, 1.0f, -1.0f };
    s32 i;
    s32 side;
    s32 k;
    s32 idx;
    float dot;
    float shade;

    for (i = 0; i < 4; i++) {
        player[i] = lbl_1_bss_8C[i]->data;
    }
    GXLoadPosMtxImm(*mtx, GX_PNMTX0);
    PSVECNormalize(&light, &light);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    for (i = 0; i < 256; i++) {
        Mtx rot;
        s32 col[5][3] = { { 255, 96, 96 }, { 96, 255, 104 }, { 96, 180, 255 }, { 255, 255, 90 }, { 232, 232, 232 } };
        Vec axis = { 1.0f, 0.0f, 0.0f };

        PSMTXRotAxisRad(rot, &lbl_1_bss_16C[i].axis, 0.017453292f * (360.0f * (lbl_1_bss_16C[i].time * lbl_1_bss_16C[i].rotSpeed)));
        PSMTXMultVecArray(rot, quad, out, 5);
        dot = PSVECDotProduct(&out[4], &light);
        GXBegin(GX_QUADS, GX_VTXFMT0, 8);
        for (side = 0; side < 2; side++) {
            shade = (side == 0) ? dot : -dot;
            shade = (shade < 0.0f) ? 0.5f : (0.5f + (0.6f * shade));
            shade = (shade > 1.0f) ? 1.0f : shade;
            for (k = 0; k < 4; k++) {
                idx = (side == 0) ? k : (3 - k);
                GXPosition3f32((10.0f * out[idx].x) + lbl_1_bss_16C[i].x, (10.0f * out[idx].y) + lbl_1_bss_16C[i].y, (10.0f * out[idx].z) + lbl_1_bss_16C[i].z);
                GXColor4u8(shade * col[lbl_1_bss_16C[i].kind][0], shade * col[lbl_1_bss_16C[i].kind][1], shade * col[lbl_1_bss_16C[i].kind][2], 255);
            }
        }
    }
}

static void fn_1_8394(HU3DMODEL *model, Mtx *mtx)
{
    Mtx modelview;
#ifdef TARGET_PC
    Mtx44 proj;
#else
    Mtx proj;
#endif
    s32 alpha;

    C_MTXOrtho(proj, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);
    PSMTXIdentity(modelview);
    GXLoadPosMtxImm(modelview, GX_PNMTX0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    if (model->pos.x > 1.0f) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTCLR, GX_BL_ONE, GX_LO_NOOP);
        alpha = 255.0f * (model->pos.x - 1.0f);
    } else {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTCLR, GX_BL_ZERO, GX_LO_NOOP);
        alpha = 255.0f * model->pos.x;
    }
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(0.0f, 0.0f, 0.0f);
    GXColor4u8(alpha, alpha, alpha, 255);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(1.0f, 0.0f, 0.0f);
    GXColor4u8(alpha, alpha, alpha, 255);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(1.0f, 1.0f, 0.0f);
    GXColor4u8(alpha, alpha, alpha, 255);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(0.0f, 1.0f, 0.0f);
    GXColor4u8(alpha, alpha, alpha, 255);
    GXTexCoord2f32(0.0f, 1.0f);
}
/* Game flow controller: intro, countdown, play, finish and result. */
static void fn_1_889C(OMOBJ *obj)
{
    M525Player *player[4];
    s32 i;
    s32 j;
    s32 carry;
    s32 count;
    s32 seNo;
    s32 tmp;

    lbl_1_bss_4[0] += 1;
    for (i = 0; i < 4; i++) {
        player[i] = lbl_1_bss_8C[i]->data;
    }
    for (i = 0; i < 10; i++) {
        s32 seTbl[10] = { 0x39C, 0x39C, 0x39B, 0x768, 0x769, 0x76A, 0x76B, 0x76C, 0x76D, 0x21 };

        if (lbl_1_bss_A4[i].num > 3) {
            lbl_1_bss_A4[i].num = 3;
        }
        if (lbl_1_bss_A4[i].num > 0 && seTbl[i] != -1) {
            seNo = HuAudFXPlay(seTbl[i]);
            lbl_1_bss_A4[i].seNo = seNo;
            HuAudFXPanning(seNo, 0.5 + (64.0f + (63.0f * lbl_1_bss_A4[i].pan[lbl_1_bss_A4[i].num - 1])));
            lbl_1_bss_A4[i].num--;
        }
    }
    if (omSysExitReq != 0 && WipeCheckEnd() == 0 && lbl_1_bss_10[0] == 0) {
        HuAudFadeOut(1000);
        lbl_1_data_104 = 1;
        lbl_1_bss_10[0] = 1;
    }
    switch (lbl_1_bss_80) {
        case 0x3E8:
            lbl_1_bss_0 += 1.0f;
            if (lbl_1_bss_0 == 5.0f && lbl_1_bss_A4[2].num < 3) {
                lbl_1_bss_A4[2].pan[lbl_1_bss_A4[2].num] = 0.0f;
                lbl_1_bss_A4[2].num++;
            }
            if (lbl_1_bss_0 == 5.0f) {
                lbl_1_data_100 = HuAudSStreamPlay(0x59);
            }
            if (WipeCheckEnd() == 0) {
                lbl_1_bss_80++;
                lbl_1_bss_0 = 0.0f;
                for (i = 0; i < 4; i++) {
                    player[i]->mode = 2;
                }
            }
            break;
        case 0x3E9:
            lbl_1_bss_0 += 1.0f;
            if (lbl_1_bss_0 > 30.0f) {
                lbl_1_bss_80++;
                lbl_1_bss_0 = 0.0f;
                Hu3DModelAttrSet(lbl_1_bss_88->mdlId[17], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrSet(lbl_1_bss_88->mdlId[18], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrSet(lbl_1_bss_88->mdlId[19], HU3D_ATTR_DISPOFF);
                Hu3DShadowMultiTPLvlSet(0.625f, -1);
                Hu3DModelAttrReset(lbl_1_bss_88->mdlId[14], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrReset(lbl_1_bss_88->mdlId[15], HU3D_ATTR_DISPOFF);
                Hu3DModelLayerSet(lbl_1_bss_88->mdlId[2], 7);
                Hu3DModelLayerSet(lbl_1_bss_88->mdlId[4], 7);
                Hu3DModelLayerSet(lbl_1_bss_88->mdlId[6], 7);
                Hu3DModelCameraSet(lbl_1_bss_88->mdlId[16], HU3D_CAM0);
                Hu3DModelLayerSet(lbl_1_bss_88->mdlId[16], 3);
                Hu3DMotionTimeSet(lbl_1_bss_88->mdlId[16], 0.0f);
                for (i = 1; i < 4; i++) {
                    Hu3DModelAmbSet(lbl_1_bss_8C[i]->mdlId[0], 0.375f, 0.375f, 0.375f);
                    Hu3DLLightColorSet(lbl_1_bss_8C[i]->mdlId[0], player[i]->lightId, 31, 31, 31, 255);
                    Hu3DLLightPosSet(lbl_1_bss_8C[i]->mdlId[0], player[i]->lightId, 0.0f, 0.0f, 500.0f, 0.0f, -1.0f, -3.0f);
                }
                if (lbl_1_bss_A4[3].num < 3) {
                    lbl_1_bss_A4[3].pan[lbl_1_bss_A4[3].num] = 0.0f;
                    lbl_1_bss_A4[3].num++;
                }
            }
            break;
        case 0x3EA:
            lbl_1_bss_0 += 1.0f;
            lbl_1_bss_0 == 20.0f;
            if (lbl_1_bss_0 > 60.0f) {
                lbl_1_bss_80++;
                lbl_1_bss_0 = 0.0f;
            }
            break;
        case 0x3EB:
            lbl_1_bss_0 += 1.0f;
            if (lbl_1_bss_0 > 180.0f) {
                lbl_1_bss_80++;
                lbl_1_bss_0 = 0.0f;
                Hu3DModelCameraSet(lbl_1_bss_88->mdlId[0], HU3D_CAM0);
                Hu3DModelCameraSet(lbl_1_bss_88->mdlId[1], HU3D_CAM0);
                Hu3DModelCameraSet(lbl_1_bss_88->mdlId[2], HU3D_CAM0);
                Hu3DModelCameraSet(lbl_1_bss_88->mdlId[4], HU3D_CAM0);
                Hu3DModelCameraSet(lbl_1_bss_88->mdlId[6], HU3D_CAM0);
                Hu3DModelCameraSet(lbl_1_bss_88->mdlId[10], HU3D_CAM0);
                Hu3DModelCameraSet(lbl_1_bss_88->mdlId[11], HU3D_CAM0);
                Hu3DModelCameraSet(lbl_1_bss_88->mdlId[12], HU3D_CAM0);
                Hu3DModelCameraSet(lbl_1_bss_88->mdlId[16], HU3D_CAM0);
                Hu3DModelCameraSet(lbl_1_bss_88->mdlId[63], HU3D_CAM0);
                Hu3DModelCameraSet(lbl_1_bss_88->mdlId[14], HU3D_CAM1);
                for (i = 1; i < 4; i++) {
                    Hu3DModelAmbSet(lbl_1_bss_8C[i]->mdlId[0], 1.0f, 1.0f, 1.0f);
                }
                for (i = 0; i < 4; i++) {
                    Hu3DModelLayerSet(lbl_1_bss_88->mdlId[i + 23], 2);
                }
                for (i = 0; i < 20; i++) {
                    Hu3DModelCameraSet(lbl_1_bss_88->mdlId[i + 23], HU3D_CAM0);
                    Hu3DModelLayerSet(lbl_1_bss_88->mdlId[i + 23], 4);
                    Hu3DModelShadowDispOff(lbl_1_bss_88->mdlId[i + 23]);
                }
                Hu3DModelAttrSet(lbl_1_bss_88->mdlId[15], HU3D_ATTR_DISPOFF);
                Hu3DModelPosSet(lbl_1_bss_88->mdlId[14], 0.45f, 0.0f, 0.0f);
                Hu3DModelAttrSet(lbl_1_bss_88->mdlId[13], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrSet(lbl_1_bss_88->mdlId[3], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrSet(lbl_1_bss_88->mdlId[5], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrSet(lbl_1_bss_88->mdlId[7], HU3D_ATTR_DISPOFF);
                Hu3DModelLayerSet(lbl_1_bss_88->mdlId[2], 0);
                Hu3DModelLayerSet(lbl_1_bss_88->mdlId[4], 0);
                Hu3DModelLayerSet(lbl_1_bss_88->mdlId[6], 0);
                Hu3DModelLayerSet(lbl_1_bss_88->mdlId[16], 0);
                Hu3DModelShadowMapObjReset(lbl_1_bss_88->mdlId[0]);
                Hu3DModelShadowMapObjReset(lbl_1_bss_88->mdlId[1]);
                Hu3DModelShadowMapObjReset(lbl_1_bss_88->mdlId[2]);
                Hu3DModelShadowMapObjReset(lbl_1_bss_88->mdlId[12]);
                Hu3DModelShadowMapObjReset(lbl_1_bss_88->mdlId[6]);
                Hu3DModelShadowMapObjReset(lbl_1_bss_88->mdlId[11]);
                lbl_1_bss_4C.x = 0.0f;
                lbl_1_bss_4C.y = 10000.0f;
                lbl_1_bss_4C.z = 0.0f;
                lbl_1_bss_40.x = 0.0f;
                lbl_1_bss_40.y = 0.0f;
                lbl_1_bss_40.z = -1.0f;
                lbl_1_bss_34.x = 0.0f;
                lbl_1_bss_34.y = 0.0f;
                lbl_1_bss_34.z = 0.0f;
                Hu3DShadowMultiPosSet(&lbl_1_bss_4C, &lbl_1_bss_40, &lbl_1_bss_34, -1);
            }
            break;
        case 0x3EC:
            lbl_1_bss_0 += 1.0f;
            if (lbl_1_bss_0 == 1.0f && lbl_1_bss_A4[2].seNo != -1) {
                HuAudFXFadeOut(lbl_1_bss_A4[2].seNo, 2000);
                lbl_1_bss_A4[2].seNo = -1;
            }
            if (lbl_1_bss_0 == 21.0f) {
                Hu3DModelAttrReset(lbl_1_bss_88->mdlId[16], HU3D_MOTATTR_PAUSE);
            }
            if (!(lbl_1_bss_0 < 20.0f)) {
                if (lbl_1_bss_0 < 80.0f) {
                    Hu3DModelPosSet(lbl_1_bss_88->mdlId[14], 0.44999999999999996 + (0.55 * ((lbl_1_bss_0 - 20.0f) / 60.0f)), 0.0f, 0.0f);
                } else if (lbl_1_bss_0 == 80.0f) {
                    Hu3DModelAttrSet(lbl_1_bss_88->mdlId[14], HU3D_ATTR_DISPOFF);
                }
            }
            if (lbl_1_bss_0 > 20.0f && lbl_1_bss_0 <= 80.0f) {
                float t = (lbl_1_bss_0 - 20.0f) / 60.0f;
                float ti = 1.0f - t;
                float col = 255.0 * (t + (0.2777777777777778 * ti));

                for (i = 1; i < 4; i++) {
                    Hu3DLLightColorSet(lbl_1_bss_8C[i]->mdlId[0], player[i]->lightId, col, col, col, 255);
                    Hu3DLLightPosSet(lbl_1_bss_8C[i]->mdlId[0], player[i]->lightId, 0.0f, 500.0f * t, 500.0f * ti, 0.0f,
                                     (-3.0f * t) + (-2.0f * ti), (-1.0f * t) + (-2.0f * ti));
                }
            }
            if (lbl_1_bss_0 == 5.0f && lbl_1_bss_7A < 0) {
                lbl_1_bss_7A = GameMesMgStartCreate();
            }
            if (lbl_1_bss_0 >= 5.0f && GameMesStatGet(lbl_1_bss_7A) == 0 && lbl_1_bss_7A >= 0) {
                lbl_1_bss_80++;
                lbl_1_bss_74 = 1800;
                lbl_1_bss_7A = -1;
                lbl_1_bss_0 = 0.0f;
                for (i = 0; i < 4; i++) {
                    Hu3DModelCameraSet(lbl_1_bss_8C[i]->mdlId[0], HU3D_CAM0);
                    Hu3DModelCameraSet(lbl_1_bss_88->mdlId[16], HU3D_CAM0);
                }
            }
            break;
        case 0x3ED:
            lbl_1_bss_0 += 1.0f;
            if (lbl_1_bss_74 == 1800) {
                lbl_1_bss_78 = GameMesTimerCreate(lbl_1_bss_74 / 60);
                GameMesPosSet(lbl_1_bss_78, 288.0f, 64.0f);
            }
            if (lbl_1_bss_74 <= 3600) {
                GameMesDispSet(lbl_1_bss_78, 1, (lbl_1_bss_74 + 58) / 60);
            }
            lbl_1_bss_74--;
            if (lbl_1_bss_74 <= 0 || lbl_1_data_FC != -1) {
                lbl_1_bss_80 = 0x3EE;
                lbl_1_bss_0 = 0.0f;
                lbl_1_data_FC = (lbl_1_data_FC != -1) ? lbl_1_data_FC : 1;
                if (lbl_1_data_FC == 1) {
                    for (i = 0; i < 20; i++) {
                        lbl_1_bss_316C[i].hit = 1;
                    }
                }
            }
            break;
        case 0x3EE:
            if (lbl_1_bss_7A < 0 && lbl_1_bss_0 == 0.0f) {
                GameMesDispSet(lbl_1_bss_78, 2, -1);
                lbl_1_bss_7A = GameMesMgFinishCreate();
                if (lbl_1_data_100 != -1) {
                    HuAudSStreamFadeOut(lbl_1_data_100, 100);
                }
            } else if (GameMesStatGet(lbl_1_bss_7A) == 0 && lbl_1_bss_0 >= 180.0f) {
                lbl_1_bss_80++;
                lbl_1_bss_0 = 0.0f;
                if (lbl_1_bss_A4[1].seNo != -1) {
                    HuAudFXStop(lbl_1_bss_A4[1].seNo);
                    lbl_1_bss_A4[1].seNo = -1;
                }
                if (lbl_1_bss_A4[0].num < 3) {
                    lbl_1_bss_A4[0].pan[lbl_1_bss_A4[0].num] = 0.0f;
                    lbl_1_bss_A4[0].num++;
                }
                if (lbl_1_data_FC != 4) {
                    GWMgCoinBonusSet(player[lbl_1_data_FC]->playerNo, 10);
                    if (lbl_1_data_FC != 0) {
                        GWMgCoinBonusSet(player[2]->playerNo, 10);
                        GWMgCoinBonusSet(player[3]->playerNo, 10);
                    }
                    lbl_1_bss_7A = GameMesMgWinnerCreate(player[lbl_1_data_FC]->charNo, (lbl_1_data_FC == 0) ? -1 : player[2]->charNo,
                                                         (lbl_1_data_FC == 0) ? -1 : player[3]->charNo, -1);
                    HuAudSStreamPlay(0x79);
                } else {
                    lbl_1_bss_7A = GameMesMgDrawCreate();
                    HuAudSStreamPlay(0x7B);
                }
            }
            lbl_1_bss_0 += (lbl_1_data_FC == 0)
                ? ((lbl_1_bss_0 < 175.0f || player[0]->mode == 4) ? 1 : 0)
                : ((lbl_1_bss_0 != 60.0f
                    || ((player[1]->down != 0 || player[1]->motNo == 0) && (player[2]->down != 0 || player[2]->motNo == 0)
                        && (player[3]->down != 0 || player[3]->motNo == 0)))
                       ? 1
                       : 0);
            if (lbl_1_data_FC == 1 && lbl_1_bss_0 == 61.0f) {
                s32 rank[3] = { -2, -2, -2 };

                count = 0;
                for (j = 0; j < 3; j++) {
                    carry = -1;
                    if (player[j + 1]->down == 0) {
                        count++;
                        for (i = 0; i < 3; i++) {
                            if (carry == -1) {
                                if (rank[i] == -2 || lbl_1_bss_8C[rank[i] + 1]->trans.x > lbl_1_bss_8C[j + 1]->trans.x) {
                                    carry = rank[i];
                                    rank[i] = j;
                                }
                            } else {
                                tmp = rank[i];
                                rank[i] = carry;
                                carry = tmp;
                            }
                        }
                    }
                }
                for (i = 0; i < 3; i++) {
                    if (rank[i] >= 0) {
                        player[rank[i] + 1]->rankOfs = i - ((count - 1) / 2.0f);
                    }
                }
                for (i = 1; i < 4; i++) {
                    if (player[i]->down == 0) {
                        player[i]->mode = 4;
                        player[i]->startX = lbl_1_bss_8C[i]->trans.x;
                        player[i]->startY = lbl_1_bss_8C[i]->trans.y;
                        player[i]->throwState = 0;
                    }
                }
                if (lbl_1_bss_A4[8].num < 3) {
                    lbl_1_bss_A4[8].pan[lbl_1_bss_A4[8].num] = 0.0f;
                    lbl_1_bss_A4[8].num++;
                }
                for (i = 0; i < 20; i++) {
                    if (lbl_1_bss_316C[i].time != 0) {
                        lbl_1_bss_316C[i].time = 0;
                        Hu3DModelAttrSet(lbl_1_bss_88->mdlId[i + 23], HU3D_ATTR_DISPOFF);
                    }
                }
            }
            break;
        case 0x3EF:
            lbl_1_bss_0 += 1.0f;
            for (i = 0; i < 4; i++) {
                if (lbl_1_data_FC == 4 || lbl_1_data_FC == i / 2) {
                    player[i]->mode = (player[i]->mode == 4) ? 6 : player[i]->mode;
                }
            }
            if (210.0f < lbl_1_bss_0 && lbl_1_bss_10[0] == 0) {
                if (lbl_1_bss_A4[0].seNo != -1) {
                    HuAudFXFadeOut(lbl_1_bss_A4[0].seNo, 1000);
                    lbl_1_bss_A4[0].seNo = -1;
                }
                lbl_1_bss_7A = -1;
                lbl_1_bss_10[0] = 1;
            }
            break;
        default:
            OSReport("*** main mode error(%d)!!\n", lbl_1_bss_80);
            break;
    }
}

// NONMATCHING: 73.37%, and the remaining 326 differing rows are 290 pure
// register renames plus 36 stack-slot placements, not logic. The renames are
// the r0-vs-r3/r4/r5 choice for the mdlNo*2 index and the rodata address
// scratch; declaration order, register hints and splitting have all been tried
// and none moved it. The 36 real ones are the local swingCopy landing at
// 0x1c(r1) instead of 0x8(r1) (the target copies lbl_1_bss_339C into a local
// then memsets 0x14 bytes of it before the field writes).
static void fn_1_A690(OMOBJ *obj)
{
    s32 mdlNo;
    s32 i;
    HU3DLLIGHTID lightId;
    M525Swing swingCopy;
    M525Swing *swingP;

    mdlNo = 0;
    obj->mdlId[mdlNo] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m525, 6), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(obj->mdlId[mdlNo], 0);
    Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_MOTATTR_LOOP);
    lightId = Hu3DLLightCreate(obj->mdlId[mdlNo], 0.0f, -500.0f, 500.0f, 0.0f, 1.0f, -1.0f, 255, 255, 255);
    Hu3DLLightInfinitytSet(obj->mdlId[mdlNo], lightId);
    Hu3DLLightStaticSet(obj->mdlId[mdlNo], lightId, TRUE);
    Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM1);
    Hu3DModelShadowMapSet(obj->mdlId[mdlNo]);
    mdlNo = 1;
    obj->mdlId[mdlNo] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m525, 7), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(obj->mdlId[mdlNo], 0);
    Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_MOTATTR_LOOP);
    lightId = Hu3DLLightCreate(obj->mdlId[mdlNo], 0.0f, -500.0f, 500.0f, 0.0f, 1.0f, -1.0f, 255, 255, 255);
    Hu3DLLightInfinitytSet(obj->mdlId[mdlNo], lightId);
    Hu3DLLightStaticSet(obj->mdlId[mdlNo], lightId, TRUE);
    Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM1);
    Hu3DModelShadowMapSet(obj->mdlId[mdlNo]);
    mdlNo = 2;
    obj->mdlId[mdlNo] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m525, 0), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(obj->mdlId[mdlNo], 0);
    Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_MOTATTR_LOOP);
    lightId = Hu3DLLightCreate(obj->mdlId[mdlNo], 0.0f, 500.0f, 500.0f, 0.0f, -1.0f, -1.0f, 255, 255, 255);
    Hu3DLLightInfinitytSet(obj->mdlId[mdlNo], lightId);
    Hu3DLLightStaticSet(obj->mdlId[mdlNo], lightId, TRUE);
    swingCopy = lbl_1_bss_339C;
    swingP = &swingCopy;
    memset(swingP, 0, sizeof(M525Swing));
    lbl_1_bss_339C.pivotY = 1400.0f;
    lbl_1_bss_339C.angle = -35.0f;
    lbl_1_bss_339C.speed = 0.0f;
    lbl_1_bss_339C.accel = 40.0f;
    lbl_1_bss_339C.accel = fabs(lbl_1_bss_339C.angle / ((1.0f + lbl_1_bss_339C.accel) * (lbl_1_bss_339C.accel * 0.5f)));
    Hu3DModelPosSet(obj->mdlId[mdlNo], 0.0f, lbl_1_bss_339C.pivotY, 0.0f);
    Hu3DModelRotSet(obj->mdlId[mdlNo], 0.0f, 0.0f, lbl_1_bss_339C.angle);
    Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM0);
    Hu3DModelShadowMapSet(obj->mdlId[mdlNo]);
    obj->mdlId[mdlNo + 1] = Hu3DModelLink(obj->mdlId[mdlNo]);
    Hu3DModelPosSet(obj->mdlId[mdlNo + 1], 0.0f, lbl_1_bss_339C.pivotY, 0.0f);
    Hu3DModelCameraSet(obj->mdlId[mdlNo + 1], HU3D_CAM1);
    Hu3DModelShadowSet(obj->mdlId[mdlNo + 1]);
    Hu3DModelAttrSet(obj->mdlId[mdlNo + 1], HU3D_ATTR_MOTION_OFF);
    for (i = 0; i < 2; i++) {
        mdlNo = (i * 2) + 4;
        obj->mdlId[mdlNo] = Hu3DModelCreate(HuDataSelHeapReadNum((i == 0) ? DATANUM(DATA_m525, 1) : DATANUM(DATA_m525, 2), HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelLayerSet(obj->mdlId[mdlNo], 0);
        Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_MOTATTR_LOOP);
        lightId = Hu3DLLightCreate(obj->mdlId[mdlNo], 0.0f, 500.0f, 500.0f, 0.0f, -1.0f, -1.0f, 255, 255, 255);
        Hu3DLLightInfinitytSet(obj->mdlId[mdlNo], lightId);
        Hu3DLLightStaticSet(obj->mdlId[mdlNo], lightId, TRUE);
        Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM0);
        Hu3DModelShadowMapSet(obj->mdlId[mdlNo]);
        obj->mdlId[mdlNo + 1] = Hu3DModelLink(obj->mdlId[mdlNo]);
        Hu3DModelCameraSet(obj->mdlId[mdlNo + 1], HU3D_CAM1);
        Hu3DModelShadowSet(obj->mdlId[mdlNo + 1]);
        Hu3DModelAttrSet(obj->mdlId[mdlNo + 1], HU3D_ATTR_MOTION_OFF);
    }
    mdlNo = 8;
    obj->mdlId[mdlNo] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m525, 3), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(obj->mdlId[mdlNo], 0);
    Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_MOTATTR_LOOP);
    lightId = Hu3DLLightCreate(obj->mdlId[mdlNo], 0.0f, 500.0f, 500.0f, 0.0f, -1.0f, -1.0f, 255, 255, 255);
    Hu3DLLightInfinitytSet(obj->mdlId[mdlNo], lightId);
    Hu3DLLightStaticSet(obj->mdlId[mdlNo], lightId, TRUE);
    Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM0);
    for (i = 0; i < 2; i++) {
        mdlNo = i + 10;
        obj->mdlId[mdlNo] = Hu3DModelCreate(HuDataSelHeapReadNum((i == 0) ? DATANUM(DATA_m525, 4) : DATANUM(DATA_m525, 5), HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelLayerSet(obj->mdlId[mdlNo], 0);
        Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_MOTATTR_LOOP);
        lightId = Hu3DLLightCreate(obj->mdlId[mdlNo], 0.0f, 500.0f, 500.0f, 0.0f, -1.0f, -1.0f, 255, 255, 255);
        Hu3DLLightInfinitytSet(obj->mdlId[mdlNo], lightId);
        Hu3DLLightStaticSet(obj->mdlId[mdlNo], lightId, TRUE);
        Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM1);
        Hu3DModelShadowMapSet(obj->mdlId[mdlNo]);
    }
    mdlNo = 12;
    obj->mdlId[mdlNo] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m525, 8), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(obj->mdlId[mdlNo], 0);
    Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_MOTATTR_LOOP);
    lightId = Hu3DLLightCreate(obj->mdlId[mdlNo], 0.0f, -500.0f, 500.0f, 0.0f, 1.0f, -1.0f, 255, 255, 255);
    Hu3DLLightInfinitytSet(obj->mdlId[mdlNo], lightId);
    Hu3DLLightStaticSet(obj->mdlId[mdlNo], lightId, TRUE);
    Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM1);
    Hu3DModelShadowMapSet(obj->mdlId[mdlNo]);
    mdlNo = 16;
    obj->mdlId[mdlNo] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m525, 9), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(obj->mdlId[mdlNo], 7);
    Hu3DMotionTimeSet(obj->mdlId[mdlNo], 10.0f);
    Hu3DMotionSpeedSet(obj->mdlId[mdlNo], 0.25f);
    Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_MOTATTR_PAUSE);
    lightId = Hu3DLLightCreate(obj->mdlId[mdlNo], 0.0f, -500.0f, 500.0f, 0.0f, 1.0f, -1.0f, 255, 255, 255);
    Hu3DLLightInfinitytSet(obj->mdlId[mdlNo], lightId);
    Hu3DLLightStaticSet(obj->mdlId[mdlNo], lightId, TRUE);
    Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM1);
    memset(lbl_1_bss_316C, 0, sizeof(lbl_1_bss_316C));
    for (i = 0; i < 20; i++) {
        s32 itemData[5] = { DATANUM(DATA_m525, 13), DATANUM(DATA_m525, 14), DATANUM(DATA_m525, 15), DATANUM(DATA_m525, 16), DATANUM(DATA_m525, 17) };

        mdlNo = i + 23;
        if (i < 5) {
            obj->mdlId[mdlNo] = Hu3DModelCreate(HuDataSelHeapReadNum(itemData[i], HU_MEMNUM_OVL, HEAP_MODEL));
        } else {
            obj->mdlId[mdlNo] = Hu3DModelLink(obj->mdlId[(i % 5) + 23]);
        }
        Hu3DModelLayerSet(obj->mdlId[mdlNo], 0);
        Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_ATTR_DISPOFF | HU3D_ATTR_ZCMP_OFF);
        Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM1);
        lightId = Hu3DLLightCreate(obj->mdlId[mdlNo], 0.0f, 500.0f, 500.0f, 0.0f, -1.0f, -1.0f, 255, 255, 255);
        Hu3DLLightInfinitytSet(obj->mdlId[mdlNo], lightId);
        Hu3DLLightStaticSet(obj->mdlId[mdlNo], lightId, TRUE);
        lbl_1_bss_316C[i].lightId = lightId;
        obj->mdlId[mdlNo + 20] = Hu3DModelLink(obj->mdlId[mdlNo]);
        Hu3DModelCameraSet(obj->mdlId[mdlNo + 20], HU3D_CAM1);
        Hu3DModelShadowSet(obj->mdlId[mdlNo + 20]);
        Hu3DModelAttrSet(obj->mdlId[mdlNo + 20], HU3D_ATTR_MOTION_OFF);
    }
    for (i = 0; i < 3; i++) {
        mdlNo = i + 17;
        if (i == 0) {
            obj->mdlId[mdlNo] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m525, 10), HU_MEMNUM_OVL, HEAP_MODEL));
        } else {
            obj->mdlId[mdlNo] = Hu3DModelLink(obj->mdlId[17]);
        }
        Hu3DModelLayerSet(obj->mdlId[mdlNo], 3);
        Hu3DModelPosSet(obj->mdlId[mdlNo], lbl_1_data_160[i + 1][0], lbl_1_data_160[i + 1][1], lbl_1_data_160[i + 1][2]);
        Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM1);
    }
    for (i = 0; i < 3; i++) {
        mdlNo = i + 20;
        if (i == 0) {
            obj->mdlId[mdlNo] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m525, 11), HU_MEMNUM_OVL, HEAP_MODEL));
        } else {
            obj->mdlId[mdlNo] = Hu3DModelLink(obj->mdlId[20]);
        }
        Hu3DModelLayerSet(obj->mdlId[mdlNo], 7);
        Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM0);
        Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_ATTR_DISPOFF);
    }
    mdlNo = 13;
    obj->mdlId[mdlNo] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m525, 12), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(obj->mdlId[mdlNo], 0);
    Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_ATTR_MOTION_OFF);
    Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM0 | HU3D_CAM1);
    Hu3DModelShadowSet(obj->mdlId[mdlNo]);
    Hu3DModelScaleSet(obj->mdlId[mdlNo], 2.0f, 2.0f, 2.0f);
    lbl_1_bss_58 = espEntry(DATANUM(DATA_m525, 18), 0, 0);
    espDispOff(lbl_1_bss_58);
    mdlNo = 63;
    obj->mdlId[mdlNo] = Hu3DHookFuncCreate(fn_1_7698);
    Hu3DModelLayerSet(obj->mdlId[mdlNo], 3);
    Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM1);
    mdlNo = 14;
    obj->mdlId[mdlNo] = Hu3DHookFuncCreate(fn_1_8394);
    Hu3DModelLayerSet(obj->mdlId[mdlNo], 6);
    Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM0);
    Hu3DModelPosSet(obj->mdlId[mdlNo], 0.375f, 0.0f, 0.0f);
    Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_ATTR_DISPOFF);
    mdlNo = 15;
    obj->mdlId[mdlNo] = Hu3DHookFuncCreate(fn_1_8394);
    Hu3DModelLayerSet(obj->mdlId[mdlNo], 6);
    Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM1);
    Hu3DModelPosSet(obj->mdlId[mdlNo], 1.2f, 0.0f, 0.0f);
    Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_ATTR_DISPOFF);
    memset(lbl_1_bss_16C, 0, sizeof(lbl_1_bss_16C));
    mdlNo = 64;
    obj->mdlId[mdlNo] = Hu3DHookFuncCreate(fn_1_7C60);
    Hu3DModelLayerSet(obj->mdlId[mdlNo], 7);
    Hu3DModelCameraSet(obj->mdlId[mdlNo], HU3D_CAM1);
    Hu3DModelAttrSet(obj->mdlId[mdlNo], HU3D_ATTR_DISPOFF);
    for (i = 0; i < 256; i++) {
        lbl_1_bss_16C[i].time = i;
        lbl_1_bss_16C[i].y = -1000.0f;
    }
    obj->objFunc = fn_1_5AF4;
}
/* Player object setup: 0 = swinger, 1..3 = runners; teams ordered by GwPlayerConf group. */
static void fn_1_B930(OMOBJ *obj)
{
    M525Player *player;
    s32 order[4] = { 0, 1, 2, 3 };
    s32 num;
    s32 isRunner;
    s32 i;
    s32 j;

    num = 0;
    isRunner = obj->work[0] != 0;
    player = obj->data;
    memset(player, 0, sizeof(M525Player));
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
    player->grounded = 1;
    player->mode = 2;
    player->aiState = 0;
    if (player->no == 0) {
        player->throwTime = 255;
    }
    obj->mdlId[0] = CharModelCreate(player->charNo, (player->no == 0) ? 2 : 2);
    Hu3DModelLayerSet(obj->mdlId[0], 2);
    Hu3DModelCameraSet(obj->mdlId[0], HU3D_CAM1);
    Hu3DModelAttrReset(obj->mdlId[0], HU3D_MOTATTR_PAUSE);
    Hu3DModelAttrSet(obj->mdlId[0], HU3D_MOTATTR_LOOP);
    if (player->no == 0) {
        Hu3DModelShadowSet(obj->mdlId[0]);
    }
    CharEffectLayerSet(3);
    player->lightId = Hu3DLLightCreate(obj->mdlId[0], 0.0f, 500.0f, 0.0f, 0.0f, -3.0f, -1.0f, 255, 255, 255);
    Hu3DLLightInfinitytSet(obj->mdlId[0], player->lightId);
    Hu3DLLightStaticSet(obj->mdlId[0], player->lightId, TRUE);
    Hu3DModelAmbSet(obj->mdlId[0], 1.0f, 1.0f, 1.0f);
    player->motNo = (player->no == 0) ? 1 : 0;
    player->nextMotNo = -1;
    for (i = 0; i < (isRunner == 0 ? 6 : 10); i++) {
        obj->mtnId[i] = CharMotionCreate(player->charNo, lbl_1_data_34[isRunner][i]);
    }
    Hu3DMotionSet(obj->mdlId[0], obj->mtnId[player->motNo]);
    Hu3DModelAttrSet(obj->mdlId[0], HU3D_MOTATTR_LOOP);
    obj->trans.x = lbl_1_data_160[player->no][0];
    obj->trans.y = lbl_1_data_160[player->no][1];
    obj->trans.z = lbl_1_data_160[player->no][2];
    obj->rot.y = lbl_1_data_160[player->no][3];
    obj->scale.x = obj->scale.y = obj->scale.z = 1.0f;
    obj->rot.x = obj->rot.z = 0.0f;
    obj->objFunc = fn_1_654;
}

static void fn_1_BDB4(OMOBJ *obj)
{
    lbl_1_bss_80 = 0x3E8;
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_PREVTYPE, 60);
    obj->objFunc = fn_1_889C;
}

void ObjectSetup(void)
{
    s32 i;
    s32 j;
    OMOBJ *obj;

    OSReport("******* M525 ObjectSetup *********\n");
    lbl_1_bss_7C = omInitObjMan(600, 0x2000);
    omGameSysInit(lbl_1_bss_7C);
    Hu3DCameraCreate(HU3D_CAM0 | HU3D_CAM1);
    for (i = 0; i < 2; i++) {
        s32 w = 640;
        s32 h = (w * 3) / 4;
        s32 x = (640 - w) / 2;
        s32 y = (480 - h) / 2;

        Hu3DCameraPerspectiveSet((i == 0) ? HU3D_CAM0 : HU3D_CAM1, 25.0f, 40.0f, 20000.0f, 1.2f);
        Hu3DCameraViewportSet((i == 0) ? HU3D_CAM0 : HU3D_CAM1, x, y, w, h, 0.0f, 1.0f);
        Hu3DCameraScissorSet((i == 0) ? HU3D_CAM0 : HU3D_CAM1, x, y, w, h);
    }
    for (i = 0; i < 2; i++) {
        CZoomM[i] = 2800.0f;
        CenterM[i].x = 0.0f;
        CenterM[i].y = 390.0f;
        CenterM[i].z = 0.0f;
        CRotM[i].x = 0.0f;
        CRotM[i].y = 0.0f;
        CRotM[i].z = 0.0f;
    }
    Hu3DLighInit();
    {
        Vec lightPos = { 0.0f, 5000.0f, 0.0f };
        Vec lightDir = { 0.75f, -1.0f, -0.5f };
        GXColor lightColor = { 0, 0, 0, 0 };

        lbl_1_bss_4C.x = -300.0f;
        lbl_1_bss_4C.y = -400.0f;
        lbl_1_bss_4C.z = 3000.0f;
        lbl_1_bss_40.x = 0.0f;
        lbl_1_bss_40.y = 1.0f;
        lbl_1_bss_40.z = 0.0f;
        lbl_1_bss_34.x = -50.0f;
        lbl_1_bss_34.y = 260.0f;
        lbl_1_bss_34.z = -20.0f;
        Hu3DShadowMultiCreate(32.0f, 200.0f, 20000.0f, HU3D_CAM0 | HU3D_CAM1);
        Hu3DShadowMultiPosSet(&lbl_1_bss_4C, &lbl_1_bss_40, &lbl_1_bss_34, -1);
        Hu3DShadowMultiTPLvlSet(0.0f, -1);
        Hu3DShadowMultiColSet(0, 0, 0, -1);
        lbl_1_bss_7A = -1;
        lbl_1_data_FC = -1;
        lbl_1_bss_88 = omAddObjEx(lbl_1_bss_7C, 101, 65, 0, 1, fn_1_A690);
        for (i = 0; i < 4; i++) {
            obj = lbl_1_bss_8C[i] = omAddObjEx(lbl_1_bss_7C, 100, 2, 10, 0, fn_1_B930);
            obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M525Player), HU_MEMNUM_OVL);
            obj->work[0] = i;
        }
    }
    {
        float xs[3];
        s32 used[3] = { 0, 0, 0 };

        xs[0] = lbl_1_data_160[1][0];
        xs[1] = lbl_1_data_160[2][0];
        xs[2] = lbl_1_data_160[3][0];
        for (i = 0; i < 3; i++) {
            do {
                j = rand8();
            } while (j % 4 == 3 || used[j % 3] == 1);
            j = j % 3;
            used[j] = 1;
            lbl_1_data_160[i + 1][0] = xs[j];
        }
    }
    lbl_1_bss_9C = omAddObjEx(lbl_1_bss_7C, 0x7FDA, 0, 0, -1, omOutViewMulti);
    lbl_1_bss_9C->work[0] = 2;
    lbl_1_bss_A0 = omAddObjEx(lbl_1_bss_7C, 102, 0, 0, -1, fn_1_BDB4);
    lbl_1_bss_84 = omAddObjEx(lbl_1_bss_7C, 103, 1, 0, -1, fn_1_C51C);
}

static void fn_1_C51C(OMOBJ *obj)
{
    if (lbl_1_bss_10[0] != 0) {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_WHITEFADE, 60);
        obj->objFunc = fn_1_C574;
    }
}

static void fn_1_C574(OMOBJ *obj)
{
    s32 i;

    if (WipeCheckEnd() == 0) {
        for (i = 0; i < 4; i++) {
            M525Player *player = lbl_1_bss_8C[i]->data;

            CharModelKill(player->charNo);
        }
        if (lbl_1_bss_7A >= 0) {
            GameMesKill(lbl_1_bss_7A);
        }
        GameMesClose();
        HuAudAllStop();
        omOvlReturnEx(lbl_1_data_104, 1);
    }
}
