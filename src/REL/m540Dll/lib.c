/*
 * Object, camera and window helpers of m540. This file is included by
 * m540.c: its functions are defined before the game code, which lets MWCC
 * inline them there, while their out-of-line copies land after the game code
 * in .text.
 */

#define M540_CUROBJ ((M540_OBJ *)HuPrcCurrentGet()->property)
#define M540_CURCAMERA ((M540_CAMERA *)HuPrcCurrentGet()->property)

/* Sleeps tick frames, returns from the calling mode function on a mode change */
#define M540_OBJ_SLEEP(tick) \
do { \
    int i_; \
    int num_; \
    int mode_; \
    num_ = (tick); \
    for(i_=0; i_<num_; i_++) { \
        HuPrcVSleep(); \
        mode_ = M540_CUROBJ->mode; \
        if(M540_CUROBJ->prevMode != mode_) { \
            M540_CUROBJ->prevMode = M540_CUROBJ->mode; \
            return; \
        } \
        M540_CUROBJ->prevMode = M540_CUROBJ->mode; \
    } \
} while(0)

static void fn_1_130F4(void);
static void fn_1_1315C(M540_OBJ *obj);
static void fn_1_14050(void);
static void fn_1_14468(M540_CAMERA *camera);
static void fn_1_149A4(M540_CAMERA *camera);
static void fn_1_143A8(M540_CAMERA *camera);
static void fn_1_14CB0(M540_CAMERA *camera, float vpX, float vpY, float vpW, float vpH, float vpNearZ, float vpFarZ);
static void fn_1_14D10(M540_CAMERA *camera, float fov, float near, float far, float aspect);
static void fn_1_14D60(M540_CAMERA *camera, float scissorX, float scissorY, float scissorW, float scissorH);
static void fn_1_14DF4(M540_CAMERA *camera);
static void fn_1_15270(M540_CAMERA *camera);
static void fn_1_15630(void);

static M540_CAMERAFUNC lbl_1_data_0[] = {
    fn_1_14468,
    fn_1_149A4,
    fn_1_143A8
};

static M540_OBJ *fn_1_12F80(int prio, u32 stackSize, int funcNum, M540_OBJFUNC *funcTbl)
{
    M540_OBJ *obj;
    int i;
    obj = HuMemDirectMallocNum(HEAP_HEAP, M540_OBJ_SIZE, HU_MEMNUM_OVL);
    obj->mode = 0;
    obj->prevMode = 0;
    obj->modeNum = funcNum;
    obj->unkC = 0;
    obj->modeFunc = HuMemDirectMallocNum(HEAP_HEAP, funcNum*sizeof(M540_OBJFUNC), HU_MEMNUM_OVL);
    for(i=0; i<funcNum; i++) {
        obj->modeFunc[i] = fn_1_1315C;
    }
    obj->process = HuPrcChildCreate(fn_1_130F4, prio, stackSize, 0, HuPrcCurrentGet());
    obj->process->property = obj;
    if(funcTbl != NULL) {
        for(i=0; i<funcNum; i++) {
            if(funcTbl[i] != NULL) {
                obj->modeFunc[i] = funcTbl[i];
            }
        }
    }
    return obj;
}

static void fn_1_130B4(M540_OBJ *obj)
{
    HuMemDirectFree(obj->modeFunc);
    HuPrcKill(obj->process);
    HuMemDirectFree(obj);
}

static void fn_1_130F4(void)
{
    M540_OBJ *obj = M540_CUROBJ;
    while(obj->mode < obj->modeNum) {
        obj->modeFunc[obj->mode](obj);
        obj->prevMode = obj->mode;
    }
    HuPrcEnd();
}

static void fn_1_1315C(M540_OBJ *obj)
{
    while(1) {
        HuPrcSleep(0);
    }
}

static M540_OBJ *fn_1_13174(int prio, u32 stackSize, u32 dataNum, int motNum, u32 *motTbl, int funcNum, M540_OBJFUNC *funcTbl)
{
    M540_OBJ *obj;
    int i;
    obj = HuMemDirectMallocNum(HEAP_HEAP, M540_MODELOBJ_SIZE, HU_MEMNUM_OVL);
    obj->mode = 0;
    obj->prevMode = 0;
    obj->modeNum = funcNum;
    obj->dispF = TRUE;
    obj->pos.x = 0;
    obj->pos.y = 0;
    obj->pos.z = 0;
    obj->rot.x = 0;
    obj->rot.y = 0;
    obj->rot.z = 0;
    obj->scale.x = 1;
    obj->scale.y = 1;
    obj->scale.z = 1;
    obj->modelId = HU3D_MODELID_NONE;
    obj->unkC = 0;
    obj->motNum = motNum;
    obj->motId = HuMemDirectMallocNum(HEAP_HEAP, motNum*sizeof(HU3DMOTID), HU_MEMNUM_OVL);
    for(i=0; i<motNum; i++) {
        obj->motId[i] = HU3D_MOTID_NONE;
    }
    obj->modeFunc = HuMemDirectMallocNum(HEAP_HEAP, funcNum*sizeof(M540_OBJFUNC), HU_MEMNUM_OVL);
    for(i=0; i<funcNum; i++) {
        obj->modeFunc[i] = fn_1_1315C;
    }
    obj->process = HuPrcChildCreate(fn_1_130F4, prio, stackSize, 0, HuPrcCurrentGet());
    obj->process->property = obj;
    if(dataNum != HU_DATANUM_NONE) {
        obj->modelId = Hu3DModelCreateData(dataNum);
    }
    for(i=0; i<motNum; i++) {
        if(motTbl[i] != HU_DATANUM_NONE) {
            obj->motId[i] = Hu3DJointMotionData(obj->modelId, motTbl[i]);
        }
    }
    if(funcTbl != NULL) {
        for(i=0; i<funcNum; i++) {
            if(funcTbl[i] != NULL) {
                obj->modeFunc[i] = funcTbl[i];
            }
        }
    }
    return obj;
}

static void fn_1_133F8(M540_OBJ *obj)
{
    int i;
    for(i=0; i<obj->motNum; i++) {
        if(obj->motId[i] != HU3D_MOTID_NONE) {
            Hu3DMotionKill(obj->motId[i]);
        }
    }
    HuMemDirectFree(obj->motId);
    if(obj->modelId != HU3D_MODELID_NONE) {
        Hu3DModelKill(obj->modelId);
    }
    HuMemDirectFree(obj->modeFunc);
    HuPrcKill(obj->process);
    HuMemDirectFree(obj);
}

static void fn_1_13498(M540_OBJ *obj, float x, float y, float z)
{
    Hu3DModelPosSet(obj->modelId, obj->pos.x = x, obj->pos.y = y, obj->pos.z = z);
}

static void fn_1_134EC(M540_OBJ *obj, float *x, float *y, float *z)
{
    if(x != NULL) {
        *x = obj->pos.x;
    }
    if(y != NULL) {
        *y = obj->pos.y;
    }
    if(z != NULL) {
        *z = obj->pos.z;
    }
}

static void fn_1_1352C(M540_OBJ *obj, float x, float y, float z)
{
    Hu3DModelRotSet(obj->modelId, obj->rot.x = x, obj->rot.y = y, obj->rot.z = z);
}

static void fn_1_13580(M540_OBJ *obj, float *x, float *y, float *z)
{
    if(x != NULL) {
        *x = obj->rot.x;
    }
    if(y != NULL) {
        *y = obj->rot.y;
    }
    if(z != NULL) {
        *z = obj->rot.z;
    }
}

static void fn_1_135C0(M540_OBJ *obj, float x, float y, float z)
{
    Hu3DModelScaleSet(obj->modelId, obj->scale.x = x, obj->scale.y = y, obj->scale.z = z);
}

static void fn_1_13614(M540_OBJ *obj, float *x, float *y, float *z)
{
    if(x != NULL) {
        *x = obj->scale.x;
    }
    if(y != NULL) {
        *y = obj->scale.y;
    }
    if(z != NULL) {
        *z = obj->scale.z;
    }
}

static void fn_1_13654(M540_OBJ *obj, float tpLvl)
{
    Hu3DModelTPLvlSet(obj->modelId, obj->tpLvl = tpLvl);
}

static float fn_1_13690(M540_OBJ *obj)
{
    return obj->tpLvl;
}

static void fn_1_13698(M540_OBJ *obj)
{
    obj->dispF = TRUE;
    Hu3DModelAttrReset(obj->modelId, HU3D_ATTR_DISPOFF);
}

static void fn_1_136D4(M540_OBJ *obj)
{
    obj->dispF = FALSE;
    Hu3DModelAttrSet(obj->modelId, HU3D_ATTR_DISPOFF);
}

static BOOL fn_1_13710(M540_OBJ *obj)
{
    return obj->dispF;
}

static void fn_1_13718(M540_OBJ *obj, int time)
{
    Hu3DMotionTimeSet(obj->modelId, time);
}

static void fn_1_13774(M540_OBJ *obj, BOOL loopF)
{
    HU3DMODELID modelId = obj->modelId;
    Hu3DModelAttrReset(modelId, HU3D_MOTATTR_PAUSE);
    if(loopF) {
        Hu3DModelAttrSet(modelId, HU3D_MOTATTR_LOOP);
    } else {
        Hu3DModelAttrReset(modelId, HU3D_MOTATTR_LOOP);
    }
}

static void fn_1_137EC(M540_OBJ *obj)
{
    Hu3DModelAttrSet(obj->modelId, HU3D_MOTATTR_PAUSE);
}

static BOOL fn_1_13824(M540_OBJ *obj)
{
    return (!Hu3DMotionEndCheck(obj->modelId)) ? TRUE : FALSE;
}

static void fn_1_13874(M540_OBJ *obj, int motNo, float start, float end, u32 attr)
{
    obj->motNo = motNo;
    if(motNo == -1) {
        Hu3DMotionSet(obj->modelId, HU3D_MOTID_NONE);
    } else {
        Hu3DMotionShiftSet(obj->modelId, obj->motId[motNo], start, end, attr);
    }
}

static int fn_1_138F0(M540_OBJ *obj)
{
    return obj->motNo;
}

static void fn_1_138F8(M540_OBJ *obj, float speed)
{
    Hu3DMotionSpeedSet(obj->modelId, obj->motSpeed = speed);
}

static void fn_1_13934(M540_OBJ *obj)
{
    Hu3DModelShadowSet(obj->modelId);
}

static void fn_1_13964(M540_OBJ *obj)
{
    Hu3DModelShadowReset(obj->modelId);
}

static void fn_1_13994(M540_OBJ *obj)
{
    Hu3DModelShadowMapSet(obj->modelId);
}

static void fn_1_139C4(M540_OBJ *obj)
{
}

static void fn_1_139C8(M540_OBJ *obj, char *objName, M540_OBJ *hookObj)
{
    Hu3DModelHookSet(obj->modelId, objName, hookObj->modelId);
}

static void fn_1_13A10(M540_OBJ *obj)
{
    Hu3DModelHookReset(obj->modelId);
}

static M540_OBJ *fn_1_13A40(int prio, u32 stackSize, int playerNo, int motNum, u32 *motTbl, int funcNum, M540_OBJFUNC *funcTbl, s16 modelNo)
{
    M540_OBJ *obj;
    int i;
    obj = HuMemDirectMallocNum(HEAP_HEAP, M540_CHAROBJ_SIZE, HU_MEMNUM_OVL);
    obj->mode = 0;
    obj->prevMode = 0;
    obj->modeNum = funcNum;
    obj->dispF = TRUE;
    obj->pos.x = 0;
    obj->pos.y = 0;
    obj->pos.z = 0;
    obj->rot.x = 0;
    obj->rot.y = 0;
    obj->rot.z = 0;
    obj->scale.x = 1;
    obj->scale.y = 1;
    obj->scale.z = 1;
    obj->modelId = HU3D_MODELID_NONE;
    obj->comF = (GwPlayerConf[playerNo].type != GW_TYPE_MAN) ? TRUE : FALSE;
    obj->charNo = GwPlayerConf[playerNo].charNo;
    obj->padNo = GwPlayerConf[playerNo].padNo;
    obj->dif = GwPlayerConf[playerNo].dif;
    obj->playerNo = playerNo;
    obj->unkC = 0;
    obj->motNum = motNum;
    obj->motId = HuMemDirectMallocNum(HEAP_HEAP, motNum*sizeof(HU3DMOTID), HU_MEMNUM_OVL);
    for(i=0; i<motNum; i++) {
        obj->motId[i] = HU3D_MOTID_NONE;
    }
    obj->modeFunc = HuMemDirectMallocNum(HEAP_HEAP, funcNum*sizeof(M540_OBJFUNC), HU_MEMNUM_OVL);
    for(i=0; i<funcNum; i++) {
        obj->modeFunc[i] = fn_1_1315C;
    }
    obj->process = HuPrcChildCreate(fn_1_130F4, prio, stackSize, 0, HuPrcCurrentGet());
    obj->process->property = obj;
    obj->modelId = CharModelCreate(obj->charNo, modelNo);
    for(i=0; i<motNum; i++) {
        int motIdx = i+(obj->charNo*13);
        if(motTbl[motIdx] != HU_DATANUM_NONE) {
            obj->motId[i] = CharMotionCreate(obj->charNo, motTbl[i]);
        }
    }
    CharMotionDataClose(obj->charNo);
    for(i=0; i<funcNum; i++) {
        if(funcTbl[i] != NULL) {
            obj->modeFunc[i] = funcTbl[i];
        }
    }
    return obj;
}

static void fn_1_13D30(M540_OBJ *obj)
{
    int i;
    for(i=0; i<obj->motNum; i++) {
        if(obj->motId[i] != HU3D_MOTID_NONE) {
            Hu3DMotionKill(obj->motId[i]);
        }
    }
    HuMemDirectFree(obj->motId);
    if(obj->modelId != HU3D_MODELID_NONE) {
        Hu3DModelKill(obj->modelId);
    }
    HuMemDirectFree(obj->modeFunc);
    HuPrcKill(obj->process);
    HuMemDirectFree(obj);
}

static void fn_1_13DD0(M540_OBJ *obj, char *objName)
{
    if(obj->charNo = 0) {
        CharModelHookDustCreate(obj->charNo, objName);
    } else {
        Hu3DModelHookReset(obj->modelId);
    }
}

static M540_CAMERA *fn_1_13E28(s16 cameraBit)
{
    M540_CAMERA *camera;
    M540_CAMERAVIEW *view;
    int i;
    camera = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M540_CAMERA), HU_MEMNUM_OVL);
    camera->cameraBit = cameraBit;
    for(i=0; i<3; i++) {
        view = &camera->view[i];
        view->center.x = 0;
        view->center.y = 0;
        view->center.z = 0;
        view->rot.x = 0;
        view->rot.y = 0;
        view->rot.z = 0;
        view->zoom = 0;
    }
    camera->process = HuPrcChildCreate(fn_1_14050, 0x1000, 0x2000, 0, HuPrcCurrentGet());
    camera->process->property = camera;
    camera->debugF = FALSE;
    Hu3DCameraCreate(cameraBit);
    fn_1_14D10(camera, 30, 10, 8000, 1.2f);
    fn_1_14CB0(camera, 0, 0, 640, 480, 0, 1);
    fn_1_14D60(camera, 0, 0, 640, 480);
    camera->mode = 2;
    return camera;
}

static void fn_1_14018(M540_CAMERA *camera)
{
    HuPrcKill(camera->process);
    HuMemDirectFree(camera);
}

static void fn_1_14050(void)
{
    M540_CAMERA *camera = M540_CURCAMERA;
    while(camera->mode != 3) {
        lbl_1_data_0[camera->mode](camera);
    }
    HuPrcEnd();
}

static void fn_1_140B4(M540_CAMERA *camera)
{
    M540_CAMERAVIEW *view;
    int i;
    for(i=0; i<3; i++) {
        view = &camera->view[i];
        view->pos.x = view->center.x+(view->zoom*(HuSin(view->rot.y)*HuCos(view->rot.x)));
        view->pos.y = view->center.y+(view->zoom*(-HuSin(view->rot.x)));
        view->pos.z = view->center.z+(view->zoom*(HuCos(view->rot.y)*HuCos(view->rot.x)));
        view->up.x = HuSin(view->rot.y)*HuSin(view->rot.x);
        view->up.y = HuCos(view->rot.x);
        view->up.z = HuCos(view->rot.y)*HuSin(view->rot.x);
    }
}

static void fn_1_1432C(M540_CAMERA *camera)
{
    M540_CAMERAVIEW *view;
    if(camera->mode != 2) {
        view = &camera->view[0];
        Hu3DCameraPosSet(camera->cameraBit,
            view->pos.x, view->pos.y, view->pos.z,
            view->up.x, view->up.y, view->up.z,
            view->center.x, view->center.y, view->center.z);
    }
}

/* Sleeps tick frames while updating the camera, returns on a mode change */
#define M540_CAMERA_SLEEP(tick) \
do { \
    int i_; \
    int mode_; \
    int num_; \
    num_ = (tick); \
    for(i_=0; i_<num_; i_++) { \
        mode_ = M540_CURCAMERA->mode; \
        fn_1_1432C(M540_CURCAMERA); \
        HuPrcVSleep(); \
        if(mode_ != M540_CURCAMERA->mode) { \
            return; \
        } \
    } \
} while(0)

static void fn_1_1439C(M540_CAMERA *camera)
{
    camera->mode = 2;
}

static void fn_1_143A8(M540_CAMERA *camera)
{
    while(1) {
        M540_CAMERA_SLEEP(1);
    }
}

static void fn_1_1445C(M540_CAMERA *camera)
{
    camera->mode = 0;
}

static void fn_1_14468(M540_CAMERA *camera)
{
    while(1) {
        fn_1_14DF4(camera);
        fn_1_15270(camera);
        M540_CAMERA_SLEEP(1);
    }
}

static BOOL fn_1_14530(M540_CAMERA *camera)
{
    return (camera->mode != 0) ? TRUE : FALSE;
}

static void fn_1_14560(M540_CAMERA *camera, HuVecF *center, HuVecF *rot, float *zoom, int time)
{
    M540_CAMERAVIEW *view = &camera->view[0];
    M540_CAMERAVIEW *start = &camera->view[1];
    M540_CAMERAVIEW *end = &camera->view[2];
    *start = *view;
    *end = *view;
    if(center != NULL) {
        end->center = *center;
    }
    if(rot != NULL) {
        end->rot = *rot;
    }
    if(zoom != NULL) {
        end->zoom = *zoom;
    }
    fn_1_140B4(camera);
    camera->time = time;
    if(time > 0) {
        camera->mode = 1;
    } else {
        *view = *end;
        camera->mode = 0;
    }
}

static void fn_1_149A4(M540_CAMERA *camera)
{
    M540_CAMERAVIEW *view = &camera->view[0];
    M540_CAMERAVIEW *start = &camera->view[1];
    M540_CAMERAVIEW *end = &camera->view[2];
    float step = 1.0f/camera->time;
    float t = 0;
    float s;
    while(1) {
        s = HuSin(90.0f*t);
        s = s*s;
        view->center.x = start->center.x+(s*(end->center.x-start->center.x));
        view->center.y = start->center.y+(s*(end->center.y-start->center.y));
        view->center.z = start->center.z+(s*(end->center.z-start->center.z));
        view->pos.x = start->pos.x+(s*(end->pos.x-start->pos.x));
        view->pos.y = start->pos.y+(s*(end->pos.y-start->pos.y));
        view->pos.z = start->pos.z+(s*(end->pos.z-start->pos.z));
        view->up.x = start->up.x+(s*(end->up.x-start->up.x));
        view->up.y = start->up.y+(s*(end->up.y-start->up.y));
        view->up.z = start->up.z+(s*(end->up.z-start->up.z));
        fn_1_14DF4(camera);
        fn_1_15270(camera);
        t += step;
        if(t >= 1.0f) {
            break;
        }
        M540_CAMERA_SLEEP(1);
    }
    *view = *end;
    camera->mode = 0;
}

static void fn_1_14CB0(M540_CAMERA *camera, float vpX, float vpY, float vpW, float vpH, float vpNearZ, float vpFarZ)
{
    Hu3DCameraViewportSet(camera->cameraBit, vpX, vpY, vpW, vpH, vpNearZ, vpFarZ);
}

static void fn_1_14D10(M540_CAMERA *camera, float fov, float near, float far, float aspect)
{
    Hu3DCameraPerspectiveSet(camera->cameraBit, fov, near, far, aspect);
}

static void fn_1_14D60(M540_CAMERA *camera, float scissorX, float scissorY, float scissorW, float scissorH)
{
    Hu3DCameraScissorSet(camera->cameraBit, scissorX, scissorY, scissorW, scissorH);
}

static void fn_1_14DF4(M540_CAMERA *camera)
{
    M540_CAMERAVIEW *view;
    if(camera->debugF && (HuPadBtn[0] & PAD_BUTTON_Y)) {
        view = &camera->view[0];
        view->center.x += 0.1f*HuPadSubStkX[0];
        view->center.z -= 0.02f*HuPadSubStkY[0];
        view->rot.y -= 0.1f*HuPadStkX[0];
        view->rot.x += 0.1f*HuPadStkY[0];
        view->zoom += 0.1f*HuPadTrigL[0];
        view->zoom -= 0.1f*HuPadTrigR[0];
        fn_1_140B4(camera);
    }
}

static void fn_1_15270(M540_CAMERA *camera)
{
}

static void fn_1_15274(int time, BOOL waitF)
{
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_PREVTYPE, time);
    while(waitF && WipeCheckEnd()) {
        HuPrcVSleep();
    }
}

static void fn_1_152D0(int time, BOOL waitF)
{
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_WHITEFADE, time);
    while(waitF && WipeCheckEnd()) {
        HuPrcVSleep();
    }
}

static void fn_1_1532C(int time)
{
    int i;
    GAMEMESID gameMesId = GameMesMgStartCreate();
    if(time == -1) {
        while(GameMesStatGet(gameMesId)) {
            HuPrcVSleep();
        }
    } else {
        for(i=0; i<time; i++) {
            HuPrcVSleep();
        }
    }
}

static void fn_1_153B8(int time)
{
    int i;
    GAMEMESID gameMesId = GameMesMgFinishCreate();
    if(time == -1) {
        while(GameMesStatGet(gameMesId)) {
            HuPrcVSleep();
        }
    } else {
        for(i=0; i<time; i++) {
            HuPrcVSleep();
        }
    }
}

static float fn_1_15444(float start, float end, float t)
{
    float diff;
    float angle;
    diff = fmod(end-start, 360.0);
    if(diff < 0.0f) {
        diff += 360.0f;
    }
    if(diff > 180.0f) {
        diff -= 360.0f;
    }
    angle = fmod(start+(diff*t), 360.0);
    if(angle < 0.0f) {
        angle += 360.0f;
    }
    return angle;
}

static M540_WIN *fn_1_15548(int w, int h)
{
    M540_WIN *win = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M540_WIN), HU_MEMNUM_OVL);
    win->process = HuPrcChildCreate(fn_1_15630, 0x1000, 0x2000, 0, HuPrcCurrentGet());
    win->process->property = win;
    win->winId = HuWinExCreateFrame(0, 0, w, h, HUWIN_SPEAKER_NONE, 1);
    return win;
}

static void fn_1_155F0(M540_WIN *win)
{
    HuWinKill(win->winId);
    HuPrcKill(win->process);
    HuMemDirectFree(win);
}

static void fn_1_15630(void)
{
    while(1) {
        HuPrcVSleep();
    }
}

static void fn_1_15644(M540_WIN *win, float x, float y)
{
    HuWinPosSet(win->winId, win->x = x, win->y = y);
}

static void fn_1_1568C(M540_WIN *win, float *x, float *y)
{
    *x = win->x;
    *y = win->y;
}

static void fn_1_156A0(M540_WIN *win, u32 mess)
{
    HuWinMesSet(win->winId, mess);
}

static void fn_1_156D8(M540_WIN *win)
{
    HuWinDispOn(win->winId);
    win->dispF = TRUE;
}

static void fn_1_15710(M540_WIN *win)
{
    HuWinDispOff(win->winId);
    win->dispF = FALSE;
}

static void fn_1_15748(M540_WIN *win, int color)
{
    HuWinMesColSet(win->winId, color);
}

static void fn_1_15784(M540_WIN *win, s16 speed)
{
    HuWinMesSpeedSet(win->winId, speed);
}

static void fn_1_157BC(M540_WIN *win)
{
    HuWinMesWait(win->winId);
}

static void fn_1_157EC(M540_WIN *win)
{
    HuWinExOpen(win->winId);
}

static void fn_1_1581C(M540_WIN *win)
{
    HuWinExClose(win->winId);
}

static void fn_1_1584C(M540_WIN *win, float scaleX, float scaleY)
{
    HuWinScaleSet(win->winId, scaleX, scaleY);
}
