#include "REL/partyresultdll.h"

static s8 teamNoTbl[9*10] = {
    -1, 0, 1, 2, 3, 4, 5, 6, 7, 8,
    -1, -1, 9, 10, 11, 12, 13, 14, 15, 16,
    -1, -1, -1, 17, 18, 19, 20, 21, 22, 23,
    -1, -1, -1, -1, 24, 25, 26, 27, 28, 29,
    -1, -1, -1, -1, -1, 30, 31, 32, 33, 34,
    -1, -1, -1, -1, -1, -1, 35, 36, 37, 38,
    -1, -1, -1, -1, -1, -1, -1, 39, 40, 41,
    -1, -1, -1, -1, -1, -1, -1, -1, 42, 43,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, 44
};

u32 PResTeamNameMesGet(s16 charNo1, s16 charNo2)
{
    int teamNo;

    if(charNo1 > CHARNO_MINIKOOPA || charNo2 > CHARNO_MINIKOOPA) {
        return MESS_TEAMNAME_UNKNOWN;
    }
    if(charNo1 < charNo2) {
        teamNo = teamNoTbl[(charNo1*10)+charNo2];
    } else {
        teamNo = teamNoTbl[(charNo2*10)+charNo1];
    }
    if(teamNo == -1) {
        return MESS_TEAMNAME_UNKNOWN;
    } else {
        return MESS_TEAMNAME_START+teamNo;
    }
}

void PResBtnAWait(void)
{
    s16 i;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        if(GwPlayerConf[i].type == GW_TYPE_MAN) {
            break;
        }
    }
    if(i == GW_PLAYER_MAX) {
        HuPrcSleep(30);
    } else {
        while(1) {
            if(HuPadBtnDown[0] & PAD_BUTTON_A) {
                break;
            }
            HuPrcVSleep();
        }
    }
}

void PResComInputWait(void)
{
    s16 i;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        if(GwPlayerConf[i].type == GW_TYPE_MAN) {
            return;
        }
    }
    HuWinComKeyWait(PAD_BUTTON_A, PAD_BUTTON_A, PAD_BUTTON_A, PAD_BUTTON_A, 30);
    HuWinComKeyWait(PAD_BUTTON_A, PAD_BUTTON_A, PAD_BUTTON_A, PAD_BUTTON_A, 30);
    HuWinComKeyWait(PAD_BUTTON_A, PAD_BUTTON_A, PAD_BUTTON_A, PAD_BUTTON_A, 30);
    HuWinComKeyWait(PAD_BUTTON_A, PAD_BUTTON_A, PAD_BUTTON_A, PAD_BUTTON_A, 30);
    HuWinComKeyWait(PAD_BUTTON_A, PAD_BUTTON_A, PAD_BUTTON_A, PAD_BUTTON_A, 30);
}
