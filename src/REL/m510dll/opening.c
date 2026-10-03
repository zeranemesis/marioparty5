#include "game/hu3d.h"
#include "humath.h"

#include "REL/m510dll.h"

/* Default target of the first half of the slide. */
static Vec lbl_1_data_420 = { 0.0f, 3000.0f, 16000.0f };
/* Default target of the second half of the slide. */
static Vec lbl_1_data_42C = { 0.0f, -52.0f, 400.0f };
/* Start and end of the two interpolated Vecs; filled in on the first phase. */
static Vec lbl_1_data_438 = { 0.0f, 0.0f, 0.0f };
static Vec lbl_1_data_444 = { 0.0f, 0.0f, 0.0f };

// The declaration order below is load-bearing. MWCC fills the whole local block
// in order, so the loop counter has to come first: with `i` between the
// pointers the four M510_VIEWOBJ addresses are emitted r29,r27,r26,r25 instead
// of r27,r26,r25,r29. `.text` is a byte-for-byte match.
s32 fn_1_966C(M510_WORK *work)
{
    M510_SLIDE *slide = &work->slide;
    M510_VIEW *slot0 = &work->view0;
    M510_VIEW *slot1 = &work->view1;
    M510_VIEW *slot2 = &work->view2;
    M510_VIEW *slot3 = &work->view3;
    s32 i;
    M510_VIEWOBJ *view0 = &slot0->obj;
    M510_VIEWOBJ *view1 = &slot1->obj;
    M510_VIEWOBJ *view2 = &slot2->obj;
    M510_VIEWOBJ *view3 = &slot3->obj;
    M510_SLOT *slot;
    f32 rate;
    s32 result;

    result = 0;

    switch (work->phase) {
    case 0:
        lbl_1_data_438 = slide->from;
        lbl_1_data_444 = slide->to;
        slide->from.x = slide->x = view3->offset;
        slide->from.y = slide->y = lbl_1_data_420.y;
        slide->from.z = slide->z = lbl_1_data_420.z;
        slide->to.x = slide->x2 = view3->offset;
        slide->to.y = slide->y2 = lbl_1_data_42C.y;
        slide->to.z = slide->z2 = lbl_1_data_42C.z;
        slide->fade = 0.1f;
        work->frame = 0;
        work->time = 0;
        work->phase++;
    case 1:
        if (work->fieldId < 0) {
            work->fieldId = fn_1_734(0x4E);
        }
        rate = work->frame / 90.0f;
        slide->from.x = slide->to.x =
            view3->offset + rate * (lbl_1_data_438.x - view3->offset);
        if (work->frame++ > 0x5A) {
            work->frame = 0;
            work->phase++;
        }
        break;
    case 2:
        rate = work->frame / 30.0f;
        slide->from.x = slide->from.x + rate * (lbl_1_data_438.x - slide->from.x);
        slide->from.y = slide->from.y + rate * (lbl_1_data_438.y - slide->from.y);
        slide->from.z = slide->from.z + rate * (lbl_1_data_438.z - slide->from.z);
        slide->to.x = slide->to.x + rate * (lbl_1_data_444.x - slide->to.x);
        slide->to.y = slide->to.y + rate * (lbl_1_data_444.y - slide->to.y);
        slide->to.z = slide->to.z + rate * (lbl_1_data_444.z - slide->to.z);
        if (work->frame++ > 0x1E) {
            work->frame = 0;
            work->phase++;
        }
        break;
    case 3:
        if (work->frame++ > 5) {
            for (i = 0; i < 4; i++) {
                slot = (M510_SLOT *)((u8 *)work + i * 0x880 + 0x134);
                slot->state = 3;
                slot->activeF = 1;
            }
            work->frame = 0;
            work->phase++;
        }
        break;
    case 4:
        if (work->frame++ > 0x64) {
            work->frame = 0;
            work->phase++;
        }
        break;
    case 5:
        slide->fade = 0.4f;
        work->phase = 0U;
        result = 1;
        break;
    }

    if (work->phase != 0) {
        if (work->time == 0x14) {
            view3->showF = 1;
        } else if (work->time == 0x3C) {
            view2->showF = 1;
        } else if (work->time == 0x64) {
            view1->showF = 1;
        } else if (work->time == 0x8C) {
            view0->showF = 1;
        } else if (work->time == 0xB8) {
            view0->showF = 1;
            view1->showF = 1;
            view2->showF = 1;
            view3->showF = 1;
        }
        work->time = work->time + 1;
    }
    return result;
}
