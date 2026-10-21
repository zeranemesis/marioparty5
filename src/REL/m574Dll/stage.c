#include "REL/m574Dll.h"
#include "game/mg/seqman.h"

typedef struct M574StageCell_s {
    s32 v00;
    u8 pad04[0x08];
    s32 v0C;
    s32 v10;
    u8 pad14[0x0C];
    s32 v20[3];
    f32 f2C[3];
    f32 f38[3];
    u8 pad44[0x0C];
    void *v50[3];
} M574_STAGE_CELL;

typedef struct M574SlotHook_s {
    s32 v00;
    s32 v04;
    s32 v08;
    s32 v0C;
    void *v10;
    s32 v14;
    void *v18;
    void *v1C;
    f32 v20;
} M574_SLOT_HOOK;

typedef struct M574StageSlot_s {
    s32 v00;
    s32 v04;
    u8 pad08[0x08];
    Vec v10;
    Vec v1C;
    u8 pad28[0x0C];
    f32 f34;
    s32 v38;
    f32 f3C;
    f32 f40;
    f32 f44;
    void *v48;
    M574_SLOT_HOOK hook;
    u8 pad70[0x18];
} M574_STAGE_SLOT;

typedef struct M574SlotPtr_s {
    u8 pad00[0x1C];
    f32 f1C;
    f32 f20;
} M574_SLOT_PTR;

typedef struct M574StageHookEnt_s {
    M574_SLOT_HOOK hook;
    u8 pad24[0x18];
} M574_STAGE_HOOKENT;

typedef struct M574StageRow_s {
    f32 f00;
    f32 f04;
    f32 f08;
    f32 f0C;
    s32 v10;
    M574_SLOT_HOOK hook;
    u8 pad38[0x18];
} M574_STAGE_ROW;

typedef struct M574StageMot_s {
    s32 v00;
    u8 pad04[0x08];
    s32 v0C;
    Vec v10;
    Vec v1C;
    u8 pad28[0x10];
    s32 v38;
    Vec v3C;
    void *v48;
    M574_SLOT_HOOK hook;
    u8 pad70[0x18];
} M574_STAGE_MOT;

typedef struct M574StageEnt_s {
    s16 v00;
    u8 pad02[0x12];
    Vec v14;
    u8 pad20[0x0C];
    f32 f2C;
    u8 pad30[0x04];
    Vec v34;
    u8 pad40[0x03];
    u8 v43;
} M574_STAGE_ENT;

typedef struct M574StageEnt2_s {
    s32 v00;
    u8 pad04[0x0C];
    f32 f10;
    f32 f14;
    u8 pad18[0x1C];
    f32 f34;
    u8 pad38[0x10];
    void *v48;
    u8 pad4C[0x3C];
} M574_STAGE_ENT2;

typedef struct M574StageEnt3_s {
    u32 v00;
    u8 pad04[0x18];
    Vec *v1C;
} M574_STAGE_ENT3;

typedef struct M574StageTbl_s {
    u8 pad00[0x08];
    HU3DTEXSCRID texId;
    s32 v0C;
    f32 f10;
    M574_STAGE_MOT mot;
    s16 ids[5];
    u8 padA6[0x02];
    M574_STAGE_ROW row[4];
    M574_STAGE_CELL cell[1];
    u8 pad244[0x5C];
    M574_STAGE_ENT2 ent2[2];
    M574_STAGE_SLOT slotB[14];
    M574_STAGE_SLOT slotA[6];
    M574_STAGE_SLOT slot[1];
    u8 padED8[0x88];
    M574_SLOT_HOOK hookF60;
    u8 padF84[0x18];
    M574_STAGE_HOOKENT hookEnt[4];
} M574_STAGE_TBL;

typedef struct M574ScrollUv_s {
    f32 x;
    f32 y;
} M574_SCROLL_UV;

typedef struct M574ModelObj_s {
    u8 pad00[0x1C];
    Vec v1C;
    u8 pad28[0x08];
    f32 f30;
} M574_MODEL_OBJ;

typedef struct M574ModelNode_s {
    u8 pad00[0x04];
    s16 v04;
    u8 pad06[0x02];
    s16 id08;
    s16 id0A;
    u8 pad0C[0x02];
    s16 id0E;
    s16 id;
} M574_MODEL_NODE;

typedef struct M574ModelPos_s {
    u8 pad00[0x1C];
    f32 x;
    f32 y;
    f32 z;
} M574_MODEL_POS;

typedef struct M574StageObj_s {
    u16 unk00;
    u8 pad02[0x06];
    s32 unk08;
    u8 pad0C[0x08];
    void *v14;
    f32 *unk18;
    Vec *v1C;
    u8 pad20[0x10];
    s16 v30;
    u8 pad32[0x06];
    f32 *unk38;
    u8 pad3C[0x04];
    M574_MODEL_NODE *unk40;
    u8 pad44[0x04];
    M574_STAGE_ENT *unk48;
    s32 v4C;
    u8 pad50[0x0C];
    M574_STAGE_TBL *unk5C;
} M574_STAGE_OBJ;

typedef struct M574TexAnimDef_s {
    char name[12];
    f32 f[20];
    s32 datanum[4];
} M574_TEXANIM_DEF;

typedef struct M574Vec3_s {
    s32 x;
    s32 y;
    s32 z;
} M574_VEC3;

typedef struct M574MotNode_s {
    s16 f00;
    u8 pad02[0x12];
    M574_VEC3 rot;
    u8 pad20[0x0C];
    f32 f2C;
    u8 pad30[0x04];
    M574_VEC3 pos;
    u8 pad40[0x04];
} M574_MOTNODE;

typedef struct M574MotGrp_s {
    u8 pad00[0x30];
    s16 cnt30;
    u8 pad32[0x16];
    M574_MOTNODE *nd48;
} M574_MOTGRP;

typedef struct M574Hu3DModel_s {
    u8 pad00[0x120];
    M574_MOTGRP *grp120;
} M574_HU3DMODEL;

M574_STAGE_OBJ *lbl_1_bss_48;

s32 lbl_1_data_220[4] = { 0x0081000C, 0x0081000D, 0x00810011, 0x00810012 };

s32 lbl_1_data_230[4] = { 0x00810018, 0x00810019, 0x00810018, 0x00810019 };

s32 lbl_1_data_240[4] = { 0x0081000F, 0x00810010, 0x00810014, 0x00810015 };

f32 lbl_1_data_250[3][3] = {
    { -3456.0f, 100.0f, -40.0f },
    { -255.0f, 100.0f, -40.0f },
    { 3200.0f, 100.0f, -40.0f }
};

f32 lbl_1_data_280[4] = { 60.0f, 300.0f, 60.0f, 300.0f };

char lbl_1_data_290[4][10] = {
    "furihan", "furihan", "furihan", "furihan"
};

u8 lbl_1_data_2C4[20] = {
    0, 1, 0, 0, 1, 1, 2, 0, 1, 2, 2, 1, 0, 2, 1, 1, 0, 2, 0, 2
};

char lbl_1_data_2D8[6][10] = {
    "slot_L3", "slot_L2", "Slot", "slot_R3", "slot_R2", "slot_R1"
};

s32 lbl_1_data_314[2] = { 0x0081000E, 0x00810013 };

char lbl_1_data_31C[14][10] = {
    "block", "blockL2", "blockL3", "blockL4", "blockL5", "blockLLD", "blockLLU",
    "blockR5", "blockR4", "blockR3", "blockR2", "blockR1", "blockRRD", "blockRRU"
};

char lbl_1_data_3A8[4][10] = {
    "b_ataLL", "b_ataL", "b_ataR", "b_ataRR"
};

char lbl_1_data_3D0[9][10] = {
    "kuppa", "10L", "50L", "10L", "10R", "50R", "100R", "baneL", "baneR"
};

s32 lbl_1_data_42C[9] = {
    0x00810005, 0x00810008, 0x00810009, 0x0081000A, 0x00810008,
    0x00810009, 0x0081000A, 0x0081000B, 0x0081000B
};

s32 lbl_1_data_450[6] = { 0, 0, 0, 1, 1, 1 };

s32 lbl_1_data_468[6] = { 3, 4, 5, 3, 4, 5 };

s32 lbl_1_data_480[3] = { 0x0081001B, 0x0081001C, 0x0081001D };

Vec lbl_1_data_48C[2] = {
    { -650.0f, 960.0f, 0.0f },
    { -650.0f, 960.0f, 0.0f }
};

char lbl_1_data_4A4[12] = "kuppaabomb";

M574_TEXANIM_DEF lbl_1_data_504 = {
    "845memit",
    {
        -0.25f, -0.125f, -0.25f, 0.0f, -0.25f,
        0.125f, -0.25f, 0.25f, -0.25f, 0.375f,
        -0.25f, 0.5f, -0.25f, 0.625f, -0.25f,
        0.75f, 0.0f, -0.125f, 0.0f, 0.0f
    },
    { 0x923, 0x924, 0x925, 0x926 }
};

static const f32 lbl_1_rodata_E4 = 80.0f;

static const f32 lbl_1_rodata_128 = 0.0f;

static const f32 lbl_1_rodata_178 = 60.0f;

static const f32 lbl_1_rodata_1F0 = 8.333334f;

static const f64 lbl_1_rodata_1F8 = 1.666666753590107;

static const f32 lbl_1_rodata_160 = 1.0f;

static const f32 lbl_1_rodata_218 = 40.0f;

static const f32 lbl_1_rodata_254 = 5.0f;

static const f64 lbl_1_rodata_F0 = 10.0;

static const f64 lbl_1_rodata_120 = 4503601774854144.0;

static const f64 lbl_1_rodata_130 = 0.5;

static const f64 lbl_1_rodata_158 = 0.1;

static const f64 lbl_1_rodata_258 = 0.0;

static const f64 lbl_1_rodata_260 = 5.0;

static const f64 lbl_1_rodata_268 = 0.125;

M574_SCROLL_UV lbl_1_data_510[10] = {
    { -0.25f, -0.125f },
    { -0.25f, 0.0f },
    { -0.25f, 0.125f },
    { -0.25f, 0.25f },
    { -0.25f, 0.375f },
    { -0.25f, 0.5f },
    { -0.25f, 0.625f },
    { -0.25f, 0.75f },
    { 0.0f, -0.125f },
    { 0.0f, 0.0f }
};

s32 lbl_1_data_560[4] = { 0x923, 0x924, 0x925, 0x926 };

float Hu3DMotionMaxTimeGet(s16 modelId);

void Hu3DTexScrollPosSet(HU3DTEXSCRID id, f32 x, f32 y, f32 z);

double fmod(double x, double y);

void fn_1_77D4(s32 a, s32 b);
void fn_1_C10(s32 a, void *b);

void fn_1_20CC(M574_STAGE_OBJ *p);
void fn_1_2364(M574_STAGE_OBJ *p);
void fn_1_2928(M574_STAGE_OBJ *p);
void fn_1_2F8C(M574_STAGE_OBJ *p);
void fn_1_3374(M574_STAGE_OBJ *p);
void fn_1_383C(M574_STAGE_OBJ *p);
void fn_1_448C(M574_STAGE_OBJ *p);
void fn_1_4D28(M574_STAGE_OBJ *p);
void fn_1_5750(M574_STAGE_OBJ *p);
void fn_1_5E5C(M574_STAGE_OBJ *p);
void fn_1_6168(M574_STAGE_OBJ *p);
void fn_1_2B88(M574_STAGE_OBJ *p);
void fn_1_26F4(M574_STAGE_OBJ *p);
void fn_1_3154(M574_STAGE_OBJ *p);
void fn_1_34B0(M574_STAGE_OBJ *p);
void fn_1_3A1C(M574_STAGE_OBJ *p);
void fn_1_4660(M574_STAGE_OBJ *p);
void fn_1_4EE8(M574_STAGE_OBJ *p);
void fn_1_5934(M574_STAGE_OBJ *p);
void fn_1_5ECC(M574_STAGE_OBJ *p);
void fn_1_629C(M574_STAGE_OBJ *p);
s32 fn_1_3240(M574_STAGE_OBJ *p, M574_STAGE_OBJ *p2);
void fn_1_8B48(M574_SLOT_HOOK *hook);
s32 fn_1_5628(M574_STAGE_OBJ *p);
void fn_1_62A0(s32 unused, M574_STAGE_OBJ *p);
u32 frand(void);
double sin(double x);
double cos(double x);
void fn_1_6C2C(Vec *v, f32 *s);
void fn_1_8FDC(M574_SLOT_HOOK *hook);
s32 fn_1_5D64(s32 unused, M574_STAGE_OBJ *p);
f32 fn_1_6568(s32 idx);
void fn_1_8FA8(M574_SLOT_HOOK *hook, M574_MODEL_OBJ *obj);
void fn_1_47DC(M574_STAGE_OBJ *p, M574_STAGE_TBL *tbl);
s32 fn_1_6CC8(s32 idx);
s32 fn_1_918(void);
void fn_1_8F54(M574_SLOT_HOOK *hook, HU3DMODELID model);
s32 fn_1_2D88(M574_STAGE_OBJ *p, M574_SLOT_HOOK *hook);

void fn_1_2028(OMOBJMAN *objMan) {
    M574_STAGE_OBJ *obj;
    M574_STAGE_TBL *tbl;

    obj = (M574_STAGE_OBJ *)omAddObjEx(objMan, 0x14, 0x21, 3, OM_GRP_NONE, (void (*)(OMOBJ *))fn_1_20CC);
    lbl_1_bss_48 = obj;
    obj->unk00 |= 0x100;
    tbl = HuMemDirectMallocNum(0, 0x108C, 0x10000000);
    obj->unk5C = tbl;
    memset(tbl, 0, 0x108C);
    obj->v4C = 0;
}

void fn_1_20C8(void) {
}

void fn_1_20CC(M574_STAGE_OBJ *p) {
    HU3DMODELID model;
    M574_SLOT_HOOK *h;
    M574_STAGE_TBL *tbl;

    tbl = p->unk5C;
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x00810000, HU_MEMNUM_OVL, HEAP_MODEL));
    ((s16 *)p->unk40)[0] = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 3);
    Hu3DModelShadowMapSet(model);
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x00810001, HU_MEMNUM_OVL, HEAP_MODEL));
    ((s16 *)p->unk40)[1] = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 3);
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x00810002, HU_MEMNUM_OVL, HEAP_MODEL));
    ((s16 *)p->unk40)[2] = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 7);
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x00810003, HU_MEMNUM_OVL, HEAP_MODEL));
    ((s16 *)p->unk40)[3] = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 7);
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x00810006, HU_MEMNUM_OVL, HEAP_MODEL));
    ((s16 *)p->unk40)[4] = model;
    Hu3DModelAttrSet(model, 1);
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x00810004, HU_MEMNUM_OVL, HEAP_MODEL));
    ((s16 *)p->unk40)[5] = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 3);
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x00810016, HU_MEMNUM_OVL, HEAP_MODEL));
    ((s16 *)p->unk40)[6] = model;
    Hu3DModelAttrSet(model, 1);
    h = &tbl->hookF60;
    h->v00 = 1;
    h->v04 = 5;
    h->v08 = 0;
    h->v0C = 1;
    h->v10 = p;
    h->v14 = 0;
    h->v18 = 0;
    fn_1_8F54(h, model);
    fn_1_8FDC(h);
    fn_1_8B48(h);
    fn_1_2928(p);
    fn_1_2F8C(p);
    fn_1_3374(p);
    fn_1_383C(p);
    fn_1_448C(p);
    fn_1_4D28(p);
    fn_1_5750(p);
    fn_1_5E5C(p);
    fn_1_6168(p);
    Hu3DZClearLayerSet(5);
    p->v14 = (void *)fn_1_2364;
}

void fn_1_23E8(M574_STAGE_OBJ *p, s16 idx, s32 n) {
    M574_STAGE_TBL *unused = p->unk5C;
    M574_MODEL_POS *pos;
    s16 modelId;

    modelId = p->unk40->id08;
    pos = (M574_MODEL_POS *)Hu3DModelObjPtrGet(modelId, &lbl_1_data_3D0[n][0]);
    if (pos != (M574_MODEL_POS *)0) {
        Hu3DModelPosSet(idx, pos->x, pos->y, pos->z);
    }
}

void fn_1_2478(M574_STAGE_OBJ *p, f32 *out, s32 n) {
    M574_STAGE_TBL *unused = p->unk5C;
    M574_MODEL_POS *pos;
    s16 modelId;

    modelId = p->unk40->id08;
    pos = (M574_MODEL_POS *)Hu3DModelObjPtrGet(modelId, &lbl_1_data_3D0[n][0]);
    if (pos != (M574_MODEL_POS *)0) {
        out[0] = pos->x;
        out[1] = pos->y;
        out[2] = pos->z;
    }
}

void fn_1_2514(M574_STAGE_OBJ *p, s32 flag, s32 n) {
    M574_STAGE_MOT *mot;
    s32 i;
    M574_STAGE_TBL *tbl;
    f32 base;
    Vec v;
    f32 f80;
    f32 cv;
    f32 sv;

    tbl = p->unk5C;
    mot = &tbl->mot;
    v.x = mot->v10.x;
    v.y = mot->v10.y - 150.0;
    v.z = 300.0f;
    f80 = lbl_1_rodata_E4;
    for (i = 0; i < n; i++) {
        if (flag == 0) {
            base = 240.0f;
        } else {
            base = 120.0f;
        }
        base = base + 10.0 * (3.0517578125e-05 * (f32)((u16)frand() - 0x8000));
        sv = 100.0 * (0.2 * sin(3.141592653589793 * base / 180.0));
        cv = 100.0 * (0.2 * cos(3.141592653589793 * base / 180.0));
        fn_1_6C2C(&v, &sv);
    }
}

void fn_1_2364(M574_STAGE_OBJ *p) {
    fn_1_2B88(p);
    fn_1_3154(p);
    fn_1_34B0(p);
    fn_1_3A1C(p);
    fn_1_4660(p);
    fn_1_4EE8(p);
    fn_1_5934(p);
    fn_1_5ECC(p);
    fn_1_629C(p);
    fn_1_26F4(p);
    p->v4C++;
}

void fn_1_26F4(M574_STAGE_OBJ *p) {
    /* Non-identique : tie-break de registres. En donnant un nom au second
       `p->unk5C` (t2) on obtient _savegpr_26 et le scratch r26 comme le
       code original ; il reste 8 lignes, toutes des renommages : MWCC donne
       tbl=r29/mot=r30 la ou CodeWarrior donne tbl=r30/mot=r29. L'ordre de
       declaration de tbl/mot/t2 ne change rien a l'objet produit. */
    s32 i;
    M574_STAGE_TBL *tbl;
    M574_STAGE_MOT *mot;
    M574_STAGE_TBL *t2;
    s32 j;
    f32 base;
    f32 a[6];

    tbl = p->unk5C;
    if (MgSeqModeGet() != 5) {
        return;
    }
    if (fn_1_918() <= 1) {
        return;
    }
    for (i = 0; i < 2; i++) {
        if (fn_1_6CC8(i) != 0) {
            ((s32 *)tbl)[i] = 0x78;
            continue;
        }
        if ((((s32 *)tbl)[i] -= 1) < 0) {
            t2 = p->unk5C;
            mot = &t2->mot;
            a[0] = mot->v10.x;
            a[1] = mot->v10.y - 150.0;
            a[2] = 300.0f;
            a[5] = 80.0f;
            for (j = 0; j < 1; j++) {
                if (i == 0) {
                    base = 240.0f;
                } else {
                    base = 120.0f;
                }
                base = base + 10.0 * (3.0517578125e-05 * (f32)((u16)frand() - 0x8000));
                a[3] = 100.0 * (0.2 * sin(3.141592653589793 * base / 180.0));
                a[4] = 100.0 * (0.2 * cos(3.141592653589793 * base / 180.0));
                fn_1_6C2C((Vec *)&a[0], &a[3]);
            }
            ((s32 *)tbl)[i] = 0x78;
        }
    }
}

void fn_1_2B88(M574_STAGE_OBJ *p) {
    M574_STAGE_ROW *row;
    s32 i;
    s16 id;
    M574_MODEL_OBJ *obj;
    M574_STAGE_TBL *tbl;
    f32 fv;

    tbl = p->unk5C;
    for (i = 0; i < 4; i++) {
        row = &tbl->row[i];
        row->f08 = row->f08 + 0.5 * (row->f0C - row->f08);
        row->f04 = row->f0C - row->f08;
        if (i & 1) {
            row->f04 = -row->f04;
        }
        if (row->f04 < 0.0f) {
            row->f04 = 0.0f;
        }
        row->f04 = row->f04 / 60.0;
        id = ((s16 *)p->unk40)[i + 9];
        Hu3DModelRotSet(id, 0.0f, 0.0f, row->f08);
        id = ((s16 *)p->unk40)[i + 0x11];
        fv = fn_1_6568(i);
        Hu3DMotionTimeSet(id, 30.0 * (1.0 - fv));
        if (row->v10 == 0) {
            if (MgSeqModeGet() == 6) {
                row->v10 = 1;
                Hu3DModelLayerSet(id, 3);
                id = p->unk40->v04;
                Hu3DModelLayerSet(id, 3);
            }
        }
        id = ((s16 *)p->unk40)[i + 0xD];
        obj = (M574_MODEL_OBJ *)Hu3DModelObjPtrGet(id, lbl_1_data_290[i]);
        if (obj != (M574_MODEL_OBJ *)0) {
            obj->f30 = row->f08;
        }
        fn_1_8FDC(&row->hook);
    }
}

void fn_1_2928(M574_STAGE_OBJ *p) {
    s32 i;
    M574_SLOT_HOOK *h;
    HU3DMODELID model;
    M574_MODEL_OBJ *o;
    M574_STAGE_TBL *tbl;

    tbl = p->unk5C;
    for (i = 0; i < 4; i++) {
        model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_220[i], HU_MEMNUM_OVL, HEAP_MODEL));
        ((s16 *)p->unk40)[i + 9] = model;
        Hu3DModelLayerSet(model, 4);
        Hu3DModelShadowSet(model);
        Hu3DModelPosSetV(model, (HuVecF *)lbl_1_data_250[i]);
        tbl->row[i].f08 = tbl->row[i].f0C = lbl_1_data_280[i];
        model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_240[i], HU_MEMNUM_OVL, HEAP_MODEL));
        ((s16 *)p->unk40)[i + 0x11] = model;
        Hu3DModelLayerSet(model, 6);
        Hu3DModelShadowSet(model);
        Hu3DModelAttrSet(model, 0x40000002);
        Hu3DMotionTimeSet(model, 0.0f);
        model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_230[i], HU_MEMNUM_OVL, HEAP_MODEL));
        ((s16 *)p->unk40)[i + 0xD] = model;
        Hu3DModelAttrSet(model, 1);
        o = (M574_MODEL_OBJ *)Hu3DModelObjPtrGet(model, lbl_1_data_290[i]);
        if (o != (M574_MODEL_OBJ *)0) {
            o->v1C.x = lbl_1_data_250[i][0];
            o->v1C.y = lbl_1_data_250[i][1];
            o->v1C.z = lbl_1_data_250[i][2];
        }
        h = &tbl->row[i].hook;
        h->v00 = 1;
        h->v04 = 6;
        h->v08 = i;
        h->v0C = 1;
        h->v10 = p;
        h->v14 = 0;
        h->v18 = (void *)fn_1_2D88;
        fn_1_8F54(h, model);
        fn_1_8FDC(h);
        fn_1_8FDC(h);
        fn_1_8B48(h);
    }
}

s32 fn_1_2D88(M574_STAGE_OBJ *p, M574_SLOT_HOOK *hook) {
    /* Non-identique : MWCC 1.3.2 charge les deux litteraux (100.0 puis 0.5)
       en tete d'expression et fait `fmul f0, f0, f32` (litteral en operande A)
       la ou CodeWarrior charge 0.5 apres le `fsubs` et ecrit
       `fmul f0, f1, f0`. 17 lignes, choix d'operandes FPR uniquement. */
    s32 idx;
    M574_STAGE_ROW *row;
    M574_STAGE_TBL *tbl;
    f32 scale;

    tbl = lbl_1_bss_48->unk5C;
    idx = p->unk08;
    row = &tbl->row[idx];
    if (p->unk38[7] > 0.3) {
        if (row->f04 >= 0.1) {
            scale = 100.0 * ((*(f32 *)hook->v18 - lbl_1_data_250[idx][0]) * 0.5);
            if (idx & 1) {
                scale = -scale;
            }
            if (scale > 0.1) {
                if (scale > 1.0) {
                    scale = 1.0f;
                }
                ((f32 *)hook->v1C)[0] = ((f32 *)hook->v1C)[0] -
                    50.0 * p->unk38[6] * 100.0 * 0.01666666753590107 * scale;
                ((f32 *)hook->v1C)[1] = ((f32 *)hook->v1C)[1] -
                    50.0 * p->unk38[7] * 100.0 * 0.01666666753590107 * scale;
                if (idx & 1) {
                    scale = -scale;
                }
                ((f32 *)hook->v1C)[0] = ((f32 *)hook->v1C)[0] + 100.0 * (0.1 * scale);
            }
        }
    }
    return 0;
}

void fn_1_2F8C(M574_STAGE_OBJ *p) {
    M574_STAGE_TBL *unused2;
    M574_STAGE_TBL *unused1;
    M574_SLOT_HOOK *h;
    s32 i;
    M574_STAGE_SLOT *slot;
    M574_MODEL_OBJ *obj;
    M574_MODEL_OBJ *obj2;
    HU3DMODELID model;
    M574_STAGE_TBL *tbl;
    HU3DMODELID id;
    HU3DMODELID id2;

    tbl = p->unk5C;
    for (i = 0; i < 6; i++) {
        slot = &tbl->slotA[i];
        model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_42C[i + 1],
                                                     HU_MEMNUM_OVL, HEAP_MODEL));
        ((s16 *)p->unk40)[i + 0x15] = model;
        Hu3DModelLayerSet(model, 4);
        Hu3DModelAttrSet(model, 0x40000001);
        Hu3DMotionStartEndSet(model, lbl_1_rodata_128, lbl_1_rodata_178);
        unused2 = p->unk5C;
        id = p->unk40->id08;
        obj = (M574_MODEL_OBJ *)Hu3DModelObjPtrGet(id, lbl_1_data_3D0[i + 1]);
        if (obj != (M574_MODEL_OBJ *)0) {
            Hu3DModelPosSet(model, obj->v1C.x, obj->v1C.y, obj->v1C.z);
        }
        unused1 = p->unk5C;
        id2 = p->unk40->id08;
        obj2 = (M574_MODEL_OBJ *)Hu3DModelObjPtrGet(id2, lbl_1_data_3D0[i + 1]);
        if (obj2 != (M574_MODEL_OBJ *)0) {
            slot->v10.x = obj2->v1C.x;
            slot->v10.y = obj2->v1C.y;
            slot->v10.z = obj2->v1C.z;
        }
        h = &slot->hook;
        h->v00 = 1;
        h->v04 = 3;
        h->v08 = i;
        h->v0C = 0;
        h->v10 = p;
        h->v14 = 0;
        h->v18 = fn_1_3240;
        h->v1C = &slot->v10;
        h->v20 = lbl_1_rodata_E4;
        fn_1_8B48(h);
    }
}

void fn_1_3154(M574_STAGE_OBJ *p) {
    s32 i;
    s16 modelId;
    M574_STAGE_SLOT *s;
    M574_STAGE_TBL *tbl;
    Vec tmp;

    tbl = p->unk5C;
    i = 0;
    while (i < 6) {
        s = &tbl->slotA[i];
        modelId = ((s16 *)p->unk40)[i + 0x15];
        if (Hu3DMotionEndCheck(modelId)) {
            Hu3DModelAttrSet(modelId, 0x40000001);
            Hu3DMotionStartEndSet(modelId, 0.0f, 60.0f);
            Hu3DMotionTimeSet(modelId, 0.0f);
        }
        PSVECScale(&s->v1C, &s->v1C, 0.2f);
        PSVECAdd(&s->v10, &s->v1C, &tmp);
        Hu3DModelPosSetV(modelId, &tmp);
        i++;
    }
}

s32 fn_1_3240(M574_STAGE_OBJ *p, M574_STAGE_OBJ *p2) {
    s32 idx;
    M574_STAGE_OBJ *stage;
    s16 modelId;
    M574_STAGE_SLOT *slot;
    M574_STAGE_TBL *tbl;

    idx = p->unk08;
    stage = lbl_1_bss_48;
    tbl = stage->unk5C;
    slot = &tbl->slotA[idx];
    modelId = ((s16 *)stage->unk40)[idx + 0x15];
    Hu3DModelAttrReset(modelId, 0x40000001);
    Hu3DMotionStartEndSet(modelId, 60.0f, Hu3DMotionMaxTimeGet(modelId));
    Hu3DMotionTimeSet(modelId, 60.0f);
    PSVECNormalize(p2->v1C, &slot->v1C);
    PSVECScale(&slot->v1C, &slot->v1C, 50.0f);
    fn_1_77D4(lbl_1_data_450[idx], lbl_1_data_468[idx]);
    if (*p2->unk18 < 0.0f) {
        fn_1_C10(0x918, p2->unk18);
    } else {
        fn_1_C10(0x919, p2->unk18);
    }
    return 0;
}

void fn_1_3374(M574_STAGE_OBJ *p) {
    M574_STAGE_TBL *unused;
    M574_STAGE_ENT2 *ent;
    M574_STAGE_TBL *tbl;
    HU3DMODELID id2;
    HU3DMODELID model;
    M574_MODEL_OBJ *obj;
    s32 i;

    tbl = p->unk5C;
    for (i = 0; i < 2; i++) {
        ent = &tbl->ent2[i];
        ent->v00 = 0;
        model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_42C[i + 7],
                                                     HU_MEMNUM_OVL, HEAP_MODEL));
        ((s16 *)p->unk40)[i + 0x1B] = model;
        Hu3DModelAttrSet(model, 0x40000002);
        Hu3DModelAttrSet(model, 0x40000080);
        Hu3DMotionTimeSet(model, lbl_1_rodata_128);
        Hu3DMotionShapeTimeSet(model, lbl_1_rodata_128);
        Hu3DModelLayerSet(model, 4);
        unused = p->unk5C;
        id2 = p->unk40->id08;
        obj = (M574_MODEL_OBJ *)Hu3DModelObjPtrGet(id2, lbl_1_data_3D0[i + 7]);
        if (obj != (M574_MODEL_OBJ *)0) {
            Hu3DModelPosSet(model, obj->v1C.x, obj->v1C.y, obj->v1C.z);
        }
        (void)i;
        (void)i;
        (void)i;
        (void)i;
    }
}

void fn_1_34B0(M574_STAGE_OBJ *p) {
    /* 1 seule ligne differente : `&= 0xE0000000` ; MWCC 1.3.2 choisit
       clrrwi (clear bits bas) la ou CodeWarrior emet rlwinm 0,31,29. */
    M574_STAGE_SLOT *s;
    M574_SLOT_HOOK *h;
    s32 i;
    s16 id;
    M574_STAGE_TBL *tbl;

    tbl = p->unk5C;
    for (i = 0; i < 2; i++) {
        s = (M574_STAGE_SLOT *)&tbl->ent2[i];
        id = ((s16 *)p->unk40)[i + 0x1B];
        h = s->v48;
        switch (s->v00) {
        case 0:
            break;
        case 1:
            s->v00++;
            Hu3DModelAttrReset(id, 0x40000002);
            Hu3DModelAttrReset(id, 0x40000080);
            Hu3DMotionTimeSet(id, 0.0f);
            Hu3DMotionShapeTimeSet(id, 0.0f);
            if (i == 0) {
                HuAudFXPlay(0x927);
            } else {
                HuAudFXPlay(0x928);
            }
        case 2:
            s->f34 = s->f34 - 20.0;
            if (Hu3DMotionTimeGet(id) < 3.0) {
                break;
            }
            s->v00++;
            s->f34 = 0.0f;
        case 3:
            if (Hu3DMotionTimeGet(id) < 19.0) {
                break;
            }
            s->v00++;
            ((f32 *)h->v1C)[0] = 0.0f;
            ((f32 *)h->v1C)[1] = 333.33334f;
            h->v00 &= ~2;
            h = 0;
            s->v48 = h;
            if (i == 0) {
                HuAudFXPlay(0x929);
            } else {
                HuAudFXPlay(0x92A);
            }
        case 4:
            if (Hu3DMotionTimeGet(id) < 22.0) {
                break;
            }
            s->v00 = 0;
            break;
        }
        if (h != (M574_SLOT_HOOK *)0) {
            ((f32 *)h->v1C)[0] = 0.0f;
            ((f32 *)h->v1C)[1] = 0.0f;
            ((f32 *)h->v18)[0] = s->v10.x;
            ((f32 *)h->v18)[1] = s->v10.y + s->f34;
        }
    }
}

s32 fn_1_3704(M574_STAGE_OBJ *p, M574_STAGE_ENT3 *q) {
    M574_STAGE_ENT2 *ent;
    f32 *v;
    M574_STAGE_OBJ *stage;
    s32 flag;
    M574_STAGE_TBL *tbl;

    if ((q->v00 & 2) == 0) {
        stage = lbl_1_bss_48;
        tbl = stage->unk5C;
        if (p->unk08 == 0) {
            flag = 0;
        } else {
            flag = 1;
        }
        ent = &tbl->ent2[flag];
        if (ent->v00 == 0) {
            ent->v00++;
            ent->v48 = q;
            q->v00 |= 2;
            q->v1C->x = 0.0f;
            q->v1C->y = 0.0f;
            v = p->unk38;
            ent->f10 = v[0] + 0.5 * (v[3] - v[0]);
            ent->f14 = v[1] + 0.5 * (v[4] - v[1]);
            ent->f34 = 80.0f;
            return 1;
        }
    }
    return 0;
}

void fn_1_383C(M574_STAGE_OBJ *p) {
    /* Non-identique : MWCC 1.3.2 repartit val/tbl/obj en r26/r25/r24 la ou
       CodeWarrior met tbl/obj/val ; 7 lignes, rename de registres seule. */
    M574_STAGE_CELL *cell;
    s32 j;
    s32 i;
    HU3DMODELID model;
    M574_STAGE_TBL *tbl;
    M574_MODEL_OBJ *obj;
    s32 val;
    s16 modelId;

    tbl = p->unk5C;
    modelId = p->unk40->id0A;
    for (i = 0; i < 2; i++) {
        cell = &tbl->cell[i];
        cell->v0C = -1;
        for (j = 0; j < 3; j++) {
            obj = (M574_MODEL_OBJ *)Hu3DModelObjPtrGet(modelId, lbl_1_data_2D8[j + i * 3]);
            cell->v50[j] = obj;
            val = (s32)(20.0 * (1.52587890625e-05 * (f32)(s32)(u16)frand()));
            cell->v20[j] = val;
            cell->f2C[j] = cell->f38[j] = 18.0f * (f32)val;
        }
        model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_314[i], 0x10000000, 2));
        ((s16 *)p->unk40)[i + 0x1D] = model;
        Hu3DModelLayerSet(model, 4);
        Hu3DModelAttrSet(model, 0x40000002);
        Hu3DMotionStartEndSet(model, lbl_1_rodata_128, lbl_1_rodata_178);
        Hu3DMotionTimeSet(model, lbl_1_rodata_128);
    }
}

s32 fn_1_4330(f32 f) {
    s32 i;

    f += 9.0;
    if (f >= 360.0f) {
        f -= 360.0f;
    }
    if (f < 0.0f) {
        f += 360.0f;
    }
    i = f / 18.0f;
    if (i < 0) {
        i = 0x13;
    }
    if (i >= 0x14) {
        i = 0;
    }
    return lbl_1_data_2C4[i];
}

void fn_1_43E8(M574_STAGE_OBJ *p, s32 idx) {
    M574_STAGE_CELL *c;
    M574_STAGE_TBL *tbl = p->unk5C;

    c = &tbl->cell[idx];

    if (MgSeqModeGet() == 5 && c->v00 == 0) {
        c->v00++;
    }
}

s32 fn_1_445C(M574_STAGE_OBJ *p, s32 idx) {
    M574_STAGE_CELL *c;
    M574_STAGE_TBL *tbl = p->unk5C;

    c = &tbl->cell[idx];
    return c->v10;
}

void fn_1_448C(M574_STAGE_OBJ *p) {
    M574_STAGE_SLOT *slot;
    M574_STAGE_HOOKENT *ent;
    s32 i;
    M574_MODEL_OBJ *obj;
    HU3DMODELID id;
    s32 idx;
    s32 a;
    M574_STAGE_TBL *tbl;

    tbl = p->unk5C;
    id = Hu3DModelCreate(HuDataSelHeapReadNum(0x00810017, 0x10000000, 2));
    p->unk40->id0E = id;
    Hu3DModelAttrSet(id, 1);
    for (i = 0; i < 4; i++) {
        obj = (M574_MODEL_OBJ *)Hu3DModelObjPtrGet(id, lbl_1_data_3A8[i]);
        if (obj != (M574_MODEL_OBJ *)0) {
            ent = &tbl->hookEnt[i];
            ent->hook.v00 = 1;
            ent->hook.v04 = 4;
            ent->hook.v08 = i;
            ent->hook.v0C = 1;
            ent->hook.v10 = p;
            ent->hook.v14 = 0;
            ent->hook.v18 = fn_1_47DC;
            fn_1_8FA8(&ent->hook, obj);
            fn_1_8FDC(&ent->hook);
            fn_1_8B48(&ent->hook);
        }
    }
    id = p->unk40->id0A;
    idx = 0;
    for (a = 0; a < 2; a++) {
        for (i = 0; i < 7; i++) {
            slot = &tbl->slotB[idx];
            obj = (M574_MODEL_OBJ *)Hu3DModelObjPtrGet(id, lbl_1_data_31C[idx]);
            slot->v48 = obj;
            if (obj != (M574_MODEL_OBJ *)0) {
                slot->v10.x = obj->v1C.x;
                slot->v10.y = obj->v1C.y;
                slot->v10.z = lbl_1_rodata_128;
            }
            slot->v00 = 0;
            slot->f34 = lbl_1_rodata_128;
            slot->f3C = slot->f40 = slot->f44 = lbl_1_rodata_128;
            slot->v1C.x = slot->v1C.y = slot->v1C.z = lbl_1_rodata_128;
            idx++;
        }
    }
}

void fn_1_4660(M574_STAGE_OBJ *p) {
    M574_STAGE_SLOT *slot;
    M574_SLOT_PTR *ptr;
    s32 i;
    s32 j;
    s32 k;
    M574_STAGE_TBL *tbl;

    tbl = p->unk5C;
    k = 0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 7; j++) {
            slot = &tbl->slotB[k];
            ptr = slot->v48;
            if (ptr != (M574_SLOT_PTR *)0) {
                switch (slot->v00) {
                case 0:
                    break;
                case 1:
                    slot->v1C.y = lbl_1_rodata_1F0;
                    slot->v00++;
                    break;
                case 2:
                    slot->f34 += slot->v1C.y;
                    slot->v1C.y = slot->v1C.y - lbl_1_rodata_1F8;
                    if (slot->f34 > 0.0f) {
                    } else {
                        slot->f34 = 0.0f;
                        slot->v00 = 0;
                    }
                    break;
                }
                if (j < 5) {
                    slot->f40 = slot->f34;
                } else if (i == 0) {
                    slot->f3C = -slot->f34;
                } else {
                    slot->f3C = slot->f34;
                }
                ptr->f1C = slot->v10.x + slot->f3C;
                ptr->f20 = slot->v10.y + slot->f40;
                slot->v04++;
                k++;
            }
        }
    }
}

void fn_1_4D28(M574_STAGE_OBJ *p) {
    M574_SLOT_HOOK *h;
    M574_STAGE_SLOT *slot;
    HU3DMODELID mot;
    s32 i;
    s32 j;
    HU3DMODELID model;
    M574_STAGE_TBL *tbl;
    HU3DMOTID joint;

    tbl = p->unk5C;
    for (i = 0; i < 2; i++) {
        slot = &tbl->slot[i];
        if (i == 0) {
            model = Hu3DModelCreate(HuDataSelHeapReadNum(0x81 << 16 | 0x1A,
                                                       HU_MEMNUM_OVL, HEAP_MODEL));
            mot = model;
            for (j = 0; j < 3; j++) {
                joint = Hu3DJointMotion(mot, HuDataSelHeapReadNum(lbl_1_data_480[j],
                                            HU_MEMNUM_OVL, HEAP_MODEL));
                ((s16 *)p->unk48)[j] = joint;
            }
        } else {
            mot = Hu3DModelLink(model);
        }
        ((s16 *)p->unk40)[i + 0x1F] = mot;
        Hu3DMotionSet(mot, p->unk48->v00);
        Hu3DModelAttrSet(mot, 0x40000001);
        Hu3DModelLayerSet(mot, 4);
        Hu3DModelShadowSet(mot);
        slot->v00 = 0;
        slot->v10 = lbl_1_data_48C[i];
        slot->f3C = slot->f40 = slot->f44 = lbl_1_rodata_160;
        h = &slot->hook;
        h->v00 = 1;
        h->v04 = 2;
        h->v08 = i;
        h->v0C = 0;
        h->v10 = p;
        h->v14 = 0;
        h->v18 = fn_1_5628;
        h->v1C = &slot->v10;
        h->v20 = lbl_1_rodata_218;
        fn_1_8B48(h);
        (void)tbl;
        (void)tbl;
    }
}

s32 fn_1_5628(M574_STAGE_OBJ *p) {
    M574_STAGE_OBJ *stage;
    M574_STAGE_SLOT *s;
    s32 idx;
    M574_STAGE_TBL *tbl;

    idx = p->unk08;
    stage = lbl_1_bss_48;
    tbl = stage->unk5C;
    s = &tbl->slot[idx];
    s->v38 = 1;
    return 0;
}

void fn_1_5680(f32 *x) {
    M574_STAGE_SLOT *slot;
    s32 i;
    M574_STAGE_OBJ *stage;
    M574_STAGE_TBL *tbl;
    f32 v1;
    f32 v2;
    f64 v3;

    stage = lbl_1_bss_48;
    tbl = stage->unk5C;
    i = 0;
    while (i < 2) {
        slot = &tbl->slot[i];
        v2 = slot->v10.x - *x;
        /* mwcceppc 1.3.2 always routes fabs() through a scratch FPR: the fabs/copy/frsp chain lands on f28/f29/f31 instead of f29/f28/f31. */
        v3 = fabs(v2);
        v1 = v3;
        if (v1 < 50.0) {
            slot->v38 = 2;
        }
        i++;
    }
}

void fn_1_5750(M574_STAGE_OBJ *p) {
    M574_STAGE_TBL *tbl;
    M574_STAGE_MOT *mot;
    HU3DMODELID model;
    s16 id1;
    M574_MODEL_OBJ *obj1;
    s16 id2;
    M574_MODEL_OBJ *obj2;
    M574_SLOT_HOOK *hook;
    M574_MODEL_OBJ *obj3;
    M574_STAGE_TBL *unused1;
    M574_STAGE_TBL *unused2;

    tbl = p->unk5C;
    mot = &tbl->mot;
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x00810005, 0x10000000, 2));
    p->unk40->id = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DMotionStartEndSet(model, lbl_1_rodata_128, lbl_1_rodata_178);
    Hu3DModelLayerSet(model, 4);
    unused1 = p->unk5C;
    id1 = p->unk40->id08;
    obj1 = (M574_MODEL_OBJ *)Hu3DModelObjPtrGet(id1, lbl_1_data_3D0[0]);
    if (obj1 != (M574_MODEL_OBJ *)0) {
        Hu3DModelPosSet(model, obj1->v1C.x, obj1->v1C.y, obj1->v1C.z);
    }
    unused2 = p->unk5C;
    id2 = p->unk40->id08;
    obj2 = (M574_MODEL_OBJ *)Hu3DModelObjPtrGet(id2, lbl_1_data_3D0[0]);
    if (obj2 != (M574_MODEL_OBJ *)0) {
        mot->v10.x = obj2->v1C.x;
        mot->v10.y = obj2->v1C.y;
        mot->v10.z = obj2->v1C.z;
    }
    hook = &mot->hook;
    hook->v00 = 1;
    hook->v04 = 1;
    hook->v08 = 100;
    hook->v0C = 0;
    hook->v10 = p;
    hook->v14 = 0;
    hook->v18 = fn_1_5D64;
    hook->v1C = &mot->v10;
    hook->v20 = 200.0f;
    fn_1_8B48(hook);
    mot->v00 = 0;
    mot->v38 = 0;
    mot->v1C.x = mot->v1C.y = mot->v1C.z = lbl_1_rodata_128;
    obj3 = (M574_MODEL_OBJ *)Hu3DModelObjPtrGet(model, lbl_1_data_4A4);
    mot->v48 = obj3;
    if (obj3 != (M574_MODEL_OBJ *)0) {
        mot->v3C.x = obj3->v1C.x;
        mot->v3C.y = obj3->v1C.y;
        mot->v3C.z = obj3->v1C.z;
    }
}

s32 fn_1_5D64(s32 unused, M574_STAGE_OBJ *p) {
    M574_STAGE_MOT *mot;
    M574_STAGE_OBJ *stage;
    s16 modelId;
    s32 flag;
    M574_STAGE_TBL *tbl;

    stage = lbl_1_bss_48;
    tbl = stage->unk5C;
    mot = &tbl->mot;
    modelId = stage->unk40->id;
    if (mot->v00 == 0) {
        mot->v38 = 1;
        if (*p->unk18 < 0.0f) {
            flag = 0;
        } else {
            flag = 1;
        }
        mot->v0C = flag;
        Hu3DModelAttrReset(modelId, 0x40000001);
        Hu3DMotionStartEndSet(modelId, 60.0f, Hu3DMotionMaxTimeGet(modelId));
        Hu3DMotionTimeSet(modelId, 60.0f);
        PSVECNormalize(p->v1C, &mot->v1C);
        PSVECScale(&mot->v1C, &mot->v1C, 50.0f);
    }
    return 0;
}

void fn_1_5E5C(M574_STAGE_OBJ *p) {
    M574_STAGE_TBL *tbl = p->unk5C;
    s16 modelId = p->unk40->id;

    tbl->texId = Hu3DTexScrollCreate(modelId, lbl_1_data_504.name);
    tbl->v0C = 5;
    tbl->f10 = lbl_1_rodata_254;
}

void fn_1_5ECC(M574_STAGE_OBJ *p) {
    M574_STAGE_TBL *tbl;
    f32 cur;
    f32 ux;
    f32 uy;
    f32 dv;
    f32 base;
    f32 adv;
    f32 m1;
    f32 m2;
    f32 m3;
    f32 m4;
    s32 idx;

    tbl = p->unk5C;
    /* The u32->f32 bias idiom (lis 0x4330 / lfd / fsubs 2^52) is what the original
       emits for both (f32)(v ^ 0x8000) conversions below. */
    base = (f32)tbl->v0C - tbl->f10;
    m1 = (f32)fmod(base, lbl_1_rodata_F0);
    m2 = m1;
    cur = m2;
    if (cur < lbl_1_rodata_258) {
        cur += lbl_1_rodata_F0;
    }
    if (cur > lbl_1_rodata_260) {
        cur -= lbl_1_rodata_F0;
    }
    adv = tbl->f10 + lbl_1_rodata_158 * cur;
    m3 = (f32)fmod(adv, lbl_1_rodata_F0);
    m4 = m3;
    cur = m4;
    if (cur < lbl_1_rodata_258) {
        cur += lbl_1_rodata_F0;
    }
    tbl->f10 = cur;
    idx = (s32)(lbl_1_rodata_130 + cur);
    dv = lbl_1_rodata_268 * (cur - (f32)idx);
    if (idx > 9) {
        idx = 0;
    }
    if (idx < 0) {
        idx = 0;
    }
    ux = lbl_1_data_510[idx].x;
    uy = dv + lbl_1_data_510[idx].y;
    Hu3DTexScrollPosSet(tbl->texId, ux, uy, lbl_1_rodata_128);
}

void fn_1_6168(M574_STAGE_OBJ *p) {
    M574_MOTNODE *nd;
    HU3DMODELID particle;
    s32 i;
    M574_MOTGRP *grp;
    s32 j;
    M574_STAGE_TBL *tbl;
    ANIMDATA *anim;

    tbl = p->unk5C;
    anim = HuSprAnimDataRead(0x81 << 16 | 0x20);
    for (i = 0; i < 5; i++) {
        particle = Hu3DParticleCreate(anim, 8);
        tbl->ids[i] = particle;
        grp = ((M574_HU3DMODEL *)((u8 *)Hu3DData + particle * 0x134))->grp120;
        Hu3DParticleBlendModeSet(particle, HU3D_PARTICLE_BLEND_NORMAL);
        Hu3DParticleHookSet(particle, (HU3DPARTICLEHOOK)fn_1_62A0);
        Hu3DModelLayerSet(particle, 7);
        Hu3DModelPosSet(particle, lbl_1_rodata_128, lbl_1_rodata_128,
                        lbl_1_rodata_128);
        Hu3DParticleAnimModeSet(particle, 4 - i);
        for (nd = grp->nd48, j = 0; j < grp->cnt30; j++, nd++) {
            nd->f00 = -1;
            nd->f2C = lbl_1_rodata_128;
        }
        (void)nd;
        (void)nd;
        (void)nd;
        (void)nd;
    }
}

void fn_1_629C(M574_STAGE_OBJ *p) {
}

void fn_1_62A0(s32 unused, M574_STAGE_OBJ *p) {
    M574_STAGE_ENT *e;
    s32 i;

    e = p->unk48;
    i = 0;
    while (i < p->v30) {
        switch (e->v00) {
        case 0:
            e->v00++;
            e->f2C = 200.0f;
            e->v43 = 0xB4;
        case 1:
            e->f2C -= 10.0f;
            e->v43 -= 4;
            if (e->f2C <= 0.0f || e->v43 > 0xF0) {
                e->v00 = -1;
                e->f2C = 0.0f;
            } else {
                PSVECAdd(&e->v34, &e->v14, &e->v34);
            }
            break;
        }
        i++;
        e++;
    }
}

void fn_1_63B0(M574_VEC3 *a, M574_VEC3 *b, s32 idx) {
    M574_MOTNODE *nd;
    M574_MOTGRP *grp;
    s32 i;
    M574_STAGE_TBL *tbl;

    tbl = lbl_1_bss_48->unk5C;
    grp = ((M574_HU3DMODEL *)((u8 *)Hu3DData + tbl->ids[4 - idx] * 0x134))->grp120;
    nd = grp->nd48;
    i = 0;
    while (i < grp->cnt30) {
        if (nd->f00 < 0) {
            nd->f00 = 0;
            nd->pos = *a;
            nd->rot = *b;
            break;
        }
        i++;
        nd++;
    }
}

void fn_1_6478(s32 idx, f32 f) {
    M574_STAGE_ROW *row;
    M574_STAGE_TBL *tbl;
    f32 v;

    tbl = lbl_1_bss_48->unk5C;
    row = &tbl->row[idx];
    if (f - row->f00 > 0.2) {
        HuAudFXPlay(lbl_1_data_560[idx]);
    }
    row->f00 = f;
    v = 60.0 * f;
    if (idx & 1) {
        v = -v;
    }
    row->f0C = v + lbl_1_data_280[idx];
}

f32 fn_1_6568(s32 idx) {
    M574_STAGE_ROW *row;
    M574_STAGE_TBL *tbl;
    f32 v;

    tbl = lbl_1_bss_48->unk5C;
    row = &tbl->row[idx];
    if (idx & 1) {
        v = lbl_1_data_280[idx] - row->f08;
    } else {
        v = row->f08 - lbl_1_data_280[idx];
    }
    v = v / 60.0;
    if (v < 0.0f) {
        v = 0.0f;
    }
    if (v > 1.0f) {
        v = 1.0f;
    }
    return v;
}
