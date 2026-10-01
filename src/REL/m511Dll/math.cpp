#include "dolphin.h"

extern "C" {

f32 fn_1_884(f64 arg0, f64 arg1)
{
    s32 var_r28 = (s32)(182.04444444444445 * arg0);
    s32 var_r27 = (s32)(182.04444444444445 * arg1);
    u16 var_r26 = var_r28 - var_r27;

    return (f32)(0.0054931640625 * (f64)var_r26);
}

f32 fn_1_93C(f64 arg0, f64 arg1, f32 arg2)
{
    u16 var_r26;
    f32 var_f29;
    f32 var_f28;
    f32 var_f27;
    f32 var_f26;
    f32 var_f31;
    f32 var_f30;

    var_r26 = (s32)(182.04444444444445 * arg0) - (s32)(182.04444444444445 * arg1);
    var_f29 = (f32)(0.0054931640625 * (f64)var_r26);
    var_f28 = var_f29;
    var_f27 = var_f28;
    var_f26 = var_f27;
    var_f31 = var_f26;
    if (var_f31 < -arg2) {
        var_f30 = -arg2;
    }
    else if (arg2 < var_f31) {
        var_f30 = arg2;
    }
    else {
        var_f30 = var_f31;
    }
    return var_f30;
}

f32 fn_1_AA0(f32 arg0, f64 arg1, f32 arg2)
{
    f32 var_f31;
    f32 var_f30;

    var_f31 = (f32)(arg1 - 0.27240694444444447);
    var_f30 = arg2 - arg0;
    if (var_f30 > 0.0f) {
        var_f31 = (f32)((f64)var_f31 + (0.0002777777777777778 * (0.1 * (f64)var_f30)));
    }
    return var_f31;
}

}