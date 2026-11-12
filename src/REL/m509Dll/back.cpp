#include "REL/m509Dll/m509.h"

extern "C" {
double cos(double x);
double sin(double x);
s32 irandom__7CRandomFv(void);
f32 randomMantissa__7CRandomFv(void);
void fn_1_3D30(s16 *model, u32 attr);
void fn_1_3FAC(s16 *model, u16 cam);
}

typedef struct BackItem {
    s32 idx;
    s16 *model;
    Vec pos;
    Vec vel;
    Vec rot;
    Vec rotVel;
    f32 scale;
} BackItem;

typedef struct Back {
    M509Base base;
    u8 mode;
    s16 model[3];
    BackItem item[3];
} Back;

typedef struct BackSky {
    M509Base base;
    f32 spin;
    f32 spinRot;
    Vec pos;
    s16 model;
} BackSky;

extern "C" {
void *fn_1_CED0(Back *self);
void fn_1_D05C(Back *self, BackItem *item);
void fn_1_D4B0(Back *self, BackItem *item);
void fn_1_D5A8(BackItem *item);
void fn_1_D704(Back *self, BackItem *item);
void fn_1_DA98(BackSky *self);
void fn_1_D004(Back *self);
void fn_1_D950(Back *self, s32 bit);
void *fn_1_D98C(BackSky *self);
void fn_1_DA70(BackSky *self);
void fn_1_DC64(BackSky *self, s32 bit);
void *fn_1_DC8C(BackSky *self, s16 flag);
void *fn_1_DD18(Back *self, s16 flag);
}

f32 lbl_1_data_3398[3] = { -2000.0f, -2700.0f, -3500.0f };
void *lbl_1_data_33A4[5] = { 0, 0, (void *)fn_1_DC8C, (void *)fn_1_DC64, (void *)fn_1_DA70 };
void *lbl_1_data_33B8[6] = { 0, 0, (void *)fn_1_DD18, (void *)fn_1_D950, (void *)fn_1_D004, 0 };


void *fn_1_CED0(Back *self)
{
    s32 i;

    fn_1_E48(self);
    self->base.vtbl = (void **)lbl_1_data_1C4;
    self->base.flags = 0;
    fn_1_F14(self, lbl_1_bss_1C);
    self->base.vtbl = lbl_1_data_33B8;
    fn_1_3BBC(&self->model[0], 0x430002);
    fn_1_3BBC(&self->model[1], 0x430003);
    fn_1_3BBC(&self->model[2], 0x430004);
    Hu3DModelLayerSet(self->model[0], 1);
    Hu3DModelLayerSet(self->model[1], 1);
    Hu3DModelLayerSet(self->model[2], 1);
    self->item[0].model = &self->model[0];
    self->item[1].model = &self->model[1];
    self->item[2].model = &self->model[2];
    self->mode = 0;
    for (i = 0; i < 3; i++) {
        self->item[i].idx = i;
        fn_1_D05C(self, &self->item[i]);
    }
    return self;
}

void fn_1_D004(Back *self)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        fn_1_D704(self, &self->item[i]);
    }
}

void fn_1_D05C(Back *self, BackItem *item)
{
    f32 px;
    f32 pz;
    f32 vx;
    f32 vz;
    f32 a;
    f32 b;
    f32 ry;
    s32 r = irandom__7CRandomFv();

    if (self->mode != 0) {
        r = (r & 1) + 2;
    } else {
        r = r & 3;
    }
    switch (r) {
    case 0:
        px = 100.0f * (15.0f * (randomMantissa__7CRandomFv() - 0.5f));
        pz = -2500.0f;
        vx = 20.0f * (randomMantissa__7CRandomFv() - 0.5f);
        vz = 10.0f + 20.0f * randomMantissa__7CRandomFv();
        break;
    case 1:
        px = 100.0f * (15.0f * (randomMantissa__7CRandomFv() - 0.5f));
        pz = 2500.0f;
        vx = 20.0f * (randomMantissa__7CRandomFv() - 0.5f);
        vz = -(20.0f * randomMantissa__7CRandomFv()) - 10.0f;
        break;
    case 2:
        px = -3000.0f;
        pz = 100.0f * (12.0f * (randomMantissa__7CRandomFv() - 0.5f));
        vx = 10.0f + 20.0f * randomMantissa__7CRandomFv();
        vz = 20.0f * (randomMantissa__7CRandomFv() - 0.5f);
        break;
    case 3:
        px = 3000.0f;
        pz = 100.0f * (12.0f * (randomMantissa__7CRandomFv() - 0.5f));
        vx = -(20.0f * randomMantissa__7CRandomFv()) - 10.0f;
        vz = 20.0f * (randomMantissa__7CRandomFv() - 0.5f);
        break;
    }
    item->scale = 0.0f;
    item->pos.x = px;
    item->pos.y = lbl_1_data_3398[item->idx];
    item->pos.z = pz;
    item->vel.x = vx;
    item->vel.y = 0.0f;
    item->vel.z = vz;
    ry = 3.141592653589793 * (60.0f * randomMantissa__7CRandomFv()) / 180.0;
    item->rot.x = 0.0f;
    item->rot.y = ry;
    item->rot.z = 0.0f;
    a = randomMantissa__7CRandomFv() - 0.5f;
    b = 180.0f * randomMantissa__7CRandomFv();
    item->rotVel.x = 0.0f;
    item->rotVel.y = b;
    item->rotVel.z = 5.0f * a;
}

void fn_1_D4B0(Back *self, BackItem *item)
{
    u8 out = 0;

    if (item->vel.x > 0.0f) {
        if (item->pos.x > 3000.0f) {
            out = 1;
        }
    } else if (item->pos.x < -3000.0f) {
        out = 1;
    }
    if (item->vel.z > 0.0f) {
        if (item->pos.z > 2500.0f) {
            out = 1;
        }
    } else if (item->pos.z < -2500.0f) {
        out = 1;
    }
    if (out != 0) {
        fn_1_D05C(self, item);
    }
}

void fn_1_D5A8(BackItem *item)
{
    Vec scale;
    f32 ang = item->rot.z + item->rotVel.z;
    s32 n = (s32)(182.04444444444445 * ang);

    n = (u16)(n + 0x8000) - 0x8000;
    item->rot.z = 0.0054931640625 * n;
    fn_1_3E64(item->model, &item->rot);
    item->scale += 0.1f;
    if (item->scale > 1.0f) {
        item->scale = 1.0f;
    }
    scale.z = item->scale;
    scale.y = item->scale;
    scale.x = item->scale;
    fn_1_3EAC(item->model, &scale);
    item->pos.x += item->vel.x;
    item->pos.z += item->vel.z;
    fn_1_3E1C(item->model, &item->pos);
}

void fn_1_D704(Back *self, BackItem *item)
{
    fn_1_D4B0(self, item);
    fn_1_D5A8(item);
}

void fn_1_D950(Back *self, s32 bit)
{
    self->base.flags &= ~(1 << bit);
    if (bit != 3) {
        if (bit < 3) {
            if (bit >= 0) {
            }
        } else if (bit < 5) {
        }
        return;
    }
    self->mode = 1;
}

void *fn_1_D98C(BackSky *self)
{
    s16 m;
    s32 mot;

    fn_1_E48(self);
    self->base.vtbl = (void **)lbl_1_data_1C4;
    self->base.flags = 0;
    fn_1_F14(self, lbl_1_bss_1C);
    self->base.vtbl = (void **)lbl_1_data_33A4;
    fn_1_3BBC(&self->model, 0x430005);
    self->spin = 0.0f;
    self->spinRot = 0.0f;
    m = self->model;
    mot = Hu3DMotionIDGet(m);
    Hu3DModelLayerSet(m, 1);
    Hu3DModelAttrSet(m, 0x40000001);
    Hu3DMotionSet(m, mot);
    return self;
}

void fn_1_DA70(BackSky *self)
{
    fn_1_DA98(self);
}

void fn_1_DA98(BackSky *self)
{
    Vec rot;
    f32 ang;

    self->spinRot += 1.0f;
    if (self->spinRot > 180.0f) {
        self->spinRot -= 360.0f;
    }
    rot.x = 0.0f;
    rot.y = self->spinRot;
    rot.z = 30.0f;
    fn_1_3E64(&self->model, &rot);
    self->spin += 0.1f;
    if (self->spin > 180.0f) {
        self->spin -= 360.0f;
    }
    ang = 3.141592653589793 * self->spin / 180.0;
    self->pos.x = 500.0 + 100.0 * (12.0 * cos(ang));
    self->pos.y = -1700.0f;
    self->pos.z = 100.0 * (12.0 * sin(ang)) - 200.0;
    fn_1_3E1C(&self->model, &self->pos);
}

void fn_1_DC64(BackSky *self, s32 bit)
{
    self->base.flags &= ~(1 << bit);
    switch (bit) {
    case 0:
    case 5:
        break;
    }
}

void *fn_1_DC8C(BackSky *self, s16 flag)
{
    if (self != 0) {
        self->base.vtbl = (void **)lbl_1_data_33A4;
        fn_1_401C(&self->model, -1);
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

void *fn_1_DD18(Back *self, s16 flag)
{
    if (self != 0) {
        self->base.vtbl = lbl_1_data_33B8;
        fn_1_401C(&self->model[2], -1);
        fn_1_401C(&self->model[1], -1);
        fn_1_401C(&self->model[0], -1);
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
