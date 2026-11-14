// m562dll/camera - the scene camera: four follow modes, a shake effect and a
// debug free-fly camera.
#include <humath.h>
#include <math.h>
#include <string.h>

#include "game/pad.h"
#include "game/printfunc.h"
#include "REL/m562dll.h"

/* fov of the debug camera, edited with the pad */
f32 lbl_1_data_AE0 = 45.0f;

char lbl_1_data_AE4[6] = "PX:%f";
char lbl_1_data_AEA[6] = "PY:%f";
char lbl_1_data_AF0[6] = "PZ:%f";
char lbl_1_data_AF6[6] = "IX:%f";
char lbl_1_data_AFC[6] = "IY:%f";
char lbl_1_data_B02[6] = "IZ:%f";
char lbl_1_data_B08[8] = "FOVY:%f";

/* Vector constants the original keeps in .rodata and copies onto the stack
   (rodata 0x960..0xA30, in order). */
static const Vec kUp = {0.0f, 1.0f, 0.0f};
static const Vec kZeroA = {0.0f, 0.0f, 0.0f};
static const Vec kZeroB = {0.0f, 0.0f, 0.0f};
static const Vec kForward = {0.0f, 0.0f, -1.0f};
static const Vec kShakeOff[2] = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
static const Vec kUp2 = {0.0f, 1.0f, 0.0f};
static const Vec kUp3 = {0.0f, 1.0f, 0.0f};
static const Vec kModeAPos = {0.0f, 1000.0f, 1400.0f};
static const Vec kModeATgt = {0.0f, 100.0f, -200.0f};
static const Vec kModeBPos = {0.0f, 800.0f, 1400.0f};
static const Vec kModeBTgt = {0.0f, 100.0f, -200.0f};

// Resets the camera, creates the hardware camera and seeds every pose with
// the stage's starting view; with flag 0 the viewport is blanked.
void fn_1_20CE0(CAMERA *cam, s32 camId, s32 flag, CAMSTAGE *owner)
{
    Vec up;
    Vec tgt;
    Vec pos;
    Vec dir;

    memset(cam, 0, sizeof(CAMERA));
    cam->owner = owner;
    cam->camId = camId;
    cam->prevMode = 0;
    cam->mode = 0;
    fn_1_20EF4(cam, cam->camId);
    up = kUp;
    cam->up = up;
    pos = kZeroA;
    cam->pos = pos;
    cam->posGoal = cam->pos;
    tgt = kZeroB;
    cam->tgt = tgt;
    cam->tgtGoal = cam->tgt;
    dir = kForward;
    cam->dir = dir;
    cam->dirGoal = cam->dir;
    if (flag == 0) {
        Hu3DCameraViewportSet(camId, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    }
    cam->speed = 0.4f;
}

// Creates the hardware camera and clears the shake offsets.
void fn_1_20EF4(CAMERA *cam, s32 camId)
{
    Vec zero;

    Hu3DCameraCreate(camId);
    Hu3DCameraPerspectiveSet(camId, 45.0f, 20.0f, 50000.0f, 1.2f);
    Hu3DCameraViewportSet(camId, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(camId, 0, 0, 640, 480);
    zero = kShakeOff[0];
    cam->shakeTgt = zero;
    cam->shakePos = cam->shakeTgt;
}

void fn_1_2102C(CAMERA *cam)
{
    CAMERA *self = cam;

    memset(self, 0, sizeof(CAMERA));
}

// Per-frame update: runs the active mode, applies the shake, then remembers
// which mode it ran.
void fn_1_2105C(CAMERA *cam)
{
    switch (cam->mode) {
    case 0:
        fn_1_21280(cam);
        break;
    case 1:
        fn_1_21344(cam);
        break;
    case 2:
        fn_1_21700(cam);
        break;
    case 3:
        fn_1_2191C(cam);
        break;
    }
    fn_1_210F4(cam);
    cam->prevMode = cam->mode;
}

// Shake: while frames remain, a decaying sine offsets the camera on the axis
// the shake mode selects; when it runs out the offsets are cleared.
void fn_1_210F4(CAMERA *cam)
{
    Vec zero;
    f32 shake;

    if (cam->shakeFrames != 0) {
        shake = cam->shakeAmp * sin((M_PI * cam->shakePhase) / 180.0);
        cam->shakePhase += cam->shakeStep;
        cam->shakeAmp *= cam->shakeDecay;
        if (cam->shakeMode == 0 || cam->shakeMode == 3) {
            cam->shakePos.x = shake;
            cam->shakeTgt.x = shake;
        } else if (cam->shakeMode == 1 || cam->shakeMode == 3) {
            cam->shakePos.y = shake;
            cam->shakeTgt.y = shake;
        } else if (cam->shakeMode == 2 || cam->shakeMode == 3) {
            cam->shakePos.z = shake;
            cam->shakeTgt.z = shake;
        }
        if (--cam->shakeFrames == 0) {
            zero = kShakeOff[1];
            cam->shakeTgt = zero;
            cam->shakePos = cam->shakeTgt;
            cam->shakePhase = 0.0f;
            cam->shakeStep = 0.0f;
            cam->shakeDecay = 0.0f;
            cam->shakeAmp = 0.0f;
        }
    }
}

// Mode 0: ease the pose towards its goal and aim there.
void fn_1_21280(CAMERA *cam)
{
    Vec posStep;
    Vec tgtStep;
    Vec pos;
    Vec tgt;

    PSVECSubtract(&cam->posGoal, &cam->pos, &posStep);
    PSVECSubtract(&cam->tgtGoal, &cam->tgt, &tgtStep);
    PSVECScale(&posStep, &posStep, cam->speed);
    PSVECScale(&tgtStep, &tgtStep, cam->speed);
    PSVECAdd(&cam->pos, &posStep, &cam->pos);
    PSVECAdd(&cam->tgt, &tgtStep, &cam->tgt);
    PSVECAdd(&cam->pos, &cam->shakePos, &pos);
    PSVECAdd(&cam->tgt, &cam->shakeTgt, &tgt);
    Hu3DCameraPosSetV(cam->camId, &pos, &cam->up, &tgt);
}

/* Builds the camera basis (right, up, forward) of a forward vector into the
   3x4 matrix the quaternion conversion reads. */
#define BASIS(mtx, fwd, upConst)               \
    do {                                       \
        Vec f_;                                \
        Vec up_;                               \
        Vec right_;                            \
        f_.x = (fwd)->x;                       \
        f_.y = (fwd)->y;                       \
        f_.z = (fwd)->z;                       \
        up_ = (upConst);                       \
        PSVECCrossProduct(&f_, &up_, &right_); \
        PSVECCrossProduct(&f_, &right_, &up_); \
        (mtx)[0][0] = right_.x;                \
        (mtx)[1][0] = right_.y;                \
        (mtx)[2][0] = right_.z;                \
        (mtx)[0][1] = up_.x;                   \
        (mtx)[1][1] = up_.y;                   \
        (mtx)[2][1] = up_.z;                   \
        (mtx)[0][2] = f_.x;                    \
        (mtx)[1][2] = f_.y;                    \
        (mtx)[2][2] = f_.z;                    \
        (mtx)[0][3] = 0.0f;                    \
        (mtx)[1][3] = 0.0f;                    \
        (mtx)[2][3] = 0.0f;                    \
    } while (0)

// Mode 1: ease the position, but turn the view direction with a quaternion
// slerp from the current heading to the goal heading, keeping the distance.
void fn_1_21344(CAMERA *cam)
{
    Vec dist;
    Vec posStep;
    Vec pos;
    Vec tgt;
    Mtx m;
    Quaternion qCur;
    Quaternion qGoal;
    Quaternion qOut;
    f32 len;
    f32 mag;

    if (cam->prevMode != 1) {
        PSVECSubtract(&cam->tgt, &cam->pos, &dist);
        PSVECNormalize(&dist, &cam->dir);
        PSVECSubtract(&cam->tgtGoal, &cam->posGoal, &dist);
        PSVECNormalize(&dist, &cam->dirGoal);
    }
    PSVECSubtract(&cam->tgt, &cam->pos, &dist);
    len = PSVECMag(&dist);
    PSVECSubtract(&cam->posGoal, &cam->pos, &posStep);
    PSVECScale(&posStep, &posStep, cam->speed);
    PSVECAdd(&cam->pos, &posStep, &cam->pos);

    BASIS(m, &cam->dir, kUp2);
    C_QUATMtx(&qCur, m);
    BASIS(m, &cam->dirGoal, kUp3);
    C_QUATMtx(&qGoal, m);
    C_QUATSlerp(&qCur, &qGoal, &qOut, cam->slerpT);
    PSMTXQuat(m, &qOut);
    cam->dir.x = m[0][2];
    cam->dir.y = m[1][2];
    cam->dir.z = m[2][2];

    PSVECScale(&cam->dir, &dist, len);
    mag = PSVECMag(&dist);
    if (mag > 5.0f) {
        if (mag < 20.0f) {
            PSVECScale(&dist, &dist, 20.0f / mag);
        }
        PSVECAdd(&cam->pos, &dist, &cam->tgt);
    }
    PSVECAdd(&cam->pos, &cam->shakePos, &pos);
    PSVECAdd(&cam->tgt, &cam->shakeTgt, &tgt);
    Hu3DCameraPosSetV(cam->camId, &pos, &cam->up, &tgt);
}

// Mode 2: the stage's wide shot; the goal z follows Bowser's position.
void fn_1_21700(CAMERA *cam)
{
    Vec posStep;
    Vec tgtStep;
    Vec pos;
    Vec tgt;
    Vec dir;
    Vec vp;
    Vec vt;
    CAMKOOPA *koopa = &cam->owner->koopa;

    switch (cam->state) {
    case 0:
        vp = kModeAPos;
        cam->pos = vp;
        cam->posGoal = cam->pos;
        vt = kModeATgt;
        cam->tgt = vt;
        cam->tgtGoal = cam->tgt;
        cam->state++;
        /* fallthrough */
    case 1:
        cam->posGoal.z = 1400.0f + 0.25f * koopa->pos.z;
        cam->tgtGoal.z = 0.5f * koopa->pos.z - 200.0f;
        PSVECSubtract(&cam->posGoal, &cam->pos, &posStep);
        PSVECSubtract(&cam->tgtGoal, &cam->tgt, &tgtStep);
        PSVECScale(&posStep, &posStep, cam->speed);
        PSVECScale(&tgtStep, &tgtStep, cam->speed);
        PSVECAdd(&cam->pos, &posStep, &cam->pos);
        PSVECAdd(&cam->tgt, &tgtStep, &cam->tgt);
        PSVECAdd(&cam->pos, &cam->shakePos, &pos);
        PSVECAdd(&cam->tgt, &cam->shakeTgt, &tgt);
        Hu3DCameraPosSetV(cam->camId, &pos, &cam->up, &tgt);
        break;
    }
    PSVECSubtract(&cam->tgt, &cam->pos, &dir);
    PSVECNormalize(&dir, &dir);
    fn_1_904(&cam->pos, &dir);
}

// Mode 3: like mode 2 but the x goals follow Bowser as well.
void fn_1_2191C(CAMERA *cam)
{
    Vec posStep;
    Vec tgtStep;
    Vec pos;
    Vec tgt;
    Vec dir;
    Vec vp;
    Vec vt;
    CAMKOOPA *koopa = &cam->owner->koopa;

    switch (cam->state) {
    case 0:
        vp = kModeBPos;
        cam->pos = vp;
        cam->posGoal = cam->pos;
        vt = kModeBTgt;
        cam->tgt = vt;
        cam->tgtGoal = cam->tgt;
        cam->state++;
        /* fallthrough */
    case 1:
        cam->posGoal.x = 0.8f * koopa->pos.x;
        cam->tgtGoal.x = 0.6f * koopa->pos.x;
        cam->posGoal.z = 1400.0f + 0.25f * koopa->pos.z;
        cam->tgtGoal.z = 0.5f * koopa->pos.z - 200.0f;
        PSVECSubtract(&cam->posGoal, &cam->pos, &posStep);
        PSVECSubtract(&cam->tgtGoal, &cam->tgt, &tgtStep);
        PSVECScale(&posStep, &posStep, cam->speed);
        PSVECScale(&tgtStep, &tgtStep, cam->speed);
        PSVECAdd(&cam->pos, &posStep, &cam->pos);
        PSVECAdd(&cam->tgt, &tgtStep, &cam->tgt);
        PSVECAdd(&cam->pos, &cam->shakePos, &pos);
        PSVECAdd(&cam->tgt, &cam->shakeTgt, &tgt);
        Hu3DCameraPosSetV(cam->camId, &pos, &cam->up, &tgt);
        break;
    }
    PSVECSubtract(&cam->tgt, &cam->pos, &dir);
    PSVECNormalize(&dir, &dir);
    fn_1_904(&cam->pos, &dir);
}

// Starts (or strengthens) a shake: frame count and amplitude only ever grow;
// the decay and phase step are replaced and the phase restarts at 90.
void fn_1_21B68(CAMERA *cam, s16 mode, s32 frames, f32 amp, f32 decay, f32 step)
{
    cam->shakeMode = mode;
    if (cam->shakeFrames < frames) {
        cam->shakeFrames = frames;
    }
    if (cam->shakeAmp < amp) {
        cam->shakeAmp = amp;
    }
    cam->shakeDecay = decay;
    cam->shakeStep = step;
    cam->shakePhase = 90.0f;
}

// Debug free-fly camera (held buttons pick orbit / dolly / pan, the stick
// drives it) with an on-screen read-out of the pose and the field of view.
void fn_1_21BA8(CAMERA *cam)
{
    Vec dir;
    Vec base;
    Mtx rotY;
    Mtx rotX;
    f32 len;
    f32 stickX;
    f32 stickY;
    f32 fx;
    f32 fy;
    f32 fov;

    base = cam->tgt;
    PSVECSubtract(&cam->pos, &cam->tgt, &dir);
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
        PSMTXRotRad(rotY, 'Y', 0.05f * fx);
        PSMTXRotRad(rotX, 'X', 0.05f * fy);
        PSMTXMultVec(rotY, &dir, &dir);
        PSMTXMultVec(rotX, &dir, &dir);
        PSVECAdd(&dir, &base, &base);
        cam->posGoal = base;
        cam->pos = cam->posGoal;
        cam->tgt = cam->tgtGoal;
        if ((HuPadBtn[0] & 0x800) != 0) {
            fov = 0.2f + lbl_1_data_AE0;
            if (fov > 90.0f) {
                fov = 90.0f;
            } else if (fov < 0.0f) {
                fov = 0.0f;
            }
            lbl_1_data_AE0 = fov;
            Hu3DCameraPerspectiveSet(cam->camId, lbl_1_data_AE0, 20.0f, 50000.0f, 1.2f);
        } else if ((HuPadBtn[0] & 0x400) != 0) {
            fov = lbl_1_data_AE0 - 0.2f;
            if (fov > 90.0f) {
                fov = 90.0f;
            } else if (fov < 0.0f) {
                fov = 0.0f;
            }
            lbl_1_data_AE0 = fov;
            Hu3DCameraPerspectiveSet(cam->camId, lbl_1_data_AE0, 20.0f, 50000.0f, 1.2f);
        }
    } else if ((HuPadBtn[0] & 0x4000) == 0 && (HuPadBtn[0] & 0x2000) != 0) {
        PSVECScale(&dir, &dir, 1.0f + (0.02f * -((f32)HuPadSubStkY[0] / 59.0f)));
        PSVECAdd(&dir, &base, &base);
        cam->posGoal = base;
        cam->pos = cam->posGoal;
        cam->tgt = cam->tgtGoal;
    } else if ((HuPadBtn[0] & 0x4000) != 0 && (HuPadBtn[0] & 0x2000) != 0) {
        fx = ((f32)HuPadSubStkX[0] / 59.0f) * 50.0f;
        fy = ((f32)HuPadSubStkY[0] / 59.0f) * 50.0f;
        cam->posGoal.z += fx;
        cam->posGoal.y += fy;
        cam->tgtGoal.z += fx;
        cam->tgtGoal.y += fy;
    }
    fontcolor = 1;
    print8(0x1BA, 0x152, 1.5f, lbl_1_data_AE4, cam->posGoal.x);
    print8(0x1BA, 0x15E, 1.5f, lbl_1_data_AEA, cam->posGoal.y);
    print8(0x1BA, 0x16A, 1.5f, lbl_1_data_AF0, cam->posGoal.z);
    print8(0x1BA, 0x176, 1.5f, lbl_1_data_AF6, cam->tgtGoal.x);
    print8(0x1BA, 0x182, 1.5f, lbl_1_data_AFC, cam->tgtGoal.y);
    print8(0x1BA, 0x18E, 1.5f, lbl_1_data_B02, cam->tgtGoal.z);
    print8(0x1BA, 0x19A, 1.5f, lbl_1_data_B08, lbl_1_data_AE0);
    fontcolor = 7;
    print8(0x1B8, 0x150, 1.5f, lbl_1_data_AE4, cam->posGoal.x);
    print8(0x1B8, 0x15C, 1.5f, lbl_1_data_AEA, cam->posGoal.y);
    print8(0x1B8, 0x168, 1.5f, lbl_1_data_AF0, cam->posGoal.z);
    print8(0x1B8, 0x174, 1.5f, lbl_1_data_AF6, cam->tgtGoal.x);
    print8(0x1B8, 0x180, 1.5f, lbl_1_data_AFC, cam->tgtGoal.y);
    print8(0x1B8, 0x18C, 1.5f, lbl_1_data_B02, cam->tgtGoal.z);
    print8(0x1B8, 0x198, 1.5f, lbl_1_data_B08, lbl_1_data_AE0);
}
