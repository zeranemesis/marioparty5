#define _MATH_H

#include "REL/m504dll.h"
#include "game/data.h"
#include "string.h"

#ifdef __MWERKS__
#define ST_RODATA __declspec(section ".rodata")
#else
#define ST_RODATA
#endif

#define ST_ABS(x) (((x) >= 0.0f) ? (x) : -(x))

extern float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);
extern void Hu3D3Dto2D(Vec *in, s32 camBit, Vec *out);

extern void fn_1_5EC(s32 seId);
extern void fn_1_5934(M504PLAYER *player, s32 slotNo, M504SLOT *slot);
extern void fn_1_5E1C(M504PLAYER *player);
extern s32 fn_1_5EC8(M504PLAYER *player);
extern void fn_1_9680(M504WANWAN *wanwan, M504SLOT *slot);
extern void fn_1_9C10(M504WANWAN *wanwan);
extern s32 fn_1_9CAC(M504WANWAN *wanwan);
extern void fn_1_C4F4(M504CAMERA *cam, s32 slotNo, M504SLOT *slot);
extern void fn_1_C71C(M504CAMERA *cam);
extern void fn_1_C74C(M504CAMERA *cam);

typedef struct M504StageDef_s {
    s16 mdlIdx;
    u16 type;
    f32 x0;
    f32 y0;
    f32 z0;
    f32 x1;
    f32 y1;
    f32 z1;
    s32 hidden;
} M504STAGEDEF;

typedef struct M504StageTbl_s {
    M504STAGEDEF def[10];
    s16 tail[14];
} M504STAGETBL;

extern M504STAGETBL lbl_1_data_A8;

void fn_1_255C(M504STAGE *stage, s32 slotNo, M504SLOT *slot);
void fn_1_3B9C(M504STAGE *stage);
void fn_1_3D9C(M504STAGE *stage);
s32 fn_1_3FCC(Vec *p);
void fn_1_4FA4(M504STAGE *stage);
void fn_1_4FEC(M504STAGE *stage);
void fn_1_5168(M504STAGE *stage);
void fn_1_51B4(M504STAGE *stage, u16 i);
void fn_1_53AC(M504STAGE *stage);
void fn_1_572C(M504STAGE *stage);

ST_RODATA const Vec lbl_1_rodata_38 = { 0.0f, 0.0f, 0.0f };
ST_RODATA const Vec lbl_1_rodata_4C = { 1.0f, 0.0f, 0.0f };
ST_RODATA const Vec lbl_1_rodata_58 = { 0.0f, 0.0f, 1.0f };
ST_RODATA const Vec lbl_1_rodata_64 = { 0.0f, 0.0f, 0.0f };
ST_RODATA const Vec lbl_1_rodata_70 = { -230.0f, 0.0f, 0.0f };
ST_RODATA const Vec lbl_1_rodata_7C = { 230.0f, 0.0f, 0.0f };
ST_RODATA const Vec lbl_1_rodata_88 = { 0.0f, 0.0f, 3700.0f };
ST_RODATA const Vec lbl_1_rodata_94 = { 0.0f, 0.0f, 0.0f };
ST_RODATA const Vec lbl_1_rodata_A0 = { 0.0f, 0.0f, 0.0f };
ST_RODATA const Vec lbl_1_rodata_AC = { 0.0f, 0.0f, 0.0f };
ST_RODATA const Vec lbl_1_rodata_B8 = { 0.0f, 0.0f, 0.0f };

void fn_1_23DC(M504SLOT *slot, s32 type, M504WORK *work)
{
    s32 flag;

    memset(slot, 0, sizeof(M504SLOT));
    slot->work = work;
    slot->slotNo = type;

    if (type & 1) {
        flag = 0;
    } else {
        flag = 1;
    }
    slot->unk46C = flag;

    fn_1_C4F4(&slot->cam, type, slot);
    fn_1_255C(&slot->stage, type, slot);
    fn_1_5934(&slot->player, type, slot);
    slot->player.active = 0;
    fn_1_9680(&slot->wanwan, slot);
    slot->player.active = 0;
}

void fn_1_24A0(M504SLOT *slot)
{
    fn_1_9C10(&slot->wanwan);
    fn_1_5E1C(&slot->player);
    fn_1_3B9C(&slot->stage);
    fn_1_C71C(&slot->cam);
    memset(slot, 0, sizeof(M504SLOT));
}

s32 fn_1_24F8(M504SLOT *slot)
{
    s32 ret;

    ret = 0;
    fn_1_5EC8(&slot->player);

    if (fn_1_9CAC(&slot->wanwan) != 0) {
        ret = 1;
    }
    fn_1_3D9C(&slot->stage);
    fn_1_C74C(&slot->cam);
    return ret;
}

#define ST_LINK(dst, idx, layer, attrFn, attrVal)                      \
    do {                                                               \
        s16 _m = Hu3DModelLink(mdlTbl[idx]);                           \
        dst = _m;                                                      \
        Hu3DModelPosSet(_m, 0.0f, 0.0f, 0.0f);                         \
        Hu3DModelRotSet(_m, 0.0f, 0.0f, 0.0f);                         \
        Hu3DModelCameraSet(_m, (u16)lbl_1_data_45E0[slotNo]);          \
        Hu3DModelLayerSet(_m, layer);                                  \
        attrFn(_m, attrVal);                                           \
    } while (0)

#define ST_CREATE(dst, num, layer, attrFn, attrVal)                                          \
    do {                                                                                     \
        mdl = Hu3DModelCreate(HuDataSelHeapReadNum(num, 0x10000000, HEAP_MODEL));            \
        dst = mdl;                                                                           \
        Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);                                              \
        Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);                                              \
        Hu3DModelCameraSet(mdl, (u16)lbl_1_data_45E0[slotNo]);                               \
        Hu3DModelLayerSet(mdl, layer);                                                       \
        attrFn(mdl, attrVal);                                                                \
    } while (0)

#define ST_JOINT(dst, mdlId, num) dst = Hu3DJointMotion(mdlId, HuDataSelHeapReadNum(num, 0x10000000, HEAP_MODEL))

void fn_1_255C(M504STAGE *stage, s32 slotNo, M504SLOT *slot)
{
    s16 *mdlTbl;
    M504STAGEOBJ *obj;
    M504STAGEDEF *def;
    s16 mdl;
    s32 i;

    mdlTbl = slot->work->mdl;
    memset(stage, 0, sizeof(M504STAGE));
    stage->slot = slot;
    ST_LINK(stage->mdl4, 1, 0, Hu3DModelAttrReset, 1);
    ST_LINK(stage->mdl6, 2, 0, Hu3DModelAttrSet, 1);
    ST_LINK(stage->mdl8, 3, 0, Hu3DModelAttrReset, 1);
    ST_LINK(stage->mdlC, 9, 0, Hu3DModelAttrReset, 1);
    ST_LINK(stage->mdlE, 4, 0, Hu3DModelAttrReset, 1);
    ST_LINK(stage->mdl10, 5, 0, Hu3DModelAttrReset, 1);
    ST_LINK(stage->mdl12, 6, 0, Hu3DModelAttrReset, 1);
    ST_LINK(stage->mdl14, 10, 0, Hu3DModelAttrReset, 1);
    ST_LINK(stage->mdl16, 7, 0, Hu3DModelAttrReset, 1);
    ST_LINK(stage->mdl18, 8, 0, Hu3DModelAttrReset, 1);
    ST_LINK(stage->mdl1A, 11, 0, Hu3DModelAttrReset, 1);
    ST_LINK(stage->mdl1C, 12, 0, Hu3DModelAttrSet, 1);
    ST_LINK(stage->mdl1E, 13, 0, Hu3DModelAttrSet, 1);
    ST_LINK(stage->mdl20, 14, 0, Hu3DModelAttrSet, 1);
    ST_CREATE(stage->mdl22, 0x3E0016, 0, Hu3DModelAttrReset, 1);
    ST_JOINT(stage->mtn24[0], mdl, 0x3E0026);
    ST_JOINT(stage->mtn24[1], mdl, 0x3E0027);
    ST_JOINT(stage->mtn24[2], mdl, 0x3E0028);
    Hu3DMotionSet(mdl, stage->mtn24[0]);
    Hu3DMotionSpeedSet(mdl, 1.0f);
    Hu3DModelAttrSet(mdl, 0x40000001);
    for (i = 0; i < 3; i++) {
        ST_CREATE(stage->mdl2E[i], 0x3E0017, 0, Hu3DModelAttrSet, 1);
    }
    ST_JOINT(stage->mtn34[0], mdl, 0x3E0029);
    ST_JOINT(stage->mtn34[1], mdl, 0x3E002A);
    ST_JOINT(stage->mtn34[2], mdl, 0x3E002B);
    ST_JOINT(stage->mtn34[3], mdl, 0x3E002C);
    ST_JOINT(stage->mtn34[4], mdl, 0x3E002D);
    ST_JOINT(stage->mtn34[5], mdl, 0x3E002E);
    ST_JOINT(stage->mtn34[6], mdl, 0x3E002F);
    Hu3DMotionSpeedSet(stage->mdl2E[0], 1.0f);
    Hu3DMotionSpeedSet(stage->mdl2E[1], 1.0f);
    Hu3DMotionSpeedSet(stage->mdl2E[2], 1.0f);
    Hu3DMotionSet(stage->mdl2E[0], stage->mtn34[0]);
    Hu3DModelAttrSet(stage->mdl2E[0], 0x40000001);
    Hu3DModelAttrReset(stage->mdl2E[0], 1);
    ST_CREATE(stage->mdl4A, 0x3E0018, 4, Hu3DModelAttrSet, 1);
    ST_JOINT(stage->mtn52, mdl, 0x3E0030);
    ST_CREATE(stage->mdl4E, 0x3E0019, 4, Hu3DModelAttrSet, 1);
    ST_JOINT(stage->mtn56, mdl, 0x3E0031);
    ST_CREATE(stage->mdl4C, 0x3E001A, 5, Hu3DModelAttrSet, 1);
    ST_JOINT(stage->mtn54, mdl, 0x3E0032);
    ST_CREATE(stage->mdl50, 0x3E001B, 5, Hu3DModelAttrSet, 1);
    ST_JOINT(stage->mtn58, mdl, 0x3E0033);
    ST_CREATE(stage->mdl60, 0x3E0013, 4, Hu3DModelAttrReset, 1);
    ST_JOINT(stage->mtn66, mdl, 0x3E0034);
    Hu3DMotionSet(stage->mdl60, stage->mtn66);
    Hu3DMotionTimeSet(stage->mdl60, 0.0f);
    Hu3DMotionSpeedSet(stage->mdl60, 1.0f);
    Hu3DModelAttrSet(stage->mdl60, 0x40000001);
    ST_CREATE(stage->mdl62, 0x3E0014, 4, Hu3DModelAttrSet, 1);
    ST_JOINT(stage->mtn68, mdl, 0x3E0035);
    ST_CREATE(stage->mdl64, 0x3E0015, 4, Hu3DModelAttrSet, 1);
    ST_JOINT(stage->mtn6A, mdl, 0x3E0036);
    stage->unk5C = -1;
    ST_CREATE(stage->mdl72, 0x3E001C, 0, Hu3DModelAttrReset, 1);
    ST_JOINT(stage->mtn74[0], mdl, 0x3E0037);
    ST_JOINT(stage->mtn74[1], mdl, 0x3E0038);
    Hu3DMotionSet(mdl, stage->mtn74[0]);
    Hu3DMotionSpeedSet(mdl, 1.0f);
    Hu3DModelAttrSet(mdl, 0x40000001);
    for (i = 0; i < 10; i++) {
        obj = &stage->obj[i];
        def = &lbl_1_data_A8.def[i];
        memset(obj, 0, sizeof(M504STAGEOBJ));
        obj->stage = stage;
        if (def->hidden != 0 || def->mdlIdx == -1) {
            mdl = -1;
        } else {
            mdl = Hu3DModelLink(mdlTbl[def->mdlIdx]);
        }
        obj->mdl = mdl;
        obj->type = def->type;
        obj->x0 = def->x0;
        obj->y0 = def->y0;
        obj->z0 = def->z0;
        obj->x1 = def->x1;
        obj->y1 = def->y1;
        obj->z1 = def->z1;
        obj->shown = 0;
        obj->hidden = def->hidden;
        obj->active = 1;
        if (def->hidden == 0 && mdl != -1) {
            Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
            Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
            Hu3DModelCameraSet(mdl, (u16)lbl_1_data_45E0[slotNo]);
            Hu3DModelLayerSet(mdl, 0);
            Hu3DModelAttrSet(mdl, 1);
        }
    }
}

void fn_1_3B9C(M504STAGE *stage)
{
    M504STAGEOBJ *obj;
    s32 i;

    for (i = 0; i < 2; i++) {
        Hu3DMotionKill(stage->mtn74[i]);
    }
    Hu3DMotionKill(stage->mtn6A);
    Hu3DMotionKill(stage->mtn68);
    Hu3DMotionKill(stage->mtn58);
    Hu3DMotionKill(stage->mtn54);
    Hu3DMotionKill(stage->mtn56);
    Hu3DMotionKill(stage->mtn52);
    for (i = 0; i < 7; i++) {
        Hu3DMotionKill(stage->mtn34[i]);
    }
    for (i = 0; i < 3; i++) {
        Hu3DMotionKill(stage->mtn24[i]);
    }
    for (i = 0; i < 6; i++) {
        obj = &stage->obj[i];
        if (obj->hidden == 0 && obj->mdl != -1) {
            Hu3DModelKill(obj->mdl);
        }
    }
    Hu3DModelKill(stage->mdl72);
    Hu3DModelKill(stage->mdl64);
    Hu3DModelKill(stage->mdl62);
    Hu3DModelKill(stage->mdl60);
    Hu3DModelKill(stage->mdl50);
    Hu3DModelKill(stage->mdl4C);
    Hu3DModelKill(stage->mdl4E);
    Hu3DModelKill(stage->mdl4A);
    for (i = 0; i < 3; i++) {
        Hu3DModelKill(stage->mdl2E[i]);
    }
    Hu3DModelKill(stage->mdl22);
    Hu3DModelKill(stage->mdl20);
    Hu3DModelKill(stage->mdl1E);
    Hu3DModelKill(stage->mdl1C);
    Hu3DModelKill(stage->mdl1A);
    Hu3DModelKill(stage->mdl18);
    Hu3DModelKill(stage->mdl16);
    Hu3DModelKill(stage->mdl14);
    Hu3DModelKill(stage->mdl12);
    Hu3DModelKill(stage->mdl10);
    Hu3DModelKill(stage->mdlE);
    Hu3DModelKill(stage->mdlC);
    Hu3DModelKill(stage->mdl8);
    Hu3DModelKill(stage->mdl6);
    Hu3DModelKill(stage->mdl4);
    memset(stage, 0, sizeof(M504STAGE));
}

void fn_1_3D9C(M504STAGE *stage)
{
    M504WORK *work;
    M504STAGEOBJ *obj;
    Vec scr0;
    Vec scr1;
    Vec scr2;
    Vec scr3;
    Vec p0;
    Vec p1;
    Vec p2;
    Vec p3;
    s32 i;

    work = stage->slot->work;
    for (i = 0; i < 6; i++) {
        obj = &stage->obj[i];
        if (obj->hidden == 0) {
            p0 = lbl_1_rodata_38;
            p1 = p0;
            p2 = p0;
            p3 = p0;
            p3.x = obj->x0;
            p3.z = obj->z0;
            p2.x = obj->x1;
            p2.z = obj->z0;
            p1.x = obj->x0;
            p1.z = obj->z1;
            p0.x = obj->x1;
            p0.z = obj->z1;
            Hu3D3Dto2D(&p3, stage->slot->cam.camBit, &scr0);
            Hu3D3Dto2D(&p2, stage->slot->cam.camBit, &scr1);
            Hu3D3Dto2D(&p1, stage->slot->cam.camBit, &scr2);
            Hu3D3Dto2D(&p0, stage->slot->cam.camBit, &scr3);
            if (work->unk1218 != 0 && i == 2) {
                obj->shown = 1;
                Hu3DModelAttrReset(obj->mdl, 1);
            } else if (fn_1_3FCC(&scr0) != 0 || fn_1_3FCC(&scr1) != 0 || fn_1_3FCC(&scr2) != 0 || fn_1_3FCC(&scr3) != 0) {
                if (obj->shown == 0) {
                    obj->shown = 1;
                    Hu3DModelAttrReset(obj->mdl, 1);
                }
            } else if (obj->shown != 0) {
                obj->shown = 0;
                Hu3DModelAttrSet(obj->mdl, 1);
            }
        }
    }
    fn_1_4FA4(stage);
}

s32 fn_1_3FCC(Vec *p)
{
    f32 lim;

    lim = 96.0f;

    /* NONMATCHING: 3 rows - MWCC branches over the first return block with bge
     * instead of branching forward to it with blt.
     */
    if (p->y < -lim) {
        return 0;
    }

    if (p->y >= lim + 576.0f) {
        return 0;
    }
    return 1;
}

s32 fn_1_4030(M504STAGE *stage, Vec *pos, Vec *out, s32 flag, s16 *wallIdx, s16 *cornerIdx, f32 range)
{
    Vec unusedA;
    Vec unusedB;
    Vec p;
    Vec res;
    Vec c0;
    Vec c1;
    Vec c2;
    Vec c3;
    Vec d0;
    Vec d1;
    Vec d2;
    Vec d3;
    Vec n;
    Vec v1;
    Vec v2;
    Vec t;
    M504STAGEOBJ *obj;
    f32 dx;
    f32 dz;
    f32 dx2;
    f32 dz2;
    f32 m0;
    f32 m1;
    f32 m2;
    f32 m3;
    f32 mag;
    f32 radius;
    f32 over;
    f32 x0;
    f32 x1;
    f32 z0;
    f32 z1;
    f32 e0;
    f32 e1;
    f32 e2;
    f32 e3;
    s32 ret;
    s32 i;

    unusedA = lbl_1_rodata_4C;
    unusedB = lbl_1_rodata_58;
    p = *pos;
    *wallIdx = -1;
    *cornerIdx = -1;
    ret = 0;
    res = lbl_1_rodata_64;
    *out = res;
    dx = 0.0f;
    dz = 0.0f;
    if ((p.x - range) < -400.0f) {
        dx = 0.0f + (-400.0f - (p.x - range));
        ret = 1;
    } else if ((p.x + range) > 400.0f) {
        dx = 0.0f + (400.0f - (p.x + range));
        ret = 1;
    }
    if ((p.z - range) < 0.0f) {
        dz = 0.0f + (0.0f - (p.z - range));
        ret = 1;
    } else if ((p.z + range) > 3550.0f) {
        c0 = lbl_1_rodata_70;
        c0.z = 3550.0f;
        t = c0;
        PSVECSubtract(&t, &p, &d0);
        m0 = PSVECMag(&d0);
        c1 = lbl_1_rodata_7C;
        c1.z = 3550.0f;
        v1 = c1;
        PSVECSubtract(&v1, &p, &d1);
        m1 = PSVECMag(&d1);
        if (m0 < range && p.x > t.x) {
            PSVECNormalize(&d0, &n);
            PSVECScale(&n, &n, range);
            PSVECSubtract(&n, &d0, &n);
            dx += -n.x;
            dz = 0.0f + -n.z;
            ret = 1;
        } else if (m1 < range && p.x < t.x) {
            PSVECNormalize(&d1, &n);
            PSVECScale(&n, &n, range);
            PSVECSubtract(&n, &d1, &n);
            dx += -n.x;
            dz = 0.0f + -n.z;
            ret = 1;
        } else if ((p.x - range) < -230.0f || (p.x + range) > 230.0f) {
            if (p.z < 3550.0f) {
                dz = 0.0f + (3550.0f - (p.z + range));
                ret = 1;
            } else if ((p.z - range) < 3550.0f) {
                dz = 0.0f + (3550.0f - (p.z - range));
                ret = 1;
            }
        }
        if (p.z > 3550.0f) {
            c2 = lbl_1_rodata_88;
            v2 = c2;
            PSVECSubtract(&v2, &p, &n);
            mag = PSVECMag(&n);
            if (mag > (250.0f - range)) {
                PSVECNormalize(&n, &n);
                PSVECScale(&n, &n, mag - (250.0f - range));
                dx += n.x;
                dz += n.z;
                ret = 1;
            }
        }
    }
    p.x += dx;
    p.z += dz;
    dz2 = 0.0f;
    dx2 = 0.0f;
    for (i = 0; i < 10; i++) {
        obj = &stage->obj[i];
        if (obj->active != 0 && (obj->hidden == 0 || flag != 0)) {
            if (obj->type == 1) {
                radius = obj->x1;
                PSVECSubtract((Vec *)&obj->x0, &p, &n);
                n.y = 0.0f;
                mag = PSVECMag(&n);
                if (mag < (range + radius)) {
                    over = (range + radius) - mag;
                    PSVECNormalize(&n, &n);
                    PSVECScale(&n, &v1, range - over);
                    PSVECAdd(&v1, &p, &v1);
                    PSVECScale(&n, &v2, -(radius - over));
                    PSVECAdd(&v2, (Vec *)&obj->x0, &v2);
                    dx2 += v1.x - v2.x;
                    dz2 += v1.z - v2.z;
                    ret = 1;
                    if (i < 6) {
                        *wallIdx = i;
                    }
                }
            } else {
                x0 = obj->x0;
                x1 = obj->x1;
                z0 = obj->z0;
                z1 = obj->z1;
                if ((p.x - range) < x1 && (p.x + range) > x0 && (p.z - range) < z1 && (p.z + range) > z0) {
                    c0 = lbl_1_rodata_94;
                    c0.x = x0;
                    c0.z = z0;
                    t = c0;
                    PSVECSubtract(&t, &p, &d0);
                    m0 = PSVECMag(&d0);
                    c1 = lbl_1_rodata_A0;
                    c1.x = x1;
                    c1.z = z0;
                    t = c1;
                    PSVECSubtract(&t, &p, &d1);
                    m1 = PSVECMag(&d1);
                    c2 = lbl_1_rodata_AC;
                    c2.x = x0;
                    c2.z = z1;
                    t = c2;
                    PSVECSubtract(&t, &p, &d2);
                    m2 = PSVECMag(&d2);
                    c3 = lbl_1_rodata_B8;
                    c3.x = x1;
                    c3.z = z1;
                    t = c3;
                    PSVECSubtract(&t, &p, &d3);
                    m3 = PSVECMag(&d3);
                    if (m0 < range && p.x < x0 && p.z < z0) {
                        PSVECNormalize(&d0, &n);
                        PSVECScale(&n, &n, range - m0);
                        dx2 += -n.x;
                        dz2 += -n.z;
                        *cornerIdx = 0;
                        ret = 1;
                    } else if (m1 < range && p.x > x1 && p.z < z0) {
                        PSVECNormalize(&d1, &n);
                        PSVECScale(&n, &n, range - m1);
                        dx2 += -n.x;
                        dz2 += -n.z;
                        *cornerIdx = 1;
                        ret = 1;
                    } else if (m2 < range && p.x < x0 && p.z > z1) {
                        PSVECNormalize(&d2, &n);
                        PSVECScale(&n, &n, range - m2);
                        dx2 += -n.x;
                        dz2 += -n.z;
                        *cornerIdx = 2;
                        ret = 1;
                    } else if (m3 < range && p.x > x1 && p.z > z1) {
                        PSVECNormalize(&d3, &n);
                        PSVECScale(&n, &n, range - m3);
                        dx2 += -n.x;
                        dz2 += -n.z;
                        *cornerIdx = 3;
                        ret = 1;
                    } else if (p.x > x0 && p.x < x1 && p.z < z0) {
                        dz2 += z0 - (p.z + range);
                        ret = 1;
                    } else if (p.x > x0 && p.x < x1 && p.z > z1) {
                        dz2 += z1 - (p.z - range);
                        ret = 1;
                    } else if (p.z > z0 && p.z < z1 && p.x < x0) {
                        dx2 += x0 - (p.x + range);
                        ret = 1;
                        *wallIdx = i;
                    } else if (p.z > z0 && p.z < z1 && p.x > x1) {
                        dx2 += x1 - (p.x - range);
                        ret = 1;
                    } else if (flag == 0) {
                        e0 = z0 - (p.z + range);
                        e1 = z1 - (p.z - range);
                        e2 = x0 - (p.x + range);
                        e3 = x1 - (p.x - range);
                        if (ST_ABS(e2) < ST_ABS(e3) && ST_ABS(e2) < ST_ABS(e0) && ST_ABS(e2) < ST_ABS(e1)) {
                            dx2 += e2;
                        } else if (ST_ABS(e3) < ST_ABS(e2) && ST_ABS(e3) < ST_ABS(e0) && ST_ABS(e3) < ST_ABS(e1)) {
                            dx2 += e3;
                        } else if (ST_ABS(e0) < ST_ABS(e2) && ST_ABS(e0) < ST_ABS(e3) && ST_ABS(e0) < ST_ABS(e1)) {
                            dz2 += e0;
                        } else {
                            dz2 += e1;
                        }
                        ret = 1;
                    }
                    if (i < 6 && ret != 0) {
                        *wallIdx = i;
                    }
                }
            }
        }
    }
    p.x += dx2;
    p.z += dz2;
    out->x = p.x - pos->x;
    out->z = p.z - pos->z;
    return ret;
}

void fn_1_4FA4(M504STAGE *stage)
{
    fn_1_4FEC(stage);
    fn_1_5168(stage);
    fn_1_53AC(stage);
    fn_1_572C(stage);
}

void fn_1_4FEC(M504STAGE *stage)
{
    M504SLOT *slot;
    M504WORK *work;

    slot = stage->slot;
    work = slot->work;

    switch (stage->st2A) {
        case 1:
            Hu3DMotionShiftSet(stage->mdl22, stage->mtn24[2], 0.0f, 4.0f, 0);
            stage->st2A++;
            if (work->unk1210 != 0) {
                if (slot->unk46C != 0) {
                    fn_1_5EC(0x653);
                    return;
                }
                fn_1_5EC(0x654);
                return;
            }
        case 0:
            return;
        case 2:
            if (Hu3DMotionShiftIDGet(stage->mdl22) < 0) {
                Hu3DMotionShiftSet(stage->mdl22, stage->mtn24[1], 0.0f, 30.0f, 0x40000001);
                stage->tm2C = 360;
                stage->st2A++;
                return;
            }
            break;
        case 3: {
            s16 c = stage->tm2C;
            stage->tm2C = c - 1;
            if (c <= 0) {
                Hu3DMotionShiftSet(stage->mdl22, stage->mtn24[0], 0.0f, 10.0f, 0x40000001);
                stage->tm2C = 0;
                stage->st2A = 0;
            }
            break;
        }
    }
}

void fn_1_5168(M504STAGE *stage)
{
    fn_1_51B4(stage, 0);
    fn_1_51B4(stage, 1);
    fn_1_51B4(stage, 2);
}

void fn_1_51B4(M504STAGE *stage, u16 i)
{
    M504SLOT *slot;
    M504WORK *work;
    s16 mtnA;
    s16 mtnB;
    f32 maxTime;

    slot = stage->slot;
    work = slot->work;
    if (i == 0) {
        mtnA = stage->mtn34[1];
    } else {
        if (i == 1) {
            mtnA = stage->mtn34[3];
        } else {
            mtnA = stage->mtn34[5];
        }
    }
    if (i == 0) {
        mtnB = stage->mtn34[2];
    } else {
        if (i == 1) {
            mtnB = stage->mtn34[4];
        } else {
            mtnB = stage->mtn34[6];
        }
    }
    switch (stage->st42[i]) {
        case 1:
            Hu3DModelAttrReset(stage->mdl2E[i], 1);
            Hu3DModelAttrReset(stage->mdl2E[i], 0x40000001);
            Hu3DMotionSet(stage->mdl2E[i], mtnA);
            stage->st42[i]++;
            if (work->unk1210 != 0) {
                if (slot->unk46C != 0) {
                    fn_1_5EC(0x651);
                    return;
                }
                fn_1_5EC(0x652);
                return;
            }
        case 0:
            return;
        case 2:
            maxTime = Hu3DMotionMaxTimeGet(stage->mdl2E[i]);
            if (Hu3DMotionTimeGet(stage->mdl2E[i]) >= maxTime) {
                Hu3DModelAttrSet(stage->mdl2E[i], 0x40000001);
                Hu3DMotionSet(stage->mdl2E[i], mtnB);
                stage->st42[i]++;
            }
            break;
    }
}

void fn_1_53AC(M504STAGE *stage)
{
    M504SLOT *slot;
    M504WORK *work;
    f32 t;

    slot = stage->slot;
    work = slot->work;
    switch (stage->st6C) {
        case 0:
            break;
        case 1:
            Hu3DMotionShapeSet(stage->mdl62, stage->mtn68);
            Hu3DMotionShapeTimeSet(stage->mdl62, 0.0f);
            Hu3DMotionShapeSpeedSet(stage->mdl62, 1.0f);
            Hu3DModelAttrReset(stage->mdl62, 0x40000001);
            Hu3DModelAttrSet(stage->mdl60, 1);
            Hu3DModelAttrReset(stage->mdl62, 1);
            if (work->unk1210 != 0) {
                if (slot->unk46C != 0) {
                    fn_1_5EC(0x657);
                } else {
                    fn_1_5EC(0x658);
                }
            }
            stage->st6C++;
            break;
        case 2:
            t = Hu3DData[stage->mdl62].motShapeWork.time;
            if (t >= Hu3DMotionShapeMaxTimeGet(stage->mdl62)) {
                Hu3DMotionShapeSet(stage->mdl64, stage->mtn6A);
                Hu3DMotionShapeTimeSet(stage->mdl64, 0.0f);
                Hu3DMotionShapeSpeedSet(stage->mdl64, 1.0f);
                Hu3DModelAttrSet(stage->mdl64, 0x40000001);
                Hu3DModelAttrSet(stage->mdl62, 1);
                Hu3DModelAttrReset(stage->mdl64, 1);
                stage->st6C++;
            }
            break;
        case 3:
            t = Hu3DData[stage->mdl64].motShapeWork.time;
            if (t >= Hu3DMotionShapeMaxTimeGet(stage->mdl64)) {
                Hu3DMotionShapeTimeSet(stage->mdl64, 0.0f);
            }
            break;
    }
    switch (stage->st6E) {
        case 1:
            Hu3DMotionShapeSet(stage->mdl4A, stage->mtn52);
            Hu3DMotionShapeSet(stage->mdl4C, stage->mtn54);
            Hu3DModelAttrReset(stage->mdl4A, 1);
            Hu3DModelAttrReset(stage->mdl4C, 1);
            stage->st6E++;
            return;
        case 2:
            t = Hu3DData[stage->mdl4A].motShapeWork.time;
            if (t >= Hu3DMotionShapeMaxTimeGet(stage->mdl4A)) {
                Hu3DMotionShapeSet(stage->mdl4E, stage->mtn56);
                Hu3DMotionShapeSet(stage->mdl50, stage->mtn58);
                Hu3DModelAttrSet(stage->mdl4A, 1);
                Hu3DModelAttrSet(stage->mdl4C, 1);
                Hu3DModelAttrReset(stage->mdl4E, 1);
                Hu3DModelAttrReset(stage->mdl50, 1);
                stage->st6E++;
                return;
            }
        case 0:
            return;
        case 3:
            t = Hu3DData[stage->mdl4E].motShapeWork.time;
            if (t >= Hu3DMotionShapeMaxTimeGet(stage->mdl4E)) {
                Hu3DMotionShapeTimeSet(stage->mdl4E, 0.0f);
            }
            t = Hu3DData[stage->mdl50].motShapeWork.time;
            if (t >= Hu3DMotionShapeMaxTimeGet(stage->mdl50)) {
                Hu3DMotionShapeTimeSet(stage->mdl50, 0.0f);
            }
            break;
    }
}

void fn_1_572C(M504STAGE *stage)
{
    M504SLOT *slot;
    M504WORK *work;
    f32 t;

    slot = stage->slot;
    work = slot->work;
    switch (stage->st78) {
        case 1:
            Hu3DMotionShapeSet(stage->mdl72, stage->mtn74[1]);
            Hu3DMotionShapeSpeedSet(stage->mdl72, 1.0f);
            Hu3DModelAttrReset(stage->mdl72, 0x40000001);
            stage->obj[1].active = 0;
            if (work->unk1210 != 0) {
                if (slot->unk46C != 0) {
                    fn_1_5EC(0x655);
                } else {
                    fn_1_5EC(0x656);
                }
            }
            stage->st78++;
            return;
        case 2:
            t = Hu3DData[stage->mdl72].motShapeWork.time;
            if (t >= Hu3DMotionShapeMaxTimeGet(stage->mdl72)) {
                Hu3DModelAttrSet(stage->mdl72, 1);
                stage->st78++;
            }
        case 0:
            return;
    }
}

void fn_1_5870(M504STAGE *stage, s16 idx)
{
    switch (idx) {
        case 1:
            if (stage->st78 == 0) {
                stage->st78 = 1;
                return;
            }
            return;
        case 3:
            if (stage->st42[0] == 0) {
                stage->st42[0] = 1;
                return;
            }
            if (stage->st42[1] == 0) {
                stage->st42[1] = 1;
                return;
            }
            if (stage->st42[2] == 0) {
                stage->st42[2] = 1;
                return;
            }
            break;
        case 4:
            stage->st2A = 1;
            return;
        case 5:
            if (stage->st6C == 0) {
                stage->st6C = 1;
            }
            if (stage->st6E == 0) {
                stage->st6E = 1;
            }
            break;
    }
}
