// mdcardDll/mdcard - decompilation of the mdcardDll/mdcard unit (46528 B, 39 fns).
// Done: _prolog, _epilog, ObjectSetup, MdMain, fn_1_9A0, LoadProc, MdInit, MdClose, fn_1_DD8,
// fn_1_942C, fn_1_A204. Still to write (address order): fn_1_1058, fn_1_1448, fn_1_1DF0, fn_1_22B8,
// fn_1_2A3C, fn_1_44F8..fn_1_4F80, fn_1_61F8..fn_1_78C4, fn_1_7DA0, fn_1_80A8, fn_1_84AC, fn_1_9100,
// fn_1_951C, fn_1_9714, fn_1_98C8, fn_1_9A70, fn_1_9CC8, fn_1_9E88, fn_1_A094, fn_1_A2BC, MdPosAdjustMode.

#include "REL/mdcardDll/mdcard.h"
#include "datadir_enum.h"
#include "game/hsfex.h"
#include "game/gamework.h"
#include "game/wipe.h"
#include "game/audio.h"
#include "dolphin/os.h"
#include "math.h"

typedef void (*VoidFunc)(void);

typedef struct MdPinInfo_s {
    HuVecF scrPos; //0x00
    HuVecF pos; //0x0C
    HuVecF rot; //0x18
    HuVecF fingerPos; //0x24
    HuVecF fingerRot; //0x30
} MDPIN_INFO;

typedef struct MdManSel_s {
    s8 cur; //0x00
    s8 next; //0x01
} MDMAN_SEL;

typedef struct MdlInfo_s {
    const char *name; //0x00
    s32 cameraNo; //0x04
} MDL_INFO;

typedef struct MdCamMove_s {
    HuVecF centerFrom; //0x00
    HuVecF hvrFrom; //0x0C
    HuVecF centerTo; //0x18
    HuVecF hvrTo; //0x24
} MDCAM_MOVE;

/* .bss: MWCC lays statics out in reverse declaration order, so the block is declared by descending address
   (the function-local statics of the original are file statics here) */
static MDPIN_INFO lbl_1_bss_2F0[7];
static MDPIN_INFO lbl_1_bss_200[4];
static s32 lbl_1_bss_1FC;
static s32 lbl_1_bss_1F8;
static MDCAM_MOVE lbl_1_bss_1C8;
static HuVecF lbl_1_bss_1B0[2];
static float lbl_1_bss_1AC;
static float lbl_1_bss_1A8;
static HuVecF lbl_1_bss_19C;
static HuVecF lbl_1_bss_16C[4];
static MDMAN_SEL lbl_1_bss_164[4];
static float lbl_1_bss_160;
static float lbl_1_bss_15C;
static HuVecF lbl_1_bss_12C[4];
static HuVecF lbl_1_bss_FC[4];
static HuVecF lbl_1_bss_F0;
static s8 lbl_1_bss_EC;
static HuVecF lbl_1_bss_D4[2];
static HuVecF lbl_1_bss_A4[4];
static HuVecF lbl_1_bss_8C[2];
static HuVecF lbl_1_bss_68[3];
static HuVecF lbl_1_bss_44[3];
static HuVecF lbl_1_bss_2C[2];
static s32 bgmStream;
static MDL_INFO *mdlInfoP;
static HuVec2f sprPos;
static float sprRot;
static s32 menuSel;
static s32 mdlSel;
static s32 dispTimer;
static s32 showF;
static s32 lbl_1_bss_4;
static s8 lbl_1_bss_2;
static s8 lbl_1_bss_1;
static s8 lbl_1_bss_0;


extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

void MdObjDebugFontCreate(OMOBJ *obj);
void MdObjPadMain(OMOBJ *obj);
void MdObjCameraCreate(OMOBJ *obj);
void MdObjLightCreate(OMOBJ *obj);
void MdObjShadowCreate(OMOBJ *obj);
void MdObjMessCreate(OMOBJ *obj);
void MdMathStdCurveInit(void);
void MdMessKill(MDMESS_WORK *mess);
s16 MdMessWinChoiceGet(MDMESS_WORK *mess, int winNo);
void MdSpriteCreate(MDSPRITE_WORK *sprite);
void MdModelCreate(MDMODEL_WORK *model, MDCAMERA_WORK *camera);
void MdToueiMdlInit(void);
Mtx *MdMathMatrixDir(HuVecF *start, HuVecF *end);
void MdMathMatrixRotRPY(Mtx *matrix, HuVecF *rot);
void MdMathMtxTrans(Mtx *src, Mtx *dst, HuVecF *trans);
void MdMathOscillate(int type, float *in, float *out, float t);
void MdMathLerp(float *out, float a, float b, float t);
void MdMathLerpScale(HuVecF *out, float a, float b, float t);
void MdMathLerpOfs(float *out, float a, float b, float t, float ofs);
void MdMathLerpScaleOfs(HuVecF *out, float a, float b, float t, float ofs);

void MdMessWinOpen(MDMESS_WORK *mess, int winNo, float posX, float posY, s16 w, s16 h, float bgTPLvl, s16 drawNo);
void MdMessWinClose(MDMESS_WORK *mess, int winNo);
void MdMessWinKill(MDMESS_WORK *mess, int winNo);
void MdMessWinPosSet(MDMESS_WORK *mess, int winNo, float posX, float posY);
void MdMessWinHomeClear(MDMESS_WORK *mess, int winNo);
void MdMessWinMesSet(MDMESS_WORK *mess, int winNo, u32 mesNo);
void MdMessWinInsertMesSet(MDMESS_WORK *mess, int winNo, u32 mesNo, u32 insertMesNo);
BOOL MdMessWinCheck(MDMESS_WORK *mess, int winNo);
s16 MdMessWinChoiceNowGet(MDMESS_WORK *mess, int winNo);
void MdMessWinChoiceNowSet(MDMESS_WORK *mess, int winNo, s16 choice);
void MdMessWinAttrSet(MDMESS_WORK *mess, int winNo, u32 attr);
void MdMessWinAttrReset(MDMESS_WORK *mess, int winNo, u32 attr);
#ifdef __MWERKS__
void MdMessHelpWinCreate(MDMESS_WORK *mess, int winNo, u32 mesNo, double posX, double posY, s16 w, s16 h, double bgTPLvl, s16 drawNo);
#else
void MdMessHelpWinCreate(MDMESS_WORK *mess, int winNo, u32 mesNo, float posX, float posY, s16 w, s16 h, float bgTPLvl, s16 drawNo);
#endif
void MdSpriteDispSet(MDSPRITE_WORK *sprite, int grpNo, BOOL dispF);
HuVec2f *MdSpritePosGet(MDSPRITE_WORK *sprite, int grpNo, int memberNo);
HuVec2f *MdSpriteScaleGet(MDSPRITE_WORK *sprite, int grpNo, int memberNo);

void ObjectSetup(void);
void MdMain(void);
void MdInit(void);
void MdClose(void);
void fn_1_9A0(void);
void LoadProc(void);
void fn_1_DD8(void);
void fn_1_1058(void);
void fn_1_1448(void);
void fn_1_1DF0(void);
void fn_1_22B8(void);
void fn_1_2A3C(void);
s32 fn_1_44F8(void);
s32 fn_1_461C(void);
s32 fn_1_4944(s32 arg0);
s32 fn_1_4C00(s32 arg0);
s32 fn_1_4DC8(s32 arg0);
s32 fn_1_4F80(s32 mode);
s32 fn_1_61F8(s32 mode);
s32 fn_1_6B0C(s32 mode);
s32 fn_1_71B8(s32 mode);
s32 fn_1_78C4(s32 mode);
void fn_1_7DA0(void);
void fn_1_84AC(void);
void fn_1_9100(void);
void fn_1_98C8(void);
void fn_1_942C(s32 modelNo, s32 cameraNo);
void fn_1_951C(void);
void fn_1_9714(void);
void fn_1_9CC8(s32 dir);
s32 fn_1_9A70(void);
void fn_1_9E88(void);
void fn_1_80A8(void);
void fn_1_A2BC(s32 arg0, s32 arg1);
void MdPosAdjustMode(void);
void MdMathAngleClamp(float *angle);
void MdDebugFontPrint(MDDEBUGFONT_WORK *debugFont, const char *str, ...);
void fn_1_A094(s32 no);
s32 fn_1_A204(s32 modelNo, s32 cameraNo);

static float lbl_1_data_0[4] = { 0.64f, 4000.0f, 80.0f, 2500.0f };

static HuVecF lbl_1_data_10[2] = {
    { 310.0f, 290.0f, 2500.0f },
    { -70.0f, 200.0f, 150.0f }
};

static HuVecF lbl_1_data_28[7] = {
    { -65.0f, 134.5f, -371.0f },
    { 103.5f, 134.5f, -371.0f },
    { 272.0f, 134.5f, -371.0f },
    { -151.0f, 110.5f, -115.0f },
    { 18.5f, 134.5f, -115.0f },
    { 190.5f, 134.5f, -115.0f },
    { 362.0f, 135.0f, -115.0f }
};

static HuVecF lbl_1_data_7C = { -90.0f, 0.0f, 0.0f };

void (*MdFlowTbl[6])(void) = {
    fn_1_DD8, fn_1_1058, fn_1_1448, fn_1_1DF0, fn_1_22B8, fn_1_2A3C
};

s32 (*MdProcTbl[10])() = {
    fn_1_44F8, fn_1_461C, fn_1_4944, fn_1_4C00, fn_1_4DC8, fn_1_4F80, fn_1_61F8, fn_1_6B0C, fn_1_71B8, fn_1_78C4
};

SYS sysWork;
SYS *sys;
SYS *sysP;

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

void ObjectSetup(void)
{
    sys = &sysWork;
    sysP = sys;
    sys->step = 0;
    sys->subStep = 0;
    sys->unk8 = 0;
    sys->counter = 0;
    sys->flowMode = 0;
    sys->ovlMode = 0;
    sys->ovlNo = 0x70;
    sys->exitF = 0;
    sys->skipWipeF = 0;
    sys->objman = omInitObjMan(512, 8192);
    omGameSysInit(sys->objman);
    sys->obj[MDCARD_OBJ_DEBUGFONT] = omAddObjEx(sys->objman, 100, 0, 0, -1, MdObjDebugFontCreate);
    sys->obj[MDCARD_OBJ_DEBUGFONT]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDDEBUGFONT_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDCARD_OBJ_DEBUGFONT]->data) {
        OSReport("sys->obj[MDCARD_OBJ_DEBUGFONT]->data = NULL...\n");
    }
    sys->debugFont = sys->obj[MDCARD_OBJ_DEBUGFONT]->data;
    sys->debugFont->createF = FALSE;
    sys->obj[MDCARD_OBJ_PAD] = omAddObjEx(sys->objman, 101, 0, 0, -1, MdObjPadMain);
    sys->obj[MDCARD_OBJ_PAD]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDPAD_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDCARD_OBJ_PAD]->data) {
        OSReport("sys->obj[MDCARD_OBJ_PAD]->data = NULL...\n");
    }
    sys->padWork = sys->obj[MDCARD_OBJ_PAD]->data;
    sys->obj[MDCARD_OBJ_CAMERA] = omAddObjEx(sys->objman, 102, 0, 0, -1, MdObjCameraCreate);
    sys->obj[MDCARD_OBJ_CAMERA]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDCAMERA_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDCARD_OBJ_CAMERA]->data) {
        OSReport("sys->obj[MDCARD_OBJ_CAMERA]->data = NULL...\n");
    }
    sys->camera = sys->obj[MDCARD_OBJ_CAMERA]->data;
    sys->camera->createF = FALSE;
    sys->camera->debugFont = sys->debugFont;
    sys->obj[MDCARD_OBJ_LIGHT] = omAddObjEx(sys->objman, 103, 0, 0, -1, MdObjLightCreate);
    sys->obj[MDCARD_OBJ_LIGHT]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDLIGHT_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDCARD_OBJ_LIGHT]->data) {
        OSReport("sys->obj[MDCARD_OBJ_LIGHT]->data = NULL...\n");
    }
    sys->light = sys->obj[MDCARD_OBJ_LIGHT]->data;
    sys->light->createF = FALSE;
    sys->light->debugFont = sys->debugFont;
    sys->obj[MDCARD_OBJ_SHADOW] = omAddObjEx(sys->objman, 104, 0, 0, -1, MdObjShadowCreate);
    sys->obj[MDCARD_OBJ_SHADOW]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDSHADOW_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDCARD_OBJ_SHADOW]->data) {
        OSReport("sys->obj[MDCARD_OBJ_SHADOW]->data = NULL...\n");
    }
    sys->shadow = sys->obj[MDCARD_OBJ_SHADOW]->data;
    sys->shadow->createF = FALSE;
    sys->shadow->debugFont = sys->debugFont;
    sys->obj[MDCARD_OBJ_MESS] = omAddObjEx(sys->objman, 105, 0, 0, -1, MdObjMessCreate);
    sys->obj[MDCARD_OBJ_MESS]->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDMESS_WORK), HU_MEMNUM_OVL);
    if(!sys->obj[MDCARD_OBJ_MESS]->data) {
        OSReport("sys->obj[MDCARD_OBJ_MESS]->data = NULL...\n");
    }
    sys->mess = sys->obj[MDCARD_OBJ_MESS]->data;
    sys->mess->createF = FALSE;
    sys->sprite = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDSPRITE_WORK), HU_MEMNUM_OVL);
    if(!sys->sprite) {
        OSReport("sys->sprite = NULL...\n");
    }
    sys->model = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MDMODEL_WORK), HU_MEMNUM_OVL);
    if(!sys->model) {
        OSReport("sys->model = NULL...\n");
    }
    MdMathStdCurveInit();
    Hu3DBGColorSet(0, 0, 0);
    Hu3DNoSyncSet(0);
    while(1) {
        if(sys->debugFont->createF == TRUE && sys->camera->createF == TRUE && sys->light->createF == TRUE
            && sys->shadow->createF == TRUE && sys->mess->createF == TRUE) {
            break;
        }
        HuPrcVSleep();
    }
    sys->mainProc = HuPrcChildCreate(MdMain, 107, 12288, 0, sys->objman);
    sys->loadState = 0;
    sys->loadAsyncStat = 0;
    sys->loadDirNo = DATA_mdsel;
    sys->loadProc = HuPrcChildCreate(LoadProc, 110, 12288, 0, sys->objman);
}

void MdMain(void)
{
    MdInit();
    while (omSysExitReq == 0 && sys->exitF == 0) {
        MdFlowTbl[sys->flowMode]();
        fn_1_84AC();
        if (++sys->counter > 36000) {
            sys->counter = 0;
        }
        HuPrcVSleep();
    }
    MdClose();
}

void fn_1_9A0(void)
{
    MDMESS_WORK *win;
    MDCARD_MSG *msg;

    win = sys->mess;
    msg = &sys->msg;
    while (1) {
        win = sys->mess;
        msg = &sys->msg;
        switch (msg->mode) {
        case 0:
            break;
        case 1:
            msg->choice = MdMessWinChoiceGet(win, 1);
            msg->mode = 2;
            break;
        case 2:
            break;
        }
        HuPrcVSleep();
    }
}

void LoadProc(void)
{
    OSTick tick;

    while(1) {
        switch(sys->loadState) {
            case 0:
                break;

            case 1:
                tick = OSGetTick();
                sys->loadAsyncStat = HuDataDirReadAsync(sys->loadDirNo);
                while(!HuDataGetAsyncStat(sys->loadAsyncStat)) {
                    HuPrcVSleep();
                }
                OSReport("Load Time:%d\n", OSTicksToMilliseconds(OSGetTick()-tick));
                OSReport("Finished!\n");
                sys->loadState = 2;
                break;

            case 2:
                HuPrcEnd();
                break;
        }
        HuPrcVSleep();
    }
}

void MdInit(void)
{
    MDCARD_SLOT *slot;
    MDCARD_CELL *cell;
    int i;
    int j;

    MdSpriteCreate(sys->sprite);
    HuPrcVSleep();
    MdModelCreate(sys->model, sys->camera);
    HuPrcVSleep();
    HuDataDirClose(DATA_mdcard);
    HuPrcVSleep();
    MdToueiMdlInit();
    sys->finger.state = 0;
    sys->finger.cmd = 1;
    sys->unk5C = HuPrcChildCreate(fn_1_7DA0, 109, 12288, 0, sys->objman);
    for(cell=&sys->unk33C[0], i=0; i<4; i++, cell++) {
        cell->state = 0;
    }
    sys->unk58 = HuPrcChildCreate(fn_1_9100, 108, 4096, 0, sys->objman);
    for(slot=&sys->unk9C[0], j=0; j<7; j++, slot++) {
        slot->state = 0;
        slot->val = 0;
    }
    HuPrcVSleep();
}

void MdClose(void)
{
    if(sys->skipWipeF == 0) {
        WipeCreate(2, 0, 60);
        while(WipeCheckEnd()) {
            HuPrcVSleep();
        }
    }
    fn_1_A094(13);
    MdMessKill(sys->mess);
    if(sys->ovlMode == 0) {
        omOvlGotoEx(sys->ovlNo, 1, 0, 0);
    } else if(sys->ovlMode == 1) {
        omOvlCallEx(sys->ovlNo, 1, 0, 0);
    } else if(sys->ovlMode == 2) {
        omOvlReturnEx(1, 1);
    }
    HuPrcEnd();
    while(1) {
        HuPrcVSleep();
    }
}

void fn_1_DD8(void)
{
    int i;
    MDMODEL_WORK *model;
    MDSPRITE_WORK *sprite;
    MDMESS_WORK *mess;
    MDCARD_MSG *msg;

    sprite = sys->sprite;
    model = sys->model;
    mess = sys->mess;
    for(i=0; i<4; i++) {
        sys->unk3B[i] = 0;
    }
    MdSpriteDispSet(sprite, 0, FALSE);
    MdSpriteDispSet(sprite, 1, FALSE);
    MdSpriteDispSet(sprite, 3, FALSE);
    Hu3DModelAttrReset(model->mdlId[0], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(model->mdlId[1], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(model->mdlId[2], HU3D_ATTR_DISPOFF);
    for(i=0; i<7; i++) {
        Hu3DModelAttrReset(model->mdlId[i*3+3], HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(model->mdlId[i*3+4], HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(model->mdlId[i*3+5], HU3D_ATTR_DISPOFF);
    }
    for(i=0; i<8; i++) {
        Hu3DModelAttrSet(model->mdlId[i+24], HU3D_ATTR_DISPOFF);
    }
    Hu3DModelAttrSet(model->mdlId[32], HU3D_ATTR_DISPOFF);
    for(i=0; i<5; i++) {
        Hu3DModelAttrSet(model->mdlId[i+33], HU3D_ATTR_DISPOFF);
    }
    fn_1_98C8();
    MdProcTbl[0](0);
    msg = &sys->msg;
    msg->mode = 0;
    sys->selMode = 0;
    sys->curSel = 3;
    WipeColorSet(255, 255, 255);
    WipeCreate(1, 0, 30);
    fn_1_A094(0);
    while(WipeCheckEnd()) {
        HuPrcVSleep();
    }
    sys->step = 0;
    sys->subStep = 0;
    sys->flowMode = 1;
}

// remaining diff: WipeCheckEnd result kept in r0 instead of r30
void fn_1_1058(void)
{
    MDMESS_WORK *mess;
    int stat;
    MDMODEL_WORK *model;

    model = sys->model;
    mess = sys->mess;
    switch(sys->step) {
        case 0:
            MdProcTbl[0](0);
            sys->step = 6;
            break;

        case 6:
            stat = MdProcTbl[1](0);
            if(stat == 1) {
                MdProcTbl[0](0);
                sys->step = 8;
            }
            break;

        case 8:
            MdMessWinOpen(mess, 2, 80.0f, 376.0f, 0x1A0, 0x40, 1.0f, 0);
            MdMessWinMesSet(mess, 2, 0x150000);
            fn_1_A094(11);
            sys->finger.cmd = 2;
            sys->step = 1;
            break;

        case 1:
            stat = MdMessWinCheck(mess, 2);
            if(stat != 0) {
                MdMessWinHomeClear(mess, 2);
                MdMessWinMesSet(mess, 2, 0x150001);
                sys->step = 2;
            }
            break;

        case 2:
            stat = MdMessWinChoiceGet(mess, 2);
            if(stat == -1) {
                MdMessWinHomeClear(mess, 2);
                MdMessWinMesSet(mess, 2, 0x150002);
                sys->step = 3;
            } else if(stat == 0) {
                MdMessWinKill(mess, 2);
                sys->step = 0;
                sys->flowMode = 2;
            } else if(stat == 1) {
                sys->step = 0;
                sys->flowMode = 3;
            }
            break;

        case 3:
            stat = MdMessWinChoiceGet(mess, 2);
            if(stat == -1 || stat == 1) {
                MdMessWinHomeClear(mess, 2);
                MdMessWinMesSet(mess, 2, 0x150001);
                sys->step = 2;
            } else if(stat == 0) {
                fn_1_A094(1);
                WipeColorSet(255, 255, 255);
                WipeCreate(2, 0, 30);
                sys->loadState = 1;
                sys->loadDirNo = DATA_mdomake;
                sys->step = 4;
            }
            break;

        case 4:
            if(WipeCheckEnd() == 0) {
                sys->step = 5;
            }
            break;

        case 5:
            if(sys->loadState == 2) {
                sys->ovlMode = 2;
                sys->ovlNo = 0x5E;
                sys->skipWipeF = 1;
                sys->exitF = 1;
            }
            break;
    }
}

// remaining diff: r30/r31 swap between mess and stat (register allocation)
void fn_1_1448(void)
{
    MDMODEL_WORK *model;
    int i;
    int stat;
    MDMESS_WORK *mess;

    model = sys->model;
    mess = sys->mess;
    if((HuPadBtnDown[0] & 0x200) != 0 && (sys->step != 0 || sys->step != 0x16)) {
        sys->step = 0x16;
    }
    switch(sys->step) {
        case 0:
            MdProcTbl[0](0);
            MdMessWinOpen(mess, 1, 64.0f, 160.0f, 0x1C0, 0x60, 1.0f, 0x40);
            MdMessWinMesSet(mess, 1, 0x140000);
            sys->step = 1;
            break;

        case 1:
            stat = MdMessWinCheck(mess, 1);
            if(stat != 0) {
                MdMessWinHomeClear(mess, 1);
                MdMessWinMesSet(mess, 1, 0x140001);
                sys->step = 2;
            }
            break;

        case 2:
            stat = MdMessWinCheck(mess, 1);
            if(stat != 0) {
                MdMessWinHomeClear(mess, 1);
                MdMessWinMesSet(mess, 1, 0x140002);
                sys->step = 3;
            }
            break;

        case 3:
            stat = MdMessWinCheck(mess, 1);
            if(stat != 0) {
                sys->step = 4;
            }
            break;

        case 4:
            stat = MdProcTbl[2](0x21);
            if(stat == 1) {
                MdProcTbl[0](0);
                MdMessWinMesSet(mess, 1, 0x140003);
                sys->step = 5;
            }
            break;

        case 5:
            stat = MdMessWinCheck(mess, 1);
            if(stat != 0) {
                MdMessWinHomeClear(mess, 1);
                sys->step = 6;
            }
            break;

        case 6:
            stat = MdProcTbl[4](0x21);
            if(stat == 1) {
                MdProcTbl[0](0);
                sys->step = 7;
            }
            break;

        case 7:
            stat = MdProcTbl[2](0x22);
            if(stat == 1) {
                MdProcTbl[0](0);
                MdMessWinHomeClear(mess, 1);
                MdMessWinMesSet(mess, 1, 0x140004);
                sys->step = 8;
            }
            break;

        case 8:
            stat = MdMessWinCheck(mess, 1);
            if(stat != 0) {
                MdMessWinHomeClear(mess, 1);
                sys->step = 9;
            }
            break;

        case 9:
            stat = MdProcTbl[4](0x22);
            if(stat == 1) {
                MdProcTbl[0](0);
                sys->step = 10;
            }
            break;

        case 10:
            stat = MdProcTbl[2](0x23);
            if(stat == 1) {
                MdProcTbl[0](0);
                MdMessWinHomeClear(mess, 1);
                MdMessWinMesSet(mess, 1, 0x140005);
                sys->step = 11;
            }
            break;

        case 11:
            stat = MdMessWinCheck(mess, 1);
            if(stat != 0) {
                MdMessWinHomeClear(mess, 1);
                sys->step = 12;
            }
            break;

        case 12:
            stat = MdProcTbl[4](0x23);
            if(stat == 1) {
                MdProcTbl[0](0);
                sys->step = 13;
            }
            break;

        case 13:
            stat = MdProcTbl[2](0x24);
            if(stat == 1) {
                MdProcTbl[0](0);
                MdMessWinHomeClear(mess, 1);
                MdMessWinMesSet(mess, 1, 0x140009);
                sys->step = 14;
            }
            break;

        case 14:
            stat = MdMessWinCheck(mess, 1);
            if(stat != 0) {
                sys->step = 15;
            }
            break;

        case 15:
            stat = MdProcTbl[3](0x24);
            if(stat == 1) {
                MdProcTbl[0](0);
                MdMessWinHomeClear(mess, 1);
                MdMessWinMesSet(mess, 1, 0x14000A);
                sys->step = 16;
            }
            break;

        case 16:
            stat = MdMessWinCheck(mess, 1);
            if(stat != 0) {
                MdMessWinHomeClear(mess, 1);
                sys->step = 17;
            }
            break;

        case 17:
            stat = MdProcTbl[4](0x24);
            if(stat == 1) {
                MdProcTbl[0](0);
                sys->step = 18;
            }
            break;

        case 18:
            stat = MdProcTbl[2](0x25);
            if(stat == 1) {
                MdProcTbl[0](0);
                MdMessWinHomeClear(mess, 1);
                MdMessWinMesSet(mess, 1, 0x140007);
                sys->step = 19;
            }
            break;

        case 19:
            stat = MdMessWinCheck(mess, 1);
            if(stat != 0) {
                MdMessWinHomeClear(mess, 1);
                sys->step = 20;
            }
            break;

        case 20:
            stat = MdProcTbl[4](0x25);
            if(stat == 1) {
                MdProcTbl[0](0);
                MdMessWinHomeClear(mess, 1);
                MdMessWinMesSet(mess, 1, 0x140008);
                sys->step = 21;
            }
            break;

        case 21:
            stat = MdMessWinCheck(mess, 1);
            if(stat != 0) {
                MdMessWinKill(mess, 1);
                MdMessWinOpen(mess, 2, 80.0f, 376.0f, 0x1A0, 0x40, 1.0f, 0);
                sys->step = 0;
                sys->flowMode = 3;
            }
            break;

        case 22:
            WipeColorSet(0, 0, 0);
            WipeCreate(2, 0, 30);
            while(WipeCheckEnd()) {
                HuPrcVSleep();
            }
            for(i=0; i<5; i++) {
                Hu3DModelAttrSet(model->mdlId[i+33], HU3D_ATTR_DISPOFF);
            }
            MdMessWinClose(mess, 1);
            WipeColorSet(0, 0, 0);
            WipeCreate(1, 0, 30);
            while(WipeCheckEnd()) {
                HuPrcVSleep();
            }
            MdMessWinOpen(mess, 2, 80.0f, 376.0f, 0x1A0, 0x40, 1.0f, 0);
            MdMessWinMesSet(mess, 2, 0x150001);
            sys->step = 2;
            sys->flowMode = 1;
            break;
    }
}

void fn_1_1DF0(void)
{
    MDMODEL_WORK *model;
    int stat;
    MDMESS_WORK *mess;
    MDCARD_MSG *msg;

    model = sys->model;
    mess = sys->mess;
    msg = &sys->msg;
    switch(sys->step) {
        case 0:
            sys->selMode = 0;
            MdProcTbl[0](0);
            MdMessWinHomeClear(mess, 2);
            MdMessWinMesSet(mess, 2, 0x150003);
            fn_1_A094(12);
            Hu3DModelAttrReset(model->mdlId[2], HU3D_MOTATTR_REV);
            Hu3DModelAttrReset(model->mdlId[2], HU3D_MOTATTR_PAUSE);
            Hu3DMotionSpeedSet(model->mdlId[2], 1.0f);
            fn_1_A094(9);
            sys->step = 1;
            break;

        case 1:
            stat = MdMessWinCheck(mess, 2);
            if(stat != 0) {
                MdMessWinOpen(mess, 1, 48.0f, 64.0f, 0x1E0, 0xB0, 1.0f, 0x40);
                sys->step = 2;
            }
            break;

        case 2:
            MdProcTbl[0](0);
            MdMessHelpWinCreate(mess, 0, 0x20004, 0.0, 340.0, 0x200, 0x20, 0.0, 0);
            MdMessWinAttrSet(mess, 1, HUWIN_ATTR_ALIGN_CENTER);
            MdMessWinMesSet(mess, 1, 0x150004);
            MdMessWinChoiceNowSet(mess, 1, 0);
            lbl_1_bss_1FC = 0;
            MdMessWinHomeClear(mess, 2);
            MdMessWinMesSet(mess, 2, 0x150005);
            sys->unk50 = HuPrcChildCreate(fn_1_9A0, 106, 0x1000, 0, sys->objman);
            msg->mode = 1;
            sys->step = 3;
            break;

        case 3:
            lbl_1_bss_1F8 = MdMessWinChoiceNowGet(mess, 1);
            if(lbl_1_bss_1FC != lbl_1_bss_1F8 && lbl_1_bss_1F8 >= 0 && lbl_1_bss_1F8 <= 3) {
                MdMessWinHomeClear(mess, 2);
                MdMessWinMesSet(mess, 2, 0x150005 + lbl_1_bss_1F8);
            }
            lbl_1_bss_1FC = lbl_1_bss_1F8;
            if(msg->mode == 2) {
                HuPrcKill(sys->unk50);
                sys->step = 4;
            }
            break;

        case 4:
            stat = MdMessWinCheck(mess, 1);
            if(stat != 0) {
                if(msg->choice == -1) {
                    MdMessWinClose(mess, 0);
                    MdMessWinAttrReset(mess, 1, HUWIN_ATTR_ALIGN_CENTER);
                    MdMessWinKill(mess, 1);
                    MdMessWinHomeClear(mess, 2);
                    MdMessWinMesSet(mess, 2, 0x150001);
                    Hu3DModelAttrReset(model->mdlId[2], HU3D_MOTATTR_PAUSE);
                    Hu3DModelAttrSet(model->mdlId[2], HU3D_MOTATTR_REV);
                    Hu3DMotionSpeedSet(model->mdlId[2], 2.0f);
                    fn_1_A094(10);
                    sys->step = 2;
                    sys->flowMode = 1;
                } else {
                    sys->selMode = msg->choice;
                    sys->step = 0;
                    sys->flowMode = 4;
                }
            }
            break;
    }
}

// remaining diff: sys@ha/jumptable temp register allocation (r3/r4/r30)
void fn_1_22B8(void)
{
    MDMODEL_WORK *model;
    MDMESS_WORK *mess;
    int stat;
    MDCARD_MSG *msg;

    model = sys->model;
    mess = sys->mess;
    msg = &sys->msg;
    switch(sys->step) {
        case 0:
            MdProcTbl[0](0);
            fn_1_951C();
            sys->prevSel = sys->curSel;
            MdMessWinHomeClear(mess, 1);
            MdMessWinMesSet(mess, 1, 0x150009);
            sys->step = 1;
            break;

        case 1:
            stat = MdProcTbl[5](0);
            if(stat == 1) {
                MdProcTbl[0](0);
                MdMessWinHomeClear(mess, 2);
                MdMessWinMesSet(mess, 2, 0x15000D - sys->curSel);
                if(sys->selNum == 1) {
                    sys->step = 3;
                } else {
                    sys->step = 2;
                }
            }
            break;

        case 2:
            stat = MdProcTbl[6](0);
            if(stat == 1) {
                MdProcTbl[0](0);
                sys->step = 3;
            }
            break;

        case 3:
            if((sys->padWork->DStkDown[0] & 1) != 0 && sys->selNum > 1) {
                sys->prevSel = sys->curSel;
                sys->curSel--;
                if(sys->curSel == -1) {
                    sys->curSel = sys->selNum-1;
                }
                MdMessWinHomeClear(mess, 2);
                MdMessWinMesSet(mess, 2, 0x15000D - sys->curSel);
                fn_1_A094(2);
                sys->step = 6;
            } else if((sys->padWork->DStkDown[0] & 2) != 0 && sys->selNum > 1) {
                sys->prevSel = sys->curSel;
                sys->curSel++;
                sys->curSel = sys->curSel % sys->selNum;
                MdMessWinHomeClear(mess, 2);
                MdMessWinMesSet(mess, 2, 0x15000D - sys->curSel);
                fn_1_A094(2);
                sys->step = 7;
            } else if((HuPadBtnDown[0] & 0x200) != 0) {
                fn_1_A094(5);
                if(sys->selNum == 1) {
                    sys->step = 5;
                } else {
                    sys->step = 4;
                }
            } else if((HuPadBtnDown[0] & 0x100) != 0) {
                fn_1_9714();
                fn_1_A094(3);
                if(sys->selNum == 1) {
                    sys->step = 9;
                } else {
                    sys->step = 8;
                }
            }
            break;

        case 4:
            stat = MdProcTbl[6](1);
            if(stat == 1) {
                MdProcTbl[0](0);
                sys->step = 5;
            }
            break;

        case 5:
            stat = MdProcTbl[5](1);
            if(stat == 1) {
                MdProcTbl[0](0);
                MdMessWinAttrSet(mess, 1, HUWIN_ATTR_ALIGN_CENTER);
                MdMessWinMesSet(mess, 1, 0x150004);
                MdMessWinChoiceNowSet(mess, 1, 0);
                lbl_1_bss_1FC = 0;
                MdMessWinHomeClear(mess, 2);
                MdMessWinMesSet(mess, 2, 0x150005);
                sys->unk50 = HuPrcChildCreate(fn_1_9A0, 106, 0x1000, 0, sys->objman);
                msg->mode = 1;
                sys->step = 3;
                sys->flowMode = 3;
            }
            break;

        case 6:
        case 7:
            stat = MdProcTbl[5](sys->step-4);
            if(stat == 1) {
                MdProcTbl[0](0);
                sys->step = 3;
            }
            break;

        case 8:
            stat = MdProcTbl[6](1);
            if(stat == 1) {
                MdProcTbl[0](0);
                sys->step = 9;
            }
            break;

        case 9:
            stat = MdProcTbl[7](0);
            if(stat == 1) {
                MdProcTbl[0](0);
                sys->step = 0;
                sys->flowMode = 5;
            }
            break;
    }
}

// remaining diff: temp register allocation / operand order of index math (logic complete)
void fn_1_2A3C(void)
{
    int i;
    MDMODEL_WORK *model;
    MDCARD_SLOT *slot;
    MDMESS_WORK *mess;
    int stat;
    int mdlIdx;
    MDCARD_CELL *cell;
    int j;
    int motIdx;
    s32 *state;
    MDSPRITE_WORK *sprite;

    model = sys->model;
    sprite = sys->sprite;
    mess = sys->mess;
    state = &sys->finger.state;
    switch(sys->step) {
        case 0:
            sys->turn = 0;
            sys->plSlot[sys->turn] = 0;
            sys->plSlotPrev[sys->turn] = 0;
            MdProcTbl[0](0);
            MdMessWinHomeClear(mess, 2);
            MdMessWinInsertMesSet(mess, 2, 0x15000E, sys->plChr[sys->turn] + 0x15001C);
            sys->step = 1;
            break;

        case 1:
            stat = MdProcTbl[8](0);
            if(stat == 1) {
                MdProcTbl[0](0);
                fn_1_80A8();
                sys->step = 2;
            }
            break;

        case 2:
            stat = MdProcTbl[9](0);
            if(stat == 1) {
                MdProcTbl[0](0);
                lbl_1_bss_0 = 1;
                slot = &sys->unk9C[sys->plSlot[sys->turn]];
                slot->state = 1;
                sys->step = 3;
            }
            break;

        case 3:
            stat = fn_1_9A70();
            if(stat == 0 || stat == 1) {
                slot = &sys->unk9C[sys->plSlotPrev[sys->turn]];
                slot->state = 4;
                fn_1_9CC8(stat);
                sys->plSlotPrev[sys->turn] = sys->plSlot[sys->turn];
                slot = &sys->unk9C[sys->plSlot[sys->turn]];
                slot->state = 1;
                fn_1_A094(2);
                MdProcTbl[9](2);
            } else if(stat == 2) {
                fn_1_A094(5);
                if(sys->turn == 0) {
                    slot = &sys->unk9C[sys->plSlot[sys->turn]];
                    slot->state = 4;
                    sys->step = 4;
                } else {
                    slot = &sys->unk9C[sys->plSlot[sys->turn]];
                    slot->state = 4;
                    for(i=0; i<3; i++) {
                        Hu3DMotionShiftSet(model->mdlId[sys->plSlot[sys->turn-1]*3+i+3], model->motId[sys->plSlot[sys->turn-1]*3+i+23], 0.0f, 56.0f, 0);
                    }
                    cell = &sys->unk33C[sys->turn-1];
                    cell->state = 4;
                    sys->step = 8;
                }
            } else if(stat == 3) {
                sys->unk3B[sys->turn] = stat-3;
                slot = &sys->unk9C[sys->plSlot[sys->turn]];
                slot->state = 7;
                slot->val = sys->unk3B[sys->turn];
                if(sys->turn < 3) {
                    fn_1_A094(4);
                } else {
                    fn_1_A094(3);
                }
                for(i=0; i<3; i++) {
                    mdlIdx = sys->plSlot[sys->turn]*3+i+3;
                    motIdx = sys->plSlot[sys->turn]*3+i+23;
                    Hu3DMotionSet(model->mdlId[mdlIdx], model->motId[motIdx]);
                    Hu3DMotionTimeSet(model->mdlId[mdlIdx], 0.0f);
                    Hu3DModelAttrReset(model->mdlId[mdlIdx], HU3D_MOTATTR_LOOP);
                    Hu3DModelAttrSet(model->mdlId[mdlIdx], HU3D_MOTATTR_PAUSE);
                    if(i == sys->unk3B[sys->turn]) {
                        Hu3DModelAttrReset(model->mdlId[mdlIdx], HU3D_ATTR_DISPOFF);
                    } else {
                        Hu3DModelAttrSet(model->mdlId[mdlIdx], HU3D_ATTR_DISPOFF);
                    }
                }
                sys->step = 7;
            }
            break;

        case 4:
            stat = MdProcTbl[9](1);
            if(stat == 1) {
                MdProcTbl[0](0);
                sys->step = 5;
            }
            break;

        case 5:
            stat = MdProcTbl[8](1);
            if(stat == 1) {
                MdProcTbl[0](0);
                sys->step = 6;
            }
            break;

        case 6:
            stat = MdProcTbl[7](1);
            if(stat == 1) {
                MdProcTbl[0](0);
                MdMessWinHomeClear(mess, 2);
                MdMessWinMesSet(mess, 2, 0x15000D - sys->curSel);
                if(sys->selNum == 1) {
                    sys->step = 3;
                } else {
                    sys->step = 2;
                }
                sys->flowMode = 4;
            }
            break;

        case 7:
            MdProcTbl[9](1);
            slot = &sys->unk9C[sys->plSlot[sys->turn]];
            if(slot->state == 9) {
                MdProcTbl[0](0);
                for(i=0; i<3; i++) {
                    mdlIdx = sys->plSlot[sys->turn]*3+i+3;
                    motIdx = sys->plSlot[sys->turn]*3+i+2;
                    Hu3DMotionSet(model->mdlId[mdlIdx], model->motId[motIdx]);
                    Hu3DMotionTimeSet(model->mdlId[mdlIdx], 0.0f);
                    Hu3DModelAttrReset(model->mdlId[mdlIdx], HU3D_MOTATTR_PAUSE);
                    Hu3DModelAttrSet(model->mdlId[mdlIdx], HU3D_MOTATTR_LOOP);
                }
                cell = &sys->unk33C[sys->turn];
                cell->state = 1;
                sys->turn++;
                if(sys->turn >= 4) {
                    if(sys->plChr[3] == -1) {
                        MdMessWinOpen(mess, 2, 80.0f, 376.0f, 0x1A0, 0x40, 1.0f, 0);
                        MdMessWinMesSet(mess, 2, 0x15000F);
                        sys->finger.cmd = 2;
                    } else {
                        MdMessWinClose(mess, 0);
                        MdMessWinHomeClear(mess, 2);
                        MdMessWinMesSet(mess, 2, 0x15000F);
                    }
                    sys->step = 10;
                } else {
                    sys->plSlot[sys->turn] = sys->plSlot[sys->turn-1];
                    fn_1_9CC8(1);
                    sys->plSlotPrev[sys->turn] = sys->plSlot[sys->turn];
                    if(sys->plChr[sys->turn] != -1) {
                        MdMessWinHomeClear(mess, 2);
                        MdMessWinInsertMesSet(mess, 2, 0x15000E, sys->plChr[sys->turn] + 0x15001C);
                    } else if(sys->plChr[sys->turn-1] != -1) {
                        MdMessWinClose(mess, 0);
                        MdMessWinKill(mess, 2);
                        sys->finger.cmd = 3;
                    }
                    sys->step = 2;
                }
            }
            break;

        case 8:
            MdProcTbl[9](1);
            slot = &sys->unk9C[sys->plSlot[sys->turn]];
            if(slot->state == 6) {
                MdProcTbl[0](0);
                sys->turn--;
                slot = &sys->unk9C[sys->plSlot[sys->turn]];
                slot->state = 10;
                if(sys->plChr[sys->turn] != -1 && sys->plChr[sys->turn+1] == -1) {
                    MdMessHelpWinCreate(mess, 0, 0x20004, 0.0, 340.0, 0x200, 0x20, 0.0, 0);
                    MdMessWinOpen(mess, 2, 80.0f, 376.0f, 0x1A0, 0x40, 1.0f, 0);
                    MdMessWinInsertMesSet(mess, 2, 0x15000E, sys->plChr[sys->turn] + 0x15001C);
                } else if(sys->plChr[sys->turn] != -1) {
                    MdMessWinHomeClear(mess, 2);
                    MdMessWinInsertMesSet(mess, 2, 0x15000E, sys->plChr[sys->turn] + 0x15001C);
                }
                sys->step = 9;
            }
            break;

        case 9:
            MdProcTbl[9](0);
            slot = &sys->unk9C[sys->plSlot[sys->turn]];
            if(slot->state == 12) {
                MdProcTbl[0](0);
                sys->unk3B[sys->turn] = 0;
                for(i=0; i<3; i++) {
                    mdlIdx = sys->plSlot[sys->turn]*3+i+3;
                    if(i == sys->unk3B[sys->turn]) {
                        Hu3DModelAttrReset(model->mdlId[mdlIdx], HU3D_ATTR_DISPOFF);
                    } else {
                        Hu3DModelAttrSet(model->mdlId[mdlIdx], HU3D_ATTR_DISPOFF);
                    }
                }
                sys->step = 3;
            }
            break;

        case 10:
            stat = MdMessWinChoiceGet(mess, 2);
            if(stat == -1 || stat == 1) {
                MdMessWinHomeClear(mess, 2);
                MdMessWinMesSet(mess, 2, 0x150010);
                sys->step = 11;
            } else if(stat == 0) {
                for(i=0; i<4; i++) {
                    Hu3DModelLayerSet(model->mdlId[sys->plSlot[i]*3+sys->unk3B[i]+3], 2);
                }
                MdMessWinKill(mess, 2);
                sys->finger.cmd = 3;
                Hu3DModelAttrReset(model->mdlId[2], HU3D_MOTATTR_PAUSE);
                Hu3DModelAttrSet(model->mdlId[2], HU3D_MOTATTR_REV);
                Hu3DMotionSpeedSet(model->mdlId[2], 4.0f);
                fn_1_A094(10);
                fn_1_9E88();
                sys->loadState = 1;
                sys->loadDirNo = DATA_card;
                sys->step = 16;
            }
            break;

        case 11:
            stat = MdMessWinChoiceGet(mess, 2);
            if(stat == -1 || stat == 1) {
                MdMessWinHomeClear(mess, 2);
                MdMessWinMesSet(mess, 2, 0x150002);
                sys->step = 12;
            } else if(stat == 0) {
                sys->step = 13;
            }
            break;

        case 12:
            stat = MdMessWinChoiceGet(mess, 2);
            if(stat == -1 || stat == 1) {
                MdMessWinHomeClear(mess, 2);
                MdMessWinMesSet(mess, 2, 0x15000F);
                sys->step = 10;
            } else if(stat == 0) {
                fn_1_A094(1);
                WipeColorSet(255, 255, 255);
                WipeCreate(2, 0, 30);
                sys->loadState = 1;
                sys->loadDirNo = DATA_mdomake;
                sys->step = 14;
            }
            break;

        case 13:
            WipeColorSet(0, 0, 0);
            WipeCreate(2, 0, 30);
            while(WipeCheckEnd()) {
                HuPrcVSleep();
            }
            MdMessWinHomeClear(mess, 2);
            model->posP[2]->x = 0.0f;
            model->posP[2]->y = 0.0f;
            model->posP[2]->z = 0.0f;
            model->rotP[2]->x = 0.0f;
            model->rotP[2]->y = 0.0f;
            model->rotP[2]->z = 0.0f;
            Hu3DMotionTimeSet(model->mdlId[2], 0.0f);
            Hu3DModelAttrSet(model->mdlId[2], HU3D_MOTATTR_PAUSE);
            for(slot=&sys->unk9C[0], i=0; i<7; i++, slot++) {
                slot->state = 0;
            }
            fn_1_98C8();
            for(i=0; i<4; i++) {
                sys->unk3B[i] = 0;
            }
            for(i=0; i<7; i++) {
                for(j=0; j<3; j++) {
                    if(j == 0) {
                        Hu3DModelAttrReset(model->mdlId[i*3+j+3], HU3D_ATTR_DISPOFF);
                    } else {
                        Hu3DModelAttrSet(model->mdlId[i*3+j+3], HU3D_ATTR_DISPOFF);
                    }
                    Hu3DMotionSet(model->mdlId[i*3+j+3], model->motId[i*3+j+23]);
                    Hu3DMotionTimeSet(model->mdlId[i*3+j+3], 0.0f);
                    Hu3DModelAttrSet(model->mdlId[i*3+j+3], HU3D_MOTATTR_PAUSE);
                }
            }
            MdSpriteDispSet(sprite, 2, FALSE);
            for(cell=&sys->unk33C[0], i=0; i<4; i++, cell++) {
                cell->state = 0;
            }
            WipeColorSet(0, 0, 0);
            WipeCreate(1, 0, 30);
            while(WipeCheckEnd()) {
                HuPrcVSleep();
            }
            sys->step = 0;
            sys->flowMode = 3;
            break;

        case 14:
            stat = WipeCheckEnd();
            if(stat == 0) {
                sys->step = 15;
            }
            break;

        case 15:
            if(sys->loadState == 2) {
                sys->ovlMode = 2;
                sys->ovlNo = 0x5E;
                sys->skipWipeF = 1;
                sys->exitF = 1;
            }
            break;

        case 16:
            if(Hu3DMotionEndCheck(model->mdlId[2]) == 1) {
                for(i=0; i<7; i++) {
                    if(i != sys->plSlot[0] && i != sys->plSlot[1] && i != sys->plSlot[2] && i != sys->plSlot[3]) {
                        Hu3DModelAttrSet(model->mdlId[i*3+3], HU3D_ATTR_DISPOFF);
                    }
                }
                sys->step = 17;
            }
            break;

        case 17:
            stat = MdProcTbl[8](1);
            if(stat == 1) {
                MdProcTbl[0](0);
                *state = 1;
                for(cell=&sys->unk33C[0], i=0; i<4; i++, cell++) {
                    cell->state = 4;
                }
                sys->step = 18;
            }
            break;

        case 18:
            stat = MdProcTbl[8](2);
            if(stat == 1) {
                MdProcTbl[0](0);
                for(i=0; i<4; i++) {
                    slot = &sys->unk9C[sys->plSlot[i]];
                    slot->state = 13;
                }
                fn_1_A094(1);
                WipeColorSet(255, 255, 255);
                WipeCreate(2, 0, 60);
                sys->step = 19;
            }
            break;

        case 19:
            stat = WipeCheckEnd();
            if(stat == 0) {
                sys->step = 20;
            }
            break;

        case 20:
            if(sys->loadState == 2) {
                sys->ovlMode = 0;
                sys->ovlNo = 2;
                sys->skipWipeF = 1;
                sys->exitF = 1;
            }
            break;
    }
}








static HuVecF lbl_1_data_318[2] = {
    { 288.0f, 100.0f, 3000.0f },
    { 0.0f, 0.0f, 0.0f }
};

s32 fn_1_44F8(void)
{
    sys->procInit = 0;
    sys->unk3A0 = 0;
    sys->unk3A4 = 0;
    sys->procT[0] = 0.0f;
    sys->procT[1] = 0.0f;
    sys->procT[2] = 0.0f;
    sys->procT[3] = 0.0f;
    sys->procOut[0] = 0.0f;
    sys->procOut[1] = 0.0f;
    sys->procOut[2] = 0.0f;
    sys->procOut[3] = 0.0f;
    return 0;
}

s32 fn_1_461C(void)
{
    MDCAMERA *cam;
    MDCAMERA_WORK *camWork;

    camWork = sys->camera;
    cam = &camWork->camera[0];
    if(sys->procInit == 0) {
        lbl_1_bss_1C8.centerFrom = cam->center;
        lbl_1_bss_1C8.hvrFrom.x = cam->hvr.x;
        lbl_1_bss_1C8.hvrFrom.y = cam->hvr.y;
        lbl_1_bss_1C8.hvrFrom.z = cam->hvr.z;
        lbl_1_bss_1C8.centerTo.x = 0.0f;
        lbl_1_bss_1C8.centerTo.y = 470.0f;
        lbl_1_bss_1C8.centerTo.z = 0.0f;
        lbl_1_bss_1C8.hvrTo.x = 50.0f;
        lbl_1_bss_1C8.hvrTo.y = 15.0f;
        lbl_1_bss_1C8.hvrTo.z = 4000.0f;
        sys->procInit = 1;
    }
    MdMathOscillate(MDMATH_OSC_90, &sys->procT[0], &sys->procOut[0], 240.0f);
    MdMathOscillate(MDMATH_OSC_180, &sys->procT[1], &sys->procOut[1], 240.0f);
    MdMathLerp(&cam->center.x, lbl_1_bss_1C8.centerFrom.x, lbl_1_bss_1C8.centerTo.x, sys->procOut[0]);
    MdMathLerp(&cam->center.y, lbl_1_bss_1C8.centerFrom.y, lbl_1_bss_1C8.centerTo.y, sys->procOut[0]);
    MdMathLerp(&cam->center.z, lbl_1_bss_1C8.centerFrom.z, lbl_1_bss_1C8.centerTo.z, sys->procOut[0]);
    MdMathLerp(&cam->hvr.x, lbl_1_bss_1C8.hvrFrom.x, lbl_1_bss_1C8.hvrTo.x, sys->procOut[0]);
    MdMathLerp(&cam->hvr.y, lbl_1_bss_1C8.hvrFrom.y, lbl_1_bss_1C8.hvrTo.y, sys->procOut[0]);
    MdMathLerpOfs(&cam->hvr.z, lbl_1_bss_1C8.hvrFrom.z, lbl_1_bss_1C8.hvrTo.z, sys->procOut[0], 0.5f*sys->procOut[1]);
    if(sys->procT[0] >= 1.0f) {
        return 1;
    }
    return 0;
}

// remaining diff: local register order, extsh on camera bit (hsfex.h takes s16)
s32 fn_1_4944(s32 modelNo)
{
    HuVecF *pos;
    MDMODEL_WORK *model;
    HuVecF *rot;
    HuVecF *scale;
    int i;
    MDCAMERA_WORK *camWork;

    model = sys->model;
    camWork = sys->camera;
    pos = model->posP[modelNo];
    rot = model->rotP[modelNo];
    scale = model->scaleP[modelNo];
    if(sys->procInit == 0) {
        lbl_1_data_318[1] = lbl_1_data_318[0];
        lbl_1_data_318[1].x = -128.0f;
        for(i=0; i<2; i++) {
            Hu3D2Dto3D(&lbl_1_data_318[i], camWork->camera[1].bit, &lbl_1_bss_1B0[i]);
        }
        *pos = lbl_1_bss_1B0[1];
        rot->x = 90.0f;
        rot->y = 180.0f;
        rot->z = 0.0f;
        scale->x = 1.0f;
        scale->y = 1.0f;
        scale->z = 1.0f;
        Hu3DModelAttrReset(model->mdlId[modelNo], HU3D_ATTR_DISPOFF);
        fn_1_A094(6);
        sys->procInit = 1;
    }
    MdMathOscillate(MDMATH_OSC_90, &sys->procT[0], &sys->procOut[0], 30.0f);
    MdMathLerp(&pos->x, lbl_1_bss_1B0[1].x, lbl_1_bss_1B0[0].x, sys->procOut[0]);
    MdMathLerp(&pos->y, lbl_1_bss_1B0[1].y, lbl_1_bss_1B0[0].y, sys->procOut[0]);
    MdMathLerp(&pos->z, lbl_1_bss_1B0[1].z, lbl_1_bss_1B0[0].z, sys->procOut[0]);
    if(sys->procT[0] >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_4C00(s32 modelNo)
{
    MDMODEL_WORK *model;
    MDCAMERA_WORK *camWork;
    HuVecF *pos;
    HuVecF *rot;
    HuVecF *scale;

    model = sys->model;
    camWork = sys->camera;
    pos = model->posP[modelNo];
    rot = model->rotP[modelNo];
    scale = model->scaleP[modelNo];
    if(sys->procInit == 0) {
        lbl_1_bss_1AC = rot->y;
        fn_1_A094(8);
        sys->procInit = 1;
    }
    MdMathOscillate(MDMATH_OSC_90, &sys->procT[0], &sys->procOut[0], 30.0f);
    MdMathOscillate(MDMATH_OSC_180, &sys->procT[1], &sys->procOut[1], 30.0f);
    MdMathLerpOfs(&rot->y, lbl_1_bss_1AC, 180.0f+lbl_1_bss_1AC, sys->procOut[0], 0.2f*sys->procOut[1]);
    if(sys->procT[0] >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_4DC8(s32 modelNo)
{
    MDMODEL_WORK *model;
    MDCAMERA_WORK *camWork;
    HuVecF *pos;
    HuVecF *rot;
    HuVecF *scale;

    model = sys->model;
    camWork = sys->camera;
    pos = model->posP[modelNo];
    rot = model->rotP[modelNo];
    scale = model->scaleP[modelNo];
    if(sys->procInit == 0) {
        lbl_1_bss_1A8 = rot->z;
        fn_1_A094(7);
        sys->procInit = 1;
    }
    MdMathOscillate(MDMATH_OSC_90, &sys->procT[0], &sys->procOut[0], 30.0f);
    MdMathLerp(&rot->z, lbl_1_bss_1A8, lbl_1_bss_1A8-512.0f, sys->procOut[0]);
    MdMathLerpScale(scale, 1.0f, 0.0f, sys->procOut[0]);
    if(sys->procT[0] >= 1.0f) {
        Hu3DModelAttrSet(model->mdlId[modelNo], HU3D_ATTR_DISPOFF);
        return 1;
    }
    return 0;
}

static HuVecF lbl_1_data_330 = { 0.0f, 0.0f, 0.0f };

static HuVecF lbl_1_data_33C[4] = {
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f }
};

// remaining diff: Hu3D2Dto3D prototype (u16 arg / int return in the original header), float operand order
s32 fn_1_4F80(s32 mode)
{
    int i;
    MDMODEL_WORK *model;
    MDCAMERA_WORK *camWork;
    HuVecF *pos[4][2];
    HuVecF *rot[4][2];
    HuVecF *scale[4][2];

    model = sys->model;
    camWork = sys->camera;
    if(lbl_1_bss_1 == 0) {
        lbl_1_data_330.x = 288.0f;
        lbl_1_data_330.y = 64.0f+(176.0f*lbl_1_data_0[0]);
        lbl_1_data_330.z = lbl_1_data_0[1];
        for(i=0; i<4; i++) {
            lbl_1_data_33C[i].x = (288.0f-((3.0f*lbl_1_data_0[2])*0.5f))+((float)i*lbl_1_data_0[2]);
            lbl_1_data_33C[i].y = lbl_1_data_330.y;
            lbl_1_data_33C[i].z = lbl_1_data_0[3];
        }
        lbl_1_bss_1 = 1;
    }
    for(i=0; i<4; i++) {
        pos[i][0] = model->posP[i+24];
        pos[i][1] = model->posP[i+28];
        rot[i][0] = model->rotP[i+24];
        rot[i][1] = model->rotP[i+28];
        scale[i][0] = model->scaleP[i+24];
        scale[i][1] = model->scaleP[i+28];
    }
    if(sys->procInit == 0) {
        for(i=0; i<4; i++) {
            if(i < sys->curSel+1) {
                lbl_1_bss_164[i].cur = 0;
            } else {
                lbl_1_bss_164[i].cur = 1;
            }
            if(i < sys->prevSel+1) {
                lbl_1_bss_164[i].next = 0;
            } else {
                lbl_1_bss_164[i].next = 1;
            }
        }
        if(mode == 0) {
            Hu3D2Dto3D(&lbl_1_data_330, camWork->camera[1].bit, &lbl_1_bss_19C);
            for(i=0; i<4; i++) {
                Hu3D2Dto3D(&lbl_1_data_33C[i], camWork->camera[1].bit, &lbl_1_bss_16C[i]);
            }
            for(i=0; i<4; i++) {
                pos[i][lbl_1_bss_164[i].cur]->x = lbl_1_bss_19C.x;
                pos[i][lbl_1_bss_164[i].cur]->y = lbl_1_bss_19C.y;
                pos[i][lbl_1_bss_164[i].cur]->z = lbl_1_bss_19C.z;
                rot[i][lbl_1_bss_164[i].cur]->x = 0.0f;
                rot[i][lbl_1_bss_164[i].cur]->y = 0.0f;
                rot[i][lbl_1_bss_164[i].cur]->z = 0.0f;
                scale[i][lbl_1_bss_164[i].cur]->x = 0.0f;
                scale[i][lbl_1_bss_164[i].cur]->y = 0.0f;
                scale[i][lbl_1_bss_164[i].cur]->z = 0.0f;
                if(lbl_1_bss_164[i].cur == 0) {
                    Hu3DModelAttrReset(model->mdlId[i+24], HU3D_ATTR_DISPOFF);
                    Hu3DModelAttrSet(model->mdlId[i+28], HU3D_ATTR_DISPOFF);
                } else if(lbl_1_bss_164[i].cur == 1) {
                    Hu3DModelAttrSet(model->mdlId[i+24], HU3D_ATTR_DISPOFF);
                    Hu3DModelAttrReset(model->mdlId[i+28], HU3D_ATTR_DISPOFF);
                }
            }
        } else if(mode == 1) {
        } else if(mode == 2 || mode == 3) {
            for(i=0; i<4; i++) {
                if(lbl_1_bss_164[i].cur != lbl_1_bss_164[i].next) {
                    pos[i][lbl_1_bss_164[i].next]->x = lbl_1_bss_19C.x;
                    pos[i][lbl_1_bss_164[i].next]->y = lbl_1_bss_19C.y;
                    pos[i][lbl_1_bss_164[i].next]->z = lbl_1_bss_19C.z;
                    rot[i][lbl_1_bss_164[i].next]->x = 0.0f;
                    rot[i][lbl_1_bss_164[i].next]->y = 0.0f;
                    rot[i][lbl_1_bss_164[i].next]->z = 0.0f;
                    scale[i][lbl_1_bss_164[i].next]->x = 0.0f;
                    scale[i][lbl_1_bss_164[i].next]->y = 0.0f;
                    scale[i][lbl_1_bss_164[i].next]->z = 0.0f;
                    Hu3DModelAttrReset(model->mdlId[i+lbl_1_bss_164[i].cur*4+24], HU3D_ATTR_DISPOFF);
                }
            }
            lbl_1_bss_160 = 200.0f;
            lbl_1_bss_15C = 90.0f;
            for(i=0; i<4; i++) {
                lbl_1_bss_12C[i] = lbl_1_bss_16C[i];
                lbl_1_bss_12C[i].z -= lbl_1_bss_160;
            }
        }
        sys->procInit = 1;
    }
    if(mode == 0) {
        MdMathOscillate(MDMATH_OSC_90, &sys->procT[0], &sys->procOut[0], 30.0f);
        for(i=0; i<4; i++) {
            MdMathLerp(&pos[i][lbl_1_bss_164[i].cur]->x, lbl_1_bss_19C.x, lbl_1_bss_16C[i].x, sys->procOut[0]);
            MdMathLerp(&pos[i][lbl_1_bss_164[i].cur]->y, lbl_1_bss_19C.y, lbl_1_bss_16C[i].y, sys->procOut[0]);
            MdMathLerp(&pos[i][lbl_1_bss_164[i].cur]->z, lbl_1_bss_19C.z, lbl_1_bss_16C[i].z, sys->procOut[0]);
            MdMathLerpScale(scale[i][lbl_1_bss_164[i].cur], 0.0f, 1.0f, sys->procOut[0]);
        }
    } else if(mode == 1) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procT[0], &sys->procOut[0], 15.0f);
        for(i=0; i<4; i++) {
            MdMathLerpScale(scale[i][lbl_1_bss_164[i].cur], 1.0f, 0.0f, sys->procOut[0]);
        }
    } else if(mode == 2 || mode == 3) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procT[0], &sys->procOut[0], 20.0f);
        if(mode == 2) {
            MdMathLerp(&lbl_1_bss_15C, 90.0f, 270.0f, sys->procOut[0]);
        } else if(mode == 3) {
            MdMathLerp(&lbl_1_bss_15C, 90.0f, -90.0f, sys->procOut[0]);
        }
        for(i=0; i<4; i++) {
            if(lbl_1_bss_164[i].cur != lbl_1_bss_164[i].next) {
                MdMathLerpScale(scale[i][lbl_1_bss_164[i].next], 1.0f, 0.0f, sys->procOut[0]);
                pos[i][lbl_1_bss_164[i].next]->x = lbl_1_bss_12C[i].x+(lbl_1_bss_160*cos(M_PI*lbl_1_bss_15C/180.0));
                pos[i][lbl_1_bss_164[i].next]->y = lbl_1_bss_12C[i].y;
                pos[i][lbl_1_bss_164[i].next]->z = lbl_1_bss_12C[i].z+(lbl_1_bss_160*sin(M_PI*lbl_1_bss_15C/180.0));
                MdMathLerpScale(scale[i][lbl_1_bss_164[i].cur], 0.0f, 1.0f, sys->procOut[0]);
                pos[i][lbl_1_bss_164[i].cur]->x = lbl_1_bss_12C[i].x+(lbl_1_bss_160*cos(M_PI*(180.0f+lbl_1_bss_15C)/180.0));
                pos[i][lbl_1_bss_164[i].cur]->y = lbl_1_bss_12C[i].y;
                pos[i][lbl_1_bss_164[i].cur]->z = lbl_1_bss_12C[i].z+(lbl_1_bss_160*sin(M_PI*(180.0f+lbl_1_bss_15C)/180.0));
            }
        }
        MdProcTbl[6](mode);
    }
    for(i=0; i<4; i++) {
        fn_1_942C(i+24, 1);
        fn_1_942C(i+28, 1);
    }
    if(sys->procT[0] >= 1.0f) {
        if(mode == 0) {
        } else if(mode == 1) {
            for(i=0; i<4; i++) {
                Hu3DModelAttrSet(model->mdlId[i+lbl_1_bss_164[i].cur*4+24], HU3D_ATTR_DISPOFF);
            }
        } else if(mode == 2 || mode == 3) {
            for(i=0; i<4; i++) {
                if(lbl_1_bss_164[i].cur != lbl_1_bss_164[i].next) {
                    Hu3DModelAttrSet(model->mdlId[i+lbl_1_bss_164[i].next*4+24], HU3D_ATTR_DISPOFF);
                }
            }
        }
        return 1;
    }
    return 0;
}

// remaining diff: a few dead mr pairs around MdSpritePosGet results
s32 fn_1_61F8(s32 mode)
{
    MDSPRITE_WORK *sprite;
    MDMODEL_WORK *model;
    HUSPRGRPID grpId;
    HuVec2f *pos;
    HuVec2f *scale;

    sprite = sys->sprite;
    model = sys->model;
    grpId = sprite->grpId[1];
    if(lbl_1_bss_2 == 0) {
        lbl_1_bss_FC[0].x = 112.0f;
        lbl_1_bss_FC[1].x = 464.0f;
        lbl_1_bss_FC[0].y = 64.0f+(176.0f*lbl_1_data_0[0]);
        lbl_1_bss_FC[1].y = lbl_1_bss_FC[0].y;
        lbl_1_bss_FC[2].x = -128.0f;
        lbl_1_bss_FC[3].x = 704.0f;
        lbl_1_bss_FC[2].y = lbl_1_bss_FC[0].y;
        lbl_1_bss_FC[3].y = lbl_1_bss_FC[1].y;
        lbl_1_bss_2 = 1;
    }
    if(sys->unk3A0 == 0) {
        HuSprGrpPosSet(grpId, 0.0f, 0.0f);
        if(mode == 0) {
            pos = MdSpritePosGet(sprite, 1, 0);
            pos->x = lbl_1_bss_FC[2].x;
            pos->y = lbl_1_bss_FC[2].y;
            HuSprAttrReset(grpId, 0, HUSPR_ATTR_DISPOFF);
            pos = MdSpritePosGet(sprite, 1, 1);
            pos->x = lbl_1_bss_FC[3].x;
            pos->y = lbl_1_bss_FC[3].y;
            HuSprAttrReset(grpId, 1, HUSPR_ATTR_DISPOFF);
        } else if(mode == 1) {
        } else if(mode == 2) {
            pos = MdSpritePosGet(sprite, 1, 2);
            pos->x = lbl_1_bss_FC[2].x;
            pos->y = lbl_1_bss_FC[2].y;
            HuSprTPLvlSet(grpId, 2, 0.0f);
            HuSprAttrReset(grpId, 2, HUSPR_ATTR_DISPOFF);
        } else if(mode == 3) {
            pos = MdSpritePosGet(sprite, 1, 3);
            pos->x = lbl_1_bss_FC[3].x;
            pos->y = lbl_1_bss_FC[3].y;
            HuSprTPLvlSet(grpId, 3, 0.0f);
            HuSprAttrReset(grpId, 3, HUSPR_ATTR_DISPOFF);
        }
        sys->unk3A0 = 1;
    }
    if(mode == 0) {
        MdMathOscillate(MDMATH_OSC_90, &sys->procT[1], &sys->procOut[1], 15.0f);
        MdMathLerp((float *)MdSpritePosGet(sprite, 1, 0), lbl_1_bss_FC[2].x, lbl_1_bss_FC[0].x, sys->procOut[1]);
        MdMathLerp((float *)MdSpritePosGet(sprite, 1, 1), lbl_1_bss_FC[3].x, lbl_1_bss_FC[1].x, sys->procOut[1]);
    } else if(mode == 1) {
        MdMathOscillate(MDMATH_OSC_90, &sys->procT[1], &sys->procOut[1], 15.0f);
        MdMathLerp((float *)MdSpritePosGet(sprite, 1, 0), lbl_1_bss_FC[0].x, lbl_1_bss_FC[2].x, sys->procOut[1]);
        MdMathLerp((float *)MdSpritePosGet(sprite, 1, 1), lbl_1_bss_FC[1].x, lbl_1_bss_FC[3].x, sys->procOut[1]);
    } else if(mode == 2) {
        MdMathOscillate(MDMATH_OSC_180, &sys->procT[1], &sys->procOut[1], 15.0f);
        pos = MdSpritePosGet(sprite, 1, 0);
        scale = MdSpriteScaleGet(sprite, 1, 0);
        MdMathLerp(&pos->x, lbl_1_bss_FC[0].x, lbl_1_bss_FC[0].x-24.0f, sys->procOut[1]);
        MdMathLerp(&scale->x, 1.0f, 1.5f, sys->procOut[1]);
        MdMathLerp(&scale->y, 1.0f, 1.5f, sys->procOut[1]);
        pos = MdSpritePosGet(sprite, 1, 2);
        scale = MdSpriteScaleGet(sprite, 1, 2);
        MdMathLerp(&pos->x, lbl_1_bss_FC[0].x, lbl_1_bss_FC[0].x-24.0f, sys->procOut[1]);
        MdMathLerp(&scale->x, 1.0f, 1.5f, sys->procOut[1]);
        MdMathLerp(&scale->y, 1.0f, 1.5f, sys->procOut[1]);
        HuSprTPLvlSet(grpId, 2, sys->procOut[1]);
    } else if(mode == 3) {
        MdMathOscillate(MDMATH_OSC_180, &sys->procT[1], &sys->procOut[1], 15.0f);
        pos = MdSpritePosGet(sprite, 1, 1);
        scale = MdSpriteScaleGet(sprite, 1, 1);
        MdMathLerp(&pos->x, lbl_1_bss_FC[1].x, 24.0f+lbl_1_bss_FC[1].x, sys->procOut[1]);
        MdMathLerp(&scale->x, 1.0f, 1.5f, sys->procOut[1]);
        MdMathLerp(&scale->y, 1.0f, 1.5f, sys->procOut[1]);
        pos = MdSpritePosGet(sprite, 1, 3);
        scale = MdSpriteScaleGet(sprite, 1, 3);
        MdMathLerp(&pos->x, lbl_1_bss_FC[1].x, 24.0f+lbl_1_bss_FC[1].x, sys->procOut[1]);
        MdMathLerp(&scale->x, 1.0f, 1.5f, sys->procOut[1]);
        MdMathLerp(&scale->y, 1.0f, 1.5f, sys->procOut[1]);
        HuSprTPLvlSet(grpId, 3, sys->procOut[1]);
    }
    if(sys->procT[1] >= 1.0f) {
        if(mode == 0) {
        } else if(mode == 1) {
            HuSprAttrSet(grpId, 0, HUSPR_ATTR_DISPOFF);
            HuSprAttrSet(grpId, 1, HUSPR_ATTR_DISPOFF);
        } else if(mode == 2) {
            HuSprAttrSet(grpId, 2, HUSPR_ATTR_DISPOFF);
        } else if(mode == 3) {
            HuSprAttrSet(grpId, 3, HUSPR_ATTR_DISPOFF);
        }
        return 1;
    }
    return 0;
}

// remaining diff: float operand order, extsh on camera bit (see fn_1_4944)
s32 fn_1_6B0C(s32 mode)
{
    int i;
    MDCAMERA_WORK *camWork;
    MDMODEL_WORK *model;
    MDMESS_WORK *mess;
    HuVecF in[4];
    HuVecF out[4];

    camWork = sys->camera;
    model = sys->model;
    mess = sys->mess;
    if(lbl_1_bss_EC == 0) {
        lbl_1_bss_D4[0].x = 48.0f;
        lbl_1_bss_D4[0].y = 64.0f;
        lbl_1_bss_D4[1].x = 48.0f;
        lbl_1_bss_D4[1].y = -240.0f;
        for(i=0; i<4; i++) {
            lbl_1_bss_A4[i].x = 48.0f-((288.0f-((3.0f*lbl_1_data_0[2])*0.5f))+((float)i*lbl_1_data_0[2]));
            lbl_1_bss_A4[i].y = 64.0f-(64.0f+(176.0f*lbl_1_data_0[0]));
        }
        lbl_1_bss_EC = 1;
    }
    if(sys->procInit == 0) {
        if(mode == 0) {
            lbl_1_bss_F0.x = 48.0f;
            lbl_1_bss_F0.y = 64.0f;
        } else if(mode == 1) {
            MdMessWinOpen(mess, 1, lbl_1_bss_D4[1].x, lbl_1_bss_D4[1].y, 0x1E0, 0xB0, 1.0f, 0x40);
            MdMessWinHomeClear(mess, 1);
            MdMessWinAttrSet(mess, 1, HUWIN_ATTR_ALIGN_CENTER);
            MdMessWinMesSet(mess, 1, 0x150009);
            for(i=0; i<4; i++) {
                if(i < sys->curSel+1) {
                    Hu3DModelAttrReset(model->mdlId[i+24], HU3D_ATTR_DISPOFF);
                } else {
                    Hu3DModelAttrReset(model->mdlId[i+28], HU3D_ATTR_DISPOFF);
                }
            }
        }
        sys->procInit = 1;
    }
    if(mode == 0) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procT[0], &sys->procOut[0], 30.0f);
        MdMathLerp(&lbl_1_bss_F0.x, lbl_1_bss_D4[0].x, lbl_1_bss_D4[1].x, sys->procOut[0]);
        MdMathLerp(&lbl_1_bss_F0.y, lbl_1_bss_D4[0].y, lbl_1_bss_D4[1].y, sys->procOut[0]);
    } else if(mode == 1) {
        MdMathOscillate(MDMATH_OSC_90, &sys->procT[0], &sys->procOut[0], 30.0f);
        MdMathLerp(&lbl_1_bss_F0.x, lbl_1_bss_D4[1].x, lbl_1_bss_D4[0].x, sys->procOut[0]);
        MdMathLerp(&lbl_1_bss_F0.y, lbl_1_bss_D4[1].y, lbl_1_bss_D4[0].y, sys->procOut[0]);
    }
    MdMessWinPosSet(mess, 1, lbl_1_bss_F0.x, lbl_1_bss_F0.y);
    for(i=0; i<4; i++) {
        in[i].x = lbl_1_bss_F0.x-lbl_1_bss_A4[i].x;
        in[i].y = lbl_1_bss_F0.y-lbl_1_bss_A4[i].y;
        in[i].z = lbl_1_data_0[3];
        Hu3D2Dto3D(&in[i], camWork->camera[1].bit, &out[i]);
        if(i < sys->curSel+1) {
            *model->posP[i+24] = out[i];
            fn_1_942C(i+24, 1);
        } else {
            *model->posP[i+28] = out[i];
            fn_1_942C(i+28, 1);
        }
    }
    if(sys->procT[0] >= 1.0f) {
        if(mode == 0) {
            MdMessWinClose(mess, 1);
            for(i=0; i<4; i++) {
                if(i < sys->curSel+1) {
                    Hu3DModelAttrSet(model->mdlId[i+24], HU3D_ATTR_DISPOFF);
                } else {
                    Hu3DModelAttrSet(model->mdlId[i+28], HU3D_ATTR_DISPOFF);
                }
            }
        } else if(mode == 1) {
        }
        return 1;
    }
    return 0;
}

// remaining diff: extsh on camera bit (see fn_1_4944)
s32 fn_1_71B8(s32 mode)
{
    MDCAMERA_WORK *camWork;
    MDMODEL_WORK *model;
    HuVecF *pos;
    HuVecF *rot;
    HuVecF *scale;
    Mtx *mtxP;
    Mtx rotMtx;

    camWork = sys->camera;
    model = sys->model;
    pos = model->posP[2];
    rot = model->rotP[2];
    scale = model->scaleP[2];
    if(sys->procInit == 0) {
        lbl_1_bss_8C[0].x = lbl_1_data_10[0].x;
        lbl_1_bss_8C[0].y = lbl_1_data_10[0].y;
        lbl_1_bss_8C[0].z = lbl_1_data_10[0].z;
        lbl_1_bss_68[0].x = 0.0f;
        lbl_1_bss_68[0].y = 0.0f;
        lbl_1_bss_68[0].z = 0.0f;
        Hu3D2Dto3D(&lbl_1_bss_8C[0], camWork->camera[0].bit, &lbl_1_bss_68[1]);
        lbl_1_bss_44[0].x = 0.0f;
        lbl_1_bss_44[0].y = 0.0f;
        lbl_1_bss_44[0].z = 0.0f;
        mtxP = MdMathMatrixDir(&lbl_1_bss_68[1], &camWork->camera[0].pos);
        PSMTXRotRad(rotMtx, 'X', 1.5707964f);
        PSMTXConcat(*mtxP, rotMtx, *mtxP);
        Hu3DMtxRotGet(*mtxP, &lbl_1_bss_44[1]);
        if(mode == 2) {
            lbl_1_bss_8C[1].x = 288.0f;
            lbl_1_bss_8C[1].y = -384.0f;
            lbl_1_bss_8C[1].z = 2500.0f;
            Hu3D2Dto3D(&lbl_1_bss_8C[1], camWork->camera[0].bit, &lbl_1_bss_68[2]);
            lbl_1_bss_44[2].x = 0.0f;
            lbl_1_bss_44[2].y = -180.0f;
            lbl_1_bss_44[2].z = 0.0f;
        }
        sys->procInit = 1;
    }
    if(mode == 0) {
        MdMathOscillate(MDMATH_OSC_90, &sys->procT[0], &sys->procOut[0], 60.0f);
        MdMathLerp(&pos->x, lbl_1_bss_68[0].x, lbl_1_bss_68[1].x, sys->procOut[0]);
        MdMathLerp(&pos->y, lbl_1_bss_68[0].y, lbl_1_bss_68[1].y, sys->procOut[0]);
        MdMathLerp(&pos->z, lbl_1_bss_68[0].z, lbl_1_bss_68[1].z, sys->procOut[0]);
        MdMathLerp(&rot->x, lbl_1_bss_44[0].x, lbl_1_bss_44[1].x, sys->procOut[0]);
        MdMathLerp(&rot->y, lbl_1_bss_44[0].y, lbl_1_bss_44[1].y, sys->procOut[0]);
        MdMathLerp(&rot->z, lbl_1_bss_44[0].z, lbl_1_bss_44[1].z, sys->procOut[0]);
    } else if(mode == 1) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procT[0], &sys->procOut[0], 30.0f);
        MdMathLerp(&pos->x, lbl_1_bss_68[1].x, lbl_1_bss_68[0].x, sys->procOut[0]);
        MdMathLerp(&pos->y, lbl_1_bss_68[1].y, lbl_1_bss_68[0].y, sys->procOut[0]);
        MdMathLerp(&pos->z, lbl_1_bss_68[1].z, lbl_1_bss_68[0].z, sys->procOut[0]);
        MdMathLerp(&rot->x, lbl_1_bss_44[1].x, lbl_1_bss_44[0].x, sys->procOut[0]);
        MdMathLerp(&rot->y, lbl_1_bss_44[1].y, lbl_1_bss_44[0].y, sys->procOut[0]);
        MdMathLerp(&rot->z, lbl_1_bss_44[1].z, lbl_1_bss_44[0].z, sys->procOut[0]);
    } else if(mode == 2) {
        MdMathOscillate(MDMATH_OSC_90_REV, &sys->procT[0], &sys->procOut[0], 60.0f);
        MdMathLerp(&pos->x, lbl_1_bss_68[0].x, lbl_1_bss_68[2].x, sys->procOut[0]);
        MdMathLerp(&pos->y, lbl_1_bss_68[0].y, lbl_1_bss_68[2].y, sys->procOut[0]);
        MdMathLerp(&pos->z, lbl_1_bss_68[0].z, lbl_1_bss_68[2].z, sys->procOut[0]);
        MdMathLerp(&rot->x, lbl_1_bss_44[0].x, lbl_1_bss_44[2].x, sys->procOut[0]);
        MdMathLerp(&rot->y, lbl_1_bss_44[0].y, lbl_1_bss_44[2].y, sys->procOut[0]);
        MdMathLerp(&rot->z, lbl_1_bss_44[0].z, lbl_1_bss_44[2].z, sys->procOut[0]);
        MdMathLerpScale(scale, 1.0f, 0.0f, sys->procOut[0]);
    }
    fn_1_98C8();
    if(sys->procT[0] >= 1.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_78C4(s32 mode)
{
    MDCAMERA_WORK *camWork;
    MDMODEL_WORK *model;
    HuVecF *pos;
    HuVecF *rot;
    HuVecF *scale;
    MDCARD_SLOT *slot;

    camWork = sys->camera;
    model = sys->model;
    pos = model->posP[32];
    rot = model->rotP[32];
    scale = model->scaleP[32];
    if(sys->procInit == 0) {
        if(mode == 0) {
            slot = &sys->unk9C[sys->plSlot[sys->turn]];
            *pos = lbl_1_bss_2F0[sys->plSlot[sys->turn]].fingerPos;
            *rot = lbl_1_bss_2F0[sys->plSlot[sys->turn]].fingerRot;
            scale->x = 0.0f;
            scale->y = 0.0f;
            scale->z = 0.0f;
            Hu3DModelAttrReset(model->mdlId[32], HU3D_ATTR_DISPOFF);
            if(sys->plChr[sys->turn] == -1) {
                Hu3DAnmNoSet(model->animId[0], 4);
            } else {
                Hu3DAnmNoSet(model->animId[0], sys->plChr[sys->turn]);
            }
        } else if(mode == 1) {
        } else if(mode == 2) {
        }
        sys->procInit = 1;
    }
    if(mode == 0) {
        MdMathOscillate(MDMATH_OSC_90, &sys->procT[0], &sys->procOut[0], 15.0f);
        MdMathOscillate(MDMATH_OSC_180, &sys->procT[1], &sys->procOut[1], 15.0f);
        MdMathLerpScaleOfs(scale, 0.0f, 4.0f, sys->procOut[0], 2.0f*sys->procOut[1]);
    } else if(mode == 1) {
        MdMathOscillate(MDMATH_OSC_90, &sys->procT[0], &sys->procOut[0], 15.0f);
        MdMathLerpScale(scale, 4.0f, 0.0f, sys->procOut[0]);
    } else if(mode == 2) {
        slot = &sys->unk9C[sys->plSlot[sys->turn]];
        *pos = lbl_1_bss_2F0[sys->plSlot[sys->turn]].fingerPos;
        *rot = lbl_1_bss_2F0[sys->plSlot[sys->turn]].fingerRot;
    }
    if(sys->procT[0] >= 1.0f) {
        if(mode == 0) {
        } else if(mode == 1) {
            Hu3DModelAttrSet(model->mdlId[32], HU3D_ATTR_DISPOFF);
        } else if(mode == 2) {
        }
        return 1;
    }
    return 0;
}

static HuVecF lbl_1_data_36C[2] = {
    { 64.0f, 300.0f, 1280.0f },
    { 64.0f, -128.0f, 1280.0f }
};

static HuVecF lbl_1_data_384[4] = {
    { -300.0f, 2200.0f, -100.0f },
    { -100.0f, 2200.0f, -100.0f },
    { 100.0f, 2200.0f, -100.0f },
    { 300.0f, 2200.0f, -100.0f }
};

// remaining diff: first switch compare tree
void fn_1_7DA0(void)
{
    int i;
    MDMODEL_WORK *model = sys->model;
    MDCAMERA_WORK *camWork = sys->camera;
    MDMESS_WORK *mess = sys->mess;
    MDCARD_FNG *finger = &sys->finger;
    HuVecF *pos = model->posP[0];
    HuVecF *rot = model->rotP[0];
    HuVecF *scale = model->scaleP[0];

    while(1) {
        model = sys->model;
        camWork = sys->camera;
        mess = sys->mess;
        finger = &sys->finger;
        pos = model->posP[0];
        rot = model->rotP[0];
        scale = model->scaleP[0];
        switch(finger->state) {
            case 0:
                Hu3D2Dto3D(&lbl_1_data_36C[0], camWork->camera[1].bit, &lbl_1_bss_2C[0]);
                *pos = lbl_1_bss_2C[0];
                fn_1_942C(0, 1);
                break;

            case 1:
                finger->t = 0.0f;
                finger->out = 0.0f;
                for(i=0; i<2; i++) {
                    Hu3D2Dto3D(&lbl_1_data_36C[i], camWork->camera[1].bit, &lbl_1_bss_2C[i]);
                }
                finger->state = 2;
                break;

            case 2:
                MdMathOscillate(MDMATH_OSC_90_REV, &finger->t, &finger->out, 60.0f);
                MdMathLerp(&pos->x, lbl_1_bss_2C[0].x, lbl_1_bss_2C[1].x, finger->out);
                MdMathLerp(&pos->y, lbl_1_bss_2C[0].y, lbl_1_bss_2C[1].y, finger->out);
                MdMathLerp(&pos->z, lbl_1_bss_2C[0].z, lbl_1_bss_2C[1].z, finger->out);
                if(finger->t >= 1.0f) {
                    finger->state = 3;
                }
                break;
        }
        switch(finger->cmd) {
            case 0:
                break;

            case 1:
                Hu3DModelAttrSet(model->mdlId[0], HU3D_MOTATTR_LOOP);
                Hu3DModelAttrReset(model->mdlId[0], HU3D_MOTATTR_PAUSE);
                Hu3DMotionSet(model->mdlId[0], model->motId[0]);
                finger->cmd = 0;
                break;

            case 2:
                Hu3DMotionShiftSet(model->mdlId[0], model->motId[1], 0.0f, 32.0f, HU3D_MOTATTR_LOOP);
                finger->cmd = 0;
                break;

            case 3:
                Hu3DMotionShiftSet(model->mdlId[0], model->motId[0], 0.0f, 32.0f, HU3D_MOTATTR_LOOP);
                finger->cmd = 0;
                break;
        }
        HuPrcVSleep();
    }
}

void fn_1_80A8(void)
{
    int i;
    MDMODEL_WORK *model;
    MDCAMERA_WORK *camWork;
    MDCAMERA *cam;
    HuVecF *pos;
    HuVecF *rot;
    HuVecF vec;
    Mtx *dirA;
    Mtx *dirB;
    Mtx rotMtx;
    Mtx mtxA;
    Mtx mtxB;
    Mtx mtxC;

    camWork = sys->camera;
    cam = &camWork->camera[0];
    model = sys->model;
    MdMathMatrixRotRPY(&rotMtx, model->rotP[2]);
    MdMathMtxTrans(&rotMtx, &mtxA, model->posP[2]);
    MdMathMatrixRotRPY(&mtxB, &lbl_1_data_7C);
    for(i=0; i<7; i++) {
        pos = model->posP[i*3+3];
        rot = model->rotP[i*3+3];
        lbl_1_bss_2F0[i].scrPos = *pos;
        vec = lbl_1_data_28[i];
        vec.y += 200.0f;
        MdMathMtxTrans(&mtxB, &mtxC, &vec);
        PSMTXConcat(mtxA, mtxC, mtxC);
        Hu3DMtxTransGet(mtxC, &lbl_1_bss_2F0[i].pos);
        lbl_1_bss_2F0[i].rot = *rot;
        vec = lbl_1_data_28[i];
        vec.x -= 70.0f;
        vec.y += 150.0f;
        vec.z -= 200.0f;
        MdMathMtxTrans(&mtxB, &mtxC, &vec);
        PSMTXConcat(mtxA, mtxC, mtxC);
        Hu3DMtxTransGet(mtxC, &lbl_1_bss_2F0[i].fingerPos);
        dirA = MdMathMatrixDir(&lbl_1_bss_2F0[i].fingerPos, &cam->pos);
        Hu3DMtxRotGet(*dirA, &lbl_1_bss_2F0[i].fingerRot);
    }
    for(i=0; i<4; i++) {
        lbl_1_bss_200[i].scrPos.x = 168.0f+(80.0f*(float)i);
        lbl_1_bss_200[i].scrPos.y = 340.0f;
        lbl_1_bss_200[i].scrPos.z = 2000.0f;
        Hu3D2Dto3D(&lbl_1_bss_200[i].scrPos, cam->bit, &lbl_1_bss_200[i].pos);
        dirB = MdMathMatrixDir(&lbl_1_bss_200[i].pos, &cam->pos);
        Hu3DMtxRotGet(*dirB, &lbl_1_bss_200[i].rot);
        lbl_1_bss_200[i].fingerPos = lbl_1_data_384[i];
        dirB = MdMathMatrixDir(&lbl_1_bss_200[i].pos, &lbl_1_bss_200[i].fingerPos);
        Hu3DMtxRotGet(*dirB, &lbl_1_bss_200[i].fingerRot);
    }
}

// remaining diff: operand order of i*3+j+3 index math
void fn_1_84AC(void)
{
    int i;
    int j;
    int idx;
    MDCARD_SLOT *slot;
    MDMODEL_WORK *model;
    MDCAMERA_WORK *camWork;
    HuVecF *pos[3];
    HuVecF *rot[3];
    HuVecF *scale[3];

    camWork = sys->camera;
    model = sys->model;
    slot = &sys->unk9C[0];
    for(i=0; i<7; i++, slot++) {
        for(j=0; j<3; j++) {
            pos[j] = model->posP[i*3+j+3];
            rot[j] = model->rotP[i*3+j+3];
            scale[j] = model->scaleP[i*3+j+3];
        }
        switch(slot->state) {
            case 1:
                slot->t[0] = 0.0f;
                slot->t[1] = 0.0f;
                slot->out[0] = 0.0f;
                slot->out[1] = 0.0f;
                slot->fromPos.x = pos[0]->x;
                slot->fromPos.y = pos[0]->y;
                slot->fromPos.z = pos[0]->z;
                slot->toPos.x = lbl_1_bss_2F0[i].pos.x;
                slot->toPos.y = lbl_1_bss_2F0[i].pos.y;
                slot->toPos.z = lbl_1_bss_2F0[i].pos.z;
                slot->state = 2;
                break;

            case 2:
                MdMathOscillate(MDMATH_OSC_90, &slot->t[0], &slot->out[0], 8.0f);
                MdMathOscillate(MDMATH_OSC_180, &slot->t[1], &slot->out[1], 8.0f);
                MdMathLerpOfs(&pos[0]->x, slot->fromPos.x, slot->toPos.x, slot->out[0], 0.75f*slot->out[1]);
                MdMathLerpOfs(&pos[0]->y, slot->fromPos.y, slot->toPos.y, slot->out[0], 0.75f*slot->out[1]);
                MdMathLerpOfs(&pos[0]->z, slot->fromPos.z, slot->toPos.z, slot->out[0], 0.75f*slot->out[1]);
                for(j=1; j<3; j++) {
                    *pos[j] = *pos[0];
                }
                if(slot->t[0] >= 1.0f) {
                    slot->state = 3;
                }
                break;

            case 4:
                slot->t[0] = 0.0f;
                slot->t[1] = 0.0f;
                slot->out[0] = 0.0f;
                slot->out[1] = 0.0f;
                slot->fromPos.x = pos[0]->x;
                slot->fromPos.y = pos[0]->y;
                slot->fromPos.z = pos[0]->z;
                slot->toPos.x = lbl_1_bss_2F0[i].scrPos.x;
                slot->toPos.y = lbl_1_bss_2F0[i].scrPos.y;
                slot->toPos.z = lbl_1_bss_2F0[i].scrPos.z;
                slot->state = 5;
                break;

            case 5:
                MdMathOscillate(MDMATH_OSC_90, &slot->t[0], &slot->out[0], 8.0f);
                MdMathLerp(&pos[0]->x, slot->fromPos.x, slot->toPos.x, slot->out[0]);
                MdMathLerp(&pos[0]->y, slot->fromPos.y, slot->toPos.y, slot->out[0]);
                MdMathLerp(&pos[0]->z, slot->fromPos.z, slot->toPos.z, slot->out[0]);
                for(j=1; j<3; j++) {
                    *pos[j] = *pos[0];
                }
                if(slot->t[0] >= 1.0f) {
                    slot->state = 6;
                }
                break;

            case 7:
                slot->t[0] = 0.0f;
                slot->t[1] = 0.0f;
                slot->out[0] = 0.0f;
                slot->out[1] = 0.0f;
                slot->fromPos.x = pos[0]->x;
                slot->fromPos.y = pos[0]->y;
                slot->fromPos.z = pos[0]->z;
                slot->toPos.x = lbl_1_bss_200[sys->turn].pos.x;
                slot->toPos.y = lbl_1_bss_200[sys->turn].pos.y;
                slot->toPos.z = lbl_1_bss_200[sys->turn].pos.z;
                slot->fromRot.x = rot[0]->x;
                slot->fromRot.y = rot[0]->y;
                slot->fromRot.z = rot[0]->z;
                slot->toRot.x = lbl_1_bss_200[sys->turn].rot.x;
                slot->toRot.y = lbl_1_bss_200[sys->turn].rot.y;
                slot->toRot.z = lbl_1_bss_200[sys->turn].rot.z;
                slot->state = 8;
                break;

            case 8:
                MdMathOscillate(MDMATH_OSC_90, &slot->t[0], &slot->out[0], 25.0f);
                MdMathOscillate(MDMATH_OSC_180, &slot->t[1], &slot->out[1], 25.0f);
                MdMathLerp(&pos[0]->x, slot->fromPos.x, slot->toPos.x, slot->out[0]);
                MdMathLerp(&pos[0]->y, slot->fromPos.y, slot->toPos.y, slot->out[0]);
                MdMathLerp(&pos[0]->z, slot->fromPos.z, slot->toPos.z, slot->out[0]);
                MdMathLerp(&rot[0]->x, slot->fromRot.x, slot->toRot.x, slot->out[0]);
                MdMathLerp(&rot[0]->y, slot->fromRot.y, slot->toRot.y, slot->out[0]);
                MdMathLerp(&rot[0]->z, slot->fromRot.z, slot->toRot.z, slot->out[0]);
                for(j=1; j<3; j++) {
                    *pos[j] = *pos[0];
                    *rot[j] = *rot[0];
                }
                if(slot->t[0] >= 1.0f) {
                    slot->state = 9;
                }
                break;

            case 10:
                slot->t[0] = 0.0f;
                slot->t[1] = 0.0f;
                slot->out[0] = 0.0f;
                slot->out[1] = 0.0f;
                slot->fromPos.x = pos[0]->x;
                slot->fromPos.y = pos[0]->y;
                slot->fromPos.z = pos[0]->z;
                slot->toPos.x = lbl_1_bss_2F0[i].pos.x;
                slot->toPos.y = lbl_1_bss_2F0[i].pos.y;
                slot->toPos.z = lbl_1_bss_2F0[i].pos.z;
                slot->fromRot.x = rot[0]->x;
                slot->fromRot.y = rot[0]->y;
                slot->fromRot.z = rot[0]->z;
                slot->toRot.x = lbl_1_bss_2F0[i].rot.x;
                slot->toRot.y = lbl_1_bss_2F0[i].rot.y;
                slot->toRot.z = lbl_1_bss_2F0[i].rot.z;
                slot->state = 11;
                break;

            case 11:
                MdMathOscillate(MDMATH_OSC_90, &slot->t[0], &slot->out[0], 25.0f);
                MdMathOscillate(MDMATH_OSC_180, &slot->t[1], &slot->out[1], 25.0f);
                MdMathLerp(&pos[0]->x, slot->fromPos.x, slot->toPos.x, slot->out[0]);
                MdMathLerp(&pos[0]->y, slot->fromPos.y, slot->toPos.y, slot->out[0]);
                MdMathLerp(&pos[0]->z, slot->fromPos.z, slot->toPos.z, slot->out[0]);
                MdMathLerp(&rot[0]->x, slot->fromRot.x, slot->toRot.x, slot->out[0]);
                MdMathLerp(&rot[0]->y, slot->fromRot.y, slot->toRot.y, slot->out[0]);
                MdMathLerp(&rot[0]->z, slot->fromRot.z, slot->toRot.z, slot->out[0]);
                for(j=1; j<3; j++) {
                    *pos[j] = *pos[0];
                    *rot[j] = *rot[0];
                }
                if(slot->t[0] >= 1.0f) {
                    slot->state = 12;
                }
                break;

            case 13:
                idx = 0;
                for(j=0; j<4; j++) {
                    if(i == sys->plSlot[j]) {
                        idx = j;
                        break;
                    }
                }
                slot->t[0] = 0.0f;
                slot->t[1] = 0.0f;
                slot->out[0] = 0.0f;
                slot->out[1] = 0.0f;
                slot->fromPos.x = pos[0]->x;
                slot->fromPos.y = pos[0]->y;
                slot->fromPos.z = pos[0]->z;
                slot->toPos.x = lbl_1_bss_200[idx].fingerPos.x;
                slot->toPos.y = lbl_1_bss_200[idx].fingerPos.y;
                slot->toPos.z = lbl_1_bss_200[idx].fingerPos.z;
                slot->fromRot.x = rot[0]->x;
                slot->fromRot.y = rot[0]->y;
                slot->fromRot.z = rot[0]->z;
                slot->toRot.x = lbl_1_bss_200[idx].fingerRot.x;
                slot->toRot.y = lbl_1_bss_200[idx].fingerRot.y;
                slot->toRot.z = lbl_1_bss_200[idx].fingerRot.z;
                slot->state = 14;
                break;

            case 14:
                MdMathOscillate(MDMATH_OSC_90_REV, &slot->t[0], &slot->out[0], 64.0f);
                MdMathLerp(&pos[0]->x, slot->fromPos.x, slot->toPos.x, slot->out[0]);
                MdMathLerp(&pos[0]->y, slot->fromPos.y, slot->toPos.y, slot->out[0]);
                MdMathLerp(&pos[0]->z, slot->fromPos.z, slot->toPos.z, slot->out[0]);
                MdMathLerp(&rot[0]->x, slot->fromRot.x, slot->toRot.x, slot->out[0]);
                MdMathLerp(&rot[0]->y, slot->fromRot.y, slot->toRot.y, slot->out[0]);
                MdMathLerp(&rot[0]->z, slot->fromRot.z, slot->toRot.z, slot->out[0]);
                for(j=1; j<3; j++) {
                    *pos[j] = *pos[0];
                    *rot[j] = *rot[0];
                }
                if(slot->t[0] >= 1.0f) {
                    slot->state = 15;
                }
                break;
        }
    }
}

void fn_1_9100(void)
{
    int i;
    MDCAMERA_WORK *camWork = sys->camera;
    MDMODEL_WORK *model = sys->model;
    MDSPRITE_WORK *sprite = sys->sprite;
    HUSPRGRPID grpId = sprite->grpId[2];
    MDCARD_CELL *cell;
    HuVec2f *scale;

    while(1) {
        camWork = sys->camera;
        model = sys->model;
        sprite = sys->sprite;
        grpId = sprite->grpId[2];
        HuSprGrpPosSet(grpId, 0.0f, 0.0f);
        cell = &sys->unk33C[0];
        for(i=0; i<4; i++, cell++) {
            scale = MdSpriteScaleGet(sprite, 2, i);
            switch(cell->state) {
                case 0:
                case 3:
                case 6:
                    break;

                case 1:
                    HuSprAttrReset(grpId, i, HUSPR_ATTR_DISPOFF);
                    HuSprPosSet(grpId, i, lbl_1_bss_200[i].scrPos.x, lbl_1_bss_200[i].scrPos.y-64.0f);
                    scale->x = 0.0f;
                    scale->y = 0.0f;
                    cell->t[0] = 0.0f;
                    cell->t[1] = 0.0f;
                    cell->out[0] = 0.0f;
                    cell->out[1] = 0.0f;
                    cell->state = 2;
                    break;

                case 2:
                    MdMathOscillate(MDMATH_OSC_90, &cell->t[0], &cell->out[0], 15.0f);
                    MdMathOscillate(MDMATH_OSC_180, &cell->t[1], &cell->out[1], 15.0f);
                    MdMathLerpOfs(&scale->x, 0.0f, 1.0f, cell->out[0], 0.5f*cell->out[1]);
                    scale->y = scale->x;
                    if(cell->t[0] >= 1.0f) {
                        cell->state = 3;
                    }
                    break;

                case 4:
                    cell->t[0] = 0.0f;
                    cell->t[1] = 0.0f;
                    cell->out[0] = 0.0f;
                    cell->out[1] = 0.0f;
                    cell->state = 5;
                    break;

                case 5:
                    MdMathOscillate(MDMATH_OSC_90, &cell->t[0], &cell->out[0], 15.0f);
                    MdMathLerp(&scale->x, 1.0f, 0.0f, cell->out[0]);
                    scale->y = scale->x;
                    if(cell->t[0] >= 1.0f) {
                        HuSprAttrSet(grpId, i, HUSPR_ATTR_DISPOFF);
                        cell->state = 6;
                    }
                    break;
            }
        }
        HuPrcVSleep();
    }
}

void fn_1_942C(s32 modelNo, s32 cameraNo)
{
    HuVecF d;
    MDCAMERA *cam;
    HuVecF *pos;
    MDMODEL_WORK *model;
    HuVecF *rot;
    MDCAMERA_WORK *camWork;
    float out;

    camWork = sys->camera;
    cam = &camWork->camera[cameraNo];
    model = sys->model;
    pos = model->posP[modelNo];
    rot = model->rotP[modelNo];
    d.x = cam->pos.x - pos->x;
    d.y = cam->pos.y - pos->y;
    d.z = cam->pos.z - pos->z;
    out = (float)(atan2(d.x, d.z)/M_PI*180.0);
    rot->y = out;
}

void fn_1_951C(void)
{
    int i;
    u16 stat;
    u8 tmp[4] = { 0, 0, 0, 0 };

    sys->selNum = 0;
    for(i=0; i<4; i++) {
        sys->padList[i] = -1;
    }
    for(i=0; i<4; i++) {
        stat = HuPadStatGet(i);
        if(stat == 0) {
            sys->padList[sys->selNum] = i;
            sys->padConn[i] = 1;
            sys->selNum++;
        } else {
            sys->padConn[i] = 0;
        }
    }
    if(sys->padConn[0] == 0) {
        for(i=0; i<4; i++) {
            sys->padList[i] = -1;
            sys->padConn[i] = 0;
        }
        sys->padList[0] = 0;
        sys->padConn[0] = 1;
        sys->selNum = 1;
    }
    sys->curSel = sys->selNum-1;
}

// remaining diff: one extra callee-saved register in the original
void fn_1_9714(void)
{
    int i;
    HUSPRGRPID grpId;

    grpId = sys->sprite->grpId[2];
    for(i=0; i<4; i++) {
        sys->plChr[i] = sys->padList[i];
        if(i > sys->curSel) {
            sys->plChr[i] = -1;
        }
    }
    for(i=0; i<4; i++) {
        sys->padUse[i] = 0;
    }
    for(i=0; i<4; i++) {
        if(sys->plChr[i] != -1) {
            sys->padUse[sys->plChr[i]] = 1;
        }
    }
    for(i=0; i<4; i++) {
        if(sys->plChr[i] != -1) {
            HuSprBankSet(grpId, i, sys->plChr[i]);
        } else {
            HuSprBankSet(grpId, i, 4);
        }
    }
}

// remaining diff: operand order of i*3+j+3
void fn_1_98C8(void)
{
    int i;
    int j;
    MDCARD_SLOT *slot;
    MDMODEL_WORK *model;
    HuVecF *pos;
    HuVecF *rot;
    HuVecF *rotC;
    Mtx rotMtx;
    Mtx mtxA;
    Mtx mtxB;
    Mtx mtxC;

    model = sys->model;
    pos = model->posP[2];
    rotC = model->rotP[2];
    MdMathMatrixRotRPY(&rotMtx, rotC);
    MdMathMtxTrans(&rotMtx, &mtxA, pos);
    slot = &sys->unk9C[0];
    for(i=0; i<7; i++, slot++) {
        if(slot->state != 9 && slot->state != 13 && slot->state != 14 && slot->state != 15) {
            MdMathMatrixRotRPY(&mtxB, &lbl_1_data_7C);
            MdMathMtxTrans(&mtxB, &mtxC, &lbl_1_data_28[i]);
            PSMTXConcat(mtxA, mtxC, mtxC);
            for(j=0; j<3; j++) {
                rot = model->rotP[i*3+j+3];
                Hu3DMtxTransGet(mtxC, model->posP[i*3+j+3]);
                Hu3DMtxRotGet(mtxC, rot);
            }
            slot->savePos = *model->posP[i*3+3];
            slot->saveRot = *model->rotP[i*3+3];
        }
    }
}

s32 fn_1_9A70(void)
{
    s32 ret;

    ret = -1;
    if(sys->plChr[sys->turn] != -1) {
        if((sys->padWork->DStkDown[sys->plChr[sys->turn]] & 1) != 0) {
            ret = 0;
        } else if((sys->padWork->DStkDown[sys->plChr[sys->turn]] & 2) != 0) {
            ret = 1;
        } else if((HuPadBtnDown[sys->plChr[sys->turn]] & 0x200) != 0) {
            ret = 2;
        } else if((HuPadBtnDown[sys->plChr[sys->turn]] & 0x100) != 0) {
            ret = 3;
        }
    } else if(sys->counter % 15 == 0) {
        lbl_1_bss_4++;
        lbl_1_bss_4 %= 8;
        if(lbl_1_bss_4 == 0) {
            ret = 3;
        } else {
            ret = frandmod(2);
        }
    }
    return ret;
}

void fn_1_9CC8(s32 dir)
{
    int i;
    MDCARD_SLOT *slot;
    s8 avail[7];

    slot = &sys->unk9C[0];
    for(i=0; i<7; i++, slot++) {
        if(slot->state == 9) {
            avail[i] = 0;
        } else {
            avail[i] = 1;
        }
    }
    for(i=0; i<7; i++) {
        if(dir == 0) {
            if(--sys->plSlot[sys->turn] == -1) {
                sys->plSlot[sys->turn] = 6;
            }
        } else if(dir == 1) {
#ifdef TARGET_PC
            sys->plSlot[sys->turn]++;
            sys->plSlot[sys->turn] = sys->plSlot[sys->turn] % 7;
#else
            sys->plSlot[sys->turn] = (++sys->plSlot[sys->turn]) % 7;
#endif
        }
        if(avail[sys->plSlot[sys->turn]] == 1) {
            break;
        }
    }
}

void fn_1_9E88(void)
{
    int i;
    s8 next;
    s8 sel[4];

#ifdef TARGET_PC
    next = 0;
#endif
    for(i=0; i<4; i++) {
        sel[i] = -1;
    }
    for(i=0; i<4; i++) {
        if(sys->plChr[i] != -1) {
            sel[sys->plChr[i]] = sys->plSlot[i];
        } else {
            next = i;
            break;
        }
    }
    for(i=0; i<4; i++) {
        if(sel[i] == -1) {
            sel[i] = sys->plSlot[next];
            next++;
        }
    }
    for(i=0; i<4; i++) {
        GwPlayerConf[i].charNo = sel[i];
        if(sys->padUse[i] == 0) {
            GwPlayerConf[i].type = 1;
        } else {
            GwPlayerConf[i].type = 0;
        }
        GwPlayerConf[i].padNo = i;
        GwCardCommon.pedestalF[i] = sys->unk3B[i];
        GwCardCommon.starNum[i] = 0;
        GwCardCommon.ztarNum[i] = 0;
    }
    GwCardCommon.boardSize = sys->selMode;
}

// jump table here; original is a compare tree (remaining diff)
void fn_1_A094(s32 no)
{
    s16 pan;

    switch(no) {
        case 0:
            bgmStream = HuAudBGMPlay(39);
            break;

        case 1:
            HuAudSStreamFadeOut(bgmStream, 1000);
            break;

        case 2:
            HuAudFXPlay(0);
            break;

        case 3:
            HuAudFXPlay(2);
            break;

        case 4:
            HuAudFXPlay(1);
            break;

        case 5:
            HuAudFXPlay(3);
            break;

        case 6:
            HuAudFXPlay(1021);
            break;

        case 7:
            HuAudFXPlay(1039);
            break;

        case 8:
            HuAudFXPlay(1014);
            break;

        case 9:
            HuAudFXPlay(1137);
            break;

        case 10:
            HuAudFXPlay(1138);
            break;

        case 11:
        case 12:
            pan = fn_1_A204(0, 1);
            if(no == 11) {
                HuAudFXPlayPan(754, pan);
            }
            break;

        case 13:
            HuAudAllStop();
            break;
    }
}

s32 fn_1_A204(s32 modelNo, s32 cameraNo)
{
    HuVecF dst;
    u16 *bit;
    MDMODEL_WORK *model;
    MDCAMERA_WORK *camWork;
    HuVecF *pos;
    s32 ret;

    camWork = sys->camera;
    bit = &camWork->camera[cameraNo].bit;
    model = sys->model;
    pos = model->posP[modelNo];
    Hu3D3Dto2D(pos, *bit, &dst);
    ret = (s32)(32.0f+(1.0f/9.0f)*dst.x);
    return ret;
}

static HuVecF lbl_1_data_410 = { 0.0f, 1.0f, 0.0f };

void fn_1_A2BC(s32 modelNo, s32 cameraNo)
{
    HuVecF camPos;
    HuVecF mdlPos;
    HuVecF dir;
    HuVecF cross2;
    HuVecF cross;
    Mtx mtx;
    HuVecF rotOut;
    MDMODEL_WORK *model;

    model = sys->model;
    camPos = sys->camera->camera[cameraNo].pos;
    mdlPos = *model->posP[modelNo];
    VECSubtract(&camPos, &mdlPos, &dir);
    VECNormalize(&dir, &dir);
    VECCrossProduct(&lbl_1_data_410, &dir, &cross);
    VECNormalize(&cross, &cross);
    VECCrossProduct(&dir, &cross, &cross2);
    VECNormalize(&cross2, &cross2);
    mtx[0][1] = cross2.x;
    mtx[1][1] = cross2.y;
    mtx[2][1] = cross2.z;
    mtx[0][0] = cross.x;
    mtx[1][0] = cross.y;
    mtx[2][0] = cross.z;
    mtx[0][2] = dir.x;
    mtx[1][2] = dir.y;
    mtx[2][2] = dir.z;
    Hu3DMtxRotGet(mtx, &rotOut);
    model->rotP[modelNo]->x = rotOut.x;
    model->rotP[modelNo]->y = rotOut.y;
    model->rotP[modelNo]->z = rotOut.z;
}
static MDL_INFO mdlInfo[39] = {
    { "STAR", 0 },
    { "STAGE", 0 },
    { "BOX", 0 },
    { "PIN0_MARIO", 0 },
    { "PIN1_MARIO", 0 },
    { "PIN2_MARIO", 0 },
    { "PIN0_LUIGI", 0 },
    { "PIN1_LUIGI", 0 },
    { "PIN2_LUIGI", 0 },
    { "PIN0_PEACH", 0 },
    { "PIN1_PEACH", 0 },
    { "PIN2_PEACH", 0 },
    { "PIN0_YOSHI", 0 },
    { "PIN1_YOSHI", 0 },
    { "PIN2_YOSHI", 0 },
    { "PIN0_WARIO", 0 },
    { "PIN1_WARIO", 0 },
    { "PIN2_WARIO", 0 },
    { "PIN0_DAISY", 0 },
    { "PIN1_DAISY", 0 },
    { "PIN2_DAISY", 0 },
    { "PIN0_WALUIGI", 0 },
    { "PIN1_WALUIGI", 0 },
    { "PIN2_WALUIGI", 0 },
    { "MAN0", 0 },
    { "MAN1", 0 },
    { "MAN2", 0 },
    { "MAN3", 0 },
    { "COM0", 0 },
    { "COM1", 0 },
    { "COM2", 0 },
    { "COM3", 0 },
    { "FINGER", 0 },
    { "CARD_ROOT", 0 },
    { "CARD_ITEM", 0 },
    { "CARD_EVENT", 0 },
    { "CARD_KOOPA", 0 },
    { "CARD_STAR", 1 },
    { "FONTBGFILTER", 0 }
};

#define MDADJ_STICK(p, v) \
    if(HuPadSubStkX[0] <= -29) { \
        p->x -= v; \
    } else if(HuPadSubStkX[0] >= 29) { \
        p->x += v; \
    } \
    if(HuPadStkY[0] <= -36) { \
        p->y -= v; \
    } else if(HuPadStkY[0] >= 36) { \
        p->y += v; \
    } \
    if(HuPadSubStkY[0] <= -29) { \
        p->z += v; \
    } else if(HuPadSubStkY[0] >= 29) { \
        p->z -= v; \
    }

// remaining diff: a dead mr r24 copy, extsh on camera bit
void MdPosAdjustMode(void)
{
    int i;
    u32 attr;
    HuVecF *pos;
    HuVecF *rot;
    HuVecF *scale;
    MDDEBUGFONT_WORK *font;
    MDMODEL_WORK *model;
    HUSPRGRPID grpId;
    MDSPRITE_WORK *sprite;
    MDCAMERA_WORK *camWork;
    HuVecF scr;

    font = sys->debugFont;
    camWork = sys->camera;
    sprite = sys->sprite;
    model = sys->model;
    grpId = sprite->grpId[3];
    if(font->ctrl != MD_DEBUGFONT_CTRL_MODEL) {
        return;
    }
#ifdef TARGET_PC
    pos = model->posP[mdlSel];
    rot = model->rotP[mdlSel];
    scale = model->scaleP[mdlSel];
    attr = Hu3DModelAttrGet(model->mdlId[mdlSel]);
#endif
    if(showF == 0) {
        if((HuPadBtn[0] & 0x400) != 0) {
            if(++dispTimer >= 60) {
                MdSpriteDispSet(sprite, 3, TRUE);
                dispTimer = 0;
                showF = 1;
            }
        } else {
            dispTimer = 0;
        }
    } else if(showF == 1) {
        while(1) {
            if((HuPadBtn[0] & 0x400) != 0) {
                if(++dispTimer >= 60) {
                    MdSpriteDispSet(sprite, 3, FALSE);
                    dispTimer = 0;
                    showF = 0;
                    break;
                }
            } else {
                dispTimer = 0;
            }
            if(font->ctrl == MD_DEBUGFONT_CTRL_MODEL) {
                if((sys->padWork->DStkDown[0] & 1) != 0) {
                    for(i=0; i<39; i++) {
                        if(--mdlSel == -1) {
                            mdlSel = 38;
                        }
                        if((Hu3DModelAttrGet(model->mdlId[mdlSel]) & 1) == 0) {
                            break;
                        }
                    }
                } else if((sys->padWork->DStkDown[0] & 2) != 0) {
                    for(i=0; i<39; i++) {
#ifdef TARGET_PC
                        mdlSel++;
                        mdlSel = mdlSel % 39;
#else
                        mdlSel = (++mdlSel) % 39;
#endif
                        if((Hu3DModelAttrGet(model->mdlId[mdlSel]) & 1) == 0) {
                            break;
                        }
                    }
                }
                if((HuPadBtnDown[0] & 4) != 0) {
#ifdef TARGET_PC
                    menuSel++;
                    menuSel = menuSel % 3;
#else
                    menuSel = (++menuSel) % 3;
#endif
                }
                mdlInfoP = &mdlInfo[mdlSel];
                pos = model->posP[mdlSel];
                rot = model->rotP[mdlSel];
                scale = model->scaleP[mdlSel];
                attr = Hu3DModelAttrGet(model->mdlId[mdlSel]);
                if(menuSel == 0) {
                        if(HuPadTrigL[0] <= 75) {
                            MDADJ_STICK(pos, 5.0f)
                        } else if((HuPadBtn[0] & 0x40) != 0) {
                            MDADJ_STICK(pos, 0.5f)
                        } else {
                            MDADJ_STICK(pos, 1.0f)
                        }
                } else if(menuSel == 1) {
                        if(HuPadTrigL[0] <= 75) {
                            MDADJ_STICK(rot, 10.0f)
                        } else if((HuPadBtn[0] & 0x40) != 0) {
                            MDADJ_STICK(rot, 1.0f)
                        } else {
                            MDADJ_STICK(rot, 5.0f)
                        }
                } else if(menuSel == 2) {
                        if(HuPadTrigL[0] <= 75) {
                            if(HuPadSubStkX[0] <= -29) {
                                scale->x -= 0.05f;
                                scale->y -= 0.05f;
                                scale->z -= 0.05f;
                            } else if(HuPadSubStkX[0] >= 29) {
                                scale->x += 0.05f;
                                scale->y += 0.05f;
                                scale->z += 0.05f;
                            }
                        } else if((HuPadBtn[0] & 0x40) != 0) {
                            MDADJ_STICK(scale, 0.01f)
                        } else {
                            MDADJ_STICK(scale, 0.1f)
                        }
                }
            }
            HuSprPosSet(grpId, 0, 32.0f, 0.0f);
            HuSprPosSet(grpId, 1, -32.0f, 0.0f);
            sprRot = sprRot+2.0f;
            MdMathAngleClamp(&sprRot);
            HuSprGrpZRotSet(grpId, sprRot);
            Hu3D3Dto2D(pos, camWork->camera[mdlInfoP->cameraNo].bit, &scr);
            HuSprGrpPosSet(grpId, scr.x, scr.y);
            if((attr & 1) != 0) {
                HuSprAttrSet(grpId, 0, 0x80);
                HuSprAttrSet(grpId, 1, 0x80);
                HuSprGrpTPLvlSet(grpId, 0.4f);
            } else {
                HuSprAttrReset(grpId, 0, 0x80);
                HuSprAttrReset(grpId, 1, 0x80);
                HuSprGrpTPLvlSet(grpId, 1.0f);
            }
            MdDebugFontPrint(font, "POS-ADJUST-MODE(%2d / %2d) -> %s", mdlSel, 39, mdlInfoP->name);
            if(font->ctrl == MD_DEBUGFONT_CTRL_MODEL) {
                if(menuSel == 0) {
                    MdDebugFontPrint(font, "*  POS(%.1f / %.1f / %.1f)", pos->x, pos->y, pos->z);
                } else {
                    MdDebugFontPrint(font, "   POS(%.1f / %.1f / %.1f)", pos->x, pos->y, pos->z);
                }
                if(menuSel == 1) {
                    MdDebugFontPrint(font, "*  ROT(%.1f / %.1f / %.1f)", rot->x, rot->y, rot->z);
                } else {
                    MdDebugFontPrint(font, "   ROT(%.1f / %.1f / %.1f)", rot->x, rot->y, rot->z);
                }
                if(menuSel == 2) {
                    MdDebugFontPrint(font, "*SCALE(%.2f / %.2f / %.2f)", scale->x, scale->y, scale->z);
                } else {
                    MdDebugFontPrint(font, " SCALE(%.2f / %.2f / %.2f)", scale->x, scale->y, scale->z);
                }
            }
            HuPrcVSleep();
        }
    }
}
