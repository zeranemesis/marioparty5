#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/window.h"
#include "game/wipe.h"

#include <stdio.h>

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

void ObjectSetup(void);
void MesChkMainProc(void);
void MesChkExec(void);
void MesChkWinSizeGet(u32 messNum, HuVec2f *size);

static OMOBJMAN *objman;

static char lbl_1_data_0[] = "******* Message Checker *********\n";

static char lbl_1_data_23[] = "000_Chara_name.th";
static char lbl_1_data_35[] = "001_Staff.th";
static char lbl_1_data_42[] = "010_Sys_help.th";
static char lbl_1_data_52[] = "020_Mp_system.th";
static char lbl_1_data_63[] = "030_Mp_mode.th";
static char lbl_1_data_72[] = "035_Bord_Pause.th";
static char lbl_1_data_84[] = "040_PartyMode_sys.th";
static char lbl_1_data_99[] = "041_MAP_NAME.th";
static char lbl_1_data_A9[] = "042_TEAM_NAME.th";
static char lbl_1_data_BA[] = "043_Bord_Results.th";
static char lbl_1_data_CE[] = "044_Bord_Results2.th";
static char lbl_1_data_E3[] = "050_StoryMode_sys.th";
static char lbl_1_data_F8[] = "051_StoryMode_demo.th";
static char lbl_1_data_10E[] = "060_MgMode_sys.th";
static char lbl_1_data_120[] = "061_MgMode_free.th";
static char lbl_1_data_133[] = "062_MgMode_match.th";
static char lbl_1_data_147[] = "064_Mg_decathlon.th";
static char lbl_1_data_15B[] = "065_Mg_wars.th";
static char lbl_1_data_16A[] = "066_MgMode_to.th";
static char lbl_1_data_17B[] = "070_Mpcard_mess.th";
static char lbl_1_data_18E[] = "071_Mpcard_tutorial.th";
static char lbl_1_data_1A5[] = "072_Mpcard_sys.th";
static char lbl_1_data_1B7[] = "073_extra.th";
static char lbl_1_data_1C4[] = "075_Beach_volley.th";
static char lbl_1_data_1D8[] = "076_Ice_hockey.th";
static char lbl_1_data_1EA[] = "080_SD_G.th";
static char lbl_1_data_1F6[] = "081_SD_parts.th";
static char lbl_1_data_206[] = "082_SD_A.th";
static char lbl_1_data_212[] = "083_SD_D.th";
static char lbl_1_data_21E[] = "084_SD_machine.th";
static char lbl_1_data_230[] = "085_SD_B.th";
static char lbl_1_data_23C[] = "090_Mp_option.th";
static char lbl_1_data_24D[] = "091_Mp_mgpack.th";
static char lbl_1_data_25E[] = "092_Mp_selist.th";
static char lbl_1_data_26F[] = "200_Board_ope.th";
static char lbl_1_data_280[] = "201_Board_star.th";
static char lbl_1_data_292[] = "202_Board_capmachine.th";
static char lbl_1_data_2AA[] = "203_Board_battle.th";
static char lbl_1_data_2BE[] = "204_Board_pause.th";
static char lbl_1_data_2D1[] = "205_board_story.th";
static char lbl_1_data_2E4[] = "206_Board_gate.th";
static char lbl_1_data_2F6[] = "207_Board_name.th";
static char lbl_1_data_308[] = "208_Board_tutorial.th";
static char lbl_1_data_31E[] = "209_Board_mgcircuit.th";
static char lbl_1_data_335[] = "210_Board_opening.th";
static char lbl_1_data_34A[] = "220_Board_w01.th";
static char lbl_1_data_35B[] = "221_Board_w02.th";
static char lbl_1_data_36C[] = "222_Board_w03.th";
static char lbl_1_data_37D[] = "223_Board_w04.th";
static char lbl_1_data_38E[] = "224_Board_w05.th";
static char lbl_1_data_39F[] = "225_Board_w06.th";
static char lbl_1_data_3B0[] = "226_Board_w07.th";
static char lbl_1_data_3C1[] = "230_Capsule_Ex01.th";
static char lbl_1_data_3D5[] = "231_Capsule_Ex02.th";
static char lbl_1_data_3E9[] = "232_Capsule_Ex03.th";
static char lbl_1_data_3FD[] = "233_Capsule_Ex04.th";
static char lbl_1_data_411[] = "234_Capsule_Ex99.th";
static char lbl_1_data_425[] = "235_Capsule_Ex98.th";
static char lbl_1_data_439[] = "236_Last5Koopa.th";
static char lbl_1_data_44B[] = "237_KoopaMasu.th";
static char lbl_1_data_45C[] = "238_VSMasu.th";
static char lbl_1_data_46A[] = "500_mg_inst.th";
static char lbl_1_data_479[] = "540_mg.th";
static char lbl_1_data_483[] = "562_mg_last.th";
static char lbl_1_data_492[] = "598_mg_inst_sys.th";
static char lbl_1_data_4A5[] = "599_mg_name.th";
static char lbl_1_data_4B4[] = "neltya.th";
static char lbl_1_data_4BE[] = "saf_test.th";

static char *mesDirNameTbl[] = {
    lbl_1_data_23,  lbl_1_data_35,  lbl_1_data_42,  lbl_1_data_52,
    lbl_1_data_63,  lbl_1_data_72,  lbl_1_data_84,  lbl_1_data_99,
    lbl_1_data_A9,  lbl_1_data_BA,  lbl_1_data_CE,  lbl_1_data_E3,
    lbl_1_data_F8,  lbl_1_data_10E, lbl_1_data_120, lbl_1_data_133,
    lbl_1_data_147, lbl_1_data_15B, lbl_1_data_16A, lbl_1_data_17B,
    lbl_1_data_18E, lbl_1_data_1A5, lbl_1_data_1B7, lbl_1_data_1C4,
    lbl_1_data_1D8, lbl_1_data_1EA, lbl_1_data_1F6, lbl_1_data_206,
    lbl_1_data_212, lbl_1_data_21E, lbl_1_data_230, lbl_1_data_23C,
    lbl_1_data_24D, lbl_1_data_25E, lbl_1_data_26F, lbl_1_data_280,
    lbl_1_data_292, lbl_1_data_2AA, lbl_1_data_2BE, lbl_1_data_2D1,
    lbl_1_data_2E4, lbl_1_data_2F6, lbl_1_data_308, lbl_1_data_31E,
    lbl_1_data_335, lbl_1_data_34A, lbl_1_data_35B, lbl_1_data_36C,
    lbl_1_data_37D, lbl_1_data_38E, lbl_1_data_39F, lbl_1_data_3B0,
    lbl_1_data_3C1, lbl_1_data_3D5, lbl_1_data_3E9, lbl_1_data_3FD,
    lbl_1_data_411, lbl_1_data_425, lbl_1_data_439, lbl_1_data_44B,
    lbl_1_data_45C, lbl_1_data_46A, lbl_1_data_479, lbl_1_data_483,
    lbl_1_data_492, lbl_1_data_4A5, lbl_1_data_4B4, lbl_1_data_4BE,
    NULL,
};

static char lbl_1_data_5E0[] = "%d";
static char lbl_1_data_5E3[] = "Error: Message Dir Over\n";
static char lbl_1_data_5FC[] = "Error: Message Number Over\n";

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
    OSReport(lbl_1_data_0);
    objman = omInitObjMan(0x32, 0x2000);
    HuWinInit(0);
    HuPrcChildCreate(MesChkMainProc, 0x3e8, 0x3000, 0, objman);
}

void MesChkMainProc(void)
{
    WipeCreate(1, 0, 0x1e);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }

    MesChkExec();

    WipeCreate(2, 0, 0x1e);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }

    omOvlReturnEx(1, 1);
    HuPrcEnd();
    while (TRUE) {
        HuPrcVSleep();
    }
}

// NONMATCHING: MWCC allocates dirIdx/messNum across several overlapping
// registers between the direction/message browsing loops; this version keeps
// one variable per role instead of reusing r29/r30 the way the original does.
void MesChkExec(void)
{
    s16 dirIdx;
    s16 messIdx;
    s16 messMax;
    s16 prevDirLastMsgF;
    char *str;
    char *msgPtr;
    HUWINID dirWin;
    HUWINID numWin;
    HUWINID msgWin;
    HUWIN *win;
    u32 messNum;
    HuVec2f size;
    s32 hasCtrlCode;
    s16 activeKey;
    char numBuf[8];

    dirIdx = 0;
    while (mesDirNameTbl[dirIdx] != NULL) {
        str = mesDirNameTbl[dirIdx];
        while (*str != '\0') {
            if (*str == '_') {
                *str = '=';
            }
            if (*str == '.') {
                *str = '\0';
            }
            str++;
        }
        dirIdx++;
    }

    dirWin = HuWinCreate(478.0f, 32.0f, 0x52, 0x2a, 0);
    HuWinAttrSet(dirWin, HUWIN_ATTR_ALIGN_CENTER);
    HuWinMesSpeedSet(dirWin, 0);
    HuWinScaleSet(dirWin, 0.8f, 0.8f);

    numWin = HuWinCreate(24.0f, 32.0f, 0x20a, 0x2a, 0);
    HuWinMesSpeedSet(numWin, 0);
    HuWinScaleSet(numWin, 0.8f, 0.8f);

    dirIdx = 0;
    prevDirLastMsgF = 0;
    while (mesDirNameTbl[dirIdx] != NULL) {
        HuWinMesSet(numWin, (u32)(uintptr_t) mesDirNameTbl[dirIdx]);
        messMax = HuWinMesMaxNumGet(dirIdx << 16);

        if (prevDirLastMsgF == 0) {
            messIdx = 0;
        } else {
            prevDirLastMsgF = 0;
            messIdx = messMax - 1;
        }

        while (messIdx < messMax) {
            sprintf(numBuf, lbl_1_data_5E0, messIdx);
            HuWinMesSet(dirWin, (u32)(uintptr_t) numBuf);

            messNum = (dirIdx << 16) | (u16) messIdx;
            MesChkWinSizeGet(messNum, &size);

            if (size.x == 255.0f) {
                HuWinMesMaxSizeGet(1, &size, messNum);
            } else {
                size.x = (f32) (((s16) size.x * 0x15 + 0x1f) & 0xfff0);
                size.y = (f32) (((s16) size.y * 0x1a + 0x1f) & 0xfff0);
            }

            msgWin = HuWinCreate(HUWIN_POS_CENTER, 200.0f, (s16) size.x, (s16) size.y, 0);
            HuWinMesSpeedSet(msgWin, 0);

            hasCtrlCode = 0;
            msgPtr = HuWinMesPtrGet(messNum);
            while (*msgPtr != 0) {
                if ((u8) *msgPtr == 0xff) {
                    hasCtrlCode = 1;
                }
                msgPtr++;
            }

            win = &winData[msgWin];
            win->pushKey = PAD_BUTTON_START | PAD_BUTTON_A | PAD_BUTTON_B | PAD_TRIGGER_R | PAD_TRIGGER_L;
            HuWinMesSet(msgWin, messNum);

            while (win->stat != 0) {
                HuPadBtnDown[0] = HuPadBtnRep[0];
                HuPrcVSleep();
            }

            if (hasCtrlCode == 0) {
                while ((HuPadBtnRep[0] & (PAD_BUTTON_START | PAD_BUTTON_A | PAD_BUTTON_B | PAD_TRIGGER_R | PAD_TRIGGER_L)) == 0) {
                    HuPrcVSleep();
                }
                win->activePadKey = (s16) HuPadBtnRep[0];
            }

            activeKey = win->activePadKey;
            HuWinKill(msgWin);

            if (activeKey & PAD_BUTTON_START) {
                goto out;
            }

            if (activeKey & PAD_TRIGGER_R) {
                goto nextDir;
            }

            if (activeKey & PAD_TRIGGER_L) {
                dirIdx -= 2;
                if (dirIdx < 0) {
                    dirIdx = -1;
                }
                goto nextDir;
            }

            if (activeKey & PAD_BUTTON_B) {
                messIdx -= 2;
                if (messIdx < 0) {
                    dirIdx -= 2;
                    if (dirIdx < 0) {
                        dirIdx = -1;
                    }
                    prevDirLastMsgF = 1;
                    goto nextDir;
                }
            }

            HuPrcVSleep();
            messIdx++;
        }

    nextDir:
        dirIdx++;
        if (mesDirNameTbl[dirIdx] == NULL) {
            dirIdx = 0;
        }
        HuPrcVSleep();
    }

out:
    return;
}

void MesChkWinSizeGet(u32 messNum, HuVec2f *size)
{
    u32 dirNum;
    u32 msgNum;
    u32 *p;

    dirNum = messNum >> 16;
    msgNum = (u16) messNum;

    p = (u32 *) messDataPtr;
    if (dirNum >= *p) {
        OSReport(lbl_1_data_5E3);
    }

    p++;
    p += *(p + dirNum) >> 2;

    if (msgNum >= *p) {
        OSReport(lbl_1_data_5FC);
    }

    p++;
    p += *(p + msgNum) >> 2;

    size->x = (f32) (*p >> 16);
    size->y = (f32) (u16) *p;
}
