#include "REL/m513Dll.h"
#include "game/sprite.h"

/* The "indicator" hook object. It owns a 640x480 RGB565 texture and copies the
 * frame buffer into it every hook callback, so the whole frame buffer is part
 * of the object: 0x60 + 640 * 480 * 2 == 0x96060. */
typedef struct M513IndHook_s {
    /* 0x000000 */ M513_OBJHEAD head;
    /* 0x000008 */ void **vtable;
    /* 0x00000C */ u32 flags;
    /* 0x000010 */ u8 pad10[0x20];
    /* 0x000030 */ GXTexObj texObj;
    /* 0x000050 */ u8 pad50[0x10];
    /* 0x000060 */ u16 frameBuf[0x280 * 0x1E0];
    /* 0x096060 */ f32 fade[3];
    /* 0x09606C */ ANIMDATA *sprAnim[32];
    /* 0x0960EC */ ANIMDATA *subAnim[4];
} M513IndHook;

extern "C" {
s16 Hu3DCameraNo;
HU3DCAMERA Hu3DCamera[];

static char lbl_1_data_378[] = "ms_pointer == 0";
static char lbl_1_data_388[] = "singleton.h";
static char lbl_1_data_394[] = "ms_pointer != 0";

M513IndHook *fn_1_B110(M513IndHook *, int);
void fn_1_B23C(M513IndHook *, int);
void fn_1_B254(M513IndHook *);
void fn_1_B2AC(M513IndHook *);
void fn_1_B458(M513IndHook *, Mtx);

void *lbl_1_data_3A4[5] = {
    NULL, NULL, (void *) fn_1_B110, (void *) fn_1_B23C, (void *) fn_1_B254
};

M513_OBJHEAD *lbl_1_bss_9F0;
}

extern "C" void fn_1_AE0C(HU3DMODEL *arg0, f32 (*mtx)[3][4])
{
    fn_1_B458((M513IndHook *) lbl_1_bss_9F0, (f32 (*)[4]) mtx);
}

// NONMATCHING: singleton pointer cast
extern "C" M513IndHook *fn_1_AE58(M513IndHook *arg0)
{
    M513_OBJHEAD *ms;
    s16 hookId;
    int i;

    fn_1_1768(&arg0->head);
    arg0->vtable = lbl_1_data_218;
    arg0->flags = 0;
    fn_1_1834(&arg0->head, &lbl_1_bss_C);
    if (lbl_1_bss_9F0 != NULL) {
        fn_1_1D4("ms_pointer == 0", "singleton.h", 0x35);
    }
    ms = (M513_OBJHEAD *) ((u8 *) arg0 + 0x10);
    if (ms != NULL) {
        ms = (M513_OBJHEAD *) ((u8 *) ms - 0x10);
    }
    lbl_1_bss_9F0 = ms;
    arg0->vtable = lbl_1_data_3A4;

    hookId = Hu3DHookFuncCreate(fn_1_AE0C);
    Hu3DModelLayerSet(hookId, 4);
    Hu3DModelCameraSet(hookId, 1);
    fn_1_B2AC(arg0);

    arg0->fade[0] = 0.0f;
    arg0->fade[1] = 0.0f;
    arg0->fade[2] = 0.0f;

    arg0->subAnim[0] = HuSprAnimDataRead(M513_ANM_ind_a);
    HuSprAnimLock(arg0->subAnim[0]);
    arg0->subAnim[1] = HuSprAnimDataRead(M513_ANM_ind_b);
    HuSprAnimLock(arg0->subAnim[1]);
    arg0->subAnim[2] = HuSprAnimDataRead(M513_ANM_ind_c);
    HuSprAnimLock(arg0->subAnim[2]);
    arg0->subAnim[3] = HuSprAnimDataRead(M513_ANM_ind_d);
    HuSprAnimLock(arg0->subAnim[3]);

    for (i = 0; i < 32; i++) {
        arg0->sprAnim[i] = HuSprAnimDataRead(M513_ANM_ind_num + i);
        HuSprAnimLock(arg0->sprAnim[i]);
    }
    return arg0;
}

// NONMATCHING: singleton pointer cast
extern "C" M513IndHook *fn_1_B084(M513IndHook *arg0, int arg1)
{
    if (arg0 != NULL) {
        if (lbl_1_bss_9F0 == NULL) {
            fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3d);
        }
        lbl_1_bss_9F0 = (M513_OBJHEAD *) NULL;
        if (arg1 > 0) {
            fn_1_C8(arg0);
        }
    }
    return arg0;
}

// NONMATCHING: singleton pointer cast
extern "C" M513IndHook *fn_1_B110(M513IndHook *arg0, int arg1)
{
    int i;

    if (arg0 != NULL) {
        arg0->vtable = lbl_1_data_3A4;
        HuSprAnimKill(arg0->subAnim[0]);
        HuSprAnimKill(arg0->subAnim[1]);
        HuSprAnimKill(arg0->subAnim[2]);
        HuSprAnimKill(arg0->subAnim[3]);
        for (i = 0; i < 32; i++) {
            HuSprAnimKill(arg0->sprAnim[i]);
        }
        if ((void *) ((u8 *) arg0 + 0x10) != NULL) {
            if (lbl_1_bss_9F0 == NULL) {
                fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3d);
            }
            lbl_1_bss_9F0 = (M513_OBJHEAD *) NULL;
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

extern "C" void fn_1_B23C(M513IndHook *arg0, int arg1)
{
    arg0->flags &= ~(1 << arg1);
}

extern "C" void fn_1_B254(M513IndHook *arg0)
{
    arg0->fade[0] += 0.003f;
    arg0->fade[1] += 0.003f;
    arg0->fade[2] += 0.003f;
}

extern "C" void fn_1_B2AC(M513IndHook *arg0)
{
    GXInitTexObj(&arg0->texObj, arg0->frameBuf, 0x280, 0x1E0, GX_TF_RGB565, GX_CLAMP, GX_CLAMP,
        GX_FALSE);
    GXInitTexObjLOD(&arg0->texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE,
        GX_ANISO_1);
}

extern "C" void fn_1_B338(M513IndHook *arg0)
{
    GXSetTexCopySrc(0, 0, 0x280, 0x1E0);
    GXSetTexCopyDst(0x280, 0x1E0, GX_TF_RGB565, GX_FALSE);
    GXCopyTex(arg0->frameBuf, GX_FALSE);
}

// NONMATCHING: frame size
extern "C" void fn_1_B390(const Mtx *mtx)
{
    Mtx lightMtx;
    Mtx texMtx;

    GXSetNumTexGens(1);
    C_MTXLightPerspective((f32 (*)[4]) lightMtx, Hu3DCamera[Hu3DCameraNo].fov, Hu3DCamera[Hu3DCameraNo].aspect,
        0.5f, -0.5f, 0.5f, 0.5f);
    PSMTXConcat((f32 (*)[4]) lightMtx, (f32 (*)[4]) mtx, (f32 (*)[4]) texMtx);
    GXLoadTexMtxImm(texMtx, 0x1E, GX_MTX3x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, 0x1E, GX_FALSE, 0x7D);
}

// NONMATCHING: frame size
extern "C" void fn_1_B458(M513IndHook *arg0, Mtx mtx)
{
    Mtx lightMtx;
    Mtx texMtx;

    GXLoadPosMtxImm(mtx, 0);
    GXSetNumTexGens(1);
    C_MTXLightPerspective((f32 (*)[4]) lightMtx, Hu3DCamera[Hu3DCameraNo].fov, Hu3DCamera[Hu3DCameraNo].aspect,
        0.5f, -0.5f, 0.5f, 0.5f);
    PSMTXConcat((f32 (*)[4]) lightMtx, (f32 (*)[4]) mtx, (f32 (*)[4]) texMtx);
    GXLoadTexMtxImm(texMtx, 0x1E, GX_MTX3x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, 0x1E, GX_FALSE, 0x7D);
    GXSetTexCopySrc(0, 0, 0x280, 0x1E0);
    GXSetTexCopyDst(0x280, 0x1E0, GX_TF_RGB565, GX_FALSE);
    GXCopyTex(arg0->frameBuf, GX_FALSE);
}