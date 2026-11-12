#include "REL/m510dll.h"
#include "string.h"
#include "game/printfunc.h"

// .data:0x0 (absolute 0x3D0) - field of view of the debug camera
f32 lbl_1_data_3D0 = 2.0f;

char lbl_1_data_3D4[6] = "PX:%f";
char lbl_1_data_3DA[6] = "PY:%f";
char lbl_1_data_3E0[6] = "PZ:%f";
char lbl_1_data_3E6[6] = "IX:%f";
char lbl_1_data_3EC[6] = "IY:%f";
char lbl_1_data_3F2[6] = "IZ:%f";
char lbl_1_data_3F8[8] = "FOVY:%f";

void fn_1_89B8(M510_OPEN_CAM *cam, M510_OBJ *obj)
{
    M510_VECF up = { 0.0f, 1.0f, 0.0f };
    M510_VECF pos = { 380.0f, 1.0f, 1.0f };
    M510_VECF look = { 380.0f, 104.0f, 400.0f };

    pos.y = 7300.0f;
    pos.z = 29835.0f;
    memset(cam, 0, sizeof(M510_OPEN_CAM));
    cam->obj = obj;
    cam->unk04 = 1;
    fn_1_8B1C(cam, cam->unk04);
    cam->up = up;
    cam->cur = pos;
    cam->tgt = cam->cur;
    cam->dst = look;
    cam->ofs = cam->dst;
    cam->speed = 0.4f;
}

void fn_1_8B1C(M510_OPEN_CAM *cam, s32 camId)
{
    Hu3DCameraCreate(camId);
    Hu3DCameraPerspectiveSet(camId, 2.0f, 400.0f, 100000.0f, 1.2f);
    Hu3DCameraViewportSet(camId, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(camId, 0, 0, 640, 480);
}

void fn_1_8BEC(M510_OPEN_CAM *cam)
{
    memset(cam, 0, sizeof(M510_OPEN_CAM));
}

void fn_1_8C1C(M510_OPEN_CAM *cam)
{
    M510_VECF posDiff;
    M510_VECF lookDiff;
    M510_VECF pos;
    M510_VECF look;
    M510_VECF zero = { 0.0f, 0.0f, 0.0f };
    M510_OBJ *obj = cam->obj;

    PSVECSubtract(&cam->tgt, &cam->cur, &posDiff);
    PSVECSubtract(&cam->ofs, &cam->dst, &lookDiff);
    PSVECScale(&posDiff, &posDiff, cam->speed);
    PSVECScale(&lookDiff, &lookDiff, cam->speed);
    PSVECAdd(&cam->cur, &posDiff, &cam->cur);
    PSVECAdd(&cam->dst, &lookDiff, &cam->dst);
    cam->ofsOfs = zero;
    cam->posOfs = cam->ofsOfs;
    PSVECAdd(&cam->cur, &cam->posOfs, &pos);
    PSVECAdd(&cam->dst, &cam->ofsOfs, &look);
    Hu3DCameraPosSetV(cam->unk04, &pos, &cam->up, &look);
}

void fn_1_8D48(M510_OPEN_CAM *cam)
{
    M510_VECF dir;
    M510_VECF base;
    Mtx rotY;
    Mtx rotX;
    f32 len;
    f32 stickX;
    f32 stickY;
    f32 fx;
    f32 fy;
    f32 fov;

    base = cam->dst;
    PSVECSubtract(&cam->cur, &cam->dst, &dir);
    len = PSVECMag(&dir);
    if ((HuPadBtn[0] & 0x4000) != 0 && (HuPadBtn[0] & 0x2000) == 0) {
        stickX = (f32)HuPadSubStkX[0] / 59.0f;
        stickY = -((f32)HuPadSubStkY[0] / 59.0f);
        if (((stickX >= 0.0f) ? stickX : -stickX) < 0.1f) {
            fx = 0.0f;
        } else {
            fx = stickX;
        }
        if (((stickY >= 0.0f) ? stickY : -stickY) < 0.1f) {
            fy = 0.0f;
        } else {
            fy = stickY;
        }
        PSMTXRotRad(rotY, 0x59, 0.05f * fx);
        PSMTXRotRad(rotX, 0x58, 0.05f * fy);
        PSMTXMultVec(rotY, &dir, &dir);
        PSMTXMultVec(rotX, &dir, &dir);
        PSVECAdd(&dir, &base, &base);
        cam->tgt = base;
        cam->cur = cam->tgt;
        cam->dst = cam->ofs;
        if ((HuPadBtn[0] & 0x800) != 0) {
            fov = 0.2f + lbl_1_data_3D0;
            if (fov > 90.0f) {
                fov = 90.0f;
            } else if (fov < 0.0f) {
                fov = 0.0f;
            }
            lbl_1_data_3D0 = fov;
            Hu3DCameraPerspectiveSet(cam->unk04, lbl_1_data_3D0, 400.0f, 100000.0f, 1.2f);
        } else if ((HuPadBtn[0] & 0x400) != 0) {
            fov = lbl_1_data_3D0 - 0.2f;
            if (fov > 90.0f) {
                fov = 90.0f;
            } else if (fov < 0.0f) {
                fov = 0.0f;
            }
            lbl_1_data_3D0 = fov;
            Hu3DCameraPerspectiveSet(cam->unk04, lbl_1_data_3D0, 400.0f, 100000.0f, 1.2f);
        }
    } else if ((HuPadBtn[0] & 0x4000) == 0 && (HuPadBtn[0] & 0x2000) != 0) {
        PSVECScale(&dir, &dir, 1.0f + (0.02f * -((f32)HuPadSubStkY[0] / 59.0f)));
        PSVECAdd(&dir, &base, &base);
        cam->tgt = base;
        cam->cur = cam->tgt;
        cam->dst = cam->ofs;
    } else if ((HuPadBtn[0] & 0x4000) != 0 && (HuPadBtn[0] & 0x2000) != 0) {
        fx = ((f32)HuPadSubStkX[0] / 59.0f) * 50.0f;
        fy = ((f32)HuPadSubStkY[0] / 59.0f) * 50.0f;
        cam->tgt.x = cam->tgt.x + fx;
        cam->tgt.y = cam->tgt.y + fy;
        cam->ofs.x = cam->ofs.x + fx;
        cam->ofs.y = cam->ofs.y + fy;
    }
    fontcolor = 3;
    print8(0x1B8, 0x150, 1.5f, lbl_1_data_3D4, cam->tgt.x);
    print8(0x1B8, 0x15C, 1.5f, lbl_1_data_3DA, cam->tgt.y);
    print8(0x1B8, 0x168, 1.5f, lbl_1_data_3E0, cam->tgt.z);
    print8(0x1B8, 0x174, 1.5f, lbl_1_data_3E6, cam->ofs.x);
    print8(0x1B8, 0x180, 1.5f, lbl_1_data_3EC, cam->ofs.y);
    print8(0x1B8, 0x18C, 1.5f, lbl_1_data_3F2, cam->ofs.z);
    print8(0x1B8, 0x198, 1.5f, lbl_1_data_3F8, lbl_1_data_3D0);
}
