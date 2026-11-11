#include "game/animdata.h"
#include "game/armem.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/mgdata.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/sprite.h"
#include "game/thpmain.h"
#include "game/window.h"
#include "game/wipe.h"

#include "REL/mgfreeDll.h"

#include <math.h>
#include <string.h>

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* Not declared in game/hu3d.h (see bootDll/boot.c). */
float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

#define MGFREE_CAM 0x10
#define MGFREE_MODEL_NUM 14

typedef struct MgFreeEntry_s {
    s32 model; /* 0x00 */
    s8 slot; /* 0x04 */
    f32 unk8; /* 0x08 */
    f32 angle; /* 0x0C */
    f32 y; /* 0x10 */
    f32 scale; /* 0x14 */
    f32 startAngle; /* 0x18 */
    f32 startY; /* 0x1C */
    f32 startScale; /* 0x20 */
    f32 endAngle; /* 0x24 */
    f32 endY; /* 0x28 */
    f32 endScale; /* 0x2C */
} MgFreeEntry;

typedef struct MgFreeWork_s {
    s16 model; /* 0x000 */
    HuVecF pos; /* 0x004 */
    MgFreeEntry entry[8]; /* 0x010 */
    OMOBJMAN *objman; /* 0x190 */
    s32 models[MGFREE_MODEL_NUM]; /* 0x194 */
    u32 flags; /* 0x1CC */
    s16 selMg; /* 0x1D0 */
    HUPROCESS *procLeft; /* 0x1D4 */
    HUPROCESS *procRight; /* 0x1D8 */
    HUPROCESS *procMovie; /* 0x1DC */
    s16 movieState; /* 0x1E0 */
    s32 bgmStream; /* 0x1E4 */
    CAMMOTIONWORK camWork[31]; /* 0x1E8 */
    s32 camCount; /* 0x7B8 */
    s32 camCur; /* 0x7BC */
    s32 camNext; /* 0x7C0 */
    ANIMDATA *mgAnim[90]; /* 0x7C4 */
    ANIMDATA *shotAnim[2]; /* 0x92C */
    ANIMDATA *noMovieAnim; /* 0x934 */
    HU3DANIMID anim; /* 0x938 */
} MgFreeWork;

typedef struct MgFreeMode_s {
    u32 unk0;
    s32 sel[3];
    s32 idx;
    HuVecF pos;
    HuVecF up;
    HuVecF target;
    f32 fov;
} MgFreeMode;

typedef struct MgFreeModelData_s {
    s32 dataNum;
    s32 camBit;
    s32 layer;
    s32 attr1;
    s32 attr2;
} MgFreeModelData;

static MgFreeWork lbl_1_bss_8;
static MgFreeMode *lbl_1_bss_4;
static MgFreeWork *lbl_1_bss_0;

static HuVecF lbl_1_data_0[2] = {
    { 1000.0f, -900.0f, 0.0f },
    { 3000.0f, -900.0f, 0.0f },
};

static MgFreeModelData lbl_1_data_18[13] = {
    { 0x970002, 0x10, 0, 0, 0x40000001 },
    { 0x970003, 0x10, 3, 0, 0x40000001 },
    { 0x970004, 0x10, 5, 0, 0x40000001 },
    { 0x970005, 0x10, 5, 0, 0x40000001 },
    { 0x970006, 0x10, 3, 0, 0x40000001 },
    { 0x970008, 0x10, 3, 0, 0x40000001 },
    { 0x97000A, 0x10, 3, 0, 0x40000001 },
    { 0x97000C, 0x10, 3, 0, 0x40000001 },
    { 0x97000D, 0x10, 3, 0, 0x40000001 },
    { 0x97000B, 0x10, 3, 0, 0x40000001 },
    { 0x970009, 0x10, 3, 0, 0x40000001 },
    { 0x970007, 0x10, 3, 0, 0x40000001 },
    { 0x97000E, 0x10, 4, 0, 0x40000001 },
};

static u32 lbl_1_data_11C[8][2] = {
    { 0, 0xE0007 },
    { 1, 0xE0008 },
    { 2, 0xE0009 },
    { 3, 0xE000A },
    { 6, 0xE000B },
    { 4, 0xE000C },
    { 8, 0xE000D },
    { 5, 0xE000E },
};

static HuVecF lbl_1_data_15C = { 0.0f, 650.0f, -900.0f };

static f32 lbl_1_data_168[8] = { 20.0f, 120.0f, 220.0f, 320.0f, 370.0f, 320.0f, 220.0f, 120.0f };

static f32 lbl_1_data_188[8] = { 1.0f, 0.9f, 0.8f, 0.7f, 0.6f, 0.7f, 0.8f, 0.9f };

static HuVecF lbl_1_data_1A8[2] = {
    { 270.0f, 0.0f, 200.0f },
    { 5000.0f, 0.0f, 5000.0f },
};

static HuVecF lbl_1_data_1C0[2] = {
    { -60.0f, -80.0f, 400.0f },
    { 2000.0f, -80.0f, 400.0f },
};

int _prolog(void)
{
    const VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void fn_1_A0(s32 time)
{
    if (WipeTypeGet() == 0x89) {
        WipeCreate(1, 0, time);
    } else {
        WipeCreate(1, 5, time);
    }
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
}

void fn_1_114(s32 time)
{
    WipeCreate(2, 6, time);
    while (WipeCheckEnd()) {
        HuPrcVSleep();
    }
}

void fn_1_160(s32 model, f32 start, f32 end)
{
    Hu3DMotionStartEndSet(model, start, end);
    Hu3DMotionTimeSet(model, start);
    Hu3DMotionShapeTimeSet(model, start);
}

void fn_1_1C8(u32 flags)
{
    lbl_1_bss_0->flags |= flags;
}

void fn_1_1E4(u32 flags)
{
    lbl_1_bss_0->flags &= ~flags;
}

u32 fn_1_200(u32 mask)
{
    return lbl_1_bss_0->flags & mask;
}

u8 fn_1_218(void)
{
    return lbl_1_data_11C[lbl_1_bss_4->idx][0];
}

u32 fn_1_244(void)
{
    return lbl_1_data_11C[lbl_1_bss_4->idx][1];
}

void fn_1_26C(s32 evtno)
{
    OMOVLHIS *his = omOvlHisGet(0);
    omOvlHisChg(0, his->ovl, evtno, his->stat);
}

f32 fn_1_2B4(f32 a, f32 b, f32 t)
{
    return b + (t * (a - b));
}

s32 fn_1_2C4(f32 *angle, f32 end, f32 speed)
{
    f32 diff = *angle - end;
    f32 change = fmodf(diff, 180);
    f32 wrap = fmodf(diff, 360);

    if (fabsf(wrap) > 180.0f) {
        if (change > 0) {
            change = -(180.0f - fabsf(change));
        } else {
            change = 180.0f - fabsf(change);
        }
    }
    if (fabsf(change) < speed) {
        *angle = end;
        return 1;
    }
    if (change > 0) {
        change -= speed;
    } else {
        change += speed;
    }
    *angle = end + change;
    return 0;
}

void fn_1_4CC(void)
{
    s32 model = lbl_1_bss_0->models[3];

    lbl_1_bss_0->flags &= ~4;
    Hu3DModelAttrReset(model, 0x40000001);
    Hu3DMotionStartEndSet(model, 121.0f, 145.0f);
    Hu3DMotionTimeSet(model, 121.0f);
    Hu3DMotionShapeTimeSet(model, 121.0f);
    while (!Hu3DMotionEndCheck(model)) {
        if (lbl_1_bss_0->flags & 4) {
            Hu3DMotionStartEndSet(model, 121.0f, 145.0f);
            Hu3DMotionTimeSet(model, 121.0f);
            Hu3DMotionShapeTimeSet(model, 121.0f);
            lbl_1_bss_0->flags &= ~4;
        }
        HuPrcVSleep();
    }
    Hu3DModelAttrReset(model, 0x40000002);
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DMotionStartEndSet(model, 0.0f, 120.0f);
    Hu3DMotionTimeSet(model, 0.0f);
    Hu3DMotionShapeTimeSet(model, 0.0f);
    lbl_1_bss_0->procLeft = NULL;
    HuPrcEnd();
}

void fn_1_684(void)
{
    lbl_1_bss_0->flags |= 4;
    if (lbl_1_bss_0->procLeft == 0) {
        lbl_1_bss_0->procLeft = HuPrcChildCreate(fn_1_4CC, 100, 0x1000, 0, lbl_1_bss_0->objman);
    }
}

void fn_1_708(void)
{
    s32 model = lbl_1_bss_0->models[2];

    lbl_1_bss_0->flags &= ~8;
    Hu3DModelAttrReset(model, 0x40000001);
    Hu3DMotionStartEndSet(model, 121.0f, 145.0f);
    Hu3DMotionTimeSet(model, 121.0f);
    Hu3DMotionShapeTimeSet(model, 121.0f);
    while (!Hu3DMotionEndCheck(model)) {
        if (lbl_1_bss_0->flags & 8) {
            Hu3DMotionStartEndSet(model, 121.0f, 145.0f);
            Hu3DMotionTimeSet(model, 121.0f);
            Hu3DMotionShapeTimeSet(model, 121.0f);
            lbl_1_bss_0->flags &= ~8;
        }
        HuPrcVSleep();
    }
    Hu3DModelAttrReset(model, 0x40000002);
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DMotionStartEndSet(model, 0.0f, 120.0f);
    Hu3DMotionTimeSet(model, 0.0f);
    Hu3DMotionShapeTimeSet(model, 0.0f);
    lbl_1_bss_0->procRight = NULL;
    HuPrcEnd();
}

void fn_1_8C0(void)
{
    lbl_1_bss_0->flags |= 8;
    if (lbl_1_bss_0->procRight == 0) {
        lbl_1_bss_0->procRight = HuPrcChildCreate(fn_1_708, 100, 0x1000, 0, lbl_1_bss_0->objman);
    }
}

void fn_1_944(s32 dir)
{
    HuVecF pos;
    MgFreeEntry *entry;
    f32 t;
    f32 rad;
    f32 angle;
    s32 i;
    s32 frame;
    s32 total;

    fn_1_A454();
    lbl_1_bss_4->idx += dir;
    if (lbl_1_bss_4->idx < 0) {
        lbl_1_bss_4->idx += 8;
    }
    lbl_1_bss_4->idx %= 8;
    HuAudFXPlay(0x4F3);
    if (dir > 0) {
        fn_1_8C0();
        entry = lbl_1_bss_0->entry;
        i = 8;
        while (i--) {
            entry->slot++;
            entry->slot = entry->slot % 8;
            entry++;
        }
    } else if (dir < 0) {
        fn_1_684();
        entry = lbl_1_bss_0->entry;
        i = 8;
        while (i--) {
            entry->slot--;
            if (entry->slot < 0) {
                entry->slot += 8;
            }
            entry++;
        }
    }
    total = 15;
    frame = 0;
    Hu3DModelAttrSet(lbl_1_bss_0->models[12], 1);
    entry = lbl_1_bss_0->entry;
    i = 8;
    while (i--) {
        angle = 90.0f + (45.0f * (f32)entry->slot);
        if (angle > 360.0f) {
            angle -= 360.0f;
        }
        entry->unk8 = 0.0f;
        entry->startAngle = entry->angle;
        entry->startY = entry->y;
        entry->startScale = entry->scale;
        entry->endAngle = angle;
        entry->endY = lbl_1_data_168[entry->slot];
        entry->endScale = lbl_1_data_188[entry->slot];
        if (dir < 0 && entry->startAngle < entry->endAngle) {
            entry->startAngle = entry->startAngle + 360.0f;
        }
        entry++;
    }
    while (frame != total) {
        frame++;
        entry = lbl_1_bss_0->entry;
        t = (f32)frame / (f32)total;
        t = sin(3.141592653589793 * t * 0.5);
        t = sin(3.141592653589793 * t * 0.5);
        i = 8;
        while (i--) {
            entry->angle = fn_1_2B4(entry->endAngle, entry->startAngle, t);
            entry->y = fn_1_2B4(entry->endY, entry->startY, t);
            entry->scale = fn_1_2B4(entry->endScale, entry->startScale, t);
            entry++;
        }
        entry = lbl_1_bss_0->entry;
        i = 8;
        while (i--) {
            rad = (3.141592653589793 * entry->angle) / 180.0;
            pos.x = lbl_1_data_15C.x + (lbl_1_data_1A8[0].x * cos(rad));
            pos.y = lbl_1_data_15C.y + entry->y;
            pos.z = lbl_1_data_15C.z + (lbl_1_data_1A8[0].z * sin(rad));
            Hu3DModelPosSetV(entry->model, &pos);
            Hu3DModelScaleSet(entry->model, entry->scale, entry->scale, entry->scale);
            entry++;
        }
        HuPrcVSleep();
    }
    entry = lbl_1_bss_0->entry;
    i = 8;
    while (i--) {
        t = (f32)frame / (f32)total;
        entry->angle = fn_1_2B4(entry->endAngle, entry->startAngle, t);
        entry->y = fn_1_2B4(entry->endY, entry->startY, t);
        entry->scale = fn_1_2B4(entry->endScale, entry->startScale, t);
        if (entry->angle >= 360.0f) {
            entry->angle -= 360.0f;
        }
        if (entry->angle >= 360.0f) {
            entry->angle -= 360.0f;
        }
        if (entry->angle >= 360.0f) {
            entry->angle -= 360.0f;
        }
        rad = (3.141592653589793 * entry->angle) / 180.0;
        pos.x = lbl_1_data_15C.x + (lbl_1_data_1A8[0].x * cos(rad));
        pos.y = lbl_1_data_15C.y + entry->y;
        pos.z = lbl_1_data_15C.z + (lbl_1_data_1A8[0].z * sin(rad));
        Hu3DModelPosSetV(entry->model, &pos);
        Hu3DModelScaleSet(entry->model, entry->scale, entry->scale, entry->scale);
        entry++;
    }
    Hu3DModelAttrReset(lbl_1_bss_0->models[12], 1);
    Hu3DMotionTimeSet(lbl_1_bss_0->models[12], 0.0f);
}

void fn_1_128C(void)
{
    s32 thpSpr = -1;
    s32 timer;
    s32 run = 1;
    s32 mgNo;
    char *movie;

    while (run != 0) {
        switch (lbl_1_bss_0->movieState) {
            case 0:
                OSReport("\t\tMovie Phase : \217\211\212\372\211\273\n");
                if (thpSpr >= 0) {
                    HuTHPClose();
                    Hu3DModelMatHookSet(lbl_1_bss_0->models[1], NULL);
                    thpSpr = -1;
                }
                timer = 120;
                OSReport("\t\tMovie Phase : \212J\216n\203J\203E\203\223\203g\212J\216n\n");
                lbl_1_bss_0->movieState = 1;
                break;
            case 1:
                timer--;
                if (timer == 0) {
                    OSReport("\t\tMovie Phase : \212J\216n\203J\203E\203\223\203g\217I\227\271\n");
                    OSReport("\t\tMovie Phase : \203\200\201[\203r\201[\217I\227\271\221\322\213@\n");
                    lbl_1_bss_0->movieState = 2;
                }
                break;
            case 2:
                if (HuTHPProcCheck() == 0) {
                    OSReport("\t\tMovie Phase : \203\200\201[\203r\201[\217I\227\271\n");
                    lbl_1_bss_0->movieState = 3;
                }
                break;
            case 3:
                mgNo = fn_1_A4DC();
                if (mgNo >= 0) {
                    movie = mgDataTbl[mgNo].movie[0];
                    if (mgNo + 0x1F5 == 0x207) {
                        movie = mgDataTbl[mgNo].movie[(s8)fn_1_4D4C()];
                    }
                    OSReport("\t\tMovie Phase : \203\200\201[\203r\201[\223\307\202\335\215\236\202\335\212J\216n\n");
                    thpSpr = HuTHPSprCreateVol(movie, 1, 0, 0.0f);
                    lbl_1_bss_0->movieState = 4;
                }
                break;
            case 4:
                if (HuTHPStartCheck() != 0) {
                    OSReport("\t\tMovie Phase : \203\200\201[\203r\201[\223\307\202\335\215\236\202\335\217I\227\271\n");
                    Hu3DModelMatHookSet(lbl_1_bss_0->models[1], HuTHPMatHook);
                    lbl_1_bss_0->movieState = 5;
                }
                break;
            case 6:
                if (thpSpr >= 0) {
                    HuTHPClose();
                    Hu3DModelMatHookSet(lbl_1_bss_0->models[1], NULL);
                    thpSpr = -1;
                }
                lbl_1_bss_0->movieState = 5;
                break;
            case 7:
                if (thpSpr >= 0) {
                    HuTHPClose();
                    Hu3DModelMatHookSet(lbl_1_bss_0->models[1], NULL);
                    thpSpr = -1;
                }
                if (HuTHPProcCheck() == 0) {
                    run = 0;
                }
                break;
        }
        HuPrcVSleep();
    }
    lbl_1_bss_0->procMovie = NULL;
    HuPrcEnd();
}

void fn_1_1568(void)
{
    lbl_1_bss_0->movieState = 7;
    while (lbl_1_bss_0->procMovie != 0) {
        HuPrcVSleep();
    }
}

void fn_1_15B8(void)
{
    s32 mgNo = fn_1_A4DC();

    if (lbl_1_bss_0->anim >= 0) {
        Hu3DAnimKill(lbl_1_bss_0->anim);
    }
    if (mgNo >= 0) {
        if (mgNo + 0x1F5 == 0x207) {
            switch ((s8)fn_1_4D4C()) {
                case 0:
                    lbl_1_bss_0->anim = Hu3DAnimCreate(lbl_1_bss_0->mgAnim[mgNo], lbl_1_bss_0->models[1], "mg_shot");
                    break;
                case 1:
                    lbl_1_bss_0->anim = Hu3DAnimCreate(lbl_1_bss_0->shotAnim[0], lbl_1_bss_0->models[1], "mg_shot");
                    break;
                case 2:
                    lbl_1_bss_0->anim = Hu3DAnimCreate(lbl_1_bss_0->shotAnim[1], lbl_1_bss_0->models[1], "mg_shot");
                    break;
            }
        } else {
            lbl_1_bss_0->anim = Hu3DAnimCreate(lbl_1_bss_0->mgAnim[mgNo], lbl_1_bss_0->models[1], "mg_shot");
        }
        lbl_1_bss_0->movieState = 0;
        if (lbl_1_bss_0->procMovie == 0) {
            lbl_1_bss_0->procMovie = HuPrcChildCreate(fn_1_128C, 200, 0x1000, 0, lbl_1_bss_0->objman);
        }
    } else {
        lbl_1_bss_0->anim = Hu3DAnimCreate(lbl_1_bss_0->noMovieAnim, lbl_1_bss_0->models[1], "mg_shot");
        lbl_1_bss_0->movieState = 6;
    }
}

void fn_1_1828(void)
{
    s32 n;

    if (lbl_1_bss_0->camCur >= 0) {
        Hu3DCameraMotionOff(lbl_1_bss_0->models[lbl_1_bss_0->camCur]);
        Hu3DModelAttrSet(lbl_1_bss_0->models[lbl_1_bss_0->camCur], 0x40000002);
    }
    Hu3DModelAttrReset(lbl_1_bss_0->models[lbl_1_bss_0->camNext], 0x40000002);
    Hu3DCameraMotionOn(lbl_1_bss_0->models[lbl_1_bss_0->camNext], MGFREE_CAM);
    Hu3DCameraMotionStart(lbl_1_bss_0->models[lbl_1_bss_0->camNext], MGFREE_CAM);
    lbl_1_bss_0->camCur = lbl_1_bss_0->camNext;
    n = (s32)Hu3DMotionMaxTimeGet(lbl_1_bss_0->models[lbl_1_bss_0->camNext]);
    while (n--) {
        HuPrcVSleep();
    }
    lbl_1_bss_0->flags |= 2;
    HuPrcEnd();
}

void fn_1_19F8(s32 cam)
{
    lbl_1_bss_0->flags &= ~2;
    if (lbl_1_bss_0->camCur == cam) {
        lbl_1_bss_0->flags |= 2;
        return;
    }
    lbl_1_bss_0->camNext = cam;
    if (cam < 0) {
        if (lbl_1_bss_0->camCur >= 0) {
            Hu3DCameraMotionOff(lbl_1_bss_0->models[lbl_1_bss_0->camCur]);
            Hu3DModelAttrSet(lbl_1_bss_0->models[lbl_1_bss_0->camCur], 0x40000002);
        }
        lbl_1_bss_0->camCur = lbl_1_bss_0->camNext;
        lbl_1_bss_0->flags |= 2;
        return;
    }
    lbl_1_bss_0->flags &= ~2;
    HuPrcChildCreate(fn_1_1828, 100, 0x1000, 0, lbl_1_bss_0->objman);
    HuPrcVSleep();
}

void fn_1_1B84(void)
{
    s32 model;
    s32 model12;
    s32 model3;
    s32 model2;

    {
        MgFreeModelData *data;
        s32 i = 13;

        while (i--) {
            data = &lbl_1_data_18[i];
            model = Hu3DModelCreate(HuDataSelHeapReadNum(data->dataNum, HU_MEMNUM_OVL, HEAP_MODEL));
            lbl_1_bss_0->models[i] = model;
            Hu3DModelCameraSet(model, data->camBit);
            Hu3DModelLayerSet(model, data->layer);
            if (data->attr1 != 0) {
                Hu3DModelAttrSet(model, data->attr1);
            }
            if (data->attr2 != 0) {
                Hu3DModelAttrSet(model, data->attr2);
            }
        }
    }
    Hu3DModelPosSetV(lbl_1_bss_0->models[1], &lbl_1_data_1C0[0]);
    model12 = lbl_1_bss_0->models[12];
    Hu3DMotionStartEndSet(model12, 0.0f, 300.0f);
    Hu3DMotionTimeSet(model12, 0.0f);
    Hu3DMotionShapeTimeSet(model12, 0.0f);
    Hu3DModelPosSet(lbl_1_bss_0->models[3], 20.0f, 0.0f, 400.0f);
    Hu3DModelPosSet(lbl_1_bss_0->models[2], -20.0f, 0.0f, 400.0f);
    Hu3DModelScaleSet(lbl_1_bss_0->models[3], 0.8f, 0.8f, 0.8f);
    Hu3DModelScaleSet(lbl_1_bss_0->models[2], 0.8f, 0.8f, 0.8f);
    model3 = lbl_1_bss_0->models[3];
    Hu3DMotionStartEndSet(model3, 0.0f, 120.0f);
    Hu3DMotionTimeSet(model3, 0.0f);
    Hu3DMotionShapeTimeSet(model3, 0.0f);
    model2 = lbl_1_bss_0->models[2];
    Hu3DMotionStartEndSet(model2, 0.0f, 120.0f);
    Hu3DMotionTimeSet(model2, 0.0f);
    Hu3DMotionShapeTimeSet(model2, 0.0f);
    {
        s32 camData[1] = { 0x970039 };
        s32 j = 1;

        while (j--) {
            model = Hu3DModelCameraCreate(Hu3DMotionCreate(HuDataSelHeapReadNum(camData[j], HU_MEMNUM_OVL, HEAP_MODEL)), MGFREE_CAM);
            lbl_1_bss_0->models[j + 13] = model;
            Hu3DModelAttrSet(model, 0x40000002);
            Hu3DCameraMotionOff(model);
        }
    }
}

void ObjectSetup(void)
{
    _SetFlag(0xB);
    mgPauseExitF = TRUE;
    GwSystem.subGameNo = 0;
    HuPrcChildCreate(fn_1_1F50, 200, 0x3000, 0, HuPrcCurrentGet());
}

void fn_1_1F50(void)
{
    OMOVLHIS *his;
    s32 run;

    his = omOvlHisGet(0);
    fn_1_21AC();
    run = 1;
    while (run != 0) {
        switch (his->evtno) {
            case 0:
                fn_1_2C40();
                break;
            case 1:
                fn_1_37A4();
                break;
            case 2:
                fn_1_3C8C();
                break;
            case 3:
                fn_1_205C();
                break;
            case 4:
                HuAudSStreamFadeOut(lbl_1_bss_0->bgmStream, 1000);
                lbl_1_bss_0->bgmStream = -1;
                fn_1_114(60);
                run = 0;
                break;
        }
        HuPrcVSleep();
    }
    fn_1_45A4();
    omOvlReturnEx(1, 1);
    HuPrcEnd();
}

void fn_1_205C(void)
{
    s32 mgNo;
    OMOVLHIS *his;

    HuAudSStreamFadeOut(lbl_1_bss_0->bgmStream, 1000);
    lbl_1_bss_0->bgmStream = -1;
    fn_1_114(60);
    fn_1_1568();
    his = omOvlHisGet(0);
    omOvlHisChg(0, his->ovl, 1, his->stat);
    HuDataDirClose(0x970000);
    HuDataDirClose(0x270000);
    GwSystem.mgNo = lbl_1_bss_0->selMg;
    mgNo = lbl_1_bss_0->selMg + 0x1F5;
    GWMgUnlockSet(mgNo);
    GwSystem.mgNo = mgNo - 0x1F5;
    GwSystem.mgInstF = 1;
    omOvlCallEx(7, 1, 0, 0);
    while (1) {
        HuPrcVSleep();
    }
}

void fn_1_21AC(void)
{
    HuVecF lightPos;
    HuVecF lightDir;
    OMOVLHIS *his;
    HU3DLIGHTID light;
    s32 model12;
    s32 model3;
    s32 model2;
    s32 model;
    s32 i;
    s32 j;
    s8 slot;
    s32 entIdx;
    MgFreeEntry *entry;
    f32 angle;
    ANIMDATA *anim;

    his = omOvlHisGet(0);
    lbl_1_bss_4 = (MgFreeMode *)mgModeWork;
    lbl_1_bss_0 = &lbl_1_bss_8;
    HuDataSelHeapReadNum(0x270000, HU_MEMNUM_OVL, HEAP_MODEL);
    HuAR_DVDtoARAM(0x970000);
    lbl_1_bss_0->camCur = -1;
    lbl_1_bss_0->anim = -1;
    lbl_1_bss_0->procLeft = NULL;
    lbl_1_bss_0->procRight = NULL;
    lbl_1_bss_0->procMovie = NULL;
    lbl_1_bss_0->bgmStream = -1;
    lbl_1_bss_0->movieState = 0;
    {
        HuVecF pos = { 3000.0f, 3000.0f, 3000.0f };
        HuVecF dir = { -1.0f, -1.0f, -1.0f };

        lightPos = pos;
        lightDir = dir;
    }
    lbl_1_bss_0->objman = omInitObjMan(200, 1000);
    omGameSysInit(lbl_1_bss_0->objman);
    Hu3DCameraCreate(MGFREE_CAM);
    Hu3DCameraPerspectiveSet(MGFREE_CAM, 60.0f, 20.0f, 60000.0f, 1.2f);
    Hu3DCameraViewportSet(MGFREE_CAM, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(MGFREE_CAM, 0, 0, 640, 480);
    light = Hu3DGLightCreate(0.0f, 1000.0f, 1000.0f, 0.0f, -1.0f, -1.0f, 255, 255, 255);
    Hu3DGLightStaticSet(light, 1);
    Hu3DGLightPosSetV(light, &lightPos, &lightDir);
    HuWinInit(1);
    if (his->evtno == 0) {
        _SetFlag(0x30003);
        _SetFlag(0x30001);
        mgInstExitF = TRUE;
        lbl_1_bss_4->idx = 0;
        lbl_1_bss_4->sel[0] = 0;
        lbl_1_bss_4->sel[1] = 0;
        lbl_1_bss_4->sel[2] = 0;
    }
    {
        MgFreeModelData *data;
        s32 i = 13;

        while (i--) {
            data = &lbl_1_data_18[i];
            model = Hu3DModelCreate(HuDataSelHeapReadNum(data->dataNum, HU_MEMNUM_OVL, HEAP_MODEL));
            lbl_1_bss_0->models[i] = model;
            Hu3DModelCameraSet(model, data->camBit);
            Hu3DModelLayerSet(model, data->layer);
            if (data->attr1 != 0) {
                Hu3DModelAttrSet(model, data->attr1);
            }
            if (data->attr2 != 0) {
                Hu3DModelAttrSet(model, data->attr2);
            }
        }
    }
    Hu3DModelPosSetV(lbl_1_bss_0->models[1], &lbl_1_data_1C0[0]);
    model12 = lbl_1_bss_0->models[12];
    Hu3DMotionStartEndSet(model12, 0.0f, 300.0f);
    Hu3DMotionTimeSet(model12, 0.0f);
    Hu3DMotionShapeTimeSet(model12, 0.0f);
    Hu3DModelPosSet(lbl_1_bss_0->models[3], 20.0f, 0.0f, 400.0f);
    Hu3DModelPosSet(lbl_1_bss_0->models[2], -20.0f, 0.0f, 400.0f);
    Hu3DModelScaleSet(lbl_1_bss_0->models[3], 0.8f, 0.8f, 0.8f);
    Hu3DModelScaleSet(lbl_1_bss_0->models[2], 0.8f, 0.8f, 0.8f);
    model3 = lbl_1_bss_0->models[3];
    Hu3DMotionStartEndSet(model3, 0.0f, 120.0f);
    Hu3DMotionTimeSet(model3, 0.0f);
    Hu3DMotionShapeTimeSet(model3, 0.0f);
    model2 = lbl_1_bss_0->models[2];
    Hu3DMotionStartEndSet(model2, 0.0f, 120.0f);
    Hu3DMotionTimeSet(model2, 0.0f);
    Hu3DMotionShapeTimeSet(model2, 0.0f);
    {
        s32 camData[1] = { 0x970039 };
        s32 j = 1;

        while (j--) {
            model = Hu3DModelCameraCreate(Hu3DMotionCreate(HuDataSelHeapReadNum(camData[j], HU_MEMNUM_OVL, HEAP_MODEL)), MGFREE_CAM);
            lbl_1_bss_0->models[j + 13] = model;
            Hu3DModelAttrSet(model, 0x40000002);
            Hu3DCameraMotionOff(model);
        }
    }
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x180000, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_0->model = model;
    lbl_1_bss_0->pos = lbl_1_data_0[0];
    Hu3DMotionSet(model, Hu3DJointMotion(model, HuDataSelHeapReadNum(0x180001, HU_MEMNUM_OVL, HEAP_MODEL)));
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 4);
    Hu3DModelCameraSet(model, MGFREE_CAM);
    Hu3DModelPosSetV(model, &lbl_1_bss_0->pos);
    Hu3DModelScaleSet(model, 4.0f, 4.0f, 4.0f);
    i = 0;
    while (i < 8) {
        if (i != 0) {
            lbl_1_data_188[i] = 0.7 * lbl_1_data_188[i - 1];
        } else {
            lbl_1_data_188[i] = 1.0f;
        }
        i++;
    }
    lbl_1_data_188[7] = lbl_1_data_188[1];
    lbl_1_data_188[6] = lbl_1_data_188[2];
    lbl_1_data_188[5] = lbl_1_data_188[3];
    slot = 0;
    while (slot < 8) {
        entIdx = ((8 - lbl_1_bss_4->idx) + slot) % 8;
        entry = &lbl_1_bss_0->entry[entIdx];
        angle = 90.0f + (45.0f * (f32)slot);
        if (angle >= 360.0f) {
            angle -= 360.0f;
        }
        entry->model = lbl_1_bss_0->models[entIdx + 4];
        entry->angle = angle;
        entry->y = lbl_1_data_168[slot];
        entry->scale = lbl_1_data_188[slot];
        entry->slot = slot;
        slot++;
    }
    i = 0;
    while (mgDataTbl[i].ovl != 0xFFFF) {
        j = 8;
        while (j--) {
            if ((s32)lbl_1_data_11C[j][0] == mgDataTbl[i].type) {
                anim = HuSprAnimRead(HuDataSelHeapReadNum(mgDataTbl[i].instPic[0], HU_MEMNUM_OVL, HEAP_MODEL));
                HuSprAnimLock(anim);
                lbl_1_bss_0->mgAnim[i] = anim;
                break;
            }
        }
        i++;
    }
    i = 0;
    while (i < 2) {
        anim = HuSprAnimRead(HuDataSelHeapReadNum(mgDataTbl[18].instPic[i + 1], HU_MEMNUM_OVL, HEAP_MODEL));
        HuSprAnimLock(anim);
        lbl_1_bss_0->shotAnim[i] = anim;
        i++;
    }
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x97001C, HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(anim);
    lbl_1_bss_0->noMovieAnim = anim;
}

void fn_1_2C40(void)
{
    HuVecF pos;
    CAMMOTIONWORK *cam;
    int steps;
    OMOVLHIS *his;
    MgFreeEntry *entry;
    f32 rad;
    f32 t;
    f32 rx;
    f32 rz;
    f32 scale;
    s32 i;
    s32 n;
    s32 m;

    his = omOvlHisGet(0);
    omOvlHisChg(0, his->ovl, 2, his->stat);
    entry = lbl_1_bss_0->entry;
    i = 8;
    while (i--) {
        rad = (3.141592653589793 * entry->angle) / 180.0;
        pos.x = lbl_1_data_15C.x + (lbl_1_data_1A8[1].x * cos(rad));
        pos.y = lbl_1_data_15C.y + entry->y;
        pos.z = lbl_1_data_15C.z + (lbl_1_data_1A8[1].z * sin(rad));
        Hu3DModelPosSetV(entry->model, &pos);
        Hu3DModelScaleSet(entry->model, 0.0f, 0.0f, 0.0f);
        entry++;
    }
    pos.x = lbl_1_data_15C.x + (lbl_1_data_1A8[0].x * cos(1.5707963267948966));
    pos.y = lbl_1_data_15C.y + lbl_1_data_168[0];
    pos.z = lbl_1_data_15C.z + (lbl_1_data_1A8[0].z * sin(1.5707963267948966));
    Hu3DModelPosSetV(lbl_1_bss_0->models[12], &pos);
    Hu3DModelAttrSet(lbl_1_bss_0->models[12], 1);
    Hu3DModelPosSetV(lbl_1_bss_0->models[1], &lbl_1_data_1C0[1]);
    Hu3DModelTPLvlSet(lbl_1_bss_0->models[3], 0.0f);
    Hu3DModelTPLvlSet(lbl_1_bss_0->models[2], 0.0f);
    lbl_1_bss_0->pos = lbl_1_data_0[1];
    Hu3DModelPosSetV(lbl_1_bss_0->model, &lbl_1_bss_0->pos);
    lbl_1_bss_0->bgmStream = HuAudBGMPlay(0x23);
    cam = lbl_1_bss_0->camWork;
    steps = 31;
    cam = lbl_1_bss_0->camWork;
    lbl_1_bss_0->camCount = CamMotionExPathGet(lbl_1_bss_0->models[13], 6, 48.0f, 0, &cam, &steps);
    cam = &lbl_1_bss_0->camWork[lbl_1_bss_0->camCount - 1];
    lbl_1_bss_4->pos = cam->pos;
    lbl_1_bss_4->up = cam->up;
    lbl_1_bss_4->target = cam->target;
    lbl_1_bss_4->fov = cam->fov;
    Hu3DCameraPosSetV(MGFREE_CAM, &lbl_1_bss_4->pos, &lbl_1_bss_4->up, &lbl_1_bss_4->target);
    Hu3DCameraPerspectiveSet(MGFREE_CAM, lbl_1_bss_4->fov, 20.0f, 60000.0f, 1.2f);
    fn_1_19F8(13);
    fn_1_6BC4(1, lbl_1_bss_4->sel[0], lbl_1_bss_4->sel[1], lbl_1_bss_4->sel[2]);
    fn_1_A0(60);
    n = 0;
    while (n <= 60) {
        lbl_1_bss_0->pos.y = lbl_1_data_0[1].y + ((f32)sin(3.141592653589793 * (f32)sin(3.141592653589793 * ((f32)n / 60.0f) * 0.5) * 0.5) * (lbl_1_data_0[0].y - lbl_1_data_0[1].y));
        Hu3DModelPosSetV(lbl_1_bss_0->model, &lbl_1_bss_0->pos);
        Hu3DModelRotSet(lbl_1_bss_0->model, 0.0f, 1080.0f * ((f32)n / 60.0f), 0.0f);
        HuPrcVSleep();
        n++;
    }
    m = 60;
    while (m--) {
        t = (f32)sin(3.141592653589793 * (f32)sin(3.141592653589793 * ((f32)m / 60.0f) * 0.5) * 0.5);
        entry = lbl_1_bss_0->entry;
        i = 8;
        while (i--) {
            rad = (3.141592653589793 * entry->angle) / 180.0;
            pos.y = lbl_1_data_15C.y + entry->y;
            rx = lbl_1_data_1A8[0].x + (t * (lbl_1_data_1A8[1].x - lbl_1_data_1A8[0].x));
            pos.x = lbl_1_data_15C.x + (rx * cos(rad));
            rz = lbl_1_data_1A8[0].z + (t * (lbl_1_data_1A8[1].z - lbl_1_data_1A8[0].z));
            pos.z = lbl_1_data_15C.z + (rz * sin(rad));
            Hu3DModelPosSetV(entry->model, &pos);
            scale = entry->scale * (1.0f - t);
            if (scale < 0.00001f) {
                scale = 0.00001f;
            }
            Hu3DModelScaleSet(entry->model, scale, scale, scale);
            entry++;
        }
        HuPrcVSleep();
    }
    Hu3DModelAttrReset(lbl_1_bss_0->models[12], 1);
    fn_1_A068();
    m = 60;
    while (m--) {
        pos = lbl_1_data_1C0[0];
        pos.x = lbl_1_data_1C0[0].x + (((f32)m / 60.0f) * (lbl_1_data_1C0[1].x - lbl_1_data_1C0[0].x));
        Hu3DModelPosSetV(lbl_1_bss_0->models[1], &pos);
        HuPrcVSleep();
    }
    n = 0;
    while (n <= 30) {
        t = (f32)n / 30.0f;
        Hu3DModelTPLvlSet(lbl_1_bss_0->models[3], t);
        Hu3DModelTPLvlSet(lbl_1_bss_0->models[2], t);
        HuPrcVSleep();
        n++;
    }
}

void fn_1_37A4(void)
{
    HuVecF pos;
    CAMMOTIONWORK *cam;
    int steps;
    OMOVLHIS *his;
    MgFreeEntry *entry;
    f32 rad;
    s32 i;

    his = omOvlHisGet(0);
    omOvlHisChg(0, his->ovl, 2, his->stat);
    entry = lbl_1_bss_0->entry;
    i = 8;
    while (i--) {
        rad = (3.141592653589793 * entry->angle) / 180.0;
        pos.x = lbl_1_data_15C.x + (lbl_1_data_1A8[0].x * cos(rad));
        pos.y = lbl_1_data_15C.y + entry->y;
        pos.z = lbl_1_data_15C.z + (lbl_1_data_1A8[0].z * sin(rad));
        Hu3DModelPosSetV(entry->model, &pos);
        Hu3DModelScaleSet(entry->model, entry->scale, entry->scale, entry->scale);
        entry++;
    }
    pos.x = lbl_1_data_15C.x + (lbl_1_data_1A8[0].x * cos(1.5707963267948966));
    pos.y = lbl_1_data_15C.y + lbl_1_data_168[0];
    pos.z = lbl_1_data_15C.z + (lbl_1_data_1A8[0].z * sin(1.5707963267948966));
    Hu3DModelPosSetV(lbl_1_bss_0->models[12], &pos);
    lbl_1_bss_0->bgmStream = HuAudBGMPlay(0x23);
    cam = lbl_1_bss_0->camWork;
    steps = 31;
    cam = lbl_1_bss_0->camWork;
    lbl_1_bss_0->camCount = CamMotionExPathGet(lbl_1_bss_0->models[13], 6, 48.0f, 0, &cam, &steps);
    cam = &lbl_1_bss_0->camWork[lbl_1_bss_0->camCount - 1];
    lbl_1_bss_4->pos = cam->pos;
    lbl_1_bss_4->up = cam->up;
    lbl_1_bss_4->target = cam->target;
    lbl_1_bss_4->fov = cam->fov;
    Hu3DCameraPosSetV(MGFREE_CAM, &lbl_1_bss_4->pos, &lbl_1_bss_4->up, &lbl_1_bss_4->target);
    Hu3DCameraPerspectiveSet(MGFREE_CAM, lbl_1_bss_4->fov, 20.0f, 60000.0f, 1.2f);
    fn_1_19F8(13);
    fn_1_6BC4(0, lbl_1_bss_4->sel[0], lbl_1_bss_4->sel[1], lbl_1_bss_4->sel[2]);
    fn_1_A0(60);
}

void fn_1_3C8C(void)
{
    u32 state;
    s32 mgNo;
    OMOVLHIS *his;

    state = 0;
    lbl_1_bss_0->selMg = -1;
    while (lbl_1_bss_0->selMg < 0) {
        HuPrcVSleep();
        if (fn_1_A544() == 0) {
            continue;
        }
        switch (state) {
            case 0:
                if (HuPadDStkRep[0] & PAD_BUTTON_RIGHT) {
                    fn_1_4D20(5);
                } else if (HuPadDStkRep[0] & PAD_BUTTON_LEFT) {
                    fn_1_4D20(4);
                } else if (HuPadDStkRep[0] & PAD_BUTTON_UP) {
                    fn_1_4D20(2);
                } else if (HuPadDStkRep[0] & PAD_BUTTON_DOWN) {
                    fn_1_4D20(3);
                }
                if (HuPadBtnDown[0] & PAD_TRIGGER_L) {
                    fn_1_944(-1);
                } else if (HuPadBtnDown[0] & PAD_TRIGGER_R) {
                    fn_1_944(1);
                } else if (HuPadBtnDown[0] & PAD_BUTTON_A) {
                    mgNo = fn_1_A4DC();
                    if (mgNo >= 0) {
                        fn_1_4D20(0x13);
                        switch (mgDataTbl[mgNo].type) {
                            case 0:
                            case 3:
                            case 4:
                            case 8:
                                state = 7;
                                break;
                            case 2:
                                HuAudFXPlay(1);
                                state = 1;
                                break;
                            case 1:
                                HuAudFXPlay(1);
                                state = 2;
                                break;
                            case 6:
                                HuAudFXPlay(1);
                                state = 3;
                                break;
                            case 5:
                                HuAudFXPlay(1);
                                state = 4;
                                break;
                        }
                    } else {
                        HuAudFXPlay(4);
                    }
                } else if (HuPadBtnDown[0] & PAD_BUTTON_B) {
                    state = 8;
                }
                break;
            case 1:
                if (HuPadBtnDown[0] & PAD_BUTTON_A) {
                    GwPlayerConf[fn_1_4D60(0)].grp = 0;
                    GwPlayerConf[fn_1_4D60(1)].grp = 0;
                    GwPlayerConf[fn_1_4D60(2)].grp = 1;
                    GwPlayerConf[fn_1_4D60(3)].grp = 1;
                    state = 7;
                } else if (HuPadBtnDown[0] & PAD_BUTTON_B) {
                    HuAudFXPlay(3);
                    state = 0;
                    fn_1_4D20(0x15);
                } else if (HuPadDStkRep[0] & PAD_BUTTON_RIGHT) {
                    fn_1_4D20(8);
                } else if (HuPadDStkRep[0] & PAD_BUTTON_LEFT) {
                    fn_1_4D20(7);
                }
                break;
            case 2:
                if (HuPadBtnDown[0] & PAD_BUTTON_A) {
                    GwPlayerConf[fn_1_4D60(0)].grp = 0;
                    GwPlayerConf[fn_1_4D60(1)].grp = 1;
                    GwPlayerConf[fn_1_4D60(2)].grp = 1;
                    GwPlayerConf[fn_1_4D60(3)].grp = 1;
                    if (fn_1_A4DC() + 0x1F5 == 0x207) {
                        HuAudFXPlay(1);
                        fn_1_4D20(0x14);
                        state = 5;
                    } else {
                        state = 7;
                    }
                } else if (HuPadBtnDown[0] & PAD_BUTTON_B) {
                    HuAudFXPlay(3);
                    state = 0;
                    fn_1_4D20(0x15);
                } else if (HuPadDStkRep[0] & PAD_BUTTON_RIGHT) {
                    fn_1_4D20(10);
                } else if (HuPadDStkRep[0] & PAD_BUTTON_LEFT) {
                    fn_1_4D20(9);
                }
                break;
            case 3:
                if (HuPadBtnDown[0] & PAD_BUTTON_A) {
                    GwPlayerConf[fn_1_4D60(0)].grp = 0;
                    GwPlayerConf[fn_1_4D60(1)].grp = 1;
                    GwPlayerConf[fn_1_4D60(2)].grp = 3;
                    GwPlayerConf[fn_1_4D60(3)].grp = 3;
                    if (fn_1_A4DC() + 0x1F5 == 0x228) {
                        HuAudFXPlay(1);
                        fn_1_4D20(0x14);
                        state = 6;
                    } else {
                        state = 7;
                    }
                } else if (HuPadBtnDown[0] & PAD_BUTTON_B) {
                    HuAudFXPlay(3);
                    state = 0;
                    fn_1_4D20(0x15);
                } else if (HuPadDStkRep[0] & PAD_BUTTON_RIGHT) {
                    fn_1_4D20(12);
                } else if (HuPadDStkRep[0] & PAD_BUTTON_LEFT) {
                    fn_1_4D20(11);
                }
                break;
            case 4:
                if (HuPadBtnDown[0] & PAD_BUTTON_A) {
                    s32 list4[4] = { 0, 1, 2, 3 };

                    GwPlayerConf[0].dif = list4[(s8)fn_1_4D4C()];
                    GwSystem.subGameNo = -1;
                    state = 7;
                } else if (HuPadBtnDown[0] & PAD_BUTTON_B) {
                    HuAudFXPlay(3);
                    state = 0;
                    fn_1_4D20(0x15);
                } else if (HuPadDStkRep[0] & PAD_BUTTON_RIGHT) {
                    fn_1_4D20(14);
                } else if (HuPadDStkRep[0] & PAD_BUTTON_LEFT) {
                    fn_1_4D20(13);
                }
                break;
            case 5:
                if (HuPadBtnDown[0] & PAD_BUTTON_A) {
                    GwSystem.subGameNo = (s8)fn_1_4D4C();
                    state = 7;
                } else if (HuPadBtnDown[0] & PAD_BUTTON_B) {
                    HuAudFXPlay(3);
                    fn_1_4D20(0x16);
                    state = 2;
                } else if (HuPadDStkRep[0] & PAD_BUTTON_UP) {
                    fn_1_4D20(15);
                } else if (HuPadDStkRep[0] & PAD_BUTTON_DOWN) {
                    fn_1_4D20(16);
                }
                break;
            case 6: {
                s32 list3[3] = { 0, 1, 2 };

                if (HuPadBtnDown[0] & PAD_BUTTON_A) {
                    mgSubMode = list3[(s8)fn_1_4D4C()];
                    state = 7;
                } else if (HuPadBtnDown[0] & PAD_BUTTON_B) {
                    HuAudFXPlay(3);
                    fn_1_4D20(0x16);
                    state = 3;
                } else if (HuPadDStkRep[0] & PAD_BUTTON_UP) {
                    fn_1_4D20(17);
                } else if (HuPadDStkRep[0] & PAD_BUTTON_DOWN) {
                    fn_1_4D20(18);
                }
                break;
            }
            case 7:
                if (fn_1_A4DC() >= 0) {
                    HuAudFXPlay(2);
                    lbl_1_bss_0->selMg = fn_1_A4DC();
                    his = omOvlHisGet(0);
                    omOvlHisChg(0, his->ovl, 3, his->stat);
                } else {
                    state = 0;
                }
                break;
            case 8:
                HuAudFXPlay(3);
                his = omOvlHisGet(0);
                omOvlHisChg(0, his->ovl, 4, his->stat);
                lbl_1_bss_0->selMg = 0;
                break;
        }
    }
    fn_1_6B80(&lbl_1_bss_4->sel[0], &lbl_1_bss_4->sel[1], &lbl_1_bss_4->sel[2]);
}

void fn_1_45A4(void)
{
    OMOVLHIS *his;

    fn_1_1568();
    HuARDirFree(0x970000);
    HuDataDirClose(0x970000);
    HuDataDirClose(0x270000);
    mgInstExitF = FALSE;
    mgPauseExitF = FALSE;
    mgSubMode = 0;
    GwSystem.mgInstF = 1;
    his = omOvlHisGet(0);
    omOvlHisChg(0, his->ovl, 0, his->stat);
    _ClearFlag(0x30003);
    _ClearFlag(0x30001);
    memset(lbl_1_bss_4, 0, 0x3C);
}

u32 fn_1_46AC(s32 idx)
{
    return mgDataTbl[lbl_1_bss_974->entry[idx].mgNo].nameMes;
}
