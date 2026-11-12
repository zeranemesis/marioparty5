#include "types.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/audio.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/flag.h"
#include "humath.h"
#include "REL/m503Dll.h"

typedef struct {
    HuVecF pos;
    HuVecF rot;
} M503OpCam;

extern s32 fn_1_D778(s32 idx);

OMOBJFUNC fn_1_2D4C(OMOBJMAN *objMan);
void fn_1_2DD8(OMOBJ *obj);
OMOBJFUNC fn_1_2E80(OMOBJMAN *objMan, OMOBJ *obj);
void fn_1_2FC0(OMOBJ *obj);
void fn_1_32E0(s32 from, s32 to, f32 t);

M503OpCam lbl_1_data_60[3] = {
    { { 0.0f, 0.0f, 0.0f }, { -50.0f, 0.0f, 1500.0f } },
    { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 100.0f } },
    { { 0.0f, 0.0f, 100.0f }, { -35.0f, 0.0f, 1200.0f } },
};

s32 lbl_1_bss_28;

OMOBJFUNC fn_1_2D4C(OMOBJMAN *objMan)
{
    Center = lbl_1_data_60[0].pos;
    CRot.x = lbl_1_data_60[0].rot.x;
    CRot.y = lbl_1_data_60[0].rot.y;
    CRot.z = 0.0f;
    CZoom = lbl_1_data_60[0].rot.z;
    return fn_1_2DD8;
}

void fn_1_2DD8(OMOBJ *obj)
{
    M503Data *data = obj->data;

    fn_1_430(obj);
    switch (data->unk10) {
        case 0:
            fn_1_2AD4(HuDataSelHeapReadNum(0x3D0004, HU_MEMNUM_OVL, HEAP_MODEL));
            data->unk10 = 1;
            break;
        case 1:
            if (fn_1_2B4C() != 0) {
                break;
            }
            data->unk10 = 2;
        case 2:
            fn_1_5B0(obj);
            break;
    }
}

OMOBJFUNC fn_1_2E80(OMOBJMAN *objMan, OMOBJ *obj)
{
    M503Data *data = obj->data;
    s32 scores[4];
    s32 best = 0;
    s32 i;

    for (i = 0; i < 4; i++) {
        s32 score = fn_1_D778(i);

        if (score > best) {
            best = score;
        }
        scores[i] = score;
    }
    data->unk58 = 0;
    data->unk5C = 0;
    for (i = 0; i < 4; i++) {
        data->unk48[i] = -1;
        if (best == scores[i]) {
            data->unk48[i] = data->unk38[i];
            data->unk60[i] = data->unk58++;
        } else {
            data->unk60[i] = data->unk5C++;
        }
    }
    if (best == 0) {
        data->unk70 = 0;
    } else {
        data->unk70 = 1;
    }
    data->unk20 = 1;
    lbl_1_bss_28 = -1;
    return fn_1_2FC0;
}

void fn_1_2FC0(OMOBJ *obj)
{
    M503Data *data = obj->data;
    f32 t;
    s16 coin;
    s32 i;

    fn_1_430(obj);
    switch (data->unk20) {
        case 1:
            data->time24 = 0;
            data->unk20 = 2;
            break;
        case 2:
            if (fn_1_1BFC(2) != 0) {
                lbl_1_data_60[1].pos.x = Center.x;
                lbl_1_data_60[1].pos.y = Center.y;
                lbl_1_data_60[1].pos.z = Center.z;
                lbl_1_data_60[1].rot.x = CRot.x;
                lbl_1_data_60[1].rot.y = CRot.y;
                lbl_1_data_60[1].rot.z = CZoom;
                data->unk20 = 3;
                data->time24 = 0;
            }
            break;
        case 3:
            t = 0.05555555555555555 * (f32)data->time24;
            if (t >= 1.0) {
                t = 1.0f;
                data->unk20 = 4;
                data->time24 = 0;
            }
            if (data->unk70 != 0) {
                fn_1_32E0(1, 2, t);
            }
            break;
        case 4:
            if (!((f64)data->time24 < 12.0)) {
                if (data->unk70 == 0) {
                    lbl_1_bss_28 = GameMesCreate(2, 2);
                    HuAudJinglePlay(0x7B);
                } else {
                    lbl_1_bss_28 = GameMesCreate(4, 3, data->unk48[0], data->unk48[1], data->unk48[2], data->unk48[3]);
                    HuAudJinglePlay(0x79);
                    for (i = 0; i < 4; i++) {
                        if (data->unk48[i] >= 0) {
                            coin = GwPlayer[i].mgCoinBonus + 10;
                            if (_CheckFlag(0x1000F) == 0) {
                                GwPlayer[i].mgCoinBonus = coin;
                            }
                        }
                    }
                }
                data->unk20 = 5;
                data->time24 = 0;
            }
            break;
        case 5:
            if (!((f64)data->time24 < 210.0)) {
                data->unk20 = 6;
                fn_1_5B0(obj);
            }
            break;
    }
}

void fn_1_32E0(s32 from, s32 to, f32 t)
{
    HuVecF vec;
    M503OpCam *src;
    M503OpCam *dst;

    if (from < 0) {
        if (to < 0) {
            return;
        }
        from = to;
    }
    if (to < 0) {
        to = from;
    }
    src = &lbl_1_data_60[from];
    dst = &lbl_1_data_60[to];
    PSVECSubtract(&dst->pos, &src->pos, &vec);
    PSVECScale(&vec, &vec, t);
    PSVECAdd(&src->pos, &vec, &vec);
    Center.x = vec.x;
    Center.y = vec.y;
    Center.z = vec.z;
    PSVECSubtract(&dst->rot, &src->rot, &vec);
    PSVECScale(&vec, &vec, t);
    PSVECAdd(&src->rot, &vec, &vec);
    CRot.x = vec.x;
    CRot.y = vec.y;
    CRot.z = 0.0f;
    CZoom = vec.z;
}
