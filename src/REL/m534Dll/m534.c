#include "REL/m534Dll.h"

extern void (*_ctors[])(void);
extern void (*_dtors[])(void);

static void ObjectSetup(void);
static void fn_1_36C(OMOBJ *obj);
static void fn_1_648(OMOBJ *obj);
static void fn_1_924(OMOBJ *obj);
static void fn_1_C1C(OMOBJ *obj);
static void fn_1_E28(OMOBJ *obj);
static void fn_1_10F0(OMOBJ *obj);
static void fn_1_1364(OMOBJ *obj);

Vec lbl_1_data_0 = { 100.0f, 800.0f, -100.0f };
Vec lbl_1_data_C = { 0.3f, -0.8f, 0.3f };
Vec lbl_1_data_18 = { 40.0f, 45.0f, 1000.0f };
GXColor lbl_1_data_24 = { 0xFF, 0xFF, 0xFF, 0xFF };
Vec lbl_1_data_28 = { 200.0f, 3000.0f, 100.0f };
Vec lbl_1_data_34 = { 0.0f, 1.0f, 0.0f };
Vec lbl_1_data_40 = { 0.0f, 0.0f, 0.0f };

MGTIMER *lbl_1_bss_0;
s16 lbl_1_bss_4;
s16 lbl_1_bss_6;
s32 lbl_1_bss_8;
s16 lbl_1_bss_C;
s32 lbl_1_bss_10;
s32 lbl_1_bss_14;
OMOBJ *lbl_1_bss_18;
OMOBJ *lbl_1_bss_1C;
HUPROCESS *lbl_1_bss_20;

s32 _prolog(void)
{
    void (**ctor)(void) = _ctors;

    while (*ctor != 0) {
        (*ctor)();
        ctor++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    void (**dtor)(void) = _dtors;

    while (*dtor != 0) {
        (*dtor)();
        dtor++;
    }
}

static void ObjectSetup(void)
{
    Vec zero = { 0.0f, 0.0f, 0.0f };
    Vec back = { 0.0f, 0.0f, -1.0f };
    Vec aim;
    Vec pos;
    Mtx mtx;
    HUPROCESS *objman;

    lbl_1_bss_8 = -1;
    lbl_1_bss_0 = NULL;
    lbl_1_bss_C = Hu3DGLightCreateV(&lbl_1_data_0, &lbl_1_data_C, &lbl_1_data_24);
    Hu3DGLightInfinitytSet(lbl_1_bss_C);
    Hu3DGLightStaticSet(lbl_1_bss_C, 1);
    aim.x = aim.y = aim.z = 0.0f;
    pos.x = pos.z = 0.0f;
    pos.y = lbl_1_data_18.z;
    mtxRot(mtx, lbl_1_data_18.x, lbl_1_data_18.y, 0.0f);
    PSMTXMultVec(mtx, &pos, &pos);
    Hu3DGLightPosAimSetV(lbl_1_bss_C, &pos, &aim);
    Hu3DShadowCreate(80.0f, 20.0f, 10000.0f);
    Hu3DShadowPosSet(&lbl_1_data_28, &lbl_1_data_34, &lbl_1_data_40);
    Hu3DShadowColSet(0, 0, 0x40);
    objman = omInitObjMan(0x32, 0x2000);
    lbl_1_bss_20 = objman;
    omGameSysInit(objman);
    lbl_1_bss_14 = 0;
    lbl_1_bss_10 = 0;
    lbl_1_bss_6 = lbl_1_bss_4 = -1;
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 45.0f, 10.0f, 20000.0f, 1.2f);
    lbl_1_bss_18 = omAddObjEx(objman, 0x7FDA, 0, 0, -1, omOutView);
    lbl_1_bss_1C = omAddObjEx(objman, 0xA, 0, 0, -1, fn_1_36C);
    fn_1_6BDC(objman, &lbl_1_bss_1C);
    fn_1_6314(objman);
    fn_1_3360(objman);
    fn_1_2290(objman);
}

static void fn_1_36C(OMOBJ *obj)
{
    M534MAIN *m;

    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M534MAIN), 0x10000000U);
    m = obj->data;
    memset(m, 0, sizeof(M534MAIN));
    m->state = 0;
    m->timer4 = 0;
    m->camPhase = 0;
    m->camTimer = 0;
    m->phase = 0;
    m->timer18 = 0;
    m->endPhase = 0;
    m->endTimer = 0;
    m->busy = 0;
    m->mode = 3;
    m->unk44[0] = m->unk44[1] = m->unk44[2] = m->unk44[3] = -1;
    m->flags58 = 0;
    m->flags5C = 0;
    obj->objFunc = fn_1_648;
}

void fn_1_444(OMOBJ *obj)
{
    M534MAIN *m = obj->data;

    m->timer4++;
    m->camTimer++;
    m->timer18++;
    m->endTimer++;
    if (omSysExitReq != 0 && (u32) m->state != 5) {
        m->state = 4;
        if (lbl_1_bss_8 != -1) {
            HuAudSStreamFadeOut(lbl_1_bss_8, 1000);
            lbl_1_bss_8 = -1;
        }
        HuAudFadeOut(1000);
        fn_1_518(obj);
    }
}

void fn_1_518(OMOBJ *obj)
{
    M534MAIN *m = obj->data;

    if (m->busy == 0) {
        switch (m->state) {
            case 0:
                WipeCreate(1, 5, 60);
                m->state = 1;
                m->camTimer = 0;
                obj->objFunc = fn_1_19E8(lbl_1_bss_20);
                break;
            case 1:
                m->state = 2;
                m->timer18 = 0;
                obj->objFunc = fn_1_E28;
                break;
            case 2:
                m->state = 3;
                m->endTimer = 0;
                obj->objFunc = fn_1_1DF4(lbl_1_bss_20, obj);
                break;
            case 3:
            case 4:
                m->busy = 1;
                m->state = 5;
                obj->objFunc = fn_1_924;
                break;
            case 5:
            default:
                m->state = 5;
                obj->objFunc = fn_1_C1C;
                break;
        }
        m->timer4 = 0;
    }
}

static void fn_1_648(OMOBJ *obj)
{
    fn_1_444(obj);
    fn_1_518(obj);
}

static void fn_1_924(OMOBJ *obj)
{
    M534MAIN *m = obj->data;

    fn_1_444(obj);
    if (WipeCheckEnd() == 0) {
        WipeCreate(2, 6, 60);
        m->busy = 0;
        fn_1_518(obj);
    }
}

static void fn_1_C1C(OMOBJ *obj)
{
    fn_1_444(obj);
    if (WipeCheckEnd() == 0) {
        fn_1_33E8();
        fn_1_2438();
        fn_1_6338();
        fn_1_6C74();
        GameMesClose();
        HuAudFadeOut(1);
        omOvlReturnEx(1, 1);
    }
}

static void fn_1_E28(OMOBJ *obj)
{
    M534MAIN *m = obj->data;

    fn_1_444(obj);
    switch (m->phase) {
        case 0:
            lbl_1_bss_6 = GameMesCreate(2, 0);
            lbl_1_bss_8 = HuAudBGMPlay(0x37);
            m->phase = 1;
            m->timer18 = 0;
            break;
        case 1:
            if (GameMesStatGet(lbl_1_bss_6) == 0 && m->busy == 0) {
                lbl_1_bss_0 = MgTimerCreate(0);
                MgTimerParamSet(lbl_1_bss_0, 600, 0, 0);
                MgTimerModeOnSet(lbl_1_bss_0, 1);
                m->phase = 2;
                m->timer18 = 0;
                obj->objFunc = fn_1_10F0;
            }
            break;
    }
}

static void fn_1_10F0(OMOBJ *obj)
{
    M534MAIN *m = obj->data;
    s32 done = 0;

    fn_1_444(obj);
    if (MgTimerDoneCheck(lbl_1_bss_0) != 0) {
        done = 1;
    }
    if (done != 0) {
        lbl_1_bss_4 = GameMesCreate(2, 1);
        if (lbl_1_bss_8 != -1) {
            HuAudSStreamFadeOut(lbl_1_bss_8, 100);
            lbl_1_bss_8 = -1;
        }
        m->phase = 3;
        m->timer18 = 0;
        if (m->busy == 0) {
            obj->objFunc = fn_1_1364;
        }
    }
}

static void fn_1_1364(OMOBJ *obj)
{
    M534MAIN *m = obj->data;

    if (lbl_1_bss_0 != NULL) {
        MgTimerKill(lbl_1_bss_0);
        lbl_1_bss_0 = NULL;
    }
    fn_1_444(obj);
    if (fn_1_17C8() != 0 && m->flags58 == 0x1111) {
        m->phase = 4;
        m->timer18 = 0;
        fn_1_518(obj);
    }
}

s32 fn_1_1694(void)
{
    return ((M534MAIN *) lbl_1_bss_1C->data)->state;
}

u32 fn_1_16AC(void)
{
    return ((M534MAIN *) lbl_1_bss_1C->data)->camPhase;
}

u32 fn_1_16C4(void)
{
    return ((M534MAIN *) lbl_1_bss_1C->data)->phase;
}

u32 fn_1_16DC(void)
{
    return ((M534MAIN *) lbl_1_bss_1C->data)->endPhase;
}

void fn_1_16F4(s32 arg)
{
    ((M534MAIN *) lbl_1_bss_1C->data)->mode = arg;
}

s32 fn_1_170C(s32 no)
{
    M534MAIN *m = lbl_1_bss_1C->data;
#ifdef TARGET_PC
    no &= 3;
#endif
    return m->unk44[no];
}

void fn_1_173C(s32 no)
{
    ((M534MAIN *) lbl_1_bss_1C->data)->flags58 |= 1 << ((no & 3) * 4);
}

void fn_1_176C(s32 no)
{
    ((M534MAIN *) lbl_1_bss_1C->data)->flags5C |= 1 << ((no & 3) * 4);
}

s32 fn_1_179C(void)
{
    return lbl_1_bss_4 != -1;
}

s32 fn_1_17C8(void)
{
    return GameMesStatGet(lbl_1_bss_4) == 0;
}

void fn_1_1800(s32 no, s32 a, s32 b)
{
    M534MAIN *m = lbl_1_bss_1C->data;
    no &= 3;
    m->unk24[no] = a;
    m->unk34[no] = b;
}
