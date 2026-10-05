#include "REL/partyresultdll.h"

extern s16 Hu3DCameraNo;
extern u8 CharFXVol[];
extern u8 CharFXPan[];

extern s16 BonusFadeSize[];
extern void *BonusFadeAddr;

extern HuVecF PlayerPosTbl[4];
extern s16 TeamNameWinId[];

static void ResultCharFXPlay(s16 charNo, s16 seNo)
{
    CharFXPlayVolPan(charNo, seNo, CharFXVol[charNo], CharFXPan[charNo]);
}

static void BonusFadeFbCopy(void)
{
    if (Hu3DCameraNo != 1) {
        return;
    }

    Hu3DFbCopyExec(BonusFadeSize[0], BonusFadeSize[1], BonusFadeSize[2], BonusFadeSize[3], 0x4, 0, BonusFadeAddr);
}

static void TeamNamePosUpdate(void)
{
    s16 i;
    HuVecF pos3D;
    HuVecF pos2D;

    for (i = 0; i < 2; i++) {
        PSVECAdd(&pos3D, &PlayerPosTbl[i * 2], &PlayerPosTbl[i * 2 + 1]);
        PSVECScale(&pos3D, &pos3D, 0.5f);
        Hu3D3Dto2D(&pos3D, 4, &pos2D);
        HuWinPosSet(TeamNameWinId[i], pos2D.x, pos2D.y - 10.0f);
    }
}