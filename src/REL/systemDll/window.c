#include "REL/systemDll/sysmd.h"

static void MdMessCreate(SYSMESS *mess);
static void MdMessMain(SYSMESS *mess);
static void MdMessDlgWinCreateInsert(s32 mesNo);
static void MdMessDlgWinKill(void);
static void MdMessDebug(SYSMESS *mess);

void MdMessProc(void)
{
    NEL_SYS *sys = &sysWork;
    SYSMESS *mess = (SYSMESS *)sys->mess;
    s32 *ctrl = &sys->blendDbg.stat;

    MdMessCreate(mess);
    while (sys->unk234 == -1) {
        HuPrcVSleep();
    }
    mess = (SYSMESS *)sys->mess;
    ctrl = &sys->blendDbg.stat;
    while (1) {
        if (sys->unk234 == 0) {
            MdMessMain(mess);
        } else if (sys->unk234 == 1 && *ctrl == 2) {
            MdMessDebug(mess);
        }
        HuPrcVSleep();
    }
}

static void MdMessCreate(SYSMESS *mess)
{
    SYSMESSWIN *win;
    SYSMESSSPR *spr;
    s32 i;

    mess->mode = 0;
    mess->dbgSel = 0;
    mess->dbgColor = 7;
    mess->dlgWinId = -1;
    mess->createF = 1;
    HuWinInit(0);
    for (win = &mess->win[0], i = 0; i < 3; i++, win++) {
        win->winId = -1;
        win->unk4 = 0;
        win->unk8 = 0;
        win->unkC = 0;
    }
    spr = &mess->spr;
    spr->mode = 0;
    spr->time = 0;
    spr->ofsTime = 0;
    spr->t = 0;
    spr->unk14 = 0;
}

static void MdMessMain(SYSMESS *mess)
{
    s16 stat;

    switch (mess->mode) {
        case 1:
            MdMessDlgWinCreateInsert(mess->dlgMesNo);
            mess->mode = 3;
            break;
        case 2:
            MdMessDlgWinCreateInsert(mess->dlgMesNo);
            mess->mode = 4;
            break;
        case 3:
            stat = HuWinStatGet(mess->dlgWinId);
            if (stat == 0) {
                mess->mode = 5;
            }
            break;
        case 4:
            stat = HuWinStatGet(mess->dlgWinId);
            if (stat == 0) {
                mess->mode = 6;
            }
            break;
        case 6:
            mess->choice = HuWinChoiceGet(mess->dlgWinId, -1);
            mess->mode = 5;
            break;
        case 7:
            MdMessDlgWinKill();
            mess->mode = 0;
            break;
    }
}

void MdMessDlgMesSet(int type, s32 mesNo)
{
    NEL_SYS *sys = &sysWork;
    SYSMESS *mess = (SYSMESS *)sys->mess;
    SYSMESSSPR *spr = &mess->spr;
    s16 stat;

    switch (type) {
        case 0:
            MdMessDlgWinKill();
            mess->mode = 0;
            break;
        case 1:
        case 3:
            if (mess->dlgWinId == -1) {
                if (type == 1) {
                    mess->dlgType = 0;
                } else {
                    mess->dlgType = 1;
                }
                mess->dlgMesNo = mesNo;
                mess->mode = 1;
            }
            break;
        case 2:
        case 4:
            if (mess->dlgWinId == -1) {
                if (type == 2) {
                    mess->dlgType = 0;
                } else {
                    mess->dlgType = 1;
                }
                mess->dlgMesNo = mesNo;
                mess->mode = 2;
            }
            break;
        case 5:
            switch (mess->mode) {
                case 1:
                    MdMessDlgWinCreateInsert(mess->dlgMesNo);
                    mess->mode = 3;
                    break;
                case 2:
                    MdMessDlgWinCreateInsert(mess->dlgMesNo);
                    mess->mode = 4;
                    break;
                case 3:
                    stat = HuWinStatGet(mess->dlgWinId);
                    if (stat == 0) {
                        mess->mode = 5;
                    }
                    break;
                case 4:
                    stat = HuWinStatGet(mess->dlgWinId);
                    if (stat == 0) {
                        mess->mode = 6;
                    }
                    break;
                case 6:
                    mess->choice = HuWinChoiceGet(mess->dlgWinId, -1);
                    mess->mode = 5;
                    break;
                case 7:
                    MdMessDlgWinKill();
                    mess->mode = 0;
                    break;
            }
            break;
        case 6:
        case 7:
        case 8:
        case 9:
            if (type == 6) {
                spr->mode = 1;
            } else if (type == 7) {
                spr->mode = 2;
            } else if (type == 8) {
                spr->mode = 3;
            } else if (type == 9) {
                spr->mode = 4;
            }
            break;
    }
}

void MdMessDlgInsertMesReset(void)
{
    NEL_SYS *sys = &sysWork;
    SYSMESS *mess = (SYSMESS *)sys->mess;
    SYSMESSINSERT *insert = &mess->insert;

    insert->num = 0;
}

void MdMessDlgInsertMesAdd(s32 mesNo, s16 insertNo)
{
    NEL_SYS *sys = &sysWork;
    SYSMESS *mess = (SYSMESS *)sys->mess;
    SYSMESSINSERT *insert = &mess->insert;

    insert->mes[insert->num] = mesNo;
    insert->no[insert->num] = insertNo;
    if (insert->num < 16) {
        insert->num++;
    }
}

static void MdMessDlgWinCreateInsert(s32 mesNo)
{
    NEL_SYS *sys = &sysWork;
    SYSMESS *mess = (SYSMESS *)sys->mess;
    SYSMESSINSERT *insert = &mess->insert;
    HuVec2f size;
    s32 i;

    if (mess->dlgWinId == -1) {
        if (mess->dlgType == 1) {
            for (i = 0; i < insert->num; i++) {
                HuWinInsertMesSizeGet(insert->mes[i], insert->no[i]);
            }
        }
        HuWinMesMaxSizeGet(1, &size, mesNo);
        if (size.x >= 472.0f) {
            size.x = 472.0f;
        }
        mess->dlgWinId = HuWinCreate(-10000, 374, size.x, 64, 0);
        if (mess->dlgType == 1) {
            for (i = 0; i < insert->num; i++) {
                HuWinInsertMesSet(mess->dlgWinId, insert->mes[i], insert->no[i]);
            }
        }
        HuWinBGTPLvlSet(mess->dlgWinId, 0);
        HuWinDrawNoSet(mess->dlgWinId, 0);
        HuWinPriSet(mess->dlgWinId, 0);
        HuWinPriSet(mess->dlgWinId, 0);
        HuWinMesSet(mess->dlgWinId, mesNo);
        HuWinDispOn(mess->dlgWinId);
    }
}

static void MdMessDlgWinKill(void)
{
    NEL_SYS *sys = &sysWork;
    SYSMESS *mess = (SYSMESS *)sys->mess;

    if (mess->dlgWinId != -1) {
        HuWinHomeClear(mess->dlgWinId);
        HuWinDispOff(mess->dlgWinId);
        HuWinKill(mess->dlgWinId);
        mess->dlgWinId = -1;
    }
}

void MdMessWinMesSet(volatile s32 winNo, s32 mesNo)
{
    NEL_SYS *sys = &sysWork;
    SYSMESS *mess = (SYSMESS *)sys->mess;
    SYSMESSWIN *win = &mess->win[winNo];
    HuVec2f size;

    if (win->winId == -1) {
        HuWinMesMaxSizeGet(1, &size, mesNo);
        if (size.x >= 472.0f) {
            size.x = 472.0f;
        }
        win->winId = HuWinCreate(0, 0, size.x, 32, 0);
        HuWinBGTPLvlSet(win->winId, 0);
        HuWinDrawNoSet(win->winId, 0);
        HuWinPriSet(win->winId, 0);
        HuWinPriSet(win->winId, 0);
        HuWinMesSet(win->winId, mesNo);
        HuWinDispOn(win->winId);
    }
}

void MdMessWinKill(volatile s32 winNo)
{
    NEL_SYS *sys = &sysWork;
    SYSMESS *mess = (SYSMESS *)sys->mess;
    SYSMESSWIN *win = &mess->win[winNo];

    if (win->winId != -1) {
        HuWinHomeClear(win->winId);
        HuWinDispOff(win->winId);
        HuWinKill(win->winId);
        win->winId = -1;
    }
}

void MdMessWinPosSet(volatile s32 winNo, volatile float x, volatile float y)
{
    NEL_SYS *sys = &sysWork;
    SYSMESS *mess = (SYSMESS *)sys->mess;
    SYSMESSWIN *win = &mess->win[winNo];

    HuWinPosSet(win->winId, x, y);
}

void MdMessWinSpeedSet(volatile s32 winNo, volatile s16 speed)
{
    NEL_SYS *sys = &sysWork;
    SYSMESS *mess = (SYSMESS *)sys->mess;
    SYSMESSWIN *win = &mess->win[winNo];

    HuWinMesSpeedSet(win->winId, speed);
}

void fn_1_28F74(s32 winNo)
{
    NEL_SYS *sys = &sysWork;
    SYSMESS *mess = (SYSMESS *)sys->mess;
    SYSMESSWIN *win = &mess->win[winNo];
}

void fn_1_28FAC(s32 winNo)
{
    NEL_SYS *sys = &sysWork;
    SYSMESS *mess = (SYSMESS *)sys->mess;
    SYSMESSWIN *win = &mess->win[winNo];
}

void MdMessDlgSprUpdate(SYSMESS *volatile mess)
{
    NEL_SYS *sys = &sysWork;
    SYSMESSSPR *spr = &mess->spr;
    SYSSPRITE *sprite = (SYSSPRITE *)sys->sprite;
    s16 grpId = sprite->grpId[0];
    HuVec2f *pos1 = MdSpritePosGet(0, 1);
    HuVec2f *pos2 = MdSpritePosGet(0, 2);
    HuVec2f *pos0 = MdSpritePosGet(0, 0);
    HuVec2f *scale1 = MdSpriteScaleGet(0, 1);
    HuVec2f *scale2 = MdSpriteScaleGet(0, 2);
    HuVec2f *scale0 = MdSpriteScaleGet(0, 0);

    switch (spr->mode) {
        case 0:
        case 9:
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            MdMessDlgMesSet(0, -1);
            spr->step = 0;
            spr->time = 0;
            spr->ofsTime = 0;
            spr->t = 0;
            spr->unk14 = 0;
            spr->pos1 = pos1->y;
            spr->pos2 = pos2->y;
            spr->scale = scale0->y;
            if (spr->mode == 1) {
                spr->mode = 5;
            } else if (spr->mode == 2) {
                spr->mode = 6;
            } else if (spr->mode == 3) {
                spr->mode = 7;
            } else if (spr->mode == 4) {
                spr->mode = 8;
            }
            break;
        case 5:
            MdMathOscillate(3, &spr->time, &spr->t, 30);
            MdMathLerp(&pos1->y, -28, -2, spr->t);
            MdMathLerp(&pos2->y, 32, 2, spr->t);
            MdMathLerp(&scale0->y, 8, 0, spr->t);
            if (spr->time >= 1) {
                spr->mode = 0;
            }
            break;
        case 6:
            MdMathOscillate(1, &spr->time, &spr->t, 15);
            MdMathLerp(&pos1->y, spr->pos1, -2, spr->t);
            MdMathLerp(&pos2->y, spr->pos2, 2, spr->t);
            MdMathLerp(&scale0->y, spr->scale, 0, spr->t);
            if (spr->time >= 1) {
                spr->mode = 9;
            }
            break;
        case 7:
            MdMathOscillate(1, &spr->time, &spr->t, 15);
            MdMathLerp(&pos1->y, spr->pos1, -28, spr->t);
            MdMathLerp(&pos2->y, spr->pos2, 32, spr->t);
            MdMathLerp(&scale0->y, spr->scale, 8, spr->t);
            if (spr->time >= 1) {
                spr->mode = 0;
            }
            break;
        case 8:
            if (spr->step == 0) {
                MdMathOscillate(1, &spr->time, &spr->t, 15);
                MdMathLerp(&pos1->y, spr->pos1, -2, spr->t);
                MdMathLerp(&pos2->y, spr->pos2, 2, spr->t);
                MdMathLerp(&scale0->y, spr->scale, 0, spr->t);
                if (spr->time >= 1) {
                    spr->time = 0;
                    spr->step++;
                }
            } else if (spr->step == 1) {
                MdMathOscillate(3, &spr->time, &spr->t, 15);
                MdMathLerp(&pos1->y, -2, 0, spr->t);
                MdMathLerp(&pos2->y, 2, 0, spr->t);
                MdMathLerp(&scale1->x, 72, 0, spr->t);
                MdMathLerp(&scale2->x, 72, 0, spr->t);
                MdMathLerp(&scale0->x, 1, 0, spr->t);
                if (spr->time >= 1) {
                    spr->time = 0;
                    spr->step++;
                }
            } else if (spr->step == 2) {
                MdMathOscillate(1, &spr->time, &spr->t, 15);
                MdMathLerp(&pos1->y, -2, -28, spr->t);
                MdMathLerp(&pos2->y, 2, 32, spr->t);
                MdMathLerp(&scale0->y, 0, 8, spr->t);
                if (spr->time >= 1) {
                    spr->mode = 0;
                }
            }
            break;
    }
}

static void MdMessDebug(SYSMESS *mess)
{
    volatile s16 stat;
    HuVec2f size;
    s32 tbl[4] = { 0x440000, 0x440001, 0x440002, 0x440003 };

    switch (mess->mode) {
        case 0:
            if (HuPadBtnDown[0] & PAD_TRIGGER_L) {
                if (--mess->dbgSel == -1) {
                    mess->dbgSel = 3;
                }
            }
            if (HuPadBtnDown[0] & PAD_TRIGGER_R) {
                mess->dbgSel = ++mess->dbgSel % 4;
            }
            if (HuPadBtnDown[0] & PAD_BUTTON_A) {
                HuWinMesMaxSizeGet(1, &size, tbl[mess->dbgSel]);
                if (size.x >= 472.0f) {
                    size.x = 472.0f;
                }
                mess->dlgWinId = HuWinCreate(-10000, 374, size.x, 64, 0);
                HuWinBGTPLvlSet(mess->dlgWinId, 0);
                HuWinDrawNoSet(mess->dlgWinId, 0);
                HuWinPriSet(mess->dlgWinId, 0);
                HuWinMesColSet(mess->dlgWinId, mess->dbgColor);
                HuWinMesSet(mess->dlgWinId, tbl[mess->dbgSel]);
                HuWinDispOn(mess->dlgWinId);
                mess->exWinId = HuWinExCreateFrame(440, 304, 128, 64, -1, 1);
                HuWinPriSet(mess->exWinId, 0);
                HuWinExOpen(mess->exWinId);
                mess->mode++;
            }
            break;
        case 1:
            HuWinMesColSet(mess->dlgWinId, mess->dbgColor);
            stat = HuWinStatGet(mess->dlgWinId);
            if (stat == 0) {
                HuWinHomeClear(mess->dlgWinId);
                HuWinDispOff(mess->dlgWinId);
                HuWinKill(mess->dlgWinId);
                HuWinExClose(mess->exWinId);
                HuWinExKill(mess->exWinId);
                mess->mode = 0;
            }
            break;
    }
    if (HuPadBtnDown[0] & PAD_BUTTON_UP) {
        mess->dbgColor = ++mess->dbgColor % 10;
    }
    MdDebugFontPrint("mess(%d / %d)", mess->dbgSel, mess->dbgColor);
}

void MdMessKill(void)
{
    HuWinAllKill();
}
