#include "REL/m562dll.h"
#include "game/charman.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/pad.h"
#include "datadir_enum.h"
#include "humath.h"
#include <math.h>
#include <string.h>

/* Not declared in game/hu3d.h on purpose (see the note there): the float return is used here. */
extern float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

/* wrap a degree angle into (-180, 180], re-evaluating the expression in every branch like the original */
#define PL_WRAP_D(e) (((e) > 180.0) ? ((e) - 360.0) : (((e) <= -180.0) ? (360.0 + (e)) : (e)))
#define PL_WRAP_F(e) (((e) > 180.0f) ? ((e) - 360.0f) : (((e) <= -180.0f) ? (360.0f + (e)) : (e)))
#define PL_CLAMP(v, lo, hi) (((v) > (hi)) ? (hi) : (((v) < (lo)) ? (lo) : (v)))
#define PL_RAD(a) (3.141592653589793 * (a) / 180.0)

/* shift to motion `idx`, then overlay the requested one (looping motions also get the overlay attribute) */
#define PL_SHIFT_OVERLAY(idx)     do {         u32 attr;         if (p->loopMotion != 0) {             attr = 0x40000001;         } else {             attr = 0;         }         Hu3DMotionShiftSet(model, p->obj->mtnId[idx], 0.0f, 4.0f, attr);         Hu3DMotionOverlaySet(model, p->obj->mtnId[p->curMotion]);         if (p->loopMotion != 0) {             Hu3DModelAttrSet(model, 0x40000200);         }     } while (0)

/* launch fields of a bomb slot; the sunbomb view of the slot keeps them in its padding */
#define PL_BOMB_F32(slot, off) (*(f32 *)((slot)->pad68 + (off)))

/* motion table: the 30 motions of every character (DATANUM), then the character's own model entry */
u32 lbl_1_data_190[13][31] = {
    {
        DATANUM(DATA_mariomot, 0x00), DATANUM(DATA_mariomot, 0x01), DATANUM(DATA_mariomot, 0x02),
        DATANUM(DATA_mariomot, 0x03), DATANUM(DATA_mariomot, 0x04), DATANUM(DATA_mariomot, 0x28),
        DATANUM(DATA_mariomot, 0x26), DATANUM(DATA_mariomot, 0x26), DATANUM(DATA_mariomot, 0x27),
        DATANUM(DATA_mariomot, 0x26), DATANUM(DATA_mariomot, 0x26), DATANUM(DATA_mariomot, 0x59),
        DATANUM(DATA_mariomot, 0x2B), DATANUM(DATA_mariomot, 0x15), DATANUM(DATA_mariomot, 0x17),
        DATANUM(DATA_mariomot, 0x18), DATANUM(DATA_mariomot, 0x16), DATANUM(DATA_mariomot, 0x24),
        DATANUM(DATA_mariomot, 0x3F), DATANUM(DATA_mariomot, 0x10), DATANUM(DATA_mariomot, 0x11),
        DATANUM(DATA_mariomot, 0x13), DATANUM(DATA_mariomot, 0x14), DATANUM(DATA_mariomot, 0x1A),
        DATANUM(DATA_mariomot, 0x06), DATANUM(DATA_mariomot, 0x31), DATANUM(DATA_mariomot, 0x19),
        DATANUM(DATA_mariomot, 0x2C), DATANUM(DATA_mariomot, 0x83), DATANUM(DATA_mariomot, 0x84),
        DATANUM(DATA_mario, 0xBD)
    },
    {
        DATANUM(DATA_luigimot, 0x00), DATANUM(DATA_luigimot, 0x01), DATANUM(DATA_luigimot, 0x02),
        DATANUM(DATA_luigimot, 0x03), DATANUM(DATA_luigimot, 0x04), DATANUM(DATA_luigimot, 0x28),
        DATANUM(DATA_luigimot, 0x26), DATANUM(DATA_luigimot, 0x26), DATANUM(DATA_luigimot, 0x27),
        DATANUM(DATA_luigimot, 0x26), DATANUM(DATA_luigimot, 0x26), DATANUM(DATA_luigimot, 0x59),
        DATANUM(DATA_luigimot, 0x2B), DATANUM(DATA_luigimot, 0x15), DATANUM(DATA_luigimot, 0x17),
        DATANUM(DATA_luigimot, 0x18), DATANUM(DATA_luigimot, 0x16), DATANUM(DATA_luigimot, 0x24),
        DATANUM(DATA_luigimot, 0x3F), DATANUM(DATA_luigimot, 0x10), DATANUM(DATA_luigimot, 0x11),
        DATANUM(DATA_luigimot, 0x13), DATANUM(DATA_luigimot, 0x14), DATANUM(DATA_luigimot, 0x1A),
        DATANUM(DATA_luigimot, 0x06), DATANUM(DATA_luigimot, 0x31), DATANUM(DATA_luigimot, 0x19),
        DATANUM(DATA_luigimot, 0x2C), DATANUM(DATA_luigimot, 0x83), DATANUM(DATA_luigimot, 0x84),
        DATANUM(DATA_luigi, 0xBD)
    },
    {
        DATANUM(DATA_peachmot, 0x00), DATANUM(DATA_peachmot, 0x01), DATANUM(DATA_peachmot, 0x02),
        DATANUM(DATA_peachmot, 0x03), DATANUM(DATA_peachmot, 0x04), DATANUM(DATA_peachmot, 0x28),
        DATANUM(DATA_peachmot, 0x26), DATANUM(DATA_peachmot, 0x26), DATANUM(DATA_peachmot, 0x27),
        DATANUM(DATA_peachmot, 0x26), DATANUM(DATA_peachmot, 0x26), DATANUM(DATA_peachmot, 0x59),
        DATANUM(DATA_peachmot, 0x2B), DATANUM(DATA_peachmot, 0x15), DATANUM(DATA_peachmot, 0x17),
        DATANUM(DATA_peachmot, 0x18), DATANUM(DATA_peachmot, 0x16), DATANUM(DATA_peachmot, 0x24),
        DATANUM(DATA_peachmot, 0x3F), DATANUM(DATA_peachmot, 0x10), DATANUM(DATA_peachmot, 0x11),
        DATANUM(DATA_peachmot, 0x13), DATANUM(DATA_peachmot, 0x14), DATANUM(DATA_peachmot, 0x1A),
        DATANUM(DATA_peachmot, 0x06), DATANUM(DATA_peachmot, 0x31), DATANUM(DATA_peachmot, 0x19),
        DATANUM(DATA_peachmot, 0x2C), DATANUM(DATA_peachmot, 0x83), DATANUM(DATA_peachmot, 0x84),
        DATANUM(DATA_peach, 0xBD)
    },
    {
        DATANUM(DATA_yoshimot, 0x00), DATANUM(DATA_yoshimot, 0x01), DATANUM(DATA_yoshimot, 0x02),
        DATANUM(DATA_yoshimot, 0x03), DATANUM(DATA_yoshimot, 0x04), DATANUM(DATA_yoshimot, 0x28),
        DATANUM(DATA_yoshimot, 0x26), DATANUM(DATA_yoshimot, 0x26), DATANUM(DATA_yoshimot, 0x27),
        DATANUM(DATA_yoshimot, 0x26), DATANUM(DATA_yoshimot, 0x26), DATANUM(DATA_yoshimot, 0x59),
        DATANUM(DATA_yoshimot, 0x2B), DATANUM(DATA_yoshimot, 0x15), DATANUM(DATA_yoshimot, 0x17),
        DATANUM(DATA_yoshimot, 0x18), DATANUM(DATA_yoshimot, 0x16), DATANUM(DATA_yoshimot, 0x24),
        DATANUM(DATA_yoshimot, 0x3F), DATANUM(DATA_yoshimot, 0x10), DATANUM(DATA_yoshimot, 0x11),
        DATANUM(DATA_yoshimot, 0x13), DATANUM(DATA_yoshimot, 0x14), DATANUM(DATA_yoshimot, 0x1A),
        DATANUM(DATA_yoshimot, 0x06), DATANUM(DATA_yoshimot, 0x31), DATANUM(DATA_yoshimot, 0x19),
        DATANUM(DATA_yoshimot, 0x2C), DATANUM(DATA_yoshimot, 0x83), DATANUM(DATA_yoshimot, 0x84),
        DATANUM(DATA_yoshi, 0xBD)
    },
    {
        DATANUM(DATA_wariomot, 0x00), DATANUM(DATA_wariomot, 0x01), DATANUM(DATA_wariomot, 0x02),
        DATANUM(DATA_wariomot, 0x03), DATANUM(DATA_wariomot, 0x04), DATANUM(DATA_wariomot, 0x28),
        DATANUM(DATA_wariomot, 0x26), DATANUM(DATA_wariomot, 0x26), DATANUM(DATA_wariomot, 0x27),
        DATANUM(DATA_wariomot, 0x26), DATANUM(DATA_wariomot, 0x26), DATANUM(DATA_wariomot, 0x59),
        DATANUM(DATA_wariomot, 0x2B), DATANUM(DATA_wariomot, 0x15), DATANUM(DATA_wariomot, 0x17),
        DATANUM(DATA_wariomot, 0x18), DATANUM(DATA_wariomot, 0x16), DATANUM(DATA_wariomot, 0x24),
        DATANUM(DATA_wariomot, 0x3F), DATANUM(DATA_wariomot, 0x10), DATANUM(DATA_wariomot, 0x11),
        DATANUM(DATA_wariomot, 0x13), DATANUM(DATA_wariomot, 0x14), DATANUM(DATA_wariomot, 0x1A),
        DATANUM(DATA_wariomot, 0x06), DATANUM(DATA_wariomot, 0x31), DATANUM(DATA_wariomot, 0x19),
        DATANUM(DATA_wariomot, 0x2C), DATANUM(DATA_wariomot, 0x83), DATANUM(DATA_wariomot, 0x84),
        DATANUM(DATA_wario, 0xBD)
    },
    {
        DATANUM(DATA_daisymot, 0x00), DATANUM(DATA_daisymot, 0x01), DATANUM(DATA_daisymot, 0x02),
        DATANUM(DATA_daisymot, 0x03), DATANUM(DATA_daisymot, 0x04), DATANUM(DATA_daisymot, 0x28),
        DATANUM(DATA_daisymot, 0x26), DATANUM(DATA_daisymot, 0x26), DATANUM(DATA_daisymot, 0x27),
        DATANUM(DATA_daisymot, 0x26), DATANUM(DATA_daisymot, 0x26), DATANUM(DATA_daisymot, 0x59),
        DATANUM(DATA_daisymot, 0x2B), DATANUM(DATA_daisymot, 0x15), DATANUM(DATA_daisymot, 0x17),
        DATANUM(DATA_daisymot, 0x18), DATANUM(DATA_daisymot, 0x16), DATANUM(DATA_daisymot, 0x24),
        DATANUM(DATA_daisymot, 0x3F), DATANUM(DATA_daisymot, 0x10), DATANUM(DATA_daisymot, 0x11),
        DATANUM(DATA_daisymot, 0x13), DATANUM(DATA_daisymot, 0x14), DATANUM(DATA_daisymot, 0x1A),
        DATANUM(DATA_daisymot, 0x06), DATANUM(DATA_daisymot, 0x31), DATANUM(DATA_daisymot, 0x19),
        DATANUM(DATA_daisymot, 0x2C), DATANUM(DATA_daisymot, 0x83), DATANUM(DATA_daisymot, 0x84),
        DATANUM(DATA_daisy, 0xBD)
    },
    {
        DATANUM(DATA_waluigimot, 0x00), DATANUM(DATA_waluigimot, 0x01), DATANUM(DATA_waluigimot, 0x02),
        DATANUM(DATA_waluigimot, 0x03), DATANUM(DATA_waluigimot, 0x04), DATANUM(DATA_waluigimot, 0x28),
        DATANUM(DATA_waluigimot, 0x26), DATANUM(DATA_waluigimot, 0x26), DATANUM(DATA_waluigimot, 0x27),
        DATANUM(DATA_waluigimot, 0x26), DATANUM(DATA_waluigimot, 0x26), DATANUM(DATA_waluigimot, 0x59),
        DATANUM(DATA_waluigimot, 0x2B), DATANUM(DATA_waluigimot, 0x15), DATANUM(DATA_waluigimot, 0x17),
        DATANUM(DATA_waluigimot, 0x18), DATANUM(DATA_waluigimot, 0x16), DATANUM(DATA_waluigimot, 0x24),
        DATANUM(DATA_waluigimot, 0x3F), DATANUM(DATA_waluigimot, 0x10), DATANUM(DATA_waluigimot, 0x11),
        DATANUM(DATA_waluigimot, 0x13), DATANUM(DATA_waluigimot, 0x14), DATANUM(DATA_waluigimot, 0x1A),
        DATANUM(DATA_waluigimot, 0x06), DATANUM(DATA_waluigimot, 0x31), DATANUM(DATA_waluigimot, 0x19),
        DATANUM(DATA_waluigimot, 0x2C), DATANUM(DATA_waluigimot, 0x83), DATANUM(DATA_waluigimot, 0x84),
        DATANUM(DATA_waluigi, 0xBD)
    },
    {
        DATANUM(DATA_kinopiomot, 0x00), DATANUM(DATA_kinopiomot, 0x01), DATANUM(DATA_kinopiomot, 0x02),
        DATANUM(DATA_kinopiomot, 0x03), DATANUM(DATA_kinopiomot, 0x04), DATANUM(DATA_kinopiomot, 0x28),
        DATANUM(DATA_kinopiomot, 0x26), DATANUM(DATA_kinopiomot, 0x26), DATANUM(DATA_kinopiomot, 0x27),
        DATANUM(DATA_kinopiomot, 0x26), DATANUM(DATA_kinopiomot, 0x26), DATANUM(DATA_kinopiomot, 0x59),
        DATANUM(DATA_kinopiomot, 0x2B), DATANUM(DATA_kinopiomot, 0x15), DATANUM(DATA_kinopiomot, 0x17),
        DATANUM(DATA_kinopiomot, 0x18), DATANUM(DATA_kinopiomot, 0x16), DATANUM(DATA_kinopiomot, 0x24),
        DATANUM(DATA_kinopiomot, 0x3F), DATANUM(DATA_kinopiomot, 0x10), DATANUM(DATA_kinopiomot, 0x11),
        DATANUM(DATA_kinopiomot, 0x13), DATANUM(DATA_kinopiomot, 0x14), DATANUM(DATA_kinopiomot, 0x1A),
        DATANUM(DATA_kinopiomot, 0x06), DATANUM(DATA_kinopiomot, 0x31), DATANUM(DATA_kinopiomot, 0x19),
        DATANUM(DATA_kinopiomot, 0x2C), DATANUM(DATA_kinopiomot, 0x83), DATANUM(DATA_kinopiomot, 0x84),
        DATANUM(DATA_kinopio, 0xBD)
    },
    {
        DATANUM(DATA_teresamot, 0x00), DATANUM(DATA_teresamot, 0x01), DATANUM(DATA_teresamot, 0x02),
        DATANUM(DATA_teresamot, 0x03), DATANUM(DATA_teresamot, 0x04), DATANUM(DATA_teresamot, 0x28),
        DATANUM(DATA_teresamot, 0x26), DATANUM(DATA_teresamot, 0x26), DATANUM(DATA_teresamot, 0x27),
        DATANUM(DATA_teresamot, 0x26), DATANUM(DATA_teresamot, 0x26), DATANUM(DATA_teresamot, 0x59),
        DATANUM(DATA_teresamot, 0x2B), DATANUM(DATA_teresamot, 0x15), DATANUM(DATA_teresamot, 0x17),
        DATANUM(DATA_teresamot, 0x18), DATANUM(DATA_teresamot, 0x16), DATANUM(DATA_teresamot, 0x24),
        DATANUM(DATA_teresamot, 0x3F), DATANUM(DATA_teresamot, 0x10), DATANUM(DATA_teresamot, 0x11),
        DATANUM(DATA_teresamot, 0x13), DATANUM(DATA_teresamot, 0x14), DATANUM(DATA_teresamot, 0x1A),
        DATANUM(DATA_teresamot, 0x06), DATANUM(DATA_teresamot, 0x31), DATANUM(DATA_teresamot, 0x19),
        DATANUM(DATA_teresamot, 0x2C), DATANUM(DATA_teresamot, 0x83), DATANUM(DATA_teresamot, 0x84),
        DATANUM(DATA_teresa, 0xBD)
    },
    {
        DATANUM(DATA_minikoopamot, 0x00), DATANUM(DATA_minikoopamot, 0x01), DATANUM(DATA_minikoopamot, 0x02),
        DATANUM(DATA_minikoopamot, 0x03), DATANUM(DATA_minikoopamot, 0x04), DATANUM(DATA_minikoopamot, 0x28),
        DATANUM(DATA_minikoopamot, 0x26), DATANUM(DATA_minikoopamot, 0x26), DATANUM(DATA_minikoopamot, 0x27),
        DATANUM(DATA_minikoopamot, 0x26), DATANUM(DATA_minikoopamot, 0x26), DATANUM(DATA_minikoopamot, 0x59),
        DATANUM(DATA_minikoopamot, 0x2B), DATANUM(DATA_minikoopamot, 0x15), DATANUM(DATA_minikoopamot, 0x17),
        DATANUM(DATA_minikoopamot, 0x18), DATANUM(DATA_minikoopamot, 0x16), DATANUM(DATA_minikoopamot, 0x24),
        DATANUM(DATA_minikoopamot, 0x3F), DATANUM(DATA_minikoopamot, 0x10), DATANUM(DATA_minikoopamot, 0x11),
        DATANUM(DATA_minikoopamot, 0x13), DATANUM(DATA_minikoopamot, 0x14), DATANUM(DATA_minikoopamot, 0x1A),
        DATANUM(DATA_minikoopamot, 0x06), DATANUM(DATA_minikoopamot, 0x31), DATANUM(DATA_minikoopamot, 0x19),
        DATANUM(DATA_minikoopamot, 0x2C), DATANUM(DATA_minikoopamot, 0x83), DATANUM(DATA_minikoopamot, 0x84),
        DATANUM(DATA_minikoopa, 0xBD)
    },
    {
        DATANUM(DATA_mariomot, 0x00), DATANUM(DATA_mariomot, 0x01), DATANUM(DATA_mariomot, 0x02),
        DATANUM(DATA_mariomot, 0x03), DATANUM(DATA_mariomot, 0x04), DATANUM(DATA_mariomot, 0x28),
        DATANUM(DATA_mariomot, 0x26), DATANUM(DATA_mariomot, 0x26), DATANUM(DATA_mariomot, 0x27),
        DATANUM(DATA_mariomot, 0x26), DATANUM(DATA_mariomot, 0x26), DATANUM(DATA_mariomot, 0x59),
        DATANUM(DATA_mariomot, 0x2B), DATANUM(DATA_mariomot, 0x15), DATANUM(DATA_mariomot, 0x17),
        DATANUM(DATA_mariomot, 0x18), DATANUM(DATA_mariomot, 0x16), DATANUM(DATA_mariomot, 0x24),
        DATANUM(DATA_mariomot, 0x3F), DATANUM(DATA_mariomot, 0x10), DATANUM(DATA_mariomot, 0x11),
        DATANUM(DATA_mariomot, 0x13), DATANUM(DATA_mariomot, 0x14), DATANUM(DATA_mariomot, 0x1A),
        DATANUM(DATA_mariomot, 0x06), DATANUM(DATA_mariomot, 0x31), DATANUM(DATA_mariomot, 0x19),
        DATANUM(DATA_mariomot, 0x2C), DATANUM(DATA_mariomot, 0x83), DATANUM(DATA_mariomot, 0x84),
        DATANUM(DATA_mario, 0xBD)
    },
    {
        DATANUM(DATA_mariomot, 0x00), DATANUM(DATA_mariomot, 0x01), DATANUM(DATA_mariomot, 0x02),
        DATANUM(DATA_mariomot, 0x03), DATANUM(DATA_mariomot, 0x04), DATANUM(DATA_mariomot, 0x28),
        DATANUM(DATA_mariomot, 0x26), DATANUM(DATA_mariomot, 0x26), DATANUM(DATA_mariomot, 0x27),
        DATANUM(DATA_mariomot, 0x26), DATANUM(DATA_mariomot, 0x26), DATANUM(DATA_mariomot, 0x59),
        DATANUM(DATA_mariomot, 0x2B), DATANUM(DATA_mariomot, 0x15), DATANUM(DATA_mariomot, 0x17),
        DATANUM(DATA_mariomot, 0x18), DATANUM(DATA_mariomot, 0x16), DATANUM(DATA_mariomot, 0x24),
        DATANUM(DATA_mariomot, 0x3F), DATANUM(DATA_mariomot, 0x10), DATANUM(DATA_mariomot, 0x11),
        DATANUM(DATA_mariomot, 0x13), DATANUM(DATA_mariomot, 0x14), DATANUM(DATA_mariomot, 0x1A),
        DATANUM(DATA_mariomot, 0x06), DATANUM(DATA_mariomot, 0x31), DATANUM(DATA_mariomot, 0x19),
        DATANUM(DATA_mariomot, 0x2C), DATANUM(DATA_mariomot, 0x83), DATANUM(DATA_mariomot, 0x84),
        DATANUM(DATA_mario, 0xBD)
    },
    {
        DATANUM(DATA_mariomot, 0x00), DATANUM(DATA_mariomot, 0x01), DATANUM(DATA_mariomot, 0x02),
        DATANUM(DATA_mariomot, 0x03), DATANUM(DATA_mariomot, 0x04), DATANUM(DATA_mariomot, 0x28),
        DATANUM(DATA_mariomot, 0x26), DATANUM(DATA_mariomot, 0x26), DATANUM(DATA_mariomot, 0x27),
        DATANUM(DATA_mariomot, 0x26), DATANUM(DATA_mariomot, 0x26), DATANUM(DATA_mariomot, 0x59),
        DATANUM(DATA_mariomot, 0x2B), DATANUM(DATA_mariomot, 0x15), DATANUM(DATA_mariomot, 0x17),
        DATANUM(DATA_mariomot, 0x18), DATANUM(DATA_mariomot, 0x16), DATANUM(DATA_mariomot, 0x24),
        DATANUM(DATA_mariomot, 0x3F), DATANUM(DATA_mariomot, 0x10), DATANUM(DATA_mariomot, 0x11),
        DATANUM(DATA_mariomot, 0x13), DATANUM(DATA_mariomot, 0x14), DATANUM(DATA_mariomot, 0x1A),
        DATANUM(DATA_mariomot, 0x06), DATANUM(DATA_mariomot, 0x31), DATANUM(DATA_mariomot, 0x19),
        DATANUM(DATA_mariomot, 0x2C), DATANUM(DATA_mariomot, 0x83), DATANUM(DATA_mariomot, 0x84),
        DATANUM(DATA_mario, 0xBD)
    }
};

void fn_1_6B28(M562Player *p, s16 playerNo, M562Work *owner)
{
    CAMERA *cam = &owner->camera[0];
    OMOBJ *obj = NULL;
    u16 i;
    HU3DMODELID model;
    HU3DMODELID ring;

    memset(p, 0, sizeof(M562Player));
    p->owner = owner;
    p->playerNo = playerNo;
    p->grp = GwPlayerConf[p->playerNo].grp;
    p->charNo = GwPlayerConf[p->playerNo].charNo;
    obj = omAddObjEx(owner->manager, 0x64, 1, 0x1F, -1, NULL);
    p->obj = obj;
    p->inputOn = 0;
    p->height = CharModelHeightGet((s16) p->charNo);
    p->active = 1;
    model = CharModelCreate((s16) p->charNo, 2);
    p->modelId = model;
    obj->mdlId[0] = model;
    Hu3DModelShadowSet(model);
    CharModelStepFxSet((s16) p->charNo, 0);
    CharEffectLayerSet(4);
    for (i = 0; i < 31; i++) {
        obj->mtnId[i] = CharMotionCreate((s16) p->charNo, lbl_1_data_190[p->charNo][i]);
        CharMotionSet((s16) p->charNo, obj->mtnId[i]);
    }
    Hu3DModelAttrSet(model, 1);
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 3);
    Hu3DMotionSet(model, obj->mtnId[0]);
    Hu3DModelCameraSet(model, (u16) cam->camMask);
    Hu3DMotionShiftSet(model, p->obj->mtnId[0], 0.0f, 4.0f, 0);
    p->curMotion = 0;
    p->reqMotion = 0;
    p->loopMotion = 1;
    p->modelPos = &Hu3DData[model].pos;
    p->modelRot = &Hu3DData[model].rot;
    if (owner->diff == 0) {
        p->hp = 6;
    } else if (owner->diff == 1) {
        p->hp = 8;
    } else if (owner->diff == 2) {
        p->hp = 10;
    } else {
        p->hp = 10;
    }
    {
        Vec posInit = { 0.0f, 0.0f, 0.0f };
        p->pos = posInit;
    }
    {
        Vec rotInit = { 0.0f, 0.0f, 0.0f };
        p->rot = rotInit;
    }
    omSetTra(obj, p->pos.x, p->pos.y, p->pos.z);
    omSetRot(obj, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(model, 1);
    ring = Hu3DModelCreateData(DATANUM(DATA_m562, 0x20));
    p->ringModel = ring;
    Hu3DModelScaleSet(ring, 0.5f, 0.5f, 0.5f);
    Hu3DModelPosSet(ring, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(ring, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(ring, (u16) cam->camMask);
    Hu3DModelLayerSet(ring, 4);
    Hu3DModelAttrSet(ring, 1);
    p->ringMotion = Hu3DJointMotionData(ring, DATANUM(DATA_m562, 0x55));
    Hu3DMotionSet(ring, p->ringMotion);
    Hu3DMotionSpeedSet(ring, 9.08f);
    Hu3DModelAttrSet(ring, 0x40000001);
    fn_1_B07C(&p->input, playerNo, p);
}

void fn_1_6FBC(M562Player *p)
{
    M562Work *owner = p->owner;

    fn_1_B0E0(&p->input);
    Hu3DMotionKill(p->ringMotion);
    Hu3DModelKill(p->ringModel);
    if (p->obj != 0) {
        CharModelKill((s16) p->charNo);
        omDelObjEx(owner->manager, p->obj);
        p->obj = NULL;
    }
    memset(p, 0, sizeof(M562Player));
}

/* matrix of the right hand hook, read from the model on first use each frame */
Mtx *fn_1_7044(M562Player *p)
{
    if (p->handRValid == 0) {
        Hu3DModelObjMtxGet(p->modelId, CharModelItemHookGet((s16) p->charNo, 2, 0), p->handR);
        p->handRValid = 1;
    }
    return &p->handR;
}

/* same for the left hand hook */
Mtx *fn_1_70A8(M562Player *p)
{
    if (p->handLValid == 0) {
        Hu3DModelObjMtxGet(p->modelId, CharModelItemHookGet((s16) p->charNo, 2, 1), p->handL);
        p->handLValid = 1;
    }
    return &p->handL;
}

/* gravity while airborne, snap to the floor otherwise */
void fn_1_710C(M562Player *p)
{
    if (p->pos.y > 0.0f) {
        p->vy -= 1.5f;
        p->pos.y += p->vy;
    } else {
        p->vy = 0.0f;
        p->pos.y = 0.0f;
    }
}

s32 fn_1_7174(M562Player *p)
{
    M562PlayerInput *inp = &p->input;
    M562Work *owner = p->owner;
    HU3DMODELID model;

    if (p->invuln != 0) {
        if (p->invuln & 1) {
            Hu3DModelAttrSet(p->modelId, 1);
        } else {
            Hu3DModelAttrReset(p->modelId, 1);
        }
        p->invuln--;
    } else {
        Hu3DModelAttrReset(p->modelId, 1);
    }
    if (p->active == 0) {
        Hu3DModelAttrSet(p->modelId, 1);
    }
    if (p->scaleTimer != 0) {
        if (p->scaleTimer > 20) {
            f32 t = (f32) (p->scaleTimer - 0x87) / 5.0f;
            f32 c;
            f32 k;
            f32 sc;
            f32 sy;

            if (t > 1.0f) {
                c = 1.0f;
            } else {
                if (t < 0.0f) {
                    k = 0.0f;
                } else {
                    k = t;
                }
                c = k;
            }
            t = 1.0f - c;
            sc = 1.0f + 0.5f * t;
            sy = 0.05f + 0.95f * (1.0f - t);
            Hu3DModelScaleSet(p->modelId, sc, sy, sc);
        } else {
            f32 t = (f32) p->scaleTimer / 20.0f;
            f32 c;
            f32 k;
            f32 sc;
            f32 sy;

            if (t > 1.0f) {
                c = 1.0f;
            } else {
                if (t < 0.0f) {
                    k = 0.0f;
                } else {
                    k = t;
                }
                c = k;
            }
            t = c;
            sc = 1.0f + 0.5f * t;
            sy = 0.05f + 0.95f * (1.0f - t);
            Hu3DModelScaleSet(p->modelId, sc, sy, sc);
        }
        p->scaleTimer--;
    } else {
        Hu3DModelScaleSet(p->modelId, 1.0f, 1.0f, 1.0f);
    }
    if (owner->step >= 7) {
        p->scaleTimer = 0;
        p->invuln = 0;
        p->state = 0;
    }
    inp->btn = inp->btnDown = inp->stkX = inp->stkY = 0;
    if (p->inputOn != 0) {
        fn_1_B110(inp);
    }
    if (owner->mode != 1 && owner->mode != 4 && p->state == 2) {
        p->state = 1;
        p->sub = 0;
    }
    switch (p->state) {
        case 1:
            fn_1_7D58(p);
            break;
        case 2:
            fn_1_8DF8(p);
            break;
        case 3:
            if (fn_1_9C3C(p) != 0) {
                p->subTimer = 0;
                p->state = 1;
            }
            break;
        case 4:
            if (fn_1_A02C(p) != 0) {
                p->subTimer = 0;
                p->state = 1;
            }
            break;
        case 5:
            if (fn_1_A48C(p) != 0) {
                p->subTimer = 0;
                p->state = 1;
            }
            break;
        case 6:
            if (fn_1_A87C(p) != 0) {
                p->subTimer = 0;
                p->state = 1;
            }
            break;
    }
    model = p->modelId;
    if (p->curMotion != p->reqMotion) {
        if (Hu3DMotionShiftIDGet(model) < 0) {
            if (p->curMotion == 6 || p->curMotion == 7 || p->curMotion == 8 || p->curMotion == 9
                || p->curMotion == 10) {
                Hu3DModelAttrReset(model, 0x40000200);
                Hu3DMotionOverlayReset(model);
            }
            fn_1_7938(p);
        }
    } else if (p->curMotion == 0x1C) {
        f32 maxTime = Hu3DMotionMaxTimeGet(model);
        if (Hu3DMotionTimeGet(model) >= maxTime) {
            Hu3DMotionTimeSet(model, 75.0f);
        }
    }
    if (owner->mode < 4) {
        p->pos.x = PL_CLAMP(p->pos.x, -575.0f, 575.0f);
        p->pos.z = PL_CLAMP(p->pos.z, -450.0f, 450.0f);
    } else {
        p->pos.x = PL_CLAMP(p->pos.x, -1075.0f, 1075.0f);
        p->pos.z = PL_CLAMP(p->pos.z, -350.0f, 550.0f);
    }
    omSetTra(p->obj, p->pos.x, p->pos.y, p->pos.z);
    omSetRot(p->obj, p->rot.x, p->rot.y, p->rot.z);
    p->handRValid = 0;
    p->handLValid = 0;
    return 0;
}

/* start the motion requested in reqMotion (6..10 are overlay motions played over the base pose) */
void fn_1_7938(M562Player *p)
{
    HU3DMODELID model = p->modelId;

    if (p->reqMotion == 6 || p->reqMotion == 7 || p->reqMotion == 8 || p->reqMotion == 9
        || p->reqMotion == 10) {
        p->curMotion = p->reqMotion;
        Hu3DModelAttrReset(model, 0x40000001);
        Hu3DModelAttrReset(model, 0x40000200);
        switch (p->reqMotion) {
            case 6:
                PL_SHIFT_OVERLAY(0);
                break;
            case 7:
                PL_SHIFT_OVERLAY(1);
                break;
            case 8:
                PL_SHIFT_OVERLAY(2);
                break;
            case 9:
                PL_SHIFT_OVERLAY(3);
                break;
            case 10:
                PL_SHIFT_OVERLAY(0);
                break;
        }
    } else {
        f32 shift;

        if (p->curMotion == 0x1C) {
            shift = 15.0f;
        } else {
            shift = 4.0f;
        }
        p->curMotion = p->reqMotion;
        Hu3DModelAttrReset(model, 0x40000001);
        Hu3DMotionShiftSet(model, p->obj->mtnId[p->curMotion], 0.0f, shift, (p->loopMotion != 0) ? 0x40000001 : 0);
    }
}

/* state 1: walking on the plain stage */
void fn_1_7D58(M562Player *p)
{
    M562Work *owner = p->owner;
    M562PlayerInput *inp = &p->input;
    f32 mag2;
    f32 ang;
    f32 factor;
    f32 dx;
    f32 dz;

    mag2 = (f32) inp->stkX * (f32) inp->stkX + (f32) inp->stkY * (f32) inp->stkY;
    p->speed = 10.0f * (sqrtf(mag2) / 56.0f);
    ang = 0.0f;
    if (p->speed >= 0.001f && p->sub != 4) {
        ang = (f32) PL_WRAP_D(HuAtan((double) inp->stkX, (double) -inp->stkY));
    }
    switch (p->sub) {
        case 0:
            p->reqMotion = 0;
            p->loopMotion = 1;
            if (p->speed >= 0.001f) {
                if (p->speed > 6.0f) {
                    p->reqMotion = 2;
                    p->loopMotion = 1;
                } else {
                    p->reqMotion = 1;
                    p->loopMotion = 1;
                }
                p->goalRotY = ang;
            }
            fn_1_710C(p);
            if (inp->btnDown & 0x100) {
                p->reqMotion = 3;
                p->loopMotion = 0;
                p->vy = sqrtf(600.0f);
                p->sub = 1;
            } else if ((inp->btnDown & 0x200) && (owner->mode == 1 || owner->mode == 4)) {
                p->reqMotion = 5;
                p->loopMotion = 0;
                fn_1_89B0(p);
                if (owner->mode == 1) {
                    Fire *fire = p->held;

                    if (fire != 0) {
                        if (fire->state < 5) {
                            p->timerA = 0;
                            p->sub = 4;
                            p->goalRotY = p->rotY;
                        } else {
                            p->held = NULL;
                        }
                    }
                } else if (owner->mode == 4 && p->target != 0) {
                    p->timerA = 0;
                    p->sub = 4;
                    p->goalRotY = p->rotY;
                }
                if (p->held == 0 && p->target == 0) {
                    p->sub = 5;
                }
            }
            break;
        case 1:
            if ((inp->btn & 0x100) == 0 && p->vy > 0.0f) {
                p->vy *= 0.5f;
            }
            p->vy -= 1.5f;
            p->pos.y += p->vy;
            if (p->vy < 0.0f) {
                if (p->pos.y <= 0.0f) {
                    p->pos.y = 0.0f;
                    p->vy = 0.0f;
                    p->reqMotion = 4;
                    p->loopMotion = 0;
                    p->sub = 2;
                }
            }
            if (p->speed >= 0.001f) {
                p->goalRotY = ang;
            }
            break;
        case 2:
            if (p->curMotion == 4) {
                p->reqMotion = 0;
                p->loopMotion = 1;
                p->sub = 0;
            }
            if (p->speed >= 0.001f) {
                p->goalRotY = ang;
            }
            break;
        case 3:
            p->vy -= 1.5f;
            p->pos.y += p->vy;
            if (p->vy < 0.0f) {
                if (p->pos.y <= 0.0f) {
                    p->pos.y = 0.0f;
                    p->vy = 0.0f;
                    p->reqMotion = 4;
                    p->loopMotion = 0;
                    p->sub = 2;
                }
            }
            if (p->speed >= 0.001f) {
                p->goalRotY = ang;
            }
            break;
        case 4:
            p->speed = 0.0f;
            fn_1_8C50(p, (f32) p->timerA / 30.0f);
            p->timerA++;
            if (p->timerA >= 30) {
                fn_1_8C50(p, 1.0f);
                p->reqMotion = 6;
                p->loopMotion = 1;
                p->timerA = 0;
                p->state = 2;
                p->sub = 0;
            }
            break;
        case 5:
            p->speed = 0.0f;
            if (p->curMotion == 5) {
                f32 maxTime = Hu3DMotionMaxTimeGet(p->modelId);

                if (Hu3DMotionTimeGet(p->modelId) >= maxTime) {
                    p->reqMotion = 0;
                    p->loopMotion = 1;
                    p->sub = 0;
                }
            }
            break;
    }
    p->rotY = fn_1_36EC(p->goalRotY, p->rotY, 0.4f);
    if (p->scaleTimer != 0) {
        factor = 0.5f;
    } else {
        factor = 1.0f;
    }
    dx = (f32) (factor * (p->speed * sin(PL_RAD(p->rotY))));
    dz = (f32) (factor * (p->speed * cos(PL_RAD(p->rotY))));
    p->pos.x += dx;
    p->pos.z += dz;
    p->rot.y = p->rotY;
}

/* look for something in front of the player to pick up: a fire (stage 1) or a bomb (stage 4) */
s32 fn_1_89B0(M562Player *p)
{
    M562Work *owner = p->owner;
    s32 i;

    p->held = NULL;
    p->target = NULL;
    if (owner->mode == 1) {
        for (i = 0; i < FIRE_MAX; i++) {
            Fire *fire = &owner->fire.fire[i];

            if (fire->state != 7 && fn_1_8ACC(&p->pos, &fire->pos, p->rotY) != 0) {
                p->held = fire;
                return 1;
            }
        }
    } else if (owner->mode == 4) {
        for (i = 0; i < 5; i++) {
            KbBombSlot *slot = &owner->boss.bombs.slot[i];

            if (fn_1_8ACC(&p->pos, &slot->pos, p->rotY) != 0 && slot->state >= 5 && slot->state < 9) {
                p->target = slot;
                return 1;
            }
        }
    }
    return 0;
}

/* is `target` within reach and in front of the player (a 60 degree cone, 150 units) */
s32 fn_1_8ACC(Vec *pos, Vec *target, f32 rotY)
{
    Vec diff;
    Vec dir;
    Vec fwd;
    f32 dot;
    f32 mag;

    PSVECSubtract(target, pos, &diff);
    diff.y = 0.0f;
    mag = PSVECMag(&diff);
    if (mag < 150.0f) {
        if (diff.x == 0.0f && diff.z == 0.0f) {
            diff.z = 1.0f;
        }
        PSVECNormalize(&diff, &dir);
        fwd.x = sin(PL_RAD(rotY));
        fwd.y = 0.0f;
        fwd.z = cos(PL_RAD(rotY));
        dot = PSVECDotProduct(&dir, &fwd);
        if (dot >= cos(1.0471975511965976)) {
            return 1;
        }
    }
    return 0;
}

/* pull the held fire (stage 1) or bomb (stage 4) towards the right hand by `t` */
void fn_1_8C50(M562Player *p, f32 t)
{
    M562Work *owner = p->owner;
    Fire *fire = p->held;
    KbBombSlot *bomb = p->target;
    Vec hand;
    Vec delta;

    Hu3DMtxTransGet(*fn_1_7044(p), &hand);
    if (owner->mode == 1) {
        {
            PSVECSubtract(&hand, &fire->pos, &delta);
            PSVECScale(&delta, &delta, t);
            PSVECAdd(&fire->pos, &delta, &fire->pos);
            fire->angle = fn_1_36EC(p->rotY, fire->angle, 1.0f - t);
            fire->rot.y = fire->angle;
            fire->state = 4;
            fire->phase = 0;
            fire->timer = 0;
            if (fire->flameStage >= 3 && (p->state == 1 || p->state == 2) && p->invuln == 0) {
                p->timerA = 0;
                p->sub = 0;
                p->subTimer = 0;
                p->state = 4;
            }
        }
    } else if (owner->mode == 4) {
        PSVECSubtract(&hand, &bomb->pos, &delta);
        PSVECScale(&delta, &delta, t);
        PSVECAdd(&bomb->pos, &delta, &bomb->pos);
    }
}

/* state 2: walking while carrying (stage 1 / 4) */
void fn_1_8DF8(M562Player *p)
{
    M562Work *owner = p->owner;
    M562PlayerInput *inp = &p->input;
    f32 mag2;
    f32 ang;
    f32 factor;
    f32 dx;
    f32 dz;

    fn_1_99F4(p);
    mag2 = (f32) inp->stkX * (f32) inp->stkX + (f32) inp->stkY * (f32) inp->stkY;
    p->speed = 10.0f * (sqrtf(mag2) / 56.0f);
    ang = 0.0f;
    if (p->speed >= 0.001f) {
        ang = (f32) PL_WRAP_D(HuAtan((double) inp->stkX, (double) -inp->stkY));
    }
    if (owner->mode == 4) {
        p->speed *= 0.6f;
    }
    switch (p->sub) {
        case 0:
            p->reqMotion = 6;
            p->loopMotion = 1;
            if (p->speed >= 0.001f) {
                if (owner->mode == 4) {
                    p->reqMotion = 7;
                    p->loopMotion = 1;
                } else if (p->speed > 6.0f) {
                    p->reqMotion = 8;
                    p->loopMotion = 1;
                } else {
                    p->reqMotion = 7;
                    p->loopMotion = 1;
                }
                p->goalRotY = ang;
            }
            fn_1_710C(p);
            if (inp->btnDown & 0x100) {
                p->reqMotion = 9;
                p->loopMotion = 0;
                p->vy = sqrtf(600.0f);
                p->sub = 1;
            } else if ((inp->btnDown & 0x200) || owner->step > 4) {
                p->reqMotion = 0xB;
                p->loopMotion = 0;
                p->sub = 4;
            }
            break;
        case 1:
            if ((inp->btn & 0x100) == 0 && p->vy > 0.0f) {
                p->vy *= 0.5f;
            }
            p->vy -= 1.5f;
            p->pos.y += p->vy;
            if ((inp->btnDown & 0x200) || owner->step > 4) {
                p->reqMotion = 0xB;
                p->loopMotion = 0;
                p->sub = 5;
            } else {
                if (p->vy < 0.0f) {
                    if (p->pos.y <= 0.0f) {
                        p->pos.y = 0.0f;
                        p->vy = 0.0f;
                        p->reqMotion = 10;
                        p->loopMotion = 0;
                        p->sub = 2;
                        CharModelLandDustCreate((s16) p->charNo, &p->pos);
                        CharFXPlay((s16) p->charNo, 0x96);
                    }
                }
                if (p->speed >= 0.001f) {
                    p->goalRotY = ang;
                }
            }
            break;
        case 2:
            if (p->curMotion == 10) {
                p->reqMotion = 6;
                p->loopMotion = 1;
                p->sub = 0;
            }
            if (p->speed >= 0.001f) {
                p->goalRotY = ang;
            }
            break;
        case 3:
            p->vy -= 1.5f;
            p->pos.y += p->vy;
            if ((inp->btnDown & 0x200) || owner->step > 4) {
                p->reqMotion = 0xB;
                p->loopMotion = 0;
                p->sub = 5;
            } else {
                if (p->vy < 0.0f) {
                    if (p->pos.y <= 0.0f) {
                        p->pos.y = 0.0f;
                        p->vy = 0.0f;
                        p->reqMotion = 10;
                        p->loopMotion = 0;
                        p->sub = 2;
                        CharModelLandDustCreate((s16) p->charNo, &p->pos);
                        CharFXPlay((s16) p->charNo, 0x96);
                    }
                }
                if (p->speed >= 0.001f) {
                    p->goalRotY = ang;
                }
            }
            break;
        case 4:
            if (p->curMotion == 0xB) {
                fn_1_9AEC(p);
                p->reqMotion = 0;
                p->loopMotion = 1;
                p->state = 1;
                p->sub = 0;
            }
            break;
        case 5:
            if (p->curMotion == 0xB) {
                fn_1_9AEC(p);
                p->reqMotion = 3;
                p->loopMotion = 0;
                p->state = 1;
                p->sub = 1;
            }
            break;
    }
    p->rotY = fn_1_36EC(p->goalRotY, p->rotY, 0.4f);
    if (p->scaleTimer != 0) {
        factor = 0.5f;
    } else {
        factor = 1.0f;
    }
    dx = (f32) (factor * (p->speed * sin(PL_RAD(p->rotY))));
    dz = (f32) (factor * (p->speed * cos(PL_RAD(p->rotY))));
    p->pos.x += dx;
    p->pos.z += dz;
    p->rot.y = p->rotY;
}

/* keep the carried fire / bomb glued to the right hand */
void fn_1_99F4(M562Player *p)
{
    M562Work *owner = p->owner;
    Fire *fire = p->held;
    KbBombSlot *bomb = p->target;
    Vec hand;

    Hu3DMtxTransGet(*fn_1_7044(p), &hand);
    if (owner->mode == 1) {
        if (fire != 0) {
            fire->pos = hand;
            fire->targetAngle = p->rotY;
            fire->angle = fire->targetAngle;
            fire->rot.y = fire->angle;
        }
    } else if (owner->mode == 4) {
        if (bomb != 0) {
            bomb->pos = hand;
        }
    }
}

/* let go of the carried fire / bomb: the fire drops, the bomb is thrown away from the player */
void fn_1_9AEC(M562Player *p)
{
    M562Work *owner = p->owner;
    Fire *fire = p->held;
    KbBombSlot *bomb = p->target;

    if (owner->mode == 1) {
        if (fire != 0) {
            fire->state = 5;
            fire->phase = 0;
            fire->timer = 0;
        }
    } else if (owner->mode == 4 && bomb != 0) {
        PL_BOMB_F32(bomb, 4) = (f32) (0.8f * (10.0 * sin(PL_RAD(p->rotY))));
        PL_BOMB_F32(bomb, 12) = (f32) (0.8f * (10.0 * cos(PL_RAD(p->rotY))));
        PL_BOMB_F32(bomb, 0) = 20.0f;
    }
    p->held = NULL;
    p->target = NULL;
}

/* state 3: knocked back */
s32 fn_1_9C3C(M562Player *p)
{
    s32 done = 0;

    switch (p->sub) {
        case 0:
            p->hp--;
            omVibrate(p->playerNo, 0x1E, 4, 2);
            if (p->held != 0 || p->target != 0) {
                fn_1_9AEC(p);
            }
            p->reqMotion = 0xC;
            p->loopMotion = 1;
            p->vy = sqrtf(1200.0f);
            p->sub = 1;
            /* fallthrough */
        case 1:
            p->vy -= 1.5f;
            p->pos.y += p->vy;
            if (p->vy < 0.0f) {
                if (p->pos.y <= 0.0f) {
                    p->pos.y = 0.0f;
                    p->vy = 0.0f;
                    p->reqMotion = 0xE;
                    p->loopMotion = 1;
                    p->timerA = 0x3C;
                    p->sub = 2;
                }
            }
            break;
        case 2:
            if (p->timerA-- <= 0) {
                p->reqMotion = 0xF;
                p->loopMotion = 0;
                p->timerA = 0x3C;
                p->sub = 3;
            }
            break;
        case 3:
            if (p->curMotion == 0xF) {
                f32 maxTime = Hu3DMotionMaxTimeGet(p->modelId);

                if (Hu3DMotionTimeGet(p->modelId) >= maxTime) {
                    p->timerA = 0;
                    p->sub = 0;
                    p->invuln = 0x5A;
                    done = 1;
                }
            }
            break;
    }
    return done;
}

/* state 4: hit by a flame (stage 1) */
s32 fn_1_A02C(M562Player *p)
{
    s32 done = 0;

    switch (p->sub) {
        case 0:
            p->hp--;
            omVibrate(p->playerNo, 0x1E, 4, 2);
            CharFXPlay((s16) p->charNo, 0x1F9);
            if (p->held != 0 || p->target != 0) {
                fn_1_9AEC(p);
            }
            p->reqMotion = 0xD;
            p->loopMotion = 1;
            p->vy = sqrtf(1200.0f);
            p->sub = 1;
            /* fallthrough */
        case 1:
            p->vy -= 1.5f;
            p->pos.y += p->vy;
            if (p->vy < 0.0f) {
                if (p->pos.y <= 0.0f) {
                    p->pos.y = 0.0f;
                    p->vy = 0.0f;
                    p->reqMotion = 0xE;
                    p->loopMotion = 1;
                    p->timerA = 0x3C;
                    p->sub = 2;
                }
            }
            break;
        case 2:
            fn_1_710C(p);
            if (p->timerA-- <= 0) {
                p->reqMotion = 0xF;
                p->loopMotion = 0;
                p->timerA = 0x3C;
                p->sub = 3;
            }
            break;
        case 3:
            if (p->curMotion == 0xF) {
                f32 maxTime = Hu3DMotionMaxTimeGet(p->modelId);

                if (Hu3DMotionTimeGet(p->modelId) >= maxTime) {
                    p->timerA = 0;
                    p->sub = 0;
                    p->invuln = 0x5A;
                    done = 1;
                }
            }
            break;
    }
    return done;
}

/* state 5: the ring model plays over the player */
s32 fn_1_A48C(M562Player *p)
{
    s32 done = 0;

    switch (p->sub) {
        case 0:
            p->hp--;
            omVibrate(p->playerNo, 0x1E, 4, 2);
            if (p->held != 0 || p->target != 0) {
                fn_1_9AEC(p);
            }
            p->rotY = 0.0f;
            p->goalRotY = 0.0f;
            p->rot.y = p->rotY;
            Hu3DModelPosSet(p->ringModel, p->pos.x, p->pos.y, p->pos.z);
            Hu3DModelAttrReset(p->ringModel, 1);
            p->reqMotion = 0x11;
            p->loopMotion = 0;
            p->timerA = 0;
            p->sub = 1;
            /* fallthrough */
        case 1:
            Hu3DModelPosSet(p->ringModel, p->pos.x,
                (f32) (p->pos.y + 200.0 * sin(PL_RAD(180.0f * ((f32) p->timerA / 93.0f)))), p->pos.z);
            p->timerA++;
            if (p->timerA >= 0x5D) {
                Hu3DModelAttrSet(p->ringModel, 1);
                p->reqMotion = 0xE;
                p->loopMotion = 1;
                p->timerA = 0x3C;
                p->sub = 2;
            }
            break;
        case 2:
            fn_1_710C(p);
            if (p->timerA-- <= 0) {
                p->reqMotion = 0xF;
                p->loopMotion = 0;
                p->timerA = 0x3C;
                p->sub = 3;
            }
            break;
        case 3:
            if (p->curMotion == 0xF) {
                f32 maxTime = Hu3DMotionMaxTimeGet(p->modelId);

                if (Hu3DMotionTimeGet(p->modelId) >= maxTime) {
                    p->timerA = 0;
                    p->sub = 0;
                    p->invuln = 0x5A;
                    done = 1;
                }
            }
            break;
    }
    return done;
}

/* state 6: knocked away by the Bowser actor (stage 4) */
s32 fn_1_A87C(M562Player *p)
{
    Koopa *koopa = &p->owner->koopa;
    s32 done = 0;

    switch (p->sub) {
        case 0:
            p->hp--;
            omVibrate(p->playerNo, 0x1E, 4, 2);
            if (p->held != 0 || p->target != 0) {
                fn_1_9AEC(p);
            }
            p->reqMotion = 0xC;
            p->loopMotion = 1;
            p->vy = sqrtf(1200.0f);
            p->sub = 1;
            /* fallthrough */
        case 1:
            p->vy -= 1.5f;
            p->pos.y += p->vy;
            p->pos.x = (f32) (p->pos.x + 10.0 * sin(PL_RAD(koopa->rotY)));
            p->pos.z = (f32) (p->pos.z + 10.0 * cos(PL_RAD(koopa->rotY)));
            if (p->vy < 0.0f) {
                if (p->pos.y <= 0.0f) {
                    p->pos.y = 0.0f;
                    p->vy = 0.0f;
                    p->reqMotion = 0xE;
                    p->loopMotion = 1;
                    p->timerA = 0x3C;
                    p->sub = 2;
                }
            }
            break;
        case 2:
            fn_1_710C(p);
            if (p->timerA-- <= 0) {
                p->reqMotion = 0xF;
                p->loopMotion = 0;
                p->timerA = 0x3C;
                p->sub = 3;
            }
            break;
        case 3:
            if (p->curMotion == 0xF) {
                f32 maxTime = Hu3DMotionMaxTimeGet(p->modelId);

                if (Hu3DMotionTimeGet(p->modelId) >= maxTime) {
                    p->timerA = 0;
                    p->sub = 0;
                    p->invuln = 0x5A;
                    done = 1;
                }
            }
            break;
    }
    return done;
}

/* a hit on the player: skip ahead to the recovery phase if already knocked down */
void fn_1_AD68(M562Player *boss)
{
    if (boss->state == 2) {
        if (boss->sub < 3) {
            boss->sub = 3;
        }
    } else {
        boss->sub = 3;
    }
}

/* turn the player towards `dir` at `rate`; returns 1 once facing it */
s32 fn_1_AD98(M562Player *p, Vec *dir, f32 rate)
{
    Vec fwd;
    Vec want;
    f32 diff;
    f32 target;
    f32 absDiff;
    s32 facing = 0;

    fwd.x = sin(PL_RAD(p->rotY));
    fwd.y = 0.0f;
    fwd.z = cos(PL_RAD(p->rotY));
    want.x = dir->x;
    want.y = 0.0f;
    want.z = dir->z;
    if (want.x != 0.0f || want.z != 0.0f) {
        PSVECNormalize(&want, &want);
    } else {
        want.z = 1.0f;
    }
    diff = (f32) acos(PSVECDotProduct(&fwd, &want));
    diff = 180.0 * (diff / M_PI);
    target = 180.0 * (atan2(want.x, want.z) / M_PI);
    target = PL_WRAP_F(target);
    if (diff >= 0.0f) {
        absDiff = diff;
    } else {
        absDiff = -diff;
    }
    if (absDiff > 3.0f) {
        p->reqMotion = 1;
        p->loopMotion = 1;
        p->rotY = fn_1_36EC(target, p->rotY, rate);
        p->rot.y = p->rotY;
    } else {
        p->reqMotion = 0;
        p->loopMotion = 1;
        facing = 1;
    }
    return facing;
}

/* the pad snapshot of one player */
void fn_1_B07C(M562PlayerInput *inp, s32 playerNo, M562Player *owner)
{
    memset(inp, 0, sizeof(M562PlayerInput));
    inp->owner = owner;
    inp->padNo = GwPlayerConf[playerNo].padNo;
}

void fn_1_B0E0(M562PlayerInput *inp)
{
    memset(inp, 0, sizeof(M562PlayerInput));
}

void fn_1_B110(M562PlayerInput *inp)
{
    s32 absX;
    s32 absY;
    s8 a;
    s8 b;

    inp->btn = HuPadBtn[inp->padNo];
    inp->btnDown = HuPadBtnDown[inp->padNo];
    inp->btnRep = HuPadBtnRep[inp->padNo];
    inp->stkX = HuPadStkX[inp->padNo];
    if (inp->stkX >= 0) {
        absX = inp->stkX;
    } else {
        absX = -inp->stkX;
    }
    if (absX < 10) {
        a = 0;
    } else {
        a = inp->stkX;
    }
    inp->stkX = a;
    inp->stkY = HuPadStkY[inp->padNo];
    if (inp->stkY >= 0) {
        absY = inp->stkY;
    } else {
        absY = -inp->stkY;
    }
    if (absY < 10) {
        b = 0;
    } else {
        b = inp->stkY;
    }
    inp->stkY = b;
    inp->subX = HuPadSubStkX[inp->padNo];
    inp->subY = HuPadSubStkY[inp->padNo];
    inp->trigL = HuPadTrigL[inp->padNo];
    inp->trigR = HuPadTrigR[inp->padNo];
    inp->dstk = HuPadDStk[inp->padNo];
    inp->dstkRep = HuPadDStkRep[inp->padNo];
}
