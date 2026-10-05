#include "types.h"

/* library */
extern void Hu3DModelKill(s16 modelId);
extern void Hu3DMotionKill(s16 motionId);
extern void HuWinExClose(s16 winId);
extern void HuWinExKill(s16 winId);
extern void HuMemDirectFree(void *ptr);
extern void GameMesKill(s16 mesId);
extern void GameMesClose(void);
extern void HuAudAllStop(void);
extern void *memset(void *dst, s32 val, u32 len);

/* sibling module functions */
extern void fn_1_2DB74(u8 *arg0);
extern void fn_1_24178(void *sprite);
extern void fn_1_17984(void *arg0);
extern void fn_1_140E4(void *arg0);
extern void fn_1_B4F8(void *arg0);
extern void fn_1_18F5C(void *arg0);
extern void fn_1_10018(void *arg0);
extern void fn_1_6FBC(void *arg0);
extern void fn_1_5BE4(void *arg0);
extern void fn_1_41B4(void *arg0);
extern void fn_1_2102C(void *arg0);
extern void fn_1_22620(void *arg0);
extern s32 fn_1_2446C(u8 *arg0);
extern s32 fn_1_252B4(u8 *arg0);
extern s32 fn_1_260B8(u8 *arg0);
extern s32 fn_1_27950(u8 *arg0);
extern s32 fn_1_280C8(u8 *arg0);
extern s32 fn_1_29DF8(u8 *arg0);

/* a model/motion id triple that gets torn down and then zeroed */
typedef struct {
    /* 0x00 */ u8 unk0[4];
    /* 0x04 */ s16 modelId0;
    /* 0x06 */ s16 motionId;
    /* 0x08 */ s16 modelId1;
} Lbl1ModelTriple;

void fn_1_12E0(Lbl1ModelTriple *arg0);

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Lbl1Vec;

typedef struct {
    Lbl1Vec pos;
    Lbl1Vec aim;
} Lbl1Marker;

typedef struct {
    u8 id0;
    u8 id1;
    u8 id2;
    u8 id3;
} Lbl1Tag;

void fn_1_FB0(u8 *arg0) {
    // NONMATCHING: MWCC 1.3.2 materializes the first call argument from its
    // callee-saved home (mr r31,r3) instead of reusing the still-live r3.
    fn_1_2DB74(arg0);
    fn_1_12E0((void *) ((u32) arg0 + 0x2858));
    HuWinExClose(*(s16 *) ((u32) arg0 + 0x28A4));
    HuWinExKill(*(s16 *) ((u32) arg0 + 0x28A4));
    fn_1_24178((void *) ((u32) arg0 + 0x221C));
    fn_1_17984((void *) ((u32) arg0 + 0x1B90));
    if (*(s32 *) ((u32) arg0 + 0x289C) == 0) {
        fn_1_140E4((void *) ((u32) arg0 + 0x88C));
        fn_1_B4F8((void *) ((u32) arg0 + 0x30C));
        fn_1_18F5C((void *) ((u32) arg0 + 0x7BC));
        HuMemDirectFree((void *) *(s32 *) ((u32) arg0 + 0x2864));
        *(s32 *) ((u32) arg0 + 0x2864) = 0;
    } else {
        fn_1_10018((void *) ((u32) arg0 + 0x3C4));
    }
    fn_1_6FBC((void *) ((u32) arg0 + 0x28));
    fn_1_5BE4((void *) ((u32) arg0 + 0x227C));
    fn_1_41B4((void *) ((u32) arg0 + 0x160));
    fn_1_2102C((void *) ((u32) arg0 + 0x258));
    fn_1_2102C((void *) ((u32) arg0 + 0x1BC));
    fn_1_22620((void *) ((u32) arg0 + 0x2F4));
    if (*(s32 *) ((u32) arg0 + 0x28A0) >= 0) {
        GameMesKill((s16) *(s32 *) ((u32) arg0 + 0x28A0));
    }
    GameMesClose();
    memset(arg0, 0, 0x28B4);
    HuAudAllStop();
}

void fn_1_12E0(Lbl1ModelTriple *arg0) {
    Hu3DModelKill(arg0->modelId1);
    Hu3DMotionKill(arg0->motionId);
    Hu3DModelKill(arg0->modelId0);
    memset(arg0, 0, 0xC);
}

void fn_1_1CB0(u8 *arg0, s32 arg1, s32 arg2) {
    void *p31 = (void *) ((u32) arg0 + 0x88C);
    void *p30 = (void *) ((u32) arg0 + 0x7BC);
    void *sp0C = (void *) ((u32) arg0 + 0x1B90);
    void *p29 = (void *) ((u32) arg0 + 0x30C);
    void *p28 = (void *) ((u32) arg0 + 0x3C4);
    void *sp08 = (void *) ((u32) arg0 + 0x28);

    switch ((s16) arg1) {
    case 0:
        *(s32 *) ((u32) p31 + 0x8) = arg2;
        *(s32 *) ((u32) p30 + 0x14) = 0;
        *(s32 *) ((u32) p29 + 0x1C) = arg2;
        *(s32 *) ((u32) p28 + 0x1C) = 0;
        break;
    case 1:
        *(s32 *) ((u32) p31 + 0x8) = 0;
        *(s32 *) ((u32) p30 + 0x14) = arg2;
        *(s32 *) ((u32) p29 + 0x1C) = arg2;
        *(s32 *) ((u32) p28 + 0x1C) = 0;
        break;
    case 2:
        *(s32 *) ((u32) p31 + 0x8) = 0;
        *(s32 *) ((u32) p30 + 0x14) = 0;
        *(s32 *) ((u32) p29 + 0x1C) = arg2;
        *(s32 *) ((u32) p28 + 0x1C) = 0;
        break;
    case 3:
        *(s32 *) ((u32) p31 + 0x8) = 0;
        *(s32 *) ((u32) p30 + 0x14) = 0;
        *(s32 *) ((u32) p29 + 0x1C) = 0;
        *(s32 *) ((u32) p28 + 0x1C) = arg2;
        break;
    }
}

s32 fn_1_1F54(u8 *arg0) {
    // NONMATCHING: the object pointer has to be a local to win r31 (MWCC puts
    // parameters after locals), and copying a parameter into a local makes
    // 1.3.2 spill it to a stack home, so the frame is 0x20 instead of 0x10.
    u8 *p = arg0;
    s32 ret = 1;

    switch (*(u16 *) ((u32) p + 0x12)) {
    case 0:
        ret = fn_1_2446C(p);
        break;
    case 1:
        ret = fn_1_252B4(p);
        break;
    case 2:
        ret = fn_1_260B8(p);
        break;
    case 3:
        ret = fn_1_27950(p);
        break;
    case 4:
        ret = fn_1_280C8(p);
        break;
    case 5:
        ret = fn_1_29DF8(p);
        break;
    }
    if (ret != 0) {
        (*(u16 *) ((u32) p + 0x12))++;
    }
    return ret;
}

static u16 lbl_1_bss_8;
static Lbl1Tag lbl_1_bss_80E[0x100];
static Lbl1Marker lbl_1_bss_6010[0x100];

void fn_1_2FF4(Lbl1Vec *arg0, Lbl1Vec *arg1, Lbl1Tag *arg2) {
    if (lbl_1_bss_8 >= 0x100) {
        return;
    }
    lbl_1_bss_6010[lbl_1_bss_8].pos = *arg0;
    lbl_1_bss_6010[lbl_1_bss_8].aim = *arg1;
    lbl_1_bss_80E[lbl_1_bss_8] = *arg2;
    lbl_1_bss_8++;
}