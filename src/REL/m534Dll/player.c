#include "REL/m534Dll.h"

static void fn_1_245C(OMOBJ *obj);
static void fn_1_2768(OMOBJ *obj);
static void fn_1_2968(OMOBJ *obj);
static void fn_1_2C50(OMOBJ *obj);
static void fn_1_2CB4(OMOBJ *obj);
static void fn_1_2CB8(OMOBJ *obj);
static void fn_1_2E4C(OMOBJ *obj);
static void fn_1_2EB8(OMOBJ *obj);
static void fn_1_2F88(OMOBJ *obj, u32 motion);
static s32 fn_1_3118(OMOBJ *obj);

typedef struct M534MotInfo_s {
    u16 index;   /* 0x00 */
    f32 shiftIn; /* 0x04 */
    f32 start;   /* 0x08 */
    f32 end;     /* 0x0C */
    u32 attr;    /* 0x10 */
} M534MOTINFO;

u32 lbl_1_data_100[4] = { 0x8C0000, 0x8C0006, 0x8C0044, 0x8C0045 };

M534MOTINFO lbl_1_data_110[4] = {
    { 0, 0.2f, 0.0f, -1.0f, 0x40000001 },
    { 1, 0.2f, 0.0f, -1.0f, 0 },
    { 2, 0.2f, 0.0f, -1.0f, 0x40000001 },
    { 3, 0.1f, 0.0f, -1.0f, 0 },
};

HuVecF lbl_1_data_160[4] = {
    { -257.24f, 0.0f, -40.0f },
    { -109.67f, 0.0f, -40.0f },
    { 109.67f, 0.0f, -40.0f },
    { 257.24f, 0.0f, -40.0f },
};

s16 lbl_1_bss_30;
OMOBJ *lbl_1_bss_34[5];

void fn_1_2290(HUPROCESS *objman)
{
    s32 count[2] = { 0, 0 };
    s32 g;
    s32 grp[4];
    s32 i;
    s32 base[2] = { 0, 2 };
    s32 bad;
    OMOBJ *obj;

    bad = 0;
    for (i = 0; i < 4; i++) {
        g = GwPlayerConf[i].grp;
        grp[i] = g;
#ifdef TARGET_PC
        if (g < 0 || g >= 2) {
#else
        if (g >= 2) {
#endif
            bad = 1;
        } else {
            count[g]++;
        }
    }
    if (count[0] != 2 || count[1] != 2) {
        bad = 1;
    }
    if (bad != 0) {
        for (i = 0; i < 4; i++) {
            grp[i] = i >> 1;
        }
    }
    omMakeGroupEx(objman, 0, 4);
    lbl_1_bss_30 = -1;
    for (i = 0; i < 4; i++) {
        obj = lbl_1_bss_34[i] = omAddObjEx(objman, 30, 1, 4, 0, fn_1_245C);
        obj->work[0] = i;
        g = grp[i];
        obj->work[1] = g;
        obj->work[2] = base[g];
        base[g]++;
    }
}

void fn_1_2438(void)
{
    CharModelKill(-1);
}

static void fn_1_245C(OMOBJ *obj)
{
    f32 speed[4] = { 6.5f, 5.8f, 4.8f, 4.0f };
    M534PLAYER *p;
    s32 mot;
    s32 i;
    u32 slot;
    u32 no;
    s32 charNo;
    char *hook;
    s16 model;

    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M534PLAYER), 0x10000000U);
    p = obj->data;
    memset(p, 0, sizeof(M534PLAYER));
    obj->stat |= 0x100;
    no = obj->work[0];
    p->no = no;
    p->team = obj->work[1];
    slot = obj->work[2];
    p->slot = slot;
    charNo = GwPlayerConf[no].charNo;
    p->charNo = charNo;
    fn_1_1800(no, p->team, p->charNo);
    p->speed = speed[GwPlayerConf[no].dif & 3];
    p->cpu = GwPlayerConf[no].type;
    p->wait = 2.0f + (f32) (s32) (frand() & 3);
    *obj->mdlId = CharModelCreate(charNo, 2);
    Hu3DModelLayerSet(*obj->mdlId, 4);
    Hu3DModelShadowSet(*obj->mdlId);
    for (i = 0; i < 4; i++) {
        mot = CharMotionCreate(charNo, lbl_1_data_100[i]);
        obj->mtnId[i] = mot;
    }
    CharMotionDataClose(charNo);
    p->motion = -1;
    fn_1_2F88(obj, 2);
    if (lbl_1_bss_30 < 0) {
        model = lbl_1_bss_30 = Hu3DModelCreate(HuDataSelHeapReadNum(0x5C0009, 0x10000000, HEAP_MODEL));
    } else {
        model = Hu3DModelLink(lbl_1_bss_30);
    }
    hook = CharModelItemHookGet(charNo, 2, 0);
    Hu3DModelHookSet(*obj->mdlId, hook, model);
    obj->trans.x = p->pos.x = lbl_1_data_160[slot].x;
    obj->trans.y = p->pos.y = lbl_1_data_160[slot].y;
    obj->trans.z = p->pos.z = lbl_1_data_160[slot].z;
    p->rotY = 0.0f;
    p->scaleY = p->scaleXZ = 1.0f;
    p->flag58 = 0;
    p->flag5C = 0;
    p->flag34 = 0;
    p->animSpeed = 1.0f;
    p->state = 0;
    obj->objFunc = fn_1_2768;
}

static void fn_1_2768(OMOBJ *obj)
{
    M534PLAYER *p = obj->data;
    char *hook;

    p->btnDown = 0;
    p->btn = 0;
    switch (p->state) {
        case 0:
            if (fn_1_16AC() == 2) {
                p->btnDown = 0x100;
                p->state++;
            case 1:
                if (fn_1_16C4() == 2) {
                    p->state++;
                    p->frames = 0;
            case 2:
                    if (fn_1_16C4() != 2) {
                        p->state++;
                        p->timer = 20;
                    } else if (p->cpu != 0) {
                        fn_1_2EB8(obj);
                    } else {
                        s32 no = p->no;

                        p->btn = HuPadBtn[GwPlayerConf[no].padNo];
                        p->btnDown = HuPadBtnDown[GwPlayerConf[no].padNo];
                    }
                }
            }
            break;
        case 3:
            if (--p->timer == 0) {
                p->state++;
                hook = CharModelItemHookGet(p->charNo, 2, 0);
                CharModelHookDustCreate(p->charNo, hook);
                fn_1_2F88(obj, 0);
            case 4:
                if (fn_1_16C4() == 3 && fn_1_17C8() != 0) {
                    p->state = 0;
                    obj->objFunc = fn_1_2968;
                }
            }
            break;
    }
    fn_1_2CB8(obj);
    fn_1_2CB4(obj);
    fn_1_2C50(obj);
    fn_1_2E4C(obj);
}

static void fn_1_2968(OMOBJ *obj)
{
    M534PLAYER *p = obj->data;
    f32 scale;
    f32 lim;

    p->btnDown = 0;
    p->btn = 0;
    switch (p->state) {
        case 0:
            fn_1_173C(p->no);
            p->state++;
            break;
        case 1:
            if (fn_1_16DC() != 0) {
                if (fn_1_170C(p->no) < 0) {
                    p->state = 4;
                    p->timer = 8;
                } else {
                    p->state++;
                }
            }
            break;
        case 2:
            if (fn_1_16DC() == 4) {
                fn_1_2F88(obj, 1);
                p->state++;
            }
            break;
        case 3:
            if (fn_1_3118(obj) != 0) {
                fn_1_176C(p->no);
            }
            break;
        case 4:
            scale = fn_1_4848();
            lim = 0.75f;
            if (scale >= lim) {
                scale = (1.0 - scale) * (1.0 / (1.0 - lim));
                if (p->flag5C == 0) {
                    CharFXPlayPan(p->charNo, 0x202, fn_1_189C(&p->pos));
                    p->flag5C = 1;
                }
                if (scale < 0.1 && p->flag58 == 0) {
                    scale = 0.1f;
                    p->flag58 = 1;
                    omVibrate(p->no, 0x28, 0xC, 0);
                    Hu3DModelAttrSet(*obj->mdlId, 0x40000002);
                    fn_1_176C(p->no);
                }
                if (scale < p->scaleY) {
                    p->scaleY = scale;
                }
                if (p->scaleY <= 0.1) {
                    if (--p->timer < 0) {
                        p->scaleXZ *= 0.8;
                    }
                }
                if (p->scaleXZ < 0.1) {
                    Hu3DModelAttrSet(*obj->mdlId, 1);
                } else {
                    scale = p->scaleY;
                    lim = p->scaleXZ;
                    Hu3DModelScaleSet(*obj->mdlId, lim, scale, lim);
                }
            }
            break;
    }
    fn_1_2CB8(obj);
    fn_1_2CB4(obj);
    fn_1_2C50(obj);
    fn_1_2E4C(obj);
}

static void fn_1_2C50(OMOBJ *obj)
{
    M534PLAYER *p = obj->data;

    p->counter++;
    p->frames++;
    CharModelVoicePanSet(p->charNo, fn_1_189C(&p->pos));
}

static void fn_1_2CB4(OMOBJ *obj)
{
}

static void fn_1_2CB8(OMOBJ *obj)
{
    M534PLAYER *p = obj->data;
    f32 sec;

    if ((p->btnDown & 0x100) != 0) {
        fn_1_449C(p->team);
        sec = 0.016666668f * (f32) p->frames;
        if (sec == 0.0f) {
            sec = 1.0f;
        }
        p->frames = 0;
        sec = 0.1666666753590107 / sec;
        if (sec < 1.0) {
            sec = 1.0f;
        }
        p->animSpeed = sec;
        p->motion = -1;
        fn_1_2F88(obj, 3);
        p->flag34 = 0;
    }
    if (p->motion == 3) {
        if (fn_1_3118(obj) != 0) {
            fn_1_2F88(obj, 2);
            return;
        }
        CharMotionSpeedSet(p->charNo, p->animSpeed);
        if (p->flag34 == 0) {
            if (CharMotionTimeGet(p->charNo) >= 4.0) {
                p->flag34 = 1;
                fn_1_43D4(p->slot);
                fn_1_4114(p->slot);
            }
        }
    }
}

static void fn_1_2E4C(OMOBJ *obj)
{
    M534PLAYER *p = obj->data;

    Hu3DModelPosSetV(*obj->mdlId, &p->pos);
    Hu3DModelRotSet(*obj->mdlId, 0.0f, p->rotY, 0.0f);
}

static void fn_1_2EB8(OMOBJ *obj)
{
    M534PLAYER *p = obj->data;
    if ((p->wait = p->wait - 1.0) <= 0.0f) {
        p->btnDown |= 0x100;
        p->wait = p->speed + 2.5 * (1.52587890625e-05 * (f32) (s32) (FRAND16() & 0xFFFF));
    }
}

static void fn_1_2F88(OMOBJ *obj, u32 motion)
{
    M534PLAYER *p = obj->data;
    f32 end;

    if ((u32) p->motion != motion && motion < 4) {
        if (p->motion < 0) {
            end = 0.0f;
        } else {
            end = 60.0f * lbl_1_data_110[motion].shiftIn;
        }
        p->motion = motion;
        CharMotionShiftSet(p->charNo, obj->mtnId[lbl_1_data_110[motion].index], 60.0f * lbl_1_data_110[motion].start, end, lbl_1_data_110[motion].attr);
        if (lbl_1_data_110[motion].end >= 0.0f) {
            Hu3DMotionShiftStartEndSet(*obj->mdlId, 60.0f * lbl_1_data_110[motion].start, 60.0f * lbl_1_data_110[motion].end);
        }
    }
}

static s32 fn_1_3118(OMOBJ *obj)
{
    M534PLAYER *p = obj->data;
    s32 ret = 0;

    if (CharMotionEndCheck(p->charNo) != 0 && CharMotionShiftIDGet(p->charNo) < 0) {
        ret = 1;
    }
    return ret;
}

s32 fn_1_318C(OMOBJ *obj, u32 motion)
{
    s32 ret = 0;

    if (fn_1_3118(obj)) {
        fn_1_2F88(obj, motion);
        ret = 1;
    }
    return ret;
}
