// m531Dll "cleaneff": the cleaning-contest board. Each side of the stage owns a
// 128x128 grid of dirt heights (CCleanSide, a CEffectModel that draws a
// distorted quad through the indirect texture unit); the singleton CClean holds
// the two sides. Classes carry their vtable pointer after the base data (offset
// 0xC on the GameCube) and are driven through hand-written vtables.
#define _MATH_H
#include "dolphin.h"

extern "C" {
double sin(double x);
double cos(double x);
double atan(double x);
double fabs(double x);
void abort(void);

void *__dl__FPv(void *ptr);
void *fn_1_0(s32 size);
void fn_1_1D4(const char *expr, const char *file, s32 line);
int fn_1_234(void *unused, const char *fmt, ...);
// The base constructor and destructor live in effect.cpp.
void __ct__12CEffectModelFP9Process_sssUs(void *self, struct Process_s *objMan, s16 layer, s16 prio, u16 camera);
void __dt__12CEffectModelFv(void *self, s32 flag);

typedef struct ANIMDATA ANIMDATA;
s32 HuAudFXPlay(s32 seId);
void HuAudFXStop(s32 seNo);
void *HuDataSelHeapReadNum(s32 dataNum, s32 num, s32 heap);
ANIMDATA *HuSprAnimRead(void *data);
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);
}

#ifdef TARGET_PC
// The original passes stderr (an element of the DOL's __files); fn_1_234 ignores it.
#define CLEAN_STDERR 0
#else
extern "C" u8 __files[];
#define CLEAN_STDERR (__files + 0xA0)
#endif

#ifdef TARGET_PC
#define VTX_POS3(x, y, z) GXPosition3f32(x, y, z)
#define VTX_TEX2(u, v) GXTexCoord2f32(u, v)
#define VTX_END() GXEnd()
#else
#define VTX_POS3(x, y, z) \
    { \
        GXWGFifo.f32 = (x); \
        GXWGFifo.f32 = (y); \
        GXWGFifo.f32 = (z); \
    }
#define VTX_TEX2(u, v) \
    { \
        GXWGFifo.f32 = (u); \
        GXWGFifo.f32 = (v); \
    }
#define VTX_END()
#endif

// Float-to-byte store: the PC compilers leave an out-of-range direct conversion undefined.
#ifdef TARGET_PC
#define F2B(v) ((u8)(s32)(v))
#else
#define F2B(v) ((s8)(v))
#endif

#define CLEAN_GRID 128

struct CCleanSide;

struct CleanSideVt {
    void *m_head[2];
    void (*m_update)(CCleanSide *self);
    void (*m_draw)(CCleanSide *self, Mtx mtx);
    CCleanSide *(*m_dtor)(CCleanSide *self, s16 flag);
};

// std::vector<float> as laid out by the original STL: capacity, size, storage.
struct CFloatVec {
    u32 m_cap;
    u32 m_size;
    f32 *m_data;
};

// One side of the board. Offsets in comments are the GameCube ones.
struct CCleanSide {
    struct Process_s *m_objMan; // 0x00 CEffectModel
    struct omObj_s *m_obj; // 0x04
    s16 m_model; // 0x08
    const CleanSideVt *m_vt; // 0x0C
    GXTexObj m_texMask; // 0x10 128x128 I8 mask of the cells still dirty
    GXTexObj m_texWave; // 0x30 16x32 IA8 distortion pattern
    ANIMDATA *m_anim; // 0x50
    f32 ATTRIBUTE_ALIGN(32) m_height[CLEAN_GRID][CLEAN_GRID]; // 0x60
    u8 m_maskImg[0x4000]; // 0x10060
    u8 m_waveImg[0x400]; // 0x14060
    s32 m_total; // 0x14460 cell count
    f32 m_thresh; // 0x14464 height at which a cell counts as dirty
    u8 m_dirty; // 0x14468
    s32 m_remain; // 0x1446C cells still dirty
    f32 m_x0; // 0x14470 world bounds of the grid
    f32 m_x1; // 0x14474
    f32 m_z0; // 0x14478
    f32 m_z1; // 0x1447C
    Vec m_center; // 0x14480 fixed centre the quad scales around
    f32 m_blend; // 0x1448C
    CFloatVec m_stamp; // 0x14490 brush profile
    f32 m_stampRadius; // 0x1449C
    s32 m_stampW; // 0x144A0
    s32 m_stampH; // 0x144A4
    f32 m_scrollU; // 0x144A8
    f32 m_scrollV; // 0x144AC
    s32 m_seId; // 0x144B0
    s32 m_seHandle; // 0x144B4
    s32 m_state; // 0x144B8
    s32 m_frame; // 0x144BC
};

// Owning pointer to a side: the assignment deletes the previous one.
struct CSidePtr {
    CCleanSide *m_ptr;
};

// The singleton holding both sides.
struct CClean {
    CSidePtr m_side[2];
};

extern "C" {
extern void *lbl_1_bss_1C;
f32 lbl_1_data_49C = 0.0f;
extern CleanSideVt lbl_1_data_4A0;

void *fn_1_D62C(CClean *self, struct Process_s *objMan);
CSidePtr *fn_1_D7F0(CSidePtr *self, s16 flag);
CClean *fn_1_D86C(CClean *self, s16 flag);
CClean *fn_1_D8F8(CClean *self, s16 flag);
s32 fn_1_DA00(CClean *self, u8 mode);
CCleanSide *fn_1_DAEC(CClean *self, s32 side);
CCleanSide *fn_1_DB1C(CCleanSide *self, struct Process_s *objMan, f32 x0, f32 x1, f32 z0, f32 z1);
CFloatVec *fn_1_DD90(CFloatVec *self, s16 flag);
CFloatVec *fn_1_DE24(CFloatVec *self, s16 flag);
CFloatVec *fn_1_DEB0(CFloatVec *self, s16 flag);
CCleanSide *fn_1_DF34(CCleanSide *self, s16 flag);
void fn_1_E010(CCleanSide *self);
void fn_1_E31C(CCleanSide *self);
void fn_1_E560(CCleanSide *self);
s32 fn_1_E5A4(u8 **dst, const f32 *src, s32 stride, s32 thresh);
void fn_1_E694(CCleanSide *self);
u8 fn_1_E800(CCleanSide *self, const Vec *pos, s32 *outX, s32 *outZ);
void fn_1_E9C8(CCleanSide *self, f32 radius);
void fn_1_EE68(CCleanSide *self, const Vec *pos, f32 size, f32 strength);
void fn_1_F624(CCleanSide *self, const Vec *pos, f32 size);
void fn_1_F668(CCleanSide *self, const Vec *pos, f32 size);
void fn_1_F6AC(CCleanSide *self, const Vec *pos);
void fn_1_F6D4(CCleanSide *self, f32 value);
f32 fn_1_F744(CCleanSide *self);
f32 fn_1_F7BC(CCleanSide *self, const Vec *a, const Vec *b);
void fn_1_FDC8(CCleanSide *self);
void fn_1_100CC(CCleanSide *self);
void fn_1_10294(CCleanSide *self);
void fn_1_102E0(CCleanSide *self);
void fn_1_10330(CCleanSide *self);
void fn_1_109C0(CCleanSide *self, Mtx mtx);
void fn_1_110C8(CFloatVec *vec, u32 size, const f32 *value);
f32 *fn_1_11178(CFloatVec *vec, f32 *first, f32 *last);
void fn_1_11220(CFloatVec *vec, f32 *pos, u32 count, const f32 *value);
}

CleanSideVt lbl_1_data_4A0 = {
    { 0, 0 },
    fn_1_10330,
    fn_1_109C0,
    fn_1_DF34,
};

extern "C" {

void *fn_1_D62C(CClean *self, struct Process_s *objMan)
{
    CCleanSide *side0;
    CCleanSide *side1;
    CCleanSide *old;
    CCleanSide *tmp0;
    CCleanSide *tmp1;

    if (lbl_1_bss_1C != 0) {
        fn_1_1D4("ms_pointer == 0", "singleton.h", 0x35);
    }
    lbl_1_bss_1C = self;
    self->m_side[0].m_ptr = 0;
    self->m_side[1].m_ptr = 0;
    side0 = (CCleanSide *)fn_1_0(sizeof(CCleanSide));
    if (side0 != 0) {
        side0 = fn_1_DB1C(side0, objMan, -525.0f, -125.0f, -230.0f, 610.0f);
    }
    tmp0 = side0;
    if (self->m_side[0].m_ptr != tmp0) {
        old = self->m_side[0].m_ptr;
        if (old != 0) {
            old->m_vt->m_dtor(old, 1);
        }
        self->m_side[0].m_ptr = tmp0;
    }
    side1 = (CCleanSide *)fn_1_0(sizeof(CCleanSide));
    if (side1 != 0) {
        side1 = fn_1_DB1C(side1, objMan, 125.0f, 525.0f, -230.0f, 610.0f);
    }
    tmp1 = side1;
    if (self->m_side[1].m_ptr != tmp1) {
        old = self->m_side[1].m_ptr;
        if (old != 0) {
            old->m_vt->m_dtor(old, 1);
        }
        self->m_side[1].m_ptr = tmp1;
    }
    fn_1_F6D4(self->m_side[1].m_ptr, 1.0f);
    return self;
}

CSidePtr *fn_1_D7F0(CSidePtr *self, s16 flag)
{
    if (self != 0) {
        CCleanSide *p = self->m_ptr;
        if (p != 0) {
            p->m_vt->m_dtor(p, 1);
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CClean *fn_1_D86C(CClean *self, s16 flag)
{
    if (self != 0) {
        if (lbl_1_bss_1C != 0) {
        } else {
            fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_1C = 0;
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CClean *fn_1_D8F8(CClean *self, s16 flag)
{
    if (self != 0) {
        if (&self->m_side[1] != 0) {
            CCleanSide *p1 = self->m_side[1].m_ptr;
            if (p1 != 0) {
                p1->m_vt->m_dtor(p1, 1);
            }
        }
        if (self != 0) {
            CCleanSide *p0 = self->m_side[0].m_ptr;
            if (p0 != 0) {
                p0->m_vt->m_dtor(p0, 1);
            }
        }
        if (self != 0) {
            if (lbl_1_bss_1C != 0) {
            } else {
                fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
            }
            lbl_1_bss_1C = 0;
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// Winner check: mode 0 compares the cleaned percentages, otherwise a side wins by reaching 100%.
s32 fn_1_DA00(CClean *self, u8 mode)
{
    s32 a = (s32)(100.0f * fn_1_F744(self->m_side[0].m_ptr));
    s32 b = (s32)(100.0f * fn_1_F744(self->m_side[1].m_ptr));

    if (mode != 0) {
        if (a > b) {
            return 0;
        }
        if (a < b) {
            return 1;
        }
        return 2;
    }
    if (a >= 100 && b < 100) {
        return 0;
    }
    if (b >= 100 && a < 100) {
        return 1;
    }
    return 2;
}

CCleanSide *fn_1_DAEC(CClean *self, s32 side)
{
    switch (side) {
    case 1:
        return self->m_side[1].m_ptr;
    case 0:
        return self->m_side[0].m_ptr;
    default:
        return self->m_side[0].m_ptr;
    }
}

CCleanSide *fn_1_DB1C(CCleanSide *self, struct Process_s *objMan, f32 x0, f32 x1, f32 z0, f32 z1)
{
    s32 se;
    s32 state;
    f32 tmp;

    __ct__12CEffectModelFP9Process_sssUs(self, objMan, 2, 5, 1);
    self->m_vt = &lbl_1_data_4A0;
    self->m_total = 0x4000;
    self->m_thresh = 16.0f;
    self->m_dirty = 0;
    self->m_remain = 0;
    self->m_x0 = x0;
    self->m_x1 = x1;
    self->m_z0 = z0;
    self->m_z1 = z1;
    self->m_blend = 0.0f;
    self->m_stamp.m_cap = 0;
    self->m_stamp.m_size = 0;
    self->m_stamp.m_data = 0;
    self->m_stampRadius = 65.0f;
    self->m_stampW = 0;
    self->m_stampH = 0;
    self->m_scrollU = 0.0f;
    self->m_scrollV = 0.0f;
    if (x0 < 0.0f) {
        se = 0x7A8;
    } else {
        se = 0x7A9;
    }
    self->m_seId = se;
    self->m_seHandle = -1;
    if (x0 < 0.0f) {
        state = 0;
    } else {
        state = 5;
    }
    self->m_state = state;
    self->m_frame = 0;
    self->m_center.x = (x0 + x1) * 0.5f;
    self->m_center.y = 0.0f;
    tmp = 0.75f * z0;
    self->m_center.z = tmp + 0.25f * z1;
    fn_1_E010(self);
    fn_1_E31C(self);
    fn_1_E560(self);
    return self;
}

// Destructor chain of the brush vector: each class level inlines its base's destructor.
static inline void VecFree0(CFloatVec *self)
{
    f32 *p;
    f32 **b;
    f32 **a;
    a = &self->m_data;
    if (*a != 0) {
        b = &self->m_data;
        p = *b;
        __dl__FPv(p);
    }
}

static inline void VecFree1(CFloatVec *self)
{
    if (self != 0) {
        VecFree0(self);
    }
}

static inline void VecFree2(CFloatVec *self)
{
    if (self != 0) {
        VecFree1(self);
    }
}

CFloatVec *fn_1_DD90(CFloatVec *self, s16 flag)
{
    if (self != 0) {
        VecFree2(self);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CFloatVec *fn_1_DE24(CFloatVec *self, s16 flag)
{
    if (self != 0) {
        VecFree1(self);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CFloatVec *fn_1_DEB0(CFloatVec *self, s16 flag)
{
    if (self != 0) {
        VecFree0(self);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CCleanSide *fn_1_DF34(CCleanSide *self, s16 flag)
{
    if (self != 0) {
        self->m_vt = &lbl_1_data_4A0;
        fn_1_102E0(self);
        if (&self->m_stamp != 0) {
            VecFree2(&self->m_stamp);
        }
        __dt__12CEffectModelFv(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// Fills the 128x128 height grid with the dissolve pattern: a lobed disc that is 240 inside
// and fades out over the last 0.08 of the radius.
void fn_1_E010(CCleanSide *self)
{
    f32 *p;
    s32 count;
    f32 y;
    f32 x;
    f32 r;
    f32 h;
    f32 v;
    f64 d;
    f32 rc;
    f32 q;
    f32 at;
    f32 at2;
    f32 ang;
    f32 rsq;
    f32 xsq;

    p = &self->m_height[0][0];
    count = 0;
    for (y = -1.4921875f; y < 0.5f; y += 0.015625f) {
        for (x = -0.9921875f; x < 1.0; x += 0.015625f) {
            if (y < 0.0f) {
                r = y / -1.5f;
            } else {
                r = y / 0.5f;
            }
            rc = r;
            q = x / rc;
            at = (f32)atan(q);
            at2 = at;
            ang = 14.0f * at2;
            rsq = rc * rc;
            xsq = x * x;
            h = (f32)(0.05 * sin(ang) + 1.05 * (f64)(xsq + rsq));
            if (h < 1.0f) {
                if (h <= 0.9200000017881393) {
                    d = 240.0;
                } else {
                    d = 240.0 * cos(1.5707963267948966 * ((h - 0.9200000017881393) / 0.07999999821186066));
                }
            } else {
                d = 0.0;
            }
            v = (f32)d;
            if (v >= self->m_thresh) {
                count++;
            }
            *p = v;
            p++;
        }
    }
    self->m_remain = count;
    GXInitTexObj(&self->m_texMask, self->m_maskImg, 0x80, 0x80, GX_TF_I8, GX_CLAMP, GX_CLAMP, GX_FALSE);
}

// Builds the 16x32 IA8 distortion pattern (concentric rings) in tiled 4x4 order.
void fn_1_E31C(CCleanSide *self)
{
    s32 y;
    s32 x;
    s32 ofs;
    f32 fx;
    f32 fy;
    f32 t;

    for (y = 0; y < 0x20; y++) {
        for (x = 0; x < 0x10; x++) {
            ofs = ((x & 3) + (((x >> 2) * 0x10) + ((y & 3) * 4) + ((y >> 2) * 4 * 0x10))) * 2;
            fx = ((f32)(x * 2) * 0.0625f) - 1.0f;
            fy = ((f32)(y * 2) * 0.03125f) - 1.0f;
            t = (f32)(9.42477796076938 * (fx * fx + fy * fy));
            self->m_waveImg[ofs] = F2B(128.0 + 24.0 * sin(t));
            self->m_waveImg[ofs + 1] = F2B(128.0 + 48.0 * cos(t));
        }
    }
    DCFlushRange(self->m_waveImg, 0x400);
    GXInitTexObj(&self->m_texWave, self->m_waveImg, 0x10, 0x20, GX_TF_IA8, GX_REPEAT, GX_REPEAT, GX_FALSE);
}

void fn_1_E560(CCleanSide *self)
{
    self->m_anim = HuSprAnimRead(HuDataSelHeapReadNum(0x590005, 0x10000000, 2));
}

// Packs a 8x4 block of heights into 4 rows of 8 mask bytes; returns the number of cells >= thresh.
static inline s32 CleanPackBlock(u8 **dst, const f32 *src, s32 stride, s32 thresh)
{
    s32 count = 0;
    s32 row;
    s32 k;
    f32 v;
    s32 hit;
    u8 *out;

    for (row = 0; row < 4; row++) {
        out = *dst;
        for (k = 0; k < 8; k++) {
            v = src[k];
            out[k] = F2B(v);
            if (v >= (f32)thresh) {
                hit = 1;
            } else {
                hit = 0;
            }
            count += hit;
        }
        src += stride;
        *dst += 8;
    }
    return count;
}

s32 fn_1_E5A4(u8 **dst, const f32 *src, s32 stride, s32 thresh)
{
    return CleanPackBlock(dst, src, stride, thresh);
}

// State 4: rebuilds the I8 mask texture from the heights.
static inline void CleanPack(CCleanSide *self)
{
    u8 *out;
    s32 sum;
    s32 y;
    s32 x;

    if (self->m_state == 4) {
        out = self->m_maskImg;
        sum = 0;
        for (y = 0; y < CLEAN_GRID; y += 4) {
            for (x = 0; x < CLEAN_GRID; x += 8) {
                sum += CleanPackBlock(&out, &self->m_height[y][x], CLEAN_GRID, (s32)self->m_thresh);
            }
        }
        DCFlushRangeNoSync(self->m_maskImg, 0x4000);
        self->m_remain = sum;
    }
}

void fn_1_E694(CCleanSide *self)
{
    CleanPack(self);
}

// World position -> grid cell, clamped; returns nonzero when the position was outside the grid.
static inline s32 CellClamp(s32 v)
{
    s32 r;

    if (v < 0) {
        r = 0;
    } else if (v > 0x7F) {
        r = 0x7F;
    } else {
        r = v;
    }
    return r;
}

static inline u8 CleanCell(CCleanSide *self, const Vec *pos, s32 *outX, s32 *outZ)
{
    u8 outside;
    u8 zHigh;
    u8 zLow;

    *outX = (s32)((128.0f * (pos->x - self->m_x0)) / (self->m_x1 - self->m_x0));
    *outZ = (s32)((128.0f * (pos->z - self->m_z0)) / (self->m_z1 - self->m_z0));
    outside = 1;
    zHigh = 1;
    zLow = 1;
    if (*outX >= 0 && *outX < 0x80) {
        zLow = 0;
    }
    if (zLow == 0 && *outZ >= 0) {
        zHigh = 0;
    }
    if (zHigh == 0 && *outZ < 0x80) {
        outside = 0;
    }
    if (outside != 0) {
        *outX = CellClamp(*outX);
        *outZ = CellClamp(*outZ);
    }
    return outside;
}

u8 fn_1_E800(CCleanSide *self, const Vec *pos, s32 *outX, s32 *outZ)
{
    return CleanCell(self, pos, outX, outZ);
}

// (Re)builds the brush profile for the given radius: 1 - r^2 over a w x h block of cells.
void fn_1_E9C8(CCleanSide *self, f32 radius)
{
    Vec pos = { 0.0f, 0.0f, 0.0f };
    s32 w;
    s32 h;
    s32 evenW;
    f32 *p;
    f32 dx;
    f32 dy;
    f32 x;
    f32 y;
    f32 r2;
    f32 c;

    if (self->m_stamp.m_size == 0 || self->m_stampRadius != radius) {
        pos.x = self->m_x0 + radius;
        pos.z = self->m_z0 + radius;
        CleanCell(self, &pos, &w, &h);
        evenW = w & ~1;
        if (evenW < 2 || h < 2) {
            self->m_stamp.m_size = 0;
            self->m_stampRadius = 0.0f;
            self->m_stampW = 0;
            self->m_stampH = 0;
            return;
        }
        fn_1_110C8(&self->m_stamp, evenW * h, &lbl_1_data_49C);
        p = self->m_stamp.m_data;
        dx = (f32)(2.0 / (f64)evenW);
        dy = (f32)(2.0 / (f64)h);
        for (y = (f32)(-1.0 + (f64)dy * 0.5); y < 1.0; y += dy) {
            for (x = (f32)(-1.0 + (f64)dx * 0.5); x < 1.0; x += dx) {
                r2 = x * x + y * y;
                if (r2 < 1.0f) {
                    c = r2;
                } else {
                    c = 1.0f;
                }
                *p = 1.0f - c;
                p++;
            }
        }
        self->m_stampRadius = radius;
        self->m_stampW = evenW;
        self->m_stampH = h;
    }
}

// Applies the brush at pos: adds strength * profile to the heights under it (clamped to 0..255).
void fn_1_EE68(CCleanSide *self, const Vec *pos, f32 size, f32 strength)
{
    Vec lo = { 0.0f, 0.0f, 0.0f };
    Vec hi = { 0.0f, 0.0f, 0.0f };
    f32 r;
    s32 x0;
    s32 z0;
    s32 x1;
    s32 z1;
    s32 w;
    s32 h;
    s32 evenW;
    s32 i;
    s32 j;
    f32 *cell;
    f32 *data;
    f32 old0;
    f32 a;
    f32 b;
    f32 n0;
    f32 n1;
    u8 changed;

    if (self->m_state == 0) {
        r = (f32)fabs(size);
        fn_1_E9C8(self, r);
        lo.x = pos->x - r * 0.5f;
        lo.z = pos->z - r * 0.5f;
        hi.x = pos->x + r * 0.5f;
        hi.z = pos->z + r * 0.5f;
        CleanCell(self, &lo, &x0, &z0);
        CleanCell(self, &hi, &x1, &z1);
        if (x0 == x1 || z0 == z1) {
            return;
        }
        w = (x1 - x0) + 1;
        if (!(w < self->m_stampW)) {
            w = self->m_stampW;
        }
        evenW = w & ~1;
        h = (z1 - z0) + 1;
        if (!(h < self->m_stampH)) {
            h = self->m_stampH;
        }
        cell = &self->m_height[z0][x0];
        data = self->m_stamp.m_data;
        if (evenW < self->m_stampW && x0 == 0) {
            data += self->m_stampW - evenW;
        }
        if (h < self->m_stampH && z0 == 0) {
            data += self->m_stampW * (self->m_stampH - h);
        }
        changed = 0;
        for (j = 0; j < h; j++) {
            for (i = 0; i < evenW; i += 2) {
                old0 = cell[i];
                a = old0 + strength * data[i];
                if (a < 0.0f) {
                    n0 = 0.0f;
                } else if (a > 255.0f) {
                    n0 = 255.0f;
                } else {
                    n0 = a;
                }
                b = cell[i + 1] + strength * data[i + 1];
                if (b < 0.0f) {
                    n1 = 0.0f;
                } else if (b > 255.0f) {
                    n1 = 255.0f;
                } else {
                    n1 = b;
                }
                cell[i + 1] = n1;
                cell[i] = n0;
                if (old0 != n0 || old0 != n1) {
                    changed = 1;
                }
            }
            data += self->m_stampW;
            cell += CLEAN_GRID;
        }
        self->m_dirty = changed;
    }
}

void fn_1_F624(CCleanSide *self, const Vec *pos, f32 size)
{
    fn_1_EE68(self, pos, size, 32.0f);
}

void fn_1_F668(CCleanSide *self, const Vec *pos, f32 size)
{
    fn_1_EE68(self, pos, size, -160.0f);
}

void fn_1_F6AC(CCleanSide *self, const Vec *pos)
{
    self->m_center.x = pos->x;
    self->m_center.y = pos->y;
    self->m_center.z = pos->z;
}

void fn_1_F6D4(CCleanSide *self, f32 value)
{
    f32 v;

    if (value < 0.0f) {
        v = 0.0f;
    } else if (value > 1.0f) {
        v = 1.0f;
    } else {
        v = value;
    }
    self->m_blend = v;
}

f32 fn_1_F744(CCleanSide *self)
{
    return 1.0f - ((f32)self->m_remain / (f32)self->m_total);
}

// Fraction of the cells under the rectangle (a, b) that are still dirty.
f32 fn_1_F7BC(CCleanSide *self, const Vec *a, const Vec *b)
{
    Vec lo;
    Vec hi;
    f32 minX;
    f32 minZ;
    f32 maxX;
    f32 maxZ;
    s32 x0;
    s32 z0;
    s32 x1;
    s32 z1;
    s32 x;
    s32 z;
    f32 total;
    f32 left;
    s32 clean;

    minZ = a->z < b->z ? a->z : b->z;
    minX = a->x < b->x ? a->x : b->x;
    lo.x = minX;
    lo.y = 0.0f;
    lo.z = minZ;
    maxZ = a->z > b->z ? a->z : b->z;
    maxX = a->x > b->x ? a->x : b->x;
    hi.x = maxX;
    hi.y = 0.0f;
    hi.z = maxZ;
    CleanCell(self, &lo, &x0, &z0);
    CleanCell(self, &hi, &x1, &z1);
    if (x0 == x1 || z0 == z1) {
        return 1.0f;
    }
    total = (f32)((x1 - x0) * (z1 - z0));
    left = total;
    for (z = z0; z < z1; z++) {
        for (x = x0; x < x1; x++) {
            if (self->m_height[z][x] < self->m_thresh) {
                clean = 0;
            } else {
                clean = 1;
            }
            left -= (f32)clean;
        }
    }
    return left / total;
}

// State 2: dirt that touches a clean cell erodes by 3.1875 per tick.
static inline void CleanErode(CCleanSide *self)
{
    s32 count;
    s32 z;
    s32 x;
    f32 *cell;
    f32 left;
    f32 right;
    f32 up;
    f32 down;
    f32 v;
    f32 nv;
    f32 nv2;
    f32 nv3;
    f32 left2;
    f32 right2;
    f32 up2;
    f32 down2;

    if (self->m_state == 2) {
        count = 0;
        for (z = 0; z < CLEAN_GRID; z++) {
            for (x = 0; x < CLEAN_GRID; x++) {
                cell = &self->m_height[z][x];
                if (*cell > 0.0f) {
                    if (x > 0) {
                        left = self->m_height[z][x - 1];
                    } else {
                        left = 0.0f;
                    }
                    if (x < 0x7F) {
                        right = self->m_height[z][x + 1];
                    } else {
                        right = 0.0f;
                    }
                    if (z > 0) {
                        up = self->m_height[z - 1][x];
                    } else {
                        up = 0.0f;
                    }
                    if (z < 0x7F) {
                        down = self->m_height[z + 1][x];
                    } else {
                        down = 0.0f;
                    }
                    left2 = left;
                    right2 = right;
                    up2 = up;
                    down2 = down;
                    if (left2 <= 0.0f || right2 <= 0.0f || up2 <= 0.0f || down2 <= 0.0f) {
                        v = *cell - 3.1875f;
                        if (0.0f > v) {
                            nv = 0.0f;
                        } else {
                            nv = v;
                        }
                        nv2 = nv;
                        nv3 = nv2;
                        *cell = nv3;
                    }
                    if (*cell >= self->m_thresh) {
                        count++;
                    }
                }
            }
        }
        self->m_remain = count;
    }
}

void fn_1_FDC8(CCleanSide *self)
{
    CleanErode(self);
}

// Scrolls the distortion texture along a slowly turning direction.
static inline void CleanScroll(CCleanSide *self)
{
    f32 t;

    t = (f32)(6.283185307179586 * (f64)((f32)self->m_frame / 7200.0f));
    self->m_scrollU = (f32)((f64)self->m_scrollU + 0.0005f * sin(t));
    self->m_scrollV = (f32)((f64)self->m_scrollV + 0.0005f * cos(t));
    if (self->m_scrollU > 1.0f) {
        self->m_scrollU = (f32)(self->m_scrollU - 1.0f);
    } else if (self->m_scrollU < 0.0f) {
        self->m_scrollU = (f32)(self->m_scrollU + 1.0f);
    }
    if (self->m_scrollV > 1.0f) {
        self->m_scrollV = (f32)(self->m_scrollV - 1.0f);
        return;
    }
    if (self->m_scrollV < 0.0f) {
        self->m_scrollV = (f32)(self->m_scrollV + 1.0f);
    }
}

void fn_1_100CC(CCleanSide *self)
{
    CleanScroll(self);
}

void fn_1_10294(CCleanSide *self)
{
    if (self->m_seHandle < 0) {
        self->m_seHandle = HuAudFXPlay(self->m_seId);
    }
}

void fn_1_102E0(CCleanSide *self)
{
    if (self->m_seHandle >= 0) {
        HuAudFXStop(self->m_seHandle);
        self->m_seHandle = -1;
    }
}

// Per-frame update: a six-state cycle (0 brush sound, 2 erosion, 4 mask rebuild), then the scroll.
void fn_1_10330(CCleanSide *self)
{
    if (self->m_state == 0) {
        if (self->m_dirty != 0) {
            if (self->m_seHandle < 0) {
                self->m_seHandle = HuAudFXPlay(self->m_seId);
            }
        } else if (self->m_seHandle >= 0) {
            HuAudFXStop(self->m_seHandle);
            self->m_seHandle = -1;
        }
        self->m_dirty = 0;
    }
    CleanErode(self);
    CleanPack(self);
    CleanScroll(self);
    self->m_state++;
    if (self->m_state >= 6) {
        self->m_state = 0;
    }
    self->m_frame++;
}

// Draw hook: the mask is drawn through an indirect warp (distortion texture) over the board quad.
void fn_1_109C0(CCleanSide *self, Mtx mtx)
{
    static const f32 kIndMtx[2][3] = { { 0.1f, 0.0f, 0.0f }, { 0.0f, 0.1f, 0.0f } };
    GXColor color = { 0x3B, 0xB5, 0x3B, 0x18 };
    f32 t;
    f32 x0;
    f32 x1;
    f32 z0;
    f32 z1;
    f32 y;
    f32 u0;
    f32 v0;
    f32 u1;
    f32 v1;

    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetNumChans(0);
    HuSprTexLoad(self->m_anim, 0, 0, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    GXLoadTexObj(&self->m_texWave, GX_TEXMAP1);
    GXLoadTexObj(&self->m_texMask, GX_TEXMAP2);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumIndStages(1);
    GXSetIndTexMtx(GX_ITM_0, (f32 (*)[3])kIndMtx, 0);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD0, GX_TEXMAP1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_8, GX_ITS_4);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetNumTevStages(2);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_TEXC, GX_CC_A0, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVREG1);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVREG1);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP2, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C1);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetCullMode(GX_CULL_NONE);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, (s32)self->m_thresh, GX_AOP_AND, GX_LEQUAL, 0xFF);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0);
    t = self->m_blend;
    x0 = (f32)((1.0 - (f64)t) * (f64)self->m_center.x + (f64)t * (f64)self->m_x0);
    x1 = (f32)((1.0 - (f64)t) * (f64)self->m_center.x + (f64)t * (f64)self->m_x1);
    z0 = (f32)((1.0 - (f64)t) * (f64)self->m_center.z + (f64)t * (f64)self->m_z0);
    z1 = (f32)((1.0 - (f64)t) * (f64)self->m_center.z + (f64)t * (f64)self->m_z1);
    y = (f32)((1.0 - (f64)t) * (f64)self->m_center.y + (f64)t * 0.0);
    u0 = self->m_scrollU;
    v0 = self->m_scrollV;
    u1 = (f32)(0.5 + (f64)u0);
    v1 = (f32)(1.0 + (f64)v0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    VTX_POS3(x0, y, z0);
    VTX_TEX2(0.0f, 0.0f);
    VTX_TEX2(u0, v0);
    VTX_POS3(x1, y, z0);
    VTX_TEX2(1.0f, 0.0f);
    VTX_TEX2(u1, v0);
    VTX_POS3(x1, y, z1);
    VTX_TEX2(1.0f, 1.0f);
    VTX_TEX2(u1, v1);
    VTX_POS3(x0, y, z1);
    VTX_TEX2(0.0f, 1.0f);
    VTX_TEX2(u0, v1);
    VTX_END();
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetNumIndStages(0);
}

// vector<float>::resize
void fn_1_110C8(CFloatVec *vec, u32 size, const f32 *value)
{
    if (size > vec->m_size) {
        fn_1_11220(vec, vec->m_data + vec->m_size, size - vec->m_size, value);
        return;
    }
    if (size < vec->m_size) {
        fn_1_11178(vec, vec->m_data + size, vec->m_data + vec->m_size);
    }
}

// vector<float>::erase(first, last)
f32 *fn_1_11178(CFloatVec *vec, f32 *first, f32 *last)
{
    f32 *end;
    f32 *dst;
    f32 *src;

    if (first == last) {
        return first;
    }
    end = vec->m_data + vec->m_size;
    if ((end - last) != 0) {
        dst = first;
        src = last;
        while (src < end) {
            *dst = *src;
            src++;
            dst++;
        }
    }
    vec->m_size -= (u32)(last - first);
    return first;
}

// Accessors and copy/fill primitives of the original STL, inlined into insert.
static inline u32 VecMax(CFloatVec *vec)
{
    u32 m = 0x3FFFFFFF;
    u32 m2 = m;
    return m2;
}

static inline u32 VecCap(CFloatVec *vec)
{
    CFloatVec *t = vec;
    u32 c = t->m_cap;
    return c;
}

static inline f32 *VecBegin(CFloatVec *vec)
{
    f32 **p = &vec->m_data;
    return *p;
}

static inline f32 *VecEnd(CFloatVec *vec)
{
    f32 **p = &vec->m_data;
    return *p + vec->m_size;
}

static inline f32 *VecCopy(f32 *first, f32 *last, f32 *dst)
{
    f32 *s = first;
    f32 *d = dst;
    while (s < last) {
        *d = *s;
        s++;
        d++;
    }
    return d;
}

static inline void VecFill(f32 *dst, u32 n, const f32 *value)
{
    u32 cnt = n;
    f32 *d = dst;
    while (cnt != 0) {
        *d = *value;
        d++;
        cnt--;
    }
}

// vector<float>::insert(pos, count, value)
void fn_1_11220(CFloatVec *vec, f32 *pos, u32 count, const f32 *value)
{
    u32 maxSize;
    f32 *end;
    s32 moveCount;
    const f32 *src;
    f32 *from;
    f32 *to;
    f32 *oldData;
    s32 index;
    u32 newSize;
    u32 newCap;
    f32 *newData;
    f32 *dst;

    if (count == 0) {
        return;
    }
    maxSize = VecMax(vec);
    if (count > maxSize || vec->m_size > maxSize - count) {
        fn_1_234(CLEAN_STDERR, "vector::insert length error\n");
        abort();
    }
    if (vec->m_size + count <= VecCap(vec)) {
        end = VecEnd(vec);
        moveCount = (s32)(end - pos);
        src = value;
        if ((end - (moveCount - (s32)count)) <= src && src < end) {
            src = src + count;
        }
        if (moveCount != 0) {
            from = end;
            to = end + count;
            while (from > pos) {
                from--;
                to--;
                *to = *from;
            }
        }
        VecFill(pos, count, src);
        vec->m_size += count;
        return;
    }
    oldData = VecBegin(vec);
    index = (s32)(pos - oldData);
    newSize = vec->m_size + count;
    if (VecCap(vec) != 0) {
        newCap = VecCap(vec);
    } else {
        newCap = 1;
    }
    while (newSize > newCap) {
        if (newCap < (maxSize >> 1)) {
            newCap *= 2;
        } else {
            newCap = maxSize;
        }
    }
    newData = (f32 *)fn_1_0(newCap * sizeof(f32));
    if (newData == 0) {
        fn_1_234(CLEAN_STDERR, "Memory allocation failure");
        abort();
    }
    vec->m_data = newData;
    VecFill(VecBegin(vec) + index, count, value);
    if (oldData != 0) {
        dst = VecCopy(oldData, oldData + index, VecBegin(vec));
        dst = VecCopy(oldData + index, oldData + vec->m_size, VecBegin(vec) + index + count);
        __dl__FPv(oldData);
    }
    vec->m_size = newSize;
    vec->m_cap = newCap;
}

}
