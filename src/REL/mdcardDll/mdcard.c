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
s32 fn_1_4F80(s32 arg0);
s32 fn_1_61F8(s32 arg0);
s32 fn_1_6B0C(s32 arg0);
s32 fn_1_71B8(s32 arg0);
s32 fn_1_78C4(s32 arg0);
void fn_1_7DA0(void);
void fn_1_84AC(void);
void fn_1_9100(void);
void fn_1_98C8(void);
void fn_1_951C(void);
void fn_1_9714(void);
void fn_1_9CC8(s32 arg0);
s32 fn_1_9A70(void);
void fn_1_9E88(void);
void fn_1_80A8(s32 arg0);
void fn_1_A2BC(s32 arg0, s32 arg1);
void MdPosAdjustMode(void);
void fn_1_A094(s32 no);
s32 fn_1_A204(s32 modelNo, s32 cameraNo);

void (*MdFlowTbl[6])(void) = {
    fn_1_DD8, fn_1_1058, fn_1_1448, fn_1_1DF0, fn_1_22B8, fn_1_2A3C
};

s32 (*MdProcTbl[10])() = {
    fn_1_44F8, fn_1_461C, fn_1_4944, fn_1_4C00, fn_1_4DC8, fn_1_4F80, fn_1_61F8, fn_1_6B0C, fn_1_71B8, fn_1_78C4
};

static s32 bgmStream;
static s32 lbl_1_bss_1F8;
static s32 lbl_1_bss_1FC;
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
    sys->unk38C = 0;
    sys->unk390 = 1;
    sys->unk5C = HuPrcChildCreate(fn_1_7DA0, 109, 12288, 0, sys->objman);
    for(cell=&sys->unk33C[0], i=0; i<4; i++, cell++) {
        cell->unk0 = 0;
    }
    sys->unk58 = HuPrcChildCreate(fn_1_9100, 108, 4096, 0, sys->objman);
    for(slot=&sys->unk9C[0], j=0; j<7; j++, slot++) {
        slot->unk0 = 0;
        slot->unk4 = 0;
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
            sys->unk390 = 2;
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
