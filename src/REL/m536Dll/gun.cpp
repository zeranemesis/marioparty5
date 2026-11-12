#include "REL/m536Dll.h"

extern "C" {

float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);
extern void *lbl_1_bss_A04;
extern void *lbl_1_data_1FC[];

// One cannon: a list node, a PTMF hook, three linked models and its placement.
struct M536GunUnit {
    M536_NODE m_node;   /* 0x00 */
    M536_PTMF m_hook;   /* 0x08 */
    s16 m_mdlBody;      /* 0x14 */
    s16 m_mdlShadow;    /* 0x16 */
    s16 m_mdlMuzzle;    /* 0x18 */
    s32 m_idx;          /* 0x1C */
    Vec m_pos;          /* 0x20 */
    f32 m_rotY;         /* 0x2C */
    s32 m_timer;        /* 0x30 */
};

// The list head is followed by one word of padding in .bss.
struct M536GunList {
    M536_NODE m_node;
    s32 m_pad;
};

// Singleton owning the three source models and spawning the four cannons.
struct M536Gun {
    M536_NODE m_node;   /* 0x00 */
    void **m_vptr;      /* 0x08 */
    s32 m_flag;         /* 0x0C */
    s16 m_mdl[3];       /* 0x10 */
};

M536Gun *fn_1_F878(M536Gun *self, s16 freeIt);
void fn_1_F98C(M536Gun *self, s32 bit);
void fn_1_F96C(void);
void fn_1_FF2C(void);
void fn_1_FBF8(void);
void fn_1_FCE4(void);
void fn_1_FD14(M536GunUnit *unit);
void fn_1_102B8(M536GunList *list, void (*cb)(M536GunUnit *));
void fn_1_10318(M536GunUnit *node, M536GunList *stop, void (*cb)(M536GunUnit *));
void fn_1_FF9C(s32 idx, Vec *pos);
M536GunUnit *fn_1_F9B4(M536GunUnit *self, s16 *a, s16 *b, s16 *c, s32 idx);
void *fn_1_103D0(void *self, s16 freeIt);

M536_PTMF lbl_1_data_69C = { 0, -1, (void *)fn_1_FF2C };
void *lbl_1_data_6A8[5] = { 0, 0, (void *)fn_1_F878, (void *)fn_1_F98C, (void *)fn_1_F96C };

M536_RODATA const f32 lbl_1_rodata_318[4] = { 45.0f, -45.0f, 135.0f, -135.0f };

u8 lbl_1_bss_AC8[12];
M536GunList lbl_1_bss_AD4;

M536Gun *fn_1_F670(M536Gun *self)
{
    M536Gun *ms;
    s32 i;
    M536GunUnit *unit;

    fn_1_1264(self);
    self->m_vptr = lbl_1_data_1FC;
    self->m_flag = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    if (lbl_1_bss_A04 != NULL) {
        fn_1_1D4("ms_pointer == 0", "singleton.h", 0x35);
    }
    ms = (M536Gun *)((u8 *)self + 0x10);
    if (ms != NULL) {
        ms = (M536Gun *)((u8 *)ms - 0x10);
    }
    lbl_1_bss_A04 = ms;
    self->m_vptr = lbl_1_data_6A8;
    fn_1_2C94(&self->m_mdl[0], 0x5E0008);
    fn_1_2C94(&self->m_mdl[1], 0x5E0006);
    fn_1_2C94(&self->m_mdl[2], 0x5E0009);
    Hu3DModelAttrSet(self->m_mdl[0], 1);
    Hu3DModelAttrSet(self->m_mdl[1], 1);
    Hu3DModelAttrSet(self->m_mdl[2], 1);
    for (i = 0; i < 4; i++) {
        unit = (M536GunUnit *)fn_1_0(sizeof(M536GunUnit));
        if (unit != NULL) {
            fn_1_F9B4(unit, &self->m_mdl[0], &self->m_mdl[1], &self->m_mdl[2], i);
        }
    }
    return self;
}

M536Gun *fn_1_F7EC(M536Gun *self, s16 freeIt)
{
    if (self != NULL) {
        if (lbl_1_bss_A04 == NULL) {
            fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A04 = NULL;
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M536Gun *fn_1_F878(M536Gun *self, s16 freeIt)
{
    if (self != NULL) {
        self->m_vptr = lbl_1_data_6A8;
        fn_1_FBF8();
        fn_1_30F4(&self->m_mdl[2], -1);
        fn_1_30F4(&self->m_mdl[1], -1);
        fn_1_30F4(&self->m_mdl[0], -1);
        if ((M536Gun *)((u8 *)self + 0x10) != NULL) {
            if (lbl_1_bss_A04 == NULL) {
                fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
            }
            lbl_1_bss_A04 = NULL;
        }
        if (self != NULL) {
            self->m_vptr = lbl_1_data_1FC;
            if (self != NULL) {
                fn_1_1270(self, 0);
            }
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_F96C(void)
{
    fn_1_FCE4();
}

void fn_1_F98C(M536Gun *self, s32 bit)
{
    self->m_flag &= ~(1 << bit);
    if (bit < 5) {
    }
}

M536GunUnit *fn_1_F9B4(M536GunUnit *self, s16 *a, s16 *b, s16 *c, s32 idx)
{
    M536_PTMF tmp;
    s16 mdl;

    fn_1_1264(self);
    fn_1_2DB4(&self->m_mdlBody, a);
    fn_1_2DB4(&self->m_mdlShadow, b);
    fn_1_2DB4(&self->m_mdlMuzzle, c);
    fn_1_1330(self, &lbl_1_bss_AD4);
    self->m_idx = idx;
    mdl = self->m_mdlBody;
    Hu3DModelLayerSet(mdl, 4);
    Hu3DModelCameraSet(mdl, 1);
    Hu3DModelAttrReset(mdl, 1);
    Hu3DModelAttrSet(mdl, 0x40000002);
    Hu3DModelAttrReset(mdl, 0x40000001);
    Hu3DMotionSet(mdl, Hu3DMotionIDGet(mdl));
    mdl = self->m_mdlMuzzle;
    Hu3DModelLayerSet(mdl, 4);
    Hu3DModelCameraSet(mdl, 1);
    Hu3DModelAttrReset(mdl, 1);
    Hu3DModelAttrSet(mdl, 0x40000002);
    Hu3DModelAttrReset(mdl, 0x40000001);
    Hu3DMotionSet(mdl, Hu3DMotionIDGet(mdl));
    mdl = self->m_mdlShadow;
    Hu3DModelLayerSet(mdl, 3);
    Hu3DModelCameraSet(mdl, 1);
    Hu3DModelAttrReset(mdl, 1);
    Hu3DModelAttrSet(mdl, 0x40000002);
    Hu3DModelAttrReset(mdl, 0x40000001);
    tmp = lbl_1_data_69C;
    self->m_hook = tmp;
    self->m_timer = 0;
    return self;
}

M536GunUnit *fn_1_FBA0(M536GunUnit *self, s16 freeIt)
{
    if (self != NULL) {
        fn_1_1270(self, 0);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_FBF8(void)
{
    u8 empty;
    M536GunUnit *unit;

    for (;;) {
        empty = 0;
        if (lbl_1_bss_AD4.m_node.next == &lbl_1_bss_AD4.m_node && lbl_1_bss_AD4.m_node.next == lbl_1_bss_AD4.m_node.prev) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        unit = (M536GunUnit *)lbl_1_bss_AD4.m_node.prev;
        if (unit != NULL) {
            fn_1_30F4(&unit->m_mdlMuzzle, -1);
            fn_1_30F4(&unit->m_mdlShadow, -1);
            fn_1_30F4(&unit->m_mdlBody, -1);
            if (unit != NULL) {
                fn_1_1270(unit, 0);
            }
            fn_1_C8(unit);
        }
    }
}

void fn_1_FCE4(void)
{
    fn_1_102B8(&lbl_1_bss_AD4, fn_1_FD14);
}

void fn_1_FD14(M536GunUnit *unit)
{
    Vec rot;
    f32 rotY;
    f32 spin;
    f32 yShadow;
    s16 body, bodyId;
    s16 muzzle, muzzleId;
    s16 shadow, shadowId;

    __ptmf_scall(unit);
    rotY = unit->m_rotY;
    rot.x = 0.0f;
    rot.y = rotY;
    rot.z = 0.0f;
    body = unit->m_mdlBody;
    bodyId = body;
    Hu3DModelRotSet(bodyId, rot.x, rot.y, rot.z);
    Hu3DModelPosSet(bodyId, unit->m_pos.x, unit->m_pos.y, unit->m_pos.z);
    muzzle = unit->m_mdlMuzzle;
    muzzleId = muzzle;
    Hu3DMotionSpeedSet(muzzleId, 1.0f);
    unit->m_timer++;
    spin = 360.0 * -((f32)(unit->m_timer & 0x3FF) * 0.0009765625f - 0.5);
    Hu3DModelRotSet(muzzleId, 0.0f, spin, 0.0f);
    Hu3DModelPosSet(muzzleId, unit->m_pos.x, unit->m_pos.y, unit->m_pos.z);
    rot.x = 0.0f;
    rot.y = lbl_1_rodata_318[unit->m_idx];
    rot.z = 0.0f;
    shadow = unit->m_mdlShadow;
    shadowId = shadow;
    yShadow = unit->m_pos.y - 60.0f;
    Hu3DModelRotSet(shadowId, rot.x, rot.y, rot.z);
    Hu3DModelPosSet(shadowId, unit->m_pos.x, yShadow, unit->m_pos.z);
}

void fn_1_FF28(void)
{
}

void fn_1_FF2C(void)
{
}

void fn_1_FF30(s32 idx, f32 rotY)
{
    M536GunUnit *n;
    M536GunUnit *first;
    M536GunUnit *cur;
    M536GunUnit *next;
    M536GunList *end;

    first = (M536GunUnit *)lbl_1_bss_AD4.m_node.next;
    cur = first;
    n = cur;
    end = &lbl_1_bss_AD4;
    while (n != (M536GunUnit *)end) {
        if (n->m_idx == idx) {
            n->m_rotY = rotY;
        }
        next = (M536GunUnit *)n->m_node.next;
        n = next;
    }
}

void fn_1_FF9C(s32 idx, Vec *pos)
{
    M536GunUnit *first;
    M536GunUnit *cur;
    M536GunUnit *it;
    M536GunUnit *next;
    M536GunList *end;

    first = (M536GunUnit *)lbl_1_bss_AD4.m_node.next;
    cur = first;
    it = cur;
    end = &lbl_1_bss_AD4;
    while (it != (M536GunUnit *)end) {
        if (it->m_idx == idx) {
            it->m_pos.x = pos->x;
            it->m_pos.y = pos->y;
            it->m_pos.z = pos->z;
        }
        next = (M536GunUnit *)it->m_node.next;
        it = next;
    }
}

void fn_1_1001C(s32 idx)
{
    M536GunUnit *n;
    M536GunUnit *first;
    M536GunUnit *cur;
    M536GunUnit *next;
    M536GunList *end;
    s16 mdl;

    first = (M536GunUnit *)lbl_1_bss_AD4.m_node.next;
    cur = first;
    n = cur;
    end = &lbl_1_bss_AD4;
    while (n != (M536GunUnit *)end) {
        if (n->m_idx == idx) {
            mdl = n->m_mdlShadow;
            Hu3DModelAttrReset(mdl, 0x40000002);
            Hu3DModelAttrReset(mdl, 0x40000001);
            Hu3DMotionSet(mdl, Hu3DMotionIDGet(mdl));
        }
        next = (M536GunUnit *)n->m_node.next;
        n = next;
    }
}

void fn_1_100D0(s32 idx)
{
    M536GunUnit *n;
    M536GunUnit *first;
    M536GunUnit *cur;
    M536GunUnit *next;
    M536GunList *end;
    s16 mdl;

    first = (M536GunUnit *)lbl_1_bss_AD4.m_node.next;
    cur = first;
    n = cur;
    end = &lbl_1_bss_AD4;
    while (n != (M536GunUnit *)end) {
        if (n->m_idx == idx) {
            mdl = n->m_mdlBody;
            Hu3DModelAttrReset(mdl, 0x40000002);
            Hu3DModelAttrReset(mdl, 0x40000001);
            Hu3DMotionTimeSet(mdl, 0.0f);
        }
        next = (M536GunUnit *)n->m_node.next;
        n = next;
    }
}

void fn_1_10180(s32 idx, s32 mode)
{
    M536GunUnit *n;
    M536GunUnit *first;
    M536GunUnit *cur;
    M536GunUnit *next;
    M536GunList *end;
    f32 maxTime;
    s16 mdl;

    first = (M536GunUnit *)lbl_1_bss_AD4.m_node.next;
    cur = first;
    n = cur;
    end = &lbl_1_bss_AD4;
    while (n != (M536GunUnit *)end) {
        if (n->m_idx == idx) {
            mdl = n->m_mdlMuzzle;
            maxTime = Hu3DMotionMaxTimeGet(mdl);
            if (mode == 2) {
                Hu3DMotionTimeSet(mdl, 0.0f);
                Hu3DMotionStartEndSet(mdl, 0.0f, maxTime * 0.5f);
            } else {
                Hu3DMotionTimeSet(mdl, maxTime * 0.5f);
                Hu3DMotionStartEndSet(mdl, maxTime * 0.5f, maxTime);
            }
            Hu3DModelAttrReset(mdl, 0x40000002);
            Hu3DModelAttrReset(mdl, 0x40000001);
        }
        next = (M536GunUnit *)n->m_node.next;
        n = next;
    }
}

void fn_1_102B8(M536GunList *list, void (*cb)(M536GunUnit *))
{
    M536GunList *head;
    M536GunUnit *first;
    M536GunUnit *start;

    head = list;
    first = (M536GunUnit *)list->m_node.next;
    start = first;
    fn_1_10318(start, head, cb);
}

void fn_1_10318(M536GunUnit *node, M536GunList *stop, void (*cb)(M536GunUnit *))
{
    while (node != (M536GunUnit *)stop) {
        cb(node);
        node = (M536GunUnit *)node->m_node.next;
    }
}

void fn_1_10378(void)
{
    M536GunList *list;
    M536GunList *obj;

    list = &lbl_1_bss_AD4;
    fn_1_1264(list);
    obj = list;
    __register_global_object(obj, (void *)fn_1_103D0, &lbl_1_bss_AC8);
}

void *fn_1_103D0(void *self, s16 freeIt)
{
    if (self != NULL) {
        fn_1_1270(self, 0);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

}
