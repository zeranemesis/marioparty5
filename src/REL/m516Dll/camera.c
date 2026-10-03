#include "game/audio.h"
#include "game/gamework.h"
#include "game/object.h"
#include "dolphin/mtx.h"

typedef struct {
    Vec center;
    Vec target;
} CAMPOS;

extern CAMPOS lbl_1_data_50[3];
static s32 lbl_1_bss_20;

void fn_1_410(OMOBJ *obj);
void fn_1_524(OMOBJ *obj);
s32 fn_1_8088(s32 index);
void fn_1_1F90(OMOBJ *obj);
void fn_1_21C4(OMOBJ *obj);
void fn_1_23EC(s32 idxA, s32 idxB, f32 t);

OMOBJFUNC fn_1_1F04(OMOBJMAN *objman)
{
    CAMPOS *pos = &lbl_1_data_50[0];

    Center = pos->center;
    CRot.x = pos->target.x;
    CRot.y = pos->target.y;
    CRot.z = 0.0f;
    CZoom = pos->target.z;
    return fn_1_1F90;
}

void fn_1_1F90(OMOBJ *obj)
{
    u32 *work = (u32 *)obj->data;
    f32 t;

    fn_1_410(obj);

    switch (work[0x10 / 4]) {
    case 0:
        work[0x10 / 4] = 1;
        work[0x14 / 4] = 0;
        break;
    case 1:
        if ((f64)work[0x14 / 4] < 102.0) {
            break;
        }
        work[0x10 / 4] = 2;
        work[0x14 / 4] = 0;
        break;
    case 2:
        t = (f32)(0.007246376811594203 * ((f32)work[0x14 / 4]));
        if (t >= 1.0) {
            t = 1.0f;
        }
        fn_1_23EC(0, 1, t);
        if (t < 1.0) {
            break;
        }
        work[0x10 / 4] = 3;
        work[0x14 / 4] = 0;
        break;
    case 3:
        fn_1_524(obj);
    default:
        break;
    }
}

OMOBJFUNC fn_1_2108(OMOBJMAN *objman, OMOBJ *obj)
{
    u32 *work = (u32 *)obj->data;
    s32 i;
    s32 v = 0;

    for (i = 0; i < 4; i++) {
        if (fn_1_8088(i) != 0) {
            *(u32 *)((u8 *)work + i * 4 + 0x48) =
                *(u32 *)((u8 *)work + i * 4 + 0x38);
        } else {
            *(u32 *)((u8 *)work + i * 4 + 0x48) = -1;
        }
    }

    work[0x58 / 4] = 0;
    work[0x20 / 4] = 1;
    lbl_1_bss_20 = -1;
    return fn_1_21C4;
}

void fn_1_21C4(OMOBJ *obj)
{
    u32 *work = (u32 *)obj->data;
    s32 total;
    s32 i;
    f32 t;

    fn_1_410(obj);

    switch (work[0x20 / 4]) {
    case 1:
        work[0x24 / 4] = 0;
        work[0x20 / 4] = 2;
        break;
    case 2:
        t = (f32)(0.05555555555555555 * ((f32)work[0x24 / 4]));
        if (t >= 1.0) {
            t = 1.0f;
            work[0x20 / 4] = 3;
            work[0x24 / 4] = 0;
        }
        fn_1_23EC(1, 2, t);
        break;
    case 3:
        if ((f64)work[0x24 / 4] < 12.0) {
            break;
        }
        work[0x20 / 4] = 4;
        work[0x24 / 4] = 0;
        total = 0;
        for (i = 0; i < 4; i++) {
            if ((s32) * (u32 *)((u8 *)work + i * 4 + 0x48) >= 0) {
                s32 v = fn_1_8088(i);
                total += v;
                *(s16 *)((u8 *)&GwPlayer[i] + 0x2A) =
                    (s16)(*(s16 *)((u8 *)&GwPlayer[i] + 0x2A) + v);
            }
        }
        if (total > 0) {
            HuAudJinglePlay(0x79);
        } else {
            HuAudJinglePlay(0x7B);
        }
        break;
    case 4:
        if ((f64)work[0x24 / 4] < 210.0) {
            break;
        }
        work[0x20 / 4] = 5;
        fn_1_524(obj);
        break;
    }
}

void fn_1_23EC(s32 idxA, s32 idxB, f32 t)
{
    CAMPOS *pa;
    CAMPOS *pb;
    Vec v;

    if (idxA < 0) {
        if (idxB < 0) {
            return;
        }
        idxA = idxB;
    }

    if (idxB < 0) {
        idxB = idxA;
    }

    pa = &lbl_1_data_50[idxA];
    pb = &lbl_1_data_50[idxB];

    PSVECSubtract(&pb->center, &pa->center, &v);
    PSVECScale(&v, &v, t);
    PSVECAdd(&pa->center, &v, &v);
    Center = v;

    PSVECSubtract(&pb->target, &pa->target, &v);
    PSVECScale(&v, &v, t);
    PSVECAdd(&pa->target, &v, &v);
    CRot.x = v.x;
    CRot.y = v.y;
    CRot.z = 0.0f;
    CZoom = v.z;
}