#include "dolphin.h"
#include "game/hu3d.h"
#include "game/sprite.h"
#include "game/data.h"
#include "game/memory.h"

void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, s32 wrapS, s32 wrapT, s32 filter);

f32 lbl_1_bss_15F8;
ANIMDATA *lbl_1_bss_15FC;
ANIMDATA *lbl_1_bss_1600;
void *lbl_1_bss_1604;

f32 lbl_1_data_510[2][3] = {
    {0.03f, 0.0f, 0.02f},
    {0.0f, 0.04f, 0.02f},
};

void fn_1_11694(s16 layerNo);
void fn_1_1172C(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);

void fn_1_115A8(void *arg0)
{
    s32 i;
    u8 *base = arg0;

    Hu3DLayerHookSet(8, fn_1_11694);
    lbl_1_bss_1604 = HuMemDirectMallocNum(2, GXGetTexBufferSize(0x140, 0xF0, 4, 0, 0), 0x10000000);
    lbl_1_bss_1600 = HuSprAnimRead(HuDataSelHeapReadNum(0x6B0019, 0x10000000, 2));
    lbl_1_bss_15FC = HuSprAnimRead(HuDataSelHeapReadNum(0x21000C, 0x10000000, 2));
    for (i = 0; i < 10; i++) {
        Hu3DModelMatHookSet(*(s16 *)((u8 *)*(void **)(base + 0x58) + (i + 5) * 0x1C), fn_1_1172C);
    }
}

void fn_1_11694(s16 layerNo)
{
    if (Hu3DCameraBit & 2) {
        GXSetTexCopySrc(0, 0, 0x280, 0x1E0);
        GXSetTexCopyDst(0x140, 0xF0, 4, 1);
        GXCopyTex(lbl_1_bss_1604, 0);
        lbl_1_bss_15F8 = lbl_1_bss_15F8 - 0.002f;
    }
}

void fn_1_1172C(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    GXTexObj texObj;
    Mtx sp64;
    Mtx sp94;
    Mtx spC4;
    Mtx spF4;
    Mtx inv;
    GXColor color;

    color.r = 0x10;
    color.g = 0x10;
    color.b = 0x10;
    color.a = 0x00;

    GXInitTexObj(&texObj, lbl_1_bss_1604, 0x140, 0xF0, 4, 0, 0, 0);
    GXInitTexObjLOD(&texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(&texObj, 1);
    HuSprTexLoad(lbl_1_bss_1600, 0, 2, 1, 1, 1);
    HuSprTexLoad(lbl_1_bss_15FC, 0, 3, 1, 1, 1);
    GXSetNumTexGens(3);
    GXSetNumTevStages(3);

    C_MTXLightPerspective(sp64, Hu3DCamera[0].fov, 1.3333334f, 0.5f, -0.5f, 0.5f, 0.5f);
    PSMTXInverse(Hu3DCameraMtx, inv);
    PSMTXConcat(inv, *(Mtx *)((u8 *)drawObj + 0xC), spF4);
    PSMTXConcat(sp64, Hu3DCameraMtx, sp94);
    PSMTXConcat(sp94, spF4, sp94);
    GXLoadTexMtxImm(sp94, 0x21, GX_MTX3x4);
    GXSetTexCoordGen2(0, 0, 0, 0x21, 0, 0x7D);

    PSMTXTrans(spF4, 0.0f, lbl_1_bss_15F8, 0.0f);
    PSMTXScale(spC4, 0.8f, 0.8f, 1.0f);
    PSMTXConcat(spC4, spF4, sp94);
    GXLoadTexMtxImm(sp94, 0x1E, 1);
    GXSetTexCoordGen2(1, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(2, 1, 4, 0x3C, 0, 0x7D);

    GXSetTevColor(1, color);
    GXSetTevOrder(0, 0, 1, 0);
    GXSetTevColorIn(0, 8, 15, 15, 2);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(0, 7, 7, 7, 6);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);

    GXSetTevOrder(1, 2, 0, 0);
    GXSetTevColorIn(1, 0, 8, 2, 15);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(1, 7, 7, 7, 6);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);

    GXSetTevOrder(2, 1, 3, 0);
    GXSetTevColorIn(2, 15, 8, 3, 0);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(2, 7, 7, 7, 6);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);

    GXSetNumIndStages(1);
    GXSetIndTexOrder(0, 1, 2);
    GXSetIndTexCoordScale(0, 0, 0);
    GXSetTevIndWarp(0, 0, 1, 0, 1);
    GXSetIndTexMtx(1, lbl_1_data_510, -1);
}
