#include "REL/mdoptionDll/mdoption.h"
#include "stdarg.h"
#include "string.h"

void MdObjDebugFontMain(OMOBJ *obj);
void MdObjCameraMain(OMOBJ *obj);
void MdObjLightMain(OMOBJ *obj);
void MdDebugFontCreate(MDDEBUGFONT_WORK *debugFont);
void MdDebugFontUpdate(MDDEBUGFONT_WORK *debugFont);
void MdDebugFontPrint(MDDEBUGFONT_WORK *debugFont, const char *str, ...);
void MdCameraCreate(MDCAMERA_WORK *cameraWork);
void MdCameraDebug(MDCAMERA_WORK *cameraWork);
void MdCameraUpdate(MDCAMERA_WORK *cameraWork);
void MdCameraPrint(MDCAMERA_WORK *cameraWork);
void MdLightCreate(MDLIGHT_WORK *lightWork);
void MdLightUpdate(MDLIGHT_WORK *lightWork);
void MdLightUpdateData(MDLIGHT *light);
void MdLightDebug(MDLIGHT_WORK *lightWork);
void MdLightPrint(MDLIGHT_WORK *lightWork);
void MdShadowCreate(MDSHADOW_WORK *shadowWork);
void MdShadowUpdate(MDSHADOW_WORK *shadowWork);
void MdShadowDebug(MDSHADOW_WORK *shadowWork);
void MdShadowPrint(MDSHADOW_WORK *shadowWork);
void MdObjShadowMain(OMOBJ *obj);
void MdObjMessMain(OMOBJ *obj);
void MdMessCreate(MDMESS_WORK *mess);
void MdMathMtxRotGet(Mtx mtx, HuVecF *rot);
void MdMathMtxRotCalc(HuVecF *dir, HuVecF *up, float angle, Mtx out);
void MdMessHelpWinCreate(MDMESS_WORK *mess, int winNo, u32 mesNo, float posX, float posY, s16 w, s16 h, float bgTPLvl, s16 drawNo);
void MdMessWinHomeClear(MDMESS_WORK *mess, int winNo);
void MdMessWinAttrSet(MDMESS_WORK *mess, int winNo, u32 attr);
void MdMessWinSpeedSet(MDMESS_WORK *mess, int winNo, s16 speed);
void MdMessWinScaleSet(MDMESS_WORK *mess, int winNo, float scaleX, float scaleY);
void MdMessWinPosSet(MDMESS_WORK *mess, int winNo, float posX, float posY);
void MdMessDlgWinCreateInsert(MDMESS_WORK *mess, u32 mesNo);
void MdMessDlgWinKill(MDMESS_WORK *mess);

static float stdCurveSample[MDMATH_CURVE_STD_MAX][MDMATH_CURVE_SAMPLE_MAX] = {
    { 0.0f, 0.1f, 0.4f, 0.2f, 1.0f },
    { 0.0f, 0.5f, 1.0f },
};

static int stdCurveSampleNum[MDMATH_CURVE_STD_MAX];
static float stdCurveTime[MDMATH_CURVE_STD_MAX][MDMATH_CURVE_SAMPLE_MAX];
static float stdCurveSlope[MDMATH_CURVE_STD_MAX][MDMATH_CURVE_SAMPLE_MAX];

void MdObjDebugFontCreate(OMOBJ *obj)
{
    MDDEBUGFONT_WORK *debugFont = obj->data;
    MdDebugFontCreate(debugFont);
    obj->objFunc = MdObjDebugFontMain;
}

void MdObjDebugFontMain(OMOBJ *obj)
{
    MDDEBUGFONT_WORK *debugFont = obj->data;
    static char *ctrlStr[MD_DEBUGFONT_CTRL_MAX] = {
        "nop",
        "camera",
        "light",
        "model"
    };
    MdDebugFontUpdate(debugFont);
    
    if(HuPadBtnDown[0] & PAD_BUTTON_LEFT) {
        if(--debugFont->ctrl == -1) {
            debugFont->ctrl = MD_DEBUGFONT_CTRL_MAX-1;
        }
    }
    if(HuPadBtnDown[0] & PAD_BUTTON_RIGHT) {
        debugFont->ctrl = ++debugFont->ctrl%MD_DEBUGFONT_CTRL_MAX;
    }
    MdDebugFontPrint(debugFont, "<- %s ->", ctrlStr[debugFont->ctrl]);
    MdDebugFontPrint(debugFont, "X : FONT COLOR");
    MdDebugFontPrint(debugFont, "Y : FONT ONOFF");
}

void MdObjPadMain(OMOBJ *obj)
{
    MDPAD_WORK *pad = obj->data;
    int i;
    for(i=0; i<4; i++) {
        pad->DStkDown[i] = HuPadDStk[i] & ~pad->DStk[i];
        pad->DStk[i] = HuPadDStk[i];
    }
}

void MdObjCameraCreate(OMOBJ *obj)
{
    MDCAMERA_WORK *camera = obj->data;
    MdCameraCreate(camera);
    obj->objFunc = MdObjCameraMain;
}

void MdObjCameraMain(OMOBJ *obj)
{
    MDCAMERA_WORK *camera = obj->data;
    MdCameraUpdate(camera);
}

void MdObjLightCreate(OMOBJ *obj)
{
    MDLIGHT_WORK *light = obj->data;
    MdLightCreate(light);
    obj->objFunc = MdObjLightMain;
}

void MdObjLightMain(OMOBJ *obj)
{
    MDLIGHT_WORK *light = obj->data;
    MdLightUpdate(light);
}

void MdObjShadowCreate(OMOBJ *obj)
{
    MDSHADOW_WORK *shadow = obj->data;
    MdShadowCreate(shadow);
    obj->objFunc = MdObjShadowMain;
}

void MdObjShadowMain(OMOBJ *obj)
{
    MDSHADOW_WORK *shadow = obj->data;
    MdShadowUpdate(shadow);
}

void MdObjMessCreate(OMOBJ *obj)
{
    MDMESS_WORK *mess = obj->data;
    MdMessCreate(mess);
    obj->objFunc = MdObjMessMain;
}

void MdObjMessMain(OMOBJ *obj)
{
    MDMESS_WORK *mess = obj->data;
}

void MdDebugFontCreate(MDDEBUGFONT_WORK *debugFont)
{
    debugFont->createF = TRUE;
    debugFont->ctrl = MD_DEBUGFONT_CTRL_CAMERA;
    debugFont->printF = FALSE;
    debugFont->color = FONT_COLOR_YELLOW;
    debugFont->posX = 16;
    debugFont->posY = 40;
    debugFont->ofsY = 216;
    debugFont->rowOfsY = 0;
    debugFont->rowH = 15;
    debugFont->size = 1.5f;
    debugFont->maxLen = 0;
}

void MdDebugFontUpdate(MDDEBUGFONT_WORK *debugFont)
{
    debugFont->maxLen = 0;
    debugFont->rowOfsY = debugFont->ofsY;
}

void MdDebugFontPrint(MDDEBUGFONT_WORK *debugFont, const char *str, ...)
{
    size_t len = 0;
    char outBuf[160];
    va_list arg;
    if(debugFont->printF) {
        
        
        va_start(arg, str);
        vsprintf(outBuf, str, arg);
        len = strlen(outBuf);
        if(debugFont->maxLen < len) {
            debugFont->maxLen = len;
        }
        fontcolor = debugFont->color;
        print8(debugFont->posX, debugFont->posY+debugFont->rowOfsY, debugFont->size, "%s", outBuf);
        va_end(arg);
        debugFont->rowOfsY += debugFont->rowH;
    }
}

void MdCameraCreate(MDCAMERA_WORK *cameraWork)
{
    float scale = sysP->modelScale;
    int bit[2] = { HU3D_CAM0, HU3D_CAM1 };
    float aspect[2];
    MDCAMERA_RECT rect[2] = {
        { 0, 0, 640, 480 },
        { 0, 0, 640, 480 },
    };
    MDCAMERA_PARAM param[2] = {
        {
            { 90, 5, 4500 },
            { 0, -50, 0 },
        },
        {
            { 90, 0, 2500 },
            { 0, 400, 0 },
        },
    };
    MDCAMERA *camera;
    int i;

    param[0].hvr.z = 3375;
    cameraWork->selCamera = 0;
    cameraWork->createF = TRUE;
    for(i=0; i<2; i++) {
        camera = &cameraWork->camera[i];
        camera->bit = bit[i];
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
        camera->unk50 = 0;
        camera->center.x = param[i].center.x;
        camera->center.y = param[i].center.y;
        camera->center.z = param[i].center.z;
        camera->fovy = 30;
        aspect[i] = ((rect[i].w*HU_DISP_WIDTH)/640)/((rect[i].h*480)/480);
        Hu3DCameraPerspectiveSet(camera->bit, camera->fovy, 20, 20000, aspect[i]);
        Hu3DCameraViewportSet(camera->bit, rect[i].x, rect[i].y, rect[i].w, rect[i].h, 0, 1);
        Hu3DCameraScissorSet(camera->bit, rect[i].x, rect[i].y, rect[i].w, rect[i].h);
    }
    MdCameraUpdate(cameraWork);
}

void MdCameraDebug(MDCAMERA_WORK *cameraWork)
{
    MDCAMERA *camera;
    if(HuPadBtnDown[0] & PAD_TRIGGER_R) {
        cameraWork->selCamera = ++cameraWork->selCamera%2;
    }
    camera = &cameraWork->camera[cameraWork->selCamera];
    if(HuPadTrigL[0] <= 75) {
        if(HuPadSubStkX[0] <= -29) {
            camera->hvr.x += 1;
        } else if(HuPadSubStkX[0] >= 29) {
            camera->hvr.x -= 1;
        }
        if(HuPadSubStkY[0] <= -29) {
            camera->hvr.y -= 1;
        } else if(HuPadSubStkY[0] >= 29) {
            camera->hvr.y += 1;
        }
    } else if(HuPadBtn[0] & PAD_TRIGGER_L) {
        if(HuPadStkY[0] <= -36) {
            camera->center.y -= 50;
        } else if(HuPadStkY[0] >= 36) {
            camera->center.y += 50;
        }
        if(HuPadSubStkY[0] <= -29) {
            camera->hvr.z += 50;
        } else if(HuPadSubStkY[0] >= 29) {
            camera->hvr.z -= 50;
        }
    } else {
        if(HuPadStkY[0] <= -36) {
            camera->center.y -= 10;
        } else if(HuPadStkY[0] >= 36) {
            camera->center.y += 10;
        }
        if(HuPadSubStkY[0] <= -29) {
            camera->hvr.z += 10;
        } else if(HuPadSubStkY[0] >= 29) {
            camera->hvr.z -= 10;
        }
    }
}

void MdCameraUpdate(MDCAMERA_WORK *cameraWork)
{
    int i;
    static HuVecF up = { 0, 1, 0 };
    static HuVecF forward = { 0, 0, 1 };
    
    for(i=0; i<2; i++) {
        MDCAMERA *camera = &cameraWork->camera[i];
        camera->target.x = camera->center.x;
        camera->target.y = camera->center.y;
        camera->target.z = camera->center.z;
        camera->pos.x = camera->target.x+(camera->hvr.z*HuCos(camera->hvr.y)*HuCos(camera->hvr.x));
        camera->pos.y = camera->target.y+(camera->hvr.z*HuSin(camera->hvr.y));
        camera->pos.z = camera->target.z+(camera->hvr.z*HuCos(camera->hvr.y)*HuSin(camera->hvr.x));
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

void MdCameraPrint(MDCAMERA_WORK *cameraWork)
{
    MDCAMERA *camera = &cameraWork->camera[cameraWork->selCamera];
    MdDebugFontPrint(cameraWork->debugFont, "camera(HU3D_CAM%d))", cameraWork->selCamera);
    MdDebugFontPrint(cameraWork->debugFont, "   hvr(%.1f / %.1f / %.1f / %.1f)", camera->hvr.x, camera->hvr.y, camera->hvr.z);
    MdDebugFontPrint(cameraWork->debugFont, "   pos(%.1f / %.1f / %.1f)", camera->pos.x, camera->pos.y, camera->pos.z);
    MdDebugFontPrint(cameraWork->debugFont, "target(%.1f / %.1f / %.1f)", camera->target.x, camera->target.y, camera->target.z);
    MdDebugFontPrint(cameraWork->debugFont, "  fovy(%.1f)", camera->fovy);
}

void MdCameraPosSet(MDCAMERA *camera, float h, float v, float r, float cx, float cy, float cz)
{
    camera->hvr.x = h;
    camera->hvr.y = v;
    camera->hvr.z = r;
    camera->center.x = cx;
    camera->center.y = cy;
    camera->center.z = cz;
}

void MdLightCreate(MDLIGHT_WORK *lightWork)
{
    MDLIGHT *light;
    Hu3DLighInit();
    lightWork->createF = TRUE;
    lightWork->lightSel = 0;
    light = &lightWork->light[0];
    light->type = MDLIGHT_TYPE_G_DIRECT;
    light->hvr.x = 90;
    light->hvr.y = 20;
    light->hvr.z = 1000;
    light->center.x = 0;
    light->center.y = 0;
    light->center.z = 0;
    light->rgb.r = 204;
    light->rgb.g = 179;
    light->rgb.b = 115;
    light->rgb.a = 255;
    light->pos.x = 0;
    light->pos.y = 0;
    light->pos.z = 0;
    light->target.x = 0;
    light->target.y = 0;
    light->target.z = 0;
    light->lightDir.x = 0;
    light->lightDir.y = 0;
    light->lightDir.z = 1;
    light->lightPos = light->pos;
    VECSubtract(&light->target, &light->pos, &light->lightDir);
    light->gLightId = Hu3DGLightCreateV(&light->lightPos, &light->lightDir, &light->rgb);
    MdLightUpdateData(light);
    Hu3DGLightStaticSet(light->gLightId, TRUE);
    Hu3DGLightInfinitytSet(light->gLightId);
}

void MdLightUpdate(MDLIGHT_WORK *lightWork)
{
    int i;
    for(i=0; i<MDLIGHT_MAX; i++) {
        MDLIGHT *light = &lightWork->light[i];
        MdLightUpdateData(light);
    }
}

void MdLightUpdateData(MDLIGHT *light)
{
    if(light->type == MDLIGHT_TYPE_NOP) {
        return;
    }
    if(light->type == MDLIGHT_TYPE_G_DIRECT || light->type == MDLIGHT_TYPE_L_DIRECT) {
        light->target.x = light->center.x;
        light->target.y = light->center.y;
        light->target.z = light->center.z;
        light->pos.x = light->target.x+(light->hvr.z*HuCos(light->hvr.y)*HuCos(light->hvr.x));
        light->pos.y = light->target.y+(light->hvr.z*HuSin(light->hvr.y));
        light->pos.z = light->target.z+(light->hvr.z*HuCos(light->hvr.y)*HuSin(light->hvr.x));
        light->lightPos = light->pos;
        VECSubtract(&light->target, &light->pos, &light->lightDir);
        VECNormalize(&light->lightDir, &light->lightDir);
        if(light->type == MDLIGHT_TYPE_G_DIRECT) {
            Hu3DGLightPosSetV(light->gLightId, &light->lightPos, &light->lightDir);
            Hu3DGLightColorSet(light->gLightId, light->rgb.r, light->rgb.g, light->rgb.b, light->rgb.a);
        } else if(light->type == MDLIGHT_TYPE_L_DIRECT) {
            Hu3DLLightPosSetV(light->lLightId, light->gLightId, &light->lightPos, &light->lightDir);
            Hu3DLLightColorSet(light->lLightId, light->gLightId, light->rgb.r, light->rgb.g, light->rgb.b, light->rgb.a);
        }
    } else if(light->type == MDLIGHT_TYPE_G_POINT || light->type == MDLIGHT_TYPE_L_POINT) {
        light->target.x = light->center.x;
        light->target.y = light->center.y;
        light->target.z = light->center.z;
        light->pos.x = light->target.x+(light->hvr.z*HuCos(light->hvr.y)*HuCos(light->hvr.x));
        light->pos.y = light->target.y+(light->hvr.z*HuSin(light->hvr.y));
        light->pos.z = light->target.z+(light->hvr.z*HuCos(light->hvr.y)*HuSin(light->hvr.x));
        light->lightPos = light->target;
        VECSubtract(&light->pos, &light->target, &light->lightDir);
        VECNormalize(&light->lightDir, &light->lightDir);
        if(light->type == MDLIGHT_TYPE_G_POINT) {
            Hu3DGLightPosSetV(light->gLightId, &light->lightPos, &light->lightDir);
            Hu3DGLightColorSet(light->gLightId, light->rgb.r, light->rgb.g, light->rgb.b, light->rgb.a);
        } else if(light->type == MDLIGHT_TYPE_L_POINT) {
            Hu3DLLightPosSetV(light->lLightId, light->gLightId, &light->lightPos, &light->lightDir);
            Hu3DLLightColorSet(light->lLightId, light->gLightId, light->rgb.r, light->rgb.g, light->rgb.b, light->rgb.a);
            Hu3DLLightPointSet(light->lLightId, light->gLightId, light->hvr.z, 0.1f, GX_DA_GENTLE);
        }
    } else if(light->type == MDLIGHT_TYPE_G_SPOT || light->type == MDLIGHT_TYPE_L_SPOT) {
        light->lightPos = light->pos;
        VECSubtract(&light->target, &light->pos, &light->lightDir);
        VECNormalize(&light->lightDir, &light->lightDir);
        if(light->type == MDLIGHT_TYPE_G_SPOT) {
            Hu3DGLightPosSetV(light->gLightId, &light->lightPos, &light->lightDir);
            Hu3DGLightColorSet(light->gLightId, light->rgb.r, light->rgb.g, light->rgb.b, light->rgb.a);
        } else if(light->type == MDLIGHT_TYPE_L_SPOT) {
            Hu3DLLightPosSetV(light->lLightId, light->gLightId, &light->lightPos, &light->lightDir);
            Hu3DLLightColorSet(light->lLightId, light->gLightId, light->rgb.r, light->rgb.g, light->rgb.b, light->rgb.a);
        }
    }
}

void MdLightDebug(MDLIGHT_WORK *lightWork)
{
    MDLIGHT *light;
    if(HuPadBtnDown[0] & PAD_TRIGGER_R) {
        lightWork->lightSel = ++lightWork->lightSel%MDLIGHT_MAX;
    }
    light = &lightWork->light[lightWork->lightSel];
    if(light->type == MDLIGHT_TYPE_G_DIRECT || light->type == MDLIGHT_TYPE_L_DIRECT) {
        if(HuPadTrigL[0] <= 75) {
            if(HuPadSubStkX[0] <= -29) {
                light->hvr.x += 1;
            } else if(HuPadSubStkX[0] >= 29) {
                light->hvr.x -= 1;
            }
            if(HuPadSubStkY[0] <= -29) {
                light->hvr.y -= 1;
            } else if(HuPadSubStkY[0] >= 29) {
                light->hvr.y += 1;
            }
        } else {
            if(HuPadSubStkY[0] <= -29) {
                light->hvr.z -= 5;
            } else if(HuPadSubStkY[0] >= 29) {
                light->hvr.z += 5;
            }
        }
    } else if(light->type == MDLIGHT_TYPE_G_POINT || light->type == MDLIGHT_TYPE_L_POINT) {
        if(HuPadTrigL[0] <= 75) {
            if(HuPadSubStkX[0] <= -29) {
                light->center.x += 10;
            } else if(HuPadSubStkX[0] >= 29) {
                light->center.x -= 10;
            }
            if(HuPadSubStkY[0] <= -29) {
                light->center.z -= 10;
            } else if(HuPadSubStkY[0] >= 29) {
                light->center.z += 10;
            }
        } else {
            if(HuPadSubStkY[0] <= -29) {
                light->center.y -= 5;
            } else if(HuPadSubStkY[0] >= 29) {
                light->center.y += 5;
            }
            if(HuPadSubStkX[0] <= -29) {
                light->hvr.z -= 5;
            } else if(HuPadSubStkX[0] >= 29) {
                light->hvr.z += 5;
            }
        }
    } else if(light->type == MDLIGHT_TYPE_G_SPOT || light->type == MDLIGHT_TYPE_L_SPOT) {
        if(HuPadTrigL[0] <= 75) {
            if(HuPadSubStkX[0] <= -29) {
                light->pos.x -= 5;
            } else if(HuPadSubStkX[0] >= 29) {
                light->pos.x += 5;
            }
            if(HuPadSubStkY[0] <= -29) {
                light->pos.z -= 5;
            } else if(HuPadSubStkY[0] >= 29) {
                light->pos.z += 5;
            }
            if(HuPadStkY[0] <= -36) {
                light->pos.y -= 5;
            } else if(HuPadStkY[0] >= 36) {
                light->pos.y += 5;
            }
        } else {
            if(HuPadSubStkX[0] <= -29) {
                light->target.x -= 5;
            } else if(HuPadSubStkX[0] >= 29) {
                light->target.x += 5;
            }
            if(HuPadSubStkY[0] <= -29) {
                light->target.z -= 5;
            } else if(HuPadSubStkY[0] >= 29) {
                light->target.z += 5;
            }
            if(HuPadStkY[0] <= -36) {
                light->target.y -= 5;
            } else if(HuPadStkY[0] >= 36) {
                light->target.y += 5;
            }
        }
    }
}

void MdLightPrint(MDLIGHT_WORK *lightWork)
{
    MDLIGHT *light = &lightWork->light[lightWork->lightSel];
    static char *lightType[MDLIGHT_TYPE_MAX] = {
        "NOP",
        "G_DIRECT",
        "L_DIRECT",
        "G_POINT",
        "L_POINT",
        "G_SPOT",
        "L_SPOT"
    };
    MdDebugFontPrint(lightWork->debugFont, "light(%d)->%s)", lightWork->lightSel, lightType[light->type]);
    MdDebugFontPrint(lightWork->debugFont, "   hvr(%.1f / %.1f / %.1f)", light->hvr.x, light->hvr.y, light->hvr.z);
    MdDebugFontPrint(lightWork->debugFont, "   pos(%.1f / %.1f / %.1f)", light->pos.x, light->pos.y, light->pos.z);
    MdDebugFontPrint(lightWork->debugFont, "target(%.1f / %.1f / %.1f)", light->target.x, light->target.y, light->target.z);
    MdDebugFontPrint(lightWork->debugFont, "rgb(%d / %d / %d)", light->rgb.r, light->rgb.g, light->rgb.b);
}

void MdShadowCreate(MDSHADOW_WORK *shadowWork)
{
    MDCAMERA_WORK *camera = sysP->camera;
    MDSHADOW *shadow;
    int i;
    shadowWork->createF = TRUE;
    shadowWork->shadowSel = 0;
    Hu3DShadowCreate(30, 20, 10000);
    for(i=0; i<1; i++) {
        shadow = &shadowWork->shadow[i];
        shadow->bit = camera->camera[0].bit;
        shadow->tpLvl = 0.3f;
        shadow->hvr.x = 65;
        shadow->hvr.y = 65;
        shadow->hvr.z = 6000;
        shadow->center.x = 0;
        shadow->center.y = 0;
        shadow->center.z = 0;
    }
    MdShadowUpdate(shadowWork);
}

void MdShadowUpdate(MDSHADOW_WORK *shadowWork)
{
    int i;
    static HuVecF up = { 0, 1, 0 };
    static HuVecF forward = { 0, 0, 1 };

    for(i=0; i<1; i++) {
        MDSHADOW *shadow = &shadowWork->shadow[i];
        shadow->target.x = shadow->center.x;
        shadow->target.y = shadow->center.y;
        shadow->target.z = shadow->center.z;
        shadow->pos.x = shadow->target.x+(shadow->hvr.z*HuCos(shadow->hvr.y)*HuCos(shadow->hvr.x));
        shadow->pos.y = shadow->target.y+(shadow->hvr.z*HuSin(shadow->hvr.y));
        shadow->pos.z = shadow->target.z+(shadow->hvr.z*HuCos(shadow->hvr.y)*HuSin(shadow->hvr.x));
        VECSubtract(&shadow->target, &shadow->pos, &shadow->dir);
        VECNormalize(&shadow->dir, &shadow->dir);
        VECCrossProduct(&up, &shadow->dir, &shadow->cross);
        VECNormalize(&shadow->cross, &shadow->cross);
        VECCrossProduct(&shadow->dir, &shadow->cross, &shadow->up);
        VECNormalize(&shadow->up, &shadow->up);
        Hu3DShadowTPLvlSet(shadow->tpLvl);
        Hu3DShadowPosSet(&shadow->pos, &shadow->up, &shadow->target);
    }
}

void MdShadowDebug(MDSHADOW_WORK *shadowWork)
{
    MDSHADOW *shadow;
    if(HuPadBtnDown[0] & PAD_TRIGGER_R) {
        shadowWork->shadowSel = ++shadowWork->shadowSel%1;
    }
    shadow = &shadowWork->shadow[shadowWork->shadowSel];
    if(HuPadTrigL[0] <= 75) {
        if(HuPadSubStkX[0] <= -29) {
            shadow->hvr.x += 1;
        } else if(HuPadSubStkX[0] >= 29) {
            shadow->hvr.x -= 1;
        }
        if(HuPadSubStkY[0] <= -29) {
            shadow->hvr.y -= 1;
        } else if(HuPadSubStkY[0] >= 29) {
            shadow->hvr.y += 1;
        }
    } else if(HuPadBtn[0] & PAD_TRIGGER_L) {
        if(HuPadSubStkY[0] <= -29) {
            shadow->hvr.z += 50;
        } else if(HuPadSubStkY[0] >= 29) {
            shadow->hvr.z -= 50;
        }
        if(HuPadStkY[0] <= -36) {
            shadow->center.y -= 50;
        } else if(HuPadStkY[0] >= 36) {
            shadow->center.y += 50;
        }
    } else {
        if(HuPadSubStkY[0] <= -29) {
            shadow->hvr.z += 10;
        } else if(HuPadSubStkY[0] >= 29) {
            shadow->hvr.z -= 10;
        }
        if(HuPadStkY[0] <= -36) {
            shadow->center.y -= 10;
        } else if(HuPadStkY[0] >= 36) {
            shadow->center.y += 10;
        }
    }
}

void MdShadowPrint(MDSHADOW_WORK *shadowWork)
{
    MDSHADOW *shadow = &shadowWork->shadow[shadowWork->shadowSel];
    MdDebugFontPrint(shadowWork->debugFont, "shadow(%d))", shadowWork->shadowSel);
    MdDebugFontPrint(shadowWork->debugFont, "   hvr(%.1f / %.1f / %.1f)", shadow->hvr.x, shadow->hvr.y, shadow->hvr.z);
    MdDebugFontPrint(shadowWork->debugFont, "   pos(%.1f / %.1f / %.1f)", shadow->pos.x, shadow->pos.y, shadow->pos.z);
    MdDebugFontPrint(shadowWork->debugFont, "target(%.1f / %.1f / %.1f)", shadow->target.x, shadow->target.y, shadow->target.z);
}

Mtx *MdMathMatrixDir(HuVecF *start, HuVecF *end)
{
    static Mtx out;
    static HuVecF up = { 0, 1, 0 };
    HuVecF cross;
    HuVecF cross2;
    HuVecF dir;
    HuVecF a;
    HuVecF b;
    
    b = *end;
    a = *start;
    VECSubtract(&b, &a, &dir);
    VECNormalize(&dir, &dir);
    VECCrossProduct(&up, &dir, &cross);
    VECNormalize(&cross, &cross);
    VECCrossProduct(&dir, &cross, &cross2);
    VECNormalize(&cross2, &cross2);
    out[0][1] = cross2.x;
    out[1][1] = cross2.y;
    out[2][1] = cross2.z;
    out[0][0] = cross.x;
    out[1][0] = cross.y;
    out[2][0] = cross.z;
    out[0][2] = dir.x;
    out[1][2] = dir.y;
    out[2][2] = dir.z;
    return &out;
}

void MdMathMatrixRot(Mtx *matrix, HuVecF *rot)
{
    Mtx rotX, rotY, rotZ;
    MTXRotDeg(rotX, 'X', rot->x);
    MTXRotDeg(rotY, 'Y', rot->y);
    MTXRotDeg(rotZ, 'Z', rot->z);
    MTXConcat(rotZ, rotX, *matrix);
    MTXConcat(*matrix, rotY, *matrix);
}

void MdMathMatrixRotRPY(Mtx *matrix, HuVecF *rot)
{
    Mtx rotX, rotY, rotZ;
    MTXRotRad(rotX, 'X', 0.017453292f*rot->x);
    MTXRotRad(rotY, 'Y', 0.017453292f*rot->y);
    MTXRotRad(rotZ, 'Z', 0.017453292f*rot->z);
    MTXConcat(rotZ, rotY, *matrix);
    MTXConcat(*matrix, rotX, *matrix);
}

void MdMathMtxTrans(Mtx *src, Mtx *dst, HuVecF *trans)
{
    MTXTransApply(*src, *dst, trans->x, trans->y, trans->z);
}

void MdMathClamp(float *val, float min, float max)
{
    if(*val <= min) {
        *val = min;
    }
    if(*val >= max) {
        *val = max;
    }
}

void MdMathAngleClamp(float *angle)
{
    if(*angle > 180.0f) {
        *angle = -180.0f+(*angle-180.0f);
    } else if(*angle < -180.0f) {
        *angle = 180.0f+(*angle+180.0f);
    }
}

void MdMathOscillate(int type, float *in, float *out, float t)
{
    *in += 1/t;
    if(*in <= 0) {
        *in = 0;
    }
    if(*in >= 1) {
        *in = 1;
    }
    switch(type) {
        case MDMATH_OSC_0:
            *out = *in;
            break;
        
        case MDMATH_OSC_90:
            *out = HuSin((*in)*90);
            break;
        
        case MDMATH_OSC_90_REV:
            *out = 1-HuSin(90-((*in)*90));
            break;
        
        case MDMATH_OSC_180:
            *out = HuSin((*in)*180);
            break;
        
        case MDMATH_OSC_360:
            *out = HuSin((*in)*360);
            break;
        
        case MDMATH_OSC_540:
            *out = HuSin((*in)*540);
            break;
        
        case MDMATH_OSC_720:
            *out = HuSin((*in)*720);
            break;

        case 7:
            *out = HuCos((*in)*180);
            break;

        case 8:
            *out = HuCos((*in)*360);
            break;

        case 9:
            *out = HuCos((*in)*540);
            break;

        case 10:
            *out = HuCos((*in)*720);
            break;
    }
}

static float StdCurveGet(int type, float t);
static void CurveInit(int sampleNum, float *sampleT, float *sample, float *sampleSlope);
static float CurveGet(int sampleNum, float t, float *sampleT, float *sample, float *sampleSlope);
static void StdCurveInit(int idx);

void MdMathStdCurveGet(int type, float *in, float *out, float t)
{
    *in += 1/t;
    if(*in <= 0) {
        *in = 0;
    }
    if(*in >= 1) {
        *in = 1;
    }
    *out = StdCurveGet(type, *in);
}

void MdMathLerp(float *out, float a, float b, float t)
{
    *out = a+(t*(b-a));
}

void MdMathLerpScale(HuVecF *out, float a, float b, float t)
{
    float result = a+(t*(b-a));
    out->x = result;
    out->y = result;
    out->z = result;
}

void MdMathLerpColor(u8 *out, float a, float b, float t)
{
    *out = a+(t*(b-a));
}

void MdMathLerpOfs(float *out, float a, float b, float t, float ofs)
{
    *out = a+((b-a)*(t+ofs));
}

void MdMathLerpScaleOfs(HuVecF *out, float a, float b, float t, float ofs)
{
    float result = a+((b-a)*(t+ofs));
    out->x = result;
    out->y = result;
    out->z = result;
}

void MdMathCurveInit(int sampleNum, float *sampleT, float *sample, float *sampleSlope)
{
    int i;
    sampleT[0] = 0;
    (sampleT-1)[sampleNum] = 1;
    for(i=1; i<sampleNum-1; i++) {
        sampleT[i] = (1.0f/(sampleNum-1))*i;
    }
    
    CurveInit(sampleNum, sampleT, sample, sampleSlope);
}

void MdMathCurveGet(float *out, int sampleNum, float *sampleT, float *sample, float *sampleSlope, float t)
{
    *out = CurveGet(sampleNum, t, sampleT, sample, sampleSlope);
}

void MdMathStdCurveInit(void)
{
    int i;
    int j;
    for(i=0; i<MDMATH_CURVE_STD_MAX; i++) {
        for(j=0; j<MDMATH_CURVE_SAMPLE_MAX; j++) {
            if(1.0f == stdCurveSample[i][j]) {
                stdCurveSampleNum[i] = j+1;
                break;
            }
        }
    }
    for(i=0; i<MDMATH_CURVE_STD_MAX; i++) {
        stdCurveTime[i][0] = 0;
        stdCurveTime[i][stdCurveSampleNum[i]-1] = 1;
        for(j=1; j<stdCurveSampleNum[i]-1; j++) {
            stdCurveTime[i][j] = (1.0f/(stdCurveSampleNum[i]-1))*j;
        }
    }
    for(i=0; i<MDMATH_CURVE_STD_MAX; i++) {
        StdCurveInit(i);
    }
}

static void CurveInit(int sampleNum, float *sampleT, float *sample, float *sampleSlope)
{
    int i;
    float step;
    static float dt[MDMATH_CURVE_SAMPLE_MAX+1];
    static float dt2[MDMATH_CURVE_SAMPLE_MAX+1];
    static float slope[MDMATH_CURVE_SAMPLE_MAX+1];
    sampleNum--;
    for(i=0; i<sampleNum; i++) {
        dt[i] = sampleT[i+1]-sampleT[i];
        slope[i] = (sample[i+1]-sample[i])/dt[i];
    }
    slope[sampleNum] = slope[0];
    for(i=1; i<sampleNum; i++) {
        dt2[i] = 2*(sampleT[i+1]-sampleT[i-1]);
    }
    dt2[sampleNum] = 2*(dt[0]+dt[sampleNum-1]);
    for(i=1; i<=sampleNum; i++) {
        sampleSlope[i] = slope[i]-slope[i-1];
    }
    slope[1] = dt[0];
    slope[sampleNum-1] = dt[sampleNum-1];
    slope[sampleNum] = dt2[sampleNum];
    for(i=2; i<sampleNum-1; i++) {
        slope[i] = 0;
    }
    for(i=1; i<sampleNum; i++) {
        step = dt[i]/dt2[i];
        sampleSlope[i+1] = sampleSlope[i+1]-(step*sampleSlope[i]);
        dt2[i+1] = dt2[i+1]-(step*dt[i]);
        slope[i+1] = slope[i+1]-(step*slope[i]);
        
    }
    slope[0] = slope[sampleNum];
    sampleSlope[0] = sampleSlope[sampleNum];
    for(i=sampleNum-2; i>=0; i--) {
        step = dt[i]/dt2[i+1];
        sampleSlope[i] = sampleSlope[i]-(step*sampleSlope[i+1]);
        slope[i] = slope[i]-(step*slope[i+1]);
    }
    step = sampleSlope[0]/slope[0];
    sampleSlope[0] = step;
    sampleSlope[sampleNum] = step;
    for(i=1; i<sampleNum; i++) {
        sampleSlope[i] = (sampleSlope[i]-(step*slope[i]))/dt2[i];
    }
}

static float CurveGet(int sampleNum, float t, float *sampleT, float *sample, float *sampleSlope)
{
    float dt;
    float sampleDt;
    float len;
    
    int i;
    int num;
    int mid;
    
    sampleNum--;
    len = sampleT[sampleNum]-sampleT[0];
    while(t > sampleT[sampleNum]) {
        t -= len;
    }
    while(t < sampleT[0]) {
        t += len;
    }
    for(i=0, num=sampleNum; i<num;) {
        mid = (i+num)/2;
        if(sampleT[mid] < t) {
            i = mid+1;
        } else {
            num = mid;
        }
    }
    if(i > 0) {
        i--;
    }
    sampleDt = sampleT[i+1]-sampleT[i];
    dt = t-sampleT[i];
    return sample[i]+(dt*((dt*((3*sampleSlope[i])+((dt*(sampleSlope[i+1]-sampleSlope[i]))/sampleDt)))+(((sample[i+1]-sample[i])/sampleDt)-(sampleDt*((2*sampleSlope[i])+sampleSlope[i+1])))));
}

static void StdCurveInit(int idx)
{
    int i;
    float *point;
    int pointNum;
    float *time;
    float *samplePoint;
    float step;
    
    static float dt[MDMATH_CURVE_SAMPLE_MAX+1];
    static float dt2[MDMATH_CURVE_SAMPLE_MAX+1];
    static float slope[MDMATH_CURVE_SAMPLE_MAX+1];
    
    pointNum = stdCurveSampleNum[idx]-1;
    time = &stdCurveTime[idx][0];
    samplePoint = &stdCurveSample[idx][0];
    point = &stdCurveSlope[idx][0];
    for(i=0; i<pointNum; i++) {
        dt[i] = time[i+1]-time[i];
        slope[i] = (samplePoint[i+1]-samplePoint[i])/dt[i];
    }
    slope[pointNum] = slope[0];
    for(i=1; i<pointNum; i++) {
        dt2[i] = 2*(time[i+1]-time[i-1]);
    }
    dt2[pointNum] = 2*(dt[0]+dt[pointNum-1]);
    for(i=1; i<=pointNum; i++) {
        point[i] = slope[i]-slope[i-1];
    }
    slope[1] = dt[0];
    slope[pointNum-1] = dt[pointNum-1];
    slope[pointNum] = dt2[pointNum];
    for(i=2; i<pointNum-1; i++) {
        slope[i] = 0;
    }
    for(i=1; i<pointNum; i++) {
        step = dt[i]/dt2[i];
        point[i+1] = point[i+1]-(step*point[i]);
        dt2[i+1] = dt2[i+1]-(step*dt[i]);
        slope[i+1] = slope[i+1]-(step*slope[i]);
        
    }
    slope[0] = slope[pointNum];
    point[0] = point[pointNum];
    for(i=pointNum-2; i>=0; i--) {
        step = dt[i]/dt2[i+1];
        point[i] = point[i]-(step*point[i+1]);
        slope[i] = slope[i]-(step*slope[i+1]);
    }
    step = point[0]/slope[0];
    point[0] = step;
    point[pointNum] = step;
    for(i=1; i<pointNum; i++) {
        point[i] = (point[i]-(step*slope[i]))/dt2[i];
    }
}

static float StdCurveGet(int type, float t)
{
    float dt;
    float sampleDt;
    float len;
    
    float *sampleT;
    float *sampleSlope;
    float *sample;
    int i;
    int num;
    int mid;
    
    int sampleNum;
    sampleNum = stdCurveSampleNum[type]-1;
    sampleT = &stdCurveTime[type][0];
    sample = &stdCurveSample[type][0];
    sampleSlope = &stdCurveSlope[type][0];
    len = sampleT[sampleNum]-sampleT[0];
    while(t > sampleT[sampleNum]) {
        t -= len;
    }
    while(t < sampleT[0]) {
        t += len;
    }
    for(i=0, num=sampleNum; i<num;) {
        mid = (i+num)/2;
        if(sampleT[mid] < t) {
            i = mid+1;
        } else {
            num = mid;
        }
    }
    if(i > 0) {
        i--;
    }
    sampleDt = sampleT[i+1]-sampleT[i];
    dt = t-sampleT[i];
    return sample[i]+(dt*((dt*((3*sampleSlope[i])+((dt*(sampleSlope[i+1]-sampleSlope[i]))/sampleDt)))+(((sample[i+1]-sample[i])/sampleDt)-(sampleDt*((2*sampleSlope[i])+sampleSlope[i+1])))));
}

void MdMessCreate(MDMESS_WORK *mess)
{
    MDMESS_WIN *win;
    MDMESS_MISC *misc;
    int i;

    mess->mode = 0;
    mess->dlgWinId = HUWIN_NONE;
    mess->createF = TRUE;
    for(win=&mess->win[0], i=0; i<MDMESS_WIN_NUM; i++, win++) {
        win->winId = HUWIN_NONE;
        win->unk4 = 0;
        win->unk8 = 0;
        win->unkC = 0;
    }
    misc = &mess->misc;
    misc->state = 0;
    misc->unk8 = 0;
    misc->unkC = 0;
    misc->unk10 = 0;
    misc->unk14 = 0;
    {
        HuVecF pos[9] = {
            { 0, 49, 0 },
            { 0, 84, 0 },
            { 0, 112, 0 },
            { 0, 48, 0 },
            { 0, 48, 0 },
            { 0, 48, 0 },
            { 0, 48, 0 },
            { 0, 48, 0 },
            { 0, 48, 0 },
        };
        for(i=2; i<MDMESS_WIN_NUM; i++) {
            int idx = i-2;
#ifdef TARGET_PC
            if(idx >= 9) idx = 8; // GC reads past pos[9] (stack garbage); clamp to the last 48 entry
#endif
            if(i < 4) {
                if(i == 2) {
                    MdMessHelpWinCreate(mess, i, 0, 138, pos[idx].y, 300, 28, 0, 0);
                } else {
                    MdMessHelpWinCreate(mess, i, 0, 0, pos[idx].y, 566, 28, 0, 0);
                }
            } else {
                MdMessHelpWinCreate(mess, i, 0x20000B, 138, pos[idx].y, 300, 28, 1, 0);
            }
            MdMessWinHomeClear(mess, i);
            MdMessWinAttrSet(mess, i, 8);
            MdMessWinSpeedSet(mess, i, 0);
            MdMessWinScaleSet(mess, i, 0, 0);
        }
        MdMessWinPosSet(mess, 2, 133, pos[0].y);
    }
    MdMessHelpWinCreate(mess, 1, 0x20004, 0, 344, 576, 28, 0, 0);
    MdMessWinHomeClear(mess, 1);
    MdMessWinAttrSet(mess, 1, 0x800);
}

void MdMessUpdate(MDMESS_WORK *mess)
{
    s16 stat;
    switch(mess->mode) {
        case 1:
            MdMessDlgWinCreateInsert(mess, mess->dlgMesNo);
            mess->mode = 3;
            break;

        case 2:
            MdMessDlgWinCreateInsert(mess, mess->dlgMesNo);
            mess->mode = 4;
            break;

        case 3:
            stat = HuWinStatGet(mess->dlgWinId);
            if(stat == 0) {
                mess->mode = 5;
            }
            break;

        case 4:
            stat = HuWinStatGet(mess->dlgWinId);
            if(stat == 0) {
                mess->mode = 6;
            }
            break;

        case 6:
            mess->dlgChoice = HuWinChoiceGet(mess->dlgWinId, -1);
            mess->mode = 5;
            break;

        case 7:
            MdMessDlgWinKill(mess);
            mess->mode = 0;
            break;
    }
}

void MdMessKill(void)
{
    HuWinAllKill();
}

void MdMessDlgMesSet(MDMESS_WORK *mess, int cmd, u32 mesNo)
{
    s16 stat;
    MDMESS_MISC *misc = &mess->misc;
    switch(cmd) {
        case 0:
            MdMessDlgWinKill(mess);
            mess->mode = 0;
            break;

        case 1:
        case 3:
            if(mess->dlgWinId == HUWIN_NONE) {
                if(cmd == 1) {
                    mess->dlgType = 0;
                } else {
                    mess->dlgType = 1;
                }
                mess->dlgMesNo = mesNo;
                mess->mode = 1;
            }
            break;

        case 2:
        case 4:
            if(mess->dlgWinId == HUWIN_NONE) {
                if(cmd == 2) {
                    mess->dlgType = 0;
                } else {
                    mess->dlgType = 1;
                }
                mess->dlgMesNo = mesNo;
                mess->mode = 2;
            }
            break;

        case 5:
            switch(mess->mode) {
                case 1:
                    MdMessDlgWinCreateInsert(mess, mess->dlgMesNo);
                    mess->mode = 3;
                    break;

                case 2:
                    MdMessDlgWinCreateInsert(mess, mess->dlgMesNo);
                    mess->mode = 4;
                    break;

                case 3:
                    stat = HuWinStatGet(mess->dlgWinId);
            if(stat == 0) {
                        mess->mode = 5;
                    }
                    break;

                case 4:
                    stat = HuWinStatGet(mess->dlgWinId);
            if(stat == 0) {
                        mess->mode = 6;
                    }
                    break;

                case 6:
                    mess->dlgChoice = HuWinChoiceGet(mess->dlgWinId, -1);
                    mess->mode = 5;
                    break;

                case 7:
                    MdMessDlgWinKill(mess);
                    mess->mode = 0;
                    break;
            }
            break;

        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            if(cmd == 6) {
                misc->state = 1;
            } else if(cmd == 7) {
                misc->state = 2;
            } else if(cmd == 8) {
                misc->state = 3;
            } else if(cmd == 9) {
                misc->state = 4;
            } else if(cmd == 10) {
                misc->state = 5;
            } else if(cmd == 11) {
                misc->state = 6;
            }
            break;
    }
}

void MdMessDlgInsertMesReset(MDMESS_WORK *mess)
{
    MDMESS_INSERT *insert = &mess->insert;
    insert->num = 0;
}

void MdMessDlgInsertMesAdd(MDMESS_WORK *mess, u32 mesNo, s16 insertNo)
{
    MDMESS_INSERT *insert = &mess->insert;
    insert->mesNo[insert->num] = mesNo;
    insert->insertNo[insert->num] = insertNo;
    if(insert->num < MDMESS_INSERT_MAX) {
        insert->num++;
    }
}

void MdMessDlgWinCreateInsert(MDMESS_WORK *mess, u32 mesNo)
{
    HuVec2f size;
    MDMESS_INSERT *insert = &mess->insert;
    int i;

    if(mess->dlgWinId == HUWIN_NONE) {
        if(mess->dlgType == 1) {
            for(i=0; i<insert->num; i++) {
                HuWinInsertMesSizeGet(insert->mesNo[i], insert->insertNo[i]);
            }
        }
        HuWinMesMaxSizeGet(1, &size, mesNo);
        if(size.x >= 472) {
            size.x = 472;
        }
        mess->dlgWinId = HuWinCreate(-10000, 374, size.x, 64, 0);
        if(mess->dlgType == 1) {
            for(i=0; i<insert->num; i++) {
                HuWinInsertMesSet(mess->dlgWinId, insert->mesNo[i], insert->insertNo[i]);
            }
        }
        HuWinBGTPLvlSet(mess->dlgWinId, 0);
        HuWinDrawNoSet(mess->dlgWinId, 0);
        HuWinPriSet(mess->dlgWinId, 0);
        HuWinPriSet(mess->dlgWinId, 0);
        HuWinMesSet(mess->dlgWinId, mesNo);
        HuWinDispOn(mess->dlgWinId);
    }
}

void MdMessDlgWinKill(MDMESS_WORK *mess)
{
    if(mess->dlgWinId != HUWIN_NONE) {
        HuWinHomeClear(mess->dlgWinId);
        HuWinDispOff(mess->dlgWinId);
        HuWinKill(mess->dlgWinId);
        mess->dlgWinId = HUWIN_NONE;
    }
}

void MdMessHelpWinCreate(MDMESS_WORK *mess, int winNo, u32 mesNo, float posX, float posY, s16 w, s16 h, float bgTPLvl, s16 drawNo)
{
    HuVec2f size;
    MDMESS_WIN *win = &mess->win[winNo];

    if(win->winId == HUWIN_NONE) {
        HuWinMesMaxSizeGet(1, &size, mesNo);
        if(size.x >= 472) {
            size.x = 472;
        }
        if(w == 0) {
            w = size.x;
        }
        if(h == 0) {
            h = size.y;
        }
        win->winId = HuWinCreate(0, 0, w, h, 0);
        HuWinPosSet(win->winId, posX, posY);
        HuWinBGTPLvlSet(win->winId, bgTPLvl);
        HuWinDrawNoSet(win->winId, drawNo);
        HuWinPriSet(win->winId, 0);
        HuWinMesSet(win->winId, mesNo);
        HuWinDispOn(win->winId);
    }
}

void MdMessWinOpen(MDMESS_WORK *mess, int winNo, float posX, float posY, s16 w, s16 h, float bgTPLvl, s16 drawNo)
{
    MDMESS_WIN *win = &mess->win[winNo];

    if(win->winId == HUWIN_NONE) {
        win->winId = HuWinExCreateFrame(posX, posY, w, h, -1, 0);
        HuWinPosSet(win->winId, posX, posY);
        HuWinMesPalSet(win->winId, 7, 255, 255, 255);
        HuWinBGTPLvlSet(win->winId, bgTPLvl);
        HuWinDrawNoSet(win->winId, drawNo);
        HuWinPriSet(win->winId, 0);
        HuWinExOpen(win->winId);
    }
}

void MdMessWinClose(MDMESS_WORK *mess, int winNo)
{
    MDMESS_WIN *win = &mess->win[winNo];

    if(win->winId != HUWIN_NONE) {
        HuWinHomeClear(win->winId);
        HuWinDispOff(win->winId);
        HuWinKill(win->winId);
        win->winId = HUWIN_NONE;
    }
}

void MdMessWinKill(MDMESS_WORK *mess, int winNo)
{
    MDMESS_WIN *win = &mess->win[winNo];

    if(win->winId != HUWIN_NONE) {
        HuWinHomeClear(win->winId);
        HuWinExClose(win->winId);
        HuWinExKill(win->winId);
        win->winId = HUWIN_NONE;
    }
}

void MdMessWinPosSet(MDMESS_WORK *mess, int winNo, float posX, float posY)
{
    MDMESS_WIN *win = &mess->win[winNo];

    if(win->winId != HUWIN_NONE) {
        HuWinPosSet(win->winId, posX, posY);
    }
}

void MdMessWinSpeedSet(MDMESS_WORK *mess, int winNo, s16 speed)
{
    MDMESS_WIN *win = &mess->win[winNo];

    if(win->winId != HUWIN_NONE) {
        HuWinMesSpeedSet(win->winId, 0);
    }
}

void MdMessWinScaleSet(MDMESS_WORK *mess, int winNo, float scaleX, float scaleY)
{
    MDMESS_WIN *win = &mess->win[winNo];

    if(win->winId != HUWIN_NONE) {
        HuWinScaleSet(win->winId, scaleX, scaleY);
    }
}

void MdMessWinHomeClear(MDMESS_WORK *mess, int winNo)
{
    MDMESS_WIN *win = &mess->win[winNo];
    HUWIN *w;
    int i;

    if(win->winId != HUWIN_NONE) {
        w = &winData[win->winId];
        HuWinHomeClear(win->winId);
        for(i=0; i<HUWIN_INSERTMES_MAX; i++) {
            w->messDataInsert[i] = NULL;
        }
    }
}

void MdMessWinMesSet(MDMESS_WORK *mess, int winNo, u32 mesNo)
{
    MDMESS_WIN *win = &mess->win[winNo];

    if(win->winId != HUWIN_NONE) {
        HuWinMesSet(win->winId, mesNo);
    }
}

void MdMessWinInsertMesSet(MDMESS_WORK *mess, int winNo, u32 mesNo, u32 insertMesNo)
{
    MDMESS_WIN *win = &mess->win[winNo];

    if(win->winId != HUWIN_NONE) {
        HuWinInsertMesSet(win->winId, insertMesNo, 0);
        HuWinMesSet(win->winId, mesNo);
    }
}

void MdMessWinInsertMultiMesSet(MDMESS_WORK *mess, int winNo, u32 mesNo, u32 *insertMesNo)
{
    MDMESS_WIN *win = &mess->win[winNo];
    int i;

    if(win->winId != HUWIN_NONE) {
        for(i=0; i<8; i++) {
            if(insertMesNo[i] != -1) {
                HuWinInsertMesSet(win->winId, insertMesNo[i], i);
            }
        }
        HuWinMesSet(win->winId, mesNo);
    }
}

BOOL MdMessWinCheck(MDMESS_WORK *mess, int winNo)
{
    MDMESS_WIN *win = &mess->win[winNo];
    s16 stat = HuWinStatGet(win->winId);
    if(stat == 0) {
        return TRUE;
    }
    return FALSE;
}

s16 MdMessWinChoiceGet(MDMESS_WORK *mess, int winNo)
{
    MDMESS_WIN *win = &mess->win[winNo];
    s16 choice;
    choice = HuWinChoiceGet(win->winId, -1);
    return choice;
}

s16 MdMessWinChoiceNowGet(MDMESS_WORK *mess, int winNo)
{
    MDMESS_WIN *win = &mess->win[winNo];
    HUWIN *w = &winData[win->winId];
    return w->choice;
}

void MdMessWinChoiceSet(MDMESS_WORK *mess, int winNo, s16 choice)
{
    MDMESS_WIN *win = &mess->win[winNo];
    HUWIN *w = &winData[win->winId];
    w->choice = choice;
}

s16 MdMessWinStatGet(MDMESS_WORK *mess, int winNo)
{
    MDMESS_WIN *win = &mess->win[winNo];
    s16 stat;
    stat = HuWinStatGet(win->winId);
    return stat;
}

void MdMessWinAttrSet(MDMESS_WORK *mess, int winNo, u32 attr)
{
    MDMESS_WIN *win = &mess->win[winNo];
    HuWinAttrSet(win->winId, attr);
}

void MdMessWinPad1Set(MDMESS_WORK *mess, int winNo)
{
    MDMESS_WIN *win = &mess->win[winNo];

    if(win->winId != HUWIN_NONE) {
        HuWinDisablePlayerSet(win->winId, 0xE);
    }
}

void MdMessWinAttrReset(MDMESS_WORK *mess, int winNo, u32 attr)
{
    MDMESS_WIN *win = &mess->win[winNo];
    HuWinAttrReset(win->winId, attr);
}

void fn_1_F980(void)
{
}

void MdMathPolarCoord(HuVecF *out, HuVecF *center, float radX, float radZ, float scale, float angle)
{
    HuVecF pos = { 0, 0, 0 };

    pos.z = pos.z-(scale*(radX*HuCos(angle)));
    pos.x = pos.x-(scale*(radZ*HuSin(angle)));
    VECAdd(&pos, center, &pos);
    *out = pos;
}

void ObjSelectUpdate(void *work)
{
    MDOPT_MODEL_WORK *model = sysP->model;
    MDCAMERA_WORK *camera = sysP->camera;
    HuVecF rot;
    Mtx mtx;
    float z;
    float cz;
    float base;
    float scale;
    int i;
    int idx;

    for(i=0; i<=22; i++) {
        if(i != 4) {
            idx = i+3;
            VECSubtract(&Hu3DCamera[0].pos, model->posP[idx], &rot);
            MdMathMtxRotCalc(&rot, &Hu3DCamera[0].up, 0, mtx);
            MdMathMtxRotGet(mtx, &rot);
            *model->rotP[idx] = rot;
            if(i < 5) {
                Hu3DModelLayerSet(model->mdlId[idx], 1);
            }
            if(sysP->unk3D8 != 0) {
                if(i < 5) {
                    base = 1;
                } else {
                    base = 0.4f;
                }
                if(model->selState[idx] == 1) {
                    model->selT[idx] += 0.1f;
                    if(model->selT[idx] > 1) {
                        model->selT[idx] = 1;
                    }
                    model->selState[idx] = 0;
                } else if(model->selState[idx] == 0) {
                    model->selT[idx] -= 0.1f;
                    if(model->selT[idx] < 0) {
                        model->selT[idx] = 0;
                    }
                } else if(model->selState[idx] == 2) {
                    model->selT[idx] -= 0.025f;
                    if(model->selT[idx] < 0) {
                        model->selState[idx] = 0;
                        model->selT[idx] = 0;
                    }
                }
                scale = base+((0.5f*base)*(0.5*(1-HuCos(180.0f*model->selT[idx]))));
                model->scaleP[idx]->z = scale;
                model->scaleP[idx]->y = scale;
                model->scaleP[idx]->x = scale;
            }
        }
    }
    if(sysP->unk3DC != 0) {
        Hu3DModelLayerSet(model->mdlId[sysP->unk3DC+2], 3);
    }
    *model->posP[7] = *model->posP[4];
    *model->rotP[7] = *model->rotP[4];
    Hu3DModelLayerSet(model->mdlId[7], Hu3DData[model->mdlId[4]].layerNo);
    z = model->posP[3]->z;
    for(i=8; i<=12; i++) {
        cz = model->posP[i]->z;
        if(z >= cz) {
            Hu3DModelLayerSet(model->mdlId[i], 3);
        } else {
            Hu3DModelLayerSet(model->mdlId[i], 6);
        }
    }
    z = model->posP[4]->z;
    for(i=13; i<=14; i++) {
        cz = model->posP[i]->z;
        if(z >= cz) {
            Hu3DModelLayerSet(model->mdlId[i], 3);
        } else {
            Hu3DModelLayerSet(model->mdlId[i], 6);
        }
    }
    z = model->posP[5]->z;
    for(i=15; i<=19; i++) {
        cz = model->posP[i]->z;
        if(z >= cz) {
            Hu3DModelLayerSet(model->mdlId[i], 3);
        } else {
            Hu3DModelLayerSet(model->mdlId[i], 6);
        }
    }
    z = model->posP[6]->z;
    for(i=20; i<=25; i++) {
        cz = model->posP[i]->z;
        if(z >= cz) {
            Hu3DModelLayerSet(model->mdlId[i], 3);
        } else {
            Hu3DModelLayerSet(model->mdlId[i], 6);
        }
    }
}

void MdMathMtxRotGet(Mtx mtx, HuVecF *rot)
{
    float tmp;
    Hu3DMtxRotGet(mtx, rot);
    if(rot->y == -90) {
        tmp = -rot->z;
        rot->z = -rot->x;
        rot->x = tmp;
    }
}

void MdMathMtxRotCalc(HuVecF *dir, HuVecF *up, float angle, Mtx out)
{
    Mtx rotMtx;
    HuVecF upTbl[2] = {
        { 0, 1, 0 },
        { 0, 0, 1 },
    };
    HuVecF right;
    HuVecF cross;
    HuVecF dirN;
    HuVecF upN;
    float ay;
    float y;

    VECNormalize(dir, &dirN);
    if(up != 0) {
        VECNormalize(up, &upN);
    } else {
        if(dirN.y > 0) {
            y = dirN.y;
        } else {
            y = -dirN.y;
        }
        ay = y;
        if(ay < 0.98f) {
            upN = upTbl[0];
        } else {
            upN = upTbl[1];
        }
    }
    VECCrossProduct(&upN, &dirN, &right);
    VECNormalize(&right, &right);
    VECCrossProduct(&dirN, &right, &cross);
    VECNormalize(&cross, &cross);
    out[0][0] = right.x;
    out[1][0] = right.y;
    out[2][0] = right.z;
    out[0][1] = cross.x;
    out[1][1] = cross.y;
    out[2][1] = cross.z;
    out[0][2] = dirN.x;
    out[1][2] = dirN.y;
    out[2][2] = dirN.z;
    if(up == 0) {
        MTXRotRad(rotMtx, 'z', 0.017453292f*angle);
        MTXConcat(out, rotMtx, out);
    }
}

void MdMath3Dto2D(HuVecF *src, s16 cameraBit, HuVecF *dst)
{
    HuVecF v;
    Mtx mtx;
    HU3DCAMERA *cam;
    float fx;
    float fy;
    s16 i;

    for(i=0; i<16; i++) {
        if(cameraBit & (1 << i)) {
            break;
        }
    }
    cam = &Hu3DCamera[i];
    C_MTXLookAt(mtx, &cam->pos, &cam->up, &cam->target);
    MTXMultVec(mtx, src, &v);
    fx = 1.2000000476837158*(v.z*(HuSin(0.5f*cam->fov)/HuCos(0.5f*cam->fov)));
    fy = v.z*(HuSin(0.5f*cam->fov)/HuCos(0.5f*cam->fov));
    dst->x = 288.0f+(v.x*(288.0f/-fx));
    dst->y = 240.0f+(v.y*(240.0f/fy));
    dst->z = v.z;
}
