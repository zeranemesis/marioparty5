#include "game/object.h"
#include "game/window.h"
#include "game/hu3d.h"
#include "humath.h"

typedef struct MdDebugFontWork_s {
    int createF;
    int ctrl;
    int printF;
    int color;
    short posX;
    short posY;
    short ofsY;
    short rowOfsY;
    short rowH;
    short maxLen;
    float size;
} MDDEBUGFONT_WORK;

typedef struct MdCamera_s {
    short bit;
    int focusType;
    HuVecF pos;
    HuVecF target;
    HuVecF cross;
    HuVecF up;
    HuVecF dir;
    HuVecF hvr;
    float unk50;
    HuVecF center;
    HuVecF unk60[3];
    float fovy;
} MDCAMERA;

extern void HuWinAllKill(void);

void MdDebugFontUpdate(MDDEBUGFONT_WORK *debugFont)
{
    debugFont->maxLen = 0;
    debugFont->rowOfsY = debugFont->ofsY;
}

void MdCameraPosSet(MDCAMERA *camera, float h, float v, float r, float cx, float cy, float cz)
{
    camera->hvr.x = h;
    camera->hvr.y = v;
    camera->hvr.z = r;
    camera->center.x = cx;
    camera->center.y = cy;
    camera->center.z = cz;
}

void MdMathLerp(float *out, float a, float b, float t)
{
    *out = a + (b - a) * t;
}

void MdMathLerpColor(unsigned char *out, float a, float b, float t)
{
    float val = a + (b - a) * t;
    *out = (unsigned char)val;
}
// NONMATCHING: stack frame size and f31 usage differs (paired singles)

void MdMathLerpOfs(float *out, float a, float b, float t, float ofs)
{
    *out = a + (b - a) * (t + ofs);
}

void MdMessKill(void *mess)
{
    HuWinAllKill();
}