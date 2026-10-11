#include "types.h"

static const float lbl_1_rodata_158 = 1.5f;

extern void MdObjDebugFontMain(void *arg0);
extern void MdObjCameraMain(void *arg0);
extern void MdObjLightMain(void *arg0);
extern void MdCameraCreate(void *arg0);
extern void MdLightCreate(void *arg0);
extern u8 sysWork[];
extern u8 HuPadDStk[];

#pragma dont_inline on
void MdDebugFontCreate(void *arg0)
{
    *(s32 *)((u8 *)arg0 + 0) = 1;
    *(s32 *)((u8 *)arg0 + 4) = 0;
    *(s32 *)((u8 *)arg0 + 8) = 0xe;
    *(s16 *)((u8 *)arg0 + 0xc) = 0x10;
    *(s16 *)((u8 *)arg0 + 0xe) = 0x28;
    *(s16 *)((u8 *)arg0 + 0x10) = 0xd8;
    *(s16 *)((u8 *)arg0 + 0x12) = 0;
    *(s16 *)((u8 *)arg0 + 0x14) = 0xf;
    *(float *)((u8 *)arg0 + 0x18) = lbl_1_rodata_158;
    *(s16 *)((u8 *)arg0 + 0x16) = 0;
}
#pragma dont_inline reset

void MdDebugFontUpdate(void *arg0)
{
    *(s16 *)((u8 *)arg0 + 0x16) = 0;
    *(s16 *)((u8 *)arg0 + 0x12) = *(s16 *)((u8 *)arg0 + 0x10);
}

void MdObjDebugFontCreate(void *obj)
{
    // NONMATCHING: register allocation - the original keeps the object in r31 and
    // the work pointer in r30; MWCC swaps them (4 differing rows, no logic impact).
    void *work = *(void **)((u8 *)obj + 0x5c);

    MdDebugFontCreate(work);
    *(void (**)(void *))((u8 *)obj + 0x14) = MdObjDebugFontMain;
}

void MdObjCameraCreate(void *obj)
{
    void *work = *(void **)((u8 *)obj + 0x5c);

    MdCameraCreate(work);
    *(void (**)(void *))((u8 *)obj + 0x14) = MdObjCameraMain;
}

void MdObjLightCreate(void *obj)
{
    void *work = *(void **)((u8 *)obj + 0x5c);

    MdLightCreate(work);
    *(void (**)(void *))((u8 *)obj + 0x14) = MdObjLightMain;
}

void MdObjPadMain(void)
{
    // NONMATCHING: register allocation - the original caches &sysWork in r30 and
    // loads HuPadDStk[i] into r4 without a mask; MWCC reloads and masks differently.
    s32 i;

    for (i = 0; i < 4; i++) {
        *(u16 *)(sysWork + i * 2 + 0x140) = *(s16 *)(sysWork + i * 2 + 0x148) & ~HuPadDStk[i];
        *(u16 *)(sysWork + i * 2 + 0x148) = HuPadDStk[i];
    }
}
