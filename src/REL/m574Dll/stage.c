#include "REL/m574Dll.h"
#include "game/mg/seqman.h"
#include "game/frand.h"

typedef struct M574StageCell_s {
    s32 v00;
    s32 v04;
    s32 v08;
    s32 v0C;
    s32 v10;
    s32 v14[3];
    s32 v20[3];
    f32 f2C[3];
    f32 f38[3];
    f32 f44[3];
    HSFOBJECT *v50[3];
} M574_STAGE_CELL;

typedef union M574PtrU_s {
    HSFOBJECT *obj;
    M574_COLLIDER *col;
} M574_PTRU;

typedef struct M574StageSlot_s {
    s32 v00;
    s32 v04;
    u8 pad08[0x08];
    Vec v10;
    Vec v1C;
    Vec v28;
    f32 f34;
    s32 v38;
    f32 f3C;
    f32 f40;
    f32 f44;
    M574_PTRU v48;
    M574_COL_CIRCLE hook;
    u8 pad70[0x18];
} M574_STAGE_SLOT;

typedef struct M574StageRow_s {
    f32 f00;
    f32 f04;
    f32 f08;
    f32 f0C;
    s32 v10;
    M574_COL_WALL hook;
} M574_STAGE_ROW;

typedef struct M574StageMot_s {
    s32 v00;
    s32 v04;
    s32 v08;
    s32 v0C;
    Vec v10;
    Vec v1C;
    u8 pad28[0x10];
    s32 v38;
    Vec v3C;
    HSFOBJECT *v48;
    M574_COL_CIRCLE hook;
    u8 pad70[0x18];
} M574_STAGE_MOT;

typedef struct M574StageTbl_s {
    s32 timer[2];
    HU3DTEXSCRID texId;
    s32 v0C;
    f32 f10;
    M574_STAGE_MOT mot;
    s16 ids[5];
    u8 padA6[0x02];
    M574_STAGE_ROW row[4];
    M574_STAGE_CELL cell[2];
    M574_STAGE_SLOT ent2[2];
    M574_STAGE_SLOT slotB[14];
    M574_STAGE_SLOT slotA[6];
    M574_STAGE_SLOT slot[2];
    M574_COL_WALL hookF60;
    M574_COL_WALL hookEnt[4];
} M574_STAGE_TBL;

typedef struct M574ScrollUv_s {
    f32 x;
    f32 y;
} M574_SCROLL_UV;

typedef struct M574TexAnimDef_s {
    char name[12];
    f32 f[20];
    s32 datanum[4];
} M574_TEXANIM_DEF;

OMOBJ *lbl_1_bss_48;

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

s32 lbl_1_data_2B8[3] = { 0, 1, 2 };

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

Vec lbl_1_data_4B0[2] = {
    { -95.0f, 1215.0f, 1000.0f },
    { 95.0f, 1215.0f, 1000.0f }
};

Vec lbl_1_data_4C8[2] = {
    { 3.0f, 3.0f, -1.0f },
    { -3.0f, 3.0f, -1.0f }
};

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
s32 fn_1_4330(f32 f);
void fn_1_7874(s32 grp, s32 type);
f32 fn_1_CC8(f32 cur, f32 target, f32 rate);
void fn_1_C10(s32 a, void *b);

void fn_1_20CC(OMOBJ *p);
void fn_1_2364(OMOBJ *p);
void fn_1_2928(OMOBJ *p);
void fn_1_2F8C(OMOBJ *p);
void fn_1_3374(OMOBJ *p);
void fn_1_383C(OMOBJ *p);
void fn_1_448C(OMOBJ *p);
void fn_1_4D28(OMOBJ *p);
void fn_1_5750(OMOBJ *p);
void fn_1_5E5C(OMOBJ *p);
void fn_1_6168(OMOBJ *p);
void fn_1_2B88(OMOBJ *p);
void fn_1_26F4(OMOBJ *p);
void fn_1_3154(OMOBJ *p);
void fn_1_34B0(OMOBJ *p);
void fn_1_3A1C(OMOBJ *p);
void fn_1_4660(OMOBJ *p);
void fn_1_4EE8(OMOBJ *p);
void fn_1_5934(OMOBJ *p);
void fn_1_5ECC(OMOBJ *p);
void fn_1_629C(OMOBJ *p);
s32 fn_1_3240(M574_COL_CIRCLE *c, M574_COLLIDER *col);
s32 fn_1_5628(M574_COL_CIRCLE *c, M574_COLLIDER *col);
void fn_1_62A0(HU3DMODEL *model, HU3DPARTICLE *p, Mtx mtx);
u32 frand(void);
double sin(double x);
double cos(double x);
s32 fn_1_5D64(M574_COL_CIRCLE *c, M574_COLLIDER *col);
f32 fn_1_6568(s32 idx);
s32 fn_1_47DC(M574_COL_WALL *wall, M574_COLLIDER *col);
void fn_1_5680(f32 *x);
void fn_1_63B0(Vec *a, Vec *b, s32 idx);
s32 fn_1_6CC8(s32 idx);
s32 fn_1_918(void);
s32 fn_1_2D88(M574_COL_WALL *wall, M574_COLLIDER *col);

void fn_1_2028(OMOBJMAN *objMan) {
    OMOBJ *obj;
    M574_STAGE_TBL *tbl;

    obj = omAddObjEx(objMan, 0x14, 0x21, 3, OM_GRP_NONE, fn_1_20CC);
    lbl_1_bss_48 = obj;
    obj->stat |= OM_STAT_MODELPAUSE;
    tbl = HuMemDirectMallocNum(0, sizeof(M574_STAGE_TBL), 0x10000000);
    obj->data = tbl;
    memset(tbl, 0, sizeof(M574_STAGE_TBL));
    obj->work[0] = 0;
}

void fn_1_20C8(void) {
}

void fn_1_20CC(OMOBJ *p) {
    HU3DMODELID model;
    M574_COL_WALL *h;
    M574_STAGE_TBL *tbl;

    tbl = p->data;
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x00810000, HU_MEMNUM_OVL, HEAP_MODEL));
    p->mdlId[0] = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 3);
    Hu3DModelShadowMapSet(model);
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x00810001, HU_MEMNUM_OVL, HEAP_MODEL));
    p->mdlId[1] = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 3);
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x00810002, HU_MEMNUM_OVL, HEAP_MODEL));
    p->mdlId[2] = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 7);
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x00810003, HU_MEMNUM_OVL, HEAP_MODEL));
    p->mdlId[3] = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 7);
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x00810006, HU_MEMNUM_OVL, HEAP_MODEL));
    p->mdlId[4] = model;
    Hu3DModelAttrSet(model, 1);
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x00810004, HU_MEMNUM_OVL, HEAP_MODEL));
    p->mdlId[5] = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelLayerSet(model, 3);
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x00810016, HU_MEMNUM_OVL, HEAP_MODEL));
    p->mdlId[6] = model;
    Hu3DModelAttrSet(model, 1);
    h = &tbl->hookF60;
    h->flag = 1;
    h->shape = 5;
    h->id = 0;
    h->kind = 1;
    h->user = p;
    h->unk14 = 0;
    h->hook = 0;
    fn_1_8F54(h, model);
    fn_1_8FDC(h);
    fn_1_8B48((M574_COLHEAD *)h);
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
    p->objFunc = fn_1_2364;
}

void fn_1_23E8(OMOBJ *p, s16 idx, s32 n) {
    M574_STAGE_TBL *unused = p->data;
    HSFOBJECT *pos;
    s16 modelId;

    modelId = p->mdlId[4];
    pos = Hu3DModelObjPtrGet(modelId, &lbl_1_data_3D0[n][0]);
    if (pos != NULL) {
        Hu3DModelPosSet(idx, pos->mesh.base.pos.x, pos->mesh.base.pos.y, pos->mesh.base.pos.z);
    }
}

void fn_1_2478(OMOBJ *p, f32 *out, s32 n) {
    M574_STAGE_TBL *unused = p->data;
    HSFOBJECT *pos;
    s16 modelId;

    modelId = p->mdlId[4];
    pos = Hu3DModelObjPtrGet(modelId, &lbl_1_data_3D0[n][0]);
    if (pos != NULL) {
        out[0] = pos->mesh.base.pos.x;
        out[1] = pos->mesh.base.pos.y;
        out[2] = pos->mesh.base.pos.z;
    }
}

void fn_1_2514(OMOBJ *p, s32 flag, s32 n) {
    M574_STAGE_MOT *mot;
    s32 i;
    M574_STAGE_TBL *tbl;
    f32 base;
    Vec v;
    Vec vel;

    tbl = p->data;
    mot = &tbl->mot;
    v.x = mot->v10.x;
    v.y = mot->v10.y - 150.0;
    v.z = 300.0f;
    vel.z = lbl_1_rodata_E4;
    for (i = 0; i < n; i++) {
        if (flag == 0) {
            base = 240.0f;
        } else {
            base = 120.0f;
        }
        base = base + 10.0 * (3.0517578125e-05 * (f32)((u16)FRAND16() - 0x8000));
        vel.x = 100.0 * (0.2 * sin(3.141592653589793 * base / 180.0));
        vel.y = 100.0 * (0.2 * cos(3.141592653589793 * base / 180.0));
        fn_1_6C2C(&v, &vel);
    }
}

void fn_1_2364(OMOBJ *p) {
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
    p->work[0]++;
}

void fn_1_26F4(OMOBJ *p) {
    /* Non-identique : tie-break de registres. En donnant un nom au second
       `p->data` (t2) on obtient _savegpr_26 et le scratch r26 comme le
       code original ; il reste 8 lignes, toutes des renommages : MWCC donne
       tbl=r29/mot=r30 la ou CodeWarrior donne tbl=r30/mot=r29. L'ordre de
       declaration de tbl/mot/t2 ne change rien a l'objet produit. */
    s32 i;
    M574_STAGE_TBL *t2;
    M574_STAGE_MOT *mot;
    M574_STAGE_TBL *tbl;
    s32 j;
    f32 base;
    Vec vel;
    Vec pos;

    tbl = p->data;
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
            t2 = p->data;
            mot = &t2->mot;
            pos.x = mot->v10.x;
            pos.y = mot->v10.y - 150.0;
            pos.z = 300.0f;
            vel.z = 80.0f;
            for (j = 0; j < 1; j++) {
                if (i == 0) {
                    base = 240.0f;
                } else {
                    base = 120.0f;
                }
                base = base + 10.0 * (3.0517578125e-05 * (f32)((u16)FRAND16() - 0x8000));
                vel.x = 100.0 * (0.2 * sin(3.141592653589793 * base / 180.0));
                vel.y = 100.0 * (0.2 * cos(3.141592653589793 * base / 180.0));
                fn_1_6C2C(&pos, &vel);
            }
            ((s32 *)tbl)[i] = 0x78;
        }
    }
}

void fn_1_2B88(OMOBJ *p) {
    M574_STAGE_ROW *row;
    s32 i;
    s16 id;
    HSFOBJECT *obj;
    M574_STAGE_TBL *tbl;
    f32 fv;

    tbl = p->data;
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
        id = p->mdlId[i + 9];
        Hu3DModelRotSet(id, 0.0f, 0.0f, row->f08);
        id = p->mdlId[i + 0x11];
        fv = fn_1_6568(i);
        Hu3DMotionTimeSet(id, 30.0 * (1.0 - fv));
        if (row->v10 == 0) {
            if (MgSeqModeGet() == 6) {
                row->v10 = 1;
                Hu3DModelLayerSet(id, 3);
                id = p->mdlId[2];
                Hu3DModelLayerSet(id, 3);
            }
        }
        id = p->mdlId[i + 0xD];
        obj = Hu3DModelObjPtrGet(id, lbl_1_data_290[i]);
        if (obj != NULL) {
            obj->mesh.base.rot.z = row->f08;
        }
        fn_1_8FDC(&row->hook);
    }
}

void fn_1_2928(OMOBJ *p) {
    s32 i;
    M574_COL_WALL *h;
    HU3DMODELID model;
    HSFOBJECT *o;
    M574_STAGE_TBL *tbl;

    tbl = p->data;
    for (i = 0; i < 4; i++) {
        model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_220[i], HU_MEMNUM_OVL, HEAP_MODEL));
        p->mdlId[i + 9] = model;
        Hu3DModelLayerSet(model, 4);
        Hu3DModelShadowSet(model);
        Hu3DModelPosSetV(model, (HuVecF *)lbl_1_data_250[i]);
        tbl->row[i].f08 = tbl->row[i].f0C = lbl_1_data_280[i];
        model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_240[i], HU_MEMNUM_OVL, HEAP_MODEL));
        p->mdlId[i + 0x11] = model;
        Hu3DModelLayerSet(model, 6);
        Hu3DModelShadowSet(model);
        Hu3DModelAttrSet(model, 0x40000002);
        Hu3DMotionTimeSet(model, 0.0f);
        model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_230[i], HU_MEMNUM_OVL, HEAP_MODEL));
        p->mdlId[i + 0xD] = model;
        Hu3DModelAttrSet(model, 1);
        o = Hu3DModelObjPtrGet(model, lbl_1_data_290[i]);
        if (o != NULL) {
            o->mesh.base.pos.x = lbl_1_data_250[i][0];
            o->mesh.base.pos.y = lbl_1_data_250[i][1];
            o->mesh.base.pos.z = lbl_1_data_250[i][2];
        }
        h = &tbl->row[i].hook;
        h->flag = 1;
        h->shape = 6;
        h->id = i;
        h->kind = 1;
        h->user = p;
        h->unk14 = 0;
        h->hook = fn_1_2D88;
        fn_1_8F54(h, model);
        fn_1_8FDC(h);
        fn_1_8FDC(h);
        fn_1_8B48((M574_COLHEAD *)h);
    }
}

s32 fn_1_2D88(M574_COL_WALL *wall, M574_COLLIDER *col) {
    /* Non-identique : MWCC 1.3.2 charge les deux litteraux (100.0 puis 0.5)
       en tete d'expression et fait `fmul f0, f0, f32` (litteral en operande A)
       la ou CodeWarrior charge 0.5 apres le `fsubs` et ecrit
       `fmul f0, f1, f0`. 17 lignes, choix d'operandes FPR uniquement. */
    s32 idx;
    M574_STAGE_ROW *row;
    M574_STAGE_TBL *tbl;
    f32 scale;

    tbl = lbl_1_bss_48->data;
    idx = wall->id;
    row = &tbl->row[idx];
    if (wall->lastPlane->n.y > 0.3) {
        if (row->f04 >= 0.1) {
            scale = 100.0 * ((col->pos->x - lbl_1_data_250[idx][0]) * 0.5);
            if (idx & 1) {
                scale = -scale;
            }
            if (scale > 0.1) {
                if (scale > 1.0) {
                    scale = 1.0f;
                }
                col->vel->x = col->vel->x -
                    50.0 * wall->lastPlane->n.x * 100.0 * 0.01666666753590107 * scale;
                col->vel->y = col->vel->y -
                    50.0 * wall->lastPlane->n.y * 100.0 * 0.01666666753590107 * scale;
                if (idx & 1) {
                    scale = -scale;
                }
                col->vel->x = col->vel->x + 100.0 * (0.1 * scale);
            }
        }
    }
    return 0;
}

void fn_1_2F8C(OMOBJ *p) {
    M574_STAGE_TBL *unused2;
    M574_STAGE_TBL *unused1;
    M574_COL_CIRCLE *h;
    s32 i;
    M574_STAGE_SLOT *slot;
    HSFOBJECT *obj;
    HSFOBJECT *obj2;
    HU3DMODELID model;
    M574_STAGE_TBL *tbl;
    HU3DMODELID id;
    HU3DMODELID id2;

    tbl = p->data;
    for (i = 0; i < 6; i++) {
        slot = &tbl->slotA[i];
        model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_42C[i + 1],
                                                     HU_MEMNUM_OVL, HEAP_MODEL));
        p->mdlId[i + 0x15] = model;
        Hu3DModelLayerSet(model, 4);
        Hu3DModelAttrSet(model, 0x40000001);
        Hu3DMotionStartEndSet(model, lbl_1_rodata_128, lbl_1_rodata_178);
        unused2 = p->data;
        id = p->mdlId[4];
        obj = Hu3DModelObjPtrGet(id, lbl_1_data_3D0[i + 1]);
        if (obj != NULL) {
            Hu3DModelPosSet(model, obj->mesh.base.pos.x, obj->mesh.base.pos.y, obj->mesh.base.pos.z);
        }
        unused1 = p->data;
        id2 = p->mdlId[4];
        obj2 = Hu3DModelObjPtrGet(id2, lbl_1_data_3D0[i + 1]);
        if (obj2 != NULL) {
            slot->v10.x = obj2->mesh.base.pos.x;
            slot->v10.y = obj2->mesh.base.pos.y;
            slot->v10.z = obj2->mesh.base.pos.z;
        }
        h = &slot->hook;
        h->flag = 1;
        h->shape = 3;
        h->id = i;
        h->kind = 0;
        h->user = p;
        h->unk14 = 0;
        h->hook = fn_1_3240;
        h->pos = &slot->v10;
        h->radius = lbl_1_rodata_E4;
        fn_1_8B48((M574_COLHEAD *)h);
    }
}

void fn_1_3154(OMOBJ *p) {
    s32 i;
    s16 modelId;
    M574_STAGE_SLOT *s;
    M574_STAGE_TBL *tbl;
    Vec tmp;

    tbl = p->data;
    i = 0;
    while (i < 6) {
        s = &tbl->slotA[i];
        modelId = p->mdlId[i + 0x15];
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

s32 fn_1_3240(M574_COL_CIRCLE *c, M574_COLLIDER *col) {
    s32 idx;
    OMOBJ *stage;
    s16 modelId;
    M574_STAGE_SLOT *slot;
    M574_STAGE_TBL *tbl;

    idx = c->id;
    stage = lbl_1_bss_48;
    tbl = stage->data;
    slot = &tbl->slotA[idx];
    modelId = stage->mdlId[idx + 0x15];
    Hu3DModelAttrReset(modelId, 0x40000001);
    Hu3DMotionStartEndSet(modelId, 60.0f, Hu3DMotionMaxTimeGet(modelId));
    Hu3DMotionTimeSet(modelId, 60.0f);
    PSVECNormalize(col->vel, &slot->v1C);
    PSVECScale(&slot->v1C, &slot->v1C, 50.0f);
    fn_1_77D4(lbl_1_data_450[idx], lbl_1_data_468[idx]);
    if (col->pos->x < 0.0f) {
        fn_1_C10(0x918, col->pos);
    } else {
        fn_1_C10(0x919, col->pos);
    }
    return 0;
}

void fn_1_3374(OMOBJ *p) {
    M574_STAGE_TBL *unused;
    M574_STAGE_SLOT *ent;
    M574_STAGE_TBL *tbl;
    HU3DMODELID id2;
    HU3DMODELID model;
    HSFOBJECT *obj;
    s32 i;

    tbl = p->data;
    for (i = 0; i < 2; i++) {
        ent = &tbl->ent2[i];
        ent->v00 = 0;
        model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_42C[i + 7],
                                                     HU_MEMNUM_OVL, HEAP_MODEL));
        p->mdlId[i + 0x1B] = model;
        Hu3DModelAttrSet(model, 0x40000002);
        Hu3DModelAttrSet(model, 0x40000080);
        Hu3DMotionTimeSet(model, lbl_1_rodata_128);
        Hu3DMotionShapeTimeSet(model, lbl_1_rodata_128);
        Hu3DModelLayerSet(model, 4);
        unused = p->data;
        id2 = p->mdlId[4];
        obj = Hu3DModelObjPtrGet(id2, lbl_1_data_3D0[i + 7]);
        if (obj != NULL) {
            Hu3DModelPosSet(model, obj->mesh.base.pos.x, obj->mesh.base.pos.y, obj->mesh.base.pos.z);
        }
        (void)i;
        (void)i;
        (void)i;
        (void)i;
    }
}

void fn_1_34B0(OMOBJ *p) {
    /* 1 seule ligne differente : `&= 0xE0000000` ; MWCC 1.3.2 choisit
       clrrwi (clear bits bas) la ou CodeWarrior emet rlwinm 0,31,29. */
    M574_STAGE_SLOT *s;
    M574_COLLIDER *h;
    s32 i;
    s16 id;
    M574_STAGE_TBL *tbl;

    tbl = p->data;
    for (i = 0; i < 2; i++) {
        s = &tbl->ent2[i];
        id = p->mdlId[i + 0x1B];
        h = s->v48.col;
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
            h->vel->x = 0.0f;
            h->vel->y = 333.33334f;
            h->flag &= ~2;
            h = 0;
            s->v48.col = h;
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
        if (h != (M574_COLLIDER *)0) {
            h->vel->x = 0.0f;
            h->vel->y = 0.0f;
            h->pos->x = s->v10.x;
            h->pos->y = s->v10.y + s->f34;
        }
    }
}

s32 fn_1_3704(M574_COL_WALL *wall, M574_COLLIDER *col) {
    M574_STAGE_SLOT *ent;
    M574_PLANE *v;
    OMOBJ *stage;
    s32 flag;
    M574_STAGE_TBL *tbl;

    if ((col->flag & 2) == 0) {
        stage = lbl_1_bss_48;
        tbl = stage->data;
        if (wall->id == 0) {
            flag = 0;
        } else {
            flag = 1;
        }
        ent = &tbl->ent2[flag];
        if (ent->v00 == 0) {
            ent->v00++;
            ent->v48.col = col;
            col->flag |= 2;
            col->vel->x = 0.0f;
            col->vel->y = 0.0f;
            v = wall->lastPlane;
            ent->v10.x = v->p0.x + 0.5 * (v->p1.x - v->p0.x);
            ent->v10.y = v->p0.y + 0.5 * (v->p1.y - v->p0.y);
            ent->f34 = 80.0f;
            return 1;
        }
    }
    return 0;
}

void fn_1_383C(OMOBJ *p) {
    /* Non-identique : MWCC 1.3.2 repartit val/tbl/obj en r26/r25/r24 la ou
       CodeWarrior met tbl/obj/val ; 7 lignes, rename de registres seule. */
    M574_STAGE_CELL *cell;
    s32 j;
    s32 i;
    HU3DMODELID model;
    M574_STAGE_TBL *tbl;
    HSFOBJECT *obj;
    s32 val;
    s16 modelId;

    tbl = p->data;
    modelId = p->mdlId[5];
    for (i = 0; i < 2; i++) {
        cell = &tbl->cell[i];
        cell->v0C = -1;
        for (j = 0; j < 3; j++) {
            obj = Hu3DModelObjPtrGet(modelId, lbl_1_data_2D8[j + i * 3]);
            cell->v50[j] = (HSFOBJECT *)obj;
            val = (s32)(20.0 * (1.52587890625e-05 * (f32)(s32)(u16)FRAND16()));
            cell->v20[j] = val;
            cell->f2C[j] = cell->f38[j] = 18.0f * (f32)val;
        }
        model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_314[i], 0x10000000, 2));
        p->mdlId[i + 0x1D] = model;
        Hu3DModelLayerSet(model, 4);
        Hu3DModelAttrSet(model, 0x40000002);
        Hu3DMotionStartEndSet(model, lbl_1_rodata_128, lbl_1_rodata_178);
        Hu3DMotionTimeSet(model, lbl_1_rodata_128);
    }
}

void fn_1_3A1C(OMOBJ *p) {
    M574_STAGE_CELL *cell;
    M574_STAGE_TBL *tbl;
    HU3DMODELID model;
    s16 modelId;
    s32 i;
    s32 j;
    s32 idx;
    s32 limit;
    s32 sym;
    s32 stop;
    s32 n;
    f32 ang;

    tbl = p->data;
    modelId = p->mdlId[5];
    for (i = 0; i < 2; i++) {
        cell = &tbl->cell[i];
        model = p->mdlId[i + 0x1D];
        switch (cell->v00) {
            case 0:
                break;
            case 1:
                cell->v00++;
                cell->v04 = 0;
                cell->v10 = 1;
                for (j = 0; j < 3; j++) {
                    cell->f44[j] = 4.0 + 1.52587890625e-05 * (f32)(s32)(u16)FRAND16();
                    cell->v14[j] = 0;
                }
                Hu3DModelAttrReset(model, 0x40000002);
                Hu3DModelAttrSet(model, 0x40000001);
                Hu3DMotionStartEndSet(model, 0.0f, 60.0f);
                Hu3DMotionTimeSet(model, 0.0f);
                if (i == 0) {
                    cell->v0C = HuAudFXPlay(0x91F);
                }
                else {
                    cell->v0C = HuAudFXPlay(0x920);
                }
                break;
            case 2:
                for (j = 0; j < 3; j++) {
                    if (cell->v14[j] != 0) {
                        continue;
                    }
                    if (MgSeqModeGet() == 5) {
                        limit = 60.0 * (3.0 + 0.4 * j);
                    }
                    else {
                        limit = 60.0 * (1.0 + 0.4 * j);
                    }
                    if (cell->v04 > limit) {
                        cell->v14[j] = 1;
                        idx = cell->f38[j] / 18.0f;
                        cell->f38[j] = 18.0f * (f32)idx;
                        switch (j) {
                            case 0:
                                stop = 0;
                                break;
                            case 1:
                                if (1.52587890625e-05 * (f32)(s32)(u16)FRAND16() < 0.5) {
                                    stop = 1;
                                }
                                else {
                                    stop = 0;
                                }
                                break;
                            case 2:
                                if (1.52587890625e-05 * (f32)(s32)(u16)FRAND16() < 0.5) {
                                    stop = 1;
                                }
                                else {
                                    stop = 0;
                                }
                                break;
                        }
                        if (j != 0 && stop != 0) {
                            sym = fn_1_4330(cell->f38[j]);
                            while (1) {
                                if (idx >= 0x14) {
                                    idx = 0;
                                }
                                if (lbl_1_data_2C4[idx] == sym) {
                                    break;
                                }
                                idx++;
                            }
                            cell->f38[j] = 18.0f * (f32)idx;
                        }
                        if (j == 2) {
                            n = cell->v00;
                            cell->v00 = n + 1;
                            cell->v04 = 0;
                            if (fn_1_918() <= 1) {
                                sym = fn_1_4330(cell->f38[j]);
                                while (1) {
                                    if (idx >= 0x14) {
                                        idx = 0;
                                    }
                                    if (lbl_1_data_2C4[idx] != sym) {
                                        break;
                                    }
                                    idx++;
                                }
                                cell->f38[j] = 18.0f * (f32)idx;
                            }
                        }
                    }
                    else if ((f32)cell->v04 > (f32)(60.0 * (0.2 * j))) {
                        cell->f38[j] += cell->f44[j];
                    }
                }
                cell->v04++;
                break;
            case 3:
                n = cell->v04 + 1;
                cell->v04 = n;
                if (!((f32)n < 54.0)) {
                    cell->v00++;
                    cell->v10 = 0;
                    if (i == 0) {
                        HuAudFXStop(cell->v0C);
                    }
                    else {
                        HuAudFXStop(cell->v0C);
                    }
                    sym = cell->v20[0];
                    if (sym == cell->v20[1] && sym == cell->v20[2]) {
                        fn_1_7874(i, lbl_1_data_2B8[sym]);
                        if (i == 0) {
                            HuAudFXPlay(0x921);
                        }
                        else {
                            HuAudFXPlay(0x922);
                        }
                        cell->v08 = 0x3C;
                        Hu3DModelAttrReset(model, 0x40000002);
                        Hu3DModelAttrSet(model, 0x40000001);
                        Hu3DMotionStartEndSet(model, 60.0f, Hu3DMotionMaxTimeGet(model));
                        Hu3DMotionTimeSet(model, 60.0f);
                    }
                    else {
                        cell->v08 = 0x1E;
                        Hu3DModelAttrSet(model, 0x40000002);
                        Hu3DModelAttrReset(model, 0x40000001);
                        Hu3DMotionStartEndSet(model, 0.0f, 60.0f);
                        Hu3DMotionTimeSet(model, 0.0f);
                    }
                }
                break;
            case 4:
                n = cell->v08 - 1;
                cell->v08 = n;
                if (n <= 0) {
                    cell->v00 = 0;
                    Hu3DModelAttrSet(model, 0x40000002);
                    Hu3DModelAttrReset(model, 0x40000001);
                    Hu3DMotionStartEndSet(model, 0.0f, 60.0f);
                    Hu3DMotionTimeSet(model, 0.0f);
                }
                break;
        }
        for (j = 0; j < 3; j++) {
            ang = fmod(cell->f38[j], 360.0);
            if (ang < 0.0f) {
                ang += 360.0f;
            }
            cell->f38[j] = ang;
            cell->f2C[j] = fn_1_CC8(cell->f2C[j], cell->f38[j], 0.1f);
            cell->v20[j] = fn_1_4330(cell->f2C[j]);
            if (cell->v50[j] != NULL) {
                cell->v50[j]->mesh.base.rot.x = cell->f2C[j];
            }
        }
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

void fn_1_43E8(OMOBJ *p, s32 idx) {
    M574_STAGE_CELL *c;
    M574_STAGE_TBL *tbl = p->data;

    c = &tbl->cell[idx];

    if (MgSeqModeGet() == 5 && c->v00 == 0) {
        c->v00++;
    }
}

s32 fn_1_445C(OMOBJ *p, s32 idx) {
    M574_STAGE_CELL *c;
    M574_STAGE_TBL *tbl = p->data;

    c = &tbl->cell[idx];
    return c->v10;
}

void fn_1_448C(OMOBJ *p) {
    M574_STAGE_SLOT *slot;
    M574_COL_WALL *ent;
    s32 i;
    HSFOBJECT *obj;
    HU3DMODELID id;
    s32 idx;
    s32 a;
    M574_STAGE_TBL *tbl;

    tbl = p->data;
    id = Hu3DModelCreate(HuDataSelHeapReadNum(0x00810017, 0x10000000, 2));
    p->mdlId[7] = id;
    Hu3DModelAttrSet(id, 1);
    for (i = 0; i < 4; i++) {
        obj = Hu3DModelObjPtrGet(id, lbl_1_data_3A8[i]);
        if (obj != NULL) {
            ent = &tbl->hookEnt[i];
            ent->flag = 1;
            ent->shape = 4;
            ent->id = i;
            ent->kind = 1;
            ent->user = p;
            ent->unk14 = 0;
            ent->hook = fn_1_47DC;
            fn_1_8FA8(ent, obj);
            fn_1_8FDC(ent);
            fn_1_8B48((M574_COLHEAD *)ent);
        }
    }
    id = p->mdlId[5];
    idx = 0;
    for (a = 0; a < 2; a++) {
        for (i = 0; i < 7; i++) {
            slot = &tbl->slotB[idx];
            obj = Hu3DModelObjPtrGet(id, lbl_1_data_31C[idx]);
            slot->v48.obj = obj;
            if (obj != NULL) {
                slot->v10.x = obj->mesh.base.pos.x;
                slot->v10.y = obj->mesh.base.pos.y;
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

void fn_1_4660(OMOBJ *p) {
    M574_STAGE_SLOT *slot;
    HSFOBJECT *ptr;
    s32 i;
    s32 j;
    s32 k;
    M574_STAGE_TBL *tbl;

    tbl = p->data;
    k = 0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 7; j++) {
            slot = &tbl->slotB[k];
            ptr = slot->v48.obj;
            if (ptr != NULL) {
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
                ptr->mesh.base.pos.x = slot->v10.x + slot->f3C;
                ptr->mesh.base.pos.y = slot->v10.y + slot->f40;
                slot->v04++;
                k++;
            }
        }
    }
}

s32 fn_1_47DC(M574_COL_WALL *wall, M574_COLLIDER *col) {
    M574_STAGE_TBL *tbl;
    M574_STAGE_SLOT *ent;
    M574_STAGE_SLOT *slot;
    M574_PLANE *plane;
    M574_PLANE *plane2;
    f32 dx;
    f32 dy;
    f32 dist;
    f32 minY;
    s32 id;
    s32 idx;
    s32 idx2;
    s32 side;
    s32 which;
    s32 ok;

    tbl = lbl_1_bss_48->data;
    id = wall->id;
    idx = -1;
    if (id < 2) {
        side = 0;
    }
    else {
        side = 1;
    }
    if (id >= 1 && id <= 2) {
        if (!(wall->lastPlane->n.y > -0.5)) {
            idx = (wall->lastPlane->p0.x - col->pos->x) / 100.0;
            if (idx < 0) {
                idx = 0;
            }
            if (idx > 4) {
                idx = 4;
            }
            fn_1_77D4(side, 0xA);
            if (col->pos->x < 0.0f) {
                fn_1_C10(0x916, col->pos);
            }
            else {
                fn_1_C10(0x917, col->pos);
            }
        }
    }
    else {
        if (wall->lastPlane->n.y > 0.5) {
            plane = wall->lastPlane;
            dx = (plane->p0.x + plane->p1.x) * 0.5f - col->pos->x;
            dy = (plane->p0.y + plane->p1.y) * 0.5f - col->pos->y;
            dist = sqrtf(dx * dx + dy * dy);
            if (dist < 300.0) {
                if ((col->flag & 2) == 0) {
                    if (wall->id == 0) {
                        which = 0;
                    }
                    else {
                        which = 1;
                    }
                    ent = &((M574_STAGE_TBL *)lbl_1_bss_48->data)->ent2[which];
                    if (ent->v00 == 0) {
                        ent->v00++;
                        ent->v48.col = col;
                        col->flag |= 2;
                        col->vel->x = 0.0f;
                        col->vel->y = 0.0f;
                        plane2 = wall->lastPlane;
                        ent->v10.x = plane2->p0.x + 0.5 * (plane2->p1.x - plane2->p0.x);
                        ent->v10.y = plane2->p0.y + 0.5 * (plane2->p1.y - plane2->p0.y);
                        ent->f34 = 80.0f;
                        ok = 1;
                    }
                    else {
                        ok = 0;
                    }
                }
                else {
                    ok = 0;
                }
                if (ok != 0) {
                    return 1;
                }
            }
        }
        if ((id != 0 || !(wall->lastPlane->n.x < 0.5)) && (id != 3 || !(wall->lastPlane->n.x > -0.5))) {
            minY = wall->lastPlane->p0.y;
            if (minY > wall->lastPlane->p1.y) {
                minY = wall->lastPlane->p1.y;
            }
            idx2 = (col->pos->y - minY) / 100.0;
            if (idx2 < 0) {
                idx2 = 0;
            }
            if (idx2 >= 2) {
                idx = -1;
            }
            else {
                idx = idx2 + 5;
                fn_1_77D4(side, 0xB);
                if (col->pos->x < 0.0f) {
                    fn_1_C10(0x916, col->pos);
                }
                else {
                    fn_1_C10(0x917, col->pos);
                }
            }
        }
    }
    if (idx >= 0) {
        slot = &tbl->slotB[idx + side * 7];
        if (slot->v00 == 0) {
            slot->v00++;
            if (idx < 5) {
                fn_1_5680(&slot->v10.x);
            }
        }
    }
    return 0;
}

void fn_1_4D28(OMOBJ *p) {
    M574_COL_CIRCLE *h;
    M574_STAGE_SLOT *slot;
    HU3DMODELID mot;
    s32 i;
    s32 j;
    HU3DMODELID model;
    M574_STAGE_TBL *tbl;
    HU3DMOTID joint;

    tbl = p->data;
    for (i = 0; i < 2; i++) {
        slot = &tbl->slot[i];
        if (i == 0) {
            model = Hu3DModelCreate(HuDataSelHeapReadNum(0x81 << 16 | 0x1A,
                                                       HU_MEMNUM_OVL, HEAP_MODEL));
            mot = model;
            for (j = 0; j < 3; j++) {
                joint = Hu3DJointMotion(mot, HuDataSelHeapReadNum(lbl_1_data_480[j],
                                            HU_MEMNUM_OVL, HEAP_MODEL));
                p->mtnId[j] = joint;
            }
        } else {
            mot = Hu3DModelLink(model);
        }
        p->mdlId[i + 0x1F] = mot;
        Hu3DMotionSet(mot, p->mtnId[0]);
        Hu3DModelAttrSet(mot, 0x40000001);
        Hu3DModelLayerSet(mot, 4);
        Hu3DModelShadowSet(mot);
        slot->v00 = 0;
        slot->v10 = lbl_1_data_48C[i];
        slot->f3C = slot->f40 = slot->f44 = lbl_1_rodata_160;
        h = &slot->hook;
        h->flag = 1;
        h->shape = 2;
        h->id = i;
        h->kind = 0;
        h->user = p;
        h->unk14 = 0;
        h->hook = fn_1_5628;
        h->pos = &slot->v10;
        h->radius = lbl_1_rodata_218;
        fn_1_8B48((M574_COLHEAD *)h);
        (void)tbl;
        (void)tbl;
    }
}

void fn_1_4EE8(OMOBJ *p) {
    M574_STAGE_TBL *tbl;
    M574_STAGE_SLOT *slot;
    HU3DMODELID model;
    Vec *target;
    f32 rotTarget;
    s32 i;
    s32 n;
    f64 dir;

    tbl = p->data;
    for (i = 0; i < 2; i++) {
        slot = &tbl->slot[i];
        model = p->mdlId[i + 0x1F];
        target = &lbl_1_data_48C[i];
        rotTarget = 0.0f;
        switch (slot->v00) {
            case 0:
                slot->v00++;
                slot->v10.x = slot->v10.x + 150.0 * (3.0517578125e-05 * (f32)((s32)FRAND16() - 0x8000));
                slot->f34 = 0.0f;
                Hu3DMotionSet(model, p->mtnId[0]);
                // fallthrough
            case 1:
                if (MgSeqModeGet() != 5) {
                    break;
                }
                slot->v00++;
                if ((s32)(frand() & 1) != 0) {
                    dir = -1.0;
                }
                else {
                    dir = 1.0;
                }
                slot->f34 = dir;
                Hu3DMotionShiftSet(model, p->mtnId[1], 0.0f, 12.0f, 0x40000001);
                // fallthrough
            case 2:
                if (MgSeqModeGet() != 5) {
                    slot->v00 = 8;
                    slot->f34 = 0.0f;
                    Hu3DMotionShiftSet(model, p->mtnId[0], 0.0f, 12.0f, 0x40000001);
                }
                else if (slot->f34 < 0.0f) {
                    if (slot->v10.x > target->x - 180.0) {
                        rotTarget = 270.0f;
                    }
                    else {
                        slot->f34 = 1.0f;
                    }
                }
                else if (slot->v10.x < 180.0 + target->x) {
                    rotTarget = 90.0f;
                }
                else {
                    slot->f34 = -1.0f;
                }
                break;
            case 3:
                slot->v1C.y = slot->v1C.y - 1.666666753590107;
                if (!(slot->v10.y + slot->v1C.y > target->y)) {
                    slot->v10.y = target->y;
                    slot->v1C.y = 0.0f;
                    if (((M574_STAGE_TBL *)p->data)->cell[i].v10 == 0) {
                        slot->v00++;
                        slot->v04 = 0;
                        Hu3DMotionShiftSet(model, p->mtnId[0], 0.0f, 30.0f, 0x40000001);
                    }
                }
                break;
            case 4:
                n = slot->v04 + 1;
                slot->v04 = n;
                if (!((f32)n < 30.0)) {
                    slot->v00 = 2;
                    if ((s32)(frand() & 1) != 0) {
                        dir = -1.0;
                    }
                    else {
                        dir = 1.0;
                    }
                    slot->f34 = dir;
                    Hu3DMotionShiftSet(model, p->mtnId[1], 0.0f, 12.0f, 0x40000001);
                }
                break;
            case 5:
                slot->f40 = slot->f40 - 0.2;
                if (slot->f40 > 0.4) {
                    break;
                }
                slot->f40 = 0.4f;
                slot->v00++;
                // fallthrough
            case 6:
                if (((M574_STAGE_TBL *)p->data)->cell[i].v10 != 0) {
                    break;
                }
                slot->v00++;
                // fallthrough
            case 7:
                slot->f40 = slot->f40 + 0.1;
                if (!(slot->f40 < 1.0)) {
                    slot->f40 = 1.0f;
                    slot->v00 = 4;
                    slot->v04 = 0;
                    Hu3DMotionShiftSet(model, p->mtnId[0], 0.0f, 30.0f, 0x40000001);
                }
                break;
            case 8:
                break;
        }
        if ((slot->v00 == 2 || slot->v00 == 4) && slot->v38 != 0) {
            slot->v04 = 0;
            slot->f34 = 0.0f;
            if (MgSeqModeGet() == 5 && ((M574_STAGE_TBL *)p->data)->cell[i].v00 == 0) {
                ((M574_STAGE_TBL *)p->data)->cell[i].v00++;
            }
            Hu3DMotionShiftSet(model, p->mtnId[2], 0.0f, 12.0f, 0x40000001);
            if (slot->v38 == 1) {
                slot->v00 = 5;
                fn_1_77D4(i, 7);
                if (slot->v10.x < 0.0f) {
                    fn_1_C10(0x92B, &slot->v10);
                    return;
                }
                fn_1_C10(0x92C, &slot->v10);
                return;
            }
            slot->v00 = 3;
            slot->v1C.y = 13.333334f;
            fn_1_77D4(i, 8);
            if (slot->v10.x < 0.0f) {
                fn_1_C10(0x91A, &slot->v10);
                return;
            }
            fn_1_C10(0x91B, &slot->v10);
            return;
        }
        slot->v38 = 0;
        slot->v28.y = fn_1_CC8(slot->v28.y, rotTarget, 0.05f);
        slot->v1C.x = slot->v1C.x + 0.05 * (0.01666666753590107 * (150.0 * slot->f34) - slot->v1C.x);
        PSVECAdd(&slot->v10, &slot->v1C, &slot->v10);
        Hu3DModelPosSet(model, slot->v10.x, slot->v10.y - 20.0, slot->v10.z);
        Hu3DModelRotSetV(model, &slot->v28);
        Hu3DModelScaleSetV(model, (Vec *)&slot->f3C);
    }
}

s32 fn_1_5628(M574_COL_CIRCLE *c, M574_COLLIDER *col) {
    OMOBJ *stage;
    M574_STAGE_SLOT *s;
    s32 idx;
    M574_STAGE_TBL *tbl;

    idx = c->id;
    stage = lbl_1_bss_48;
    tbl = stage->data;
    s = &tbl->slot[idx];
    s->v38 = 1;
    return 0;
}

void fn_1_5680(f32 *x) {
    M574_STAGE_SLOT *slot;
    s32 i;
    OMOBJ *stage;
    M574_STAGE_TBL *tbl;
    f32 v1;
    f32 v2;
    f64 v3;

    stage = lbl_1_bss_48;
    tbl = stage->data;
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

void fn_1_5750(OMOBJ *p) {
    M574_STAGE_TBL *tbl;
    M574_STAGE_MOT *mot;
    HU3DMODELID model;
    s16 id1;
    HSFOBJECT *obj1;
    s16 id2;
    HSFOBJECT *obj2;
    M574_COL_CIRCLE *hook;
    HSFOBJECT *obj3;
    M574_STAGE_TBL *unused1;
    M574_STAGE_TBL *unused2;

    tbl = p->data;
    mot = &tbl->mot;
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x00810005, 0x10000000, 2));
    p->mdlId[8] = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DMotionStartEndSet(model, lbl_1_rodata_128, lbl_1_rodata_178);
    Hu3DModelLayerSet(model, 4);
    unused1 = p->data;
    id1 = p->mdlId[4];
    obj1 = Hu3DModelObjPtrGet(id1, lbl_1_data_3D0[0]);
    if (obj1 != NULL) {
        Hu3DModelPosSet(model, obj1->mesh.base.pos.x, obj1->mesh.base.pos.y, obj1->mesh.base.pos.z);
    }
    unused2 = p->data;
    id2 = p->mdlId[4];
    obj2 = Hu3DModelObjPtrGet(id2, lbl_1_data_3D0[0]);
    if (obj2 != NULL) {
        mot->v10.x = obj2->mesh.base.pos.x;
        mot->v10.y = obj2->mesh.base.pos.y;
        mot->v10.z = obj2->mesh.base.pos.z;
    }
    hook = &mot->hook;
    hook->flag = 1;
    hook->shape = 1;
    hook->id = 100;
    hook->kind = 0;
    hook->user = p;
    hook->unk14 = 0;
    hook->hook = fn_1_5D64;
    hook->pos = &mot->v10;
    hook->radius = 200.0f;
    fn_1_8B48((M574_COLHEAD *)hook);
    mot->v00 = 0;
    mot->v38 = 0;
    mot->v1C.x = mot->v1C.y = mot->v1C.z = lbl_1_rodata_128;
    obj3 = Hu3DModelObjPtrGet(model, lbl_1_data_4A4);
    mot->v48 = (HSFOBJECT *)obj3;
    if (obj3 != NULL) {
        mot->v3C.x = obj3->mesh.base.pos.x;
        mot->v3C.y = obj3->mesh.base.pos.y;
        mot->v3C.z = obj3->mesh.base.pos.z;
    }
}

void fn_1_5934(OMOBJ *p) {
    M574_STAGE_TBL *tbl;
    M574_STAGE_MOT *mot;
    HSFOBJECT *obj;
    HU3DMODELID model;
    Vec vel;
    Vec pos;
    f32 ang;
    f32 base;
    s32 side;
    s32 n;
    s32 k;

    tbl = p->data;
    mot = &tbl->mot;
    model = p->mdlId[8];
    switch (mot->v00) {
        case 0:
            if (mot->v38 == 0 || MgSeqModeGet() != 5) {
                break;
            }
            mot->v00++;
            mot->v04 = 0;
            fn_1_77D4(mot->v0C, 6);
            HuAudFXPlay(0x91C);
            HuAudFXPlay(0x91E);
            tbl->v0C--;
            fn_1_63B0(&lbl_1_data_4B0[0], &lbl_1_data_4C8[0], tbl->v0C);
            fn_1_63B0(&lbl_1_data_4B0[1], &lbl_1_data_4C8[1], tbl->v0C);
            // fallthrough
        case 1:
            if (tbl->v0C > 0) {
                n = mot->v04 + 1;
                mot->v04 = n;
                if (n >= 4) {
                    mot->v00--;
                    mot->v04 = 0;
                }
                break;
            }
            mot->v00++;
            mot->v04 = 0;
            // fallthrough
        case 2:
            n = mot->v04 + 1;
            mot->v04 = n;
            if ((f32)n < 60.0) {
                break;
            }
            side = mot->v0C;
            mot = &((M574_STAGE_TBL *)p->data)->mot;
            pos.x = mot->v10.x;
            pos.y = mot->v10.y - 150.0;
            pos.z = 300.0f;
            vel.z = 80.0f;
            for (k = 0; k < 5; k++) {
                if (side == 0) {
                    base = 240.0f;
                }
                else {
                    base = 120.0f;
                }
                ang = base + 10.0 * (3.0517578125e-05 * (f32)((s32)FRAND16() - 0x8000));
                vel.x = 100.0 * (0.2 * sin(3.141592653589793 * ang / 180.0));
                vel.y = 100.0 * (0.2 * cos(3.141592653589793 * ang / 180.0));
                fn_1_6C2C(&pos, &vel);
            }
            mot->v00++;
            mot->v04 = 0;
            // fallthrough
        case 3:
            n = mot->v04 + 1;
            mot->v04 = n;
            if (!((f32)n < 60.0)) {
                mot->v00 = 0;
                mot->v04 = 0;
                tbl->v0C = 5;
            }
            break;
    }
    if (Hu3DMotionEndCheck(model) != 0) {
        Hu3DModelAttrSet(model, 0x40000001);
        Hu3DMotionStartEndSet(model, 0.0f, 60.0f);
        Hu3DMotionTimeSet(model, 0.0f);
    }
    mot = &tbl->mot;
    mot->v38 = 0;
    PSVECScale(&mot->v1C, &mot->v1C, 0.2f);
    obj = mot->v48;
    if (obj != NULL) {
        obj->mesh.base.pos.x = mot->v3C.x + mot->v1C.x;
        obj->mesh.base.pos.y = mot->v3C.y + mot->v1C.y;
    }
}

s32 fn_1_5D64(M574_COL_CIRCLE *c, M574_COLLIDER *col) {
    M574_STAGE_MOT *mot;
    OMOBJ *stage;
    s16 modelId;
    s32 flag;
    M574_STAGE_TBL *tbl;

    stage = lbl_1_bss_48;
    tbl = stage->data;
    mot = &tbl->mot;
    modelId = stage->mdlId[8];
    if (mot->v00 == 0) {
        mot->v38 = 1;
        if (col->pos->x < 0.0f) {
            flag = 0;
        } else {
            flag = 1;
        }
        mot->v0C = flag;
        Hu3DModelAttrReset(modelId, 0x40000001);
        Hu3DMotionStartEndSet(modelId, 60.0f, Hu3DMotionMaxTimeGet(modelId));
        Hu3DMotionTimeSet(modelId, 60.0f);
        PSVECNormalize(col->vel, &mot->v1C);
        PSVECScale(&mot->v1C, &mot->v1C, 50.0f);
    }
    return 0;
}

void fn_1_5E5C(OMOBJ *p) {
    M574_STAGE_TBL *tbl = p->data;
    s16 modelId = p->mdlId[8];

    tbl->texId = Hu3DTexScrollCreate(modelId, lbl_1_data_504.name);
    tbl->v0C = 5;
    tbl->f10 = lbl_1_rodata_254;
}

void fn_1_5ECC(OMOBJ *p) {
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

    tbl = p->data;
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

void fn_1_6168(OMOBJ *p) {
    HU3DPARTICLEDATA *nd;
    HU3DMODELID particle;
    s32 i;
    HU3DPARTICLE *grp;
    s32 j;
    M574_STAGE_TBL *tbl;
    ANIMDATA *anim;

    tbl = p->data;
    anim = HuSprAnimDataRead(0x81 << 16 | 0x20);
    for (i = 0; i < 5; i++) {
        particle = Hu3DParticleCreate(anim, 8);
        tbl->ids[i] = particle;
        grp = Hu3DData[particle].hookData;
        Hu3DParticleBlendModeSet(particle, HU3D_PARTICLE_BLEND_NORMAL);
        Hu3DParticleHookSet(particle, fn_1_62A0);
        Hu3DModelLayerSet(particle, 7);
        Hu3DModelPosSet(particle, lbl_1_rodata_128, lbl_1_rodata_128,
                        lbl_1_rodata_128);
        Hu3DParticleAnimModeSet(particle, 4 - i);
        for (nd = grp->data, j = 0; j < grp->maxCnt; j++, nd++) {
            nd->time = -1;
            nd->scale = lbl_1_rodata_128;
        }
        (void)nd;
        (void)nd;
        (void)nd;
        (void)nd;
    }
}

void fn_1_629C(OMOBJ *p) {
}

void fn_1_62A0(HU3DMODEL *model, HU3DPARTICLE *p, Mtx mtx) {
    HU3DPARTICLEDATA *e;
    s32 i;

    e = p->data;
    i = 0;
    while (i < p->maxCnt) {
        switch (e->time) {
        case 0:
            e->time++;
            e->scale = 200.0f;
            e->color.a = 0xB4;
        case 1:
            e->scale -= 10.0f;
            e->color.a -= 4;
            if (e->scale <= 0.0f || e->color.a > 0xF0) {
                e->time = -1;
                e->scale = 0.0f;
            } else {
                PSVECAdd(&e->pos, &e->accel, &e->pos);
            }
            break;
        }
        i++;
        e++;
    }
}

void fn_1_63B0(Vec *a, Vec *b, s32 idx) {
    HU3DPARTICLEDATA *nd;
    HU3DPARTICLE *grp;
    s32 i;
    M574_STAGE_TBL *tbl;

    tbl = lbl_1_bss_48->data;
    grp = Hu3DData[tbl->ids[4 - idx]].hookData;
    nd = grp->data;
    i = 0;
    while (i < grp->maxCnt) {
        if (nd->time < 0) {
            nd->time = 0;
            nd->pos = *a;
            nd->accel = *b;
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

    tbl = lbl_1_bss_48->data;
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

    tbl = lbl_1_bss_48->data;
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
