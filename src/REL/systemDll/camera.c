#include "REL/systemDll/sysmd.h"

typedef struct SysCameraRect_s {
    float x;
    float y;
    float w;
    float h;
} SYSCAMERA_RECT;

typedef struct SysCameraParam_s {
    HuVecF hvr;
    HuVecF center;
} SYSCAMERA_PARAM;

void MdCameraCreate(SYSCAMERA_WORK *work)
{
    int type[3] = { HU3D_CAM0, HU3D_CAM1, HU3D_CAM2 };
    SYSCAMERA_RECT rect[3] = {
        { 0, 0, 640, 480 },
        { 0, 0, 640, 480 },
        { 0, 0, 640, 480 },
    };
    SYSCAMERA_PARAM param[3] = {
        { { 90, 0, 1920 }, { 0, 0, 0 } },
        { { 90, 0, 3000 }, { 0, 0, 0 } },
        { { 90, 0, 1920 }, { 0, 0, 0 } },
    };
    float aspect[3];
    SYSCAMERA *camera;
    s32 i;

    work->selCamera = 0;
    work->createF = 1;
    for (i = 0; i < 3; i++) {
        camera = &work->camera[i];
        camera->bit = type[i];
        Hu3DCameraCreate(camera->bit);
        camera->pos.x = 0;
        camera->pos.y = 0;
        camera->pos.z = 0;
        camera->target.x = 0;
        camera->target.y = 0;
        camera->target.z = 0;
        camera->hvr.x = param[i].hvr.x;
        camera->hvr.y = param[i].hvr.y;
        camera->hvr.z = param[i].hvr.z;
        camera->unk4C = 0;
        camera->center.x = param[i].center.x;
        camera->center.y = param[i].center.y;
        camera->center.z = param[i].center.z;
        if (i == 0) {
            camera->fovy = 30;
        } else if (i == 1) {
            camera->fovy = 10;
        } else if (i == 2) {
            camera->fovy = 30;
        }
        aspect[i] = ((576 * rect[i].w) / 640) / ((480 * rect[i].h) / 480);
        Hu3DCameraPerspectiveSet(camera->bit, camera->fovy, 20, 20000, aspect[i]);
        Hu3DCameraViewportSet(camera->bit, rect[i].x, rect[i].y, rect[i].w, rect[i].h, 0, 1);
        Hu3DCameraScissorSet(camera->bit, rect[i].x, rect[i].y, rect[i].w, rect[i].h);
    }
    MdCameraUpdate(work);
}

void MdCameraDebug(SYSCAMERA_WORK *work)
{
    SYSCAMERA *camera;

    if (HuPadBtnDown[0] & PAD_TRIGGER_R) {
        work->selCamera = ++work->selCamera % 3;
    }
    camera = &work->camera[work->selCamera];
    if (HuPadTrigL[0] <= 75) {
        if (HuPadSubStkX[0] <= -29) {
            camera->hvr.x += 1;
        } else if (HuPadSubStkX[0] >= 29) {
            camera->hvr.x -= 1;
        }
        if (HuPadSubStkY[0] <= -29) {
            camera->hvr.y -= 1;
        } else if (HuPadSubStkY[0] >= 29) {
            camera->hvr.y += 1;
        }
    } else if (HuPadBtn[0] & PAD_TRIGGER_L) {
        if (HuPadStkY[0] <= -36) {
            camera->center.y -= 50;
        } else if (HuPadStkY[0] >= 36) {
            camera->center.y += 50;
        }
        if (HuPadSubStkY[0] <= -29) {
            camera->hvr.z += 50;
        } else if (HuPadSubStkY[0] >= 29) {
            camera->hvr.z -= 50;
        }
    } else {
        if (HuPadStkY[0] <= -36) {
            camera->center.y -= 10;
        } else if (HuPadStkY[0] >= 36) {
            camera->center.y += 10;
        }
        if (HuPadSubStkY[0] <= -29) {
            camera->hvr.z += 10;
        } else if (HuPadSubStkY[0] >= 29) {
            camera->hvr.z -= 10;
        }
    }
}

void MdCameraUpdate(SYSCAMERA_WORK *work)
{
    s32 i;
    static HuVecF up = { 0, 1, 0 };
    static HuVecF forward = { 0, 0, 1 };

    for (i = 0; i < 3; i++) {
        SYSCAMERA *camera = &work->camera[i];
        camera->target.x = camera->center.x;
        camera->target.y = camera->center.y;
        camera->target.z = camera->center.z;
        camera->pos.x = camera->target.x + (camera->hvr.z * HuCos(camera->hvr.y) * HuCos(camera->hvr.x));
        camera->pos.y = camera->target.y + (camera->hvr.z * HuSin(camera->hvr.y));
        camera->pos.z = camera->target.z + (camera->hvr.z * HuCos(camera->hvr.y) * HuSin(camera->hvr.x));
        VECSubtract(&camera->target, &camera->pos, &camera->dir);
        VECNormalize(&camera->dir, &camera->dir);
        VECCrossProduct(&up, &camera->dir, &camera->cross);
        VECNormalize(&camera->cross, &camera->cross);
        VECCrossProduct(&camera->dir, &camera->cross, &camera->up);
        VECNormalize(&camera->up, &camera->up);
        Hu3DCameraPosSet(camera->bit,
            camera->pos.x, camera->pos.y, camera->pos.z,
            camera->up.x, camera->up.y, camera->up.z,
            camera->target.x, camera->target.y, camera->target.z);
    }
}

void MdCameraUpdateSingle(SYSCAMERA *camera)
{
    static HuVecF up = { 0, 1, 0 };
    static HuVecF forward = { 0, 0, 1 };

    camera->target.x = camera->center.x;
    camera->target.y = camera->center.y;
    camera->target.z = camera->center.z;
    camera->pos.x = camera->target.x + (camera->hvr.z * HuCos(camera->hvr.y) * HuCos(camera->hvr.x));
    camera->pos.y = camera->target.y + (camera->hvr.z * HuSin(camera->hvr.y));
    camera->pos.z = camera->target.z + (camera->hvr.z * HuCos(camera->hvr.y) * HuSin(camera->hvr.x));
    VECSubtract(&camera->target, &camera->pos, &camera->dir);
    VECNormalize(&camera->dir, &camera->dir);
    VECCrossProduct(&up, &camera->dir, &camera->cross);
    VECNormalize(&camera->cross, &camera->cross);
    VECCrossProduct(&camera->dir, &camera->cross, &camera->up);
    VECNormalize(&camera->up, &camera->up);
}

void MdCameraPrint(SYSCAMERA_WORK *work)
{
    SYSCAMERA *camera = &work->camera[work->selCamera];

    MdDebugFontPrint("camera(HU3D_CAM%d))", work->selCamera);
    MdDebugFontPrint("   hvr(%.1f / %.1f / %.1f / %.1f)", camera->hvr.x, camera->hvr.y, camera->hvr.z);
    MdDebugFontPrint("   pos(%.1f / %.1f / %.1f)", camera->pos.x, camera->pos.y, camera->pos.z);
    MdDebugFontPrint("target(%.1f / %.1f / %.1f)", camera->target.x, camera->target.y, camera->target.z);
    MdDebugFontPrint("  fovy(%.1f)", camera->fovy);
}

void MdCameraPosSet(SYSCAMERA *camera, float h, float v, float r, float cx, float cy, float cz)
{
    camera->hvr.x = h;
    camera->hvr.y = v;
    camera->hvr.z = r;
    camera->center.x = cx;
    camera->center.y = cy;
    camera->center.z = cz;
}

void MdLightCreate(SYSLIGHT *light)
{
    Hu3DLighInit();
    light->createF = 1;
    light->hvr.x = 45;
    light->hvr.y = -45;
    light->hvr.z = 1000;
    light->center.x = 0;
    light->center.y = 0;
    light->center.z = 0;
    light->rgb.r = 255;
    light->rgb.g = 224;
    light->rgb.b = 144;
    light->rgb.a = 255;
    light->pos.x = 0;
    light->pos.y = 20000;
    light->pos.z = -1000;
    light->target.x = 0;
    light->target.y = 0;
    light->target.z = 0;
    light->lightPos = light->pos;
    VECSubtract(&light->target, &light->pos, &light->lightDir);
    light->gLightId = Hu3DGLightCreateV(&light->lightPos, &light->lightDir, &light->rgb);
    MdLightUpdateData(light);
    Hu3DGLightStaticSet(light->gLightId, TRUE);
    Hu3DGLightInfinitytSet(light->gLightId);
}

void MdLightUpdateData(SYSLIGHT *light)
{
    light->target.x = light->center.x;
    light->target.y = light->center.y;
    light->target.z = light->center.z;
    light->pos.x = light->target.x + (light->hvr.z * HuCos(light->hvr.y) * HuCos(light->hvr.x));
    light->pos.y = light->target.y + (light->hvr.z * HuSin(light->hvr.y));
    light->pos.z = light->target.z + (light->hvr.z * HuCos(light->hvr.y) * HuSin(light->hvr.x));
    light->lightPos = light->pos;
    VECSubtract(&light->target, &light->pos, &light->lightDir);
    VECNormalize(&light->lightDir, &light->lightDir);
    Hu3DGLightPosSetV(light->gLightId, &light->lightPos, &light->lightDir);
    Hu3DGLightColorSet(light->gLightId, light->rgb.r, light->rgb.g, light->rgb.b, light->rgb.a);
}

void MdLightDebug(SYSLIGHT *light)
{
    if (HuPadTrigL[0] <= 75) {
        if (HuPadSubStkX[0] <= -29) {
            light->hvr.x += 1;
        } else if (HuPadSubStkX[0] >= 29) {
            light->hvr.x -= 1;
        }
        if (HuPadSubStkY[0] <= -29) {
            light->hvr.y -= 1;
        } else if (HuPadSubStkY[0] >= 29) {
            light->hvr.y += 1;
        }
    } else {
        if (HuPadSubStkX[0] <= -29) {
            light->rgb.r--;
        } else if (HuPadSubStkX[0] >= 29) {
            light->rgb.r++;
        }
        if (HuPadSubStkY[0] <= -29) {
            light->rgb.g--;
        } else if (HuPadSubStkY[0] >= 29) {
            light->rgb.g++;
        }
        if (HuPadStkX[0] <= -36) {
            light->rgb.b--;
        } else if (HuPadStkX[0] >= 36) {
            light->rgb.b++;
        }
    }
}

void MdLightPrint(SYSLIGHT *light)
{
    MdDebugFontPrint("lhvr(%.1f / %.1f / %.1f)", light->hvr.x, light->hvr.y, light->hvr.z);
    MdDebugFontPrint("lrgb(%d / %d / %d)", light->rgb.r, light->rgb.g, light->rgb.b);
}
