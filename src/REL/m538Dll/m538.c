/* The REL signature of Hu3DCameraPosSet takes nine floats after the camera
 * index, i.e. one more than include/game/hu3d.h declares: the ninth
 * (targetZ) does not fit in f1..f8 and rides the outgoing-argument slot at
 * 8(sp), which is where the `stfs ...,0x8(r1)` in fn_1_1488 and fn_1_16BC
 * comes from -- it is a real argument, not a dead local.  Rename the header
 * declaration away and declare the REL signature so the trailing load is
 * emitted.  (A cast on the designator drops it but makes MWCC emit an
 * indirect call, and warn_aliasing does not cover a redeclaration.) */
#define Hu3DCameraPosSet Hu3DCameraPosSet_hu3d
#include "REL/m538Dll.h"
#undef Hu3DCameraPosSet
void Hu3DCameraPosSet(int cameraBit, f32 posX, f32 posY, f32 posZ, f32 upX, f32 upY, f32 upZ, f32 targetX, f32 targetY, f32 targetZ);

/* The 17 M2C_ERROR sites below stand for `cror eq,gt,eq` after an fcmpo, which
 * m2c could not translate. Without this header M2C_ERROR is an undeclared
 * identifier, CodeWarrior emits a call to a symbol named M2C_ERROR, and the REL
 * link fails with "Failed to find symbol M2C_ERROR in any module". m2c_macros.h
 * defines it as the no-op the decompiler expects. These functions stay
 * NONMATCHING either way -- the cror is not reproduced. */
#include "m2c_macros.h"


/* fn_1_69C4 only ever touches one s16 field of its argument, at 0x48. */
struct m538_s16_48_s {
    u8 pad[0x48];
    s16 model;
};

void     ObjectSetup(void);                           /* static */
void     fn_1_1FC(s16 mode, s16 frameNo);             /* static */
void     fn_1_264(s16 mode, s16 frameNo);             /* static */
void     fn_1_4A4(s16 mode, s16 frameNo);             /* static */
void     fn_1_4F8(s16 mode, s16 frameNo);             /* static */
void     fn_1_614(s16 mode, s16 frameNo);             /* static */
void     fn_1_848(s16 mode, s16 frameNo);             /* static */
void     fn_1_BB4(s16 mode, s16 frameNo);             /* static */
static void     fn_1_CC0(s16 mode, s16 frameNo);             /* static */
void     fn_1_CFC(s16 mode, s16 frameNo);             /* static */
void     fn_1_D1C(OMOBJ *obj);                        /* static */
void     fn_1_FD0(OMOBJ *obj);                        /* static */
void     fn_1_1194(void *arg0);                       /* static */
void     fn_1_1198(OMOBJ *obj);                       /* static */
void     fn_1_12F0(OMOBJ *obj);                       /* static */
void     fn_1_1378(void *arg0);                       /* static */
void     fn_1_137C(M538 *arg0);                       /* static */
void     fn_1_13D8(M538 *arg0);                       /* static */
void     fn_1_1488(void *arg0);                       /* static */
void     fn_1_1574(void *arg0);                       /* static */
void     fn_1_1650(M538_CAMWORK *arg0);               /* static */
void     fn_1_16BC(M538_CAMWORK *arg0);               /* static */
void     fn_1_1958(M538 *arg0);                       /* static */
s32      fn_1_1B94(M538_PLAYER *arg0);                 /* static */
s32      fn_1_1DC8(M538_PLAYER *arg0);                 /* static */
s32      fn_1_2988(M538_PLAYER *arg0);                 /* static */
s32      fn_1_29F8(M538_PLAYER *arg0);                 /* static */
s32      fn_1_2CB8(M538_PLAYER *arg0);                 /* static */
s32      fn_1_32C4(M538_PLAYER *arg0);                 /* static */
s32      fn_1_39DC(M538_PLAYER *arg0);                 /* static */
s32      fn_1_3B2C(M538_PLAYER *arg0);                 /* static */
s32      fn_1_3CD4(M538_PLAYER *arg0);                 /* static */
s32      fn_1_3E7C(M538_PLAYER *arg0);                 /* static */
s32      fn_1_3F60(M538_PLAYER *arg0);                 /* static */
s32      fn_1_4428(M538_PLAYER *arg0);                 /* static */
s32      fn_1_44AC(M538_DOLLY *arg0);                  /* static */
void     fn_1_4534(M538_PLAYER *arg0);                /* static */
void     fn_1_4714(M538 *arg0);                       /* static */
void     fn_1_4968(M538 *arg0);                       /* static */
void     fn_1_4C38(M538 *arg0);                       /* static */
void     fn_1_4F74(M538 *arg0);                       /* static */
s32      fn_1_55C8(M538_PLAYER *arg0, f32 *arg1, f32 *arg2); /* static */
void     fn_1_56A4(M538 *arg0);                       /* static */
void     fn_1_5FC4(M538_ANIM *arg0, s16 arg1);        /* static */
s32      fn_1_61F8(M538_ANIM *arg0);                   /* static */
s32      fn_1_62B4(M538_ANIM *arg0);                   /* static */
s32      fn_1_6400(M538_ANIM *arg0);                   /* static */
s32      fn_1_6558(M538_ANIM *arg0);                   /* static */
s32      fn_1_66A4(M538_ANIM *arg0);                   /* static */
s32      fn_1_6918(void *arg0);                        /* static */
void     fn_1_69C4(struct m538_s16_48_s *arg0);        /* static */
void     fn_1_6AE4(M538_PLAYER *arg0);                /* static */
void     fn_1_6C24(M538_PLAYER *arg0);                /* static */
void     fn_1_6CD4(M538 *arg0);                       /* static */
M538_NODE0 * fn_1_6D98(M538 *arg0, M538_NODE0FUNC arg1, void *arg2); /* static */
void     fn_1_6DEC(M538 *arg0, M538_NODE0 *arg1);     /* static */
void     fn_1_6E50(M538 *arg0);                       /* static */
s32      fn_1_6F24(M538_NODE0 *arg0);                  /* static */
void     fn_1_72CC(M538_CAMSWING *arg0);              /* static */
void     fn_1_765C(M538_CAMSWING *arg0);              /* static */
void     fn_1_77A4(M538_CAMSWING *arg0);              /* static */
s32      fn_1_788C(M538_CAMWORK *arg0);                /* static */
s32      fn_1_7AA0(M538_CAMWORK *arg0);                /* static */
s32      fn_1_7CB0(M538_CAMWORK *arg0);                /* static */
s32      fn_1_7F04(M538_PLAYER *arg0);                 /* static */
s32      fn_1_7FAC(M538_PLAYER *arg0);                 /* static */
s32      fn_1_8554(M538_PLAYER *arg0);                 /* static */
void     fn_1_87E4(M538_POOL *arg0, M538_NODE *arg1, s32 arg2); /* static */
M538_NODE * fn_1_8858(M538_POOL *arg0, M538_NODEFUNC arg1); /* static */
void     fn_1_88A8(M538_NODE *arg0);                  /* static */
void     fn_1_8920(M538_POOL *arg0);                  /* static */
void     fn_1_8A00(M538 *arg0);                       /* static */
void     fn_1_8B6C(M538 *arg0);                       /* static */
s32      fn_1_8CD8(M538_NODE *arg0);                   /* static */
s32      fn_1_8F58(M538_NODE *arg0);                   /* static */
s32      fn_1_9024(M538_NODE0 *arg0);                  /* static */
s32      fn_1_91D8(f32 *arg0, s32 arg1);                   /* static */
void     fn_1_93C4(M538 *arg0);                       /* static */
s32      fn_1_94DC(M538_NODE *arg0);                   /* static */
s32      fn_1_9C04(M538_NODE *arg0);                   /* static */
void     fn_1_A2A4(M538 *arg0);                       /* static */
void     fn_1_A368(M538 *arg0);                       /* static */
s32      fn_1_A424(M538_DOLLY *arg0);                  /* static */
void     fn_1_A6F8(M538 *arg0);                       /* static */
void     fn_1_A8A0(M538_CTRL *arg0);                   /* static */
s32      fn_1_AB54(M538_NODE0 *arg0);                  /* static */
s32      fn_1_B0D0(M538_NODE0 *arg0);                  /* static */

extern void (*_ctors[])(void);
extern void (*_dtors[])(void);
static MGSEQ_PARAM lbl_1_data_0 = {
    0,
    0,
    fn_1_1FC,
    fn_1_264,
    fn_1_4A4,
    fn_1_4F8,
    fn_1_614,
    fn_1_848,
    fn_1_BB4,
    fn_1_CC0,
    fn_1_CFC,
};
static const u32 m538_charMotion[0xD][0xB] = {
    { 0x0087007A, 0x0087007C, 0x0087007D, 0x008C005C, 0x008C005D, 0x0087007E, 0x008C002B, 0x008C0001, 0x008C0006, 0x008C0017, 0x008C0000 },
    { 0x002E007A, 0x002E007C, 0x002E007D, 0x0033005C, 0x0033005D, 0x002E007E, 0x0033002B, 0x00330001, 0x00330006, 0x00330017, 0x00330000 },
    { 0x00B2007A, 0x00B2007C, 0x00B2007D, 0x00B7005C, 0x00B7005D, 0x00B2007E, 0x00B7002B, 0x00B70001, 0x00B70006, 0x00B70017, 0x00B70000 },
    { 0x00E8007A, 0x00E8007C, 0x00E8007D, 0x00ED005C, 0x00ED005D, 0x00E8007E, 0x00ED002B, 0x00ED0001, 0x00ED0006, 0x00ED0017, 0x00ED0000 },
    { 0x00E1007A, 0x00E1007C, 0x00E1007D, 0x00E6005C, 0x00E6005D, 0x00E1007E, 0x00E6002B, 0x00E60001, 0x00E60006, 0x00E60017, 0x00E60000 },
    { 0x0012007A, 0x0012007C, 0x0012007D, 0x0017005C, 0x0017005D, 0x0012007E, 0x0017002B, 0x00170001, 0x00170006, 0x00170017, 0x00170000 },
    { 0x00DB007A, 0x00DB007C, 0x00DB007D, 0x00E0005C, 0x00E0005D, 0x00DB007E, 0x00E0002B, 0x00E00001, 0x00E00006, 0x00E00017, 0x00E00000 },
    { 0x0028007A, 0x0028007C, 0x0028007D, 0x002D005C, 0x002D005D, 0x0028007E, 0x002D002B, 0x002D0001, 0x002D0006, 0x002D0017, 0x002D0000 },
    { 0x00C4007A, 0x00C4007C, 0x00C4007D, 0x00C9005C, 0x00C9005D, 0x00C4007E, 0x00C9002B, 0x00C90001, 0x00C90006, 0x00C90017, 0x00C90000 },
    { 0x009C007A, 0x009C007C, 0x009C007D, 0x00B0005C, 0x00B0005D, 0x009C007E, 0x00B0002B, 0x00B00001, 0x00B00006, 0x00B00017, 0x00B00000 },
    { 0x009C007A, 0x009C007C, 0x009C007D, 0x00B0005C, 0x00B0005D, 0x009C007E, 0x00B0002B, 0x00B00001, 0x00B00006, 0x00B00017, 0x00B00000 },
    { 0x009C007A, 0x009C007C, 0x009C007D, 0x00B0005C, 0x00B0005D, 0x009C007E, 0x00B0002B, 0x00B00001, 0x00B00006, 0x00B00017, 0x00B00000 },
};
static s32 lbl_1_data_264[0x1D] = {
    0x600009,
    0x600000,
    0x600001,
    0x600002,
    0x600003,
    0x600004,
    0x600005,
    0x600006,
    0x600007,
    0x600008,
    0x60000A,
    0x60000B,
    0x60000C,
    0x60000D,
    0x60000E,
    0x60000F,
    0x600010,
    0x600011,
    0x600013,
    0x600015,
    0x600016,
    0x600017,
    0x600019,
    0x60001A,
    0x60001B,
    0x60001C,
    0x60001D,
    0x60001E,
    0x60001F,
};
static s32 lbl_1_data_2D8[3] = { 0x600012, 0x600014, 0x600018 };
static const f32 lbl_1_data_2E4[44] = {0.0, 190.0, -44.0, 90.0, 7.0, 3900.0, 0.0, 0.0, -44.0, 90.0, 60.0, 6700.0, 0.0, 0.0, -44.0, 90.0, 60.0, 3000.0, 177.0, 0.0, 177.0, 0.0, 177.0, 0.0, 177.0, 0.0, 177.0, 0.0, 177.0, 0.0, 177.0, 0.0, 177.0, 0.0, 177.0, 0.0, 177.0, 0.0, 177.0, 0.0, 177.0, 0.0, 177.0, 0.0};
static s32 lbl_1_data_394[0xD] = {
    0x1F9,
    0x1E5,
    0x25D,
    0x2AD,
    0x285,
    0x1BD,
    0x299,
    0x1D1,
    0x271,
    0x235,
    0x249,
    0x221,
    0x20D,
};
static s32 lbl_1_data_3C8[0xD] = {
    0x1FD,
    0x1E9,
    0x261,
    0x2B1,
    0x289,
    0x1C1,
    0x29D,
    0x1D5,
    0x275,
    0x239,
    0x24D,
    0x225,
    0x211,
};
static char lbl_1_data_418[0xC] = { 0x6D, 0x35, 0x33, 0x38, 0x5F, 0x74, 0x73, 0x75, 0x6D, 0x75, 0, 0 };
static s32 lbl_1_bss_0;
static s32 lbl_1_bss_4;
static s32 lbl_1_bss_8;
static OMOBJ *lbl_1_bss_C;
static HUPROCESS *lbl_1_bss_10;
static u16 lbl_1_rodata_10[4] = { 1, 2, 4, 8 };     /* const */
static const f32 lbl_1_rodata_60[3] = {0.0, 1.0, 0.0};
static const f32 lbl_1_rodata_6C[3] = {0.0, 4800.0, 751.0};
static const f32 lbl_1_rodata_78[3] = {0.0, 0.0, 0.0};
static const u16 lbl_1_rodata_B8[4] = {0x50, 0x5A, 0x5F, 0x0};
static const u16 lbl_1_rodata_C0[4] = {0x14, 0x32, 0x3C, 0x50};
static const u16 lbl_1_rodata_C8[4] = {0x3C, 0x28, 0xA, 0x0};
static const u16 lbl_1_rodata_D0[4] = {0x0, 0x0, 0x1E, 0x32};
static const f32 lbl_1_rodata_EC[4] = {26244.0, 24964.0, 23716.0, 22500.0};
static const f32 lbl_1_rodata_FC[4] = {7.006492321624085e-44, 9.80908925027372e-44, 1.2611686178923354e-43, 1.401298464324817e-43};
static const f32 lbl_1_rodata_158[4] = {-100.0, -100.0, 100.0, 100.0};
static const f32 lbl_1_rodata_168[4] = {0.0, 150.0, 150.0, 0.0};
static const u16 lbl_1_rodata_190[4] = { 0x1, 0x4, 0x7, 0x0 };
static M538_ANIMFUNC lbl_1_rodata_1A0[3] = { fn_1_62B4, fn_1_6400, fn_1_6558 };
/* a four-float table that the original also reads as raw 32-bit words */
union m538_f4 { struct { s32 a, b, c, d; } w; f32 f[4]; s32 i[4]; };
struct m538_xy { f32 x, y; };
static const union m538_f4 lbl_1_rodata_1CC = {{-400.0, -200.0, 200.0, 400.0}};
static const u8 lbl_1_rodata_1EC[24] = {0xD9, 0x99, 0x52, 0xB3, 0xCC, 0x0, 0xE0, 0x99, 0x52, 0xE6, 0x66, 0x33, 0x99, 0x66, 0x99, 0xBF, 0x33, 0x99, 0xFF, 0xB3, 0xCC, 0xE6, 0x7F, 0x99};
static const u8 lbl_1_rodata_21C[48] = {0xFF, 0xB3, 0x0, 0xE5, 0xFF, 0x66, 0xFF, 0xE5, 0x4D, 0xCC, 0xB3, 0x0, 0xCC, 0x66, 0x33, 0xFF, 0xE5, 0x4D, 0xFF, 0xA6, 0x4D, 0x7F, 0x33, 0x0, 0xB3, 0x4D, 0xB3, 0xE5, 0xB3, 0xE5, 0xCC, 0x7F, 0xE5, 0x7F, 0x33, 0x7F, 0xCC, 0x66, 0x7F, 0xFF, 0xBF, 0xCC, 0xFF, 0x7F, 0x99, 0xA6, 0x4D, 0x7F};
static const u8 lbl_1_rodata_258[48] = {0xFF, 0xB3, 0x0, 0xE5, 0xFF, 0x66, 0xFF, 0xE5, 0x4D, 0xCC, 0xB3, 0x0, 0xCC, 0x66, 0x33, 0xFF, 0xE5, 0x4D, 0xFF, 0xA6, 0x4D, 0x7F, 0x33, 0x0, 0xB3, 0x4D, 0xB3, 0xE5, 0xB3, 0xE5, 0xCC, 0x7F, 0xE5, 0x7F, 0x33, 0x7F, 0xCC, 0x66, 0x7F, 0xFF, 0xBF, 0xCC, 0xFF, 0x7F, 0x99, 0xA6, 0x4D, 0x7F};
static const s16 lbl_1_rodata_2A4[4] = { 0x1A, 0x1A, 0x19, 0x1B };

s32 _prolog(void) {
    void (**ctor)(void) = (void (**)(void)) &_ctors;

    while (*ctor != 0) {
        (*ctor)();
        ctor++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void) {
    void (**dtor)(void) = (void (**)(void)) &_dtors;

    while (*dtor != 0) {
        (*dtor)();
        dtor++;
    }
}

void ObjectSetup(void) {
    OMOBJ *temp_r31;
    M538 *temp_r30;
    s32 var_r29;

    lbl_1_bss_10 = omInitObjMan(0xA, 0x2000);
    omGameSysInit(lbl_1_bss_10);
    temp_r31 = omAddObjEx(lbl_1_bss_10, 0x65, 0U, 0U, -1, fn_1_1198);
    lbl_1_bss_C = temp_r31;
    temp_r31->data = HuMemDirectMallocNum(HEAP_HEAP, 0x1920, HU_MEMNUM_OVL);
    temp_r30 = temp_r31->data;
    temp_r30->activeF = 0;
    fn_1_1488(temp_r30);
    fn_1_13D8(temp_r30);
    fn_1_1574(temp_r30);
    var_r29 = 0;
    while (var_r29 < 4) {
        temp_r31 = omAddObjEx(lbl_1_bss_10, 0x64, 1U, 0xBU, 0, fn_1_D1C);
        temp_r31->data = HuMemDirectMallocNum(HEAP_HEAP, 0x104, HU_MEMNUM_OVL);
        temp_r31->work[0] = (u32) var_r29;
        temp_r30->player[var_r29] = temp_r31->data;
        var_r29 += 1;
    }
    fn_1_137C(temp_r30);
    Hu3DBGColorSet(0U, 0U, 0U);
    Hu3DNoSyncSet(1);
    MgSeqCreate(&lbl_1_data_0);
}

void fn_1_1FC(s16 mode, s16 frameNo) {
    M538 *temp_r31;

    temp_r31 = lbl_1_bss_C->data;
    fn_1_1958(temp_r31);
    temp_r31->activeF = 0;
    temp_r31->state = 0;
    temp_r31->state2 = 0;
    temp_r31->fireCount = 0;
    temp_r31->roundEndF = 0;
    MgSeqModeNext();
}

void fn_1_264(s16 mode, s16 frameNo) {
    M538 *temp_r31;

    temp_r31 = lbl_1_bss_C->data;
    switch (temp_r31->state) {
    case 0:
        MgSeqModeChangeOff();
        temp_r31->timer = 0;
        temp_r31->state += 1;
        /* fallthrough */
    case 1:
        temp_r31->timer += 1;
        if (temp_r31->timer == 0x3C) {
            temp_r31->timer = 0;
            temp_r31->state += 1;
        }
        break;
    case 2:
        HuAudFXPlay(0x7DD);
        temp_r31->swing.state = 0;
        temp_r31->state += 1;
        /* fallthrough */
    case 3:
        temp_r31->timer += 1;
        fn_1_765C(&temp_r31->swing);
        if (temp_r31->timer == 0xB4) {
            temp_r31->timer = 0;
            temp_r31->state += 1;
        }
        break;
    case 4:
        temp_r31->anim[0].func = fn_1_61F8;
        temp_r31->anim[0].state = 0;
        temp_r31->anim[1].func = fn_1_61F8;
        temp_r31->anim[1].state = 0;
        temp_r31->anim[2].func = fn_1_61F8;
        temp_r31->anim[2].state = 0;
        temp_r31->dolly.state = 0;
        temp_r31->dollyHook = (M538_WORKFUNC) fn_1_A424;
        temp_r31->cam.state = 0;
        HuAudFXPlay(0x7DE);
        lbl_1_bss_4 = HuAudFXPlay(0x7DF);
        lbl_1_bss_0 = HuAudFXPlay(0x7E1);
        temp_r31->state += 1;
        /* fallthrough */
    case 5:
        fn_1_765C(&temp_r31->swing);
        if (fn_1_788C(&temp_r31->cam) != 0) {
            Hu3DModelAttrSet(temp_r31->models[13], 1U);
            Hu3DModelAttrSet(temp_r31->models[14], 1U);
            Hu3DModelAttrSet(temp_r31->models[15], 1U);
            temp_r31->cam.state = 0;
            temp_r31->state += 1;
        }
        break;
    case 6:
        fn_1_765C(&temp_r31->swing);
        lbl_1_bss_8 = HuAudBGMPlay(0x37);
        temp_r31->timer = 0;
        temp_r31->state = 0;
        MgSeqModeNext();
        break;
    }
    fn_1_6918(&temp_r31->anim[0]);
    if ((temp_r31->dollyHook != 0) && (temp_r31->dollyHook(&temp_r31->dolly) != 0)) {
        temp_r31->dollyHook = 0;
    }
}

void fn_1_4A4(s16 mode, s16 frameNo) {
    M538 *temp_r31;

    temp_r31 = lbl_1_bss_C->data;
    fn_1_765C(&temp_r31->swing);
    fn_1_6918(&temp_r31->anim[0]);
    temp_r31->state = 0;
    temp_r31->state2 = 0;
}

void fn_1_4F8(s16 mode, s16 frameNo) {
    M538 *temp_r31;

    temp_r31 = lbl_1_bss_C->data;
    switch (temp_r31->state2) {                              /* irregular */
    case 1:
        break;
    case 0:
        temp_r31->activeF = 1;
        temp_r31->state2 += 1;
        temp_r31->state = 0;
        if ((u32) temp_r31->dollyHook != 0U) {
            temp_r31->dollyHook = 0U;
        }
        temp_r31->anim[0].state += 1;
        temp_r31->anim[1].state += 1;
        temp_r31->anim[2].state += 1;
        break;
    }
    fn_1_7AA0(&temp_r31->cam);
    if (temp_r31->fireCount >= 3) {
        MgSeqModeNext();
        temp_r31->activeF = 0;
        temp_r31->anim[0].flag = 0;
        temp_r31->anim[1].flag = 0;
        temp_r31->anim[2].flag = 0;
    } else if (fn_1_6918(&temp_r31->anim[0]) != 0) {
        MgSeqModeNext();
        temp_r31->activeF = 0;
        temp_r31->anim[0].flag = 0;
        temp_r31->anim[1].flag = 0;
        temp_r31->anim[2].flag = 0;
    }
    fn_1_A8A0(&temp_r31->ctrl);
}

void fn_1_614(s16 mode, s16 frameNo) {
    s16 sp8[4];
    M538 *temp_r31;
    s32 var_r30;
    M538_PLAYER *temp_r29;
    s16 var_r28;
    s16 var_r27;
    s16 temp_r26;
    s32 temp_r25;
    s16 temp_r24;

    temp_r31 = lbl_1_bss_C->data;
    switch (temp_r31->state) {                      /* irregular */
    case 1:
        break;
    case 0:
        var_r28 = 0;
        var_r30 = 0;
        goto loop_6_t;
    loop_6:
        temp_r29 = temp_r31->player[var_r30];
        var_r28 += temp_r29->place;
        var_r30 += 1;
    loop_6_t:
        if (var_r30 < 4) {
            goto loop_6;
        }
        if ((s16) var_r28 == 0) {
            var_r30 = 0;
            goto loop_10_t;
    loop_10:
            temp_r29 = temp_r31->player[var_r30];
            sp8[var_r30] = 3;
            var_r30 += 1;
        loop_10_t:
            if (var_r30 < 4) {
                goto loop_10;
            }
            temp_r31->loseF = 1;
        } else {
            var_r27 = 0;
            var_r28 = 3;
            goto loop_19_t;
    loop_19:
            temp_r26 = var_r27;
            var_r30 = 0;
            goto loop_17_t;
    loop_17:
            temp_r29 = temp_r31->player[var_r30];
            if ((s16) temp_r29->place == (s16) var_r28) {
                sp8[var_r30] = temp_r26;
                var_r27 += 1;
            }
            var_r30 += 1;
        loop_17_t:
            if (var_r30 < 4) {
                goto loop_17;
            }
            var_r28 -= 1;
        loop_19_t:
            if (var_r27 < 4) {
                goto loop_19;
            }
            temp_r31->loseF = 0;
        }
        var_r30 = 0;
        goto loop_27_t;
    loop_27:
        temp_r29 = temp_r31->player[var_r30];
        temp_r24 = sp8[var_r30];
        temp_r25 = temp_r29->playerNo;
        if (_CheckFlag(0x1000FU) == 0) {
            GwPlayer[temp_r25].mgCoinBonus = temp_r24;
        }
        if (temp_r29->active != 0) {
            temp_r29->func = fn_1_7FAC;
            temp_r29->state = 0;
        }
        var_r30 += 1;
    loop_27_t:
        if (var_r30 < 4) {
            goto loop_27;
        }
        temp_r31->state += 1;
        temp_r31->state2 = 0;
        temp_r31->anim[0].func = fn_1_66A4;
        temp_r31->anim[0].state = 0;
        temp_r31->anim[1].func = fn_1_66A4;
        temp_r31->anim[1].state = 0;
        temp_r31->anim[2].func = fn_1_66A4;
        temp_r31->anim[2].state = 0;
        HuAudSStreamFadeOut(lbl_1_bss_8, 0x3E8);
        HuAudFXStop(lbl_1_bss_4);
        HuAudFXStop(lbl_1_bss_0);
        HuAudFXPlay(0x7E0);
        break;
    }
    fn_1_6918(&temp_r31->anim[0]);
}

void fn_1_848(s16 mode, s16 frameNo) {
    s16 temp_r0;
    s32 var_r28;
    s32 var_r29;
    s32 var_r29_2;
    s32 var_r29_3;
    M538_PLAYER *temp_r30;
    M538_PLAYER *temp_r30_2;
    M538 *temp_r31;

    temp_r31 = lbl_1_bss_C->data;
    temp_r0 = temp_r31->state2;
    switch (temp_r0) {                              /* irregular */
    case 0:
        MgSeqModeChangeOff();
        temp_r31->timer = 0;
        temp_r31->state = 0;
        temp_r31->dollyHook = (M538_WORKFUNC) fn_1_A424;
        temp_r31->dolly.state = 2;
        if (fn_1_6D98(temp_r31, (M538_NODE0FUNC) fn_1_B0D0, &temp_r31->fall) != NULL) {
            temp_r31->fall.model = (s16) temp_r31->models[24];
            temp_r31->fall.state = 0;
        }
        temp_r31->state2 = (s16) (temp_r31->state2 + 1);
        temp_r31->cam.state = 0;
        /* fallthrough */
    case 1:
        var_r29 = 0;
loop_18:
        if (var_r29 < 4) {
            temp_r30 = temp_r31->player[var_r29];
            if ((s16) temp_r30->active != 0) {
                if ((void *) temp_r30->func == fn_1_7F04) {
                    goto block_17;
                }
            } else if ((void *) temp_r30->func == fn_1_4428) {
block_17:
                var_r29 += 1;
                goto loop_18;
            }
        }
        if (var_r29 == 4) {
            Hu3DModelAttrReset(temp_r31->models[13], 1U);
            Hu3DModelAttrReset(temp_r31->models[14], 1U);
            Hu3DModelAttrReset(temp_r31->models[15], 1U);
            Hu3DModelAttrReset(temp_r31->models[28], 1U);
            temp_r31->swing.state = 0;
            temp_r31->state2 = (s16) (temp_r31->state2 + 1);
        }
        break;
    case 2:
        temp_r31->timer = (s16) (temp_r31->timer + 1);
        fn_1_77A4(&temp_r31->swing);
        fn_1_7CB0(&temp_r31->cam);
        if ((s16) temp_r31->timer == 0x78) {
            temp_r31->timer = 0;
            temp_r31->state2 = (s16) (temp_r31->state2 + 1);
        }
        break;
    case 3:
        fn_1_A368(temp_r31);
        fn_1_77A4(&temp_r31->swing);
        if (fn_1_7CB0(&temp_r31->cam) != 0) {
            temp_r31->state2 = (s16) (temp_r31->state2 + 1);
        }
        break;
    case 4:
        fn_1_A368(temp_r31);
        fn_1_77A4(&temp_r31->swing);
        var_r29_2 = 0;
loop_29:
        if (var_r29_2 < 4) {
            temp_r30_2 = temp_r31->player[var_r29_2];
            if ((s16) temp_r30_2->active == 0) {
                temp_r30_2->func = fn_1_8554;
                temp_r30_2->state = 0;
                temp_r30_2->counter3 = (s16) ((temp_r30_2->place * 0x14) + 0x1E);
            }
            var_r29_2 += 1;
            goto loop_29;
        }
        Hu3DModelAttrReset(temp_r31->models[13], 0x40000002U);
        Hu3DModelAttrReset(temp_r31->models[14], 0x40000002U);
        Hu3DModelAttrReset(temp_r31->models[16], 0x40000002U);
        Hu3DMotionSpeedSet(temp_r31->models[13], 0.5f);
        Hu3DMotionSpeedSet(temp_r31->models[14], 0.5f);
        Hu3DMotionSpeedSet(temp_r31->models[16], 0.5f);
        temp_r31->state2 = (s16) (temp_r31->state2 + 1);
        /* fallthrough */
    case 5:
        fn_1_A368(temp_r31);
        fn_1_77A4(&temp_r31->swing);
        var_r28 = 0;
        var_r29_3 = 0;
loop_35:
        if (var_r29_3 < 4) {
            if ((u32) temp_r31->player[var_r29_3]->func == 0U) {
                var_r28 += 1;
            }
            var_r29_3 += 1;
            goto loop_35;
        }
        if (var_r28 >= 3) {
            temp_r31->state = 0;
            temp_r31->state2 = 0;
            MgSeqModeNext();
        }
        break;
    }
    fn_1_6918(&temp_r31->anim[0]);
    if (((temp_r31->dollyHook) != NULL) && (temp_r31->dollyHook(&temp_r31->dolly) != 0)) {
        temp_r31->dollyHook = NULL;
    }
}

void fn_1_BB4(s16 mode, s16 frameNo) {
    s32 var_r29;
    M538_PLAYER *temp_r30;
    M538 *temp_r31;

    temp_r31 = lbl_1_bss_C->data;
    switch (temp_r31->state) {                              /* irregular */
    case 0:
        temp_r31->roundEndF = 1;
        var_r29 = 0;
        goto loop_9;
loop_8:
        temp_r30 = temp_r31->player[var_r29];
        if (temp_r30->active != 0) {
            temp_r30->state = temp_r30->state + 1;
        }
        CharModelVoicePanSet(temp_r30->charNo, 0x40);
        CharModelVoiceVolSet(temp_r30->charNo, 0x73);
        var_r29 += 1;
loop_9:
        if (var_r29 < 4) {
            goto loop_8;
        }
        if (temp_r31->loseF != 0) {
            HuAudJinglePlay(0x7B);
            CharLoseVoicePlay(temp_r31->player[0]->charNo, temp_r31->player[1]->charNo, temp_r31->player[2]->charNo, temp_r31->player[3]->charNo);
        } else {
            HuAudJinglePlay(0x79);
        }
        temp_r31->state += 1;
        /* fallthrough */
    case 1:
        fn_1_A368(temp_r31);
    }
}

void fn_1_CC0(s16 mode, s16 frameNo) {
    void *temp_r31 = lbl_1_bss_C->data;

    fn_1_A368(temp_r31);
}

void fn_1_CFC(s16 mode, s16 frameNo) {
    void *sp8;

    sp8 = lbl_1_bss_C->data;
}

void fn_1_D1C(OMOBJ *obj) {
    M538_PLAYER *temp_r31;
    void *temp_r28;
    s32 var_r29;

    temp_r31 = obj->data;
    temp_r28 = lbl_1_bss_C->data;
    temp_r31->playerNo = (s16) obj->work[0];
    temp_r31->charNo = GwPlayerConf[temp_r31->playerNo].charNo;
    temp_r31->grp = GwPlayerConf[temp_r31->playerNo].grp;
    temp_r31->padNo = GwPlayerConf[temp_r31->playerNo].padNo;
    temp_r31->type = GwPlayerConf[temp_r31->playerNo].type;
    temp_r31->dif = GwPlayerConf[temp_r31->playerNo].dif;
    temp_r31->obj = obj;
    ((M538 *) temp_r28)->player[temp_r31->playerNo] = temp_r31;
    temp_r31->accX = 0.0f;
    temp_r31->accY = 0.0f;
    temp_r31->accZ = 0.0f;
    fn_1_4534(temp_r31);
    temp_r31->func = fn_1_2CB8;
    temp_r31->state = 0;
    temp_r31->counter = 0;
    temp_r31->flag = 0;
    temp_r31->active = 1;
    temp_r31->rotSpeed = 30.0f;
    *obj->mdlId = CharModelCreate(temp_r31->charNo, 8);
    temp_r31->modelId = *obj->mdlId;
    temp_r31->voiceHook = 0;
    temp_r31->fwModel = temp_r31->modelId;
    temp_r31->pad7E = 0;
    temp_r31->place = 3;
    var_r29 = 0;
    goto loop_2_t;
loop_2:
    obj->mtnId[var_r29] = CharMotionCreate(temp_r31->charNo, m538_charMotion[temp_r31->charNo][var_r29]);
    var_r29 += 1;
loop_2_t:
    if (var_r29 < 0xB) {
        goto loop_2;
    }
    CharMotionSet(temp_r31->charNo, *obj->mtnId);
    Hu3DModelAttrSet(*obj->mdlId, 0x40000001U);
    Hu3DModelLayerSet(*obj->mdlId, 2);
    Hu3DModelShadowSet(*obj->mdlId);
    CharEffectLayerSet(4);
    CharMotionDataClose(temp_r31->charNo);
    fn_1_6AE4(temp_r31);
    temp_r31->pool1 = &((M538 *) temp_r28)->pool1;
    temp_r31->pool2 = &((M538 *) temp_r28)->pool2;
    temp_r31->aiState = 0;
    temp_r31->aiFlag = 0;
    temp_r31->aiTarget = 0;
    CharMotionVoiceOnSet(temp_r31->charNo, 0x2B, 0);
    CharMotionVoiceOnSet(temp_r31->charNo, 0x5C, 0);
    CharMotionVoiceOnSet(temp_r31->charNo, 0x5D, 0);
    obj->objFunc = fn_1_FD0;
}

void fn_1_FD0(OMOBJ *obj) {
    f32 temp_f31;
    f32 temp_f30;
    f32 temp_f29;
    M538 *temp_r30;
    M538_PLAYER *temp_r31;

    temp_r30 = lbl_1_bss_C->data;
    temp_r31 = obj->data;
    temp_f31 = temp_r30->anim[1].scale;
    if (temp_r30->roundEndF == 0) {
        temp_f30 = 64.0f + (32.0f * (temp_r31->x / temp_f31));
        temp_f29 = 115.5f + (11.5f * (temp_r31->z / temp_f31));
        temp_r31->voicePan = (s16) temp_f30;
        temp_r31->voiceVol = (s16) temp_f29;
        if (temp_r31->voicePan < 0x20) {
            temp_r31->voicePan = 0x20;
        }
        if (temp_r31->voicePan > 0x60) {
            temp_r31->voicePan = 0x60;
        }
        if (temp_r31->voiceVol < 0x68) {
            temp_r31->voiceVol = 0x68;
        }
        if (temp_r31->voiceVol > 0x7F) {
            temp_r31->voiceVol = 0x7F;
        }
        CharModelVoicePanSet(temp_r31->charNo, temp_r31->voicePan);
        CharModelVoiceVolSet(temp_r31->charNo, temp_r31->voiceVol);
    }
    fn_1_4534(temp_r31);
    if (((temp_r31->func) != 0) && (temp_r31->func(temp_r31) != 0)) {
        temp_r31->func = 0;
    }
    if (((temp_r31->voiceHook) != 0) && (temp_r31->voiceHook(&temp_r31->fwModel) != 0)) {
        temp_r31->voiceHook = 0;
    }
}

void fn_1_1194(void *arg0) {

}

void fn_1_1198(OMOBJ *obj) {
    M538 *temp_r31;

    temp_r31 = lbl_1_bss_C->data;
    fn_1_56A4(temp_r31);
    fn_1_5FC4(&temp_r31->anim[0], 0);
    fn_1_5FC4(&temp_r31->anim[1], 1);
    fn_1_5FC4(&temp_r31->anim[2], 2);
    fn_1_1650(&temp_r31->cam);
    fn_1_72CC(&temp_r31->swing);
    temp_r31->cam.x = (f32) lbl_1_data_2E4[0];
    temp_r31->cam.y = (f32) lbl_1_data_2E4[1];
    temp_r31->cam.z = (f32) lbl_1_data_2E4[2];
    temp_r31->cam.theta = (f32) lbl_1_data_2E4[3];
    temp_r31->cam.phi = (f32) lbl_1_data_2E4[4];
    temp_r31->cam.radius = (f32) lbl_1_data_2E4[5];
    fn_1_6CD4(temp_r31);
    fn_1_8A00(temp_r31);
    fn_1_8B6C(temp_r31);
    fn_1_93C4(temp_r31);
    fn_1_87E4(&temp_r31->pool1, (M538_NODE *) &temp_r31->node1, 0x10);
    fn_1_87E4(&temp_r31->pool2, (M538_NODE *) &temp_r31->node2, 0x10);
    fn_1_87E4(&temp_r31->pool3, (M538_NODE *) &temp_r31->node3, 0x40);
    temp_r31->tick = 0;
    temp_r31->dolly.state = 0;
    temp_r31->dollyHook = 0;
    fn_1_A6F8(temp_r31);
    obj->objFunc = fn_1_12F0;
}

void fn_1_12F0(OMOBJ *obj) {
    M538 *temp_r31;

    temp_r31 = lbl_1_bss_C->data;
    fn_1_16BC(&temp_r31->cam);
    fn_1_4C38(temp_r31);
    fn_1_4F74(temp_r31);
    if ((s32) temp_r31->anim[1].flag != 0) {
        fn_1_4968(temp_r31);
    }
    fn_1_4714(temp_r31);
    fn_1_6E50(temp_r31);
    fn_1_8920(&temp_r31->pool1);
    fn_1_8920(&temp_r31->pool2);
    fn_1_8920(&temp_r31->pool3);
}

void fn_1_1378(void *arg0) {

}

void fn_1_137C(M538 *arg0) {
    s32 i;

    for (i = 0; i < 0x1D; i++) {
        arg0->models[i] = -1;
    }
    for (i = 0; i < 3; i++) {
        arg0->camModels[i] = -1;
    }
}

void fn_1_13D8(M538 *arg0) {
    s16 temp_r3;

    temp_r3 = Hu3DGLightCreate(0.0f, 4800.0f, 3600.0f, 0.0f, -1.0f, -1.0f, 0x7FU, 0x7FU, 0x7FU);
    Hu3DGLightInfinitytSet(temp_r3);
    arg0->lightId = temp_r3;
    Hu3DGLightColorSet(arg0->lightId, 0x7FU, 0x7FU, 0x7FU, 0U);
}

void fn_1_1488(void *arg0) {
    Hu3DCameraCreate((s32) lbl_1_rodata_10[0]);
    Hu3DCameraPerspectiveSet((s32) lbl_1_rodata_10[0], 15.0f, 20.0f, 20000.0f, 1.2f);
    /* nine floats: the ninth (targetZ) rides the outgoing-argument slot at
     * 8(sp), which is why the original stores a constant there. */
    Hu3DCameraPosSet((s32) lbl_1_rodata_10[0], 0.0f, 300.0f, 1000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 150.0f, 0.0f);
}

void fn_1_1574(void *arg0) {
    struct m538_raw3_s { s32 a; s32 b; s32 c; };
    struct m538_raw3_s sp20;
    struct m538_raw3_s sp14;
    struct m538_raw3_s sp8;

    sp20 = *(struct m538_raw3_s *) &lbl_1_rodata_60[0];
    sp14 = *(struct m538_raw3_s *) &lbl_1_rodata_6C[0];
    sp8 = *(struct m538_raw3_s *) &lbl_1_rodata_78[0];
    Hu3DShadowCreate(15.0f, 20.0f, 20000.0f);
    Hu3DShadowTPLvlSet(0.4f);
    Hu3DShadowPosSet((Point3d *) &sp14, (Point3d *) &sp20, (Point3d *) &sp8);
    Hu3DShadowSizeSet(0x80U);
    Hu3DShadowColSet(0xFU, 0xFU, 0xFU);
}

void fn_1_1650(M538_CAMWORK *arg0) {
    arg0->theta = 0.0f;
    arg0->phi = 0.0f;
    arg0->radius = 1000.0f;
    arg0->x = 0.0f;
    arg0->y = 0.0f;
    arg0->z = 0.0f;
    arg0->state = 0;
}

void fn_1_16BC(M538_CAMWORK *arg0) {
    f32 temp_f31;
    f32 temp_f30;
    f32 temp_f29;
    f64 temp_f28;

    /* The original's four wrap loops are ROTATED while-loops: the prologue
     * branches over the body to a latch at the bottom.  A guarded `while` is
     * the faithful form and emits exactly that rotation.
     *
     * The two `+= 360` bodies load the struct field into f1 before the float
     * literal, which is the order a `= a + b` assignment does NOT give: MWCC
     * evaluates a commutative fadd's literal operand first and hoists the
     * rodata lis/addi above both loads.  Writing the same update as a compound
     * assignment (`x += 360.0f`) makes MWCC evaluate left-to-right and matches
     * the original byte for byte.  The subtraction loops already do so.
     *
     * NONMATCHING: 13 rows, all a float-register rename.  The original keeps
     * the f64 temporary in f28 and the three f32 results in f31/f30/f29;
     * MWCC always hands f31 to an f64 local when three f32 locals share the
     * frame.  Five formulations tried, all 13: f64 declared first, f64 declared
     * last, `register f64`, `register volatile f64` (28), f64 in an inner block,
     * and the f64 passed through an inlined helper (51). */
    while (arg0->theta > 360.0f) {
        arg0->theta = arg0->theta - 360.0f;
    }
    while (arg0->theta < 0.0f) {
        arg0->theta += 360.0f;
    }
    while (arg0->phi > 360.0f) {
        arg0->phi = arg0->phi - 360.0f;
    }
    while (arg0->phi < 0.0f) {
        arg0->phi += 360.0f;
    }
    temp_f28 = cos((3.141592653589793 * (f64) arg0->theta) / 180.0);
    temp_f31 = (f32) ((f64) arg0->x + ((f64) arg0->radius * cos((3.141592653589793 * (f64) arg0->phi) / 180.0) * temp_f28));
    temp_f30 = (f32) ((f64) arg0->y + ((f64) arg0->radius * sin((3.141592653589793 * (f64) arg0->phi) / 180.0)));
    temp_f28 = sin((3.141592653589793 * (f64) arg0->theta) / 180.0);
    temp_f29 = (f32) ((f64) arg0->z + ((f64) arg0->radius * cos((3.141592653589793 * (f64) arg0->phi) / 180.0) * temp_f28));
    Hu3DCameraPosSet((s32) lbl_1_rodata_10[0], temp_f31, temp_f30, temp_f29, 0.0f, 1.0f, 0.0f, arg0->x, arg0->y, arg0->z);
}

void fn_1_1958(M538 *arg0) {
    f32 temp_f28;
    f32 temp_f29;
    f32 temp_f30;
    f32 var_f31;
    s32 var_r30;
    u32 temp_r3;
    M538_PLAYER *temp_r31;

    var_r30 = 0;
loop_7:
    if (var_r30 < 4) {
        temp_r31 = arg0->player[var_r30];
        temp_r3 = (u32) var_r30 >> 0x1FU;
        temp_r31->x = (f32) ((300.0f * (f32) (((var_r30 & 1) ^ temp_r3) - temp_r3)) - 150.0f);
        temp_r31->y = 0.0f;
        temp_r31->z = (f32) ((300.0f * (f32) ((s32) (((u32) var_r30 >> 0x1FU) + var_r30) >> 1)) - 150.0f);
        temp_f28 = -temp_r31->z;
        temp_f29 = -temp_r31->x;
        if (temp_f28 == 0.0f) {
            if (temp_f29 >= 0.0f) {
                var_f31 = 1.5707964f;
            } else {
                var_f31 = -1.5707964f;
            }
        } else {
            var_f31 = (f32) atan2((f64) temp_f29, (f64) temp_f28);
        }
        temp_f30 = (f32) ((f64) (180.0f * var_f31) / 3.141592653589793);
        temp_r31->targetRot = temp_f30;
        temp_r31->rotX = 0.0f;
        temp_r31->rotY = temp_f30;
        temp_r31->rotZ = 0.0f;
        omSetTra(temp_r31->obj, temp_r31->x, temp_r31->y, temp_r31->z);
        omSetRot(temp_r31->obj, temp_r31->rotX, temp_r31->rotY, temp_r31->rotZ);
        var_r30 += 1;
        goto loop_7;
    }
}

s32 fn_1_1B94(M538_PLAYER *arg0) {
    f32 temp_f31;
    f32 temp_f30;
    f32 var_f29;
    f32 var_f28;
    f32 var_f27;
    f32 temp_f26;
    f32 temp_f25;
    s32 var_r30;
    M538 *var_r29;

    var_r30 = 0;
    var_r29 = lbl_1_bss_C->data;
    if (var_r29->activeF == 0) {
        return 0;
    }
    temp_f31 = (f32) (s8) HuPadStkX[arg0->padNo];
    temp_f30 = (f32) -(s8) HuPadStkY[arg0->padNo];
    if (temp_f30 < 0.0f) {
        var_f28 = -temp_f30;
    } else {
        var_f28 = temp_f30;
    }
    if (temp_f31 < 0.0f) {
        var_f27 = -temp_f31;
    } else {
        var_f27 = temp_f31;
    }
    temp_f26 = var_f27 + var_f28;
    if (temp_f26 > 32.0f) {
        if (temp_f30 == 0.0f) {
            if (temp_f31 >= 0.0f) {
                var_f29 = 1.5707964f;
            } else {
                var_f29 = -1.5707964f;
            }
        } else {
            temp_f25 = (f32) atan2((f64) temp_f31, (f64) temp_f30);
            var_f29 = temp_f25;
        }
        arg0->targetRot = (f32) ((180.0f * var_f29) / 3.141592653589793);
        var_r30 = 1;
    }
    return var_r30;
}

s32 fn_1_1DC8(M538_PLAYER *arg0) {
    u16 sp66;
    u16 sp64;
    u16 sp62;
    u16 sp60;
    u16 sp5E;
    u16 sp5C;
    u16 sp5A;
    u16 sp58;
    u16 sp56;
    u16 sp54;
    u16 sp52;
    u16 sp50;
    u16 sp4E;
    u16 sp4C;
    u16 sp4A;
    u16 sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    f32 sp34;
    f32 sp30;
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    f32 sp20;
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    f32 sp8;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f0_5;
    f32 temp_f0_6;
    f32 temp_f18;
    f32 temp_f19;
    f32 temp_f20;
    f32 temp_f21;
    f32 temp_f22;
    f32 temp_f23;
    f32 temp_f31;
    f32 var_f24;
    f32 var_f25;
    f32 var_f26;
    f32 var_f27;
    f32 var_f28;
    f32 var_f29;
    f32 var_f30;
    s16 temp_r0;
    s32 var_r24;
    s32 var_r25;
    s32 var_r26;
    s32 var_r28;
    s32 var_r28_2;
    s32 var_r28_3;
    u32 var_r23;
    M538 *temp_r29;
    M538_PLAYER *temp_r31;
    M538_PLAYER *var_r27;

    var_r26 = 0;
    sp60 = lbl_1_rodata_B8[0];
    sp62 = lbl_1_rodata_B8[0];
    sp64 = lbl_1_rodata_B8[1];
    sp66 = lbl_1_rodata_B8[1];
    sp58 = lbl_1_rodata_C0[0];
    sp5A = lbl_1_rodata_C0[0];
    sp5C = lbl_1_rodata_C0[1];
    sp5E = lbl_1_rodata_C0[1];
    sp50 = lbl_1_rodata_C8[0];
    sp52 = lbl_1_rodata_C8[0];
    sp54 = lbl_1_rodata_C8[1];
    sp56 = lbl_1_rodata_C8[1];
    sp48 = lbl_1_rodata_D0[0];
    sp4A = lbl_1_rodata_D0[0];
    sp4C = lbl_1_rodata_D0[1];
    sp4E = lbl_1_rodata_D0[1];
    temp_r29 = lbl_1_bss_C->data;
    if ((s16) temp_r29->activeF == 0) {
        return 0;
    }
    temp_r31 = arg0;
    temp_r0 = temp_r31->y;
    switch (temp_r0) {                              /* irregular */
    case 0:
        arg0->aiTarget = NULL;
        temp_r31->aiFlag = 0;
        temp_r31->y += 1;
        /* fallthrough */
    case 1:
        temp_f31 = (arg0->x * arg0->x) + (arg0->z * arg0->z);
        temp_f0 = (temp_r29->lightRadius - 200.0f) * (temp_r29->lightRadius - 200.0f);
        sp44 = temp_f0;
        if ((temp_f31 > temp_f0) && (frandmod(0x64U) < (s16) (&sp60)[arg0->dif])) {
            temp_f19 = -arg0->z;
            temp_f20 = -arg0->x;
            if (temp_f19 == 0.0f) {
                M2C_ERROR(/* unknown instruction: cror eq, gt, eq */);
                if (temp_f20 == 0.0f) {
                    var_f29 = 1.5707964f;
                } else {
                    var_f29 = -1.5707964f;
                }
            } else {
                sp1C = (f32) atan2((f64) temp_f20, (f64) temp_f19);
                var_f29 = sp1C;
            }
            arg0->targetRot = (f32) (((f64) (180.0f * var_f29) / 3.141592653589793) + (f64) (20.0f * (((f32) frandmod(0x2711U) / 10000.0f) - 0.5f)));
            temp_r31->aiTimer = 0;
            temp_r31->z = (s16) (frandmod(0xAU) + 0xA);
            temp_r31->y = 2;
            var_r26 = 1;
        } else {
            var_f30 = 2250000.0f;
            var_r27 = NULL;
            var_r28 = 0;
loop_27:
            if (var_r28 < 4) {
                if ((var_r28 != (s16) arg0->playerNo) && ((s16) temp_r29->player[var_r28]->active != 0) && ((s16) temp_r29->player[var_r28]->flag == 0)) {
                    temp_f22 = arg0->x - temp_r29->player[var_r28]->x;
                    temp_f21 = arg0->z - temp_r29->player[var_r28]->z;
                    temp_f23 = (temp_f22 * temp_f22) + (temp_f21 * temp_f21);
                    if (temp_f23 < var_f30) {
                        var_f30 = temp_f23;
                        var_r27 = temp_r29->player[var_r28];
                    }
                }
                var_r28 += 1;
                goto loop_27;
            }
            if ((var_f30 < 160000.0f) && (var_r27 != NULL)) {
                if (frandmod(0x64U) < (s16) (&sp58)[arg0->dif]) {
                    temp_r31->aiFlag = 1;
                    arg0->aiTarget = var_r27;
                    temp_f0_2 = var_r27->z - arg0->z;
                    sp40 = temp_f0_2;
                    temp_f18 = var_r27->x - arg0->x;
                    if (temp_f0_2 == 0.0f) {
                        M2C_ERROR(/* unknown instruction: cror eq, gt, eq */);
                        if (temp_f18 == 0.0f) {
                            var_f28 = 1.5707964f;
                        } else {
                            var_f28 = -1.5707964f;
                        }
                    } else {
                        sp18 = (f32) atan2((f64) temp_f18, (f64) sp40);
                        var_f28 = sp18;
                    }
                    arg0->targetRot = (f32) ((f64) (180.0f * var_f28) / 3.141592653589793);
                    temp_r31->aiTimer = 0;
                    temp_r31->z = (s16) (frandmod(0xAU) + 0xA);
                    temp_r31->y = 3;
                    var_r26 = 1;
                } else {
                    arg0->aiTarget = var_r27;
                    temp_f0_3 = arg0->z - var_r27->z;
                    sp38 = temp_f0_3;
                    sp3C = arg0->x - var_r27->x;
                    if (temp_f0_3 == 0.0f) {
                        M2C_ERROR(/* unknown instruction: cror eq, gt, eq */);
                        if (sp3C == 0.0f) {
                            var_f27 = 1.5707964f;
                        } else {
                            var_f27 = -1.5707964f;
                        }
                    } else {
                        sp14 = (f32) atan2((f64) sp3C, (f64) sp38);
                        var_f27 = sp14;
                    }
                    temp_f0_4 = -arg0->z;
                    sp30 = temp_f0_4;
                    sp34 = -arg0->x;
                    if (temp_f0_4 == 0.0f) {
                        M2C_ERROR(/* unknown instruction: cror eq, gt, eq */);
                        if (sp34 == 0.0f) {
                            var_f26 = 1.5707964f;
                        } else {
                            var_f26 = -1.5707964f;
                        }
                    } else {
                        sp10 = (f32) atan2((f64) sp34, (f64) sp30);
                        var_f26 = sp10;
                    }
                    arg0->targetRot = (f32) (((f64) (180.0f * var_f27) / 3.141592653589793) + ((f64) (180.0f * var_f26) / 3.141592653589793));
                    temp_r31->aiTimer = 0;
                    temp_r31->z = (s16) (frandmod(0xAU) + 0xA);
                    temp_r31->y = 2;
                    var_r26 = 1;
                }
            } else if (frandmod(0x64U) < (s16) (&sp48)[arg0->dif]) {
                if (temp_f31 > 40000.0f) {
                    temp_f0_5 = -arg0->z;
                    sp28 = temp_f0_5;
                    sp2C = -arg0->x;
                    if (temp_f0_5 == 0.0f) {
                        M2C_ERROR(/* unknown instruction: cror eq, gt, eq */);
                        if (sp2C == 0.0f) {
                            var_f25 = 1.5707964f;
                        } else {
                            var_f25 = -1.5707964f;
                        }
                    } else {
                        spC = (f32) atan2((f64) sp2C, (f64) sp28);
                        var_f25 = spC;
                    }
                    arg0->targetRot = (f32) (((f64) (180.0f * var_f25) / 3.141592653589793) + (f64) (20.0f * (((f32) frandmod(0x2711U) / 10000.0f) - 0.5f)));
                    temp_r31->aiTimer = 0;
                    temp_r31->z = (s16) (frandmod(0xAU) + 0xA);
                    temp_r31->y = 5;
                    var_r26 = 1;
                } else if (var_r27 != NULL) {
                    temp_r31->aiFlag = 1;
                    arg0->aiTarget = var_r27;
                    temp_r31->aiTimer = 0;
                    temp_r31->z = (s16) (frandmod(3U) + 3);
                    temp_r31->y = 3;
                } else {
                    temp_r31->aiTimer = 0;
                    temp_r31->z = (s16) (frandmod(3U) + 3);
                    temp_r31->y = 4;
                }
            } else if (frandmod(0x64U) < (s16) (&sp50)[arg0->dif]) {
                temp_r31->aiTimer = 0;
                temp_r31->z = (s16) (frandmod(0xAU) + 0xA);
                temp_r31->y = 4;
            } else {
                var_r25 = 0;
                var_r28_2 = 0;
loop_66:
                if (var_r28_2 < 4) {
                    if ((var_r28_2 != (s16) arg0->playerNo) && ((s16) temp_r29->player[var_r28_2]->active != 0)) {
                        var_r25 += 1;
                    }
                    var_r28_2 += 1;
                    goto loop_66;
                }
                if (var_r25 > 1) {
                    var_r23 = frandmod((u32) var_r25);
                } else {
                    var_r23 = 0;
                }
                var_r24 = 0;
                var_r28_3 = 0;
loop_77:
                if (var_r28_3 < 4) {
                    if ((var_r28_3 != (s16) arg0->playerNo) && ((s16) temp_r29->player[var_r28_3]->active != 0)) {
                        if ((s32) var_r23 == var_r24) {
                            var_r27 = temp_r29->player[var_r28_3];
                        } else {
                            var_r24 += 1;
                            goto block_76;
                        }
                    } else {
block_76:
                        var_r28_3 += 1;
                        goto loop_77;
                    }
                }
                temp_r31->aiFlag = 1;
                arg0->aiTarget = var_r27;
                temp_f0_6 = var_r27->z - arg0->z;
                sp20 = temp_f0_6;
                sp24 = var_r27->x - arg0->x;
                if (temp_f0_6 == 0.0f) {
                    M2C_ERROR(/* unknown instruction: cror eq, gt, eq */);
                    if (sp24 == 0.0f) {
                        var_f24 = 1.5707964f;
                    } else {
                        var_f24 = -1.5707964f;
                    }
                } else {
                    sp8 = (f32) atan2((f64) sp24, (f64) sp20);
                    var_f24 = sp8;
                }
                arg0->targetRot = (f32) ((f64) (180.0f * var_f24) / 3.141592653589793);
                temp_r31->aiTimer = 0;
                temp_r31->z = (s16) (frandmod(0xAU) + 0xA);
                temp_r31->y = 3;
                var_r26 = 1;
            }
        }
        break;
    case 2:
        temp_r31->aiTimer += 1;
        if ((s16) temp_r31->aiTimer == (s16) temp_r31->z) {
            temp_r31->y = 0;
        }
        var_r26 = 1;
        break;
    case 3:
        temp_r31->aiTimer += 1;
        if (((s16) temp_r31->aiTimer == (s16) temp_r31->z) || ((s16) temp_r31->aiFlag == 0)) {
            temp_r31->y = 0;
        }
        var_r26 = 1;
        break;
    case 4:
        temp_r31->aiTimer += 1;
        if ((s16) temp_r31->aiTimer == (s16) temp_r31->z) {
            temp_r31->y = 0;
        }
        break;
    case 5:
        temp_r31->aiTimer += 1;
        if (((s16) temp_r31->aiTimer == (s16) temp_r31->z) || (((arg0->x * arg0->x) + (arg0->z * arg0->z)) < 40000.0f)) {
            temp_r31->y = 0;
        }
        var_r26 = 1;
        break;
    }
    return var_r26;
}

s32 fn_1_2988(M538_PLAYER *arg0) {
    s32 var_r31;
    M538 *temp_r30;

    var_r31 = 0;
    temp_r30 = (M538 *) lbl_1_bss_C->data;
    if (temp_r30->activeF == 0) {
        return 0;
    }
    if ((s32) (HuPadBtnDown[arg0->padNo] & 0x100) != 0) {
        var_r31 = 1;
    }
    return var_r31;
}

s32 fn_1_29F8(M538_PLAYER *arg0) {
    s32 sp24;
    s32 sp20;
    s32 sp1C;
    f32 sp18;
    s32 sp14;
    s32 sp10;
    s32 spC;
    s32 sp8;
    f32 temp_f29;
    f32 temp_f30;
    f32 var_f28;
    f32 var_f31;
    s32 var_r29;
    M538_PLAYER *temp_r31;

    var_r29 = 0;
    sp18 = lbl_1_rodata_EC[0];
    sp1C = lbl_1_rodata_EC[1];
    sp20 = lbl_1_rodata_EC[2];
    sp24 = lbl_1_rodata_EC[3];
    sp8 = lbl_1_rodata_FC[0];
    spC = lbl_1_rodata_FC[1];
    sp10 = lbl_1_rodata_FC[2];
    sp14 = lbl_1_rodata_FC[3];
    if ((s16) ((M538 *) lbl_1_bss_C->data)->activeF == 0) {
        return 0;
    }
    temp_r31 = arg0;
    if (((s16) temp_r31->aiFlag != 0) && (arg0->aiTarget != NULL)) {
        temp_f30 = arg0->aiTarget->x - arg0->x;
        temp_f29 = arg0->aiTarget->z - arg0->z;
        if (((temp_f30 * temp_f30) + (temp_f29 * temp_f29)) < (&sp18)[arg0->dif]) {
            if (temp_f29 == 0.0f) {
                M2C_ERROR(/* unknown instruction: cror eq, gt, eq */);
                if (temp_f30 == 0.0f) {
                    var_f28 = 1.5707964f;
                } else {
                    var_f28 = -1.5707964f;
                }
            } else {
                var_f28 = (f32) atan2((f64) temp_f30, (f64) temp_f29);
            }
            var_f31 = (f32) (((f64) (180.0f * var_f28) / 3.141592653589793) - (f64) arg0->targetRot);
loop_12:
            if (var_f31 > 180.0f) {
                var_f31 -= 360.0f;
                goto loop_12;
            }
loop_15:
            if (var_f31 < -180.0f) {
                var_f31 += 360.0f;
                goto loop_15;
            }
            if (((f32) fabs(var_f31) < 90.0f) && (frandmod(0x64U) < (s32) (&sp8)[arg0->dif])) {
                temp_r31->aiFlag = 0;
                var_r29 = 1;
            }
        }
    }
    return var_r29;
}

s32 fn_1_2CB8(M538_PLAYER *arg0) {
    s32 sp44;
    s32 sp40;
    s32 sp3C;
    s32 sp38;
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    f32 sp28;
    f64 sp20;
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    f32 sp8;
    f32 temp_f0_2;
    f32 temp_f27;
    f32 temp_f28;
    f32 temp_f29;
    f32 temp_f30;
    f32 var_f19;
    f32 var_f20;
    f32 var_f25;
    f32 var_f26;
    f32 var_f31;
    f64 temp_f0;
    s32 temp_r0;
    s32 var_r20;
    s32 var_r22;
    s32 var_r23;
    s32 var_r25;
    s32 var_r26;
    s32 var_r28;
    s32 var_r29;
    s32 var_r29_2;
    M538_PLAYER *temp_r30;

    temp_r0 = arg0->state;
    switch (temp_r0) {                              /* irregular */
    case 0:
        arg0->flag = 0;
        CharMotionShiftSet(arg0->charNo, *arg0->obj->mtnId, 0.0f, 5.0f, 0x40000001U);
        arg0->state += 1;
        /* fallthrough */
    case 1:
        if ((s16) arg0->type != 0) {
            var_r26 = 0;
            sp28 = lbl_1_rodata_EC[0];
            sp2C = lbl_1_rodata_EC[1];
            sp30 = lbl_1_rodata_EC[2];
            sp34 = lbl_1_rodata_EC[3];
            sp38 = lbl_1_rodata_FC[0];
            sp3C = lbl_1_rodata_FC[1];
            sp40 = lbl_1_rodata_FC[2];
            sp44 = lbl_1_rodata_FC[3];
            if ((s16) ((M538 *) lbl_1_bss_C->data)->activeF == 0) {
                var_r28 = 0;
            } else {
                temp_r30 = arg0;
                if (((s16) temp_r30->aiFlag != 0) && (arg0->aiTarget != NULL)) {
                    temp_f27 = arg0->aiTarget->x - arg0->x;
                    temp_f28 = arg0->aiTarget->z - arg0->z;
                    if (((temp_f27 * temp_f27) + (temp_f28 * temp_f28)) < (&sp28)[arg0->dif]) {
                        if (temp_f28 == 0.0f) {
                            M2C_ERROR(/* unknown instruction: cror eq, gt, eq */);
                            if (temp_f27 == 0.0f) {
                                var_f26 = 1.5707964f;
                            } else {
                                var_f26 = -1.5707964f;
                            }
                        } else {
                            sp14 = (f32) atan2((f64) temp_f27, (f64) temp_f28);
                            var_f26 = sp14;
                        }
                        var_f31 = (f32) (((f64) (180.0f * var_f26) / 3.141592653589793) - (f64) arg0->targetRot);
loop_18:
                        if (var_f31 > 180.0f) {
                            var_f31 -= 360.0f;
                            goto loop_18;
                        }
loop_21:
                        if (var_f31 < -180.0f) {
                            var_f31 += 360.0f;
                            goto loop_21;
                        }
                        sp18 = var_f31;
                        temp_f0 = fabs(sp18);
                        sp20 = temp_f0;
                        temp_f0_2 = (f32) temp_f0;
                        sp10 = temp_f0_2;
                        spC = temp_f0_2;
                        if ((spC < 90.0f) && (frandmod(0x64U) < (s32) (&sp38)[arg0->dif])) {
                            temp_r30->aiFlag = 0;
                            var_r26 = 1;
                        }
                    }
                }
                var_r28 = var_r26;
            }
            var_r29 = var_r28;
        } else {
            var_r23 = 0;
            if ((s16) ((M538 *) lbl_1_bss_C->data)->activeF == 0) {
                var_r25 = 0;
            } else {
                if ((s32) (HuPadBtnDown[arg0->padNo] & 0x100) != 0) {
                    var_r23 = 1;
                }
                var_r25 = var_r23;
            }
            var_r29 = var_r25;
        }
        if (var_r29 != 0) {
            arg0->func = fn_1_39DC;
            arg0->state = 0;
        } else {
            if ((s16) arg0->type != 0) {
                var_r29_2 = fn_1_1DC8(arg0);
            } else {
                var_r20 = 0;
                if ((s16) ((M538 *) lbl_1_bss_C->data)->activeF == 0) {
                    var_r22 = 0;
                } else {
                    temp_f29 = (f32) (s8) (u8) HuPadStkX[arg0->padNo];
                    temp_f30 = (f32) -(s8) (u8) HuPadStkY[arg0->padNo];
                    if (temp_f30 < 0.0f) {
                        var_f20 = -temp_f30;
                    } else {
                        var_f20 = temp_f30;
                    }
                    if (temp_f29 < 0.0f) {
                        var_f19 = -temp_f29;
                    } else {
                        var_f19 = temp_f29;
                    }
                    sp1C = var_f19 + var_f20;
                    if (sp1C > 32.0f) {
                        if (temp_f30 == 0.0f) {
                            M2C_ERROR(/* unknown instruction: cror eq, gt, eq */);
                            if (temp_f29 == 0.0f) {
                                var_f25 = 1.5707964f;
                            } else {
                                var_f25 = -1.5707964f;
                            }
                        } else {
                            sp8 = (f32) atan2((f64) temp_f29, (f64) temp_f30);
                            var_f25 = sp8;
                        }
                        arg0->targetRot = (f32) ((f64) (180.0f * var_f25) / 3.141592653589793);
                        var_r20 = 1;
                    }
                    var_r22 = var_r20;
                }
                var_r29_2 = var_r22;
            }
            if (var_r29_2 != 0) {
                arg0->func = fn_1_32C4;
                arg0->state = 0;
            }
        }
        break;
    }
    return 0;
}

s32 fn_1_32C4(M538_PLAYER *arg0) {
    s32 sp4C;
    s32 sp48;
    s32 sp44;
    s32 sp40;
    s32 sp3C;
    s32 sp38;
    s32 sp34;
    f32 sp30;
    f64 sp28;
    f32 sp20;
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    f32 sp8;
    M538_NODE *temp_r3;
    f32 temp_f0_2;
    f32 temp_f1;
    f32 temp_f27;
    f32 temp_f28;
    f32 temp_f29;
    f32 temp_f30;
    f32 var_f18;
    f32 var_f19;
    f32 var_f25;
    f32 var_f26;
    f32 var_f31;
    f64 temp_f0;
    s32 temp_r0;
    s32 var_r19;
    s32 var_r21;
    s32 var_r22;
    s32 var_r24;
    s32 var_r25;
    s32 var_r27;
    s32 var_r29;
    s32 var_r29_2;
    M538_PLAYER *temp_r30;

    temp_r0 = arg0->state;
    switch (temp_r0) {                              /* irregular */
    case 0:
        arg0->flag = 0;
        CharMotionShiftSet(arg0->charNo, arg0->obj->mtnId[3], 0.0f, 5.0f, 0x40000001U);
        arg0->state += 1;
        /* fallthrough */
    case 1:
        if ((s16) arg0->type != 0) {
            var_r25 = 0;
            sp30 = lbl_1_rodata_EC[0];
            sp34 = lbl_1_rodata_EC[1];
            sp38 = lbl_1_rodata_EC[2];
            sp3C = lbl_1_rodata_EC[3];
            sp40 = lbl_1_rodata_FC[0];
            sp44 = lbl_1_rodata_FC[1];
            sp48 = lbl_1_rodata_FC[2];
            sp4C = lbl_1_rodata_FC[3];
            if ((s16) ((M538 *) lbl_1_bss_C->data)->activeF == 0) {
                var_r27 = 0;
            } else {
                temp_r30 = arg0;
                if (((s16) temp_r30->aiFlag != 0) && (arg0->aiTarget != NULL)) {
                    temp_f27 = arg0->aiTarget->x - arg0->x;
                    temp_f28 = arg0->aiTarget->z - arg0->z;
                    sp20 = (temp_f27 * temp_f27) + (temp_f28 * temp_f28);
                    if (sp20 < (&sp30)[arg0->dif]) {
                        if (temp_f28 == 0.0f) {
                            M2C_ERROR(/* unknown instruction: cror eq, gt, eq */);
                            if (temp_f27 == 0.0f) {
                                var_f26 = 1.5707964f;
                            } else {
                                var_f26 = -1.5707964f;
                            }
                        } else {
                            sp14 = (f32) atan2((f64) temp_f27, (f64) temp_f28);
                            var_f26 = sp14;
                        }
                        var_f31 = (f32) (((f64) (180.0f * var_f26) / 3.141592653589793) - (f64) arg0->targetRot);
loop_18:
                        if (var_f31 > 180.0f) {
                            var_f31 -= 360.0f;
                            goto loop_18;
                        }
loop_21:
                        if (var_f31 < -180.0f) {
                            var_f31 += 360.0f;
                            goto loop_21;
                        }
                        sp18 = var_f31;
                        temp_f0 = fabs(sp18);
                        sp28 = temp_f0;
                        temp_f0_2 = (f32) temp_f0;
                        sp10 = temp_f0_2;
                        spC = temp_f0_2;
                        if ((spC < 90.0f) && (frandmod(0x64U) < (s32) (&sp40)[arg0->dif])) {
                            temp_r30->aiFlag = 0;
                            var_r25 = 1;
                        }
                    }
                }
                var_r27 = var_r25;
            }
            var_r29 = var_r27;
        } else {
            var_r22 = 0;
            if ((s16) ((M538 *) lbl_1_bss_C->data)->activeF == 0) {
                var_r24 = 0;
            } else {
                if ((s32) (HuPadBtnDown[arg0->padNo] & 0x100) != 0) {
                    var_r22 = 1;
                }
                var_r24 = var_r22;
            }
            var_r29 = var_r24;
        }
        if (var_r29 != 0) {
            arg0->func = fn_1_39DC;
            arg0->state = 0;
        } else {
            if ((s16) arg0->type != 0) {
                var_r29_2 = fn_1_1DC8(arg0);
            } else {
                var_r19 = 0;
                if ((s16) ((M538 *) lbl_1_bss_C->data)->activeF == 0) {
                    var_r21 = 0;
                } else {
                    temp_f29 = (f32) (s8) (u8) HuPadStkX[arg0->padNo];
                    temp_f30 = (f32) -(s8) (u8) HuPadStkY[arg0->padNo];
                    if (temp_f30 < 0.0f) {
                        var_f19 = -temp_f30;
                    } else {
                        var_f19 = temp_f30;
                    }
                    if (temp_f29 < 0.0f) {
                        var_f18 = -temp_f29;
                    } else {
                        var_f18 = temp_f29;
                    }
                    sp1C = var_f18 + var_f19;
                    if (sp1C > 32.0f) {
                        if (temp_f30 == 0.0f) {
                            M2C_ERROR(/* unknown instruction: cror eq, gt, eq */);
                            if (temp_f29 == 0.0f) {
                                var_f25 = 1.5707964f;
                            } else {
                                var_f25 = -1.5707964f;
                            }
                        } else {
                            sp8 = (f32) atan2((f64) temp_f29, (f64) temp_f30);
                            var_f25 = sp8;
                        }
                        arg0->targetRot = (f32) ((f64) (180.0f * var_f25) / 3.141592653589793);
                        var_r19 = 1;
                    }
                    var_r21 = var_r19;
                }
                var_r29_2 = var_r21;
            }
            if (var_r29_2 != 0) {
                arg0->velX = (f32) ((f64) arg0->velX + (7.5 * sin((3.141592653589793 * (f64) arg0->targetRot) / 180.0)));
                arg0->velZ = (f32) ((f64) arg0->velZ + (7.5 * cos((3.141592653589793 * (f64) arg0->targetRot) / 180.0)));
                temp_f1 = Hu3DMotionTimeGet(arg0->modelId);
                if ((temp_f1 == 15.0f) || (temp_f1 == 30.0f)) {
                    temp_r3 = fn_1_8858(arg0->pool1, (M538_NODEFUNC) fn_1_8CD8);
                    if (temp_r3 != NULL) {
                        temp_r3->rotY = (f32) arg0->x;
                        temp_r3->rotZ = (s32) arg0->y;
                        temp_r3->x = (f32) arg0->z;
                        temp_r3->rotX = 0;
                    }
                }
            } else {
                arg0->func = fn_1_2CB8;
                arg0->state = 0;
            }
        }
        break;
    }
    return 0;
}

s32 fn_1_39DC(M538_PLAYER *arg0) {
    switch (arg0->state) {
    case 0:
        arg0->flag = 0;
        CharMotionShiftSet(arg0->charNo, arg0->obj->mtnId[2], 0.0f, 5.0f, 0U);
        HuAudFXPlayVolPan(0x7E2, arg0->voiceVol, arg0->voicePan);
        arg0->timer = 0;
        arg0->state += 1;
        /* fallthrough */
    case 1:
        if (arg0->obj->mtnId[2] == Hu3DMotionIDGet(arg0->modelId)) {
            CharMotionSpeedSet(arg0->charNo, 1.2f);
            arg0->state += 1;
        }
        break;
    case 2:
        arg0->timer += 1;
        if ((arg0->timer >= 9) && (arg0->counter <= 5)) {
            arg0->counter += 1;
        }
        if (Hu3DMotionEndCheck(arg0->modelId) != 0) {
            arg0->func = fn_1_2CB8;
            arg0->state = 0;
            arg0->counter = 0;
        }
        break;
    }
    return 0;
}

s32 fn_1_3B2C(M538_PLAYER *arg0) {
    M538_NODE *temp_r3;

    switch (arg0->state) {
    case 0:
        arg0->aiState = 0;
        arg0->counter = 0;
        arg0->flag = 1;
        CharMotionShiftSet(arg0->charNo, arg0->obj->mtnId[3], 0.0f, 5.0f, 0U);
        temp_r3 = fn_1_8858(arg0->pool2, (M538_NODEFUNC) fn_1_8F58);
        if (temp_r3 != 0) {
            temp_r3->rotY = (f32) *(s32 *) &arg0->x;
            temp_r3->rotZ = (f32) *(s32 *) &arg0->y;
            temp_r3->x = (f32) *(s32 *) &arg0->z;
            temp_r3->timer = 0;
        }
        HuAudFXPlayVolPan(0x7E3, arg0->voiceVol, arg0->voicePan);
        HuAudFXPlayVolPan(lbl_1_data_3C8[arg0->charNo], arg0->voiceVol, arg0->voicePan);
        arg0->state += 1;
        /* fallthrough */
    case 1:
        arg0->velX = (f32) arg0->accX;
        arg0->velZ = (f32) arg0->accZ;
        if ((s16) arg0->obj->mtnId[3] == Hu3DMotionIDGet(arg0->modelId)) {
            CharMotionSpeedSet(arg0->charNo, 2.0f);
            arg0->state += 1;
        }
        break;
    case 2:
        arg0->velX = (f32) arg0->accX;
        arg0->velZ = (f32) arg0->accZ;
        if (Hu3DMotionEndCheck(arg0->modelId) != 0) {
            arg0->func = fn_1_3E7C;
            arg0->state = 0;
        }
        break;
    }
    return 0;
}

s32 fn_1_3CD4(M538_PLAYER *arg0) {
    struct m538_raw3_s { s32 a; s32 b; s32 c; };
    M538_NODE *temp_r3;

    switch (arg0->state) {
    case 0:
        arg0->aiState = 0;
        arg0->counter = 0;
        arg0->flag = 1;
        CharMotionShiftSet(arg0->charNo, arg0->obj->mtnId[4], 0.0f, 5.0f, 0U);
        temp_r3 = fn_1_8858(arg0->pool2, (M538_NODEFUNC) fn_1_8F58);
        if (temp_r3 != 0) {
            *(struct m538_raw3_s *) &temp_r3->x = *(struct m538_raw3_s *) &arg0->x;
            temp_r3->timer = 0;
        }
        HuAudFXPlayVolPan(0x7E3, arg0->voiceVol, arg0->voicePan);
        HuAudFXPlayVolPan(lbl_1_data_3C8[arg0->charNo], arg0->voiceVol, arg0->voicePan);
        arg0->state += 1;
        /* fallthrough */
    case 1:
        arg0->velX = (f32) arg0->accX;
        arg0->velZ = (f32) arg0->accZ;
        if (arg0->obj->mtnId[4] == Hu3DMotionIDGet(arg0->modelId)) {
            CharMotionSpeedSet(arg0->charNo, 2.0f);
            arg0->state += 1;
        }
        break;
    case 2:
        arg0->velX = (f32) arg0->accX;
        arg0->velZ = (f32) arg0->accZ;
        if (Hu3DMotionEndCheck(arg0->modelId) != 0) {
            arg0->func = fn_1_3E7C;
            arg0->state = 0;
        }
        break;
    }
    return 0;
}

s32 fn_1_3E7C(M538_PLAYER *arg0) {
    struct m538_raw3_s { s32 a; s32 b; s32 c; };
    M538_NODE *temp_r30;

    switch (arg0->state) {
    case 0:
        arg0->aiState = 0;
        CharMotionSet(arg0->charNo, arg0->obj->mtnId[5]);
        temp_r30 = fn_1_8858(arg0->pool1, (M538_NODEFUNC) fn_1_8CD8);
        if (temp_r30 != 0) {
            *(struct m538_raw3_s *) &temp_r30->x = *(struct m538_raw3_s *) &arg0->x;
            temp_r30->timer = 0;
        }
        arg0->state++;
        /* fallthrough */
    case 1:
        if (Hu3DMotionEndCheck(arg0->modelId) != 0) {
            arg0->voiceHook = (M538_WORKFUNC) fn_1_44AC;
            arg0->func = fn_1_32C4;
            arg0->state = 0;
        }
        break;
    }
    return 0;
}

s32 fn_1_3F60(M538_PLAYER *arg0) {
    f32 sp8;
    f32 temp_f24;
    f32 temp_f25;
    f32 temp_f27;
    f32 temp_f28;
    f32 temp_f30;
    f32 var_f26;
    f32 var_f29;
    f64 temp_f31;
    f64 temp_f31_2;
    f64 temp_f31_3;
    s32 temp_r0;
    void *temp_r30;

    temp_r30 = lbl_1_bss_C->data;
    temp_r0 = arg0->state;
    switch (temp_r0) {                              /* irregular */
    case 0:
        fn_1_6C24(arg0);
        CharMotionShiftSet(arg0->charNo, arg0->obj->mtnId[7], 0.0f, 5.0f, 0x40000001U);
        temp_f27 = arg0->z;
        temp_f28 = arg0->x;
        if (temp_f27 == 0.0f) {
            M2C_ERROR(/* unknown instruction: cror eq, gt, eq */);
            if (temp_f28 == 0.0f) {
                var_f29 = 1.5707964f;
            } else {
                var_f29 = -1.5707964f;
            }
        } else {
            var_f29 = (f32) atan2((f64) temp_f28, (f64) temp_f27);
        }
        arg0->aimRot = (f32) ((f64) (180.0f * var_f29) / 3.141592653589793);
        temp_f30 = (arg0->x * arg0->x) + (arg0->z * arg0->z);
        if (temp_f30 > 0.0f) {
            temp_f31 = __frsqrte(temp_f30);
            temp_f31_2 = 0.5 * temp_f31 * (3.0 - ((f64) temp_f30 * (temp_f31 * temp_f31)));
            temp_f31_3 = 0.5 * temp_f31_2 * (3.0 - ((f64) temp_f30 * (temp_f31_2 * temp_f31_2)));
            sp8 = (f32) ((f64) temp_f30 * (0.5 * temp_f31_3 * (3.0 - ((f64) temp_f30 * (temp_f31_3 * temp_f31_3)))));
            var_f26 = sp8;
        } else {
            var_f26 = temp_f30;
        }
        arg0->aimDist = var_f26;
        arg0->rotY = (f32) arg0->targetRot;
        if (fn_1_6D98(temp_r30, (M538_NODE0FUNC) fn_1_6F24, &arg0->fw) != NULL) {
            arg0->fw.x = arg0->hookX;
            arg0->fw.y = arg0->hookY;
            arg0->fw.z = (s32) arg0->hookZ;
            arg0->fw.rotX = (s32) arg0->hookRotX;
            arg0->fw.radius2 = (s32) arg0->hookRotY;
            arg0->fw.radius3 = (s32) arg0->hookRotZ;
            arg0->fw.model = (s16) arg0->itemModel;
            arg0->fw.state = 0;
        }
        arg0->counter2 = 0;
        omVibrate(arg0->playerNo, 0x1E, 0xC, 0);
        arg0->sfxId = HuAudFXPlay(lbl_1_data_394[arg0->charNo]);
        arg0->state += 1;
        /* fallthrough */
    case 1:
        HuAudFXPanning(arg0->sfxId, arg0->voicePan);
        HuAudFXVolSet(arg0->sfxId, arg0->voiceVol);
        arg0->aimRot = (f32) (arg0->aimRot - 6.3f);
        arg0->velY = 2.0f;
        arg0->aimDist = (f32) (arg0->aimDist + 4.3f);
        temp_f25 = (f32) ((f64) arg0->aimDist * sin((3.141592653589793 * (f64) arg0->aimRot) / 180.0));
        temp_f24 = (f32) ((f64) arg0->aimDist * cos((3.141592653589793 * (f64) arg0->aimRot) / 180.0));
        arg0->x = temp_f25;
        arg0->z = temp_f24;
        arg0->targetRot = (f32) (arg0->targetRot + 1.0f);
loop_17:
        if (arg0->targetRot > 180.0f) {
            arg0->targetRot = (f32) (arg0->targetRot - 360.0f);
            goto loop_17;
        }
        arg0->rotY = (f32) (arg0->rotY + 3.0f);
loop_20:
        if (arg0->rotY > 180.0f) {
            arg0->rotY = (f32) (arg0->rotY - 360.0f);
            goto loop_20;
        }
        arg0->counter2 += 1;
        if ((s16) arg0->counter2 == 0xB4) {
            arg0->func = fn_1_4428;
            arg0->state = 0;
        }
        break;
    }
    return 0;
}

s32 fn_1_4428(M538_PLAYER *arg0) {
    switch (arg0->state) {                          /* irregular */
    case 1:
        break;
    case 0:
        CharMotionSet(arg0->charNo, arg0->obj->mtnId[10]);
        CharModelAttrSet(arg0->charNo, 1U);
        CharModelAttrSet(arg0->charNo, 0x40000002U);
        arg0->state += 1;
        break;
    }
    return 0;
}

s32 fn_1_44AC(M538_DOLLY *arg0) {
    s32 var_r30;

    var_r30 = 0;
    arg0->counter++;
    if (arg0->counter == 0x3C) {
        arg0->counter = 0;
        var_r30 = 1;
    }
    if ((arg0->counter & 1) != 0) {
        Hu3DModelAttrSet(arg0->state, 1U);
    } else {
        Hu3DModelAttrReset(arg0->state, 1U);
    }
    return var_r30;
}

void fn_1_4534(M538_PLAYER *arg0) {
    arg0->velX = 0.0f;
    arg0->velY = 0.0f;
    arg0->velZ = 0.0f;
    arg0->accX = (f32) (arg0->accX * 0.9f);
    arg0->accY = (f32) (arg0->accY * 0.9f);
    arg0->accZ = (f32) (arg0->accZ * 0.9f);
    if ((f32) fabs(arg0->accX) < 0.01f) {
        arg0->accX = 0.0f;
    }
    if ((f32) fabs(arg0->accY) < 0.01f) {
        arg0->accY = 0.0f;
    }
    if ((f32) fabs(arg0->accZ) < 0.01f) {
        arg0->accZ = 0.0f;
    }
}

void fn_1_4714(M538 *arg0) {
    M538_PLAYER *temp_r31;
    f32 var_f31;
    s32 var_r30;

    var_r30 = 0;
loop_18:
    if (var_r30 < 4) {
        temp_r31 = arg0->player[var_r30];
        temp_r31->x += temp_r31->velX;
        temp_r31->y += temp_r31->velY;
        temp_r31->z += temp_r31->velZ;
        var_f31 = temp_r31->targetRot;
        if ((f32) fabs(temp_r31->rotY - temp_r31->targetRot) > 180.0f) {
            if (temp_r31->rotY < temp_r31->targetRot) {
                var_f31 = temp_r31->targetRot - 360.0f;
            } else if (temp_r31->rotY > temp_r31->targetRot) {
                var_f31 = 360.0f + temp_r31->targetRot;
            }
        }
        if (temp_r31->rotY < var_f31) {
            temp_r31->rotY = (f32) (temp_r31->rotY + temp_r31->rotSpeed);
            if (temp_r31->rotY > var_f31) {
                temp_r31->rotY = var_f31;
            }
        } else if (temp_r31->rotY > var_f31) {
            temp_r31->rotY = (f32) (temp_r31->rotY - temp_r31->rotSpeed);
            if (temp_r31->rotY < var_f31) {
                temp_r31->rotY = var_f31;
            }
        }
loop_13:
        if (temp_r31->rotY > 180.0f) {
            temp_r31->rotY = (f32) (temp_r31->rotY - 360.0f);
            goto loop_13;
        }
loop_16:
        if (temp_r31->rotY < -180.0f) {
            temp_r31->rotY = (f32) (temp_r31->rotY + 360.0f);
            goto loop_16;
        }
        Hu3DModelPosSetV(temp_r31->modelId, (HuVecF *) temp_r31);
        omSetTra(temp_r31->obj, temp_r31->x, temp_r31->y, temp_r31->z);
        Hu3DModelRotSetV(temp_r31->modelId, (HuVecF *) &temp_r31->rotX);
        omSetRot(temp_r31->obj, temp_r31->rotX, temp_r31->rotY, temp_r31->rotZ);
        var_r30 += 1;
        goto loop_18;
    }
}

void fn_1_4968(M538 *arg0) {
    f32 temp_f31;
    s32 var_r29;
    s32 var_r30;
    M538_PLAYER *temp_r31;

    temp_f31 = arg0->anim[1].scale;
    arg0->lightRadius = temp_f31;
    var_r29 = 0;
    var_r30 = 0;
loop_13:
    if (var_r30 < 4) {
        temp_r31 = arg0->player[var_r30];
        if (((temp_r31->func) != fn_1_3F60) && ((temp_r31->func) != fn_1_4428) && ((s16) temp_r31->active != 0) && (((temp_r31->x * temp_r31->x) + (temp_r31->z * temp_r31->z)) > (temp_f31 * temp_f31))) {
            temp_r31->velX = 0.0f;
            temp_r31->velY = 0.0f;
            temp_r31->velZ = 0.0f;
            temp_r31->accX = (f32) (temp_r31->accX * 0.9f);
            temp_r31->accY = (f32) (temp_r31->accY * 0.9f);
            temp_r31->accZ = (f32) (temp_r31->accZ * 0.9f);
            if ((f32) fabs(temp_r31->accX) < 0.01f) {
                temp_r31->accX = 0.0f;
            }
            if ((f32) fabs(temp_r31->accY) < 0.01f) {
                temp_r31->accY = 0.0f;
            }
            if ((f32) fabs(temp_r31->accZ) < 0.01f) {
                temp_r31->accZ = 0.0f;
            }
            temp_r31->func = fn_1_3F60;
            temp_r31->state = 0;
            temp_r31->active = 0;
            temp_r31->place = (s16) arg0->fireCount;
            var_r29 += 1;
        }
        var_r30 += 1;
        goto loop_13;
    }
    arg0->fireCount = (s16) (arg0->fireCount + var_r29);
}

void fn_1_4C38(M538 *arg0) {
    f32 temp_f31;
    f32 temp_f30;
    f32 temp_f29;
    f32 var_f28;
    f32 temp_f27;
    f32 temp_f26;
    s32 var_r28;
    s32 var_r29;
    M538_PLAYER *temp_r31;
    M538_PLAYER *temp_r30;

    var_r29 = 0;
    goto loop_18_test;
loop_18:
    temp_r31 = arg0->player[var_r29];
    if (((void *) temp_r31->func != fn_1_3F60) && ((void *) temp_r31->func != fn_1_4428) && (temp_r31->active != 0)) {
        var_r28 = var_r29 + 1;
        goto loop_16_test;
    loop_16:
        temp_r30 = arg0->player[var_r28];
        if (((void *) temp_r30->func != fn_1_3F60) && ((void *) temp_r30->func != fn_1_4428) && (temp_r30->active != 0)) {
            temp_f31 = temp_r31->x - temp_r30->x;
            temp_f30 = temp_r31->z - temp_r30->z;
            temp_f27 = (temp_f31 * temp_f31) + (temp_f30 * temp_f30);
            if (temp_f27 < 12100.0f) {
                if (temp_f30 == 0.0f) {
                    if (temp_f31 >= 0.0f) {
                        var_f28 = 1.5707964f;
                    } else {
                        var_f28 = -1.5707964f;
                    }
                } else {
                    temp_f26 = (f32) atan2((f64) temp_f31, (f64) temp_f30);
                    var_f28 = temp_f26;
                }
                temp_f29 = (f32) ((180.0f * var_f28) / 3.141592653589793);
                temp_r31->velX = (f32) ((0.5f * temp_r31->velX) + (3.75 * sin((3.141592653589793 * (f64) temp_f29) / 180.0)));
                temp_r31->velZ = (f32) ((0.5f * temp_r31->velZ) + (3.75 * cos((3.141592653589793 * (f64) temp_f29) / 180.0)));
                temp_r30->velX = (f32) ((0.5f * temp_r30->velX) - (3.75 * sin((3.141592653589793 * (f64) temp_f29) / 180.0)));
                temp_r30->velZ = (f32) ((0.5f * temp_r30->velZ) - (3.75 * cos((3.141592653589793 * (f64) temp_f29) / 180.0)));
            }
        }
        var_r28 += 1;
    loop_16_test:
        if (var_r28 < 4) {
            goto loop_16;
        }
    }
    var_r29 += 1;
loop_18_test:
    if (var_r29 < 3) {
        goto loop_18;
    }
}

void fn_1_4F74(M538 *arg0) {
    f32 sp38[4];
    f32 sp28[4];
    f32 temp_f24;
    f32 temp_f25;
    f32 temp_f26;
    f32 temp_f27;
    f32 var_f28;
    f32 var_f29;
    f32 var_f30;
    f32 var_f31;
    s32 var_r28;
    s32 var_r29;
    s32 var_r29_2;
    M538_PLAYER *temp_r30;
    M538_PLAYER *temp_r31;
    f32 sp18[4];
    f32 sp8[4];
    f32 sp14[4];
    var_r28 = 0;
loop_52:
    if (var_r28 < 4) {
        temp_r30 = arg0->player[var_r28];
        if ((s16) temp_r30->counter != 0) {
            var_f29 = (temp_r30->rotY - 60.0f) + ((120.0f * (f32) temp_r30->counter) / 5.0f);
loop_4:
            if (var_f29 > 180.0f) {
                var_f29 -= 360.0f;
                goto loop_4;
            }
loop_7:
            if (var_f29 < -180.0f) {
                var_f29 += 360.0f;
                goto loop_7;
            }
            temp_f25 = (f32) sin((3.141592653589793 * (f64) var_f29) / 180.0);
            temp_f24 = (f32) cos((3.141592653589793 * (f64) var_f29) / 180.0);
            var_r29 = 0;
loop_10:
            if (var_r29 < 4) {
                sp18[var_r29] = temp_r30->x + ((temp_f24 * sp38[var_r29]) + (temp_f25 * sp28[var_r29]));
                sp8[var_r29] = temp_r30->z + ((temp_f25 * -sp38[var_r29]) + (temp_f24 * sp28[var_r29]));
                var_r29 += 1;
                goto loop_10;
            }
            var_r29_2 = 0;
loop_50:
            if (var_r29_2 < 4) {
                if (var_r28 != var_r29_2) {
                    temp_r31 = arg0->player[var_r29_2];
                    if (((s16) temp_r31->flag == 0) && ((void *) temp_r31->voiceHook == 0U) && (fn_1_55C8(temp_r31, &sp18[0], &sp8[0]) != 0)) {
                        omVibrate(temp_r30->playerNo, 0xC, 6, 6);
                        omVibrate(temp_r31->playerNo, 0xC, 0xC, 0);
                        temp_f27 = temp_r30->x - temp_r31->x;
                        temp_f26 = temp_r30->z - temp_r31->z;
                        if (temp_f26 == 0.0f) {
                            M2C_ERROR(/* unknown instruction: cror eq, gt, eq */);
                            if (temp_f27 == 0.0f) {
                                var_f28 = 1.5707964f;
                            } else {
                                var_f28 = -1.5707964f;
                            }
                        } else {
                            var_f28 = (f32) atan2((f64) temp_f27, (f64) temp_f26);
                        }
                        var_f31 = (f32) (180.0 + ((f64) (180.0f * var_f28) / 3.141592653589793));
loop_23:
                        if (var_f31 > 180.0f) {
                            var_f31 -= 360.0f;
                            goto loop_23;
                        }
loop_26:
                        if (var_f31 < -180.0f) {
                            var_f31 += 360.0f;
                            goto loop_26;
                        }
                        temp_r31->accX = (f32) (35.0 * sin((3.141592653589793 * (f64) var_f31) / 180.0));
                        temp_r31->accZ = (f32) (35.0 * cos((3.141592653589793 * (f64) var_f31) / 180.0));
                        var_f30 = temp_r31->rotY - var_f31;
loop_29:
                        if (var_f30 > 180.0f) {
                            var_f30 -= 360.0f;
                            goto loop_29;
                        }
loop_32:
                        if (var_f30 < -180.0f) {
                            var_f30 += 360.0f;
                            goto loop_32;
                        }
                        if ((f32) fabs(var_f30) < 90.0f) {
                            temp_r31->targetRot = var_f31;
loop_36:
                            if (temp_r31->targetRot > 180.0f) {
                                temp_r31->targetRot = (f32) (temp_r31->targetRot - 360.0f);
                                goto loop_36;
                            }
loop_39:
                            if (temp_r31->targetRot < -180.0f) {
                                temp_r31->targetRot = (f32) (temp_r31->targetRot + 360.0f);
                                goto loop_39;
                            }
                            temp_r31->func = fn_1_3B2C;
                        } else {
                            temp_r31->targetRot = (f32) (180.0f + var_f31);
loop_43:
                            if (temp_r31->targetRot > 180.0f) {
                                temp_r31->targetRot = (f32) (temp_r31->targetRot - 360.0f);
                                goto loop_43;
                            }
loop_46:
                            if (temp_r31->targetRot < -180.0f) {
                                temp_r31->targetRot = (f32) (temp_r31->targetRot + 360.0f);
                                goto loop_46;
                            }
                            temp_r31->func = fn_1_3CD4;
                        }
                        temp_r31->state = 0;
                    }
                }
                var_r29_2 += 1;
                goto loop_50;
            }
        }
        var_r28 += 1;
        goto loop_52;
    }
}

s32 fn_1_55C8(M538_PLAYER *arg0, f32 *arg1, f32 *arg2) {
    s32 var_r31;
    f32 temp_f31;

    for (var_r31 = 0; var_r31 < 4; var_r31++) {
        temp_f31 = ((arg0->x - arg1[var_r31]) * (arg2[(var_r31 + 1) % 4] - arg2[var_r31])) -
                   ((arg0->z - arg2[var_r31]) * (arg1[(var_r31 + 1) % 4] - arg1[var_r31]));
        if (temp_f31 < 0.0f) {
            return 0;
        }
    }
    return 1;
}
void fn_1_56A4(M538 *arg0) {
    f32 temp_f30;
    f32 temp_f31;
    s16 temp_r3;
    s16 temp_r3_10;
    s16 temp_r3_2;
    s16 temp_r3_3;
    s16 temp_r3_4;
    s16 temp_r3_5;
    s16 temp_r3_6;
    s16 temp_r3_7;
    s16 temp_r3_8;
    s16 temp_r3_9;
    s16 var_r28;
    s16 var_r31;
    s16 var_r31_2;
    s16 var_r31_3;
    s16 var_r31_4;
    s16 var_r31_5;
    s16 var_r31_6;
    s16 var_r31_7;
    s16 var_r31_8;
    s16 var_r31_9;
    s32 var_r29;

    if ((s16) arg0->models[11] < 0) {
        temp_r3 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_264[0xB], 0x10000000, HEAP_MODEL));
        arg0->models[11] = temp_r3;
        var_r31 = temp_r3;
    } else {
        var_r31 = Hu3DModelLink(arg0->models[11]);
    }
    Hu3DModelAttrSet(var_r31, 1U);
    Hu3DModelAttrSet(var_r31, 0x40000002U);
    Hu3DModelLayerSet(var_r31, 0);
    Hu3DModelPosSet(var_r31, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(var_r31, 0.0f, 0.0f, 0.0f);
    Hu3DModelShadowMapSet(var_r31);
    Hu3DModelAttrReset(var_r31, 1U);
    Hu3DMotionSpeedSet(var_r31, 0.0f);
    if ((s16) arg0->models[12] < 0) {
        temp_r3_2 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_264[0xC], 0x10000000, HEAP_MODEL));
        arg0->models[12] = temp_r3_2;
        var_r31_2 = temp_r3_2;
    } else {
        var_r31_2 = Hu3DModelLink(arg0->models[12]);
    }
    Hu3DModelAttrSet(var_r31_2, 1U);
    Hu3DModelAttrSet(var_r31_2, 0x40000002U);
    Hu3DModelLayerSet(var_r31_2, 3);
    Hu3DModelPosSet(var_r31_2, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(var_r31_2, 0.0f, 0.0f, 0.0f);
    Hu3DModelShadowMapSet(var_r31_2);
    Hu3DModelAttrReset(var_r31_2, 1U);
    Hu3DMotionSpeedSet(var_r31_2, 0.0f);
    if ((s16) arg0->models[13] < 0) {
        temp_r3_3 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_264[0xD], 0x10000000, HEAP_MODEL));
        arg0->models[13] = temp_r3_3;
        var_r31_3 = temp_r3_3;
    } else {
        var_r31_3 = Hu3DModelLink(arg0->models[13]);
    }
    Hu3DModelAttrSet(var_r31_3, 1U);
    Hu3DModelAttrSet(var_r31_3, 0x40000002U);
    Hu3DModelLayerSet(var_r31_3, 0);
    Hu3DModelPosSet(var_r31_3, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(var_r31_3, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(var_r31_3, 1U);
    if ((s16) arg0->models[14] < 0) {
        temp_r3_4 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_264[0xE], 0x10000000, HEAP_MODEL));
        arg0->models[14] = temp_r3_4;
        var_r31_4 = temp_r3_4;
    } else {
        var_r31_4 = Hu3DModelLink(arg0->models[14]);
    }
    Hu3DModelAttrSet(var_r31_4, 1U);
    Hu3DModelAttrSet(var_r31_4, 0x40000002U);
    Hu3DModelLayerSet(var_r31_4, 0);
    Hu3DModelPosSet(var_r31_4, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(var_r31_4, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(var_r31_4, 1U);
    if ((s16) arg0->models[15] < 0) {
        temp_r3_5 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_264[0xF], 0x10000000, HEAP_MODEL));
        arg0->models[15] = temp_r3_5;
        var_r31_5 = temp_r3_5;
    } else {
        var_r31_5 = Hu3DModelLink(arg0->models[15]);
    }
    Hu3DModelAttrSet(var_r31_5, 1U);
    Hu3DModelAttrSet(var_r31_5, 0x40000001U);
    Hu3DModelLayerSet(var_r31_5, 0);
    Hu3DModelPosSet(var_r31_5, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(var_r31_5, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(var_r31_5, 1U);
    if ((s16) arg0->models[16] < 0) {
        temp_r3_6 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_264[0x10], 0x10000000, HEAP_MODEL));
        arg0->models[16] = temp_r3_6;
        var_r31_6 = temp_r3_6;
    } else {
        var_r31_6 = Hu3DModelLink(arg0->models[16]);
    }
    Hu3DModelAttrSet(var_r31_6, 1U);
    Hu3DModelAttrSet(var_r31_6, 0x40000002U);
    Hu3DModelLayerSet(var_r31_6, 0);
    Hu3DModelPosSet(var_r31_6, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(var_r31_6, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(var_r31_6, 1U);
    if ((s16) arg0->models[24] < 0) {
        temp_r3_7 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_264[0x18], 0x10000000, HEAP_MODEL));
        arg0->models[24] = temp_r3_7;
        var_r31_7 = temp_r3_7;
    } else {
        var_r31_7 = Hu3DModelLink(arg0->models[24]);
    }
    Hu3DModelAttrSet(var_r31_7, 1U);
    Hu3DModelAttrSet(var_r31_7, 0x40000002U);
    Hu3DModelLayerSet(var_r31_7, 3);
    Hu3DModelPosSet(var_r31_7, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(var_r31_7, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(var_r31_7, 1U);
    if ((s16) arg0->models[28] < 0) {
        temp_r3_8 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_264[0x1C], 0x10000000, HEAP_MODEL));
        arg0->models[28] = temp_r3_8;
        var_r31_8 = temp_r3_8;
    } else {
        var_r31_8 = Hu3DModelLink(arg0->models[28]);
    }
    Hu3DModelAttrSet(var_r31_8, 1U);
    Hu3DModelAttrSet(var_r31_8, 0x40000001U);
    Hu3DModelLayerSet(var_r31_8, 3);
    Hu3DModelPosSet(var_r31_8, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(var_r31_8, 0.0f, 0.0f, 0.0f);
    var_r29 = 0;
loop_32:
    if (var_r29 < 0x18) {
        if ((s16) arg0->models[17] < 0) {
            temp_r3_9 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_264[0x11], 0x10000000, HEAP_MODEL));
            arg0->models[17] = temp_r3_9;
            var_r31_9 = temp_r3_9;
        } else {
            var_r31_9 = Hu3DModelLink(arg0->models[17]);
        }
        if ((s16) arg0->models[29] < 0) {
            temp_r3_10 = Hu3DJointMotion(var_r31_9, HuDataSelHeapReadNum(*lbl_1_data_2D8, 0x10000000, HEAP_MODEL));
            arg0->models[29] = temp_r3_10;
            var_r28 = temp_r3_10;
        } else {
            var_r28 = arg0->models[29];
        }
        Hu3DMotionSet(var_r31_9, var_r28);
        Hu3DModelAttrSet(var_r31_9, 1U);
        Hu3DModelAttrSet(var_r31_9, 0x40000002U);
        Hu3DModelLayerSet(var_r31_9, 1);
        temp_f31 = (f32) ((3.141592653589793 * (f64) (15.0f * (f32) var_r29)) / 180.0);
        temp_f30 = (f32) (750.0 * sin((f64) temp_f31));
        Hu3DModelPosSet(var_r31_9, temp_f30, 0.0f, (f32) (750.0 * cos((f64) temp_f31)));
        Hu3DModelRotSet(var_r31_9, 0.0f, 15.0f * (f32) var_r29, 0.0f);
        Hu3DModelShadowMapSet(var_r31_9);
        Hu3DMotionSpeedSet(var_r31_9, 0.0f);
        Hu3DMotionTimeSet(var_r31_9, (f32) frandmod(0x10U));
        Hu3DModelAttrReset(var_r31_9, 1U);
        var_r29 += 1;
        goto loop_32;
    }
}

void fn_1_5FC4(M538_ANIM *arg0, s16 arg1) {
    s16 sp8[4];
    f32 var_f31;
    M538 *temp_r28;
    s16 var_r30;
    s32 var_r29;
    /* 7 rows differ, all in this init: the original loads the three u16 offsets
     * with one lhz/sth pair each and no narrowing (lbl_1_rodata_190 read
     * unsigned, sp8 read back signed with lhax). MWCC 1.3.2 always narrows a
     * 16-bit array store -- extsh for an s16 destination, clrlwi for a u16 one --
     * and recomputes the rodata base per statement. */
    sp8[0] = lbl_1_rodata_190[0];
    sp8[1] = lbl_1_rodata_190[1];
    sp8[2] = lbl_1_rodata_190[2];

    if (arg1 == 1) {
        var_f31 = 50.0f;
    } else {
        var_f31 = 0.0f;
    }
    temp_r28 = (M538 *) lbl_1_bss_C->data;
    var_r29 = 0;
    goto loop_11_t;
loop_11:
    if (temp_r28->models[var_r29 + sp8[arg1]] < 0) {
        var_r30 = temp_r28->models[var_r29 + sp8[arg1]] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_264[var_r29 + sp8[arg1]], 0x10000000, HEAP_MODEL));
    } else {
        var_r30 = Hu3DModelLink(temp_r28->models[var_r29 + sp8[arg1]]);
    }
    arg0->model[var_r29] = var_r30;
    Hu3DModelAttrSet(var_r30, 1U);
    Hu3DModelAttrSet(var_r30, 0x40000002U);
    if (arg1 == 1) {
        Hu3DModelLayerSet(var_r30, 7);
    } else {
        Hu3DModelLayerSet(var_r30, 6);
    }
    Hu3DModelPosSet(var_r30, 0.0f, var_f31, 0.0f);
    Hu3DModelRotSet(var_r30, 0.0f, 0.0f, 0.0f);
    var_r29 += 1;
loop_11_t:
    if (var_r29 < 3) {
        goto loop_11;
    }
    arg0->scale = 750.0f;
    arg0->state = 0;
    arg0->counter = 0;
    arg0->flag = 1;
    arg0->func = 0;
    arg0->mode = arg1;
}

s32 fn_1_61F8(M538_ANIM *arg0) {
    M538_ANIMFUNC next[3];

    next[0] = lbl_1_rodata_1A0[0];
    next[1] = lbl_1_rodata_1A0[1];
    next[2] = lbl_1_rodata_1A0[2];
    switch (arg0->state) {                          /* irregular */
    case 0:
        Hu3DModelAttrReset(arg0->model[0], 1U);
        Hu3DModelAttrReset(arg0->model[0], 0x40000002U);
        arg0->state++;
        /* fallthrough */
    case 1:
        if (Hu3DMotionEndCheck(arg0->model[0]) != 0) {
            arg0->func = next[arg0->mode];
            arg0->state = 0;
        }
        break;
    }
    return 0;
}

s32 fn_1_62B4(M538_ANIM *arg0) {
    f32 temp_f31;
    s32 var_r30;

    var_r30 = 0;
    switch (arg0->state) {
    case 1:
        break;
    case 0:
        Hu3DModelAttrReset(arg0->model[1], 1U);
        Hu3DModelAttrReset(arg0->model[1], 0x40000002U);
        Hu3DModelAttrSet(arg0->model[1], 0x40000001U);
        Hu3DModelAttrSet(arg0->model[0], 1U);
        arg0->state += 1;
        break;
    case 2:
        arg0->counter += 1;
        if (arg0->counter == 0x708) {
            arg0->state += 1;
            var_r30 = 1;
        }
        temp_f31 = (630.0f * (f32) arg0->counter) / 1800.0f;
        Hu3DModelPosSet(arg0->model[1], 0.0f, temp_f31, 0.0f);
        break;
    }
    return var_r30;
}

s32 fn_1_6400(M538_ANIM *arg0) {
    f32 temp_f31;
    s32 var_r30;

    var_r30 = 0;
    switch (arg0->state) {                              /* irregular */
    case 1:
        break;
    case 0:
        Hu3DModelAttrReset(arg0->model[1], 1U);
        Hu3DModelAttrReset(arg0->model[1], 0x40000002U);
        Hu3DModelAttrSet(arg0->model[1], 0x40000001U);
        Hu3DModelAttrSet(arg0->model[0], 1U);
        arg0->state++;
        break;
    case 2:
        arg0->counter++;
        if (arg0->counter == 0x708) {
            arg0->state++;
            var_r30 = 1;
        }
        temp_f31 = 1.0f - ((f32) arg0->counter / 1800.0f);
        arg0->scale = 750.0f * temp_f31;
        Hu3DModelScaleSet(arg0->model[1], temp_f31, 1.0f, temp_f31);
        break;
    }
    return var_r30;
}

s32 fn_1_6558(M538_ANIM *arg0) {
    s32 var_r30;
    f32 temp_f31;

    var_r30 = 0;
    switch (arg0->state) {                              /* irregular */
    case 1:
        break;
    case 0:
        Hu3DModelAttrReset(arg0->model[1], 1U);
        Hu3DModelAttrReset(arg0->model[1], 0x40000002U);
        Hu3DModelAttrSet(arg0->model[1], 0x40000001U);
        Hu3DModelAttrSet(arg0->model[0], 1U);
        arg0->state++;
        break;
    case 2:
        arg0->counter++;
        if (arg0->counter == 0x708) {
            arg0->state++;
            var_r30 = 1;
        }
        temp_f31 = (270.0f * (f32) arg0->counter) / 1800.0f;
        Hu3DModelPosSet(arg0->model[1], 0.0f, temp_f31, 0.0f);
        break;
    }
    return var_r30;
}

s32 fn_1_66A4(M538_ANIM *arg0) {
    f32 temp_f31;
    f32 temp_f30;

    switch (arg0->state) {
    case 0:
        Hu3DModelAttrReset(arg0->model[1], 0x40000001U);
        arg0->state += 1;
        /* fallthrough */
    case 1:
        if (Hu3DMotionEndCheck(arg0->model[1]) != 0) {
            arg0->state += 1;
        }
        break;
    case 2:
        Hu3DModelAttrReset(arg0->model[2], 1U);
        Hu3DModelAttrReset(arg0->model[2], 0x40000002U);
        Hu3DModelAttrSet(arg0->model[1], 1U);
        if (arg0->mode == 0) {
            temp_f31 = (630.0f * (f32) arg0->counter) / 1800.0f;
            Hu3DModelPosSet(arg0->model[2], 0.0f, temp_f31, 0.0f);
        }
        if (arg0->mode == 1) {
            temp_f30 = 1.0f - ((f32) arg0->counter / 1800.0f);
            Hu3DModelScaleSet(arg0->model[2], temp_f30, 1.0f, temp_f30);
        }
        if (arg0->mode == 2) {
            temp_f31 = (270.0f * (f32) arg0->counter) / 1800.0f;
            Hu3DModelPosSet(arg0->model[2], 0.0f, temp_f31, 0.0f);
        }
        arg0->state += 1;
        /* fallthrough */
    case 3:
        if (Hu3DMotionEndCheck(arg0->model[2]) != 0) {
            Hu3DModelAttrSet(arg0->model[2], 1U);
            arg0->func = 0;
            arg0->state = 0;
        }
        break;
    }
    return 0;
}

s32 fn_1_6918(void *arg0) {
    s32 var_r30;
    M538 *temp_r31;

    var_r30 = 0;
    temp_r31 = lbl_1_bss_C->data;
    if (temp_r31->anim[0].func != 0) {
        temp_r31->anim[0].func(&temp_r31->anim[0]);
    }
    if ((temp_r31->anim[1].func) != 0) {
        var_r30 = temp_r31->anim[1].func(&temp_r31->anim[1]);
    }
    if (temp_r31->anim[2].func != 0) {
        temp_r31->anim[2].func(&temp_r31->anim[2]);
        fn_1_A2A4(temp_r31);
    }
    var_r30 = var_r30 & temp_r31->anim[1].flag;
    return var_r30;
}

void fn_1_69C4(struct m538_s16_48_s *arg0) {
    s16 linkedModel;
    M538 *m538;

    m538 = lbl_1_bss_C->data;
    if (m538->models[0] < 0) {
        linkedModel = m538->models[0] = Hu3DModelCreate(HuDataSelHeapReadNum(*lbl_1_data_264, 0x10000000, HEAP_MODEL));
    } else {
        linkedModel = Hu3DModelLink(m538->models[0]);
    }
    arg0->model = linkedModel;
    Hu3DModelAttrSet(linkedModel, 1U);
    Hu3DModelAttrSet(linkedModel, 0x40000002U);
    Hu3DModelLayerSet(linkedModel, 2);
    Hu3DModelPosSet(linkedModel, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(linkedModel, 0.0f, 0.0f, 0.0f);
    Hu3DModelShadowSet(linkedModel);
    Hu3DModelAttrReset(linkedModel, 1U);
}

void fn_1_6AE4(M538_PLAYER *arg0) {
    s16 linkedModel;
    M538 *m538;

    m538 = lbl_1_bss_C->data;
    if (m538->models[0] < 0) {
        linkedModel = m538->models[0] = Hu3DModelCreate(HuDataSelHeapReadNum(*lbl_1_data_264, 0x10000000, HEAP_MODEL));
    } else {
        linkedModel = Hu3DModelLink(m538->models[0]);
    }
    arg0->itemModel = linkedModel;
    Hu3DModelAttrSet(linkedModel, 1U);
    Hu3DModelAttrSet(linkedModel, 0x40000002U);
    Hu3DModelLayerSet(linkedModel, 2);
    Hu3DModelPosSet(linkedModel, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(linkedModel, 0.0f, 0.0f, 0.0f);
    Hu3DModelShadowSet(linkedModel);
    Hu3DModelAttrReset(linkedModel, 1U);
    Hu3DModelHookSet(arg0->modelId, CharModelItemHookGet(arg0->charNo, 8, 0), arg0->itemModel);
}

void fn_1_6C24(M538_PLAYER *arg0) {
    struct m538_pair_s { s32 a; s32 b; };
    Mtx sp20;
    s32 sp14[3];
    s32 sp8[3];

    Hu3DModelObjMtxGet(arg0->modelId, CharModelItemHookGet(arg0->charNo, 8, 0), sp20);
    Hu3DMtxTransGet(sp20, (HuVecF *)sp14);
    Hu3DMtxRotGet(sp20, (HuVecF *)sp8);
    Hu3DModelHookReset(arg0->modelId);
    /* the hook slots are raw 32 bit words in the original, not floats */
    *(struct m538_pair_s *) &arg0->hookX = *(struct m538_pair_s *) &sp14[0];
    *(s32 *) &arg0->hookZ = sp14[2];
    *(struct m538_pair_s *) &arg0->hookRotX = *(struct m538_pair_s *) &sp8[0];
    *(s32 *) &arg0->hookRotZ = sp8[2];
    Hu3DModelPosSetV(arg0->itemModel, (HuVecF *)sp14);
    Hu3DModelRotSetV(arg0->itemModel, (HuVecF *)sp8);
}

void fn_1_6CD4(M538 *arg0) {
    s32 var_r31;

    for (var_r31 = 0; var_r31 < 0x1F; var_r31++) {
        arg0->node0[var_r31].prev = 0;
        arg0->node0[var_r31].next = &arg0->node0[var_r31 + 1];
        arg0->node0[var_r31].func = 0;
        arg0->node0[var_r31].work = 0;
    }
    arg0->node0[var_r31].prev = 0;
    arg0->node0[var_r31].next = 0;
    arg0->node0[var_r31].func = 0;
    arg0->node0[var_r31].work = 0;
    arg0->node0Head = 0;
    arg0->node0Free = &arg0->node0[0];
}

M538_NODE0 *fn_1_6D98(M538 *arg0, M538_NODE0FUNC arg1, void *arg2) {
    M538_NODE0 *temp_r31;

    temp_r31 = arg0->node0Free;
    if (temp_r31 != 0) {
        arg0->node0Free = temp_r31->next;
        temp_r31->func = arg1;
        temp_r31->work = arg2;
        temp_r31->next = arg0->node0Head;
        if (temp_r31->next != 0) {
            temp_r31->next->prev = temp_r31;
        }
        arg0->node0Head = temp_r31;
    }
    return temp_r31;
}

void fn_1_6DEC(M538 *arg0, M538_NODE0 *arg1) {
    if (arg1 != 0) {
        if (arg1->prev != 0) {
            arg1->prev->next = arg1->next;
        } else {
            arg0->node0Head = arg1->next;
        }
        if (arg1->next != 0) {
            arg1->next->prev = arg1->prev;
        }
        arg1->prev = NULL;
        arg1->next = arg0->node0Free;
        arg1->func = 0;
        arg0->node0Free = arg1;
    }
}

void fn_1_6E50(M538 *arg0) {
    M538_NODE0 *temp_r29;
    M538_NODE0 *var_r31;

    var_r31 = arg0->node0Head;
    while (var_r31 != 0) {
        temp_r29 = var_r31->next;
        if ((var_r31->func != 0) && (var_r31->func(var_r31) != 0) && (var_r31 != 0)) {
            if (var_r31->prev != 0) {
                var_r31->prev->next = var_r31->next;
            } else {
                arg0->node0Head = var_r31->next;
            }
            if (var_r31->next != 0) {
                var_r31->next->prev = var_r31->prev;
            }
            var_r31->prev = NULL;
            var_r31->next = arg0->node0Free;
            var_r31->func = NULL;
            arg0->node0Free = var_r31;
        }
        var_r31 = temp_r29;
    }
}

s32 fn_1_6F24(M538_NODE0 *arg0) {
    f32 sp8;
    M538_FW *temp_r31;
    f32 temp_f27;
    f32 temp_f28;
    f32 temp_f30;
    f32 var_f26;
    f32 var_f29;
    f64 temp_f31;
    f64 temp_f31_2;
    f64 temp_f31_3;
    s16 temp_r0;
    s32 var_r30;

    var_r30 = 0;
    temp_r31 = (M538_FW *) arg0->work;
    temp_r0 = temp_r31->state;
    switch (temp_r0) {                              /* irregular */
    case 0:
        temp_f27 = temp_r31->z;
        temp_f28 = temp_r31->x;
        if (temp_f27 == 0.0f) {
            M2C_ERROR(/* unknown instruction: cror eq, gt, eq */);
            if (temp_f28 == 0.0f) {
                var_f29 = 1.5707964f;
            } else {
                var_f29 = -1.5707964f;
            }
        } else {
            var_f29 = (f32) atan2((f64) temp_f28, (f64) temp_f27);
        }
        temp_r31->angle = (f32) ((f64) (180.0f * var_f29) / 3.141592653589793);
        temp_f30 = (temp_r31->x * temp_r31->x) + (temp_r31->z * temp_r31->z);
        if (temp_f30 > 0.0f) {
            temp_f31 = __frsqrte(temp_f30);
            temp_f31_2 = 0.5 * temp_f31 * (3.0 - ((f64) temp_f30 * (temp_f31 * temp_f31)));
            temp_f31_3 = 0.5 * temp_f31_2 * (3.0 - ((f64) temp_f30 * (temp_f31_2 * temp_f31_2)));
            sp8 = (f32) ((f64) temp_f30 * (0.5 * temp_f31_3 * (3.0 - ((f64) temp_f30 * (temp_f31_3 * temp_f31_3)))));
            var_f26 = sp8;
        } else {
            var_f26 = temp_f30;
        }
        temp_r31->dist = var_f26;
        temp_r31->timer = 0;
        temp_r31->state += 1;
        /* fallthrough */
    case 1:
        temp_r31->timer += 1;
        if ((s16) temp_r31->timer == 0xF0) {
            Hu3DModelAttrSet(temp_r31->model, 1U);
            var_r30 = 1;
        }
        temp_r31->angle = (f32) (temp_r31->angle - 6.1f);
        temp_r31->dist = (f32) (temp_r31->dist + 4.1f);
        temp_r31->x = (f32) ((f64) temp_r31->dist * sin((3.141592653589793 * (f64) temp_r31->angle) / 180.0));
        temp_r31->z = (f32) ((f64) temp_r31->dist * cos((3.141592653589793 * (f64) temp_r31->angle) / 180.0));
        temp_r31->radius2 = (f32) (temp_r31->radius2 + 4.0f);
loop_17:
        if (temp_r31->radius2 > 180.0f) {
            temp_r31->radius2 = (f32) (temp_r31->radius2 - 360.0f);
            goto loop_17;
        }
loop_20:
        if (temp_r31->radius2 < -180.0f) {
            temp_r31->radius2 = (f32) (temp_r31->radius2 + 360.0f);
            goto loop_20;
        }
        Hu3DModelPosSetV(temp_r31->model, (HuVecF *) temp_r31);
        Hu3DModelRotSetV(temp_r31->model, (HuVecF *) &temp_r31->rotX);
        break;
    }
    return var_r30;
}

void fn_1_72CC(M538_CAMSWING *arg0) {
    s16 temp_r3;
    s16 temp_r3_2;
    s16 temp_r3_3;
    s16 temp_r3_4;
    s16 var_r31;
    s16 var_r31_2;
    s16 var_r31_3;
    s16 var_r31_4;
    M538 *temp_r30;

    temp_r30 = lbl_1_bss_C->data;
    if ((s16) temp_r30->models[10] < 0) {
        temp_r3 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_264[0xA], 0x10000000, HEAP_MODEL));
        temp_r30->models[10] = temp_r3;
        var_r31 = temp_r3;
    } else {
        var_r31 = Hu3DModelLink(temp_r30->models[10]);
    }
    arg0->model[0] = var_r31;
    Hu3DModelAttrSet(var_r31, 1U);
    Hu3DModelAttrSet(var_r31, 0x40000001U);
    Hu3DModelLayerSet(var_r31, 7);
    Hu3DModelPosSet(var_r31, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(var_r31, 0.0f, 0.0f, 0.0f);
    if ((s16) temp_r30->models[19] < 0) {
        temp_r3_2 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_264[0x13], 0x10000000, HEAP_MODEL));
        temp_r30->models[19] = temp_r3_2;
        var_r31_2 = temp_r3_2;
    } else {
        var_r31_2 = Hu3DModelLink(temp_r30->models[19]);
    }
    arg0->model[1] = var_r31_2;
    Hu3DModelAttrSet(var_r31_2, 1U);
    Hu3DModelAttrSet(var_r31_2, 0x40000002U);
    Hu3DModelLayerSet(var_r31_2, 2);
    Hu3DModelPosSet(var_r31_2, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(var_r31_2, 0.0f, 0.0f, 0.0f);
    if ((s16) temp_r30->models[20] < 0) {
        temp_r3_3 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_264[0x14], 0x10000000, HEAP_MODEL));
        temp_r30->models[20] = temp_r3_3;
        var_r31_3 = temp_r3_3;
    } else {
        var_r31_3 = Hu3DModelLink(temp_r30->models[20]);
    }
    arg0->model[2] = var_r31_3;
    Hu3DModelAttrSet(var_r31_3, 1U);
    Hu3DModelAttrSet(var_r31_3, 0x40000002U);
    Hu3DModelLayerSet(var_r31_3, 2);
    Hu3DModelPosSet(var_r31_3, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(var_r31_3, 0.0f, 0.0f, 0.0f);
    if ((s16) temp_r30->models[23] < 0) {
        temp_r3_4 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_264[0x17], 0x10000000, HEAP_MODEL));
        temp_r30->models[23] = temp_r3_4;
        var_r31_4 = temp_r3_4;
    } else {
        var_r31_4 = Hu3DModelLink(temp_r30->models[23]);
    }
    arg0->model[3] = var_r31_4;
    Hu3DModelAttrSet(var_r31_4, 1U);
    Hu3DModelAttrSet(var_r31_4, 0x40000002U);
    Hu3DModelLayerSet(var_r31_4, 7);
    Hu3DModelPosSet(var_r31_4, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(var_r31_4, 0.0f, 0.0f, 0.0f);
    arg0->radius = 0.0f;
    arg0->unk20 = 150.0f;
    arg0->pad1C = 0.0f;
    arg0->state = 0;
}

void fn_1_765C(M538_CAMSWING *arg0) {
    switch (arg0->state) {
    case 0:
        arg0->counter = 0;
        arg0->radius = 30.0f;
        Hu3DModelAttrReset(arg0->model[0], 1U);
        Hu3DModelAttrReset(arg0->model[1], 1U);
        Hu3DModelAttrReset(arg0->model[1], 0x40000002U);
        Hu3DModelAttrReset(arg0->model[3], 1U);
        Hu3DModelAttrReset(arg0->model[3], 0x40000002U);
        Hu3DMotionSpeedSet(arg0->model[3], 0.6f);
        Hu3DModelHookSet(arg0->model[1], lbl_1_data_418, arg0->model[0]);
        arg0->state += 1;
        /* fallthrough */
    case 1:
        if (Hu3DMotionEndCheck(arg0->model[1]) != 0) {
            Hu3DModelHookReset(arg0->model[1]);
            Hu3DModelAttrSet(arg0->model[0], 1U);
            Hu3DModelAttrSet(arg0->model[1], 1U);
            arg0->state += 1;
        }
        break;
    case 2:
        if (Hu3DMotionEndCheck(arg0->model[3]) != 0) {
            Hu3DModelAttrSet(arg0->model[3], 1U);
            arg0->state += 1;
        }
        break;
    case 3:
        break;
    }
}

void fn_1_77A4(M538_CAMSWING *arg0) {
    switch (arg0->state) {
    case 0:
        arg0->counter = 0;
        arg0->state++;
        arg0->radius = 30.0f;
        Hu3DModelAttrReset(arg0->model[0], 1U);
        Hu3DModelAttrReset(arg0->model[2], 1U);
        Hu3DModelAttrReset(arg0->model[2], 0x40000002U);
        Hu3DModelHookSet(arg0->model[2], lbl_1_data_418, arg0->model[0]);
        /* fallthrough */
    case 1:
        if (Hu3DMotionEndCheck(arg0->model[2]) != 0) {
            Hu3DModelHookReset(arg0->model[2]);
            Hu3DModelAttrSet(arg0->model[0], 1U);
            Hu3DModelAttrSet(arg0->model[2], 1U);
            arg0->state++;
        }
        break;
    default:
        break;
    }
}

/* CodeWarrior emits a dead "cmpwi r0,3 / b end" upper bound in this switch
 * tree; MWCC roots the same search at 1 and drops the bound. */
s32 fn_1_788C(M538_CAMWORK *arg0) {
    f32 temp_f30;
    f32 temp_f31;
    s32 var_r30;

    var_r30 = 0;
    switch (arg0->state) {                              /* irregular */
    case 0:
        arg0->counter = 0;
        arg0->state++;
        /* fallthrough */
    case 1:
        arg0->counter++;
        if (arg0->counter == 0xB4) {
            var_r30 = 1;
        }
        temp_f31 = (f32) sin((3.141592653589793 * (f64) ((90.0f * (f32) arg0->counter) / 180.0f)) / 180.0);
        temp_f30 = 1.0f - temp_f31;
        arg0->theta = (f32) ((lbl_1_data_2E4[3] * temp_f30) + (lbl_1_data_2E4[9] * temp_f31));
        arg0->phi = (f32) ((lbl_1_data_2E4[4] * temp_f30) + (lbl_1_data_2E4[10] * temp_f31));
        arg0->radius = (f32) ((lbl_1_data_2E4[5] * temp_f30) + (lbl_1_data_2E4[11] * temp_f31));
        arg0->x = (f32) ((lbl_1_data_2E4[0] * temp_f30) + (lbl_1_data_2E4[6] * temp_f31));
        arg0->y = (f32) ((lbl_1_data_2E4[1] * temp_f30) + (lbl_1_data_2E4[7] * temp_f31));
        arg0->z = (f32) ((lbl_1_data_2E4[2] * temp_f30) + (lbl_1_data_2E4[8] * temp_f31));
        break;
    }
    return var_r30;
}

s32 fn_1_7AA0(M538_CAMWORK *arg0) {
    s32 var_r30;
    f32 temp_f30;
    f32 temp_f31;

    var_r30 = 0;
    switch (arg0->state) {                              /* irregular */
    case 0:
        arg0->counter = 0;
        arg0->state++;
        /* fallthrough */
    case 1:
        if (arg0->counter < 0x708) {
            arg0->counter++;
        }
        temp_f31 = (f32) sin((3.141592653589793 * (f64) ((90.0f * (f32) arg0->counter) / 1800.0f)) / 180.0);
        temp_f30 = 1.0f - temp_f31;
        arg0->theta = (f32) ((lbl_1_data_2E4[9] * temp_f30) + (lbl_1_data_2E4[15] * temp_f31));
        arg0->phi = (f32) ((lbl_1_data_2E4[10] * temp_f30) + (lbl_1_data_2E4[16] * temp_f31));
        arg0->radius = (f32) ((lbl_1_data_2E4[11] * temp_f30) + (lbl_1_data_2E4[17] * temp_f31));
        arg0->x = (f32) ((lbl_1_data_2E4[6] * temp_f30) + (lbl_1_data_2E4[12] * temp_f31));
        arg0->y = (f32) ((lbl_1_data_2E4[7] * temp_f30) + (lbl_1_data_2E4[13] * temp_f31));
        arg0->z = (f32) ((lbl_1_data_2E4[8] * temp_f30) + (lbl_1_data_2E4[14] * temp_f31));
        break;
    }
    return var_r30;
}

s32 fn_1_7CB0(M538_CAMWORK *arg0) {
    f32 temp_f31;
    s32 var_r30;

    var_r30 = 0;
    switch (arg0->state) {
    case 0:
        arg0->counter = 0;
        arg0->from[3] = (f32) arg0->theta;
        arg0->from[4] = (f32) arg0->phi;
        arg0->from[5] = (f32) arg0->radius;
        arg0->from[0] = (f32) arg0->x;
        arg0->from[1] = (f32) arg0->y;
        arg0->from[2] = (f32) arg0->z;
        arg0->state += 1;
        /* fallthrough */
    case 1:
        arg0->counter += 1;
        if (arg0->counter == 0x96) {
            var_r30 = 1;
        }
        temp_f31 = (f32) sin((3.141592653589793 * (f64) ((90.0f * (f32) arg0->counter) / 150.0f)) / 180.0);
        arg0->theta = (f32) ((arg0->from[3] * (1.0f - temp_f31)) + (lbl_1_data_2E4[3] * temp_f31));
        arg0->phi = (f32) ((arg0->from[4] * (1.0f - temp_f31)) + (lbl_1_data_2E4[4] * temp_f31));
        arg0->radius = (f32) ((arg0->from[5] * (1.0f - temp_f31)) + (lbl_1_data_2E4[5] * temp_f31));
        arg0->x = (f32) ((arg0->from[0] * (1.0f - temp_f31)) + (lbl_1_data_2E4[0] * temp_f31));
        arg0->y = (f32) ((arg0->from[1] * (1.0f - temp_f31)) + (lbl_1_data_2E4[1] * temp_f31));
        arg0->z = (f32) ((arg0->from[2] * (1.0f - temp_f31)) + (lbl_1_data_2E4[2] * temp_f31));
        break;
    }
    return var_r30;
}

s32 fn_1_7F04(M538_PLAYER *arg0) {
    s32 var_r30;

    var_r30 = 0;
    switch (arg0->state) {                          /* irregular */
    case 0:
        arg0->state += 1;
        break;
    case 2:
        CharMotionShiftSet(arg0->charNo, arg0->obj->mtnId[8], 0.0f, 5.0f, 0U);
        arg0->state += 1;
        break;
    case 4: /* MWCC keeps a redundant beq to the empty case 4 body here */
        break;
    }
    return var_r30;
}

s32 fn_1_7FAC(M538_PLAYER *arg0) {
    f32 (*sp20)[4];
    Point3d sp14;
    Point3d sp8;
    M538_NODE0 *temp_r30;
    M538_NODE *temp_r3;
    M538_NODE *temp_r3_2;
    f32 temp_f1;
    f32 temp_f1_2;
    f32 temp_f28;
    f32 temp_f29;
    f32 var_f30;
    u32 temp_r0;
    M538 *temp_r28;

    temp_r28 = (M538 *) lbl_1_bss_C->data;
    temp_r0 = arg0->state;
    switch (temp_r0) {
    case 0:
        temp_f28 = -arg0->z;
        temp_f29 = -arg0->x;
        if (temp_f28 == 0.0f) {
            M2C_ERROR(/* unknown instruction: cror eq, gt, eq */);
            if (temp_f29 == 0.0f) {
                var_f30 = 1.5707964f;
            } else {
                var_f30 = -1.5707964f;
            }
        } else {
            var_f30 = (f32) atan2((f64) temp_f29, (f64) temp_f28);
        }
        arg0->targetRot = (f32) ((f64) (180.0f * var_f30) / 3.141592653589793);
        CharMotionShiftSet(arg0->charNo, *arg0->obj->mtnId, 0.0f, 5.0f, 0x40000001U);
        arg0->state = (u32) (arg0->state + 1);
        /* fallthrough */
    case 1:
        if (((s16) *arg0->obj->mtnId == Hu3DMotionIDGet(arg0->modelId)) && (Hu3DMotionShiftIDGet(arg0->modelId) == -1)) {
            arg0->state = (u32) (arg0->state + 1);
        }
        break;
    case 2:
        CharMotionSet(arg0->charNo, arg0->obj->mtnId[2]);
        arg0->state = (u32) (arg0->state + 1);
        /* fallthrough */
    case 3:
        if (((arg0->x * arg0->x) + (arg0->z * arg0->z)) < 225.0f) {
            arg0->state = (u32) (arg0->state + 1);
        } else {
            arg0->velX = (f32) (7.5 * sin((3.141592653589793 * (f64) arg0->targetRot) / 180.0));
            arg0->velZ = (f32) (7.5 * cos((3.141592653589793 * (f64) arg0->targetRot) / 180.0));
            temp_f1 = Hu3DMotionTimeGet(arg0->modelId);
            if ((temp_f1 == 15.0f) || (temp_f1 == 30.0f)) {
                temp_r3 = fn_1_8858(arg0->pool1, (M538_NODEFUNC) fn_1_8CD8);
                if (temp_r3 != NULL) {
                    temp_r3->rotY = (f32) arg0->x;
                    temp_r3->rotZ = (s32) arg0->y;
                    temp_r3->x = (f32) arg0->z;
                    temp_r3->rotX = 0;
                }
            }
        }
        break;
    case 4:
        arg0->targetRot = 0.0f;
        arg0->rotSpeed = 5.0f;
        arg0->state = (u32) (arg0->state + 1);
        /* fallthrough */
    case 5:
        temp_f1_2 = Hu3DMotionTimeGet(arg0->modelId);
        if ((temp_f1_2 == 15.0f) || (temp_f1_2 == 30.0f)) {
            temp_r3_2 = fn_1_8858(arg0->pool1, (M538_NODEFUNC) fn_1_8CD8);
            if (temp_r3_2 != NULL) {
                temp_r3_2->rotY = (f32) arg0->x;
                temp_r3_2->rotZ = (s32) arg0->y;
                temp_r3->x = (f32) arg0->z;
                temp_r3->rotX = 0;
            }
        }
        if ((f32) fabs(arg0->rotY) < 1.0f) {
            arg0->state = (u32) (arg0->state + 1);
        }
        break;
    case 6:
        Hu3DModelObjMtxGet(arg0->modelId, CharModelItemHookGet(arg0->charNo, 8, 0), sp20);
        Hu3DMtxTransGet(sp20, &sp8);
        Hu3DMtxRotGet(sp20, &sp14);
        Hu3DModelHookReset(arg0->modelId);
        arg0->hookX = sp8.x;
        arg0->hookY = sp8.y;
        arg0->hookZ = sp8.z;
        arg0->hookRotX = sp14.x;
        arg0->hookRotY = sp14.y;
        arg0->hookRotZ = sp14.z;
        Hu3DModelPosSetV(arg0->itemModel, &sp8);
        Hu3DModelRotSetV(arg0->itemModel, &sp14);
        temp_r30 = temp_r28->node0Free;
        if (temp_r30 != NULL) {
            temp_r28->node0Free = temp_r30->next;
            temp_r30->func = (M538_NODE0FUNC) fn_1_9024;
            temp_r30->work = &arg0->fw;
            temp_r30->next = temp_r28->node0Head;
            if (temp_r30->next != NULL) {
                temp_r30->next->prev = temp_r30;
            }
            temp_r28->node0Head = temp_r30;
        }
        if (temp_r30 != NULL) {
            arg0->fw.x = arg0->hookX;
            arg0->fw.y = (f32) arg0->hookY;
            arg0->fw.z = arg0->hookZ;
            arg0->fw.rotX = arg0->hookRotX;
            arg0->fw.radius2 = arg0->hookRotY;
            arg0->fw.radius3 = arg0->hookRotZ;
            arg0->fw.model = (s16) arg0->itemModel;
            arg0->fw.state = 0;
        }
        CharMotionShiftSet(arg0->charNo, arg0->obj->mtnId[9], 0.0f, 5.0f, 0x40000001U);
        arg0->state = (u32) (arg0->state + 1);
        /* fallthrough */
    case 7:
        arg0->func = fn_1_7F04;
        arg0->state = 0U;
        break;
    }
    return 0;
}

s32 fn_1_8554(M538_PLAYER *arg0) {
    M538_NODE *temp_r3;
    s32 var_r29 = 0;
    union m538_f4 sp;

    sp.w = lbl_1_rodata_1CC.w;
    switch (arg0->state) {                           /* irregular */
    case 0:
        Hu3DModelShadowReset(*arg0->obj->mdlId);
        arg0->counter2 = 0;
        arg0->state += 1;
        /* fallthrough */
    case 1:
        arg0->counter2 += 1;
        if ((s16) arg0->counter2 == arg0->counter3) {
            arg0->counter2 = 0;
            arg0->counter3 = 0;
            arg0->state += 1;
        }
        break;
    case 2:
        CharModelAttrReset(arg0->charNo, 1U);
        CharMotionSet(arg0->charNo, arg0->obj->mtnId[9]);
        CharModelAttrSet(arg0->charNo, 0x40000001U);
        CharModelAttrReset(arg0->charNo, 0x40000002U);
        arg0->x = sp.f[arg0->playerNo];
        arg0->y = 800.0f;
        arg0->z = 0.0f;
        arg0->state += 1;
        /* fallthrough */
    case 3:
        if (arg0->y < 600.0f) {
            Hu3DModelShadowSet(*arg0->obj->mdlId);
            arg0->state += 1;
        }
        /* fallthrough */
    case 4:
        arg0->counter2 += 1;
        arg0->y = (f32) (arg0->y - (1.3f * (f32) arg0->counter2));
        if (arg0->y <= 10.0f) {
            temp_r3 = fn_1_8858(arg0->pool1, (M538_NODEFUNC) fn_1_8CD8);
            if (temp_r3 != 0) {
                *(struct m538_xy *) &temp_r3->x = *(struct m538_xy *) &arg0->x;
                *(s32 *) &temp_r3->z = *(s32 *) &arg0->z;
                temp_r3->timer = 0;
            }
            arg0->y = 0.0f;
            arg0->state += 1;
        }
        break;
    case 5:
        arg0->func = 0;
        arg0->state = 0;
        break;
    }
    return var_r29;
}

void fn_1_87E4(M538_POOL *arg0, M538_NODE *arg1, s32 arg2) {
    s32 i;

    arg0->head = NULL;
    arg0->free = arg1;
    i = 0;
    while (i < arg2 - 1) {
        arg1->prev = NULL;
        arg1->next = (M538_NODE *) ((u8 *) arg1 + sizeof(M538_NODE));
        arg1->func = NULL;
        arg1->pool = arg0;
        i++;
        arg1 = (M538_NODE *) ((u8 *) arg1 + sizeof(M538_NODE));
    }
    arg1->prev = NULL;
    arg1->next = NULL;
    arg1->func = NULL;
    arg1->pool = arg0;
}

M538_NODE *fn_1_8858(M538_POOL *arg0, M538_NODEFUNC arg1) {
    M538_NODE *temp_r31;

    temp_r31 = arg0->free;
    if (temp_r31 != 0) {
        arg0->free = temp_r31->next;
        temp_r31->func = arg1;
        temp_r31->next = arg0->head;
        if (temp_r31->next != 0) {
            temp_r31->next->prev = temp_r31;
        }
        arg0->head = temp_r31;
    }
    return temp_r31;
}

void fn_1_88A8(M538_NODE *arg0) {
    M538_POOL *temp_r31;

    if (arg0 != 0) {
        temp_r31 = arg0->pool;
        if (arg0->prev != 0) {
            arg0->prev->next = arg0->next;
        } else {
            temp_r31->head = arg0->next;
        }
        if (arg0->next != 0) {
            arg0->next->prev = arg0->prev;
        }
        arg0->prev = NULL;
        arg0->next = temp_r31->free;
        arg0->func = 0;
        temp_r31->free = arg0;
    }
}

void fn_1_8920(M538_POOL *arg0) {
    M538_NODE *var_r31;
    M538_POOL *temp_r30;
    M538_NODE *temp_r29;

    var_r31 = arg0->head;
    while (var_r31 != 0) {
        temp_r29 = var_r31->next;
        if ((var_r31->func != 0) && (var_r31->func(var_r31) != 0) && (var_r31 != 0)) {
            temp_r30 = var_r31->pool;
            if (var_r31->prev != 0) {
                var_r31->prev->next = var_r31->next;
            } else {
                temp_r30->head = var_r31->next;
            }
            if (var_r31->next != 0) {
                var_r31->next->prev = var_r31->prev;
            }
            var_r31->prev = NULL;
            var_r31->next = temp_r30->free;
            var_r31->func = NULL;
            temp_r30->free = var_r31;
        }
        var_r31 = temp_r29;
    }
}

void fn_1_8A00(M538 *arg0) {
    s16 var_r28;
    s16 var_r30;
    s32 var_r29;

    var_r29 = 0;
    goto loop_9;
loop_8:
    if (arg0->models[21] < 0) {
        var_r30 = arg0->models[21] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_264[0x15], 0x10000000, HEAP_MODEL));
    } else {
        var_r30 = Hu3DModelLink(arg0->models[21]);
    }
    if (arg0->models[31] < 0) {
        var_r28 = arg0->models[31] = Hu3DJointMotion(var_r30, HuDataSelHeapReadNum(lbl_1_data_2D8[2], 0x10000000, HEAP_MODEL));
    } else {
        var_r28 = arg0->models[31];
    }
    Hu3DMotionSet(var_r30, var_r28);
    arg0->node1[var_r29].model = var_r30;
    Hu3DModelAttrSet(var_r30, 1U);
    Hu3DModelAttrSet(var_r30, 0x40000002U);
    Hu3DModelLayerSet(var_r30, 4);
    Hu3DModelPosSet(var_r30, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(var_r30, 0.0f, 0.0f, 0.0f);
    var_r29 += 1;
loop_9:
    if (var_r29 < 0x10) {
        goto loop_8;
    }
}

void fn_1_8B6C(M538 *arg0) {
    s16 var_r28;
    s16 var_r30;
    s32 var_r29;

    var_r29 = 0;
    goto loop_9;
loop_8:
    if (arg0->models[18] < 0) {
        var_r30 = arg0->models[18] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_264[0x12], 0x10000000, HEAP_MODEL));
    } else {
        var_r30 = Hu3DModelLink(arg0->models[18]);
    }
    if (arg0->models[30] < 0) {
        var_r28 = arg0->models[30] = Hu3DJointMotion(var_r30, HuDataSelHeapReadNum(lbl_1_data_2D8[1], 0x10000000, HEAP_MODEL));
    } else {
        var_r28 = arg0->models[30];
    }
    Hu3DMotionSet(var_r30, var_r28);
    arg0->node2[var_r29].model = var_r30;
    Hu3DModelAttrSet(var_r30, 1U);
    Hu3DModelAttrSet(var_r30, 0x40000002U);
    Hu3DModelLayerSet(var_r30, 4);
    Hu3DModelPosSet(var_r30, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(var_r30, 0.0f, 0.0f, 0.0f);
    var_r29 += 1;
loop_9:
    if (var_r29 < 0x10) {
        goto loop_8;
    }
}

s32 fn_1_8CD8(M538_NODE *arg0) {
    u8 sp1F;
    u8 sp1E;
    u8 sp1D;
    u8 sp1C;
    u8 sp1B;
    u8 sp1A;
    u8 sp19;
    u8 sp18;
    u8 sp17;
    u8 sp16;
    u8 sp15;
    u8 sp14;
    u8 sp13;
    u8 sp12;
    u8 sp11;
    u8 sp10;
    u8 spF;
    u8 spE;
    u8 spD;
    u8 spC;
    u8 spB;
    u8 spA;
    u8 sp9;
    u8 sp8;
    HSFMATERIAL *temp_r31;
    HSFMATERIAL *temp_r31_2;
    HSFMATERIAL *temp_r31_3;
    s16 temp_r0;
    s16 temp_r0_2;
    s32 temp_r29;
    s32 temp_r29_2;
    s32 var_r28;

    var_r28 = 0;
    sp8 = lbl_1_rodata_1EC[0];
    sp9 = lbl_1_rodata_1EC[0];
    spA = lbl_1_rodata_1EC[0];
    spB = lbl_1_rodata_1EC[0];
    spC = lbl_1_rodata_1EC[1];
    spD = lbl_1_rodata_1EC[1];
    spE = lbl_1_rodata_1EC[1];
    spF = lbl_1_rodata_1EC[1];
    sp10 = lbl_1_rodata_1EC[2];
    sp11 = lbl_1_rodata_1EC[2];
    sp12 = lbl_1_rodata_1EC[2];
    sp13 = lbl_1_rodata_1EC[2];
    sp14 = lbl_1_rodata_1EC[3];
    sp15 = lbl_1_rodata_1EC[3];
    sp16 = lbl_1_rodata_1EC[3];
    sp17 = lbl_1_rodata_1EC[3];
    sp18 = lbl_1_rodata_1EC[4];
    sp19 = lbl_1_rodata_1EC[4];
    sp1A = lbl_1_rodata_1EC[4];
    sp1B = lbl_1_rodata_1EC[4];
    sp1C = lbl_1_rodata_1EC[5];
    sp1D = lbl_1_rodata_1EC[5];
    sp1E = lbl_1_rodata_1EC[5];
    sp1F = lbl_1_rodata_1EC[5];
    temp_r0 = arg0->timer;
    switch (temp_r0) {                              /* irregular */
    case 0:
        Hu3DModelAttrReset(arg0->model, 1U);
        Hu3DModelAttrReset(arg0->model, 0x40000002U);
        Hu3DModelPosSetV(arg0->model, (HuVecF *) &arg0->x);
        Hu3DMotionTimeSet(arg0->model, 0.0f);
        temp_r0_2 = arg0->model;
        temp_r31 = Hu3DData[temp_r0_2].hsf->material;
        temp_r29 = fn_1_91D8((f32 *) &arg0->x, temp_r0_2 * 0x134) * 2;
        temp_r31->color[0] = *(&sp8 + (temp_r29 * 3));
        temp_r31->color[1] = *(&sp8 + ((temp_r29 * 3) + 1));
        temp_r31->color[2] = *(&sp8 + ((temp_r29 * 3) + 2));
        temp_r31_2 = temp_r31 + 0x3C;
        temp_r31_2->color[0] = *(&sp8 + (temp_r29 * 3));
        temp_r31_2->color[1] = *(&sp8 + ((temp_r29 * 3) + 1));
        temp_r31_2->color[2] = *(&sp8 + ((temp_r29 * 3) + 2));
        temp_r31_3 = temp_r31_2 + 0x3C;
        temp_r29_2 = temp_r29 + 1;
        temp_r31_3->color[0] = *(&sp8 + (temp_r29_2 * 3));
        temp_r31_3->color[1] = *(&sp8 + ((temp_r29_2 * 3) + 1));
        temp_r31_3->color[2] = *(&sp8 + ((temp_r29_2 * 3) + 2));
        arg0->timer += 1;
        /* fallthrough */
    case 1:
        if (Hu3DMotionEndCheck(arg0->model) != 0) {
            Hu3DModelAttrSet(arg0->model, 1U);
            Hu3DModelAttrSet(arg0->model, 0x40000002U);
            var_r28 = 1;
        }
        break;
    }
    return var_r28;
}

s32 fn_1_8F58(M538_NODE *arg0) {
    s32 var_r30;

    var_r30 = 0;
    switch (arg0->timer) {
    case 0:
        Hu3DModelAttrReset(arg0->model, 1U);
        Hu3DModelAttrReset(arg0->model, 0x40000002U);
        Hu3DModelPosSetV(arg0->model, (HuVecF *) &arg0->x);
        Hu3DMotionTimeSet(arg0->model, 0.0f);
        arg0->timer++;
        /* fallthrough */
    case 1:
        if (Hu3DMotionEndCheck(arg0->model) != 0) {
            Hu3DModelAttrSet(arg0->model, 1U);
            Hu3DModelAttrSet(arg0->model, 0x40000002U);
            var_r30 = 1;
        }
        break;
    }
    return var_r30;
}

s32 fn_1_9024(M538_NODE0 *arg0) {
    M538_FW *temp_r31;
    f32 temp_f31;
    s32 var_r30;

    var_r30 = 0;
    temp_r31 = (M538_FW *) arg0->work;
    switch (temp_r31->state) {
    case 0:
        temp_r31->timer = 0;
        temp_r31->radius = temp_r31->radius2;
        temp_r31->angle = temp_r31->radius3;
        temp_r31->rotX = 0.0f;
        temp_r31->radius2 = 0.0f;
        temp_r31->radius3 = 0.0f;
        temp_r31->state += 1;
        /* fallthrough */
    case 1:
        temp_r31->rotX = (f32) (temp_r31->rotX - 40.0f);
        temp_r31->y += 3.0f;
        temp_r31->z -= 6.0f;
        temp_r31->timer += 1;
        if (temp_r31->timer == 0x3C) {
            var_r30 = 1;
        }
        temp_f31 = 1.0f - ((f32) temp_r31->timer / 60.0f);
        temp_r31->radius2 = (f32) (temp_r31->radius * temp_f31);
        temp_r31->radius3 = (f32) (temp_r31->angle * temp_f31);
        Hu3DModelScaleSet(temp_r31->model, temp_f31, temp_f31, temp_f31);
        Hu3DModelPosSetV(temp_r31->model, (HuVecF *) temp_r31);
        Hu3DModelRotSetV(temp_r31->model, (HuVecF *) &temp_r31->rotX);
        break;
    }
    return var_r30;
}

s32 fn_1_91D8(f32 *arg0, s32 arg1) {
    f32 var_f31;
    f32 temp_f30;
    f32 temp_f29;
    f32 temp_f28;
    f32 temp_f27;
    f32 temp_f26;
    f32 temp_f25;
    f32 temp_f24;
    s32 var_r30;

    var_r30 = -1;
    temp_f30 = (arg0[0] * arg0[0]) + (arg0[2] * arg0[2]);
    if (temp_f30 < 62500.0f) {
        var_r30 = 0;
    } else if (temp_f30 < 250000.0f) {
        temp_f27 = arg0[2];
        temp_f28 = arg0[0];
        if (temp_f27 == 0.0f) {
            if (temp_f28 >= 0.0f) {
                var_f31 = 1.5707964f;
            } else {
                var_f31 = -1.5707964f;
            }
        } else {
            temp_f24 = (f32) atan2((f64) temp_f28, (f64) temp_f27);
            var_f31 = temp_f24;
        }
        temp_f26 = (f32) ((180.0f * var_f31) / 3.141592653589793);
        temp_f25 = (f32) fabs(temp_f26);
        temp_f29 = temp_f25;
        if ((temp_f29 >= 45.0f) && (temp_f29 < 135.0f)) {
            var_r30 = 2;
        } else {
            var_r30 = 1;
        }
    } else {
        var_r30 = 3;
    }
    return var_r30;
}

void fn_1_93C4(M538 *arg0) {
    s16 linkedModel;
    s32 i;

    i = 0;
    goto cond_5;
loop_5:
    if (arg0->models[22] < 0) {
        linkedModel = arg0->models[22] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_264[0x16], 0x10000000, HEAP_MODEL));
    } else {
        linkedModel = Hu3DModelLink(arg0->models[22]);
    }
    arg0->node3[i].model = linkedModel;
    Hu3DModelAttrSet(linkedModel, 1U);
    Hu3DModelAttrSet(linkedModel, 0x40000002U);
    Hu3DModelLayerSet(linkedModel, 4);
    Hu3DModelPosSet(linkedModel, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(linkedModel, 0.0f, 0.0f, 0.0f);
    i += 1;
cond_5:
    if (i < 0x40) {
        goto loop_5;
    }
}

s32 fn_1_94DC(M538_NODE *arg0) {
    u8 sp37;
    u8 sp36;
    u8 sp35;
    u8 sp34;
    u8 sp33;
    u8 sp32;
    u8 sp31;
    u8 sp30;
    u8 sp2F;
    u8 sp2E;
    u8 sp2D;
    u8 sp2C;
    u8 sp2B;
    u8 sp2A;
    u8 sp29;
    u8 sp28;
    u8 sp27;
    u8 sp26;
    u8 sp25;
    u8 sp24;
    u8 sp23;
    u8 sp22;
    u8 sp21;
    u8 sp20;
    u8 sp1F;
    u8 sp1E;
    u8 sp1D;
    u8 sp1C;
    u8 sp1B;
    u8 sp1A;
    u8 sp19;
    u8 sp18;
    u8 sp17;
    u8 sp16;
    u8 sp15;
    u8 sp14;
    u8 sp13;
    u8 sp12;
    u8 sp11;
    u8 sp10;
    u8 spF;
    u8 spE;
    u8 spD;
    u8 spC;
    u8 spB;
    u8 spA;
    u8 sp9;
    u8 sp8;
    HSFMATERIAL *var_r30;
    f32 temp_f24;
    f32 temp_f26;
    f32 temp_f27;
    f32 temp_f28;
    f32 temp_f31;
    f32 var_f30;
    s16 temp_r0;
    s32 var_r26;
    s32 var_r27;
    s32 var_r28;
    s32 var_r29;

    var_r26 = 0;
    sp8 = lbl_1_rodata_21C[0];
    sp9 = lbl_1_rodata_21C[0];
    spA = lbl_1_rodata_21C[0];
    spB = lbl_1_rodata_21C[0];
    spC = lbl_1_rodata_21C[1];
    spD = lbl_1_rodata_21C[1];
    spE = lbl_1_rodata_21C[1];
    spF = lbl_1_rodata_21C[1];
    sp10 = lbl_1_rodata_21C[2];
    sp11 = lbl_1_rodata_21C[2];
    sp12 = lbl_1_rodata_21C[2];
    sp13 = lbl_1_rodata_21C[2];
    sp14 = lbl_1_rodata_21C[3];
    sp15 = lbl_1_rodata_21C[3];
    sp16 = lbl_1_rodata_21C[3];
    sp17 = lbl_1_rodata_21C[3];
    sp18 = lbl_1_rodata_21C[4];
    sp19 = lbl_1_rodata_21C[4];
    sp1A = lbl_1_rodata_21C[4];
    sp1B = lbl_1_rodata_21C[4];
    sp1C = lbl_1_rodata_21C[5];
    sp1D = lbl_1_rodata_21C[5];
    sp1E = lbl_1_rodata_21C[5];
    sp1F = lbl_1_rodata_21C[5];
    sp20 = lbl_1_rodata_21C[6];
    sp21 = lbl_1_rodata_21C[6];
    sp22 = lbl_1_rodata_21C[6];
    sp23 = lbl_1_rodata_21C[6];
    sp24 = lbl_1_rodata_21C[7];
    sp25 = lbl_1_rodata_21C[7];
    sp26 = lbl_1_rodata_21C[7];
    sp27 = lbl_1_rodata_21C[7];
    sp28 = lbl_1_rodata_21C[8];
    sp29 = lbl_1_rodata_21C[8];
    sp2A = lbl_1_rodata_21C[8];
    sp2B = lbl_1_rodata_21C[8];
    sp2C = lbl_1_rodata_21C[9];
    sp2D = lbl_1_rodata_21C[9];
    sp2E = lbl_1_rodata_21C[9];
    sp2F = lbl_1_rodata_21C[9];
    sp30 = lbl_1_rodata_21C[10];
    sp31 = lbl_1_rodata_21C[10];
    sp32 = lbl_1_rodata_21C[10];
    sp33 = lbl_1_rodata_21C[10];
    sp34 = lbl_1_rodata_21C[11];
    sp35 = lbl_1_rodata_21C[11];
    sp36 = lbl_1_rodata_21C[11];
    sp37 = lbl_1_rodata_21C[11];
    temp_r0 = arg0->timer;
    switch (temp_r0) {                              /* irregular */
    case 0:
        arg0->unk2C = (f32) (360.0f * ((f32) frandmod(0x2711U) / 10000.0f));
        arg0->x = (f32) ((f64) arg0->angle * sin((3.141592653589793 * (f64) arg0->unk2C) / 180.0));
        arg0->y = 0.0f;
        arg0->z = (f32) ((f64) arg0->angle * cos((3.141592653589793 * (f64) arg0->unk2C) / 180.0));
        arg0->rotX = (f32) (50.0f * ((f32) frandmod(0x2711U) / 10000.0f));
        arg0->rotY = (f32) (360.0f + ((f32) frandmod(0x2711U) / 10000.0f));
        arg0->rotZ = 0.0f;
        Hu3DModelAttrReset(arg0->model, 1U);
        var_r30 = Hu3DData[arg0->model].hsf->material;
        temp_f28 = (arg0->x * arg0->x) + (arg0->z * arg0->z);
        if (temp_f28 < 62500.0f) {
            var_r28 = 0;
        } else if (temp_f28 < 250000.0f) {
            temp_f26 = arg0->z;
            temp_f27 = arg0->x;
            if (temp_f26 == 0.0f) {
                M2C_ERROR(/* unknown instruction: cror eq, gt, eq */);
                if (temp_f27 == 0.0f) {
                    var_f30 = 1.5707964f;
                } else {
                    var_f30 = -1.5707964f;
                }
            } else {
                var_f30 = (f32) atan2((f64) temp_f27, (f64) temp_f26);
            }
            temp_f24 = (f32) fabs((f32) ((f64) (180.0f * var_f30) / 3.141592653589793));
            M2C_ERROR(/* unknown instruction: cror eq, gt, eq */);
            if ((temp_f24 == 45.0f) && (temp_f24 < 135.0f)) {
                var_r28 = 2;
            } else {
                var_r28 = 1;
            }
        } else {
            var_r28 = 3;
        }
        var_r29 = var_r28 * 4;
        var_r27 = 0;
loop_19:
        if (var_r27 < 4) {
            var_r30->color[0] = *(&sp8 + (var_r29 * 3));
            var_r30->color[1] = *(&sp8 + ((var_r29 * 3) + 1));
            var_r30->color[2] = *(&sp8 + ((var_r29 * 3) + 2));
            var_r27 += 1;
            var_r30 += 0x3C;
            var_r29 += 1;
            goto loop_19;
        }
        arg0->pad32 = 0;
        arg0->timer += 1;
        /* fallthrough */
    case 1:
        arg0->pad32 += 1;
        if ((s16) arg0->pad32 == 0xB4) {
            Hu3DModelAttrSet(arg0->model, 1U);
            var_r26 = 1;
        }
        temp_f31 = 1.0f + ((f32) arg0->pad32 / 240.0f);
        arg0->unk2C = (f32) (arg0->unk2C - 5.3f);
        arg0->angle = (f32) (arg0->angle + 1.1f);
        arg0->x = (f32) ((f64) arg0->angle * sin((3.141592653589793 * (f64) arg0->unk2C) / 180.0));
        arg0->y = (f32) (arg0->y + 4.1f);
        arg0->z = (f32) ((f64) arg0->angle * cos((3.141592653589793 * (f64) arg0->unk2C) / 180.0));
        arg0->rotY = (f32) (arg0->rotY - 1.1f);
        Hu3DModelPosSetV(arg0->model, (HuVecF *) &arg0->x);
        Hu3DModelRotSetV(arg0->model, (HuVecF *) &arg0->rotX);
        Hu3DModelScaleSet(arg0->model, temp_f31, temp_f31, temp_f31);
        break;
    }
    return var_r26;
}

s32 fn_1_9C04(M538_NODE *arg0) {
    u8 sp37;
    u8 sp36;
    u8 sp35;
    u8 sp34;
    u8 sp33;
    u8 sp32;
    u8 sp31;
    u8 sp30;
    u8 sp2F;
    u8 sp2E;
    u8 sp2D;
    u8 sp2C;
    u8 sp2B;
    u8 sp2A;
    u8 sp29;
    u8 sp28;
    u8 sp27;
    u8 sp26;
    u8 sp25;
    u8 sp24;
    u8 sp23;
    u8 sp22;
    u8 sp21;
    u8 sp20;
    u8 sp1F;
    u8 sp1E;
    u8 sp1D;
    u8 sp1C;
    u8 sp1B;
    u8 sp1A;
    u8 sp19;
    u8 sp18;
    u8 sp17;
    u8 sp16;
    u8 sp15;
    u8 sp14;
    u8 sp13;
    u8 sp12;
    u8 sp11;
    u8 sp10;
    u8 spF;
    u8 spE;
    u8 spD;
    u8 spC;
    u8 spB;
    u8 spA;
    u8 sp9;
    u8 sp8;
    HSFMATERIAL *var_r30;
    f32 temp_f30;
    f32 temp_f31;
    f32 temp_f31_2;
    s16 temp_r0;
    s32 var_r26;
    s32 var_r27;
    s32 var_r29;
    M538 *temp_r28;

    var_r26 = 0;
    sp8 = lbl_1_rodata_258[0];
    sp9 = lbl_1_rodata_258[0];
    spA = lbl_1_rodata_258[0];
    spB = lbl_1_rodata_258[0];
    spC = lbl_1_rodata_258[1];
    spD = lbl_1_rodata_258[1];
    spE = lbl_1_rodata_258[1];
    spF = lbl_1_rodata_258[1];
    sp10 = lbl_1_rodata_258[2];
    sp11 = lbl_1_rodata_258[2];
    sp12 = lbl_1_rodata_258[2];
    sp13 = lbl_1_rodata_258[2];
    sp14 = lbl_1_rodata_258[3];
    sp15 = lbl_1_rodata_258[3];
    sp16 = lbl_1_rodata_258[3];
    sp17 = lbl_1_rodata_258[3];
    sp18 = lbl_1_rodata_258[4];
    sp19 = lbl_1_rodata_258[4];
    sp1A = lbl_1_rodata_258[4];
    sp1B = lbl_1_rodata_258[4];
    sp1C = lbl_1_rodata_258[5];
    sp1D = lbl_1_rodata_258[5];
    sp1E = lbl_1_rodata_258[5];
    sp1F = lbl_1_rodata_258[5];
    sp20 = lbl_1_rodata_258[6];
    sp21 = lbl_1_rodata_258[6];
    sp22 = lbl_1_rodata_258[6];
    sp23 = lbl_1_rodata_258[6];
    sp24 = lbl_1_rodata_258[7];
    sp25 = lbl_1_rodata_258[7];
    sp26 = lbl_1_rodata_258[7];
    sp27 = lbl_1_rodata_258[7];
    sp28 = lbl_1_rodata_258[8];
    sp29 = lbl_1_rodata_258[8];
    sp2A = lbl_1_rodata_258[8];
    sp2B = lbl_1_rodata_258[8];
    sp2C = lbl_1_rodata_258[9];
    sp2D = lbl_1_rodata_258[9];
    sp2E = lbl_1_rodata_258[9];
    sp2F = lbl_1_rodata_258[9];
    sp30 = lbl_1_rodata_258[10];
    sp31 = lbl_1_rodata_258[10];
    sp32 = lbl_1_rodata_258[10];
    sp33 = lbl_1_rodata_258[10];
    sp34 = lbl_1_rodata_258[11];
    sp35 = lbl_1_rodata_258[11];
    sp36 = lbl_1_rodata_258[11];
    sp37 = lbl_1_rodata_258[11];
    temp_r28 = lbl_1_bss_C->data;
    temp_r0 = arg0->timer;
    switch (temp_r0) {                              /* irregular */
    case 0:
        temp_f31 = (180.0f * ((f32) frandmod(0x2711U) / 10000.0f)) - 90.0f;
        temp_f30 = 500.0f * ((f32) frandmod(0x2711U) / 10000.0f);
        arg0->x = (f32) ((f64) temp_f30 * sin((3.141592653589793 * (f64) temp_f31) / 180.0));
        arg0->y = 700.0f;
        arg0->z = (f32) ((f64) temp_f30 * cos((3.141592653589793 * (f64) temp_f31) / 180.0));
        arg0->angle = (f32) (2.0f * (2.0f + ((f32) frandmod(0x2711U) / 10000.0f)));
        arg0->unk2C = 0.0f;
        arg0->rotX = (f32) (50.0f * ((f32) frandmod(0x2711U) / 10000.0f));
        arg0->rotY = (f32) (360.0f + ((f32) frandmod(0x2711U) / 10000.0f));
        arg0->rotZ = 0.0f;
        Hu3DModelAttrReset(arg0->model, 1U);
        Hu3DModelScaleSet(arg0->model, 1.0f, 1.0f, 1.0f);
        var_r30 = Hu3DData[arg0->model].hsf->material;
        if (temp_r28->lightRadius < 250.0f) {
            var_r29 = frandmod(4U) * 4;
        } else if (temp_r28->lightRadius < 500.0f) {
            var_r29 = (frandmod(3U) + 1) * 4;
        } else {
            var_r29 = 0xC;
        }
        var_r27 = 0;
loop_11:
        if (var_r27 < 4) {
            var_r30->color[0] = *(&sp8 + (var_r29 * 3));
            var_r30->color[1] = *(&sp8 + ((var_r29 * 3) + 1));
            var_r30->color[2] = *(&sp8 + ((var_r29 * 3) + 2));
            var_r27 += 1;
            var_r30 += 0x3C;
            var_r29 += 1;
            goto loop_11;
        }
        arg0->pad32 = 0;
        arg0->timer += 1;
        /* fallthrough */
    case 1:
        arg0->unk2C = (f32) (arg0->unk2C + 15.0f);
        if (arg0->unk2C > 180.0f) {
            arg0->unk2C = (f32) (arg0->unk2C - 360.0f);
        }
        temp_f31_2 = (f32) sin((3.141592653589793 * (f64) arg0->unk2C) / 180.0);
        arg0->y = (f32) ((f64) arg0->y - ((f64) arg0->angle + (0.30000001192092896 * sin((3.141592653589793 * (f64) arg0->unk2C) / 180.0))));
        arg0->x = (f32) (arg0->x + (0.5f * arg0->angle * temp_f31_2));
        arg0->rotY = (f32) (arg0->rotY - (2.0f * arg0->angle));
        if (arg0->y < 0.0f) {
            Hu3DModelAttrSet(arg0->model, 1U);
            var_r26 = 1;
        }
        Hu3DModelPosSetV(arg0->model, (HuVecF *) &arg0->x);
        Hu3DModelRotSetV(arg0->model, (HuVecF *) &arg0->rotX);
        break;
    }
    return var_r26;
}

void fn_1_A2A4(M538 *arg0) {
    M538_NODE *temp_r31;
    M538_NODE *temp_r30;
    M538_NODE *temp_r29;

    arg0->tick++;
    arg0->tick = (s16) (arg0->tick % 6);
    if (arg0->tick == 0) {
        temp_r31 = arg0->pool3.free;
        if (temp_r31 != 0) {
            arg0->pool3.free = temp_r31->next;
            temp_r31->func = fn_1_94DC;
            temp_r31->next = arg0->pool3.head;
            if (temp_r31->next != 0) {
                temp_r31->next->prev = temp_r31;
            }
            arg0->pool3.head = temp_r31;
        }
        temp_r29 = temp_r31;
        temp_r30 = temp_r29;
        if (temp_r30 != 0) {
            temp_r30->angle = (f32) arg0->lightRadius;
            temp_r30->timer = 0;
        }
    }
}

void fn_1_A368(M538 *arg0) {
    M538_NODE *temp_r31;
    M538_NODE *temp_r30;
    M538_NODE *temp_r29;

    arg0->tick++;
    arg0->tick = (s16) (arg0->tick % 3);
    if (arg0->tick == 0) {
        temp_r31 = arg0->pool3.free;
        if (temp_r31 != 0) {
            arg0->pool3.free = temp_r31->next;
            temp_r31->func = fn_1_9C04;
            temp_r31->next = arg0->pool3.head;
            if (temp_r31->next != 0) {
                temp_r31->next->prev = temp_r31;
            }
            arg0->pool3.head = temp_r31;
        }
        temp_r29 = temp_r31;
        temp_r30 = temp_r29;
        if (temp_r30 != 0) {
            temp_r30->timer = 0;
        }
    }
}

s32 fn_1_A424(M538_DOLLY *arg0) {
    f32 temp_f31;
    s32 var_r28;
    s32 var_r29;
    M538 *temp_r31;

    var_r28 = 0;
    temp_r31 = lbl_1_bss_C->data;
    switch (arg0->state) {
    case 0:
        var_r29 = 0;
        goto loop_9_test;
    loop_9:
        Hu3DModelAttrReset((s16) (temp_r31->models[17] + var_r29), 0x40000002U);
        Hu3DModelAttrSet((s16) (temp_r31->models[17] + var_r29), 0x40000001U);
        var_r29 += 1;
    loop_9_test:
        if (var_r29 < 0x18) {
            goto loop_9;
        }
        Hu3DModelAttrReset(temp_r31->models[11], 0x40000002U);
        Hu3DModelAttrSet(temp_r31->models[11], 0x40000001U);
        Hu3DModelAttrReset(temp_r31->models[12], 0x40000002U);
        Hu3DModelAttrSet(temp_r31->models[12], 0x40000001U);
        Hu3DModelAttrReset(temp_r31->models[24], 0x40000002U);
        Hu3DModelAttrSet(temp_r31->models[24], 0x40000001U);
        arg0->counter = 0;
        arg0->state += 1;
        /* fallthrough */
    case 1:
        arg0->counter += 1;
        temp_f31 = (1.5f * (f32) arg0->counter) / 120.0f;
        var_r29 = 0;
        goto loop_13_test;
    loop_13:
        Hu3DMotionSpeedSet((s16) (temp_r31->models[17] + var_r29), temp_f31);
        var_r29 += 1;
    loop_13_test:
        if (var_r29 < 0x18) {
            goto loop_13;
        }
        Hu3DMotionSpeedSet(temp_r31->models[11], temp_f31);
        Hu3DMotionSpeedSet(temp_r31->models[12], temp_f31);
        Hu3DMotionSpeedSet(temp_r31->models[24], temp_f31);
        if (arg0->counter == 0x78) {
            var_r28 = 1;
        }
        break;
    case 2:
        arg0->counter = 0;
        arg0->state += 1;
        /* fallthrough */
    case 3:
        arg0->counter += 1;
temp_f31 = 1.5f * (1.0f - ((f32) arg0->counter / 180.0f));
        var_r29 = 0;
        goto loop_19_test;
    loop_19:
        Hu3DMotionSpeedSet((s16) (temp_r31->models[17] + var_r29), temp_f31);
        var_r29 += 1;
    loop_19_test:
        if (var_r29 < 0x18) {
            goto loop_19;
        }
        Hu3DMotionSpeedSet(temp_r31->models[11], temp_f31);
        Hu3DMotionSpeedSet(temp_r31->models[12], temp_f31);
        Hu3DMotionSpeedSet(temp_r31->models[24], temp_f31);
        if (arg0->counter == 0xB4) {
            var_r28 = 1;
        }
        break;
    }
    return var_r28;
}

struct m538_idx4_s {
    s32 v0;
    s32 v1;
    s32 v2;
    s32 v3;
};

void fn_1_A6F8(M538 *arg0) {
    s32 sp8[4];
    s16 var_r29;
    s32 var_r30;

    *(struct m538_idx4_s *) &sp8[0] = *(struct m538_idx4_s *) &lbl_1_rodata_2A4[0];
    arg0->ctrl.state = 0;
    var_r30 = 0;
    goto loop_6;
loop_5:
    if (arg0->models[sp8[var_r30]] < 0) {
        var_r29 = arg0->models[sp8[var_r30]] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_264[sp8[var_r30]], 0x10000000, HEAP_MODEL));
    } else {
        var_r29 = Hu3DModelLink(arg0->models[sp8[var_r30]]);
    }
    Hu3DModelAttrSet(var_r29, 1U);
    Hu3DModelAttrSet(var_r29, 0x40000002U);
    Hu3DModelLayerSet(var_r29, 2);
    Hu3DModelPosSet(var_r29, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(var_r29, 0.0f, 0.0f, 0.0f);
    arg0->ctrl.work[var_r30].model = var_r29;
    arg0->ctrl.work[var_r30].state = 0;
    var_r30 += 1;
loop_6:
    if (var_r30 < 4) {
        goto loop_5;
    }
}

void fn_1_A8A0(M538_CTRL *arg0) {
    M538_NODE0 *node;
    M538_FW *work;
    s16 temp_r0;

    node = NULL;
    work = &arg0->work[0];
    temp_r0 = arg0->state;
    switch (temp_r0) {                              /* irregular */
    case 0:
        arg0->timer = 0;
        arg0->state += 1;
        /* fallthrough */
        case 1:
            arg0->timer += 1;
            if ((s16) arg0->timer == 0x168) {
                work = &arg0->work[0];
            node = ((M538 *) lbl_1_bss_C->data)->node0Free;
            if (node != NULL) {
                ((M538 *) lbl_1_bss_C->data)->node0Free = node->next;
                node->func = fn_1_AB54;
                node->work = work;
                node->next = ((M538 *) lbl_1_bss_C->data)->node0Head;
                if (node->next != NULL) {
                    node->next->prev = node;
                }
                ((M538 *) lbl_1_bss_C->data)->node0Head = node;
            }
            if (node != NULL) {
                work->radius = ((M538 *) lbl_1_bss_C->data)->lightRadius;
                work->state = 0;
            }
            arg0->state += 1;
            }
            break;
        case 2:
            arg0->timer += 1;
            if ((s16) arg0->timer == 0x2D0) {
                work = &arg0->work[1];
            node = ((M538 *) lbl_1_bss_C->data)->node0Free;
            if (node != NULL) {
                ((M538 *) lbl_1_bss_C->data)->node0Free = node->next;
                node->func = fn_1_AB54;
                node->work = work;
                node->next = ((M538 *) lbl_1_bss_C->data)->node0Head;
                if (node->next != NULL) {
                    node->next->prev = node;
                }
                ((M538 *) lbl_1_bss_C->data)->node0Head = node;
            }
            if (node != NULL) {
                work->radius = ((M538 *) lbl_1_bss_C->data)->lightRadius;
                work->state = 0;
            }
            arg0->state += 1;
            }
            break;
        case 3:
            arg0->timer += 1;
            if ((s16) arg0->timer == 0x438) {
                work = &arg0->work[2];
            node = ((M538 *) lbl_1_bss_C->data)->node0Free;
            if (node != NULL) {
                ((M538 *) lbl_1_bss_C->data)->node0Free = node->next;
                node->func = fn_1_AB54;
                node->work = work;
                node->next = ((M538 *) lbl_1_bss_C->data)->node0Head;
                if (node->next != NULL) {
                    node->next->prev = node;
                }
                ((M538 *) lbl_1_bss_C->data)->node0Head = node;
            }
            if (node != NULL) {
                work->radius = ((M538 *) lbl_1_bss_C->data)->lightRadius;
                work->state = 0;
            }
            arg0->state += 1;
            }
            break;
        case 4:
            arg0->timer += 1;
            if ((s16) arg0->timer == 0x5A0) {
                work = &arg0->work[3];
            node = ((M538 *) lbl_1_bss_C->data)->node0Free;
            if (node != NULL) {
                ((M538 *) lbl_1_bss_C->data)->node0Free = node->next;
                node->func = fn_1_AB54;
                node->work = work;
                node->next = ((M538 *) lbl_1_bss_C->data)->node0Head;
                if (node->next != NULL) {
                    node->next->prev = node;
                }
                ((M538 *) lbl_1_bss_C->data)->node0Head = node;
            }
            if (node != NULL) {
                work->radius = 250.0f + ((M538 *) lbl_1_bss_C->data)->lightRadius;
                work->state = 0;
            }
            arg0->state += 1;
            }
            break;
    }
}

s32 fn_1_AB54(M538_NODE0 *arg0) {
    M538_FW *temp_r31;
    f32 temp_f30;
    f32 temp_f31;
    s16 temp_r0;
    s32 var_r30;

    var_r30 = 0;
    temp_r31 = (M538_FW *) arg0->work;
    temp_r0 = temp_r31->state;
    switch (temp_r0) {                              /* irregular */
    case 0:
        Hu3DModelAttrReset(temp_r31->model, 1U);
        temp_r31->timer = 0;
        temp_r31->dist = 0.0f;
        temp_r31->radius2 = 0.0f;
        temp_r31->radius3 = 0.0f;
        temp_r31->angle = (f32) (360.0f * ((f32) frandmod(0x2711U) / 10000.0f));
        temp_r31->x = (f32) ((f64) temp_r31->dist * sin((3.141592653589793 * (f64) temp_r31->angle) / 180.0));
        temp_r31->y = 300.0f;
        temp_r31->z = (f32) ((f64) temp_r31->dist * cos((3.141592653589793 * (f64) temp_r31->angle) / 180.0));
        temp_r31->state += 1;
        /* fallthrough */
    case 1:
        temp_r31->timer += 1;
        if ((s16) temp_r31->timer == 0x3C) {
            temp_r31->state += 1;
        }
        temp_r31->angle = (f32) (temp_r31->angle - 4.9f);
        temp_r31->dist = (f32) (temp_r31->dist + 5.0f);
        temp_r31->radius2 = (f32) (temp_r31->radius2 + 5.0f);
        temp_f30 = 1.0f - ((f32) temp_r31->timer / 60.0f);
        temp_f31 = temp_r31->dist + (temp_r31->dist * (0.5f * temp_f30 * temp_f30));
        temp_r31->x = (f32) ((f64) temp_f31 * sin((3.141592653589793 * (f64) temp_r31->angle) / 180.0));
        temp_r31->z = (f32) ((f64) temp_f31 * cos((3.141592653589793 * (f64) temp_r31->angle) / 180.0));
        Hu3DModelPosSetV(temp_r31->model, (HuVecF *) temp_r31);
        Hu3DModelRotSetV(temp_r31->model, (HuVecF *) &temp_r31->rotX);
        break;
    case 2:
        temp_r31->timer = 0;
        temp_r31->state += 1;
        /* fallthrough */
    case 3:
        temp_r31->timer += 1;
        if ((s16) temp_r31->timer == 0x3C) {
            temp_r31->state += 1;
        }
        temp_r31->angle = (f32) (temp_r31->angle - 4.9f);
        temp_r31->dist = (f32) (temp_r31->dist + 5.0f);
        temp_r31->radius2 = (f32) (temp_r31->radius2 + 5.0f);
        temp_r31->x = (f32) ((f64) temp_r31->dist * sin((3.141592653589793 * (f64) temp_r31->angle) / 180.0));
        temp_r31->z = (f32) ((f64) temp_r31->dist * cos((3.141592653589793 * (f64) temp_r31->angle) / 180.0));
        Hu3DModelPosSetV(temp_r31->model, (HuVecF *) temp_r31);
        Hu3DModelRotSetV(temp_r31->model, (HuVecF *) &temp_r31->rotX);
        break;
    case 4:
        temp_r31->angle = (f32) (temp_r31->angle - 90.0f);
        temp_r31->dist = (f32) ((3.141592653589793 * (f64) (2.0f * temp_r31->dist)) / 72.0);
        temp_r31->timer = 0;
        temp_r31->state += 1;
        /* fallthrough */
    case 5:
        temp_r31->x = (f32) ((f64) temp_r31->x + ((f64) temp_r31->dist * sin((3.141592653589793 * (f64) temp_r31->angle) / 180.0)));
        temp_r31->z = (f32) ((f64) temp_r31->z + ((f64) temp_r31->dist * cos((3.141592653589793 * (f64) temp_r31->angle) / 180.0)));
        temp_r31->dist = (f32) (temp_r31->dist + 5.0f);
        temp_r31->radius2 = (f32) (temp_r31->radius2 + 5.0f);
        Hu3DModelPosSetV(temp_r31->model, (HuVecF *) temp_r31);
        Hu3DModelRotSetV(temp_r31->model, (HuVecF *) &temp_r31->rotX);
        temp_r31->timer += 1;
        if ((s16) temp_r31->timer == 0x3C) {
            Hu3DModelAttrSet(temp_r31->model, 1U);
            var_r30 = 1;
        }
        break;
    }
    return var_r30;
}

s32 fn_1_B0D0(M538_NODE0 *arg0) {
    f32 temp_f31;
    s32 var_r30;
    M538_FALL *temp_r31;

    var_r30 = 0;
    temp_r31 = arg0->work;
    switch (temp_r31->state) {                        /* irregular */
    case 0:
        temp_r31->timer = 0;
        temp_r31->state += 1;
        /* fallthrough */
    case 1:
        temp_r31->timer += 1;
        if (temp_r31->timer == 0x3C) {
            var_r30 = 1;
        }
        temp_f31 = 1.0f + ((3.0f * (f32) temp_r31->timer) / 60.0f);
        Hu3DModelScaleSet(temp_r31->model, temp_f31, 1.0f, temp_f31);
        break;
    }
    return var_r30;
}
