#include "REL/sd00dll_sys.h"
#include "game/frand.h"
#include <math.h>

/* Random point around `center` at a distance in [minR, maxR].
   mode 0 + axis: on the ring around axis; mode 0: on the XZ plane;
   otherwise on a random direction of the sphere. */
void SdRandomPosGet(Vec *out, s32 mode, Vec *center, Vec *axis, f32 maxR, f32 minR)
{
    Vec pos = { 0.0f, 0.0f, 0.0f };
    Vec axisN;
    Vec upVec = { 0.0f, 1.0f, 0.0f };
    Vec rightVec = { 1.0f, 0.0f, 0.0f };
    Vec *ref;
    Mtx rotA;
    Mtx rotB;
    f32 dot;

    if (mode == 0) {
        if (axis != NULL) {
            /* the original normalizes &axis (the parameter slot) here; both callers pass NULL */
            PSVECNormalize(axis, &axisN);
            dot = PSVECDotProduct(&axisN, &upVec);
            if (dot < 0.707f && dot > -0.707f) {
                ref = &upVec;
            } else {
                ref = &rightVec;
            }
            PSVECCrossProduct(&axisN, ref, &pos);
            PSVECNormalize(&pos, &pos);
            PSVECScale(&pos, &pos, minR + (f32)frandmod((u32)(maxR - minR)));
            PSMTXRotAxisRad(rotA, &axisN, 0.017453292f * (f32)frandmod(360));
            PSMTXMultVec(rotA, &pos, &pos);
        } else {
            pos.x = minR + (f32)frandmod((u32)(maxR - minR));
            PSMTXRotRad(rotA, 'y', 0.017453292f * (f32)frandmod(360));
            PSMTXMultVec(rotA, &pos, &pos);
        }
    } else {
        pos.x = minR + (f32)frandmod((u32)(maxR - minR));
        PSMTXRotRad(rotA, 'x', 0.017453292f * ((f32)frandmod(180) - 90.0f));
        PSMTXRotRad(rotB, 'y', 0.017453292f * (f32)frandmod(360));
        PSMTXConcat(rotA, rotB, rotA);
        PSMTXMultVec(rotA, &pos, &pos);
    }
    PSVECAdd(&pos, center, &pos);
    *out = pos;
}

/* Sets the lit/color/shadow colors of materials 2 and 3 to a blue glow. */
void SdModelGlowSet(HU3DMODELID mdlId)
{
    HSFMATERIAL *mat = Hu3DData[mdlId].hsf->material;
    s32 i;

    for (i = 2; i < 4; i++) {
        mat[i].litColor[0] = 0;
        mat[i].litColor[1] = 0;
        mat[i].litColor[2] = 0;
        mat[i].color[0] = 0x19;
        mat[i].color[1] = 0x19;
        mat[i].color[2] = 0x4C;
        mat[i].shadowColor[0] = 0xFF;
        mat[i].shadowColor[1] = 0xFF;
        mat[i].shadowColor[2] = 0xFF;
    }
}

void SdScreenPosGet(Vec *pos, s16 cameraBit, Vec *out)
{
    Mtx lookMtx;
    Vec view;
    HU3DCAMERA *cam;
    f32 scaleY;
    f32 scaleX;
    s16 i;

    for (i = 0; i < 16; i++) {
        if ((cameraBit & (1 << i)) != 0) {
            break;
        }
    }
    cam = &Hu3DCamera[i];
    C_MTXLookAt(lookMtx, &cam->pos, &cam->up, &cam->target);
    PSMTXMultVec(lookMtx, pos, &view);
    scaleX = (f32)(1.2000000476837158 * (view.z * (sin(3.141592653589793 * (cam->fov * 0.5f) / 180.0) / cos(3.141592653589793 * (cam->fov * 0.5f) / 180.0))));
    scaleY = (f32)(view.z * (sin(3.141592653589793 * (cam->fov * 0.5f) / 180.0) / cos(3.141592653589793 * (cam->fov * 0.5f) / 180.0)));
    out->x = 288.0f + view.x * (288.0f / -scaleX);
    out->y = 240.0f + view.y * (240.0f / scaleY);
    out->z = view.z;
}
