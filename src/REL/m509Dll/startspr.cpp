#include "game/data.h"
#include "game/sprite.h"

#ifdef __MWERKS__
#define XX_RODATA __declspec(section ".rodata")
#else
#define XX_RODATA
#endif

extern "C" {
void fn_1_0(const char *cond, const char *file, s32 line);
void fn_1_21C(void *self);
void fn_1_E48(void *self);
void fn_1_E54(void *self, s32 flag);
void fn_1_F14(void *self, void *base);
extern u8 lbl_1_bss_1C[];
extern u8 lbl_1_data_1C4[];
}

typedef struct {
    s16 grp;
    f32 x;
    f32 y;
} StartSprEntry;

typedef struct StartSpr {
    void *base[2];
    void **vtbl;
    u32 flags;
    u8 fadeOut;
    s32 timer;
    f32 alpha;
    StartSprEntry entry[4];
} StartSpr;

extern "C" {
void *fn_1_C924(StartSpr *self, s32 *icons);
void *fn_1_CB78(StartSpr *self, s16 flag);
void *fn_1_CC04(StartSpr *self, s16 flag);
void fn_1_CD08(StartSpr *self);
void fn_1_CE80(StartSpr *self, s32 bit);
}

s32 lbl_1_data_3308[4] = { 0x430006, 0x430007, 0x430008, 0x430009 };
s32 lbl_1_data_3318[15] = {
    0x43000A, 0x43000B, 0x43000C, 0x43000D, 0x43000E, 0x43000F, 0x430010, 0x430011,
    0x430012, 0x430013, 0x430014, 0x430015, 0x430016, 0x430014, 0x430014,
};
char lbl_1_data_3354[] = "ms_pointer == 0";
char lbl_1_data_3364[] = "singleton.h";
char lbl_1_data_3370[] = "ms_pointer != 0";
void *lbl_1_data_3380[6] = { 0, 0, (void *)fn_1_CC04, (void *)fn_1_CE80, (void *)fn_1_CD08, 0 };

void *lbl_1_bss_50[2];
#define ms_pointer (*(void **)lbl_1_bss_50)

typedef struct {
    f32 x;
    f32 y;
} StartSprPos;
XX_RODATA const StartSprPos lbl_1_rodata_230[4] = { { 118.0f, 126.0f }, { 460.0f, 126.0f }, { 118.0f, 384.0f }, { 460.0f, 384.0f } };

void *fn_1_C924(StartSpr *self, s32 *icons)
{
    ANIMDATA *anim1[4];
    ANIMDATA *anim2[4];
    StartSpr *p = self;
    StartSpr *p2 = p;
    s32 i;
    s32 j;
    s32 k;
    s16 grp;
    StartSprEntry *e;
    u8 *q;

    fn_1_E48(p2);
    p->vtbl = (void **)lbl_1_data_1C4;
    p->flags = 0;
    fn_1_F14(p, lbl_1_bss_1C);
    ms_pointer == 0 ? (void)0 : fn_1_0(lbl_1_data_3354, lbl_1_data_3364, 0x35);
    q = (u8 *)self + 0x10;
    if (q != 0) {
        q -= 0x10;
    }
    ms_pointer = q;
    self->vtbl = lbl_1_data_3380;
    self->fadeOut = 0;
    self->timer = 0;
    self->alpha = 0.0f;
    for (i = 0; i < 4; i++) {
        anim1[i] = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_3308[i], HU_MEMNUM_OVL, HEAP_MODEL));
        anim2[i] = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_3318[icons[i]], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    for (j = 0; j < 4; j++) {
        e = &self->entry[j];
        e->x = lbl_1_rodata_230[j].x;
        e->y = lbl_1_rodata_230[j].y;
        grp = HuSprGrpCreate(2);
        e->grp = grp;
        HuSprGrpMemberSet(grp, 0, HuSprCreate(anim2[j], 200, 0));
        HuSprGrpMemberSet(grp, 1, HuSprCreate(anim1[j], 100, 0));
        HuSprGrpPosSet(e->grp, e->x, e->y);
        for (k = 0; k < 2; k++) {
            HuSprAttrSet(grp, k, 1);
            HuSprAttrSet(grp, k, 4);
        }
    }
    return self;
}

void *fn_1_CB78(StartSpr *self, s16 flag)
{
    if (self != 0) {
        ms_pointer != 0 ? (void)0 : fn_1_0(lbl_1_data_3370, lbl_1_data_3364, 0x3D);
        ms_pointer = 0;
        if (flag > 0) {
            fn_1_21C(self);
        }
    }
    return self;
}

void *fn_1_CC04(StartSpr *self, s16 flag)
{
    s32 i;
    StartSprEntry *e;

    if (self != 0) {
        self->vtbl = lbl_1_data_3380;
        for (i = 0; i < 4; i++) {
            e = &self->entry[i];
            HuSprGrpKill(e->grp);
        }
        if ((u8 *)self + 0x10 != 0) {
            ms_pointer != 0 ? (void)0 : fn_1_0(lbl_1_data_3370, lbl_1_data_3364, 0x3D);
            ms_pointer = 0;
        }
        if (self != 0) {
            self->vtbl = (void **)lbl_1_data_1C4;
            if (self != 0) {
                fn_1_E54(self, 0);
            }
        }
        if (flag > 0) {
            fn_1_21C(self);
        }
    }
    return self;
}

void fn_1_CD08(StartSpr *self)
{
    s16 j;
    s32 i;
    StartSprEntry *e;
    StartSpr *t;

    self->timer = self->timer + 1;
    if (!((f32)self->timer < 60.0f)) {
        if (self->fadeOut == 0) {
            self->alpha = self->alpha + 0.033333335f;
            if (self->alpha > 1.0) {
                self->alpha = 1.0f;
            }
        } else {
            self->alpha = self->alpha - 0.033333335f;
            if (self->alpha < 0.0f) {
                t = self;
                if (t != 0) {
                    t = ((StartSpr *(*)(StartSpr *, s32))t->vtbl[2])(t, 1);
                }
                return;
            }
        }
        for (i = 0; i < 4; i++) {
            e = &self->entry[i];
            for (j = 0; j < 2; j++) {
                HuSprTPLvlSet(e->grp, j, self->alpha);
                HuSprAttrReset(e->grp, j, 4);
            }
        }
    }
}

void fn_1_CE80(StartSpr *self, s32 bit)
{
    self->flags &= ~(1 << bit);
    switch (bit) {
    case 1:
        self->fadeOut = 1;
        return;
    case 2:
        self->fadeOut = 1;
        return;
    case 0:
    case 5:
        break;
    }
}
