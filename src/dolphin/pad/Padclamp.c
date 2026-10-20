#include <dolphin/types.h>
#include <dolphin/pad.h>
#include <math.h>

#pragma fp_contract off

static const u8 ClampRegion[10] = {0x1E, 0xB4, 0x0F, 0x48, 0x28, 0x0F, 0x3B, 0x1F, 0x38, 0x2C};

static void ClampCircle(s8* px, s8* py, s8 outer, s8 dead)
{
    s32 ndz = -dead;
    s32 x = *px;
    s32 y = *py;
    s32 dx, dy, sum, len;

    if (x > ndz && x < dead) {
        dx = 0;
    } else if (x > 0) {
        dx = x - dead;
    } else {
        dx = x + dead;
    }

    if (y > ndz && y < dead) {
        dy = 0;
    } else if (y > 0) {
        dy = y - dead;
    } else {
        dy = y + dead;
    }

    sum = dx * dx + dy * dy;
    if (outer * outer < sum) {
        len = (s32)sqrtf((float)sum);
        dx = dx * outer / len;
        dy = dy * outer / len;
    }

    *px = dx;
    *py = dy;
}

void PADClampCircle(PADStatus* status)
{
    s32 i;
    PADStatus* s = status;
    const u8* region = ClampRegion;

    for (i = 0; i < 4; i++, s++) {
        if (s->err != 0) {
            continue;
        }

        ClampCircle(&s->stickX, &s->stickY, region[8], region[2]);
        ClampCircle(&s->substickX, &s->substickY, region[9], region[5]);

        {
            u8 hi = region[1];
            if (s->triggerL <= region[0]) {
                s->triggerL = 0;
            } else {
                if (hi < s->triggerL) {
                    s->triggerL = hi;
                }
                s->triggerL -= region[0];
            }

            if (s->triggerR <= region[0]) {
                s->triggerR = 0;
            } else {
                if (hi < s->triggerR) {
                    s->triggerR = hi;
                }
                s->triggerR -= region[0];
            }
        }
    }
}
