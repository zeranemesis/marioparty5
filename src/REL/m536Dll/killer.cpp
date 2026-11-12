#include "REL/m536Dll.h"

extern "C" {

extern void *lbl_1_bss_A00;
extern void *lbl_1_data_1FC[];

// One flying bullet: list node, PTMF hook, model, owner and kinematics.
struct M536Bullet {
    M536_NODE m_node;   /* 0x00 */
    M536_PTMF m_hook;   /* 0x08 */
    s16 m_mdl;          /* 0x14 */
    u8 m_active;        /* 0x16 */
    s32 m_idx;          /* 0x18 */
    s32 m_owner;        /* 0x1C */
    s32 m_timer;        /* 0x20 */
    Vec m_pos;          /* 0x24 */
    Vec m_vel;          /* 0x30 */
    f32 m_rotY;         /* 0x3C */
};

// The bullet list head is a bare node.
typedef M536_NODE M536BulletList;
typedef void (*M536BulletFn)(M536Bullet *);

// Per-player firing slot.
struct M536KillerSlot {
    u8 m_active;        /* 0x00 */
    s32 m_timer;        /* 0x04 */
};

// Singleton that owns the bullet model and the four firing slots.
struct M536Killer {
    M536_NODE m_node;           /* 0x00 */
    void **m_vptr;              /* 0x08 */
    s32 m_flag;                 /* 0x0C */
    s16 m_mdl;                  /* 0x10 */
    M536KillerSlot m_slot[4];   /* 0x14 */
};

s32 fn_1_B4B0(s32 owner, Vec *pos, f32 radius);

M536Killer *fn_1_E42C(M536Killer *self, s16 freeIt);
void fn_1_E650(M536Killer *self, s32 bit);
void fn_1_E508(M536Killer *self);
M536Bullet *fn_1_E678(M536Bullet *self, s16 *model, s32 idx);
s32 fn_1_E7BC(s32 owner, Vec *pos);
void fn_1_E8D4(void);
void fn_1_E9A8(void);
void fn_1_EA2C(M536Bullet *self, s32 owner, Vec *pos, f32 rotY);
u8 fn_1_EC28(s32 owner, Vec *pos, f32 rotY);
void fn_1_EE68(M536Bullet *self);
void fn_1_EF4C(M536Bullet *self);
void fn_1_F3CC(M536Bullet *self);
void fn_1_F4A8(M536Bullet *self);
void fn_1_F500(M536BulletList *list, M536BulletFn cb);
void fn_1_F560(M536Bullet *node, M536BulletList *stop, M536BulletFn cb);
void *fn_1_F618(void *self, s16 freeIt);

M536_PTMF lbl_1_data_5E4 = { 0, -1, (void *)fn_1_F4A8 };
M536_PTMF lbl_1_data_5F0 = { 0, -1, (void *)fn_1_F3CC };
M536_PTMF lbl_1_data_5FC = { 0, -1, (void *)fn_1_EF4C };
Vec lbl_1_data_608 = { 0.0f, 0.0f, 0.0f };
M536_PTMF lbl_1_data_614 = { 0, -1, (void *)fn_1_F4A8 };
M536_PTMF lbl_1_data_620 = { 0, -1, (void *)fn_1_F4A8 };
M536_PTMF lbl_1_data_62C = { 0, -1, (void *)fn_1_F4A8 };
M536_PTMF lbl_1_data_638 = { 0, -1, (void *)fn_1_F3CC };
M536_PTMF lbl_1_data_644 = { 0, -1, (void *)fn_1_F3CC };
M536_PTMF lbl_1_data_650 = { 0, -1, (void *)fn_1_F4A8 };
void *lbl_1_data_65C[5] = { 0, 0, (void *)fn_1_E42C, (void *)fn_1_E650, (void *)fn_1_E508 };

// Four corner posts the bullets bounce off (x, unused y, z).
M536_RODATA const Vec lbl_1_rodata_2C8[4] = {
    { 860.0f, 0.0f, -860.0f },
    { -860.0f, 0.0f, -860.0f },
    { 860.0f, 0.0f, 860.0f },
    { -860.0f, 0.0f, 860.0f },
};

u8 lbl_1_bss_AB0[12];
M536BulletList lbl_1_bss_ABC;
s8 lbl_1_bss_AC4;

M536Killer *fn_1_E24C(M536Killer *self)
{
    M536Killer *ms;
    M536Bullet *bullet;
    s32 i;
    s32 j;

    fn_1_1264(self);
    self->m_vptr = lbl_1_data_1FC;
    self->m_flag = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    if (lbl_1_bss_A00 != 0) {
        fn_1_1D4("ms_pointer == 0", "singleton.h", 0x35);
    }
    ms = (M536Killer *)((u8 *)self + 0x10);
    if (ms != 0) {
        ms = (M536Killer *)((u8 *)ms - 0x10);
    }
    lbl_1_bss_A00 = ms;
    self->m_vptr = lbl_1_data_65C;
    fn_1_2C94(&self->m_mdl, 0x5E000A);
    Hu3DModelAttrSet(self->m_mdl, 1);
    for (i = 0; i < 4; i++) {
        self->m_slot[i].m_active = 0;
        self->m_slot[i].m_timer = 0;
    }
    for (j = 0; j < 8; j++) {
        bullet = (M536Bullet *)fn_1_0(sizeof(M536Bullet));
        if (bullet != 0) {
            fn_1_E678(bullet, &self->m_mdl, j);
        }
    }
    return self;
}

M536Killer *fn_1_E3A0(M536Killer *self, s16 freeIt)
{
    if (self != 0) {
        if (lbl_1_bss_A00 == 0) {
            fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A00 = 0;
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M536Killer *fn_1_E42C(M536Killer *self, s16 freeIt)
{
    if (self != 0) {
        self->m_vptr = lbl_1_data_65C;
        fn_1_E8D4();
        fn_1_30F4(&self->m_mdl, -1);
        if ((M536Killer *)((u8 *)self + 0x10) != 0) {
            if (lbl_1_bss_A00 == 0) {
                fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
            }
            lbl_1_bss_A00 = 0;
        }
        if (self != 0) {
            self->m_vptr = lbl_1_data_1FC;
            if (self != 0) {
                fn_1_1270(self, 0);
            }
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_E508(M536Killer *self)
{
    M536KillerSlot *slot;
    s32 i;

    fn_1_E9A8();
    for (i = 0; i < 4; i++) {
        slot = &self->m_slot[i];
        if (slot->m_active != 0) {
            if ((f32)++slot->m_timer > 60.0f) {
                slot->m_active = 0;
            }
        }
    }
}

s32 fn_1_E5B8(M536Killer *self, s32 idx, Vec *pos, f32 rotY)
{
    M536KillerSlot *slot;

    slot = &self->m_slot[idx];
    if (slot->m_active == 1) {
        return 0;
    }
    if (fn_1_EC28(idx, pos, rotY) != 0) {
        slot->m_active = 1;
        slot->m_timer = 0;
        return 1;
    }
    return 0;
}

void fn_1_E650(M536Killer *self, s32 bit)
{
    self->m_flag &= ~(1 << bit);
    if (bit < 5) {
    }
}

M536Bullet *fn_1_E678(M536Bullet *self, s16 *model, s32 idx)
{
    M536_PTMF tmp;
    s16 mdl;

    fn_1_1264(self);
    fn_1_2DB4(&self->m_mdl, model);
    fn_1_1330(self, &lbl_1_bss_ABC);
    self->m_active = 0;
    self->m_idx = idx;
    mdl = self->m_mdl;
    Hu3DModelLayerSet(mdl, 1);
    Hu3DModelCameraSet(mdl, 1);
    Hu3DModelAttrSet(mdl, 1);
    tmp = lbl_1_data_5E4;
    self->m_hook = tmp;
    return self;
}

M536Bullet *fn_1_E764(M536Bullet *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

s32 fn_1_E7BC(s32 owner, Vec *pos)
{
    M536_PTMF tmp;
    M536Bullet *b;
    f32 dx;
    f32 dz;

    b = (M536Bullet *)lbl_1_bss_ABC.next;
    while (b != (M536Bullet *)&lbl_1_bss_ABC) {
        if (b->m_owner != owner && b->m_active == 1) {
            dx = b->m_pos.x - pos->x;
            dz = b->m_pos.z - pos->z;
            if ((dx * dx + dz * dz) < 2500.0f) {
                tmp = lbl_1_data_5F0;
                b->m_hook = tmp;
                return 1;
            }
        }
        b = (M536Bullet *)b->m_node.next;
    }
    return 0;
}

void fn_1_E8D4(void)
{
    M536_NODE *back;
    M536_NODE *back2;
    M536Bullet *bullet;

    while (M536ListEmpty(&lbl_1_bss_ABC) == 0) {
        back = lbl_1_bss_ABC.prev;
        back2 = back;
        bullet = (M536Bullet *)back2;
        if (bullet != 0) {
            fn_1_30F4(&bullet->m_mdl, -1);
            if (bullet != 0) {
                fn_1_1270(bullet, 0);
            }
            fn_1_C8(bullet);
        }
    }
}

void fn_1_E9A8(void)
{
    M536_NODE *first;
    M536_NODE *a;
    M536_NODE *b;

    fn_1_F500(&lbl_1_bss_ABC, fn_1_EE68);
    a = lbl_1_bss_ABC.next;
    b = a;
    first = b;
    if (first != &lbl_1_bss_ABC) {
        fn_1_12DC(first);
        fn_1_1330(first, &lbl_1_bss_ABC);
    }
}

void fn_1_EA2C(M536Bullet *self, s32 owner, Vec *pos, f32 rotY)
{
    M536_PTMF tmp;
    Mtx mtx;
    Vec rot;
    M536_EFFOBJ *eff;
    s16 mdl;
    s16 se;

    self->m_active = 1;
    self->m_owner = owner;
    self->m_timer = 0;
    self->m_pos.x = pos->x;
    self->m_pos.y = pos->y;
    self->m_pos.z = pos->z;
    self->m_rotY = rotY;
    tmp = lbl_1_data_5FC;
    self->m_hook = tmp;
    if (lbl_1_bss_AC4 == 0) {
        lbl_1_data_608.z = 25.0f;
        lbl_1_bss_AC4 = 1;
    }
    PSMTXRotRad(mtx, 'y', 0.017453292f * rotY);
    PSMTXMultVec(mtx, &lbl_1_data_608, &self->m_vel);
    rot.x = 0.0f;
    rot.y = self->m_rotY;
    rot.z = 0.0f;
    mdl = self->m_mdl;
    Hu3DModelAttrReset(mdl, 1);
    Hu3DModelRotSet(mdl, rot.x, rot.y, rot.z);
    Hu3DModelPosSet(mdl, self->m_pos.x, self->m_pos.y, self->m_pos.z);
    if (pos->x > 0.0f) {
        se = 0x7D3;
    } else {
        se = 0x7D2;
    }
    HuAudFXPlay(se);
    eff = (M536_EFFOBJ *)fn_1_0(sizeof(M536_EFFOBJ));
    if (eff != 0) {
        fn_1_7B3C(eff, &self->m_pos, rotY);
    }
}

u8 fn_1_EC28(s32 owner, Vec *pos, f32 rotY)
{
    M536Bullet *b;

    b = (M536Bullet *)lbl_1_bss_ABC.next;
    while (b != (M536Bullet *)&lbl_1_bss_ABC) {
        if (b->m_active == 0) {
            fn_1_EA2C(b, owner, pos, rotY);
            return 1;
        }
        b = (M536Bullet *)b->m_node.next;
    }
    return 0;
}

void fn_1_EE68(M536Bullet *self)
{
    Vec rot;
    s16 mdl;
    s16 id;
    f32 rotY;

    __ptmf_scall(self);
    rotY = self->m_rotY;
    rot.x = 0.0f;
    rot.y = rotY;
    rot.z = 0.0f;
    mdl = self->m_mdl;
    id = mdl;
    Hu3DModelRotSet(id, rot.x, rot.y, rot.z);
    Hu3DModelScaleSet(id, 0.7f, 0.7f, 0.7f);
    Hu3DModelPosSet(id, self->m_pos.x, self->m_pos.y, self->m_pos.z);
}

void fn_1_EF4C(M536Bullet *self)
{
    M536_PTMF t1;
    M536_PTMF t2;
    M536_PTMF t3;
    M536_PTMF t4;
    M536_PTMF t5;
    Vec prev;
    f32 x;
    f32 z;
    f32 ax;
    f32 az;
    f32 dx;
    f32 dz;
    s32 i;

    if ((f32)self->m_timer > 120.0f) {
        t1 = lbl_1_data_614;
        self->m_hook = t1;
    }
    self->m_pos.x = self->m_pos.x + self->m_vel.x;
    self->m_pos.y = self->m_pos.y + self->m_vel.y;
    self->m_pos.z = self->m_pos.z + self->m_vel.z;
    x = self->m_pos.x;
    if (x < 0.0f) {
        ax = -x;
    } else {
        ax = x;
    }
    if (ax > 1200.0f) {
        t2 = lbl_1_data_620;
        self->m_hook = t2;
    }
    z = self->m_pos.z;
    if (z < 0.0f) {
        az = -z;
    } else {
        az = z;
    }
    if (az > 1200.0f) {
        t3 = lbl_1_data_62C;
        self->m_hook = t3;
    }
    self->m_timer++;
    prev.x = self->m_pos.x - 5.0f * self->m_vel.x;
    prev.y = self->m_pos.y - 5.0f * self->m_vel.y;
    prev.z = self->m_pos.z - 5.0f * self->m_vel.z;
    if (fn_1_B4B0(self->m_owner, &self->m_pos, 5.0f) != 0 || fn_1_E7BC(self->m_owner, &self->m_pos) != 0) {
        t4 = lbl_1_data_638;
        self->m_hook = t4;
    }
    for (i = 0; i < 4; i++) {
        dx = self->m_pos.x - lbl_1_rodata_2C8[i].x;
        dz = self->m_pos.z - lbl_1_rodata_2C8[i].z;
        if ((dx * dx + dz * dz) < 75625.0f) {
            t5 = lbl_1_data_644;
            self->m_hook = t5;
            return;
        }
    }
}

void fn_1_F3CC(M536Bullet *self)
{
    M536_PTMF tmp;
    M536_EFFOBJ *eff;
    s16 se;

    eff = (M536_EFFOBJ *)fn_1_0(sizeof(M536_EFFOBJ));
    if (eff != 0) {
        fn_1_8098(eff, &self->m_pos);
    }
    self->m_active = 0;
    if (self->m_pos.x > 0.0f) {
        se = 0x7D3;
    } else {
        se = 0x7D2;
    }
    HuAudFXPlay(se);
    Hu3DModelAttrSet(self->m_mdl, 1);
    tmp = lbl_1_data_650;
    self->m_hook = tmp;
}

void fn_1_F4A8(M536Bullet *self)
{
    s16 mdl;
    s16 id;

    mdl = self->m_mdl;
    id = mdl;
    Hu3DModelAttrSet(id, 1);
    self->m_active = 0;
}

void fn_1_F500(M536BulletList *list, M536BulletFn cb)
{
    M536BulletList *head;
    M536Bullet *first;
    M536Bullet *start;

    head = list;
    first = (M536Bullet *)list->next;
    start = first;
    fn_1_F560(start, head, cb);
}

void fn_1_F560(M536Bullet *node, M536BulletList *stop, M536BulletFn cb)
{
    while (node != (M536Bullet *)stop) {
        cb(node);
        node = (M536Bullet *)node->m_node.next;
    }
}

void fn_1_F5C0(void)
{
    M536BulletList *list;
    M536BulletList *obj;

    list = &lbl_1_bss_ABC;
    fn_1_1264(list);
    obj = list;
    __register_global_object(obj, (void *)fn_1_F618, &lbl_1_bss_AB0);
}

void *fn_1_F618(void *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

}
