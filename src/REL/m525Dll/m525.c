#include "REL/m525Dll.h"

#include "math.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

int rand8(void);

void ObjectSetup(void);

int _prolog(void)
{
    const VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

/* Debug camera: moves the two cameras with the first pad. */
void fn_1_A0(void)
{
    Vec pos;
    Vec target;
    Vec up;
    float rotX;
    float rotY;
    float rotZ;
    s32 i;

    for (i = 0; i < 2; i++) {
        CenterM[i].x += 25.0 * (((HuPadBtn[0] & PAD_BUTTON_RIGHT) != 0) - ((HuPadBtn[0] & PAD_BUTTON_LEFT) != 0));
        CenterM[i].y += 25.0 * (((HuPadBtn[0] & PAD_BUTTON_UP) != 0) - ((HuPadBtn[0] & PAD_BUTTON_DOWN) != 0));
        CZoomM[i] += 25.0 * (((HuPadBtn[0] & PAD_BUTTON_X) != 0) - ((HuPadBtn[0] & PAD_BUTTON_Y) != 0));
        CRotM[i].y += HuPadSubStkX[0] / 32.0;
        CRotM[i].x += HuPadSubStkY[0] / -32.0;
        rotX = CRotM[i].x;
        rotY = CRotM[i].y;
        rotZ = CRotM[i].z;
        pos.x = CenterM[i].x + (CZoomM[i] * (sin((M_PI * rotY) / 180.0) * cos((M_PI * rotX) / 180.0)));
        pos.y = CenterM[i].y + (CZoomM[i] * -sin((M_PI * rotX) / 180.0));
        pos.z = CenterM[i].z + (CZoomM[i] * (cos((M_PI * rotY) / 180.0) * cos((M_PI * rotX) / 180.0)));
        target.x = CenterM[i].x;
        target.y = CenterM[i].y;
        target.z = CenterM[i].z;
        up.x = sin((M_PI * rotY) / 180.0) * sin((M_PI * rotX) / 180.0);
        up.y = cos((M_PI * rotX) / 180.0);
        up.z = cos((M_PI * rotY) / 180.0) * sin((M_PI * rotX) / 180.0);
    }
}

void ObjectSetup(void)
{
}
