#include "REL/mgtourDll.h"
#include "math.h"
extern void fn_1_C76C(void);

MGT_PAPER lbl_1_bss_0[MGTOUR_PAPER_MAX];
MGT_KAMIWORK *lbl_1_bss_16760;
MGT_FLOW *lbl_1_bss_16818;
MGT_MODEWORK *lbl_1_bss_1681C;
MGT_FLOW lbl_1_bss_16820;

HUWINID fn_1_660(void)
{
    return lbl_1_bss_16818->winId;
}

u32 fn_1_7E4(u32 mask)
{
    return lbl_1_bss_16818->stat & mask;
}

u32 fn_1_83C(u32 mask)
{
    return lbl_1_bss_1681C->flag & mask;
}

u32 fn_1_88C(u32 mask)
{
    return lbl_1_bss_16818->flag & mask;
}

int fn_1_96C(void)
{
    return lbl_1_bss_16818->turnNo;
}

int fn_1_984(void)
{
    return lbl_1_bss_16818->charNo;
}

void fn_1_A0(s8* arr1, s8* arr2, s8 len)
{
    s8 i;
    s8 j;
    s8 temp1;
    s8 temp2;

    i = 0;
    if (len <= 1) {
        return;
    }
    while (i < len - 1) {
        j = i + 1;
        while (j < len) {
            if (arr1[i] > arr1[j]) {
                temp1 = arr1[i];
                arr1[i] = arr1[j];
                arr1[j] = temp1;
                if (arr2 != NULL) {
                    temp2 = arr2[i];
                    arr2[i] = arr2[j];
                    arr2[j] = temp2;
                }
            }
            j++;
        }
        i++;
    }
}

void fn_1_140(s8 *arr, int num)
{
    s8 usednum[256];
    int i, j;
    int no;
    for (i = num; i--;) {
        usednum[i] = arr[i];
    }
    for (i = num; i--;) {
        if (i) {
            no = frandmod(i + 1);
        } else {
            no = 0;
        }
        arr[i] = usednum[no];
        for (j = 0; j <= i; j++) {
            if (j != no && j > no) {
                usednum[j - 1] = usednum[j];
            }
        }
    }
}

void fn_1_204(void)
{
}

void fn_1_40C(void)
{
}

void fn_1_448(void)
{
}

void fn_1_49C(void)
{
}

void fn_1_4F0(void)
{
}

void fn_1_5CC(void)
{
}

void fn_1_674(void)
{
}

void fn_1_708(void)
{
}

void fn_1_750(void)
{
}

void fn_1_7AC(u32 mask)
{
    lbl_1_bss_16818->stat |= mask;
}

void fn_1_7C8(u32 mask)
{
    lbl_1_bss_16818->stat &= ~mask;
}

// NONMATCHING: 45.45%. Target keeps the global pointer in r4 across the or/clrlwi; MWCC reloads it (lis/addi/lwz) before the stb. Register allocation not controllable from C.
void fn_1_7FC(u8 mask)
{
    lbl_1_bss_1681C->flag = (u8)(lbl_1_bss_1681C->flag | mask);
}

// NONMATCHING: 45.45%. Target keeps the global pointer in r4 across the andc/clrlwi; MWCC reloads it before the stb. Register allocation not controllable from C.
void fn_1_81C(u8 mask)
{
    lbl_1_bss_1681C->flag = (u8)(lbl_1_bss_1681C->flag & ~mask);
}

void fn_1_854(u32 mask)
{
    lbl_1_bss_16818->flag |= mask;
}

void fn_1_870(u32 mask)
{
    lbl_1_bss_16818->flag &= ~mask;
}

void fn_1_8A4(void)
{
}

// NONMATCHING: 18.75%. Target reloads the global twice (r4 and r5) and uses lbzx/lhax; MWCC collapses to one register and reorders. Not controllable from C.
s16 fn_1_904(s32 arg)
{
    MGT_FLOW *f = lbl_1_bss_16818;
    s8 idx = (s8)*((u8 *)f + arg + 0x22A);
    return *(s16 *)((u8 *)f + idx * 0x30 + 0x16);
}

// NONMATCHING: 18.75%. Same register reallocation as fn_1_904 (target uses r4/r5, MWCC collapses).
s16 fn_1_938(s32 arg)
{
    MGT_FLOW *f = lbl_1_bss_16818;
    s8 idx = (s8)*((u8 *)f + arg + 0x22A);
    return *(s16 *)((u8 *)f + idx * 0x30 + 0x14);
}

// NONMATCHING: 33.33%. Target emits extsb after the lbz; MWCC omits it (char-return codegen). Not controllable from C.
s8 fn_1_99C(void)
{
    return lbl_1_bss_1681C->mgNum;
}

s8 fn_1_9B4(s32 arg)
{
    return (s8)*(s16 *)((u8 *)lbl_1_bss_16818 + arg * 0x30 + 0x14);
}

void fn_1_9D4(void)
{
}

void fn_1_A14(void)
{
}

void fn_1_A7C(void)
{
}

void fn_1_B98(void)
{
}

void fn_1_BEC(void)
{
}

void fn_1_C30(void)
{
}

void fn_1_D5C(void)
{
}

// NONMATCHING: 52.63%. Target keeps the current-process pointer in r7 (5th arg) with no frame; MWCC spills it to r31 + a frame. Register allocation not controllable from C.
void fn_1_DA4(void (*arg)(void))
{
    HUPROCESS *parent = HuPrcCurrentGet();
    HuPrcChildCreate(arg, 0x64, 0x1000, 0, parent);
    HuPrcVSleep();
}

void fn_1_DE4(s16 layerNo)
{
}

void fn_1_13D4(void)
{
}

void fn_1_1880(void)
{
}

void fn_1_18E0(void)
{
}

void fn_1_1938(void)
{
}

void fn_1_1C68(void)
{
}

void fn_1_1DE8(void)
{
}

void fn_1_1E1C(void)
{
}

void fn_1_1ED4(void)
{
}

void fn_1_2078(void)
{
}

void fn_1_242C(void)
{
}

void fn_1_25E4(void)
{
}

void fn_1_263C(void)
{
}

void fn_1_28C0(void)
{
}

void fn_1_2CE8(void)
{
}

void fn_1_2E68(void)
{
}

void fn_1_3B98(void)
{
}

void fn_1_3BF0(void)
{
}

void fn_1_3D2C(void)
{
}

static inline BOOL MgtAngleSub(float *angle, float end, float speed)
{
    float diff = *angle - end;
    float change = fmodf(diff, 180);
    float wrap = fmodf(diff, 360);
    if (fabsf(wrap) > 180.0f) {
        if (change > 0) {
            change = -(180.0f - fabsf(change));
        } else {
            change = 180.0f - fabsf(change);
        }
    }
    if (fabsf(change) < speed) {
        *angle = end;
        return TRUE;
    }
    if (change > 0) {
        change -= speed;
    } else {
        change += speed;
    }
    *angle = end + change;
    return FALSE;
}

void fn_1_3DE4(void)
{
    Vec dir;
    Vec aimPos;
    MGT_PLAYER *player;
    int i;
    int doneNum;
    float rotEnd;
    for (player = &lbl_1_bss_16818->player[0], i = 0; i < MGTOUR_PLAYER_MAX; i++, player++) {
        switch (lbl_1_bss_16818->mode) {
            case 0:
                player->rotX = 0;
                break;
            case 1:
                aimPos = lbl_1_bss_16818->shakePos;
                aimPos.z += 350;
                VECSubtract(&aimPos, &player->pos, &aimPos);
                aimPos.y = 0;
                VECNormalize(&aimPos, &dir);
                if (dir.z > 1) {
                    dir.z = 1;
                } else if (dir.z < -1) {
                    dir.z = -1;
                }
                rotEnd = acosf(dir.z);
                if (dir.x < 0) {
                    rotEnd = -rotEnd;
                }
                player->rotX = (180 * rotEnd) / M_PI;
                break;
        }
        if (!MgtAngleSub(&player->rotY, player->rotX, 0.01f)) {
            if (player->state != 1) {
                CharMotionShiftSet(player->charNo, player->motId[1], 0, 5, HU3D_MOTATTR_LOOP);
                player->state = 1;
            }
        }
    }
    while (1) {
        doneNum = 0;
        for (player = &lbl_1_bss_16818->player[0], i = MGTOUR_PLAYER_MAX; i--; player++) {
            if (MgtAngleSub(&player->rotY, player->rotX, lbl_1_bss_16818->rotSpeed)) {
                if (player->state != 0) {
                    CharMotionShiftSet(player->charNo, player->motId[0], 0, 5, HU3D_MOTATTR_LOOP);
                    player->state = 0;
                }
                doneNum++;
            }
            Hu3DModelRotSet(player->modelId, 0, player->rotY, 0);
        }
        if (doneNum == 4) {
            break;
        }
        HuPrcVSleep();
    }
    lbl_1_bss_16818->stat |= 0x1000;
    HuPrcEnd();
}

void fn_1_44E0(int mode, float speed)
{
    lbl_1_bss_16818->stat &= ~0x1000;
    lbl_1_bss_16818->mode = mode;
    lbl_1_bss_16818->rotSpeed = speed;
    HuPrcChildCreate(fn_1_3DE4, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
}

Vec lbl_1_data_28[12] = {
    { -160.0f, 0.0f, 160.0f },
    { 0.0f, 0.0f, 160.0f },
    { 160.0f, 0.0f, 160.0f },
    { 320.0f, 0.0f, 160.0f },
    { -180.0f, 31.0f, 420.0f },
    { 167.0f, 31.0f, 420.0f },
    { 0.0f, -300.0f, -7000.0f },
    { 0.0f, 100.0f, 0.0f },
    { -320.0f, 100.0f, 140.0f },
    { -420.0f, 100.0f, 140.0f },
    { -380.0f, 100.0f, 140.0f },
    { 20.0f, 100.0f, 140.0f }
};

struct MgtParCol {
    u8 c[4][4];
};

struct MgtParCol lbl_1_data_1E8 = {
    { { 0x80, 0x80, 0x80, 0xFF }, { 0x40, 0x40, 0x40, 0xFF }, { 0x00, 0x00, 0x00, 0x00 }, { 0x40, 0x40, 0x40, 0x80 } }
};

HU3DPARMANPARAM lbl_1_data_1F8 = {
    40, 0, 0.3f, 30.0f, 10.0f, { 0.0f, -0.2f, 0.0f }, 2.0f, 1.0f, 40.0f, 0.98f, 2,
    { { 0xFF, 0xFF, 0xFF, 0xFF }, { 0xFF, 0xFF, 0xFF, 0xFF }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 0xFF, 0xFF, 0xFF, 0x00 }, { 0xFF, 0xFF, 0xFF, 0x00 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};

static inline void MgtParColSet(HU3DPARMANPARAM *dst, struct MgtParCol *src)
{
    dst->colorStart[0].r = src->c[0][0];
    dst->colorStart[0].g = src->c[0][1];
    dst->colorStart[0].b = src->c[0][2];
    dst->colorStart[0].a = src->c[0][3];
    dst->colorStart[1].r = src->c[1][0];
    dst->colorStart[1].g = src->c[1][1];
    dst->colorStart[1].b = src->c[1][2];
    dst->colorStart[1].a = src->c[1][3];
    dst->colorEnd[0].r = src->c[2][0];
    dst->colorEnd[0].g = src->c[2][1];
    dst->colorEnd[0].b = src->c[2][2];
    dst->colorEnd[0].a = src->c[2][3];
    dst->colorEnd[1].r = src->c[3][0];
    dst->colorEnd[1].g = src->c[3][1];
    dst->colorEnd[1].b = src->c[3][2];
    dst->colorEnd[1].a = src->c[3][3];
}

void fn_1_456C(void)
{
    int angle;
    HU3DPARMANID parManId;
    HU3DMODELID mdlId;
    ANIMDATA *anim;
    s8 charNo;
    Vec basePos;
    Vec pos;
    float s;
    mdlId = lbl_1_bss_16818->mdl[4];
    angle = 0;
    basePos = lbl_1_data_28[lbl_1_bss_16818->turnNo + 4];
    charNo = lbl_1_bss_16818->charNo;
    basePos.y += 25.0f + (40.0f + CharModelHeightGet(charNo));
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x9A0023, 0x10000000, HEAP_MODEL));
    MgtParColSet(&lbl_1_data_1F8, &lbl_1_data_1E8);
    parManId = Hu3DParManCreate(anim, 100, &lbl_1_data_1F8);
    Hu3DParManAttrSet(parManId, 0x54);
    Hu3DParManLayerSet(parManId, 4);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(parManId), 1);
    while (lbl_1_bss_16818->stat & 0x2000) {
        s = sin(M_PI * angle / 180);
        pos = basePos;
        pos.y += 25.0f * s;
        Hu3DModelPosSetV(mdlId, &pos);
        Hu3DParManPosSet(parManId, pos.x, pos.y, pos.z);
        angle = (angle + 1) % 360;
        HuPrcVSleep();
    }
    Hu3DParManAttrSet(parManId, 1);
    HuPrcEnd();
}

void fn_1_485C(void)
{
    lbl_1_bss_16818->stat |= 0x2000;
    HuPrcChildCreate(fn_1_456C, 0x64, 0x1000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
}

MGT_MGENTRY lbl_1_data_B8[MGTOUR_MGTBL_MAX] = {
    { 0x37, 0x21D, 0 }, { 0x38, 0x21E, 0 }, { 0x39, 0x21F, 0 }, { 0x3A, 0x220, 0 }, { 0x3B, 0x221, 0 },
    { 0x3C, 0x222, 0 }, { 0x3D, 0x223, 0 }, { 0x3E, 0x224, 0 }, { 0x41, 0x227, 0 }, { 0x42, 0x228, 0 },
    { 0x3F, 0x225, 0 }, { 0x40, 0x226, 0 }, { 0x43, 0x229, 0 }, { 0x44, 0x22A, 0 }, { 0x45, 0x22B, 0 },
};

unsigned int lbl_1_data_0[10] = {
    0x008C0000, 0x008C0001, 0x008C0032, 0x008C0031, 0x008C0052, 0x008C0036, 0x00870099, 0x008C001A, 0x008C002E, 0
};

Vec lbl_1_data_114 = { 0, -100, -2000 };

extern void fn_1_C434(HU3DMODELID *mdl, MGT_FLOW *flow);

// remaining diff: stack slot order of shakePos/Vec locals and 2 extra callee-saved regs
void fn_1_48B4(void)
{
    OMOVLHIS *his;
    int i;
    MGT_PLAYER *player;
    s8 mgNo;
    s8 k;
    HU3DLIGHTID lightId;
    Vec shakePos;
    HuVec2f winSize;
    HU3DMODELID shakeMdlId;
    ANIMDATA *anim;

    his = omOvlHisGet(0);
    lbl_1_bss_1681C = (MGT_MODEWORK *)mgModeWork;
    lbl_1_bss_16818 = &lbl_1_bss_16820;
    {
        Vec shadowPos = { 0, 8000, 0 };
        Vec shadowUp = { 0, 0, 1 };
        Vec shadowTarget = { 0, 0, 0 };
        Vec altDown = { 0, -1, 0 };
        Vec tempPos = { 0, 50, 4000 };
        Vec altBack = { 0, 0, -1 };
        lbl_1_bss_16818->objman = omInitObjMan(200, 1000);
        HuDataSelHeapReadNum(DATA_instpic, 0x10000000, HEAP_MODEL);
        Hu3DCameraCreate(HU3D_CAM4);
        Hu3DCameraPerspectiveSet(HU3D_CAM4, 60, 20, 30000, 1.2f);
        Hu3DCameraViewportSet(HU3D_CAM4, 0, 0, 640, 480, 0, 1);
        Hu3DCameraScissorSet(HU3D_CAM4, 0, 0, 640, 480);
        lightId = Hu3DGLightCreate(0, 1000, 1000, 0, -1, -1, 255, 255, 255);
        Hu3DGLightStaticSet(lightId, 1);
        Hu3DGLightPosSetV(lightId, &shadowPos, &altDown);
        lightId = Hu3DGLightCreate(0, 1000, 1000, 0, -1, -1, 128, 128, 128);
        Hu3DGLightStaticSet(lightId, 1);
        Hu3DGLightPosSetV(lightId, &tempPos, &altBack);
    }
    lbl_1_bss_16818->mgTblNum = 0;
    for (i = 0; i < MGTOUR_MGTBL_MAX; i++) {
        for (mgNo = 0; mgDataTbl[mgNo].ovl != lbl_1_data_B8[i].mes1; mgNo++) {
            if (mgDataTbl[mgNo].ovl == (u16)DLL_NONE) {
                OSReport("mgtournament.c : ＤＬＬが見つかりません！！\n");
                abort();
            }
        }
        lbl_1_data_B8[i].mgIdx = mgNo;
        if (GWMgUnlockGet(mgNo + 0x1F5)) {
            MGT_MGENTRY *src = &lbl_1_data_B8[i];
            MGT_MGENTRY *dst = &lbl_1_bss_16818->mgTbl[lbl_1_bss_16818->mgTblNum];
            dst->mes1 = src->mes1;
            dst->mes2 = src->mes2;
            dst->mgIdx = src->mgIdx;
            lbl_1_bss_16818->mgTblNum++;
        }
    }
    Hu3DParManInit();
    lbl_1_bss_16818->winId2 = -1;
    lbl_1_bss_16818->winId = 0;
    lbl_1_bss_16818->flag = 0;
    lbl_1_bss_16818->stat = 0;
    lbl_1_bss_16818->turnNo = -1;
    lbl_1_bss_16818->charNo = -1;
    lbl_1_bss_16818->player1No = -1;
    lbl_1_bss_16818->player2No = -1;
    lbl_1_bss_16818->bgmId = -1;
    lbl_1_bss_16818->animId = -1;
    lbl_1_bss_16818->seId = -1;
    lbl_1_bss_16818->mode = 0;
    lbl_1_bss_16818->rotSpeed = 2;
    HuWinInit(1);
    HuWinMesMaxSizeBetGet(&winSize, 0x120000, 0x12000E);
    lbl_1_bss_16818->winId = HuWinExCreateFrame(-10000, 360, winSize.x, winSize.y, -1, 1);
    HuWinMesPalSet(lbl_1_bss_16818->winId, 7, 0, 0, 0);
    fn_1_13D4();
    if ((lbl_1_bss_16818->stat & 0x200) == 0) {
        Hu3DModelAttrSet(lbl_1_bss_16818->mdl[5], 1);
        Hu3DModelScaleSet(lbl_1_bss_16818->mdl[5], 3.8f, 3.8f, 1);
        Hu3DModelPosSet(lbl_1_bss_16818->mdl[5], lbl_1_data_114.x, lbl_1_data_114.y, lbl_1_data_114.z);
        Hu3DModelAttrSet(lbl_1_bss_16818->mdl[6], 1);
        Hu3DModelScaleSet(lbl_1_bss_16818->mdl[6], 3.8f, 3.8f, 1);
        Hu3DModelPosSet(lbl_1_bss_16818->mdl[6], lbl_1_data_114.x, lbl_1_data_114.y, lbl_1_data_114.z);
        lbl_1_bss_16818->stat |= 0x200;
        HuPrcChildCreate(fn_1_1938, 100, 4096, 0, HuPrcCurrentGet());
    }
    fn_1_C434(lbl_1_bss_16818->mdl, lbl_1_bss_16818);
    lbl_1_bss_16818->texId = HuSprAnimRead(HuDataSelHeapReadNum(DATA_mgtour | 0xF, 0x10000000, HEAP_MODEL));
    Hu3DLayerHookSet(0, fn_1_DE4);
    lbl_1_bss_16818->scrollX = 0;
    lbl_1_bss_16818->scrollY = 0;
    if (his->evtno == 0) {
        _SetFlag(FLAG_INST_NOHISCHG);
        _SetFlag(FLAG_INST_MGMODE);
        for (k = 4; k--;) {
            lbl_1_bss_1681C->playerNo[k] = k;
        }
        fn_1_140(lbl_1_bss_1681C->playerNo, 4);
    } else {
        Hu3DCameraPosSetV(HU3D_CAM4, &lbl_1_bss_1681C->camPos, &lbl_1_bss_1681C->camUp, &lbl_1_bss_1681C->camTarget);
        Hu3DCameraPerspectiveSet(HU3D_CAM4, lbl_1_bss_1681C->aspect, 20, 30000, 1.2f);
    }
    for (player = &lbl_1_bss_16818->player[0], i = 0; i < MGTOUR_PLAYER_MAX; i++, player++) {
        player->playerNo = i;
        player->charNo = GwPlayerConf[player->playerNo].charNo;
        player->charNo2 = GwPlayerConf[player->playerNo].padNo;
        player->modelId = CharModelMotListCreate(player->charNo, 2, lbl_1_data_0, player->motId);
        player->state = -1;
        player->pos = lbl_1_data_28[lbl_1_bss_1681C->playerNo[i]];
        player->rotY = 0;
        player->rotX = 0;
        CharMotionVoiceOnSet(player->charNo, lbl_1_data_0[2], FALSE);
        CharMotionVoiceOnSet(player->charNo, lbl_1_data_0[3], FALSE);
        CharMotionVoiceOnSet(player->charNo, lbl_1_data_0[4], FALSE);
        if (player->state != 0) {
            CharMotionShiftSet(player->charNo, player->motId[0], 0, 5, HU3D_MOTATTR_LOOP);
            player->state = 0;
        }
        Hu3DModelAttrSet(player->modelId, 1);
        Hu3DModelPosSet(player->modelId, player->pos.x, player->pos.y, player->pos.z);
        Hu3DModelLayerSet(player->modelId, 4);
        Hu3DModelCameraSet(player->modelId, HU3D_CAM4);
        lbl_1_bss_16818->mdl[i + 7] = player->modelId;
    }
    shakePos = lbl_1_data_28[7];
    shakeMdlId = Hu3DModelCreate(HuDataSelHeapReadNum(DATA_mgtour, 0x10000000, HEAP_MODEL));
    lbl_1_bss_16818->mdlShake = shakeMdlId;
    lbl_1_bss_16818->shakePos = shakePos;
    Hu3DMotionSet(shakeMdlId, Hu3DJointMotion(shakeMdlId, HuDataSelHeapReadNum(DATA_mgtour | 1, 0x10000000, HEAP_MODEL)));
    Hu3DModelAttrSet(shakeMdlId, HU3D_MOTATTR_LOOP);
    Hu3DModelLayerSet(shakeMdlId, 5);
    Hu3DModelCameraSet(shakeMdlId, HU3D_CAM4);
    Hu3DModelPosSet(shakeMdlId, shakePos.x, shakePos.y, shakePos.z);
    for (i = lbl_1_bss_16818->mgTblNum; i--;) {
        anim = HuSprAnimRead(HuDataSelHeapReadNum(mgDataTbl[lbl_1_bss_16818->mgTbl[i].mgIdx].instPic[0], 0x10000000, HEAP_MODEL));
        HuSprAnimLock(anim);
        lbl_1_bss_16818->thpAnim[i] = anim;
    }
    for (i = MGTOUR_MDL_MAX; i--;) {
        Hu3DCameraMotionOff(lbl_1_bss_16818->mdl[i]);
    }
    lbl_1_bss_16818->seId = HuAudFXPlay(0x39F);
}

void fn_1_5540(void)
{
    OMOVLHIS *his;
    if (lbl_1_bss_16818->bgmId >= 0) {
        HuAudSStreamFadeOut(lbl_1_bss_16818->bgmId, 1000);
        lbl_1_bss_16818->bgmId = -1;
    }
    HuDataDirClose(DATA_mgtour);
    HuDataDirClose(DATA_instpic);
    CharModelDataClose(-1);
    fn_1_C76C();
    HuTHPClose();
    GWMgInstFSet(TRUE);
    his = omOvlHisGet(0);
    omOvlHisChg(0, his->ovl, 0, his->stat);
    HuWinExClose(lbl_1_bss_16818->winId);
    HuWinExKill(lbl_1_bss_16818->winId);
    if (lbl_1_bss_16818->seId >= 0) {
        HuAudFXStop(lbl_1_bss_16818->seId);
        lbl_1_bss_16818->seId = -1;
    }
    WipeCreate(WIPE_MODE_OUT, 6, 60);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
    _ClearFlag(FLAG_INST_NOHISCHG);
    _ClearFlag(FLAG_INST_MGMODE);
    memset(lbl_1_bss_1681C, 0, sizeof(MGT_MODEWORK));
}

void fn_1_56C0(void)
{
}

void fn_1_6538(void)
{
}

void fn_1_7450(void)
{
}

void fn_1_7CB8(void)
{
}

void fn_1_9094(void)
{
}

void fn_1_A5D8(void)
{
}

void fn_1_A5DC(void)
{
}

void fn_1_A644(void)
{
}

void fn_1_A6BC(void)
{
}

void fn_1_A910(void)
{
}

void fn_1_A94C(void)
{
}

void fn_1_B318(void)
{
}

void fn_1_B678(void)
{
}

void ObjectSetup(void)
{
}