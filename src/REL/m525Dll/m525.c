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
extern M525ESPRITE esprite[];
extern M525ESPANIM espanim[];
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

static void fn_1_654(OMOBJ *obj)
{
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
   spawns/advances the spark (M525Fx) particles. */
static void fn_1_5AF4(OMOBJ *obj)
{
    M525Player *player[4];
    f32 prevZoom;
    f32 maxX;
    f32 minX;
    f32 prevHeight;
    f32 maxHeight;
    f32 groundY;
    f64 posY;
    f64 dist;
    f64 fall;
    f64 fade;
    f32 camRate;
    f32 camY;
    f32 camZ;
    f32 camRot;
    f32 endY;
    f32 endRot;
    f32 blend;
    f32 zoom;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    f32 rot;
    f32 one;
    f32 rate;
    s32 i;
    s32 j;
    s32 dir;
    s32 leader;
    M525Fx *fx;

    for (i = 0; i < 4; i++) {
        player[i] = lbl_1_bss_8C[i]->data;
    }
    maxX = 0.0f;
    prevZoom = 0.0f;
    minX = -500.0f;
    prevHeight = 0.0f;
    for (i = 0; i < 11; i++) {
        lbl_1_data_108[i] = -500.0f;
        lbl_1_data_134[i] = 0.0f;
    }
    while (maxX < 500.0f) {
        maxX = 500.0f;
        prevZoom = 0.0f;
        for (j = 0; j < 9; j++) {
            posY = player[(j / 3) + 1]->footX[j % 3];
            if (maxX != posY) {
                dist = fabs(posY);
                fall = dist / 1000.0;
                groundY = 33.0f * player[(j / 3) + 1]->footY[j % 3];
                camRate = (1.0 - ((0.875 * fall) + (0.125 * (fall * fall)))) * groundY;                fade = (posY - maxX) / (minX - maxX);
                endY = (prevHeight * (1.0f - fade)) + (prevZoom * fade);
                if (endY > camRate) {
                    maxX = posY;
                    prevZoom = camRate;
                }
            }
        }
        i++;
        lbl_1_data_108[i] = maxX;
        lbl_1_data_134[i] = prevZoom;
        minX = maxX;
        prevHeight = prevZoom;
    }
    for (i = 0; i < 3; i++) {
        if (player[i + 1]->down == 0 && player[i + 1]->mode < 4 && player[i + 1]->grounded != 0) {
            posY = player[i + 1]->footX[player[i + 1]->footNo];
            groundY = 33.0f * player[i + 1]->footY[player[i + 1]->footNo];
            dist = fabs(posY);
            fall = dist / 1000.0;
            camRate = (1.0 - ((0.875 * fall) + (0.125 * (fall * fall)))) * groundY;
            player[i + 1]->velZ = camRate;
        }
    }
    if (lbl_1_bss_80 <= 1001) {
        for (i = 0; i < 2; i++) {
            CZoomM[i] = 1700.0f;
            CenterM[i].y = 115.0f;
            CRotM[i].x = 5.0f;
        }
    }
    if (lbl_1_bss_80 == 1003) {
        for (i = 0; i < 2; i++) {
            rate = 1.0f - 0.96f;
            CZoomM[i] = (2400.0f * rate) + (0.96f * CZoomM[i]);
            CenterM[i].y = (390.0f * rate) + (0.96f * CenterM[i].y);
            CRotM[i].x = (18.0f * rate) + (0.96f * CRotM[i].x);
        }
    }
    if (lbl_1_bss_80 == 1004) {
        for (i = 0; i < 2; i++) {
            rate = 1.0f - 0.96f;
            CZoomM[i] = (2900.0f * rate) + (0.96f * CZoomM[i]);
            CenterM[i].y = (390.0f * rate) + (0.96f * CenterM[i].y);
            CRotM[i].x = 0.96f * CRotM[i].x;
        }
    }
    if (lbl_1_bss_80 >= 1006 && lbl_1_data_FC == 1 && lbl_1_bss_0 >= 61.0f) {
        rot = lbl_1_bss_0 - 61.0f;
        endY = 50.0f;
        if (rot < 10.0f) {
            blend = (((1.0f + rot) * 0.5f * rot) / 10.0f) / endY;
        } else if (rot < 40.0f) {
            camRot = (5.5f / endY) + ((rot - 10.0f) / endY);
            blend = camRot;
        } else {
            if (rot < 69.0f) {
                zoom = 1.0f - ((((1.0f + ((70.0f - rot) - 1.0f)) * 0.5f * ((69.0f - rot) - 1.0f)) / 29.0f) / endY);
            } else {
                zoom = 1.0f;
            }
            camRot = zoom;
            blend = camRot;
        }
        fade = blend;
        camRate = blend;
        one = 1.0f - camRate;
        lbl_1_bss_30 = CZoomM[0];
        lbl_1_bss_2C = CenterM[0].x;
        lbl_1_bss_28 = CenterM[0].y;
        lbl_1_bss_24 = CenterM[0].z;
        lbl_1_bss_20 = CRotM[0].x;
        if (lbl_1_bss_0 >= 81.0f) {
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
            CZoomM[i] = (1300.0f * camRate) + (lbl_1_bss_30 * one);
            CenterM[i].x = lbl_1_bss_2C * one;
            CenterM[i].y = (461.0f * camRate) + (lbl_1_bss_28 * one);
            CenterM[i].z = (-1500.0f * camRate) + (lbl_1_bss_24 * one);
            CRotM[i].x = (-4.0f * camRate) + (lbl_1_bss_20 * one);
        }
    }
    if (lbl_1_bss_80 == 1007) {
        if (lbl_1_data_FC == 0) {
            Hu3DModelPosSet(obj->mdlId[16], player[0]->startX, 100.0f + player[0]->startY, player[0]->unk54);
            Hu3DModelAttrReset(obj->mdlId[13], 1);
            Hu3DModelPosSet(obj->mdlId[13], player[0]->startX, 100.0f + player[0]->startY, player[0]->unk54);
            for (i = 0; i < 2; i++) {
                rate = 1.0f - 0.96f;
                CZoomM[i] = (1000.0f * rate) + (0.96f * CZoomM[i]);
                CenterM[i].x = (0.96f * CenterM[i].x) + (rate * player[0]->startX);
                CenterM[i].y = (0.96f * CenterM[i].y) + (rate * (100.0f + player[0]->startY));
                CenterM[i].z = (0.96f * CenterM[i].z) + (rate * player[0]->unk54);
                CRotM[i].x = (-8.0f * rate) + (0.96f * CRotM[i].x);
                dir = (player[0]->startX < 0.0f) ? -1 : 1;
                CRotM[i].y = (0.96f * CRotM[i].y) + (16.0f * rate * dir);
            }
            lbl_1_bss_4C.x = player[0]->startX;
            lbl_1_bss_4C.y = 2000.0f + player[0]->startY;
            lbl_1_bss_4C.z = player[0]->unk54;
            lbl_1_bss_40.x = 0.0f;
            lbl_1_bss_40.y = 0.0f;
            lbl_1_bss_40.z = -1.0f;
            lbl_1_bss_34.x = player[0]->startX;
            lbl_1_bss_34.y = player[0]->startY;
            lbl_1_bss_34.z = player[0]->unk54;
            Hu3DShadowMultiPosSet(&lbl_1_bss_4C, &lbl_1_bss_40, &lbl_1_bss_34, -1);
        } else {
            Hu3DModelPosSet(obj->mdlId[16], 0.0f, 461.0f, -1500.0f);
            for (i = 0; i < 3; i++) {
                Hu3DModelAttrReset(obj->mdlId[i + 20], 1);
                Hu3DModelPosSet(obj->mdlId[i + 20], player[i + 1]->startX, player[i + 1]->startY, player[i + 1]->unk54);
            }
            for (i = 0; i < 2; i++) {
                rate = 1.0f - 0.9f;
                leader = (player[0]->down == 0) ? 1 : ((player[1]->down == 0) ? 2 : 3);
                CZoomM[i] = (1300.0f * rate) + (0.9f * CZoomM[i]);
                CenterM[i].x = 0.9f * CenterM[i].x;
                CenterM[i].y = (461.0f * rate) + (0.9f * CenterM[i].y);
                CenterM[i].z = (-1500.0f * rate) + (0.9f * CenterM[i].z);
                CRotM[i].x = (-4.0f * rate) + (0.9f * CRotM[i].x);
            }
        }
    }
    if (lbl_1_data_FC != -1 && lbl_1_bss_80 >= 1007) {
        if (lbl_1_data_FC == 1 || (lbl_1_bss_80 == 1007 && lbl_1_bss_0 > 7.0f)) {
            Hu3DModelAttrReset(lbl_1_bss_88->mdlId[16], 1);
        }
        for (i = 0; i < 256; i++) {
            if (lbl_1_bss_16C[i].time >= 256) {
                fx = &lbl_1_bss_16C[i];
                fx->x = 900.0 * ((rand8() / 255.0) - 0.5);
                fx->y = 320.0f;
                fx->z = 0.0f;
                fx->velX = 400.0 * ((rand8() / 255.0) - 0.5);
                fx->velY = (-900.0f + (-100.0f * (rand8() / 255.0))) * 0.00390625f;
                fx->velZ = 0.0f;
                fx->axis.x = 4.0 * ((rand8() / 255.0) - 0.5);
                fx->axis.y = rand8();
                fx->axis.z = (rand8() / 255.0) - 0.5;
                fx->rotSpeed = 0.5 * ((rand8() / 255.0) - 0.5);
                fx->kind = 1.0f / (45.0f + (30.0f * (rand8() / 255.0)));
                if (fx->axis.x == 0.0f && fx->axis.y == 0.0f && fx->axis.z == 0.0f) {
                    fx->axis.x = 1.0f;
                }
                fx->time = 0;
                fx->kind = (rand8() % 4);
            }
            lbl_1_bss_16C[i].x += lbl_1_bss_16C[i].velX;
            lbl_1_bss_16C[i].y += lbl_1_bss_16C[i].velY;
            lbl_1_bss_16C[i].z += lbl_1_bss_16C[i].velZ;
            lbl_1_bss_16C[i].time += 1;
        }
    }
    if (lbl_1_bss_C != 0) {
        if (lbl_1_bss_18[1] > 1 && lbl_1_bss_A4[9].seNo != -1) {
            HuAudFXStop(lbl_1_bss_A4[9].seNo);
            lbl_1_bss_A4[9].seNo = -1;
        }
        if (lbl_1_bss_A4[5].num < 3) {
            lbl_1_bss_A4[lbl_1_bss_A4[5].num].pan[0] = lbl_1_bss_5C[0];
            lbl_1_bss_A4[5].num += 1;
        }
        lbl_1_bss_18[1] = 120;
        lbl_1_bss_C = 0;
    }
    if (lbl_1_bss_18[1] > 0) {
        lbl_1_bss_18[1] -= 1;
    }
    if (lbl_1_bss_18[1] == 1 && lbl_1_bss_A4[9].seNo != -1) {
        HuAudFXStop(lbl_1_bss_A4[9].seNo);
        lbl_1_bss_A4[9].seNo = -1;
    }
    lbl_1_bss_4C.x = 0.0f;
    lbl_1_bss_4C.y = sin(1.5707963267948966);
    lbl_1_bss_4C.z = 10000.0f;
    lbl_1_bss_40.x = 0.0f;
    lbl_1_bss_40.y = 1.0f;
    lbl_1_bss_40.z = 0.0f;
    lbl_1_bss_34.x = player[0]->startX;
    lbl_1_bss_34.y = player[0]->startY;
    lbl_1_bss_34.z = player[0]->unk54;
    Hu3DModelPosSet(obj->mdlId[13], player[0]->startX, player[0]->startY, player[0]->unk54);
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
                    Hu3DModelPosSet(lbl_1_bss_88->mdlId[14], 0.45 + (0.55 * ((lbl_1_bss_0 - 20.0f) / 60.0f)), 0.0f, 0.0f);
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
