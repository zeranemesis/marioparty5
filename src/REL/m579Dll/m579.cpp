#include "dolphin/types.h"

extern "C" {

extern s16 omSysExitReq;

typedef struct {
    s16 id;
} M579Hu3DRef;

typedef struct {
    u8 pad_00[0xF0];
    void *unk_F0;
    u8 pad_F4[0x2C];
    s32 unk_120;
    u8 pad_124[0x10];
} M579Hu3DModel;

extern M579Hu3DModel Hu3DData[];

typedef struct M579Node M579Node;

struct M579Node {
    M579Node *unk_00;
    M579Node *unk_04;
};

typedef struct {
    u8 pad_00[0x14];
    void *unk_14;
    u8 pad_18[0x44];
    void *unk_5C;
} M579Thing;

typedef struct {
    void *unk_00;
} M579VtblHolder;

typedef void *M579Vtbl[4];

typedef struct {
    u8 pad_00[0xC];
    f32 unk_0C[3];
    f32 unk_18[3];
} M579Cam;

typedef struct {
    void *unk_00;
} M579TimerRef;

typedef struct M579TimerState M579TimerState;

struct M579TimerState {
    s32 unk_00;
    u8 pad_04[0x10];
    s32 unk_14;
};

typedef struct {
    M579TimerState *unk_00;
} M579TimerStateRef;

extern void HuMemDirectFree(void *ptr);
extern void WipeCreate(s32 a, s32 b, s32 c);
extern s16 Hu3DModelCreate(u32 arg);
extern s16 Hu3DHookFuncCreate(u32 arg);
extern void Hu3DModelAttrSet(s16 id, u32 attr);
extern void Hu3DModelAttrReset(s16 id, u32 attr);
extern void Hu3DModelTPLvlSet(s16 id, f32 lvl);
extern void Hu3DModelLayerSet(s16 id, s16 layer);
extern void Hu3DModelCameraSet(s16 id, u16 cam);
extern void Hu3DModelShadowSet(s16 id);
extern void Hu3DModelShadowMapSet(s16 id);
extern void Hu3DModelShadowMapObjSet(s16 id, u32 obj);
extern void Hu3DModelShadowMapObjReset(s16 id);
extern s32 MgTimerValueGet(void *timer);
extern void MgTimerPosSet(void *timer, f32 x, f32 y);
extern void MgTimerPosGet(void *timer, void *x, void *y);
extern void MgTimerModeOffSet(void *timer);
extern void MgTimerKill(void *timer);
extern void *MgTimerCreate(s32 arg);
extern s32 MgTimerDoneCheck(void *timer);
extern void Hu3DCameraPosSetV(u16 cam, M579Cam *self, f32 *a, f32 *b);
extern void Hu3DCameraPosGet(u16 cam, M579Cam *self, f32 *a, f32 *b);
extern void C_MTXLookAt(void *mtx, void *eye, void *cen, void *up);
extern void PSMTXInverse(void *src, void *dst);
extern void PSVECAdd(void *a, void *b, void *c);
extern void *OSGetCurrentThread(void);
extern s32 OSGetThreadPriority(void *thread);
extern s32 OSIsThreadTerminated(void *thread);
extern s32 HuDataSelHeapReadNum(u32 dataNum, u32 heap, u32 sel);
extern void Hu3DModelPosSet(s16 id, f32 x, f32 y, f32 z);
extern void Hu3DModelRotSet(s16 id, f32 x, f32 y, f32 z);
extern void Hu3DModelScaleSet(s16 id, f32 x, f32 y, f32 z);
extern void MgTimerParamSet(void *timer, s32 maxTime, s32 endTime, s32 recordTime);
extern void MgTimerWinColorSet(void *timer, u8 r, u8 g, u8 b);
extern u8 lbl_1_bss_9E8;
extern u8 lbl_1_bss_14[];

typedef void (*M579VtblEntry)(void *obj, M579Thing *self);

typedef struct {
    M579VtblEntry *v;
} M579VtblObj;

void fn_1_4874(M579Thing *self);
void fn_1_48C0(M579Thing *self);
void fn_1_4934(M579Thing *self);

M579Vtbl lbl_1_data_1D4 = { 0, 0, (void *)fn_1_4934, (void *)fn_1_48C0 };

void fn_1_12DC(M579Node *self)
{
    self->unk_00->unk_04 = self->unk_04;
    self->unk_04->unk_00 = self->unk_00;
    self->unk_00 = self;
    self->unk_04 = self;
}

void fn_1_2C44(void)
{
    omSysExitReq = 1;
}

void fn_1_30BC(M579Hu3DRef *self, s32 val)
{
    Hu3DData[self->id].unk_120 = val;
}

void *fn_1_30D8(M579Hu3DRef *self)
{
    return &Hu3DData[self->id].unk_F0;
}

void fn_1_4854(M579VtblHolder *obj, M579Thing *self)
{
    obj->unk_00 = &lbl_1_data_1D4;
    self->unk_5C = obj;
    self->unk_14 = &fn_1_4874;
}

void fn_1_1DDC(void)
{
}

void fn_1_246C(void)
{
}

void fn_1_48C0(M579Thing *self)
{
}

void fn_1_1264(M579Node *self)
{
    self->unk_00 = self;
    self->unk_04 = self;
}

void fn_1_C8(u32 p)
{
    if (p != 0) {
        HuMemDirectFree((void *)p);
    }
}

void fn_1_100(u32 p)
{
    if (p != 0) {
        HuMemDirectFree((void *)p);
    }
}

void fn_1_1300(M579Node *self, M579Node *arg)
{
    M579Node *n = arg;

    self->unk_04 = n;
    self->unk_00 = n->unk_00;
    n->unk_00->unk_04 = self;
    n->unk_00 = self;
}

void fn_1_1330(M579Node *self, M579Node *arg)
{
    M579Node *n = arg;

    self->unk_00 = n;
    self->unk_04 = n->unk_04;
    n->unk_04->unk_00 = self;
    n->unk_04 = self;
}

void fn_1_2BEC(void)
{
    WipeCreate(1, 5, 0x3C);
}

void fn_1_2C18(void)
{
    WipeCreate(2, 6, 0x3C);
}

M579Hu3DRef *fn_1_2C58(M579Hu3DRef *self, u32 arg)
{
    self->id = Hu3DModelCreate(arg);
    return self;
}

M579Hu3DRef *fn_1_2CDC(M579Hu3DRef *self, u32 arg)
{
    self->id = Hu3DHookFuncCreate(arg);
    return self;
}

void fn_1_2E08(M579Hu3DRef *self, u32 attr)
{
    Hu3DModelAttrSet(self->id, attr);
}

void fn_1_2E40(M579Hu3DRef *self, u32 attr)
{
    Hu3DModelAttrReset(self->id, attr);
}

void fn_1_3014(M579Hu3DRef *self, f32 lvl)
{
    Hu3DModelTPLvlSet(self->id, lvl);
}

void fn_1_304C(M579Hu3DRef *self, s16 layer)
{
    Hu3DModelLayerSet(self->id, layer);
}

void fn_1_3084(M579Hu3DRef *self, u16 cam)
{
    Hu3DModelCameraSet(self->id, cam);
}

void fn_1_31E4(M579Hu3DRef *self)
{
    Hu3DModelShadowSet(self->id);
}

void fn_1_3214(M579Hu3DRef *self)
{
    Hu3DModelShadowMapSet(self->id);
}

void fn_1_3244(M579Hu3DRef *self, u32 obj)
{
    Hu3DModelShadowMapObjSet(self->id, obj);
}

void fn_1_327C(M579Hu3DRef *self)
{
    Hu3DModelShadowMapObjReset(self->id);
}

void fn_1_47E4(u32 p)
{
    if (p != 0) {
        HuMemDirectFree((void *)p);
    }
}

void fn_1_481C(u32 p)
{
    if (p != 0) {
        HuMemDirectFree((void *)p);
    }
}

void fn_1_4B0(M579Cam *self, void *mtx)
{
    C_MTXLookAt(mtx, self, self->unk_0C, self->unk_18);
    PSMTXInverse(mtx, mtx);
}

void fn_1_56C(M579Cam *self, void *out)
{
    PSVECAdd(out, self->unk_18, self->unk_18);
    PSVECAdd(out, self, self);
}

void fn_1_9A0(M579Cam *self, u16 cam)
{
    Hu3DCameraPosSetV(cam, self, self->unk_0C, self->unk_18);
}

void fn_1_9E0(M579Cam *self, u16 cam)
{
    Hu3DCameraPosGet(cam, self, self->unk_0C, self->unk_18);
}

s32 fn_1_5C0C(M579TimerRef *self)
{
    return MgTimerValueGet(self->unk_00);
}

void fn_1_5C3C(M579TimerRef *self, f32 x, f32 y)
{
    MgTimerPosSet(self->unk_00, x, y);
}

void fn_1_5C7C(M579TimerRef *self, void *x, void *y)
{
    MgTimerPosGet(self->unk_00, x, y);
}

void fn_1_5D80(M579TimerRef *self)
{
    MgTimerModeOffSet(self->unk_00);
}

void fn_1_5DB0(M579TimerStateRef *self)
{
    if (self->unk_00->unk_14 == 0) {
        self->unk_00->unk_14 = 1;
        self->unk_00->unk_00 = 1;
    }
}

void fn_1_5DDC(M579TimerRef *self)
{
    MgTimerKill(self->unk_00);
    self->unk_00 = MgTimerCreate(0);
}

u8 fn_1_5E18(M579TimerRef *self)
{
    return MgTimerDoneCheck(self->unk_00) != 0;
}

s32 fn_1_5244(void)
{
    return OSGetThreadPriority(OSGetCurrentThread());
}

M579Hu3DRef *fn_1_2C94(M579Hu3DRef *self, u32 arg)
{
    // NONMATCHING: 83.33%. target emits `lis r4,0x1000`; MWCC 1.3.2 always emits
    // `li r4,0x1000` for any literal that fits a signed 16-bit immediate
    // (HANDOVER section 16: the li/lis wall, unreachable from C).
    self->id = Hu3DModelCreate(HuDataSelHeapReadNum(arg, 0x1000, 2));
    return self;
}

void fn_1_2EF4(M579Hu3DRef *self, f32 *v)
{
    Hu3DModelPosSet(self->id, v[0], v[1], v[2]);
}

void fn_1_2F3C(M579Hu3DRef *self, f32 *v)
{
    Hu3DModelRotSet(self->id, v[0], v[1], v[2]);
}

void fn_1_2F84(M579Hu3DRef *self, f32 *v)
{
    Hu3DModelScaleSet(self->id, v[0], v[1], v[2]);
}

void fn_1_2FCC(M579Hu3DRef *self, f32 s)
{
    Hu3DModelScaleSet(self->id, s, s, s);
}

u8 fn_1_5350(void *self)
{
    void *th = (u8 *)self + 8;

    return OSIsThreadTerminated(th) != 0;
}

void fn_1_5BC4(M579TimerRef *self, s32 a, s32 b, s32 c)
{
    MgTimerParamSet(self->unk_00, a, b, c);
}

void fn_1_5E58(M579TimerRef *self, u8 r, u8 g, u8 b)
{
    MgTimerWinColorSet(self->unk_00, r, g, b);
}

void fn_1_4874(M579Thing *self)
{
    M579VtblObj *obj = (M579VtblObj *)self->unk_5C;

    obj->v[3](self->unk_5C, self);
}

void fn_1_5A54(u32 *p, s32 count)
{
    u32 *end;

    if (lbl_1_bss_9E8) {
        end = p + count;
        for (; p != end; p++) {
            *p = 0xDEADBABE;
        }
    }
}

void *fn_1_1F30(void)
{
    return *(void **)lbl_1_bss_14;
}

}