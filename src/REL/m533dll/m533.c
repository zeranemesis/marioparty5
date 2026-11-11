#include <humath.h>
#include <math.h>

#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/printfunc.h"
#include "game/process.h"
#include "game/sprite.h"
#include "game/wipe.h"
#include "game/mg/seqman.h"

extern void (*_ctors[])(void);
extern void (*_dtors[])(void);

typedef struct M533Player_s {
    u32 flags; // 0x00
    s32 state; // 0x04
    s32 unk8;
    s32 no; // 0x0C
    s32 partner; // 0x10
    s32 charNo; // 0x14
    s32 padNo; // 0x18
    s32 grp; // 0x1C
    s32 cam; // 0x20
    s32 unk24;
    s32 unk28;
    Vec pos; // 0x2C
    Vec rot; // 0x38
    float unk44;
    float unk48;
    float unk4C;
    u16 btn; // 0x50
    s32 btnCnt; // 0x54
    s32 lastFrame; // 0x58
    s32 frameDiff; // 0x5C
    float speed; // 0x60
    s32 timer; // 0x64
    s32 unk68;
    float ang; // 0x6C
    float angSpd; // 0x70
    s32 unk74;
    s32 dif; // 0x78
    s32 level; // 0x7C
    s32 unk80;
    s32 unk84;
    s32 unk88;
    float stamina; // 0x8C
    float unk90;
    float unk94;
    float unk98;
} M533PLAYER;

typedef struct M533Mark_s {
    s32 no;
    Vec pos;
} M533MARK;

typedef struct M533Team_s {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    Vec pos; // 0x0C
    float unk18;
    float unk1C;
    float unk20;
    float unk24;
    float unk28;
    float unk2C;
    float unk30;
    s32 unk34;
    float unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
} M533TEAM;

typedef struct M533Flyer_s {
    float unk0;
    float unk4;
    float unk8;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    s32 unk14;
} M533FLYER;

typedef struct M533Item_s {
    s32 unk0;
    s32 unk4;
    float unk8;
    s32 unkC;
    float unk10;
} M533ITEM;

typedef struct M533Spawn_s {
    float x;
    float y;
    float z;
    s32 unkC;
    float unk10;
} M533SPAWN;

typedef struct M533Level_s {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
} M533LEVEL;

typedef struct M533ListNode_s {
    u16 unk0;
    s16 unk2;
    struct M533ListNode_s *prev;
    struct M533ListNode_s *next;
} M533LISTNODE;

typedef struct M533CacheEntry_s {
    u32 unk0;
    s16 unk4;
    s16 unk6;
} M533CACHEENTRY;

/* pillar.c */
void fn_1_6528(void);
void fn_1_6590(OMOBJ *arg0);
void fn_1_6770(OMOBJ *arg0, s32 arg1);

float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

void ObjectSetup(void);
void fn_1_6A8(OMOBJ *obj);
void fn_1_A80(OMOBJ *obj);
void fn_1_165C(OMOBJ *obj);
void fn_1_1840(OMOBJ *obj);
void fn_1_1A5C(OMOBJ *arg0, s32 arg1);
void fn_1_1AE8(OMOBJ *arg0, s32 arg1, float arg3, u32 arg2);
void fn_1_1BA0(OMOBJ *arg0);
s32 fn_1_1CD0(OMOBJ *arg0);
void fn_1_1DAC(OMOBJ *arg0);
void fn_1_1E3C(OMOBJ *arg0);
void fn_1_1F98(OMOBJ *obj);
s32 fn_1_2698(s32 arg0);
void fn_1_2724(OMOBJ *obj);
void fn_1_2D4C(s16 mode, s16 frameNo);
void fn_1_3424(s16 mode, s16 frameNo);
void fn_1_3890(s16 mode, s16 frameNo);
void fn_1_3950(s16 mode, s16 frameNo);
void fn_1_3D50(s16 mode, s16 frameNo);
void fn_1_4080(s16 mode, s16 frameNo);
void fn_1_44D4();
void fn_1_44D8(void);
float fn_1_45B8(s32 arg0);
void fn_1_46A4(float arg0);
void fn_1_48B8(void);
void fn_1_48BC(s32 arg0);
void fn_1_4A44(OMOBJ *obj);
void fn_1_4CC8(OMOBJ *obj);
void fn_1_4F84(OMOBJ *obj);
void fn_1_5154(OMOBJ *obj);
void fn_1_52C4(void);

Vec lbl_1_data_0 = { 13000.0f, 10000.0f, 0.0f };
Vec lbl_1_data_C = { 0.0f, 0.0f, 0.0f };
/* data_18 (1 byte) and data_19 (3 bytes) are the four bytes of this colour */
GXColor lbl_1_data_18 = { 255, 255, 255, 255 };
s32 lbl_1_data_1C = -1;
u32 lbl_1_data_20[10] = {
    0x8C0000,
    0x8C0006,
    0x8C0031,
    0x8C004C,
    0x8C0001,
    0x8C0002,
    0x8C0051,
    0x8C004B,
    0x8C0032,
    0x8C0032,
};
s32 lbl_1_data_48[2] = { 0x5B0011, 0x5B0012 };
float lbl_1_data_50[13] = {
    1.0f, 1.0f, 2.0f, 1.0f, 1.5f, 2.0f, 1.3f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
};
s32 lbl_1_data_84[9] = {
    0x5B0000, 0x5B0001, 0x5B0002, 0x5B0003, 0x5B0004, 0x5B0005, 0x5B000A, 0x5B000B, 0x5B0009,
};
Vec lbl_1_data_A8[4] = {
    { 4800.0f, -850.0f, 550.0f },
    { 4800.0f, -850.0f, 550.0f },
    { 5100.0f, -850.0f, 550.0f },
    { 5100.0f, -850.0f, 550.0f },
};
Vec lbl_1_data_D8[4] = {
    { 11920.0f, -850.0f, 550.0f },
    { 11920.0f, -850.0f, 550.0f },
    { 12220.0f, -850.0f, 550.0f },
    { 12220.0f, -850.0f, 550.0f },
};
M533SPAWN lbl_1_data_108[24] = {
    { 7500.0f, 100.0f, -150.0f, 0, -10.0f },
    { 7500.0f, 100.0f, -150.0f, 0, -10.0f },
    { 7500.0f, -50.0f, -200.0f, 1, -15.0f },
    { 7500.0f, -50.0f, -200.0f, 1, -15.0f },
    { 7500.0f, -200.0f, -250.0f, 2, -12.0f },
    { 7500.0f, -200.0f, -250.0f, 2, -12.0f },
    { 7700.0f, 125.0f, -200.0f, 2, -12.0f },
    { 7700.0f, 125.0f, -200.0f, 2, -12.0f },
    { 7700.0f, -50.0f, -250.0f, 0, -8.5f },
    { 7700.0f, -50.0f, -250.0f, 0, -8.5f },
    { 7700.0f, -180.0f, -150.0f, 1, -10.0f },
    { 7700.0f, -180.0f, -150.0f, 1, -10.0f },
    { -1000.0f, 100.0f, -250.0f, 0, -10.0f },
    { -1000.0f, 100.0f, -250.0f, 0, -10.0f },
    { -1000.0f, 100.0f, -250.0f, 1, -12.5f },
    { -1000.0f, 100.0f, -250.0f, 1, -12.5f },
    { -1000.0f, 100.0f, -250.0f, 2, -15.0f },
    { -1000.0f, 100.0f, -250.0f, 2, -15.0f },
    { -1000.0f, 100.0f, -250.0f, 0, -10.0f },
    { -1000.0f, 100.0f, -250.0f, 0, -10.0f },
    { -1000.0f, 100.0f, -250.0f, 1, -12.5f },
    { -1000.0f, 100.0f, -250.0f, 1, -12.5f },
    { -1000.0f, 100.0f, -250.0f, 2, -15.0f },
    { -1000.0f, 100.0f, -250.0f, 2, -15.0f },
};
M533SPAWN lbl_1_data_2E8[24] = {
    { 13370.0f, 212.5f, -250.0f, -1, -10.0f },
    { 13370.0f, 212.5f, -250.0f, -1, -10.0f },
    { 13370.0f, 37.5f, -250.0f, -1, -8.0f },
    { 13370.0f, 37.5f, -250.0f, -1, -8.0f },
    { 13670.0f, 300.0f, -250.0f, -1, -8.0f },
    { 13670.0f, 300.0f, -250.0f, -1, -8.0f },
    { 13690.0f, 125.0f, -250.0f, -1, -11.0f },
    { 13690.0f, 125.0f, -250.0f, -1, -11.0f },
    { 13670.0f, -50.0f, -250.0f, -1, -9.5f },
    { 13670.0f, -50.0f, -250.0f, -1, -9.5f },
    { 13970.0f, 212.5f, -250.0f, -1, -8.5f },
    { 13970.0f, 212.5f, -250.0f, -1, -8.5f },
    { 14020.0f, 37.5f, -250.0f, -1, -12.0f },
    { 14020.0f, 37.5f, -250.0f, -1, -12.0f },
    { 14270.0f, 400.0f, -250.0f, -1, -10.0f },
    { 14270.0f, 400.0f, -250.0f, -1, -10.0f },
    { 14310.0f, 125.0f, -250.0f, -1, -11.0f },
    { 14310.0f, 125.0f, -250.0f, -1, -11.0f },
    { 14420.0f, -100.0f, -250.0f, -1, -7.5f },
    { 14420.0f, -100.0f, -250.0f, -1, -7.5f },
    { 14570.0f, 212.5f, -250.0f, -1, -10.5f },
    { 14570.0f, 212.5f, -250.0f, -1, -10.5f },
    { 14620.0f, 37.5f, -250.0f, -1, -12.0f },
    { 14620.0f, 37.5f, -250.0f, -1, -12.0f },
};
MGSEQ_PARAM lbl_1_data_4C8 = {
    300, 0, fn_1_2D4C, fn_1_3424, NULL, fn_1_3890, fn_1_3950, fn_1_3D50, fn_1_4080, NULL, NULL,
};
Vec lbl_1_data_4F0 = { 0.0f, 300.0f, 0.0f };
Vec lbl_1_data_4FC = { 1500.0f, -100.0f, 0.0f };
OMCAMERAVIEW lbl_1_data_508 = { { 0.5f, 500.0f, 0.0f }, { -2.0f, 0.0f, 0.0f }, 4100.0f };
OMCAMERAVIEW lbl_1_data_524 = { { 1500.0f, 0.0f, 0.0f }, { -6.0f, 0.0f, 0.0f }, 2700.0f };
M533LEVEL lbl_1_data_540[4] = {
    { 16, 21, 21, 26, 26, 30 },
    { 13, 18, 18, 23, 23, 28 },
    { 10, 15, 15, 20, 21, 25 },
    { 7, 12, 12, 17, 17, 22 },
};
float lbl_1_data_5A0[5] = { 3.0f, 2.5f, 2.0f, 1.5f, 1.5f };
float lbl_1_data_5B4[5] = { 10.0f, 15.0f, 20.0f, 25.0f, 25.0f };
char lbl_1_data_5C8[0x24] = "******* M533 ObjectSetup *********\n";
char lbl_1_data_5EC[0xE] = "itemhook_play";
char lbl_1_data_5FA[0xF] = "itemhook_taiki";
char lbl_1_data_609[0xF] = "front_itemhook";
char lbl_1_data_618[0xE] = "back_itemhook";
char lbl_1_data_626[0x12] = "itemhook_p_shadow";
char lbl_1_data_638[0x12] = "itemhook_t_shadow";
char lbl_1_data_64A[0x26] = "POSX:%.02f POSY:%.02f POSZ:%.02f";

/* .bss is emitted in reverse declaration order by MWCC, so the original's
   address order is declared bottom-up; pads precede the variable they follow. */
M533LISTNODE lbl_1_bss_2BC;
HUPROCESS *lbl_1_bss_2B8;
Vec lbl_1_bss_2AC;
Vec lbl_1_bss_2A0;
Vec lbl_1_bss_294;
Vec lbl_1_bss_288;
Vec lbl_1_bss_258[4];
Vec lbl_1_bss_228[4];
Vec lbl_1_bss_1F8[4];
Vec lbl_1_bss_1C8[4];
u32 lbl_1_bss_1BC_pad[2];
s32 lbl_1_bss_1BC;
s32 lbl_1_bss_1B8;
u32 lbl_1_bss_B4_pad;
M533CACHEENTRY lbl_1_bss_B4[32];
OMOBJ *lbl_1_bss_B0;
OMOBJ **lbl_1_bss_AC;
OMOBJ **lbl_1_bss_A8;
OMOBJ **lbl_1_bss_A4;
OMOBJ **lbl_1_bss_A0;
OMOBJ **lbl_1_bss_9C;
OMOBJ *lbl_1_bss_98;
s16 lbl_1_bss_90[4];
s16 lbl_1_bss_88[4];
s16 lbl_1_bss_84[2];
s16 lbl_1_bss_54[24];
s16 lbl_1_bss_50[2];
s16 lbl_1_bss_4C[2];
float lbl_1_bss_44[2];
s16 lbl_1_bss_42;
s16 lbl_1_bss_40;
s16 lbl_1_bss_3E;
s16 lbl_1_bss_3C;
s16 lbl_1_bss_34[4];
u32 lbl_1_bss_30;
u32 lbl_1_bss_2C;
s32 lbl_1_bss_28;
float lbl_1_bss_24;
s16 lbl_1_bss_10[10];
s32 lbl_1_bss_C;
s32 lbl_1_bss_8;
s32 lbl_1_bss_4;
s32 lbl_1_bss_0;

s32 _prolog(void)
{
    void (**ctor)(void) = (void (**)(void)) &_ctors;

    while (*ctor != 0) {
        (*ctor)();
        ctor++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    void (**dtor)(void) = (void (**)(void)) &_dtors;

    while (*dtor != 0) {
        (*dtor)();
        dtor++;
    }
}

void ObjectSetup(void)
{
    s32 sp8 = 0;
    s32 spC[2] = { -1, -1 };
    OMOBJ *var_r31;
    s32 temp_r29;
    s32 i;

    OSReport(lbl_1_data_5C8);
    lbl_1_bss_2B8 = omInitObjMan(200, 0x2000);
    omGameSysInit(lbl_1_bss_2B8);
    lbl_1_bss_1BC = -1;
    omCameraViewSetMulti(3, &lbl_1_data_508);
    Hu3DCameraCreate(3);
    fn_1_46A4(0.0f);
    lbl_1_bss_98 = omAddObjEx(lbl_1_bss_2B8, 0x7FDA, 0, 0, -1, omOutViewMulti);
    lbl_1_bss_98->work[0] = 2;
    lbl_1_bss_42 = Hu3DGLightCreate(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    Hu3DGLightPosAimSetV(lbl_1_bss_42, &lbl_1_data_0, &lbl_1_data_C);
    Hu3DGLightStaticSet(lbl_1_bss_42, 1);
    Hu3DGLightInfinitytSet(lbl_1_bss_42);
    Hu3DGLightColorSet(lbl_1_bss_42, lbl_1_data_18.r, lbl_1_data_18.g, lbl_1_data_18.b, lbl_1_data_18.a);
    omMakeGroupEx(lbl_1_bss_2B8, 0, 4);
    lbl_1_bss_AC = omGetGroupMemberListEx(lbl_1_bss_2B8, 0);
    for (i = 0; i < 4; i++) {
        var_r31 = omAddObjEx(lbl_1_bss_2B8, 100, 1, 10, 0, fn_1_6A8);
        lbl_1_bss_AC[i] = var_r31;
        var_r31->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M533PLAYER), 0x10000000U);
        var_r31->work[0] = i;
        temp_r29 = GwPlayerConf[i].grp;
        var_r31->work[1] = temp_r29;
        if (spC[temp_r29] == -1) {
            spC[temp_r29] = frandmod(2);
            var_r31->work[2] = spC[temp_r29];
        } else if (spC[temp_r29] == 0) {
            var_r31->work[2] = 1;
        } else {
            var_r31->work[2] = 0;
        }
    }
    omMakeGroupEx(lbl_1_bss_2B8, 1, 4);
    lbl_1_bss_A8 = omGetGroupMemberListEx(lbl_1_bss_2B8, 1);
    for (i = 0; i < 4; i++) {
        var_r31 = omAddObjEx(lbl_1_bss_2B8, 101, 1, 1, 1, fn_1_165C);
        lbl_1_bss_A8[i] = var_r31;
        var_r31->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M533MARK), 0x10000000U);
        var_r31->work[0] = i;
    }
    omMakeGroupEx(lbl_1_bss_2B8, 2, 2);
    lbl_1_bss_A4 = omGetGroupMemberListEx(lbl_1_bss_2B8, 2);
    for (i = 0; i < 2; i++) {
        var_r31 = omAddObjEx(lbl_1_bss_2B8, 100, 1, 3, 2, fn_1_1F98);
        lbl_1_bss_A4[i] = var_r31;
        var_r31->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M533TEAM), 0x10000000U);
        var_r31->work[0] = i;
    }
    lbl_1_bss_C = -1;
    lbl_1_bss_8 = -1;
    lbl_1_bss_4 = -1;
    lbl_1_bss_0 = -1;
    omMakeGroupEx(lbl_1_bss_2B8, 4, 4);
    lbl_1_bss_A0 = omGetGroupMemberListEx(lbl_1_bss_2B8, 4);
    for (i = 0; i < 4; i++) {
        var_r31 = omAddObjEx(lbl_1_bss_2B8, 104, 1, 2, 4, fn_1_4A44);
        lbl_1_bss_A0[i] = var_r31;
        var_r31->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M533FLYER), 0x10000000U);
        var_r31->work[0] = i;
    }
    omMakeGroupEx(lbl_1_bss_2B8, 5, 24);
    lbl_1_bss_9C = omGetGroupMemberListEx(lbl_1_bss_2B8, 5);
    for (i = 0; i < 24; i++) {
        var_r31 = omAddObjEx(lbl_1_bss_2B8, 105, 1, 1, 5, fn_1_4F84);
        lbl_1_bss_9C[i] = var_r31;
        var_r31->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M533ITEM), 0x10000000U);
        var_r31->work[0] = i;
    }
    fn_1_6528();
    fn_1_6590(var_r31);
    lbl_1_bss_B0 = var_r31;
    MgSeqCreate(&lbl_1_data_4C8);
}

void fn_1_6A8(OMOBJ *obj)
{
    M533PLAYER *p;
    s32 i;

    p = obj->data;
    p->no = obj->work[0];
    p->charNo = GwPlayerConf[p->no].charNo;
    p->grp = obj->work[1];
    p->flags = 0;
    p->unk28 = 0;
    if ((obj->work[2] & 1) == 0) {
        p->flags |= 1;
        p->rot.y = -90.0f;
    } else {
        p->flags |= 2;
        p->rot.y = 0.0f;
    }
    p->padNo = GwPlayerConf[p->no].padNo;
    p->state = 0;
    p->cam = p->grp;
    p->btn = 0;
    p->btnCnt = 0;
    p->unk74 = 0;
    p->timer = 0;
    p->ang = 180.0f;
    p->angSpd = 3.0f;
    p->stamina = 100.0f;
    p->unk44 = p->unk48 = p->unk4C = 0.0f;
    for (i = 0; i < 4; i++) {
        if (i != p->no && p->grp == GwPlayerConf[i].grp) {
            p->partner = i;
            break;
        }
    }
    p->dif = GwPlayerConf[p->no].dif;
    if (GwPlayerConf[p->no].type != 0) {
        p->level = p->dif;
    } else {
        p->level = 4;
    }
    p->unk98 = 50.0f;
    fn_1_1E3C(obj);
    p->unk88 = 0;
    *obj->mdlId = CharModelCreate(p->charNo, 4);
    lbl_1_bss_90[p->no] = *obj->mdlId;
    Hu3DModelCameraSet(*obj->mdlId, 1 << p->cam);
    for (i = 0; i < 10; i++) {
        obj->mtnId[i] = CharMotionCreate(p->charNo, lbl_1_data_20[i]);
        CharMotionSet(p->charNo, obj->mtnId[i]);
    }
    CharMotionVoiceOnSet(p->charNo, 0x4B, 0);
    Hu3DModelAttrSet(*obj->mdlId, 1);
    Hu3DModelAttrSet(*obj->mdlId, 0x40000001);
    Hu3DMotionSet(*obj->mdlId, *obj->mtnId);
    p->rot.x = p->rot.z = 0.0f;
    obj->trans.x = p->pos.x = 0.0f;
    obj->trans.y = p->pos.y = 0.0f;
    obj->trans.z = p->pos.z = 0.0f;
    p->pos.x = 0.0f;
    p->pos.y = 0.0f;
    p->pos.z = 0.0f;
    omSetTra(obj, p->pos.x, p->pos.y, p->pos.z);
    omSetRot(obj, p->rot.x, p->rot.y, p->rot.z);
    Hu3DModelAttrReset(*obj->mdlId, 1);
    CharMotionDataClose(p->charNo);
    obj->objFunc = fn_1_A80;
}

void fn_1_A80(OMOBJ *obj)
{
    Vec sp8;
    M533PLAYER *p;
    M533TEAM *team;
    M533PLAYER *partner;
    float temp_f1;

    p = obj->data;
    partner = lbl_1_bss_AC[p->partner]->data;
    team = lbl_1_bss_A4[p->grp]->data;
    if ((p->flags & 4) != 0) {
        fn_1_1A5C(obj, 5);
        if (--p->timer < 0) {
            fn_1_1A5C(obj, 0);
            p->flags &= ~4;
            p->flags |= 2;
            p->flags &= ~1;
            p->timer = 0;
            p->ang = 180.0f;
            p->rot.y = 0.0f;
            partner->flags &= ~8;
            partner->flags |= 1;
            partner->flags &= ~2;
            partner->timer = 0;
            partner->ang = 180.0f;
            partner->rot.y = -90.0f;
        } else {
            p->ang -= p->angSpd;
            p->pos.z = 100.0 * -sin(3.141592653589793 * p->ang / 180.0);
            p->rot.y -= p->angSpd;
            p->pos.x += 3.1666667f;
            Hu3DModelAttrReset(lbl_1_bss_4C[p->grp], 0x40000001);
            team->unk44 = 0;
            p->speed = 0.0f;
            Hu3DMotionTimeSet(lbl_1_bss_4C[p->grp], 0.0f);
            Hu3DMotionSpeedSet(lbl_1_bss_4C[p->grp], 0.0f);
        }
    } else if ((p->flags & 8) != 0) {
        lbl_1_bss_44[p->grp] = 99.0f;
        fn_1_1A5C(obj, 5);
        if (--p->timer < 0) {
            fn_1_1A5C(obj, 0);
            fn_1_1E3C(obj);
        } else {
            p->ang -= p->angSpd;
            p->pos.z = 100.0 * sin(3.141592653589793 * p->ang / 180.0);
            p->rot.y -= p->angSpd;
            p->pos.x -= 3.1666667f;
        }
    }
    if (MgSeqModeGet() == 3 && (p->flags & 1) != 0) {
        fn_1_1BA0(obj);
    }
    if (MgSeqModeGet() == 6 && MgSeqFrameNoGet() == 0) {
        fn_1_1A5C(obj, 0);
    }
    if (MgSeqModeGet() >= 6U) {
        sp8.x = team->pos.x;
        sp8.y = team->pos.y;
        sp8.z = team->pos.z;
        omSetTra(obj, p->pos.x + sp8.x, p->pos.y + sp8.y, p->pos.z + sp8.z);
        omSetRot(obj, p->rot.x, p->rot.y, p->rot.z);
        return;
    }
    if (MgSeqModeGet() == 5) {
        if (MgSeqModeGet() == 5) {
            if (MgSeqFrameNoGet() == 0) {
                if ((p->flags & 1) != 0) {
                    obj->trans.x = p->pos.x = lbl_1_data_4FC.x - 80.0f;
                } else {
                    obj->trans.x = p->pos.x = 110.0f + lbl_1_data_4FC.x;
                }
                obj->trans.y = p->pos.y = lbl_1_data_4FC.y;
                obj->trans.z = p->pos.z = lbl_1_data_4FC.z;
                omSetTra(obj, p->pos.x, p->pos.y, p->pos.z);
                return;
            }
            if (MgSeqFrameNoGet() == 1) {
                if ((p->flags & 1) != 0) {
                    obj->trans.x = p->pos.x = -80.0f;
                } else {
                    obj->trans.x = p->pos.x = 110.0f;
                }
                obj->trans.y = p->pos.y = 0.0f;
                obj->trans.z = p->pos.z = 0.0f;
                omSetTra(obj, p->pos.x, p->pos.y, p->pos.z);
            }
        }
        if ((p->flags & 1) != 0) {
            fn_1_1BA0(obj);
            if (Hu3DMotionShiftIDGet(*obj->mdlId) == 0) {
                Hu3DMotionSpeedSet(*obj->mdlId, 2.0f);
                Hu3DMotionSpeedSet(lbl_1_bss_4C[p->grp], 2.0f);
            }
            if ((p->flags & 0x20) == 0) {
                if (GwPlayerConf[p->no].type != 0) {
                    p->btn |= fn_1_1CD0(obj);
                } else {
                    p->btn |= HuPadBtnDown[p->padNo];
                }
            }
            {
                u16 mask = 0x60;
                if ((p->btn & 0x800) != 0 && (p->flags & 0x20) == 0) {
                    p->flags |= 0x20;
                }
                if ((p->flags & 0x20) != 0 && p->state == 3
                    && (Hu3DMotionEndCheck(*obj->mdlId) != 0 || Hu3DMotionTimeGet(*obj->mdlId) == 0.0f)) {
                    p->btn = 0;
                    p->flags &= ~1;
                    p->flags |= 4;
                    p->timer = 60;
                    p->rot.y = 180.0f;
                    p->flags &= ~0x20;
                    partner->flags &= ~2;
                    partner->flags |= 8;
                    partner->timer = 60;
                    partner->rot.y = 0.0f;
                }
                if (mask == (p->btn & mask)) {
                    p->btn &= ~mask;
                    p->btnCnt = 0;
                    p->frameDiff = MgSeqFrameNoGet() - p->lastFrame;
                    p->lastFrame = MgSeqFrameNoGet();
                    p->speed = 0.25f * (2.0f * fn_1_45B8(p->frameDiff));
                    Hu3DMotionSpeedSet(*obj->mdlId, p->speed);
                    Hu3DMotionSpeedSet(lbl_1_bss_4C[p->grp], p->speed);
                    team->unk24 += 1.5f;
                    p->unk74 += 1;
                    if (team->pos.x < 10670.0f) {
                        p->stamina -= lbl_1_data_5A0[p->level];
                        if (p->stamina < 0.0f) {
                            p->stamina = 0.0f;
                        }
                    }
                }
                if (p->btn != 0) {
                    if (++p->btnCnt >= 6) {
                        p->btn &= ~mask;
                        p->btnCnt = 0;
                    }
                }
                if (p->state == 3) {
                    if (MgSeqFrameNoGet() - p->lastFrame > 30U) {
                        Hu3DModelAttrReset(*obj->mdlId, 0x40000001);
                        Hu3DModelAttrReset(lbl_1_bss_4C[p->grp], 0x40000001);
                        team->unk44 = 0;
                    } else {
                        Hu3DModelAttrSet(*obj->mdlId, 0x40000001);
                        Hu3DModelAttrSet(lbl_1_bss_4C[p->grp], 0x40000001);
                        team->unk44 = 1;
                    }
                }
            }
        }
        if ((p->flags & 2) != 0) {
            p->btn = HuPadBtnDown[p->padNo];
            if (GwPlayerConf[p->no].type == 0 && (p->btn & 0x100) != 0 && Hu3DMotionEndCheck(lbl_1_bss_50[p->grp]) != 0) {
                HuAudFXPlay(0x7BA);
                Hu3DMotionTimeSet(lbl_1_bss_50[p->grp], 0.0f);
                Hu3DMotionSpeedSet(lbl_1_bss_50[p->grp], 1.0f);
                Hu3DMotionClusterSpeedSet(lbl_1_bss_50[p->grp], 0, 1.0f);
                Hu3DMotionClusterTimeSet(lbl_1_bss_50[p->grp], 0, 0.0f);
            }
            if (p->stamina < 100.0f) {
                fn_1_1A5C(obj, 6);
                p->stamina += lbl_1_data_5B4[p->level] / 60.0f;
                if (p->stamina > 100.0f) {
                    p->stamina = 100.0f;
                }
            } else {
                fn_1_1A5C(obj, 0);
            }
        }
        sp8.x = team->pos.x;
        sp8.y = team->pos.y;
        sp8.z = team->pos.z;
        omSetTra(obj, p->pos.x + sp8.x, p->pos.y + sp8.y, p->pos.z + sp8.z);
        omSetRot(obj, p->rot.x, p->rot.y, p->rot.z);
        if (team->unk24 > 0.0f && (p->flags & 1) != 0) {
            temp_f1 = Hu3DMotionTimeGet(lbl_1_bss_4C[p->grp]);
            if (temp_f1 < lbl_1_bss_44[p->grp]) {
                if (team->unk8 == 0) {
                    HuAudFXPlay(0x7B8);
                } else if (team->unk8 == 1) {
                    HuAudFXPlay(0x7B8);
                }
            }
            lbl_1_bss_44[p->grp] = temp_f1;
        }
    }
}

void fn_1_165C(OMOBJ *obj)
{
    M533MARK *mark;
    M533PLAYER *p;

    mark = obj->data;
    mark->no = obj->work[0];
    p = lbl_1_bss_AC[mark->no]->data;
    *obj->mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(0x5B0009, 0x10000000, HEAP_MODEL));
    lbl_1_bss_88[mark->no] = *obj->mdlId;
    Hu3DModelAttrSet(*obj->mdlId, 1);
    Hu3DModelCameraSet(*obj->mdlId, 1 << p->cam);
    obj->trans.x = mark->pos.x = 0.0f;
    obj->trans.y = mark->pos.y = 0.0f;
    obj->trans.z = mark->pos.z = 0.0f;
    mark->pos.x = 0.0f;
    mark->pos.y = 0.0f;
    mark->pos.z = 0.0f;
    omSetTra(obj, mark->pos.x, mark->pos.y, mark->pos.z);
    omSetRot(obj, 0.0f, 0.0f, 0.0f);
    omSetSca(obj, lbl_1_data_50[p->charNo], lbl_1_data_50[p->charNo], lbl_1_data_50[p->charNo]);
    Hu3DModelAttrReset(*obj->mdlId, 1);
    obj->objFunc = fn_1_1840;
}

void fn_1_1840(OMOBJ *obj)
{
    Vec sp8;
    M533MARK *mark;
    M533PLAYER *p;
    M533TEAM *team;

    mark = obj->data;
    p = lbl_1_bss_AC[mark->no]->data;
    team = lbl_1_bss_A4[p->grp]->data;
    if (MgSeqModeGet() >= 5U) {
        if (MgSeqModeGet() == 5) {
            if (MgSeqFrameNoGet() == 0) {
                if ((p->flags & 1) != 0) {
                    obj->trans.x = mark->pos.x = lbl_1_data_4FC.x - 80.0f;
                } else {
                    obj->trans.x = mark->pos.x = 110.0f + lbl_1_data_4FC.x;
                }
                obj->trans.y = mark->pos.y = lbl_1_data_4FC.y;
                obj->trans.z = mark->pos.z = lbl_1_data_4FC.z;
                omSetTra(obj, mark->pos.x, mark->pos.y, mark->pos.z);
                return;
            }
            if (MgSeqFrameNoGet() == 1) {
                if ((p->flags & 1) != 0) {
                    obj->trans.x = mark->pos.x = -80.0f;
                } else {
                    obj->trans.x = mark->pos.x = 110.0f;
                }
                obj->trans.y = mark->pos.y = 0.0f;
                obj->trans.z = mark->pos.z = 0.0f;
                omSetTra(obj, mark->pos.x, mark->pos.y, mark->pos.z);
            }
        }
        sp8.x = team->pos.x;
        sp8.y = team->pos.y;
        sp8.z = team->pos.z;
        mark->pos.x = p->pos.x;
        mark->pos.y = p->pos.y;
        mark->pos.z = p->pos.z;
        omSetTra(obj, mark->pos.x + sp8.x, mark->pos.y + sp8.y, mark->pos.z + sp8.z);
    }
}

void fn_1_1A5C(OMOBJ *arg0, s32 arg1)
{
    M533PLAYER *p;

    p = arg0->data;
    if (p->state != arg1) {
        p->state = arg1;
        CharMotionShiftSet(p->charNo, arg0->mtnId[p->state], 0.0f, 8.0f, 0x40000001);
    }
}

void fn_1_1AE8(OMOBJ *arg0, s32 arg1, float arg3, u32 arg2)
{
    M533PLAYER *p;

    p = arg0->data;
    if (p->state != arg1) {
        p->state = arg1;
        CharMotionShiftSet(p->charNo, arg0->mtnId[p->state], 0.0f, arg3, arg2);
        if (lbl_1_bss_30 == -1 && arg1 == 2) {
            CharFXPlay(p->charNo, 0x200);
        }
    }
}

void fn_1_1BA0(OMOBJ *arg0)
{
    M533PLAYER *p;

    p = arg0->data;
    if (p->state != 3) {
        p->state = 3;
        Hu3DMotionShiftSet(*arg0->mdlId, arg0->mtnId[p->state], 0.0f, 8.0f, 0);
        p->unk28 = 8;
        return;
    }
    if (p->unk28-- == 0) {
        Hu3DMotionTimeSet(*arg0->mdlId, 0.0f);
        Hu3DMotionSpeedSet(*arg0->mdlId, 0.0f);
        Hu3DMotionTimeSet(lbl_1_bss_4C[p->grp], 0.0f);
        Hu3DMotionSpeedSet(lbl_1_bss_4C[p->grp], 0.0f);
        p->speed = 0.0f;
    }
}

s32 fn_1_1CD0(OMOBJ *arg0)
{
    M533PLAYER *p;
    s32 var_r30;
    M533TEAM *team;

    p = arg0->data;
    team = lbl_1_bss_A4[p->grp]->data;
    var_r30 = 0;
    p->unk80 = p->unk84;
    if (p->stamina > p->unk98 || team->pos.x >= 10670.0f) {
        if (p->unk88 >= p->unk80) {
            var_r30 = 0x60;
            p->unk88 = 0;
            fn_1_1E3C(arg0);
        } else {
            p->unk88++;
            var_r30 = 0;
        }
    } else {
        var_r30 = 0x800;
    }
    return var_r30;
}

void fn_1_1DAC(OMOBJ *arg0)
{
    float temp_f30;
    float temp_f31;
    M533PLAYER *p;

    p = arg0->data;
    temp_f30 = p->unk90;
    temp_f31 = p->unk94;
    p->unk98 = temp_f31 + frandmod(temp_f30 - temp_f31);
}

void fn_1_1E3C(OMOBJ *arg0)
{
    s32 var_r29;
    s32 var_r30;
    M533PLAYER *p;

    p = arg0->data;
    if (p->stamina <= 100.0f && p->stamina >= 80.0f) {
        var_r29 = lbl_1_data_540[p->level].unk4;
        var_r30 = lbl_1_data_540[p->level].unk0;
    } else if (p->stamina <= 79.0f && p->stamina >= 60.0f) {
        var_r29 = lbl_1_data_540[p->level].unkC;
        var_r30 = lbl_1_data_540[p->level].unk8;
    } else {
        var_r29 = lbl_1_data_540[p->level].unk14;
        var_r30 = lbl_1_data_540[p->level].unk10;
    }
    p->unk84 = var_r30 + frandmod(var_r29 - var_r30);
}

void fn_1_1F98(OMOBJ *obj)
{
    M533PLAYER *players[4];
    M533TEAM *team;
    s32 temp_r3;
    s32 k;
    s32 i;
    s16 var_r26;
    s16 var_r25;

    team = obj->data;
    for (i = 0; i < 4; i++) {
        players[i] = lbl_1_bss_AC[i]->data;
    }
    team->unk8 = obj->work[0];
    team->unk38 = 0.0f;
    team->unk3C = 0;
    team->unk40 = 0;
    team->unk24 = team->unk28 = team->unk2C = 0.0f;
    team->unk30 = 0.0f;
    team->unk0 = 0;
    k = 0;
    if (team->unk8 == 0) {
        temp_r3 = fn_1_2698(0);
        if ((players[temp_r3]->flags & 1) != 0) {
            var_r26 = temp_r3;
            var_r25 = players[temp_r3]->partner;
        } else {
            var_r25 = temp_r3;
            var_r26 = players[temp_r3]->partner;
        }
        obj->mdlId[k] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_84[0], 0x10000000, HEAP_MODEL));
    } else {
        temp_r3 = fn_1_2698(1);
        if ((players[temp_r3]->flags & 1) != 0) {
            var_r26 = temp_r3;
            var_r25 = players[temp_r3]->partner;
        } else {
            var_r25 = temp_r3;
            var_r26 = players[temp_r3]->partner;
        }
        obj->mdlId[k] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_84[1], 0x10000000, HEAP_MODEL));
    }
    lbl_1_bss_84[team->unk8] = obj->mdlId[k];
    Hu3DModelCameraSet(obj->mdlId[k], 1 << team->unk8);
    Hu3DMotionSpeedSet(obj->mdlId[k], 0.0f);
    Hu3DModelAttrSet(obj->mdlId[k], 1);
    team->unk18 = team->unk1C = team->unk20 = 0.0f;
    team->pos.z = team->pos.y = team->pos.x = 0.0f;
    obj->trans.x = team->pos.x = lbl_1_data_4F0.x;
    obj->trans.y = team->pos.y = lbl_1_data_4F0.y;
    obj->trans.z = team->pos.z = lbl_1_data_4F0.z;
    omSetTra(obj, team->pos.x, team->pos.y, team->pos.z);
    omSetRot(obj, team->unk18, team->unk1C, team->unk20);
    lbl_1_bss_50[team->unk8] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_84[3], 0x10000000, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_50[team->unk8], 1);
    Hu3DModelCameraSet(lbl_1_bss_50[team->unk8], 1 << team->unk8);
    Hu3DModelPosSet(lbl_1_bss_50[team->unk8], 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(lbl_1_bss_50[team->unk8], 0.0f, 0.0f, 0.0f);
    Hu3DMotionSpeedSet(lbl_1_bss_50[team->unk8], 0.0f);
    Hu3DMotionTimeSet(lbl_1_bss_50[team->unk8], Hu3DMotionMaxTimeGet(lbl_1_bss_50[team->unk8]));
    lbl_1_bss_4C[team->unk8] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_84[2], 0x10000000, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_4C[team->unk8], 1);
    Hu3DModelCameraSet(lbl_1_bss_4C[team->unk8], 1 << team->unk8);
    Hu3DModelPosSet(lbl_1_bss_4C[team->unk8], 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(lbl_1_bss_4C[team->unk8], 0.0f, 0.0f, 0.0f);
    Hu3DMotionSpeedSet(lbl_1_bss_4C[team->unk8], 0.0f);
    lbl_1_bss_44[team->unk8] = 99.0f;
    Hu3DModelHookSet(lbl_1_bss_84[team->unk8], lbl_1_data_5EC, lbl_1_bss_90[var_r26]);
    Hu3DModelHookSet(lbl_1_bss_84[team->unk8], lbl_1_data_5FA, lbl_1_bss_90[var_r25]);
    Hu3DModelHookSet(lbl_1_bss_84[team->unk8], lbl_1_data_609, lbl_1_bss_50[team->unk8]);
    Hu3DModelHookSet(lbl_1_bss_84[team->unk8], lbl_1_data_618, lbl_1_bss_4C[team->unk8]);
    Hu3DModelAttrReset(obj->mdlId[k], 1);
    Hu3DModelAttrReset(lbl_1_bss_50[team->unk8], 1);
    Hu3DModelAttrReset(lbl_1_bss_4C[team->unk8], 1);
    obj->objFunc = fn_1_2724;
}

s32 fn_1_2698(s32 arg0)
{
    M533PLAYER *players[4];
    s32 var_r30;
    s32 i;

#ifdef TARGET_PC
    var_r30 = 0;
#endif
    for (i = 0; i < 4; i++) {
        players[i] = lbl_1_bss_AC[i]->data;
    }
    for (i = 0; i < 4; i++) {
        if (arg0 == players[i]->grp) {
            var_r30 = i;
            break;
        }
    }
    return var_r30;
}

void fn_1_2724(OMOBJ *obj)
{
    M533FLYER *flyers[4];
    M533ITEM *items[24];
    M533TEAM *team;
    s32 i;

    team = obj->data;
    for (i = 0; i < 4; i++) {
        flyers[i] = lbl_1_bss_A0[i]->data;
    }
    for (i = 0; i < 24; i++) {
        items[i] = lbl_1_bss_9C[i]->data;
    }
    if (5.0f < team->unk24) {
        team->unk24 = 5.0f;
    }
    team->pos.x += team->unk24;
    team->unk30 += team->unk24;
    if (MgSeqModeGet() >= 5U) {
        team->unk38 += 2.0f;
        if (team->unk38 > 360.0f) {
            team->unk38 = 0.0f;
        }
        team->pos.y = lbl_1_data_4FC.y + (20.0 * HuSin(team->unk38));
    }
    if (team->unk3C == 0 && team->unk30 > 3333.0f) {
        if (team->unk8 == 0) {
            if (flyers[0]->unk10 == 0) {
                flyers[0]->unk10 = 1;
                flyers[2]->unk10 = 1;
            }
        } else if (flyers[1]->unk10 == 0) {
            flyers[1]->unk10 = 1;
            flyers[3]->unk10 = 1;
        }
        team->unk3C = 1;
    }
    if (team->unk40 == 0 && team->pos.x > 6000.0f) {
        if (team->unk8 == 0) {
            if (items[0]->unk0 == 0) {
                for (i = 0; i < 24; i += 2) {
                    items[i]->unk0 = 1;
                    if (i > 12) {
                        Hu3DModelAttrSet(lbl_1_bss_54[i], 1);
                    }
                }
            }
        } else if (items[1]->unk0 == 0) {
            for (i = 1; i < 24; i += 2) {
                items[i]->unk0 = 1;
                if (i > 12) {
                    Hu3DModelAttrSet(lbl_1_bss_54[i], 1);
                }
            }
        }
        team->unk40 = 1;
    }
    if (MgSeqModeGet() == 5) {
        team->unk24 -= 0.05f;
        if (team->unk24 < 0.0f) {
            team->unk24 = 0.0f;
        }
    }
    if (MgSeqModeGet() == 5 && MgSeqFrameNoGet() == 0) {
        obj->trans.x = team->pos.x = lbl_1_data_4FC.x;
        obj->trans.y = team->pos.y = lbl_1_data_4FC.y;
        obj->trans.z = team->pos.z = lbl_1_data_4FC.z;
        Hu3DMotionTimeSet(*obj->mdlId, 0.0f);
        Hu3DMotionSpeedSet(*obj->mdlId, 0.0f);
        Hu3DModelAttrReset(*obj->mdlId, 0x40000001);
        Hu3DModelHookObjReset(*obj->mdlId, lbl_1_data_5EC);
        Hu3DModelHookObjReset(*obj->mdlId, lbl_1_data_5FA);
        Hu3DModelHookObjReset(*obj->mdlId, lbl_1_data_626);
        Hu3DModelHookObjReset(*obj->mdlId, lbl_1_data_638);
    }
    omSetTra(obj, team->pos.x, team->pos.y, team->pos.z);
    omSetRot(obj, team->unk18, team->unk1C, team->unk20);
    if (MgSeqModeGet() >= 5U) {
        CenterM[team->unk8].x = team->pos.x;
        CenterM[team->unk8].y = 0.0f;
        CenterM[team->unk8].z = team->pos.z;
    }
    if (team->pos.x >= 11170.0f) {
        team->unk30 = 9670.0f;
        team->unk0 = 1;
        if (MgSeqModeGet() == 5) {
            MgSeqModeNext();
            lbl_1_bss_2C = MgSeqFrameNoGet();
        }
    }
    if (team->unk8 == 0) {
        if (team->unk24 > 0.0f) {
            if (lbl_1_bss_C == -1) {
                lbl_1_bss_C = HuAudFXPlay(0x7B7);
            }
        } else if (lbl_1_bss_C != -1) {
            HuAudFXStop(lbl_1_bss_C);
            lbl_1_bss_C = -1;
        }
    } else if (team->unk24 > 0.0f) {
        if (lbl_1_bss_8 == -1) {
            lbl_1_bss_8 = HuAudFXPlay(0x7B7);
        }
    } else if (lbl_1_bss_8 != -1) {
        HuAudFXStop(lbl_1_bss_8);
        lbl_1_bss_8 = -1;
    }
}

void fn_1_2D4C(s16 mode, s16 frameNo)
{
    M533TEAM *teams[2];
    M533PLAYER *players[4];
    M533PLAYER *tmp[4];
    s32 var_r25;
    s16 var_r27;
    s16 var_r28;
    s32 k;
    s32 i;

#ifdef TARGET_PC
    var_r25 = 0;
#endif
    for (i = 0; i < 2; i++) {
        teams[i] = lbl_1_bss_A4[i]->data;
    }
    for (i = 0; i < 4; i++) {
        players[i] = lbl_1_bss_AC[i]->data;
    }
    lbl_1_bss_28 = 0;
    fn_1_44D4();
    lbl_1_bss_24 = 0.0f;
    lbl_1_bss_10[5] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_84[5], 0x10000000, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_10[5], 1);
    Hu3DModelAttrSet(lbl_1_bss_10[5], 0x40000001);
    Hu3DModelCameraSet(lbl_1_bss_10[5], 3);
    Hu3DModelPosSet(lbl_1_bss_10[5], 0.0f, 300.0f, 0.0f);
    Hu3DModelRotSet(lbl_1_bss_10[5], 0.0f, 0.0f, 0.0f);
    lbl_1_bss_10[4] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_84[4], 0x10000000, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_10[4], 1);
    Hu3DModelCameraSet(lbl_1_bss_10[4], 3);
    Hu3DModelPosSet(lbl_1_bss_10[4], 0.0f, 300.0f, 0.0f);
    Hu3DModelRotSet(lbl_1_bss_10[4], 0.0f, 0.0f, 0.0f);
    lbl_1_bss_10[6] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_84[6], 0x10000000, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_10[6], 1);
    Hu3DModelCameraSet(lbl_1_bss_10[6], 3);
    Hu3DModelPosSet(lbl_1_bss_10[6], 0.0f, 300.0f, 0.0f);
    Hu3DModelRotSet(lbl_1_bss_10[6], 0.0f, 0.0f, 0.0f);
    lbl_1_bss_10[7] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_84[7], 0x10000000, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_10[7], 1);
    Hu3DModelCameraSet(lbl_1_bss_10[7], 3);
    Hu3DModelPosSet(lbl_1_bss_10[7], 0.0f, 300.0f, 0.0f);
    Hu3DModelRotSet(lbl_1_bss_10[7], 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(lbl_1_bss_10[5], 1);
    Hu3DModelAttrReset(lbl_1_bss_10[4], 1);
    Hu3DModelAttrReset(lbl_1_bss_10[6], 1);
    Hu3DModelAttrReset(lbl_1_bss_10[7], 1);
    lbl_1_bss_40 = espEntry(0x5B000D, 10, 0);
    espDispOff(lbl_1_bss_40);
    espDrawNoSet(lbl_1_bss_40, 0);
    espPosSet(lbl_1_bss_40, 16.0f, 224.0f);
    lbl_1_bss_3E = espEntry(0x5B000E, 10, 0);
    espDispOff(lbl_1_bss_3E);
    espDrawNoSet(lbl_1_bss_3E, 0);
    espPosSet(lbl_1_bss_3E, 16.0f, 256.0f);
    lbl_1_bss_3C = espEntry(0x5B000C, 11, 0);
    espDispOff(lbl_1_bss_3C);
    espDrawNoSet(lbl_1_bss_3C, 0);
    espPosSet(lbl_1_bss_3C, 320.0f, 240.0f);
    fn_1_48BC(0);
    for (k = 0; k < 2; k++) {
        for (i = 0; i < 4; i++) {
            tmp[i] = lbl_1_bss_AC[i]->data;
        }
        for (i = 0; i < 4; i++) {
            if (k == tmp[i]->grp) {
                var_r25 = i;
                break;
            }
        }
        if ((players[var_r25]->flags & 1) != 0) {
            var_r28 = var_r25;
            var_r27 = players[var_r25]->partner;
        } else {
            var_r27 = var_r25;
            var_r28 = players[var_r25]->partner;
        }
        Hu3DModelHookSet(lbl_1_bss_84[teams[k]->unk8], lbl_1_data_626, lbl_1_bss_88[var_r28]);
        Hu3DModelHookSet(lbl_1_bss_84[teams[k]->unk8], lbl_1_data_638, lbl_1_bss_88[var_r27]);
    }
    if (WipeCheckEnd() == 0) {
        MgSeqModeNext();
    }
}

void fn_1_3424(s16 mode, s16 frameNo)
{
    s16 sp8;
    M533TEAM *teams[2];
    M533PLAYER *players[4];
    float temp_f31;
    float var_f30;
    float var_f29;
    s16 j;
    s32 i;

    for (i = 0; i < 2; i++) {
        teams[i] = lbl_1_bss_A4[i]->data;
    }
    for (i = 0; i < 4; i++) {
        players[i] = lbl_1_bss_AC[i]->data;
    }
    fn_1_46A4(0.0f);
    lbl_1_data_1C = HuAudFXPlay(0x7B9);
    Hu3DModelAttrReset(lbl_1_bss_10[4], 0x40000001);
    Hu3DModelAttrReset(lbl_1_bss_10[6], 0x40000001);
    Hu3DModelAttrReset(lbl_1_bss_10[7], 0x40000001);
    for (i = 0; i < 125; i++) {
        HuPrcVSleep();
    }
    for (i = 0; i < 4; i++) {
        omVibrate(i, 15, 12, 0);
    }
    HuAudFXPlay(0x7B6);
    for (i = 0; i < 2; i++) {
        Hu3DMotionTimeSet(lbl_1_bss_84[i], 0.0f);
        Hu3DMotionSpeedSet(lbl_1_bss_84[i], 1.0f);
    }
    for (i = 0; i < 10; i++) {
        HuPrcVSleep();
    }
    sp8 = omCameraViewMoveSimpleMulti(3, &lbl_1_data_524, 240);
    for (i = 0; i < 4; i++) {
        fn_1_1A5C(lbl_1_bss_AC[i], 7);
        if (players[i]->grp == 0) {
            CharFXPlay(players[i]->charNo, 0x1FF);
        }
    }
    for (i = 0; i < 120; i++) {
        HuPrcVSleep();
    }
    for (i = 0; i < 4; i++) {
        fn_1_1AE8(lbl_1_bss_AC[i], 0, 10.0f, 0x40000001);
    }
    for (i = 0; i < 17; i++) {
        HuPrcVSleep();
    }
    for (i = 0; i < 55.0f; i++) {
        HuPrcVSleep();
    }
    var_f29 = 0.016666668f;
    var_f30 = 0.0f;
    do {
        temp_f31 = HuSin(90.0f * var_f30);
        temp_f31 *= temp_f31;
        fn_1_46A4(0.5f * temp_f31);
        HuPrcVSleep();
        var_f30 += var_f29;
    } while (var_f30 < 1.0f);
    fn_1_48BC(1);
    lbl_1_bss_1BC = HuAudSStreamPlay(0x33);
    MgSeqModeNext();
}

void fn_1_3890(s16 mode, s16 frameNo)
{
    M533PLAYER *players[4];
    M533PLAYER *players2[2];
    u32 timer;
    s32 i;

    fn_1_44D4();
    for (i = 0; i < 4; i++) {
        players[i] = lbl_1_bss_AC[i]->data;
    }
    for (i = 0; i < 2; i++) {
        players2[i] = lbl_1_bss_AC[i]->data;
    }
    fn_1_48B8();
    timer = MgSeqTimerValueGet();
    if (timer == 0) {
        MgSeqModeNext();
    }
    fn_1_48BC(1);
}

void fn_1_3950(s16 mode, s16 frameNo)
{
    M533PLAYER *players[4];
    M533TEAM *teams[2];
    s32 winners[2];
    s16 temp_r27;
    s16 temp_r26;
    s32 n;
    s32 i;

    fn_1_44D4();
    if (MgSeqFrameNoGet() == 0) {
        HuAudSStreamFadeOut(lbl_1_bss_1BC, 100);
        for (i = 0; i < 4; i++) {
            lbl_1_bss_34[i] = -1;
            players[i] = lbl_1_bss_AC[i]->data;
            if ((players[i]->flags & 1) != 0) {
                fn_1_1AE8(lbl_1_bss_AC[i], 0, 8.0f, 0x40000001);
            }
        }
        for (i = 0; i < 2; i++) {
            teams[i] = lbl_1_bss_A4[i]->data;
            Hu3DModelAttrReset(lbl_1_bss_4C[i], 0x40000001);
            teams[i]->unk44 = 0;
        }
        if (teams[0]->unk0 == 0 && teams[1]->unk0 == 0) {
            lbl_1_bss_30 = -1;
        } else if (teams[0]->unk0 != 0 && teams[1]->unk0 != 0) {
            lbl_1_bss_30 = frandmod(2);
        } else if (teams[0]->unk0 != 0) {
            lbl_1_bss_30 = 0;
        } else {
            lbl_1_bss_30 = 1;
        }
        if (lbl_1_bss_30 == 0) {
            fn_1_6770(lbl_1_bss_B0, 1);
        } else if (lbl_1_bss_30 == 1) {
            fn_1_6770(lbl_1_bss_B0, 2);
        }
        i = 0;
        n = 0;
        for (; i < 4; i++) {
            if (lbl_1_bss_30 == players[i]->grp) {
                lbl_1_bss_34[n] = players[i]->charNo;
                winners[n] = players[i]->no;
                n++;
                temp_r26 = GwPlayer[i].mgCoinBonus;
                temp_r27 = temp_r26 + 10;
                if (_CheckFlag(0x1000F) == 0) {
                    GwPlayer[i].mgCoinBonus = temp_r27;
                }
            }
        }
        if (lbl_1_bss_30 == -1) {
            MgSeqWinnerSet(-1, -1, -1, -1);
            return;
        }
        if ((players[winners[0]]->flags & 1) != 0) {
            MgSeqWinnerSet(lbl_1_bss_34[0], lbl_1_bss_34[1], -1, -1);
            return;
        }
        MgSeqWinnerSet(lbl_1_bss_34[1], lbl_1_bss_34[0], -1, -1);
    }
}

void fn_1_3D50(s16 mode, s16 frameNo)
{
    OMCAMERAVIEW sp24;
    M533PLAYER *players[4];
    M533TEAM *teams[2];
    float temp_f31;
    float var_f30;
    float var_f29;
    s32 i;

    fn_1_44D4();
    for (i = 0; i < 4; i++) {
        players[i] = lbl_1_bss_AC[i]->data;
    }
    for (i = 0; i < 2; i++) {
        teams[i] = lbl_1_bss_A4[i]->data;
    }
    if (lbl_1_bss_30 == -1) {
        if (frameNo == 60) {
            MgSeqModeNext();
        }
        return;
    }
    if (MgSeqFrameNoGet() >= 60U) {
        sp24.center.x = 50.0f + teams[lbl_1_bss_30]->pos.x;
        sp24.center.y = 100.0f + teams[lbl_1_bss_30]->pos.y;
        sp24.center.z = teams[lbl_1_bss_30]->pos.z;
        sp24.rot.x = 0.0f;
        sp24.rot.y = 0.0f;
        sp24.rot.z = 0.0f;
        sp24.zoom = 1200.0f;
        if (lbl_1_bss_2C < 0x960U) {
            fn_1_52C4();
        }
        fn_1_48BC(0);
        var_f29 = 0.016666668f;
        var_f30 = 0.0f;
        if (lbl_1_bss_30 == 0) {
            do {
                temp_f31 = HuSin(90.0f * var_f30);
                temp_f31 *= temp_f31;
                fn_1_46A4(0.5f - (0.5f * temp_f31));
                HuPrcVSleep();
                var_f30 += var_f29;
            } while (var_f30 < 1.0f);
            fn_1_46A4(0.0f);
        } else {
            do {
                temp_f31 = HuSin(90.0f * var_f30);
                temp_f31 *= temp_f31;
                fn_1_46A4(0.5f + (0.5f * temp_f31));
                HuPrcVSleep();
                var_f30 += var_f29;
            } while (var_f30 < 1.0f);
            fn_1_46A4(1.0f);
        }
        MgSeqModeNext();
    }
}

void fn_1_4080(s16 mode, s16 frameNo)
{
    M533PLAYER *players[4];
    s32 i;
    s32 k;

    fn_1_44D4();
    for (i = 0; i < 4; i++) {
        players[i] = lbl_1_bss_AC[i]->data;
    }
    if (lbl_1_bss_30 == -1) {
        for (i = 0; i < 4; i++) {
            fn_1_1AE8(lbl_1_bss_AC[i], 2, 8.0f, 0);
        }
    } else {
        for (i = 0; i < 4; i++) {
            if (lbl_1_bss_30 == players[i]->grp) {
                if (players[i]->charNo == 6) {
                    fn_1_1AE8(lbl_1_bss_AC[i], 8, 8.0f, 0);
                } else if (players[i]->charNo == 8) {
                    fn_1_1AE8(lbl_1_bss_AC[i], 9, 8.0f, 0);
                } else {
                    fn_1_1AE8(lbl_1_bss_AC[i], 1, 8.0f, 0);
                }
            } else {
                fn_1_1AE8(lbl_1_bss_AC[i], 2, 8.0f, 0);
            }
        }
    }
    for (k = 0; k < 4; k++) {
        if ((players[k]->flags & 1) != 0) {
            if (players[k]->rot.y < 0.0f) {
                players[k]->rot.y += 11.25f;
                players[k]->pos.x += 1.25f;
            } else {
                players[k]->rot.y = 0.0f;
            }
        }
    }
}

void fn_1_44D4(void)
{
}

void fn_1_44D8(void)
{
    GXColor sp8;
    M533PLAYER *players[4];
    s32 i;

    for (i = 0; i < 4; i++) {
        players[i] = lbl_1_bss_AC[i]->data;
    }
    sp8.a = 0xC0;
    sp8.r = 0;
    sp8.g = 0;
    sp8.b = 0x90;
    printWin(16, 40, 480, 96, &sp8);
    fontcolor = 14;
    print8(16, 40, 1.5f, lbl_1_data_64A, players[0]->pos.x, players[0]->pos.y, players[0]->pos.z);
}

float fn_1_45B8(s32 arg0)
{
    float var_f31;

    if (arg0 >= 1 && arg0 <= 5) {
        var_f31 = 4.0f;
    } else if (arg0 >= 6 && arg0 <= 10) {
        var_f31 = 3.5f;
    } else if (arg0 >= 11 && arg0 <= 15) {
        var_f31 = 3.0f;
    } else if (arg0 >= 16 && arg0 <= 20) {
        var_f31 = 2.5f;
    } else if (arg0 >= 21 && arg0 <= 25) {
        var_f31 = 2.0f;
    } else if (arg0 >= 26 && arg0 <= 30) {
        var_f31 = 1.5f;
    } else {
        var_f31 = 1.0f;
    }
    return var_f31;
}

void fn_1_46A4(float arg0)
{
    Hu3DCameraPerspectiveSet(1, 30.0f, 10.0f, 10000.0f, 1.3333334f);
    Hu3DCameraViewportSet(1, 0.0f, -(480.0f * arg0 * 0.5f), 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(1, 0, 0, 640, 480.0f * (1.0f - arg0));
    Hu3DCameraPerspectiveSet(2, 30.0f, 10.0f, 10000.0f, 1.3333334f);
    Hu3DCameraViewportSet(2, 0.0f, (480.0f - (480.0f * arg0)) * 0.5f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(2, 0, 480.0f - (480.0f * arg0), 640, 480.0f * arg0);
}

void fn_1_48B8(void)
{
}

void fn_1_48BC(s32 arg0)
{
    M533TEAM *teams[2];
    float var_f31;
    s32 i;

    for (i = 0; i < 2; i++) {
        teams[i] = lbl_1_bss_A4[i]->data;
    }
    var_f31 = (teams[0]->unk30 / 10000.0f) * 512.0f;
    espPosSet(lbl_1_bss_40, 32.0f + var_f31, 224.0f);
    var_f31 = (teams[1]->unk30 / 10000.0f) * 512.0f;
    espPosSet(lbl_1_bss_3E, 32.0f + var_f31, 256.0f);
    if (arg0 != 0) {
        espDispOn(lbl_1_bss_40);
        espDispOn(lbl_1_bss_3E);
        espDispOn(lbl_1_bss_3C);
        return;
    }
    espDispOff(lbl_1_bss_40);
    espDispOff(lbl_1_bss_3E);
    espDispOff(lbl_1_bss_3C);
}

void fn_1_4A44(OMOBJ *obj)
{
    M533FLYER *flyer;
    s32 i;

    flyer = obj->data;
    *obj->mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(0x5B0010, 0x10000000, HEAP_MODEL));
    for (i = 0; i < 2; i++) {
        obj->mtnId[i] = Hu3DJointMotion(*obj->mdlId, HuDataSelHeapReadNum(lbl_1_data_48[i], 0x10000000, HEAP_MODEL));
    }
    flyer->unk14 = obj->work[0];
    flyer->unk0 = -250.0f + (0.003921569f * (100.0f * (frand() & 0xFF)));
    flyer->unk4 = 180.0 * (atan2(500.0f + obj->trans.x, 100.0f + obj->trans.y) / 3.141592653589793);
    flyer->unk10 = 0;
    flyer->unkE = flyer->unkC = (frand() % 30) + 30;
    flyer->unk8 = 0.0f;
    Hu3DModelCameraSet(*obj->mdlId, 1 << (flyer->unk14 % 2));
    obj->trans.x = lbl_1_data_A8[flyer->unk14].x;
    obj->trans.y = 300.0f + lbl_1_data_A8[flyer->unk14].y;
    obj->trans.z = lbl_1_data_A8[flyer->unk14].z;
    omSetTra(obj, obj->trans.x, obj->trans.y, obj->trans.z);
    omSetRot(obj, 0.0f, 0.0f, 0.0f);
    obj->objFunc = fn_1_4CC8;
}

void fn_1_4CC8(OMOBJ *obj)
{
    M533FLYER *flyer;

    flyer = obj->data;
    if (flyer->unk10 != 0) {
        if (flyer->unk8 == 0.0f) {
            if (flyer->unkC == 0) {
                flyer->unk8 = 8.0f + (0.003921569f * (10.0f * (frand() & 0xFF)));
            } else {
                if (flyer->unkC == 6) {
                    Hu3DMotionShiftSet(*obj->mdlId, obj->mtnId[1], 0.0f, 8.0f, 0);
                    Hu3DModelAttrReset(*obj->mdlId, 0x40000001);
                }
                flyer->unkC--;
                flyer->unk0 -= 1.5f;
            }
        } else {
            flyer->unk0 = flyer->unk0 + (flyer->unk8 * sin(3.141592653589793 * flyer->unk4 / 180.0));
            obj->trans.x += 5.0f;
            flyer->unk8 *= 0.94f;
            if (flyer->unk8 < 0.1f) {
                flyer->unk8 = 0.0f;
                flyer->unkE = flyer->unkC = (frand() % 30) + 30;
                Hu3DMotionSet(*obj->mdlId, obj->mtnId[0]);
                Hu3DModelAttrSet(*obj->mdlId, 0x40000001);
            }
        }
        omSetTra(obj, obj->trans.x, flyer->unk0, obj->trans.z);
        if (obj->trans.y > 1000.0f) {
            Hu3DModelAttrSet(*obj->mdlId, 1);
            omSetTra(obj, lbl_1_data_D8[flyer->unk14].x, lbl_1_data_D8[flyer->unk14].y, lbl_1_data_D8[flyer->unk14].z);
            flyer->unk10 = 0;
        }
    }
}

void fn_1_4F84(OMOBJ *obj)
{
    M533ITEM *item;
    M533SPAWN *spawn;

    item = obj->data;
    item->unk4 = obj->work[0];
    item->unk0 = 0;
    item->unkC = item->unk4 % 2;
    item->unk10 = 0.0f;
    spawn = &lbl_1_data_108[item->unk4];
    item->unk8 = spawn->unk10;
    switch (spawn->unkC) {
        case 0:
            *obj->mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(0x5B0006, 0x10000000, HEAP_MODEL));
            break;
        case 1:
            *obj->mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(0x5B0007, 0x10000000, HEAP_MODEL));
            break;
        case 2:
            *obj->mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(0x5B0008, 0x10000000, HEAP_MODEL));
            break;
    }
    lbl_1_bss_54[item->unk4] = *obj->mdlId;
    Hu3DModelCameraSet(*obj->mdlId, 1 << (item->unk4 % 2));
    obj->trans.x = spawn->x;
    obj->trans.y = spawn->y;
    obj->trans.z = spawn->z;
    omSetTra(obj, obj->trans.x, obj->trans.y, obj->trans.z);
    omSetRot(obj, 0.0f, 0.0f, 0.0f);
    obj->objFunc = fn_1_5154;
}

void fn_1_5154(OMOBJ *obj)
{
    float var_f31;
    M533SPAWN *spawn;
    M533ITEM *item;

    item = obj->data;
    spawn = &lbl_1_data_2E8[item->unk4];
    if (item->unk0 != 0) {
        if (MgSeqModeGet() == 5) {
            if (item->unk4 < 12) {
                Hu3DModelAttrReset(*obj->mdlId, 1);
            } else {
                Hu3DModelAttrSet(*obj->mdlId, 1);
            }
        } else {
            Hu3DModelAttrReset(*obj->mdlId, 1);
        }
        obj->trans.x += item->unk8;
        if (item->unk8 < 0.0f) {
            var_f31 = -item->unk8;
        } else {
            var_f31 = item->unk8;
        }
        item->unk10 += var_f31;
        omSetTra(obj, obj->trans.x, obj->trans.y, obj->trans.z);
        if (item->unk10 > 3000.0f) {
            Hu3DModelAttrSet(*obj->mdlId, 1);
            omSetTra(obj, spawn->x, spawn->y, spawn->z);
            item->unk0 = 0;
            item->unk8 = spawn->unk10;
            item->unk10 = 0.0f;
        }
    }
}

void fn_1_52C4(void)
{
    M533FLYER *flyers[4];
    M533ITEM *items[24];
    s32 i;

    for (i = 0; i < 4; i++) {
        flyers[i] = lbl_1_bss_A0[i]->data;
    }
    for (i = 0; i < 24; i++) {
        items[i] = lbl_1_bss_9C[i]->data;
    }
    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_30 == i % 2) {
            flyers[i]->unk10 = 1;
            flyers[i]->unk8 = 0.0f;
            flyers[i]->unk4 = 180.0 * (atan2(500.0f + lbl_1_bss_A0[i]->trans.x, 100.0f + lbl_1_bss_A0[i]->trans.y) / 3.141592653589793);
            flyers[i]->unk0 = -250.0f + (0.003921569f * (100.0f * (frand() & 0xFF)));
            Hu3DModelAttrReset(*lbl_1_bss_A0[i]->mdlId, 1);
        }
    }
    for (i = 0; i < 24; i++) {
        if (lbl_1_bss_30 == i % 2) {
            items[i]->unk0 = 1;
            Hu3DModelAttrReset(*lbl_1_bss_9C[i]->mdlId, 1);
        }
    }
}

void fn_1_5544(void)
{
    Hu3DModelKill(lbl_1_bss_10[6]);
    Hu3DModelKill(lbl_1_bss_10[7]);
}

void fn_1_5580(void)
{
    lbl_1_bss_2BC.prev = NULL;
    lbl_1_bss_2BC.next = NULL;
}

void fn_1_55A4(u16 arg0, s16 arg1)
{
    M533LISTNODE *head;
    M533LISTNODE *temp_r3;
    M533LISTNODE *temp_r29;

    temp_r3 = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M533LISTNODE), 0x10000000U);
    temp_r3->unk0 = arg0;
    temp_r3->unk2 = arg1;
    head = &lbl_1_bss_2BC;
    temp_r29 = head->next;
    head->next = temp_r3;
    if (NULL != temp_r29) {
        temp_r29->prev = temp_r3;
    }
    temp_r3->prev = head;
    temp_r3->next = temp_r29;
}

s16 fn_1_562C(s16 arg0)
{
    M533LISTNODE *var_r31;
    M533LISTNODE *temp_r29;
    M533LISTNODE *temp_r30;
    s16 temp_r28;

    var_r31 = lbl_1_bss_2BC.next;
    while (NULL != var_r31) {
        if (arg0 == var_r31->unk0) {
            temp_r28 = var_r31->unk2;
            temp_r29 = var_r31->prev;
            temp_r30 = var_r31->next;
            temp_r29->next = temp_r30;
            if (NULL != temp_r30) {
                temp_r30->prev = temp_r29;
            }
            HuMemDirectFree(var_r31);
            return temp_r28;
        }
        var_r31 = var_r31->next;
    }
    return -1;
}

void fn_1_56D0(void)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        lbl_1_bss_B4[i].unk0 = 0;
        lbl_1_bss_B4[i].unk4 = -1;
    }
}

s16 fn_1_5728(u32 arg0)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        if (arg0 == lbl_1_bss_B4[i].unk0) {
            return Hu3DModelLink(lbl_1_bss_B4[i].unk4);
        }
    }
    for (i = 0; i < 32; i++) {
        if (lbl_1_bss_B4[i].unk0 == 0U) {
            lbl_1_bss_B4[i].unk0 = arg0;
            lbl_1_bss_B4[i].unk4 = Hu3DModelCreate(HuDataSelHeapReadNum(arg0, 0x10000000, HEAP_MODEL));
            return lbl_1_bss_B4[i].unk4;
        }
    }
    return Hu3DModelCreate(HuDataSelHeapReadNum(arg0, 0x10000000, HEAP_MODEL));
}

void fn_1_583C(float var_f30, Vec *arg0, Vec *arg1, Vec *arg2)
{
    float var_f31;

    if (1.0f < var_f30) {
        var_f30 = 1.0f;
    }
    var_f31 = HuSin(90.0f * var_f30) * HuSin(90.0f * var_f30);
    arg2->x = arg0->x + (var_f31 * (arg1->x - arg0->x));
    arg2->y = arg0->y + (var_f31 * (arg1->y - arg0->y));
    arg2->z = arg0->z + (var_f31 * (arg1->z - arg0->z));
}

void fn_1_5998(Vec *arg0, Vec *arg1, Vec *arg2)
{
    Hu3DCameraPosSetV(1, arg0, arg1, arg2);
    lbl_1_bss_2AC = *arg0;
    lbl_1_bss_294 = *arg2;
}

s32 fn_1_5A24(float var_f27, Vec *arg0)
{
    Vec sp18;
    Vec spC;
    float var_f31;
    float var_f30;
    float var_f29;
    float var_f28;

    var_f31 = var_f27;
    if (1.0f < var_f31) {
        var_f31 = 1.0f;
    }
    var_f29 = HuSin(90.0f * var_f31) * HuSin(90.0f * var_f31);
    sp18.x = lbl_1_bss_2AC.x + (var_f29 * (lbl_1_bss_2A0.x - lbl_1_bss_2AC.x));
    sp18.y = lbl_1_bss_2AC.y + (var_f29 * (lbl_1_bss_2A0.y - lbl_1_bss_2AC.y));
    sp18.z = lbl_1_bss_2AC.z + (var_f29 * (lbl_1_bss_2A0.z - lbl_1_bss_2AC.z));
    var_f30 = var_f27;
    if (1.0f < var_f30) {
        var_f30 = 1.0f;
    }
    var_f28 = HuSin(90.0f * var_f30) * HuSin(90.0f * var_f30);
    spC.x = lbl_1_bss_294.x + (var_f28 * (lbl_1_bss_288.x - lbl_1_bss_294.x));
    spC.y = lbl_1_bss_294.y + (var_f28 * (lbl_1_bss_288.y - lbl_1_bss_294.y));
    spC.z = lbl_1_bss_294.z + (var_f28 * (lbl_1_bss_288.z - lbl_1_bss_294.z));
    Hu3DCameraPosSetV(1, &sp18, arg0, &spC);
    if (1.0f <= var_f27) {
        return 1;
    }
    lbl_1_bss_1B8 = 1;
    return 0;
}

s32 fn_1_5D58(float var_f27, Vec *arg0)
{
    Vec sp18;
    Vec spC;
    s32 i;
    float var_f31;
    float var_f30;
    float var_f29;
    float var_f28;

    for (i = 0; i < 4; i++) {
        var_f31 = var_f27;
        if (1.0f < var_f31) {
            var_f31 = 1.0f;
        }
        var_f29 = HuSin(90.0f * var_f31) * HuSin(90.0f * var_f31);
        sp18.x = lbl_1_bss_258[i].x + (var_f29 * (lbl_1_bss_228[i].x - lbl_1_bss_258[i].x));
        sp18.y = lbl_1_bss_258[i].y + (var_f29 * (lbl_1_bss_228[i].y - lbl_1_bss_258[i].y));
        sp18.z = lbl_1_bss_258[i].z + (var_f29 * (lbl_1_bss_228[i].z - lbl_1_bss_258[i].z));
        var_f30 = var_f27;
        if (1.0f < var_f30) {
            var_f30 = 1.0f;
        }
        var_f28 = HuSin(90.0f * var_f30) * HuSin(90.0f * var_f30);
        spC.x = lbl_1_bss_1F8[i].x + (var_f28 * (lbl_1_bss_1C8[i].x - lbl_1_bss_1F8[i].x));
        spC.y = lbl_1_bss_1F8[i].y + (var_f28 * (lbl_1_bss_1C8[i].y - lbl_1_bss_1F8[i].y));
        spC.z = lbl_1_bss_1F8[i].z + (var_f28 * (lbl_1_bss_1C8[i].z - lbl_1_bss_1F8[i].z));
        Hu3DCameraPosSetV(1 << i, &sp18, arg0, &spC);
    }
    if (1.0f <= var_f27) {
        return 1;
    }
    lbl_1_bss_1B8 = 1;
    return 0;
}

float fn_1_613C(float var_f29, float arg1, float arg2)
{
    float var_f31;
    float var_f30;

    var_f31 = fmod(arg1 - var_f29, 360.0);
    if (0.0f > var_f31) {
        var_f31 += 360.0f;
    }
    if (180.0f < var_f31) {
        var_f31 -= 360.0f;
    }
    var_f30 = fmod(var_f29 + (var_f31 * arg2), 360.0);
    if (0.0f > var_f30) {
        var_f30 += 360.0f;
    }
    return var_f30;
}

void fn_1_6240(s32 arg0)
{
    if (lbl_1_bss_1BC >= 0) {
        if (arg0 < 0) {
            arg0 = 1000;
        }
        HuAudSeqFadeOut(lbl_1_bss_1BC, arg0);
    }
    lbl_1_bss_1BC = -1;
}

void fn_1_62AC(s16 arg0, char *arg1, Vec *arg2)
{
    Mtx sp10;

    Hu3DModelObjMtxGet(arg0, arg1, sp10);
    arg2->x = sp10[0][3];
    arg2->y = sp10[1][3];
    arg2->z = sp10[2][3];
}

u32 fn_1_6304(u32 arg0, u32 arg1)
{
    return arg0 + frandmod(arg1 - arg0);
}

s32 fn_1_6340(Vec *arg0, Vec *arg1, float arg2, float arg3)
{
    if ((((arg1->x - arg0->x) * (arg1->x - arg0->x)) + ((arg1->z - arg0->z) * (arg1->z - arg0->z)))
        < ((arg2 + arg3) * (arg2 + arg3))) {
        return 1;
    }
    return 0;
}

s32 fn_1_63A0(Vec *arg0, s32 arg1, s32 arg2)
{
    Vec sp10;
    s32 var_r31;

    Hu3D3Dto2D(arg0, 1, &sp10);
    var_r31 = arg1 + ((sp10.x / 576.0f) * (arg2 - arg1));
    return var_r31;
}

s32 fn_1_6464(Vec *arg0, s32 arg1, s32 arg2)
{
    Vec sp10;
    s32 var_r31;

    Hu3D3Dto2D(arg0, 1, &sp10);
    var_r31 = arg1 + ((sp10.y / 480.0f) * (arg2 - arg1));
    return var_r31;
}
