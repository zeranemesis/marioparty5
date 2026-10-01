#include "dolphin/gx.h"
#include "dolphin/mtx.h"

typedef s16 HU3DMODELID;

// Opaque except for hookData, which the hook receives back as the CDebugLine.
struct HU3DMODEL_s {
    u8 m_unk000[0x120];
    void *m_hookData;
    u8 m_unk124[0x134 - 0x124];
};

extern HU3DMODEL_s Hu3DData[];

extern "C" {
typedef void (*HU3DMODELHOOK)(HU3DMODEL_s *modelP, Mtx *mtx);

HU3DMODELID Hu3DHookFuncCreate(HU3DMODELHOOK hookFunc);
void Hu3DModelKill(HU3DMODELID modelId);
void Hu3DModelLayerSet(HU3DMODELID modelId, s16 layerNo);
void Hu3DModelCameraSet(HU3DMODELID modelId, u16 cameraBit);

// A screen-space line: both endpoints plus a packed RGBA, cleared on create.
class CDebugLine {
public:
    s16 m_modelId;
    f32 m_x0;
    f32 m_y0;
    f32 m_z0;
    f32 m_x1;
    f32 m_y1;
    f32 m_z1;
    u8 m_r;
    u8 m_g;
    u8 m_b;
    u8 m_a;
};

void fn_1_AC74(CDebugLine *line, Mtx *mtx);

void fn_1_AADC(HU3DMODEL_s *model, Mtx *mtx)
{
    CDebugLine *line = (CDebugLine *)model->m_hookData;
    fn_1_AC74(line, mtx);
}

CDebugLine *fn_1_AB20(CDebugLine *line)
{
    HU3DMODELID modelId;
    HU3DMODEL_s *model;

    modelId = Hu3DHookFuncCreate(fn_1_AADC);
    model = &Hu3DData[modelId];
    model->m_hookData = line;
    Hu3DModelLayerSet(modelId, 5);
    Hu3DModelCameraSet(modelId, 1);
    line->m_modelId = modelId;
    line->m_x0 = 0.0f;
    line->m_y0 = 0.0f;
    line->m_z0 = 0.0f;
    line->m_x1 = 0.0f;
    line->m_y1 = 0.0f;
    line->m_z1 = 0.0f;
    line->m_r = 0xFF;
    line->m_g = 0xFF;
    line->m_b = 0xFF;
    line->m_a = 0xFF;
    return line;
}

CDebugLine *fn_1_AC20(CDebugLine *line, s16 freeIt)
{
    if (line != 0) {
        Hu3DModelKill(line->m_modelId);
        if (freeIt > 0) {
            delete line;
        }
    }
    return line;
}

void fn_1_AC74(CDebugLine *line, Mtx *mtx)
{
    GXLoadPosMtxImm((f32 (*)[4])mtx, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_RGBA6, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 1, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
    GXSetAlphaUpdate(GX_TRUE);
    f32 *pos0 = (f32 *)line + 1;
    f32 *pos1 = (f32 *)line + 4;
    u8 *color = (u8 *)line + 0x1C;
    GXBegin(GX_LINESTRIP, GX_VTXFMT0, 2);
    GXPosition3f32(pos0[0], pos0[1], pos0[2]);
    GXColor4u8(color[0], color[1], color[2], color[3]);
    GXPosition3f32(pos1[0], pos1[1], pos0[2]);
    GXColor4u8(color[0], color[1], color[2], color[3]);
}
}
