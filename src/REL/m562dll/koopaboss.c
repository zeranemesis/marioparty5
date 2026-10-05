#include "types.h"

/* library */
extern void Hu3DModelObjMtxGet(s16 modelId, char *nodeName, void *objData);

/* node names, .data of this translation unit */
static char lbl_1_data_8E4[] = "g000mSP-itemhook_R";
static char lbl_1_data_8F7[] = "g000mSP-itemhook_L";
static char lbl_1_data_90A[] = "g000mSP-head";

void *fn_1_1010C(u8 *arg0) {
    if (*(s32 *) ((u32) arg0 + 0x110) == 0) {
        Hu3DModelObjMtxGet(*(s16 *) ((u32) arg0 + 0x20), lbl_1_data_8E4,
                          (void *) ((u32) arg0 + 0x80));
        *(s32 *) ((u32) arg0 + 0x110) = 1;
    }
    return (void *) ((u32) arg0 + 0x80);
}

void *fn_1_10160(u8 *arg0) {
    if (*(s32 *) ((u32) arg0 + 0x114) == 0) {
        Hu3DModelObjMtxGet(*(s16 *) ((u32) arg0 + 0x20), lbl_1_data_8F7,
                          (void *) ((u32) arg0 + 0xB0));
        *(s32 *) ((u32) arg0 + 0x114) = 1;
    }
    return (void *) ((u32) arg0 + 0xB0);
}

void *fn_1_101B4(u8 *arg0) {
    if (*(s32 *) ((u32) arg0 + 0x118) == 0) {
        Hu3DModelObjMtxGet(*(s16 *) ((u32) arg0 + 0x20), lbl_1_data_90A,
                          (void *) ((u32) arg0 + 0xE0));
        *(s32 *) ((u32) arg0 + 0x118) = 1;
    }
    return (void *) ((u32) arg0 + 0xE0);
}

void fn_1_10520(u8 *arg0) {
    void *p31 = *(void **) arg0;

    if (*(u16 *) ((u32) arg0 + 0x4) == 1 ||
        *(u16 *) ((u32) arg0 + 0x4) == 2) {
        if (*(s16 *) ((u32) arg0 + 0x10) == 0 &&
            *(u16 *) ((u32) p31 + 0xC) == 4) {
            *(u16 *) ((u32) arg0 + 0xA) = 0;
            *(u16 *) ((u32) arg0 + 0x6) = 0;
            *(u16 *) ((u32) arg0 + 0x8) = 0;
            *(u16 *) ((u32) arg0 + 0x4) = 4;
        }
    }
}

void fn_1_10FEC(u8 *arg0) {
    void *p31 = *(void **) arg0;

    if (*(u16 *) ((u32) arg0 + 0x4) == 1 ||
        *(u16 *) ((u32) arg0 + 0x4) == 2) {
        if (*(s16 *) ((u32) arg0 + 0x10) == 0 &&
            *(u16 *) ((u32) p31 + 0xC) == 4) {
            *(u16 *) ((u32) arg0 + 0xA) = 0;
            *(u16 *) ((u32) arg0 + 0x6) = 0;
            *(u16 *) ((u32) arg0 + 0x8) = 0;
            *(u16 *) ((u32) arg0 + 0x4) = 5;
        }
    }
}