#include "REL/m531Dll.h"
#include "game/flag.h"
#include "game/gamework.h"

extern "C" {
void *__dl__FPv(void *ptr);
HU3DMODELID Hu3DModelCreate(void *data);
}

struct CPlayerBase;

struct PlayerBaseVt {
    void *m_head[2];
    void (*m_update0)(CPlayerBase *self);
    void (*m_update1)(CPlayerBase *self);
    void (*m_update2)(CPlayerBase *self);
    void (*m_update3)(CPlayerBase *self);
    void (*m_input)(CPlayerBase *self);
    void (*m_cpu)(CPlayerBase *self);
    void (*m_post)(CPlayerBase *self);
    CPlayerBase *(*m_dtor)(CPlayerBase *self, s16 flag);
    void (*m_set0)(CPlayerBase *self);
    void (*m_set1)(CPlayerBase *self);
};

struct PlayerMotInfo {
    s32 dataNum;
    u32 attr;
    f32 shiftTime;
};

// MWCC puts the vtable pointer after the data of the class (offset 0xB0 on the GC).
struct CPlayerBase {
    OMOBJMAN *m_objMan;
    s16 m_playerNo;
    s16 m_charNo;
    s16 m_model;
    s16 m_motId[32];
    s16 m_type;
    s16 m_dif;
    s8 m_padNo;
    s32 m_curMot;
    u8 m_flag54;
    u8 m_flag55;
    OMOBJ *m_obj;
    PlayerMotInfo *m_motTbl;
    s32 m_motNum;
    f32 m_stickX;
    f32 m_stickY;
    u8 m_unk6C[0x84 - 0x6C];
    Vec m_pos;
    Vec m_rot;
    Vec m_scale;
    s32 m_nextMot;
    s32 m_dataNum;
    const PlayerBaseVt *m_vt;
};

extern "C" {
extern PlayerBaseVt lbl_1_data_394;
extern PlayerBaseVt lbl_1_data_440;

void fn_1_C0E8(OMOBJ *obj);
void fn_1_C130(OMOBJ *obj);
void fn_1_C178(OMOBJ *obj);
void fn_1_C1C0(OMOBJ *obj);
CPlayerBase *fn_1_C208(CPlayerBase *self, OMOBJMAN *objMan, PlayerMotInfo *tbl, s32 num, s16 playerNo, u16 camBit);
CPlayerBase *fn_1_C390(CPlayerBase *self, OMOBJMAN *objMan, PlayerMotInfo *tbl, s32 num, s32 dataNum, u16 camBit);
CPlayerBase *fn_1_C590(CPlayerBase *self, s16 flag);
void fn_1_C654(CPlayerBase *self);
void fn_1_C74C(CPlayerBase *self);
void fn_1_C774(CPlayerBase *self);
void fn_1_C808(CPlayerBase *self);
void fn_1_C830(CPlayerBase *self);
void fn_1_C83C(CPlayerBase *self);
void fn_1_C8D8(CPlayerBase *self, s32 mot);
void fn_1_C8E0(CPlayerBase *self);
void fn_1_C948(CPlayerBase *self, u32 mot);
void fn_1_CA30(CPlayerBase *self);
void fn_1_CA44(CPlayerBase *self, s32 mode, s32 mot);
void fn_1_CB20(CPlayerBase *self);
CPlayerBase *fn_1_CB34(CPlayerBase *self, OMOBJMAN *objMan, PlayerMotInfo *tbl, s32 num, s32 dataNum);
void fn_1_BA5C(CPlayerBase *self);
void fn_1_BA60(CPlayerBase *self);
}

void fn_1_C0E8(OMOBJ *obj)
{
    CPlayerBase *self = (CPlayerBase *)obj->data;
    self->m_vt->m_update0(self);
}

void fn_1_C130(OMOBJ *obj)
{
    CPlayerBase *self = (CPlayerBase *)obj->data;
    self->m_vt->m_update1(self);
}

void fn_1_C178(OMOBJ *obj)
{
    CPlayerBase *self = (CPlayerBase *)obj->data;
    self->m_vt->m_update2(self);
}

void fn_1_C1C0(OMOBJ *obj)
{
    CPlayerBase *self = (CPlayerBase *)obj->data;
    self->m_vt->m_update3(self);
}

CPlayerBase *fn_1_C208(CPlayerBase *self, OMOBJMAN *objMan, PlayerMotInfo *tbl, s32 num, s16 playerNo, u16 camBit)
{
    s32 i;
    self->m_vt = &lbl_1_data_440;
    self->m_objMan = objMan;
    self->m_playerNo = playerNo;
    self->m_charNo = GwPlayerConf[playerNo].charNo;
    self->m_flag54 = 0;
    self->m_flag55 = 0;
    self->m_motTbl = tbl;
    self->m_motNum = num;
    self->m_model = CharModelCreate(self->m_charNo, 2);
    for (i = 0; i < self->m_motNum; i++) {
        self->m_motId[i] = CharMotionCreate(self->m_charNo, self->m_motTbl[i].dataNum);
    }
    CharMotionDataClose(self->m_charNo);
    CharMotionSet(self->m_charNo, self->m_motId[0]);
    Hu3DModelAttrSet(self->m_model, 0x40000001);
    self->m_obj = omAddObjEx(objMan, 4, 0x10, 0x10, -1, fn_1_C0E8);
    self->m_obj->data = self;
    Hu3DModelShadowSet(self->m_model);
    Hu3DModelShadowDispOn(self->m_model);
    Hu3DModelLayerSet(self->m_model, 3);
    Hu3DModelCameraSet(self->m_model, camBit);
    fn_1_C654(self);
    return self;
}

CPlayerBase *fn_1_C390(CPlayerBase *self, OMOBJMAN *objMan, PlayerMotInfo *tbl, s32 num, s32 dataNum, u16 camBit)
{
    s32 i;
    self->m_vt = &lbl_1_data_440;
    self->m_objMan = objMan;
    self->m_playerNo = -1;
    self->m_charNo = -1;
    self->m_dif = -1;
    self->m_flag54 = 0;
    self->m_flag55 = 0;
    self->m_motTbl = tbl;
    self->m_motNum = num;
    self->m_dataNum = dataNum;
    self->m_model = Hu3DModelCreate(HuDataSelHeapReadNum(dataNum, 0x10000000, 2));
    for (i = 0; i < self->m_motNum; i++) {
        self->m_motId[i] = Hu3DJointMotion(self->m_model, HuDataSelHeapReadNum(self->m_motTbl[i].dataNum, 0x10000000, 2));
    }
    Hu3DMotionSet(self->m_model, self->m_motId[0]);
    Hu3DModelAttrSet(self->m_model, 0x40000001);
    Hu3DModelCameraSet(self->m_model, camBit);
    self->m_obj = omAddObjEx(objMan, 4, 0x10, 0x10, -1, fn_1_C130);
    self->m_obj->data = self;
    Hu3DModelShadowSet(self->m_model);
    Hu3DModelShadowDispOn(self->m_model);
    Hu3DModelLayerSet(self->m_model, 3);
    Hu3DModelCameraSet(self->m_model, camBit);
    self->m_pos.x = 0.0f;
    self->m_pos.y = 0.0f;
    self->m_pos.z = 0.0f;
    self->m_rot.x = 0.0f;
    self->m_rot.y = 0.0f;
    self->m_rot.z = 0.0f;
    self->m_scale.x = 1.0f;
    self->m_scale.y = 1.0f;
    self->m_scale.z = 1.0f;
    return self;
}

CPlayerBase *fn_1_C590(CPlayerBase *self, s16 flag)
{
    s32 i;
    if (self != NULL) {
        self->m_vt = &lbl_1_data_440;
        if (self->m_charNo != -1) {
            CharModelKill(self->m_charNo);
        } else {
            for (i = 0; i < self->m_motNum; i++) {
                Hu3DMotionKill(self->m_motId[i]);
            }
            Hu3DModelKill(self->m_model);
        }
        self->m_obj->data = NULL;
        omDelObjEx(self->m_objMan, self->m_obj);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void fn_1_C654(CPlayerBase *self)
{
    self->m_pos.x = 0.0f;
    self->m_pos.y = 0.0f;
    self->m_pos.z = 0.0f;
    self->m_rot.x = 0.0f;
    self->m_rot.y = 0.0f;
    self->m_rot.z = 0.0f;
    self->m_scale.x = 1.0f;
    self->m_scale.y = 1.0f;
    self->m_scale.z = 1.0f;
    self->m_type = GwPlayerConf[self->m_playerNo].type;
    self->m_dif = GwPlayerConf[self->m_playerNo].dif;
    self->m_padNo = GwPlayerConf[self->m_playerNo].padNo;
    self->m_curMot = 0;
    self->m_nextMot = 0;
}

void fn_1_C74C(CPlayerBase *self)
{
    fn_1_C8E0(self);
}

void fn_1_C774(CPlayerBase *self)
{
    if (self->m_flag54 == 0) {
        if (self->m_type != 0 || self->m_playerNo == -1) {
            self->m_vt->m_cpu(self);
        } else {
            self->m_vt->m_input(self);
        }
    }
    self->m_vt->m_post(self);
    fn_1_C8E0(self);
}

void fn_1_C808(CPlayerBase *self)
{
    fn_1_C8E0(self);
}

void fn_1_C830(CPlayerBase *self)
{
    self->m_flag55 = 1;
}

void fn_1_C83C(CPlayerBase *self)
{
    s8 pad = self->m_padNo;
    self->m_stickX = HuPadStkX[pad];
    self->m_stickY = -HuPadStkY[pad];
}

void fn_1_C8D8(CPlayerBase *self, s32 mot)
{
    self->m_nextMot = mot;
}

void fn_1_C8E0(CPlayerBase *self)
{
    s16 model = self->m_model;
    Hu3DModelPosSet(model, self->m_pos.x, self->m_pos.y, self->m_pos.z);
    Hu3DModelRotSet(model, self->m_rot.x, self->m_rot.y, self->m_rot.z);
    fn_1_C948(self, self->m_nextMot);
}

void fn_1_C948(CPlayerBase *self, u32 mot)
{
    if ((u32)self->m_curMot != mot) {
        Hu3DModelAttrReset(self->m_model, 0x40000002);
        if (self->m_playerNo != -1) {
            CharMotionShiftSet(self->m_charNo, self->m_motId[mot], 0.0f, self->m_motTbl[mot].shiftTime, self->m_motTbl[mot].attr);
        } else {
            Hu3DMotionShiftSet(self->m_model, self->m_motId[mot], 0.0f, self->m_motTbl[mot].shiftTime, self->m_motTbl[mot].attr);
        }
        self->m_curMot = mot;
    }
}

void fn_1_CA30(CPlayerBase *self)
{
    self->m_obj->objFunc = fn_1_C178;
}

void fn_1_CA44(CPlayerBase *self, s32 mode, s32 mot)
{
    s16 coin;
    s16 playerNo;
    switch (mode) {
        case 0:
            self->m_nextMot = mot;
            coin = GwPlayer[self->m_playerNo].mgCoinBonus + 10;
            playerNo = self->m_playerNo;
            if (_CheckFlag(0x1000F) == 0) {
                GwPlayer[playerNo].mgCoinBonus = coin;
            }
            break;
        case 1:
            self->m_nextMot = mot;
            break;
        case -1:
            self->m_nextMot = mot;
            break;
    }
    self->m_flag54 = 1;
    self->m_obj->objFunc = fn_1_C1C0;
}

void fn_1_CB20(CPlayerBase *self)
{
    self->m_obj->objFunc = fn_1_C130;
}

CPlayerBase *fn_1_CB34(CPlayerBase *self, OMOBJMAN *objMan, PlayerMotInfo *tbl, s32 num, s32 dataNum)
{
    fn_1_C390(self, objMan, tbl, num, dataNum, 1);
    self->m_vt = &lbl_1_data_394;
    return self;
}

// The player-base vtable: two zero header words, then the ten virtuals in slot order.
PlayerBaseVt lbl_1_data_440 = {
    { NULL, NULL },
    fn_1_C74C, fn_1_C774, fn_1_C830, fn_1_C808, fn_1_C83C, fn_1_BA5C, fn_1_BA60,
    fn_1_C590, fn_1_CA30, fn_1_CB20,
};
