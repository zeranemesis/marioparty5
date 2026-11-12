#include "dolphin/types.h"
#include "dolphin/mtx.h"
#include "game/charman.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "REL/m521Dll/m521Types.h"

extern "C" {
double atan2(double y, double x);
double sin(double x);
double cos(double x);
void abort(void);
void *memcpy(void *dst, const void *src, u32 n);
void *memmove(void *dst, const void *src, u32 n);

void __dl__FPv(void *ptr);
void *fn_1_4A68(u32 n);
void fn_1_4C3C(const char *cond, const char *file, s32 line);
void fn_1_4C9C(void *file, const char *msg);
void fn_1_5CCC(CListNode *self);
void fn_1_5CD8(void *obj, s16 flag);
void fn_1_5D98(CListNode *self, CListNode *list);
s32 fn_1_6998(void);

// stderr of the C library (element 2 of the FILE table)
extern u8 __files[];
#define PLAYER_STDERR ((void *)(__files + 0xA0))

// CModel (m521 unit)
CModel *fn_1_7780(CModel *self, s16 charNo, s32 kind);
void fn_1_795C(CModel *m, const void *pos);
void fn_1_79A4(CModel *m, const void *rot);
MtxPtr getMatrix__6CModelFv(CModel *m);

// controllers (m521 unit)
CCtl *fn_1_AF78(CCtl *self, s32 charIdx);
void fn_1_AF00(CCtl *self);
CCtl *fn_1_B3CC(CCtl *self, CPlayer *owner);

// clock hands (clock unit)
s32 fn_1_139C(CClock *c, s32 idx, f32 v);
s32 fn_1_1824(CClock *c, M521Vec *pt);
s32 fn_1_1CA0(CClock *c, const M521Vec *pos, M521Vec *outNormal, f32 *outDot);
u8 fn_1_9B2C(M521Sphere *a, M521Sphere *b, f32 f0, f32 f1);

extern CModel *lbl_1_bss_A30;
extern CModel *lbl_1_bss_A40;
extern u8 lbl_1_bss_A4C;
extern CClock *lbl_1_bss_A50;
extern CClock *lbl_1_bss_A54;
extern CListNode *lbl_1_bss_A70;

s32 lbl_1_bss_AA8;
s32 lbl_1_bss_AAC;
s32 lbl_1_bss_AB0;

#ifdef __MWERKS__
#define PL_RODATA __declspec(section ".rodata")
#else
#define PL_RODATA
#endif

f32 lbl_1_data_680 = 20.0f;
M521Ptmf lbl_1_data_684 = M521_PTMF_NULL_INIT;
const char *lbl_1_data_6F0[4] = { "cyara1", "cyara2", "cyara3", "cyara4" };

s16 fn_1_15298(M521Sphere *pos);
f32 fn_1_15338(f32 a, f32 b);
f32 fn_1_153F0(f32 a, f32 b, f32 limit);
void fn_1_15578(CPlayer *self);
CCtl *fn_1_155B8(s32 charIdx, CPlayer *owner);
CPlayer *fn_1_15664(CPlayer *self, s32 charIdx, s32 slot);
CDeque16 *fn_1_15AF0(CDeque16 *self, s16 flag);
CDeque16 *fn_1_15B84(CDeque16 *self, s16 flag);
CDeque16 *fn_1_15C10(CDeque16 *self, s16 flag);
CCtl **fn_1_15C94(CCtl **self, s16 flag);
CListNode *fn_1_15D10(CListNode *self, s16 flag);
s32 fn_1_15D68(CPlayer *self);
void fn_1_15D94(CPlayer *a, CPlayer *b);
void fn_1_15DF0(CPlayer *self, u32 motId);
void fn_1_15E68(s32 slot, MtxPtr mtx);
void fn_1_15F04(CPlayer *self);
void fn_1_1644C(CPlayer *self, s32 kind);
CClock *fn_1_1650C(CPlayer *self);
void fn_1_1654C(CPlayer *self, s32 motion, u8 flag);
void fn_1_16648(CPlayer *self);
void fn_1_16720(CPlayer *self);
void fn_1_168C0(CPlayer *self);
f32 fn_1_16BD0(CPlayer *self);
void fn_1_16C54(CPlayer *self, u32 state);
void fn_1_16F98(CPlayer *self);
void fn_1_170F8(CPlayer *self);
u8 fn_1_17218(CPlayer *self);
void fn_1_173B8(CPlayer *self);
void fn_1_173EC(CPlayer *self);
void fn_1_1743C(CPlayer *self);
void fn_1_174B0(CPlayer *self);
void fn_1_17578(CPlayer *self, M521Vec *out);
void fn_1_1763C(CPlayer *self);
u8 fn_1_17760(CPlayer *self);
void fn_1_17788(CPlayer *self);
void fn_1_17794(CPlayer *self);
void fn_1_177A0(CPlayer *self);
void fn_1_17840(CPlayer *self);
void fn_1_17884(CPlayer *self);
void fn_1_17920(CPlayer *self);
void fn_1_179E8(CPlayer *self);
u16 *fn_1_179F4(CDeque16 *self, u32 idx);
void fn_1_17A6C(CDeque16 *self, u16 *pos, u32 n, u16 *val);
void fn_1_17E1C(CDeque16 *self, u32 cap);
void fn_1_17F44(void);

// Per-frame handler stored in the player hook (PTMF slot).
M521Ptmf lbl_1_data_700 = M521_PTMF_INIT(fn_1_1763C);

// Start x position of each slot.
PL_RODATA const f32 lbl_1_rodata_33C[4] = { -600.0f, -300.0f, 300.0f, 600.0f };
PL_RODATA const M521Vec lbl_1_rodata_360 = { 0.0f, 0.0f, 0.0f };

s16 fn_1_15298(M521Sphere *pos)
{
    M521Vec screen;
    s16 pan;
    s16 v;

    Hu3D3Dto2D((HuVecF *)pos, 1, (HuVecF *)&screen);
    screen.x = screen.x * 0.11111111f;
    v = (s16)(32.0f + screen.x);
    if (v < 0) {
        pan = 0;
    } else if (v > 0x7F) {
        pan = 0x7F;
    } else {
        pan = v;
    }
    return pan;
}

// Signed angle (degrees) from b to a, wrapped to the 16 bit angle range.
f32 fn_1_15338(f32 a, f32 b)
{
    s32 ia;
    s32 ib;
    u16 wrapped;

    ia = (s32)(182.04444444444445 * a);
    ib = (s32)(182.04444444444445 * b);
    wrapped = (u16)((ia - ib) - 0x8000);
    return (f32)(0.0054931640625 * (f64)((s32)wrapped - 0x8000));
}

f32 fn_1_153F0(f32 a, f32 b, f32 limit)
{
    f32 diff;
    f32 clamped;
    s32 ia;
    s32 ib;
    u16 wrapped;

    ia = (s32)(182.04444444444445 * a);
    ib = (s32)(182.04444444444445 * b);
    wrapped = (u16)((ia - ib) - 0x8000);
    diff = (f32)(0.0054931640625 * (f64)((s32)wrapped - 0x8000));
    if (diff < -limit) {
        clamped = -limit;
    } else if (diff > limit) {
        clamped = limit;
    } else {
        clamped = diff;
    }
    return clamped;
}

bool fn_1_15554(CCtlInput *in)
{
    return in->m_mag >= 0.7f;
}

void fn_1_15578(CPlayer *self)
{
    self->m_pos.y = lbl_1_data_680;
    self->m_pos.x = lbl_1_rodata_33C[self->m_slot];
    self->m_pos.z = 0.0f;
}

CCtl *fn_1_155B8(s32 charIdx, CPlayer *owner)
{
    CCtl *ctl;
    CCtl *other;

    if (GwPlayerConf[charIdx].type != 0) {
        ctl = (CCtl *)fn_1_4A68(0x94);
        if (ctl != 0) {
            ctl = fn_1_B3CC(ctl, owner);
        }
        return ctl;
    }
    other = (CCtl *)fn_1_4A68(0x20);
    if (other != 0) {
        other = fn_1_AF78(other, charIdx);
    }
    return other;
}

CPlayer *fn_1_15664(CPlayer *self, s32 charIdx, s32 slot)
{
    CCtl *ctl;
    CCtl *ctlCpu;
    CCtl *ctlPad;
    CModel *model;
    s32 motIds[10];
    M521Ptmf nullHook;
    s16 modelId;

    fn_1_5CCC(&self->m_node);
    if (GwPlayerConf[charIdx].type != 0) {
        ctlCpu = (CCtl *)fn_1_4A68(0x94);
        if (ctlCpu != 0) {
            ctlCpu = fn_1_B3CC(ctlCpu, self);
        }
        ctl = ctlCpu;
    } else {
        ctlPad = (CCtl *)fn_1_4A68(0x20);
        if (ctlPad != 0) {
            ctlPad = fn_1_AF78(ctlPad, charIdx);
        }
        ctl = ctlPad;
    }
    self->m_ctl = ctl;
    self->m_slot = slot;
    self->m_i38 = 0;
    self->m_timer = 0;
    self->m_i40 = 0;
    self->m_i44 = 0;
    self->m_i48 = 0;
    self->m_mot.m_cap = 0;
    self->m_mot.m_size = 0;
    self->m_mot.m_data = 0;
    model = (CModel *)fn_1_4A68(sizeof(CModel));
    if (model != 0) {
        model = fn_1_7780(model, GwPlayerConf[charIdx].charNo, 2);
    }
    self->m_model = model;
    if (lbl_1_bss_A70 == 0) {
        fn_1_4C3C("sm_ptr != 0", "a_set.h", 0x24);
    }
    fn_1_5D98(&self->m_node, lbl_1_bss_A70);
    self->m_pos.r = 50.0f;
    self->m_padNo = charIdx;
    self->m_charNo = GwPlayerConf[charIdx].charNo;
    self->m_motion = -1;
    self->m_mot.m_size = 0;
    fn_1_17E1C(&self->m_mot, 0xA);
    fn_1_15DF0(self, 0x8C0000);
    fn_1_15DF0(self, 0x8C0036);
    fn_1_15DF0(self, 0x8C0001);
    fn_1_15DF0(self, 0x8C0075);
    fn_1_15DF0(self, 0x8C0006);
    fn_1_15DF0(self, 0x8C0031);
    fn_1_15DF0(self, 0x8C0003);
    fn_1_15DF0(self, 0x8C0004);
    fn_1_15DF0(self, 0x8C0030);
    fn_1_15DF0(self, 0x8C002E);
    if (self->m_mot.m_size != 0xA) {
        fn_1_4C3C("m_mot_id.size() == NUMBEROF_MOTION", "player.cpp", 0x77);
    }
    CharMotionDataClose(self->m_charNo);
    modelId = self->m_model->m_id;
    Hu3DModelShadowSet(modelId);
    Hu3DModelShadowDispOn(modelId);
    Hu3DModelLayerSet(modelId, 0);
    Hu3DModelCameraSet(modelId, 1);
    self->m_pos.y = lbl_1_data_680;
    self->m_pos.x = lbl_1_rodata_33C[self->m_slot];
    self->m_pos.z = 0.0f;
    nullHook = lbl_1_data_684;
    self->m_hook = nullHook;
    self->m_vel.x = 0.0f;
    self->m_vel.y = 0.0f;
    self->m_vel.z = 0.0f;
    self->m_acc.x = 0.0f;
    self->m_acc.y = 0.0f;
    self->m_acc.z = 0.0f;
    self->m_state = 0;
    self->m_f60 = 0.0f;
    self->m_f64 = 0.0f;
    self->m_flag51 = 0;
    self->m_ctl->m_in.m_scale = (f32)((f32)(GwPlayerConf[charIdx].dif + 1) * 0.25f);
    self->m_req = -1;
    return self;
}

CDeque16 *fn_1_15AF0(CDeque16 *self, s16 flag)
{
    if (self != 0) {
        if (self != 0 && self != 0 && self->m_data != 0) {
            __dl__FPv(self->m_data);
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CDeque16 *fn_1_15B84(CDeque16 *self, s16 flag)
{
    if (self != 0) {
        if (self != 0 && self->m_data != 0) {
            __dl__FPv(self->m_data);
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CDeque16 *fn_1_15C10(CDeque16 *self, s16 flag)
{
    if (self != 0) {
        if (self->m_data != 0) {
            __dl__FPv(self->m_data);
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CCtl **fn_1_15C94(CCtl **self, s16 flag)
{
    CCtl *ctl;

    if (self != 0) {
        ctl = *self;
        if (ctl != 0) {
            ((void (*)(CCtl *, s32))ctl->m_vtable[2])(ctl, 1);
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CListNode *fn_1_15D10(CListNode *self, s16 flag)
{
    if (self != 0) {
        fn_1_5CD8(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

s32 fn_1_15D68(CPlayer *self)
{
    return GwPlayerConf[self->m_padNo].type != 0;
}

void fn_1_15D94(CPlayer *a, CPlayer *b)
{
    if (fn_1_9B2C(&a->m_pos, &b->m_pos, 0.5f, 1.001f) != 0) {
    }
}

void fn_1_15DF0(CPlayer *self, u32 motId)
{
    u16 mot;

    mot = CharMotionCreate(self->m_charNo, motId);
    fn_1_17A6C(&self->m_mot, self->m_mot.m_data + self->m_mot.m_size, 1, &mot);
}

void fn_1_15E68(s32 slot, MtxPtr mtx)
{
    CModel *model;

    if ((slot & 2) == 0) {
        model = lbl_1_bss_A30;
    } else {
        model = lbl_1_bss_A40;
    }
    Hu3DModelObjMtxGet(model->m_id, (char *)lbl_1_data_6F0[slot & 3], mtx);
}

void fn_1_15F04(CPlayer *self)
{
    M521Sphere *pos;
    s16 pan;
    s32 idx;
    CModel *srcModel;
    MtxPtr mtx;
    Mtx rot;
    M521Vec rotVec;
    M521Vec posVec;

    pan = fn_1_15298(&self->m_pos);
    CharModelVoicePanSet(self->m_charNo, pan);
    if (self->m_req >= 0 && fn_1_17760(self) == 0) {
        self->m_i40 = 0x9C;
        switch (self->m_req) {
        case 0:
            CharMotionVoiceOnSet(self->m_charNo, 0x30, 0);
            CharWinLoseVoicePlay(self->m_charNo, 0x8C0030, 0x1FB);
            fn_1_1654C(self, 8, 1);
            fn_1_16C54(self, 6);
            break;
        case 1:
            fn_1_1654C(self, 9, 1);
            fn_1_16C54(self, 6);
            break;
        }
        self->m_req = -1;
    }
    if (M521PtmfTest(&self->m_hook) != 0) {
        M521PtmfCall(self, &self->m_hook);
    }
    self->m_f60 = self->m_f60 + fn_1_153F0(self->m_f64, self->m_f60, 13.666667f);
    fn_1_168C0(self);
    fn_1_16720(self);
    fn_1_1824(fn_1_1650C(self), (M521Vec *)&self->m_pos);
    if (self->m_flag51 == 0) {
        rotVec.x = 0.0f;
        rotVec.y = self->m_f60;
        rotVec.z = 0.0f;
        fn_1_79A4(self->m_model, &rotVec);
        fn_1_795C(self->m_model, &self->m_pos);
    } else {
        posVec = lbl_1_rodata_360;
        fn_1_79A4(self->m_model, &posVec);
        fn_1_795C(self->m_model, &posVec);
        mtx = getMatrix__6CModelFv(self->m_model);
        idx = self->m_slot;
        if ((idx & 2) == 0) {
            srcModel = lbl_1_bss_A30;
        } else {
            srcModel = lbl_1_bss_A40;
        }
        Hu3DModelObjMtxGet(srcModel->m_id, (char *)lbl_1_data_6F0[idx & 3], mtx);
        if (GwPlayerConf[self->m_padNo].charNo == 6) {
            PSMTXRotRad(rot, 'Y', 0.43633232f);
            PSMTXTransApply(mtx, mtx, -20.0f, 0.0f, -50.0f);
            PSMTXConcat(mtx, rot, mtx);
        }
    }
    if (self->m_timer > 0) {
        if (--self->m_timer == 0) {
        }
    }
}

void fn_1_1644C(CPlayer *self, s32 kind)
{
    s32 padNo;

    padNo = self->m_padNo;
    if (GwPlayerConf[padNo].type == 0) {
        switch (kind) {
        case 0:
            omVibrate(padNo, 0xC, 6, 6);
            return;
        case 1:
            omVibrate(padNo, 0xC, 4, 2);
            return;
        case 2:
            omVibrate(padNo, 0xC, 0xC, 0);
            break;
        }
    }
}

CClock *fn_1_1650C(CPlayer *self)
{
    CClock *clock;

    if (self->m_slot < 2) {
        clock = lbl_1_bss_A50;
    } else {
        clock = lbl_1_bss_A54;
    }
    return clock;
}

void fn_1_1654C(CPlayer *self, s32 motion, u8 flag)
{
    s16 modelId;
    s16 mot;
    u32 attr;

    modelId = self->m_model->m_id;
    mot = *fn_1_179F4(&self->m_mot, motion);
    if (self->m_motion != motion) {
        if (flag == 0) {
            attr = 0x40000001;
        } else {
            attr = 0;
        }
        CharMotionShiftSet(self->m_charNo, mot, 0.0f, 18.0f, attr);
        self->m_motion = motion;
        switch (motion) {
        case 4:
            break;
        case 5:
            if (lbl_1_bss_A4C != 0) {
                CharWinLoseVoicePlay(self->m_charNo, 0x8C0031, 0x200);
            }
            break;
        }
    }
}

void fn_1_16648(CPlayer *self)
{
    s16 modelId;
    s16 mot;
    M521Ptmf hook;

    modelId = self->m_model->m_id;
    mot = *fn_1_179F4(&self->m_mot, 0);
    if (self->m_motion != 0) {
        CharMotionShiftSet(self->m_charNo, mot, 0.0f, 18.0f, 0x40000001);
        self->m_motion = 0;
    }
    hook = lbl_1_data_700;
    self->m_hook = hook;
}

void fn_1_16720(CPlayer *self)
{
    M521Vec diff;
    M521Vec center;
    M521Sphere sphere;
    s32 side;
    f32 mag;

    if (self->m_pos.y < lbl_1_data_680) {
        self->m_pos.y = lbl_1_data_680;
        self->m_vel.y = 0.0f;
    }
    if (self->m_slot < 2) {
        side = -0x1C2;
    } else {
        side = 0x1C2;
    }
    center.x = (f32)side;
    center.y = self->m_pos.y;
    center.z = 0.0f;
    sphere.x = center.x;
    sphere.y = center.y;
    sphere.z = center.z;
    sphere.r = 30.000002f;
    fn_1_9B2C(&sphere, &self->m_pos, 0.0f, 1.0f);
    PSVECSubtract((M521Vec *)&self->m_pos, &center, &diff);
    mag = PSVECMag(&diff);
    if (!(mag < 320.0f)) {
        PSVECScale(&diff, &diff, 1.0f + ((320.0f - mag) / mag));
        PSVECAdd(&center, &diff, (M521Vec *)&self->m_pos);
    }
}

void fn_1_168C0(CPlayer *self)
{
    M521Vec prev;
    M521Vec normal;
    f32 dot;
    f32 angle;
    f32 clamped;
    CClock *clock;
    CClock *clock2;
    s32 idx;

    self->m_acc.y = (f32)((f64)self->m_acc.y + -3.3333333333333335);
    prev.x = self->m_vel.x;
    prev.y = self->m_vel.y;
    prev.z = self->m_vel.z;
    PSVECAdd(&self->m_acc, &self->m_vel, &self->m_vel);
    self->m_acc.x = 0.0f;
    self->m_acc.y = 0.0f;
    self->m_acc.z = 0.0f;
    PSVECAdd(&self->m_vel, &prev, &prev);
    PSVECScale(&prev, &prev, 0.5f);
    PSVECAdd(&prev, (M521Vec *)&self->m_pos, (M521Vec *)&self->m_pos);
    if (self->m_state == 5) {
        if (self->m_slot < 2) {
            clock = lbl_1_bss_A50;
        } else {
            clock = lbl_1_bss_A54;
        }
        idx = fn_1_1CA0(clock, (M521Vec *)&self->m_pos, &normal, &dot);
        self->m_i5C = idx;
        if (idx < 0) {
            fn_1_16C54(self, 1);
            return;
        }
        if (fn_1_16BD0(self) >= 328.0f) {
            if (dot < 0.0f) {
                angle = (f32)(6.0f + (180.0 * (atan2(normal.x, normal.z) / 3.141592653589793)));
            } else {
                angle = (f32)((180.0 * (atan2(-normal.x, -normal.z) / 3.141592653589793)) - 6.0f);
            }
            self->m_f60 = angle;
        }
        if (self->m_slot < 2) {
            clock2 = lbl_1_bss_A50;
        } else {
            clock2 = lbl_1_bss_A54;
        }
        if (dot < -0.6666667f) {
            clamped = -0.6666667f;
        } else if (dot > 0.6666667f) {
            clamped = 0.6666667f;
        } else {
            clamped = dot;
        }
        if (fn_1_139C(clock2, idx, clamped) == 1) {
            if (GwPlayerConf[self->m_padNo].type == 0) {
                omVibrate(self->m_padNo, 0xC, 4, 2);
            }
        }
    }
}

f32 fn_1_16BD0(CPlayer *self)
{
    CClock *clock;
    M521Vec diff;

    if (self->m_slot < 2) {
        clock = lbl_1_bss_A50;
    } else {
        clock = lbl_1_bss_A54;
    }
    PSVECSubtract(&clock->m_pos, (M521Vec *)&self->m_pos, &diff);
    return PSVECMag(&diff);
}

void fn_1_16C54(CPlayer *self, u32 state)
{
    s16 modelId;
    s16 mot;
    CCtlInput *in;

    self->m_timer = 0x96;
    switch (state) {
    case 0:
        self->m_i48 = 1;
        break;
    case 1:
        modelId = self->m_model->m_id;
        mot = *fn_1_179F4(&self->m_mot, 2);
        if (self->m_motion != 2) {
            CharMotionShiftSet(self->m_charNo, mot, 0.0f, 18.0f, 0x40000001);
            self->m_motion = 2;
        }
        break;
    case 2:
        modelId = self->m_model->m_id;
        mot = *fn_1_179F4(&self->m_mot, 6);
        if (self->m_motion != 6) {
            CharMotionShiftSet(self->m_charNo, mot, 0.0f, 18.0f, 0);
            self->m_motion = 6;
        }
        self->m_acc.y = (f32)((f64)self->m_acc.y + 50.0);
        in = &self->m_ctl->m_in;
        self->m_acc.x = self->m_acc.x + (1.2f * in->m_x);
        self->m_acc.z = self->m_acc.z + (1.2f * in->m_z);
        break;
    case 4:
        modelId = self->m_model->m_id;
        mot = *fn_1_179F4(&self->m_mot, 7);
        if (self->m_motion != 7) {
            CharMotionShiftSet(self->m_charNo, mot, 0.0f, 18.0f, 0);
            self->m_motion = 7;
        }
        self->m_i44 = 0x12;
        break;
    case 5:
        modelId = self->m_model->m_id;
        mot = *fn_1_179F4(&self->m_mot, 3);
        if (self->m_motion != 3) {
            CharMotionShiftSet(self->m_charNo, mot, 0.0f, 18.0f, 0x40000001);
            self->m_motion = 3;
        }
        break;
    case 6:
        self->m_vel.x = 0.0f;
        self->m_vel.y = 0.0f;
        self->m_vel.z = 0.0f;
        break;
    }
    self->m_state = state;
    self->m_i58 = 0;
}

void fn_1_16F98(CPlayer *self)
{
    CCtlInput *in;
    u8 expired;
    s16 modelId;
    s16 mot;

    self->m_vel.x = 0.0f;
    self->m_vel.y = 0.0f;
    self->m_vel.z = 0.0f;
    in = &self->m_ctl->m_in;
    if (in->m_btn != 0) {
        fn_1_16C54(self, 2);
        return;
    }
    if (in->m_mag >= 0.7f) {
        fn_1_16C54(self, 1);
        return;
    }
    expired = 0;
    if (self->m_i48 > 0) {
        if (--self->m_i48 == 0) {
            expired = 1;
        }
    }
    if (expired != 0) {
        modelId = self->m_model->m_id;
        mot = *fn_1_179F4(&self->m_mot, 0);
        if (self->m_motion != 0) {
            CharMotionShiftSet(self->m_charNo, mot, 0.0f, 18.0f, 0x40000001);
            self->m_motion = 0;
        }
    }
}

void fn_1_170F8(CPlayer *self)
{
    M521Vec normal;
    M521Vec dir;
    f32 dot;
    f32 absDot;
    CClock *clock;

    if (fn_1_17218(self) == 0) {
        if (self->m_slot < 2) {
            clock = lbl_1_bss_A50;
        } else {
            clock = lbl_1_bss_A54;
        }
        if (fn_1_1CA0(clock, (M521Vec *)&self->m_pos, &normal, &dot) >= 0) {
            fn_1_17578(self, &dir);
            dot = PSVECDotProduct(&normal, &dir);
            if (dot < 0.0f) {
                absDot = -dot;
            } else {
                absDot = dot;
            }
            if (absDot > 0.707f) {
                fn_1_16C54(self, 5);
            }
        }
    }
}

u8 fn_1_17218(CPlayer *self)
{
    CCtlInput *in;
    M521Vec dir;
    M521Vec input;
    f32 ang;
    f32 dot;

    in = &self->m_ctl->m_in;
    if (in->m_btn != 0) {
        fn_1_16C54(self, 2);
        return 1;
    }
    if (!(in->m_mag >= 0.7f)) {
        fn_1_16C54(self, 0);
        return 1;
    }
    ang = atan2(in->m_x, in->m_z) / 3.141592653589793;
    self->m_f64 = (f32)(180.0 * ang);
    fn_1_17578(self, &dir);
    input.x = in->m_x;
    input.y = 0.0f;
    input.z = in->m_z;
    dot = PSVECDotProduct(&dir, &input);
    if (dot > 0.0f) {
        PSVECScale(&dir, &self->m_vel, dot * (6.666667f * in->m_mag));
    }
    return 0;
}

void fn_1_173B8(CPlayer *self)
{
    if (fn_1_17218(self) == 0) {
    }
}

void fn_1_173EC(CPlayer *self)
{
    if (self->m_vel.y < 0.0f) {
        fn_1_16C54(self, 3);
    }
}

void fn_1_1743C(CPlayer *self)
{
    if ((self->m_pos.y - 30.000002f) <= lbl_1_data_680) {
        fn_1_16C54(self, 4);
    }
}

void fn_1_174B0(CPlayer *self)
{
    fn_1_168C0(self);
    if (self->m_i44 > 0) {
        if (--self->m_i44 == 0) {
        }
    }
    if (self->m_pos.y <= lbl_1_data_680) {
        self->m_vel.x = 0.0f;
        self->m_vel.y = 0.0f;
        self->m_vel.z = 0.0f;
        if (self->m_i44 <= 0) {
            fn_1_16C54(self, 0);
        }
    }
}

void fn_1_17578(CPlayer *self, M521Vec *out)
{
    f32 rad;
    f32 c;

    rad = (f32)((3.141592653589793 * (f64)self->m_f60) / 180.0);
    c = cos(rad);
    out->x = sin(rad);
    out->y = 0.0f;
    out->z = c;
}

void fn_1_1763C(CPlayer *self)
{
    CCtl *ctl;

    if (self->m_ctl != 0) {
        ctl = self->m_ctl;
        if (fn_1_6998() == 2) {
            fn_1_AF00(ctl);
        } else {
            ctl->m_in.m_x = 0.0f;
            ctl->m_in.m_z = 0.0f;
            ctl->m_in.m_mag = 0.0f;
            ctl->m_in.m_btn = 0;
        }
        switch (self->m_state) {
        case 0:
            fn_1_16F98(self);
            break;
        case 1:
            fn_1_170F8(self);
            break;
        case 2:
            fn_1_173EC(self);
            break;
        case 3:
            fn_1_1743C(self);
            break;
        case 4:
            fn_1_174B0(self);
            break;
        case 5:
            fn_1_173B8(self);
            break;
        case 6:
            fn_1_177A0(self);
            break;
        }
        self->m_i58++;
    }
}

u8 fn_1_17760(CPlayer *self)
{
    switch (self->m_state) {
    case 2:
    case 3:
    case 4:
        return 1;
    default:
        return 0;
    }
}

void fn_1_17788(CPlayer *self)
{
    self->m_req = 0;
}

void fn_1_17794(CPlayer *self)
{
    self->m_req = 1;
}

void fn_1_177A0(CPlayer *self)
{
    u8 expired;

    expired = 0;
    if (self->m_i40 > 0) {
        if (--self->m_i40 == 0) {
            expired = 1;
        }
    }
    if (CharMotionEndCheck(self->m_charNo) != 0 || expired != 0) {
        fn_1_16C54(self, 0);
    }
}

void fn_1_17840(CPlayer *self)
{
    fn_1_16C54(self, 0);
    self->m_f64 = 0.0f;
}

void fn_1_17884(CPlayer *self)
{
    s16 modelId;
    s16 mot;

    modelId = self->m_model->m_id;
    mot = *fn_1_179F4(&self->m_mot, 4);
    if (self->m_motion != 4) {
        CharMotionShiftSet(self->m_charNo, mot, 0.0f, 18.0f, 0);
        self->m_motion = 4;
    }
}

void fn_1_17920(CPlayer *self)
{
    s16 modelId;
    s16 mot;

    modelId = self->m_model->m_id;
    mot = *fn_1_179F4(&self->m_mot, 5);
    if (self->m_motion != 5) {
        CharMotionShiftSet(self->m_charNo, mot, 0.0f, 18.0f, 0);
        self->m_motion = 5;
        if (lbl_1_bss_A4C != 0) {
            CharWinLoseVoicePlay(self->m_charNo, 0x8C0031, 0x200);
        }
    }
}

void fn_1_179E8(CPlayer *self)
{
    self->m_flag51 = 1;
}

u16 *fn_1_179F4(CDeque16 *self, u32 idx)
{
    if (idx >= self->m_size) {
        fn_1_4C9C(PLAYER_STDERR, "vector::at index out of range");
        abort();
    }
    return self->m_data + idx;
}

// vector<u16>::insert(pos, n, value)
void fn_1_17A6C(CDeque16 *self, u16 *pos, u32 n, u16 *val)
{
    u32 maxSize;
    u16 *end;
    s32 tail;
    u16 *src;
    u32 i;
    u16 *dst;
    u16 *oldData;
    s32 offset;
    u32 newSize;
    u32 newCap;
    u16 *newData;

    if (n != 0) {
        maxSize = 0x7FFFFFFF;
        if (n > maxSize || self->m_size > maxSize - n) {
            fn_1_4C9C(PLAYER_STDERR, "vector::insert length error\n");
            abort();
        }
        if (self->m_size + n <= self->m_cap) {
            end = self->m_data + self->m_size;
            tail = end - pos;
            src = val;
            if (end - (tail - n) <= src && src < end) {
                src = src + n;
            }
            if ((u32)tail != 0) {
                memmove(end + n - tail, pos, tail * 2);
            }
            i = n;
            dst = pos;
            while (i != 0) {
                *dst = *src;
                dst++;
                i--;
            }
            self->m_size = self->m_size + n;
            return;
        }
        offset = pos - self->m_data;
        oldData = self->m_data;
        newSize = self->m_size + n;
        if (self->m_cap != 0) {
            newCap = self->m_cap;
        } else {
            newCap = 1;
        }
        while (newSize > newCap) {
            if (newCap < (maxSize >> 1)) {
                newCap = newCap * 2;
            } else {
                newCap = maxSize;
            }
        }
        newData = (u16 *)fn_1_4A68(newCap * 2);
        if (newData == 0) {
            fn_1_4C9C(PLAYER_STDERR, "Memory allocation failure");
            abort();
        }
        self->m_data = newData;
        i = n;
        dst = self->m_data + offset;
        while (i != 0) {
            *dst = *val;
            dst++;
            i--;
        }
        if (oldData != 0) {
            memcpy(self->m_data, oldData, offset * 2);
            memcpy(self->m_data + offset + n, oldData + offset, ((oldData + self->m_size) - (oldData + offset)) * 2);
            __dl__FPv(oldData);
        }
        self->m_size = newSize;
        self->m_cap = newCap;
    }
}

// vector<u16>::reserve
void fn_1_17E1C(CDeque16 *self, u32 cap)
{
    u16 *oldData;
    u32 maxSize;
    u16 *newData;

    if (cap > self->m_cap) {
        maxSize = 0x7FFFFFFF;
        if (cap > maxSize) {
            fn_1_4C9C(PLAYER_STDERR, "vector::reserve length error\n");
            abort();
        }
        oldData = self->m_data;
        newData = (u16 *)fn_1_4A68(cap * 2);
        if (newData == 0) {
            fn_1_4C9C(PLAYER_STDERR, "Memory allocation failure");
            abort();
        }
        self->m_data = newData;
        if (oldData != 0) {
            memcpy(self->m_data, oldData, ((oldData + self->m_size) - oldData) * 2);
            __dl__FPv(oldData);
        }
        self->m_cap = cap;
    }
}

void fn_1_17F44(void)
{
    lbl_1_bss_AA8 = 0x78;
    lbl_1_bss_AAC = 0xD2;
    lbl_1_bss_AB0 = 0xD2;
}
}
