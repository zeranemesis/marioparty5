#include "REL/m509Dll/m509.h"

extern "C" {
#include "dolphin/gx.h"
void HuAudFXPlayVol(s32 id, s32 vol);
s32 HuAudFXPlay(s32 id);
void HuAudFXStop(s32 id);
s32 irandom__7CRandomFv(void);
}

/* one cell of the 16 x 12 grid: wall bits (0x40/0x80), a marker and the countdowns that grow walls */
typedef struct MazeCell {
    u8 wall;
    u8 mark;
    u8 fade;
    u8 vTimer;
    u8 hTimer;
} MazeCell;

/* a walker that carves the wall pattern while the intro runs */
typedef struct MazeWalker {
    u8 state;
    u8 dir;
    u8 unused;
    u8 row;
    u8 col;
} MazeWalker;

typedef struct Maze {
    M509Base base;
    s16 hook[2];
    u8 dirty;
    u32 phase;
    s32 alpha;
    u8 busy;
    s32 busyTimer;
    s32 busyParam;
    s32 timer;
    s32 hold;
    s32 sfx;
    MazeCell cell[16][12];
    MazeWalker walker[3];
    u64 seedSaved;
    u64 seed;
} Maze;

extern "C" {
void *fn_1_5A7C(Maze *self);
void *fn_1_5CA8(Maze *self, s16 flag);
void *fn_1_5D34(Maze *self, s16 flag);
void *fn_1_5D8C(Maze *self, s16 flag);
void fn_1_5E68(Maze *self, s32 bit);
void fn_1_5EDC(Maze *self);
void fn_1_63D8(Maze *self);
s32 fn_1_6524(Maze *self, s32 dir, s32 i, s32 j);
s32 fn_1_666C(Maze *self, s32 dir, s32 i, s32 j);
s32 fn_1_67BC(Maze *self, s32 dir, s32 i, s32 j);
s32 fn_1_6904(Maze *self, s32 dir, s32 i, s32 j);
void fn_1_6A58(Maze *self);
void fn_1_7244(Maze *self, s32 step);
void fn_1_73A4(Maze *self, s8 param);
void fn_1_73C8(Maze *self, s32 val, s32 i, s32 j);
void fn_1_7484(Maze *self);
void fn_1_785C(HU3DMODEL *model, Mtx *mtx);
void fn_1_78AC(HU3DMODEL *model, Mtx *mtx);
void fn_1_78FC(Maze *self, s32 i, s32 j);
void fn_1_8094(Maze *self, s32 i, s32 j);
void fn_1_8828(Maze *self);
void fn_1_89C0(Maze *self, s32 i, s32 j);
void fn_1_8F80(Maze *self, s32 i, s32 j);
void fn_1_9540(Maze *self);
void fn_1_96E8(Maze *self, u8 r, u8 g, u8 b, u8 a, f32 x, f32 z);
void fn_1_A100(Maze *self);
void fn_1_A3C0(Maze *self, u8 type, f32 x, f32 z);
void fn_1_A6EC(Maze *self);
void fn_1_ABC4(Maze *self, s32 alpha);
void fn_1_AEAC(Maze *self, Mtx *mtx);
void fn_1_AEF8(Maze *self, Mtx *mtx);
void fn_1_B524(Maze *self);
void fn_1_B6C8(Maze *self);
void fn_1_B86C(Maze *self, u8 *src, s32 flags);
f32 fn_1_BCAC(Maze *self);
void fn_1_BD44(Maze *self, s32 val, f32 x, f32 z);
u8 fn_1_BE30(Maze *self, f32 x, f32 z);
u8 fn_1_BEE8(Maze *self, f32 x, f32 z);
s32 fn_1_C000(Maze *self, f32 x, f32 z);
void fn_1_C224(void *unused, s32 idx);
extern u8 lbl_1_data_210[0x3000];
}

char lbl_1_data_3210[] = "ms_pointer == 0";
char lbl_1_data_3220[] = "singleton.h";
char lbl_1_data_322C[] = "ms_pointer != 0";
u8 lbl_1_data_3258[4][4] = { { 0, 0, 2, 3 }, { 1, 1, 2, 3 }, { 0, 1, 2, 2 }, { 0, 1, 3, 3 } };
void *lbl_1_data_3268[6] = { 0, 0, (void *)fn_1_5D8C, (void *)fn_1_5E68, (void *)fn_1_5EDC, 0 };

void *lbl_1_bss_30;
Maze *lbl_1_bss_34;

XX_RODATA const Vec lbl_1_rodata_134[16] = {
    { 0.0f, 0.0f, -80.0f }, { -40.0f, 0.0f, 80.0f }, { 0.0f, 0.0f, 40.0f }, { 40.0f, 0.0f, 80.0f },
    { 0.0f, 0.0f, 80.0f }, { -40.0f, 0.0f, -80.0f }, { 0.0f, 0.0f, -40.0f }, { 40.0f, 0.0f, -80.0f },
    { -80.0f, 0.0f, 0.0f }, { 80.0f, 0.0f, 40.0f }, { 40.0f, 0.0f, 0.0f }, { 80.0f, 0.0f, -40.0f },
    { 80.0f, 0.0f, 0.0f }, { -80.0f, 0.0f, 40.0f }, { -40.0f, 0.0f, 0.0f }, { -80.0f, 0.0f, -40.0f },
};
XX_RODATA const GXColor lbl_1_rodata_FC[14] = {
    { 0xFE, 0x00, 0x00, 0xFF }, { 0x00, 0x00, 0xFE, 0xFF }, { 0xFF, 0x7B, 0xDE, 0xFF }, { 0x00, 0xFF, 0x00, 0xFF },
    { 0x7D, 0x00, 0xBE, 0xFF }, { 0xFF, 0xFF, 0x00, 0xFF }, { 0x3F, 0x43, 0x8C, 0xFF }, { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0x00, 0xFF, 0xFF, 0xFF }, { 0xFF, 0x60, 0x16, 0xFF }, { 0xFF, 0x50, 0x50, 0xFF }, { 0x50, 0xFF, 0x50, 0xFF },
    { 0x50, 0x50, 0xFF, 0xFF }, { 0xFF, 0xFF, 0xFF, 0xFF },
};
XX_RODATA const u8 lbl_1_rodata_204[6] = { 0, 1, 2, 4, 3, 5 };
XX_RODATA const u8 lbl_1_rodata_20A[6] = { 0, 2, 1, 3, 4, 5 };

/* state shared by every draw routine below */
#define MAZE_GX_BEGIN_STATE() \
    GXSetNumTevStages(1); \
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE); \
    GXSetNumChans(1); \
    GXSetNumTexGens(0); \
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0); \
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR)
#define MAZE_GX_VTX_STATE() \
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0); \
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0); \
    GXClearVtxDesc(); \
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT); \
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT); \
    GXSetLineWidth(6, GX_TO_ZERO)

void *fn_1_5A7C(Maze *self)
{
    s32 i;
    s32 j;
    s32 k;
    u8 *q;

    fn_1_E48(self);
    self->base.vtbl = (void **)lbl_1_data_1C4;
    self->base.flags = 0;
    fn_1_F14(self, lbl_1_bss_1C);
    lbl_1_bss_34 == 0 ? (void)0 : fn_1_0(lbl_1_data_3210, lbl_1_data_3220, 0x35);
    q = (u8 *)self + 0x10;
    if (q != 0) {
        q -= 0x10;
    }
    lbl_1_bss_34 = (Maze *)q;
    self->base.vtbl = lbl_1_data_3268;
    self->seedSaved = 0x368B9170;
    self->seed = 0x368B9170;
    self->phase = 0;
    self->alpha = 0;
    self->timer = 0;
    self->hold = 0;
    self->busy = 0;
    self->dirty = 0;
    self->hook[0] = Hu3DHookFuncCreate(fn_1_785C);
    Hu3DModelLayerSet(self->hook[0], 3);
    Hu3DModelCameraSet(self->hook[0], 1);
    self->hook[1] = Hu3DHookFuncCreate(fn_1_78AC);
    Hu3DModelLayerSet(self->hook[1], 4);
    Hu3DModelCameraSet(self->hook[1], 1);
    for (j = 0; j < 12; j++) {
        for (i = 0; i < 16; i++) {
            self->cell[i][j].wall = 0;
            self->cell[i][j].mark = 0;
            self->cell[i][j].hTimer = 0;
            self->cell[i][j].vTimer = 0;
            self->cell[i][j].fade = 0;
        }
    }
    for (k = 0; k < 3; k++) {
        self->walker[k].state = 1;
    }
    self->sfx = HuAudFXPlay(0x690);
    return self;
}

void *fn_1_5CA8(Maze *self, s16 flag)
{
    if (self != 0) {
        lbl_1_bss_34 != 0 ? (void)0 : fn_1_0(lbl_1_data_322C, lbl_1_data_3220, 0x3D);
        lbl_1_bss_34 = 0;
        if (flag > 0) {
            fn_1_21C(self);
        }
    }
    return self;
}

void *fn_1_5D34(Maze *self, s16 flag)
{
    if (self != 0) {
        fn_1_E54(self, 0);
        if (flag > 0) {
            fn_1_21C(self);
        }
    }
    return self;
}

void *fn_1_5D8C(Maze *self, s16 flag)
{
    if (self != 0) {
        self->base.vtbl = lbl_1_data_3268;
        Hu3DModelKill(self->hook[0]);
        Hu3DModelKill(self->hook[1]);
        if ((u8 *)self + 0x10 != 0) {
            lbl_1_bss_34 != 0 ? (void)0 : fn_1_0(lbl_1_data_322C, lbl_1_data_3220, 0x3D);
            lbl_1_bss_34 = 0;
        }
        if (self != 0) {
            self->base.vtbl = (void **)lbl_1_data_1C4;
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

void fn_1_5E68(Maze *self, s32 bit)
{
    self->base.flags &= ~(1 << bit);
    if (bit != 3) {
        if (bit < 3) {
            return;
        }
        return;
    }
    HuAudFXStop(self->sfx);
    self->sfx = -1;
}

void fn_1_5EDC(Maze *self)
{
    s32 i;
    s32 j;

    if ((self->timer & 1) == 0) {
        self->seedSaved = self->seed;
    }
    if (self->busy != 0) {
        fn_1_7484(self);
    }
    for (j = 0; j < 12; j++) {
        for (i = 0; i < 16; i++) {
            self->cell[i][j].mark &= 0xEF;
            if ((self->cell[i][j].mark & 0x80) != 0) {
                s32 v = self->cell[i][j].fade + 0x10;
                if (v > 0xFF) {
                    self->cell[i][j].mark &= 0x7F;
                    v = 0xFF;
                }
                self->cell[i][j].fade = v;
            } else {
                u8 f = self->cell[i][j].fade;
                if (f != 0) {
                    s32 step;
                    s32 v;
                    if (self->busy != 0) {
                        step = 8;
                    } else {
                        step = 2;
                    }
                    v = f - step;
                    if (v < 0) {
                        v = 0;
                    }
                    self->cell[i][j].fade = v;
                }
            }
        }
    }
    switch (self->phase) {
    case 0:
        if ((f32)self->timer > 150.0f) {
            self->phase++;
            HuAudFXPlayVol(0x68F, 0x48);
        }
        self->alpha = 0xC0;
        break;
    case 1:
        if ((f32)self->timer > 156.0f) {
            self->phase++;
            HuAudFXPlayVol(0x68F, 0x48);
        }
        self->alpha = (s32)((f64)(192.0f * ((f32)self->timer - 150.0f)) / 6.0);
        break;
    case 2:
        if ((f32)self->timer > 162.0f) {
            self->phase++;
        }
        self->alpha = (s32)((f64)(192.0f * ((f32)self->timer - 156.0f)) / 6.0);
        break;
    case 3:
        if ((f32)self->timer > 174.0f) {
            self->phase++;
            HuAudFXPlayVol(0x68F, 0x48);
        }
        self->alpha = 0xC0;
        break;
    case 4:
        if ((f32)self->timer > 180.0f) {
            self->phase++;
        }
        self->alpha = (s32)((f64)(192.0f * ((f32)self->timer - 174.0f)) / 6.0);
        break;
    case 5:
        self->alpha = 0;
        fn_1_63D8(self);
        self->phase++;
        break;
    }
    if ((s32)self->phase < 5) {
        fn_1_7244(self, self->phase);
    }
    self->timer++;
    if (self->hold > 0) {
        self->hold--;
    }
}

void fn_1_63D8(Maze *self)
{
    s32 i;
    s32 j;

    for (j = 0; j < 12; j++) {
        for (i = 0; i < 16; i++) {
            self->cell[i][j].wall = 0;
            self->cell[i][j].mark = 0;
            self->cell[i][j].hTimer = 0;
            self->cell[i][j].vTimer = 0;
            self->cell[i][j].fade = 0;
        }
    }
    for (i = 0; i < 15; i++) {
        self->cell[i][0].wall |= 0x40;
        self->cell[i][11].wall |= 0x40;
    }
    for (j = 0; j < 11; j++) {
        self->cell[0][j].wall |= 0x80;
        self->cell[15][j].wall |= 0x80;
    }
    self->hold = 0x14;
}

#define MAZE_W(i_, j_) (self->cell[i_][j_].wall)

s32 fn_1_6524(Maze *self, s32 dir, s32 i, s32 j)
{
    if (j <= 0) {
        return 1;
    }
    switch (dir) {
    case 0:
        if ((MAZE_W(i, j) & 0x40) != 0) {
            return 1;
        }
        if (i > 0 && (MAZE_W(i - 1, j) & 0x40) != 0) {
            return 1;
        }
        return 0;
    case 2:
        if ((MAZE_W(i, j) & 0x80) != 0) {
            return 1;
        }
        if ((MAZE_W(i, j - 1) & 0x80) != 0) {
            return 1;
        }
        return 0;
    case 3:
        if ((MAZE_W(i + 1, j) & 0x80) != 0) {
            return 1;
        }
        if ((MAZE_W(i + 1, j - 1) & 0x80) != 0) {
            return 1;
        }
        return 0;
    default:
        return 0;
    }
}

s32 fn_1_666C(Maze *self, s32 dir, s32 i, s32 j)
{
    if (j >= 10) {
        return 1;
    }
    switch (dir) {
    case 1:
        if ((MAZE_W(i, j + 1) & 0x40) != 0) {
            return 1;
        }
        if (i > 0 && (MAZE_W(i - 1, j + 1) & 0x40) != 0) {
            return 1;
        }
        return 0;
    case 2:
        if ((MAZE_W(i, j) & 0x80) != 0) {
            return 1;
        }
        if ((MAZE_W(i, j - 1) & 0x80) != 0) {
            return 1;
        }
        return 0;
    case 3:
        if ((MAZE_W(i + 1, j) & 0x80) != 0) {
            return 1;
        }
        if ((MAZE_W(i + 1, j - 1) & 0x80) != 0) {
            return 1;
        }
        return 0;
    default:
        return 0;
    }
}

s32 fn_1_67BC(Maze *self, s32 dir, s32 i, s32 j)
{
    if (i <= 0) {
        return 1;
    }
    switch (dir) {
    case 2:
        if ((MAZE_W(i, j) & 0x80) != 0) {
            return 1;
        }
        if (j > 0 && (MAZE_W(i, j - 1) & 0x80) != 0) {
            return 1;
        }
        return 0;
    case 0:
        if ((MAZE_W(i, j) & 0x40) != 0) {
            return 1;
        }
        if ((MAZE_W(i - 1, j) & 0x40) != 0) {
            return 1;
        }
        return 0;
    case 1:
        if ((MAZE_W(i, j + 1) & 0x40) != 0) {
            return 1;
        }
        if ((MAZE_W(i - 1, j + 1) & 0x40) != 0) {
            return 1;
        }
        return 0;
    default:
        return 0;
    }
}

s32 fn_1_6904(Maze *self, s32 dir, s32 i, s32 j)
{
    if (i >= 14) {
        return 1;
    }
    switch (dir) {
    case 3:
        if ((MAZE_W(i + 1, j) & 0x80) != 0) {
            return 1;
        }
        if (j > 0 && (MAZE_W(i + 1, j - 1) & 0x80) != 0) {
            return 1;
        }
    case 2:
        return 0;
    case 0:
        if ((MAZE_W(i, j) & 0x40) != 0) {
            return 1;
        }
        if ((MAZE_W(i - 1, j) & 0x40) != 0) {
            return 1;
        }
        return 0;
    case 1:
        if ((MAZE_W(i, j + 1) & 0x40) != 0) {
            return 1;
        }
        if ((MAZE_W(i - 1, j + 1) & 0x40) != 0) {
            return 1;
        }
        return 0;
    default:
        return 0;
    }
}

void fn_1_6A58(Maze *self)
{
    s32 k;
    u8 blocked;
    u8 prev;
    u8 dir;
    u8 row;
    u8 col;

    for (k = 0; k < 3; k++) {
        if (self->walker[k].state == 1) {
            self->walker[k].state = 0;
            self->walker[k].dir = irandom__7CRandomFv() & 3;
            self->walker[k].row = irandom__7CRandomFv() % 15;
            self->walker[k].col = irandom__7CRandomFv() % 11;
        }
    }
    for (k = 0; k < 3; k++) {
        prev = self->walker[k].dir;
        dir = lbl_1_data_3258[prev][irandom__7CRandomFv() & 3];
        self->walker[k].dir = dir;
        row = self->walker[k].row;
        col = self->walker[k].col;
        switch (dir) {
        case 0:
            blocked = fn_1_6524(self, prev, row, col);
            if (blocked != 0) {
                self->walker[k].state = 1;
            } else {
                col -= 1;
                if (prev == 3) {
                    row += 1;
                }
            }
            break;
        case 1:
            blocked = fn_1_666C(self, prev, row, col);
            if (blocked != 0) {
                self->walker[k].state = 1;
            } else {
                if (prev == 1) {
                    col += 1;
                }
                if (prev == 3) {
                    row += 1;
                }
            }
            break;
        case 2:
            blocked = fn_1_67BC(self, prev, row, col);
            if (blocked != 0) {
                self->walker[k].state = 1;
            } else {
                row -= 1;
                if (prev == 1) {
                    col += 1;
                }
            }
            break;
        case 3:
            blocked = fn_1_6904(self, prev, row, col);
            if (blocked != 0) {
                self->walker[k].state = 1;
            } else {
                if (prev == 1) {
                    col += 1;
                }
                if (prev == 3) {
                    row += 1;
                }
            }
            break;
        }
        self->walker[k].row = row;
        self->walker[k].col = col;
        if (self->walker[k].state == 0) {
            if (dir < 2) {
                self->cell[row][col].hTimer = 8;
            } else {
                self->cell[row][col].vTimer = 8;
            }
        }
    }
}

void fn_1_7244(Maze *self, s32 step)
{
    s32 i;
    s32 j;

    if (step == 0) {
        fn_1_6A58(self);
    }
    self->dirty = 0;
    for (j = 0; j <= 11; j++) {
        for (i = 0; i <= 15; i++) {
            self->cell[i][j].wall &= 0x3F;
            if (self->cell[i][j].vTimer != 0) {
                self->cell[i][j].vTimer--;
                self->cell[i][j].wall |= 0x40;
                self->dirty = 1;
            }
            if (self->cell[i][j].hTimer != 0) {
                self->cell[i][j].hTimer--;
                self->cell[i][j].wall |= 0x80;
                self->dirty = 1;
            }
        }
    }
}

void fn_1_73A4(Maze *self, s8 param)
{
    if (self->busy == 0) {
        self->busy = 1;
        self->busyTimer = 0;
        self->busyParam = param;
    }
}

void fn_1_73C8(Maze *self, s32 val, s32 i, s32 j)
{
    if (i < 0 || i >= 15 || j < 0 || j >= 11) {
        return;
    }
    self->cell[i][j].wall = 5;
    self->cell[i + 1][j].wall &= 0x7F;
    self->cell[i][j + 1].wall &= 0xBF;
    self->cell[i][j].mark = val | 0x80;
    self->cell[i][j].fade = 0;
}

void fn_1_7484(Maze *self)
{
    s32 r;
    s32 i;
    s32 j;

    if ((self->busyTimer & 7) == 0) {
        r = self->busyTimer >> 3;
        for (i = 7 - r; i <= r + 7; i++) {
            fn_1_73C8(self, self->busyParam, i, 5 - r);
            fn_1_73C8(self, self->busyParam, i, r + 5);
        }
        for (j = 5 - r; j <= r + 5; j++) {
            fn_1_73C8(self, self->busyParam, 7 - r, j);
            fn_1_73C8(self, self->busyParam, r + 7, j);
        }
    }
    self->busyTimer++;
}

void fn_1_785C(HU3DMODEL *model, Mtx *mtx)
{
    Maze *self;
    Maze *self2;

    Hu3DModelObjDrawInit();
    self = lbl_1_bss_34;
    self2 = self;
    fn_1_AEAC(self2, mtx);
}

void fn_1_78AC(HU3DMODEL *model, Mtx *mtx)
{
    Maze *self;
    Maze *self2;

    Hu3DModelObjDrawInit();
    self = lbl_1_bss_34;
    self2 = self;
    fn_1_AEF8(self2, mtx);
}

void fn_1_78FC(Maze *self, s32 i, s32 j)
{
    Vec v[5];
    Vec base;
    f32 scale;
    f32 half;
    f32 t;
    f32 x0;
    f32 x1;
    f32 x3;
    s32 k;

    base.x = (220.0f * (f32)i) - 1650.0f;
    base.y = 100.0f * (2.0f + fn_1_BCAC(self));
    base.z = (220.0f * (f32)j) - 1210.0f;
    scale = 0.8f + ((f32)self->hold / 10.0f);
    half = scale * 0.5f;
    for (k = 0; k < 3; k++) {
        t = (0.6f * fn_1_BCAC(self)) - 0.3f;
        v[k].x = 100.0f * t;
        t = (0.6f * fn_1_BCAC(self)) - 0.3f;
        v[k].y = 100.0f * t;
        v[k].z = 100.0f * ((scale * fn_1_BCAC(self)) - half);
    }
    v[3].x = 0.6f * v[0].x;
    v[3].y = 0.6f * v[0].y;
    v[3].z = 0.6f * v[0].z;
    v[4].x = 0.6f * v[2].x;
    v[4].y = 0.6f * v[2].y;
    v[4].z = 0.6f * v[2].z;
    for (k = 0; k < 5; k++) {
        v[k].y += base.y;
        v[k].z += base.z;
    }
    x0 = v[0].x + (55.0f + base.x);
    v[0].x = x0;
    x1 = v[1].x + (110.0f + base.x);
    v[1].x = x1;
    v[2].x += 165.0f + base.x;
    v[3].x += 55.0f + base.x;
    x3 = v[4].x + (165.0f + base.x);
    v[4].x = x3;
    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 4);
    GXPosition3f32(x0, v[0].y, v[0].z);
    GXColor4u8(0, 0xFF, 0xFF, 0xFF);
    GXPosition3f32(base.x, base.y, base.z);
    GXColor4u8(0, 0, 0xFF, 0xFF);
    GXPosition3f32(v[3].x, v[3].y, v[3].z);
    GXColor4u8(0, 0, 0xFF, 0xFF);
    GXPosition3f32(x1, v[1].y, v[1].z);
    GXColor4u8(0xFF, 0xFF, 0xFF, 0xFF);
    GXEnd();
    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 4);
    GXPosition3f32(v[2].x, v[2].y, v[2].z);
    GXColor4u8(0, 0xFF, 0xFF, 0xFF);
    GXPosition3f32(x1, v[1].y, v[1].z);
    GXColor4u8(0xFF, 0xFF, 0xFF, 0xFF);
    GXPosition3f32(x3, v[4].y, v[4].z);
    GXColor4u8(0, 0, 0xFF, 0xFF);
    GXPosition3f32((f32)(220.0f + base.x), base.y, base.z);
    GXColor4u8(0, 0, 0xFF, 0xFF);
    GXEnd();
}

void fn_1_8094(Maze *self, s32 i, s32 j)
{
    Vec v[5];
    Vec base;
    f32 scale;
    f32 half;
    f32 t;
    f32 z1;
    f32 z2;
    f32 z4;
    f32 zEnd;
    s32 k;

    base.x = (220.0f * (f32)i) - 1650.0f;
    base.y = 100.0f * (2.0f + fn_1_BCAC(self));
    base.z = (220.0f * (f32)j) - 1210.0f;
    scale = 0.8f + ((f32)self->hold / 10.0f);
    half = scale * 0.5f;
    for (k = 0; k < 3; k++) {
        t = (scale * fn_1_BCAC(self)) - half;
        v[k].z = 100.0f * t;
        t = (0.6f * fn_1_BCAC(self)) - 0.3f;
        v[k].y = 100.0f * t;
        v[k].x = 100.0f * ((0.6f * fn_1_BCAC(self)) - 0.3f);
    }
    v[3].x = 0.6f * v[0].x;
    v[3].y = 0.6f * v[0].y;
    v[3].z = 0.6f * v[0].z;
    v[4].x = 0.6f * v[2].x;
    v[4].y = 0.6f * v[2].y;
    v[4].z = 0.6f * v[2].z;
    for (k = 0; k < 5; k++) {
        v[k].x += base.x;
        v[k].y += base.y;
    }
    v[0].z += 55.0f + base.z;
    z1 = v[1].z + (110.0f + base.z);
    v[1].z = z1;
    z2 = v[2].z + (165.0f + base.z);
    v[2].z = z2;
    v[3].z += 55.0f + base.z;
    z4 = v[4].z + (165.0f + base.z);
    v[4].z = z4;
    zEnd = 220.0f + base.z;
    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 4);
    GXPosition3f32(v[0].x, v[0].y, v[0].z);
    GXColor4u8(0xFF, 0xFF, 0xFF, 0xFF);
    GXPosition3f32(base.x, base.y, base.z);
    GXColor4u8(0, 0, 0xFF, 0xFF);
    GXPosition3f32(v[3].x, v[3].y, v[3].z);
    GXColor4u8(0, 0, 0xFF, 0xFF);
    GXPosition3f32(v[1].x, v[1].y, z1);
    GXColor4u8(0xFF, 0xFF, 0xFF, 0xFF);
    GXEnd();
    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 4);
    GXPosition3f32(v[2].x, v[2].y, z2);
    GXColor4u8(0xFF, 0xFF, 0xFF, 0xFF);
    GXPosition3f32(v[1].x, v[1].y, z1);
    GXColor4u8(0xFF, 0xFF, 0xFF, 0xFF);
    GXPosition3f32(v[4].x, v[4].y, z4);
    GXColor4u8(0, 0, 0xFF, 0xFF);
    GXPosition3f32(base.x, base.y, zEnd);
    GXColor4u8(0, 0, 0xFF, 0xFF);
    GXEnd();
}

void fn_1_8828(Maze *self)
{
    s32 i;
    s32 j;
    u8 w;

    MAZE_GX_BEGIN_STATE();
    GXSetCullMode(GX_CULL_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    MAZE_GX_VTX_STATE();
    for (j = 0; j <= 11; j++) {
        for (i = 0; i <= 15; i++) {
            w = self->cell[i][j].wall;
            if (i < 15 && (w & 0x40) != 0) {
                fn_1_78FC(self, i, j);
            }
            if (j < 11 && (w & 0x80) != 0) {
                fn_1_8094(self, i, j);
            }
        }
    }
}

void fn_1_89C0(Maze *self, s32 i, s32 j)
{
    f32 x0;
    f32 z0;
    f32 x1;
    f32 frac;

    x0 = (220.0f * (f32)i) - 1650.0f;
    z0 = (220.0f * (f32)j) - 1210.0f;
    frac = (f32)(0x14 - self->hold) / 20.0f;
    if (j <= 0 || j >= 11) {
        frac = 1.0f;
    }
    x1 = x0 + (220.0f * frac);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x0, 280.0f, (f32)(z0 + 7.0f));
    GXColor4u8(0, 0xFF, 0xFF, 0x80);
    GXPosition3f32(x1, 280.0f, (f32)(z0 + 7.0f));
    GXColor4u8(0, 0xFF, 0xFF, 0x80);
    GXPosition3f32(x1, 170.0f, (f32)(z0 + 7.0f));
    GXColor4u8(0, 0, 0xFF, 0);
    GXPosition3f32(x0, 170.0f, (f32)(z0 + 7.0f));
    GXColor4u8(0, 0, 0xFF, 0);
    GXEnd();
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x0, 280.0f, (f32)(z0 - 7.0f));
    GXColor4u8(0, 0xFF, 0xFF, 0x80);
    GXPosition3f32(x1, 280.0f, (f32)(z0 - 7.0f));
    GXColor4u8(0, 0xFF, 0xFF, 0x80);
    GXPosition3f32(x1, 280.0f, (f32)(z0 + 7.0f));
    GXColor4u8(0, 0xFF, 0xFF, 0x80);
    GXPosition3f32(x0, 280.0f, (f32)(z0 + 7.0f));
    GXColor4u8(0, 0xFF, 0xFF, 0x80);
    GXEnd();
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x1, 280.0f, (f32)(z0 - 7.0f));
    GXColor4u8(0, 0xFF, 0xFF, 0x80);
    GXPosition3f32(x0, 280.0f, (f32)(z0 - 7.0f));
    GXColor4u8(0, 0xFF, 0xFF, 0x80);
    GXPosition3f32(x0, 170.0f, (f32)(z0 - 7.0f));
    GXColor4u8(0, 0, 0xFF, 0);
    GXPosition3f32(x1, 170.0f, (f32)(z0 - 7.0f));
    GXColor4u8(0, 0, 0xFF, 0);
    GXEnd();
}

void fn_1_8F80(Maze *self, s32 i, s32 j)
{
    f32 x0;
    f32 z0;
    f32 z1;
    f32 frac;

    x0 = (220.0f * (f32)i) - 1650.0f;
    z0 = (220.0f * (f32)j) - 1210.0f;
    frac = (f32)(0x14 - self->hold) / 20.0f;
    if (i <= 0 || i >= 15) {
        frac = 1.0f;
    }
    z1 = z0 + (220.0f * frac);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x0 - 7.0f, 280.0f, z0);
    GXColor4u8(0, 0xFF, 0xFF, 0x80);
    GXPosition3f32((f32)(x0 - 7.0f), 280.0f, z1);
    GXColor4u8(0, 0xFF, 0xFF, 0x80);
    GXPosition3f32((f32)(x0 - 7.0f), 170.0f, z1);
    GXColor4u8(0, 0, 0xFF, 0);
    GXPosition3f32((f32)(x0 - 7.0f), 170.0f, z0);
    GXColor4u8(0, 0, 0xFF, 0);
    GXEnd();
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32((f32)(x0 - 7.0f), 280.0f, z0);
    GXColor4u8(0, 0xFF, 0xFF, 0x80);
    GXPosition3f32((f32)(x0 + 7.0f), 280.0f, z0);
    GXColor4u8(0, 0xFF, 0xFF, 0x80);
    GXPosition3f32((f32)(x0 + 7.0f), 280.0f, z1);
    GXColor4u8(0, 0xFF, 0xFF, 0x80);
    GXPosition3f32((f32)(x0 - 7.0f), 280.0f, z1);
    GXColor4u8(0, 0xFF, 0xFF, 0x80);
    GXEnd();
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32((f32)(x0 + 7.0f), 280.0f, z1);
    GXColor4u8(0, 0xFF, 0xFF, 0x80);
    GXPosition3f32((f32)(x0 + 7.0f), 280.0f, z0);
    GXColor4u8(0, 0xFF, 0xFF, 0x80);
    GXPosition3f32((f32)(x0 + 7.0f), 170.0f, z0);
    GXColor4u8(0, 0, 0xFF, 0);
    GXPosition3f32((f32)(x0 + 7.0f), 170.0f, z1);
    GXColor4u8(0, 0, 0xFF, 0);
    GXEnd();
}

void fn_1_9540(Maze *self)
{
    s32 i;
    s32 j;
    u8 w;

    MAZE_GX_BEGIN_STATE();
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetCullMode(GX_CULL_BACK);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    MAZE_GX_VTX_STATE();
    for (j = 0; j <= 11; j++) {
        for (i = 0; i <= 15; i++) {
            w = self->cell[i][j].wall;
            if (i < 15 && (w & 0x40) != 0) {
                fn_1_89C0(self, i, j);
            }
            if (j < 11 && (w & 0x80) != 0) {
                fn_1_8F80(self, i, j);
            }
        }
    }
}

void fn_1_96E8(Maze *self, u8 r, u8 g, u8 b, u8 a, f32 x, f32 z)
{
    f32 px[8];
    f32 pz[8];
    f32 x1;
    f32 x2;
    f32 x3;
    f32 z1;
    f32 z2;
    f32 z3;
    s32 a1;
    s32 a2;
    s32 a3;
    s32 a4;

    /* outer corners 0..3, inner corners 4..7 */
    px[0] = x;
    pz[0] = z;
    x1 = 220.0f + x;
    px[1] = x1;
    px[2] = x1;
    px[3] = x;
    x2 = 11.0f + x;
    px[4] = x2;
    x3 = x1 - 11.0f;
    px[5] = x3;
    px[6] = x3;
    px[7] = x2;
    pz[1] = z;
    z1 = 220.0f + z;
    pz[2] = z1;
    pz[3] = z1;
    z2 = 11.0f + z;
    pz[4] = z2;
    pz[5] = z2;
    z3 = z1 - 11.0f;
    pz[6] = z3;
    pz[7] = z3;
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    a1 = (s32)(a * 6) / 8;
    GXPosition3f32(px[4], 124.0f, pz[4]);
    GXColor4u8(r, g, b, a);
    GXPosition3f32(px[5], 124.0f, pz[5]);
    GXColor4u8(r, g, b, a);
    GXPosition3f32(px[6], 124.0f, pz[6]);
    GXColor4u8(r, g, b, a);
    GXPosition3f32(px[7], 124.0f, pz[7]);
    GXColor4u8(r, g, b, a);
    GXEnd();
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    a2 = (s32)(a * 5) / 8;
    GXPosition3f32(px[0], 115.0f, pz[0]);
    GXColor4u8(r, g, b, (u8)a1);
    GXPosition3f32(px[1], 115.0f, pz[1]);
    GXColor4u8(r, g, b, (u8)a1);
    GXPosition3f32(px[5], 124.0f, pz[5]);
    GXColor4u8(r, g, b, (u8)a1);
    GXPosition3f32(px[4], 124.0f, pz[4]);
    GXColor4u8(r, g, b, (u8)a1);
    GXEnd();
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    a3 = (s32)(a * 4) / 8;
    GXPosition3f32(px[0], 115.0f, pz[0]);
    GXColor4u8(r, g, b, (u8)a2);
    GXPosition3f32(px[4], 124.0f, pz[4]);
    GXColor4u8(r, g, b, (u8)a2);
    GXPosition3f32(px[7], 124.0f, pz[7]);
    GXColor4u8(r, g, b, (u8)a2);
    GXPosition3f32(px[3], 115.0f, pz[3]);
    GXColor4u8(r, g, b, (u8)a2);
    GXEnd();
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    a4 = (s32)(a * 3) / 8;
    GXPosition3f32(px[5], 124.0f, pz[5]);
    GXColor4u8(r, g, b, (u8)a3);
    GXPosition3f32(px[1], 115.0f, pz[1]);
    GXColor4u8(r, g, b, (u8)a3);
    GXPosition3f32(px[2], 115.0f, pz[2]);
    GXColor4u8(r, g, b, (u8)a3);
    GXPosition3f32(px[6], 124.0f, pz[6]);
    GXColor4u8(r, g, b, (u8)a3);
    GXEnd();
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(px[7], 124.0f, pz[7]);
    GXColor4u8(r, g, b, (u8)a4);
    GXPosition3f32(px[6], 124.0f, pz[6]);
    GXColor4u8(r, g, b, (u8)a4);
    GXPosition3f32(px[2], 115.0f, pz[2]);
    GXColor4u8(r, g, b, (u8)a4);
    GXPosition3f32(px[3], 115.0f, pz[3]);
    GXColor4u8(r, g, b, (u8)a4);
    GXEnd();
}

void fn_1_A100(Maze *self)
{
    s32 i;
    s32 j;
    u8 fade;
    u8 mark;
    f32 x;
    f32 z;

    MAZE_GX_BEGIN_STATE();
    GXSetCullMode(GX_CULL_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    MAZE_GX_VTX_STATE();
    for (j = 0; j < 11; j++) {
        for (i = 0; i < 15; i++) {
            fade = self->cell[i][j].fade;
            mark = self->cell[i][j].mark & 0xF;
            if (fade != 0) {
                x = (220.0f * (f32)i) - 1650.0f;
                z = (220.0f * (f32)j) - 1210.0f;
                if (mark != 0xF) {
                    fn_1_96E8(self, lbl_1_rodata_FC[mark].r, lbl_1_rodata_FC[mark].g, lbl_1_rodata_FC[mark].b, (u8)(fade >> 1), x, z);
                } else {
                    fn_1_96E8(self, 0xFF, 0xFF, (u8)((self->timer & 0xF) * 0x10), (u8)(fade >> 1), x, z);
                }
            }
        }
    }
}

void fn_1_A3C0(Maze *self, u8 type, f32 x, f32 z)
{
    s32 t;
    s32 idx;

    t = type & 7;
    if (t > 4 || t == 0) {
        return;
    }
    idx = (t - 1) * 4;
    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 4);
    GXPosition3f32(x + lbl_1_rodata_134[idx].x, 300.0f, (f32)(z + lbl_1_rodata_134[idx].z));
    GXColor4u8(0xFF, 0xFF, 0, 0x80);
    GXPosition3f32((f32)(x + lbl_1_rodata_134[idx + 1].x), 300.0f, (f32)(z + lbl_1_rodata_134[idx + 1].z));
    GXColor4u8(0xFF, 0, 0, 0x80);
    GXPosition3f32((f32)(x + lbl_1_rodata_134[idx + 2].x), 300.0f, (f32)(z + lbl_1_rodata_134[idx + 2].z));
    GXColor4u8(0xFF, 0, 0, 0x80);
    GXPosition3f32((f32)(x + lbl_1_rodata_134[idx + 3].x), 300.0f, (f32)(z + lbl_1_rodata_134[idx + 3].z));
    GXColor4u8(0xFF, 0, 0, 0x80);
    GXEnd();
}

void fn_1_A6EC(Maze *self)
{
    s32 i;
    s32 j;
    f32 x;
    f32 z;

    MAZE_GX_BEGIN_STATE();
    GXSetCullMode(GX_CULL_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    MAZE_GX_VTX_STATE();
    for (j = 0; j < 11; j++) {
        for (i = 0; i < 15; i++) {
            x = 110.0f + ((220.0f * (f32)i) - 1650.0f);
            z = 110.0f + ((220.0f * (f32)j) - 1210.0f);
            fn_1_A3C0(self, self->cell[i][j].wall, x, z);
        }
    }
}

void fn_1_ABC4(Maze *self, s32 alpha)
{
    if (alpha > 0) {
        MAZE_GX_BEGIN_STATE();
        GXSetCullMode(GX_CULL_NONE);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetZMode(GX_TRUE, GX_GREATER, GX_FALSE);
        MAZE_GX_VTX_STATE();
        GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 4);
        GXPosition3f32(-3000.0f, -200.0f, -3000.0f);
        GXColor4u8(0, 0, 0, (u8)alpha);
        GXPosition3f32(3000.0f, -200.0f, -3000.0f);
        GXColor4u8(0, 0, 0, (u8)alpha);
        GXPosition3f32(3000.0f, -200.0f, 3000.0f);
        GXColor4u8(0, 0, 0, (u8)alpha);
        GXPosition3f32(-3000.0f, -200.0f, 3000.0f);
        GXColor4u8(0, 0, 0, (u8)alpha);
        GXEnd();
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    }
}

void fn_1_AEAC(Maze *self, Mtx *mtx)
{
    GXLoadPosMtxImm(*mtx, 0);
    if (self->dirty == 0) {
        fn_1_A100(self);
    }
}

void fn_1_AEF8(Maze *self, Mtx *mtx)
{
    self->seed = self->seedSaved;
    GXLoadPosMtxImm(*mtx, 0);
    fn_1_ABC4(self, self->alpha);
    if (self->dirty != 0) {
        fn_1_8828(self);
        return;
    }
    fn_1_8828(self);
    fn_1_9540(self);
}

void fn_1_B524(Maze *self)
{
    s32 i;
    s32 j;
    u8 a;
    u8 b;

    for (j = 0; j < 11; j++) {
        for (i = 0; i < 8; i++) {
            a = self->cell[i][j].wall;
            b = self->cell[14 - i][j].wall;
            self->cell[i][j].wall = (a & 0xB0) | (lbl_1_rodata_204[b & 0xF] | (b & 0x40));
            self->cell[14 - i][j].wall = (b & 0xB0) | (lbl_1_rodata_204[a & 0xF] | (a & 0x40));
        }
    }
    for (j = 0; j < 11; j++) {
        for (i = 0; i < 8; i++) {
            a = self->cell[i][j].wall;
            b = self->cell[15 - i][j].wall;
            self->cell[i][j].wall = (b & 0x80) | (a & 0x7F);
            self->cell[15 - i][j].wall = (a & 0x80) | (b & 0x7F);
        }
    }
}

void fn_1_B6C8(Maze *self)
{
    s32 i;
    s32 j;
    u8 a;
    u8 b;

    for (j = 0; j < 6; j++) {
        for (i = 0; i < 15; i++) {
            a = self->cell[i][j].wall;
            b = self->cell[i][10 - j].wall;
            self->cell[i][j].wall = (a & 0x70) | (lbl_1_rodata_20A[b & 0xF] | (b & 0x80));
            self->cell[i][10 - j].wall = (b & 0x70) | (lbl_1_rodata_20A[a & 0xF] | (a & 0x80));
        }
    }
    for (j = 0; j < 6; j++) {
        for (i = 0; i < 15; i++) {
            a = self->cell[i][j].wall;
            b = self->cell[i][11 - j].wall;
            self->cell[i][j].wall = (b & 0x40) | (a & 0xBF);
            self->cell[i][11 - j].wall = (a & 0x40) | (b & 0xBF);
        }
    }
}

void fn_1_B86C(Maze *self, u8 *src, s32 flags)
{
    s32 i;
    s32 j;
    u8 *p;

    if (self->busy == 0) {
        p = src;
        for (j = 0; j <= 11; j++) {
            for (i = 0; i <= 15; i++) {
                self->cell[i][j].wall = *p++;
            }
        }
        if ((flags & 1) != 0) {
            fn_1_B524(self);
        }
        if ((flags & 2) != 0) {
            fn_1_B6C8(self);
        }
        for (i = 0; i < 15; i++) {
            self->cell[i][0].wall |= 0x40;
            self->cell[i][11].wall |= 0x40;
        }
        for (j = 0; j < 11; j++) {
            self->cell[0][j].wall |= 0x80;
            self->cell[15][j].wall |= 0x80;
        }
        self->hold = 0x14;
    }
}

f32 fn_1_BCAC(Maze *self)
{
    self->seed = self->seed * 0x41C64E6D + 0x4D2;
    return (f32)(((u32)self->seed >> 16) & 0x7FFF) / 32767.0f;
}

void fn_1_BD44(Maze *self, s32 val, f32 x, f32 z)
{
    s32 i;
    s32 j;

    if (self->busy == 0 && (s32)self->phase >= 5) {
        i = (s32)((1650.0f + x) / 220.0f);
        j = (s32)((1210.0f + z) / 220.0f);
        if (i >= 0 && i < 15 && j >= 0 && j < 11) {
            if ((self->cell[i][j].mark & 0x10) != 0) {
                val = 0xF;
            }
            self->cell[i][j].mark = val | 0x90;
        }
    }
}

u8 fn_1_BE30(Maze *self, f32 x, f32 z)
{
    s32 i;
    s32 j;
    u8 ret;

    i = (s32)((1650.0f + x) / 220.0f);
    j = (s32)((1210.0f + z) / 220.0f);
    ret = 0;
    if (i >= 0 && i < 15 && j >= 0 && j < 11) {
        ret = self->cell[i][j].wall;
    }
    return ret;
}

u8 fn_1_BEE8(Maze *self, f32 x, f32 z)
{
    s32 i;
    s32 j;
    u8 ret;

    i = (s32)((1650.0f + x) / 220.0f);
    j = (s32)((1210.0f + z) / 220.0f);
    ret = 0;
    if (i >= 0 && i < 15 && j >= 0 && j < 11) {
        ret = self->cell[i][j].wall & 0xC0;
        if ((self->cell[i + 1][j].wall & 0x80) != 0) {
            ret |= 8;
        }
        if ((self->cell[i][j + 1].wall & 0x40) != 0) {
            ret |= 4;
        }
    }
    return ret;
}

s32 fn_1_C000(Maze *self, f32 x, f32 z)
{
    f32 fx;
    f32 fz;
    f32 ux;
    f32 uz;
    s32 i;
    s32 j;
    s32 ret;

    fx = 1650.0f + x;
    fz = 1210.0f + z;
    i = (s32)(fx / 220.0f);
    j = (s32)(fz / 220.0f);
    ret = 0;
    if (i < 0 || i >= 15 || j < 0 || j >= 11) {
        return 0;
    }
    ux = (fx - (220.0f * (f32)i)) / 220.0f;
    uz = (fz - (220.0f * (f32)j)) / 220.0f;
    if (ux < 0.1f) {
        ret = self->cell[i][j].wall & 0x80;
    }
    if (uz < 0.1f) {
        ret |= self->cell[i][j].wall & 0x40;
    }
    if (ux > 0.9f) {
        ret = self->cell[i + 1][j].wall & 0x80;
    }
    if (uz > 0.9f) {
        ret |= self->cell[i][j + 1].wall & 0x40;
    }
    return ret > 0;
}

void fn_1_C224(void *unused, s32 idx)
{
    Maze *maze;
    s32 off;

    lbl_1_bss_34 != 0 ? (void)0 : fn_1_0(lbl_1_data_322C, lbl_1_data_3220, 0x27);
    maze = lbl_1_bss_34;
    off = (idx % 64) * 0xC0;
    fn_1_B86C(maze, &lbl_1_data_210[off], irandom__7CRandomFv() & 3);
}
