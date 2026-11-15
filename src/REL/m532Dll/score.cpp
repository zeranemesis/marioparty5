#define _MATH_H
extern "C" {
#include "dolphin.h"
#include "game/data.h"
#include "game/memory.h"
#include "game/sprite.h"
#include "game/gamemes.h"
}
#include "REL/m532Dll/score.h"

/*
 * m532 "score": the two score boxes (one per team) drawn as 2D quads over
 * a score window. Each box shows up to three digits that pop in with a scale
 * fade; a shared fade drives the whole object's appearance.
 */

class M532ScoreConst {
public:
    M532ScoreConst(s32 v) { m_v = v; }
    s32 m_v;
};

extern "C" {

extern void *lbl_1_data_B4[];
extern M532ScoreNode lbl_1_bss_24;
extern M532Score *lbl_1_bss_A28;
extern M532ScoreConst lbl_1_bss_A58;
extern M532ScoreConst lbl_1_bss_A5C;
extern M532ScoreConst lbl_1_bss_A60;
extern M532ScoreConst lbl_1_bss_A64;

void fn_1_1C10(void *ptr);
void fn_1_1D1C(const char *expr, const char *file, s32 line);
void fn_1_32B0(M532ScoreNode *node);
void fn_1_32BC(M532ScoreNode *node, s32 flag);
void fn_1_337C(M532ScoreNode *node, M532ScoreNode *list);
void GXSetVtxAttrFmtv(GXVtxFmt fmt, GXVtxAttrFmtList *list);
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, s32 wrapS, s32 wrapT, s32 filter);
#ifdef __MWERKS__
void *__construct_array(void *ptr, void *ctor, void *dtor, u32 size, u32 n);
#else
static void *__construct_array(void *ptr, void *ctor, void *dtor, u32 size, u32 n)
{
    u8 *p = (u8 *)ptr;

    (void)dtor;
    while (n-- != 0) {
        ((void *(*)(void *))ctor)(p);
        p += size;
    }
    return ptr;
}
#endif

void *fn_1_CD18(M532ScoreBox *box);
void fn_1_D178(void);
void fn_1_D1EC(M532Score *self);
void fn_1_D238(void);
void fn_1_D71C(M532ScoreBox *box, u8 r, u8 g, u8 b, f32 x, f32 y);
void fn_1_D924(M532ScoreBox *box, ANIMDATA *small, ANIMDATA *big);
void fn_1_DAA4(M532ScoreBox *box, f32 alpha);
void fn_1_DB80(M532ScoreBox *box);
s32 fn_1_DB9C(M532ScoreBox *box);
void fn_1_DBA4(M532ScoreBox *box);
void fn_1_DC78(M532ScoreBox *box);
void fn_1_DDDC(M532ScoreBox *box);
f32 fn_1_E3B0(M532ScoreFade *fade);
void fn_1_E490(M532ScoreFade *fade, s32 total, f32 to);
void fn_1_E4D4(M532ScoreFade *fade, s32 total, f32 from, f32 to);
void *fn_1_D048(M532Score *self, s32 idx);
void fn_1_D0DC(M532Score *self);

char lbl_1_data_610[] = "ms_pointer == 0";
char lbl_1_data_620[] = "singleton.h";
char lbl_1_data_62C[] = "ms_pointer != 0";
void *lbl_1_data_63C[5] = { 0, 0, (void *)fn_1_CDE8, (void *)fn_1_D048, (void *)fn_1_D0DC };

const s32 lbl_1_rodata_310[3] = { 0x5A0015, 0x5A0016, 0x5A0017 };
const s32 lbl_1_rodata_32C[6] = { GX_VA_POS, GX_DIRECT, GX_VA_TEX0, GX_DIRECT, GX_VA_NULL, 0 };
const s32 lbl_1_rodata_344[13] = { GX_VA_POS, GX_POS_XY, GX_F32, 0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0, GX_VA_NULL, 0, 0, 0, 0 };
const u8 lbl_1_rodata_388[4] = { 0, 0, 0, 0 };

// NONMATCHING: stack spills / inlining / register allocation differ
M532Score *fn_1_CB30(M532Score *self)
{
    s32 i;
    s16 grp;
    s16 spr;

    fn_1_32B0(&self->link);
    self->vtbl = lbl_1_data_B4;
    self->mask = 0;
    fn_1_337C(&self->link, &lbl_1_bss_24);
    if (lbl_1_bss_A28 != NULL) {
        fn_1_1D1C(lbl_1_data_610, lbl_1_data_620, 0x35);
    }
    lbl_1_bss_A28 = self;
    self->vtbl = lbl_1_data_63C;
    self->ready = 0;
    fn_1_E490(&self->fade, 0, 0.0f);
    __construct_array(self->box, (void *)fn_1_CD18, 0, sizeof(M532ScoreBox), 2);
    for (i = 0; i < 3; i++) {
        self->anim[i] = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_rodata_310[i], HU_MEMNUM_OVL, (HEAPID)2));
    }
    grp = HuSprGrpCreate(1);
    spr = HuSprFuncCreate((HUSPRFUNC)fn_1_D178, 0x63);
    HuSprGrpMemberSet(grp, 0, spr);
    fn_1_D71C(&self->box[0], 250, 0, 30, 60.0f, 60.0f);
    fn_1_D71C(&self->box[1], 0, 50, 250, 516.0f, 60.0f);
    fn_1_D924(&self->box[0], self->anim[1], self->anim[0]);
    fn_1_D924(&self->box[1], self->anim[2], self->anim[0]);
    return self;
}

void *fn_1_CD18(M532ScoreBox *box)
{
    fn_1_E490(&box->fade, 0, 0.0f);
    return box;
}

// Singleton base destructor.
// NONMATCHING: stack spills / inlining / register allocation differ
void *fn_1_CD5C(void *self, s16 flag)
{
    s16 f = flag;

    if (self != NULL) {
        if (lbl_1_bss_A28 == NULL) {
            fn_1_1D1C(lbl_1_data_62C, lbl_1_data_620, 0x3D);
        }
        lbl_1_bss_A28 = 0;
        if (f > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

// NONMATCHING: stack spills / inlining / register allocation differ
void *fn_1_CDE8(M532Score *self, s16 flag)
{
    s16 f = flag;

    if (self != NULL) {
        self->vtbl = lbl_1_data_63C;
        if (lbl_1_bss_A28 == NULL) {
            fn_1_1D1C(lbl_1_data_62C, lbl_1_data_620, 0x3D);
        }
        lbl_1_bss_A28 = 0;
        self->vtbl = lbl_1_data_B4;
        fn_1_32BC(&self->link, 0);
        if (f > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

void fn_1_CEB4(M532Score *self, s32 idx)
{
    fn_1_DB80(&self->box[idx]);
}

s32 fn_1_CEF0(M532Score *self, s32 idx)
{
    return fn_1_DB9C(&self->box[idx]);
}

void fn_1_CF2C(M532Score *self)
{
    fn_1_DBA4(&self->box[0]);
    fn_1_DBA4(&self->box[1]);
}

s32 fn_1_CF64(M532Score *self, s32 round)
{
    s32 a = fn_1_DB9C(&self->box[0]);
    s32 b = fn_1_DB9C(&self->box[1]);

    if (a >= 3 && b < 3) {
        return 0;
    }
    if (a < 3 && b >= 3) {
        return 1;
    }
    if (a >= 3 && b >= 3) {
        return 2;
    }
    if (round < 5) {
        return -1;
    }
    if (a > b) {
        return 0;
    }
    if (a < b) {
        return 1;
    }
    if (a <= 0) {
        return 3;
    }
    return 2;
}

// NONMATCHING: stack spills / inlining / register allocation differ
void *fn_1_D048(M532Score *self, s32 idx)
{
    if (idx != 2) {
        self->mask &= ~(1 << idx);
        return self;
    }
    self->ready = 1;
    fn_1_E490(&self->fade, lbl_1_bss_A64.m_v, 1.0f);
    self->mask &= ~(1 << idx);
    return self;
}

void fn_1_D0DC(M532Score *self)
{
    f32 val;

    if (self->ready != 0) {
        if (self->fade.cur <= self->fade.total) {
            val = self->fade.val;
            fn_1_DAA4(&self->box[0], val);
            fn_1_DAA4(&self->box[1], val);
            fn_1_E3B0(&self->fade);
        }
        fn_1_DC78(&self->box[0]);
        fn_1_DC78(&self->box[1]);
    }
}

void fn_1_D178(void)
{
    if (lbl_1_bss_A28 == NULL) {
        fn_1_1D1C(lbl_1_data_62C, lbl_1_data_620, 0x27);
    }
    fn_1_D1EC(lbl_1_bss_A28);
}

void fn_1_D1EC(M532Score *self)
{
    if (self->ready != 0) {
        fn_1_D238();
        fn_1_DDDC(&self->box[0]);
        fn_1_DDDC(&self->box[1]);
    }
}

// Set up GX for flat textured 2D quads.
void fn_1_D238(void)
{
    Mtx m;

    PSMTXIdentity(m);
    GXLoadPosMtxImm(m, 0);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C1);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A1, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetCullMode(GX_CULL_NONE);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0xFF);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXClearVtxDesc();
    GXSetVtxDescv((GXVtxDescList *)lbl_1_rodata_32C);
#ifdef TARGET_PC
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
#else
    GXSetVtxAttrFmtv(GX_VTXFMT0, (GXVtxAttrFmtList *)lbl_1_rodata_344);
#endif
}

void fn_1_D398(M532ScoreQuad *quad, ANIMDATA *tex)
{
    quad->tex = tex;
    quad->halfW = (f32)((f64)tex->bmp->sizeX * 0.5);
    quad->halfH = (f32)((f64)tex->bmp->sizeY * 0.5);
}

void fn_1_D430(M532ScoreQuad *quad, f32 x, f32 y)
{
    quad->x = x;
    quad->y = y;
}

void fn_1_D43C(M532ScoreQuad *quad, f32 scale)
{
    quad->scale = scale;
}

void fn_1_D444(M532ScoreQuad *quad, f32 alpha)
{
    f32 a;

    if (alpha < 0.0f) {
        a = 0.0f;
    } else if (alpha > 1.0f) {
        a = 1.0f;
    } else {
        a = alpha;
    }
    quad->alpha = a;
}

void fn_1_D4B0(M532ScoreQuad *quad)
{
    GXColor col;
    f32 w, h, x0, x1, y0, y1;

    if (quad->alpha > 0.0f) {
        HuSprTexLoad(quad->tex, 0, 0, 0, 0, 1);
        col.r = lbl_1_rodata_388[0];
        col.g = lbl_1_rodata_388[1];
        col.b = lbl_1_rodata_388[2];
        col.a = (u8)(255.0f * quad->alpha);
        GXSetTevColor(GX_TEVREG0, col);
        w = quad->scale * quad->halfW;
        h = quad->scale * quad->halfH;
        x0 = quad->x - w;
        x1 = quad->x + w;
        y0 = quad->y - h;
        y1 = quad->y + h;
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXWGFifo.f32 = x0;
        GXWGFifo.f32 = y0;
        GXWGFifo.f32 = 0.0f;
        GXWGFifo.f32 = 0.0f;
        GXWGFifo.f32 = x1;
        GXWGFifo.f32 = y0;
        GXWGFifo.f32 = 1.0f;
        GXWGFifo.f32 = 0.0f;
        GXWGFifo.f32 = x1;
        GXWGFifo.f32 = y1;
        GXWGFifo.f32 = 1.0f;
        GXWGFifo.f32 = 1.0f;
        GXWGFifo.f32 = x0;
        GXWGFifo.f32 = y1;
        GXWGFifo.f32 = 0.0f;
        GXWGFifo.f32 = 1.0f;
    }
}

void fn_1_D71C(M532ScoreBox *box, u8 r, u8 g, u8 b, f32 x, f32 y)
{
    s32 i;
    f32 px;

    box->win = MgScoreWinCreate(0x58, 0x28);
    MgScoreWinPosSet(box->win, x, y);
    MgScoreWinColorSet(box->win, r, g, b);
    MgScoreWinTPLvlSet(box->win, 0.0f);
    MgScoreWinDispSet(box->win, 1);
    for (i = 0; i < 3; i++) {
        px = (f32)(x + 24.0 * ((f64)i - 1.0));
        box->small[i].x = px;
        box->small[i].y = y;
        box->small[i].alpha = 0.0f;
        box->small[i].scale = 1.0f;
        box->big[i].x = px;
        box->big[i].y = y;
        box->big[i].alpha = 0.0f;
        box->big[i].scale = 3.0f;
    }
    box->count = 0;
    box->maxCount = 0;
}

void fn_1_D924(M532ScoreBox *box, ANIMDATA *small, ANIMDATA *big)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        box->small[i].tex = small;
        box->small[i].halfW = (f32)((f64)small->bmp->sizeX * 0.5);
        box->small[i].halfH = (f32)((f64)small->bmp->sizeY * 0.5);
        box->big[i].tex = big;
        box->big[i].halfW = (f32)((f64)big->bmp->sizeX * 0.5);
        box->big[i].halfH = (f32)((f64)big->bmp->sizeY * 0.5);
    }
}

void fn_1_DAA4(M532ScoreBox *box, f32 alpha)
{
    f32 a;
    s32 i;

    MgScoreWinTPLvlSet(box->win, 0.7f * alpha);
    for (i = 0; i < 3; i++) {
        if (alpha < 0.0f) {
            a = 0.0f;
        } else if (alpha > 1.0f) {
            a = 1.0f;
        } else {
            a = alpha;
        }
        box->small[i].alpha = a;
    }
}

void fn_1_DB80(M532ScoreBox *box)
{
    if (box->count < 3) {
        box->count = box->count + 1;
    }
}

s32 fn_1_DB9C(M532ScoreBox *box)
{
    return box->count;
}

void fn_1_DBA4(M532ScoreBox *box)
{
    s32 i;

    if (box->maxCount < box->count) {
        box->maxCount = box->count;
        for (i = 0; i < box->maxCount - 2; i++) {
            M532ScoreQuad *q = &box->big[i];
            q->alpha = 1.0f;
            q->scale = 1.0f;
        }
        fn_1_E4D4(&box->fade, lbl_1_bss_A60.m_v, 0.0f, 1.0f);
    }
}

void fn_1_DC78(M532ScoreBox *box)
{
    f32 val;
    f32 a;
    M532ScoreQuad *q;

    if (box->maxCount > 0 && box->fade.cur <= box->fade.total) {
        val = box->fade.val;
        q = &box->big[box->maxCount - 1];
        if (val < 0.0f) {
            a = 0.0f;
        } else if (val > 1.0f) {
            a = 1.0f;
        } else {
            a = val;
        }
        q->alpha = a;
        q->scale = (f32)((1.0 - (f64)val) * 4.0 + (f64)val * 1.0);
        fn_1_E3B0(&box->fade);
    }
}

void fn_1_DDDC(M532ScoreBox *box)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        fn_1_D4B0(&box->small[i]);
    }
    for (i = 0; i < box->maxCount; i++) {
        fn_1_D4B0(&box->big[i]);
    }
}

// NONMATCHING: stack spills / inlining / register allocation differ
f32 fn_1_E3B0(M532ScoreFade *fade)
{
    f32 val;
    f64 t;

    val = fade->val;
    if (fade->cur <= fade->total) {
        fade->cur = fade->cur + 1;
        t = (f64)fade->cur / (f64)(fade->total + 1);
        val = (f32)((f64)fade->from * (1.0 - t) + (f64)fade->to * t);
        fade->val = val;
    }
    return val;
}

void fn_1_E490(M532ScoreFade *fade, s32 total, f32 to)
{
    fn_1_E4D4(fade, total, fade->val, to);
}

void fn_1_E4D4(M532ScoreFade *fade, s32 total, f32 from, f32 to)
{
    fade->total = total;
    fade->cur = 0;
    if (total == 0) {
        fade->val = to;
        fade->from = to;
        fade->to = to;
        return;
    }
    fade->val = from;
    fade->from = from;
    fade->to = to;
}

// NONMATCHING: the static initializer is named __sinit_score_cpp (original fn_1_E508)
// Static init: the four score timing constants (frames).
M532ScoreConst lbl_1_bss_A58(300);
M532ScoreConst lbl_1_bss_A5C(60);
M532ScoreConst lbl_1_bss_A60(30);
M532ScoreConst lbl_1_bss_A64(12);

}
