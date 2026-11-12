#include "types.h"

#include "string.h"
#include "game/gamework.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/charman.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/audio.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"
#include "game/mg/actman.h"
#include "math.h"
#include "REL/m543Dll.h"

/* m543.c */
extern M543Mg lbl_1_bss_0;
extern M543Cam *lbl_1_bss_18;
extern M543Node lbl_1_bss_1C[40];
extern M543Tile lbl_1_bss_33C[45];
extern M543Actor lbl_1_bss_8DC[2];
extern M543Game lbl_1_bss_B94;
extern M543CamInit lbl_1_data_1D8[4];
void fn_1_91C(s16 arg0);
void fn_1_10C4(void);
void fn_1_14C4(void);
s32 fn_1_1970(f32 *arg0, f32 steps, f32 start, f32 target);
MGTIMER *fn_1_1A8C(s32 type, s32 maxSec, s32 endSec, f32 x, f32 y);
void fn_1_1B00(MGTIMER *timer);
void fn_1_1B54(MGTIMER *timer);
void fn_1_239C(void);
void fn_1_2898(M543Vec3 *arg0, s16 mode);
void fn_1_38EC(void);
void fn_1_391C(s16 isBgm, s16 speed);
void fn_1_39E4(s16 arg0);
void fn_1_3A5C(s16 arg0, s16 vol, s16 pan);
void fn_1_3CCC(void);

/* stage.c */
void fn_1_10150(void);

/* game.c */
void fn_1_6770(void);
s32 fn_1_69BC(s16 arg0);
s16 fn_1_6CF0(s16 arg0);
s32 fn_1_71AC(void);
s32 fn_1_7394(s16 arg0);
s32 fn_1_7704(s16 arg0);
s16 fn_1_86E4(void);
s16 fn_1_91F8(void);
int fn_1_A308(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b);
void fn_1_A69C(MGACTOR *actorP, int param);
void fn_1_A6B0(M543Actor *actors);
void fn_1_D408(s16 arg0);
s32 fn_1_D9E8(s16 arg0);
s16 fn_1_E144(s16 arg0);
s16 fn_1_E340(s16 arg0);
s16 fn_1_EF58(s16 arg0);

void MgActorPosGet(MGACTOR *actorP, HuVecF *pos);

s32 lbl_1_bss_15E4_pad; /* original symbol lbl_1_bss_15E0 is 8 bytes long */
M543State *lbl_1_bss_15E0;

/* unit-owned .data, in original address order */
f32 lbl_1_data_380[16][6] = {
    { 230.0f, -554.0f, 554.0f, -230.0f, 1.0f, 1.0f },
    { 554.0f, -230.0f, 554.0f, -150.0f, 1.0f, 0.0f },
    { 554.0f, -150.0f, 754.0f, -150.0f, 0.0f, 1.0f },
    { 754.0f, -150.0f, 754.0f, 150.0f, 1.0f, 0.0f },
    { 754.0f, 150.0f, 554.0f, 150.0f, 0.0f, 1.0f },
    { 554.0f, 150.0f, 554.0f, 230.0f, 1.0f, 0.0f },
    { 554.0f, 230.0f, 230.0f, 554.0f, 1.0f, 1.0f },
    { 230.0f, 554.0f, -230.0f, 554.0f, 0.0f, 1.0f },
    { -230.0f, 554.0f, -554.0f, 230.0f, 1.0f, 1.0f },
    { -554.0f, 230.0f, -554.0f, 150.0f, 1.0f, 0.0f },
    { -554.0f, 150.0f, -754.0f, 150.0f, 0.0f, 1.0f },
    { -754.0f, 150.0f, -754.0f, -150.0f, 1.0f, 0.0f },
    { -754.0f, -150.0f, -554.0f, -150.0f, 0.0f, 1.0f },
    { -554.0f, -150.0f, -554.0f, -230.0f, 1.0f, 0.0f },
    { -554.0f, -230.0f, -230.0f, -554.0f, 1.0f, 1.0f },
    { -230.0f, -554.0f, 230.0f, -554.0f, 0.0f, 1.0f },
};
f32 lbl_1_data_500[12][2] = {
    { 554.0f, -150.0f },
    { 554.0f, 150.0f },
    { -554.0f, 150.0f },
    { -554.0f, -150.0f },
    { 230.0f, -554.0f },
    { 554.0f, -230.0f },
    { 554.0f, 230.0f },
    { 230.0f, 554.0f },
    { -230.0f, 554.0f },
    { -554.0f, 230.0f },
    { -554.0f, -230.0f },
    { -230.0f, -554.0f },
};
s16 lbl_1_data_560[15][21] = {
    { 16, 46, 21, 26, 48, 44, 29, 45, 34, 15, 47, 19, 39, 23, 28, 41, -1, 0, 0, 0, 0 },
    { 15, 19, 43, 25, 48, 31, 35, 16, 46, 21, 51, 52, 45, 24, 29, 45, -1, 0, 0, 0, 0 },
    { 17, 21, 51, 27, 52, 48, 44, 29, 33, 16, 20, 43, 39, 23, 28, 41, -1, 0, 0, 0, 0 },
    { 16, 20, 25, 48, 31, 49, 45, 33, 15, 17, 21, 40, 19, 24, 44, -1, 0, 0, 0, 0, 0 },
    { 15, 42, 46, 21, 26, 48, 30, 34, 51, 27, 32, 38, 18, 39, 31, 44, 29, -1, 0, 0, 0 },
    { 17, 21, 51, 27, 52, 31, 35, 15, 19, 39, 23, 40, 44, 48, 29, -1, 0, 0, 0, 0, 0 },
    { 16, 42, 19, 43, 25, 30, 45, 33, 39, 24, 46, 17, 50, 22, 27, 52, 31, 49, -1, 0, 0 },
    { 17, 21, 26, 48, 26, 44, 29, 33, 16, 20, 47, 51, 52, 43, 39, 23, 40, 31, -1, 0, 0 },
    { 15, 19, 39, 23, 40, 29, 45, 34, 43, 47, 26, 42, 46, 50, 48, 31, 49, -1, 0, 0, 0 },
    { 16, 46, 21, 26, 31, 49, 30, 44, 29, 33, 17, 51, 27, 15, 19, 39, 23, -1, 0, 0, 0 },
    { 16, 20, 43, 24, 40, 28, 41, 45, 34, 15, 25, 46, 47, 51, 27, 32, 38, 18, 23, -1, 0 },
    { 17, 50, 22, 27, 52, 31, 49, 30, 25, 43, 24, 29, 33, 15, 16, 51, 32, 39, 40, -1, 0 },
    { 15, 19, 24, 44, 48, 52, 32, 53, 35, 38, 18, 17, 50, 22, 51, 26, 29, 30, -1, 0, 0 },
    { 17, 21, 47, 25, 44, 29, 45, 34, 16, 26, 31, 53, 15, 38, 18, 23, 28, 41, -1, 0, 0 },
    { 15, 17, 38, 18, 23, 28, 41, 29, 44, 48, 52, 32, 53, 49, 34, 16, 20, 21, 51, 24, -1 },
};
s16 lbl_1_data_560_pad = 0; /* original symbol lbl_1_data_560 is 0x278 bytes long */
s32 (*lbl_1_data_7F4[2])(s16) = { fn_1_7394, fn_1_7704 };
s16 lbl_1_data_8A0[2][4] = { { 5, -1, 1, -5 }, { 5, -1, 1, -5 } };
s16 lbl_1_data_8B0[2][4] = { { 3, 0, 1, 2 }, { 3, 0, 1, 2 } };
f32 lbl_1_data_8FC[25] = {
    0.0f, 0.0f, 0.0f, 0.0f, 913.0f, 0.0f, -1177.0f, 993.0f, 0.0f, -1038.0f, 550.0f, 1000.0f, -3250.0f,
    593.0f, 312.0f, -1119.0f, -462.0f, 330.0f, -1100.0f, -1000.0f, 40.0f, -2100.0f, -2500.0f, 0.0f, -3900.0f,
};

void fn_1_3CEC(M543State *state)
{
    s32 jointNums[3] = { 0x650010, 0x650011, 0x650012 };
    f32 posTbl[30][2] = {
        { -520.0f, -422.0f },
        { -520.0f, -162.0f },
        { -530.0f, 0.0f },
        { -520.0f, 162.0f },
        { -520.0f, 422.0f },
        { -316.0f, -422.0f },
        { -316.0f, -211.0f },
        { -316.0f, 0.0f },
        { -316.0f, 211.0f },
        { -316.0f, 422.0f },
        { -105.0f, -422.0f },
        { -105.0f, -211.0f },
        { -105.0f, 0.0f },
        { -105.0f, 211.0f },
        { -105.0f, 422.0f },
        { 105.0f, -422.0f },
        { 105.0f, -211.0f },
        { 105.0f, 0.0f },
        { 105.0f, 211.0f },
        { 105.0f, 422.0f },
        { 316.0f, -422.0f },
        { 316.0f, -211.0f },
        { 316.0f, 0.0f },
        { 316.0f, 211.0f },
        { 316.0f, 422.0f },
        { 480.0f, -422.0f },
        { 480.0f, -211.0f },
        { 480.0f, 0.0f },
        { 480.0f, 211.0f },
        { 480.0f, 422.0f },
    };
    s16 adjTbl[40][4] = {
        { -1, -1, -1, -1 },
        { -1, 56, -1, 15 },
        { 56, 57, -1, 16 },
        { 57, -1, -1, 17 },
        { -1, -1, -1, -1 },
        { -1, 38, -1, 18 },
        { 38, 42, 15, 19 },
        { 42, 46, 16, 20 },
        { 46, 50, 17, 21 },
        { 50, -1, -1, 22 },
        { 36, 39, 18, 23 },
        { 39, 43, 19, 24 },
        { 43, 47, 20, 25 },
        { 47, 51, 21, 26 },
        { 51, 54, 22, 27 },
        { 37, 40, 23, 28 },
        { 40, 44, 24, 29 },
        { 44, 48, 25, 30 },
        { 48, 52, 26, 31 },
        { 52, 55, 27, 32 },
        { -1, 41, 28, -1 },
        { 41, 45, 29, 33 },
        { 45, 49, 30, 34 },
        { 49, 53, 31, 35 },
        { 53, -1, 32, -1 },
        { -1, -1, -1, -1 },
        { -1, 58, 33, -1 },
        { 58, 59, 34, -1 },
        { 59, -1, 35, -1 },
        { -1, -1, -1, -1 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
    };
    s16 i;
    s16 j;
    s16 idx;
    s16 rnd;
    s16 sel;
    HU3DMODELID id;
    MGACTOR_PARAM param;

    memset(&lbl_1_bss_B94, 0, 0x74);
    memset(lbl_1_bss_8DC, 0, 0x2B8);
    memset(lbl_1_bss_33C, 0, 0x5A0);
    memset(lbl_1_bss_1C, 0, 0x320);
    memset(&lbl_1_bss_0, 0, 0x10);
    lbl_1_bss_15E0 = state;
    for (i = 0; i < 4; i++) {
        lbl_1_bss_B94.fxOn[i] = -1;
    }
    for (i = 0; i < 0x2D; i++) {
        lbl_1_bss_33C[i].active = 1;
    }
    rnd = frandmod(1000);
    for (i = 1; i < rnd; i++) {
        frandmod(i);
    }
    sel = frandmod(15);
    for (i = 0; i < 0x15; i++) {
        if (lbl_1_data_560[sel][i] == -1) {
            break;
        }
        lbl_1_bss_33C[lbl_1_data_560[sel][i] - 0xF].active = 0;
    }
    for (i = 0; i < 4; i++) {
        lbl_1_bss_33C[i + 0x29].active = 0;
    }
    for (i = 0; i < 0x28; i++) {
#ifdef TARGET_PC
        /* the original reads 10 entries past the 30-entry table (into adjTbl) */
        if (i < 30) {
            lbl_1_bss_1C[i].x = posTbl[i][0];
            lbl_1_bss_1C[i].z = posTbl[i][1];
        } else {
            lbl_1_bss_1C[i].x = 0.0f;
            lbl_1_bss_1C[i].z = 0.0f;
        }
#else
        lbl_1_bss_1C[i].x = posTbl[i][0];
        lbl_1_bss_1C[i].z = posTbl[i][1];
#endif
        lbl_1_bss_1C[i].y = 100.0f;
        for (j = 0; j < 4; j++) {
            if (adjTbl[i][j] != -1) {
                lbl_1_bss_1C[i].adj[j] = adjTbl[i][j] - 0xF;
            } else {
                lbl_1_bss_1C[i].adj[j] = -1;
            }
        }
    }
    for (i = 0; i < 7; i++) {
        id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_15E0->dataNums[i], HU_MEMNUM_OVL, HEAP_MODEL));
        lbl_1_bss_15E0->models[i].id = id;
        Hu3DModelAttrSet(lbl_1_bss_15E0->models[i].id, 1);
        Hu3DModelCameraSet(lbl_1_bss_15E0->models[i].id, 1);
        Hu3DModelLayerSet(lbl_1_bss_15E0->models[i].id, 0);
        lbl_1_bss_15E0->models[i].x = 0.0f;
        lbl_1_bss_15E0->models[i].y = 0.0f;
        lbl_1_bss_15E0->models[i].z = 0.0f;
        lbl_1_bss_15E0->models[i].rotX = 0.0f;
        lbl_1_bss_15E0->models[i].rotY = 0.0f;
        lbl_1_bss_15E0->models[i].rotZ = 0.0f;
        Hu3DModelShadowMapSet(lbl_1_bss_15E0->models[i].id);
        Hu3DModelAttrReset(lbl_1_bss_15E0->models[i].id, 1);
        Hu3DModelAttrSet(lbl_1_bss_15E0->models[i].id, 0x40000001);
    }
    lbl_1_bss_15E0->models[6].id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_15E0->dataNums[6], HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_15E0->models[6].id, 1);
    Hu3DModelCameraSet(lbl_1_bss_15E0->models[6].id, 1);
    Hu3DModelLayerSet(lbl_1_bss_15E0->models[6].id, 1);
    lbl_1_bss_15E0->models[6].x = 0.0f;
    lbl_1_bss_15E0->models[6].y = 0.0f;
    lbl_1_bss_15E0->models[6].z = 0.0f;
    lbl_1_bss_15E0->models[6].rotX = 0.0f;
    lbl_1_bss_15E0->models[6].rotY = 0.0f;
    lbl_1_bss_15E0->models[6].rotZ = 0.0f;
    Hu3DModelAttrReset(lbl_1_bss_15E0->models[6].id, 1);
    Hu3DModelAttrSet(lbl_1_bss_15E0->models[6].id, 0x40000001);
    for (i = 0, idx = 0xC; i < 3; i++, idx++) {
        lbl_1_bss_15E0->models[i + 0xC].id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_15E0->dataNums[idx], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(lbl_1_bss_15E0->models[i + 0xC].id, 1);
        Hu3DModelCameraSet(lbl_1_bss_15E0->models[i + 0xC].id, 1);
        lbl_1_bss_15E0->models[i + 0xC].x = 0.0f;
        lbl_1_bss_15E0->models[i + 0xC].y = 0.0f;
        lbl_1_bss_15E0->models[i + 0xC].z = 0.0f;
        lbl_1_bss_15E0->models[i + 0xC].rotX = 0.0f;
        lbl_1_bss_15E0->models[i + 0xC].rotY = 0.0f;
        lbl_1_bss_15E0->models[i + 0xC].rotZ = 0.0f;
    }
    for (i = 8, idx = 8; i <= 0xB; i++, idx++) {
        lbl_1_bss_15E0->models[i].id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_15E0->dataNums[idx], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(lbl_1_bss_15E0->models[i].id, 1);
        Hu3DModelCameraSet(lbl_1_bss_15E0->models[i].id, 1);
        Hu3DModelLayerSet(lbl_1_bss_15E0->models[i].id, 0);
        lbl_1_bss_15E0->models[i].x = 0.0f;
        lbl_1_bss_15E0->models[i].y = 0.0f;
        lbl_1_bss_15E0->models[i].z = 0.0f;
        lbl_1_bss_15E0->models[i].rotX = 0.0f;
        lbl_1_bss_15E0->models[i].rotY = 0.0f;
        lbl_1_bss_15E0->models[i].rotZ = 0.0f;
        if (i != 0xA && i != 0xB) {
            Hu3DModelAttrReset(lbl_1_bss_15E0->models[i].id, 1);
        }
        Hu3DModelAttrSet(lbl_1_bss_15E0->models[i].id, 0x40000001);
    }
    Hu3DModelAttrSet(lbl_1_bss_15E0->models[8].id, 1);
    Hu3DModelAttrSet(lbl_1_bss_15E0->models[9].id, 1);
    for (i = 0, idx = 0xF; i < 0x29; i++, idx++) {
        lbl_1_bss_15E0->models[idx].id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_15E0->dataNums[15], HU_MEMNUM_OVL, HEAP_MODEL));
        for (j = 0; j < 3; j++) {
            lbl_1_bss_33C[i].mot[j] = Hu3DJointMotion(lbl_1_bss_15E0->models[idx].id,
                                                      HuDataSelHeapReadNum(jointNums[j], HU_MEMNUM_OVL, HEAP_MODEL));
        }
        Hu3DModelAttrSet(lbl_1_bss_15E0->models[idx].id, 1);
        Hu3DModelCameraSet(lbl_1_bss_15E0->models[idx].id, 1);
        Hu3DModelLayerSet(lbl_1_bss_15E0->models[idx].id, 0);
        Hu3DModelScaleSet(lbl_1_bss_15E0->models[idx].id, 1.154f, 1.0f, 1.0f);
        Hu3DMotionSet(lbl_1_bss_15E0->models[idx].id, lbl_1_bss_33C[i].mot[0]);
        Hu3DMotionSpeedSet(lbl_1_bss_15E0->models[idx].id, 0.0f);
        Hu3DMotionTimeSet(lbl_1_bss_15E0->models[idx].id, 0.0f);
        Hu3DModelAttrReset(lbl_1_bss_15E0->models[idx].id, 1);
        Hu3DModelAttrReset(lbl_1_bss_15E0->models[idx].id, 0x40000001);
        lbl_1_bss_15E0->models[idx].x = 0.0f;
        lbl_1_bss_15E0->models[idx].y = 100.0f;
        lbl_1_bss_15E0->models[idx].z = 0.0f;
        lbl_1_bss_15E0->models[idx].rotX = 0.0f;
        lbl_1_bss_15E0->models[idx].rotY = 0.0f;
        lbl_1_bss_15E0->models[idx].rotZ = 0.0f;
    }
    for (i = 0xF; i <= 0x23; i++) {
        lbl_1_bss_15E0->models[i].rotX = 0.0f;
        lbl_1_bss_15E0->models[i].rotY = -90.0f;
        lbl_1_bss_15E0->models[i].rotZ = 0.0f;
    }
    for (i = 0, idx = 0xF; i < 3; i++, idx++) {
        lbl_1_bss_15E0->models[idx].x = -422.0f;
        lbl_1_bss_15E0->models[idx].y = 100.0f;
        lbl_1_bss_15E0->models[idx].z = -211.0f + 211.0f * i;
    }
    for (i = 0, idx = 0x12; i < 5; i++, idx++) {
        lbl_1_bss_15E0->models[idx].x = -211.0f;
        lbl_1_bss_15E0->models[idx].y = 100.0f;
        lbl_1_bss_15E0->models[idx].z = -422.0f + 211.0f * i;
    }
    for (i = 0, idx = 0x17; i < 5; i++, idx++) {
        lbl_1_bss_15E0->models[idx].x = 0.0f;
        lbl_1_bss_15E0->models[idx].y = 100.0f;
        lbl_1_bss_15E0->models[idx].z = -422.0f + 211.0f * i;
    }
    for (i = 0, idx = 0x1C; i < 5; i++, idx++) {
        lbl_1_bss_15E0->models[idx].x = 211.0f;
        lbl_1_bss_15E0->models[idx].y = 100.0f;
        lbl_1_bss_15E0->models[idx].z = -422.0f + 211.0f * i;
    }
    for (i = 0, idx = 0x21; i < 3; i++, idx++) {
        lbl_1_bss_15E0->models[idx].x = 422.0f;
        lbl_1_bss_15E0->models[idx].y = 100.0f;
        lbl_1_bss_15E0->models[idx].z = -211.0f + 211.0f * i;
    }
    for (i = 0, idx = 0x24; i < 2; i++, idx++) {
        lbl_1_bss_15E0->models[idx].x = -105.5f + 211.0f * i;
        lbl_1_bss_15E0->models[idx].y = 100.0f;
        lbl_1_bss_15E0->models[idx].z = -527.5f;
    }
    for (i = 0, idx = 0x26; i < 4; i++, idx++) {
        lbl_1_bss_15E0->models[idx].x = -316.5f + 211.0f * i;
        lbl_1_bss_15E0->models[idx].y = 100.0f;
        lbl_1_bss_15E0->models[idx].z = -316.5f;
    }
    for (i = 0, idx = 0x2A; i < 4; i++, idx++) {
        lbl_1_bss_15E0->models[idx].x = -316.5f + 211.0f * i;
        lbl_1_bss_15E0->models[idx].y = 100.0f;
        lbl_1_bss_15E0->models[idx].z = -105.5f;
    }
    for (i = 0, idx = 0x2E; i < 4; i++, idx++) {
        lbl_1_bss_15E0->models[idx].x = -316.5f + 211.0f * i;
        lbl_1_bss_15E0->models[idx].y = 100.0f;
        lbl_1_bss_15E0->models[idx].z = 105.5f;
    }
    for (i = 0, idx = 0x32; i < 4; i++, idx++) {
        lbl_1_bss_15E0->models[idx].x = -316.5f + 211.0f * i;
        lbl_1_bss_15E0->models[idx].y = 100.0f;
        lbl_1_bss_15E0->models[idx].z = 316.5f;
    }
    for (i = 0, idx = 0x36; i < 2; i++, idx++) {
        lbl_1_bss_15E0->models[idx].x = -105.5f + 211.0f * i;
        lbl_1_bss_15E0->models[idx].y = 100.0f;
        lbl_1_bss_15E0->models[idx].z = 527.5f;
    }
    lbl_1_bss_0.colMdl[0] = lbl_1_bss_15E0->models[12].id;
    lbl_1_bss_0.colMdl[1] = lbl_1_bss_15E0->models[13].id;
    lbl_1_bss_0.colMdl[2] = lbl_1_bss_15E0->models[14].id;
    MgActorColMapInit(lbl_1_bss_0.colMdl, 3, 10);
    param.height = 150.0f;
    param.radius = 35.0f;
    param.param = 0;
    param.type = 0;
    param.attr = 0x8000;
    param.narrowHook = fn_1_A308;
    param.correctHookParam = 0;
    param.correctHook = fn_1_A69C;
    lbl_1_bss_0.player[0] = MgPlayerCreate(lbl_1_bss_15E0->chars[0].playerNo, &param, 1, 1, -0x3F, NULL);
    lbl_1_bss_0.player[0]->actor->pos.x = -614.0f;
    lbl_1_bss_0.player[0]->actor->pos.y = 140.0f;
    lbl_1_bss_0.player[0]->actor->pos.z = -100.0f;
    lbl_1_bss_0.player[0]->actor->rotY = 90.0f;
    Hu3DModelLayerSet(lbl_1_bss_0.player[0]->actor->mdlId, 1);
    Hu3DModelShadowSet(lbl_1_bss_0.player[0]->actor->mdlId);
    Hu3DModelShadowDispOn(lbl_1_bss_0.player[0]->actor->mdlId);
    lbl_1_bss_15E0->models[0x3C].id = lbl_1_bss_0.player[0]->actor->mdlId;
    param.param = 1;
    lbl_1_bss_0.player[1] = MgPlayerCreate(lbl_1_bss_15E0->chars[1].playerNo, &param, 1, 1, -0x3F, NULL);
    lbl_1_bss_0.player[1]->actor->pos.x = -614.0f;
    lbl_1_bss_0.player[1]->actor->pos.y = 140.0f;
    lbl_1_bss_0.player[1]->actor->pos.z = 100.0f;
    lbl_1_bss_0.player[1]->actor->rotY = 90.0f;
    Hu3DModelLayerSet(lbl_1_bss_0.player[1]->actor->mdlId, 1);
    Hu3DModelShadowSet(lbl_1_bss_0.player[1]->actor->mdlId);
    Hu3DModelShadowDispOn(lbl_1_bss_0.player[1]->actor->mdlId);
    lbl_1_bss_15E0->models[0x3D].id = lbl_1_bss_0.player[1]->actor->mdlId;
    CharModelStepFxSet(lbl_1_bss_15E0->chars[0].charNo, 0);
    CharModelStepFxSet(lbl_1_bss_15E0->chars[1].charNo, 0);
    CharEffectLayerSet(2);
    fn_1_D408(0);
    fn_1_D408(1);
    fn_1_10150();
    fn_1_239C();
    fn_1_6770();
    lbl_1_bss_B94.timer = fn_1_1A8C(0, 30, 0, 292.0f, 64.0f);
}

s32 fn_1_5A6C(M543State *state)
{
    HU3DMODEL *model0;
    HU3DMODEL *model1;
    s32 result = 1;

    fn_1_3CEC(state);
    MgPlayerPadSet(lbl_1_bss_0.player[0], 0, 0, 0, 0);
    MgPlayerPadSet(lbl_1_bss_0.player[1], 0, 0, 0, 0);
    MgActorExec();
    model0 = &Hu3DData[lbl_1_bss_15E0->models[0x3C].id];
    *(HuVecF *)&lbl_1_bss_8DC[0].f38 = model0->pos;
    model1 = &Hu3DData[lbl_1_bss_15E0->models[0x3D].id];
    *(HuVecF *)&lbl_1_bss_8DC[1].f38 = model1->pos;
    return result;
}

s32 fn_1_5B78(void)
{
    s16 result = 0;

    result = fn_1_86E4();
    fn_1_6770();
    return result;
}

s32 fn_1_5BB0(void)
{
    lbl_1_bss_B94.phase = 0;
    lbl_1_bss_B94.f_02 = 0;
    lbl_1_bss_B94.f_04 = 0;
    lbl_1_bss_B94.f_06 = 0;
    fn_1_6770();
    return 0;
}

s32 fn_1_5C20(void)
{
    M543Vec3 pos;
    s16 i;
    s16 done;
    s32 ret;
    s32 end;

    ret = 0;
    end = 0;
    switch (lbl_1_bss_B94.phase) {
    case 0:
        fn_1_1B00(lbl_1_bss_B94.timer);
        lbl_1_bss_B94.phase++;
        /* fallthrough */
    case 1:
        done = fn_1_1B2C(lbl_1_bss_B94.timer);
        for (i = 0; i < 2; i++) {
            fn_1_69BC(i);
        }
        if (lbl_1_bss_B94.winner == 0 && done == 0) {
            for (i = 0; i < 2; i++) {
                if (lbl_1_bss_8DC[i].f50 != 0) {
                    if (lbl_1_bss_8DC[i].f5A == -1) {
                        fn_1_D9E8(i);
                    } else {
                        lbl_1_bss_8DC[i].f5A--;
                    }
                    if (lbl_1_bss_8DC[i].f5A == 0) {
                        lbl_1_bss_8DC[i].f5A = -1;
                        lbl_1_bss_8DC[i].f5C = 0;
                    }
                }
            }
        }
        if (lbl_1_bss_B94.winner != 0 || done != 0) {
            lbl_1_bss_8DC[0].f14 = 1;
            lbl_1_bss_8DC[1].f14 = 1;
            end = 1;
        }
        if (end != 0 || done != 0) {
            lbl_1_bss_B94.phase++;
        }
        break;
    case 2:
        fn_1_391C(1, 0x64);
        fn_1_1B54(lbl_1_bss_B94.timer);
        if (lbl_1_bss_B94.winner != 0) {
            MgSeqWinnerSet(lbl_1_bss_15E0->chars[lbl_1_bss_B94.winner & 1].charNo, -1, -1, -1);
            if (_CheckFlag(0x1000F) == 0) {
                GwPlayer[lbl_1_bss_15E0->chars[lbl_1_bss_B94.winner & 1].playerNo].mgCoinBonus = 10;
            }
        } else {
            MgSeqWinnerSet(-1, -1, -1, -1);
        }
        ret = 1;
        break;
    }
    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_8DC[i].f50 != 0) {
            MgPlayerPadSet(lbl_1_bss_0.player[i], lbl_1_bss_8DC[i].stkX, lbl_1_bss_8DC[i].stkY, lbl_1_bss_8DC[i].btnDown,
                           lbl_1_bss_8DC[i].btn);
        }
        if (lbl_1_bss_8DC[i].f14 != 0) {
            MgPlayerPadSet(lbl_1_bss_0.player[i], 0, 0, 0, 0);
        }
    }
    MgActorPosGet(lbl_1_bss_0.player[0]->actor, (HuVecF *)&pos);
    lbl_1_bss_8DC[0].f44 = pos.x;
    lbl_1_bss_8DC[0].f48 = pos.y;
    lbl_1_bss_8DC[0].f4C = pos.z;
    MgActorPosGet(lbl_1_bss_0.player[1]->actor, (HuVecF *)&pos);
    lbl_1_bss_8DC[1].f44 = pos.x;
    lbl_1_bss_8DC[1].f48 = pos.y;
    lbl_1_bss_8DC[1].f4C = pos.z;
    MgActorExec();
    MgActorPosGet(lbl_1_bss_0.player[0]->actor, (HuVecF *)&pos);
    lbl_1_bss_8DC[0].f38 = pos.x;
    lbl_1_bss_8DC[0].f3C = pos.y;
    lbl_1_bss_8DC[0].f40 = pos.z;
    MgActorPosGet(lbl_1_bss_0.player[1]->actor, (HuVecF *)&pos);
    lbl_1_bss_8DC[1].f38 = pos.x;
    lbl_1_bss_8DC[1].f3C = pos.y;
    lbl_1_bss_8DC[1].f40 = pos.z;
    fn_1_A6B0(lbl_1_bss_8DC);
    pos.x = lbl_1_bss_8DC[0].f38;
    pos.y = lbl_1_bss_8DC[0].f3C;
    pos.z = lbl_1_bss_8DC[0].f40;
    MgActorPosUpdate(lbl_1_bss_0.player[0]->actor, (HuVecF *)&pos);
    pos.x = lbl_1_bss_8DC[1].f38;
    pos.y = lbl_1_bss_8DC[1].f3C;
    pos.z = lbl_1_bss_8DC[1].f40;
    MgActorPosUpdate(lbl_1_bss_0.player[1]->actor, (HuVecF *)&pos);
    if (lbl_1_bss_B94.f_22 == 0) {
        lbl_1_bss_8DC[0].f5C = 0;
        lbl_1_bss_8DC[1].f5C = 0;
    }
    lbl_1_bss_B94.f_22 = 0;
    fn_1_71AC();
    fn_1_6770();
    return ret;
}

s32 fn_1_623C(void)
{
    lbl_1_bss_B94.phase = 0;
    lbl_1_bss_B94.f_02 = 0;
    lbl_1_bss_B94.f_04 = 0;
    lbl_1_bss_B94.f_06 = 0;
    lbl_1_bss_8DC[0].sub = 0;
    lbl_1_bss_8DC[1].sub = 0;
    MgPlayerPadSet(lbl_1_bss_0.player[0], 0, 0, 0, 0);
    MgPlayerPadSet(lbl_1_bss_0.player[1], 0, 0, 0, 0);
    MgActorExec();
    fn_1_71AC();
    fn_1_6770();
    return 0;
}

s32 fn_1_6314(void)
{
    s16 result = 0;

    result = fn_1_91F8();
    fn_1_6770();
    return result;
}

s32 fn_1_634C(void)
{
    M543StageSlot *slot;
    HU3DMODEL *model = &Hu3DData[lbl_1_bss_15E0->models[(lbl_1_bss_B94.winner & 1) + 0x3C].id];
    s32 result = 0;

    if (lbl_1_bss_15E0->field_178 == 0) {
        if (lbl_1_bss_B94.winner != 0) {
            slot = &lbl_1_bss_15E0->slots[lbl_1_bss_B94.winner & 1];
            slot->motion = 5;
            slot->speed = 1.0f;
            slot->time = 0.0f;
            slot->attrSet = 0;
            slot->attrReset = 0x40000001;
        }
        lbl_1_bss_15E0->field_178 = 1;
        lbl_1_bss_B94.f_24 = model->rot.y;
    }
    fn_1_6770();
    CharModelVoicePanSet(lbl_1_bss_15E0->chars[lbl_1_bss_B94.winner & 1].charNo, 0x40);
    return result;
}

s32 fn_1_64AC(void)
{
    s16 result = 1;

    fn_1_6770();
    return result;
}

s32 fn_1_64DC(void)
{
    s16 result = 1;

    fn_1_3CCC();
    MgActorKill(lbl_1_bss_0.player[0]->actor);
    MgActorKill(lbl_1_bss_0.player[1]->actor);
    return result;
}

void fn_1_6534(void)
{
    s16 i;
    HU3DMODEL *model0;
    HU3DMODEL *model1;

    model0 = &Hu3DData[lbl_1_bss_15E0->models[0x3C].id];
    model1 = &Hu3DData[lbl_1_bss_15E0->models[0x3D].id];
    for (i = 0; i <= 0x37; i++) {
        Hu3DModelPosSet(lbl_1_bss_15E0->models[i].id, lbl_1_bss_15E0->models[i].x, lbl_1_bss_15E0->models[i].y,
                        lbl_1_bss_15E0->models[i].z);
        Hu3DModelRotSet(lbl_1_bss_15E0->models[i].id, lbl_1_bss_15E0->models[i].rotX, lbl_1_bss_15E0->models[i].rotY,
                        lbl_1_bss_15E0->models[i].rotZ);
    }
    CharModelVoicePanSet(lbl_1_bss_15E0->chars[0].charNo, 64.0f + 0.042666666f * model0->pos.x);
    CharModelVoicePanSet(lbl_1_bss_15E0->chars[1].charNo, 64.0f + 0.042666666f * model1->pos.x);
}

void fn_1_6770(void)
{
    s16 i;
    HU3DMODEL *model0;
    HU3DMODEL *model1;

    model0 = &Hu3DData[lbl_1_bss_15E0->models[0x3C].id];
    model1 = &Hu3DData[lbl_1_bss_15E0->models[0x3D].id];
    for (i = 0; i <= 0x37; i++) {
        Hu3DModelPosSet(lbl_1_bss_15E0->models[i].id, lbl_1_bss_15E0->models[i].x, lbl_1_bss_15E0->models[i].y,
                        lbl_1_bss_15E0->models[i].z);
        Hu3DModelRotSet(lbl_1_bss_15E0->models[i].id, lbl_1_bss_15E0->models[i].rotX, lbl_1_bss_15E0->models[i].rotY,
                        lbl_1_bss_15E0->models[i].rotZ);
    }
    CharModelVoicePanSet(lbl_1_bss_15E0->chars[0].charNo, 64.0f + 0.042666666f * model0->pos.x);
    CharModelVoicePanSet(lbl_1_bss_15E0->chars[1].charNo, 64.0f + 0.042666666f * model1->pos.x);
    fn_1_91C(0);
    fn_1_10C4();
    fn_1_14C4();
}

s32 fn_1_69BC(s16 arg0)
{
    M543StageSlot *slot;
    s32 ret;

    slot = &lbl_1_bss_15E0->slots[arg0];
    ret = 0;
    switch (lbl_1_bss_8DC[arg0].mode) {
    case 0:
        if (lbl_1_bss_8DC[arg0].f18 != 0) {
            omVibrate(lbl_1_bss_15E0->chars[arg0].playerNo, 0x14, 0xC, 0);
            fn_1_3A5C(1, 0x7E, 64.0f + 0.042666666f * lbl_1_bss_15E0->models[arg0 + 0x3C].x);
            lbl_1_bss_8DC[arg0].f14 = 1;
            lbl_1_bss_8DC[arg0].mode++;
        }
        break;
    case 1:
        if (lbl_1_bss_0.player[arg0]->mode == 0) {
            lbl_1_bss_8DC[arg0].mode++;
        }
        break;
    case 2:
        MgPlayerPauseOn(lbl_1_bss_0.player[arg0]);
        lbl_1_bss_8DC[arg0].mode++;
        break;
    case 3:
        if (fn_1_6CF0(arg0) != 0) {
            lbl_1_bss_8DC[arg0].f18 = 0;
            lbl_1_bss_8DC[arg0].mode++;
        }
        break;
    case 4:
        MgPlayerPauseOff(lbl_1_bss_0.player[arg0], (HuVecF *)&lbl_1_bss_8DC[arg0].f38);
        lbl_1_bss_8DC[arg0].f16 = 0;
        lbl_1_bss_8DC[arg0].mode++;
        break;
    case 5:
        lbl_1_bss_8DC[arg0].f14 = 0;
        lbl_1_bss_8DC[arg0].mode = 0;
        break;
    case 6:
        ret = 1;
        break;
    }
    return ret;
}

s16 fn_1_6CF0(s16 arg0)
{
    M543StageSlot *slot;
    HU3DMODEL *model;
    M543Vec3 pos;
    s16 ret;
    s16 moved;

    model = &Hu3DData[lbl_1_bss_15E0->models[arg0 + 0x3C].id];
    slot = &lbl_1_bss_15E0->slots[arg0];
    ret = 0;
    switch (lbl_1_bss_8DC[arg0].sub) {
    case 0:
        slot->motion = 2;
        slot->speed = 1.0f;
        slot->time = 0.0f;
        slot->attrSet = 0;
        slot->attrReset = 0x40000001;
        lbl_1_bss_8DC[arg0].f1A = 0;
        lbl_1_bss_8DC[arg0].sub++;
        break;
    case 1:
        moved = fn_1_1970(&lbl_1_bss_8DC[arg0].f38, 42.0f, lbl_1_bss_8DC[arg0].f1C, lbl_1_bss_8DC[arg0].f28);
        moved = moved + fn_1_1970(&lbl_1_bss_8DC[arg0].f40, 42.0f, lbl_1_bss_8DC[arg0].f24, lbl_1_bss_8DC[arg0].f30);
        lbl_1_bss_8DC[arg0].f1A++;
        pos.x = lbl_1_bss_8DC[arg0].f38;
        pos.y = lbl_1_bss_8DC[arg0].f3C;
        pos.z = lbl_1_bss_8DC[arg0].f40;
        MgActorPosUpdate(lbl_1_bss_0.player[arg0]->actor, (HuVecF *)&pos);
        if (slot->endFlag != 0 || moved == 0 || lbl_1_bss_8DC[arg0].f1A >= 0x2A) {
            slot->shiftState = 1;
            slot->shiftMotion = 3;
            slot->shiftStart = 0.0f;
            slot->shiftEnd = 8.0f;
            slot->shiftAttr = 0x40000001;
            slot->attrReset = 0;
            lbl_1_bss_8DC[arg0].f1A = 0;
            lbl_1_bss_8DC[arg0].f34 = 1;
            lbl_1_bss_8DC[arg0].sub++;
        }
        break;
    case 2:
        if (lbl_1_bss_8DC[arg0].f1A++ >= 0x19) {
            slot->shiftState = 1;
            slot->shiftMotion = 4;
            slot->shiftStart = 0.0f;
            slot->shiftEnd = 8.0f;
            slot->shiftAttr = 0;
            slot->attrReset = 0x40000001;
            lbl_1_bss_8DC[arg0].f34 = 0;
            lbl_1_bss_8DC[arg0].sub++;
        }
        break;
    case 3:
        if (slot->endFlag != 0) {
            slot->motion = 0;
            slot->speed = 1.0f;
            slot->time = 0.0f;
            slot->attrSet = 0x40000001;
            slot->attrReset = 0;
            lbl_1_bss_8DC[arg0].sub = 0;
            ret = 1;
        }
        break;
    }
    model->pos.x = lbl_1_bss_8DC[arg0].f38;
    model->pos.z = lbl_1_bss_8DC[arg0].f40;
    return ret;
}

s32 fn_1_71AC(void)
{
    s16 i;

    for (i = 0; i < 0x29; i++) {
        if (lbl_1_bss_33C[i].active != 0) {
            switch (lbl_1_bss_33C[i].mode) {
            case 0:
                lbl_1_bss_33C[i].prevSel = 1;
                lbl_1_bss_33C[i].sel = 1;
                lbl_1_bss_33C[i].step = 2;
                lbl_1_bss_33C[i].f_0E = 0;
                lbl_1_bss_33C[i].mode++;
                /* fallthrough */
            case 1:
                if (lbl_1_bss_33C[i].sel != lbl_1_bss_33C[i].prevSel) {
                    lbl_1_bss_33C[i].step = 0;
                }
                lbl_1_bss_33C[i].prevSel = lbl_1_bss_33C[i].sel;
                lbl_1_data_7F4[lbl_1_bss_33C[i].sel](i);
                break;
            }
        }
    }
    return 0;
}

s32 fn_1_7394(s16 arg0)
{
    M543Vec3 pos;
    s16 idx;
    s32 ret;

    ret = 0;
    idx = arg0 + 0xF;
    switch (lbl_1_bss_33C[arg0].step) {
    case 0:
        Hu3DMotionSet(lbl_1_bss_15E0->models[idx].id, lbl_1_bss_33C[arg0].mot[0]);
        Hu3DMotionSpeedSet(lbl_1_bss_15E0->models[idx].id, 1.0f);
        lbl_1_bss_33C[arg0].time = 0.0f;
        Hu3DMotionTimeSet(lbl_1_bss_15E0->models[idx].id, lbl_1_bss_33C[arg0].time);
        Hu3DModelAttrReset(lbl_1_bss_15E0->models[idx].id, 0x40000001);
        pos.x = lbl_1_bss_15E0->models[idx].x;
        pos.y = 100.0f;
        pos.z = lbl_1_bss_15E0->models[idx].z;
        if (arg0 < 0x15) {
            fn_1_2898(&pos, 1);
        } else {
            fn_1_2898(&pos, 0);
        }
        lbl_1_bss_33C[arg0].step++;
        /* fallthrough */
    case 1:
        lbl_1_bss_33C[arg0].time = Hu3DMotionTimeGet(lbl_1_bss_15E0->models[idx].id);
        if (Hu3DMotionEndCheck(lbl_1_bss_15E0->models[idx].id) != 0) {
            Hu3DMotionSet(lbl_1_bss_15E0->models[idx].id, lbl_1_bss_33C[arg0].mot[2]);
            Hu3DModelAttrSet(lbl_1_bss_15E0->models[idx].id, 0x40000001);
            Hu3DMotionSpeedSet(lbl_1_bss_15E0->models[idx].id, 1.0f);
            Hu3DMotionTimeSet(lbl_1_bss_15E0->models[idx].id, 0.0f);
            lbl_1_bss_33C[arg0].time = 0.0f;
            lbl_1_bss_33C[arg0].step++;
        }
        break;
    case 2:
        ret = 1;
        break;
    }
    return ret;
}

s32 fn_1_7704(s16 arg0)
{
    M543Vec3 pos;
    s16 idx;
    s32 ret;

    ret = 0;
    idx = arg0 + 0xF;
    switch (lbl_1_bss_33C[arg0].step) {
    case 0:
        Hu3DMotionSet(lbl_1_bss_15E0->models[idx].id, lbl_1_bss_33C[arg0].mot[1]);
        Hu3DMotionSpeedSet(lbl_1_bss_15E0->models[idx].id, 1.0f);
        lbl_1_bss_33C[arg0].time = 0.0f;
        Hu3DMotionTimeSet(lbl_1_bss_15E0->models[idx].id, lbl_1_bss_33C[arg0].time);
        Hu3DModelAttrReset(lbl_1_bss_15E0->models[idx].id, 0x40000001);
        pos.x = lbl_1_bss_15E0->models[idx].x;
        pos.y = 100.0f;
        pos.z = lbl_1_bss_15E0->models[idx].z;
        if (arg0 < 0x15) {
            fn_1_2898(&pos, 1);
        } else {
            fn_1_2898(&pos, 0);
        }
        lbl_1_bss_33C[arg0].step++;
        /* fallthrough */
    case 1:
        lbl_1_bss_33C[arg0].time = Hu3DMotionTimeGet(lbl_1_bss_15E0->models[idx].id);
        if (Hu3DMotionEndCheck(lbl_1_bss_15E0->models[idx].id) != 0) {
            Hu3DMotionSet(lbl_1_bss_15E0->models[idx].id, lbl_1_bss_33C[arg0].mot[0]);
            Hu3DMotionSpeedSet(lbl_1_bss_15E0->models[idx].id, 0.0f);
            Hu3DMotionTimeSet(lbl_1_bss_15E0->models[idx].id, 0.0f);
            Hu3DModelAttrReset(lbl_1_bss_15E0->models[idx].id, 0x40000001);
            lbl_1_bss_33C[arg0].time = 0.0f;
            lbl_1_bss_33C[arg0].step++;
        }
        break;
    case 2:
        ret = 1;
        break;
    }
    return ret;
}

void fn_1_7A74(void)
{
    f32 camStep[4][2] = { { 8.0f, 0.0f }, { -8.0f, 0.0f }, { 0.0f, 8.0f }, { 0.0f, -8.0f } };
    s16 sp8;
    s16 spA;

    spA = 0;
    sp8 = 0;
    switch (lbl_1_bss_B94.f_0C) {
    case 0:
        lbl_1_bss_B94.f_14 = 0.0f;
        lbl_1_bss_B94.f_1C = 0.0f;
        lbl_1_bss_B94.f_10 = 0;
        lbl_1_bss_B94.f_0C++;
        return;
    case 1:
        lbl_1_bss_18->center.x += camStep[lbl_1_bss_B94.f_0E][0];
        lbl_1_bss_18->center.z += camStep[lbl_1_bss_B94.f_0E][1];
        lbl_1_bss_B94.f_10++;
        if (lbl_1_bss_B94.f_10 >= 2) {
            lbl_1_bss_B94.f_0E++;
            if (lbl_1_bss_B94.f_0E >= 4) {
                lbl_1_bss_B94.f_0E = 0;
            }
            lbl_1_bss_B94.f_0C = 0;
        }
        return;
    }
}

s16 fn_1_7C18(void)
{
    s16 tiles[9][7] = {
        { 18, 19, 20, -1, 0, 0, 0 },
        { 26, 30, 34, 38, -1, 0, 0 },
        { 13, 14, 15, 16, 17, -1, 0 },
        { 22, 25, 29, 33, 37, 40, -1 },
        { 8, 9, 10, 11, 12, -1, 0 },
        { 21, 24, 28, 32, 36, 39, -1 },
        { 3, 4, 5, 6, 7, -1, 0 },
        { 23, 27, 31, 35, -1, 0, 0 },
        { 0, 1, 2, -1, 0, 0, 0 },
    };
    s16 vib[9] = { 15, 15, 15, 15, 15, 15, 15, 18, 25 };
    M543Vec3 posA;
    M543Vec3 posB;
    s16 i;
    s16 j;
    s16 idx;
    s16 done;
    s16 ret;
    s16 t;

    ret = 0;
    t = lbl_1_bss_B94.f_0A++;
    if (t > 0x19) {
        lbl_1_bss_B94.f_0A = 0;
        if (lbl_1_bss_B94.f_08 < 9) {
            for (i = 0; tiles[lbl_1_bss_B94.f_08][i] != -1; i++) {
                lbl_1_bss_33C[tiles[lbl_1_bss_B94.f_08][i]].flag2 = 1;
            }
            fn_1_39E4(3);
            if (lbl_1_bss_B94.f_08 < 6) {
                if (lbl_1_bss_B94.f_08 < 3) {
                    if (lbl_1_bss_B94.f_08 >= 0) {
                        omVibrate(lbl_1_bss_15E0->chars[0].playerNo, vib[lbl_1_bss_B94.f_08], 6, 6);
                        omVibrate(lbl_1_bss_15E0->chars[1].playerNo, vib[lbl_1_bss_B94.f_08], 6, 6);
                    }
                } else {
                    omVibrate(lbl_1_bss_15E0->chars[0].playerNo, vib[lbl_1_bss_B94.f_08], 4, 2);
                    omVibrate(lbl_1_bss_15E0->chars[1].playerNo, vib[lbl_1_bss_B94.f_08], 4, 2);
                }
            } else if (lbl_1_bss_B94.f_08 < 9) {
                omVibrate(lbl_1_bss_15E0->chars[0].playerNo, vib[lbl_1_bss_B94.f_08], 0xC, 0);
                omVibrate(lbl_1_bss_15E0->chars[1].playerNo, vib[lbl_1_bss_B94.f_08], 0xC, 0);
            }
        }
        lbl_1_bss_B94.f_08++;
        if (lbl_1_bss_B94.f_08 >= 9) {
            ret = 1;
        }
    }
    for (j = 0; j < 0x29; j++) {
        if (lbl_1_bss_33C[j].flag2 != 0) {
            switch (lbl_1_bss_33C[j].phase) {
            case 0:
                lbl_1_bss_33C[j].phase++;
                /* fallthrough */
            case 1:
                done = 0;
                idx = j + 0xF;
                switch (lbl_1_bss_33C[j].step) {
                case 0:
                    Hu3DMotionSet(lbl_1_bss_15E0->models[idx].id, lbl_1_bss_33C[j].mot[0]);
                    Hu3DMotionSpeedSet(lbl_1_bss_15E0->models[idx].id, 1.0f);
                    lbl_1_bss_33C[j].time = 0.0f;
                    Hu3DMotionTimeSet(lbl_1_bss_15E0->models[idx].id, lbl_1_bss_33C[j].time);
                    Hu3DModelAttrReset(lbl_1_bss_15E0->models[idx].id, 0x40000001);
                    posA.x = lbl_1_bss_15E0->models[idx].x;
                    posA.y = 100.0f;
                    posA.z = lbl_1_bss_15E0->models[idx].z;
                    if (j < 0x15) {
                        fn_1_2898(&posA, 1);
                    } else {
                        fn_1_2898(&posA, 0);
                    }
                    lbl_1_bss_33C[j].step++;
                    /* fallthrough */
                case 1:
                    lbl_1_bss_33C[j].time = Hu3DMotionTimeGet(lbl_1_bss_15E0->models[idx].id);
                    if (Hu3DMotionEndCheck(lbl_1_bss_15E0->models[idx].id) != 0) {
                        Hu3DMotionSet(lbl_1_bss_15E0->models[idx].id, lbl_1_bss_33C[j].mot[2]);
                        Hu3DModelAttrSet(lbl_1_bss_15E0->models[idx].id, 0x40000001);
                        Hu3DMotionSpeedSet(lbl_1_bss_15E0->models[idx].id, 1.0f);
                        Hu3DMotionTimeSet(lbl_1_bss_15E0->models[idx].id, 0.0f);
                        lbl_1_bss_33C[j].time = 0.0f;
                        lbl_1_bss_33C[j].step++;
                    }
                    break;
                case 2:
                    done = 1;
                    break;
                }
                if (done != 0) {
                    lbl_1_bss_33C[j].step = 0;
                    lbl_1_bss_33C[j].phase++;
                }
                break;
            case 2:
                done = 0;
                idx = j + 0xF;
                switch (lbl_1_bss_33C[j].step) {
                case 0:
                    Hu3DMotionSet(lbl_1_bss_15E0->models[idx].id, lbl_1_bss_33C[j].mot[1]);
                    Hu3DMotionSpeedSet(lbl_1_bss_15E0->models[idx].id, 1.0f);
                    lbl_1_bss_33C[j].time = 0.0f;
                    Hu3DMotionTimeSet(lbl_1_bss_15E0->models[idx].id, lbl_1_bss_33C[j].time);
                    Hu3DModelAttrReset(lbl_1_bss_15E0->models[idx].id, 0x40000001);
                    posB.x = lbl_1_bss_15E0->models[idx].x;
                    posB.y = 100.0f;
                    posB.z = lbl_1_bss_15E0->models[idx].z;
                    if (j < 0x15) {
                        fn_1_2898(&posB, 1);
                    } else {
                        fn_1_2898(&posB, 0);
                    }
                    lbl_1_bss_33C[j].step++;
                    /* fallthrough */
                case 1:
                    lbl_1_bss_33C[j].time = Hu3DMotionTimeGet(lbl_1_bss_15E0->models[idx].id);
                    if (Hu3DMotionEndCheck(lbl_1_bss_15E0->models[idx].id) != 0) {
                        Hu3DMotionSet(lbl_1_bss_15E0->models[idx].id, lbl_1_bss_33C[j].mot[0]);
                        Hu3DMotionSpeedSet(lbl_1_bss_15E0->models[idx].id, 0.0f);
                        Hu3DMotionTimeSet(lbl_1_bss_15E0->models[idx].id, 0.0f);
                        Hu3DModelAttrReset(lbl_1_bss_15E0->models[idx].id, 0x40000001);
                        lbl_1_bss_33C[j].time = 0.0f;
                        lbl_1_bss_33C[j].step++;
                    }
                    break;
                case 2:
                    done = 1;
                    break;
                }
                if (done != 0) {
                    lbl_1_bss_33C[j].flag2 = 0;
                    lbl_1_bss_33C[j].step = 0;
                    lbl_1_bss_33C[j].phase = 0;
                }
                break;
            }
        }
    }
    return ret;
}

s16 fn_1_86E4(void)
{
    s16 ret;
    s16 i;
    s16 idx;
    s16 moved;
    s32 sum;

    ret = 0;
    switch (lbl_1_bss_B94.phase) {
    case 0:
        fn_1_38EC();
        lbl_1_bss_B94.phase++;
        /* fallthrough */
    case 1:
        if (lbl_1_bss_B94.f_06++ > 0x3C) {
            fn_1_39E4(2);
            lbl_1_bss_B94.f_06 = 0;
            lbl_1_bss_B94.phase++;
        }
        break;
    case 2: {
        s16 sp14 = 0;
        s16 sp16 = 0;
        f32 camStep[4][2] = { { 8.0f, 0.0f }, { -8.0f, 0.0f }, { 0.0f, 8.0f }, { 0.0f, -8.0f } };

        switch (lbl_1_bss_B94.f_0C) {
        case 0:
            lbl_1_bss_B94.f_14 = 0.0f;
            lbl_1_bss_B94.f_1C = 0.0f;
            lbl_1_bss_B94.f_10 = 0;
            lbl_1_bss_B94.f_0C++;
            break;
        case 1:
            lbl_1_bss_18->center.x += camStep[lbl_1_bss_B94.f_0E][0];
            lbl_1_bss_18->center.z += camStep[lbl_1_bss_B94.f_0E][1];
            lbl_1_bss_B94.f_10++;
            if (lbl_1_bss_B94.f_10 >= 2) {
                lbl_1_bss_B94.f_0E++;
                if (lbl_1_bss_B94.f_0E >= 4) {
                    lbl_1_bss_B94.f_0E = 0;
                }
                lbl_1_bss_B94.f_0C = 0;
            }
            break;
        }
        if (lbl_1_bss_B94.f_06++ > 0x1E) {
            lbl_1_bss_B94.f_06 = 0;
            lbl_1_bss_B94.phase++;
        }
        break;
    }
    case 3: {
        s16 sp10 = 0;
        s16 sp12 = 0;
        f32 camStep[4][2] = { { 8.0f, 0.0f }, { -8.0f, 0.0f }, { 0.0f, 8.0f }, { 0.0f, -8.0f } };

        switch (lbl_1_bss_B94.f_0C) {
        case 0:
            lbl_1_bss_B94.f_14 = 0.0f;
            lbl_1_bss_B94.f_1C = 0.0f;
            lbl_1_bss_B94.f_10 = 0;
            lbl_1_bss_B94.f_0C++;
            break;
        case 1:
            lbl_1_bss_18->center.x += camStep[lbl_1_bss_B94.f_0E][0];
            lbl_1_bss_18->center.z += camStep[lbl_1_bss_B94.f_0E][1];
            lbl_1_bss_B94.f_10++;
            if (lbl_1_bss_B94.f_10 >= 2) {
                lbl_1_bss_B94.f_0E++;
                if (lbl_1_bss_B94.f_0E >= 4) {
                    lbl_1_bss_B94.f_0E = 0;
                }
                lbl_1_bss_B94.f_0C = 0;
            }
            break;
        }
        fn_1_7C18();
        if (lbl_1_bss_B94.f_06++ > 0x23) {
            lbl_1_bss_B94.f_06 = 0;
            lbl_1_bss_B94.phase++;
        }
        break;
    }
    case 4: {
        s16 spC = 0;
        s16 spE = 0;
        f32 camStep[4][2] = { { 8.0f, 0.0f }, { -8.0f, 0.0f }, { 0.0f, 8.0f }, { 0.0f, -8.0f } };

        switch (lbl_1_bss_B94.f_0C) {
        case 0:
            lbl_1_bss_B94.f_14 = 0.0f;
            lbl_1_bss_B94.f_1C = 0.0f;
            lbl_1_bss_B94.f_10 = 0;
            lbl_1_bss_B94.f_0C++;
            break;
        case 1:
            lbl_1_bss_18->center.x += camStep[lbl_1_bss_B94.f_0E][0];
            lbl_1_bss_18->center.z += camStep[lbl_1_bss_B94.f_0E][1];
            lbl_1_bss_B94.f_10++;
            if (lbl_1_bss_B94.f_10 >= 2) {
                lbl_1_bss_B94.f_0E++;
                if (lbl_1_bss_B94.f_0E >= 4) {
                    lbl_1_bss_B94.f_0E = 0;
                }
                lbl_1_bss_B94.f_0C = 0;
            }
            break;
        }
        fn_1_1850(&lbl_1_bss_18->center.x, 250.0f, 500.0f, -654.0f);
        if (fn_1_7C18() != 0) {
            lbl_1_bss_B94.phase++;
        }
        break;
    }
    case 5: {
        s16 sp8 = 0;
        s16 spA = 0;
        f32 camStep[4][2] = { { 8.0f, 0.0f }, { -8.0f, 0.0f }, { 0.0f, 8.0f }, { 0.0f, -8.0f } };

        switch (lbl_1_bss_B94.f_0C) {
        case 0:
            lbl_1_bss_B94.f_14 = 0.0f;
            lbl_1_bss_B94.f_1C = 0.0f;
            lbl_1_bss_B94.f_10 = 0;
            lbl_1_bss_B94.f_0C++;
            break;
        case 1:
            lbl_1_bss_18->center.x += camStep[lbl_1_bss_B94.f_0E][0];
            lbl_1_bss_18->center.z += camStep[lbl_1_bss_B94.f_0E][1];
            lbl_1_bss_B94.f_10++;
            if (lbl_1_bss_B94.f_10 >= 2) {
                lbl_1_bss_B94.f_0E++;
                if (lbl_1_bss_B94.f_0E >= 4) {
                    lbl_1_bss_B94.f_0E = 0;
                }
                lbl_1_bss_B94.f_0C = 0;
            }
            break;
        }
        moved = fn_1_1850(&lbl_1_bss_18->center.x, 250.0f, 500.0f, -654.0f);
        fn_1_7C18();
        if (lbl_1_bss_B94.f_06++ > 0x23 && moved == 0) {
            lbl_1_bss_B94.f_06 = 0;
            lbl_1_bss_B94.phase++;
        }
        break;
    }
    case 6:
        if (lbl_1_bss_B94.f_06++ > 0x3C) {
            lbl_1_bss_B94.f_14 = lbl_1_bss_18->center.x;
            lbl_1_bss_B94.f_1C = lbl_1_bss_18->center.z;
            lbl_1_bss_B94.f_06 = 0;
            lbl_1_bss_B94.phase++;
        }
        break;
    case 7:
        moved = fn_1_1850(&lbl_1_bss_18->center.x, 30.0f, lbl_1_bss_B94.f_14, lbl_1_data_1D8[1].center.x);
        sum = moved + fn_1_1850(&lbl_1_bss_18->center.z, 30.0f, lbl_1_bss_B94.f_1C, lbl_1_data_1D8[1].center.z);
        sum = sum + fn_1_1850(&lbl_1_bss_18->rotX, 30.0f, lbl_1_data_1D8[0].rotX, lbl_1_data_1D8[1].rotX);
        if ((s16)(sum + fn_1_1850(&lbl_1_bss_18->zoom, 30.0f, lbl_1_data_1D8[0].zoom, lbl_1_data_1D8[1].zoom)) == 0) {
            for (i = 0, idx = 0xF; i < 0x2D; i++, idx++) {
                if (lbl_1_bss_33C[i].active == 0) {
                    lbl_1_bss_15E0->models[idx].y = 0.0f;
                }
            }
            lbl_1_bss_B94.phase++;
        }
        break;
    case 8:
        ret = 1;
        break;
    }
    return ret;
}

s16 fn_1_91F8(void)
{
    s16 grp[5][12] = {
        { 18, 19, 20, 26, 30, 34, 38, -1, 0, 0, 0, 0 },
        { 13, 14, 15, 16, 17, 22, 25, 29, 33, 37, 40, -1 },
        { 8, 9, 10, 11, 12, 21, 24, 28, 32, 36, 39, -1 },
        { 3, 4, 5, 6, 7, 23, 27, 31, 35, -1, 0, 0 },
        { 0, 1, 2, -1, 0, 0, 0, 0, 0, 0, 0, 0 },
    };
    s16 panTbl[5] = { 316, 105, -105, -316, -480 };
    M543Vec3 pos;
    HU3DMODEL *model;
    M543StageSlot *slot;
    M543StageSlot *slot2;
    f32 dx;
    f32 dz;
    s16 ret;
    s16 i;
    s16 t;
    s16 idx;
    s16 w;
    s16 moved;
    s16 cnt;
    s16 done;
    s32 r;
    s32 sum;

    model = &Hu3DData[lbl_1_bss_15E0->models[(lbl_1_bss_B94.winner & 1) + 0x3C].id];
    slot = &lbl_1_bss_15E0->slots[lbl_1_bss_B94.winner & 1];
    ret = 0;
    switch (lbl_1_bss_B94.phase) {
    case 0:
        if (lbl_1_bss_B94.f_06++ > 0x1E) {
            for (i = 0; i < 0x29; i++) {
                lbl_1_bss_33C[i].step = 0;
            }
            omVibrate(lbl_1_bss_15E0->chars[0].playerNo, 0x1E, 0xC, 0);
            omVibrate(lbl_1_bss_15E0->chars[1].playerNo, 0x1E, 0xC, 0);
            fn_1_3A5C(3, 0x7E, 64.0f + 0.042666666f * panTbl[0]);
            lbl_1_bss_B94.f_06 = 0;
            lbl_1_bss_B94.f_08 = 0;
            lbl_1_bss_B94.phase++;
        }
        break;
    case 1:
        i = 0;
        cnt = 0;
        done = 0;
        do {
            if (lbl_1_bss_33C[grp[lbl_1_bss_B94.f_08][i]].active != 0 && lbl_1_bss_33C[grp[lbl_1_bss_B94.f_08][i]].sel == 1) {
                t = grp[lbl_1_bss_B94.f_08][i];
                r = 0;
                idx = t + 0xF;
                switch (lbl_1_bss_33C[t].step) {
                case 0:
                    Hu3DMotionSet(lbl_1_bss_15E0->models[idx].id, lbl_1_bss_33C[t].mot[0]);
                    Hu3DMotionSpeedSet(lbl_1_bss_15E0->models[idx].id, 1.0f);
                    lbl_1_bss_33C[t].time = 0.0f;
                    Hu3DMotionTimeSet(lbl_1_bss_15E0->models[idx].id, lbl_1_bss_33C[t].time);
                    Hu3DModelAttrReset(lbl_1_bss_15E0->models[idx].id, 0x40000001);
                    pos.x = lbl_1_bss_15E0->models[idx].x;
                    pos.y = 100.0f;
                    pos.z = lbl_1_bss_15E0->models[idx].z;
                    if (t < 0x15) {
                        fn_1_2898(&pos, 1);
                    } else {
                        fn_1_2898(&pos, 0);
                    }
                    lbl_1_bss_33C[t].step++;
                    /* fallthrough */
                case 1:
                    lbl_1_bss_33C[t].time = Hu3DMotionTimeGet(lbl_1_bss_15E0->models[idx].id);
                    if (Hu3DMotionEndCheck(lbl_1_bss_15E0->models[idx].id) != 0) {
                        Hu3DMotionSet(lbl_1_bss_15E0->models[idx].id, lbl_1_bss_33C[t].mot[2]);
                        Hu3DModelAttrSet(lbl_1_bss_15E0->models[idx].id, 0x40000001);
                        Hu3DMotionSpeedSet(lbl_1_bss_15E0->models[idx].id, 1.0f);
                        Hu3DMotionTimeSet(lbl_1_bss_15E0->models[idx].id, 0.0f);
                        lbl_1_bss_33C[t].time = 0.0f;
                        lbl_1_bss_33C[t].step++;
                    }
                    break;
                case 2:
                    r = 1;
                    break;
                }
                done += r;
                cnt++;
            }
            i++;
        } while (grp[lbl_1_bss_B94.f_08][i] != -1);
        if (done == cnt) {
            lbl_1_bss_B94.f_08++;
            if (lbl_1_bss_B94.f_08 >= 5) {
                lbl_1_bss_B94.f_0A = 0;
                lbl_1_bss_B94.phase++;
            } else {
                i = 0;
                cnt = 0;
                do {
                    if (lbl_1_bss_33C[grp[lbl_1_bss_B94.f_08][i]].active != 0
                        && lbl_1_bss_33C[grp[lbl_1_bss_B94.f_08][i]].sel == 1) {
                        cnt++;
                    }
                    i++;
                } while (grp[lbl_1_bss_B94.f_08][i] != -1);
                if (cnt != 0) {
                    omVibrate(lbl_1_bss_15E0->chars[0].playerNo, 0x1E, 0xC, 0);
                    omVibrate(lbl_1_bss_15E0->chars[1].playerNo, 0x1E, 0xC, 0);
                    fn_1_3A5C(3, 0x7E, 64.0f + 0.042666666f * panTbl[lbl_1_bss_B94.f_08]);
                }
            }
        }
        break;
    case 2:
        if (lbl_1_bss_B94.f_06++ > 0x5A) {
            lbl_1_bss_B94.f_06 = 0;
            lbl_1_bss_B94.phase++;
        }
        break;
    case 3:
        if (lbl_1_bss_B94.winner == 0) {
            lbl_1_bss_B94.phase = 5;
        } else {
            lbl_1_bss_B94.phase = 4;
        }
        break;
    case 4:
        moved = fn_1_1850(&lbl_1_bss_18->center.x, 30.0f, lbl_1_data_1D8[1].center.x, model->pos.x);
        sum = moved + fn_1_1850(&lbl_1_bss_18->center.y, 30.0f, lbl_1_data_1D8[1].center.y, 70.0f + model->pos.y);
        sum = sum + fn_1_1850(&lbl_1_bss_18->center.z, 30.0f, lbl_1_data_1D8[1].center.z, model->pos.z);
        sum = sum + fn_1_1850(&lbl_1_bss_18->rotX, 30.0f, lbl_1_data_1D8[1].rotX, -18.0f);
        if ((s16)(sum + fn_1_1850(&lbl_1_bss_18->zoom, 30.0f, lbl_1_data_1D8[1].zoom, 500.0f)) == 0) {
            lbl_1_bss_B94.phase++;
        }
        break;
    case 5:
        ret = 1;
        break;
    }
    if (lbl_1_bss_B94.winner != 0) {
        switch (lbl_1_bss_B94.f_02) {
        case 0:
            lbl_1_bss_B94.f_02++;
            break;
        case 1:
            MgPlayerPadSet(lbl_1_bss_0.player[0], 0, 0, 0, 0);
            MgPlayerPadSet(lbl_1_bss_0.player[1], 0, 0, 0, 0);
            w = lbl_1_bss_B94.winner & 1;
            lbl_1_bss_8DC[w].f84 = 654.0f;
            lbl_1_bss_8DC[w].f8C = 0.0f;
            lbl_1_bss_8DC[w].fA0 = 7.0f;
            lbl_1_bss_8DC[w].f9C = 0.0f;
            dx = 654.0f - model->pos.x;
            dz = -model->pos.z;
            fn_1_E144(w);
            MgActorExec();
            if ((f32)sqrt(dx * dx + dz * dz) < 20.0f) {
                lbl_1_bss_B94.f_24 = model->rot.y;
                lbl_1_bss_B94.f_02++;
            }
            break;
        case 2:
            if (fn_1_1850(&model->rot.y, 30.0f, lbl_1_bss_B94.f_24, 0.0f) == 0) {
                slot->shiftState = 1;
                slot->shiftMotion = 0;
                slot->shiftStart = 0.0f;
                slot->shiftEnd = 8.0f;
                slot->shiftAttr = 0x40000001;
                slot->attrReset = 0;
                lbl_1_bss_B94.f_02++;
            }
            break;
        }
    }
    for (i = 0; i < 2; i++) {
        switch (lbl_1_bss_8DC[i].sub) {
        case 1:
            break;
        case 0:
            slot2 = &lbl_1_bss_15E0->slots[i];
            if (slot2->motion == 2 || slot2->motion == 3 || slot2->motion == 4) {
                slot2->shiftState = 1;
                slot2->shiftMotion = 0;
                slot2->shiftStart = 0.0f;
                slot2->shiftEnd = 8.0f;
                slot2->shiftAttr = 0x40000001;
                slot2->attrReset = 0;
            }
            lbl_1_bss_8DC[i].sub++;
            break;
        }
    }
    return ret;
}

void fn_1_A164(void)
{
    if (lbl_1_bss_8DC[0].f34 == 0) {
        if (lbl_1_bss_8DC[0].f50 != 0) {
            lbl_1_bss_8DC[0].f5C++;
        }
        if (lbl_1_bss_8DC[0].f5C > 0x28) {
            lbl_1_bss_8DC[0].f5A = 0x50;
            lbl_1_bss_8DC[1].f5A = -1;
            lbl_1_bss_8DC[1].f5C = 0;
        }
        if (lbl_1_bss_8DC[0].f5C > 0xB4) {
            lbl_1_bss_8DC[0].f5A = -1;
            lbl_1_bss_8DC[0].f5C = 0;
        }
    } else {
        lbl_1_bss_8DC[0].f5C = 0;
    }
    if (lbl_1_bss_8DC[1].f34 == 0) {
        if (lbl_1_bss_8DC[1].f50 != 0) {
            lbl_1_bss_8DC[1].f5C++;
        }
        if (lbl_1_bss_8DC[1].f5C > 0x28) {
            lbl_1_bss_8DC[1].f5A = 0x50;
            lbl_1_bss_8DC[0].f5A = -1;
            lbl_1_bss_8DC[0].f5C = 0;
        }
        if (lbl_1_bss_8DC[1].f5C > 0xB4) {
            lbl_1_bss_8DC[1].f5A = -1;
            lbl_1_bss_8DC[1].f5C = 0;
        }
    } else {
        lbl_1_bss_8DC[1].f5C = 0;
    }
    lbl_1_bss_B94.f_22 = 1;
}

int fn_1_A308(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b)
{
    f32 dx;
    f32 dz;
    f32 dist;
    s16 sp8;
    s32 type;

    sp8 = 0;
    type = b->type;
    if (type != 0) {
        if (type >= 0) {
        }
    } else if (a->paramB == 0) {
        dx = lbl_1_bss_8DC[0].f38 - lbl_1_bss_8DC[1].f38;
        dz = lbl_1_bss_8DC[0].f40 - lbl_1_bss_8DC[1].f40;
        dist = (f32)sqrt(dx * dx + dz * dz);
        if (lbl_1_bss_8DC[0].f34 == 0) {
            if (lbl_1_bss_8DC[0].f50 != 0) {
                lbl_1_bss_8DC[0].f5C++;
            }
            if (lbl_1_bss_8DC[0].f5C > 0x28) {
                lbl_1_bss_8DC[0].f5A = 0x50;
                lbl_1_bss_8DC[1].f5A = -1;
                lbl_1_bss_8DC[1].f5C = 0;
            }
            if (lbl_1_bss_8DC[0].f5C > 0xB4) {
                lbl_1_bss_8DC[0].f5A = -1;
                lbl_1_bss_8DC[0].f5C = 0;
            }
        } else {
            lbl_1_bss_8DC[0].f5C = 0;
        }
        if (lbl_1_bss_8DC[1].f34 == 0) {
            if (lbl_1_bss_8DC[1].f50 != 0) {
                lbl_1_bss_8DC[1].f5C++;
            }
            if (lbl_1_bss_8DC[1].f5C > 0x28) {
                lbl_1_bss_8DC[1].f5A = 0x50;
                lbl_1_bss_8DC[0].f5A = -1;
                lbl_1_bss_8DC[0].f5C = 0;
            }
            if (lbl_1_bss_8DC[1].f5C > 0xB4) {
                lbl_1_bss_8DC[1].f5A = -1;
                lbl_1_bss_8DC[1].f5C = 0;
            }
        } else {
            lbl_1_bss_8DC[1].f5C = 0;
        }
        lbl_1_bss_B94.f_22 = 1;
    }
    return 0;
}

void fn_1_A69C(MGACTOR *actorP, int param)
{
    s32 sp8;

    sp8 = actorP->param;
}

void fn_1_A6B0(M543Actor *actors)
{
    f32 push60[2];
    f32 push68[2];
    f32 edge[4][4];
    f32 nrm[4][2];
    s16 order[4];
    f32 minX;
    f32 minZ;
    f32 maxX;
    f32 maxZ;
    f32 scaleX;
    f32 scaleZ;
    f32 radius;
    f32 len2;
    f32 proj;
    f32 t;
    f32 k;
    f32 dx;
    f32 dz;
    f32 px;
    f32 pz;
    f32 dist;
    f32 sgn;
    f32 rotY;
    f32 lo0;
    f32 hi0;
    f32 lo1;
    f32 hi1;
    f32 ax;
    f32 az;
    f32 cx;
    f32 cz;
    f32 vx;
    f32 vz;
    s16 sp8;
    s16 i;
    s16 j;
    s16 e;
    s16 k2;
    s16 idx;
    s16 hit;
    s16 pushed;

    sp8 = 0;
#ifdef TARGET_PC
    order[0] = order[1] = order[2] = order[3] = 0;
#endif
    for (i = 0; i < 2; i++) {
        push68[i] = 0.0f;
        push60[i] = 0.0f;
    }
    radius = 40.0f + 25.0f;
    for (j = 0, idx = 0xF; j < 0x29; j++, idx++) {
        if (lbl_1_bss_33C[j].active == 0) {
            continue;
        }
        if (idx < 0x24) {
            minX = lbl_1_bss_15E0->models[idx].x - 25.0f;
            minZ = lbl_1_bss_15E0->models[idx].z - 80.5f;
            maxX = 25.0f + lbl_1_bss_15E0->models[idx].x;
            maxZ = 80.5f + lbl_1_bss_15E0->models[idx].z;
            scaleX = 1.0f;
            scaleZ = 0.0f;
        } else {
            minX = lbl_1_bss_15E0->models[idx].x - 80.5f;
            minZ = lbl_1_bss_15E0->models[idx].z - 25.0f;
            maxX = 80.5f + lbl_1_bss_15E0->models[idx].x;
            maxZ = 25.0f + lbl_1_bss_15E0->models[idx].z;
            scaleX = 0.0f;
            scaleZ = 1.0f;
        }
        hit = 0;
        for (i = 0; i < 2; i++) {
            scaleX = 1.0f;
            scaleZ = 1.0f;
            if (actors[i].f38 + radius * 1.0f >= minX && actors[i].f38 - radius * scaleX <= maxX
                && actors[i].f40 + radius * scaleZ >= minZ && actors[i].f40 - radius * scaleZ <= maxZ) {
                hit = 1;
                lbl_1_bss_33C[j].f_10 = i;
                if (actors[i].f38 + 25.0f * scaleX >= minX && actors[i].f38 - 25.0f * scaleX <= maxX
                    && actors[i].f40 + 25.0f * scaleZ >= minZ && actors[i].f40 - 25.0f * scaleZ <= maxZ) {
                    actors[i].f18 = 1;
                    actors[i].f16 = 1;
                    edge[0][0] = minX;
                    edge[0][1] = minZ;
                    edge[0][2] = maxX;
                    edge[0][3] = minZ;
                    edge[1][0] = maxX;
                    edge[1][1] = minZ;
                    edge[1][2] = maxX;
                    edge[1][3] = maxZ;
                    edge[2][0] = maxX;
                    edge[2][1] = maxZ;
                    edge[2][2] = minX;
                    edge[2][3] = maxZ;
                    edge[3][0] = minX;
                    edge[3][1] = maxZ;
                    edge[3][2] = minX;
                    edge[3][3] = minZ;
                    nrm[0][0] = 1.0f;
                    nrm[0][1] = 1.0f;
                    nrm[1][0] = 1.0f;
                    nrm[1][1] = 1.0f;
                    nrm[2][0] = 1.0f;
                    nrm[2][1] = 1.0f;
                    nrm[3][0] = 1.0f;
                    nrm[3][1] = 1.0f;
                    rotY = lbl_1_bss_0.player[i]->actor->rotY;
                    if (rotY <= 45.0f && rotY > -45.0f) {
                        order[0] = 0;
                        order[3] = 2;
                        if (rotY <= 45.0f && rotY > 0.0f) {
                            order[1] = 3;
                            order[2] = 1;
                        }
                        if (rotY <= 0.0f && rotY > -45.0f) {
                            order[1] = 1;
                            order[2] = 3;
                        }
                    }
                    if (rotY <= 135.0f && rotY > 45.0f) {
                        order[0] = 3;
                        order[3] = 1;
                        if (rotY <= 135.0f && rotY > 90.0f) {
                            order[1] = 2;
                            order[2] = 0;
                        }
                        if (rotY <= 90.0f && rotY > 45.0f) {
                            order[1] = 0;
                            order[2] = 2;
                        }
                    }
                    if (rotY <= -45.0f && rotY > -135.0f) {
                        order[0] = 1;
                        order[3] = 3;
                        if (rotY <= -45.0f && rotY > -90.0f) {
                            order[1] = 0;
                            order[2] = 2;
                        }
                        if (rotY <= -90.0f && rotY > -135.0f) {
                            order[1] = 2;
                            order[2] = 0;
                        }
                    }
                    if (rotY <= -135.0f || rotY > 135.0f) {
                        order[0] = 2;
                        order[3] = 0;
                        if (rotY <= -135.0f) {
                            order[1] = 1;
                            order[2] = 3;
                        }
                        if (rotY > 135.0f) {
                            order[1] = 3;
                            order[2] = 1;
                        }
                    }
                    pushed = 0;
                    for (k2 = 0; k2 < 4; k2++) {
                        e = order[k2];
                        if (actors[i].f38 + 25.0f * nrm[e][0] >= minX && actors[i].f38 - 25.0f * nrm[e][0] <= maxX
                            && actors[i].f40 + 25.0f * nrm[e][1] >= minZ && actors[i].f40 - 25.0f * nrm[e][1] <= maxZ) {
                            dx = edge[e][0] - edge[e][2];
                            dz = edge[e][1] - edge[e][3];
                            len2 = dx * dx + dz * dz;
                            proj = -1.0f * (dx * (edge[e][0] - actors[i].f38) + dz * (edge[e][1] - actors[i].f40));
                            t = proj / len2;
                            ax = dx * t + edge[e][0];
                            az = dz * t + edge[e][1];
                            dx = actors[i].f38 - ax;
                            dz = actors[i].f40 - az;
                            dist = (f32)sqrt(dx * dx + dz * dz);
                            if (dist < 25.0f) {
                                if (dist <= 1.0f) {
                                    dist = 1.0f;
                                }
                                k = (1.0f + 25.0f) / dist;
                                actors[i].f38 = ax + 1.0f * (dx * k);
                                actors[i].f40 = az + 1.0f * (dz * k);
                                if (pushed == 0) {
                                    switch (e) {
                                    case 0:
                                        push60[i] = -1.0f;
                                        break;
                                    case 1:
                                        push68[i] = 1.0f;
                                        break;
                                    case 2:
                                        push60[i] = 1.0f;
                                        break;
                                    case 3:
                                        push68[i] = -1.0f;
                                        break;
                                    }
                                    pushed = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
        if (hit != 0 && lbl_1_bss_33C[j].step == 2) {
            lbl_1_bss_33C[j].sel = 0;
            if (lbl_1_bss_33C[j].sel != lbl_1_bss_33C[j].prevSel) {
                omVibrate(lbl_1_bss_15E0->chars[lbl_1_bss_33C[j].f_10].playerNo, 0xA, 4, 2);
                actors[0].tileHit[j] = 1;
                actors[1].tileHit[j] = 1;
                fn_1_3A5C(0, 0x7E, 64.0f + 0.042666666f * lbl_1_bss_15E0->models[idx].x);
            }
        } else if (lbl_1_bss_33C[j].step == 2) {
            lbl_1_bss_33C[j].sel = 1;
        }
    }
    for (i = 0; i < 2; i++) {
        if (push68[i] != 0.0f || push60[i] != 0.0f) {
            actors[i].f1C = actors[i].f38;
            actors[i].f20 = actors[i].f3C;
            actors[i].f24 = actors[i].f40;
            actors[i].f28 = actors[i].f38;
            actors[i].f2C = actors[i].f3C;
            actors[i].f30 = actors[i].f40;
            if (push68[i] != 0.0f) {
                actors[i].f28 = actors[i].f38 + 30.0f * push68[i];
            }
            if (push60[i] != 0.0f) {
                actors[i].f30 = actors[i].f40 + 30.0f * push60[i];
            }
        }
    }
    for (i = 0; i < 2; i++) {
        if (actors[i ^ 1].f16 != 0) {
            dx = actors[i ^ 1].f38 - actors[i].f38;
            dz = actors[i ^ 1].f40 - actors[i].f40;
            dist = (f32)sqrt(dx * dx + dz * dz);
            if (dist < 70.0f) {
                if (dist <= 1.0f) {
                    dist = 1.0f;
                }
                k = 70.0f / dist;
                actors[i].f38 = actors[i ^ 1].f38 - dx * k;
                actors[i].f40 = actors[i ^ 1].f40 - dz * k;
                if (actors[0].f34 == 0) {
                    if (actors[0].f50 != 0) {
                        actors[0].f5C++;
                    }
                    if (actors[0].f5C > 0x28) {
                        actors[0].f5A = 0x50;
                        actors[1].f5A = -1;
                        actors[1].f5C = 0;
                    }
                    if (actors[0].f5C > 0xB4) {
                        actors[0].f5A = -1;
                        actors[0].f5C = 0;
                    }
                } else {
                    actors[0].f5C = 0;
                }
                if (actors[1].f34 == 0) {
                    if (actors[1].f50 != 0) {
                        actors[1].f5C++;
                    }
                    if (actors[1].f5C > 0x28) {
                        actors[1].f5A = 0x50;
                        actors[0].f5A = -1;
                        actors[0].f5C = 0;
                    }
                    if (actors[1].f5C > 0xB4) {
                        actors[1].f5A = -1;
                        actors[1].f5C = 0;
                    }
                } else {
                    actors[1].f5C = 0;
                }
                lbl_1_bss_B94.f_22 = 1;
            }
        }
        for (j = 0; j < 4; j++) {
            dx = lbl_1_data_500[j][0] - actors[i].f38;
            dz = lbl_1_data_500[j][1] - actors[i].f40;
            dist = (f32)sqrt(dx * dx + dz * dz);
            if (dist < 1.0f + 25.0f) {
                if (dist <= 1.0f) {
                    dist = 1.0f;
                }
                k = (1.0f + 25.0f) / dist;
                actors[i].f38 = lbl_1_data_500[j][0] - dx * k;
                actors[i].f40 = lbl_1_data_500[j][1] - dz * k;
            }
        }
        ax = actors[i].f38;
        az = actors[i].f40;
        if ((!(-ax + az > -784.0f) || !(ax < 554.0f) || !(ax + az < 784.0f) || !(az < 554.0f) || !(-ax + az < 784.0f)
             || !(ax > -554.0f) || !(ax + az > -784.0f) || !(az > -554.0f))
            && (!(ax > -754.0f) || !(az > -150.0f) || !(az < 150.0f))
            && (!(ax < 754.0f) || !(az > -150.0f) || !(az < 150.0f))) {
            actors[i].f38 = actors[i].f44;
            actors[i].f40 = actors[i].f4C;
        }
        for (j = 0; j < 0x10; j++) {
            if (lbl_1_data_380[j][0] < lbl_1_data_380[j][2]) {
                lo0 = lbl_1_data_380[j][0];
                hi0 = lbl_1_data_380[j][2];
            } else {
                lo0 = lbl_1_data_380[j][2];
                hi0 = lbl_1_data_380[j][0];
            }
            if (lbl_1_data_380[j][1] < lbl_1_data_380[j][3]) {
                lo1 = lbl_1_data_380[j][1];
                hi1 = lbl_1_data_380[j][3];
            } else {
                lo1 = lbl_1_data_380[j][3];
                hi1 = lbl_1_data_380[j][1];
            }
            if (actors[i].f38 + 25.0f * lbl_1_data_380[j][4] >= lo0
                && actors[i].f38 - 25.0f * lbl_1_data_380[j][4] <= hi0
                && actors[i].f40 + 25.0f * lbl_1_data_380[j][5] >= lo1
                && actors[i].f40 - 25.0f * lbl_1_data_380[j][5] <= hi1) {
                dx = lbl_1_data_380[j][0] - lbl_1_data_380[j][2];
                dz = lbl_1_data_380[j][1] - lbl_1_data_380[j][3];
                len2 = dx * dx + dz * dz;
                proj = -1.0f * (dx * (lbl_1_data_380[j][0] - actors[i].f38)
                                + dz * (lbl_1_data_380[j][1] - actors[i].f40));
                t = proj / len2;
                cx = dx * t + lbl_1_data_380[j][0];
                cz = dz * t + lbl_1_data_380[j][1];
                vx = actors[i].f38 - cx;
                vz = actors[i].f40 - cz;
                dist = (f32)sqrt(vx * vx + vz * vz);
                if (dist < 25.0f) {
                    if (dist <= 1.0f) {
                        dist = 1.0f;
                    }
                    sgn = 1.0f;
                    ax = actors[i].f38;
                    az = actors[i].f40;
                    switch (j) {
                    case 0:
                        if (-ax + az < -784.0f) {
                            sgn = -1.0f;
                        }
                        break;
                    case 1:
                        if (ax > 554.0f) {
                            sgn = -1.0f;
                        }
                        break;
                    case 5:
                        if (ax > 554.0f) {
                            sgn = -1.0f;
                        }
                        break;
                    case 6:
                        if (ax + az > 784.0f) {
                            sgn = -1.0f;
                        }
                        break;
                    case 7:
                        if (az > 554.0f) {
                            sgn = -1.0f;
                        }
                        break;
                    case 8:
                        if (-ax + az > 784.0f) {
                            sgn = -1.0f;
                        }
                        break;
                    case 9:
                        if (ax < -554.0f) {
                            sgn = -1.0f;
                        }
                        break;
                    case 13:
                        if (ax < -554.0f) {
                            sgn = -1.0f;
                        }
                        break;
                    case 14:
                        if (ax + az < -784.0f) {
                            sgn = -1.0f;
                        }
                        break;
                    case 15:
                        if (az < -554.0f) {
                            sgn = -1.0f;
                        }
                        break;
                    }
                    k = (1.0f + 25.0f) / dist;
                    actors[i].f38 = cx + sgn * (vx * k);
                    actors[i].f40 = cz + sgn * (vz * k);
                }
            }
        }
        for (j = 0; j < 0x10; j++) {
            if (lbl_1_data_380[j][0] < lbl_1_data_380[j][2]) {
                lo0 = lbl_1_data_380[j][0];
                hi0 = lbl_1_data_380[j][2];
            } else {
                lo0 = lbl_1_data_380[j][2];
                hi0 = lbl_1_data_380[j][0];
            }
            if (lbl_1_data_380[j][1] < lbl_1_data_380[j][3]) {
                lo1 = lbl_1_data_380[j][1];
                hi1 = lbl_1_data_380[j][3];
            } else {
                lo1 = lbl_1_data_380[j][3];
                hi1 = lbl_1_data_380[j][1];
            }
            if (actors[i].f38 + 25.0f * lbl_1_data_380[j][4] >= lo0
                && actors[i].f38 - 25.0f * lbl_1_data_380[j][4] <= hi0
                && actors[i].f40 + 25.0f * lbl_1_data_380[j][5] >= lo1
                && actors[i].f40 - 25.0f * lbl_1_data_380[j][5] <= hi1) {
                dx = lbl_1_data_380[j][0] - lbl_1_data_380[j][2];
                dz = lbl_1_data_380[j][1] - lbl_1_data_380[j][3];
                len2 = dx * dx + dz * dz;
                proj = -1.0f * (dx * (lbl_1_data_380[j][0] - actors[i].f38)
                                + dz * (lbl_1_data_380[j][1] - actors[i].f40));
                t = proj / len2;
                cx = dx * t + lbl_1_data_380[j][0];
                cz = dz * t + lbl_1_data_380[j][1];
                vx = actors[i].f38 - cx;
                vz = actors[i].f40 - cz;
                dist = (f32)sqrt(vx * vx + vz * vz);
                if (dist < 25.0f) {
                    if (dist <= 1.0f) {
                        dist = 1.0f;
                    }
                    sgn = 1.0f;
                    ax = actors[i].f38;
                    az = actors[i].f40;
                    switch (j) {
                    case 0:
                        if (-ax + az < -784.0f) {
                            sgn = -1.0f;
                        }
                        break;
                    case 1:
                        if (ax > 554.0f) {
                            sgn = -1.0f;
                        }
                        break;
                    case 5:
                        if (ax > 554.0f) {
                            sgn = -1.0f;
                        }
                        break;
                    case 6:
                        if (ax + az > 784.0f) {
                            sgn = -1.0f;
                        }
                        break;
                    case 7:
                        if (az > 554.0f) {
                            sgn = -1.0f;
                        }
                        break;
                    case 8:
                        if (-ax + az > 784.0f) {
                            sgn = -1.0f;
                        }
                        break;
                    case 9:
                        if (ax < -554.0f) {
                            sgn = -1.0f;
                        }
                        break;
                    case 13:
                        if (ax < -554.0f) {
                            sgn = -1.0f;
                        }
                        break;
                    case 14:
                        if (ax + az < -784.0f) {
                            sgn = -1.0f;
                        }
                        break;
                    case 15:
                        if (az < -554.0f) {
                            sgn = -1.0f;
                        }
                        break;
                    }
                    k = (1.0f + 25.0f) / dist;
                    actors[i].f38 = cx + sgn * (vx * k);
                    actors[i].f40 = cz + sgn * (vz * k);
                }
            }
        }
        if (lbl_1_bss_B94.winner == 0 && lbl_1_data_500[0][0] < actors[i].f38) {
            lbl_1_bss_B94.winner = i | ((i ^ 1) * 2);
        }
    }
}

void fn_1_D408(s16 arg0)
{
    f32 speedTbl[4] = { 7.2f, 8.0f, 9.0f, 10.28571f };
    f32 paramTbl[4][5] = {
        { 25.0f, 25.0f, 25.0f, 25.0f, 10.0f },
        { 25.0f, 25.0f, 15.0f, 35.0f, 15.0f },
        { 20.0f, 20.0f, 5.0f, 55.0f, 20.0f },
        { 15.0f, 15.0f, 10.0f, 60.0f, 30.0f },
    };
    f32 fieldTbl[4] = { 2.88f, 3.6f, 4.8f, 7.2f };
    s16 chanceTbl[4] = { 50, 40, 30, 15 };
    s16 waitTbl[4] = { 20, 15, 10, 3 };
    s16 lvl;

    if (lbl_1_bss_15E0->chars[arg0].cpuLevel != 0) {
        lbl_1_bss_8DC[arg0].f50 = 1;
        lbl_1_bss_8DC[arg0].f52 = lbl_1_bss_15E0->chars[arg0].cpuLevel - 1;
        lbl_1_bss_8DC[arg0].fA4 = speedTbl[lbl_1_bss_8DC[arg0].f52];
        lbl_1_bss_8DC[arg0].f9C = 11.0f;
        lbl_1_bss_8DC[arg0].f74[0] = paramTbl[lbl_1_bss_8DC[arg0].f52][0];
        lbl_1_bss_8DC[arg0].f74[1] = paramTbl[lbl_1_bss_8DC[arg0].f52][1];
        lbl_1_bss_8DC[arg0].f74[2] = paramTbl[lbl_1_bss_8DC[arg0].f52][2];
        lbl_1_bss_8DC[arg0].f74[3] = paramTbl[lbl_1_bss_8DC[arg0].f52][3];
        lbl_1_bss_8DC[arg0].f74[4] = paramTbl[lbl_1_bss_8DC[arg0].f52][4];
        lbl_1_bss_8DC[arg0].f7E = chanceTbl[lbl_1_bss_8DC[arg0].f52];
        lbl_1_bss_8DC[arg0].fA8 = fieldTbl[lbl_1_bss_8DC[arg0].f52];
        lbl_1_bss_8DC[arg0].f142 = waitTbl[lbl_1_bss_8DC[arg0].f52];
        lbl_1_bss_8DC[arg0].f106[0] = -1;
        lbl_1_bss_8DC[arg0].f106[4] = -1;
        lbl_1_bss_8DC[arg0].f106[0x1A] = -1;
        lbl_1_bss_8DC[arg0].f106[0x1D] = -1;
        lbl_1_bss_8DC[arg0].tileHit[0x15] = 1;
        lbl_1_bss_8DC[arg0].tileHit[0x16] = 1;
        lbl_1_bss_8DC[arg0].tileHit[0x27] = 1;
        lbl_1_bss_8DC[arg0].tileHit[0x28] = 1;
        if (arg0 == 0) {
            lbl_1_bss_8DC[arg0].f146 = frandmod(2) + 1;
        } else if (arg0 == 1) {
            if (lbl_1_bss_8DC[0].f146 == 1) {
                lbl_1_bss_8DC[1].f146 = frandmod(2) + 2;
            } else if (lbl_1_bss_8DC[0].f146 == 2) {
                lbl_1_bss_8DC[1].f146 = 3;
            } else {
                lbl_1_bss_8DC[1].f146 = frandmod(3) + 1;
            }
        }
        lvl = lbl_1_bss_8DC[arg0].f146;
        lbl_1_bss_8DC[arg0].f144 = lvl;
        lbl_1_bss_8DC[arg0].f148 = lvl;
    }
}

s32 fn_1_D9E8(s16 arg0)
{
    s16 dirTbl[4] = { -1, 1, -5, 5 };
    s16 used[4] = { 0, 0, 0, 0 };
    s16 bonus[4] = { 0, 0, 0, 0 };
    s32 score[4];
    s16 best;
    s16 sel;
    s16 i;
    s16 j;
    s16 r;

    lbl_1_bss_8DC[arg0].stkX = 0;
    lbl_1_bss_8DC[arg0].stkY = 0;
    lbl_1_bss_8DC[arg0].btnDown = 0;
    lbl_1_bss_8DC[arg0].btn = 0;
    switch (lbl_1_bss_8DC[arg0].f54) {
    case 0:
        lbl_1_bss_8DC[arg0].f84 = lbl_1_bss_1C[lbl_1_bss_8DC[arg0].f148].x;
        lbl_1_bss_8DC[arg0].f88 = lbl_1_bss_1C[lbl_1_bss_8DC[arg0].f148].y;
        lbl_1_bss_8DC[arg0].f8C = lbl_1_bss_1C[lbl_1_bss_8DC[arg0].f148].z;
        lbl_1_bss_8DC[arg0].fA0 = lbl_1_bss_8DC[arg0].fA4;
        lbl_1_bss_8DC[arg0].f54++;
        /* fallthrough */
    case 1:
        if (fn_1_E144(arg0) != 0) {
            if (lbl_1_bss_8DC[arg0].f146 == 0x1A || lbl_1_bss_8DC[arg0].f146 == 0x1B || lbl_1_bss_8DC[arg0].f146 == 0x1C) {
                lbl_1_bss_8DC[arg0].f54 = 3;
            } else {
                r = fn_1_EF58(arg0);
                if (r == 2) {
                    lbl_1_bss_8DC[arg0].f54 = 2;
                } else if (r == 1) {
                    lbl_1_bss_8DC[arg0].f84 = lbl_1_bss_1C[lbl_1_bss_8DC[arg0].f148].x;
                    lbl_1_bss_8DC[arg0].f88 = lbl_1_bss_1C[lbl_1_bss_8DC[arg0].f148].y;
                    lbl_1_bss_8DC[arg0].f8C = lbl_1_bss_1C[lbl_1_bss_8DC[arg0].f148].z;
                    lbl_1_bss_8DC[arg0].fA0 = lbl_1_bss_8DC[arg0].fA4;
                    if ((lbl_1_bss_8DC[arg0].f52 == 2 || lbl_1_bss_8DC[arg0].f52 == 3) && lbl_1_bss_8DC[arg0].f146 >= 0xF) {
                        switch (lbl_1_bss_8DC[arg0].f146) {
                        case 15:
                        case 16:
                        case 20:
                        case 21:
                        case 25:
                        case 26:
                            bonus[1] = lbl_1_bss_8DC[arg0].f74[4];
                            break;
                        case 18:
                        case 19:
                        case 23:
                        case 24:
                        case 28:
                        case 29:
                            bonus[0] = lbl_1_bss_8DC[arg0].f74[4];
                            break;
                        }
                    }
                    for (i = 0; i < 4; i++) {
                        for (j = 0; j < 4; j++) {
                            if (used[j] == 0) {
                                score[j] = frandmod(lbl_1_bss_8DC[arg0].f74[j] + bonus[j]);
                            } else {
                                score[j] = -1;
                            }
                        }
                        best = 0;
                        sel = 0;
                        for (j = 0; j < 4; j++) {
                            if (score[j] >= best) {
                                best = score[j];
                                sel = j;
                            }
                        }
                        used[sel] = 1;
                        lbl_1_data_8B0[arg0][i] = sel;
                    }
                    for (i = 0; i < 4; i++) {
                        lbl_1_data_8A0[arg0][i] = dirTbl[lbl_1_data_8B0[arg0][i]];
                    }
                }
            }
        }
        break;
    case 2:
        if (fn_1_E340(arg0) != 0) {
            lbl_1_bss_8DC[arg0].f54 = 1;
        }
        break;
    case 3:
        lbl_1_bss_8DC[arg0].f84 = lbl_1_bss_1C[27].x;
        lbl_1_bss_8DC[arg0].f88 = lbl_1_bss_1C[27].y;
        lbl_1_bss_8DC[arg0].f8C = lbl_1_bss_1C[27].z;
        lbl_1_bss_8DC[arg0].fA0 = lbl_1_bss_8DC[arg0].fA4;
        if (fn_1_E144(arg0) != 0) {
            lbl_1_bss_8DC[arg0].f54++;
        }
        break;
    case 4:
        lbl_1_bss_8DC[arg0].f84 = 654.0f;
        lbl_1_bss_8DC[arg0].f8C = 0.0f;
        lbl_1_bss_8DC[arg0].fA0 = lbl_1_bss_8DC[arg0].fA4;
        fn_1_E144(arg0);
        break;
    }
    return 0;
}

s16 fn_1_E144(s16 arg0)
{
    M543Vec3 pos;
    s16 ret;

    ret = 0;
    switch (lbl_1_bss_8DC[arg0].f80) {
    case 0:
        MgPlayerComStkOn(lbl_1_bss_0.player[arg0]);
        lbl_1_bss_8DC[arg0].f80++;
        break;
    case 1:
        pos.x = lbl_1_bss_8DC[arg0].f84;
        pos.y = lbl_1_bss_8DC[arg0].f88;
        pos.z = lbl_1_bss_8DC[arg0].f8C;
        if (arg0 == 0) {
            pos.x += 4.0f;
            pos.z += 4.0f;
        } else {
            pos.x -= 4.0f;
            pos.z -= 4.0f;
        }
        if (MgPlayerVecChase(lbl_1_bss_0.player[arg0], (HuVecF *)&pos, lbl_1_bss_8DC[arg0].fA0, lbl_1_bss_8DC[arg0].f9C) != 0) {
            ret = 1;
        }
        break;
    case 2:
        MgPlayerComStkOff(lbl_1_bss_0.player[arg0]);
        lbl_1_bss_8DC[arg0].f80 = 0;
        ret = 1;
        break;
    }
    return ret;
}

s16 fn_1_E340(s16 arg0)
{
    f32 offs[4][4] = {
        { 0.0f, -40.0f, 0.0f, -40.0f },
        { 0.0f, 40.0f, 0.0f, 40.0f },
        { -40.0f, 0.0f, -40.0f, 0.0f },
        { 40.0f, 0.0f, 40.0f, 0.0f },
    };
    M543Vec3 pos;
    f32 bonus;
    s16 ret;
    s16 done;
    s16 chased;

    bonus = 0.0f;
    ret = 0;
    switch (lbl_1_bss_8DC[arg0].f62) {
    case 0:
        if (lbl_1_bss_33C[lbl_1_bss_1C[lbl_1_bss_8DC[arg0].f146].adj[lbl_1_data_8B0[arg0][lbl_1_bss_8DC[arg0].f66]]].active != 0
            && frandmod(0x65) < lbl_1_bss_8DC[arg0].f7E) {
            bonus = 1.0f;
        }
        lbl_1_bss_8DC[arg0].f84 = lbl_1_bss_1C[lbl_1_bss_8DC[arg0].f146].x
            + offs[lbl_1_data_8B0[arg0][lbl_1_bss_8DC[arg0].f66]][0]
            + bonus * offs[lbl_1_data_8B0[arg0][lbl_1_bss_8DC[arg0].f66]][2];
        lbl_1_bss_8DC[arg0].f8C = lbl_1_bss_1C[lbl_1_bss_8DC[arg0].f146].z
            + offs[lbl_1_data_8B0[arg0][lbl_1_bss_8DC[arg0].f66]][1]
            + bonus * offs[lbl_1_data_8B0[arg0][lbl_1_bss_8DC[arg0].f66]][3];
        lbl_1_bss_8DC[arg0].fA0 = 5.0f * bonus + lbl_1_bss_8DC[arg0].fA8;
        lbl_1_bss_8DC[arg0].f5E = 0;
        lbl_1_bss_8DC[arg0].f90 = lbl_1_bss_8DC[arg0].f84;
        lbl_1_bss_8DC[arg0].f94 = lbl_1_bss_8DC[arg0].f88;
        lbl_1_bss_8DC[arg0].f98 = lbl_1_bss_8DC[arg0].f8C;
        lbl_1_bss_8DC[arg0].f62++;
        /* fallthrough */
    case 1:
        done = 0;
        switch (lbl_1_bss_8DC[arg0].f80) {
        case 0:
            MgPlayerComStkOn(lbl_1_bss_0.player[arg0]);
            lbl_1_bss_8DC[arg0].f80++;
            break;
        case 1:
            pos.x = lbl_1_bss_8DC[arg0].f84;
            pos.y = lbl_1_bss_8DC[arg0].f88;
            pos.z = lbl_1_bss_8DC[arg0].f8C;
            if (arg0 == 0) {
                pos.x += 4.0f;
                pos.z += 4.0f;
            } else {
                pos.x -= 4.0f;
                pos.z -= 4.0f;
            }
            if (MgPlayerVecChase(lbl_1_bss_0.player[arg0], (HuVecF *)&pos, lbl_1_bss_8DC[arg0].fA0, lbl_1_bss_8DC[arg0].f9C) != 0) {
                done = 1;
            }
            break;
        case 2:
            MgPlayerComStkOff(lbl_1_bss_0.player[arg0]);
            lbl_1_bss_8DC[arg0].f80 = 0;
            done = 1;
            break;
        }
        chased = done;
        if (chased == 0) {
            if (lbl_1_bss_8DC[arg0].f5E++ >= 0x5A) {
                chased = 1;
            }
        }
        if (lbl_1_bss_8DC[arg0].f18 != 0) {
            chased = 1;
        }
        if (chased != 0) {
            lbl_1_bss_8DC[arg0].f64 = 0;
            lbl_1_bss_8DC[arg0].f62++;
        }
        break;
    case 2:
        if (lbl_1_bss_8DC[arg0].f64++ > lbl_1_bss_8DC[arg0].f142) {
            lbl_1_bss_8DC[arg0].f62++;
        }
        break;
    case 3:
        if (lbl_1_bss_33C[lbl_1_bss_1C[lbl_1_bss_8DC[arg0].f146].adj[lbl_1_data_8B0[arg0][lbl_1_bss_8DC[arg0].f66]]].active != 0) {
            lbl_1_bss_8DC[arg0].tileHit[lbl_1_bss_1C[lbl_1_bss_8DC[arg0].f146].adj[lbl_1_data_8B0[arg0][lbl_1_bss_8DC[arg0].f66]]] = 1;
            lbl_1_bss_8DC[arg0].f66 = -1;
            lbl_1_bss_8DC[arg0].f62++;
        } else {
            lbl_1_bss_8DC[arg0].tileHit[lbl_1_bss_1C[lbl_1_bss_8DC[arg0].f146].adj[lbl_1_data_8B0[arg0][lbl_1_bss_8DC[arg0].f66]]] = 2;
            lbl_1_bss_8DC[arg0].f60++;
            lbl_1_bss_8DC[arg0].f62 = 0;
            ret = 1;
        }
        break;
    case 4:
        lbl_1_bss_8DC[arg0].f84 = lbl_1_bss_1C[lbl_1_bss_8DC[arg0].f146].x;
        lbl_1_bss_8DC[arg0].f8C = lbl_1_bss_1C[lbl_1_bss_8DC[arg0].f146].z;
        lbl_1_bss_8DC[arg0].fA0 = lbl_1_bss_8DC[arg0].fA8;
        done = 0;
        switch (lbl_1_bss_8DC[arg0].f80) {
        case 0:
            MgPlayerComStkOn(lbl_1_bss_0.player[arg0]);
            lbl_1_bss_8DC[arg0].f80++;
            break;
        case 1:
            pos.x = lbl_1_bss_8DC[arg0].f84;
            pos.y = lbl_1_bss_8DC[arg0].f88;
            pos.z = lbl_1_bss_8DC[arg0].f8C;
            if (arg0 == 0) {
                pos.x += 4.0f;
                pos.z += 4.0f;
            } else {
                pos.x -= 4.0f;
                pos.z -= 4.0f;
            }
            if (MgPlayerVecChase(lbl_1_bss_0.player[arg0], (HuVecF *)&pos, lbl_1_bss_8DC[arg0].fA0, lbl_1_bss_8DC[arg0].f9C) != 0) {
                done = 1;
            }
            break;
        case 2:
            MgPlayerComStkOff(lbl_1_bss_0.player[arg0]);
            lbl_1_bss_8DC[arg0].f80 = 0;
            done = 1;
            break;
        }
        if (done != 0) {
            lbl_1_bss_8DC[arg0].f62 = 0;
            ret = 1;
        }
        break;
    }
    return ret;
}

s16 fn_1_EF58(s16 arg0)
{
    s16 ret;
    s16 cnt;
    s16 i;
    s16 v28;
    s16 v29;
    s16 t;

    ret = 0;
    cnt = 0;
    switch (lbl_1_bss_8DC[arg0].f60) {
    case 0:
        lbl_1_bss_8DC[arg0].f66 = -1;
        lbl_1_bss_8DC[arg0].f68 = 0;
        lbl_1_bss_8DC[arg0].f6A = 0;
        for (i = 0; i < 4; i++) {
            t = lbl_1_bss_8DC[arg0].f146 + lbl_1_data_8A0[arg0][i];
            if (t != lbl_1_bss_8DC[arg0].f144
                && lbl_1_bss_1C[lbl_1_bss_8DC[arg0].f146].adj[lbl_1_data_8B0[arg0][i]] != -1 && t >= 0 && t <= 0x1D) {
                if (lbl_1_bss_8DC[arg0].tileHit[lbl_1_bss_1C[lbl_1_bss_8DC[arg0].f146].adj[lbl_1_data_8B0[arg0][i]]] == 0) {
                    if (lbl_1_bss_1C[lbl_1_bss_8DC[arg0].f146].adj[lbl_1_data_8B0[arg0][i]] < 0x29) {
                        lbl_1_bss_8DC[arg0].f66 = i;
                        return 2;
                    }
                    lbl_1_bss_8DC[arg0].f66 = i;
                } else if (lbl_1_bss_8DC[arg0].tileHit[lbl_1_bss_1C[lbl_1_bss_8DC[arg0].f146].adj[lbl_1_data_8B0[arg0][i]]] == 2) {
                    if (lbl_1_bss_8DC[arg0].f144 != lbl_1_bss_8DC[arg0].f146 + lbl_1_data_8A0[arg0][i]) {
                        lbl_1_bss_8DC[arg0].f68 = i;
                    }
                    lbl_1_bss_8DC[arg0].f6C[lbl_1_bss_8DC[arg0].f6A] = i;
                    lbl_1_bss_8DC[arg0].f6A++;
                }
            }
        }
        lbl_1_bss_8DC[arg0].f60++;
        return ret;
    case 1:
        if (lbl_1_bss_8DC[arg0].f66 != -1) {
            lbl_1_bss_8DC[arg0].f14A = 0;
        } else {
            lbl_1_bss_8DC[arg0].f14A = 1;
        }
        for (i = 0; i < 4; i++) {
            if (lbl_1_bss_1C[lbl_1_bss_8DC[arg0].f146].adj[i] == -1) {
                cnt++;
            } else if (lbl_1_bss_8DC[arg0].tileHit[lbl_1_bss_1C[lbl_1_bss_8DC[arg0].f146].adj[i]] == 1) {
                cnt++;
            }
        }
        v28 = 0;
        v29 = 0;
        if (cnt >= 3) {
            lbl_1_bss_8DC[arg0].f106[lbl_1_bss_8DC[arg0].f146] = 3;
            lbl_1_bss_8DC[arg0].f14A = 1;
        } else if (cnt == 2) {
            if (lbl_1_bss_8DC[arg0].f106[lbl_1_bss_8DC[arg0].f146 + lbl_1_data_8A0[arg0][lbl_1_bss_8DC[arg0].f68]] == 2) {
                lbl_1_bss_8DC[arg0].f14A = 1;
            }
            lbl_1_bss_8DC[arg0].f106[lbl_1_bss_8DC[arg0].f146] = 2;
            if (lbl_1_bss_8DC[arg0].f14A != 0) {
                lbl_1_bss_8DC[arg0].f106[lbl_1_bss_8DC[arg0].f146] = 3;
            }
        } else if (cnt <= 1) {
            if (lbl_1_bss_8DC[arg0].f14A != 0) {
                v28 = 0;
                for (i = 0; i < lbl_1_bss_8DC[arg0].f6A; i++) {
                    if (lbl_1_bss_8DC[arg0].f106[lbl_1_bss_8DC[arg0].f146 + lbl_1_data_8A0[arg0][lbl_1_bss_8DC[arg0].f6C[i]]] != 3) {
                        if (v28 == 0) {
                            v29 = lbl_1_bss_8DC[arg0].f6C[i];
                        }
                        v28++;
                    }
                }
                lbl_1_bss_8DC[arg0].f106[lbl_1_bss_8DC[arg0].f146] = 3;
            } else {
                lbl_1_bss_8DC[arg0].f106[lbl_1_bss_8DC[arg0].f146] = 1;
            }
        }
        if (lbl_1_bss_8DC[arg0].f66 != -1) {
            lbl_1_bss_8DC[arg0].f148 = lbl_1_bss_8DC[arg0].f146 + lbl_1_data_8A0[arg0][lbl_1_bss_8DC[arg0].f66];
        } else if (lbl_1_bss_8DC[arg0].f14A != 0) {
            if (v28 != 0) {
                lbl_1_bss_8DC[arg0].f148 = lbl_1_bss_8DC[arg0].f146 + lbl_1_data_8A0[arg0][v29];
            } else if (cnt >= 3) {
                lbl_1_bss_8DC[arg0].f148 = lbl_1_bss_8DC[arg0].f144;
            } else {
                lbl_1_bss_8DC[arg0].f148 = lbl_1_bss_8DC[arg0].f146 + lbl_1_data_8A0[arg0][lbl_1_bss_8DC[arg0].f68];
            }
        }
        lbl_1_bss_8DC[arg0].f144 = lbl_1_bss_8DC[arg0].f146;
        lbl_1_bss_8DC[arg0].f146 = lbl_1_bss_8DC[arg0].f148;
        lbl_1_bss_8DC[arg0].f60 = 0;
        ret = 1;
        return ret;
    default:
        return ret;
    }
}
