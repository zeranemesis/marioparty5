#include "types.h"

void MdDebugFontUpdate(void *arg0)
{
    *(s16 *)((u8 *)arg0 + 0x1a) = 0;
    *(s16 *)((u8 *)arg0 + 0x16) = *(s16 *)((u8 *)arg0 + 0x14);
}

void MdMathLerp(float *out, float a, float b, float t)
{
    *out = a + t * (b - a);
}

void MdMathLerpOfs(float *out, float a, float b, float t, float ofs)
{
    *out = a + (b - a) * (t + ofs);
}

typedef struct ResCam_s {
    u8 pad0[0x44];
    float posX;
    float posY;
    float posZ;
    u8 pad50[0x4];
    float upX;
    float upY;
    float upZ;
} ResCam;

void MdCameraPosSet(ResCam *arg0, float posX, float posY, float posZ, float upX, float upY, float upZ)
{
    arg0->posX = posX;
    arg0->posY = posY;
    arg0->posZ = posZ;
    arg0->upX = upX;
    arg0->upY = upY;
    arg0->upZ = upZ;
}

extern void HuWinAllKill(void);
extern void MdCameraUpdate(void *arg0);
extern void MdLightUpdate(void *arg0);
extern void MdMessMain(void *arg0);

static const float lbl_1_rodata_130 = 1.5f;

extern void *sysP;

void MdLightSetupModel(void)
{
    // NONMATCHING: the original keeps the model pointer in callee-saved r31/r30 and
    // passes sysP in r4 (an inlined call); MWCC keeps everything transient.
    void *model = *(void **)((u8 *)sysP + 0x70);
}

void MdMathClamp(float *val, float min, float max)
{
    if (*val <= min) {
        *val = min;
    }
    if (*val >= max) {
        *val = max;
    }
}

void MdMathLerpScale(float *out, float a, float b, float t)
{
    float v = a + t * (b - a);

    out[0] = v;
    out[1] = v;
    out[2] = v;
}

extern void MdLightUpdateData(void *arg0);
extern void HuWinHomeClear(s16 arg0);
extern void HuWinDispOff(s16 arg0);
extern void HuWinKill(s16 arg0);
extern void HuWinInit(s32 arg0);

void MdMessDlgInsertMesAdd(void *arg0, s32 val, s16 flag)
{
    s32 *state = (s32 *)((u8 *)arg0 + 0x14);

    ((s32 *)((u8 *)state + state[0] * 4 + 4))[0] = val;
    ((s16 *)((u8 *)state + state[0] * 2 + 0x44))[0] = flag;
    if (state[0] < 0x10) {
        state[0] += 1;
    }
}

void MdMessWinClose(void *arg0, s32 index)
{
    s16 *slot = (s16 *)((u8 *)arg0 + index * 0x10 + 0x78);

    if (slot[0] != -1) {
        HuWinHomeClear(slot[0]);
        HuWinDispOff(slot[0]);
        HuWinKill(slot[0]);
        slot[0] = -1;
    }
}

void MdMessWinHomeClear(void *arg0, s32 index)
{
    s16 *slot = (s16 *)((u8 *)arg0 + index * 0x10 + 0x78);

    if (slot[0] != -1) {
        HuWinHomeClear(slot[0]);
    }
}

static const float lbl_1_rodata_18C = 0.0f;

void MdMessCreate(void *arg0)
{
    // NONMATCHING: register allocation only - the original keeps arg0 in r30 and the
    // loop index in r29; MWCC swaps them (15 rows, no logic impact).
    s16 *slot;
    s32 i;

    *(s32 *)((u8 *)arg0 + 4) = 0;
    *(s16 *)((u8 *)arg0 + 8) = -1;
    *(s32 *)((u8 *)arg0 + 0) = 1;
    HuWinInit(0);
    slot = (s16 *)((u8 *)arg0 + 0x78);
    for (i = 0; i < 0x10; i++) {
        slot[0] = -1;
        *(float *)((u8 *)slot + 4) = lbl_1_rodata_18C;
        ((float *)((u8 *)slot + 4))[1] = lbl_1_rodata_18C;
        slot = (s16 *)((u8 *)slot + 0x10);
    }
}

static const float lbl_1_rodata_1E0 = 180.0f;
static const float lbl_1_rodata_1E4 = -180.0f;

void MdMathAngleClamp(float *angle)
{
    if (*angle > lbl_1_rodata_1E0) {
        *angle = lbl_1_rodata_1E4 + (*angle - lbl_1_rodata_1E0);
    } else if (*angle < lbl_1_rodata_1E4) {
        *angle = lbl_1_rodata_1E0 + *angle + lbl_1_rodata_1E0;
    }
}

void MdLightUpdate(void *arg0)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        MdLightUpdateData((u8 *)arg0 + i * 0x54 + 0xc);
    }
}

void MdDebugFontCreate(void *arg0)
{
    *(s32 *)((u8 *)arg0 + 0) = 1;
    *(s32 *)((u8 *)arg0 + 4) = 1;
    *(s32 *)((u8 *)arg0 + 8) = 0;
    *(s32 *)((u8 *)arg0 + 0xc) = 0xe;
    *(s16 *)((u8 *)arg0 + 0x10) = 0x10;
    *(s16 *)((u8 *)arg0 + 0x12) = 0x28;
    *(s16 *)((u8 *)arg0 + 0x14) = 0xd8;
    *(s16 *)((u8 *)arg0 + 0x16) = 0;
    *(s16 *)((u8 *)arg0 + 0x18) = 0xf;
    *(float *)((u8 *)arg0 + 0x1c) = lbl_1_rodata_130;
    *(s16 *)((u8 *)arg0 + 0x1a) = 0;
}

void MdMessKill(void)
{
    HuWinAllKill();
}

void MdObjCameraMain(void *obj)
{
    // NONMATCHING: the original keeps the object in r30 and the work pointer in r31
    // via "mr r30, r3"; MWCC keeps obj transient and passes the load directly in r3.
    MdCameraUpdate(*(void **)((u8 *)obj + 0x5c));
}

void MdObjLightMain(void *obj)
{
    // NONMATCHING: register allocation - see MdObjCameraMain.
    MdLightUpdate(*(void **)((u8 *)obj + 0x5c));
}

void MdObjMessMain(void *obj)
{
    // NONMATCHING: register allocation - see MdObjCameraMain.
    MdMessMain(*(void **)((u8 *)obj + 0x5c));
}

void MdMessDlgInsertMesReset(void *arg0)
{
    s32 *p = (s32 *)((u8 *)arg0 + 0x14);

    *p = 0;
}

extern void MdDebugFontPrint(const char *arg0, ...);
extern u16 HuPadBtnDown;
extern char *rescard_ctrlStr[];

static const char lbl_1_data_DF0[] = "";
static const char lbl_1_data_DF9[] = "";
static const char lbl_1_data_E08[] = "";

void MdObjDebugFontMain(void *obj)
{
    // NONMATCHING: MWCC spills obj to the stack and uses a -0x20 frame; the original
    // keeps it in r30 and needs only -0x10. The pad logic and prints are identical.
    void *work = *(void **)((u8 *)obj + 0x5c);
    s32 *param = (s32 *)work;

    MdDebugFontUpdate(work);
    if (HuPadBtnDown & 1) {
        param[1] -= 1;
        if (param[1] == -1) {
            param[1] = 3;
        }
    }
    if (HuPadBtnDown & 2) {
        param[1] += 1;
        param[1] %= 4;
    }
    MdDebugFontPrint(lbl_1_data_DF0, rescard_ctrlStr[param[1]]);
    MdDebugFontPrint(lbl_1_data_DF9);
    MdDebugFontPrint(lbl_1_data_E08);
}extern void MdObjCameraMain(void *arg0);
extern void MdLightCreate(void *arg0);
extern void MdObjLightMain(void *arg0);
extern void MdMessCreate(void *arg0);
extern void MdObjMessMain(void *arg0);

void MdObjCameraCreate(void *obj)
{
    // NONMATCHING: register allocation - the original keeps the object in r31 and
    // the work pointer in r30; MWCC swaps them.
    void *work = *(void **)((u8 *)obj + 0x5c);

    MdCameraCreate(work);
    *(void (**)(void *))((u8 *)obj + 0x14) = MdObjCameraMain;
}

void MdObjLightCreate(void *obj)
{
    // NONMATCHING: register allocation - the original keeps the object in r31 and
    // the work pointer in r30; MWCC swaps them.
    void *work = *(void **)((u8 *)obj + 0x5c);

    MdLightCreate(work);
    *(void (**)(void *))((u8 *)obj + 0x14) = MdObjLightMain;
}

void MdObjMessCreate(void *obj)
{
    // NONMATCHING: register allocation - the original keeps the object in r31 and
    // the work pointer in r30; MWCC swaps them.
    void *work = *(void **)((u8 *)obj + 0x5c);

    MdMessCreate(work);
    *(void (**)(void *))((u8 *)obj + 0x14) = MdObjMessMain;
}
