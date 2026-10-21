#include "REL/m504dll.h"
#include "string.h"

extern u8 *lbl_1_data_4134[];

void PSVECSubtract(const Vec *a, const Vec *b, Vec *dst);
f32 PSVECMag(const Vec *v);
s32 rand8(void);

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

/* The four helpers fn_1_BC84 calls. They live in com.o in the original too, so
 * the unit only linked at all once they were here: the REL link resolves calls
 * against the objects in the module, and com.o is linked from source, so an
 * undefined fn_1_BEF0 is "Failed to find symbol fn_1_BEF0 in any module".
 *
 * Constants come from com.o's .rodata (base label lbl_1_rodata_2E8):
 * 2F4 = 0.0f, 2F8 = 15.0f, 2FC = 1.0f, 300 = 0.2f, 304 = 0.8f,
 * 308 = 56.0f, 30C = 600.0f, 310 = 700.0f. */

/* Advances the cursor at work->unk6 by one 12-byte node, then measures the
 * stick. Returns 1 once the cursor has caught up with the count stored at the
 * head of `entry`. */
s32 fn_1_BEF0(INPUTWORK *work, u8 *entry)
{
    Vec sp24 = {0.0f, 0.0f, 0.0f};
    Vec sp36 = {0.0f, 0.0f, 0.0f};
    INPUTMAIN *main;
    PADCTRL *ctrl;
    Vec *node;
    f32 f30;
    f32 f31;
    s32 ret = 0;

    main = work->main;
    ctrl = (PADCTRL *)main->playerConf->unk0;
    sp36.y = 0.0f;

    node = (Vec *)((u8 *)entry + work->unk6 * 12 + 4);
    PSVECSubtract(node, &ctrl->unk100, &sp36);
    f30 = PSVECMag(&sp36);

    f31 = f30 / 15.0f;
    if (f31 > 1.0f) {
        f31 = 1.0f;
    } else if (f31 < 0.2f) {
        f31 = 0.2f;
    }

    if (f30 < 15.0f) {
        ctrl->unk100 = *node;
        work->unk6 = work->unk6 + 1;
        if (work->unk6 >= *(s16 *)entry) {
            ret = 1;
        }
    } else {
        PSVECNormalize((const Vec *) &sp36, &sp24);
        sp24.x = sp24.x * f31;
        sp24.z = sp24.z * f31;
        work->smoothX = 0.8f * work->smoothX + 0.2f * sp24.x;
        work->smoothY = 0.8f * work->smoothY + 0.2f * sp24.z;
        main->stkX = (s8) (56.0f * work->smoothX);
        main->stkY = (s8) (-56.0f * work->smoothY);
    }
    return ret;
}

/* Latches the "stick released" bit while the pad is in one of the two neutral
 * states. Returns 0 if it latched, 1 otherwise -- fn_1_BC84 advances only on 0. */
s32 fn_1_C178(INPUTWORK *work)
{
    INPUTMAIN *main = work->main;
    PADCTRL *ctrl = (PADCTRL *)main->playerConf->unk0;
    s32 ret = 0;

    if (ctrl->unkE0 == 5 || ctrl->unkE0 == 6) {
        main->btn |= 0x100;
    } else {
        ret = 1;
    }
    return ret;
}

/* Gate on the distance between the node and the pad, per difficulty. */
s32 fn_1_C1E8(INPUTWORK *work)
{
    INPUTMAIN *main = work->main;
    PADCTRL *ctrl = (PADCTRL *)main->playerConf->unk0;
    Vec sp8 = {0.0f, 0.0f, 0.0f};
    f32 f31;
    s32 ret = 0;

    PSVECSubtract((Vec *) (u8 *) main + 0x60, (Vec *) ((u8 *) ctrl + 0x38), &sp8);
    f31 = PSVECMag(&sp8);

    if (work->dif == 0 || work->dif == 1) {
        if (ctrl->unkE0 == 7) {
            ret = 1;
        }
    } else if (ctrl->unkE0 == 3) {
        if (f31 < 600.0f) {
            ret = 1;
        }
    } else if (ctrl->unkE0 == 4) {
        if (f31 < 700.0f) {
            ret = 1;
        }
    }
    return ret;
}

/* Picks the node with the fewest uses, ties broken by index, then bumps it. */
void **fn_1_C2CC(INPUTWORK *work, void **table)
{
    INPUTMAIN *main = work->main;
    s32 locals[4];
    s32 best;
    s32 count;
    s32 i;
    s32 n;
    void **node;
    u16 *p;

    best = 9999;
    n = *(u16 *) ((u8 *) table + 8);
    for (i = 0; i < n; i++) {
        node = table[3 + i];
        if (*(u16 *) ((u8 *) node + 0xA) < best) {
            best = *(u16 *) ((u8 *) node + 0xA);
        }
    }

    count = 0;
    for (i = 0; i < n; i++) {
        node = table[3 + i];
        if (*(u16 *) ((u8 *) node + 0xA) == best) {
            locals[count] = i;
            count++;
        }
    }

    node = table[3 + locals[rand8() % (count * 10) / 10]];
    p = (u16 *) ((u8 *) node + 0xA);
    *p = *p + 1;
    return node;
}
