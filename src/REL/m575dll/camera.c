// REL/m575dll/camera.c - the per-side camera, its split-screen setup and the
// free-fly debug controls.
#include "REL/m575dll.h"
#include "game/hu3d.h"
#include "game/pad.h"
#include "game/printfunc.h"
#include "string.h"
#include "math.h"

void espDispOn(s16 espId);
void espPosSet(s16 espId, float posX, float posY);
void fn_1_86A8(M575Camera *self, s32 camBit);

s32 lbl_1_data_278[8] = { 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80 };
char lbl_1_data_2B6[10] = "IZ:%f";

void fn_1_851C(M575Camera *self, s16 side, M575Main *main)
{
    Vec up;
    Vec pos;
    Vec tgt;

    memset(self, 0, sizeof(M575Camera));
    self->main = main;
    self->camBit = lbl_1_data_278[side];
    fn_1_86A8(self, self->camBit);
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    self->up = up;
    pos.x = 0.0f;
    pos.y = 1370.0f;
    pos.z = 1270.0f;
    self->pos = pos;
    self->posTarget = self->pos;
    tgt.x = 0.0f;
    tgt.y = 200.0f;
    tgt.z = 0.0f;
    self->tgt = tgt;
    self->tgtTarget = self->tgt;
    self->lerp = 0.4f;
    if (side != 0) {
        Hu3DCameraScissorSet(self->camBit, 0, 0, 0, 0);
    }
}

void fn_1_86A8(M575Camera *self, s32 camBit)
{
    Hu3DCameraCreate(camBit);
    Hu3DCameraPerspectiveSet(camBit, 45.0f, 20.0f, 80000.0f, 1.2f);
    Hu3DCameraViewportSet(camBit, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(camBit, 0, 0, 640, 480);
}

void fn_1_8778(M575Camera *self)
{
    memset(self, 0, sizeof(M575Camera));
}

void fn_1_87A8(M575Camera *self)
{
    M575Game *game = self->main->game;
    Vec delta1;
    Vec delta2;
    Vec outPos;
    Vec outTgt;
    Vec shake;
    f32 wave;

    PSVECSubtract(&self->posTarget, &self->pos, &delta1);
    PSVECSubtract(&self->tgtTarget, &self->tgt, &delta2);
    PSVECScale(&delta1, &delta1, self->lerp);
    PSVECScale(&delta2, &delta2, self->lerp);
    PSVECAdd(&self->pos, &delta1, &self->pos);
    PSVECAdd(&self->tgt, &delta2, &self->tgt);
    if (self->shakeOn != 0) {
        wave = (f32)(3.0 * sin(3.141592653589793 * (f64)(360.0f * ((f32)(game->frame % 4) * 0.25f)) / 180.0));
        shake.x = 0.0f;
        shake.y = 0.0f;
        shake.z = 0.0f;
        shake.x = wave;
        shake.y = wave;
        self->shake2 = shake;
        self->shake = self->shake2;
    }
    else {
        shake.x = 0.0f;
        shake.y = 0.0f;
        shake.z = 0.0f;
        self->shake2 = shake;
        self->shake = self->shake2;
    }
    PSVECAdd(&self->pos, &self->shake, &outPos);
    PSVECAdd(&self->tgt, &self->shake2, &outTgt);
    Hu3DCameraPosSetV(self->camBit, &outPos, &self->up, &outTgt);
}

// Split-screen: `a` gets the left (1 - t) part of the screen, `b` the right t part.
void fn_1_89D8(M575Camera *a, M575Camera *b, f32 t)
{
    M575Game *game = a->main->game;

    espDispOn(game->esp);
    espPosSet(game->esp, 576.0f * (1.0f - t), 240.0f);
    Hu3DCameraPerspectiveSet(a->camBit, 45.0f, 20.0f, 80000.0f, (576.0f * (1.0f - t)) / 480.0f);
    Hu3DCameraViewportSet(a->camBit, 0.0f, 0.0f, 640.0f * (1.0f - t), 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(a->camBit, 0, 0, (u32)(640.0f * (1.0f - t)), 480);
    Hu3DCameraPerspectiveSet(b->camBit, 45.0f, 20.0f, 80000.0f, (576.0f * t) / 480.0f);
    Hu3DCameraViewportSet(b->camBit, 640.0f * (1.0f - t), 0.0f, 640.0f * t, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(b->camBit, (u32)(640.0f * (1.0f - t)), 0, (u32)(640.0f * t), 480);
}

// Free-fly debug camera driven by the sub stick of pad 0.
void fn_1_8C68(M575Camera *self)
{
    Mtx rotY;
    Mtx rotX;
    Vec dir;
    Vec focus;
    f32 dist;
    f32 fx;
    f32 fy;
    f32 dx;
    f32 dy;
    f32 ax;
    f32 ay;
    f32 mx;
    f32 my;

    focus = self->tgt;
    PSVECSubtract(&self->pos, &self->tgt, &dir);
    dist = PSVECMag(&dir);
    if ((HuPadBtn[0] & 0x4000) != 0 && (HuPadBtn[0] & 0x2000) == 0) {
        fx = (f32)HuPadSubStkX[0] / 59.0f;
        fy = -((f32)HuPadSubStkY[0] / 59.0f);
        ax = (fx >= 0.0f) ? fx : -fx;
        if (ax < 0.1f) {
            dx = 0.0f;
        }
        else {
            dx = fx;
        }
        ay = (fy >= 0.0f) ? fy : -fy;
        if (ay < 0.1f) {
            dy = 0.0f;
        }
        else {
            dy = fy;
        }
        PSMTXRotRad(rotY, 'Y', 0.05f * dx);
        PSMTXRotRad(rotX, 'X', 0.05f * dy);
        PSMTXMultVec(rotY, &dir, &dir);
        PSMTXMultVec(rotX, &dir, &dir);
        PSVECAdd(&dir, &focus, &focus);
        self->posTarget = focus;
        self->pos = self->posTarget;
        self->tgt = self->tgtTarget;
    }
    else if ((HuPadBtn[0] & 0x4000) == 0 && (HuPadBtn[0] & 0x2000) != 0) {
        PSVECScale(&dir, &dir, 1.0f + 0.02f * -((f32)HuPadSubStkY[0] / 59.0f));
        PSVECAdd(&dir, &focus, &focus);
        self->posTarget = focus;
        self->pos = self->posTarget;
        self->tgt = self->tgtTarget;
    }
    else if ((HuPadBtn[0] & 0x4000) != 0 && (HuPadBtn[0] & 0x2000) != 0) {
        mx = ((f32)HuPadSubStkX[0] / 59.0f) * 50.0f;
        my = ((f32)HuPadSubStkY[0] / 59.0f) * 50.0f;
        self->posTarget.x += mx;
        self->posTarget.y += my;
        self->tgtTarget.x += mx;
        self->tgtTarget.y += my;
    }
    fontcolor = 1;
    print8(0x28, 0x150, 1.5f, "PX:%f", self->posTarget.x);
    print8(0x28, 0x15C, 1.5f, "PY:%f", self->posTarget.y);
    print8(0x28, 0x168, 1.5f, "PZ:%f", self->posTarget.z);
    print8(0x28, 0x174, 1.5f, "IX:%f", self->tgtTarget.x);
    print8(0x28, 0x180, 1.5f, "IY:%f", self->tgtTarget.y);
    print8(0x28, 0x18C, 1.5f, lbl_1_data_2B6, self->tgtTarget.z);
}
