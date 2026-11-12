#include "REL/m510dll.h"
#include "string.h"

// .data:0x0 (absolute 0x3C8) - frames the CPU waits per pitch type
s16 lbl_1_data_3C8[4] = { 15, 43, 18, 10 };

void fn_1_8184(M510_PAD *data, s16 playerNo, M510_PLAYER_MODEL_WORK *owner)
{
    memset(data, 0, sizeof(M510_PAD));
    data->owner = owner;
    data->padNo = GwPlayerConf[playerNo].padNo;
    fn_1_8420(&data->ai, data);
}

void fn_1_81F4(M510_PAD *data)
{
    fn_1_8778(&data->ai);
    memset(data, 0, sizeof(M510_PAD));
}

void fn_1_8234(M510_PAD *data)
{
    s8 absX;
    s8 x;
    s8 absY;
    s8 y;

    if (data->ai.type != 0) {
        fn_1_87A8(&data->ai);
        return;
    }
    data->btn = HuPadBtn[data->padNo];
    data->btnDown = HuPadBtnDown[data->padNo];
    data->btnRep = HuPadBtnRep[data->padNo];
    data->stkX = HuPadStkX[data->padNo];
    if (data->stkX >= 0) {
        absX = data->stkX;
    } else {
        absX = -data->stkX;
    }
    if (absX < 10) {
        x = 0;
    } else {
        x = data->stkX;
    }
    data->stkX = x;
    data->stkY = HuPadStkY[data->padNo];
    if (data->stkY >= 0) {
        absY = data->stkY;
    } else {
        absY = -data->stkY;
    }
    if (absY < 10) {
        y = 0;
    } else {
        y = data->stkY;
    }
    data->stkY = y;
    data->subStkX = HuPadSubStkX[data->padNo];
    data->subStkY = HuPadSubStkY[data->padNo];
    data->trigL = HuPadTrigL[data->padNo];
    data->trigR = HuPadTrigR[data->padNo];
    data->dStk = HuPadDStk[data->padNo];
    data->dStkRep = HuPadDStkRep[data->padNo];
}

void fn_1_8420(M510_AI *data, M510_PAD *owner)
{
    M510_PLAYER_MODEL_WORK *model = owner->owner;
    s16 count;
    s16 r;
    s16 a;
    s16 b;
    s16 tmp;
    s32 i;
    u16 level;

    memset(data, 0, sizeof(M510_AI));
    data->owner = owner;
    data->type = GwPlayerConf[model->playerNo].type;
    if (GwPlayerConf[model->playerNo].dif == 3) {
        level = 0;
    } else if (GwPlayerConf[model->playerNo].dif == 2) {
        level = 1;
    } else if (GwPlayerConf[model->playerNo].dif == 1) {
        level = 2;
    } else {
        level = 3;
    }
    data->level = level;

    switch (data->level) {
    case 0:
        count = (frand() % 6) + 18;
        break;
    case 1:
        count = (frand() & 3) + 14;
        break;
    case 2:
        count = (frand() % 5) + 9;
        break;
    default:
        count = (frand() % 5) + 4;
        break;
    }

    for (i = 0; i < 30; i++) {
        if (i < count) {
            data->pattern[i] = 1;
        } else {
            r = frand() % 10000;
            data->pattern[i] = 0;
            switch (data->level) {
            case 0:
                if (r < 1500) {
                    data->pattern[i] = 2;
                }
                break;
            case 1:
                if (r < 3000) {
                    data->pattern[i] = 2;
                }
                break;
            case 2:
                if (r < 4000) {
                    data->pattern[i] = 2;
                }
                break;
            default:
                if (r < 5000) {
                    data->pattern[i] = 2;
                }
                break;
            }
        }
    }

    for (i = 0; i < 120; i++) {
        a = i % 30;
        b = frand() % 30;
        tmp = data->pattern[a];
        data->pattern[a] = data->pattern[b];
        data->pattern[b] = tmp;
    }
    data->done = 0;
}

void fn_1_8778(M510_AI *data)
{
    memset(data, 0, sizeof(M510_AI));
}

void fn_1_87A8(M510_AI *data)
{
    M510_PAD *pad = data->owner;
    M510_PLAYER_MODEL_WORK *model = pad->owner;
    M510_PLAYER_WORK *player = model->owner;
    M510_PITCH_WORK *pitch = &player->pitch;
    s16 idx;

    idx = 30 - player->obj->remain;
#ifdef TARGET_PC
    if (idx > 29) {
        idx = 29;
    } else if (idx < 0) {
        idx = 0;
    }
#endif
    pad->btnDown = 0;

    switch (data->state) {
    case 0:
        if (pitch->state == 3) {
            data->timer = lbl_1_data_3C8[pitch->index];
            data->timer = ((frand() % 3) + data->timer) - 1;
            if (data->pattern[idx] == 0) {
                data->timer = 0;
                data->state = 2;
                return;
            }
            if (data->pattern[idx] == 2) {
                if (pitch->index == 1) {
                    data->timer = data->timer - ((frand() % 7) + 7);
                } else {
                    data->timer = (frand() % 7) + data->timer + 7;
                }
                data->state = 1;
                return;
            }
            data->state = 1;
            return;
        }
        return;
    case 1:
        if (data->timer-- <= 0) {
            model->pad.btnDown |= 0x100;
            data->timer = 0;
            data->state = 2;
            return;
        }
        break;
    case 2:
        if (pitch->state == 5) {
            data->timer = 0;
            data->state = 0;
        }
        break;
    }
}
