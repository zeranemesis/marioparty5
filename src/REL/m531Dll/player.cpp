#include "REL/m531Dll.h"
#include <math.h>

extern "C" {
void PSVECSubtract(const Vec *a, const Vec *b, Vec *ab);
f32 PSVECMag(const Vec *v);
void PSVECScale(const Vec *src, Vec *dst, f32 scale);
void PSVECAdd(const Vec *a, const Vec *b, Vec *ab);
void PSMTXRotRad(Mtx m, char axis, f32 rad);
void PSMTXMultVecSR(const Mtx m, const Vec *src, Vec *dst);

void *__dl__FPv(void *ptr);
void *fn_1_0(s32 size);
HU3DMODELID Hu3DModelCreate(void *data);
void Hu3DModelHookSet(HU3DMODELID modelId, s32 hook, s16 arg);
void CharModelHookDustCreate(s16 charNo, s32 hook);
s32 CharModelItemHookGet(s16 charNo, s32 a, s32 b);
void CharModelStepFxSet(s16 charNo, s32 a);
void CharEffectLayerSet(s32 layer);
void CharEffectHitCreate(s32 type, Vec *pos, Vec *rot);
void CharEffectShoeHitCreate(s32 type, Vec *pos, Vec *rot);
void Hu3DMotionSpeedSet(HU3DMODELID modelId, f32 speed);
void HuAudFXStop(int seNo);
void omVibrate(s16 playerNo, s32 a, s32 b, s32 c);
void fn_1_D0C4(s16 charNo, Vec *pos);
void *fn_1_5700(void *self, const Vec *pos);
void *fn_1_66B8(void *self, u32 maxCnt, const Vec *pos, const Vec *rot);
void fn_1_7030(void *self, const Vec *src);
void fn_1_704C(void *self, const Vec *src);
void fn_1_2B08(CEffectParticle *self);
f32 fn_1_4B6C(m531::CFlow *flow, s32 mode, s32 col, s32 row);
void fn_1_4BC0(m531::CFlow *flow, s32 mode, s32 col, s32 row, f32 value);
f32 fn_1_F7BC(s32 a, Vec *p0, Vec *p1, f32 x, f32 y);
void *fn_1_805C(void *self, OMOBJMAN *objMan, s32 dataNum, s32 index);
void fn_1_8190(CStageObj *self);

CPlayerBase *fn_1_C208(CPlayerBase *self, OMOBJMAN *objMan, PlayerMotInfo *tbl, s32 num, s16 playerNo, u16 camBit);
CPlayerBase *fn_1_C590(CPlayerBase *self, s16 flag);
void fn_1_C74C(CPlayerBase *self);
void fn_1_C774(CPlayerBase *self);
void fn_1_C808(CPlayerBase *self);
void fn_1_C830(CPlayerBase *self);
void fn_1_C83C(CPlayerBase *self);
void fn_1_C8D8(CPlayerBase *self, s32 mot);
void fn_1_C8E0(CPlayerBase *self);
void fn_1_CA30(CPlayerBase *self);
void fn_1_CB20(CPlayerBase *self);
CPlayerBase *fn_1_CB34(CPlayerBase *self, OMOBJMAN *objMan, PlayerMotInfo *tbl, s32 num, s32 dataNum);
}

static inline s32 PlayerItemHook(s16 charNo)
{
    return CharModelItemHookGet(charNo, 2, 0);
}

static inline void PlayerPanSet(s16 charNo, Vec *pos)
{
    fn_1_D0C4(charNo, pos);
}

static inline CFlowStage *FlowGetStage(m531::CFlow *flow)
{
    return flow->m_stage;
}

static inline f32 StageGetAngle(CFlowStage *stage)
{
    return stage->m_angle;
}

class CEffectParticle {
public:
    void *m_anim;
    s16 m_model;
    void *m_work;
    virtual ~CEffectParticle();
};

extern "C" {
f32 fn_1_90CC(f32 from, f32 to, f32 rate);
f32 fn_1_91D0(f32 from, f32 to);
CPlayer *fn_1_9270(CPlayer *self, m531::CFlow *flow, s32 arg2, PlayerMotInfo *tbl, s32 num, s16 playerNo, s32 team);
CPlayer *fn_1_948C(CPlayer *self, s16 flag);
void fn_1_9520(CPlayer *self);
void fn_1_95DC(CPlayer *self);
void fn_1_964C(CPlayer *self);
void fn_1_97E4(CPlayer *self);
void fn_1_9878(CPlayer *self);
void fn_1_9BF4(CPlayer *self);
void fn_1_9CC4(CPlayer *self);
void fn_1_9E54(CPlayer *self);
void fn_1_9EF4(CPlayer *self);
void fn_1_9F1C(CPlayer *self);
void fn_1_9FFC(CPlayer *self);
void fn_1_A2C8(CPlayer *self, s32 exCol, s32 exRow);
f32 fn_1_A568(CPlayer *self);
void fn_1_A714(CPlayer *self, s16 dif);
void fn_1_AA98(CPlayer *self);
void fn_1_ACF8(CPlayer *self);
void fn_1_B0BC(CPlayer *self);
CPlayerRunner *fn_1_B55C(CPlayerRunner *self, OMOBJMAN *objMan, PlayerMotInfo *tbl, s32 num, s32 dataNum);
CPlayerBase *fn_1_B6C4(CPlayerBase *self, s16 flag);
CPlayerRunner *fn_1_B728(CPlayerRunner *self, s16 flag);
void fn_1_B7F4(CPlayerRunner *self);
void fn_1_BA5C(CPlayerBase *self);
void fn_1_BA60(CPlayerBase *self);
void fn_1_BA64(CPlayerBase *self);
void fn_1_BA68(void);

extern s32 lbl_1_bss_28;
extern s32 lbl_1_bss_2C;
extern s32 lbl_1_bss_30;
extern s32 lbl_1_data_348[4];
extern Vec lbl_1_data_358;
extern PlayerBaseVt lbl_1_data_364;
extern PlayerBaseVt lbl_1_data_394;
extern PlayerBaseVt lbl_1_data_3C4;
}

s32 lbl_1_bss_28;
s32 lbl_1_bss_2C;
s32 lbl_1_bss_30;

s32 lbl_1_data_348[4] = { -1, -1, 1, 1 };
Vec lbl_1_data_358 = { 0.0f, 0.0f, 100.0f };

static const s32 kHatData[3] = { 0x590008, 0x590009, 0 };

f32 fn_1_90CC(f32 from, f32 to, f32 rate)
{
    f32 diff;
    f32 result;

    diff = fmod(to - from, 360.0);
    if (diff < 0.0f) {
        diff += 360.0f;
    }
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    result = fmod(from + (rate * diff), 360.0);
    if (result < 0.0f) {
        result += 360.0f;
    }
    return result;
}

f32 fn_1_91D0(f32 from, f32 to)
{
    f32 diff;

    diff = fmod(to - from, 360.0);
    if (diff < 0.0f) {
        diff += 360.0f;
    }
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    return diff;
}

CPlayer *fn_1_9270(CPlayer *self, m531::CFlow *flow, s32 arg2, PlayerMotInfo *tbl, s32 num, s16 playerNo, s32 team)
{
    void *effect;

    fn_1_C208(&self->m_base, flow->m_objMan, tbl, num, playerNo, 1);
    self->m_base.m_vt = &lbl_1_data_3C4;
    self->m_turn = 0.0f;
    self->m_flow = flow;
    self->m_unkBC = arg2;
    self->m_unkD4 = 0.0f;
    self->m_unkD8 = 0;
    self->m_unkDC = 0.0f;
    self->m_unkE4 = 0.0f;
    self->m_unkE8 = 0;
    self->m_unkEC = 0;
    self->m_team = team;
    self->m_unkF8 = 0;
    self->m_unk100 = 0;
    self->m_unk118 = 1.0f;
    self->m_unkF0 = 0.9f;
    self->m_side = self->m_team >> 1;
    self->m_unk114 = 0;
    CharModelStepFxSet(self->m_base.m_charNo, 0);
    fn_1_9520(self);
    CharEffectLayerSet(4);
    self->m_unkC8.x = 0.0f;
    self->m_unkC8.y = 0.0f;
    self->m_unkC8.z = 0.0f;
    self->m_unk128.x = 0.0f;
    self->m_unk128.y = 0.0f;
    self->m_unk128.z = 0.0f;
    effect = fn_1_0(0x28);
    if (effect != NULL) {
        Vec rot = self->m_base.m_rot;
        Vec pos = self->m_base.m_pos;
        effect = fn_1_66B8(effect, 0x20, &pos, &rot);
    }
    self->m_hit = (CEffectParticle *)effect;
    fn_1_9FFC(self);
    return self;
}

CPlayer *fn_1_948C(CPlayer *self, s16 flag)
{
    if (self != NULL) {
        self->m_base.m_vt = &lbl_1_data_3C4;
        delete self->m_hit;
        fn_1_C590(&self->m_base, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void fn_1_9520(CPlayer *self)
{
    fn_1_C8D8(&self->m_base, 0xB);
    self->m_hatModel = Hu3DModelCreate(HuDataSelHeapReadNum(kHatData[self->m_side], 0x10000000, 2));
    Hu3DModelHookSet(self->m_base.m_model, PlayerItemHook(self->m_base.m_charNo), self->m_hatModel);
    Hu3DModelAttrSet(self->m_hatModel, 0x40000002);
}

void fn_1_95DC(CPlayer *self)
{
    CharModelHookDustCreate(self->m_base.m_charNo, PlayerItemHook(self->m_base.m_charNo));
}

void fn_1_964C(CPlayer *self)
{
    self->m_unkD4 = self->m_unkD4 + 0.016666666666666666;
    if (self->m_unkD4 > 3.0f) {
        f32 prev = self->m_unkDC;
        self->m_unkDC = self->m_unkDC + 1.0896277777777779;
        self->m_base.m_pos.y = self->m_base.m_pos.y - (0.5f * (prev + self->m_unkDC));
        if (self->m_base.m_pos.y < 10.0f) {
            self->m_base.m_pos.y = 10.0f;
            if (self->m_unkD8 == 0) {
                void *effect = fn_1_0(0x10);
                if (effect != NULL) {
                    Vec pos = self->m_base.m_pos;
                    fn_1_5700(effect, &pos);
                }
                self->m_unkD8 = 1;
                omVibrate(self->m_base.m_playerNo, 0x1E, 0xC, 0);
            }
        }
    }
    fn_1_C74C(&self->m_base);
    Vec pos = self->m_base.m_pos;
    PlayerPanSet(self->m_base.m_charNo, &pos);
}

void fn_1_97E4(CPlayer *self)
{
    void *effect;

    fn_1_C774(&self->m_base);
    effect = self->m_hit;
    fn_1_7030(effect, &self->m_unkC8);
    fn_1_704C(effect, &self->m_base.m_rot);
    Vec pos = self->m_base.m_pos;
    PlayerPanSet(self->m_base.m_charNo, &pos);
}

void fn_1_9878(CPlayer *self)
{
    u8 arrived = 0;
    Vec diff;
    Vec pos;
    f32 dist;
    f32 rotY;
    f32 angle;
    f32 newRot;
    f32 speed;
    f32 rad;

    PSVECSubtract(&self->m_base.m_home, &self->m_base.m_pos, &diff);
    dist = PSVECMag(&diff);
    rotY = self->m_base.m_rot.y;
    angle = fmod((f32)(180.0 * (atan2(diff.x, diff.z) / 3.141592653589793)) - rotY, 360.0);
    if (angle < 0.0f) {
        angle += 360.0f;
    }
    if (angle > 180.0f) {
        angle -= 360.0f;
    }
    newRot = fmod(rotY + (0.2f * angle), 360.0);
    if (newRot < 0.0f) {
        newRot += 360.0f;
    }
    self->m_base.m_rot.y = newRot;
    if (dist < 50.0f) {
        speed = 2.0f * (1.6666666f * (1.0f - ((50.0f - dist) / 100.0f)));
        if (speed < 2.0f) {
            speed = 2.0f;
        }
    } else {
        speed = 5.0f;
    }
    self->m_base.m_nextMot = 1;
    if (dist < 20.0f) {
        self->m_base.m_pos.x = self->m_base.m_home.x;
        self->m_base.m_pos.y = self->m_base.m_home.y;
        self->m_base.m_pos.z = self->m_base.m_home.z;
        self->m_base.m_rot.y = 0.0f;
        self->m_base.m_nextMot = 0;
        arrived = 1;
    } else {
        rad = (3.141592653589793 * self->m_base.m_rot.y) / 180.0;
        self->m_base.m_pos.x = self->m_base.m_pos.x + (speed * sin(rad));
        self->m_base.m_pos.z = self->m_base.m_pos.z + (speed * cos(rad));
    }
    fn_1_C8E0(&self->m_base);
    if (arrived != 0) {
        self->m_base.m_flag55 = 1;
    }
    pos = self->m_base.m_pos;
    PlayerPanSet(self->m_base.m_charNo, &pos);
}

void fn_1_9BF4(CPlayer *self)
{
    static m531::CFlow *s_flow = self->m_flow;

    self->m_base.m_rot.y = StageGetAngle(FlowGetStage(s_flow));
    fn_1_C808(&self->m_base);
    Vec pos = self->m_base.m_pos;
    PlayerPanSet(self->m_base.m_charNo, &pos);
}

void fn_1_9CC4(CPlayer *self)
{
    s32 dir;
    s32 se[2] = { 0x7A6, 0x7A7 };

    fn_1_CB20(&self->m_base);
    fn_1_2B08(self->m_hit);
    Hu3DModelAttrReset(self->m_hatModel, 0x40000002);
    Hu3DModelAttrSet(self->m_hatModel, 0x40000001);
    if (HuPadStkX[self->m_base.m_padNo] == 0 && self->m_base.m_type == 0) {
        self->m_dir = lbl_1_data_348[self->m_team];
        self->m_base.m_stickX = self->m_dir << 7;
        if (self->m_base.m_stickX < 0.0f) {
            self->m_unkE4 = 2.0f;
        } else if (self->m_base.m_stickX > 0.0f) {
            self->m_unkE4 = -2.0f;
        }
    } else {
        if ((frand() & 1) == 0) {
            dir = -1;
        } else {
            dir = 1;
        }
        self->m_dir = dir;
    }
    self->m_se = HuAudFXPlay(se[self->m_side]);
}

void fn_1_9E54(CPlayer *self)
{
    CharModelHookDustCreate(self->m_base.m_charNo, PlayerItemHook(self->m_base.m_charNo));
    fn_1_CA30(&self->m_base);
    delete self->m_hit;
    self->m_hit = (CEffectParticle *)0;
    HuAudFXStop(self->m_se);
}

void fn_1_9EF4(CPlayer *self)
{
    fn_1_C83C(&self->m_base);
}

void fn_1_9F1C(CPlayer *self)
{
    static m531::CFlow *s_flow = self->m_flow;
    s16 dif;

    dif = self->m_base.m_dif;
    self->m_unk118 = fn_1_A568(self);
    fn_1_4BC0(s_flow, self->m_side, (s32)self->m_cell.x, (s32)self->m_cell.z, self->m_unk118);
    fn_1_A714(self, dif);
    fn_1_9FFC(self);
    self->m_unk100 = self->m_unk100 + 1;
}

void fn_1_9FFC(CPlayer *self)
{
    s32 row;
    s32 col;
    u8 flag;
    u8 side;
    s32 team;
    static m531::CFlow *s_flow = self->m_flow;
    Vec *cell;

    if (!(self->m_unk118 < self->m_unkF0)) {
        cell = &self->m_cell;
        team = self->m_team;
        flag = 0;
        if (team != 1 && team != 2) {
            flag = 1;
        }
        if (flag != 0) {
            side = 1;
        } else {
            side = 0;
        }
        if (self->m_unkF8 == 1) {
            if (side == 1) {
                side = 0;
            } else {
                side = 1;
            }
        }
        if (side != 0) {
            for (row = 0; row < 5; row++) {
                for (col = 0; col < 5; col++) {
                    self->m_cell.x = col;
                    cell->z = row;
                    if (fn_1_4B6C(s_flow, self->m_side, (s32)self->m_cell.x, (s32)cell->z) < self->m_unkF0) {
                        return;
                    }
                }
            }
        } else {
            for (row = 4; row >= 0; row--) {
                for (col = 4; col >= 0; col--) {
                    self->m_cell.x = col;
                    cell->z = row + 5;
                    if (fn_1_4B6C(s_flow, self->m_side, (s32)self->m_cell.x, (s32)cell->z) < self->m_unkF0) {
                        return;
                    }
                }
            }
        }
        self->m_unkF0 = self->m_unkF0 + 0.05f;
        if (self->m_unkF0 > 1.0f) {
            self->m_unkF8 = 1;
            self->m_unkF0 = 1.0f;
        }
    }
}

void fn_1_A2C8(CPlayer *self, s32 exCol, s32 exRow)
{
    s32 row;
    s32 col;
    u8 flag;
    u8 side;
    s32 team;
    static m531::CFlow *s_flow = self->m_flow;
    Vec *cell;

    cell = &self->m_cell;
    team = self->m_team;
    flag = 0;
    if (team != 1 && team != 2) {
        flag = 1;
    }
    if (flag != 0) {
        side = 1;
    } else {
        side = 0;
    }
    if (self->m_unkF8 == 1) {
        if (side == 1) {
            side = 0;
        } else {
            side = 1;
        }
    }
    if (side != 0) {
        for (row = 4; row >= 0; row--) {
            if (row == exRow) {
                continue;
            }
            for (col = 4; col >= 0; col--) {
                if (col == exCol) {
                    continue;
                }
                self->m_cell.x = col;
                cell->z = row;
                if (fn_1_4B6C(s_flow, self->m_side, (s32)self->m_cell.x, (s32)cell->z) < self->m_unkF0) {
                    return;
                }
            }
        }
    } else {
        for (row = 0; row < 5; row++) {
            if (row + 5 == exRow) {
                continue;
            }
            for (col = 0; col < 5; col++) {
                if (col == exCol) {
                    continue;
                }
                self->m_cell.x = col;
                cell->z = row + 5;
                if (fn_1_4B6C(s_flow, self->m_side, (s32)self->m_cell.x, (s32)cell->z) < self->m_unkF0) {
                    return;
                }
            }
        }
    }
}

f32 fn_1_A568(CPlayer *self)
{
    Vec a;
    Vec b;
    f32 result;
    f32 offX;
    Vec *target;

    target = &self->m_target;
    if ((self->m_team >> 1) == 0) {
        offX = 615.0f;
    } else {
        offX = -95.0f;
    }
    a.x = (100.0f * self->m_cell.x) - offX;
    a.y = 0.0f;
    a.z = -230.0f + (100.0f * self->m_cell.z);
    b.x = 100.0f + a.x;
    b.y = 0.0f;
    b.z = 100.0f + a.z;
    result = fn_1_F7BC(self->m_unkBC, &a, &b, 100.0f, -230.0f);
    self->m_target.x = a.x + (0.5f * (b.x - a.x));
    target->y = 0.0f;
    target->z = a.z + (0.5f * (b.z - a.z));
    return result;
}

static const s32 kFollowTbl[4] = { 0x36, 0x2A, 0x1E, 0x18 };

void fn_1_A714(CPlayer *self, s16 dif)
{
    Vec diff;
    f32 dist;
    f32 angle;
    f32 mag;
    s32 dir;

    PSVECSubtract(&self->m_target, &self->m_base.m_pos, &diff);
    dist = PSVECMag(&diff);
    angle = fmod((f32)(180.0 * (atan2(diff.x, diff.z) / 3.141592653589793)) - self->m_base.m_rot.y, 360.0);
    if (angle < 0.0f) {
        angle += 360.0f;
    }
    if (angle > 180.0f) {
        angle -= 360.0f;
    }
    if (self->m_unkEC != 0) {
        if ((angle - self->m_base.m_rot.y) > 0.0f) {
            dir = 1;
        } else {
            dir = -1;
        }
        self->m_dir = dir;
        self->m_base.m_stickX = self->m_dir << 7;
        return;
    }
    if (dist < 100.0f) {
        if (dist < 50.0f) {
            fn_1_A2C8(self, (s32)self->m_cell.x, (s32)self->m_cell.z);
        }
    } else if (self->m_unk100 > self->m_unk114) {
        self->m_unk100 = 0;
        if (angle < 0.0f) {
            mag = -angle;
        } else {
            mag = angle;
        }
        self->m_unk114 = (s32)mag;
        if (angle < 0.0f) {
            dir = 1;
        } else {
            dir = -1;
        }
        self->m_dir = dir;
        if (dif < 2) {
            if (dif < 0) {
            } else if (self->m_unk114 < kFollowTbl[dif]) {
                self->m_unk114 = kFollowTbl[dif];
            }
        } else if (dif < 4) {
            if (dist > 150.0f && self->m_unk114 < kFollowTbl[dif]) {
                self->m_unk114 = kFollowTbl[dif];
            }
        }
    }
    self->m_base.m_stickX = self->m_dir << 7;
}

void fn_1_AA98(CPlayer *self)
{
    Mtx mtx;
    f32 rad;

    if (self->m_base.m_flag54 == 0) {
        if (self->m_base.m_stickX < 0.0f) {
            self->m_unkE4 = 2.0f;
        } else if (self->m_base.m_stickX > 0.0f) {
            self->m_unkE4 = -2.0f;
        }
        self->m_turn = self->m_turn + ((self->m_unkE4 - self->m_turn) / 30.0f);
        if (self->m_unkE4 != 0.0f) {
            self->m_base.m_rot.y = self->m_base.m_rot.y + self->m_turn;
            rad = (3.141592653589793 * self->m_base.m_rot.y) / 180.0;
            self->m_base.m_pos.x = self->m_base.m_pos.x + (0.5 * (1.6666666f * sin(rad)));
            self->m_base.m_pos.z = self->m_base.m_pos.z + (0.5 * (1.6666666f * cos(rad)));
            self->m_base.m_nextMot = 0xA;
        } else {
            self->m_base.m_nextMot = 0xB;
        }
        if (self->m_base.m_rot.y >= 360.0f) {
            self->m_base.m_rot.y = self->m_base.m_rot.y - 360.0f;
        }
        if (self->m_base.m_rot.y < 0.0f) {
            self->m_base.m_rot.y = self->m_base.m_rot.y + 360.0f;
        }
        PSMTXRotRad(mtx, 'y', 0.017453292f * self->m_base.m_rot.y);
        PSMTXMultVecSR(mtx, &lbl_1_data_358, &self->m_unk128);
        PSVECAdd(&self->m_base.m_pos, &self->m_unk128, &self->m_unkC8);
        fn_1_B0BC(self);
        fn_1_ACF8(self);
        PSVECAdd(&self->m_base.m_pos, &self->m_unk128, &self->m_unkC8);
    }
}

void fn_1_ACF8(CPlayer *self)
{
    Vec newPos;
    Vec a;
    Vec b;
    s32 side;

    newPos = self->m_base.m_pos;
    a = self->m_base.m_pos;
    b = self->m_unkC8;
    if (self->m_team < 2) {
        side = 0;
    } else {
        side = 1;
    }
    if (side == 0) {
        a.x += 555.0f;
        b.x += 555.0f;
    } else {
        a.x += -95.0f;
        b.x += -95.0f;
    }
    a.z += 252.0f;
    b.z += 252.0f;
    if ((a.x - 60.000004f) < 0.0f) {
        newPos.x -= a.x - 60.000004f;
    }
    if ((60.000004f + a.x) > 460.0f) {
        newPos.x -= (60.000004f + a.x) - 460.0f;
    }
    if ((a.z - 60.000004f) < 0.0f) {
        newPos.z -= a.z - 60.000004f;
    }
    if ((60.000004f + a.z) > 876.0f) {
        newPos.z -= (60.000004f + a.z) - 876.0f;
    }
    if ((b.x - 50.0f) < 0.0f) {
        newPos.x -= b.x - 50.0f;
    }
    if ((50.0f + b.x) > 460.0f) {
        newPos.x -= (50.0f + b.x) - 460.0f;
    }
    if ((b.z - 50.0f) < 0.0f) {
        newPos.z -= b.z - 50.0f;
    }
    if ((50.0f + b.z) > 876.0f) {
        newPos.z -= (50.0f + b.z) - 876.0f;
    }
    self->m_base.m_pos.x = newPos.x;
    self->m_base.m_pos.y = newPos.y;
    self->m_base.m_pos.z = newPos.z;
}

void fn_1_B0BC(CPlayer *self)
{
    Vec cPos;
    Vec otherPos;
    Vec otherC8;
    Vec hit;
    Vec push0;
    Vec push1;
    Vec diff;
    Vec p2;
    f32 dist;
    s32 team;
    s32 i;
    CPlayer *other;
    static m531::CFlow *s_flow = self->m_flow;
    Vec *pos;

    pos = &self->m_base.m_pos;
    cPos = self->m_unkC8;
    team = self->m_team;
    for (i = 0; i < 4; i++) {
        other = s_flow->m_player[i];
        if (((team & 1) ^ 1) + ((team / 2) * 2) == other->m_team) {
            break;
        }
    }
    otherPos = other->m_base.m_pos;
    p2 = otherPos;
    self->m_unkEC = 0;
    PSVECSubtract(pos, &p2, &diff);
    dist = PSVECMag(&diff);
    if (dist < 120.00001f) {
        PSVECScale(&diff, &diff, (120.00001f - dist) / dist);
        PSVECScale(&diff, &push0, -0.5f);
        PSVECScale(&diff, &push1, 0.5f);
        PSVECAdd(&push0, &p2, &p2);
        PSVECAdd(&push1, pos, pos);
        other->m_base.m_pos.x = p2.x;
        other->m_base.m_pos.y = p2.y;
        other->m_base.m_pos.z = p2.z;
        self->m_unkEC = 1;
    }
    otherC8 = other->m_unkC8;
    PSVECSubtract(pos, &otherC8, &diff);
    dist = PSVECMag(&diff);
    if (dist < 120.00001f) {
        PSVECScale(&diff, &diff, (120.00001f - dist) / dist);
        PSVECScale(&diff, &push0, -0.5f);
        PSVECScale(&diff, &push1, 0.5f);
        PSVECAdd(&push0, &p2, &p2);
        PSVECAdd(&push1, pos, pos);
        other->m_base.m_pos.x = p2.x;
        other->m_base.m_pos.y = p2.y;
        other->m_base.m_pos.z = p2.z;
        self->m_unkEC = 1;
    }
    otherC8 = other->m_unkC8;
    PSVECSubtract(&cPos, &otherC8, &diff);
    dist = PSVECMag(&diff);
    if (dist < 100.0f) {
        hit = diff;
        PSVECScale(&hit, &hit, -0.5f);
        PSVECAdd(&cPos, &hit, &hit);
        PSVECScale(&diff, &diff, (100.0f - dist) / dist);
        PSVECScale(&diff, &push0, -0.5f);
        PSVECScale(&diff, &push1, 0.5f);
        PSVECAdd(&push0, &p2, &p2);
        PSVECAdd(&push1, pos, pos);
        other->m_base.m_pos.x = p2.x;
        other->m_base.m_pos.y = p2.y;
        other->m_base.m_pos.z = p2.z;
        if ((self->m_unkE8 & 0x1F) == 0) {
            CharEffectHitCreate(1, &hit, &self->m_base.m_rot);
        }
        self->m_unkE8 = self->m_unkE8 + 1;
        omVibrate(self->m_base.m_playerNo, 1, 6, 6);
        self->m_unkEC = 1;
        return;
    }
    self->m_unkE8 = 0;
}

static const s32 kRunnerData[2] = { 0x590006, 0x590007 };

CPlayerRunner *fn_1_B55C(CPlayerRunner *self, OMOBJMAN *objMan, PlayerMotInfo *tbl, s32 num, s32 dataNum)
{
    s32 i;
    Vec ofs[2] = { { -325.0f, 0.0f, -100.0f }, { 325.0f, 0.0f, -100.0f } };

    fn_1_CB34(&self->m_base, objMan, tbl, num, dataNum);
    self->m_base.m_vt = &lbl_1_data_364;
    self->m_frame = 0;
    self->m_vel = 20.0f;
    self->m_hit = 0;
    for (i = 0; i < 2; i++) {
        void *obj = fn_1_0(0x60);
        if (obj != NULL) {
            obj = fn_1_805C(obj, objMan, kRunnerData[i], i);
        }
        self->m_obj[i] = (CStageObj *)obj;
        self->m_obj[i]->m_pos.x = ofs[i].x;
        self->m_obj[i]->m_pos.y = ofs[i].y;
        self->m_obj[i]->m_pos.z = ofs[i].z;
    }
    HuAudFXPlay(0x7A2);
    return self;
}

CPlayerBase *fn_1_B6C4(CPlayerBase *self, s16 flag)
{
    if (self != NULL) {
        self->m_vt = &lbl_1_data_394;
        fn_1_C590(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CPlayerRunner *fn_1_B728(CPlayerRunner *self, s16 flag)
{
    s32 i;

    if (self != NULL) {
        self->m_base.m_vt = &lbl_1_data_364;
        for (i = 0; i < 2; i++) {
            CStageObj *obj = self->m_obj[i];
            if (obj != NULL) {
                obj->m_vt->m_dtor(obj, 1);
            }
        }
        if (self != NULL) {
            self->m_base.m_vt = &lbl_1_data_394;
            fn_1_C590(&self->m_base, 0);
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void fn_1_B7F4(CPlayerRunner *self)
{
    s32 i;

    self->m_base.m_rot.y = 90.0f;
    self->m_frame = self->m_frame + 1;
    if ((f32)self->m_frame < 72.0f) {
        self->m_base.m_nextMot = 1;
        self->m_base.m_pos.x = self->m_base.m_pos.x + 5.0f;
    } else {
        self->m_base.m_nextMot = 2;
        self->m_base.m_pos.x = self->m_base.m_pos.x + 11.666667f;
        if (self->m_hit == 0) {
            Hu3DMotionSpeedSet(self->m_base.m_model, 2.0f);
        } else {
            Hu3DMotionSpeedSet(self->m_base.m_model, 0.5f);
        }
    }
    if (self->m_base.m_pos.x > -340.0f && self->m_base.m_pos.x < -300.0f) {
        for (i = 0; i < 2; i++) {
            fn_1_8190(self->m_obj[i]);
        }
    }
    if (self->m_base.m_pos.x > -200.0f && self->m_base.m_pos.x < 300.0f) {
        if (self->m_base.m_pos.x < 0.0f) {
            self->m_vel = self->m_vel + (-self->m_vel / 6.0f);
            self->m_base.m_pos.y = self->m_base.m_pos.y + self->m_vel;
        } else {
            self->m_vel = self->m_vel + ((20.0f - self->m_vel) / 6.0f);
            self->m_base.m_pos.y = self->m_base.m_pos.y + -self->m_vel;
        }
        if (self->m_hit == 0) {
            CharEffectShoeHitCreate(1, &self->m_base.m_pos, &self->m_base.m_rot);
            self->m_hit = 1;
        }
    }
    fn_1_C8E0(&self->m_base);
}

void fn_1_BA5C(CPlayerBase *self)
{
}

void fn_1_BA60(CPlayerBase *self)
{
}

void fn_1_BA64(CPlayerBase *self)
{
}

void fn_1_BA68(void)
{
    lbl_1_bss_28 = 0x78;
    lbl_1_bss_2C = 0xD2;
    lbl_1_bss_30 = 0xD2;
}

// Vtables of the three player classes; slot order follows PlayerBaseVt.
#define VT(base) ((void (*)(CPlayerBase *))base)
PlayerBaseVt lbl_1_data_364 = {
    { NULL, NULL },
    fn_1_C74C, VT(fn_1_B7F4), fn_1_C830, fn_1_C808, fn_1_C83C, fn_1_BA5C, fn_1_BA60,
    (CPlayerBase * (*)(CPlayerBase *, s16)) fn_1_B728, fn_1_CA30, fn_1_CB20,
};

PlayerBaseVt lbl_1_data_394 = {
    { NULL, NULL },
    fn_1_C74C, fn_1_BA64, fn_1_C830, fn_1_C808, fn_1_C83C, fn_1_BA5C, fn_1_BA60,
    fn_1_B6C4, fn_1_CA30, fn_1_CB20,
};

PlayerBaseVt lbl_1_data_3C4 = {
    { NULL, NULL },
    VT(fn_1_964C), VT(fn_1_97E4), VT(fn_1_9878), VT(fn_1_9BF4), VT(fn_1_9EF4), VT(fn_1_9F1C), VT(fn_1_AA98),
    (CPlayerBase * (*)(CPlayerBase *, s16)) fn_1_948C, VT(fn_1_9E54), VT(fn_1_9CC4),
};
