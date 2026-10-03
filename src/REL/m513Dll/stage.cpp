#include "REL/m513Dll.h"

/* The stage object: work list node, base class subobject and the three models
 * the minigame loads out of DATA_m513. */
typedef struct M513Stage_s {
    /* 0x00 */ M513_OBJHEAD head;
    /* 0x08 */ void **vtable;
    /* 0x0C */ u32 flags;
    /* 0x10 */ s16 modelId[3];
} M513Stage;

/* Strings from singleton.h, referenced by the two singleton assertions. */
extern "C" {
char lbl_1_data_760[] = "ms_pointer == 0";
char lbl_1_data_770[] = "singleton.h";
char lbl_1_data_77C[] = "ms_pointer != 0";
}

extern "C" {
void fn_1_23AC(M513Stage *, int);
void fn_1_22E0(M513Stage *);
void fn_1_1220C(M513Stage *);
void fn_1_12208(M513Stage *);

M513Stage *fn_1_10874(M513Stage *, int);
void fn_1_1084C(M513Stage *, int);
void fn_1_10848(M513Stage *);

void fn_1_1768(M513_OBJHEAD *);
void fn_1_1774(M513_OBJHEAD *, int);
void fn_1_1834(M513_OBJHEAD *, M513_OBJHEAD *);
void fn_1_3198(s16 *, u32);
void fn_1_35F8(s16 *, int);
void fn_1_C8(void *);

/* Base class vtable: [offset_to_top, typeinfo, dtor, vfunc, ..., pure, ...]. */
void *lbl_1_data_218[12] = {
    NULL, NULL, (void *) fn_1_23AC, NULL, (void *) fn_1_22E0, NULL,
    NULL, (void *) -1, (void *) fn_1_1220C, NULL, (void *) -1, (void *) fn_1_12208
};

/* Derived class vtable. */
void *lbl_1_data_78C[5] = {
    NULL, NULL, (void *) fn_1_10874, (void *) fn_1_1084C, (void *) fn_1_10848
};

/* Singleton pointer and the head of the process work list. */
M513_OBJHEAD *lbl_1_bss_A1C;
M513_OBJHEAD lbl_1_bss_C;
}

// NONMATCHING: singleton pointer cast
extern "C" M513Stage *fn_1_104B0(M513Stage *arg0)
{
    M513_OBJHEAD *ms;
    GXColor lightColor;
    HuVecF lightPos;
    HuVecF lightDir;
    HuVecF camTarget;
    HuVecF camPos;
    HuVecF camUp;

    fn_1_1768(&arg0->head);
    arg0->vtable = lbl_1_data_218;
    arg0->flags = 0;
    fn_1_1834(&arg0->head, &lbl_1_bss_C);
    if (lbl_1_bss_A1C != NULL) {
        fn_1_1D4("ms_pointer == 0", "singleton.h", 0x35);
    }
    ms = (M513_OBJHEAD *) ((u8 *) arg0 + 0x10);
    if (ms != NULL) {
        ms = (M513_OBJHEAD *) ((u8 *) ms - 0x10);
    }
    lbl_1_bss_A1C = ms;
    arg0->vtable = lbl_1_data_78C;

    fn_1_3198(&arg0->modelId[0], M513_HSF_stage);
    fn_1_3198(&arg0->modelId[1], M513_HSF_mame);
    fn_1_3198(&arg0->modelId[2], M513_HSF_mame_small);

    Hu3DBGColorSet(0x32, 0x32, 0x9c);
    {
        char name[6] = "light";
        Hu3DModelLightInfoSet(arg0->modelId[0], 1);
        Hu3DGLightParamGet(Hu3DModelLightIdGet(arg0->modelId[0], name), &lightPos, &lightDir, &lightColor);
    }
    Hu3DShadowMultiCreate(20.0f, 100.0f, 50000.0f, 1);
    Hu3DShadowTPLvlSet(0.4f);

    camTarget.x = 0.0f;
    camTarget.y = 0.0f;
    camTarget.z = 0.0f;
    PSVECScale(&lightDir, &camPos, -3000.0f);
    PSVECAdd(&camPos, &camTarget, &camPos);

    camUp.x = 0.0f;
    camUp.y = 1.0f;
    camUp.z = 0.0f;
    Hu3DShadowMultiPosSet(&camPos, &camUp, &camTarget, 1);

    Hu3DShadowMultiColSet(0, 0, 0x40, 1);
    Hu3DShadowMultiSizeSet(0x60, 1);
    Hu3DModelShadowMapSet(arg0->modelId[0]);
    Hu3DModelLayerSet(arg0->modelId[0], 1);
    Hu3DModelCameraSet(arg0->modelId[0], 1);
    Hu3DModelLayerSet(arg0->modelId[1], 1);
    Hu3DModelCameraSet(arg0->modelId[1], 1);
    Hu3DModelLayerSet(arg0->modelId[2], 3);
    Hu3DModelCameraSet(arg0->modelId[2], 1);
    Hu3DModelAttrSet(arg0->modelId[2], 0x40000001);
    return arg0;
}

// NONMATCHING: singleton pointer cast
extern "C" M513Stage *fn_1_107BC(M513Stage *arg0, int arg1)
{
    if (arg0 != NULL) {
        if (lbl_1_bss_A1C == NULL) {
            fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3d);
        }
        lbl_1_bss_A1C = (M513_OBJHEAD *) NULL;
        if (arg1 > 0) {
            fn_1_C8(arg0);
        }
    }
    return arg0;
}

extern "C" void fn_1_10848(M513Stage *arg0)
{
}

// NONMATCHING: dummy compare left over from a bounds assert
extern "C" void fn_1_1084C(M513Stage *arg0, int arg1)
{
    arg0->flags &= ~(1 << arg1);
    if (arg1 < 5) {
    }
}

// NONMATCHING: singleton pointer cast
extern "C" M513Stage *fn_1_10874(M513Stage *arg0, int arg1)
{
    if (arg0 != NULL) {
        arg0->vtable = lbl_1_data_78C;
        fn_1_35F8(&arg0->modelId[2], -1);
        fn_1_35F8(&arg0->modelId[1], -1);
        fn_1_35F8(&arg0->modelId[0], -1);
        if ((void *) ((u8 *) arg0 + 0x10) != NULL) {
            if (lbl_1_bss_A1C == NULL) {
                fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3d);
            }
            lbl_1_bss_A1C = (M513_OBJHEAD *) NULL;
        }
        if (arg0 != NULL) {
            arg0->vtable = lbl_1_data_218;
            if (arg0 != NULL) {
                fn_1_1774(&arg0->head, 0);
            }
        }
        if (arg1 > 0) {
            fn_1_C8(arg0);
        }
    }
    return arg0;
}