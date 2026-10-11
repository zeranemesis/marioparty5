#include "types.h"
#include "game/hu3d.h"

extern void fn_1_1F00(void);
extern void fn_1_1C0(void);
extern void fn_1_808(void);
extern void fn_1_84C(s16 arg);
extern void fn_1_914(void);
extern void fn_1_958(s16 arg);
extern void fn_1_990(void);
extern void fn_1_9D4(void);
extern void fn_1_9F8(void);
extern void fn_1_A2C(void);
extern void fn_1_A30(void);
extern void fn_1_2118(void);
extern void fn_1_22CC(void);
extern void fn_1_240C(void);
extern void fn_1_2654(void);
extern void fn_1_2800(void);
extern void fn_1_2870(void);
extern void fn_1_2C74(void);
extern void fn_1_2804(void);
extern void fn_1_31C8(void);
extern void fn_1_53B0(void);
extern void MgSeqModeNext(void);
extern void MgSeqModeDelaySet(s16 arg0, s32 arg1);
extern s32 WipeCheckEnd(void);
extern s32 omCameraViewCheck(s32 arg0);
extern void omAddObjEx(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, void *arg5);
extern void Hu3DModelAttrReset(s16 arg0, u32 arg1);
extern s32 lbl_1_bss_0;

typedef struct {
    void *field0;              // 0x0
    u8 pad4[0x3c - 0x4];
    void *field3c;             // 0x3c
    u8 pad40[0x100 - 0x40];
    s32 mode;                  // 0x100
    s32 field104;              // 0x104
} M554WORK;
extern M554WORK lbl_1_bss_B1C;

typedef struct {
    s16 field0;                // 0x0
    u8 pad2[0x2];
    f32 field4;                // 0x4
    f32 field8;                // 0x8
    f32 fieldC;                // 0xC
    f32 field10;               // 0x10
    f32 field14;               // 0x14
    f32 field18;               // 0x18
} M554ENTRY;                   // 0x1c bytes

void *lbl_1_data_180[3] = {
    (void *)fn_1_1F00, (void *)fn_1_2118, (void *)fn_1_22CC,
};

void *lbl_1_data_18C[3] = {
    (void *)fn_1_240C, (void *)fn_1_2654, (void *)fn_1_2800,
};

void *lbl_1_data_198[4] = {
    (void *)fn_1_2870, (void *)fn_1_2C74, (void *)fn_1_31C8, (void *)0x3F800000,
};

u32 lbl_1_data_1C0 = 0x12;
s32 lbl_1_data_4[20] = {
    0x00700000, 0x00700001, 0x00700002, 0x00700004, 0x00700005,
    0x00700006, 0x00700007, 0x00700008, 0x00700009, 0x0070000a,
    0x0070000b, 0x0070000c, 0x0070000d, 0x0070000e, 0x0070000f,
    0x00700010, 0x00700011, 0x00700012, 0x00700013, 0x00700014,
};

extern void *omInitObjMan(s16 arg0, s32 arg1);
extern void omGameSysInit(void *arg0);
extern void omOutView(void);
extern void fn_1_6E80(void);
extern void fn_1_62F0(void *arg0, void *arg1, void *arg2);
extern void MgSeqCreate(void *arg0);
extern M554ENTRY lbl_1_bss_10C[0x5c];

void *lbl_1_data_158[10] = {
    0, (void *)fn_1_1C0, (void *)fn_1_808, (void *)fn_1_84C, (void *)fn_1_914,
    (void *)fn_1_958, (void *)fn_1_990, (void *)fn_1_9D4, (void *)fn_1_9F8, (void *)fn_1_A2C,
};

void ObjectSetup(void)
{
    s16 i;
    for (i = 0; i < 0x5c; i++) {
        lbl_1_bss_10C[i].field0 = -1;
    }
    fn_1_62F0(&lbl_1_bss_B1C, lbl_1_bss_10C, lbl_1_data_4);
    lbl_1_bss_B1C.field0 = omInitObjMan(0x32, 0x2000);
    omGameSysInit((void *)lbl_1_bss_B1C.field0);
    omAddObjEx((void *)lbl_1_bss_B1C.field0, 0x7fda, 0, 0, -1, omOutView);
    omAddObjEx((void *)lbl_1_bss_B1C.field0, 0x7fda, 0, 0, -1, fn_1_6E80);
    omAddObjEx((void *)lbl_1_bss_B1C.field0, 0x7fda, 0, 0, -1, fn_1_A30);
    MgSeqCreate(lbl_1_data_158);
}

void fn_1_84C(s16 arg)
{
    lbl_1_bss_B1C.mode = 0;
    lbl_1_bss_B1C.field104 = 0;
    MgSeqModeDelaySet(arg + 1, -1);
    if (lbl_1_bss_0 == 0 && omCameraViewCheck(1) != 0) {
        omAddObjEx(lbl_1_bss_B1C.field0, 0x7fda, 0, 0, -1, fn_1_53B0);
        Hu3DModelAttrReset(*(s16 *)((u8 *)lbl_1_bss_B1C.field3c + 0x86c), 1);
        lbl_1_bss_0 = 1;
    }
}

void fn_1_808(void)
{
    ((void (*)(void))lbl_1_data_180[lbl_1_bss_B1C.mode])();
}

void fn_1_914(void)
{
    ((void (*)(void))lbl_1_data_18C[lbl_1_bss_B1C.mode])();
}

void fn_1_958(s16 arg)
{
    MgSeqModeDelaySet(arg + 1, -1);
    fn_1_2804();
}

void fn_1_990(void)
{
    ((void (*)(void))lbl_1_data_198[lbl_1_bss_B1C.mode])();
}

void fn_1_9D4(void)
{
    fn_1_31C8();
    MgSeqModeNext();
}

void fn_1_9F8(void)
{
    fn_1_31C8();
    if ((u8)WipeCheckEnd() == 0) {
        MgSeqModeNext();
    }
}

void fn_1_A2C(void)
{
}

void fn_1_A30(void)
{
}

void fn_1_A34(void)
{
    lbl_1_data_1C0 = lbl_1_data_1C0 - 1;
}

s32 fn_1_A54(void)
{
    return lbl_1_data_1C0;
}


void fn_1_1320(M554ENTRY *arg0)
{
    Hu3DModelAttrSet(arg0->field0, 1);
    Hu3DModelPosSet(arg0->field0, 0, 0, 0);
    Hu3DModelRotSet(arg0->field0, 0, 0, 0);
    arg0->field4 = 0;
    arg0->field8 = 0;
    arg0->fieldC = 0;
    arg0->field10 = 0;
    arg0->field14 = 0;
    arg0->field18 = 0;
    Hu3DModelCameraSet(arg0->field0, 1);
    Hu3DMotionTimeSet(arg0->field0, 0);
    Hu3DMotionSpeedSet(arg0->field0, 0);
    Hu3DModelAttrReset(arg0->field0, 1);
}
void fn_1_12A4(s32 arg, f32 value)
{
    // NONMATCHING: register allocation only - the target keeps model in r31 and
    // the loop index in r29, MWCC assigns r29/r30/r31 differently here.
    HU3DMODEL *model = &Hu3DData[((M554ENTRY *)lbl_1_bss_B1C.field3c)[arg].field0];
    void *data = *(void **)((u8 *)model + 0xc4);
    u8 *node = *(u8 **)((u8 *)data + 0x10);
    s32 i = 0;
    while (i < *(s16 *)((u8 *)(*(void **)((u8 *)model + 0xc4)) + 0x5c)) {
        *(f32 *)(node + 0x1c) = value;
        node = node + 0x3c;
        i = i + 1;
    }
}
