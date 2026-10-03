#define _MATH_H
#include "REL/m510dll.h"

// .data:0x0 (absolute 0x420)
HuVecF lbl_1_data_420 = { 0.0f, 3000.0f, 16000.0f };

// .data:0xC (absolute 0x42C)
HuVecF lbl_1_data_42C = { 0.0f, -52.0f, 400.0f };

// .data:0x18 (absolute 0x438)
HuVecF lbl_1_data_438 = { 0.0f, 0.0f, 0.0f };

// .data:0x24 (absolute 0x444)
HuVecF lbl_1_data_444 = { 0.0f, 0.0f, 0.0f };

s32 fn_1_966C(M510_OBJ *obj)
{
    M510_OPEN_CAM *cam = &obj->cam;
    M510_PLAYER_WORK *p0 = &obj->players[0];
    M510_PLAYER_WORK *p1 = &obj->players[1];
    M510_PLAYER_WORK *p2 = &obj->players[2];
    M510_PLAYER_WORK *p3 = &obj->players[3];
    s32 i;
    M510_PLAYER_MODEL_WORK *m0 = &p0->model;
    M510_PLAYER_MODEL_WORK *m1 = &p1->model;
    M510_PLAYER_MODEL_WORK *m2 = &p2->model;
    M510_PLAYER_MODEL_WORK *m3 = &p3->model;
    M510_PITCH_WORK *pitch;
    s32 ret;
    float t;

    ret = 0;

    switch (obj->state) {
    case 0:
        lbl_1_data_438 = cam->tgt;
        lbl_1_data_444 = cam->ofs;
        cam->tgt.x = cam->cur.x = m3->camPos.x;
        cam->tgt.y = cam->cur.y = lbl_1_data_420.y;
        cam->tgt.z = cam->cur.z = lbl_1_data_420.z;
        cam->ofs.x = cam->dst.x = m3->camPos.x;
        cam->ofs.y = cam->dst.y = lbl_1_data_42C.y;
        cam->ofs.z = cam->dst.z = lbl_1_data_42C.z;
        cam->speed = 0.1f;
        obj->timer1 = 0;
        obj->timer2 = 0;
        obj->state++;
        // fall through
    case 1:
        if (obj->soundId < 0) {
            obj->soundId = fn_1_734(0x4E);
        }
        t = obj->timer1 / 90.0f;
        cam->tgt.x = cam->ofs.x = m3->camPos.x
            + t * (lbl_1_data_438.x - m3->camPos.x);
        if (obj->timer1++ > 0x5A) {
            obj->timer1 = 0;
            obj->state++;
        }
        break;
    case 2:
        t = obj->timer1 / 30.0f;
        cam->tgt.x = cam->tgt.x + t * (lbl_1_data_438.x - cam->tgt.x);
        cam->tgt.y = cam->tgt.y + t * (lbl_1_data_438.y - cam->tgt.y);
        cam->tgt.z = cam->tgt.z + t * (lbl_1_data_438.z - cam->tgt.z);
        cam->ofs.x = cam->ofs.x + t * (lbl_1_data_444.x - cam->ofs.x);
        cam->ofs.y = cam->ofs.y + t * (lbl_1_data_444.y - cam->ofs.y);
        cam->ofs.z = cam->ofs.z + t * (lbl_1_data_444.z - cam->ofs.z);
        if (obj->timer1++ > 0x1E) {
            obj->timer1 = 0;
            obj->state++;
        }
        break;
    case 3:
        if (obj->timer1++ > 5) {
            for (i = 0; i < 4; i++) {
                pitch = &obj->players[i].pitch;
                pitch->state = 3;
                pitch->unk24 = 1;
            }
            obj->timer1 = 0;
            obj->state++;
        }
        break;
    case 4:
        if (obj->timer1++ > 0x64) {
            obj->timer1 = 0;
            obj->state++;
        }
        break;
    case 5:
        cam->speed = 0.4f;
        obj->state = 0;
        ret = 1;
        break;
    }

    if (obj->state != 0) {
        if (obj->timer2 == 0x14) {
            m3->flag = 1;
        }
        else if (obj->timer2 == 0x3C) {
            m2->flag = 1;
        }
        else if (obj->timer2 == 0x64) {
            m1->flag = 1;
        }
        else if (obj->timer2 == 0x8C) {
            m0->flag = 1;
        }
        else if (obj->timer2 == 0xB8) {
            m0->flag = 1;
            m1->flag = 1;
            m2->flag = 1;
            m3->flag = 1;
        }
        obj->timer2++;
    }
    return ret;
}
