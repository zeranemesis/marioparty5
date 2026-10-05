#include "game/object.h"
#include "dolphin/mtx.h"

extern "C" {
    #include "game/window.h"

    // NONMATCHING: register allocation differs (uses r31 instead of r0 for local)
    void MdObjMessMain(OMOBJ *obj)
    {
        volatile void *data = obj->data;
        (void)data;
    }

    // NONMATCHING: clrlwi inserted for unsigned store after signed load
    void MdDebugFontUpdate(void *obj)
    {
        ((s16 *)obj)[0x1A / 2] = 0;
        ((s16 *)obj)[0x16 / 2] = ((s16 *)obj)[0x14 / 2];
    }

    void MdMathLerp(f32 *out, f32 start, f32 end, f32 t)
    {
        *out = start + t * (end - start);
    }

    void MdMathLerpOfs(f32 *out, f32 start, f32 end, f32 t, f32 ofs)
    {
        *out = start + (end - start) * (t + ofs);
    }

    // NONMATCHING: stack frame and register allocation differs
    void MdCameraPosSet(void *camera, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f)
    {
        ((f32 *)camera)[0x44 / 4] = a;
        ((f32 *)camera)[0x48 / 4] = b;
        ((f32 *)camera)[0x4C / 4] = c;
        ((f32 *)camera)[0x54 / 4] = d;
        ((f32 *)camera)[0x58 / 4] = e;
        ((f32 *)camera)[0x5C / 4] = f;
    }

    void MdMessKill(void)
    {
        HuWinAllKill();
    }
}