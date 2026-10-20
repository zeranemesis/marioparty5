#include "dolphin/types.h"
#include "game/hu3d.h"
#include "game/process.h"
#include "dolphin/mtx.h"
#include "stddef.h"

typedef s16 HU3DMODELID;

extern "C" {
void Hu3DModelShadowSet(HU3DMODELID modelId);
void Hu3DModelShadowDispOn(HU3DMODELID modelId);
void Hu3DModelShadowMapSet(HU3DMODELID modelId);
void Hu3DMotionTimeSet(HU3DMODELID modelId, float time);
void HuAudFXPlay(s16 id);
}

struct CModel {
    HU3DMODELID m_id;
};

CModel *__ct__6CModelFUi(CModel *self, u32 modelIdx);
void __dt__6CModelFv(CModel *self, int freeIt);

namespace m521 {
struct CClockHand {
    CModel m_model;
    Mtx m_baseMtx;
    Mtx m_mtx;
    float m_angle;
};

CClockHand *__ct__Q24m52110CClockHandFUiPA4_f(CClockHand *self, u32 dataNum, Mtx baseMtx);
void __dt__Q24m52110CClockHandFv(CClockHand *self, int freeIt);
}

extern "C" {
extern const int lbl_1_rodata_10[2];
extern const int lbl_1_rodata_20[2][2];
extern const float lbl_1_rodata_38;
extern const float lbl_1_rodata_3C;
extern const float lbl_1_rodata_40;
extern const float lbl_1_rodata_44;
extern const float lbl_1_rodata_48;
extern const float lbl_1_rodata_4C;
extern const float lbl_1_rodata_50;
extern const float lbl_1_rodata_54;
extern const float lbl_1_rodata_58;
extern const float lbl_1_rodata_5C;
extern const float lbl_1_rodata_60;
extern const float lbl_1_rodata_64;
extern const double lbl_1_rodata_68;
extern const double lbl_1_rodata_70;
extern const float lbl_1_rodata_78;
extern const float lbl_1_rodata_7C;
extern const double lbl_1_rodata_80;
extern const float lbl_1_rodata_88;
extern const double lbl_1_rodata_90;
extern const double lbl_1_rodata_98;
extern const float lbl_1_rodata_A0;
extern const double lbl_1_rodata_A8;
extern const double lbl_1_rodata_B0;
extern const float lbl_1_rodata_B8;
extern const float lbl_1_rodata_BC;
extern const float lbl_1_rodata_C0;
extern const float lbl_1_rodata_C4;
extern const float lbl_1_rodata_C8;
extern const double lbl_1_rodata_D0;
extern const double lbl_1_rodata_D8;
extern const double lbl_1_rodata_E0;
extern const float lbl_1_rodata_E8;
extern const double lbl_1_rodata_F0;
extern const float lbl_1_rodata_F8;
extern const float lbl_1_rodata_FC;
extern const double lbl_1_rodata_100;
extern const float lbl_1_rodata_108[4];
extern const float lbl_1_rodata_118;
extern const float lbl_1_rodata_11C;
extern const float lbl_1_rodata_120;
extern const float lbl_1_rodata_124;
extern const float lbl_1_rodata_128;
extern const float lbl_1_rodata_12C;
extern const float lbl_1_rodata_130;
extern const double lbl_1_rodata_138;
extern const float lbl_1_rodata_140;
extern const float lbl_1_rodata_144;
extern const float lbl_1_rodata_148;
extern const float lbl_1_rodata_14C;
extern const float lbl_1_rodata_150;
extern const double lbl_1_rodata_158;
extern const double lbl_1_rodata_160[2];
extern const float lbl_1_rodata_168;
extern const float lbl_1_rodata_16C;
extern const double lbl_1_rodata_170;
extern const double lbl_1_rodata_178;
extern const float lbl_1_rodata_180;
extern const double lbl_1_rodata_188;
extern const float lbl_1_rodata_190[2];
extern const float lbl_1_rodata_198;
extern const float lbl_1_rodata_19C;
extern const float lbl_1_rodata_1A0;
extern const double lbl_1_rodata_1A8;
extern const double lbl_1_rodata_1B0;
extern const double lbl_1_rodata_1B8;
extern const double lbl_1_rodata_1C0;
extern const double lbl_1_rodata_1C8;
extern const double lbl_1_rodata_1D0;
extern const float lbl_1_rodata_1D8;
extern const float lbl_1_rodata_1DC;
extern const float lbl_1_rodata_1E0;
extern const float lbl_1_rodata_1E4;
extern const float lbl_1_rodata_1E8;
extern const float lbl_1_rodata_1EC;
extern const float lbl_1_rodata_1F0;
extern const double lbl_1_rodata_1F8;
extern const double lbl_1_rodata_200;
extern const float lbl_1_rodata_208;
extern const float lbl_1_rodata_20C;
extern const float lbl_1_rodata_210;
extern const float lbl_1_rodata_214;
extern const float lbl_1_rodata_218;
extern const float lbl_1_rodata_21C;
extern const double lbl_1_rodata_220;
extern const float lbl_1_rodata_228;
extern const float lbl_1_rodata_22C;
extern const float lbl_1_rodata_230;
extern const float lbl_1_rodata_234;
extern const float lbl_1_rodata_238[7];
extern const float lbl_1_rodata_258;
extern const float lbl_1_rodata_25C;
extern const float lbl_1_rodata_260;
extern const double lbl_1_rodata_268;
extern const double lbl_1_rodata_270;
extern const float lbl_1_rodata_278;
extern const float lbl_1_rodata_27C;
extern const float lbl_1_rodata_280;
extern const float lbl_1_rodata_284;
extern const float lbl_1_rodata_288;
extern const float lbl_1_rodata_28C;
extern const float lbl_1_rodata_290;
extern const float lbl_1_rodata_294;
extern const int lbl_1_rodata_298[7];
extern const float lbl_1_rodata_2B4;
extern const float lbl_1_rodata_2B8;
extern const float lbl_1_rodata_2BC;
extern const float lbl_1_rodata_2C0;
extern const float lbl_1_rodata_2C4;
extern const double lbl_1_rodata_2C8;
extern const float lbl_1_rodata_2F4;
extern const float lbl_1_rodata_2F8;
extern const float lbl_1_rodata_2FC;
extern const float lbl_1_rodata_300;
extern const float lbl_1_rodata_304;
extern const float lbl_1_rodata_308;
extern const float lbl_1_rodata_30C;
extern const float lbl_1_rodata_310;
extern const float lbl_1_rodata_314;
extern const float lbl_1_rodata_318;
extern const float lbl_1_rodata_31C;
extern const double lbl_1_rodata_320;
extern const double lbl_1_rodata_328;
extern const double lbl_1_rodata_330;
extern const float lbl_1_rodata_338;
extern const float lbl_1_rodata_33C[4];
extern const float lbl_1_rodata_34C;
extern const float lbl_1_rodata_350;
extern const float lbl_1_rodata_354;
extern const float lbl_1_rodata_358;
extern const float lbl_1_rodata_35C;
extern const float lbl_1_rodata_360[3];
extern const float lbl_1_rodata_36C;
extern const float lbl_1_rodata_370;
extern const float lbl_1_rodata_374;
extern const float lbl_1_rodata_378;
extern const float lbl_1_rodata_37C;
extern const float lbl_1_rodata_380;
extern const float lbl_1_rodata_384;
extern const float lbl_1_rodata_388;
extern const float lbl_1_rodata_38C;
extern const double lbl_1_rodata_390;
extern const float lbl_1_rodata_398;
extern const float lbl_1_rodata_39C;
extern const double lbl_1_rodata_3A0;
extern const double lbl_1_rodata_3A8;
extern const float lbl_1_rodata_3B0;
extern const float lbl_1_rodata_3B4;
extern const double lbl_1_rodata_3B8;
extern const float lbl_1_rodata_3C0;
extern const float lbl_1_rodata_3C4;
extern const float lbl_1_rodata_3C8[2];
extern const float lbl_1_rodata_3D0;
extern const float lbl_1_rodata_3D4;
extern const float lbl_1_rodata_3D8;
extern const float lbl_1_rodata_3DC[4];
extern const float lbl_1_rodata_3EC;
extern const double lbl_1_rodata_3F0;
extern const double lbl_1_rodata_3F8;

extern void *lbl_1_data_0;
extern void *lbl_1_data_12;
extern void *lbl_1_data_20;
extern void *lbl_1_data_1E4;
extern void *lbl_1_data_1F4;

extern u8 lbl_1_bss_A5C;

extern void __construct_array(void *array, void *dtor, void *ctor, unsigned int count, unsigned int size, int flags);
extern void __destroy_arr(void *array, void *dtor, void *ctor, unsigned int count, unsigned int size, int flags);
extern void __dl__FPv(void *ptr);
extern int fn_1_4A68(void);
extern void fn_1_4C3C(void);
extern void fn_1_4790(void);
extern void fn_1_4844(void *obj, int arg);
extern void fn_1_48B8(void *obj);
extern void fn_1_48C0(void *obj);
extern void fn_1_7870(void *obj, unsigned int flag);
extern void fn_1_7F8(void *obj, int arg1, int arg2);
extern void fn_1_894(void *obj, int arg1, int arg2);
extern void fn_1_42A0(void *obj);
extern void fn_1_42B4(void *obj);
extern void fn_1_139C(void *obj, int arg1, float arg2);
extern void fn_1_1824(void *obj, void *arg1, void *arg2, void *arg3);
extern void fn_1_1CA0(void *obj, void *arg1, void *arg2, void *arg3, void *arg4);
extern void fn_1_20B8(void *obj, void *arg1, void *arg2, void *arg3);
extern void fn_1_231C(void *obj);
extern void fn_1_23D8(void *obj, float arg1);
extern void fn_1_2480(void *obj, void *arg1, void *arg2);
extern void fn_1_24D0(void *obj, float arg1, float arg2, float arg3);
extern void fn_1_2658(void *obj, void *arg1, void *arg2);
extern void fn_1_2998(void *obj, float arg1);
extern void fn_1_2C98(void *obj, float arg1);
extern void fn_1_2EFC(void *obj, void *arg1, void *arg2);
extern void fn_1_3268(void *obj, void *arg1, void *arg2);
extern void fn_1_35B0(void *obj);
extern void fn_1_42C8(void *obj);

int fn_1_0(void *obj) {
    u8 *ptr = (u8 *)obj;
    int result = 0;
    if (ptr[0x14] != 0) {
        if (ptr[0x15] != 0) {
            result = 1;
        }
    }
    return result;
}

void fn_1_5D0(void *obj) {
    *(int *)obj = 0;
}

int fn_1_7E8(void *obj) {
    int val = *(int *)((u8 *)obj + 0x20);
    val = __cntlzw(val);
    return val >> 5;
}

void fn_1_924(void *obj) {
    float *ptr = (float *)obj;
    ptr[0x40 / 4] = ptr[0x38 / 4];
    ptr[0x44 / 4] = ptr[0x3C / 4];
}

int fn_1_1370(void *obj, int idx) {
    u8 *ptr = (u8 *)obj;
    if (idx >= 2) return 0;
    if (idx < 0) return 0;
    return ptr[idx + 0x14];
}

int fn_1_22BC(void *obj, int arg) {
    u8 *ptr = (u8 *)obj;
    int result = 0;
    if (arg != 0) {
        if (ptr[0x15] != 0) {
            result = 1;
        } else {
            result = 1;
        }
    } else {
        if (ptr[0x14] != 0) {
            result = 0;
        } else if (ptr[0x15] != 0) {
            result = 1;
        } else {
            result = 0;
        }
    }
    return result;
}

void fn_1_4C8(void *obj, short arg) {
    int *ptr = (int *)obj;
    if (ptr != 0) {
        int val = ptr[0];
        if (val != 0) {
            CModel *model = (CModel *)val;
            __dt__6CModelFv(model, 1);
        }
        if (arg != 0) {
            __dl__FPv(ptr);
        }
    }
}

void fn_1_520(void *obj) {
    int *ptr = (int *)obj;
    if (ptr != 0) {
        int val = ptr[0];
        if (val != 0) {
            CModel *model = (CModel *)val;
            __dt__6CModelFv(model, 1);
        }
        fn_1_48B8(ptr);
    }
}

void fn_1_54C(void *obj, short arg) {
    int *ptr = (int *)obj;
    if (ptr != 0) {
        int val = ptr[0];
        if (val != 0) {
            m521::CClockHand *hand = (m521::CClockHand *)val;
            __dt__Q24m52110CClockHandFv(hand, 1);
        }
        if (arg != 0) {
            __dl__FPv(ptr);
        }
    }
}

void fn_1_5A4(void *obj) {
    int *ptr = (int *)obj;
    if (ptr != 0) {
        int val = ptr[0];
        if (val != 0) {
            m521::CClockHand *hand = (m521::CClockHand *)val;
            __dt__Q24m52110CClockHandFv(hand, 1);
        }
        fn_1_48C0(ptr);
    }
}

void fn_1_5DC(void *obj, short arg) {
    int *ptr = (int *)obj;
    if (ptr != 0) {
        __destroy_arr((u8 *)ptr + 0x6C, (void *)fn_1_4C8, (void *)0, 4, 0x34, 2);
        __destroy_arr((u8 *)ptr + 0x64, (void *)fn_1_54C, (void *)0, 4, 0x34, 2);
        if (arg != 0) {
            __dl__FPv(ptr);
        }
    }
}

void fn_1_38(void *obj, int arg1, int arg2) {
    int *objPtr = (int *)obj;
    int *arg1Ptr = (int *)arg1;
    int *arg2Ptr = (int *)arg2;

    objPtr[0] = arg1Ptr[0];
    objPtr[1] = arg1Ptr[1];
    objPtr[2] = arg2Ptr[0];
    objPtr[3] = arg2Ptr[1];

    Mtx mtx1, mtx2;
    float *floatPtr = (float *)objPtr;
    int *intPtr = objPtr;

    if (arg2Ptr != 0) {
        fn_1_4A68();
        int val = fn_1_4A68();
        if (val != 0) {
            intPtr[0x15] = val;
            int modelData = lbl_1_rodata_10[arg2Ptr[0] * 2];
            CModel *model = (CModel *)(intPtr + 0x15);
            model = __ct__6CModelFUi(model, modelData);
            intPtr[0x15] = (int)model;
            intPtr[0x19] = 0;
        }
        if (arg2Ptr[0] < 0) {
            intPtr[0x19] = 0;
        } else if (arg2Ptr[0] > 1) {
            intPtr[0x19] = 0;
        }
    }

    if (arg2Ptr[0] >= 0) {
        if (arg2Ptr[0] <= 1) {
            int modelData = lbl_1_rodata_10[arg2Ptr[0] * 2];
            CModel *model = (CModel *)(intPtr + 0x15);
            model = __ct__6CModelFUi(model, modelData);
            intPtr[0x15] = (int)model;
            int modelId = model->m_id;
            fn_1_7870((void *)modelId, 0x40000002);
            modelId = model->m_id;
            fn_1_7870((void *)modelId, 0x40000002);
        }
    }

    if (intPtr[0x1C] != intPtr[0x15]) {
        if (intPtr[0x1C] != 0) {
            CModel *model = (CModel *)intPtr[0x1C];
            __dt__6CModelFv(model, 1);
        }
        intPtr[0x1C] = intPtr[0x15];
    }

    if (intPtr[0x1B] != intPtr[0x19]) {
        if (intPtr[0x1B] != 0) {
            CModel *model = (CModel *)intPtr[0x1B];
            __dt__6CModelFv(model, 1);
        }
        intPtr[0x1B] = intPtr[0x19];
    }

    if (arg2Ptr[0] == 2) {
        PSMTXTrans(mtx1, lbl_1_rodata_38, lbl_1_rodata_38, lbl_1_rodata_40);
        PSMTXTrans(mtx2, lbl_1_rodata_38, lbl_1_rodata_38, lbl_1_rodata_40);
    } else {
        PSMTXTrans(mtx1, lbl_1_rodata_38, lbl_1_rodata_44, lbl_1_rodata_38);
        PSMTXTrans(mtx2, lbl_1_rodata_38, lbl_1_rodata_48, lbl_1_rodata_38);
    }

    if (arg2Ptr[0] == 2) {
        float f29 = ((float *)arg2Ptr)[2];
        float f30 = ((float *)arg2Ptr)[1];
        float f31 = ((float *)arg2Ptr)[0];
        PSMTXTransApply(mtx1, mtx1, f31, f30, f29);
        float f26 = ((float *)arg2Ptr)[5];
        float f27 = ((float *)arg2Ptr)[4];
        float f28 = ((float *)arg2Ptr)[3];
        PSMTXTransApply(mtx2, mtx2, f28, f27, f26);
    }

    int model = fn_1_4A68();
    if (model != 0) {
        int modelData = lbl_1_rodata_20[arg2Ptr[0]][0];
        m521::CClockHand *hand = (m521::CClockHand *)model;
        hand = __ct__Q24m52110CClockHandFUiPA4_f(hand, modelData, mtx1);
        intPtr[0x19] = (int)hand;
    }

    if (intPtr[0x19] != intPtr[0x15]) {
        if (intPtr[0x19] != 0) {
            m521::CClockHand *hand = (m521::CClockHand *)intPtr[0x19];
            __dt__Q24m52110CClockHandFv(hand, 1);
        }
        intPtr[0x19] = intPtr[0x15];
    }

    model = fn_1_4A68();
    if (model != 0) {
        int modelData = lbl_1_rodata_20[arg2Ptr[0]][1];
        m521::CClockHand *hand = (m521::CClockHand *)model;
        hand = __ct__Q24m52110CClockHandFUiPA4_f(hand, modelData, mtx2);
        intPtr[0x1A] = (int)hand;
    }

    if (intPtr[0x1A] != intPtr[0x16]) {
        if (intPtr[0x1A] != 0) {
            m521::CClockHand *hand = (m521::CClockHand *)intPtr[0x1A];
            __dt__Q24m52110CClockHandFv(hand, 1);
        }
        intPtr[0x1A] = intPtr[0x16];
    }

    objPtr[0] = (int)objPtr;
    fn_1_7F8(objPtr, 6, 0);
    fn_1_894(objPtr, 6, 0);
    fn_1_924(objPtr);

    floatPtr[9] = lbl_1_rodata_38;
    intPtr[8] = 0;
    intPtr[0x13] = -1;
    intPtr[0x14] = -1;
    intPtr[0x12] = 0;
    intPtr[0x17] = -1;
    intPtr[0x18] = -1;

    fn_1_42A0(objPtr);

    floatPtr[0xC] = lbl_1_rodata_38;
    floatPtr[0xD] = lbl_1_rodata_38;
    floatPtr[0xE] = lbl_1_rodata_38;
}
}