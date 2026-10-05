#include "dolphin/types.h"

extern "C" {

extern void Hu3DModelShadowMapSet(s16 modelId);
extern void Hu3DModelShadowDispOn(s16 modelId);
extern void Hu3DModelLayerSet(s16 modelId, s32 layer);
extern void fn_1_A8(void *obj, s16 flag);
extern void fn_1_6E0(void *obj);
extern void PSVECScale(f32 *vec, f32 scale);

// fn_1_CE20: copies 3 floats (0.0, -0.8660254, -0.5) to the destination
static const f32 lbl_1_rodata_258[3] = { 0.0f, -0.8660254f, -0.5f };
static const f64 lbl_1_rodata_2E8 = 0.0002777777777777778;  // 1/3600
static const f32 lbl_1_rodata_2F0 = 9.80665f;
static const f32 lbl_1_rodata_2F4 = 100.0f;

void fn_1_CE20(f32 *dest) {
    dest[0] = lbl_1_rodata_258[0];
    dest[1] = lbl_1_rodata_258[1];
    dest[2] = lbl_1_rodata_258[2];
}

// fn_1_D21C: enables shadow map, shadow display, and sets layer to 0 for a model
void fn_1_D21C(s16 modelId) {
    Hu3DModelShadowMapSet(modelId);
    Hu3DModelShadowDispOn(modelId);
    Hu3DModelLayerSet(modelId, 0);
}

// fn_1_D5A8: processes object with flag
void *fn_1_D5A8(void *obj, s16 flag) {
    if (obj != 0) {
        fn_1_A8(*(void **)obj, 1);
        if (flag > 0) {
            fn_1_6E0(obj);
        }
    }
    return obj;
}

// fn_1_CE54: scales lbl_1_rodata_258 by (1/3600) * 9.80665 * f1^2
void fn_1_CE54(f32 *vec, f32 f1) {
    f32 f2 = lbl_1_rodata_2F0;
    f32 f3 = lbl_1_rodata_2F4;
    f64 f4 = lbl_1_rodata_2E8;
    
    f32 t = f3 * f1;
    t = f2 * t;
    f64 t2 = f4 * t;
    f32 scale = (f32)t2;
    PSVECScale(vec, scale);
}

}