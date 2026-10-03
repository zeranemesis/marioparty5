#include "REL/m510dll.h"

// .data:0x0 (absolute 0x420) - 4 x HuVecF
HuVecF lbl_1_data_420[4] = {
    { 0.0f, 3000.0f, 16000.0f },
    { 0.0f, 3000.0f, 16000.0f },
    { 0.0f, 3000.0f, 16000.0f },
    { 0.0f, 3000.0f, 16000.0f },
};

// .data:0xC (absolute 0x42C) - 4 x HuVecF
HuVecF lbl_1_data_42C[4] = {
    { 0.0f, -52.0f, 400.0f },
    { 0.0f, -52.0f, 400.0f },
    { 0.0f, -52.0f, 400.0f },
    { 0.0f, -52.0f, 400.0f },
};

// .data:0x18 (absolute 0x438) - 4 x HuVecF, all zero
HuVecF lbl_1_data_438[4] = {
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
};

// .data:0x24 (absolute 0x444) - 4 x HuVecF, all zero
HuVecF lbl_1_data_444[4] = {
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
};

s32 fn_1_966C(M510_OBJ *obj)
{
    M510_OPEN_CAM *cam = &obj->cam;
    M510_OBJ *self = obj;
    M510_PLAYER_WORK *p0 = &obj->players[0];
    M510_PLAYER_WORK *p1 = &obj->players[1];
    M510_PLAYER_WORK *p2 = &obj->players[2];
    M510_PLAYER_WORK *p3 = &obj->players[3];
    M510_PLAYER_MODEL_WORK *m0 = &p0->model;
    M510_PLAYER_MODEL_WORK *m1 = &p1->model;
    M510_PLAYER_MODEL_WORK *m2 = &p2->model;
    M510_PLAYER_MODEL_WORK *m3 = &p3->model;
    s32 ret = 0;
    s32 i;
    float t;

    switch (self->state) {
    case 0:
        lbl_1_data_438[0] = cam->tgt;
        lbl_1_data_444[0] = cam->ofs;
        cam->tgt.x = cam->cur.x = m3->camPos.x;
        cam->tgt.y = cam->cur.y = lbl_1_data_420[0].y;
        cam->tgt.z = cam->cur.z = lbl_1_data_420[0].z;
        cam->ofs.x = cam->dst.x = m3->camPos.x;
        cam->ofs.y = cam->dst.y = lbl_1_data_42C[0].y;
        cam->ofs.z = cam->dst.z = lbl_1_data_42C[0].z;
        cam->speed = 0.1f;
        self->timer1 = 0;
        self->timer2 = 0;
        self->state++;
        // fall through
    case 1:
        if (self->soundId < 0) {
            self->soundId = fn_1_734(0x4E);
        }
        t = self->timer1 / 90.0f;
        cam->tgt.x = cam->ofs.x = m3->camPos.x
            + t * (lbl_1_data_438[0].x - m3->camPos.x);
        if (self->timer1++ > 0x5A) {
            self->timer1 = 0;
            self->state++;
        }
        break;
    case 2:
        t = self->timer1 / 30.0f;
        cam->tgt.x += t * (lbl_1_data_438[0].x - cam->tgt.x);
        cam->tgt.y += t * (lbl_1_data_438[0].y - cam->tgt.y);
        cam->tgt.z += t * (lbl_1_data_438[0].z - cam->tgt.z);
        cam->ofs.x += t * (lbl_1_data_444[0].x - cam->ofs.x);
        cam->ofs.y += t * (lbl_1_data_444[0].y - cam->ofs.y);
        cam->ofs.z += t * (lbl_1_data_444[0].z - cam->ofs.z);
        if (self->timer1++ > 0x1E) {
            self->timer1 = 0;
            self->state++;
        }
        break;
    case 3:
        if (self->timer1++ > 5) {
            for (i = 0; i < 4; i++) {
                M510_PITCH_WORK *pitch = &obj->players[i].pitch;
                pitch->state = 3;
                pitch->unk24 = 1;
            }
            self->timer1 = 0;
            self->state++;
        }
        break;
    case 4:
        if (self->timer1++ > 0x64) {
            self->timer1 = 0;
            self->state++;
        }
        break;
    case 5:
        cam->speed = 0.4f;
        self->state = 0;
        ret = 1;
        break;
    }

    if (self->state != 0) {
        if (self->timer2 == 0x14) {
            m3->flag = 1;
        }
        else if (self->timer2 == 0x3C) {
            m2->flag = 1;
        }
        else if (self->timer2 == 0x64) {
            m1->flag = 1;
        }
        else if (self->timer2 == 0x8C) {
            m0->flag = 1;
        }
        else if (self->timer2 == 0xB8) {
            m0->flag = 1;
            m1->flag = 1;
            m2->flag = 1;
            m3->flag = 1;
        }
        self->timer2++;
    }
    return ret;
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
