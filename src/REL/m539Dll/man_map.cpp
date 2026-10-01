#include "REL/m539Dll.h"

/*
 * m539's "man_map": the map backdrop.
 *
 * It owns two scene lights, a pair of shadow-casting lights built from two
 * four-entry tables of positions, and a single sprite group that fades out
 * whenever the player leaves the stage.
 */

extern "C" {

typedef struct M539Fade_s {
    s32 frames;
    s32 cur;
    f32 from;
    f32 hold;
    f32 to;
} M539Fade;

typedef struct {
    s32 ptmfA;
    s32 ptmfB;
    void *ptmfFn;
} PTMF;

typedef struct M539ManMap_s {
    s32 unk0;
    s32 unk4;
    void *vtable;
    s32 flags;
    s16 *shadowLight;
    s16 *infLight;
    s16 *sprLight[2];
    PTMF fadeHook;
    s16 sprGrp;
    M539Fade fade;
} M539ManMap;

typedef struct M539SprLight_s {
    void *unk0;
    s16 *unk4;
} M539SprLight;

extern void *lbl_1_data_1FC;
extern s32 lbl_1_bss_C;
extern void *lbl_1_bss_A10;

void *lbl_1_rodata_1C0[12];
void *lbl_1_rodata_1F0[12];
s32 lbl_1_rodata_220[3];
f32 lbl_1_rodata_22C;
f32 lbl_1_rodata_230;
f32 lbl_1_rodata_234;
f32 lbl_1_rodata_238;
f32 lbl_1_rodata_23C;
f32 lbl_1_rodata_240;
f32 lbl_1_rodata_244;
double lbl_1_rodata_248;
double lbl_1_rodata_250;

char lbl_1_data_3B0[] = "ms_pointer != 0";
char lbl_1_data_3C0[] = "singleton.h";
s32 lbl_1_data_3CC[3] = {0, -1, 0};
char lbl_1_data_3D8[] = "ms_pointer == 0";
char lbl_1_data_3E8[] = "lights > 0";
char lbl_1_data_3F3[] = "man_map.cpp";
char lbl_1_data_3FF[] = "inf_light1";
char lbl_1_data_40A[] = "light_id >= 0";
void *lbl_1_data_418[6];

s32 lbl_1_bss_A38;
s32 lbl_1_bss_A3C;
s32 lbl_1_bss_A40;

void *__construct_array(void *ptr, void *ctor, void *dtor, u32 size, u32 n);
void __destroy_arr(void *block, void *dtor, u32 size, u32 n);
s32 __ptmf_test(const PTMF *ptmf);
void __ptmf_scall(void *obj, PTMF *ptmf);

void *HuDataSelHeapReadNum(u32 dataNum, u32 num, s32 heap);
void *HuSprAnimRead(void *data);
s32 HuSprGrpCreate(s32 count);
void HuSprGrpMemberSet(s32 grp, s32 idx, s32 member);
s32 HuSprCreate(void *anim, u16 attr, s16 n);
void HuSprTPLvlSet(s32 grp, s32 member, f32 level);
void HuSprPosSet(s32 grp, s32 member, f32 x, f32 y);
void Hu3DModelShadowMapSet(s16 modelId);
void Hu3DModelShadowDispOn(s16 modelId);
void Hu3DModelLayerSet(s16 modelId, s16 layerNo);
s32 Hu3DModelLightInfoSet(s16 modelId, s32 count);
s16 Hu3DModelLightIdGet(s16 modelId, const char *name);
void Hu3DGLightParamGet(s16 lightId, Point3d *pos, Point3d *target, s32 *color);
void Hu3DShadowMultiTPLvlSet(f32 level, s16 lightId);
void Hu3DShadowMultiColSet(u8 r, u8 g, u8 b, u8 a);

void fn_1_1264(void *self);
void fn_1_1270(void *self, s32 arg);
void fn_1_1330(void *self, s32 *arg);
void fn_1_1D4(const char *msg, const char *file, s32 line);
s16 *fn_1_0(s32 arg);
s16 *fn_1_2C94(s16 *self, s32 dataNum);
void fn_1_30F4(s32 ptr, s32 flag);
void fn_1_50C4(s32 lightId, Point3d *target, s32 *color, f32 *param, f32 a, f32 b);
void *fn_1_9DF8(void *self, Point3d *start, s32 count);
void fn_1_9F6C(void *self);

s16 **fn_1_C408(s16 **arg0, s16 arg1);
void fn_1_C488(s16 **arg0);
void fn_1_C748(s16 **arg0, s32 arg1);
void fn_1_C750(M539Fade *self, s32 frames, f32 level);
void fn_1_C794(M539Fade *self, s32 frames, f32 from, f32 to);
f32 fn_1_C7C8(M539Fade *self);
void fn_1_CBAC(void *self, M539Fade *fade);
void fn_1_C540(void);

// NONMATCHING: register allocation only.
void *fn_1_BEBC(void)
{
    if (lbl_1_bss_A10 == NULL) {
        fn_1_1D4(lbl_1_data_3B0, lbl_1_data_3C0, 0x27);
    }
    return lbl_1_bss_A10;
}

// NONMATCHING: register allocation only.
void fn_1_BF2C(s16 modelId)
{
    Hu3DModelShadowMapSet(modelId);
    Hu3DModelShadowDispOn(modelId);
    Hu3DModelLayerSet(modelId, 0);
}

// NONMATCHING: register allocation only.
M539ManMap *fn_1_BF70(M539ManMap *self, s32 arg)
{
    Vec pos;
    Vec target;
    s32 color[4];
    s32 sp50;
    s32 sp54;
    s32 sp58;
    void *anim;
    s16 *p;
    s16 *q;
    s16 *sprLight;

    fn_1_1264(self);
    self->vtable = &lbl_1_data_1FC;
    self->flags = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    if (lbl_1_bss_A10 == NULL) {
        fn_1_1D4(lbl_1_data_3D8, lbl_1_data_3C0, 0x35);
    }
    lbl_1_bss_A10 = self;
    self->vtable = lbl_1_data_418;
    p = fn_1_0(2);
    if (p != NULL) {
        p = fn_1_2C94(p, 0x610001);
    }
    self->shadowLight = p;
    q = fn_1_0(2);
    if (q != NULL) {
        q = fn_1_2C94(q, 0x610000);
    }
    self->infLight = q;
    __construct_array(&self->sprLight[0], (void *)fn_1_C488, (void *)fn_1_C408, 4, 2);
    fn_1_C750(&self->fade, 0, 0.0f);
    if (Hu3DModelLightInfoSet(*self->infLight, 1) <= 0) {
        fn_1_1D4(lbl_1_data_3E8, lbl_1_data_3F3, 0x2A);
    }
    p = (s16 *)fn_1_0(0x3C);
    if (p != NULL) {
        p = (s16 *)fn_1_9DF8(p, (Point3d *)lbl_1_rodata_1C0, 4);
    }
    if (self->sprLight[0] != p) {
        sprLight = self->sprLight[0];
        if (sprLight != NULL) {
            if ((u32)(sprLight + 0x30) != 0U) {
                fn_1_30F4(*(s32 *)((char *)sprLight + 0x30), 1);
            }
            fn_1_C8(sprLight);
        }
        self->sprLight[0] = p;
    }
    q = (s16 *)fn_1_0(0x3C);
    if (q != NULL) {
        q = (s16 *)fn_1_9DF8(q, (Point3d *)lbl_1_rodata_1F0, 4);
    }
    if (self->sprLight[1] != q) {
        sprLight = self->sprLight[1];
        if (sprLight != NULL) {
            if ((u32)(sprLight + 0x30) != 0U) {
                fn_1_30F4(*(s32 *)((char *)sprLight + 0x30), 1);
            }
            fn_1_C8(sprLight);
        }
        self->sprLight[1] = q;
    }
    fn_1_BF2C(*self->shadowLight);
    if (Hu3DModelLightIdGet(*self->infLight, lbl_1_data_3FF) < 0) {
        fn_1_1D4(lbl_1_data_40A, lbl_1_data_3F3, 0x44);
    }
    Hu3DGLightParamGet(*self->infLight, &pos, &target, color);
    sp50 = lbl_1_rodata_220[0];
    sp54 = lbl_1_rodata_220[1];
    sp58 = lbl_1_rodata_220[2];
    fn_1_50C4(0xF, &target, &sp50, (f32 *)&lbl_1_rodata_234, 600.0f, 530.0f);
    Hu3DShadowMultiTPLvlSet(0.5f, 0xF);
    Hu3DShadowMultiColSet(0x20, 0x20, 0x20, 0xF);
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x61000B, 0x10000000, 2));
    self->sprGrp = HuSprGrpCreate(1);
    HuSprGrpMemberSet(self->sprGrp, 0, HuSprCreate(anim, 0x190, 0));
    HuSprTPLvlSet(self->sprGrp, 0, 0.0f);
    HuSprPosSet(self->sprGrp, 0, 288.0f, 240.0f);
    self->fadeHook.ptmfA = lbl_1_data_3CC[0];
    self->fadeHook.ptmfB = lbl_1_data_3CC[1];
    self->fadeHook.ptmfFn = (void *)fn_1_C540;
    fn_1_2E08(self->infLight, 0x40000001);
    return self;
}

// NONMATCHING: register allocation only.
s16 **fn_1_C408(s16 **arg0, s16 arg1)
{
    s16 count = arg1;
    s16 *elem;

    if (arg0 != NULL) {
        elem = *arg0;
        if (elem != NULL) {
            if ((u32)(elem + 0x30) != 0U) {
                fn_1_30F4(*(s32 *)((char *)elem + 0x30), 1);
            }
            fn_1_C8(elem);
        }
        if (count > 0) {
            fn_1_C8(arg0);
        }
    }
    return arg0;
}

// NONMATCHING: register allocation only.
void fn_1_C488(s16 **arg0)
{
    fn_1_C748(arg0, 0);
}

// NONMATCHING: register allocation only.
M539ManMap *fn_1_C4B4(M539ManMap *self, s16 arg)
{
    s16 mode = arg;

    if (self != NULL) {
        if (lbl_1_bss_A10 == NULL) {
            fn_1_1D4(lbl_1_data_3B0, lbl_1_data_3C0, 0x3D);
        }
        lbl_1_bss_A10 = NULL;
        if (mode > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_C540(void)
{
}

// NONMATCHING: register allocation only.
void fn_1_C544(M539ManMap *self)
{
    if (__ptmf_test(&self->fadeHook) != 0) {
        __ptmf_scall(self, &self->fadeHook);
    }
    HuSprTPLvlSet(self->sprGrp, 0, fn_1_C7C8(&self->fade));
    fn_1_9F6C(self->sprLight[0]);
    fn_1_9F6C(self->sprLight[1]);
}

// NONMATCHING: register allocation only.
void fn_1_C5CC(M539ManMap *self, s32 arg)
{
    if (arg == 1) {
        fn_1_C750(&self->fade, 0x30, 1.0f);
    }
    self->flags = self->flags & ~(1 << arg);
}

// NONMATCHING: register allocation only.
M539ManMap *fn_1_C634(M539ManMap *self, s16 arg)
{
    s16 count = arg;

    if (self != NULL) {
        self->vtable = lbl_1_data_418;
        __destroy_arr(&self->sprLight[0], (void *)fn_1_C408, 4, 2);
        if (self->infLight != NULL) {
            fn_1_30F4((s32)self->infLight, 1);
        }
        if (self->shadowLight != NULL) {
            fn_1_30F4((s32)self->shadowLight, 1);
        }
        if (self->shadowLight != NULL) {
            if (lbl_1_bss_A10 == NULL) {
                fn_1_1D4(lbl_1_data_3B0, lbl_1_data_3C0, 0x3D);
            }
            lbl_1_bss_A10 = NULL;
        }
        if (self != NULL) {
            self->vtable = &lbl_1_data_1FC;
            fn_1_1270(self, 0);
        }
        if (count > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_C748(s16 **arg0, s32 arg1)
{
    *arg0 = (s16 *)arg1;
}

// NONMATCHING: register allocation only.
void fn_1_C750(M539Fade *self, s32 frames, f32 level)
{
    f32 cur = level;
    s32 f = frames;

    fn_1_C794(self, f, self->from, cur);
}

void fn_1_C794(M539Fade *self, s32 frames, f32 from, f32 to)
{
    self->frames = frames;
    self->cur = 0;
    if (frames == 0) {
        self->from = to;
        self->hold = to;
        self->to = to;
        return;
    }
    self->from = from;
    self->hold = from;
    self->to = to;
}

// NONMATCHING: register allocation only - the double division of the fade
// weight is done in a different order.
f32 fn_1_C7C8(M539Fade *self)
{
    f32 value = self->from;
    f64 weight;
    s32 step;

    if (self->cur <= self->frames) {
        step = self->cur + 1;
        self->cur = step;
        weight = (f64)step / (f64)(self->frames + 1);
        value = (f32)(((f64)self->hold * (1.0 - weight)) + ((f64)self->to * weight));
        self->from = value;
    }
    return value;
}

// NONMATCHING: register allocation only.
void fn_1_C8A8(void)
{
    lbl_1_bss_A38 = 0x78;
    lbl_1_bss_A3C = 0xD2;
    lbl_1_bss_A40 = 0xD2;
}

}
