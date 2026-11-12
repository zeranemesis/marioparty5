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
s32 fn_1_A308(void *arg0, void *arg1);
void fn_1_A69C(void *arg0);
void fn_1_A6B0(M543Actor *actors);
void fn_1_D408(s16 arg0);
s32 fn_1_D9E8(s16 arg0);
s16 fn_1_E144(s16 arg0, M543Vec3 *pos);
s16 fn_1_E340(s16 arg0);
s16 fn_1_EF58(s16 arg0);

void MgActorPosGet(MGACTOR *actorP, HuVecF *pos);

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
s32 (*lbl_1_data_7F4[2])(s16) = { fn_1_7394, fn_1_7704 };

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
    param.narrowHook = (COL_NARROW_HOOK)fn_1_A308;
    param.correctHookParam = 0;
    param.correctHook = (MGACTOR_CORRECT_HOOK)fn_1_A69C;
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

    fn_1_3CEC(state);
    MgPlayerPadSet(lbl_1_bss_0.player[0], 0, 0, 0, 0);
    MgPlayerPadSet(lbl_1_bss_0.player[1], 0, 0, 0, 0);
    MgActorExec();
    model0 = &Hu3DData[lbl_1_bss_15E0->models[0x3C].id];
    lbl_1_bss_8DC[0].f38 = model0->pos.x;
    lbl_1_bss_8DC[0].f3C = model0->pos.y;
    lbl_1_bss_8DC[0].f40 = model0->pos.z;
    model1 = &Hu3DData[lbl_1_bss_15E0->models[0x3D].id];
    lbl_1_bss_8DC[1].f38 = model1->pos.x;
    lbl_1_bss_8DC[1].f3C = model1->pos.y;
    lbl_1_bss_8DC[1].f40 = model1->pos.z;
    return 1;
}

s32 fn_1_5B78(void)
{
    s16 ret;

    ret = fn_1_86E4();
    fn_1_6770();
    return ret;
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
    s16 ret;

    ret = fn_1_91F8();
    fn_1_6770();
    return ret;
}

s32 fn_1_634C(void)
{
    M543StageSlot *slot;

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
        lbl_1_bss_B94.f_24 = Hu3DData[lbl_1_bss_15E0->models[(lbl_1_bss_B94.winner & 1) + 0x3C].id].rot.y;
    }
    fn_1_6770();
    CharModelVoicePanSet(lbl_1_bss_15E0->chars[lbl_1_bss_B94.winner & 1].charNo, 0x40);
    return 0;
}

s32 fn_1_64AC(void)
{
    fn_1_6770();
    return 1;
}

s32 fn_1_64DC(void)
{
    fn_1_3CCC();
    MgActorKill(lbl_1_bss_0.player[0]->actor);
    MgActorKill(lbl_1_bss_0.player[1]->actor);
    return 1;
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
