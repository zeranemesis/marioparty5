#include "types.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/hsfformat.h"
#include "game/data.h"
#include "math.h"
#include "REL/m516Dll/m516types.h"

extern void *memset(void *dst, int c, u32 n);
extern u16 frand(void);
extern f32 Hu3DMotionMaxTimeGet(s16 modelId);
extern void PSVECAdd(const Vec *a, const Vec *b, Vec *out);
extern void HuAudFXFadeOut(int seNo, s32 speed);
extern s32 HuAudFXPitchSet(int seNo, s16 pitch);
extern int HuAudFXPlay(int seId);
extern u32 fn_1_1838(void);
extern u32 fn_1_1850(void);
extern s32 fn_1_1B8C(s32 soundId, Vec *pos);
extern f32 fn_1_1D34(f32 start, f32 end, f32 t);
extern void fn_1_78B8(Vec *pos, f32 size);
extern void fn_1_800C(void *obj, s32 flag);

typedef struct {
    s32 state;
    s32 f04;
    s32 f08;
    s32 kind;
    s16 mdl;
    s16 mdl2;
    Vec pos;
    Vec vel;
    Vec rot;
    f32 f38;
    s32 row;
    u8 pad40[0x0c];
    ACTHITEX hit;
} MGSLOT;

typedef struct {
    Vec v0;
    ACTHITEX hit;
    HSFOBJECT *objs[6];
    f32 scroll[3];
    s32 sfx;
    s32 timer;
    f32 timerMax;
    f32 dir;
    f32 baseY[3];
    f32 baseZ[3];
    f32 speed[3];
    f32 yOff;
    MGSLOT slotsA[66];
    s32 stateB[12];
    MGSLOT slotsB[12];
    MGSLOT slotsC[4];
    MGSLOT slotsD[3];
    MGSLOT slotsE[3];
    f32 waterX;
    f32 waterY;
} MGSTAGEWORK;

typedef struct {
    u32 id;
    s32 layer;
    f32 rotSpeed;
    Vec from;
    Vec to;
} MGSTAGEDEF;

OMOBJ *lbl_1_bss_38;

char lbl_1_data_138[6][10] = { "block_LA", "block_LB", "block_LC", "block_RA", "block_RB", "block_RC" };
MGSTAGEDEF lbl_1_data_174[4] = {
    { 0x4A0006, 4, 0.2f, { -2500.0f, 1000.0f, -3000.0f }, { 2500.0f, 1800.0f, -3000.0f } },
    { 0x4A0007, 4, 0.0f, { -1600.0f, 0.0f, 0.0f }, { 350.0f, 0.0f, 0.0f } },
    { 0x4A0008, 4, 0.0f, { -2400.0f, 0.0f, 0.0f }, { -950.0f, 0.0f, 0.0f } },
    { 0x4A0009, 4, 0.0f, { -2700.0f, 0.0f, 0.0f }, { -2100.0f, 0.0f, 0.0f } },
};
Vec lbl_1_data_204[3] = {
    { -1000.0f, -150.0f, -650.0f },
    { -650.0f, -150.0f, -650.0f },
    { 800.0f, -150.0f, -650.0f },
};
Vec lbl_1_data_228[3] = {
    { -600.0f, 150.0f, 150.0f },
    { 300.0f, 150.0f, 150.0f },
    { 600.0f, 150.0f, 150.0f },
};
s32 lbl_1_data_24C[4] = { 0x6E9, 0x6EA, 0x6EB, 0x6EC };
s32 lbl_1_data_25C[4] = { 0x6ED, 0x6EE, 0x6EF, 0x6F0 };
Vec lbl_1_data_26C[2] = { { -4000.0f, -100.0f, -5000.0f }, { 4000.0f, -100.0f, -100.0f } };
GXColor lbl_1_data_284 = { 0xFF, 0xFF, 0xFF, 0xFF };
f32 lbl_1_data_288[6] = { 0.04f, 0.0f, 0.0f, 0.0f, 0.04f, 0.0f };
char lbl_1_data_2A0[4][8] = { "Chara1", "Chara2", "Chara3", "Chara4" };


void fn_1_4960(OMOBJ *obj);
void fn_1_4CF8(OMOBJ *obj);
void fn_1_4DB4(OMOBJ *obj);
void fn_1_51C4(OMOBJ *obj);
void fn_1_59EC(OMOBJ *obj);
void fn_1_5AC4(OMOBJ *obj);
void fn_1_5B6C(Vec *pos);
void fn_1_5C70(OMOBJ *obj);
void fn_1_5D2C(OMOBJ *obj);
void fn_1_5EDC(Vec *pos);
void fn_1_5F6C(OMOBJ *obj);
void fn_1_601C(OMOBJ *obj);
void fn_1_6154(OMOBJ *obj);
void fn_1_6234(OMOBJ *obj);
void fn_1_6814(Vec *pos);
void fn_1_6870(OMOBJ *obj);
void fn_1_69A4(OMOBJ *obj);
void fn_1_6C54(OMOBJ *obj);
void fn_1_6CF4(OMOBJ *obj);

void fn_1_48CC(OMOBJMAN *objman)
{
    OMOBJ *stage;
    MGSTAGEWORK *work;

    stage = omAddObjEx(objman, 0x14, 0x86, 0, -1, fn_1_4960);
    lbl_1_bss_38 = stage;
    work = HuMemDirectMallocNum(0, sizeof(MGSTAGEWORK), 0x10000000);
    stage->data = work;
    memset(work, 0, sizeof(MGSTAGEWORK));
    stage->work[0] = 0;
}

void fn_1_495C(void)
{
}

// Remaining difference: local register allocation (extra GPR in original frame).
void fn_1_4960(OMOBJ *obj)
{
    MGSTAGEWORK *work = (MGSTAGEWORK *)obj->data;
    HSFOBJECT *hobj;
    s16 mdl;
    s32 i;

    omSetTra(obj, 0.0f, 0.0f, 0.0f);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x4A0000, 0x10000000, HEAP_MODEL));
    obj->mdlId[0] = mdl;
    Hu3DModelAttrSet(mdl, 0x40000001);
    Hu3DModelLayerSet(mdl, 4);
    for (i = 0; i < 3; i++) {
        hobj = Hu3DModelObjPtrGet(mdl, lbl_1_data_138[i]);
        work->objs[i] = hobj;
        work->scroll[i] = 0.0f;
        if (hobj != NULL) {
            work->baseY[i] = hobj->mesh.base.pos.y;
            work->baseZ[i] = hobj->mesh.base.pos.z;
            work->speed[i] = 0.0f;
        }
        work->objs[i + 3] = Hu3DModelObjPtrGet(mdl, lbl_1_data_138[i + 3]);
    }
    work->v0.x = 0.0f;
    work->v0.y = 0.0f;
    work->v0.z = 0.0f;
    work->hit.base.flags = 1;
    work->hit.base.kind = 1;
    work->hit.base.id = 0;
    work->hit.base.disabled = 0;
    work->hit.base.arg = obj;
    work->hit.base.callback = NULL;
    work->hit.base.pos = &work->v0;
    work->hit.base.vel = NULL;
    work->hit.base.f20 = -3000.0f;
    work->hit.base.f24 = 0.0f;
    work->hit.base.f28.f = 3000.0f;
    work->hit.base.f2C.f = -300.0f;
    work->hit.base.f30.id = 0;
    work->hit.f38 = 0;
    work->hit.hitObj = NULL;
    fn_1_8E00(&work->hit.base);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x4A0001, 0x10000000, HEAP_MODEL));
    obj->mdlId[1] = mdl;
    Hu3DModelLayerSet(mdl, 4);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x4A0002, 0x10000000, HEAP_MODEL));
    obj->mdlId[2] = mdl;
    Hu3DModelAttrSet(mdl, 0x40000001);
    Hu3DModelLayerSet(mdl, 0);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x4A0003, 0x10000000, HEAP_MODEL));
    obj->mdlId[3] = mdl;
    Hu3DModelLayerSet(mdl, 4);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x4A0005, 0x10000000, HEAP_MODEL));
    obj->mdlId[0x7A] = mdl;
    Hu3DModelAttrSet(mdl, 0x40000040);
    Hu3DModelLayerSet(mdl, 0);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x4A0004, 0x10000000, HEAP_MODEL));
    obj->mdlId[0x7B] = mdl;
    Hu3DModelLayerSet(mdl, 4);
    fn_1_4DB4(obj);
    fn_1_59EC(obj);
    fn_1_5C70(obj);
    fn_1_5F6C(obj);
    fn_1_6154(obj);
    fn_1_6870(obj);
    fn_1_6C54(obj);
    obj->objFunc = fn_1_4CF8;
}

void fn_1_4CF8(OMOBJ *obj)
{
    s16 mdl = obj->mdlId[1];

    if (Hu3DMotionEndCheck(mdl) != 0) {
        Hu3DModelAttrSet(mdl, 0x40000001);
        Hu3DMotionStartEndSet(mdl, 250.0f, Hu3DMotionMaxTimeGet(mdl));
    }
    fn_1_51C4(obj);
    fn_1_5AC4(obj);
    fn_1_5D2C(obj);
    fn_1_601C(obj);
    fn_1_6234(obj);
    fn_1_69A4(obj);
    fn_1_6CF4(obj);
    obj->work[0]++;
}

// Remaining difference: local table/loop register allocation.
void fn_1_4DB4(OMOBJ *obj)
{
    MGSTAGEWORK *work = (MGSTAGEWORK *)obj->data;
    s32 idx = 0;
    s16 mdlA = -1;
    s16 mdlB = -1;
    s32 tbl[3][22] = {
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 1, 1, 1, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    };
    Vec pos;
    MGSLOT *slot;
    s32 row;
    s32 j;
    s32 k;
    s32 r;
    s32 t;
    s16 m;

    work->sfx = -1;
    work->timer = 0;
    work->timerMax = 1.0f;
    work->dir = 3.3333335f;
    work->yOff = 200.0f;
    for (row = 0; row < 3; row++) {
        for (j = 10; j < 22; j++) {
            tbl[row][j] = 0;
        }
        for (j = 0; j < 22; j++) {
            r = (s32)(22.0 * (1.52587890625e-05 * (f32)(s32)frand()));
            t = tbl[row][r];
            tbl[row][r] = tbl[row][j];
            tbl[row][j] = t;
        }
    }
    for (row = 0; row < 3; row++) {
        pos.x = -1050.0f;
        pos.y = work->baseY[row];
        pos.z = work->baseZ[row];
        for (k = 0; k < 22; k++) {
            slot = &work->slotsA[idx];
            slot->kind = tbl[row][k];
            slot->pos.x = pos.x;
            slot->pos.y = pos.y;
            slot->pos.z = pos.z;
            slot->f38 = pos.y;
            slot->row = row;
            switch (slot->kind) {
            case 1:
                slot->f08 = 1;
                break;
            case 2:
                slot->f08 = 10;
                break;
            default:
                slot->f08 = 0;
                break;
            }
            if (mdlA == -1) {
                mdlA = Hu3DModelCreate(HuDataSelHeapReadNum(0x4A000D, 0x10000000, HEAP_MODEL));
                m = mdlA;
            } else {
                m = Hu3DModelLink(mdlA);
            }
            obj->mdlId[idx + 6] = m;
            slot->mdl = m;
            Hu3DModelAttrSet(m, 0x40000002);
            Hu3DModelLayerSet(m, 4);
            Hu3DModelPosSetV(m, &pos);
            pos.x = pos.x + 100.0;
            if (slot->kind != 0) {
                if (mdlB == -1) {
                    mdlB = Hu3DModelCreate(HuDataSelHeapReadNum(0x4A000E, 0x10000000, HEAP_MODEL));
                    m = mdlB;
                } else {
                    m = Hu3DModelLink(mdlB);
                }
                slot->mdl2 = m;
                Hu3DModelAttrSet(m, 1);
                Hu3DModelAttrSet(m, 0x40000002);
                Hu3DModelLayerSet(m, 4);
            } else {
                slot->mdl2 = -1;
            }
            slot->hit.base.flags = 1;
            slot->hit.base.kind = 2;
            slot->hit.base.id = idx;
            slot->hit.base.disabled = 0;
            slot->hit.base.arg = obj;
            slot->hit.base.callback = NULL;
            slot->hit.base.pos = &slot->pos;
            slot->hit.base.vel = &slot->vel;
            slot->hit.base.f20 = -50.0f;
            slot->hit.base.f24 = 0.0f;
            slot->hit.base.f28.f = 50.0f;
            slot->hit.base.f2C.f = -100.0f;
            slot->hit.base.f30.id = 1;
            slot->hit.f34 = 0;
            slot->hit.f38 = 0;
            slot->hit.hitObj = NULL;
            fn_1_8E00(&slot->hit.base);
            idx++;
        }
    }
}

// Remaining difference: fabs/fmod temporaries use extra FPRs in original.
void fn_1_51C4(OMOBJ *obj)
{
    MGSTAGEWORK *work = (MGSTAGEWORK *)obj->data;
    MGSLOT *slot;
    f32 f;
    f32 sp;
    f32 mod;
    s32 pitch;
    s32 i;
    HSFOBJECT *hobj;

    if (fn_1_1838() != 1) {
        work->yOff = work->yOff - 2.0;
        if (work->yOff < 0.0f) {
            work->yOff = 0.0f;
        }
    }
    if ((f32)(++work->timer) > work->timerMax) {
        work->timer = 0;
        work->timerMax = (f32)(420.0 + (s32)(180.0 * (1.52587890625e-05 * (f32)(s32)frand())));
        f = (f32)(0.01666666753590107 * (150.0 + (180.0 * (1.52587890625e-05 * (f32)(s32)frand()))));
        if (work->dir > 0.0f) {
            f = -f;
        }
        work->dir = f;
    }
    for (i = 0; i < 3; i++) {
        if (fn_1_1850() == 2) {
            if ((f32)work->timer < (f32)(work->timerMax - 240.0)) {
                if ((i & 1) != 0) {
                    f = work->dir;
                } else {
                    f = -work->dir;
                }
            } else {
                f = 0.0f;
            }
        } else {
            work->timer = 0;
            f = 0.0f;
        }
        work->speed[i] += 0.04 * (f - work->speed[i]);
    }
    sp = (f32)fabs(work->speed[0]);
    if (sp < 0.1) {
        sp = 0.0f;
    }
    if (0.0 != sp) {
        if (work->sfx == -1) {
            work->sfx = HuAudFXPlay(0x6F1);
        } else {
            pitch = (s32)(-8191.0f + (8191.0f * (sp / 5.5000005f)));
            if (pitch < -0x1FFF) {
                pitch = -0x1FFF;
            }
            if (pitch > 0) {
                pitch = 0;
            }
            HuAudFXPitchSet(work->sfx, (s16)pitch);
        }
    } else if (work->sfx != -1) {
        HuAudFXFadeOut(work->sfx, 0x3E8);
        work->sfx = -1;
    }
    for (i = 0; i < 3; i++) {
        work->scroll[i] += work->speed[i];
        mod = (f32)fmod(work->scroll[i], 100.0);
        if (mod < 0.0f) {
            mod = (f32)(mod + 100.0);
        }
        hobj = work->objs[i];
        if (hobj != NULL) {
            hobj->mesh.base.pos.x = (f32)(-1100.0 + mod);
        }
        hobj = work->objs[i + 3];
        if (hobj != NULL) {
            hobj->mesh.base.pos.x = (f32)(1000.0 + mod);
        }
    }
    for (i = 0; i < 0x42; i++) {
        slot = &work->slotsA[i];
        f = work->speed[slot->row];
        slot->vel.x = f;
        slot->pos.x = slot->pos.x + f;
        if (f < 0.0f && slot->pos.x < -1100.0) {
            slot->pos.x += 2200.0;
        }
        if (f > 0.0f && slot->pos.x > 1100.0) {
            slot->pos.x -= 2200.0;
        }
        switch (slot->state) {
        case 0:
            if (slot->hit.f38 != 0) {
                if (slot->kind < 3) {
                    if (slot->kind < 1) {
                        goto small;
                    }
                    fn_1_800C(slot->hit.hitObj, 1);
                    fn_1_5EDC(&slot->pos);
                    slot->vel.y = 8.333334f;
                    slot->state = 1;
                    fn_1_1B8C(7, &slot->pos);
                } else {
                small:
                    slot->vel.y = 8.333334f;
                    Hu3DModelAttrSet(slot->mdl, 1);
                    fn_1_5B6C(&slot->pos);
                    slot->state = 3;
                    fn_1_1B8C(lbl_1_data_24C[slot->hit.f34], &slot->pos);
                }
            }
            break;
        case 1:
            slot->pos.y = slot->pos.y + slot->vel.y;
            slot->vel.y = slot->vel.y - 1.666666753590107;
            if (slot->vel.y < 0.0f && slot->f08 <= 1) {
                Hu3DModelAttrSet(slot->mdl, 1);
                Hu3DModelAttrReset(slot->mdl2, 1);
            }
            if (!(slot->pos.y > slot->f38)) {
                slot->pos.y = slot->f38;
                slot->f08--;
                if (slot->f08 > 0) {
                    slot->state = 0;
                } else {
                    slot->hit.base.f30.id = 0;
                    slot->state = 2;
                }
            }
            break;
        case 2:
            if (slot->hit.f38 != 0) {
                fn_1_1B8C(lbl_1_data_25C[slot->hit.f34], &slot->pos);
            }
            break;
        case 3:
            slot->hit.base.flags &= ~1;
            slot->state--;
            break;
        }
        slot->hit.f38 = 0;
        slot->hit.hitObj = NULL;
        Hu3DModelPosSet(slot->mdl, slot->pos.x, slot->pos.y + work->yOff, slot->pos.z);
        if (slot->mdl2 != -1) {
            Hu3DModelPosSet(slot->mdl2, slot->pos.x, slot->pos.y + work->yOff, slot->pos.z);
        }
    }
}

void fn_1_59EC(OMOBJ *obj)
{
    s32 i;
    s16 m;
    MGSTAGEWORK *work = (MGSTAGEWORK *)obj->data;
    s16 first;

    for (i = 0; i < 0xC; i++) {
        if (i == 0) {
            first = Hu3DModelCreate(HuDataSelHeapReadNum(0x4A000F, 0x10000000, HEAP_MODEL));
            m = first;
        } else {
            m = Hu3DModelLink(first);
        }
        obj->mdlId[i + 0x62] = m;
        Hu3DModelAttrSet(m, 1);
        Hu3DModelAttrSet(m, 0x40000002);
        Hu3DModelAttrReset(m, 0x40000001);
        Hu3DModelLayerSet(m, 4);
        work->stateB[i] = 0;
    }
}

void fn_1_5AC4(OMOBJ *obj)
{
    MGSTAGEWORK *work = (MGSTAGEWORK *)obj->data;
    s16 m;
    s32 i;

    for (i = 0; i < 0xC; i++) {
        if (work->stateB[i] != 0) {
            m = obj->mdlId[i + 0x62];
            if (Hu3DMotionEndCheck(m) != 0) {
                Hu3DModelAttrSet(m, 1);
                work->stateB[i] = 0;
            }
        }
    }
}

void fn_1_5B6C(Vec *pos)
{
    OMOBJ *stage = lbl_1_bss_38;
    MGSTAGEWORK *work = (MGSTAGEWORK *)stage->data;
    s16 m;
    s32 i;

    for (i = 0; i < 0xC; i++) {
        if (work->stateB[i] == 0) {
            work->stateB[i] = 1;
            m = stage->mdlId[i + 0x62];
            Hu3DModelPosSetV(m, pos);
            Hu3DModelRotSet(m, 0.0f, 0.0f, 0.0f);
            Hu3DModelAttrReset(m, 1);
            Hu3DModelAttrReset(m, 0x40000002);
            Hu3DMotionTimeSet(m, 0.0f);
            return;
        }
    }
}

void fn_1_5C70(OMOBJ *obj)
{
    MGSTAGEWORK *work = (MGSTAGEWORK *)obj->data;
    s16 first;
    s16 m;
    s32 i;

    for (i = 0; i < 0xC; i++) {
        if (i == 0) {
            first = Hu3DModelCreate(HuDataSelHeapReadNum(0x4A0010, 0x10000000, HEAP_MODEL));
            m = first;
        } else {
            m = Hu3DModelLink(first);
        }
        obj->mdlId[i + 0x6E] = m;
        Hu3DModelAttrSet(m, 1);
        Hu3DModelLayerSet(m, 4);
    }
}

void fn_1_5D2C(OMOBJ *obj)
{
    MGSTAGEWORK *work = (MGSTAGEWORK *)obj->data;
    MGSLOT *slot;
    f32 target;
    s16 m;
    s32 i;

    for (i = 0; i < 0xC; i++) {
        slot = &work->slotsB[i];
        m = obj->mdlId[i + 0x6E];
        switch (slot->state) {
        case 0:
            break;
        case 1:
            slot->state++;
            slot->f08 = 0;
            Hu3DModelPosSetV(m, &slot->pos);
            Hu3DModelRotSet(m, 0.0f, 0.0f, 0.0f);
            Hu3DModelAttrReset(m, 1);
            break;
        case 2:
            slot->pos.y += 5.0;
            Hu3DModelPosSetV(m, &slot->pos);
            Hu3DModelRotSet(m, 0.0f, (f32)(slot->f08 * 0x14), 0.0f);
            if (!((f64)(++slot->f08) < 24.0)) {
                slot->state = 0;
                Hu3DModelAttrSet(m, 1);
            }
            break;
        }
    }
}

void fn_1_5EDC(Vec *pos)
{
    MGSLOT *slot;
    s32 i;
    OMOBJ *stage = lbl_1_bss_38;
    MGSTAGEWORK *work;

    work = (MGSTAGEWORK *)stage->data;
    for (i = 0; i < 0xC; i++) {
        slot = &work->slotsB[i];
        if (slot->state == 0) {
            slot->state++;
            slot->pos = *pos;
            break;
        }
    }
}

void fn_1_5F6C(OMOBJ *obj)
{
    MGSTAGEWORK *work = (MGSTAGEWORK *)obj->data;
    MGSLOT *slot;
    MGSTAGEDEF *def;
    s16 m;
    s32 i;

    for (i = 0; i < 4; i++) {
        slot = &work->slotsC[i];
        def = &lbl_1_data_174[i];
        m = Hu3DModelCreate(HuDataSelHeapReadNum(def->id, 0x10000000, HEAP_MODEL));
        obj->mdlId[i + 0x7C] = m;
        Hu3DModelAttrSet(m, 0x40000001);
        Hu3DModelLayerSet(m, (s16)def->layer);
    }
}

void fn_1_601C(OMOBJ *obj)
{
    MGSTAGEWORK *work = (MGSTAGEWORK *)obj->data;
    MGSLOT *slot;
    MGSTAGEDEF *def;
    f32 t = (f32)((f32)obj->work[0] / 2400.0);
    s16 m;
    s32 i;

    for (i = 0; i < 4; i++) {
        slot = &work->slotsC[i];
        def = &lbl_1_data_174[i];
        m = obj->mdlId[i + 0x7C];
        slot->pos.x = def->from.x + (t * (def->to.x - def->from.x));
        slot->pos.y = def->from.y + (t * (def->to.y - def->from.y));
        slot->pos.z = def->from.z + (t * (def->to.z - def->from.z));
        slot->rot.y = slot->rot.y + def->rotSpeed;
        Hu3DModelPosSetV(m, &slot->pos);
        Hu3DModelRotSetV(m, &slot->rot);
    }
}

void fn_1_6154(OMOBJ *obj)
{
    MGSTAGEWORK *work = (MGSTAGEWORK *)obj->data;
    s16 first;
    s16 mot;
    s16 m;
    s32 i;

    for (i = 0; i < 3; i++) {
        if (i == 0) {
            first = Hu3DModelCreate(HuDataSelHeapReadNum(0x4A000B, 0x10000000, HEAP_MODEL));
            m = first;
            mot = Hu3DJointMotion(m, HuDataSelHeapReadNum(0x4A000C, 0x10000000, HEAP_MODEL));
        } else {
            m = Hu3DModelLink(first);
        }
        obj->mdlId[i + 0x80] = m;
        Hu3DMotionSet(m, mot);
        Hu3DModelAttrSet(m, 0x40000001);
        Hu3DModelLayerSet(m, 0);
    }
}

void fn_1_6234(OMOBJ *obj)
{
    MGSTAGEWORK *work = (MGSTAGEWORK *)obj->data;
    MGSLOT *slot;
    f32 ang;
    f32 a;
    f32 b;
    f32 c;
    s16 m;
    s32 i;

    for (i = 0; i < 3; i++) {
        slot = &work->slotsD[i];
        m = obj->mdlId[i + 0x80];
        switch (slot->state) {
        case 0:
            slot->state++;
            slot->f08 = (s32)(240.0 + (f64)(s32)(240.0 * (1.52587890625e-05 * (f32)(s32)frand())));
            slot->pos = lbl_1_data_204[i];
            slot->vel.y = 0.0f;
            slot->f38 = (f32)(360.0 * (1.52587890625e-05 * (f32)(s32)frand()));
        case 1:
            slot->rot.x = fn_1_1D34(slot->rot.x, 0.0f, 0.1f);
            if ((slot->f08 -= 1) > 0) {
                break;
            }
            ang = slot->f38;
            if (ang < 135.0f || (ang > 180.0f && ang < 315.0f)) {
                break;
            }
            slot->f08 = 5;
            slot->state++;
        case 2:
            slot->rot.x = fn_1_1D34(slot->rot.x, -70.0f, 0.1f);
            if ((slot->f08 -= 1) > 0) {
                break;
            }
            slot->state++;
            slot->vel.y = 30.000002f;
            fn_1_6814(&slot->pos);
        case 3:
            slot->rot.x += 1.5;
            slot->vel.y = slot->vel.y - 1.3333334028720856;
            if (slot->vel.y > 0.0f || slot->pos.y > lbl_1_data_204[i].y) {
                break;
            }
            fn_1_6814(&slot->pos);
            slot->pos.y = lbl_1_data_204[i].y;
            slot->vel.y = 0.0f;
            slot->state = 1;
            slot->f08 = (s32)(240.0 + (f64)(s32)(240.0 * (1.52587890625e-05 * (f32)(s32)frand())));
            break;
        }
        a = (f32)(1.2 + slot->f38);
        b = (f32)fmod(a, 360.0);
        c = b;
        slot->f38 = c;
        if (slot->f38 < 0.0) {
            slot->f38 += 360.0;
        }
        slot->vel.x = (lbl_1_data_204[i].x + (100.0 * (3.0 * sin((3.141592653589793 * slot->f38) / 180.0)))) - slot->pos.x;
        slot->vel.z = (lbl_1_data_204[i].z + (100.0 * cos((3.141592653589793 * slot->f38) / 180.0))) - slot->pos.z;
        ang = (f32)(180.0 * (atan2(slot->vel.x, slot->vel.z) / 3.141592653589793));
        slot->rot.y = fn_1_1D34(slot->rot.y, ang, 0.1f);
        PSVECAdd(&slot->pos, &slot->vel, &slot->pos);
        Hu3DModelPosSetV(m, &slot->pos);
        Hu3DModelRotSetV(m, &slot->rot);
    }
}

void fn_1_6814(Vec *pos)
{
    Vec v;

    v.x = pos->x;
    v.y = -100.0f;
    v.z = pos->z;
    fn_1_78B8(&v, 10.0f);
}

// Remaining difference: fmul operand order.
void fn_1_6870(OMOBJ *obj)
{
    MGSTAGEWORK *work = (MGSTAGEWORK *)obj->data;
    s16 first;
    s16 m;
    f64 maxT;
    s32 i;

    for (i = 0; i < 3; i++) {
        if (i == 0) {
            first = Hu3DModelCreate(HuDataSelHeapReadNum(0x4A000A, 0x10000000, HEAP_MODEL));
            m = first;
        } else {
            m = Hu3DModelLink(first);
        }
        obj->mdlId[i + 0x83] = m;
        Hu3DModelAttrSet(m, 0x40000001);
        maxT = Hu3DMotionMaxTimeGet(m) - 1.0;
        Hu3DMotionTimeSet(m, (f32)(maxT * (1.52587890625e-05 * (f32)(s32)frand())));
        Hu3DModelLayerSet(m, 4);
    }
}

void fn_1_69A4(OMOBJ *obj)
{
    MGSTAGEWORK *work = (MGSTAGEWORK *)obj->data;
    MGSLOT *slot;
    f32 target;
    s16 m;
    s32 i;

    for (i = 0; i < 3; i++) {
        slot = &work->slotsE[i];
        m = obj->mdlId[i + 0x83];
        switch (slot->state) {
        case 0:
            slot->state++;
            slot->f08 = 0;
            slot->pos = lbl_1_data_228[i];
        case 1:
            slot->vel.x = slot->vel.x + 0.05 * (3.0517578125e-05 * (f32)(frand() - 0x8000));
            slot->vel.z = slot->vel.z + 0.05 * (3.0517578125e-05 * (f32)(frand() - 0x8000));
            if (slot->vel.x > 0.5) {
                slot->vel.x = 0.5f;
            }
            if (slot->vel.x < -0.5) {
                slot->vel.x = -0.5f;
            }
            if (slot->vel.z > 0.3) {
                slot->vel.z = 0.3f;
            }
            if (slot->vel.z < -0.3) {
                slot->vel.z = -0.3f;
            }
            break;
        }
        target = (f32)(180.0 + (180.0 * (atan2(slot->vel.x, slot->vel.z) / 3.141592653589793)));
        slot->rot.y = fn_1_1D34(slot->rot.y, target, 0.05f);
        PSVECAdd(&slot->pos, &slot->vel, &slot->pos);
        Hu3DModelPosSetV(m, &slot->pos);
        Hu3DModelRotSetV(m, &slot->rot);
    }
}

void fn_1_6C54(OMOBJ *obj)
{
    MGSTAGEWORK *work = (MGSTAGEWORK *)obj->data;

    work->waterX = work->waterY = 0.0f;
    Hu3DWaterCreate(2, (void *)1, NULL, NULL, 0, &lbl_1_data_26C[0], &lbl_1_data_26C[1]);
    Hu3DWaterPadYSet(-100.0f);
    Hu3DWaterGlowSet(8, &lbl_1_data_284);
    Hu3DWaterIndTexMtxSet((f32 (*)[3])lbl_1_data_288);
}

void fn_1_6CF4(OMOBJ *obj)
{
    MGSTAGEWORK *work = (MGSTAGEWORK *)obj->data;

    work->waterX += 0.0005;
    work->waterY += 0.0005;
    Hu3DWaterTexPosSet(work->waterX, work->waterY, 0.0f);
}

s32 fn_1_6D78(void)
{
    s32 i;
    s32 count;
    OMOBJ *stage = lbl_1_bss_38;
    MGSLOT *slot;
    MGSTAGEWORK *work;

    work = (MGSTAGEWORK *)stage->data;
    count = 0;
    for (i = 0; i < 0x42; i++) {
        slot = &work->slotsA[i];
        if (slot->state != 2) {
            count++;
        }
    }
    return count;
}

s32 fn_1_6DEC(s32 idx, Vec *pos, f32 *rotY)
{
    OMOBJ *stage = lbl_1_bss_38;
    HSFOBJECT *hobj;
    s16 m;

    m = stage->mdlId[0x7B];
    hobj = Hu3DModelObjPtrGet(m, lbl_1_data_2A0[idx]);
    if (hobj != NULL) {
        pos->x = hobj->mesh.curr.pos.x;
        pos->y = hobj->mesh.curr.pos.y;
        pos->z = hobj->mesh.curr.pos.z;
        *rotY = hobj->mesh.curr.rot.y;
    }
    return Hu3DMotionEndCheck(m) == 0;
}

f32 fn_1_6E94(f64 x)
{
    OMOBJ *stage = lbl_1_bss_38;
    MGSTAGEWORK *work = (MGSTAGEWORK *)stage->data;
    s32 i;

    i = (s32)((x + (0.5 * work->baseY[0])) / work->baseY[0]);
    if (i < 0) {
        i = 0;
    }
    if (i >= 3) {
        return 0.0f;
    }
    return work->speed[i];
}
