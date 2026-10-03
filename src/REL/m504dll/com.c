#include "REL/m504dll.h"
#include "string.h"

extern u8 *lbl_1_data_4134[];

s32 fn_1_BEF0(INPUTWORK *work, u8 *entry);
s32 fn_1_C178(INPUTWORK *work);
s32 fn_1_C1E8(INPUTWORK *work);
void **fn_1_C2CC(INPUTWORK *work, void **table);
u16 fn_1_C3E0(INPUTWORK *work);

void fn_1_BC54(INPUTWORK *work)
{
    memset(work, 0, 0x24);
}

// NONMATCHING: register allocation of the switch cases differs
void fn_1_BC84(INPUTWORK *work)
{
    register INPUTMAIN *main;
    register PADCTRL *ctrl;
    register void **table;
    register u8 *entry;
    register void *entry2;

    main = work->main;
    ctrl = (PADCTRL *)main->playerConf->unk0;
    main->btnDown = 0;
    main->stkY = 0;
    main->stkX = 0;

    table = work->dataTable;
    if (table != NULL) {
        entry = table[0];
        entry2 = table[1];
        if (*(s16 *)entry == 0) {
            work->state = 6;
        } else if (*(s16 *)entry < 0) {
            work->state = 7;
        }
    }

    switch (work->state) {
        case 0:
            if (fn_1_BEF0(work, entry) != 0) {
                work->state = 1;
                work->unk6 = 0;
                work->timer = fn_1_C3E0(work);
            }
            break;
        case 1:
            if (work->timer) {
                work->timer--;
                break;
            }
            if (work->dif == 0 || work->dif == 1) {
                if (ctrl->unkE0 == 5 || ctrl->unkE0 == 6) {
                    work->state = 2;
                    work->timer = 0;
                }
            } else if (ctrl->unkE0 == 6) {
                work->state = 2;
                work->timer = 0;
            }
            break;
        case 2:
            if (fn_1_C178(work) != 0) {
                work->state = 3;
            }
            break;
        case 3:
            if (fn_1_C1E8(work) != 0) {
                work->state = 4;
            }
            break;
        case 4:
            if (fn_1_BEF0(work, entry2) != 0) {
                work->state = 5;
                work->unk6 = 0;
                work->timer = fn_1_C3E0(work);
            }
            break;
        case 5:
            if (work->timer) {
                work->timer--;
                break;
            }
            if (work->dif == 0 || work->dif == 1) {
                if (ctrl->unkE0 == 5) {
                    work->state = 6;
                    work->timer = 0;
                }
            } else if (ctrl->unkE0 == 6) {
                work->state = 6;
                work->timer = 0;
            }
            break;
        case 6:
            work->dataTable = fn_1_C2CC(work, table);
            work->state = 0;
            break;
        case 7:
            break;
    }
}
