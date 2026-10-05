#include "dolphin/types.h"

typedef struct MdDebugFont {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s16 unk_10;
    s16 unk_12;
    s16 unk_14;
    s16 unk_16;
    s16 unk_18;
    s16 unk_1A;
    f32 unk_1C;
} MdDebugFont;

void MdDebugFontUpdate(MdDebugFont* font) {
    font->unk_1A = 0;
    font->unk_16 = font->unk_14;
}

void MdMathLerp(f32* out, f32 start, f32 end, f32 t) {
    *out = start + t * (end - start);
}

void MdMathLerpOfs(f32* out, f32 start, f32 end, f32 t1, f32 t2) {
    *out = start + (end - start) * (t1 + t2);
}

typedef struct MdCamera {
    u8 pad[0x44];
    f32 unk_44;
    f32 unk_48;
    f32 unk_4C;
    u8 pad2[0x4];
    f32 unk_54;
    f32 unk_58;
    f32 unk_5C;
} MdCamera;

void MdCameraPosSet(MdCamera* cam, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f) {
    cam->unk_44 = a;
    cam->unk_48 = b;
    cam->unk_4C = c;
    cam->unk_54 = d;
    cam->unk_58 = e;
    cam->unk_5C = f;
}

extern void HuWinAllKill(void);

void MdMessKill(void) {
    HuWinAllKill();
}

void MdMathLerpColor(u8* out, f32 start, f32 end, f32 t) {
    *out = (u8)(start + t * (end - start));
}