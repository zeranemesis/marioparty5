#include "dolphin.h"
#include <math.h>
#include "game/data.h"
#include "game/hu3d.h"
#include "game/sprite.h"
#include "REL/m579Dll/m579types.h"

typedef struct M579FishMgr {
    M579Base base;
} M579FishMgr;

extern "C" {
void *__register_global_object(void *object, void *destructor, void *regmem);
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);
void fn_1_E9BC(M579Player *player, Mtx out);
void fn_1_7B4C(M579Fish *f);
s32 fn_1_7CD8(M579Fish *f, Vec *a);
void fn_1_84EC(M579Fish *f, Vec *a, u8 flag);
M579Puku *fn_1_62B4(M579Fish *owner, s32 type);
void fn_1_7324(M579Puku *puku, u8 flag);
extern M579FishMgr *lbl_1_bss_A28;
extern u8 lbl_1_bss_C[];
extern u8 lbl_1_data_1FC[];

void *fn_1_10C84(M579FishMgr *self);
M579FishMgr *fn_1_110B4(M579FishMgr *self, s16 flag);
M579FishMgr *fn_1_11140(M579FishMgr *self, s16 flag);
void fn_1_11244(void);
void fn_1_11264(M579FishMgr *self, s32 bit);
void fn_1_112C4(HU3DMODEL *modelP, Mtx *mtx);
M579Fish *fn_1_11308(M579Fish *self, Vec *pos, s32 index, f32 rotY);
M579Fish *fn_1_115B8(M579Fish *self, s16 flag);
M579Fish *fn_1_11610(M579Fish *self, s16 flag);
void fn_1_11678(void);
void fn_1_11748(void);
M579Fish *fn_1_11778(Vec *pos);
s32 fn_1_11844(Vec *pos);
s32 fn_1_1191C(void);
M579Fish *fn_1_11990(Vec *pos);
f32 fn_1_11C34(M579Fish *f, Vec *pos);
void fn_1_11E2C(M579Fish *f);
void fn_1_121CC(M579Fish *f);
void fn_1_1246C(M579Fish *f);
void fn_1_12A88(M579Fish *f);
void fn_1_12CDC(M579Fish *f);
void fn_1_12F38(void);
void fn_1_13014(void *unused, s32 idx);
void fn_1_13168(M579Fish *f);
void fn_1_132F4(M579Fish *f, Vec *off);
void fn_1_13678(M579Fish *f, Mtx mtx);
void fn_1_13FE0(M579Fish *f);
void fn_1_1404C(M579Fish *f, u8 flag);
void fn_1_14098(M579Node *list, void (*fn)(M579Fish *));
void fn_1_140F8(M579Node *first, M579Node *list, void (*fn)(M579Fish *));
void fn_1_14158(void);
M579Node *fn_1_141B0(M579Node *self, s16 flag);
}

char lbl_1_data_528[16] = "ms_pointer == 0";
char lbl_1_data_538[12] = "singleton.h";
char lbl_1_data_544[16] = "ms_pointer != 0";
s32 lbl_1_data_554[12] = { 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3 };
f32 lbl_1_data_584[9] = { -0.15f, -0.025000006f, 0.099999994f, 0.225f, 0.35f, 0.475f, 0.6f, 0.725f, 0.85f };
void *lbl_1_data_5A8[6] = { 0, 0, (void *) fn_1_11140, (void *) fn_1_11264, (void *) fn_1_11244, 0 };

Vec lbl_1_bss_AE0[20][8];
ANIMDATA *lbl_1_bss_1260[4];
u8 lbl_1_bss_1270;
u8 lbl_1_bss_1274[12];
M579Node lbl_1_bss_1280;

M579_RODATA const Vec lbl_1_rodata_658 = { 0.0f, 1.0f, 0.0f };
M579_RODATA const Vec lbl_1_rodata_6A8 = { 0.0f, 0.0f, 0.0f };
M579_RODATA const Vec lbl_1_rodata_6B4 = { 0.0f, 1.0f, 0.0f };
M579_RODATA const Vec lbl_1_rodata_6E8 = { 0.0f, 0.0f, 0.0f };

void *fn_1_10C84(M579FishMgr *self)
{
    Vec v;
    f32 r;
    f32 a;
    s32 i;
    s32 j;
    s32 k;
    void *mem;

    fn_1_1264(self);
    self->base.vtbl = (void **) lbl_1_data_1FC;
    self->base.flags = 0;
    fn_1_1330(self, lbl_1_bss_C);
    lbl_1_bss_A28 == 0 ? (void) 0 : fn_1_1D4(lbl_1_data_528, lbl_1_data_538, 0x35);
#ifdef TARGET_PC
    lbl_1_bss_A28 = self;
#else
    {
        u8 *q = (u8 *) self + 0x10;
        if (q != 0) {
            q -= 0x10;
        }
        lbl_1_bss_A28 = (M579FishMgr *) q;
    }
#endif
    self->base.vtbl = lbl_1_data_5A8;
    lbl_1_bss_1270 = 0;
    lbl_1_bss_1260[0] = HuSprAnimRead(HuDataSelHeapReadNum(0x85000E, 0x10000000, HEAP_MODEL));
    lbl_1_bss_1260[1] = HuSprAnimRead(HuDataSelHeapReadNum(0x85000F, 0x10000000, HEAP_MODEL));
    lbl_1_bss_1260[2] = HuSprAnimRead(HuDataSelHeapReadNum(0x850010, 0x10000000, HEAP_MODEL));
    lbl_1_bss_1260[3] = HuSprAnimRead(HuDataSelHeapReadNum(0x850011, 0x10000000, HEAP_MODEL));
    for (i = 0; i < 4; i++) {
        HuSprAnimLock(lbl_1_bss_1260[i]);
    }
    for (i = 0; i < 20; i++) {
        r = 0.25f * (f32) (0x18 - i);
        for (j = 0; j < 8; j++) {
            lbl_1_bss_AE0[i][j].x = (f32) ((f64) r * cos((3.141592653589793 * (f64) (j * 0x2D)) / 180.0));
            lbl_1_bss_AE0[i][j].y = 0.0f;
            lbl_1_bss_AE0[i][j].z = (f32) ((f64) r * sin((3.141592653589793 * (f64) (j * 0x2D)) / 180.0));
        }
    }
    for (k = 0; k < 12; k++) {
        a = 15.0f + 30.0f * (f32) k;
        v.x = (f32) (540.0 * cos((3.141592653589793 * (f64) a) / 180.0));
        v.y = 0.0f;
        v.z = (f32) (540.0 * sin((3.141592653589793 * (f64) a) / 180.0));
        mem = fn_1_0(sizeof(M579Fish));
        if (mem != 0) {
            fn_1_11308((M579Fish *) mem, &v, k, -a - 90.0f);
        }
    }
    return self;
}

M579FishMgr *fn_1_110B4(M579FishMgr *self, s16 flag)
{
    if (self != 0) {
        lbl_1_bss_A28 != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_544, lbl_1_data_538, 0x3D);
        lbl_1_bss_A28 = 0;
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M579FishMgr *fn_1_11140(M579FishMgr *self, s16 flag)
{
    s32 i;

    if (self != 0) {
        self->base.vtbl = lbl_1_data_5A8;
        fn_1_11678();
        for (i = 0; i < 4; i++) {
            HuSprAnimKill(lbl_1_bss_1260[i]);
        }
        if (self != 0) {
            lbl_1_bss_A28 != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_544, lbl_1_data_538, 0x3D);
            lbl_1_bss_A28 = 0;
        }
        if (self != 0) {
            self->base.vtbl = (void **) lbl_1_data_1FC;
            if (self != 0) {
                fn_1_1270(self, 0);
            }
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_11244(void)
{
    fn_1_11748();
}

void fn_1_11264(M579FishMgr *self, s32 bit)
{
    self->base.flags = self->base.flags & ~(1 << bit);
    switch (bit) {
    case 2:
        lbl_1_bss_1270 = 1;
        return;
    case 3:
        lbl_1_bss_1270 = 0;
        return;
    }
}

void fn_1_112C4(HU3DMODEL *modelP, Mtx *mtx)
{
    Mtx *m;

    m = mtx;
    fn_1_13678((M579Fish *) modelP->hookData, *m);
}

M579Fish *fn_1_11308(M579Fish *self, Vec *pos, s32 index, f32 rotY)
{
    s16 model;
    s32 i;

    fn_1_1264(self);
    self->pos.x = pos->x;
    self->pos.y = pos->y;
    self->pos.z = pos->z;
    self->index = index;
    self->rotY = rotY;
    self->caught = 0;
    self->held = 0;
    self->b1E = 0;
    self->b1F = 0;
    self->b20 = 0;
    self->ready = 0;
    self->ropeInit = 0;
    self->ropeLen = 600.0f;
    self->mode = 8;
    self->timer = 0;
    self->wait = 0xF0;
    self->f780 = 0.0f;
    self->f784 = 2.0f;
    self->f788 = 0.0f;
    self->f78C = 40.0f;
    self->holder = 0;
    self->puku = 0;
    fn_1_1330(self, &lbl_1_bss_1280);
    model = Hu3DHookFuncCreate(fn_1_112C4);
    self->model = model;
    Hu3DModelLayerSet(model, 4);
    Hu3DModelCameraSet(model, 1);
    Hu3DModelPosSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DData[model].hookData = self;
    self->rnd = fn_1_4AE0();
    fn_1_7B4C(self);
    for (i = 0; i < 20; i++) {
        self->seg[i] = 0.0f;
        PSMTXIdentity(self->mtx[i]);
    }
    self->axis.x = 1.0f;
    self->axis.y = 0.0f;
    self->axis.z = 0.0f;
    self->lineOfs.x = 0.0f;
    self->lineOfs.y = 0.0f;
    self->lineOfs.z = 0.0f;
    self->linePrev.x = 0.0f;
    self->linePrev.y = 0.0f;
    self->linePrev.z = 0.0f;
    return self;
}

M579Fish *fn_1_115B8(M579Fish *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M579Fish *fn_1_11610(M579Fish *self, s16 flag)
{
    if (self != 0) {
        Hu3DModelKill(self->model);
        if (self != 0) {
            fn_1_1270(self, 0);
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_11678(void)
{
    M579Fish *p;
    u8 empty;

    for (;;) {
        empty = 0;
        if (lbl_1_bss_1280.next == &lbl_1_bss_1280 && lbl_1_bss_1280.next == lbl_1_bss_1280.prev) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        p = (M579Fish *) lbl_1_bss_1280.prev;
        if (p != 0) {
            Hu3DModelKill(p->model);
            if (p != 0) {
                fn_1_1270(p, 0);
            }
            fn_1_C8(p);
        }
    }
}

void fn_1_11748(void)
{
    fn_1_14098(&lbl_1_bss_1280, fn_1_12CDC);
}

M579Fish *fn_1_11778(Vec *pos)
{
    M579Fish *res;
    M579Node *n;
    M579Fish *f;
    f32 dx;
    f32 dz;

    res = 0;
    n = lbl_1_bss_1280.next;
    while (n != &lbl_1_bss_1280) {
        f = (M579Fish *) n;
        dx = f->pos.x - pos->x;
        dz = f->pos.z - pos->z;
        if (f->caught == 0 && (dx * dx + dz * dz) < 4900.0f) {
            res = f;
        }
        n = n->next;
    }
    return res;
}

s32 fn_1_11844(Vec *pos)
{
    M579Node *n;
    M579Fish *f;
    f32 dx;
    f32 dz;

    n = lbl_1_bss_1280.next;
    while (n != &lbl_1_bss_1280) {
        f = (M579Fish *) n;
        if (f->mode < 8 && f->caught == 0) {
            dx = f->pos.x - pos->x;
            dz = f->pos.z - pos->z;
            if ((dx * dx + dz * dz) < 4900.0f) {
                return 1;
            }
        }
        n = n->next;
    }
    return 0;
}

s32 fn_1_1191C(void)
{
    M579Node *n;
    M579Node *a;
    M579Node *b;
    u8 found;

    a = lbl_1_bss_1280.next;
    b = a;
    n = b;
    for (;;) {
        if (n == &lbl_1_bss_1280) {
            found = 0;
            break;
        }
        if (((M579Fish *) n)->mode == 7) {
            found = 1;
            break;
        }
        n = n->next;
    }
    return found;
}

M579Fish *fn_1_11990(Vec *pos)
{
    M579Fish *res;
    M579Node *n;
    M579Fish *f;
    f32 best;
    f32 dx;
    f32 dz;
    f32 d;

    res = 0;
    n = lbl_1_bss_1280.next;
    best = 2000.0f;
    while (n != &lbl_1_bss_1280) {
        f = (M579Fish *) n;
        if (f->mode < 8 && f->caught == 0) {
            dx = f->pos.x - pos->x;
            dz = f->pos.z - pos->z;
            d = (f32) sqrt(dx * dx + dz * dz);
            if (d < best) {
                best = d;
                res = f;
            }
        }
        n = n->next;
    }
    return res;
}

f32 fn_1_11C34(M579Fish *f, Vec *pos)
{
    f32 dx;
    f32 dz;

    dx = f->pos.x - pos->x;
    dz = f->pos.z - pos->z;
    return (f32) sqrt(dx * dx + dz * dz);
}

void fn_1_11E2C(M579Fish *f)
{
    M579Node *n;
    s32 v;
    s32 w;
    u8 found;
    f32 t;

    if (f->held != 0) {
        if (f->b1E == 0) {
            f->b1E = 1;
            f->f784 = 15.0f;
        } else {
            f->f784 = (f32) (f->f784 + (0.2f * (1.0f - f->f784)));
        }
        f->count24 = f->count24 + 1;
        return;
    }
    f->timer = f->timer - 1;
    if (f->timer < 0) {
        f->mode = 8;
    }
    if (f->mode != 7) {
        if (f->mode < 7 && f->mode >= 0) {
            if ((s32) (fn_1_4AE0() & 7) == 0) {
                f->f784 = 4.0f;
                return;
            }
            f->f784 = (f32) (f->f784 + (0.1f * (3.2f - f->f784)));
            return;
        }
        if (f->ropeFlag[29] != 0) {
            f->f784 = (f32) (2.0 + (0.5 * sin((3.141592653589793 * (f64) (360.0f * ((f32) (f->rnd & 0xFF) / 255.0f))) / 180.0)));
        } else {
            f->f784 = (f32) (f->f784 + (0.1f * (2.0f - f->f784)));
        }
        if (lbl_1_bss_1270 != 0 && f->caught == 0 && f->ropeFlag[29] != 0) {
            w = f->wait;
            f->wait = w + 1;
            if ((f32) w > 300.0f && (s32) (fn_1_4AE0() & 0x1FF) == 0) {
                v = fn_1_4AE0() & 7;
                if (v == 7) {
                    n = lbl_1_bss_1280.next;
                    for (;;) {
                        if (n == &lbl_1_bss_1280) {
                            found = 0;
                            break;
                        }
                        if (((M579Fish *) n)->mode == 7) {
                            found = 1;
                            break;
                        }
                        n = n->next;
                    }
                    if (found != 0) {
                        v = 0;
                    }
                }
                f->mode = v;
                t = fn_1_4D1C() * 120.0f;
                f->timer = (s32) (300.0f + t);
                f->wait = 0;
                fn_1_9668(&f->pos, 0x95C);
            }
        }
        return;
    }
    if ((s32) (fn_1_4AE0() & 7) == 0) {
        f->f784 = 5.0f;
        return;
    }
    f->f784 = (f32) (f->f784 + (0.1f * (4.0f - f->f784)));
}

void fn_1_121CC(M579Fish *f)
{
    Mtx tm;
    Vec v;
    f32 sq;
    f64 len;
    s32 i;

    PSMTXTranspose(f->mtx[2], tm);
    PSMTXMultVec(tm, &f->ropePos, &v);
    PSVECCrossProduct(&lbl_1_rodata_658, &v, &f->axis);
    f->f780 = (f32) (f->f780 + (0.1f * (f->f784 - f->f780)));
    sq = (v.x * v.x) + (v.z * v.z);
    len = sqrt(sq);
    for (i = 1; i < 0x14; i++) {
        f->seg[i] = (f32) (0.0003 * (f64) (f32) ((f64) f->f780 * len) * (f64) (1.0f + (1.1f * (f32) i)));
    }
}

void fn_1_1246C(M579Fish *f)
{
    Mtx tm;
    Vec v;
    Vec d;
    f32 x;
    f32 z;
    f32 sq;
    f32 dist;
    f64 len;
    s32 i;

    fn_1_11E2C(f);
    f->f78C = (f32) ((f64) f->f78C + (0.20000000298023224 * (30.0 - (f64) f->f78C)));
    if (f->ropeInit != 0) {
        x = f->lineOfs.x;
        z = f->lineOfs.z;
        sq = (x * x) + (z * z);
        if (sq > 0.0f) {
            dist = (f32) sqrt(sq);
        } else {
            dist = sq;
        }
        if (dist > 370.0000047683716) {
            f->ropeLen = (f32) (f->ropeLen - 16.666666f);
            if (f->ropeLen < 100.0f) {
                f->ropeLen = 100.0f;
            }
        } else {
            f->ropeLen = (f32) (f->ropeLen + 50.0f);
            if (f->ropeLen > 600.0f) {
                f->ropeLen = 600.0f;
            }
        }
    }
    if (f->held != 0) {
        f->f788 = 20.0f;
        f->b1F = 0;
        f->b20 = 0;
    } else {
        f->f788 = (f32) ((f64) f->f788 * 0.95);
        if (f->b1F == 0) {
            f->b20 = 1;
        } else {
            f->b20 = 0;
        }
        f->b1F = 1;
    }
    if (f->ready != 0) {
        if (f->ropeInit == 0) {
            f->ropeInit = 1;
            f->linePrev.x = f->lineOfs.x;
            f->linePrev.y = f->lineOfs.y;
            f->linePrev.z = f->lineOfs.z;
        }
        if (f->b20 != 0) {
            d.x = 0.0f;
            d.y = 0.0f;
            d.z = 0.0f;
        } else {
            PSVECSubtract(&f->lineOfs, &f->linePrev, &d);
        }
        fn_1_84EC(f, &d, (u8) fn_1_7CD8(f, &d));
        f->linePrev.x = f->lineOfs.x;
        f->linePrev.y = f->lineOfs.y;
        f->linePrev.z = f->lineOfs.z;
        PSMTXTranspose(f->mtx[2], tm);
        PSMTXMultVec(tm, &f->ropePos, &v);
        PSVECCrossProduct(&lbl_1_rodata_658, &v, &f->axis);
        f->f780 = (f32) (f->f780 + (0.1f * (f->f784 - f->f780)));
        sq = (v.x * v.x) + (v.z * v.z);
        len = sqrt(sq);
        for (i = 1; i < 0x14; i++) {
            f->seg[i] = (f32) (0.0003 * (f64) (f32) ((f64) f->f780 * len) * (f64) (1.0f + (1.1f * (f32) i)));
        }
    }
}

void fn_1_12A88(M579Fish *f)
{
    Vec rot;
    Vec p;
    Vec a;
    Vec b;
    f32 tx;
    f32 tz;
    f32 ang;
    M579Puku *puku;

    if (f->puku != 0) {
        rot = lbl_1_rodata_6A8;
        PSVECAdd(&f->lineOfs, &f->ropePos, &p);
        tx = -f->ropePos.x;
        tz = -f->ropePos.z;
        if (tx != 0.0f || tz != 0.0f) {
            a = lbl_1_rodata_6B4;
            b = f->ropePos;
            PSVECNormalize(&a, &a);
            PSVECNormalize(&b, &b);
            rot.x = -(f32) (-90.0 + (180.0 * (acos(PSVECDotProduct(&a, &b)) / 3.141592653589793)));
            ang = (f32) (180.0 * (atan2(tx, tz) / 3.141592653589793));
            rot.y = ang;
            f->yaw = ang;
        } else {
            if (tz > 0.0f) {
                rot.x = 90.0f;
            } else {
                rot.x = -90.0f;
            }
            rot.y = f->yaw;
        }
        puku = f->puku;
        puku->pos = p;
        puku->rot.x = rot.x;
        puku->rot.y = rot.y;
        puku->rot.z = rot.z;
        puku->rot.z = 0.0f;
    }
}

void fn_1_12CDC(M579Fish *f)
{
    fn_1_1246C(f);
    fn_1_12A88(f);
}

void fn_1_12F38(void)
{
    GXSetNumTevStages(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetCullMode(GX_CULL_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
}

void fn_1_13014(void *unused, s32 idx)
{
    Mtx m;

    HuSprTexLoad(lbl_1_bss_1260[idx], 0, 0, GX_REPEAT, GX_CLAMP, GX_LINEAR);
    PSMTXIdentity(m);
    GXLoadTexMtxImm(m, 0x1E, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1E, GX_FALSE, 0x7D);
    GXSetZMode(GX_TRUE, GX_LESS, GX_TRUE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetChanCtrl(GX_COLOR_NULL, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_COPY);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
}

void fn_1_13168(M579Fish *f)
{
    Mtx m;
    Mtx m1;
    Mtx m2;
    Mtx t;
    Quaternion q;
    s32 i;

    if (f->held != 0) {
        fn_1_E9BC(f->holder, m);
    } else {
        PSMTXRotRad(m1, 'y', 0.017453292f * f->rotY);
        PSMTXRotRad(m2, 'x', 0.017453292f * (f->f78C + f->f788));
        PSMTXConcat(m1, m2, m);
        PSMTXTransApply(m, m, f->pos.x, f->pos.y, f->pos.z);
    }
    for (i = 0; i < 0x14; i++) {
        if (i == 0) {
            PSMTXCopy(m, f->mtx[i]);
        } else {
            C_QUATRotAxisRad(&q, &f->axis, (f32) ((3.141592653589793 * (f64) f->seg[i]) / 180.0));
            PSMTXQuat(t, &q);
            PSMTXTransApply(t, t, 0.0f, 20.0f, 0.0f);
            PSMTXConcat(f->mtx[i - 1], t, f->mtx[i]);
        }
    }
}

void fn_1_132F4(M579Fish *f, Vec *off)
{
    f32 x;
    f32 y;
    f32 z;
    f32 dist;
    s32 i;

    GXSetLineWidth(6, GX_TO_ZERO);
    GXBegin(GX_LINESTRIP, GX_VTXFMT0, 0x1E);
    for (i = 0; i < 0x1E; i++) {
        x = f->ropePt[i].x + off->x;
        y = f->ropePt[i].y + off->y;
        z = f->ropePt[i].z + off->z;
        dist = (f32) sqrt((x * x) + (z * z));
        if (dist > 500.0 && dist < 800.0 && y < 5.0f) {
            f->ropePt[i].y = f->ropePt[i].y - (5.0f + off->y);
            y = 5.0f;
        }
        if (y < -150.0) {
            f->ropeFlag[i] = 1;
        } else {
            f->ropeFlag[i] = 0;
        }
        GXPosition3f32(x, y, z);
        GXColor4u8(0xFF, 0xFF, 0xFF, 0xFF);
    }
    GXEnd();
}

void fn_1_13678(M579Fish *f, Mtx mtx)
{
    Vec world[20][8];
    Mtx m;
    Mtx m1;
    Mtx m2;
    Mtx t;
    Mtx idm;
    Quaternion q;
    f32 x;
    f32 y;
    f32 z;
    f32 dist;
    f32 tx;
    s32 i;
    s32 j;
    s32 k;

    GXLoadPosMtxImm(mtx, 0);
    if (f->held != 0) {
        fn_1_E9BC(f->holder, m);
    } else {
        PSMTXRotRad(m1, 'y', 0.017453292f * f->rotY);
        PSMTXRotRad(m2, 'x', 0.017453292f * (f->f78C + f->f788));
        PSMTXConcat(m1, m2, m);
        PSMTXTransApply(m, m, f->pos.x, f->pos.y, f->pos.z);
    }
    for (i = 0; i < 0x14; i++) {
        if (i == 0) {
            PSMTXCopy(m, f->mtx[i]);
        } else {
            C_QUATRotAxisRad(&q, &f->axis, (f32) ((3.141592653589793 * (f64) f->seg[i]) / 180.0));
            PSMTXQuat(t, &q);
            PSMTXTransApply(t, t, 0.0f, 20.0f, 0.0f);
            PSMTXConcat(f->mtx[i - 1], t, f->mtx[i]);
        }
    }
    for (i = 0; i < 0x14; i++) {
        for (j = 0; j < 8; j++) {
            PSMTXMultVec(f->mtx[i], &lbl_1_bss_AE0[i][j], &world[i][j]);
        }
    }
    HuSprTexLoad(lbl_1_bss_1260[lbl_1_data_554[f->index]], 0, 0, GX_REPEAT, GX_CLAMP, GX_LINEAR);
    PSMTXIdentity(idm);
    GXLoadTexMtxImm(idm, 0x1E, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1E, GX_FALSE, 0x7D);
    GXSetZMode(GX_TRUE, GX_LESS, GX_TRUE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetChanCtrl(GX_COLOR_NULL, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_COPY);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    for (i = 0; i < 0x13; i++) {
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 0x12);
        for (j = 0; j < 9; j++) {
            k = j & 7;
            GXPosition3f32(world[i + 1][k].x, world[i + 1][k].y, world[i + 1][k].z);
            GXTexCoord2f32(lbl_1_data_584[j], (f32) (0x14 - (i + 1)) / 20.0f);
            GXPosition3f32(world[i][k].x, world[i][k].y, world[i][k].z);
            tx = lbl_1_data_584[j];
            GXTexCoord2f32(tx, (f32) (0x14 - i) / 20.0f);
        }
        GXEnd();
    }
    PSMTXMultVec(f->mtx[19], &lbl_1_rodata_6E8, &f->lineOfs);
    GXSetNumTevStages(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetCullMode(GX_CULL_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetLineWidth(6, GX_TO_ZERO);
    GXBegin(GX_LINESTRIP, GX_VTXFMT0, 0x1E);
    for (i = 0; i < 0x1E; i++) {
        x = f->ropePt[i].x + f->lineOfs.x;
        y = f->ropePt[i].y + f->lineOfs.y;
        z = f->ropePt[i].z + f->lineOfs.z;
        dist = (f32) sqrt((x * x) + (z * z));
        if (dist > 500.0 && dist < 800.0 && y < 5.0f) {
            f->ropePt[i].y = f->ropePt[i].y - (5.0f + f->lineOfs.y);
            y = 5.0f;
        }
        if (y < -150.0) {
            f->ropeFlag[i] = 1;
        } else {
            f->ropeFlag[i] = 0;
        }
        GXPosition3f32(x, y, z);
        GXColor4u8(0xFF, 0xFF, 0xFF, 0xFF);
    }
    GXEnd();
    f->ready = 1;
}

void fn_1_13FE0(M579Fish *f)
{
    s32 type;

    if (f->mode > 7) {
        f->puku = 0;
        return;
    }
    type = 1;
    if (f->mode < 7) {
        type = 0;
    }
    f->puku = fn_1_62B4(f, type);
}

void fn_1_1404C(M579Fish *f, u8 flag)
{
    u8 fl;

    fl = flag;
    if (f->puku != 0) {
        fn_1_7324(f->puku, fl);
        f->puku = 0;
    }
}

void fn_1_14098(M579Node *list, void (*fn)(M579Fish *))
{
    M579Node *l;
    M579Node *first;
    M579Node *n;

    l = list;
    first = l->next;
    n = first;
    while (n != l) {
        fn((M579Fish *) n);
        n = n->next;
    }
}

void fn_1_140F8(M579Node *first, M579Node *list, void (*fn)(M579Fish *))
{
    M579Node *n;

    n = first;
    while (n != list) {
        fn((M579Fish *) n);
        n = n->next;
    }
}

void fn_1_14158(void)
{
    M579Node *a;
    M579Node *b;

    a = &lbl_1_bss_1280;
    fn_1_1264(a);
    b = a;
#ifndef TARGET_PC
    __register_global_object(b, (void *) fn_1_141B0, lbl_1_bss_1274);
#endif
}

M579Node *fn_1_141B0(M579Node *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}
